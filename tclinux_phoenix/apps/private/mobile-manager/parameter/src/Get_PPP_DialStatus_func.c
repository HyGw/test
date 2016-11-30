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

json_t* GetPPP_DialStatus(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	char IPv4ConStatus[8] = {0}, IPv4Status[32] = {0}, IPv4ConError[128] = {0};
	char IPv6ConStatus[8] = {0}, IPv6Status[32] = {0}, IPv6ConError[128] = {0};
	uint32 wanNodeIndexv4 = 0, pvcNodeEntryv4 = 0, wanNodeIndexv6 = 0, pvcNodeEntryv6 = 0;
	int IPv4ret = -1, IPv6ret = -1, defRouterIfv4 = -1, defRouterIfv6 = -1;

	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__, __LINE__);

	getISPIndex(&defRouterIfv4, &defRouterIfv6);

	if(defRouterIfv4 < 0){
		strcpy(IPv4ConStatus, "0");
		strcpy(IPv4Status, "Unconfigured");
		strcpy(IPv4ConError, "ERROR_NO_VALID_CONNECTION");
	}
	else{
		wanNodeIndexv4 = defRouterIfv4 / MAX_PVC_NUM;
		pvcNodeEntryv4 = defRouterIfv4 % MAX_PVC_NUM;
	
		IPv4ret = getWANCnt1_IPv4InfoValue(IPv4ConStatus, IPv4Status, IPv4ConError, wanNodeIndexv4, pvcNodeEntryv4);

		if(IPv4ret == -1){
			strcpy(IPv4ConStatus, "0");
			strcpy(IPv4Status, "Unconfigured");
			strcpy(IPv4ConError, "ERROR_NO_VALID_CONNECTION");
		}
	}

	if(defRouterIfv6 < 0){
		strcpy(IPv6ConStatus, "0");
		strcpy(IPv6Status, "Unconfigured");
		strcpy(IPv6ConError, "ERROR_NO_VALID_CONNECTION");
	}
	else{
		wanNodeIndexv6 = defRouterIfv6 / MAX_PVC_NUM;
		pvcNodeEntryv6 = defRouterIfv6 % MAX_PVC_NUM;
	
		IPv6ret = getWANCnt1_IPv6InfoValue(IPv6ConStatus, IPv6Status, IPv6ConError, wanNodeIndexv6, pvcNodeEntryv6);

		if(IPv6ret == -1){
			strcpy(IPv6ConStatus, "0");
			strcpy(IPv6Status, "Unconfigured");
			strcpy(IPv6ConError, "ERROR_NO_VALID_CONNECTION");
		}
	}

	//build parameter
	if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){//ok
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_CONNECTIONSTATUS, IPv4ConStatus)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_WANSTATUS, IPv4Status)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_DIALREASON, IPv4ConError)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_CONNECTIONSTATUS1, IPv6ConStatus)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_WANSTATUS1, IPv6Status)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_DIALREASON1, IPv6ConError)){
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

int getWANCnt1_IPv4InfoValue(char IPv4ConStatus[8], char IPv4Status[32], char IPv4ConError[128], uint32 wanNodeIndex, uint32 pvcNodeEntry)
{
	char nodeName[34] = {0};
	char ipver[16] = {0};
	char status[10] = {0};
	int IPv4_type = 1;//IPv4:1\IPv6:2;

	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__, __LINE__);

	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex, pvcNodeEntry);

	tcapi_get(nodeName, "IPVERSION", ipver);
	if ( 0 == strcmp(ipver, "IPv4") || 0 == strcmp(ipver, "IPv4/IPv6") )
	{
		mobile_printf(MOBILE_DEBUG,"=====IPv4=====\n");

		//ConnectionStatus
		sprintf(nodeName, "WanInfo_Entry%d", wanNodeIndex<<3|pvcNodeEntry);
		tcapi_get(nodeName, "Status", status);
		if( 0 == strcmp(status,"up"))
			strcpy(IPv4ConStatus, "0");
		else
			strcpy(IPv4ConStatus, "1");

		//WANStatus	
		getWanPPPCntGetStatusInfo(IPv4_type,IPv4Status,wanNodeIndex,pvcNodeEntry);

		//ConnectionError
		sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex, pvcNodeEntry);
		if( 0 == strcmp(IPv4Status, "Unconfigured"))
			strcpy(IPv4ConError, "ERROR_NO_VALID_CONNECTION");
		else if( 0 == strcmp(IPv4Status, "Connecting"))
			tcapi_get(nodeName, "ConnectionError", IPv4ConError);

		return 0;
	}

	return -1;
}

