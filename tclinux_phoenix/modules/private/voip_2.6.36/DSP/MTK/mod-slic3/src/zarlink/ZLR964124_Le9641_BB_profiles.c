/*
 * le9641_964124_FH_MTK.c --
 *
 * This file contains profile data in byte format
 *
 * Project Info --
 *   File:   D:\Microsemi\profile_dir\le9641_964124_FH_MTK.vpw
 *   Type:   Design for ZLR964124 SM2 Line Module Featuring the Le9641 miSLIC, Lite Coefficients
 *   Date:   Wednesday, April 29, 2015 09:37:50
 *   Device: miSLIC Le9641
 *
 *   This file was generated with Profile Wizard Version: P2.7.0
 *
 * Project Comments --
 *  ----------------------------------------------------------------------------------------------------------------------------------
 *  Profile Wizard Coefficient Revision 2.9 Release Notes:
 *  ----------------------------------------------------------------------------------------------------------------------------------
 *  I. General:
 *  1. This release includes support for the following 44 countries:
 *  Argentina (AR), Austria (AT), Australia (AU), Belgium (BE), Brazil (BR), Bulgaria (BG), Canada (CA), Switzerland (CH),
 *  Chile (CL), China (CN), Czech Republic (CZ), Germany (DE), Denmark (DK), Ecuador (EC), Spain (ES), Finland (FI),
 *  France (FR), UK (GB), Greece (GR), Hong Kong SAR China (HK), Hungary (HU), Indonesia (ID), Ireland (IE), Israel (IL),
 *  India (IN), Iceland (IS), Italy (IT), Japan (JP), S. Korea (KR), Mexico (MX), Malaysia (MY), Netherlands (NL),
 *  Norway (NO), New Zealand (NZ), Poland (PL), Portugal (PT), Russian Federation (RU), Sweden (SE), Singapore (SG),
 *  Thailand (TH), Turkey (TK), Taiwan (TW), USA (US), and South Africa (ZA).
 *  2. The coefficients in this and all releases are provided for use only with the Microsemi VoicePath API-II (VP-API-II). Please refer 
 *  to the terms and conditions for licensing the software regarding terms and conditions of usage. These profiles are provided for 
 *  reference only with no guarantee whatsoever by Microsemi Corporation.
 *  3. This release is for the ZLR964122 SM2 Line Module based on the Le9641.
 *  
 *  II. Device Profile:
 *  1. The default settings for the Device Profile are:
 *         PCLK = 8192 kHz
 *         PCM Transmit Edge = Negative
 *         Transmit Time Slot = 6 (for ZSI)
 *         Receive Time Slot = 0
 *         Interrupt Mode = Open Drain
 *         Switcher = BuckBoost miSLIC (12V in / 100V out)
 *      
 *  2. The settings may be changed by the user as necessary.  Please refer to the miSLIC and VP-API-II documentation for information 
 *  about the supported settings.
 *  
 *  II. DC Profiles:
 *  1. The DC_FXS_MISLIC_BB_DEF Profile is the default unless a country specific profile is selected. Example DC profile settings are
 *  provided for China, ETSI and the USA.
 *  
 *  III. AC Profiles:
 *  1. FXS Coefficients assume -6dBr RX (Output from chipset) and 0dB TX relative gain levels.
 *  2. Supported countries not individually listed should use the default 600R profile AC_FXS_RF14_600R_DEF.
 *  4. AC FXS Coefficients assume the use of two 7 ohm series resistors or PTCs. Customers using other PTC resistance values (such as 
 *  25ohms or 50 ohms) should not use these AC coefficients and can request alternate ones from Microsemi.
 *  5. This release includes both Narrowband and Wideband coefficients. Note that the miSLIC Series devices support per channel Narrowband or
 *  Wideband audio selection.
 *  
 *  
 *  IV. Ring Profiles:
 *  1. RING_MISLIC_BB_DEF is the default ringing profile and should be used for all countries which do not have a listed ringing profile.  
 *  The default ringing profile is set for a sine wave ringing with an amplitude of 50Vrms (70.7Vpk) with no DC bias and a frequency of 25 Hz 
 *  tracking supply.
 *  2. Most ringing profiles on the list are sinusoidal with an amplitude of 50Vrms with no DC bias. 
 *  3. The ringing definitions may be changed based on the requirements of the target market as long as the total amplitude (AC + DC 
 *  components) does not exceed the limits set forth in the Le9641 data sheet.
 *  
 *  
 *  V. Tone Profiles:
 *  1. These profiles are available only in the full version of the VP-API-II.
 *  
 *  VI. Cadence Profiles:
 *  1. These profiles are available only in the full version of the VP-API-II.
 *  
 *  VII. Caller ID Profiles:
 *  1. These profiles are available only in the full version of the VP-API-II.
 *  
 *  VIII. Metering Profiles:
 *  1. These profiles are available only in the full version of the VP-API-II.
 *  
 *  (c) Copyright 2014 Microsemi Corporation. All rights reserved.
 *  
 *  -----------------------------------------------------------------------------------------------------------------------------------------------------
 */

