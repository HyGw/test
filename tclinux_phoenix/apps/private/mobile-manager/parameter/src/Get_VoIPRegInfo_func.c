/************************************************************************
 *
 *	Copyright (C) 2003 Trendchip Technologies, Corp.
 *	All Rights Reserved.
 *
 * Trendchip Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of Trendchip Technologies, Co. and shall
 * not be reproduced, copied, disclosed, or used in whole or in part
 * for any reason without the prior express written permission of
 * Trendchip Technologeis, Co.
 *
 *************************************************************************/

#include "Get_VoIPRegInfo_func.h"

#define VOICE_IAD_ERROR      1
#define VOICE_ROUTE_ERROR    2
#define VOICE_RESPONSE_ERROR 3
#define VOICE_USER_PW_ERROR  4
#define VOICE_UNKNOWN_ERROR  5

json_t* Get_VoIPRegInfo(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	char VoIPRegInfo1[32] = {0},VoIPRegInfo2[32] = {0};
	char tempVoIPRegInfo1[8] = {0}, tempVoIPRegInfo2[8] = {0};
	char tempVoIPRegInfo11[8] = {0}, tempVoIPRegInfo22[8] = {0};
	char  tempvoiceIfName[128] = {0};
	int ret1 = 0, ret2 = 0;
	FILE *fp=NULL;
	char tempValue[16] = {0};
	unsigned int u32Temp = 0;
	uint8 u8TempVoIP1 = 0, u8TempVoIP2 = 0;
	char sipProtocol[16] = {0};
	int isH248 = 0;

	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__,__LINE__);

	memset(VoIPRegInfo1,0,sizeof(VoIPRegInfo1));
	memset(VoIPRegInfo2,0,sizeof(VoIPRegInfo2));

	if ( 0 == tcapi_get("VoIPBasic_Common", "SIPProtocol", sipProtocol)
		&& 0 == strcmp("H.248", sipProtocol) )
		isH248 = 1;
#if 0
	fp = fopen("/proc/tc3162/pcmlastregerror", "r");
	if(fp != NULL){
	fgets(tempValue, 16, fp);
	fclose(fp);
	}
