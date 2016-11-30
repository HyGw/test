#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>

//#include <asm/tc3162/tc3162.h>

#include <asm/io.h>

#include "drv_comm.h"
#include "sim_hw_mtk.h"
#include "sim_operation.h"



#ifdef TCSUPPORT_AUTOBENCH
int sim_autobench_result =0;
EXPORT_SYMBOL(sim_autobench_result);
#endif


static unsigned char msg[300];
int sim_dbg_level = SIM_DBG_ERR;//SIM_DBG_TRACE  SIM_DBG_ERR SIM_DBG_NONE

void  sim_print(__u32 debugLevel, char *fmt,...){
	va_list ap;
	
	if (sim_dbg_level >= debugLevel)
	{
		va_start(ap, fmt);
		vsnprintf(msg, 256, fmt, ap);	
		printk(msg);
		va_end(ap);
	}

	return ;
}

#define SIM_HW_CB_HEAD 0x5A5A5A5A
#define SIM_HW_CB_TAIL 0xA5A5A5A5

#define SIM_MT6306_MAX_INTERFACES   	(2)
#define SIM_MT6306_REG_NUMBER   		(9)
#define SIM_MT6306_REG_CARDSS   		(8)
#define SIM_MT6306_REG_RST  			(0)
#define SIM_MT6306_REG_CLK      		(1)
#define SIM_MT6306_REG_DAT  			(2)
#define SIM_MT6306_REG_VCC  			(3)

#define DRV_SIM_MAX_LOGICAL_INTERFACE   (4)
#define MAX_SIM_CARD_NUM 				(1)
#define SIM_MT6302_MAX_MT6302       	(2)

#define KAL_TRUE						(1)
#define KAL_FALSE						(0)

#define ATR_RETRY 						(3)

#define FI_DEFAULT      				372
#define DI_DEFAULT      				1
#define INIT_WWT_T0     				(9600+400)  // etu  (initial work waiting time) +400 to cover some slow card
#define TOUT_OFFSET     				0x10        // apply a offset to all timeout settings (4*16 = 64 etu)
#define BGT_T1              			22      	// etu  (block guard time)
#define NAD                 			0           // node address byte
#define SIM_DEFAULT_TOUT_VALUE      	0x983
#define SIM_CMD_TOUT_VALUE          	0x1400

#define LEN_MIN_T1          			0
#define LEN_MAX_T1          			254
#define USIM_IFSC_DEFAULT   			32
#define USIM_IFSD_DEFAULT   			32
#define USIM_CWT_DEFAULT    			8203        // (11 + 1>>13) etu
#define USIM_BWT_DEFAULT    			15360       // (1<<4)*960
#define USIM_POW_CLASS_MSK  			0x3f        // TAi bit 1~6
#define USIM_PTS_PS1_MSK    			0x10
#define USIM_PTS_PS0_T1     			0x1     // select T1 protocol
#define USIM_NAD_DEFAULT   				0x0
#define USIM_RETRY          			3
#define USIM_IFSD_MAX					0xFE
#define SIM_TOTAL_FIFO_LEN  			16 

#define USIM_RETRY          			3
#define INDEX_COUNT         			4       // the count of the wline and sline
#define MAX_BWI             			9
#define MAX_CWI             			16
#define SIM_TOTAL_FIFO_LEN  			16      // excep 6208

#define HIST_FIRST_USIM     	        0x80    // the first of the historical character of USIM
#define HIST_SEC_USIM           	    0x31    // the second of the historical character of USIM
#define HIST_FOUR_USIM             	 	0x73    // the fourth of the historical character of USIM
#define USIM_DMA_MAX_SIZE           	260
#define USIM_GPT_TIMEOUT_PERIOD  		500     // x 10ms

#define ATR_TA1_372_5       			0x11
#define ATR_TA1_372_4       			0x01
#define ATR_TA1_64          			0x94
#define ATR_TA1_32          			0x95
#define ATR_TA1_16          			0x96
#define ATR_TA1_8           			0x97

/* ATR data define */
#define TAMask      					0x0010
#define TBMask      					0x0020
#define TCMask      					0x0040
#define TDMask      					0x0080

// coding of PCB for S-block (11xxxxxx)
#define PCB_S_DEFAULT   				0xc0
#define PCB_S_RESP      				0x20        // 1: a response, 0: a request
#define PCB_S_ID        	    		0x1f

/* SIM Format */
#define SIM_direct      				0
#define SIM_indirect    				1

#define SIM_MT6306_CONFMSK  			0x33B

#define RESET_18V						0  /* Driver reset result = 1.8V */
#define RESET_30V						1  /* Driver reset result = 3V */
#define SIM_NO_ERROR       	    	 	0   /* return value for SIM no error found */
#define SIM_NO_INSERT      	    	 	1   /* return value for no SIM insert */
#define SIM_CARD_ERROR     		     	3   /* return value for SIM error found */

/* SIM Power */
#define SIM_30V         	 			RESET_30V
#define SIM_18V          				RESET_18V

// SIM_BRR
#define  SIM_BRR_CLKMSK         	   	0x0003
#define  SIM_BRR_CLK_Div2      	    	0x0000
#define  SIM_BRR_CLK_Div4      	    	0x0001
#define  SIM_BRR_CLK_Div8      	    	0x0002
#define  SIM_BRR_CLK_Div12     	    	0x0003

#define CLOCK_STOP_AT_HIGH   			0x00   	/* config SIM colck stop at high */
#define CLOCK_STOP_AT_LOW    			0x01   	/* config SIM clock stop at low */
#define CLOCK_STOP_NOT_ALLOW 			0x02   	/* config SIM clock stop not allowed */
#define CHECK_CLOCK_MODE	 			0x0d

/* SIM State   */
#define  SIM_WAIT_FOR_ATR     			0     	/* reset SIM card and wait ATR */
#define  SIM_PROCESS_ATR      			1        	/* receiving ATR data */
#define  SIM_PROCESS_PTS    	  		2        	/* receiving PTS response data */
#define  SIM_PROCESSCMD       			3
#define  SIM_SERIOUSERR     	  		4        	/* sereous error due to txerr*/
#define  SIM_PWROFF           			5
#define  SIM_WaitRejectDone   			6

/* SIM Miner State   */
#ifdef NoT0CTRL
#define SIMD_CmdIdle    	  0
#define SIM_WaitProcByte      1
#define SIM_AckDataState      2
#define SIM_NAckDataState     3
#define SIM_WaitSWByte        4
#endif /*NoT0CTRL*/
/*just for clock stop mode*/
#define SIM_ProcessClk        5
#define SIM_StopClk           6
#define SIM_WaitCmdEnd        7

#define Speed372              0
#define Speed64               1
#define Speed32               2
#define Speed16               3
#define Speed8                4

/* Result */
#define SIM_SUCCESS              SIM_NO_ERROR
#define SIM_NOREADY              SIM_NO_INSERT
#define SIM_CARDERR              SIM_CARD_ERROR
#define SIM_INITXERR             5
#define SIM_INIPTSERR            6
#define SIM_CMDTXERR             7  /* parity error */
#define SIM_CMDRECERR            8
#define SIM_CMDTOUT              9
#define SIM_CLKPROC              10
#define SIM_NULLTIMEOUT 11
#define SIM_TS_INVALID           12
#define SIM_NO_ATR               13
#define SIM_RX_INVALID           14

/* Event */
#define  ATR_END           		0x0010
#define  PTS_END           		0x0008
#define  SIM_EVT_CMD_END        0x0004
#define  RST_READY     			0x0002
#define  CLK_PROC       		0x0020
#define  ACTIVATE_DONE  		0x0040
#define  DEACTIVATE_DONE    	0x0080

//SIM_IRQEN
#define  SIM_IRQEN_TX              	0x0001
#define  SIM_IRQEN_RX              	0x0002
#define  SIM_IRQEN_OV              	0x0004
#define  SIM_IRQEN_TOUT            	0x0008
#define  SIM_IRQEN_TXERR			0x0010
#define  SIM_IRQEN_NATR            	0x0020
#define  SIM_IRQEN_SIMOFF        	0x0040
#define  SIM_IRQEN_T0END         	0x0080
#define  SIM_IRQEN_RXERR         	0x0100

#define  SIM_IRQEN_ALL             	0x01bf
#define  SIM_IRQEN_ALLOFF          	0x0000
/* SIM_IRQEN_TXErr, SIM_IRQEN_NATR, SIM_IRQEN_TOUT,SIM_IRQEN_OV,SIM_IRQEN_RX*/
#define  SIM_IRQEN_Normal          	0x013e
/* SIM_IRQEN_T0END,SIM_IRQEN_TXErr,SIM_IRQEN_TOUT*/
#define  SIM_IRQEN_CMDNormal    	0x019e
/*#define    SIM_IRQEN_CMDNormal    0x019c*/
#define  SIM_IRQEN_CMDDMANormal     0x019c
#define  USIM_IRQEN_ATR              (SIM_IRQEN_RX|SIM_IRQEN_NATR)
#define  USIM_IRQEN_NORMAL           (SIM_STS_RX|SIM_STS_OV|SIM_STS_TOUT|SIM_STS_TXERR|SIM_STS_RXERR)
#define  USIM_IRQEN_CMD_T1           (SIM_STS_EDCERR|SIM_STS_T1END|SIM_STS_RXERR|SIM_STS_OV|SIM_STS_TOUT)
#define  USIM_IRQEN_CMD_T1_RX        (SIM_STS_EDCERR|SIM_STS_T1END|SIM_STS_OV|SIM_STS_TOUT|SIM_STS_RX)
#define  USIM_IRQEN_CMD_T1_TX        (SIM_STS_T1END)

#define SIM_ICC_MT6302_NONE 			0xffffffff

#define usim_power_enum					sim_power_enum
#define usim_speed_enum					sim_speed_enum
#define usim_clock_stop_enum        	sim_clock_stop_enum 
#define usim_protocol_enum 				sim_protocol_phy_enum

#define isEN751221 	(((SIM_Reg(0xbfb00064)&0xffff0000))==0x00070000)


/*****************************
 * Arbiter/Decoder Registers *
 *****************************/
#define CR_AHB_BASE       	0xBFB00000
#define CR_AHB_HWCONF       (CR_AHB_BASE + 0x8C)
#define isEN751221FPGA          ((SIM_Reg(CR_AHB_HWCONF)&(1<<29)) ? 0 : 1) //used for 7512/7521
#define isGenernalFPGA          ((SIM_Reg(CR_AHB_HWCONF)&(1<<31)) ? 1 : 0) //used for 63365/751020
#define isFPGA          (isEN751221 ? isEN751221FPGA : isGenernalFPGA )

#define USIM_IS_SBLOCK(pcb) 			((pcb&PCB_S_DEFAULT) == PCB_S_DEFAULT)

#define USIM_CAL_TD_COUNT(a,b)  \
		{\
			if(a & TAMask) b++;\
			if(a & TBMask) b++;\
			if(a & TCMask) b++;\
		}

static const kal_uint8  ClkStopTimeTable[3][2]=	{ {0,5}, {3,11}, {6,19} };
typedef unsigned char           UINT8;
typedef unsigned short          UINT16;
typedef unsigned int            UINT32;
typedef unsigned long long      UINT64;
typedef int                     INT32;
typedef long long               INT64;
typedef signed char             INT8;

typedef unsigned char           UCHAR;
typedef unsigned short          USHORT;
typedef unsigned int            UINT;
typedef unsigned long           ULONG;
typedef void                    VOID;

typedef unsigned short 			sim_status;

/* this is the prototype in MT6302 driver */
typedef void (*sim_MT6306_SPIWriter)(kal_uint32 chipNo, kal_uint16 data);

/* For sim hot plug callback function */
typedef void (*DCL_SIM_PLUG_OUT_CALLBACK)(kal_uint32 simIf);
typedef void (*DCL_SIM_PLUG_IN_CALLBACK)(kal_uint32 simIf);

typedef void (*sim_MT6306_I2CWriter_implementation)(kal_uint8 addr, kal_uint8 data_addr, kal_uint8 data_value);
typedef kal_uint8 (*sim_MT6306_I2CReader_implementation)(kal_uint8 device_addr, kal_uint8 data_addr);

typedef enum
{
	T0_PROTOCOL,
	T1_PROTOCOL,
	UNKNOWN_PROTOCOL
} sim_protocol_phy_enum;

/* define ATR data Structure */
typedef struct
{
	kal_uint8          info [40];
} AtrStruct;

typedef struct{
	kal_uint32  head;
	/*
		Here defines MTK related HW information of this logical interface, 
		these values are defined as constant in old driver.
		Now we make it variable.
	*/
	kal_uint32  mtk_baseAddr;
	kal_uint32  mtk_pdnAddr;
	kal_uint32  mtk_pdnBit;
	kal_uint32  mtk_pdnDevice;
	kal_uint32  mtk_dmaMaster;
	kal_uint32  mtk_lisrCode;
	/* 
	    in multiple SIM drivers, simInterface is used in all most 
	    all functions, we need record this information.
	*/

	kal_uint32  simInterface; 
	/*  
		The logical number. This value now can be 0~n, not limted as 0~1 before. 
		We can assume it less than 2 now 
	*/
	kal_uint32  MT6302ChipNo;   	/* record which MT6302 switch used for this card */
	kal_uint32  MT6302PortNo;   	/* record which port of MT6302 is used for this card */
	void *MT6302PeerInterfaceCb;	/* MT6302 need peer's information, so we 
								   	   have to maintain a way to find its peer*/
	kal_uint32  simSwitchChipNo;
	kal_uint32  simSwitchPortNo;
	void        *simSwitchPeerInterfaceCb;
	void        *simSwitchPeerInterfaceCb1;
	void        *simSwitchPeerInterfaceCb2;
	kal_uint32 	debounceTime; /* hot swap EINT debounce time */
	kal_uint32  tail;
	kal_bool 	polarity; /* hot swap EINT poarity */
	kal_bool    IsCardRemove;
	kal_uint8 	smHandler;
} sim_HW_cb;

typedef enum
{
	sim_card_normal_speed = 0,
	sim_card_enhance_speed_64,
	sim_card_enhance_speed_32,
	sim_card_enhance_speed_16,
	sim_card_enhance_speed_8
} sim_card_speed_type;

typedef enum
{
	MTK_SIMIF0,
	MTK_SIMIF1
} SIM_hwCtrlInfo;

typedef enum
{
	SIM_SWITCH_NONE,
	SIM_SWITCH_6302,
	SIM_SWITCH_6306,
	SIM_SWITCH_6314
} SIM_ICC_SWITCHCONTROL;

typedef enum{
	sim_MT6306_eventReset = 0,
	sim_MT6306_eventCommand,
	sim_MT6306_eventPowerOff
} sim_MT6306_changeEvent;

typedef enum{
	sim_MT6306_statusNoError = 0
} sim_MT6306_status;

typedef enum
{
	SIM_ICC_APPLICATION_PHONE1 = 0x00000000, //in sim driver UT, we play a trick on this enum
	SIM_ICC_APPLICATION_PHONE2,
	SIM_ICC_APPLICATION_CMMB_SMD,
	SIM_ICC_APPLICATION_PHONE3,
	SIM_ICC_APPLICATION_PHONE4
} SIM_ICC_APPLICATION;

typedef struct
{
	kal_uint32 logicalNum;
	SIM_ICC_APPLICATION application;
	SIM_hwCtrlInfo hwCtrl;
	kal_uint8 needMT6302;
	kal_uint8 MT6302ChipNum; /*whip MT6302, if there are lots*/
	kal_uint8 MT6302PortNum; /*whip interface of this MT6302*/
	kal_uint8 hotSwap; /* support hot swap or not */
	kal_bool polarity; /* hot swap EINT poarity */
	kal_uint32 debounceTime; /* hot swap EINT debounce time */
} SIM_ICC_HW_SW_MAPPING;

typedef struct{
	kal_uint32      	    MT6306ChipNo; //for which MT6306 chip does this control block stand
	sim_MT6306_SPIWriter    sim_MT6320_writer;
	kal_bool                sim_MT6306_taskAccessing;
	kal_bool                sim_MT6306_needCLKStartTimeout;
	kal_uint8               sim_MT6306_regValue[SIM_MT6306_REG_NUMBER];
	kal_bool                sim_MT6306_needManualControl; /*this flag will only be set true in MT6306*/
	kal_bool                sim_MT6306_initialized;
} sim_MT6306_switchInfo;

typedef enum{
	USIM_NO_ERROR = 0,

	// expected status
	USIM_WAITING_EVENT = 1, // initial wait event status
	USIM_BLOCK_REC = 2,     // successfully received a complete block
	USIM_POWER_OFF = 3,     // successfully powered off
	USIM_ATR_REC = 4,       // successfully reveived all ATR
	USIM_S_BLOCK_REC = 5,   // successfully reveived S RESP

	// error status
	USIM_NO_INSERT = -1,
	USIM_VOLT_NOT_SUPPORT = -2,
	USIM_NO_ATR = -3,
	USIM_TS_INVALID = -4,
	USIM_ATR_ERR = -5,
	USIM_INVALID_ATR = -6,
	USIM_PTS_FAIL = -7,
	USIM_RX_INVALID = -8,   // EDC error or parity error
	USIM_BWT_TIMEOUT = -9,
	USIM_DATA_ABORT = -10,
	USIM_DEACTIVATED = -11,
	USIM_S_BLOCK_FAIL = -12,
	USIM_INVALID_WRST = -13,
	USIM_GPT_TIMEOUT = -14
} usim_status_enum;

typedef enum{
	sim_MT6306_CLKBlockedLow = 0,
	sim_MT6306_CLKPass = 1,
	sim_MT6306_CLKBlockedHigh = 5,
	sim_MT6306_CLKInvalidState = 4,

	/*in following state, pin is stay in low lv because no ldo supply*/
	sim_MT6306_NoLDO_CLKBlockedLow = 0x10,
	sim_MT6306_NoLDO_CLKPass = 0x11,
	sim_MT6306_NoLDO_CLKBlockedHigh = 0x15,
	sim_MT6306_NoLDO_CLKInvalidState = 0x14
} sim_MT6306_stateCLK;

typedef enum{
	sim_MT6306_DATBlockedHigh = 0,
	/* Data is now pass and controlled by the side that driving, but DATA_L is 0, 
	   that is, when we block it will become high */
	sim_MT6306_DATPass_High = 1,    
	sim_MT6306_DATBlockedLow = 5,
	/* Data is now block high but DATA_L is 1, that is, when we set DATAEN, it will become block low */
	sim_MT6306_DATBlockedHigh_Low = 4,
   
	/* in following state, pin is stay in low lv because no ldo supply */
	sim_MT6306_NoLDO_DATBlockedHigh = 0x10,
	sim_MT6306_NoLDO_DATPass_High = 0x11,
	sim_MT6306_NoLDO_DATBlockedLow = 0x15,
	sim_MT6306_NoLDO_DATBlockedHigh_Low = 0x14
} sim_MT6306_stateDAT;

typedef enum{
	sim_MT6306_RSTPass_Low = 0, //RSTVAL is low, that is, when we block RST, it will becomes block low
	sim_MT6306_RSTBlockedLow = 1,
	sim_MT6306_RSTBlockedHigh = 5,
	sim_MT6306_RSTPass_High = 4,//RSTVAL is high, that is, when we block RST, it will becomes block high

	/*in following state, pin is stay in low lv because no ldo supply*/
	sim_MT6306_NoLDO_RSTPass_Low = 0x10,
	sim_MT6306_NoLDO_RSTBlockedLow = 0x11,
	sim_MT6306_NoLDO_RSTBlockedHigh = 0x15,
	sim_MT6306_NoLDO_RSTPass_High = 0x14
} sim_MT6306_stateRST;

  	/* MT6306 's pin state is more complicated, if I don't describe it precisely, 
	   I may encounter struggle in the future */
typedef enum {
	sim_MT6306_VCC18VDisable = 0,
	sim_MT6306_VCC3VDisable = 1,
	/* I plan to use ((reg_value >> simInterface) & 0x5) to assign the pin state value, 
	   thus the enum should be the same as reg value */
	sim_MT6306_VCC18V = 4, 
	sim_MT6306_VCC3V = 5
} sim_MT6306_stateVCC;

typedef enum
{
	ME_UNKNOW =0,
	ME_18V_30V,
	ME_30V_ONLY,
	ME_18V_ONLY
} sim_env;

typedef struct {
	sim_MT6306_stateCLK CLK;
	sim_MT6306_stateDAT DAT;
	sim_MT6306_stateRST RST;
	sim_MT6306_stateVCC VCC;
} sim_MT6306_pinStatus;

typedef enum {
	sim_MT6306_stateDeactiavate = 0,
	sim_MT6306_stateClkRunning = 1,
	sim_MT6306_stateClkStopped = 2
} sim_MT6306_cardState;

typedef enum{
	sim_MT6306_cardTypeAL = 0,
	sim_MT6306_cardTypeIR = 1,
	sim_MT6306_cardTypeMaxValue
} sim_MT6306_cardType;

typedef struct{
	sim_MT6306_pinStatus    pins;
	sim_MT6306_cardState    state;
	sim_MT6306_cardType 	type;
//	sim_MT6306_LISRState    lisrState;
	kal_uint16              baud;
	kal_uint16              config;
	kal_bool                sim_MT6306_clkStopQueued;
} sim_MT6306_cardInfo;

typedef enum{
	UNKNOWN_POWER_CLASS = 0,
	CLASS_A_50V 		= 1,
	CLASS_B_30V 		= 2,
	CLASS_AB			= 3,
	CLASS_C_18V 		= 4,
	ClASS_BC			= 6,
	CLASS_ABC   		= 7,
	CLASS_ALLSUPPORT	= 0xff
} sim_power_enum;

typedef enum{
	CLOCK_STOP_NOT_SUPPORT  = 0x0,
	CLOCK_STOP_LOW          = 0x40,
	CLOCK_STOP_HIGH         = 0x80,
	CLOCK_STOP_ANY          = 0xc0,
	CLOCK_STOP_MSK          = 0xc0,
	CLOCK_STOP_UNKONW       = 0x0f
} sim_clock_stop_enum;

typedef enum {
	SPEED_372,
	SPEED_64,
	SPEED_32,
	SPEED_16,
	SPEED_8,
	SPEED_MAX
} sim_speed_enum;

typedef enum
{
	SIM_PROTOCOL,
	USIM_PROTOCOL
} sim_protocol_app_enum;

typedef struct{
	sim_power_enum power;
	sim_speed_enum speed;
	sim_clock_stop_enum clock_stop;
	sim_protocol_app_enum app_proto;
	sim_protocol_phy_enum phy_proto;
	kal_bool T0_support;    // if T0 is supported
	kal_bool T1_support;    // if T1 is supported
	kal_uint8 hist_index; // index to the historical char of ATR
	kal_uint8 *ATR;
	/*following information is necessary for SIM task for UICC identification*/
	kal_bool TAiExist; //if the first TA for T=15 is existed
	kal_uint8 ATR_length;           // length to the ATR_data
	kal_bool isSW6263; // query if status word 0x62xx 0x63xx happen
} sim_info_struct;

typedef enum{
	USIM_CMD_READY,
	I_BLOCK_RX,
	I_BLOCK_TX,
	I_BLOCK_M0_RX,
	I_BLOCK_M0_TX,
	I_BLOCK_M1_RX,
	I_BLOCK_M1_TX,
	R_BLOCK_RX,
	R_BLOCK_TX,
	S_BlOCK_REQ_RX,
	S_BlOCK_REQ_TX,
	S_BlOCK_RESP_RX,
	S_BlOCK_RESP_TX
} usim_cmd_state_enum;

typedef enum{
	RESYNC_REQ = PCB_S_DEFAULT,
	IFS_REQ = (PCB_S_DEFAULT|1),
	ABORT_REQ = (PCB_S_DEFAULT|2),
	WTX_REQ = (PCB_S_DEFAULT|3),
	RESYNC_RESP = RESYNC_REQ|PCB_S_RESP,
	IFS_RESP = IFS_REQ|PCB_S_RESP,
	ABORT_RESP = ABORT_REQ|PCB_S_RESP,
	WTX_RESP = WTX_REQ|PCB_S_RESP
} usim_s_block_id_enum;


typedef struct
{
	kal_uint8    State;
	kal_uint8    Data_format;      /*SIM_direct,SIM_indirect*/
	kal_uint8    Power;            /*SIM_3V,SIM_5V*/
	kal_uint8    recData[40];     /*PTS or ATR data*/
	kal_bool     recDataErr;
	kal_uint8    result;           /* for ATR, command, RST   */
	sim_env      SIM_ENV;
#ifndef SIM_ADDDMA
	kal_uint8    *txbuffer;        /* only used for no DMA */
	kal_uint16   txsize;           /* only used for no DMA */
	kal_uint16   txindex;          /* only used for no DMA */
	kal_uint8    *rxbuffer;        /* only used for no DMA */
#ifdef   NoT0CTRL
	kal_uint8    INS;
	kal_uint8    SW1;
	kal_uint8    SW2;
	kal_uint16   recsize;
#endif	/*NoT0CTRL*/
#endif   /*SIM_ADDDMA*/

/*add for clock stop mode*/
	kal_uint8    cmdState;         /* only used for no T0CTRL, and for clock stop */
	kal_uint8    Speed;            /*Speed372,Speed64,Speed32*/
   kal_bool     clkStop;          /*Clok Stop Enable*/
   kal_bool     clkStopLevel;     /*Clok Stop level*/
   kal_bool     reject_set_event;
   kal_bool     event_state;
   kal_uint8    initialPower;
   sim_card_speed_type  sim_card_speed;

  	sim_protocol_app_enum 	app_proto;
  	kal_bool 				timeout;
// 	usim_cmd_case_enum cmd_case;
	kal_bool 				is_err;	// sim command has error once.
	kal_bool				get9000WhenSelect;

	/*
		following variables are get from global variables for 2 SIM projects.
		We won't use these in single SIM driver, but we need to define the power set.
	*/
  	kal_uint8		TOUT_Factor;
  	kal_uint8		sim_dmaport;
  	kal_uint8		reset_index;
  	kal_bool		TS_HSK_ENABLE;
  	kal_bool		sim_ATR_fail;
  	kal_bool		PTS_check;
  	kal_uint8		PTS_data[4];
	kal_uint8		sim_nullByteIssueNullCount;
	kal_bool		keepAtrFatal;
	kal_uint16   	recDataLen;       /* for command, ATR process   */
	kal_uint32   	EvtFlag;
  	kal_uint32		TOUTValue;
	kal_uint32		sim_nullByteIssueGPT;

/*	magic1 is an increasing counter, increases when 
	1) start new command, 
	2)get SIM timeout, 
	3)get T0 end
	for case 2 and 3, it means that one of the ends of SIM commands has appeared. */

	kal_uint32	simMagic1;

/* 	magic2 is used to compared with magic1 every time GPT expires. 
	It is set to magic1 in the start of a new command, if they were 
	compared equally in GPT timer, we know that we are still waiting 
	for SIM controller's event. */

	kal_uint32		simMagic2;
	kal_bool	   	clkstoping;
    sim_power_enum 	power_class;		// supported power class indicated at ATR	
} Sim_Card;

typedef usim_status_enum(*SIM_API_RESET)(sim_power_enum ExpectVolt, sim_power_enum *ResultVolt, kal_bool warm, sim_HW_cb *hw_cb);
typedef sim_status (*SIM_API_CMD)(kal_uint8 *txData,kal_uint32 *txSize, kal_uint8 *rxData, kal_uint32 *rxSize, sim_HW_cb *hw_cb);
typedef void (*SIM_API_PWROFF)(sim_HW_cb *hw_cb);
typedef void (*SIM_API_CARDINFO)(sim_info_struct *info, sim_HW_cb *hw_cb);
//typedef void (*SIM_API_ENHANCED_SPEED)(kal_bool enable, sim_HW_cb *hw_cb);
//typedef void (*SIM_API_ENHANCED_SELECT_PHY)(sim_protocol_phy_enum T, sim_HW_cb *hw_cb);
typedef kal_bool (*SIM_API_SET_CLKSTOP)(sim_clock_stop_enum mode, sim_HW_cb *hw_cb);
typedef void (*SIM_API_EOC)(sim_HW_cb *hw_cb);
typedef void (*SIM_API_MSG)(kal_uint32 tag, kal_uint32 event, kal_uint32 data1, kal_uint32 data2);
typedef void (*SIM_API_TOUT_TEST)(kal_uint32 toutValue, sim_HW_cb *hw_cb);

typedef struct
{
	SIM_API_RESET reset;
	SIM_API_CMD command;
	SIM_API_PWROFF powerOff;
	SIM_API_CARDINFO getCardInfo;
	SIM_API_SET_CLKSTOP setClockStopMode;
	/* use this to hook necessary action before return to SIM task, this is called by adaption layer, 
	   not SIM task */
	SIM_API_EOC EOC;
	SIM_API_MSG addMessage;
	SIM_API_TOUT_TEST toutTest;
} sim_ctrlDriver;

typedef enum {
	IDLE_STATE,
	ACTIVATION_STATE,
	ATR_STATE,
	PTS_STATE,
	MAIN_CMD_READY_STATE,
	CMD_TX_STATE,
	//CMD_RX_HEADER_STATE,
	CMD_RX_BLOCK_REC_STATE,
	//CMD_RX_S_BLOCK_STATE,
	CMD_RX_STATE,
	CLK_STOPPING_STATE,
	CLK_STOPPED_STATE,
	DEACTIVATION_STATE
} usim_main_state_enum;

typedef enum {
	USIM_RESET_NEGOTIABLE,  // type 1
	USIM_RESET_SPECIFIC     // type 2
} usim_reset_type_enum;

typedef enum {
	USIM_DIRECT,
	USIM_INVERSE
} usim_dir_enum;

typedef enum{
	PPSS = 0,   // initial character 0xFF
	PPS0 = 1,   // format character 0x1x
	PPS1 = 2,   // indicate the baudrate F, D
	PCK  = 3,   // exclusive-or PPSS to PCK should be null
	PPS_LEN = 4
} usim_pts_enum;

