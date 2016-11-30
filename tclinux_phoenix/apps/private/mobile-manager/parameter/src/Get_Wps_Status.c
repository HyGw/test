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

#include "Get_Wps_Status.h"

#if defined(TCSUPPORT_CT_JOYME)
json_t* GetWpsStatus(json_t* req_root, json_t* req_para)
{
	json_t  *reply_root = NULL, *reply_para = NULL;
	int ret = -1;
	char wpssta[8] = {0},ip[512]={0};
	char nodename[64] = {0},tempbuf[512] = {0},ipbuf[20] = {0},ssid[8] = {0}, wpsStatusNode[20] = {0};
	FILE *fp =NULL;
	char macstr[32]= {0};
	char mac[18] = {0}, ipaddr[16] = {0},expire[10] = {0}, hostname[32] = {0}; 
	char *path = "/etc/udhcp_lease";
	
	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__,__LINE__);

	strcpy(nodename,"WLan_Common");
	tcapi_get(nodename,"APOn",tempbuf);
	if(!strcmp(tempbuf,"1")){
#if defined(TCSUPPORT_WLAN_MULTI_WPS)
		if(tcapi_get("WebCurSet_Entry","wlan_id",ssid)<0)
		strcpy(nodename,"WLan_Entry0");
		else
			sprintf(nodename,"WLan_Entry%s",ssid);
#endif

		memset(tempbuf,0,sizeof(tempbuf));
		tcapi_get(nodename,"EnableSSID",tempbuf);
		if(!strcmp(tempbuf,"1")){
			ret = 0;
			strcpy(nodename,"Info_WLan");
#if defined(TCSUPPORT_WLAN_MULTI_WPS)
			sprintf(wpsStatusNode,"wlanWPSStatus_%s",ssid);
			tcapi_get(nodename,wpsStatusNode,tempbuf);
#endif

			if(!strcmp(tempbuf,"In progress")){
				strcpy(wpssta,"0");
				strcpy(ip,"");
			}
			else if(!strcmp(tempbuf,"WPS Process Fail")){
				strcpy(wpssta,"1");
				strcpy(ip,"");
			}
			else if(!strcmp(tempbuf,"Configured")){
				strcpy(wpssta,"2");
				tcapi_get("Info_WLan", "wpsmac", macstr);
				fp = fopen(path,"r");
				if(fp == NULL)
					strcpy(ip,"");
				else{
						memset(tempbuf,0,sizeof(tempbuf));
					while(feof(fp) == 0 ){
						fgets(tempbuf,sizeof(tempbuf),fp);
						sscanf(tempbuf, "%s %s %s %s",mac, ipaddr, expire, hostname);
						if(strcmp(macstr,mac) == 0)
							strcpy(ip,ipaddr);
						
					}
				}
					
			}
			else if(!strcmp(tempbuf,"Idle")){
				strcpy(wpssta,"3");
				strcpy(ip,"");
			}
		}
	}
	if(ret == 0){
		//build parameter
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){//ok
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, WPSSTATUS, wpssta)){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_IP, ip)){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	else{
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "WIFI is not available")){
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

	if(fp) fclose(fp);
	if(reply_para) ParameterFreeJSON(&reply_para);
	return reply_root;
	
err:
	if(fp) fclose(fp);
	if(reply_root) ParameterFreeJSON(&reply_root);
	if(reply_para) ParameterFreeJSON(&reply_para);
	return NULL;
}
#endif
