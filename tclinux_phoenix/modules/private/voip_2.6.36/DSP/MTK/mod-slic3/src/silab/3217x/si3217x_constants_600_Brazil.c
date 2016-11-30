/*
** Copyright (c) 2012 Silicon Laboratories, Inc.
** 2013-12-13 15:39:55
**
** Si3217x ProSLIC API Configuration Tool Version 3.0.0
*/


#include "proslic.h"
#include "si3217x.h"

Si3217x_General_Cfg Si3217x_General_Configuration  = {
0x71,                       /* DEVICE_KEY */
BO_DCDC_BUCK_BOOST,         /* BOM_OPT */
BO_GDRV_NOT_INSTALLED,      /* GDRV_OPTION */
VDC_8P0_16P0,               /* VDC_RANGE_OPT */
VDAA_DISABLED,              /* DAA_ENABLE */
AUTO_ZCAL_ENABLED,          /* ZCAL_ENABLE */
BO_STD_BOM,                 /* PM_BOM */
1100L,                      /* I_OITHRESH (1100mA) */
900L,                       /* I_OITHRESH_LO (900mA) */
1700L,                      /* I_OITHRESH_HI (1700mA) */
136000L,                    /* V_OVTHRESH (136000mV) */
5000L,                      /* V_UVTHRESH (5000mV) */
1000L,                      /* V_UVHYST (1000mV) */
0x00000000L,                /* DCDC_FSW_VTHLO */
0x00000000L,                /* DCDC_FSW_VHYST */
0x0048D15BL,                /* P_TH_HVIC */
0x07FE4B00L,                /* COEF_P_HVIC */
0x00083120L,                /* BAT_HYST */
0x03D70A20L,                /* VBATH_EXPECT (60.00V) */
0x06147AB2L,                /* VBATR_EXPECT (95.00V) */
0x0FFF0000L,                /* PWRSAVE_TIMER */
0x01999A00L,                /* OFFHOOK_THRESH */
0x00C00000L,                /* VBAT_TRACK_MIN */
0x01800000L,                /* VBAT_TRACK_MIN_RNG */
0x00200000L,                /* DCDC_FSW_NORM */
0x00200000L,                /* DCDC_FSW_NORM_LO */
0x00200000L,                /* DCDC_FSW_RINGING */
0x00200000L,                /* DCDC_FSW_RINGING_LO */
0x0D980000L,                /* DCDC_DIN_LIM */
0x00C00000L,                /* DCDC_VOUT_LIM */
0x0ADD5500L,                /* DCDC_ANA_SCALE */
0x00800000L,                /* THERM_DBI */
0x00FFFFFFL,                /* VOV_DCDC_SLOPE */
0x00A18937L,                /* VOV_DCDC_OS */
0x00A18937L,                /* VOV_RING_BAT_DCDC */
0x00E49BA5L,                /* VOV_RING_BAT_MAX */
0x01018900L,                /* DCDC_VERR */
0x0080C480L,                /* DCDC_VERR_HYST */
0x00400000L,                /* PD_UVLO */
0x00400000L,                /* PD_OVLO */
0x00200000L,                /* PD_OCLO */
0x00400000L,                /* PD_SWDRV */
0x00000000L,                /* DCDC_UVPOL */
0x00000000L,                /* DCDC_RNGTYPE */
0x00600000L,                /* DCDC_ANA_TOFF */
0x00400000L,                /* DCDC_ANA_TONMIN */
0x01500000L,                /* DCDC_ANA_TONMAX */
0x00,                       /* IRQEN1 */
0x00,                       /* IRQEN */
0x03,                       /* IRQEN3 */
0x00,                       /* IRQEN4 */
0x30,                       /* ENHANCE */
0x00,                       /* DAA_CNTL */
0x3F,                       /* AUTO */
};

Si3217x_GPIO_Cfg Si3217x_GPIO_Configuration = {
0x00,     /* GPIO_OE */
0x00,     /* GPIO_ANA */
0x00,     /* GPIO_DIR */
0x00,     /* GPIO_MAN */
0x00,     /* GPIO_POL */
0x00,     /* GPIO_OD */
0x00     /* BATSELMAP */
};
Si3217x_CI_Cfg Si3217x_CI_Presets [] = {
{0}
};
Si3217x_audioGain_Cfg Si3217x_audioGain_Presets [] = {
{0x1377080L,0, 0x0L, 0x0L, 0x0L, 0x0L},
{0x80C3180L,0, 0x0L, 0x0L, 0x0L, 0x0L}
};

