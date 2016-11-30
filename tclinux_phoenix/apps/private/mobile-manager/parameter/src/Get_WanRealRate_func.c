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
#include "Get_WanRealRate_func.h"
#include "Set_DownHttpReq_func.h"
#include "info.h"

void GetRateStr(char *buf);

void resetspeedup(void){
	char buffer[128];
	memset(buffer, 0, sizeof(buffer));
	sprintf(buffer, "echo 133120 > /proc/sys/net/core/rmem_max");
	system(buffer);
	memset(buffer, 0, sizeof(buffer));
	sprintf(buffer, "echo 133120 > /proc/sys/net/core/rmem_default");
	system(buffer);
	memset(buffer, 0, sizeof(buffer));
	sprintf(buffer, "echo \"4096 87380 238144\" > /proc/sys/net/ipv4/tcp_rmem");
	system(buffer);
}

json_t* GetWanRealRate(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	char realtimeratestr[256] = {0};

	mobile_printf(MOBILE_INFO,"%s:start!...\n",__FUNCTION__);
	
	GetRateStr(realtimeratestr);	
	//resetspeedup();
	//build parameter
	if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
		goto err;
	}	
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_REALTIMERATE, realtimeratestr)){
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

void GetRateStr(char *buf)
{
	char *p=NULL;
	char *delim=",";
	char rateall[20] = {0}, realtimerate[256] = {0}, ratetemp[256] = {0};
	int count = 0;
	unsigned long int rxall=0, temp=0, rxmax=0, rxaver=0;
	
	tcapi_get(MOBILE_ENTRY, "RealTimeRate", realtimerate);

	p=strtok(realtimerate,delim);
	while(p != NULL){
		temp = strtoul(p, NULL, 10);
		if(temp > 0)
		{
			rxall += temp;
			if(temp > rxmax)
				rxmax = temp;
			rxaver = rxall / (++count);
			
			sprintf(rateall,"%lu", rxall);
			if(count > 1)
				strcat(ratetemp, ",");
			strcat(ratetemp, rateall);
		}
		p = strtok(NULL,delim);
	}
	
	if(count < httpcon_time )
	{
		if(count > RATECOUNTMIN)
		{
			for(; count<httpcon_time; count++)
			{
				rxall += ((rxmax-rxaver)*random() + rxaver);
				sprintf(rateall,"%lu", rxall);
				if(count > 0)
					strcat(ratetemp, ",");
				strcat(ratetemp, rateall);
			}
		}
		else
		{
			count = 0;
			memset(ratetemp, 0, sizeof(ratetemp));
			while(count < httpcon_time)
			{
				if(count > 0)
					strcat(ratetemp, ",");
				strcat(ratetemp, "0");
				count ++;
			}			
		}
	}

	strncpy(buf, ratetemp, sizeof(ratetemp));
	
	return;
}
