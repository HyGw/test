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
#include <libapi_lib_qdmamgr.h>

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
int do_get_txratemeter(int argc, char **argv, void *p);
int do_get_txq_cngst(int argc, char **argv, void *p);
int do_get_lmgr_status(int argc, char **argv, void *p);
int do_get_lmgr_low_threshold(int argc, char **argv, void *p);
int do_get_mac_resource_limit(int argc, char **argv, void *p);
int do_get_overhead(int argc, char **argv, void *p);
int do_get_txbuf_threshold(int argc, char **argv, void *p);
int do_dump_dbareport(int argc, char **argv, void *p);


/************************************************************************
*                  P U B L I C   D A T A
************************************************************************/

/************************************************************************
*                  P R I V A T E   D A T A
************************************************************************/
const cmds_t QdmaGetCmds[] = {
/*	name,					func,						flags,	argcmin,		argc_errmsg */
	{"txratemeter",			do_get_txratemeter,			0,		0,			"Get QDMA tx ratemeter value"},
	{"txq_cngst",			do_get_txq_cngst,			0,		0,			"Get QDMA txq cngst config"},
	{"lmgr_status",			do_get_lmgr_status,			0,		0,			"Get QDMA LMGR Status"},
	{"lmgr_low_thrh",		do_get_lmgr_low_threshold,	0,		0,			"Get QDMA LMGR low threshold"},
	{"overhead",			do_get_overhead,			0,		0,			"Get QDMA OverHead value"},
	{"mac_limit",			do_get_mac_resource_limit,	0,		0,			"Get QDMA Mac Resource Limit"},
	{"txbuf_thrh",			do_get_txbuf_threshold,		0,		0,			"Get QDMA tx buffer threshold"},
	{"dbareport",			do_dump_dbareport,			0,		0,			"Get QDMA DBA Report value"},
	/* Cmd should be added before this line!  */
	{"help",				api_lib_help,				0,		0,			"Show qdmamgr get usage."},
	{NULL, NULL, 0, 0, NULL}
};

/************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
************************************************************************/

