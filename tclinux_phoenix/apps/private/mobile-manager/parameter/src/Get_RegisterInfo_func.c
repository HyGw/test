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
#include "Get_RegisterInfo_func.h"
int iRetryNumber20, iRetryNumber40, iRetryNumber30, iRegisterCmdFlag, iRegisterTime;

json_t* Get_RegisterInfo(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	char tempStatus[16] = {0};
	char tempResult[16] = {0};
	char tempserviceNum[16] = {0};
	char tempserviceName[64] = {0};
	char tempphyStatus[20] = {0};
	char temptrafficStatus[20] = {0};
	char tempRetryLimit[16] = {0};
	char tempRetryTimes[16] = {0};
	char tempRemainTimes[16] = {0};
	char tempifname[10] = {0};
	char tempInformStatus[10] = {0};
	int iRetryLimit = 0, iRetryTimes = 0, iRemainTimes = 0, cur_serviceNum = 0, icurrentTime = 0;
	char cRegisterStatus[60] = {0}, cRegisterResult[16] = {0}, tempnewStatus[16] = {0}, tempnewResult[16] = {0};
	struct tm *tblock;
	time_t timenow;

	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__,__LINE__);

	if(1 == iRegisterCmdFlag)
	{
		timenow = time(NULL);	
		tblock = localtime(&timenow);
		icurrentTime = tblock->tm_hour * 3600 + tblock->tm_min *60 + tblock->tm_sec;
	}

	if((1 == iRegisterCmdFlag) && (abs(icurrentTime - iRegisterTime) < 300))
	{
		tcapi_get("XPON_Common", "phyStatus", tempphyStatus);

		if(abs(icurrentTime - iRegisterTime) < 15)
			strcpy(temptrafficStatus, "down");
		else
		tcapi_get("XPON_Common", "trafficStatus", temptrafficStatus);
	}
	else{
		iRegisterCmdFlag = 0;
		iRetryNumber20 = 60;
		iRetryNumber30 = 60;
		iRetryNumber40 = 60;
		strcpy(tempphyStatus, "up");
		strcpy(temptrafficStatus, "up");
	}

	if(strstr(tempphyStatus, "up") == NULL)
	{
		iRegisterCmdFlag = 0;
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){//ok
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, "RegisterStatus", "REGISTER_OLT_FAIL")){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	else if( (strstr(tempphyStatus, "up") != NULL) && (strstr(temptrafficStatus, "up") == NULL) )
	{
		iRetryNumber20++;

		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){//ok
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}

		if(iRetryNumber20 > 25){
			iRegisterCmdFlag = 0;
			iRetryNumber20 = 0;
			if(JSON_OK != ParameterAddNewJSONChild(reply_para, "RegisterStatus", "REGISTER_OLT_FAIL")){
				mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
				goto err;
			}
		}
		else{
			if(JSON_OK != ParameterAddNewJSONChild(reply_para, "RegisterStatus", "REGISTER_OLT")){
				mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
				goto err;
			}
			if(JSON_OK != ParameterAddNewJSONChild(reply_para, "RegisterResult", "20")){
				mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
				goto err;
		}
	}
	}
	else if( (strstr(tempphyStatus, "up") != NULL) && (strstr(temptrafficStatus, "up") != NULL) )
	{
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){//ok
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}

		tcapi_get( "deviceAccount_Entry", "registerStatus", tempStatus );
		tcapi_get( "deviceAccount_Entry", "registerResult", tempResult );
		tcapi_get( "deviceAccount_Entry", "retryLimit", tempRetryLimit );
		tcapi_get( "deviceAccount_Entry", "retryTimes", tempRetryTimes );
		tcapi_get( "deviceAccount_Entry", "newStatus", tempnewStatus );
		tcapi_get( "deviceAccount_Entry", "newResult", tempnewResult );
		tcapi_get( "CwmpRoute_Entry","ifName0",tempifname);	
		tcapi_get( "Cwmp_Entry","Inform_Status",tempInformStatus);

		if(1 == iRegisterCmdFlag){
			if(strcmp(tempnewStatus, "1") != 0)
				strcpy(tempStatus, "99");		
			if(strcmp(tempnewResult, "1") != 0)
				strcpy(tempResult, "99");
		}

		iRetryLimit = atoi(tempRetryLimit);
		iRetryTimes = atoi(tempRetryTimes);
		iRemainTimes = iRetryLimit - iRemainTimes;
		sprintf(tempRemainTimes, "%d", iRemainTimes);
		
		if((strcmp(tempStatus, "99") == 0) || (strlen(tempStatus) == 0))
		{
			if(((strcmp(tempifname, "N/A") == 0) || (strlen(tempifname)==0)))
			{
				iRetryNumber30++;
				if(JSON_OK != ParameterAddNewJSONChild(reply_para, "RegisterStatus", "REGISTER_OLT")){
						mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
						goto err;
				}
				if(JSON_OK != ParameterAddNewJSONChild(reply_para, "RegisterResult", "30")){
						mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
						goto err;
				}
			}
			else if( (strcmp(tempifname, "N/A") != 0) && (strlen(tempifname) != 0))
			{
				iRetryNumber40++;

				if(JSON_OK != ParameterAddNewJSONChild(reply_para, "RegisterStatus", "REGISTER_OLT")){
						mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
						goto err;
				}
				if(JSON_OK != ParameterAddNewJSONChild(reply_para, "RegisterResult", "40")){
						mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
						goto err;
				}
			}
			else if((strcmp(tempInformStatus, "1") != 0) && (strcmp(tempInformStatus, "2") != 0) )
			{
				iRetryNumber40++;
				if(JSON_OK != ParameterAddNewJSONChild(reply_para, "RegisterStatus", "REGISTER_OLT")){
						mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
						goto err;
				}
				if(JSON_OK != ParameterAddNewJSONChild(reply_para, "RegisterResult", "40")){
						mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
						goto err;
				}
			}
			else
			{
				//iRegisterCmdFlag = 0;
				//iRetryNumber30 = 0;
				//iRetryNumber40 = 0;
				if(JSON_OK != ParameterAddNewJSONChild(reply_para, "RegisterStatus", "REGISTER_DEFAULT")){
						mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
						goto err;
				}
			}
		}
		else if(strcmp(tempStatus, "0") == 0)
		{
			if(strcmp(tempResult, "99") == 0)
			{
				if(JSON_OK != ParameterAddNewJSONChild(reply_para, "RegisterStatus", "REGISTER_OK_DOWN_BUSINESS")){
						mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
						goto err;
				}
				if(JSON_OK != ParameterAddNewJSONChild(reply_para, "RegisterProgressResult", "50")){
					mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
					goto err;
				}
				if(JSON_OK != ParameterAddNewJSONChild(reply_para, "RegisterBussNameResult", "")){
					mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
					goto err;
				}
			}
			else if(strcmp(tempResult, "0") == 0)
			{
				tcapi_get( "deviceAccount_Entry", "serviceNum", tempserviceNum );
				cur_serviceNum = atoi(tempserviceNum);
				if(cur_serviceNum <= 0)
				{
					if(JSON_OK != ParameterAddNewJSONChild(reply_para, "RegisterStatus", "REGISTER_OK_DOWN_BUSINESS")){
						mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
						goto err;
					}
					if(JSON_OK != ParameterAddNewJSONChild(reply_para, "RegisterProgressResult", "60")){
						mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
						goto err;
					}
					if(JSON_OK != ParameterAddNewJSONChild(reply_para, "RegisterBussNameResult", "")){
						mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
						goto err;
					}
				}
				else
				{
					tcapi_get( "deviceAccount_Entry", "serviceName", tempserviceName );
					if(JSON_OK != ParameterAddNewJSONChild(reply_para, "RegisterStatus", "REGISTER_OK_DOWN_BUSINESS")){
						mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
						goto err;
					}
					if(JSON_OK != ParameterAddNewJSONChild(reply_para, "RegisterProgressResult", "70")){
						mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
						goto err;
					}
					if(JSON_OK != ParameterAddNewJSONChild(reply_para, "RegisterBussNameResult", tempserviceName)){
						mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
						goto err;
					}
				}	
			}
			else if(strcmp(tempResult, "1") == 0)
			{
				iRegisterCmdFlag = 0;
				tcapi_get( "deviceAccount_Entry", "serviceName", tempserviceName );
				if(JSON_OK != ParameterAddNewJSONChild(reply_para, "RegisterStatus", "REGISTER_OK")){
					mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
					goto err;
				}
				if(JSON_OK != ParameterAddNewJSONChild(reply_para, "RegisterServiceResult", tempserviceName)){
					mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
					goto err;
				}
			}
			else if(strcmp(tempResult, "2") == 0)
			{
				iRegisterCmdFlag = 0;
				if(JSON_OK != ParameterAddNewJSONChild(reply_para, "RegisterStatus", "REGISTER_POK")){
					mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
					goto err;
				}
			}
		}
		else if(strcmp(tempStatus, "1") == 0)
		{
			iRegisterCmdFlag = 0;
			if(iRetryTimes < iRetryLimit)
			{
				if(JSON_OK != ParameterAddNewJSONChild(reply_para, "RegisterStatus", "REGISTER_NOACCOUNT_NOLIMITED")){
					mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
					goto err;
				}
				if(JSON_OK != ParameterAddNewJSONChild(reply_para, "RegisterServiceResult", tempRemainTimes)){
					mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
					goto err;
				}
			}
			else
			{
				if(JSON_OK != ParameterAddNewJSONChild(reply_para, "RegisterStatus", "REGISTER_NOACCOUNT_LIMITED")){
					mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
					goto err;
				}
			}
		}
		else if(strcmp(tempStatus, "2") == 0)
		{
			iRegisterCmdFlag = 0;
			if(iRetryTimes < iRetryLimit)
			{
				if(JSON_OK != ParameterAddNewJSONChild(reply_para, "RegisterStatus", "REGISTER_NOUSER_NOLIMITED")){
					mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
					goto err;
				}
				if(JSON_OK != ParameterAddNewJSONChild(reply_para, "RegisterServiceResult", tempRemainTimes)){
					mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
					goto err;
				}
			}
			else
			{
				if(JSON_OK != ParameterAddNewJSONChild(reply_para, "RegisterStatus", "REGISTER_NOUSER_LIMITED")){
					mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
					goto err;
				}
			}
		}
		else if(strcmp(tempStatus, "3") == 0)
		{			
			iRegisterCmdFlag = 0;
			if(iRetryTimes < iRetryLimit)
			{
				if(JSON_OK != ParameterAddNewJSONChild(reply_para, "RegisterStatus", "REGISTER_NOMATCH_NOLIMITED")){
					mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
					goto err;
				}
				if(JSON_OK != ParameterAddNewJSONChild(reply_para, "RegisterResult", tempRemainTimes)){
					mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
					goto err;
				}
			}
			else
			{
				if(JSON_OK != ParameterAddNewJSONChild(reply_para, "RegisterStatus", "REGISTER_NOMATCH_LIMITED")){
					mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
					goto err;
				}
				if(JSON_OK != ParameterAddNewJSONChild(reply_para, "RegisterResult", "")){
					mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
					goto err;
				}
			}
		}
		else if(strcmp(tempStatus, "4") == 0)
		{
			iRegisterCmdFlag = 0;
			if(JSON_OK != ParameterAddNewJSONChild(reply_para, "RegisterStatus", "REGISTER_TIMEOUT")){
				mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
				goto err;
			}
		}
		else if(strcmp(tempStatus, "5") == 0)
		{
			iRegisterCmdFlag = 0;
			if(JSON_OK != ParameterAddNewJSONChild(reply_para, "RegisterStatus", "REGISTER_REGISTED")){
				mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
				goto err;
			}
		}
	}
	else{
		iRegisterCmdFlag = 0;
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "Query the Registration has a problem")){
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

