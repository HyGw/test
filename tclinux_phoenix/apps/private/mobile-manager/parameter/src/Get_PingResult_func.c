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
#include "Get_PingResult_func.h"
#include "PingDiagnosticsRequest_func.h"

json_t* Get_PingResult(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	char PingResult[16] = {0}, PingStatus[16] = {0};
	uint32 sent = 0;	/*Total number of sent*/
	uint32 responses = 0;/*Total number of responses*/
	uint32 maxrtt = 0;	/*Maximun rtt*/
	uint32 minrtt = 0;	/*Minimun rtt*/
	uint32 avrrtt = 0;	/*Average rtt*/
	int ret = 0;
	FILE *fp = NULL;

	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__, __LINE__);
	
	if(thePingFlag == 1)
	{
		strcpy(PingStatus,"Request");
		strcpy(PingResult,"Ping Diagnosics is in progress");
	}
	else if(thePingFlag == 0)
	{
		fp = fopen(IPPINGDIAGNOSTICFILE,"r");
		if(fp == NULL)
		{
			ret = -1;	
		}
		else
		{
			fscanf(fp,"%ld,%ld,%lu,%lu,%lu",&sent,&responses,&minrtt,&avrrtt,&maxrtt);
			fclose(fp);
		}
		unlink(IPPINGDIAGNOSTICFILE);

		if( (sent > responses) || (DomainNameError == -1) || (resolveHostError == -1) )
		{
			strcpy(PingStatus,"Unreach");
			if(DomainNameError == -1)
				strcpy(PingResult,"Error_PingAddressWrong");
			else if(resolveHostError == -1)
				strcpy(PingResult,"Error_CannotResolveHostName");
			else
				strcpy(PingResult,"Error_Other");
		}
		else if(responses > 0)
		{
			strcpy(PingStatus,"Complete");		
			
			sprintf(PingResult, "%lu", avrrtt);
		}
	}
	else
	{
		ret = -1;
		strcpy(PingResult,"Ping Is Not Executed");
	}

	//build parameter
	if(ret == -1){
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, PingResult)){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	else{//Calculate CPU Usage Success
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){//ok
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_PINGSTATUS, PingStatus)){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_PINGRESULT, PingResult)){
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
