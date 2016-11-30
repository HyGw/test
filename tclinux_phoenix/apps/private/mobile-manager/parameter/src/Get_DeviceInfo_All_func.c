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
#include "Get_PonStatus_func.h"
#include "Get_CpuUsage_func.h"
#include "Get_MemUsage_func.h"
#include "Get_PPP_DialStatus_func.h"
#include "Get_Duration_func.h"
#include "Get_DeviceInfo_All_func.h"
#include "libcompileoption.h"

#define VOICE_IAD_ERROR      1
#define VOICE_ROUTE_ERROR    2
#define VOICE_RESPONSE_ERROR 3
#define VOICE_USER_PW_ERROR  4
#define VOICE_UNKNOWN_ERROR  5

json_t* GetDeviceInfoAll(json_t* req_root, json_t* req_para)
{
	json_t  *reply_root = NULL, *reply_para = NULL;
	char *temp_user = NULL, *temp_pwd = NULL;
	DeviceInfo_All_t * devinfo = NULL;

	mobile_printf(MOBILE_INFO, "%s, line = %d, start !\n",__FUNCTION__, __LINE__);

	devinfo = (DeviceInfo_All_t *)malloc(sizeof(DeviceInfo_All_t));
	if(devinfo == NULL){
		mobile_printf(MOBILE_ERR, "%s, malloc err !\n", __FUNCTION__);
		return NULL;
	}
	memset(devinfo, 0, sizeof(DeviceInfo_All_t));

	DeviceInfo(devinfo);
	
	//build parameter
	if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
		goto err;
	}	
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_WVERSION, devinfo->SwVersion)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_HDVERSION, devinfo->hwVersion)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_PRODUCTCLASS, devinfo->ProductClass)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_CPUCLASS, devinfo->cpuclass)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_DEVTYPE, devinfo->ModelName)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_DEVTYPE1, devinfo->devtype1)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_CAPABILITY, devinfo->devcapval)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_CARD, devinfo->iscardsepa)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_UPLINK, devinfo->uplinkval)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_CARDNO, devinfo->cardno)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_PONREGSTATUS, devinfo->tempPonRegStatus)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_WANINFO, devinfo->AllWanStaInfo)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_TXPOWER, devinfo->ctempTXPower)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_RXPOWER, devinfo->ctempRXPower)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_SYSDURATION, devinfo->passedsystime)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_PPPOEDURATION, devinfo->pppdurtime)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_PONDURATION, devinfo->ponduration)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_VOTTAGE, devinfo->ctempVottage)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_CURRENT, devinfo->ctempCurrent)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_LINE, devinfo->tempPonStatus)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_REGSTATUS, devinfo->regstainfo)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_BUSSSTATUS, devinfo->bussinesssta)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_IP, devinfo->str_tr069Ip)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_INTERDNS1, devinfo->str_dns)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_INTERDNS2, devinfo->str_secdns)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_CPUPERCENT, devinfo->CpuUsagevalue)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_STOREPERCENT, devinfo->MemUsagevalue)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_INFO, devinfo->LanNetInfo)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddJSONChild(reply_para, PARA_WIFIEN, TCSUPPORT_WLAN_VAL ? JSON_STRING : JSON_NULL, devinfo->WLanEnable)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddJSONChild(reply_para, PARA_WLANINFORM, TCSUPPORT_WLAN_VAL ? JSON_STRING : JSON_NULL, devinfo->wlaninfom)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_VOIPNAME1, devinfo->VoipName1)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddJSONChild(reply_para, PARA_VOIPNAME2, TCSUPPORT_CT_1FXS_VAL ? JSON_NULL : JSON_STRING, devinfo->VoipName2)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_VOIP1REGSTATUS, devinfo->VoIPRegInfo1)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddJSONChild(reply_para, PARA_VOIP2REGSTATUS, TCSUPPORT_CT_1FXS_VAL ? JSON_NULL : JSON_STRING, devinfo->VoIPRegInfo2)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_CONNESTATUS, devinfo->IPv4ConStatus)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_WANSTATUS, devinfo->IPv4Status)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_DIALREASON, devinfo->IPv4ConError)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_CONNESTATUS1, devinfo->IPv6ConStatus)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_WANSTATUS1, devinfo->IPv6Status)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_DIALREASON1, devinfo->IPv6ConError)){
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_CARDSTATUS, devinfo->cardsta)){
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
	mobile_printf(MOBILE_INFO, "%s: reply json pkt=%s!...\n", __FUNCTION__,value_str);
	free(value_str);	
#endif	

	free(devinfo);
	return reply_root;

err:
	if(reply_root) ParameterFreeJSON(&reply_root);
	if(reply_para) ParameterFreeJSON(&reply_para);
	free(devinfo);
	return NULL;

}

