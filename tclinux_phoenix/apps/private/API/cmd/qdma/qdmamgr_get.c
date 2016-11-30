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

#include "qdmamgr_get.h"

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
/* qos */
int do_get_qos(int argc, char* argv[], void *p);
/* tx ratelimit */
int do_get_txratelimit(int argc, char *argv[], void *p);
/* rx ratelimit */
int do_get_rxratelimit(int argc, char *argv[], void *p);
/* virtual channel */
int do_get_virtual_channel(int argc, char *argv[], void *p);
/* others */
int do_get_rx_low_threshold(int argc, char *argv[], void *p);
int do_get_dei_drop_en(int argc, char* argv[], void *p);


/************************************************************************
*                  P U B L I C   D A T A
************************************************************************/

/************************************************************************
*                  P R I V A T E   D A T A
************************************************************************/
const cmds_t qdmamgr_get_command[] = {
/*	name,					func,						flags,	argcmin,		argc_errmsg */
	{"qos",					do_get_qos,					0,		0,			"Get QDMA qos information"},
	{"txratelimit",			do_get_txratelimit,			0,		0,			"Get QDMA tx rate limit infomation "},
	{"rxratelimit",			do_get_rxratelimit,			0,		0,			"Get QDMA rx rate limit information"},
	{"virtual_channel",		do_get_virtual_channel,		0,		0,			"Get QDMA virtual channel information"},
	{"rx_low_thrh",			do_get_rx_low_threshold,	0,		0,			"Get QDMA rx low threshold"},
	{"dei_drop_en",			do_get_dei_drop_en,			0,		0,			"Get QDMA dei drop enable or not"},
	/* Cmd should be added before this line!  */
	{"help",				api_lib_help,				0,		0,			"Show qdmamgr get usage."},
	{NULL, NULL, 0, 0, NULL}
};


/************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
************************************************************************/