int getWANCnt1_IPv6InfoValue(char IPv6ConStatus[8],char IPv6Status[32],char IPv6ConError[128],unsigned long wanNodeIndex,unsigned long pvcNodeEntry)
{
	char nodeName[34] = {0};
	char ipver[16] = {0};
	char IPv6status[12] = {0};
	int IPv6_type = 2;//IPv4:1\IPv6:2;

	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__, __LINE__);

	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex, pvcNodeEntry);

	tcapi_get(nodeName, "IPVERSION", ipver);
	if ( 0 == strcmp(ipver, "IPv6") || 0 == strcmp(ipver, "IPv4/IPv6") )
	{
		mobile_printf(MOBILE_DEBUG,"=====IPv6=====\n");

		//ConnectionStatus
		sprintf(nodeName, "WanInfo_Entry%d", ((wanNodeIndex<<3)|pvcNodeEntry));
		tcapi_get(nodeName, "Status6", IPv6status);
		if ( 0 == strcmp(IPv6status, "up") )
			strcpy(IPv6ConStatus, "0");
		else
			strcpy(IPv6ConStatus, "1");

		//WANStatus
		getWanPPPCntGetStatusInfo(IPv6_type,IPv6Status,wanNodeIndex,pvcNodeEntry);

		//ConnectionError
		sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex, pvcNodeEntry);
		if( 0 == strcmp(IPv6Status, "Unconfigured"))
			strcpy(IPv6ConError, "ERROR_NO_VALID_CONNECTION");
		else if( 0 == strcmp(IPv6Status, "Connecting"))
			tcapi_get(nodeName, "ConnectionError", IPv6ConError);

		return 0;
	}

	return -1;
}

int getISPIndex(int *defRouterIfv4, int *defRouterIfv6)
{
	char default_routev4[16]={0}, default_routev6[16]={0};
	char active[8] = {0};
	char nodeName[48] = {0};
	char serviceType[64] = {0};
	char wanMode[32] = {0};
	char IPVERSION[8] = {0};
	int i = 0, j = 0, IPv4_flag = 0, IPv6_flag = 0;

	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__, __LINE__);

	memset(default_routev4,0,sizeof(default_routev4));
	memset(default_routev6,0,sizeof(default_routev6));

	tcapi_get("WanInfo_Common", "DefRouteIndexv4", default_routev4);
	if(strlen(default_routev4) != 0 && strncmp(default_routev4, "N/A", 3)) {
		*defRouterIfv4 =  atoi(default_routev4);
		
		mobile_printf(MOBILE_DEBUG,"==IPv4 pvc active default route==\n");
	}
	else {	/*If no IPv4 pvc active default route, find the first IPv4 actived Internet interface*/
		mobile_printf(MOBILE_DEBUG,"==find the first actived Internet interface==\n");

		*defRouterIfv4 = -1;

		for(i = 0; i < MAX_PVC_NUM; i++) {
			for(j = 0; j < MAX_SMUX_NUM; j++) {
				memset(active, 0, sizeof(active));
				memset(serviceType, 0, sizeof(serviceType));
				memset(wanMode, 0, sizeof(wanMode));

				sprintf(nodeName, "Wan_PVC%d_Entry%d", i, j);
				tcapi_get(nodeName, "Active", active);
				tcapi_get(nodeName, "ServiceList", serviceType);
				tcapi_get(nodeName, "WanMode", wanMode);
				tcapi_get(nodeName, "IPVERSION", IPVERSION);
				
				if(!strcmp(active, "Yes") &&
					!strcmp(wanMode, "Route") &&
					(!strcmp(IPVERSION, "IPv4") || !strcmp(IPVERSION, "IPv4/IPv6")) &&
					(strstr(serviceType,"INTERNET")!=NULL)) {
					*defRouterIfv4 = i * MAX_SMUX_NUM +j;
					IPv4_flag = 1;
					break; 
				}
			}
			if(IPv4_flag == 1){
				IPv4_flag = 0;
				break; 
			}
		}
	}

	tcapi_get("WanInfo_Common", "DefRouteIndexv6", default_routev6);
	if(strlen(default_routev6) != 0 && strncmp(default_routev6, "N/A", 3)) {
		*defRouterIfv6 =  atoi(default_routev6);
		
		mobile_printf(MOBILE_DEBUG,"==IPv6 pvc active default route==\n");
	}
	else {	/*If no IPv6 pvc active default route, find the first IPv6 actived Internet interface*/
		mobile_printf(MOBILE_DEBUG,"==find the first actived Internet interface==\n");

		*defRouterIfv6 = -1;

		for(i = 0; i < MAX_PVC_NUM; i++) {
			for(j = 0; j < MAX_SMUX_NUM; j++) {
				memset(active, 0, sizeof(active));
				memset(serviceType, 0, sizeof(serviceType));
				memset(wanMode, 0, sizeof(wanMode));

				sprintf(nodeName, "Wan_PVC%d_Entry%d", i, j);
				tcapi_get(nodeName, "Active", active);
				tcapi_get(nodeName, "ServiceList", serviceType);
				tcapi_get(nodeName, "WanMode", wanMode);
				tcapi_get(nodeName, "IPVERSION", IPVERSION);			

				if(!strcmp(active, "Yes") &&
					!strcmp(wanMode, "Route") &&
					(!strcmp(IPVERSION, "IPv6") || !strcmp(IPVERSION, "IPv4/IPv6")) &&
					(strstr(serviceType,"INTERNET")!=NULL)) {
					*defRouterIfv6 = i * MAX_SMUX_NUM +j;
					IPv6_flag = 1;
					break; 
				}
			}
			if(IPv6_flag == 1){
				IPv6_flag = 0;
				break; 
			}
		}
	}

	return 0;
}

