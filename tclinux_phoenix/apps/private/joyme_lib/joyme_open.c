/******************************************************************************/
/*
 * Copyright (C) 1994-2008 TrendChip Technologies, Corp.
 * All Rights Reserved.
 *
 * TrendChip Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of TrendChip Technologies, Corp. and
 * shall not be reproduced, copied, disclosed, or used in whole or
 * in part for any reason without the prior express written permission of
 * TrendChip Technologies, Corp.
 */
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/msg.h>
#include <sys/uio.h>
#include "joyme.h"


/*
 *@param serverIpAddr(IN): L2TP server IP
 *@param username(IN): L2TP server name 
 *@param password(IN): L2TP server password
 *@param tunnelName(OUT): tunnel name
 *@param nameLen(IN): L2TP server password
 *@return 0:success -1:fail
 */

typedef enum {
	JOYME_NONE = 0,
	JOYME_ERR,
	JOYME_INFO,
	JOYME_DEBUG,
	JOYME_ALL
} DebugLevel;

int GetMacAddr(unsigned char *macstr, unsigned char *macnum)
{
        int temp[6];
        int i;
        
	i = sscanf(macstr, "%02x:%02x:%02x:%02x:%02x:%02x", &temp[0],&temp[1],&temp[2],&temp[3],&temp[4],&temp[5]);
	if(i != 6){
		joyme_printf(JOYME_ERR,"%s: get MAC Err!!\n", __FUNCTION__);
		return -1;
	}
        for(i=0;i<6;i++){
                macnum[i] = (unsigned char)temp[i];
        }
	return 0;
}
int GetNamesAndTime(unsigned char *mac1,char *devname,char *osname,char *brandname,unsigned long *time)
{
	char mac[20] = {0};
	FILE *fp = NULL;
	char buf[512] = {0};
	char macbuf[20] = {0};
	int flag = 0,ret = 0;
	if(mac1 == NULL)
		return -1;
	
	sprintf(mac,"%02x:%02x:%02x:%02x:%02x:%02x",mac1[0],
		mac1[1],mac1[2],mac1[3],mac1[4],mac1[5]);

	joyme_printf(JOYME_INFO,"%s: mac=%s\n",__FUNCTION__,mac);

	fp = fopen(ATTACH_LANINFO_PATH,"r");
	
	if(fp == NULL){
		*time = 1;
		strcpy(devname,"anonymous");
		strcpy(osname,"anonymous");
		strcpy(brandname,"anonymous");
		return -1;
	}
	while(fgets(buf,sizeof(buf),fp))
	{
		sscanf(buf,"%s %lu %d %s %s %s",macbuf,time,&flag,devname,osname,brandname);
		joyme_printf(JOYME_INFO,"%s: macbuf=%s,time=%ld\n",__FUNCTION__,macbuf,*time);
		if(!strcasecmp(mac,macbuf)){
			
			joyme_printf(JOYME_INFO,"%s: mac match\n",__FUNCTION__);
			ret = 1;
			break;
		}
	}
	if(ret == 0){
		*time = 1;
		strcpy(devname,"anonymous");
		strcpy(osname,"anonymous");
		strcpy(brandname,"anonymous");
	}
	fclose(fp);	
	return 0;
}

