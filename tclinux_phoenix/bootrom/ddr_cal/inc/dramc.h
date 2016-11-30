/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein is
 * confidential and proprietary to MediaTek Inc. and/or its licensors. Without
 * the prior written permission of MediaTek inc. and/or its licensors, any
 * reproduction, modification, use or disclosure of MediaTek Software, and
 * information contained herein, in whole or in part, shall be strictly
 * prohibited.
 * 
 * MediaTek Inc. (C) 2010. All rights reserved.
 * 
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER
 * ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
 * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 * INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES
 * TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * RECEIVER EXPRESSLY ACKNOWLEDGES THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO
 * OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES CONTAINED IN MEDIATEK
 * SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE
 * RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S
 * ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE
 * RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE
 * MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE
 * CHARGE PAID BY RECEIVER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek
 * Software") have been modified by MediaTek Inc. All revisions are subject to
 * any receiver's applicable license agreements with MediaTek Inc.
 */

#ifndef _DRAMC_H
#define _DRAMC_H

#if 0
typedef struct {
    char *name;
    char **factor_tbl;
    char *curr_val;
    char *opt_val;
    void (*factor_handler) (char *);
} tuning_factor;


#define ETT_TUNING_FACTOR_NUMS(x)	(sizeof(x)/sizeof(tuning_factor))
#endif

#define REDUCE_CODE_SIZE

#define print prom_printf_s
#define RELEASE
#define dbg_print 



typedef unsigned char U8;
typedef signed char S8;
typedef unsigned short U16;
typedef signed short S16;
typedef unsigned int U32;
typedef signed int S32;
typedef unsigned long long U64;
typedef signed long long S64;
typedef unsigned char bool;

//*****************************************
//Define how many steps we have in coarse tune, fine tune
//check the number of  dqsi_gw_dly_fine_tbl and dqsi_gw_dly_coarse_tbl
//To-be-porting
//Yanbing: Modify DRAM_START macro to NAND flash version, which is also compatible for SPI flash.
#if 1 // def DDR_CALI_NAND
#define DRAM_START (0xa0080000) //(0x80040000)
#else
#define DRAM_START (0xa0000000) //(0x80000000)
#endif

#define DDR2		0
#define DDR3		1
#define DISABLE		0
#define ENABLE		1
#define POS_CONF1_SELFREF	26
#define POS_SPCMDRESP_SREF_STATE	16
#define CEIL_A_OVER_B(_A, _B)	(((_A)-(_B)*((_A)/(_B))) > 0? (_A)/(_B)+1:(_A)/(_B))

//for 751020 ddr2 gw
#define DQS_GW_COARSE_START_ddr2 2
#define DQS_GW_COARSE_END_ddr2	14

//for 751020 ddr3 gw
#define DQS_GW_COARSE_START_ddr3 4
#define DQS_GW_COARSE_END_ddr3	16


#define DQS_GW_COARSE_STEP	1
//#define	DQS_GW_COARSE_MAX	((DQS_GW_COARSE_END-DQS_GW_COARSE_START)/DQS_GW_COARSE_STEP+1)
#define DQS_GW_FINE_START 0
#define DQS_GW_FINE_END	95  // MT751x/2x only have 96 steps
#define DQS_GW_FINE_STEP	1
#define	DQS_GW_FINE_MAX	((DQS_GW_FINE_END-DQS_GW_FINE_START)/DQS_GW_FINE_STEP+1)
#define DQS_GW_LEN_PER_COARSE_ELEMENT	(8*sizeof(unsigned int)) // depends on type of dqs_gw[], unsigned int now: 8 elements per byte
//#define DQS_GW_LEN_PER_COARSE	((int)ceil(DQS_GW_FINE_MAX/DQS_GW_LEN_PER_COARSE_ELEMENT))
#define DQS_GW_LEN_PER_COARSE	(CEIL_A_OVER_B(DQS_GW_FINE_MAX,DQS_GW_LEN_PER_COARSE_ELEMENT))
//#define	DQS_GW_LEN	(DQS_GW_COARSE_MAX*DQS_GW_LEN_PER_COARSE)

#define HW_DQS_GW_COUNTER_CPU_READ 0x04040404

#define	DQS_GW_CAL_METHOD_TA2 // using test agent 2 to calibrate gating window

//#define DQS_GW_COARSE_MAX 32
//#define DQS_GW_FINE_MAX 16
#define DQS_GW_FINE_CHK_RANGE 4

#ifdef DQS_GW_CAL_METHOD_TA2
static const int HW_DQS_GW_COUNTER = 0xf8f8f8f8;  // for reading offset 0xff
//static const int HW_DQS_GW_COUNTER = 0x18181818; // for reading offset 0x3
#else
static const int HW_DQS_GW_COUNTER = 0x80808080; 
#endif

//static unsigned int dqs_gw[DQS_GW_COARSE_MAX];
//static int dqs_gw_coarse, dqs_gw_fine;
//static struct dqs_gw_pass_win cur_pwin, max_pwin;


#define DLE_START	0
#define DLE_END		15
#define DLE_STEP	1
#define DLE_MAX		((DLE_END-DLE_START)/DLE_STEP + 1)

#define fcTX_PER_BIT_DESKEW

