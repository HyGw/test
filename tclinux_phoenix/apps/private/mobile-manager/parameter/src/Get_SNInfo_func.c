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
#include "Get_SNInfo_func.h"
#include "info.h"

json_t* Cmd_GetSNInfo(json_t* req_root, json_t* req_para)
{
	json_t* reply_root = NULL, *reply_para = NULL;
	
	mobile_printf(MOBILE_INFO,"%s:start!...\n",__FUNCTION__);
	
#if defined(TCSUPPORT_CT_JOYME)
		char bussinessStatus[16] = {0};
		char status[10], result[10] = {0};
#endif
	
	//build parameter
	if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_MAC, gMac)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}

#if defined(TCSUPPORT_CT_JOYME)
	tcapi_get("deviceAccount_Entry", "registerStatus", status);
	tcapi_get("deviceAccount_Entry", "registerResult", result);
	if (!strcmp(status, "0") && !strcmp(result, "1")) {
		strcpy(bussinessStatus, "SUCCESS");
	}
	else {
		strcpy(bussinessStatus, "FAIL");
	}

	tcdbg_printf("\r\nCmd_GetSNInfo(): status = %s, result = %s, bussinessStatus = %s", status, result, bussinessStatus);

	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_BUSSINESSSTATUS, bussinessStatus)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}	
#endif

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