int joyme_wanCreateL2tpTunnel(const char *userId,
								const char *serverIpAddr,
								const char *username ,
								const char *password ,
								char *tunnelName,
								int nameLen){
	
	int i = 0, ret = 0;
	char strValue[64]={0}, NodeName[32]={0};
	
	joyme_printf(JOYME_INFO, "\r\n %s:start ...\n", __FUNCTION__);
	/* find empty vpn index */
	for (i = 0; i < VPN_INSTANCE_NUM; i++) {
		memset(NodeName, 0, sizeof(NodeName));
		sprintf(NodeName, "%s%d", VPN_ENTRY, i);
		memset(strValue, 0, sizeof(strValue));
		ret = tcapi_get(NodeName, "vpnstatus", strValue);
		if (!strcmp(strValue, "0") || ret != 0) {
			break;
		}
	}

	/* only support VPN_INSTANCE_NUM vpn */
	if (i == VPN_INSTANCE_NUM){
		return -1;
	}


	/* start to crate vpn */
	tcapi_set(NodeName, "userId", userId);
	tcapi_set(NodeName, "serverIpAddr", serverIpAddr);
	tcapi_set(NodeName, "username", serverIpAddr);
	tcapi_set(NodeName, "password", password);
	tcapi_set(NodeName, "vpnaction", "ADD");
	tcapi_commit(NodeName);
	tcapi_save();

	/* check vpn result */
	memset(strValue, 0, sizeof(strValue));
	tcapi_get(NodeName, "vpnstatus", strValue);
	if (!strcmp(strValue, "1")) { 
		tcapi_get(NodeName, "tunnelName", tunnelName);
		return 0;
	}
	else {
		return -1;
	}

}

/*
 *@param tunnelName(IN): 
 *@return 0:success -1:fail -2:vpntunnel not exist -3:userId not exist
 */
int joyme_wanRemoveL2tpTunnel(const char *userId,const char *tunnelName){
	int ret = 0, number = 0, i =0 , flag = 0;
	int tunnelName_num = 0, userId_num = 0;
	char strValue[64]={0},NodeName[32]={0};
	
	joyme_printf(JOYME_INFO, "\r\n %s:start ...\n", __FUNCTION__);
	if( !strcmp(tunnelName, "all")){
		/*tunnelName is all ,userId is 0 , remove all the tunnel ,don't match the tunnelName  and userId*/
		if (!strcmp(userId , "0")) {        
			for(i = 0; i < VPN_INSTANCE_NUM; i++){
				sprintf(NodeName, "%s%d", VPN_ENTRY, i);
				
				memset(strValue, 0, sizeof(strValue));
				tcapi_get(NodeName, "vpnstatus", strValue);
				
				if (!strcmp(strValue, "1")) {									
				   /*excute the remove tunnel*/
				   tcapi_set(NodeName, "vpnaction", "DEL");
				   tcapi_commit(NodeName);
				   
				   memset(strValue, 0, sizeof(strValue));
				   tcapi_get(NodeName, "vpnstatus", strValue);
				   if (!strcmp(strValue, "1")) { 		  
					   flag = 1;
					   continue;
				   }
				}	
			}
			if (flag!=1)
				ret = 0;
			else
				ret = -1;
		}
		else { 
			/*tunnelName is all ,but userId isn't 0 ,match the userId */
			for(i = 0; i < VPN_INSTANCE_NUM; i++){
				sprintf(NodeName, "%s%d", VPN_ENTRY, i);
				
				memset(strValue, 0, sizeof(strValue));
				tcapi_get(NodeName, "vpnstatus", strValue);
				
				if (!strcmp(strValue, "1")) {  
					/* t he tunnel is exist */
					memset(strValue, 0, sizeof(strValue));
					tcapi_get(NodeName, "userId", strValue);
					if(!strcmp(userId,strValue)){   
						userId_num++;
						
						/*excute the remove tunnel*/
						tcapi_set(NodeName, "vpnaction", "DEL");
						tcapi_commit(NodeName);
						
						memset(strValue, 0, sizeof(strValue));
						tcapi_get(NodeName, "vpnstatus", strValue);
						 /* if the status is 1 , remove fail*/
						if (!strcmp(strValue, "1")){
							flag = 1;
							continue;
						}
					}
				}
			}
			if(userId_num == 0)
				ret = -3;
			else if (flag != 1)
				ret = 0 ;
			else
				ret = -1 ;
		}
	}
	else { 
		/*tunnelName is not all ,  should match the userId*/
		memset(strValue, 0, sizeof(strValue));
		if(tcapi_get(VPN_COMMON, "number", strValue) == 0)
			number= atoi(strValue);
		
		for (i = 0; i < VPN_INSTANCE_NUM; i++) {
			sprintf(NodeName, "%s%d", VPN_ENTRY, i);
			memset(NodeName, 0, sizeof(NodeName));
			tcapi_get(NodeName, "vpnstatus", strValue);
			if(!strcmp(strValue, "1")){
				memset(strValue, 0, sizeof(strValue));
				tcapi_get(NodeName, "userId", strValue);
				if (!strcmp(userId, strValue)) {
					userId_num++;
					
					memset(strValue, 0, sizeof(strValue));
					tcapi_get(NodeName, "tunnelName", strValue);
					if(!strcmp(tunnelName, strValue)){
						tunnelName_num++;
						
						/***  excute the remove tunnel    ***/
						tcapi_set(NodeName, "vpnaction", "DEL");
						tcapi_commit(NodeName);
						
						memset(strValue, 0, sizeof(strValue));
						tcapi_get(NodeName, "vpnstatus", strValue);
						if (!strcmp(strValue, "1")) {
							/* if the status is 0 , remove fail*/
							flag = 1;
						}
						break;
					}
				}
			}
		}
		if (userId_num == 0)
			ret = -3;
		else if(tunnelName_num == 0)
			ret = -2;
		else if (flag != 1)
			ret = 0;
		else 
			ret = -1;
	}
	tcapi_save();

	return ret;
}

