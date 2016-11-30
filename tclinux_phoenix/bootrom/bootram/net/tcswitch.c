#include <linux/errno.h>
#include <linux/init.h>
#include <linux/kernel_stat.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/random.h>
#include <asm/bitops.h>
#include <asm/bootinfo.h>
#include <asm/irq.h>
#include <asm/mipsregs.h>
#include <asm/system.h>
#include <linux/circ_buf.h>
#include <asm/tc3162.h>
#include <asm/io.h>

#include "skbuff.h"
#include "eth.h"

#if defined(TCSUPPORT_CPU_EN7512) ||defined(TCSUPPORT_CPU_EN7521)
#include "7512_eth.h"
#else
#include "tc3162l2mac.h"
#endif


#define TCPHY_4PORT 1
#ifdef RT63365
#define TC2105MJ_SUPPORT 1
#else
#define TC2104MC_SUPPORT 1
#endif
#define TCPHY_SUPPORT 1

#define DO_PER_PORT 1
#define DO_4_PORT 0
#define DO_5_PORT 2

#define SWIC_TC2206 7
#define tcPhyVer_2031 0 // 9400, LEH
#define tcPhyVer_2101mb  1 // 9401, LEM
#define tcPhyVer_2104mc  2 // 9402, tc2206
#define tcPhyVer_2104sd  3 // 9403, tc2104sd
#define tcPhyVer_2101me  4 // 9404, 62UE
#define tcPhyVer_2102me  5 // 9405, tc3182
#define tcPhyVer_2104me  6 // 9406, tc2206F
#define tcPhyVer_2101mf  7 // 9407, FPGA(MF)
#define tcPhyVer_2105mj  11 // 940b, RT63365
#define TCPHY_PORTNUM 4
#define mtPhyVer_7530    13 //MT7530
#define tcPhyVer_mt7510FE 15 //MT7510,FE(Fast Ethernet)
#define mtPhyVer_7510Ge  16 //MT7510Ge      

#define tcPhyVer_7512Fe		18	// 9412, EN7512Fe, need to check R15(0xf)[13]=0: no Giga capability
#define tcPhyVer_7512Ge		19	// 9451, EN7512Ge


#define TCPHYDISP1 if(tcPhy_disp_level>=1) printf
#define TCPHYDISP4 if(tcPhy_disp_level>=4) printf
#define TCPHYDISP5 if(tcPhy_disp_level>=5) printf

#define bool uint8 
#define ENABLE 1
#define DISABLE 0
#define tc2104mc_G0R22_Tx10AmpSave_ON  0x0e64
#define  tc2104mc_G0R22_Tx10AmpSave_OFF  0x066F

#define printf prom_printf

#define tcMiiStationRead(x, y)   miiStationRead(x, y)
#define tcMiiStationWrite(x, y, z) miiStationWrite(x, y, z)
// for tcXXXX_link_state
#define ST_LINK_DOWN 0
static uint32 eco_rev = 0x00;
extern macAdapter_t *mac_p;


static phyDeviceList_t phyDeviceList[] = {
	{ 0x0013, "LXT972" },
	{ 0x0022, "AC101L" },
	{ 0x0243, "IP101" },
	{ 0x8201, "RTL8201" },
	{ 0x03a2, "TC2031" }
};
static uint8	ephy_addr_base = 0;		// replace  "mac_p->enetPhyAddr"


#define MODEL_V0	(1<<0)	/*TC2206A1*/
#define MODEL_V1	(1<<1)	/*TC2206B2*/
#define MODEL_V2	(1<<2)	/*TC2206F*/

typedef struct swic_reg{
	uint16 model_id;
	uint16 phyaddr;
	uint16 regaddr;
	uint16 value;
}swic_reg_t;

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
	{MODEL_V0|MODEL_V1, 20, 2, 0xf20f},/*P0CR*/
	{MODEL_V0|MODEL_V1, 20, 3, 0xf20f},/*P1CR*/
	{MODEL_V0|MODEL_V1, 20, 4, 0xf20f},/*P2CR*/
	{MODEL_V0|MODEL_V1, 20, 5, 0xf20f},/*P3CR*/
	{MODEL_V0|MODEL_V1, 20, 6, 0xf20f},/*P4CR*/
	{MODEL_V0|MODEL_V1, 20, 7, 0xf20f},/*P5CR*/
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

