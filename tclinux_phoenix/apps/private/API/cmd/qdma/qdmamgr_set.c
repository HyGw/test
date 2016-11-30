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

#include "qdmamgr_set.h"

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
/* qos */
int do_set_qos(int argc, char* argv[], void *p);
int do_set_qos_config(int argc, char *argv[], void *p);
int do_set_qos_value(int argc, char *argv[], void *p);
/* tx ratelimit */
int do_set_txratelimit_value(int argc, char *argv[], void *p);
/* rx ratelimit */
int do_set_rxratelimit(int argc, char* argv[], void *p) ;
int do_set_rxratelimit_config(int argc, char *argv[], void *p);
int do_set_rxratelimit_value(int argc, char *argv[], void *p);
/* virtual channel */
int do_set_virtual_channel(int argc, char *argv[], void *p);
int do_set_virChnl_config(int argc, char *argv[], void *p);
int do_set_virChnl_value(int argc, char *argv[], void *p);
/* others */
int do_set_rx_low_thrh(int argc, char *argv[], void *p);
int do_set_dei_drop_en(int argc, char *argv[], void *p);

/************************************************************************
*                  P U B L I C   D A T A
************************************************************************/
QDMA_RxPktMode_t pktMode=0;


/************************************************************************
*                  P R I V A T E   D A T A
************************************************************************/
const cmds_t qdmamgr_set_command[] = {
/*	name,					func,						flags,	argcmin,		argc_errmsg */
	{"qos",					do_set_qos,					0,		0,			"Set QDMA qos information"},
	{"txratelimit",			do_set_txratelimit_value,	0,		0,			"Set QDMA tx ratelimit <chnlIdx, enMode, value>"},
	{"rxratelimit",			do_set_rxratelimit,			0,		0,			"Set QDMA rx ratelimit <config/value>"},
	{"virtual_channel",		do_set_virtual_channel,		0,		0,			"Set QDMA virtual channel <config/value>"},
	{"rx_low_thrh",			do_set_rx_low_thrh,			0,		0,			"Set QDMA rx low threshold <ring0LowThrh, ring1LowThrh>"},
	{"dei_drop_en",			do_set_dei_drop_en,			0,		0,			"Set QDMA dei drop <enable/disable>"},
	/* Cmd should be added before this line!  */
	{"help",				api_lib_help,				0,		0,			"Show qdmamgr set usage."},
	{NULL, NULL, 0, 0, NULL}
};

const cmds_t qos_set_command[] = {
/*	name,					func,						flags,	argcmin,		argc_errmsg */
	{"config",				do_set_qos_config,			0,		2,			"Set QDMA qos config <weightBase, weightScale>"},
	{"value",				do_set_qos_value,			0,		2,			"Set QDMA qos value <chnlIdx, qosType, weight0, ..., weight7>"},
	/* Cmd should be added before this line!  */
	{"help",				api_lib_help,				0,		0,			"Show qdmamgr set usage."},
	{NULL, NULL, 0, 0, NULL}
};

const cmds_t rxratelimit_set_command[] = {
/*	name,					func,						flags,	argcmin,		argc_errmsg */
	{"config",				do_set_rxratelimit_config,	0,		2,			"Set QDMA rx ratelimit config <enMode, pktMode>"},
	{"value",				do_set_rxratelimit_value,	0,		2,			"Set QDMA rx ratelimit value <ringIdx, value>"},
	/* Cmd should be added before this line!  */
	{"help",				api_lib_help,				0,		0,			"Show qdmamgr set usage."},
	{NULL, NULL, 0, 0, NULL}
};

const cmds_t virtual_channel_set_command[] = {
/*	name,					func,						flags,	argcmin,		argc_errmsg */
	{"config",				do_set_virChnl_config,		0,		2,			"Set QDMA virtual channel config <enMode, virChnlMode>"},
	{"value",				do_set_virChnl_value,		0,		2,			"Set QDMA virtual channel value <phyChnl, phyQosType, weight0, ..., weight7>"},
	/* Cmd should be added before this line!  */
	{"help",				api_lib_help,				0,		0,			"Show qdmamgr set usage."},
	{NULL, NULL, 0, 0, NULL}
};

