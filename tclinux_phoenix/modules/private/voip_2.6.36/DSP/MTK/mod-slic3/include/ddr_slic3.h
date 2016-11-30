/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/
/**
 *      @file   ddr_slic3.h
 *      @brief  SLIC driver declaraction file
 *
 *      @author mtk02588
 *      @date   04/06/2010
 *
 **/
/*==========================================================================
 *
 *      Library Inclusion Segment
 *
 *==========================================================================
 */

#ifndef __DDR_SLIC3_H__
#define __DDR_SLIC3_H__


#include "_TDI.h"
#include "dsp_constant.h"   /*upgrade_to_4_channel*/
#include "eva_log.h"

/* ========== SLIC debug msg can by open/close by cmd under /proc/fxs/ =========== */

#define _SLIC_PRINT(fmt, var ...)	do {printk("<SLIC>"fmt, ##var);}while(0)
#define _SLIC_1_PRINT(fmt, var ...)	do {printk("<SLIC_1>"fmt, ##var);}while(0)

/* declare in fxs_proc.c */
extern unsigned int _slic_dbg_type_mask;  //dyma_131106
extern unsigned int spi_interface_type ;  /*default 0 = normal PCM+SPI interface; 1 = ZSI interface (Microsemi) ;2=ISI interface(silicon lab)*/


/* ========== SLIC debug msg can by open/close by cmd under /proc/fxs/ =========== */

#define NORMAL_PCM_SPI_INTERFACE 0
#define MICROMSEMI_ZSI_INTERFACE 1
#define SILICON_ISI_INTERFACE 2
#define LANTIQ_CSI_INTERFACE 3
#define SPI_WRAPPER 4




#define SLIC_TYPE_ZARLINK 0
#define SLIC_TYPE_SILICON 1
#define SLIC_TYPE_LANTIQ  2

/*
 * typedef enum    e_phone_event
{
	PHONE_EVENT_MIN = 0,
	 -----------------------------------------------
	PHONE_EVENT_ONHOOK = 1,
	PHONE_EVENT_OFFHOOK = 2,
	PHONE_EVENT_DIGIT = 3,
	PHONE_EVENT_RING_TA = 4,
	PHONE_EVENT_RING_TI = 5,
	PHONE_EVENT_FSKBUF_AVAIL = 6,
	PHONE_EVENT_OSC1_TA = 7,
	 -----------------------------------------------
	PHONE_EVENT_MAX,
} E_PHONE_EVENT;
*/

/* Line State */
typedef enum {
	LINE_DOWN,
	LINE_ACTIVE_FWD,
	LINE_ACTIVE_REV,
	LINE_RING, 
	LINE_RING_PAUSE,
	LINE_BUSY, 
	LINE_SLEEP, 
	LINE_ERROR,
	LINE_LIU,
	LINE_LNIU,
	LINE_OHT_REV//MTK69036 REV_OHT
} slicLineState_e;


typedef enum
{	
	SLIC_LF_OPEN = 0,
	SLIC_LF_FORWARD_ACTIVE, // TIC_STATE_ACTIVE,TIC_STATE_RING_PAUSE
	SLIC_LF_FORWARD_OHT,// TIC_STATE_ACTIVE
	SLIC_LF_TIP_OPEN,
	SLIC_LF_RINGING, //TIC_STATE_RINGING
	SLIC_LF_REV_ACTIVE,//VP_LINE_TALK_POLREV
	SLIC_LF_REV_OHT, //VP_LINE_TALK_POLREV
	SLIC_LF_RING_OPEN,
	SLIC_LF_RING_PAUSE,
	SLIC_LF_DOWN,
	SLIC_LF_STAND_BY,//add by mtk69090 for COC   #OSBNB00030711
}E_SLIC_LINEFEED;

/* added by mtk07059 lin chen */
typedef enum
{
	SLIC_LF_POL_FORWARD = 0,
	SLIC_LF_POL_REVERSE
}E_SLIC_LINEFEED_POL;

typedef enum
{	
	SLIC_LAYOUT_NONE = 0,
	SLIC_LAYOUT_BB,
	SLIC_LAYOUT_FB
}E_SLIC_LAYOUT_TYPE;