int DeviceInfo(DeviceInfo_All_t *devinfo)
{
	getDeviceBasicInfo(devinfo->SwVersion, devinfo->hwVersion, devinfo->ProductClass, devinfo->ModelName, devinfo->devtype1, devinfo->cpuclass);
	getDevCapabilityValue(devinfo->devcapval);
	GetSimCardInfo(devinfo->iscardsepa, devinfo->cardno, devinfo->cardsta);
	getUPLinkValue(devinfo->uplinkval);
	GetPonRegInfo(devinfo->tempPonRegStatus, devinfo->tempPonStatus);
	GetVoIPRegInfo(devinfo->VoIPRegInfo1, devinfo->VoIPRegInfo2);
	getWanStaInfo(devinfo->AllWanStaInfo, devinfo->str_tr069Ip, devinfo->str_dns, devinfo->str_secdns); 

	GetPonStatus(devinfo->ctempTemperature, devinfo->ctempVottage, devinfo->ctempCurrent, devinfo->ctempTXPower, devinfo->ctempRXPower);
	GetAssignedSSID1Info(devinfo->WLanEnable, devinfo->ssidEnable, devinfo->cur_ssid_name);
	sprintf(devinfo->wlaninfom, "%s/%s", devinfo->ssidEnable, devinfo->cur_ssid_name);
	get_sysup_duration(devinfo->passedsystime);
	get_ppp_duration(devinfo->pppdurtime);
	get_PON_duration(devinfo->ponduration);
	GetRegStatusInfo(devinfo->regstainfo);
	GetBussinessSta(devinfo->bussinesssta);
	CalcuCpuUsage(devinfo->CpuUsagevalue);
	memory_read(devinfo->MemUsagevalue);
	getLanEntryInfo(devinfo->LanNetInfo);
	GetVoIPInfo(devinfo->VoipName1, devinfo->VoipName2);
	GetPPPDialStatus(devinfo->IPv4ConStatus,devinfo->IPv4Status,devinfo->IPv4ConError,devinfo->IPv6ConStatus,devinfo->IPv6Status,devinfo->IPv6ConError);

	return 0;
}

