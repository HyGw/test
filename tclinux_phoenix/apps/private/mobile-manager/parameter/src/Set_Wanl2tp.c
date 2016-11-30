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
#if defined(TCSUPPORT_CT_JOYME)
#include "Set_Wanl2tp.h"

/*8.2.71*/
json_t* Create_WANL2TP_Tunnel(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	char *str_serverIpAddr = NULL, *str_username = NULL, *str_password = NULL, *str_userId = NULL;
	char tunnelName[128]={0};
	int ret = -1, i=0;
	char strValue[64]={0},attrName[32]={0};

	mobile_printf(MOBILE_INFO,"%s:start!...\n",__FUNCTION__);

	str_userId = ParameterSearchValue(req_para, "userId");
	str_serverIpAddr = ParameterSearchValue(req_para, "serverIpAddr");
	str_username = ParameterSearchValue(req_para, "username");
	str_password = ParameterSearchValue(req_para, "password");
	
	mobile_printf(MOBILE_INFO,"%s: serverIpAddr = %s ,username= %s ,password = %s...  !!!\n",__FUNCTION__,str_serverIpAddr,
		str_username,str_password);
	ret=joyme_wanCreateL2tpTunnel(str_userId,str_serverIpAddr,str_username ,str_password ,tunnelName,sizeof(tunnelName)-1);
	
//build parameter
	if(ret == -1){ /*fail*/
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, " create fail !")){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	else{ /*success*/
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){//ok
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_TUNNELNAME, tunnelName)){
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

/*8.2.72*/
json_t* Remove_WANL2TP_Tunnel(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	char *str_tunnelName = NULL, *str_userId;
	int ret = 1, i =0 , flag = 0;
	int tunnelName_num = 0, userId_num = 0;
	char strValue[64]={0},attrName[32]={0};
	
	mobile_printf(MOBILE_INFO,"%s:start!...\n",__FUNCTION__);
	
	str_tunnelName = ParameterSearchValue(req_para, "tunnelName");
	str_userId = ParameterSearchValue(req_para, "userId");

	
	ret = joyme_wanRemoveL2tpTunnel(str_userId, str_tunnelName);
			

	//build parameter
	if(ret == -1){ /*fail*/
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, " Remove fail !")){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	else if (ret == -2){
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 2))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, " VPN tunnel not exist !")){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	else if (ret == -3){
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 3))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, " UserId not exist !")){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	else{ /*success*/
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){//ok
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
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

/*8.2.73*/
json_t* Attach_WANL2TP_Tunnel(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	json_t *json_list = NULL, *json_ipaddr = NULL;
	char *str_tunnelName = NULL, *str_ipAddr = NULL;
	int ret = -1, i = 0;
	CtIpAddr CipAddrList[VPN_ATTACH_NUM];
	char strValue[64]={0},attrName[32]={0};
	
	mobile_printf(MOBILE_INFO,"%s:start!...\n",__FUNCTION__);
	
	str_tunnelName = ParameterSearchValue(req_para, "tunnelName");
	json_list = ParameterSearchFirstLabel(req_para, "List");
	
	
	if(json_list == NULL){
		mobile_printf(MOBILE_INFO,"%s, line = %d, search node error\n",__FUNCTION__,__LINE__);	
		ret = -1;
		goto end;
	}
	memset(CipAddrList,0,sizeof(CipAddrList));
	if((json_list->child != NULL) && (json_list->child->child != NULL)){
		json_ipaddr = json_list->child->child;
		str_ipAddr = ParameterSearchFirstValue(json_ipaddr, "ipDomainNameAddr");
		while(str_ipAddr != NULL ){
			//strcat(CipAddrList[i].ipAddr,str_ipAddr);
			sprintf(CipAddrList[i].ipAddr,"%s",str_ipAddr);
			i++;
			json_ipaddr = json_ipaddr->next;	
			if(json_ipaddr == NULL)
				break;
			str_ipAddr = ParameterSearchFirstValue(json_ipaddr, "ipDomainNameAddr");
		}	
	}
			
	mobile_printf(MOBILE_INFO,"%s: tunnelName = %s, ipAddrNum=%d!!!\n",__FUNCTION__,str_tunnelName,i);



	ret=joyme_wanAttachL2tpTunnel(str_tunnelName,&CipAddrList,i);

	
end:		
	//build parameter
	if(ret == -1){ /*fail*/
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, " attach fail !")){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	else if (ret == -2){
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 2))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, " VPN tunnel not exist !")){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	else{ /*success*/
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){//ok
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
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

/*8.2.74*/
json_t* Get_WANL2TP_Tunnel_Status(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	json_t *nodes_array = NULL, *array_obj = NULL;
	char *str_tunnelName = NULL, *str_userId =NULL;
	int tunnelStatus = 1;
	char tunstatus[VPN_INSTANCE_NUM][8] ,tunName[VPN_INSTANCE_NUM][32];
	int ret = -1, i =0, rt = 0, flag = 1;
	int userId_num = 0, tunnelName_num = 0;
	char strValue[64]={0},attrName[32]={0};
	
	mobile_printf(MOBILE_INFO,"%s:start!...\n",__FUNCTION__);
	
	str_tunnelName = ParameterSearchValue(req_para, "tunnelName");
	str_userId = ParameterSearchValue(req_para, "userId");
			
	mobile_printf(MOBILE_INFO,"%s:  tunnelName = %s	 \n",__FUNCTION__,str_tunnelName);

	memset(tunstatus, 0, sizeof(tunstatus));
	memset(tunName, 0, sizeof(tunName));
	if( !strcmp(str_tunnelName , "all")){
		if (!strcmp(str_userId , "0")){        /*tunnelName is all ,userId is 0 , remove all the tunnel ,don't match the tunnelName  and userId*/
			for(i = 0; i < VPN_INSTANCE_NUM; i++){
				memset(attrName, 0, sizeof(attrName));
				sprintf(attrName, "%s%d", VPN_ENTRY, i);
				memset(strValue, 0, sizeof(strValue));
				//tcapi_get(attrName, "tunnelName", strValue);
				rt=tcapi_get(attrName, "vpnstatus",strValue);
				if ( rt != 0)
					continue;
				else {
					flag = 0;
					if (strcmp(strValue, "0")) 
						tunnelStatus = 1;
					else 
						tunnelStatus = 0;
					sprintf(tunstatus[i],"%d",tunnelStatus);
					
					memset(strValue, 0, sizeof(strValue));
					tcapi_get(attrName, "tunnelName",strValue);
					sprintf(tunName[i],"%s",strValue);
				}
			}
			if (flag == 0)
				ret = 0;
			else
				ret = -1;
		}
		else{        /*tunnelName is all ,but userId isn't 0 ,match the userId */
			for(i = 0; i < VPN_INSTANCE_NUM; i++){
				memset(attrName, 0, sizeof(attrName));
				sprintf(attrName, "%s%d", VPN_ENTRY, i);
				memset(strValue, 0, sizeof(strValue));
				tcapi_get(attrName, "userId", strValue);
				if(!strcmp(str_userId,strValue)){
					userId_num++;
					memset(strValue, 0, sizeof(strValue));
					rt=tcapi_get(attrName, "vpnstatus",strValue);
					if ( rt != 0)
						continue;
					else {
						flag = 0;
						if (strcmp(strValue, "0")) 
							tunnelStatus = 1;
						else 
							tunnelStatus = 0;
						sprintf(tunstatus[i],"%d",tunnelStatus);
						
						memset(strValue, 0, sizeof(strValue));
						tcapi_get(attrName, "tunnelName",strValue);
						sprintf(tunName[i],"%s",strValue);
					}				
				}
			}
			if (userId_num == 0)
				ret = -3;
			else if (flag == 0)
				ret = 0;
			else 
				ret = -1;
		}
	}
	else{ 
		/*tunnelName is not all ,  should match the userId*/
		for(i = 0; i < VPN_INSTANCE_NUM; i++){
			memset(attrName, 0, sizeof(attrName));
			sprintf(attrName, "%s%d", VPN_ENTRY, i);
			memset(strValue, 0, sizeof(strValue));
			tcapi_get(attrName, "userId", strValue);
			if(!strcmp(str_userId,strValue)){
				userId_num ++;
				memset(strValue, 0, sizeof(strValue));
				tcapi_get(attrName, "tunnelName", strValue);
				if(!strcmp(str_tunnelName, strValue)){
					tunnelName_num++;
					memset(strValue, 0, sizeof(strValue));
					rt=tcapi_get(attrName, "vpnstatus",strValue);
					if ( rt != 0)
						break;
					else {
						flag = 0;
						if (strcmp(strValue, "0")) 
							tunnelStatus = 1;
						else 
							tunnelStatus = 0;
						sprintf(tunstatus[i],"%d",tunnelStatus);
						
						memset(strValue, 0, sizeof(strValue));
						tcapi_get(attrName, "tunnelName",strValue);
						sprintf(tunName[i],"%s",strValue);
					}	
				}
			}
		}
		if (userId_num == 0)
			ret = -3;
		else if(tunnelName_num == 0)
			ret = -2;
		else if (flag == 0)
			ret = 0;
		else 
			ret = -1;
	}

	//build parameter
	if(ret == -1){ /*fail*/
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, " getStatus fail !")){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	else if (ret == -2){
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 2))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, " VPN tunnel not exist !")){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	else if (ret == -3){
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 2))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, " UserId not exist !")){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	else{ /*success*/
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		
		nodes_array = ParameterNewJSONArray();
		if ( !nodes_array )
		{
			mobile_printf(MOBILE_INFO,
				"%s:malloc JSON array Err!...\n",__FUNCTION__);
			goto err;
		}
	
		/*add array to parameter list, then only need free reply_para*/
		if ( JSON_OK != ParameterInsertJSONPair(reply_para, "List", nodes_array) )
		{
			mobile_printf(MOBILE_INFO, "%s:Add JSON array to para Err!...\n" ,__FUNCTION__);
			goto err;
		}
		for(i = 0; i < VPN_INSTANCE_NUM; i++){
			if(strcmp(tunName[i],"")){
				array_obj = ParameterNewJSONObject();
				if ( !array_obj )
				{
					mobile_printf(MOBILE_INFO,
						"%s:malloc JSON array obj Err!...\n",__FUNCTION__);
					goto err;
				}
				if( JSON_OK != ParameterAddNewJSONChild(array_obj, "tunnelName", tunName[i]) )
				{
					mobile_printf(MOBILE_INFO ,"%s:ParameterAddNewJSONChild Err!...\n" ,__FUNCTION__);
					goto err;
				}
				if( JSON_OK != ParameterAddNewJSONChild(array_obj, "tunnelStatus", tunstatus[i]) )
				{
					mobile_printf(MOBILE_INFO ,"%s:ParameterAddNewJSONChild Err!...\n" ,__FUNCTION__);
					goto err;
				}

				/*add to array then it will free array only no need to free every object*/
				if ( JSON_OK != json_insert_child(nodes_array, array_obj) )
				{
					mobile_printf(MOBILE_INFO, "%s:Add JSON array obj Err!...\n", __FUNCTION__);
					goto err;
				}
			}
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

/*8.2.77*/
json_t* Detach_WANL2TP_Tunnel(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	json_t *json_list = NULL, *json_ipaddr = NULL;
	char *str_tunnelName = NULL, *str_ipAddr = NULL;
	int ret = -2, i = 0;
	CtIpAddr CipAddrList[2];
	char strValue[64]={0},attrName[32]={0};
	
	mobile_printf(MOBILE_INFO,"%s:start!...\n",__FUNCTION__);
	
	str_tunnelName = ParameterSearchValue(req_para, "tunnelName");
	json_list = ParameterSearchFirstLabel(req_para, "List");
	
	if(json_list == NULL){
		mobile_printf(MOBILE_INFO,"%s, line = %d, search node error\n",__FUNCTION__,__LINE__);	
		ret = -1;
		goto end;
	}
	
	memset(CipAddrList,0,sizeof(CtIpAddr)*2);
	if((json_list->child != NULL) && (json_list->child->child != NULL)){
		json_ipaddr = json_list->child->child;
		str_ipAddr = ParameterSearchFirstValue(json_ipaddr, "ipDomainNameAddr");
		while(str_ipAddr != NULL ){
			
			sprintf(CipAddrList[i].ipAddr,"%s",str_ipAddr);
			i++;
			json_ipaddr = json_ipaddr->next;	
			if(json_ipaddr == NULL)
				break;
			str_ipAddr = ParameterSearchFirstValue(json_ipaddr, "ipDomainNameAddr");

		}	
	}		
	mobile_printf(MOBILE_INFO,"%s: tunnelName = %s, ipAddrNum=%d!!!\n",__FUNCTION__,str_tunnelName,i);
	

	ret=joyme_wanDetachL2tpTunnel(str_tunnelName,&CipAddrList,i);


end:		
	//build parameter
	if(ret == -1){ /*fail*/
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, " detach fail !")){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	else if(ret == -2){ /*fail*/
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 2))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, " VPN tunnel not exist ! ")){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	else{ /*success*/
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){//ok
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
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
#endif


