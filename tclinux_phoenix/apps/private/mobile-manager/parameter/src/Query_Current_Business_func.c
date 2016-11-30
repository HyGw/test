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

#include "Query_Current_Business_func.h"

json_t* Query_Current_Business_func(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	char tempstartTime[10] = {0}, tempendTime[10] = {0}, tempControlCycle[10] = {0}, tempEnable[4] = {0};
	char tmpValue[10] = {0};
	int size=0;
	
	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__,__LINE__);
	if(!TCSUPPORT_WLAN_VAL){
		mobile_printf(MOBILE_INFO,"%s:wlan is not support!...\n",__FUNCTION__);
		return ParameterBuildJSON(req_root,req_para,CMD_UNRECOGNIZE);
	}

	tcapi_get(MOBILE_ENTRY, "WLanStartTime", tempstartTime);
	tcapi_get(MOBILE_ENTRY, "WLanEndTime", tempendTime);
	tcapi_get(MOBILE_ENTRY, "WLanScheduleEable", tempEnable);
	sprintf(tempControlCycle,"%s","DAY");
	
	//build parameter
	if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){//ok
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_WIFISTARTTIME, tempstartTime)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_WIFIENDTIME, tempendTime)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_WIFICONTROLCYCLE, tempControlCycle)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_WIFIENABLE, tempEnable)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}

	tcapi_get(MOBILE_ENTRY, "LedStartTime", tempstartTime);
	tcapi_get(MOBILE_ENTRY, "LedStartTime", tempendTime);
	tcapi_get(MOBILE_ENTRY, "LedScheduleEnable", tempEnable);
	sprintf(tmpValue,"%d",LedStatus);
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_LEDSTATUS, tmpValue)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_LEDSTARTTIME, tempstartTime)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_LEDENDTIME, tempendTime)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_LEDCONTROLCYCLE, tempControlCycle)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_LEDENABLE, tempEnable)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	sprintf(tmpValue,"%d",NFCStatus);
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_NFC, tmpValue)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	sprintf(tmpValue,"%d",SleepStatus);
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_HGWSLEEP, tmpValue)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
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
	if(reply_para)
	{
		ParameterFreeJSON(&reply_para);
		reply_para = NULL;
	}
	return reply_root;
	
err:
	if(reply_root) ParameterFreeJSON(&reply_root);
	if(reply_para) ParameterFreeJSON(&reply_para);
	return NULL;
}
