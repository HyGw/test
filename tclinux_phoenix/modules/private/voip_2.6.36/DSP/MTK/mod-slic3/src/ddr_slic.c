#ifdef __KERNEL__

#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/proc_fs.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <asm/tc3162/tc3162.h>
#include <asm/tc3162/ledcetrl.h>
#include "_TDI.h"
#include "ddr_slic3.h"
#include <linux/libcompileoption.h>
#include <ecnt_hook/ecnt_hook.h>
#include <ecnt_hook/ecnt_hook_voip.h>
#define MSG						printk
#define MALLOC(size)				kmalloc(size, GFP_ATOMIC)
#define FREE(ptr)					kfree(ptr)

//#include "modid.h"
//#include "my_cisr.h"
#if !defined(TCSUPPORT_SLIC_SEPARATE) && !defined(SLIC_SILICON)
#include <vp_api.h>
#include <VE890_ACA_Rev2_5profiles8.h>
#include <vp_api_cfg.h>
#include "lt_api.h" //Roger_lt
#endif
#ifdef SLIC_SILICON  //dyma_131106
#include "proslic.h"
#include "proslic_mlt.h" //Roger_silicon_lt
#endif
#define MLOG0(FMT) \
		while(0){}
#define MLOG1( FMT, ARG1) \
		while(0){}
#define MLOG2( FMT, ARG1, ARG2) \
		while(0){}
#define MLOG3( FMT, ARG1, ARG2, ARG3) \
		while(0){}

#define SLIC_DUMP_WAIT_TIMES  1000   //dyma_130306
#else
#ERROR "define___________error___________"
#include <bsp/arm.h> 
#include <bsp/cisr.h>
#include <bsp/config.h>
#include <stdio.h>
#define MSG						printf
#define MALLOC(size)				malloc(size)
#define FREE(ptr)					free(ptr)
#define MLOG0( FMT) \
	while(0){}
#define MLOG1( FMT, ARG1) \
	while(0){}
#define MLOG2( FMT, ARG1, ARG2) \
	while(0){}
#define MLOG3( FMT, ARG1, ARG2, ARG3) \
	while(0){}
#endif
#include "slic_func_tbl.h"
#include "pcmdriver.h"

/*****************************/
/*		MACRO DEFINIFION	 */
/*****************************/
//dyma_131106#define MAX_SLIC_CHAN 4
//dyma_131106#define CHAN_PER_DEVICE 2

enum {
SLIC_OFFHOOK,
SLIC_ONHOOK
};
enum{
	LINE_TEST_UNINIT=-2,
	LINE_TEST_DOING,
	LINE_TEST_UNCONNECT,
	LINE_TEST_CONNECT
};
/*****************************/
/*	STRUCTURE DEFINITION	 */
/*****************************/

/****************************/
/*		GLOBAL VARIABLE		*/
/****************************/
#ifdef TCSUPPORT_SLIC_SEPARATE
slicFuncTbl si32260Func[]={0};
slicFuncTbl si32260Func_1[]={0};
slicFuncTbl si32176Func[]={0};
slicFuncTbl si3218xFunc[]={0};
slicFuncTbl si3228xFunc[]={0};
slicFuncTbl le89116Func[] = {0};
slicFuncTbl le89156Func[] = {0};
slicFuncTbl le89116_1Func[] = {0};
slicFuncTbl le89156_2Func[] = {0};
slicFuncTbl le89316Func[] = {0};
slicFuncTbl zl88601Func[] = {0};
slicFuncTbl zl88601_1Func[] = {0};
slicFuncTbl le9662Func[] = {0}; 
slicFuncTbl le9662_1Func[] = {0};
slicFuncTbl le9641Func[] = {0}; //dyma_150112
slicFuncTbl le9641_1Func[] = {0};

int fxsNum=0, fxoNum=0, pcm1ChNum=0; pcm2ChNum=0; 
EXPORT_SYMBOL(fxsNum);
EXPORT_SYMBOL(fxoNum);
#else
#ifdef SLIC_SILICON
extern slicFuncTbl si32260Func[];
extern slicFuncTbl si32176Func[];
extern slicFuncTbl si3218xFunc[];
extern slicFuncTbl si3228xFunc[];

#else
extern slicFuncTbl le89116Func[];
extern slicFuncTbl le89156Func[];
extern slicFuncTbl le89116_1Func[];
extern slicFuncTbl le89156_2Func[];
extern slicFuncTbl le89316Func[];
extern slicFuncTbl zl88601Func[]; 
extern slicFuncTbl zl88601_1Func[]; 
extern slicFuncTbl le9662Func[];
extern slicFuncTbl le9662_1Func[];
extern slicFuncTbl le9641Func[]; //dyma_150112
extern slicFuncTbl le9641_1Func[]; 
#endif
#endif

int devNum=1;/* Add by mtk06404_20160318 devNum modify for normal */
EXPORT_SYMBOL(devNum);/* Add by mtk06404_20160318 devNum modify for normal */

SLIC_FUNC_TBL slicFuncTab;
EXPORT_SYMBOL(slicFuncTab);

#ifdef SLIC_SILICON
int fxs_intr_cnt;  //dyma_131227
fxsSetLineStateQueue_t slicSetQueue; //dyma_131230
SLIC_TIMER_STATUS slicTimerStatus;  //dyma_131230 //silabs SLIC linefeed register can't be set continusly, at least 100ms buffer must be inserted
#endif

slicFuncTbl* slicFunc[DSP_MAX_CHAN] = {0};/*upgrade_to_4_channel*/

#ifdef FAXLAB_DET
int downSlicGainFlag=0; //flag 0: not faxlab auto reduce slic gain ; !0: faxlab detect set slic gain
_TDI_gainSetting_t lastGainSetting[DSP_MAX_CHAN]={0};/*upgrade_to_4_channel*/
EXPORT_SYMBOL(downSlicGainFlag);
EXPORT_SYMBOL(lastGainSetting);
#endif
static unsigned char g_slicInitStat[DSP_MAX_CHAN];/*upgrade_to_4_channel*/
static _TDI_initSetting_t g_initSetting[DSP_MAX_CHAN];/*upgrade_to_4_channel*/
unsigned int _slic_dbg_type_mask=0x0;   //dyma_130316 //dyma_131106
static atomic_t slicDumpFlag;  //dyma_130306
static unsigned char sliclinetestFlag[DSP_MAX_CHAN]={0};/*upgrade_to_4_channel*/
static  char lineTestProgress[DSP_MAX_CHAN] = {LINE_TEST_UNINIT,LINE_TEST_UNINIT,LINE_TEST_UNINIT,LINE_TEST_UNINIT};/*upgrade_to_4_channel*/
static  int  lineTestId;
static  int32 lineTestData;

static  int32 lineTestProcRes[8] ={0};	/* 20160920_for proc to read result */


#ifdef OSBN_VOIP
deviceInfo_t g_deviceInfo;
#else
static int slic_ch_num;
#endif
#ifdef __KERNEL__
spinlock_t slic_irq_lock;
#endif
#ifdef SLIC_SILICON  //dyma_131106   //dyma_131212
gainTbl_t g_slicGainTbl[] = {
	{-24, -24},
	{-23, -23},
	{-22, -22},
	{-21, -21},
	{-20, -20},
	{-19, -19},
	{-18, -18},
	{-17, -17},
	{-16, -16},
	{-15, -15},
	{-14, -14},
	{-13, -13},
	{-12, -12},
	{-11, -11},
	{-10, -10},
	{-9 , -9 },
	{-8 , -8 },
	{-7 , -7 },
	{-6 , -6 },
	{-5 , -5 },
	{-4 , -4 },
	{-3 , -3 },
	{-2 , -2 },
	{-1 , -1 },
	{0	, 0  },
	{1	, 1  },
	{2	, 2  },
	{3	, 3  },
	{4	, 4  },
	{5	, 5  },
	{6	, 6  },
	{7	, 7  },
	{8	, 8  },
	{9	, 9  },
	{10 , 10 },
	{11 , 11 },
	{12 , 12 },
	{13 , 13 },
	{14 , 14 },
	{15 , 15 },
	{16 , 16 },
	{17 , 17 },
	{18 , 18 },
	{19 , 19 },
	{20 , 20 },
	{21 , 21 },
	{22 , 22 },
	{23 , 23 },
	{24 , 24 } 
};
#else   //Zarlink gain table
gainTbl_t g_slicGainTbl[] = {
		{-24,0x1013},
		{-23,0x1107},
		{-22,0x1209},
		{-21,0x131B},
		{-20,0x143D},
		{-19,0x1570},
		{-18,0x16B5},
		{-17,0x180D},
		{-16,0x197A},
		{-15,0x1AFD},
		{-14,0x1C96},
		{-13,0x1E48},
		{-12,0x2013},
		{-11,0x21FA},
		{-10,0x23FD},
		{-9,0x261F},
		{-8,0x2861},
		{-7,0x2AC6},
		{-6,0x2D4E},
		{-5,0x2FFE},
		{-4,0x32D6},
		{-3,0x35D9},
		{-2,0x390A},
		{-1,0x3C6B},
		{0,0x4000},
		{1,0x43CA},
		{2,0x47CF},
		{3,0x4C10},
		{4,0x5092},
		{5,0x5558},
		{6,0x5A67},
		{7,0x5FC2},
		{8,0x656E},
		{9,0x6B71},
		{10,0x71CF},
		{11,0x788D},
		{12,0x7FB2},
		{13,0x8743},
		{14,0x8F47},
		{15,0x97C4},
		{16,0xA0C2},
		{17,0xAA49},
		{18,0xB460},
		{19,0xBF10},
		{20,0xCA62},
		{21,0xD660},
		{22,0xE314},
		{23,0xF089},
		{24,0xFEC9}
};

#endif
EXPORT_SYMBOL(g_slicGainTbl);
int g_slicGainTbl_size=0;
EXPORT_SYMBOL(g_slicGainTbl_size);

/********************************/
/*		FUNCTION DECLARTION		*/
/*******************************/
int SLIC_SPI_Reset(void)
{
	SPI_Reset(spi_interface_type);
}

