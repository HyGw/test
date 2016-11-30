/***************************************************************
Copyright Statement:

This software/firmware and related documentation (EcoNet Software) 
are protected under relevant copyright laws. The information contained herein 
is confidential and proprietary to EcoNet (HK) Limited (EcoNet) and/or 
its licensors. Without the prior written permission of EcoNet and/or its licensors, 
any reproduction, modification, use or disclosure of EcoNet Software, and 
information contained herein, in whole or in part, shall be strictly prohibited.

EcoNet (HK) Limited  EcoNet. ALL RIGHTS RESERVED.

BY OPENING OR USING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY 
ACKNOWLEDGES AND AGREES THAT THE SOFTWARE/FIRMWARE AND ITS 
DOCUMENTATIONS (ECONET SOFTWARE) RECEIVED FROM ECONET 
AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON AN AS IS 
BASIS ONLY. ECONET EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, 
WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, 
OR NON-INFRINGEMENT. NOR DOES ECONET PROVIDE ANY WARRANTY 
WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTIES WHICH 
MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE ECONET SOFTWARE. 
RECEIVER AGREES TO LOOK ONLY TO SUCH THIRD PARTIES FOR ANY AND ALL 
WARRANTY CLAIMS RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES 
THAT IT IS RECEIVERS SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD 
PARTY ALL PROPER LICENSES CONTAINED IN ECONET SOFTWARE.

ECONET SHALL NOT BE RESPONSIBLE FOR ANY ECONET SOFTWARE RELEASES 
MADE TO RECEIVERS SPECIFICATION OR CONFORMING TO A PARTICULAR 
STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND 
ECONET'S ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE ECONET 
SOFTWARE RELEASED HEREUNDER SHALL BE, AT ECONET'S SOLE OPTION, TO 
REVISE OR REPLACE THE ECONET SOFTWARE AT ISSUE OR REFUND ANY SOFTWARE 
LICENSE FEES OR SERVICE CHARGES PAID BY RECEIVER TO ECONET FOR SUCH 
ECONET SOFTWARE.
***************************************************************/

/************************************************************************
*                  I N C L U D E S
************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <asm/tc3162/cmdparse.h>
#include <libapi_gponmgr_internal.h>

/************************************************************************
*                  D E F I N E S   &   C O N S T A N T S
************************************************************************/

/************************************************************************
*                  M A C R O S
************************************************************************/

/************************************************************************
*                  D A T A	 T Y P E S
************************************************************************/

/************************************************************************
*                  E X T E R N A L	 D A T A   D E C L A R A T I O N S
************************************************************************/
extern int api_lib_help(int argc, char *argv[], void *p);

/************************************************************************
*                  F U N C T I O N   D E F I N I T I O N S
************************************************************************/
int do_get_dbg_lv(int argc, char *argv[], void *p);
int do_get_gemport(int argc, char *argv[], void *p);
int do_get_gemcnt(int argc, char *argv[], void *p);
int do_get_gemcnt_all(void);
int do_get_tod_info(int argc, char *argv[], void *p);
int do_get_tcont(int argc, char *argv[], void *p);
int do_get_eqd_off(int argc, char *argv[], void *p);
int do_get_spf(int argc, char *argv[], void *p);
int do_get_cnt_type(int argc, char *argv[], void *p);
int do_get_rsp_time(int argc, char *argv[], void *p);
int do_get_itl_dly_fine_tune(int argc, char *argv[], void *p);
int do_get_burst_mode_overhead(int argc, char *argv[], void *p);
int do_get_key_info(int argc, char *argv[], void *p);
int do_get_block_size(int argc, char *argv[], void *p);
int do_get_dying_gasp(int argc, char *argv[], void *p);
int do_get_up_traffic(int argc, char *argv[], void *p);
int do_get_sniffer(int argc, char *argv[], void *p);
int do_get_csr(int argc, char *argv[], void *p);
int do_get_gem_info(int argc, char *argv[], void *p);
int do_get_tcont_info(int argc, char *argv[], void *p);
int do_get_filter(int argc, char *argv[], void *p);
int do_get_dba_backdoor(int argc, char *argv[], void *p);
int do_get_dba_slight_modify(int argc, char *argv[], void *p);
int do_get_o3_o4_ploam(int argc, char *argv[], void *p);
int do_get_tx_sync_off(int argc, char *argv[], void *p);
int do_get_interrupt(int argc, char *argv[], void *p);
int do_get_idle_gem(int argc, char *argv[], void *p);
#ifdef TCSUPPORT_CPU_EN7521
int do_get_1pps_h_w(int argc, char *argv[], void *p);
int do_get_send_ploamu_wait_mode(int argc, char *argv[], void *p);
#endif

/************************************************************************
*                  P U B L I C   D A T A
************************************************************************/