#define MAX_TX_DQDLY_TAPS 16 
#define MAX_TX_DQSDLY_TAPS 16 
#define TX_STEPS 0   // This value is obtained by measurement of waveform for the relationship between DQS and CLK.
#define TX_STEPS_DQ 0
#define TX_DQS_NUMBER	2
#define TX_DQ_DATA_WIDTH	16

#define DQ_DATA_WIDTH 16
#define MAX_RX_DQSDLY_TAPS 96 // MT7510/MT7520 only have 96 taps
#define MAX_RX_DQDLY_TAPS 16 
#define DQS_NUMBER 2
#define DQS_BIT_NUMBER (DQ_DATA_WIDTH/DQS_NUMBER)


#define         DRAMC_ACTIM0            (0x0)           /* DRAM AC TIMING SETTING 0 */
#define         DRAMC_CONF1             (0x4)           /* DRAM CONFIGURATION 1 */
#define         DRAMC_CONF2             (0x8)           /* DRAM CONFIGURATION 2 */
#define         DRAMC_PADCTL1           (0xc)           /* DRAM PAD CONTROL 1 */
#define         DRAMC_PADCTL2           (0x10)          /* DRAM PAD CONTROL 2 */
#define         DRAMC_PADCTL3           (0x14)          /* DRAM PAD CONTROL 3 */
#define         DRAMC_R0DELDLY          (0x18)          /* RANK0 DQS INPUT DELAY CHAIN SETTING */
#define         DRAMC_R1DELDLY          (0x1c)          /* RANK1 DQS INPUT DELAY CHAIN SETTING */
#define         DRAMC_R0DIFDLY          (0x20)          /* RANK0 DQS INPUT DELAY CHAIN OFFSET SETTING */
#define         DRAMC_R1DIFDLY          (0x24)          /* RANK1 DQS INPUT DELAY CHAIN OFFSET SETTING */
#define         DRAMC_DLLCONF           (0x28)          /* DLL CONFIGURATION */
#define         DRAMC_TESTMODE          (0x2c)          /* TEST MODE CONFIGURATION 1 */
#define         DRAMC_TEST2_1           (0x3c)          /* TEST AGENT 2 CONFIGURATION 1 */
#define         DRAMC_TEST2_2           (0x40)          /* TEST AGENT 2 CONFIGURATION 2 */
#define         DRAMC_TEST2_3           (0x44)          /* TEST AGENT 2 CONFIGURATION 3 */
#define         DRAMC_TEST2_4           (0x48)          /* TEST AGENT 2 CONFIGURATION 4 */
#define         DRAMC_DDR2CTL           (0x7c)          /* DDR2 CONTROL REGISTER */
#define         DRAMC_MRS               (0x88)          /* MRS value setting */
#define         DRAMC_CLK1DELAY         (0x8c)          /* Clock 1 output delay CONTROL */
#define         DRAMC_IOCTL             (0x90)          /* IO CONTROL */
#define         DRAMC_R0DQSIEN          (0x94)          /* RANK0 DQS INPUT RANGE FINE TUNER */
#define         DRAMC_R1DQSIEN          (0x98)          /* RANK1 DQS INPUT RANGE FINE TUNER */
#define         DRAMC_DRVCTL00          (0xb4)          /* PAD DRIVING CONTROL SETTING 00 */
#define         DRAMC_DRVCTL0           (0xb8)          /* PAD DRIVING CONTROL SETTING 0 */
#define         DRAMC_DRVCTL1           (0xbc)          /* PAD DRIVING CONTROL SETTING 1 */
#define         DRAMC_DLLSEL            (0xc0)          /* DLL SELECTION SETTING */
#define         DRAMC_TDSEL0            (0xcc)          /* IO OUTPUT DUTY CONTROL 0 */
#define         DRAMC_TDSEL1            (0xd0)          /* IO OUTPUT DUTY CONTROL 1 */
#define         DRAMC_MCKDLY            (0xd8)          /* MEMORY CLOCK DELAY CHAIN SETTING */
#define         DRAMC_DQSCTL0           (0xdc)          /* DQS INPUT RANGE CONTROL 0 */
#define         DRAMC_DQSCTL1           (0xe0)          /* DQS INPUT RANGE CONTROL 1 */
#define         DRAMC_PADCTL4           (0xe4)          /* PAD CONTROL 1 */
#define         DRAMC_PADCTL5           (0xe8)          /* PAD CONTROL 2 */
#define         DRAMC_PADCTL6           (0xec)          /* PAD CONTROL 3 */
#define         DRAMC_PHYCTL1           (0xf0)          /* DDR PHY CONTROL 1 */
#define         DRAMC_GDDR3CTL1         (0xf4)          /* GDDR3 CONTROL 1 */
#define         DRAMC_PADCTL7           (0xf8)          /* PAD CONTROL 4 */
#define         DRAMC_MISCTL0           (0xfc)          /* MISC CONTROL 0 */
#define         DRAMC_OCDK              (0x100)         /* OCD CALIBRATION CONTROL */
#define         DRAMC_LBWDAT0           (0x104)         /* LOOP BACK DATA 0 */
#define         DRAMC_LBWDAT1           (0x108)         /* LOOP BACK DATA 1 */
#define         DRAMC_LBWDAT2           (0x10c)         /* LOOP BACK DATA 2 */
#define         DRAMC_RKCFG             (0x110)         /* RANK CONFIGURATION */
#define         DRAMC_CKPHDET           (0x114)         /* CLOCK PHASE DETECTION SETTING */
#define         DRAMC_DQSGCTL           (0x124)         /* INPUT DQS GATING CONTROL */
#define         DRAMC_CLKENCTL          (0x130)         /* DRAM CLOCK ENABLE CONTROL */
#define         DRAMC_DQSGCTL1          (0x140)         /* DQS gating delay control 1 */
#define         DRAMC_DQSGCTL2          (0x144)         /* DQS gating delay control 2 */
#define         DRAMC_ARBCTL0           (0x168)         /* ARBITRATION CONTROL 0 */
#define         DRAMC_CMDDLY0           (0x1a8)         /* Command Delay CTL0 */
#define         DRAMC_CMDDLY1           (0x1ac)         /* Command Delay CTL1 */
#define         DRAMC_CMDDLY2           (0x1b0)         /* Command Delay CTL2 */
#define         DRAMC_CMDDLY3           (0x1b4)         /* Command Delay CTL3 */
#define         DRAMC_CMDDLY4           (0x1b8)         /* Command Delay CTL4 */
#define         DRAMC_CMDDLY5           (0x1bc)         /* Command Delay CTL5 */
#define         DRAMC_DQSCAL0           (0x1c0)         /* DQS CAL CONTROL 0 */
#define         DRAMC_DQSCAL1           (0x1c4)         /* DQS CAL CONTROL 1 */
#define         DRAMC_DMMonitor         (0x1d8)         /* Monitor parameter */
#define         DRAMC_DRAMC_PD_CTRL             (0x1dc)         /* PD mode parameter */
#define         DRAMC_LPDDR2            (0x1e0)         /* LPDDR2 setting */
#define         DRAMC_SPCMD             (0x1e4)         /* Special command mode */
#define         DRAMC_ACTIM1            (0x1e8)         /* DRAM AC TIMING SETTING 1 */
#define         DRAMC_PERFCTL0          (0x1ec)         /* PERFORMANCE CONTROL 0 */
#define         DRAMC_AC_DERATING               (0x1f0)         /* AC TIME DERATING CONTROL */
#define         DRAMC_RRRATE_CTL                (0x1f4)         /* REFRESH RATE CONTROL */
#define         DRAMC_DQODLY1           (0x200)         /* DQ output DELAY1 CHAIN setting  */
#define         DRAMC_DQODLY2           (0x204)         /* DQ output DELAY2 CHAIN setting  */
#define         DRAMC_DQODLY3           (0x208)         /* DQ output DELAY3 CHAIN setting  */
#define         DRAMC_DQODLY4           (0x20c)         /* DQ output DELAY4 CHAIN setting  */
#define         DRAMC_DQIDLY1           (0x210)         /* DQ input DELAY1 CHAIN setting  */
#define         DRAMC_DQIDLY2           (0x214)         /* DQ input DELAY2 CHAIN setting  */
#define         DRAMC_DQIDLY3           (0x218)         /* DQ input DELAY3 CHAIN setting  */
#define         DRAMC_DQIDLY4           (0x21c)         /* DQ input DELAY4 CHAIN setting  */
#define         DRAMC_DQIDLY5           (0x220)         /* DQ input DELAY5 CHAIN setting  */
#define         DRAMC_DQIDLY6           (0x224)         /* DQ input DELAY6 CHAIN setting  */
#define         DRAMC_DQIDLY7           (0x228)         /* DQ input DELAY7 CHAIN setting  */
#define         DRAMC_DQIDLY8           (0x22c)         /* DQ input DELAY8 CHAIN setting  */
#define         DRAMC_R2R_page_hit_counter              (0x280)         /* R2R_page_hit_counter */
#define         DRAMC_R2R_page_miss_counter             (0x284)         /* R2R_page_miss_counter */
#define         DRAMC_R2R_interbank_counter             (0x288)         /* R2R_interbank_counter */
#define         DRAMC_R2W_page_hit_counter              (0x28c)         /* R2W_page_hit_counter */
#define         DRAMC_R2W_page_miss_counter             (0x290)         /* R2W_page_miss_counter */
#define         DRAMC_R2W_interbank_counter             (0x294)         /* R2W_interbank_counter */
#define         DRAMC_W2R_page_hit_counter              (0x298)         /* W2R_page_hit_counter */
#define         DRAMC_W2R_page_miss_counter             (0x29c)         /* W2R_page_miss_counter */
#define         DRAMC_W2R_interbank_counter             (0x2a0)         /* W2R_interbank_counter */
#define         DRAMC_W2W_page_hit_counter              (0x2a4)         /* W2W_page_hit_counter */
#define         DRAMC_W2W_page_miss_counter             (0x2a8)         /* W2W_page_miss_counter */
#define         DRAMC_W2W_interbank_counter             (0x2ac)         /* W2W_interbank_counter */
#define         DRAMC_dramc_idle_counter                (0x2b0)         /* dramc_idle_counter */
#define         DRAMC_freerun_26m_counter               (0x2b4)         /* freerun_26m_counter */
#define         DRAMC_refresh_pop_counter               (0x2b8)         /* refresh_pop_counter */
#define         DRAMC_JMETER_ST         (0x2bc)         /* Jitter Meter Status */
#define         DRAMC_DQ_CAL_MAX_0              (0x2c0)         /* DQ INPUT CALIBRATION per bit 3-0 */
#define         DRAMC_DQ_CAL_MAX_1              (0x2c4)         /* DQ INPUT CALIBRATION per bit 7-4 */
#define         DRAMC_DQ_CAL_MAX_2              (0x2c8)         /* DQ INPUT CALIBRATION per bit 11-8 */
#define         DRAMC_DQ_CAL_MAX_3              (0x2cc)         /* DQ INPUT CALIBRATION per bit 15-12 */
#define         DRAMC_DQ_CAL_MAX_4              (0x2d0)         /* DQ INPUT CALIBRATION per bit 19-16 */
#define         DRAMC_DQ_CAL_MAX_5              (0x2d4)         /* DQ INPUT CALIBRATION per bit 23-20 */
#define         DRAMC_DQ_CAL_MAX_6              (0x2d8)         /* DQ INPUT CALIBRATION per bit 27-34 */
#define         DRAMC_DQ_CAL_MAX_7              (0x2dc)         /* DQ INPUT CALIBRATION per bit 31-28 */
#define         DRAMC_DQS_CAL_MIN_0             (0x2e0)         /* DQS INPUT CALIBRATION per bit 3-0 */
#define         DRAMC_DQS_CAL_MIN_1             (0x2e4)         /* DQS INPUT CALIBRATION per bit 7-4 */
#define         DRAMC_DQS_CAL_MIN_2             (0x2e8)         /* DQS INPUT CALIBRATION per bit 11-8 */
#define         DRAMC_DQS_CAL_MIN_3             (0x2ec)         /* DQS INPUT CALIBRATION per bit 15-12 */
#define         DRAMC_DQS_CAL_MIN_4             (0x2f0)         /* DQS INPUT CALIBRATION per bit 19-16 */
#define         DRAMC_DQS_CAL_MIN_5             (0x2f4)         /* DQS INPUT CALIBRATION per bit 23-20 */
#define         DRAMC_DQS_CAL_MIN_6             (0x2f8)         /* DQS INPUT CALIBRATION per bit 27-34 */
#define         DRAMC_DQS_CAL_MIN_7             (0x2fc)         /* DQS INPUT CALIBRATION per bit 31-28 */
#define         DRAMC_DQS_CAL_MAX_0             (0x300)         /* DQS INPUT CALIBRATION per bit 3-0 */
#define         DRAMC_DQS_CAL_MAX_1             (0x304)         /* DQS INPUT CALIBRATION per bit 7-4 */
#define         DRAMC_DQS_CAL_MAX_2             (0x308)         /* DQS INPUT CALIBRATION per bit 11-8 */
#define         DRAMC_DQS_CAL_MAX_3             (0x30c)         /* DQS INPUT CALIBRATION per bit 15-12 */
#define         DRAMC_DQS_CAL_MAX_4             (0x310)         /* DQS INPUT CALIBRATION per bit 19-16 */
#define         DRAMC_DQS_CAL_MAX_5             (0x314)         /* DQS INPUT CALIBRATION per bit 23-20 */
#define         DRAMC_DQS_CAL_MAX_6             (0x318)         /* DQS INPUT CALIBRATION per bit 27-34 */
#define         DRAMC_DQS_CAL_MAX_7             (0x31c)         /* DQS INPUT CALIBRATION per bit 31-28 */
#define         DRAMC_DQICAL0           (0x350)         /* DQS INPUT CALIBRATION 0 */
#define         DRAMC_DQICAL1           (0x354)         /* DQS INPUT CALIBRATION 1 */
#define         DRAMC_DQICAL2           (0x358)         /* DQS INPUT CALIBRATION 2 */
#define         DRAMC_DQICAL3           (0x35c)         /* DQS INPUT CALIBRATION 3 */
#define         DRAMC_CMP_ERR           (0x370)         /* CMP ERROR */
#define         DRAMC_R0DQSIENDLY               (0x374)         /* RANK0 DQS INPUT GATING DELAY VALUE */
#define         DRAMC_R1DQSIENDLY               (0x378)         /* RANK1 DQS INPUT GATING DELAY VALUE */
#define         DRAMC_STBEN0            (0x38c)         /* DQS RING COUNTER 0 */
#define         DRAMC_STBEN1            (0x390)         /* DQS RING COUNTER 1 */
#define         DRAMC_STBEN2            (0x394)         /* DQS RING COUNTER 2 */
#define         DRAMC_STBEN3            (0x398)         /* DQS RING COUNTER 3 */
#define         DRAMC_DQSDLY0           (0x3a0)         /* DQS INPUT DELAY SETTING 0 */
#define         DRAMC_SPCMDRESP         (0x3b8)         /* SPECIAL COMMAND RESPONSE */
#define         DRAMC_IORGCNT           (0x3bc)         /* IO RING COUNTER */
#define         DRAMC_DQSGNWCNT0                (0x3c0)         /* DQS GATING WINODW COUNTER 0 */
#define         DRAMC_DQSGNWCNT1                (0x3c4)         /* DQS GATING WINODW COUNTER 1 */
#define         DRAMC_DQSGNWCNT2                (0x3c8)         /* DQS GATING WINODW COUNTER 2 */
#define         DRAMC_DQSGNWCNT3                (0x3cc)         /* DQS GATING WINODW COUNTER 3 */
#define         DRAMC_DQSGNWCNT4                (0x3d0)         /* DQS GATING WINODW COUNTER 4 */
#define         DRAMC_DQSGNWCNT5                (0x3d4)         /* DQS GATING WINODW COUNTER 5 */
#define         DRAMC_DQSSAMPLEV                (0x3d8)         /* DQS SAMPLE VALUE */
#define         DRAMC_DLLCNT0           (0x3dc)         /* DLL STATUS 0 */
#define         DRAMC_CKPHCNT           (0x3e8)         /* CLOCK PHASE DETECTION RESULT */
#define         DRAMC_TESTRPT           (0x3fc)         /* TEST AGENT STATUS */
#define         DRAMC_MEMPLL0           (0x600)         /* MEMPLL REGISTER SETTING 0 */
#define         DRAMC_MEMPLL1           (0x604)         /* MEMPLL REGISTER SETTING 1 */
#define         DRAMC_MEMPLL2           (0x608)         /* MEMPLL REGISTER SETTING 2 */
#define         DRAMC_MEMPLL3           (0x60c)         /* MEMPLL REGISTER SETTING 3 */
#define         DRAMC_MEMPLL4           (0x610)         /* MEMPLL REGISTER SETTING 4 */
#define         DRAMC_MEMPLL5           (0x614)         /* MEMPLL REGISTER SETTING 5 */
#define         DRAMC_MEMPLL6           (0x618)         /* MEMPLL REGISTER SETTING 6 */
#define         DRAMC_MEMPLL7           (0x61c)         /* MEMPLL REGISTER SETTING 7 */
#define         DRAMC_MEMPLL8           (0x620)         /* MEMPLL REGISTER SETTING 8 */
#define         DRAMC_MEMPLL9           (0x624)         /* MEMPLL REGISTER SETTING 9 */
#define         DRAMC_MEMPLL10          (0x628)         /* MEMPLL REGISTER SETTING 10 */
#define         DRAMC_MEMPLL11          (0x62c)         /* MEMPLL REGISTER SETTING 11 */
#define         DRAMC_MEMPLL12          (0x630)         /* MEMPLL REGISTER SETTING 12 */
#define         DRAMC_MEMPLL_DIVIDER            (0x640)         /* MEMPLL DIVIDER REGISTER CONTROL */
#define         DRAMC_VREFCTL0          (0x644)         /* VREF REGISTER SETTING 0 */