static uint8 tcPhyFlag = 0;
static uint8 tcPhyVer = 99; // default: unknown PhyVer
static uint8 tcPhyPortNum = 1; // set value in tcPhyVerLookUp
static uint8 swicVendor = 0;
static uint8 esd_sw_patch=1;//default is enable ESD software patch
static uint8 sw_patch_flag = 1;
uint8 tcPhy_disp_level = 0; // default level 0
static uint8 current_idx = 0; // default 0, Do NOT change 
static uint8 sw_FixPdWakeup_flag = 1;/*Default is enable*/
static uint8 tcphy_link_state[TCPHY_PORTNUM];
static uint8 cfg_Tx10AmpSave_flag = 1; // default ENABLE
static uint8 sw_ErrOverMonitor_flag = 1; // default enable
static uint8 tc2104mc_FixNonAnPairSwap_flag = 1;// default: enable

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
	//uint8 parallel_detect_fault; // 4
	uint8 lp_np_able; // 3
	//uint8 np_able; // 2
	//uint8 lch_page_rx; // 1
	uint8 lp_autoneg_able; // 0
} tcphy_mr6_reg_t; // use for mr6

typedef struct {
	uint8  slicer_err_thd; // 15:11
	uint16 err_over_cnt;   // 10:0
	uint16 err_over_cnt_prev; // 10:0
} tcphy_l0r25_reg_t;

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

tcphy_mr0_reg_t mr0;
tcphy_mr4_reg_t mr4; 
tcphy_mr4_reg_t mr5;
tcphy_mr6_reg_t mr6;
tcphy_l0r28_reg_t mr28; // L0R28

// for multiple phy support
tcphy_mr1_reg_t Nmr1[TCPHY_PORTNUM];
tcphy_l0r25_reg_t Nmr25[TCPHY_PORTNUM];

