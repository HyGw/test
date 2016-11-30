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
#include "Set_GatewayTime_func.h"

json_t* SetGatewayTime(json_t* req_root, json_t* req_para)
{
	json_t *cursor = NULL, *reply_root = NULL, *reply_para = NULL;
	char *cGatewayTimeValue = NULL;
	char GatewayTimeValue[30] = {0};
	unsigned int tm_year=0, tm_mon=0, tm_date=0, tm_hour=0, tm_min=0, tm_sec=0;
	char cTimezoneType[8] = {0};
	int ret = -1;

	mobile_printf(MOBILE_INFO,"%s:start!...\n",__FUNCTION__);

	cGatewayTimeValue = ParameterSearchValue(req_para, "Time");

	if(cGatewayTimeValue !=NULL)
	{
		if(sscanf(cGatewayTimeValue, "%04d-%02d-%02dT%02d:%02d:%02dZ", &tm_year, &tm_mon, &tm_date, &tm_hour, &tm_min, &tm_sec) == 6)
		{
			if( ((tm_mon<13)&&(tm_mon>0)) && (tm_date<32) && (tm_hour<25) && (tm_min<60) && (tm_sec<60) )
				ret = 0;
		}
	}
	
	if(ret != -1)
	{
		sprintf(GatewayTimeValue, "%d/%d/%d/%d:%d:%d", tm_year, tm_mon, tm_date, tm_hour, tm_min, tm_sec);
		
		mobile_printf(MOBILE_DEBUG,"%s, line = %d, PC_CLOCK = %s\n",__FUNCTION__, __LINE__, GatewayTimeValue);

		tcapi_get("Timezone_Entry", "TYPE", cTimezoneType);
		if(strcmp(cTimezoneType, "1"))
			tcapi_set("Timezone_Entry", "TYPE", "1");
		tcapi_set("Timezone_Entry", "PC_CLOCK", GatewayTimeValue);
		tcapi_commit("Timezone_Entry");
		
		if(strcmp(cTimezoneType, "1"))
		{
			sleep(1);
			tcapi_set("Timezone_Entry", "TYPE", cTimezoneType);
			tcapi_commit("Timezone_Entry");
		}

		//build parameter
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
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "ERROR_DATETIME_FORMAT")){
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
