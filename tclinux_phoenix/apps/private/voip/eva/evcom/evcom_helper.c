/*********************************************************************
* LEGAL DISCLAIMER
*
* (Header of MediaTek Software/Firmware Release or Documentation)
*
* BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY 
* ACKNOWLEDGES AND AGREES THAT THE SOFTWARE/FIRMWARE AND ITS 
* DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED FROM MEDIATEK AND/OR 
* ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS ONLY. 
* MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR 
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF 
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR 
* NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY 
* WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH 
* MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK 
* SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY
* WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO NOT BE 
* RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S 
* SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
* BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND 
* CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED 
* HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE 
* MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR 
* SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE
* AT ISSUE.
*
* THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN 
* ACCORDANCE WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING
* ITS CONFLICT OF LAWS PRINCIPLES.
**********************************************************************
*/

/*
 *	File:	evcom_helper.c
 *	 
 *	Author: PT Chen, Quark Li
 * 		
 *	Last Revision Date: 2011.9.29
 *	 
 *	Description:
 *	EVA Console Manager CLI Tool, data validation and conversion toolkit.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "evcom.h"
#include "evcom_helper.h"

extern const char * const gValidMWIStrVal[MTK_EVA_MWI_VALUE_MAX];


evaBool_e 
isValidHexNum(const char *inStr, int min, int max) {
	char tmp[10];
	int	x = strtol(inStr,NULL,16);

	sprintf(tmp, "%x", x);
	if (!strcasecmp(inStr, tmp)) {
		if (((x >= min) && (x <= max)) || ((min == MAX32) && (max == MAX32))) { 
		/* min == max == MAX32, means no limit on the range of value */
			return EVA_TRUE;
		}
	}
	
	return EVA_FALSE;
}

evaBool_e 
isValidNum(const char *inStr, int min, int max) {
	char tmp[10];
	int	x = atoi(inStr);

	sprintf(tmp, "%d", x);
	
	if (!strcasecmp(inStr, tmp)) {
		if (((x >= min) && (x <= max)) || ((min == MAX32) && (max == MAX32))) { 
		/* min == max == MAX32, means no limit on the range of value */
			return EVA_TRUE;
		}
	}
	
	return EVA_FALSE;
}

evaBool_e 
isValidIp(const char* inStr) {	
	struct sockaddr_in sa;
	int result = inet_pton(AF_INET, inStr, &(sa.sin_addr));
	
	if (result == 0) {
		printf("Incorrect network address! i.e. ip=192.168.1.1:5000\n\n");
		return EVA_FALSE;
	}
	
	return EVA_TRUE;
}

evaBool_e 
isValidCodec(const char* inStr) {
	if (CODEC_INVALID != convertCodec(inStr)) {
		return EVA_TRUE;
	}

	//printf("Incorrect codec! codec: g711a | g711u | g722 | g726 | g729| t38\n\n"); //isabella_g7231
	printf("Incorrect codec! codec: g711a | g711u | g722 | g7231 | g726 | g729 | t38| vbd-pcma| vbd-pcmu\n\n"); //isabella_g7231
	return EVA_FALSE;
}

evaBool_e 
isValidPtime(const char* inStr) {
	if (convertPtime(inStr) >= 0) {
		return EVA_TRUE;
	}

	printf("Incorrect ptime! ptime: 10 | 20 | 30 | 40\n\n");
	return EVA_FALSE;
}

evaBool_e 
isValidOnOff(const char* inStr) {
	if (!strcasecmp(inStr, "on") || !strcasecmp(inStr, "off")) {
		return EVA_TRUE;
	}
	
	return EVA_FALSE;
}

evaBool_e 
isValidStrmdir(const char* inStr) {
	if (convertStrmdir(inStr) >= 0) {
		return EVA_TRUE;
	}
	
	printf("Incorrect stream direction! dir: sr | so | ro | in\n\n");
	return EVA_FALSE;
}

evaBool_e 
isValidDtmf(const char* inStr) {
	if (convertDtmf(inStr) >= 0) { 
		return EVA_TRUE;
	}

	printf("Incorrect DTMF key! dtmf: <0~9> | * | # | a | b | c | d\n\n");
	return EVA_FALSE;
}

evaBool_e 
isValidChanId(const char* inStr) {
	if (isValidNum(inStr, CHAN_FIRST, CHAN_LAST)) {
		return EVA_TRUE;
	}

	printf("Valid channel ID: [%d ~ %d]\n\n", CHAN_FIRST, CHAN_LAST);
	return EVA_FALSE;
}

evaBool_e 
isValidStrmId(const char* inStr, chanId_e *pCh, strmId_e *pSt) {
	char *param[MAX_TOKEN];
	int sz = 0;

	sz = getTokens(inStr, ":", param);
	if (2 == sz && isValidNum(param[1], STRM_FIRST, STRM_LAST) && \
		isValidNum(param[0], CHAN_FIRST, CHAN_LAST)) {
		*pCh = atoi(param[0]);
		*pSt = atoi(param[1]);
		return EVA_TRUE;
	}

	printf("Valid stream ID, syntax: st=[chan_id:strm_id]\n");
	printf("Channel ID:[%d ~ %d]\n", CHAN_FIRST, CHAN_LAST);
	printf("Stream ID: [%d ~ %d]\n\n", STRM_FIRST, STRM_LAST);
	return EVA_FALSE;
}