// type for register settings
typedef struct cfg_data_s{
	uint32 reg_num;
	uint32 val;
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

#ifdef TC2206_SUPPORT
#define TC2104MC_PHY_INIT_GDATA_LEN 10
#define TC2104MC_PHY_INIT_LDATA_LEN 5
#define TC2104MC_PHY_INIT_SET_NUM 1

typedef struct tc2104mc_cfg_data_s{
	char name[10];
	cfg_data_t gdata[TC2104MC_PHY_INIT_GDATA_LEN];
	cfg_data_t ldata[TC2104MC_PHY_INIT_LDATA_LEN];
}tc2104mc_cfg_data_t;

// G0R22b11: tx10_eee_mode
// L0R30b11: bypass_pd_wakeup
// L0R30b3: pd_analog_wakeup
//const uint16 tc2104mc_L0R30_PDWAKEUP_ON = 0x8808;
const uint16 tc2104mc_L0R30_PDWAKEUP_OFF = 0x8800;
const uint16 tc2104mc_L0R30_PDWAKEUP_AUTO = 0x8000;

#define tc2104mc_FixTrcyStep_L1R18_default 0x240a // trgap=16T, trstep=4  --> 1/4
#define tc2104mc_G3R16_default 0x0018
#define tc2104mc_L0R25_default 0xa000

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

void tc2104meCfgLoad(uint8 idx, uint8 doPerPort_flag, uint8 port_num);
#endif
// note: The value of L0R25 and G3R16 are also used in tc2104mcInitialRegFlag()

uint32 
tcPhyReadReg(uint8 port_num, uint8 reg_num)
{
	uint32 val, val_r31;
	uint32 phyAddr = mac_p->enetPhyAddr + port_num;

	// page to L0
	val_r31 = tcMiiStationRead(phyAddr, 31); 
	tcMiiStationWrite(phyAddr, 31, 0x8000); 	
	// read reg
	val = tcMiiStationRead(phyAddr, reg_num); 
	// restore page if necessary
	if (val_r31 != 0x8000) {
		tcMiiStationWrite(phyAddr, 31, val_r31);
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


// read Global Reg
uint32
tcPhyReadGReg(uint8 port_num,uint8 page_num,uint8 reg_num){
	uint32 val, val_r31;
	uint32 phyAddr = mac_p->enetPhyAddr + port_num;
	uint32 pageAddr = (page_num<<12);

	// page to L0
	val_r31 = tcMiiStationRead(phyAddr, 31); 
	tcMiiStationWrite(phyAddr, 31, pageAddr); 	
	val = tcMiiStationRead(phyAddr, reg_num); 
	// restore page if necessary
	if (val_r31 != pageAddr) {
		tcMiiStationWrite(phyAddr, 31, val_r31);
	}
	
	return val;
}

void
tcPhyWriteGReg(uint8 port_num,uint8 page_num,uint8 reg_num,uint32 reg_data){
	uint32 val_r31;
	uint32 phyAddr = mac_p->enetPhyAddr + port_num;
	uint32 pageAddr = (page_num<<12);

	// page to L0
	val_r31 = tcMiiStationRead(phyAddr, 31); 
	tcMiiStationWrite(phyAddr, 31, pageAddr); 	
	tcMiiStationWrite(phyAddr, reg_num, reg_data); 
	// restore page if necessary
	if (val_r31 != pageAddr) {
		tcMiiStationWrite(phyAddr, 31, val_r31);
	}
}

// write Local Reg
void
tcPhyWriteLReg(uint8 port_num,uint8 page_num,uint8 reg_num,uint32 reg_data){
	uint32 val_r31;
	uint32 phyAddr = mac_p->enetPhyAddr + port_num;
	uint32 pageAddr = (page_num<<12)+0x8000;

	// page to L0
	val_r31 = tcMiiStationRead(phyAddr, 31); 
	tcMiiStationWrite(phyAddr, 31, pageAddr); 	
	tcMiiStationWrite(phyAddr, reg_num, reg_data); 
	// restore page if necessary
	if (val_r31 != pageAddr) {
		tcMiiStationWrite(phyAddr, 31, val_r31);
	}
}

#ifdef TC2206_SUPPORT
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

// G0R22b11: tx10_eee_mode
const uint16 tc2104me_G0R22_Tx10AmpSave_ON  = 0x0264;
const uint16 tc2104me_G0R22_Tx10AmpSave_OFF = 0x006F;

#define tc2104me_G3R23_default 0x48f0
#define tc2104me_L0R25_default 0xa000


tc2104me_cfg_data_t tc2104me_cfg[TC2104ME_PHY_INIT_SET_NUM]={
    //{{"F1.0"}, 
     //{ {31,0x3000},{16,0x001a},{23,0xabf0},{24,0x8801},
     //  {31,0x1000}
     //},
    {{"F11.0"},
     { {31,0x3000},{16,0x001a},{17,0x7138},{19,0x0100},{21,0x7386},{23,tc2104me_G3R23_default},{24,0x2201},
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


// 1. loading default register setting in tcPhyInit()
// 2. changing register setting
void 
tc2104meCfgLoad(uint8 idx, uint8 doPerPort_flag, uint8 port_num)
{
    int pn, i;
    uint16 phyAddr;

    current_idx = idx;
	

#ifdef UNH_TEST
        cfg_Tx10AmpSave_flag=0;
#endif

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
			if ((eco_rev == 0x00) || (eco_rev == 0x01)) { // bypass powerdown rx filter if eco_rev=0
			    tc2104mePerPortCfgLoad(current_idx,phyAddr);
			}
		 }
		}

	

    TCPHYDISP4("tcphy: CfgLoad %s\r\n",  tc2104me_cfg[current_idx].name);

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
#endif

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

    { {"J8.0"},
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

static int32 tc2105mj_erroversum=0;

void tc2105mjCfgLoad(uint8 idx, uint8 doPerPort_flag, uint8 port_num);
static void tc2105mjLRCfgLoad(uint8 idx, uint8 port_num);
static void tc2105mjCfgTx10AmpSave(void);
void tc2105mjGRCfgCheck(void);
void tc2105mjLRCfgCheck(uint8 port_num);
void tc2105mjCfgCheck(void);
int32 tc2105mjReadProbe(uint8 port_num, uint8 mode);
void tc2105mjSwPatch(void);

// 1. loading default register setting in tcPhyInit()
// 2. changing register setting
void 
tc2105mjCfgLoad(uint8 idx, uint8 doPerPort_flag, uint8 port_num)
{
    int pn,i;
    uint16 phyAddr;
	current_idx = idx;
	uint32 rev = 0x00;
	
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
	rev = tcMiiStationRead(0, 31);
    	rev &= (0x0f);
	// load revision-related settings
	if (rev == 0x00) { //rev=A1
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
	uint32 rev = 0x00;
	current_idx = idx;
	for( i=0; i<TC2105MJ_PHY_INIT_LDATA_LEN; i++ ){         
            tcMiiStationWrite(port_num, tc2105mj_cfg[current_idx].ldata[i].reg_num, tc2105mj_cfg[current_idx].ldata[i].val );
        }

	tcPhyWriteLReg(port_num,2,16,tc2105mj_L2R16[port_num]);
	
	rev = tcMiiStationRead(port_num, 31);
   	rev &= (0x0f);
    if (rev == 0x00) { //rev=A1
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
#endif

bool 
tc2104mcInitialRegFlag(void)
{
	bool initial_value_flag=1; // default: correct initial value
	uint8 pn;
	uint16 rval;

	// check G3R16
	rval = tcPhyReadGReg(0,3,16);
	if (rval != 0x0018){
		initial_value_flag=0; // wrong initial value
	}
	// check L0R25
	for(pn=0;pn<TCPHY_PORTNUM;pn++){
		rval = tcPhyReadReg(pn,25);
		if ((rval&0xf000)!=0xc000){
			initial_value_flag=0; // wrong initial value
		}
	}
	return (initial_value_flag);
}


bool getTcPhyInitialRegFlag(void){
	if (!sw_patch_flag)
		return 0;

	switch (tcPhyVer) {
#ifdef TC2104MC_SUPPORT 	
		case tcPhyVer_2104mc: // 2206
			return (tc2104mcInitialRegFlag());
			break;
#endif
	}
	return 0;
}

#ifdef TC2206_SUPPORT
static void
tc2104mcCfgTx10AmpSave(void)
{
	uint16 phyAddr = mac_p->enetPhyAddr;

	if (cfg_Tx10AmpSave_flag==ENABLE){
		tcPhyWriteGReg(phyAddr,0,22,tc2104mc_G0R22_Tx10AmpSave_ON);
		TCPHYDISP4("tcphy: Tx10AmpSave ENABLE!\r\n");		
	}
	else { // DISABLE
		tcPhyWriteGReg(phyAddr,0,22,tc2104mc_G0R22_Tx10AmpSave_OFF);
		TCPHYDISP4("tcphy: Tx10AmpSave DISABLE!\r\n");		
	}
}

const uint32 tc2104mc_G3R17_B1 = 0x4683;
const uint32 tc2104mc_G3R20_A1 = 0x0f30;
const uint32 tc2104mc_G3R20_B1 = 0x0e30;

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

#endif

#ifdef TC2102ME_SUPPORT
static uint8 sw_FixUp2DownFast_flag = 1;

#define TC2102ME_PHY_INIT_GDATA_LEN 9
#define TC2102ME_PHY_INIT_LDATA_LEN 7
#define TC2102ME_PHY_INIT_SET_NUM 1

typedef struct tc2102me_cfg_data_s{
    char name[10];
    cfg_data_t gdata[TC2102ME_PHY_INIT_GDATA_LEN];
    cfg_data_t ldata[TC2102ME_PHY_INIT_LDATA_LEN];
}tc2102me_cfg_data_t;

#define tc2102me_G3R17_default 0x7238
#define tc2102me_L0R25_default 0xa000

const uint16 tc2102me_G0R22_Tx10AmpSave_ON  = 0x0264;
const uint16 tc2102me_G0R22_Tx10AmpSave_OFF = 0x006F;

tc2102me_cfg_data_t tc2102me_cfg[TC2102ME_PHY_INIT_SET_NUM]={
    {{"G3.2"},
     { {31,0x3000},{17,tc2102me_G3R17_default},{21,0x7107},
       {31,0x1000},{20,0x0914},{25,0x0018},{27,0x0125},
       {31,0x0000},{23,0x0006}
     },
     { {31,0x9000},{16,0x0001},{22,0x0140},{23,0x0400},{24,0x4447},
       {31,0x8000},{25,tc2102me_L0R25_default}
     }
    }, 
};
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
#endif

#ifdef TC2031_SUPPORT	
static uint8 config_ldps = 0; // off, on

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
    //TCPHYDISP2("tcphy CfgLoad %s\r\n",  tc2031_cfg[current_idx].name);
}

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
#endif

#ifdef TC2101ME_SUPPORT
static uint8 tc2101me_FixNonAnPairSwap_flag = 1;// default: enable

#define TC2101ME_PHY_INIT_DATA_LEN 13
#define TC2101ME_PHY_INIT_SET_NUM 1

typedef struct tc2101me_cfg_data_s{
    char name[10];
    cfg_data_t data[TC2101ME_PHY_INIT_DATA_LEN];
}tc2101me_cfg_data_t;


tc2101me_cfg_data_t tc2101me_cfg[TC2101ME_PHY_INIT_SET_NUM]={
    {{"E5.2"}, 
	{{31,0x3000}, {17,0x7238}, {21,0x7247},
	 {31,0x1000}, {20,0x0914}, {25,0x0018}, {27,0x0125},
     {31,0x0000}, {23,0x0006},
	 {31,0x9000}, {22,0x0140}, {23,0x0400}, {24,0x4447}}}, 
};

const uint16 tc2101me_G0R22_Tx10AmpSave_ON  = 0x0264;
const uint16 tc2101me_G0R22_Tx10AmpSave_OFF = 0x0064;

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
  //TCPHYDISP3("tcphy: CfgLoad %s\r\n",  tc2101me_cfg[current_idx].name);
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
#endif 

#ifdef TC2101MB_SUPPORT 
//*** TC2101MB register settting ***//
#define TC2101MB_PHY_INIT_DATA_LEN 29
#define TC2101MB_PHY_INIT_SET_NUM 1

typedef struct tc2101mb_cfg_data_s{
    char name[10];
    cfg_data_t data[TC2101MB_PHY_INIT_DATA_LEN];
}tc2101mb_cfg_data_t;

#define tc2101mb_FixFalseSquelch_G0R16_default  0x040F
#define tc2101mb_FixTrcyStep_L1R18_default 0x240a // trgap=16T, trstep=4  --> 1/4

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
#ifdef TCPHY_PHY2PHY 
    tc2101mbCfgPhy2Phy();
#endif
    
}

#endif

/************************************************************************
*
*            Variables and Functions for 7530 GE PHY (ID=0x9412)
*
**************************************************************************/
#define MTPHY_PORTNUM 5


#define MT7530_PORTNUM 					5
#define MT7530_PHY_INIT_LDATA_LEN 		1
#define MT7530_PHY_INIT_PERPDATA_LEN 	0
#define MT7530_PHY_INIT_SET_NUM 		1

#define MT7530_PHY_INIT_CL45_GDATA_LEN 		1	// allen_20150206	
#define MT7530_PHY_INIT_CL45_LDATA_LEN 		1	// allen_20150206
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
			{0x1f,0x0417,0x7775}  // allen_20150206
		},
     	//local data
     	{
        	//for reference {0x1e,0x003D,0x0800},//modify Pair A DC for 100M
        	//{0x1e,0x00a6,0x0000} // disable TX power control //CML_20130321 //CML_20130327
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
	//printf("* doPhyMMDWrite_CL22=> phyaddr=%d, dev_addr=%d, data_addr=0x%04lX , value=0x%04lX\r\n", port_num, dev_num, reg_num, reg_data);
}

void mtEMiiRegWrite(uint32 port_num, uint32 dev_num, uint32 reg_num, uint32 reg_data)
{
	tcMiiExtStationWrite_CL22( port_num, dev_num, reg_num, reg_data);
}

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
    //printf("mtphy: CfgLoad %s\r\n",  mt7530_cfg[current_idx].name);
}

/************************************************************************
*
*            Variables and Functions for 7512 FE PHY (ID=0x9412, R15(0xf)[13]=0)
*
**************************************************************************/
#define MT7512FE_PHY_INIT_GDATA_LEN 	5	
#define MT7512FE_PHY_INIT_LDATA_LEN 	9
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
     		{30,0x0000},			// l0r30: tx pwd
     		{31,0x9000},			// l1
     		{22,0xf000},			// l1r22: for intel82579 EEE IOT
     		{31,0xa000},			// l2
     		{22,0x4444},			// l2r22: ps_dri (0x3444)
     		{28,0x7c44},			// l2r28: ps_op (0x6c44)
			{31,0xb000},  			// l3
			{17,0x0000}   			//disable EEE
     	},
     	{ 
     		{31,0x8000}
	 	} 
    }, 
};

static void mt7512FECfgTx10AmpSave(void)
{
    uint16 phyAddr = ephy_addr_base;

    if(cfg_Tx10AmpSave_flag==1)
	{ 	// enable
        tcPhyWriteGReg(phyAddr,0,22,mt7512FE_G0R22_Tx10AmpSave_ON);
        //printf("tcphy: Tx10AmpSave enable!\r\n");       
    }
    else 
	{ 	// disable
        tcPhyWriteGReg(phyAddr,0,22,mt7512FE_G0R22_Tx10AmpSave_OFF);
        //printf("tcphy: Tx10AmpSave disable!\r\n");      
    }
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

	//printf("phyaddr = %d \r\n", phyaddr);

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

    //printf("tcphy: CfgLoad %s\r\n",  mt7512FE_cfg[current_idx].name);
}


/************************************************************************
*
*            Variables and Functions for 7512 GE PHY (ID=0x9451)
*
**************************************************************************/

#define MT7512Ge_PORTNUM 				1
#define MT7512Ge_PHY_INIT_LDATA_LEN 	1
#define MT7512Ge_PHY_INIT_PERPDATA_LEN 	0
#define MT7512Ge_INIT_SET_NUM 			1
#define MT7512Ge_PHY_INIT_SET_NUM 		1

#define MT7512Ge_PHY_INIT_CL45_GDATA_LEN 	9 	
#define MT7512Ge_PHY_INIT_CL45_LDATA_LEN 	9 	 
#define MT7512Ge_PHY_INIT_CL45_PERPDATA_LEN 0
#define MT7512Ge_PHY_INIT_CL45_SET_NUM 		1

#define MT7512Ge_PHY_INIT_TR_LDATA_LEN 		0
#define MT7512Ge_PHY_INIT_TR_PERPDATA_LEN 	0
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
       		{0x09, 0x1e00}
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
			{0x1e,0x0002,0x01b4},	// z2p_ovs
			{0x1e,0x0005,0x0080},	// p2z_ovs
			{0x1e,0x0008,0x03b7},	// z2n_ovs
			{0x1e,0x000b,0x0002},	// n2z_ovs
			
			{0x1e,0x0014,0x0440},	// pair delay (afe)

			{0x1e,0x0012,0x2c10},	// 10 base Tx
			{0x1e,0x0017,0x0410},	// 10 base Tx
			
			{0x1e,0x00a6,0x0350},
			{0x07,0x003c,0x0000}
			//{0x1e,0x0120,0x8d0d},  // for 7510 E4 
			//{0x1e,0x0122,0xffa0},  // for 7510 E4
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
     	},

     	//per-port data
     	{ //{"PMA",0x20,0x0700}
	 	}
    }, 
};



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
	
		//printf("\n 7512Ge, phyaddr= (%d,%d) \n", phyAddr_base, phyAddr);
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

    //printf("mtphy: CfgLoad %s\r\n",  mt7512Ge_cfg[current_idx].name);
}


