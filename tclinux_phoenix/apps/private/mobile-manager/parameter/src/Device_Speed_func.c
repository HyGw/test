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

#include "Device_Speed_func.h"

#define BANDWIDTH_DEVICE_SPEED		"BandWidth_DeviceSpeed"

/*______________________________________________________________________________
**	Device_Speed_func
**		Device_Speed_func
**
**	descriptions:
**		
**	parameters:
**	return:
**		
**_______________________________________________________________________________
*/
json_t* Device_Speed_func(json_t* req_root, json_t* req_para)
{
#if defined(TCSUPPORT_CT_JOYME)
	json_t *reply_root = NULL, *reply_para = NULL;
	CtMacAddr macAddr;
	int usBandwidth; 
	int dsBandwidth;
	char strUpBandWidth[64] = {0};
	char strDownBandWidth[64] = {0};

	
	json_t *json_list = NULL, *json_mac = NULL;
	char *strEnable = NULL, *strTime = NULL, *strMac = NULL;
	char buf[64] = {0};
	int ret = -1;
	int index = 0;

	int mac[6] = {0};
	int rt = -1;

	strEnable = ParameterSearchValue(req_para, "Enable");
	strTime = ParameterSearchValue(req_para, "Time");
	json_list = ParameterSearchFirstLabel(req_para, "List");


	if((strEnable == NULL) || (strTime == NULL) || (json_list == NULL)){
		mobile_printf(MOBILE_INFO,"%s, line = %d, search node error\n",__FUNCTION__,__LINE__);	
		ret = -1;
		goto end;
	}


	if((strcmp(strEnable,"0") != 0) && (strcmp(strEnable,"1") != 0)){
		mobile_printf(MOBILE_INFO,"%s, line = %d, Enable=%s error\n",__FUNCTION__,__LINE__,strEnable); 
		ret = -1;
		goto end;
	}


	tcapi_unset(BANDWIDTH_DEVICE_SPEED);		
	/*check device mac list*/
	if((json_list->child != NULL) && (json_list->child->child != NULL)){
		json_mac = json_list->child->child;
		strMac = ParameterSearchFirstValue(json_mac, "MAC");
		while(strMac != NULL ){
			/*match the mac*/
			ret = -1;
			rt = sscanf(strMac,"%02x:%02x:%02x:%02x:%02x:%02x", &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
			if (rt > 1){
				sprintf(strMac, "%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
			}
			memset(buf,0,sizeof(buf));
			sprintf(buf,"MAC%d",index);
			mobile_printf(MOBILE_INFO,"%s, line = %d, MAC%d:%s\n",__FUNCTION__,__LINE__,index,strMac);	
			tcapi_set(BANDWIDTH_DEVICE_SPEED,buf,strMac);		
			index++;
			json_mac = json_mac->next;	
			
			if(json_mac == NULL)
				break;
			
			strMac = ParameterSearchFirstValue(json_mac, "MAC");
		}	
	}
	
	memset(buf,0,sizeof(buf));	
	sprintf(buf,"%d",index);
	tcapi_set(BANDWIDTH_DEVICE_SPEED,"macNum",buf); 	
	tcapi_set(BANDWIDTH_DEVICE_SPEED,"Enable",strEnable);		
	tcapi_set(BANDWIDTH_DEVICE_SPEED,"Time",strTime);	

	tcapi_save();
	Sendclientpipe();
	ret = 0;

end:	
	//build parameter
	if(ret == -1){
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "Device Speed Config fail")){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
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

	return reply_root;
	
err:
	if(reply_root) ParameterFreeJSON(&reply_root);
	if(reply_para) ParameterFreeJSON(&reply_para);
	
#endif
	
	return NULL;
}


