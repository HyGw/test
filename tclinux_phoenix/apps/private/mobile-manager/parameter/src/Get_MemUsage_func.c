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
#include "Get_MemUsage_func.h"
#if defined(TCSUPPORT_CT_JOYME)
#include "joyme.h"
#endif
#define  MEMINFO_FILE  "/proc/meminfo"


json_t* GetMemUsage(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	char MemUsagevalue[8] = {0};
	int ret = -1,percent;

	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__, __LINE__);

#if defined(TCSUPPORT_CT_JOYME)
	ret = joyme_sysGetMemUsage(&percent);
	sprintf(MemUsagevalue,"%d",percent);	
#else
	//Calculate Memory Usage
	ret=memory_read(MemUsagevalue);
#endif

	//build parameter
	if(ret == -1){
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "Meminfo file Is Not Exist")){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	else{//Calculate Memory Usage Success
		mobile_printf(MOBILE_DEBUG,"line = %d,MemUsage = %s\n",__LINE__,MemUsagevalue);

		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){//ok
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_Percent, MemUsagevalue)){
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

int memory_read(char *memoryUsage){
    FILE *fp = NULL;
    char buf[40] = {0};
    char temp[40] = {0};
    uint32 memTotal = 0;
    uint32 memFree = 0;
    uint32 buffers = 0;
    uint32 cached = 0;
    int i = 0;
    
    fp = fopen(MEMINFO_FILE,"r");
    if(NULL == fp)
    {       
        mobile_printf(MOBILE_DEBUG,"\r\nopen meminfo file error!");
        return -1;
    }

    while( !(feof(fp)) && (i<4) )
    {
        memset(buf,0, sizeof(buf));
        memset(temp,0, sizeof(temp));
        fgets(buf , sizeof(buf) , fp);
        sscanf(buf,"%*s%s",temp);
        if(strstr(buf,"MemTotal:") != NULL){
            memTotal = atoi(temp);
        } else if(strstr(buf,"MemFree:") != NULL){
            memFree = atoi(temp);
        } else if(strstr(buf,"Buffers:") != NULL){
            buffers = atoi(temp);
        } else if(strstr(buf,"Cached:") != NULL){
            cached = atoi(temp);
        } else {
            mobile_printf(MOBILE_DEBUG,"memory_read can't find the key word!\n");
        }
        
        i++;
    }
    sprintf(memoryUsage,"%d",((memTotal-memFree-buffers-cached)*100)/memTotal);
    fclose(fp);
    return 0;
}