// tcPhy initial: reset, load default register setting, restat AN
int tcPhyInit(macAdapter_t * mac_p){
	int i=0;
	uint16 start_addr= mac_p->enetPhyAddr;

	//miiStationWrite( mac_p, PHY_CONTROL_REG, PHY_RESET );
	for( i=start_addr; i<start_addr+tcPhyPortNum; i++ ){			
		tcMiiStationWrite(i, PHY_CONTROL_REG, PHY_RESET );
	}

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
#ifdef TC2206_SUPPORT     
		case tcPhyVer_2104mc: // 2206
            tc2104mcCfgLoad(0, DO_4_PORT, 0);
			break;
#endif
#ifdef TC2104SD_SUPPORT 	
		case tcPhyVer_2104sd: // 2104sd
			tc2104sdCfgLoad(0);
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
#ifdef TC2206_SUPPORT      
	    case tcPhyVer_2104me: // 2206F
			tc2104meCfgLoad(0, DO_4_PORT, 0);
			break;
#endif
#ifdef TC2105MJ_SUPPORT     
		case tcPhyVer_2105mj: // RT63365
			tc2105mjCfgLoad(0, DO_4_PORT, 0);
			break;
#endif
        
		case mtPhyVer_7530: 		// MT7530
            mt7530GePhyCfgLoad(0, DO_5_PORT, 0);
		    break;
        case tcPhyVer_7512Fe:
            mt7512FECfgLoad(0, DO_4_PORT, 8);
            break;
            
        case tcPhyVer_7512Ge:
            mt7512FECfgLoad(0, DO_4_PORT, 8);
			mt7512GePhyCfgLoad(0);   
            break;
            
	    default:
	    	printf("Not found TC Phy\r\n");
	    	break;
	    	
	}

	// always boot-up with AN-ENABLE
	//miiStationWrite( mac_p, PHY_CONTROL_REG, MIIDR_AUTO_NEGOTIATE );
	for( i=start_addr; i<start_addr+tcPhyPortNum; i++ ){			
		tcMiiStationWrite(i, PHY_CONTROL_REG, MIIDR_AUTO_NEGOTIATE );
	}

	// tcphy_link_state init.
	for(i=0;i<TCPHY_PORTNUM;i++){
		tcphy_link_state[i]=ST_LINK_DOWN;
	}

	return 0;
}
int tcPhyPortNumGet(void){
	if(tcPhyVer != 99)
		return tcPhyPortNum;
	else
		return 0;
}