/************************************************************************
*                  P R I V A T E   D A T A
************************************************************************/
const cmds_t gponmgr_get_command[] = {
/*	name,			func,								flags,	argcmin,	argc_errmsg */
	{"dbg_lv",				do_get_dbg_lv,				0,		0,			"Get the GPON MAC debug level"},
	{"gemport", 			do_get_gemport, 			0,		0,			"Get the GPON MAC GEM port information"},
	{"gemcnt",				do_get_gemcnt,				0,		1,			"Get the GPON MAC GEM counter information <all | GEM port id>"},
	{"tod_info",			do_get_tod_info, 			0,		0,			"Get the current time of ONU MAC"},
	{"tcont",				do_get_tcont,				0,		0,			"Get the GPON MAC TCONT information"},
	{"eqd_off",				do_get_eqd_off,				0,		0,			"Get the EqD offset detail information"},
	{"spf",					do_get_spf,					0,		0,			"Get the GPON MAC super frame counter"},
	{"cnt_type",			do_get_cnt_type,			0,		0,			"Get the GPON MAC counter type"},
	{"rsp_time",			do_get_rsp_time,			0,		0,			"Get the GPON MAC response time"},
	{"itl_dly_ftune",		do_get_itl_dly_fine_tune,	0,		0,			"Get the GPON MAC internal delay fine tune(BFB64028[15:8])"},
	{"bst_m_ovhead",		do_get_burst_mode_overhead,	0,		0,			"Get the GPON MAC burst mode overhead supports more than 128bytes status"},
	{"key_info",			do_get_key_info,			0,		0,			"Get the encryption key info"},
	{"block_size",			do_get_block_size,			0,		0,			"Get the GPON DBA block size"},
	{"dying_gasp",			do_get_dying_gasp,			0,		0,			"Get the GPON Dying Gasp parameters"},
	{"up_traffic",			do_get_up_traffic,			0,		0,			"Get the software upstream traffic status"},
	{"sniffer",				do_get_sniffer,				0,		0,			"Get the GPON sniffer detail informations"},
	{"csr",					do_get_csr,					0,		0,			"Show the GPON control and status registers"},
	{"gem_info",			do_get_gem_info,			0,		0,			"Show the GPON hardware gemport table information"},
	{"tcont_info",			do_get_tcont_info,			0,		0,			"Show the GPON hardware tcont table information"},
	{"filter",				do_get_filter,				0,		0,			"Get the GPON filter detail information"},
	{"dbru_fix",			do_get_dba_backdoor,		0,		0,			"Get the GPON DBA backdoor detail information"},
	{"dbru_offset",			do_get_dba_slight_modify,	0,		0,			"Get the GPON DBA slight modify detail information"},
	{"o3_o4_ploam",			do_get_o3_o4_ploam,			0,		0,			"Get o3 and o4 ploam control status"},
	{"tx_sync_off",			do_get_tx_sync_off,			0,		0,			"Get tx sync offset"},
	{"interrupt",			do_get_interrupt,			0,		0,			"Get GPON MAC interrupt mask"},
	{"idle_gem",			do_get_idle_gem,				0,		0,			"Get GPON MAC idle gem threshold"},
#ifdef TCSUPPORT_CPU_EN7521
	{"1pps_h_w",			do_get_1pps_h_w,				0,		0,			"Get the high width of a 1pps pluse"},
	{"ploamu_w_m",			do_get_send_ploamu_wait_mode,	0,		0,			"Get the send ploamu wait mode"},
#endif
	/* Cmd should be added before this line!  */
	{"help",				api_lib_help,				0,		0,			"Show gponmgr get usage."},
	{NULL, NULL, 0, 0, NULL}
};

/************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
************************************************************************/

/******************************************************************************
 Descriptor:	It's used to get the GPON MAC debug level.
 Input Args:	see gponmgr_get_command.
 Ret Value:	none.
******************************************************************************/
int do_get_dbg_lv(int argc, char *argv[], void *p)
{
	ushort gponDbgLevel;

#if API_CMD_GPONMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(gponmgr_lib_get_dbg_level(&gponDbgLevel) == ECONET_E_FAIL) {
		printf("Exec. Failed: Get the 'dbg_level' command failed\n") ;
		return ECONET_E_FAIL;
	}
	
	printf("Debug level:\n") ;
	printf("     act: %s\n", (gponDbgLevel & MSG_ACT) ? "enable" : "disable") ;
	printf("    omci: %s\n", (gponDbgLevel & MSG_OMCI) ? "enable" : "disable") ;
	printf("     oam: %s\n", (gponDbgLevel & MSG_OAM) ? "enable" : "disable") ;
	printf("     int: %s\n", (gponDbgLevel & MSG_INT) ? "enable" : "disable") ;
	printf("   trace: %s\n", (gponDbgLevel & MSG_TRACE) ? "enable" : "disable") ;
	printf(" warning: %s\n", (gponDbgLevel & MSG_WARN) ? "enable" : "disable") ;
	printf(" content: %s\n", (gponDbgLevel & MSG_CONTENT) ? "enable" : "disable") ;
	printf("   debug: %s\n", (gponDbgLevel & MSG_DBG) ? "enable" : "disable") ;
	printf("     err: %s\n", (gponDbgLevel & MSG_ERR) ? "enable" : "disable") ;
	printf("     EqD: %s\n", (gponDbgLevel & MSG_EQD) ? "enable" : "disable") ;
	printf("    XMCS: %s\n", (gponDbgLevel & MSG_XMCS) ? "enable" : "disable") ;
	
	return ECONET_E_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to get the GPON GEM port information.
 Input Args:	see gponmgr_get_command.
 Ret Value:	success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int do_get_gemport(int argc, char *argv[], void *p)
{
	struct XMCS_GemPortInfo_S gponGemInfo ;
	int i ;

#if API_CMD_GPONMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	memset(&gponGemInfo, 0, sizeof(gponGemInfo)) ;
	if(gponmgr_lib_get_gemport(&gponGemInfo) == ECONET_E_FAIL) {
		printf("Exec. Failed: Get the 'gemport' command failed\n") ;
		return ECONET_E_FAIL;
	}
	
	printf("GPON GEM Port information, Entries:%d:\n", gponGemInfo.entryNum) ;
	for(i=0 ; i < gponGemInfo.entryNum ; i++) {
		printf(" %.2d  %.9s GEM Port:%4d, TCONT:%4d,", i + 1, (gponGemInfo.info[i].gemType == GPON_MULTICAST_GEM) ? "Multicast" : "Unicast",
														gponGemInfo.info[i].gemPortId, gponGemInfo.info[i].allocId) ;
		
		if(gponGemInfo.info[i].aniIdx >= 0 && gponGemInfo.info[i].aniIdx < GPON_MAX_ANI_INTERFACE) {
			printf(" MAC If:pon%d,", gponGemInfo.info[i].aniIdx) ;
		} else if(gponGemInfo.info[i].aniIdx == GPON_OMCI_INTERFACE) {
			printf(" MAC If:omci,") ;
		} else {
			printf(" MAC If:drop,") ;	
		}
		
		printf(" Loopback:%s, Encryption:%s\n", (gponGemInfo.info[i].lbMode == XPON_ENABLE)?"Enable":"Disable", 
												(gponGemInfo.info[i].enMode == XPON_ENABLE)?"Enable":"Disable") ; 
	}

	return ECONET_E_SUCCESS;
}


