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
#include "Get_PPP_DialStatus_func.h"
#include "Set_DownHttpReq_func.h"
#include "Set_HttpCfg_func.h"
#include "threadpool.h"
#include "info.h"
#include "exec.h"

int httpcon_time = 0;
char Wan_InfoName[30] = {0};

void * wan_rxbytes (worker_data_t *argv);
int GetWanStatus(char *wanifname);
void stopdownload();

void speedup(void){
	char buffer[128];
	memset(buffer, 0, sizeof(buffer));
	sprintf(buffer, "echo 524288 > /proc/sys/net/core/rmem_max");
	system(buffer);
	memset(buffer, 0, sizeof(buffer));
	sprintf(buffer, "echo 524288 > /proc/sys/net/core/rmem_default");
	system(buffer);
	memset(buffer, 0, sizeof(buffer));
	sprintf(buffer, "echo \"87380 238144 524288\" > /proc/sys/net/ipv4/tcp_rmem");
	system(buffer);
}

json_t* SetDownHttpReq(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	char *temp_method = NULL, *temp_time = NULL;
	int ret = 1;
	char *temp = NULL;
	int len = 0, threadlinkcount = 4, proclinkcount = 0, method_flag = 0;
	char tempstr[10] = {0};
	int SystemStatus = 0;
	const char* req_err_string[] = {
		"Method_Or_Time_ERROR",
		"Method_ERROR",
		"Time_ERROR",
		"No Wan Connected",
		"Please set dowload URL first!!",
		"HTTP_DOWNLOAD_REQUEST_ERROR",
		NULL	
	};

	if(HttpReqUrl)
	{
		temp_method = ParameterSearchValue(req_para, "Method");
		temp_time = ParameterSearchValue(req_para, "Time");
		
		if((temp_method != NULL) && (temp_time != NULL))
		{
			if(strcmp(temp_method, "Start") == 0)
			{
				httpcon_time = atoi(temp_time);
				if((httpcon_time > 0) && (httpcon_time <= 30))
				{
					stopdownload();
					memset(Wan_InfoName, 0, sizeof(Wan_InfoName));
					if(GetWanStatus(Wan_InfoName) == 0 && HttpReqUrl)
					{
						len = strlen(HttpReqUrl) + 80;
						temp = malloc(len);
						if(NULL == temp) {
							mobile_printf(MOBILE_ERR, "%s: malloc err!...\n", __FUNCTION__);
							return NULL;
						}

						tcapi_set(MOBILE_ENTRY, "RealTimeRate", "");

						//speedup();
						memset(temp, 0, sizeof(temp));
						memset(tempstr, 0, sizeof(tempstr));
						if(!tcapi_get(MOBILE_ENTRY, "methodflag", tempstr))
							method_flag = atoi(tempstr);
						if(method_flag == 1){
							sprintf(temp, "/userfs/bin/wget --delete-after --rate-testtime %d -b -O /tmp/httpdownfile %s", httpcon_time+WGET_WAITTIME, HttpReqUrl);
						}
						else
						{
							memset(tempstr, 0, sizeof(tempstr));
							if(!tcapi_get(MOBILE_ENTRY, "threadcount", tempstr))
								threadlinkcount = atoi(tempstr);
							if((threadlinkcount < 1) || (threadlinkcount > THREAD_MAX_COUNT))
								threadlinkcount = THREAD_DEF_COUNT;
						}
						
						memset(tempstr, 0, sizeof(tempstr));
						if(!tcapi_get(MOBILE_ENTRY, "processcount", tempstr))
							proclinkcount = atoi(tempstr);
						if((proclinkcount < 1) || (proclinkcount > PROC_MAX_COUNT))
							proclinkcount = PROC_DEF_COUNT;
						
						do {
							if(method_flag != 1)
							{
								memset(temp, 0, sizeof(temp));
								sprintf(temp, "/userfs/bin/axel -q -w -n %d -o /tmp/httpdownfile%d %s &",threadlinkcount,proclinkcount,HttpReqUrl);
							}
					#if EXEC_SYSTEM
							SystemStatus = send2Exec(0x30, temp);
							if(SystemStatus != EXEC_PROCESS_OK)
							{
								ret = 6;
								break;
							}
					#else
							system(temp);
					#endif
							
						}while((--proclinkcount) > 0);
						
						if(free) free(temp);
						if(ret != 6)
						{
							ThreadPoolAddWork(&gThreadPool, wan_rxbytes, NULL);
							ret = 0;
						}
					}
					else
						ret = 4;
				}
				else
					ret = 3;
			}
			else
				ret = 2;
		}
	}
	else	
		ret = 5;
	
	//build parameter
	if(ret == 0)
	{
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){
			mobile_printf(MOBILE_ERR,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}			
	}
	else
	{
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_ERR,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}			
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, req_err_string[ret-1])){
			mobile_printf(MOBILE_ERR, "%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
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

int GetWanStatus(char *wanifname)
{
	unsigned long wanNodeIndexv4 = 0, pvcNodeEntryv4 = 0, wanNodeIndexv6 = 0, pvcNodeEntryv6 = 0;
	int IPv4ret = -1, IPv6ret = -1, defRouterIfv4 = -1, defRouterIfv6 = -1;
	char ipver[16] = {0}, str_ip6[64] = {0};
	char status[10] = {0}, IPv6status[12] = {0}, str_wanIp[128] = {0};
	char nodeName[34] = {0};
	char nodeWanName[34] = {0};

	getISPIndex(&defRouterIfv4, &defRouterIfv6);

	if(defRouterIfv4 >= 0)
	{
		wanNodeIndexv4 = defRouterIfv4 / MAX_PVC_NUM;
		pvcNodeEntryv4 = defRouterIfv4 % MAX_PVC_NUM;

		sprintf(nodeWanName, "Wan_PVC%d_Entry%d", wanNodeIndexv4, pvcNodeEntryv4);
		tcapi_get(nodeWanName, "IPVERSION", ipver);
		
		if ( 0 == strcmp(ipver, "IPv4") || 0 == strcmp(ipver, "IPv4/IPv6"))
		{
			sprintf(nodeName, "WanInfo_Entry%d", wanNodeIndexv4<<3|pvcNodeEntryv4);
			tcapi_get(nodeName, "Status", status);
			if( 0 == strcmp(status,"up"))
			{
				tcapi_get(nodeName, "IP", str_wanIp);
				tcapi_get(nodeWanName, "NASName", wanifname);
				if((strlen(str_wanIp) != 0) && (strstr(wanifname, "nas") != NULL))
					return 0;
			}
		}	
	}	
	
	if(defRouterIfv6 >= 0)
	{
		wanNodeIndexv6 = defRouterIfv6 / MAX_PVC_NUM;
		pvcNodeEntryv6 = defRouterIfv6 % MAX_PVC_NUM;	
		
		sprintf(nodeWanName, "Wan_PVC%d_Entry%d", wanNodeIndexv6, pvcNodeEntryv6);
		tcapi_get(nodeWanName, "IPVERSION", ipver);
		if ( 0 == strcmp(ipver, "IPv6") || 0 == strcmp(ipver, "IPv4/IPv6") )
		{
			sprintf(nodeName, "WanInfo_Entry%d", ((wanNodeIndexv6<<3)|pvcNodeEntryv6));
			tcapi_get(nodeName, "Status6", IPv6status);
			if ( 0 == strcmp(IPv6status, "up") )
			{
				tcapi_get(nodeName, "IP6", str_ip6);
				tcapi_get(nodeWanName, "NASName", wanifname);
				if((strlen(str_ip6) != 0) && (strstr(wanifname, "nas") != NULL))
					return 0;
			}
		}
	}
	
	return -1;
}

void stopdownload(){
	char tempstr[10] = {0};
	int ret = 0;
	
	memset(tempstr, 0, sizeof(tempstr));
	tcapi_get(MOBILE_ENTRY, "methodflag", tempstr);
	if(atoi(tempstr) != 1)
	{
#if EXEC_SYSTEM
		ret = send2Exec(0x30, "killall -9 axel");
		send2Exec(0x30, "rm /tmp/httpdownfile*");
#else
		ret = system("killall -9 axel");
		system("rm /tmp/httpdownfile*");
#endif
	}
	mobile_printf(MOBILE_ERR,"%s: exec=%d,%d thread 0x%x!...\n", __FUNCTION__, ret, errno,pthread_self());
}

void * wan_rxbytes (worker_data_t *argv)
{
	int i = 0, count = 0;
	char *pt = NULL;
	char rxbytes[20] = {0};
	unsigned long int temp=0, rxtemp=0, rxpre=0;
	char realtimeratestr[256] = {0};
	char nodeName[34] = {0}, nasnode[34] = {0};
	char tempstr[10] = {0};
	int SystemStatus = 0;

	strcpy(nasnode, Wan_InfoName);
	if((pt=strstr(nasnode, "_")) != NULL )
		*pt = '.';	
	sprintf(nodeName, "Info_%s", nasnode);
	tcapi_get(nodeName, "rxbytes", rxbytes);
	rxpre = strtoul(rxbytes,NULL, 10);
	for(i=0; i<(httpcon_time+WGET_WAITTIME); i++)
	{
		sleep(1);
		memset(rxbytes, 0, sizeof(rxbytes));
		tcapi_get(nodeName, "rxbytes", rxbytes);
		temp = strtoul(rxbytes,NULL, 10);
		if((temp > rxpre) && (((temp-rxpre)/1024) > 0))
		{
			rxtemp = temp-rxpre;
			rxpre = temp;
			count ++;
		}
		else
		{
			rxpre = temp;
			continue;
		}
		sprintf(rxbytes,"%lu", rxtemp/1024);
		strcat(realtimeratestr, rxbytes);
		strcat(realtimeratestr, ",");
		tcapi_set(MOBILE_ENTRY, "RealTimeRate", realtimeratestr);
		
		if(count >= httpcon_time)
			break;
	}
	stopdownload();

	return NULL;
}