Si3217x_Ring_Cfg Si3217x_Ring_Presets[] ={
{
/*
	Loop = 500.0 ft @ 0.044 ohms/ft, REN = 5, Rcpe = 600 ohms
	Rprot = 30 ohms, Type = LPR, Waveform = SINE
*/ 
0x00050000L,	/* RTPER */
0x07EFE000L,	/* RINGFR (20.000 Hz) */
0x001B9F2EL,	/* RINGAMP (45.000 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x00A2DB71L,	/* IRING_LIM (70.000 mA) */
0x0068E9B4L,	/* RTACTH (57.936 mA) */
0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
0x00006000L,	/* RTACDB (75.000 ms) */
0x00006000L,	/* RTDCDB (75.000 ms) */
0x00C49BA0L,	/* VOV_RING_BAT (12.000 v) */
0x00000000L,	/* VOV_RING_GND (0.000 v) */
0x0558ABFCL,	/* VBATR_EXPECT (83.537 v) */
0x80,			/* RINGTALO (2.000 s) */
0x3E,			/* RINGTAHI */
0x00,			/* RINGTILO (4.000 s) */
0x7D,			/* RINGTIHI */
0x00000000L,	/* ADAP_RING_MIN_I */
0x00003000L,	/* COUNTER_IRING_VAL */
0x00051EB8L,	/* COUNTER_VTR_VAL */
0x00000000L,	/* CONST_028 */
0x00000000L,	/* CONST_032 */
0x00000000L,	/* CONST_038 */
0x00000000L,	/* CONST_046 */
0x00000000L,	/* RRD_DELAY */
0x00000000L,	/* RRD_DELAY2 */
0x01893740L,	/* DCDC_VREF_MIN_RNG */
0x40,			/* RINGCON */
0x01,			/* USERSTAT */
0x02AC55FEL,	/* VCM_RING (38.769 v) */
0x02AC55FEL,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00000000L,	/* DCDC_RNGTYPE */
}    /* DEFAULT_RINGING */
};

Si3217x_DCfeed_Cfg Si3217x_DCfeed_Presets[] = {
{
0x1C8A024CL,	/* SLOPE_VLIM */
0x1EE08C11L,	/* SLOPE_RFEED */
0x0040A0E0L,	/* SLOPE_ILIM */
0x1C940D71L,	/* SLOPE_DELTA1 */
0x1DD87A3EL,	/* SLOPE_DELTA2 */
0x05A38633L,	/* V_VLIM (48.000 v) */
0x050D2839L,	/* V_RFEED (43.000 v) */
0x03FE7F0FL,	/* V_ILIM  (34.000 v) */
0x01241BC9L,	/* CONST_RFEED (15.000 mA) */
0x0074538FL,	/* CONST_ILIM (25.000 mA) */
0x002D8D96L,	/* I_VLIM (0.000 mA) */
0x005B0AFBL,	/* LCRONHK (10.000 mA) */
0x006D4060L,	/* LCROFFHK (12.000 mA) */
0x00008000L,	/* LCRDBI (5.000 ms) */
0x0048D595L,	/* LONGHITH (8.000 mA) */
0x003FBAE2L,	/* LONGLOTH (7.000 mA) */
0x00008000L,	/* LONGDBI (5.000 ms) */
0x000F0000L,	/* LCRMASK (150.000 ms) */
0x00080000L,	/* LCRMASK_POLREV (80.000 ms) */
0x00140000L,	/* LCRMASK_STATE (200.000 ms) */
0x00140000L,	/* LCRMASK_LINECAP (200.000 ms) */
0x01BA5E35L,	/* VCM_OH (27.000 v) */
0x0051EB85L,	/* VOV_BAT (5.000 v) */
0x00418937L,	/* VOV_GND (4.000 v) */
}    /* DEFAULT_DCFEED */
};