/******************************************************************************
 Descriptor:	It's used to get qdma lmgr status:free dscp, used dscp, used buffer.
 Input Args:	see QdmaGetCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_get_txratemeter(int argc, char *argv[], void *p)
{
	int ret=0;
	QDMA_TxRateMeter_T txRateMeter;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_get_tx_ratemeter(&txRateMeter);
	} else if(strcmp((char *)p, QDMA_TYPE_WAN) == 0) {
		ret = qdmamgr_wan_lib_get_tx_ratemeter(&txRateMeter);
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}
	
	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: 'dbgmgr %s get ratemeter' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}

	printf("%s TX Rate Meter:\nTimeDivisor:%u, TimeSlice:%uus\n\n", (char *)p
		, 1<<txRateMeter.txRateMeterTimeDivisor, txRateMeter.txRateMeterTimeSlice);
	return QDMAMGR_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to get qdma txq cngst infomation.
 Input Args:	see QdmaGetCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_get_txq_cngst(int argc, char *argv[], void *p)
{
	int ret=0;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_get_txq_cngst();
	} else if(strcmp((char *)p, QDMA_TYPE_WAN) == 0) {
		ret = qdmamgr_wan_lib_get_txq_cngst();
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}
	
	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: 'dbgmgr %s get txq_cngst' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}

	printf("Exec. Successful\n") ;
	return QDMAMGR_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to get qdma lmgr status:free dscp, used dscp, used buffer.
 Input Args:	see QdmaGetCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_get_lmgr_status(int argc, char *argv[], void *p)
{
	int ret=0;
	QDMA_LmgrStatus_T lmgrStatus;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_get_lmgr_status(&lmgrStatus);
	} else if(strcmp((char *)p, QDMA_TYPE_WAN) == 0) {
		ret = qdmamgr_wan_lib_get_lmgr_status(&lmgrStatus);
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}
	
	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: 'dbgmgr %s get lmgr_status' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}

	printf("%s LMGR Status:\nFree DSCP:%u\nUsed DSCP:%u\nUsed Memory:%uKByte\n\n"
		, (char *)p, lmgrStatus.freeLmgr, lmgrStatus.usedLmgr, lmgrStatus.usedBuf>>10);
	return QDMAMGR_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to get qdma lmgr low threshold.
 Input Args:	see QdmaGetCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_get_lmgr_low_threshold(int argc, char *argv[], void *p)
{
	int ret=0;
	uint threshold=0;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_get_lmgr_low_threshold(&threshold);
	} else if(strcmp((char *)p, QDMA_TYPE_WAN) == 0) {
		ret = qdmamgr_wan_lib_get_lmgr_low_threshold(&threshold);
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}
	
	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: 'dbgmgr %s get lmgr_low_thrh' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}

	printf("%s LMGR Low Threshold:%u\n\n", (char *)p, threshold);
	return QDMAMGR_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to get qdma overhead value.
 Input Args:	see QdmaGetCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_get_overhead(int argc, char *argv[], void *p)
{
	int ret=0;
	uint mode=0, cnt=0;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_get_overhead_en(&mode) ;
	} else if(strcmp((char *)p, QDMA_TYPE_WAN) == 0) {
		ret = qdmamgr_wan_lib_get_overhead_en(&mode) ;
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}
	if(ret != QDMAMGR_SUCCESS) {
		printf("Exec. Failed: 'dbgmgr %s get overhead' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}

	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_get_overhead_value(&cnt) ;
	} else if(strcmp((char *)p, QDMA_TYPE_WAN) == 0) {
		ret = qdmamgr_wan_lib_get_overhead_value(&cnt) ;
	}
	if(ret != QDMAMGR_SUCCESS) {
		printf("Exec. Failed: 'dbgmgr %s get overhead' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}

	printf("%s OVER HEAD Status:\nmode:%s, overhead size:%uByte\n\n"
		, (char *)p, ((mode==QDMA_ENABLE)?("Enable"):("Disable")), cnt);
	return QDMAMGR_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to get qdma mac resource limit.
 Input Args:	see QdmaGetCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_get_mac_resource_limit(int argc, char *argv[], void *p)
{
	int ret=0;
	QDMA_MacLimitThrh_T qdma_mac_limit_thrh;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_get_mac_resource_limit(&qdma_mac_limit_thrh);
	} else if(strcmp((char *)p, QDMA_TYPE_WAN) == 0) {
		ret = qdmamgr_wan_lib_get_mac_resource_limit(&qdma_mac_limit_thrh);
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}
	
	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: 'dbgmgr %s get mac_limit' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}

	printf("%s mac resource limit:\nchannel threshold:%u\nqueue threshold:%u\n\n", (char *)p
		, qdma_mac_limit_thrh.chnlThrh, qdma_mac_limit_thrh.queueThrh);
	return QDMAMGR_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to get qdma lmgr low threshold.
 Input Args:	see QdmaGetCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_get_txbuf_threshold(int argc, char *argv[], void *p)
{
	int ret=0;
	QDMA_TxBufCtrl_T txBufCtrl;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_get_txbuf_threshold(&txBufCtrl);
	} else if(strcmp((char *)p, QDMA_TYPE_WAN) == 0) {
		ret = qdmamgr_wan_lib_get_txbuf_threshold(&txBufCtrl);
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}
	
	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: 'dbgmgr %s get txbuf_thrh' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}

	printf("%s TX Buffer Threshold:\nmode:%s\nTotal Threshold:%uByte\nChannel Threshold:%uByte\n\n", (char *)p
		, ((txBufCtrl.mode ==QDMA_ENABLE)?("Enable"):("Disable")), txBufCtrl.totalThreshold<<8, txBufCtrl.chnThreshold<<8);
	return QDMAMGR_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to dump qdma DBA Report value.
 Input Args:	see QdmaGetCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_dump_dbareport(int argc, char *argv[], void *p)
{
	int ret=0, qdmaType=0;
	uint channel=0, channel_max=0;
	char *endPtr = NULL;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		qdmaType = QDMA_LAN_FLAG;
		channel_max = CONFIG_QDMA_LAN_CHANNEL;
	} else if(strcmp((char *)p, QDMA_TYPE_WAN) == 0) {
		qdmaType = QDMA_WAN_FLAG;
		channel_max = CONFIG_QDMA_WAN_CHANNEL;
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}

	if(argc>=2) {
		channel = strtol(argv[1], &endPtr, 0);
		if((*endPtr!='\0') || (channel>channel_max)) {
			printf("Error: input channel is '%s', which should between 0 and %d\n", argv[1], channel_max);
			return QDMAMGR_FAIL;
		}
	}
	
	if((argc<2) || (channel == channel_max)) {
		if(qdmaType == QDMA_LAN_FLAG) {
			ret = qdmamgr_lan_lib_dump_dbareport(channel_max) ;
		} else if(qdmaType == QDMA_WAN_FLAG) {
			ret = qdmamgr_wan_lib_dump_dbareport(channel_max) ;
		} 
	} else if(channel < channel_max){
		if(qdmaType == QDMA_LAN_FLAG) {
			ret = qdmamgr_lan_lib_dump_dbareport(channel) ;
		} else if(qdmaType == QDMA_WAN_FLAG) {
			ret = qdmamgr_wan_lib_dump_dbareport(channel) ;
		} 
	} else {
		printf("Exec. Failed: 'dbgmgr %s get dbareport [ChnlID:0~%d]' command failed\n", (char *)p, channel_max) ;
	}
	
	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: 'dbgmgr %s get dbareport [ChnlID:0~%d]' command failed\n", (char *)p, channel_max) ;
		return QDMAMGR_FAIL;
	}

	return QDMAMGR_SUCCESS;
}