#include "ZLR964124_Le9641_BB_profiles.h"


/************** Device Profile **************/

/* Device Configuration Data - 47uH 1.5A Buck-Boost */
/* ZSI 5REN*/
const VpProfileDataType ZLR964124_BB100V_DEVICE[] =
{
/* Profile Header ---------------------------------------------------------- */
   0x0D, 0xFF, 0x00,       /* Device Profile for Le9641 device */
   0x28,                   /* Profile length = 40 + 4 = 44 bytes */
   0x04,                   /* Profile format version = 4 */
   0x14,                   /* MPI section length = 20 bytes */
/* Raw MPI Section --------------------------------------------------------- */
   0x46, 0x02,             /* PCLK = 2.048 MHz; INTM = CMOS-compatible */
   0x44, 0x46,             /* PCM Clock Slot = 6 TX / 0 RX; XE = Pos. */
   0x5E, 0x14, 0x00,       /* Device Mode Register */
   0xF6, 0x95, 0x00, 0x94, /* Switching Regulator Timing Params */
         0x30, 0x94, 0x30,
   0xE4, 0x44, 0x92, 0x0A, /* Switching Regulator Params */
   0xE6, 0x60,             /* Switching Regulator Control */
/* Formatted Parameters Section -------------------------------------------- */
   0x02,                   /* IO21 Mode: Analog Voltage Sense */
                           /* IO22 Mode: Digital */
   0x20,                   /* Dial pulse correction: 2 ms */
                           /* Switcher Configuration =
                              BuckBoost 47uH 1.5A miSLIC (12 V in, 100 V out) */
   0x00, 0x00,             /* Over-current event threshold = Off */
   0x01,                   /* Leading edge blanking window = 81 ns */
   0x30, 0x14, 0x30, 0x14, /* FreeRun Timing Parameters */
         0x30, 0x14,
   0xFF,
   0x95, 0x00,             /* Low-Power Timing Parameters */
   0x62, 0x62,             /* Switcher Limit Voltages 98 V, 98 V */
   0x04,                   /* Charge pump disabled */
                           /* Charge Pump Overload Protection: Disabled */
   0x3C                    /* Switcher Input 12 V */
};

/* Device Configuration Power Limited-47uH 1.5A Buck-Boost*/

const VpProfileDataType DEV_PROFILE_100V_BB_124[] =
{
/* Profile Header ---------------------------------------------------------- */
   0x0D, 0xFF, 0x00,       /* Device Profile for Le9641 device */
   0x28,                   /* Profile length = 40 + 4 = 44 bytes */
   0x04,                   /* Profile format version = 4 */
   0x14,                   /* MPI section length = 20 bytes */
/* Raw MPI Section --------------------------------------------------------- */
   0x46, 0x02,             /* PCLK = 2.048 MHz; INTM = CMOS-compatible */
   0x44, 0x40,             /* PCM Clock Slot = 0 TX / 0 RX; XE = Pos. */
   0x5E, 0x14, 0x00,       /* Device Mode Register */
   0xF6, 0x95, 0x00, 0x58, /* Switching Regulator Timing Params */
         0x30, 0x5C, 0x30,
   0xE4, 0x44, 0x92, 0x0A, /* Switching Regulator Params */
   0xE6, 0x60,             /* Switching Regulator Control */
/* Formatted Parameters Section -------------------------------------------- */
   0x00,                   /* IO21 Mode: Digital */
                           /* IO22 Mode: Digital */
   0xA0,                   /* Dial pulse correction: 10 ms */
                           /* Switcher Configuration =
                              BuckBoost 47uH 1.5A miSLIC Power Limited(12 V in, 100 V out) */
   0x00, 0x00,             /* Over-current event threshold = Off */
   0x01,                   /* Leading edge blanking window = 81 ns */
   0x30, 0x14, 0x30, 0x14, /* FreeRun Timing Parameters */
         0x30, 0x14,
   0xFF,
   0x95, 0x00,             /* Low-Power Timing Parameters */
   0x62, 0x62,             /* Switcher Limit Voltages 98 V, 98 V */
   0x04,                   /* Charge pump disabled */
                           /* Charge Pump Overload Protection: Disabled */
   0x3C                    /* Switcher Input 12 V */
};

