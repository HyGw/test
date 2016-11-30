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
#include "Set_RegisterInfo_func.h"
extern int iRetryNumber20, iRetryNumber40, iRetryNumber30, iRegisterCmdFlag, iRegisterTime;

json_t* SetRegisterInfo(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	char *RegisterUserId = NULL, *RegisterPassword = "";
	json_t *RegisterPasswordnode = NULL;
	char regSta[64] = {0};
	int ret = -1;
	time_t timenow;
	struct tm *tblock;

	mobile_printf(MOBILE_INFO,"%s:start!...\n",__FUNCTION__);

	RegisterUserId = ParameterSearchValue(req_para, "USERID");
	RegisterPasswordnode = ParameterSearchFirstLabel(req_para, "PASSWORD");

	if(RegisterUserId != NULL)
	{
		if((RegisterPasswordnode != NULL) && (RegisterPasswordnode->child->text != NULL))
			RegisterPassword = RegisterPasswordnode->child->text;

		tcapi_set("EPON_LOIDAuth", "LOID0", RegisterUserId);
		tcapi_set("EPON_LOIDAuth", "Password0", RegisterPassword);
		tcapi_set("GPON_LOIDAuth", "LOID", RegisterUserId);
		tcapi_set("GPON_LOIDAuth", "Password", RegisterPassword);
		tcapi_save();
		tcapi_commit("EPON_LOIDAuth");
		tcapi_commit("GPON_LOIDAuth");

		tcapi_set("Cwmp_Entry", "devregInform", "1");
		tcapi_set("deviceAccount_Entry", "newStatus", "0");
		tcapi_set("deviceAccount_Entry", "newResult", "0");
		tcapi_set("deviceAccount_Entry", "userName", RegisterUserId);
		tcapi_set("deviceAccount_Entry", "userPasswordDEV", RegisterPassword);
		tcapi_get("deviceAccount_Entry", "registerStatus", regSta);
		if(strcmp(regSta, "0"))
			tcapi_set("deviceAccount_Entry", "registerResult", "99");
		tcapi_commit("deviceAccount_Entry");
		tcapi_save();

		//Init Parameter
		timenow = time(NULL);	
		tblock = localtime(&timenow);
		iRegisterTime = tblock->tm_hour * 3600 + tblock->tm_min *60 + tblock->tm_sec;

		iRetryNumber20 = 0;
		iRetryNumber40 = 0;
		iRetryNumber30 = 0;
		iRegisterCmdFlag = 1;

		ret = 0;
	}
	
	//build parameter
	if(ret == 0)
	{
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}	
	}
	else
	{
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "USERID_OR_PASSWORD_ERROR")){
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
