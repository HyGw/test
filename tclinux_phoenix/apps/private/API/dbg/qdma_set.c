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
extern int subcmd(const cmds_t tab[], int argc, char *argv[], void *p);
extern int api_lib_help(int argc, char *argv[], void *p);

/************************************************************************
*                  F U N C T I O N   D E F I N I T I O N S
************************************************************************/
/* tx rate limit */
int do_set_txratemeter(int argc, char **argv, void *p);
int do_set_txratelimit(int argc, char **argv, void *p);
/* txq cngst */
int do_set_txq_cngst(int argc, char **argv, void *p);
int do_set_txqcngst_mode(int argc, char **argv, void *p);
int do_set_txqcngst_dei_scale(int argc, char **argv, void *p);
int do_set_txqcngst_autoconfig(int argc, char **argv, void *p);
int do_set_txqcngst_total_thrh(int argc, char **argv, void *p);
int do_set_txqcngst_channel_thrh(int argc, char **argv, void *p);
int do_set_txqcngst_queue_thrh(int argc, char **argv, void *p);
int do_set_txqcngst_peekrate(int argc, char **argv, void *p);
int do_set_static_cngst(int argc, char **argv, void *p);
int do_set_static_cngst_normal(int argc, char **argv, void *p);
int do_set_static_cngst_dei(int argc, char **argv, void *p);
/* dbg cntr */
int do_set_dbgcntr(int argc, char **argv, void *p);
int do_set_dbgcntr_channel(int argc, char **argv, void *p);
int do_set_dbgcntr_queue(int argc, char **argv, void *p);
int do_clear_dbgcntr(int argc, char **argv, void *p);
int do_dump_dbgcntr(int argc, char **argv, void *p);
/* cpu counter */
int do_set_cpu_counter(int argc, char **argv, void *p);
int do_clear_cpu_counter(int argc, char **argv, void *p);
int do_dump_cpu_counter(int argc, char **argv, void *p);
int do_dump_register(int argc, char **argv, void *p);
int do_dump_descriptor(int argc, char **argv, void *p);
int do_dump_irq(int argc, char **argv, void *p);
int do_dump_hwfwd(int argc, char **argv, void *p);
int do_dump_all(int argc, char **argv, void *p);

/* test */
static int do_set_test(int argc, char **argv, void *p);
int do_set_test_dbglevel(int argc, char **argv, void *p);
int do_set_test_dma_busy(int argc, char **argv, void *p);
int do_set_test_reg_poll(int argc, char **argv, void *p);
int do_set_test_force_rx1(int argc, char **argv, void *p);
/* other */
int do_set_lmgr_low_threshold(int argc, char **argv, void *p);
int do_set_overhead(int argc, char **argv, void *p);
int do_set_mac_resource_limit(int argc, char **argv, void *p);
int do_set_txbuf_threshold(int argc, char **argv, void *p);
int do_set_prefetch(int argc, char **argv, void *p);
int do_set_dbareport(int argc, char **argv, void *p);


/************************************************************************
*                  P U B L I C   D A T A
************************************************************************/

/************************************************************************
*                  P R I V A T E   D A T A
************************************************************************/
const cmds_t QdmaSetCmds[] = {
/*	name,					func,						flags,	argcmin,		argc_errmsg */
	{"txratemeter",			do_set_txratemeter,			0,		0,			"Set QDMA tx ratemeter value"},
	{"txratelimit",			do_set_txratelimit,			0,		0,			"Set QDMA tx ratelimit config"},
	{"txq_cngst",			do_set_txq_cngst,			0,		0,			"Set QDMA txq cngst"},	
	{"static_cngst",		do_set_static_cngst,		0,		0,			"Set QDMA txq static cngst"},	
	{"dbgcntr",				do_set_dbgcntr,				0,		0,			"Set QDMA dbg cntr"},
	{"cpu_counter",			do_set_cpu_counter,			0,		0,			"Set QDMA cpu counter"},
	{"register_dump",		do_dump_register,			0,		0,			"Dump QDMA register info"},
	{"descriptor_dump",		do_dump_descriptor,			0,		0,			"Dump QDMA descriptor info"},
	{"irq_dump",			do_dump_irq,				0,		0,			"Dump QDMA irq info"},
	{"hwfwd_dump",			do_dump_hwfwd,				0,		0,			"Dump QDMA hw_fwd info"},
	{"all_dump",			do_dump_all,				0,		0,			"Dump QDMA hw_fwd info"},
	{"test",				do_set_test,				0,		0,			"Set QDMA Test"},
	{"lmgr_low_thrh",		do_set_lmgr_low_threshold,	0,		0,			"Set QDMA LMGR low threshold"},
	{"overhead",			do_set_overhead,			0,		0,			"Set QDMA OverHead value"},
	{"mac_limit",			do_set_mac_resource_limit,	0,		0,			"Set QDMA Mac Resource Limit"},
	{"txbuf_thrh",			do_set_txbuf_threshold,		0,		0,			"Set QDMA tx buffer threshold"},
	{"prefetch",			do_set_prefetch,			0,		0,			"Set QDMA Prefetch config"},
	{"dbareport",			do_set_dbareport,			0,		0,			"Set QDMA DBA Report value"},
	/* Cmd should be added before this line!  */
	{"help",				api_lib_help,				0,		0,			"Show qdma set usage."},
	{NULL, NULL, 0, 0, NULL}
};

const cmds_t QdmaSetTxqCngstCmds[] = {
/*	name,			func,							flags,	argcmin,		argc_errmsg */
	{"mode",		do_set_txqcngst_mode,			0,		0,			"set QDMA txq cngst mode <dynamic/static>"},
	{"dei_scale",	do_set_txqcngst_dei_scale,		0,		0,			"set QDMA txq cngst dei scale <1/2~1/16>"},
	{"autoconfig",	do_set_txqcngst_autoconfig,		0,		0,			"set QDMA txq cngst auto config <cngstMode, margin, duration>"},
	{"total",		do_set_txqcngst_total_thrh,		0,		0,			"set QDMA txq cngst total threshold <totalMin, totalMax>"},
	{"channel",		do_set_txqcngst_channel_thrh,	0,		0,			"set QDMA txq cngst channel threshold <chnlMin, chnlMax>"},
	{"queue",		do_set_txqcngst_queue_thrh,		0,		0,			"set QDMA txq cngst queue threshold <queueMin, queueMax>"},
	{"peekrate",	do_set_txqcngst_peekrate,		0,		0,			"set QDMA txq cngst peek rate <peekEn, margin, duration>"},
	/* Cmd should be added before this line!  */
	{"help",		api_lib_help,					0,		0,			"Show qdma set txq cngst usage."},
	{NULL, NULL, 0, 0, NULL}
};