/* ZSI 4REN*/
const VpProfileDataType DEV_PROFILE_100V_BB_124_ZSI[] =
{
/* Profile Header ---------------------------------------------------------- */
   0x0D, 0xFF, 0x00,       /* Device Profile for Le9641 device */
   0x28,                   /* Profile length = 40 + 4 = 44 bytes */
   0x04,                   /* Profile format version = 4 */
   0x14,                   /* MPI section length = 20 bytes */
/* Raw MPI Section --------------------------------------------------------- */
   0x46, 0x02,             /* PCLK = 2.048 MHz; INTM = CMOS-compatible */
   0x44, 0x46,             /* PCM Clock Slot = 6 TX / 0 RX; XE = Pos. */
   0x5E, 0x14, 0x00,       /* Device Mode Register */
   0xF6, 0x95, 0x00, 0x58, /* Switching Regulator Timing Params */
         0x30, 0x5C, 0x30,
   0xE4, 0x44, 0x92, 0x0A, /* Switching Regulator Params */
   0xE6, 0x60,             /* Switching Regulator Control */
/* Formatted Parameters Section -------------------------------------------- */
   0x00,                   /* IO21 Mode: Digital */
                           /* IO22 Mode: Digital */
   0xA0,                   /* Dial pulse correction: 10 ms */
                           /* Switcher Configuration =
                              BuckBoost 47uH 1.5A miSLIC Power Limited(12 V in, 100 V out) */
   0x00, 0x00,             /* Over-current event threshold = Off */
   0x01,                   /* Leading edge blanking window = 81 ns */
   0x30, 0x14, 0x30, 0x14, /* FreeRun Timing Parameters */
         0x30, 0x14,
   0xFF,
   0x95, 0x00,             /* Low-Power Timing Parameters */
   0x62, 0x62,             /* Switcher Limit Voltages 98 V, 98 V */
   0x04,                   /* Charge pump disabled */
                           /* Charge Pump Overload Protection: Disabled */
   0x3C                    /* Switcher Input 12 V */
};


/************** DC Profile **************/

/* China DC FXS Parameters - 25mA Current Feed */
const VpProfileDataType DC_FXS_miSLIC_BB_CN[] =
{
/* Profile Header ---------------------------------------------------------- */
   0x0D, 0x01, 0x00,       /* DC Profile for Le9641 device */
   0x0C,                   /* Profile length = 12 + 4 = 16 bytes */
   0x02,                   /* Profile format version = 2 */
   0x03,                   /* MPI section length = 3 bytes */
/* Raw MPI Section --------------------------------------------------------- */
   0xC6, 0x92, 0x27,       /* DC Feed Parameters: ir_overhead = 100 ohm; */
                           /* VOC = 48 V; LI = 50 ohm; VAS = 8.78 V; ILA = 25 mA */
                           /* Maximum Voice Amplitude = 2.93 V */
/* Formatted Parameters Section -------------------------------------------- */
   0x1A,                   /* Switch Hook Threshold = 9 mA */
                           /* Ground-Key Threshold = 18 mA */
   0x84,                   /* Switch Hook Debounce = 8 ms */
                           /* Ground-Key Debounce = 16 ms */
   0x58,                   /* Low-Power Switch Hook Hysteresis = 2 V */
                           /* Ground-Key Hysteresis = 6 mA */
                           /* Switch Hook Hysteresis = 2 mA */
   0x40,                   /* Low-Power Switch Hook Threshold = 18 V */
   0x04,                   /* Floor Voltage = -25 V */
   0x00,                   /* R_OSP = 0 ohms */
   0x07                    /* R_ISP = 7 ohms */
};

