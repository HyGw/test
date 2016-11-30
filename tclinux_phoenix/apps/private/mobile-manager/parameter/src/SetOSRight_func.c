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
#include "SetOSRight_func.h"

#if defined(TCSUPPORT_CT_JOYME)
json_t* SetOSRight(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	char *pCPU = NULL;
	char *pRAM = NULL;
	char *pFLASH = NULL;
	char *pSOCKET = NULL;
	char *pPORT = NULL;
	char *pthread = NULL;
	int ret = 0;
	char failReason[32] = {0};

	mobile_printf(MOBILE_INFO,"%s:start!...\n",__FUNCTION__);

	pCPU = ParameterSearchValue(req_para, "CPU");
	pRAM = ParameterSearchValue(req_para, "RAM");
	pFLASH = ParameterSearchValue(req_para, "FLASH");
	pSOCKET = ParameterSearchValue(req_para, "SOCKET");
	pPORT = ParameterSearchValue(req_para, "PORT");
	pthread = ParameterSearchValue(req_para, "thread");
	if(pCPU==NULL || pRAM==NULL || pFLASH==NULL || pSOCKET==NULL || pPORT==NULL || pthread==NULL){
		ret = -1;
		goto REPLY_ERR_MSG;
	}
	mobile_printf(MOBILE_INFO,"%s:CPU=%s,RAM=%s,FLASH=%s,SOCKET=%s,PORT=%s,thread=%s\n", __FUNCTION__,pCPU,pRAM,pFLASH,pSOCKET,pPORT,pthread);

	tcapi_set(MOBILE_OSRIGHTENTRY,"CPU",pCPU);
	tcapi_set(MOBILE_OSRIGHTENTRY,"RAM",pRAM);
	tcapi_set(MOBILE_OSRIGHTENTRY,"FLASH",pFLASH);
	tcapi_set(MOBILE_OSRIGHTENTRY,"SOCKET",pSOCKET);
	tcapi_set(MOBILE_OSRIGHTENTRY,"PORT",pPORT);
	tcapi_set(MOBILE_OSRIGHTENTRY,"thread",pthread);
	tcapi_save();

REPLY_ERR_MSG:
	//build parameter
	if(ret < 0){/* Fail */
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		switch (ret) {
			case -1:
				strcpy(failReason, "Parameter error");
				break;
			default:
				strcpy(failReason, "Parameter error");
				break;
		}		
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, failReason)){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	else{/* Success */
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
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
#endif
