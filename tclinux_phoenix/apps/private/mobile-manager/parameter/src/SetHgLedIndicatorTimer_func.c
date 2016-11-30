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
#include "SetHgLedIndicatorTimer_func.h"
#include "client.h"

#if defined(TCSUPPORT_CT_JOYME)
json_t* SetHgLedIndicatorTimer(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	char *pStartTime = NULL, *pEndTime = NULL;
	char *pControlCycle = NULL, *pEnable = NULL;
	int ret = 0;

	mobile_printf(MOBILE_INFO,"%s:start!...\n",__FUNCTION__);

	pStartTime = ParameterSearchValue(req_para, "StartTime");
	pEndTime = ParameterSearchValue(req_para, "EndTime");
	pControlCycle = ParameterSearchValue(req_para, "ControlCycle");
	pEnable = ParameterSearchValue(req_para, "Enable");

	if((pStartTime==NULL) || (pEndTime==NULL) || (pControlCycle==NULL) || (pEnable==NULL) )
		ret = -1;
	
	mobile_printf(MOBILE_INFO,"%s:StartTime = %s, EndTime = %s, ControlCycle = %s, Enable = %s\n", __FUNCTION__, pStartTime, pEndTime, pControlCycle, pEnable);
	
	//build parameter
	if(ret == -1){/* Fail */
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "Parameter error")){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	else{/* Success */
		tcapi_set(MOBILE_ENTRY, "LedStartTime", pStartTime);
		tcapi_set(MOBILE_ENTRY, "LedEndTime", pEndTime);
		tcapi_set(MOBILE_ENTRY, "LedScheduleEnable", pEnable);
		tcapi_save();
		Sendclientpipe();
		
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){
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
#endif