/******************************************************************************
 Descriptor:	It's used to get the GPON GEM counter information.
 Input Args:	see gponmgr_get_command.
 Ret Value:	success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int do_get_gemcnt(int argc, char *argv[], void *p)
{
	struct XMCS_GponGemCounter_S gponGemCounter ;
	unsigned int value ;

#if API_CMD_GPONMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(strncmp(argv[1], "all", strlen("all")) == 0) {
		return do_get_gemcnt_all();
	}

	value = strtoul(argv[1], NULL, 0) ;
	if(value >= 4096) {
		printf("Input Error: gem port id error.\n") ;
		return ECONET_E_FAIL;
	}
	
	memset(&gponGemCounter, 0, sizeof(gponGemCounter)) ;
	gponGemCounter.gemPortId = value ;
	if(gponmgr_lib_get_gemcounter(&gponGemCounter) == ECONET_E_FAIL) {
		printf("Exec. Failed: Get the 'gemcounter' command failed\n\n") ;
		return ECONET_E_FAIL;
	}

	printf("Gem Port:%d\n", gponGemCounter.gemPortId);
	printf("GemFrame Rx High:%u, GemFrame Rx Low:%u\n", gponGemCounter.rxGemFrameH, gponGemCounter.rxGemFrameL) ;
	printf("GemPayload Rx High:%u, GemPayload Rx Low:%u\n", gponGemCounter.rxGemPayloadH, gponGemCounter.rxGemPayloadL) ;
	printf("GemFrame Tx High:%u, GemFrame Tx Low:%u\n", gponGemCounter.txGemFrameH, gponGemCounter.txGemFrameL) ;
	printf("GemPayload Tx High:%u, GemPayload Tx Low:%u\n", gponGemCounter.txGemPayloadH, gponGemCounter.txGemPayloadL) ;

	return ECONET_E_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to get the GPON all GEM counter information.
 Input Args:	see gponmgr_get_command.
 Ret Value:	success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int do_get_gemcnt_all(void)
{
	struct XMCS_GponGemCounter_S gponGemCounter ;
	struct XMCS_GemPortInfo_S gponGemInfo ;
	int i ;

#if API_CMD_GPONMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	memset(&gponGemInfo, 0, sizeof(gponGemInfo)) ;
	if(gponmgr_lib_get_gemport(&gponGemInfo) == ECONET_E_FAIL) {
		printf("Exec. Failed: Get the 'gemport' command failed\n") ;
		return ECONET_E_FAIL;
	}

	printf("GPON GEM Port information, Entries:%d:\n", gponGemInfo.entryNum) ;
	for(i=0 ; i < gponGemInfo.entryNum ; i++) {
		memset(&gponGemCounter, 0, sizeof(gponGemCounter)) ;
		gponGemCounter.gemPortId = gponGemInfo.info[i].gemPortId ;
		if(gponmgr_lib_get_gemcounter(&gponGemCounter) == ECONET_E_FAIL) {
			printf("Exec. Failed: Get the 'gemcounter' command failed\n\n") ;
			return ECONET_E_FAIL;
		}
	
		printf(" Gem Port:%d\n", gponGemCounter.gemPortId);
		printf(" GemFrame Rx High:%u, GemFrame Rx Low:%u\n", gponGemCounter.rxGemFrameH, gponGemCounter.rxGemFrameL) ;
		printf(" GemPayload Rx High:%u, GemPayload Rx Low:%u\n", gponGemCounter.rxGemPayloadH, gponGemCounter.rxGemPayloadL) ;
		printf(" GemFrame Tx High:%u, GemFrame Tx Low:%u\n", gponGemCounter.txGemFrameH, gponGemCounter.txGemFrameL) ;
		printf(" GemPayload Tx High:%u, GemPayload Tx Low:%u\n", gponGemCounter.txGemPayloadH, gponGemCounter.txGemPayloadL) ;
	}
	return ECONET_E_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to get the ToD detial information.
 Input Args:	see gponmgr_get_command.
 Ret Value:	success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int do_get_tod_info(int argc, char *argv[], void *p)
{
	struct XMCS_GponTodCfg_S gponTodCfg ;

#if API_CMD_GPONMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	memset(&gponTodCfg, 0, sizeof(gponTodCfg)) ;
	if(gponmgr_lib_get_tod_switch_time(&gponTodCfg) == ECONET_E_FAIL) {
		printf("Exec. Failed: Get the 'tod_info' command failed\n") ;
		return ECONET_E_FAIL;
	}

	printf("Current ToD Information:\n") ;
	printf(" Switch time:        0x%.8x\n", gponTodCfg.superframe) ;

	if(gponmgr_lib_get_current_tod(&gponTodCfg) == ECONET_E_FAIL) {
		printf("Exec. Failed: Get the 'tod_info' command failed\n") ;
		return ECONET_E_FAIL;
	}
	
	printf(" Current second:     %u\n", gponTodCfg.sec) ;
	printf(" Current nanosecond: %u\n", gponTodCfg.nanosec) ;

	if(gponmgr_lib_get_new_tod(&gponTodCfg) == ECONET_E_FAIL) {
		printf("Exec. Failed: Get the 'tod_info' command failed\n") ;
		return ECONET_E_FAIL;
	}
	
	printf(" New second:         %u\n", gponTodCfg.sec) ;
	printf(" New nanosecond:     %u\n", gponTodCfg.nanosec) ;
	
	return ECONET_E_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to get the GPON TCONT information.
 Input Args:	see gponmgr_get_command.
 Ret Value:	success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int do_get_tcont(int argc, char *argv[], void *p)
{
	struct XMCS_TcontInfo_S gponTcontInfo ;
	int i ;

#if API_CMD_GPONMGR_DBG
	printf("enter %s.\n", __func__);
#endif
	
	memset(&gponTcontInfo, 0, sizeof(gponTcontInfo)) ;
	if(gponmgr_lib_get_tcont(&gponTcontInfo) == ECONET_E_FAIL) {
		printf("Exec. Failed: Get the 'tcont' command failed\n") ;
		return ECONET_E_FAIL;
	}
	
	printf("GPON TCONT information:\n") ;
	for(i=0 ; i<gponTcontInfo.entryNum ; i++) {
		printf(" %.2d   TCONT ID:%d, Channel:%d\n", i + 1, gponTcontInfo.info[i].allocId, gponTcontInfo.info[i].channel) ; 
	}
	
	return ECONET_E_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to get the EqD detail information.
 Input Args:	see gponmgr_get_command.
 Ret Value:	success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int do_get_eqd_off(int argc, char *argv[], void *p)
{
	struct XMCS_EqdOffset_S gponEqdOffset ;

#if API_CMD_GPONMGR_DBG
	printf("enter %s.\n", __func__);
#endif
	
	memset(&gponEqdOffset, 0, sizeof(gponEqdOffset)) ;
	if(gponmgr_lib_get_eqd_offset(&gponEqdOffset) == ECONET_E_FAIL) {
		printf("Exec. Failed: Get the 'eqd_offset' command failed\n") ;
		return ECONET_E_FAIL;
	}
	
	printf("EqD offset:\n") ;
	printf(" flag:%s\n", (gponEqdOffset.eqdOffsetFlag == GPON_EQD_OFFSET_FLAG_ADD) ? "+" : "-") ;
	printf(" O4:%u\n", gponEqdOffset.O4) ;
	printf(" O5:%u\n", gponEqdOffset.O5) ;
	
	return ECONET_E_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to get GPON super frame counter.
 Input Args:	see gponmgr_get_command.
 Ret Value:	success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int do_get_spf(int argc, char *argv[], void *p)
{
	uint spf ;

#if API_CMD_GPONMGR_DBG
	printf("enter %s.\n", __func__);
#endif
	
	if(gponmgr_lib_get_super_frame_counter(&spf) == ECONET_E_FAIL) {
		printf("Exec. Failed: Get the 'spf' command failed\n") ;
		return ECONET_E_FAIL;
	}
	
	printf("Super frame counter:%.8x\n", spf) ;
	
	return ECONET_E_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to get GPON MAC counter type.
 Input Args:	see gponmgr_get_command.
 Ret Value:	success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int do_get_cnt_type(int argc, char *argv[], void *p)
{
	GPON_COUNTER_TYPE_t type ;

#if API_CMD_GPONMGR_DBG
	printf("enter %s.\n", __func__);
#endif
	
	if(gponmgr_lib_get_counter_type(&type) == ECONET_E_FAIL) {
		printf("Exec. Failed: Get the 'cnt_type' command failed\n") ;
		return ECONET_E_FAIL;
	}

	printf("Counter type:%s\n", (type == GPON_COUNTER_TYPE_GEM) ? "GEM" : "Ethernet") ;
	
	return ECONET_E_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to get GPON response time.
 Input Args:	see gponmgr_get_command.
 Ret Value:	success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int do_get_rsp_time(int argc, char *argv[], void *p)
{
	ushort time ;

#if API_CMD_GPONMGR_DBG
	printf("enter %s.\n", __func__);
#endif
	
	if(gponmgr_lib_get_response_time(&time) == ECONET_E_FAIL) {
		printf("Exec. Failed: Get the 'rsp_time' command failed\n") ;
		return ECONET_E_FAIL;
	}

	printf("Response time:0x%.4x\n", time) ;
	
	return ECONET_E_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to get the GPON MAC internal delay fine tune(BFB64028[15:8]).
 Input Args:	see gponmgr_get_command.
 Ret Value:	success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int do_get_itl_dly_fine_tune(int argc, char *argv[], void *p)
{
	unsigned char delay ;

#if API_CMD_GPONMGR_DBG
	printf("enter %s.\n", __func__);
#endif
	
	if(gponmgr_lib_get_internal_delay_fine_tune(&delay) == ECONET_E_FAIL) {
		printf("Exec. Failed: Get the 'itl_dly_fine_tune' command failed\n") ;
		return ECONET_E_FAIL;
	}

	printf("Internal delay fine tune:0x%.2x\n", delay) ;
	
	return ECONET_E_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to get the GPON MAC burst mode overhead supports more than 
                 128bytes status.
 Input Args:	see gponmgr_get_command.
 Ret Value:	success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int do_get_burst_mode_overhead(int argc, char *argv[], void *p)
{
	GPON_BURST_MODE_OVERHEAD_LEN_T type ;

#if API_CMD_GPONMGR_DBG
	printf("enter %s.\n", __func__);
#endif
	
	if(gponmgr_lib_get_burst_mode_overhead(&type) == ECONET_E_FAIL) {
		printf("Exec. Failed: Get the 'burst_mode_overhead' command failed\n") ;
		return ECONET_E_FAIL;
	}

	if(type == GPON_BURST_MODE_OVERHEAD_LEN_DEFAULT) {
		printf("Burst mode overhead length does not support more than 128 bytes\n") ;
	} else {
		printf("Burst mode overhead length supports more than 128 bytes\n") ;
	}
	
	return ECONET_E_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to get the encryption key info.
 Input Args:	see gponmgr_get_command.
 Ret Value:	success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int do_get_key_info(int argc, char *argv[], void *p)
{
	GPON_DEV_ENCRYPT_KEY_INFO_T keyInfo;

#if API_CMD_GPONMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	memset(&keyInfo, 0, sizeof(keyInfo)) ;
	if(gponmgr_lib_get_key_info(&keyInfo) == ECONET_E_FAIL) {
		printf("Exec. Failed: Get the 'key_info' command failed\n") ;
		return ECONET_E_FAIL;
	}

	printf("Current key Information:\n") ;
	
	/* Show active super frame counter */
	printf("  Key switch time    :0x%08x\n", keyInfo.aesSpf) ;

	/* Show SHADOW key */
	printf("  Encrypt shadow key0:0x%08x\n", keyInfo.shadowKey[0]) ;
	printf("  Encrypt shadow key1:0x%08x\n", keyInfo.shadowKey[1]) ;
	printf("  Encrypt shadow key2:0x%08x\n", keyInfo.shadowKey[2]) ;
	printf("  Encrypt shadow key3:0x%08x\n", keyInfo.shadowKey[3]) ;

	/* Show active key */
	printf("  Encrypt active key0:0x%08x\n", keyInfo.activeKey[0]) ;
	printf("  Encrypt active key1:0x%08x\n", keyInfo.activeKey[1]) ;
	printf("  Encrypt active key2:0x%08x\n", keyInfo.activeKey[2]) ;
	printf("  Encrypt active key3:0x%08x\n", keyInfo.activeKey[3]) ;

	return ECONET_E_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to get the GPON DBA block size.
 Input Args:	see gponmgr_get_command.
 Ret Value:	success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int do_get_block_size(int argc, char *argv[], void *p)
{
	ushort blockSize;

#if API_CMD_GPONMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(gponmgr_lib_get_dba_block_size(&blockSize) == ECONET_E_FAIL) {
		printf("Exec. Failed: Get the 'block_size' command failed\n") ;
		return ECONET_E_FAIL;
	}

	printf("Block size:0x%.2x\n", blockSize) ;
	
	return ECONET_E_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to get the GPON Dying Gasp parameters.
 Input Args:	see gponmgr_get_command.
 Ret Value:	success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int do_get_dying_gasp(int argc, char *argv[], void *p)
{
	GPON_SW_HW_SELECT_T mode;
	uint num;
	
#if API_CMD_GPONMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(gponmgr_lib_get_dying_gasp_mode(&mode) == ECONET_E_FAIL) {
		printf("Exec. Failed: Get the 'dying_gasp' command failed\n") ;
		return ECONET_E_FAIL;
	}

	if(gponmgr_lib_get_dying_gasp_num(&num) == ECONET_E_FAIL) {
		printf("Exec. Failed: Get the 'dying_gasp' command failed\n") ;
		return ECONET_E_FAIL;
	}

	printf("Dying Gasp\n") ;
	printf("  Mode:%s\n", (mode == GPON_SW) ? "software" : "hardware") ;
	printf("  Number:%u\n", num) ;

	return ECONET_E_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to get the software upstream traffic enable or disable.
 Input Args:	see gponmgr_get_command.
 Ret Value:	success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int do_get_up_traffic(int argc, char *argv[], void *p)
{
	GPON_DEV_UP_TRAFFIC_T up_traffic;
	
#if API_CMD_GPONMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(gponmgr_lib_get_up_traffic(&up_traffic) == ECONET_E_FAIL) {
		printf("Exec. Failed: Get the 'up_traffic' command failed\n") ;
		return ECONET_E_FAIL;
	}

	switch(up_traffic.omci) {
		case XPON_ENABLE:
			printf("Software path omci tx : enable\n") ;
			break;
		case XPON_DISABLE:
			printf("Software path omci tx : disable\n") ;
			break;
		default:
			printf("Get software path error!\n") ;
			return ECONET_E_FAIL;
	}

	switch(up_traffic.data) {
		case XPON_ENABLE:
			printf("Software path data tx : enable\n") ;
			break;
		case XPON_DISABLE:
			printf("Software path data tx : disable\n") ;
			break;
		default:
			printf("Get software path error!\n") ;
			return ECONET_E_FAIL;
	}
	
	return ECONET_E_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to get the GPON sniffer detail informations.
 Input Args:	see gponmgr_get_command.
 Ret Value:	success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int do_get_sniffer(int argc, char *argv[], void *p)
{
	GPON_DEV_SNIFFER_MODE_T sniffer;
	
#if API_CMD_GPONMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	memset(&sniffer, 0, sizeof(sniffer)) ;
	if(gponmgr_lib_get_sniffer_gtc(&sniffer) == ECONET_E_FAIL) {
		printf("Exec. Failed: Get the 'sniffer' command failed\n") ;
		return ECONET_E_FAIL;
	}

	printf("Sniffer mode:\n") ;
	printf("  Upstream DMAC:       0x%.4x\n", sniffer.tx_da) ;
	printf("  Upstream SMAC:       0x%.4x\n", sniffer.tx_sa) ;
	printf("  Upstream ethertype:  0x%.4x\n", sniffer.tx_ethertype) ;
	printf("  Upstream VID:        0x%.4x\n", sniffer.tx_vid) ;
	printf("  Upstream TPID:       0x%.4x\n", sniffer.tx_tpid) ;
	printf("  Downstream DMAC:     0x%.4x\n", sniffer.rx_da) ;
	printf("  Downstream SMAC:     0x%.4x\n", sniffer.rx_sa) ;
	printf("  Downstream ethertype:0x%.4x\n", sniffer.rx_ethertype) ;
	printf("  Downstream VID:      0x%.4x\n", sniffer.rx_vid) ;
	printf("  Downstream TPID:     0x%.4x\n", sniffer.rx_tpid) ;
	printf("  Sniffer lan port:    ") ;
	
	switch(sniffer.lan_port) {
		case SNIFFER_MODE_LAN0:
			printf("0\n") ;
			break;
		case SNIFFER_MODE_LAN1:
			printf("1\n") ;
			break;
		case SNIFFER_MODE_LAN2:
			printf("2\n") ;
			break;
		case SNIFFER_MODE_LAN3:
			printf("3\n") ;
			break;
		default:
			printf("unknow\n") ;
			break;
	}

	printf("  Packet padding:      %s\n", (sniffer.packet_padding == XPON_ENABLE) ? "enable" : "disable") ;
	printf("  Sniffer mode:        %s\n", (sniffer.enable== XPON_ENABLE) ? "enable" : "disable") ;
	
	return ECONET_E_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to show the GPON control and status registers.
 Input Args:	see gponmgr_get_command.
 Ret Value:	success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int do_get_csr(int argc, char *argv[], void *p)
{
#if API_CMD_GPONMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(gponmgr_lib_get_csr() == ECONET_E_FAIL) {
		printf("Exec. Failed: Get the 'csr' command failed\n") ;
		return ECONET_E_FAIL;
	}
	
	return ECONET_E_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to show the GPON hardware gemport table information.
 Input Args:	see gponmgr_get_command.
 Ret Value:	success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int do_get_gem_info(int argc, char *argv[], void *p)
{
#if API_CMD_GPONMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(gponmgr_lib_get_gem_info() == ECONET_E_FAIL) {
		printf("Exec. Failed: Get the 'csr' command failed\n") ;
		return ECONET_E_FAIL;
	}
	
	return ECONET_E_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to show the GPON hardware tcont table information.
 Input Args:	see gponmgr_get_command.
 Ret Value:	success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int do_get_tcont_info(int argc, char *argv[], void *p)
{
#if API_CMD_GPONMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(gponmgr_lib_get_tcont_info() == ECONET_E_FAIL) {
		printf("Exec. Failed: Get the 'csr' command failed\n") ;
		return ECONET_E_FAIL;
	}
	
	return ECONET_E_SUCCESS;
}