int GetPonStatus(char *cTemperature, char *cVottage, char *cCurrent, char *cTXPower, char *cRXPower)
{
	char ctempTemperature[16] = {0}, ctempVottage[16] = {0}, ctempCurrent[16] = {0}, ctempTXPower[16] = {0}, ctempRXPower[16] = {0}, cLinkSta[4] = {0};
	int tempTemperature = 0, tempVottage= 0, tempCurrent = 0, tempTXPower = 0, tempRXPower = 0;
	float fTXPower = 0.0, fRXPower = 0.0, fVottage = 0.0, fCurrent = 0.0;

	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__,__LINE__);
	
	tcapi_get("XPON_LinkCfg", "LinkSta",cLinkSta);
	tcapi_get("Info_PonPhy", "Temperature",ctempTemperature);
	tcapi_get("Info_PonPhy", "TxPower", ctempTXPower);
	tcapi_get("Info_PonPhy", "RxPower", ctempRXPower);
	tcapi_get("Info_PonPhy", "SupplyVoltage", ctempVottage);
	tcapi_get("Info_PonPhy", "TxBiasCurrent", ctempCurrent);
	
	tempTXPower = atoi(ctempTXPower);
	tempRXPower = atoi(ctempRXPower);
	tempVottage = atoi(ctempVottage);
	tempCurrent = atoi(ctempCurrent);

	if(strcmp(cLinkSta, "0") != 0)
	{
		fTXPower = tempTXPower * 1.0 / 10000;
		fRXPower = tempRXPower * 1.0 / 10000;
		fTXPower = (logFunction(fTXPower, 10, 6) * 100) / 10;
		fRXPower = (logFunction(fRXPower, 10, 6) * 100) / 10;
		sprintf(cTXPower,"%f",fTXPower);
		sprintf(cRXPower,"%f",fRXPower);
	}
	else
	{
		strcpy(cTXPower, "");
		strcpy(cRXPower, "");
	}

	tempVottage	= tempVottage / 10;
	fCurrent	= tempCurrent * 2.0 / 1000;
	tempTemperature = atoi(ctempTemperature);
	if (tempTemperature >= pow(2, 15))
	{
		tempTemperature = -round((pow(2, 16)-tempTemperature)/256);
	}
	else
	{
		tempTemperature = round(tempTemperature/256);
	}

	//Diaplay 'V' unit
	fVottage = tempVottage * 1.0 / 1000;

	sprintf(cTemperature,"%d",tempTemperature);	
	sprintf(cVottage,"%.4f",fVottage);
	sprintf(cCurrent,"%.2f",fCurrent);	

	return 0;
}

void GetPonRegInfo(char *PonRegStatus, char *PonStatus)
{
	char tempphyStatus[20] = {0};
	char temptrafficStatus[20] = {0};

	tcapi_get("XPON_Common", "phyStatus", tempphyStatus);
	tcapi_get("XPON_Common", "trafficStatus", temptrafficStatus);	

	if(strstr(tempphyStatus, "up") == NULL)
	{
		strcpy(PonRegStatus, "PON_STATUS_NO_REG_NO_AUTH");
	}
	else
	{
		if(strstr(temptrafficStatus, "up") == NULL)
		{
			strcpy(PonRegStatus, "PON_STATUS_REG_NO_AUTH");
		}
		else
		{
			strcpy(PonRegStatus, "PON_STATUS_REG_AUTH");
		}
	}

	if(strstr(temptrafficStatus, "up") == NULL)
	{
		strcpy(PonStatus, "0");
	}
	else
	{
		strcpy(PonStatus, "1");
	}

}