/*
 *@param tunnelName(IN): 
 *@param ipAddrList(IN): 
 *@param ipAddrNum(IN): 
 *@return 0:success -1:fail -2:vpntunnel not exist
 */
int joyme_wanAttachL2tpTunnel(const char *tunnelName,const CtIpAddr *ipAddrList, int ipAddrNum){
	int ret = -2, i = 0, j = 0,len = 0, attatchNum = 0;
	char strValue[64]={0}, nodeName[32]={0}, attrBuf[32] = {0}, valueBuf[32] = {0};
	
	joyme_printf(JOYME_INFO, "\r\n %s:start ...\n", __FUNCTION__);
	for(i = 0; i < VPN_INSTANCE_NUM; i++){
		memset(nodeName, 0, sizeof(nodeName));
		sprintf(nodeName, "%s%d", VPN_ENTRY, i);
		memset(strValue, 0, sizeof(strValue));
		tcapi_get(nodeName, "vpnstatus", strValue);
		if(!strcmp(strValue, "1")){
			memset(strValue, 0, sizeof(strValue));
			tcapi_get(nodeName, "tunnelName", strValue);
			if (!strcmp(tunnelName, strValue)) {
				for (j = 0; j < VPN_ATTACH_NUM; j++) {
					sprintf(attrBuf, "ipDomainNameAddr%d", j);
					memset(valueBuf, 0, sizeof(valueBuf));
					ret = tcapi_get(nodeName, attrBuf, valueBuf);
					if (ret != 0 || strlen(valueBuf) == 0) {
						tcapi_set(nodeName, attrBuf, ipAddrList->ipAddr);
					}
					
					ipAddrList++;
					attatchNum++;
					
					if (attatchNum == ipAddrNum)
						break;
				}

				tcapi_set(nodeName, "vpnaction", "Attach");
				tcapi_commit(nodeName);
				
				memset(strValue, 0, sizeof(strValue));
				tcapi_get(nodeName, "vpnattachstatus", strValue);
				if (!strcmp(strValue, "1")) {
					tcapi_save();
					return 0;
				}
				else
					return -1;
			}
		}
	}

	return -2;
}


/*
 *@param tunnelName(IN): 
 *@param ipAddrList(IN): 
 *@param ipAddrNum(IN): 
 *@return 0:success -1:fail -2:vpntunnel not exist
 */