/* DC FXS Default - Use for all countries unless country file exists - 25mA Current Feed */
const VpProfileDataType DC_FXS_miSLIC_BB_DEF[] =
{
/* Profile Header ---------------------------------------------------------- */
   0x0D, 0x01, 0x00,       /* DC Profile for Le9641 device */
   0x0C,                   /* Profile length = 12 + 4 = 16 bytes */
   0x02,                   /* Profile format version = 2 */
   0x03,                   /* MPI section length = 3 bytes */
/* Raw MPI Section --------------------------------------------------------- */
   0xC6, 0x92, 0x27,       /* DC Feed Parameters: ir_overhead = 100 ohm; */
                           /* VOC = 48 V; LI = 50 ohm; VAS = 8.78 V; ILA = 25 mA */
                           /* Maximum Voice Amplitude = 2.93 V */
/* Formatted Parameters Section -------------------------------------------- */
   0x1C,                   /* Switch Hook Threshold = 11 mA */
                           /* Ground-Key Threshold = 18 mA */
   0x84,                   /* Switch Hook Debounce = 8 ms */
                           /* Ground-Key Debounce = 16 ms */
   0x58,                   /* Low-Power Switch Hook Hysteresis = 2 V */
                           /* Ground-Key Hysteresis = 6 mA */
                           /* Switch Hook Hysteresis = 2 mA */
   0x80,                   /* Low-Power Switch Hook Threshold = 22 V */
   0x02,                   /* Floor Voltage = -15 V */
   0x00,                   /* R_OSP = 0 ohms */
   0x07                    /* R_ISP = 7 ohms */
};


/************** AC Profile **************/

/* AC FXS RF14 600R Normal Coefficients (Default)  */
const VpProfileDataType AC_FXS_RF14_600R_DEF_LE9641[] =
{
  /* AC Profile */
 0xA4, 0x00, 0xF4, 0x4C, 0x01, 0x49, 0xCA, 0xF5, 0x98, 0xAA, 0x7B, 0xAB,
 0x2C, 0xA3, 0x25, 0xA5, 0x24, 0xB2, 0x3D, 0x9A, 0x2A, 0xAA, 0xA6, 0x9F,
 0x01, 0x8A, 0x1D, 0x01, 0xA3, 0xA0, 0x2E, 0xB2, 0xB2, 0xBA, 0xAC, 0xA2,
 0xA6, 0xCB, 0x3B, 0x45, 0x88, 0x2A, 0x20, 0x3C, 0xBC, 0x4E, 0xA6, 0x2B,
 0xA5, 0x2B, 0x3E, 0xBA, 0x8F, 0x82, 0xA8, 0x71, 0x80, 0xA9, 0xF0, 0x50,
 0x00, 0x86, 0x2A, 0x42, 0xA1, 0xCB, 0x1B, 0xA3, 0xA8, 0xFB, 0x87, 0xAA,
 0xFB, 0x9F, 0xA9, 0xF0, 0x96, 0x2E, 0x01, 0x00
};

/* AC FXS RF14 China 200R+680R//100nF Normal Coefficients */
const VpProfileDataType AC_FXS_RF14_CN[] =
{
  /* AC Profile */
 0xB9, 0x00, 0x1F, 0x4C, 0x01, 0x49, 0xCA, 0xE9, 0x98, 0x5C, 0xB1, 0xAA,
 0x23, 0x43, 0xEB, 0x32, 0xAA, 0x23, 0xB3, 0x9A, 0x98, 0x94, 0x97, 0x9F,
 0x01, 0x8A, 0x2D, 0x01, 0x22, 0x30, 0x6A, 0x59, 0x2A, 0xCB, 0xB4, 0xB2,
 0xBF, 0x3B, 0xA3, 0x25, 0x88, 0x43, 0xC0, 0x3A, 0x45, 0x2B, 0xBD, 0x4C,
 0x2E, 0x4C, 0x25, 0x42, 0xAD, 0x82, 0xA8, 0x71, 0x80, 0xA9, 0xF0, 0x50,
 0x00, 0x86, 0xA4, 0x5A, 0x32, 0xB5, 0x12, 0xA1, 0xA2, 0x3A, 0xA4, 0xB5,
 0x64, 0x97, 0xAA, 0xF0, 0x96, 0x2E, 0x01, 0x00
};

