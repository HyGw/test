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
 *	File:	evcom.c
 *	 
 *	Author: PT Chen, Quark Li
 *	 		
 *	Last Revision Date: 2011.9.29
 *	 
 *	Description:
 *	EVA Console Manager CLI tool. A test application to control DSP
 *	functionalities through ADAM API and demostration of EVA architecture. 
 *
 */

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "evcom.h"
#include "evcom_global.h"
#include "evcom_helper.h"

#include "eva_constant.h"
#include "eva_struct.h"
#include "adam.h"
#include <signal.h>

#ifdef DSPID_VIKING
#define DSP_ID DSP_VIKING
#elif DSPID_MTK
#define DSP_ID DSP_MTK
#define EVENT_NOTIFY
#endif

#define DTMF_DEVIATION_BASELINE (15) 
#define DTMF_DURATION_INTERVAL (5) //5ms as a step

#define CONSOLE_PROMPT 	printf("\nEVCom >> ");
#define CHK_DSP_STATE	if (0 == gDspState) { \
							printf("DSP was not initialized, please run DspInvoke first!\n\n"); \
							return; \
						} \
						else if (1 < gDspState) { \
							printf("DSP has been revoked, please exit evcom, "); \
							printf("remove all DSP modules, and start again!\n\n"); \
							return; \
						}

evaBool_e gRunTask = EVA_TRUE;
tone_t gTones[TONECFG_LAST + 1];
cpt_t gCpts[TONE_CUSTOM_4 - TONE_DIAL + 1]; /* only DIAL to C4 will be used */
static int gDspState = 0;
evaBool_e printJB = CONTROL_DISABLE;

const char * const gValidMWIStrVal[MTK_EVA_MWI_VALUE_MAX] = {"none", "set","clear"};

static void _promptCmd(void);
static void _promptCmdHelp(command_e cmdid);
#ifdef EVENT_NOTIFY
static void _threadEventPolling(void *argv);
#else
static void _threadEventPolling(void);
#endif


void cmdDspInvoke(const char *inStr) {
	activeState_e active;
	dspFeature_t feature;
#ifdef EVENT_NOTIFY
	event_t mEvent;
	int ret = 0;
#else
	pthread_t thread;
#endif

	if (gDspState > 1) {
		printf("DSP has been revoked, please exit evcom, remove and re-insert DSP modules, and start again!\n\n");
		return;
	}
		
	if (EXEC_SUCCESS != adamDspQuery(DSP_ID, &active, &feature)) {
		printf("Error: Cannot get DSP status, application will quit now!\n\n");
		exit(-1);
	}
	if (active) {
		printf("DSP already initialized!(%s)\n", etosActive(active));
		return;
	}
	
	printf("Invoking DSP ...\n\n");

	if (EXEC_SUCCESS != adamDspInvoke(DSP_ID)) {
		printf("Error: DSP initialization failed!\n");
	}

	gDspState++;
	
#ifdef EVENT_NOTIFY
	mEvent.eventNotify = _threadEventPolling;
	if (EXEC_SUCCESS != adamPollEvent(BLOCKING, &mEvent)) {
		printf ("Critical error: Cannot create thread!\n");
		exit (1);
	}
#else
	if (pthread_create(&thread, NULL, (void *) _threadEventPolling, NULL) != 0) {
		printf ("Critical error: Cannot create thread!\n");
		exit (1);
	}
#endif

	if (EXEC_SUCCESS != adamDspQuery(DSP_ID, &active, &feature)) {
		printf("Error: Cannot get DSP status, application will quit now!\n\n");
		exit(-1);
	}
	if (active) {
		printf("DSP initialization completed! You can issue command to use DSP now.\n");
	}
	else {
		printf("DSP initialization failed! Please try again.\n");		
	}	
}

void cmdDspRevoke(const char *inStr) {
	char cmdLine[MAX_CMD_LEN] = {0};

	if (1 == gDspState) {
		printf("Once DSP is revoked, it cannot be invoked again until re-insert DSP modules, sure? (type 'yes' to revoke) ");

		fgets(cmdLine, MAX_CMD_LEN, stdin);
		cmdLine[strlen(cmdLine) - 1] = '\0';
		adjustBackSpace(cmdLine);

		if (!strcasecmp("yes", cmdLine)) {
			printf("Revoking DSP ...\n\n");
			if (EXEC_SUCCESS != adamDspRevoke(DSP_ID)) {
				printf("Error: DSP shutdown failed!\n");;
				return;
			}

			gDspState++;
			printf("DSP shutdown completed! You can quit evcom safely now.\n");
		}
		return;
	}

	printf("DSP already shutdown!\n");
	return;
}

void cmdDspQuery(const char *inStr) {
	activeState_e active;
	dspFeature_t feature;

	if (EXEC_SUCCESS != adamDspQuery(DSP_ID, &active, &feature)) {
		printf("Error: Cannot get DSP status, application will quit now!\n\n");
		exit(-1);
	}
	
	printf("DSP features: \n");
	printf("Active status: %s\n", etosActive(active));
	printf("DSP ID: (%d)\n", feature.dspId);	
	printf("Number of Channel: %d\n", feature.numOfChan);	
	printf("Stream per Channel: %d\n", feature.strmsPerChan);	
}

void cmdDspConfigTone(const char *inStr) {
	int toneId, toneType, i;
	char pVal[MAX_CMD_LEN] = {0};
	char *param[MAX_TOKEN];
	
	CHK_DSP_STATE

	getAttrVal(inStr, "tone", "=", pVal);
	if (isValidToneid(pVal)) {
		toneId = atoi(pVal);
		if (0 == toneId) {
			printf("Tone[0] is reserved as silence tone, please configure tone[1~9].\n\n");
			return;
		}

		getAttrVal(inStr, "type", "=", pVal);
		if (strlen(pVal) >0 && isValidToneType(pVal)) {
			 gTones[toneId].toneType = convertToneType(pVal);
		}
		toneType = gTones[toneId].toneType;
		
		printf("Tone (%d) configuration: \n", toneId);
		printf("Tone type: %s\n", etosTonetype(gTones[toneId].toneType));

		GetConfigFrequencyAndPower(toneId,toneType,inStr); 

        	GetConfigCadenceAttribute(toneId,inStr);

		if(EXEC_SUCCESS !=  adamDspConfigTone(DSP_ID, toneId, &gTones[toneId])){
			printf("Error: adamDspConfigTone failed!\n");
			return;
		}
		return;
	}

	_promptCmdHelp(CMD_DSP_CONFIG_TONE);
} 

void cmdDspConfigCpt(const char *inStr) {
	int i, cptIndex;
	char pVal[MAX_CMD_LEN] = {0};
	char *param[MAX_TOKEN];
	
	CHK_DSP_STATE

	getAttrVal(inStr, "cpt", "=", pVal);
	if (isValidCptType(pVal)) {
		cptIndex = convertCptType(pVal) - TONE_DIAL;
		gCpts[cptIndex].type =  convertCptType(pVal);

		printf("Cpt type (%s) configuration: \n", etosTone(gCpts[cptIndex].type));


		for (i = 0; i < DUAL_TONE_FREQ; i++) {
			if  (0 == i) {
		 		getAttrVal(inStr, "f0", "=", pVal);
			}
			else if  (1 == i) {
		 		getAttrVal(inStr, "f1", "=", pVal);
			}
			
			if (strlen(pVal) > 0) {
				if (2 == getTokens(pVal, ",", param)) {
					if (isValidNum(param[0], 0, 4000)) {
						gCpts[cptIndex].dual.toneFreq[i] = atoi(param[0]);
					}
					if (isValidNum(param[1], 0, 4000)) {
						gCpts[cptIndex].dual.toneDev[i] = atoi(param[1]);
					}
				}
			}
			printf("Freqency[%d] = %dHz, Deviation[%d] = %dHz \n",i ,gCpts[cptIndex].dual.toneFreq[i], i\
				, gCpts[cptIndex].dual.toneDev[i]);
		}
		
		getAttrVal(inStr, "pwr", "=", pVal);
		if (strlen(pVal) >0 && isValidNum(pVal, -40, 0)) {
			gCpts[cptIndex].dual.power = atoi(pVal) * 2;
		}
		printf("Power = %ddb \n", gCpts[cptIndex].dual.power/2);
		
		for (i = 0; i < MAX_CADENCE; i++) {
				if  (0 == i) {
		 			getAttrVal(inStr, "cad0", "=", pVal);
				}
				else if  (1 == i) {
		 			getAttrVal(inStr, "cad1", "=", pVal);
				}
				else if  (2 == i) {
		 			getAttrVal(inStr, "cad2", "=", pVal);
				}
					
					
				if (strlen(pVal) > 0) {
					if (4 == getTokens(pVal, ",", param)) {
						if (isValidNum(param[0], 0, MAX16)) {
							gCpts[cptIndex].dual.minMake[i]  = atoi(param[0]);
						}
						if (isValidNum(param[1], 0, MAX16)) {
							gCpts[cptIndex].dual.maxMake[i]  = atoi(param[1]);
						}
						if (isValidNum(param[1], 0, MAX16)) {
							gCpts[cptIndex].dual.minBreak[i]  = atoi(param[2]);
						}
						if (isValidNum(param[1], 0, MAX16)) {
							gCpts[cptIndex].dual.maxBreak[i]  = atoi(param[3]);
						}
					}					
				}
				printf("cad[%d] : minMake = %dms, maxMake = %dms, minBreak = %dms, maxBreak = %dms \n" \
					, i, gCpts[cptIndex].dual.minMake[i], gCpts[cptIndex].dual.maxMake[i] \
					, gCpts[cptIndex].dual.minBreak[i], gCpts[cptIndex].dual.maxBreak[i]);
			}
		if(EXEC_SUCCESS !=  adamDspConfigCpt(DSP_ID, &gCpts[cptIndex])){
			printf("Error: adamDspConfigTone failed!\n");
			return;
		}
		return;
	}

	_promptCmdHelp(CMD_DSP_CONFIG_CPT);
}

void cmdDspConfigSit(const char *inStr) {
	int i, cptIndex;
	char pVal[MAX_CMD_LEN] = {0};
	char *param[MAX_TOKEN];
	
	CHK_DSP_STATE

	if (1 != getTokens(inStr, " ", param)){
		cptIndex = TONE_SIT - TONE_DIAL;
		gCpts[cptIndex].type =  TONE_SIT;

		printf("SIT configuration: \n");

		for (i = 0; i < SIT_TONE_FREQ; i++) {
			if  (0 == i) {
		 		getAttrVal(inStr, "f0", "=", pVal);
			}
			else if  (1 == i) {
		 		getAttrVal(inStr, "f1", "=", pVal);
			}
			else if  (2 == i) {
				getAttrVal(inStr, "f2", "=", pVal);
			}
			else if  (3 == i) {
				getAttrVal(inStr, "f3", "=", pVal);
			}
			else if  (4 == i) {
		 		getAttrVal(inStr, "f4", "=", pVal);
			}
				
			if (strlen(pVal) > 0) {
				if (2 == getTokens(pVal, ",", param)) {
					if (isValidNum(param[0], 0, 4000)) {
						gCpts[cptIndex].sit.toneFreq[i] = atoi(param[0]);
					}
					if (isValidNum(param[1], 0, 4000)) {
						gCpts[cptIndex].sit.toneDev[i] = atoi(param[1]);
					}
				}
			}
			printf("Freqency[%d] = %dHz, Deviation[%d] = %dHz \n",i ,gCpts[cptIndex].sit.toneFreq[i], i\
				, gCpts[cptIndex].sit.toneDev[i]);
		}
			
		getAttrVal(inStr, "pwr", "=", pVal);
		if (strlen(pVal) >0 && isValidNum(pVal, -40, 0)) {
			gCpts[cptIndex].sit.power = atoi(pVal) * 2;
		}
		printf("Power = %ddb \n", gCpts[cptIndex].sit.power/2);

		getAttrVal(inStr, "sd", "=", pVal);
		if (strlen(pVal) > 0) {
			if (2 == getTokens(pVal, ",", param)) {
				if (isValidNum(param[0], 0, MAX16)) {
					gCpts[cptIndex].sit.minShortDur = atoi(param[0]);
				}
				if (isValidNum(param[1], 0, MAX16)) {
					gCpts[cptIndex].sit.maxShortDur = atoi(param[1]);
				}
			}					
		}
		printf("minShortDur = %dms, maxShortDur = %dms \n", gCpts[cptIndex].sit.minShortDur, gCpts[cptIndex].sit.maxShortDur);

		getAttrVal(inStr, "ld", "=", pVal);
		if (strlen(pVal) > 0) {
			if (2 == getTokens(pVal, ",", param)) {
				if (isValidNum(param[0], 0, MAX16)) {
					gCpts[cptIndex].sit.minLongDur = atoi(param[0]);
				}
				if (isValidNum(param[1], 0, MAX16)) {
					gCpts[cptIndex].sit.maxLongDur = atoi(param[1]);
				}
			}					
		}
		printf("minLongDur = %dms, maxLongDur = %dms \n", gCpts[cptIndex].sit.minLongDur, gCpts[cptIndex].sit.maxLongDur);
			
			
		if(EXEC_SUCCESS !=  adamDspConfigCpt(DSP_ID, &gCpts[cptIndex])){
			printf("Error: adamDspConfigTone failed!\n");
			return;
		}
		return;
	}
	_promptCmdHelp(CMD_DSP_CONFIG_SIT);
}


void cmdChanConfig(const char *inStr) {
	chanConfig_t config;
	chanId_e	ch;
	char pVal[MAX_CMD_LEN] = {0};
	int ret;
	
	CHK_DSP_STATE

 	getAttrVal(inStr, "ch", "=", pVal);
	if (isValidChanId(pVal)) {
		ch = atoi(pVal);

		if (EXEC_SUCCESS != adamChanQuery(DSP_ID, ch, &config)) {
			printf("Execution failed! Cannot retrieve channel configuration.\n\n");
			return;
		}
	
		getAttrVal(inStr, "tx", "=", pVal);
		if (strlen(pVal) > 0 && isValidGain(pVal)) {
			config.ampTx = atoi(pVal) * 2;
		}

		getAttrVal(inStr, "rx", "=", pVal);
		if (strlen(pVal) > 0 && isValidGain(pVal)) {
			config.ampRx = atoi(pVal) * 2;
		}

		getAttrVal(inStr, "dsptx", "=", pVal);
		if (strlen(pVal) > 0 && isValidGain(pVal)) {
			config.digitTx = atoi(pVal) * 2;
		}

		getAttrVal(inStr, "dsprx", "=", pVal);
		if (strlen(pVal) > 0 && isValidGain(pVal)) {
			config.digitRx = atoi(pVal) * 2;
		}
		
		getAttrVal(inStr, "ec", "=", pVal);
		if (strlen(pVal) > 0 && isValidOnOff(pVal)) {
			config.ecEnable = convertControl(pVal);
		}

		getAttrVal(inStr, "ecbypass", "=", pVal);
		if (strlen(pVal) > 0 && isValidOnOff(pVal)) {
			config.ecBypass= convertControl(pVal);
		}

		getAttrVal(inStr, "ecaes", "=", pVal);
		if (strlen(pVal) > 0 && isValidOnOff(pVal)) {
			config.ecAesEnable= convertControl(pVal);
		}

		
		getAttrVal(inStr, "ecnlp", "=", pVal);
		if (strlen(pVal) > 0 && isValidOnOff(pVal)) {
			config.ecNlpEnable= convertControl(pVal);
		}

		getAttrVal(inStr, "ectaillen", "=", pVal);
		if (strlen(pVal) > 0 && isValidLecTailLen(pVal)) {
			config.ecTailMode= convertLecTailLen(pVal);
		}

		getAttrVal(inStr, "det", "=", pVal);
		if (strlen(pVal) > 0) {
			ret = isEnabled(pVal, "dtmf");
			if (EVA_TRUE == ret) {
				config.detectMask |= DETECT_TONE_DTMF;
			}
			else if (EVA_FALSE == ret ) {
				config.detectMask &= ~(DETECT_TONE_DTMF);
			}

			ret = isEnabled(pVal, "mdm");
			if (EVA_TRUE == ret) {
				config.detectMask |= DETECT_TONE_MODEM;
			}
			else if (EVA_FALSE == ret ) {
				config.detectMask &= ~(DETECT_TONE_MODEM);
			}

			ret = isEnabled(pVal, "cpt");
			if (EVA_TRUE == ret) {
				config.detectMask |= DETECT_TONE_CPT;
			}
			else if (EVA_FALSE == ret ) {
				config.detectMask &= ~(DETECT_TONE_CPT);
			}

			ret = isEnabled(pVal, "cid");
			if (EVA_TRUE == ret) {
				config.detectMask |= DETECT_CID;
			}
			else if (EVA_FALSE == ret ) {
				config.detectMask &= ~(DETECT_CID);
			}
			
			ret = isEnabled(pVal, "pulse");
			if (EVA_TRUE == ret) {
				config.detectMask |= DETECT_PULSE;
			}
			else if (EVA_FALSE == ret ) {
				config.detectMask &= ~(DETECT_PULSE);
			}

			ret = isEnabled(pVal, "jb");
			if (EVA_TRUE == ret) {
				config.detectMask |= DETECT_JB_EVENT;
			}
			else if (EVA_FALSE == ret ) {
				config.detectMask &= ~(DETECT_JB_EVENT);
			}
		}

		getAttrVal(inStr, "accdev", "=", pVal);
		if (strlen(pVal) > 0 && isValidAccDev(pVal)){
			getAttrVal(inStr, "rejdev", "=", pVal);
			if (strlen(pVal) > 0 && isValidRejDev(pVal,config.dtmfdet.devAcc)) {
				config.dtmfdet.devRej = atoi(pVal) - DTMF_DEVIATION_BASELINE;
				config.dtmfdet.devAcc= atoi(pVal) - DTMF_DEVIATION_BASELINE;
			}
		}

		getAttrVal(inStr, "dtmfpower", "=", pVal);
		if(strlen(pVal) > 0 && isValidPower(pVal)){
			config.dtmfdet.power = atoi(pVal);
		}

		getAttrVal(inStr, "dtmfdur", "=", pVal);
		if (strlen(pVal) > 0 && isValidDuration(pVal)) {
			config.dtmfdet.duration = atoi(pVal);
		}
	

		if(EXEC_SUCCESS !=  adamChanConfig(DSP_ID, ch, &config)){
			printf("Error: adamChanConfig failed!\n");
			return;
		}
		if (EXEC_SUCCESS != adamChanQuery(DSP_ID, ch, &config)) {
			printf("Execution failed! Cannot retrieve channel configuration.\n\n");
			return;
		}
		printf("Channel (%d) configuration:\n", ch);
		printf("Enabled Detectors:\n");
		printf("%-8s : dtmf (DTMF_TONE)\n", etosActive(config.detectMask & DETECT_TONE_DTMF));
		printf("%-8s : mdm  (FAX/MODEM_TONE)\n", etosActive(config.detectMask & DETECT_TONE_MODEM));
		printf("%-8s : cpt  (CALL_PROGRESS_TONE)\n", etosActive(config.detectMask & DETECT_TONE_CPT));
		printf("%-8s : cid  (Caller_ID)\n", etosActive(config.detectMask & DETECT_CID));
		printf("%-8s : pulse(Pulse event)\n", etosActive(config.detectMask & DETECT_PULSE));
		printf("%-8s : jb   (JB event)\n", etosActive(config.detectMask & DETECT_JB_EVENT));
		printf("EC : %s\n", etosControl(config.ecEnable));
		printf("EC Bypass switch : %s\n", etosControl(config.ecBypass));
		printf("EC AesEnable : %s\n", etosControl(config.ecAesEnable));
		printf("EC NlpEnable  : %s\n", etosControl(config.ecNlpEnable));
		printf("Tx Gain: %ddb\n", config.ampTx/2);
		printf("Rx Gain: %ddb\n", config.ampRx/2);
		printf("DSP Tx Gain: %ddb\n", config.digitTx/2);
		printf("DSP Rx Gain: %ddb\n", config.digitRx/2);
		printf("accept deviation: %d percent\n", config.dtmfdet.devAcc + DTMF_DEVIATION_BASELINE);
		printf("reject deviation: %d percent\n",config.dtmfdet.devRej + DTMF_DEVIATION_BASELINE);
		printf("dtmf detect power: %d dbm\n",config.dtmfdet.power);
		printf("dtmf detect duration: %d ms\n",(config.dtmfdet.duration * DTMF_DURATION_INTERVAL));

		return;
	}
	
	_promptCmdHelp(CMD_CHAN_CONFIG);
}