static unchar qosTypeStr[QDMA_TXQOS_TYPE_NUMS][16] = {
	"WRR",
	"SP",
	"SPWRR7", 
	"SPWRR6", 
	"SPWRR5", 
	"SPWRR4", 
	"SPWRR3",
	"SPWRR2",
};

static unchar vChnlQosTypeStr[QDMA_VCHNL_TXQOS_TYPE_NUMS][16] = {
	"WRR",
	"SP",
	"SPWRR3",
	"SPWRR2",
};

/************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
************************************************************************/

/******************************************************************************
 Descriptor:	It's used to set qdma qos status.
 Input Args:	see qdmamgr_set_command.
 Ret Value: 	none.
******************************************************************************/
int do_set_qos(int argc, char* argv[], void *p)
{
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	return subcmd(qos_set_command, argc, argv, (void *)qos_set_command);
}

/******************************************************************************
 Descriptor:	It's used to set qdma qos config.
 Input Args:	see qos_set_command.
 Ret Value: none.
******************************************************************************/
int do_set_qos_config(int argc, char *argv[], void *p)
{
	QDMA_TxQosWeightType_t weightBase;
	QDMA_TxQosWeightScale_t weightScale;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(argc<3)
		goto err;
	
	if(strncasecmp(argv[1], "packet", strlen("packet")) == 0) {
		weightBase = QDMA_TXQOS_WEIGHT_BY_PACKET ;
	} else if(strncasecmp(argv[1], "byte", strlen("byte")) == 0) {
		weightBase = QDMA_TXQOS_WEIGHT_BY_BYTE ;
	} else {
		goto err;
	}
	
	if(strncasecmp(argv[2], "64Byte", strlen("64Byte")) == 0) {
		weightScale = QDMA_TXQOS_WEIGHT_SCALE_64B ;
	} else if(strncasecmp(argv[2], "16Byte", strlen("16Byte")) == 0) {
		weightScale = QDMA_TXQOS_WEIGHT_SCALE_16B ;
	} else {
		goto err;
	}

	if(qdmamgr_lib_set_qos_config(weightBase, weightScale) == QDMAMGR_FAIL) {
		printf("Exec. Failed: '%s set qos config [weightBase] [weightScale]' command failed\n", qdmamgr_name) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;
	return QDMAMGR_SUCCESS;

err:
	printf("Format: %s set qos config [packet/byte] [64Byte/16Byte]\n", qdmamgr_name) ;
	return QDMAMGR_FAIL;
}

/******************************************************************************
 Descriptor:	It's used to set qdma qos value.
 Input Args:	see qos_set_command.
 Ret Value: none.
******************************************************************************/
int do_set_qos_value(int argc, char *argv[], void *p)
{
	int idx=0, weight=0;
	char *endPtr = NULL;
	QDMA_TxQosScheduler_T txQos;
	int argcMin[QDMA_TXQOS_TYPE_NUMS]={11, 3, 10, 9, 8, 7, 6, 5};
		
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	/* get channel idx */
	txQos.channel = strtoul(argv[1], &endPtr, 0);
	if((*endPtr!='\0') || (txQos.channel>=CONFIG_QDMA_CHANNEL)) {
		printf("Error: input channel is '%s', which should between 0 and %d\n", argv[1], CONFIG_QDMA_CHANNEL-1);
		goto err;
	}
	
	/* get qos type */
	for(idx=0; idx<QDMA_TXQOS_TYPE_NUMS; idx++) {
		if(strncasecmp(argv[2], (char *)qosTypeStr[idx], strlen((char *)argv[2])) == 0) {
			txQos.qosType = idx;
			break;
		}
	}
	if(txQos.qosType == QDMA_TXQOS_TYPE_NUMS) /* qosType error */
		goto err;
	
	/* get wrr weight */
	if(argc < argcMin[txQos.qosType]) {
		printf("Fault: qosType %s need at least %d wrr weight\n", qosTypeStr[txQos.qosType], argcMin[txQos.qosType]-3);
		goto err;
	}
	for(idx=0; idx<QDMA_TXQOS_TYPE_NUMS; idx++) {
		/* skipped the redundant wrr weight */
		if(idx >= argcMin[txQos.qosType]-3)
			break;
		weight = (unchar)strtol(argv[3+idx], &endPtr, 0);
		if((*endPtr!='\0') || (weight < 1) || (weight > 100)) {
			printf("Fault: qos weight should not be 0, which should between 1 and 100!\n");
			goto err;
		}
		txQos.queue[idx].weight = (unchar)weight;
	}

	/* set qos value */
	if(qdmamgr_lib_set_qos_value(&txQos) == QDMAMGR_FAIL) {
		printf("Exec. Failed: '%s set qos value [ChnlID] [qosType] [weight0~weight7]' command failed\n", qdmamgr_name) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;
	return QDMAMGR_SUCCESS;

err:
	printf("Format: %s set qos value [ChnlID:0~%d] [SP/WRR/SPWRR2/../SPWRR7] [weight0:1~100] ... [weightn:1~100]\n", qdmamgr_name, CONFIG_QDMA_CHANNEL-1) ;
	return QDMAMGR_FAIL;
}

/******************************************************************************
 Descriptor:	It's used to set tx rate limit value.
 Input Args:	see qdmamgr_set_command.
 Ret Value: none.
******************************************************************************/
int do_set_txratelimit_value(int argc, char *argv[], void *p)
{
	char *endPtr = NULL;
	QDMA_TxRateLimitSet_T txRateLimitSet;
		
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(argc<4)
		goto err;

	/* get channel idx */
	txRateLimitSet.chnlIdx = strtoul(argv[1], &endPtr, 0);
	if((*endPtr!='\0') || (txRateLimitSet.chnlIdx>=CONFIG_QDMA_CHANNEL)) {
		printf("Error: input channel is '%s', which should between 0 and %d\n", argv[1], CONFIG_QDMA_CHANNEL-1);
		goto err;
	}
	
	/* get tx rate limit enable/disable */	
	if(strncasecmp(argv[2], "enable", strlen("enable")) == 0) {
		txRateLimitSet.chnlRateLimitEn = QDMA_ENABLE;
	} else if(strncasecmp(argv[2], "disable", strlen("disable")) == 0) {
		txRateLimitSet.chnlRateLimitEn = QDMA_DISABLE;
	} else {
		goto err;
	}
	
	/* get tx rate limit value in kbps */
	txRateLimitSet.rateLimitValue = strtoul(argv[3], &endPtr, 0);
	if(*endPtr!='\0') {
		printf("Error: input tx rate limit value is '%s', which should be digital\n", argv[3]);
		goto err;
	}
	if(txRateLimitSet.rateLimitValue > (4<<20)) {
		printf("Error: tx rate limit value is %dkbps, which shold less than 4gbps!", txRateLimitSet.rateLimitValue);
		goto err;
	}

	/* set tx rate limit value */
	if(qdmamgr_lib_set_tx_ratelimit_value(&txRateLimitSet) == QDMAMGR_FAIL) {
		printf("Exec. Failed: '%s set txratelimit [ChnlID] [Enable/Disable] [pir(unit:kbps)]' command failed\n", qdmamgr_name) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful and the accuracy is 8kbps\n") ;
	return QDMAMGR_SUCCESS;

err:
	printf("Format: %s set txratelimit [ChnlID:0~%d] [Enable/Disable] [1~1000000(unit:kbps)]\n", qdmamgr_name, CONFIG_QDMA_CHANNEL-1) ;
	return QDMAMGR_FAIL;
}

/******************************************************************************
 Descriptor:	It's used to set qdma rx rate limit info.
 Input Args:	see qdmamgr_set_command.
 Ret Value: 	none.
******************************************************************************/
int do_set_rxratelimit(int argc, char* argv[], void *p)
{
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	return subcmd(rxratelimit_set_command, argc, argv, (void *)rxratelimit_set_command);
}

/******************************************************************************
 Descriptor:	It's used to set rx rate limit config.
 Input Args:	see rxratelimit_set_command.
 Ret Value: none.
******************************************************************************/
int do_set_rxratelimit_config(int argc, char *argv[], void *p)
{
	QDMA_Mode_t rxRateLimitEn=0;
		
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(argc<3)
		goto err;

	/* get rx rate limit enable mode */
	if(strncasecmp(argv[1], "enable", strlen("enable")) == 0) {
		rxRateLimitEn = QDMA_ENABLE;
	} else if(strncasecmp(argv[1], "disable", strlen("disable")) == 0) {
		rxRateLimitEn = QDMA_DISABLE;
	} else {
		goto err;
	}
		
	/* get rx rate limit packet mode */
	if(strncasecmp(argv[2], "packet", strlen("packet")) == 0) {
		pktMode = QDMA_ENABLE;
	} else if(strncasecmp(argv[2], "byte", strlen("byte")) == 0) {
		pktMode = QDMA_DISABLE;
	} else {
		goto err;
	}

	/* set rx rate limit config */
	if(qdmamgr_lib_set_rx_ratelimit_en(rxRateLimitEn) == QDMAMGR_FAIL) {
		printf("Exec. Failed: '%s set rxratelimit config [enMode] [pktMode]' command failed\n", qdmamgr_name) ;
		return QDMAMGR_FAIL;
	}
	
	if(qdmamgr_lib_set_rx_ratelimit_pkt_mode(pktMode) == QDMAMGR_FAIL) {
		printf("Exec. Failed: '%s set rxratelimit config [enMode] [pktMode]' command failed\n", qdmamgr_name) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;
	return QDMAMGR_SUCCESS;

err:
	printf("Format: %s set rxratelimit config [Enable/Disable] [packet/byte]\n", qdmamgr_name) ;
	return QDMAMGR_FAIL;
}

/******************************************************************************
 Descriptor:	It's used to set tx rate limit value.
 Input Args:	see rxratelimit_set_command.
 Ret Value: none.
******************************************************************************/
int do_set_rxratelimit_value(int argc, char *argv[], void *p)
{
	char *endPtr = NULL;
	char pktModeStr[64];
	QDMA_RxRateLimitSet_T rxRateLimitSet;
		
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(argc<3)
		goto err;
	if(pktMode == QDMA_ENABLE)
		memcpy(pktModeStr, "pps", sizeof("pps"));
	else
		memcpy(pktModeStr, "kbps", sizeof("kbps"));

	/* get channel idx */
	rxRateLimitSet.ringIdx = strtoul(argv[1], &endPtr, 0);
	if((*endPtr!='\0') || (rxRateLimitSet.ringIdx>=RX_RING_NUM)) {
		printf("Error: input ringIdx is '%s', which should between 0 and 1\n", argv[1]);
		goto err;
	}
	
	/* get rx rate limit value in kbps */
	rxRateLimitSet.rateLimitValue = strtoul(argv[2], &endPtr, 0);
	if(*endPtr!='\0') {
		printf("Error: input rx rate limit value is '%s', which should be digital\n", argv[2]);
		goto err;
	}
	if(rxRateLimitSet.rateLimitValue > (4<<20)) {
		printf("Error: rx rate limit value is %d%s, which shold less than 4000000%s!", rxRateLimitSet.rateLimitValue, pktModeStr, pktModeStr);
		goto err;
	}

	/* set rx rate limit value */
	if(qdmamgr_lib_set_rx_ratelimit_value(&rxRateLimitSet) == QDMAMGR_FAIL) {
		printf("Exec. Failed: '%s set rxratelimit value [ChnlID] [pir(unit:%s)]' command failed\n", qdmamgr_name, pktModeStr) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful and the accuracy is 8kbps\n") ;
	return QDMAMGR_SUCCESS;

err:
	printf("Format: %s set rxratelimit value [RingIdx:0~%d] [1~1000000(unit:%s)]\n", qdmamgr_name, RX_RING_NUM-1, pktModeStr) ;
	return QDMAMGR_FAIL;
}

/******************************************************************************
 Descriptor:	It's used to set qdma virtual chanenl info.
 Input Args:	see qdmamgr_set_command.
 Ret Value: 	none.
******************************************************************************/
int do_set_virtual_channel(int argc, char* argv[], void *p)
{
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	return subcmd(virtual_channel_set_command, argc, argv, (void *)virtual_channel_set_command);
}

/******************************************************************************
 Descriptor:	It's used to set qdma virtual channel config.
 Input Args:	see virtual_channel_set_command.
 Ret Value: none.
******************************************************************************/
int do_set_virChnl_config(int argc, char *argv[], void *p)
{
	QDMA_VirtualChannelMode_T virChnlMode;
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(argc<3)
		goto err;

	if(strncasecmp(argv[1], "Enable", strlen("Enable")) == 0) {
		virChnlMode.virChnlEn = QDMA_ENABLE ;
	} else if(strncasecmp(argv[1], "Disable", strlen("Disable")) == 0) {
		virChnlMode.virChnlEn = QDMA_DISABLE ;
	} else {
		goto err;
	}
	
	if(strncasecmp(argv[2], "32Queue", strlen("32Queue")) == 0) {
		virChnlMode.virChnlMode = QDMA_VIRTUAL_CHANNEL_32Queue ;
	} else if(strncasecmp(argv[2], "16Queue", strlen("16Queue")) == 0) {
		virChnlMode.virChnlMode = QDMA_VIRTUAL_CHANNEL_16Queue ;
	} else {
		goto err;
	}

	if(qdmamgr_lib_set_virtual_channel_config(&virChnlMode) == QDMAMGR_FAIL) {
		printf("Exec. Failed: '%s set virtual_channel config [enMode] [virChnlMode]' command failed\n", qdmamgr_name) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;
	return QDMAMGR_SUCCESS;

err:
	printf("Format: %s set virtual_channel config [Enable/Disable] [32Queue/16Queue]\n", qdmamgr_name) ;
	return QDMAMGR_FAIL;
}

/******************************************************************************
 Descriptor:	It's used to set qdma virtual channel value.
 Input Args:	see virtual_channel_set_command.
 Ret Value: none.
******************************************************************************/
int do_set_virChnl_value(int argc, char *argv[], void *p)
{
	int idx=0;
	char *endPtr = NULL;
	QDMA_VirtualChannelQoS_T virChnlQoS;
	int argcMin[QDMA_VCHNL_TXQOS_TYPE_NUMS]={7, 3, 6, 5};
		
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	/* get channel idx */
	virChnlQoS.phyChnl = strtoul(argv[1], &endPtr, 0);
	if((*endPtr!='\0') || (virChnlQoS.phyChnl>=(CONFIG_QDMA_CHANNEL>>1))) {
		printf("Error: input physical channel is '%s', which should between 0 and %d\n", argv[1], (CONFIG_QDMA_CHANNEL>>1)-1);
		goto err;
	}
	
	/* get qos type */
	for(idx=0; idx<QDMA_VCHNL_TXQOS_TYPE_NUMS; idx++) {
		if(strncasecmp(argv[2], (char *)vChnlQosTypeStr[idx], strlen(argv[2])) == 0) {
			virChnlQoS.qosType = idx;
			break;
		}
	}
	if(virChnlQoS.qosType == QDMA_TXQOS_TYPE_NUMS) /* qosType error */
		goto err;
	
	/* get wrr weight */
	if(argc < argcMin[virChnlQoS.qosType]) {
		printf("Fault: qosType %s need at least %d wrr weight\n", vChnlQosTypeStr[virChnlQoS.qosType], argcMin[virChnlQoS.qosType]-3);
		goto err;
	}
	for(idx=0; idx<QDMA_VCHNL_TXQOS_TYPE_NUMS; idx++) {
		/* skipped the redundant wrr weight */
		if(idx >= argcMin[virChnlQoS.qosType]-3)
			break;
		virChnlQoS.queue[idx].weight = (unchar)strtoul(argv[3+idx], &endPtr, 0);
		if((*endPtr!='\0') || (virChnlQoS.queue[idx].weight == 0)) {
			printf("Fault: qos weight should not be 0, which should between 1 and 100!\n");
			goto err;
		}
	}

	/* set qos value */
	if(qdmamgr_lib_set_virtual_channel_value(&virChnlQoS) == QDMAMGR_FAIL) {
		printf("Exec. Failed: '%s set virtual_channel value [phyChnlID] [phyQosType] [weight0~weight3]' command failed\n", qdmamgr_name) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;
	return QDMAMGR_SUCCESS;

err:
	printf("Format: %s set qos virtual_channel [phyChnlID:0~%d] [SP/WRR/SPWRR2/SPWRR3] [weight0:1~100] ... [weightn:1~100]\n", qdmamgr_name, (CONFIG_QDMA_CHANNEL>>1)-1) ;
	return QDMAMGR_FAIL;
}

/******************************************************************************
 Descriptor:	It's used to set qdma txq cngst dei enable or not.
 Input Args:	see qdmamgr_set_command.
 Ret Value: none.
******************************************************************************/
int do_set_rx_low_thrh(int argc, char *argv[], void *p)
{
	char *endPtr = NULL;
	QDMA_RxLowThreshold_T rxLowThreshold ;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif
 
	if(argc<3)
		goto err;
	
	rxLowThreshold.rxRing0LowThrh = strtoul(argv[1], &endPtr, 0);
	if(*endPtr!='\0') {
		printf("Error: input ring0 low threshold is '%s', which should be digital\n", argv[1]);
		goto err;
	}
	
	rxLowThreshold.rxRing1LowThrh = strtoul(argv[2], &endPtr, 0);
	if(*endPtr!='\0') {
		printf("Error: input ring1 low threshold is '%s', which should be digital\n", argv[2]);
		goto err;
	}
	
	if(qdmamgr_lib_set_rx_low_threshold(&rxLowThreshold) == QDMAMGR_FAIL) {
		printf("Exec. Failed: '%s set rx_low_thrh [ring0LowThrh] [ring1LowThrh]' command failed\n", qdmamgr_name) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;
	return QDMAMGR_SUCCESS;

err:
	printf("Format: %s set rx_low_thrh [ring0LowThrh] [ring1LowThrh]\n", qdmamgr_name) ;
	return QDMAMGR_FAIL;
}

/******************************************************************************
 Descriptor:	It's used to set qdma txq cngst dei enable or not.
 Input Args:	see qdmamgr_set_command.
 Ret Value: none.
******************************************************************************/
int do_set_dei_drop_en(int argc, char *argv[], void *p)
{
	QDMA_Mode_t deiDropMode = 0;
	
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(argc<2)
		goto err;

	if(strncasecmp(argv[1], "Enable", strlen("Enable")) == 0) {
		deiDropMode = QDMA_ENABLE;
	} else if(strncasecmp(argv[1], "Disable", strlen("Disable")) == 0) {
		deiDropMode = QDMA_DISABLE;
	} else {
		goto err;
	}
	
	if(qdmamgr_lib_set_dei_drop_en(deiDropMode) == QDMAMGR_FAIL) {
		printf("Exec. Failed: '%s set dei_drop_en [enMode]' command failed\n", qdmamgr_name) ;
		return QDMAMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;
	return QDMAMGR_SUCCESS;

err:
	printf("Format: %s set dei_drop_en [Enable/Disable]\n", qdmamgr_name) ;
	return QDMAMGR_FAIL;
}

