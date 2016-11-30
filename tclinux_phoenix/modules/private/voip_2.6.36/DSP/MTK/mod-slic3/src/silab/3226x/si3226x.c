/*
** Si3226x.c
** Si32260 function table interface for fxs
*/
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
#include <linux/timer.h>
#include <linux/proc_fs.h>
#include <linux/slab.h> 
#include "_TDI.h"
#include "TDI.h"
#include "proslic.h"
#include "si3226x.h"
#include "si3226x_intf.h"
#include "si3226x_registers.h"
#include "si3226x_CUK_constants.h"
#include "ddr_slic3.h"
#include <linux/spinlock.h>
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,31)  //dyma_131230
#include <linux/semaphore.h>
#else
#include <asm/semaphore.h>
#endif
#include <linux/jiffies.h>     //dyma_140619

extern int fxs_intr_cnt; //dyma_131227
static int write_LINEFEED_intr_cnt; //dyma_131227

DEFINE_SPINLOCK(silabs_spi_if_lock_si3226x); //dyma_131230
int silabs_spi_if_flags_si3226x; //dyma_131230

typedef struct slicPreset_s{
	int ringCountryIdx;
	int impCountryIdx;
}slicPresetParam_t;

typedef struct chanStatus chanState;
struct chanStatus {
    proslicChanType_ptr ProObj;
};

typedef enum {
	SILAB_CHAN_TYPE_UNKNOWN = 0,
	SILAB_CHAN_TYPE_FXS = 1,
	SILAB_CHAN_TYPE_FXO = 2
} siChanType_e;

typedef enum {
	SILAB_CHIP_TYPE_UNKNOWN = 0,
	SILAB_CHIP_TYPE_SI32260 = 1,
	SILAB_CHIP_TYPE_SI32261 = 2,
	SILAB_CHIP_TYPE_SI32266 = 3,
	SILAB_CHIP_TYPE_SI32267 = 4
} siChipType_e;

typedef struct chanPvt_s {
	siChanType_e chanType;
	siChipType_e chipType;
} chanPvt_t;

#ifdef SI3226X
#define CHAN_PER_DEVICE     2

#else
#error "Codec need to be defined in make/system-$(PLATFORM) VTSP_HW_OPTIONS"
#endif

extern int SPI_bytes_write(int type,int interface_type,int id, unsigned char ctrl, unsigned char cmd, unsigned char *data_ptr, unsigned char cmdLen);
extern int SPI_bytes_read(int type,int interface_type,int id, unsigned char ctrl, unsigned char cmd, unsigned char *data_ptr, unsigned char cmdLen);
extern unsigned int spi_interface_type;  /*0 = normal PCM+SPI interface; 1 = ZSI interface (Microsemi) ;2=ISI interface(silicon lab)*/

/* FXS on device 0*/
//#define FXS_LINE    0
#define SI32260_CHAN_NUM 2	/* 2 FXS */
#define SI32260_FXS_NUM	2
#define SI32260_FXO_NUM	0

#define SILAB_LINE_STATE_OPEN           (0)
#define SILAB_LINE_STATE_FORWARD_ACTIVE (1)
#define SILAB_LINE_STATE_FORWARE_OHT    (2)
#define SILAB_LINE_STATE_RING           (4)
#define SILAB_LINE_STATE_REVERSE_ACTIVE (5)
#define SILAB_LINE_STATE_REVERSE_OHT    (6)
/*
 * Define channel numbers
 * TIC API allows TIC module to define these constants as desired.
 * Define constants as bitfield for the chip select SPI byte.
 */
#define SILAB_CHAN_A                    (0x00)
#define SILAB_CHAN_B                    (0x01)
#define SILAB_CHAN_C                    (0x02)
#define SILAB_CHAN_BROADCAST            (0xFF)

/*
 * FXS Register 11 (PCMMODE)  PCM_FMT[1:0]
 */
#define SILAB_PCM_ALAW                  (0x0)
#define SILAB_PCM_ULAW                  (0x1)
#define SILAB_PCM_LINEARMODE            (0x3)

/*
 * FXS Register 13 (PCMTXHI)  TX_EDGE[4] 0:positive 1:negative
 */
#define SILAB_PCM_TX_EDGE               (1 << 4)

static int  si32260NumProslic = 0;
static int  si32260NumChannel = 0;
static int  si32260deviceId = 0;  //dyma_131216 evaluate default value

/* Define ProSLIC control interface object */
static controlInterfaceType *ProHWIntf = NULL;

/* Define array of ProSLIC device objects */
static ProslicDeviceType **ProSLICDevices = NULL;

/* Define array of ProSLIC channel object pointers */
static proslicChanType_ptr *arrayOfProslicChans;

static chanPvt_t chanPvt[SI32260_CHAN_NUM];
static chanState *ports = NULL;
static slicPresetParam_t slicPreset;
static uint8 si32260_DevPhyLocation=0xff;
atomic_t si3226x_slic_init_flag = ATOMIC_INIT(0);


static ProslicRegInit regTable[] = {
    {11, 0x13}, /* PCMMODE */
    {38, 0x00}, /* RINGCON */
    {39, 0x40}, /* RINGTALO */
    {40, 0x1F}, /* RINGTAHI */
    {41, 0x00}, /* RINGTILO */
    {42, 0x7D}, /* RINGTIHI */
    {22, 0xFF}, /* IRQEN1 */
    {23, 0xFF}, /* IRQEN2 */
    {24, 0xFF}, /* IRQEN3 */
    {0xFF, 0xFF}
};

slicFuncTbl si32260Func[] = {
	si32260_deviceVerify,
	si32260_deviceInit,
	si32260_shutdownHw,
	si32260_paramReset,
	si32260_phoneRingStart,
	si32260_phoneRingStop,
	si32260_eventProcess,
	si32260_fxsSetLineState,
	si32260_fxsGetLineState,
	si32260_fxsSetBatteryPol,
	si32260_fxsGetBatteryPol,
	si32260_fxsGetHookState,
	si32260_ringState,
	si32260_fxsGetPulseStatus,
	si32260_fxsSetGainLv,
	slicFunc_dummy,
	slicFunc_dummy,
	slicFunc_dummy,
	slicFunc_dummy,
	slicFunc_dummy,
	slicFunc_dummy,
	si32260_dumpregister,
	slicFunc_dummy,
	si32260_LtTestStart,
	si32260_getLtTestResult,//Roger_silicon_lt
	slicFunc_dummy,//dyma_20140428 metering
	si32260_SetLoopback,//FXS_SET_LOOPBACK
	si32260_SetRingParam,
	si32260_SetDcFeedParam,	/* added by mtk08176_20150627  */
	si32260_PowerSaveMode,
};