#define         DRAMC_ACTIM0_DEFAULT            0x22564154
#define         DRAMC_CONF1_DEFAULT             0x00000000
#define         DRAMC_CONF2_DEFAULT             0x00000000
#define         DRAMC_PADCTL1_DEFAULT           0x00000000
#define         DRAMC_PADCTL2_DEFAULT           0x00000000
#define         DRAMC_PADCTL3_DEFAULT           0x00000000
#define         DRAMC_R0DELDLY_DEFAULT          0x00000000
#define         DRAMC_R1DELDLY_DEFAULT          0x00000000
#define         DRAMC_R0DIFDLY_DEFAULT          0x00000000
#define         DRAMC_R1DIFDLY_DEFAULT          0x00000000
#define         DRAMC_DLLCONF_DEFAULT           0x8000FF01
#define         DRAMC_TESTMODE_DEFAULT          0x55010000
#define         DRAMC_TEST2_1_DEFAULT           0x01200000
#define         DRAMC_TEST2_2_DEFAULT           0x00010000
#define         DRAMC_TEST2_3_DEFAULT           0x00000000
#define         DRAMC_TEST2_4_DEFAULT           0x0000110D
#define         DRAMC_DDR2CTL_DEFAULT           0x00000000
#define         DRAMC_MRS_DEFAULT               0x00000000
#define         DRAMC_CLK1DELAY_DEFAULT         0x00000000
#define         DRAMC_IOCTL_DEFAULT             0x00000000
#define         DRAMC_R0DQSIEN_DEFAULT          0x00000000
#define         DRAMC_R1DQSIEN_DEFAULT          0x00000000
#define         DRAMC_DRVCTL00_DEFAULT          0xAA22AA22
#define         DRAMC_DRVCTL0_DEFAULT           0xAA22AA22
#define         DRAMC_DRVCTL1_DEFAULT           0xAA22AA22
#define         DRAMC_DLLSEL_DEFAULT            0x00000000
#define         DRAMC_TDSEL0_DEFAULT            0x00000000
#define         DRAMC_TDSEL1_DEFAULT            0x00000000
#define         DRAMC_MCKDLY_DEFAULT            0x00000900
#define         DRAMC_DQSCTL0_DEFAULT           0x00000000
#define         DRAMC_DQSCTL1_DEFAULT           0x00000000
#define         DRAMC_PADCTL4_DEFAULT           0x00000000
#define         DRAMC_PADCTL5_DEFAULT           0x00000000
#define         DRAMC_PADCTL6_DEFAULT           0x00000000
#define         DRAMC_PHYCTL1_DEFAULT           0x00000000
#define         DRAMC_GDDR3CTL1_DEFAULT         0x00000000
#define         DRAMC_PADCTL7_DEFAULT           0xEDCB0000
#define         DRAMC_MISCTL0_DEFAULT           0x00000000
#define         DRAMC_OCDK_DEFAULT              0x00000000
#define         DRAMC_LBWDAT0_DEFAULT           0x00000000
#define         DRAMC_LBWDAT1_DEFAULT           0x00000000
#define         DRAMC_LBWDAT2_DEFAULT           0x00000000
#define         DRAMC_RKCFG_DEFAULT             0x0B051100
#define         DRAMC_CKPHDET_DEFAULT           0x00000000
#define         DRAMC_DQSGCTL_DEFAULT           0xAA080088
#define         DRAMC_CLKENCTL_DEFAULT          0x50000000
#define         DRAMC_DQSGCTL1_DEFAULT          0x00000000
#define         DRAMC_DQSGCTL2_DEFAULT          0x00000000
#define         DRAMC_ARBCTL0_DEFAULT           0x00000000
#define         DRAMC_CMDDLY0_DEFAULT           0x00000000
#define         DRAMC_CMDDLY1_DEFAULT           0x00000000
#define         DRAMC_CMDDLY2_DEFAULT           0x00000000
#define         DRAMC_CMDDLY3_DEFAULT           0x00000000
#define         DRAMC_CMDDLY4_DEFAULT           0x00000000
#define         DRAMC_CMDDLY5_DEFAULT           0x00000000
#define         DRAMC_DQSCAL0_DEFAULT           0x00000000
#define         DRAMC_DQSCAL1_DEFAULT           0x00000000
#define         DRAMC_DMMonitor_DEFAULT         0x00C80000
#define         DRAMC_DRAMC_PD_CTRL_DEFAULT             0x10622842
#define         DRAMC_LPDDR2_DEFAULT            0x00000000
#define         DRAMC_SPCMD_DEFAULT             0x00000000
#define         DRAMC_ACTIM1_DEFAULT            0x00000000
#define         DRAMC_PERFCTL0_DEFAULT          0x00000000
#define         DRAMC_AC_DERATING_DEFAULT               0x00000000
#define         DRAMC_RRRATE_CTL_DEFAULT                0x00020100
#define         DRAMC_DQODLY1_DEFAULT           0x00000000
#define         DRAMC_DQODLY2_DEFAULT           0x00000000
#define         DRAMC_DQODLY3_DEFAULT           0x00000000
#define         DRAMC_DQODLY4_DEFAULT           0x00000000
#define         DRAMC_DQIDLY1_DEFAULT           0x00000000
#define         DRAMC_DQIDLY2_DEFAULT           0x00000000
#define         DRAMC_DQIDLY3_DEFAULT           0x00000000
#define         DRAMC_DQIDLY4_DEFAULT           0x00000000
#define         DRAMC_DQIDLY5_DEFAULT           0x00000000
#define         DRAMC_DQIDLY6_DEFAULT           0x00000000
#define         DRAMC_DQIDLY7_DEFAULT           0x00000000
#define         DRAMC_DQIDLY8_DEFAULT           0x00000000
#define         DRAMC_R2R_page_hit_counter_DEFAULT              0x00000000
#define         DRAMC_R2R_page_miss_counter_DEFAULT             0x00000000
#define         DRAMC_R2R_interbank_counter_DEFAULT             0x00000000
#define         DRAMC_R2W_page_hit_counter_DEFAULT              0x00000000
#define         DRAMC_R2W_page_miss_counter_DEFAULT             0x00000000
#define         DRAMC_R2W_interbank_counter_DEFAULT             0x00000000
#define         DRAMC_W2R_page_hit_counter_DEFAULT              0x00000000
#define         DRAMC_W2R_page_miss_counter_DEFAULT             0x00000000
#define         DRAMC_W2R_interbank_counter_DEFAULT             0x00000000
#define         DRAMC_W2W_page_hit_counter_DEFAULT              0x00000000
#define         DRAMC_W2W_page_miss_counter_DEFAULT             0x00000000
#define         DRAMC_W2W_interbank_counter_DEFAULT             0x00000000
#define         DRAMC_dramc_idle_counter_DEFAULT                0x00000000
#define         DRAMC_freerun_26m_counter_DEFAULT               0x00000000
#define         DRAMC_refresh_pop_counter_DEFAULT               0x00000000
#define         DRAMC_JMETER_ST_DEFAULT         0x00000000
#define         DRAMC_DQ_CAL_MAX_0_DEFAULT              0x00000000
#define         DRAMC_DQ_CAL_MAX_1_DEFAULT              0x00000000
#define         DRAMC_DQ_CAL_MAX_2_DEFAULT              0x00000000
#define         DRAMC_DQ_CAL_MAX_3_DEFAULT              0x00000000
#define         DRAMC_DQ_CAL_MAX_4_DEFAULT              0x00000000
#define         DRAMC_DQ_CAL_MAX_5_DEFAULT              0x00000000
#define         DRAMC_DQ_CAL_MAX_6_DEFAULT              0x00000000
#define         DRAMC_DQ_CAL_MAX_7_DEFAULT              0x00000000
#define         DRAMC_DQS_CAL_MIN_0_DEFAULT             0x00000000
#define         DRAMC_DQS_CAL_MIN_1_DEFAULT             0x00000000
#define         DRAMC_DQS_CAL_MIN_2_DEFAULT             0x00000000
#define         DRAMC_DQS_CAL_MIN_3_DEFAULT             0x00000000
#define         DRAMC_DQS_CAL_MIN_4_DEFAULT             0x00000000
#define         DRAMC_DQS_CAL_MIN_5_DEFAULT             0x00000000
#define         DRAMC_DQS_CAL_MIN_6_DEFAULT             0x00000000
#define         DRAMC_DQS_CAL_MIN_7_DEFAULT             0x00000000
#define         DRAMC_DQS_CAL_MAX_0_DEFAULT             0x00000000
#define         DRAMC_DQS_CAL_MAX_1_DEFAULT             0x00000000
#define         DRAMC_DQS_CAL_MAX_2_DEFAULT             0x00000000
#define         DRAMC_DQS_CAL_MAX_3_DEFAULT             0x00000000
#define         DRAMC_DQS_CAL_MAX_4_DEFAULT             0x00000000
#define         DRAMC_DQS_CAL_MAX_5_DEFAULT             0x00000000
#define         DRAMC_DQS_CAL_MAX_6_DEFAULT             0x00000000
#define         DRAMC_DQS_CAL_MAX_7_DEFAULT             0x00000000
#define         DRAMC_DQICAL0_DEFAULT           0x00000000
#define         DRAMC_DQICAL1_DEFAULT           0x00000000
#define         DRAMC_DQICAL2_DEFAULT           0x00000000
#define         DRAMC_DQICAL3_DEFAULT           0x00000000
#define         DRAMC_CMP_ERR_DEFAULT           0x00000000
#define         DRAMC_R0DQSIENDLY_DEFAULT               0x00000000
#define         DRAMC_R1DQSIENDLY_DEFAULT               0x00000000
#define         DRAMC_STBEN0_DEFAULT            0x00000003
#define         DRAMC_STBEN1_DEFAULT            0x00000003
#define         DRAMC_STBEN2_DEFAULT            0x00000003
#define         DRAMC_STBEN3_DEFAULT            0x00000003
#define         DRAMC_DQSDLY0_DEFAULT           0x0F0F0F0F
#define         DRAMC_SPCMDRESP_DEFAULT         0x00010300
#define         DRAMC_IORGCNT_DEFAULT           0x00000000
#define         DRAMC_DQSGNWCNT0_DEFAULT                0x00000000
#define         DRAMC_DQSGNWCNT1_DEFAULT                0x00000000
#define         DRAMC_DQSGNWCNT2_DEFAULT                0x00000000
#define         DRAMC_DQSGNWCNT3_DEFAULT                0x00000000
#define         DRAMC_DQSGNWCNT4_DEFAULT                0x00000000
#define         DRAMC_DQSGNWCNT5_DEFAULT                0x00000000
#define         DRAMC_DQSSAMPLEV_DEFAULT                0x00000000
#define         DRAMC_DLLCNT0_DEFAULT           0x00000000
#define         DRAMC_CKPHCNT_DEFAULT           0x00000000
#define         DRAMC_TESTRPT_DEFAULT           0x00000000
#define         DRAMC_MEMPLL0_DEFAULT           0x044C6000
#define         DRAMC_MEMPLL1_DEFAULT           0x00002000
#define         DRAMC_MEMPLL2_DEFAULT           0x0000044C
#define         DRAMC_MEMPLL3_DEFAULT           0x60000000
#define         DRAMC_MEMPLL4_DEFAULT           0x0000044C
#define         DRAMC_MEMPLL5_DEFAULT           0x60000000
#define         DRAMC_MEMPLL6_DEFAULT           0x0000044C
#define         DRAMC_MEMPLL7_DEFAULT           0x60000000
#define         DRAMC_MEMPLL8_DEFAULT           0x00000000
#define         DRAMC_MEMPLL9_DEFAULT           0x00000000
#define         DRAMC_MEMPLL10_DEFAULT          0x00000000
#define         DRAMC_MEMPLL11_DEFAULT          0x00000000
#define         DRAMC_MEMPLL12_DEFAULT          0x00000000
#define         DRAMC_MEMPLL_DIVIDER_DEFAULT            0x00000003
#define         DRAMC_VREFCTL0_DEFAULT          0x00000000