#endif
	tcapi_get("InfoVoIP_Entry0", "RegFailReason", tempValue);
	tcapi_get("VoIPSysParam_Common", "SC_SYS_CFG_WAN_IFNAME", tempvoiceIfName);
	/*
	tempU32: bit31~bit24  |  bit23~bit16  |  bit15~bit8  | bit7~bit0
				reserved         reserved        Line2            Line1
	*/		
	sscanf(tempValue,"%x",&u32Temp);

	if ( isH248 )
	{
		tcapi_get("VoIPH248_Entry0", "UserServiceState", tempVoIPRegInfo1);
		tcapi_get("VoIPH248_Entry1", "UserServiceState", tempVoIPRegInfo2);
	}
	else
	{
		tcapi_get("InfoVoIP_Entry0", "Status", tempVoIPRegInfo1);
		tcapi_get("InfoVoIP_Entry1", "Status", tempVoIPRegInfo2);
		tcapi_get("InfoVoIP_Entry0", "WAN_STATUS_CHANGE_FLAG", tempVoIPRegInfo11);
		tcapi_get("InfoVoIP_Entry1", "WAN_STATUS_CHANGE_FLAG", tempVoIPRegInfo22);
	}

	if ( isH248 )
	{
		switch ( atoi(tempVoIPRegInfo1) )
		{
			case 1:
				strcpy(VoIPRegInfo1, "VOIP_STATUS_SUCCESS");
				break;
			case 5: /* register failed*/
			case 3: /* registering */
			case 8: /* disabled */
			default:
				strcpy(VoIPRegInfo1, "VOIP_STATUS_UNKNOWN");
				break;
		}

		if(!TCSUPPORT_CT_1FXS_VAL){
		switch ( atoi(tempVoIPRegInfo2) )
		{
			case 1:
				strcpy(VoIPRegInfo2, "VOIP_STATUS_SUCCESS");
				break;
			case 5: /* register failed*/
			case 3: /* registering */
			case 8: /* disabled */
			default:
				strcpy(VoIPRegInfo2, "VOIP_STATUS_UNKNOWN");
				break;
		}
	}
	}
	else
	{
		if(strcmp(tempVoIPRegInfo1, "Up") == 0){
			strcpy(VoIPRegInfo1,"VOIP_STATUS_SUCCESS");
		}
		else{
			if((strcmp(tempvoiceIfName ,"") == 0)||(strcmp(tempvoiceIfName ,"br0") == 0)||!strlen(tempVoIPRegInfo11)){
				strcpy(VoIPRegInfo1,"VOIP_NOVALID_CONNECTION");				
				}
			else{				
			u8TempVoIP1 = u32Temp & 0xff;

			switch(u8TempVoIP1)
			{
				case VOICE_IAD_ERROR:
					strcpy(VoIPRegInfo1,"VOIP_STATUS_IADERROR");
					break;
				case VOICE_ROUTE_ERROR:
					strcpy(VoIPRegInfo1,"VOIP_STATUS_NOROUTE");
					break;
				case VOICE_RESPONSE_ERROR:
					strcpy(VoIPRegInfo1,"VOIP_STATUS_NORESPONSE");
					break;
				case VOICE_USER_PW_ERROR:
					strcpy(VoIPRegInfo1,"VOIP_STATUS_ACCOUNTERR");
					break;
				case VOICE_UNKNOWN_ERROR:
					strcpy(VoIPRegInfo1,"VOIP_STATUS_UNKNOWN");
					break;
				default:
					strcpy(VoIPRegInfo1,"VOIP_NOVALID_CONNECTION");
					break;
			}
		}
		}

        memset(tempValue, 0, sizeof(tempValue));
		tcapi_get("InfoVoIP_Entry1", "RegFailReason", tempValue);
		u32Temp = 0;
		sscanf(tempValue, "%x", &u32Temp);

		if(!TCSUPPORT_CT_1FXS_VAL){
		if(strcmp(tempVoIPRegInfo2, "Up") == 0){
			strcpy(VoIPRegInfo2,"VOIP_STATUS_SUCCESS");
		}
		else{
			if((strcmp(tempvoiceIfName ,"") == 0)||(strcmp(tempvoiceIfName ,"br0") == 0)||!strlen(tempVoIPRegInfo22)){
				strcpy(VoIPRegInfo2,"VOIP_NOVALID_CONNECTION");
				}
			else{
			u8TempVoIP2 = u32Temp & 0xff;

			switch(u8TempVoIP2)
			{
				case VOICE_IAD_ERROR:
					strcpy(VoIPRegInfo2,"VOIP_STATUS_IADERROR");
					break;
				case VOICE_ROUTE_ERROR:
					strcpy(VoIPRegInfo2,"VOIP_STATUS_NOROUTE");
					break;
				case VOICE_RESPONSE_ERROR:	
					strcpy(VoIPRegInfo2,"VOIP_STATUS_NORESPONSE");
					break;
				case VOICE_USER_PW_ERROR:
					strcpy(VoIPRegInfo2,"VOIP_STATUS_ACCOUNTERR");
					break;
				case VOICE_UNKNOWN_ERROR:
					strcpy(VoIPRegInfo2,"VOIP_STATUS_UNKNOWN");
					break;
				default:
					strcpy(VoIPRegInfo2,"VOIP_NOVALID_CONNECTION");
					break;
			}
		}
	}
	}
	}

	if((ret1 == 0) || (ret2 == 0)){
		//build parameter
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){//ok
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
			}

			if(JSON_OK != ParameterAddNewJSONChild(reply_para, "Line1Status", VoIPRegInfo1)){
				mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
				goto err;
			}

			if(!TCSUPPORT_CT_1FXS_VAL){
			if(JSON_OK != ParameterAddNewJSONChild(reply_para, "Line2Status", VoIPRegInfo2)){
				mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
				goto err;
			}
		}
		}
	else{
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "Get Voip Register Information fail")){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	
	//build head
	if(!(reply_root = ParameterBuildJSONReplyHead(req_root,PARA_RESULT_SUCCESS))){
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyHead Err!...\n",__FUNCTION__);
		goto err;
	}
	if(ParameterBuildJSONAddPara2Head(reply_root, reply_para)){
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONAddPara2Head Err!...\n",__FUNCTION__);
		goto err;
	}	
		
#if PARAMETER_REPLAY_DBG
	char* value_str = NULL; 
	json_tree_to_string(reply_root,&value_str);
	mobile_printf(MOBILE_INFO, "%s: reply json pkt=%s!...\n", __FUNCTION__,value_str);
	free(value_str);	
#endif	

	return reply_root;
	
err:
	if(reply_root) ParameterFreeJSON(&reply_root);
	if(reply_para) ParameterFreeJSON(&reply_para);
	return NULL;
}