int SLIC_Reset(void){
    /* Reset the Codec SLIC */

	SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "slic reset num:%d \n",ledGetGpio(LED_VOIP_SLIC1_RESET));
	if (ledGetGpio(LED_VOIP_SLIC1_RESET)!=0)	
		led_oen(ledGetGpio(LED_VOIP_SLIC1_RESET));
	ledTurnOff(LED_VOIP_SLIC1_RESET);
	mdelay(10);
	ledTurnOn(LED_VOIP_SLIC1_RESET);
	mdelay(50);
	ledTurnOff(LED_VOIP_SLIC1_RESET);
	mdelay(10);

	SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "slic reset num:%d \n",ledGetGpio(LED_VOIP_SLIC2_RESET));
	if (ledGetGpio(LED_VOIP_SLIC2_RESET)!=0)
		led_oen(ledGetGpio(LED_VOIP_SLIC2_RESET));
	ledTurnOff(LED_VOIP_SLIC2_RESET);
	mdelay(10);
	ledTurnOn(LED_VOIP_SLIC2_RESET);
	mdelay(50);
	ledTurnOff(LED_VOIP_SLIC2_RESET);
	mdelay(10);
	return 0;
}
#ifdef TCSUPPORT_SLIC_SEPARATE
#ifdef OSBN_VOIP
int SLIC_SEP_getDeviceInfo(int selection)
{
	int retVal = 0;
	
	switch(selection){
		case DEVICE_NUM:
			retVal = devNum;
			break;
		case FXS_NUM:
			retVal = fxsNum;
			break;
		case FXO_NUM:
			retVal = fxoNum;
			break;
		case TOTALCH_NUM:
			retVal = fxsNum + fxoNum;
			break;
		case PCM1_CH_NUM:
			if(pcm2ChNum==0)
				retVal = fxsNum + fxoNum;
			else
				retVal = pcm1ChNum;
			break;
		case PCM2_CH_NUM:
			retVal = pcm2ChNum;
			break;
		default:
			break;
	}
	return retVal;
}
#else
int SLIC_SEP_TotalChannelGet(void)
{	
	return fxsNum + fxoNum;
}
#endif

#else //else of TCSUPPORT_SLIC_SEPARATE
static int Time_delay(void *hTimer, int timeInMs){
    mdelay(timeInMs);
	return 0;
}

static int TimeElapsed_f(void *hTimer, void *startTime, int *timeInMs){
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG, "%s is called\n",__FUNCTION__);
	return 0;
}

static int GetTime_f(void *hTimer, void *time){
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG, " %s is called\n",__FUNCTION__);
	return 0;
}

//dyma_130306
static int slicDumpChkLock(atomic_t *slicDumpFlag){
	uint16 apiWaitCnt=0;
	while(atomic_read(slicDumpFlag)){
		apiWaitCnt++;
		mdelay(10);
		if(apiWaitCnt>SLIC_DUMP_WAIT_TIMES){
			atomic_set(slicDumpFlag,0);
			SLIC_DBG_MSG(_SLIC_DBG_TYPE_WARNING, "\n\rslicDumpTryLock timeout!\n");
			break;
		}
	}
	return 0;
}
static int slicDumpLock(atomic_t *slicDumpFlag){
	uint16 apiWaitCnt=0;
	while(atomic_read(slicDumpFlag)){
		apiWaitCnt++;
		mdelay(10);
		if(apiWaitCnt>SLIC_DUMP_WAIT_TIMES){
			atomic_set(slicDumpFlag,0);
			SLIC_DBG_MSG(_SLIC_DBG_TYPE_WARNING, "\n\rslicDumpLock timeout!\n");
			break;
		}
	}
	atomic_set(slicDumpFlag,1);
	return 0;
}
static int slicDumpUnLock(atomic_t *slicDumpFlag){
	atomic_set(slicDumpFlag,0);
	return 0;
}

#ifdef SLIC_SILICON  //dyma_131230
void slicTimer_Start(void)	
{
	if(slicTimerStatus==SLIC_TIMER_CMP){
		/* only when timer is free, can we start another timer counting process */
		/* so after each slicTimer_Check CMP, please start another timer counting process */
		slicTimerStatus=SLIC_TIMER_START;
	}
}

char slicTimer_Check(void)	
{
	static int start_time;
	//int delay_times=g_deviceInfo.deviceNum*10;
	int delay_times=SI_SET_QUEUE_SIZE;
	
	if(slicTimerStatus==SLIC_TIMER_START){
		slicTimerStatus=SLIC_TIMER_COUNTING;
		/* record start time and start timer counting */
		start_time=0;
	}
	else if(slicTimerStatus==SLIC_TIMER_COUNTING){
		/* check time gap between now and  start time */
		start_time++;
		if(start_time>delay_times){
			slicTimerStatus=SLIC_TIMER_CMP;
		}
	}
	else if(slicTimerStatus==SLIC_TIMER_CMP){
		/* do nothing */
	}
	return (char)slicTimerStatus;
}

void fxsSetLineStateQueue_Clear(void)	
{
	memset(&slicSetQueue, 0, sizeof(fxsSetLineStateQueue_t));
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG,"fxsSetLineStateQueue_Clear\r\n");
}

void fxsSetLineStateQueue_Insert(_TDI_lineInfo_t* plineInfo)
{
	char i;	
	static short max_cnt=0;
	
	if ((slicSetQueue.w_idx == slicSetQueue.r_idx) && (slicSetQueue.count >= SI_SET_QUEUE_SIZE))
	{
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG,"setQueue is full\r\n");
		fxsSetLineStateQueue_Clear();
		return;
	}
	#if 0
	if ((slicSetQueue.lineInfoQueue[(slicSetQueue.w_idx-1+SI_SET_QUEUE_SIZE)%SI_SET_QUEUE_SIZE].chan == plineInfo->chan) 
		&&(slicSetQueue.lineInfoQueue[(slicSetQueue.w_idx-1+SI_SET_QUEUE_SIZE)%SI_SET_QUEUE_SIZE].state == plineInfo->state)
		&&(slicSetQueue.lineInfoQueue[(slicSetQueue.w_idx-1+SI_SET_QUEUE_SIZE)%SI_SET_QUEUE_SIZE].polarity== plineInfo->polarity)
		&&(slicSetQueue.lineInfoQueue[(slicSetQueue.w_idx-1+SI_SET_QUEUE_SIZE)%SI_SET_QUEUE_SIZE].retTicStat== plineInfo->retTicStat)
		){
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG,"setQueue repeat, ignore later chan %d state 0x%x\r\n",plineInfo->chan,plineInfo->state);
		return;
	}
	#endif

	slicSetQueue.lineInfoQueue[slicSetQueue.w_idx].chan = plineInfo->chan;
	slicSetQueue.lineInfoQueue[slicSetQueue.w_idx].state = plineInfo->state;
	slicSetQueue.lineInfoQueue[slicSetQueue.w_idx].polarity= plineInfo->polarity;
	slicSetQueue.lineInfoQueue[slicSetQueue.w_idx].retTicStat= plineInfo->retTicStat;
	
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG,"setQueue<%d/%d>_Insert idx %d, chan %d, state 0x%x\r\n",
		max_cnt,
		SI_SET_QUEUE_SIZE,
		slicSetQueue.w_idx, 
		slicSetQueue.lineInfoQueue[slicSetQueue.w_idx].chan,
		slicSetQueue.lineInfoQueue[slicSetQueue.w_idx].state
		);

	slicSetQueue.w_idx++;
	slicSetQueue.w_idx %= SI_SET_QUEUE_SIZE;
	slicSetQueue.count++;
	if(slicSetQueue.count>max_cnt){
		max_cnt=slicSetQueue.count;
	}
}

static char fxsSetLineStateQueue_Check(void)
{
	if ((slicSetQueue.r_idx == slicSetQueue.w_idx) || (slicSetQueue.count == 0))
	{
		return 0;
	}
	return 1;
}

void fxsSetLineStateQueue_Get(void)
{
	static short get_cnt=0;

	if(slicTimer_Check()==(char)SLIC_TIMER_CMP){
		if(fxsSetLineStateQueue_Check()){

			SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG,"setQueue_Get idx %d, chan %d, state 0x%x\r\n", 
				slicSetQueue.r_idx, 
				slicSetQueue.lineInfoQueue[slicSetQueue.r_idx].chan,
				slicSetQueue.lineInfoQueue[slicSetQueue.r_idx].state
				);
			
			slicFunc[slicSetQueue.lineInfoQueue[slicSetQueue.w_idx].chan][FXS_SET_LINE_STAT]((char *)&slicSetQueue.lineInfoQueue[slicSetQueue.r_idx]);	

			slicSetQueue.r_idx++;
			slicSetQueue.r_idx %= SI_SET_QUEUE_SIZE;
			slicSetQueue.count--;
			slicTimer_Start();
		}
	}
}

#endif

#ifdef OSBN_VOIP
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,31)
extern int SPI_cfg(int type,int id);
#endif

int SLIC_DeviceVerify(void)
{
	int i = 0;
	int ret=0; 

	g_deviceInfo.deviceNum = 0;
	g_deviceInfo.fxsNum = 0;
	g_deviceInfo.fxoNum = 0;
	g_deviceInfo.slicType = 0;
	
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "SLIC_DeviceVerify\n");  /*upgrade_to_4_channel*/

	#ifdef SLIC_SILICON
    SPI_cfg(SLIC_TYPE_SILICON,0);  /*upgrade_to_4_channel*/
    #else 
    SPI_cfg(SLIC_TYPE_ZARLINK,0);  
    #endif	

	for(i=0; i< DSP_MAX_CHAN; i++){/*upgrade_to_4_channel*/
   		/* Init SPI */
		if(slicFunc[i]!=NULL){ /*upgrade_to_4_channel*/
			ret = slicFunc[i][DEVICE_VERIFY]((deviceInfo_t*)&g_deviceInfo);
			SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "DEVICE_VERIFY ret:%d\r\n", ret); /*Here_Modify*/
			if( (ret==0) && (i==0)) /*DEVICE_VERIFY Fail*/{
				SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, "DEVICE_VERIFY Fail, ret:%d\r\n", ret);
				return -1;
			}
		} /*upgrade_to_4_channel*/		 
	}
#if 0 /* Add by mtk06404_20160318 for gpio set */
	if(isEN751221){		
		if(g_deviceInfo.deviceNum==1){
			if(spi_interface_type==NORMAL_PCM_SPI_INTERFACE)
				regWrite(IOMUX_CONTROL1,(regRead(IOMUX_CONTROL1))&(~GPIO_SPI_SLIC_2nd)); /*release GPIO3  */
			else
				regWrite(IOMUX_CONTROL1,(regRead(IOMUX_CONTROL1))&(~GPIO_ZSI_ISI_2nd)); /*release GPIO4~7  */
		}		
	}
#endif    
	return 0;
}
#endif

int SLIC_Dest(int cid){
	slicFunc[cid][SHUTDOWN_HW](NULL);
	#ifdef SLIC_SILICON  
	fxsSetLineStateQueue_Clear();  //dyma_131230
	#endif
	return 0;
}
int SLIC_ConfQuery(_TDI_initSetting_t* pinitSetting){
	int retVal = _TDI_FAIL;
	int chan = 0;
	if(!pinitSetting)
		return _TDI_FAIL;
	chan = pinitSetting->chan;
	memcpy(pinitSetting,&(g_initSetting[chan]),sizeof(_TDI_initSetting_t));
	return _TDI_SUCCESS;
}

