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
#include "PingDiagnosticsRequest_func.h"
#include "threadpool.h"

static char pingDomainName[128] = {0};
static char gIPPingDev[16] = "NoDevice";
pthread_t p_ping = 0;
ping_t phone_ping_p;
static uint32 NumberOfRepetitions = 4;//add the default value for ip ping diagnostic,brian

void *cwmpPingTask(void)
{
	int rtAddStatus = 0;
	int ret = 0;
	int isRTFlag = 0;//0:IPPing rule is not in route table; 1:in route table; -1:IPPing rule error.
	char cmdbuf[200] = {'\0'};
	char sub_mask[20] = "255.255.255.255";
	char ipaddr[20] = {0};
	char *tmpbuf = NULL;
	char path[] ="/tmp/cwmp/ippingdiagnostic_stat";
	char cmd_rmRoute[200] = {'\0'};

	thePingFlag = 0;	
	strcpy(ipaddr,(char*)inet_ntoa(phone_ping_p.target));

	mobile_printf(MOBILE_DEBUG,"%s, line = %d, ipaddr = %s\n",__FUNCTION__, __LINE__, ipaddr);

	//judge if resolve host name error!
	if(phone_ping_p.target == 0)//resolve host name error
	{
		resolveHostError = -1;
	}
	else//judge if in route table
	{
		if(strcmp(gIPPingDev, "NoDevice") == 0)
		{		
			isRTFlag = -1;
			
		}else if(strcmp(gIPPingDev, "") == 0)
		{// default gateway
			isRTFlag = 1;// in route table
		}
		else
		{
			isRTFlag = isInRouteTable(ipaddr, gIPPingDev) ;
		}
	}
	
	//if not in route table ,add it------use in the future!!
	if((isRTFlag == 0) && (resolveHostError != -1))// not in route table then add it.
	{	
		if(strcmp(gIPPingDev, "br0") == 0)
		{   //add route for br0
			sprintf(cmdbuf,"/sbin/route add -net %s netmask %s dev %s", ipaddr, sub_mask, gIPPingDev);
			sprintf(cmd_rmRoute,"/sbin/route del -net %s netmask %s dev %s", ipaddr, sub_mask, gIPPingDev);
			//rtAddStatus = system(cmdbuf);
		}
		else
		{
			sprintf(cmdbuf, "/sbin/route add -host %s dev %s",ipaddr, gIPPingDev);
			sprintf(cmd_rmRoute, "/sbin/route del %s", ipaddr);
			//rtAddStatus = system(cmdbuf);
		}
#if EXEC_SYSTEM
		rtAddStatus= send2Exec(0x30, cmdbuf);
#else
		rtAddStatus = system(cmdbuf);
#endif
	}

	mobile_printf(MOBILE_DEBUG,"%s, line = %d, cmdbuf = %s\n",__FUNCTION__, __LINE__, cmdbuf);
		
	//adding route fail or illegal route for adding or resolve host name error
	memset(cmdbuf,0,sizeof(cmdbuf));
	if((rtAddStatus < 0) || (isRTFlag == -1) || (resolveHostError == -1))
	{
		mobile_printf(MOBILE_DEBUG,"\r\nenter ip ping error!");
	}
	else
	{
		//do ping 
		thePingFlag = 1;
		phone_ping_p.interval = 0;//default;The reserved parameters
		phone_ping_p.dscp = 0;//default;The reserved parameters

		if((phone_ping_p.interval == 0) && (phone_ping_p.len == 0))
		{
			sprintf(cmdbuf,"ping -q -M 1 -c %lu -Q %lu %s > %s",NumberOfRepetitions,phone_ping_p.dscp,ipaddr,path);
		}
		else if((phone_ping_p.interval == 0) && (phone_ping_p.len != 0))
		{
			sprintf(cmdbuf,"ping -q -M 1 -c %d -s %u -Q %lu %s > %s",NumberOfRepetitions,phone_ping_p.len,phone_ping_p.dscp,ipaddr,path);
		}
		else if((phone_ping_p.interval != 0) && (phone_ping_p.len == 0))
		{
			sprintf(cmdbuf,"ping -q -M 1 -c %d -W %lu -Q %lu %s > %s",NumberOfRepetitions,phone_ping_p.interval/1000,phone_ping_p.dscp,ipaddr,path);
		}
		else
		{
			sprintf(cmdbuf,"ping -q -M 1 -c %d -s %u -W %lu -Q %lu %s > %s",NumberOfRepetitions,phone_ping_p.len,phone_ping_p.interval/1000,phone_ping_p.dscp,ipaddr,path);
		}
		//exec ping
		//ret = system(cmdbuf);
#if EXEC_SYSTEM
		ret= send2Exec(0x30, cmdbuf);
#else
		ret = system(cmdbuf);
#endif
		mobile_printf(MOBILE_ERR,"Ping task is end, ret = %d!!\n",ret);
    }
	
	//delete route added before
	if( (strlen(cmd_rmRoute) != 0) && ( rtAddStatus == 0) ){
		//system(cmd_rmRoute);
#if EXEC_SYSTEM
		send2Exec(0x30, cmd_rmRoute);
#else
		system(cmd_rmRoute);
#endif
	}

	mobile_printf(MOBILE_DEBUG,"Ping task is end, line = %d, cmdbuf = %s\n", __LINE__, cmdbuf);

	//reset all flag.
	rtAddStatus = 0;
	isRTFlag = 0;
	thePingFlag = 0;

	/*Set p_ping varible to 0 means thread exit*/
	p_ping = 0;
	return NULL;
	
}

