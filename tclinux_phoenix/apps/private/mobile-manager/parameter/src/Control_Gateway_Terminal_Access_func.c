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

#include "Control_Gateway_Terminal_Access_func.h"

int findMacaddr(char *adrr)
{
	char listmacnum[4] = {0}, macadrr[19] = {0}, macname[20] = {0};
	int indexi = 0, indexj = 0, indexmax = 40, listmacnum_int = 0;

	tcapi_get("IpMacFilter", "mac_num", listmacnum);
	listmacnum_int = atoi(listmacnum);
	for(;indexi < indexmax;indexi ++){
		sprintf(macname,"IpMacFilter_Entry%d",indexi);
		if(!tcapi_get(macname,"MacAddr" ,macadrr)){
			if(!strcmp(adrr,macadrr)){
				return indexi;
			}
			indexj ++;
		}
		if(indexj >= listmacnum_int)
			return -1;
	}
}

void DELMacaddr(int num)
{
	char delIndex[4]={0};
	sprintf(delIndex,"%d,",num);
	tcapi_set("IpMacFilter_Common", "IpMacType", "Mac");
	tcapi_set("IpMacFilter_Common", "Action", "Del");
	tcapi_set("IpMacFilter_Common", "DeleteIndex", delIndex);
	tcapi_commit("IpMacFilter_Entry");
	tcapi_save();
}

void addMacaddr(char *macadrr)
{
	char ip_id[4] = {0};
	tcapi_get("IpMacFilter", "add_macnum", ip_id);
	tcapi_set("WebCurSet_Entry", "ipfilter_id", ip_id);
	tcapi_set("IpMacFilter_Common", "Action", "Add");
	tcapi_set("IpMacFilter_Common", "IpMacType", "Mac");
	tcapi_set("IpMacFilter_Entry", "Active", "Yes");
	tcapi_set("IpMacFilter_Entry", "RuleType", "MAC");
	tcapi_set("IpMacFilter_Entry", "MacName", ip_id);
	tcapi_set("IpMacFilter_Entry", "MacAddr", macadrr);
	tcapi_set("IpMacFilter_Entry", "Interface", "br0");
	tcapi_set("IpMacFilter_Entry", "Direction", "Incoming");
	
	tcapi_commit("IpMacFilter_Entry");
	tcapi_save();
}

#if defined(TCSUPPORT_CT_JOYME)
#define FREE_X(x) { if (x) free(x); x = NULL; }

/*
get mac from storage access right table.
*/
int getSARMACListLib(char *buf)
{
	int idx = 0, macNUM = 0;
	char macBuf[20] = {0};
	CtMacAddr *macAddrList = NULL;

	if ( !buf )
		return -1;

	if ( 0 != joyme_lanGetDevStorageAccessBlacklist(&macAddrList, &macNUM) )
	{
		FREE_X(macAddrList);
		return -1;
	}

	for ( idx = 0; idx < macNUM; idx++ )
	{
		snprintf(macBuf, sizeof(macBuf) - 1
			, "%02X%02X%02X%02X%02X%02X"
			, macAddrList[idx].macAddr[0], macAddrList[idx].macAddr[1]
			, macAddrList[idx].macAddr[2], macAddrList[idx].macAddr[3]
			, macAddrList[idx].macAddr[4], macAddrList[idx].macAddr[5]);

		if ( 0 != buf[0] )
			strcat(buf, "/");
		strcat(buf, macBuf);
	}
	FREE_X(macAddrList);

	return 0;
}
#endif

json_t* Control_Gateway_Terminal_Access_func(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	char *macaddr = NULL, *internetaccessright = NULL, activemac[4] = {0}, listtypemac[10] = {0},inmacaddr[19] = {0};
	int ret = 0,i = 1;
	int listmacnum = 0;
#if defined(TCSUPPORT_CT_JOYME)
	char *sarPtr = NULL;
	int sarRET = 0;
	int internet_access = 99, storage_access = 99, macidx = 0;
	int mac_addr[6] = {0};
	CtMacAddr in_macAddr = {0};
#endif
	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__,__LINE__);

	macaddr = ParameterSearchValue(req_para, "MAC");
	internetaccessright = ParameterSearchValue(req_para, "InternetAccessRight");