const cmds_t QdmaSetStaticCngstCmds[] = {
/*	name,		func,						flags,	argcmin,		argc_errmsg */
	{"normal",	do_set_static_cngst_normal,	0,		0,			"set static cngst normal threshold <norm[0], norm[1], ... ,norm[7]>"},
	{"dei",		do_set_static_cngst_dei,	0,		0,			"set static cngst dei threshold <dei[0], dei[1], ... ,dei[7]>"},
	/* Cmd should be added before this line!  */
	{"help",	api_lib_help,				0,		0,			"Show qdma set static cngst usage."},
	{NULL, NULL, 0, 0, NULL}
};

const cmds_t QdmaSetDbgCntrCmds[] = {
/*	name,		func,						flags,	argcmin,		argc_errmsg */
	{"channel",	do_set_dbgcntr_channel,		0,		0,			"set dbgcntr as channel groups"},
	{"queue",	do_set_dbgcntr_queue,		0,		0,			"set dbgcntr as queue groups"},
	{"clear",	do_clear_dbgcntr,			0,		0,			"Clear dbgcntr counters"},
	{"dump",	do_dump_dbgcntr,			0,		0,			"Dump dbgcntr counters"},
	/* Cmd should be added before this line!  */
	{"help",	api_lib_help,				0,		0,			"Show qdma set dbgcntr usage."},
	{NULL, NULL, 0, 0, NULL}
};

const cmds_t QdmaSetCpuCounterCmds[] = {
/*	name,		func,						flags,	argcmin,		argc_errmsg */
	{"clear",	do_clear_cpu_counter,		0,		0,			"Clear QDMA cpu counter"},
	{"dump",	do_dump_cpu_counter,		0,		0,			"Dump QDMA cpu counter"},
	/* Cmd should be added before this line!  */
	{"help",	api_lib_help,				0,		0,			"Show qdma set cpu counter usage."},
	{NULL, NULL, 0, 0, NULL}
};

const cmds_t QdmaSetTestCmds[] = {
/*	name,		func,						flags,	argcmin,		argc_errmsg */
	{"dbglevel",	do_set_test_dbglevel,	0,		0,			"Set QDMA debug level"},
	{"dma_busy",	do_set_test_dma_busy,	0,		0,			"Set QDMA dma busy timer"},
	{"reg_polling",	do_set_test_reg_poll,	0,		0,			"Set QDMA dma busy timer"},
	{"force_rx1",	do_set_test_force_rx1,	0,		0,			"Set QDMA force to RX1 enable or disable"},
	/* Cmd should be added before this line!  */
	{"help",		api_lib_help,			0,		0,			"Show qdma set cpu counter usage."},
	{NULL, NULL, 0, 0, NULL}
};


/************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
************************************************************************/