/* AC FXS RF14 600R Wideband Coefficients (Default)  */
const VpProfileDataType AC_FXS_RF14_WB_600R_DEF[] =
{
  /* AC Profile */
 0xED, 0x00, 0xA9, 0x4C, 0x01, 0x49, 0xCA, 0xFC, 0x98, 0xCC, 0x2A, 0xA2,
 0x3B, 0x98, 0xB3, 0x43, 0xA2, 0x3A, 0xAB, 0x9A, 0x2D, 0xAA, 0xB6, 0x9F,
 0x01, 0x8A, 0x2E, 0x01, 0xC4, 0xA0, 0x2E, 0x23, 0x3A, 0x2E, 0x9F, 0x87,
 0x4C, 0xCE, 0xB2, 0x27, 0x88, 0x4A, 0x20, 0xBA, 0xAC, 0x22, 0xB5, 0x5A,
 0xB5, 0x3B, 0x4D, 0xA2, 0x27, 0x82, 0xA8, 0x71, 0x80, 0xA9, 0xF0, 0x50,
 0x00, 0x86, 0xCB, 0x5E, 0x22, 0xBC, 0x12, 0xA3, 0xA9, 0xFA, 0xC6, 0x34,
 0xE2, 0x9F, 0xBA, 0xF0, 0x96, 0x2E, 0x01, 0x00
};

/* AC FXS RF14 China 200R+680R//100nF Wideband Coefficients */
const VpProfileDataType AC_FXS_RF14_WB_CN[] =
{
  /* AC Profile */
 0xEF, 0x00, 0x46, 0x4C, 0x01, 0x49, 0xCA, 0xDA, 0x98, 0xB3, 0xB1, 0x23,
 0x32, 0x64, 0x97, 0xAA, 0x2B, 0x4A, 0xA4, 0x9A, 0x01, 0x90, 0x01, 0x01,
 0x90, 0x8A, 0x2E, 0x01, 0xCD, 0x10, 0x2B, 0x29, 0x22, 0x2A, 0xA2, 0x33,
 0x4B, 0x4C, 0xBD, 0xF6, 0x88, 0x2A, 0xC0, 0x52, 0x97, 0xF2, 0x2E, 0xAA,
 0x4E, 0xAC, 0xB5, 0x5B, 0xBE, 0x82, 0xA8, 0x71, 0x80, 0xA9, 0xF0, 0x50,
 0x00, 0x86, 0x2A, 0x5A, 0xA2, 0xBA, 0x1B, 0x22, 0xCA, 0x24, 0xC3, 0x43,
 0x42, 0xA4, 0x4B, 0x50, 0x96, 0x3C, 0x41, 0x00
};


/* AC FXS RF14 Russia 150R+510R 47nF Normal Coefficients */
const VpProfileDataType AC_FXS_RF14_150R_RU_LE9641[] =
{
  /* AC Profile */
 0x00, 0x00, 0x70, 0x4C, 0x01, 0x49, 0xCA, 0xED, 0x98, 0x98, 0x13, 0xB3,
 0xA4, 0xAB, 0xAC, 0xBB, 0xEC, 0x2B, 0x25, 0x9A, 0x3E, 0xAA, 0xA7, 0x9F,
 0x01, 0x8A, 0x2D, 0x01, 0xA8, 0x70, 0x2C, 0x2A, 0x7A, 0xC3, 0x23, 0x2D,
 0x22, 0xA6, 0xAB, 0x87, 0x88, 0x65, 0x40, 0xBC, 0xB6, 0x22, 0xAD, 0x2A,
 0xAF, 0x22, 0xA5, 0x32, 0xBE, 0x82, 0xF8, 0x71, 0x80, 0xA9, 0xF0, 0x50,
 0x00, 0x86, 0xE3, 0x5A, 0x22, 0xAC, 0x14, 0xB2, 0x34, 0x53, 0xAF, 0xDA,
 0xFA, 0x9F, 0xA9, 0xF0, 0x96, 0x2E, 0x01, 0x00
};