/******************************************************************************
 Descriptor:	It's used to get the GPON filter detail information.
 Input Args:	see gponmgr_get_command.
 Ret Value:	success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int do_get_filter(int argc, char *argv[], void *p)
{
	XPON_Mode_t enable;
	
#if API_CMD_GPONMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(gponmgr_lib_get_ext_bst_len_ploamd_filter(&enable) == ECONET_E_FAIL) {
		printf("Exec. Failed: Get the 'filter' command failed\n") ;
		return ECONET_E_FAIL;
	}

	printf("Filter:\n");
	printf("  extented burst length:%s\n", (enable== XPON_ENABLE) ? "enable" : "disable");

	if(gponmgr_lib_get_up_overhead_ploamd_filter(&enable) == ECONET_E_FAIL) {
		printf("Exec. Failed: Get the 'filter' command failed\n") ;
		return ECONET_E_FAIL;
	}

	printf("      upstream overhead:%s\n", (enable == XPON_ENABLE) ? "enable" : "disable");
	
	return ECONET_E_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to get the GPON DBA backdoor detail information.
 Input Args:	see gponmgr_get_command.
 Ret Value:	success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int do_get_dba_backdoor(int argc, char *argv[], void *p)
{
	GPON_DEV_DBA_BACKDOOR_T dba_backdoor;
	
#if API_CMD_GPONMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	memset(&dba_backdoor, 0, sizeof(dba_backdoor)) ;
	if(gponmgr_lib_get_dba_backdoor(&dba_backdoor) == ECONET_E_FAIL) {
		printf("Exec. Failed: Get the 'dbru_fix' command failed\n") ;
		return ECONET_E_FAIL;
	}

	printf("DBA backdoor:\n");
	printf("  enable:     %s\n", (dba_backdoor.enable == XPON_ENABLE) ? "enable" : "disable");
	printf("  total size: %u\n", dba_backdoor.dba_backdoor_total_buf);
	printf("  green size: %u\n", dba_backdoor.dba_backdoor_green_buf);
	printf("  yellow size:%u\n", dba_backdoor.dba_backdoor_yellow_buf);
	
	return ECONET_E_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to get the GPON DBA slight modify detail information.
 Input Args:	see gponmgr_get_command.
 Ret Value:	success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int do_get_dba_slight_modify(int argc, char *argv[], void *p)
{
	GPON_DEV_SLIGHT_MODIFY_T dba_slight_modify;
	
#if API_CMD_GPONMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	memset(&dba_slight_modify, 0, sizeof(dba_slight_modify)) ;
	if(gponmgr_lib_get_dba_slight_modify(&dba_slight_modify) == ECONET_E_FAIL) {
		printf("Exec. Failed: Get the 'dbru_offset' command failed\n") ;
		return ECONET_E_FAIL;
	}

	printf("DBA slight modify:\n");
	printf("  enable:     %s\n", (dba_slight_modify.enable == XPON_ENABLE) ? "enable" : "disable");
	printf("  total size: %u\n", dba_slight_modify.dba_slight_modify_total_buf);
	printf("  green size: %u\n", dba_slight_modify.dba_slight_modify_green_buf);
	printf("  yellow size:%u\n", dba_slight_modify.dba_slight_modify_yellow_buf);
	
	return ECONET_E_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to get o3 and o4 ploam control status.
 Input Args:	see gponmgr_get_command.
 Ret Value:	success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int do_get_o3_o4_ploam(int argc, char *argv[], void *p)
{
	GPON_SW_HW_SELECT_T mode;
	
#if API_CMD_GPONMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(gponmgr_lib_get_o3_o4_ploam_ctrl(&mode) == ECONET_E_FAIL) {
		printf("Exec. Failed: Get the 'o3_o4_ploam' command failed\n") ;
		return ECONET_E_FAIL;
	}

	printf("O3 and O4 PLOAM control:%s\n", (mode == GPON_SW) ? "software" : "hardware") ;

	return ECONET_E_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to get tx sync offset.
 Input Args:	see gponmgr_get_command.
 Ret Value:	success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int do_get_tx_sync_off(int argc, char *argv[], void *p)
{
	unsigned char tx_sync;

#if API_CMD_GPONMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(gponmgr_lib_get_tx_sync(&tx_sync) == ECONET_E_FAIL) {
		printf("Exec. Failed: Get the 'tx_sync_off' command failed\n") ;
		return ECONET_E_FAIL;
	}

	printf("Tx sync:%u\n", tx_sync) ;
	
	return ECONET_E_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to get the GPON MAC interrupt mask.
 Input Args:	see gponmgr_get_command.
 Ret Value:	none.
******************************************************************************/
int do_get_interrupt(int argc, char *argv[], void *p)
{
	uint mask;

#if API_CMD_GPONMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(gponmgr_lib_get_int_mask(&mask) == ECONET_E_FAIL) {
		printf("Exec. Failed: Get the 'interrupt' command failed\n") ;
		return ECONET_E_FAIL;
	}
	
	printf("Interrupt mask:\n") ;
	/* 0 */
	printf("  bit%02d, PLOAMD RECV:        %s\n", GPON_INT_PLOAMD_RECV, (mask & (1 << GPON_INT_PLOAMD_RECV)) ? "enable" : "disable") ;
	printf("  bit%02d, PLOAMU SEND:        %s\n", GPON_INT_PLOAMU_SEND, (mask & (1 << GPON_INT_PLOAMU_SEND)) ? "enable" : "disable") ;
	printf("  bit%02d, SN REQ RECV:        %s\n", GPON_INT_SN_REQ_RECV, (mask & (1 << GPON_INT_SN_REQ_RECV)) ? "enable" : "disable") ;
	printf("  bit%02d, SN ONU SEND O3:     %s\n", GPON_INT_SN_ONU_SEND_O3, (mask & (1 << GPON_INT_SN_ONU_SEND_O3)) ? "enable" : "disable") ;
	printf("  bit%02d, RANGING REQ RECV:   %s\n", GPON_INT_RANGING_REQ_RECV, (mask & (1 << GPON_INT_RANGING_REQ_RECV)) ? "enable" : "disable") ;
	/* 5 */
	printf("  bit%02d, ONU SEND O4:        %s\n", GPON_INT_ONU_SEND_O4, (mask & (1 << GPON_INT_ONU_SEND_O4)) ? "enable" : "disable") ;
	printf("  bit%02d, SN REQ CRS:         %s\n", GPON_INT_SN_REQ_CRS, (mask & (1 << GPON_INT_SN_REQ_CRS)) ? "enable" : "disable") ;
	printf("  bit%02d, LOS GEN DEL:        %s\n", GPON_INT_LOS_GEN_DEL, (mask & (1 << GPON_INT_LOS_GEN_DEL)) ? "enable" : "disable") ;
	printf("  bit%02d, AES KEY SWITCH DONE:%s\n", GPON_INT_AES_KEY_SWITCH_DONE, (mask & (1 << GPON_INT_AES_KEY_SWITCH_DONE)) ? "enable" : "disable") ;
	printf("  bit%02d, TOD UPDATE DONE:    %s\n", GPON_INT_TOD_UPDATE_DONE, (mask & (1 << GPON_INT_TOD_UPDATE_DONE)) ? "enable" : "disable") ;
	/* 10 */
	printf("  bit%02d, TOD_1PPS:           %s\n", GPON_INT_TOD_1PPS, (mask & (1 << GPON_INT_TOD_1PPS)) ? "enable" : "disable") ;
	printf("  bit%02d, DYING_GASP_SEND:    %s\n", GPON_INT_DYING_GASP_SEND, (mask & (1 << GPON_INT_DYING_GASP_SEND)) ? "enable" : "disable") ;
	/* 16 */
	printf("  bit%02d, RX_ERR:             %s\n", GPON_INT_RX_ERR, (mask & (1 << GPON_INT_RX_ERR)) ? "enable" : "disable") ;
	printf("  bit%02d, FIFO_ERR:           %s\n", GPON_INT_FIFO_ERR, (mask & (1 << GPON_INT_FIFO_ERR)) ? "enable" : "disable") ;
	printf("  bit%02d, BST SGL DIFF:       %s\n", GPON_INT_BST_SGL_DIFF, (mask & (1 << GPON_INT_BST_SGL_DIFF)) ? "enable" : "disable") ;
	printf("  bit%02d, TX LATE START:      %s\n", GPON_INT_TX_LATE_START, (mask & (1 << GPON_INT_TX_LATE_START)) ? "enable" : "disable") ;
	printf("  bit%02d, RX EOF ERR:         %s\n", GPON_INT_RX_EOF_ERR, (mask & (1 << GPON_INT_RX_EOF_ERR)) ? "enable" : "disable") ;
	/* 21 */
	printf("  bit%02d, RX GEM INTLV ERR:   %s\n", GPON_INT_RX_GEM_INTLV_ERR, (mask & (1 << GPON_INT_RX_GEM_INTLV_ERR)) ? "enable" : "disable") ;