int getWanPPPCntGetStatusInfo(int type, char WanStatus[32], uint32 wanNodeIndex, uint32 pvcNodeEntry)
{
	int	RouterIf = 0;
	char temp[32];
	wanConnStatus  connstatus;

	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__, __LINE__);

	RouterIf = wanNodeIndex * MAX_SMUX_NUM + pvcNodeEntry;

	/*Get NewConnectionStatus stateVariable*/
	if(TcGetWANConnStatus(type, &connstatus, RouterIf) != 0)
		return -1;
	else
		switch(connstatus){
			case Connecting:
				strcpy(WanStatus, "Connecting");
				break;
			case Connected:
				strcpy(WanStatus, "Connected");
				break;
			case PendingDisconnect:
				strcpy(WanStatus, "PendingDisconnect");
				break;
			case Disconnecting:
				strcpy(WanStatus, "Disconnecting");
				break;
			case Disconnected:
				strcpy(WanStatus, "Disconnected");
				break;
			case Unconfigured:
				strcpy(WanStatus, "Unconfigured");
				break;
			default:
				strcpy(WanStatus, "Unconfigured");
	}

	return 0;
}

int TcGetWANConnStatus(int type, wanConnStatus *connstatus, int pvcindex)
{
	char nodeName[32];
	char connectStatus[8];
	char temptrafficStatus[20] = {0};

	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__, __LINE__);

	if(pvcindex<0 || pvcindex>=MAX_WAN_IF_INDEX){
		return -1;
	}
	
	memset(nodeName, 0, sizeof(nodeName));
	memset(connectStatus, 0, sizeof(connectStatus));

	sprintf(nodeName,"WanInfo_Entry%d", pvcindex);

	if(1 == type)
	{
		tcapi_get(nodeName, "Status", connectStatus);
	}
	else if(2 == type)
	{
		tcapi_get(nodeName, "Status6", connectStatus);
	}

	tcapi_get("XPON_Common", "trafficStatus", temptrafficStatus);
	if(strstr(temptrafficStatus, "up") == NULL)
	{
		*connstatus = Unconfigured;
	}
	else
	{
	if(strcmp(connectStatus, "down") == 0)
			*connstatus = Connecting;
	else if(strcmp(connectStatus, "up") == 0)
		*connstatus = Connected;
	}

	return 0;
}

