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
#include "if_sysinfo_func.h"
#include "../../../lib/libtcapi.h"
#include "info.h"

json_t* GetCommProtocolVer(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;	
	char Versionnum[16] = {0};	
	char devVersion[32] = {0};
	char ITMSVersion[32] = {0};

	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__,__LINE__);		

#if defined(TCSUPPORT_CT_JOYME)
	strcpy(Versionnum,"1.1");
	strcpy(devVersion,"Q/CT 2603-2015|a0.2");
	strcpy(ITMSVersion,"Q/CT 2271-2014|a0.1");
#else
	tcapi_get(MOBILE_ENTRY, "Version", Versionnum);
	if(strlen(Versionnum) == 0)
		strcpy(Versionnum,"1.0");
#endif

	//build parameter	
	if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){//ok
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_VERSION, Versionnum)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}

#if defined(TCSUPPORT_CT_JOYME)
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_VERSION1, devVersion)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}

	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_VERSION2, ITMSVersion)){
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

json_t* Cmd_CheckPassword(json_t* req_root, json_t* req_para)
{
	json_t* reply_root = NULL, *reply_para = NULL;
	char *temp_pwd = NULL;
	int status = 1;
	char SNbuf[33] = {'\0'};
	char buffer[128] = {'\0'};
	
	mobile_printf(MOBILE_INFO,"%s:start!...\n",__FUNCTION__);

	temp_pwd  = ParameterSearchFirstValue(req_para, "PassWD");
	if(temp_pwd ){
		mobile_printf(MOBILE_ERR,"%s:pass %s!...\n",__FUNCTION__, temp_pwd);
		if(!tcapi_get(USERENTRY, ACCOUNTPASS, buffer)  && !strcmp(temp_pwd, buffer)){			
			status = 0;
		}
		else if(!tcapi_get(ADMINENTRY, ACCOUNTPASS, buffer)   && !strcmp(temp_pwd, buffer)){
			status = 0;
		}	
	}
	//build parameter
	if(!(reply_para = ParameterBuildJSONReplyPara(req_para, status))){
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
		goto err;
	}
	if(0 == status){
		buffer[0] = '\0';
		if(JSON_OK != ParameterAddJSONChild(reply_para, PARA_CARD, JSON_STRING, js_get_card(0, buffer))){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_LOID, js_get_LOID(0,NULL))){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}

		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_SN, JS_Calc_MD5(gDevRNDstr, getSSN(), SNbuf))){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	}else{
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "password err!")){
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
	mobile_printf(MOBILE_INFO, "%s: reply json pkt=%s!...\n", __FUNCTION__, value_str);
	free(value_str);	
#endif	

	return reply_root;

err:
	if(reply_root) ParameterFreeJSON(&reply_root);
	if(reply_para) ParameterFreeJSON(&reply_para);
	return NULL;

}