evaBool_e 
isValidInfcId(const char* inStr) {
	if (isValidNum(inStr, INFC_FIRST, INFC_LAST)) {
		return EVA_TRUE;
	}

	printf("Valid interface ID: [%d ~ %d]\n\n", INFC_FIRST, INFC_LAST);
	return EVA_FALSE;
}

evaBool_e 
isValidToneid(const char* inStr) {
	if (isValidNum( inStr, TONECFG_FIRST, TONECFG_LAST)) {
		return EVA_TRUE;
	}

	printf("Valid tone ID: [%d ~ %d]\n", TONECFG_FIRST, TONECFG_LAST);
	printf("Note: tone[0] is reserved for silence and cannot be configured.\n\n");
	return EVA_FALSE;
}

evaBool_e 
isValidCptType(const char* inStr) {
	if (convertCptType(inStr) < 0) {
		printf("Valid Cpt type : <dial | ringback | busy | reorder | sit | c1 | c2 | c3 | c4>\n\n");
		return EVA_FALSE;
	}

	return EVA_TRUE;
}

evaBool_e 
isValidToneType(const char* inStr) {
	if (convertToneType(inStr) < 0) {
		printf("Valid tone type: reg | mod, Note: reg=regular, mod=modulated \n\n");
		return EVA_FALSE;
	}

	return EVA_TRUE;
}

evaBool_e 
isValidGain(const char* inStr) {
	if (isValidNum(inStr, -20, 20)) {
		return EVA_TRUE;
	}

	printf("Valid gain range: [-20 ~ 20] (db) \n\n");
	return EVA_FALSE;
}

evaBool_e 
isValidAccDev(const char* inStr) {
	if (isValidNum(inStr, 15, 35)) {
		return EVA_TRUE;
	}

	printf("Valid Accept Deviation range: [15 ~ 35] (%) \n\n");
	return EVA_FALSE;
}

evaBool_e 
isValidRejDev(const char* inStr, int accDev) {
	if (isValidNum(inStr, (accDev+10), 45) ) {
		return EVA_TRUE;
	}

	printf("Valid Reject Deviation range: [15 ~ 45] (%) AND should at least 10% larger than accept deviation \n\n");
	return EVA_FALSE;
}

evaBool_e 
isValidPower(const char* inStr, int power) {
	if (isValidNum(inStr, DTMF_POWER_MIN, DTMF_POWER_MAX) ) {
		return EVA_TRUE;
	}

	printf("Valid Power range: [-25 ~ -45] (dBm) \n\n");
	return EVA_FALSE;
}

evaBool_e 
isValidDuration(const char* inStr, int duration) {
	if (isValidNum(inStr, DTMF_DURATION_MIN, DTMF_DURATION_MAX) ) {
		return EVA_TRUE;
	}

	printf("Valid Duration range: [5 ~ 16] (5ms)  \n\n");
	return EVA_FALSE;
}





evaBool_e 
isEnabled(const char* inStr, const char* pAttr) {
	char *key;

	if ((key = strstr(inStr, pAttr))) {
		if ('+' == key[strlen(pAttr)]) {
			return EVA_TRUE;
		}
		else if ('-' == key[strlen(pAttr)]) {
			return EVA_FALSE;
		}
	}

	return (-1);
}

evaBool_e 
isValidJbMode(const char* inStr) {
	if (convertJbMode(inStr) >= 0) { 
		return EVA_TRUE;
	}

	printf("Valid JB mode: a | f, Note: a=adaptive, f=fixed\n\n");
	return EVA_FALSE;
}

evaBool_e 
isValidJbSize(const char* inStr) {
	if (isValidNum(inStr, JB_SZ_MIN, JB_SZ_MAX)) {
		return EVA_TRUE;
	}

	printf("Valid JB size: [0 ~ 800] (ms) \n\n");
	return EVA_FALSE;
}

evaBool_e 
isValidCidFormat(const char* inStr) {
	if (convertCidFormat(inStr) >= 0) {
		return EVA_TRUE;
	}
	printf("Valid Cid format: us-BELLCORE_FSK, eu-ETSI_FSK, jp-NT, dtmf-DTMF-cid \n\n");
	return EVA_FALSE;
}

evaBool_e 
isValidDtmfDigit(char inStr) {
	if( ('a' <= (inStr) && (inStr) <= 'd') ){
		inStr = inStr + ('A' - 'a');
	}
	if( (inStr >= '0' && inStr <= '9') || (inStr=='*') ||(inStr=='#')|| ('A' <= (inStr) && (inStr) <= 'D') ){
		return EVA_TRUE;
	}

	printf("Valid DTMF digit: [0 ~ 9], *, #, [A(a) ~ D(d)] \n\n");
	return EVA_FALSE;
}