static TIC_Return silab_alloc(int numTicFxs);
static void silab_free(void);

static uInt8 silab_ctrl_ReadRegisterWrapper (void * hSpiGci, uInt8 channel,uInt8 regAddr); 	
static int silab_ctrl_WriteRegisterWrapper (void * hSpiGci, uInt8 channel, uInt8 regAddr, uInt8 data); 
static int silab_ctrl_ResetWrapper (void * hSpiGci, int status);
static int silab_ctrl_WriteRAMWrapper (void * hSpiGci, uInt8 channel, uInt16 ramAddr, ramData data);
static uInt32 silab_ctrl_ReadRAMWrapper (void * hSpiGci, uInt8 channel, uInt16 ramAddr);
static int time_DelayWrapper (void *hTimer, int timeInMs);
static int time_TimeElapsedWrapper (void *hTimer, void *startTime, int *timeInMs);
static int time_GetTimeWrapper (void *hTimer, void *time);

static void silab_writeSi32260DirectSpi(vint channel, uint8 regAddr, uint8 writeData);
static uint8 silab_readSi32260DirectSpi(vint  channel, uint8 regAddr);
static void silab_writeSi32260IndirectSpi(vint channel, uint16 iAddr, uint32 data32);
static uint32 silab_readSi32260IndirectSpi(vint channel, uint16 iAddr);

static uint8 silab_bitReverse(uint8 fiveBits);
static void silab_dumpReg_si3226x(uint8 chan);

int si32260_deviceVerify(char* buf){
	deviceInfo_t* devInfo = (deviceInfo_t*)buf;
	uint8	retValue = 0;
	uint8	part_num = 0;
	uint8	reversion = 0;
	int 	channel;
	uint8   f_id=0;
	static  int silab_reset_check_cnt=0;  
	
	channel = devInfo->deviceNum;
	#ifdef SLIC_ISI //dyma_20140909
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "\r\n[ SI32266 ] ");
	#else
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "\r\n[ SI32260 ] ");
	#endif

	if(si32260_DevPhyLocation==0xff)
	{
		si32260_DevPhyLocation = devInfo->deviceNum;
	}

	if(si32260_DevPhyLocation!=devInfo->deviceNum)
	{
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_WARNING, "Bypass si32260_deviceVerify on device %d\n", channel);
		return _TDI_FAIL;
	}

	SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "DeviceVerify ProSLIC_Version %s channel:%d\r\n",ProSLIC_Version(),channel);

	/* Check whether Silabs SLIC reset complete */ //dyma_20140909  
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "Silabs SLIC Reset ");   
	silab_reset_check_cnt=0;
    while (0x1F!=(silab_ctrl_ReadRegisterWrapper(NULL,channel, 3)&0x1F)){
		silab_reset_check_cnt++;
		if((silab_reset_check_cnt%100)==0)
		{
			printk(".");
		}
		if(silab_reset_check_cnt>200)
		{
			printk(" Fail\r\n");
			return _TDI_FAIL;
		}
		msleep(10);
    }
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, " Complete\r\n");
	
	retValue = silab_ctrl_ReadRegisterWrapper (NULL,channel, 0);
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "REG_0  = 0x%x\n", retValue);
	f_id=((retValue>>6)&0x03);
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "f_id = 0x%x (si3226x should be 0x3)\n", f_id);
	reversion=(retValue&0x07);
	part_num =((retValue >> 3) & 0x07);
	retValue = silab_ctrl_ReadRegisterWrapper (NULL,channel, 47);
	//SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "REG_47 = 0x%x\n", retValue);
	part_num = ((retValue&0x80)>>4)|part_num;
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "Channel %d, Part_num 0x%x, Revision 0x%x\n", channel,part_num,reversion);
	
	#ifdef SLIC_ISI  //dyma_20140909
	if ( 0x8 != part_num) {
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, "ERROR! Channel %d Part number 0x%x\n", channel, part_num);
		return _TDI_FAIL;
	}

	#else
	if ( 0x0 != part_num) {
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, "ERROR! Channel %d Part number 0x%x\n", channel, part_num);
		return _TDI_FAIL;
	}
	
	#endif
	
	silab_ctrl_WriteRegisterWrapper (NULL,channel, 14, 0x40);
	retValue = silab_ctrl_ReadRegisterWrapper (NULL,channel, 14);
	if(retValue!=0x40)
	{
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, "AN381_4.2.1.2.1 Channel %d Verification Fail !\n",channel);
		return _TDI_FAIL;
	}

	SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "si32260_deviceVerify %d Success!\r\n\r\n",si32260_DevPhyLocation);

	devInfo->deviceNum += 1;
	devInfo->fxsNum += SI32260_FXS_NUM;
	devInfo->fxoNum += SI32260_FXO_NUM;
	devInfo->slicType = SILICON_32260;
	devInfo->pcm1ChNum += SI32260_FXS_NUM+SI32260_FXO_NUM;
	devInfo->pcm2ChNum += 0;

	return _TDI_SUCCESS;
}

