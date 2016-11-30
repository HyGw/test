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

unsigned long round(double number)
{
    unsigned long   ret_num = 0;
    if (number - (unsigned long) number < 0.5)
        ret_num = (unsigned long) number;
    else
        ret_num = (unsigned long) number + 1;
    return ret_num;
}

float logFunction(float x, int base, int decimalplaces)
{
	int integer = 0;
	float return_value = 0;
	float decimalplaces_count = 0.1;

	if (x <= 0.0)
		return -40;  //assume it is x = 1 when x is 0.
	
	while(x < 1)
	{
		integer = integer -1;
		x = x*base;
	}
	while(x >= base)
	{
		integer = integer + 1;
		x = x/base;
	}
	return_value = integer;
	while(decimalplaces > 0)
	{
		x = x*x*x*x*x*x*x*x*x*x;
		int digit = 0;
		while (x >= base)
		{
			digit = digit + 1;
			x = x/base;
		}

		return_value = return_value + (digit * decimalplaces_count);
		decimalplaces = decimalplaces - 1;
		decimalplaces_count = decimalplaces_count * 0.1;
	}

	return return_value;

}

json_t* Get_PonStatus(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	char ctempTemperature[16] = {0}, ctempVottage[16] = {0}, ctempCurrent[16] = {0}, ctempTXPower[16] = {0}, ctempRXPower[16] = {0};
	char Temperature[16] = {0}, Vottage[16] = {0}, Current[16] = {0}, TXPower[16] = {0}, RXPower[16] = {0}, cLinkSta[4] = {0};
	int tempTemperature = 0, tempVottage= 0, tempCurrent = 0, tempTXPower = 0, tempRXPower = 0;
	float fTXPower = 0.0, fRXPower = 0.0, fVottage = 0.0, fCurrent = 0.0;
	int ret1 = -1, ret2 = -1, ret3 = -1, ret4 = -1, ret5 = -1;
	
	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__,__LINE__);

	tcapi_get("XPON_LinkCfg", "LinkSta",cLinkSta);
	ret1 = tcapi_get("Info_PonPhy", "Temperature",ctempTemperature);
	ret2 = tcapi_get("Info_PonPhy", "TxPower", ctempTXPower);
	ret3 = tcapi_get("Info_PonPhy", "RxPower", ctempRXPower);
	ret4 = tcapi_get("Info_PonPhy", "SupplyVoltage", ctempVottage);
	ret5 = tcapi_get("Info_PonPhy", "TxBiasCurrent", ctempCurrent);

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
		sprintf(TXPower,"%f",fTXPower);
		sprintf(RXPower,"%f",fRXPower);
	}
	else
	{
		strcpy(TXPower, "");
		strcpy(RXPower, "");
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

	sprintf(Vottage,"%.4f",fVottage);
	sprintf(Current,"%.2f",fCurrent);
	sprintf(Temperature,"%d",tempTemperature);

	//if((ret1 != -1) && (ret2 != -1) && (ret3 != -1) && (ret4 != -1) &&(ret5 != -1))
	if(1)
	{	
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){//ok
			mobile_printf(MOBILE_ERR,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_TEMPERATURE, Temperature)){
			mobile_printf(MOBILE_ERR,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_VOTTAGE, Vottage)){
			mobile_printf(MOBILE_ERR,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_CURRENT, Current)){
			mobile_printf(MOBILE_ERR,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_TXPOWER, TXPower)){
			mobile_printf(MOBILE_ERR,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_RXPOWER, RXPower)){
			mobile_printf(MOBILE_ERR,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	else
	{
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "DEVICE_NOT_SUPPORT_REQ")){
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
