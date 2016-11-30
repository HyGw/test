/*
 * le9641_964124_FH_MTK.h --
 *
 * This header file exports the Profile data types
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

#ifndef LE9641_964124_FH_MTK_H
#define LE9641_964124_FH_MTK_H

#ifdef VP_API_TYPES_H
#include "vp_api_types.h"
#else
typedef unsigned char VpProfileDataType;
#endif


/************** Device Profile **************/
extern const VpProfileDataType ZLR964124_BB100V_DEVICE[];/* Device Configuration Data - 47uH 1.5A Buck-Boost  ZSI 5REN*/
extern const VpProfileDataType DEV_PROFILE_100V_BB_124[];/* Device Configuration Power Limited-47uH 1.5A Buck-Boost*/
extern const VpProfileDataType DEV_PROFILE_100V_BB_124_ZSI[];/* ZSI 4REN*/

/************** DC Profile **************/
extern const VpProfileDataType DC_FXS_miSLIC_BB_CN[];/* China DC FXS Parameters - 25mA Current Feed */
extern const VpProfileDataType DC_FXS_miSLIC_BB_DEF[];/* DC FXS Default - Use for all countries unless country file exists - 25mA Current Feed */

/************** AC Profile **************/
extern const VpProfileDataType AC_FXS_RF14_600R_DEF_LE9641[];/* AC FXS RF14 600R Normal Coefficients (Default)  */
extern const VpProfileDataType AC_FXS_RF14_CN[];     /* AC FXS RF14 China 200R+680R//100nF Normal Coefficients */
extern const VpProfileDataType AC_FXS_RF14_WB_600R_DEF[];/* AC FXS RF14 600R Wideband Coefficients (Default)  */
extern const VpProfileDataType AC_FXS_RF14_WB_CN[];  /* AC FXS RF14 China 200R+680R//100nF Wideband Coefficients */
extern const VpProfileDataType AC_FXS_RF14_150R_RU_LE9641[]; /* AC FXS RF14 Russia 150R+510R 47nF Normal Coefficients */

/************** Ringing Profile **************/
extern const VpProfileDataType RING_miSLIC_BB_China[];/* China Ringing 25Hz 60Vrms, AC Trip */
extern const VpProfileDataType RING_ZL880_BB90V_DEF[];/* Default Ringing 25Hz 50Vrms Tracking, AC Trip - Use for all countries unless country profile exists */


/************** Tone Profile **************/

/************** Cadence Profile **************/

/************** Caller ID Profile **************/

/************** Metering_Profile **************/
#define DEV_PROFILE_BB_124 ZLR964124_BB100V_DEVICE

extern const VpProfileDataType RING_25HZ_TRAP_9641[];	//mtk07059_20150423

#endif /* LE9641_964124_FH_MTK_H */