int si32260_deviceInit(char* buf)
{
	int spiGciObj;  /* used to fit parameter requirement, not used in wrap function */
	int timerObj;

	_TDI_initSetting_t *pinitSetting = (_TDI_initSetting_t *)buf;
	int 	i, index, channel;
	int 	slot;
	uint8	retValue = 0;
	uint8	data;
	int numTicFxs = SI32260_CHAN_NUM;
	int init_ret=0;
	unsigned char devCnt=0;

	if(pinitSetting == NULL) {
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, "ERROR! si32260_deviceInit pinitSetting NULL\n");
		return _TDI_FAIL;
	}
	
    devCnt = pinitSetting->chan;
	if(devCnt!=si32260_DevPhyLocation){
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_WARNING, "Bypass si32260_deviceInit on Device %d!\r\n\r\n",devCnt);  //dyma_130118
		return _TDI_SUCCESS;
	}

	si32260deviceId = pinitSetting->chan;	
	memset(chanPvt, 0, sizeof(chanPvt_t)*(SI32260_CHAN_NUM));
	/*
	* Allocate memory for ProSLIC API
	*/
	if ((retValue = silab_alloc(numTicFxs) != TIC_OK)) {
		return _TDI_FAIL;
	}
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "\r\nsi32260NumProslic = %d\n", si32260NumProslic);
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "si32260NumChannel = %d\n", si32260NumChannel);
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "si32260deviceId = %d\n", si32260deviceId);

	/*
	** Step 1: (optional)
	** Initialize users control interface and timer objects C this
	** may already be done, if not, do it here
	*/
	if(NULL == ProHWIntf)
	{
		/*
		** Step 2: (required)
		** Create ProSLIC Control Interface Object
		*/
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "ProSLIC_createControlInterface = %d\n", ProSLIC_createControlInterface(&ProHWIntf));

		/*
		** Step 3: (required)
		** Create ProSLIC Device Objects
		*/
		for (index = 0; index < si32260NumProslic; index++) 
		{
			SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "ProSLIC_createDevice = %d\n", ProSLIC_createDevice (&(ProSLICDevices[index])));
		}

		/*
		** Step 4: (required)
		** Create and initialize ProSLIC channel objects
		** Also initialize array pointers to users proslic channel object
		** members to simplify initialization process.
		*/
		for (index= 0; index < si32260NumChannel; index++) {
			SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "ProSLIC_createChannel = %d\n", ProSLIC_createChannel(&(ports[index].ProObj)));
			SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "ProSLIC_SWInitChan = %d\n", ProSLIC_SWInitChan (ports[index].ProObj, index ,SI3226X_TYPE, ProSLICDevices[index/CHAN_PER_DEVICE], ProHWIntf));
			ProSLIC_setSWDebugMode(ports[index].ProObj, 1);
			arrayOfProslicChans[index] = ports[index].ProObj;
			chanPvt[index].chanType = SILAB_CHAN_TYPE_FXS;
			#ifdef SLIC_ISI 
			chanPvt[index].chipType = SILAB_CHIP_TYPE_SI32266;
			#else
			chanPvt[index].chipType = SILAB_CHIP_TYPE_SI32260;
			#endif
			SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "chanPvt[%d].chanType %d\n",index, chanPvt[index].chanType);
			SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "chanPvt[%d].chipType %d\n",index, chanPvt[index].chipType);
		}

		/*
		** Step 5: (required)
		** Establish linkage between host objects/functions and
		** ProSLIC API
		*/
		ProSLIC_setControlInterfaceCtrlObj (ProHWIntf, &spiGciObj);
		ProSLIC_setControlInterfaceReset (ProHWIntf, silab_ctrl_ResetWrapper);
		ProSLIC_setControlInterfaceWriteRegister (ProHWIntf, silab_ctrl_WriteRegisterWrapper);
		ProSLIC_setControlInterfaceReadRegister (ProHWIntf, silab_ctrl_ReadRegisterWrapper);
		ProSLIC_setControlInterfaceWriteRAM (ProHWIntf, silab_ctrl_WriteRAMWrapper);
		ProSLIC_setControlInterfaceReadRAM (ProHWIntf, silab_ctrl_ReadRAMWrapper);
		ProSLIC_setControlInterfaceTimerObj (ProHWIntf, &timerObj);
		ProSLIC_setControlInterfaceDelay (ProHWIntf, time_DelayWrapper);
		ProSLIC_setControlInterfaceTimeElapsed (ProHWIntf, time_TimeElapsedWrapper);
		ProSLIC_setControlInterfaceGetTime (ProHWIntf, time_GetTimeWrapper);
		
	}
	/*
	** Step 6: (system dependent)
	** Assert hardware Reset C ensure VDD, PCLK, and FSYNC are present and stable
	** before releasing reset
	*/
	ProSLIC_Reset(ports[0].ProObj);
	
	/*
	** Step 7: (required)
	** Initialize device (loading of general parameters, calibrations,
	** dc-dc powerup, etc.)
	*/
	if (ProSLIC_Init(arrayOfProslicChans,si32260NumChannel)) {
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, "%s %d initialization in Silabs Si3226x failed \n",__FILE__,__LINE__);
	}

	/*
	** Step 8: (design dependent)
	** Execute longitudinal balance calibration
	** or reload coefficients from factory LB cal
	**
	** Note: all batteries should be up and stable prior to
	** executing the lb cal
	*/
	ProSLIC_LBCal(arrayOfProslicChans,si32260NumChannel);

	/*
	** Step 9: (design dependent)
	** Load custom configuration presets(generated using
	** ProSLIC API Config Tool)
	*/
	index = 0;
	slot = si32260deviceId * 16;

	si32260_paramReset(buf);

	for(index = 0; index < si32260NumChannel; index++)
	{
		ProSLIC_DCFeedSetup(ports[index].ProObj, 0);

		ProSLIC_RingSetup(ports[index].ProObj, slicPreset.ringCountryIdx);
		ProSLIC_ZsynthSetup(ports[index].ProObj, slicPreset.impCountryIdx);
		ProSLIC_ToneGenSetup(ports[index].ProObj, slicPreset.ringCountryIdx);

		ProSLIC_LoadRegTables(ports[index].ProObj, NULL, regTable, 1);  // si32260NumChannel);
		ProSLIC_PCMSetup(ports[index].ProObj, 0);
		ProSLIC_PCMTimeSlotSetup(ports[index].ProObj, slot, slot);
		ProSLIC_PCMStart(ports[index].ProObj);
		ProSLIC_SetLinefeedStatus(ports[index].ProObj, LF_FWD_ACTIVE);

		/* Set PCM linear 16-bit mode */
		data = silab_ctrl_ReadRegisterWrapper (NULL,index, PCMMODE);
		data = (data & ~(0x3)) | SILAB_PCM_LINEARMODE;	 /* PCM_FMT[1:0] */
		silab_ctrl_WriteRegisterWrapper (NULL,index, PCMMODE, data);

		/* Set DTX data to be driven on positive edge of PCLK */
		data = silab_ctrl_ReadRegisterWrapper (NULL,index, PCMTXHI);
		silab_ctrl_WriteRegisterWrapper (NULL,index, PCMTXHI, (data & ~SILAB_PCM_TX_EDGE));

		/* Adjust Tx/Rx gain level to attenuate -3dB */
		ProSLIC_AudioGainSetup(ports[index].ProObj,pinitSetting->listenSlicVal,pinitSetting->speakSlicVal,slicPreset.impCountryIdx);  //dyma_131212
		//silab_writeSi32260IndirectSpi(index, 544, 0x8000000);  /* TX AGC Gain */ // unit gain = 0x8000000   //dyma_131212
		//silab_writeSi32260IndirectSpi(index, 545, 0x1009B9C);  /* RX AGC Gain */ // unit gain = 0x2000000   //dyma_131212

		/*set loopback test mode*/
		#if 0
		ProSLIC_SetMuteStatus(ports[index].ProObj,PROSLIC_MUTE_ALL);
		ProSLIC_SetLoopbackMode(ports[index].ProObj,PROSLIC_LOOPBACK_DIG); 
		#endif
		slot+=16;//xflu add for channel 1 no data 20131022
	}
	atomic_set(&si3226x_slic_init_flag,1);
	/*
	** END OF TYPICAL INITIALIZATION
	*/
	return _TDI_SUCCESS;
}

