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
#include "Get_PPP_DialStatus_func.h"
#include "PPPDialControl_func.h"
#include <unistd.h>

static char nodeName[48] = {0};

json_t* PPP_DialControl(json_t* req_root, json_t* req_para)
{
	json_t * reply_root = NULL, *reply_para = NULL;
	char  *temp_pppusername = NULL, *temp_ppppass = NULL;
	char pppdialflag[10] = {0};
	char IPv4ConStatus[8] = {0}, IPv4Status[32] = {0}, IPv4ConError[128] = {0};
	unsigned long wanNodeIndex = 0, pvcNodeEntry = 0;
	int IPv4ret = -1, IPv6ret = -1, defRouterIfv4 = -1, defRouterIfv6 = -1;
	unsigned char timecount = 0;
	int ret = 0;

	mobile_printf(MOBILE_INFO,"%s:start!...\n",__FUNCTION__);

	temp_pppusername = ParameterSearchValue(req_para, "Account");
	temp_ppppass = ParameterSearchValue(req_para, "Password");		

	if((temp_pppusername != NULL) && (temp_ppppass != NULL))
	{
		getISPIndex(&defRouterIfv4, &defRouterIfv6);;
		if(defRouterIfv4 >= 0)
		{
			wanNodeIndex = defRouterIfv4 / MAX_PVC_NUM;
			pvcNodeEntry = defRouterIfv4 % MAX_PVC_NUM;

			sprintf(pppdialflag, "ppp%d", defRouterIfv4);
			tcapi_set("WebCustom_Entry", "pppdialflag", pppdialflag);
			tcapi_set("WebCustom_Entry", "pppdialusername", temp_pppusername);
			tcapi_set("WebCustom_Entry", "pppdialpassword", temp_ppppass);
			sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex, pvcNodeEntry);
			tcapi_commit(nodeName);

			while(1)
			{
				sleep(1);
				IPv4ret = getWANCnt1_IPv4InfoValue(IPv4ConStatus, IPv4Status, IPv4ConError, wanNodeIndex, pvcNodeEntry);

				if (IPv4ret == -1)
				{
					ret = -3;
					break;
				}
				else if(strcmp(IPv4ConStatus, "0") == 0)
					break;
				else if((++timecount) > PPPDIALMAXTIME)
					break;
			}
			
			ThreadPoolAddWork(&gThreadPool, pppdial_process, NULL);
		}
		else
			ret = -2;
	}
	else
		ret = -1;

	//build parameter
	if(ret == 0){
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_CONSTATUS, IPv4ConStatus)){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_WANSTA, IPv4Status)){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_PPPDIALREASON, IPv4ConError)){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	else{
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		 if(ret == -1)
		{
			if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "Account or Password Error")){
				mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
				goto err;
			}
		}
		else if(ret == -2)
		{
			if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "There Is No Actived Route Internet interface")){
				mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
				goto err;
			}
		}
		else if(ret == -3)
		{
			if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "Waninfo file Is Not Exist")){
				mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
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

	return reply_root;

err:
	if(reply_root) ParameterFreeJSON(&reply_root);
	if(reply_para) ParameterFreeJSON(&reply_para);
		return NULL;

}

void * pppdial_process (worker_data_t *argv)
{
	sleep(RESTOREDELAYTIME);
	tcapi_set("WebCustom_Entry", "pppdialflag", "");
	tcapi_set("WebCustom_Entry", "pppdialusername", "");
	tcapi_set("WebCustom_Entry", "pppdialpassword", "");
	tcapi_commit(nodeName); 	
	return NULL;
}