void cmdChanPlayTone(const char *inStr)
{
	chanId_e	ch;
	char pVal[MAX_CMD_LEN] = {0};
	char *param[MAX_TOKEN];
	int sz =0;
	int i;
	uint32 rpt=0, dur=0 ;
	toneSeq_t toneList;
	uint8 mTone[TONECFG_LAST + 1];

	CHK_DSP_STATE

 	getAttrVal(inStr, "ch", "=", pVal);
	if (isValidChanId(pVal)) {
		ch = atoi(pVal);

		getAttrVal(inStr, "rpt", "=", pVal);
		if (isValidNum(pVal, ~(MAX32 >> 1), MAX32 >> 1)) {
			rpt = (uint32)atoi(pVal);
			printf("Tone repeat times: %d\n", (int)rpt);

			getAttrVal(inStr, "tone", "=", pVal);
			if (strlen(pVal) > 0) {
				sz =getTokens(pVal, ",", param);
				if (sz > TONECFG_LAST) {
					printf("The maximum tone set is %d!\n",TONECFG_LAST);
					return;
				}

				for (i = 0; i < sz; i++) {
					if (isValidToneid(param[i])) {
						mTone[i] = atoi(param[i]);
						printf("Playing Tone (%d).\n", atoi(param[i]));
					}
				}
				toneList.numOfTone = sz;
				toneList.toneIdSeq = mTone;

				/* by mtk07059_20140521 ,for tone_stop_event */
				getAttrVal(inStr, "dur", "=", pVal);
				if (isValidNum(pVal, 10, 300000)) {
					dur = (uint32)atoi(pVal);
				}
				printf("dur:%d\r\n", dur);
				if(EXEC_SUCCESS != adamChanPlayTone(DSP_ID, ch, &toneList,rpt,dur)){
					printf("Error: adamChanPlayTone failed! \n");
					return;
				}

				printf("Generate tone(s) on channel (%d), please check the telephone!\n", ch);
				return;
			}
		}
	}

	_promptCmdHelp(CMD_CHAN_PLAY_TONE);
}

void cmdChanStopTone(const char *inStr)
{
	chanId_e	ch = 0;
	char pVal[MAX_CMD_LEN] = {0};

	CHK_DSP_STATE

 	getAttrVal(inStr, "ch", "=", pVal);
	if (isValidChanId(pVal)) {
		ch = atoi(pVal);
        if(EXEC_SUCCESS != adamChanStopTone(DSP_ID, ch)){
            printf("Error: adamChanStopTone failed! \n");
            return;
        }

        printf("Stop tone(s) on channel (%d), please check the telephone!\n", ch);
        return;
	}

	_promptCmdHelp(CMD_CHAN_STOP_TONE);
}


void cmdChanPlayDtmfTone(const char *inStr)
{
	chanId_e	ch;
	char pVal[MAX_CMD_LEN] = {0};
	uint32 dur=0;
	char digit;

	CHK_DSP_STATE

 	getAttrVal(inStr, "ch", "=", pVal);
	if (isValidChanId(pVal)) {
		ch = atoi(pVal);

		getAttrVal(inStr, "dur", "=", pVal);
		if (isValidNum(pVal, ~(MAX32 >> 1), MAX32 >> 1)) {
			dur = (uint32)atoi(pVal);
			printf("DTMF tone duration: %d\n", (int)dur);

			getAttrVal(inStr, "digit", "=", pVal);
			digit = pVal[0];
			if (isValidDtmfDigit(digit)) {
				printf("Playing DTMF tone (%c).\n", digit);
			}

			if(EXEC_SUCCESS != adamChanPlayDtmf(DSP_ID, ch, digit, dur)){
				printf("Error: adamChanPlayDtmfTone failed! \n");
				return;
			}

			printf("Generate DTMF tone(s) on channel (%d), please check the telephone!\n", ch);
			return;

		}
	}

	_promptCmdHelp(CMD_CHAN_PLAY_DTMF_TONE);
}

void cmdChanPlayCid(const char *inStr) {
	chanId_e	ch;
	char pVal[MAX_CMD_LEN] = {0};
	cid_t cid;
		
	CHK_DSP_STATE

 	getAttrVal(inStr, "ch", "=", pVal);
	if (isValidChanId(pVal)) {
		ch = atoi(pVal);
		
		getAttrVal(inStr, "cid", "=", pVal);
		strcpy(cid.number, pVal);

		getAttrVal(inStr, "name", "=", pVal);
		strcpy(cid.name, pVal);

		getAttrVal(inStr, "cidf", "=", pVal);
		if (strlen(pVal) > 0 && isValidCidFormat(pVal)) {
			cid.format = convertCidFormat(pVal);
		}
		getAttrVal(inStr, "cidp", "=", pVal);
		if (isValidNum(pVal, ~(MAX32 >> 1), MAX32 >> 1)) {
			cid.fskPower= ((uint32)atoi(pVal));
		}
		getAttrVal(inStr, "mwi", "=", pVal);
		cid.mwi = convertMWI(pVal);

		/* added by mtk07059_20140616 ,format:MMddHHmm */
		getAttrVal(inStr, "date", "=", pVal);
		strcpy(cid.dateTime, pVal);

		getAttrVal(inStr, "T2", "=", pVal);
		if(strlen(pVal) > 0 && (isValidNum(pVal, 20, 50))){
			cid.cid1timing.T2 = atoi(pVal);
		}
		else			
			cid.cid1timing.T2 = 30;

		getAttrVal(inStr, "T3", "=", pVal);
		if(strlen(pVal) > 0 && (isValidNum(pVal, 50, 80))){
			cid.cid1timing.T3= atoi(pVal);
		}
		else			
			cid.cid1timing.T3= 65;
		
		getAttrVal(inStr, "T4", "=", pVal);
		if(strlen(pVal) > 0 && (isValidNum(pVal, 4, 50))){
			cid.cid1timing.T4= atoi(pVal);
		}
		else			
			cid.cid1timing.T4= 30;
		

		if(EXEC_SUCCESS != adamChanPlayCid(DSP_ID, ch, &cid)){
			printf("Error: adamChanPlayCid failed! \n");
			return;
		}	

		printf("Generate %s caller ID [%s] name [%s]on channel (%d),the fskower is %d mwi=%s\n" \
			, etosCidFormat(cid.format), cid.number,cid.name, ch,cid.fskPower,gValidMWIStrVal[cid.mwi]);
		printf("T2=:%d(10ms) T3:%d(10ms) T4=:%d(10ms), please check the telephone!\n", cid.cid1timing.T2,cid.cid1timing.T3,cid.cid1timing.T4);
		return;
	}

	_promptCmdHelp(CMD_CHAN_PLAY_CID);
}

void cmdChanPlayCid2(const char *inStr) {
	chanId_e	ch;
	char pVal[MAX_CMD_LEN] = {0};
	cid_t cid;
		
	CHK_DSP_STATE

 	getAttrVal(inStr, "ch", "=", pVal);
	if (isValidChanId(pVal)) {
		ch = atoi(pVal);

		getAttrVal(inStr, "cid", "=", pVal);
		strcpy(cid.number, pVal);

		getAttrVal(inStr, "cidf", "=", pVal);
		if (strlen(pVal) > 0 && isValidCidFormat(pVal)) {
			cid.format = convertCidFormat(pVal);
		}
		
		getAttrVal(inStr, "name", "=", pVal);
		strcpy(cid.name, pVal);
		
		getAttrVal(inStr, "cidp", "=", pVal);
		if (isValidNum(pVal, ~(MAX32 >> 1), MAX32 >> 1)) {
			cid.fskPower= ((uint32)atoi(pVal));
		}
		
		getAttrVal(inStr, "mwi", "=", pVal);
		cid.mwi = convertMWI(pVal);

		getAttrVal(inStr, "date", "=", pVal);
		strcpy(cid.dateTime, pVal);

		getAttrVal(inStr, "T9", "=", pVal);
		if(strlen(pVal) > 0 && (isValidNum(pVal, 0, 15))){
			cid.cid2timing.T9 = atoi(pVal);
		}
		else			
			cid.cid2timing.T9 = 6;
		
		getAttrVal(inStr, "T10", "=", pVal);
		if(strlen(pVal) > 0 && (isValidNum(pVal, 0, 15))){
			cid.cid2timing.T10 = atoi(pVal);
		}
		else			
			cid.cid2timing.T10 = 10;
		
		getAttrVal(inStr, "T11", "=", pVal);
		if(strlen(pVal) > 0 && (isValidNum(pVal, 4, 5))){
			cid.cid2timing.T11= atoi(pVal);
		}
		else			
			cid.cid2timing.T11 = 4;
		
		getAttrVal(inStr, "T12", "=", pVal);
		if(strlen(pVal) > 0 && (isValidNum(pVal, 6, 20))){
			cid.cid2timing.T12= atoi(pVal);
		}
		else			
			cid.cid2timing.T12 = 15;
		
		getAttrVal(inStr, "T13", "=", pVal);
		if(strlen(pVal) > 0 && (isValidNum(pVal, 4, 12))){
			cid.cid2timing.T13= atoi(pVal);
		}
		else			
			cid.cid2timing.T13 = 10;
		
		getAttrVal(inStr, "T14", "=", pVal);
		if(strlen(pVal) > 0 && (isValidNum(pVal, 15, 16))){
			cid.cid2timing.T14= atoi(pVal);
		}
		else
			cid.cid2timing.T14 = 15;

		if(EXEC_SUCCESS != adamChanPlayType2Cid(DSP_ID, ch, &cid)){
			printf("Error: adamChanPlayType2Cid failed! \n");
			return;
		}

		printf("Generate %s caller ID [%s] name [%s] on channel (%d),cidp=:%d mwi=%s T9=%d T10=%d T11=%d T12=%d T13=%d T14=%d" \
			",please check the telephone!\n" \
			, etosCidFormat(cid.format), cid.number,cid.name, ch,cid.fskPower,gValidMWIStrVal[cid.mwi],cid.cid2timing.T9,cid.cid2timing.T10,cid.cid2timing.T11,
			cid.cid2timing.T12,cid.cid2timing.T13,cid.cid2timing.T14);
	
return;
	}

	_promptCmdHelp(CMD_CHAN_PLAY_CID_T2);
}

void cmdChanPlayDtmfCid(const char *inStr) {
	chanId_e	ch;
	char pVal[MAX_CMD_LEN] = {0};
	cid_t cid;
		
	CHK_DSP_STATE
 
 	getAttrVal(inStr, "ch", "=", pVal);
	if (isValidChanId(pVal)) {
		ch = atoi(pVal);

		getAttrVal(inStr, "cid", "=", pVal);
		strcpy(cid.number, pVal);

		getAttrVal(inStr, "make", "=", pVal);
		if (strlen(pVal) > 0 ) {
			cid.makeTime = atoi(pVal);
		}

		getAttrVal(inStr, "break", "=", pVal);
		if (strlen(pVal) > 0 ) {
			cid.breakTime = atoi(pVal);
		}

		getAttrVal(inStr, "dtmfp", "=", pVal);
		if (strlen(pVal) > 0 && isValidNum(pVal, ~(MAX32 >> 1), MAX32 >> 1)) {
			cid.dtmfPower= ((uint32)atoi(pVal));
		}
		else
			cid.dtmfPower=0;

		getAttrVal(inStr, "dtmfT3", "=", pVal);
		if (strlen(pVal) > 0 ) {
			cid.dtmfT3= atoi(pVal);
		}
		else
			cid.dtmfT3= 350;
		  
		if (EXEC_SUCCESS != adamChanPlayDtmfCid(DSP_ID, ch, &cid)){
			printf("Error: adamChanPlayDtmfCid failed! \n");
			return;
		}

		printf("Generate dtmf CID [%s] on channel (%d),maketime:%d breaktime:%d dtmfp:%d dtmfT3:%d,please check the telephone!\n" \
		, cid.number, ch,cid.makeTime,cid.breakTime,cid.dtmfPower,cid.dtmfT3);
		return;
	}

	_promptCmdHelp(CMD_CHAN_PLAY_CID_TDTMF);
}

void cmdChanPcmDump(const char *inStr) {
	chanId_e ch;
	char pVal[MAX_CMD_LEN] = {0};
	netAddr_t mAddr;
	
	
	CHK_DSP_STATE

 	getAttrVal(inStr, "ch", "=", pVal);
	if (isValidChanId(pVal)) {
		ch = atoi(pVal);
		
		getAttrVal(inStr, "ip", "=", pVal);
		if (isValidNum(pVal, 0, 0)) {
			strcpy(pVal, "0.0.0.0");
		}
		
		if (isValidIp(pVal)) {
			mAddr.ver = IPV4;
			
			mAddr.addrV4 = ntohl(inet_addr(pVal));
			if(EXEC_SUCCESS != adamChanDumpPcm(DSP_ID, ch, &mAddr)){
				printf("Error: adamChanDumpPcm failed! \n");
				return;
			}

			if (0 == mAddr.addrV4) {
				printf("Disable channel (%d) PCM dump.\n", ch);
				return;
			}
				
			printf("Dump channel (%d) PCM to (%s).\n", ch, pVal);
		}
		return;
	}

	_promptCmdHelp(CMD_CHAN_PCM_DUMP);
}