#ifdef TCSUPPORT_CPU_EN7521
	printf("  bit%02d, BFIFO FULL:         %s\n", GPON_INT_BFIFO_FULL, (mask & (1 << GPON_INT_BFIFO_FULL)) ? "enable" : "disable") ;
	printf("  bit%02d, sFIFO FULL:         %s\n", GPON_INT_SFIFO_FULL, (mask & (1 << GPON_INT_SFIFO_FULL)) ? "enable" : "disable") ;
#endif
	printf("  bit%02d, TX LATE START:      %s\n", GPON_INT_TX_LATE_START, (mask & (1 << GPON_INT_TX_LATE_START)) ? "enable" : "disable") ;
#ifdef TCSUPPORT_CPU_EN7521
	printf("  bit%02d, OLT DS FEC CHG:     %s\n", GPON_INT_OLT_DS_FEC_CHG, (mask & (1 << GPON_INT_OLT_DS_FEC_CHG)) ? "enable" : "disable") ;
	/* 26 */
	printf("  bit%02d, ONU US FEC CHG,:    %s\n", GPON_INT_ONU_US_FEC_CHG, (mask & (1 << GPON_INT_ONU_US_FEC_CHG)) ? "enable" : "disable") ;
	printf("  bit%02d, POP UP RECV IN O6:  %s\n", GPON_INT_POP_UP_RECV_IN_O6, (mask & (1 << GPON_INT_POP_UP_RECV_IN_O6)) ? "enable" : "disable") ;
	printf("  bit%02d, FWI:                %s\n", GPON_INT_FWI, (mask & (1 << GPON_INT_FWI)) ? "enable" : "disable") ;
	printf("  bit%02d, LWI:                %s\n", GPON_INT_LWI, (mask & (1 << GPON_INT_LWI)) ? "enable" : "disable") ;
	printf("  bit%02d, BWM STOP TIME ERR:  %s\n", GPON_INT_BWM_STOP_TIME_ERR, (mask & (1 << GPON_INT_BWM_STOP_TIME_ERR)) ? "enable" : "disable") ;
