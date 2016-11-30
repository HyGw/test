/* //BBN_Linux/DEV/main/tclinux_phoenix/modules/private/tcphy/tcetherphy_7512.c */
/************************************************************************
*                E X T E R N A L   R E F E R E N C E S
**************************************************************************/

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
//#ifdef MII_INTERFACE
#include <linux/mii.h>
//#endif

#include <ecnt_hook/ecnt_hook_ether.h>

#include "tcetherphy_hook.h"

#include "tcetherphy.h"


#endif //LINUX_OS

#define tcMiiStationRead(phy_add, phy_reg) ETHER_MDIO_READ(phy_add, phy_reg)
#define tcMiiStationWrite(phy_add, phy_reg, phy_data) ETHER_MDIO_WRITE(phy_add, phy_reg, phy_data)
#define tc3262_gmac_tx(a,b) 0
#define tc3262_gmac_dev 

#define isEN7526c	(((VPint(0xbfb00064)&0xffff0000))==0x00080000)


/************************************************************************
*                         D A T A   T Y P E S
**************************************************************************/
#ifdef LINUX_OS
#define pause(x)					mdelay(x)
#endif //LINUX_OS

#ifdef TCPHY_DEBUG
#ifdef LINUX_OS
extern void setUserSpaceFlag(int num);
#endif //LINUX_OS

#ifndef LINUX_OS 					/*Linux platform is used sendicmp user-space to send icmp packet*/
typedef struct 
{
    ping_t  *ping_p;        		/* ping pointer         */
} pingEvt_t;
typedef struct 
{
    uint32  trace:1;
    uint32  echo:1;
    uint32  active:1;
    uint32  user:1;
    uint32  checkCmd:1;
    uint32  checkRsp:1;
} pingCmdFlag_t;
#define N_PELOG         20
typedef struct 
{
    mbuf_t* mbp;
    uint16  seq1;
    uint16  seq2;
    char    cc[4];
} peLog_t;
typedef struct 
{
    uint16  index;
    uint16  cnt;            		/* data drror counter       */
    peLog_t log[N_PELOG];
} peLogDcb_t;
typedef struct 
{
    pingCmdFlag_t flags;
    int actSocket;
    uint32  pingCnt;
    uint32  echoSeq;
    uint16  option;         		/* ping data option     */
    uint16  dataSeq;        		/* data sequence number     */
    char    dataChar;       		/* data character       */
    uint8   checkInd;       		/* data check indication    */
    peLogDcb_t* pelog_p;        	/* data log dcb         */
} pingCmdCb_t;
#endif // ndef LINUX_OS
#endif

struct ecnt_hook_ops ecnt_driver_ephy_op = {  // JasonG_7512
    .name = "driver_ephy_hook",
    .hookfn = ecnt_ephy_hook,
    .maintype = ECNT_ETHER_PHY,
    .is_execute = 1,
    .subtype = ECNT_DRIVER_API,
    .priority = 1
};


/************************************************************************
*                        Data for EPHY
**************************************************************************/

static uint8	ephy_addr_base = 0;		// replace  "mac_p->enetPhyAddr"
#ifdef TCPHY_SUPPORT
static uint8  tcPhyFlag = 0;
static uint8  tcPhyInitFlag = 0;
static uint8  tcPhyVer = 99; 					// default: unknown PhyVer
static uint8  fgMT7530_INT = 0;
static uint8  fgMT7510Ge_INT = 0;
static uint8  mt7530_flag = 0; 
static int eco_rev = 0x00;
static uint16  default_fe_l3r17 = 0x0000;   	// set value in init setting
static uint16  default_ge_dev7r3c = 0x0006; 	// set value in init setting
#define tcPhyVer_2031     	0  	// 9400, LEH
#define tcPhyVer_2101mb   	1  	// 9401, LEM
#define tcPhyVer_2104mc   	2  	// 9402, tc2206
#define tcPhyVer_2104sd   	3  	// 9403, tc2104sd
#define tcPhyVer_2101me   	4  	// 9404, 62UE
#define tcPhyVer_2102me   	5  	// 9405, tc3182
#define tcPhyVer_2104me   	6  	// 9406, tc2206F
#define tcPhyVer_2101mf   	7  	// 9407, FPGA(MF)
#define tcPhyVer_2105sg   	8  	// 9408, TC6501
#define tcPhyVer_2101mi   	10 	// 940a, RT63260
#define tcPhyVer_2105mj   	11 	// 940b, RT63365
#define tcPhyVer_2105sk   	12 	// 940c, TC6508
#define mtPhyVer_7530     	13 	// 9412, MT7530
#define tcPhyVer_2101mm   	14 	// 940e, MT7502 
#define tcPhyVer_mt7510FE 	15 	// 940f, MT7510, FE(Fast Ethernet)
#define mtPhyVer_7510Ge   	16 	// 9421, MT7510Ge      
#define tcPhyVer_mt7505FE 	17 	// 9411, MT7505, FE(Fast Ethernet)
#define tcPhyVer_7512Fe		18	// 9412, EN7512Fe, need to check R15(0xf)[13]=0: no Giga capability
#define tcPhyVer_7512Ge		19	// 9451, EN7512Ge

static uint8 tcPhyPortNum = 1; // set value in tcPhyVerLookUp

#define EPHY_ID_2031		0x9400
#define	EPHY_ID_2101mb		0x9401
#define	EPHY_ID_2104mc		0x9402
#define	EPHY_ID_2104sd		0x9403
#define	EPHY_ID_2101me		0x9404
#define	EPHY_ID_2102me		0x9405	
#define	EPHY_ID_2104me		0x9406
#define	EPHY_ID_2101mf		0x9407
#define	EPHY_ID_2105sg		0x9408
#define	EPHY_ID_2101mi		0x940a
#define	EPHY_ID_2105mj		0x940b
#define	EPHY_ID_2105sk		0x940c
#define	EPHY_ID_2101mm		0x940e
#define	EPHY_ID_7510Fe		0x940f
#define	EPHY_ID_7505Fe		0x9411
#define	EPHY_ID_7530Ge_7512Fe	0x9412
#define	EPHY_ID_7510Ge		0x9421
#define	EPHY_ID_7512Ge		0x9451

// for tcXXXX_link_state
#define ST_LINK_DOWN 	0
#define ST_LINK_DOWN2UP 1
#define ST_LINK_UP 		2
#define ST_LINK_UP2DOWN 3

static uint8 tcphy_link_state[TCPHY_PORTNUM];
static uint8 mtphy_link_state[MTPHY_PORTNUM];

// Auto, AN, 100F, 100H, 10F,  10H,
#ifdef TCPHY_1PORT
#define tcphy_speed_Auto  		0
#define tcphy_speed_ForceAN  	1
#define tcphy_speed_Force100F  	2
#define tcphy_speed_Force100H  	3
#define tcphy_speed_Force10F  	4
#define tcphy_speed_Force10H  	5
#endif

#ifdef TCPHY_1PORT
static uint8 tcphy_speed = tcphy_speed_Auto;
#endif
#endif 			// TCPHY_SUPPORT

#if defined( TCPHY_DEBUG) || defined(MTPHY_DEBUG)
// variable for doPing()
static uint32 PingReplyCnt = 0;
static uint8 phychkval_flag = 0;			// variable for doPhyChkVal()
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

extern void macSend(uint32 chanId, struct sk_buff *skb);

//V2.00 mark  #define PERIOD_PRINT
/************************************************************************
*            Variables for EPHY
**************************************************************************/
//*** TCPHY registers ***//
#ifdef TCPHY_SUPPORT
// type for register settings
typedef struct cfg_data_s
{
    uint32 	reg_num;
    uint32 	val;
}cfg_data_t;
typedef struct cfg_cL45data_s
{
    uint32 	dev_num;
    uint32 	reg_num;
    uint32 	val;
}cfg_cl45data_t;
typedef struct cfg_trdata_s
{
    char 	reg_typ[10];
    uint32 	reg_num;
    uint32 	val;
}cfg_trdata_t;
// variables for doPhyConfig()
static uint8 	sw_patch_flag = 1;
static uint8 	Skew_update_flag = 0;
static uint8 	EEE_DSP_Freeze_flag = 0;
static uint8 	VBuffer_1000M_flag = 0;
static uint32 	Skew_port_cnt = 0;
static uint8 	current_idx = 0; 				// default 0, Do NOT change 
static uint8 	sw_ErrOverMonitor_flag = 1; 	// default enable
#ifdef PERIOD_PRINT 							// V1.17
static uint8 	period_print_flag = 1; 			// default enable
static uint16 	period_print_cnt = 0; 			// counter
static uint16 	period_print_threshold = 240; 	// print message every 0.25s*240=60s
#endif
static uint8 	cfg_Tx10AmpSave_flag = 1; 	// default enable
static uint8 	cfg_LDPS_flag = 1; 			// default enable
//#if  defined(MT7510Ge_SUPPORT) || defined(SW_MT7530_SUPPORT)
static uint8 	sw_FixUp2DownFast_flag = 1;
//#endif
#endif 			// TCPHY_SUPPORT
#ifdef TCPHY_SUPPORT
// variables for doPhyDispFlag()
#ifdef TCPHY_DEBUG_DISP_LEVEL
uint8 tcPhy_disp_level = 2; 				// default level 2
#else
uint8 	tcPhy_disp_level = 0; 				// turn all message OFF for formal release
#endif
uint 	mtSkewCal_disp_level=0;
static uint8 	power_on_finish_flag = 0;  	//dai121019 indicate boot end
#ifdef LINUX_OS
#ifdef TCPHY_SUPPORT
//extern uint32 miiStationRead(uint32 phyaddr, uint32 phyReg);
//extern void   miiStationWrite(uint32 phyaddr, uint32 phyReg,uint32 miiData);
//extern void   tcMiiStationWrite_ext(uint32 enetPhyAddr,uint32 phyReg,uint32 miiData);
//extern uint32 tcMiiStationRead_ext(uint32 enetPhyAddr,uint32 phyReg);
//extern void   tcMiiExtStationWrite_ext(uint32 portAddr,uint32 devAddr,uint32 regAddr,uint32 miiData);
//extern uint32 tcMiiExtStationRead_ext(uint32 portAddr,uint32 devAddr, uint32 regAddr, uint8  op);


//extern uint32 	tcMiiStationRead(uint32 enetPhyAddr,uint32 phyReg);
//extern void   	tcMiiStationWrite(uint32 enetPhyAddr,uint32 phyReg,uint32 miiData);
extern int    	subcmd(const cmds_t tab[], int argc, char *argv[], void *p);
extern int    	cmd_register(cmds_t *cmds_p);
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
typedef struct 
{
    uint8 main_reset; 		// 15
    uint8 force_speed; 		// 13
    uint8 autoneg_enable; 	// 12
    uint8 powerdown; 		// 11
    uint8 force_duplex; 	// 8
} tcphy_mr0_reg_t;
typedef struct 
{
    uint16 value; 				// 15:0
    //uint8 autoneg_complete; 	// 5
    bool link_status; 			// 2
    bool link_status_prev;
} tcphy_mr1_reg_t;
typedef struct 
{
    uint8 selector_field; 	// 4:0
    uint8 able100F; 		// 8
    uint8 able100H; 		// 7
    uint8 able10F; 			// 6
    uint8 able10H; 			// 5
} tcphy_mr4_reg_t; // use for mr4 & mr5
typedef struct 
{
    uint8 selector_field; 	// 4:0
    uint8 able100F; 		// 8
    uint8 able100H; 		// 7
    uint8 able10F; 			// 6
    uint8 able10H; 			// 5
    uint8 LPNextAble;		// 15
} tcphy_mr5_reg_t; // use for mr4 & mr5
typedef struct 
{
    //uint8 parallel_detect_fault; 	// 4
    uint8 lp_np_able; 				// 3
    //uint8 np_able; 				// 2
    //uint8 lch_page_rx; 			// 1
    uint8 lp_autoneg_able; 			// 0
} tcphy_mr6_reg_t; // use for mr6
typedef struct 
{
    //uint8  slicer_err_thd; 	// 15:11
    uint16 err_over_cnt;   		// 10:0
    uint16 err_over_cnt_prev; 	// 10:0
} tcphy_l0r25_reg_t;
typedef struct 
{
    //uint8  slicer_err_thd; 	// 15:11
    uint16 err_over_cnt;   		// 10:0
    uint16 err_over_cnt_prev; 	// 10:0
} mtphy_errovcnt_reg_t;
typedef struct 
{
    uint8 lch_sig_detect;		// 15
    uint8 lch_rx_linkpulse;		// 14
    uint8 lch_linkup_100;		// 13
    uint8 lch_linkup_10;		// 12
    uint8 lch_linkup_mdi;		// 11
    uint8 lch_linkup_mdix;		// 10
    uint8 lch_descr_lock;		// 9
    uint8 mdix_status;			// 5
    uint8 tx_amp_save;			// 4:3
    uint8 final_duplex;			// 2
    uint8 final_speed;			// 1
    uint8 final_link;   		// 0
} tcphy_l0r28_reg_t;
typedef struct 
{
    uint8 lch_SignalDetect;			// 15
    uint8 lch_LinkPulse;			// 14
    uint8 lch_DescramblerLock1000;	// 13
	uint8 lch_DescramblerLock100;	// 12
    uint8 lch_LinkStatus1000_OK;	// 11
	uint8 lch_LinkStatus100_OK;		// 10
	uint8 lch_LinkStatus10_OK;		// 9 
	uint8 lch_MrPageRx;				// 8
	uint8 lch_MrAutonegComplete;	// 7
  	uint8 da_mdix;					// 6
  	uint8 FullDuplexEnable;			// 5
  	uint8 MSConfig1000;				// 4
  	uint8 final_speed_1000;			// 3 
  	uint8 final_speed_100;			// 2
  	uint8 final_speed_10;			// 1
} tcphy_1ErA2_reg_t;
typedef struct 
{
    uint8 lp_eee_10g;
    uint8 lp_eee_1000;
    uint8 lp_eee_100;  
} tcphy_l3r18_reg_t;
typedef struct 
{
    uint8 lp_eee_10g;
    uint8 lp_eee_1000;
    uint8 lp_eee_100;  
} tcphy_7r3D_reg_t;
tcphy_mr0_reg_t 	mr0;
tcphy_mr4_reg_t 	mr4; 
tcphy_mr5_reg_t 	mr5;
tcphy_mr6_reg_t 	mr6;
tcphy_l0r28_reg_t 	mr28; 		// L0R28
tcphy_l3r18_reg_t 	mrl3_18; 	// L3R17
tcphy_1ErA2_reg_t 	mr1E_A2;
tcphy_7r3D_reg_t 	mr7_3D;
// for multiple phy support
tcphy_mr1_reg_t 		Nmr1[TCPHY_PORTNUM];
tcphy_l0r25_reg_t 		Nmr25[TCPHY_PORTNUM];
mtphy_errovcnt_reg_t 	ErovcntA[MTPHY_PORTNUM];
mtphy_errovcnt_reg_t 	ErovcntB[MTPHY_PORTNUM];
mtphy_errovcnt_reg_t 	ErovcntC[MTPHY_PORTNUM];
mtphy_errovcnt_reg_t 	ErovcntD[MTPHY_PORTNUM];
// define for tc????ReadProbe()
#define ProbeZfgain  					0
#define ProbeAgccode 					1
#define ProbeBoosten 					2
#define ProbeSnr     					3
#define ProbeDcoff   					4
#define ProbeAdcoff  					5
#define ProbeAdcSign 					6
#define Probe_VgaState 					7
#define Probe_MSE 						8
#define Probe_MSE_ALL 					9
#define Probe_MSE_Sum 					10
#define Probe_Slicer_Err_Over_Sum 		11
#define Probe_Slicer_Err_Over_Sum_ALL 	12
#define Probe_MSE_Slicer_err_thres 		13
//bool tcphy_anen = ENABLE;
//bool tcphy_speed = ENABLE;
//bool tcphy_duplex = ENABLE;
// define for tc??LinkFailDetect()
#define TbtOrHbt 		0
#define HbtOnly 		1
#define TbtOnly 		2
#define TbtOrHbtOrGbt 	3
#endif

/************************************************************************
*
*            Common Functions for EPHY
*
**************************************************************************/
int 	ePhyInit(uint16 ephy_addr);
#ifdef TCPHY_SUPPORT
uint32 	tcPhyReadReg(uint8 port_num,uint8 reg_num);
void 	tcPhyWriteReg(uint8 port_num,uint8 reg_num,uint32 reg_data);
uint32 	tcPhyReadLReg(uint8 port_num,uint8 page_num,uint8 reg_num);
void 	tcPhyWriteLReg(uint8 port_num,uint8 page_num,uint8 reg_num,uint32 reg_data);
uint32 	tcPhyReadGReg(uint8 port_num,uint8 page_num,uint8 reg_num);
void 	tcPhyWriteGReg(uint8 port_num,uint8 page_num,uint8 reg_num,uint32 reg_data);  
#ifdef LINUX_OS 
uint32 	mtPhyReadGReg(uint32 port_num, uint32 dev_num, uint32 reg_num);
uint32 	mtPhyReadReg(uint8 port_num, uint8 reg_num);
int 	mtPhyMiiWrite_TrDbg(uint8 phyaddr, char *type, uint32 reg ,uint32 val, uint8 ch_num);
int32 	mtPhyMiiRead_TrDbg(uint8 phyaddr, char *type, uint32 reg , uint8 ch_num);
//void 	mtPhyChkVal (void);
void 	mtMiiRegWrite(uint32 port_num, uint32 reg_num, uint32 reg_data);
uint32 	mtMiiRegRead(uint8 port_num,uint8 reg_num);
uint32 	tcMiiExtStationRead_CL22(uint32 port_num, uint32 dev_num, uint32 reg_num);
void 	tcMiiExtStationWrite_CL22(uint32 port_num,uint32 dev_num,uint32 reg_num,uint32 reg_data);
void 	mtEMiiRegWrite(uint32 port_num, uint32 dev_num, uint32 reg_num, uint32 reg_data);
uint32 	mtEMiiRegRead(uint32 port_num, uint32 dev_num, uint32 reg_num);
#endif
void mtMiiRegWrite(uint32 port_num, uint32 reg_num, uint32 reg_data)
{
	tcMiiStationWrite(port_num, reg_num, reg_data);
}
uint32 mtMiiRegRead(uint8 port_num,uint8 reg_num)
{
	return(tcMiiStationRead(port_num, reg_num));
}
#if defined(TCSUPPORT_CT_LOOPDETECT) || !(!defined(TCSUPPORT_AUTOBENCH) || !defined(TCSUPPORT_CPU_MT7520))
EXPORT_SYMBOL(mtMiiRegWrite);
EXPORT_SYMBOL(mtMiiRegRead);
#endif
uint32 tcMiiExtStationRead_CL22(uint32 port_num, uint32 dev_num, uint32 reg_num)
{
    const uint16 MMD_Control_register=0xD;  	// 0xd=13
    const uint16 MMD_addr_data_register=0xE;	// 0xe=14	
  	const uint16 page_reg=31;
	uint32 value=0;
	tcMiiStationWrite(port_num, page_reg, 0x00); //switch to main page
	tcMiiStationWrite(port_num, MMD_Control_register, (0<<14)+dev_num);
	tcMiiStationWrite(port_num, MMD_addr_data_register, reg_num);
	tcMiiStationWrite(port_num, MMD_Control_register, (1<<14)+dev_num);
	value = tcMiiStationRead(port_num, MMD_addr_data_register);
	//printf("* doPhyMMDRead_CL22=>phyaddr=%d,  dev_addr=%d, data_addr=0x%04lX , value=0x%04lX\r\n", port_num, dev_num, reg_num, value);
	return(value); 	
}
uint32 mtEMiiRegRead(uint32 port_num, uint32 dev_num, uint32 reg_num)
{
	return(tcMiiExtStationRead_CL22( port_num, dev_num, reg_num));
}
void tcMiiExtStationWrite_CL22(uint32 port_num,uint32 dev_num,uint32 reg_num,uint32 reg_data)
{
    const uint16 MMD_Control_register=0xD;
    const uint16 MMD_addr_data_register=0xE;
  	const uint16 page_reg=31;
	tcMiiStationWrite(port_num, page_reg, 0x00); //switch to main page
	tcMiiStationWrite(port_num, MMD_Control_register, (0<<14)+dev_num);
	tcMiiStationWrite(port_num, MMD_addr_data_register, reg_num);
	tcMiiStationWrite(port_num, MMD_Control_register, (1<<14)+dev_num);
	tcMiiStationWrite(port_num, MMD_addr_data_register, reg_data);
	TCPHYDISP3("* doPhyMMDWrite_CL22=> phyaddr=%d, dev_addr=%d, data_addr=0x%04lX , value=0x%04lX\r\n", port_num, dev_num, reg_num, reg_data);
}
void mtEMiiRegWrite(uint32 port_num, uint32 dev_num, uint32 reg_num, uint32 reg_data)
{
	tcMiiExtStationWrite_CL22( port_num, dev_num, reg_num, reg_data);
}
#if defined(TCSUPPORT_AUTOBENCH) && defined(TCSUPPORT_CPU_MT7520)
EXPORT_SYMBOL(mtEMiiRegWrite);
EXPORT_SYMBOL(mtEMiiRegRead);
#endif
#ifdef LINUX_OS // allen_20130926 : merge 7502 & 7510/20

uint32 mtPhyReadReg(uint8 port_num, uint8 reg_num)  // for GEPHY
{
    uint32 val, val_r31;
    uint32 phyAddr;
	
	if(ephy_addr_base == 0) // allen_20140822
	{
		if((tcPhyVer == tcPhyVer_7512Ge)||(tcPhyVer == tcPhyVer_7512Fe))
			ephy_addr_base += 8;
	}
		
	phyAddr = ephy_addr_base + port_num;
    tcMiiStationWrite(phyAddr, 31, 0);
     
    if (tcPhyVer!=tcPhyVer_2031 && (reg_num<16 || reg_num==31))
	{     
        val = tcMiiStationRead(phyAddr, reg_num); 
        //printf("mtPhyReadReg:phyAddr =%d, reg_num =0x%02lX, val =0x%04lx\r\n",phyAddr,reg_num,val);
    }
    else
	{
        val_r31 = tcMiiStationRead(phyAddr, 31); // remember last page
        // set page to L0 if necessary
        if (val_r31 != 0x8000) 
		{
            tcMiiStationWrite(phyAddr, 31, 0x8000);
        }
        // read reg
        val = tcMiiStationRead(phyAddr, reg_num); 
        // restore page if necessary
        if (val_r31 != 0x8000) 
		{
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
        //if(Nmr1[port_num].link_status !=  Nmr1[port_num].link_status_prev)
		//{
	    //    printf("mtPhyReadReg case 1 :Nmr1[%d].value =0x%04lx\r\n", port_num,Nmr1[port_num].value);
    	//    printf("mtPhyReadReg case 1 :Nmr1[%d].link_status_prev =0x%04lx\r\n",port_num,Nmr1[port_num].link_status_prev );
        //	printf("mtPhyReadReg case 1 :Nmr1[%d].link_status =0x%04lx\r\n", port_num,Nmr1[port_num].link_status);
        //}
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
#endif

uint32 tcPhyReadReg(uint8 port_num, uint8 reg_num)  // for FEPHY
{
    uint32 val, val_r31;
    uint32 phyAddr;

	if(ephy_addr_base == 0) // allen_20140822
	{
		if((tcPhyVer == tcPhyVer_7512Ge)||(tcPhyVer == tcPhyVer_7512Fe))
			ephy_addr_base += 8;
	}
	phyAddr = ephy_addr_base + port_num;

    if (tcPhyVer!=tcPhyVer_2031 && (reg_num<16 || reg_num==31))
    {
        val = tcMiiStationRead(phyAddr, reg_num); 
    }
    else
    {
        val_r31 = tcMiiStationRead(phyAddr, 31); // remember last page
        // set page to L0 if necessary
        if (val_r31 != 0x8000) 
        {
            tcMiiStationWrite(phyAddr, 31, 0x8000);
        }
        // read reg
        val = tcMiiStationRead(phyAddr, reg_num); 
        // restore page if necessary
        if (val_r31 != 0x8000) 
        {
            tcMiiStationWrite(phyAddr, 31, val_r31);
        }
    }

    // update variables
    switch(reg_num)
    {    
    	case 0:
        	mr0.main_reset 		= (val>>15)&0x00000001;
        	mr0.force_speed    	= (val>>13)&0x00000001;
        	mr0.autoneg_enable 	= (val>>12)&0x00000001;
        	mr0.powerdown      	= (val>>11)&0x00000001;
        	mr0.force_duplex   	= (val>>8)&0x00000001;
        	break;
        
    	case 1:
        	//mr1.autoneg_complete = (val>>5)&0x00000001;       
        	//mr1_link_status_reg = val;
        	//mr1.value = val;
        	//mr1.link_status_prev = mr1.link_status; 
        	//mr1.link_status = (val>>2)&0x00000001;
        	Nmr1[port_num].value 			= val;
        	Nmr1[port_num].link_status_prev = Nmr1[port_num].link_status;
        	Nmr1[port_num].link_status 		= (val>>2)&0x00000001;
        	break;

    	case 4:
        	mr4.able100F 		= (val>>8)&0x0001;     
        	mr4.able100H 		= (val>>7)&0x0001;     
        	mr4.able10F 		= (val>>6)&0x0001;  
        	mr4.able10H 		= (val>>5)&0x0001;  
        	mr4.selector_field 	= (val)&0x001f;
        	break;

    	case 5:
        	mr5.able100F 		= (val>>8)&0x0001;     
        	mr5.able100H 		= (val>>7)&0x0001;     
        	mr5.able10F 		= (val>>6)&0x0001;  
        	mr5.able10H 		= (val>>5)&0x0001;  
        	mr5.selector_field 	= (val)&0x001f;
			mr5.LPNextAble 		= (val>>15)&0x0001;
        	break;
        
    	case 6:     
        	mr6.lp_np_able 		= (val>>3)&0x0001;
        	mr6.lp_autoneg_able = (val)&0x0001;
        	break;
        
	    case 25:           
        	Nmr25[port_num].err_over_cnt_prev 	= Nmr25[port_num].err_over_cnt;
        	Nmr25[port_num].err_over_cnt		= (val & 0x0000007ff);
        	break;
        
    	case 28:
        	mr28.lch_sig_detect  	= (val>>15)&0x0001;
        	mr28.lch_rx_linkpulse	= (val>>14)&0x0001;
        	mr28.lch_linkup_100  	= (val>>13)&0x0001;
        	mr28.lch_linkup_10   	= (val>>12)&0x0001;
        	mr28.lch_linkup_mdi  	= (val>>11)&0x0001; 	// after LEM
        	mr28.lch_linkup_mdix 	= (val>>10)&0x0001; 	// after LEM
        	mr28.lch_descr_lock  	= (val>>9)&0x0001; 		// after LEM
        	mr28.mdix_status  		= (val>>5)&0x0001; 		/* {0:mdi,1:mdix} */   
        	mr28.tx_amp_save  		= (val>>3)&0x0003; 		/* 0:100%, 1:90%, 2:80%, 3:70% */
        	mr28.final_duplex 		= (val>>2)&0x0001; 		/* {0:half-duplex, 1:full-duplex} */
        	mr28.final_speed  		= (val>>1)&0x0001; 		/* {0:10, 1:100} */
        	mr28.final_link			= (val)&0x0001; 		/* {0:linkdown, 1:linkup} */      
        	break;
        
    	default:
        	break;      
    }   
    return (val);
}

void tcPhyWriteReg(uint8 port_num,uint8 reg_num,uint32 reg_data)
{
    uint32 val_r31;
    uint32 phyAddr;

	if(ephy_addr_base == 0) // allen_20140822
	{
		if((tcPhyVer == tcPhyVer_7512Ge)||(tcPhyVer == tcPhyVer_7512Fe))
			ephy_addr_base += 8;
	}
	phyAddr = ephy_addr_base + port_num;

    val_r31 = tcMiiStationRead(phyAddr, 31); // remember last page
    // set page if necessary
    if (val_r31 != 0x8000) 
    {
        tcMiiStationWrite(phyAddr, 31, 0x8000); // page to L0
    }
    tcMiiStationWrite(phyAddr, reg_num, reg_data); 
    // restore page if necessary
    if (val_r31 != 0x8000) 
    {
        tcMiiStationWrite(phyAddr, 31, val_r31);
    }
}

// read Local Reg
uint32 tcPhyReadLReg(uint8 port_num,uint8 page_num,uint8 reg_num)
{
    uint32 val, val_r31;
    uint32 phyAddr;
    uint32 pageAddr = (page_num<<12)+0x8000;

	if(ephy_addr_base == 0) // allen_20140822
	{
		if((tcPhyVer == tcPhyVer_7512Ge)||(tcPhyVer == tcPhyVer_7512Fe))
			ephy_addr_base += 8;
	}
	phyAddr = ephy_addr_base + port_num;

    val_r31 = tcMiiStationRead(phyAddr, 31);  // remember last page
    // set page if necessary
    if (val_r31 != pageAddr) 
	{
        tcMiiStationWrite(phyAddr, 31, pageAddr); // switch to page Lx
    }
    val = tcMiiStationRead(phyAddr, reg_num); 
    // restore page if necessary
    if (val_r31 != pageAddr) 
	{
        tcMiiStationWrite(phyAddr, 31, val_r31);
    }
	
    if (page_num==3) 
	{
		switch(reg_num)
		{
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

// write Local Reg
void tcPhyWriteLReg(uint8 port_num,uint8 page_num,uint8 reg_num,uint32 reg_data)
{
    uint32 val_r31;
    uint32 phyAddr;
    uint32 pageAddr = (page_num<<12)+0x8000;

	if(ephy_addr_base == 0) // allen_20140822
	{
		if((tcPhyVer == tcPhyVer_7512Ge)||(tcPhyVer == tcPhyVer_7512Fe))
			ephy_addr_base += 8;
	}
	phyAddr = ephy_addr_base + port_num;

    val_r31 = tcMiiStationRead(phyAddr, 31);  // remember last page
    // set page if necessary
    if (val_r31 != pageAddr) 
	{
        tcMiiStationWrite(phyAddr, 31, pageAddr); // switch to page Lx  
    }
    tcMiiStationWrite(phyAddr, reg_num, reg_data); 
    // restore page if necessary
    if (val_r31 != pageAddr) 
	{
        tcMiiStationWrite(phyAddr, 31, val_r31);
    }
}

// read Global Reg
uint32 tcPhyReadGReg(uint8 port_num,uint8 page_num,uint8 reg_num)
{
    uint32 val, val_r31;
    uint32 phyAddr;
    uint32 pageAddr = (page_num<<12);

	if(ephy_addr_base == 0) // allen_20140822
	{
		if((tcPhyVer == tcPhyVer_7512Ge)||(tcPhyVer == tcPhyVer_7512Fe))
			ephy_addr_base += 8;
	}
	phyAddr = ephy_addr_base + port_num;

    val_r31 = tcMiiStationRead(phyAddr, 31);  // remember last page
    // set page if necessary
    if (val_r31 != pageAddr) 
	{
        tcMiiStationWrite(phyAddr, 31, pageAddr); // switch to page Gx  
    }
    val = tcMiiStationRead(phyAddr, reg_num); 
    // restore page if necessary
    if (val_r31 != pageAddr) 
	{
        tcMiiStationWrite(phyAddr, 31, val_r31);
    }
    
    return val;
}

// write Global Reg
void tcPhyWriteGReg(uint8 port_num,uint8 page_num,uint8 reg_num,uint32 reg_data)
{
    uint32 val_r31;
    uint32 phyAddr;
    uint32 pageAddr = (page_num<<12);

	if(ephy_addr_base == 0) // allen_20140822
	{
		if((tcPhyVer == tcPhyVer_7512Ge)||(tcPhyVer == tcPhyVer_7512Fe))
			ephy_addr_base += 8;
	}
	phyAddr = ephy_addr_base + port_num;

    val_r31 = tcMiiStationRead(phyAddr, 31);  // remember last page
    // set page if necessary
    if (val_r31 != pageAddr) 
	{
        tcMiiStationWrite(phyAddr, 31, pageAddr); // switch to page Gx
    }
    tcMiiStationWrite(phyAddr, reg_num, reg_data); 
    // restore page if necessary
    if (val_r31 != pageAddr) 
	{
        tcMiiStationWrite(phyAddr, 31, val_r31);
    }
}

#ifdef LINUX_OS // allen_20130926 : merge 7502 & 7510/20
uint32 mtPhyReadGReg(uint32 port_num, uint32 dev_num, uint32 reg_num)
{
    uint32 val;
    
    val = mtEMiiRegRead(port_num, dev_num, reg_num);

    // update variables
    switch(reg_num)
    {
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
        
	case 0x3d:
		mr7_3D.lp_eee_100=(val>>1)&0x0001;
		mr7_3D.lp_eee_1000=(val>>2)&0x0001;
		mr7_3D.lp_eee_10g=(val>>3)&0x0001;
		break;

		//printf("kant_7dev_3d = 0x%x \n\r", val);
		
    default:
        break;      
    }   
    return (val);
}
#endif
#endif		// TCPHY_SUPPORT
/************************************************************************
*
*            Variables and Functions for 7512 FE PHY (ID=0x9412, R15(0xf)[13]=0)
*
**************************************************************************/
#define MT7512FE_PHY_INIT_GDATA_LEN 	5	
#define MT7512FE_PHY_INIT_LDATA_LEN 	11
#define MT7512FE_PHY_INIT_PERPDATA_LEN 	1
#define MT7512FE_PHY_INIT_SET_NUM 		1
#define MT7512FE_PORTNUM 				4

typedef struct mt7512FE_cfg_data_s
{
    char 		name[10];
    cfg_data_t 	gdata[MT7512FE_PHY_INIT_GDATA_LEN];
    cfg_data_t 	ldata[MT7512FE_PHY_INIT_LDATA_LEN];
	cfg_data_t 	perpdata[MT7512FE_PHY_INIT_PERPDATA_LEN]; 	//per port register setting
}mt7512FE_cfg_data_t;

const uint32 mt7512FE_page_sel_addr = 31;
const uint16 mt7512FE_G0R22_Tx10AmpSave_ON  = 0x0264;
const uint16 mt7512FE_G0R22_Tx10AmpSave_OFF = 0x006F;
const uint16 mt7512FE_L2R16[MT7512FE_PORTNUM] = {0x0606, 0x0606, 0x0606, 0x0606, 0x0606};  		// allen_20130926 : FE R offset
const uint16 mt7512FE_L2R17_A1[MT7512FE_PORTNUM] = {0x0003, 0x0003, 0x0003, 0x0003, 0x0003};  	// 0002 // allen_20130926 : FE Tx gain
const uint16 mt7512FE_L3R17_A1[MT7512FE_PORTNUM] = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000}; 	// default_fe_l3r17
const uint16 mt7512FE_L0R30_E1[MT7512FE_PORTNUM] = {0x2200, 0x2200, 0x2200, 0x2200, 0x2200};	// allen_20160831
const uint16 mt7512FE_L0R30_E3[MT7512FE_PORTNUM] = {0x2220, 0x2220, 0x2220, 0x2220, 0x2220};	// allen_20160831

mt7512FE_cfg_data_t mt7512FE_cfg[MT7512FE_PHY_INIT_SET_NUM] = 
{
	{ 
		{"Fe_1.0"},
		{ 	/* global */
            /*
       		{31,0x2000},           	// g2
	   		{22,0x0092},
	   		{23,0x007d},
	   		{24,0x09b0},
	   		{25,0x0f6e},
	   		{26,0x0fc4},
	   		*/
        	{31,0x4000},           	// g4  // allen_20130926 : [5:4] reduce cr_a_tx_cmd_cld for 10Base 3th harmonic 
        	//{16,0x00cc}  
        	{26,0x8044},			// g4r26: tx bias (0x8444)
        	{31,0x5000},
        	{21,0x00ea},			// g5r21: z2p_ovs
        	{27,0x02f0}				// g5r27: z2n_ovs
     	},    
     	{  							// local
     		{31,0x8000}, 			// l0
     		{30,0xa000},			// l0r30: tx pwd : link down power saving
     		{31,0x9000},			// l1
     		{22,0xf000},			// l1r22: for intel82579 EEE IOT
     		{31,0xa000},			// l2
     		{22,0x4444},			// l2r22: ps_dri (0x3444)
     		{24,0x0c0c},			// l2r24: i2mpb_tbt
     		{28,0x7c44},			// l2r28: ps_op (0x6c44)
     		{30,0x0005},			// i2mpb_tbh_hbt [5:4]=00
			{31,0xb000},  			// l3
			{17,0x0000}   			//disable EEE
     	},
     	{ 
     		{31,0x8000}
	 	} 
    }, 
};

static int32 	mt7512FE_erroversum = 0;
static void 	mt7512FEConfig(int argc, char *argv[], void *p);
void 			mt7512FECfgLoad(uint8 idx, uint8 doPerPort_flag, uint8 phyaddr);
static void 	mt7512FELRCfgLoad(uint8 idx, uint8 port_num);
static void 	mt7512FECfgTx10AmpSave(void);
void 			mt7512FEGRCfgCheck(void);
void 			mt7512FELRCfgCheck(uint8 port_num);
void 			mt7512FECfgCheck(void);
int32 			mt7512FEReadProbe(uint8 port_num, uint8 mode);

#ifdef TCPHY_DEBUG
int32 		mt7512FEReadErrOverSum(uint8 port_num);
int32 		mt7512FEReadAdcSum(uint8 port_num);
void 		mt7512FEDispProbe100(uint8 port_num, bool long_msg);
void 		mt7512FESwDispMessage(uint8 port_num);
uint16 		mt7512FEReadSnrSum(uint8 port_num, uint16 cnt);
void 		mt7512FEUpdateErrOverSum(uint8 port_num);
void 		mt7512FESwErrOverMonitor(uint8 port_num);
#endif //TCPHY_DEBUG

void 		mt7512FESwPatch(void);
void 		doFEPhyALLAnalogCal(uint8 port_num); 				// for EN7512 
uint8 		allFEAnaCalWait(uint32 delay, uint8 port_num);		// for EN7512 
uint8 		allFEAnaCalWait_txamp(uint32 delay, uint8 port_num);		// for EN7512 // allen_20160616

void 		FECal_R50(uint8 port_num, uint32 delay);		// for EN7512 
void 		FECal_Tx_offset(uint8 port_num, uint32 delay);		// for EN7512 
void 		FECal_TxAmp(uint8 port_num, uint32 delay);			// for EN7512 
static uint8 	FECal_flag = 0;
static uint8 	FECal_tx_offset_flag = 0;
static uint8 	FECal_R50_flag = 0;



static void mt7512FEConfig(int argc, char *argv[], void *p)
{																// global config in doPhyConfig()
	#ifdef TCPHY_DEBUG
    if(argc==1) 
    {
        TCPHYDISP3("Usage: config Tx10AmpSave [on|off]\r\n");
        return;
    }
    else if(argc==2) 
    {
        if(stricmp(argv[1], "list") == 0)
        {
            TCPHYDISP3("config Tx10AmpSave : %s\r\n",
                    (cfg_Tx10AmpSave_flag ? "on" : "off" ));  
            return;
        }
    }
	#endif
    if(argc==2 || argc==3)
    {
        if(stricmp(argv[1], "Tx10AmpSave") == 0)
        {
            if(stricmp(argv[2], "on") == 0 )
            {
                cfg_Tx10AmpSave_flag = 1; 			// enable
                mt7512FECfgTx10AmpSave();
            }
            else if(stricmp(argv[2], "off") == 0 )
            {
                cfg_Tx10AmpSave_flag = 0;
                mt7512FECfgTx10AmpSave();
            }
            TCPHYDISP2("config Tx10AmpSave : %s\r\n",
                        (cfg_Tx10AmpSave_flag ? "on" : "off"));
        }
    }
}

// 1. loading default register setting in EPhyInit()
// 2. changing register setting
void mt7512FECfgLoad(uint8 idx, uint8 doPerPort_flag, uint8 phyaddr)
{
    int pn,i;
    uint16 port_num;
	current_idx = idx;
	
	if(ephy_addr_base == 0)
		ephy_addr_base += 8; 

	TCPHYDISP3("phyaddr = %d \r\n", phyaddr);

    for(i=0; i<MT7512FE_PHY_INIT_GDATA_LEN; i++ )
	{         
        tcMiiStationWrite(phyaddr, mt7512FE_cfg[current_idx].gdata[i].reg_num, mt7512FE_cfg[current_idx].gdata[i].val );
    }
    
    mt7512FECfgTx10AmpSave();
	
    // local registers 
    if(doPerPort_flag == DO_PER_PORT) 
	{
		port_num = phyaddr - ephy_addr_base;  
		mt7512FELRCfgLoad(current_idx, port_num);
    }
	else 
	{
		 for(pn=0; pn<MT7512FE_PORTNUM; pn++)
		 {
		 	mt7512FELRCfgLoad(current_idx, pn);
		}
	}
    // load revision-related settings

    TCPHYDISP4("tcphy: CfgLoad %s\r\n",  mt7512FE_cfg[current_idx].name);
}

static void mt7512FELRCfgLoad(uint8 idx, uint8 port_num)
{
    int i;
	current_idx = idx;

	if(ephy_addr_base == 0)   // allen_20140911
		ephy_addr_base += 8;
	
	for(i=0; i<MT7512FE_PHY_INIT_LDATA_LEN; i++ )
	{         
    	tcMiiStationWrite((port_num+ephy_addr_base), mt7512FE_cfg[current_idx].ldata[i].reg_num, mt7512FE_cfg[current_idx].ldata[i].val );
    }   

	/*tcPhyWriteLReg(port_num,2,16,mt7512FE_L2R16[port_num]); // allen_20130926 : FE R offset*/

	// load revision-related settings
	/*tcPhyWriteLReg(port_num,2,17,mt7512FE_L2R17_A1[port_num]);*/
	/*tcPhyWriteLReg(port_num,3,17,mt7512FE_L3R17_A1[port_num]);*/

	if(VPint(0xbfb0005c)>=3)	// allen_20160831
	{
		tcPhyWriteLReg(port_num, 0, 30, mt7512FE_L0R30_E3[port_num]);
	}
	else
	{
		tcPhyWriteLReg(port_num, 0, 30, mt7512FE_L0R30_E1[port_num]);
	}
	
	if(isEN7526c)	// allen_20160831
	{
		tcPhyWriteLReg(port_num, 0, 30, mt7512FE_L0R30_E1[port_num]);
	}
}

static void mt7512FECfgTx10AmpSave(void)
{
    uint16 phyAddr = ephy_addr_base;

    if(cfg_Tx10AmpSave_flag==1)
	{ 	// enable
        tcPhyWriteGReg(phyAddr,0,22,mt7512FE_G0R22_Tx10AmpSave_ON);
        TCPHYDISP4("tcphy: Tx10AmpSave enable!\r\n");       
    }
    else 
	{ 	// disable
        tcPhyWriteGReg(phyAddr,0,22,mt7512FE_G0R22_Tx10AmpSave_OFF);
        TCPHYDISP4("tcphy: Tx10AmpSave disable!\r\n");      
    }
}

void mt7512FEGRCfgCheck(void)
{
    int i;
	uint32 phyaddr = ephy_addr_base;
	uint32 rval;
	current_idx = 0; 
	
	for(i=0; i<MT7512FE_PHY_INIT_GDATA_LEN; i++)
	{
		if(mt7512FE_cfg[current_idx].gdata[i].reg_num == mt7512FE_page_sel_addr) 
		{
			tcMiiStationWrite(phyaddr, mt7512FE_cfg[current_idx].gdata[i].reg_num, mt7512FE_cfg[current_idx].gdata[i].val);
		}//if reg_num == page_sel_addr
		else 
		{
			rval = tcMiiStationRead(phyaddr, mt7512FE_cfg[current_idx].gdata[i].reg_num);
			if(rval!=mt7512FE_cfg[current_idx].gdata[i].val)
			{
				printf("Wrong initial setting of global register %d is detected!!\r\n",mt7512FE_cfg[current_idx].gdata[i].reg_num);
			} //rval==gdata
		}
	}
	printf("Check done!! \r\n");
}

void mt7512FELRCfgCheck(uint8 port_num)
{
	uint32 phyaddr = ephy_addr_base+port_num;
	uint32 rval;
}

void mt7512FECfgCheck(void)
{
     int pn;
	 mt7512FEGRCfgCheck();
	 
	 for(pn=0; pn<MT7512FE_PORTNUM; pn++)
	 {
	 	mt7512FELRCfgCheck(pn);
	 }
}

int32 mt7512FEReadProbe(uint8 port_num, uint8 mode)
{
    uint32 val, val_r31, val_g0r28;
    uint32 rval, wval;
    uint32 phyaddr = ephy_addr_base + port_num;
    uint32 testport= port_num;
	
    val_r31 = tcMiiStationRead(phyaddr, 31); 
    tcMiiStationWrite( phyaddr, 31, 0x0000 );
    val_g0r28 = tcMiiStationRead(phyaddr, 28);
	
    switch(mode)
    {
    	case ProbeZfgain:
            wval = 0x0b04 + testport;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
			rval = (val)&0x3f ;
            break;

        case ProbeAgccode:
            wval = 0x2e04 + testport;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val>>1)&0x1f ;
            break;

        case ProbeBoosten:   // boosten=1:high pass, for long, boosten=0:low pass, for short
            wval = 0x2e04 + testport;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val>>6)&0x01 ;
		    break;
        
        case ProbeSnr:
            wval = 0x0904 + testport;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val)&0xff ;
            break;  

        case ProbeAdcSign:
            wval = 0x4104 + testport;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val)&0x7f ;
            if (rval>64)
            {
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
int32 mt7512FEReadErrOverSum(uint8 port_num)
{
    int32 err_over_sum;
    
    tcPhyReadReg(port_num, 25);		// for FE
    pause(300);
    tcPhyReadReg(port_num, 25);		// for FE
    err_over_sum = Nmr25[port_num].err_over_cnt - Nmr25[port_num].err_over_cnt_prev;
    if(err_over_sum < 0 )
    {
        err_over_sum += 2048;
    }
    return err_over_sum;
}
#endif

#ifdef TCPHY_DEBUG
int32 mt7512FEReadAdcSum(uint8 port_num)
{
    int32 cnt=1000;
    int32 AdcSign_sum = 0;
    int j;
	uint32 val_g3r20, val_g3r20_newval, val_l0r30, val_l1r22;
	
    val_g3r20 = tcPhyReadGReg(port_num, 3, 20);
	val_g3r20_newval = (val_g3r20) & 0x7fff;
	tcPhyWriteGReg(port_num, 3, 20, val_g3r20_newval);//switch to full AD
	val_l0r30 = tcPhyReadLReg(port_num, 0, 30);
	tcPhyWriteLReg(port_num, 0, 30, 0x1510);//power down buffer
	val_l1r22 = tcPhyReadLReg(port_num, 1, 22);
	tcPhyWriteLReg(port_num, 1, 22, 0x000c);//force HP
	
    for(j=0;j<cnt;j++)
    {
        AdcSign_sum += mt7512FEReadProbe(port_num, ProbeAdcSign);
    }
    //shift right to show percent of the dc offset (unit:%)
    AdcSign_sum = (AdcSign_sum>>6);
	tcPhyWriteGReg(port_num, 3, 20, val_g3r20);
	tcPhyWriteLReg(port_num, 0, 30, val_l0r30);
	tcPhyWriteLReg(port_num, 1, 22, val_l1r22);
    return AdcSign_sum;
}
#endif

#ifdef TCPHY_DEBUG
uint16 mt7512FEReadSnrSum(uint8 port_num, uint16 cnt)
{
    uint16 snr_sum = 0;
    uint16 j;
    
    for(j=0; j<cnt; j++) 
    {
    	snr_sum += mt7512FEReadProbe(port_num,ProbeSnr);
    }
    return snr_sum;
}
#endif

#ifdef TCPHY_DEBUG
void mt7512FEDispProbe100(uint8 port_num, bool long_msg)
{
	uint32 phyaddr = ephy_addr_base + port_num;

    const uint16 mt7512FEReadSnrCnt = 1000;
	
    TCPHYDISP2("\r\n tcphy[%d]:",phyaddr);
    TCPHYDISP2(" boosten=%ld", mt7512FEReadProbe(port_num,ProbeBoosten));
    TCPHYDISP2(" agccode=%ld", mt7512FEReadProbe(port_num,ProbeAgccode));
    TCPHYDISP2(" zfgain=%ld", mt7512FEReadProbe(port_num,ProbeZfgain));
    TCPHYDISP2(" ch_idx=%ld", (tcPhyReadReg(port_num,29)&0x003f));		// for FE
    TCPHYDISP2(" snr=%ld", mt7512FEReadProbe(port_num,ProbeSnr));
    if (long_msg)
    {
        TCPHYDISP2(" err_over_sum=%ld", mt7512FEReadErrOverSum(port_num));
    }
    else 
	{
        tcPhyReadReg(port_num, 25);		// for FE
    }
    TCPHYDISP2(" \r\n");
	
    if (long_msg)
	{
        TCPHYDISP3(" snr_sum(x1000)=%d",mt7512FEReadSnrSum(port_num,mt7512FEReadSnrCnt)); // snr_sum   
        TCPHYDISP4(" adc_avg=%ld/1000", mt7512FEReadAdcSum(port_num));  
        TCPHYDISP3(" \r\n");
    }
}
#endif

#ifdef TCPHY_DEBUG
void mt7512FESwDispMessage(uint8 port_num)
{
    switch (tcphy_link_state[port_num]) 
    {
    	case ST_LINK_DOWN: 
            break;
            
        case ST_LINK_DOWN2UP: 
            tcPhyReadReg(port_num,28);		// for FE
            TCPHYDISP1("tcphy[%d]: Link-up at %d%s.",
                port_num,(mr28.final_speed ? 100:10),(mr28.final_duplex ? "F":"H")); 
			
			TCPHYDISP1(" tx_amp_save=%d.",(mr28.tx_amp_save));          
            TCPHYDISP1("\r\n");
			
            if(mr28.final_speed) 
            {
                mt7512FEDispProbe100(port_num,0); // short message
            }
            break;
            
        case ST_LINK_UP: 
            break;
            
        case ST_LINK_UP2DOWN:
            TCPHYDISP1("tcphy[%d]: Link-down!!!\r\n",port_num);
            break;
            
        default: 
        	printf("\r\ntcphy error: SwDispMessage error!\r\n");
    }
}
#endif

#ifdef TCPHY_DEBUG
void mt7512FEUpdateErrOverSum(uint8 port_num)
{
    uint16 err_over_cnt_prev; // to store previous err_over_cnt value
    int32 val;
    
    // clear Nmr25[port_num].err_over_cnt in tc2104meDispProbe100() in ST_LINK_DOWN2UP
    if((mr28.final_speed == 1) // 100BaseTX
        && (tcphy_link_state[port_num] == ST_LINK_UP)) 
    {
        err_over_cnt_prev = Nmr25[port_num].err_over_cnt;
        tcPhyReadReg(port_num,25);		// for FE
        val = Nmr25[port_num].err_over_cnt - err_over_cnt_prev;
        if(val < 0 )
        {
            val += 2048;
        }
        mt7512FE_erroversum = val;
    }
    else 
    {
        mt7512FE_erroversum = -1;
    }
}
#endif

#ifdef TCPHY_DEBUG
void mt7512FESwErrOverMonitor(uint8 port_num)
{
    if(tcPhy_disp_level < 3) 
        return; // inactive

    if(mt7512FE_erroversum > 0)
	{
        TCPHYDISP3("tcphy[%d]: ErrOver=%ld\r\n",port_num,mt7512FE_erroversum);          
    }
}
#endif

void mt7512FESwPatch(void)
{
    uint8 pn;

    for(pn=0; pn<MT7512FE_PORTNUM; pn++)
	{
        tcPhyReadReg(pn,1);					// for FE
        
        if(!Nmr1[pn].link_status_prev && !Nmr1[pn].link_status )
		{
            tcphy_link_state[pn] = ST_LINK_DOWN;
        }
        else if(!Nmr1[pn].link_status_prev && Nmr1[pn].link_status )
		{
            tcphy_link_state[pn] = ST_LINK_DOWN2UP;
        }
        else if(Nmr1[pn].link_status_prev && !Nmr1[pn].link_status )
		{
            tcphy_link_state[pn]= ST_LINK_UP2DOWN;
        }
        else 
		{ 	//if( Nmr1[pn].link_status_prev && Nmr1[pn].link_status ){
            tcphy_link_state[pn] = ST_LINK_UP;
        }
    
        // read registers
        tcPhyReadReg(pn,0);			// for FE
        tcPhyReadReg(pn,5);			// for FE
        tcPhyReadReg(pn,28);		// for FE
		tcPhyReadLReg(pn,3,18);		// for FE

        #ifdef TCPHY_DEBUG
        // display message
        mt7512FESwDispMessage(pn);
		#endif

		#ifdef TCPHY_DEBUG
		if(sw_ErrOverMonitor_flag)
			mt7512FEUpdateErrOverSum(pn);
        if(sw_ErrOverMonitor_flag) 
            mt7512FESwErrOverMonitor(pn); // call after mt7510FEUpdateErrOverSum()
		#endif
	}
}

#ifdef LINUX_OS // allen_20130926 : merge 7502 & 7510/20
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
    
  	if(stricmp(type, "DSPF") == 0)
  	{	// DSP Filter Debug Node
        ch_addr = 0x02;
		node_addr = 0x0D;
	}
	else if(stricmp(type, "PMA") == 0)
	{ 	// PMA Debug Node 
		ch_addr = 0x01;
		node_addr = 0x0F;
	}
	else if(stricmp(type, "TR") == 0)
	{ 	// Timing Recovery  Debug Node 
		ch_addr=0x01;
		node_addr=0x0D;
	}
	else if(stricmp(type, "PCS") == 0)
	{ 	// R1000PCS Debug Node 
		ch_addr=0x02;
		node_addr=0x0F;
	}
	else if(stricmp(type, "FFE") == 0)
	{ 	// FFE Debug Node 
		ch_addr=ch_num;
		node_addr=0x04;
	}
	else if(stricmp(type, "EC") == 0)
	{ 	// ECC Debug Node 
		ch_addr=ch_num;
		node_addr=0x00;
	}
	else if(stricmp(type, "ECT") == 0)
	{ 	// EC/Tail Debug Node 
		ch_addr=ch_num;
		node_addr=0x01;
	}
	else if(stricmp(type, "NC") == 0)
	{ 	// EC/NC Debug Node 
		ch_addr=ch_num;
		node_addr=0x01;
	}
	else if(stricmp(type, "DFEDC") == 0)
	{ 	// DFETail/DC Debug Node 
		ch_addr=ch_num;
		node_addr=0x05;
	}
	else if(stricmp(type, "DEC") == 0)
	{ 	// R1000DEC Debug Node 
		ch_addr=0x00; 
		node_addr=0x07;
	}
	else if(stricmp(type, "CRC") == 0)
	{ 	// R1000CRC Debug Node 
		ch_addr=ch_num;
		node_addr=0x06;
	}
	else if(stricmp(type, "AN") == 0)
	{ 	// Autoneg Debug Node 
		ch_addr=0x00; 
		node_addr=0x0F;
	}
	else if(stricmp(type, "CMI") == 0)
	{ 	// CMI Debug Node 
		ch_addr=0x03; 
		node_addr=0x0F;
	}
	else if(stricmp(type, "SUPV") == 0)
	{ 	// SUPV PHY  Debug Node 
		ch_addr=0x00; 
		node_addr=0x0D;
	}
	else
	{
    	return(0xFFFF);
		printf("Wrong TR register Type !");
	}
	data_addr=data_addr&0x3F;
      
	tcMiiStationWrite(phyaddr, page_reg, Token_Ring_debug_reg);
	tcMiiStationWrite(phyaddr, Token_Ring_Control_reg, (1<<15)|(1<<13)|(ch_addr<<11)|(node_addr<<7)|(data_addr<<1));

	//while(!(tcMiiStationRead(phyaddr, Token_Ring_Control_reg)&0x8000)); // data ready
	{
		value_low = tcMiiStationRead(phyaddr, Token_Ring_Low_data_reg);
		value_high = tcMiiStationRead(phyaddr, Token_Ring_High_data_reg);
		value=value_low+((value_high&0x00FF)<<16);
		TCPHYDISP4("*%s => Phyaddr=%d, ch_addr=%d, node_addr=0x%02lX, data_addr=0x%02lX , value=0x%08lX\r\n", type,phyaddr, ch_addr, node_addr, data_addr, value);
	}
	tcMiiStationWrite(phyaddr, page_reg, 0x00);//V1.11
		
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
	
	if(stricmp(type, "DSPF") == 0)
	{	// DSP Filter Debug Node
		ch_addr=0x02;
		node_addr=0x0D;
	}
	else if(stricmp(type, "PMA") == 0)
	{ 	// PMA Debug Node 
		ch_addr=0x01;
		node_addr=0x0F;
	}
	else if(stricmp(type, "TR") == 0)
	{ 	// Timing Recovery	Debug Node 
		ch_addr=0x01;
		node_addr=0x0D;
	}
	else if(stricmp(type, "PCS") == 0)
	{ 	// R1000PCS Debug Node 
		ch_addr=0x02;
		node_addr=0x0F;
	}
	else if(stricmp(type, "FFE") == 0)
	{ 	// FFE Debug Node 
		ch_addr=ch_num;
		node_addr=0x04;
	}
	else if(stricmp(type, "EC") == 0)
	{ 	// ECC Debug Node 
		ch_addr=ch_num;
		node_addr=0x00;
	}
	else if(stricmp(type, "ECT") == 0)
	{ 	// EC/Tail Debug Node 
		ch_addr=ch_num;
		node_addr=0x01;
	}
	else if(stricmp(type, "NC") == 0)
	{ 	// EC/NC Debug Node 
		ch_addr=ch_num;
		node_addr=0x01;
	}
	else if(stricmp(type, "DFEDC") == 0)
	{ 	// DFETail/DC Debug Node 
		ch_addr=ch_num;
		node_addr=0x05;
	}
	else if(stricmp(type, "DEC") == 0)
	{ 	// R1000DEC Debug Node 
		ch_addr=0x00; 
		node_addr=0x07;
	}
	else if(stricmp(type, "CRC") == 0)
	{ 	// R1000CRC Debug Node 
		ch_addr=ch_num;
		node_addr=0x06;
	}
	else if(stricmp(type, "AN") == 0)
	{ 	// Autoneg Debug Node 
		ch_addr=0x00; 
		node_addr=0x0F;
	}
	else if(stricmp(type, "CMI") == 0)
	{ 	// CMI Debug Node 
		ch_addr=0x03; 
		node_addr=0x0F;
	}
	else if(stricmp(type, "SUPV") == 0)
	{ 	// SUPV PHY  Debug Node 
		ch_addr=0x00; 
		node_addr=0x0D;
	}
	   
	data_addr=data_addr&0x3F;
	value_high=(0x00FF0000&value)>>16;
	value_low=(0x0000FFFF&value);
   
	tcMiiStationWrite(phyaddr, page_reg, Token_Ring_debug_reg);
		   
	tcMiiStationWrite(phyaddr, Token_Ring_Low_data_reg, value_low);
	tcMiiStationWrite(phyaddr, Token_Ring_High_data_reg, value_high);
	tcMiiStationWrite(phyaddr, Token_Ring_Control_reg, (1<<15)|(0<<13)|(ch_addr<<11)|(node_addr<<7)|(data_addr<<1));
	//while(!(tcMiiStationRead(phyaddr, Token_Ring_Control_reg)&0x8000)); 			// data ready
	{
		TCPHYDISP4("*%s => Phyaddr=%d, ch_addr=%d, node_addr=0x%02lX, data_addr=0x%02lX , value=0x%08lX\r\n", type,phyaddr, ch_addr, node_addr, data_addr, value);
	}
	tcMiiStationWrite(phyaddr, page_reg, 0x00);			//V1.11
	return 0;
}
#endif

/************************************************************************
*
*            Variables and Functions for 7530 GE PHY (ID=0x9412)
*
**************************************************************************/
#define MT7530_PORTNUM 					5
#define MT7530_PHY_INIT_LDATA_LEN 		1
#define MT7530_PHY_INIT_PERPDATA_LEN 	0
#define MT7530_PHY_INIT_SET_NUM 		1

#define MT7530_PHY_INIT_CL45_GDATA_LEN 		2	// allen_20150206	
#define MT7530_PHY_INIT_CL45_LDATA_LEN 		4	// allen_20150206
#define MT7530_PHY_INIT_CL45_PERPDATA_LEN 	0
#define MT7530_PHY_INIT_CL45_SET_NUM 		1

#define MT7530_PHY_INIT_TR_LDATA_LEN 		0
#define MT7530_PHY_INIT_TR_PERPDATA_LEN 	0
#define MT7530_PHY_INIT_TR_SET_NUM 			1

typedef struct mt7530_cfg_data_s
{
	char name[10];
	cfg_data_t ldata[MT7530_PHY_INIT_LDATA_LEN];
	cfg_data_t perpdata[MT7530_PHY_INIT_PERPDATA_LEN]; //per port register setting
}mt7530_cfg_data_t;

typedef struct mt7530_cfg_cl45data_s
{
	cfg_cl45data_t gdata[MT7530_PHY_INIT_CL45_GDATA_LEN];
	cfg_cl45data_t ldata[MT7530_PHY_INIT_CL45_LDATA_LEN];
	cfg_cl45data_t perpdata[MT7530_PHY_INIT_CL45_PERPDATA_LEN]; //per port register setting
}mt7530_cfg_cl45data_t;

typedef struct mt7530_cfg_trdata_s
{
	cfg_trdata_t ldata[MT7530_PHY_INIT_TR_LDATA_LEN];
	cfg_trdata_t perpdata[MT7530_PHY_INIT_PERPDATA_LEN]; //per port register setting
}mt7530_cfg_trdata_t;

const uint32 mt7530_page_sel_addr = 31;

mt7530_cfg_data_t mt7530_cfg[MT7530_PHY_INIT_SET_NUM] = 
{
    {
    	{"E4.0"},
	    //local data	
     	{ 	//{0x00,0x4000}
       		//for reference {0x1f,0x2A30},{0x0d,0x1002}, //TEST 0D//force txfil value at test page
        	#if 0  //E2 setting
			{0x1f,0x0003},{0x15,0x206a},{0x1a,0x0604},{0x1f,0x0000}
        	#endif
			{0x09, 0x1e00}	// master mode
		},
    	//per-port data 
     	{ 
	 	}
    }, 
};

mt7530_cfg_cl45data_t mt7530_cfg_cl45[MT7530_PHY_INIT_CL45_SET_NUM]=
{
    {	//globle data
     	{
			//for reference {0x1f,0x0032,0x0C24},// disable lpi_1000_pwr_seq_en
        	#if 0  //E2 setting
			{0x1f, 0x0103, 0x0020}, // invert clock of DFF for phase-mux data input //CML_20130223
			{0x1f, 0x0104, 0x0608}, // disable PLL //CML_20130321
			{0x1f, 0x0104, 0x2608}, // enable PLL //CML_20130321
			{0x1f, 0x0416, 0x0047}, // mcc_acc_ref_th setting //CML_20130327
			{0x1f, 0x027c, 0x0c0c}, // for 10BASE-T amplitude //CML_20130327
			{0x1f, 0x027b, 0x1177}  //disable 10BASE-TE
        	#endif
			{0x1f, 0x0417, 0x7775},  	// allen_20150206
			{0x1f, 0x027b, 0x1157}		// allen_20160803 for ASKEY request
		},
     	//local data
     	{
        	//for reference {0x1e,0x003D,0x0800},//modify Pair A DC for 100M
        	//{0x1e,0x00a6,0x0000} // disable TX power control //CML_20130321 //CML_20130327
			{0x1e,0x0045,0x0404},
			{0x1e,0x0046,0x0404},

			{0x07,0x003c,0x0000},
			{0x1e,0x00a6,0x0350}   
			#if 0 // keep E1 setting //CML_20130401
        	{0x1e,0x0001,0x0158}, // for 100BASE-T TX //CML_20130327
        	{0x1e,0x0003,0x0043}, // for 100BASE-T TX //CML_20130327
        	{0x1e,0x0004,0x0207}, // for 100BASE-T TX //CML_20130327
        	{0x1e,0x0007,0x0357}, // for 100BASE-T TX //CML_20130327
        	{0x1e,0x0008,0x02f0}, // for 100BASE-T TX //CML_20130327
        	{0x1e,0x000a,0x0006} // for 100BASE-T TX //CML_20130327
			#endif //CML_20130401

	        #if 0  //E2 setting
			{0x1e,0x0012,0x7210}, // for 100BASE-T TX //CML_20130401
			{0x1e,0x00a6,0x0300}, //disable power saving
			{0x1e,0x0120,0x8d0d},
			{0x1e,0x0122,0xffa0},
       	 	#endif
		},
	    //per-port data
     	{
	 	}
    }, 
};

mt7530_cfg_trdata_t mt7530_cfg_tr[MT7530_PHY_INIT_TR_SET_NUM]=
{
    {	//local data
     	{ 
			//for reference {"PMA",0x20,0x0700},
       		#if 0  //E2 setting
			{"DSPF",0x0010,0x005000},
        	{"DSPF",0x000e,0x800008}
       		#endif
		},
     	//per-port data
     	{ 	//{"PMA",0x20,0x0700}
        }
    }, 
};

static int32 MT7530GePhy_erroversum_A = 0;
static int32 MT7530GePhy_erroversum_B = 0;
static int32 MT7530GePhy_erroversum_C = 0;
static int32 MT7530GePhy_erroversum_D = 0;

static void mt7530GePhyConfig(int argc, char *argv[], void *p);
void 		mt7530GePhyCfgLoad(uint8 idx, uint8 doPerPort_flag, uint8 port_num);
static void mt7530GePhyCfgTx10AmpSave(void);
void 		mt7530GePhyGRCfgCheck(void);
void 		mt7530GePhyLRCfgCheck(uint8 port_num);
void 		mt7530GePhyCfgCheck(void);

#ifdef MTPHY_DEBUG
//ESD detect
static int16 mt7530GePhy_Up2Down_cnt_gbt[5] = {0,0,0,0,0};
static int16 mt7530GePhy_Up2Down_cnt_hbt[5] = {0,0,0,0,0};
static uint8 mt7530GePhy_Up2DownFastHbt_detect[5] = {0,0,0,0,0};
static uint8 mt7530GePhy_Up2DownFastgbt_detect[5] = {0,0,0,0,0};

static bool 	mt7530GePhy_link_fail_detect_flag = 0;
static uint8 	mt7530GePhy_reset_needed_perport[5] = {0,0,0,0,0};
static uint8 	mt7530GePhy_reset_needed_flag = 0;
static uint8 	mt7530GePhy_esdphy_init_flag = 1; // default: enable

//Disable EEE if link fail
static int16 	mt7530GePhy_Up2Down_cnt_tbt[5] = {0,0,0,0,0};
static uint8 	mt7530GePhy_Up2DownFastTbt_detect[5] = {0,0,0,0,0};
static uint8 	mt7530GePhy_linkup_check_timer[5] = {0,0,0,0,0};
static bool 	mt7530GePhyDisEEEHappened[5] = {0,0,0,0,0};
static uint8 	mt7530GePhy_linkup_check_cnt[5] = {0,0,0,0,0};
static uint8 	mt7530GePhyDisNextPage_cnt[5] = {0,0,0,0,0};
static bool 	mt7530GePhyDisNextPage_flag[5] = {0,0,0,0,0};
static uint8 	mt7530GePhyDisSlaveMode_cnt[5] = {0,0,0,0,0};  // allen_20140822
static bool 	mt7530GePhyDisSlaveMode_flag[5] = {0,0,0,0,0};
int32 	mt7530GePhyReadProbe(uint8 port_num, uint8 mode, uint8 ch_num);
int32 	mt7530GePhyReadCoeff(uint8 port_num, uint8 mode, uint8 ch_num);
void 	mt7530GePhyDbgPorbeSet(uint8 prb_num, uint8  port_num, uint16 flg_num, uint16 clk_num );
int32 	mt7530GePhyDbgPorbeRead(uint8 prb_num,uint8  port_num);
#endif

#ifdef MTPHY_DEBUG //CML_20130312
int32 	mt7530GePhyReadErrOverSum(uint8 port_num);
int32 	mt7530GePhyReadAdcSum(uint8 port_num);
void 	mt7530GePhyDispProbe100(uint8 port_num, bool long_msg);
void 	mt7530GePhySwDispMessage(uint8 port_num);
uint16	mt7530GePhyReadSnrSum(uint8 port_num, uint16 cnt);
void 	mt7530GePhyUpdateErrOverSum(uint8 port_num);
void 	mt7530GePhySwErrOverMonitor(uint8 port_num);
void 	mt7530GePhyUp2DownFastHbt(uint8 port_num);
void 	mt7530GePhyUp2DownFastTbt(uint8 port_num);
bool 	mt7530GePhyLinkFailDetect (uint8 port_num, uint8 mode);//0:10BT or 100BT, 1:100BT only, 2:10BT only,3:10BT or 100BT or 1000BT
void 	mt7530GePhyDisEEENeeded(uint8 port_num);
void 	mt7530GePhyDisNextPageNeeded(uint8 port_num);
#endif //TCPHY_DEBUG
void 	mt7530GePhySwPatch(void);

#ifdef MTPHY_DEBUG
void mt7530GePhyDbgPorbeSet(uint8 prb_num, uint8  port_num, uint16 flg_num, uint16 clk_num)
{
	uint16 value=0;

	#define dbg_flg_en 		1 
	#define dbg_DevAddr 	0x1F
	#define dbg_Flg0Ctrl 	0x15
	#define dbg_Flg1Ctrl 	0x16
	#define dbg_ClkAddr 	0x19
 
	if(prb_num == 2)
	{
		value = (port_num<<13)|(dbg_flg_en<<12)|(flg_num);
		TCPHYDISP1("Dbg Flag Register Set Value=%lx\r\n", value);
		mtEMiiRegWrite(port_num,dbg_DevAddr, dbg_Flg0Ctrl, value);
		mtEMiiRegWrite(port_num,dbg_DevAddr, dbg_Flg1Ctrl, value);
	}
	else if(prb_num == 0)
	{
		value = (port_num<<13)|(dbg_flg_en<<12)|(flg_num);
		TCPHYDISP1("Dbg Flag Register Set Value=%lx\r\n", value);
		mtEMiiRegWrite(port_num, dbg_DevAddr, dbg_Flg0Ctrl, value);
	}
	else if(prb_num == 1)
	{
		value = (port_num<<13)|(dbg_flg_en<<12)|(flg_num);
		TCPHYDISP1("Dbg Flag Register Set Value=%lx\r\n", value);
		mtEMiiRegWrite(port_num, dbg_DevAddr, dbg_Flg1Ctrl, value);
	}

    if(clk_num == 0xff)
    {	//disable clk output
		value = mtEMiiRegRead( port_num, dbg_DevAddr, dbg_ClkAddr);
		value = value & 0xEF;
		mtEMiiRegWrite(port_num, dbg_DevAddr, dbg_ClkAddr, value);
    }
	else
	{	//enable clk output
		value = (dbg_flg_en<<12)|(clk_num);
		mtEMiiRegWrite(port_num, dbg_DevAddr, dbg_ClkAddr, value);
	}
}

int32 mt7530GePhyDbgPorbeRead(uint8 prb_num,uint8  port_num)
{
	uint16 value=0;
	#define dbg_flg_en 		1 
	#define dbg_DevAddr 	0x1F
	#define dbg_Flg0Ctrl 	0x15
	#define dbg_Flg1Ctrl 	0x16
	#define dbg_ClkAddr 	0x19
	#define dbg_monAddr 	0x1A

	value = mtEMiiRegRead( port_num, dbg_DevAddr,dbg_monAddr);

	if(prb_num == 0)
		value = (value & 0x00FF);
	else if(prb_num == 1)
		value = (value>>8) & 0x00FF;
             	
	TCPHYDISP1("Dbg Flag[%d] Monitor Register Value=%lx\r\n", prb_num,value);
	return(value);      	
}

int32 mt7530GePhyReadProbe(uint8 port_num, uint8 mode, uint8 ch_num)
{
    uint32 val=0,val_A=0,val_B=0,val_C=0,val_D=0;
    uint32 avg_val=0, avg_A=0,avg_B=0,avg_C=0,avg_D=0;
    uint32 phyaddr = port_num;
    uint16 i;
	uint32 avg_cnt=1000;	
    //#define avg_cnt 1000
    tcMiiStationWrite(port_num, 31, 0);
    switch(mode)
    {
    	case Probe_VgaState:
    		val=mtPhyMiiRead_TrDbg(phyaddr,"PMA",0x38,0);
    		TCPHYDISP3("VgaStateA =0x%x\r\n",((val>>4) & 0x1F));
    		TCPHYDISP3("VgaStateB =0x%x\r\n",((val>>9) & 0x1F));
    		TCPHYDISP3("VgaStateC =0x%x\r\n",((val>>14) & 0x1F));
    		TCPHYDISP3("VgaStateD =0x%x\r\n",((val>>19) & 0x1F));
    		break;
    		
        case Probe_MSE:
        	switch(ch_num)
        	{
	            case 0:		//Pair A
            	  	val=mtEMiiRegRead(phyaddr,0x1E,0x9B);
			  		TCPHYDISP1("XX0 0x1E,0x9B =0x%x\r\n",val);
              		val =(val>>8) & 0xFF;  //V1.16
              		TCPHYDISP1("AA0 lch_mse_mdcA =0x%x\r\n",val);
              		break;

            	case 1:		//Pair B
              		val=mtEMiiRegRead(phyaddr,0x1E,0x9B);
			  		TCPHYDISP1("XX1 0x1E,0x9B =0x%x\r\n",val);
              		val =(val) & 0xFF;  //V1.16
              		TCPHYDISP1("AA1 lch_mse_mdcB =0x%x\r\n",val);
              		break;

            	case 2:		//Pair C
              		val=mtEMiiRegRead(phyaddr,0x1E,0x9C);
			  		TCPHYDISP1("XX2 0x1E,0x9C =0x%x\r\n",val);
              		val =(val>>8) & 0xFF;  //V1.16
              		TCPHYDISP1("AA2 lch_mse_mdcC =0x%x\r\n",val);
              		break;
              		
	            case 3:		//Pair D
            	  	val=mtEMiiRegRead(phyaddr,0x1E,0x9C);
			  		TCPHYDISP1("XX3 0x1E,0x9C =0x%x\r\n",val);
              		val =(val) & 0xFF;  //V1.16
              		TCPHYDISP1("AA3 lch_mse_mdcD =0x%x\r\n",val);
              		break;
            }
		    break;
		    
        case Probe_MSE_ALL:
			val=mtEMiiRegRead(phyaddr,0x1E,0x9B);
			val_A =(val>>8) & 0xFF;  //V1.16
			val_B =(val) & 0xFF;  //V1.16
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
			val=mtEMiiRegRead(phyaddr,0x1E,0x9D);
			val=(val>>11) & 0x1F; 
			TCPHYDISP3("slicer_err_thres =0x%x\r\n",val);
			break;
			
        case Probe_MSE_Sum:
			for(i=0;i<avg_cnt;i++)
			{
				val=mtEMiiRegRead(phyaddr,0x1E,0x9B);
				val_A =(val>>8) & 0xFF;  //V1.16
				val_B =(val) & 0xFF;  //V1.16
				avg_A = avg_A + val_A;
				avg_B = avg_B + val_B;

				val=mtEMiiRegRead(phyaddr,0x1E,0x9C);
				val_C =(val>>8) & 0xFF;  //V1.16
				val_D =(val) & 0xFF;  //V1.16
				avg_C = avg_C + val_C;
				avg_D = avg_D + val_D;

				val=(mtEMiiRegRead(phyaddr,0x1E,0x9D)>>11)&0x1F;

				avg_val = avg_val + val;
			}
	        avg_A = avg_A/1000;
          	avg_B = avg_B/1000;
          	avg_C = avg_C/1000;
          	avg_D = avg_D/1000;
          	avg_val = avg_val/1000;
          
          	val = avg_A + avg_B + avg_C + avg_D;
          
          	TCPHYDISP3("lch_mse_mdcA avg1000 =0x%x\r\n",avg_A);
          	TCPHYDISP3("lch_mse_mdcB avg1000 =0x%x\r\n",avg_B);
          	TCPHYDISP3("lch_mse_mdcC avg1000 =0x%x\r\n",avg_C);
          	TCPHYDISP3("lch_mse_mdcD avg1000 =0x%x\r\n",avg_D);
          	TCPHYDISP3("slicer_err_thres avg1000 =0x%x\r\n",avg_val);
          	break;

        case Probe_Slicer_Err_Over_Sum:  
        	switch(ch_num)
        	{
            	case 0:
              		val=mtEMiiRegRead(phyaddr,0x1E,0x9E);
              		ErovcntA[port_num].err_over_cnt=val;
              		TCPHYDISP3("ProbeSlicerErrOverSumA =0x%x\r\n",val);
              		break;

            	case 1:
              		val=mtEMiiRegRead(phyaddr,0x1E,0x9F);
              		ErovcntB[port_num].err_over_cnt=val;
              		TCPHYDISP3("ProbeSlicerErrOverSumB =0x%x\r\n",val);
              		break;
              		
            	case 2:
              		val=mtEMiiRegRead(phyaddr,0x1E,0xA0);
              		ErovcntC[port_num].err_over_cnt=val;
              		TCPHYDISP3("ProbeSlicerErrOverSumC =0x%x\r\n",val);
              		break;
              		
	            case 3:
              		val=mtEMiiRegRead(phyaddr,0x1E,0xA1);
              		ErovcntD[port_num].err_over_cnt=val;
              		TCPHYDISP3("ProbeSlicerErrOverSumD =0x%x\r\n",val);
              		break;
            }
          	break;

        	case Probe_Slicer_Err_Over_Sum_ALL:
              	val_A = mtEMiiRegRead(phyaddr,0x1E,0x9E);
              	TCPHYDISP3("ProbeSlicerErrOverSumA =0x%x\r\n",val_A);
              	val_B = mtEMiiRegRead(phyaddr,0x1E,0x9F);
              	TCPHYDISP3("ProbeSlicerErrOverSumB =0x%x\r\n",val_B);
              	val_C = mtEMiiRegRead(phyaddr,0x1E,0xA0);
              	TCPHYDISP3("ProbeSlicerErrOverSumC =0x%x\r\n",val_C);
              	val_D = mtEMiiRegRead(phyaddr,0x1E,0xA1);
              	TCPHYDISP3("ProbeSlicerErrOverSumD =0x%x\r\n",val_D);
              	val = val_A + val_B + val_C + val_D;
              	break;
  
        	default:
            	TCPHYDISP1("\r\ntcphy error: ReadProbe %d.\r\n",mode);
            	break;
    }
    return val;
}

void mt7530GePhySwDispMessage(uint8 port_num)
{
#ifdef PERIOD_PRINT //V1.17
    uint32 value;
#endif

	switch (mtphy_link_state[port_num]) 
	{
    	case ST_LINK_DOWN: 
      		// printf("Link Satus is ST_LINK_DOWN!\r\n");
            break;
            
        case ST_LINK_DOWN2UP: 
      		//  printf("Link Satus is ST_LINK_DOWN2UP!\r\n");
			if(mr1E_A2.final_speed_1000)
			{
				TCPHYDISP1("mtphy[%d]:Link-up at 1000 %s.\r\n",port_num, (mr1E_A2.FullDuplexEnable ?"F":"H"));
			}
			else if (mr1E_A2.final_speed_100)
			{
				TCPHYDISP1("mtphy[%d]:Link-up at 100%s.\r\n",port_num, (mr1E_A2.FullDuplexEnable ?"F":"H"));
			}
			else if (mr1E_A2.final_speed_10)
			{
				TCPHYDISP1("mtphy[%d]:Link-up at 10%s.\r\n",port_num, (mr1E_A2.FullDuplexEnable ?"F":"H"));
			}

			if(mr1E_A2.final_speed_1000 ||mr1E_A2.final_speed_100 ||mr1E_A2.final_speed_10)
			{
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
            
        default: 
        	printf("\r\nmtphy error: SwDispMessage error!\r\n");
    }
}

void mt7530GePhyUpdateErrOverSum(uint8 port_num)
{
    uint16 	err_over_cnt_prev_A; // to store previous err_over_cnt value
    uint16 	err_over_cnt_prev_B; // to store previous err_over_cnt value
    uint16 	err_over_cnt_prev_C; // to store previous err_over_cnt value
    uint16 	err_over_cnt_prev_D; // to store previous err_over_cnt value
    int32 	val_A, val_B, val_C, val_D;
    
    // clear Nmr25[port_num].err_over_cnt in tc2104meDispProbe100() in ST_LINK_DOWN2UP
    if (((mr1E_A2.final_speed_1000== 1) ||(mr1E_A2.final_speed_100 == 1)) // 1000/100BaseTX
        && (mtphy_link_state[port_num] == ST_LINK_UP)) 
    {
        err_over_cnt_prev_A = ErovcntA[port_num].err_over_cnt;
        err_over_cnt_prev_B = ErovcntB[port_num].err_over_cnt;
        err_over_cnt_prev_C = ErovcntC[port_num].err_over_cnt;
        err_over_cnt_prev_D = ErovcntD[port_num].err_over_cnt;
        
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
        
        if( val_A < 0 )
        {
            val_A += 2048;
        }

        if( val_B < 0 )
        {
            val_B += 2048;
        }

        if( val_C < 0 )
        {
            val_C += 2048;
        }

        if( val_D < 0 )
        {
            val_D += 2048;
        }
        
        MT7530GePhy_erroversum_A = val_A;
        MT7530GePhy_erroversum_B = val_B;
        MT7530GePhy_erroversum_C = val_C;
        MT7530GePhy_erroversum_D = val_D;
    }
    else 
    {
        MT7530GePhy_erroversum_A = -1;
        MT7530GePhy_erroversum_B = -1;
        MT7530GePhy_erroversum_C = -1;
        MT7530GePhy_erroversum_D = -1;
    }
}

void mt7530GePhySwErrOverMonitor(uint8 port_num)
{
    if(tcPhy_disp_level<3) 
        return; // inactive

    if(MT7530GePhy_erroversum_A>0)
	{
        TCPHYDISP3("tcphy[%d]: ErrOver A=%ld\r\n",port_num,MT7530GePhy_erroversum_A);          
    }
    
    if(MT7530GePhy_erroversum_B>0)
	{
        TCPHYDISP3("tcphy[%d]: ErrOver B=%ld\r\n",port_num,MT7530GePhy_erroversum_B);          
    }
    
    if(MT7530GePhy_erroversum_C>0)
	{
        TCPHYDISP3("tcphy[%d]: ErrOver C =%ld\r\n",port_num,MT7530GePhy_erroversum_C);          
    }
    
    if(MT7530GePhy_erroversum_D>0)
	{
        TCPHYDISP3("tcphy[%d]: ErrOver D=%ld\r\n",port_num,MT7530GePhy_erroversum_D);          
    }
}

void mt7530GePhyUp2DownFastgbt(uint8 port_num)
{
	const uint8 mt7530GePhy_Up2Down_gbt_Thd = 10;
	const uint8 mt7530GePhy_Up2Down_gbt_ub = 40;
	
    if(mr1E_A2.final_speed_1000 & !Nmr1[port_num].link_status) 
	{
		if(mt7530GePhy_Up2Down_cnt_gbt[port_num]<mt7530GePhy_Up2Down_gbt_ub) 
		{
			mt7530GePhy_Up2Down_cnt_gbt[port_num]+=3;
		}
    }
	else if(!mr1E_A2.final_speed_1000 & !Nmr1[port_num].link_status) 
	{
		if(mt7530GePhy_Up2Down_cnt_gbt[port_num]>0) 
		{
			mt7530GePhy_Up2Down_cnt_gbt[port_num]--;
		}
	}
	else if(mr1E_A2.final_speed_1000 & Nmr1[port_num].link_status) 
	{	//adjust link-up time to clear counter
		mt7530GePhy_Up2Down_cnt_gbt[port_num]=0;
	}
	
	if(mt7530GePhy_Up2Down_cnt_gbt[port_num]> mt7530GePhy_Up2Down_gbt_Thd) 
	{
	    mt7530GePhy_Up2Down_cnt_gbt[port_num]=0;
		mt7530GePhy_Up2DownFastgbt_detect[port_num]=1;
		#ifdef TCPHY_DEBUG
		TCPHYDISP4("set mt7530GePhy_Up2DownFastgbt_detect[%d] to 1.\r\n",port_num); 
		#endif
		//printf("set mt7530GePhy_Up2DownFastgbt_detect[%d] to 1.\r\n",port_num); 
	}
	else 
	{
		mt7530GePhy_Up2DownFastgbt_detect[port_num]=0;
	}  
}

void mt7530GePhyUp2DownFastHbt(uint8 port_num)
{
	const uint8 mt7530GePhy_Up2Down_hbt_Thd = 10;
	const uint8 mt7530GePhy_Up2Down_hbt_ub = 40;

    if(mr1E_A2.final_speed_100 & !Nmr1[port_num].link_status) 
	{
		if(mt7530GePhy_Up2Down_cnt_hbt[port_num] < mt7530GePhy_Up2Down_hbt_ub) 
		{
			mt7530GePhy_Up2Down_cnt_hbt[port_num]+=3;
		}
    }
	else if(!mr1E_A2.final_speed_100 & !Nmr1[port_num].link_status) 
	{
		if(mt7530GePhy_Up2Down_cnt_hbt[port_num]>0) 
		{
			mt7530GePhy_Up2Down_cnt_hbt[port_num]--;
		}
	}
	else if(mr1E_A2.final_speed_100 & Nmr1[port_num].link_status) 
	{	//adjust link-up time to clear counter
		mt7530GePhy_Up2Down_cnt_hbt[port_num]=0;
	}
	
	if(mt7530GePhy_Up2Down_cnt_hbt[port_num] > mt7530GePhy_Up2Down_hbt_Thd) 
	{
	    mt7530GePhy_Up2Down_cnt_hbt[port_num]=0;
		mt7530GePhy_Up2DownFastHbt_detect[port_num]=1;
		#ifdef TCPHY_DEBUG
		TCPHYDISP4("set mt7530GePhy_Up2DownFastHbt_detect[%d] to 1.\r\n",port_num); 
		#endif
		//printf("set mt7530GePhy_Up2DownFastHbt_detect[%d] to 1.\r\n",port_num); 
	}
	else 
	{
		mt7530GePhy_Up2DownFastHbt_detect[port_num]=0;
	}  
}

void mt7530GePhyUp2DownFastTbt(uint8 port_num)
{
	const uint8 mt7530GePhy_Up2Down_Tbt_Thd = 10;
	const uint8 mt7530GePhy_Up2Down_Tbt_ub = 40;
	const uint8 linkup_check_timer_done = 1;
	
    if(mr1E_A2.final_speed_10 & !Nmr1[port_num].link_status) 
	{
		mt7530GePhy_linkup_check_timer[port_num]=0;
		if(mt7530GePhy_Up2Down_cnt_tbt[port_num] < mt7530GePhy_Up2Down_Tbt_ub) 
		{
			mt7530GePhy_Up2Down_cnt_tbt[port_num]+=5;
		}
    }
	else if(!mr1E_A2.final_speed_10 & !Nmr1[port_num].link_status) 
	{
		mt7530GePhy_linkup_check_timer[port_num]=0;
		if(mt7530GePhy_Up2Down_cnt_tbt[port_num]>0) 
		{
			mt7530GePhy_Up2Down_cnt_tbt[port_num]--;
		}
	}
	else if(mr1E_A2.final_speed_10 & Nmr1[port_num].link_status ) 
	{
		if((mt7530GePhy_linkup_check_timer[port_num] == linkup_check_timer_done))
		{
			mt7530GePhy_Up2Down_cnt_tbt[port_num]=0;
			mt7530GePhy_linkup_check_timer[port_num]=0;
		}
		else if((mt7530GePhy_linkup_check_timer[port_num] < linkup_check_timer_done))
			mt7530GePhy_linkup_check_timer[port_num]++;
	}
	
	if(mt7530GePhy_Up2Down_cnt_tbt[port_num]> mt7530GePhy_Up2Down_Tbt_Thd) 
	{
	    mt7530GePhy_Up2Down_cnt_tbt[port_num]=0; 
		mt7530GePhy_Up2DownFastTbt_detect[port_num]=1;
		#ifdef TCPHY_DEBUG
		TCPHYDISP4("set mt7530GePhy_Up2DownFastTbt_detect[%d] to 1.\r\n",port_num); 
		#endif
		//printf("set mt7530GePhy_Up2DownFastTbt_detect[%d] to 1.\r\n",port_num);
	}
	else 
	{
		mt7530GePhy_Up2DownFastTbt_detect[port_num]=0;
	}
}

bool mt7530GePhyLinkFailDetect (uint8 port_num, uint8 mode)
{
	switch(mode)
	{
		case TbtOrHbtOrGbt:
			if(sw_FixUp2DownFast_flag 
				&(mt7530GePhy_Up2DownFastTbt_detect[port_num]
				||mt7530GePhy_Up2DownFastHbt_detect[port_num]||mt7530GePhy_Up2DownFastgbt_detect[port_num])) 
			{
				mt7530GePhy_link_fail_detect_flag =1;
		        #ifdef TCPHY_DEBUG
		        TCPHYDISP3("mt7530GePhy_link_fail_detect_flag(%d)=%d\r\n",port_num,mt7530GePhy_link_fail_detect_flag); 
		        #endif
			}
			else 
				mt7530GePhy_link_fail_detect_flag =0;
			break;
			
		case HbtOnly:
			if (sw_FixUp2DownFast_flag & mt7530GePhy_Up2DownFastHbt_detect[port_num]) 
			{
				mt7530GePhy_link_fail_detect_flag = 1;
		        #ifdef TCPHY_DEBUG
		        TCPHYDISP3("Hbt: mt7530GePhy_link_fail_detect_flag(%d)=%d\r\n",port_num,mt7530GePhy_link_fail_detect_flag); 
		        #endif
			}
			else 
				mt7530GePhy_link_fail_detect_flag = 0;
			break;
				
		case TbtOnly:
			if(sw_FixUp2DownFast_flag & mt7530GePhy_Up2DownFastTbt_detect[port_num]) 
			{
				mt7530GePhy_link_fail_detect_flag =1;
		        #ifdef TCPHY_DEBUG
		        TCPHYDISP3("Tbt: mt7530GePhy_link_fail_detect_flag(%d)=%d\r\n",port_num,mt7530GePhy_link_fail_detect_flag); 
		        #endif
			}
			else 
				mt7530GePhy_link_fail_detect_flag =0;
			break;

		default: 
			mt7530GePhy_link_fail_detect_flag =0;
	}
	return mt7530GePhy_link_fail_detect_flag;
}

void mt7530GePhyDisNextPageNeeded(uint8 port_num)
{
	const uint8 mt7530GePhyDisNextPage_cntdone = 5;
	const uint8 mt7530GePhyDisSlaveMode_cntdone = 3; // allen_20140822
   
	if((mtphy_link_state[port_num] == ST_LINK_UP)||(mt7530GePhyDisEEEHappened[port_num])) 
   	{
   		mt7530GePhyDisNextPage_cnt[port_num] = 0;
   	}
   	else if ((mr5.LPNextAble) && !(mr7_3D.lp_eee_100||mr7_3D.lp_eee_1000||mr7_3D.lp_eee_10g)
   		&& (mt7530GePhyDisNextPage_cnt[port_num]<=mt7530GePhyDisNextPage_cntdone)) 
   	{
   		mt7530GePhyDisNextPage_cnt[port_num]++;
   	}

	if(mt7530GePhyDisNextPage_cnt[port_num]>mt7530GePhyDisNextPage_cntdone) 
	{
   		mt7530GePhyDisNextPage_flag[port_num]=1;
		mt7530GePhyDisNextPage_cnt[port_num] = 0;
   	}
   	else 
	{
   		mt7530GePhyDisNextPage_flag[port_num]=0;
   	}

	// allen_20140822 === for gphy eee iot with realtek
	if((mtphy_link_state[port_num] != ST_LINK_UP) && (mtphy_link_state[port_num] != ST_LINK_DOWN) &&
		(mt7530GePhyDisSlaveMode_cnt[port_num] <= mt7530GePhyDisSlaveMode_cntdone))
	{
		mt7530GePhyDisSlaveMode_cnt[port_num]++;
	}

	if(mt7530GePhyDisSlaveMode_cnt[port_num] > mt7530GePhyDisSlaveMode_cntdone)
	{
		mt7530GePhyDisSlaveMode_flag[port_num] = 1;
		mt7530GePhyDisSlaveMode_cnt[port_num] = 0;
	}
	else
	{
		mt7530GePhyDisSlaveMode_flag[port_num] = 0;
	}
}

void mt7530GePhyDisEEENeeded(uint8 port_num)
{
    const uint8 mt7530GePhy_linkup_check_done = 1;
		
    if((tcphy_link_state[port_num] == ST_LINK_UP)
		&& (mt7530GePhy_linkup_check_cnt[port_num]<mt7530GePhy_linkup_check_done)) 
	{
		mt7530GePhy_linkup_check_cnt[port_num]++;
    }
	else if (tcphy_link_state[port_num] != ST_LINK_UP)
	{
		mt7530GePhy_linkup_check_cnt[port_num] = 0;
    }
	
    if(mt7530GePhyDisEEEHappened[port_num] && mt7530GePhy_linkup_check_cnt[port_num]==mt7530GePhy_linkup_check_done) 
	{	//extend link-up time
		mtEMiiRegWrite(port_num,0x07,0x3c,0x06);
		mt7530GePhyDisEEEHappened[port_num]=0;
    }
	else if(mt7530GePhyLinkFailDetect(port_num,3) || mt7530GePhyDisNextPage_flag[port_num]) 
	{
		mtEMiiRegWrite(port_num,0x07,0x3c,0x0000);//disable next page
		tcMiiStationWrite(0, 0x00, 0x1200);//re-start AN
		mt7530GePhyDisEEEHappened[port_num]=1;
    }
	//TCPHYDISP3("mt7530GePhy_linkup_check_cnt[%d]=%d\r\n",port_num,mt7530GePhy_linkup_check_cnt[port_num]);
	//TCPHYDISP3("mt7530GePhyDisEEEHappened[%d]=%d\r\n",port_num,mt7530GePhyDisEEEHappened[port_num]);
}

void mt7530GePhySwPatch(void)
{
    uint8 pn;
	uint32 reg0, reg1, reg9, reg10;
	
    if(power_on_finish_flag == 0)
    {
    	if((tcMiiStationRead(0, 0x09)&0xE000) == 0xE000)
      	{
        	power_on_finish_flag = 1;
        	tcMiiStationWrite(0, 0x09, 0x0700);
      	}
    }

    for(pn=0; pn<MT7530_PORTNUM; pn++)
	{
        mtPhyReadReg(pn,1);
		reg0 = tcMiiStationRead(pn, 0x0);
		reg1 = tcMiiStationRead(pn, 0x1);
		reg9 = tcMiiStationRead(pn, 0x9);
		reg10 = tcMiiStationRead(pn, 0xa);

        if( !Nmr1[pn].link_status_prev && !Nmr1[pn].link_status )
		{
            mtphy_link_state[pn] = ST_LINK_DOWN;
        }
        else if( !Nmr1[pn].link_status_prev && Nmr1[pn].link_status )
		{
            mtphy_link_state[pn] = ST_LINK_DOWN2UP;
        }
        else if( Nmr1[pn].link_status_prev && !Nmr1[pn].link_status )
		{
            mtphy_link_state[pn]= ST_LINK_UP2DOWN;
        }
        else 
		{ 	//if( Nmr1[pn].link_status_prev && Nmr1[pn].link_status ){
            mtphy_link_state[pn] = ST_LINK_UP;
        }
    
        mtPhyReadReg(pn,5);
        mtPhyReadGReg(pn,0x1E,0xA2);
		mtPhyReadGReg(pn,0x7,0x3d);
		//	tcPhyReadLReg(pn,3,18);//TBD

		if(((reg9>>9)&0x1)==1) // check 1000Mbps capbility  // allen_20140530
		{
			if((((reg10>>15)&0x1)==1)&&(((reg9>>12)&0x1)==1))  // local phy is slave && collision
			{
				tcMiiStationWrite(pn, 0x09, 0x0600); // disable manual 
				tcMiiStationWrite(pn, 0x00, 0x1200); // re-start AN
			}
			mt7530GePhyDisNextPageNeeded(pn);
			mt7530GePhyUp2DownFastgbt(pn);
			mt7530GePhyUp2DownFastHbt(pn);
			mt7530GePhyUp2DownFastTbt(pn);
			mt7530GePhyDisEEENeeded(pn);

			if((mtphy_link_state[pn] != ST_LINK_UP) && (mt7530GePhyDisSlaveMode_flag[pn] == 1) && (((reg9>>12)&0x1)==1)) // allen_20140822
			{
				tcMiiStationWrite(pn, 0x09, 0x0600); // disable manual 
				tcMiiStationWrite(pn, 0x00, 0x1200); // re-start AN
				mt7530GePhyDisSlaveMode_cnt[pn] = 0;
				mt7530GePhyDisSlaveMode_flag[pn] = 0;
			}
			else if(mtphy_link_state[pn] == ST_LINK_UP) // link up
			{
				tcMiiStationWrite(pn, 0x09, 0x1e00); // master 
				mt7530GePhyDisSlaveMode_cnt[pn] = 0;
				mt7530GePhyDisSlaveMode_flag[pn] = 0;
			}
		}

		#ifdef MTPHY_DEBUG
		// display message
		mt7530GePhySwDispMessage(pn);
		#endif

		#ifdef MTPHY_DEBUG
		if (sw_ErrOverMonitor_flag)
			mt7530GePhyUpdateErrOverSum(pn);
		if (sw_ErrOverMonitor_flag) 
			mt7530GePhySwErrOverMonitor(pn); // call after mt7510FEUpdateErrOverSum()
		#endif
    }
}
#endif

void mt7530GePhyCfgLoad(uint8 idx, uint8 doPerPort_flag, uint8 port_num)
{
	int pn, i;
	uint16 phyAddr;
	current_idx = idx;

    // global registers
    phyAddr = port_num; //ephy_addr_base;
	for(pn=0; pn<MTPHY_PORTNUM; pn++)
	{
		phyAddr = pn; //ephy_addr_base + pn;

		for( i=0; i<MT7530_PHY_INIT_CL45_GDATA_LEN; i++ )
		{         
			mtEMiiRegWrite(phyAddr, mt7530_cfg_cl45[current_idx].gdata[i].dev_num, mt7530_cfg_cl45[current_idx].gdata[i].reg_num, mt7530_cfg_cl45[current_idx].gdata[i].val);
		}
		
		for( i=0; i<MT7530_PHY_INIT_CL45_LDATA_LEN; i++ )
		{         
			mtEMiiRegWrite(phyAddr, mt7530_cfg_cl45[current_idx].ldata[i].dev_num, mt7530_cfg_cl45[current_idx].ldata[i].reg_num, mt7530_cfg_cl45[current_idx].ldata[i].val);
		}
		
		for( i=0; i<MT7530_PHY_INIT_LDATA_LEN; i++ )
		{         
			tcMiiStationWrite(phyAddr, mt7530_cfg[current_idx].ldata[i].reg_num, mt7530_cfg[current_idx].ldata[i].val);
		}
		
		for( i=0; i<MT7530_PHY_INIT_TR_LDATA_LEN; i++ )
		{         
			mtPhyMiiWrite_TrDbg( phyAddr, mt7530_cfg_tr[current_idx].ldata[i].reg_typ, mt7530_cfg_tr[current_idx].ldata[i].reg_num, mt7530_cfg_tr[current_idx].ldata[i].val,0);
		}
		//mtEMiiRegWrite(phyAddr, 0x1e, 0x12, mt7530_dev1Ehreg12h_A2[phyAddr]); // for 100BASE-T TX //CML_20130327 //CML_20130401
	}  
    TCPHYDISP4("mtphy: CfgLoad %s\r\n",  mt7530_cfg[current_idx].name);
}
//#endif //SW_MT7530_SUPPORT

/************************************************************************
*
*            Variables and Functions for 7512 GE PHY (ID=0x9451)
*
**************************************************************************/
#ifdef LINUX_OS 
#define MT7512Ge_PORTNUM 				1
#define MT7512Ge_PHY_INIT_LDATA_LEN 	3
#define MT7512Ge_PHY_INIT_PERPDATA_LEN 	0
#define MT7512Ge_INIT_SET_NUM 			1
#define MT7512Ge_PHY_INIT_SET_NUM 		1

#define MT7512Ge_PHY_INIT_CL45_GDATA_LEN 	10 	
#define MT7512Ge_PHY_INIT_CL45_LDATA_LEN 	20 	 
#define MT7512Ge_PHY_INIT_CL45_PERPDATA_LEN 0
#define MT7512Ge_PHY_INIT_CL45_SET_NUM 		1

#define MT7512Ge_PHY_INIT_TR_LDATA_LEN 		1
#define MT7512Ge_PHY_INIT_TR_PERPDATA_LEN 	1
#define MT7512Ge_PHY_INIT_TR_SET_NUM 		1

typedef struct mt7512Ge_cfg_data_s
{
	char 		name[10];
	cfg_data_t 	ldata[MT7512Ge_PHY_INIT_LDATA_LEN];
	cfg_data_t 	perpdata[MT7512Ge_PHY_INIT_PERPDATA_LEN]; //per port register setting	
}mt7512Ge_cfg_data_t;

typedef struct mt7512Ge_cfg_cl45data_s
{
	cfg_cl45data_t gdata[MT7512Ge_PHY_INIT_CL45_GDATA_LEN];
	cfg_cl45data_t ldata[MT7512Ge_PHY_INIT_CL45_LDATA_LEN];
	cfg_cl45data_t perpdata[MT7512Ge_PHY_INIT_CL45_PERPDATA_LEN]; //per port register setting	
}mt7512Ge_cfg_cl45data_t;

typedef struct mt7512Ge_cfg_trdata_s
{
	cfg_trdata_t ldata[MT7512Ge_PHY_INIT_TR_LDATA_LEN];
	cfg_trdata_t perpdata[MT7512Ge_PHY_INIT_PERPDATA_LEN]; //per port register setting	
}mt7512Ge_cfg_trdata_t;

const uint32 mt7512Ge_page_sel_addr = 31;

mt7512Ge_cfg_data_t mt7512Ge_cfg[MT7512Ge_PHY_INIT_SET_NUM]=
{
	{
		{"Ge_1.0"}, 
    	//local data	
     	{ 	//{0x00,0x4000}
       		//for reference {0x1f,0x2A30},{0x0d,0x1002}, //TEST 0D//force txfil value at test page
       		//{0x1f,0x0003}, // allen_20140530, lpi (21=0x15, 26=0x1a)
       		//{0x15,0x206a},
       		//{0x1a,0x0604}
       		{0x09, 0x1e00},
       		{0x1f, 0x0001},
       		{0x14, 0x3a14}    // allen_20160317
     	},
     	
    	//per-port data 
	    { //{0x00,0x4000}
	 	}
    },
};

mt7512Ge_cfg_cl45data_t mt7512Ge_cfg_cl45[MT7512Ge_PHY_INIT_CL45_SET_NUM]=
{
    {	//globle data
     	{
			//for reference {0x1f,0x0032,0x0C24},// disable lpi_1000_pwr_seq_en
			//{0x1f, 0x0103, 0x0020}, // invert clock of DFF for phase-mux data input //CML_20130223
			//{0x1f, 0x0104, 0x0608}, // disable PLL //CML_20130321
			//{0x1f, 0x0104, 0x2608}, // enable PLL //CML_20130321
			//{0x1f, 0x0416, 0x0047}, // mcc_acc_ref_th setting //CML_20130327
			/*{0x1f, 0x027c, 0x0c0c}, // for 10BASE-T amplitude //CML_20130327*/
			/*{0x1f, 0x0021, 0x800a}, // allen_20131120*/
			/*{0x1f, 0x0024, 0xc007}, // allen_20131120*/
			/*{0x1f, 0x0025, 0x003f},  // allen_20131120*/
			{0x1f, 0x0273, 0x1000}, 	// i2mpb_tbh_ots
			{0x1f, 0x0272, 0x3cff},		// ps_op
			{0x1f, 0x0269, 0x4344},		// ps_dri
			{0x1f, 0x0044, 0x0030},		// pair delay (digital)

			{0x1f, 0x027c, 0x0808},		// 10 base Tx
			{0x1f, 0x027b, 0x1177},		// disable 10 base-Te
			
			{0x1f, 0x0417, 0x7775},
			{0x1f, 0x0024, 0xc007},		// gphy led
			{0x1f, 0x0025, 0x003f},		// gphy led
			{0x1f, 0x0021, 0x800a}		// gphy led
			
			//{0x1f, 0x027b, 0x1177}   // allen_20140530 (10baseT Tx gain for IOT, if tx no EEE, rx only)
     	},
     	
     	//local data
     	{
        	//for reference {0x1e,0x003D,0x0800},//modify Pair A DC for 100M
        	//{0x1e,0x00a6,0x0000} // disable TX power control //CML_20130321 //CML_20130327
    		//{0x7,0x003c,0x0000},  //disable EEE mode  // allen_20130926 : GePhy: IOT, wait 7511 E4 ECO
			#if 0 // keep E1 setting //CML_20130401
        	{0x1e,0x0001,0x0158}, // for 100BASE-T TX //CML_20130327
        	{0x1e,0x0003,0x0043}, // for 100BASE-T TX //CML_20130327
        	{0x1e,0x0004,0x0207}, // for 100BASE-T TX //CML_20130327
        	{0x1e,0x0007,0x0357}, // for 100BASE-T TX //CML_20130327
        	{0x1e,0x0008,0x02f0}, // for 100BASE-T TX //CML_20130327
        	{0x1e,0x000a,0x0006} // for 100BASE-T TX //CML_20130327
			#endif //CML_20130401
			/*{0x1e,0x0012,0x7230},  // 0x7210  // allen_20130926 : tx gain*/
			/*{0x1e,0x0045,0x0303},  // 0x0202  // allen_20130926 : R offset*/
			/*{0x1e,0x0046,0x0303},   // 0x0303 // for 100BASE-T TX //CML_20130401  // allen_20130926 : R offset */

			{0x1e,0x0000,0x01b7},	// 0->1, middle
			{0x1e,0x0001,0x01c0},	// 0->1, ov1
			{0x1e,0x0002,0x01c0},	// 0->1, ov2, z2p_ovs
			{0x1e,0x0003,0x0090},	// 1->0, middle
			{0x1e,0x0004,0x0205},	// 1->0, ov1
			{0x1e,0x0005,0x0205},	// 1->0, ov2, p2z_ovs

			{0x1e,0x0006,0x039c},	// 0->-1, middle
			{0x1e,0x0007,0x03c0},	// 0->-1, ov1
			{0x1e,0x0008,0x03c7},	// 0->-1, ov2, z2n_ovs
			{0x1e,0x0009,0x0297},	// -1->0, middle
			{0x1e,0x000a,0x0005},	// -1->0, ov1
			{0x1e,0x000b,0x0007},	// -1->0, ov2, n2z_ovs
			
			{0x1e,0x0014,0x0040},	// pair delay (afe)
			{0x1e,0x0044,0x0060},
			{0x1e,0x0176,0x5500},
			{0x1e,0x0177,0x0055},

			{0x1e,0x0012,0x2c10},	// 10 base Tx
			{0x1e,0x0017,0x0410},	// 10 base Tx
			
			{0x1e,0x00a6,0x0350},
			{0x07,0x003c,0x0000}
			//{0x1e,0x0120,0x8d0d},  // for 7510 E4 
			//{0x1e,0x0122,0xffa0},   // for 7510 E4
			//{0x1e,0x00a6,0x03e0}   // for 7510 E4 	
	     },

     	//per-port data
     	{
	 	}
    }, 
};

mt7512Ge_cfg_trdata_t mt7512Ge_cfg_tr[MT7512Ge_PHY_INIT_TR_SET_NUM]=
{
    {	//local data
     	{ 
			//for reference {"PMA",0x20,0x0700},
			//{"DSPF",0x0010,0x005000},
			//{"DSPF",0x0011,0x040004},
			//{"DSPF",0x000e,0x800008}
			{"PMA",0x03,0x082422}
     	},

     	//per-port data
     	{ //{"PMA",0x20,0x0700}
     		{"PMA",0x03,0x082422}	// allen_20160826
	 	}
    }, 
};

static int32 MT7512GePhy_erroversum_A = 0;
static int32 MT7512GePhy_erroversum_B = 0;
static int32 MT7512GePhy_erroversum_C = 0;
static int32 MT7512GePhy_erroversum_D = 0;

static void mt7512GePhyConfig(int argc, char *argv[], void *p);
void 		mt7512GePhyCfgLoad(uint8 idx);
static void mt7512GePhyCfgTx10AmpSave(void);
void 		mt7512GePhyGRCfgCheck(void);
void 		mt7512GePhyLRCfgCheck(uint8 port_num);
void 		mt7512GePhyCfgCheck(void);

#ifdef MTPHY_DEBUG
//ESD detect
static int16 mt7512GePhy_Up2Down_cnt_gbt[5] = {0,0,0,0,0};
static int16 mt7512GePhy_Up2Down_cnt_hbt[5] = {0,0,0,0,0};
static uint8 mt7512GePhy_Up2DownFastHbt_detect[5] = {0,0,0,0,0};
static uint8 mt7512GePhy_Up2DownFastgbt_detect[5] = {0,0,0,0,0};

static bool 	mt7512GePhy_link_fail_detect_flag = 0;
static uint8 	mt7512GePhy_reset_needed_perport[5] = {0,0,0,0,0};
static uint8 	mt7512GePhy_reset_needed_flag = 0;
static uint8 	mt7512GePhy_esdphy_init_flag = 1; // default: enable

//Disable EEE if link fail
static int16 	mt7512GePhy_Up2Down_cnt_tbt[5] = {0,0,0,0,0};
static uint8 	mt7512GePhy_Up2DownFastTbt_detect[5] = {0,0,0,0,0};
static uint8 	mt7512GePhy_linkup_check_timer[5] = {0,0,0,0,0};
static bool 	mt7512GePhyDisEEEHappened[5] = {0,0,0,0,0};
static uint8 	mt7512GePhy_linkup_check_cnt[5] = {0,0,0,0,0};
static uint8 	mt7512GePhyDisNextPage_cnt[5] = {0,0,0,0,0};
static bool 	mt7512GePhyDisNextPage_flag[5] = {0,0,0,0,0};
static uint8 	mt7512GePhyDisSlaveMode_cnt[5] = {0,0,0,0,0};  // allen_20140822
static bool 	mt7512GePhyDisSlaveMode_flag[5] = {0,0,0,0,0};
int32 			mt7512GePhyReadProbe(uint8 port_num, uint8 mode, uint8 ch_num);
int32 			mt7512GePhyReadCoeff(uint8 port_num, uint8 mode, uint8 ch_num);
void 			mt7512GePhyDbgPorbeSet(uint8 prb_num, uint8  port_num, uint16 flg_num, uint16 clk_num );
int32 			mt7512GePhyDbgPorbeRead(uint8 prb_num,uint8  port_num);
#endif

#ifdef MTPHY_DEBUG 
int32 	mt7512GePhyReadErrOverSum(uint8 port_num);
int32 	mt7512GePhyReadAdcSum(uint8 port_num);
void 	mt7512GePhyDispProbe100(uint8 port_num, bool long_msg);
void 	mt7512GePhySwDispMessage(uint8 port_num);
uint16 	mt7512GePhyReadSnrSum(uint8 port_num, uint16 cnt);
void 	mt7512GePhyUpdateErrOverSum(uint8 port_num);
void 	mt7512GePhySwErrOverMonitor(uint8 port_num);
void 	mt7512GePhyUp2DownFastHbt(uint8 port_num);
void 	mt7512GePhyUp2DownFastTbt(uint8 port_num);
bool 	mt7512GePhyLinkFailDetect (uint8 port_num, uint8 mode);		//0:10BT or 100BT, 1:100BT only, 2:10BT only,3:10BT or 100BT or 1000BT
void 	mt7512GePhyDisEEENeeded(uint8 port_num);
void 	mt7512GePhyDisNextPageNeeded(uint8 port_num);
#endif //TCPHY_DEBUG

void 	mt7512GePhySwPatch(void);
void 	doGePhyALLAnalogCal(uint8 phyaddr); 				// for EN7512 
uint8 	allGeAnaCalWait(uint32 delay, uint8 phyaddr);  		// for EN7512 
void	GECal_Rext(uint8 phyaddr, uint32 delay);			// for EN7512 
void	GECal_R50(uint8 phyaddr, uint32 delay);			// for EN7512 
void	GECal_tx_amp(uint8 phyaddr, uint32 delay);			// for EN7512 
static uint8 	GECal_flag = 0;


#ifdef MTPHY_DEBUG
void mt7512GePhyDbgPorbeSet(uint8 prb_num, uint8  port_num, uint16 flg_num, uint16 clk_num)
{
	uint16 	value=0;
	#define dbg_flg_en 1 
	#define dbg_DevAddr 0x1F
	#define dbg_Flg0Ctrl 0x15
	#define dbg_Flg1Ctrl 0x16
	#define dbg_ClkAddr 0x19
 
	if(prb_num == 2)
	{
		value = (port_num<<13)|(dbg_flg_en<<12)|(flg_num);
		TCPHYDISP1("Dbg Flag Register Set Value=%lx\r\n", value);
		mtEMiiRegWrite(port_num,dbg_DevAddr, dbg_Flg0Ctrl, value);
		mtEMiiRegWrite(port_num,dbg_DevAddr, dbg_Flg1Ctrl, value);
	}
	else if(prb_num == 0)
	{
		value = (port_num<<13)|(dbg_flg_en<<12)|(flg_num);
		TCPHYDISP1("Dbg Flag Register Set Value=%lx\r\n", value);
		mtEMiiRegWrite(port_num, dbg_DevAddr, dbg_Flg0Ctrl, value);
	}
	else if(prb_num == 1)
	{
		value = (port_num<<13)|(dbg_flg_en<<12)|(flg_num);
		TCPHYDISP1("Dbg Flag Register Set Value=%lx\r\n", value);
		mtEMiiRegWrite(port_num, dbg_DevAddr, dbg_Flg1Ctrl, value);
	}

    if(clk_num == 0xff)
    {	//disable clk output
		value = mtEMiiRegRead( port_num, dbg_DevAddr, dbg_ClkAddr);
		value = value & 0xEF;
		mtEMiiRegWrite(port_num, dbg_DevAddr, dbg_ClkAddr, value);
    }
	else
	{	//enable clk output
		value = (dbg_flg_en<<12)|(clk_num);
		mtEMiiRegWrite(port_num, dbg_DevAddr, dbg_ClkAddr, value);
	}	
}

int32 mt7512GePhyDbgPorbeRead(uint8 prb_num,uint8  port_num)
{
	uint16 	value=0;
	#define dbg_flg_en 1 
	#define dbg_DevAddr 0x1F
	#define dbg_Flg0Ctrl 0x15
	#define dbg_Flg1Ctrl 0x16
	#define dbg_ClkAddr 0x19
	#define dbg_monAddr 0x1A

	value = mtEMiiRegRead( port_num, dbg_DevAddr, dbg_monAddr);

	if(prb_num == 0)
		value = (value & 0x00FF);
	else if(prb_num == 1)
		value = (value>>8) & 0x00FF;
             	
	TCPHYDISP1("Dbg Flag[%d] Monitor Register Value=%lx\r\n", prb_num,value);
	return(value);	       	
}

int32 mt7512GePhyReadProbe(uint8 port_num, uint8 mode, uint8 ch_num)
{
    uint32 val=0,val_A=0,val_B=0,val_C=0,val_D=0;
    uint32 avg_val=0, avg_A=0,avg_B=0,avg_C=0,avg_D=0;
    uint32 phyaddr = port_num;
    uint16 i;
    #define avg_cnt 1000
    
    tcMiiStationWrite(port_num, 31, 0);
    switch(mode)
    {
    	case Probe_VgaState:
    		val = mtPhyMiiRead_TrDbg(phyaddr,"PMA",0x38,0);
    		TCPHYDISP3("VgaStateA =0x%x\r\n",((val>>4) & 0x1F));
    		TCPHYDISP3("VgaStateB =0x%x\r\n",((val>>9) & 0x1F));
    		TCPHYDISP3("VgaStateC =0x%x\r\n",((val>>14) & 0x1F));
    		TCPHYDISP3("VgaStateD =0x%x\r\n",((val>>19) & 0x1F));
    		break;
    		
        case Probe_MSE:
          	switch(ch_num)
          	{
	        	case 0:		//Pair A
              		val = mtEMiiRegRead(phyaddr,0x1E,0x9B);
			  		TCPHYDISP1("XX0 0x1E,0x9B =0x%x\r\n",val);
              		val = (val>>8) & 0xFF;  	//V1.16
              		TCPHYDISP1("AA0 lch_mse_mdcA =0x%x\r\n",val);
              		break;
            	case 1:		//Pair B
              		val = mtEMiiRegRead(phyaddr,0x1E,0x9B);
			  		TCPHYDISP1("XX1 0x1E,0x9B =0x%x\r\n",val);
              		val = (val) & 0xFF;  	//V1.16
              		TCPHYDISP1("AA1 lch_mse_mdcB =0x%x\r\n",val);
              		break;
            	case 2:		//Pair C
              		val = mtEMiiRegRead(phyaddr,0x1E,0x9C);
			  		TCPHYDISP1("XX2 0x1E,0x9C =0x%x\r\n",val);
              		val = (val>>8) & 0xFF;  	//V1.16
              		TCPHYDISP1("AA2 lch_mse_mdcC =0x%x\r\n",val);
              		break;
            	case 3:		//Pair D
              		val = mtEMiiRegRead(phyaddr,0x1E,0x9C);
			  		TCPHYDISP1("XX3 0x1E,0x9C =0x%x\r\n",val);
              		val = (val) & 0xFF;  	//V1.16
              		TCPHYDISP1("AA3 lch_mse_mdcD =0x%x\r\n",val);
              		break;
            }
		    break;
		    
        case Probe_MSE_ALL:
			val=mtEMiiRegRead(phyaddr,0x1E,0x9B);
			val_A =(val>>8) & 0xFF;  //V1.16
			val_B =(val) & 0xFF;  //V1.16
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
			val=mtEMiiRegRead(phyaddr,0x1E,0x9D);
			val=(val>>11) & 0x1F; 
			TCPHYDISP3("slicer_err_thres =0x%x\r\n",val);
			break;
			
        case Probe_MSE_Sum:
			for(i=0; i<avg_cnt; i++)
			{
				val = mtEMiiRegRead(phyaddr, 0x1E, 0x9B);
				val_A =(val>>8) & 0xFF;  //V1.16
				val_B =(val) & 0xFF;  //V1.16
				avg_A = avg_A + val_A;
				avg_B = avg_B + val_B;
            
				val = mtEMiiRegRead(phyaddr, 0x1E, 0x9C);
				val_C =(val>>8) & 0xFF;  //V1.16
				val_D =(val) & 0xFF;  //V1.16
				avg_C = avg_C + val_C;
				avg_D = avg_D + val_D;
             
				val = (mtEMiiRegRead(phyaddr, 0x1E, 0x9D)>>11)&0x1F;

				avg_val = avg_val + val;
			}
	        avg_A = avg_A/1000;
    	    avg_B = avg_B/1000;
          	avg_C = avg_C/1000;
          	avg_D = avg_D/1000;
          	avg_val = avg_val/1000;
          
          	val = avg_A + avg_B + avg_C + avg_D;
          
          	TCPHYDISP3("lch_mse_mdcA avg1000 =0x%x\r\n", avg_A);
          	TCPHYDISP3("lch_mse_mdcB avg1000 =0x%x\r\n", avg_B);
          	TCPHYDISP3("lch_mse_mdcC avg1000 =0x%x\r\n", avg_C);
          	TCPHYDISP3("lch_mse_mdcD avg1000 =0x%x\r\n", avg_D);
          	TCPHYDISP3("slicer_err_thres avg1000 =0x%x\r\n", avg_val);
          	break;
          	
        case Probe_Slicer_Err_Over_Sum:      
	    	switch(ch_num)
	        {
              	case 0:
              		val = mtEMiiRegRead(phyaddr, 0x1E, 0x9E);
              		ErovcntA[port_num].err_over_cnt = val;
              		TCPHYDISP3("ProbeSlicerErrOverSumA =0x%x\r\n", val);
              		break;
              		
            	case 1:
              		val = mtEMiiRegRead(phyaddr, 0x1E, 0x9F);
              		ErovcntB[port_num].err_over_cnt = val;
              		TCPHYDISP3("ProbeSlicerErrOverSumB =0x%x\r\n", val);
              		break;
              		
	            case 2:
              		val = mtEMiiRegRead(phyaddr, 0x1E, 0xA0);
              		ErovcntC[port_num].err_over_cnt = val;
              		TCPHYDISP3("ProbeSlicerErrOverSumC =0x%x\r\n", val);
              		break;
              		
            	case 3:
              		val = mtEMiiRegRead(phyaddr, 0x1E, 0xA1);
              		ErovcntD[port_num].err_over_cnt = val;
              		TCPHYDISP3("ProbeSlicerErrOverSumD =0x%x\r\n", val);
              		break;
        	}
          	break;
          		
        case Probe_Slicer_Err_Over_Sum_ALL:
              val_A = mtEMiiRegRead(phyaddr, 0x1E, 0x9E);
              TCPHYDISP3("ProbeSlicerErrOverSumA =0x%x\r\n", val_A);
              val_B = mtEMiiRegRead(phyaddr, 0x1E, 0x9F);
              TCPHYDISP3("ProbeSlicerErrOverSumB =0x%x\r\n", val_B);
              val_C = mtEMiiRegRead(phyaddr, 0x1E, 0xA0);
              TCPHYDISP3("ProbeSlicerErrOverSumC =0x%x\r\n", val_C);
              val_D = mtEMiiRegRead(phyaddr, 0x1E, 0xA1);
              TCPHYDISP3("ProbeSlicerErrOverSumD =0x%x\r\n", val_D);
              val = val_A + val_B + val_C + val_D;
              break;
  
        default:
            TCPHYDISP1("\r\ntcphy error: ReadProbe %d.\r\n",mode);
            break;
    }
    return val;
}

void mt7512GePhySwDispMessage(uint8 port_num)
{
	#ifdef PERIOD_PRINT 	//V1.17
    uint32 value;
	#endif

    switch(mtphy_link_state[port_num]) 
    {
        case ST_LINK_DOWN: 
      		// printf("Link Satus is ST_LINK_DOWN!\r\n");
            break;
            
        case ST_LINK_DOWN2UP: 
      		//  printf("Link Satus is ST_LINK_DOWN2UP!\r\n");
			if(mr1E_A2.final_speed_1000)
			{
				TCPHYDISP1("mtphy[%d]:Link-up at 1000 %s.\r\n", port_num, (mr1E_A2.FullDuplexEnable ?"F":"H"));
			}
			else if (mr1E_A2.final_speed_100)
			{
				TCPHYDISP1("mtphy[%d]:Link-up at 100%s.\r\n", port_num, (mr1E_A2.FullDuplexEnable ?"F":"H"));
			}
			else if (mr1E_A2.final_speed_10)
			{
				TCPHYDISP1("mtphy[%d]:Link-up at 10%s.\r\n",port_num, (mr1E_A2.FullDuplexEnable ?"F":"H"));
			}
	
			if(mr1E_A2.final_speed_1000 ||mr1E_A2.final_speed_100 ||mr1E_A2.final_speed_10)
			{
				mt7512GePhyReadProbe(port_num, Probe_VgaState, 0);
				mt7512GePhyReadProbe(port_num, Probe_MSE_ALL, 0);
				mt7512GePhyReadProbe(port_num, Probe_MSE_Slicer_err_thres, 0);
				mt7512GePhyReadProbe(port_num, Probe_Slicer_Err_Over_Sum_ALL, 0);
			}
            break;
            
        case ST_LINK_UP: 
         	// printf("Link Satus is ST_LINK_UP!\r\n");
          	#ifdef PERIOD_PRINT //V1.17
            if (period_print_flag == 1)
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

void mt7512GePhyUpdateErrOverSum(uint8 port_num)
{
    uint16 	err_over_cnt_prev_A; // to store previous err_over_cnt value
    uint16 	err_over_cnt_prev_B; // to store previous err_over_cnt value
    uint16 	err_over_cnt_prev_C; // to store previous err_over_cnt value
    uint16 	err_over_cnt_prev_D; // to store previous err_over_cnt value
    int32 	val_A, val_B, val_C, val_D;
    
    // clear Nmr25[port_num].err_over_cnt in tc2104meDispProbe100() in ST_LINK_DOWN2UP
    if (((mr1E_A2.final_speed_1000== 1) ||(mr1E_A2.final_speed_100 == 1)) // 1000/100BaseTX
        && (mtphy_link_state[port_num] == ST_LINK_UP)) 
    {
    	err_over_cnt_prev_A = ErovcntA[port_num].err_over_cnt;
        err_over_cnt_prev_B = ErovcntB[port_num].err_over_cnt;
        err_over_cnt_prev_C = ErovcntC[port_num].err_over_cnt;
        err_over_cnt_prev_D = ErovcntD[port_num].err_over_cnt;
        
      	// mt7510GePhyReadProbe(port_num,Probe_MSE_Sum,0);
       	mt7512GePhyReadProbe(port_num,Probe_Slicer_Err_Over_Sum,0);		//Pair A
       	mt7512GePhyReadProbe(port_num,Probe_Slicer_Err_Over_Sum,1);		//Pair B
       	mt7512GePhyReadProbe(port_num,Probe_Slicer_Err_Over_Sum,2);		//Pair C
       	mt7512GePhyReadProbe(port_num,Probe_Slicer_Err_Over_Sum,3);		//Pair D
        val_A = ErovcntA[port_num].err_over_cnt - err_over_cnt_prev_A;
        val_B = ErovcntB[port_num].err_over_cnt - err_over_cnt_prev_B;
        val_C = ErovcntC[port_num].err_over_cnt - err_over_cnt_prev_C;
        val_D = ErovcntD[port_num].err_over_cnt - err_over_cnt_prev_D;
        TCPHYDISP4("ProbeSlicerErrOverSumA =0x%x\r\n",val_A);
        TCPHYDISP4("ProbeSlicerErrOverSumB =0x%x\r\n",val_B);
        TCPHYDISP4("ProbeSlicerErrOverSumC =0x%x\r\n",val_C);
        TCPHYDISP4("ProbeSlicerErrOverSumD =0x%x\r\n",val_D);
        
        if( val_A < 0 )
        {
        	val_A += 2048;
        }
        if( val_B < 0 )
        {
            val_B += 2048;
        }
        if( val_C < 0 )
        {
            val_C += 2048;
        }
        if( val_D < 0 )
        {
            val_D += 2048;
        }
        MT7512GePhy_erroversum_A = val_A;
        MT7512GePhy_erroversum_B = val_B;
        MT7512GePhy_erroversum_C = val_C;
        MT7512GePhy_erroversum_D = val_D;
    }
    else 
    {
        MT7512GePhy_erroversum_A = -1;
        MT7512GePhy_erroversum_B = -1;
        MT7512GePhy_erroversum_C = -1;
        MT7512GePhy_erroversum_D = -1;
    }
}

void mt7512GePhySwErrOverMonitor(uint8 port_num)
{
    if (tcPhy_disp_level < 3) 
        return; // inactive

    if (MT7512GePhy_erroversum_A>0)
    {
        TCPHYDISP3("tcphy[%d]: ErrOver A=%ld\r\n",port_num,MT7512GePhy_erroversum_A);          
    }
    if (MT7512GePhy_erroversum_B>0)
    {
        TCPHYDISP3("tcphy[%d]: ErrOver B=%ld\r\n",port_num,MT7512GePhy_erroversum_B);          
    }
    if (MT7512GePhy_erroversum_C>0)
    {
        TCPHYDISP3("tcphy[%d]: ErrOver C =%ld\r\n",port_num,MT7512GePhy_erroversum_C);          
    }
    if (MT7512GePhy_erroversum_D>0)
    {
        TCPHYDISP3("tcphy[%d]: ErrOver D=%ld\r\n",port_num,MT7512GePhy_erroversum_D);          
    }
}

void mt7512GePhyUp2DownFastgbt(uint8 port_num)
{
	const uint8 mt7512GePhy_Up2Down_gbt_Thd = 10;
	const uint8 mt7512GePhy_Up2Down_gbt_ub = 40;

	if(mr1E_A2.final_speed_1000 & !Nmr1[port_num].link_status) 
	{
		if(mt7512GePhy_Up2Down_cnt_gbt[port_num]<mt7512GePhy_Up2Down_gbt_ub) 
		{
			mt7512GePhy_Up2Down_cnt_gbt[port_num]+=3;
		}
    }
	else if(!mr1E_A2.final_speed_1000 & !Nmr1[port_num].link_status)
	{
		if(mt7512GePhy_Up2Down_cnt_gbt[port_num]>0) 
		{
			mt7512GePhy_Up2Down_cnt_gbt[port_num]--;
		}
	}
	else if(mr1E_A2.final_speed_1000 & Nmr1[port_num].link_status) 
	{	//adjust link-up time to clear counter
		mt7512GePhy_Up2Down_cnt_gbt[port_num]=0;
	}
	
	if(mt7512GePhy_Up2Down_cnt_gbt[port_num] > mt7512GePhy_Up2Down_gbt_Thd) 
	{
	    mt7512GePhy_Up2Down_cnt_gbt[port_num]=0;
		mt7512GePhy_Up2DownFastgbt_detect[port_num]=1;
		#ifdef TCPHY_DEBUG
		TCPHYDISP4("set mt7512GePhy_Up2DownFastgbt_detect[%d] to 1.\r\n",port_num); 
		#endif
	}
	else 
	{
		mt7512GePhy_Up2DownFastgbt_detect[port_num]=0;
	}  
}

void mt7512GePhyUp2DownFastHbt(uint8 port_num)
{
	const uint8 mt7512GePhy_Up2Down_hbt_Thd = 10;
	const uint8 mt7512GePhy_Up2Down_hbt_ub = 40;

	if(mr1E_A2.final_speed_100 & !Nmr1[port_num].link_status) 
	{
		if(mt7512GePhy_Up2Down_cnt_hbt[port_num] < mt7512GePhy_Up2Down_hbt_ub) 
		{
			mt7512GePhy_Up2Down_cnt_hbt[port_num] += 3;
		}
    }
	else if(!mr1E_A2.final_speed_100 & !Nmr1[port_num].link_status) 
	{
		if(mt7512GePhy_Up2Down_cnt_hbt[port_num]>0) 
		{
			mt7512GePhy_Up2Down_cnt_hbt[port_num]--;
		}
	}
	else if(mr1E_A2.final_speed_100 & Nmr1[port_num].link_status) 
	{	//adjust link-up time to clear counter
		mt7512GePhy_Up2Down_cnt_hbt[port_num]=0;
	}
	
	if(mt7512GePhy_Up2Down_cnt_hbt[port_num]> mt7512GePhy_Up2Down_hbt_Thd) 
	{
	    mt7512GePhy_Up2Down_cnt_hbt[port_num]=0;
		mt7512GePhy_Up2DownFastHbt_detect[port_num]=1;
		#ifdef TCPHY_DEBUG
		TCPHYDISP4("set mt7512GePhy_Up2DownFastHbt_detect[%d] to 1.\r\n",port_num); 
		#endif
	}
	else 
	{
		mt7512GePhy_Up2DownFastHbt_detect[port_num]=0;
	}  
}

void mt7512GePhyUp2DownFastTbt(uint8 port_num)
{
	const uint8 mt7512GePhy_Up2Down_Tbt_Thd = 10;
	const uint8 mt7512GePhy_Up2Down_Tbt_ub = 40;
	const uint8 linkup_check_timer_done = 1;
	
    if(mr1E_A2.final_speed_10 & !Nmr1[port_num].link_status) 
	{
		mt7512GePhy_linkup_check_timer[port_num]=0;
		if(mt7512GePhy_Up2Down_cnt_tbt[port_num] < mt7512GePhy_Up2Down_Tbt_ub) 
		{
			mt7512GePhy_Up2Down_cnt_tbt[port_num]+=5;
		}
    }
	else if(!mr1E_A2.final_speed_10 & !Nmr1[port_num].link_status) 
	{
		mt7512GePhy_linkup_check_timer[port_num]=0;
		if(mt7512GePhy_Up2Down_cnt_tbt[port_num]>0) 
		{
			mt7512GePhy_Up2Down_cnt_tbt[port_num]--;
		}
	}
	else if(mr1E_A2.final_speed_10 & Nmr1[port_num].link_status ) 
	{
		if((mt7512GePhy_linkup_check_timer[port_num] == linkup_check_timer_done)) 
		{
			mt7512GePhy_Up2Down_cnt_tbt[port_num]=0;
			mt7512GePhy_linkup_check_timer[port_num]=0;
		}
		else if((mt7512GePhy_linkup_check_timer[port_num] < linkup_check_timer_done))
		{
			mt7512GePhy_linkup_check_timer[port_num]++;
		}
	}
	
	if(mt7512GePhy_Up2Down_cnt_tbt[port_num]> mt7512GePhy_Up2Down_Tbt_Thd) 
	{
	    mt7512GePhy_Up2Down_cnt_tbt[port_num]=0; 
		mt7512GePhy_Up2DownFastTbt_detect[port_num]=1;
		#ifdef TCPHY_DEBUG
		TCPHYDISP4("set mt7512GePhy_Up2DownFastTbt_detect[%d] to 1.\r\n",port_num); 
		#endif
	}
	else 
	{
		mt7512GePhy_Up2DownFastTbt_detect[port_num]=0;
	}	
}

bool mt7512GePhyLinkFailDetect(uint8 port_num, uint8 mode)
{
	switch(mode)
	{
		case TbtOrHbtOrGbt:
			if(sw_FixUp2DownFast_flag 
				&(mt7512GePhy_Up2DownFastTbt_detect[port_num]
				||mt7512GePhy_Up2DownFastHbt_detect[port_num]||mt7512GePhy_Up2DownFastgbt_detect[port_num])) 
			{
				mt7512GePhy_link_fail_detect_flag =1;
		        #ifdef TCPHY_DEBUG
		        TCPHYDISP3("mt7512GePhy_link_fail_detect_flag(%d)=%d\r\n", port_num, mt7512GePhy_link_fail_detect_flag); 
		        #endif
			}
			else 
			{
				mt7512GePhy_link_fail_detect_flag =0;
			}	
			break;

		case HbtOnly:
			if(sw_FixUp2DownFast_flag & mt7512GePhy_Up2DownFastHbt_detect[port_num]) 
			{
				mt7512GePhy_link_fail_detect_flag =1;
		        #ifdef TCPHY_DEBUG
		        TCPHYDISP3("Hbt: mt7512GePhy_link_fail_detect_flag(%d)=%d\r\n", port_num, mt7512GePhy_link_fail_detect_flag); 
		        #endif
			}
			else 
			{
				mt7512GePhy_link_fail_detect_flag =0;
			}
			break;

		case TbtOnly:
			if(sw_FixUp2DownFast_flag & mt7512GePhy_Up2DownFastTbt_detect[port_num]) 
			{
				mt7512GePhy_link_fail_detect_flag =1;
		       	#ifdef TCPHY_DEBUG
		        TCPHYDISP3("Tbt: mt7512GePhy_link_fail_detect_flag(%d)=%d\r\n", port_num, mt7512GePhy_link_fail_detect_flag); 
		        #endif
			}
			else 
			{
				mt7512GePhy_link_fail_detect_flag =0;
			}
			break;

		default: 
			mt7512GePhy_link_fail_detect_flag =0;
	}
	return mt7512GePhy_link_fail_detect_flag;
}

void mt7512GePhyDisNextPageNeeded(uint8 port_num)
{
	const uint8 mt7512GePhyDisNextPage_cntdone = 5;
	const uint8 mt7512GePhyDisSlaveMode_cntdone = 3; // allen_20140822
   
   	if((mtphy_link_state[port_num] == ST_LINK_UP)||(mt7512GePhyDisEEEHappened[port_num])) 
 	{
   		mt7512GePhyDisNextPage_cnt[port_num] = 0;
   	}
   	else if((mr5.LPNextAble) && !(mr7_3D.lp_eee_100||mr7_3D.lp_eee_1000||mr7_3D.lp_eee_10g)
   		&& (mt7512GePhyDisNextPage_cnt[port_num] <= mt7512GePhyDisNextPage_cntdone)) 
   	{
   		mt7512GePhyDisNextPage_cnt[port_num]++;
   	}

   	if(mt7512GePhyDisNextPage_cnt[port_num] > mt7512GePhyDisNextPage_cntdone) 
	{
   		mt7512GePhyDisNextPage_flag[port_num] = 1;
		mt7512GePhyDisNextPage_cnt[port_num] = 0;
   	}
   	else 
	{
   		mt7512GePhyDisNextPage_flag[port_num]=0;
   	}

	// allen_20140822 === for gphy eee iot with realtek : default slave mode
	if((mtphy_link_state[port_num] != ST_LINK_UP) && (mtphy_link_state[port_num] != ST_LINK_DOWN) &&
		(mt7512GePhyDisSlaveMode_cnt[port_num] <= mt7512GePhyDisSlaveMode_cntdone))
	{
		mt7512GePhyDisSlaveMode_cnt[port_num]++;
	}

	if(mt7512GePhyDisSlaveMode_cnt[port_num] > mt7512GePhyDisSlaveMode_cntdone)
	{
		mt7512GePhyDisSlaveMode_flag[port_num] = 1;
		mt7512GePhyDisSlaveMode_cnt[port_num] = 0;
	}
	else
	{
		mt7512GePhyDisSlaveMode_flag[port_num] = 0;
	}
}

void mt7512GePhyDisEEENeeded(uint8 port_num)
{
    const uint8 mt7512GePhy_linkup_check_done = 1;
	uint8 phy_base = 8, phyaddr;
	
	phyaddr = phy_base + port_num;
	
    if((mtphy_link_state[port_num] == ST_LINK_UP)
		&& (mt7512GePhy_linkup_check_cnt[port_num] < mt7512GePhy_linkup_check_done)) 
	{
		mt7512GePhy_linkup_check_cnt[port_num]++;
    }
	else if(mtphy_link_state[port_num] != ST_LINK_UP)
	{
		mt7512GePhy_linkup_check_cnt[port_num] = 0;
    }
	
    if(mt7512GePhyDisEEEHappened[port_num] && mt7512GePhy_linkup_check_cnt[port_num]==mt7512GePhy_linkup_check_done) 
	{	//extend link-up time
		mtEMiiRegWrite(phyaddr, 0x07, 0x3c, 0x06);
		mt7512GePhyDisEEEHappened[port_num]=0;
    }
	else if(mt7512GePhyLinkFailDetect(port_num,3) || mt7512GePhyDisNextPage_flag[port_num]) 
	{
		mtEMiiRegWrite(phyaddr, 0x07, 0x3c, 0x0000);	//disable next page
		tcMiiStationWrite(0,0x00,0x1200);					//re-start AN
		mt7512GePhyDisEEEHappened[port_num]=1;
    }
	//TCPHYDISP3("mt7510GePhy_linkup_check_cnt[%d]=%d\r\n",port_num,mt7510GePhy_linkup_check_cnt[port_num]);
	//TCPHYDISP3("mt7510GePhyDisEEEHappened[%d]=%d\r\n",port_num,mt7510GePhyDisEEEHappened[port_num]);
}

void mt7512GePhySwPatch(void)
{
    uint8 	pn;
	uint32 	reg0, reg1, reg9, reg10;
	uint8 	gphy_base = 12, phyaddr, phy_base = 8;
	
    if(power_on_finish_flag == 0)
    {
    	if((tcMiiStationRead(12, 0x09)&0xE000) == 0xE000)
      	{
        	power_on_finish_flag = 1;
        	printf("\n Power On Process Finish \n");
        	tcMiiStationWrite(12, 0x09, 0x0700);
      	}
    }

    for(phyaddr = gphy_base; phyaddr < (gphy_base+MT7512Ge_PORTNUM); phyaddr++) // allen_20140822
	{
		pn = phyaddr - phy_base;
        mtPhyReadReg(pn,1);
        reg0 = tcMiiStationRead(phyaddr, 0x0);
		reg1 = tcMiiStationRead(phyaddr, 0x1);
		reg9 = tcMiiStationRead(phyaddr, 0x9);
		reg10 = tcMiiStationRead(phyaddr, 0xa);
		
        if( !Nmr1[pn].link_status_prev && !Nmr1[pn].link_status )
		{
            mtphy_link_state[pn] = ST_LINK_DOWN;
        }
        else if( !Nmr1[pn].link_status_prev && Nmr1[pn].link_status )
		{
            mtphy_link_state[pn] = ST_LINK_DOWN2UP;
        }
        else if( Nmr1[pn].link_status_prev && !Nmr1[pn].link_status )
		{
            mtphy_link_state[pn]= ST_LINK_UP2DOWN;
        }
        else 
		{ 	//if( Nmr1[pn].link_status_prev && Nmr1[pn].link_status ){
            mtphy_link_state[pn] = ST_LINK_UP;
        }
    
        mtPhyReadReg(phyaddr,5);
        mtPhyReadGReg(phyaddr,0x1E,0xA2);
		mtPhyReadGReg(phyaddr,0x7,0x3d);
        
		if(((reg9>>9)&0x1) == 1) // check 1000Mbps capbility
		{
			if((((reg10>>15)&0x1)==1)&&(((reg9>>12)&0x1)==1))  // local phy is slave && collision
			{
				tcMiiStationWrite(phyaddr, 0x09, 0x0600); // disable manual 
				tcMiiStationWrite(phyaddr, 0x00, 0x1200); // re-start AN
			}

			mt7512GePhyDisNextPageNeeded(pn);  // allen_20140618
			mt7512GePhyUp2DownFastgbt(pn);
			mt7512GePhyUp2DownFastHbt(pn);
			mt7512GePhyUp2DownFastTbt(pn);
			mt7512GePhyDisEEENeeded(pn);
 
			if((mtphy_link_state[pn] != ST_LINK_UP) && (mt7512GePhyDisSlaveMode_flag[pn] == 1) && (((reg9>>12)&0x1)==1)) // allen_20140822
			{
				tcMiiStationWrite(phyaddr, 0x09, 0x0600); // disable manual  
				tcMiiStationWrite(phyaddr, 0x00, 0x1200); // re-start AN
				mt7512GePhyDisSlaveMode_cnt[pn] = 0;
				mt7512GePhyDisSlaveMode_flag[pn] = 0;
			}
			else if(mtphy_link_state[pn] == ST_LINK_UP)
			{
				tcMiiStationWrite(phyaddr, 0x09, 0x1e00); // keep default master 
				mt7512GePhyDisSlaveMode_cnt[pn] = 0;
				mt7512GePhyDisSlaveMode_flag[pn] = 0;
			}
		}

		#ifdef MTPHY_DEBUG
		// display message
		mt7512GePhySwDispMessage(pn);
		#endif

		#ifdef MTPHY_DEBUG
		if(sw_ErrOverMonitor_flag)
			mt7512GePhyUpdateErrOverSum(pn);
		if(sw_ErrOverMonitor_flag) 
			mt7512GePhySwErrOverMonitor(pn); // call after mt7510FEUpdateErrOverSum()
		#endif
    }
}
#endif

void mt7512GePhyCfgLoad(uint8 idx)
{
	int pn, i;
	uint16 phyAddr, phyAddr_base = 12;
	current_idx = idx;

	eco_rev = VPint(0xbfb00064);

    // global registers
    for( pn=0; pn<tcPhyPortNum; pn++)
	{
		phyAddr = phyAddr_base + pn;
	
		TCPHYDISP3("\n 7512Ge, phyaddr= (%d,%d) \n", phyAddr_base, phyAddr);
		for( i=0; i<MT7512Ge_PHY_INIT_CL45_GDATA_LEN; i++ )
		{         
			mtEMiiRegWrite(phyAddr, mt7512Ge_cfg_cl45[current_idx].gdata[i].dev_num, mt7512Ge_cfg_cl45[current_idx].gdata[i].reg_num, mt7512Ge_cfg_cl45[current_idx].gdata[i].val);
		}
		for( i=0; i<MT7512Ge_PHY_INIT_CL45_LDATA_LEN; i++ )
		{         
			mtEMiiRegWrite(phyAddr, mt7512Ge_cfg_cl45[current_idx].ldata[i].dev_num, mt7512Ge_cfg_cl45[current_idx].ldata[i].reg_num, mt7512Ge_cfg_cl45[current_idx].ldata[i].val);
		}
		for( i=0; i<MT7512Ge_PHY_INIT_LDATA_LEN; i++ )
		{         
			tcMiiStationWrite(phyAddr, mt7512Ge_cfg[current_idx].ldata[i].reg_num, mt7512Ge_cfg[current_idx].ldata[i].val);
		}
		for( i=0; i<MT7512Ge_PHY_INIT_TR_LDATA_LEN; i++ )
		{         
			mtPhyMiiWrite_TrDbg( phyAddr, mt7512Ge_cfg_tr[current_idx].ldata[i].reg_typ, mt7512Ge_cfg_tr[current_idx].ldata[i].reg_num, mt7512Ge_cfg_tr[current_idx].ldata[i].val,0);
		}
	}
	#if 0
	if(eco_rev >= 0x00050003) // allen_20140530  // allen_20150206
	{
		for(pn=0; pn < tcPhyPortNum; pn++)
		{
			phyAddr = phyAddr_base + pn;
			
			TCPHYDISP3("debug... ,phyaddr=%d ,eco=0x%x \r\n", phyAddr, eco_rev);

			tcMiiStationWrite(phyAddr, 0x1F, 0x0003);  // lpi
			tcMiiStationWrite(phyAddr, 0x15, 0x206a);  // 21 = 0x15
			tcMiiStationWrite(phyAddr, 0x1a, 0x0604);  // 26 = 0x1a

			mtEMiiRegWrite(phyAddr, 0x1e, 0x0120, 0x8d0d);
			mtEMiiRegWrite(phyAddr, 0x1e, 0x0122, 0xffa0);
			mtEMiiRegWrite(phyAddr, 0x1e, 0x00a6, 0x0350);
			mtEMiiRegWrite(phyAddr, 0x1f, 0x0417, 0x7775);  // allen_20150206
			//mtEMiiRegWrite(phyAddr, 0x1e, 0x00a6, 0x0350); // allen_20150206
			mtEMiiRegWrite(phyAddr, 0x7, 0x003c, 0x0000);	// allen_20141121 // allen_20141225
		
			mtPhyMiiWrite_TrDbg(phyAddr, "DSPF", 0x0010, 0x005000, 0);
			mtPhyMiiWrite_TrDbg(phyAddr, "DSPF", 0x0011, 0x040004, 0);
			mtPhyMiiWrite_TrDbg(phyAddr, "DSPF", 0x000e, 0x800008, 0);
		}  
	}
	#endif
	
    TCPHYDISP4("mtphy: CfgLoad %s\r\n",  mt7512Ge_cfg[current_idx].name);
}
#endif  // allen_20130926

/************************************************************************
*                       API for EPHY (TCPHY_SUPPORT)
**************************************************************************/
// function declaration for TCEPHYDBG commands
#ifdef TCPHY_SUPPORT
static int doPhySwVer(int argc, char *argv[], void *p);
static int doPhyMiiRead (int argc, char *argv[], void *p);
static int doPhyMiiWrite(int argc, char *argv[], void *p);
static int doPhyConfig (int argc, char *argv[], void *p);
static int doPhyGphyTestMode (int argc, char *argv[], void *p);
#ifdef TCPHY_1PORT
static int doPhySpeed (int argc, char *argv[], void *p);
#endif
#ifdef TCPHY_DEBUG
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
#ifdef LINUX_OS // allen_20130926 : merge 7502 & 7510/20
static int doPhyDispFlag (int argc, char *argv[], void *p);
#endif
static int doPhyChkVal (int argc, char *argv[], void *p);
static int doPhyForceLink (int argc, char *argv[], void *p);
//static int doErrMonitor (int argc, char *argv[], void *p);
#ifdef TC2031_DEBUG
static int doPhyErrOver (int argc, char *argv[], void *p);
#endif
#ifdef LINUX_OS // allen_20130926 : merge 7502 & 7510/20
#ifdef MTPHY_DEBUG
static int doPhyInit(int argc, char *argv[], void *p);
static int doDbgPorbeSet(int argc, char *argv[], void *p);
static int doReadPhycoeff(int argc, char *argv[], void *p);
static int doPhyMiiRead_TrDbg(int argc, char *argv[], void *p);
static int doPhyMiiWrite_TrDbg(int argc, char *argv[], void *p);
static int doPhyChkVal (int argc, char *argv[], void *p);
static int doPhyMMDRead(int argc, char *argv[], void *p);
static int doPhyMMDWrite(int argc, char *argv[], void *p);
static int doLpbk_setting(int argc, char *argv[], void *p);
static int doGbeTxShaper (int argc, char *argv[], void *p); 	//dai120828
static int doFeTxShaper (int argc, char *argv[], void *p); 		//dai120828
static int doPhyPktCnt(int argc, char *argv[], void *p);
static int doPhySwPatch (int argc, char *argv[], void *p);
static int doSkewDispFlag (int argc, char *argv[], void *p);
static int doPhyDispFlag (int argc, char *argv[], void *p);
static int doPhyForceMode (int argc, char *argv[], void *p);
static int doPhyForceEEE (int argc, char *argv[], void *p);
static int doPhyForceGE (int argc, char *argv[], void *p);
static int doPhyForceReset (int argc, char *argv[], void *p);
static int doPhyrestartAN (int argc, char *argv[], void *p);
static int doPhygetLinkRate (int argc, char *argv[], void *p);
static int doPhygetDuplex (int argc, char *argv[], void *p);
static int doPhygetAN (int argc, char *argv[], void *p);
static int doPhyforcePWD (int argc, char *argv[], void *p);
static int doPhygetLinkStatus (int argc, char *argv[], void *p);
static int doPhyforcePause (int argc, char *argv[], void *p);
static int doPhygetLoopback (int argc, char *argv[], void *p);
static int doPhyforceLoopback (int argc, char *argv[], void *p);
static int doPhygetLPCap (int argc, char *argv[], void *p);
static int doPhygetCALReg (int argc, char *argv[], void *p);
static int doPhyforceLinkDownPowerSaving (int argc, char *argv[], void *p);
static int doPhyset7530_15R (int argc, char *argv[], void *p);
static int doPhyForceLED (int argc, char *argv[], void *p);


#endif
#endif
#endif // TCPHY_DEBUG
#ifdef TCPHY_FIELD_DBGTEST
static int doFieldDebugTest (int argc, char *argv[], void *p);
static int doFieldDebugPrint (int argc, char *argv[], void *p);
#endif // TCPHY_FIELD_DBGTEST
static const cmds_t ethertphyprint[] =
{
    {"ver",       		doPhySwVer,           	0x02, 0, NULL},
    {"miir",      		doPhyMiiRead,         	0x02, 0, NULL},
    {"miiw",      		doPhyMiiWrite,        	0x02, 0, NULL},
    //{"config",    	doPhyConfig,          	0x02, 0, NULL},
   	{"testmode",  		doPhyGphyTestMode,		0x02, 0, "send Giga test mode <1|2|3|4>"},
	#ifdef TCPHY_DEBUG
    //{"reset",     	doPhyInit,          	0x02, 0, NULL},     
    {"swpatch",   		doPhySwPatch,       	0x02, 0, NULL},
    //{"regcheck",  	doPhyRegCheck,  		0x02, 0, NULL},   
    {"send",      		doMacSend,      		0x02, 0x3, "send <pattern> <len> <loop>"},
    {"sendrandom", 		doMacSendRandom, 		0x02, 0, NULL},
	#if 0
	#ifndef PURE_BRIDGE     
    {"ping",      		doPing,        			0x02, 3, "ping <ip> <len> <loopnum> [delay_cnt]"},
    {"pingechocnt", 	doPingEchoCnt, 			0x02, 1, "pingechocnt <display | clear>"},
	#endif
	#endif
    //{"dispflag",    	doPhyDispFlag,  		0x02, 0, NULL},          
    //{"forcelink",   	doPhyForceLink,  		0x02, 0, NULL},      
    //{"errmonitor",  	doErrMonitor,         	0x02, 0, NULL},    
	#ifdef LINUX_OS // allen_20130926 : merge 7502 & 7510/20
	#ifdef MTPHY_DEBUG 
	{"chkval",  		doPhyChkVal,  			0x02, 1, "chkval <phyaddr>"},  
	{"emiir",			doPhyMMDRead,		  	0x02, 0, NULL},
	{"emiiw",			doPhyMMDWrite,		  	0x02, 0, NULL},
	//{"emiiwb",		doPhyMMDWrite,		  	0x02, 0, NULL},
	//{"miiwb",	  		doPhyMiiWrite,			0x02, 0, NULL},
	{"dbgprb",	  		doDbgPorbeSet,			0x02, 0, NULL},
	{"miir_trdbg",		doPhyMiiRead_TrDbg,		0x02, 0, NULL},
	{"miiw_trdbg",  	doPhyMiiWrite_TrDbg,	0x02, 0, NULL},
	//{"coeffr", 		doReadPhycoeff, 		0x02, 0, NULL},
	//{"lbpk",			doLpbk_setting,			0x02, 0, NULL},
	//{"gtxshaper", 	doGbeTxShaper, 			0x02, 0, NULL},
	//{"ftxshaper", 	doFeTxShaper, 			0x02, 0, NULL},
	//{"phycnt",		doPhyPktCnt,			0x02, 0, NULL},
	//{"swpatch",   	doPhySwPatch,         	0x02, 1, "swpatch 		<on|off>"},
	//{"dispflag",    	doPhyDispFlag,  		0x02, 0, NULL},
	//{"Skewdispflag", 	doSkewDispFlag,  		0x02, 0, NULL},
	{"forcemode",  		doPhyForceMode,  		0x02, 1, "forcemode <phyaddr> <an|force> <10|100|1000> <full|half>"}, 
	{"forceEEE",  		doPhyForceEEE,  		0x02, 1, "forceEEE <phyaddr> <on|off|recover>"},
	{"forceGE",  		doPhyForceGE,  			0x02, 1, "forceGE <phyaddr> <multi|single> <master|slave>"},
	{"forceReset", 		doPhyForceReset,		0x02, 1, "forceReset <phyaddr> "},
	{"restartAN", 		doPhyrestartAN,			0x02, 1, "restartAN <phyaddr> "}, 
	{"getLinkRate", 	doPhygetLinkRate,		0x02, 1, "getLinkRate <phyaddr> "}, 
	{"getDuplex", 		doPhygetDuplex,			0x02, 1, "getDuplex <phyaddr> "},
	{"getAN", 			doPhygetAN,				0x02, 1, "getAN <phyaddr> "},
	{"forcePWD", 		doPhyforcePWD,			0x02, 1, "forcePWD <phyaddr> "},
	{"getLinkStatus", 	doPhygetLinkStatus,		0x02, 1, "getLinkStatus <phyaddr> "},
	{"forcePause", 		doPhyforcePause,		0x02, 1, "forcePause <phyaddr> <on|off> "},
	{"getLoopback", 	doPhygetLoopback,		0x02, 1, "getLoopback <phyaddr> "},
	{"forceLoopback", 	doPhyforceLoopback,		0x02, 1, "forceLoopback <phyaddr> "},
	{"getLPCap", 		doPhygetLPCap,			0x02, 1, "getLPCap <phyaddr> "},
	{"getCAL", 			doPhygetCALReg,			0x02, 1, "getCAL <phyaddr> "}, 
    {"forceLDPS", 		doPhyforceLinkDownPowerSaving,	0x02, 1, "forceLDPS <phyaddr> <on|off> "}, 
	{"set7530_15R", 	doPhyset7530_15R,		0x02, 1, "set7530_15R <on|off>"}, 
	{"forceLED", 		doPhyForceLED,			0x02, 1, "forceLED <phyaddr> <enable|disable>"}, 
	#endif
	#endif
	#endif // TCPHY_DEBUG
	#ifdef TCPHY_FIELD_DBGTEST
    //{"dbgtest",  	doFieldDebugTest, 		0x02, 0, NULL},      
    //{"dbgprint", 	doFieldDebugPrint, 		0x02, 0, NULL},
	#endif // TCPHY_FIELD_DBGTEST   
    {NULL,			NULL,					0,	0,	NULL},
};

/*** public, Called by tc3162l2mac.c ***/
int tcPhyPortNumGet(void)
{
	if(tcPhyVer != 99)
		return tcPhyPortNum;
	else
		return 0;
}

// ************************************************************************
// 		set tcPhyFlag & tcPhyVer
//		[in] ephy_addr
// ************************************************************************
//int tcPhyVerLookUp(macAdapter_t *mac_p)
int tcPhyVerLookUp(uint16 ephy_addr)
{
    uint32 rval;
	uint16 r15_temp;

	ephy_addr_base = 0;

	if(tcPhyVer == mtPhyVer_7530)
		mt7530_flag = 1;

	TCPHYDISP3(" [tcPhyVerLookUp] (%d)...in \r\n", ephy_addr);
	
	r15_temp = tcMiiStationRead(ephy_addr, 15);
	//printk(" [tcPhyVerLookUp] (%d)...in...in...in...in...in, r15=0x%x \r\n", ephy_addr, r15_temp);

    rval = tcMiiStationRead(ephy_addr, 3); // phy revision id
	if(rval == 0xffff)
	{
		#ifdef LINUX_OS 
		rval = tcMiiStationRead(ephy_addr, 3); 	// phy revision id
		#endif
	}
	else
	{
		fgMT7530_INT = 1;
	}	
    rval &= 0xffff;
	printk("%s %d %X \n", __func__, __LINE__, rval);  // JasonG_7512
    if(rval == EPHY_ID_2031)
	{
        tcPhyVer = tcPhyVer_2031;         
        tcPhyPortNum = 1;
        TCPHYDISP1("TC2031, ");
    }
	else if(rval == EPHY_ID_2101mb)
    {
        tcPhyVer = tcPhyVer_2101mb;
        tcPhyPortNum = 1;
        TCPHYDISP1("TC2101MB, ");
    }
	else if(rval == EPHY_ID_2104mc)
	{
        tcPhyVer = tcPhyVer_2104mc;
        tcPhyPortNum = 4;
        TCPHYDISP1("TC2104MC, ");
    }
	else if(rval == EPHY_ID_2104sd)
	{
        tcPhyVer = tcPhyVer_2104sd;
        tcPhyPortNum = 4;
        TCPHYDISP1("TC2104SD, ");
    }
	else if (rval == EPHY_ID_2101me)
	{
        tcPhyVer = tcPhyVer_2101me;
        tcPhyPortNum = 1;
        TCPHYDISP1("TC2101ME, ");
	}
	else if(rval == EPHY_ID_2102me)
	{
        tcPhyVer = tcPhyVer_2102me;
        tcPhyPortNum = 1;
        TCPHYDISP1("TC2102ME, ");	
	}
	else if (rval == EPHY_ID_2104me)
	{
        tcPhyVer = tcPhyVer_2104me;
        tcPhyPortNum = 4;
        TCPHYDISP1("TC2104ME, ");
	}
	else if(rval == EPHY_ID_2101mf)
	{
        tcPhyVer = tcPhyVer_2101mf;
        tcPhyPortNum = 1;
        TCPHYDISP1("TC2101MF, ");
    }
	else if(rval == EPHY_ID_2105sg)
	{
        tcPhyVer = tcPhyVer_2105sg;
        tcPhyPortNum = 5;
        TCPHYDISP1("TC2105SG, ");
	}
	else if(rval == EPHY_ID_2101mi)
	{
        tcPhyVer = tcPhyVer_2101mi;
        tcPhyPortNum = 1;
        TCPHYDISP1("TC2101MI, ");	
   	}
	else if(rval == EPHY_ID_2105mj)
	{
        tcPhyVer = tcPhyVer_2105mj;
        tcPhyPortNum = 5;
        TCPHYDISP1("TC2105MJ, "); 
    }
	else if(rval == EPHY_ID_2105sk)
	{
        tcPhyVer = tcPhyVer_2105sk;
        tcPhyPortNum = 5;
        TCPHYDISP1("TC2105SK, ");
    }
	else if(rval == EPHY_ID_2101mm)
	{
        tcPhyVer = tcPhyVer_2101mm;
		tcPhyPortNum = 1;
		TCPHYDISP1("TC2101MM, ");
   	}
	else if(rval == EPHY_ID_7510Fe)
	{
        tcPhyVer = tcPhyVer_mt7510FE;
        tcPhyPortNum = 4;
        TCPHYDISP1(" 7510 Fe, \n");
        tcPhyFlag = 1;
        tcPhyInitFlag = 1;
    } 
	else if(rval == EPHY_ID_7505Fe)
	{
        tcPhyVer = tcPhyVer_mt7505FE;
        tcPhyPortNum = 5;
        TCPHYDISP1(" 7505 Fe, \n");	
    } 
	else if(rval == EPHY_ID_7530Ge_7512Fe)
	{ 	
		if((r15_temp & 0x2000)== 0x2000)
		{
			//printk("r15 = 0x%x, (0x%x) \n", r15_temp, (r15_temp & 0x2000));
			
	        tcPhyVer = mtPhyVer_7530;
    	    tcPhyPortNum = 5;
        	TCPHYDISP1(" 7530 Ge, 0x%x \n", fgMT7530_INT); 
        	tcPhyFlag = 1;
        	tcPhyInitFlag = 1;
        	mt7530_flag = 1;
        }
        else  // *********************************************************
        {
        	//printk("r15 = 0x%x, (0x%x) \n", r15_temp, (r15_temp & 0x2000));

			tcPhyVer = tcPhyVer_7512Fe;
    	    tcPhyPortNum = 4;
        	TCPHYDISP1(" 7512 Fe, \n");
        	tcPhyFlag = 1;
        	tcPhyInitFlag = 1;
        }
    } 
	else if(rval == EPHY_ID_7510Ge)
	{ 
        tcPhyVer = mtPhyVer_7510Ge;  
        tcPhyPortNum = 1;
		eco_rev = VPint(0xbfb00064);
        TCPHYDISP1(" 7510 Ge, 0x%x, eco=0x%x \n", fgMT7510Ge_INT, eco_rev);		// internal check flag 
        tcPhyFlag = 1;
        tcPhyInitFlag = 1;
    }
    else if(rval == EPHY_ID_7512Ge)  // ***************************
	{
        tcPhyVer = tcPhyVer_7512Ge;		
        tcPhyPortNum = 1;
        TCPHYDISP1(" 7512 Ge, \n");
        tcPhyFlag = 1;
        tcPhyInitFlag = 1;
    } 
    else 
	{
        TCPHYDISP3(" unknown PHYID: %lx, \n", rval);        
        tcPhyInitFlag = 0;
    }

	if(tcPhyVer == mtPhyVer_7530)
	{ 	//CML_20130226_2
		eco_rev = (mtEMiiRegRead(ephy_addr, 0x1f, 0) >> 4);
	}
	else
	{
		eco_rev = tcMiiStationRead(ephy_addr, 31);
    }
	eco_rev &= (0x0f);
	
	if(mt7530_flag == 1)
	{
		tcPhyVer = mtPhyVer_7530;
        tcPhyPortNum = 5;
        TCPHYDISP1("MCM MT7530 \n");
	}
	TCPHYDISP3(" [tcPhyVerLookUp] (%d)(%d)...out	 \r\n", ephy_addr, tcPhyVer);
	//printk(" [tcPhyVerLookUp] (%d)(%d)...out...out...out...out...out	 \r\n", ephy_addr, tcPhyVer);

	if(ephy_addr_base == 0) // allen_20140822
	{
		if(mt7530_flag == 0)
		{
			if((tcPhyVer == tcPhyVer_7512Ge)||(tcPhyVer == tcPhyVer_7512Fe))
				ephy_addr_base += 8;
		}
	}
	else
	{
		if(tcPhyVer == mtPhyVer_7530)
			ephy_addr_base = 0;
	}
	
    return 0;
}


// ************************************************************************
// 		tcPhy initial: reset, load default register setting, restat AN
//		[in] *mac_p
// ************************************************************************
int tcPhyInit(void)
{
	uint16	ephy_addr, port_num;

	uint32 one_tick_unit = 1 * SYS_HCLK * 1000 / 2;
	volatile uint32 timer_1st, timer_2nd, timer_3rd;
	volatile uint32 timer_1_1, timer_1_2;
	volatile uint32 timer_2_1, timer_2_2;
	volatile uint32 timer_3_1, timer_3_2;
	unsigned long	time_1, time_2, time_diff;
	unsigned long	time_1_1, time_2_1, time_1_diff, time_2_diff;

	//#define FEMAC_START_TIMER(timer)			{ mod_timer(&timer, (jiffies + ((timer.data*HZ)/1000))) ; }//{ mod_timer(&timer, (jiffies + ((timer.data)/HZ))) ; }
	time_1 = jiffies; 
	//printf("========== allen_test ========== jiffies_1: %ld, %ld \r\n", (time_1), jiffies);
 	//timer_1st = VPint(CR_TIMER1_VLR);
 	//printf("========== allen_test ========== ms unit: %d \r\n", (one_tick_unit));
 	//printf("========== allen_test ========== ephy 1st: %d \r\n", (timer_1st));
 	
	if(ecnt_register_hook(&ecnt_driver_ephy_op)) {
		printk("ecnt_driver_ephy_op register fail\n");
		return -ENODEV ;
    }  // JasonG_7512

	if (isFPGA) 
	{	//xyyou add for 7505 FPGA phy addr1 and addr2 reset 			
    	tcMiiStationWrite(1, MII_BMCR, BMCR_RESET); 	
    	pause(10);			
    	tcMiiStationWrite(1, MII_BMCR, BMCR_ANRESTART | BMCR_ANENABLE); 
    	pause(10);			
    	tcMiiStationWrite(2, MII_BMCR, BMCR_RESET); 	
    	pause(10);			
    	tcMiiStationWrite(2, MII_BMCR, BMCR_ANRESTART | BMCR_ANENABLE); 
	}
	else
	{
		//		timer_1_1 = VPint(CR_TIMER1_VLR);
		//	printf("========== allen_test ========== ephy 1-1: %d \r\n", (timer_1_1));

		#if 0	// allen_20160831
		ephy_addr_base = 0;
		for(ephy_addr = 0; ephy_addr <= 12; ephy_addr++)
		{
    		tcPhyVerLookUp(ephy_addr);
    		if(tcPhyInitFlag == 1)
    		{
    			port_num = ephy_addr - ephy_addr_base;
    			//TCPHYDISP1(", port_num = %d, ephy_addr = %d, ephy_addr_base = 0x%x \r\n", port_num, ephy_addr, ephy_addr_base);

    			if(port_num >= 0)
    			{
    				ePhyInit(ephy_addr);
    			}	
    			//		timer_1_2 = VPint(CR_TIMER1_VLR);
				//	printf("========== allen_test ========== ephy 1-2: %d \r\n", (timer_1_2));
    		}	
    	}
    	#endif

		//    	timer_2nd = VPint(CR_TIMER1_VLR);
 		//		printf("========== allen_test ========== ephy 2nd: %d \r\n", (timer_2nd));
 		time_1_1 = jiffies;
		time_1_diff = time_1_1 - time_1; 
		//printf("========== allen_test ========== jiffies_1_1: %ld, %ld \r\n", (time_1_1), jiffies);
		//printf("========== allen_test ========== jiffies_1_diff: %ld, %ld \r\n", (time_1_diff), HZ);
		//	timer_2_1 = VPint(CR_TIMER1_VLR);
 		//printf("========== allen_test ========== ephy 2-1: %d \r\n", (timer_2_1));
		
		GECal_flag = 0;
    	doGePhyALLAnalogCal(12); 
	
		//    	timer_2_2 = VPint(CR_TIMER1_VLR);
 		//		printf("========== allen_test ========== ephy 2-2: %d \r\n", (timer_2_2));
	
		#if defined(TCSUPPORT_CPU_EN7521) // allen_20160602
		#if defined(TCSUPPORT_AUTOBENCH)  // for SLT (SD2/Trey's request)
		for(ephy_addr = 8; ephy_addr <= 11; ephy_addr++)
		#else
		for(ephy_addr = 9; ephy_addr <= 11; ephy_addr++)
		#endif
		#else
		for(ephy_addr = 8; ephy_addr <= 11; ephy_addr++)
		#endif
		{
    		tcPhyVerLookUp(ephy_addr);
    		if(tcPhyInitFlag == 1)
    		{
    			port_num = ephy_addr - ephy_addr_base;
    			//TCPHYDISP1(", port_num = %d, ephy_addr = %d, ephy_addr_base = 0x%x \r\n", port_num, ephy_addr, ephy_addr_base);

    			if(port_num >= 0)
    			{
    				if(tcPhyVer==tcPhyVer_7512Fe)
    				{
						FECal_flag = 0;
						FECal_tx_offset_flag = 0;
						FECal_R50_flag = 0;
    					doFEPhyALLAnalogCal(port_num);
    					
    					delay1ms(100);
    				}
    			}	
   				// 			timer_3_1 = VPint(CR_TIMER1_VLR);
				//			printf("========== allen_test ========== ephy 3_1: %d \r\n", (timer_3_1));
    		}	
    	}

    	ephy_addr_base = 0;
		for(ephy_addr = 0; ephy_addr <= 12; ephy_addr++)
		{
    		tcPhyVerLookUp(ephy_addr);
    		if(tcPhyInitFlag == 1)
    		{
    			port_num = ephy_addr - ephy_addr_base;
    			//TCPHYDISP1(", port_num = %d, ephy_addr = %d, ephy_addr_base = 0x%x \r\n", port_num, ephy_addr, ephy_addr_base);

    			if(port_num >= 0)
    			{
    				ePhyInit(ephy_addr);
    			}	
    			//		timer_1_2 = VPint(CR_TIMER1_VLR);
				//	printf("========== allen_test ========== ephy 1-2: %d \r\n", (timer_1_2));
    		}	
    	}
	}

	time_2_1 = jiffies;
	time_2_diff = time_2_1 - time_1_1; 
	//printf("========== allen_test ========== jiffies_2_1: %ld, %ld \r\n", (time_2_1), jiffies);
	//printf("========== allen_test ========== jiffies_2_diff: %ld, %ld \r\n", (time_2_diff), HZ);
		
	tcephydbgcmd();

	//	timer_3rd = VPint(CR_TIMER1_VLR);
 	//	printf("========== allen_test ========== ephy 3rd: %d \r\n", (timer_3rd));
	time_2 = jiffies;
	time_diff = time_2 - time_1; 
	//printf("========== allen_test ========== jiffies_2: %ld, %ld \r\n", (time_2), jiffies);
	//printf("========== allen_test ========== jiffies_diff: %ld, %ld \r\n", (time_diff), HZ);

	return 0;
}


int tcPhyDeinit(void)  // JasonG_7512
{
    ecnt_unregister_hook(&ecnt_driver_ephy_op);
}



int ePhyInit(uint16 ephy_addr)
{
    int i=0;
   // uint16 start_addr = ephy_addr;
	uint32 reg_value; //CML_20130226_1

	TCPHYDISP1(" [tcPhyInit] in ");
	//TCPHYDISP1(", tcphyver = %d, phyaddr = %d, eco = 0x%x \r\n", tcPhyVer, start_addr, eco_rev);
	
	switch(tcPhyVer) 
	{			    
		case tcPhyVer_7512Fe: 		// MT7512 FE
		case tcPhyVer_7512Ge: 		// MT7512 Ge
			if(ephy_addr_base == 0)
				ephy_addr_base += 8;
			break;
	}

    //for(i=start_addr; i<start_addr+tcPhyPortNum; i++ )
	//{            
        tcMiiStationWrite(ephy_addr, PHY_CONTROL_REG, PHY_RESET);
	//}

    switch(tcPhyVer) 
	{
		#ifdef LINUX_OS 			// allen_20130926
		case mtPhyVer_7530: 		// MT7530
			mt7530GePhyCfgLoad(0, DO_5_PORT, 0);
			break;

		case tcPhyVer_7512Fe: 		// MT7512 FE
		case tcPhyVer_7512Ge: 		// MT7512 Ge
			mt7512FECfgLoad(0, DO_4_PORT, 8);
			mt7512GePhyCfgLoad(0);   
			break;
		#endif
    }

    // always boot-up with AN-enable
    //for(i=start_addr; i<start_addr+tcPhyPortNum; i++ )
	//{            
        //tcMiiStationWrite(i, PHY_CONTROL_REG, MIIDR_AUTO_NEGOTIATE );
		// rewrite to avoid changing H/W default setting //CML_20130226_1
		#ifdef LINUX_OS // allen_20130926
		reg_value = tcMiiStationRead(ephy_addr, PHY_CONTROL_REG);
		reg_value |= MIIDR_AUTO_NEGOTIATE;
        tcMiiStationWrite(ephy_addr, PHY_CONTROL_REG, reg_value );
		#else
		tcMiiStationWrite(ephy_addr, PHY_CONTROL_REG, MIIDR_AUTO_NEGOTIATE );
		#endif
    //}

    // tcphy_link_state init.
    for(i=0; i<tcPhyPortNum; i++)
    {
        tcphy_link_state[i] = ST_LINK_DOWN;
    }
	TCPHYDISP1(" [tcPhyInit] out ");
	TCPHYDISP1(", r31 = (%d, 0x%x) \r\n", ephy_addr, tcMiiStationRead(ephy_addr, 31));


	
    return 0;
}

// tcPhy initial: reset, load default register setting, restat AN
int tcPhyPortInit(uint8 port_num)
{
	uint32 reg_value; // CML_20130226_1
	
    #ifndef LINUX_OS
    tcMiiStationWrite(port_num, PHY_CONTROL_REG, PHY_RESET);
	#endif

    switch(tcPhyVer) 
	{     
		case tcPhyVer_7512Fe: 		// mt7512 FE
			mt7512FECfgLoad(0, DO_PER_PORT, port_num);
			break;
    }

    // always boot-up with AN-enable         
    //tcMiiStationWrite(port_num, PHY_CONTROL_REG, MIIDR_AUTO_NEGOTIATE );
    // rewrite to avoid changing H/W default setting //CML_20130226_1
	//tcMiiStationWrite(port_num, PHY_CONTROL_REG, MIIDR_AUTO_NEGOTIATE );
	reg_value = tcMiiStationRead(port_num, PHY_CONTROL_REG);
	reg_value |= MIIDR_AUTO_NEGOTIATE;
	tcMiiStationWrite(port_num, PHY_CONTROL_REG, reg_value);
	
    // tcphy_link_state init.
    tcphy_link_state[port_num]=ST_LINK_DOWN;
    return 0;
}

// return LP0Reg1 PHY Status Registers Value
uint8 getTcPhyFlag(void)
{
    return tcPhyFlag;
}

uint32 getTcPhyStatusReg(int phy_add)
{
	// no called if (force_link_flag==1)
	/*
	#ifdef PHYPART_DEBUG
	printf("PhyPart debug: getTcPhyStatusReg() in \r\n");
	#endif
	*/
    if(!sw_patch_flag)//if sw patch off, mac get PHY status via MDIO directly.
    {
		/*    
		#ifdef PHYPART_DEBUG
		printf("PhyPart debug: getTcPhyStatusReg() out \r\n");
		#endif
		*/
		return ( tcMiiStationRead(phy_add, PHY_STATUS_REG) );
    }
    else//if sw patch on, mac get the PHY status that patch intercept. 
    {
	    /*
		#ifdef PHYPART_DEBUG
		printf("PhyPart debug: getTcPhyStatusReg() out \r\n");
		#endif
		*/
		return (Nmr1[0].value); //mr1_link_status_reg;
	}
}

uint32 getTcPhyRMCAPReg(int phy_add)
{
#if 0
	#ifdef TC2101MI_SUPPORT
	if ((tcPhyVer==tcPhyVer_2101mi) && sw_patch_flag)
		return tc2101mi_reg5_val;
	else
	#endif
#endif	
	return ( tcMiiStationRead(phy_add, PHY_REMOTE_CAP_REG));	
}

uint16 getTcPhyNStatusReg(uint8 port_num)
{
    uint16 phyAddr;

    if(!sw_patch_flag) 
	{
        phyAddr = ephy_addr_base + port_num;
        return ( tcMiiStationRead(phyAddr, PHY_STATUS_REG) ); 
    }
    else 
	{
        return (Nmr1[port_num].value);
    }
}

bool getTcPhyEsdDetectFlag(void)
{
	return 0;
}
	
// Software Patch for TrendChip's ethernet PHY
int tcPhySwPatch(void)
{
    if(!sw_patch_flag)
    {
       	Skew_update_flag=0;
    	return 0;
    }
	#ifdef PHYPART_DEBUG_SW_PATCH
	printf("PhyPart debug: tcPhySwPatch() in  \r\n");
	#endif
    switch (tcPhyVer) 
	{  
    	case mtPhyVer_7530: 
       		mt7530GePhySwPatch();
      		break;

      	case tcPhyVer_7512Fe: 		// 7510Fe	
		case tcPhyVer_7512Ge: 
			mt7512GePhySwPatch();  
			mt7512FESwPatch();			
			break;
    }
	#ifdef PHYPART_DEBUG_SW_PATCH
	printf("PhyPart debug: tcPhySwPatch() out  \r\n");
	#endif
    return 0;
}

// CI command for tcephydbg
int doEtherPhydbg(int argc, char *argv[], void *p) 
{
    return subcmd(ethertphyprint, argc, argv, p);
} 
 
#ifdef LINUX_OS
/*=========================================================================
**   tcephydbgcmd: register rootcommand of "ether ". 
**           call: call by femac.c/mainInit(), tc3262_gmac_init
**=========================================================================*/
void tcephydbgcmd(void)
{
	cmds_t tcephydbg_cmd;

	tcephydbg_cmd.name = "tce";
	tcephydbg_cmd.func = doEtherPhydbg;
	tcephydbg_cmd.flags = 0x12;
	tcephydbg_cmd.argcmin = 0;
	tcephydbg_cmd.argc_errmsg = NULL;
	cmd_register(&tcephydbg_cmd);
} /*end tcephydbgcmd*/

void tcephydbgcmd1(void)
{
	cmds_t tcephydbg_cmd;

	tcephydbg_cmd.name = "tce1";
	tcephydbg_cmd.func = doEtherPhydbg;
	tcephydbg_cmd.flags = 0x12;
	tcephydbg_cmd.argcmin = 0;
	tcephydbg_cmd.argc_errmsg = NULL;
	cmd_register(&tcephydbg_cmd);
} /*end tcephydbgcmd*/
#endif

#endif //TCPHY_SUPPORT
#ifdef TCPHY_DEBUG
uint8 getTcPhyLookbackFlag(void)
{
    return phy_loopback_flag;
}
uint8 getTcPhyForceLinkFlag(void)
{	
    return force_link_flag;
}
#endif //TCPHY_DEBUG

/************************************************************************
*         API functions body for EPHY (TCPHY_SUPPORT)
**************************************************************************/
#ifdef TCPHY_SUPPORT
static int doPhySwVer(int argc, char *argv[], void *p)
{
	printf("Ver : ");

    switch (tcPhyVer) 
	{    
		case tcPhyVer_7512Fe: 					// en7512 FE
			printf("%s ", mt7512FE_cfg[0].name);
			printf(" (%d)\r\n", eco_rev);
			mt7512FECfgCheck();
			break;
		
		case tcPhyVer_7512Ge: 					// en7512 Ge
			printf("%s, %s \r\n", mt7512Ge_cfg[0].name, mt7512FE_cfg[0].name);
		 	printf("tcPhyVer_7512Ge, tcPhyVer=0x%02lX \r\n", tcPhyVer);
			break;
		
		case mtPhyVer_7530: 					// mt7530
			printf("%s \r\n", mt7530_cfg[0].name );
			printf("mtPhyVer_7530, tcPhyVer=0x%02lX \r\n", tcPhyVer);
			break;	
		
        default: 
	    	printf("Unknown PHY, tcPhyVer=0x%02lX \r\n", tcPhyVer);
    }

	printf(": 2016-09-01      \r\n");  
	return 0;
} 

uint16 getMiiPage(char *page)
{
    // g0,g1,g2,g3,g4,g5 = 0x0000~0x5000
    // l0,l1,l2,l3 = 0x8000~0xb000
    if(stricmp(page, "g0") == 0)
    {
        return (0x0000);
    }
    else if(stricmp(page, "g1") == 0)
    {
        return (0x1000);
    }
    else if(stricmp(page, "g2") == 0)
    {
        return (0x2000);
    }
    else if(stricmp(page, "g3") == 0)
    {
        return (0x3000);
    }
	else if(stricmp(page, "g4") == 0)
	{
        return (0x4000);
    }
	else if(stricmp(page, "g5") == 0)
	{
        return (0x5000);
    }
	else if(stricmp(page, "g6") == 0)
	{
        return (0x6000);
    }
    else if(stricmp(page, "g7") == 0)
	{
        return (0x7000);
    }
    else if(stricmp(page, "l0") == 0)
    {
        return (0x8000);
    }
    else if(stricmp(page, "l1") == 0)
    {
        return (0x9000);
    }
    else if(stricmp(page, "l2") == 0)
    {
        return (0xa000);
    }
    else if(stricmp(page, "l3") == 0)
    {
        return (0xb000);
    }
    else if(stricmp(page, "l4") == 0)
    {
        return (0xc000);
    }
	else if(stricmp(page, "ext") == 0)
	{	//Extended Page Registers
	    return (0x0001);
	}
	else if(stricmp(page, "misc") == 0)
	{	//Misc Page Registers
	    return (0x0002);
	}
	else if(stricmp(page, "lpi") == 0)
	{	//LPI Page Registers
	    return (0x0003);
	}
	else if( stricmp(page, "test") == 0)
	{	//Test Page Registers
	    return (0x2a30);
	}
	else if(stricmp(page, "tr") == 0)
	{	//Token Ring Debug Registers
		return (0x52b5);
	}
    else 
    {
        printf("* Wrong PageNo(%s).\r\n",page);
	    printf("* MTGPHY PageNo=><g0|ext|misc|lpi|test|tr> \r\n");
	    printf("* Tcxxx PageNo=><g0|g1|g2|g3|g4|g5|l0|l1|l2|l3|l4> \r\n");
        return (0xFFFF);
    }
}

uint16 checked_atoi(char *val)
{
	#ifdef TCPHY_DEBUG
    // only check 1st char
    if(val[0]<'0' || val[0]>'9')
        return (0xffff);
	#endif
    return (atoi(val));
}

int	doPhyMiiRead(int argc, char *argv[], void *p)
{
    uint16 	phyaddr=0;
    const 	uint16 page_reg=31;
    uint16 	page_val=0;
    uint32 	reg=0;
    uint32 	value;
    int i;

    //  argc:3
    //  tce miir all <PhyAddr> 
    //  tce miir <PhyAddr> <RegAddr>
    //  argc:4
    //  tce miir all <PhyAddr> <PageNo>
    //  tce miir <PhyAddr> <PageNo> <RegAddr>

    // get parameters
    if(argc==3)
    {
        if(stricmp(argv[1], "all") == 0)
        { 	// tce miir all <PhyAddr> 
            phyaddr = checked_atoi(argv[2]);
        }
        else
        { 	// tce miir <PhyAddr> <RegAddr>
            phyaddr = checked_atoi(argv[1]);
            reg = checked_atoi(argv[2]);
           	//sscanf(argv[2], "%lx", &reg);
		   //printf("*1: PhyAddr=%d, RegAddr=0x%02lX\r\n", phyaddr, reg);	   
        }
    }
    else if(argc==4)
    {
        if(stricmp(argv[1], "all") == 0)
        { 	// tce miir all <PhyAddr> <PageNo>
            phyaddr = checked_atoi(argv[2]);
            page_val = getMiiPage(argv[3]);
        }
        else
        { 	// tce miir <PhyAddr> <PageNo> <RegAddr>
            phyaddr = checked_atoi(argv[1]);
            page_val = getMiiPage(argv[2]);
            reg = checked_atoi(argv[3]);
            //sscanf(argv[3], "%lX", &reg);
        }
    }

    // check parameters
    if((argc==3 || argc==4) 
        && phyaddr<=31 && reg<=31 && page_val!=0xffff)
    {
        // set page
        if(!isFPGA)
		{	
        	if (argc==4)
        	{           
            	if(stricmp(argv[1], "all") == 0)
            	{ 	// multiple read
                	printf("* PageNo=%s ",argv[3]);
                	printf("\r\n");             
            	}
            	else
            	{
                	printf("* PageNo=%s ",argv[2]);
            	}
				#ifdef LINUX_OS // allen_20130926
				tcMiiStationWrite(phyaddr, page_reg, page_val);
				#else
				tcMiiStationWrite(phyaddr, page_reg, page_val);
				#endif	
        	}
		}//if(!isFPGA)
		
        // read data
        if(stricmp(argv[1], "all") == 0)
		{	// multiple read
           	for( i=0; i<32; i++ )
			{
				#ifdef LINUX_OS // allen_20130926	
				value = tcMiiStationRead(phyaddr, i);
				#else
				value = tcMiiStationRead(phyaddr, i);
				#endif
                printf("[reg=%02d val=%04lX]", i, value);
                if( (i+1) % 4 == 0 )
                    printf("\r\n");             
            }
        }
        else
		{
			#ifdef LINUX_OS // allen_20130926
			value = tcMiiStationRead(phyaddr, reg);
			#else
			value = tcMiiStationRead(phyaddr, reg);
			#endif
            printf("* PhyAddr=%d RegAddr=%02d value=%04lX\r\n", phyaddr, reg, value);
        }       
        return 0;           
    }   
    else
    { 	// error message
        printf("Usage: miir all <PhyAddr> [PageNo]\r\n");
        printf("       miir <PhyAddr> <RegAddr>\r\n");
        printf("       miir <PhyAddr> <PageNo> <RegAddr>\r\n");
        return 0;           
    }
}

int doPhyMiiWrite (int argc, char *argv[], void *p)
{
    uint16 	phyaddr=0;
    const 	uint16 page_reg=31;
    uint16 	page_val=0;
	uint32 	reg=0;
    uint32 	value=0;
	uint16 	STBit=0;
	uint16 	BFlen=0;
	uint16 	BF,BFMsk=0;
	uint32 	RValue =0;
	int 	i;
    //  tce miir <PhyAddr> <RegAddr> <Value>
    //  tce miir <PhyAddr> <PageNo> <RegAddr> <Value>
    // get parameters
    if(argc==4)
	{
        phyaddr = checked_atoi(argv[1]);
		reg = checked_atoi(argv[2]);
		//sscanf(argv[2], "%lx", &reg);
        sscanf(argv[3], "%lx", &value);
		//printf("*1: argv[2]=%s, argv[3]=%s\r\n", argv[2], argv[3]);		
		//printf("*2: phyaddr=%d, Reg=0x%02lX, value=0x%04lX\r\n", phyaddr, reg , value);		
    }
    else if (argc==5)
    {
        phyaddr = checked_atoi(argv[1]);
        page_val = getMiiPage(argv[2]);
        reg = checked_atoi(argv[3]);
        //sscanf(argv[3], "%lx", &reg);
        sscanf(argv[4], "%lx", &value);
    }
	else if (argc==6)
	{
		if(stricmp(argv[0], "miiwb") == 0)
		{
			phyaddr = checked_atoi(argv[1]);
		    reg = checked_atoi(argv[2]);
			//sscanf(argv[2], "%lx", &reg);
			STBit = checked_atoi(argv[3]);
			BFlen = checked_atoi(argv[4]);
			sscanf(argv[5], "%lx", &value);
			printf("* Phyaddr=%d, RegAddr=%02d, STBit=%0d, BFlen=%0d, value=%04lX\r\n", phyaddr, reg, STBit, BFlen, value);			
		}
	}
	else if (argc==7)
	{
		if(stricmp(argv[0], "miiwb") == 0)
		{
			phyaddr = checked_atoi(argv[1]);
			page_val = getMiiPage(argv[2]);
			reg = checked_atoi(argv[3]);
			//sscanf(argv[3], "%lx", &reg);
			STBit = checked_atoi(argv[4]);
			BFlen = checked_atoi(argv[5]);
			sscanf(argv[6], "%lx", &value);
			printf("* Phyaddr=%d, pageNo=%d, RegAddr=%02d, STBit=%0d, BFlen=%0d, value=%04lX\r\n", phyaddr, page_val, reg, STBit, BFlen, value);			
		}
	}
		
    // check parameters and write
    if ((argc==4 || argc==5) 
        && (phyaddr<=31) && (reg<=31) && (page_val!=0xffff))
	{
        // set page
        if(!isFPGA)
		{
       		if (argc==5) 
			{
        	    printf("* PageNo=%s ",argv[2]);
				#ifdef LINUX_OS // allen_20130926			
				tcMiiStationWrite(phyaddr, page_reg, page_val);
				#else
				tcMiiStationWrite(phyaddr, page_reg, page_val);
				#endif	
        	}
	   	}
        // write data
        printf("* Phyaddr=%d RegAddr=%02d value=%04lX\r\n", phyaddr, reg, value);
		#ifdef LINUX_OS // allen_20130926	
		tcMiiStationWrite(phyaddr, reg, value);
		#else
		tcMiiStationWrite(phyaddr, reg, value);
		#endif
    }
	#ifdef LINUX_OS // allen_20130926
 	else if (((argc == 6) || (argc == 7))
		&& (phyaddr<=31) && (reg<=31) && (page_val != 0xffff))
	{
		if(!isFPGA)
		{
        	if (argc==7) 
			{
            	printf("* PageNo=%s ",argv[2]);
				tcMiiStationWrite(phyaddr, page_reg, page_val);
        	}
		}
		
		if(stricmp(argv[0], "miiwb") == 0)
		{
		 	RValue = tcMiiStationRead(phyaddr, reg);
		 	for(i=0;i<BFlen;i++)
		 	{
				BF=1;
			 	BF=BF<<(STBit+i);
			 	BFMsk = BFMsk | BF;
			  	//printf("1: BF=%d, BFMsk=%04x\r\n", BF, BFMsk);
			}
			BFMsk = ~BFMsk;
			//printf("2: BF=%d, BFMsk=%04x\r\n", BF, BFMsk); 
			value = (RValue & BFMsk) | (value<<STBit);
			//printf("* Phyaddr=%d, RegAddr=0x%02lX, OrgValue=0x%04lX, Modified value=0x%04lX\r\n", phyaddr, reg,  RValue, value);

			printf("* OrgValue=%04lX,",RValue);
			tcMiiStationWrite(phyaddr, reg, value);
			RValue = tcMiiStationRead(phyaddr, reg);
			printf(" ModValue=%04lX\r\n", RValue);
		}
    }
	#endif
    else 
	{	// error message
		printf("Usage: miiw <PhyAddr> <RegAddr> <RegVal>\r\n");
		printf("       miiw <PhyAddr> <PageNo> <RegAddr> <RegVal>\r\n");
		#ifdef LINUX_OS // allen_20130926	
		printf("       miiwb <PhyAddr> <RegAddr> <STBit> <BFLen> <BFVal> \r\n");	
		printf("       miiwb <PhyAddr> <PageNo> <RegAddr> <STBit> <BFLen> <BFVal> \r\n");	
		#endif
        return 0;
    }
    return 0;
}

#ifdef TCPHY_1PORT
static int doPhySpeed (int argc, char *argv[], void *p)
{
    uint8 port_num = 0;

    if(argc != 2 ) 
    {
        printf("Usage: Speed <Auto|AN|100F|100H|10F|10H|Disp>\r\n");
    }   
    else 
    {
        if(stricmp(argv[1], "Auto") == 0 ) 
        {
            tcphy_speed = tcphy_speed_Auto;
            tcPhyWriteReg(port_num,0,0x1200);
        }
        else if(stricmp(argv[1], "AN") == 0 )
        {
            tcphy_speed = tcphy_speed_ForceAN;
            tcPhyWriteReg(port_num,0,0x1200);
        }
        else if(stricmp(argv[1], "100F") == 0 ) 
        {
            tcphy_speed = tcphy_speed_Force100F;
            tcPhyWriteReg(port_num,0,0x2100);
        }
        else if(stricmp(argv[1], "100H") == 0 )
        {
            tcphy_speed = tcphy_speed_Force100H;
            tcPhyWriteReg(port_num,0,0x2000);
        }
        else if(stricmp(argv[1], "10F") == 0 ) 
        {
            tcphy_speed = tcphy_speed_Force10F;
            tcPhyWriteReg(port_num,0,0x0100);
        }
        else if(stricmp(argv[1], "10H") == 0 )
        {
            tcphy_speed = tcphy_speed_Force10H;
            tcPhyWriteReg(port_num,0,0x0000);
        }
        else 
        {
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

int doPhyGphyTestMode (int argc, char *argv[], void *p)
{
	uint16 phyaddr = 0, port = 0;
	uint32 reg3 = 0, r15_temp = 0;

	if(argc!=3) 
	{
	   	printf("Usage: testmode <phyaddr> <1|2|3|4>\r\n");
	}
	else	
	{
    	phyaddr = checked_atoi(argv[1]);
    	port = phyaddr - 8;
    	
		reg3 = mtPhyReadReg(port, 3);
		r15_temp = tcMiiStationRead(phyaddr, 15);
		if((reg3 == EPHY_ID_7512Ge)||((reg3 == EPHY_ID_7530Ge_7512Fe)&&(((r15_temp>>13)&0x1) == 1)))
		{
			if(stricmp(argv[2], "0") == 0 )
			{	//default
				tcPhyWriteReg(port, 9, 0x600);
				printf("Usage: default\r\n");
			}
			else if(stricmp(argv[2], "1") == 0 )
			{	//Gphy test mode1
				tcPhyWriteReg(port, 9, 0x2700);
				printf("Usage:port[%d] into testmode1\r\n",phyaddr);
			}
			else if(stricmp(argv[2], "2") == 0 )
			{	//Gphy test mode2
				tcPhyWriteReg(port, 9, 0x4700);
				printf("Usage:port[%d] into testmode2\r\n",phyaddr);
			}
			else if(stricmp(argv[2], "3") == 0 )
			{	//Gphy test mode3
				tcPhyWriteReg(port, 9, 0x6700);
				printf("Usage:port[%d] into testmode3\r\n",phyaddr);
			}
			else if(stricmp(argv[2], "4") == 0 )
			{	//Gphy test mode4
				tcPhyWriteReg(port, 9, 0x8700);
				printf("Usage:port[%d] into testmode4\r\n",phyaddr);
			}
		}
		else
		{
			printf("Usage:port[%d] No support Giga\r\n", phyaddr);
		}
	}
}

static int doPhyConfig (int argc, char *argv[], void *p)
{
	#ifdef TCPHY_DEBUG
    if(argc==1) 
    {
        TCPHYDISP3("Usage: config SwPatch [on|off]\r\n");
        TCPHYDISP3("       config DispLevel [0..6]\r\n");
        TCPHYDISP3("       config ErrOverMonitor [on|off]\r\n");
    }
    else if(argc==2) 
    {
        if(stricmp(argv[1], "list") == 0)
        {
            TCPHYDISP3("config SwPatch : %s\r\n",
                    (sw_patch_flag?"on":"off"));            
            TCPHYDISP3("config DispLevel : %d\r\n",
                    (tcPhy_disp_level));            
            TCPHYDISP3("config ErrOverMonitor : %s\r\n",
                    (sw_ErrOverMonitor_flag?"on":"off"));
        }
    }
	#endif

    if(argc==2 || argc==3)
    {
        if(stricmp(argv[1], "SwPatch") == 0)
        {
            if(stricmp(argv[2], "on") == 0 )
            {
                sw_patch_flag = 1; // enable
            }
            else if(stricmp(argv[2], "off") == 0 )
            {
                sw_patch_flag = 0; // disable
            }
            else 
            {
                TCPHYDISP2("config SwPatch : %s\r\n",
                        (sw_patch_flag?"on":"off"));
            }
            return 0; // Not call individual config function
        }
		#ifdef TCPHY_DEBUG
        else if(stricmp(argv[1], "DispLevel") == 0)
        {
            if(argc == 3) 
            {
                tcPhy_disp_level = atoi(argv[2]);
            }
            TCPHYDISP2("config DispLevel : %d\r\n",tcPhy_disp_level);           
            return 0; // Not call individual config function
        }
        else if(stricmp(argv[1], "ErrOverMonitor") == 0)
        {
        	if(argc == 3)
        	{
	            if(stricmp(argv[2], "on") == 0 )
    	        {
        	        sw_ErrOverMonitor_flag = 1; // enable
            	}
            	else if(stricmp(argv[2], "off") == 0 )
            	{
                	sw_ErrOverMonitor_flag = 0; // disable
            	}
            	else 
            	{
                	TCPHYDISP2("config ErrOverMonitor : %s\r\n",
                        (sw_ErrOverMonitor_flag?"on":"off"));
            	}
            }	
            TCPHYDISP2("config ErrOverMonitor : %s\r\n", (sw_ErrOverMonitor_flag? "on":"off")); 
            return 0; // Not call individual config function
        }
		#endif
    }

    switch(tcPhyVer) 
    { 
		case tcPhyVer_7512Fe: // mt7512 FE
			mt7512FEConfig(argc, argv, p);
			break;
    }
    return 0;
}
#endif // TCPHY_SUPPORT

/************************************************************************
*                       API functions body for EPHY
**************************************************************************/
#if defined(TCPHY_DEBUG) || defined(MTPHY_DEBUG)
static int doPhyInit(int argc, char *argv[], void *p)
{
 //   tcPhyInit(ephy_addr_base);

    tcPhyInit();

    return 0;
}
#endif

#if defined(TCPHY_DEBUG)|| defined(MTPHY_DEBUG)
static int doPhySwPatch (int argc, char *argv[], void *p)
{
	uint8 pn;
	
    if(argc != 2 ) 
	{
        printf("Sw patch status: %s.\r\n", (sw_patch_flag? "on": "off"));
    }
    else if(stricmp(argv[1], "on") == 0 )  
	{
        sw_patch_flag = 1;      
        printf("Sw patch status: ON.\r\n");
    }
    else if(stricmp(argv[1], "off") == 0 ) 
	{
        sw_patch_flag = 0;      
        printf("Sw patch status: OFF.\r\n");
    }           
	else if(stricmp(argv[1], "dbg") == 0) // allen_20140822
	{
		printf("\n tcPhyVer = %d \n", tcPhyVer);
		for(pn=0; pn<4; pn++)
		{
			if(tcPhyVer == tcPhyVer_7512Ge)
				printf("\n mtphy_state=%d, DisSlave_flag[%d]=%d, DisSlave_cnt=%d \n", mtphy_link_state[pn], pn, mt7512GePhyDisSlaveMode_flag[pn], mt7512GePhyDisSlaveMode_cnt[pn]);
			else if(tcPhyVer == mtPhyVer_7530)
				printf("\n mtphy_state=%d, DisSlave_flag[%d]=%d, DisSlave_cnt=%d \n", mtphy_link_state[pn], pn, mt7530GePhyDisSlaveMode_flag[pn], mt7530GePhyDisSlaveMode_cnt[pn]);
		}
		pn = 4;
		printf("mtPhyReg case 1 :Nmr1[%d].value =0x%04lx\r\n", pn, Nmr1[pn].value);
		printf("mtPhyReg case 1 :Nmr1[%d].link_status_prev =0x%04lx\r\n", pn, Nmr1[pn].link_status_prev );
		printf("mtPhyReg case 1 :Nmr1[%d].link_status =0x%04lx\r\n", pn, Nmr1[pn].link_status);
	}	
    else 
	{
        printf("Sw patch status: %s.\r\n", (sw_patch_flag? "on": "off"));
    }
    return 0;
}
#endif

#if defined(TCPHY_DEBUG)
static int doPhyRegCheck (int argc, char *argv[], void *p)
{
    uint32 val, i;
    uint32 phyaddr;
    uint32 reg;
    uint32 loop;
    uint32 val_inc;
    volatile uint32 data;

    if((argc != 5) && (argc != 6) )
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

    for(i=0; i<loop; i++ )
    {
        if(argc==6)
        {
            val = (val+val_inc)&0xffff;
            tcMiiStationWrite(phyaddr,reg,val);
        }

        data = tcMiiStationRead(phyaddr, reg);
        if(data != val )
        {
            printf("\r\nError!! Phyaddr=%ld reg=%ld loop=%ld Expected=0x%lx Read=0x%lx\r\n", 
                phyaddr, reg, i, val, data);
            return 0;
        }
        
        if((i%10000) == 0)
        {
            printf(".");
        }
    }
    printf("\r\nLoop=%ld Done!!\r\n", i);
    return 0;
}

// called by macRxRingProc() for doPhyLoopback()
#ifdef LINUX_OS
uint8 phyMbpChk(struct sk_buff* oldMbp)
{
#else
uint8 phyMbpChk(mbuf_t*oldMbp)
{
#endif
    if(phy_loopback_flag)
    {
        recv_ok_flag = 1;
        return 1;
    }
    
    if((oldMbp->data[12] == 8) && (oldMbp->data[13] == 0) )
    {	// ip packet
        if(oldMbp->data[34] == 0 && oldMbp->data[23] == 1)//Echo Ping Reply
        {
            if( oldMbp->data[50] == 'B' && oldMbp->data[51] == 'E' && 
                 oldMbp->data[52] == 'A' && oldMbp->data[53] == 'F' )
            {
                PingReplyCnt++;
                return 1;
            }
        }
    }   
    return 0;
}

// called by macRxRingProc() for doPhyLoopback()
#ifdef LINUX_OS
uint8 phy_recv_err_check(struct sk_buff** oldMbp, uint32 *frameSize)
{
	#ifdef TC2031_DEBUG
 	#if  !(defined(CONFIG_MIPS_TC3182) ||  defined(CONFIG_MIPS_TC3262))
	int i = 0;
	if(phy_loopback_flag)
	{
	#if 0  /*jason.gong allen*/
		if(rxDescrp->rdes0.word & (1<<30))
		{
			*frameSize = rxDescrp->rdes0.bits.rfl;
			*frameSize = (*frameSize > 60) ? *frameSize : 60;
		}
		else
		{
			*frameSize = rxDescrp->rdes0.bits.rfl;
		}
		*oldMbp = (struct sk_buff *)(rxDescrp->skb);
		skb_put(*oldMbp, *frameSize);
		dbg_plinel_1("Received packet have CRC error! length=", *frameSize);
		dbg_pline_1("\r\n");
		for(i=0; i<*frameSize; i++ )
			dbg_plineb_1(" ", (*oldMbp)->data[i]);
		dbg_pline_1("\r\n");
		recv_err_flag = 1;	
		#endif
	}
	#endif
	#endif
	return 0;
}
#else
uint8 phy_recv_err_check(mbuf_t **oldMbp, macRxDescr_t *rxDescrp, uint32 *frameSize)
{
    #ifdef TC2031_DEBUG
    int i = 0;
    if(phy_loopback_flag)
    {
    	if(rxDescrp->rdes0.word & (1<<30))
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

#ifdef LINUX_OS
/*===============================================================================
**      function name: delay1ms
**      descriptions: delay 1 ms 
=================================================================================*/
void delay1ms(int ms)
{
	volatile uint32 timer_now, timer_last;
	volatile uint32 tick_acc;
	uint32 one_tick_unit = 1 * SYS_HCLK * 1000 / 2;
	volatile uint32 tick_wait = ms * one_tick_unit;
	volatile uint32 timer1_ldv = VPint(CR_TIMER1_LDV);

	tick_acc = 0;
 	timer_last = VPint(CR_TIMER1_VLR);
	do 
	{
   		timer_now = VPint(CR_TIMER1_VLR);
       	if(timer_last >= timer_now)
       		tick_acc += timer_last - timer_now;
      	else
       		tick_acc += timer1_ldv - timer_now + timer_last;
     	timer_last = timer_now;
	}while(tick_acc < tick_wait);
}
/*======================================================================================
**      function name: reverseLong
**      descriptions: Reverse the bytes ordering of the input value.
**      parameters:
**            ul: Specify the 4 bytes value that you want to reverse the ordering.
**      revision: Here 2008/08/24
========================================================================================*/
static uint32 reverseLong(uint32 ul)
{
	uint8 z[4];
	z[3] = *((uint8 *)&ul + 0);
	z[2] = *((uint8 *)&ul + 1);
	z[1] = *((uint8 *)&ul + 2);
	z[0] = *((uint8 *)&ul + 3);
	return *((uint32 *)z);
}/*end reverseLong*/
/*======================================================================================
**      function name: scramble
**      descriptions: Scramble the input 32bits value.            
**      return:  After Scramble the value
**      call: reverseLong
**      revision: Here 2008/08/24
=========================================================================================*/
static uint32 scramble(uint32 checkCode)
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
/*==============================================================================
**      function name: rand
**      descriptions: Random value generation. 
**      return: Random value
**      call: timerVlrGet
**      revision: Here 2008/08/24
================================================================================*/
static uint32 rand(void)
{
	uint32 vlr;
	timerVlrGet(1, vlr);
	scramble(vlr);
	return (vlr & RAND_MAX);
}
/*================================================================================
**      function name: doMacSend
**      descriptions: send packet  with the  data pattern we specific
**      parameters:
**            argc : argument number
**            argv : argument point 
**		 		p  : no use
**      return:
**            Success:        0
**            Otherwise:     -1
=================================================================================*/
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
	if(argv[4] != NULL)
	{
		delay_cnt  = simple_strtoul(argv[4],NULL,10);
	}
	pattern = (uint8) pattern32;
	dbg_plineb_1("\n\rFill pattern  =",pattern);
	dbg_plinel_1("\n\rPacket length =",len);
	dbg_plinel_1("\n\rLoop cnt      =",loop);
	dbg_plinel_1("\n\rdelay_cnt      =",delay_cnt);
	j = 0;
	while (j < loop) 
	{
		mbp = (struct sk_buff *)skbmgr_dev_alloc_skb2k();
		if(mbp == NULL)
		{
			continue;
		}			
		skb_reserve(mbp, 2);
		skb_put(mbp, len);
		for(i = 0; i < len; i++)
		{	/* fill pattern */
			mbp->data[i] = pattern;
		}
		macSend(0, mbp);
			j++;

		
		if( delay_cnt != 0 )
		{
			if( (j%delay_cnt)==0)
				delay1ms(1);
		}
	}
	dbg_pline_1("\n\rDone!\n\r");
	return 0;
}
/*===============================================================
**      function name: doMacSendRandom
**      descriptions: send packet with random dataa pattern
**      parameters:
**            argc : argument number
**            argv : argument point 
**		 		p  : no use      
**      return:
**            Success:        0
**            Otherwise:     -1
=================================================================*/
static int doMacSendRandom(int argc, char *argv[], void *p)
{
	uint32 len=0;
	uint32 loop=0;
	struct sk_buff *mbp = NULL;
	uint32 i, j;
	
	if(argc == 2)
	{
		loop = simple_strtoul(argv[1],NULL,10);
		dbg_plinel_1("\n\rLoop cnt      =",loop);
	} 
	else if(argc==3)
	{
		len = simple_strtoul(argv[1],NULL,10);
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
	while(j < loop) 
	{
		mbp = (struct sk_buff *)skbmgr_dev_alloc_skb2k();
		if(mbp == NULL)
		{
			continue;
		}
		/* da */  // broadcast
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
		if(argc == 2)		
			len = rand();
		if(len < 60)
			len = 60;
		if(len > 1514)
			len = 1514;
		for(i = 14; i < len; i++)
		{	/* fill pattern */
			mbp->data[i] = (uint8) (rand() + rand());
		}
		skb_reserve(mbp, 2);
		skb_put(mbp, len);
		
		macSend(0, mbp);
			j++;
		
		// print "." every 1024 packets
		if((j % 1024) == 0) 
		{
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
static int pingExec(cbuf_t  *cbp,           /* cbuf pointer         */
    uint16  signature,      /* event function signature */
    void    *data_p,        /* data pointer         */
    uint16  data_size       /* data size            */)
{
    pingEvt_t *ping_evt_p = (pingEvt_t *)data_p;
    pingtx( ping_evt_p->ping_p );
    return 0;
} /* pintExec */
// called by doPing()
static int pingKeyProc(int c)
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
static int doPing(int argc, char *argv[], void *p)
{       
#if 0
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
    if((ping_p=calloc(1, sizeof(ping_t))) == NULL)
        sysreset();
    if((ip_sock = socket(AF_INET,SOCK_RAW,ICMP_PTCL)) == -1)
    {
        printf("Can't create socket\n");
        retvalue = 1;
        goto ping_exit1;
    }
    ping_p->s = ip_sock;

    printf("Resolving %s... ",argv[1]);
    dnsQueryResult = resolveName(argv[1]);
    if(!dnsQueryResult)
    {
        printf("unknown\n");
        retvalue = 1;
        goto ping_exit;
    }
    if(resolve( inet_ntoa(dnsQueryResult), &ping_p->target, NULL) != 0)
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
    TRCLOG3216( LOG_IP, 5, "doping %lx %x %x", ping_p->target,ping_p->len,ping_p->interval);

    /* user has specified interval */
    pingCmdCb.flags.active = 1;
    pingCmdCb.actSocket = ip_sock;
    if(olStart( NULL, OL_SIG_PING ) != 0)
    {
        goto ping_exit;
    }
    ping_p->flag.from = 1;
    ping_evt.ping_p = ping_p;
    olDataCmdSend(NULL, EVT_SYS_OL_PING, pingExec, OL_SIG_PING, &ping_evt, sizeof(pingEvt_t));
    /* CTLC is the delimeter */
    ControlStdio(0,0,CTLC,pingKeyProc); 
    retvalue = 1;
    pause(1000);
    ResetStdio();
    olStop(NULL, OL_SIG_PING);
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
#endif
	return 0;
}

static int doPingEchoCnt(int argc, char *argv[], void *p)
{
    if(!strcmp(argv[1], "clear"))
        PingReplyCnt = 0;
    printf("Ping Reply Count: %ld\r\n", PingReplyCnt);
    return 0;
}
#endif

// called by macPeriodCheck() in tc3162l2mac.c
void tcPhyChkVal(void)
{ 	// for 7512 (linux)
	uint32 port_num;

	if((tcPhyVer == tcPhyVer_7512Fe)||(tcPhyVer == tcPhyVer_7512Ge)||(tcPhyVer == tcPhyVer_mt7505FE))
	{
		ephy_addr_base = 8;
	}
	port_num = phychkval_portnum - ephy_addr_base;  // 0~4

   	if(phychkval_flag)
	{
	    uint32 	val;
		uint32 	phyAddr = phychkval_portnum;
        uint32 	mr02,mr03;
        uint8 	mr_anen, mr_dplx, mr_speed;
        uint8 	mr_an_capable;
        uint8 	mr_lp_an_capable;
        uint8 	mr_lp_anen;
		uint8 	mr01_link;	
        uint32 	reg3, r15_temp;
        uint8 	lr_linkup, lr_speed, lr_dplx, lr_mdix;
		// show phy_id
	
		tcMiiStationWrite(phyAddr, 31, 0);   	// g0r31 = 0x0
		val = tcMiiStationRead(phyAddr, 2);
		mr02 = val&(0xffff);
		val = tcMiiStationRead(phyAddr, 3); 
		mr03 = val&(0xffff);
		reg3 = tcMiiStationRead(phyAddr, 3); 
		printf(" tcPhy ID: %lx %lx\r\n", mr02, mr03); 
	
		val = tcMiiStationRead(phyAddr, 0);
		mr_anen = (val>>12)&0x01;
		mr_dplx = (val>>8)&0x01;
		mr_speed = ((val>>13)|(val>>5))&0x03;	//00:10,01:100,10:1000
		
		// reg4h
		val = tcMiiStationRead(phyAddr, 0x04);
		mr_an_capable = (val>>5)&0x0F; 			//100F,100H,10F,10H

		// reg9h
		val = tcMiiStationRead(phyAddr, 0x09);
		mr_an_capable = mr_an_capable | ((val>>4)&0x30);	//1000F,1000H

		// reg5h
		val = tcMiiStationRead(phyAddr, 0x05);
		mr_lp_an_capable = (val>>5)&0x0F; 					//100F,100H,10F,10H
		
		// regAh
		val = tcMiiStationRead(phyAddr, 0x0A);
		mr_lp_an_capable = mr_lp_an_capable | ((val>>6)&0x30); 	//1000F,1000H

		// reg6h
		val = tcMiiStationRead(phyAddr, 0x06);
		mr_lp_anen = (val)&0x01;
   
		// reg1h 	// allen_20130926 from biker
		val = tcMiiStationRead(phyAddr, 0x01);
		mr01_link = (val>>2)&0x01;
		//printf(" [1] reg1 = 0x%x, mr01_link = %d \n", val, mr01_link);
		
		val = tcMiiStationRead(phyAddr, 0x01);
		mr01_link = (val>>2)&0x01;
		//printf(" [2] reg1 = 0x%x, mr01_link = %d \n", val, mr01_link);
	
		val = tcMiiStationRead(phyAddr, 0x01);
		mr01_link = (val>>2)&0x01;
		printf(" [3] reg1 = 0x%x, mr01_link = %d, tcPhyVer = 0x%x \r\n", val, mr01_link, tcPhyVer);
    	//printf(" [3] phychkval_portnum = %d, phyAddr = %d, port_num = %d \r\n", phychkval_portnum, phyAddr, port_num);

		r15_temp = tcMiiStationRead(phyAddr, 0xf);
		if((reg3==EPHY_ID_7512Ge) || ((reg3 == EPHY_ID_7530Ge_7512Fe)&&(((r15_temp>>13)&0x1) == 1)))
		{	//MT7530 ,MT7510Ge 
			#ifdef LINUX_OS // allen_20130926
			val = mtPhyReadGReg(phyAddr,0x1E,0xA2);
    		#endif
    		//printf(" tcPhyChkVal= 0x%04lx",val);
  
			lr_linkup = (val>>9)&0x07;		//dev1e_A2[9]:LinkStatus10_OK,dev1e_A2[10]:LinkStatus100_OK,dev1e_A2[11]:LinkStatus1000_OK
			lr_speed = (val>>1)&0x07; 		//dev1e_A2[1]:final_speed_10,,dev1e_A2[2]:dev1e_A2[2]:final_speed_100,dev1e_A2[3]:final_speed_1000
			lr_dplx = (val>>5)&0x01;
			lr_mdix = (val>>6)&0x01;
		}    
		else
		{ 
			// l0reg28
			val = tcPhyReadLReg(port_num,0, 28);
			//printf(" tcPhyChkVal= 0x%04lx",val);
			
			lr_linkup = (val)&0x01;
			lr_speed = (val>>1)&0x01;
			lr_dplx = (val>>2)&0x01;
			lr_mdix = (val>>5)&0x01;
		}
			
		printf(" TcPhy mode:");	
		if(mr_anen)
		{ 	// Auto-neg
	    	printf(" AN-(");
	    	printf(" %s,",((mr_an_capable>>5)&0x01)?"1000F":"");
	    	printf(" %s,",((mr_an_capable>>4)&0x01)?"1000H":"");
	    	printf(" %s,",((mr_an_capable>>3)&0x01)?"100F":"");
	    	printf(" %s,",((mr_an_capable>>2)&0x01)?"100H":"");     
	    	printf(" %s,",((mr_an_capable>>1)&0x01)?"10F":"");      
	    	printf(" %s)\r\n",((mr_an_capable>>0)&0x01)?"10H":"");  
		}
		else if((reg3==EPHY_ID_7512Ge) || (reg3 == EPHY_ID_7530Ge_7512Fe))
		{ 	// Force-speed
			if(mr_speed==0x00)
	 		{
		 		printf(" Force-%d%s\r\n",10,(mr_dplx?"F":"H"));
			}
			else if(mr_speed==0x01)
			{
				printf(" Force-%d%s\r\n",100,(mr_dplx?"F":"H"));
	 		}
			else if(mr_speed==0x02)
			{
				printf(" Force-%d%s\r\n",1000,(mr_dplx?"F":"H"));
	 		}
    	}
	    else 
	 	{
	 		printf(" Force-%d%s\r\n",(mr_speed?100:10),(mr_dplx?"F":"H")); 	
        }
		 
		if(mr01_link == 0)  // link down
		{ 	// allen_20130926
			printf(" *** Link is down!\r\n");

			if((tcPhyVer==tcPhyVer_7512Fe)||(tcPhyVer==tcPhyVer_7512Ge))
			{
				printf(" tcphy[%ld]: adc_avg=%ld/1000\r\n", phyAddr, mt7512FEReadAdcSum(port_num));            
			}

            // dev1E_RA2 message
            if(mr1E_A2.lch_SignalDetect || mr1E_A2.lch_LinkPulse
                || mr1E_A2.final_speed_1000 || mr1E_A2.final_speed_100|| mr1E_A2.final_speed_10)
            {
				TCPHYDISP4(" tcphy[%ld]: ",phyAddr);

				if(mr1E_A2.lch_SignalDetect) 
					TCPHYDISP4("SigDet ");
				if(mr1E_A2.lch_LinkPulse)
					TCPHYDISP4("RxLkp ");
				if(mr1E_A2.final_speed_1000)
				{					
					if(mr1E_A2.MSConfig1000)
					{
						TCPHYDISP4("Up1000 Master ");
					}
					else
					{
						TCPHYDISP4("Up1000 Slave ");
					}
				}

				if(mr1E_A2.final_speed_100)
					TCPHYDISP4("Up100 ");
				if(mr1E_A2.final_speed_10)
					TCPHYDISP4("Up10 ");
				if(mr1E_A2.da_mdix)
				{
					TCPHYDISP4("UpMdix ");
				}
				else
				{
					TCPHYDISP4("UpMdi ");
				}
				TCPHYDISP4("\r\n");
            }

			if((tcPhyVer==tcPhyVer_7512Fe))
			{
				mt7512FEDispProbe100(port_num,1);
			}
        }
		else 
		{ 	// link-up
			if((reg3==EPHY_ID_7512Ge) || ((reg3 == EPHY_ID_7530Ge_7512Fe)&&(((r15_temp>>13)&0x1) == 1))) // 7512GE, 7530GE
			{
				if(lr_speed == 0x04)
				{
					printf(" TCphy is link-up at 1000 %s ",(mr1E_A2.MSConfig1000?"Master":"Slave"));
				}
				else if(lr_speed == 0x02)
				{
					printf(" TCphy is link-up at 100 %s ",(lr_dplx?"F":"H"));
				}
				else if(lr_speed == 0x01)
				{	
					printf(" TCphy is link-up at 10 %s ",(lr_dplx?"F":"H"));
				}
	      	}
			else
			{
				printf(" TCphy is link-up at %d%s ",(lr_speed?100:10),(lr_dplx?"F":"H"));	
			}
           
	   		if(mr_lp_anen)
			{
                printf(", Link-partner supports AN-(");
                printf(" %s,",((mr_lp_an_capable>>5)&0x01)?"1000F":"");
                printf(" %s,",((mr_lp_an_capable>>4)&0x01)?"1000H":"");
                printf(" %s,",((mr_lp_an_capable>>3)&0x01)?"100F":"");
                printf(" %s,",((mr_lp_an_capable>>2)&0x01)?"100H":"");  
                printf(" %s,",((mr_lp_an_capable>>1)&0x01)?"10F":"");       
                printf(" %s),  ",((mr_lp_an_capable>>0)&0x01)?"10H":"");   
            }
	    	else 
			{
                printf(" Link-partner operates in Force mode.\r\n");
            }

            printf(" %s,",(lr_mdix?"mdix":"mdi"));
            if(tcPhyVer == tcPhyVer_7512Fe)
            {
                printf(" tx_amp_save=%d\r\n", (mr28.tx_amp_save));
            }           
            
			if(lr_speed >= 0x02)
			{	// 100Mbps/1000Mbps

				//printf(" tcPhyVer = 0x%x \r\n", tcPhyVer);
				
				//if(tcPhyVer == mtPhyVer_7530)
				if((reg3 == EPHY_ID_7530Ge_7512Fe)&&(((r15_temp>>13)&0x1) == 1))
				{
					//mt7530GePhyReadProbe(port_num,Probe_VgaState,0);
					//mt7530GePhyReadProbe(port_num,Probe_MSE_ALL,0);
					//mt7530GePhyReadProbe(port_num,Probe_MSE_Slicer_err_thres,0);
		    		//   mt7530GePhyReadProbe(port_num,Probe_Slicer_Err_Over_Sum_ALL,0);
					val=mt7530GePhyReadProbe(port_num, Probe_VgaState, 0);
					printf("VgaStateA, B, C, D = 0x%x, 0x%x, 0x%x, 0x%x\r\n",((val>>4) & 0x1F),((val>>9) & 0x1F),((val>>14) & 0x1F),((val>>19) & 0x1F));
					//printf("VgaStateB =0x%x,\r\n",((val>>9) & 0x1F));
					//printf("VgaStateC =0x%x,\r\n",((val>>14) & 0x1F));
					//printf("VgaStateD =0x%x\r\n",((val>>19) & 0x1F));
					
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
				}
				//else if(tcPhyVer == tcPhyVer_7512Ge)
				else if(reg3==EPHY_ID_7512Ge)
				{
					val=mt7512GePhyReadProbe(phyAddr, Probe_VgaState, 0);
					printf("7512 VgaStateA, B, C, D = 0x%x, 0x%x, 0x%x, 0x%x\r\n",((val>>4) & 0x1F),((val>>9) & 0x1F),((val>>14) & 0x1F),((val>>19) & 0x1F));
				
					val=mt7512GePhyReadProbe(phyAddr,Probe_MSE,0);
					printf("7512 lch_mse_mdcA =0x%x\r\n",val);
					val=mt7512GePhyReadProbe(phyAddr,Probe_MSE,1);
					printf("7512 lch_mse_mdcB =0x%x\r\n",val);
					val=mt7512GePhyReadProbe(phyAddr,Probe_MSE,2);
					printf("7512 lch_mse_mdcC =0x%x\r\n",val);
					val=mt7512GePhyReadProbe(phyAddr,Probe_MSE,3);
					printf("7512 lch_mse_mdcD =0x%x\r\n",val);
			
					val=mt7512GePhyReadProbe(phyAddr,Probe_MSE_Slicer_err_thres,0);
					printf("slicer_err_thres =0x%x\r\n",val);
					val=mt7512GePhyReadProbe(phyAddr,Probe_Slicer_Err_Over_Sum,0);
					printf("ProbeSlicerErrOverSumA =0x%x\r\n",val);
					val=mt7512GePhyReadProbe(phyAddr,Probe_Slicer_Err_Over_Sum,1);
					printf("ProbeSlicerErrOverSumB =0x%x\r\n",val);
					val=mt7512GePhyReadProbe(phyAddr,Probe_Slicer_Err_Over_Sum,2);
					printf("ProbeSlicerErrOverSumC =0x%x\r\n",val);
					val=mt7512GePhyReadProbe(phyAddr,Probe_Slicer_Err_Over_Sum,3);
					printf("ProbeSlicerErrOverSumD =0x%x\r\n",val);
				}
			}

			//if((reg3!=0x9421) && (reg3!=EPHY_ID_7530Ge_7512Fe))  // not GPHY
			if(((r15_temp>>13)&0x1) == 0)	// not GEPHY
			{
				if (lr_speed)
				{
					if((tcPhyVer==tcPhyVer_7512Fe)||(tcPhyVer==tcPhyVer_7512Ge))
					{
						mt7512FEDispProbe100(port_num,1);
					}
				}
			}
		}   
		phychkval_flag = 0;
	}
}

static int doPhyForceLink(int argc, char *argv[], void *p)
{
    if(!tcPhyFlag)
        return 0;

    if(argc != 2) 
    {
        printf("forcelink status: %s.\r\n", (force_link_flag?"on":"off"));
    }
    else if(stricmp(argv[1], "on") == 0)  
    {
        force_link_flag = 1;        
        printf("Force link status: ON.\r\n");
    }
    else if(stricmp(argv[1], "off") == 0) 
    {
        force_link_flag = 0;        
        printf("Force link status: OFF.\r\n");
    }           
    else 
    {
        printf("Usage: forcelink status [on|off] \r\n");
    }
    return 0;
}


void EphyMonitor(void)
{
    /* add your monitor code here*/
    tcPhyChkVal();
    tcPhySwPatch();
    //printf(" ephy monitor \r\n");
}

// set flag for tcPhyErrMonitor
static int doErrMonitor(int argc, char *argv[], void *p)
{
    if(!tcPhyFlag)
        return 0;

    if(argc != 2) 
    {
        printf("Error monitor status: %s!\r\n", (err_monitor_flag?"on":"off"));
    }
    else if(stricmp(argv[1], "on") == 0)  
    {
        err_monitor_flag = 1;       
        printf("Error monitor ON!\r\n");
    }
    else if(stricmp(argv[1], "off") == 0) 
    {
        err_monitor_flag = 0;       
        printf("Error monitor OFF!\r\n");
    }           
    else 
    {
        printf("Usage: errmonitor [on|off] \r\n");
    }
    return 0;
}

int tcPhyErrMonitor(void)
{
    uint32 	val;
    uint16 	curr_runt_cnt = 0;
    uint16 	curr_tlcc_cnt = 0;
    uint16 	curr_crc_cnt = 0;
    uint16 	curr_long_cnt = 0;
    uint16 	curr_loss_cnt = 0;
    uint16 	curr_col_cnt = 0;
	/*
	#ifdef PHYPART_DEBUG
		printf("PhyPart debug: tcPhyErrMonitor() in \r\n");
	#endif
	*/
	/*
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
		//dbg_plinel_1("\r\n ", mac_p->macStat_p->MIB_II.inOctets);
        timeout_cnt++;
    }
	/*
	#ifdef PHYPART_DEBUG
	printf("PhyPart debug: tcPhyErrMonitor() out \r\n");
	#endif
	*/
    return 0;
}
#endif // TCPHY_DEBUG

/************************************************************************
*            debug functions body for EPHY (TCPHY_TESTDBG CI commands)
**************************************************************************/
#ifdef TCPHY_FIELD_DBGTEST
static int doFieldDebugTest(int argc, char *argv[], void *p)
{
    if(!tcPhyFlag) 
    {
        printf("NOT TC PHY!!\r\n");
        return 0;
    }
    if(argc==2 || argc==3) 
    {	// yhchen: need to re-write to support both 2031 & 2101mb !!
        if(tcPhyVer == tcPhyVer_2031) 
        {
            #ifdef TC2031_DBGTEST
            doPhyDebugTest(argc, argv, p);
            #endif
        }
        else 
        {
            printf("Not support!!\r\n");
        }
        return 0;
    }
    printf("Usage: dbgtest <1|2|3> [wait_time]\r\n");
    return 0;
}

static int doFieldDebugPrint(int argc, char *argv[], void *p)
{
    uint32 	i = 0;
    for(i=0; i<msgcnt; i++) 
    {
        printf("%3d: %s \n", i, tc_ether_phy[i]);
    }
    return 0;
}
#endif // TCPHY_FIELD_DBGTEST
#ifdef TCSUPPORT_DYNAMIC_VLAN
extern dyVLAN_PORT_t dyVLAN_PORT[];
void tcPortLinkChk(void)
{
	int port;
	for(port = 0; port < TCPHY_PORTNUM; port++)
	{
		if(tcphy_link_state[port] == ST_LINK_DOWN2UP)
		{
			printk("Port %d link up\n", port);
		}
		else if(tcphy_link_state[port] == ST_LINK_UP2DOWN)
		{
			printk("Port %d link down\n", port);
			dyVLAN_PORT[port].state = 0;
		}
	}
}
#endif

#ifdef LINUX_OS // allen_20130926
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
	
   	if(argc==4)
	{
		PortSel = checked_atoi(argv[2]);
		sscanf(argv[3], "%lx", &DbgflgSel);
		//DbgClk = checked_atoi(argv[3]);
		printf("* dbg_port_Sel=%d, dbg_flag_sel=0x%04lX\r\n", PortSel, DbgflgSel);		
	}
   	else if(argc==5)
   	{
		PortSel = checked_atoi(argv[2]);
		sscanf(argv[3], "%lx", &DbgflgSel);
		//DbgClk = checked_atoi(argv[3]);
		sscanf(argv[4], "%lx", &DbgClk);
		printf("* dbg_port_Sel=%d, dbg_flag_sel=0x%04lX, dbg_clk_sel=0x%02lX\r\n", PortSel, DbgflgSel, DbgClk);		
	}

	if(((argc == 4) ||(argc == 5)) && (PortSel <= 31))
	{
		if(argc == 4)
		{
			if(stricmp(argv[1], "all") == 0)
			{
	           value = (PortSel<<13)|(dbg_flg_en<<12)|(DbgflgSel);
			   printf("Dbg Flag Register Set Value=%lx\r\n", value);
			   mtEMiiRegWrite(PortSel, dbg_DevAddr, dbg_Flg0Ctrl, value);
			   mtEMiiRegWrite(PortSel, dbg_DevAddr, dbg_Flg1Ctrl, value);
			}
			else if(stricmp(argv[1], "0") == 0)
			{
				value = (PortSel<<13)|(dbg_flg_en<<12)|(DbgflgSel);
				printf("Dbg Flag Register Set Value=%lx\r\n", value);
				mtEMiiRegWrite(PortSel, dbg_DevAddr, dbg_Flg0Ctrl, value);
			}
			else if(stricmp(argv[1], "1") == 0)
			{
				value = (PortSel<<13)|(dbg_flg_en<<12)|(DbgflgSel);
				printf("Dbg Flag Register Set Value=%lx\r\n", value);
				mtEMiiRegWrite(PortSel, dbg_DevAddr, dbg_Flg1Ctrl, value);
			}
			DbgClk = mtEMiiRegRead(PortSel, dbg_DevAddr, dbg_ClkAddr);
			value = DbgClk & 0xEF;
			mtEMiiRegWrite(PortSel, dbg_DevAddr, dbg_ClkAddr, value);
		}
		else if((argc == 5) && (stricmp(argv[1], "1") == 0))
		{
	    	value = (PortSel<<13)|(dbg_flg_en<<12)|(DbgflgSel);
			mtEMiiRegWrite(PortSel, dbg_DevAddr, dbg_Flg1Ctrl, value);
			value = (dbg_flg_en<<12)|(DbgClk);
			mtEMiiRegWrite(PortSel, dbg_DevAddr, dbg_ClkAddr, value);    
	 	   	printf("Dbg Flag Register Set Value=%lx\r\n", value);
		}
		else if((argc == 5) && (stricmp(argv[1], "all") == 0))
		{
	    	value = (PortSel<<13)|(dbg_flg_en<<12)|(DbgflgSel);
	    	printf("Dbg Flag Register Set Value=%lx\r\n", value);
    		mtEMiiRegWrite(PortSel, dbg_DevAddr, dbg_Flg0Ctrl, value);
			mtEMiiRegWrite(PortSel, dbg_DevAddr, dbg_Flg1Ctrl, value);
			value = (dbg_flg_en<<12)|(DbgClk);
			mtEMiiRegWrite(PortSel, dbg_DevAddr, dbg_ClkAddr, value);
		}
	}
	else 
	{ 	// error message
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

    if(argc ==4) 
    {	// tce miir_trdgb <PhyAddr> <TrRegType> <DataAddr>
		phyaddr = checked_atoi(argv[1]);	//0~31
		sscanf(argv[3], "%lx", &data_addr);	// 6 bit HEX
        if(strcasecmp(argv[2], "DSPF") == 0)
        {	// DSP Filter Debug Node
            ch_addr = 0x02;
			node_addr = 0x0D;
	    }
		else if(strcasecmp(argv[2], "PMA") == 0)
		{ 	// PMA Debug Node 
			ch_addr = 0x01;
			node_addr = 0x0F;
		}
		else if(strcasecmp(argv[2], "TR") == 0)
		{ 	// Timing Recovery  Debug Node 
			ch_addr = 0x01;
			node_addr = 0x0D;
		}
		else if(strcasecmp(argv[2], "PCS") == 0)
		{ 	// R1000PCS Debug Node 
			ch_addr = 0x02;
			node_addr = 0x0F;
		}
		else if(strcasecmp(argv[2], "FFE_A") == 0)
		{ 	// FFE Debug Node 
			ch_addr = 0x00;
			node_addr = 0x04;
		}
		else if(strcasecmp(argv[2], "FFE_B") == 0)
		{ 	// FFE Debug Node 
			ch_addr = 0x01;
			node_addr = 0x04;
		}
		else if(strcasecmp(argv[2], "FFE_C") == 0)
		{ 	// FFE Debug Node 
			ch_addr = 0x02;
			node_addr = 0x04;
		}
		else if(strcasecmp(argv[2], "FFE_D") == 0)
		{ 	// FFE Debug Node 
			ch_addr = 0x03;
			node_addr = 0x04;
		}
		else if(strcasecmp(argv[2], "FFE_INDEP") == 0)
		{ 	// FFE Debug Node 
			ch_addr = 0x00;
			node_addr = 0x04;
		}
 		else if(strcasecmp(argv[2], "ECNC_A") == 0)
 		{ 	// EC/NC Debug Node 
			ch_addr = 0x00;
			node_addr = (data_addr&0xC0)>>6; //V1.17 for ECNC_0xE0 is per port
			printf("* ECNC_A => Phyaddr=%d,TrRegTyp =%s,node_addr=0x%02lX,data_addr=0x%04lX ,ch_addr=0x%02lX\r\n",phyaddr, argv[2],node_addr,data_addr, ch_addr);
		}
 		else if(strcasecmp(argv[2], "ECNC_B") == 0)
 		{ 	// EC/NC Debug Node 
			ch_addr = 0x01;
			node_addr = (data_addr&0xC0)>>6; //V1.17 for ECNC_0xE0 is per port
			printf("* ECNC_B => Phyaddr=%d,TrRegTyp =%s,node_addr=0x%02lX,data_addr=0x%04lX ,ch_addr=0x%02lX\r\n",phyaddr, argv[2],node_addr,data_addr, ch_addr);
		}
 		else if(strcasecmp(argv[2], "ECNC_C") == 0)
 		{ 	// EC/NC Debug Node 
			ch_addr = 0x02;
			node_addr = (data_addr&0xC0)>>6; //V1.17 for ECNC_0xE0 is per port
			printf("* ECNC_C => Phyaddr=%d,TrRegTyp =%s,node_addr=0x%02lX,data_addr=0x%04lX ,ch_addr=0x%02lX\r\n",phyaddr, argv[2],node_addr,data_addr, ch_addr);
		}
 		else if(strcasecmp(argv[2], "ECNC_D") == 0)
 		{ 	// EC/NC Debug Node 
			ch_addr = 0x03;
			node_addr = (data_addr&0xC0)>>6; //V1.17 for ECNC_0xE0 is per port
			printf("* ECNC_D => Phyaddr=%d,TrRegTyp =%s,node_addr=0x%02lX,data_addr=0x%04lX ,ch_addr=0x%02lX\r\n",phyaddr, argv[2],node_addr,data_addr, ch_addr);
		}
 		else if(strcasecmp(argv[2], "ECNC_INDEP") == 0)
 		{ 	// EC/NC Debug Node 
			ch_addr=0x00;
			node_addr=(data_addr&0xC0)>>6;
			printf("* ECNC_INDEP => Phyaddr=%d,TrRegTyp =%s,node_addr=0x%02lX,data_addr=0x%04lX ,ch_addr=0x%02lX\r\n",phyaddr, argv[2],node_addr,data_addr, ch_addr);
		}
		else if(strcasecmp(argv[2], "DFEDC_A") == 0)
		{ 	// DFETail/DC Debug Node 
			ch_addr=0x00;
			node_addr=0x05;
		}
		else if(strcasecmp(argv[2], "DFEDC_B") == 0)
		{ 	// DFETail/DC Debug Node 
			ch_addr=0x01;
			node_addr=0x05;
		}
		else if(strcasecmp(argv[2], "DFEDC_C") == 0)
		{ 	// DFETail/DC Debug Node 
			ch_addr=0x02;
			node_addr=0x05;
		}
		else if(strcasecmp(argv[2], "DFEDC_D") == 0)
		{ 	// DFETail/DC Debug Node 
			ch_addr=0x03;
			node_addr=0x05;
		}
		else if(strcasecmp(argv[2], "DFEDC_INDEP") == 0)
		{ 	// DFETail/DC Debug Node 
			ch_addr=0x00;
			node_addr=0x05;
		}
		else if(strcasecmp(argv[2], "DEC") == 0)
		{ 	// R1000DEC Debug Node 
			ch_addr=0x00; 
			node_addr=0x07;
		}
		else if(strcasecmp(argv[2], "CRC_A") == 0)
		{ 	// R1000CRC Debug Node 
			ch_addr=0x00;
			node_addr=0x06;
		}
		else if(strcasecmp(argv[2], "CRC_B") == 0)
		{ 	// R1000CRC Debug Node 
			ch_addr=0x01;
			node_addr=0x06;
		}
		else if(strcasecmp(argv[2], "CRC_C") == 0)
		{ 	// R1000CRC Debug Node 
			ch_addr=0x02;
			node_addr=0x06;
		}
		else if(strcasecmp(argv[2], "CRC_D") == 0)
		{ 	// R1000CRC Debug Node 
			ch_addr=0x03;
			node_addr=0x06;
		}
		else if(strcasecmp(argv[2], "AN") == 0)
		{ 	// Autoneg Debug Node 
			ch_addr=0x00; 
			node_addr=0x0F;
		}
		else if(strcasecmp(argv[2], "CMI") == 0)
		{ 	// CMI Debug Node 
			ch_addr=0x03; 
			node_addr=0x0F;
		}
		else if(strcasecmp(argv[2], "SUPV") == 0)
		{ 	// SUPV PHY  Debug Node 
			ch_addr=0x00; 
			node_addr=0x0D;
		}
		else
		{
			printf("	miir_trdgb <PhyAddr> <TrRegTyp>(DSPF|PMA|TR|PCS|FFE_x|ECNC_x|DFEDC_x|DEC|CRC_x|AN|CMI|SUPV) <DataAddr>\r\n");
			return 0;
		}
		data_addr = data_addr&0x3F;
		tcMiiStationWrite(phyaddr, page_reg, Token_Ring_debug_reg);
		tcMiiStationWrite(phyaddr, Token_Ring_Control_reg, (1<<15)|(1<<13)|(ch_addr<<11)|(node_addr<<7)|(data_addr<<1));
		//while(!(tcMiiStationRead(phyaddr, Token_Ring_Control_reg)&0x8000)); 			// data ready
		{
			value_low = tcMiiStationRead(phyaddr, Token_Ring_Low_data_reg);
			value_high = tcMiiStationRead(phyaddr, Token_Ring_High_data_reg);
			value=value_low+((value_high&0x00FF)<<16);
			printf("* %s => Phyaddr=%d,TrRegTyp =%s,node_addr=0x%02lX,data_addr=0x%04lX ,value=0x%06lX\r\n", argv[2], phyaddr, argv[2],node_addr,data_addr, value);
		}
		tcMiiStationWrite(phyaddr, page_reg, 0x00);			//V1.11
		return 0;		
	}
	else if (argc==5)
	{	// tce miir_trdgb <PhyAddr> <CHAddr> <NodeAddr> <DataAddr>
        phyaddr = checked_atoi(argv[1]);		//0~31
   	    ch_addr = checked_atoi(argv[2]);		//0:A pair  /1:B pair  /2:C pair  /3:D pair 
		sscanf(argv[3], "%lx", &node_addr);		// 4 bit HEX
		sscanf(argv[4], "%lx", &data_addr);		// 6 bit HEX
    
		tcMiiStationWrite(phyaddr, page_reg, Token_Ring_debug_reg);
		tcMiiStationWrite(phyaddr, Token_Ring_Control_reg, (1<<15)|(1<<13)|(ch_addr<<11)|(node_addr<<7)|(data_addr<<1));
		//while(!(tcMiiStationRead(phyaddr, Token_Ring_Control_reg)&0x8000)); 			// data ready
		{
			value_low = tcMiiStationRead(phyaddr, Token_Ring_Low_data_reg);		
			value_high = tcMiiStationRead(phyaddr, Token_Ring_High_data_reg);			
			value = value_low + ((value_high&0x00FF)<<16);
			printf("* miir_trdgb => Phyaddr=%d, ch_addr=%d,node_addr=0x%02lX,data_addr=0x%04lX ,value=0x%06lX\r\n", phyaddr, ch_addr, node_addr, data_addr, value);
		}
		tcMiiStationWrite(phyaddr, page_reg, 0x00);		//V1.11
		return 0;
    }
	else if(argc ==6) 
	{	//miir_trdgb all <PhyAddr> <TrRegTyp> <s_addr> <e_addr>
		phyaddr = checked_atoi(argv[2]);		//0~31
		sscanf(argv[4], "%lx", &start_addr);	//hex
		sscanf(argv[5], "%lx", &end_addr);		//hex
		for(i=start_addr; i<=end_addr; i++)
		{
		    data_addr=i;
	        if(stricmp(argv[3], "DSPF") == 0)
	        {	// DSP Filter Debug Node
        	    ch_addr=0x02;
				node_addr=0x0D;
		    }
			else if(stricmp(argv[3], "PMA") == 0)
			{ 	// PMA Debug Node 
					ch_addr=0x01;
					node_addr=0x0F;
			}
			else if(stricmp(argv[3], "TR") == 0)
			{ 	// Timing Recovery  Debug Node 
				ch_addr=0x01;
				node_addr=0x0D;
			}
			else if(stricmp(argv[3], "PCS") == 0)
			{ 	// R1000PCS Debug Node 
				ch_addr=0x02;
				node_addr=0x0F;
			}
			else if(stricmp(argv[3], "FFE_A") == 0)
			{ 	// FFE Debug Node 
				ch_addr=0x00;
				node_addr=0x04;
			}
			else if(stricmp(argv[3], "FFE_B") == 0)
			{ 	// FFE Debug Node 
				ch_addr=0x01;
				node_addr=0x04;
			}
			else if(stricmp(argv[3], "FFE_C") == 0)
			{ 	// FFE Debug Node 
				ch_addr=0x02;
				node_addr=0x04;
			}
			else if(stricmp(argv[3], "FFE_D") == 0)
			{ 	// FFE Debug Node 
				ch_addr=0x03;
				node_addr=0x04;
			}
			else if(stricmp(argv[3], "FFE_INDEP") == 0)
			{ 	// FFE Debug Node 
				ch_addr=0x00;
				node_addr=0x04;
			}
 			else if(stricmp(argv[3], "ECNC_A") == 0)
 			{ 	// EC/NC Debug Node 
				ch_addr=0x00;
				node_addr=(data_addr&0x40)>>6;
			}
 			else if(stricmp(argv[3], "ECNC_B") == 0)
 			{ 	// EC/NC Debug Node 
				ch_addr=0x01;
				node_addr=(data_addr&0x40)>>6;
			}
 			else if(stricmp(argv[3], "ECNC_C") == 0)
 			{ 	// EC/NC Debug Node 
				ch_addr=0x02;
				node_addr=(data_addr&0x40)>>6;
			}
 			else if(stricmp(argv[3], "ECNC_D") == 0)
 			{ 	// EC/NC Debug Node 
				ch_addr=0x03;
				node_addr=(data_addr&0x40)>>6;
			}
 			else if(stricmp(argv[3], "ECNC_INDEP") == 0)
 			{ 	// EC/NC Debug Node 
				ch_addr=0x00;
				node_addr=(data_addr&0xC0)>>6;
			}
			else if(stricmp(argv[3], "DFEDC_A") == 0)
			{ 	// DFETail/DC Debug Node 
				ch_addr=0x00;
				node_addr=0x05;
			}
			else if(stricmp(argv[3], "DFEDC_B") == 0)
			{ 	// DFETail/DC Debug Node 
				ch_addr=0x01;
				node_addr=0x05;
			}
			else if(stricmp(argv[3], "DFEDC_C") == 0)
			{ 	// DFETail/DC Debug Node 
				ch_addr=0x02;
				node_addr=0x05;
			}
			else if(stricmp(argv[3], "DFEDC_D") == 0)
			{ 	// DFETail/DC Debug Node 
				ch_addr=0x03;
				node_addr=0x05;
			}
			else if(stricmp(argv[3], "DFEDC_INDEP") == 0)
			{ 	// DFETail/DC Debug Node 
				ch_addr=0x00;
				node_addr=0x05;
			}
			else if(stricmp(argv[3], "DEC") == 0)
			{ 	// R1000DEC Debug Node 
				ch_addr=0x00; 
				node_addr=0x07;
			}
			else if(stricmp(argv[3], "CRC_A") == 0)
			{ 	// R1000CRC Debug Node 
				ch_addr=0x00;
				node_addr=0x06;
			}
			else if(stricmp(argv[3], "CRC_B") == 0)
			{ 	// R1000CRC Debug Node 
				ch_addr=0x01;
				node_addr=0x06;
			}
			else if(stricmp(argv[3], "CRC_C") == 0)
			{ 	// R1000CRC Debug Node 
				ch_addr=0x02;
				node_addr=0x06;
			}
			else if(stricmp(argv[3], "CRC_D") == 0)
			{ 	// R1000CRC Debug Node 
				ch_addr=0x03;
				node_addr=0x06;
			}
			else if(stricmp(argv[3], "AN") == 0)
			{ 	// Autoneg Debug Node 
				ch_addr=0x00; 
				node_addr=0x0F;
			}
			else if(stricmp(argv[3], "CMI") == 0)
			{ 	// CMI Debug Node 
				ch_addr=0x03; 
				node_addr=0x0F;
			}
			else if(stricmp(argv[3], "SUPV") == 0)
			{ 	// SUPV PHY  Debug Node 
				ch_addr=0x00; 
				node_addr=0x0D;
			}
			else 
			{
				printf("	miir_trdgb all <PhyAddr> <TrRegTyp>(DSPF|PMA|TR|PCS|FFE_x|ECNC_x|DFEDC_x|DEC|CRC_x|AN|CMI|SUPV) <s_addr> <e_addr>\r\n");
				return 0;
			}
			data_addr=data_addr&0x3F;
			tcMiiStationWrite(phyaddr, page_reg, Token_Ring_debug_reg);
			tcMiiStationWrite(phyaddr, Token_Ring_Control_reg, (1<<15)|(1<<13)|(ch_addr<<11)|(node_addr<<7)|(data_addr<<1));
			//while(!(tcMiiStationRead(phyaddr, Token_Ring_Control_reg)&0x8000)); // data ready
			{
				value_low = tcMiiStationRead(phyaddr, Token_Ring_Low_data_reg);
				value_high = tcMiiStationRead(phyaddr, Token_Ring_High_data_reg);
				value = value_low + ((value_high&0x00FF)<<16);
				printf("* %s => Phyaddr=%d,TrRegTyp =%s,node_addr=0x%02lX,data_addr=0x%04lX,value=0x%06lX\r\n", argv[2], phyaddr, argv[2],node_addr, i, value);
			}
		}
		tcMiiStationWrite(phyaddr, page_reg, 0x00);		//V1.11
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
	
	if(argc ==5)
	{	// tce miiw_trdgb <PhyAddr> <TrRegType> <DataAddr> <Value>	
		phyaddr = checked_atoi(argv[1]);//0~31
		sscanf(argv[3], "%lx", &data_addr);// 6 bit HEX
		if(strcasecmp(argv[2], "DSPF") == 0)
		{	// DSP Filter Debug Node
			ch_addr=0x02;
		   	node_addr=0x0D;
		}
		else if(strcasecmp(argv[2], "PMA") == 0)
		{ 	// PMA Debug Node 
			ch_addr=0x01;
			node_addr=0x0F;
		}
		else if(strcasecmp(argv[2], "TR") == 0)
		{ 	// Timing Recovery  Debug Node 
			ch_addr=0x01;
			node_addr=0x0D;
		}
		else if(strcasecmp(argv[2], "PCS") == 0)
		{ 	// R1000PCS Debug Node 
			ch_addr=0x02;
			node_addr=0x0F;
		}
		else if(strcasecmp(argv[2], "FFE_A") == 0)
		{ 	// FFE Debug Node 
			ch_addr=0x00;
			node_addr=0x04;
		}
		else if(strcasecmp(argv[2], "FFE_B") == 0)
		{ 	// FFE Debug Node 
			ch_addr=0x01;
			node_addr=0x04;
		}
		else if(strcasecmp(argv[2], "FFE_C") == 0)
		{ 	// FFE Debug Node 
			ch_addr=0x02;
			node_addr=0x04;
		}
		else if(strcasecmp(argv[2], "FFE_D") == 0)
		{ 	// FFE Debug Node 
			ch_addr=0x03;
			node_addr=0x04;
		}
		else if(strcasecmp(argv[2], "FFE_INDEP") == 0)
		{ 	// FFE Debug Node 
			ch_addr=0x00;
			node_addr=0x04;
		}
		else if(strcasecmp(argv[2], "ECNC_A") == 0)
		{ 	// EC/NC Debug Node 
			ch_addr=0x00;
			node_addr=(data_addr&0x40)>>6;
		}
		else if(strcasecmp(argv[2], "ECNC_B") == 0)
		{ 	// EC/NC Debug Node 
			ch_addr=0x01;
			node_addr=(data_addr&0x40)>>6;
		}
		else if(strcasecmp(argv[2], "ECNC_C") == 0)
		{ 	// EC/NC Debug Node 
			ch_addr=0x02;
			node_addr=(data_addr&0x40)>>6;
		}
		else if(strcasecmp(argv[2], "ECNC_D") == 0)
		{ 	// EC/NC Debug Node 
			ch_addr=0x03;
			node_addr=(data_addr&0x40)>>6;
		}
		else if(strcasecmp(argv[2], "ECNC_INDEP") == 0)
		{ 	// EC/NC Debug Node 
			ch_addr=0x00;
			node_addr=(data_addr&0xC0)>>6;
		}
		else if(strcasecmp(argv[2], "DFEDC_A") == 0)
		{ 	// DFETail/DC Debug Node 
			ch_addr=0x00;
			node_addr=0x05;
		}
		else if(strcasecmp(argv[2], "DFEDC_B") == 0)
		{ 	// DFETail/DC Debug Node 
			ch_addr=0x01;
			node_addr=0x05;
		}
		else if(strcasecmp(argv[2], "DFEDC_C") == 0)
		{ 	// DFETail/DC Debug Node 
			ch_addr=0x02;
			node_addr=0x05;
		}
		else if(strcasecmp(argv[2], "DFEDC_D") == 0)
		{ 	// DFETail/DC Debug Node 
			ch_addr=0x03;
			node_addr=0x05;
		}
		else if(strcasecmp(argv[2], "DFEDC_INDEP") == 0)
		{ 	// DFETail/DC Debug Node 
			ch_addr=0x00;
			node_addr=0x05;
		}
		else if(strcasecmp(argv[2], "DEC") == 0)
		{ 	// R1000DEC Debug Node 
			ch_addr=0x00; 
			node_addr=0x07;
		}
		else if(strcasecmp(argv[2], "CRC_A") == 0)
		{ 	// R1000CRC Debug Node 
			ch_addr=0x00;
			node_addr=0x06;
		}
		else if(strcasecmp(argv[2], "CRC_B") == 0)
		{ 	// R1000CRC Debug Node 
			ch_addr=0x01;
			node_addr=0x06;
		}
		else if(strcasecmp(argv[2], "CRC_C") == 0)
		{ 	// R1000CRC Debug Node 
			ch_addr=0x02;
			node_addr=0x06;
		}
		else if(strcasecmp(argv[2], "CRC_D") == 0)
		{ 	// R1000CRC Debug Node 
			ch_addr=0x03;
			node_addr=0x06;
		}
		else if(strcasecmp(argv[2], "AN") == 0)
		{ 	// Autoneg Debug Node 
			ch_addr=0x00; 
			node_addr=0x0F;
		}
		else if(strcasecmp(argv[2], "CMI") == 0)
		{ 	// CMI Debug Node 
			ch_addr=0x03; 
			node_addr=0x0F;
		}
		else if(strcasecmp(argv[2], "SUPV") == 0)
		{ 	// SUPV PHY  Debug Node 
			ch_addr=0x00; 
			node_addr=0x0D;
		}
		else 
		{
			printf("	miiw_trdgb <PhyAddr> <TrRegTyp>(DSPF|PMA|TR|PCS|FFE_x|ECNC_x|DFEDC_x|DEC|CRC_x|AN|CMI|SUPV) <DataAddr> <Value>\r\n");
			return 0;
		}
        data_addr=data_addr&0x3F;
		sscanf(argv[4], "%lx", &value);			// 24 bit //HEX
		value_high=(0x00FF0000&value)>>16;
		value_low=(0x0000FFFF&value);

		tcMiiStationWrite(phyaddr, page_reg, Token_Ring_debug_reg);	   
		tcMiiStationWrite(phyaddr, Token_Ring_Low_data_reg, value_low);
		tcMiiStationWrite(phyaddr, Token_Ring_High_data_reg, value_high);
		tcMiiStationWrite(phyaddr, Token_Ring_Control_reg, (1<<15)|(0<<13)|(ch_addr<<11)|(node_addr<<7)|(data_addr<<1));
		//while(!(tcMiiStationRead(phyaddr, Token_Ring_Control_reg)&0x8000)); // data ready
	   	{
			printf("* %s => Phyaddr=%d, ch_addr=%d, node_addr=0x%02lX, data_addr=0x%04lX , value=0x%06lX\r\n",argv[2], phyaddr, ch_addr, node_addr, data_addr, value);
		}
		tcMiiStationWrite(phyaddr, page_reg, 0x00);		//V1.11
		return 0;
	}
	else if (argc==6)
	{
    	// tce miiw_trdgb <PhyAddr> <CHAddr> <NodeAddr> <DataAddr> <Value>
    	phyaddr = checked_atoi(argv[1]);		//0~31
    	ch_addr = checked_atoi(argv[2]);		//0:A pair  /1:B pair  /2:C pair  /3:D pair 
    	sscanf(argv[3], "%lx", &node_addr);		// 4 bit //HEX
    	sscanf(argv[4], "%lx", &data_addr);		// 6 bit //HEX
    	sscanf(argv[5], "%lx", &value);			// 24 bit //HEX
        value_high=(0x00FF0000&value)>>16;
        value_low=(0x0000FFFF&value);
        tcMiiStationWrite(phyaddr, page_reg, Token_Ring_debug_reg);	
        tcMiiStationWrite(phyaddr, Token_Ring_Low_data_reg, value_low);
        tcMiiStationWrite(phyaddr, Token_Ring_High_data_reg, value_high);
        tcMiiStationWrite(phyaddr, Token_Ring_Control_reg, (1<<15)|(0<<13)|(ch_addr<<11)|(node_addr<<7)|(data_addr<<1));
        //while(!(tcMiiStationRead(phyaddr, Token_Ring_Control_reg)&0x8000)); // data ready
		{
			printf("* miiw_trdgb ready => Phyaddr=%d, ch_addr=%d, node_addr=0x%02lX, data_addr=0x%04lX , value=0x%06lX\r\n", phyaddr, ch_addr, node_addr, data_addr, value);
		}
		tcMiiStationWrite(phyaddr, page_reg, 0x00);		//V1.11
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
int doReadPhycoeff(int argc, char *argv[], void *p)		//coeffr <PhyAddr> <FFE/DEC/EC/NC/TR/DFE>
{ 
	uint16 	phyaddr=0;
 	uint32 	value;
 	uint8 	i, j;
 	uint8 	sw_patch_flag_bak;			// V1.11
 	char 	*ch_tbl[4]={"A","B","C","D"};

	sw_patch_flag_bak = sw_patch_flag;	// V1.11
  	if(argc==3)
  	{
    	sw_patch_flag = 0;
        mtPhyMiiWrite_TrDbg(phyaddr, "PMA", 4, 0x60000, 0);		//V1.11  force freeze DSP
        phyaddr = checked_atoi(argv[1]);
        printf("* PhyAddr=%d\r\n", phyaddr);
        if((stricmp(argv[2], "ffe") == 0) ||(stricmp(argv[2], "FFE") == 0))
        {
        	for(j=0; j<4; j++)
        	{	//chA~chD
              	for(i=0; i<16; i++)
              	{
                	value = mtPhyMiiRead_TrDbg(phyaddr, "FFE", i,j);
                  	printf("* FFE_%02xh CH%s value=0x%04lX\r\n", i,ch_tbl[j], value);
                }
            }
        }
        else if((stricmp(argv[2], "dec") == 0) ||(stricmp(argv[2], "DEC") == 0))
        {    
        	for(j=0; j<4; j++)
        	{
            	for(i=0; i<3; i++)
            	{
                	value = mtPhyMiiRead_TrDbg(phyaddr, "DEC", (4*j+i),0); 
                    printf("* DEC_%02xh CH%s, value=0x%04lX\r\n",(4*j+i),ch_tbl[j],value);
                }
            }
        } 
        else if((stricmp(argv[2], "ec") == 0) ||(stricmp(argv[2], "EC") == 0))
        {
        	for(j=0; j<4; j++)
        	{
            	for(i=0; i<64; i++)
            	{
                	value = mtPhyMiiRead_TrDbg(phyaddr, "EC", i, j); 
                    printf("* EC[%02d], CH%s value=0x%04lX\r\n", i,ch_tbl[j],value);
                }
              	for(i=0; i<16; i++)
              	{
                	value = mtPhyMiiRead_TrDbg(phyaddr, "ECT", i, j); 
                  	//printf("* PhyAddr=%d, ECTail[%02d], CH%s value=0x%04lX\r\n", i,ch_tbl[j],value);
                  	printf("* PhyAddr=%d, ECTail[%02d], CH%s value=0x%04lX\r\n", phyaddr,i,ch_tbl[j],value);
                }
            }
        } 
        else if((stricmp(argv[2], "nc") == 0) ||(stricmp(argv[2], "NC") == 0))
        {
        	for(j=0; j<4; j++)
            {
            	for(i=16; i<32; i++)
                {
                	value = mtPhyMiiRead_TrDbg(phyaddr, "NC", i, j); 
                    printf("* NC1[%02d], CH%s value=0x%04lX\r\n", i, ch_tbl[j], value);
                }
                for(i=32; i<48; i++)
                {
                	value = mtPhyMiiRead_TrDbg(phyaddr, "NC", i, j); 
                    printf("* NC2[%02d], CH%s value=0x%04lX\r\n", i, ch_tbl[j], value);
                }
                for(i=48; i<64; i++)
                {
                	value = mtPhyMiiRead_TrDbg(phyaddr, "NC", i, j); 
                    printf("* NC3[%02d], CH%s value=0x%04lX\r\n", i, ch_tbl[j], value);
                }  
            }
        }
        else if((stricmp(argv[2], "tr") == 0) ||(stricmp(argv[2], "TR") == 0))
        {
        	for(i=0; i<16; i++)
        	{
            	value = mtPhyMiiRead_TrDbg(phyaddr, "NC", i, 0); 
                printf("* NC[%02d], CH%s value=0x%04lX\r\n", i, ch_tbl[j], value);
            }
        }
        else if((stricmp(argv[2], "dfe") == 0) ||(stricmp(argv[2], "DFE") == 0))
        {
        	for(j=0; j<4; j++)
        	{
				for(i=0; i<3; i++)
				{	//DFE tap0/1/2
					value = mtPhyMiiRead_TrDbg(phyaddr, "DEC", (4*j+i), j); 
					printf("* DFETap[%02d], CH%s value=0x%06lX\r\n", i, ch_tbl[j], value);
				}  
			}
            for(j=0; j<4; j++)
            {
                for(i=0; i<16; i++)
                {	//DFE Tail
                    value = mtPhyMiiRead_TrDbg(phyaddr, "DFEDC", i, j); 
                    printf("* DFEDC[%02d], CH%s value=0x%04lX\r\n", i, ch_tbl[j], value);
                }
            }
        }
		mtPhyMiiWrite_TrDbg(phyaddr, "PMA", 4, 0x40000, 0);		//V1.11  un-force freeze DSP
		sw_patch_flag = sw_patch_flag_bak;						//V1.11
 	}  
  	else 
  	{ 	// error message
      	printf("Usage: coeffr <PhyAddr> <ffe/dec/ec/nc/tr/dfe>\r\n");
      	return 0;           
  	}
  	// check parameters
  	return 0;           
}

#ifdef MTPHY_DEBUG //MT7530
static int doPhyDispFlag(int argc, char *argv[], void *p)
{
    int i;
    if(argc >= 2 && argc <= 9) 
	{
        tcPhy_disp_level = 0;
        for(i=1; i<argc; i++) 
		{
            if(stricmp(argv[i], "on") == 0)
			{
                tcPhy_disp_level++;
            }
        }
    }   
    printf("tcPhy display level: %d.\r\n", tcPhy_disp_level);
    return 0;
}

static int doSkewDispFlag(int argc, char *argv[], void *p)
{
	if(argc >= 2 && argc <= 9) 
	{
		mtSkewCal_disp_level = 0;
		if(stricmp(argv[1], "on") == 0)
		{
			mtSkewCal_disp_level++;
		}
	}   
    printf("1000M Skew cal SW patch display level: %d.\r\n", mtSkewCal_disp_level);
    return 0;
}

int doPhyForceMode(int argc, char *argv[], void *p) 	//biker_20130814, add command for QA auto test
{
	uint16 port_num = 0, phyaddr = 0;
	uint32 reg3 = 0, r15_temp = 0;
	
	phyaddr = checked_atoi(argv[1]);
	reg3 = tcMiiStationRead(phyaddr, 3);
	r15_temp = tcMiiStationRead(phyaddr, 15);

	if(reg3==EPHY_ID_7512Ge) // 7512GE
	{
		tcPhyVer = tcPhyVer_7512Ge;
	}
	else if((reg3 == EPHY_ID_7530Ge_7512Fe)&&(((r15_temp>>13)&0x1) == 1)) // 7530GE
	{
		tcPhyVer = mtPhyVer_7530;
	}
	else
	{
		tcPhyVer = tcPhyVer_7512Fe;
	}
	
	switch(tcPhyVer) 
	{    
		case mtPhyVer_7530:			// MT7530
			tcMiiStationWrite(phyaddr, 0x0, 0x0800);
			pause(1000);
			mt7530GePhyCfgLoad(0, DO_5_PORT, phyaddr);
			printf("phyaddr[%d] reset \r\n", phyaddr);
			break;

		case tcPhyVer_7512Fe: 		// MT7512 Fe
			tcMiiStationWrite(phyaddr, 0x0, 0x0800);
			pause(1000);
			mt7512FECfgLoad(0, DO_4_PORT, phyaddr);
			printf("phyaddr[%d] reset \r\n", phyaddr);
			break;
  
		case tcPhyVer_7512Ge: 		// MT7512 Ge
			tcMiiStationWrite(phyaddr, 0x0, 0x0800);
			pause(1000);
			mt7512GePhyCfgLoad(0);
			mt7512FECfgLoad(0, DO_4_PORT, phyaddr);
			printf("phyaddr[%d] reset \r\n", phyaddr);
			break;
	}
	tcMiiStationWrite(phyaddr, 0x0, 0x1040);  // [12]:AN, [6]:MSB_0x10=giga	

	if(argc == 5) 
	{ 
		if(stricmp(argv[2], "an") == 0)
		{
			tcMiiStationWrite(phyaddr, 0x00, 0x1040);
			printf("back to AN(half) \r\n");
		}
		else if(stricmp(argv[2], "force") == 0)
		{
			if ((stricmp(argv[3], "10") == 0)&&(stricmp(argv[4], "full") == 0))
			{
				tcMiiStationWrite(phyaddr, 0x00, 0x100);
				printf("force 10 full  \r\n");
			}
			else if((stricmp(argv[3], "10") == 0)&&(stricmp(argv[4], "half") == 0))
			{
				tcMiiStationWrite(phyaddr, 0x00, 0x0);
				printf("force 10 half  \r\n");
			}
			else if((stricmp(argv[3], "100") == 0)&&(stricmp(argv[4], "full") == 0))
			{
				tcMiiStationWrite(phyaddr, 0x00, 0x2100);
				printf("force 100 full  \r\n");
			}
			else if((stricmp(argv[3], "100") == 0)&&(stricmp(argv[4], "half") == 0))
			{
				tcMiiStationWrite(phyaddr, 0x00, 0x2000);
				printf("force 100 half  \r\n");
			}
			else if(stricmp(argv[3], "1000") == 0)
			{
				if((reg3==0x9421) || ((reg3 == EPHY_ID_7530Ge_7512Fe)&&(((r15_temp>>13)&0x1) == 1)))
				{
					if(stricmp(argv[4], "full") == 0)
					{
						tcMiiStationWrite(phyaddr, 0x00, 0x140);
						printf("force 1000 full  \r\n");
				 	}
					else if(stricmp(argv[4], "half") == 0)
					{
				 		//tcMiiStationWrite(phyaddr, 0x00, 0x40);
						printf("not support 1000 half \r\n");
				 	}		 	
			 	}
				else
					printf("port[%d] not support 1000 mode. \r\n",phyaddr);	
			}
		}
		else
		{
			phyaddr = 0xFFFF;
		}
	}
	else if(argc == 4)
	{
		if(stricmp(argv[2], "an") == 0)
		{
			tcMiiStationWrite(phyaddr, 0x00, 0x1040);
			printf("back to AN(half) \r\n");

			if(stricmp(argv[3], "10") == 0)
			{
				tcMiiStationWrite(phyaddr, 0x04, 0x0061);
				tcMiiStationWrite(phyaddr, 0x09, 0x0000);
			}
			else if(stricmp(argv[3], "100") == 0)
			{
				tcMiiStationWrite(phyaddr, 0x04, 0x01e1);
				tcMiiStationWrite(phyaddr, 0x09, 0x0000);
			}
			else if((stricmp(argv[3], "1000") == 0)&&((reg3==0x9421) || ((reg3 == EPHY_ID_7530Ge_7512Fe)&&(((r15_temp>>13)&0x1) == 1))))
			{
				tcMiiStationWrite(phyaddr, 0x04, 0x01e1);
				tcMiiStationWrite(phyaddr, 0x09, 0x0600);
			}
			else
			{
				phyaddr = 0xFFFF;
			}
		}
		else if(stricmp(argv[2], "force") == 0)
		{
			if(stricmp(argv[3], "10") == 0)
			{
				tcMiiStationWrite(phyaddr, 0x00, 0x0);
				printf("force 10 half  \r\n");
			}
			else if(stricmp(argv[3], "100") == 0)
			{
				tcMiiStationWrite(phyaddr, 0x00, 0x2000);
				printf("force 100 half  \r\n");
			}
			else if((stricmp(argv[3], "1000") == 0)&&((reg3==0x9421) || ((reg3 == EPHY_ID_7530Ge_7512Fe)&&(((r15_temp>>13)&0x1) == 1))))
			{
				//tcMiiStationWrite(phyaddr, 0x00, 0x40);
				printf("not support 1000 half  \r\n");
			}
			else
			{
				phyaddr= 0xFFFF;
			}
		}
		else
		{
			phyaddr= 0xFFFF;
		}
	}
	else if(argc == 3)
	{
		if(stricmp(argv[2], "an") == 0)
		{
			tcMiiStationWrite(phyaddr, 0x00, 0x1040);
			printf("back to AN(half)  \r\n");
			if((reg3==0x9421) || ((reg3 == EPHY_ID_7530Ge_7512Fe)&&(((r15_temp>>13)&0x1) == 1)))
			{
				tcMiiStationWrite(phyaddr, 0x04, 0x01e1);
				tcMiiStationWrite(phyaddr, 0x09, 0x0600);
			}
			else
			{
				tcMiiStationWrite(phyaddr, 0x04, 0x01e1);
			}
		}
		else
		{
			phyaddr= 0xFFFF;
		}
	}
	else
	{
		phyaddr= 0xFFFF;
	}
	if(phyaddr == 0xFFFF)
		printf("usage: tce forcemode <phyaddr> <an|force> <10|100|1000> <full|half>\r\n");
	return 0;
}

uint32 getPhyPortMask(void)
{
	if(isMT7520S)
		return 0x1000;		//port12
	else if( isMT7520 || isMT7525)
		return 0x1e00;		//port9-12
	else if(isMT7525G || isMT7520G)
		return 0xf;			//port0-3
	else
	{
		printf("not found phy port\n");
		return 0;
	}
}

#define PHY_EEE_ON 1
#define PHY_EEE_OFF 2
#define PHY_EEE_RECOVER 3
void phyForceEEEAllOnOffRecover(int mode)
{
	int port_num;
	uint32 reg3;
	uint32 phy_port_mask = getPhyPortMask();
	for(port_num= 0 ; port_num<32; port_num++)
	{
		if(!(phy_port_mask & (1<<port_num)))
			continue;		
		reg3 = tcMiiStationRead(port_num, 3); 
		switch(mode)
		{
			case PHY_EEE_ON:
				if((reg3 == 0x9421)||(reg3 == 9412))
				{
					mtEMiiRegWrite(port_num, 0x7, 0x003c, 0x0006);
				}
				else
				{
					tcMiiStationWrite(port_num, 31, 0xb000); //  l3
					tcMiiStationWrite(port_num, 17, 0x0002); //  17 2 : enable FE EEE
				}
				printf("force port[%d] EEE on \r\n", port_num);					
				break;
			case PHY_EEE_OFF:
				if((reg3 == 0x9421)||(reg3 == 9412))
				{
					mtEMiiRegWrite(port_num, 0x7, 0x003c, 0x0000);
				}
				else
				{
					tcMiiStationWrite(port_num, 31, 0xb000); //  l3
					tcMiiStationWrite(port_num, 17, 0x0000); //  17 2 : enable FE EEE
				}
				printf("force port[%d] EEE off \r\n", port_num);
				break;
			case PHY_EEE_RECOVER:
				if((reg3 == 0x9421)||(reg3 == 9412))
				{
					mtEMiiRegWrite(port_num, 0x7, 0x003c, default_ge_dev7r3c);
				}
				else
				{
					tcMiiStationWrite(port_num, 31, 0xb000); //  l3
					tcMiiStationWrite(port_num, 17, default_fe_l3r17); //  17 2 : enable FE EEE
				}
				printf("recover port[%d] EEE to default \r\n", port_num);
				break;
			default:
				printf("not support mode %d",mode);
				break;
		}
	}
}

int doPhyForceEEE (int argc, char *argv[], void *p) 
{
	uint16 phyaddr = 0;
	uint32 reg3 = 0, r15_temp = 0;
	if(!strcmp(argv[1],"all"))
	{
		phyaddr = 0xffff;
	}
	else
	{
		phyaddr = checked_atoi(argv[1]);
		reg3 = tcMiiStationRead(phyaddr, 3); 
	}
	r15_temp = tcMiiStationRead(phyaddr, 0xf);
	if(argc == 3) 
	{
		if((stricmp(argv[2], "on") == 0))
		{
			if(phyaddr == 0xffff)
			{
				phyForceEEEAllOnOffRecover(PHY_EEE_ON);
				return 0;
			}
			if((reg3 == 0x9421)||((reg3 == EPHY_ID_7530Ge_7512Fe)&&(((r15_temp>>13)&0x1) == 1)))
			{
				mtEMiiRegWrite(phyaddr, 0x7, 0x003c, 0x0006);
			}
			else
			{
				tcMiiStationWrite(phyaddr, 31, 0xb000); //  l3
				tcMiiStationWrite(phyaddr, 17, 0x0002); //  17 2 : enable FE EEE
			}
			printf("force port[%d] EEE on \r\n", phyaddr);
		}
		else if((stricmp(argv[2], "off") == 0))
		{
		
			if(phyaddr == 0xffff)
			{
				phyForceEEEAllOnOffRecover(PHY_EEE_OFF);
				return 0;
			}
			if((reg3 == 0x9421)||((reg3 == EPHY_ID_7530Ge_7512Fe)&&(((r15_temp>>13)&0x1) == 1)))
			{
				mtEMiiRegWrite(phyaddr, 0x7, 0x003c, 0x0000);
			}
			else
			{
				tcMiiStationWrite(phyaddr, 31, 0xb000); //  l3
				tcMiiStationWrite(phyaddr, 17, 0x0000); //  17 2 : enable FE EEE
			}
			printf("force port[%d] EEE off \r\n", phyaddr);
		}
		else if((stricmp(argv[2], "recover") == 0))
		{
		
			if(phyaddr == 0xffff){
				phyForceEEEAllOnOffRecover(PHY_EEE_RECOVER);
				return 0;
			}
			if((reg3 == 0x9421)||((reg3 == EPHY_ID_7530Ge_7512Fe)&&(((r15_temp>>13)&0x1) == 1)))
			{
				mtEMiiRegWrite(phyaddr, 0x7, 0x003c, default_ge_dev7r3c);
			}
			else
			{
				tcMiiStationWrite(phyaddr, 31, 0xb000); //  l3
				tcMiiStationWrite(phyaddr, 17, default_fe_l3r17); //  17 2 : enable FE EEE
			}
			printf("recover port[%d] EEE to default \r\n", phyaddr);
		}
		else
		{
			phyaddr = 0xFFFF;
		}
	}
	else
	{
		phyaddr = 0xFFFF;
	}
	if(phyaddr == 0xFFFF)
		printf("usage: tce forceEEE <phyaddr> <on|off|recover> \r\n");

	return 0;
}

int doPhyForceGE (int argc, char *argv[], void *p) // allen_20131120
{
	uint16 phyaddr = 0;
	uint32 reg3 = 0, r15_temp = 0;
	
	phyaddr = checked_atoi(argv[1]);
	reg3 = tcMiiStationRead(phyaddr, 3); 
	r15_temp = tcMiiStationRead(phyaddr, 0xf);
	
	if((reg3 == EPHY_ID_7512Ge)||((reg3 == EPHY_ID_7530Ge_7512Fe)&&(((r15_temp>>13)&0x1) == 1))) // GE // allen_20140108
	{
		if (argc == 4) 
		{
			if ((stricmp(argv[2], "multi") == 0))
			{
				if ((stricmp(argv[3], "master") == 0))
				{
					tcMiiStationWrite(phyaddr, 0x9, 0x1e00);
					printf("force port[%d] multi master on \r\n", phyaddr);
				}
				else if((stricmp(argv[3], "slave") == 0))
				{
					tcMiiStationWrite(phyaddr, 0x9, 0x1600);
					printf("force port[%d] multi slave on \r\n", phyaddr);
				}
			}
			else if((stricmp(argv[2], "single") == 0))
			{
				if ((stricmp(argv[3], "master") == 0))
				{
					tcMiiStationWrite(phyaddr, 0x9, 0x1a00);
					printf("force port[%d] single master on \r\n", phyaddr);
				}
				else if((stricmp(argv[3], "slave") == 0))
				{
					tcMiiStationWrite(phyaddr, 0x9, 0x1400);
					printf("force port[%d] single slave on \r\n", phyaddr);
				}
			}
			else
			{
				phyaddr = 0xFFFF;
			}			
		}
		else if(argc == 3) 
		{
			if((stricmp(argv[2], "multi") == 0))
			{
				tcMiiStationWrite(phyaddr, 0x9, 0x0600);
				printf("force port[%d] multi on \r\n", phyaddr);
			}
			else if((stricmp(argv[2], "single") == 0))
			{
				tcMiiStationWrite(phyaddr, 0x9, 0x0400);
				printf("force port[%d] single on \r\n", phyaddr);
			}
			else
			{
				phyaddr = 0xFFFF;
			}
		}
		else
		{
			phyaddr = 0xFFFF;
		}
	}
	else
	{
		phyaddr = 0xFFFF;
	}
	if(phyaddr == 0xFFFF)
		printf("usage: tce forceGE <phyaddr> <multi|single> <master|slave>\r\n");

	return 0;
}

int doPhyForceReset(int argc, char *argv[], void *p) // allen_20131120
{
	uint16 phyaddr = 0;
	uint32 reg3, r15_temp;

	phyaddr = checked_atoi(argv[1]);
	reg3 = tcMiiStationRead(phyaddr, 3); 
	r15_temp = tcMiiStationRead(phyaddr, 15); 
	
	if(reg3==EPHY_ID_7512Ge) // 7512GE
	{
		tcPhyVer = tcPhyVer_7512Ge;
	}
	else if((reg3 == EPHY_ID_7530Ge_7512Fe)&&(((r15_temp>>13)&0x1) == 1)) // 7530GE
	{
		tcPhyVer = mtPhyVer_7530;
	}
	else
	{
		tcPhyVer = tcPhyVer_7512Fe;
	}
	
	if (argc == 2) 
	{
		tcMiiStationWrite(phyaddr, 0x0, 0x0800);
		pause(1000);
		tcMiiStationWrite(phyaddr, 0x0, 0x3100);
		printf("port[%d] reset \r\n", phyaddr);

		switch (tcPhyVer) 
		{  
			case mtPhyVer_7530: // MT7530
				mt7530GePhyCfgLoad(0, DO_5_PORT, phyaddr);
			break;
			   
			case tcPhyVer_7512Fe: // MT7512 Fe
				mt7512FECfgLoad(0, DO_4_PORT, phyaddr);
			break;
			     
			case tcPhyVer_7512Ge: // MT7512 Ge
				mt7512GePhyCfgLoad(0);			
				//mt7512FECfgLoad(0, DO_4_PORT, phyaddr);
			break;
		}
	}
	else
	{
		phyaddr = 0xFFFF;
	}
	if(phyaddr == 0xFFFF)
		printf("usage: tce forceReset <phyaddr> \r\n");
	return 0;
}

int doPhyrestartAN(int argc, char *argv[], void *p) 	// allen_20131213
{
	uint16 phyaddr = 0;
	uint32 reg3, reg0;

	phyaddr = checked_atoi(argv[1]);
	reg3 = tcMiiStationRead(phyaddr, 3); 
	reg0 = tcMiiStationRead(phyaddr, 0);
	reg0 = reg0 | 0x1300;
	if(argc == 2) 
	{
		tcMiiStationWrite(phyaddr, 0x0, reg0);
		printf("phyaddr[%d] restart AN \r\n", phyaddr);		
	}
	else
	{
		phyaddr = 0xFFFF;
	}

	if(phyaddr == 0xFFFF)
		printf("usage: tce restartAN <phyaddr> \r\n");
	return 0;
}

int doPhyChkVal(int argc, char *argv[], void *p)
{	// for tcPhyChkVal					
	phychkval_flag = 1; 

	//printf(" [debug 0] (%d) \r\n", tcPhyVer);
	
   	if(argc == 2) 
	{ 	// set port_num for tcPhyChkVal()
		switch (tcPhyVer) 
	   	{
			case mtPhyVer_7530:	
				phychkval_portnum = checked_atoi(argv[1]);
				//printf(" [debug 1-0] (%d) \r\n", tcPhyVer);
				break;
				
			case tcPhyVer_7512Fe:	
				phychkval_portnum = checked_atoi(argv[1]);
				//printf(" [debug 1-1] (%d) \r\n", tcPhyVer);
				break;

			case tcPhyVer_7512Ge:	
				phychkval_portnum = checked_atoi(argv[1]);
				//printf(" [debug 1-2] (%d) \r\n", tcPhyVer);
				break;	

			default:
			//printf(" [debug 1-3] (%d) \r\n", tcPhyVer);
				break;
		}
		//printf(" [debug 2] (%d) \r\n", tcPhyVer);
    }
    //printf(" [debug 3] (%d) \r\n", tcPhyVer);
    return 0;
}

int doPhygetLinkRate (int argc, char *argv[], void *p)
{
	uint16 phyaddr = 0;
	uint32 	val;
    uint8 	mr_speed;

	phyaddr = checked_atoi(argv[1]);
	val = tcMiiStationRead(phyaddr, 0);
	mr_speed = ((val>>13)|(val>>5))&0x03;	//00:10,01:100,10:1000
	
	if(mr_speed == 0x0)
		printf(" [%d] 10M \r\n", phyaddr);	
	else if(mr_speed == 0x1)
		printf(" [%d] 100M \r\n", phyaddr);
	else if(mr_speed == 0x2)
		printf(" [%d] 1000M \r\n", phyaddr);

	return mr_speed;	
}

int doPhygetDuplex (int argc, char *argv[], void *p)
{
	uint16 phyaddr = 0, r10_temp, r5_temp, r1_temp;
	uint32 	val;
    uint8 	mr_dplx;

	phyaddr = checked_atoi(argv[1]);
	val = tcMiiStationRead(phyaddr, 0);
	mr_dplx = (val>>8)&0x01;
	r10_temp = tcMiiStationRead(phyaddr, 10);
	r5_temp = tcMiiStationRead(phyaddr, 5);
	r1_temp = tcMiiStationRead(phyaddr, 1);
	if((r1_temp>>5)&0x1 == 1)
	{
		if(((r10_temp>>11)&0x1 == 1)||((r5_temp>>8)&0x1 == 1)||((r5_temp>>6)&0x1 == 1))
		{
			mr_dplx = 1;
		}
	}
	
	if(mr_dplx == 0x0)
		printf(" [%d] half \r\n", phyaddr);	
	else if(mr_dplx == 0x1)
		printf(" [%d] full \r\n", phyaddr);
		
	return mr_dplx;	
}

int doPhygetAN (int argc, char *argv[], void *p)
{
	uint16 phyaddr = 0;
	uint32 	val;
    uint8 	mr_anen;

	phyaddr = checked_atoi(argv[1]);
	val = tcMiiStationRead(phyaddr, 0);
	mr_anen = (val>>12)&0x01;
	
	if(mr_anen == 0x0)
		printf(" [%d] AN disable \r\n", phyaddr);	
	else if(mr_anen == 0x1)
		printf(" [%d] AN enable \r\n", phyaddr);

	return mr_anen;	
}

int doPhyforcePWD (int argc, char *argv[], void *p)
{
	uint16 phyaddr = 0;

	phyaddr = checked_atoi(argv[1]);
	printf(" force [%d] power down \r\n", phyaddr);
	tcMiiStationWrite(phyaddr, 0, 0x0800);

	return 0;	
}

int doPhygetLinkStatus (int argc, char *argv[], void *p)
{
	uint16 phyaddr = 0;
	uint32 	val;
    uint8 	r1_link_status;

	phyaddr = checked_atoi(argv[1]);
	val = tcMiiStationRead(phyaddr, 1);
	r1_link_status = (val>>2)&0x01;
	
	if(r1_link_status == 0x0)
		printf(" [%d] link down \r\n", phyaddr);	
	else if(r1_link_status == 0x1)
		printf(" [%d] link up \r\n", phyaddr);

	return r1_link_status;	
}

int doPhyforcePause (int argc, char *argv[], void *p)
{	// pause: r4[11:10]
	uint16 phyaddr = 0, r4_temp = 0;
	
	phyaddr = checked_atoi(argv[1]);
	if((stricmp(argv[2], "on") == 0))
	{
		r4_temp = tcMiiStationRead(phyaddr, 4);
		tcMiiStationWrite(phyaddr, 4, (r4_temp|0x0c00));
		printf(" force [%d] pause on \r\n", phyaddr);
	}
	else if((stricmp(argv[2], "off") == 0))
	{
		r4_temp = tcMiiStationRead(phyaddr, 4);
		tcMiiStationWrite(phyaddr, 4, (r4_temp&0xf3ff));
		printf(" force [%d] pause off \r\n", phyaddr);
	}

	return 0;	
}

int doPhygetLoopback (int argc, char *argv[], void *p)
{
	uint16 phyaddr = 0;
	uint32 	val;
    uint8 	r0_loopback;

	phyaddr = checked_atoi(argv[1]);
	val = tcMiiStationRead(phyaddr, 0);
	r0_loopback = (val>>14)&0x01;
	
	if(r0_loopback == 0x0)
		printf(" [%d] loopback off \r\n", phyaddr);	
	else if(r0_loopback == 0x1)
		printf(" [%d] loopback on \r\n", phyaddr);

	return r0_loopback;	
}

int doPhyforceLoopback (int argc, char *argv[], void *p)
{
	uint16 phyaddr = 0, r0_temp = 0;

	phyaddr = checked_atoi(argv[1]);
	r0_temp = tcMiiStationRead(phyaddr, 0);
	printf(" force [%d] loopback \r\n", phyaddr);
	tcMiiStationWrite(phyaddr, 0, (r0_temp|0x4000));

	return 0;
}

int doPhygetLPCap (int argc, char *argv[], void *p)
{
	uint16 phyaddr = 0, r5_temp = 0, r10_temp = 0;

	phyaddr = checked_atoi(argv[1]);
	r5_temp = tcMiiStationRead(phyaddr, 5);
	r10_temp = tcMiiStationRead(phyaddr, 10);

	if(((r10_temp>>11)&0x1) == 1)
	{
		printf(" [%d] LPCap support 1000M \r\n", phyaddr);	
		return 2;    // 1000
	}	
	else
	{
		if(((r5_temp>>7)&0x3) != 0)
		{
			printf(" [%d] LPCap support 100M \r\n", phyaddr);	
			return 1;    // 100
		}
		else
		{
			return 0;	// 10 or not an up
		}
	}
}

int doPhygetCALReg (int argc, char *argv[], void *p)
{
	uint16 	phyaddr = 0, port_num = 0;
	uint16	l2_30=0, l0_0=0, l0_26=0, g2_25=0, l3_25=0;
	uint16	l0_30=0, l4_21=0, g4_21=0, l2_23=0;
	uint16	g1_26=0, l4_23=0;
	uint32  delay=20, cal_case=0;

	phyaddr = checked_atoi(argv[1]);

	if(argc == 4)
	{
		delay = checked_atoi(argv[2]);
		cal_case =  checked_atoi(argv[3]);
	}
	
	port_num = phyaddr - ephy_addr_base;

	if(cal_case == 1) // R50
	{
		FECal_R50_flag = 0;							// cmd
		//while(FECal_R50_flag == 0)					// cmd
			FECal_R50(port_num, delay);				// cmd	
	}
	else if(cal_case == 2) // Tx offset
	{
		FECal_tx_offset_flag = 0;					// cmd
		//while(FECal_tx_offset_flag == 0)			// cmd
			FECal_Tx_offset(port_num, delay);		// cmd
	}
	else if(cal_case == 3) // Tx amp
	{
		FECal_flag = 0;								// cmd
		//while(FECal_flag == 0)						// cmd
			FECal_TxAmp(port_num, delay);			// cmd	
		tcPhyWriteGReg(port_num, 1, 26, 0x0000);	
	}
	else if(cal_case == 4) // Rext
	{
		GECal_flag = 0;
		//while(GECal_flag == 0)
			GECal_Rext(phyaddr, delay);
	}
	else if(cal_case == 5) // GE tx amp
	{
		GECal_flag = 0;
		//while(GECal_flag == 0)
			GECal_R50(phyaddr, delay);
	}
	else if(cal_case == 6) // GE tx amp
	{
		GECal_flag = 0;
		//while(GECal_flag == 0)
			GECal_tx_amp(phyaddr, delay);
	}
	
	tcMiiStationWrite(phyaddr, 31, 0x8000); // l0
	l0_0 = tcMiiStationRead(phyaddr, 0);
	l0_26 = tcMiiStationRead(phyaddr, 26);
	l0_30 = tcMiiStationRead(phyaddr, 30);
	
	tcMiiStationWrite(phyaddr, 31, 0xa000); // l2
	l2_23 = tcMiiStationRead(phyaddr, 23);
	l2_30 = tcMiiStationRead(phyaddr, 30);

	tcMiiStationWrite(phyaddr, 31, 0xb000); // l3
	l3_25 = tcMiiStationRead(phyaddr, 25);

	tcMiiStationWrite(phyaddr, 31, 0xc000); // l4
	l4_21 = tcMiiStationRead(phyaddr, 21); 
	l4_23 = tcMiiStationRead(phyaddr, 23); 

	tcMiiStationWrite(phyaddr, 31, 0x1000); // g1
	g1_26 = tcMiiStationRead(phyaddr, 26); 
	
	tcMiiStationWrite(phyaddr, 31, 0x2000); // g2
	g2_25 = tcMiiStationRead(phyaddr, 25); 

	tcMiiStationWrite(phyaddr, 31, 0x4000); // g4
	g4_21 = tcMiiStationRead(phyaddr, 21);

	tcMiiStationWrite(phyaddr, 31, 0x0000);

	printf("l2_30=0x%x, l0_0 =0x%x, l0_26=0x%x, g2_25=0x%x, l3_25=0x%x, \r\n", l2_30, l0_0 , l0_26, g2_25, l3_25);
	printf("l0_30=0x%x, l4_21=0x%x, g4_21=0x%x, l2_23=0x%x,             \r\n", l0_30, l4_21, g4_21, l2_23);
	printf("g1_26=0x%x, l4_23=0x%x,                                     \r\n", g1_26, l4_23);

	return 0;
}

int doPhyforceLinkDownPowerSaving (int argc, char *argv[], void *p)  // allen_20160415
{
	uint16 	phyaddr = 0, port_num = 0;
	uint16	l0_30=0;

	phyaddr = checked_atoi(argv[1]);
	port_num = phyaddr - ephy_addr_base;

	if((stricmp(argv[2], "on") == 0))
	{
		if((phyaddr>=8)&&(phyaddr<=11))
		{
			tcMiiStationWrite(phyaddr, 31, 0x8000); // l0
			l0_30 = tcMiiStationRead(phyaddr, 30);
			tcMiiStationWrite(phyaddr, 30, 0xa000);
			printf("l0_30 = 0x%x \r\n",  tcMiiStationRead(phyaddr, 30));
		}else
			printf(" phyaddr = 8~11 only \r\n");
	}
	else if((stricmp(argv[2], "off") == 0))
	{
		if((phyaddr>=8)&&(phyaddr<=11))
		{
			tcMiiStationWrite(phyaddr, 31, 0x8000); // l0
			l0_30 = tcMiiStationRead(phyaddr, 30);
			tcMiiStationWrite(phyaddr, 30, 0x0000);
			printf("l0_30 = 0x%x \r\n",  tcMiiStationRead(phyaddr, 30));
		}else
			printf(" phyaddr = 8~11 only \r\n");
	}
	else
	{
		printf("Usage: forceLDPS <phyaddr> <on|off> \r\n");
	}
	tcMiiStationWrite(phyaddr, 31, 0x0000);
}

int doPhyset7530_15R(int argc, char *argv[], void *p)	// allen_20160428
{				
	uint16 	phyaddr = 0;

	if((stricmp(argv[1], "on") == 0))
	{
	   	for(phyaddr = 0; phyaddr<=4; phyaddr ++)
   		{
			mtEMiiRegWrite(phyaddr, 0x1f, 0x108, 0xd010);		// 15R match
			mtEMiiRegWrite(phyaddr, 0x1e,  0x45, 0x0706);		// R offset
			mtEMiiRegWrite(phyaddr, 0x1e,  0x46, 0x0707);		// R offset
			mtEMiiRegWrite(phyaddr, 0x1f,  0x44, 0x0000);		// for 1000 diff A&B
   		}
   		printf(" 7530 set 15R \r\n");
	}
	else
	{
		for(phyaddr = 0; phyaddr<=4; phyaddr ++)
   		{
			mtEMiiRegWrite(phyaddr, 0x1f, 0x108, 0x1010);		
			mtEMiiRegWrite(phyaddr, 0x1e,  0x45, 0x0404);	
			mtEMiiRegWrite(phyaddr, 0x1e,  0x46, 0x0404);	
			mtEMiiRegWrite(phyaddr, 0x1f,  0x44, 0x00a0);	
   		}
   		printf(" 7530 set 0R \r\n");
	}

    return 0;
}

int doPhyForceLED(int argc, char *argv[], void *p) 	// allen_20160630 
{
	uint16 port_num = 0, phyaddr = 0;
	uint32 reg3 = 0, r15_temp = 0, dev_1f_24_temp = 0, dev_1f_24 = 0;
	
	phyaddr = checked_atoi(argv[1]);
	reg3 = tcMiiStationRead(phyaddr, 3);
	r15_temp = tcMiiStationRead(phyaddr, 15);

	if(reg3==EPHY_ID_7512Ge) // 7512GE
	{
		tcPhyVer = tcPhyVer_7512Ge;
	}
	else if((reg3 == EPHY_ID_7530Ge_7512Fe)&&(((r15_temp>>13)&0x1) == 1)) // 7530GE
	{
		tcPhyVer = mtPhyVer_7530;
	}
	else
	{
		tcPhyVer = tcPhyVer_7512Fe;
	}

	//printf("(%d) tcPhyVer = %x \r\n", phyaddr, tcPhyVer);

	if(tcPhyVer == mtPhyVer_7530)
	{
		dev_1f_24_temp = mtEMiiRegRead(phyaddr, 0x1f, 0x24);
		//printf("1f_24 = 0x%x \r\n", dev_1f_24_temp);
		
		if((stricmp(argv[2], "enable") == 0))
		{
			dev_1f_24 = (dev_1f_24_temp | (0x1<<15));		
			mtEMiiRegWrite(phyaddr, 0x1f, 0x24, dev_1f_24);
			//printf("1f_24 = 0x%x \r\n", dev_1f_24);
		}
		else if((stricmp(argv[2], "disable") == 0))
		{
			dev_1f_24 = (dev_1f_24_temp & 0x7fff);		
			mtEMiiRegWrite(phyaddr, 0x1f, 0x24, dev_1f_24);
			//printf("1f_24 = 0x%x \r\n", dev_1f_24);
		}
		else
		{
			printf("Usage: forceLED <phyaddr> <enable|disable> \r\n");
		}
	}
	return 0;
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

	if (argc!=4 && argc!=6)
	{
		printf("Usage: emiir all <phyaddr> <devaddr> <s_addr> <e_addr>\r\n");
		printf("       emiir <phyaddr> <devaddr> <reg>\r\n");
		return 0;			
	}
	
	if (strcasecmp(argv[1], "all") == 0)
	{ 	/*Post Read*/
		phyaddr = checked_atoi(argv[2]);//0~31
		sscanf(argv[3], "%lx", &dev_addr);//hex
		sscanf(argv[4], "%lx", &start_addr);//hex
		sscanf(argv[5], "%lx", &end_addr);//hex
		if((start_addr>65535)  ||(end_addr>65535))
		{
			printk("s_addr or e_addr must be less than 65536\r\n");
			return 0;
		}
		for(i=start_addr; i<=end_addr; i++)
		{
			#ifdef CL45_CMD_SUPPORT
			value = mtEMiiRegRead(phyaddr, dev_addr, i);
			printf("* doPhyMMDRead =>phyaddr=%d,  dev_addr=%d, data_addr=0x%04lX , value=0x%04lX\r\n", phyaddr, dev_addr,  i, value);
			#else
			tcMiiStationWrite(phyaddr, page_reg, 0x00); 					//switch to main page
			tcMiiStationWrite(phyaddr, MMD_Control_register, (0<<14)+dev_addr);
			tcMiiStationWrite(phyaddr, MMD_addr_data_register, i);
			tcMiiStationWrite(phyaddr, MMD_Control_register, (1<<14)+dev_addr);
			value = tcMiiStationRead(phyaddr, MMD_addr_data_register);
			printf("* doPhyMMDRead_CL22 =>phyaddr=%d,  dev_addr=%d, data_addr=0x%04lX , value=0x%04lX\r\n", phyaddr, dev_addr,  i, value);
			#endif
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
		value = mtEMiiRegRead(phyaddr, dev_addr, reg_addr);
		printf("* doPhyMMDRead =>phyaddr=%d,  dev_addr=%d, data_addr=0x%04lX , value=0x%04lX\r\n", phyaddr, dev_addr, reg_addr, value);
		#else
		tcMiiStationWrite(phyaddr, page_reg, 0x00); 						//switch to main page
		tcMiiStationWrite(phyaddr, MMD_Control_register, (0<<14)+dev_addr);
		tcMiiStationWrite(phyaddr, MMD_addr_data_register, reg_addr);
		tcMiiStationWrite(phyaddr, MMD_Control_register, (1<<14)+dev_addr);
		value = tcMiiStationRead(phyaddr, MMD_addr_data_register);
		printf("* doPhyMMDRead_CL22=>phyaddr=%d,  dev_addr=%d, data_addr=0x%04lX , value=0x%04lX\r\n", phyaddr, dev_addr, reg_addr, value);
		#endif
		return 0;
    }
    return 0;           
}

int doPhyMMDWrite(int argc, char *argv[], void *p) 	// cl45
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

	if(argc==5)
	{	// tce emiiw <DevAddr> <PhyAddr> <regAddr> <data>
		phyaddr = checked_atoi(argv[1]);//0~31
		//dev_addr = checked_atoi(argv[2]);//0~31
		sscanf(argv[2], "%lx", &dev_addr);//hex
		sscanf(argv[3], "%lx", &reg_addr);//hex
		sscanf(argv[4], "%lx", &value);//hex
    }
	else if(argc==7)
	{
		if(stricmp(argv[0], "emiiwb") == 0)
		{
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

 	if(((argc == 5) || (argc == 7)) && (phyaddr<=0x1f))
 	{
 		if(argc == 5)
 		{     	
        	#ifdef CL45_CMD_SUPPORT
			mtEMiiRegWrite(phyaddr, dev_addr,reg_addr,value);
			printf("* doPhyMMDWrite => phyaddr=%d, dev_addr=%d, data_addr=0x%04lX , value=0x%04lX\r\n", phyaddr, dev_addr, reg_addr, value);
        	#else
			tcMiiStationWrite(phyaddr, page_reg, 0x00); 						//switch to main page
			tcMiiStationWrite(phyaddr, MMD_Control_register, (0<<14)+dev_addr);
			tcMiiStationWrite(phyaddr, MMD_addr_data_register, reg_addr);
			tcMiiStationWrite(phyaddr, MMD_Control_register, (1<<14)+dev_addr);
			tcMiiStationWrite(phyaddr, MMD_addr_data_register, value);
			printf("* doPhyMMDWrite_CL22=> phyaddr=%d, dev_addr=%d, data_addr=0x%04lX , value=0x%04lX\r\n", phyaddr, dev_addr, reg_addr, value);
        	#endif
		}
 		else if(((argc == 7) && (stricmp(argv[0], "emiiwb")) == 0))
 		{
			RValue = mtEMiiRegRead(phyaddr,dev_addr, reg_addr);
			for(i=0; i<BFlen; i++)
			{
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
			mtEMiiRegWrite(phyaddr, dev_addr,reg_addr, value);
			RValue = mtEMiiRegRead(phyaddr,dev_addr, reg_addr);
			printf(" ModValue=0x%04lX\r\n", RValue);
 		}
		return 0;
	}
	else	
	{
        printf("       CMD Error : emiiw <PhyAddr> <DevAddr> <regAddr> <data>\r\n");
		printf("                         emiiwb <PhyAddr> <DevAddr> <RegAddr> <STBit> <BFLen> <BFVal> \r\n");	
        return 0;      
	}
}

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

	if(argc==5)
	{
		phyaddr = checked_atoi(argv[2]);//0~31
		if((strcasecmp(argv[1], "10M") == 0)&&((strcasecmp(argv[4], "on") == 0)||(strcasecmp(argv[4], "off") == 0)))
		{
			if(strcasecmp(argv[3], "farend") == 0)
			{
				if(strcasecmp(argv[4], "on") == 0)
				{
					tcMiiStationWrite(phyaddr, page_reg, ExtendPage);
					value = tcMiiStationRead(phyaddr, 0x1A);
					tcMiiStationWrite(phyaddr, 0x1A, value|0x8000);		//set Media_Loopback(ExtReg1Ah[15])='1'
					tcMiiStationWrite(phyaddr, page_reg, 0);			//recover page number to main page
					tcMiiStationWrite(phyaddr, 0x00, 0x0100);			//set Smi_Reg00 as Force-10F _mode , set reg00=0x0100
				}
				else
				{
					tcMiiStationWrite(phyaddr, page_reg, ExtendPage);
					value = tcMiiStationRead(phyaddr, 0x1A);
					tcMiiStationWrite(phyaddr, 0x1A, value&0x7FFF);		//set Media_Loopback(ExtReg1Ah[15])='0'
					tcMiiStationWrite(phyaddr, page_reg, 0);			//recover page number to main page
					tcMiiStationWrite(phyaddr, 0x00, 0x1040);			//set Smi_Reg00 to initial value , set reg00=0x1040(initial value) 
				}
			}
			else if(strcasecmp(argv[3], "di_part1") == 0)
			{
				if(strcasecmp(argv[4], "on") == 0)
				{
					tcMiiStationWrite(phyaddr, page_reg, 0);			//force page number to main page
					tcMiiStationWrite(phyaddr, 0x00, 0x0000);			//set Smi_Reg00 as Force-10H _mode ,no Loopback mode, no MrCollisionTestEn , set reg00=0x0000
				}
				else
				{
					tcMiiStationWrite(phyaddr, page_reg, 0);			//force page number to main page 
					tcMiiStationWrite(phyaddr, 0x00, 0x1040);			//set Smi_Reg00 to initial value , set reg00=0x1040(initial value) 
					value = tcMiiStationRead(phyaddr, 0x16);
					tcMiiStationWrite(phyaddr, 0x16, value|0x2000);		//recover EchoMode10Dis(Reg16h[13]) ='1'
				}
			}
			else if(strcasecmp(argv[3], "di_part2") == 0)
			{
				if(strcasecmp(argv[4], "on") == 0)
				{
					tcMiiStationWrite(phyaddr, page_reg, 0);			//force page number to main page
					value = tcMiiStationRead(phyaddr, 0x16);
					tcMiiStationWrite(phyaddr, 0x16, value&0xDFFF);		//set EchoMode10Dis(Reg16h[13]) ='0'
				}
				else
				{
					tcMiiStationWrite(phyaddr, page_reg, 0);			//force page number to main page
					tcMiiStationWrite(phyaddr, 0x00, 0x1040);			//set Smi_Reg00 to initial value , set reg00=0x1040(initial value) 
					value = tcMiiStationRead(phyaddr, 0x16);
					tcMiiStationWrite(phyaddr, 0x16, value|0x2000);		//recover EchoMode10Dis(Reg16h[13]) ='1'
				}
			}
			else if(strcasecmp(argv[3], "digital") == 0)
			{
				if(strcasecmp(argv[4], "on") == 0)
				{
					tcMiiStationWrite(phyaddr, page_reg, 0);			//force page number to main page
					tcMiiStationWrite(phyaddr, 0x00, 0x4100);			//set Smi_Reg00 as Force-10F _mode + Loopback mode , set reg00=0x4100
				}
				else
				{
					tcMiiStationWrite(phyaddr, page_reg, 0);			//force page number to main page 
					tcMiiStationWrite(phyaddr, 0x00, 0x1040);			//set Smi_Reg00 to initial value , set reg00=0x1040(initial value) 
				}
			}
			else if(strcasecmp(argv[3], "analog") == 0)
			{
				if(strcasecmp(argv[4], "on") == 0)
				{ 
                    value=mtEMiiRegRead(phyaddr, 0x1E, 0x14D);
					mtEMiiRegWrite(phyaddr, 0x1E, 0x14D, value|0x0080);	//set AnaLoopBack_pre(dev1eh_reg14Dh[7])='1' //CML_20130221
					value=mtEMiiRegRead(phyaddr, 0x1E, 0x33);
					mtEMiiRegWrite(phyaddr, 0x1E, 0x33, value&0xFFF8);	//set cr_pd_*_pair_sw_en(dev1eh_reg33h[2:0]) ='000'
					tcMiiStationWrite(phyaddr, 0x00, 0x0100);			//set Smi_Reg00 as Force-10F _mode , set reg00=0x0100
				}
				else
				{
                    value=mtEMiiRegRead(phyaddr, 0x1E, 0x14D);
					mtEMiiRegWrite(phyaddr, 0x1E, 0x14D, value&0xFF7F);	//set AnaLoopBack_pre(dev1eh_reg14Dh[7])='0'				  
					value=mtEMiiRegRead(phyaddr, 0x1E, 0x33);
					mtEMiiRegWrite(phyaddr, 0x1E, 0x33, value|0x0007);	//set cr_pd_*_pair_sw_en(dev1eh_reg33h[2:0]) ='111'		   
					tcMiiStationWrite(phyaddr, 0x00, 0x1040);			//set Smi_Reg00 to initial value , set reg00=0x1040(initial value) 
				}
			}
			else
			{
				printf("	 CMD Error : tce lpbk <10M> <phyaddr> <farend/di/digital/analog> <on/off>\r\n");
			}
			return 0;
		}
		else if((strcasecmp(argv[1], "100M") == 0)&&((strcasecmp(argv[4], "on") == 0)||(strcasecmp(argv[4], "off") == 0)))
		{
			if(strcasecmp(argv[3], "farend") == 0)
			{
				if(strcasecmp(argv[4], "on") == 0)
				{
					tcMiiStationWrite(phyaddr, page_reg, ExtendPage);
					value = tcMiiStationRead(phyaddr, 0x1A);
					tcMiiStationWrite(phyaddr, 0x1A, value|0x8000);		//set Media_Loopback(ExtReg1Ah[15])='1'
					tcMiiStationWrite(phyaddr, page_reg, 0);			//recover page number to main page
					tcMiiStationWrite(phyaddr, 0x00, 0x1040);			//set Smi_Reg00 to initial value , set reg00=0x1040(initial value) 
					value = tcMiiStationRead(phyaddr, 0x09);					
					tcMiiStationWrite(phyaddr, 0x09, value&0xFCFF);		//set AdvCap1000FDX(Reg09[9])=0 &AdvCap1000HDX(Reg09[8])=0 to disable 1000M ability , use AN to get 100M Full ability to remote
				}
				else
				{
					tcMiiStationWrite(phyaddr, page_reg, ExtendPage);
					value = tcMiiStationRead(phyaddr, 0x1A);					
					tcMiiStationWrite(phyaddr, 0x1A, value&0x7FFF);		//set Media_Loopback(ExtReg1Ah[15])='0'
					tcMiiStationWrite(phyaddr, page_reg, 0);			//recover page number to main page
					tcMiiStationWrite(phyaddr, 0x00, 0x1040);			//set Smi_Reg00 to initial value , set reg00=0x1040(initial value) 
					value = tcMiiStationRead(phyaddr, 0x04);					
					tcMiiStationWrite(phyaddr, 0x04, value|0x0300);		//recovery to default value,set AdvCap1000FDX(Reg09[9])=1 &AdvCap1000HDX(Reg09[8])=1
				}
			}
			else if(strcasecmp(argv[3], "digital") == 0)
			{
				if(strcasecmp(argv[4], "on") == 0)
				{
					tcMiiStationWrite(phyaddr,page_reg,0);			//force page number to main page
					tcMiiStationWrite(phyaddr,0x00,0x6100);			//set Smi_Reg00 as Force-100F _mode + Loopback mode , set reg00=0x6100
				}
				else
				{
					tcMiiStationWrite(phyaddr,page_reg,0);			//force page number to main page 
					tcMiiStationWrite(phyaddr,0x00,0x1040);			//set Smi_Reg00 to initial value , set reg00=0x1040(initial value) 
				}
			}
			else if(strcasecmp(argv[3], "analog") == 0)
			{
				if(strcasecmp(argv[4], "on") == 0)
				{ 
                    value=mtEMiiRegRead(phyaddr, 0x1E,0x14D);
					mtEMiiRegWrite(phyaddr, 0x1E, 0x14D, value|0x0080);	//set AnaLoopBack_pre(dev1eh_reg14Dh[7])='1' //CML_20130221
					value=mtEMiiRegRead(phyaddr, 0x1E,0x33);
					mtEMiiRegWrite(phyaddr, 0x1E, 0x33, value&0xFFF8);	//set cr_pd_*_pair_sw_en(dev1eh_reg33h[2:0]) ='000'
                    //token ring register PCS_01 setting
					tcMiiStationWrite(phyaddr, page_reg, Token_Ring_debug_reg);
					tcMiiStationWrite(phyaddr, Token_Ring_Control_reg, (1<<15)|(1<<13)|(Token_Ring_CH_addr_PCS<<11)|(Token_Ring_Node_addr_PCS<<7)|(0x01<<1));
					while(!((tcMiiStationRead(phyaddr, Token_Ring_Control_reg)&0x8000)==0x8000)); 	// data ready					
					value_low = tcMiiStationRead(phyaddr, Token_Ring_Low_data_reg);
					value_high = tcMiiStationRead(phyaddr, Token_Ring_High_data_reg);
					value=value_low+((value_high&0x00FF)<<15);
					value=value|0x0000C000; 							//set RxABPairSwapForce(PCS_01[15])=1 && set RxABPairSwapForceVal(PCS_01[14])=1
					value_high=(0x00FF0000&value);
					value_low=(0x0000FFFF&value);
					tcMiiStationWrite(phyaddr, page_reg, Token_Ring_debug_reg);
					tcMiiStationWrite(phyaddr, Token_Ring_Low_data_reg, value_low);
					tcMiiStationWrite(phyaddr, Token_Ring_High_data_reg, value_high);
					tcMiiStationWrite(phyaddr, Token_Ring_Control_reg, (1<<15)|(0<<13)|(Token_Ring_CH_addr_PCS<<11)|(Token_Ring_Node_addr_PCS<<7)|(0x01<<1));
					while(!((tcMiiStationRead(phyaddr, Token_Ring_Control_reg)&0x8000)==0x8000)); 	// data ready
					//token ring register PCS_01 setting end
					tcMiiStationWrite(phyaddr, page_reg, 0x00);			//V1.11
					tcMiiStationWrite(phyaddr,0x00,0x2100);				//set Smi_Reg00 as Force-100F _mode , set reg00=0x0100
				}
				else
				{
                    value = mtEMiiRegRead(phyaddr, 0x1E, 0x14D);
					mtEMiiRegWrite(phyaddr, 0x1E, 0x14D, value&0xFF7F);	//set AnaLoopBack_pre(dev1eh_reg14Dh[7])='0'				  
					value = mtEMiiRegRead(phyaddr, 0x1E, 0x33);
					mtEMiiRegWrite(phyaddr, 0x1E, 0x33, value|0x0007);	//set cr_pd_*_pair_sw_en(dev1eh_reg33h[2:0]) ='111'
					//token ring register PCS_01 setting
					tcMiiStationWrite(phyaddr, page_reg, Token_Ring_debug_reg);
					tcMiiStationWrite(phyaddr, Token_Ring_Control_reg, (1<<15)|(1<<13)|(Token_Ring_CH_addr_PCS<<11)|(Token_Ring_Node_addr_PCS<<7)|(0x01<<1));
					while(!((tcMiiStationRead(phyaddr, Token_Ring_Control_reg)&0x8000)==0x8000)); 	// data ready
					value_low = tcMiiStationRead(phyaddr, Token_Ring_Low_data_reg);
					value_high = tcMiiStationRead(phyaddr, Token_Ring_High_data_reg);
					value=value_low+((value_high&0x00FF)<<15);
					value=value&0x00FF3FFF; 							//set RxABPairSwapForce(PCS_01[15])=0 && set RxABPairSwapForceVal(PCS_01[14])=0
					value_high=(0x00FF0000&value);
					value_low=(0x0000FFFF&value);
					tcMiiStationWrite(phyaddr, page_reg, Token_Ring_debug_reg);
					tcMiiStationWrite(phyaddr, Token_Ring_Low_data_reg, value_low);
					tcMiiStationWrite(phyaddr, Token_Ring_High_data_reg, value_high);
					tcMiiStationWrite(phyaddr, Token_Ring_Control_reg, (1<<15)|(0<<13)|(Token_Ring_CH_addr_PCS<<11)|(Token_Ring_Node_addr_PCS<<7)|(0x01<<1));
					while(!((tcMiiStationRead(phyaddr, Token_Ring_Control_reg)&0x8000)==0x8000)); 	// data ready
					//token ring register PCS_01 setting end
					tcMiiStationWrite(phyaddr, page_reg, 0x00);			//V1.11
					tcMiiStationWrite(phyaddr,0x00,0x1040);				//set Smi_Reg00 to initial value , set reg00=0x1040(initial value)
				}
			}
			else
			{
				printf("     CMD Error : tce lpbk <100M> <phyaddr> <farend/digital/analog> <on/off>\r\n");
			}
			return 0;
		}
		else if((strcasecmp(argv[1], "1000M") == 0)&&((strcasecmp(argv[4], "on") == 0)||(strcasecmp(argv[4], "off") == 0)))
		{
			if(strcasecmp(argv[3], "farend") == 0)
			{
				if(strcasecmp(argv[4], "on") == 0)
				{
					tcMiiStationWrite(phyaddr,page_reg,ExtendPage);
					value=tcMiiStationRead(phyaddr,0x1A);
					tcMiiStationWrite(phyaddr,0x1A,value|0x8000);		//set Media_Loopback(ExtReg1Ah[15])='1'
					tcMiiStationWrite(phyaddr,page_reg,0);				//recover page number to main page 
					tcMiiStationWrite(phyaddr,0x00,0x1040);				//set Smi_Reg00 to initial value , set reg00=0x1040(initial value)  
					tcMiiStationWrite(phyaddr,0x09,0x0700);				//set smi_Reg09 to initial value , set reg09=0x0700(initial value) 
				}
				else
				{
					tcMiiStationWrite(phyaddr,page_reg,ExtendPage);
					value=tcMiiStationRead(phyaddr,0x1A);
					tcMiiStationWrite(phyaddr,0x1A,value&0x7FFF);		//set Media_Loopback(ExtReg1Ah[15])='0'
					tcMiiStationWrite(phyaddr,page_reg,0);				//recover page number to main page
					tcMiiStationWrite(phyaddr,0x00,0x1040);				//set Smi_Reg00 to initial value , set reg00=0x1040(initial value) 
					tcMiiStationWrite(phyaddr,0x09,0x0700);				//set smi_Reg09 to initial value , set reg09=0x0700(initial value)
				}
			}
			else if(strcasecmp(argv[3], "digital_lpbk") == 0)
			{
				if(strcasecmp(argv[4], "on") == 0)
				{
					tcMiiStationWrite(phyaddr,page_reg,0);				//force page number to main page
					tcMiiStationWrite(phyaddr,0x00,0x4140);				//set Smi_Reg00 as Force-1000F _mode + Loopback mode , set reg00=0x4140
				}
				else
				{
					tcMiiStationWrite(phyaddr,page_reg,0);				//force page number to main page
					tcMiiStationWrite(phyaddr,0x00,0x1040);				//set Smi_Reg00 to initial value , set reg00=0x1040(initial value) 
				}
			}
			else if(strcasecmp(argv[3], "analog") == 0)
			{
				if(strcasecmp(argv[4], "on") == 0)
				{
                    value=mtEMiiRegRead(phyaddr, 0x1E,0x14D);
					mtEMiiRegWrite(phyaddr, 0x1E, 0x14D, value|0x0080);	//set AnaLoopBack_pre(dev1eh_reg14Dh[7])='1' //CML_20130221
					value=tcMiiStationRead(phyaddr,0x18);
					tcMiiStationWrite(phyaddr,0x18,value|0x0001);		//set CableLoopback_pre(Reg18h)=1 to enable 1000M cable loop back
					value=mtEMiiRegRead(phyaddr, 0x1E,0x33);
					mtEMiiRegWrite(phyaddr, 0x1E, 0x33, value&0xFFF8);	//set cr_pd_*_pair_sw_en(dev1eh_reg33h[2:0]) ='000'
					//token ring register PCS_01 setting
					tcMiiStationWrite(phyaddr, page_reg, Token_Ring_debug_reg);
					tcMiiStationWrite(phyaddr, Token_Ring_Control_reg, (1<<15)|(1<<13)|(Token_Ring_CH_addr_PCS<<11)|(Token_Ring_Node_addr_PCS<<7)|(0x01<<1));
					while(!((tcMiiStationRead(phyaddr, Token_Ring_Control_reg)&0x8000)==0x8000)); // data ready
					value_low = tcMiiStationRead(phyaddr, Token_Ring_Low_data_reg);
					value_high = tcMiiStationRead(phyaddr, Token_Ring_High_data_reg);
					value=value_low+((value_high&0x00FF)<<15);
					value=value|0x00003C00; 						//set TxCDPairSwapForce(PCS_01[13])=1 && set TxCDPairSwapForceVal(PCS_01[12])=1 && set TxABPairSwapForce(PCS_01[11])=1 && set TxABPairSwapForceVal(PCS_01[10])=1
					value_high=(0x00FF0000&value);
					value_low=(0x0000FFFF&value);
					tcMiiStationWrite(phyaddr, page_reg, Token_Ring_debug_reg);
					tcMiiStationWrite(phyaddr, Token_Ring_Low_data_reg, value_low);
					tcMiiStationWrite(phyaddr, Token_Ring_High_data_reg, value_high);
					tcMiiStationWrite(phyaddr, Token_Ring_Control_reg, (1<<15)|(0<<13)|(Token_Ring_CH_addr_PCS<<11)|(Token_Ring_Node_addr_PCS<<7)|(0x01<<1));
					while(!((tcMiiStationRead(phyaddr, Token_Ring_Control_reg)&0x8000)==0x8000)); // data ready
					//token ring register PCS_01 setting end
					//token ring register ECNC_C1 setting
					tcMiiStationWrite(phyaddr, page_reg, Token_Ring_debug_reg);
					tcMiiStationWrite(phyaddr, Token_Ring_Control_reg, (1<<15)|(1<<13)|(0x0<<11)|(0x3<<7)|(0x01<<1));
					while(!((tcMiiStationRead(phyaddr, Token_Ring_Control_reg)&0x8000)==0x8000)); // data ready
					value_low = tcMiiStationRead(phyaddr, Token_Ring_Low_data_reg);
					value_high = tcMiiStationRead(phyaddr, Token_Ring_High_data_reg);
					value=value_low+((value_high&0x00FF)<<15);
					value=value|0x00000041; 						//set EcAForceGainswapdisab(ECNC_C1[6])=1 && set ECForceUpdateDis(ECNC_C1[0])=1
					value_high=(0x00FF0000&value);
					value_low=(0x0000FFFF&value);
					tcMiiStationWrite(phyaddr, page_reg, Token_Ring_debug_reg);
					tcMiiStationWrite(phyaddr, Token_Ring_Low_data_reg, value_low);
					tcMiiStationWrite(phyaddr, Token_Ring_High_data_reg, value_high);
					tcMiiStationWrite(phyaddr, Token_Ring_Control_reg, (1<<15)|(0<<13)|(0x0<<11)|(0x3<<7)|(0x01<<1));
					while(!((tcMiiStationRead(phyaddr, Token_Ring_Control_reg)&0x8000)==0x8000)); // data ready
					//token ring register PCS_01 setting end
					tcMiiStationWrite(phyaddr, page_reg, 0x00);		//V1.11
					tcMiiStationWrite(phyaddr,0x00,0x0140);			//set Smi_Reg00 as Force-1000F _mode , set reg00=0x0140
				}
				else
				{
                    value=mtEMiiRegRead(phyaddr, 0x1E,0x14D);
					mtEMiiRegWrite(phyaddr, 0x1E, 0x14D, value&0xFF7F);		//set AnaLoopBack_pre(dev1eh_reg14Dh[7])='0'
					value=tcMiiStationRead(phyaddr,0x18);			
					tcMiiStationWrite(phyaddr,0x18,value&0xFFFE);			//recovery to default,set CableLoopback_pre(Reg18h)=0 to disable 1000M cable loop back		  
					value=mtEMiiRegRead(phyaddr, 0x1E,0x33);
					mtEMiiRegWrite(phyaddr, 0x1E, 0x33, value|0x0007);		//recovery to default,set cr_pd_*_pair_sw_en(dev1eh_reg33h[2:0]) ='111'
					//token ring register PCS_01 setting
					tcMiiStationWrite(phyaddr, page_reg, Token_Ring_debug_reg);
					tcMiiStationWrite(phyaddr, Token_Ring_Control_reg, (1<<15)|(1<<13)|(Token_Ring_CH_addr_PCS<<11)|(Token_Ring_Node_addr_PCS<<7)|(0x01<<1));
					while(!((tcMiiStationRead(phyaddr, Token_Ring_Control_reg)&0x8000)==0x8000)); // data ready
					value_low = tcMiiStationRead(phyaddr, Token_Ring_Low_data_reg);
					value_high = tcMiiStationRead(phyaddr, Token_Ring_High_data_reg);
					value=value_low+((value_high&0x00FF)<<15);
					value=value&0x00FFC3FF; 					//recovery to default,set TxCDPairSwapForce(PCS_01[13])=0 && set TxCDPairSwapForceVal(PCS_01[12])=0 && set TxABPairSwapForce(PCS_01[11])=0 && set TxABPairSwapForceVal(PCS_01[10])=0
					value_high=(0x00FF0000&value);
					value_low=(0x0000FFFF&value);
					tcMiiStationWrite(phyaddr, page_reg, Token_Ring_debug_reg);
					tcMiiStationWrite(phyaddr, Token_Ring_Low_data_reg, value_low);
					tcMiiStationWrite(phyaddr, Token_Ring_High_data_reg, value_high);
					tcMiiStationWrite(phyaddr, Token_Ring_Control_reg, (1<<15)|(0<<13)|(Token_Ring_CH_addr_PCS<<11)|(Token_Ring_Node_addr_PCS<<7)|(0x01<<1));
					while(!((tcMiiStationRead(phyaddr, Token_Ring_Control_reg)&0x8000)==0x8000)); // data ready
					tcMiiStationWrite(phyaddr, page_reg, Token_Ring_debug_reg);
					tcMiiStationWrite(phyaddr, Token_Ring_Control_reg, (1<<15)|(1<<13)|(0x0<<11)|(0x3<<7)|(0x01<<1));
					while(!((tcMiiStationRead(phyaddr, Token_Ring_Control_reg)&0x8000)==0x8000)); // data ready
					value_low = tcMiiStationRead(phyaddr, Token_Ring_Low_data_reg);
					value_high = tcMiiStationRead(phyaddr, Token_Ring_High_data_reg);
					value=value_low+((value_high&0x00FF)<<15);
					value=value&0xFFFFFFBE; 					//recovery to default,set EcAForceGainswapdisab(ECNC_C1[6])=0 && set ECForceUpdateDis(ECNC_C1[0])=0
					value_high=(0x00FF0000&value);
					value_low=(0x0000FFFF&value);
					tcMiiStationWrite(phyaddr, page_reg, Token_Ring_debug_reg);
					tcMiiStationWrite(phyaddr, Token_Ring_Low_data_reg, value_low);
					tcMiiStationWrite(phyaddr, Token_Ring_High_data_reg, value_high);
					tcMiiStationWrite(phyaddr, Token_Ring_Control_reg, (1<<15)|(0<<13)|(0x0<<11)|(0x3<<7)|(0x01<<1));
					while(!((tcMiiStationRead(phyaddr, Token_Ring_Control_reg)&0x8000)==0x8000)); // data ready
					tcMiiStationWrite(phyaddr, page_reg, 0x00);		//V1.11		  
					tcMiiStationWrite(phyaddr,0x00,0x1040);				//set Smi_Reg00 to initial value , set reg00=0x1040(initial value) 
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
  	if((argc != 7) && (argc != 4))
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
    	if((strcasecmp("norm",argv[1]) == 0)) 		//GbE Normal Mode
    	{
      		BaseAddr = 0x200;
    	}
    	else if((strcasecmp("test",argv[1]) == 0))	//GbE Test Mode
    	{
      		BaseAddr = 0x200;  						///V2.00 change 0x232 to 0x200, E2 merge normal and test mode
    	}
    	else
    	{
      		printf("Arg[1] Error: {norm|test}\n");
    	}

    	if(strcasecmp("P2",argv[2]) == 0)
    	{
      		TuneAddr1 = 0;
    	}
    	else if(strcasecmp("P1",argv[2]) == 0)
    	{
      		TuneAddr1 = 10;
    	}
    	else if(strcasecmp("Z0",argv[2]) == 0)
    	{
      		TuneAddr1 = 20;
    	}
    	else if(strcasecmp("N1",argv[2]) == 0)
    	{
      		TuneAddr1 = 30;
    	}
    	else if(strcasecmp("N2",argv[2]) == 0)
    	{
      		TuneAddr1 = 40;
    	}
    	else
    	{
      		printf("Arg[2] Error: {P2|P1|Z0|N1|N2}\n");
    	}

    	if(strcasecmp("P2",argv[3]) == 0)
    	{
      		TuneAddr2 = 0;
    	}
    	else if(strcasecmp("P1",argv[3]) == 0)
    	{
      		TuneAddr2 = 2;
    	}
    	else if(strcasecmp("Z0",argv[3]) == 0)
    	{
      		TuneAddr2 = 4;
    	}
    	else if(strcasecmp("N1",argv[3]) == 0)
    	{
      		TuneAddr2 = 6;
    	}
    	else if(strcasecmp("N2",argv[3]) == 0)
    	{
      		TuneAddr2 = 8;
    	}
    	else
    	{
      		printf("Arg[3] Error: {P2|P1|Z0|N1|N2}\n");
    	}

    	if(argc == 7) //Set corresponding register
    	{
      		sscanf(argv[4], "%lx", &wTmp1);
      		sscanf(argv[5], "%lx", &wTmp2);
      		sscanf(argv[6], "%lx", &wTmp3);
      		if((wTmp1 > 0x3F) || (wTmp2 > 0x3F) || (wTmp2 > 0x3F))
      		{
        		printf("Arg[4~6] Error: value should be 6bit digit\n");
      		}
      		else
      		{
        		wData = (wTmp2 << 8) + wTmp1;
        
        		TarAddr = BaseAddr + TuneAddr1 + TuneAddr2;
        		mtEMiiRegWrite(0x00, 0x1F, TarAddr, wData);
        		mtEMiiRegWrite(0x00, 0x1F, TarAddr+1, wTmp3);
        		printf("Write(dev1Fh_reg%Xh) = %X\n", TarAddr, wData); 		//debug
        		printf("Write(dev1Fh_reg%Xh) = %X\n", TarAddr+1, wTmp3); 	//debug
      		}
    	}
    	else  //Get corresponding setting
    	{
        	TarAddr = BaseAddr + TuneAddr1 + TuneAddr2;
        	wData = mtEMiiRegRead(0x00, 0x1F, TarAddr);
        	wTmp1 = wData & 0x003F;
        	wTmp2 = (wData & 0x3F00) >> 8;
        	printf("Read(dev1Fh_reg%Xh) = %X\n", TarAddr, wData);
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
  	uint32 wData = 0;
  	uint32 wTmp1 = 0, wTmp2 = 0, wTmp3 = 0;
  	uint32 iPort = 0;
  
  	if((argc != 7) && (argc != 4))
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
  	else if(argc == 7)  //Set Mid, Ovs, Ovp
  	{
    	sscanf(argv[1], "%lx", &iPort);
    	sscanf(argv[4], "%lx", &wTmp1);
    	sscanf(argv[5], "%lx", &wTmp2);
    	sscanf(argv[6], "%lx", &wTmp3);
    	if((iPort > 4) || (iPort < 0))
      		printf("Port Number Error: {0~4}");
    	else if((wTmp1 > 0x3FF) || (wTmp2 > 0x3FF) || (wTmp2 > 0x3FF))
      		printf("Arg[4~6] Error: value should be 10bit digit\n");
    	else
    	{
      		if((strcasecmp("0",argv[2])==0) && (strcasecmp("1",argv[3])==0))
      		{
        		wData = mtEMiiRegRead(iPort, 0x1E, 0x00);
        		wData = (wData & 0x1000) | wTmp1;
        		mtEMiiRegWrite(iPort, 0x1E, 0x00, wData);
        		mtEMiiRegWrite(iPort, 0x1E, 0x01, wTmp2);
        		mtEMiiRegWrite(iPort, 0x1E, 0x02, wTmp3);
        		printf("Set Port%d 0 to 1: %X %X %X\n", iPort, wData, wTmp2, wTmp3);
      		}
      		else if((strcasecmp("1",argv[2])==0) && (strcasecmp("0",argv[3])==0))
      		{
        		mtEMiiRegWrite(iPort, 0x1E, 0x03, wTmp1);
        		mtEMiiRegWrite(iPort, 0x1E, 0x04, wTmp2);
        		mtEMiiRegWrite(iPort, 0x1E, 0x05, wTmp3);
        		printf("Set Port%d 1 to 0: %X %X %X\n", iPort, wTmp1, wTmp2, wTmp3);
      		}
      		else if((strcasecmp("0",argv[2])==0) && (strcasecmp("-1",argv[3])==0))
      		{
        		mtEMiiRegWrite(iPort, 0x1E, 0x06, wTmp1);
        		mtEMiiRegWrite(iPort, 0x1E, 0x07, wTmp2);
        		mtEMiiRegWrite(iPort, 0x1E, 0x08, wTmp3);
        		printf("Set Port%d 0 to -1: %X %X %X\n", iPort, wTmp1, wTmp2, wTmp3);
      		}
      		else if((strcasecmp("-1",argv[2])==0) && (strcasecmp("0",argv[3])==0))
      		{
        		mtEMiiRegWrite(iPort, 0x1E, 0x09, wTmp1);
        		mtEMiiRegWrite(iPort, 0x1E, 0x0A, wTmp2);
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
    	if((iPort > 4) || (iPort < 0))
      		printf("Port Number Error: {0~4}");
    	else
    	{
      		if((strcasecmp("0",argv[2])==0) && (strcasecmp("1",argv[3])==0))
      		{
        		wData = mtEMiiRegRead(iPort, 0x1E, 0x00);
        		wData = wData & 0x3FF;
        		printf("Mid (dev1Eh_reg000h[9:0]) = %X\n", wData);
        		wData = mtEMiiRegRead(iPort, 0x1E, 0x01);
        		wData = wData & 0x3FF;
        		printf("Ovs (dev1Eh_reg001h[9:0]) = %X\n", wData);
        		wData = mtEMiiRegRead(iPort, 0x1E, 0x02);
        		wData = wData & 0x3FF;
        		printf("Ovp (dev1Eh_reg002h[9:0]) = %X\n", wData);
      		}
      		else if((strcasecmp("1",argv[2])==0) && (strcasecmp("0",argv[3])==0))
      		{
        		wData = mtEMiiRegRead(iPort, 0x1E, 0x03);
        		wData = wData & 0x3FF;
        		printf("Mid (dev1Eh_reg003h[9:0]) = %X\n", wData);
        		wData = mtEMiiRegRead(iPort, 0x1E, 0x04);
        		wData = wData & 0x3FF;
        		printf("Ovs (dev1Eh_reg004h[9:0]) = %X\n", wData);
        		wData = mtEMiiRegRead(iPort, 0x1E, 0x05);
        		wData = wData & 0x3FF;
        		printf("Ovp (dev1Eh_reg005h[9:0]) = %X\n", wData);
      		}
      		else if((strcasecmp("0",argv[2])==0) && (strcasecmp("-1",argv[3])==0))
      		{
        		wData = mtEMiiRegRead(iPort, 0x1E, 0x06);
        		wData = wData & 0x3FF;
        		printf("Mid (dev1Eh_reg006h[9:0]) = %X\n", wData);
        		wData = mtEMiiRegRead(iPort, 0x1E, 0x07);
        		wData = wData & 0x3FF;
        		printf("Ovs (dev1Eh_reg007h[9:0]) = %X\n", wData);
        		wData = mtEMiiRegRead(iPort, 0x1E, 0x08);
        		wData = wData & 0x3FF;
        		printf("Ovp (dev1Eh_reg008h[9:0]) = %X\n", wData);
      		}
      		else if((strcasecmp("-1",argv[2])==0) && (strcasecmp("0",argv[3])==0))
      		{
        		wData = mtEMiiRegRead(iPort, 0x1E, 0x09);
        		wData = wData & 0x3FF;
        		printf("Mid (dev1Eh_reg009h[9:0]) = %X\n", wData);
        		wData = mtEMiiRegRead(iPort, 0x1E, 0x0A);
        		wData = wData & 0x3FF;
        		printf("Ovs (dev1Eh_reg00Ah[9:0]) = %X\n", wData);
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
	
    if(argc==2)
	{
    	// tce phycnt <PhyAddr> 
    	phyaddr = checked_atoi(argv[1]);		//0~31
    	tcMiiStationWrite(phyaddr, page_reg, page_Addr);
    	PktCnt = tcMiiStationRead(phyaddr, PktCnt_Addr);
    	if(PktCnt & 0x8000)
		{
    		PktCnt = PktCnt & 0x7FFF;
    	}
    	else
		{
    	    printf("Invalid Pkcnt !, value=%d",PktCnt);
    	    return 0;
    	}
        CrcErrCnt = tcMiiStationRead(phyaddr, CrcErrcnt_Addr) & 0x00FF;
        printf(" PktCnt= %d, CrcErrCnt = %d \r\n", PktCnt, CrcErrCnt); 
    }
    else
	{
        printf("       CMD Error : phycnt <PhyAddr>\r\n");
        return 0;      
    }
    return 0;
}
#else
// called by macPeriodCheck() in tc3162l2mac.c
#endif
#endif

/************************************************************************
*                       END
*************************************************************************/
#define ANACAL_INIT			0x01
#define ANACAL_ERROR		0xFD
#define ANACAL_SATURATION	0xFE
#define	ANACAL_FINISH		0xFF

#define ANACAL_PAIR_A		0
#define ANACAL_PAIR_B		1
#define ANACAL_PAIR_C		2
#define ANACAL_PAIR_D		3

#define DAC_IN_0V					0x000
#define DAC_IN_2V					0x0f0	// +/-1V
#define TX_AMP_OFFSET_0mV			0
#define TX_AMP_OFFSET_VALID_BITS	6

#define FE_CAL_P0			0

const uint8	ZCAL_TO_R50ohm_TBL[64] =
{
	127,	127,	124,	120,	117,	114,	112,	109,	106,	104,	102,	99,		97,		79,		77,		75,
	73,		72,		70,		68,		66,		65,		47,		46,		44,		43,		42,		40,		39,		38,		37,		36,
	//34,		33,		32,		15,		14,		13,		12,		11,		10,		10,		9,		8,		7,		6,		5,		5,
	//34,		33,		32,		20,		20,		20,		20,		20,		10,		10,		9,		8,		7,		6,		5,		5,
	34,		33,		32,		26,		26,		20,		20,		20,		10,		10,		9,		8,		7,		6,		5,		5, // BGA
	4,		3,		2,		2,		1,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0
#if 0 // mog_review
	127,	127, 	127, 	127, 	127, 	127, 	127, 	127, 	127, 	127, 	127, 	127, 	127, 	127, 	127, 	127, 
  	123, 	122, 	117, 	115, 	112, 	103, 	100,  	98,  	87,  	85,  	83,  	81,  	72,  	70,  	68,  	66, 
   	64,  	55,  	53,  	52,  	50,  	49,  	48,  	38,  	36,  	35,  	34,  	33,  	32,  	22,  	21,  	20,
   	19,  	18,  	17,  	16,   	7,   	6,   	5,   	4,   	3,   	2,   	1,   	0,   	0,   	0,   	0,   	0
#endif 
};

const uint8	ZCAL_TO_R50ohm_TBL_100[64] =
{
	127,	127,	127,	127,	127,	127,	126,	123,	120,	117,	114,	112,	110,	107,	105,	103, 
	101,	99,		97,		79,		77,		75,		74,		72,		70,		69,		67,		66,		65,		47,		46,		45,
	43,		42,		41,		40,		39,		38,		37,		36,		34,		34,		33,		32,		15,		14,		13,		12,
	11,		10,		10,		9,		8,		7,		7,		6,		5,		4,		4,		3,		2,		2,		1,		1		
#if 0 // 2nd	
	127,	127,	124,	120,	117,	114,	112,	109,	106,	104,	102,	99,		97,		79,		77,		75,
	//73,		72,		70,		68,		66,		65,		47,		46,		44,		43,		42,		40,		39,		38,		37,		36,
	73,		72,		70,		68,		66,		65,		47,		44,		44,		44,		44,		44,		44,		38,		37,		36,
	34,		33,		32,		15,		14,		13,		12,		11,		10,		10,		9,		8,		7,		6,		5,		5,
	4,		3,		2,		2,		1,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0
#endif
#if 0 // mog_review // 1st
	127,	127, 	127, 	127, 	127, 	127, 	127, 	127, 	127, 	127, 	127, 	127, 	127, 	127, 	127, 	127, 
  	123, 	122, 	117, 	115, 	112, 	103, 	100,  	98,  	87,  	85,  	83,  	81,  	72,  	70,  	68,  	66, 
   	64,  	55,  	53,  	52,  	50,  	49,  	48,  	38,  	36,  	35,  	34,  	33,  	32,  	22,  	21,  	20,
   	19,  	18,  	17,  	16,   	7,   	6,   	5,   	4,   	3,   	2,   	1,   	0,   	0,   	0,   	0,   	0
#endif 
};


// ************************************************************************
// 		Tx calibration use the Software flow : Rext, R50, Tx offset, Tx amp
//		Rx calibration use the Hardware flow
// ************************************************************************
#define   regWriteWord(addr,val)      ( *(volatile unsigned int *)(addr)=val )
#define   regReadWord(addr,val) 	  ( val=*(volatile unsigned int *)(addr) )

void doGePhyALLAnalogCal(uint8 phyaddr) // for EN7512 GBE 
{
	uint8	rg_zcal_ctrl, all_ana_cal_status;
	uint16	ad_cal_comp_out_init;
	uint16	dev1e_e0_ana_cal_r5, reg0_temp, dev1e_145_temp; 
	int 	calibration_polarity, tx_offset_temp;
	uint16	calibration_pair, cal_temp;
	uint8	tx_offset_reg_shift, tx_amp_reg_shift; 
	uint16	tx_offset_reg, reg_temp;
	uint8	tx_amp_temp, tx_amp_reg, cnt=0, tx_amp_reg_100;

	tcMiiStationWrite(phyaddr, 0x1f, 0x0000);		// g0 
	reg0_temp = tcMiiStationRead(phyaddr, 0x0);		// keep the default value
	tcMiiStationWrite(phyaddr, 0x0,  0x0140);		// set [12]AN disable, [8]full duplex, [13/6]1000Mbps

	mtEMiiRegWrite(phyaddr, 0x1f, 0x0100, 0xc000);			// BG voltage output
	dev1e_145_temp = mtEMiiRegRead(phyaddr, 0x1e, 0x0145);
	mtEMiiRegWrite(phyaddr, 0x1e, 0x0145, 0x1010);			// fix mdi
	mtEMiiRegWrite(phyaddr, 0x1e, 0x0185, 0x0000);			// disable tx slew control
	
	// calibration start ======================================================================================
	while(GECal_flag == 0)
		GECal_Rext(phyaddr, 100);

	// *** R50 Cal start *******************************************************************************
	mtEMiiRegWrite(phyaddr, 0x1e, 0x00db, 0x1100);	// 1e_db[12]:rg_cal_ckinv, [8]:rg_ana_calen, [4]:rg_rext_calen, [0]:rg_zcalen_a
	mtEMiiRegWrite(phyaddr, 0x1e, 0x00dc, 0x0000);	// 1e_dc[0]:rg_txvos_calen
	for(calibration_pair = ANACAL_PAIR_A; calibration_pair <= ANACAL_PAIR_D; calibration_pair ++)
	{
		rg_zcal_ctrl = 0x20;  						// start with 0 dB
		dev1e_e0_ana_cal_r5 = (mtEMiiRegRead(phyaddr, 0x1e, 0x00e0) & (~0x003f));
		mtEMiiRegWrite(phyaddr, 0x1e, 0x00e0, (dev1e_e0_ana_cal_r5 | rg_zcal_ctrl));	// 1e_e0[5:0]:rg_zcal_ctrl
		if(calibration_pair == ANACAL_PAIR_A)
		{
			mtEMiiRegWrite(phyaddr, 0x1e, 0x00db, 0x1101);	// 1e_db[12]:rg_cal_ckinv, [8]:rg_ana_calen, [4]:rg_rext_calen, [0]:rg_zcalen_a
			mtEMiiRegWrite(phyaddr, 0x1e, 0x00dc, 0x0000);	
		}
		else if(calibration_pair == ANACAL_PAIR_B)
		{
			mtEMiiRegWrite(phyaddr, 0x1e, 0x00db, 0x1100);	// 1e_db[12]:rg_cal_ckinv, [8]:rg_ana_calen, [4]:rg_rext_calen, [0]:rg_zcalen_a
			mtEMiiRegWrite(phyaddr, 0x1e, 0x00dc, 0x1000);	// 1e_dc[12]:rg_zcalen_b
		}
		else if(calibration_pair == ANACAL_PAIR_C)
		{
			mtEMiiRegWrite(phyaddr, 0x1e, 0x00db, 0x1100);	// 1e_db[12]:rg_cal_ckinv, [8]:rg_ana_calen, [4]:rg_rext_calen, [0]:rg_zcalen_a
			mtEMiiRegWrite(phyaddr, 0x1e, 0x00dc, 0x0100);	// 1e_dc[8]:rg_zcalen_c
		}
		else // if(calibration_pair == ANACAL_PAIR_D)
		{
			mtEMiiRegWrite(phyaddr, 0x1e, 0x00db, 0x1100);	// 1e_db[12]:rg_cal_ckinv, [8]:rg_ana_calen, [4]:rg_rext_calen, [0]:rg_zcalen_a
			mtEMiiRegWrite(phyaddr, 0x1e, 0x00dc, 0x0010);	// 1e_dc[4]:rg_zcalen_d
		}

		all_ana_cal_status = allGeAnaCalWait(20, phyaddr); // delay 20 usec
		if(all_ana_cal_status == 0)
		{
			all_ana_cal_status = ANACAL_ERROR;	
			printf("  GE R50 AnaCal ERROR!   \r\n");
		}
	
		ad_cal_comp_out_init = (mtEMiiRegRead(phyaddr, 0x1e, 0x017a)>>8) & 0x1;		// 1e_17a[8]:ad_cal_comp_out	
		if(ad_cal_comp_out_init == 1)
		{
			calibration_polarity = -1;
		}
		else
		{
			calibration_polarity = 1;
		}

		cnt = 0;
		while(all_ana_cal_status < ANACAL_ERROR)
		{
			cnt ++;
			rg_zcal_ctrl += calibration_polarity;
			mtEMiiRegWrite(phyaddr, 0x1e, 0x00e0, (dev1e_e0_ana_cal_r5 | rg_zcal_ctrl));
			all_ana_cal_status = allGeAnaCalWait(20, phyaddr); // delay 20 usec

			if(all_ana_cal_status == 0)
			{
				all_ana_cal_status = ANACAL_ERROR;	
				printf("  GE R50 AnaCal ERROR!   \r\n");
			}
			else if(((mtEMiiRegRead(phyaddr, 0x1e, 0x017a)>>8)&0x1) != ad_cal_comp_out_init) 
			{
				all_ana_cal_status = ANACAL_FINISH;	
			}
			else
			{
				if((rg_zcal_ctrl == 0x3F)||(rg_zcal_ctrl == 0x00))	
				{
					all_ana_cal_status = ANACAL_SATURATION;  // need to FT
					printf(" GE R50 AnaCal Saturation!  \r\n");
				}
			}
		}
		
		if(all_ana_cal_status == ANACAL_ERROR)
		{	
			rg_zcal_ctrl = 0x20;  // 0 dB
			//mtEMiiRegWrite(phyaddr, 0x1e, 0x00e0, (dev1e_e0_ana_cal_r5 | rg_zcal_ctrl));
		}
		else
		{
			rg_zcal_ctrl = ZCAL_TO_R50ohm_TBL_100[rg_zcal_ctrl]-2;	// wait Mog zcal/r50 mapping table
			printf("  GE R50 AnaCal Done! (%d) (0x%x)(0x%x) \r\n", cnt, rg_zcal_ctrl, (rg_zcal_ctrl|0x80));
		}
		
		if(calibration_pair == ANACAL_PAIR_A)  
		{
			ad_cal_comp_out_init = mtEMiiRegRead(phyaddr, 0x1e, 0x0174) & (~0x7f00);
			//ad_cal_comp_out_init = mtEMiiRegRead(phyaddr, 0x1e, 0x0174);
			//printf(" GE-a 1e_174(0x%x)(0x%x), 1e_175(0x%x)  \r\n", mtEMiiRegRead(phyaddr, 0x1e, 0x0174), ad_cal_comp_out_init, mtEMiiRegRead(phyaddr, 0x1e, 0x0175));
			mtEMiiRegWrite(phyaddr, 0x1e, 0x0174, (ad_cal_comp_out_init | (((rg_zcal_ctrl<<8)&0xff00) | 0x8000)));	// 1e_174[15:8]
			//printf(" GE-a 1e_174(0x%x), 1e_175(0x%x)  \r\n", mtEMiiRegRead(phyaddr, 0x1e, 0x0174), mtEMiiRegRead(phyaddr, 0x1e, 0x0175));
		}
		else if(calibration_pair == ANACAL_PAIR_B)
		{
			ad_cal_comp_out_init = mtEMiiRegRead(phyaddr, 0x1e, 0x0174) & (~0x007f);
			//ad_cal_comp_out_init = mtEMiiRegRead(phyaddr, 0x1e, 0x0174);
			//printf(" GE-b 1e_174(0x%x)(0x%x), 1e_175(0x%x)  \r\n", mtEMiiRegRead(phyaddr, 0x1e, 0x0174), ad_cal_comp_out_init, mtEMiiRegRead(phyaddr, 0x1e, 0x0175));
			
			mtEMiiRegWrite(phyaddr, 0x1e, 0x0174, (ad_cal_comp_out_init | (((rg_zcal_ctrl<<0)&0x00ff) | 0x0080)));	// 1e_174[7:0]
			//printf(" GE-b 1e_174(0x%x), 1e_175(0x%x)  \r\n", mtEMiiRegRead(phyaddr, 0x1e, 0x0174), mtEMiiRegRead(phyaddr, 0x1e, 0x0175));
		}
		else if(calibration_pair == ANACAL_PAIR_C)
		{
			ad_cal_comp_out_init = mtEMiiRegRead(phyaddr, 0x1e, 0x0175) & (~0x7f00);
			//ad_cal_comp_out_init = mtEMiiRegRead(phyaddr, 0x1e, 0x0175);
			mtEMiiRegWrite(phyaddr, 0x1e, 0x0175, (ad_cal_comp_out_init | (((rg_zcal_ctrl<<8)&0xff00) | 0x8000)));	// 1e_175[15:8]
			//printf(" GE-c 1e_174(0x%x), 1e_175(0x%x)  \r\n", mtEMiiRegRead(phyaddr, 0x1e, 0x0174), mtEMiiRegRead(phyaddr, 0x1e, 0x0175));
		}
		else // if(calibration_pair == ANACAL_PAIR_D)
		{
			ad_cal_comp_out_init = mtEMiiRegRead(phyaddr, 0x1e, 0x0175) & (~0x007f);
			//ad_cal_comp_out_init = mtEMiiRegRead(phyaddr, 0x1e, 0x0175);
			mtEMiiRegWrite(phyaddr, 0x1e, 0x0175, (ad_cal_comp_out_init | (((rg_zcal_ctrl<<0)&0x00ff) | 0x0080)));	// 1e_175[7:0]
			//printf(" GE-d 1e_174(0x%x), 1e_175(0x%x)  \r\n", mtEMiiRegRead(phyaddr, 0x1e, 0x0174), mtEMiiRegRead(phyaddr, 0x1e, 0x0175));
		}
		//mtEMiiRegWrite(phyaddr, 0x1e, 0x00e0, ((rg_zcal_ctrl<<8)|rg_zcal_ctrl));
	}
	

	mtEMiiRegWrite(phyaddr, 0x1e, 0x00db, 0x0000);
	// *** R50 Cal end ***

	// *** Tx offset Cal start *************************************************************************
	mtEMiiRegWrite(phyaddr, 0x1e, 0x00db, 0x0100);	// 1e_db[12]:rg_cal_ckinv, [8]:rg_ana_calen, [4]:rg_rext_calen, [0]:rg_zcalen_a
	mtEMiiRegWrite(phyaddr, 0x1e, 0x00dc, 0x0001);	// 1e_dc[0]:rg_txvos_calen
	mtEMiiRegWrite(phyaddr, 0x1e, 0x0096, 0x8000);	// 1e_96[15]:bypass_tx_offset_cal, Hw bypass, Fw cal
	mtEMiiRegWrite(phyaddr, 0x1e, 0x003e, 0xf808);	// 1e_3e
	
	for(calibration_pair = ANACAL_PAIR_A; calibration_pair <= ANACAL_PAIR_D; calibration_pair ++)
	{
		tx_offset_temp = TX_AMP_OFFSET_0mV;
		
		if(calibration_pair == ANACAL_PAIR_A)
		{
			mtEMiiRegWrite(phyaddr, 0x1e, 0x00dd, 0x1000);				// 1e_dd[12]:rg_txg_calen_a
			mtEMiiRegWrite(phyaddr, 0x1e, 0x017d, (0x8000|DAC_IN_0V));	// 1e_17d:dac_in0_a
			mtEMiiRegWrite(phyaddr, 0x1e, 0x0181, (0x8000|DAC_IN_0V));	// 1e_181:dac_in1_a
			reg_temp = (mtEMiiRegRead(phyaddr, 0x1e, 0x0172) & (~0x3f00));
			tx_offset_reg_shift = 8;									// 1e_172[13:8]
			tx_offset_reg = 0x0172;
			//mtEMiiRegWrite(phyaddr, 0x1e, tx_offset_reg, (reg_temp|(tx_offset_temp<<tx_offset_reg_shift)));
		}
		else if(calibration_pair == ANACAL_PAIR_B)
		{
			mtEMiiRegWrite(phyaddr, 0x1e, 0x00dd, 0x0100);				// 1e_dd[8]:rg_txg_calen_b
			mtEMiiRegWrite(phyaddr, 0x1e, 0x017e, (0x8000|DAC_IN_0V));	// 1e_17e:dac_in0_b
			mtEMiiRegWrite(phyaddr, 0x1e, 0x0182, (0x8000|DAC_IN_0V));	// 1e_182:dac_in1_b
			reg_temp = (mtEMiiRegRead(phyaddr, 0x1e, 0x0172) & (~0x003f));
			tx_offset_reg_shift = 0;									// 1e_172[5:0]
			tx_offset_reg = 0x0172;
			//mtEMiiRegWrite(phyaddr, 0x1e, tx_offset_reg, (reg_temp|(tx_offset_temp<<tx_offset_reg_shift)));
		}
		else if(calibration_pair == ANACAL_PAIR_C)
		{
			mtEMiiRegWrite(phyaddr, 0x1e, 0x00dd, 0x0010);				// 1e_dd[4]:rg_txg_calen_c
			mtEMiiRegWrite(phyaddr, 0x1e, 0x017f, (0x8000|DAC_IN_0V));	// 1e_17f:dac_in0_c
			mtEMiiRegWrite(phyaddr, 0x1e, 0x0183, (0x8000|DAC_IN_0V));	// 1e_183:dac_in1_c
			reg_temp = (mtEMiiRegRead(phyaddr, 0x1e, 0x0173) & (~0x3f00));
			tx_offset_reg_shift = 8;									// 1e_173[13:8]
			tx_offset_reg = 0x0173;
			//mtEMiiRegWrite(phyaddr, 0x1e, tx_offset_reg, (reg_temp|(tx_offset_temp<<tx_offset_reg_shift)));
		}
		else // if(calibration_pair == ANACAL_PAIR_D)
		{
			mtEMiiRegWrite(phyaddr, 0x1e, 0x00dd, 0x0001);				// 1e_dd[0]:rg_txg_calen_d
			mtEMiiRegWrite(phyaddr, 0x1e, 0x0180, (0x8000|DAC_IN_0V));	// 1e_180:dac_in0_d
			mtEMiiRegWrite(phyaddr, 0x1e, 0x0184, (0x8000|DAC_IN_0V));	// 1e_184:dac_in1_d
			reg_temp = (mtEMiiRegRead(phyaddr, 0x1e, 0x0173) & (~0x003f));
			tx_offset_reg_shift = 0;									// 1e_173[5:0]
			tx_offset_reg = 0x0173;
			//mtEMiiRegWrite(phyaddr, 0x1e, tx_offset_reg, (reg_temp|(tx_offset_temp<<tx_offset_reg_shift)));
		}
		mtEMiiRegWrite(phyaddr, 0x1e, tx_offset_reg, (reg_temp|(tx_offset_temp<<tx_offset_reg_shift)));	// 1e_172, 1e_173
		
		all_ana_cal_status = allGeAnaCalWait(20, phyaddr); // delay 20 usec
		if(all_ana_cal_status == 0)
		{
			all_ana_cal_status = ANACAL_ERROR;	
			printf(" GE Tx offset AnaCal ERROR!   \r\n");
		}
	
		ad_cal_comp_out_init = (mtEMiiRegRead(phyaddr, 0x1e, 0x017a)>>8) & 0x1;		// 1e_17a[8]:ad_cal_comp_out	
		if(ad_cal_comp_out_init == 1)
		{
			calibration_polarity = -1;
		}
		else
		{
			calibration_polarity = 1;
		}

		cnt = 0;
		while(all_ana_cal_status < ANACAL_ERROR)
		{
			cnt ++;
			tx_offset_temp += calibration_polarity;
			if(tx_offset_temp >= 0)
			{
				cal_temp = tx_offset_temp;
			}
			else
			{
				cal_temp = (1<<(TX_AMP_OFFSET_VALID_BITS-1)) | abs(tx_offset_temp);
			}
			mtEMiiRegWrite(phyaddr, 0x1e, tx_offset_reg, (reg_temp|(cal_temp<<tx_offset_reg_shift)));

			all_ana_cal_status = allGeAnaCalWait(20, phyaddr); // delay 20 usec
			if(all_ana_cal_status == 0)
			{
				all_ana_cal_status = ANACAL_ERROR;	
				printf(" GE Tx offset AnaCal ERROR!   \r\n");
			}
			else if(((mtEMiiRegRead(phyaddr, 0x1e, 0x017a)>>8)&0x1) != ad_cal_comp_out_init) 
			{
				all_ana_cal_status = ANACAL_FINISH;	
			}
			else
			{
				if((tx_offset_temp == -31)||(tx_offset_temp == 31))	
				{
					all_ana_cal_status = ANACAL_SATURATION;  // need to FT
					printf(" GE Tx offset AnaCal Saturation!  \r\n");
				}
			}
		}
		
		if(all_ana_cal_status == ANACAL_ERROR)
		{	
			tx_offset_temp = TX_AMP_OFFSET_0mV;
			mtEMiiRegWrite(phyaddr, 0x1e, tx_offset_reg, (reg_temp|(tx_offset_temp<<tx_offset_reg_shift)));
		}
		else
		{
			printf(" GE Tx offset AnaCal Done! (%d)(0x%x)  \r\n", cnt, cal_temp);
		}
	}
	mtEMiiRegWrite(phyaddr, 0x1e, 0x017d, 0x0000);
	mtEMiiRegWrite(phyaddr, 0x1e, 0x017e, 0x0000);
	mtEMiiRegWrite(phyaddr, 0x1e, 0x017f, 0x0000);
	mtEMiiRegWrite(phyaddr, 0x1e, 0x0180, 0x0000);
	mtEMiiRegWrite(phyaddr, 0x1e, 0x0181, 0x0000);
	mtEMiiRegWrite(phyaddr, 0x1e, 0x0182, 0x0000);
	mtEMiiRegWrite(phyaddr, 0x1e, 0x0183, 0x0000);
	mtEMiiRegWrite(phyaddr, 0x1e, 0x0184, 0x0000);
	
	mtEMiiRegWrite(phyaddr, 0x1e, 0x00db, 0x0000);	// disable analog calibration circuit
	mtEMiiRegWrite(phyaddr, 0x1e, 0x00dc, 0x0000);	// disable Tx offset calibration circuit
	mtEMiiRegWrite(phyaddr, 0x1e, 0x003e, 0x0000);	// disable Tx VLD force mode
	mtEMiiRegWrite(phyaddr, 0x1e, 0x00dd, 0x0000);	// disable Tx offset/amplitude calibration circuit
	// *** Tx offset Cal end ***

	// *** Tx Amp Cal start *******************************************************************************
	mtEMiiRegWrite(phyaddr, 0x1e, 0x00db, 0x1100);	// 1e_db[12]:rg_cal_ckinv, [8]:rg_ana_calen, [4]:rg_rext_calen, [0]:rg_zcalen_a
	mtEMiiRegWrite(phyaddr, 0x1e, 0x00dc, 0x0001);	// 1e_dc[0]:rg_txvos_calen
	mtEMiiRegWrite(phyaddr, 0x1e, 0x00e1, 0x0010);	// 1e_e1[4]:select 1V
	mtEMiiRegWrite(phyaddr, 0x1e, 0x003e, 0xf808);	// 1e_3e:enable Tx VLD
	
	for(calibration_pair = ANACAL_PAIR_A; calibration_pair <= ANACAL_PAIR_D; calibration_pair ++)
	{
		tx_amp_temp = 0x20;	// start with 0 dB

		if(calibration_pair == ANACAL_PAIR_A)
		{
			mtEMiiRegWrite(phyaddr, 0x1e, 0x00dd, 0x1000);				// 1e_dd[12]:tx_a amp calibration enable
			mtEMiiRegWrite(phyaddr, 0x1e, 0x017d, (0x8000|DAC_IN_2V));	// 1e_17d:dac_in0_a	
			mtEMiiRegWrite(phyaddr, 0x1e, 0x0181, (0x8000|DAC_IN_2V));	// 1e_181:dac_in1_a
			reg_temp = (mtEMiiRegRead(phyaddr, 0x1e, 0x012) & (~0xfc00));
			tx_amp_reg_shift = 10;										// 1e_12[15:10]
			tx_amp_reg = 0x12;
			tx_amp_reg_100 = 0x16;
		}
		else if(calibration_pair == ANACAL_PAIR_B)
		{
			mtEMiiRegWrite(phyaddr, 0x1e, 0x00dd, 0x0100);				// 1e_dd[8]:tx_b amp calibration enable
			mtEMiiRegWrite(phyaddr, 0x1e, 0x017e, (0x8000|DAC_IN_2V));	// 1e_17e:dac_in0_b
			mtEMiiRegWrite(phyaddr, 0x1e, 0x0182, (0x8000|DAC_IN_2V));	// 1e_182:dac_in1_b
			reg_temp = (mtEMiiRegRead(phyaddr, 0x1e, 0x017) & (~0x3f00));
			tx_amp_reg_shift = 8;										// 1e_17[13:8]
			tx_amp_reg = 0x17;
			tx_amp_reg_100 = 0x18;
		}
		else if(calibration_pair == ANACAL_PAIR_C)
		{
			mtEMiiRegWrite(phyaddr, 0x1e, 0x00dd, 0x0010);				// 1e_dd[4]:tx_c amp calibration enable
			mtEMiiRegWrite(phyaddr, 0x1e, 0x017f, (0x8000|DAC_IN_2V));	// 1e_17f:dac_in0_c
			mtEMiiRegWrite(phyaddr, 0x1e, 0x0183, (0x8000|DAC_IN_2V));	// 1e_183:dac_in1_c
			reg_temp = (mtEMiiRegRead(phyaddr, 0x1e, 0x019) & (~0x3f00));
			tx_amp_reg_shift = 8;										// 1e_19[13:8]
			tx_amp_reg = 0x19;
			tx_amp_reg_100 = 0x20;
		}
		else //if(calibration_pair == ANACAL_PAIR_D)
		{
			mtEMiiRegWrite(phyaddr, 0x1e, 0x00dd, 0x0001);				// 1e_dd[0]:tx_d amp calibration enable
			mtEMiiRegWrite(phyaddr, 0x1e, 0x0180, (0x8000|DAC_IN_2V));	// 1e_180:dac_in0_d
			mtEMiiRegWrite(phyaddr, 0x1e, 0x0184, (0x8000|DAC_IN_2V));	// 1e_184:dac_in1_d
			reg_temp = (mtEMiiRegRead(phyaddr, 0x1e, 0x021) & (~0x3f00));
			tx_amp_reg_shift = 8;										// 1e_21[13:8]
			tx_amp_reg = 0x21;
			tx_amp_reg_100 = 0x22;
		}
		mtEMiiRegWrite(phyaddr, 0x1e, tx_amp_reg, (reg_temp|(tx_amp_temp<<tx_amp_reg_shift)));	// 1e_12, 1e_17, 1e_19, 1e_21

		all_ana_cal_status = allGeAnaCalWait(20, phyaddr); 	// delay 20 usec
		if(all_ana_cal_status == 0)
		{
			all_ana_cal_status = ANACAL_ERROR;	
			printf(" GE Tx amp AnaCal ERROR!   \r\n");
		}
	
		ad_cal_comp_out_init = (mtEMiiRegRead(phyaddr, 0x1e, 0x017a)>>8) & 0x1;		// 1e_17a[8]:ad_cal_comp_out
		if(ad_cal_comp_out_init == 1)
		{
			calibration_polarity = -1;
		}
		else
		{
			calibration_polarity = 1;
		}

		cnt =0;
		while(all_ana_cal_status < ANACAL_ERROR)
		{
			cnt ++;
			tx_amp_temp += calibration_polarity;
			mtEMiiRegWrite(phyaddr, 0x1e, tx_amp_reg, (reg_temp|(tx_amp_temp<<tx_amp_reg_shift)));
			all_ana_cal_status = allGeAnaCalWait(100, phyaddr); // delay 20 usec
			if(all_ana_cal_status == 0)
			{
				all_ana_cal_status = ANACAL_ERROR;	
				printf(" GE Tx amp AnaCal ERROR!   \r\n");
			}
			else if(((mtEMiiRegRead(phyaddr, 0x1e, 0x017a)>>8)&0x1) != ad_cal_comp_out_init) 
			{
				all_ana_cal_status = ANACAL_FINISH;	
				reg_temp = mtEMiiRegRead(phyaddr, 0x1e, tx_amp_reg)&(~0xff00);
				if(calibration_pair == ANACAL_PAIR_A)
					//mtEMiiRegWrite(phyaddr, 0x1e, tx_amp_reg, (reg_temp|((tx_amp_temp+0x9)<<tx_amp_reg_shift)));	// for gbe(DAC), 0xc
					mtEMiiRegWrite(phyaddr, 0x1e, tx_amp_reg, (reg_temp|((tx_amp_temp+0x2)<<tx_amp_reg_shift)));
				else //if(calibration_pair == ANACAL_PAIR_B)
					mtEMiiRegWrite(phyaddr, 0x1e, tx_amp_reg, (reg_temp|((tx_amp_temp+0x2)<<tx_amp_reg_shift)));	// for gbe(DAC), 0xc

				printf(" GE Tx amp AnaCal Done! (pair-%d)(0x%x)(0x%x)\r\n", calibration_pair, tx_amp_reg, mtEMiiRegRead(phyaddr, 0x1e, tx_amp_reg));
				
			}
			else
			{
				if((tx_amp_temp == 0x3f)||(tx_amp_temp == 0x00))	
				{
					all_ana_cal_status = ANACAL_SATURATION;  // need to FT
					printf(" GE Tx amp AnaCal Saturation!  \r\n");
				}
			}
		}
		if(all_ana_cal_status == ANACAL_ERROR)
		{	
			tx_amp_temp = 0x20;
			mtEMiiRegWrite(phyaddr, 0x1e, tx_amp_reg, (reg_temp|(tx_amp_temp<<tx_amp_reg_shift)));
		}
		else
		{
			printf(" GE Tx amp AnaCal Done! (%d) (0x%x)\r\n", cnt, tx_amp_temp);
			
			reg_temp = mtEMiiRegRead(phyaddr, 0x1e, tx_amp_reg_100)& (~0x003f);
			if((all_ana_cal_status == ANACAL_SATURATION)||(tx_amp_temp==0))
				mtEMiiRegWrite(phyaddr, 0x1e, tx_amp_reg_100, (reg_temp|(0x0010)));  // for tst	
			else
				mtEMiiRegWrite(phyaddr, 0x1e, tx_amp_reg_100, (reg_temp|(tx_amp_temp+14)));  // for tst

			reg_temp = mtEMiiRegRead(phyaddr, 0x1e, tx_amp_reg_100)&(~0xff00);
			if((all_ana_cal_status == ANACAL_SATURATION)||(tx_amp_temp==0))
				mtEMiiRegWrite(phyaddr, 0x1e, tx_amp_reg_100, (reg_temp|((0x4)<<tx_amp_reg_shift)));	// for 100
			else
			{
				//if(calibration_pair == ANACAL_PAIR_A)
				#if defined(TCSUPPORT_CPU_EN7521)
					mtEMiiRegWrite(phyaddr, 0x1e, tx_amp_reg_100, (reg_temp|((tx_amp_temp+0x4)<<tx_amp_reg_shift)));	// for 100, 0xc
				#else //if(calibration_pair == ANACAL_PAIR_B)
					mtEMiiRegWrite(phyaddr, 0x1e, tx_amp_reg_100, (reg_temp|((tx_amp_temp+0x9)<<tx_amp_reg_shift)));	// for 100, 0xc, 0x9 // for 7512
				#endif	
			}
		}
	}
	mtEMiiRegWrite(phyaddr, 0x1e, 0x017d, 0x0000);
	mtEMiiRegWrite(phyaddr, 0x1e, 0x017e, 0x0000);
	mtEMiiRegWrite(phyaddr, 0x1e, 0x017f, 0x0000);
	mtEMiiRegWrite(phyaddr, 0x1e, 0x0180, 0x0000);
	mtEMiiRegWrite(phyaddr, 0x1e, 0x0181, 0x0000);
	mtEMiiRegWrite(phyaddr, 0x1e, 0x0182, 0x0000);
	mtEMiiRegWrite(phyaddr, 0x1e, 0x0183, 0x0000);
	mtEMiiRegWrite(phyaddr, 0x1e, 0x0184, 0x0000);
	
	mtEMiiRegWrite(phyaddr, 0x1e, 0x00db, 0x0000);	// disable analog calibration circuit
	mtEMiiRegWrite(phyaddr, 0x1e, 0x00dc, 0x0000);	// disable Tx offset calibration circuit
	mtEMiiRegWrite(phyaddr, 0x1e, 0x003e, 0x0000);	// disable Tx VLD force mode
	mtEMiiRegWrite(phyaddr, 0x1e, 0x00dd, 0x0000);	// disable Tx offset/amplitude calibration circuit
	// *** Tx Amp Cal end ***

	// *** Rx offset Cal start ******************************************************************************
	mtEMiiRegWrite(phyaddr, 0x1e, 0x0096, 0x8000);					// 1e_96[15]:bypass_tx_offset_cal, Hw bypass, Fw cal
	mtEMiiRegWrite(phyaddr, 0x1e, 0x0037, 0x0033);					// tx/rx_cal_criteria_value
	reg_temp = (mtEMiiRegRead(phyaddr, 0x1e, 0x0039) & (~0x4800));	// [14]: bypass all calibration, [11]: bypass adc offset cal analog
	mtEMiiRegWrite(phyaddr, 0x1e, 0x0039, reg_temp);				// rx offset cal by Hw setup
	reg_temp = (mtEMiiRegRead(phyaddr, 0x1f, 0x0107) & (~0x1000));	// [12]: enable rtune calibration
	mtEMiiRegWrite(phyaddr, 0x1f, 0x0107, reg_temp);				// disable rtune calibration
	reg_temp = (mtEMiiRegRead(phyaddr, 0x1e, 0x0171) & (~0x0180));	// 1e_171[8:7]: bypass tx/rx dc offset cancellation process
	mtEMiiRegWrite(phyaddr, 0x1e, 0x0171, (reg_temp | 0x0180));
	reg_temp = mtEMiiRegRead(phyaddr, 0x1e, 0x0039);
	mtEMiiRegWrite(phyaddr, 0x1e, 0x0039, (reg_temp | 0x2000));		// rx offset calibration start
	mtEMiiRegWrite(phyaddr, 0x1e, 0x0039, (reg_temp & (~0x2000)));	// rx offset calibration end
	pause(10);														// mdelay for Hw calibration finish
	reg_temp = (mtEMiiRegRead(phyaddr, 0x1e, 0x0171) & (~0x0180));
	mtEMiiRegWrite(phyaddr, 0x1e, 0x0171, reg_temp);

	tcMiiStationWrite(phyaddr, 0x0,  reg0_temp);
	mtEMiiRegWrite(phyaddr, 0x1f, 0x0100, 0x0000);
	mtEMiiRegWrite(phyaddr, 0x1e, 0x0145, dev1e_145_temp);
	// *** Rx offset Cal end ***
}


void	GECal_Rext(uint8 phyaddr, uint32 delay)									// for EN7512 
{
	uint8	rg_zcal_ctrl, all_ana_cal_status;
	uint16	ad_cal_comp_out_init;
	uint16	dev1e_e0_ana_cal_r5; 
	int 	calibration_polarity;
	uint16	reg_temp;
	uint8	cnt=0;

	// *** Iext/Rext Cal start ********************************************************************************
	all_ana_cal_status = ANACAL_INIT;
	// analog calibration enable, Rext calibration enable
	mtEMiiRegWrite(phyaddr, 0x1e, 0x00db, 0x1110);	// 1e_db[12]:rg_cal_ckinv, [8]:rg_ana_calen, [4]:rg_rext_calen, [0]:rg_zcalen_a
	mtEMiiRegWrite(phyaddr, 0x1e, 0x00dc, 0x0000);	// 1e_dc[0]:rg_txvos_calen
	mtEMiiRegWrite(phyaddr, 0x1e, 0x00e1, 0x0000);	// 1e_e1[4]:rg_cal_refsel(0:1.2V)

	rg_zcal_ctrl = 0x20;  								// start with 0 dB
	dev1e_e0_ana_cal_r5 = mtEMiiRegRead(phyaddr, 0x1e, 0x00e0);
	mtEMiiRegWrite(phyaddr, 0x1e, 0x00e0, (rg_zcal_ctrl));	// 1e_e0[5:0]:rg_zcal_ctrl

	all_ana_cal_status = allGeAnaCalWait(delay, phyaddr); 	// delay 20 usec
	if(all_ana_cal_status == 0)
	{
		all_ana_cal_status = ANACAL_ERROR;	
		printf(" GE Rext AnaCal ERROR!   \r\n");
	}
	
	ad_cal_comp_out_init = (mtEMiiRegRead(phyaddr, 0x1e, 0x017a)>>8) & 0x1;		// 1e_17a[8]:ad_cal_comp_out	
	if(ad_cal_comp_out_init == 1)
	{
		calibration_polarity = -1;
	}
	else // ad_cal_comp_out_init == 0
	{
		calibration_polarity = 1;
	}
	
	cnt = 0;
	while(all_ana_cal_status < ANACAL_ERROR)
	{
		cnt ++;
		rg_zcal_ctrl += calibration_polarity;
		mtEMiiRegWrite(phyaddr, 0x1e, 0x00e0, (rg_zcal_ctrl));
		all_ana_cal_status = allGeAnaCalWait(delay, phyaddr); // delay 20 usec

		if(all_ana_cal_status == 0)
		{
			all_ana_cal_status = ANACAL_ERROR;	
			printf("  GE Rext AnaCal ERROR!   \r\n");
		}
		else if(((mtEMiiRegRead(phyaddr, 0x1e, 0x017a)>>8)&0x1) != ad_cal_comp_out_init) 
		{
			all_ana_cal_status = ANACAL_FINISH;	
			printf("  GE Rext AnaCal Done! (%d)(0x%x)  \r\n", cnt, rg_zcal_ctrl);
		}
		else
		{
			if((rg_zcal_ctrl == 0x3F)||(rg_zcal_ctrl == 0x00))	
			{
				all_ana_cal_status = ANACAL_SATURATION;  // need to FT(IC fail?)
				printf(" GE Rext AnaCal Saturation!  \r\n");
				rg_zcal_ctrl = 0x20;  // 0 dB
			}
			else
				printf(" GE Rxet cal (%d)(%d)(%d)(0x%x)  \r\n", cnt, ad_cal_comp_out_init, ((mtEMiiRegRead(phyaddr, 0x1e, 0x017a)>>8)&0x1), mtEMiiRegRead(phyaddr, 0x1e, 0xe0));
		}
	}

	if(all_ana_cal_status == ANACAL_ERROR)
	{	
		rg_zcal_ctrl = 0x20;  // 0 dB
		mtEMiiRegWrite(phyaddr, 0x1e, 0x00e0, (dev1e_e0_ana_cal_r5 | rg_zcal_ctrl));
	}
	else
	{
		mtEMiiRegWrite(phyaddr, 0x1e, 0x00e0, (dev1e_e0_ana_cal_r5 | rg_zcal_ctrl));
		mtEMiiRegWrite(phyaddr, 0x1e, 0x00e0, ((rg_zcal_ctrl<<8)|rg_zcal_ctrl));
		//****  1f_115[2:0] = rg_zcal_ctrl[5:3]  // Mog review
		mtEMiiRegWrite(phyaddr, 0x1f, 0x0115, ((rg_zcal_ctrl & 0x3f)>>3));
		printf("  GE Rext AnaCal Done! (%d)(0x%x)  \r\n", cnt, rg_zcal_ctrl);
		GECal_flag = 1;
		
		regReadWord(0xbfa2016c, reg_temp);
		//printf("RG_BG_RASEL = 0x%x (x%x)\r\n", reg_temp, rg_zcal_ctrl);
		reg_temp = (reg_temp & 0x1fff); 
		reg_temp = ((((rg_zcal_ctrl>>3)&0x7)<<13) | reg_temp);
		//printf("RG_BG_RASEL = 0x%x (x%x)\r\n", reg_temp, rg_zcal_ctrl);
		regWriteWord(0xbfa2016c, reg_temp);   // for ACD/steven simldo
	}
	mtEMiiRegWrite(phyaddr, 0x1e, 0x00db, 0x0000);
	// *** Iext/Rext Cal end ***
}

void	GECal_R50(uint8 phyaddr, uint32 delay)									// for EN7512 
{
	uint8	rg_zcal_ctrl, all_ana_cal_status;
	uint16	ad_cal_comp_out_init;
	uint16	dev1e_e0_ana_cal_r5; 
	int 	calibration_polarity;
	uint16	calibration_pair;
	uint8	cnt=0;

	// *** R50 Cal start *******************************************************************************
	mtEMiiRegWrite(phyaddr, 0x1e, 0x00db, 0x1100);	// 1e_db[12]:rg_cal_ckinv, [8]:rg_ana_calen, [4]:rg_rext_calen, [0]:rg_zcalen_a
	mtEMiiRegWrite(phyaddr, 0x1e, 0x00dc, 0x0000);	// 1e_dc[0]:rg_txvos_calen
	for(calibration_pair = ANACAL_PAIR_A; calibration_pair <= ANACAL_PAIR_D; calibration_pair ++)
	{
		rg_zcal_ctrl = 0x20;  						// start with 0 dB
		dev1e_e0_ana_cal_r5 = (mtEMiiRegRead(phyaddr, 0x1e, 0x00e0) & (~0x003f));
		mtEMiiRegWrite(phyaddr, 0x1e, 0x00e0, (dev1e_e0_ana_cal_r5 | rg_zcal_ctrl));	// 1e_e0[5:0]:rg_zcal_ctrl
		if(calibration_pair == ANACAL_PAIR_A)
		{
			mtEMiiRegWrite(phyaddr, 0x1e, 0x00db, 0x1101);	// 1e_db[12]:rg_cal_ckinv, [8]:rg_ana_calen, [4]:rg_rext_calen, [0]:rg_zcalen_a
			mtEMiiRegWrite(phyaddr, 0x1e, 0x00dc, 0x0000);	
		}
		else if(calibration_pair == ANACAL_PAIR_B)
		{
			mtEMiiRegWrite(phyaddr, 0x1e, 0x00db, 0x1100);	// 1e_db[12]:rg_cal_ckinv, [8]:rg_ana_calen, [4]:rg_rext_calen, [0]:rg_zcalen_a
			mtEMiiRegWrite(phyaddr, 0x1e, 0x00dc, 0x1000);	// 1e_dc[12]:rg_zcalen_b
		}
		else if(calibration_pair == ANACAL_PAIR_C)
		{
			mtEMiiRegWrite(phyaddr, 0x1e, 0x00db, 0x1100);	// 1e_db[12]:rg_cal_ckinv, [8]:rg_ana_calen, [4]:rg_rext_calen, [0]:rg_zcalen_a
			mtEMiiRegWrite(phyaddr, 0x1e, 0x00dc, 0x0100);	// 1e_dc[8]:rg_zcalen_c
		}
		else // if(calibration_pair == ANACAL_PAIR_D)
		{
			mtEMiiRegWrite(phyaddr, 0x1e, 0x00db, 0x1100);	// 1e_db[12]:rg_cal_ckinv, [8]:rg_ana_calen, [4]:rg_rext_calen, [0]:rg_zcalen_a
			mtEMiiRegWrite(phyaddr, 0x1e, 0x00dc, 0x0010);	// 1e_dc[4]:rg_zcalen_d
		}

		all_ana_cal_status = allGeAnaCalWait(20, phyaddr); // delay 20 usec
		if(all_ana_cal_status == 0)
		{
			all_ana_cal_status = ANACAL_ERROR;	
			printf("  GE R50 AnaCal ERROR!   \r\n");
		}
	
		ad_cal_comp_out_init = (mtEMiiRegRead(phyaddr, 0x1e, 0x017a)>>8) & 0x1;		// 1e_17a[8]:ad_cal_comp_out	
		if(ad_cal_comp_out_init == 1)
		{
			calibration_polarity = -1;
		}
		else
		{
			calibration_polarity = 1;
		}

		cnt = 0;
		while(all_ana_cal_status < ANACAL_ERROR)
		{
			cnt ++;
			rg_zcal_ctrl += calibration_polarity;
			mtEMiiRegWrite(phyaddr, 0x1e, 0x00e0, (dev1e_e0_ana_cal_r5 | rg_zcal_ctrl));
			all_ana_cal_status = allGeAnaCalWait(20, phyaddr); // delay 20 usec

			if(all_ana_cal_status == 0)
			{
				all_ana_cal_status = ANACAL_ERROR;	
				printf("  GE R50 AnaCal ERROR!   \r\n");
			}
			else if(((mtEMiiRegRead(phyaddr, 0x1e, 0x017a)>>8)&0x1) != ad_cal_comp_out_init) 
			{
				all_ana_cal_status = ANACAL_FINISH;	
			}
			else
			{
				if((rg_zcal_ctrl == 0x3F)||(rg_zcal_ctrl == 0x00))	
				{
					all_ana_cal_status = ANACAL_SATURATION;  // need to FT
					printf(" GE R50 AnaCal Saturation!  \r\n");
				}
			}
		}
		
		if(all_ana_cal_status == ANACAL_ERROR)
		{	
			rg_zcal_ctrl = 0x20;  // 0 dB
			//mtEMiiRegWrite(phyaddr, 0x1e, 0x00e0, (dev1e_e0_ana_cal_r5 | rg_zcal_ctrl));
		}
		else
		{
			rg_zcal_ctrl = ZCAL_TO_R50ohm_TBL[rg_zcal_ctrl];	// wait Mog zcal/r50 mapping table
			printf("  GE R50 AnaCal Done! (%d) (0x%x)(0x%x) \r\n", cnt, rg_zcal_ctrl, (rg_zcal_ctrl|0x80));
		}
		
		if(calibration_pair == ANACAL_PAIR_A)  
		{
			ad_cal_comp_out_init = mtEMiiRegRead(phyaddr, 0x1e, 0x0174) & (~0x7f00);
			//ad_cal_comp_out_init = mtEMiiRegRead(phyaddr, 0x1e, 0x0174);
			//printf(" GE-a 1e_174(0x%x)(0x%x), 1e_175(0x%x)  \r\n", mtEMiiRegRead(phyaddr, 0x1e, 0x0174), ad_cal_comp_out_init, mtEMiiRegRead(phyaddr, 0x1e, 0x0175));
			mtEMiiRegWrite(phyaddr, 0x1e, 0x0174, (ad_cal_comp_out_init | (((rg_zcal_ctrl<<8)&0xff00) | 0x8000)));	// 1e_174[15:8]
			//printf(" GE-a 1e_174(0x%x), 1e_175(0x%x)  \r\n", mtEMiiRegRead(phyaddr, 0x1e, 0x0174), mtEMiiRegRead(phyaddr, 0x1e, 0x0175));
		}
		else if(calibration_pair == ANACAL_PAIR_B)
		{
			ad_cal_comp_out_init = mtEMiiRegRead(phyaddr, 0x1e, 0x0174) & (~0x007f);
			//ad_cal_comp_out_init = mtEMiiRegRead(phyaddr, 0x1e, 0x0174);
			//printf(" GE-b 1e_174(0x%x)(0x%x), 1e_175(0x%x)  \r\n", mtEMiiRegRead(phyaddr, 0x1e, 0x0174), ad_cal_comp_out_init, mtEMiiRegRead(phyaddr, 0x1e, 0x0175));
			
			mtEMiiRegWrite(phyaddr, 0x1e, 0x0174, (ad_cal_comp_out_init | (((rg_zcal_ctrl<<0)&0x00ff) | 0x0080)));	// 1e_174[7:0]
			//printf(" GE-b 1e_174(0x%x), 1e_175(0x%x)  \r\n", mtEMiiRegRead(phyaddr, 0x1e, 0x0174), mtEMiiRegRead(phyaddr, 0x1e, 0x0175));
		}
		else if(calibration_pair == ANACAL_PAIR_C)
		{
			ad_cal_comp_out_init = mtEMiiRegRead(phyaddr, 0x1e, 0x0175) & (~0x7f00);
			//ad_cal_comp_out_init = mtEMiiRegRead(phyaddr, 0x1e, 0x0175);
			mtEMiiRegWrite(phyaddr, 0x1e, 0x0175, (ad_cal_comp_out_init | (((rg_zcal_ctrl<<8)&0xff00) | 0x8000)));	// 1e_175[15:8]
			//printf(" GE-c 1e_174(0x%x), 1e_175(0x%x)  \r\n", mtEMiiRegRead(phyaddr, 0x1e, 0x0174), mtEMiiRegRead(phyaddr, 0x1e, 0x0175));
		}
		else // if(calibration_pair == ANACAL_PAIR_D)
		{
			ad_cal_comp_out_init = mtEMiiRegRead(phyaddr, 0x1e, 0x0175) & (~0x007f);
			//ad_cal_comp_out_init = mtEMiiRegRead(phyaddr, 0x1e, 0x0175);
			mtEMiiRegWrite(phyaddr, 0x1e, 0x0175, (ad_cal_comp_out_init | (((rg_zcal_ctrl<<0)&0x00ff) | 0x0080)));	// 1e_175[7:0]
			//printf(" GE-d 1e_174(0x%x), 1e_175(0x%x)  \r\n", mtEMiiRegRead(phyaddr, 0x1e, 0x0174), mtEMiiRegRead(phyaddr, 0x1e, 0x0175));
		}
		//mtEMiiRegWrite(phyaddr, 0x1e, 0x00e0, ((rg_zcal_ctrl<<8)|rg_zcal_ctrl));
	}
	

	mtEMiiRegWrite(phyaddr, 0x1e, 0x00db, 0x0000);
	// *** R50 Cal end ***

}

void	GECal_tx_amp(uint8 phyaddr, uint32 delay)									// for EN7512 
{
	uint8	all_ana_cal_status;
	uint16	ad_cal_comp_out_init; 
	int 	calibration_polarity;
	uint16	calibration_pair;
	uint8	tx_amp_reg_shift; 
	uint16	reg_temp;
	uint8	tx_amp_temp, tx_amp_reg, cnt=0, tx_amp_reg_100;

	// *** Tx Amp Cal start *******************************************************************************
	mtEMiiRegWrite(phyaddr, 0x1e, 0x00db, 0x1100);	// 1e_db[12]:rg_cal_ckinv, [8]:rg_ana_calen, [4]:rg_rext_calen, [0]:rg_zcalen_a
	mtEMiiRegWrite(phyaddr, 0x1e, 0x00dc, 0x0001);	// 1e_dc[0]:rg_txvos_calen
	mtEMiiRegWrite(phyaddr, 0x1e, 0x00e1, 0x0010);	// 1e_e1[4]:select 1V
	mtEMiiRegWrite(phyaddr, 0x1e, 0x003e, 0xf808);	// 1e_3e:enable Tx VLD
	
	for(calibration_pair = ANACAL_PAIR_A; calibration_pair <= ANACAL_PAIR_D; calibration_pair ++)
	{
		tx_amp_temp = 0x20;	// start with 0 dB

		if(calibration_pair == ANACAL_PAIR_A)
		{
			mtEMiiRegWrite(phyaddr, 0x1e, 0x00dd, 0x1000);				// 1e_dd[12]:tx_a amp calibration enable
			mtEMiiRegWrite(phyaddr, 0x1e, 0x017d, (0x8000|DAC_IN_2V));	// 1e_17d:dac_in0_a	
			mtEMiiRegWrite(phyaddr, 0x1e, 0x0181, (0x8000|DAC_IN_2V));	// 1e_181:dac_in1_a
			reg_temp = (mtEMiiRegRead(phyaddr, 0x1e, 0x012) & (~0xfc00));
			tx_amp_reg_shift = 10;										// 1e_12[15:10]
			tx_amp_reg = 0x12;
			tx_amp_reg_100 = 0x16;
		}
		else if(calibration_pair == ANACAL_PAIR_B)
		{
			mtEMiiRegWrite(phyaddr, 0x1e, 0x00dd, 0x0100);				// 1e_dd[8]:tx_b amp calibration enable
			mtEMiiRegWrite(phyaddr, 0x1e, 0x017e, (0x8000|DAC_IN_2V));	// 1e_17e:dac_in0_b
			mtEMiiRegWrite(phyaddr, 0x1e, 0x0182, (0x8000|DAC_IN_2V));	// 1e_182:dac_in1_b
			reg_temp = (mtEMiiRegRead(phyaddr, 0x1e, 0x017) & (~0x3f00));
			tx_amp_reg_shift = 8;										// 1e_17[13:8]
			tx_amp_reg = 0x17;
			tx_amp_reg_100 = 0x18;
		}
		else if(calibration_pair == ANACAL_PAIR_C)
		{
			mtEMiiRegWrite(phyaddr, 0x1e, 0x00dd, 0x0010);				// 1e_dd[4]:tx_c amp calibration enable
			mtEMiiRegWrite(phyaddr, 0x1e, 0x017f, (0x8000|DAC_IN_2V));	// 1e_17f:dac_in0_c
			mtEMiiRegWrite(phyaddr, 0x1e, 0x0183, (0x8000|DAC_IN_2V));	// 1e_183:dac_in1_c
			reg_temp = (mtEMiiRegRead(phyaddr, 0x1e, 0x019) & (~0x3f00));
			tx_amp_reg_shift = 8;										// 1e_19[13:8]
			tx_amp_reg = 0x19;
			tx_amp_reg_100 = 0x20;
		}
		else //if(calibration_pair == ANACAL_PAIR_D)
		{
			mtEMiiRegWrite(phyaddr, 0x1e, 0x00dd, 0x0001);				// 1e_dd[0]:tx_d amp calibration enable
			mtEMiiRegWrite(phyaddr, 0x1e, 0x0180, (0x8000|DAC_IN_2V));	// 1e_180:dac_in0_d
			mtEMiiRegWrite(phyaddr, 0x1e, 0x0184, (0x8000|DAC_IN_2V));	// 1e_184:dac_in1_d
			reg_temp = (mtEMiiRegRead(phyaddr, 0x1e, 0x021) & (~0x3f00));
			tx_amp_reg_shift = 8;										// 1e_21[13:8]
			tx_amp_reg = 0x21;
			tx_amp_reg_100 = 0x22;
		}
		mtEMiiRegWrite(phyaddr, 0x1e, tx_amp_reg, (reg_temp|(tx_amp_temp<<tx_amp_reg_shift)));	// 1e_12, 1e_17, 1e_19, 1e_21

		all_ana_cal_status = allGeAnaCalWait(delay, phyaddr); 	// delay 20 usec
		if(all_ana_cal_status == 0)
		{
			all_ana_cal_status = ANACAL_ERROR;	
			printf(" GE Tx amp AnaCal ERROR!   \r\n");
		}
	
		ad_cal_comp_out_init = (mtEMiiRegRead(phyaddr, 0x1e, 0x017a)>>8) & 0x1;		// 1e_17a[8]:ad_cal_comp_out
		if(ad_cal_comp_out_init == 1)
		{
			calibration_polarity = -1;
		}
		else
		{
			calibration_polarity = 1;
		}

		cnt =0;
		while(all_ana_cal_status < ANACAL_ERROR)
		{
			cnt ++;
			tx_amp_temp += calibration_polarity;
			mtEMiiRegWrite(phyaddr, 0x1e, tx_amp_reg, (reg_temp|(tx_amp_temp<<tx_amp_reg_shift)));
			all_ana_cal_status = allGeAnaCalWait(delay, phyaddr); // delay 20 usec
			if(all_ana_cal_status == 0)
			{
				all_ana_cal_status = ANACAL_ERROR;	
				printf(" GE Tx amp AnaCal ERROR!   \r\n");
			}
			else if(((mtEMiiRegRead(phyaddr, 0x1e, 0x017a)>>8)&0x1) != ad_cal_comp_out_init) 
			{
				all_ana_cal_status = ANACAL_FINISH;	
				reg_temp = mtEMiiRegRead(phyaddr, 0x1e, tx_amp_reg)&(~0xff00);
				if(calibration_pair == ANACAL_PAIR_A)
					//mtEMiiRegWrite(phyaddr, 0x1e, tx_amp_reg, (reg_temp|((tx_amp_temp+0x9)<<tx_amp_reg_shift)));	// for gbe(DAC), 0xc
					mtEMiiRegWrite(phyaddr, 0x1e, tx_amp_reg, (reg_temp|((tx_amp_temp)<<tx_amp_reg_shift)));
				else //if(calibration_pair == ANACAL_PAIR_B)
					mtEMiiRegWrite(phyaddr, 0x1e, tx_amp_reg, (reg_temp|((tx_amp_temp)<<tx_amp_reg_shift)));	// for gbe(DAC), 0xc

				printf(" GE Tx amp AnaCal Done! (pair-%d)(0x%x)(0x%x)\r\n", calibration_pair, tx_amp_reg, mtEMiiRegRead(phyaddr, 0x1e, tx_amp_reg));
				
			}
			else
			{
				if((tx_amp_temp == 0x3f)||(tx_amp_temp == 0x00))	
				{
					all_ana_cal_status = ANACAL_SATURATION;  // need to FT
					printf(" GE Tx amp AnaCal Saturation!  \r\n");
				}
			}
		}
		if(all_ana_cal_status == ANACAL_ERROR)
		{	
			tx_amp_temp = 0x20;
			mtEMiiRegWrite(phyaddr, 0x1e, tx_amp_reg, (reg_temp|(tx_amp_temp<<tx_amp_reg_shift)));
		}
		else
		{
			printf(" GE Tx amp AnaCal Done! (%d) (0x%x)\r\n", cnt, tx_amp_temp);

			reg_temp = mtEMiiRegRead(phyaddr, 0x1e, tx_amp_reg_100)& (~0x003f);
			if((all_ana_cal_status == ANACAL_SATURATION)||(tx_amp_temp==0))
				mtEMiiRegWrite(phyaddr, 0x1e, tx_amp_reg_100, (reg_temp|(0x0010)));  // for tst	
			else
				mtEMiiRegWrite(phyaddr, 0x1e, tx_amp_reg_100, (reg_temp|(tx_amp_temp+14)));  // for tst
				
			reg_temp = mtEMiiRegRead(phyaddr, 0x1e, tx_amp_reg_100)&(~0xff00);
			if((all_ana_cal_status == ANACAL_SATURATION)||(tx_amp_temp==0))
				mtEMiiRegWrite(phyaddr, 0x1e, tx_amp_reg_100, (reg_temp|((0x4)<<tx_amp_reg_shift)));	// for 100
			else
			{
				if(calibration_pair == ANACAL_PAIR_A)
					mtEMiiRegWrite(phyaddr, 0x1e, tx_amp_reg_100, (reg_temp|((tx_amp_temp+0x9)<<tx_amp_reg_shift)));	// for 100, 0xc
				else if(calibration_pair == ANACAL_PAIR_B)
					mtEMiiRegWrite(phyaddr, 0x1e, tx_amp_reg_100, (reg_temp|((tx_amp_temp+0x9)<<tx_amp_reg_shift)));	// for 100, 0xc
			}
		}
	}
	mtEMiiRegWrite(phyaddr, 0x1e, 0x017d, 0x0000);
	mtEMiiRegWrite(phyaddr, 0x1e, 0x017e, 0x0000);
	mtEMiiRegWrite(phyaddr, 0x1e, 0x017f, 0x0000);
	mtEMiiRegWrite(phyaddr, 0x1e, 0x0180, 0x0000);
	mtEMiiRegWrite(phyaddr, 0x1e, 0x0181, 0x0000);
	mtEMiiRegWrite(phyaddr, 0x1e, 0x0182, 0x0000);
	mtEMiiRegWrite(phyaddr, 0x1e, 0x0183, 0x0000);
	mtEMiiRegWrite(phyaddr, 0x1e, 0x0184, 0x0000);
	
	mtEMiiRegWrite(phyaddr, 0x1e, 0x00db, 0x0000);	// disable analog calibration circuit
	mtEMiiRegWrite(phyaddr, 0x1e, 0x00dc, 0x0000);	// disable Tx offset calibration circuit
	mtEMiiRegWrite(phyaddr, 0x1e, 0x003e, 0x0000);	// disable Tx VLD force mode
	mtEMiiRegWrite(phyaddr, 0x1e, 0x00dd, 0x0000);	// disable Tx offset/amplitude calibration circuit
	// *** Tx Amp Cal end ***
}



uint8 allGeAnaCalWait(uint32 delay, uint8 phyaddr) // for EN7512 
{
	uint8 all_ana_cal_status;	
	
	mtEMiiRegWrite(phyaddr, 0x1e, 0x017c, 0x0001);	// da_calin_flag pull high
	
	udelay(delay);
	all_ana_cal_status = mtEMiiRegRead(phyaddr, 0x1e, 0x017b) & 0x1;
	mtEMiiRegWrite(phyaddr, 0x1e, 0x017c, 0x0000);	// da_calin_flag pull low

	return all_ana_cal_status;
}





// ************************************************************************
// 		Tx calibration use the Software flow : R50, Tx offset, Tx amp
//		Rx calibration use the Hardware flow
// ************************************************************************
void doFEPhyALLAnalogCal(uint8 port_num) // for EN7512 FE 
{
	uint16	l0r26_temp; 
	uint16 	g4r21_temp; 
	uint8	cnt=0, phyaddr;

	phyaddr = port_num + ephy_addr_base;	

	tcPhyWriteGReg(port_num, 2, 25, 0x10c0);		// g2r25[7:5]:0x110, BG voltage output
	//printf(" (init-%d) g2r25(0x%x)   \r\n", phyaddr, tcPhyReadGReg(port_num, 2, 25));

	l0r26_temp = tcPhyReadLReg(port_num, 0, 26);
	tcPhyWriteLReg(port_num, 0, 26, 0x5600);		// fix mdi
	tcPhyWriteLReg(port_num, 4, 21, 0x0000);		// l4r21[15:12]:disable tx slew control
	tcPhyWriteLReg(port_num, 0, 0, 0x2100);
	//tcPhyWriteLReg(port_num, 2, 22, 0xffff);


	// *** R50 Cal start ***********************************************************************************
	cnt = 0;
	while((FECal_R50_flag == 0)&&(cnt < 0x3))
	{
		FECal_R50(port_num, 1);  // allen_20160608
		cnt++;
		if(FECal_R50_flag == 0)
			printf(" FE-%d R50 wait! (%d)  \r\n", phyaddr, cnt);
	}
	// *** R50 Cal end ***

	// *** Tx offset Cal start ******************************************************************

	#if 1
	cnt = 0;
	while((FECal_tx_offset_flag == 0)&&(cnt < 0x3))
	{
		FECal_Tx_offset(port_num, 100);
		cnt++;
		if(FECal_tx_offset_flag == 0)
			printf(" FE-%d Tx offset AnaCal wait! (%d)  \r\n", phyaddr, cnt);
	}
	#endif
	// *** Tx offset Cal end ***

	// *** Tx Amp Cal start ********************************************************************
	cnt = 0;
	while((FECal_flag == 0)&&(cnt < 0x3))
	{
			FECal_TxAmp(port_num, 300);  // allen_20160608
			
		cnt++;
		if(FECal_flag == 0)
			printf(" FE-%d Tx amp AnaCal wait! (%d)  \r\n", phyaddr, cnt);
	}

	tcPhyWriteGReg(port_num, 1, 26, 0x0000);
	
	
	//tcPhyWriteGReg(port_num, 1, 26, 0x0000);	
	//l3r25_temp = tcPhyReadLReg(FE_CAL_P0, 3, 25);
	//tcPhyWriteLReg(FE_CAL_P0, 3, 25, (l3r25_temp&0x01ff));
	//tcPhyWriteLReg(port_num, 3, 25, 0x0000);
	//tcPhyWriteLReg(0, 3, 25, 0x0000);
    //tcPhyWriteLReg(port_num, 0, 30, 0x0000);
	// *** Tx Amp Cal end ***

	// *** Rx offset Cal start *************************************************************************
	//g4r21_temp = tcPhyReadGReg(port_num, 4, 21);
	//tcPhyWriteGReg(port_num, 4, 21, (g4r21_temp|0x0830));	// g4r21[11],[6:4]:Hw bypass tx offset cal, Fw cal
	
	//l1r29_temp = tcPhyReadLReg(port_num, 1, 29);
	//tcPhyWriteLReg(port_num, 1, 29, (l1r29_temp|0x2000));	
	
	//pause(10);		// mdelay for Hw calibration finish

	
	tcPhyWriteLReg(port_num, 0, 26, l0r26_temp);
	tcPhyWriteGReg(port_num, 1, 26, 0x0000);	
	//tcPhyWriteLReg(port_num, 3, 25, 0x0000);
	//tcPhyWriteLReg(FE_CAL_P0, 3, 25, (0x0000));
	tcPhyWriteLReg(port_num, 2, 22, 0x4444);
	tcPhyWriteLReg(port_num, 0, 0, 0x3100);
	//tcPhyWriteLReg(port_num, 4, 21, (0x0000));
	// *** Rx offset Cal end ***
}


void FECal_R50(uint8 port_num, uint32 delay)
{

		uint8	rg_zcal_ctrl, all_ana_cal_status, rg_zcal_ctrl_tx, rg_zcal_ctrl_rx;
		uint8	ad_cal_comp_out_init;
		uint16	l3r26_temp, l3r25_temp, l4r22_temp; 
		int 	calibration_polarity;
		uint8	cnt=0, phyaddr;
			
		phyaddr = port_num + ephy_addr_base;	

	l3r25_temp = tcPhyReadLReg(FE_CAL_P0, 3, 25);				// design bug...it shall be global page register
	tcPhyWriteLReg(FE_CAL_P0, 3, 25, (0xc000));					// l3r25[15]:rg_cal_ckinv, [14]:rg_ana_calen, [13]:rg_rext_calen, [12]:rg_zcalen
	
		l3r25_temp = tcPhyReadLReg(port_num, 3, 25);
		tcPhyWriteLReg(port_num, 3, 25, (l3r25_temp|0x1000));		// l3r25[12]:rg_zcalen

	rg_zcal_ctrl = 0x20;  										// start with 0 dB
	l3r26_temp = (tcPhyReadLReg(FE_CAL_P0, 3, 26) & (~0x003f));  	// design bug...it shall be global page register
		tcPhyWriteLReg(FE_CAL_P0, 3, 26, (rg_zcal_ctrl));	// l3r26[5:0]:rg_zcal_ctrl // design bug...it shall be global page register



		all_ana_cal_status = allFEAnaCalWait(delay, port_num); 		// delay 20 usec
	if(all_ana_cal_status == 0)
	{
		all_ana_cal_status = ANACAL_ERROR;	
		printf(" FE R50 AnaCal ERROR! (init)   \r\n");
	}
	ad_cal_comp_out_init = (tcPhyReadLReg(FE_CAL_P0, 4, 23)>>4) & 0x1;	// l4r23[4]:ad_cal_comp_out, design bug...it shall be global page register
	if(ad_cal_comp_out_init == 1)
	{
		calibration_polarity = -1;
	}
	else
	{
		calibration_polarity = 1;
	}

	cnt = 0;
	while((all_ana_cal_status < ANACAL_ERROR)&&(cnt < 254))
	{
		cnt ++;
		
		rg_zcal_ctrl += calibration_polarity;
			tcPhyWriteLReg(FE_CAL_P0, 3, 26, (rg_zcal_ctrl));	// l3r26[5:0]:rg_zcal_ctrl, design bug...it shall be global page register
		
			all_ana_cal_status = allFEAnaCalWait(delay, port_num); 			// delay 20 usec

		if(all_ana_cal_status == 0)
		{
			all_ana_cal_status = ANACAL_ERROR;	
			printf(" FE R50 AnaCal ERROR! (%d)  \r\n", cnt);
		}
		else if(((tcPhyReadLReg(FE_CAL_P0, 4, 23)>>4)&0x1) != ad_cal_comp_out_init) // design bug...it shall be global page register
		{
			all_ana_cal_status = ANACAL_FINISH;	
		}
		else
		{
			if((rg_zcal_ctrl == 0x3F)||(rg_zcal_ctrl == 0x00))	
			{
				all_ana_cal_status = ANACAL_SATURATION;  // need to FT
				printf(" FE R50 AnaCal Saturation! (%d)  \r\n", cnt);
			}
				//else
					//printf(" FE-%d R50 ... (%d)(%d)(0x%x)(0x%x)(0x%x)	\r\n", phyaddr, cnt, ((tcPhyReadLReg(FE_CAL_P0, 4, 23)>>4)&0x1), rg_zcal_ctrl, tcPhyReadLReg(FE_CAL_P0, 3, 26));
		}
	}
		
		//printf(" FE R50 AnaCal zcal_ctrl (%d)  \r\n", (rg_zcal_ctrl));

		if(phyaddr == 11)
		{
		rg_zcal_ctrl_tx = ZCAL_TO_R50ohm_TBL_100[(rg_zcal_ctrl-2)];		// +2
		rg_zcal_ctrl_rx = ZCAL_TO_R50ohm_TBL_100[(rg_zcal_ctrl-8)];		// -4
		}
		else
		{
			rg_zcal_ctrl_tx = ZCAL_TO_R50ohm_TBL_100[(rg_zcal_ctrl+2)];		// +2
			rg_zcal_ctrl_rx = ZCAL_TO_R50ohm_TBL_100[(rg_zcal_ctrl-4)];	
		}
		//printf(" FE R50 AnaCal zcal_ctrl (0x%x)  \r\n", rg_zcal_ctrl);
	
	if((all_ana_cal_status == ANACAL_ERROR)||(all_ana_cal_status == ANACAL_SATURATION))
	{	
		rg_zcal_ctrl = 0x20;  // 0 dB
		//mtEMiiRegWrite(phyaddr, 0x1e, 0x00e0, (dev1e_e0_ana_cal_r5 | rg_zcal_ctrl));
	}
	else
	{
			printf(" FE-%d R50 AnaCal Done! (%d)(0x%x)	\r\n", phyaddr, cnt, rg_zcal_ctrl);
			FECal_R50_flag = 1;
	}
	
	//tcPhyWriteLReg(port_num, 4, 22, (l4r22_temp|(rg_zcal_ctrl<<8)));		// l4r22[14:8]:for R50_rsel TX
	tcPhyWriteLReg(port_num, 4, 22, ((rg_zcal_ctrl_tx<<8)));		// l4r22[14:8]:for R50_rsel TX
	l4r22_temp = tcPhyReadLReg(port_num, 4, 22);
	//tcPhyWriteLReg(port_num, 4, 22, ((rg_zcal_ctrl<<0)));		// l4r22[6:0] :for R50_rsel RX	
	tcPhyWriteLReg(port_num, 4, 22, (l4r22_temp|(rg_zcal_ctrl_rx<<0)));		// l4r22[6:0] :for R50_rsel RX		

		//tcPhyWriteLReg(FE_CAL_P0, 3, 26, l3r26_temp);	// design bug...it shall be global page register
	//l3r25_temp = tcPhyReadLReg(port_num, 3, 25);
		tcPhyWriteLReg(port_num, 3, 25, 0x0000);   // need to be
		tcPhyWriteLReg(FE_CAL_P0, 3, 25, 0x0000);

}

void	FECal_Tx_offset(uint8 port_num, uint32 delay)									// for EN7512 
{
	uint8	all_ana_cal_status;
	uint8	ad_cal_comp_out_init;
	uint16	l3r25_temp; 
	uint16	g4r21_temp, l0r30_temp, l4r17_temp;
	int 	calibration_polarity, tx_offset_temp;
	uint16	cal_temp;
	uint8	tx_offset_reg_shift; 
	uint8	cnt=0, phyaddr, tx_amp_cnt=0;
	
	phyaddr = port_num + ephy_addr_base;	

	tcPhyWriteLReg(port_num, 0, 0, 0x2100);
	tcPhyWriteLReg(port_num, 0, 26, 0x5200);		// fix mdi
	l3r25_temp = tcPhyReadLReg(FE_CAL_P0, 3, 25);
	tcPhyWriteLReg(FE_CAL_P0, 3, 25, (0x0000));
	tcPhyWriteLReg(FE_CAL_P0, 3, 25, (0x4800));				// l3r25[15]:rg_cal_ckinv, [14]:rg_ana_calen, [13]:rg_rext_calen, [11]:rg_txvos_calen
	tcPhyWriteLReg(port_num, 3, 25, (0x4c00));
	
	g4r21_temp = tcPhyReadGReg(port_num, 4, 21);
	tcPhyWriteGReg(port_num, 4, 21, (g4r21_temp|0x0800));	// g4r21[11]:Hw bypass tx offset cal, Fw cal

	l0r30_temp = tcPhyReadLReg(port_num, 0, 30);
	tcPhyWriteLReg(port_num, 0, 30, (l0r30_temp|0x02c0));	// l0r30[9], [7], [6], [1]
	
	tx_offset_temp = TX_AMP_OFFSET_0mV;
	tx_offset_reg_shift = 8;
	//l3r25_temp = tcPhyReadLReg(port_num, 3, 25);
	//tcPhyWriteLReg(port_num, 3, 25, (l3r25_temp|0x0400));	// l3r25[10]:rg_txg_calen
	tcPhyWriteGReg(port_num, 1, 26, (0x8000|DAC_IN_0V));	// g1r26[15], [9:0]

	tcPhyWriteLReg(port_num, 4, 17, (0x0000));
	l4r17_temp = tcPhyReadLReg(port_num, 4, 17);
	tcPhyWriteLReg(port_num, 4, 17, l4r17_temp|(tx_offset_temp<<tx_offset_reg_shift));	// l4r17[13:8]
		
	all_ana_cal_status = allFEAnaCalWait(40, port_num); 		// delay 20 usec
	if(all_ana_cal_status == 0)
	{
		all_ana_cal_status = ANACAL_ERROR;	
		printf(" FE Tx offset AnaCal ERROR! (init)  \r\n");
	}
		
	tcPhyReadLReg(FE_CAL_P0, 4, 23);
	tcPhyReadLReg(FE_CAL_P0, 4, 23);
	ad_cal_comp_out_init = (tcPhyReadLReg(FE_CAL_P0, 4, 23)>>4) & 0x1;	// l4r23[4]:ad_cal_comp_out
	if(ad_cal_comp_out_init == 1)
	{
		calibration_polarity = -1;
	}
	else
	{
		calibration_polarity = 1;
	}

	cnt = 0;
	if(port_num == 0)
		tx_amp_cnt = 1;
	else
		tx_amp_cnt = 0;
	while((all_ana_cal_status < ANACAL_ERROR)&&(cnt < 254))
	{
		cnt ++;
		
		//tx_offset_temp += calibration_polarity;
		if(tx_offset_temp >= 0)
		{
			cal_temp = tx_offset_temp;
		}
		else
		{
			cal_temp = (1<<(TX_AMP_OFFSET_VALID_BITS-1)) | abs(tx_offset_temp);
		}
		//l4r17_temp = tcPhyReadLReg(port_num, 4, 17);
		tcPhyWriteLReg(port_num, 4, 17, (cal_temp<<tx_offset_reg_shift));		// l4r17[13:8]
	
		tcPhyReadLReg(FE_CAL_P0, 4, 23);
		tcPhyReadLReg(FE_CAL_P0, 4, 23);
		all_ana_cal_status = allFEAnaCalWait(40, port_num);		// delay 20 usec

		if(all_ana_cal_status == 0)
		{
			all_ana_cal_status = ANACAL_ERROR;	
			printf(" FE Tx offset AnaCal ERROR! (%d)  \r\n", cnt);
		}
		else if(((tcPhyReadLReg(FE_CAL_P0, 4, 23)>>4)&0x1) != ad_cal_comp_out_init) 
		{
			tx_amp_cnt ++;
			if(tx_amp_cnt >= 2)
		{
			all_ana_cal_status = ANACAL_FINISH;	
		}
			tcPhyReadLReg(FE_CAL_P0, 4, 23);
			tcPhyReadLReg(FE_CAL_P0, 4, 23);
			ad_cal_comp_out_init = (tcPhyReadLReg(FE_CAL_P0, 4, 23)>>4) & 0x1;	
		}
		else
		{
			l4r17_temp = tcPhyReadLReg(port_num, 4, 17);
			//printf(" FE-%d Tx offset : offset(%d) pol(%d) cal(0x%x) \r\n", phyaddr, tx_offset_temp, calibration_polarity, cal_temp);
			if(((tx_offset_temp == (-31))||(tx_offset_temp == 31))&&(cnt>3))	
			{
				if(calibration_polarity == 1)
				{
					calibration_polarity = -1;
				}
				else
			{
					calibration_polarity = 1;
				}
				//all_ana_cal_status = ANACAL_SATURATION;  // need to FT
				//printf(" FE Tx offset AnaCal Saturation! (%d)  \r\n", cnt);
				//printf(" FE Tx offset AnaCal Saturation! (%d) (%x)(%x)(%x)(%x) (%x) (%x)\r\n", cnt, tcPhyReadLReg(0, 3, 25), tcPhyReadLReg(1, 3, 25), tcPhyReadLReg(2, 3, 25), tcPhyReadLReg(3, 3, 25), tcPhyReadLReg(0, 2, 30), tx_offset_temp);
				tx_offset_temp += calibration_polarity;
			}
			else
				tx_offset_temp += calibration_polarity;
		}
	}
		
	if((all_ana_cal_status == ANACAL_ERROR)||(all_ana_cal_status == ANACAL_SATURATION))
	{	
		tx_offset_temp = TX_AMP_OFFSET_0mV;
		l4r17_temp = tcPhyReadLReg(port_num, 4, 17);
		tcPhyWriteLReg(port_num, 4, 17, l4r17_temp|(tx_offset_temp<<tx_offset_reg_shift));
	}
	else
	{
		printf(" FE-%d Tx offset AnaCal Done! (%d)(%d)(0x%x)  \r\n", phyaddr, cnt, tx_offset_temp, cal_temp);
		FECal_tx_offset_flag = 1;
	}
	//tcPhyWriteGReg(port_num, 1, 26, 0x0000);	
	//l3r25_temp = tcPhyReadLReg(port_num, 3, 25);
	tcPhyWriteLReg(port_num, 3, 25, 0x0000);
	tcPhyWriteLReg(FE_CAL_P0, 3, 25, 0x0000);
	//tcPhyWriteLReg(port_num, 0, 30, 0x0000);
	
}

void	FECal_TxAmp(uint8 port_num, uint32 delay)									// for EN7512 
{
	uint8	all_ana_cal_status;
	uint8	ad_cal_comp_out_init;
	uint16	l3r25_temp; 
	uint16	l2r23_temp;
	int 	calibration_polarity;
	uint8	tx_amp_reg_shift; 
	uint8	tx_amp_temp, cnt=0, phyaddr, tx_amp_cnt=0;

	phyaddr = port_num + ephy_addr_base;	

	// *** Tx Amp Cal start ********************************************************************
	//tcPhyWriteLReg(port_num, 2, 30, 0x0005);

	tcPhyWriteLReg(port_num, 0, 0, 0x2100);
	tcPhyWriteLReg(port_num, 0, 26, 0x5200);					// fix mdi
	tcPhyWriteGReg(port_num, 2, 25, 0x10c0);					// 0x10c0? // 0x1020?

	tcPhyWriteGReg(port_num, 1, 26, (0x8000|DAC_IN_2V));		// g1r26[15], [9:0]
	tcPhyWriteGReg(port_num, 4, 21, (0x0800));   				// set default
	tcPhyWriteLReg(port_num, 0, 30, (0x02c0));					// 0x3f80  // l0r30[9], [7], [6], [1]
	tcPhyWriteLReg(port_num, 4, 21, (0x0000));

	tcPhyWriteLReg(FE_CAL_P0, 3, 25, (0xca00));		// 0xca00	// l3r25[15]:rg_cal_ckinv, [14]:rg_ana_calen, [13]:rg_rext_calen, [11]:rg_txvos_calen, [9]: rg_cal_refsel
	tcPhyWriteLReg(port_num, 3, 25, (0xca00));	 // 0xca00
	l3r25_temp = tcPhyReadLReg(port_num, 3, 25);			
	tcPhyWriteLReg(port_num, 3, 25, (l3r25_temp|0x0400));		// l3r25[10]:rg_txg_calen
	
	
	tcPhyWriteLReg(port_num, 2, 23, (0x0000));
	tx_amp_temp = 0x00; //0x20;										// start with 0 dB
	tx_amp_reg_shift = 0;
	//l2r23_temp = tcPhyReadLReg(port_num, 2, 23);
	tcPhyWriteLReg(port_num, 2, 23, (tx_amp_temp));	// 100M: l2r23[5:0]

	all_ana_cal_status = allFEAnaCalWait_txamp(delay, port_num);   // allen_20160616

        if(all_ana_cal_status == 0)
	{
		all_ana_cal_status = ANACAL_ERROR;	
		printf(" FE Tx amp AnaCal ERROR! (init)  \r\n");
	}
	
	tcPhyReadLReg(FE_CAL_P0, 4, 23);
	tcPhyReadLReg(FE_CAL_P0, 4, 23);
	ad_cal_comp_out_init = (tcPhyReadLReg(FE_CAL_P0, 4, 23)>>4) & 0x1;	// l4r23[4]:ad_cal_comp_out
	if(ad_cal_comp_out_init == 1)
	{
		calibration_polarity = -1;
		tx_amp_temp = 0x10;
	}
	else
	{
		calibration_polarity = 1;
	}
	tx_amp_temp += calibration_polarity;

	//printf(" FE Tx amp (%d)(%d)(%d) === \r\n", port_num, calibration_polarity, ad_cal_comp_out_init);	
	//printf(" (%x)(%x)(%x)(%x)(%x)(%x)\r\n", tcPhyReadLReg(port_num, 2, 30), tcPhyReadLReg(port_num, 0, 0), tcPhyReadLReg(port_num, 0, 26), tcPhyReadGReg(port_num, 2, 25), tcPhyReadLReg(0, 3, 25), tcPhyReadLReg(port_num, 3, 25));	
	//printf(" (%x)(%x)(%x)(%x)(%x)\r\n", tcPhyReadLReg(port_num, 0, 30), tcPhyReadLReg(port_num, 4, 21), tcPhyReadGReg(port_num, 4, 21), tcPhyReadLReg(port_num, 2, 23), tcPhyReadGReg(port_num, 1, 26));	

	cnt = 0;
	tx_amp_cnt = 0;
	while(all_ana_cal_status < ANACAL_ERROR)
	{
		tcPhyWriteLReg(port_num, 2, 23, (tx_amp_temp));	// 100M: l2r23[5:0]
		l2r23_temp = tcPhyReadLReg(port_num, 2, 23);
		
		cnt ++;

		tcPhyReadLReg(FE_CAL_P0, 4, 23);
		tcPhyReadLReg(FE_CAL_P0, 4, 23);
		all_ana_cal_status = allFEAnaCalWait_txamp(delay, port_num);      // allen_20160616

		if(((tcPhyReadLReg(FE_CAL_P0, 4, 23)>>4)&0x1) != ad_cal_comp_out_init) 
		{
		//	printf(" FE Tx amp final       (%d)(%d)(0x%x)  \r\n", cnt, ((tcPhyReadLReg(FE_CAL_P0, 4, 23)>>4)&0x1), tcPhyReadLReg(FE_CAL_P0, 4, 23));
			all_ana_cal_status = ANACAL_FINISH;	
			FECal_flag = 1;
		}
		//else
			//printf(" FE Tx amp cal (%d)(%d)(0x%x)(0x%x)  \r\n", cnt, ((tcPhyReadLReg(FE_CAL_P0, 4, 23)>>4)&0x1), tcPhyReadLReg(FE_CAL_P0, 4, 23), tcPhyReadLReg(port_num, 2, 23));
			
		
		if(all_ana_cal_status == 0)
		{
			all_ana_cal_status = ANACAL_ERROR;	
			printf(" FE Tx amp AnaCal ERROR! (%d)  \r\n", cnt);
		}
		else if(((tcPhyReadLReg(FE_CAL_P0, 4, 23)>>4)&0x1) != ad_cal_comp_out_init)
		{
			tx_amp_cnt ++; 
			//if(tx_amp_cnt >= 2)
			//{
			all_ana_cal_status = ANACAL_FINISH;	
			//}
			tcPhyReadLReg(FE_CAL_P0, 4, 23);
			tcPhyReadLReg(FE_CAL_P0, 4, 23);
			ad_cal_comp_out_init = (tcPhyReadLReg(FE_CAL_P0, 4, 23)>>4) & 0x1;
			//printf(" FE Tx amp final (%d)(%d)(%d) === \r\n", port_num, calibration_polarity, ad_cal_comp_out_init);
		}
		else
		{
			if(((l2r23_temp == 0x3f)||(l2r23_temp == 0x00))&&(cnt>5))	
			{
				#if 0
				if(l2r23_temp == 0x00)
				{
					calibration_polarity = 1;
				}
				else
			{
					calibration_polarity = -1;
				}
				#endif
				
				all_ana_cal_status = ANACAL_SATURATION;  // need to FT // need Mog comment
				printf(" FE Tx amp AnaCal Saturation!");
				//printf(" FE Tx amp AnaCal Saturation! (%d) (%x)(%x)(%x)(%x) (%x)\r\n", cnt, tcPhyReadLReg(0, 3, 25), tcPhyReadLReg(1, 3, 25), tcPhyReadLReg(2, 3, 25), tcPhyReadLReg(3, 3, 25), tcPhyReadLReg(0, 2, 30));
				//tx_amp_temp += calibration_polarity;
			}
			else
				tx_amp_temp += calibration_polarity;
			//printf(" FE Tx amp...(%d), l2r23(0x%x)  \r\n", cnt, l2r23_temp);
			}
		}

	if((all_ana_cal_status == ANACAL_ERROR)||(all_ana_cal_status == ANACAL_SATURATION))
	{	
		l2r23_temp = tcPhyReadLReg(port_num, 2, 23);
		//printf(" FE-%d Tx amp AnaCal Saturation! (%d)(0x%x)  \r\n", phyaddr, cnt, l2r23_temp);
		tcPhyWriteLReg(port_num, 2, 23, ((tx_amp_temp<<tx_amp_reg_shift))); 
		l2r23_temp = tcPhyReadLReg(port_num, 2, 23);
		//printf(" FE-%d Tx amp AnaCal Saturation! (%d)(0x%x)  \r\n", phyaddr, cnt, l2r23_temp);
	}
	else
	{	
		printf(" FE-%d Tx amp AnaCal Done! (%d)(0x%x)  \r\n", phyaddr, cnt, l2r23_temp);
		#if defined(TCSUPPORT_CPU_EN7512)  // allen_20160608
		if((port_num == 2)) // allen_20160603 : test for MSTC(7512 LAN2)
			tcPhyWriteLReg(port_num, 2, 23, ((l2r23_temp+2)<<tx_amp_reg_shift));
		else if((port_num == 3))
			tcPhyWriteLReg(port_num, 2, 23, ((l2r23_temp+1)<<tx_amp_reg_shift));
		else
		#endif
		{
#if defined(TCSUPPORT_CPU_EN7521)
			if(port_num == 3)	
				tcPhyWriteLReg(port_num, 2, 23, ((l2r23_temp-1)<<tx_amp_reg_shift));
			else
#endif
				tcPhyWriteLReg(port_num, 2, 23, ((l2r23_temp)<<tx_amp_reg_shift));
		}
		FECal_flag = 1;
	}
	
	tcPhyWriteLReg(port_num, 3, 25, 0x0000);
	tcPhyWriteLReg(FE_CAL_P0, 3, 25, 0x0000);
	// *** Tx Amp Cal end ***

}

uint8 allFEAnaCalWait(uint32 delay, uint8 port_num) // for EN7512 FE 
{
	uint8 	all_ana_cal_status;	
	uint16	l4r23_temp, cnt;
	
	tcPhyWriteLReg(FE_CAL_P0, 4, 23, (0x0000));
	//udelay(10000);
	l4r23_temp = tcPhyReadLReg(FE_CAL_P0, 4, 23);			// design bug...it shall be global page register
	tcPhyWriteLReg(FE_CAL_P0, 4, 23, (l4r23_temp|0x0004));	// da_calin_flag pull high, l4r23[2], design bug...it shall be global page register
	
	cnt = 1000;
	do{
		udelay(delay);
		cnt --;
		all_ana_cal_status = ((tcPhyReadLReg(FE_CAL_P0, 4, 23)>>6) & 0x1);  // l4r23[6], design bug...it shall be global page register
	}while ((all_ana_cal_status==0)&&(cnt!=0));    // allen_20160616
	//printf("=== (%d)(%d) ===\r\n", all_ana_cal_status, cnt);
	//delay1ms(delay);
	//printf(" (%x), l026(%x), g225(%x), g126(%x), g421(%x), l030(%x), l421(%x), l325(%x), l423(%x), l223(%x) \r\n", tcPhyReadLReg(port_num,0,0), tcPhyReadLReg(port_num,0,26), tcPhyReadGReg(port_num,2,25), tcPhyReadGReg(0,1,26), tcPhyReadGReg(port_num,4,21), tcPhyReadLReg(port_num,0,30), tcPhyReadLReg(port_num,4,21), tcPhyReadLReg(port_num,3,25), tcPhyReadLReg(0,4,23), tcPhyReadLReg(port_num,2,23));
	
	//tcPhyWriteLReg(FE_CAL_P0, 4, 23, (l4r23_temp&0xfffb));				// da_calin_flag pull low, l4r23[2], design bug...it shall be global page register
	tcPhyWriteLReg(FE_CAL_P0, 4, 23, (0x0000));

	return all_ana_cal_status;
}

uint8 allFEAnaCalWait_txamp(uint32 delay, uint8 port_num) // for EN7512 FE // allen_20160616
{
	uint8 	all_ana_cal_status;	
	uint16	l4r23_temp, cnt;
	
	tcPhyWriteLReg(FE_CAL_P0, 4, 23, (0x0000));
	//udelay(10000);
	l4r23_temp = tcPhyReadLReg(FE_CAL_P0, 4, 23);			// design bug...it shall be global page register
	tcPhyWriteLReg(FE_CAL_P0, 4, 23, (l4r23_temp|0x0004));	// da_calin_flag pull high, l4r23[2], design bug...it shall be global page register

	cnt = 1000;
	do{
		udelay(delay);
		cnt --;
		all_ana_cal_status = ((tcPhyReadLReg(FE_CAL_P0, 4, 23)>>6) & 0x1);  // l4r23[6], design bug...it shall be global page register
	}while ((all_ana_cal_status==0)&&(cnt!=0));
	//printf("=== (%d)(%d) ===\r\n", all_ana_cal_status, cnt);

	udelay((cnt*delay));  // workaround for 7512 FE tx amp calibration, it will fix in 7522
	udelay((cnt*delay));  // workaround for 7512 FE tx amp calibration, it will fix in 7522
	udelay((cnt*delay));  // workaround for 7512 FE tx amp calibration, it will fix in 7522
	
	//delay1ms(delay);
	//printf(" (%x), l026(%x), g225(%x), g126(%x), g421(%x), l030(%x), l421(%x), l325(%x), l423(%x), l223(%x) \r\n", tcPhyReadLReg(port_num,0,0), tcPhyReadLReg(port_num,0,26), tcPhyReadGReg(port_num,2,25), tcPhyReadGReg(0,1,26), tcPhyReadGReg(port_num,4,21), tcPhyReadLReg(port_num,0,30), tcPhyReadLReg(port_num,4,21), tcPhyReadLReg(port_num,3,25), tcPhyReadLReg(0,4,23), tcPhyReadLReg(port_num,2,23));
	
	//tcPhyWriteLReg(FE_CAL_P0, 4, 23, (l4r23_temp&0xfffb));				// da_calin_flag pull low, l4r23[2], design bug...it shall be global page register
	tcPhyWriteLReg(FE_CAL_P0, 4, 23, (0x0000));

	return all_ana_cal_status;
}



/* Register startup/shutdown routines */
module_init(tcPhyInit);
module_exit(tcPhyDeinit);  // JasonG_7512