int doCwmpPing(void)
{
	ip4a dnsQueryResult;
	char cmdbuf[200]  = {0};
	int ret = -1;
	#define IPPING_DNSURL "/tmp/ippingdiag.conf"

	DomainNameError = 0;
	resolveHostError = 0;

	mobile_printf(MOBILE_DEBUG,"%s, line = %d, pingDomainName = %s\n",__FUNCTION__, __LINE__, pingDomainName);

	if(strcmp(pingDomainName, "") != 0)
	{
		memset(cmdbuf, 0, sizeof(cmdbuf));
		//sprintf(cmdbuf, "/bin/echo \"%s %s\" > %s", pingDomainName, gIPPingDev, IPPING_DNSURL);
		//system(cmdbuf);
		sprintf(cmdbuf, "\"%s %s\"", pingDomainName, gIPPingDev);
		fileWrite(IPPING_DNSURL, cmdbuf, strlen(cmdbuf));

		dnsQueryResult = resolveName(pingDomainName);

		mobile_printf(MOBILE_DEBUG,"%s, line = %d, dnsQueryResult = %d\n",__FUNCTION__, __LINE__, dnsQueryResult);

		unlink(IPPING_DNSURL);
		if(!dnsQueryResult)
		{
			phone_ping_p.target = 0L;
			resolveHostError = -1;
			ret = -1;
		}
		else
		{
			if(resolve(inet_ntoa(dnsQueryResult),&phone_ping_p.target,NULL) != 0)
			{
				phone_ping_p.target = 0L;
				resolveHostError = -1;
				ret = -1;
			}
			else
			{
				if(p_ping == 0)
				{
					mobile_printf(MOBILE_DEBUG,"\r\ncreate thread to do ip ping diagnostic!");
					//ret = cwmpPingTaskStart();
					ret = ThreadPoolAddWork(&gThreadPool, cwmpPingTask, NULL);
				}
				else
				{
					mobile_printf(MOBILE_DEBUG,"\r\n----------------------------p_ping is not 0, error occured!-------------------------------");
					ret = -1;
				}
			}
		}
	}
	else
	{
		phone_ping_p.target = 0L;
		DomainNameError = -1;
		ret = -1;
	}
	
	return ret;
}

json_t* PingDiagnosticsRequest(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	char *cWanName = NULL, *cWanName2 = NULL, *temppingDomainName = NULL, *templength = NULL, *temptime = NULL;
	char nodeName[48] = {0}, str_ifIdx[4] = {0}, str_servicelist[64] = {0}, str_vid[6] = {0}, str_mode[16] = {0};
	char cur_servicelist[64] = {0}, cur_wanmode[16] = {0}, cur_vid[6] = {0};
	unsigned int pvc_index = 0, entry_idx = 0;
	char buffer[128] = {0};
	int ret = -1, flag = 0;

	temppingDomainName	= ParameterSearchValue(req_para, "Dest");
	cWanName = ParameterSearchValue(req_para, "WANName");
	templength = ParameterSearchValue(req_para, "leng");
	temptime = ParameterSearchValue(req_para, "time");
	
	if((temppingDomainName != NULL) && (cWanName != NULL) && (templength != NULL) && (temptime != NULL))
	{
		phone_ping_p.len = atoi(templength);
		NumberOfRepetitions = atoi(temptime);
		
		for ( pvc_index = 0; pvc_index < MAX_PVC_NUM; pvc_index++ )
		{
			for ( entry_idx = 0; entry_idx < MAX_SMUX_NUM; entry_idx ++ )
			{
				sprintf(nodeName, "Wan_PVC%d_Entry%d", pvc_index, entry_idx);
				if(tcapi_get(nodeName, "IFIdx", str_ifIdx)) continue;				
				if(tcapi_get(nodeName, "ServiceList", cur_servicelist)) 	continue;				
				if(tcapi_get(nodeName, "WanMode", cur_wanmode)) continue;
				
				tcapi_get(nodeName, "VLANID", cur_vid);
				sprintf(buffer,"%s_%s_%c_VID_%s", str_ifIdx, cur_servicelist, cur_wanmode[0], cur_vid);
			
				if(!strcmp(buffer,cWanName))
				{
					tcapi_get(nodeName, "IFName", gIPPingDev);
					flag = 1;
					break;
				}
			}
			if(flag){
				flag = 0;
				break;
			}
		}

		memcpy(pingDomainName, temppingDomainName , sizeof(pingDomainName));
		
		mobile_printf(MOBILE_DEBUG,"%s, line = %d, pingDomainName = %s, gIPPingDev = %s\n",__FUNCTION__, __LINE__, pingDomainName, gIPPingDev);
		
		ret = doCwmpPing();	
	}
	else
	{
		ret = -1;
	}

	if(ret != -1){
		//build parameter
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){//ok
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	else{
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "PING_DIAG_REQ_NO_ROUTE")){
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

