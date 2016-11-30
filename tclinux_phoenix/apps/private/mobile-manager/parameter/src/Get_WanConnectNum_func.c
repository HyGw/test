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
#include "Get_WanConnectNum_func.h"

json_t* Get_WanConnectNum(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	char AllWanName[512] = {0}, WanName[64] = {0}, cWanConnectNum[8] = {0}, nodeName[48] = {0};
	char *cur_IFIndex = NULL, *cValidIFIndex = NULL, ValidIFIndex[64] = {0}, IFIndex[MAX_PVC_NUM][MAX_SMUX_NUM] = {0};
	char str_ifaceID[8] = {0}, str_servicelist[64] = {0}, str_vid[6] = {0}, str_mode[16] = {0};
	unsigned int uiWanConnectNum = 0, pvc_index = 0, entry_idx = 0, cur_id = 0;
	int ret = 0, pvc_num = 0, i = 0, j = 0;

	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__, __LINE__);

	tcapi_get("WanInfo_Common", "CycleNum", cWanConnectNum);
	tcapi_get("WanInfo_Common", "ValidIFIndex", ValidIFIndex);
	
	cValidIFIndex = ValidIFIndex;
	cur_IFIndex = ValidIFIndex;

	uiWanConnectNum = atoi(cWanConnectNum);

	while(*cValidIFIndex!='\0')
	{
		if(*cValidIFIndex == ','){
			strncpy(IFIndex[pvc_num], cur_IFIndex,j);
			j = 0;
			pvc_num++;
			cValidIFIndex++;
			cur_IFIndex = cValidIFIndex;
		}
		else{
			j++;
			cValidIFIndex++;
		}
	}

	if(0 <= uiWanConnectNum)
	{	
		memset(nodeName,0,sizeof(nodeName));

		for(i = 0; i < pvc_num; i++){

			cur_id = atoi(IFIndex[i]);		
			pvc_index = cur_id / MAX_PVC_NUM;
			entry_idx = cur_id % MAX_PVC_NUM;

			sprintf(nodeName, "Wan_PVC%d_Entry%d", pvc_index,entry_idx);

			tcapi_get(nodeName, "IFIdx", str_ifaceID);
			tcapi_get(nodeName, "ServiceList", str_servicelist);			
			tcapi_get(nodeName, "VLANID", str_vid);
			tcapi_get(nodeName, "WanMode", str_mode);
			if( 0 == strcmp(str_mode, "Route") )
				strcpy(str_mode, "R");
			else
				strcpy(str_mode, "B");
		
			sprintf(WanName, "%s_%s_%s_VID_%s", str_ifaceID, str_servicelist, str_mode, str_vid);

			if(i > 0)
				strcat(AllWanName,"/");
			
			strcat(AllWanName,WanName);
		}
	}


	//build parameter
	if(ret == -1){
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "Waninfo file Is Not Exist")){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	else{
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){//ok
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_NUM, cWanConnectNum)){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_DESCRIPTION, AllWanName)){
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