typedef enum
{
	SLIC_RING_PROFILE_DEFAULT=0,
	SLIC_RING_PROFILE_AUSTRALIA,
	SLIC_MAX_RING_PROFILE,						/* NOT TO USE : Maximum count of profile*/
} slic_ring_profile_t;
typedef enum {
	DEVICE_VERIFY,
	DEVICE_INIT,
	SHUTDOWN_HW,
	PARAM_RESET,
	FXS_PHONE_RING_START,
	FXS_PHONE_RING_STOP,
	FXS_EVENT_PROCESS,
	FXS_SET_LINE_STAT,
	FXS_GET_LINE_STAT,
	FXS_SET_BATTERY_POL,
	FXS_GET_BATTERY_POL,
	FXS_GET_HOOK_STAT,
	FXS_RING_STATE,
	FXS_GET_PULSE_STAT,
	FXS_SET_GAIN_LV,
	FXO_GET_RING_CAD,
	FXO_GET_HOOK_STAT,
	FXO_SET_HOOK_STAT,
	FXO_GET_LINE_STAT,
	FXO_GET_PSTN_STAT,
	FXO_SET_MONITOR_MODE,
	DUMP_REGISTER,
	FXO_GET_LIU_STAT,
	FXS_LT_START,//Roger_lt
	FXS_LT_RESULT_GET,//Roger_lt
	FXS_SET_METERING,  //dyma_20140428 metering
	FXS_SET_LOOPBACK,
	FXS_SET_RINGPARAMS,	//mtk07059_20150423
	FXS_SET_DCFEEDPARAMS,	//mtk07059_20150616
	POWERSAVE_HW,
}_TIC_ETDIFunc;

int slicFunc_dummy(char* buf);

#ifdef SLIC_SILICON  //dyma_131212
typedef struct gainTbl_s{
	char dbVal;
	short slicVal;
}gainTbl_t;

#define SI_SET_QUEUE_SIZE  20  //dyma_131230 

#ifdef SI3226X
#undef SI_SET_QUEUE_SIZE
#define SI_SET_QUEUE_SIZE  20  //dyma_131230 //silabs SLIC linefeed register can't be set continusly, at least 100ms buffer must be inserted
#endif
#ifdef SI3217X
#undef SI_SET_QUEUE_SIZE
#define SI_SET_QUEUE_SIZE  10  //dyma_131230 //silabs SLIC linefeed register can't be set continusly, at least 100ms buffer must be inserted
#endif

typedef struct fxsSetLineStateQueue_s{
	_TDI_lineInfo_t lineInfoQueue[SI_SET_QUEUE_SIZE];
	short w_idx;
	short r_idx;
	short count;
	short show_cnt;
}fxsSetLineStateQueue_t; //dyma_131230

typedef enum{
    SLIC_TIMER_CMP      = 0,
    SLIC_TIMER_START    = 1,
    SLIC_TIMER_COUNTING = 2
}SLIC_TIMER_STATUS;  //dyma_131230

#else
typedef struct gainTbl_s{
	char dbVal;
	uint16 slicVal;
}gainTbl_t;
#endif

int SLIC_Reset(void);
int SLIC_SPI_Reset(void);
extern int SPI_bytes_write(int slic_type,int interface_type,int id, unsigned char ctrl, unsigned char cmd, unsigned char *data_ptr, unsigned char cmdLen);
extern int SPI_bytes_read(int slic_type,int interface_type,int id, unsigned char ctrl, unsigned char cmd, unsigned char *data_ptr, unsigned char cmdLen);

#ifdef TCSUPPORT_SLIC_SEPARATE
#ifdef OSBN_VOIP
int SLIC_SEP_getDeviceInfo(int selection);
#else
int SLIC_SEP_TotalChannelGet(void);
#endif

#else
#ifdef OSBN_VOIP
int SLIC_DeviceVerify(void);
#endif

/**
 *  @brief init SLIC module & hardware
 *  @param type: type of SLIC chip: 3215 ; 3226 ;32176 ; 32176
 *  @param layout_type: only valid for 32176 chip: SLIC_LAYOUT_BB (for buck boost layput) ; SLIC_LAYOUT_FB (for fly back layout)
 *  @return value > 0 :number of SLIC channel probed , value <0 : initial fail
 */
int SLIC_Init(_TDI_initSetting_t* pinitSetting);
/**
 *  @brief SLIC  interrupt serivce routing
 *  @return none
 */
int SLIC_ISR(int chan);

/**
 *  @brief Enable SLIC IRQ
 *  @param enable: 0 is disable , 1 is enable
 *  @return none
 */
int SLIC_IRQENABLE(int en);

/**
 *  @brief Get hook status of channel
 *  @param cid: channel ID 
 *  @param status: variable to store hook status
 *  @return none
 */
int SLIC_HOOKStatusGet(_TDI_hookStat_t* pHookStat);

/**
 *  @brief Get total channel of SLIC
 *  @return none
 */
#ifdef OSBN_VOIP
int SLIC_getDeviceInfo(int selection);
#else
int SLIC_TotalChannelGet(void);
#endif

/**
 *  @brief Register call back function for event notify
 *  @param data:  call back function pointer
 *  @return none
 */
int SLIC_EventNotify_Register(void *data);