typedef struct
{
#if 1
	usim_power_enum power;			// power class used
	usim_status_enum status;
	usim_speed_enum speed;		// speed selected
	// state control
	volatile usim_main_state_enum main_state;
	volatile usim_status_enum ev_status;
	// informations
	usim_dir_enum dir;	// convention of character frame
 	sim_protocol_app_enum app_proto;	// application protocol (USIM, SIM)
 	sim_protocol_phy_enum phy_proto; // protocol type selected (physical layer)

	// ATR info
	kal_uint8 ATR_data[36];		// used to store all ATR data string
	kal_uint8 ATR_index;			// index to the ATR_data
	kal_uint8 header_tx[4], header_tx_bak[4]; // header_tx_bak used to backup the previous command
	kal_uint8 header_rx[4];
	kal_uint8 retry;

	sim_env sim_env;		// the voltage which MS can supply
	usim_power_enum power_in;		// expected power class input form application layer
	usim_power_enum power_class;		// supported power class indicated at ATR
	kal_bool	clock_stop_en;		// clock_stop is enabled or not
	usim_protocol_enum T;
	usim_speed_enum card_speed; // TA1, max speed card can support
	kal_bool high_speed_en;		// control if high speed is enalbed
	usim_clock_stop_enum clock_stop_type;
	kal_bool present;
	usim_reset_type_enum reset_mode; // specific or negotiable mode
	kal_bool warm_rst;	// KAL_TRUE: it's a warm reset, KAL_FALSE: a cold reset
	kal_bool T0_support;	// if T0 is supported
	kal_bool T1_support;	// if T1 is supported
	kal_uint8 Di;

	// T=1
	kal_uint8 ns;		// sequence # of sending
	kal_uint8 nr;		// sequence # of receiving
	kal_uint8 ifsd;	// information size of interface device
	kal_uint8 ifsc;	// information size of card
	usim_cmd_state_enum cmd_state;
	usim_cmd_state_enum cmd_state_bak;
	kal_bool abort;
	kal_bool wtx;	// waiting time extension
	kal_bool resync;
  	kal_bool send_prev;	// send the previous block
	kal_bool tx_chain;
	kal_bool rx_chain;
	kal_uint8 *tx_buf;
	kal_uint8 *rx_buf;
	kal_uint8 sw[2];		// used to contain SW1 and SW2
	kal_uint8 wtx_m;		// multiplier of BWT

	// others
	kal_bool ts_hsk_en;	// enable handshake at TS byte (error signal and char repetition)
	kal_uint8 dma_port;
  	kal_uint8 hist_index;		// index to the historical characters
// 	usim_cmd_case_enum cmd_case;
//	sim_protocol_phy_enum perfer_phy_proto; // protocol type selected (physical layer)
	/*SIM task need following information for UICC identification*/
	kal_bool TAiExist; //if the first TA for T=15 is existed

#if defined(USIM_DEBUG)
	kal_int32 sline[INDEX_COUNT];	// set event at the which line in usim_drv.c
	kal_uint32 sindex;	// index to the sline[4]
	kal_int32 wline[INDEX_COUNT];	// wait event at the which line in usim_drv.c
	kal_uint32 windex;	// index to the wline
#endif

	kal_uint16 tx_size;
	kal_uint16 rx_size;
	kal_uint16 tx_index;
	kal_uint16 rx_index;

	kal_uint16 Fi;
	kal_uint16 etu_of_1860;
	kal_uint16 etu_of_700;
	kal_uint32 gpt_handle;

	// time out control
	kal_uint32 WWT;	// work waiting time (T0)
	kal_uint32 CWT;	// character waiting time in etu(T1)
	kal_uint32 BWT;	// blcok waiting time in etu(T1)
	kal_uint32 timeout;	// etu
	kal_uint32 ev_flag;
	kal_uint32 int_status;

	/*
		following variables are get from global variables for 2 SIM projects.
		We won't use these in single SIM driver, but we need to define the power set.
	*/
	kal_uint32 processingState;//to to reentry check
    kal_bool isSW6263; // query if status word 0x62xx 0x63xx happen
#endif
} usim_dcb_struct;

typedef enum {
	T1_NAD_INDEX       =       0,
	T1_PCB_INDEX       =       1,
	T1_LEN_INDEX       =       2,
	T1_INF_INDEX       =       3,
	T1_EDC_INDEX       =       4
} usim_t1_header_index_enum;

static usim_dcb_struct usim_cb[DRV_SIM_MAX_LOGICAL_INTERFACE];

sim_HW_cb   simHWCbArray[DRV_SIM_MAX_LOGICAL_INTERFACE];
static const kal_uint32 iccSlotNum = MAX_SIM_CARD_NUM;

static SIM_ICC_HW_SW_MAPPING iccMappingTable[MAX_SIM_CARD_NUM];

sim_MT6306_cardInfo   sim_MT6306_card[DRV_SIM_MAX_LOGICAL_INTERFACE];
sim_MT6306_switchInfo switchCBMT6306[SIM_MT6302_MAX_MT6302];

sim_MT6306_I2CWriter_implementation I2CWriterPtr;
sim_MT6306_I2CReader_implementation I2CReaderPtr;


static Sim_Card SimCard_cb[DRV_SIM_MAX_LOGICAL_INTERFACE];


static SIM_ICC_HW_SW_MAPPING iccMappingTable[MAX_SIM_CARD_NUM] =
{
	/*
		logical number, application usage from user's view, do it connect to MT6302, 
		owned by which SIM controller, MT6302 chip #, interface # of this MT6302
	*/
	{0, SIM_ICC_APPLICATION_PHONE1, MTK_SIMIF0, SIM_SWITCH_6306, 0, 0, 0, KAL_FALSE, 0},
};

#ifndef DRV_SIM_HIGH_SPEED
static kal_uint8 BWT_Factor[3] = {1, 6, 12}; // 372/64 = 6, 372/32 = 12
#else
static kal_uint8 BWT_Factor[5] = {1, 6, 12, 24, 48}; // 372/64 = 6, 372/32 = 12, 23< 372/16 < 24
#endif

sim_ctrlDriver *sim_driver;

extern UINT16 SIF_X_Read(UINT8 u1CHannelID, UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum, UINT32 u4WordAddr, UINT8 *pu1Buf, UINT16 u2ByteCnt);

extern UINT16 SIF_X_Write(UINT8 u1CHannelID, UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum, UINT32 u4WordAddr, UINT8 *pu1Buf, UINT16 u2ByteCnt);

kal_uint32 sim_get_logicalNum_from_app(SIM_ICC_APPLICATION application)
{
	kal_uint32 loopIndex, loopMax = iccSlotNum;

	for(loopIndex = 0; loopIndex < loopMax; loopIndex++){
		if(application == iccMappingTable[loopIndex].application){
			if(iccSlotNum <= iccMappingTable[loopIndex].logicalNum)
				return 0xffffffff;
			return iccMappingTable[loopIndex].logicalNum;
		}
	}
	return 0xffffffff;
}

kal_uint8 MT6306_getDeviceAddr(void)
{
	return 0x64;
}

// frank add
sim_env SIM_GetCurrentEnv(void)
{
	sim_print(SIM_DBG_TRACE, "SIM_GetCurrentEnv = ME_30V_ONLY \n");
	return ME_30V_ONLY;
}

//------------------------------------------------------------------------//
// General interfaces of sim driver
//------------------------------------------------------------------------//
/*************************************************************************
* FUNCTION
*  L1usim_Init
*
* DESCRIPTION
*	1. It is the initialization function of usim driver
*	2. It shall be called only once.
*	3. It gets the customization data of borad-supported voltage.
*	4. It initialize the structure of usim control block .
*	5. It get a GPT handler, a dma port,and register lisr, hisr, a event groug
*
* PARAMETERS
	None

* RETURNS
	None

* GLOBALS AFFECTED
*	usim_dcb
*
*************************************************************************/
static void L1usim_Init(sim_HW_cb *hw_cb)
{
	usim_dcb_struct *usim_dcb;
	sim_MT6306_switchInfo *switch_CB;

	sim_print(SIM_DBG_TRACE, "L1usim_Init\n\n");

	usim_dcb = &usim_cb[0];

	switch_CB = &switchCBMT6306[0];

	if(usim_dcb->warm_rst == KAL_FALSE)
	{
		usim_dcb->sim_env = SIM_GetCurrentEnv();
		usim_dcb->dir = USIM_DIRECT;
		usim_dcb->speed = SPEED_372;
		usim_dcb->clock_stop_en = KAL_FALSE;
		usim_dcb->clock_stop_type = CLOCK_STOP_UNKONW;
		usim_dcb->phy_proto = T1_PROTOCOL;
		usim_dcb->warm_rst = KAL_FALSE;
		usim_dcb->rx_size = 0;
		usim_dcb->rx_buf = NULL;
		usim_dcb->tx_size = 0;
		usim_dcb->tx_buf = NULL;
		usim_dcb->Fi = FI_DEFAULT;
		usim_dcb->Di = DI_DEFAULT;
		usim_dcb->header_tx[0] = NAD;
		usim_dcb->ts_hsk_en = KAL_TRUE;
		usim_dcb->WWT = INIT_WWT_T0;
		usim_dcb->etu_of_1860 = (1860/32);
		usim_dcb->etu_of_700 = (700/32);
		usim_dcb->present = KAL_TRUE;
		usim_dcb->power_class = UNKNOWN_POWER_CLASS;

		/*there will be no enable enhanced_speed function, we should set this myself*/
		usim_dcb->high_speed_en = KAL_TRUE;
	}

	// reset these value no matter cold or warm reset
	usim_dcb->main_state = ACTIVATION_STATE;
	usim_dcb->ifsc = USIM_IFSC_DEFAULT;
	usim_dcb->ifsd = USIM_IFSD_DEFAULT;
	usim_dcb->ns = 0;
	usim_dcb->nr = 0;
}

void SIM_SetTOUT(kal_uint32 TOUT, sim_HW_cb *hw_cb)
{
	sim_print(SIM_DBG_TRACE, "SIM_SetTOUT\n");

	SIM_ClearBits(SIM_CONF, SIM_CONF_TOUTEN);
	if (TOUT < 0xffffff)
		SIM_WriteReg32(SIM_TOUT, TOUT);
	else
		SIM_WriteReg32(SIM_TOUT, 0xffffff);

	SIM_Reg(SIM_STS);
	SIM_SetBits(SIM_CONF, SIM_CONF_TOUTEN);
}

sim_MT6306_status sim_MT6306_passDAT(sim_HW_cb *hw_cb)
{
	sim_MT6306_switchInfo *switch_CB;
	sim_MT6306_cardInfo	*card_cb;

	switch_CB = &switchCBMT6306[0];
	card_cb = &sim_MT6306_card[0];

	switch(card_cb->pins.DAT){
		case sim_MT6306_DATBlockedHigh:/*b000*/
		case sim_MT6306_NoLDO_DATBlockedHigh:/*b000*/
			{
				if(switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_DAT] & (0x1 << 0)){
					sim_print(SIM_DBG_TRACE, "sim_MT6306_passDAT -> sim_MT6306_NoLDO_DATBlockedHigh error\n");
				}

				/*b00 -> b01*/
				switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_DAT] |= (0x1 << 0);
				switch_CB->sim_MT6320_writer(0, switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_DAT]);
				card_cb->pins.DAT |= 0x1;
				break;
			}
		case sim_MT6306_DATBlockedLow:/*b101*/
		case sim_MT6306_NoLDO_DATBlockedLow:/*b101*/
			{
				if((switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_DAT] & (0x5 << 0)) != (0x5 << 0)){
					sim_print(SIM_DBG_TRACE, "sim_MT6306_passDAT -> sim_MT6306_NoLDO_DATBlockedLow error\n");
				}

				/*b101->b001*/
				switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_DAT] &= ~(0x4 << 0);
				switch_CB->sim_MT6320_writer(0, switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_DAT]);
				card_cb->pins.DAT &= ~0x4;
				break;
			}
		case sim_MT6306_DATBlockedHigh_Low:
		case sim_MT6306_NoLDO_DATBlockedHigh_Low:
			{
				sim_print(SIM_DBG_TRACE, "sim_MT6306_passDAT -> sim_MT6306_NoLDO_DATBlockedHigh_Low\n");
			}
		case sim_MT6306_DATPass_High:
		case sim_MT6306_NoLDO_DATPass_High:
			break;
		default:
			sim_print(SIM_DBG_TRACE, "sim_MT6306_passDAT Setting Error\n");
	}

	return sim_MT6306_statusNoError;
}

sim_MT6306_status sim_MT6306_passRST(sim_HW_cb *hw_cb)
{
	sim_MT6306_switchInfo *switch_CB;
	sim_MT6306_cardInfo	*card_cb;

	switch_CB = &switchCBMT6306[0];
	card_cb = &sim_MT6306_card[0];

	switch(card_cb->pins.RST)
	{
		case sim_MT6306_RSTBlockedHigh:
		case sim_MT6306_RSTBlockedLow:
		case sim_MT6306_NoLDO_RSTBlockedHigh:
		case sim_MT6306_NoLDO_RSTBlockedLow:
			{
				if((switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_RST] & (0x1 << 0)) != (0x1 << 0)){
					sim_print(SIM_DBG_TRACE, "sim_MT6306_passRST -> sim_MT6306_NoLDO_RSTBlockedLow Error\n");
				}

				/*to pass RST, we only need to clear 1<<interface*/
				switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_RST] &= ~(0x1 << 0);

				switch_CB->sim_MT6320_writer(0, switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_RST]);

				card_cb->pins.RST &= ~0x1;

				break;
			}
		case sim_MT6306_RSTPass_High:
		case sim_MT6306_RSTPass_Low:
		case sim_MT6306_NoLDO_RSTPass_High:
		case sim_MT6306_NoLDO_RSTPass_Low:
			break;
		default:
			sim_print(SIM_DBG_TRACE, "sim_MT6306_passRST Error\n");
	}

	return sim_MT6306_statusNoError;
}

sim_MT6306_status sim_MT6306_recordDirectionBaud(sim_HW_cb *hw_cb)
{
	sim_MT6306_cardInfo *card_cb;

	card_cb = &sim_MT6306_card[0];

	/*record convention*/
	card_cb->config = SIM_Reg(SIM_CONF);

	/*record baud rare*/
	card_cb->baud = SIM_Reg(SIM_BRR)>>2;

	return sim_MT6306_statusNoError;
}

sim_MT6306_status sim_MT6306_retreiveDirectionBaud(sim_HW_cb *hw_cb)
{
	kal_uint16  temp_reg;
	sim_MT6306_cardInfo *card_cb;

	card_cb = &sim_MT6306_card[0];

	/*setting baud rate*/
	temp_reg = SIM_Reg(SIM_BRR);
	temp_reg &= SIM_BRR_CLKMSK;
	temp_reg |= (card_cb->baud << 2);
	SIM_WriteReg32(SIM_BRR, temp_reg);

	/* setting convention and T0, T1 */
	temp_reg = SIM_Reg(SIM_CONF);
	temp_reg &= ~SIM_MT6306_CONFMSK;
	temp_reg |= (card_cb->config & SIM_MT6306_CONFMSK);
	SIM_WriteReg32(SIM_CONF, temp_reg);

	return sim_MT6306_statusNoError;
}

sim_MT6306_status sim_MT6306_change(sim_HW_cb *hw_cb, sim_MT6306_changeEvent event){

	sim_MT6306_switchInfo *switch_CB;
	sim_MT6306_cardInfo	*card_cb;

	switch_CB = &switchCBMT6306[0];
	card_cb = &sim_MT6306_card[0];

	/* pass DAT, maybe for some card, we can't pass DAT, need more revise */
	sim_MT6306_passDAT(hw_cb);

	/* for reset and power off, we have to pass RST, but there maybe some card 
	   can't pass, have to revise */
	if(sim_MT6306_eventCommand != event){
		sim_MT6306_passRST(hw_cb);
	}

	switch_CB->sim_MT6306_taskAccessing = KAL_TRUE;

	card_cb->sim_MT6306_clkStopQueued = KAL_FALSE;

	sim_MT6306_retreiveDirectionBaud(hw_cb);

	return sim_MT6306_statusNoError;
}

/*************************************************************************
 * FUNCTION
 *  usim_set_timeout
 *
 * DESCRIPTION
 *    setup the timeout value in the unit of etu
 *
 * PARAMETERS
 *   timeout: timeout value in the unit of etu , 0 means disabling timeout
 *
 * RETURNS
 *
 * GLOBALS AFFECTED
 *
 *************************************************************************/
static void usim_set_timeout(kal_uint32 timeout, sim_HW_cb *hw_cb)
{
	usim_dcb_struct *usim_dcb;

	usim_dcb = &usim_cb[0];

	sim_print(SIM_DBG_TRACE, "usim_set_timeout\n");

	if(timeout)
	{
		timeout >>= 2;

		sim_print(SIM_DBG_TRACE, "timeout value: %d\n", timeout);

		SIM_SetTOUT(timeout+TOUT_OFFSET, hw_cb);
		usim_dcb->timeout = timeout+TOUT_OFFSET;
	}
	else
	{
		SIM_ClearBits(SIM_CONF, SIM_CONF_TOUTEN);
	}
}

/*************************************************************************
 * FUNCTION
 *  usim_check_input_volt
 *
 * DESCRIPTION
 *    check if the input volt is supported by the interface device
 *
 * PARAMETERS
 *   volt: voltage used by SIM card
 *
 * RETURNS
 *   KAL_TRUE: it is supported
 *   KAL_FALSE: not supported
 *
 * GLOBALS AFFECTED
 *
 *
 *************************************************************************/
kal_bool usim_check_input_volt(usim_power_enum  volt, sim_HW_cb *hw_cb)
 {
	usim_dcb_struct *usim_dcb;

	usim_dcb = &usim_cb[0];

	sim_print(SIM_DBG_TRACE, "usim_check_input_volt\n");

	if(usim_dcb->sim_env == ME_18V_30V)
		return KAL_TRUE;
	if(usim_dcb->sim_env == ME_30V_ONLY && volt == CLASS_B_30V)
		return KAL_TRUE;
	if(usim_dcb->sim_env == ME_18V_ONLY && volt == CLASS_C_18V)
		return KAL_TRUE;

	sim_print(SIM_DBG_TRACE, "input volt error\n");

	return KAL_FALSE;
}

/*************************************************************************
 * FUNCTION
 *  usim_set_speed
 *
 * DESCRIPTION
 *    setup the baudrate of the SIM card, only support 372, 64 and 32.
 *    speed 16 is not supported, use speed32 insteadly.
 *
 * PARAMETERS
 *   timeout: timeout value in the unit of etu , 0 means disabling timeout
 *
 * RETURNS
 *
 * GLOBALS AFFECTED
 *
 *************************************************************************/
static void usim_set_speed(usim_speed_enum speed, sim_HW_cb *hw_cb)
{
	usim_dcb_struct *usim_dcb;
	kal_uint16 sim_brr_div[SPEED_MAX] = {SIM_BRR_BAUD_Div372,
	                                     SIM_BRR_BAUD_Div64,
	                                     SIM_BRR_BAUD_Div32,
	                                     SIM_BRR_BAUD_Div16,
	                                     SIM_BRR_BAUD_Div8
	                                      };

	sim_print(SIM_DBG_TRACE, "usim_set_speed\n");

	sim_print(SIM_DBG_TRACE, "SIM_BRR %x\n", (SIM_BRR_CLK_Div4 | sim_brr_div[speed]));

	usim_dcb = &usim_cb[0];
	// clock: 13/4 = 3.25M, with default etu F/372
	SIM_WriteReg32(SIM_BRR, (SIM_BRR_CLK_Div4 | sim_brr_div[speed]));
	usim_dcb->speed = speed;
	usim_dcb->BWT = usim_dcb->BWT*BWT_Factor[speed]+11;
}

/*
 * FUNCTION
 *   DRV_ICC_Calc_WWT
 *
 * DESCRIPTION
 *       The function is used to calc WWT.
 *
 * CALLS
 *
 * PARAMETERS
 *   Fi: value of the clock rate conversion integer
 *   Di: value of baud rate adjustment integer
 *   Wi: waiting time integer
 *   *WWT: work waiting time
 *
 * RETURNS
 *   void
 *
 * GLOBALS AFFECTED
 *   external_global
 */
void DRV_ICC_Calc_WWT(kal_uint16 Fi, kal_uint8 Di,  kal_uint8 Wi, kal_uint32 *WWT)
{
	switch(Fi)
	{
		case 372:
		/*  400*Wi to cover some slow card, margin: 3% */
			*WWT = (960*Wi+400*Wi)*Di;
			break;

		default:
		/*  40*Wi to cover some slow card, margin: 4% */
			*WWT = (960*Wi+40*Wi)*Di;
			break;
	}
}

/*************************************************************************
* FUNCTION
*  usim_process_TA1
*
* DESCRIPTION
	1. Get Di and Fi from TA1
*
* PARAMETERS
	TA1: first interface character used to indicate the Fi and Di

* RETURNS
*	None
*
* GLOBALS AFFECTED
*	usim_dcb->Fi, usim_dcb->Di
*
*************************************************************************/
static void usim_process_TA1(kal_uint8 TA1, sim_HW_cb *hw_cb)
{
	usim_dcb_struct *usim_dcb;

	usim_dcb = &usim_cb[0];

	if(TA1 == ATR_TA1_64)
	{
		sim_print(SIM_DBG_TRACE, "SPEED64 \r\n");
		usim_dcb->Fi = 512;
		usim_dcb->Di = 8;
		usim_dcb->card_speed = SPEED_64;
	}
	else if(TA1 == ATR_TA1_32)
	{
		sim_print(SIM_DBG_TRACE, "SPEED32 \r\n");
		usim_dcb->Fi = 512;
		usim_dcb->Di = 16;
		usim_dcb->card_speed = SPEED_32;
	}
	else if(TA1 == ATR_TA1_16)
	{	// only support speed32 even encounter a speed16 card
		sim_print(SIM_DBG_TRACE, "SPEED16 \r\n");
		usim_dcb->Fi = 512;
#ifndef DRV_SIM_HIGH_SPEED
		usim_dcb->Di = 16;
		usim_dcb->card_speed = SPEED_32;
#else
		usim_dcb->Di = 32;
		usim_dcb->card_speed = SPEED_16;
#endif
	}
	else if(TA1 == ATR_TA1_8)
	{	
		sim_print(SIM_DBG_TRACE, "SPEED8 \r\n");
		usim_dcb->Fi = 512;
#ifndef DRV_SIM_HIGH_SPEED
		usim_dcb->Di = 16;
		usim_dcb->card_speed = SPEED_32;
#else
		usim_dcb->Di = 64;
		usim_dcb->card_speed = SPEED_8;
#endif
	}
	else
	{
		sim_print(SIM_DBG_TRACE, "SPEED372 \r\n");
		usim_dcb->Fi = 372;
		usim_dcb->Di = 1;
		usim_dcb->card_speed = SPEED_372;
	}
}

/*************************************************************************
* FUNCTION
*  usim_check_TCK
*
* DESCRIPTION
*	 check if TCK present and the checksum of ATR is correct
*
* PARAMETERS
*
* RETURNS
*	KAL_TRUE: TCK is not present or chekcsum is correct
*	KAL_FALSE: Chekcsum is incorrect
*
* GLOBALS AFFECTED
*
*************************************************************************/
static kal_bool usim_check_TCK(sim_HW_cb *hw_cb)
{
	kal_uint32 i = 0;
	kal_uint8 ck = 0;
	kal_uint8 *ptr;
	usim_dcb_struct *usim_dcb;

	usim_dcb = &usim_cb[0];
	ptr = usim_dcb->ATR_data;

	sim_print(SIM_DBG_TRACE, "usim_check_TCK\n");
	while(SIM_Reg(SIM_COUNT) == 0 && i++ <= 22)
	{
		mdelay(10);
	}

	if(i >= 23)
	{
		sim_print(SIM_DBG_TRACE, "TCK not present\n");
		return KAL_TRUE;
	}

	ptr[usim_dcb->ATR_index++] = SIM_Reg(SIM_DATA);

	for(i=1;i<usim_dcb->ATR_index;i++)
		ck ^= ptr[i];

	if(ck != 0)
	{
		sim_print(SIM_DBG_TRACE, "TCK checksum err\n");
		return KAL_FALSE;
	}

	sim_print(SIM_DBG_TRACE, "TCK checksum ok\n");
	return KAL_TRUE;
}

/*************************************************************************
* FUNCTION
*  usim_process_ATR
*
* DESCRIPTION
*	1. wait all ATR characters received at HISR and put into usim_dcb->ATR_data
*	2. Get parameters from ATR, Fi, Di, T0_support, T1_support, reset mode, WWT(T0)
*		IFSC(T1), CWI, BWI, X, U
*
* PARAMETERS
*	None
* RETURNS
*  KAL_TRUE: Valid TS is received
*  KAL_FALSE: Valid TS is not received, card is not present or not supported
*
* GLOBALS AFFECTED
*	usim_dcb
*
*************************************************************************/
static usim_status_enum usim_process_ATR(sim_HW_cb *hw_cb)
{
	kal_uint8 data,TD;
	kal_bool T15;
	kal_uint8 *ptr;
	kal_uint32 index = 1;	// skip the first TS byte
	usim_dcb_struct *usim_dcb;

	sim_print(SIM_DBG_TRACE, "usim_process_ATR\n\n");

	usim_dcb = &usim_cb[0];
	ptr = usim_dcb->ATR_data;

	if ((ptr[index] & 0x0f) == 0){
		usim_dcb->app_proto = SIM_PROTOCOL; // no historical char imply SIM_PROTOCOL
	}	

	do
	{
		data = ptr[index++];
		USIM_CAL_TD_COUNT(data, index);
	} while(data & TDMask);

   usim_dcb->hist_index = index;

#if 1
	if(ptr[index] == HIST_FIRST_USIM && ptr[index+1] == HIST_SEC_USIM && ptr[index+3] == HIST_FOUR_USIM){
	   	usim_dcb->app_proto = USIM_PROTOCOL;
	}
	else if(ptr[index] == HIST_FIRST_USIM && ptr[index+1] == HIST_SEC_USIM 
				&& ptr[index+3] == 0x51 && ptr[index+5] == HIST_FOUR_USIM){
   		sim_print(SIM_DBG_TRACE, "historycal byte error\n");
	   	usim_dcb->app_proto = USIM_PROTOCOL;
	}
	else {
		usim_dcb->app_proto = SIM_PROTOCOL;
	}

	// parse the content of ATR
	T15 = KAL_FALSE;
	index = 1;
	TD = ptr[index++];	//T0

	if(TD & TAMask)
	{
		// TA1 (FI, DI)
		data = ptr[index++];
		usim_process_TA1(data, hw_cb);
		//usim_dcb->WWT = INIT_WWT_T0*usim_dcb->Di;
		DRV_ICC_Calc_WWT(usim_dcb->Fi, usim_dcb->Di, 10, &usim_dcb->WWT);
	}

	if(TD & TBMask)
	{
		// TB1 (PI, II) (neglect it)
		data = ptr[index++];
	}

	if(TD & TCMask)
	{
		// TC1 (N: extra guard time) (neglect it)
		data = ptr[index++];
		if(data != 0 && data != 255)
			return USIM_INVALID_ATR;
	}

	if( !(TD & TDMask))
	{
		usim_dcb->T0_support = KAL_TRUE;
		return USIM_NO_ERROR;
	}

	TD = ptr[index++]; // TD1
	if((TD & 0x0f) == 0)
		usim_dcb->T0_support = KAL_TRUE;
	else if((TD & 0x0f) == 1)
		usim_dcb->T1_support = KAL_TRUE;
	else if(TD & 0x0f == 0x0f)
	{
		// T = 15 is forbidden in TD1
		return USIM_INVALID_ATR;
	}

	if(TD & TAMask)
	{
		//TA2 (specific mode)
		sim_print(SIM_DBG_TRACE, "TA2(specific mode) \r\n");
		data = ptr[index++];
		usim_dcb->reset_mode = USIM_RESET_SPECIFIC;
		// chage the clock to the one before reset.
	}
	if(TD & TBMask)
	{
		// TB2 (PI2)(neglect it)
		data = ptr[index++];
	}
	if(TD & TCMask)
	{
		// TC2 (work waiting time = 960xWIxDi etu)(T0)
		data = ptr[index++];
		//usim_dcb->WWT = 960*data*usim_dcb->Di;
		DRV_ICC_Calc_WWT(usim_dcb->Fi, usim_dcb->Di, data, &usim_dcb->WWT);
	}

	sim_print(SIM_DBG_TRACE, "WWT: %d \r\n",usim_dcb->WWT);
	if( !(TD & TDMask))
	{
		if(usim_dcb->T0_support && !usim_dcb->T1_support && !T15)
		{
			return USIM_NO_ERROR;
		}
		else
		{
			if (usim_check_TCK(hw_cb) == KAL_FALSE)
				return USIM_INVALID_ATR;
			else
				return USIM_NO_ERROR;
		}
	}

	TD = ptr[index++]; // TD2
	if((TD & 0x0f) == 1)
	{
		usim_dcb->T1_support = KAL_TRUE;
	}
	else if((TD & 0x0f) == 0x0f)
	{
		T15 = KAL_TRUE;
		goto global_interface;
	}

	if(TD & TAMask)
	{
		//TA3 (ISFC)
		data = ptr[index++];
		usim_dcb->ifsc = data;
		sim_print(SIM_DBG_TRACE, "IFSC: %d \r\n", data);
	}

	if(TD & TBMask)
	{
		kal_uint8 cwi,bwi;

		// TB3 (PI2)
		data = ptr[index++];
		cwi = data&0xf; // range from 0~5
		bwi = (data&0xf0)>>4;
		if(cwi > MAX_CWI)
			return USIM_INVALID_ATR;
		if(bwi > MAX_BWI)
			return USIM_INVALID_ATR;
		usim_dcb->CWT = (1<<cwi)+11;
		usim_dcb->BWT = (1<<bwi)*960;
	}
	else
	{
		usim_dcb->CWT = USIM_CWT_DEFAULT;
		usim_dcb->BWT = USIM_BWT_DEFAULT;
	}

	sim_print(SIM_DBG_TRACE, "BWT: %d \r\n",usim_dcb->BWT);
	if(TD & TCMask)
	{
		// TC3 (neglect)
		data = ptr[index++];
	}

	if( !(TD & TDMask))
	{
		if(usim_dcb->T0_support && !usim_dcb->T1_support && !T15)
			return USIM_NO_ERROR;
		else
		{
			if (usim_check_TCK(hw_cb) == KAL_FALSE)
				return USIM_INVALID_ATR;
			else
				return USIM_NO_ERROR;
		}
	}

	TD = ptr[index++]; // TD3
	if((TD & 0x0f) != 0x0f)
	{
		if(usim_dcb->T0_support && !usim_dcb->T1_support && !T15)
			return USIM_NO_ERROR;
		else
		{
			if (usim_check_TCK(hw_cb) == KAL_FALSE)
				return USIM_INVALID_ATR;
			else
				return USIM_NO_ERROR;
		}
	}

global_interface:

	if (usim_check_TCK(hw_cb) == KAL_FALSE)
		return USIM_INVALID_ATR;

	if(TD & TAMask)
	{
		//TAi (clock stop(X) and power class(U))
		data = ptr[index++];
		usim_dcb->clock_stop_type = (usim_clock_stop_enum)(data&CLOCK_STOP_MSK);
		if(usim_dcb->app_proto == SIM_PROTOCOL)
			usim_dcb->power_class = CLASS_ALLSUPPORT;
		else
			usim_dcb->power_class = (usim_power_enum)(data & USIM_POW_CLASS_MSK);

		sim_print(SIM_DBG_TRACE, "clock stop[7:8]|power class[1:6]: %x \r\n",data);
		/*SIM task need following information for UICC identification*/
		usim_dcb->TAiExist = KAL_TRUE;
	}
	else{ //from latest 7816-3, if ATR is valid without class indicator, host should continue normal operation
		return USIM_NO_ERROR;
	}
	// check if used power is supported by the UICC
	if((usim_dcb->power & usim_dcb->power_class) == 0)
	{
		if(usim_dcb->sim_env == ME_18V_30V)
		{
			if(usim_dcb->power == CLASS_C_18V)
				usim_dcb->power = CLASS_B_30V;

			return USIM_VOLT_NOT_SUPPORT;
		}
	}
	else if(usim_dcb->app_proto == SIM_PROTOCOL)
		return USIM_NO_ERROR;
#endif

	return USIM_NO_ERROR;
}

void SIM_SetRXTIDE(kal_uint16 RXTIDE, sim_HW_cb *hw_cb)
{
	kal_uint16 TIDE;
//	sim_print(SIM_DBG_TRACE, "SIM_SetRXTIDE\n");
	TIDE = SIM_Reg(SIM_TIDE);
	TIDE &= ~SIM_TIDE_RXMASK;
	TIDE |= (RXTIDE-1);
//	sim_print(SIM_DBG_TRACE, "Set SIM_TIDE: %x\n", TIDE);
	SIM_WriteReg32(SIM_TIDE, TIDE);
}

void SIM_SetTXTIDE(kal_uint16 _TXTIDE, sim_HW_cb *hw_cb)
{
	kal_uint16 TIDE;
//	sim_print(SIM_DBG_TRACE, "SIM_SetTXTIDE\n");
	TIDE = SIM_Reg(SIM_TIDE);
	TIDE &= ~SIM_TIDE_TXMASK;
	TIDE |= ((_TXTIDE+1) <<8);
//	sim_print(SIM_DBG_TRACE, "Set SIM_TIDE: %x\n", TIDE);
	SIM_WriteReg32(SIM_TIDE, TIDE);
}

