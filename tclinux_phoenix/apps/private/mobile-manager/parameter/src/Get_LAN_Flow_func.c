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

#include "Get_LAN_Flow_func.h"

json_t* Get_LAN_Flow_func(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	char portrx[4][20] = {0}, porttx[4][20] = {0}, nodeName[48] = {0}, portnamerx[10] = {0}, portnametx[10] = {0};
	int ret = 0, inum = 0;
	int size = 0;
	int maxLanNum = 4;
	
	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__,__LINE__);
	if(TCSUPPORT_CT_2PORTS_VAL){
		maxLanNum = 2;
	}

	for(inum = 0;inum < maxLanNum; inum ++){
		sprintf(nodeName, "Info_eth0.%d", inum+1);

		tcapi_get( nodeName, "rxbytes", portrx[inum]);
		size = atoi(portrx[inum])/1024;
		strcpy(portrx[inum],itoa(size));

		tcapi_get( nodeName, "txbytes", porttx[inum]);
		size = atoi(porttx[inum])/1024;
		strcpy(porttx[inum],itoa(size));
	}
	
	//build parameter
	if(ret == -1){
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "fail to Get LAN Flow")){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	else{
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){//ok
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		for(inum = 0;inum < maxLanNum; inum ++){
			sprintf(portnamerx,"Port%dRX",inum+1);
			sprintf(portnametx,"Port%dTX",inum+1);
			if(JSON_OK != ParameterAddNewJSONChild(reply_para, portnamerx, portrx[inum])){
				mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
				goto err;
			}
			if(JSON_OK != ParameterAddNewJSONChild(reply_para, portnametx, porttx[inum])){
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
