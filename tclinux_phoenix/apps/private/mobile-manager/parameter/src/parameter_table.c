/*
** $Id: //BBN_Linux/Branch/Branch_for_MT7505_IC_20131122/tclinux_phoenix/apps/private/TR69_64/cwmp/cwmpParameter/Sources/cwmpParameter_table.c#1 $
*/
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

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <netinet/in.h>

#include "json.h"
#include "base64.h"
#include "info.h"
#include "parameter_table.h"
#include "if_sysinfo_func.h"
#include "if_window_func.h"
#include "Get_SNInfo_func.h"
#include "Get_SysInfo_func.h"
#include "Get_CpuUsage_func.h"
#include "Get_MemUsage_func.h"
#include "Get_PhyInterConStatus_func.h"
#include "Get_PPP_DialStatus_func.h"
#include "Get_WanConnectNum_func.h"
#include "Get_AssignedWanConnectInfo_func.h"
#include "Get_AssignedSSIDInfo_func.h"
#include "Get_VoIPInfo_func.h"
#include "Set_RegisterInfo_func.h"
#include "Get_RegisterInfo_func.h"
#include "Get_PonStatus_func.h"
#include "Get_PonRegistrationInfo_func.h"
#include "Get_WLanInfo_func.h"
#include "Get_TraceRouteResult_func.h"
#include "Get_PingResult_func.h"
#include "Get_LAN_Flow_func.h"
#include "Get_LOID_func.h"
#include "Get_LanInfo_func.h"
#include "Get_WanRealRate_func.h"
#include "Get_Gateway_Blacklist_func.h"
#include "Get_VoIPRegInfo_func.h"
#include "Get_Duration_func.h"
#include "Get_DeviceInfo_All_func.h";
#include "Set_wireless_schedule_func.h"
#include "Set_GatewayTime_func.h"
#include "Set_PlatformParam_func.h"
#include "Set_HttpCfg_func.h"
#include "Set_HttpCfg_func1.h"
#include "Set_Led.h"
#include "Set_DownHttpReq_func.h"
#include "Set_Access_of_Interface_func.h"
#include "Set_Gateway_DNS_func.h"
#include "Set_SSID_func.h"
#include "Close_SSID_func.h"
#include "Control_Gateway_Terminal_Access_func.h"
#include "DevReboot_func.h"
#include "DNSRedirect.h"
#include "TraceRouteDiagnosticsRequest_func.h"
#include "InformDiagnosticsRequest_func.h"
#include "PingDiagnosticsRequest_func.h"
#include "Query_Current_Business_func.h"
#include "PPPDialControl_func.h"
#include "Upgrade_func.h"
#include "DevLocalRecovery_func.h"
#include "Get_WLanAttachInfo_func.h"
#if defined(TCSUPPORT_CT_JOYME)
#include "MulticastDiagReq_func.h"
#include "SetHgLedIndicatorTimer_func.h"
#include "SetHgWifiTimerEx_func.h"
#include "GetHgWifiTimerEx_func.h"
#include "SetSleepStatus_func.h"
#include "GetSleepStatus_func.h"
#include "SetRight1_func.h"
#include "SetOSRight_func.h"
#include "Cwmp_RPC_func.h"
#include "Get_HW_IP_func.h"
#include "GET_NETWORK_DNS.h"
#include "Attach_Wan_DNS_Tunnel_func.h"
#include "SET_HG_DEV_NAME.h"
#include "GET_HG_DEV_NAME.h"
#include "Set_AttachDevName.h"
#include "Set_AttachDeviceBandWidth_func.h"
#include "Get_AttachDeviceBandWidth_func.h"
#include "Get_RealRate_func.h"
#include "Device_Speed_func.h"
#include "SET_WPS_ON.h"
#include "SET_WPS_OFF.h"
#include "Get_Wps_Status.h"
#include "Open_SSID_func.h"
#include "Set_Wanl2tp.h"
#endif

