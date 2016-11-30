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

#include "Set_AttachDeviceBandWidth_func.h"


/*______________________________________________________________________________
**	Set_AttachDeviceBandWidth_func
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
json_t* Set_AttachDeviceBandWidth_func(json_t* req_root, json_t* req_para)
{
#if defined(TCSUPPORT_CT_JOYME)	
	json_t *reply_root = NULL, *reply_para = NULL;
	char *cMacAddr = NULL, *cUsBandWidth = NULL, *cDsBandWidth = NULL;	
	CtMacAddr macAddr;
	int mac[6];
	int ret = 0;

	mobile_printf(MOBILE_INFO,"%s, line = %d\n",__FUNCTION__,__LINE__);
	cMacAddr = ParameterSearchValue(req_para, "MAC");
	cUsBandWidth = ParameterSearchValue(req_para, "UsBandwidth");
	cDsBandWidth = ParameterSearchValue(req_para, "DsBandwidth");

	if((cMacAddr != NULL) && (cUsBandWidth != NULL) &&  (cDsBandWidth != NULL)){
		sscanf(cMacAddr,"%02x%02x%02x%02x%02x%02x",
			&mac[0],&mac[1],&mac[2],&mac[3],&mac[4],&mac[5]);

		macAddr.macAddr[0] = (unsigned char)mac[0];
		macAddr.macAddr[1] = (unsigned char)mac[1];
		macAddr.macAddr[2] = (unsigned char)mac[2];
		macAddr.macAddr[3] = (unsigned char)mac[3];
		macAddr.macAddr[4] = (unsigned char)mac[4];
		macAddr.macAddr[5] = (unsigned char)mac[5];

		

		mobile_printf(MOBILE_INFO,"%s, line = %d,cMacAddr=%s\n",__FUNCTION__,__LINE__,cMacAddr);
		mobile_printf(MOBILE_INFO,"%s, line = %d,cUsBandWidth=%s\n",__FUNCTION__,__LINE__,cUsBandWidth);
		mobile_printf(MOBILE_INFO,"%s, line = %d,cDsBandWidth=%s\n",__FUNCTION__,__LINE__,cDsBandWidth);
		ret = joyme_lanSetDevMaxBandwidth(&macAddr, atoi(cUsBandWidth), atoi(cDsBandWidth));
	}else{
		mobile_printf(MOBILE_INFO,"%s, line = %d, search node fail\n",__FUNCTION__,__LINE__);	
		ret = -1;
	}
	//build parameter
	if(ret == -1){
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "set attach device bandwidth fail")){
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