int SLIC_Init(_TDI_initSetting_t* pinitSetting){
	int retVal = _TDI_FAIL;
	int i = 0;
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "FXS_NUM=%d FXO_NUM=%d\n", g_deviceInfo.fxsNum, g_deviceInfo.fxoNum);
	g_slicGainTbl_size=sizeof(g_slicGainTbl)/sizeof(gainTbl_t);
	slicDumpUnLock(&slicDumpFlag); //dyma_130306
	if(pinitSetting){
		#ifdef SLIC_SILICON  //dyma_131212
		if(pinitSetting->chan == 0){
			pinitSetting->speakSlicVal = 0;   // Speak 0dB is CT default setting
			pinitSetting->listenSlicVal = -6; // Listen -6dB is CT default setting
		}
		else{
			pinitSetting->speakSlicVal = 0;
			pinitSetting->listenSlicVal = -6;
		}
		fxsSetLineStateQueue_Clear();  //dyma_131230
		#else
		if(pinitSetting->chan == 0){
			pinitSetting->speakSlicVal = 0x4000;
			pinitSetting->listenSlicVal = 0x4000;
		}
		else{
			pinitSetting->speakSlicVal = 0x4000;
			pinitSetting->listenSlicVal = 0x4000;
		}
		#endif
		
		g_slicInitStat[pinitSetting->chan] = _TDI_FAIL;
		if(slicFunc[pinitSetting->chan] == slicFunc[_TDI_CHANNEL_2]){
			g_slicInitStat[_TDI_CHANNEL_2] = _TDI_FAIL;
		}
		
		retVal = slicFunc[pinitSetting->chan][DEVICE_INIT]((char*)pinitSetting);
		g_slicInitStat[pinitSetting->chan] = retVal;
		/*g_slicInitStat of FX0 is the same as the FXS's if FXS's slicFunc is the same as FX0's slicFunc*/
		if(slicFunc[pinitSetting->chan] == slicFunc[_TDI_CHANNEL_2]){
			g_slicInitStat[_TDI_CHANNEL_2] = retVal;
		}
		/*mtk04880: store one copy for init setting*/
		memcpy(&(g_initSetting[pinitSetting->chan]),pinitSetting,sizeof(_TDI_initSetting_t));
	}
	else{
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, "Input is Null\n");
	}
	return retVal;
}

int SLIC_IRQENABLE(int en){
	return 0;
}
/**
 *	@brief				Get or set the Linefeed status
 *	@param	cid			Channel id (start from 0).
 *	@param	isSet		1: Set LF status 0: Get LF status
 *	@param	cfgStatus_p	Pointer to store user-configured line-feed status.
 *	@param	currStatus_p	Pointer to store current line-feed status.
 *	@retval	0			Configuration success
 *	@retval	!=0			Configuration failed
 *
 *	This function allows user to get or current Line-Feed status of SLIC for specific channel.
 *	If isSet != 1, this procedure would process LF status set and only use the data pointed by cfgStatus_p
 *	to set current LF status.
 *	If isSet == 0, this procedure would process LF status get and 2 parts of status would be returned,
 *	one is current SLIC status and another one is user-configured status.
 *
 *	The definition of LF status is:
 *		0: Open    		1: Forward active
 *		2: Forward OHT	3: TIP Open
 *		4: Ringing		5: Reverse active
 *		6: Reverse OHT	7: RING Open
 */
int SLIC_LFStatusCfg(unsigned int isSet,_TDI_lineInfo_t* plineInfo){

	int lineState = plineInfo->state;
	_TDI_batteryPolInfo_t batteryPolInfo;
	static int pre_state[DSP_MAX_CHAN]={0,0,0,0}; /*upgrade_to_4_channel*/

	if(plineInfo){
		if(isSet){
			switch(plineInfo->state){
			case SLIC_LF_OPEN:
				/*fall through*/
			case SLIC_LF_RING_OPEN:
				/*fall through*/
			case SLIC_LF_TIP_OPEN:
				plineInfo->state = TIC_STATE_GROUND_START;
				break;
			case SLIC_LF_FORWARD_ACTIVE:
				/*fall through*/
				plineInfo->state = TIC_STATE_ACTIVE; //dyma_20140508
				break;
			case SLIC_LF_REV_ACTIVE:
				/*fall through*/
				plineInfo->state = TIC_STATE_REV_ACTIVE; //dyma_20140508
				break;
			case SLIC_LF_REV_OHT:
				plineInfo->state = TIC_STATE_REV_OHT;
				break;
			case SLIC_LF_FORWARD_OHT:
				plineInfo->state = TIC_STATE_OHT;
				break;
			case SLIC_LF_RING_PAUSE:
				plineInfo->state = TIC_STATE_RING_PAUSE;
				break;
			case SLIC_LF_RINGING:
				plineInfo->state = TIC_STATE_RINGING;
				break;
			case SLIC_LF_DOWN:
				plineInfo->state = TIC_STATE_POWER_DOWN;
				break;	
		/*add for COC by mtk69090  #OSBNB00030711*/
			case SLIC_LF_STAND_BY:
				plineInfo->state = TIC_STATE_STAND_BY;
				break;
		/*end of add for COC by mtk69090*/
			default:
				plineInfo->state = TIC_STATE_ACTIVE;
				break;
			}		
			#ifndef SLIC_SILICON//xflu 20131022
			slicFunc[plineInfo->chan][FXS_SET_LINE_STAT]((char *)plineInfo);
			if (plineInfo->polarity == SLIC_LF_POL_FORWARD) {
				batteryPolInfo.chan = plineInfo->chan;
				batteryPolInfo.polarity = TIC_BATT_POL_FORWARD;
				slicFunc[plineInfo->chan][FXS_SET_BATTERY_POL]((char *)&batteryPolInfo);
			}else if (plineInfo->polarity == SLIC_LF_POL_REVERSE) {
				batteryPolInfo.chan = plineInfo->chan;
				batteryPolInfo.polarity = TIC_BATT_POL_REVERSE;
				slicFunc[plineInfo->chan][FXS_SET_BATTERY_POL]((char *)&batteryPolInfo);
			}
			#else  //dyma_131230
			/*  mtk07059 20140513, to keep reverse after ring and ring pasue. */
			if ((plineInfo->polarity == SLIC_LF_POL_REVERSE) && \
					(plineInfo->state == TIC_STATE_ACTIVE || \
					 plineInfo->state == TIC_STATE_OHT || \
					 plineInfo->state == TIC_STATE_RING_PAUSE )){

				plineInfo->state = TIC_STATE_REV_OHT;

			}
			fxsSetLineStateQueue_Insert((char *)plineInfo);
			#endif
		}
		else{
			slicFunc[plineInfo->chan][FXS_GET_LINE_STAT]((char *)plineInfo);

			switch(plineInfo->retTicStat){
			case TIC_STATE_POWER_DOWN:
				plineInfo->state = SLIC_LF_OPEN;
				break;
			case TIC_STATE_ACTIVE:
				plineInfo->state = SLIC_LF_FORWARD_ACTIVE;
				break;
			case TIC_STATE_REV_ACTIVE: //dyma_20140508
				plineInfo->state = SLIC_LF_REV_ACTIVE;
				break;
			case TIC_STATE_RINGING:
				plineInfo->state = SLIC_LF_RINGING;
				break;
			case TIC_STATE_GROUND_START:
				plineInfo->state = SLIC_LF_TIP_OPEN;
				break;
			case TIC_STATE_OHT:
				plineInfo->state = SLIC_LF_FORWARD_OHT;
				break;
			case TIC_STATE_REV_OHT:
				plineInfo->state = SLIC_LF_REV_OHT;
				break;
			default:
				plineInfo->state = SLIC_LF_FORWARD_ACTIVE;
				break;
			}
		}

		if(pre_state[plineInfo->chan]!=plineInfo->state)  //dyma_130823
		{
			pre_state[plineInfo->chan]=plineInfo->state;
			SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG,"LFSCfg %s chan(%d) state(0x%02x)\n",isSet?"Set":"Get",plineInfo->chan,plineInfo->state);
		}
	}
	else{
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, "ERR \n");
	}
	return 0;
}

int SLIC_ISR(int chan)
{
	int tmpChan = chan;
	if(g_slicInitStat[chan] ==  _TDI_SUCCESS){
		slicDumpChkLock(&slicDumpFlag);  //dyma_130306
		#ifdef SLIC_SILICON
		fxsSetLineStateQueue_Get();  //dyma_131230
		#endif
		return slicFunc[chan][FXS_EVENT_PROCESS]((char* )&tmpChan);
	}
	else{
		return _TDI_FAIL;
	}
	return 0;
}
#ifdef OSBN_VOIP
int SLIC_getDeviceInfo(int selection)
{
	int retVal = 0;
	
	switch(selection){
		case DEVICE_NUM:
			retVal = g_deviceInfo.deviceNum;
			break;
		case FXS_NUM:
			retVal = g_deviceInfo.fxsNum;
			break;
		case FXO_NUM:
			retVal = g_deviceInfo.fxoNum;
			break;
		case SLIC_TYPE:
			retVal = g_deviceInfo.slicType;
			break;
		case TOTALCH_NUM:
			retVal = g_deviceInfo.fxsNum + g_deviceInfo.fxoNum;
			break;
		case PCM1_CH_NUM:
			retVal = g_deviceInfo.pcm1ChNum;
			break;
		case PCM2_CH_NUM:
			retVal = g_deviceInfo.pcm2ChNum;
			break;
		default:
			break;
	}
	return retVal;
}
#else
int SLIC_TotalChannelGet(void)
{	
	return slic_ch_num;
}
#endif


void (*slic_event_notify)(int cid, ...);

int SLIC_EventNotify_Register(void *data){
	slic_event_notify=data;
	return 0;
}

/**
 *  @brief  		SLIC Retrieve the Hook Status 
 *  @param cid: 	channel id (start from 0).
 *  @param status:	OFFHOOK:0 and ONHOOK:1
 *  @retval return:	larger than 0 means something failed. 
 */