phone_para_t gPhone_Para[] = {
#if defined(TCSUPPORT_CT_JOYME)
	{"ADDOBJECT",	RPC_AddObject_func, PERMIT_BOTH, 0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
	{"ATTACH_WANL2TP_TUNNEL", Attach_WANL2TP_Tunnel,			PERMIT_BOTH, 	0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
	{"ATTACH_WAN_DNS_TUNNEL", Attach_Wan_DNS_Tunnel_func, PERMIT_BOTH, 0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
#endif
	{"CHECK_PASSWD_PARAM", 	Cmd_CheckPassword, 	PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
#if defined(TCSUPPORT_CT_JOYME)
	{"CREATE_WANL2TP_TUNNEL", Create_WANL2TP_Tunnel,			PERMIT_BOTH, 	0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
	{"DELETEOBJECT",	RPC_DeleteObject_func, PERMIT_BOTH, 0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
	{"DETACH_WANL2TP_TUNNEL", Detach_WANL2TP_Tunnel,			PERMIT_BOTH, 	0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
	{"DETACH_WAN_DNS_TUNNEL", Detach_Wan_DNS_Tunnel_func, PERMIT_BOTH, 0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
	{"DEVICE_SPEED",	 Device_Speed_func, PERMIT_BOTH, 0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
	{"GETPARAMETERVALUES",	RPC_GetParameterValues_func, PERMIT_BOTH, 0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
	{"GET_ATTACH_DEVICE_BANDWIDTH", Get_AttachDeviceBandWidth_func, 				PERMIT_BOTH,	0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
#endif
	{"GET_ATTACH_DEVICE_RIGHT", Get_Gateway_Blacklist_func, PERMIT_BOTH, 0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
#if defined(TCSUPPORT_CT_JOYME)
	{"GET_ATTACH_DEVICE_STORAGE_ACCESS_BLACKLIST", Get_Attach_Device_Storage_Access_func, PERMIT_BOTH, 0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
#endif
	{"GET_DEVICE_INFO_ALL", 	GetDeviceInfoAll, 		PERMIT_BOTH,		1, 0, NULL, PTHREAD_MUTEX_INITIALIZER},	
#if defined(TCSUPPORT_CT_JOYME)
	{"GET_HG_DEV_NAME", 	GetHgdevName,		PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
#endif
	{"GET_HG_LOID", 			Get_LOID_func, 			PERMIT_BOTH, 	0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
#if defined(TCSUPPORT_CT_JOYME)
	{"GET_HG_WIFI_TIMER_EX",	GetHgWifiTimerEx,		PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
	{"GET_HW_IP", 			Get_HW_IP_func, 			PERMIT_BOTH, 	0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
#endif
	{"GET_LAN_INFO2", 		Get_LAN_Flow_func, 		PERMIT_BOTH, 	0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
	{"GET_LAN_NET_INFO", 		GetLanNetInfo, 			PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},	
#if defined(TCSUPPORT_CT_JOYME)
	{"GET_NETWORK_DNS", 		Get_Network_Dns, 			PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
#endif
	{"GET_PING_RESULT", 		Get_PingResult, 			PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
	{"GET_PONINFORM_REQ", 	Get_PonStatus, 			PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
	{"GET_PON_STATUS", 		Get_PonRegistrationInfo, 	PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
	{"GET_SERVICE", 			Query_Current_Business_func, PERMIT_BOTH, 0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
#if defined(TCSUPPORT_CT_JOYME)
	{"GET_SLEEP_STATUS",		GetSleepStatus, 	PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
#endif
	{"GET_SN_INFO", 			Cmd_GetSNInfo, 			PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},	
	{"GET_TIME_DURATION", 	Get_Duration_func, 		PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
	{"GET_TRACEROUTE_RESULT", Get_TraceRouteResult, 	PERMIT_BOTH,		1, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
	{"GET_VERSION", 			GetCommProtocolVer, 	PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},	
	{"GET_VOIP_INFO", 			Get_VoIPInfo,			PERMIT_BOTH, 	0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
	{"GET_VOIP_REG_STATUS", 	Get_VoIPRegInfo, 		PERMIT_BOTH, 	0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
#if defined(TCSUPPORT_CT_JOYME)
	{"GET_WANL2TP_TUNNEL_STATUS", Get_WANL2TP_Tunnel_Status,			PERMIT_BOTH, 	0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
#endif
	{"GET_WIFI_INFO", 			Get_AssignedSSIDInfo, 	PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
	{"GET_WLAN_ATTACH_INFO", 	GetWLanAttachInfo,		PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
	{"GET_WLAN_INTF_INFO", 	Get_WLanInfo,			PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
	{"HG_COMMAND_REBOOT", 	DevReboot, 				PERMIT_BOTH,		0, 1, DevBoot2Curset, PTHREAD_MUTEX_INITIALIZER},
	{"HG_LOCAL_RECOVERY", 	DevLocalRecovery, 		PERMIT_BOTH, 	0, 1, DevBoot2Defset, PTHREAD_MUTEX_INITIALIZER},
	{"HTTP_DOWNLOAD_REQUEST", 	SetDownHttpReq,	PERMIT_BOTH,		1, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
	{"INFORM_DIAG_REQ", 		InformDiagnosticsRequest, 	PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
#if defined(TCSUPPORT_CT_JOYME)
	{"LAN_DEVICE_ACCESS", 	LanDeviceAccess,			PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
	{"MULTICAST_DIAG_REQ", 		MulticastDiagReq,			PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
#endif
	{"PING_DIAG_REQ", 		PingDiagnosticsRequest,		PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
	{"PPPOE_DIAG_REQ", 		GetPPP_DialStatus,		PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
	{"PPPOE_DIAL_REQUEST", 	PPP_DialControl, 			PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
#if defined(TCSUPPORT_CT_JOYME)	
	{"QUERY_ATTACH_DEVICE_REALRATE", Get_AttachDeviceRealRateInfo,				PERMIT_BOTH,	0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
#endif
	{"QUERY_CPU_INFO", 		GetCpuUsage,			PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
	{"QUERY_MEM_INFO", 		GetMemUsage,			PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},	
#if defined(TCSUPPORT_CT_JOYME)	
	{"QUERY_REALRATE", 		Get_GatewayRealRateInfo,				PERMIT_BOTH,	0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
#endif
	{"QUERY_SYSTEM_INFO", 	GetSysInfo, 				PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
	{"QUERY_WAN_INFO", 		Get_AssignedWanConnectInfo, PERMIT_BOTH, 0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
	{"QUERY_WAN_NUM", 		Get_WanConnectNum, 	PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
	{"QUERY_WAN_REALRATE", 	GetWanRealRate, 		PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
	{"REGISTER", 				SetRegisterInfo,			PERMIT_BOTH, 	0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
	{"REGISTER_GETSTAT", 		Get_RegisterInfo,			PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
#if defined(TCSUPPORT_CT_JOYME)
	{"REMOVE_WANL2TP_TUNNEL", Remove_WANL2TP_Tunnel,			PERMIT_BOTH, 	0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
	{"SETPARAMETERVALUES",	RPC_SetParameterValues_func, PERMIT_BOTH, 0, 1, HandleCommit, PTHREAD_MUTEX_INITIALIZER},
	{"SET_ATTACH_DEVICE_BANDWIDTH", Set_AttachDeviceBandWidth_func, 				PERMIT_BOTH,	0, 0, NULL, PTHREAD_MUTEX_INITIALIZER}, 	
	{"SET_ATTACH_DEVICE_NAME", 	SetAttachDevName,		PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
#endif
	{"SET_ATTACH_DEVICE_RIGHT", 	Control_Gateway_Terminal_Access_func, PERMIT_BOTH, 0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
	{"SET_BUSINESS_PLATFORM_PARAM", 	Set_PlatformParam,		PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
	{"SET_HG_DATE_TIME_SYNC", 	SetGatewayTime,		PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
#if defined(TCSUPPORT_CT_JOYME)
	{"SET_HG_DEV_NAME", 	SetHgdevName,		PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},	
	{"SET_HG_LED_INDICATOR_TIMER", 	SetHgLedIndicatorTimer,		PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
#endif
	{"SET_HG_WIFI_TIMER", 		Set_wireless_schedule, 	PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
#if defined(TCSUPPORT_CT_JOYME)
	{"SET_HG_WIFI_TIMER_EX", 	SetHgWifiTimerEx,		PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
#endif
	{"SET_HTTP_CFG", 			SetHttpCfg, 				PERMIT_BOTH,		1, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
#if defined(TCSUPPORT_CT_JOYME)
	{"SET_HTTP_CFG1", 			SetHttpCfg1, 				PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
	{"SET_LED_STATUS",          Set_Led,		PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
#endif
        {"SET_NETWORK_DNS", 		Set_Gateway_DNS_func,	PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
#if defined(TCSUPPORT_CT_JOYME)
	{"SET_OS_RIGHT", 				SetOSRight, PERMIT_BOTH, 0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
#endif
	{"SET_REDIRECT_INFO", 		DNSRedirect, 			PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
	{"SET_RIGHT", 				Set_Access_of_Interface_func, PERMIT_BOTH, 0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
#if defined(TCSUPPORT_CT_JOYME)
	{"SET_RIGHT1", 				SetRight1, PERMIT_BOTH, 0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
	{"SET_SLEEP_STATUS",		SetSleepStatus,		PERMIT_BOTH,		0, 1, delaySetSleepAct, PTHREAD_MUTEX_INITIALIZER},
#endif
	{"SET_WD_INFO", 			SetWDInfo,				PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
	{"SET_WIFI_INFO", 			Set_SSID_func,			PERMIT_BOTH, 	0, 1, delaysetwifiinfo, PTHREAD_MUTEX_INITIALIZER},
	{"SET_WLAN_SSID_DISABLE", Close_SSID_func,			PERMIT_BOTH, 	0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
#if defined(TCSUPPORT_CT_JOYME)
	{"SET_WLAN_SSID_ENABLE", Open_SSID_func,			PERMIT_BOTH, 	0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
#endif
	{"SYSTEST", 				GetPhyInterConStatus, 	PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
	{"TRACEROUTE_DIAG_REQ", 	TraceRouteDiagnosticsRequest, PERMIT_BOTH,	0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
	{"UPGRADE", 				Upgrade, 				PERMIT_BOTH, 	0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
#if defined(TCSUPPORT_CT_JOYME)
	{"WPS_OFF", 					SetWpsOff,		PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},	
	{"WPS_ON", 					SetWpsOn,		PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},	
	{"WPS_QY", 					GetWpsStatus,		PERMIT_BOTH,		0, 0, NULL, PTHREAD_MUTEX_INITIALIZER},
#endif
	{NULL, 					NULL, 					PERMIT_BOTH, 	0, 0, NULL, PTHREAD_MUTEX_INITIALIZER}		
};

int GetParaTableLen(void){
	return  sizeof(gPhone_Para)/sizeof(phone_para_t);
}

void InterfaceRightinit()
{
	char rightlist[sizeof(gPhone_Para)/sizeof(phone_para_t)] = {0};
	int lenget = 0,i = 0, len_list = 0;
		
	if(!tcapi_get("mobile_Entry","InterfaceRight",rightlist)){
		len_list = sizeof(gPhone_Para)/sizeof(phone_para_t);
		lenget = strlen(rightlist);
		if(len_list == lenget){
			for(;i < len_list;i ++){
				switch (rightlist[i]){
					case '0':
						gPhone_Para[i].permit = PERMIT_LOCAL;
						break;
					case '1':
						gPhone_Para[i].permit = PERMIT_REMOTE;
						break;
					case '2':
						gPhone_Para[i].permit = PERMIT_BOTH;
						break;
					case '3':
						gPhone_Para[i].permit = PERMIT_NONE;
						break;
				}
			}
		}
	}
}

phone_para_ptr SearchParaTable(char *cmd_type, int *p)
{
	int top, mid, bot;
	int cmp;

	bot = 0;
	top = sizeof(gPhone_Para)/sizeof(phone_para_t) - 2;
	
	if(cmd_type == NULL){
		mobile_printf(MOBILE_ERR, "SearchParaTable: cmd_type is NULL\n");
		return NULL;
	}

	while( bot <= top ) {
		mid = (top + bot) >> 1;
		//mobile_printf(MOBILE_INFO, "SearchParaTable: cmd_type is %s, %s\n", cmd_type, gPhone_Para[mid].cmdtype);
		cmp = strcmp(cmd_type, gPhone_Para[mid].cmdtype); 
		if( cmp > 0 ) {
			bot = mid + 1;      /* look below mid */
		} else if( cmp < 0 ) {
			top = mid - 1;      /* look above mid */
		} else                  /* cmp == 0 */
		{
			mobile_printf(MOBILE_DEBUG, "SearchParaTable: cmd_type is %s, %s\n", cmd_type, gPhone_Para[mid].cmdtype);
			if(p) *p = mid;
			return gPhone_Para+mid;         /* match; return table index */
		}
    }

	return NULL;
}

void
ParameterDumpJSON(const json_t * object)
{
	json_render_tree(object);
}

json_err_e 
ParameterAddNewJSONChild(json_t * parent, const char *text_label, const char *text_value)
{
	json_err_e error;
	json_t *label = NULL,*value = NULL;

	/* verify if the parameters are valid */
	assert (parent != NULL);
	assert (text_label != NULL);
	//assert (text_value != NULL);

	/* enforce type coherence */
	assert (parent->type == JSON_OBJECT);

	/* create label json_value */
	label = json_new_string (text_label);
	if (label == NULL)
		goto err;
	
	//value = (text_value) ?  json_new_string (text_value) :  json_new_null();
	value = (text_value) ?  json_new_string (text_value) :  json_new_string("");
	if (value == NULL)
		goto err;

	/*insert value and check for error */
	error = json_insert_child (label, value);
	if (error != JSON_OK)
		goto err;
	value = NULL;
	/*insert value and check for error */
	error = json_insert_child (parent, label);
	if (error != JSON_OK)
		goto err;

	return JSON_OK;
err:
	if(label) json_free_value(&label);
	if(value) json_free_value(&value);
	return JSON_MEMORY;
	
}

json_err_e 
ParameterAddJSONChild(json_t *parent, const char *text_label, json_type_e type, const char *text_value)
{
	json_err_e error = JSON_MEMORY;
	json_t *value = NULL;

	/* verify if the parameters are valid */
	assert (parent != NULL);
	assert (text_label != NULL);
	//assert (text_value != NULL);

	/* enforce type coherence */
	assert (parent->type == JSON_OBJECT);

	value = json_new_child(type, type == JSON_NULL ? NULL : text_value);
	if (value == NULL)
		return error;
	
	/*insert value and check for error */
	error = json_insert_pair_into_object(parent, text_label, value);
	if (error != JSON_OK)
		goto err;

	return JSON_OK;
	
err:
	if(value) json_free_value(&value);
	return error;
	
}

int 
ParameterAddLabelByRequest(json_t* req_json, json_t* reply_root, const char *text_label){
	json_t *label = NULL;

	label = ParameterSearchFirstLabel(req_json, text_label);
	
	if (label == NULL) {
		tcdbg_printf("\r\nParameterAddLabelByRequest(): %s is null", text_label);
		return -1;
	}
		
	if(JSON_OK != ParameterCopyJSONLabel(reply_root, label)){
		mobile_printf(MOBILE_ERR, "%s:add child %s err!\n", __FUNCTION__,label);
		return -1;
	}
	
	return 0;
}

json_t* ParameterBuildJSONReplyHead(json_t* req_json, int reply_result)
{
	json_t* reply_root = NULL;
	char result_str[16] = {0};

	//creates the root node
	if(!(reply_root = ParameterNewJSONObject())){
		return NULL;
	}
	
	sprintf(result_str, "%d", reply_result);

	if(JSON_OK != ParameterAddJSONChild(reply_root, PARA_RESULT, JSON_NUMBER, result_str)){
		mobile_printf(MOBILE_ERR, "%s:add child "PARA_RESULT" err!\n", __FUNCTION__);
		goto err;
	}
	
	if(ParameterAddLabelByRequest(req_json, reply_root, PARA_ID)){
		goto err;
	}
	return reply_root;
	
err:
	if(reply_root) ParameterFreeJSON(&reply_root);
	return NULL;
	
}

json_t* ParameterBuildJSONReplyPara(json_t* req_para, int reply_status)
{
	json_t *reply_para = NULL;

	//creates the root node
	if(!(reply_para = ParameterNewJSONObject())){
		mobile_printf(MOBILE_ERR, "ParameterNewJSONObject:err!\n");
		return NULL;
	}

	if(ParameterAddLabelByRequest(req_para,reply_para,PARA_CMD_TYE)){
		goto err;
	}
	if(ParameterAddLabelByRequest(req_para,reply_para,PARA_SEQUENCE_ID)){
		goto err;
	}
if(reply_status == 1 || reply_status == 0){
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_STATUS, reply_status ? "1":"0")){
		goto err;
	}
}
else if(reply_status == 2){
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_STATUS, "2")){
			goto err;
	}
}
else{
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_STATUS, "3")){
			goto err;
	}
}
	return reply_para;

err:
	if(reply_para){
		ParameterFreeJSON(&reply_para);
	}
	return NULL;
}

int ParameterBuildJSONAddPara2Head(json_t* reply_root, json_t* reply_para)
{
	char *buffer = NULL, *enbuffer = NULL;
	int len = 0, enlen = 0, ret =0;	

	if(JSON_OK != json_tree_to_string(reply_para,&buffer)){
		goto err;
	}
	mobile_printf(MOBILE_INFO, "%s: parameter pkt=%s!...\n", __FUNCTION__, buffer);
	len   = strlen(buffer);
	enlen = len + len /3 + 4;
	enbuffer = malloc(enlen);
	if(!enbuffer){
		mobile_printf(MOBILE_ERR, "%s: malloc enbuffer err!...\n", __FUNCTION__);	
		goto err;
	}
	if(ret = base64_encode(enbuffer, &enlen, buffer, len)){
		mobile_printf(MOBILE_ERR, "%s: base64_encode err=%x!...\n", __FUNCTION__, ret);	
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_root, PARA_PARAMETER_RT, enbuffer)){
		mobile_printf(MOBILE_ERR, "%s: ParameterAddNewJSONChild "PARA_PARAMETER_RT"err!...\n", __FUNCTION__);	
		goto err;
	}
	
	free(buffer);
	free(enbuffer);
	return 0;
err:
	if(buffer) free(buffer);
	if(enbuffer) free(enbuffer);
	return -1;
}

json_t* ParameterBuildJSON(json_t* req_json, json_t* req_para, int json_type)
{
	json_t* reply_root = NULL, *reply_para = NULL;
	char* value_str = NULL;
	int result = CMD_RES_SUCCESS; /* otherwise return result:-100 form web server */
	
	reply_para = ParameterBuildJSONReplyPara(req_para, 1);	
	if(NULL == reply_para) {
		mobile_printf(MOBILE_ERR, "%s:  reply_para err! \n",__FUNCTION__);
		return NULL;
	}

	switch(json_type){
		case CMD_AUTH_ERR:
			value_str = "LOCAL_AUTH_FAIL";
			break;
		case CMD_PERMIT_ERR:
			value_str = "NO_RIGHT_ACCESS";
			break;
		case CMD_FUNC_BUSY:
			value_str = "BUSY_REQUEST";
			result = CMD_RES_BUSY;
			break;
		default:
			value_str = "UNRECOGNIZE_REQUEST";
			break;				
	}
	
	if(JSON_OK !=ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, value_str)){
		goto err;
	}
	//build head
	reply_root = ParameterBuildJSONReplyHead(req_json, result);
	if(NULL  == reply_root){
		goto err;
	}

	if(ParameterBuildJSONAddPara2Head(reply_root, reply_para)){
		goto err;
	}
	return reply_root;
	
err:
	if(reply_root)  json_free_value(&reply_root);
	if(reply_para)  json_free_value(&reply_para);
	return NULL;
}

int ParameterCheckPassWD(json_t *req_para){
	char *temp_user = NULL, *temp_pwd = NULL;
	char buffer[40] = {'\0'};

	temp_user = ParameterSearchValue(req_para, "USER");
	temp_pwd = ParameterSearchValue(req_para, "PASSWORD");
	if(temp_user == NULL || temp_pwd == NULL ){
		mobile_printf(MOBILE_ERR, "%s, get user or passwd err !\n", __FUNCTION__);
		return -1;
	}

	mobile_printf(MOBILE_INFO, "%s, get user or passwd=%s/%s !\n", __FUNCTION__, temp_user, temp_pwd);
	if(tcapi_get(ADMINENTRY,ACCOUNTUSER,buffer)  || strcmp(temp_user, buffer)){
		return -1;
	}

	if(tcapi_get(ADMINENTRY,ACCOUNTPASS,buffer)  || strcmp(temp_pwd, buffer)){
		return -1;
	}

	return 0;
}


int ParameterInit(void)
{
	int ret = 0;
	phone_para_ptr p_para = NULL;

	for(p_para = gPhone_Para; p_para->cmdtype != NULL; p_para++){
		ret = pthread_mutex_init(&p_para->mutex_lock,NULL);
		if(ret != 0)
		{
			mobile_printf(MOBILE_ERR, "\r\nMutex %s initialization fail!", p_para->cmdtype);
			return -1;
		}
	}
	
	InterfaceRightinit();
	return 0;
}

json_t* ParameterPreHandle(json_t *json_root){
	json_t *json_new = NULL, *json_para = NULL;
	char *parameter = NULL, *buffer = NULL;
	int len = 0,ret = 0;
	
	assert (json_root != NULL);	
	
	if((json_para = ParameterSearchFirstLabel(json_root, PARA_PARAMETER)) == NULL){
		mobile_printf(MOBILE_INFO, "SearchParaTable"PARA_PARAMETER" fail\n");
		goto err;
	}
	
	buffer = json_unescape(json_para->child->text);
	mobile_printf(MOBILE_DEBUG, PARA_PARAMETER" unescape=%s!\n",buffer);
	len = strlen(buffer) + 1;
	if(NULL == (parameter = malloc(len))){
		mobile_printf(MOBILE_ERR, "%s:buffer malloc err!\n",__FUNCTION__);
		goto err;
	}
	ret = base64_decode(parameter,&len,buffer,len-1);
	parameter[len] = '\0';
	
	mobile_printf(MOBILE_DEBUG, PARA_PARAMETER" decode ret[%d],len[%d]=%s!\n",ret,len,parameter);
	if(JSON_OK != json_parse_document(&json_new, parameter)){
		mobile_printf(MOBILE_ERR, "%s: create json pkt err!...\n", __FUNCTION__);
		goto err;
	}
#if 0	
	json_free_value(&json_para->child);
	
	if(JSON_OK != ParameterInsertJSONChild(json_para,json_new)){
		goto err;		
	}
#endif	
	free(buffer);
	free(parameter);
#if 1	
	json_tree_to_string(json_new,&buffer);
	mobile_printf(MOBILE_INFO, "%s: new json pkt=%s!...\n", __FUNCTION__,buffer);
	free(buffer);	
#endif	
	return json_new;	
	
err:
	if(buffer) free(buffer);
	if(parameter) free(parameter);
	if(json_new) json_free_value(&json_new);
	return NULL;
}

#if defined(TCSUPPORT_CT_JOYME)
int GetRight1(char *plgName,char *listName,char *typeName,char *nodeName){
	char fileName[128] = {0};
	FILE *fpr=NULL;
	json_t *parar = NULL;
	json_t *listnodes = NULL;
	json_t *cursor = NULL;
	json_t *curnode = NULL, *curnodeval = NULL;
	json_t *curnode2 = NULL, *curnodeval2 = NULL;
	int ret=-1;

	sprintf(fileName, "%s/%s_right1", MOBILE_RIGHT1PATH, plgName);
	fpr=fopen(fileName, "r");
	if(fpr!=NULL){
		if(json_stream_parse(fpr,&parar)!=JSON_OK){
			mobile_printf(MOBILE_INFO,"%s:json_stream_parse:%s fail\n",__FUNCTION__,fileName);
			goto err;		
		}
		else{
			mobile_printf(MOBILE_INFO,"%s:json_stream_parse:%s success\n",__FUNCTION__,fileName);
			
			/* find array node */
			listnodes = ParameterSearchFirstLabel(parar, listName);
			if ( !listnodes || !listnodes->child
				|| !listnodes->child->child )
			{
				goto err;
			}			

			/*
			listnodes->child means:  "APIRightList":[				type->JSON_ARRAY
			listnodes->child->child means:	"APIRightList":[{		type->JSON_OBJECT
			*/
			for (cursor = listnodes->child->child;
				cursor != NULL; cursor = cursor->next)
			{
				/*
				cursor->child means:				"APIRightList":[{ "API"
				cursor->child->child  means:		"APIRightList":[{ "API":"Val"
				cursor->child->next means:			"APIRightList":[{ "API":"Val", "Right"
				cursor->child->next->child means:	"APIRightList":[{ "API":"Val", "Right":"Val"
				*/
				if ( cursor->child
					&& JSON_STRING == cursor->child->type
					&& cursor->child->child
					&& JSON_STRING == cursor->child->child->type
					&& cursor->child->next
					&& JSON_STRING == cursor->child->next->type
					&& cursor->child->next->child
					&& JSON_STRING == cursor->child->next->child->type)
				{
					curnode = cursor->child;
					curnodeval = cursor->child->child;
					curnode2 = curnode->next;
					curnodeval2 = curnode->next->child;
					mobile_printf(MOBILE_INFO,"%s:%s = %s, Right = %s, SearchName=%s\n", __FUNCTION__, typeName, curnodeval->text, curnodeval2->text, nodeName);
					if ( 0 == strcmp (curnode->text, typeName)
						&& curnodeval->text
						&& 0 == strcmp (curnode2->text, "Right")
						&& curnodeval2->text)
					{
						if(0 == strcmp (curnodeval->text, nodeName)){/* find */
							if(strcmp(curnodeval2->text,"0")==0 || strcmp(curnodeval2->text,"1")==0
								|| strcmp(curnodeval2->text,"2")==0 || strcmp(curnodeval2->text,"3")==0)
								ret=atoi(curnodeval2->text);
							break;
						}
					}
				}
			}
		}		
	}
	else{/* open file fail */
		mobile_printf(MOBILE_INFO, "%s:Plugin[%s] open file[%s] fail!\n", __FUNCTION__, plgName, fileName);
		if(strcmp(listName,"APIRightList")==0)
			ret = 2;/* if not configed,local/remote both allowed */
	}

	if(fpr)
		fclose(fpr);
	if(parar)
		ParameterFreeJSON(&parar);

	return ret;
	
err:
	if(fpr)
		fclose(fpr);
	if(parar)
		ParameterFreeJSON(&parar);
	return -1;
}
#endif

json_t* ParameterHandle
(json_t *json_root, int request_from, PluginPkt_t *plugpkt, int *isLargeBuf)
{
	phone_para_ptr p_para = NULL;
	char abilitybuf[10] = {0};
	char *cmd_type = NULL;
	int retcode = CMD_RT_ERR;	
	json_t *json_reply = NULL, *req_para = NULL;
	int i = 0;
#if defined(TCSUPPORT_CT_JOYME)
	char *pPlugin_ID = NULL;
	int rightValue = 0;
#endif

	plugpkt->arg = NULL;
	plugpkt->exec_func = NULL;
	plugpkt->packet = NULL;

	req_para = ParameterPreHandle(json_root);
	if(NULL == req_para){
		mobile_printf(MOBILE_DEBUG, "%s:Parameter prehandle err!...\n", __FUNCTION__);
		return NULL;
	}	
	
	/*Search command type*/
	if((cmd_type = ParameterSearchFirstValue(req_para, PARA_CMD_TYE)) == NULL){
		mobile_printf(MOBILE_DEBUG, "%s: get "PARA_CMD_TYE" err!...\n", __FUNCTION__);
		return NULL;
	}
	
	if(!(p_para = SearchParaTable(cmd_type,&i))){
		mobile_printf(MOBILE_INFO, "SearchParaTable fail\n");
		retcode = CMD_UNRECOGNIZE;
		goto handle_end;		
	}

	if(request_from == LOCAL_REQ ){
		if(p_para->need_auth && ParameterCheckPassWD(req_para)){		
			mobile_printf(MOBILE_DEBUG, "ParameterExecute:Auth fail\n");
			retcode = CMD_AUTH_ERR;
			goto handle_end;	
		}
		abilitybuf[0] = '\0';
		if(!tcapi_get(MOBILE_ENTRY, LOCALABILITY, abilitybuf) && abilitybuf[0] == '1'){
			retcode = CMD_PERMIT_ERR;
			goto handle_end;
		}
	}
		
	if(((request_from == LOCAL_REQ) && !(p_para->permit & PERMIT_LOCAL))
		|| ((request_from == REMOTE_REQ) && !(p_para->permit & PERMIT_REMOTE))){
		mobile_printf(MOBILE_ERR, "%s:Check Permit fail:permit=%d!\n", __FUNCTION__, p_para->permit);
		retcode = CMD_PERMIT_ERR;
		goto handle_end;
	}

#if defined(TCSUPPORT_CT_JOYME)
	/* judge access */
	pPlugin_ID = ParameterSearchFirstValue(json_root, "Plugin_Name");
	if(pPlugin_ID==NULL){
		mobile_printf(MOBILE_ERR, "%s:Get Plugin_Name failed!\n", __FUNCTION__);
		retcode = CMD_PERMIT_ERR;
		goto handle_end;
	}
	rightValue=GetRight1(pPlugin_ID,"APIRightList","API",cmd_type);
	if(rightValue<0){
		mobile_printf(MOBILE_INFO, "%s:Get Right failed!\n", __FUNCTION__);
		retcode = CMD_PERMIT_ERR;
		goto handle_end;
	}	
	rightValue=(rightValue+1)%4;
	if(((request_from == LOCAL_REQ) && !(rightValue & PERMIT_LOCAL))
		|| ((request_from == REMOTE_REQ) && !(rightValue & PERMIT_REMOTE))){
		mobile_printf(MOBILE_INFO, "%s:Plugin[%s]API[%s]Check Permit fail:permit=%d!\n", __FUNCTION__, pPlugin_ID, cmd_type, rightValue);
		retcode = CMD_PERMIT_ERR;
		goto handle_end;
	}
#endif

	/*Execute Parameter Function*/
	if(pthread_mutex_trylock(&p_para->mutex_lock)){
		mobile_printf(MOBILE_DEBUG, "%s: %s is busy...!\n", __FUNCTION__, cmd_type);
		retcode = CMD_FUNC_BUSY;
		goto handle_end;
	}
	else{		
		if(p_para->need_delay){			
			json_reply = ((cmdfuncNeedDelay)(p_para->cmdfunc))(json_root, req_para, &plugpkt->arg);					
			if(NULL == json_reply) {
				plugpkt->arg = NULL;
				mobile_printf(MOBILE_INFO, "NULL == json_reply, plugpkt->arg = NULL\n");
			}
		}else{
			json_reply = ((cmdfuncNoDelay)(p_para->cmdfunc))(json_root, req_para);		
		}

		pthread_mutex_unlock(&p_para->mutex_lock);
	}

#if 0
	char *buffer = NULL;
	json_tree_to_string(json_reply,&buffer);
	mobile_printf(MOBILE_ERR, "%s: reply pkt=%s!...\n", __FUNCTION__,buffer);
	free(buffer);	
#endif	

	/*Run Delay Func*/
	if(p_para && p_para->need_delay){
		plugpkt->exec_func = p_para->cmdfuncdelay;
	}

handle_end:
	if(NULL == json_reply) 
		json_reply = ParameterBuildJSON(json_root, req_para, retcode);	

#if defined(TCSUPPORT_CT_JOYME)
	if ( json_reply
		&& 0 == strcmp(cmd_type, "GETPARAMETERVALUES") )
		*isLargeBuf = 1;
#endif

	if(json_root)
		ParameterFreeJSON(&json_root);
	if(req_para)
		ParameterFreeJSON(&req_para);
		
	return json_reply;
}

#if defined(TCSUPPORT_CT_JOYME)
json_t* ParameterBuildJSONReplyParaString
(json_t* req_para, char* reply_status)
{
	json_t *reply_para = NULL;

	/* creates the root node */
	if(!(reply_para = ParameterNewJSONObject())){
		mobile_printf(MOBILE_ERR, "ParameterNewJSONObject:err!\n");
		return NULL;
	}

	if(ParameterAddLabelByRequest(req_para,reply_para,PARA_CMD_TYE)){
		goto err;
	}
	if(ParameterAddLabelByRequest(req_para,reply_para,PARA_SEQUENCE_ID)){
		goto err;
	}

	if(JSON_OK != ParameterAddNewJSONChild(
		reply_para, PARA_STATUS, reply_status)){
		goto err;
	}

	return reply_para;

err:
	if(reply_para){
		ParameterFreeJSON(&reply_para);
	}
	return NULL;
}
#endif

