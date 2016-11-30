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

#include "SET_WPS_ON.h"

#if defined(TCSUPPORT_CT_JOYME)
json_t* SetWpsOn(json_t* req_root, json_t* req_para)
{
	json_t  *reply_root = NULL, *reply_para = NULL;
	char *ssid = NULL,*timeout = NULL;
	int ret = -1,ssidindex = 0;
	char nodename[64] = {0},tempbuf[64] = {0};
	
	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__,__LINE__);
	
	ssid = ParameterSearchValue(req_para, "SSID");
	timeout = ParameterSearchValue(req_para, "Timeout");

	if(ssid != NULL && timeout != NULL)
	{
		tcapi_get("WLan_Common","APOn",tempbuf);
		if(strcmp(tempbuf,"1") != 0)
			tcapi_set("WLan_Common","APOn","1");

#if defined(TCSUPPORT_WLAN_MULTI_WPS)
		ssidindex = atoi(ssid)-1;
		sprintf(nodename,"WLan_Entry%d",ssidindex);
#endif

		memset(tempbuf,0,sizeof(tempbuf));
		tcapi_get(nodename,"EnableSSID",tempbuf);
		if(strcmp(tempbuf,"1") != 0)
			tcapi_set(nodename,"EnableSSID","1");
			
		tcapi_set(nodename,"WPSConfMode","7");
		tcapi_set(nodename,"WPSMode","1");
		tcapi_set(nodename,"WPSConfStatus","2");
		
		memset(tempbuf,0,sizeof(tempbuf));		
		sprintf(tempbuf,"%d",ssidindex);
		
		mobile_printf(MOBILE_INFO,"%s, line = %d,tempbuf=%s\n",__FUNCTION__,__LINE__,tempbuf);
		tcapi_set("WebCurSet_Entry","wlan_id",tempbuf);
		tcapi_set("Info_WLan","WPSActiveStatus","1");		

		tcapi_commit(nodename);
		tcapi_save();
		ret = 0;
	}
	
	if(ret == 0){
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
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "Fail To set wps on")){
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

	if(reply_para) ParameterFreeJSON(&reply_para);
	return reply_root;
	
err:
	if(reply_root) ParameterFreeJSON(&reply_root);
	if(reply_para) ParameterFreeJSON(&reply_para);
	return NULL;
}
#endif