#define DRAMC0_BASE            0xbfb20000 //     0x10004000
//#define DDRPHY_BASE                 0x10011000
//#define DRAMC_NAO_BASE              0x1020F000

#define DRAMC_WRITE_REG(val,offset)     do{ \
                                      (*(volatile unsigned int *)(DRAMC0_BASE + (offset))) = (unsigned int)(val); \
                                      }while(0)

#define DRAMC_WRITE_SET(val,offset)     do{ \
                                      (*(volatile unsigned int *)(DRAMC0_BASE + (offset))) |= (unsigned int)(val); \
                                      }while(0)

#define DRAMC_WRITE_CLEAR(val,offset)   do{ \
                                      (*(volatile unsigned int *)(DRAMC0_BASE + (offset))) &= ~(unsigned int)(val); \
                                      }while(0)

#define DRAMC_WRITE_REG_W(val,offset)   do{ \
                                      (*(volatile unsigned int *)(DRAMC0_BASE + (offset))) = (unsigned int)(val); \
                                      }while(0)

#define DRAMC_WRITE_REG_H(val,offset)   do{ \
                                      (*(volatile unsigned short *)(DRAMC0_BASE + (offset))) = (unsigned short)(val); \
                                      }while(0)

#define DRAMC_WRITE_REG_B(val,offset)   do{ \
                                      (*(volatile unsigned char *)(DRAMC0_BASE + (offset))) = (unsigned char)(val); \
                                      }while(0)