int joyme_wanDetachL2tpTunnel(const char *tunnelName,const CtIpAddr *ipAddrList, int ipAddrNum){
	int ret = -2, i = 0, j = 0, k = 0, len = 0;
	char strValue[64]={0}, nodeName[32]={0}, attrBuf[32] = {0}, valueBuf[32] = {0};
	
	joyme_printf(JOYME_INFO, "\r\n %s:start ...\n", __FUNCTION__);
	for(i = 0; i < VPN_INSTANCE_NUM; i++){
		sprintf(nodeName, "%s%d", VPN_ENTRY, i);
		memset(strValue, 0, sizeof(strValue));
		tcapi_get(nodeName, "vpnstatus", strValue);
		if(!strcmp(strValue, "1")){   /* match the vpnstatus*/
			tcapi_get(nodeName, "tunnelName", strValue);
			if(!strcmp(tunnelName, strValue)){  
				/*match the tunnelName*/
				for (j = 0; j < ipAddrNum; j++) {
					for (k = 0; k < VPN_ATTACH_NUM; k++) {
						sprintf(attrBuf, "ipDomainNameAddr%d", k);
						memset(valueBuf, 0, sizeof(valueBuf));
						ret = tcapi_get(nodeName, attrBuf, valueBuf);
						if (strlen(valueBuf) > 0) {
							if (!strcmp(ipAddrList->ipAddr, valueBuf))
								tcapi_set(nodeName, attrBuf, "");
						}
					}
					ipAddrList++;
				}

				tcapi_set(nodeName, "vpnaction", "Attach");
				tcapi_commit(nodeName);

				/* check attach status */
				memset(strValue, 0, sizeof(strValue));
				tcapi_get(nodeName, "vpnattachstatus", strValue);
				if (!strcmp(strValue, "0")) {
					tcapi_save();
	return 0;
}
				else
					return -1;
			}
		}
	}
	return -2;
}


/*
 *@param tunnelName(IN): 
 *@param tunnelStatus(OUT): 
 *@return 0:success -1:fail
 */
int joyme_wanGetL2tpTunnelStatus(const char *tunnelName, int *tunnelStatus){

	int ret = -2, i = 0;
	char strValue[64]={0}, nodeName[32]={0}, attrBuf[32] = {0}, valueBuf[32] = {0};
	
	joyme_printf(JOYME_INFO, "\r\n %s:start ...\n", __FUNCTION__);
	for (i = 0; i < VPN_INSTANCE_NUM; i++) {
		sprintf(nodeName, "%s%d", VPN_ENTRY, i);
		memset(strValue, 0, sizeof(strValue));
		tcapi_get(nodeName, "tunnelName", strValue);
		if(!strcmp(tunnelName, strValue)) { 
			memset(nodeName, 0, sizeof(strValue));
			tcapi_get(nodeName, "vpnstatus", strValue);
			if (!strcmp(strValue, "1")) {
				*tunnelStatus = 0;
			}
			else
				*tunnelStatus = 1;
		}
	}

	return 0;
}

/*
 *@param devInfoList(OUT)
 *@param devNum(OUT): 
 *@return 0:success -1:fail
 */
