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

#include "Get_Gateway_Blacklist_func.h"

json_t* Get_Gateway_Blacklist_func(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	char macnum[4] = {0}, *macaddrlist = NULL, macname[20] = {0}, stormacaddr[19] = {0}, stormacaddr1[19] = {0}, *paddr = NULL, 
		*pnadrr = NULL, nmacaddr[19] = {0};
	int ret = 0, macnum_i = 0, macnum_j = 0, index_i = 0, indexmax = 40;
	
	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__,__LINE__);

	tcapi_get( "IpMacFilter", "mac_num",macnum);
	macnum_i = atoi(macnum);
	if(macnum_i < 1){
		//ret = -1;
	}
	else{
		macaddrlist=(char*)malloc(sizeof(stormacaddr)*macnum_i);
		if(macaddrlist == NULL){
			goto err;
		}
		memset(macaddrlist,0,sizeof(macaddrlist));
#if defined(TCSUPPORT_CT_JOYME)
		get_mac(macaddrlist);	
#else
		for(;index_i < indexmax;index_i ++){
			sprintf(macname,"IpMacFilter_Entry%d",index_i);
			if(!tcapi_get(macname,"MacAddr" ,stormacaddr)){
				paddr = stormacaddr;
				pnadrr = nmacaddr;
				do{
					if(*paddr != ':')
						*pnadrr++ = *paddr;
				}while (*paddr ++);
				
				if(macnum_j > 0){
					sprintf(stormacaddr1,"/%s",nmacaddr);
				}
				else{
					sprintf(stormacaddr1,"%s",nmacaddr);
				}	
				strcat(macaddrlist,stormacaddr1);
				macnum_j ++;
				if(macnum_j >= macnum_i)
					break;
			}
		}
#endif
	}
		
	//build parameter
	if(ret == -1){
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "No MAC Address In Blacklist")){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	else{
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){//ok
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_MAClist, macaddrlist)){
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
	free(macaddrlist);
	return reply_root;
	
err:
	if(reply_root) ParameterFreeJSON(&reply_root);
	if(reply_para) ParameterFreeJSON(&reply_para);
	free(macaddrlist);
	return NULL;
}

#if defined(TCSUPPORT_CT_JOYME)
int get_mac(char *maclist)
{
	CtMacAddr *macAddrList = NULL;
	char macstr[19] = {0},stormacstr[19] = {0};
	int ret = -1,macAddrNum = 0,i,j = 0;

	if(maclist == NULL)
		return -1;
	
	joyme_lanGetDevInternetAccessBlacklist(&macAddrList, &macAddrNum);
	
	for(i=0;i < macAddrNum ;i++)
	{
		memset(macstr,0,sizeof(macstr));
		memset(stormacstr,0,sizeof(stormacstr));
		sprintf(macstr,"%02x%02x%02x%02x%02x%02x",
			macAddrList[i].macAddr[0],macAddrList[i].macAddr[1],
			macAddrList[i].macAddr[2],macAddrList[i].macAddr[3],
			macAddrList[i].macAddr[4],macAddrList[i].macAddr[5]);
		if(j > 0)
			sprintf(stormacstr,"/%s",macstr);
		else
			sprintf(stormacstr,"%s",macstr);
		
		strcat(maclist,stormacstr);
		j++;
	}
	
	free(macAddrList);
	macAddrList = NULL;
	return 0;
}
#endif