int SLIC_HOOKStatusGet(_TDI_hookStat_t* pHookStat)
{
	if(pHookStat){
		slicFunc[pHookStat->chan][FXS_GET_HOOK_STAT]((char *)pHookStat);
		if(pHookStat->hookStat == TIC_IMMEDIATE_OFFHOOK){
			/*  sip register led and hook staus led shared add  by mtk06404  */
/* modified by mtk07059_20141027, remove compiler option for sipRegLed. And add cid=9 for sip register light. */
	if(!TCSUPPORT_VOIP_LED_APPCTRL_VAL){
		if(pHookStat->chan == 0){
			ledTurnOn(LED_VOIP_HOOK1_STATUS);
		}
		else if(pHookStat->chan == 1){
			ledTurnOn(LED_VOIP_HOOK2_STATUS);
		}
	}
			
			/*translate TDI infor to fxs3*/
			pHookStat->hookStat = SLIC_OFFHOOK;
		}
		else{
	if(!TCSUPPORT_VOIP_LED_APPCTRL_VAL){
		if(pHookStat->chan == 0){
			ledTurnOff(LED_VOIP_HOOK1_STATUS);
		}
		else if(pHookStat->chan == 1){
			ledTurnOff(LED_VOIP_HOOK2_STATUS);
		}
	}
	
			/*translate TDI infor to fxs3*/
			pHookStat->hookStat = SLIC_ONHOOK;
		}
	}
	else{
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, " ERR \n");
	}
	return pHookStat->hookStat;
}

int SLIC_RING(int cid, int en){
	/*20120420 mtk04880: No use*/
#if 0
	_TDI_hookStat_t tmpHookStat;
	_TDI_lineInfo_t tmpLineInfo;
#ifdef OSBN_VOIP
	if(cid<0 || cid>=g_deviceInfo.deviceNum) return 0;
#else
	if(cid<0 || cid>=slic_ch_num) return 0;
#endif

	tmpHookStat.chan = tmpLineInfo.chan = cid;
	if(en){//play ring
		slicFunc[cid][FXS_GET_HOOK_STAT]((char *)(&tmpHookStat));
		if(tmpHookStat.hookStat == TIC_IMMEDIATE_ONHOOK){
			tmpLineInfo.state = TIC_STATE_RINGING;
			slicFunc[cid][FXS_SET_LINE_STAT]((char *)(&tmpLineInfo));
		}
	}
	else{
		tmpLineInfo.state = TIC_STATE_RING_PAUSE;
		slicFunc[cid][FXS_SET_LINE_STAT]((char *)(&tmpLineInfo));
	}
	return 0;
#endif
	return 0;

}

int SLIC_ringState(_TDI_ringSetting_t *pRingSetting){
	int ret = 0;
	if(pRingSetting){

		ret = slicFunc[pRingSetting->chan][FXS_RING_STATE]((char *)pRingSetting);
		return ret;
	}
	else{
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, "%s ERR \n",__func__);
		return _TDI_FAIL;
	}
}

int SLIC_RegRead(int cid, unsigned int reg, unsigned int *value)
{
	/*TODO*/
	return 0;
}

int SLIC_RegWrite(int cid, unsigned int reg, unsigned int data)
{
	/*TODO*/
	return 0;
}

int SLIC_IndregRead(int cid, unsigned int reg, unsigned int *value)
{
	/*TODO*/
	return 0;
}

int SLIC_IndregWrite(int cid, unsigned int reg, unsigned int data)
{
	/*TODO*/
	return 0;
}
#ifndef OSBN_VOIP
int SLIC_AudioVolumeSetup(int cid, int rx_vol, int tx_vol){
	/*TODO*/
	return 0;
}
#endif
/**
 *  @brief			Instruct SLIC chip to setup 
 *					ring impendace.
 *  @param cid: 	channel id (start from 0).
 *  @param imp: 	Ring impedance:
 *                         0 -> 600ohms
 *                         1 -> 900ohms
 *                         2 -> Japan (600ohms + 1uF)
 *                         3 -> 900ohms + 2.16uF
 *                         4 -> CTR21 270ohms+(750ohms||150nF)
 *                         5 -> Australia/New Zealand220ohms+(820ohms||120nF)
 *                         6 -> Slovakia/Slovenia/South Africa220ohms+(820ohms||115nF)
 *                         7 -> China 200ohms+(680ohms||100nF)
 *  @retval return : larger than 0 means something failed. 	
 */
int SLIC_RINGImpSetup(int cid ,int imp){
	/*TODO*/
	return 0;
}

static int slic_gain_mapping(int *tx_gain,int *rx_gain){
	/*TODO*/
	return 0;
}

/**
 *  @brief		Audio Gain TX and RX Setup
 *  @param	cid	channel id (start from 0).
 *  @param	agc	The transmitting and receiving gain structure.
 *  @retval	SLIC_SUCCESS: Success, !=SLIC_SUCCESS:Failed
 */
int SLIC_AudioGainSetup(int cid, int speakdb, int listendb){
	int i = 0, ret = 0;
	int tmpEntrys,tmpVal,tmpSpeakSlicVal,tmpListenSlicVal;
	_TDI_gainSetting_t gainSetting={0};
	tmpVal = tmpSpeakSlicVal = tmpListenSlicVal = 0xff;  //dyma_131216 //Silabs may use value 0, so change default error value to 0xff which will not be used by both zarlink and silabs


	tmpEntrys = sizeof(g_slicGainTbl)/sizeof(gainTbl_t);


	tmpVal = speakdb;
	if((tmpVal <= 24) && (tmpVal >= -24)){
		tmpSpeakSlicVal = g_slicGainTbl[tmpVal + 24].slicVal;
	}
	
	tmpVal = listendb;
	if((tmpVal <= 24) && (tmpVal >= -24)){
		tmpListenSlicVal = g_slicGainTbl[tmpVal + 24].slicVal;
	}


	if((tmpSpeakSlicVal!=0xff)&(tmpListenSlicVal!=0xff)){  //dyma_131216
		gainSetting.chan=cid;
		gainSetting.speakSlicVal = tmpSpeakSlicVal;
		gainSetting.listenSlicVal  = tmpListenSlicVal;
	//	printk("%s : S/L:%x/%x \n",__func__,g_initSetting[cid].speakSlicVal,g_initSetting[cid].listenSlicVal);
		ret = slicFunc[cid][FXS_SET_GAIN_LV]((char *)(&gainSetting));
		if (!ret) {
			  SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, "ERR S/L:%x/%x \n", g_initSetting[cid].speakSlicVal,g_initSetting[cid].listenSlicVal);
			  return -1;
		}
		g_initSetting[cid].speakSlicVal = tmpSpeakSlicVal;
		g_initSetting[cid].listenSlicVal = tmpListenSlicVal;
		
	}
	else{
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, " ERR \n");
		return -1;
	}

	return 0;
}

/**
 *  @brief		SLIC ring interval Setup
 *  @param	cid			channel id (start from 0).
 *  @param	active_ms	active ms (-1 means reseve original setting),maximun 8 second
  *  @param	inactive_ms	inactive ms (-1 means reseve original setting),maximun 8 second
 *  @retval	SLIC_SUCCESS: Success, !=SLIC_SUCCESS:Failed
 */
 int SLIC_RingIntervalSetup(int cid, int active_ms, int inactive_ms)
{
	 /*TODO*/
	return 0;
}

/**
 *  @brief Setup ring profile
 *  @param cid: channel ID 
 *  @param profile: country
 *  @return none
 */
int SLIC_RingProfileSetup(int cid,int profile){
	/*TODO*/
	return 0;
}

#ifdef OSBN_VOIP
int SLIC_FxoGetRingCadence(unsigned char chan){
	return slicFunc[chan][FXO_GET_RING_CAD](NULL);
}

int SLIC_FxoHookStatusSet(_TDI_hookStat_t* pHookStat){
	if(pHookStat){
		slicFunc[pHookStat->chan][FXO_SET_HOOK_STAT]((char *)&(pHookStat->hookStat));
	}
	else{
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, " ERR \n");
	}
	return 0;
}

int SLIC_FxoLineStatusGet(_TDI_fxoLineStat_t* pFxoLineStat){
	int ret = 0;
	
	if(pFxoLineStat){
		ret = slicFunc[pFxoLineStat->chan][FXO_GET_LINE_STAT]((char *)pFxoLineStat);
		return ret;
	}
	else{
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, " ERR \n");
		return _TDI_FAIL;
	}
}

int SLIC_FxoPSTNStatusGet(unsigned char chan){
	return slicFunc[chan][FXO_GET_PSTN_STAT]((char *)NULL);
}

int SLIC_FxoLIUStatusGet(unsigned char chan){
	return slicFunc[chan][FXO_GET_LIU_STAT]((char *)NULL);
}

/*[OSBNB00041138] ADD by peter.jiang@20141126, support dsp loopback*/
int SLIC_LoopbackEnable(unsigned char chan, int en){
	_TDI_fxsSetLoopback_t loopback = {0};
	loopback.chan = chan;
	loopback.enable = en;
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG, "SLIC_LoopbackEnable chan %d en %d\n", chan, en);
	return slicFunc[chan][FXS_SET_LOOPBACK]((char *)&loopback);
}

/*[OSBNB00045371] ADD by mtk07059_20150423 , support ring parameters set. */
int SLIC_ringParams( _TDI_fxsSetRingParams_s *ringParams){
	int ret = 0;
	
	if(ringParams){
		ret = slicFunc[ringParams->chanId][FXS_SET_RINGPARAMS](ringParams);
		return ret;
	}
	else{
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, " ERR \n");
		return _TDI_FAIL;
	}
}

/*[OSBNB00047036] ADD by mtk07059_20150616 , support DC feed parameters set. */
int SLIC_dcFeedParams( _TDI_fxsSetDcFeedParams_s *dcFeedParams){
	int ret = 0;
	
	if(dcFeedParams){
		ret = slicFunc[dcFeedParams->chanId][FXS_SET_DCFEEDPARAMS](dcFeedParams);
		return ret;
	}
	else{
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, " ERR \n");
		return _TDI_FAIL;
	}
}