void GetVoIPRegInfo(char *VoIPRegInfo1, char *VoIPRegInfo2)
{
	char tempVoIPRegInfo1[8] = {0}, tempVoIPRegInfo2[8] = {0};
	char tempvoiceIfName[128] = {0};
	FILE *fp=NULL;
	char tempValue[16] = {0};
	unsigned int u32Temp = 0;
	uint8 u8TempVoIP1 = 0, u8TempVoIP2 = 0;
	char sipProtocol[16] = {0};
	int isH248 = 0;

	memset(VoIPRegInfo1,0,sizeof(VoIPRegInfo1));
	memset(VoIPRegInfo2,0,sizeof(VoIPRegInfo2));

	if ( 0 == tcapi_get("VoIPBasic_Common", "SIPProtocol", sipProtocol)
		&& 0 == strcmp("H.248", sipProtocol) )
		isH248 = 1;

	fp = fopen("/proc/tc3162/pcmlastregerror", "r");
	if(fp != NULL){
	fgets(tempValue, 16, fp);
	fclose(fp);
	}
	tcapi_get("VoIPSysParam_Common", "SC_SYS_CFG_WAN_IFNAME", tempvoiceIfName);

	/*
	tempU32: bit31~bit24  |  bit23~bit16  |  bit15~bit8  | bit7~bit0
				reserved         reserved        Line2            Line1
	*/		
	sscanf(tempValue,"%x",&u32Temp);

	if ( isH248 )
	{
		tcapi_get("VoIPH248_Entry0", "UserServiceState", tempVoIPRegInfo1);
		tcapi_get("VoIPH248_Entry1", "UserServiceState", tempVoIPRegInfo2);
	}
	else
	{
		tcapi_get("InfoVoIP_Entry0", "Status", tempVoIPRegInfo1);
		tcapi_get("InfoVoIP_Entry1", "Status", tempVoIPRegInfo2);
	}

	if ( isH248 )
	{
		switch ( atoi(tempVoIPRegInfo1) )
		{
			case 1:
				strcpy(VoIPRegInfo1, "VOIP_STATUS_SUCCESS");
				break;
			case 5: /* register failed*/
			case 3: /* registering */
			case 8: /* disabled */
			default:
				strcpy(VoIPRegInfo1, "VOIP_STATUS_UNKNOWN");
				break;
		}

		switch ( atoi(tempVoIPRegInfo2) )
		{
			case 1:
				strcpy(VoIPRegInfo2, "VOIP_STATUS_SUCCESS");
				break;
			case 5: /* register failed*/
			case 3: /* registering */
			case 8: /* disabled */
			default:
				strcpy(VoIPRegInfo2, "VOIP_STATUS_UNKNOWN");
				break;
		}
	}
	else
	{
		if(strcmp(tempVoIPRegInfo1, "Up") == 0){
			strcpy(VoIPRegInfo1,"VOIP_STATUS_SUCCESS");
		}
		else{
			if((strcmp(tempvoiceIfName ,"") == 0)||(strcmp(tempvoiceIfName ,"br0") == 0)){
				strcpy(VoIPRegInfo1,"VOIP_NOVALID_CONNECTION");				
				}
			else{				
			u8TempVoIP1 = u32Temp & 0xff;

			switch(u8TempVoIP1)
			{
				case VOICE_IAD_ERROR:
					strcpy(VoIPRegInfo1,"VOIP_STATUS_IADERROR");
					break;
				case VOICE_ROUTE_ERROR:
					strcpy(VoIPRegInfo1,"VOIP_STATUS_NOROUTE");
					break;
				case VOICE_RESPONSE_ERROR:
					strcpy(VoIPRegInfo1,"VOIP_STATUS_NORESPONSE");
					break;
				case VOICE_USER_PW_ERROR:
					strcpy(VoIPRegInfo1,"VOIP_STATUS_ACCOUNTERR");
					break;
				case VOICE_UNKNOWN_ERROR:
					strcpy(VoIPRegInfo1,"VOIP_STATUS_UNKNOWN");
					break;
				default:
					strcpy(VoIPRegInfo1,"VOIP_NOVALID_CONNECTION");
					break;
			}
		}
		}

		if(strcmp(tempVoIPRegInfo2, "Up") == 0){
			strcpy(VoIPRegInfo2,"VOIP_STATUS_SUCCESS");
		}
		else{
			if((strcmp(tempvoiceIfName ,"") == 0)||(strcmp(tempvoiceIfName ,"br0") == 0)){
				strcpy(VoIPRegInfo2,"VOIP_NOVALID_CONNECTION");
				}
			else{
			u8TempVoIP2 = (u32Temp >> 8) & 0xff;

			switch(u8TempVoIP2)
			{
				case VOICE_IAD_ERROR:
					strcpy(VoIPRegInfo2,"VOIP_STATUS_IADERROR");
					break;
				case VOICE_ROUTE_ERROR:
					strcpy(VoIPRegInfo2,"VOIP_STATUS_NOROUTE");
					break;
				case VOICE_RESPONSE_ERROR:
					strcpy(VoIPRegInfo2,"VOIP_STATUS_NORESPONSE");
					break;
				case VOICE_USER_PW_ERROR:
					strcpy(VoIPRegInfo2,"VOIP_STATUS_ACCOUNTERR");
					break;
				case VOICE_UNKNOWN_ERROR:
					strcpy(VoIPRegInfo2,"VOIP_STATUS_UNKNOWN");
					break;
				default:
					strcpy(VoIPRegInfo2,"VOIP_NOVALID_CONNECTION");
					break;
			}
		}
	}
}
}

