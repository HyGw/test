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
#include "InformDiagnosticsRequest_func.h"

json_t* InformDiagnosticsRequest(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	char temp_result[8] = {0}, inform_result[32] = {0};
	int ret = -1, int_result = -1;

	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__, __LINE__);

	ret = tcapi_set("Cwmp_Entry", "Manual_Inform", "1");
	tcapi_commit("Cwmp_Entry");
	sleep(5);

	tcapi_get("Cwmp_Entry", "Inform_Status", temp_result);
	int_result = atoi(temp_result);

	switch(int_result)
	{
		case 0:
			strcpy(inform_result,"INFORM_IAD_RUNNING");
			break;
		case 1:
			strcpy(inform_result,"INFORM_NO_WAN");
			break;
		case 2:
			strcpy(inform_result,"INFORM_INVALID_WAN");
			break;
		case 3:
			strcpy(inform_result,"INFORM_NO_DNS_INFORM");
			break;
		case 4:
			strcpy(inform_result,"INFORM_NO_ACS_CONFIGURATION");
			break;
		case 5:
			strcpy(inform_result,"INFORM_ACSDNS_RESOLVE_FAILURE");
			break;
		case 6:
			strcpy(inform_result,"INFORM_NO_RESPONSE");
			break;
		case 7:
			strcpy(inform_result,"INFORM_REPORT_INTERRUPT");
			break;
		case 8:
			strcpy(inform_result,"INFORM_OK");
			break;
		 default:
			 strcpy(inform_result,"INFORM_CFE_NO_REPORT");
			break;
	}

	//build parameter
	if(ret != 0){
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "Inform Is Failure!")){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	else{
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){//ok
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_INFORMSTATUS, "")){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_INFORMRESULT, inform_result)){
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