#define DRAMC_READ_REG(offset)         ( \
                                        (*(volatile unsigned int *)(DRAMC0_BASE + (offset))) \
                                       )

#define ADDR_WRITE_REG(val,reg_addr)     do{ \
                                      (*(volatile unsigned int *)(reg_addr)) = (unsigned int)(val); \
                                      }while(0)
#define ADDR_READ_REG(reg_addr)         ( \
											(*(volatile unsigned int *)(reg_addr)) \
										   )


#define delay_a_while(count) \
	do{	\
	pause_polling((count+999)/1000); \
	}while(0)



typedef struct {
    int (*test_case) (unsigned int, unsigned int, void *);
    unsigned int start;
    unsigned int range;
    void *ext_arg;
} test_case;

#define ETT_TEST_CASE_NUMS(x)	(sizeof(x)/sizeof(test_case))

#if 0
#if defined(MT6589)
#define DRAMC_WRITE_REG(val,offset)  do{ \
                                      (*(volatile unsigned int *)(DRAMC0_BASE + (offset))) = (unsigned int)(val); \
                                      (*(volatile unsigned int *)(DDRPHY_BASE + (offset))) = (unsigned int)(val); \
                                      (*(volatile unsigned int *)(DRAMC_NAO_BASE + (offset))) = (unsigned int)(val); \
                                      }while(0)