void GetVoIPInfo(char *voipname1, char *voipname2)
{
	char SIPProtocol[16] = {0};

	tcapi_get("VoIPBasic_Common", "SIPProtocol", SIPProtocol);
	if(strcmp(SIPProtocol, "H248") == 0){
		tcapi_get("VoIPH248_Entry0", "PhysicalTermID", voipname1);
		tcapi_get("VoIPH248_Entry1", "PhysicalTermID", voipname2);
	}
	else{
		tcapi_get("VoIPBasic_Entry0", "SIPDisplayName", voipname1);
		tcapi_get("VoIPBasic_Entry1", "SIPDisplayName", voipname2);
	}	
}

void GetAssignedSSID1Info(char *WLanEnable, char *ssidEnable, char *ssid_name)
{	
	tcapi_get("WLan_Common", "APOn", WLanEnable);
	tcapi_get("WLan_Entry0", "SSID", ssid_name);
	tcapi_get("WLan_Entry0", "EnableSSID", ssidEnable);	
}

void getDevCapabilityValue(char *devcapval)
{
	if(TCSUPPORT_VOIP_VAL){
		if(TCSUPPORT_CT_1FXS_VAL)
			sprintf(devcapval, "%d;", 1);
		else
			sprintf(devcapval, "%d;", 2);
	}
	else
		sprintf(devcapval, "%d;", 0);

	if(TCSUPPORT_2PORTS_VAL)
		strcat(devcapval, "2FE;");
	else{
		if(TCSUPPORT_CT_2PORTS_VAL){
			if(TCSUPPORT_CT_PON_CY_VAL || TCSUPPORT_CT_PON_C7_VAL)
					strcat(devcapval, "1FE+1GE;");
			else
					strcat(devcapval, "2FE;");
		}
		else{
			if(TCSUPPORT_CT_PON_CY_VAL)
				strcat(devcapval, "3FE+1GE;");
			else
				strcat(devcapval, "4FE;");
		}
	}	

	if(TCSUPPORT_USBHOST_VAL)
		strcat(devcapval, "1;");
	else
		strcat(devcapval, "0;");
	if(TCSUPPORT_WLAN_VAL)
		strcat(devcapval, "3;0");
	else
		strcat(devcapval, "0;0");
}

void getUPLinkValue(char *uplinkval)
{
	char tempValue[10] = {0};
	
	tcapi_get("XPON_LinkCfg", "LinkSta", tempValue);
	if(strcmp(tempValue, "1") == 0)
		sprintf(uplinkval, "%d", 5);
	else
		sprintf(uplinkval, "%d", 4);
}

void getDeviceBasicInfo(char *swVer,char *hwVer,char *PClass,char *MName,char *devtype1,char *cpuclass)
{
	char cwmpOUI[32] = {0};
	char SerialNum[129] = {0};
	
	tcapi_get( "DeviceInfo_devParaStatic", "CustomerSWVersion", swVer );
	tcapi_get( "DeviceInfo_devParaStatic", "CustomerHWVersion", hwVer );
	tcapi_get( "DeviceInfo_devParaDynamic", "ProductClass", PClass );
	tcapi_get("DeviceInfo_devParaDynamic","ManufacturerOUI",cwmpOUI);
	tcapi_get("DeviceInfo_devParaDynamic","SerialNum",SerialNum);
	sprintf(MName, "%s-%s", cwmpOUI, SerialNum);

	if(TCSUPPORT_CT_STANDARD_E8C_VAL){
		if(TCSUPPORT_WLAN_VAL)
			sprintf(devtype1, "%d", 3);
		else
			sprintf(devtype1, "%d", 4);
	}
	else
		sprintf(devtype1, "%d", 0);

	sprintf(cpuclass, "%s", "ECN7525F");
		
}