void cmdStrmConfig(const char *inStr) {
	chanId_e ch;
	strmId_e st;
	char pVal[MAX_CMD_LEN] = {0};
	char *param[MAX_TOKEN];
	strmConfig_t config;
	activeState_e active;

	CHK_DSP_STATE

 	getAttrVal(inStr, "st", "=", pVal);
	if (isValidStrmId(pVal, &ch, &st)) {

		if (EXEC_SUCCESS != adamStrmQuery(DSP_ID, ch, st, &active, &config)) {
			printf("Execution failed! Cannot retrieve stream configuration.\n\n");
			return;
		}

		getAttrVal(inStr, "src", "=", pVal);
		if (strlen(pVal) > 0) {
			if (3 == getTokens(pVal, ":", param)) {
				if (isValidIp(param[0]) && isValidNum(param[1], 0, MAX16) && isValidNum(param[2], 0, MAX16)) {
					config.session.srcAddr.addrV4 = ntohl(inet_addr(param[0]));
					config.session.srcAddr.port = atoi(param[1]);
					config.session.srcAddr.rtcpPort = atoi(param[2]);
				}
			}	
			else if (2 == getTokens(pVal, ":", param)) {
				if (isValidIp(param[0]) && isValidNum(param[1], 0, MAX16)) {
					config.session.srcAddr.addrV4 = ntohl(inet_addr(param[0]));
					config.session.srcAddr.port = atoi(param[1]);
					config.session.srcAddr.rtcpPort = config.session.srcAddr.port + 1;
				}
			}
		}

		getAttrVal(inStr, "dst", "=", pVal);
		if (strlen(pVal) > 0) {
			if (3 == getTokens(pVal, ":", param)) {
				if (isValidIp(param[0]) && isValidNum(param[1], 0, MAX16) && isValidNum(param[2], 0, MAX16)) {
					config.session.dstAddr.addrV4 = ntohl(inet_addr(param[0]));
					config.session.dstAddr.port = atoi(param[1]);
					config.session.dstAddr.rtcpPort = atoi(param[2]);
				}
			}
			else if (2 == getTokens(pVal, ":", param)) {
				if (isValidIp(param[0]) && isValidNum(param[1], 0, MAX16)) {
					config.session.dstAddr.addrV4 = ntohl(inet_addr(param[0]));
					config.session.dstAddr.port = atoi(param[1]);
					config.session.dstAddr.rtcpPort = config.session.dstAddr.port + 1;
				}
			}
		}
		
		getAttrVal(inStr, "codec", "=", pVal);
		if (strlen(pVal) > 0 && isValidCodec(pVal)) {
			config.strmAttr.payloadSelect = convertCodec(pVal);
		}

		getAttrVal(inStr, "ulcodec", "=", pVal);
		if (strlen(pVal) > 0 && isValidCodec(pVal)) {
			config.strmAttr.ulPayloadSelect= (UL_CODEC_IDENTIFY | convertCodec(pVal));
		}

		/*  added by mtk07059_20150811. */
		getAttrVal(inStr, "ulbr", "=", pVal);
		if (strlen(pVal) > 0 ) {
			config.strmAttr.ulBitrate = convertBitrate(pVal);
		}
        
        /*set the dynamic payloadtype of codec,which include g726_32,arm,redundant ,and so on*/
        GetConfigDynamicPtOfCodec(inStr,&config);

		/* modified by mtk07059_20150320. */
		getAttrVal(inStr, "red", "=", pVal);//mtk69097_for_redundant
		if (strlen(pVal) > 0 && isValidOnOff(pVal)) {
			config.strmAttr.rtpRedundant= convertControl(pVal);
		}

		getAttrVal(inStr, "red_mode", "=", pVal);//mtk69097_for_redundant
		if (strlen(pVal) > 0 && isValidNum(pVal, 0, 3)) {
			config.strmAttr.rtpRedundantMode= atoi(pVal);
		}
		/*set 2833 redundancy seprare with rtp */
		getAttrVal(inStr, "2833_red", "=", pVal);
		if (strlen(pVal) > 0 && isValidOnOff(pVal)) {
			config.strmAttr.rfc2833Redundant= convertControl(pVal);
		}

		getAttrVal(inStr, "2833_red_mode", "=", pVal);
		if (strlen(pVal) > 0 && isValidNum(pVal, 0, 3)) {
			config.strmAttr.rfc2833RedundantMode= atoi(pVal);
		}
		getAttrVal(inStr, "ulpt", "=", pVal);
		if (strlen(pVal) > 0 && isValidPtime(pVal)) {
			config.strmAttr.ulPtime = convertPtime(pVal);
		}
		
		getAttrVal(inStr, "dlpt", "=", pVal);
		if (strlen(pVal) > 0 && isValidPtime(pVal)) {
			config.strmAttr.dlPtime = convertPtime(pVal);
		}
		getAttrVal(inStr, "vad", "=", pVal);		
		if (strlen(pVal) > 0 && isValidOnOff(pVal)) {
			config.strmAttr.VAD= convertControl(pVal);
		}		
		getAttrVal(inStr, "cng", "=", pVal);
		if (strlen(pVal) > 0 && isValidOnOff(pVal)) {
			config.strmAttr.CNG= convertControl(pVal);
		}
		getAttrVal(inStr, "scomp", "=", pVal);
		if (strlen(pVal) > 0 && isValidOnOff(pVal)) {
			config.strmAttr.silenceComp = convertControl(pVal);
		}

#if 0   /*set tos value in  decimal mode instead of hexadecimal mode */
		getAttrVal(inStr, "rtptos", "=", pVal);
		if (strlen(pVal) > 0 && isValidHexNum(pVal, 0,  MAX32 >> 1)) {
			config.session.rtpTosVal = strtol(pVal,NULL,16);
		}

		getAttrVal(inStr, "rtcptos", "=", pVal);
		if (strlen(pVal) > 0 && isValidHexNum(pVal, 0,  MAX32 >> 1)) {
			config.session.rtcpTosVal = strtol(pVal,NULL,16);
		}
#else
		getAttrVal(inStr, "rtptos", "=", pVal);
		if (strlen(pVal) > 0 ) {
			config.session.rtpTosVal = strtol(pVal,NULL,10);
		}

		getAttrVal(inStr, "rtcptos", "=", pVal);
		if (strlen(pVal) > 0 ) {
			config.session.rtcpTosVal = strtol(pVal,NULL,10);
		}
#endif
		getAttrVal(inStr, "dtmfr", "=", pVal);
		if (strlen(pVal) > 0 && isValidOnOff(pVal)) {
			config.strmAttr.dtmfRelay = convertControl(pVal);
		}

		getAttrVal(inStr, "dtmfrm", "=", pVal);
		if (strlen(pVal) > 0 && isValidOnOff(pVal)) {
			config.strmAttr.dtmfRemove = convertControl(pVal);

		}

		getAttrVal(inStr, "dir", "=", pVal);
		if (strlen(pVal) > 0 && isValidStrmdir(pVal)) {
			config.strmAttr.direction = convertStrmdir(pVal);

		}
		
		getAttrVal(inStr, "jb", "=", pVal);
		if (strlen(pVal) > 0) {
			if (3 == getTokens(pVal, ",", param)) {
				if (isValidJbMode(param[0])) {
					config.jbConf.mode = convertJbMode(param[0]);
				}
				if (isValidJbSize(param[1])) {
					config.jbConf.szJbInit = atoi(param[1]);
				}
				if (isValidJbSize(param[2])) {
					if (atoi(param[2]) < atoi(param[1])){
						config.jbConf.szJbMax = atoi(param[1]);
					}else{
						config.jbConf.szJbMax = atoi(param[2]);
					}
				}
			}	
		}
		
		getAttrVal(inStr, "plc", "=", pVal);		
		if (strlen(pVal) > 0 && !strcasecmp(pVal, "off")) {	
			config.session.plc_enable = CONTROL_DISABLE;		
		}		
		else if (strlen(pVal) > 0 && !strcasecmp(pVal, "on")){
			config.session.plc_enable = CONTROL_ENABLE;		
		}	
		
		/* added by mtk07059_20140527, for rtcp report interval set. */
		getAttrVal(inStr, "rtcp_interval", "=", pVal);
		if (isValidNum(pVal, 0, 1000)) {
			config.session.rtcp_report_interval = atoi(pVal);
		}

		/* Add by mtk06404_20140902, for t38 ecm */
		getAttrVal(inStr, "ecm", "=", pVal);
		if (strlen(pVal) > 0 && !strcasecmp(pVal, "off")) {	
			config.t38Ctrl.ecm_enable = CONTROL_DISABLE;		
		}		
		else if (strlen(pVal) > 0 && !strcasecmp(pVal, "on")){
			config.t38Ctrl.ecm_enable = CONTROL_ENABLE;		
		}

		/* enable t38 v34 */
		getAttrVal(inStr, "v34", "=", pVal);
		if (strlen(pVal) > 0 && !strcasecmp(pVal, "on")) {
			config.t38Ctrl.version = 3; /*0: support only v17; 3: support v34 */
			config.t38Ctrl.maxRate = 13;/*5: 14400bps; 13: 33600 bps*/
			config.t38Ctrl.rateMgnt = 2;/*1: do not pass TCF(tcp); 2 - Pass TCF(udp)*/
			config.t38Ctrl.eccType = 1; /*0: None(tcp); 1: Redundancy; 2: FEC*/
			//config.t38Ctrl.ecc_num = 3;
			//config.t38Ctrl.ecc_signal = 2;
			config.t38Ctrl.connection_type = 1; /*0: TCP; 1: UDP*/
			getAttrVal(inStr, "pktsize", "=", pVal);
			if (strlen(pVal) > 0){
				 config.t38Ctrl.imagePktSize=strtol(pVal,NULL,10);
				}
			config.t38Ctrl.max_jitter = 500;
			config.t38Ctrl.nsf_enable = 0; /*permit NSF to pass unchanged.*/
			config.t38Ctrl.opMode = 0; /*0: Unkown; 1: Calling; 2: Called  */
		}
		else {
			config.t38Ctrl.version = 0; /*0: support only v17; 3: support v34 */
			config.t38Ctrl.maxRate = 5;/*5: 14400bps; 13: 33600 bps*/
			config.t38Ctrl.rateMgnt = 2;/*1: do not pass TCF(tcp); 2 - Pass TCF(udp)*/
			config.t38Ctrl.eccType = 1; /*0: None(tcp); 1: Redundancy; 2: FEC*/
			//config.t38Ctrl.ecc_num = 3;
			//config.t38Ctrl.ecc_signal = 2;
			config.t38Ctrl.connection_type = 1; /*0: TCP; 1: UDP*/
			getAttrVal(inStr, "pktsize", "=", pVal);
			if (strlen(pVal) > 0){
				 config.t38Ctrl.imagePktSize=strtol(pVal,NULL,10);
				}
			config.t38Ctrl.max_jitter = 500;
			config.t38Ctrl.nsf_enable = 0; /*permit NSF to pass unchanged.*/
			config.t38Ctrl.opMode = 0; /*0: Unkown; 1: Calling; 2: Called  */
		}
  
		if (EXEC_SUCCESS != adamStrmConfig(DSP_ID, ch, st, &config)) {
			printf("Execution failed! Cannot set stream configuration.\n\n");
			return;
		}
		
		if (EXEC_SUCCESS != adamStrmQuery(DSP_ID, ch, st, &active, &config)) {
			printf("Execution failed! Cannot retrieve stream configuration.\n\n");
			return;
		}		

		printf("Channel %d -> Stream %d Configuration:\n", ch, st);
		printf("Stream state: %s\n",etosActive(active));
		printf("Source address: %s Rtp port: %d  Rtcp port: %d\n", ipToStr(config.session.srcAddr.addrV4) \
		, config.session.srcAddr.port, config.session.srcAddr.rtcpPort);
		printf("Destination address: %s Rtp port: %d  Rtcp port: %d\n", ipToStr(config.session.dstAddr.addrV4) \
		, config.session.dstAddr.port, config.session.dstAddr.rtcpPort);
		printf("RTP tos: %x RTCP tos: %x\n", config.session.rtpTosVal, config.session.rtcpTosVal);
		printf("Codec: %s\n", etosCodec(config.strmAttr.payloadSelect));
		printf("UL Codec: %s\n", etosCodec(config.strmAttr.ulPayloadSelect&0x7f)); 
		printf("red: %s\n",etosActive(config.strmAttr.rtpRedundant));
		printf("red_mode: %d\n",config.strmAttr.rtpRedundantMode);
		printf("2833_red: %s\n",etosActive(config.strmAttr.rfc2833Redundant));
		printf("2833_red_mode: %d\n",config.strmAttr.rfc2833RedundantMode);
        #ifdef AMR_CODEC
		printf("redundant_pt: %d,g726_pt: %d,amr_pt: %d\n",config.payloadType[CODEC_REDUNDANT],config.payloadType[CODEC_G726_32],config.payloadType[CODEC_AMR]);
        #else        
		printf("redundant_pt: %d,g726_pt: %d\n",config.payloadType[CODEC_REDUNDANT],config.payloadType[CODEC_G726_32]);
        #endif
        #ifdef CJ_DSP_SUPPORT
        	printf("dtmful_pt: %d,dtmfdl_pt: %d\n",config.payloadType[CODEC_DTMFR],config.payloadType[CODEC_DTMFR_DL]);
        #endif
		printf("ulPtime: %s  dlPtime: %s\n", etosPtime(config.strmAttr.ulPtime), etosPtime(config.strmAttr.dlPtime));
		printf("ulBitrate: %s \n", etosBitrate(config.strmAttr.ulBitrate));   /*  added by mtk07059_20150811. */
		printf("vad:%s cng:%s\n", etosActive(config.strmAttr.VAD),etosActive(config.strmAttr.CNG));
		printf("Silence compression: %s\n", etosActive(config.strmAttr.silenceComp));
		printf("DTMF Relay: %s DTMF Remove: %s\n", etosActive(config.strmAttr.dtmfRelay), etosActive(config.strmAttr.dtmfRemove));
		printf("Stream direction: %s\n", etosStrmdir(config.strmAttr.direction));		
		printf("RTCP report interval: %d\n", config.session.rtcp_report_interval);
		printf("Jitter Buffer Configuration: %s,  Initial Size: %ld  Max Size: %ld\n", etosJbMode(config.jbConf.mode), config.jbConf.szJbInit, config.jbConf.szJbMax);
		printf("T38 ecm enable: %s\n", etosControl(config.t38Ctrl.ecm_enable));
		printf("T38 version: %d\n", config.t38Ctrl.version);
		printf("T38 maxRate: %d\n", config.t38Ctrl.maxRate);
		printf("T38 opMode: %d\n", config.t38Ctrl.opMode);
		printf("T38 packet size: %d\n",config.t38Ctrl.imagePktSize);
	
		return;
	}

	_promptCmdHelp(CMD_STRM_CONFIG);
}

void cmdStrmStart(const char *inStr) {
	chanId_e ch;
	strmId_e st;
	char pVal[MAX_CMD_LEN] = {0};
	
	CHK_DSP_STATE

	/*for internal debug use. If turn on, you can see JB information when stream start. */
	getAttrVal(inStr, "jbEvent", "=", pVal);
	if (isValidOnOff(pVal)) {
		printJB = convertControl(pVal);
		printf("Update JB Event: %s.\n",etosControl(printJB));
		return;
	}

 	getAttrVal(inStr, "st", "=", pVal);
	if (isValidStrmId(pVal, &ch, &st)) {
	
		if (EXEC_SUCCESS != adamStrmStart(DSP_ID, ch, st)) {
			printf("Execution failed! Cannot start stream .\n\n");
			return;
		}

		printf("Enable streaming process on channel (%d) -> stream (%d).\n", ch, st);
		return;
	}
	_promptCmdHelp(CMD_STRM_START);	
}

void cmdStrmStop(const char *inStr)
{
	chanId_e ch;
	strmId_e st;
	char pVal[MAX_CMD_LEN] = {0};
	
	CHK_DSP_STATE

 	getAttrVal(inStr, "st", "=", pVal);
	if (isValidStrmId(pVal, &ch, &st)) {

		if (EXEC_SUCCESS != adamStrmStop(DSP_ID, ch, st)) {
			printf("Execution failed! Cannot stop stream .\n\n");
			return;
		}
		printJB = CONTROL_DISABLE;
		printf("Disable streaming process on channel (%d) -> stream (%d).\n", ch, st);
		return;
	}

	_promptCmdHelp(CMD_STRM_STOP);	
}

void cmdStrmSendDtmfr(const char *inStr)
{
	chanId_e ch;
	strmId_e st;
	char pVal[MAX_CMD_LEN] = {0};
	dtmf_e dtmf = 0;
	uint32 dur = 0;
	
	CHK_DSP_STATE

 	getAttrVal(inStr, "st", "=", pVal);
	if (isValidStrmId(pVal, &ch, &st)) {
		getAttrVal(inStr, "dtmf", "=", pVal);

  		if (1 == strlen(pVal) && isValidDtmf(pVal)) {
			dtmf = convertDtmf(pVal);	

			getAttrVal(inStr, "dur", "=", pVal);
			if (isValidNum(pVal, 10, 30000)) {
				dur = (uint32)atoi(pVal);
			
				if (EXEC_SUCCESS != adamStrmSendDtmfr(DSP_ID, ch, st, dtmf, dur)) {
					printf("Execution failed! Cannot send stream dtmfr .\n\n");
					return;
				}
			
				printf("Generate %s relay packet to channel (%d) -> stream (%d), dur: %d(ms) " \
				"please check with sniffer!\n", etosDtmf(dtmf), ch, st, (int)dur);
			return;
			}
		}
	}

	_promptCmdHelp(CMD_STRM_SEND_DTMFR);	
}

void cmdStrmPlayTone(const char *inStr) {
	chanId_e ch;
	strmId_e st;
	char pVal[MAX_CMD_LEN] = {0};
	char *param[MAX_TOKEN];
	int sz =0;
	int i;
	uint32 rpt=0;
	toneSeq_t toneList;
	uint8 mTone[TONECFG_LAST + 1];
	
	CHK_DSP_STATE

 	getAttrVal(inStr, "st", "=", pVal);
	if (isValidStrmId(pVal, &ch, &st)) {

		getAttrVal(inStr, "rpt", "=", pVal);
		if (isValidNum(pVal, ~(MAX32 >> 1), MAX32 >> 1)) {
			rpt = (uint32)atoi(pVal);
			getAttrVal(inStr, "tone", "=", pVal);

			if (strlen(pVal) > 0) {
				sz =getTokens(pVal, ",", param);

				if (sz > TONECFG_LAST) {
					printf("The maximum tone set is %d!\n",TONECFG_LAST);
					return;
				}
				
				for (i = 0; i < sz; i++) {
					if (isValidToneid(param[i])) {
						mTone[i] = atoi(param[i]);
						printf("Playing Tone (%d).\n", atoi(param[i]));
					}
				}
				toneList.numOfTone = sz;
				toneList.toneIdSeq = mTone;
				if (EXEC_SUCCESS != adamStrmPlayTone(DSP_ID, ch, st, &toneList, rpt)) {
					printf("Execution failed! Cannot generate tone to stream.\n\n");
				}

				printf("Generate tone(s) on channel (%d) -> stream (%d), please check the peer!\n", ch, st);
				return;
			}
		}
	}

	_promptCmdHelp(CMD_STRM_PLAY_TONE);	
}