// set tcPhyFlag & tcPhyVer
int tcPhyVerLookUp(macAdapter_t *mac_p){
	uint32 rval;
    uint16 r15_val;
	tcPhyFlag = 1;
	rval = miiStationRead(mac_p->enetPhyAddr, 3); // phy revision id
	rval &= 0xffff;
	r15_val = miiStationRead(mac_p->enetPhyAddr, 0xF);
	if (rval==0x9400){
		tcPhyVer=tcPhyVer_2031; 		
		tcPhyPortNum=1;
		TCPHYDISP1("TC2031, ");
//		#if ( defined(TC2031_DEBUG) && !defined(ZYXEL_LEH) )
//			printf("TC2031_%s, ", tc2031_cfg[current_idx].name);
//		#endif
	} else if (rval==0x9401){
		tcPhyVer=tcPhyVer_2101mb;
		tcPhyPortNum=1;
		//printf("TC2101MB, ");
	} else if (rval==0x9402){
		tcPhyVer=tcPhyVer_2104mc;
		tcPhyPortNum=4;
		//printf("TC2104MC, ");
	} else if (rval==0x9403){
		tcPhyVer=tcPhyVer_2104sd;
		tcPhyPortNum=4;
		//printf("TC2104SD, ");
	} else if (rval==0x9404){
		tcPhyVer=tcPhyVer_2101me;
		tcPhyPortNum=1;
		//printf("TC2101ME, ");
	 } else if (rval==0x9405){
        tcPhyVer=tcPhyVer_2102me;
        tcPhyPortNum=2;
	 } else if (rval==0x9406){
        tcPhyVer=tcPhyVer_2104me;
        tcPhyPortNum=4;
	} else if (rval==0x9407){
        tcPhyVer=tcPhyVer_2101mf;
        tcPhyPortNum=1;
   } else if (rval==0x940b){	
        tcPhyVer=tcPhyVer_2105mj;
        tcPhyPortNum=5;
#if defined(TCSUPPORT_CPU_MT7520) || defined(TCSUPPORT_CPU_EN7512) || defined(TCSUPPORT_CPU_EN7521)
	} else if (rval==0x940F){	 
		 tcPhyVer=tcPhyVer_mt7510FE;
		 tcPhyPortNum=4;
		 printf("tcPhyVer_mt7510FE \n ");	 
	 //} else if (rval==0x0440){ 
	 } else if (rval==0x9412){ //CML_20130219, E2 PHY ID
        if(r15_val & 0x2000)
        {
            tcPhyVer=mtPhyVer_7530;
            tcPhyPortNum=1;
#if defined(TCSUPPORT_CPU_EN7512) || defined(TCSUPPORT_CPU_EN7521)
        }else{
            tcPhyVer=tcPhyVer_7512Fe;
            tcPhyPortNum=4;
#endif
	    } 
	 } else if (rval==0x9421){ 
		 tcPhyVer=mtPhyVer_7510Ge;
		 tcPhyPortNum=1;
		 
#endif

#if defined(TCSUPPORT_CPU_EN7512) || defined(TCSUPPORT_CPU_EN7521)
    }else if (rval==0x9451){
        tcPhyVer=tcPhyVer_7512Ge;
		 tcPhyPortNum=1;
#endif
#ifdef TCPHY_DEBUG
	} else {
		printf("unknown PHYID: %lx, ", rval);
#endif
	}
	return 0;
}