void getWanStaInfo(char *AllWanStaInfo, char *tr069ip, char *intdns1, char *intdns2)
{
	char WanName[64] = {0}, cWanConnectNum[8] = {0}, nodeName[48] = {0};
	char *cur_IFIndex = NULL, *cValidIFIndex = NULL, ValidIFIndex[64] = {0}, IFIndex[MAX_PVC_NUM][MAX_SMUX_NUM] = {0};
	char str_ifaceID[8] = {0}, str_servicelist[64] = {0}, str_vid[6] = {0}, str_mode[16] = {0}, str_wanmode[16] = {0}, str_status[8] = {0};
	unsigned int uiWanConnectNum = 0, pvc_index = 0, entry_idx = 0, cur_id = 0;
	int ret = 0, pvc_num = 0, i = 0, j = 0;

	tcapi_get("WanInfo_Common", "CycleNum", cWanConnectNum);
	tcapi_get("WanInfo_Common", "ValidIFIndex", ValidIFIndex);
	
	cValidIFIndex = ValidIFIndex;
	cur_IFIndex = ValidIFIndex;

	uiWanConnectNum = atoi(cWanConnectNum);

	while(*cValidIFIndex!='\0')
	{
		if(*cValidIFIndex == ','){
			strncpy(IFIndex[pvc_num], cur_IFIndex,j);
			j = 0;
			pvc_num++;
			cValidIFIndex++;
			cur_IFIndex = cValidIFIndex;
		}
		else{
			j++;
			cValidIFIndex++;
		}
	}

	if(0 <= uiWanConnectNum)
	{	
		memset(nodeName,0,sizeof(nodeName));

		for(i = 0; i < pvc_num; i++){

			cur_id = atoi(IFIndex[i]);		
			pvc_index = cur_id / MAX_PVC_NUM;
			entry_idx = cur_id % MAX_PVC_NUM;

			sprintf(nodeName, "Wan_PVC%d_Entry%d", pvc_index,entry_idx);

			tcapi_get(nodeName, "IFIdx", str_ifaceID);
			tcapi_get(nodeName, "ServiceList", str_servicelist);			
			tcapi_get(nodeName, "VLANID", str_vid);
			tcapi_get(nodeName, "WanMode", str_wanmode);
			if( 0 == strcmp(str_wanmode, "Route") )
				strcpy(str_mode, "R");
			else
				strcpy(str_mode, "B");
		
			sprintf(WanName, "%s_%s_%s_VID_%s", str_ifaceID, str_servicelist, str_mode, str_vid);

			sprintf(nodeName, "WanInfo_Entry%d", cur_id);
			tcapi_get(nodeName, "Status", str_status);
			if(strcmp(str_status, "up"))
				strcpy(str_status, "down");
			
			if(strcmp(str_servicelist, "TR069") == 0)
				tcapi_get(nodeName, "IP", tr069ip);
			else if(strcmp(str_servicelist, "INTERNET") == 0)
			{
				tcapi_get(nodeName, "DNS", intdns1);
				tcapi_get(nodeName, "SecDNS", intdns2);
			}
			if(i > 0)
				strcat(AllWanStaInfo,"#");
			strcat(AllWanStaInfo,WanName);
			strcat(AllWanStaInfo,"/");
			strcat(AllWanStaInfo,str_wanmode);
			strcat(AllWanStaInfo,"/");
			strcat(AllWanStaInfo,str_status);
			
		}
	}	
}

void GetSimCardInfo(char *existflag, char *num, char *cardstatus)
{
	char buf[5] = {0};
	int cardsta = 0;
	
	if(TCSUPPORT_CT_SIMCARD_SEPARATION_VAL){	
		tcapi_get("SimCard_Entry","cpeFlag",buf);
		if(strcmp(buf,"1") == 0)
		{
			memset(buf, 0, sizeof(buf) );
			tcapi_get("SimCard_Entry","cardExist",buf);		
			if(strcmp(buf,"1") == 0)
			{
				tcapi_get("SimCard_Entry","CardSN",num);	
				tcapi_get("SimCard_Entry","simAuthRet",buf);
				if(strcmp(buf,"1") != 0)
					cardsta = 4;
			}
			else
				cardsta = 1;
		}
		else
			cardsta = 1;
		sprintf(existflag, "%s", "1");
		sprintf(cardstatus, "%d", cardsta);
	}
	else{
		sprintf(existflag, "%s", "0");
		sprintf(cardstatus, "%s", "1");
	}
}

