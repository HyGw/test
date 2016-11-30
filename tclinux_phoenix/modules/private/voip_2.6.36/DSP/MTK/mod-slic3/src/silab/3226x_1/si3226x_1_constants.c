/*
** Copyright (c) 2012 Silicon Laboratories, Inc.
** 2013-09-11 14:19:30
**
** Si3226x ProSLIC API Configuration Tool Version 3.0.0
*/


#include "proslic.h"
#include "si3226x.h"

Si3226x_General_Cfg Si3226x_General_Configuration  = {
0x64,     /* DEVICE_KEY */
BO_DCDC_PMOS_BUCK_BOOST,    /* BOM_OPT */
BO_DCDC_TRACKING,    /* BAT_RAIL_TYPE */
0x000AD000L,    /* bat_hyst */
0x070A3D3AL,    /* VBATR_EXPECT (110.00) */
0x03D70A20L,    /* VBATH_EXPECT (60.00) */
0x0FFF0000L,    /* PWRSAVE_TIMER */
0x01999A00L,    /* OFFHOOK_THRESH */
0x00F00000L,    /* VBAT_TRACK_MIN */
0x00F00000L,    /* VBAT_TRACK_MIN_RNG */
0x00014000L,    /* PWRSAVE_DBI */
0x0ADD5500L,    /* DCDC_ANA_SCALE */
0x00312700L,    /* VOV_BAT_PWRSAVE_MIN */
0x00395800L,    /* VOV_BAT_PWRSAVE_MIN_RNG */
0x00800000L,    /* THERM_DBI */
0x00004000L,    /* CPUMP_DBI */
0x00F18900L,    /* DCDC_VERR */
0x00809D80L,    /* DCDC_VERR_HYST */
0x00000000L,    /* DCDC_OITHRESH_LO */
0x00D00000L,    /* DCDC_OITHRESH_HI */
0x00400000L,    /* PD_UVLO */
0x00400000L,    /* PD_OVLO */
0x00200000L,    /* PD_OCLO */
0x00400000L,    /* PD_SWDRV */
0x00500000L,    /* DCDC_UVHYST */
0x00000000L,    /* DCDC_UVTHRESH */
0x00D00000L,    /* DCDC_OVTHRESH */
0x00800000L,    /* DCDC_OITHRESH */
0x00100000L,    /* DCDC_SWDRV_POL */
0x00000000L,    /* DCDC_UVPOL */
0x00000000L,    /* DCDC_VREF_MAN */
0x00000000L,    /* DCDC_VREF_CTRL */
0x00200000L,    /* DCDC_RNGTYPE */
0x05B00000L,    /* DCDC_ANA_VREF */
0x00300000L,    /* DCDC_ANA_GAIN */
0x00300000L,    /* DCDC_ANA_TOFF */
0x00100000L,    /* DCDC_ANA_TONMIN */
0x01F00000L,    /* DCDC_ANA_TONMAX */
0x00F00000L,    /* DCDC_ANA_DSHIFT */
0x0FDA4000L,    /* DCDC_ANA_LPOLY */
0x00000000L,    /* DCDC_AUX_INVERT */
0x00400000L,    /* DCDC_CPUMP_LP */
0x00000000L,    /* DCDC_CPUMP_PULLDOWN */
0x00000000L,    /* DCDC_LIFT_EN */
0x07FE4B00L,    /* COEF_P_HVIC */
0x0048D15BL,    /* P_TH_HVIC */
VDC_4P5_16P0,    /* VDC_RANGE_OPT */
0x3F,     /* AUTO */
0x00,     /* IRQEN1 */
0x00,     /* IRQEN2 */
0x07,     /* IRQEN3 */
0x00,     /* IRQEN4 */
0x10,     /* ENHANCE */
0x03A2E8BAL,    /* SCALE_KAUDIO */
0x04,    /* ZCAL_EN */
0x03000000L,    /* LKG_OFHK_OFFSET */
0x05000000L,    /* LKG_LB_OFFSET */
0x01000000L,    /* VBATH_DELTA */
0x03700000L,    /* UVTHRESH_MAX */
0x04B80200L,    /* UVTHRESH_SCALE */
0x00823000L     /* UVTHRESH_BIAS */
};

Si3226x_GPIO_Cfg Si3226x_GPIO_Configuration = {
0x00,     /* GPIO_OE */
0x00,     /* GPIO_ANA */
0x00,     /* GPIO_DIR */
0x00,     /* GPIO_MAN */
0x00,     /* GPIO_POL */
0x00,     /* GPIO_OD */
0x00     /* BATSELMAP */
};
Si3226x_CI_Cfg Si3226x_CI_Presets [] = {
{0}
};
Si3226x_audioGain_Cfg Si3226x_audioGain_Presets [] = {
{0x1377080L,0, 0x0L, 0x0L, 0x0L, 0x0L},
{0x80C3180L,0, 0x0L, 0x0L, 0x0L, 0x0L}
};

