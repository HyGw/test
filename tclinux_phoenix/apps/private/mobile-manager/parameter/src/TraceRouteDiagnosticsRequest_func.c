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
#include "TraceRouteDiagnosticsRequest_func.h"
#include "threadpool.h"
#include <netdb.h>
#include "exec.h"

static char gTraceRouteDev[16] = "NoDevice";
traceroute_t cwmp_traceroute_p;

void *cwmpTraceRouteTask (worker_data_t *argv)
{
	int resolveHostError = 0;
	int rtAddStatus = 0;
	int isRTFlag = 0;//0:trace route rule is not in route table; 1:in route table; -1:trace route rule error.
	char cmdbuf[200] = {'\0'};
	char sub_mask[20] = "255.255.255.255";
	uint32 tmpdatablockSize = 38;
	uint32  tmptimeout = 5000;
	uint32 tmpnumberoftries = 1;
	uint32 tmpmaxhopcount = 30;
	char cmd_rmRoute[200] = {0};
	theTraceRouteFlag = 0;
	char convertIP[64]={0};

	strcpy(convertIP,(char*)inet_ntoa(cwmp_traceroute_p.target));

	mobile_printf(MOBILE_DEBUG,"%s, line = %d, convertIP = %s\n",__FUNCTION__, __LINE__, convertIP);

	//judge if resolve host name error!
	if(cwmp_traceroute_p.target == 0)//resolve host name error
	{
		resolveHostError = -1;
	}
	else//judge if in route table
	{
		if(strcmp(gTraceRouteDev, "NoDevice") == 0)
		{		
			isRTFlag = -1;
			
		}
		else if(strcmp(gTraceRouteDev, "") == 0)
		{// default gateway
			isRTFlag = 1;// in route table
		}
		else//judge if in route table
		{
			isRTFlag = isInRouteTable(convertIP, gTraceRouteDev);	
		}
	}
	
	//if not in route table ,add it------use in the future!!
	if((isRTFlag == 0) && (resolveHostError != -1))// not in route table then add it.
	{
		if(strcmp(gTraceRouteDev, "br0")==0)
		{
			sprintf(cmdbuf, "/sbin/route add -net %s netmask %s dev %s", convertIP, sub_mask,gTraceRouteDev);
			sprintf(cmd_rmRoute, "/sbin/route del -net %s netmask %s dev %s", convertIP, sub_mask,gTraceRouteDev);
			//rtAddStatus = system(cmdbuf);
		}
		else
		{
			sprintf(cmdbuf, "/sbin/route add -host %s dev %s",convertIP, gTraceRouteDev);
			sprintf(cmd_rmRoute, "/sbin/route del %s", convertIP);			
			//rtAddStatus = system(cmdbuf);
		}		
		#if EXEC_SYSTEM
		rtAddStatus= send2Exec(0x30, cmdbuf);
		#else
		rtAddStatus = system(cmdbuf);
		#endif
		mobile_printf(MOBILE_DEBUG,"line = %d, rtAddStatus = %d, cmdbuf = %s\n", __LINE__, rtAddStatus, cmdbuf);
	}

	//adding route fail or illegal route for adding or resolve host name error
	memset(cmdbuf,0,sizeof(cmdbuf));
	if((rtAddStatus < 0) || (isRTFlag == -1) || (resolveHostError == -1))
	{
		mobile_printf(MOBILE_ERR, "enter trace route error!\n");
	}
	else
	{
		theTraceRouteFlag = 1;//The cwmp trace route is in progress.
		//do trace route
		//default value for trace route
		if(cwmp_traceroute_p.DataBlockSize)
			tmpdatablockSize = cwmp_traceroute_p.DataBlockSize;
		if(cwmp_traceroute_p.Timeout)
			tmptimeout = cwmp_traceroute_p.Timeout;
		if(cwmp_traceroute_p.NumberofTries)
			tmpnumberoftries = cwmp_traceroute_p.NumberofTries; 
		if(cwmp_traceroute_p.MaxHopCount)
			tmpmaxhopcount = cwmp_traceroute_p.MaxHopCount;

		sprintf(cmdbuf,"/usr/bin/traceroute -x 1 -m %lu -q %lu -w %lu %s %lu  > %s", tmpmaxhopcount, tmpnumberoftries, tmptimeout/1000, convertIP, tmpdatablockSize, TMP_PHONEAPP_TRACE_PATH);
		//system(cmdbuf);
		#if EXEC_SYSTEM
		send2Exec(0x30, cmdbuf);
		#else
		system(cmdbuf);
		#endif

		mobile_printf(MOBILE_DEBUG,"Traceroute task is end; line = %d, cmdbuf = %s\n", __LINE__, cmdbuf);
	}

	if((strlen(cmd_rmRoute)!=0) && ( rtAddStatus == 0)){
		//system(cmd_rmRoute);
		#if EXEC_SYSTEM
		send2Exec(0x30, cmd_rmRoute);
		#else
		system(cmd_rmRoute);
		#endif
	}

	//reset all flag.
	rtAddStatus = 0;
	isRTFlag = 0;
	theTraceRouteFlag = 0;//The cwmp trace route is finish,reset..
		
	return NULL;
		
}

