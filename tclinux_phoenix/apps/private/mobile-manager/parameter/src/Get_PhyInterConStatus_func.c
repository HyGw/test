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
#include "Get_PhyInterConStatus_func.h"
#define  ETH_PORT_STS  "/proc/tc3162/eth_port_status"


json_t* GetPhyInterConStatus(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	char LanPortConStatus[4][8] = {0};
	char WanPortConStatus[8] = {0};
	char WIFIModuleStatus[8] = {0};
	int ret = -1;

	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__, __LINE__);

	memset(LanPortConStatus, 0, sizeof(LanPortConStatus));

	//Get LAN Physical interface connection status
	ret=get_LanPhyInterConStatus(LanPortConStatus);

	//Get WAN Physical interface connection status
	get_WanPhyInterConStatus(WanPortConStatus);

	//Get WLAN Physical interface connection status
	get_WLanPhyInterConStatus(WIFIModuleStatus);

	//build parameter
	if(ret == -1){
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "LANPhyInterConStatus file Is Not Exist")){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	else{
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){//ok
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_LAN1STATUS, LanPortConStatus[0])){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_LAN2STATUS, LanPortConStatus[1])){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddJSONChild(reply_para, PARA_LAN3STATUS, TCSUPPORT_CT_2PORTS_VAL ? JSON_NULL : JSON_STRING, LanPortConStatus[2])){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddJSONChild(reply_para, PARA_LAN4STATUS, TCSUPPORT_CT_2PORTS_VAL ? JSON_NULL : JSON_STRING, LanPortConStatus[3])){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_WANSTATUS, WanPortConStatus)){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddJSONChild(reply_para, PARA_WIFIMODULESTATUS, TCSUPPORT_WLAN_VAL ? JSON_STRING : JSON_NULL, WIFIModuleStatus)){
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

int get_LanPhyInterConStatus(char LanPortConStatus[4][8])
{
	char result[20] = {0};
	int LanPortConValue[4] = {0}, i = 0;
	FILE *fp=NULL;

	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__, __LINE__);

	memset(result, 0, sizeof(result));
	if(access(ETH_PORT_STS, F_OK) != 0)
		return -1;
	fp = fopen(ETH_PORT_STS, "r+");
	if(!fp)
		return -1;	
	fgets(result, sizeof(result), fp);
	fclose(fp);
	
	sscanf(result,"%d %d %d %d", LanPortConValue, LanPortConValue+1, LanPortConValue+2, LanPortConValue+3);

	for(i = 0;i < 4; i++){
		if(0 == LanPortConValue[i])
			strcpy(LanPortConStatus[i], "off");
		else if(1 == LanPortConValue[i])
			strcpy(LanPortConStatus[i], "on");
	}

	return 0;
}

/*
    rpc of tr069 parameters: InternetGatewayDevice.WANDevice.1.X_CT-COM_EponInterfaceConfig.
	                                 InternetGatewayDevice.WANDevice.1.X_CT-COM_GponInterfaceConfig.
*/
void get_WanPhyInterConStatus(char WanPortConStatus[8])
{
	char nodeValue[32] = {0};

	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__, __LINE__);

	tcapi_get("XPON_LinkCfg","LinkSta",nodeValue);
	if(!strcmp(nodeValue, "0"))
	{
		strcpy(WanPortConStatus, "off");
	}
	else
	{
		strcpy(WanPortConStatus, "on");
	}
		
	return;
}

void get_WLanPhyInterConStatus(char WIFIModuleStatus[8])
{
	char nodeValue[32] = {0};

	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__, __LINE__);

	tcapi_get("WLan_Common","APOn",nodeValue);

	if(TCSUPPORT_WLAN_VAL && (!strcmp(nodeValue, "1")))
	{
		strcpy(WIFIModuleStatus, "on");
	}
	else
	{
		strcpy(WIFIModuleStatus, "off");
	}

	return;
}

