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
#include "Get_CpuUsage_func.h"
#if defined(TCSUPPORT_CT_JOYME)
#include "joyme.h"
#endif

json_t* GetCpuUsage(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	char CpuUsagevalue[8] = {0};
	int ret = -1,percent;

	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__, __LINE__);

#if defined(TCSUPPORT_CT_JOYME)
	ret = joyme_sysGetCpuUsage(&percent);
	sprintf(CpuUsagevalue,"%d",percent);
#else
	//Calculate CPU Usage
	ret = CalcuCpuUsage(CpuUsagevalue);
#endif

	//build parameter
	if(ret == -1){
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "Cpustat file Is Not Exist")){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	else{//Calculate CPU Usage Success
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_PERCENT, CpuUsagevalue)){
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

	if(reply_para) ParameterFreeJSON(&reply_para);
	return reply_root;
	
err:
	if(reply_root) ParameterFreeJSON(&reply_root);
	if(reply_para) ParameterFreeJSON(&reply_para);
	return NULL;
}

int CalcuCpuUsage(char *CpuUsage){
	uint32 curtotal = 0, curidle = 0;
	uint32 totalnew = 0, totalold = 0;
	uint32 idlenew = 0, idleold = 0;
	int ret = 0;

	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n", __FUNCTION__, __LINE__);

	ret=GetCpuUsageTOT_IDLE(&totalold, &idleold);
	if(ret == -1){
		return -1;
	}

	mobile_printf(MOBILE_DEBUG,"totalold=%d, idleold = %d\n", totalold, idleold);

	sleep(1);
	
	ret=GetCpuUsageTOT_IDLE(&totalnew, &idlenew);
	if(ret == -1){
		return -1;
	}
	
	mobile_printf(MOBILE_DEBUG,"totalnew=%d, idlenew = %d\n", totalnew, idlenew);

	curtotal = totalnew - totalold;
	curidle = idlenew - idleold;

	if(0 == curtotal)
		return -1;

    sprintf(CpuUsage,"%d",(curtotal - curidle)*100/curtotal);
 
	return 0;
}

int GetCpuUsageTOT_IDLE(unsigned int *total, unsigned int *idle){
	static struct stats stat;
	char value[80] = {0};
	char temp[80] = {0};

	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__, __LINE__);

	memset(value,0, sizeof(value));
	memset(temp,0, sizeof(temp));	
	fileRead("/proc/stat", temp, sizeof(temp));
	if(strlen(temp) == 0)
		return -1;
	sscanf(temp, "%s %d %d %d %d %d %d %d", value, &stat.user, &stat.nice,&stat.system, 
			&stat.idle, &stat.iowait, &stat.irq, &stat.softirq);
	stat.total = stat.user + stat.nice + stat.system + stat.idle + stat.iowait + stat.irq + stat.softirq;

	*total = stat.total;
	*idle = stat.idle;

	return 0;
}