void GetBussinessSta(char *bussta)
{
	char tempStatus[16] = {0};
	char tempResult[16] = {0};
	tcapi_get( "deviceAccount_Entry", "registerStatus", tempStatus );
	tcapi_get( "deviceAccount_Entry", "registerResult", tempResult );
	if(((strcmp(tempStatus, "0")==0)&&(strcmp(tempResult, "1")==0)) || ((strcmp(tempStatus, "5")==0)&&(strcmp(tempResult, "0")==0)))
		strcpy(bussta, "SUCCESS");
	else
		strcpy(bussta, "FAIL");
}

int GetPPPDialStatus(char *IPv4ConStatus,char *IPv4Status,char *IPv4ConError,char *IPv6ConStatus,char *IPv6Status,char *IPv6ConError)
{
	unsigned long wanNodeIndexv4 = 0, pvcNodeEntryv4 = 0, wanNodeIndexv6 = 0, pvcNodeEntryv6 = 0;
	int IPv4ret = -1, IPv6ret = -1, defRouterIfv4 = -1, defRouterIfv6 = -1;

	getISPIndex(&defRouterIfv4, &defRouterIfv6);

	if(defRouterIfv4 >= 0)
	{
		wanNodeIndexv4 = defRouterIfv4 / MAX_PVC_NUM;
		pvcNodeEntryv4 = defRouterIfv4 % MAX_PVC_NUM;

		IPv4ret = getWANCnt1_IPv4InfoValue(IPv4ConStatus, IPv4Status, IPv4ConError, wanNodeIndexv4, pvcNodeEntryv4);

		if(IPv4ret == -1){
			strcpy(IPv4ConStatus, "0");
			strcpy(IPv4Status, "Unconfigured");
			strcpy(IPv4ConError, "ERROR_NO_VALID_CONNECTION");
		}
	}
	else{
		strcpy(IPv4ConStatus, "0");
		strcpy(IPv4Status, "Unconfigured");
		strcpy(IPv4ConError, "ERROR_NO_VALID_CONNECTION");
	}
	
	if(defRouterIfv6 >= 0)
	{
		wanNodeIndexv6 = defRouterIfv6 / MAX_PVC_NUM;
		pvcNodeEntryv6 = defRouterIfv6 % MAX_PVC_NUM;

		IPv6ret = getWANCnt1_IPv6InfoValue(IPv6ConStatus, IPv6Status, IPv6ConError, wanNodeIndexv6, pvcNodeEntryv6);

		if(IPv6ret == -1){
			strcpy(IPv6ConStatus, "0");
			strcpy(IPv6Status, "Unconfigured");
			strcpy(IPv6ConError, "ERROR_NO_VALID_CONNECTION");
		}
	}
	else{
		strcpy(IPv6ConStatus, "0");
		strcpy(IPv6Status, "Unconfigured");
		strcpy(IPv6ConError, "ERROR_NO_VALID_CONNECTION");
	}
		
			return 0;
	}	

void GetRegStatusInfo(char * regstavalue)
{
	char tempStatus[16] = {0};

	tcapi_get( "deviceAccount_Entry", "registerStatus", tempStatus );
	
	if(strcmp(tempStatus, "99") == 0)
		strcpy(regstavalue, "ITMS_REG_DEFAULT");
	else if((strcmp(tempStatus, "0") == 0) || (strcmp(tempStatus, "5") == 0))
	{
		strcpy(regstavalue, "ITMS_REG_SUCCESS");
	}
	else if((strcmp(tempStatus, "1")==0)||(strcmp(tempStatus, "2")==0)||(strcmp(tempStatus, "3")==0)||(strcmp(tempStatus, "4")==0))
			strcpy(regstavalue, "ITMS_REG_FAIL");
	else
		strcpy(regstavalue, "ITMS_REG_DEFAULT");
}

