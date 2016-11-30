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

#include "GET_HG_DEV_NAME.h"

#if defined(TCSUPPORT_CT_JOYME)
json_t* LanDeviceAccess(json_t* req_root, json_t* req_para) {
	json_t  *reply_root = NULL, *reply_para = NULL;
	json_t *label = NULL;

	label = ParameterSearchFirstLabel(req_para, "Enable");
	mobile_printf(MOBILE_DEBUG,"%s, line = %d enable = %s\n",__FUNCTION__,__LINE__, label->child->text);
	if(label && label->child && (!strcmp(label->child->text, "0") || 
	   !strcmp(label->child->text, "1"))) {
	     tcapi_set("mobile_Entry", "lanenable", label->child->text);
		 tcapi_save();
		 if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))) {
			 mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			 goto err;
		 }	
	}
	else {
		 if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))) {
			 mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			 goto err;
		 }
		 if(JSON_OK != ParameterAddNewJSONChild(reply_para, FAILREASON, "parse error")){
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
	if(reply_para) ParameterFreeJSON(&reply_para);
	return reply_root;

err:
	if(reply_root) ParameterFreeJSON(&reply_root);
	if(reply_para) ParameterFreeJSON(&reply_para);
	return NULL;
}

json_t* GetHgdevName(json_t* req_root, json_t* req_para)
{
	json_t  *reply_root = NULL, *reply_para = NULL;
	char *MAClist = NULL, *Namelist = NULL;
	char MacAddr[24] = {0},name[64] = {0}; 
	int ret = -1;
	
	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__,__LINE__);
	MAClist = (char*)malloc(sizeof(MacAddr)*MAX_LDEV_NUM);
	Namelist = (char*)malloc(sizeof(name)*MAX_LDEV_NUM);
	
	if(MAClist == NULL || Namelist == NULL){
		ret = -1;
	}
	memset(MAClist,0,sizeof(MAClist));
	memset(Namelist,0,sizeof(Namelist));
	ret = get_mac_name(MAClist,Namelist);
	if(ret == 0){
		//build parameter
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){//ok
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, NAMELIST, Namelist)){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, MACLIST, MAClist)){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	else{
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){//ok
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "fail to get devname")){
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

	free(MAClist);
	free(Namelist);
	if(reply_para) ParameterFreeJSON(&reply_para);
	return reply_root;
	
err:
	free(MAClist);
	free(Namelist);
	if(reply_root) ParameterFreeJSON(&reply_root);
	if(reply_para) ParameterFreeJSON(&reply_para);
	return NULL;
}

int get_mac_name(char *maclist,char *namelist)
{
	int ret,devNum,i;
	CtLanDevInfo *devInfoList = NULL;
	char macAddr[24] = {0},Name[64] = {0};

	if(maclist == NULL || namelist ==NULL)
		return -1;

	ret = joyme_lanGetDevDevInfo(&devInfoList,&devNum); 
	if(ret != 0){
		free(devInfoList);
		return -1;
	}
	
	tcapi_get("Info_Ether","mac",macAddr);
	tcapi_get("DeviceInfo","GateWay",Name);
	strcat(maclist,macAddr);
	strcat(namelist,Name);
	for(i = 0;i < devNum;i++){
		memset(macAddr,0,sizeof(macAddr));
		memset(Name,0,sizeof(Name));
		
		snprintf(macAddr,sizeof(macAddr)-1,"/%02x:%02x:%02x:%02x:%02x:%02x",
			devInfoList[i].macAddr[0],devInfoList[i].macAddr[1],devInfoList[i].macAddr[2],
			devInfoList[i].macAddr[3],devInfoList[i].macAddr[4],devInfoList[i].macAddr[5]);
			
		sprintf(Name,"/%s",devInfoList[i].devName);
		strcat(maclist,macAddr);
		strcat(namelist,Name);
	}

	free(devInfoList);
	devInfoList = NULL;
	return 0;
}
#endif
