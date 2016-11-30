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

#include "Get_Duration_func.h"

json_t* Get_Duration_func(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	char sysduration[10] = {0}, pppdurtime[64] = {0}, ponduration[10] = {0};
	int size = 0;
	
	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__,__LINE__);

	get_sysup_duration(sysduration);
	get_ppp_duration(pppdurtime);
	get_PON_duration(ponduration);

	//build parameter
	if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){//ok
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_SYSDuration, sysduration)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_PPPoEDuration, pppdurtime)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_PONDuration, ponduration)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
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

void get_sysup_duration(char* temp)
{
	char begintime[10] = {0};
	struct timespec currtime;
	int nowtime=0;

	tcapi_get("SysInfo_Entry","sysStartTime",begintime);
	clock_gettime(CLOCK_MONOTONIC,&currtime);
	nowtime = currtime.tv_sec - atoi(begintime);
	strcpy(temp,itoa(nowtime));
}

void get_ppp_duration(char* temp)
{
	int nIspNode = -1;
	char nodeName[20] = {0}, active[8] = {0}, serviceType[64] = {0}, pppUptimePath[32] = {0}, linkmode[20] = {0},
		pvc_info[16] = {0},str_wanIp[128]={0};
	unsigned int pvc_index = 0, entry_idx = 0;
	struct timespec currtime;
	
	for ( pvc_index = 0; pvc_index < MAX_PVC_NUM; pvc_index ++ ){
		for ( entry_idx = 0; entry_idx < MAX_SMUX_NUM; entry_idx ++ ){
			sprintf(nodeName, "Wan_PVC%d_Entry%d", pvc_index, entry_idx);
			tcapi_get(nodeName, "Active", active);
			tcapi_get(nodeName, "ServiceList", serviceType);
			tcapi_get(nodeName, "LinkMode", linkmode);
			sprintf(nodeName, "WanInfo_Entry%d",pvc_index* MAX_SMUX_NUM + entry_idx);
			tcapi_get(nodeName, "IP", str_wanIp);
			if(!strcmp(active, "Yes") &&
				!strcmp(linkmode, "linkPPP")&&
				!strcmp(serviceType, "INTERNET")&&
				(strlen(str_wanIp)>0)){
					nIspNode = pvc_index* MAX_SMUX_NUM + entry_idx;
					sprintf(pppUptimePath,"/tmp/pppconntime-ppp%d",nIspNode);
					fileRead(pppUptimePath, pvc_info, sizeof(pvc_info));
					if(strlen(pvc_info) > 0){
					/*Caculate ppp link-up time*/
						clock_gettime(CLOCK_MONOTONIC,&currtime);
				      	sprintf(temp,"%d",currtime.tv_sec - atoi(pvc_info));
					}
					else{
						sprintf(temp,"%d",0);
					}
					return ; 
			}	
		}
	}
	sprintf(temp,"%d",0);
}
	
void get_PON_duration(char* temp)
{
	struct timespec currtime;
	int nowtime=0;
	char begintime[10] = {0}, tempphyStatus[20] = {0}, temptrafficStatus[20] = {0};

	tcapi_get("XPON_Common", "phyStatus", tempphyStatus);
	tcapi_get("XPON_Common", "trafficStatus", temptrafficStatus);
	if(strstr(tempphyStatus, "up") && strstr(temptrafficStatus, "up")){
		tcapi_get("mobile_Entry", "PONstarttime",begintime);
		clock_gettime(CLOCK_MONOTONIC,&currtime);
		nowtime=currtime.tv_sec - atoi(begintime);
	}
	strcpy(temp,itoa(nowtime));
}