#define DRAMC_WRITE_REG_W(val,offset)     do{ \
                                      (*(volatile unsigned int *)(DRAMC0_BASE + (offset))) = (unsigned int)(val); \
                                      (*(volatile unsigned int *)(DDRPHY_BASE + (offset))) = (unsigned int)(val); \
                                      (*(volatile unsigned int *)(DRAMC_NAO_BASE + (offset))) = (unsigned int)(val); \
                                      }while(0)

#define DRAMC_WRITE_REG_H(val,offset)     do{ \
                                      (*(volatile unsigned short *)(DRAMC0_BASE + (offset))) = (unsigned short)(val); \
                                      (*(volatile unsigned short *)(DDRPHY_BASE + (offset))) = (unsigned short)(val); \
                                      (*(volatile unsigned short *)(DDRPHY_BASE + (offset))) = (unsigned short)(val); \
                                      }while(0)
#define DRAMC_WRITE_REG_B(val,offset)     do{ \
                                      (*(volatile unsigned char *)(DRAMC0_BASE + (offset))) = (unsigned char)(val); \
                                      (*(volatile unsigned char *)(DDRPHY_BASE + (offset))) = (unsigned char)(val); \
                                      (*(volatile unsigned char *)(DDRPHY_BASE + (offset))) = (unsigned char)(val); \
                                      }while(0)