void SIM_SetRXRetry(sim_HW_cb *hw_cb, kal_uint16 RXRetry)
{
	kal_uint16 Retry;
	Retry = SIM_Reg(SIM_RETRY);
	Retry &= ~SIM_RETRY_RXMASK;
	Retry |= RXRetry;
	SIM_WriteReg32(SIM_RETRY, Retry);
}

void SIM_SetTXRetry(sim_HW_cb *hw_cb, kal_uint16 TXRetry)
{
	kal_uint16  Retry;
	Retry = SIM_Reg(SIM_RETRY);
	Retry &= ~SIM_RETRY_TXMASK;
	Retry |= (TXRetry<<8);
	SIM_WriteReg32(SIM_RETRY, Retry);
}

kal_uint32 sim_get_MT6302_from_logicalNum(kal_uint32 logicalNum)
{
	kal_uint32 loopIndex, loopMax = iccSlotNum;

	if (iccSlotNum <= logicalNum){
		sim_print(SIM_DBG_TRACE, "sim_get_MT6302_from_logicalNum: iccSlotNum Error\n");
	}	

	for (loopIndex = 0; loopIndex < loopMax; loopIndex++){
		if (logicalNum == iccMappingTable[loopIndex].logicalNum){
			if (SIM_SWITCH_6302 == (kal_bool)iccMappingTable[loopIndex].needMT6302)
				return (iccMappingTable[loopIndex].MT6302ChipNum << 8) | (iccMappingTable[loopIndex].MT6302PortNum);
			else if (SIM_SWITCH_6306 == (kal_bool)iccMappingTable[loopIndex].needMT6302)
				return (iccMappingTable[loopIndex].MT6302ChipNum << 8) | (iccMappingTable[loopIndex].MT6302PortNum) |      					(SIM_SWITCH_6306 <<16);
			else if (SIM_SWITCH_6314 == (kal_bool)iccMappingTable[loopIndex].needMT6302)
				return (iccMappingTable[loopIndex].MT6302ChipNum << 8) | (iccMappingTable[loopIndex].MT6302PortNum) |       				(SIM_SWITCH_6314 <<16);
			else
				return SIM_ICC_MT6302_NONE;
		}
	}

	/*there is no this logical number*/
	sim_print(SIM_DBG_TRACE, "sim_get_MT6302_from_logicalNum Error\n");
}

/*************************************************************************
* FUNCTION
*  usim_activation
*
* DESCRIPTION
* Perform the activation of USIM
* It is a cold reset
* select the power according to usim_dcb->power (input)
* select the convention according to usim_dcb->dir (input)
* the clock rate adopted is SPEED_372
* set the default timeout value
*
* PARAMETERS
	None

* RETURNS
	None

* GLOBALS AFFECTED
*	usim_dcb
*
*************************************************************************/
static void usim_activation(sim_HW_cb *hw_cb)
{
	kal_uint16 reg = 0;
	usim_dcb_struct *usim_dcb;
	sim_MT6306_cardInfo *card_cb;
	sim_MT6306_switchInfo *switch_CB;

	sim_print(SIM_DBG_TRACE, "usim_activation\n\n");

	usim_dcb = &usim_cb[0];
	card_cb = &sim_MT6306_card[0];
	switch_CB = &switchCBMT6306[0];

	/*control the real voltage to card*/
	if (usim_dcb->power == CLASS_B_30V)
	{
		sim_print(SIM_DBG_TRACE, "select 3.0V power supply\n");
		switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_VCC] |= (0x1 << 0);
		sim_print(SIM_DBG_TRACE, "Choose MT6306 power supply to 3.0V\n");
		switch_CB->sim_MT6320_writer(0, switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_VCC]);
		/* enable VSEL bit */
		card_cb->pins.VCC |= 0x1;

		/*b00 -> b01*/
		/* passDAT */

		switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_DAT] |= (0x1 << 0);
		switch_CB->sim_MT6320_writer(0, switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_DAT]);
		card_cb->pins.DAT |= 0x1;
#if !defined(TCSUPPORT_CPU_MT7520) && !defined(TCSUPPORT_CPU_MT7505)
		switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_RST] |= (0x1 << 0);  
		switch_CB->sim_MT6320_writer(0, switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_RST]);
		card_cb->pins.RST |= 0x1;
#endif
	} else {
		sim_print(SIM_DBG_TRACE, "select 1.8V power supply\n");
		/*in Gemini project, we need 3V to communicate with outer switch*/
		switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_VCC] &= ~(0x1 << 0);
		sim_print(SIM_DBG_TRACE, "switch MT6306 power supply to 1.8V\n");
		switch_CB->sim_MT6320_writer(0, switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_VCC]);
		/* clear VSEL bit */
		card_cb->pins.VCC &= ~0x1;
	}

	reg = SIM_CONF_SIMSEL;

	if(usim_dcb->dir == USIM_DIRECT)
		reg |= SIM_CONF_Direct;
	else
		reg |= SIM_CONF_InDirect;

	if(KAL_TRUE == usim_dcb->ts_hsk_en)
	{
		reg |= (SIM_CONF_TXHSK | SIM_CONF_RXHSK);
		SIM_SetRXRetry(hw_cb, 1);
		SIM_SetTXRetry(hw_cb, 1);
	}
	else
	{
		SIM_SetRXRetry(hw_cb, 0);
		SIM_SetTXRetry(hw_cb, 0);
	}

//	sim_print(SIM_DBG_TRACE, "Set SIM_CONF value: %x\n", reg);
	SIM_WriteReg32(SIM_CONF, reg);
	mdelay(100);
	reg = SIM_Reg(SIM_CONF);
//	sim_print(SIM_DBG_TRACE, "Read SIM_CONF: %x\n", reg);

	SIM_WriteReg32(SIM_COUNT, 0x01);
	mdelay(100);
	reg = SIM_Reg(SIM_COUNT);

	SIM_SetRXTIDE(2, hw_cb);	// generate a interrupt while TS byte and T0 is received

	SIM_WriteReg32(SIM_IRQEN, USIM_IRQEN_ATR|SIM_STS_RXERR);
	reg = SIM_Reg(SIM_IRQEN);
//	sim_print(SIM_DBG_TRACE, "IRQ EN1: %x\n", reg);
//	sim_print(SIM_DBG_TRACE, "IRQ EN2: %x\n", reg);

	reg = SIM_Reg(SIM_STS);
	sim_print(SIM_DBG_TRACE, "IRQ STS1: %x\n", reg);

	usim_dcb->main_state = ACTIVATION_STATE;
	usim_dcb->ev_status = USIM_NO_ERROR;

	if(usim_dcb->warm_rst == KAL_FALSE){
/*********************************************************************************************/
		switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_VCC] |= (0x4 << 0);
/*********************************************************************************************/
		sim_print(SIM_DBG_TRACE, "Write MT6306 VCC Enable\n");
		switch_CB->sim_MT6320_writer(0, switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_VCC]);
		/* enable VSEL bit */
		card_cb->pins.CLK &= ~0x10;
		card_cb->pins.DAT &= ~0x10;
		card_cb->pins.RST &= ~0x10;
		reg = SIM_Reg(SIM_STS);
		SIM_WriteReg32(SIM_CTRL, SIM_CTRL_SIMON);
	}
	else
	{
//		USIM_WRST();
		SIM_WriteReg32(SIM_CTRL, (SIM_CTRL_WRST|SIM_CTRL_SIMON));
		mdelay(10);
	}

	// go to usim_hisr of case RXTIDE, ATRERR, RXERR(parity error)
}

/*************************************************************************
* FUNCTION
*  usim_deactivation
*
* DESCRIPTION
*	1. deactivate the UICC card
*	2. wait util the the deactivation is complete
*
* PARAMETERS
	None

* RETURNS
	None

* GLOBALS AFFECTED
*	usim_dcb->main_state
*
*************************************************************************/
static void usim_deactivation(sim_HW_cb *hw_cb)
{
	usim_dcb_struct *usim_dcb;
	sim_MT6306_switchInfo *switch_CB;
	sim_MT6306_cardInfo *card_cb;

	usim_dcb = &usim_cb[0];
	switch_CB = &switchCBMT6306[0];
	card_cb = &sim_MT6306_card[0];

	sim_print(SIM_DBG_TRACE, "usim_deactivation\n");

	if(SIM_Reg(SIM_CTRL) & SIM_CTRL_SIMON)
	{
		kal_uint32 i = 5;
		// before deactivate the SIM interface, turn on the clock first.
		SIM_ClearBits(SIM_CTRL, SIM_CTRL_HALT);
		while(i--);
		SIM_WriteReg32(SIM_IRQEN, SIM_IRQEN_SIMOFF);

		// deactivate SIM Card
		SIM_ClearBits(SIM_CTRL, SIM_CTRL_SIMON);
		mdelay(10);
		SIM_WriteReg32(SIM_IRQEN, SIM_IRQEN_SIMOFF);
		switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_VCC] &= ~(0x4 << 0);
		switch_CB->sim_MT6320_writer(0, switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_VCC]);

		/*in MT6306, if VCC is down, pin state have to change*/
		card_cb->pins.VCC &= (~0x4) ;
		card_cb->pins.CLK |= 0x10;
		card_cb->pins.DAT |= 0x10;
		card_cb->pins.RST |= 0x10;
		/*since we don't use HISR and event, we have to set ev_status manually*/
		usim_dcb->ev_status = USIM_POWER_OFF;

		// Clear FIFO
		SIM_WriteReg32(SIM_COUNT, 1);

		usim_dcb->main_state = DEACTIVATION_STATE;
		if (usim_dcb->ev_status == USIM_POWER_OFF)
			usim_dcb->main_state = DEACTIVATION_STATE;
		else {
			sim_print(SIM_DBG_TRACE, "usim_deactivation error\n");
		}
	}
}

/*************************************************************************
* FUNCTION
*  usim_rx_handler
*
* DESCRIPTION
*	1. It is called byt usim_hisr
*	2. It is called while RXTIDE interrupt is triggerred
*
* PARAMETERS
	None

* RETURNS
	None

* GLOBALS AFFECTED
*	usim_dcb
*
*************************************************************************/
static void usim_rx_handler(kal_uint32 int_status, sim_HW_cb *hw_cb)
{
	usim_dcb_struct *usim_dcb;
	kal_uint8 data;
	int cnt = 0;

	usim_dcb = &usim_cb[0];

	sim_print(SIM_DBG_TRACE, "usim_rx_handler\n");

	switch(usim_dcb->main_state)
	{
		case ACTIVATION_STATE:
			sim_print(SIM_DBG_TRACE, "\n\n\nACTIVATION_STATE \n");
			/////dbg_print("ACTIVATION_STATE ");
			{
				kal_uint8 TS,T0,count;

				TS = SIM_Reg(SIM_DATA);

				sim_print(SIM_DBG_TRACE, "TS = %x ,SIM_CONF=%x\n", TS,SIM_Reg(SIM_CONF) );

				/////dbg_print("TS = %x ", TS);
				if(TS == 0x3B || TS == 0x3F)
				{
					usim_set_timeout(INIT_WWT_T0, hw_cb);
					SIM_SetBits(SIM_CONF, (SIM_CONF_TXHSK | SIM_CONF_RXHSK));
					SIM_SetRXRetry(hw_cb, USIM_RETRY);
					SIM_SetTXRetry(hw_cb, USIM_RETRY);
					count = 0;
					T0 = SIM_Reg(SIM_DATA);
					usim_dcb->hist_index = T0&0xf; // use to contain the length of historical char (temperary)
					sim_print(SIM_DBG_TRACE, "T0=%x\n",T0);
					if((T0 & TDMask) == (kal_uint32)NULL)
					{
						count += usim_dcb->hist_index;
						if(count >= SIM_TOTAL_FIFO_LEN)
						{
							usim_dcb->abort = KAL_TRUE;	// for temp usage (separate two times)
							count -= 6;
						}
						usim_dcb->resync = KAL_TRUE; // for temp usage (last time)
					}
					else
						count++;

					if(count == 0)
					{
						usim_dcb->ev_status = USIM_ATR_REC;
					}
					else
					{
					
#if defined(TCSUPPORT_CPU_MT7520)||defined(TCSUPPORT_CPU_MT7505)				
					USIM_CAL_TD_COUNT(T0, cnt);
					if(T0 & TDMask)
						cnt ++;
					usim_dcb->rx_size = cnt+1;	// for temp usage (index to TD byte)
#else
usim_dcb->rx_size = count+1;	// for temp usage (index to TD byte)
#endif
						SIM_SetRXTIDE(count, hw_cb);
						SIM_WriteReg32(SIM_IRQEN, USIM_IRQEN_NORMAL);
						usim_dcb->ev_status = USIM_NO_ERROR;
					}
					usim_dcb->main_state = ATR_STATE;
					usim_dcb->ATR_index = 0;
					usim_dcb->ATR_data[usim_dcb->ATR_index++] = TS;
					usim_dcb->ATR_data[usim_dcb->ATR_index++] = T0;
				}
				else
				{
					kal_uint16 reg;
                    reg = reg;
					usim_dcb->ev_status = USIM_TS_INVALID;
					//	SIM_DisAllIntr(); // prevent the following ATR bytes trigger RX interrupt
					SIM_WriteReg32(SIM_IRQEN, SIM_IRQEN_ALLOFF);
					reg = SIM_Reg(SIM_STS);
					
				}
			}
			break;

		case ATR_STATE:
			sim_print(SIM_DBG_TRACE, "\n\n\nATR_STATE \n");
			{	// receive all ATR data without timeout to indicate
				kal_uint32 count;
				kal_uint8 TD;

				count = SIM_Reg(SIM_COUNT);
				sim_print(SIM_DBG_TRACE, "Receive ATR Data Count : %d\n", count);
				/////dbg_print("ATR_STATE : %d ",count);

				while(count--){
					data = (kal_uint8)SIM_Reg(SIM_DATA);
					usim_dcb->ATR_data[usim_dcb->ATR_index++] = data;
					sim_print(SIM_DBG_TRACE, "%x ",data);
				}

				if(usim_dcb->abort == KAL_TRUE)
				{
					usim_dcb->abort = KAL_FALSE;
					SIM_SetRXTIDE(6, hw_cb);
					usim_dcb->resync = KAL_TRUE;
					break;
				}
#if defined(TCSUPPORT_CPU_MT7520)||defined(TCSUPPORT_CPU_MT7505)
				TD = usim_dcb->ATR_data[usim_dcb->rx_size];
				
				sim_print(SIM_DBG_TRACE, ", TD = %x ,%d", TD,usim_dcb->rx_size);

				if(TD == 0)
				{
					usim_set_timeout(0, hw_cb);
					sim_print(SIM_DBG_TRACE, "All ATR received \r\n");
					usim_dcb->hist_index = 0;
					usim_dcb->rx_size = 0;
					usim_dcb->resync = KAL_FALSE;
					usim_dcb->abort = KAL_FALSE;
					usim_dcb->ev_status = USIM_ATR_REC;
					break;
				
				}
				
#endif

				if(usim_dcb->resync == KAL_FALSE)
				{
					TD = usim_dcb->ATR_data[usim_dcb->rx_size];
					
					sim_print(SIM_DBG_TRACE, ", TD = %x ,%d", TD,usim_dcb->rx_size);

					count = 0;
					USIM_CAL_TD_COUNT(TD, count);
					if((TD & TDMask) == (kal_uint32)NULL)
					{
						count += usim_dcb->hist_index;
						if(count >= SIM_TOTAL_FIFO_LEN)
						{
							// usim_dcb->rx_index = count; // for temp usage (total ATR len)
							usim_dcb->abort = KAL_TRUE;	// for temp usage (separate two times)
							count -= 6;
						}
						usim_dcb->resync = KAL_TRUE; // for temp usage (last time)
					}
					else
						count++;
					usim_dcb->rx_size += (count);	// for temp usage (index to TD byte)
					SIM_SetRXTIDE(count, hw_cb);
				}
				else
				{
					usim_set_timeout(0, hw_cb);
					sim_print(SIM_DBG_TRACE, "All ATR received \r\n");
					usim_dcb->hist_index = 0;
					usim_dcb->rx_size = 0;
					usim_dcb->resync = KAL_FALSE;
					usim_dcb->abort = KAL_FALSE;
		            usim_dcb->ev_status = USIM_ATR_REC;
				}
			}
			break;

		case PTS_STATE:
			sim_print(SIM_DBG_TRACE, "\n\n\nPTS_STATE \n");
			SIM_WriteReg32(SIM_IRQEN, SIM_IRQEN_ALLOFF);
			usim_dcb->ev_status = USIM_NO_ERROR;
#if 0
			SIM_DisAllIntr();
			usim_dcb->ev_status = USIM_NO_ERROR;
			USIM_SET_EVENT_Multiple(usim_dcb);
#endif
			break;

		default:
			sim_print(SIM_DBG_TRACE, "Error RX State\n");
			break;
	}
}


static void usim_hisr_common(sim_HW_cb *hw_cb, kal_uint32 int_status)
{
	usim_dcb_struct *usim_dcb;

	usim_dcb = &usim_cb[0];

	usim_dcb->int_status = int_status;

	sim_print(SIM_DBG_TRACE, "usim_hisr_common\n");
	sim_print(SIM_DBG_TRACE, "usim_hisr int:%x\r\n",int_status);
	sim_print(SIM_DBG_TRACE, "SIM_CONF: %x\n", SIM_Reg(SIM_CONF));

	if (int_status & SIM_STS_RXERR)
	{
		sim_print(SIM_DBG_ERR, "SIM_STS_RXERR\n");
		usim_dcb->ev_status = USIM_RX_INVALID;
	}

	if(int_status& SIM_STS_EDCERR)
	{
		sim_print(SIM_DBG_ERR, "SIM_STS_EDCERR\n");
		usim_dcb->ev_status = USIM_RX_INVALID;
	}

	if(int_status& SIM_STS_TOUT)
	{
		sim_print(SIM_DBG_ERR, "SIM_STS_TOUT\n");
		usim_dcb->ev_status = USIM_NO_ATR;
	}

	if(int_status & SIM_STS_T1END)
	{
		sim_print(SIM_DBG_TRACE, "SIM_STS_T1END\n");
	}
	
	if(int_status & SIM_STS_RX)
	{
		sim_print(SIM_DBG_TRACE, "SIM_STS_RX\n");
		usim_rx_handler(int_status, hw_cb);
	}

	if(int_status & SIM_STS_SIMOFF)
	{
		sim_print(SIM_DBG_ERR, "SIM_STS_SIMOFF\n");
		usim_dcb->ev_status = USIM_POWER_OFF;
	}

	if(int_status & SIM_STS_TXERR)
	{
		sim_print(SIM_DBG_ERR, "SIM_STS_TXERR\n");
	}

	if(int_status & SIM_STS_OV)
	{
		sim_print(SIM_DBG_ERR, "SIM_STS_OV\n");
	}

	if(int_status & SIM_STS_NATR)
	{
		sim_print(SIM_DBG_TRACE, "SIM_STS_NATR\n");
		usim_dcb->ev_status = USIM_NO_ATR;
	}
}


/*we need usim_hisr and usim_hisr2, since there is no argument to tell additional information*/
static void usim_hisr(void)
{
	kal_uint32 int_status;
	sim_HW_cb *hw_cb;
	usim_dcb_struct *usim_dcb;

	usim_dcb = &usim_cb[0];

	sim_print(SIM_DBG_TRACE, "usim_hisr\n");

	int_status = SIM_Reg(SIM_STS);

	usim_hisr_common(hw_cb, int_status);
}


/*  this function pure set RSTVAL, that is, if we block RST, what level will RST stay
 *   call this function doesn't change block or pass on RST
 */
sim_MT6306_status sim_MT6306_setRST(sim_HW_cb *hw_cb, kal_uint32 level)
{
	kal_uint32 MT6306PortNo;
	sim_MT6306_switchInfo *switch_CB;
	sim_MT6306_cardInfo *card_cb;

	switch_CB = &switchCBMT6306[0];
	card_cb = &sim_MT6306_card[0];

	if(0 == level) {/*turn off*/
		switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_RST] &= ~(0x4 << 0);
		switch_CB->sim_MT6320_writer(0, switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_RST]);
		card_cb->pins.RST &= ~0x4;
	}
	else {/*set to high*/
		switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_RST] |= (0x4 << 0);
		switch_CB->sim_MT6320_writer(0, switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_RST]);
		card_cb->pins.RST |= 0x4;
	}

	return sim_MT6306_statusNoError;
}


sim_MT6306_status sim_MT6306_passCLK(sim_HW_cb *hw_cb)
{
	kal_uint32 MT6306PortNo;
	sim_MT6306_switchInfo *switch_CB;
	sim_MT6306_cardInfo	*card_cb;

	switch_CB = &switchCBMT6306[0];
	card_cb = &sim_MT6306_card[0];

#if 0
	switch(card_cb->pins.CLK)
	{
		case sim_MT6306_CLKBlockedHigh:
		case sim_MT6306_NoLDO_CLKBlockedHigh:
			{
				/*should be 0x5<<interface, others assert*/
				if((switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_CLK] & (0x5 << MT6306PortNo)) != (0x5 << MT6306PortNo))
					ASSERT(0);
				/*original is 0x5<<interface, now change to 0x1<<interface, mask 0x4<<interface*/
				switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_CLK] &= ~(0x4 << MT6306PortNo);

				sim_MT6306_SPIWrite(switch_CB, switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_CLK]);
				card_cb->pins.CLK &= 0xf0;// we must preserve the information of LDO state
				card_cb->pins.CLK |= sim_MT6306_CLKPass;
				if((switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_CLK] & (0x5 << MT6306PortNo)) != (0x1 << MT6306PortNo))
					ASSERT(0);
				break;
			}
		case sim_MT6306_CLKBlockedLow:
		case sim_MT6306_NoLDO_CLKBlockedLow:
			{
				/*should be 0x0<<interface, others assert*/
				if(switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_CLK] & (0x5 << MT6306PortNo))
					ASSERT(0);
				/*original is 0x0<<interface, now change to 0x1<<interface, set 0x1<<interface*/
				switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_CLK] |= (0x1 << MT6306PortNo);

				sim_MT6306_SPIWrite(switch_CB, switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_CLK]);
				card_cb->pins.CLK &= 0xf0;// we must preserve the information of LDO state
				card_cb->pins.CLK |= sim_MT6306_CLKPass;
				if((switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_CLK] & (0x5 << MT6306PortNo)) != (0x1 << MT6306PortNo))
					ASSERT(0);
				break;
			}
		case sim_MT6306_CLKPass:
		case sim_MT6306_NoLDO_CLKPass:
			break;
		case sim_MT6306_CLKInvalidState:
		case sim_MT6306_NoLDO_CLKInvalidState:
			ASSERT(0);
		default:
			ASSERT(0);
	}

	sim_MT6306_ClearCSTOP(hw_cb);

	/*if SIM controller is now clk running, from card's viewpoint, its clk is now starting*/
	if((DRV_Reg(SIM0_BASE_ADDR_MTK + SIM_CTRL_MTK) & SIM_CTRL_HALT) == 0x0)
	{
		switch_CB->sim_MT6306_needCLKStartTimeout = KAL_TRUE;
	}
#endif

#if 0
	MT6306PortNo = hw_cb->simSwitchPortNo;
	card_cb = sim_MT6306_get_MT6306CardCB(hw_cb);
	switch_CB = sim_MT6306_get_MT6306switchCB(hw_cb);
#if defined(__DRV_2_SIM_USING_MT6306__)
	switch(card_cb->pins.CLK)
	{
		case sim_MT6306_CLKBlockedHigh:
		case sim_MT6306_NoLDO_CLKBlockedHigh:
			{
				/*should be 0x5<<interface, others assert*/
				if((switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_CLK] & (0x5 << MT6306PortNo)) != (0x5 << MT6306PortNo))
					ASSERT(0);
				/*original is 0x5<<interface, now change to 0x1<<interface, mask 0x4<<interface*/
				switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_CLK] &= ~(0x4 << MT6306PortNo);

				sim_MT6306_SPIWrite(switch_CB, switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_CLK]);
				card_cb->pins.CLK &= 0xf0;// we must preserve the information of LDO state
				card_cb->pins.CLK |= sim_MT6306_CLKPass;
				if((switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_CLK] & (0x5 << MT6306PortNo)) != (0x1 << MT6306PortNo))
					ASSERT(0);
				break;
			}
		case sim_MT6306_CLKBlockedLow:
		case sim_MT6306_NoLDO_CLKBlockedLow:
			{
				/*should be 0x0<<interface, others assert*/
				if(switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_CLK] & (0x5 << MT6306PortNo))
					ASSERT(0);
				/*original is 0x0<<interface, now change to 0x1<<interface, set 0x1<<interface*/
				switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_CLK] |= (0x1 << MT6306PortNo);

				sim_MT6306_SPIWrite(switch_CB, switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_CLK]);
				card_cb->pins.CLK &= 0xf0;// we must preserve the information of LDO state
				card_cb->pins.CLK |= sim_MT6306_CLKPass;
				if((switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_CLK] & (0x5 << MT6306PortNo)) != (0x1 << MT6306PortNo))
					ASSERT(0);
				break;
			}
		case sim_MT6306_CLKPass:
		case sim_MT6306_NoLDO_CLKPass:
			break;
		case sim_MT6306_CLKInvalidState:
		case sim_MT6306_NoLDO_CLKInvalidState:
			ASSERT(0);
		default:
			ASSERT(0);
	}

#elif defined (DRV_2_SIM_USING_LTC4558)
	if(sim_MT6306_CLKBlockedHigh == card_cb->pins.CLK){
			/*it's impossible for LTC4558 to be in this state*/
			IMPLEMENTING_ASSERT;
	}
	else if(sim_MT6306_CLKBlockedLow == card_cb->pins.CLK){
			/*to pass CLK to interface not selected, must use CLKRUN to pass the Clk*/
			/*we don't select interface here, since if we do need to select interface, it can be done when pass other signals*/

			sim_LTC4558_CLKCtrl(MT6306Interface, 1);
			/*state of this interface has been changed*/
			card_cb->pins.CLK = sim_MT6306_CLKPass;
	}
	else
		ASSERT(0);
#endif

	sim_MT6306_ClearCSTOP(hw_cb);

	/*if SIM controller is now clk running, from card's viewpoint, its clk is now starting*/
	if((DRV_Reg(SIM0_BASE_ADDR_MTK + SIM_CTRL_MTK) & SIM_CTRL_HALT) == 0x0)
	{
		switch_CB->sim_MT6306_needCLKStartTimeout = KAL_TRUE;
	}
	sim_MT6306_addMsg(SIM_MT6306_PASS_CLK, hw_cb->simInterface, sim_MT6306_card[hw_cb->simInterface].pins.CLK, sim_MT6306_card[hw_cb->simInterface].pins.CLK);
#endif

	return sim_MT6306_statusNoError;
}


void sim_MT6306_manualReset(sim_HW_cb *hw_cb)
{
	sim_MT6306_cardInfo *card_cb;

	card_cb = &sim_MT6306_card[0];

	sim_print(SIM_DBG_TRACE, "sim_MT6306_manualReset\n");

	/*
		in design spec, we have to do following:
		(1)raise B's VCC, (2)pass B's CLK, (3)pass B's DAT, (4)set timeout, 
		(5)raise B's RST to replace SIM controller's activation
	*/

	/* VCC has been raised in USIM/SIM driver, we don't need to control VCC here */

	/* controller's clk must now running, that is why we implement manual reset */
	if (SIM_Reg(SIM_CTRL) & 0x2){
		sim_print(SIM_DBG_TRACE, "sim_MT6306_manualReset: Error, controller's clk doesn't runn\n");
	}

	/* after we pass B's clk, clk start running */
	sim_MT6306_passCLK(hw_cb);

	sim_MT6306_passDAT(hw_cb);

	// DRV_Reg(SIM0_BASE_ADDR_MTK + SIM_IRQEN_MTK) |= SIM_IRQEN_TOUT;
	SIM_SetBits(SIM_IRQEN, SIM_IRQEN_TOUT);

	/* enable timeout, must guarantee a 0->1 change */
	SIM_ClearBits(SIM_CONF, SIM_CONF_TOUTEN);
	SIM_SetBits(SIM_CONF, SIM_CONF_TOUTEN);

	/* RX IRQ has been enabled */
	/* RST should be block now */
	if ((card_cb->pins.RST & 0x1) == 0){
		sim_print(SIM_DBG_TRACE, "sim_MT6306_manualReset: Error, RST should be block now\n");
	}

	/* manual raise B's RST, not by controller */
	sim_MT6306_setRST(hw_cb, 1);
}



/*************************************************************************
* FUNCTION
*  usim_select_power
*
* DESCRIPTION
	1. Try the input voltage from application layer if availabe.
	2. Try the possible voltage which the ME can support.
	3. Get the valid TS
*
* PARAMETERS
	1. ExpectVolt: application layer give a expected power class

* RETURNS
*  KAL_TRUE: Valid TS is received
*  KAL_FALSE: Valid TS is not received, card is not present or not supported
*
* GLOBALS AFFECTED
*	usim_dcb
*
*************************************************************************/
static kal_bool usim_select_power(usim_power_enum ExpectVolt, sim_HW_cb *hw_cb)
{
	kal_uint32 retry;
	usim_dcb_struct *usim_dcb;
	int i;
	sim_print(SIM_DBG_TRACE, "usim_select_power\n");

	usim_dcb = &usim_cb[0];

	SIM_WriteReg32(SIM_COUNT, 0x01);	//Flush SIM FIFO Queue

	if(usim_dcb->warm_rst == KAL_FALSE)
	{
		sim_print(SIM_DBG_TRACE, "COLD RST\n");

		usim_deactivation(hw_cb);

		// decide the initial power class
		usim_dcb->power_in = ExpectVolt;
		if(ExpectVolt != UNKNOWN_POWER_CLASS)
		{
			sim_print(SIM_DBG_TRACE, "ExpectVolt not UNKNOWN_POWER_CLASS\n");
			usim_dcb->power = ExpectVolt;
		}
		else
		{
			sim_print(SIM_DBG_TRACE, "Start Check Power\n");
			if (usim_dcb->sim_env == ME_30V_ONLY)
			{
				sim_print(SIM_DBG_TRACE, "Power is CLASS_B_30V\n");
				usim_dcb->power = CLASS_B_30V;
			}
			else // ME_18_ONLY, ME_18V_30V
			{
				sim_print(SIM_DBG_TRACE, "Power is CLASS_C_18V\n");
				usim_dcb->power = CLASS_C_18V;
			}
		}
	}

	// start from low power class to high, if no ATR received, try another power class
	// if the an invalid TS byte is received, change the convention with the same power class
	retry = 0;
	while(retry++ < 3)
	{
		sim_print(SIM_DBG_TRACE, "select power: start from low power class to high\n\n");
		sim_print(SIM_DBG_TRACE, "retry count: %d\n", retry);

		//2007_04_12, some 3G card will give wrong ATR in the first time, and we should reset it twice
		//If we don't reset these 2 variables, former wrong path will affect next time we process ATR
		usim_dcb->abort = usim_dcb->resync = KAL_FALSE;
		usim_activation(hw_cb);

		#if !defined(TCSUPPORT_CPU_MT7520)&&!defined(TCSUPPORT_CPU_MT7505)
		mdelay(100);
		sim_print(SIM_DBG_TRACE, "1. start polling interrupt status\n");
		usim_hisr();

		#else
		sim_print(SIM_DBG_TRACE, "1. start polling interrupt status\n");
		
		for(i = 0; i<300;i++){

			if(SIM_STS_RX & SIM_Reg(SIM_STS)){
				usim_hisr();
				break;
			}
			else{
				mdelay(2);
				continue;
			}
		}
		usim_hisr();
		#endif
		if(usim_dcb->ev_status == USIM_NO_ERROR )
		{	// a correct TS byte is received
			// all ATR characters are received
			mdelay(100);
			sim_print(SIM_DBG_TRACE, "2. start polling interrupt status\n");
			usim_hisr();

			sim_print(SIM_DBG_TRACE, "USIM_NO_ERROR\n");

			if (usim_dcb->ev_status != USIM_ATR_REC){
				sim_print(SIM_DBG_TRACE, "NOT USIM_ATR_REC\n");
				return KAL_FALSE;
			} else {
				sim_print(SIM_DBG_TRACE, "USIM_ATR_REC\n");
				return KAL_TRUE;
			}
		}
		else if(usim_dcb->ev_status == USIM_ATR_REC)
		{	// all ATR characters are received
			sim_print(SIM_DBG_TRACE, "all ATR characters are received\n");
			return KAL_TRUE;
		}
		else if(usim_dcb->warm_rst == KAL_TRUE)
		{
			sim_print(SIM_DBG_TRACE, "WARM RESET\n");
			usim_deactivation(hw_cb);
			return KAL_FALSE;
		}
		else if((usim_dcb->ev_status == USIM_TS_INVALID || usim_dcb->ev_status == USIM_RX_INVALID)
			&& (usim_dcb->dir == USIM_DIRECT))
		{
			// try another convention
			usim_dcb->dir = (USIM_INVERSE == usim_dcb->dir)? USIM_DIRECT: USIM_INVERSE;
			usim_deactivation(hw_cb);
			sim_print(SIM_DBG_TRACE, "change another convention %d !!\r\n", usim_dcb->dir);
		}
		else if(usim_dcb->ev_status == USIM_NO_ATR || usim_dcb->ev_status == USIM_BWT_TIMEOUT ||
			(usim_dcb->ev_status == USIM_TS_INVALID || usim_dcb->ev_status == USIM_RX_INVALID ))
		{
			retry = 0;
			sim_print(SIM_DBG_TRACE, "USIM_NO_ATR\n");

			usim_dcb->dir = USIM_DIRECT;
			// deactivate and delay
			usim_deactivation(hw_cb);
			// change another power class if availabe, no retry with the same power class
			if (usim_dcb->sim_env == ME_18V_30V)
			{
				if (usim_dcb->power_in == UNKNOWN_POWER_CLASS && usim_dcb->power == CLASS_C_18V)
				{
					usim_dcb->power = CLASS_B_30V;
				}
				else if (usim_dcb->power_in != UNKNOWN_POWER_CLASS && usim_dcb->power_in == usim_dcb->power)
				{
					if(usim_dcb->power_in == CLASS_C_18V)
						usim_dcb->power = CLASS_B_30V;
					else
						usim_dcb->power = CLASS_C_18V;
				}
				else
				{
					return KAL_FALSE;
				}
			}
			else
			{
				return KAL_FALSE;
			}
		}
		else
		{
			sim_print(SIM_DBG_TRACE, "usim_select_power: status error\n");
		}
	}
	return KAL_FALSE;
}


