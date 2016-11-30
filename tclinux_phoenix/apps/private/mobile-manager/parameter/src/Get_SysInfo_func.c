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
#include "Get_DeviceInfo_All_func.h"
#include "Get_SysInfo_func.h"
#include "libcompileoption.h"

json_t* GetSysInfo(json_t* req_root, json_t* req_para)
{
	json_t* reply_root = NULL, *reply_para = NULL;
	char swVersion[64] = {0};
	char hwVersion[64] = {0};
	char ProductClass[64] = {0};
	char gateWayName[32] = {0};
	char ModelName[164] = {0};
	char devtype1[12] = {0};
	char devcapval[100] = {0};
	char iscardsepa[4] = {0};
	char cardno[60] = {0};
	char carsta[4] = {0};
	char uplinkval[12] = {0};
	char dslite[10] = {0};
	char cpuclass[10] = {0};
#if defined(TCSUPPORT_CT_JOYME)
	int iflashsize = 0, iramsize = 0;
	char LED[10] = {0}, DevName[64] = {0}, FlashSize[64] = {0}, RamSize[64] = {0};
	char WifiMode[64] = {0}, NFC[10] = {0};
	char WanIp[32] = {0}, LanIp[32] = {0}, tempbuf[32] = {0}, nodename[64] = {0};
#endif
	
	mobile_printf(MOBILE_INFO,"%s:start!...\n",__FUNCTION__);

	getDeviceBasicInfo(swVersion, hwVersion, ProductClass, ModelName, devtype1, cpuclass);
	getDevCapabilityValue(devcapval);
	GetSimCardInfo(iscardsepa, cardno, carsta);
	getUPLinkValue(uplinkval);
	
#if defined(TCSUPPORT_CT_JOYME)
	sprintf(LED,"%s","1");
	fileRead(GATEWAY_PATH, DevName, sizeof(DevName));
	
	/*FlashSize 128M, RamSize 256M*/
	iflashsize = 128*1024*1024;
	iramsize = 256*1024*1024;
	sprintf(FlashSize, "%d", iflashsize);
	sprintf(RamSize, "%d", iramsize);
	sprintf(WifiMode, "%s", "802.11b/802.11g/802.11n");

	/*LanIPAddr,WanIPAddr*/
	tcapi_get("Lan_Entry0","IP",LanIp);

	tcapi_get("WanInfo_Common","DefRouteIndexv4",tempbuf);
	if(!strcmp(tempbuf,"N/A")){
		strcpy(WanIp,"0.0.0.0");
	}
	else{
		sprintf(nodename,"WanInfo_Entry%d",atoi(tempbuf));
		tcapi_get(nodename,"Status",tempbuf);
		if(!strcmp(tempbuf,"up")){
			tcapi_get(nodename,"IP",WanIp);
		}
		else{
			strcpy(WanIp,"0.0.0.0");
		}
	}

	sprintf(NFC,"%s","0");
#endif

	//build parameter
	if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
		goto err;
	}			
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_SWVERSION, swVersion)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_HDVERSION, hwVersion)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_PRODUCTCLASS, ProductClass)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_CPUCLASS, cpuclass)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_DEVTYPE, ModelName)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_DEVTYPE1, devtype1)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_CAPABILITY, devcapval)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_CARD, iscardsepa)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_UPLINK, uplinkval)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_CARDNO, cardno)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}

	if(TCSUPPORT_IPV6_VAL){
		if(TCSUPPORT_CT_DSLITE_VAL){
			if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_IPV6, "2")){
				mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
				goto err;
			}
		}
		else{
			if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_IPV6, "1")){
				mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
				goto err;
			}
		}
	}
	else{
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_IPV6, "0")){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}

	if(TCSUPPORT_CT_VLAN_BIND_VAL){
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_VLANBIND, "1")){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	else{
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_VLANBIND, "0")){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}

#if defined(TCSUPPORT_CT_JOYME)
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_LED, LED)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FLASHSIZE, FlashSize)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_RAMSIZE, RamSize)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_DevName, DevName)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_WIFIMode, WifiMode)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_NFC, NFC)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_WANIPADDR, WanIp)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_LANIPADDR, LanIp)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
#endif


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
