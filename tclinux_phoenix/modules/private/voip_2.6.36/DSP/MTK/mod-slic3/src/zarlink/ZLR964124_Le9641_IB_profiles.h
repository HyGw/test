/*
 * ZLR964124_Le9641_IB_profiles.h --
 *
 * This header file exports the Profile data types
 *
 * Project Info --
 *   File:   C:\Users\Sainz.jeng\Desktop\ZLR964124_Le9641_IB_profiles.vpw
 *   Type:   miSLIC- Le9641 - Buck-Boost Fixed Ringing and Inverting Boost Tracker
 *   Date:   Monday, January 12, 2015 09:21:52
 *   Device: miSLIC Le9641
 *
 *   This file was generated with Profile Wizard Version: P2.7.0
 */

#ifndef ZLR964124_LE9641_IB_PROFILES_H
#define ZLR964124_LE9641_IB_PROFILES_H

#ifdef VP_API_TYPES_H
#include "vp_api_types.h"
#else
typedef unsigned char VpProfileDataType;
#endif


/************** Device Profile **************/
extern const VpProfileDataType DEV_PROFILE_90V_IB_124[];/* Device Configuration Data - 90V 500Khz IB */


/************** DC Profile **************/
extern const VpProfileDataType DC_FXS_miSLIC_IB_DEF[];/* DC FXS Default - Use for all countries unless country file exists - 25mA Current Feed */

/************** AC Profile **************/
extern const VpProfileDataType AC_FXS_RF14_600R_DEF_LE9641[];/* AC FXS RF14 600R Normal Coefficients (Default)  */

extern const VpProfileDataType AC_FXS_RF14_150R_RU_LE9641[];/* AC FXS RF14 Russia 150R+510R 47nF Normal Coefficients */

/************** Ringing Profile **************/
extern const VpProfileDataType RING_ZL880_IB90V_DEF[];/* Default Ringing 25Hz 50Vrms Tracking, AC Trip - Use for all countries unless country profile exists */

extern const VpProfileDataType RING_25HZ_TRAP_9641[];	//mtk07059_20150423

/************** Tone Profile **************/

/************** Cadence Profile **************/

/************** Caller ID Profile **************/

/************** Metering_Profile **************/

#endif /* ZLR964124_LE9641_IB_PROFILES_H */

