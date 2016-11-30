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

#include "Close_SSID_func.h"

json_t* Close_SSID_func(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	char *cSSIDIndex = NULL, nodeName[48] = {0};
	char ssidEnable[10] = {0};
	int cur_ssid_idx = 0;
	int ret = -1;

	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__,__LINE__);
	if(!TCSUPPORT_WLAN_VAL){
		mobile_printf(MOBILE_INFO,"%s:wlan is not support!...\n",__FUNCTION__);
		return ParameterBuildJSON(req_root,req_para,CMD_UNRECOGNIZE);
	}


	cSSIDIndex = ParameterSearchValue(req_para, "SSIDIndex");
	if(cSSIDIndex != NULL){
		cur_ssid_idx = atoi(cSSIDIndex);
		if( (cur_ssid_idx <= 8) && (cur_ssid_idx >= 0) ){
			ret = 0;
			if(0 == cur_ssid_idx){
				tcapi_set("WLan_Common", "APOn", "0");
			}
			else{
				sprintf(nodeName, "WLan_Entry%d", cur_ssid_idx-1);
				tcapi_set(nodeName, "EnableSSID", "0");
			}
			tcapi_commit("WLan_Entry");
			tcapi_save();
		}
	}
	//build parameter
	if(ret == -1){
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "SSIDIndex does not meet the requirements")){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	else{
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){//ok
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
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