void sim_MT6306_setCardState(sim_HW_cb *hw_cb, sim_MT6306_cardState cardState)
{
	sim_MT6306_switchInfo *switch_CB;
	sim_MT6306_cardInfo *card_cb;

	switch_CB = &switchCBMT6306[0];
	card_cb = &sim_MT6306_card[0];

	/*doing state assert*/
	if (sim_MT6306_stateDeactiavate == card_cb->state){
		if (sim_MT6306_stateClkStopped == cardState){
			sim_print(SIM_DBG_TRACE, "sim_MT6306_setCardState: stateClkStopped\n");
		}
	}
  
	card_cb->state = cardState;

	if (sim_MT6306_stateDeactiavate == cardState ){
		card_cb->sim_MT6306_clkStopQueued = KAL_FALSE;
	}
}


void sim_MT6306_setCardType(sim_HW_cb *hw_cb, sim_MT6306_cardType cardType)
{
	sim_MT6306_cardInfo *card_cb;

	card_cb = &sim_MT6306_card[0];
	card_cb->type = cardType;
}


/*************************************************************************
* FUNCTION
*  usim_send_block
*
* DESCRIPTION
*	1. sending a block to UICC, with header in usim_dcb->header_tx
*	2. if len > 0 then using DMA to transfer data from tx buffer to the fifo of sim
		interface.
	3. after a complete block is sent, T1END is generated
	4. after that, three bytes of received block header will come into rx fifo
*
* PARAMETERS
	adrs: tx buffer address

* RETURNS
	None

* GLOBALS AFFECTED
*	usim_dcb
*
*************************************************************************/
static void usim_send_block(kal_uint8 *adrs, sim_HW_cb *hw_cb)
{
	usim_dcb_struct *usim_dcb;
	kal_uint32 len, pcb;
	kal_uint8 *header;

	usim_dcb = &usim_cb[0];

	header = usim_dcb->header_tx;
	usim_dcb->main_state = CMD_TX_STATE;
	SIM_WriteReg32(SIM_COUNT, 1);
	
	// make sure FIFO really cleared
	SIM_Reg(SIM_COUNT);

	// write header into fifo
	len = header[T1_LEN_INDEX];
	pcb = header[T1_PCB_INDEX];
	sim_print(SIM_DBG_TRACE, "usim_send_block [00][%x][%x]\r\n", pcb, len);

//	#if defined(USIM_DEBUG)
	{
		kal_uint32 i;
		sim_print(SIM_DBG_TRACE, "tx:");
		if(USIM_IS_SBLOCK(pcb)&& len ==1)
		{
			sim_print(SIM_DBG_TRACE, " %x",header[T1_INF_INDEX]);
		}
		else
		{
			for(i=0;i<len;i++)
			{
				sim_print(SIM_DBG_TRACE, " %x",adrs[i]);
			}
		}
		sim_print(SIM_DBG_TRACE, "\r\n");
	}
//	#endif

	SIM_WriteReg32(SIM_DATA, header[T1_NAD_INDEX]);
	SIM_WriteReg32(SIM_DATA, pcb);
	SIM_WriteReg32(SIM_DATA, len);
	SIM_WriteReg32(SIM_IMP3, len);
	// transfer by DMA if the count > 12, otherwise by MCU
	SIM_SetTXTIDE(1, hw_cb);
	if(len > 12)
	{
		SIM_WriteReg32(SIM_INS, 1);
	}
	else if(USIM_IS_SBLOCK(pcb)&& len ==1)
	{
		SIM_WriteReg32(SIM_DATA, header[T1_INF_INDEX]);
		SIM_WriteReg32(SIM_INS, 1);
	}
	else
	{
		kal_uint32 i;

		for(i=0;i<len;i++){
			SIM_WriteReg32(SIM_DATA, adrs[i]);
		}

		SIM_WriteReg32(SIM_INS, 1);
	}
	SIM_WriteReg32(SIM_IRQEN, SIM_STS_T1END);
}


/*
1. send S blocks of request or response.
2. if sending request, check if the response is correct.
3. if sending response,
4. EDC will be generated(tx) and removed(rx) by T1 controller

id: PCB of the S block
param: parameter of the S-block

*/
static usim_status_enum usim_send_s_block(usim_s_block_id_enum id, kal_uint8 param, sim_HW_cb *hw_cb)	
{
	kal_uint8 *tx_buf, *rx_buf, len, t;
	kal_bool is_resp;
	kal_uint32 i;
	usim_dcb_struct *usim_dcb;

	usim_dcb = &usim_cb[0];
	
	sim_print(SIM_DBG_TRACE, "usim_send_s_block id:%x, param:%d \r\n", id, param);

	tx_buf = usim_dcb->header_tx;
	rx_buf = usim_dcb->header_rx;

	tx_buf[T1_NAD_INDEX] = USIM_NAD_DEFAULT;
	tx_buf[T1_PCB_INDEX] = id;

	is_resp = (((kal_uint32)id & PCB_S_RESP) != 0)?KAL_TRUE: KAL_FALSE;
	usim_dcb->cmd_state = (is_resp)?(S_BlOCK_RESP_TX):(S_BlOCK_REQ_TX);

	if(id==IFS_REQ || id==WTX_REQ || id==IFS_RESP|| id==WTX_RESP )
	{
		len = 4;
		tx_buf[T1_LEN_INDEX] = 1;
	}
	else
	{
		len = 3;
		tx_buf[T1_LEN_INDEX] = 0;
	}

   tx_buf[T1_INF_INDEX] = param;
   usim_dcb->retry = 0;

   while(usim_dcb->retry++ < 3)
   {
		usim_send_block(&param, hw_cb);
		mdelay(100);
		sim_print(SIM_DBG_TRACE, "3. start polling interrupt status\n");
		usim_hisr();
		if(usim_dcb->ev_status == USIM_NO_ERROR)
		{
			if(!is_resp)
			{
				for(t=0,i=0;i<len;i++)
					t += rx_buf[i]^tx_buf[i];
				if(t != PCB_S_RESP)
					continue;
			}			
			break;
		}
   }

   if(usim_dcb->retry == 4)
   {
   	usim_deactivation(hw_cb);
   }
	
   return USIM_NO_ERROR;		
}

/*************************************************************************
 * FUNCTION
 *  usim_set_protocol
 *
 * DESCRIPTION
 *    setup the physical protocol layer including T=0 and T=1.
 *
 * PARAMETERS
 *   T: physical protocol layer including T=0 and T=1.
 *
 * RETURNS
 *
 * GLOBALS AFFECTED
 *
 *************************************************************************/
static void usim_set_protocol(usim_protocol_enum T, sim_HW_cb *hw_cb)
{
	usim_dcb_struct *usim_dcb;

	usim_dcb = &usim_cb[0];

	sim_print(SIM_DBG_TRACE, "usim_set_protocol\n");

	if (T == T1_PROTOCOL)
	{
		sim_print(SIM_DBG_TRACE, "Set Protocol as T1_PROTOCOL\n");
		SIM_SetBits(SIM_CONF, SIM_CONF_T1EN);
	}
	else
	{
		sim_print(SIM_DBG_TRACE, "Set Protocol as T0_PROTOCOL\n");
		SIM_SetBits(SIM_CONF, SIM_CONF_T0EN);
	}
	usim_dcb->phy_proto = T;
}

/*************************************************************************
* FUNCTION
*  usim_process_PTS
*
* DESCRIPTION
*	1. Perform the PTS to select the protocol and enhanced speed parameter(Fn,Dn).
		T1 has higher priority than T0
*	2. Change the clock rate according to the PTS response
*	3. Enable the T0 or T1 controller according to the PTS response
*
* PARAMETERS
	None

* RETURNS
*	None
*
* GLOBALS AFFECTED
*	usim_dcb->speed
*
*************************************************************************/
static void usim_process_PTS(sim_HW_cb *hw_cb)
{
	kal_uint32 i;
	kal_uint8 pts[PPS_LEN],pts_r[PPS_LEN],pck;
	usim_speed_enum speed;
	usim_dcb_struct *usim_dcb;
	kal_bool echoed = KAL_TRUE;
	usim_dcb = &usim_cb[0];

	sim_print(SIM_DBG_TRACE, "usim_process_PTS \r\n");

	if(usim_dcb->app_proto == SIM_PROTOCOL)
	{
		sim_print(SIM_DBG_TRACE, "PTS not performed (SIM_PROTOCOL)\n");
		echoed = KAL_FALSE;
		goto exit;
	}

	if(usim_dcb->reset_mode == USIM_RESET_SPECIFIC)
	{
		sim_print(SIM_DBG_TRACE, "not performed (specific mode)\n");
		echoed = KAL_TRUE;
		goto exit;
	}

	if(usim_dcb->high_speed_en)
	{
		if(usim_dcb->Di == 1)
			speed = SPEED_372;
		else if(usim_dcb->Di == 8)
			speed = SPEED_64;
		else if(usim_dcb->Di == 16)
			speed = SPEED_32;
		else if(usim_dcb->Di == 32)
			speed = SPEED_16;
		else if(usim_dcb->Di == 64)
			speed = SPEED_8;
		else
			speed = SPEED_372;
	}
	else
	{
		speed = SPEED_372;
	}

	sim_print(SIM_DBG_TRACE, "select speed %d(372:64:32, 0:1:2)", speed);

	// generate PTS packet
	pts[PPSS] = 0xff;
	pck = 0xff;
	pts[PPS0] = USIM_PTS_PS1_MSK;

	if(usim_dcb->T1_support && usim_dcb->app_proto == USIM_PROTOCOL && !usim_dcb->T0_support ) // priority T1 > T0
	{	// T1 only usim card will go to here
		sim_print(SIM_DBG_TRACE, "select T=1\r\n");
		pts[PPS0] |= USIM_PTS_PS0_T1;
	}
	else
		sim_print(SIM_DBG_TRACE, "select T=0\r\n");

	pck ^= pts[PPS0];
	if(speed == SPEED_372)
		pts[PPS1] = ATR_TA1_372_5;
	else if(speed == SPEED_64)
		pts[PPS1] = ATR_TA1_64;
	else if(speed == SPEED_32) 	// SPEED_32
		pts[PPS1] = ATR_TA1_32;
	else if(speed == SPEED_16) 	// SPEED_16
		pts[PPS1] = ATR_TA1_16;
	else if(speed == SPEED_8) 	// SPEED_8
		pts[PPS1] = ATR_TA1_8;
	else {
		sim_print(SIM_DBG_TRACE, "usim_process_PTS Speed Error\n");
	}

	pck ^= pts[PPS1];

	// 	send PTS packet
	usim_dcb->main_state = PTS_STATE;
	pts[PCK] = pck;
	//	SIM FIFO Flush
	SIM_WriteReg32(SIM_COUNT, 0x01);
	SIM_SetRXTIDE(PPS_LEN, hw_cb);

	for(i=0;i<PPS_LEN;i++)
	{
		SIM_WriteReg32(SIM_DATA, pts[i]);
	}

	usim_set_timeout(INIT_WWT_T0, hw_cb);

	mdelay(100);
	sim_print(SIM_DBG_TRACE, "4. start polling interrupt status\n");

	usim_hisr();

	usim_set_timeout(0, hw_cb);
	if(usim_dcb->ev_status != USIM_NO_ERROR)
		echoed = KAL_FALSE;

	// read the response
	if(echoed)
	{
		for(i=0;i<PPS_LEN;i++)
		{
			pts_r[i] = SIM_Reg(SIM_DATA);
			if(pts[i]!= pts_r[i])
				echoed = KAL_FALSE;
		}
	}

	if(echoed)
	{
		sim_print(SIM_DBG_TRACE, "PTS OK!\r\n");
		usim_set_speed(speed, hw_cb);

		// Some high speed SIM card after clock rate change have to wait a while to
		// to receive the first command.		
		if(pts[1] != 0x00){
			mdelay(100);
		}
	}
	else
	{
		sim_print(SIM_DBG_TRACE, "PTS Fail!\r\n");
		usim_set_speed(SPEED_372, hw_cb);
	}

exit:
	// GSM will only use T=0, no matter t=1 is supported.
	if(usim_dcb->T1_support && !usim_dcb->T0_support &&
		(usim_dcb->app_proto == USIM_PROTOCOL) && echoed )
	{	// T1 only usim card will go to here
		sim_print(SIM_DBG_TRACE, "Init T1_PROTOCOL\n");
		usim_dcb->phy_proto = T1_PROTOCOL;
		usim_set_protocol(T1_PROTOCOL, hw_cb);
		SIM_ClearBits(SIM_CONF, (SIM_CONF_TXHSK|SIM_CONF_RXHSK));
		SIM_SetRXRetry(hw_cb, 0);
		SIM_SetTXRetry(hw_cb, 0);
	}
	else
	{
		sim_print(SIM_DBG_TRACE, "Init T0_PROTOCOL\n");
		usim_dcb->phy_proto = T0_PROTOCOL;
		if(usim_dcb->app_proto == USIM_PROTOCOL)
			usim_set_protocol(T0_PROTOCOL, hw_cb); // SIM_PROTOCOL is enabled at simd.c
		SIM_SetRXRetry(hw_cb, 7);
		SIM_SetTXRetry(hw_cb, 7);
//		USIM_ENABLE_TXRX_HANSHAKE();
		SIM_SetBits(SIM_CONF, (SIM_CONF_TXHSK|SIM_CONF_RXHSK));
	}
}


static void SIM_Initialize(kal_uint8 format, kal_uint8 power, sim_HW_cb *hw_cb)
{
	sim_print(SIM_DBG_TRACE, "SIM_Initialize\n");

	kal_uint16 tmp;
	kal_uint16 Conf;
	Sim_Card *SimCard;
	sim_MT6306_switchInfo *switch_CB;
	usim_dcb_struct *usim_dcb;
	sim_MT6306_cardInfo *card_cb;

	const kal_uint8 *tmpPtr = ClkStopTimeTable[0];
	
	card_cb = &sim_MT6306_card[0];
	switch_CB = &switchCBMT6306[0];
	SimCard = &SimCard_cb[0];
	usim_dcb = &usim_cb[0];

	SimCard->Data_format = format;
	SimCard->Power = power;

	sim_print(SIM_DBG_TRACE, "SIM_Initialize power: %d, format: %d, TS_HSK_ENABLE: %d\n", 
			power, format, SimCard->TS_HSK_ENABLE);

	//Setup the SIM control module, SIM_BRR, SIM_CONF
	//Set SIMCLK = 13M/4, and BAUD RATE = default value(F=372,D=1);
	SIM_WriteReg32(SIM_BRR, (SIM_BRR_CLK_Div4 | SIM_BRR_BAUD_Div372));

	if (format != SIM_direct)
	{
		Conf = SIM_CONF_InDirect;
	}
	else
	{
		Conf = SIM_CONF_Direct;
	}

	Conf |= SIM_CONF_SIMSEL;

	/*control the real voltage to card*/
	if (power == SIM_30V)
	{
		switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_VCC] |= (0x1 << 0);
		switch_CB->sim_MT6320_writer(0, switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_VCC]);
		/* set VSEL bit */
		card_cb->pins.VCC |= 0x1;
	}
	else{
		switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_VCC] &= ~(0x1 << 0);
		switch_CB->sim_MT6320_writer(0, switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_VCC]);
       /*clear VSEL bit*/
		card_cb->pins.VCC &= ~0x1;
	}

	if (SimCard->TS_HSK_ENABLE == KAL_TRUE)
		Conf |= (SIM_CONF_TXHSK | SIM_CONF_RXHSK);

	SIM_WriteReg32(SIM_CONF, Conf);

	if (SimCard->TS_HSK_ENABLE == KAL_TRUE)
	{
		SIM_SetRXRetry(hw_cb, 1);
		SIM_SetTXRetry(hw_cb, 1);
	}
	else
	{
		SIM_SetRXRetry(hw_cb, 0);
		SIM_SetTXRetry(hw_cb, 0);
	}

	//Set the ATRTout as 9600etu
	SIM_SetTOUT(SimCard->TOUTValue, hw_cb);

    // reset interrupts, flush rx, tx fifo
	// SIM_FIFO_Flush();
	SIM_WriteReg32(SIM_COUNT, 0x01);

	//Set the txfifo and rxfifo tide mark
	SIM_SetRXTIDE(1, hw_cb);

	//Read Interrupt Status
	tmp = SIM_Reg(SIM_STS);
	tmp = tmp; //fix assigned but not used warning

	SimCard->State = SIM_WAIT_FOR_ATR;

	//Enable Interrupt
	SIM_WriteReg32(SIM_IRQEN, (SIM_IRQEN_Normal & ~SIM_IRQEN_RXERR));
	SimCard->recDataErr = KAL_FALSE;
	//activate the SIM card, and activate the SIMCLK

	switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_VCC] |= (0x4 << 0);
	switch_CB->sim_MT6320_writer(0, switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_VCC]);

	/* in MT6306, if VCC is down, pin state have to change*/
	card_cb->pins.VCC |= 0x4;
	if(sim_MT6306_VCC18V != card_cb->pins.VCC && sim_MT6306_VCC3V != card_cb->pins.VCC){
		sim_print(SIM_DBG_TRACE, "SIM_Initialize: VCC Setting Error\n");
	}
	card_cb->pins.CLK &= ~0x10;
	card_cb->pins.DAT &= ~0x10;
	card_cb->pins.RST &= ~0x10;

	//	SIM Active
	SIM_WriteReg32(SIM_CTRL, 0x0001);
	sim_print(SIM_DBG_TRACE, "SIM ACtive\r\n");

}

static void SIM_L1Reset(sim_HW_cb *hw_cb, kal_bool maskSIMIntr)
{
	Sim_Card *SimCard;

	sim_print(SIM_DBG_TRACE, "SIM_L1Reset\n");

	SimCard = &SimCard_cb[0];
	//	SIM DisAllIntr
	SIM_WriteReg32(SIM_IRQEN, SIM_IRQEN_ALLOFF);

	//	SIM FIFO Flush
	SIM_WriteReg32(SIM_COUNT, 0x01);
	//De-activate SIM card
	if (SIM_Reg(SIM_CTRL) & SIM_CTRL_SIMON)
	{
		SimCard->State = SIM_WaitRejectDone;
		SIM_WriteReg32(SIM_IRQEN, (SIM_IRQEN_SIMOFF|SIM_IRQEN_NATR));
		SIM_ClearBits(SIM_CTRL, SIM_CTRL_SIMON);
		/* since we have do lots actions, if its context is task, there maybe chance 
		   that hisr occur before we wait event        
 		 */
	}
	else
	{
		SIM_Initialize(SimCard->Data_format,SimCard->Power, hw_cb);
	}
}

void SIM_Txhandler(sim_HW_cb *hw_cb)
{
	/*Have T0CTRL*/
	kal_uint8 index;
	kal_uint16 reslen;
	Sim_Card *SimCard;

	SimCard = &SimCard_cb[0];

	reslen = SimCard->txsize - SimCard->txindex;

	if(reslen <= 15)
	{
		for(index=0; index<reslen; index++)
		{
			SIM_WriteReg32(SIM_DATA, *(SimCard->txbuffer+SimCard->txindex));
			SimCard->txindex++;
		}
		SIM_WriteReg32(SIM_IRQEN, SIM_IRQEN_CMDNormal);
	}
	else
	{
		for(index=0; index<15; index++)
		{
			SIM_WriteReg32(SIM_DATA, *(SimCard->txbuffer+SimCard->txindex));
			SimCard->txindex++;
		}
		SIM_SetTXTIDE(0, hw_cb);
		SIM_WriteReg32(SIM_IRQEN, (SIM_IRQEN_CMDNormal|SIM_IRQEN_TX));
   }
}

void SIM_SetEvent_MT6306(Sim_Card *SIMCARD, kal_uint8 result, sim_HW_cb *hw_cb)
{
	kal_uint32 retAddr;

	sim_print(SIM_DBG_TRACE, "SIM_SetEvent_MT6306\n");

	SIMCARD->result = result;
	SIMCARD->event_state = KAL_FALSE;

	SIM_WriteReg32(SIM_IRQEN, SIM_IRQEN_ALLOFF);
}


static void SIM_Rxhandler(kal_uint16 sim_int, sim_HW_cb *hw_cb)
{
	kal_uint16 TS, Reg;
	kal_uint8  index;
	Sim_Card *SimCard;

	sim_print(SIM_DBG_TRACE, "SIM_Rxhandler\n");
	SimCard = &SimCard_cb[0];

	if (SimCard->State == SIM_WAIT_FOR_ATR)
	{
		sim_print(SIM_DBG_TRACE, "SIM_WAIT_FOR_ATR\n");
		TS = SIM_Reg(SIM_DATA);

		if ((TS == 0x003f) || (TS == 0x003b))
		{
			SimCard->State = SIM_PROCESS_ATR;

			SIM_SetBits(SIM_CONF, (SIM_CONF_TXHSK | SIM_CONF_RXHSK |SIM_CONF_TOUTEN));

			/* *(volatile kal_uint16 *)SIM_CONF |= SIM_CONF_TOUTEN; */
			SIM_SetRXTIDE(12, hw_cb);
			SIM_SetRXRetry(hw_cb, 7);
			SIM_SetTXRetry(hw_cb, 7);
			SimCard->recDataLen = 0;

			SIM_SetEvent_MT6306(SimCard, SIM_NO_ERROR, hw_cb);
			SimCard->EvtFlag = ATR_END;
			SIM_WriteReg32(SIM_IRQEN, SIM_IRQEN_Normal);
		}
		else
		{
			SimCard->reset_index++;	//Change format!!, don't change power
			if (SimCard->reset_index>1)
			{
				SimCard->reset_index = 0;
				SIM_SetEvent_MT6306(SimCard,SIM_CARDERR, hw_cb);
				sim_print(SIM_DBG_TRACE, "SIM Card Error\n");
			}
			else
			{
				if (SimCard->Data_format == SIM_indirect)
				{
					SimCard->Data_format = SIM_direct;
					SIM_L1Reset(hw_cb, KAL_FALSE);
				}
				else
				{
					SimCard->Data_format = SIM_indirect;
					SIM_L1Reset(hw_cb, KAL_FALSE);
				}
			}
		}
		return;
	}

	if (SimCard->State == SIM_PROCESS_ATR)
	{
		sim_print(SIM_DBG_TRACE, "SIM_PROCESS_ATR\n");
		while(1)
		{
			if ((SIM_Reg(SIM_COUNT) & SIM_COUNT_MASK))
			{
	         	if(40 <= SimCard->recDataLen)
				{
					sim_print(SIM_DBG_TRACE, "SIM REC Data Length Error\n");
	         	}
	            SimCard->recData[SimCard->recDataLen] = (kal_uint8)SIM_Reg(SIM_DATA);
    	        SimCard->recDataLen++;
			}
			else
			{
				if (sim_int & SIM_STS_TOUT)
	            {
    	           SIM_SetEvent_MT6306(SimCard,SIM_SUCCESS, hw_cb);
        	    }
            	break;
			}
		}
		return;
	}

	if (SimCard->State == SIM_PROCESS_PTS)
	{
		sim_print(SIM_DBG_TRACE, "SIM_PROCESS_PTS\n");
		index = 0;
		while(KAL_TRUE)
		{
			SimCard->PTS_data[index] = (kal_uint8)SIM_Reg(SIM_DATA);
			sim_print(SIM_DBG_TRACE, "1. PTS_Data %d: %x\n", index, SimCard->PTS_data[index]);
			
			index++;
			if ((SIM_Reg(SIM_COUNT) & SIM_COUNT_MASK) == 0)
			{
				SIM_SetEvent_MT6306(SimCard, SIM_NO_ERROR, hw_cb);
				break;
			}
			mdelay(100);
		}
		// SIM DisAllIntr
		SIM_WriteReg32(SIM_IRQEN, SIM_IRQEN_ALLOFF);
		return;
	}

	if (SimCard->State == SIM_PROCESSCMD)
	{
		sim_print(SIM_DBG_TRACE, "SIM_PROCESSCMD\n");
		while((SIM_Reg(SIM_COUNT) & SIM_COUNT_MASK))
		{
			sim_print(SIM_DBG_TRACE, "SIM_COUNT: %x\n", SIM_Reg(SIM_COUNT));
			*(SimCard->rxbuffer+SimCard->recDataLen) = (kal_uint8)SIM_Reg(SIM_DATA);
			sim_print(SIM_DBG_TRACE, "Data %d: %x\n", SimCard->recDataLen, (*(SimCard->rxbuffer+SimCard->recDataLen)));
			SimCard->recDataLen++;	
		}
		return;
	}
}


static void SIM_Cmdhandler(sim_HW_cb *hw_cb)
{
	Sim_Card *SimCard;

	SimCard = &SimCard_cb[0];
	sim_print(SIM_DBG_TRACE, "SIM_Cmdhandler\n");
//#ifndef TCSUPPORT_AUTOBENCH
//	SimCard->recDataLen = 0;
//#endif
	sim_print(SIM_DBG_TRACE, "Receive Data\n");

	while ((SIM_Reg(SIM_COUNT) & SIM_COUNT_MASK))
	{
		sim_print(SIM_DBG_TRACE, "receive Data Count: %d\n", SimCard->recDataLen);
		sim_print(SIM_DBG_TRACE, "SIM_COUNT: %d\n", SIM_Reg(SIM_COUNT));

		*(SimCard->rxbuffer+SimCard->recDataLen) = (kal_uint8)SIM_Reg(SIM_DATA);
		SimCard->recDataLen++;
	}

	SIM_SetEvent_MT6306(SimCard,SIM_SUCCESS, hw_cb);
	return;
}

void SIM_HISR_MT6306_Common(sim_HW_cb *hw_cb, kal_uint32 sim_int)
{
	Sim_Card *SimCard;

	SimCard = &SimCard_cb[0];

	sim_print(SIM_DBG_TRACE, "SIM_HISR_MT6306_Common\n");

	sim_print(SIM_DBG_TRACE, "SIM_INT: %x\n", sim_int);

	if ( sim_int & SIM_STS_TXERR)
	{
		sim_print(SIM_DBG_ERR, "Error!, SIM_STS_TXERR\r\n");
		if (SimCard->State == SIM_PROCESSCMD)
		{
			SimCard->State = SIM_SERIOUSERR;
			SIM_WriteReg32(SIM_IRQEN, SIM_IRQEN_TOUT);
			sim_print(SIM_DBG_ERR, "SIM STS SIM_IRQEN_TOUT\n");
		}
		else
		{
			/* PTS TX err */
			//SIM_Reject_MT6306(hw_cb);
			SIM_SetEvent_MT6306(SimCard, SIM_INIPTSERR, hw_cb);
			sim_print(SIM_DBG_ERR, "Error!, SIM STS SIM_INIPTSERR\n");
		}
	}

	if (sim_int & SIM_STS_TX)
	{
		//sim_print(SIM_DBG_TRACE, "SIM_STS_TX\r\n");
		sim_print(SIM_DBG_TRACE, "SIM_STS_TX%02x,%02x!\r\n",SIM_Reg(SIM_IMP3),sim_int);
		SIM_ClearBits(SIM_IRQEN, SIM_IRQEN_TX);
		SIM_Txhandler(hw_cb);  /* Only used for no DMA */
	}

	if (sim_int & SIM_STS_TOUT)
	{
		sim_print(SIM_DBG_TRACE, "SIM_STS_TOUT\r\n");

		if(SimCard->State == SIM_WAIT_FOR_ATR)
		{
			SimCard->EvtFlag = ATR_END;
			SIM_SetEvent_MT6306(SimCard,SIM_RX_INVALID, hw_cb);

			sim_print(SIM_DBG_ERR, "Error!, SIM STS RX_INVALID\n");
		}

		if (SimCard->State == SIM_PROCESS_ATR)
		{
			SIM_Rxhandler(sim_int, hw_cb);
		}

		if (SimCard->State == SIM_PROCESS_PTS)
		{
			SIM_SetEvent_MT6306(SimCard,SIM_INIPTSERR, hw_cb);
			sim_print(SIM_DBG_TRACE, "SIM STS SIM_INIPTSERR\n");
		}

		if (SimCard->State == SIM_PROCESSCMD)
		{
			if(SimCard->recDataErr == KAL_TRUE)
			{
				SIM_SetEvent_MT6306(SimCard,SIM_CMDRECERR, hw_cb);
				sim_print(SIM_DBG_TRACE, "SIM STS SIM_CMDRECERR\n");
			}
			else
			{
				switch(SimCard->cmdState)
				{
					case SIM_ProcessClk:
					sim_print(SIM_DBG_TRACE, "SIM_ProcessClk\r\n");
					SIM_SetEvent_MT6306(SimCard,SIM_CLKPROC, hw_cb);
					break;

					case SIM_StopClk:
	            	/*in Gemini project, we can't use HISR to stop clk*/
					sim_print(SIM_DBG_TRACE, "Error: SIM_StopClk\n");
					break;

					default: /*normal command case*/
					SimCard->timeout = KAL_TRUE;
					SIM_SetEvent_MT6306(SimCard,SIM_CMDTOUT, hw_cb);
					sim_print(SIM_DBG_TRACE, "SIM STS SIM_CMDTOUT\n");
					break;
				} /*endof switch*/
			}
		} /*if (SimCard->State == SIM_PROCESSCMD)*/

		if (SimCard->State == SIM_SERIOUSERR)
		{
			SIM_SetEvent_MT6306(SimCard,SIM_CMDTXERR, hw_cb);
			sim_print(SIM_DBG_TRACE, "Error!, SIM STS SIM_CMDTXERR\n");
		}
		SIM_WriteReg32(SIM_IRQEN, SIM_IRQEN_ALLOFF);
	}

#if 1
	if (sim_int & SIM_STS_RX)
	{
		sim_print(SIM_DBG_TRACE, "SIM_STS_RX\r\n");
		if(SimCard->timeout != KAL_TRUE)
			SIM_Rxhandler(sim_int, hw_cb);
	}

	if ( sim_int & SIM_STS_OV)
	{
		sim_print(SIM_DBG_ERR, "Error!, SIM_STS_OV\r\n");
		SimCard->recDataErr = KAL_TRUE;
		if (SimCard->State == SIM_PROCESSCMD)
		{	
			sim_print(SIM_DBG_ERR, "Error!, SIM_STS_OVERR\r\n");
		}
	}

	if ( sim_int & SIM_STS_RXERR)
	{
		sim_print(SIM_DBG_ERR, "Error!, SIM_STS_RXERR\r\n");
		SimCard->recDataErr = KAL_TRUE;
		SIM_WriteReg32(SIM_IRQEN, SIM_IRQEN_TOUT);
		if (SimCard->State == SIM_PROCESSCMD)
		{
			sim_print(SIM_DBG_ERR, "Error!, SIM_STS_RXERR, SIM_ASSERT\r\n");
		}
	}

	if ( (sim_int &  SIM_IRQEN_T0END)
       && (SimCard->State == SIM_PROCESSCMD))
	{
		sim_print(SIM_DBG_TRACE, "SIM_IRQEN_T0END\r\n");
		SIM_Cmdhandler(hw_cb);
		SIM_SetEvent_MT6306(SimCard,SIM_SUCCESS, hw_cb);
		//	SIM DisAllIntr
		SIM_WriteReg32(SIM_IRQEN, SIM_IRQEN_ALLOFF);
	}

	if (sim_int & SIM_STS_NATR)
	{
		sim_print(SIM_DBG_TRACE, "Error!, SIM_STS_NATR\r\n");
		//	SIM DisAllIntr
		SIM_WriteReg32(SIM_IRQEN, SIM_IRQEN_ALLOFF);
		SIM_SetEvent_MT6306(SimCard, SIM_NO_ATR, hw_cb);
	}

	if (sim_int & SIM_STS_SIMOFF)
	{	
		sim_print(SIM_DBG_ERR, "SIM_STS_SIMOFF\n");
		SimCard->EvtFlag = DEACTIVATE_DONE;
		/* [MAUI_03037644] FTA test fail because result should set as SIM_NOREADY */
		SIM_SetEvent_MT6306(SimCard,SIM_NOREADY, hw_cb);
	}

#endif
}


