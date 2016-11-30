/*
 * ZLR964124_Le9641_IB_profiles.c --
 *
 * This file contains profile data in byte format
 *
 * Project Info --
 *   File:   C:\Users\Sainz.jeng\Desktop\ZLR964124_Le9641_IB_profiles.vpw
 *   Type:   miSLIC- Le9641 - Buck-Boost Fixed Ringing and Inverting Boost Tracker
 *   Date:   Monday, January 12, 2015 09:21:52
 *   Device: miSLIC Le9641
 *
 *   This file was generated with Profile Wizard Version: P2.7.0
 */

#include "ZLR964124_Le9641_IB_profiles.h"


/************** Device Profile **************/

/* Device Configuration Data - 90V 500Khz IB */
const VpProfileDataType DEV_PROFILE_90V_IB_124[] =
{
/* Profile Header ---------------------------------------------------------- */
   0x0D, 0xFF, 0x00,       /* Device Profile for Le9641 device */
   0x28,                   /* Profile length = 40 + 4 = 44 bytes */
   0x04,                   /* Profile format version = 4 */
   0x14,                   /* MPI section length = 20 bytes */
/* Raw MPI Section --------------------------------------------------------- */
   0x46, 0x02,             /* PCLK = 2.048 MHz; INTM = CMOS-compatible */
   0x44, 0x40,             /* PCM Clock Slot = 0 TX / 0 RX; XE = Pos. */
   0x5E, 0x0C, 0x80,       /* Device Mode Register */
   0xF6, 0x66, 0x00, 0x64, /* Switching Regulator Timing Params */
         0x30, 0x74, 0x30,
   0xE4, 0x04, 0x92, 0x0A, /* Switching Regulator Params */
   0xE6, 0x00,             /* Switching Regulator Control */
/* Formatted Parameters Section -------------------------------------------- */
   0x00,                   /* IO21 Mode: Digital */
                           /* IO22 Mode: Digital */
   0x03,                   /* Dial pulse correction: 0 ms */
                           /* Switcher Configuration =
                              Inverting Boost 500Khz (12 V in, 90 V out) */
   0x00, 0x00,             /* Over-current event threshold = Off */
   0x01,                   /* Leading edge blanking window = 81 ns */
   0x65, 0x00, 0x64, 0x52, /* FreeRun Timing Parameters */
         0x64, 0x52,
   0xFF,
   0x66, 0x00,             /* Low-Power Timing Parameters */
   0x5C, 0x5C,             /* Switcher Limit Voltages 92 V, 92 V */
   0x04,                   /* Charge pump disabled */
                           /* Charge Pump Overload Protection: Disabled */
   0x3C                    /* Switcher Input 12 V */
};


/************** DC Profile **************/

/* DC FXS Default - Use for all countries unless country file exists - 25mA Current Feed */
const VpProfileDataType DC_FXS_miSLIC_IB_DEF[] =
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

/* Default Ringing 25Hz 50Vrms Tracking, AC Trip - Use for all countries unless country profile exists */
const VpProfileDataType RING_ZL880_IB90V_DEF[] =
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

/************** Tone Profile **************/

/************** Cadence Profile **************/

/************** Caller ID Profile **************/

/************** Metering_Profile **************/

/* end of file ZLR964124_Le9641_IB_profiles.c */
