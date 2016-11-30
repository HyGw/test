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

#include "Set_SSID_func.h"


/*______________________________________________________________________________
**	setPwdByAuthMode
**		set WLAN paasword by ssid index.
**
**	descriptions:
**		the init function of syslog.
**	parameters:
**		ssidIndex: the index of WLAN
**      pwd: the password of SSID
**	return:
**		
**_______________________________________________________________________________
*/
#if 0
void setPwdByAuthMode(int ssidIndex, char* pwd){
	char authmode[16] = {0},nodeName[48] = {0};

	sprintf(nodeName, "WLan_Entry%d", ssidIndex-1);
	tcapi_get(nodeName, "AuthMode", authmode);

	
	if(strcmp(authmode, "WEP-64Bits") == 0){
		if(pwd == NULL)
			tcapi_set(nodeName, "Key1Str","");
		else{
			if(strlen(pwd)>0)
				tcapi_set(nodeName, "Key1Str",pwd);
		}
	}
	else if((strcmp(authmode, "WPAPSK") == 0) || (strcmp(authmode, "WPA2PSK") == 0) || (strcmp(authmode, "WPAPSKWPA2PSK") == 0)){
		if(pwd == NULL)
			tcapi_set(nodeName, "WPAPSK","");
		else{
			if(strlen(pwd)>0)
				tcapi_set(nodeName, "WPAPSK",pwd);
		}
	}

	return;
}
#endif