void cmdStrmPlayDtmf(const char *inStr)
{
	chanId_e	ch;
	char pVal[MAX_CMD_LEN] = {0};
	uint32 dur=0;
	char digit;

	CHK_DSP_STATE

 	getAttrVal(inStr, "ch", "=", pVal);
	if (isValidChanId(pVal)) {
		ch = atoi(pVal);

		getAttrVal(inStr, "dur", "=", pVal);
		if (isValidNum(pVal, ~(MAX32 >> 1), MAX32 >> 1)) {
			dur = (uint32)atoi(pVal);
			printf("DTMF tone duration: %d\n", (int)dur);

			getAttrVal(inStr, "digit", "=", pVal);
			digit = pVal[0];
			if (isValidDtmfDigit(digit)) {
				printf("Playing DTMF tone (%c).\n", digit);
			}

			if(EXEC_SUCCESS != adamStrmPlayDtmf(DSP_ID, ch, digit, dur)){
				printf("Error: adamStrmPlayDtmf failed! \n");
				return;
			}

			printf("Generate DTMF tone on far-end direction ,channel (%d), please check the telephone!\n", ch);
			return;

		}
	}

	_promptCmdHelp(CMD_STRM_PLAY_DTMF);
}

void cmdStrmQueryMediaInfo(const char *inStr){
	chanId_e ch;
	strmId_e st;
	char pVal[MAX_CMD_LEN] = {0};
	mediaInfo_t mediaInfo;

	memset(&mediaInfo, 0, sizeof(mediaInfo_t));
	
	CHK_DSP_STATE

 	getAttrVal(inStr, "st", "=", pVal);
	if (isValidStrmId(pVal, &ch, &st)) {

		if (EXEC_SUCCESS != adamStrmQueryMediaInfo(DSP_ID, ch, st, &mediaInfo)) {
			printf("Execution failed! Cannot Query Media Info .\n\n");
			return;
		}
		printf("Query Media Info on channel (%d) -> stream (%d).\n", ch, st);
		printf("RtpError	(%lld).\n", mediaInfo.rtpError);
		printf("Packet Loss 	(%lld).\n", mediaInfo.packetLoss);
		printf("Packet recv 	(%lld).\n", mediaInfo.packetRecv);
		printf("Packet Loss rate(%lld).\n", mediaInfo.packetLossRate);
		printf("Packet OutOfTime(%lld).\n", mediaInfo.packetOutoftime);
		printf("Packet Discarded(%lld).\n", mediaInfo.packetDiscarded);
		printf("maxJitter	(%lld).\n", mediaInfo.maxJitter);
		printf("maxRTCPInterval (%lld).\n", mediaInfo.maxRTCPInterval);
		printf("Average RTCPInterval	(%lld).\n", mediaInfo.avgRTCPInterval);
		/*add by kidd OSBNB00030595 20130407*/
		printf("Packet sent			(%lld).\n", mediaInfo.packetSent);
		printf("Bytes  recv			(%lld).\n", mediaInfo.bytesRecv);
		printf("Bytes  sent			(%lld).\n", mediaInfo.bytesSent);
		printf("Farend Packet Loss rate	(%lld).\n", mediaInfo.farEndpacketLossRate);
		printf("Average Jitter			(%lld).\n", mediaInfo.avgJitter);
		printf("Farend Jitter		(%lld).\n", mediaInfo.farEndJitter);
		printf("Average farend Jitter (%lld).\n", mediaInfo.avgFarEndJitter);		
		/*end of add by kidd OSBNB00030595 20130407*/
		printf("bufUnderflow 	(%lld).\n", mediaInfo.bufUnderflow);
		printf("bufOverflow 	(%lld).\n", mediaInfo.bufOverflow);
		printf("receive internal Jitter (%lld).\n", mediaInfo.revInternalJitter);
		printf("Average receive internal Jitter	(%lld).\n", mediaInfo.avgRevInternalJitter);
		printf("roundTripDelay 	(%lld).\n", mediaInfo.roundTripDelay);
		printf("Average roundTripDelay 	(%lld).\n", mediaInfo.avgRoundTripDelay);
		printf("Max roundTripDelay 	(%lld).\n", mediaInfo.maxRoundTripDelay);
                printf("rtcp Packet Recv 	(%lld).\n", mediaInfo.rtcpPacketRecv);
                printf("rtcp Packet Send 	(%lld).\n", mediaInfo.rtcpPacketSend);
                printf("rtcp Xr Packet Recv 	(%lld).\n", mediaInfo.rtcpXrPacketRecv);
                printf("rtcp Xr Packet Send 	(%lld).\n", mediaInfo.rtcpXrPacketSend);
        	printf("MOSLQ 	(%f).\n", mediaInfo.MOSLQ);
        	printf("MOSCQ 	(%f).\n", mediaInfo.MOSCQ);
		return;
	}

	_promptCmdHelp(CMD_STRM_QUERY_MEDIA_INFO);
}
void cmdStrmResetMediaInfo(const char *inStr){
	chanId_e ch;
	strmId_e st;
	char pVal[MAX_CMD_LEN] = {0};
	
	CHK_DSP_STATE

 	getAttrVal(inStr, "st", "=", pVal);
	if (isValidStrmId(pVal, &ch, &st)) {

		if (EXEC_SUCCESS != adamStrmResetMediaInfo(DSP_ID, ch, st)) {
			printf("Execution failed! Cannot Reset Media Info .\n\n");
			return;
		}
		printf("Reset Media Info on channel (%d) -> stream (%d).\n", ch, st);
		return;
	}

	_promptCmdHelp(CMD_STRM_RESET_MEDIA_INFO);
}


void cmdInfcQuery(const char *inStr)
{
	infcId_e infc = 0;
	char pVal[MAX_CMD_LEN] = {0};
	infcConfig_t infcConf;
	slicParams_t slicParams;/* added by mtk08176_20150508  */
	int i;

	CHK_DSP_STATE

 	getAttrVal(inStr, "if", "=", pVal);
	if (isValidInfcId(pVal)) {
		infc = atoi(pVal);
		printf("Interface (%d) configuration:\n", infc);

		if(EXEC_SUCCESS != adamInfcQuery(infc, &infcConf)){
			printf("Error: adamChanPlayCid failed! \n");
			return;
		}
	
		/* added by mtk08176_20150508  */
		if(EXEC_SUCCESS != adamInfcSlicTypeQuery(infc, &slicParams)){
			printf("Error: adamInfcSlicTypeQuery failed! \n");
			return;
		}
	
		printf("Interface type: %s\n", etosInfctype(infcConf.type));
		printf("Line State: %s\n", etosLine(infcConf.lineState));
		printf("Hook State: %s\n", etosHook(infcConf.hookState));
		printf("Hook Threshold: Min_flashTime:%ld Max_flashTime:%ld Min_releaseTime:%ld autoFlashDur:%ld\n", \
			infcConf.hookTs.flashMin, infcConf.hookTs.flashMax, infcConf.hookTs.releaseMin, infcConf.hookTs.autoFlashDur);
		printf("pulse Gen Seize:%ld pulse Gen Release:%ld\n",\ 
			infcConf.hookTs.pulseTime.genSeize, infcConf.hookTs.pulseTime.genRelease);	
		printf("pulse Det Seize Max:%ld pulse Det Seize Min:%ld\n", \
			infcConf.hookTs.pulseTime.detSeizeMax, infcConf.hookTs.pulseTime.detSeizeMin); 
		printf("pulse Det Release Max:%ld pulse Det Release Min:%ld\n", \
			infcConf.hookTs.pulseTime.detReleaseMax, infcConf.hookTs.pulseTime.detReleaseMin); 	
		
		/* added by mtk08176_20150508  */
		printf("fxsNum:%d\n", slicParams.fxsNum);
		printf("fxoNum:%d\n", slicParams.fxoNum);
		printf("slicType:%s\n", slicParams.slicType);
		
		if (INFC_FXS == infcConf.type) {
			printf("Ring configuration: \n");
			for (i = 0; i < MAX_CADENCE; i++) {
				printf("Cadence%d: %d(ms)/on, %d(ms)/off\n", i, infcConf.ring.cad[i].onTime, \
										infcConf.ring.cad[i].offTime);
			}
			printf("Caller ID: number=[%s]\n", infcConf.ring.cid.number);
			printf("Caller ID Generation at (%d)th break, wait time=%d\n", infcConf.ring.cidAt, infcConf.ring.cidWaitTime);
			return;
		}
		return;
	}	
	_promptCmdHelp(CMD_INFC_QUERY);
}

void cmdInfcConfigLine(const char *inStr) {
	infcId_e infc = 0;
	infcConfig_t infcConf;
	lineState_e lineState = LINE_ERROR;
	char pVal[MAX_CMD_LEN] = {0};

	CHK_DSP_STATE

 	getAttrVal(inStr, "if", "=", pVal);
	if (isValidInfcId(pVal)) {
		infc = atoi(pVal);
		
		if(EXEC_SUCCESS != adamInfcQuery(infc, &infcConf)){
			printf("Error: adamInfcQuery failed! \n");
			return;
		}
	
		if (INFC_FXS == infcConf.type) {
		 	getAttrVal(inStr, "line", "=", pVal);
			lineState = convertLineState(pVal);

			if (LINE_ERROR == lineState) {
				printf("Incorrect line state! line=[down | fwd | rev | ring | revoht]");//MTK69036 REV_OHT
			}
			else {
				infcConf.lineState = lineState;

				if(EXEC_SUCCESS != adamInfcConfigLine(infc, infcConf.lineState)){
					printf("Error: adamInfcConfigLine failed! \n");
					return;
				}
					
				if(EXEC_SUCCESS != adamInfcQuery(infc, &infcConf)){
					printf("Error: adamInfcQuery failed! \n");
					return;
				}

				printf("Line State: %s\n", etosLine(infcConf.lineState));
				return;
			}
		}
		else {
			printf("Error: Command only support for FXS interface!\n");
		}
		return;
	}
	
	_promptCmdHelp(CMD_INFC_CONFIG_LINE);
}

void cmdInfcConfigHook(const char *inStr) {
	infcId_e infc = 0;
	infcConfig_t infcConf;
	hookState_e hookState = (-1);
	char pVal[MAX_CMD_LEN] = {0};

	CHK_DSP_STATE

 	getAttrVal(inStr, "if", "=", pVal);
	if (isValidInfcId(pVal)) {
		infc = atoi(pVal);
		
		if(EXEC_SUCCESS != adamInfcQuery(infc, &infcConf)){
			printf("Error: adamInfcQuery failed! \n");
			return;
		}
		if (INFC_FXO == infcConf.type) {
		 	getAttrVal(inStr, "hook", "=", pVal);
			hookState = convertHookState(pVal);

			if (-1 == hookState) {
				printf("Incorrect hook state: on | off | flash | pulse1~pulse20");
			}
			else {
				infcConf.hookState = hookState;

				if(EXEC_SUCCESS != adamInfcConfigHook(infc, infcConf.hookState)){
					printf("Error: adamInfcConfigHook failed! \n");
					return;
				}
					
				if(EXEC_SUCCESS != adamInfcQuery(infc, &infcConf)){
					printf("Error: adamInfcQuery failed! \n");
					return;
				}

				printf("Hook State: %s\n", etosHook(infcConf.hookState));
				return;
			}
		}
		else {
			printf("Error: Command only support for FXO interface!\n");
		}
		return;
	}
	
	_promptCmdHelp(CMD_INFC_CONFIG_HOOK);
}


void cmdInfcConfigHwProf(const char *inStr) {
	infcId_e infc = 0;
	hwProf_e hwProf = 0;
	infcConfig_t infcConf;
	hookState_e hookState = (-1);
	char pVal[MAX_CMD_LEN] = {0};

	CHK_DSP_STATE

 	getAttrVal(inStr, "if", "=", pVal);
	if (isValidInfcId(pVal)) {
		infc = atoi(pVal);
		
		if(EXEC_SUCCESS != adamInfcQuery(infc, &infcConf)){
			printf("Error: adamInfcQuery failed! \n");
			return;
		}
	getAttrVal(inStr, "hwProf", "=", pVal);
	hwProf = atoi(pVal);	
	adamInfcConfigHwProf(infc,hwProf);	
	printf("Interface (%d) ,now the country code:%d\n", infc,hwProf);
	}
	_promptCmdHelp(CMD_INFC_HW_PROF);
}

void cmdInfcConfigHookTs(const char *inStr) {
	infcId_e infc = 0;
	infcConfig_t infcConf;
	char pVal[MAX_CMD_LEN] = {0};
	char *param[MAX_TOKEN];

	CHK_DSP_STATE

	getAttrVal(inStr, "if", "=", pVal);
	if (isValidInfcId(pVal)) {
		infc = atoi(pVal);
		
		if(EXEC_SUCCESS != adamInfcQuery(infc, &infcConf)){
			printf("Error: adamInfcQuery failed! \n");
			return;
		}

		getAttrVal(inStr, "fmin", "=", pVal);
		if (isValidNum(pVal, 0, MAX32 >> 1)) {
			infcConf.hookTs.flashMin = atoi(pVal);
		}

		getAttrVal(inStr, "fmax", "=", pVal);
		if (isValidNum(pVal, 0, MAX32 >> 1)) {
			infcConf.hookTs.flashMax = atoi(pVal);
		}

		getAttrVal(inStr, "rmin", "=", pVal);
		if (isValidNum(pVal, 0, MAX32 >> 1)) {
			infcConf.hookTs.releaseMin = atoi(pVal);
		}

		getAttrVal(inStr, "smin", "=", pVal);
		if (isValidNum(pVal, 0, MAX32 >> 1)) {
			infcConf.hookTs.seizeMin = atoi(pVal);
		}

		getAttrVal(inStr, "flash", "=", pVal);
		if (isValidNum(pVal, 0, MAX32 >> 1)) {
			infcConf.hookTs.autoFlashDur = atoi(pVal);
		}

		getAttrVal(inStr, "pgs", "=", pVal);
		if (isValidNum(pVal, 0, MAX32 >> 1)) {
			infcConf.hookTs.pulseTime.genSeize = atoi(pVal);
		}

		getAttrVal(inStr, "pgr", "=", pVal);
		if (isValidNum(pVal, 0, MAX32 >> 1)) {
			infcConf.hookTs.pulseTime.genRelease = atoi(pVal);
		}

		getAttrVal(inStr, "pdsmax", "=", pVal);
		if (isValidNum(pVal, 0, MAX32 >> 1)) {
			infcConf.hookTs.pulseTime.detSeizeMax = atoi(pVal);
		}

		getAttrVal(inStr, "pdsmin", "=", pVal);
		if (isValidNum(pVal, 0, MAX32 >> 1)) {
			infcConf.hookTs.pulseTime.detSeizeMin = atoi(pVal);
		}

		getAttrVal(inStr, "pdrmax", "=", pVal);
		if (isValidNum(pVal, 0, MAX32 >> 1)) {
			infcConf.hookTs.pulseTime.detReleaseMax = atoi(pVal);
		}

		getAttrVal(inStr, "pdrmin", "=", pVal);
		if (isValidNum(pVal, 0, MAX32 >> 1)) {
			infcConf.hookTs.pulseTime.detReleaseMin = atoi(pVal);
		}
		
		getAttrVal(inStr, "pdd", "=", pVal);
		if (strlen(pVal) > 0 && isValidOnOff(pVal)) {
			infcConf.hookTs.pulseEnable = convertControl(pVal);
		}

		if (infcConf.hookTs.releaseMin <  infcConf.hookTs.flashMax) {
			infcConf.hookTs.releaseMin = infcConf.hookTs.flashMax;
		}

		if (infcConf.hookTs.pulseTime.detSeizeMax < infcConf.hookTs.pulseTime.detSeizeMin) {
			infcConf.hookTs.pulseTime.detSeizeMax = infcConf.hookTs.pulseTime.detSeizeMin;
		}

		if (infcConf.hookTs.pulseTime.detReleaseMax < infcConf.hookTs.pulseTime.detReleaseMin) {
			infcConf.hookTs.pulseTime.detReleaseMax = infcConf.hookTs.pulseTime.detReleaseMin;
		}
		
		if(EXEC_SUCCESS != adamInfcConfigHookTs(infc, &infcConf.hookTs)){
			printf("Error: adamInfcConfigHookTs failed! \n");
			return;
		}
			
		if(EXEC_SUCCESS != adamInfcQuery(infc, &infcConf)){
			printf("Error: adamInfcQuery failed! \n");
			return;
		}
				

		printf("Hook Threshold: Min_flashTime:%ld Max_flashTime:%ld Min_releaseTime:%ld autoFlashDur:%ld seizeMin:%ld\n", \
			infcConf.hookTs.flashMin, infcConf.hookTs.flashMax, infcConf.hookTs.releaseMin, infcConf.hookTs.autoFlashDur , infcConf.hookTs.seizeMin);
		printf("pulse Gen Seize:%ld pulse Gen Release:%ld\n",\ 
			infcConf.hookTs.pulseTime.genSeize, infcConf.hookTs.pulseTime.genRelease);	
		printf("pulse Det Seize Max:%ld pulse Det Seize Min:%ld\n", \
			infcConf.hookTs.pulseTime.detSeizeMax, infcConf.hookTs.pulseTime.detSeizeMin); 
		printf("pulse Det Release Max:%ld pulse Det Release Min:%ld\n", \
			infcConf.hookTs.pulseTime.detReleaseMax, infcConf.hookTs.pulseTime.detReleaseMin); 
		printf("pulse Det Enable:%d\n",infcConf.hookTs.pulseEnable);
					
		return;
	}
	
	_promptCmdHelp(CMD_INFC_CONFIG_HOOKTS);
}

