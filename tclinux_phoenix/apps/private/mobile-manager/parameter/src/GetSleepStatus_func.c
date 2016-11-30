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
#include "GetSleepStatus_func.h"

json_t* GetSleepStatus(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	int ret = 0;
	json_t *nodes_array = NULL, *array_obj;
	int count = 0;
	char attrName[32] = {0};
	char attrValue[64] = {0};
	int i = 0;

	mobile_printf(MOBILE_INFO,"%s:start!...\n",__FUNCTION__);

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
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		nodes_array = ParameterNewJSONArray();
		if ( !nodes_array )
		{
			mobile_printf(MOBILE_INFO,
				"%s:malloc JSON array Err!...\n",__FUNCTION__);
			goto err;
		}
		
		/*
		add array to parameter list, then only need free reply_para
		*/
		if ( JSON_OK != ParameterInsertJSONPair(reply_para
			, "SLEEPTimer"
			, nodes_array) )
		{
			mobile_printf(MOBILE_INFO,
				"%s:Add JSON array to para Err!...\n",__FUNCTION__);
			goto err;
		}

		memset(attrValue, 0, sizeof(attrValue));
		if(tcapi_get(MOBILE_SLEEPENTRY, "Count", attrValue)==0)
			count = atoi(attrValue);
		for(i = 1; i <= count; i++){
			array_obj = ParameterNewJSONObject();
			if ( !array_obj )
			{
				mobile_printf(MOBILE_INFO,
					"%s:malloc JSON array obj Err!...\n",__FUNCTION__);
				goto err;
			}
			sprintf(attrName, "Weekday%d", i);
			memset(attrValue, 0, sizeof(attrValue));
			tcapi_get(MOBILE_SLEEPENTRY, attrName, attrValue);
			if( JSON_OK != ParameterAddNewJSONChild(array_obj
				, "Weekday", attrValue) )
			{
				mobile_printf(MOBILE_INFO
					,"%s:ParameterAddNewJSONChild Err!...\n"
					,__FUNCTION__);
				goto err;
			}
			sprintf(attrName, "Time%d", i);
			memset(attrValue, 0, sizeof(attrValue));
			tcapi_get(MOBILE_SLEEPENTRY, attrName, attrValue);
			if( JSON_OK != ParameterAddNewJSONChild(array_obj
				, "Time", attrValue) )
			{
				mobile_printf(MOBILE_INFO
					,"%s:ParameterAddNewJSONChild Err!...\n"
					,__FUNCTION__);
				goto err;
			}
			sprintf(attrName, "Active%d", i);
			memset(attrValue, 0, sizeof(attrValue));
			tcapi_get(MOBILE_SLEEPENTRY, attrName, attrValue);
			if( JSON_OK != ParameterAddNewJSONChild(array_obj
				, "Active", attrValue) )
			{
				mobile_printf(MOBILE_INFO
					,"%s:ParameterAddNewJSONChild Err!...\n"
					,__FUNCTION__);
				goto err;
			}
			sprintf(attrName, "Enable%d", i);
			memset(attrValue, 0, sizeof(attrValue));
			tcapi_get(MOBILE_SLEEPENTRY, attrName, attrValue);
			if( JSON_OK != ParameterAddNewJSONChild(array_obj
				, "Enable", attrValue) )
			{
				mobile_printf(MOBILE_INFO
					,"%s:ParameterAddNewJSONChild Err!...\n"
					,__FUNCTION__);
				goto err;
			}		
			/*
			add to array then it will free array only no need to free every object
			*/
			if ( JSON_OK != json_insert_child(nodes_array, array_obj) )
			{
				mobile_printf(MOBILE_INFO,
					"%s:Add JSON array obj Err!...\n",__FUNCTION__);
				goto err;
			}
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