evaBool_e 
isValidLecTailLen(const char* inStr) {
	if (!strcasecmp(inStr, "36")
		|| !strcasecmp(inStr, "48")
		|| !strcasecmp(inStr, "60")
		|| !strcasecmp(inStr, "72")
		){
		return EVA_TRUE;
	}
	
	return EVA_FALSE;
}

evaBool_e 
isValidDspdir(const char* inStr) {
	if (convertDspdir(inStr) < DSP_INVALID) {
		return EVA_TRUE;
	}
	
	printf("Incorrect dsp direction! dir: tx | rx\n\n");
	return EVA_FALSE;
}

evaBool_e 
isValidDspStageNum(const char* inStr, int min, int max) {
	int stage=atoi(inStr);
	char tmp[10];
	sprintf(tmp, "%d", stage);
	
	if (!strcasecmp(inStr, tmp)){//instr is real digit
		if (((stage >= min) && (stage <= max)) || ((min == MAX32) && (max == MAX32))) { 
		/* min == max == MAX32, means no limit on the range of value */
			if( stage%2 ==0)
				return EVA_TRUE;
		}
	}
	return EVA_FALSE;
}

void 
adjustBackSpace(char* inStr) {
	char tmp[MAX_CMD_LEN]= {0};
	int i,j;
	
	for (i = 0, j = 0; i <= strlen(inStr); i++ )
	{
		if (inStr[i] != '\b') {
			tmp[j++] = inStr[i];
		}else{
			if (j > 0) {
				j--;
			}
		}
	}
	strcpy(inStr, tmp);
}

void 
replaceKeyToBlank(char* inStr, char key) {
	int i;
	for (i = 0; i <= strlen(inStr); i++ )	{
		if (inStr[i] == key) {
			inStr[i] = ' ';
		}
	}
}

/* added by mtk07059_20150811. */
codec_e 
convertBitrate(const char* inStr) {
	if (!strcasecmp(inStr, "5300")) {
		return EVA_G7231_5300;
	}
	else if (!strcasecmp(inStr, "6300")) {
		return EVA_G7231_6300;
	}
	return EVA_BR_NONE;
}

codec_e 
convertCodec(const char* inStr) {
	if (!strcasecmp(inStr, "g711a")) {
		return CODEC_G711A;
	}
	else if (!strcasecmp(inStr, "g711u")) {
		return CODEC_G711U;
	}
	else if (!strcasecmp(inStr, "g729")) {
		return CODEC_G729;
	}
#ifdef AMR_CODEC
	else if (!strcasecmp(inStr, "amr")) { //isabella_130830_AMR
		return CODEC_AMR;
	}
#endif
	else if (!strcasecmp(inStr, "g726")) {
		return CODEC_G726_32;
	}
	else if (!strcasecmp(inStr, "g722")) {
		return CODEC_G722;
	}
	else if (!strcasecmp(inStr, "g7231")) { //isabella_g7231
		return CODEC_G723;
	}
	else if (!strcasecmp(inStr, "t38")) {
		return CODEC_T38;
	}
#ifdef V152_SUPPORT
	/*mtk08176 for vbd*/
	else if (!strcasecmp(inStr, "vbd-pcma")) {
		return CODEC_PCMA_VBD;
	}
	/*mtk08176 for vbd*/
	else if (!strcasecmp(inStr, "vbd-pcmu")) {
		return CODEC_PCMU_VBD;
	}
#endif	
	return CODEC_INVALID;
}

pTime_e 
convertPtime(const char* inStr) {
	if (!strcasecmp(inStr, "10")) {
		return PTIME_10MS;
	}
	else if (!strcasecmp(inStr, "20")) {
		return PTIME_20MS;
	}
	else if (!strcasecmp(inStr, "30")) {
		return PTIME_30MS;
	}
	else if (!strcasecmp(inStr, "40")) {
		return PTIME_40MS;
	}
	else if (!strcasecmp(inStr, "50")) {
		return PTIME_50MS;
	}
	else if (!strcasecmp(inStr, "60")) {
		return PTIME_60MS;
	}

	return (-1);
}


strmDir_e 
convertStrmdir(const char* inStr) {
	if (!strcasecmp(inStr, "sr")) {
		return STRM_SENDRECV;
	}
	else if (!strcasecmp(inStr, "so")) {
		return STRM_SENDONLY;
	}
	else if (!strcasecmp(inStr, "ro")) {
		return STRM_RECVONLY;
	}
	else if (!strcasecmp(inStr, "in")) {
		return STRM_INACTIVE;
	}

	return (-1);
}
dsp_dir
convertDspdir(const char* inStr) {
	if (!strcasecmp(inStr, "tx")) {
		return DSP_TX;
	}
	else if (!strcasecmp(inStr, "rx")) {
		return DSP_RX;
	}
	

	return (DSP_INVALID);
}


enableControl_e
convertControl(const char* inStr) {
	if (!strcasecmp(inStr, "on")) {
		return CONTROL_ENABLE;
	}
	else if (!strcasecmp(inStr, "off")) {
		return CONTROL_DISABLE;
	}

	return (-1);
}