uint32
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
			if( companyId == 0x03a2 ){
				tcPhyVerLookUp(mac_p);
			} 
			return 1;
		}
	}
	return 0;
}

int32
macSearchPhyAddr(
	macAdapter_t *mac_p
)
{
	uint32 miiReg = 0;
	#ifdef TC3262
	int cnt=10000;
	do{
		if(miiStationRead(0 , 2)!=0xffff){
			break;
		}
		cnt--;
	}while(cnt>0);
	#endif
#if !(defined(TCSUPPORT_CPU_MT7520) ||	defined(TCSUPPORT_CPU_EN7512)||	defined(TCSUPPORT_CPU_EN7521))
	mac_p->enetPhyAddr = 0;
	for ( mac_p->enetPhyAddr = 0; mac_p->enetPhyAddr < 32; mac_p->enetPhyAddr++ )
#endif
	{
		miiReg = miiStationRead(mac_p->enetPhyAddr , PHY_ID_REG1);
		if (miiReg == 0)
			miiReg = miiStationRead(mac_p->enetPhyAddr, PHY_ID_REG2);
		if ( macPhyLookUp(mac_p, miiReg) ) {
			return 0;
		}
	}
#if !(defined(TCSUPPORT_CPU_MT7520) ||	defined(TCSUPPORT_CPU_EN7512)||	defined(TCSUPPORT_CPU_EN7521))

	mac_p->enetPhyAddr = 0x00000000; 
#endif
	return -1;
}
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
	if(isTC3162L2P2 | isTC3162L3P3 | isTC3162L4P4){
		val = tcMiiStationRead(22, 1) & 0xfffd;
		tcMiiStationWrite(22, 1, val);
	}
}/*end filedSwicDefVal*/