/*************************************************************************
* FUNCTION
*  L1sim_Reset
*
* DESCRIPTION
* 1. Reset the sim card and parse the ATR and perform the PTS(optional) and
		enter the command ready mode
* 2. First time it is a cold reset, second it's a warm reset
* 3. If the ExpectVolt equal to the current volt, perform a warm reset.
		Otherwise perform a cold reset.
* 4. Finally, S-block of IFS request is sent the UICC to configure the IFSD
*
* PARAMETERS
	1. ExpectVolt: application layer give a expected power class

* RETURNS
*  USIM_VOLT_NOT_SUPPORT: Valid TS is received
*  KAL_FALSE: Valid TS is not received, card is not present or not supported
*
* GLOBALS AFFECTED
*	usim_dcb
*
*************************************************************************/
static usim_status_enum L1usim_Reset(usim_power_enum ExpectVolt, usim_power_enum *ResultVolt, sim_HW_cb *hw_cb)
{
	kal_uint32 retry;
	usim_dcb_struct *usim_dcb;

	sim_print(SIM_DBG_TRACE, "L1usim_Reset\n\n");

	usim_dcb = &usim_cb[0];

	if(usim_dcb->clock_stop_en == KAL_TRUE && usim_dcb->warm_rst == KAL_TRUE &&
		usim_dcb->phy_proto == T1_PROTOCOL)
	{
		sim_print(SIM_DBG_TRACE, "L1usim Reset: warm reset & t1 protocol\n");

		if(usim_dcb->main_state == CLK_STOPPING_STATE)
		{
			usim_set_timeout(0, hw_cb);
			usim_dcb->main_state = MAIN_CMD_READY_STATE;
		}
		else if(usim_dcb->main_state == CLK_STOPPED_STATE)
		{
			SIM_ClearBits(SIM_CTRL, SIM_CTRL_HALT);
			usim_set_timeout(usim_dcb->etu_of_700, hw_cb);
			SIM_WriteReg32(SIM_IRQEN, SIM_IRQEN_TOUT);
			mdelay(100);
			sim_print(SIM_DBG_TRACE, "5. start polling interrupt status\n");

			usim_hisr();
			sim_MT6306_setCardState(hw_cb, sim_MT6306_stateClkRunning);
		}
	}

	L1usim_Init(hw_cb);

	if(usim_check_input_volt(ExpectVolt, hw_cb) == KAL_FALSE){
		sim_print(SIM_DBG_TRACE, "USIM_VOLT_NOT_SUPPORT\n");
		return USIM_VOLT_NOT_SUPPORT;
	}

	SIM_WriteReg32(SIM_IRQEN, SIM_IRQEN_ALLOFF);

	usim_set_speed(SPEED_372, hw_cb);
	usim_set_timeout(INIT_WWT_T0, hw_cb);

	// if corrupted ATRs are received, retry 3 times
	for(retry = 0; retry < ATR_RETRY; retry++)
	{
		sim_print(SIM_DBG_TRACE, "ATR Retry: %d\n", retry);

		if (usim_select_power(ExpectVolt, hw_cb) == KAL_FALSE)
		{
			if (usim_dcb->warm_rst == KAL_TRUE)
				return USIM_ATR_ERR;
			if (usim_dcb->ts_hsk_en == KAL_TRUE)
				usim_dcb->ts_hsk_en = KAL_FALSE;
			else {
				sim_print(SIM_DBG_TRACE, "USIM_NO_INSERT\n");
				return USIM_NO_INSERT;
			}

			sim_print(SIM_DBG_TRACE, "Error!, usim_select_power fail!!\n");
			//continue;
		}
		else if (usim_process_ATR(hw_cb) == USIM_NO_ERROR)
				break;

		ExpectVolt = usim_dcb->power;
	}

	if (retry == 3){
		sim_print(SIM_DBG_TRACE, "USIM_ATR_ERR\n");
		return USIM_ATR_ERR;
	}	

#if 1
	*ResultVolt = usim_dcb->power;
	// 3. Process PTS
	//if(usim_dcb->reset_mode == USIM_RESET_NEGOTIABLE)
	{
		usim_process_PTS(hw_cb);
		// 4. Configure the IFSD
		if(usim_dcb->phy_proto == T1_PROTOCOL)
		{
			if( usim_send_s_block(IFS_REQ, USIM_IFSD_MAX, hw_cb) == USIM_NO_ERROR){
				/* if we failed to send S block when negotiating IFSD and deactivate the card, 
				   we should report the reset status correctly */
				if(DEACTIVATION_STATE == usim_dcb->main_state)
					return USIM_S_BLOCK_FAIL;
				usim_dcb->ifsd = USIM_IFSD_MAX;
			}
		}
	}

	// NOTE: can't turn off the PDN bit of SIM interface over, it will cause
	// the SIM behavior abnormal.
	usim_dcb->main_state = MAIN_CMD_READY_STATE;
	usim_dcb->cmd_state = USIM_CMD_READY;
#endif
	return USIM_NO_ERROR;
}

kal_uint16 SIM_HISR_MT6306(kal_uint16 sim_sts)
{
	kal_uint16  sim_int;
	Sim_Card *SimCard;
	sim_HW_cb *hw_cb;

	SimCard = &SimCard_cb[0];

	//sim_int = SIM_Reg(SIM_STS);
	sim_int = sim_sts;

	SIM_HISR_MT6306_Common(hw_cb, sim_int);
	
	return sim_int;
}

/*
* FUNCTION
*	L1sim_Init
*
* DESCRIPTION
*   	The function L1sim_Init initialize the SIM driver.
*
* CALLS
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*   external_global
*/
void L1sim_Init_MT6306(sim_HW_cb *hw_cb)		//Validate
{
	kal_uint16 tmp;
	Sim_Card *SimCard, *peer_SimCard;
	sim_HW_cb *peerHWCb;
	sim_MT6306_switchInfo *switch_cb;
	usim_dcb_struct *usim_dcb;

	switch_cb = &switchCBMT6306[0];

	SimCard = &SimCard_cb[0];

	sim_print(SIM_DBG_TRACE, "L1sim_Init_MT6306\n\n");
	SimCard->SIM_ENV = SIM_GetCurrentEnv();
	if (SimCard->SIM_ENV == ME_30V_ONLY)
	{
		sim_print(SIM_DBG_TRACE, "SimCard Power: SIM_30V\n");
		SimCard->Power = SIM_30V;
	}
	else
	{
		sim_print(SIM_DBG_TRACE, "SimCard Power: SIM_18V\n");
		SimCard->Power = SIM_18V;
	}

	sim_print(SIM_DBG_TRACE, "Sim State: SIM_WAIT_FOR_ATR\n");

	SimCard->Data_format = SIM_direct;
	SimCard->State = SIM_WAIT_FOR_ATR;
	SimCard->clkStop = KAL_FALSE;
	SimCard->Speed = Speed372;

	/*following members are originally RW global variable, need additional initialize here*/
	SimCard->TOUTValue = SIM_DEFAULT_TOUT_VALUE;
   	SimCard->TOUT_Factor = 1;
   	SimCard->PTS_check = KAL_TRUE;

	tmp = SIM_Reg(SIM_STS);
}

void SIM_WaitEvent_MT6302(Sim_Card *SIMCARD,kal_uint32 flag, kal_bool unmaskSIMIntr, sim_HW_cb *hw_cb)
{
	SIMCARD->event_state = KAL_TRUE;
	SIMCARD->EvtFlag = flag;
}

static kal_bool SIM_PTSProcess(kal_uint8 *TxBuffaddr, kal_uint8 Txlength, sim_HW_cb *hw_cb)	//Bool lalasun
{
	kal_uint8    index = 0;
	kal_uint8    tmp = 0;
  	Sim_Card *SimCard = &SimCard_cb[0];
  	usim_dcb_struct	*usim_dcb = &usim_cb[0];
	sim_MT6306_switchInfo *switch_CB = &switchCBMT6306[0];

	//switch_CB = &switchCBMT6306[0];
	//SimCard = &SimCard_cb[0];
	//usim_dcb = &usim_cb[0];

	sim_print(SIM_DBG_TRACE, "SIM_PTSProcess, Txleng: %x\n", Txlength);

	if ((SIM_Reg(SIM_CTRL) & SIM_CTRL_SIMON) == 0)
	{
		sim_print(SIM_DBG_TRACE, "SIM_PTSProcess: ATR Fail\n");
		SimCard->sim_ATR_fail = KAL_TRUE;
		SIM_Initialize(SimCard->Data_format,SimCard->Power,hw_cb);
	}

	if (SIM_Reg(SIM_COUNT))
		SIM_WriteReg32(SIM_COUNT, 1);

	SIM_SetTOUT(SimCard->TOUTValue, hw_cb);

	for (index = 0; index < Txlength; index++)
	{
	   	SimCard->PTS_data[index]=0;
		tmp = *(TxBuffaddr+index);
		SIM_WriteReg32(SIM_DATA, tmp);
	}

	SimCard->State = SIM_PROCESS_PTS;

	SIM_SetRXTIDE(Txlength, hw_cb);
	SIM_WriteReg32(SIM_IRQEN, SIM_IRQEN_Normal);

	sim_print(SIM_DBG_TRACE, "1. start polling SIM_HISR_MT6306\n");
	mdelay(100);
	SIM_WaitEvent_MT6302(SimCard,PTS_END, KAL_FALSE, hw_cb);	
	SIM_HISR_MT6306(SIM_Reg(SIM_STS));

	if ((SimCard->recDataErr == KAL_TRUE)
	   || (SimCard->result == SIM_INIPTSERR))
	{
		SimCard->recDataErr = KAL_FALSE;
		sim_print(SIM_DBG_TRACE, "recDataErr | SIM_INIPTSERR\n");
		return KAL_FALSE;
	}

	for (index = 0; index < Txlength; index++)
	{
		sim_print(SIM_DBG_TRACE, "SimCard->PTS_data[%d] %x, Real Data: %x \n",
				index, SimCard->PTS_data[index], *(TxBuffaddr+index));

		if (SimCard->PTS_data[index]!=*(TxBuffaddr+index))
		{
			sim_print(SIM_DBG_TRACE, "PTS_Data Error\n");
			return KAL_FALSE;
		}
	}
	// Some high speed SIM card after clock rate change have to wait a while to
	// to receive the first command.
	if(SimCard->PTS_data[1] != 0x00)
		mdelay(100);

	sim_print(SIM_DBG_TRACE, "SIM_PTSProcess END\n");
	return KAL_TRUE;
}

static kal_bool SIM_ProcessATRData(sim_HW_cb *hw_cb)
{
	kal_uint8 index = 0;
	kal_uint16 tmp = 0,tmp1 = 0, Fi = 372;
	kal_uint8 ptsdata[4] = {0};
	/*TOUT is an uint32 value*/
	kal_uint32 TOUT = 0, WWT = 0, reg = 0;
	kal_uint8 Dvalue = 1;
	Sim_Card *SimCard  = &SimCard_cb[0];
	usim_dcb_struct	*usim_dcb = &usim_cb[0];
	sim_MT6306_switchInfo *switch_CB = &switchCBMT6306[0];

	sim_print(SIM_DBG_TRACE, "SIM_ProcessATRData\n");
  	/* fix build warning */
	TOUT = TOUT;
	WWT = WWT;

	//switch_CB = &switchCBMT6306[0];
	//SimCard = &SimCard_cb[0];
	//usim_dcb = &usim_cb[0];

	if (SimCard->sim_ATR_fail)
	{
		sim_print(SIM_DBG_TRACE, "SimCard: sim_ATR_fail\n");

		sim_print(SIM_DBG_TRACE, "2. start polling SIM_HISR_MT6306\n");
		SIM_WaitEvent_MT6302(SimCard,ATR_END, KAL_FALSE, hw_cb);
		mdelay(100);
		SIM_HISR_MT6306(SIM_Reg(SIM_STS));

		if (SimCard->recDataErr == KAL_TRUE)
		{
			SimCard->recDataErr = KAL_FALSE;
			return KAL_FALSE;
		}
	}
	else
	{
		sim_print(SIM_DBG_TRACE, "1. SimCard: sim_ATR_success\n");
		memcpy(SimCard->recData, usim_dcb->ATR_data+1, usim_dcb->ATR_index-1);
	}

#if 1
	index = 1;
	if (SimCard->recData[0] & 0x00f0)
	{
		if (SimCard->recData[0] & TAMask)
		{
			tmp = SimCard->recData[index]; //TA1
			index++;
			sim_print(SIM_DBG_TRACE, "TA1=%x\r\n", tmp);
			if ((tmp == 0x0011)||(tmp == 0x0001))
			{
	            //Don't process ATR data!!
	            SimCard->State = SIM_PROCESSCMD;
				SIM_SetBits(SIM_CONF, SIM_CONF_T0EN);

				/* Use 372/1 as default TOUT */
				Fi = 372;
				DRV_ICC_Calc_WWT(Fi,1,10,&WWT);
				SimCard->TOUTValue = WWT>>2;
				//SimCard->TOUTValue = SIM_CMD_TOUT_VALUE;
            	SIM_SetTOUT(SimCard->TOUTValue, hw_cb);
				return KAL_TRUE;
			}
			else
			{
				/* Set default Fi as 512 */
				Fi = 512;
    	        switch (tmp)
	            {
					case 0x0094:		//F = 512,D=8
						SimCard->sim_card_speed = sim_card_enhance_speed_64;

						if (!SimCard->PTS_check)
						{
							Dvalue = 1;
							ptsdata[0]=0xff;
						   	ptsdata[1]=0x00;
						   	ptsdata[2]=0xff;
						   	if (!SIM_PTSProcess(ptsdata, 3, hw_cb))
						   	{
								return KAL_FALSE;
						   	}
	            	         /* Use 372/1 as default TOUT */
		                     Fi = 372;
		                     DRV_ICC_Calc_WWT(Fi,1,10,&WWT);
	    	                 SimCard->TOUTValue = WWT>>2;
	        	             //SimCard->TOUTValue = SIM_CMD_TOUT_VALUE;
							SIM_SetTOUT(SimCard->TOUTValue, hw_cb);
						}
						else
						{
						   	Dvalue = 8;
						   	ptsdata[0]=0xff;
						   	ptsdata[1]=0x10;
						   	ptsdata[2]=0x94;
						   	ptsdata[3]=0x7b;
						   	SimCard->Speed = Speed64;

						   	if (!SIM_PTSProcess(ptsdata,4, hw_cb))
						   	{
						   	   return KAL_FALSE;
						   	}

							reg = SIM_Reg(SIM_BRR);
							reg &= (~SIM_BRR_ETUMSK);
							reg |= (SIM_BRR_BAUD_Div64 & SIM_BRR_ETUMSK);
							SIM_WriteReg32(SIM_BRR, reg);
						   	SimCard->TOUT_Factor = 8;  //hw-specific
							/* calc 512/8 TOUT value */
							DRV_ICC_Calc_WWT(Fi,8,10,&WWT);
							SimCard->TOUTValue = WWT>>2;
						   	//SimCard->TOUTValue = SimCard->TOUT_Factor*SIM_CMD_TOUT_VALUE;
						   	SIM_SetTOUT(SimCard->TOUTValue, hw_cb);
						}
						break;

        			case 0x0095:		//F=512,D=16
        				SimCard->sim_card_speed = sim_card_enhance_speed_32;
						if (!SimCard->PTS_check)
						{
        				 	Dvalue = 1;
	        			   	ptsdata[0]=0xff;
							ptsdata[1]=0x00;
							ptsdata[2]=0xff;
							if (!SIM_PTSProcess(ptsdata,3, hw_cb))
							{
								return KAL_FALSE;
							}
							/* Use 372/1 as default TOUT */
							Fi = 372;
							DRV_ICC_Calc_WWT(Fi,1,10,&WWT);
							SimCard->TOUTValue = WWT>>2;
		                    //SimCard->TOUTValue = SIM_CMD_TOUT_VALUE;
							SIM_SetTOUT(SimCard->TOUTValue, hw_cb);					
						}
                  		else
                  		{
							Dvalue = 16;
							ptsdata[0]=0xff;
							ptsdata[1]=0x10;
							ptsdata[2]=0x95;
							ptsdata[3]=0x7a;
							SimCard->Speed = Speed32;

							if (!SIM_PTSProcess(ptsdata,4, hw_cb))
							{
								return KAL_FALSE;
							}
							reg = SIM_Reg(SIM_BRR);
							reg &= (~SIM_BRR_ETUMSK);
							reg |= (SIM_BRR_BAUD_Div32 & SIM_BRR_ETUMSK);
							SIM_WriteReg32(SIM_BRR, reg);

						 	SimCard->TOUT_Factor = 16;
		                    /* calc 512/16 TOUT value */
							DRV_ICC_Calc_WWT(Fi,16,10,&WWT);
							SimCard->TOUTValue = WWT>>2;
							//SimCard->TOUTValue = SimCard->TOUT_Factor*SIM_CMD_TOUT_VALUE;
					      	SIM_SetTOUT(SimCard->TOUTValue, hw_cb);
						}
        				break;

					case 0x0096:		//F=512,D=32
    	    			SimCard->sim_card_speed = sim_card_enhance_speed_16;

	                	if (!SimCard->PTS_check)
    	        		{
							sim_print(SIM_DBG_TRACE, "SIM Card PTS Check : False\n");
	        				Dvalue = 1;
		        			ptsdata[0]=0xff;
							ptsdata[1]=0x00;
							ptsdata[2]=0xff;
							if (!SIM_PTSProcess(ptsdata,3, hw_cb))
							{
								return KAL_FALSE;
							}
							/* Use 372/1 as default TOUT */
							Fi = 372;
							DRV_ICC_Calc_WWT(Fi,1,10,&WWT);
							SimCard->TOUTValue = WWT>>2;
							//SimCard->TOUTValue = SIM_CMD_TOUT_VALUE;
							SIM_SetTOUT(SimCard->TOUTValue, hw_cb);
						}
	                  	else
    	              	{
							sim_print(SIM_DBG_TRACE, "SIM Card PTS Check : True\n");
		        			Dvalue = 32;
		        			ptsdata[0]=0xff;
							ptsdata[1]=0x10;
							ptsdata[2]=0x96;
							ptsdata[3]=0x79;
							SimCard->Speed = Speed16;

							if (!SIM_PTSProcess(ptsdata,4, hw_cb))
							{
								sim_print(SIM_DBG_TRACE, "1. SIM_PTSProcess Fail\n");
								return KAL_FALSE;
							}
							reg = SIM_Reg(SIM_BRR);
							reg &= (~SIM_BRR_ETUMSK);
							reg |= (SIM_BRR_BAUD_Div16 & SIM_BRR_ETUMSK);
							SIM_WriteReg32(SIM_BRR, reg);
							sim_print(SIM_DBG_TRACE, "2. SIM_BRR: %x\n", SIM_Reg(SIM_BRR));
							SimCard->TOUT_Factor = 32;
                		    /* calc 512/32 TOUT value */
							DRV_ICC_Calc_WWT(Fi,32,10,&WWT);
							SimCard->TOUTValue = WWT>>2;
							//SimCard->TOUTValue = SimCard->TOUT_Factor*SIM_CMD_TOUT_VALUE;
							SIM_SetTOUT(SimCard->TOUTValue, hw_cb);
		               	}
			        	break;

					case 0x0097:		//F=512,D=64
					   	SimCard->sim_card_speed = sim_card_enhance_speed_8;
					
						if (!SimCard->PTS_check)
						{
							Dvalue = 1;
							ptsdata[0]=0xff;
							ptsdata[1]=0x00;
							ptsdata[2]=0xff;
							if (!SIM_PTSProcess(ptsdata,3, hw_cb))
							{
								return KAL_FALSE;
							}
		                    /* Use 372/1 as default TOUT */
		                    Fi = 372;
		                    DRV_ICC_Calc_WWT(Fi,1,10,&WWT);
							SimCard->TOUTValue = WWT>>2;
							//SimCard->TOUTValue = SIM_CMD_TOUT_VALUE;
							SIM_SetTOUT(SimCard->TOUTValue, hw_cb);
						}
						else
						{
							Dvalue = 64;
							ptsdata[0]=0xff;
							ptsdata[1]=0x10;
							ptsdata[2]=0x97;
							ptsdata[3]=0x78;
							SimCard->Speed = Speed8;
					
							if (!SIM_PTSProcess(ptsdata,4, hw_cb))
							{
								return KAL_FALSE;
							}
							reg = SIM_Reg(SIM_BRR);
							reg &= (~SIM_BRR_ETUMSK);
							reg |= (SIM_BRR_BAUD_Div8 & SIM_BRR_ETUMSK);
							SIM_WriteReg32(SIM_BRR, reg);

							SimCard->TOUT_Factor = 64;
		                    /* calc 512/64 TOUT value */
		                    DRV_ICC_Calc_WWT(Fi,64,10,&WWT);
		                    SimCard->TOUTValue = WWT>>2;
						   	//SimCard->TOUTValue = SimCard->TOUT_Factor*SIM_CMD_TOUT_VALUE;
							SIM_SetTOUT(SimCard->TOUTValue, hw_cb);
	                  	}
		        		break;

	        		default:		//F=372,D=1
	        			Dvalue = 1;
	        			ptsdata[0]=0xff;
 	 				    ptsdata[1]=0x00;
					    ptsdata[2]=0xff;
					    if (!SIM_PTSProcess(ptsdata,3, hw_cb))
						{
						   return KAL_FALSE;
						}
						/* Use 372/1 as default TOUT */
						Fi = 372;
						DRV_ICC_Calc_WWT(Fi,1,10,&WWT);
						SimCard->TOUTValue = WWT>>2;
						//SimCard->TOUTValue = SIM_CMD_TOUT_VALUE;
						SIM_SetTOUT(SimCard->TOUTValue, hw_cb);
        				break;
        		}
			}
		}  /*if (SimCard->recData[0] & TAMask)*/

		if (SimCard->recData[0] & TBMask)
		{
			tmp = SimCard->recData[index];
			sim_print(SIM_DBG_TRACE, "TB1=%x\r\n",tmp);
			index++;
		}
		if (SimCard->recData[0] & TCMask)
		{
			tmp = SimCard->recData[index];
			sim_print(SIM_DBG_TRACE, "TC1=%x\r\n",tmp);
			if (tmp != 0xff && tmp != 0x00)
			{
				return KAL_FALSE;
			}
        	index++;
		}

		if (SimCard->recData[0] & TDMask)
		{
			tmp = SimCard->recData[index];	///TD1
			index++;
			sim_print(SIM_DBG_TRACE, "TD1=%x\r\n",tmp);
			if (tmp & TCMask)	//TC2 is obtain
        	{
	            if (tmp & TAMask)
				{
					tmp1 = SimCard->recData[index];
					sim_print(SIM_DBG_TRACE, "TA2=%x\r\n",tmp1);
					index++;
				}

				if (tmp & TBMask)
        		{
        			tmp1 = SimCard->recData[index];
        			sim_print(SIM_DBG_TRACE, "TB2=%x\r\n",tmp1);
        			index++;
        		}

        		if (tmp & TCMask)	//TC2
				{
					tmp1 = SimCard->recData[index];
					sim_print(SIM_DBG_TRACE, "TC2=%x\r\n",tmp1);
					/*  TOUT is an uint32 value */
//						TOUT = (960*Dvalue);
//						TOUT = (TOUT*tmp1)/4;   /*(/4)is hw-specific*/
					index++;
//					SimCard->TOUTValue = TOUT+8;
					sim_print(SIM_DBG_TRACE, "TOUT=%x\r\n",TOUT);
               
					/* Calc 512/Dvalue TOUT value*/
					DRV_ICC_Calc_WWT(Fi,Dvalue,tmp1,&WWT);
					SimCard->TOUTValue = WWT>>2;
					//SimCard->TOUTValue = SIM_CMD_TOUT_VALUE;
					SIM_SetTOUT(SimCard->TOUTValue, hw_cb);

//					if (TOUT < 0xffff)
//						SIM_WriteReg32((SIM0_BASE_ADDR_MTK + SIM_TOUT_MTK), TOUT);
//					else
//						SIM_WriteReg32((SIM0_BASE_ADDR_MTK + SIM_TOUT_MTK), 0xffff);
				}
			}
		}  /*if (SimCard->recData[0] & TDMask)*/
	}/*if (SimCard->recData[0] & 0x00f0)*/
	SimCard->State = SIM_PROCESSCMD;
	SIM_SetBits(SIM_CONF, SIM_CONF_T0EN);
//	SIMCmdInit();

	#if 0
	#ifdef NoT0CTRL
	SimCard->cmdState = SIMD_CmdIdle;
	#endif   /*NoT0CTRL*/
	#endif
#endif

	return KAL_TRUE;
}

//==========================SIM adaption=============================
/*
 * FUNCTION
 *  L1sim_PowerOff
 *
 * DESCRIPTION
 *     The function requests the driver to deactivate SIM
 *
 * CALLS
 *
 * PARAMETERS
 *  None
 *
 * RETURNS
 *  None
 *
 * GLOBALS AFFECTED
 *   external_global
 */
void sim_PowerOff_MT6306(sim_HW_cb *hw_cb)  //Validate
{
	Sim_Card *SimCard = &SimCard_cb[0];

	//SimCard = &SimCard_cb[0];

	sim_print(SIM_DBG_TRACE, "sim_PowerOff_MT6306\n");

	//	SIM DisAllIntr
	SIM_WriteReg32(SIM_IRQEN, SIM_IRQEN_ALLOFF);

	if (SIM_Reg(SIM_CTRL) | SIM_CTRL_HALT){
		SIM_ClearBits(SIM_CTRL, SIM_CTRL_HALT);
	}
}

static kal_bool SIM_ResetNoATR(kal_uint8 pow, sim_HW_cb *hw_cb) //For normal case reset
{
	//Only enable SIM interrupt
	Sim_Card *SimCard = &SimCard_cb[0];
	sim_MT6306_switchInfo *switch_CB = &switchCBMT6306[0];

	//switch_CB = &switchCBMT6306[0];
	//SimCard = &SimCard_cb[0];
   
	sim_print(SIM_DBG_TRACE, "SIM_ResetNoATR\n");

	SimCard->State = SIM_WAIT_FOR_ATR;
	SimCard->reset_index = 0;
	SimCard->Power = pow;
   
	// Deactivate the SIM card
	SIM_L1Reset(hw_cb, KAL_TRUE);

	sim_print(SIM_DBG_TRACE, "3. start polling SIM_HISR_MT6306\n");
	mdelay(100);
	SIM_WaitEvent_MT6302(SimCard,RST_READY, KAL_TRUE, hw_cb);
	SIM_HISR_MT6306(SIM_Reg(SIM_STS));

	if (SimCard->result == SIM_SUCCESS)
	{
		return KAL_TRUE;
	}
	else
	{
		return KAL_FALSE;
	}
}

/*
* FUNCTION
*	L1sim_Reset
*
* DESCRIPTION
*   	The function L1sim_Reset is used to reset SIM by specific voltage
*
* CALLS
*
* PARAMETERS
*	resetVolt: Request the driver to reset SIM at voltage resetVolt
*	resultVolt: The pointer to the voltage after the driver reset SIM.
*	(RESET_3V,RESET_5V)
*	Info: The pointer to buffer of ATR data returned from SIM
*
* RETURNS
*	SIM_NO_ERROR	No SIM error
*	SIM_NO_INSERT	No SIM inserted
*	SIM_CARD_ERROR	SIM fatal error
*
* GLOBALS AFFECTED
*   external_global
*/
static kal_uint8 L1sim_Core_Reset(kal_uint8 resetVolt, kal_uint8 *resultVolt,AtrStruct *Info, sim_HW_cb *hw_cb)	//Validate
{
	kal_uint8 index = 0;
  	Sim_Card *SimCard = NULL;
  	usim_dcb_struct	*usim_dcb = NULL;
	kal_bool  returnBool = KAL_FALSE;
	sim_MT6306_switchInfo *switch_CB = NULL;
    //switch_CB = switch_CB;
	//index = index;

	sim_print(SIM_DBG_TRACE, "L1sim_Core_Reset\n");
	switch_CB = &switchCBMT6306[0];
	SimCard = &SimCard_cb[0];
	usim_dcb = &usim_cb[0];

	SIM_WriteReg32(SIM_IRQEN, SIM_IRQEN_ALLOFF);
	mdelay(100);

	SimCard->sim_card_speed = sim_card_normal_speed;
	SimCard->reject_set_event = KAL_TRUE;
	SimCard->TOUTValue = SIM_DEFAULT_TOUT_VALUE;
	SimCard->TOUT_Factor=1;
	SimCard->clkStop = KAL_FALSE;
	SimCard->Speed = Speed372;
	SimCard->State = SIM_WAIT_FOR_ATR;
	SimCard->Power = resetVolt;
	SimCard->initialPower = resetVolt;
	SimCard->power_class = UNKNOWN_POWER_CLASS;


	SimCard->Data_format = usim_dcb->dir;
	SimCard->result = SIM_SUCCESS;
	SimCard->sim_ATR_fail = KAL_FALSE;
	SimCard->power_class = usim_dcb->power_class;

	if (SimCard->result == SIM_SUCCESS)
	{
		index=0;
		while(1)
		{
			if (!SIM_ProcessATRData(hw_cb))
			{
				index++;
				if (index == 2)
				{
				   SimCard->PTS_check = KAL_FALSE;
				}
				else if (index > 2 || KAL_TRUE == SimCard->keepAtrFatal)
				{
				   	SimCard->PTS_check = KAL_FALSE;
					return SIM_CARD_ERROR;
				}
				else if ((SimCard->Power == SIM_18V) && SimCard->power_class == CLASS_C_18V)
				{
					sim_print(SIM_DBG_TRACE, "RETRY_18V_ONLY_USIM");
					SimCard->PTS_check = KAL_TRUE;
					SimCard->SIM_ENV = ME_18V_ONLY;
					//index = 0;
				}
				/* For [MAUI_01321659] begin, retry 3V when we fail in 1.8V*/
				else if ((SimCard->Power == SIM_18V) && (SIM_GetCurrentEnv() == ME_18V_30V))
				{
					sim_print(SIM_DBG_TRACE, "RETRY_3V_ON_PTS_ERROR");
					SimCard->Power = SIM_30V;
					SimCard->PTS_check = KAL_TRUE;
					//index = 0;
				}/* For [MAUI_01321659] end */

				SimCard->sim_ATR_fail = KAL_TRUE;
				returnBool = SIM_ResetNoATR(SimCard->Power, hw_cb);

				if(KAL_TRUE != returnBool){
					sim_print(SIM_DBG_TRACE, "[SIM]:resetNoATR failed, hahahaahahahahahhahaahhahaha");
					return SIM_CARD_ERROR;
				}
			}
			else
			{
				if (resultVolt != NULL)
				{
					*resultVolt = SimCard->Power;
				}

				if (Info != NULL)
				{
					for (index = 0;index < SimCard->recDataLen;index++)
					{
						Info->info[index] = SimCard->recData[index];
					}
				}
				return SIM_NO_ERROR;
			}
		}
	}
	else
	{
		return SIM_CARD_ERROR;
	}
}