int si32260_paramReset(char* buf)
{
	_TDI_initSetting_t *pinitSetting = (_TDI_initSetting_t *)buf;

	switch(pinitSetting->hwProfId)
	{
		case C_DEF:
			slicPreset.ringCountryIdx = 0; 
			slicPreset.impCountryIdx = 0; 
			break;

		default:
			slicPreset.ringCountryIdx = 0; 
			slicPreset.impCountryIdx = 0;
			break;
	}
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "___ COUNTRY SETTING ___\r\nring_idx : %d\r\n imp_idx : %d\r\n\r\n",slicPreset.ringCountryIdx,slicPreset.impCountryIdx);
	return _TDI_SUCCESS;
}

int si32260_phoneRingStart(char* buf)
{
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_WARNING, "Nothing to do");
	return _TDI_SUCCESS;
}

int si32260_phoneRingStop(char* buf)
{
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_WARNING, "Nothing to do");
	return _TDI_SUCCESS;
}

int si32260_eventProcess(char* buf)
{
	return _TDI_SUCCESS;
}

int si32260_LtTestStart(char* buf)
{

	uint8 i;
	uint8 backup_register[PDN_STAT+1];
	uint8 testresult=0;
	_TDI_devicelinetest_t* linetest=(_TDI_devicelinetest_t*)buf;

	//return if slic is at init state
	if(atomic_read(&si3226x_slic_init_flag)==0)
		return _TDI_FAIL;

	if(linetest->chan<si32260NumChannel)
	{

		//backup slic register before line test
		for(i = ID; i <= PDN_STAT; i++) {
		backup_register[i] = silab_ctrl_ReadRegisterWrapper(NULL,linetest->chan, i);	
		}
		
	 	//excute line test function 
		testresult=Silicon_LineTestStart(linetest,arrayOfProslicChans[linetest->chan]);

		//restore slic register after line test
		for(i = ID; i <= PDN_STAT; i++) {
		silab_ctrl_WriteRegisterWrapper(NULL,linetest->chan, i, backup_register[i]); 
		}
	}
	else
		return _TDI_FAIL;

	return testresult;

}

int* si32260_getLtTestResult(void)
{	
	return Silicon_getLtTestResult();

}

int si32260_dumpregister(char* buf)
{
	char chanCnt = *buf;
	silab_dumpReg_si3226x(chanCnt);
	return _TDI_SUCCESS;
}

int si32260_shutdownHw(char* buf)
{
	silab_free();
	return _TDI_SUCCESS;
}

int si32260_fxsSetLineState(char* buf)
{
	_TDI_lineInfo_t* plineInfo = (_TDI_lineInfo_t*)buf;
	uint8 stateType = 0;
	//static uint8 pre_state[2]={0,0};
	
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG,"_%d_si32260_fxsSetLine[%d]state 0x%x ",
		fxs_intr_cnt, //dyma_131227
		plineInfo->chan,
		plineInfo->state);
	
	if(chanPvt[plineInfo->chan].chanType != SILAB_CHAN_TYPE_FXS) {
		return _TDI_SUCCESS;
	}

    switch (plineInfo->state) {

        case TIC_STATE_POWER_DOWN:
            stateType = SILAB_LINE_STATE_OPEN;// LF_OPEN
            break;
            
        case TIC_STATE_ACTIVE:
        case TIC_STATE_ACTIVE_MONITOR:			
			stateType = SILAB_LINE_STATE_FORWARD_ACTIVE;// LF_FWD_ACTIVE
            break;
			
		case TIC_STATE_REV_ACTIVE: //dyma_20140508
			stateType = SILAB_LINE_STATE_REVERSE_ACTIVE;
            break;		
			
        case TIC_STATE_RING_PAUSE:
		case TIC_STATE_OHT://xflu 20131107
			stateType = SILAB_LINE_STATE_FORWARE_OHT;// LF_FWD_OHT
            break;
            
        case TIC_STATE_RINGING:
            stateType = SILAB_LINE_STATE_RING;// LF_RINGING
            break;
#ifdef SLIC_SILICON //MTK69036 REV_OHT
	case TIC_STATE_REV_OHT:
            stateType = SILAB_LINE_STATE_REVERSE_OHT;// LF_REV_OHT
            break;
#endif
        default:
            SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG,"si32260_State UNKNOWN !\n");
			//pre_state[plineInfo->chan]=0xff;
            return _TDI_FAIL;
    }
		
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG,"convert to Type %d\n",stateType);
	//pre_state[plineInfo->chan]=plineInfo->state;

    ProSLIC_SetLinefeedStatus(ports[plineInfo->chan].ProObj, stateType);	 

	return _TDI_SUCCESS;
}

int si32260_fxsGetLineState(char* buf)
{
	vint temp;
	_TDI_lineInfo_t* plineInfo = (_TDI_lineInfo_t*)buf;
	static int pre_stateType[2]={0,0};

//	printk("%s \n",__func__);
	if (atomic_read(&si3226x_slic_init_flag) ==0)
		return _TDI_FAIL;

	if(chanPvt[plineInfo->chan].chanType != SILAB_CHAN_TYPE_FXS) {
		plineInfo->retTicStat = TIC_STATE_ACTIVE;
		return _TDI_SUCCESS;
	}

    temp = silab_ctrl_ReadRegisterWrapper(NULL,plineInfo->chan, 30) & 0x07;
    if (SILAB_LINE_STATE_RING == temp) {
		plineInfo->retTicStat = TIC_STATE_RINGING;
    }
    else if (SILAB_LINE_STATE_OPEN == temp) {
		plineInfo->retTicStat = TIC_STATE_POWER_DOWN;
    }
    else if (SILAB_LINE_STATE_FORWARE_OHT == temp) {   
		plineInfo->retTicStat = TIC_STATE_OHT;//TIC_STATE_RING_PAUSE;//xflu 20131107
    }
    else if (SILAB_LINE_STATE_REVERSE_OHT == temp) {   
		plineInfo->retTicStat = TIC_STATE_REV_OHT; // TIC_STATE_RING_PAUSE; //dyma_20140508 
    }
	else if (SILAB_LINE_STATE_REVERSE_ACTIVE== temp) {   //dyma_20140508 
		plineInfo->retTicStat = TIC_STATE_REV_ACTIVE;
    }
    else {
		plineInfo->retTicStat = TIC_STATE_ACTIVE;
    }
//	 printk("%s : retTicStat=%d \n",__func__,plineInfo->retTicStat);
	if(plineInfo->retTicStat!=pre_stateType[plineInfo->chan]){
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG,"_%d_si32260_fxsGetLine(%d)Ret[%d]=State[0x%x]\n",
		fxs_intr_cnt, //dyma_131227
		plineInfo->chan,
		temp,
		plineInfo->retTicStat);
		pre_stateType[plineInfo->chan]=plineInfo->retTicStat;
	}

	return _TDI_SUCCESS;
}