lecTailMode_e 
convertLecTailLen(const char* inStr) {
	if (!strcasecmp(inStr, "36")) {
		return EC_TAIL_LENGTH_36MS;
	}else if (!strcasecmp(inStr, "48")) {
		return EC_TAIL_LENGTH_48MS;
	}else if (!strcasecmp(inStr, "60")) {
		return EC_TAIL_LENGTH_60MS;
	}else if (!strcasecmp(inStr, "72")) {
		return EC_TAIL_LENGTH_72MS;
	}
	
	return (-1);
}

lineState_e
convertLineState(const char* inStr) {
	if (!strcasecmp(inStr, "down")) {
		return LINE_DOWN;
	}
	else if (!strcasecmp(inStr, "fwd")) {
		return LINE_ACTIVE_FWD;
	}
	else if (!strcasecmp(inStr, "rev")) {
		return LINE_ACTIVE_REV;
	}
	else if (!strcasecmp(inStr, "ring")) {
		return LINE_RING;
	}
	else if (!strcasecmp(inStr, "sleep")) {
		return LINE_SLEEP;
	}
	else if (!strcasecmp(inStr, "revoht")) {
		return LINE_OHT_REV;
	}
	return LINE_ERROR;
}

hookState_e
convertHookState(const char* inStr) {
	if (!strcasecmp(inStr, "on")) {
		return HOOK_RELEASE;
	}
	else if (!strcasecmp(inStr, "off")) {
		return HOOK_SEIZE;
	}
	else if (!strcasecmp(inStr, "flash")) {
		return HOOK_FLASH;
	}
	else if (!strcasecmp(inStr, "pulse1")) {
		return HOOK_PULSE1;
	}
	else if (!strcasecmp(inStr, "pulse2")) {
		return HOOK_PULSE2;
	}
	else if (!strcasecmp(inStr, "pulse3")) {
		return HOOK_PULSE3;
	}
	else if (!strcasecmp(inStr, "pulse4")) {
		return HOOK_PULSE4;
	}
	else if (!strcasecmp(inStr, "pulse5")) {
		return HOOK_PULSE5;
	}
	else if (!strcasecmp(inStr, "pulse6")) {
		return HOOK_PULSE6;
	}
	else if (!strcasecmp(inStr, "pulse7")) {
		return HOOK_PULSE7;
	}
	else if (!strcasecmp(inStr, "pulse8")) {
		return HOOK_PULSE8;
	}
	else if (!strcasecmp(inStr, "pulse9")) {
		return HOOK_PULSE9;
	}
	else if (!strcasecmp(inStr, "pulse10")) {
		return HOOK_PULSE10;
	}
	else if (!strcasecmp(inStr, "pulse11")) {
		return HOOK_PULSE11;
	}
	else if (!strcasecmp(inStr, "pulse12")) {
		return HOOK_PULSE12;
	}
	else if (!strcasecmp(inStr, "pulse13")) {
		return HOOK_PULSE13;
	}
	else if (!strcasecmp(inStr, "pulse14")) {
		return HOOK_PULSE14;
	}
	else if (!strcasecmp(inStr, "pulse15")) {
		return HOOK_PULSE15;
	}
	else if (!strcasecmp(inStr, "pulse16")) {
		return HOOK_PULSE16;
	}
	else if (!strcasecmp(inStr, "pulse17")) {
		return HOOK_PULSE17;
	}
	else if (!strcasecmp(inStr, "pulse18")) {
		return HOOK_PULSE18;
	}
	else if (!strcasecmp(inStr, "pulse19")) {
		return HOOK_PULSE19;
	}
	else if (!strcasecmp(inStr, "pulse20")) {
		return HOOK_PULSE20;
	}
	
	
	return (-1);
}

dtmf_e 
convertDtmf(const char* inStr) {
	if (!strcasecmp(inStr, "0")) {
		return DTMF_0;
	}
	else if (!strcasecmp(inStr, "1")) {
		return DTMF_1;
	}
	else if (!strcasecmp(inStr, "2")) {
		return DTMF_2;
	}
	else if (!strcasecmp(inStr, "3")) {
		return DTMF_3;
	}
	else if (!strcasecmp(inStr, "4")) {
		return DTMF_4;
	}
	else if (!strcasecmp(inStr, "5")) {
		return DTMF_5;
	}
	else if (!strcasecmp(inStr, "6")) {
		return DTMF_6;
	}
	else if (!strcasecmp(inStr, "7")) {
		return DTMF_7;
	}
	else if (!strcasecmp(inStr, "8")) {
		return DTMF_8;
	}
	else if (!strcasecmp(inStr, "9")) {
		return DTMF_9;
	}
	else if (!strcasecmp(inStr, "*")) {
		return DTMF_STAR;
	}
	else if (!strcasecmp(inStr, "#")) {
		return DTMF_POUND;
	}
	else if (!strcasecmp(inStr, "a")) {
		return DTMF_A;
	}
	else if (!strcasecmp(inStr, "b")) {
		return DTMF_B;
	}
	else if (!strcasecmp(inStr, "c")) {
		return DTMF_C;
	}
	else if (!strcasecmp(inStr, "d")) {
		return DTMF_D;
	}
	
	return (-1);
}	

