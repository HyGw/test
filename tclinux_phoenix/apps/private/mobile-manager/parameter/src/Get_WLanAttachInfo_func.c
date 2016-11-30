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
#include "Get_WLanAttachInfo_func.h"
#include "libcompileoption.h"

json_t* GetWLanAttachInfo(json_t* req_root, json_t* req_para)
{
	json_t *cursor = NULL, *reply_root = NULL, *reply_para = NULL;
	char *wlannetinfo = NULL;
	char wlancount[8] = {0};
	int count = 0;

	mobile_printf(MOBILE_INFO,"%s:start!...\n",__FUNCTION__);

	if(!TCSUPPORT_WLAN_VAL){
		mobile_printf(MOBILE_INFO,"%s:wlan is not support!...\n",__FUNCTION__);
		return ParameterBuildJSON(req_root,req_para,CMD_UNRECOGNIZE);
	}

	wlannetinfo = (char *)malloc( WLANINFOMAXENTRY*WLANINFOENTRYSIZE*4 );
	if(wlannetinfo == NULL){
		mobile_printf(MOBILE_ERR, "%s, malloc err !\n", __FUNCTION__);
		return NULL;
	}		
	memset(wlannetinfo, 0, WLANINFOMAXENTRY*WLANINFOENTRYSIZE*4);
	count = getWLanEntryInfo(wlannetinfo);
	sprintf(wlancount, "%d", count);
	
	//build parameter
	if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
		goto err;
	}	
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_NUM, wlancount)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_INFO, wlannetinfo)){
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
	mobile_printf(MOBILE_ERR, "%s: reply json pkt=%s!...\n", __FUNCTION__,value_str);
	free(value_str);	
#endif	

	free(wlannetinfo);
	return reply_root;

err:
	if(reply_root) ParameterFreeJSON(&reply_root);
	if(reply_para) ParameterFreeJSON(&reply_para);
	free(wlannetinfo);
	return NULL;

}

int getWLanEntryInfo(char *buf)
{
	char nodeName[48]={0}, bssidnum_str[5]={0}, temp[60]={0}, nodeattr[16]={0};
	char *pSrc = NULL, *pDst = NULL, *pTemp;
	char macstr[16] = {0};
	int bssidnum = 0, ssidnum = 0, i = 0, j=0;
	int count = 0;
	
	tcapi_get( "WLan_Common", "BssidNum", bssidnum_str );
	bssidnum = atoi(bssidnum_str);

	if((bssidnum < 1) ||(bssidnum > 4))
		return 0;
	
	for(i=0; i<bssidnum; i++)
	{
		memset(nodeattr, 0, sizeof(nodeattr));
		sprintf(nodeattr, "NUM%d", i);
		
		memset(temp, 0, sizeof(temp));
		tcapi_get("wifiMacTab_Common", nodeattr, temp);
		ssidnum = atoi(temp);
		if(ssidnum <= 0)
			continue;
		else if(ssidnum > WLANINFOMAXENTRY)
			ssidnum = WLANINFOMAXENTRY;

		for(j=0; j<ssidnum; j++)
		{
			memset(nodeName, 0, sizeof(nodeName));
			sprintf(nodeName, "wifiMacTab_Entry%d", j);
			
			memset(nodeattr, 0, sizeof(nodeattr));
			sprintf(nodeattr, "MAC%d", i);
			memset(temp, 0, sizeof(temp));
			tcapi_get(nodeName, nodeattr, temp);
			
			memset(macstr, 0, sizeof(macstr));
			pSrc = temp;
			pDst = macstr;
			do{
				if(*pSrc != ':'){
					*pDst++ = *pSrc;
				}
			}while(*pSrc++);

			if(count > 0)
				strcat(buf," /");
			strcat(buf,"MAC:");
			strcat(buf,macstr);
			
			memset(nodeattr, 0, sizeof(nodeattr));
			sprintf(nodeattr, "IP%d", i);
			memset(temp, 0, sizeof(temp));
			tcapi_get(nodeName, nodeattr, temp);
			strcat(buf," IP:");
			pTemp = strchr(temp, ' ');
			if((pTemp=strchr(temp, ' '))!=NULL)
				*pTemp = '\0';
			strcat(buf,temp);

			memset(nodeName, 0, sizeof(nodeName));
			sprintf(nodeName, "WLan_Entry%d", i);
			memset(temp, 0, sizeof(temp));
			tcapi_get(nodeName, "SSID", temp);	
			strcat(buf," SSIDName:");
			strcat(buf,temp);	

			memset(temp, 0, sizeof(temp));
			if(TCSUPPORT_WLAN_RT3390_VAL || TCSUPPORT_WLAN_RT3090_VAL || TCSUPPORT_WLAN_RT5392_VAL|| TCSUPPORT_WLAN_MULTIDRIVER_VAL|| TCSUPPORT_WLAN_MT7592_VAL){
				memset(nodeName, 0, sizeof(nodeName));
				sprintf(nodeName, "wifiMacTab_Entry%d", j);
				memset(nodeattr, 0, sizeof(nodeattr));
				sprintf(nodeattr, "RSSIA%d", i);
				tcapi_get(nodeName, nodeattr, temp);
			}
			strcat(buf," PowerLevel:");
			strcat(buf,temp);	

			count ++;
		}						
	}
	
	return count;
}
