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
#include "Get_WLanInfo_func.h"

json_t* Get_WLanInfo(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	char WLanEnable[8] = {0}, Cur_channel[8]= {0}, cur_ssid_name[33] = {0}, maxBitRate[5] = {0}, cur_authmode[32] = {0};
	char temprxbytes[16] = {0}, tempRxFrames[16] = {0}, tempRxErrFrames[16] = {0}, tempRxDropFrames[16] = {0};
	char temptxbytes[16] = {0}, tempTxFrames[16] = {0}, tempTxErrFrames[16] = {0}, tempTxDropFrames[16] = {0};
	char tempRecv[64] = {0}, tempSend[64] = {0};
	int ret = 0;

	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__,__LINE__);
	
	if(!TCSUPPORT_WLAN_VAL){
		mobile_printf(MOBILE_INFO,"%s:wlan is not support!...\n",__FUNCTION__);
		return ParameterBuildJSON(req_root,req_para,CMD_UNRECOGNIZE);
	}

	
	tcapi_get("WLan_Common", "Channel", Cur_channel);
	tcapi_get("WLan_Entry0", "SSID", cur_ssid_name);
	tcapi_get("WLan_Common", "TxPowerLevel", maxBitRate);
	if(strcmp(maxBitRate, "1") == 0){
		strcpy(maxBitRate,"100");
	}
	if(strstr(maxBitRate, "2") != NULL){
		strcpy(maxBitRate,"80");
	}
	else if(strcmp(maxBitRate, "3") == 0){
		strcpy(maxBitRate,"60");
	}
	else if(strcmp(maxBitRate, "4") == 0){
		strcpy(maxBitRate,"40");
	}
	else if ( strcmp(maxBitRate, "5") == 0 ) {
		strcpy(maxBitRate, "20");
	}
	
	tcapi_get("WLan_Entry0", "AuthMode", cur_authmode);
	if(strcmp(cur_authmode, "OPEN") == 0){
		strcpy(cur_authmode,"0");
	}
	else{
		strcpy(cur_authmode, "1");
	}
	
	tcapi_get("WLan_Common", "APOn", WLanEnable);
	if(strcmp(WLanEnable, "1") == 0)
	{		
		tcapi_get("Info_WLan","rxbytes",temprxbytes);
		tcapi_get("Info_WLan","wlanRxFrames",tempRxFrames);
		tcapi_get("Info_WLan","wlanRxErrFrames",tempRxErrFrames);
		tcapi_get("Info_WLan","wlanRxDropFrames",tempRxDropFrames);
		tcapi_get("Info_WLan","txbytes",temptxbytes);
		tcapi_get("Info_WLan","wlanTxFrames",tempTxFrames);
		tcapi_get("Info_WLan","wlanTxErrFrames",tempTxErrFrames);
		tcapi_get("Info_WLan","wlanTxDropFrames",tempTxDropFrames);

		sprintf(tempRecv, "%s/%s/%s/%s", temprxbytes, tempRxFrames, tempRxErrFrames, tempRxDropFrames);
		sprintf(tempSend, "%s/%s/%s/%s", temptxbytes, tempTxFrames, tempTxErrFrames, tempTxDropFrames);
	}
	else
	{	
		strcpy(tempRecv,"0");	
		strcpy(tempSend,"0");	
	}

	//build parameter
	if(ret != 0){
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "There are no WLan Information")){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	else{
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){//ok
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_ENABLE, WLanEnable)){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_CHANNEL, Cur_channel)){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_SSID, cur_ssid_name)){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_POWERLEVEL, maxBitRate)){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_ENCRYPT, cur_authmode)){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_RECV, tempRecv)){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_SEND, tempSend)){
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