#if defined(TCSUPPORT_CT_JOYME)
	sarPtr = ParameterSearchValue(req_para, "StorageAccessRight");
	if( (macaddr != NULL) )
	{
		if ( internetaccessright )
		{
			if ( 0 == strcmp(internetaccessright, "ON") )
				internet_access = 1;
			else if ( 0 == strcmp(internetaccessright, "OFF") )
				internet_access = 0;
		}

		if ( sarPtr )
		{
			if ( 0 == strcmp(sarPtr, "ON") )
				storage_access = 1;
			else if ( 0 == strcmp(sarPtr, "OFF") )
				storage_access = 0;
		}

		sscanf(macaddr, "%02x%02x%02x%02x%02x%02x"
				, &mac_addr[0]
				, &mac_addr[1]
				, &mac_addr[2]
				, &mac_addr[3]
				, &mac_addr[4]
				, &mac_addr[5]
				);
		for ( macidx = 0; macidx < 6; macidx ++ )
			in_macAddr.macAddr[macidx] = (unsigned char)mac_addr[macidx];

		ret = joyme_lanSetDevAccessPermission(&in_macAddr
											, internet_access
											, storage_access);
	}
#else
	if((macaddr != NULL) && (internetaccessright != NULL)){
		while(*macaddr){
			if(i%3==0){
				inmacaddr[i-1] = ':';
				i++;
			}
			inmacaddr[i-1] = *macaddr;
			i ++;
			macaddr ++;
		};
		tcapi_get("IpMacFilter_Common", "ActiveMac", activemac);
		tcapi_get("IpMacFilter_Common", "ListTypeMac", listtypemac);
		if(strcmp(activemac,"1")){
			tcapi_set("IpMacFilter_Common", "ActiveMac", "1");
		}
		if(strcmp(listtypemac,"Black")){
			tcapi_set("IpMacFilter_Common", "ListTypeMac", "Black");
		}

		/*find mac address*/
		listmacnum = findMacaddr(inmacaddr);
		if(!strcmp(internetaccessright,"ON")){
			//find and delete
			if(listmacnum != -1){
				DELMacaddr(listmacnum);
			}
		}
		else if(!strcmp(internetaccessright,"OFF")){
			//add
			if(listmacnum != -1){
				/*mac is already exist*/				
				ret = -2;
			}else{
				addMacaddr(inmacaddr);
			}
		}
	}
#endif
	else
		ret=-1;
	//build parameter
	if(ret == -1 || ret == -2
#if defined(TCSUPPORT_CT_JOYME)
		|| -3 == ret
#endif
		){
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}

		
		if(ret == -1  && JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "fail to control gateway terminal access")){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}

		if(ret == -2  && JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "mac is already exist")){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}

#if defined(TCSUPPORT_CT_JOYME)
		if( -3 == ret
			&& JSON_OK != ParameterAddNewJSONChild(reply_para
					, PARA_FAIL_REASON
					, "StorageAccessRight MAC table is full"))
		{
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n"
				,__FUNCTION__);
			goto err;
		}
#endif
	}
	else{
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
	if(reply_para) ParameterFreeJSON(&reply_para);

	return reply_root;
	
err:
	if(reply_root) ParameterFreeJSON(&reply_root);
	if(reply_para) ParameterFreeJSON(&reply_para);
	return NULL;
}

#if defined(TCSUPPORT_CT_JOYME)
/*
GET_ATTACH_DEVICE_STORAGE_ACCESS_BLACKLIST callback
*/
json_t* Get_Attach_Device_Storage_Access_func
(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	char *macList = NULL;

	if ( !macList )
		macList = calloc(MAX_SAR_RULE, 20);
	if ( !macList )
		goto err;

	if ( 0 != getSARMACListLib(macList) )
		goto err;

	/* set return parameter status */
	if(!(reply_para = ParameterBuildJSONReplyPara(req_para
			, PARA_RESULT_SUCCESS)))
	{
		mobile_printf(MOBILE_INFO,
			"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
		goto err;
	}

	if( JSON_OK !=
		ParameterAddNewJSONChild(reply_para, "MACList", macList))
	{
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n"
			,__FUNCTION__);
		goto err;
	}

	if( !(reply_root = ParameterBuildJSONReplyHead(req_root
		, PARA_RESULT_SUCCESS)) )
	{
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyHead Err!...\n"
			,__FUNCTION__);
		goto err;
	}
	if ( ParameterBuildJSONAddPara2Head(reply_root, reply_para) )
	{
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONAddPara2Head Err!...\n"
			,__FUNCTION__);
		goto err;
	}

	FREE_X(macList);
	if(reply_para) ParameterFreeJSON(&reply_para);

	return reply_root;
	
err:
	FREE_X(macList);
	if(reply_root) ParameterFreeJSON(&reply_root);
	if(reply_para) ParameterFreeJSON(&reply_para);
	return NULL;
}
#endif