void cmdInfcConfigRing(const char *inStr) {
	infcId_e infc = 0;
	infcConfig_t infcConf;
	char *param[MAX_TOKEN];
	char pVal[MAX_CMD_LEN] = {0};
	int i;

	CHK_DSP_STATE

 	getAttrVal(inStr, "if", "=", pVal);
	if (isValidInfcId(pVal)) {
		infc = atoi(pVal);
		
		if(EXEC_SUCCESS != adamInfcQuery(infc, &infcConf)){
			printf("Error: adamInfcQuery failed! \n");
			return;
		}

		if (INFC_FXS == infcConf.type) {
 			getAttrVal(inStr, "dur", "=", pVal);
			if (isValidNum(pVal, ~(MAX32 >> 1), MAX32 >> 1)) {
				infcConf.ring.dur = (uint32)atoi(pVal);
			}

 			getAttrVal(inStr, "cidb", "=", pVal);
			if (isValidNum(pVal, 0, MAX8)) {
				infcConf.ring.cidAt= atoi(pVal);
			}

			getAttrVal(inStr, "cidt", "=", pVal);
			if (isValidNum(pVal, ~(MAX32 >> 1), MAX32 >> 1)) {
				infcConf.ring.cidWaitTime= (uint32)atoi(pVal);
			}

			getAttrVal(inStr, "cidp", "=", pVal);
			if (isValidNum(pVal, ~(MAX32 >> 1), MAX32 >> 1)) {
				infcConf.ring.cid.fskPower= ((uint32)atoi(pVal))*2;
			}

			getAttrVal(inStr, "cidf", "=", pVal);
			if (strlen(pVal) > 0 && isValidCidFormat(pVal)) {
				infcConf.ring.cid.format= convertCidFormat(pVal);
			}

			for (i = 0; i < MAX_CADENCE; i++) {
				if  (0 == i) {
		 			getAttrVal(inStr, "cad0", "=", pVal);
				}
				else if  (1 == i) {
		 			getAttrVal(inStr, "cad1", "=", pVal);
				}
				else if  (2 == i) {
		 			getAttrVal(inStr, "cad2", "=", pVal);
				}
					
					
				if (strlen(pVal) > 0) {
					if (2 == getTokens(pVal, ",", param)) {
						if (isValidNum(param[0], 0, MAX16)) {
							infcConf.ring.cad[i].onTime = atoi(param[0]);
						}
						if (isValidNum(param[1], 0, MAX16)) {
							infcConf.ring.cad[i].offTime = atoi(param[1]);
						}
					}
				}
			}

			getAttrVal(inStr, "T2", "=", pVal);
			if(strlen(pVal) > 0 && (isValidNum(pVal, 20, 50))){
				infcConf.ring.cid.cid1timing.T2 = atoi(pVal);
			}
			else			
				infcConf.ring.cid.cid1timing.T2 = 30;

			getAttrVal(inStr, "T3", "=", pVal);
			if(strlen(pVal) > 0 && (isValidNum(pVal, 50, 80))){
				infcConf.ring.cid.cid1timing.T3= atoi(pVal);
			}
			else			
				infcConf.ring.cid.cid1timing.T3= 65;
			
			getAttrVal(inStr, "T4", "=", pVal);
			if(strlen(pVal) > 0 && (isValidNum(pVal, 4, 50))){
				infcConf.ring.cid.cid1timing.T4= atoi(pVal);
			}
			else			
				infcConf.ring.cid.cid1timing.T4= 30;


 			getAttrVal(inStr, "cid", "=", pVal);
			printf("cid length = %d\n", strlen(pVal));
			strcpy(infcConf.ring.cid.number, pVal);

			getAttrVal(inStr, "mwi", "=", pVal);
            infcConf.ring.cid.mwi = convertMWI(pVal);
			printf("mwi=%s\n",gValidMWIStrVal[infcConf.ring.cid.mwi]);
			
			getAttrVal(inStr, "maketime", "=", pVal);			
			if (strlen(pVal) > 0 )
				infcConf.ring.cid.makeTime = atoi(pVal);
			printf("cid maketime = %d\n", infcConf.ring.cid.makeTime);

			getAttrVal(inStr, "breaktime", "=", pVal);			
			if (strlen(pVal) > 0 )
				infcConf.ring.cid.breakTime = atoi(pVal);
			printf("cid breaktime = %d\n", infcConf.ring.cid.breakTime);

			getAttrVal(inStr, "dtmfp", "=", pVal);
			if (strlen(pVal) > 0 && isValidNum(pVal, ~(MAX32 >> 1), MAX32 >> 1)) {
				infcConf.ring.cid.dtmfPower= ((uint32)atoi(pVal));
			}
			else
				infcConf.ring.cid.dtmfPower=0;
			
			getAttrVal(inStr, "dtmfT3", "=", pVal);
			if (strlen(pVal) > 0 ) {
				infcConf.ring.cid.dtmfT3= atoi(pVal);
			}
			else
				infcConf.ring.cid.dtmfT3= 350;	

			if(EXEC_SUCCESS != adamInfcConfigRing(infc, &(infcConf.ring))){
				printf("Error: adamInfcConfigRing failed! \n");
				return;
			}
					
			if(EXEC_SUCCESS != adamInfcQuery(infc, &infcConf)){
				printf("Error: adamInfcQuery failed! \n");
			return;
			}

			printf("Ring configuration: \n");
			for (i = 0; i < MAX_CADENCE; i++) {
				printf("Cadence%d: %d(ms)/On, %d(ms)/Off\n", i, infcConf.ring.cad[i].onTime, \
											infcConf.ring.cad[i].offTime);
			}
			printf("Ring duration= %d(ms)\n", (int)infcConf.ring.dur);
			printf("Caller ID: number=[%s] cid power=[%d] format=[%s] mwi=[%s]\n"\
					, infcConf.ring.cid.number, infcConf.ring.cid.fskPower,\
					etosCidFormat(infcConf.ring.cid.format),gValidMWIStrVal[infcConf.ring.cid.mwi]);
			printf("Caller ID Generation at (%d)th ring break, wait time=%d\n", infcConf.ring.cidAt, infcConf.ring.cidWaitTime);
			printf("Dtmf type cid's maketime=%d breaktime=%d dtmfT3=%d\n",infcConf.ring.cid.makeTime,infcConf.ring.cid.breakTime,infcConf.ring.cid.dtmfT3);
			printf("FSK Power= %d\n", (int)infcConf.ring.cid.fskPower);
			printf("DTMF Power= %d\n", (int)infcConf.ring.cid.dtmfPower);
			printf("T2=:%d(10ms) T3:%d(10ms) T4=:%d(10ms)\n", infcConf.ring.cid.cid1timing.T2,infcConf.ring.cid.cid1timing.T3,infcConf.ring.cid.cid1timing.T4);

			return;
		}
		else {
			printf("Error: Command only support for FXS interface!\n");
		}
		return;	
	}
	
	_promptCmdHelp(CMD_INFC_CONFIG_RING);
}

void cmdInfcRing(const char *inStr) {
	infcId_e infc = 0;
	char pVal[MAX_CMD_LEN] = {0};
	infcConfig_t infcConf;

	CHK_DSP_STATE

 	getAttrVal(inStr, "if", "=", pVal);
	if (isValidInfcId(pVal)) {
		infc = atoi(pVal);

		if(EXEC_SUCCESS != adamInfcQuery(infc, &infcConf)){
							printf("Error: adamInfcQuery failed! \n");
							return;
				}

		getAttrVal(inStr, "dur", "=", pVal);
		if (isValidNum(pVal, ~(MAX32 >> 1), MAX32 >> 1)) {
			infcConf.ring.dur = (uint32)atoi(pVal);
		}
		
		if (INFC_FXS == infcConf.type) {
			printf("Ring interface (%d), check the telephone.\n", infc);
			if(EXEC_SUCCESS != adamInfcRing(infc, infcConf.ring.dur, &(infcConf.ring.cid))){
				printf("Error: adamInfcRing failed! \n");
				return;
			}
		}
		else {
			printf("Error: Command only support for FXS interface!\n");
		}
		return;
	}

	_promptCmdHelp(CMD_INFC_RING);
}

/* added by mtk07059_20140610 ,  */
void cmdPcmLoopback(const char *inStr){
	
	char pVal[MAX_CMD_LEN] = {0};
	
	CHK_DSP_STATE
		
 	getAttrVal(inStr, "switch", "=", pVal);

	if (!strcasecmp("on", pVal)){
		adamPcmLoopback(DSP_ID, CONTROL_ENABLE);
		return;
	}
	else if (!strcasecmp("off", pVal)){
		adamPcmLoopback(DSP_ID, CONTROL_DISABLE);
		return;
	}
	_promptCmdHelp(CMD_PCM_LOOPBACK);
}

/*[OSBNB00041138] ADD by peter.jiang@20141126, support rtp/dsp loopback*/
void cmdRtpLoopback(const char *inStr){
	chanId_e	ch;
	char pVal[MAX_CMD_LEN] = {0};
		
	CHK_DSP_STATE

 	getAttrVal(inStr, "ch", "=", pVal);
	if (isValidChanId(pVal)) {
		ch = atoi(pVal);
		
		getAttrVal(inStr, "switch", "=", pVal);
		
		if (!strcasecmp("on", pVal)){
			adamRtpLoopback(DSP_ID, ch, CONTROL_ENABLE);
			return;
		}
		else if (!strcasecmp("off", pVal)){
			adamRtpLoopback(DSP_ID, ch, CONTROL_DISABLE);
			return;
		}
		return;
	}
	_promptCmdHelp(CMD_RTP_LOOPBACK);
}

void cmdDspLoopback(const char *inStr){
	chanId_e	ch;
	char pVal[MAX_CMD_LEN] = {0};
	
	CHK_DSP_STATE

 	getAttrVal(inStr, "ch", "=", pVal);
	if (isValidChanId(pVal)) {
		ch = atoi(pVal);
			
	 	getAttrVal(inStr, "switch", "=", pVal);

		if (!strcasecmp("on", pVal)){
			adamDspLoopback(DSP_ID, ch, CONTROL_ENABLE);
			return;
		}
		else if (!strcasecmp("off", pVal)){
			adamDspLoopback(DSP_ID, ch, CONTROL_DISABLE);
			return;
		}
		return;
	}
	_promptCmdHelp(CMD_DSP_LOOPBACK);
}

void cmdDspReplaceData(const char *inStr){
	chanId_e	ch;
	replaceData_t pReplaceConf;
	char pVal[MAX_CMD_LEN] = {0};
	
	CHK_DSP_STATE

 	getAttrVal(inStr, "ch", "=", pVal);
	if (isValidChanId(pVal)) {
		ch = atoi(pVal);
	}else{
		goto usage;
	}
			
 	getAttrVal(inStr, "enable", "=", pVal);
	if (strlen(pVal) > 0 && isValidOnOff(pVal)) {
		pReplaceConf.enable= convertControl(pVal);
	}else{
		goto usage;
	}

	getAttrVal(inStr, "dir", "=", pVal);
	if (strlen(pVal) > 0 && isValidDspdir(pVal)) {
		pReplaceConf.dir= convertDspdir(pVal);

	}else{
		goto usage;
	}
	
	getAttrVal(inStr, "stage", "=", pVal);
	if (strlen(pVal) > 0 && isValidDspStageNum(pVal,0,16)) {
		pReplaceConf.stage= atoi(pVal);

	}else{
		goto usage;
	}
	
	getAttrVal(inStr, "filepath", "=", pVal);
	if(strlen(pVal)>0 && strlen(pVal)<sizeof(pReplaceConf.filePath)){
		strncpy(pReplaceConf.filePath,pVal,strlen(pVal));
	}else{
		goto usage;
	}
	
	adamDSPReplaceData(DSP_ID, ch, &pReplaceConf);
	return;
	
usage:
	_promptCmdHelp(CMD_DSP_REPLACEDATA);
}

/* ADD by mtk08176_20150508 , support line test.*/
infcLineTest_t lineTest;