int
convertToneType(const char* inStr) {
	if (!strcasecmp(inStr, "reg")) {
		return TONE_REGULAR;
	}
	else if (!strcasecmp(inStr, "mod")) {
		return TONE_MODULATE;
	}

	return (-1);
}

int
convertCptType(const char* inStr) {
	if (!strcasecmp(inStr, "dial")) {
		return TONE_DIAL;
	}
	else if (!strcasecmp(inStr, "ringback")) {
		return TONE_RINGBACK;
	}
	else if (!strcasecmp(inStr, "busy")) {
		return TONE_BUSY;
	}
	else if (!strcasecmp(inStr, "reorder")) {
		return TONE_REORDER;
	}
	else if (!strcasecmp(inStr, "sit")) {
		return TONE_SIT;
	}
	else if (!strcasecmp(inStr, "c1")) {
		return TONE_CUSTOM_1;
	}
	else if (!strcasecmp(inStr, "c2")) {
		return TONE_CUSTOM_2;
	}
	else if (!strcasecmp(inStr, "c3")) {
		return TONE_CUSTOM_3;
	}
	else if (!strcasecmp(inStr, "c4")) {
		return TONE_CUSTOM_4;
	}
	
	return (-1);
}


int
convertJbMode(const char* inStr) {
	if (!strcasecmp(inStr, "a")) {
		return JB_ADAPT;
	}
	else if (!strcasecmp(inStr, "f")) {
		return JB_FIXED;
	}

	return (-1);
}

int
convertCidFormat(const char* inStr) {
	if (!strcasecmp(inStr, "us")) {
		return CID_FORMAT_BELLCORE_FSK;
	}
	else if (!strcasecmp(inStr, "eu")) {
		return CID_FORMAT_ETSI_FSK;
	}
	else if (!strcasecmp(inStr, "jp")) {
		return CID_FORMAT_NTT;
	}
	else if (!strcasecmp(inStr, "rpas")) {
		return CID_FORMAT_ETSI_RPAS;
	}
	else if (!strcasecmp(inStr, "dtas")) {
		return CID_FORMAT_ETSI_DTAS;
	}	
	else if (!strcasecmp(inStr, "dtmf")) {
		return CID_FORMAT_DTMF;
	}	
	return (-1);
}

/*ADD by mtk08176_20150508 , support line test. */
/*Modified by mtk07059_20160302 , support line test. */
slicType_t  SlicType[]=
{
	{TYPE_ZARLINK, "zl88601"},
	{TYPE_ZARLINK, "le89156"},
	{TYPE_ZARLINK, "le89116"},
	{TYPE_ZARLINK, "le89316"},
	{TYPE_ZARLINK, "le9641"},
	{TYPE_ZARLINK, "le9642"},
	{TYPE_SILICON, "si32176"},
	{TYPE_SILICON, "si32260"},
	{TYPE_SILICON, "si3218x"},
	{TYPE_SILICON, "si3228x"},
	{-1, NULL},
};

int
convertSlicType(const char* inStr) {
	int  i=0;
	for(i=0; SlicType[i].slic_name != NULL; i++){
		if( !strcasecmp(inStr,SlicType[i].slic_name)){
			break;	
		}
	}
	return SlicType[i].slicType;	
}

MWIValue_e convertMWI(const char * inStr){
    int i = 0;
    for ( ; i < MTK_EVA_MWI_VALUE_MAX; ++ i)
    {
        if( strcasecmp(inStr, gValidMWIStrVal[i]) == 0 ) {
            return i;
        }
    }

    return MTK_EVA_MWI_VALUE_NONE;
}

char* etosActive(int32 e) {
	switch (e) {
	case STATE_INACTIVE:
		return "Inactive";
	case STATE_ACTIVE:
	default:
		return "Active";
	}
}

char* etosBool(int32 e) {
	switch (e) {
	case EVA_FALSE:
		return "False";
	case EVA_TRUE:
	default:
		return "True";
	}
}

char* etosExec(int32 e) {
	switch (e) {
	case EXEC_SUCCESS:
		return "Execution success!";
	default:
		return "Execution failed! ";
	}
}
	
char* etosControl(int32 e) {
	switch (e) {
	case CONTROL_DISABLE:
		return "Disable";
	case CONTROL_ENABLE:
	default:
		return "Enable";
	}
}

char* etosInfctype(int32 e) {
	switch (e) {
	case INFC_FXS:
		return "FXS";
	case INFC_FXO:
		return "FXO";
	case INFC_AUDIO:
		return "Audio";
	case INFC_OTHER:
		return "Other";
	default:
		return "Unknown type";
	}
}

char* etosTonetype(int32 e) {
	switch (e) {
	case TONE_REGULAR:
		return "Regular tone";
	case TONE_MODULATE:
		return "Modulated tone";
	default:
		return "Invalid type";
	}
}

char* etosIpver(int32 e) {
	switch (e) {
	case IPV4:
		return "ipv4";
	case IPV6:
		return "ipv6";
	default:
		return "Invalid IP Version";
	}
}