/*_____________________________________________________________________________
**      function name: macSetUpPhy
**      descriptions:
**           Detect the switch vender id and setup related mii register,such as
 **          cpu port's link up status.
**             
**      parameters:
**            mac_p: 
**         
**      global:
**            None
**             
**      return:
**            0
**	     
**      call:
**      	miiStationRead
**      	miiStationWrite
**      	
**      revision:
**      2009/05/08
**____________________________________________________________________________
*/
int
macSetUpPhy
(
	macAdapter_t *mac_p
)
{
	/* detect if RTL8305 or RTL8306 Switch*/	
	if ((miiStationRead(4, 2) == 0x1c) && (miiStationRead(4, 3) == 0xc852)) {
		if ((miiStationRead(6, 2) == 0x1c) && (miiStationRead(6, 3) == 0xc852) ){
    			/*swicVendor = SWIC_RTL8306SD;Let CPU Port Link up*/
    			miiStationWrite(5, 22,miiStationRead(5,22)|0x8000);
    			miiStationWrite(6, 22,miiStationRead(6,22)|0x8000);    		
        	}
	}
	tcPhyInit(mac_p);
	/*Switch Model Number Register (SMNR), Addr.: 16'h02FE*/
	if (tcMiiStationRead(31, 31) == 0x2206){
	    /*TC2206 switch IC and to filled the switch default value*/
	    swicVendor=SWIC_TC2206;
	    filedSwicDefVal();
	}


  return 0;
}/*end macSetUpPhy*/
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
				if(tcMiiStationRead(esd_chk_reg[i].phyaddr, esd_chk_reg[i].regaddr)!=tc2206_swic_reg[j].value){
					return 0;
				}
			}
		}
	
	}
	return 1;
}/*end getTC2206InitRegFlag*/

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
	uint8 ether_gpio=*(uint8 *)(FLASH_ETHER_GPIO);
	/*The esd software patch is only work at TC2206A1 version*/
	if((swicVendor==SWIC_TC2206) && esd_sw_patch && (miiStationRead(31,30)==0x0)){
		if((!getTC2206InitRegFlag()) ||(!getTcPhyInitialRegFlag())){
			printf("\r\nESD detected!!\r\n");
			/* ----- Hardware reset Ehernet phy chip, this address is defined by h/w engineer ----- */
			VPint(CR_GPIO_CTRL) |= (1<<(ether_gpio<<1));/*Example: gpio 9=(1<<(9*2))*/
			VPint(CR_GPIO_ODRAIN) |= (1<<ether_gpio);

			VPint(CR_GPIO_DATA) &= ~(1<<ether_gpio);
			pause(100);

			VPint(CR_GPIO_DATA) |= (1<<ether_gpio);
			pause(100);

			/*Reload the switch default value for hardware trap*/
			filedSwicDefVal();
			/*Reload the tce phy default value for afe settings and power issue*/
			tcPhyInit(mac_p);
		}
	}	
}/*end filedSwicDefVal*/