void cmdInfcLineTest(const char *inStr){
	char pVal[MAX_CMD_LEN] = {0};
	slicParams_t slicParams;
	int tmpSlicType;
	
	CHK_DSP_STATE

	memset(&lineTest, 0, sizeof(infcLineTest_t));
 	getAttrVal(inStr, "if", "=", pVal);
	if (isValidChanId(pVal)) {
		lineTest.infc= atoi(pVal);
			
	 	getAttrVal(inStr, "testid", "=", pVal);
		lineTest.lineTestId= atoi(pVal);

		if(EXEC_SUCCESS != adamInfcLineTest(&lineTest)){
			printf("Error: adamInfcLineTest failed! \n");
			return;
		}

		if(EXEC_SUCCESS != adamInfcSlicTypeQuery(lineTest.infc, &slicParams)){
			printf("Error: adamInfcSlicTypeQuery failed! \n");
			return;
		}
		tmpSlicType= convertSlicType(slicParams.slicType);
		if(tmpSlicType == TYPE_ZARLINK)
			{
					switch (lineTest.lineTestId)
					{					
						case EVA_LT_TID_LINE_V:
						{	
							  /* Test results for the test LT_TID_LINE_V */
								EVA_LtLineVResultType *pResultTemp ;
								pResultTemp=(EVA_LtLineVResultType*)lineTest.lineTestData;
								
								printf("   Line Test Result -- LT_TID_LINE_V\n");
								printf("	fltMask 	 =	%d\n" , 				  pResultTemp->fltMask	   )  ;  
								printf("	measStatus	 =	%d\n" , 				  pResultTemp->measStatus  )  ;
								printf("	vAcTip		 =	%ldmVrms\n" ,					  pResultTemp->vAcTip	   )  ;
								printf("	vAcRing 	 =	%ldmVrms\n" ,			  pResultTemp->vAcRing	  )  ;	
								printf("	vAcDiff 	 =	%ldmVrms\n" ,				pResultTemp->vAcDiff	)  ;  
								printf("	vDcTip		 =	%ldmV\n" ,				pResultTemp->vDcTip    )  ;   
								printf("	vDcRing 	 =	%ldmV\n" ,					pResultTemp->vDcRing   )  ;  
								printf("	vDcDiff 	 =	%ldmV\n\n" ,				pResultTemp->vDcDiff	)  ; 
						}	
						break;
		
						case EVA_LT_TID_ROH:
						{
							  /* Test results for the test LT_TID_ROH */
							  
							  EVA_LtRohResultType *pResultTemp ;														 
							  pResultTemp=(EVA_LtRohResultType*)lineTest.lineTestData;
							  printf("	 Line Test Result -- LT_TID_ROH\n");
							  printf("	  fltMask	   =  %d\n" ,					pResultTemp->fltMask	)  ; 
							  printf("	  rLoop1	   =  %ld\n" ,					pResultTemp->rLoop1 	)  ; 
							  printf("	  rLoop2	   =  %ld\n" ,						pResultTemp->rLoop2 	)  ; 
							  printf("	  measStatus   =  %d\n\n" , 			pResultTemp->measStatus )  ;  
							  
						}	 
						break;
						
						case EVA_LT_TID_RINGERS:
						{				
								/* Test results for the test LT_TID_RINGERS */
								EVA_LtRingersResultType *pResultTemp ;
								pResultTemp=(EVA_LtRingersResultType*)lineTest.lineTestData;
								printf("   Line Test Result -- LT_TID_RINGERS in SLIC ECHO\n ");
								printf("	fltMask 		=  %d\n" ,				  pResultTemp->fltMask		   )  ;  
								printf("	measStatus		=  %d\n" ,				  pResultTemp->measStatus	   )  ;
								printf("	ren 		=  %ld\n" , 					  pResultTemp->ren			   )  ;
								printf("	rentg			=  %ld\n" , 			  pResultTemp->rentg		  )  ;	
								printf("	renrg			=  %ld\n" , 				pResultTemp->renrg			)  ;  
								printf("	ringerTestType		=  %d\n\n" ,				pResultTemp->ringerTestType)  ;  
								
						}	  
						break;
		
						case EVA_LT_TID_RES_FLT:
						{
							  /* Test results for the test LT_TID_RES_FLT */
							  EVA_LtResFltResultType *pResultTemp ;
							  pResultTemp=(EVA_LtResFltResultType*)lineTest.lineTestData;
							  printf("	 Line Test Result -- LT_TID_RES_FLT\n ");
							  printf("	 fltMask	=  %d\n" , pResultTemp->fltMask   )  ;	
							  printf("	 rtg		=  %ld\n" , pResultTemp->rrg		  )  ;
							  printf("	 rrg		=  %ld\n" , pResultTemp->rrg		  )  ;
							  printf("	 rtr		=  %ld\n" , pResultTemp->rtr		  )  ;	
							  printf("	 rGnd		=  %ld\n" , pResultTemp->rGnd		)  ;  
							  printf("	 measStatus =  %d\n\n" , pResultTemp->measStatus )	;  
						}	
						//VpSetLineState(pLCtx, VP_LINE_STANDBY);
						break;	
						
						case EVA_LT_TID_DC_FEED_ST:
						{
							  /* Test results for the test LT_TID_DC_FEED_ST */
							  EVA_LtDcFeedSTResultType *pResultTemp ;
							  pResultTemp=(EVA_LtDcFeedSTResultType*)lineTest.lineTestData;
							  printf("	 Line Test Result -- LT_TID_DC_FEED_ST\n");
							  printf("	  fltMask	   =  %d\n" ,		 pResultTemp->fltMask		 );
							  printf("	  measStatus   =  %d\n" ,		 pResultTemp->measStatus	 );
							  printf("	  rTestLoad    =  %ld\n" ,		 pResultTemp->rTestLoad 	 );
							  printf("	  vTestLoad    =  %ld\n" ,		 pResultTemp->vTestLoad 	 );
							  printf("	  iTestLoad    =  %ld\n\n" ,		 pResultTemp->iTestLoad 	 ); 
						}
						break;
						
						case EVA_LT_TID_DC_VOLTAGE:
						{
							  /* Test results for the test LT_TID_DC_VOLTAGE */
							  EVA_LtDcVoltageResultType *pResultTemp ;
							  pResultTemp=(EVA_LtDcVoltageResultType*)lineTest.lineTestData;
							  printf("	 Line Test Result -- LT_TID_DC_VOLTAGE\n");
							  printf("	  fltMask			=  %d\n" ,		  pResultTemp->fltMask			   );
							  printf("	  measStatus		=  %d\n" ,		  pResultTemp->measStatus		   );
							  printf("	  measuredVoltage1	=  %ld\n" , 	  pResultTemp->measuredVoltage1    );
							  printf("	  measuredVoltage2	=  %ld\n\n" ,		  pResultTemp->measuredVoltage2    );
						}
						break;
						
						case EVA_LT_TID_RINGING_ST:
						{
							  /* Test results for the test LT_TID_RINGING_ST */
							  EVA_LtRingingSTResultType *pResultTemp ;
							  pResultTemp=(EVA_LtRingingSTResultType*)lineTest.lineTestData;
							  printf("	 Line Test Result -- LT_TID_RINGING_ST\n");
							  printf("	  fltMask		=  %d\n" ,		  pResultTemp->fltMask			   );
							  printf("	  openCktRingVol	=  %ld\n" , 	  pResultTemp->openCktRingVol	   );
							  printf("	  freq			=  %ld\n" , 	  pResultTemp->freq 			   );
							  printf("	  acRload		=  %ld\n" , 	  pResultTemp->acRload			   );
							  printf("	  iRload		=  %ld\n" , 	  pResultTemp->iRload			   );
							  printf("	  measStatus		=  %d\n\n" ,		  pResultTemp->measStatus		   );
				//				  ReadReg(0x01,VP880_SIGA_PARAMS_RD, VP880_SIGA_PARAMS_LEN);
				//				  VpInitLine(pLCtx, AC_600R_FXS, DC_25MA_CC, RING_20HZ_SINE);
				//				  VpInitRing(pLCtx, RING_CAD_STD, VP_PTABLE_NULL);
							  //VpSetLineState(pLCtx, VP_LINE_STANDBY);
		
						}	  
						break;
						
						case EVA_LT_TID_ON_OFF_HOOK_ST:
						{
							  /* Test results for the test LT_TID_ON_OFF_HOOK_ST */
							  EVA_LtOnOffHookSTResultType *pResultTemp ;
							  pResultTemp=(EVA_LtOnOffHookSTResultType*)lineTest.lineTestData;
							  printf("	 Line Test Result -- LT_TID_ON_OFF_HOOK_ST\n");
							  printf("	  fltMask	   =  %d\n" ,		 pResultTemp->fltMask		 );
							  printf("	  measStatus   =  %d\n\n" , 	 pResultTemp->measStatus	 ); 
						}	  
						break;	  
						
						case EVA_LT_TID_RD_BAT_COND:
						{	
							  /* Test results for the test LT_TID_RD_BAT_COND */
							  EVA_LtReadBatCondResultType *pResultTemp ;
							  pResultTemp=(EVA_LtReadBatCondResultType*)lineTest.lineTestData;
							  printf("	 Line Test Result -- LT_TID_RD_BAT_COND\n");
							  printf("	  fltMask		   =  %d\n" ,		 pResultTemp->fltMask		);
							  printf("	  measStatus	=  %d\n" ,		 pResultTemp->measStatus	);
							  printf("	  bat1				=  %ldmV\n" ,		 pResultTemp->bat1			);
							  printf("	  bat2				=  %ldmv\n" ,		 pResultTemp->bat2			);
							  printf("	  bat3				=  %ldmV\n\n" , 	 pResultTemp->bat3			);
						}
							  
						break;	  
						case EVA_LT_TID_PRE_LINE_V:
						{	
							  /* Test results for the test LT_TID_PRE_LINE_V */
							  EVA_LtPreLineVResultType *pResultTemp ;
							  pResultTemp=(EVA_LtPreLineVResultType*)lineTest.lineTestData;
							  printf("	 Line Test Result -- LT_TID_PRE_LINE_V\n");
							  printf("	  fltMask	   =  %d\n" ,		 pResultTemp->fltMask		);
							  printf("	  measStatus   =  %d\n\n" , 	 pResultTemp->measStatus	);
						}	  
						break;	
		
						case EVA_LT_TID_CAP:
						{	
							  /* Test results for the test LT_TID_PRE_LINE_V */
							  EVA_LtCapResultType *pResultTemp ;
							  pResultTemp=(EVA_LtCapResultType*)lineTest.lineTestData;
							  printf("	 fltMask	= %d\n" ,pResultTemp->fltMask	  )  ;	
							  printf("	 ctg		= %ld\n" , pResultTemp->ctg 	  )  ;
							  printf("	 crg		= %ld\n" , pResultTemp->crg 	  )  ;
							  printf("	 ctr		= %ld\n" , pResultTemp->ctr 	  )  ;	
						}	  
						break;	
						case EVA_LT_TID_RD_LOOP_COND:
						{	
							  /* Test results for the test LT_TID_RD_BAT_COND */
							  EVA_LtRdLoopCondResultType *pResultTemp ;
							  pResultTemp=(EVA_LtRdLoopCondResultType*)lineTest.lineTestData;
							  printf("	 Line Test Result -- LT_TID_RD_LOOP_COND\n");
							  printf("	  fltMask		  =  %d\n" ,		 pResultTemp->fltMask		);
							  printf("	  measStatus	  =  %d\n" ,		 pResultTemp->measStatus	);
							  printf("	  rloop 			=  %ld/10ohm\n" ,		pResultTemp->rloop );
							  printf("	  imt			  =  %lduA\n" , 	 pResultTemp->imt			);
							  printf("	  ilg			  =  %lduA\n" , 	 pResultTemp->ilg			);
							  printf("	  vab			  =  %ldmV\n" , 	 pResultTemp->vab			);
							  printf("	  vag			  =  %ldmV\n" , 	 pResultTemp->vag			);
							  printf("	  vbg			  =  %ldmV\n" , 	 pResultTemp->vbg			);
							  printf("	  vbat1 		  =  %ldmV\n" , 	 pResultTemp->vbat1 		);
							  printf("	  vbat2 		  =  %ldmv\n" , 	 pResultTemp->vbat2 		);
							  printf("	  vbat3 		  =  %ldmV\n\n" ,		 pResultTemp->vbat3 		);
						}
						break;
						
						default:
							{
								printf("NO FUNDING testItem\n");
								break;
							}
						}
			}
			else if(tmpSlicType == TYPE_SILICON)
				{	EVA_ProSLICMLTType *pProSLICMLT;
					EVA_proslicMonitorType *inline_monitor;
					
					if(lineTest.lineTestId!=8)
						pProSLICMLT=(EVA_ProSLICMLTType*)lineTest.lineTestData;
					else
						inline_monitor=(EVA_proslicMonitorType*)lineTest.lineTestData;
					switch (lineTest.lineTestId)
					{					
						case EVA_LT_TID_HAZARDOUS_V:
						case EVA_LT_TID_FOREIGN_V:
						{	
							 /* 
							**
							** Example Hazardous/Foreign Voltages Test
							**
							*/
							/* Process Available Results */
							/*
							** Available Results:
							**
							** pProSLICMLT->hazVDC.measTR		TR DC Voltage (mv)
							** pProSLICMLT->hazVDC.measTG		TG DC Voltage (mv)
							** pProSLICMLT->hazVDC.measRG		RG DC Voltage (mv)
							** pProSLICMLT->hazVAC.measTR		TR AC Voltage (Vrms^2/100)
							** pProSLICMLT->hazVAC.measTG		TG AC Voltage (Vrms^2/100)
							** pProSLICMLT->hazVAC.measRG		RG AC Voltage (Vrms^2/100)
							**
							*/
							  printf("	  Hazardous/Foreign Voltages Test\n");
							  printf("	  pProSLICMLT->hazVDC.measTR   =  %d\n" ,	 pProSLICMLT->hazVDC.measTR );
							  printf("	  pProSLICMLT->hazVDC.measTG   =  %d\n" ,	 pProSLICMLT->hazVDC.measTG );
							  printf("	  pProSLICMLT->hazVDC.measRG   =  %d\n" ,	pProSLICMLT->hazVDC.measRG	);
							  printf("	  pProSLICMLT->hazVAC.measTR   =  %d\n" ,	 pProSLICMLT->hazVAC.measTR );
							  printf("	  pProSLICMLT->hazVAC.measT    =  %d\n" ,	 pProSLICMLT->hazVAC.measTG );
							  printf("	  pProSLICMLT->hazVAC.measRG   =  %d\n" ,	 pProSLICMLT->hazVAC.measRG );
						}	
						break;
						case EVA_LT_TID_RESISTIVE_FAULTS:
						{
							/* 
							**
							** Example Resistive Faults Test
							**
							*/
							  /* Process Available Results */
							  /*
							  ** Available Results:
							  **
							  ** pProSLICMLT->resFaults.measTR		  RTR (ohms/10)
							  ** pProSLICMLT->resFaults.measTG		  RTG (ohms/10)
							  ** pProSLICMLT->resFaults.measRG		  RRG (ohms/10)
							  **
							  */
							  printf("	  Resistive Faults Test\n");
							  printf("	  pProSLICMLT->resFaults.measTR 			=  %d\n" ,		 pProSLICMLT->resFaults.measTR );
							  printf("	  pProSLICMLT->resFaults.measTG 		  =  %d\n" ,		 pProSLICMLT->resFaults.measTG	);
							  printf("	  pProSLICMLT->resFaults.measRG 		  =  %d\n" ,		pProSLICMLT->resFaults.measRG	);					  
						}	 
						break;	
						case EVA_LT_TID_RES_OFFHOOK:
						{
							  
								/* 
								**
								** Example Receiver Offhook Test
								**
								*/
		
								/* Process Available Results */
								/*
								** Available Results:
								**
								** pProSLICMLT->roh.rohTrue 	
								**			RC_NONE 					No fault or receiver detected
								**			RC_MLT_ROH_FAIL_ROH 		Offhook receiver detected
								**			RC_MLT_ROH_FAIL_RESFAULT	Resistive fault detected
								*/
		
								printf("	Receiver Offhook Test\n");
								printf("	Receiver Offhook Test Result	 =	%d\n" , 	   pProSLICMLT->roh.rohTrue );				
				
						}
						break;					
						case EVA_LT_TID_REN:
						{				
							/* 
							**
							** Example REN Test
							**
							*/
							
							/* Process Available Results */
							/*
							** Available Results:
							**
							** pProSLICMLT->ren.renValue		REN (unitless/10)	
							**
							*/
							printf("	REN Test\n");
							printf("	pProSLICMLT->ren.renValue	 =	%d\n" , 	pProSLICMLT->ren.renValue );
		
								
						}	  
						break;
						
						case EVA_LT_TID_CAP_FAULTS:
						{
		
							/* 
							**
							** Example Capacitive Faults Test
							**
							*/
							/* Process Available Results */
							/*
							** Available Results:
							**
							** pProSLICMLT->capFaults.measTR		CTR (nF/10)
							** pProSLICMLT->capFaults.measTG		CTG (nF/10)
							** pProSLICMLT->capFaults.measRG		CRG (nF/10)
							**
							*/
							  printf("	  Capacitive Faults Test\n");
							  printf("	  pProSLICMLT->capFaults.measTR 		 =	%d\n" , 	 pProSLICMLT->capFaults.measTR	);
							  printf("	  pProSLICMLT->capFaults.measTG 		  =  %d\n" ,		 pProSLICMLT->capFaults.measTG	);
							  printf("	  pProSLICMLT->capFaults.measRG 		  =  %d\n" ,	   pProSLICMLT->capFaults.measRG	);				
		
						}
						break;
						
						case EVA_LT_TID_REN_CAP:
						{	
							/* 
							**
							** Example REN capacitance Test
							**
							*/
							/* Process Available Results */
							/*
							** Available Results:
							**
							** pProSLICMLT->capFaults.measTR		CTR (nF)
							**
							*/
							printf("	REN Capacitive Test\n");
							printf("	pProSLICMLT->capFaults.measTR2		  =  %d\n" ,	   pProSLICMLT->capFaults.measTR2  );			
		
						}
						break;

						case EVA_ProSLIC_LineMonitor:
						{	
							/* 
							**
							** In Line Monitor
							**
							**
							*/
							printf ("vtr = %d mv\n",inline_monitor->vtr);
							printf ("vtip = %d mv\n",inline_monitor->vtip );
							printf ("vring = %d mv\n",inline_monitor->vring);
							printf ("VBAT = %d mv\n",inline_monitor->vbat );
							printf ("itr = %d uA\n",inline_monitor->itr );
							printf ("itip = %d uA\n",inline_monitor->itip );
							printf ("iring = %d uA\n",inline_monitor->iring);
							printf ("ilong = %d uA\n",inline_monitor->ilong);
						}
						break;
						
						default:
						{
							printf("NO such testItem\n");				
						}
							break;
						}
		
			
				}

			else{
				printf("NO such slictype\n");
			}

		return;
	}
	_promptCmdHelp(CMD_INFC_LINE_TEST);
}


/*[OSBNB00045371] ADD by mtk07059_20150423 , support ring parameters set. */
void cmdInfcRingParams(const char *inStr){
	infcId_e infc = 0;
	char pVal[MAX_CMD_LEN] = {0};
	infcConfig_t infcConf;
	infcRingParams_t ringParams;

	CHK_DSP_STATE

 	getAttrVal(inStr, "if", "=", pVal);
	if (isValidInfcId(pVal)) {
		infc = atoi(pVal);

		if(EXEC_SUCCESS != adamInfcQuery(infc, &infcConf)){
							printf("Error: adamInfcQuery failed! \n");
							return;
		}

		if (INFC_FXS == infcConf.type) {
			getAttrVal(inStr, "prof", "=", pVal);	
			if (strlen(pVal) > 0 ) {	
				if (!strcasecmp("sine", pVal)){
					infcConf.ringParams.type = RING_SINE;
				}
				else if (!strcasecmp("trap", pVal)){
					infcConf.ringParams.type = RING_TRAP;				
				}
				else{
					printf("Error: Incorrect profile type!\n");
					return ;
				}
			}	
			
			getAttrVal(inStr, "freq", "=", pVal);
			if (strlen(pVal) > 0) {
				if(isValidNum(pVal, 1000, 100000)){
					infcConf.ringParams.frequence = atoi(pVal);
				}
				else{
					printf("Error: frequence should between 1~100 Hz!\n");
					return ;
				}
			}
			
			getAttrVal(inStr, "amp", "=", pVal);
			if (strlen(pVal) > 0) {
				if(isValidNum(pVal, 0, 85000)){
					infcConf.ringParams.amplitude = atoi(pVal);
				}
				else{
					printf("Error: amplitude should below 100 Vpk!\n");
					return ;				
				}
			}
			
			getAttrVal(inStr, "dcb", "=", pVal);
			if (strlen(pVal) > 0) {
				if(isValidNum(pVal, 0, 100000-infcConf.ringParams.amplitude)){
					infcConf.ringParams.dcBias = atoi(pVal);
				}
				else{
					printf("Error: sum of dcBias and amplitude should below 100 Vpk!\n");
					return ;				
				}
			}

			/* added by mtk08176_20150605  */
			getAttrVal(inStr, "rtt", "=", pVal);
			if (strlen(pVal) > 0) {
				if(isValidNum(pVal, 0, 62500)){
					infcConf.ringParams.ringTripThreshold = atoi(pVal);
				}
				else{
					printf("Error: ringTripThreshold should below 62500 ua!\n");
					return ;				
				}
			}

			/* added by mtk08176_20150627  */
			getAttrVal(inStr, "ampslab", "=", pVal);
			if (strlen(pVal) > 0) {
				if(isValidNum(pVal, 47, 65)){
					infcConf.ringParams.amplitudeSlab = atoi(pVal);
				}
				else{
					printf("Error: amplitude slab should between 47 and 65!\n");
					return ;				
				}
			}

			if(EXEC_SUCCESS != adamInfcRingParams(infc, &infcConf.ringParams)){
				printf("Error: adamInfcRingParams failed! \n");
				return;
			}

					
			if(EXEC_SUCCESS != adamInfcQuery(infc, &infcConf)){
				printf("Error: adamInfcQuery failed! \n");
				return;
			}

			printf("Ring params: \n");
			printf("Type:%d;\nFrequency:%d;\nAmplitude:%d;\ndcBias:%d;\nringTripThreshold:%d;\nAmplitudeSlab:%d;\n",	/* added by mtk08176_20150627  */
				infcConf.ringParams.type, infcConf.ringParams.frequence, infcConf.ringParams.amplitude, infcConf.ringParams.dcBias, \ 
				infcConf.ringParams.ringTripThreshold,infcConf.ringParams.amplitudeSlab);
		}
		else {
			printf("Error: Command only support for FXS interface!\n");
		}
		return;
	}
	_promptCmdHelp(CMD_INFC_RING_PARAMS);
}