int joyme_lanGetDevDevInfo(CtLanDevInfo **devInfoList, int *devNum){
	char tempbuf[512] = {0},nodename[64] = {0},flagtemp[20] = {0};
	char *path = "etc/udhcp_lease";
	int devnum,i,j,temp;
	char isupdate[][20] = {
		{"ipflag"},{"portflag"},
		{"devtypeflag"},{"conntypeflag"},
		{""}
	};
	int flag[4] = {0};
	CtLanDevInfo *devInfoTable = NULL;
	
	joyme_printf(JOYME_INFO,"%s: enter============>\n",__FUNCTION__);


	if(devInfoList == NULL || devNum == NULL )
		return -1;
	tcapi_get("LanInfo_Common","Number",tempbuf);
	devnum = atoi(tempbuf);
	tcapi_set("LanInfo_Common", "NoRead", "Yes");
	if(devnum > 0){
		*devInfoList = calloc(MAX_LDEV_NUM, sizeof(CtLanDevInfo));
		if(*devInfoList == NULL){
			tcapi_set("LanInfo_Common", "NoRead", "");
			tcdbg_printf("calloc error!\n");
			return -1;
		}
		devInfoTable = *devInfoList;
		for(i = 0;i < MAX_LDEV_NUM;i++){
			snprintf(nodename,sizeof(nodename)-1,"LanInfo_Entry%d",i);
			memset(tempbuf,0,sizeof(tempbuf));
			if(tcapi_get(nodename,"Active",tempbuf) == 0 &&
				!strcmp("1",tempbuf))
			{
				for(j=0;strlen(isupdate[j])!=0;j++)
				{
					memset(tempbuf,0,sizeof(tempbuf));
					tcapi_get(nodename,isupdate[j],tempbuf);
					flag[j] = atoi(tempbuf);
				}
				
				memset(tempbuf,0,sizeof(tempbuf));
				tcapi_get(nodename,"MacAddr",tempbuf);
				GetMacAddr(tempbuf,devInfoTable[i].macAddr);

				memset(tempbuf,0,sizeof(tempbuf));
				if(flag[0] == 1)
					tcapi_get(nodename,"IpAddr1",tempbuf);
				else
					tcapi_get(nodename,"IpAddr",tempbuf);
				
				strncpy(devInfoTable[i].ipAddr,tempbuf,
					sizeof(devInfoTable[i].ipAddr) -1);


				memset(tempbuf,0,sizeof(tempbuf));
				if(flag[1] == 1)
					tcapi_get(nodename,"Port1",tempbuf);
				else
					tcapi_get(nodename,"Port",tempbuf);
				temp = atoi(tempbuf);
				devInfoTable[i].port = temp;
				
				if(temp == 0)
					devInfoTable[i].connType = CT_LAN_DEV_CONN_UNKNOWN;
				else if(0 < temp && temp < 5)
					devInfoTable[i].connType = CT_LAN_DEV_CONN_WIRED;
				else if(5 <= temp && temp <= 8)
					devInfoTable[i].connType = CT_LAN_DEV_CONN_WLAN;
				
				memset(tempbuf,0,sizeof(tempbuf));
				if(flag[2] == 1){
					tcapi_get(nodename,"devType1",tempbuf);
					devInfoTable[i].devType = atoi(tempbuf);
				}
				else
					devInfoTable[i].devType = CT_LAN_DEV_UNKNOWN;
				
				GetNamesAndTime(devInfoTable[i].macAddr,devInfoTable[i].devName,
								devInfoTable[i].osName,devInfoTable[i].brandName,
								&devInfoTable[i].onlineTime);
			}
			*devNum = devnum;
		}
	}
	else{
		*devNum = 0;
		free(*devInfoList);
		*devInfoList = NULL;
	}
	tcapi_set("LanInfo_Common", "NoRead", "");

	joyme_printf(JOYME_INFO,"%s end,num=%d!!!\n",__FUNCTION__,*devNum);
	joyme_printf(JOYME_INFO,"%s: leave <============\n",__FUNCTION__);
	return 0;
}

/*
 *@param devInfo(IN)
 *devInfo->macAddr: the dev mac need to update
 *devInfo->devName: no need to update when devName is NULL
 *devInfo->devType: no need to update when devType is CT_LAN_DEV_UNKNOWN
 *devInfo->ipAddr: no need to update when ipAddr is  NULL
 *devInfo->connType: no need to update when connType is  CT_LAN_DEV_CONN_UNKNOWN
 *devInfo->port: no need to update when port is  0
 *devInfo->brandName: no need to update when brandName is NULL 
 *devInfo->osName: no need to update when osName is NULL 
 *devInfo->onlineTime: no need to update 
 *@return 0:success -1:fail
 */