kal_uint8 sim_Reset_MT6306(kal_uint8 resetVolt, kal_uint8 *resultVolt, AtrStruct *Info, sim_HW_cb *hw_cb)	//Validate
{
	kal_uint8 result=0;
	Sim_Card *SimCard;

	SimCard = &SimCard_cb[0];
	
	sim_print(SIM_DBG_TRACE, "sim_Reset_MT6306\n");

	SimCard->TS_HSK_ENABLE = KAL_TRUE;

	result = L1sim_Core_Reset(resetVolt,resultVolt,Info, hw_cb);

#if 1
	if (result != SIM_NO_ERROR && KAL_FALSE == SimCard->keepAtrFatal)
  	{
		sim_print(SIM_DBG_TRACE, "[SIM]: SIM reset fail with TS_HSK_ENABLE\n");
		SimCard->TS_HSK_ENABLE = KAL_FALSE;
        //no atr, give VSIM power again
        SIM_Initialize(SimCard->Data_format,SimCard->Power, hw_cb);
		result = L1sim_Core_Reset(resetVolt,resultVolt,Info, hw_cb);
	}
	if(result == SIM_NO_ERROR)
	{
		sim_MT6306_setCardState(hw_cb, sim_MT6306_stateClkRunning);
		sim_print(SIM_DBG_TRACE, "[SIM]:SIM RESET OK, power:%d ,speed:%d\n",SimCard->Power,SimCard->Speed);
	}
	else
	{
		sim_MT6306_setCardState(hw_cb, sim_MT6306_stateDeactiavate);
		sim_print(SIM_DBG_TRACE, "[SIM]:SIM RESET FAIL, result:%d\n", result);
	}
#endif
	return result;
}


/*************************************************************************
 * FUNCTION
 *  usim_update_sim_to_ready
 *
 * DESCRIPTION
 *   1. update the ATR informations from usim_dcb into SimCard
 *       to make sim(t=0) driver work..
 *
 * PARAMETERS
 *
 * RETURNS
 *
 * GLOBALS AFFECTED
 *   SimCard
 *   TOUTValue
 *
 *************************************************************************/
void static usim_update_sim_to_ready(sim_HW_cb *hw_cb)
{
	usim_dcb_struct *usim_dcb;
	Sim_Card *SimCard;
	usim_dcb = &usim_cb[0];
	SimCard = &SimCard_cb[0];

	sim_print(SIM_DBG_TRACE, "usim_update_sim_to_ready\n");

	SimCard->app_proto = usim_dcb->app_proto;
	SimCard->State = SIM_PROCESSCMD;
	SimCard->Data_format = usim_dcb->dir;
	if(usim_dcb->power == CLASS_B_30V)
		SimCard->Power = SIM_30V;
	else if(usim_dcb->power == CLASS_C_18V)
		SimCard->Power = SIM_18V;
	SimCard->SIM_ENV = usim_dcb->sim_env;
	SimCard->Speed = usim_dcb->speed;
	SimCard->clkStop = usim_dcb->clock_stop_en;
	if(usim_dcb->clock_stop_type == CLOCK_STOP_HIGH)
		SimCard->clkStopLevel = KAL_TRUE;
	else if(usim_dcb->clock_stop_type == CLOCK_STOP_LOW)
		SimCard->clkStopLevel = KAL_FALSE;
	SimCard->sim_card_speed = (sim_card_speed_type)usim_dcb->card_speed;
	SimCard->TOUTValue = usim_dcb->WWT>>2;
	SimCard->power_class = usim_dcb->power_class;
}


#if 1
/*************************************************************************
* FUNCTION
*  L1sim_Reset_All
*
* DESCRIPTION
*	1. general interface of sim reset for T=0 and T=1
*	2. it support warm reset for UICC
*	3. first enable error repeat handling process to cover parity error at ATR, if not
*		success, disable it.
*	4. for SIM protocol with T=0, additional reset will be perfromed.
*
* PARAMETERS
*	ExpectVolt: expected input voltage for the SIM card.
*	ResultVolt: finally used power voltage.
*	warm: specify warm reset for UICC
*
* RETURNS
*
* GLOBALS AFFECTED
*
*************************************************************************/
usim_status_enum L1sim_Reset_MT6306(sim_power_enum ExpectVolt, sim_power_enum *ResultVolt, kal_bool warm, sim_HW_cb *hw_cb)
{
	usim_status_enum status = 0;
	sim_MT6306_switchInfo *switch_CB = NULL;
	usim_dcb_struct *usim_dcb = NULL;
	Sim_Card *SimCard = NULL;

	sim_print(SIM_DBG_NONE, "L1sim_Reset_MT6306\n\n");

	usim_dcb = &usim_cb[0];
	switch_CB = &switchCBMT6306[0];

	sim_MT6306_change(hw_cb, sim_MT6306_eventReset);

	if(warm == KAL_FALSE){
		sim_print(SIM_DBG_DEBUG, "cold reset\n");
		status = L1usim_Reset(ExpectVolt, ResultVolt, hw_cb);
		if(status < 0){
			sim_print(SIM_DBG_DEBUG, "L1usim_Reset Fail\n");
		} else {
			kal_uint32 i;
			kal_char *p;

			sim_print(SIM_DBG_DEBUG, "L1usim Reset Success\n");

			/*reset successfully, record its IR or AL state*/
			if(SIM_Reg(SIM_ATRSTA) & 0x100){
				sim_print(SIM_DBG_DEBUG, "AL card !!!!\n");
				sim_MT6306_setCardType(hw_cb, sim_MT6306_cardTypeAL);
			}
			else if(SIM_Reg(SIM_ATRSTA) & 0x80){
				sim_print(SIM_DBG_DEBUG, "IR card !!!!\n");
				sim_MT6306_setCardType(hw_cb, sim_MT6306_cardTypeIR);
			}
			else {
				sim_print(SIM_DBG_DEBUG, "Error Card !!!!\n");
			}

			if(USIM_DIRECT != usim_dcb->dir){
				sim_print(SIM_DBG_DEBUG, "indirect card!!!!\n\r");
			}

			sim_print(SIM_DBG_DEBUG, "[SIM_DRV]:ATR= ");
			for(i = 0; i< usim_dcb->ATR_index; i++)
			{
				sim_print(SIM_DBG_DEBUG, "%02X ",usim_dcb->ATR_data[i]);
			}
			sim_print(SIM_DBG_DEBUG, "\n");

			sim_MT6306_setCardState(hw_cb, sim_MT6306_stateClkRunning);
			sim_print(SIM_DBG_ERR, "\n[SIM_DRV]: L1usim_Reset OK v: %d, T: %d, speed:%d\n",
			  	    usim_dcb->power, usim_dcb->phy_proto, usim_dcb->card_speed);
		}

		if(usim_dcb->phy_proto == T0_PROTOCOL)
		{

			kal_uint8 s;
			kal_uint8 power;

			sim_print(SIM_DBG_DEBUG, "USIM PHY Protocol: T0_PROTOCOL\n");

			SimCard = &SimCard_cb[0];
			L1sim_Init_MT6306(hw_cb);

			if (usim_dcb->power == CLASS_C_18V)
				power = SIM_18V;
			else
				power = SIM_30V;

			if (usim_dcb->app_proto == SIM_PROTOCOL){
				s = sim_Reset_MT6306(power, NULL, NULL, hw_cb);
				if (SimCard->Power == SIM_18V){
					*ResultVolt = CLASS_C_18V;
				} else {
					*ResultVolt = CLASS_B_30V;
				}
			} else {
				usim_update_sim_to_ready(hw_cb);
			}	
			sim_MT6306_recordDirectionBaud(hw_cb);
		} else {
			sim_MT6306_recordDirectionBaud(hw_cb);
		}
#if 0//origin code compile error ,so delete it
	} else {
		sim_print(SIM_DBG_TRACE, "warm reset\n");
		if (usim_dcb->app_proto == USIM_PROTOCOL)
		{
			usim_dcb->warm_rst = KAL_TRUE;
			status = L1usim_Reset(usim_dcb->power, ResultVolt, hw_cb);
			usim_dcb->warm_rst = KAL_FALSE;
			if (status <0)
			{
				//L1sim_PowerOff_All(simInterface);
				L1usim_PowerOff(hw_cb);
				sim_MT6306_setCardState(hw_cb, sim_MT6306_stateDeactiavate);
				return status;
			}

			sim_MT6306_setCardState(hw_cb, sim_MT6306_stateClkRunning);
			if (usim_dcb->phy_proto == T0_PROTOCOL)
			{
				kal_uint8 power = 0;

				SimCard = &SimCard_cb[0];
				L1sim_Init_MT6306(hw_cb);
				if (usim_dcb->power == CLASS_C_18V)
					power = SIM_18V;
				else
					power = SIM_30V;
				if (usim_dcb->app_proto == SIM_PROTOCOL)
				{
					sim_Reset_MT6306(power, NULL, NULL, hw_cb);
					if (SimCard->Power == SIM_18V)
						*ResultVolt = CLASS_C_18V;
					else
						*ResultVolt = CLASS_B_30V;
				} else {
					usim_update_sim_to_ready(hw_cb);
				}
			}
			sim_MT6306_recordDirectionBaud(hw_cb);
		} else {
			status = USIM_INVALID_WRST;
		}
#endif
	}
	return status;
}


sim_status L1sim_Cmd_MT6306(kal_uint8 *txData, kal_uint32 *txSize, kal_uint8 *rxData, kal_uint32 *rxSize, sim_HW_cb *hw_cb)
{

	return 0;
}

void L1sim_PowerOff_MT6306(sim_HW_cb *hw_cb)
{
	Sim_Card *SimCard = NULL;

	SimCard = &SimCard_cb[0];
	
	SIM_WriteReg32(SIM_IRQEN, SIM_IRQEN_ALLOFF);

	if(SIM_Reg(SIM_CTRL) | SIM_CTRL_HALT){
		SIM_ClearBits((SIM_CTRL), SIM_CTRL_HALT);
	}

	return;
}

sim_ctrlDriver sim_ctrlDriver_MT6306 = {
	L1sim_Reset_MT6306,
	L1sim_Cmd_MT6306,
	L1sim_PowerOff_MT6306
//	L1sim_Get_Card_Info_MT6306,
//	L1sim_Enable_Enhanced_Speed_MT6306,
//	L1sim_Select_Prefer_PhyLayer_MT6306,
//	L1sim_Set_ClockStopMode_MT6306,
//	L1sim_EOC_MT6306,
//	sim_MT6306_addMsg
};

void MT6306_HW_I2C_writer(kal_uint8 addr, kal_uint8 data_addr, kal_uint8 data_value)
{
	UINT8 u1Value[0x20] = {0};
	UINT16 ret;

	ret = SIF_X_Write(0, 0xc7, 0x64, 1, (UINT32)data_addr, &data_value, 1);

	if (ret != 1){
		sim_print(SIM_DBG_TRACE, "MT6306 I2C Write Addr %x Value %x Fail\n", data_addr, data_value);
	} else {
		sim_print(SIM_DBG_TRACE, "MT6306 I2C Write Addr %x Value %x Success\n", data_addr, data_value);
	}

	return (kal_uint8)(ret & 0xff);
}

kal_uint8 MT6306_HW_I2C_reader(kal_uint8 device_addr, kal_uint8 data_addr)
{
	UINT8 u1Value[0x20] = {0};
	UINT16 ret = 0;

	ret =  SIF_X_Read(0, 0xc7, 0x64, 1, (UINT32)data_addr, u1Value, 1);

	if (ret != 1){
		sim_print(SIM_DBG_TRACE, "MT6306 I2C Read Fail\n");
	} else {
		sim_print(SIM_DBG_TRACE, "Read_Addr: %x\n", data_addr);
		sim_print(SIM_DBG_TRACE, "Read_Data: %x\n", u1Value[0]);
	}

	return (kal_uint8)(ret & 0xff);
}

/*_____________________________________________________________________________
**      function name: MT6306_HW_Check
**      descriptions:
**             Check MT6306 hardware  is valid or not, by trying read I2C .
**
**      parameters:
**             n: 	retry time.
**
**      return:
**             Vaild: 		0
**             Not Vaild: 	-1
**
**      call:
**      	None
**
**      revision:
**____________________________________________________________________________
*/

int MT6306_HW_Check(int n)
{
	UINT8 u1Value[0x20] = {0};
	UINT16 ret = 0;
	int i= n;

	
	if( isEN751221 && (isFPGA==0) ){
		return 0;
	}

	while(i--){
		ret =  SIF_X_Read(0, 0xc7, 0x64, 1, 0x03, u1Value, 1);

		if (ret != 1){
			sim_print(SIM_DBG_NONE, "MT6306 I2C Read Fail\n");
		} else {
			sim_print(SIM_DBG_NONE, "Read_Data: %x\n", u1Value[0]);
			return 0;
		}
	}

	return 1;
}

void MT6306_Writer_AL(kal_uint8 chipno, kal_uint16 data)
{
	kal_uint16 tmpaddr = 0;
	kal_uint8 addr = 0, value = 0;
	kal_uint32 retAddr = 0;


        	/* no MT6306 */
	if( isEN751221 && (isFPGA==0) ){
		return 0;
	}

	/*
		data is a 12 bit value, use uint16 to store its value, its format is :
		|   11:8        |   7:4     |   3:0     |
    		addr_high       addr_low        data
		and we want to send data to MT6306 as following format:
		|S|         |W|A|   11:8        |   7:4     |   all zero    |   3:0     |
		start             R/W   addr_high      addr_low        stuff 0     data
        	device addr
	                	ack
	*/

	tmpaddr = (data & 0xFF0);
	addr = (kal_uint8) (tmpaddr >> 4);  //MT6306 I2C Spec.
	value = (kal_uint8)(data & 0x0F);

	I2CWriterPtr(MT6306_getDeviceAddr(), addr, value);
	
	mdelay(100);	

	I2CReaderPtr(MT6306_getDeviceAddr(), addr);
}

void MT6306_i2cInit(sim_MT6306_SPIWriter *spiWritePtr, kal_uint32 chipNo)
{
	kal_uint32 i, j;

	*spiWritePtr = (sim_MT6306_SPIWriter)MT6306_Writer_AL;

	//MT6306_HW_I2C_init(MT6306_getDeviceAddr());
	I2CWriterPtr = MT6306_HW_I2C_writer;
	I2CReaderPtr = MT6306_HW_I2C_reader;
}

void sim_MT6306_switchInit(sim_MT6306_switchInfo *switch_CB, kal_uint32 MT6306Interface)
{
	sim_print(SIM_DBG_TRACE, "sim_MT6306_switchInit\n\n");

    switch_CB->MT6306ChipNo = MT6306Interface;

    MT6306_i2cInit(&switch_CB->sim_MT6320_writer, MT6306Interface);

	switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_RST] = 0x00;//default all value 0, and addr is 0
	switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_CLK] = 0x13;//CLK register is default in block low state
	switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_DAT] = 0x20;//default all value 0, and addr is 2
	switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_VCC] = 0x30;//default all value 0, and addr is 3
	switch_CB->sim_MT6306_regValue[SIM_MT6306_REG_CARDSS] = 0x8C;//SIM1=card 1/2, SIM2=card 3/4

}

void sim_MT6306_init()
{
	kal_uint32 maskedValue;
	kal_uint32 i;

	sim_MT6306_switchInfo *switch_CB;
	sim_MT6306_cardInfo *card_cb;

	sim_print(SIM_DBG_TRACE, "sim_MT6306_init\n\n");

	switch_CB = &switchCBMT6306[0];
	sim_MT6306_switchInit(switch_CB, 0);

	for(i = 0; i < DRV_SIM_MAX_LOGICAL_INTERFACE; i++){
		card_cb = &sim_MT6306_card[i];
		card_cb->pins.CLK = sim_MT6306_NoLDO_CLKPass;
		card_cb->pins.RST = sim_MT6306_NoLDO_RSTPass_Low;
		card_cb->pins.DAT = sim_MT6306_NoLDO_DATBlockedHigh;
		card_cb->pins.VCC = sim_MT6306_VCC18VDisable;
	}
}


usim_status_enum L1sim_Reset_All(sim_power_enum ExpectVolt, sim_power_enum *ResultVolt, kal_bool warm, SIM_ICC_APPLICATION application)
{
	usim_status_enum status;

	sim_print(SIM_DBG_DEBUG, "L1sim_Reset_All\n\n");
	/*find out the hooked function table*/
	status = sim_driver->reset(ExpectVolt, ResultVolt, warm, NULL);
//	sim_driver->EOC((sim_HW_cb *)(hwCbArray[simInterface]));

	return status;
}


#if 1
int DRV_ICC_interface_init(SIM_ICC_APPLICATION application)
{
#if 1
	sim_HW_cb *hw_cb;
	kal_uint32 simInterface, peerInterface;
	kal_uint32 MT6302Info;

#ifdef __SIM_HOT_SWAP_SUPPORT__
	kal_uint32 cmd, value;
	sim_HW_cb *simHwCb;
#endif
	
	sim_print(SIM_DBG_TRACE, "DRV_ICC_interface_init\n\n");
	
	sim_driver = &sim_ctrlDriver_MT6306;

	sim_MT6306_init();
#endif
	return 0;
}
#endif

void sim_init_hwCb(void)
{
	kal_uint32 loopIndex;
	
	sim_print(SIM_DBG_TRACE, "sim_init_hwCb()\n\n");
	for(loopIndex = 0; DRV_SIM_MAX_LOGICAL_INTERFACE > loopIndex; loopIndex++){
		simHWCbArray[loopIndex].head = SIM_HW_CB_HEAD;
		simHWCbArray[loopIndex].tail= SIM_HW_CB_TAIL;
	}
}

void sim_init_all_cb(void)
{
	kal_uint32 loopIndex;

	sim_print(SIM_DBG_TRACE, "sim_init_all_cb()\n\n");

	sim_init_hwCb();

	for(loopIndex = 0; iccSlotNum > loopIndex; loopIndex++){
		DRV_ICC_interface_init(iccMappingTable[loopIndex].application);
	}
}
#endif

#define CMD_RECBUFSIZE  8 //13

#define SIM_ObtainSW(_SW) \
{\
	kal_uint16 _SW1;\
	kal_uint16 _SW2;\
	_SW1 = SIM_Reg(SIM_SW1);\
	_SW2 = SIM_Reg(SIM_SW2);\
	_SW = (_SW2 | (_SW1 << 8));\
}


/*
* FUNCTION
*	SIM_CMD
*
* DESCRIPTION
*   	The function is used to transmit coded command and
*	its following data to the driver.
*
* CALLS
*
* PARAMETERS
*	txData: Pointer to the transmitted command and data.
*	txSize:	The size of the transmitted command and data from AL to driver.
*	expSize: The size of expected data from SIM
*	result: Pointer to received data
* 	rcvSize: Pointer to the size of data received
*	parityError: 1 (parity error) or 0(no parity error)
*
* RETURNS
*	status(high byte:sw1 low byte: sw2)
*
* GLOBALS AFFECTED
*   external_global
*/
#define GEMINI_CLKSTART3
#define GEMINI_NEW_DELAY
void loop_count(int count)
{
	volatile int i =0;
	while(i<count)
		i++;
}

unsigned long g_loop_count = 800;
unsigned long g_loop_interval = 1;
extern void delay1us(int period, int number);

static kal_uint16 SIM_CMD(kal_uint8  *txData,kal_uint16  txSize,kal_uint8  *result, kal_uint16  *rcvSize, kal_uint8 *Error, sim_HW_cb *hw_cb)
{
	kal_uint16 SW = 0;
	kal_uint8 index;
	kal_uint32 loop_index = 0,delay_index = 0;
	kal_uint16 INS;
	kal_uint16 temp;
	kal_uint16 expSize = *rcvSize;
	Sim_Card *SimCard;
	kal_uint32 clkStartTime1;
	kal_uint16 temp_reg;
	kal_uint32 savedMask = 0;
	sim_MT6306_switchInfo	*switch_CB;
	sim_MT6306_cardInfo	*card_cb;
	Sim_Card *PeerSimCard, *SimCardNow;
	sim_HW_cb *peerHWCb;
	int writePtrIdx = 0,tmpLoop;
	kal_uint16 STS = 0, T0END_timeout = 0;

//	delayIndex = delayIndex;
	SimCard = &SimCard_cb[0];
	switch_CB = &switchCBMT6306[0];
	card_cb = &sim_MT6306_card[0];

	if(result == NULL && *rcvSize != 0)
	{
		sim_print(SIM_DBG_TRACE, "Error: result is Null & rcvSize > 0\n");
		*Error = KAL_TRUE;
	  	return 0;
	}

	if (SimCard->State != SIM_PROCESSCMD)
	{
		sim_print(SIM_DBG_TRACE, "Error: SimCard State not SIM_PROCESSCMD\n");
	   *Error = KAL_TRUE;
	   return 0;
	}

//	sim_print(SIM_DBG_TRACE, "cmd header: ");
	for (index=0; index<7; index++)
	{
		sim_print(SIM_DBG_TRACE, "%x ",*(txData+index));
	}
	sim_print(SIM_DBG_TRACE, "\r\n ");

	/*for clock stop mode*/
//	SIM_DisAllIntr();
	SIM_WriteReg32(SIM_IRQEN, SIM_IRQEN_ALLOFF);

//	sim_print(SIM_DBG_TRACE, "SIM_IRQEN: %x\n", SIM_Reg(SIM_IRQEN));
//	sim_print(SIM_DBG_TRACE, "SIM_CTRL: %x\n", SIM_Reg(SIM_CTRL));

	if (0x3 == SIM_Reg(SIM_CTRL)){
		sim_print(SIM_DBG_TRACE, "Error !! SIM State: Deactivate, CLK: Disable\n");
	}

	sim_MT6306_setCardState(hw_cb, sim_MT6306_stateClkRunning);
	if(sim_MT6306_CLKPass != card_cb->pins.CLK){
		sim_print(SIM_DBG_TRACE, "Error SIM CLKPass Setting\n");
	}

	// SIM DisAllIntr
	SIM_WriteReg32(SIM_IRQEN, SIM_IRQEN_ALLOFF);
	mdelay(100); 

	if(0x1 != SIM_Reg(SIM_CTRL)){
		sim_print(SIM_DBG_TRACE, "Error SIM State: Deactivate\n");
	}

	if(KAL_FALSE == switch_CB->sim_MT6306_taskAccessing){
		sim_print(SIM_DBG_TRACE, "SIM Task Access Invalid\n");
	}

	SIM_ClearBits(SIM_CONF, SIM_CONF_TOUTEN);
//	sim_print(SIM_DBG_TRACE, "SIM_CONF: %x\n", SIM_Reg(SIM_CONF));

	SIM_SetTOUT(SimCard->TOUTValue, hw_cb);
	//  SIM FIFO Flush
	SIM_WriteReg32(SIM_COUNT, 0x01);

	*Error = KAL_FALSE;
	SimCard->recDataErr = KAL_FALSE;

	SimCard->txbuffer = txData;
	SimCard->txsize = txSize;
	SimCard->rxbuffer = result;
	SimCard->recDataLen = 0;

	INS = (kal_uint16)*(txData+1);

//	sim_print(SIM_DBG_TRACE, "Data Transfer Length: %d\n", (*(txData+4)));
	SIM_WriteReg32(SIM_IMP3, (*(txData+4)));

//	SIM_SetIMP3(*(txData+4));
	SimCard->cmdState = SIM_WaitCmdEnd;
//	sim_print(SIM_DBG_TRACE, "sim_cmd before tx, ins=%x sts=%x,dbg_reg=%x \n",SIM_Reg(SIM_INS),SIM_Reg(SIM_STS),SIM_Reg(0xbfbf0590));
#if defined(TCSUPPORT_CPU_MT7520)||defined(TCSUPPORT_CPU_MT7505)
	loop_count(100);
#endif
	if(txSize <= 15)
	{
		for(index=0; index<txSize; index++)
		{
if( isEN751221 && (isFPGA==0) ){
				/* SIM_REG_DBG is not support in EN7512 */
			}
			else{
#if defined(TCSUPPORT_CPU_MT7520)||defined(TCSUPPORT_CPU_MT7505)
			writePtrIdx = SIM_Reg(SIM_REG_DBG) & SIM_DBG_WRITE_PTR_MASK;
#endif
}

			SIM_WriteReg32(SIM_DATA, *(txData+index));
if( isEN751221 && (isFPGA==0) ){
				/* SIM_REG_DBG is not support in EN7512 */
				loop_count(100);
			}
			else
			{
#if defined(TCSUPPORT_CPU_MT7520)||defined(TCSUPPORT_CPU_MT7505)
			for(tmpLoop = 0; tmpLoop<100;tmpLoop++){
				if(writePtrIdx != (SIM_Reg(SIM_REG_DBG) & SIM_DBG_WRITE_PTR_MASK))
					break;
			}
#endif			
		}
}

		SimCard->txindex = txSize;
		temp = SIM_Reg(SIM_STS);
		SIM_WriteReg32(SIM_IRQEN, SIM_IRQEN_CMDNormal);

//		sim_print(SIM_DBG_TRACE, "IRQEN: %x\n", SIM_Reg(SIM_IRQEN));
	}
	else
	{
		for (index=0; index<15; index++)
		{
//			sim_print(SIM_DBG_TRACE, "Write Data %d: %x\n", index, *(txData+index));
			SIM_WriteReg32(SIM_DATA, *(txData+index));
//			sim_print(SIM_DBG_TRACE, "TX SIM_COUNT: %d\n", SIM_Reg(SIM_COUNT));
#if defined(TCSUPPORT_CPU_MT7520)||defined(TCSUPPORT_CPU_MT7505)
			for(tmpLoop = 0; tmpLoop<100;tmpLoop++){
				if(writePtrIdx != (SIM_Reg(SIM_REG_DBG) & SIM_DBG_WRITE_PTR_MASK))
					break;
			}
#endif
		}
		SimCard->txindex = 15;
		SIM_SetTXTIDE(0, hw_cb);
		temp = SIM_Reg(SIM_STS);
		SIM_WriteReg32(SIM_IRQEN, (SIM_IRQEN_CMDNormal|SIM_IRQEN_TX));
	}

	if (expSize > 0)
	{
		if (expSize > 15)
		{
			SIM_SetRXTIDE(CMD_RECBUFSIZE, hw_cb);
		}
		else
		{
			SIM_SetRXTIDE(expSize, hw_cb);
		}
//		sim_print(SIM_DBG_TRACE, "0.Ready RX SIM_COUNT: %d\n", SIM_Reg(SIM_COUNT));
      /* maybe changed for 64k rate */
	}
	else
	{
		INS |= SIM_INS_INSD;
	}
	
	{
#ifndef    NoT0CTRL
#ifdef	SIM_NULLBYTE_ISSUE
		SimCard->sim_nullByteIssueNullCount = 0;
		SimCard->simMagic1 ++;
		SimCard->simMagic2 = SimCard->simMagic1;
		switch_CB->nullByteGPTServingInterface = hw_cb->simInterface;
#endif
#endif   /*NoT0CTRL*/

		/*in case */
		SimCard->EvtFlag = 0x0;
#ifndef    NoT0CTRL
		SIM_WriteReg32(SIM_INS, INS);
#endif   /*NoT0CTRL*/

	}

//	sim_print(SIM_DBG_TRACE, "3. Ready RX SIM_COUNT: %d\n", SIM_Reg(SIM_COUNT));
	SIM_SetBits(SIM_CONF, SIM_CONF_TOUTEN);

	SIM_WaitEvent_MT6302(SimCard, SIM_EVT_CMD_END, KAL_FALSE, hw_cb);
#if 0//!defined(TCSUPPORT_CPU_MT7520)&&!defined(TCSUPPORT_CPU_MT7505)
	mdelay(100);
	SIM_HISR_MT6306(SIM_Reg(SIM_STS));
#else
	for(loop_index = 0; loop_index < g_loop_count; loop_index++)
	{
		for(delay_index=0; delay_index < 0x20; delay_index++){			
			STS = SIM_Reg(SIM_STS);
			if(STS){
				SIM_HISR_MT6306(STS);
				//sim_print(SIM_DBG_DEBUG, "STS:%02x delay:%02x!\n", STS, delay_index); //for OV ERR
				break;
			}
			delay1us(20,g_loop_interval); //Delay 100us
		}
		//sim_print(SIM_DBG_TRACE, "SIM_HISR_MT6306 Run, After delay100us Now Recv len is [%d]\n", SimCard->recDataLen);
		//SIM_HISR_MT6306(STS);
		//ndelay(1000 * 100); //100us
		#if 1
		if((INS & SIM_INS_INSD) && (SIM_Reg(SIM_IMP3) == 0) ){					
			sim_print(SIM_DBG_TRACE, "SIM_CMD:send %02x IRQEN:%02x STS:%02x done:%04x!\n", txSize, SIM_Reg(SIM_IRQEN), STS, loop_index);
			break;
		}	
		
		if((INS & SIM_INS_INSD) == 0 && expSize ==  SimCard->recDataLen){
			sim_print(SIM_DBG_DEBUG, "SIM_CMD:rece %02x IRQEN:%02x STS:%02x done:%04x!\n", expSize, SIM_Reg(SIM_IRQEN), STS, loop_index);
			break;
		}

		if(STS & SIM_STS_T0END){
			sim_print(SIM_DBG_DEBUG, "SIM_CMD: expSize %04x rece %04x STS:%02x done:%04x!\n", expSize, SimCard->recDataLen, STS, loop_index);
			break;
		}
		#endif
		sim_print(SIM_DBG_TRACE, "SIM_HISR_MT6306 Run, Now Recv len is [%d]\n", SimCard->recDataLen);
	}
#endif
	//delay100us(400); //Delay 100us
#if 1	
	sim_print(SIM_DBG_DEBUG, "start wait T0END SW:%02x%02x STS:%02x!\n",SIM_Reg(SIM_SW1),SIM_Reg(SIM_SW2), STS);
	T0END_timeout = 1;
	for(loop_index = 0; loop_index <= 3000; loop_index++)
	{			
		if(STS & SIM_STS_T0END){
			sim_print(SIM_DBG_TRACE, "wait T0END SW:%02x STS:%02x done:%04x!\n",SIM_Reg(SIM_SW1), STS, loop_index);
			T0END_timeout = 0;
			break;
		}
		delay1us(100,2);
		STS  =SIM_Reg(SIM_STS);
	}
	if(T0END_timeout) sim_print(SIM_DBG_DEBUG, "SIM_CMD(%02x):SW:%02x STS:%02x timeout!\n",txData[1],SIM_Reg(SIM_SW1),STS);
#endif
#if 0	
	sim_print(SIM_DBG_NONE, "SW:%02x%02x IRQEN:%02x STS:%02x done:%04x!\n",SIM_Reg(SIM_SW1),SIM_Reg(SIM_SW2),SIM_Reg(SIM_IRQEN), STS, loop_index);
	for(loop_index = 0; loop_index < 2400; loop_index++)
	{			
		STS  =SIM_Reg(SIM_STS);		
		delay100us(2);
		if(0x60  !=SIM_Reg(SIM_SW1)){
			sim_print(SIM_DBG_NONE, "SW:%02x STS:%02x done:%04x!\n",SIM_Reg(SIM_SW1), STS, loop_index);
			break;
		}
	}
#endif

	if(0x3 == SIM_Reg(SIM_CTRL)){
		sim_print(SIM_DBG_TRACE, "Error !!, SIM State: Deactivate, CLK: Disable\n");
	}

	if(((card_cb->baud << 2)|0x1) != SIM_Reg(SIM_BRR)){
		sim_print(SIM_DBG_TRACE, "Error !!, SIM Baud Error\n");
	}
#ifdef TCSUPPORT_AUTOBENCH
	if(SimCard->recDataLen !=expSize){
		sim_print(SIM_DBG_TRACE, "recv data len(%d) NOT EQUAL expSize(%d),0860=%x\n",SimCard->recDataLen,expSize,SIM_Reg(0xbfb00860));
		sim_autobench_result ++;
	}
#endif
	//SIM_WriteReg32(SIM_IRQEN, SIM_IRQEN_ALLOFF);
	if (SimCard->result == SIM_SUCCESS && SimCard->recDataErr == KAL_FALSE && !T0END_timeout)
	{
		*rcvSize = SimCard->recDataLen;
		sim_print(SIM_DBG_TRACE, "SIM_SUCCESS, Recv len is [%d]\n", SimCard->recDataLen);
		SIM_ObtainSW(SW);
		sim_print(SIM_DBG_TRACE, "SW=%x\r\n",SW);
		return SW;
	}
	else
	{
		sim_print(SIM_DBG_DEBUG, "[SIM]:SIM_CMD fail status=%d recDataErr=%d \n", SimCard->result, SimCard->recDataErr);
   	}
    //SimCard->recDataErr = KAL_FALSE;
	
	if(T0END_timeout) {
		*Error = KAL_FALSE;
		SW = SIM_EXCUTE_NULL;
	}
	else{
	SimCard->is_err = KAL_TRUE;
	*Error = KAL_TRUE;
		SW = SIM_PARITY_ERR;
	}
	sim_print(SIM_DBG_DEBUG, "Error: [SIM]:SIM_CMD fail status=%d\n", SimCard->result);
	return SW;
}


