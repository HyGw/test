/*
** $Id: //BBN_Linux/DEV/main/tclinux_phoenix/apps/private/phone_app/parameter/src/Get_PonRegistrationInfo_func.c#1 $
*/
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

#include "Get_PonRegistrationInfo_func.h"

json_t* Get_PonRegistrationInfo(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	char tempphyStatus[20] = {0};
	char temptrafficStatus[20] = {0};
	char tempPonStatus[48] = {0};

	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__,__LINE__);

	tcapi_get("XPON_Common", "phyStatus", tempphyStatus);
	tcapi_get("XPON_Common", "trafficStatus", temptrafficStatus);

	if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){//ok
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
		goto err;
	}

	if(strstr(tempphyStatus, "up") == NULL)
	{
		strcpy(tempPonStatus, "PON_STATUS_NO_REG_NO_AUTH");
	}
	else
	{
		if(strstr(temptrafficStatus, "up") == NULL)
	 	{
			strcpy(tempPonStatus, "PON_STATUS_REG_NO_AUTH");
	 	}
		else
		{
			strcpy(tempPonStatus, "PON_STATUS_REG_AUTH");
		}
	}

	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_PONSTATUS, tempPonStatus)){
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