int slic_register_read_proc(char *page, char **start, off_t off, int count, int *eof, void *data){
	int len = 0;

#ifdef SLIC_SILICON  //dyma_131106
	printk("\n-------------------------------------------------------\n");
	printk("Read slic register usage : \n");
	printk("echo \"[slic_id] [address(dec)] [0(read)] [(0x)data(Hex)]\" > slicRegister\n");
	printk("Note: when [0(read)]=0=read, [(0x)data(Hex)] is not used\n");
	printk("example: echo \"0 14 0 0x0\" > /proc/fxs/slicRegister \n");
	printk("-------------------------------------------------------\n");
	printk("Write slic register usage : \n");
	printk("echo \"[slic_id] [address(dec)] [1(write)] [(0x)data(Hex)]\" > slicRegister\n");
	printk("example: echo \"0 14 1 0x40 \" > /proc/fxs/slicRegister \n");
	printk("-------------------------------------------------------\n");
#else
	printk("\n-------------------------------------------------------\n");
	printk("Read slic register usage : \n");
	printk("echo \"[slic_id] [(0x)address] [len] [(0x)data]\" > slicRegister\n");
	printk("Note: 1.read address should be odd number. 2.data value is not used in read\n");
	printk("example: echo \"0 0x71 1 0x0 \" > /proc/fxs/slicRegister \n");
	printk("-------------------------------------------------------\n");
	printk("Write slic register usage : \n");
	printk("echo \"[slic_id] [(0x)address] [len] [(0x)data]\" > slicRegister\n");
	printk("Note: write adress should be evet number.\n");
	printk("example: echo \"0 0x70 1 0x7 \" > /proc/fxs/slicRegister \n");
	printk("-------------------------------------------------------\n");
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

/*
 * ======== silab_bitReverse() ========
 * SiLab SLIC's SPI control interface
 * The CID[0:4] in the Control Byte is LSB first,
 * the bit order must be reversed
 *
 * Input Value : MSB first number
 * Return Value: Reversed bit order number
 */
static uint8 silab_bitReverse(    
    uint8 fiveBits)
{
    /*
     * Bit order [4,3,2,1,0] --> [0,1,2,3,4]
     */
    fiveBits &= 0x1F;
    return (((fiveBits & 0x01) << 4) | ((fiveBits & 0x02) << 2) |
             (fiveBits & 0x04)       |
            ((fiveBits & 0x08) >> 2) | ((fiveBits & 0x10) >> 4));
}

int slic_register_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data){
	char val_string[40];
	uint8 temp_val = 0;
	int realData = 0x0;
	int slicId =0;

	#ifdef SLIC_SILICON  //dyma_131106
	int slicRegAddress = 0;
	int r_w_flag = 0;  /*read write flag : 0=write, 1=read*/
	uint8 ctrl;
	#else
	uint8 res_arr[16] = {0};
	int slicCmd = 0x0;
	int datalen = 0;
	int i = 0;
	#endif

	if (count > sizeof(val_string) - 1)
		return -EINVAL;
	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	#ifdef SLIC_SILICON  //dyma_131106
	sscanf(val_string,"%d %d %d 0x%x",&slicId, &slicRegAddress, &r_w_flag, &realData);
	val_string[count] = '\0';
	if((slicRegAddress > 0) && (slicId < 3) && (slicId >= 0)){
		if(r_w_flag == 1){ /*write*/
	        ctrl = (0x20 | silab_bitReverse((uint8)slicId));
			SPI_bytes_write(SLIC_TYPE_SILICON,spi_interface_type,slicId, ctrl, (uint8)slicRegAddress, &realData, 1);
			printk("write address(%d) value : 0x%x\n", slicRegAddress, realData);	
		}
		else if(r_w_flag == 0){ /*read*/
			ctrl = (0x60 | silab_bitReverse((uint8)slicId));
			SPI_bytes_read(SLIC_TYPE_SILICON,spi_interface_type,slicId, ctrl, (uint8)slicRegAddress, &realData, 1);
			printk("read address(%d) value : 0x%x\n", slicRegAddress, temp_val);
		}
	}
	#else
	sscanf(val_string,"%d 0x%x %d 0x%x",&slicId, &slicCmd, &datalen, &realData);
	val_string[count] = '\0';

	if((slicCmd > 0) && (datalen < 16) && (datalen >= 0)){
		if(slicCmd%2 == 0){ /*write*/
			temp_val = (uint8)realData;
		    VpMpiCmd(slicId, 0x1, slicCmd, datalen, &temp_val);
		    printk("write address(%x) value:%x\n", slicCmd, temp_val);
		}
		else{ /*read*/
		    VpMpiCmd(slicId, 0x1, slicCmd, datalen, res_arr);
			for (i = 0 ; i < datalen ; i++) {
		    	printk("read address(%x): (%d)byte = %x\n", slicCmd, i, res_arr[i]);
			}
		}
	}
	#endif

	return count;
}