char* etosStrmdir(int32 e) {
	switch (e) {
	case STRM_INACTIVE:
		return "Inactive";
	case STRM_RECVONLY:
		return "Receive only";
	case STRM_SENDONLY:
		return "Send only";
	case STRM_SENDRECV:
		return "Send & Receive";
	default:
		return "Invalid direction";
	}
}

char* etosDtmf(int32 e) {
	switch (e) {
	case DTMF_1:
		return "DTMF_1";
	case DTMF_2:
		return "DTMF_2";
	case DTMF_3:
		return "DTMF_3";
	case DTMF_4:
		return "DTMF_4";
	case DTMF_5:
		return "DTMF_5";
	case DTMF_6:
		return "DTMF_6";
	case DTMF_7:
		return "DTMF_7";
	case DTMF_8:
		return "DTMF_8";
	case DTMF_9:
		return "DTMF_9";
	case DTMF_0:
		return "DTMF_0";
	case DTMF_STAR:
		return "DTMF_*";
	case DTMF_POUND:
		return "DTMF_#";
	case DTMF_A:
		return "DTMF_A";
	case DTMF_B:
		return "DTMF_B";
	case DTMF_C:
		return "DTMF_C";
	case DTMF_D:
		return "DTMF_D";
	default:
		return "INVALID_DTMF";
	}
}

char etocDtmf(int32 dtmf){
	char ch='\0';
	
	if((dtmf >= TONE_DTMF_1) && (dtmf <= TONE_DTMF_9)) {
		ch='1' + dtmf-1;//,d1....d9
	} else if(dtmf ==TONE_DTMF_0) {
		ch= '0';//*:ds
	} else if(dtmf == TONE_DTMF_STAR) {
		ch='*';//#:do
	} else if(dtmf == TONE_DTMF_POUND){
		ch='#';
	}else if(dtmf >=TONE_DTMF_A && dtmf<=TONE_DTMF_D){
		ch='a' + (dtmf - 13);//a~d
	}

	return ch;

}
char* etosTone(int32 e) {
	switch (e) {
	case TONE_DTMF_1:
		return "DTMF_1";
	case TONE_DTMF_2:
		return "DTMF_2";
	case TONE_DTMF_3:
		return "DTMF_3";
	case TONE_DTMF_4:
		return "DTMF_4";
	case TONE_DTMF_5:
		return "DTMF_5";
	case TONE_DTMF_6:
		return "DTMF_6";
	case TONE_DTMF_7:
		return "DTMF_7";
	case TONE_DTMF_8:
		return "DTMF_8";
	case TONE_DTMF_9:
		return "DTMF_9";
	case TONE_DTMF_0:
		return "DTMF_0";
	case TONE_DTMF_STAR:
		return "DTMF_*";
	case TONE_DTMF_POUND:
		return "DTMF_#";
	case TONE_DTMF_A:
		return "DTMF_A";
	case TONE_DTMF_B:
		return "DTMF_B";
	case TONE_DTMF_C:
		return "DTMF_C";
	case TONE_DTMF_D:
		return "DTMF_D";
	case TONE_DIAL:
		return "DIAL";
	case TONE_RINGBACK:
		return "RINGBACK";
	case TONE_BUSY:
		return "BUSY";
	case TONE_REORDER:
		return "REORDER";
	case TONE_SIT:
		return "SIT";
	case TONE_CUSTOM_1:
		return "TONE_CUSTOM_1";
	case TONE_CUSTOM_2:
		return "TONE_CUSTOM_2";
	case TONE_CUSTOM_3:
		return "TONE_CUSTOM_3";
	case TONE_CUSTOM_4:
		return "TONE_CUSTOM_4";	
	case TONE_CNG:
		return "CNG";
	case TONE_CED:
		return "CED";
	case TONE_ANS:
		return "ANS";
	case TONE_ANSAM:
		return "ANSAM";
	case TONE_V21PREAMBLE:
		return "V21PREAMBLE";	
	case TONE_FAX_TIMEOUT:
		return "FAX_TIMEOUT";		
	default:
		return "Invalid Tone";
	}
}

char* etosToneDir(int32 e) {
	switch (e) {
	case TONE_DIR_LOCAL:
		return "from local";
	case TONE_DIR_REMOTE:
		return "from network";
	default:
		return "Invalid Tone direction";
	}
}

char* etosEvent(int32 e) {
	switch (e) {
	case EVENT_CODE_CID:
		return "CID";
	case EVENT_CODE_ERROR:
		return "Error";
	case EVENT_CODE_HOOK:
		return "Hook";
	case EVENT_CODE_JB_UPDATE:
		return "JB";
	case EVENT_CODE_LINE:
		return "Line";
	case EVENT_CODE_NON_RTP_RECVD:
		return "Non-RTP";
	case EVENT_CODE_PERFORMANCE:
		return "Benchmark";
	case EVENT_CODE_RTCP_RECVD:
		return "RTCP Received";
	case EVENT_CODE_RTCP_SEND:
		return "RTCP Sent";
	case EVENT_CODE_STREAM_UPDATE:
		return "Stream Update";
	case EVENT_CODE_TIMER:
		return "Timer";
	case EVENT_CODE_TONE:
		return "Tone";
	case EVENT_CODE_T38:
		return "T38";	
	case EVENT_CODE_INVALID:
	default:
		return "!!! Error: Invalid event !!!";
	}
}

