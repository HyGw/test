/*
 * vpp_hbi.h
 *
 * This file declares the Host Bus Interface (application layer) for
 * VPP class of devices.
 *
 * Copyright (c) 2005, Legerity Inc.
 * All rights reserved
 *
 * This software is the property of Legerity , Inc. Please refer to the
 * Non Disclosure Agreement (NDA) that you have signed for more information
 * on legal obligations in using, modifying or distributing this file.
 */


#ifndef _VPP_HBI_H
#define _VPP_HBI_H

#include "hbi_common.h"	/* Brings in the common definitions that are applicable
                         * for VPP and DVP classes of devices */
#include "vp_hal.h"		/* Brings in HBI - HAL layer definitions */

#ifndef VPP_MAX_NUM_LINES
#define VPP_MAX_NUM_LINES		(2)
#endif /* VPP_MAX_NUM_LINES */

#define VPP_CMD_PAGE_MAX_LEN		(128)/* Maximum length of the VPP class
										  * command mailbox (page 0) length. */

/* VPP HBI Pages; These are also used as mailboxes */
typedef enum {
    VPP_HBI_CMDMB_PAGE,
    VPP_HBI_RSPMB_PAGE,
    VPP_HBI_CH1US_PAGE,
    VPP_HBI_CH2US_PAGE,
    VPP_HBI_CH1DS_PAGE,
    VPP_HBI_CH2DS_PAGE,
    VPP_HBI_CH12US_PAGE,
    VPP_HBI_CH22US_PAGE,
    VPP_HBI_CH12DS_PAGE,
    VPP_HBI_CH22DS_PAGE,
    VPP_HBI_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VppHbiPageId;

/* The following constants define registers accessible on the direct page */
/*      Register				    page,  offset,  words - 1 */
#define VPP_Reg_MAILBOX_MASK        VP_DP,  0x0B,   0
#define VPP_Reg_GLBIO_RD	        VP_DP,  0x0C,   0
#define VPP_Reg_GLBIO_WR	        VP_DP,  0x0D,   0
#define VPP_Reg_TIMESTAMP	        VP_DP,  0x0F,   0
#define VPP_Reg_CHNL_0_STATE	    VP_DP,  0x10,   0
#define VPP_Reg_CHNL_1_STATE	    VP_DP,  0x11,   0
#define VPP_Reg_LINE_0_STATUS       VP_DP,  0x12,   0
#define VPP_Reg_LINE_1_STATUS       VP_DP,  0x13,   0
#define VPP_Reg_DEVICE_EVENT_MASK      VP_DP,  0x14,   0
#define VPP_Reg_CHNL_0_EVENT_MASK   VP_DP,  0x18,   0
#define VPP_Reg_CHNL_1_EVENT_MASK   VP_DP,  0x1C,   0
#define VPP_Reg_LINE_0_TEST_RESLT   VP_DP,  0x20,   0
#define VPP_Reg_LINE_1_TEST_RESLT   VP_DP,  0x21,   0

/* VPP specific API function prototypes */
#define VPP_DEVPRF_PCLK_MSBI			(8)
#define VPP_DEVPRF_PCLK_LSBI			(9)

/* VPP Application model Commands */
typedef enum {
    VPP_CMD_RSVD,
    VPP_CMD_SET_PROFILE,
    VPP_CMD_GET_PROFILE,
    VPP_CMD_SELECT_PROFILE,
    VPP_CMD_SET_OPTION,
    VPP_CMD_GET_OPTION,
    VPP_CMD_GET_STATUS,
    VPP_CMD_SET_STATE,
    VPP_CMD_START,
    VPP_CMD_STOP,
    VPP_CMD_SET_LLOPTION,
    VPP_CMD_GET_LLOPTION,
    VPP_CMD_RST_CHANNEL,
    VPP_CMD_TEST,
    VPP_CMD_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VppMbCmdIdType;

/* VPP Application model Get Status sub commands; */
typedef enum {
    VPP_SCMD_GS_VERSION,
    VPP_SCMD_GS_LINE_COND,
    VPP_SCMD_GS_BAT_VOL,
    VPP_SCMD_GS_PKT_STS,
    VPP_SCMD_GS_LD_PRF,
    VPP_SCMD_GS_TEST_RSLT,
    VPP_SCMD_GS_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VppSCmdGetStatusIdType;

/* VPP Application model Set option sub commands; */
typedef enum {
    VPP_SCMD_SO_DS_GAIN,
    VPP_SCMD_SO_US_GAIN,
    VPP_SCMD_SO_DEVICE_IO,
    VPP_SCMD_SO_RSVD_1,
    VPP_SCMD_SO_US_TRNSCODER,
    VPP_SCMD_SO_DS_TRNSCODER,
    VPP_SCMD_SO_EC,
    VPP_SCMD_SO_TD_COEFF,
    VPP_SCMD_SO_CLR_PKT_CNT,
    VPP_SCMD_SO_LOOPBACK,
    VPP_SCMD_SO_RSVD_2,
    VPP_SCMD_SO_BAT_VOL_THRESH,
    VPP_SCMD_SO_RSVD_3,
    VPP_SCMD_SO_DTMF_LVL,
    VPP_SCMD_SO_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VppSCmdSetOptionIdType;

/* VPP specific profile types */
typedef enum {
    VPP_PROFILE_AC,
    VPP_PROFILE_DC,
    VPP_PROFILE_TONE,
    VPP_PROFILE_TONECAD,
    VPP_PROFILE_RING_AND_CAD,
    VPP_PROFILE_CID_TYPE1,
    VPP_PROFILE_CID_TYPE2,
    VPP_PROFILE_METER,
    VPP_PROFILE_RSVD,
	VPP_PROFILE_TEST_CRITERIA,
	VPP_PROFILE_TEST_TOPOLOGY,
    VPP_NUM_PROFILE_TYPES,
    VPP_PROFILE_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VppProfileType;

/* VPP Application model Start/Stop command sub commands; */
typedef enum {
    VPP_SCMD_STSP_DTMF_GEN,
    VPP_SCMD_STSP_TONE_GEN,
    VPP_SCMD_STSP_SEQNCR,
    VPP_SCMD_STSP_METERING,
    VPP_SCMD_STSP_TONE_DET,
    VPP_SCMD_STSP_DTMF_DET,
    VPP_SCMD_STSP_3WAY_CONF,
    VPP_SCMD_STSP_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VppSCmdStartStopIdType;

typedef enum {
    VPP_SCMD_STRT_IN_DS = 0,
    VPP_SCMD_STRT_IN_US	= 0x20,
    VPP_SCMD_STRT_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VppSCmdStartStopDirectionIdType;

/* Number of entries in the profile table */
#define VPP_DEV_PROF_MAX_CNT				(0)
#define VPP_AC_PROF_MAX_CNT					(2)
#define VPP_DC_PROF_MAX_CNT					(2)
#define VPP_TONE_PROF_MAX_CNT				(2) /* (4) */
#define VPP_TONECAD_PROF_MAX_CNT			(4)
#define VPP_MTR_PROF_MAX_CNT				(2)
#define VPP_CID_TYPE1_PROF_MAX_CNT			(1)
#define VPP_CID_TYPE2_PROF_MAX_CNT			(1)
#define VPP_TEST_SETUP_PROF_MAX_CNT			(1)
#define VPP_TEST_TOPOLOGY_PROF_MAX_CNT		(1)

/* Length of various profile HBI commands (in words) */
#define VPP_SELECT_PRF_LEN				(2)			/* Command header */
#define VPP_MAX_PROFILE_LEN				(0)

#define VPP_AC_PRF_LEN					(2 + 42)	/* Set profile
* command (1w) + profile length (1w) + Profile data (84 bytes)*/
#if    (VPP_AC_PRF_LEN > VPP_MAX_PROFILE_LEN)
#undef  VPP_MAX_PROFILE_LEN
#define VPP_MAX_PROFILE_LEN				VPP_AC_PRF_LEN
#endif

#define VPP_DC_PRF_LEN					(2 + 24)
#if    (VPP_DC_PRF_LEN > VPP_MAX_PROFILE_LEN)
#undef  VPP_MAX_PROFILE_LEN
#define VPP_MAX_PROFILE_LEN				VPP_DC_PRF_LEN
#endif

#define VPP_RING_PRF_LEN				(2 + 8)
#if    (VPP_RING_PRF_LEN > VPP_MAX_PROFILE_LEN)
#undef  VPP_MAX_PROFILE_LEN
#define VPP_MAX_PROFILE_LEN				VPP_RING_PRF_LEN
#endif

#define VPP_RINGCAD_PRF_LEN				(0 + (23-8))
#if    (VPP_RINGCAD_PRF_LEN > VPP_MAX_PROFILE_LEN)
#undef  VPP_MAX_PROFILE_LEN
#define VPP_MAX_PROFILE_LEN				VPP_RINGCAD_PRF_LEN
#endif

#define VPP_TONE_PRF_LEN				(2 + 11)
#if    (VPP_TONE_PRF_LEN > VPP_MAX_PROFILE_LEN)
#undef  VPP_MAX_PROFILE_LEN
#define VPP_MAX_PROFILE_LEN				VPP_TONE_PRF_LEN
#endif

#define VPP_TONECAD_PRF_LEN				(2 + 23)
#if    (VPP_TONECAD_PRF_LEN > VPP_MAX_PROFILE_LEN)
#undef  VPP_MAX_PROFILE_LEN
#define VPP_MAX_PROFILE_LEN				VPP_TONECAD_PRF_LEN
#endif

#define VPP_METER_PRF_LEN				(2 + 6)
#if    (VPP_METER_PRF_LEN > VPP_MAX_PROFILE_LEN)
#undef  VPP_MAX_PROFILE_LEN
#define VPP_MAX_PROFILE_LEN				VPP_METER_PRF_LEN
#endif

#define VPP_CID_PRF_LEN					(2 + 48)
#if    (VPP_CID_PRF_LEN > VPP_MAX_PROFILE_LEN)
#undef  VPP_MAX_PROFILE_LEN
#define VPP_MAX_PROFILE_LEN				VPP_CID_PRF_LEN
#endif

#define VPP_TESTTOPOLGY_PRF_LEN			(2 + 18)
#if    (VPP_TESTTOPOLGY_PRF_LEN > VPP_MAX_PROFILE_LEN)
#undef  VPP_MAX_PROFILE_LEN
#define VPP_MAX_PROFILE_LEN				VPP_TESTTOPOLGY_PRF_LEN
#endif

#define VPP_TEST_PRF_LEN				(2 + 18)
#if    (VPP_TEST_PRF_LEN > VPP_MAX_PROFILE_LEN)
#undef  VPP_MAX_PROFILE_LEN
#define VPP_MAX_PROFILE_LEN				VPP_TEST_PRF_LEN
#endif

/* Macro that is used to make a VPP application command */
#define MAKE_VPP_CMD(brdcst, chnl, cmd, subCmd)	\
    (((brdcst)<<15) + ((chnl)<<12) + ((cmd)<<6) + (subCmd))

/* VPP device specific command macros */
#define VPP_SET_PRF_CMD(type)					\
    MAKE_VPP_CMD(0, 0, VPP_CMD_SET_PROFILE, type)
#define VPP_SELECT_PRF_CMD(brdcst, chnl, type)	\
    MAKE_VPP_CMD(brdcst, chnl, VPP_CMD_SELECT_PROFILE, type)
#define VPP_SET_STATE_CMD(brdcst, chnl, state)	\
    MAKE_VPP_CMD(brdcst, chnl, VPP_CMD_SET_STATE, state)
#define VPP_SET_STATE_CMD_LEN					0
#define VPP_START_TONE_GEN_CH(chnl) 			\
    MAKE_VPP_CMD(0, chnl, VPP_CMD_START, VPP_SCMD_STSP_SEQNCR + \
        VPP_SCMD_STRT_IN_DS) /* DevNotes: start the sequencer instead
        * of tone for now VPP_SCMD_STRT_TONE_GEN) */
#define VPP_STOP_TONE_GEN_CH(chnl)  			\
    MAKE_VPP_CMD(0, chnl, VPP_CMD_STOP, VPP_SCMD_STSP_SEQNCR + \
        VPP_SCMD_STRT_IN_DS)  /* DevNotes: start the sequencer
        * instead of tone for now VPP_SCMD_STRT_TONE_GEN) */
#define VPP_START_DTMF_DET(brdcst, chnl) 		\
    MAKE_VPP_CMD(brdcst, chnl, VPP_CMD_START, VPP_SCMD_STSP_DTMF_DET + \
                    VPP_SCMD_STRT_IN_US)
#define VPP_START_3WAY_CONFERENCE(chnl)             \
    MAKE_VPP_CMD(0, chnl, VPP_CMD_START, VPP_SCMD_STSP_3WAY_CONF)
#define VPP_STOP_3WAY_CONFERENCE(chnl)             \
    MAKE_VPP_CMD(0, chnl, VPP_CMD_STOP, VPP_SCMD_STSP_3WAY_CONF)

/* Macros for setting options */
#define VPP_SETOPT_US_CODEC(brdcst, chnl) 		\
    MAKE_VPP_CMD(brdcst, chnl, VPP_CMD_SET_OPTION, VPP_SCMD_SO_US_TRNSCODER)
#define VPP_SETOPT_US_CODEC_LEN					(2)
#define VPP_SETOPT_DS_CODEC(brdcst, chnl) 		\
    MAKE_VPP_CMD(brdcst, chnl, VPP_CMD_SET_OPTION, VPP_SCMD_SO_DS_TRNSCODER)
#define VPP_SETOPT_DS_CODEC_LEN					(2)
#define VPP_SETOPT_ECHO_CNCL(brdcst, chnl) 		\
    MAKE_VPP_CMD(brdcst, chnl, VPP_CMD_SET_OPTION, VPP_SCMD_SO_EC)
#define VPP_SETOPT_ECHO_CNCL_LEN    			(1)
#define VPP_SETOPT_CLR_PKT_CNTR(brdcst, chnl) 	\
    MAKE_VPP_CMD(brdcst, chnl, VPP_CMD_SET_OPTION, VPP_SCMD_SO_CLR_PKT_CNT)
#define VPP_SETOPT_CLR_PKT_CNTR_LEN    			(1)

/* Marcro's for getting status information */
/* Get Version */
#define VPP_GETSTS_VERSION_CMD					\
    MAKE_VPP_CMD(0, 0, VPP_CMD_GET_STATUS, VPP_SCMD_GS_VERSION)
#define VPP_GETSTS_VERSION_CMD_LEN				0
#define VPP_GETSTS_VERSION_RSP_LEN				2

/* Get Line Conditions */
#define VPP_GETSTS_LINE_COND_CMD(chnl)			\
    MAKE_VPP_CMD(0, chnl, VPP_CMD_GET_STATUS, VPP_SCMD_GS_LINE_COND)
#define VPP_GETSTS_LINE_COND_CMD_LEN			0
#define VPP_GETSTS_LINE_COND_RSP_LEN			5

/* Get Battery Voltages */
#define VPP_GETSTS_BAT_VOLT_CMD(chnl)			\
    MAKE_VPP_CMD(0, chnl, VPP_CMD_GET_STATUS, VPP_SCMD_GS_BAT_VOL)
#define VPP_GETSTS_BAT_VOLT_CMD_LEN				0
#define VPP_GETSTS_BAT_VOLT_RSP_LEN				3

/* Get Packet Statistics */
#define VPP_GETSTS_PKT_STATS_CMD(chnl)			\
    MAKE_VPP_CMD(0, chnl, VPP_CMD_GET_STATUS, VPP_SCMD_GS_PKT_STS)
#define VPP_GETSTS_PKT_STATS_CMD_LEN			0
#define VPP_GETSTS_PKT_STATIS_RSP_LEN			8


/* VPP device Line state type */
typedef enum {
	VPP_LINEST_NO_SERVICE		= 0x00,
	VPP_LINEST_IDLE_LS 			= 0x01,
	VPP_LINEST_IDLE_GS 			= 0x11,		/* DevNotes: This is not yet
	                                         * implemented in iMARS */
	VPP_LINEST_ACTIVE			= 0x02,
	VPP_LINEST_ACTIVE_POLREV	= 0x22,
    VPP_LINEST_RINGING        	= 0x03,
    VPP_LINEST_RINGING_POLREV	= 0x23,
    VPP_LINEST_OHT				= 0x04,
    VPP_LINEST_OHT_POLREV		= 0x24,
 	VPP_LINEST_TALK				= 0x05,
 	VPP_LINEST_TALK_POLREV		= 0x25,
 	VPP_LINEST_TEST				= 0x07,
    VPP_LINEST_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VppLineStateType;

/* Vpp Specific event types */
typedef enum {
    VPP_EVID_INIT_CMP         	= 0x00,
    VPP_EVID_MAILBOX_RDY       	= 0x01,
    VPP_EVID_TIMESTAMP_ROLL     = 0x02,
    VPP_EVID_CMD_FAILED	        = 0x03,
    VPP_EVID_LINE_FLT	        = 0x40,
    VPP_EVID_GROUND_KEY	        = 0x41,
    VPP_EVID_HOOK_SWITCH        = 0x42,
    VPP_EVID_FLASH_HOOK	        = 0x43,
    VPP_EVID_PULSE_DIGIT        = 0x44,
    VPP_EVID_DTMF_DIGIT_DET     = 0x45,
    VPP_EVID_TONE_DET_US        = 0x46,
    VPP_EVID_TONE_DET_DS        = 0x47,
    VPP_EVID_SEQUNCR_EV	        = 0x48,
    VPP_EVID_PKT_MISS	        = 0x4A,
    VPP_EVID_PKT_TYPE_ERR       = 0x4B,
    VPP_EVID_TEST_CMP	        = 0x4C,

	/* System events */
    VPP_EVID_SYS_EV_OV_MASK    	= 0xC000,
    VPP_EVID_SYS_WDT_MASK    	= 0xA000,
    VPP_EVID_SYS_CFAIL_MASK    	= 0x9000,
    VPP_EVID_SYS_CFAILGT20_MASK = 0x8100,
    VPP_EVID_SYS_PINT_MASK    	= 0x8080,
    VPP_EVID_SYS_LINT_MASK    	= 0x8040,
    VPP_EVID_SYS_HINT_MASK    	= 0x8020,
    VPP_EVID_SYS_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
}VppEventIdType;

#endif /* _VPP_HBI_H */




