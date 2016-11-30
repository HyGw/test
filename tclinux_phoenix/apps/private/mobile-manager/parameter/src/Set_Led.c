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


 #include "Set_Led.h"
 #include "info.h"
 
 json_t* Set_Led (json_t* req_root, json_t* req_para)
 {
    json_t *reply_root = NULL, *reply_para = NULL;
	char* tempLEDstatus=NULL;
	char LEDstatus[4]={0};
	int ret=-1;

	mobile_printf(MOBILE_NONE,"%s, line = %d\n",__FUNCTION__, __LINE__);

	
	tempLEDstatus = ParameterSearchFirstValue(req_para, "LEDStatus");
	
	if (!strcmp(tempLEDstatus,"ON")) {
		ret = 0;
		//system("sys led recover");
		LedStatusChange(1);
		tcapi_set("mobile_Entry", "ledstatus", "on");
		tcapi_save();
	}
	else if (!strcmp(tempLEDstatus,"OFF")) {
		ret = 0;
		//system("sys led off");
		LedStatusChange(0);
		tcapi_set("mobile_Entry", "ledstatus", "off");
		tcapi_save();
	}

	//build parameter
	if(ret != -1){
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
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "Set LED fail")){
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
		