/******************************************************************************
 Descriptor:	It's used to get qdma qos value.
 Input Args:	see qdmamgr_get_command.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_get_qos(int argc, char *argv[], void *p)
{
	char *endPtr = NULL;
	QDMA_TxQosScheduler_T TxQos;
	QDMA_TxQos_T getTxQos;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	/* check argc */
	if (argc < 2)
	{
		printf("Format: %s get qos value [ChnlID:0~%d]\n", qdmamgr_name, CONFIG_QDMA_CHANNEL-1);
		return QDMAMGR_FAIL;
	}
	else if (argc >= 2)
	{
		TxQos.channel = strtol(argv[1],&endPtr,0);
		
		if ((TxQos.channel >= CONFIG_QDMA_CHANNEL) || (*endPtr != '\0'))
		{
			printf("Error: input channel is '%s', which should between 0 and %d\n",argv[1],CONFIG_QDMA_CHANNEL-1);
			return QDMAMGR_FAIL;
		}
	}

	/* get qos parameter */
	getTxQos.pTxQos = &TxQos;
	
	if (QDMAMGR_FAIL == qdmamgr_lib_get_qos(&getTxQos))
	{
		printf("Exec. Failed: '%s get qos value' command failed\n", qdmamgr_name);
		return QDMAMGR_FAIL;
	}

	/* output the parameter for test */
	switch (getTxQos.pTxQos->qosType)
	{
		case QDMA_TXQOS_TYPE_WRR:
			printf("channel-%d: qosType = WRR\n",TxQos.channel);
			printf("q7:q6:q5:q4:q3:q2:q1:q0 = %d:%d:%d:%d:%d:%d:%d:%d\n",
				getTxQos.pTxQos->queue[7].weight,
				getTxQos.pTxQos->queue[6].weight,
				getTxQos.pTxQos->queue[5].weight,
				getTxQos.pTxQos->queue[4].weight,
				getTxQos.pTxQos->queue[3].weight,
				getTxQos.pTxQos->queue[2].weight,
				getTxQos.pTxQos->queue[1].weight,
				getTxQos.pTxQos->queue[0].weight);
			break;
		case QDMA_TXQOS_TYPE_SP:
			printf("channel-%d: qosType = SP\n",TxQos.channel);
			break;
		case QDMA_TXQOS_TYPE_SPWRR7:
			printf("channel-%d: qosType = SPWRR7\n",TxQos.channel);
			printf("q6:q5:q4:q3:q2:q1:q0 = %d:%d:%d:%d:%d:%d:%d\n",
				getTxQos.pTxQos->queue[6].weight,
				getTxQos.pTxQos->queue[5].weight,
				getTxQos.pTxQos->queue[4].weight,
				getTxQos.pTxQos->queue[3].weight,
				getTxQos.pTxQos->queue[2].weight,
				getTxQos.pTxQos->queue[1].weight,
				getTxQos.pTxQos->queue[0].weight);
			break;
		case QDMA_TXQOS_TYPE_SPWRR6:
			printf("channel-%d: qosType = SPWRR6\n",TxQos.channel);
			printf("q5:q4:q3:q2:q1:q0 = %d:%d:%d:%d:%d:%d\n",
				getTxQos.pTxQos->queue[5].weight,
				getTxQos.pTxQos->queue[4].weight,
				getTxQos.pTxQos->queue[3].weight,
				getTxQos.pTxQos->queue[2].weight,
				getTxQos.pTxQos->queue[1].weight,
				getTxQos.pTxQos->queue[0].weight);
			break;
		case QDMA_TXQOS_TYPE_SPWRR5:
			printf("channel-%d: qosType = SPWRR5\n",TxQos.channel);
			printf("q4:q3:q2:q1:q0 = %d:%d:%d:%d:%d\n",
				getTxQos.pTxQos->queue[4].weight,
				getTxQos.pTxQos->queue[3].weight,
				getTxQos.pTxQos->queue[2].weight,
				getTxQos.pTxQos->queue[1].weight,
				getTxQos.pTxQos->queue[0].weight);
			break;
		case QDMA_TXQOS_TYPE_SPWRR4:
			printf("channel-%d: qosType = SPWRR4\n",TxQos.channel);
			printf("q3:q2:q1:q0 = %d:%d:%d:%d\n",
				getTxQos.pTxQos->queue[3].weight,
				getTxQos.pTxQos->queue[2].weight,
				getTxQos.pTxQos->queue[1].weight,
				getTxQos.pTxQos->queue[0].weight);
			break;
		case QDMA_TXQOS_TYPE_SPWRR3:
			printf("channel-%d: qosType = SPWRR3\n",TxQos.channel);
			printf("q2:q1:q0 = %d:%d:%d\n",
				getTxQos.pTxQos->queue[2].weight,
				getTxQos.pTxQos->queue[1].weight,
				getTxQos.pTxQos->queue[0].weight);
			break;
		case QDMA_TXQOS_TYPE_SPWRR2:
			printf("channel-%d: qosType = SPWRR2\n",TxQos.channel);
			printf("q1:q0 = %d:%d\n",
				getTxQos.pTxQos->queue[1].weight,
				getTxQos.pTxQos->queue[0].weight);
			break;
		default:
			printf("channel-%d: qosType is not correct\n",TxQos.channel);
			break;
	}
	
	printf("weightBase = %s\n",
		((QDMA_TXQOS_WEIGHT_BY_PACKET == getTxQos.weightBase) ? ("packet mode"):("byte mode")));
	printf("weightScale = %s\n",
		((QDMA_TXQOS_WEIGHT_SCALE_64B == getTxQos.weightScale) ? ("64Byte"):("16Byte")));
	
	return QDMAMGR_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to get qdma tx rate limit value.
 Input Args:	see qdmamgr_get_command.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_get_txratelimit(int argc, char *argv[], void *p)
{
	int ret=0, channel=0;
	char *endPtr = NULL;
	QDMA_TxRateLimitGet_T txRateLimit;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	/* check argc */
	if (argc < 2)
	{
		printf("Format: %s get txratelimit [ChnlID:0~%d]\n",qdmamgr_name,CONFIG_QDMA_CHANNEL-1);
		return QDMAMGR_FAIL;
	}
	else if (argc >= 2)
	{
		txRateLimit.chnlIdx = strtol(argv[1],&endPtr,0);
		
		if ((txRateLimit.chnlIdx >= CONFIG_QDMA_CHANNEL) || (*endPtr!='\0'))
		{
			printf("Error: input channel is '%s', which should between 0 and %d\n",argv[1],CONFIG_QDMA_CHANNEL-1);
			return QDMAMGR_FAIL;
		}
	}

	/* get tx ratelimit parameter */
	if (QDMAMGR_FAIL == qdmamgr_lib_get_tx_ratelimit(&txRateLimit))
	{
		printf("Exec. Failed: '%s get txratelimit' command failed\n", qdmamgr_name);
		return QDMAMGR_FAIL;
	}

	/* output the parameter for test */
	printf("Channel-%d: txRateLimit mode = %s\n",txRateLimit.chnlIdx,
	((txRateLimit.chnlRateLimitEn) ? ("Enable"):("Disable")));
	
	printf("rateLimit Value = %ukbps\n",txRateLimit.rateLimitValue);
	printf("pbs Value = %ubyte\n",txRateLimit.pbsParamValue);
	
	return QDMAMGR_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to get qdma rx rate limit value.
 Input Args:	see qdmamgr_get_command.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_get_rxratelimit(int argc, char *argv[], void *p)
{
	int ret=0, ringIdx=0;
	char *endPtr = NULL;
	QDMA_RxRateLimitGet_T rxRateLimit;
	QDMA_RxRateLimitCfg_T rxRateLimitCfg;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	/* check argc */
	if (argc < 2)
	{
		printf("Format: %s get rxratelimit [RingID:0~%d]\n",qdmamgr_name,RX_RING_NUM-1);
		return QDMAMGR_FAIL;
	}
	else if (argc >= 2)
	{
		rxRateLimit.ringIdx = strtol(argv[1],&endPtr,0);
		
		if ((rxRateLimit.ringIdx >= RX_RING_NUM) || (*endPtr!='\0'))
		{
			printf("Error: input ringIdx is '%s', which should between 0 and 1\n",argv[1]);
			return QDMAMGR_FAIL;
		}
	}

	/* get rx ratelimit parameter */
	if (QDMAMGR_FAIL == qdmamgr_lib_get_rx_ratelimit(&rxRateLimit,&rxRateLimitCfg))
	{
		printf("Exec. Failed: '%s get rxratelimit' command failed\n",qdmamgr_name);
		return QDMAMGR_FAIL;
	}

	/* output the parameter for test */
	printf("rxRateLimit: enable mode = %s\n",((rxRateLimitCfg.rxRateLimitEn) ? ("Enable"):("Disable")));
	printf("rxRateLimit: packet mode = %s\n",((rxRateLimitCfg.rxRateLimitPktMode) ? ("packet"):("byte")));
	
	printf("RingIdx-%d: ratelimit value:%u%s\n",rxRateLimit.ringIdx,rxRateLimit.rateLimitValue,
	((rxRateLimitCfg.rxRateLimitPktMode) ? ("pps"):("kbps")));
	
	printf("pbs value = %ubyte\n",rxRateLimit.pbsParamValue);
	
	return QDMAMGR_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to get qdma virtual channel information.
 Input Args:	see qdmamgr_get_command.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_get_virtual_channel(int argc, char *argv[], void *p)
{
	int ret=0, phyChnl=0;
	char *endPtr = NULL;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(argc>=2) {
		phyChnl = strtol(argv[1], &endPtr, 0);
		if((*endPtr!='\0') || (phyChnl>CONFIG_QDMA_CHANNEL)) {
			printf("Error: input physical channel is '%s', which should between 0 and %d\n", argv[1], CONFIG_QDMA_CHANNEL);
			return QDMAMGR_FAIL;
		}
	}

	if((argc<2) || (phyChnl == CONFIG_QDMA_CHANNEL)) {
		ret = qdmamgr_lib_get_virtual_channel(CONFIG_QDMA_CHANNEL) ;
	} else if(phyChnl < CONFIG_QDMA_CHANNEL){
		ret = qdmamgr_lib_get_virtual_channel(phyChnl) ;
	} else {
		printf("Format: %s get virtual_channel [ChnlID:0~%d]\n", qdmamgr_name, CONFIG_QDMA_CHANNEL>>1) ;
	}
	
	if(ret == QDMAMGR_FAIL) {
		printf("Exec. Failed: '%s get virtual_channel' command failed\n", qdmamgr_name) ;
		return QDMAMGR_FAIL;
	}

	return QDMAMGR_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to get qdma rx low threshold value.
 Input Args:	see qdmamgr_get_command.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_get_rx_low_threshold(int argc, char *argv[], void *p)
{
	QDMA_RxLowThreshold_T rxLowThreshold;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(qdmamgr_lib_get_rx_low_threshold(&rxLowThreshold) == QDMAMGR_FAIL) {
		printf("Exec. Failed: '%s get rx_low_thrh'\n", qdmamgr_name) ;
		return QDMAMGR_FAIL;
	}

	printf("rx low threshold:  Ring0:%u, Ring1:%u\n", rxLowThreshold.rxRing0LowThrh, rxLowThreshold.rxRing1LowThrh);
	return QDMAMGR_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to get qdma dei drop bit enable or not.
 Input Args:	see qdmamgr_get_command.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int do_get_dei_drop_en(int argc, char *argv[], void *p)
{
	QDMA_Mode_t dei_drop_en =0;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(qdmamgr_lib_get_dei_drop_en(&dei_drop_en) == QDMAMGR_FAIL) {
		printf("Exec. Failed: '%s get dei_drop_en'\n", qdmamgr_name) ;
		return QDMAMGR_FAIL;
	}

	printf("dei drop is %s\n", ((dei_drop_en==QDMA_ENABLE)?("Enable"):("Disable")));
	return QDMAMGR_SUCCESS;
}