json_t* Set_SSID_func(json_t* req_root, json_t* req_para, int* ssidIndex)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	char *cSSIDIndex = NULL, nodeName[48] = {0}, *cur_ssid_name = NULL, *cur_authmode = NULL, *maxBitRate = NULL,
		*ssidEnable = NULL, *channel = NULL, *pwd = NULL;
	char authmode[16] = {0},Power[3] = {0};
	int cur_ssid_idx = 0,channel_i = 0,maxBitRate_i=0;
	int ret = 0;
	json_t *cur_ssid_name_node = NULL, *cur_authmode_node = NULL, *maxBitRate_node = NULL,*ssidEnable_node = NULL, 
		*channel_node = NULL, *pwd_node = NULL;

	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__,__LINE__);
	if(!TCSUPPORT_WLAN_VAL){
		mobile_printf(MOBILE_INFO,"%s:wlan is not support!...\n",__FUNCTION__);
		return ParameterBuildJSON(req_root,req_para,CMD_UNRECOGNIZE);
	}
	
	cur_ssid_name_node = ParameterSearchFirstLabel(req_para, "SSID");
	pwd_node = ParameterSearchFirstLabel(req_para, "PWD");
	cur_authmode_node = ParameterSearchFirstLabel(req_para, "ENCRYPT");
	maxBitRate_node = ParameterSearchFirstLabel(req_para, "PowerLevel");
	channel_node = ParameterSearchFirstLabel(req_para, "Channel");
	ssidEnable_node = ParameterSearchFirstLabel(req_para, "Enable");
	cSSIDIndex = ParameterSearchValue(req_para, "SSIDIndex");

	if(cSSIDIndex != NULL){
		cur_ssid_idx = atoi(cSSIDIndex);
		if( (cur_ssid_idx > 8) && (cur_ssid_idx < 0) )
			ret=-2;
		else{
			if(cur_ssid_idx > 0 ){
				if((cur_ssid_name_node != NULL) && (cur_authmode_node != NULL) && (pwd_node != NULL)
					&& (maxBitRate_node != NULL) && (channel_node != NULL) && (ssidEnable_node != NULL)){
					cur_ssid_name = cur_ssid_name_node->child->text;
					pwd = pwd_node->child->text;
					cur_authmode = cur_authmode_node->child->text;
					maxBitRate = maxBitRate_node->child->text;
					channel = channel_node->child->text;
					ssidEnable = ssidEnable_node->child->text;
					if(cur_authmode != NULL && strlen(cur_authmode)>0){
						if(pwd == NULL && strcmp(cur_authmode, "1") == 0 ){
							strcpy(authmode,"OPEN");
						}
						else{
							if((strcmp(cur_authmode, "1") == 0) || (strcmp(cur_authmode, "2") ==0)){
								strcpy(authmode,"WEP-64Bits");
							}
							else if(strcmp(cur_authmode, "3") == 0){
								strcpy(authmode,"WPAPSK");
							}
							else if(strcmp(cur_authmode, "4") == 0){
								strcpy(authmode,"WPA2PSK");
							}
							else if( strcmp(cur_authmode, "5") == 0 ) {
								strcpy(authmode, "WPAPSKWPA2PSK");
							}
							else{
								ret = -3;
							}
						}
					}
					if(maxBitRate != NULL && strlen(maxBitRate) > 0){
						maxBitRate_i=atoi(maxBitRate);
						if(maxBitRate_i >= 0 && maxBitRate_i <= 40)
							strcpy(Power, "5");
						else if(maxBitRate_i > 40 && maxBitRate_i < 80)
							strcpy(Power,"3");
						else if(maxBitRate_i >= 80 && maxBitRate_i <= 100)
							strcpy(Power,"1");
						else
							ret = -4;
					}
					if(channel != NULL && strlen(channel)>0){
						channel_i = atoi(channel);
						if(channel_i<0||channel_i>13)
							ret = -5;
					}
				}
				else
					ret = -1;
			}
			else{
				if(ssidEnable_node == NULL)
					ret = -1;
				else
					ssidEnable = ssidEnable_node->child->text;
			}	
		}
	}
	else
		ret=-1;
	if(ret == 0){
		if(cur_ssid_idx == 0){
			if(ssidEnable == NULL)
				tcapi_set("WLan_Common", "APOn", "");
			else{
				if(strcmp(ssidEnable, "1")==0)
					tcapi_set("WLan_Common", "APOn", "0");
				if(strcmp(ssidEnable, "0")==0)
					tcapi_set("WLan_Common", "APOn", "1");
			}
		}
		else{
			sprintf(nodeName, "WLan_Entry%d", cur_ssid_idx-1);
			if(cur_ssid_name==NULL)
				tcapi_set(nodeName, "SSID","");
			else{
				if(strlen(cur_ssid_name)>0)
					tcapi_set(nodeName, "SSID",cur_ssid_name);
			}
			if(cur_authmode==NULL)
				tcapi_set(nodeName, "AuthMode", "");
			else{
				if(strlen(cur_authmode)>0){
					if(pwd == NULL && strcmp(cur_authmode, "1") == 0 ){
						tcapi_set(nodeName, "AuthMode", authmode);
						tcapi_set(nodeName, "EncrypType", "NONE");
					}
					else{
						tcapi_set(nodeName, "EncrypType", "TKIP");
						if(strcmp(cur_authmode, "1") == 0 ){
							tcapi_set(nodeName, "WPSConfMode", "0");
							tcapi_set(nodeName, "AuthMode", authmode);
							tcapi_set(nodeName, "WEPAuthType", "OpenSystem");
							tcapi_set(nodeName, "DefaultKeyID", "1");
							if(strlen(pwd)>0)
								tcapi_set(nodeName, "Key1Str",pwd);
						}
						else if(strcmp(cur_authmode, "2") == 0 ){
							tcapi_set(nodeName, "WPSConfMode", "0");
							tcapi_set(nodeName, "AuthMode", authmode);
							tcapi_set(nodeName, "WEPAuthType", "SharedKey");
							tcapi_set(nodeName, "DefaultKeyID", "1");
							if(pwd == NULL)
								tcapi_set(nodeName, "Key1Str","");
							else{
								if(strlen(pwd)>0)
									tcapi_set(nodeName, "Key1Str",pwd);
							}
						}
						else{
							tcapi_set(nodeName, "AuthMode", authmode);
							if(pwd == NULL)
								tcapi_set(nodeName, "WPAPSK","");
							else{
								if(strlen(pwd)>0)
									tcapi_set(nodeName, "WPAPSK",pwd);
							}
						}
					}
				}else if(strlen(cur_authmode)==0){
					if(pwd == NULL){
						tcapi_set(nodeName, "Key1Str","");
						tcapi_set(nodeName, "WPAPSK","");
					}
					else if(strlen(pwd)>0){
						tcapi_set(nodeName, "Key1Str",pwd);
						tcapi_set(nodeName, "WPAPSK",pwd);
					}
				}
			}
			
			if(maxBitRate==NULL)
				tcapi_set("WLan_Common", "TxPowerLevel", "");
			else{
				if(strlen(maxBitRate)>0)
					tcapi_set("WLan_Common", "TxPowerLevel", Power);
			}
			if(channel==NULL)
				tcapi_set("WLan_Common", "Channel", "");
			else{
				if(strlen(channel)>0)
					tcapi_set("WLan_Common", "Channel", channel);
			}
			if(ssidEnable==NULL)
				tcapi_set(nodeName, "EnableSSID", "");
			else{
				if(strcmp(ssidEnable, "1")==0)
					tcapi_set(nodeName, "EnableSSID", "0");
				if(strcmp(ssidEnable, "0")==0)
					tcapi_set(nodeName, "EnableSSID", "1");
			}
		}

		
		*ssidIndex = cur_ssid_idx;
		mobile_printf(MOBILE_INFO,"%s:*ssidIndex =%d!!!\n",__FUNCTION__,*ssidIndex);
	}
	
	//build parameter
	if(ret == -1){
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "input can not be NULL")){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}else if(ret == -2){
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "SSIDIndex does not meet the requirements")){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}else if(ret == -3){
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "ENCRYPT does not meet the requirements")){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}else if(ret == -4){
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "PowerLevel does not meet the requirements")){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}else if(ret == -5){
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "channel does not meet the requirements")){
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
	return NULL;
}

int delaysetwifiinfo(int done, void *arg, void *arg1)
{
	char nodeName[48] = {0};	
	char strIndex[8] = {0};
	int ssidIndex = 0;

	mobile_printf(MOBILE_INFO, "%s: start!...\n", __FUNCTION__);

	ssidIndex = arg1;
	if(ssidIndex < 0 || ssidIndex > 4){
		mobile_printf(MOBILE_ERR, "%s: ssidIndex =%d! error!!!\n", __FUNCTION__,ssidIndex);
		return -1;
	}

	sprintf(strIndex, "%d", ssidIndex-1 );
	tcapi_set( "WebCurSet_Entry", "wlan_id", strIndex);
	sprintf(nodeName, "WLan_Entry%d", ssidIndex-1);
	mobile_printf(MOBILE_ERR, "nodeName =%s\n", nodeName);
	if(!done){
		tcapi_commit(nodeName);
		tcapi_save();
	}
	else{
		tcapi_read(nodeName);
	}

	
	return 0;
}