#endif
	return ECONET_E_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to get idle gem threshold.
 Input Args:	see gponmgr_get_command.
 Ret Value:	success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int do_get_idle_gem(int argc, char *argv[], void *p)
{
	ushort idle_gem;

#if API_CMD_GPONMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(gponmgr_lib_get_idle_gem(&idle_gem) == ECONET_E_FAIL) {
		printf("Exec. Failed: Get the 'idle_gem' command failed\n") ;
		return ECONET_E_FAIL;
	}

	printf("Idle gem threshold:0x%04x\n", idle_gem) ;
	
	return ECONET_E_SUCCESS;
}


#ifdef TCSUPPORT_CPU_EN7521
/******************************************************************************
 Descriptor:	It's used to get the high width of a 1pps pluse.
 Input Args:	see gponmgr_get_command.
 Ret Value:	success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int do_get_1pps_h_w(int argc, char *argv[], void *p)
{
	uint width;

#if API_CMD_GPONMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(gponmgr_lib_get_1pps_h_w(&width) == ECONET_E_FAIL) {
		printf("Exec. Failed: Get the '1pps_h_w' command failed\n") ;
		return ECONET_E_FAIL;
	}
	
	printf("Width:%dus\n", width) ;
	
	return ECONET_E_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to get the send ploamu wait mode.
 Input Args:	see gponmgr_get_command.
 Ret Value:	success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int do_get_send_ploamu_wait_mode(int argc, char *argv[], void *p)
{
	GPON_DEV_SEND_PLOAMU_WAIT_MODE_T mode;

#if API_CMD_GPONMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(gponmgr_lib_get_send_ploamu_wait_mode(&mode) == ECONET_E_FAIL) {
		printf("Exec. Failed: Get the 'ploamu_w_m' command failed\n") ;
		return ECONET_E_FAIL;
	}
	
	printf("PLOAMu wait mode:%s\n", (mode == SEND_PLOAMU_BEFORE) ? "before" : "after") ;
	
	return ECONET_E_SUCCESS;
}

#endif