#define DRAMC_READ_REG(offset)         ( \
                                        (*(volatile unsigned int *)(DRAMC0_BASE + (offset))) |\
                                        (*(volatile unsigned int *)(DDRPHY_BASE + (offset))) |\
                                        (*(volatile unsigned int *)(DRAMC_NAO_BASE + (offset))) \
                                       )
#define DRAMC_WRITE_SET(val,offset)     do{ \
                                      (*(volatile unsigned int *)(DRAMC0_BASE + (offset))) |= (unsigned int)(val); \
                                      (*(volatile unsigned int *)(DDRPHY_BASE + (offset))) |= (unsigned int)(val); \
                                      (*(volatile unsigned int *)(DRAMC_NAO_BASE + (offset))) |= (unsigned int)(val); \
                                      }while(0)

#define DRAMC_WRITE_CLEAR(val,offset)     do{ \
                                      (*(volatile unsigned int *)(DRAMC0_BASE + (offset))) &= ~(unsigned int)(val); \
                                      (*(volatile unsigned int *)(DDRPHY_BASE + (offset))) &= ~(unsigned int)(val); \
                                      (*(volatile unsigned int *)(DRAMC_NAO_BASE + (offset))) &= ~(unsigned int)(val); \
                                      }while(0)

#define DDRPHY_WRITE_REG(val,offset)    __raw_writel(val, (DDRPHY_BASE + (offset)))
#define DRAMC0_WRITE_REG(val,offset)    __raw_writel(val, (DRAMC0_BASE + (offset)))
#define DRAMC_NAO_WRITE_REG(val,offset) __raw_writel(val, (DRAMC_NAO_BASE + (offset)))
#else