int joyme_lanUpdateDevInfo(const CtLanDevInfo *devInfo){
	char NodeName[64] = {0},tempbuf[512] = {0};
	char macbuf[24] = {0},mac[20] = {0};
	int i,updateflag = 0;
	FILE *fp = NULL;
	char devName[64] = {0};
	char osName[64] = {0};
	char brandName[64] = {0};
	unsigned long time = 0;
	int flag = 0;
	long linelen = 0,len = 0;
	joyme_printf(JOYME_INFO,"%s: enter============>\n",__FUNCTION__);

	for(i = 0;i < MAX_LDEV_NUM;i++)
	{
		sprintf(NodeName,"LanInfo_Entry%d",i);
		sprintf(macbuf,"%02x:%02x:%02x:%02x:%02x:%02x",
			devInfo->macAddr[0],devInfo->macAddr[1],
			devInfo->macAddr[2],devInfo->macAddr[3],
			devInfo->macAddr[4],devInfo->macAddr[5]);
		
		joyme_printf(JOYME_INFO,"%s: macbuf=%s\n",__FUNCTION__,macbuf);
		if((tcapi_get(NodeName,"Active",tempbuf) == 0) &&
				!strcmp(tempbuf,"1"))
		{

			memset(tempbuf,0,sizeof(tempbuf));
			tcapi_get(NodeName,"MacAddr",tempbuf);
			if(!strcmp(tempbuf,macbuf))	{
				joyme_printf(JOYME_INFO,"%s: mac=%s,match!\n",__FUNCTION__,macbuf);
				if(devInfo->devType != CT_LAN_DEV_UNKNOWN){
					sprintf(tempbuf,"%d",devInfo->devType);
					tcapi_set(NodeName,"DevType1",tempbuf);
					
				}

				if(devInfo->ipAddr[0] != '\0'){
					tcapi_set(NodeName,"IpAddr1",devInfo->ipAddr);
					tcapi_set(NodeName,"ipflag","1");
				}

				if(devInfo->connType != CT_LAN_DEV_CONN_UNKNOWN){
					sprintf(tempbuf,"%d",devInfo->connType);
					tcapi_set(NodeName,"ConnType1",tempbuf);
				}

				if(devInfo->port != 0){
					sprintf(tempbuf,"%d",devInfo->port);
					tcapi_set(NodeName,"Port1",tempbuf);
				}

				fp = fopen(ATTACH_LANINFO_PATH,"r+");
				if(fp == NULL){
					joyme_printf(JOYME_INFO,"%s: laninfo.conf not exit return -1!\n",__FUNCTION__);
					return -1;
				}
				while(fgets(tempbuf,sizeof(tempbuf),fp))
				{
					linelen = strlen(tempbuf);
					len = len + linelen;
					sscanf(tempbuf,"%s %lu %d %s %s %s",mac,&time,
						&flag,devName,osName,brandName);
					if(!strcasecmp(mac,macbuf)){
						if(devInfo->devName[0] != '\0')
							sprintf(devName,"%s",devInfo->devName);
						if(devInfo->osName[0] != '\0')
							sprintf(osName,"%s",devInfo->osName);
						if(devInfo->brandName[0] != '\0')
							sprintf(brandName,"%s",devInfo->brandName);

						len -= linelen;
						fseek(fp,len,SEEK_SET);
						fprintf(fp,"%20s %10lu %5d %64s %64s %64s\n",mac,time,flag,
								devName,osName,brandName);
						break;
					}
				}

				joyme_printf(JOYME_INFO,"%s: fclose(fp)!\n",__FUNCTION__);
				fclose(fp);				
				break;
			}
		}else{
			joyme_printf(JOYME_INFO,"%s: not active!\n",__FUNCTION__);
		}
	
	}
	
	tcapi_save();
	joyme_printf(JOYME_INFO,"%s: leave return 0<============\n",__FUNCTION__);
	return 0;
}
/*
 *@return 0:disable !0:enable
 */
int checkBandWidthEnable(){
	char enable[8] = {0};
	int ret = -1;
	
	ret = tcapi_get(BANDWIDTH_COMMON_NODE,"Enable",enable);
	if(ret < 0 || strlen(enable) == 0){
		return 0;
	}

	return atoi(enable);
}

