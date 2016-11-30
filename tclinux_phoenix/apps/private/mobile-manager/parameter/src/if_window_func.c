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
#include "if_window_func.h"
#include "../../../lib/libtcapi.h"
#if defined(TCSUPPORT_CT_JOYME)
#include "joyme.h"
#endif

json_t* SetWDInfo(json_t* req_root, json_t* req_para)
{
	json_t* reply_root = NULL, *reply_para = NULL;
	char* value_str = NULL, *place = NULL;
	char attr_array[][16]={{"enable"},{"W"},{"H"},{"Place"},{"WDurl"},{"time"},{""}};
	char attrName[32] = {0};
	char attrValue[32] = {0};
	char buffer[256] = {0};
	int value_int = 0;
	int PopupWindowEnableFlag=1;
	char info[2][32] = {0};
	int i=0,value[6] = {0};
	
	mobile_printf(MOBILE_INFO,"====SetWDInfo====.....\n");

	value_str = ParameterSearchValue(req_para, "enable");
	if(value_str && 0 == strcmp(value_str, "1")){
		tcapi_set("System_Entry","wd_Status", "-1");//set default:fail
#if defined(TCSUPPORT_CT_JOYME)
		for(i=0; 0 != strlen(attr_array[i]); i++)
		{
			value_str = ParameterSearchValue(req_para, attr_array[i]);
			if(value_str){
				if(3 == i)
					place = value_str;
				else if(4 == i)
					strcpy(buffer,value_str);
				else
					value[i] = atoi(value_str);
			}
		}
		joyme_sysPushWeb(value[0],value[1],value[2],place
			,(const char*)buffer,value[5]);
#else
		for(i=0; 0 != strlen(attr_array[i]); i++)
		{
			value_str = ParameterSearchValue(req_para, attr_array[i]);
			if(value_str){
				sprintf(attrName,"wd_%s",attr_array[i]);
				tcapi_set("System_Entry", attrName, value_str);
				if(3 == i){  /* 0 == strcmp(attr_array[i], "Place")){ */
					if(NULL!=strstr(value_str,",")){
						sscanf(value_str,"%[^,],%[^,]", info[0],info[1]);
						tcapi_set("System_Entry", "wd_top", info[0]);
						tcapi_set("System_Entry", "wd_left", info[1]);
					}
				}
				else if(4 == i){ /* WDurl */
					if(strncmp(value_str, "http://", 7)){
						snprintf(buffer, sizeof(buffer),"http://%s", value_str);
						tcapi_set("System_Entry", attrName, buffer);
					}
				}
			}
		}
#endif
		while(PopupWindowEnableFlag){
			sleep(1);
			tcapi_get("System_Entry","wd_enable", attrValue);
			PopupWindowEnableFlag=atoi(attrValue);
		}
		tcapi_get("System_Entry","wd_Status", attrValue);
		value_int=atoi(attrValue);

		//build parameter
		if(value_int == 0){
			if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){//ok
				mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
				goto err;
			}

			tcapi_get("System_Entry","wd_Stat", attrValue);
			if(JSON_OK != ParameterAddNewJSONChild(reply_para, "Stat", attrValue)){
				mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
				goto err;
			}

		}
		else{			
			if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
				mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
				goto err;
			}
		}
	}
	else{
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){//ok
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}

		if(JSON_OK != ParameterAddNewJSONChild(reply_para, "Stat", "0")){
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

