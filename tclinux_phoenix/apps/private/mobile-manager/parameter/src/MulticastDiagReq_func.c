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
#include "MulticastDiagReq_func.h"

#if defined(TCSUPPORT_CT_JOYME)
json_t* MulticastDiagReq(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	char diagValue[32] = {0};
	int ret = -1;

	mobile_printf(MOBILE_INFO,"%s:start!...\n",__FUNCTION__);

	ret = getMulticastDiagValue(diagValue);

	//build parameter
	if(ret == -1){/* Fail */
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "multicastDiagValue file Is Not Exist")){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	else{/* Success */
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_DIAGRESULT, diagValue)){
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

int getMulticastDiagValue(char *retValue){
	int i = 0, j = 0;
	char nodeName[48] = {0};
	char wanMode[32] = {0};
	char serviceList[32] = {0};
	char active[8] = {0};
	char ponStatus[8] = {0};
	char multiVlanID[8] = {0};
	int ret = 0;
	int hit_flag = 0;
	char file_name[]="/tmp/multicastDiagValue";
	char cmd[64]={0};
	FILE  *fp = NULL;
	char buf[128] = {0};
	int port_no1=0;
	int port_no2=0;

	mobile_printf(MOBILE_INFO,"%s:start!...\n",__FUNCTION__);

	for(i = 0; i < MAX_PVC_NUM; i++) {
		for(j = 0; j < MAX_SMUX_NUM; j++) {
			memset(wanMode, 0, sizeof(wanMode));	
			sprintf(nodeName, "Wan_PVC%d_Entry%d", i, j);
			tcapi_get(nodeName, "WanMode", wanMode);
			tcapi_get(nodeName, "ServiceList", serviceList);			
			if(0==strcmp(wanMode, "Bridge")&&0==strcmp(serviceList, "OTHER")){
				hit_flag = 1;
				break; 
			}
		}
		if(hit_flag==1)
			break;		
	}
	if(hit_flag==0){
		strcpy(retValue,"NO_IPTV_CONNECTION");
		return 0;
	}
	mobile_printf(MOBILE_INFO,"Find Bridge Other connection:Wan_PVC%d_Entry%d\n", i, j);

	tcapi_get(nodeName, "Active", active);
	tcapi_get("XPON_Common", "trafficStatus", ponStatus);	
	if(0!=strcmp(active, "Yes") || 0!=strcmp(ponStatus, "up")){
		strcpy(retValue,"IPTV_DISCONNECT");
		return 0;		
	}
	mobile_printf(MOBILE_INFO,"Bridge Other connection is up\n");

	tcapi_get(nodeName, "MulticastVID", multiVlanID);	
	if(strlen(multiVlanID)==0){
		strcpy(retValue,"INVALID_MULTIVLAN");
		return 0;		
	}
	mobile_printf(MOBILE_INFO,"Bridge Other connection MulticastVID=%s\n",multiVlanID);
	
	sprintf(cmd,"brctl showsnoop br0 > %s", file_name);
	system(cmd);
	fp = fopen(file_name,"r");
	if(fp  == NULL){
		mobile_printf(MOBILE_INFO,"Open multicastDiagValue file fail\n");
		return -1;
	}
	while (fgets(buf, sizeof(buf), fp)) 
	{
		if(strstr(buf,"eth0.2") == NULL)
			continue;
		else{
			sscanf(buf,"eth0.2 (%d)", &port_no1);
			break;
		}
	}	
	if(port_no1==0){
		strcpy(retValue,"IPTV_BUSSINESS_NOK");
		fclose(fp);
		mobile_printf(MOBILE_INFO,"Can not find eth0.2 port no\n");
		return 0;
	}
	while (fgets(buf, sizeof(buf), fp)) 
	{
		if(strstr(buf,"port no") == NULL)
			continue;
		else
			break;
	}
	while (fgets(buf, sizeof(buf), fp)) 
	{
		sscanf(buf,"%3i\t", &port_no2);
		if(port_no1==port_no2)
			break;
	}
	fclose(fp);
	if(port_no1!=port_no2){
		strcpy(retValue,"IPTV_BUSSINESS_NOK");
		mobile_printf(MOBILE_INFO,"Can not find eth0.2 snoop group\n");
		return 0;
	}
	mobile_printf(MOBILE_INFO,"Bridge Other connection bussiness OK\n");

	strcpy(retValue,"IPTV_BUSSINESS_OK"); 
	return 0;
}
#endif