Si3226x_Ring_Cfg Si3226x_Ring_Presets[] ={
{
/*
	Loop = 500.0 ft @ 0.044 ohms/ft, REN = 5, Rcpe = 600 ohms
	Rprot = 30 ohms, Type = LPR, Waveform = SINE
*/ 
0x00050000L,	/* RTPER */
0x07EFE000L,	/* RINGFR (20.000 Hz) */
0x00208847L,	/* RINGAMP (53.000 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x00D16348L,	/* IRING_LIM (90.000 mA) */
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
0x01893740L,	/* VBAT_TRACK_MIN_RNG */
0x80,			/* RINGCON */
0x00,			/* USERSTAT */
0x02AC55FEL,	/* VCM_RING (38.769 v) */
0x02AC55FEL,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00200000L,	/* DCDC_RNGTYPE */
0x00FFFFFFL,	/* VOV_DCDC_SLOPE */
0x00A18937L,	/* VOV_DCDC_OS */
0x00E49BA5L,	/* VOV_RING_BAT_MAX */
}    /* DEFAULT_RINGING */
};

Si3226x_DCfeed_Cfg Si3226x_DCfeed_Presets[] = {
{
0x1C8A024CL,	/* SLOPE_VLIM */
0x1F909679L,	/* SLOPE_RFEED */
0x0040A0E0L,	/* SLOPE_ILIM */
0x1D5B21A9L,	/* SLOPE_DELTA1 */
0x1DD87A3EL,	/* SLOPE_DELTA2 */
0x05A38633L,	/* V_VLIM (48.000 v) */
0x050D2839L,	/* V_RFEED (43.000 v) */
0x03FE7F0FL,	/* V_ILIM  (34.000 v) */
0x00B4F3C3L,	/* CONST_RFEED (15.000 mA) */
0x005D0FA6L,	/* CONST_ILIM (20.000 mA) */
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
0x00415F45L,	/* VOV_GND (3.990 v) */
}    /* DEFAULT_DCFEED */
};

Si3226x_Impedance_Cfg Si3226x_Impedance_Presets[] ={
/* Source: Database file: cwdb.db */
/* Database information: version: 1.1.0 build date: 2013-01-08*/
/* parameters: zref=600_0_0 rprot=20 rfuse=0 emi_cap=10*/
{
{0x07F97D80L, 0x0006CC00L, 0x1FFC1480L, 0x1FFC7B80L,    /* TXACEQ */
 0x07F36B80L, 0x000A8E00L, 0x1FF90F00L, 0x1FFAE500L},   /* RXACEQ */
{0x001AF400L, 0x1FC86A80L, 0x01E9AE00L, 0x00652F00L,    /* ECFIR/ECIIR */
 0x01F4AF00L, 0x1F57E000L, 0x00485E00L, 0x1FF3A680L,
 0x1FF83700L, 0x00011D00L, 0x01706980L, 0x066A8480L},
{0x00920F00L, 0x1EE31980L, 0x008ADF00L, 0x0F92E500L,    /* ZSYNTH */
 0x186CE880L, 0x53}, 
 0x085C6880L,   /* TXACGAIN */
 0x013E3100L,   /* RXACGAIN */
 0x07AF6F80L, 0x18509100L, 0x075EDF00L,    /* RXACHPF */
#ifdef ENABLE_HIRES_GAIN
 0, 0  /* TXGAIN*10, RXGAIN*10 (hi_res) */
#else
 0, 0  /* TXGAIN, RXGAIN */
#endif
 }    /* DEFAULT_IMPEDANCE */
};

Si3226x_FSK_Cfg Si3226x_FSK_Presets[] ={
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

Si3226x_PulseMeter_Cfg Si3226x_PulseMeter_Presets[] ={
{
0x007A2B6AL,  /* PM_AMP_THRESH (1.000) */
0,            /* Freq (12kHz) */ 
0,            /* PM_AUTO (off)*/
0x07D00000L,  /* PM_active (2000 ms) */
0x07D00000L   /* PM_inactive (2000 ms) */
 }    /* DEFAULT_PULSE_METERING */
};

Si3226x_Tone_Cfg Si3226x_Tone_Presets[] = {
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

Si3226x_PCM_Cfg Si3226x_PCM_Presets[] ={
	{
	0x00, 	 /* PCM_FMT - A-Law */
	0x00, 	 /* WIDEBAND - DISABLED (3.4kHz BW) */
	0x00, 	 /* PCM_TRI - PCLK RISING EDGE */
	0x00, 	 /* TX_EDGE - PCLK RISING EDGE */
	0x00 	 /* A-LAW -  INVERT NONE */
	}    /* DEFAULT_PCM */
};