/*
 *@param macAddr(IN)
 *@param usBandwith(OUT):kpbs 
 *@param dsBandwith(OUT):kpbs 
 *@return 0:success -1:fail
 */
int joyme_lanGetDevBandwidth(const CtMacAddr *macAddr, int* usBandwidth, int* dsBandwidth){
	int i = 0; 
	char strMacAddr[20] = {0};
	char mac[20] = {0};
	char strUsBandwidth[32] = {0};
	char strDsBandwidth[32] = {0};
	char nodeName[32] = {0};	

	
	joyme_printf(JOYME_INFO,"%s: enter============>\n",__FUNCTION__);

	if(checkBandWidthEnable() == 0){
		/*Bandwidth is disable*/
		return 0;
	}

	
	sprintf(strMacAddr,"%02x%02x%02x%02x%02x%02x",macAddr->macAddr[0],macAddr->macAddr[1],macAddr->macAddr[2],macAddr->macAddr[3],macAddr->macAddr[4],macAddr->macAddr[5]);

	
	joyme_printf(JOYME_INFO,"%s: strMacAddr=%s\n",__FUNCTION__,strMacAddr);
	for(i = 0; i < DEV_MAC_RULE_NUM; i++){
		bzero(nodeName, sizeof(nodeName));
		sprintf(nodeName,"BandWidth_Entry%d",i);
		if(tcapi_get(nodeName, "Mac", mac) == 0 && strlen(mac) != 0){
			if(strcasecmp(strMacAddr,mac) == 0){
				/*mac find,set the rate*/				
				tcapi_get(nodeName, "upRate", strUsBandwidth); 
				tcapi_get(nodeName, "downRate", strDsBandwidth);

				*usBandwidth = atoi(strUsBandwidth);
				*dsBandwidth = atoi(strDsBandwidth);
				
				joyme_printf(JOYME_INFO,"%s: leave,*usBandwidth=%d,*dsBandwidth=%d <============\n",__FUNCTION__,*usBandwidth,*dsBandwidth);
				return 0;
			}
		}
	}
	
	joyme_printf(JOYME_INFO,"%s: leave:mac is not find <============\n",__FUNCTION__);
	/*mac is not exist*/
	return 0;
}




static int setDevMaxBandWidthNode(const CtMacAddr *macAddr, int usBandwidth, int dsBandwidth){	
	char nodeName[32] = {0};	
	char strMac[20] = {0};
	char strMacAddr[20] = {0};
	char strUsBandwidth[32] = {0};
	char strDsBandwidth[32] = {0};
	int i = 0;
	int firstUnusedIndex = -1;

	//tcdbg_printf("setDevMaxBandWidthNode:usBandwidth=%d,dsBandwidth=%d\n",usBandwidth,dsBandwidth);


	sprintf(strMac,"%02x%02x%02x%02x%02x%02x",
		macAddr->macAddr[0],macAddr->macAddr[1],macAddr->macAddr[2],macAddr->macAddr[3],macAddr->macAddr[4],macAddr->macAddr[5]);
	sprintf(strUsBandwidth,"%d",usBandwidth);
	sprintf(strDsBandwidth,"%d",dsBandwidth);

	//tcdbg_printf("setDevMaxBandWidthNode:strMac=%s\n",strMac);

	for(i = 0; i < TRTCM_MATCH_MAC_RULE_NUM; i++){
		bzero(nodeName, sizeof(nodeName));
		sprintf(nodeName,"MaxBandWidth_Entry%d",i);
		if(tcapi_get(nodeName, "mac", strMacAddr) == 0 && strlen(strMacAddr) != 0){
			if(strcasecmp(strMacAddr,strMac) == 0){
				/*mac find,set the rate*/
				tcapi_set(nodeName, "upRate", strUsBandwidth); 
				tcapi_set(nodeName, "downRate", strDsBandwidth);
				return 0;
			}
		}else if(firstUnusedIndex == -1){
			firstUnusedIndex = i;
		}
	}

	//tcdbg_printf("setDevMaxBandWidthNode:firstUnusedIndex=%d\n",firstUnusedIndex);

	if(firstUnusedIndex != -1){
		/*mac not find, find Entry to store the dev information*/
		sprintf(nodeName,"MaxBandWidth_Entry%d",firstUnusedIndex);
		tcapi_set(nodeName, "mac", strMac); 
		tcapi_set(nodeName, "upRate", strUsBandwidth); 
		tcapi_set(nodeName, "downRate", strDsBandwidth);
		return 0;
	}

	/*mac not find, and no Entry can be used*/
	//tcdbg_printf("setDevMaxBandWidthNode:macAddr=%s all entry used\n",macAddr);
	return -1;
}


