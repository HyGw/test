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
#include "Get_LanInfo_func.h"
#include <arpa/inet.h>

void getLanEntryInfo(char *buf)
{
	char nodeName[48]={0}, ethernet_str[5]={0}, temp[60]={0}, tempip[60]={0}, nodeattr[16]={0};
	char vendorID[][8] ={{""},{"HGW"},{"STB"},{"Phone"},{"Camera"}};
	char devtype[][8] ={{"PC"},{"HGW"},{"STB"},{"phone"},{"Camera"}};
	uint32 ipadd=0, ipstart=0, poolcount=0;
	char *pSrc = NULL, *pDst = NULL;
	char macstr[16] = {0};
	int ethernum = 0, i = 0, j=0;
	
	tcapi_get( "LanHost", "LeaseNum", ethernet_str );
	ethernum = atoi(ethernet_str);
	
	if(ethernum <= 0)
		return;
	else if(ethernum > LANINFOMAXENTRY)
		ethernum = LANINFOMAXENTRY;
	
	for(i=0; i<ethernum; i++)
	{
		sprintf(nodeName, "LanHost_Entry%d", i);
		
		memset(temp, 0, sizeof(temp));
		tcapi_get(nodeName, "HostName", temp);
		if(i > 0)
			strcat(buf,"/");
		if(((strlen(temp)>1)&&(strcmp(temp, "N/A"))) || ((strlen(temp)==1)&&(temp[0]!=' ')))
			strcat(buf,temp);
		else
			strcat(buf,"Computer");
		strcat(buf,":");

		memset(temp, 0, sizeof(tempip));
		tcapi_get(nodeName, "IP", tempip);
		ipadd = inet_addr(tempip);
		
		for(j=0; j<5; j++)
		{
			memset(temp, 0, sizeof(temp));
			sprintf(nodeattr, "start%s", vendorID[j]);
			tcapi_get( "Dhcpd_Option60", nodeattr, temp );
			ipstart = inet_addr(temp);
			memset(temp, 0, sizeof(temp));
			sprintf(nodeattr, "pool_count%s", vendorID[j]);
			tcapi_get( "Dhcpd_Option60", nodeattr, temp );
			poolcount = atoi(temp);	
			mobile_printf(MOBILE_DEBUG,"ipadd=%08X, ipstart=%08X, count=%d,\n", ipadd, ipstart, poolcount);
			if((ipadd>=ipstart) && (ipadd<(ipstart+poolcount)))
			{
				strcat(buf,devtype[j]);
				strcat(buf,":");	
				break;
			}
		}
		if(j >= 5)
		{
			strcat(buf,"OTHER");
			strcat(buf,":");		
		}

		memset(temp, 0, sizeof(temp));
		tcapi_get(nodeName, "MAC", temp);

		memset(macstr, 0, sizeof(macstr));
		pSrc = temp;
		pDst = macstr;
		do{
			if(*pSrc != ':'){
				*pDst++ = *pSrc;
			}
		}while(*pSrc++);
		
		strcat(buf,macstr);
		strcat(buf,":");

		strcat(buf,tempip);
		strcat(buf,":");

		memset(temp, 0, sizeof(temp));
		tcapi_get(nodeName, "InterfaceType", temp);
		if(strcmp(temp, "Ethernet") == 0)
			strcat(buf,"0");
		else
			strcat(buf,"1");						
	}
}

#if defined(TCSUPPORT_CT_JOYME)
char* getJoymeLanEntryInfo(char **pBuf){
	int ret,devNum,i,index = 0;
	CtLanDevInfo *devInfoList = NULL, *devInfoList_save = NULL;
	char *pDevInfo = NULL;
	char *Type[] = {"PC","phone","Pad","PC","STB","OTHER"};   //devtype
	char *connectType[] = {"0","0","1"};//ConnectType
	char *transport[] = {"0","1","2","3","4","5","6","7","8","9","10","11","12"};  //Port
	
	ret = joyme_lanGetDevDevInfo(&devInfoList_save, &devNum); 
	if(ret != 0){
		if(devInfoList_save) free(devInfoList_save);
		return NULL;
	}
	
	mobile_printf(MOBILE_INFO,"%s: num %d!...\n", __FUNCTION__, devNum);
	if(devNum <= 0 ) return NULL;
	
	pDevInfo = calloc(devNum, sizeof(CtLanDevInfo) + 40);
	if(pDevInfo == NULL){
		mobile_printf(MOBILE_ERR,"%s: malloc err!...\n",__FUNCTION__);
		if(devInfoList_save) free(devInfoList_save);
		return NULL;
	}
	devInfoList = devInfoList_save;
	for(i = 0; i < devNum; i++){
		index += sprintf(pDevInfo+index,"%s:%s",devInfoList->devName,Type[devInfoList->devType]);			
		index += sprintf(pDevInfo+index,":%02X%02X%02X%02X%02X%02X",
				devInfoList->macAddr[0], devInfoList->macAddr[1],devInfoList->macAddr[2],devInfoList->macAddr[3],devInfoList->macAddr[4],devInfoList->macAddr[5]);			
		index += sprintf(pDevInfo+index,":%s:%s",devInfoList->ipAddr,connectType[devInfoList->connType]);
		index += sprintf(pDevInfo+index,":%s",transport[devInfoList->port]);
		index += sprintf(pDevInfo+index,":%s:%s:%d/",devInfoList->brandName, devInfoList->osName, devInfoList->onlineTime);
		devInfoList++;			

	}
	pDevInfo[index-1] = '\0';
	*pBuf = pDevInfo;
	
	if(devInfoList_save) free(devInfoList_save);

	return *pBuf;
}
#endif

json_t* GetLanNetInfo(json_t* req_root, json_t* req_para)
{
	json_t *cursor = NULL, *reply_root = NULL, *reply_para = NULL;
	char *lannetinfo = NULL;

	mobile_printf(MOBILE_INFO,"%s:start!...\n",__FUNCTION__);
#if defined(TCSUPPORT_CT_JOYME)
	getJoymeLanEntryInfo(&lannetinfo);
#else
	lannetinfo = (char *)malloc( LANINFOMAXENTRY*LANINFOENTRYSIZE );
	if(lannetinfo == NULL){
		mobile_printf(MOBILE_ERR, "%s, malloc err !\n", __FUNCTION__);
		return NULL;
	}		
	memset(lannetinfo, 0, LANINFOMAXENTRY*LANINFOENTRYSIZE);
	getLanEntryInfo(lannetinfo);	
#endif
	//build parameter
	if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
		goto err;
	}			
	if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_INFO, lannetinfo)){
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

	if(lannetinfo) free(lannetinfo);
	return reply_root;

err:
	if(reply_root) ParameterFreeJSON(&reply_root);
	if(reply_para) ParameterFreeJSON(&reply_para);
	free(lannetinfo);
	return NULL;

}
