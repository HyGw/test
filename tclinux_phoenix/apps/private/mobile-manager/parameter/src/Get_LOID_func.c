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

#include "Get_LOID_func.h"
#include "libcompileoption.h"
#if defined(TCSUPPORT_CT_JOYME)
#include "joyme.h"
#endif

json_t* Get_LOID_func(json_t* req_root, json_t* req_para)
{
	json_t  *reply_root = NULL, *reply_para = NULL;
	char loidname[256] = {0}, cardNo[64] = {0}, linkSta[32] = {0};
	int loidlen = sizeof(loidname);
	
	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__,__LINE__);

#if defined(TCSUPPORT_CT_JOYME)
	joyme_sysGetLoid(loidname,loidlen);
#else
	tcapi_get("XPON_LinkCfg", "LinkSta", linkSta);
	if (strcmp(linkSta,"2") == 0){ // epon
		tcapi_get("EPON_LOIDAuth", "LOID0", loidname);
	}
	else{
		tcapi_get("GPON_LOIDAuth", "LOID", loidname);
	}
#endif

	if(TCSUPPORT_CT_SIMCARD_SEPARATION_VAL){	
		char buf[5] = {0};	

		tcapi_get("SimCard_Entry","cpeFlag",buf);	
		if(strcmp(buf,"1") == 0){		
			memset(buf, 0, sizeof(buf) );		
			tcapi_get("SimCard_Entry","cardExist",buf); 			
			if(strcmp(buf,"1") == 0){
				tcapi_get("SimCard_Entry","CardSN",cardNo);		
			}	
		}
	}

	//build parameter
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){//ok
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_LOID, loidname)){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_CARD, cardNo)){
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

	if(reply_para) ParameterFreeJSON(&reply_para);
	return reply_root;
	
err:
	if(reply_root) ParameterFreeJSON(&reply_root);
	if(reply_para) ParameterFreeJSON(&reply_para);
	return NULL;
}