int doCwmpTraceRoute(const char *Domain, const char *Dev)
{
	ip4a dnsQueryResult;
	int ret = -1;
	char cmdbuf[200]  = {0};
	#define TraceRoute_DNSURL "/tmp/ippingdiag.conf"

	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__, __LINE__);

	if(strcmp(Domain, "") != 0)
	{
		memset(cmdbuf, 0, sizeof(cmdbuf));
//		sprintf(cmdbuf, "/bin/echo \"%s %s\" > %s", Domain, gTraceRouteDev, TraceRoute_DNSURL);
//		system(cmdbuf);
		sprintf(cmdbuf, "\"%s %s\"", Domain, gTraceRouteDev);
		fileWrite(TraceRoute_DNSURL, cmdbuf, strlen(cmdbuf));
		dnsQueryResult = resolveName(Domain);

		unlink(TraceRoute_DNSURL);

		if(!dnsQueryResult)
		{
			cwmp_traceroute_p.target = 0L;
			ret = -1;
		}
		else
		{
			if(resolve(inet_ntoa(dnsQueryResult),&cwmp_traceroute_p.target,NULL) != 0)
			{
				cwmp_traceroute_p.target = 0L;
				ret = -1;
			}
			else
			{
				mobile_printf(MOBILE_DEBUG,"success create thread to do ip traceroute diagnostic!\n");
				ret = ThreadPoolAddWork(&gThreadPool, cwmpTraceRouteTask, NULL);
				
			}
		}
	}
	else
	{
		cwmp_traceroute_p.target = 0L;
		ret = -1;
	}
	return ret;
}

json_t* TraceRouteDiagnosticsRequest(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	char *cWanName = NULL, *cWanName2 = NULL, *temptraceRouteDomainName = NULL;
	char nodeName[48] = {0}, str_ifIdx[4] = {0}, str_servicelist[64] = {0}, str_vid[6] = {0}, str_mode[16] = {0};
	char cur_servicelist[64] = {0}, cur_wanmode[16] = {0}, cur_vid[6] = {0};
	unsigned int pvc_index = 0, entry_idx = 0;
	char buffer[128] ={0};
	int ret = -1, flag = 0;
	char xpon_status[6]={0};

	temptraceRouteDomainName = ParameterSearchFirstValue(req_para, "Dest");
	cWanName = ParameterSearchFirstValue(req_para, "WANName");
	tcapi_get("XPON_Common","trafficStatus",xpon_status);
	if(strcmp(xpon_status,"up") == 0)
	{
	if((temptraceRouteDomainName != NULL) && (cWanName != NULL))
	{
		mobile_printf(MOBILE_DEBUG,"%s, line = %d, temptraceRouteDomainName = %s, cWanName = %s\n",__FUNCTION__, __LINE__, temptraceRouteDomainName, cWanName);
			
		for ( pvc_index = 0; pvc_index < MAX_PVC_NUM; pvc_index++ )
		{
			for ( entry_idx = 0; entry_idx < MAX_SMUX_NUM; entry_idx++ )
			{
				sprintf(nodeName, "Wan_PVC%d_Entry%d", pvc_index, entry_idx);
				if(tcapi_get(nodeName, "IFIdx", str_ifIdx)) continue;				
				if(tcapi_get(nodeName, "ServiceList", cur_servicelist)) 	continue;				
				if(tcapi_get(nodeName, "WanMode", cur_wanmode)) continue;
				
				tcapi_get(nodeName, "VLANID", cur_vid);
				sprintf(buffer,"%s_%s_%c_VID_%s", str_ifIdx, cur_servicelist, cur_wanmode[0], cur_vid);
			
				if(!strcmp(buffer,cWanName))
				{
					tcapi_get(nodeName, "IFName", gTraceRouteDev);
					flag = 1;
					break;
				}
			}
			if(flag){
				flag = 0;
				break;
			}
		}
		
		//memcpy(traceRouteDomainName, temptraceRouteDomainName, sizeof(traceRouteDomainName));
		
		mobile_printf(MOBILE_DEBUG,"%s, line = %d, traceRouteDomainName = %s, gTraceRouteDev = %s\n",__FUNCTION__, __LINE__, temptraceRouteDomainName, gTraceRouteDev);
		
		ret = doCwmpTraceRoute(temptraceRouteDomainName, gTraceRouteDev);
	}
	else
	{
		ret =-1;
	}
	}else
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
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "TRACEROUTE_DIAG_REQ_NO_ROUTE")){
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
	sleep(6);
	return reply_root;
		
	err:
		if(reply_root) ParameterFreeJSON(&reply_root);
		if(reply_para) ParameterFreeJSON(&reply_para);
		return NULL;

}
