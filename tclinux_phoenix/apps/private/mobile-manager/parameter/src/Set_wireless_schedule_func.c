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
#include "Set_wireless_schedule_func.h"
#include "client.h"

json_t* Set_wireless_schedule(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	char *tempstartTime = NULL, *tempendTime = NULL, *tempControlCycle = NULL, *tempEnable = NULL;
	char wireless_enable[4] = {0}, WlanStartTime[24] = {0}, WlanEndTime[24] = {0};
	int ret = -1;

	mobile_printf(MOBILE_INFO,"%s, line = %d\n",__FUNCTION__, __LINE__);
	if(!TCSUPPORT_WLAN_VAL){
		mobile_printf(MOBILE_INFO,"%s:wlan is not support!...\n",__FUNCTION__);
		return ParameterBuildJSON(req_root,req_para,CMD_UNRECOGNIZE);
	}

	tempstartTime = ParameterSearchValue(req_para, "StartTime");
	tempendTime = ParameterSearchValue(req_para, "EndTime");
	tempControlCycle = ParameterSearchValue(req_para, "ControlCycle");
	tempEnable = ParameterSearchValue(req_para, "Enable");

	if((tempstartTime != NULL) && (tempendTime != NULL) && (tempControlCycle != NULL) && (tempEnable != NULL))
	{
		ret = 0;
		memcpy(WlanStartTime, tempstartTime, sizeof(WlanStartTime));
		memcpy(WlanEndTime, tempendTime, sizeof(WlanEndTime));
		memcpy(wireless_enable, tempEnable, sizeof(wireless_enable));
		
		mobile_printf(MOBILE_INFO,"line = %d, WlanStartTime = %s, WlanEndTime = %s\n", __LINE__, WlanStartTime, WlanEndTime);
		
		tcapi_set(MOBILE_ENTRY, "WLanStartTime", WlanStartTime);
		tcapi_set(MOBILE_ENTRY, "WLanEndTime", WlanEndTime);
		tcapi_set(MOBILE_ENTRY, "WLanScheduleEable", wireless_enable);
		tcapi_save();
		Sendclientpipe();
	}
	else
	{
		ret = -1;
	}

	if(ret != -1){
		//build parameter
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){//ok
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	else{
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "Set wireless schedule fail")){
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