char* etosEdge(int32 e) {
	switch (e) {
	case EDGE_BEGIN:
		return "++ Event Begin ++";
	case EDGE_END:
		return "--  Event End  --";
	case EDGE_ONCE:
		return "== Event  Once ==";
	default:
		return "Invalid edge";
	}
}

char* etosCodec(int32 e) {
	switch (e) {
	case CODEC_DTMFR:
		return "DTMF_Relay";
#if defined(TCSUPPORT_C1_MS) ||defined(CJ_DSP_SUPPORT) 
	case CODEC_DTMFR_DL:
		return "DTMF_Relay";
#endif
	case CODEC_G711A:
		return "G.711a";
	case CODEC_G711U:
		return "G.711u";
	case CODEC_G722:
		return "G.722";
	case CODEC_G723:
		return "G.723";
	case CODEC_G726_32:
		return "G.726";
	case CODEC_G729:
		return "G.729ab";
#ifdef AMR_CODEC
	case CODEC_AMR: //isabella_130830_AMR
		return "AMR";
#endif
	case CODEC_INVALID:
		return "!!! Error: Invalid codec !!!";
	case CODEC_SILCOMP:
		return "Silence-compression";
	case CODEC_T38:
		return "T.38";
#ifdef V152_SUPPORT
	case CODEC_PCMU_VBD:  /*mtk08176 for vbd*/
		return "PCMU_VBD";
	case CODEC_PCMA_VBD: /*mtk08176 for vbd*/
		return "PCMA_VBD";
#endif
	default:
		return "Invalid codec";
	}
}

/*  added by mtk07059_20150811. */
char* etosBitrate(int32 e) {
	switch (e) {
	case EVA_G7231_5300:
		return "5300";
	case EVA_G7231_6300:
		return "6300";
	case EVA_ILBC_20:
		return "20";
	case EVA_ILBC_30:
		return "30";
	case EVA_AMR_475:
		return "475";
	case EVA_AMR_515:
		return "515";
	case EVA_AMR_59:
		return "59";
	case EVA_AMR_67:
		return "67";
	case EVA_AMR_74:
		return "74";
	case EVA_AMR_795:
		return "795";
	case EVA_AMR_102:
		return "102";
	case EVA_AMR_122:
		return "122";
	case EVA_AMR_SID:
		return "sid";
	case EVA_AMR_NONE:
		return "none";
	default:
		return "Invalid biterate";
	}
}

char* etosPtime(int32 e) {
	switch (e) {
	case PTIME_10MS:
		return "10";
	case PTIME_20MS:
		return "20";
	case PTIME_30MS:
		return "30";
	case PTIME_40MS:
		return "40";
	case PTIME_50MS:
		return "50";
	case PTIME_60MS:
		return "60";
	default:
		return "Invalid ptime";
	}
}

char* etosHook(int32 e) {
	switch (e) {
	case HOOK_FLASH:
		return "HOOK-FLASH";
	case HOOK_RELEASE:
		return "ON-HOOK";
	case HOOK_SEIZE:
		return "OFF-HOOK";
	case HOOK_PULSE1:
		return "PULSE1";
	case HOOK_PULSE2:
		return "PULSE2";
	case HOOK_PULSE3:
		return "PULSE3";
	case HOOK_PULSE4:
		return "PULSE4";
	case HOOK_PULSE5:
		return "PULSE5";
	case HOOK_PULSE6:
		return "PULSE6";
	case HOOK_PULSE7:
		return "PULSE7";
	case HOOK_PULSE8:
		return "PULSE8";
	case HOOK_PULSE9:
		return "PULSE9";
	case HOOK_PULSE10:
		return "PULSE10";
	case HOOK_PULSE11:
		return "PULSE11";
	case HOOK_PULSE12:
		return "PULSE12";
	case HOOK_PULSE13:
		return "PULSE13";
	case HOOK_PULSE14:
		return "PULSE14";
	case HOOK_PULSE15:
		return "PULSE15";
	case HOOK_PULSE16:
		return "PULSE16";	
	case HOOK_PULSE17:
		return "PULSE17";
	case HOOK_PULSE18:
		return "PULSE18";
	case HOOK_PULSE19:
		return "PULSE19";
	case HOOK_PULSE20:
		return "PULSE20";	
	default:
		return "Invalid hook state";
	}
}

char* etosLine(int32 e) {
	switch (e) {
	case LINE_ACTIVE_FWD:
		return "Line-Active-Forward";
	case LINE_ACTIVE_REV:
		return "Line-Active-Reversed";
	case LINE_BUSY:
		return "Line-Busy";
	case LINE_DOWN:
		return "Line-Power-Down";
	case LINE_ERROR:
		return "Line-Error";
	case LINE_RING:
		return "Line-Ringing";
	case LINE_RING_PAUSE:
		return "Line-Ring-Pause";
	case LINE_SLEEP:
		return "Line-Low-Power-Idle";
	case LINE_LIU:
		return "Line-Evid-Liu";
	case LINE_LNIU:
		return "Line-Evid-Lniu";		
	case LINE_OHT_REV: //MTK69036 REV OHT
		return "Line-OHT-Reversed";
	default:
		return "Invalid line state";
	}
}