int si32260_fxsGetBatteryPol(char* buf)
{
	vint polarity;
	_TDI_batteryPolInfo_t* pBatteryPolInfo = (_TDI_batteryPolInfo_t* )buf;

	if(chanPvt[pBatteryPolInfo->chan].chanType != SILAB_CHAN_TYPE_FXS) {
		pBatteryPolInfo->retTicStat = TIC_BATT_POL_FORWARD;
		return _TDI_SUCCESS;
	}

	pBatteryPolInfo->retTicStat = TIC_BATT_POL_FORWARD;

    /*
     * Bit 6 is the polarity bit, 0 is positive 1 is negative.
     */
    polarity = (silab_ctrl_ReadRegisterWrapper(NULL,pBatteryPolInfo->chan, 30) & 0x03);
    if ((0x5 == polarity) || (0x6 == polarity)) {
		pBatteryPolInfo->retTicStat = TIC_BATT_POL_REVERSE;
    }

	return _TDI_SUCCESS;
}

int si32260_fxsSetBatteryPol(char* buf)
{
	_TDI_batteryPolInfo_t* pBatteryPolInfo = (_TDI_batteryPolInfo_t* )buf;

	if(chanPvt[pBatteryPolInfo->chan].chanType != SILAB_CHAN_TYPE_FXS) {
		return _TDI_SUCCESS;
	}

	if (TIC_BATT_POL_REVERSE == pBatteryPolInfo->polarity) {
        /*
         * Set LINEFEED CONTROL to "Reverse active". This will
         * allow a Japan CID to be output.
         */
        silab_ctrl_WriteRegisterWrapper (NULL,pBatteryPolInfo->chan, LINEFEED, SILAB_LINE_STATE_REVERSE_ACTIVE);
    }
    else {
        /*
         * Set LINEFEED CONTROL to "Forward active". In this state, no on-hook
         * messages can be sent.
         */
        silab_ctrl_WriteRegisterWrapper (NULL,pBatteryPolInfo->chan, LINEFEED, SILAB_LINE_STATE_FORWARD_ACTIVE);
    }

	return _TDI_SUCCESS;
}

int si32260_fxsGetHookState(char* buf)
{
	uint8 hook;
	_TDI_hookStat_t* pHookStat = (_TDI_hookStat_t*)buf;
	static uint8 pre_hook[2]={0,0};

   /*
     * Or ring trip and hook flag status to get hook status.
     */
     if(ports != NULL && atomic_read(&si3226x_slic_init_flag) ==1)
    	ProSLIC_ReadHookStatus(ports[pHookStat->chan].ProObj, &hook);
	 else
		return _TDI_FAIL;
		
	if(pre_hook[pHookStat->chan]!=hook)
	{
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG,"_%d_si32260_fxsGetChan [%d] HookState [%d]\n",fxs_intr_cnt,pHookStat->chan,hook);  //dyma_131227
		pre_hook[pHookStat->chan]=hook;
	}
    pHookStat->hookStat = ((hook == ONHOOK) ? TIC_IMMEDIATE_ONHOOK : TIC_IMMEDIATE_OFFHOOK);

	return _TDI_SUCCESS;
}

int si32260_ringState(char* buf)
{
	int state;
    int count;
    int ret;
	_TDI_ringSetting_t *pRingSetting = (_TDI_ringSetting_t *) buf;
	static int pre_state[2]={0,0};
	static unsigned long pre_jiffies;  //dyma_140619

    state = pRingSetting->state;
    count = pRingSetting->count;
    ret   = _TDI_FAIL;

	if(pre_state[pRingSetting->chan]!=state)
	{
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG,"si32260_ringState [%d] count[%d]\n",state,count); 
		pre_state[pRingSetting->chan]=state;
	}

    /*
     * Return immediately if done.
     */
    if (state == _TDI_RING_STATE_DONE) {
        return (ret);
    }
    