/*
* FUNCTION
*	L1sim_Cmd
*
* DESCRIPTION
*   	The function is used to implement re-try command mechanism.
*
* CALLS
*
* PARAMETERS
*	txData: Pointer to the transmitted command and data.
*	txSize:	The size of the transmitted command and data from AL to driver.
*	expSize: The size of expected data from SIM
*	result: Pointer to received data
* 	rcvSize: Pointer to the size of data received
*	parityError: 1 (parity error) or 0(no parity error)
*
* RETURNS
*	status(high byte:sw1 low byte: sw2)
*
* GLOBALS AFFECTED
*   external_global
*/
static kal_uint16 L1sim_Cmd(kal_uint8 *txData, kal_uint16 txSize, kal_uint8 *result, kal_uint16 *rcvSize, kal_uint8 *Error, sim_HW_cb *hw_cb)
{
	kal_uint8 index;
	kal_uint16 SW;
	Sim_Card *SimCard;
	usim_dcb_struct *usim_dcb;

	SimCard = &SimCard_cb[0];
	usim_dcb = &usim_cb[0];
	int i = 0;

	sim_print(SIM_DBG_TRACE, "L1sim_Cmd(1) txSize=%d, rcvSize=%d\n", txSize, *rcvSize);

	if (SimCard->State != SIM_PROCESSCMD)
	{
		sim_print(SIM_DBG_TRACE, "Error: SimCard State not SIM_PROCESSCMD\n");

		*Error = KAL_TRUE;
		return 0;
	}
	SimCard->get9000WhenSelect = KAL_FALSE;

	// while encounter physical errors, deactivate the SIM immediately
	for(index=0;index<3;index++)
	{
		sim_print(SIM_DBG_TRACE, "Round %d\n", index);
		SimCard->timeout = KAL_FALSE;

		SW = SIM_CMD(txData,txSize,result,rcvSize,Error, hw_cb);
	#if 0
		sim_print(SIM_DBG_DEBUG,"TX:");
		for(i=0; i< txSize;i++ ){
			sim_print(SIM_DBG_DEBUG, "%02x", txData[i]);
		}
		sim_print(SIM_DBG_DEBUG,"<%02x>!\n",txSize);
	#endif
		sim_print(SIM_DBG_TRACE, "%d SW : %x\n", index, SW);

		if(0x9000 == SW && 0xA4 == txData[1]){
		   	sim_print(SIM_DBG_ERR, "[SIM]:0x9000 on select");
			SimCard->get9000WhenSelect = KAL_TRUE;
		}

		sim_print(SIM_DBG_TRACE, "L1sim_Cmd(2) txSize=%d, rcvSize=%d, fifo: %d\n", txSize, *rcvSize, SIM_Reg(SIM_COUNT));

		if(SimCard->timeout && SimCard->app_proto == USIM_PROTOCOL)
		{
			sim_print(SIM_DBG_ERR, "SimCard Timeout\n");
			/* SimCard->timeout's life cycle should be only in this L1sim_Cmd, reset to false before we leave */
			SimCard->timeout = KAL_FALSE;

		   	/*USIM FTA requires to deactivate the card after timeout in 100ms*/
		   	/*if we will use manual deactivate, we don't need to pass RST*/
//			if(KAL_FALSE == sim_MT6306_QueryNeedManualControl(hw_cb)){
			if (1){
				sim_MT6306_passRST(hw_cb);
			}
			sim_PowerOff_MT6306(hw_cb);
			/*set card not present to prevent next SIM command rush in*/
			usim_dcb->present = KAL_FALSE;
			sim_MT6306_setCardState(hw_cb, sim_MT6306_stateDeactiavate);
			return SW;
		}

		/* [MAUI_03009364]If card is removed, we don't need to wait status words */
		if(hw_cb->IsCardRemove && *Error == KAL_TRUE)
		{
			/*SimCard->timeout's life cycle should be only in this L1sim_Cmd, reset to false before we leave*/
			SimCard->timeout = KAL_FALSE;
			/*set card not present to prevent next SIM command rush in*/
			usim_dcb->present = KAL_FALSE;
			return SW;
		}

		if (*Error == 0)
			break;
	} /* end for */
	if( SW == SIM_EXCUTE_NULL ){
		SimCard->timeout = KAL_FALSE;
		return SIM_EXCUTE_NULL;
	}

	if (((SW&0xf000) != 0x6000) && ((SW&0xf000) != 0x9000))
	{
		sim_print(SIM_DBG_ERR, "Error: SW Data Error\n");
		*Error = KAL_TRUE;
	}

	if(*Error)
	{
		sim_print(SIM_DBG_ERR, "[SIM]:SIM_CMD fail result=%d, status=%x", SimCard->result, SW);
		sim_print(SIM_DBG_ERR, "[SIM]: recDataErr=%d, timeout=%d, cmdState=%d, event_state=%d, EvtFlag=%d, clkStop=%d, app_proto=%d",
				SimCard->recDataErr, SimCard->timeout, SimCard->cmdState, SimCard->event_state,
				SimCard->EvtFlag, SimCard->clkStop, SimCard->app_proto);
	}else{
	#if 0
		sim_print(SIM_DBG_DEBUG,"Rx:");
		for(i=0; i< *rcvSize;i++ ){
			sim_print(SIM_DBG_DEBUG, "%02x", result[i]);
		}
		sim_print(SIM_DBG_DEBUG,"<%02x>,SW=%04x!\n", *rcvSize,SW);
	#endif
	}

#ifdef SIM_CACHED_SUPPORT_WRITE_THROUGH_SERIES
	invalidate_wt_cache((kal_uint32)result, *rcvSize);
#endif

   /*SimCard->timeout's life cycle should be only in this L1sim_Cmd, reset to false before we leave*/
   SimCard->timeout = KAL_FALSE;
   return SW;
}


//================================For engineer mode end==========================
//================================SIM test code==================================
kal_uint8 Volt;
kal_uint8 resVolt;
AtrStruct ATRInfo;

#if 1
void PinDisable(kal_uint8 *inputdata,kal_uint16 *res)
{
	kal_uint8 cmd[50], i, j;
	kal_uint8 resp[50];
	kal_uint16 rec;
	kal_uint16 SW;
	kal_uint8 parity_err;
	sim_HW_cb *hw_cb;

	hw_cb = &simHWCbArray[0];

	sim_print(SIM_DBG_TRACE, "DISABLE CHV\n");

	cmd[0]= 0xa0;
	cmd[1]= 0x26;
	cmd[2]= 0x00;
	cmd[3]= 0x01;
	cmd[4]= 0x08;

	for (j=0;j<8;j++)
	{
		cmd[j+5]= *(inputdata+j);
	}

	//SIM_Command(&sim_cmd);
	rec = 0;
	SW = L1sim_Cmd(cmd, 13, (kal_uint8 *)resp, &rec, &parity_err, hw_cb);
	if (parity_err)
	{
		sim_print(SIM_DBG_TRACE, "Parity Error\r\n");
	}

	sim_print(SIM_DBG_TRACE, "PinDisable SW=%x\r\n",SW);

	for (i= 0;i<rec;i++)
	{
		sim_print(SIM_DBG_TRACE, "The rec. data[%d]=%x\r\n", i, res[i]);
	}
}

void PinEnable(kal_uint8 *inputdata,kal_uint16 *res)
{
	kal_uint8 cmd[50], i, j;
	kal_uint8 resp[50];
	kal_uint16 rec;
	kal_uint16 SW;
	kal_uint8 parity_err;
	sim_HW_cb *hw_cb;

	hw_cb = &simHWCbArray[0];

	sim_print(SIM_DBG_TRACE, "ENABLE CHV\n");

	cmd[0]= 0xa0;
	cmd[1]= 0x28;
	cmd[2]= 0x00;
	cmd[3]= 0x01;
	cmd[4]= 0x08;

	for (j=0;j<8;j++)
	{
		cmd[j+5]= *(inputdata+j);
	}

	//SIM_Command(&sim_cmd);
	rec = 0;
	SW = L1sim_Cmd(cmd, 13, (kal_uint8 *)resp, &rec, &parity_err, hw_cb);
	if (parity_err)
	{
		sim_print(SIM_DBG_TRACE, "Parity Error\r\n");
	}

	sim_print(SIM_DBG_TRACE, "PinEnable SW=%x\r\n",SW);

	for (i= 0;i<rec;i++)
	{
		sim_print(SIM_DBG_TRACE, "The rec. data[%d]=%x\r\n", i, res[i]);
	}
}

void PinChange(kal_uint8 *inputdata,kal_uint16 *res)
{
	kal_uint8 cmd[50], i, j;
	kal_uint8 resp[50];
	kal_uint16 rec;
	kal_uint16 SW;
	kal_uint8 parity_err;
	sim_HW_cb *hw_cb;

	hw_cb = &simHWCbArray[0];

	sim_print(SIM_DBG_TRACE, "CHANGE CHV\n");

	cmd[0]= 0xa0;
	cmd[1]= 0x24;
	cmd[2]= 0x00;
	cmd[3]= 0x01;
	cmd[4]= 0x10;

	for (j=0;j<16;j++)
	{
		cmd[j+5]= *(inputdata+j);
	}

	//SIM_Command(&sim_cmd);
	rec = 0;
	SW = L1sim_Cmd(cmd, 21, (kal_uint8 *)resp, &rec, &parity_err, hw_cb);
	if (parity_err)
	{
		sim_print(SIM_DBG_TRACE, "Parity Error\r\n");
	}

	sim_print(SIM_DBG_TRACE, "PinUnBlock SW=%x\r\n",SW);

	for (i= 0;i<rec;i++)
	{
		sim_print(SIM_DBG_TRACE, "The rec. data[%d]=%x\r\n", i, res[i]);
	}
}

void PinUnBlock(kal_uint8 *inputdata,kal_uint16 *res)
{
	kal_uint8 cmd[50] = {0}, i = 0, j = 0;
	kal_uint8 resp[50] ={0};
	kal_uint16 rec = 0;
	kal_uint16 SW = 0;
	kal_uint8 parity_err = 0;
	sim_HW_cb *hw_cb = NULL;

	hw_cb = &simHWCbArray[0];

	sim_print(SIM_DBG_TRACE, "UNBLOCK CHV\n");

	cmd[0]= 0xa0;
	cmd[1]= 0x2c;
	cmd[2]= 0x00;
	cmd[3]= 0x00;
	cmd[4]= 0x10;

	for (j=0;j<16;j++)
	{
		cmd[j+5]= *(inputdata+j);
	}

	//SIM_Command(&sim_cmd);
	rec = 0;
	SW = L1sim_Cmd(cmd, 21, (kal_uint8 *)resp, &rec, &parity_err, hw_cb);
	if (parity_err)
	{
		sim_print(SIM_DBG_TRACE, "Parity Error\r\n");
	}

	sim_print(SIM_DBG_TRACE, "PinUnBlock SW=%x\r\n",SW);

	for (i= 0;i<rec;i++)
	{
		sim_print(SIM_DBG_TRACE, "The rec. data[%d]=%x\r\n", i, res[i]);
	}
}

void PinCheck(kal_uint8 *inputdata,kal_uint16 *res)
{
	kal_uint8 cmd[50] = {0}, i = 0, j = 0;
	kal_uint8 resp[50] = {0};
	kal_uint16 rec = 0;
	kal_uint16 SW = 0;
	kal_uint8 parity_err = 0;
	sim_HW_cb *hw_cb = NULL;

	hw_cb = &simHWCbArray[0];

	sim_print(SIM_DBG_NONE, "VERIFY CHV\n");

	cmd[0]= 0xa0;
	cmd[1]= 0x20;
	cmd[2]= 0x0;
	cmd[3]= 0x01;
	cmd[4]= 0x08;

	for (j=0;j<8;j++)
	{
		cmd[j+5]= *(inputdata+j);
	}

	//SIM_Command(&sim_cmd);
	rec = 0;
	SW = L1sim_Cmd(cmd, 13, (kal_uint8 *)resp, &rec, &parity_err, hw_cb);
	if (parity_err)
	{
		sim_print(SIM_DBG_NONE, "Parity Error\r\n");
	}

	sim_print(SIM_DBG_NONE, "PinCheck SW=%x\r\n",SW);

	for (i= 0;i<rec;i++)
	{
		sim_print(SIM_DBG_NONE, "The rec. data[%d]=%x\r\n", i, res[i]);
	}
}


void CmdStatus(kal_uint8 *inputdata, kal_uint16 *res)
{
	int i;
	kal_uint8 cmd[50] = {0};
	kal_uint8 resp[50]= {0};
	kal_uint8 check_mode = 0, clock_mode = 0;

	kal_bool fail = KAL_FALSE;
	kal_uint16 tx_size= 0, rx_size = 0;
	kal_uint16 rec = 0;
	kal_uint8 parity_err = 0;
	kal_uint16 SW = 0;
	sim_HW_cb *hw_cb = NULL;

	hw_cb = &simHWCbArray[0];

	cmd[0]= 0xa0;
	cmd[1]= 0xf2;
	cmd[2]= 0x0;
	cmd[3]= 0x0;
	cmd[4]= 0x02;

	cmd[5]= *inputdata;
	cmd[6]= *(inputdata+1);
	rec = 0;
	rx_size = 0;

	sim_print(SIM_DBG_NONE, "Cmd Status\n");
	
	SW = L1sim_Cmd(cmd, 7, (kal_uint8 *)resp, &rec, &parity_err, hw_cb);

	if ((SW & 0xf000) == 0x6000)
		fail = KAL_TRUE;
	else
		fail = KAL_FALSE;

	if (KAL_FALSE == fail){
		check_mode=resp[13]&CHECK_CLOCK_MODE; /* bit mask for check SIM clock mode */
	}

	if (check_mode==0x00){               /* Check SIM card clock mode */
		clock_mode=CLOCK_STOP_NOT_SUPPORT;
		sim_print(SIM_DBG_NONE, "CLOCK_STOP_NOT_SUPPORT\n");
	} else if ((check_mode==0x04)||(check_mode==0x05)){
		clock_mode=CLOCK_STOP_HIGH;
		sim_print(SIM_DBG_NONE, "CLOCK_STOP_HIGH\n");
	} else {
		clock_mode=CLOCK_STOP_LOW;
		sim_print(SIM_DBG_NONE, "CLOCK_STOP_LOW\n");
	}

	if (parity_err)
	{
		sim_print(SIM_DBG_NONE, "Parity Error\r\n");
	}
	*res = (kal_uint8)(SW >> 8);
	*(res+1) = (kal_uint8)SW;
	sim_print(SIM_DBG_NONE, "Status SW=%x\r\n",SW);

	for (i= 0; i<rec; i++)
	{
		sim_print(SIM_DBG_NONE, "The rec. data[%d]=%x\r\n",i,res[i]);
	}
	sim_print(SIM_DBG_NONE, "Cmd Status cmd is ok\r\n");
}

int SelectFile(kal_uint8 *inputdata, kal_uint16 *res)
{
	int i;
	kal_uint8 cmd[50];
	kal_uint8 resp[50];
	kal_uint8 check_mode, clock_mode;

	kal_bool fail;
	kal_uint16 tx_size, rx_size;	
	kal_uint16 rec;
	kal_uint8 parity_err;
	kal_uint16 SW;
	sim_HW_cb *hw_cb;
	hw_cb = &simHWCbArray[0];

	cmd[0]= 0xa0;
	cmd[1]= 0xa4;
	cmd[2]= 0x0;
	cmd[3]= 0x0;
	cmd[4]= 0x02;

	cmd[5]= *inputdata;
	cmd[6]= *(inputdata+1);
	rec = 0;
	rx_size = 0;

	sim_print(SIM_DBG_TRACE, "SelectFile\n");
	SW = L1sim_Cmd(cmd, 7, (kal_uint8 *)resp, &rec, &parity_err, hw_cb);

	if ((SW & 0xf000) == 0x6000)
		fail = KAL_TRUE;
	else
		fail = KAL_FALSE;

	if (KAL_FALSE == fail){
		check_mode=resp[13]&CHECK_CLOCK_MODE; /* bit mask for check SIM clock mode */
	}

	if (check_mode==0x00){               /* Check SIM card clock mode */
		clock_mode=CLOCK_STOP_NOT_SUPPORT;
	} else if ((check_mode==0x04)||(check_mode==0x05)){
		clock_mode=CLOCK_STOP_HIGH;
	} else {
		clock_mode=CLOCK_STOP_LOW;
	}

	if (parity_err)
	{
		sim_print(SIM_DBG_DEBUG, "Parity Error\r\n");
	}
	*res = (kal_uint8)(SW >> 8);
	*(res+1) = (kal_uint8)SW;
	sim_print(SIM_DBG_TRACE, "SelectFile SW=%x\r\n",SW);

	for (i= 0; i<rec; i++)
	{
		sim_print(SIM_DBG_TRACE, "The rec. data[%d]=%x\r\n",i,res[i]);
	}
	sim_print(SIM_DBG_TRACE, "Select File cmd is ok\r\n");

	return SW;
}


unsigned int GetFileUpdate(kal_uint16 *res)
{
	int i;
	kal_uint8 cmd[50];
	kal_uint8 resp[50];
	kal_uint8 check_mode, clock_mode;

	kal_bool fail;
	kal_uint16 tx_size, rx_size;	
	kal_uint16 rec;
	kal_uint8 parity_err;
	kal_uint16 SW;
	sim_HW_cb *hw_cb;
	hw_cb = &simHWCbArray[0];

	cmd[0]= 0xa0;
	cmd[1]= 0xe1;
	cmd[2]= 0x0;
	cmd[3]= 0x0;
	cmd[4]= 0x0;

	rec = 0;
	rx_size = 0;

	sim_print(SIM_DBG_TRACE, "GetFileUpdate\n");
	SW = L1sim_Cmd(cmd, 5, (kal_uint8 *)resp, &rec, &parity_err, hw_cb);

	if ((SW & 0xf000) == 0x6000)
		fail = KAL_TRUE;
	else
		fail = KAL_FALSE;

	if (parity_err)
	{
		sim_print(SIM_DBG_DEBUG, "Parity Error\r\n");
	}
	*res = (kal_uint8)(SW >> 8);
	*(res+1) = (kal_uint8)SW;
	sim_print(SIM_DBG_DEBUG, "GetFileUpdate SW=%x\r\n",SW);
	return SW;

}

void GetResponse(kal_uint8 len, kal_uint16 *res)
{
	kal_uint8 cmd[50];
	kal_uint16 i;
	kal_uint16 rec;
	kal_uint8 resp[50];
	kal_uint16 SW;
	kal_uint8 parity_err;
	sim_HW_cb *hw_cb;

	hw_cb = &simHWCbArray[0];

	cmd[0]= 0xa0;
	cmd[1]= 0xc0;
	cmd[2]= 0x0;
	cmd[3]= 0x0;
	cmd[4]= len;

	//SIM_Command(&sim_cmd);
	sim_print(SIM_DBG_NONE, "GetResponse\n");
	sim_print(SIM_DBG_NONE, "Response Length: %d\n", len);

	rec = len;
	SW = L1sim_Cmd(cmd, 5, (kal_uint8 *)resp, &rec, &parity_err, hw_cb);

	sim_print(SIM_DBG_NONE, "GetResponse Actural Length: %d\r\n", rec);
	
	for(i=0;i<rec;i++)
	{
		res[i] = resp[i];
		sim_print(SIM_DBG_NONE, "Data%d: 0x%02x\r\n", i, res[i]);
	}

	*(res+rec) = (kal_uint8)(SW >> 8);
	*(res+rec+1) = (kal_uint8)SW;
	if (parity_err)
	{
		sim_print(SIM_DBG_NONE, "Parity Error\r\n");
	}

	sim_print(SIM_DBG_NONE, "GetResponse SW=%x\r\n",SW);

	sim_print(SIM_DBG_NONE, "the voltage value=%x\r\n",res[13]);
}

kal_uint16 GetResponse_CT(kal_uint8 len, kal_uint8 *res)
{
	kal_uint8 cmd[50];
	kal_uint16 i;
	kal_uint16 rec;
	kal_uint8 resp[50];
	kal_uint16 SW;
	kal_uint8 parity_err;
	sim_HW_cb *hw_cb;

	hw_cb = &simHWCbArray[0];

	cmd[0]= 0xa0;
	cmd[1]= 0xc0;
	cmd[2]= 0x0;
	cmd[3]= 0x0;
	cmd[4]= len;

	//SIM_Command(&sim_cmd);
	sim_print(SIM_DBG_TRACE, "GetResponse\n");
	sim_print(SIM_DBG_TRACE, "Response Length: %d\n", len);

	rec = len;
	SW = L1sim_Cmd(cmd, 5, (kal_uint8 *)resp, &rec, &parity_err, hw_cb);

	sim_print(SIM_DBG_TRACE, "GetResponse Actural Length: %d\r\n", rec);
	
	for(i=0; i<rec; i++)
	{
		res[i] = resp[i];
		sim_print(SIM_DBG_TRACE, "Data%d: 0x%02x ", i, res[i]);
	}
	sim_print(SIM_DBG_TRACE, "!\n", i, res[i]);
	
	*(res+rec) = '\0';

	if (parity_err)
	{
		sim_print(SIM_DBG_DEBUG, "GetResponse Parity Error\r\n");
	}
	else
	{
		sim_print(SIM_DBG_TRACE, "GetResponse_CT Success\r\n");
	}

	sim_print(SIM_DBG_TRACE, "GetResponse SW=%x\r\n",SW);

	return SW;
}

kal_uint16 SimCMD_CT(kal_uint8 sim_cmd, kal_uint8 *inputdata, kal_uint8 inputlen, kal_uint8 *res){
	kal_uint8 cmd[256] = {0};
	kal_uint16 i;
	kal_uint16 rec;
	kal_uint8 resp[50];
	kal_uint16 SW;
	kal_uint8 parity_err;
	sim_HW_cb *hw_cb;

	hw_cb = &simHWCbArray[0];

	cmd[0]= 0xa0;
	cmd[1]= sim_cmd;
	cmd[2]= 0x0;
	cmd[3]= 0x0;
	cmd[4]= inputlen;

	/*Refill with random*/
	memcpy(&cmd[5], inputdata, inputlen);
	
	//SIM_Command(&sim_cmd);
	sim_print(SIM_DBG_TRACE, "VerifyCard_CT\n");
	sim_print(SIM_DBG_TRACE, "VerifyCard_CT Length: %d\n");

	rec = 0;
	SW = L1sim_Cmd(cmd, 5 + inputlen, (kal_uint8 *)resp, &rec, &parity_err, hw_cb);

	sim_print(SIM_DBG_TRACE, "VerifyCard_CT Actural Length: %d\r\n", rec);
	
	for(i=0; i<rec; i++)
	{
		res[i] = resp[i];
		sim_print(SIM_DBG_TRACE, "Data%d: 0x%02x\r\n", i, res[i]);
	}

	*(res+rec) = '\0';

	if (parity_err)
	{
		sim_print(SIM_DBG_DEBUG, "CMD(%02x): Parity Error\r\n",sim_cmd);
	}
	else
	{
		sim_print(SIM_DBG_TRACE, "VerifyCard_CT Success\r\n");
	}

	sim_print(SIM_DBG_TRACE, "VerifyCard_CT SW=%x\r\n",SW);

	return SW;
}


kal_uint16 VerifyCard_CT(kal_uint8 *inputdata, kal_uint8 *res)
{
	kal_uint8 cmd[50] = {0};
	kal_uint16 i;
	kal_uint16 rec;
	kal_uint8 resp[50];
	kal_uint16 SW;
	kal_uint8 parity_err;
	sim_HW_cb *hw_cb;

	hw_cb = &simHWCbArray[0];

	cmd[0]= 0xa0;
	cmd[1]= 0xe2;
	cmd[2]= 0x0;
	cmd[3]= 0x0;
	cmd[4]= 0x10;

	/*Refill with random*/
	memcpy(&cmd[5], inputdata, 16);
	
	//SIM_Command(&sim_cmd);
	sim_print(SIM_DBG_TRACE, "VerifyCard_CT\n");
	sim_print(SIM_DBG_TRACE, "VerifyCard_CT Length: %d\n");

	rec = 0;
	SW = L1sim_Cmd(cmd, 5 + 16, (kal_uint8 *)resp, &rec, &parity_err, hw_cb);

	sim_print(SIM_DBG_TRACE, "VerifyCard_CT Actural Length: %d\r\n", rec);
	
	for(i=0; i<rec; i++)
	{
		res[i] = resp[i];
		sim_print(SIM_DBG_TRACE, "Data%d: 0x%02x\r\n", i, res[i]);
	}

	*(res+rec) = '\0';

	if (parity_err)
	{
		sim_print(SIM_DBG_DEBUG, "VerifyCard Parity Error\r\n");
	}
	else
	{
		sim_print(SIM_DBG_TRACE, "VerifyCard_CT Success\r\n");
	}

	sim_print(SIM_DBG_TRACE, "VerifyCard_CT SW=%x\r\n",SW);

	return SW;
}

kal_uint16 VerifyCPE_CT(kal_uint8 *inputdata, kal_uint8 *res)
{
	kal_uint8 cmd[50] = {0};
	kal_uint16 i;
	kal_uint16 rec;
	kal_uint8 resp[50];
	kal_uint16 SW;
	kal_uint8 parity_err;
	sim_HW_cb *hw_cb;

	hw_cb = &simHWCbArray[0];

	cmd[0]= 0xa0;
	cmd[1]= 0xe3;
	cmd[2]= 0x0;
	cmd[3]= 0x0;
	cmd[4]= 0x08;

	/*Refill with random*/
	memcpy(&cmd[5], inputdata, 8);
	
	//SIM_Command(&sim_cmd);
	sim_print(SIM_DBG_TRACE, "VerifyCPE_CT\n");
	
	rec = 0;
	SW = L1sim_Cmd(cmd, 5 + 8, (kal_uint8 *)resp, &rec, &parity_err, hw_cb);

	sim_print(SIM_DBG_TRACE, "VerifyCPE_CT Actural Length: %d\r\n", rec);
	
	for(i=0; i<rec; i++)
	{
		res[i] = resp[i];
		sim_print(SIM_DBG_TRACE, "Data%d: 0x%02x\r\n", i, res[i]);
	}

	*(res+rec) = '\0';

	if (parity_err)
	{
		sim_print(SIM_DBG_DEBUG, "VerifyCPE Parity Error\r\n");
	}
	else
	{
		sim_print(SIM_DBG_TRACE, "VerifyCPE_CT Success\r\n");
	}

	sim_print(SIM_DBG_NONE, "VerifyCPE_CT SW=%x\r\n",SW);

	return SW;
}

kal_uint16 VerifyTelPass_CT(kal_uint8 *inputdata, kal_uint8 inputlen, kal_uint8 *res)
{
	kal_uint8 cmd[512] = {0};
	kal_uint16 i;
	kal_uint16 rec;
	kal_uint8 resp[50];
	kal_uint16 SW;
	kal_uint8 parity_err;
	sim_HW_cb *hw_cb;

	hw_cb = &simHWCbArray[0];

	cmd[0]= 0xa0;
	cmd[1]= 0xe0;
	cmd[2]= 0x0;
	cmd[3]= 0x0;
	cmd[4]= inputlen;

	/*Refill with random*/
	memcpy(&cmd[5], inputdata, inputlen);
	
	//SIM_Command(&sim_cmd);
	sim_print(SIM_DBG_NONE, "VerifyTelPass_CT\n");
	
	rec = 0;
	SW = L1sim_Cmd(cmd, 5 + inputlen, (kal_uint8 *)resp, &rec, &parity_err, hw_cb);

	sim_print(SIM_DBG_NONE, "VerifyTelPass_CT Actural Length: %d\r\n", rec);
	
	for(i=0; i<rec; i++)
	{
		res[i] = resp[i];
		sim_print(SIM_DBG_NONE, "Data%d: 0x%02x\r\n", i, res[i]);
	}

	*(res+rec) = '\0';

	if (parity_err)
	{
		sim_print(SIM_DBG_DEBUG, "VerifyTelPass Parity Error\r\n");
	}
	else
	{
		sim_print(SIM_DBG_DEBUG, "VerifyTelPass_CT Success\r\n");
	}

	sim_print(SIM_DBG_NONE, "VerifyTelPass_CT SW=%x\r\n",SW);

	return SW;
}

