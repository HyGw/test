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
#include "Set_PlatformParam_func.h"
#include "info.h"

json_t* Set_PlatformParam(json_t* req_root, json_t* req_para)
{
	json_t * reply_root = NULL, *reply_para = NULL;
	char *mgturl = NULL, *remoteport = NULL, *heartbeat = NULL, *ability = NULL, *localport = NULL, *version = NULL;
	unsigned int remoteport_int = 0, heartbeat_int = 0, ability_int = 0, localport_int = 0;
	char temp[10] = {0}, buffer[256] = {0};
	int ret = 0;
	
	mobile_printf(MOBILE_INFO,"%s:start!...\n",__FUNCTION__);

	mgturl = ParameterSearchValue(req_para, "MgtURL");
	remoteport = ParameterSearchValue(req_para, "Port");
	heartbeat = ParameterSearchValue(req_para, "HeartBeat");
	ability = ParameterSearchValue(req_para, "Ability");
	localport = ParameterSearchValue(req_para, "LocalPort");
	version = ParameterSearchValue(req_para, "Version");

	if((mgturl==NULL) || (remoteport==NULL) || (heartbeat==NULL) || (ability==NULL) || (localport==NULL) || (version==NULL) )
		ret = -5;
	else if((strlen(remoteport)>0) && ((sscanf(remoteport, "%u", &remoteport_int)!=1) || (remoteport_int>65535)))
		ret = -1;
	else if((strlen(heartbeat)>0) && (sscanf(heartbeat, "%u", &heartbeat_int) != 1))
		ret = -2;
	else if((strlen(ability)>0) && ((sscanf(ability, "%u", &ability_int) != 1) || (ability_int>1)))
		ret = -3;
	else if((strlen(localport)>0) && ((sscanf(localport, "%u", &localport_int) != 1) || (localport_int>65535)))
		ret = -4;

	//build parameter
	if(ret == 0)
	{
		if(strlen(mgturl)>0){
			tcapi_get(MOBILE_ENTRY, DISTRIADDR, buffer);
			if(strcmp(buffer, mgturl)){
				tcapi_set(MOBILE_ENTRY, DISTRIADDR, mgturl);
				tcapi_set(MOBILE_ENTRY, OPERATADDR, "");  //clear operatr ip 
			}
		}
		
		if(strlen(remoteport)>0)
		{
			memset(temp, 0, sizeof(temp));
			sprintf(temp, "%u", remoteport_int);
			tcapi_set(MOBILE_ENTRY, "Port", temp);
		}
		
		if(strlen(heartbeat)>0)
		{
			memset(temp, 0, sizeof(temp));
			sprintf(temp, "%u", heartbeat_int);
			tcapi_set(MOBILE_ENTRY, "Heartbeat", temp);
		}
		
		if(strlen(ability)>0)
		{
			memset(temp, 0, sizeof(temp));
			sprintf(temp, "%u", ability_int);
			tcapi_set(MOBILE_ENTRY, "Ability", temp);
		}
		
		if(strlen(localport)>0)
		{
			memset(temp, 0, sizeof(temp));
			sprintf(temp, "%u", localport_int);
			tcapi_set(MOBILE_ENTRY, "LocatePort", temp);
		}

		if(strlen(version)>0)
			tcapi_set(MOBILE_ENTRY, "Version", version);
		
		tcapi_save();
		
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
		if(ret == -1)
		{
			if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "Port error")){
				mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
				goto err;
			}
		}
		else if(ret == -2)
		{
			if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "HearBeat error")){
				mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
				goto err;
			}
		}
		else if(ret == -3)
		{
			if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "Ability error")){
				mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
				goto err;
			}
		}
		else if(ret == -4)
		{
			if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "LocalPort error")){
				mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
				goto err;
			}
		}
		else if(ret == -5)
		{
			if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "Parameter error")){
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