char* etosJbMode(int32 e) {
	switch (e) {
	case JB_ADAPT:
		return "Adaptive";
	case JB_FIXED:
		return "Fixed";
	default:
		return "Invalid JB mode";
	}
}

char* etosT38State(int32 e) {
	switch (e) {
	case T38_DISCONN:
		return "T38 Disconnected";
	default:
		return "Invalid T38 state";
	}
}

char* etosCidFormat(int32 e) {
	switch (e) {
	case CID_FORMAT_BELLCORE_FSK:
		return "BELLCORE_FSK";
	case CID_FORMAT_ETSI_FSK:
		return "ETSI_FSK";
	case CID_FORMAT_NTT:
		return "NTT";	
	case CID_FORMAT_ETSI_RPAS:
		return "RPAS";
	case CID_FORMAT_ETSI_DTAS:
		return "DTAS";	
	case CID_FORMAT_DTMF:
		return "DTMF";	
	default:
		return "Invalid Cid format";
	}
}

/* ADD by mtk08176_20150824, China Unicom inside and outside line test */
char* etosCUOutlineStatus(int32 e) {
	switch (e) {
	case OutLine_Normal:
		return "OutLine_Normal";
	case OutLine_PhoneDisconnet:
		return "OutLine_PhoneDisconnet";
	case OutLine_PhoneoffHook:
		return "OutLine_PhoneoffHook";	
	case OutLine_PowerLineContacted:
		return "OutLine_PowerLineContacted";
	case OutLine_BothLineContacted:
		return "OutLine_BothLineContacted";	
	case OutLine_ALineMixOther:
		return "OutLine_ALineMixOther";
	case OutLine_BLineMixOther:
		return "OutLine_BLineMixOther";
	case OutLine_BothLineGrounding:
		return "OutLine_BothLineGrounding";
	case OutLine_ALineGrounding:
		return "OutLine_ALineGrounding";	
	case OutLine_BLineGrounding:
		return "OutLine_BLineGrounding";
	case OutLine_ABLinePoorInsulation:
		return "OutLine_ABLinePoorInsulation";	
	case OutLine_ShortCircuit:
		return "OutLine_ShortCircuit";
	case OutLine_BothLineLeakageToGround:
		return "OutLine_BothLineLeakageToGround";
	case OutLine_ALineLeakageToGround:
		return "OutLine_ALineLeakageToGround";	
	case OutLine_BLineLeakageToGround:
		return "OutLine_BLineLeakageToGround";
	default:
		return "Invalid CUOutlineStatus";
	}
}

char* etosCUInlineStatus(int32 e) {
	switch (e) {
	case Inline_Test_Item_Normal:
		return "Inline_Test_Item_Normal";
	case Inline_Test_Item_UNNormal:
		return "Inline_Test_Item_UNNormal";	
	default:
		return "Invalid Cid format";
	}
}


char* ipToStr(uint32 ip) {

	static char ret[16];

	sprintf(ret, "%ld.%ld.%ld.%ld", (ip>>24)&(0xff), (ip>>16)&(0xff), (ip>>8)&(0xff), ip&(0xff));

	return ret;
}

command_e getCmdCode(const command_t *cmdSet, const char *str) {
	command_e cmd;
	
	for (cmd = 0; cmd < NUM_OF_COMMAND ; cmd++) {
		if (!strcasecmp(cmdSet[cmd].name, str) || !strcasecmp(cmdSet[cmd].opcode, str)) {
			return cmd;
		}
	}
	
	return CMD_INVALID;
}

int getTokens(const char *inStr, const char* delim, char *pArgv[]) {
	int numOfToken = 0;
	static char tmp[MAX_CMD_LEN] = {0};
	char *t;

	strcpy(tmp, inStr);
	t = strtok(tmp, delim);
	
	while(t && numOfToken < MAX_TOKEN) 	{
		pArgv[numOfToken++] = t;
		t = strtok(NULL, delim);
	}

	return numOfToken;
}

void getAttrVal(const char *inStr, const char *pAttr, const char *delim, char *pVal) {
	char tmp[MAX_CMD_LEN] = {0};
	char *t;

	strcpy(tmp, inStr);
    *pVal = '\0';
	t = (char*)strtok(tmp, delim);

	while (t) {
		if (strlen(t) > strlen(pAttr) && ' ' == t[strlen(t) - strlen(pAttr) - 1]
			&& !strcasecmp(pAttr,  t + strlen(t) - strlen(pAttr)) ){
			t = (char*)strtok((void*)0, delim);

			if (t) {
				strcpy(pVal, t);
			}
			break;
		}

		t = (char*)strtok((void*)0, delim);
	}
	
	t = (char*)strtok(pVal, " ");
}