int findDevMaxBandWidthNode(char *macAddr, int* usBandwidth, int* dsBandwidth){	
	char nodeName[32] = {0};	
	char strUsBandwidth[20] = {0};
	char strDsBandwidth[20] = {0};	
	char mac[32] = {0};	
	int i = 0;

	for(i = 0; i < TRTCM_MATCH_MAC_RULE_NUM; i++){
		bzero(nodeName, sizeof(nodeName));
		sprintf(nodeName,"MaxBandWidth_Entry%d",i);
		if(tcapi_get(nodeName, "mac", mac) == 0 && strlen(mac) != 0){
			if(strcasecmp(macAddr,mac) == 0){
				/*mac find,set the rate*/				
				tcapi_get(nodeName, "upRate", strUsBandwidth); 
				tcapi_get(nodeName, "downRate", strDsBandwidth);

				*usBandwidth = atoi(strUsBandwidth);
				*dsBandwidth = atoi(strDsBandwidth);
				return 0;
			}
		}
	}

	/*mac not find*/
	return -1;
}


/*
 *@param macAddr(IN)
 *@param usBandwith(IN):kpbs ,0:means no limit
 *@param dsBandwith(IN):kpbs ,0:means no limit
 *@return 0:success -1:fail
 */
int joyme_lanSetDevMaxBandwidth(const CtMacAddr *macAddr, int usBandwidth, int dsBandwidth){
	int ret = -1;
	joyme_printf(JOYME_INFO,"%s: enter============>\n",__FUNCTION__);
	ret = setDevMaxBandWidthNode(macAddr, usBandwidth, dsBandwidth);
	if(ret < 0){
		return -1;
	}

	tcapi_commit("MaxBandWidth");
	tcapi_save();	
	joyme_printf(JOYME_INFO,"%s: leave <============\n",__FUNCTION__);
	return 0;
}


/*
 *@param macAddr(IN)
 *@param usBandwith(OUT):kpbs ,0:means no limit
 *@param dsBandwith(OUT):kpbs ,0:means no limit
 *@return 0:success -1:fail
 */
int joyme_lanGetDevMaxBandwidth(const CtMacAddr *macAddr, int* usBandwidth, int* dsBandwidth){
	int ret = -1;
	char strMacAddr[20] = {0};
	joyme_printf(JOYME_INFO,"%s: enter============>\n",__FUNCTION__);

	sprintf(strMacAddr,"%02x%02x%02x%02x%02x%02x",
		macAddr->macAddr[0],macAddr->macAddr[1],macAddr->macAddr[2],macAddr->macAddr[3],macAddr->macAddr[4],macAddr->macAddr[5]);

	ret = findDevMaxBandWidthNode(strMacAddr, usBandwidth, dsBandwidth);
	if(ret < 0){
		*usBandwidth = 0;
		*dsBandwidth = 0;
		
		joyme_printf(JOYME_INFO,"%s: leave <============\n",__FUNCTION__);
		return 0;
	}

	joyme_printf(JOYME_INFO,"%s: leave <============\n",__FUNCTION__);

	return 0;
}