Si3217x_Impedance_Cfg Si3217x_Impedance_Presets[] ={
/* Source: Database file: cwdb.db */
/* Database information: version: 1.1.0 build date: 2013-01-08*/
/* parameters: zref=600_0_0 rprot=30 rfuse=50 emi_cap=10*/
{
{0x07F4D700L, 0x000E5B00L, 0x0000EA00L, 0x1FFDBF80L,    /* TXACEQ */
 0x07ECDC00L, 0x0012A980L, 0x1FFDD980L, 0x1FFC8C80L},   /* RXACEQ */
{0x00225200L, 0x1FAF3080L, 0x020B7300L, 0x002E0B00L,    /* ECFIR/ECIIR */
 0x02135580L, 0x1F403B80L, 0x00535A80L, 0x1FEA9380L,
 0x1FFDF500L, 0x1FFCF380L, 0x025D9D00L, 0x0525F500L},
{0x007EA900L, 0x1F038E80L, 0x007DC900L, 0x0FF66A00L,    /* ZSYNTH */
 0x18099380L, 0x66}, 
 0x095F9080L,   /* TXACGAIN */
 0x01650680L,   /* RXACGAIN */
 0x07B8D580L, 0x18472B00L, 0x0771AB80L,    /* RXACHPF */
#ifdef ENABLE_HIRES_GAIN
 0, 0  /* TXGAIN*10, RXGAIN*10 (hi_res) */
#else
 0, 0  /* TXGAIN, RXGAIN */
#endif
 }    /* DEFAULT_IMPEDANCE */
};

Si3217x_FSK_Cfg Si3217x_FSK_Presets[] ={
{
0x02232000L,	 /* FSK01 */
0x077C2000L,	 /* FSK10 */
0x003C0000L,	 /* FSKAMP0 (0.220 vrms )*/
0x00200000L,	 /* FSKAMP1 (0.220 vrms) */
0x06B60000L,	 /* FSKFREQ0 (2200.0 Hz space) */
0x079C0000L,	 /* FSKFREQ1 (1200.0 Hz mark) */
0x00,			 /* FSK8 */
0x00,			 /* FSKDEPTH (1 deep fifo) */
}    /* DEFAULT_FSK */
};

Si3217x_PulseMeter_Cfg Si3217x_PulseMeter_Presets[] ={
{
0x007A2B6AL,  /* PM_AMP_THRESH (1.000) */
0,            /* Freq (12kHz) */ 
0,            /* PM_RAMP (24kHz)*/
0,            /* PM_FORCE (First)*/
0,            /* PWR_SAVE (off)*/
0,            /* PM_AUTO (off)*/
0x07D00000L,  /* PM_active (2000 ms) */
0x07D00000L   /* PM_inactive (2000 ms) */
 }    /* DEFAULT_PULSE_METERING */
};

Si3217x_Tone_Cfg Si3217x_Tone_Presets[] = {
{
	{
	0x07B30000L,	 /* OSC1FREQ (350.000 Hz) */
	0x000C6000L,	 /* OSC1AMP (-18.000 dBm) */
	0x00000000L,	 /* OSC1PHAS (0.000 rad) */
	0x80,			 /* O1TALO (2000 ms) */
	0x3E,			 /* O1TAHI */
	0x00,			 /* O1TILO (4000 ms) */
	0x7D			 /* O1TIHI */
	},
	{
	0x07870000L,	 /* OSC2FREQ (440.000 Hz) */
	0x000FA000L,	 /* OSC2AMP (-18.000 dBm) */
	0x00000000L,	 /* OSC2PHAS (0.000 rad) */
	0x80,			 /* O2TALO (2000 ms) */
	0x3E,			 /* O2TAHI */
	0x00,			 /* O2TILO (4000 ms) */
	0x7D 			 /* O2TIHI */
	},
	0x66 			 /* OMODE */
}    /* DEFAULT_TONE */
};

Si3217x_PCM_Cfg Si3217x_PCM_Presets[] ={
	{
	0x00, 	 /* PCM_FMT - A-Law */
	0x00, 	 /* WIDEBAND - DISABLED (3.4kHz BW) */
	0x00, 	 /* PCM_TRI - PCLK RISING EDGE */
	0x00, 	 /* TX_EDGE - PCLK RISING EDGE */
	0x00 	 /* A-LAW -  INVERT NONE */
	}    /* DEFAULT_PCM */
};