/*[OSBNB00047036] ADD by mtk07059_20150616 , support ring parameters set. */
void cmdInfcDcFeedParams(const char *inStr){
	infcId_e infc = 0;
	char pVal[MAX_CMD_LEN] = {0};
	infcConfig_t infcConf;
	infcDcFeedParams_t dcFeedParams;

	CHK_DSP_STATE

 	getAttrVal(inStr, "if", "=", pVal);
	if (isValidInfcId(pVal)) {
		infc = atoi(pVal);

		if(EXEC_SUCCESS != adamInfcQuery(infc, &infcConf)){
							printf("Error: adamInfcQuery failed! \n");
							return;
		}

		if (INFC_FXS == infcConf.type) {
			
			getAttrVal(inStr, "ila", "=", pVal);
			if (strlen(pVal) > 0) {
				if(isValidNum(pVal, 18000, 40000)){
					infcConf.dcFeedParams.ila = atoi(pVal);
				}
				else{
					printf("Error: ila should between 18000~40000 uA!\n");
					return ;
				}
			}
			
			/* added by mtk08176_20150627  */
			getAttrVal(inStr, "ilaSlab", "=", pVal);
			if (strlen(pVal) > 0) {
				if(isValidNum(pVal, 18, 28)){
					infcConf.dcFeedParams.ilaSlab = atoi(pVal);
				}
				else{
					printf("Error: ila slab should between 18~28 mA!\n");
					return ;
				}
			}

			if(EXEC_SUCCESS != adamInfcDcFeedParams(infc, &infcConf.dcFeedParams)){
				printf("Error: adamInfcDcFeedParams failed! \n");
				return;
			}

					
			if(EXEC_SUCCESS != adamInfcQuery(infc, &infcConf)){
				printf("Error: adamInfcQuery failed! \n");
				return;
			}

			printf("DC Feed params: \n");
			printf("ILA:%d;\n",	infcConf.dcFeedParams.ila);
			printf("ILA SLAB:%d;\n",	infcConf.dcFeedParams.ilaSlab);/* added by mtk08176_20150627  */
		}
		else {
			printf("Error: Command only support for FXS interface!\n");
		}
		return;
	}
	_promptCmdHelp(CMD_INFC_DCFEED_PARAMS);
}

/* ADD by mtk08176_20150824, China Unicom inside and outside line test */
infcUnicomLineTest_t unicomLineTest;

void cmdInfcCULineTest(const char *inStr){
	infcId_e infc = 0;
	char pVal[MAX_CMD_LEN] = {0};
	slicParams_t slicParams;
	int tmpSlicType;

	CHK_DSP_STATE

	memset(&lineTest, 0, sizeof(infcLineTest_t));
 	getAttrVal(inStr, "if", "=", pVal);
	if (isValidChanId(pVal)){
		lineTest.infc= atoi(pVal);
			
		if(EXEC_SUCCESS != adamInfcSlicTypeQuery(lineTest.infc, &slicParams)){
			printf("Error: adamInfcSlicTypeQuery failed! \n");
			return;
		}
		tmpSlicType= convertSlicType(slicParams.slicType);

		if(EXEC_SUCCESS != adamInfcCULineTest(&lineTest, tmpSlicType, &unicomLineTest)){
			printf("Error: adamInfcCULineTest failed! \n");
			return;
		}
		printf("    China Unicom inside and outside line test result and Params:\n");
		printf("	AGACVoltage 	 =	%d mVrms\n" , 			unicomLineTest.Outline.AGACVoltage	  )  ; 
		printf("	BGACVoltage		 =	%d mVrms\n" ,				  unicomLineTest.Outline.BGACVoltage	  )  ; 
		printf("	ABACVoltage		 =	%d mVrms\n" ,				unicomLineTest.Outline.ABACVoltage	  )  ; 
		printf("	AGDCVoltage	 =	%d mV\n" ,			  unicomLineTest.Outline.AGDCVoltage )  ; 
		printf("	BGDCVoltage 	 =	%d mV\n" , 			unicomLineTest.Outline.BGDCVoltage	  )  ; 
		printf("	ABDCVoltage		 =	%d mV\n" ,				  unicomLineTest.Outline.ABDCVoltage	  )  ; 
		printf("	AGInsuluationResistance		 =	%d ohm\n" ,				unicomLineTest.Outline.AGInsuluationResistance	  )  ; 
		printf("	BGInsuluationResistance	 =	%d ohm\n" ,			  unicomLineTest.Outline.BGInsuluationResistance )  ; 
		printf("	ABInsuluationResistance 	 =	%d ohm\n" , 			unicomLineTest.Outline.ABInsuluationResistance	  )  ; 
		printf("	AGCapacitance		 =	%d pF\n" ,				  unicomLineTest.Outline.AGCapacitance	  )  ; 
		printf("	BGCapacitance		 =	%d pF\n" ,				unicomLineTest.Outline.BGCapacitance	  )  ; 
		printf("	ABCapacitance	 =	%d pF\n" ,			  unicomLineTest.Outline.ABCapacitance )  ; 
		printf("	DCFeed_Voltage 	 =	%d V\n" , 			unicomLineTest.Inline.DCFeed_Voltage	  )  ; 
		printf("	Ringing_Voltage		 =	%d Vrms\n" ,				  unicomLineTest.Inline.Ringing_Voltage	  )  ; 
		printf("	Loop_Current		 =	%d mA\n" ,				unicomLineTest.Inline.Loop_Current	  )  ; 
		printf("	OutlineStatus	 =	%s\n" ,			  etosCUOutlineStatus(unicomLineTest.OutlineStatus))  ; 
		printf("	InlineDCVStatus  =	%s\n" , 		  etosCUInlineStatus(unicomLineTest.InlineDCVStatus))	; 
		printf("	InlineRingVStatus  =	%s\n" , 	 etosCUInlineStatus(unicomLineTest.InlineRingVStatus) )	;
		printf("	InlineCurrentStatus  =	%s\n" , 	etosCUInlineStatus(unicomLineTest.InlineCurrentStatus)); 

		return;
 	}
	_promptCmdHelp(CMD_INFC_CULINETEST_PARAMS);
}



void cmdQuit(const char *inStr) {
	char cmdLine[MAX_CMD_LEN] = {0};
	printf("Quit evcom and shutdown DSP, sure? (type 'yes' to quit) ");

	fgets(cmdLine, MAX_CMD_LEN, stdin);
	cmdLine[strlen(cmdLine) - 1] = '\0';
	adjustBackSpace(cmdLine);

	if (!strcasecmp("yes", cmdLine)) {
		if (1 == gDspState) {
			adamStrmStopAll(DSP_ID); /*release streams before exiting*/
			if (EXEC_SUCCESS != adamDspRevoke(DSP_ID)) {
				printf("Error: DSP shutdown failed!\n");;
				return;
			}

			gDspState++;
			printf("DSP shutdown completed! You can quit evcom safely now.\n");
		}
		gRunTask = EVA_FALSE;
		printf("Exit evcom... goodbye!\n\n");
	}
}
#if 1//def TCSUPPORT_CY_VOIP_SIP
#define FILE_WITH_VOIP_TEST_CFG "/userfs/withvoiptest.cfg"
#define FILE_VOIP_TESTER_CFG "/userfs/voiptester.cfg"
#define VOICE_FILE_PATH_LEN	160
int invokeOnce = 0;
int evcomApplication = 0;//1: with test machine; 2: tester; 3 wait tester digit input
unsigned long EVCOM_SINGAL;
unsigned long RING_SINGAL;
typedef enum {
    TEST_INT=0,         ///< 0, registered
    TEST_ON,        ///< 1: with test machine;
    TEST_TESTER,       ///<  2: tester
    TEST_TESTER_WAIT_DTMF,       ///<  3 wait tester digit input
   
} evcomApplication_state; 
void quitEvcom(void)
{
	if(evcomApplication == TEST_TESTER)
	{
		char tmp[32] = {0};
		printf("Strm Stop....\n\n");
		sprintf(tmp,"sp st=0:0");
		cmdStrmStop(tmp);

		printf("Revoking DSP ...\n\n");
		if (EXEC_SUCCESS != adamDspRevoke(DSP_ID)) {
			printf("Error: DSP shutdown failed!\n");;
		}
		printf("DSP shutdown completed! You can quit evcom safely now.\n");

		if(gRunTask){
			gRunTask = 0;
			sem_post(&EVCOM_SINGAL);
		}
	}
	return;
}
static char recvBuf[2][32]={0};
#define RING_DTMF "*159#"
void recvDigit(int chan, int digit)
{
	char lastRecv[32]={0};
	char tmp='\0';
	if(evcomApplication !=TEST_TESTER_WAIT_DTMF){
		return;
	}
	if(strlen(recvBuf[chan])>=32){
		memset(recvBuf[chan],0,32);
	}
	memcpy(lastRecv,recvBuf[chan],32);
	tmp=etocDtmf(digit);
	
	sprintf(recvBuf[chan],"%s%c",lastRecv,tmp);
	return;
}
void checkDigit(int chan)
{
	if(evcomApplication !=TEST_TESTER_WAIT_DTMF)
		return;
	if(strcmp(recvBuf[chan],RING_DTMF)==0){
		memset(recvBuf[chan],0,32);
		sem_post(&RING_SINGAL);
	}else{
	    printf("\nWrong digit!Type in %s please!\n",RING_DTMF);
		//evcomApplication=TEST_TESTER;
		//sem_post(&RING_SINGAL);
	    memset(recvBuf[chan],0,32);
	}
	return;
}
#endif

/*sigal_handler is needed for releasing all rtp streams if there are any active ones
**before process exit*/
void signal_handler(int signum)
{
	psignal (signum, "Get signal");
	if (EXEC_SUCCESS != adamStrmStopAll (DSP_ID)) {
		printf("Error: stream stop failed!\n");
	}

	sleep(1);
	exit(1);
}

int 
main(int argc, char* argv[]) {
	char cmdLine[MAX_CMD_LEN];
	char *param[MAX_TOKEN];
	command_e cmd;
	adamConfig_t adConf;
#if 1//def TCSUPPORT_CY_VOIP_SIP
	FILE *fp = NULL;
	char filepath[VOICE_FILE_PATH_LEN] = {0};
	int ret = 0;
#endif
    	
	signal(SIGHUP, signal_handler);
	signal(SIGABRT, signal_handler);
	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGQUIT, signal_handler);
	signal(SIGSEGV, signal_handler);
	signal(SIGFPE, signal_handler);

	printf("Initializing ADAM ... \n");
	if (EXEC_SUCCESS != adamInit()) {
		printf("ADAM initialization failed!! Exit the applicatoin!\n\n");
		exit(-1);
	}

	if (EXEC_SUCCESS == adamQuery(&adConf)) {
		printf("ADAM Version: %s\n", adConf.version);
		printf("Number of DSP: %d\n", adConf.dspNum);
		printf("Number of Interface: %d\n",  adConf.infcNum);
	}
#if 1//def TCSUPPORT_CY_VOIP_SIP
	if((argc == 2) && ((strcmp(argv[1],"on")==0)||(strcmp(argv[1],"tester")==0)))
	{
		evcomApplication = TEST_INT;
		signal(SIGHUP, SIG_IGN);
		if(strcmp(argv[1],"on")==0) // with test machine
		{
			evcomApplication = TEST_ON;
			strncpy(filepath,FILE_WITH_VOIP_TEST_CFG,(VOICE_FILE_PATH_LEN-1));
		}
		else if(strcmp(argv[1],"tester")==0)//test machine
		{		

			evcomApplication=TEST_TESTER_WAIT_DTMF;
			cmdDspInvoke("di");
			invokeOnce = 1;
			ret = sem_init(&RING_SINGAL,0,0);
			if(ret != 0)
			{
				printf("Semaphore SEM_INFORM initialization fail!\n");
				return -1;
			}
			while(1){
				sem_wait(&RING_SINGAL);
				sem_destroy(&RING_SINGAL);
				break;
			}
			evcomApplication = TEST_TESTER;
			strncpy(filepath,FILE_VOIP_TESTER_CFG,(VOICE_FILE_PATH_LEN-1));
		}
		else
		{
			printf("usage: evcom [on | tester]");
			return -1;
		}

		ret = sem_init(&EVCOM_SINGAL,0,0);
		if(ret != 0)
		{
			printf("Semaphore SEM_INFORM initialization fail!\n");
			return -1;
		}
		
		while(gRunTask)
		{
			fp = fopen(filepath,"r");
			
			if(fp == NULL)
			{
				printf("open %s fail\n",filepath);				
				sem_destroy(&EVCOM_SINGAL);
				return -1;
			}
			
			memset(cmdLine, 0, sizeof(cmdLine));
			while(fgets(cmdLine,MAX_CMD_LEN,fp) != NULL)
			{
				cmd = CMD_INVALID;
				CONSOLE_PROMPT
				
				cmdLine[strlen(cmdLine)-2] = '\0';
				adjustBackSpace(cmdLine);
				if (strlen(cmdLine) > 0) {
					if (0 < getTokens(cmdLine, " ", param)) {
						cmd = getCmdCode(gCmdSet, param[0]);
					}

					if ((CMD_INVALID == cmd) || ((CMD_DSP_INVOKE == cmd)&&(invokeOnce == 1))) {
						_promptCmd();
						continue;
					}
					gCmdSet[cmd].func(cmdLine);					
					memset(cmdLine, 0, sizeof(cmdLine));
				}
			}		
			
			if((CMD_DSP_INVOKE == cmd) &&(invokeOnce !=1))
				invokeOnce = 1;
			
			fclose(fp);

			sem_wait(&EVCOM_SINGAL);
 		}
	}
	else
#endif
	{
		while(gRunTask)
		{
			cmd = CMD_INVALID;
			CONSOLE_PROMPT
		
			memset(cmdLine, 0, sizeof(cmdLine));
			fgets(cmdLine, MAX_CMD_LEN, stdin);
			
			cmdLine[strlen(cmdLine) - 1] = '\0';
			adjustBackSpace(cmdLine);

			if (strlen(cmdLine) > 0) {
				if (0 < getTokens(cmdLine, " ", param)) {
					cmd = getCmdCode(gCmdSet, param[0]);
				}

				if (CMD_INVALID == cmd) {
					_promptCmd();
					continue;
				}

				gCmdSet[cmd].func(cmdLine);
			}
			
		}

		printf("Closing ADAM ... : %s \n\n", etosExec(adamExit()));
	
	}
#if 0//def TCSUPPORT_CY_VOIP_SIP
	if((argc == 2) && ((strcmp(argv[1],"on")==0)||(strcmp(argv[1],"tester")==0)))
	{
		tcdbg_printf("exit evcom \n");
		invokeOnce = 0;
		sem_destroy(&EVCOM_SINGAL);
	}
#endif
	return 0;
}

static void _promptCmd(void)
{
	int i;

	for (i = 0; i < NUM_OF_COMMAND;) {
		printf("%-20s",gCmdSet[i].name);
		if (!(++i & 3)) {
			printf("\n");
		}
	}
	printf("\n");
}

static void _promptCmdHelp(command_e cmdid) {
	printf("Command description: \n");
	printf("%s\n\n", gCmdSet[cmdid].desc);
	printf("Syntax: %s\n\n", gCmdSet[cmdid].usage);
	printf("Usage Example: %s\n\n", gCmdSet[cmdid].example);
}	

void GetConfigDynamicPtOfCodec(const char *inStr,strmConfig_t *config)
{
    char pVal[MAX_CMD_LEN] = {0};

    /*for redundant payloadtype*/
    getAttrVal(inStr, "redundant_pt", "=", pVal);
	if (strlen(pVal) > 0 && isValidNum(pVal, 96, 127)) 
    {
		config->payloadType[CODEC_REDUNDANT]= atoi(pVal);
	}
    
    /*for g726_32*/
    getAttrVal(inStr, "g726_pt", "=", pVal);
	if (strlen(pVal) > 0 && isValidNum(pVal, 96, 127)) 
    {
		config->payloadType[CODEC_G726_32]= atoi(pVal);
	}
    
    /*for amr*/
    #ifdef AMR_CODEC
    getAttrVal(inStr, "amr_pt", "=", pVal);
	if (strlen(pVal) > 0 && isValidNum(pVal, 96, 127)) 
    {
		config->payloadType[CODEC_AMR]= atoi(pVal);
	}
    #endif
       
    #if defined (TCSUPPORT_C1_MS)||defined(CJ_DSP_SUPPORT)
    /*for dtmf_ul */
	getAttrVal(inStr, "dtmful_pt", "=", pVal);
	if (isValidNum(pVal, 96, 127)) 
    {
		config->payloadType[CODEC_DTMFR] = atoi(pVal);
	}  
    /*for dtmf_dl*/
	getAttrVal(inStr, "dtmfdl_pt", "=", pVal);
	if (isValidNum(pVal, 96, 127)) 
    {
		config->payloadType[CODEC_DTMFR_DL]  = atoi(pVal);
	}
    #endif

    return;
    
}