/**
 *  @brief Make phone ring 
 *  @param cid: channel ID 
 *  @param en:  0 : stop ringing , 1 start to ring
 *  @return none
 */
int SLIC_RING(int cid, int en);

/**
 *  @brief Make phone ring
 *  @param cid: channel ID
 *  @param en:  0 : stop ringing , 1 start to ring
 *  @return none
 */
int SLIC_ringState(_TDI_ringSetting_t *pRingSetting);

/**
 *  @brief Read SLIC register 
 *  @param cid: channel ID 
 *  @param reg: register number
  * @param value: variable to store read data
 *  @return none
 */
int SLIC_RegRead(int cid, unsigned int reg, unsigned int *value);

/**
 *  @brief Write SLIC register 
 *  @param cid: channel ID 
 *  @param reg: register number
  * @param data: data to write into register
 *  @return none
 */
int SLIC_RegWrite(int cid, unsigned int reg, unsigned int data);

/**
 *  @brief Read SLIC RAM register 
 *  @param cid: channel ID 
 *  @param reg: RAM register number
  * @param value: variable to store read data
 *  @return none
 */
int SLIC_IndregRead(int cid, unsigned int reg, unsigned int *value);

/**
 *  @brief Write SLIC RAM register 
 *  @param cid: channel ID 
 *  @param reg: RAM register number
  * @param data: data to write into RAM register
 *  @return none
 */
int SLIC_IndregWrite(int cid, unsigned int reg, unsigned int data);

/**
 *  @brief Get and cofigure linefeed status of channel
 *  @param cid: channel ID 
 *  @param isSet: Specify if to confige linefeed status
 *  @param cfgStatus_p: linefeed status after configured
 *  @param currStatus_p: linefeed status before configured
 *  @return none
 */
int SLIC_LFStatusCfg(unsigned int isSet,_TDI_lineInfo_t* plineInfo);


/**
 *  @brief Set up impedance of SLIC
 *  @param cid: channel ID 
 *  @param imp: impedance ID
 *  @return none
 */
/*
0. 600ohms 
1. 900ohms
2. (600ohms + 1uF)
3. (900ohms + 2.16uF) 
4. 270ohms+(750ohms||150nF)
5. also _NewZealand, 220ohms+(820ohms||120nF)
6. also Slovenia/South Africa, 220ohms+(820ohms||115nF)
7. 200ohms+(680ohms||100nF)
*/
int SLIC_RINGImpSetup(int cid ,int imp);

#ifndef OSBN_VOIP
/**
 *  @brief Set up volumn value of SLIC
 *  @param cid: channel ID 
 *  @param rx_vol: rx volumn
 *  @param tx_vol: tx volumn
 *  @return none
 */
int SLIC_AudioVolumeSetup(int cid, int rx_vol, int tx_vol);
#endif
/**
 *  @brief Set up gain value of SLIC
 *  @param cid: channel ID 
 *  @param rx_gain: rx gain value (-9 ~ +9)
 *  @param tx_gain: tx gain value (-9 ~ +9)
 *  @return none
 */

int SLIC_AudioGainSetup(int cid, int rx_gain, int tx_gain);

/**
 *  @brief Destory SLIC channel data structure
 *  @return none
 */
int SLIC_Dest(int cid);
/**
 *  @brief Query SLIC init configuration
 *  @return none
 */
int SLIC_ConfQuery(_TDI_initSetting_t* pinitSetting);

/**
 *  @brief Set up ring interval
 *  @param cid: channel ID 
 *  @param active_ms: active millisecond
 *  @param inactive_ms: inactive millisecond
 *  @return none
 */
int SLIC_RingIntervalSetup(int cid, int active_ms, int inactive_ms);

/**
 *  @brief Setup ring profile
 *  @param cid: channel ID 
 *  @param profile: country
 *  @return none
 */
int SLIC_RingProfileSetup(int cid,int profile);

int* SLIC_DeviceLineTest(_TDI_devicelinetest_t* plinetest); //Roger_lt


#ifdef OSBN_VOIP
int SLIC_FxoGetRingCadence(unsigned char chan);
int SLIC_FxoHookStatusSet(_TDI_hookStat_t* pHookStat);
int SLIC_FxoLineStatusGet(_TDI_fxoLineStat_t* pFxoLineStat);
int SLIC_FxoPSTNStatusGet(unsigned char chan);
int slic_register_read_proc(char *page, char **start, off_t off, int count, int *eof, void *data);
int slic_register_write_proc(struct file *file, const char *buffer,unsigned long count, void *data);
int SLIC_DumpRegister_Cat(char *page, char **start, off_t off, int count, int *eof, void *data);
int SLIC_DumpRegister_Echo(struct file *file, const char *buffer,unsigned long count, void *data);
#endif
#endif
#endif