kal_uint16 ResetCard_CT(kal_uint8 *inputdata, kal_uint8 inputlen, kal_uint8 *res)
{
	kal_uint8 cmd[512] = {0};
	kal_uint16 i;
	kal_uint16 rec;
	kal_uint8 resp[50];
	kal_uint16 SW;
	kal_uint8 parity_err;
	sim_HW_cb *hw_cb;

	hw_cb = &simHWCbArray[0];

	cmd[0]= 0xa0;
	cmd[1]= 0xe4;
	cmd[2]= 0x0;
	cmd[3]= 0x0;
	cmd[4]= inputlen;

	/*Refill with random*/
	memcpy(&cmd[5], inputdata, inputlen);
	
	//SIM_Command(&sim_cmd);
	sim_print(SIM_DBG_TRACE, "ResetCard_CT\n");
	
	rec = 0;
	SW = L1sim_Cmd(cmd, 5 + inputlen, (kal_uint8 *)resp, &rec, &parity_err, hw_cb);

	sim_print(SIM_DBG_NONE, "ResetCard_CT Actural Length: %d\r\n", rec);
	
	for(i=0; i<rec; i++)
	{
		res[i] = resp[i];
		sim_print(SIM_DBG_NONE, "Data%d: 0x%02x\r\n", i, res[i]);
	}

	*(res+rec) = '\0';

	if (parity_err == KAL_TRUE)
	{
		sim_print(SIM_DBG_DEBUG, "ResetCard Parity Error\r\n");
	}
	else
	{
		sim_print(SIM_DBG_DEBUG, "ResetCard_CT Success\r\n");
	}

	sim_print(SIM_DBG_DEBUG, "ResetCard_CT SW=%x\r\n",SW);

	return SW;
}

kal_uint16 WriteBinary_CT(kal_uint8 *inputdata, kal_uint8 inputlen, kal_uint8 offsethigh, kal_uint8 offsetlow, kal_uint8 *res)
{
	kal_uint8 cmd[512] = {0};
	kal_uint16 i;
	kal_uint16 rec;
	kal_uint8 resp[50];
	kal_uint16 SW;
	kal_uint8 parity_err;
	sim_HW_cb *hw_cb;

	hw_cb = &simHWCbArray[0];

	cmd[0]= 0xa0;
	cmd[1]= 0xd6;
	cmd[2]= offsethigh;
	cmd[3]= offsetlow;
	cmd[4]= inputlen;

	/*Refill with random*/
	memcpy(&cmd[5], inputdata, inputlen);
	
	//SIM_Command(&sim_cmd);
	sim_print(SIM_DBG_TRACE, "WriteBinary_CT:offset:%02x,%02x,len:%02x!\n",offsethigh,offsetlow,inputlen);
	
	rec = 0;
	SW = L1sim_Cmd(cmd, 5 + inputlen, (kal_uint8 *)resp, &rec, &parity_err, hw_cb);

	sim_print(SIM_DBG_TRACE, "WriteBinary_CT Actural Length: %d\r\n", rec);
	
	for(i=0; i<rec; i++)
	{
		res[i] = resp[i];
		sim_print(SIM_DBG_TRACE, "Data%d: 0x%02x\r\n", i, res[i]);
	}

	*(res+rec) = '\0';

	if (parity_err == KAL_TRUE)
	{
		sim_print(SIM_DBG_DEBUG, "write Parity Error\r\n");
	}
	else
	{
		sim_print(SIM_DBG_TRACE, "WriteBinary_CT Success\r\n");
	}

	sim_print(SIM_DBG_TRACE, "WriteBinary_CT SW=%x\r\n",SW);

	return SW;
}

void ReadBinary(kal_uint8 len, kal_uint16 *res)
{
	kal_uint8 cmd[50];
	kal_uint16 i;
	kal_uint16 rec;
	kal_uint8 resp[300] = {0};
	kal_uint16 SW;
	kal_uint8 parity_err;
	sim_HW_cb *hw_cb;

	hw_cb = &simHWCbArray[0];

	cmd[0]= 0xa0;
	cmd[1]= 0xb0;
	cmd[2]= 0x0;
	cmd[3]= 0x0;
	cmd[4]= len;

	sim_print(SIM_DBG_DEBUG, "ReadBinary\n");
	sim_print(SIM_DBG_DEBUG, "Read Length: %d\n", len);

	//SIM_Command(&sim_cmd);
	rec = len;
	SW = L1sim_Cmd(cmd, 5, (kal_uint8 *)resp, &rec, &parity_err, hw_cb);
	for(i=0;i<rec;i++)
	{
		res[i] = (kal_uint16)resp[i];
	}
	*(res+rec) = (kal_uint8)(SW >> 8);
	*(res+rec+1) = (kal_uint8)SW;
	if (parity_err)
	{
		sim_print(SIM_DBG_ERR, "ReadBinary Parity Error\r\n");
	}

	sim_print(SIM_DBG_NONE, "ReadBinary SW=%x\r\n",SW);
	sim_print(SIM_DBG_NONE, "Read Binary cmd is ok\r\n");
	//sim_print(SIM_DBG_TRACE, "Read Binary data is [%s]\r\n", resp);
	//sim_print(SIM_DBG_TRACE, "the read data [14]=%x\r\n", res[13]);
}


kal_uint16 ReadBinary_CT(kal_uint8 offsethigh, kal_uint8 offsetlow, kal_uint8 len, kal_uint8 *res)
{
	kal_uint8 cmd[50];
	kal_uint16 i;
	kal_uint16 rec;
	kal_uint8 resp[300] = {0};
	kal_uint16 SW;
	kal_uint8 parity_err;
	sim_HW_cb *hw_cb;

	hw_cb = &simHWCbArray[0];

	cmd[0]= 0xa0;
	cmd[1]= 0xb0;
	cmd[2]= offsethigh;
	cmd[3]= offsetlow;
	cmd[4]= len;

	sim_print(SIM_DBG_TRACE, "ReadBinary CT\r\n");
	sim_print(SIM_DBG_TRACE, "Read Length: %d\r\n", len);

	//SIM_Command(&sim_cmd);
	rec = len;
	SW = L1sim_Cmd(cmd, 5, (kal_uint8 *)resp, &rec, &parity_err, hw_cb);
	sim_print(SIM_DBG_TRACE, "Read Actural Length: %d\r\n", rec);
	
	for(i=0; i<rec; i++)
	{
		res[i] = resp[i];
		sim_print(SIM_DBG_TRACE, "Data%d: 0x%02x ", i, res[i]);
	}
	sim_print(SIM_DBG_TRACE, "\r\n");
	
	*(res+rec) = '\0';
	//*(res+rec+1) = (kal_uint8)SW;
	if (parity_err)
	{
		sim_print(SIM_DBG_DEBUG, "ReadBinary Parity Error\r\n");
	}

	sim_print(SIM_DBG_TRACE, "ReadBinary SW=%x\r\n",SW);
	//sim_print(SIM_DBG_TRACE, "Read Binary cmd is ok\r\n");
	return SW;
}

kal_bool vcc_check(void)
{
	kal_uint8 cmd[50];
	kal_uint16 resp[300];
	kal_uint16 temp;
	sim_HW_cb *hw_cb;
	Sim_Card *SimCard;

	hw_cb = &simHWCbArray[0];

	SimCard = &SimCard_cb[0];
	sim_print(SIM_DBG_TRACE, "[SIM]: VCC Check Start\n");

	cmd[0]= 0x7f;
	cmd[1]= 0x20;
	SelectFile(cmd, resp);

	sim_print(SIM_DBG_TRACE, "rec length=%d\r\n", *(resp+1));

	GetResponse(*(resp+1),resp);
	temp = resp[13] & 0x000d;

	SimCard->clkStop = KAL_TRUE;

	if (temp & 0x0001)
	{
		if ((temp & 0x0004)!=0)
		{
			SimCard->clkStopLevel = KAL_TRUE;	//bit 3
		}

		if ((temp & 0x0008)!=0)		//bit 4
		{
			SimCard->clkStopLevel = KAL_FALSE;
		}

		if ((temp & 0x000c) == 0)	//default
		{
			SimCard->clkStopLevel = KAL_FALSE;
		}
	}
	else
	{
		if ((temp & 0x0004)!=0)
		{
			SimCard->clkStopLevel = KAL_TRUE;	//bit 3
		}

		if ((temp & 0x0008)!=0)		//bit 4
		{
			SimCard->clkStopLevel = KAL_FALSE;
		}

		if ((temp & 0x000c) == 0)	//default
		{
			SimCard->clkStopLevel = KAL_FALSE;
		}
	}

	temp = resp[13] & 0x000d;
	if (temp == 0)
	{
		SimCard->clkStop = KAL_FALSE;
		sim_print(SIM_DBG_TRACE, "fasfda&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\r\n");
	}

	temp = resp[13] & 0x0070;
	temp >>= 4;
	sim_print(SIM_DBG_TRACE, "the volt=%d\r\n", temp);

	#if defined(DRV_SIM_CLKSTOP_6250B_SERIES)
	if (SimCard->clkStop == KAL_TRUE)
	{
      //SIM_Reg(SIM_CONF) |= SIM_CONF_HALTEN;
      SIM_SetBits(SIM_CONF, SIM_CONF_HALTEN);
	}
	#endif   /*MT6205,MT6205B,MT6218*/

	return KAL_TRUE;
}

void CheckPinCMD(void)
{
	kal_uint8 cmd[50];
	kal_uint16 resp[300];

	sim_print(SIM_DBG_NONE, "CheckPinCMD \n");
/******************************** VERIFY CHV ******************************************/
#if !defined(TCSUPPORT_CPU_MT7520)&&!defined(TCSUPPORT_CPU_MT7505)
	sim_print(SIM_DBG_NONE, "\n\nverify chv\n");

	cmd[0]= 0x00;
	cmd[1]= 0x00;
	cmd[2]= 0x00;
	cmd[3]= 0x00;
	cmd[4]= 0x00;
	cmd[5]= 0x00;
	cmd[6]= 0x00;
	cmd[7]= 0x00;
	PinCheck(cmd, resp);

	sim_print(SIM_DBG_NONE, "\nStatus Cmd\n");
	cmd[0]= 0x7f;
	cmd[1]= 0x20;
	CmdStatus(cmd, resp);
#endif
/******************************** EF PHASE ****************************************/
	//1
	sim_print(SIM_DBG_NONE, "\n\n1.\n");
	cmd[0]= 0x7f;
	cmd[1]= 0x20;
	SelectFile(cmd, resp);

	//2
	sim_print(SIM_DBG_NONE, "\n\n2.\n");
	cmd[0]= 0x6f;
	cmd[1]= 0xae;
	SelectFile(cmd, resp);

	//3
	sim_print(SIM_DBG_NONE, "\n\n3.\n");
	GetResponse(*(resp+1), resp);	//GD

	//4
	sim_print(SIM_DBG_NONE, "\n\n4.\n");
	ReadBinary(1, resp);

/******************************** EF ICCID ****************************************/
	//5
	sim_print(SIM_DBG_NONE, "\n\n5.\n");
	cmd[0]= 0x3f;
	cmd[1]= 0x00;
	SelectFile(cmd, resp);

	//6
	sim_print(SIM_DBG_NONE, "\n\n6.\n");
	cmd[0]= 0x2f;
	cmd[1]= 0xe2;
	SelectFile(cmd, resp);

	//7
	sim_print(SIM_DBG_NONE, "\n\n7.\n");
	ReadBinary(10, resp);

	
/******************************** EF LP ****************************************/
	//8
	sim_print(SIM_DBG_NONE, "\n\n8.\n");
	cmd[0]= 0x7f;
	cmd[1]= 0x20;
	SelectFile(cmd, resp);

	//9
	sim_print(SIM_DBG_NONE, "\n\n9.\n");
	cmd[0]= 0x6f;
	cmd[1]= 0x05;
	SelectFile(cmd, resp);

	//10
	sim_print(SIM_DBG_NONE, "\n\n10.\n");
	GetResponse(*(resp+1), resp);

	//11
	sim_print(SIM_DBG_NONE, "\n\n11.\n");
	ReadBinary(1, resp);

#ifdef TCSUPPORT_AUTOBENCH
return 0;
#endif
/******************************** EF IMSI ******************************************/
	//12
	sim_print(SIM_DBG_NONE, "\n\n12.\n");
	cmd[0]= 0x7f;
	cmd[1]= 0x20;
	SelectFile(cmd, resp);

	//13
	sim_print(SIM_DBG_NONE, "\n\n13.\n");
	cmd[0]= 0x6f;
	cmd[1]= 0x07;
	SelectFile(cmd, resp);

	//14
	sim_print(SIM_DBG_NONE, "\n\n14.\n");
	GetResponse(*(resp+1), resp);


/******************************** EF LOCI ******************************************/
	//15
	sim_print(SIM_DBG_NONE, "\n\n15.\n");
	cmd[0]= 0x7f;
	cmd[1]= 0x20;
	SelectFile(cmd, resp);

	//16
	sim_print(SIM_DBG_NONE, "\n\n16.\n");
	cmd[0]= 0x6f;
	cmd[1]= 0x7e;
	SelectFile(cmd, resp);

	//17
	sim_print(SIM_DBG_NONE, "\n\n17.\n");
	GetResponse(*(resp+1), resp);

	//18
	sim_print(SIM_DBG_NONE, "\n\n18.\n");
	ReadBinary(11, resp);

	sim_print(SIM_DBG_NONE, "Check PIN END..\r\n");
}

void closeSIMcmd(void)
{
	kal_uint8 cmd[50];
	kal_uint16 resp[300];

	cmd[0]= 0x6f;
	cmd[1]= 0x37;
	SelectFile(cmd, resp);

	GetResponse(*(resp+1), resp);

	cmd[0]= 0x6f;
	cmd[1]= 0x41;
	SelectFile(cmd, resp);

	GetResponse(*(resp+1), resp);
}

#endif

int Sim_Reset(void)
{
	kal_uint8 result;
	sim_HW_cb *hw_cb;
	kal_uint8 cmd[50];
	kal_uint16 resp[300];
	hw_cb = &simHWCbArray[0];
#if defined(TCSUPPORT_AUTOBENCH)&&defined(TCSUPPORT_CPU_MT7505)	
	int cnt = 50000;
#endif
	usim_power_enum power;

	sim_print(SIM_DBG_TRACE, "Sim_Reset\n");
	sim_print(SIM_DBG_TRACE, "===================================================\n");

	result = L1sim_Reset_All(CLASS_B_30V, &power, KAL_FALSE, SIM_ICC_APPLICATION_PHONE1);
#ifdef TCSUPPORT_AUTOBENCH
	if (result != SIM_NO_ERROR)
		sim_autobench_result ++;
#endif

	if (result == SIM_NO_ERROR)
	{
		sim_print(SIM_DBG_TRACE, "SIM has no Error!\r\n");
		return 0;
	}

	if (result == SIM_CARD_ERROR)
	{
		sim_print(SIM_DBG_TRACE, "SIM CARD has something error!\r\n");
		return -1;
	}

	if (result == SIM_NO_INSERT)
	{
		sim_print(SIM_DBG_TRACE, "SIM CARD no insert!\r\n");
		return -2;
	}
	
	sim_print(SIM_DBG_TRACE, "SIM Reset OK! the resVolt=%x\r\n",resVolt);

}


int  Sim_test(void)
{
	kal_uint8 result;
	int pre_dbg_level;
	sim_HW_cb *hw_cb;
	kal_uint8 cmd[50];
	kal_uint16 resp[300];
	hw_cb = &simHWCbArray[0];
#if defined(TCSUPPORT_AUTOBENCH)&&defined(TCSUPPORT_CPU_MT7505)	
	int cnt = 50000;
#endif
	usim_power_enum power;

	
	sim_print(SIM_DBG_NONE, "Sim_Test\n");
	sim_print(SIM_DBG_NONE, "===================================================\n");

	result = L1sim_Reset_All(CLASS_B_30V, &power, KAL_FALSE, SIM_ICC_APPLICATION_PHONE1);
#ifdef TCSUPPORT_AUTOBENCH
	if (result != SIM_NO_ERROR)
		sim_autobench_result ++;
#endif

	if (result == SIM_NO_ERROR)
	{
		sim_print(SIM_DBG_NONE, "SIM has no Error!\r\n");
	}

	if (result == SIM_CARD_ERROR)
	{
		sim_print(SIM_DBG_NONE, "SIM CARD has something error!\r\n");
		return -1;
	}

	if (result == SIM_NO_INSERT)
	{
		sim_print(SIM_DBG_NONE, "SIM CARD no insert!\r\n");
		return -2;
	}
	
	sim_print(SIM_DBG_NONE, "the resVolt=%x\r\n",resVolt);

#if 0 //defined(TCSUPPORT_AUTOBENCH)&&defined(TCSUPPORT_CPU_MT7505)	
	
		while(cnt){
	CheckPinCMD();
			sim_print(SIM_DBG_NONE,"test round %d, sim_autobench_result=%d\r\n",cnt,sim_autobench_result);
			cnt--;
}
#else
		CheckPinCMD();
	
#endif
	sim_print(SIM_DBG_NONE, "=========================================\r\n");
}
#if defined(TCSUPPORT_CPU_MT7520)||defined(TCSUPPORT_CPU_MT7505)
/*************************
 * GPIO Module Registers *
 *************************/
#define CR_GPIO_BASE       	0xBFBF0200
#define CR_GPIO_CTRL	    (CR_GPIO_BASE + 0x00)
#define CR_GPIO_DATA	    (CR_GPIO_BASE + 0x04)
#define CR_GPIO_INTS      	(CR_GPIO_BASE + 0x08)
#define CR_GPIO_EDET	    (CR_GPIO_BASE + 0x0C)
#define CR_GPIO_LDET       	(CR_GPIO_BASE + 0x10)
#define CR_GPIO_ODRAIN      (CR_GPIO_BASE + 0x14)
#define CR_GPIO_CTRL1	    (CR_GPIO_BASE + 0x20)
#ifdef TCSUPPORT_GPIO_ECM
#define CR_SGPIO_DATA        (CR_GPIO_BASE + 0x24)
#define CR_SGPIO_CDIV        (CR_GPIO_BASE + 0x28)
#define CR_SGPIO_CDLY        (CR_GPIO_BASE + 0x2C)
#define CR_SGPIO_MODE	    (CR_GPIO_BASE + 0x30)
#define CR_GPIO_FLAMOD	    (CR_GPIO_BASE + 0x34)
#define CR_GPIO_IMME	    (CR_GPIO_BASE + 0x38)
#define CR_GPIO_FLAP0	    (CR_GPIO_BASE + 0x3C)
#define CR_GPIO_FLAP1	    (CR_GPIO_BASE + 0x40)
#define CR_GPIO_FLAP2 	    (CR_GPIO_BASE + 0x44)
#define CR_GPIO_FLAP3	    (CR_GPIO_BASE + 0x48)
#define CR_GPIO_FMAP0	    (CR_GPIO_BASE + 0x4C)
#define CR_GPIO_FMAP1 	    (CR_GPIO_BASE + 0x50)
#define CR_SGPIO_FMAP0	    (CR_GPIO_BASE + 0x54)
#define CR_SGPIO_FMAP1	    (CR_GPIO_BASE + 0x58)
#define CR_SGPIO_FMAP2	    (CR_GPIO_BASE + 0x5C)
#define CR_GPIO_TYPE	    0xBFB000B0
#endif
/* MT7510 */
#define CR_GPIO_CTRL2	    (CR_GPIO_BASE + 0x60)
#define CR_GPIO_CTRL3	    (CR_GPIO_BASE + 0x64)
#define CR_GPIO_FLAMOD_EXT	(CR_GPIO_BASE + 0x68)
#define CR_GPIO_DATA1		(CR_GPIO_BASE + 0x70)
#define CR_GPIO_ODRAIN1     (CR_GPIO_BASE + 0x78)

#define GPIO_HIGH(x)		do {	\
							unsigned int  value1 = SIM_Reg(CR_GPIO_DATA);	\
							unsigned int  value2 = SIM_Reg(CR_GPIO_DATA1);	\
							if(x > 31){							\
								if(x > 47){						\
									SIM_WriteReg32(CR_GPIO_CTRL3,SIM_Reg(CR_GPIO_CTRL3)|(1<<((x-48)*2)));	\
								}else{							\
									SIM_WriteReg32(CR_GPIO_CTRL2,SIM_Reg(CR_GPIO_CTRL2)|(1<<((x-32)*2)));	\
								}								\
								SIM_WriteReg32(CR_GPIO_ODRAIN1,SIM_Reg(CR_GPIO_ODRAIN1)|(1<<(x-32)));	\
								value2 |= (1<<(x-32));			\
								SIM_WriteReg32(CR_GPIO_DATA1,value2);\
							}									\
							else{								\
								if(x > 15){						\
									SIM_WriteReg32(CR_GPIO_CTRL1,SIM_Reg(CR_GPIO_CTRL1)|(1<<((x-16)*2)));	\
								}else{							\
									SIM_WriteReg32(CR_GPIO_CTRL,SIM_Reg(CR_GPIO_CTRL)|(1<<(x*2)));	\
								}								\
								SIM_WriteReg32(CR_GPIO_ODRAIN,SIM_Reg(CR_GPIO_ODRAIN)|(1<<(x)));	\
								value1 |= (1<<x);			\
								SIM_WriteReg32(CR_GPIO_DATA,value1);\
							}									\
				} while(0)

#define GPIO_LOW(x)		do {	\
							unsigned int  value1 = SIM_Reg(CR_GPIO_DATA);	\
							unsigned int  value2 = SIM_Reg(CR_GPIO_DATA1);	\
							if(x > 31){							\
								if(x > 47){						\
									SIM_WriteReg32(CR_GPIO_CTRL3,SIM_Reg(CR_GPIO_CTRL3)|(1<<((x-48)*2)));	\
								}else{							\
									SIM_WriteReg32(CR_GPIO_CTRL2,SIM_Reg(CR_GPIO_CTRL2)|(1<<((x-32)*2)));	\
								}								\
								SIM_WriteReg32(CR_GPIO_ODRAIN1,SIM_Reg(CR_GPIO_ODRAIN1)|(1<<(x-32)));	\
								value2 &= ~ (1<<(x-32));	\
								SIM_WriteReg32(CR_GPIO_DATA1,value2);\
							}									\
							else{								\
								if(x > 15){						\
									SIM_WriteReg32(CR_GPIO_CTRL1,SIM_Reg(CR_GPIO_CTRL1)|(1<<((x-16)*2)));	\
								}else{							\
									SIM_WriteReg32(CR_GPIO_CTRL,SIM_Reg(CR_GPIO_CTRL)|(1<<(x*2)));	\
								}								\
								SIM_WriteReg32(CR_GPIO_ODRAIN,SIM_Reg(CR_GPIO_ODRAIN)|(1<<(x)));	\
								value1 &= ~(1<<x);			\
								SIM_WriteReg32(CR_GPIO_DATA,value1);\
							}									\
				} while(0)
#ifdef TCSUPPORT_CPU_MT7505
#define MT6306_RST_GPIO (12)
#else
#define MT6306_RST_GPIO (18)
#endif

#endif				


static int tsim_cmd_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	printk("Now sim g_loop_count is [%d]  g_loop_interval is [%d]\r\n", g_loop_count, g_loop_interval);
	return 0;
}

static int tsim_cmd_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[512];
	char optcmd[32] = {0};
	char opt1[256] = {0};
	char opt2[256] = {0};
	char opt3[256] = {0};
	kal_uint8 cmd[50] = {0};
	kal_uint16 resp[300] = {0};

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';

	sscanf(val_string, "%s %s %s %s", optcmd, opt1, opt2, opt3) ;

	if(!strcmp(optcmd, "cardreset")){

		Sim_Reset();
	}
	if(!strcmp(optcmd, "simtest")){

		Sim_test();
	}	
	else if(!strcmp(optcmd, "dbglevel")){
		int dbglevel = simple_strtoul(opt1, NULL, 10);
		sim_dbg_level = dbglevel;
		printk("Now sim dbg level is [%d]\r\n", sim_dbg_level);
	}
	else if(!strcmp(optcmd, "loopset")){
		g_loop_count = simple_strtoul(opt1, NULL, 10);
		g_loop_interval = simple_strtoul(opt2, NULL, 10);
		printk("Now sim g_loop_count is [%d]  g_loop_interval is [%d]\r\n", g_loop_count, g_loop_interval);
	}	
	else if(!strcmp(optcmd, "select")){
		unsigned long result;
		result = simple_strtoul(opt1, NULL, 16);
		printk("select cmd opt is [0x%04x] [0x%x]\r\n", result, result);
		cmd[0]= (result >> 8) & 0xFF;
		cmd[1]= result & 0xFF;
		printk("SelectFile cmd0 is [0x%02x] cmd1 is [0x%02x]\r\n", cmd[0], cmd[1]);
		
		SelectFile(cmd, resp);
	}
	else if(!strcmp(optcmd, "getresponse")){
		unsigned long result;
		result = simple_strtoul(opt1, NULL, 16);
		printk("getresponse cmd opt len is [%d]\r\n", result);	
		GetResponse_CT(result, resp);
	}	
	else if(!strcmp(optcmd, "readbinary")){
		unsigned long offhigh, offlow, len;
		offhigh = simple_strtoul(opt1, NULL, 16);
		offlow = simple_strtoul(opt2, NULL, 16);
		len = simple_strtoul(opt3, NULL, 16);

		cmd[0]= offhigh & 0xFF;
		cmd[1]= offlow & 0xFF;
		cmd[2]= len & 0xFF;

		printk("ReadFile cmd0 is [0x%02x] cmd1 is [0x%02x] cmd2 is [0x%02x]\r\n", cmd[0], cmd[1], cmd[2]);
		printk("ReadFile cmd0 is [%u] cmd1 is [%u] cmd2 is [%u]\r\n", cmd[0], cmd[1], cmd[2]);

		ReadBinary_CT(cmd[0], cmd[1], cmd[2], resp);
		printk("Read Binary CT data is [%s]\r\n", resp);
		
	}	
	else if(!strcmp(optcmd, "getupdate")){
		GetFileUpdate(resp);
	}
	else if(!strcmp(optcmd, "resetdata")){

		//Need get real opt
		
		kal_uint8 data[27] = {0};
		data[0] = 0x11;
		ResetCard_CT(data, 27, resp);
		//GetFileUpdate(resp);
	}
	else if(!strcmp(optcmd, "writebinary")){
		unsigned long offhigh, offlow, len;
		offhigh = simple_strtoul(opt1, NULL, 16);
		offlow = simple_strtoul(opt2, NULL, 16);
		len = strlen(opt3);
		printk("writebinary offset high is [0x%02x] offset low is [0x%02x] len is [0x%02x]\r\n", offhigh, offlow, len);		
		printk("writebinary offset high is [%u] offset low is [%u] len is [%u]\r\n", offhigh, offlow, len);		
		printk("writebinary data is [%s] \r\n", opt3);		
		WriteBinary_CT(opt3, len, offhigh, offlow, resp);
		//GetFileUpdate(resp);
	}	
	else if(!strcmp(optcmd, "verfycard")){
		printk("verfycard data is [%s] len is [%d]\r\n", opt1, strlen(opt1));

		//Must change String to Hex mode
		//Ex: char opt1[] = "E0A1D2CC00011..." -> uint8 opt1[16] = {0xE0, ...}
		
		VerifyCard_CT(opt1, resp);
		//GetFileUpdate(resp);
	}
	else if(!strcmp(optcmd, "verfytelpass")){
		printk("verfytelpass data is [%s] len is [%d]\r\n", opt1, strlen(opt1));	
		
		VerifyTelPass_CT(opt1, strlen(opt1), resp);
		//GetFileUpdate(resp);
	}	
	else if(!strcmp(optcmd, "verfycpe")){
		printk("verfycpe data is [%s] len is [%d]\r\n", opt1, strlen(opt1));		

		//Must change String to Hex mode
		//Ex: char opt1[] = "E0A1D2CC00011..." -> uint8 opt1[8] = {0xE0, ...}
		
		VerifyCPE_CT(opt1, resp);
		//GetFileUpdate(resp);
	}
	else
	{
		printk("Invalid opt command: simtest, cardreset, dbglevel [1|2|3|4], loopset [loopcount, loopinterval], select [0xfe01], getresponse [len], readbinary [offhigh, offlow, len], getupdate, restore, \
				resetdata, verfycard [data16hex], verfytelpass [data], verfycpe [data8hex], writebinary [oofhigh, offlow, data] \r\n");
	}
	return count;
}

/* top register*/
#define TOP_CSR_GPIO_SHARE								0xbfb00860

#define IO_GREG(reg)						ioread32((void __iomem *)(reg)) /* GetReg((uint)reg) */
#define IO_SREG(reg, val)					iowrite32(val, (void __iomem *)(reg)) /* SetReg((uint)reg, val) */

#define GPIO_PHY_MODE							(1<<25)

void test(){
	uint read_data = 0;
	/* switch GPIO to XPON mode*/
	read_data = IO_GREG(TOP_CSR_GPIO_SHARE);
	read_data = read_data | (GPIO_PHY_MODE);
	IO_SREG(TOP_CSR_GPIO_SHARE, read_data);

}
static int __init mt7510_sim_init(void)
{
	int status;
	kal_uint8 cmd[50];
	kal_uint8 resp[100];

	kal_uint16 rec;
	kal_uint8 parity_err;
	kal_uint16 SW;
	sim_HW_cb *hw_cb;
	unsigned int val;
	usim_power_enum power;
	
	struct proc_dir_entry *tsim_proc;
	
#if defined(TCSUPPORT_CPU_MT7520)||defined(TCSUPPORT_CPU_MT7505)
	test();
#endif
	
if( isEN751221 && (isFPGA==0) ){
		SIM_WriteReg32(0xbfb00834,(SIM_Reg(0xbfb00834) | 0x40000000));
		mdelay(10);
		SIM_WriteReg32(0xbfb00834,(SIM_Reg(0xbfb00834) & (~0x40000000)));
		mdelay(10);
	}else{	
#if defined(TCSUPPORT_CPU_MT7520)||defined(TCSUPPORT_CPU_MT7505)

#ifdef TCSUPPORT_CPU_MT7505
	SIM_WriteReg32(0xbfa2007c,(SIM_Reg(0xbfa2007c) | 0x0009c000));
#else
	SIM_WriteReg32(0xbfb00860,(SIM_Reg(0xbfb00860) | 0x10000000));
#endif

	SIM_WriteReg32(0xbfb00834,(SIM_Reg(0xbfb00834) | 0x40000000));
	mdelay(10);
	SIM_WriteReg32(0xbfb00834,(SIM_Reg(0xbfb00834) & (~0x40000000)));
	
	GPIO_LOW(MT6306_RST_GPIO);
	mdelay(10);
	GPIO_HIGH(MT6306_RST_GPIO);
	mdelay(10);
#endif	

	}

	sim_print(SIM_DBG_TRACE, "mt7510 sim controller init\n");

	sim_init_all_cb();

	//L1sim_Reset_All(CLASS_B_30V, &power, KAL_FALSE, SIM_ICC_APPLICATION_PHONE1);//  CLASS_B_30V

	//Sim_test();


	/* atm related stats */
	tsim_proc = create_proc_entry("tc3162/simcmd", 0, NULL);
	if (tsim_proc){
		tsim_proc->read_proc = tsim_cmd_read_proc;
		tsim_proc->write_proc = tsim_cmd_write_proc;
	} else {
		printk("proc_entry tc3162/simcmd alloc fail\n");
	}

	simdcard_init();
	
#ifdef TCSUPPORT_AUTOBENCH
	printk ("sim test result=%d\n", sim_autobench_result );
#endif
	

	return 0;
}

static void __exit mt7510_sim_exit(void)
{
	sim_print(SIM_DBG_TRACE, "mt7510 sim controller exit\n");
	sim_HW_cb *hw_cb;
	L1sim_PowerOff_MT6306(hw_cb);
	remove_proc_entry("tc3162/simcmd", NULL);
	simdcard_exit();
	return 0;
}

module_init (mt7510_sim_init);
module_exit (mt7510_sim_exit);
