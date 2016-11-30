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

#include "Set_Access_of_Interface_func.h"

extern phone_para_t gPhone_Para[];

json_t* Set_Access_of_Interface_func(json_t* req_root, json_t* req_para)
{
	json_t  *reply_root = NULL, *reply_para = NULL;
	char *cmdtype = NULL, *right = NULL;
	char *rightlist;
	int right_num = 0;
	int ret = -1, i = 0, pos=0;
	phone_para_ptr p_para = NULL;
	
	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__,__LINE__);

	rightlist = (char *)malloc(GetParaTableLen());
	if (rightlist == NULL) {
		ret = -1;
		goto reply;
	}
	memset(rightlist, 0, GetParaTableLen());

	cmdtype = ParameterSearchValue(req_para, "API");
	right = ParameterSearchValue(req_para, "RIGHT");
	if((cmdtype != NULL) && (right != NULL)){
		right_num = atoi(right);
		if( (right_num > 3) || (right_num < 0) ){
			ret = -1;
		}
		else{
			ret = 0;
			if(!(p_para = SearchParaTable(cmdtype,&pos)))
				ret = -1;
			else{
				tcapi_get("mobile_Entry","InterfaceRight",rightlist);
				if( strlen(rightlist) != GetParaTableLen() ){
					memset(rightlist,0,sizeof(rightlist));
				for(i = 0; i < GetParaTableLen(); i++){
						if(gPhone_Para[i].permit >= PERMIT_LOCAL && gPhone_Para[i].permit <= PERMIT_BOTH){
							rightlist[i] = gPhone_Para[i].permit + 0x2F; 
						}
						else{
							rightlist[i] = '3'; //PERMIT_NONE
						}
					}
				}
				p_para->permit = (right_num+1) & 0x03;	
				rightlist[pos] = right_num + 0x30;
				tcapi_set("mobile_Entry","InterfaceRight",rightlist);
				tcapi_commit("mobile_Entry");
				tcapi_save();
			}
		}
	}

reply:
	
	//build parameter
	if(ret == -1){
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "fail to set Access of Interface")){
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
        free(rightlist);
	return reply_root;
	
err:
	if(reply_root) ParameterFreeJSON(&reply_root);
	if(reply_para) ParameterFreeJSON(&reply_para);
	free(rightlist);
	return NULL;
}