/******************************************************************************
 Function: Get frequency and power value  accouding to input command string.
 
 Date:2014/12/12
******************************************************************************/
void GetConfigFrequencyAndPower(int toneId,int toneType,const char *inStr)
{
	int i;
	char pVal[MAX_CMD_LEN] = {0};
	char *param[MAX_TOKEN];
    int cadserial;
    char strFreqCmdName[MAX_CMD_LEN] ={0};
    char ptrCmdName[MAX_CMD_LEN]={0};//used as save the  cmd name set 
#if defined(FIVE_CADENCE_SUPPORT) || defined(CJ_DSP_SUPPORT) 
    /*for ASB,support configing  different frequency and power for different cadence*/
    for(cadserial=0;cadserial<MAX_CADENCE;cadserial++)
    {
        if (TONE_REGULAR == toneType) 
        {
               
            for (i = 0; i < MAX_TONE_FREQ; i++) 
            {
                if  (0 == i) {
                    getAttrVal(inStr, etosRegularFrequencyCmdName(cadserial,i,ptrCmdName) , "=", pVal);
                }
                else if  (1 == i) {
                    getAttrVal(inStr, etosRegularFrequencyCmdName(cadserial,i,ptrCmdName), "=", pVal);
                }
                else if  (2 == i) {
                    getAttrVal(inStr, etosRegularFrequencyCmdName(cadserial,i,ptrCmdName), "=", pVal);
                }
                else if  (3 == i) {
                    getAttrVal(inStr, etosRegularFrequencyCmdName(cadserial,i,ptrCmdName), "=", pVal);
                }
                
                if (strlen(pVal) > 0) {
                    if (2 == getTokens(pVal, ",", param)) {
                        if (isValidNum(param[0], 0, 4000)) {
                            gTones[toneId].regular[cadserial].toneFreq[i] = atoi(param[0]);
                        }
                        if (isValidNum(param[1], -40, 0)) {
                            gTones[toneId].regular[cadserial].tonePwr[i] = atoi(param[1]) * 2;
                        }
                    }                   
                }
  
                printf("cadence[%d]:Freqency(%d) = %dHz @ %ddb\n", cadserial,i, gTones[toneId].regular[cadserial].toneFreq[i],gTones[toneId].regular[cadserial].tonePwr[i]/2);
            }                      
        }
        else if (TONE_MODULATE == toneType) 
        { 
            getAttrVal(inStr, etosModulateFrequencyCmdName(cadserial,ptrCmdName), "=", pVal);
            if (strlen(pVal) > 0) {
                if (4 == getTokens(pVal, ",", param)) {         
                    if (isValidNum(param[0], 0, 4000)) {
                        gTones[toneId].modulate[cadserial].baseFreq = atoi(param[0]);
                    }
                    if (isValidNum(param[1], 0, 4000)) {
                        gTones[toneId].modulate[cadserial].modFreq = atoi(param[1]);
                    }
                    if (isValidNum(param[2], -40, 0)) {
                        gTones[toneId].modulate[cadserial].modPwr = atoi(param[2])*2;
                    }
                    if (isValidNum(param[3], 0, MAX16)) {
                        gTones[toneId].modulate[cadserial].modDepth = atoi(param[3]);
                    }
                }                   
            }
            printf("Base frequency = %dHz\n",  gTones[toneId].modulate[cadserial].baseFreq);
            printf("Modulation frequency = %dHz\n",  gTones[toneId].modulate[cadserial].modFreq);
            printf("Modulation power = %ddb\n",  gTones[toneId].modulate[cadserial].modPwr/2);
            printf("Modulation depth = %d\n",  gTones[toneId].modulate[cadserial].modDepth);
        }
    }

#else
    /*for other ,just support configing the same frequency and power for different cadence*/
    if (TONE_REGULAR == toneType) 
    {
        for (i = 0; i < MAX_TONE_FREQ; i++) {
            if  (0 == i) {
                getAttrVal(inStr, "f0", "=", pVal);
            }
            else if  (1 == i) {
                getAttrVal(inStr, "f1", "=", pVal);
            }
            else if  (2 == i) {
                getAttrVal(inStr, "f2", "=", pVal);
            }
            else if  (3 == i) {
                getAttrVal(inStr, "f3", "=", pVal);
            }

            if (strlen(pVal) > 0) {
                if (2 == getTokens(pVal, ",", param)) {
                    if (isValidNum(param[0], 0, 4000)) {
                        gTones[toneId].regular.toneFreq[i] = atoi(param[0]);
                    }
                    if (isValidNum(param[1], -40, 0)) {
                        gTones[toneId].regular.tonePwr[i] = atoi(param[1]) * 2;
                    }
                }                   
            }
            printf("Freqency(%d) = %dHz @ %ddb\n", i, gTones[toneId].regular.toneFreq[i],gTones[toneId].regular.tonePwr[i]/2);
        }
    }
    else if (TONE_MODULATE == toneType) 
    { 
        getAttrVal(inStr, "mf", "=", pVal);
        if (strlen(pVal) > 0) {
            if (4 == getTokens(pVal, ",", param)) {         
                if (isValidNum(param[0], 0, 4000)) {
                    gTones[toneId].modulate.baseFreq = atoi(param[0]);
                }
                if (isValidNum(param[1], 0, 4000)) {
                    gTones[toneId].modulate.modFreq = atoi(param[1]);
                }
                if (isValidNum(param[2], -40, 0)) {
                    gTones[toneId].modulate.modPwr = atoi(param[2])*2;
                }
                if (isValidNum(param[3], 0, MAX16)) {
                    gTones[toneId].modulate.modDepth = atoi(param[3]);
                }
            }                   
        }
        printf("Base frequency = %dHz\n",  gTones[toneId].modulate.baseFreq);
        printf("Modulation frequency = %dHz\n",  gTones[toneId].modulate.modFreq);
        printf("Modulation power = %ddb\n",  gTones[toneId].modulate.modPwr/2);
        printf("Modulation depth = %d\n",  gTones[toneId].modulate.modDepth);
    }
#endif
    return;

}

/******************************************************************************
 Function: Get Cadence attribute accouding to input command string,include maketime,breaktime,repeat times .
 
 Date:2014/12/12
******************************************************************************/
void GetConfigCadenceAttribute(int toneId,const char *inStr)
{
	int i;
	char pVal[MAX_CMD_LEN] = {0};
	char *param[MAX_TOKEN];

    for (i = 0; i < MAX_CADENCE; i++) 
    {
    	if  (0 == i) {
    			getAttrVal(inStr, "cad0", "=", pVal);
    	}
    	else if  (1 == i) {
    			getAttrVal(inStr, "cad1", "=", pVal);
    	}
    	else if  (2 == i) {
    			getAttrVal(inStr, "cad2", "=", pVal);
    	}
        
    #if defined(FIVE_CADENCE_SUPPORT) || defined(CJ_DSP_SUPPORT) 
    	else if  (3 == i) {
    		getAttrVal(inStr, "cad3", "=", pVal);
    	}
    	else if  (4 == i) {
    		getAttrVal(inStr, "cad4", "=", pVal);
    	}
    #endif

    	if (strlen(pVal) > 0) {
    		if (3 == getTokens(pVal, ",", param)) {
    			
    			if (isValidNum(param[0], 0, MAX16)) {
    				gTones[toneId].makeTime[i] = atoi(param[0]);
    			}
    			if (isValidNum(param[1], 0, MAX16)) {
    				gTones[toneId].breakTime[i] = atoi(param[1]);
    			}
    			if (isValidNum(param[2], 0, MAX8)) {
    				gTones[toneId].repeat[i] = atoi(param[2]);
    			}
    		}					
    	}
    }

    for (i = 0; i < MAX_CADENCE; i++) {
    		printf("Candence(%d) = %d(ms)/On, %d(ms)/Off, x %d (times)\n", i, \
    		gTones[toneId].makeTime[i], gTones[toneId].breakTime[i], \
    		gTones[toneId].repeat[i]);
    }
    return;
}

/******************************************************************************
 Function: Get  Regular frequency command name accouding to serial number of cadence and frequency,

 Date:2014/12/12
******************************************************************************/
char* etosRegularFrequencyCmdName(int32 cadserial,int32 freqserial,char *ptrCmdName) 
{
    if(ptrCmdName==NULL)
    {
        return "Invalid_freqName";        
    }
    if(cadserial<CADENCE_ONE || cadserial>=CADENCE_MAX || freqserial<FREQUENCY_ONE || freqserial>FREQUENCY_MAX)
    {
        return "Invalid_freqName";
    }    

    /*get cmd name by splicing string,"cad"+cadserial+"_f"+freqserial,such as cad0_f0*/

    snprintf(ptrCmdName,MAX_CMD_LEN-1,"cad%d_f%d",cadserial,freqserial);
    
    return ptrCmdName;
    
}

/******************************************************************************
 Function: Get  Modulate frequency command name accouding to serial number of cadence ,

 Date:2014/12/12
******************************************************************************/
char* etosModulateFrequencyCmdName(int32 cadserial,char *ptrCmdName) 
{
    if(ptrCmdName==NULL)
    {
        return "Invalid_freqName";        
    }
   
    if(cadserial<CADENCE_ONE || cadserial>=CADENCE_MAX )
    {
        return "Invalid_freqName";
    }    

    /*get cmd name by splicing string,"cad"+cadserial+"_mf,such as cad0_mf*/
    snprintf(ptrCmdName,MAX_CMD_LEN-1,"cad%d_mf",cadserial);

    return ptrCmdName;
}


#ifdef EVENT_NOTIFY
static void _threadEventPolling(void *argv)
{
	event_t *mEvent = (event_t *)argv;
			
	if ((EVENT_CODE_JB_UPDATE != mEvent->evtCode) || (CONTROL_ENABLE == printJB)) {
		printf("\n[T:%010u] %s: %s", (unsigned int)mEvent->dspTick, etosEdge(mEvent->edge), etosEvent(mEvent->evtCode));
	}

	switch(mEvent->evtCode) {
		case EVENT_CODE_CID:
			printf("\nChannel (%d) detected caller ID:", mEvent->chanId);
	    	printf("\nNumber: %s\n", mEvent->context.cidData.number);
	    	break;
		case EVENT_CODE_LINE:
			if (mEvent->context.line.ringCount > 0) {
				printf("\nInterface (%d) detected %s x %d.\n", mEvent->infcId, \
					etosLine(mEvent->context.line.status), mEvent->context.line.ringCount);
			}
			else {
				printf("\nInterface (%d) detected %s.\n", mEvent->infcId, \
					etosLine(mEvent->context.line.status));
			}				
			break;
		case EVENT_CODE_HOOK: 
			switch(mEvent->context.hook.status) {
				case HOOK_SEIZE:
					printf("\nInterface (%d) off-hooked.\n", mEvent->infcId);
					break;
				case HOOK_RELEASE:
					#if 1//def TCSUPPORT_CY_VOIP_SIP
					checkDigit(mEvent->infcId);
					#endif
					quitEvcom();
					printf("\nInterface (%d) on-hooked.\n", mEvent->infcId);
					break;
				case HOOK_FLASH:
					printf("\nInterface (%d) hook-flashed.\n", mEvent->infcId);
					break;
				case HOOK_PULSE1:
				case HOOK_PULSE2:
				case HOOK_PULSE3:
				case HOOK_PULSE4:
				case HOOK_PULSE5:
				case HOOK_PULSE6:
				case HOOK_PULSE7:
				case HOOK_PULSE8:
				case HOOK_PULSE9:
				case HOOK_PULSE10:
				case HOOK_PULSE11:
				case HOOK_PULSE12:
				case HOOK_PULSE13:
				case HOOK_PULSE14:
				case HOOK_PULSE15:
				case HOOK_PULSE16:
				case HOOK_PULSE17:
				case HOOK_PULSE18:
				case HOOK_PULSE19:
				case HOOK_PULSE20:	
					printf("\nInterface (%d) hook-pause (%d).\n", mEvent->infcId ,mEvent->context.hook.status-2);
					break;
				default:
					break;
			}
			break;
		case EVENT_CODE_TONE:
				printf("\nChannel (%d) tone[%s] %s detected.\n", \
					mEvent->chanId,	etosTone(mEvent->context.tone.code),  etosToneDir(mEvent->context.tone.dir));
#if 1//def TCSUPPORT_CY_VOIP_SIP
				if(mEvent->edge==EDGE_END){
			//			printf("xflu:: %s 1==>recvDigit(%d,%d)\n",__func__,mEvent->chanId,mEvent->context.tone.code);
						recvDigit(mEvent->chanId,mEvent->context.tone.code);
					}
#endif
				break;
		case EVENT_CODE_DL_DTMF:
				printf("\nChannel (%d) tone[%s] %s detected.\n", \
					mEvent->chanId,	etosTone(mEvent->context.tone.code),  etosToneDir(mEvent->context.tone.dir));
				break;				
		case EVENT_CODE_JB_UPDATE:
			if (CONTROL_ENABLE == printJB){
				printf("\nChannel (%d) Stream (%d):\n Packet Recv: %lld\n Packet Drop: %lld\n Packet Loss Compensated: %ld\n JB Size: %ld avg Jitter: %ld.\n", \
					mEvent->chanId, mEvent->context.jb.streamId, mEvent->context.jb.total,mEvent->context.jb.drop, \
					mEvent->context.jb.plc, mEvent->context.jb.jbSize, mEvent->context.jb.avgJitter);	
			}
			break;
		case EVENT_CODE_T38:
			printf("\nChannel (%d) Stream (%d): %s.\n", \
				mEvent->chanId, mEvent->context.t38.streamId,	etosT38State(mEvent->context.t38.status));
			break;	
		case EVENT_CODE_CALLERID_SEND:
			printf("\nChannel (%d) callid is sended.\n",mEvent->chanId);
			break;
#ifdef CJ_DSP_SUPPORT
		case EVENT_CODE_FIRST_RTP_DELIVERD:  /* added by mtk07059_20150514. */
			printf("\nChannel (%d) first rtp income.\n",mEvent->chanId);
			break;
#endif
		default:
			break;
	}	

  return;
}
#else
static void _threadEventPolling(void)
{
	event_t mEvent;
	
	while(1 == gDspState) {
//		usleep(10000);
		memset(&mEvent, 0, sizeof(event_t));
		if (EXEC_SUCCESS == adamPollEvent(BLOCKING, &mEvent)) {

			if ((EVENT_CODE_JB_UPDATE != mEvent.evtCode) || (CONTROL_ENABLE == printJB)) {
				printf("\n[T:%010u] %s: %s", (unsigned int)mEvent.dspTick, etosEdge(mEvent.edge), etosEvent(mEvent.evtCode));
			}
			
			switch(mEvent.evtCode) {
	    		case EVENT_CODE_CID:
	    			printf("\nChannel (%d) detected caller ID:", mEvent.chanId);
	    			printf("\nNumber: %s\n", mEvent.context.cidData.number);
	    			break;
				case EVENT_CODE_LINE:
					if (mEvent.context.line.ringCount > 0) {
						printf("\nInterface (%d) detected %s x %d.\n", mEvent.infcId, \
							etosLine(mEvent.context.line.status), mEvent.context.line.ringCount);
					}
					else {
						printf("\nInterface (%d) detected %s.\n", mEvent.infcId, \
							etosLine(mEvent.context.line.status));
					}				
					break;
				case EVENT_CODE_HOOK: 
					switch(mEvent.context.hook.status) {
						case HOOK_SEIZE:
							printf("\nInterface (%d) off-hooked.\n", mEvent.infcId);
							break;
						case HOOK_RELEASE:
#if 1//def TCSUPPORT_CY_VOIP_SIP
							quitEvcom();
#endif
							printf("\nInterface1 (%d) on-hooked.\n", mEvent.infcId);
							break;
						case HOOK_FLASH:
							printf("\nInterface (%d) hook-flashed.\n", mEvent.infcId);
							break;
						case HOOK_PULSE1:
						case HOOK_PULSE2:
						case HOOK_PULSE3:
						case HOOK_PULSE4:
						case HOOK_PULSE5:
						case HOOK_PULSE6:
						case HOOK_PULSE7:
						case HOOK_PULSE8:
						case HOOK_PULSE9:
						case HOOK_PULSE10:
						case HOOK_PULSE11:
						case HOOK_PULSE12:
						case HOOK_PULSE13:
						case HOOK_PULSE14:
						case HOOK_PULSE15:
						case HOOK_PULSE16:
						case HOOK_PULSE17:
						case HOOK_PULSE18:
						case HOOK_PULSE19:
						case HOOK_PULSE20:	
							printf("\nInterface (%d) hook-pause (%d).\n", mEvent.infcId ,mEvent.context.hook.status-2);
					break;	
						default:
							break;
					}
					break;
				case EVENT_CODE_TONE:
						printf("\nChannel (%d) tone[%s] %s detected.\n", \
							mEvent.chanId,	etosTone(mEvent.context.tone.code),  etosToneDir(mEvent.context.tone.dir));
					break;
				case EVENT_CODE_DL_DTMF:
						printf("\nChannel (%d) tone[%s] %s detected.\n", \
							mEvent.chanId,	etosTone(mEvent.context.tone.code),  etosToneDir(mEvent.context.tone.dir));
					break;	
				case EVENT_CODE_JB_UPDATE:
					if (CONTROL_ENABLE == printJB){
					printf("\nChannel (%d) Stream (%d):\n Packet Recv: %lld\n Packet Drop: %lld\n Packet Loss Compensated: %ld\n JB Size: %ld avg Jitter: %ld.\n", \
					mEvent.chanId, mEvent.context.jb.streamId, mEvent.context.jb.total,mEvent.context.jb.drop, \
					mEvent.context.jb.plc, mEvent.context.jb.jbSize, mEvent.context.jb.avgJitter);	
			}
					break;
				case EVENT_CODE_T38:
						printf("\nChannel (%d) Stream (%d): %s.\n", \
							mEvent.chanId, mEvent.context.t38.streamId,	etosT38State(mEvent.context.t38.status));						
					break;	
#ifdef CJ_DSP_SUPPORT
				case EVENT_CODE_FIRST_RTP_DELIVERD:  /* added by mtk07059_20150514. */
					printf("\nChannel (%d) first rtp income222.\n",mEvent->chanId);
					break;
#endif
				default:
					break;
			}	
		}
	}

  return;
}
#endif