SILAB_RING_STATE_RUN:

    /* 
     * State machine
     */
    switch (state) {

        /*
         * Init state. When new ring starts. Always transitions to make1 state.
         */
        case _TDI_RING_STATE_INIT:
            count = pRingSetting->make1s;
            state = _TDI_RING_STATE_MAK1;
            ret   = _TDI_SUCCESS;
			pre_jiffies=jiffies;   //dyma_140619
			
			SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG,"|chan=%d,state=%d,count=%d\n|make=%d	%d	%d\n|break=%d	%d	%d\n|rep=%d,numRing=%d\n",
			pRingSetting->chan,
			pRingSetting->state,
			pRingSetting->count,
			pRingSetting->make1s,
			pRingSetting->make2s,
			pRingSetting->make3s,
			pRingSetting->break1s,
			pRingSetting->break2s,
			pRingSetting->break3s,
			pRingSetting->rep,
			pRingSetting->numRings);

        /*
         * Make state always transitions to break state.
         */
        case _TDI_RING_STATE_MAK1:
            /*
             * If time is negative, generate forever
             */
        	//dbg_printf("%s---%d %d",__func__,state,pRingSetting->make1s);
            if (pRingSetting->make1s < 0) {
                count = _TDI_RING_INFINITY;
                break;
            }

            /*
             * Not finished with this state.
             */
            if (count > 0) {
			//	ret   = _TDI_SUCCESS;//xflu 20131107
                break;
            }

            /*
             * Finsihed with this state, go to next state.
             */
            count = pRingSetting->break1s;
            state = _TDI_RING_STATE_BRK1;
            ret   = _TDI_SUCCESS ;

        /*
         * Break state always transitions to make state
         */
        case _TDI_RING_STATE_BRK1:
            /*
             * If time is negative, generate forever
             */
            if (pRingSetting->break1s < 0) {
                count = _TDI_RING_INFINITY;
                break;
            }

            /*
             * Not finished with this state.
             */
            if (count > 0) {
		//		ret   = _TDI_SUCCESS ;//xflu 20131107
                break;
            }

            /*
             * Finsihed with this state, go to next state.
             */
            count = pRingSetting->make2s;
            state = _TDI_RING_STATE_MAK2;
            ret   = _TDI_SUCCESS;

        /*
         * Make state always transitions to break state.
         */
        case _TDI_RING_STATE_MAK2:
            /*
             * If time is negative, generate forever
             */
            if (pRingSetting->make2s < 0) {
                count = _TDI_RING_INFINITY;
                break;
            }

            /*
             * Not finished with this state.
             */
            if (count > 0) {
		//		ret   = _TDI_SUCCESS ;//xflu 20131107
                break;
            }

            /*
             * Finsihed with this state, go to next state.
             */
            count = pRingSetting->break2s;
            state = _TDI_RING_STATE_BRK2;
            ret   = _TDI_SUCCESS ;

        /*
         * Break state always transitions to make state
         */
        case _TDI_RING_STATE_BRK2:
            /*
             * If time is negative, generate forever
             */
            if (pRingSetting->break2s < 0) {
                count = _TDI_RING_INFINITY;
                break;
            }

            /*
             * Not finished with this state.
             */
            if (count > 0) {
		//		ret   = _TDI_SUCCESS ;//xflu 20131107
                break;
            }

            /*
             * Finsihed with this state, go to next state.
             */
            count = pRingSetting->make3s;
            state = _TDI_RING_STATE_MAK3;
            ret   = _TDI_SUCCESS ;

        /*
         * Make state always transitions to break state.
         */
        case _TDI_RING_STATE_MAK3:
            /*
             * If time is negative, generate forever
             */
            if (pRingSetting->make3s < 0) {
                count = _TDI_RING_INFINITY;
                break;
            }

            /*
             * Not finished with this state.
             */
            if (count > 0) {
		//		ret   = _TDI_SUCCESS ;//xflu 20131107
                break;
            }

            /*
             * Finsihed with this state, go to next state.
             */
            count = pRingSetting->break3s;
            state = _TDI_RING_STATE_BRK3;
            ret   = _TDI_SUCCESS ;

        /*
         * Break state always transitions to make state
         */
        case _TDI_RING_STATE_BRK3:
            /*
             * If time is negative, generate forever
             */
            if (pRingSetting->break3s < 0) {
                count = _TDI_RING_INFINITY;
                break;
            }

            /*
             * Not finished with this state.
             */
            if (count > 0) {
			//	ret   = _TDI_SUCCESS ;//xflu 20131107
                break;
            }

            /*
             * Finsihed with this state, go to next state.
             */
            pRingSetting->numRings++;

            /*
             * Check number of repeats.
             *  <  0 means infinite repeats.
             *  >  0 means repeats not done.
             *  == 0 means repeats done.
             */
            count = pRingSetting->make1s;
            state = _TDI_RING_STATE_MAK1;
            ret   = _TDI_SUCCESS ;
            if (pRingSetting->rep < 0) {
                /*
                 * Run state machine again.
                 */
                goto SILAB_RING_STATE_RUN;
            }
            else if (pRingSetting->rep > 0) {
                /*
                 * Run state machine again.
                 */
                pRingSetting->rep--;
                goto SILAB_RING_STATE_RUN;
            }

        default:
            count = _TDI_RING_INFINITY;
            state = _TDI_RING_STATE_DONE;
            ret   = _TDI_SUCCESS ;
    }
   
    count=count-((int)(jiffies-pre_jiffies)>0?((int)(jiffies-pre_jiffies)):1); //dyma_140619
	pre_jiffies=jiffies;   //dyma_140619
    pRingSetting->state = state;
    pRingSetting->count = count;

    return (ret);
}

int si32260_fxsGetPulseStatus(char* buf)
{
	dbg_printf("%s", __func__);
	return _TDI_SUCCESS;
}

int si32260_fxsSetGainLv(char* buf)
{
	_TDI_gainSetting_t* gainSetting = (_TDI_gainSetting_t *)buf;

	ProSLIC_AudioGainSetup(ports[gainSetting->chan].ProObj,gainSetting->listenSlicVal,gainSetting->speakSlicVal,slicPreset.impCountryIdx);

	dbg_printf("%s - %x / %x \n",__func__,gainSetting->speakSlicVal,gainSetting->listenSlicVal);
	return _TDI_SUCCESS;
}

int si32260_SetLoopback(char *buf)
{
	_TDI_fxsSetLoopback_t* loopbackSetting=(_TDI_fxsSetLoopback_t*)buf;
	ProslicLoopbackModes loopBack;

	if(atomic_read(&si3226x_slic_init_flag)==0)
		return _TDI_FAIL;
	if(loopbackSetting->chan >= SI32260_CHAN_NUM)
		return _TDI_FAIL;

	SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, " %s loopback\n", loopbackSetting->enable?"Enable":"Disable");

	if (loopbackSetting->enable)
		loopBack = PROSLIC_LOOPBACK_DIG;
	else
		loopBack = PROSLIC_LOOPBACK_NONE;

	if (loopBack == PROSLIC_LOOPBACK_DIG) {
		ProSLIC_SetMuteStatus(ports[loopbackSetting->chan].ProObj,PROSLIC_MUTE_ALL);
		ProSLIC_SetLoopbackMode(ports[loopbackSetting->chan].ProObj,loopBack); 
	}else if(loopBack == PROSLIC_LOOPBACK_NONE){
		ProSLIC_SetMuteStatus(ports[loopbackSetting->chan].ProObj,PROSLIC_MUTE_NONE);
		ProSLIC_SetLoopbackMode(ports[loopbackSetting->chan].ProObj,loopBack); 
	}

	return _TDI_SUCCESS;

}



/* added by mtk08176_20150627  */
int si32260_SetRingParam(char *buf)
{
	return _TDI_SUCCESS;

}

int si32260_SetDcFeedParam(char *buf)
{
	return _TDI_SUCCESS;	
}