/************** Ringing Profile **************/

/* China Ringing 25Hz 60Vrms, AC Trip */
const VpProfileDataType RING_miSLIC_BB_China[] =
{
/* Profile Header ---------------------------------------------------------- */
   0x0D, 0x04, 0x00,       /* Ringing Profile for Le9641 device */
   0x12,                   /* Profile length = 18 + 4 = 22 bytes */
   0x01,                   /* Profile format version = 1 */
   0x0C,                   /* MPI section length = 12 bytes */
/* Raw MPI Section --------------------------------------------------------- */
   0xC0, 0x08, 0x00, 0x00, /* Ringing Generator Parameters: */
         0x00, 0x44, 0x46, /* 24.9 Hz Sine; 1.41 CF; 84.80 Vpk; 0.00 V bias */
         0x4D, 0x00, 0x00, /* Ring trip cycles = 1; RTHALFCYC = 0 */
         0x00, 0x00,
/* Formatted Parameters Section -------------------------------------------- */
   0xB2,                   /* Ring Trip Method = AC; Threshold = 25.0 mA */
   0x05,                   /* Ringing Current Limit = 60 mA */
   0x50,                   /* Fixed; Max Ringing Supply = 85 V */
   0x00                    /* Balanced; Ring Cadence Control Disabled */
};

/*[OSBNB00045371] ADD by mtk07059_20150423 , support ring parameters set. */
const VpProfileDataType RING_25HZ_TRAP_9641[] =
{
/* Profile Header ---------------------------------------------------------- */
   0x0D, 0x04, 0x00,       /* Ringing Profile for Le9661 device */
   0x12,                   /* Profile length = 18 + 4 = 22 bytes */
   0x01,                   /* Profile format version = 1 */
   0x0C,                   /* MPI section length = 12 bytes */
/* Raw MPI Section --------------------------------------------------------- */
   0xC0, 0x09, 0x00, 0x00, /* Ringing Generator Parameters: */
         0x00, 0xB5, 0x42, /* 24.9 Hz Trap; 1.42 CF; 80.00 Vpk; 0.00 V bias */
         0x52, 0x01, 0x41, /* Ring trip cycles = 1; RTHALFCYC = 0 */
         0x00, 0x00,
/* Formatted Parameters Section -------------------------------------------- */
   0xA8,                   /* Ring Trip Method = AC; Threshold = 20.0 mA */
   0x00,                   /* Ringing Current Limit = 50 mA */
   0x4F,                   /* Fixed; Max Ringing Supply = 80 V */
   0x00                    /* Balanced; Ring Cadence Control Disabled */
};

/* Default Ringing 25Hz 50Vrms Tracking, AC Trip - Use for all countries unless country profile exists */
const VpProfileDataType RING_ZL880_BB90V_DEF[] =
{
/* Profile Header ---------------------------------------------------------- */
   0x0D, 0x04, 0x00,       /* Ringing Profile for Le9641 device */
   0x12,                   /* Profile length = 18 + 4 = 22 bytes */
   0x01,                   /* Profile format version = 1 */
   0x0C,                   /* MPI section length = 12 bytes */
/* Raw MPI Section --------------------------------------------------------- */
   0xC0, 0x08, 0x00, 0x00, /* Ringing Generator Parameters: */
         0x00, 0x44, 0x3A, /* 24.9 Hz Sine; 1.41 CF; 70.70 Vpk; 0.00 V bias */
         0x9D, 0x00, 0x00, /* Ring trip cycles = 1; RTHALFCYC = 0 */
         0x00, 0x00,
/* Formatted Parameters Section -------------------------------------------- */
   0xAA,                   /* Ring Trip Method = AC; Threshold = 21.0 mA */
   0x02,                   /* Ringing Current Limit = 54 mA */
   0x0E,                   /* Tracked; Max Ringing Supply = 75 V */
   0x00                    /* Balanced; Ring Cadence Control Disabled */
};

/************** Tone Profile **************/

/************** Cadence Profile **************/

/************** Caller ID Profile **************/

/************** Metering_Profile **************/

/* end of file le9641_964124_FH_MTK.c */
