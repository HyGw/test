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
#include "Get_HW_IP_func.h"

#if defined(TCSUPPORT_CT_JOYME)
json_t* Get_HW_IP_func(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	char lanip[16] = {0},tempbuf[64] = {0}, wanip[16],ipv6wanip[64] = {0},nodename[64] = {0};
	

	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__,__LINE__);

	memset(lanip,0,sizeof(lanip));
	tcapi_get("Lan_Entry0","IP",lanip);

	tcapi_get("WanInfo_Common","DefRouteIndexv4",tempbuf);
	if(!strcmp(tempbuf,"N/A")){
		strcpy(wanip,"0.0.0.0");
	}
	else{
		sprintf(nodename,"WanInfo_Entry%d",atoi(tempbuf));
		tcapi_get(nodename,"Status",tempbuf);
		if(!strcmp(tempbuf,"up")){
			tcapi_get(nodename,"IP",wanip);
		}
		else{
			strcpy(wanip,"0.0.0.0");
		}
	}
	memset(tempbuf,0,sizeof(tempbuf));
	tcapi_get("WanInfo_Common","DefRouteIndexv6",tempbuf);
	if(!strcmp(tempbuf,"N/A")){
		strcpy(ipv6wanip,"0:0:0:0:0:0:0:0");
	}
	else{
		sprintf(nodename,"WanInfo_Entry%d",atoi(tempbuf));
		tcapi_get(nodename,"Status6",tempbuf);
		if(!strcmp(tempbuf,"up")){
			tcapi_get(nodename,"IP6",ipv6wanip);
		}
		else{
			strcpy(wanip,"0:0:0:0:0:0:0:0");
		}
	}
	
	//build parameter
	if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){//ok
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, WANIPADDRESS, wanip)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, IPV6_WANIPADDRESS, ipv6wanip)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, LANIPADDRESS, lanip)){
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
#endif