static TIC_Return silab_alloc(int numTicFxs)
{
    /*
     * Limition for Si32177+32178 daisy chained combination :
     * The unique FXO must be the last interface.
     * So both number_of_proslic & number_of_channel equal to number_of_fxs.
     */
    /*
		numTicFxs = 2
		CHAN_PER_DEVICE = 2 (support 2 fxs chaanel on si32260)
		si32260NumProslic = 1 (1 slic)
		si32260NumChannel = 2 (2 fxs)
		(ProslicDeviceType **) ProSLICDevices : 1 
		(chanState *) ports : 2
		(proslicChanType_ptr *) arrayOfProslicChans: 2
	 */
    si32260NumProslic = numTicFxs/CHAN_PER_DEVICE;
    si32260NumChannel = numTicFxs;

    /* Allocate required resources (arrays) */
	ProSLICDevices = (ProslicDeviceType **) kmalloc(sizeof(ProslicDeviceType *)*si32260NumProslic, GFP_KERNEL);
    if (NULL == ProSLICDevices) {
        return (TIC_ERROR_NOMEM);
    }

	ports = (chanState *) kmalloc(sizeof(chanState)*si32260NumChannel, GFP_KERNEL);
    if (NULL == ports) {
		kfree(ProSLICDevices);
        return (TIC_ERROR_NOMEM);
    }
	
	arrayOfProslicChans = (proslicChanType_ptr *)kmalloc(sizeof(proslicChanType_ptr)*si32260NumChannel, GFP_KERNEL);
    if (NULL == arrayOfProslicChans) {
		kfree(ports);
		kfree(ProSLICDevices);
        return (TIC_ERROR_NOMEM);
    }

    return (TIC_OK);
}

static void silab_free(
        void)
{
	
	atomic_set(&si3226x_slic_init_flag,0);	

    if (arrayOfProslicChans != NULL) {
		kfree(arrayOfProslicChans);
		arrayOfProslicChans = NULL;
    }

    if (ports != NULL) {
		kfree(ports);
		ports = NULL;
    }

    if (ProSLICDevices != NULL) {
		kfree(ProSLICDevices);
		ProSLICDevices = NULL;
    }

	if (ProHWIntf != NULL) {
		kfree(ProHWIntf);
		ProHWIntf = NULL;
    }
}

/*
 * Function: spiGci_ResetWrapper
 *
 * Description:
 * Sets the reset pin of the ProSLIC
 */
static int silab_ctrl_ResetWrapper (void * hSpiGci, int status)
{
#if 0
    SLIC_reset(RESET);
    mdelay(5);
    SLIC_reset(ENABLE);
    mdelay(5);
#endif
    return 0;
}

/*
 * Function: spiGci_WriteRegisterWrapper
 *
 * Description:
 * Writes a single ProSLIC register
 *
 * Input Parameters:
 * channel: ProSLIC channel to write to
 * address: Address of register to write
 * data: data to write to register
 *
 * Return:
 * none
 */
static int silab_ctrl_WriteRegisterWrapper (void * hSpiGci, uInt8 channel,  uInt8 regAddr, uInt8 data)
{
	//down(&SPI_SEM);  //dyma_131230
	spin_lock_irqsave(&silabs_spi_if_lock_si3226x, silabs_spi_if_flags_si3226x);  //dyma_131230
    	silab_writeSi32260DirectSpi(channel, regAddr, data);
	spin_unlock_irqrestore(&silabs_spi_if_lock_si3226x, silabs_spi_if_flags_si3226x);  //dyma_131230
	//up(&SPI_SEM);
    return 0;
}

/*
 * SPI/GCI register read
 *
 * Description:
 * Reads a single ProSLIC register
 *
 * Input Parameters:
 * channel: ProSLIC channel to read from
 * num: number of reads to perform
 * regAddr: Address of register to read
 * addr_inc: whether to increment address after each read
 * data: data to read from register
 *
 * Return:
 * none
 */
static uInt8 silab_ctrl_ReadRegisterWrapper (void * hSpiGci, uInt8 channel,uInt8 regAddr)
{
	uInt8 ret;
	//down(&SPI_SEM); //dyma_131230
	spin_lock_irqsave(&silabs_spi_if_lock_si3226x, silabs_spi_if_flags_si3226x);  //dyma_131230
	ret=silab_readSi32260DirectSpi(channel, regAddr);
	spin_unlock_irqrestore(&silabs_spi_if_lock_si3226x, silabs_spi_if_flags_si3226x);  //dyma_131230
	//up(&SPI_SEM);
    return ret;
}

/*
 * Function: SPI_WriteRAMWrapper
 *
 * Description:
 * Writes a single ProSLIC RAM location
 *
 * Input Parameters:
 * channel: ProSLIC channel to write to
 * address: Address of RAM location to write
 * data: data to write to RAM location
 *
 * Return:
 * none
 */
static int silab_ctrl_WriteRAMWrapper (void * hSpiGci, uInt8 channel, uInt16 ramAddr, ramData data)
{
	//down(&SPI_SEM); //dyma_131230
	spin_lock_irqsave(&silabs_spi_if_lock_si3226x, silabs_spi_if_flags_si3226x);  //dyma_131230
    	silab_writeSi32260IndirectSpi(channel, ramAddr, data);
	spin_unlock_irqrestore(&silabs_spi_if_lock_si3226x, silabs_spi_if_flags_si3226x);  //dyma_131230
	//up(&SPI_SEM); 
    return 0;
}

/*
 * Function: SPI_ReadRAMWrapper
 *
 * Description:
 * Reads a single ProSLIC RAM location
 *
 * Input Parameters:
 * channel: ProSLIC channel to read from
 * address: Address of RAM location to read
 * pData: data to read from RAM location
 *
 * Return:
 * none
 */
static uInt32 silab_ctrl_ReadRAMWrapper (void * hSpiGci, uInt8 channel, uInt16 ramAddr)
{
	uInt32 ret;
	//down(&SPI_SEM); //dyma_131230
	spin_lock_irqsave(&silabs_spi_if_lock_si3226x, silabs_spi_if_flags_si3226x);  //dyma_131230
	ret=silab_readSi32260IndirectSpi(channel, ramAddr);
	spin_unlock_irqrestore(&silabs_spi_if_lock_si3226x, silabs_spi_if_flags_si3226x);  //dyma_131230
	//up(&SPI_SEM);

    return ret;
}

/*
 * Function: SYSTEM_Delay
*/
int time_DelayWrapper (void *hTimer, int timeInMs)
{

    mdelay(timeInMs);
    return 0;
}

/*
 * Function: SYSTEM_TimeElapsed
 */
int time_TimeElapsedWrapper (void *hTimer, void *startTime, int *timeInMs)
{
    /* TODO */
    *timeInMs = 1000;
    return 0;
}

/* 
 * Function: SYSTEM_GetTime
 */
int time_GetTimeWrapper (void *hTimer, void *time)
{
    /* TODO:
     * Need to convert the ticks from OSAL_archCountGet() to ms
     */
    //return OSAL_archCountGet();
    return 0;
}


/*
 * ======== silab_writeSi32260DirectSpi() ========
 * Write a byte on SPI to Si32260 direct register
 *
 * Input Value: Channel Num.
                Address to write to
		1 byte of data to write
 * Return Value: none
 */