//dyma_130306
int SLIC_DumpRegister_Cat(char *page, char **start, off_t off, int count, int *eof, void *data){
	int len = 0;

	printk("\n-------------------------------------------------------\n");
	printk("Dump slic register usage : \n");
	printk("echo \"[slic chan]\" > /proc/fxs/slicDumpReg\n");
	printk("example: echo \"0\" > /proc/fxs/slicDumpReg \n");
	printk("-------------------------------------------------------\n");

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
int SLIC_DumpRegister_Echo(struct file *file, const char *buffer,unsigned long count, void *data){
	char val_string[40];
	int chan=0;

	if (count > sizeof(val_string) - 1)
		return -EINVAL;
	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	sscanf(val_string,"%d", &chan);
	val_string[count] = '\0';

	slicDumpLock(&slicDumpFlag);  //dyma_130306
	slicFunc[chan][DUMP_REGISTER]((char* )&chan);
	slicDumpUnLock(&slicDumpFlag);  //dyma_130306

	return count;
}
//#ifndef SLIC_SILICON
#if 1 //Roger_lt
int* SLIC_DeviceLineTest(_TDI_devicelinetest_t* plinetest)
{
		int lineteststat=0;
		sliclinetestFlag[plinetest->chan]=1; 	
		lineteststat=slicFunc[plinetest->chan][FXS_LT_START](plinetest);
		sliclinetestFlag[plinetest->chan]=0;
		
		if(lineteststat==1)
		{
			return slicFunc[plinetest->chan][FXS_LT_RESULT_GET]((char *)NULL);	
		}
		else
		{
			return NULL;
		}		 
}

int SLIC_lineteststatus(int chan)
{
	return sliclinetestFlag[chan];
}
int SLIC_linetest_cat(char *page, char **start, off_t off, int count, int *eof, void *data){
	int i,len = 0;

	switch (lineTestId)
	{	
		case 1://LT_TID_LINE_V
			/* voltage test result format :vAcTip-Ground  vAcRing-Ground vDcTip-Ground  
				vDcRing-Ground vAcTip-Ring ,  Add for proc to read result 20160920 */
			len += sprintf(page+len, "%d %d %d %d %d\n", lineTestProcRes[0], 
			lineTestProcRes[1], lineTestProcRes[2], lineTestProcRes[3], lineTestProcRes[4]);
			break;
#ifndef SLIC_SILICON			
		case 4://LT_TID_RES_FLT	
			/* resistance test  result format : rTip-Ground  rRing-Ground rTip-Ring Add for proc to read result 20160920 */
			len += sprintf(page+len, "%d %d %d\n", lineTestProcRes[0], lineTestProcRes[1], lineTestProcRes[2]);

			break;
		
		case 3:
			
			for (i=0; i<sizeof(lineTestProgress); i++){
				if(SLIC_lineteststatus(i)){	
					printk("\r\nChan %d  lineTestProgress=%d\r\n",i,lineTestProgress[i]);
					lineTestProgress[i] = LINE_TEST_DOING;
				}
					
			}
			len = sprintf(page, "%d %d\n", lineTestProgress[0],lineTestProgress[1]);

			break;
#else
		case 3:
			/* resistance test  result format :rtg  rrg rtr ,  Add for proc to read result 20160920 */
			len += sprintf(page+len, "%d %d %d\n", lineTestProcRes[0], lineTestProcRes[1], lineTestProcRes[2]);

			break;		

#endif

		default:
			break;
	}

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

int SLIC_linetest_echo(struct file *file, const char *buffer,unsigned long count, void *data){
	char val_string[40];
	int chan=0;
	int testid=0;	
	_TDI_devicelinetest_t plinetest;

	int* linetestresult=NULL;
	
	if (count > sizeof(val_string) - 1)
	{
		printk("\n-------------------------------------------------------\n");
		printk("slic linetest usage : \n");
		printk("echo \"[slic chan] [testid]\" > /proc/fxs/sliclinetest\n");
		printk("example: echo \"0 3\" > /proc/fxs/sliclinetest \n");
		printk("-------------------------------------------------------\n");
		return -EINVAL;
	}
	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

		/*Add for proc to read result 20160920 */
		memset(lineTestProcRes, 0, sizeof(lineTestProcRes));
		
		sscanf(val_string,"%d %d", &chan,&testid);
		val_string[count] = '\0';

		plinetest.chan=chan;
		plinetest.testid=testid;

		linetestresult=SLIC_DeviceLineTest(&plinetest);

		if(linetestresult!=NULL)
		{
			lineTestId = plinetest.testid;
			#ifndef SLIC_SILICON
			switch (plinetest.testid)
			{					
				case LT_TID_LINE_V:
				{	
					  /* Test results for the test LT_TID_LINE_V */
						LtLineVResultType *pResultTemp ;
						pResultTemp=(LtLineVResultType*)linetestresult;
						
						printf("   Line Test Result -- LT_TID_LINE_V\n");
						printf("	fltMask 	 =	%d\n" , 				  pResultTemp->fltMask	   )  ;  
						printf("	measStatus	 =	%d\n" , 				  pResultTemp->measStatus  )  ;
						printf("	vAcTip		 =	%ldmVrms\n" , 					  pResultTemp->vAcTip	   )  ;
						printf("	vAcRing 	 =	%ldmVrms\n" , 			  pResultTemp->vAcRing	  )  ;	
						printf("	vAcDiff 	 =	%ldmVrms\n" , 				pResultTemp->vAcDiff	)  ;  
						printf("	vDcTip		 =	%ldmV\n" , 				pResultTemp->vDcTip    )  ;   
						printf("	vDcRing 	 =	%ldmV\n" , 					pResultTemp->vDcRing   )  ;  
						printf("	vDcDiff 	 =	%ldmV\n\n" , 				pResultTemp->vDcDiff	)  ;

						/*Add for proc to read result 20160920 */
						lineTestProcRes[0] = pResultTemp->vAcTip;
						lineTestProcRes[1] = pResultTemp->vAcRing;
						lineTestProcRes[2] = pResultTemp->vDcTip;
						lineTestProcRes[3] = pResultTemp->vDcRing;
						lineTestProcRes[4] = pResultTemp->vDcDiff;
				}	
				break;

				case LT_TID_ROH:
				{
					  /* Test results for the test LT_TID_ROH */
					  
					  LtRohResultType *pResultTemp ;														 
					  pResultTemp=(LtRohResultType*)linetestresult;
					  printf("   Line Test Result -- LT_TID_ROH\n");
					  printf("	  fltMask	   =  %d\n" ,					pResultTemp->fltMask	)  ; 
					  printf("	  rLoop1	   =  %ld\n" ,					pResultTemp->rLoop1 	)  ; 
					  printf("	  rLoop2	   =  %ld\n" ,						pResultTemp->rLoop2 	)  ; 
					  printf("	  measStatus   =  %d\n\n" ,				pResultTemp->measStatus )  ;  
					  
				}	 
				break;		

				case LT_TID_RES_FLT:
				{
					  /* Test results for the test LT_TID_RES_FLT */
					  LtResFltResultType *pResultTemp ;
					  pResultTemp=(LtResFltResultType*)linetestresult;
					  printf("   Line Test Result -- LT_TID_RES_FLT\n ");
					  printf("   fltMask	=  %d\n" , pResultTemp->fltMask	  )  ;	
					  printf("   rtg		=  %ld(1/10 ohm)\n" , pResultTemp->rtg		  )  ;/* mtk08176_20150824 */
					  printf("   rrg		=  %ld(1/10 ohm)\n" , pResultTemp->rrg		  )  ;
					  printf("   rtr		=  %ld(1/10 ohm)\n" , pResultTemp->rtr		  )  ;	
					  printf("   rGnd		=  %ld(1/10 ohm)\n" , pResultTemp->rGnd		)  ;  
					  printf("   measStatus	=  %d\n\n" , pResultTemp->measStatus )  ; 

					  /*Add for proc to read result 20160920 */
					  lineTestProcRes[0] = pResultTemp->rtg;
					  lineTestProcRes[1] = pResultTemp->rrg;
					  lineTestProcRes[2] = pResultTemp->rtr;
				}	
				//VpSetLineState(pLCtx, VP_LINE_STANDBY);
				break;	
				
				case LT_TID_RINGERS:
				{				
						/* Test results for the test LT_TID_RINGERS */
						LtRingersResultType *pResultTemp ;
						pResultTemp=(LtRingersResultType*)linetestresult;
						if(pResultTemp->fltMask == 0||pResultTemp->fltMask == 2)
							lineTestProgress[chan] = LINE_TEST_CONNECT;
						else if(pResultTemp->fltMask == 1)
							lineTestProgress[chan] = LINE_TEST_UNCONNECT;
					   	printf("   Line Test Result -- LT_TID_RINGERS in SLIC ECHO\n ");
						printf("	fltMask			=  %d\n" , 				  pResultTemp->fltMask		   )  ;  
						printf("	measStatus		=  %d\n" , 				  pResultTemp->measStatus	   )  ;
						printf("	ren			=  %ld\n" , 					  pResultTemp->ren			   )  ;
						printf("	rentg			=  %ld\n" , 			  pResultTemp->rentg		  )  ;	
						printf("	renrg			=  %ld\n" , 				pResultTemp->renrg			)  ;  
						printf("	ringerTestType		=  %d\n\n" , 				pResultTemp->ringerTestType)  ;  
						
				}	  
				break;
				
				case LT_TID_DC_FEED_ST:
				{
					  /* Test results for the test LT_TID_DC_FEED_ST */
					  LtDcFeedSTResultType *pResultTemp ;
					  pResultTemp=(LtDcFeedSTResultType*)linetestresult;
					  printf("   Line Test Result -- LT_TID_DC_FEED_ST\n");
					  printf("	  fltMask	   =  %d\n" ,		 pResultTemp->fltMask		 );
					  printf("	  measStatus   =  %d\n" ,		 pResultTemp->measStatus	 );
					  printf("	  rTestLoad    =  %ld\n" ,		 pResultTemp->rTestLoad 	 );
					  printf("	  vTestLoad    =  %ld\n" ,		 pResultTemp->vTestLoad 	 );
					  printf("	  iTestLoad    =  %ld\n\n" ,		 pResultTemp->iTestLoad 	 ); 
				}
				break;
				
				case LT_TID_DC_VOLTAGE:
				{
					  /* Test results for the test LT_TID_DC_VOLTAGE */
					  LtDcVoltageResultType *pResultTemp ;
					  pResultTemp=(LtDcVoltageResultType*)linetestresult;
					  printf("   Line Test Result -- LT_TID_DC_VOLTAGE\n");
					  printf("	  fltMask			=  %d\n" ,		  pResultTemp->fltMask			   );
					  printf("	  measStatus		=  %d\n" ,		  pResultTemp->measStatus		   );
					  printf("	  measuredVoltage1	=  %ld\n" ,		  pResultTemp->measuredVoltage1    );
					  printf("	  measuredVoltage2	=  %ld\n\n" ,		  pResultTemp->measuredVoltage2    );
				}
				break;
				
				case LT_TID_RINGING_ST:
				{
					  /* Test results for the test LT_TID_RINGING_ST */
					  LtRingingSTResultType *pResultTemp ;
					  pResultTemp=(LtRingingSTResultType*)linetestresult;
					  printf("   Line Test Result -- LT_TID_RINGING_ST\n");
					  printf("	  fltMask		=  %d\n" ,		  pResultTemp->fltMask			   );
					  printf("	  openCktRingVol	=  %ld\n" ,		  pResultTemp->openCktRingVol	   );
					  printf("	  freq			=  %ld\n" ,		  pResultTemp->freq 			   );
					  printf("	  acRload		=  %ld\n" ,		  pResultTemp->acRload			   );
					  printf("	  iRload		=  %ld\n" ,		  pResultTemp->iRload			   );
					  printf("	  measStatus		=  %d\n\n" ,		  pResultTemp->measStatus		   );
		//				  ReadReg(0x01,VP880_SIGA_PARAMS_RD, VP880_SIGA_PARAMS_LEN);
		//				  VpInitLine(pLCtx, AC_600R_FXS, DC_25MA_CC, RING_20HZ_SINE);
		//				  VpInitRing(pLCtx, RING_CAD_STD, VP_PTABLE_NULL);
					  //VpSetLineState(pLCtx, VP_LINE_STANDBY);

				}	  
				break;
				
				case LT_TID_ON_OFF_HOOK_ST:
				{
					  /* Test results for the test LT_TID_ON_OFF_HOOK_ST */
					  LtOnOffHookSTResultType *pResultTemp ;
					  pResultTemp=(LtOnOffHookSTResultType*)linetestresult;
					  printf("   Line Test Result -- LT_TID_ON_OFF_HOOK_ST\n");
					  printf("	  fltMask	   =  %d\n" ,		 pResultTemp->fltMask		 );
					  printf("	  measStatus   =  %d\n\n" ,		 pResultTemp->measStatus	 ); 
				}	  
				break;	  
				
				case LT_TID_RD_BAT_COND:
				{	
					  /* Test results for the test LT_TID_RD_BAT_COND */
					  LtReadBatCondResultType *pResultTemp ;
					  pResultTemp=(LtReadBatCondResultType*)linetestresult;
					  printf("   Line Test Result -- LT_TID_RD_BAT_COND\n");
					  printf("	  fltMask          =  %d\n" ,		 pResultTemp->fltMask		);
					  printf("	  measStatus    =  %d\n" ,		 pResultTemp->measStatus	);
					  printf("	  bat1              =  %ldmV\n" ,		 pResultTemp->bat1			);
					  printf("	  bat2              =  %ldmv\n" ,		 pResultTemp->bat2			);
					  printf("	  bat3              =  %ldmV\n\n" ,		 pResultTemp->bat3			);
				}
					  
				break;	  
				case LT_TID_PRE_LINE_V:
				{	
					  /* Test results for the test LT_TID_PRE_LINE_V */
					  LtPreLineVResultType *pResultTemp ;
					  pResultTemp=(LtPreLineVResultType*)linetestresult;
					  printf("   Line Test Result -- LT_TID_PRE_LINE_V\n");
					  printf("	  fltMask	   =  %d\n" ,		 pResultTemp->fltMask		);
					  printf("	  measStatus   =  %d\n\n" ,		 pResultTemp->measStatus	);
				}	  
				break;	

				case LT_TID_CAP:
				{	
					  /* Test results for the test LT_TID_PRE_LINE_V */
					  LtCapResultType *pResultTemp ;
					  pResultTemp=(LtCapResultType*)linetestresult;
					  printf("   fltMask    = %d\n" ,pResultTemp->fltMask	  )  ;	
					  printf("   ctg        = %ld pF\n" , pResultTemp->ctg		  )  ;/* mtk08176_20150824 */
					  printf("   crg        = %ld pF\n" , pResultTemp->crg		  )  ;
					  printf("   ctr        = %ld pF\n" , pResultTemp->ctr		  )  ;	
				}	  
				break;	
				case LT_TID_RD_LOOP_COND:
				{	
					  /* Test results for the test LT_TID_RD_BAT_COND */
					  LtRdLoopCondResultType *pResultTemp ;
					  pResultTemp=(LtRdLoopCondResultType*)linetestresult;
					  printf("   Line Test Result -- LT_TID_RD_LOOP_COND\n");
					  printf("	  fltMask         =  %d\n" ,		 pResultTemp->fltMask		);
					  printf("	  measStatus      =  %d\n" ,		 pResultTemp->measStatus	);
					  printf("	  rloop             =  %ld/10ohm\n" ,       pResultTemp->rloop );
					  printf("	  imt             =  %lduA\n" ,		 pResultTemp->imt			);
					  printf("	  ilg             =  %lduA\n" ,		 pResultTemp->ilg			);
					  printf("	  vab             =  %ldmV\n" ,		 pResultTemp->vab			);
					  printf("	  vag             =  %ldmV\n" ,		 pResultTemp->vag			);
					  printf("	  vbg             =  %ldmV\n" ,		 pResultTemp->vbg			);
					  printf("	  vbat1           =  %ldmV\n" ,		 pResultTemp->vbat1			);
					  printf("	  vbat2           =  %ldmv\n" ,		 pResultTemp->vbat2			);
					  printf("	  vbat3           =  %ldmV\n\n" ,		 pResultTemp->vbat3			);
				}
				break;
				
				default:
					{
						printf("NO FUNDING testItem\n");
						break;
					}
				}
			#else
			ProSLICMLTType *pProSLICMLT;
			proslicMonitorType *inline_monitor;
                        /* mtk08176_20150824 */
			if(plinetest.testid!=8)
			        pProSLICMLT=(ProSLICMLTType*)linetestresult;
			else
				inline_monitor=(proslicMonitorType*)linetestresult;

			switch (plinetest.testid)
			{					
				case 1:
				case 2:
				{	
				 	 /* 
					**
					** Example Hazardous/Foreign Voltages Test
					**
					*/
					/* Process Available Results */
					/*
					** Available Results:
					**
					** pProSLICMLT->hazVDC.measTR		TR DC Voltage (mv)
					** pProSLICMLT->hazVDC.measTG		TG DC Voltage (mv)
					** pProSLICMLT->hazVDC.measRG		RG DC Voltage (mv)
					** pProSLICMLT->hazVAC.measTR		TR AC Voltage (Vrms^2/100)
					** pProSLICMLT->hazVAC.measTG		TG AC Voltage (Vrms^2/100)
					** pProSLICMLT->hazVAC.measRG		RG AC Voltage (Vrms^2/100)
					**
					*/
					  printk("    Hazardous/Foreign Voltages Test\n");
					  printk("	  pProSLICMLT->hazVDC.measTR   =  %d mV\n" ,	 pProSLICMLT->hazVDC.measTR );/* mtk08176_20150824 */
					  printk("	  pProSLICMLT->hazVDC.measTG   =  %d mV\n" ,	 pProSLICMLT->hazVDC.measTG	);
					  printk("	  pProSLICMLT->hazVDC.measRG   =  %d mV\n" ,   pProSLICMLT->hazVDC.measRG	);
					  printk("	  pProSLICMLT->hazVAC.measTR   =  %d (Vrms^2/100)\n" ,	 pProSLICMLT->hazVAC.measTR );
					  printk("	  pProSLICMLT->hazVAC.measT    =  %d (Vrms^2/100)\n" ,	 pProSLICMLT->hazVAC.measTG	);
					  printk("	  pProSLICMLT->hazVAC.measRG   =  %d (Vrms^2/100)\n" ,	 pProSLICMLT->hazVAC.measRG	);

					  /*Add for proc to read result 20160920 */
						lineTestProcRes[0] = pProSLICMLT->hazVAC.measTG	;
						lineTestProcRes[1] = pProSLICMLT->hazVAC.measRG;
						lineTestProcRes[2] = pProSLICMLT->hazVDC.measTG;
						lineTestProcRes[3] = pProSLICMLT->hazVDC.measRG;
						
					  
				}	
				break;
				case 3:
				{
				 	/* 
					**
					** Example Resistive Faults Test
					**
					*/
					  /* Process Available Results */
					  /*
					  ** Available Results:
					  **
					  ** pProSLICMLT->resFaults.measTR		  RTR (ohms/10)
					  ** pProSLICMLT->resFaults.measTG		  RTG (ohms/10)
					  ** pProSLICMLT->resFaults.measRG		  RRG (ohms/10)
					  **
					  */
					  printk("    Resistive Faults Test\n");
					  printk("	  pProSLICMLT->resFaults.measTR           =  %d (1/10 ohm)\n" ,		 pProSLICMLT->resFaults.measTR );/* mtk08176_20150824 */
					  printk("	  pProSLICMLT->resFaults.measTG           =  %d (1/10 ohm)\n" ,		 pProSLICMLT->resFaults.measTG	);
					  printk("	  pProSLICMLT->resFaults.measRG           =  %d (1/10 ohm)\n" ,      pProSLICMLT->resFaults.measRG	);	

					  /*Add for proc to read result 20160920 */
						lineTestProcRes[0] = pProSLICMLT->resFaults.measTG;
						lineTestProcRes[1] = pProSLICMLT->resFaults.measRG;
						lineTestProcRes[2] = pProSLICMLT->resFaults.measTR;
				}	 
				break;	
				case 4:
				{
					  
						/* 
						**
						** Example Receiver Offhook Test
						**
						*/

						/* Process Available Results */
						/*
						** Available Results:
						**
						** pProSLICMLT->roh.rohTrue 	
						**			RC_NONE 					No fault or receiver detected
						**			RC_MLT_ROH_FAIL_ROH 		Offhook receiver detected
						**			RC_MLT_ROH_FAIL_RESFAULT	Resistive fault detected
						*/

						printk("	Receiver Offhook Test\n");
						printk("	Receiver Offhook Test Result	 =	%d\n" , 	   pProSLICMLT->roh.rohTrue );				
		
				}
				break;					
				case 5:
				{				
					/* 
					**
					** Example REN Test
					**
					*/
					
					/* Process Available Results */
					/*
					** Available Results:
					**
					** pProSLICMLT->ren.renValue		REN (unitless/10)	
					**
					*/
					printk("	REN Test\n");
					printk("	pProSLICMLT->ren.renValue	 =	%d\n" , 	pProSLICMLT->ren.renValue );

						
				}	  
				break;
				
				case 6:
				{

					/* 
					**
					** Example Capacitive Faults Test
					**
					*/
					/* Process Available Results */
					/*
					** Available Results:
					**
					** pProSLICMLT->capFaults.measTR		CTR (nF/10)
					** pProSLICMLT->capFaults.measTG		CTG (nF/10)
					** pProSLICMLT->capFaults.measRG		CRG (nF/10)
					**
					*/
					  printk("    Capacitive Faults Test\n");
					  printk("	  pProSLICMLT->capFaults.measTR          =  %d (nF/10)\n" ,		 pProSLICMLT->capFaults.measTR	);/* mtk08176_20150824 */
					  printk("	  pProSLICMLT->capFaults.measTG           =  %d(nF/10)\n" ,		 pProSLICMLT->capFaults.measTG	);
					  printk("	  pProSLICMLT->capFaults.measRG           =  %d(nF/10)\n" ,       pProSLICMLT->capFaults.measRG	);				

				}
				break;
				
				case 7:
				{	
					/* 
					**
					** Example REN capacitance Test
					**
					*/
					/* Process Available Results */
					/*
					** Available Results:
					**
					** pProSLICMLT->capFaults.measTR		CTR (nF)
					**
					*/
					printk("	REN Capacitive Test\n");
					printk("	pProSLICMLT->capFaults.measTR		  =  %d\n" ,	   pProSLICMLT->capFaults.measTR  );
					printk("	pProSLICMLT->capFaults.measTR2		  =  %d\n" ,	   pProSLICMLT->capFaults.measTR2  );			

				}
				break;
                                /* mtk08176_20150824 */			
				case 8:
					{	
					/* 
					**
					** In Line Monitor
					**
					**
					*/
					printk ("vtr = %d mv\n",inline_monitor->vtr);
					printk ("vtip = %d mv\n",inline_monitor->vtip );
					printk ("vring = %d mv\n",inline_monitor->vring);
					printk ("VBAT = %d mv\n",inline_monitor->vbat );
					printk ("itr = %d uA\n",inline_monitor->itr );
					printk ("itip = %d uA\n",inline_monitor->itip );
					printk ("iring = %d uA\n",inline_monitor->iring);
					printk ("ilong = %d uA\n",inline_monitor->ilong);
				}
				break;	
				
				default:
				{
					printk("NO such testItem\n");				
				}
					break;
				}

			#endif
		}
		else
		{
			printk("Line test Fails or No such ID\n");
		}
	
		return count;
}

/* ADD by mtk08176_20150508 , support line test. */
int SLIC_linetest(_TDI_infcLineTestParams_t*  lineTest){

	_TDI_devicelinetest_t plinetest;
	int* linetestresult=NULL;
	int i=0;
	
	#ifndef SLIC_SILICON
	_TDI_infcLineTestSizeList_t LineTestSizeList[] =
	{
	   	{LT_TID_LINE_V,				sizeof(LtLineVResultType)},
		{LT_TID_ROH,					sizeof(LtRohResultType)},
		{LT_TID_RES_FLT,				sizeof(LtResFltResultType)},
		{LT_TID_RINGERS,			sizeof(LtRingersResultType)},
		{LT_TID_DC_FEED_ST,			sizeof(LtDcFeedSTResultType)},
		{LT_TID_DC_VOLTAGE,			sizeof(LtDcVoltageResultType)},
		{LT_TID_RINGING_ST,			sizeof(LtRingingSTResultType)},
		{LT_TID_ON_OFF_HOOK_ST,	sizeof(LtOnOffHookSTResultType)},
		{LT_TID_RD_BAT_COND,		sizeof(LtReadBatCondResultType)},
		{LT_TID_PRE_LINE_V,			sizeof(LtPreLineVResultType)},
		{LT_TID_CAP,					sizeof(LtCapResultType)},
		{LT_TID_RD_LOOP_COND,		sizeof(LtRdLoopCondResultType)},
		{0, 0},
	};
	#endif
	plinetest.chan=lineTest->infc;
	plinetest.testid=lineTest->lineTestId;

	linetestresult=SLIC_DeviceLineTest(&plinetest);

	if(linetestresult!=NULL)
	{
		#ifndef SLIC_SILICON
		for(i=0;LineTestSizeList[i].size != 0;i++){
			if(LineTestSizeList[i].testID == lineTest->lineTestId){
				memcpy(lineTest->lineTestData, linetestresult, LineTestSizeList[i].size);
				break;	
			}
		}
		#else
		/* mtk08176_20150824 */
		if(plinetest.testid != 8){
			(ProSLICMLTType*)linetestresult;
			memcpy(lineTest->lineTestData, linetestresult, sizeof(ProSLICMLTType));
		}
		else{
			(proslicMonitorType*)linetestresult;
			memcpy(lineTest->lineTestData, linetestresult, sizeof(proslicMonitorType));
		}
		#endif
		}
	else
	{
		printk("Line test Fails or No such ID\n");
	}
	
	return 0;
}

#endif
//#endif

//dyma_20140428 metering
int SLIC_fxsSetMetering(_TDI_fxsSetMetering_t* pMeteringSetting){

	return slicFunc[pMeteringSetting->chan][FXS_SET_METERING]((char *)pMeteringSetting);

}

typedef int (*voip_api_op_t)(struct ECNT_VOIP_Data *voip_data);

int SLIC_Power_Shutdown(struct ECNT_VOIP_Data *voip_data)
{
	uint32 temp_val = 0;
#ifdef SLIC_SILICON  
	int slicRegAddress = 0xBFB00834;
#else
	int slicCmd = 0x04;
	int cmdlen = 1;
#endif

#ifdef SLIC_SILICON  
	temp_val = regRead32(slicRegAddress);
	regWrite32(slicRegAddress,0x0000001|temp_val );/*bfb00834 bit[0] for reset ZSI/ISI 1 & spi/pcm mode slic */
#else
	/*shutdown zarlink slic*/
	temp_val = 0x04;
	VpMpiCmd(0,0x1,0x04,1, &temp_val);
#endif
	return 0;
}

int ZARLINK_PowerSaveMode(int enable)
{
	int i,slic_chanel_num;
	_TDI_lineInfo_t tmpLineInfo;
	
	slic_chanel_num = g_deviceInfo.fxsNum + g_deviceInfo.fxoNum;
	for(i = 0;i<slic_chanel_num;i++) /*upgrade_to_4_channel*/
	{
		if(enable)
			tmpLineInfo.state = SLIC_LF_STAND_BY;
		else
			tmpLineInfo.state = SLIC_LF_FORWARD_ACTIVE;

		tmpLineInfo.chan = (unsigned char)i;
		tmpLineInfo.polarity = SLIC_LF_POL_FORWARD;
		SLIC_LFStatusCfg(1,&tmpLineInfo);
	}

	return 0;
}

int SLIC_Power_Save_Mode(struct ECNT_VOIP_Data *voip_data)
{
	int i,slic_chanel_num;
	_TDI_lineInfo_t tmpLineInfo;
	
#ifdef SLIC_SILICON  
	slic_chanel_num = g_deviceInfo.fxsNum + g_deviceInfo.fxoNum;
	for(i = 0;i<slic_chanel_num;i++) /*upgrade_to_4_channel*/
	{
 		slicFunc[i][POWERSAVE_HW](voip_data->enable);
	}
#else
	ZARLINK_PowerSaveMode(voip_data->enable);
#endif
	return 0;
}

voip_api_op_t voip_operation[]=
{
	SLIC_Power_Shutdown,
	SLIC_Power_Save_Mode,
};

int SLIC_power_operation(struct ECNT_VOIP_Data *voip_data)
{
	int retVal = 0;
	if(NULL != voip_operation[voip_data->function_id])
		voip_data->retValue = voip_operation[voip_data->function_id](voip_data) ;
	
	return retVal;
}
#endif



int slic3_func_init(SLIC_FUNC_TBL* pSlicFuncTab)
{
	pSlicFuncTab->SLIC_SEP_HookStatusGet = SLIC_HOOKStatusGet;
	pSlicFuncTab->SLIC_SEP_AudioGainSet = SLIC_AudioGainSetup;  //dyma_131216
	pSlicFuncTab->SLIC_SEP_LFStatusCfg = SLIC_LFStatusCfg;
	pSlicFuncTab->SLIC_SEP_RingIntervalSetup = SLIC_RingIntervalSetup;

#ifdef OSBN_VOIP	
	pSlicFuncTab->SLIC_SEP_getDeviceInfo = SLIC_getDeviceInfo;
#else
	pSlicFuncTab->SLIC_SEP_TotalChannelGet = SLIC_TotalChannelGet;
#endif

	pSlicFuncTab->SLIC_SEP_Dest = SLIC_Dest;
	pSlicFuncTab->SLIC_SEP_SlicReset = SLIC_Reset;
	pSlicFuncTab->SLIC_SEP_ConfQuery = SLIC_ConfQuery;
	pSlicFuncTab->SLIC_SEP_Init = SLIC_Init;
	pSlicFuncTab->SLIC_SEP_DeviceVerify = SLIC_DeviceVerify;
	pSlicFuncTab->SLIC_SEP_RINGImpSetup = SLIC_RINGImpSetup;
	pSlicFuncTab->SLIC_SEP_RingProfileSetup = SLIC_RingProfileSetup;

#ifdef OSBN_VOIP
	pSlicFuncTab->SLIC_SEP_FxoGetRingCadence = SLIC_FxoGetRingCadence;
	pSlicFuncTab->SLIC_SEP_FxoLineStatusGet = SLIC_FxoLineStatusGet;
	pSlicFuncTab->SLIC_SEP_FxoHookStatusSet = SLIC_FxoHookStatusSet;
	pSlicFuncTab->SLIC_SEP_FxoPSTNStatusGet = SLIC_FxoPSTNStatusGet;
	pSlicFuncTab->SLIC_SEP_register_read_proc = slic_register_read_proc;
	pSlicFuncTab->SLIC_SEP_register_write_proc = slic_register_write_proc;

	pSlicFuncTab->SLIC_SEP_DumpRegister_Cat = SLIC_DumpRegister_Cat;
	pSlicFuncTab->SLIC_SEP_DumpRegister_Echo = SLIC_DumpRegister_Echo;
//#ifdef SLIC_SILICON	
#if 0
	pSlicFuncTab->SLIC_SEP_linetest_cat = NULL;
	pSlicFuncTab->SLIC_SEP_linetest_echo = NULL;
	pSlicFuncTab->SLIC_SEP_lineteststatus = NULL;

#else
	pSlicFuncTab->SLIC_SEP_linetest_cat = SLIC_linetest_cat;
	pSlicFuncTab->SLIC_SEP_linetest_echo = SLIC_linetest_echo;
	pSlicFuncTab->SLIC_SEP_lineteststatus = SLIC_lineteststatus;
	pSlicFuncTab->SLIC_SEP_linetest = SLIC_linetest;	//mtk08176_20150508
#endif 
	pSlicFuncTab->SLIC_SEP_fxsSetMetering = SLIC_fxsSetMetering;  //dyma_20140428 metering
	pSlicFuncTab->SLIC_SEP_powerOperation = SLIC_power_operation;
#endif 
	pSlicFuncTab->SLIC_SEP_EventNotify_Register = SLIC_EventNotify_Register;
	pSlicFuncTab->SLIC_SEP_RegRead = SLIC_RegRead;
	pSlicFuncTab->SLIC_SEP_RegWrite = SLIC_RegWrite;	
	pSlicFuncTab->SLIC_SEP_IndregRead = SLIC_IndregRead;
	pSlicFuncTab->SLIC_SEP_IndregWrite = SLIC_IndregWrite;
	pSlicFuncTab->SLIC_SEP_IRQENABLE = SLIC_IRQENABLE;
	pSlicFuncTab->SLIC_SEP_ringState = SLIC_ringState;
	pSlicFuncTab->SLIC_SEP_ISR = SLIC_ISR;
	pSlicFuncTab->SLIC_SEP_FxoLIUStatusGet= SLIC_FxoLIUStatusGet;
	pSlicFuncTab->SLIC_SEP_LoopbackEnable = SLIC_LoopbackEnable;
	pSlicFuncTab->SLIC_SEP_RingParams = SLIC_ringParams;	//mtk07059_20150423
	pSlicFuncTab->SLIC_SEP_DcFeedParams = SLIC_dcFeedParams;	//mtk07059_20150423
	pSlicFuncTab->SLIC_SEP_SpiReset = SLIC_SPI_Reset;
	return 0;
}

#endif

int slicFunc_dummy(char* buf)
{
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_WARNING, "dummy: this slicFunc is not supported\n");
	return _TDI_FAIL;
}

void slic3_func_uninit(SLIC_FUNC_TBL* pSlicFuncTab)
{
	memset(pSlicFuncTab, 0, sizeof(*pSlicFuncTab));
}
#ifndef TCSUPPORT_SLIC_SEPARATE
#ifdef __KERNEL__
#ifdef OSBN_VOIP
EXPORT_SYMBOL(SLIC_DeviceVerify);
#endif
EXPORT_SYMBOL(SLIC_Init);
EXPORT_SYMBOL(SLIC_LFStatusCfg);
EXPORT_SYMBOL(SLIC_ISR);
EXPORT_SYMBOL(SLIC_IRQENABLE);
EXPORT_SYMBOL(SLIC_HOOKStatusGet);
#ifdef OSBN_VOIP
EXPORT_SYMBOL(SLIC_getDeviceInfo);
#else
EXPORT_SYMBOL(SLIC_TotalChannelGet);
#endif
EXPORT_SYMBOL(SLIC_EventNotify_Register);
EXPORT_SYMBOL(SLIC_RING);
EXPORT_SYMBOL(SLIC_ringState);
EXPORT_SYMBOL(SLIC_RegRead);
EXPORT_SYMBOL(SLIC_RegWrite);
EXPORT_SYMBOL(SLIC_IndregRead);
EXPORT_SYMBOL(SLIC_IndregWrite);
#ifndef OSBN_VOIP
EXPORT_SYMBOL(SLIC_AudioVolumeSetup);
#endif
EXPORT_SYMBOL(SLIC_RINGImpSetup);
EXPORT_SYMBOL(SLIC_AudioGainSetup);
EXPORT_SYMBOL(SLIC_Dest);
EXPORT_SYMBOL(SLIC_ConfQuery);
EXPORT_SYMBOL(SLIC_RingIntervalSetup);
EXPORT_SYMBOL(SLIC_RingProfileSetup);
#ifdef OSBN_VOIP
EXPORT_SYMBOL(SLIC_FxoGetRingCadence);
EXPORT_SYMBOL(SLIC_FxoHookStatusSet);
EXPORT_SYMBOL(SLIC_FxoLineStatusGet);
EXPORT_SYMBOL(SLIC_FxoPSTNStatusGet);
EXPORT_SYMBOL(slic_register_read_proc);  //dyma_130306
EXPORT_SYMBOL(slic_register_write_proc); //dyma_130306
EXPORT_SYMBOL(SLIC_DumpRegister_Cat);    //dyma_130306
EXPORT_SYMBOL(SLIC_DumpRegister_Echo);   //dyma_130306
#ifndef SLIC_SILICON
EXPORT_SYMBOL(SLIC_FxoLIUStatusGet);	//add by mtk69097
#endif
EXPORT_SYMBOL(SLIC_linetest_cat);    //Roger_lt
EXPORT_SYMBOL(SLIC_linetest_echo);   //Roger_lt
EXPORT_SYMBOL(SLIC_lineteststatus);//Roger_lt
EXPORT_SYMBOL(SLIC_linetest);   //mtk08176_20150508


#endif
#endif
#endif
EXPORT_SYMBOL(slicFunc);

#ifdef TCSUPPORT_SLIC_SEPARATE
EXPORT_SYMBOL(si32260Func);
EXPORT_SYMBOL(si32176Func);
EXPORT_SYMBOL(si3218xFunc);
EXPORT_SYMBOL(si3228xFunc);
EXPORT_SYMBOL(le89116Func);
EXPORT_SYMBOL(le89156Func);
EXPORT_SYMBOL(le89116_1Func);
EXPORT_SYMBOL(le89156_2Func);
EXPORT_SYMBOL(le89316Func);
EXPORT_SYMBOL(zl88601Func); //dyma_130118
EXPORT_SYMBOL(zl88601_1Func); 
EXPORT_SYMBOL(le9662Func); 
EXPORT_SYMBOL(le9662_1Func);
EXPORT_SYMBOL(le9641Func); //dyma_150112
EXPORT_SYMBOL(le9641_1Func); 
#else
#ifdef SLIC_SILICON
EXPORT_SYMBOL(fxs_intr_cnt);  //dyma_131227
EXPORT_SYMBOL(si32260Func);
EXPORT_SYMBOL(si32176Func);
EXPORT_SYMBOL(si3218xFunc);
EXPORT_SYMBOL(si3228xFunc);
#else
EXPORT_SYMBOL(le89116Func);
EXPORT_SYMBOL(le89156Func);
EXPORT_SYMBOL(le89116_1Func);
EXPORT_SYMBOL(le89156_2Func);
EXPORT_SYMBOL(le89316Func);
EXPORT_SYMBOL(zl88601Func); //dyma_130118
EXPORT_SYMBOL(zl88601_1Func); 
EXPORT_SYMBOL(le9662Func); 
EXPORT_SYMBOL(le9662_1Func);
EXPORT_SYMBOL(le9641Func); //dyma_150112
EXPORT_SYMBOL(le9641_1Func);
#endif //#ifdef SLIC_SILICON
#endif //#ifdef TCSUPPORT_SLIC_SEPARATE
EXPORT_SYMBOL(_slic_dbg_type_mask);  //dyma_130316

EXPORT_SYMBOL(SLIC_Reset);
EXPORT_SYMBOL(SLIC_SPI_Reset);