/******************************************************************************
 Descriptor:	It's used to set QDMA tx rate meter value.
 Input Args:	see QdmaSetCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_set_txratemeter(int argc, char *argv[], void *p)
{
	int ret = 0;
	uint timeSlice=0 ;
	QDMA_TxRateMeter_T txRateMeter;
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(argc<3)
		goto err;
	
	if(strncasecmp(argv[1], "1", strlen("1")) == 0) {
		txRateMeter.txRateMeterTimeDivisor = 0;
	} else if(strncasecmp(argv[1], "2", strlen("2")) == 0) {
		txRateMeter.txRateMeterTimeDivisor = 1;
	} else if(strncasecmp(argv[1], "4", strlen("4")) == 0) {
		txRateMeter.txRateMeterTimeDivisor = 2;
	} else if(strncasecmp(argv[1], "8", strlen("8")) == 0) {
		txRateMeter.txRateMeterTimeDivisor = 3;
	} else {
		goto err;
	}
	
	timeSlice = (uint)strtoul(argv[2], NULL, 10) ;
	if((timeSlice < 1) || (timeSlice > 65535))
		goto err;
	txRateMeter.txRateMeterTimeSlice = (ushort)timeSlice ;
	
	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_set_tx_ratemeter(&txRateMeter);
	} else if(strcmp((char *)p, QDMA_TYPE_WAN) == 0) {
		ret = qdmamgr_wan_lib_set_tx_ratemeter(&txRateMeter);
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}
	
	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: 'dbgmgr %s set txratemeter' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;
	return QDMAMGR_SUCCESS;
	
err:
	printf("Format: dbgmgr %s set txratemeter [timeDivesel:1/2/4/8] [timeSlice:1~65535(us)]\n", (char *)p) ;
	return QDMAMGR_FAIL;
}

/******************************************************************************
 Descriptor:	It's used to set QDMA tx rate limit config.
 Input Args:	see QdmaSetCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_set_txratelimit(int argc, char *argv[], void *p)
{
	int ret = 0;
	QDMA_TxRateLimitCfg_T txRateLimitCfg;
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(argc<3)
		goto err;
	
	txRateLimitCfg.txRateLimitBucketScale = (uint)strtoul(argv[1], NULL, 10) ;
	txRateLimitCfg.txRateLimitUnit = (uint)strtoul(argv[2], NULL, 10) ;
	if((txRateLimitCfg.txRateLimitBucketScale >= QDMA_TRTCM_SCALE_MAX_ITEMS)
	|| (txRateLimitCfg.txRateLimitUnit < 1) || (txRateLimitCfg.txRateLimitUnit > 65))
		goto err;
	
	
	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_set_tx_ratelimit_config(&txRateLimitCfg);
	} else if(strcmp((char *)p, QDMA_TYPE_WAN) == 0) {
		ret = qdmamgr_wan_lib_set_tx_ratelimit_config(&txRateLimitCfg);
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}
	
	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: 'dbgmgr %s set txratelimit' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;
	return QDMAMGR_SUCCESS;
	
err:
	printf("Format: dbgmgr %s set txratelimit [buckScale:0~%d(2^nByte)] [unit:1~65(kbps)]\n", (char *)p, QDMA_TRTCM_SCALE_MAX_ITEMS-1) ;
	return QDMAMGR_FAIL;
}

/******************************************************************************
 Descriptor:	It's used to set QDMA txq cngst Commands.
 Input Args:	see QdmaSetCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_set_txq_cngst(int argc, char *argv[], void *p)
{
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	return subcmd(QdmaSetTxqCngstCmds, argc, argv, p);
}

/******************************************************************************
 Descriptor:	It's used to set QDMA txq cngst mode: dynamic/static.
 Input Args:	see QdmaSetTxqCngstCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_set_txqcngst_mode(int argc, char *argv[], void *p)
{
	int ret=0;
	QDMA_Mode_t dynCngstEn;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(argc<2)
		goto err;

	if(strncasecmp(argv[1], "enable", strlen("enable")) == 0) {
		dynCngstEn = QDMA_ENABLE;
	} else if(strncasecmp(argv[1], "disable", strlen("disable")) == 0) {
		dynCngstEn = QDMA_DISABLE;
	} else {
		goto err;
	}
	
	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_set_txq_cngst_mode(dynCngstEn);
	} else if(strcmp((char *)p, QDMA_TYPE_WAN) == 0) {
		ret = qdmamgr_wan_lib_set_txq_cngst_mode(dynCngstEn);
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}

	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: 'dbgmgr %s set txq_cngst mode' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;
	return QDMAMGR_SUCCESS;
	
err:
	printf("Format: dbgmgr %s set txq_cngst mode [Enable/Disable]\n", (char *)p) ;
	return QDMAMGR_FAIL;
}

/******************************************************************************
 Descriptor:	It's used to set QDMA txq cngst dei scale: 1/2~1/16.
 Input Args:	see QdmaSetTxqCngstCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_set_txqcngst_dei_scale(int argc, char *argv[], void *p)
{
	int ret=0;
	uint threshold = 0;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(argc<2)
		goto err;

	if(strncasecmp(argv[1], "1/2", strlen("1/2")) == 0) {
		threshold = 0;
	} else if(strncasecmp(argv[1], "1/4", strlen("1/4")) == 0) {
		threshold = 1;
	} else if(strncasecmp(argv[1], "1/8", strlen("1/8")) == 0) {
		threshold = 2;
	} else if(strncasecmp(argv[1], "1/16", strlen("1/16")) == 0) {
		threshold = 3;
	} else {
		goto err;
	}
	
	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_set_txq_cngst_dei_scale(threshold);
	} else if(strcmp((char *)p, QDMA_TYPE_WAN) == 0) {
		ret = qdmamgr_wan_lib_set_txq_cngst_dei_scale(threshold);
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}

	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: 'dbgmgr %s set txq_cngst dei_scale' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;
	return QDMAMGR_SUCCESS;
	
err:
	printf("Format: dbgmgr %s set txq_cngst dei_scale [1/2, 1/4, 1/8, 1/16]\n", (char *)p) ;
	return QDMAMGR_FAIL;
}

/******************************************************************************
 Descriptor:	It's used to set QDMA txq cngst autoconfig.
 Input Args:	see QdmaSetTxqCngstCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_set_txqcngst_autoconfig(int argc, char *argv[], void *p)
{
	int ret=0;
	QDMA_txCngstCfg_t txCngstCfg;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(argc<2)
		goto err;

	if(strncasecmp(argv[1], "static", strlen("static")) == 0) {
		txCngstCfg.txCngstMode = QDMA_TXCNGST_STATIC;
	} else if(strncasecmp(argv[1], "dynamic", strlen("dynamic")) == 0) {
		txCngstCfg.txCngstMode = QDMA_TXCNGST_DYNAMIC_NORMAL;
	} else if(strncasecmp(argv[1], "peekrate", strlen("peekrate")) == 0) {
		if(argc<4)
			goto err;
		txCngstCfg.txCngstMode = QDMA_TXCNGST_DYNAMIC_PEAKRATE_MARGIN;
		
		if(strncasecmp(argv[2], "0", strlen("0")) == 0) {
			txCngstCfg.peekRateMargin = 0;
		} else if(strncasecmp(argv[2], "25%", strlen("25%")) == 0) {
			txCngstCfg.peekRateMargin = 1;
		} else if(strncasecmp(argv[2], "50%", strlen("50%")) == 0) {
			txCngstCfg.peekRateMargin = 2;
		} else if(strncasecmp(argv[2], "100%", strlen("100%")) == 0) {
			txCngstCfg.peekRateMargin = 3;
		} else {
			goto err;
		}
		
		txCngstCfg.peekRateDuration = (uint)strtoul(argv[3], NULL, 10) ;
		if((txCngstCfg.peekRateDuration<16) || (txCngstCfg.peekRateDuration>4080))
			goto err;
	} else {
		goto err;
	}
	
	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_set_txq_cngst_autoconfig(&txCngstCfg);
	} else if(strcmp((char *)p, QDMA_TYPE_WAN) == 0) {
		ret = qdmamgr_wan_lib_set_txq_cngst_autoconfig(&txCngstCfg);
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}

	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: 'dbgmgr %s set txq_cngst autoconfig' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;
	return QDMAMGR_SUCCESS;
	
err:
	printf("Format-1: dbgmgr %s set txq_cngst autoconfig [static/dynamic]\n", (char *)p) ;
	printf("Format-2: dbgmgr %s set txq_cngst autoconfig peekrate [margin:0/25%%/50%%/100%%] [duration:16~4080(ms)]\n", (char *)p) ;
	return QDMAMGR_FAIL;
}

/******************************************************************************
 Descriptor:	It's used to set QDMA txq cngst total threshold: totalMin & totalMax.
 Input Args:	see QdmaSetTxqCngstCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_set_txqcngst_total_thrh(int argc, char *argv[], void *p)
{
	int ret=0;
	QDMA_TxQDynCngstTotalThrh_T totalThrh;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(argc<3)
		goto err;

	totalThrh.dynCngstTotalMinThrh = (uint)strtoul(argv[1], NULL, 10) ;
	totalThrh.dynCngstTotalMaxThrh = (uint)strtoul(argv[2], NULL, 10) ;
	if((totalThrh.dynCngstTotalMinThrh<1) || (totalThrh.dynCngstTotalMinThrh>4096))
		goto err;
	if((totalThrh.dynCngstTotalMaxThrh<1) || (totalThrh.dynCngstTotalMaxThrh>4096))
		goto err;
	
	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_set_txq_cngst_total_threshold(&totalThrh);
	} else if(strcmp((char *)p, QDMA_TYPE_WAN) == 0) {
		ret = qdmamgr_wan_lib_set_txq_cngst_total_threshold(&totalThrh);
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}

	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: 'dbgmgr %s set txq_cngst total' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;
	return QDMAMGR_SUCCESS;
	
err:
	printf("Format: dbgmgr %s set txq_cngst total [totalMin:1~4096(unit:kByte)] [totalMax:1~4096(unit:kByte)]\n", (char *)p) ;
	return QDMAMGR_FAIL;
}

/******************************************************************************
 Descriptor:	It's used to set QDMA txq cngst channel threshold: chnlMin & chnlMax.
 Input Args:	see QdmaSetTxqCngstCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_set_txqcngst_channel_thrh(int argc, char *argv[], void *p)
{
	int ret=0;
	QDMA_TxQDynCngstChnlThrh_T channelThrh;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(argc<3)
		goto err;

	channelThrh.dynCngstChnlMinThrh = (uint)strtoul(argv[1], NULL, 10) ;
	channelThrh.dynCngstChnlMaxThrh = (uint)strtoul(argv[2], NULL, 10) ;
	if((channelThrh.dynCngstChnlMinThrh<1) || (channelThrh.dynCngstChnlMinThrh>4096))
		goto err;
	if((channelThrh.dynCngstChnlMaxThrh<1) || (channelThrh.dynCngstChnlMaxThrh>4096))
		goto err;
	
	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_set_txq_cngst_channel_threshold(&channelThrh);
	} else if(strcmp((char *)p, QDMA_TYPE_WAN) == 0) {
		ret = qdmamgr_wan_lib_set_txq_cngst_channel_threshold(&channelThrh);
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}

	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: 'dbgmgr %s set txq_cngst channel' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;
	return QDMAMGR_SUCCESS;
	
err:
	printf("Format: dbgmgr %s set txq_cngst channel [chnlMin:1~4096(unit:kByte)] [chnlMax:1~4096(unit:kByte)]\n", (char *)p) ;
	return QDMAMGR_FAIL;
}


/******************************************************************************
 Descriptor:	It's used to set QDMA txq cngst queue threshold: queueMin & queueMax.
 Input Args:	see QdmaSetTxqCngstCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_set_txqcngst_queue_thrh(int argc, char *argv[], void *p)
{
	int ret=0;
	QDMA_TxQDynCngstQueueThrh_T queueThrh;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(argc<3)
		goto err;

	queueThrh.dynCngstQueueMinThrh = (uint)strtoul(argv[1], NULL, 10) ;
	queueThrh.dynCngstQueueMaxThrh = (uint)strtoul(argv[2], NULL, 10) ;
	if((queueThrh.dynCngstQueueMinThrh<1) || (queueThrh.dynCngstQueueMinThrh>4096))
		goto err;
	if((queueThrh.dynCngstQueueMaxThrh<1) || (queueThrh.dynCngstQueueMaxThrh>4096))
		goto err;
	
	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_set_txq_cngst_queue_threshold(&queueThrh);
	} else if(strcmp((char *)p, QDMA_TYPE_WAN) == 0) {
		ret = qdmamgr_wan_lib_set_txq_cngst_queue_threshold(&queueThrh);
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}

	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: 'dbgmgr %s set txq_cngst queue' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;
	return QDMAMGR_SUCCESS;
	
err:
	printf("Format: dbgmgr %s set txq_cngst queue [queueMin:1~4096(unit:kByte)] [queueMax:1~4096(unit:kByte)]\n", (char *)p) ;
	return QDMAMGR_FAIL;
}

/******************************************************************************
 Descriptor:	It's used to set QDMA txq cngst peekrate.
 Input Args:	see QdmaSetTxqCngstCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_set_txqcngst_peekrate(int argc, char *argv[], void *p)
{
	int ret=0;
	QDMA_PeekRateCfg_t peekrateCfg;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(argc<4)
		goto err;

	if(strncasecmp(argv[1], "Enable", strlen("Enable")) == 0) {
		peekrateCfg.peekRateEn = QDMA_ENABLE;
	} else if(strncasecmp(argv[1], "Disable", strlen("Disable")) == 0) {
		peekrateCfg.peekRateEn = QDMA_DISABLE;
	} else {
		goto err;
	}
		
	if(strncasecmp(argv[2], "0", strlen("0")) == 0) {
		peekrateCfg.peekRateMargin = 0;
	} else if(strncasecmp(argv[2], "25%", strlen("25%")) == 0) {
		peekrateCfg.peekRateMargin = 1;
	} else if(strncasecmp(argv[2], "50%", strlen("50%")) == 0) {
		peekrateCfg.peekRateMargin = 2;
	} else if(strncasecmp(argv[2], "100%", strlen("100%")) == 0) {
		peekrateCfg.peekRateMargin = 3;
	} else {
		goto err;
	}
	
	peekrateCfg.peekRateDuration = (uint)strtoul(argv[3], NULL, 10) ;
	if((peekrateCfg.peekRateDuration<16) || (peekrateCfg.peekRateDuration>4080))
		goto err;
		
	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_set_txq_cngst_peekrate(&peekrateCfg);
	} else if(strcmp((char *)p, QDMA_TYPE_WAN) == 0) {
		ret = qdmamgr_wan_lib_set_txq_cngst_peekrate(&peekrateCfg);
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}

	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: 'dbgmgr %s set txq_cngst peekrate' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;
	return QDMAMGR_SUCCESS;
	
err:
	printf("Format: dbgmgr %s set txq_cngst peekrate [Enable/Disable] [margin:0/25%%/50%%/100%%] [duration:16~4080(ms)]\n", (char *)p) ;
	return QDMAMGR_FAIL;
}

/******************************************************************************
 Descriptor:	It's used to set QDMA static cngst Commands.
 Input Args:	see QdmaSetCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_set_static_cngst(int argc, char *argv[], void *p)
{
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	return subcmd(QdmaSetStaticCngstCmds, argc, argv, (void *)QdmaSetStaticCngstCmds);
}

/******************************************************************************
 Descriptor:	It's used to set QDMA static cngst normal threshold.
 Input Args:	see QdmaSetStaticCngstCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_set_static_cngst_normal(int argc, char *argv[], void *p)
{
	int ret=0, i=0;
	uint normThrh=0;
	QDMA_TxQStaticNormalCfg_T normCfg;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(argc<1+CONFIG_QDMA_QUEUE)
		goto err;

	for(i=0; i<CONFIG_QDMA_QUEUE; i++) {
		normThrh = (uint)strtoul(argv[i+1], NULL, 10);
		if(normThrh>4096)
			goto err;
		normCfg.normalThrh[i] = normThrh<<2;
	}
	
	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_set_static_cngst_normal(&normCfg);
	} else if(strcmp((char *)p, QDMA_TYPE_WAN) == 0) {
		ret = qdmamgr_wan_lib_set_static_cngst_normal(&normCfg);
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}

	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: 'dbgmgr %s set static_cngst normal' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;
	return QDMAMGR_SUCCESS;
	
err:
	printf("Format: dbgmgr %s set static_cngst normal [normThrh_0: 1~4096(unit:kByte)] ... [normThrh_7: 1~4096(unit:kByte)]\n", (char *)p) ;
	return QDMAMGR_FAIL;
}

/******************************************************************************
 Descriptor:	It's used to set QDMA static cngst dei threshold.
 Input Args:	see QdmaSetStaticCngstCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_set_static_cngst_dei(int argc, char *argv[], void *p)
{
	int ret=0, i=0;
	uint deiThrh=0;
	QDMA_TxQStaticDeiCfg_T deiCfg;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(argc<1+CONFIG_QDMA_QUEUE)
		goto err;

	for(i=0; i<CONFIG_QDMA_QUEUE; i++) {
		deiThrh = (uint)strtoul(argv[i+1], NULL, 10);
		if(deiThrh>4096)
			goto err;
		deiCfg.deiThrh[i] = deiThrh<<2;
	}

	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_set_static_cngst_dei(&deiCfg);
	} else if(strcmp((char *)p, QDMA_TYPE_WAN) == 0) {
		ret = qdmamgr_wan_lib_set_static_cngst_dei(&deiCfg);
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}

	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: 'dbgmgr %s set static_cngst dei' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;
	return QDMAMGR_SUCCESS;
	
err:
	printf("Format: dbgmgr %s set static_cngst dei [deiThrh_0: 1~4096(unit:kByte)] ... [deiThrh_7: 1~4096(unit:kByte)]\n", (char *)p) ;
	return QDMAMGR_FAIL;
}

/******************************************************************************
 Descriptor:	It's used to set QDMA DbgCntr Commands.
 Input Args:	see QdmaSetCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_set_dbgcntr(int argc, char *argv[], void *p)
{
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	return subcmd(QdmaSetDbgCntrCmds, argc, argv, p);
}

/******************************************************************************
 Descriptor:	It's used to set QDMA DbgCntr Channel related Commands.
 Input Args:	see QdmaSetDbgCntrCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_set_dbgcntr_channel(int argc, char *argv[], void *p)
{
	int ret=0;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_set_dbgcntr_channel(QDMA_DBG_CNTR_CHNL_TXCPU);
	} else if(strcmp((char *)p, QDMA_TYPE_WAN) == 0) {
		if(strncasecmp(argv[1], "cpu", strlen("cpu")) == 0) {
			ret = qdmamgr_wan_lib_set_dbgcntr_channel(QDMA_DBG_CNTR_CHNL_TXCPU);
		} else if(strncasecmp(argv[1], "fwd1", strlen("fwd1")) == 0) {
			ret = qdmamgr_wan_lib_set_dbgcntr_channel(QDMA_DBG_CNTR_CHNL_TXFWD1);
		} else if(strncasecmp(argv[1], "fwd2", strlen("fwd2")) == 0) {
			ret = qdmamgr_wan_lib_set_dbgcntr_channel(QDMA_DBG_CNTR_CHNL_TXFWD2);
		} else {
			goto err;
		}
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}

	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: 'dbgmgr %s set dbgcntr channel' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;
	return QDMAMGR_SUCCESS;
	
err:
	printf("Format: dbgmgr %s set dbgcntr channel [cpu/fwd1/fwd2]\n", (char *)p) ;
	return QDMAMGR_FAIL;
}

/******************************************************************************
 Descriptor:	It's used to set QDMA DbgCntr Counters as queue groups.
 Input Args:	see QdmaSetDbgCntrCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_set_dbgcntr_queue(int argc, char *argv[], void *p)
{
	int ret=0;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_set_dbgcntr_queue();
	} else if(strcmp((char *)p, QDMA_TYPE_WAN) == 0) {
		ret = qdmamgr_wan_lib_set_dbgcntr_queue();
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}

	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: 'dbgmgr %s set dbgcntr queue' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;

	return 0;
}

/******************************************************************************
 Descriptor:	It's used to clear QDMA DbgCntr Counters.
 Input Args:	see QdmaSetDbgCntrCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_clear_dbgcntr(int argc, char *argv[], void *p)
{
	int ret=0;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_clear_dbgcntr();
	} else if(strcmp((char *)p, QDMA_TYPE_WAN) == 0) {
		ret = qdmamgr_wan_lib_clear_dbgcntr();
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}

	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: 'dbgmgr %s set dbgcntr clear' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;

	return 0;
}

/******************************************************************************
 Descriptor:	It's used to dump QDMA DbgCntr Counters.
 Input Args:	see QdmaSetDbgCntrCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_dump_dbgcntr(int argc, char *argv[], void *p)
{
	int ret=0;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_dump_dbgcntr();
	} else if(strcmp((char *)p, QDMA_TYPE_WAN) == 0) {
		ret = qdmamgr_wan_lib_dump_dbgcntr();
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}

	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: 'dbgmgr %s set dbgcntr dump' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;

	return 0;
}

/******************************************************************************
 Descriptor:	It's used to set QDMA CPU Counters.
 Input Args:	see QdmaSetCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_set_cpu_counter(int argc, char *argv[], void *p)
{
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	return subcmd(QdmaSetCpuCounterCmds, argc, argv, p);
}

/******************************************************************************
 Descriptor:	It's used to clear QDMA CPU Counters.
 Input Args:	see QdmaSetCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_clear_cpu_counter(int argc, char *argv[], void *p)
{
	int ret=0;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_clear_cpu_counter();
	} else if(strcmp((char *)p, QDMA_TYPE_WAN) == 0) {
		ret = qdmamgr_wan_lib_clear_cpu_counter();
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}

	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: 'dbgmgr %s set cpu_counter clear' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;
	return QDMAMGR_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to dump QDMA CPU Counters.
 Input Args:	see QdmaSetCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_dump_cpu_counter(int argc, char *argv[], void *p)
{
	int ret=0;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_dump_cpu_counter();
	} else if(strcmp((char *)p, QDMA_TYPE_WAN) == 0) {
		ret = qdmamgr_wan_lib_dump_cpu_counter();
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}

	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: 'dbgmgr %s set cpu_counter dump' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;
	return QDMAMGR_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to dump QADMA Register info.
 Input Args:	see QdmaSetCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_dump_register(int argc, char *argv[], void *p)
{
	int ret=0;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_dump_register();
	} else if(strcmp((char *)p, QDMA_TYPE_WAN) == 0) {
		ret = qdmamgr_wan_lib_dump_register();
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}

	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: 'dbgmgr %s set register_dump' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;
	return QDMAMGR_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to dump QADMA descriptor info.
 Input Args:	see QdmaSetCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_dump_descriptor(int argc, char *argv[], void *p)
{
	int ret=0;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_dump_descriptor();
	} else if(strcmp((char *)p, QDMA_TYPE_WAN) == 0) {
		ret = qdmamgr_wan_lib_dump_descriptor();
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}

	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: 'dbgmgr %s set descriptor_dump' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;
	return QDMAMGR_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to dump QADMA IRQ info.
 Input Args:	see QdmaSetCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_dump_irq(int argc, char *argv[], void *p)
{
	int ret=0;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_dump_irq();
	} else if(strcmp((char *)p, QDMA_TYPE_WAN) == 0) {
		ret = qdmamgr_wan_lib_dump_irq();
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}

	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: 'dbgmgr %s set irq_dump' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;
	return QDMAMGR_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to dump QADMA hw fwd info.
 Input Args:	see QdmaSetCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_dump_hwfwd(int argc, char *argv[], void *p)
{
	int ret=0;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_dump_hwfwd();
	} else if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_wan_lib_dump_hwfwd();
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}

	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: 'dbgmgr %s set hwfwd_dump' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;
	return QDMAMGR_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to dump QADMA all info.
 Input Args:	see QdmaSetCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_dump_all(int argc, char *argv[], void *p)
{
	int ret=0;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_dump_all();
	} else if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_wan_lib_dump_all();
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}

	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: 'dbgmgr %s set all_dump' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;
	return QDMAMGR_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to set QDMA Test Commands.
 Input Args:	see QdmaSetCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
static int do_set_test(int argc, char *argv[], void *p)
{
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	return subcmd(QdmaSetTestCmds, argc, argv, p);
}

/******************************************************************************
 Descriptor:	It's used to set QADMA debug level.
 Input Args:	see QdmaSetTestCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_set_test_dbglevel(int argc, char *argv[], void *p)
{
	int ret=0;
	uint level=0;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(argc<2)
		goto err;

	level = (uint)strtoul(argv[1], NULL, 10) ;
	if((level<0) || (level>=DBG_LEVEL_MAX))
		goto err;
	
	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_set_test_dbglevel(level);
	} else if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_wan_lib_set_test_dbglevel(level);
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}

	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: 'dbgmgr %s set dbglevel' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;
	return QDMAMGR_SUCCESS;
	
err:
	printf("Format: dbgmgr %s set dbglevel [0~%d]\n", (char *)p, DBG_LEVEL_MAX-1) ;
	return QDMAMGR_FAIL;
}

/******************************************************************************
 Descriptor:	It's used to set QADMA dma busy timer per 10ms.
 Input Args:	see QdmaSetTestCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_set_test_dma_busy(int argc, char *argv[], void *p)
{
	int ret=0;
	uint duration=0;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(argc<2)
		goto err;

	duration = (uint)strtoul(argv[1], NULL, 10) ;
	if((duration<0) || (duration>QDMA_BUSY_TIMER_MAX))
		goto err;
	
	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_set_test_dma_dusy(duration);
	} else if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_wan_lib_set_test_dma_dusy(duration);
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}

	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: 'dbgmgr %s set dma_busy' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;
	return QDMAMGR_SUCCESS;
	
err:
	printf("Format: dbgmgr %s set dma_busy [0~%d(unit:second)]\n", (char *)p, QDMA_BUSY_TIMER_MAX) ;
	return QDMAMGR_FAIL;
}

/******************************************************************************
 Descriptor:	It's used to set QADMA register polling per 10ms.
 Input Args:	see QdmaSetTestCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_set_test_reg_poll(int argc, char *argv[], void *p)
{
	int ret=0;
	uint offset=0, duration=0;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(argc<3)
		goto err;

	offset = (uint)strtoul(argv[1], NULL, 16) ;
	if((offset<0) || (offset>=0x440))
		goto err;

	duration = (uint)strtoul(argv[2], NULL, 10) ;
	if((duration<0) || (duration>QDMA_REG_POLLING_MAX))
		goto err;
	
	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_set_test_reg_polling(offset, duration);
	} else if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_wan_lib_set_test_reg_polling(offset, duration);
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}

	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: 'dbgmgr %s set reg_polling' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;
	return QDMAMGR_SUCCESS;
	
err:
	printf("Format: dbgmgr %s set reg_polling [0~0x440] [0~%d(unit:second)]\n", (char *)p, QDMA_REG_POLLING_MAX) ;
	return QDMAMGR_FAIL;
}

/******************************************************************************
 Descriptor:	It's used to set QADMA receive packet force set to RX1.
 Input Args:	see QdmaSetTestCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_set_test_force_rx1(int argc, char *argv[], void *p)
{
	int ret=0;
	QDMA_Mode_t mode=0;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(argc<2)
		goto err;
	
	if(strncasecmp(argv[1], "Enable", strlen("Enable")) == 0) {
		mode = QDMA_ENABLE;
	} else if(strncasecmp(argv[1], "Disable", strlen("Disable")) == 0) {
		mode = QDMA_DISABLE;
	} else {
		goto err;
	}
	
	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_set_test_force_rx1(mode);
	} else if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_wan_lib_set_test_force_rx1(mode);
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}

	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: 'dbgmgr %s set force_rx1' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;
	return QDMAMGR_SUCCESS;
	
err:
	printf("Format: dbgmgr %s set force_rx1 [Enable/Disable]\n", (char *)p) ;
	return QDMAMGR_FAIL;
}

/******************************************************************************
 Descriptor:	It's used to set QDMA lmgr low threshold.
 Input Args:	see QdmaSetCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_set_lmgr_low_threshold(int argc, char *argv[], void *p)
{
	int ret=0, qdmaType=0;
	uint threshold=0, dscpMax=0;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		qdmaType = QDMA_LAN_FLAG;
		dscpMax = 1024;
	} else if(strcmp((char *)p, QDMA_TYPE_WAN) == 0) {
		qdmaType = QDMA_WAN_FLAG;
		dscpMax = 4096;
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}

	if(argc<2)
		goto err;
	
	threshold = (uint)strtoul(argv[1], NULL, 10) ;
	if(threshold > dscpMax)
		goto err;
		
	if(qdmaType == QDMA_LAN_FLAG) {
		ret = qdmamgr_lan_lib_set_lmgr_low_threshold(threshold);
	} else if(qdmaType == QDMA_WAN_FLAG) {
		ret = qdmamgr_wan_lib_set_lmgr_low_threshold(threshold);
	}
	
	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: 'dbgmgr %s set lmgr_low_thrh' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;
	return QDMAMGR_SUCCESS;
	
err:
	printf("Format: dbgmgr %s set lmgr_low_thrh [threshold:0~%u]\n", (char *)p, dscpMax-1) ;
	return QDMAMGR_FAIL;
}

/******************************************************************************
 Descriptor:	It's used to set QDMA overhead value.
 Input Args:	see QdmaSetCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_set_overhead(int argc, char *argv[], void *p)
{
	int ret=0, cnt=0;
	QDMA_Mode_t mode=0;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(argc<3)
		goto err;
	
	if(strncasecmp(argv[1], "Enable", strlen("Enable")) == 0) {
		mode = QDMA_ENABLE;
	} else if(strncasecmp(argv[1], "Disable", strlen("Disable")) == 0) {
		mode = QDMA_DISABLE;
	} else {
		goto err;
	}
	
	cnt = (int)strtoul(argv[2], NULL, 10) ;
	if((cnt < 0) || (cnt > 255))
		goto err;
		
	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_set_overhead_en(mode);
	} else if(strcmp((char *)p, QDMA_TYPE_WAN) == 0) {
		ret = qdmamgr_wan_lib_set_overhead_en(mode);
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}
	if(ret == QDMAMGR_FAIL) {
		goto err;
	}
	
	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_set_overhead_value(cnt);
	} else if(strcmp((char *)p, QDMA_TYPE_WAN) == 0) {
		ret = qdmamgr_wan_lib_set_overhead_value(cnt);
	}	
	if(ret == QDMAMGR_FAIL) {
		goto err;
	}
	
	printf("Exec. Successful\n") ;
	return QDMAMGR_SUCCESS;
	
err:
	printf("Format: dbgmgr %s set overhead [Enable/Disable] [0~255(unit:Byte)]\n", (char *)p) ;
	return QDMAMGR_FAIL;
}

/******************************************************************************
 Descriptor:	It's used to set QDMA mac resource limit.
 Input Args:	see QdmaSetCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_set_mac_resource_limit(int argc, char *argv[], void *p)
{
	int ret=0;
	QDMA_MacLimitThrh_T qdma_mac_limit_thrh;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif
	
	if(argc<3)
		goto err;	
	
	qdma_mac_limit_thrh.chnlThrh = (uint)strtoul(argv[1], NULL, 10) ;
	qdma_mac_limit_thrh.queueThrh = (uint)strtoul(argv[2], NULL, 10) ;
	if((qdma_mac_limit_thrh.chnlThrh > 65536) || (qdma_mac_limit_thrh.queueThrh > 65536))
		goto err;
	
	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_set_mac_resource_limit(&qdma_mac_limit_thrh);
	} else if(strcmp((char *)p, QDMA_TYPE_WAN) == 0) {
		ret = qdmamgr_wan_lib_set_mac_resource_limit(&qdma_mac_limit_thrh);
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}
	
	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: 'dbgmgr %s set mac_limit' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;
	return QDMAMGR_SUCCESS;
	
err:
	printf("Format: dbgmgr %s set mac_limit [chnlThrh:0~65536] [queueThrh:0~65536]\n", (char *)p) ;
	return QDMAMGR_FAIL;
}

/******************************************************************************
 Descriptor:	It's used to set QDMA tx buffer threshold.
 Input Args:	see QdmaSetCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_set_txbuf_threshold(int argc, char *argv[], void *p)
{
	int ret=0;
	uint totalThrh=0, chnlThrh=0;
	QDMA_TxBufCtrl_T txBufCtrl;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif
	
	if(argc<2)
		goto err;
	
	if(strncasecmp(argv[1], "Enable", strlen("Enable")) == 0) {
		txBufCtrl.mode = QDMA_ENABLE;
	} else if(strncasecmp(argv[1], "Disable", strlen("Disable")) == 0) {
		txBufCtrl.mode = QDMA_DISABLE;
	} else {
		goto err;
	}

	if(txBufCtrl.mode == QDMA_ENABLE) {
		if(argc<4)
			goto err;
		
		totalThrh = (uint)strtoul(argv[2], NULL, 10) ;
		chnlThrh = (uint)strtoul(argv[3], NULL, 10) ;
		if((totalThrh > 63) || (chnlThrh > 63))
			goto err;
		txBufCtrl.totalThreshold = totalThrh<<2 ;
		txBufCtrl.chnThreshold = chnlThrh<<2 ;
	}
	
	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_set_txbuf_threshold(&txBufCtrl);
	} else if(strcmp((char *)p, QDMA_TYPE_WAN) == 0) {
		ret = qdmamgr_wan_lib_set_txbuf_threshold(&txBufCtrl);
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}
	
	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: 'dbgmgr %s set txbuf_thrh' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;
	return QDMAMGR_SUCCESS;
	
err:
	printf("Format-1: dbgmgr %s set txbuf_thrh Enable [totalThrh:0~63(unit:KByte)] [chnlThrh:0~63(unit:KByte)]\n", (char *)p) ;
	printf("Format-2: dbgmgr %s set txbuf_thrh Disable\n", (char *)p) ;
	return QDMAMGR_FAIL;
}

/******************************************************************************
 Descriptor:	It's used to set QDMA prefetch config.
 Input Args:	see QdmaSetCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_set_prefetch(int argc, char *argv[], void *p)
{
	int ret=0;
	QDMA_PrefetchMode_T prefetchCtrl;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif
	
	if(argc<4)
		goto err;
	
	if(strncasecmp(argv[1], "Enable", strlen("Enable")) == 0) {
		prefetchCtrl.prefecthMode = QDMA_ENABLE;
	} else if(strncasecmp(argv[1], "Disable", strlen("Disable")) == 0) {
		prefetchCtrl.prefecthMode = QDMA_DISABLE;
	} else {
		goto err;
	}
	
	if(strncasecmp(argv[2], "Enable", strlen("Enable")) == 0) {
		prefetchCtrl.overDragMode = QDMA_ENABLE;
	} else if(strncasecmp(argv[2], "Disable", strlen("Disable")) == 0) {
		prefetchCtrl.overDragMode = QDMA_DISABLE;
	} else {
		goto err;
	}
	
	prefetchCtrl.overDragCnt = (uint)strtoul(argv[3], NULL, 10) ;
	if(prefetchCtrl.overDragCnt > 32767)
		goto err;
	
	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_set_prefetch(&prefetchCtrl);
	} else if(strcmp((char *)p, QDMA_TYPE_WAN) == 0) {
		ret = qdmamgr_wan_lib_set_prefetch(&prefetchCtrl);
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}
	
	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: 'dbgmgr %s set prefetch' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;
	return QDMAMGR_SUCCESS;
	
err:
	printf("Format: dbgmgr %s set prefetch [Enable/Disable] [overDragEn: Enable/Disable] [overDragValue:0~32767(unit:Byte)]\n", (char *)p) ;
	return QDMAMGR_FAIL;
}

/******************************************************************************
 Descriptor:	It's used to set QDMA DBA Report.
 Input Args:	see QdmaSetCmds.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_set_dbareport(int argc, char *argv[], void *p)
{
	int ret=0, qdmaType=0;
	uint channel=0, channel_max=0;
	char *endPtr = NULL;
	QDMA_TxDbaReport_T txDbaReport;
	QDMA_TxRateLimitCfg_T txRateLimitCfg;
	
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
	
	if(argc<6)
		goto err;
	
	channel = (uint)strtoul(argv[1], &endPtr, 0);
	if((*endPtr!='\0') || (channel>=channel_max)) {
		printf("Error: input channel is '%s', which should between 0 and %d\n", argv[1], channel_max-1);
		return QDMAMGR_FAIL;
	}
	txDbaReport.channel = (unchar)channel;
	
	txDbaReport.cirParamValue = (uint)strtoul(argv[2], NULL, 10) ;
	txDbaReport.cbsParamValue = (uint)strtoul(argv[3], NULL, 10) ;
	txDbaReport.pirParamValue = (uint)strtoul(argv[4], NULL, 10) ;
	txDbaReport.pbsParamValue = (uint)strtoul(argv[5], NULL, 10) ;
	if((txDbaReport.cirParamValue > (4<<20)) || (txDbaReport.cbsParamValue > 65535)
	|| (txDbaReport.pirParamValue > (4<<20)) || (txDbaReport.pbsParamValue > 65535))
		goto err;
	
	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {
		ret = qdmamgr_lan_lib_get_tx_ratelimit_config(&txRateLimitCfg);
	} else if(strcmp((char *)p, QDMA_TYPE_WAN) == 0) {
		ret = qdmamgr_wan_lib_get_tx_ratelimit_config(&txRateLimitCfg);
	} else {
		printf("Exec. Failed: argc is %d\n", argc) ;
		return QDMAMGR_FAIL;
	}	
	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: 'dbgmgr %s set dbareport' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}
	
	txDbaReport.cbsParamValue <<= txRateLimitCfg.txRateLimitBucketScale ;
	txDbaReport.pbsParamValue <<= txRateLimitCfg.txRateLimitBucketScale ;
	
	if(strcmp((char *)p, QDMA_TYPE_LAN) == 0) {	
		ret = qdmamgr_lan_lib_set_dbareport(&txDbaReport);
	} else if(strcmp((char *)p, QDMA_TYPE_WAN) == 0) {
		ret = qdmamgr_wan_lib_set_dbareport(&txDbaReport);
	}	
	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: 'dbgmgr %s set dbareport' command failed\n", (char *)p) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;
	return QDMAMGR_SUCCESS;
	
err:
	printf("Format: dbgmgr %s set dbareport [ChnlIdx:0~%d] [cir:1~1000000(unit:kbps)] [cbs:0~65535] [pir:1~1000000(unit:kbps)] [pbs:0~65535]\n", (char *)p, channel_max-1) ;
	return QDMAMGR_FAIL;
}