static void silab_writeSi32260DirectSpi(
    vint   channel,
    uint8  regAddr,
    uint8  writeData)
{
    uint8 ctrl;
	vint deviceId;
	
	deviceId =  si32260deviceId + _TDI_SLIC_SPI_ID; 
    /*
     * Configure the bit7 in the control byte to 1 if channel is in the broadcast mode.
     * The CID[4:0] need to be configured to 1, too.
     */
    if (SILAB_CHAN_BROADCAST == channel) {
        ctrl = 0xBF;
    }
    else {
        /* 
         * Send Control byte, bit 6 set to 0 for writing
         */
        ctrl = (0x20 | silab_bitReverse(channel));
    }
	#if 0
	if(regAddr==LINEFEED){    //dyma_131227
		//if((fxs_intr_cnt-write_LINEFEED_intr_cnt)<20)
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG," write LINEFEED interval = %d\r\n\r\n",
			fxs_intr_cnt-write_LINEFEED_intr_cnt //dyma_131227
			);
		write_LINEFEED_intr_cnt = fxs_intr_cnt;
	}
	#endif
	SPI_bytes_write(SLIC_TYPE_SILICON,spi_interface_type,deviceId, ctrl, regAddr, &writeData, 1);

}

/*
 * ======== silab_readSi32260DirectSpi() ========
 * Receive a byte on SPI from Si32260 direct register
 *
 * Input Values: Channel Num.
                 Address of register to read
 * Return Value: 1 byte of data received.
 */
static uint8 silab_readSi32260DirectSpi(
    vint  channel,
    uint8 regAddr)
{
    uint8  retVal;
    uint8  ctrl;

	vint deviceId;
//	printk("si32260deviceId = %d\n", si32260deviceId);
	deviceId =  si32260deviceId + _TDI_SLIC_SPI_ID; 
    /* 
     * Send Control byte, bit 6 set to 1 for reading
     */
    ctrl = (0x60 | silab_bitReverse(channel));
//	printk("deviceId = %d\n", deviceId);
//	printk("ctrl = %d\n", ctrl);
//	printk("regAddr = %d\n", regAddr);
//	printk("retVal = %d\n", retVal);
	SPI_bytes_read(SLIC_TYPE_SILICON,spi_interface_type,deviceId, ctrl, regAddr, &retVal, 1);
    return (retVal);
}

/*
 * ======== silab_writeSi32260IndirectSpi() ========
 * Write a byte on SPI to Si32260 RAM
 *
 * Input Value: Channel Num.
                Address to write to
				1 byte of data to write
 * Return Value: none
 */
static void silab_writeSi32260IndirectSpi(
    vint    channel,
    uint16  iAddr,
    uint32  data32)
{
    volatile uint8 data;
    volatile uint8 status;
    volatile uint8 addrHi;
    volatile uint8 addrLo;

    addrHi = (iAddr >> 3) & 0xE0;
    addrLo = iAddr & 0xFF;

    /* Wait for RAM status register to be ready. */
    while (0 != (status = (silab_readSi32260DirectSpi(channel, 4) & 0x01) ));

    /* Write the high part of address */
    silab_writeSi32260DirectSpi(channel, 5, addrHi);

    /* Write data to ram */
    data = (uint8)((data32 << 3) & 0xFF);
    silab_writeSi32260DirectSpi(channel, 6, data);

    data = (uint8)((data32 >> 5) & 0xFF);
    silab_writeSi32260DirectSpi(channel, 7, data);

    data = (uint8)((data32 >> 13) & 0xFF);
    silab_writeSi32260DirectSpi(channel, 8, data);

    data = (uint8)((data32 >> 21) & 0xFF);
    silab_writeSi32260DirectSpi(channel, 9, data);

    /* Write the low part of address */
    silab_writeSi32260DirectSpi(channel, 10, addrLo);

    /* Wait for RAM status register to be ready. */
    while (0 != (status = (silab_readSi32260DirectSpi(channel, 4) & 0x01) ));

}

/*
 * ======== silab_readSi32260IndirectSpi() ========
 * Read a byte on SPI from Si32260 RAM
 *
 * Input Value: Channel Num.
                Address to write to
				1 byte of data to write
 * Return Value: RAM data 
 */
static uint32 silab_readSi32260IndirectSpi(
    vint   channel,
    uint16 iAddr)
{
    volatile uint8 status;
    volatile uint8 addrHi;
    volatile uint8 addrLo;
    uint32   data;
    uint32   retVal;

    addrHi = (iAddr >> 3) & 0xE0;
    addrLo = iAddr & 0xFF;

    /* Wait for RAM status register to be ready. */
    while(0 != (status = (silab_readSi32260DirectSpi(channel, 4) & 0x01) ));

     /* Write the high part of address */
    silab_writeSi32260DirectSpi(channel, 5, addrHi);

    /* Write the low part of address */
    silab_writeSi32260DirectSpi(channel, 10, addrLo);
    
    /*
     *  Above actions initiates the READ transation, cause the RAM_STAT
     *  bit to set.
     *  Wait for RAM status register to be ready.
     */
    while(0 != (status = (silab_readSi32260DirectSpi(channel, 4) & 0x01) ));

    /* Read bit 4 ~ 0 */
    retVal = (silab_readSi32260DirectSpi(channel, 6) >> 3) & 0xFF;

    /* Read bit 12 ~ 5 */
    data = (uint32)silab_readSi32260DirectSpi(channel, 7);
    retVal = retVal | (data << 5);
        
    /* Read bit 20 ~ 13 */
    data = (uint32)silab_readSi32260DirectSpi(channel, 8);
    retVal = retVal | (data << 13);
        
    /* Read bit 28 ~ 21 */
    data = (uint32)silab_readSi32260DirectSpi(channel, 9);
    retVal = retVal | (data << 21);
        
    /*
     * Return the word read
     */
    return (retVal);

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

static void silab_dumpReg_si3226x(uint8 chan)
{
	uint8 value;
	uint8 i;

	printk("Dump SLIC[%d] register:\n", chan);
	printk("=================================\n");
	printk("    00 01 02 03 04 05 06 07 08 09\n");
	printk("=================================\n");
	for(i = ID; i <= PDN_STAT; i++) {
		value = silab_readSi32260DirectSpi(chan, i);
		if((i%10) == 0) {
			printk("%02d:", i);
		}
		printk(" %02x", value);
		if((i%10) == 9) {
			printk("\n");
		}
	}
	if((i%10) != 0) {
		printk("\n");
	}
	printk("=================================\n");
	return;
}

int si32260_PowerSaveMode(int enable)
{
	return _TDI_SUCCESS;
}