#endif


#define ETT_TEST_CASE_NUMS(x)	(sizeof(x)/sizeof(test_case))

#define GRAY_ENCODED(a) (a)

#ifndef NULL
#define NULL    0
#endif
#endif

#define DDR_PHY_RESET() do { \
} while(0)

#define DDR_PHY_RESET_NEW() do { \
    DRAMC_WRITE_REG((DRAMC_READ_REG(DRAMC_PHYCTL1)) \
		| (1 << 28), \
		DRAMC_PHYCTL1); \
    DRAMC_WRITE_REG((DRAMC_READ_REG(DRAMC_GDDR3CTL1)) \
		| (1 << 25),	\
		DRAMC_GDDR3CTL1); \
    delay_a_while(1000); \
    DRAMC_WRITE_REG((DRAMC_READ_REG(DRAMC_PHYCTL1)) \
		& (~(1 << 28)),	\
		DRAMC_PHYCTL1); \
    DRAMC_WRITE_REG((DRAMC_READ_REG(DRAMC_GDDR3CTL1)) \
		& (~(1 << 25)),	\
		DRAMC_GDDR3CTL1); \
} while(0)

#if 0
/* define supported DRAM types */
enum
{
  TYPE_LPDDR2 = 0,
  TYPE_DDR3,
  TYPE_LPDDR3,
  TYPE_mDDR,
};
#endif
#endif  /* !_DRAMC_H */
