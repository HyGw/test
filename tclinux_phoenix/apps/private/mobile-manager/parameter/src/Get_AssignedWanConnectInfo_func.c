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
#include "Get_AssignedWanConnectInfo_func.h"

typedef struct{
	const char *ParaName;
	const char *EntryName;
}Para2Entry_t;

const Para2Entry_t  gWanPara[] = {
	{PARA_IFNAME, "IFName"},
	{PARA_PROTOCOL,"IPVERSION"},
	{NULL,NULL}
	};

const Para2Entry_t  gWanInfoPara[] = {
	{PARA_CONNECTIONSTATUS,		"Status"},
	{PARA_IPADDRESS, 			"IP"},
	{PARA_SUBNETMASK,			"NetMask"},
	{PARA_GATEWAY,				"GateWay"},
	{PARA_DNS1,					"DNS"},
	{PARA_DNS2,					"SecDNS"},
	{PARA_IPV6_CONNECTIONSTATUS,"Status6"},
	{PARA_IPV6_IPADDRESS,		"IP6"},
	{PARA_IPV6_PRIFIX,				"PD6"},
	{PARA_IPV6_GATEWAY,			"GateWay6"},
	{PARA_IPV6_DNS1,				"DNS6"},	
	{PARA_IPV6_DNS2,				"SecDNS6"},	
	{PARA_IPV6_PREFIXLENGTH,		"PrefixLen6"},	
	{NULL,NULL}
	};

void ParameterAddJSONChildbyTcapi(json_t* reply_para, Para2Entry_t *pEntry, char *nodename){
	char buffer[128] = {0};
	Para2Entry_t *p = pEntry;
	
	while(p->ParaName){
		memset(buffer, 0, sizeof(buffer));
		if(tcapi_get(nodename, p->EntryName, buffer)) {
			mobile_printf(MOBILE_ERR, "%s:tcapi_get(%s,%s) Err!...\n",__FUNCTION__,nodename,p->EntryName);	
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, p->ParaName, buffer)){
			mobile_printf(MOBILE_ERR, "%s: add %s Err!...\n",__FUNCTION__,p->ParaName);
		}
		p++;
	}	
}

json_t* Get_AssignedWanConnectInfo(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	char  *cWanName = NULL, str_ifaceID[8] = {0}, nodeName[48] = {0};
	char cur_servicelist[64] = {0}, cur_wanmode[16] = {0}, cur_vid[6] = {0}, cur_8021p[6] = {0};
	unsigned int pvc_index = 0, entry_idx = 0;
	int existflag = 0,  cur_ifId = 0, flag = 0;

	char buffer[128] = {0};

	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__,__LINE__);

	cWanName = ParameterSearchValue(req_para, "WANName");

	if(cWanName != NULL)
	{
		mobile_printf(MOBILE_INFO,"line = %d,cWanName = %s\n",__LINE__,cWanName);
				
		//Find the Assigned Wan Connction Interface
		for ( pvc_index = 0; pvc_index < MAX_PVC_NUM; pvc_index ++ )
		{
			for ( entry_idx = 0; entry_idx < MAX_SMUX_NUM; entry_idx ++ )
			{
				sprintf(nodeName, "Wan_PVC%d_Entry%d", pvc_index, entry_idx);
				if(tcapi_get(nodeName, "IFIdx", str_ifaceID)) continue;				
				if(tcapi_get(nodeName, "ServiceList", cur_servicelist)) 	continue;				
				if(tcapi_get(nodeName, "WanMode", cur_wanmode)) continue;

				memset(buffer,0,sizeof(buffer));
				tcapi_get(nodeName, "VLANID", buffer);
				if(strlen(buffer) == 0){
					cur_vid[0] = '\0';
				}else{
					strcpy(cur_vid,buffer);
				}
				
				memset(buffer,0,sizeof(buffer));
				tcapi_get(nodeName, "dot1p", buffer);
				if(strcmp(buffer,"Yes") == 0){
					strcpy(cur_8021p,"1");
				}else{			
					strcpy(cur_8021p,"0");
				}

				memset(buffer,0,sizeof(buffer));
				sprintf(buffer,"%s_%s_%c_VID_%s", str_ifaceID, cur_servicelist, cur_wanmode[0], cur_vid);
				mobile_printf(MOBILE_INFO,"line = %d,cWanName = %s\n",__LINE__,buffer);
				if(!strcmp(buffer,cWanName))
				{
					mobile_printf(MOBILE_DEBUG,"line = %d, pvc_index = %d, entry_idx = %d\n",__LINE__, pvc_index, entry_idx);
			
					cur_ifId = pvc_index*MAX_PVC_NUM + entry_idx;			
					existflag = 1;
					flag = 1;
					break;
				}
			}
			if(flag){
				flag = 0;
				break;
			}
		}	
	}
	else
	{
		existflag= 0;
	}

	//build parameter
	if(existflag == 0){
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "The Wan Connection Is Not Exist")){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	else{
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){//ok
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_INDEX , str_ifaceID)){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}		
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_SERVICELIST , cur_servicelist)){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_CONNECTIONTYPE , cur_wanmode)){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_VLANID , cur_vid[0] ? cur_vid : "NaN")){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}

		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_8021P, cur_8021p)){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
		ParameterAddJSONChildbyTcapi(reply_para, gWanPara, nodeName);
		memset(nodeName, 0, sizeof(nodeName));
		sprintf(nodeName, "WanInfo_Entry%d", cur_ifId);
		ParameterAddJSONChildbyTcapi(reply_para, gWanInfoPara, nodeName);		

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

