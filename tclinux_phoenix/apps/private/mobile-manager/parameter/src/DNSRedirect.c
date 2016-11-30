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
#include "DNSRedirect.h"
#include "threadpool.h"
#if defined(TCSUPPORT_CT_JOYME)
#include <syslog.h>
#endif

#define PHONE_DNS_CONFIG_PATH "/tmp/phone_DNS_config"
void initRedirect_node()
{
	char Count[4] = {0}, modeAttributeName[16] = {0}, SIPAttributeName[16] = {0},  DIPAttributeName[16] = {0};
	int num = 0, i = 0;

	mobile_printf(MOBILE_INFO,"\n%s:start!...\n",__FUNCTION__);

	tcapi_get("mobile_DnsRedirect", "DNSSIPCount", Count);
	tcapi_set("mobile_DnsRedirect", "DNSIPCount", "");
	tcapi_set("mobile_DnsRedirect", "DNSERRDIPV4", "");		
	tcapi_set("mobile_DnsRedirect", "DNSERRDIPV6", "");		

	num = atoi(Count);
	for (i = 0;i < num; i++)
	{
		sprintf(modeAttributeName, "DNSMOD%d", i);
		sprintf(SIPAttributeName, "DNSSIP%d", i);
		sprintf(DIPAttributeName, "DNSDIP%d", i);

		tcapi_set("mobile_DnsRedirect", modeAttributeName, "");
		tcapi_set("mobile_DnsRedirect", SIPAttributeName, "");
		tcapi_set("mobile_DnsRedirect", DIPAttributeName, "");		
	 }
	return;
}

void findAssignedValue(char *name, char *tempvalue)
{
	int i = 0, j = 0, length = 0, flag = 0;
	char *start = NULL;

	mobile_printf(MOBILE_INFO,"\n%s:start!...\n",__FUNCTION__);

	memset(tempvalue, 0, sizeof(tempvalue));

	start = name;
	length = strlen(name);

	for(j = 0;j < length;j++)
	{
		if(flag == 0)
		{
			if(*start == '"')
			{
				flag = 1;
				start++;
			}
			else
			{
				start++;
			}
		}
		
		if(flag == 1)
		{
			if(*start != '"')
			{
				tempvalue[i] = *start;			
				i++;
				start++;
			}
			else
			{
				flag = 0;
				break;
			}
		}
	}

	return;
}

void *RedirectTask(worker_data_t *argv)
{
	char cmd_buf[128] = {0};
	char line[128] = {0}, tempmode[16] = {0}, tempSIP[48] = {0}, tempDIP[48] = {0};
	char modeAttributeName[16] = {0}, SIPAttributeName[16] = {0}, DIPAttributeName[16] = {0};
	char SIPCount[4] = {0}, mode[4] = {0}, SIP[48] = {0}, DIP[48] = {0};
	FILE  *fp;
	int index = 0, gDNS_ERRIPv4 = 0, gDNS_ERRIPv6 = 0, ret = 0;

	if(argv->ptr != NULL)
	{
		
		sprintf(cmd_buf,"/userfs/bin/wget -O %s %s", PHONE_DNS_CONFIG_PATH, argv->ptr);
		//sprintf(cmd_buf,"/userfs/bin/axel -q -n 4 -o /tmp/phone_DNS_config %s", argv->ptr);
#if defined(TCSUPPORT_CT_JOYME)
#ifdef TCSUPPORT_SYSLOG
		openlog("TCSysLog", 0, LOG_LOCAL0);
		syslog(LOG_EMERG, "Downloading dns redirect configurations\n");
		closelog();
#endif
#endif
	
		system(cmd_buf);
		free(argv->ptr);

		fp = fopen(PHONE_DNS_CONFIG_PATH, "rb");
		if(fp == NULL)
		{
			ret = -1;
		}

		initRedirect_node();

		if(ret != -1)
		{
			while (fgets (line, sizeof(line), fp) != NULL )
			{
				if(strstr(line, PARA_ERRREDIRECT) != NULL)
				{		
					memset(tempmode, 0, sizeof(tempmode));
					memset(tempSIP, 0, sizeof(tempSIP));
					memset(tempDIP, 0, sizeof(tempDIP));
					memset(mode, 0, sizeof(mode));
					memset(SIP, 0, sizeof(SIP));
					memset(DIP, 0, sizeof(DIP));
			
					sscanf( line, "%*s %s %s %s", tempmode, tempSIP, tempDIP );
			
					findAssignedValue(tempmode, mode);
					findAssignedValue(tempSIP, SIP);		
					findAssignedValue(tempDIP, DIP);
			
					sprintf(modeAttributeName, "DNSMOD%d", index);
					sprintf(SIPAttributeName, "DNSSIP%d", index);
					sprintf(DIPAttributeName, "DNSDIP%d", index);

					if(0 == strcmp(SIP,""))
					{
						if(1 == inet_pton (AF_INET, DIP, &gDNS_ERRIPv4))
						{
							tcapi_set("mobile_DnsRedirect", "DNSERRDIPV4MOD", mode);
							tcapi_set("mobile_DnsRedirect", "DNSERRDIPV4", DIP);
						}
						else if(1 == inet_pton (AF_INET6, DIP, &gDNS_ERRIPv6))
						{
							tcapi_set("mobile_DnsRedirect", "DNSERRDIPV6MOD", mode);
							tcapi_set("mobile_DnsRedirect", "DNSERRDIPV6", DIP);
						}
					}
					else
					{	
						tcapi_set("mobile_DnsRedirect", modeAttributeName, mode);
						tcapi_set("mobile_DnsRedirect", SIPAttributeName, SIP);			
						tcapi_set("mobile_DnsRedirect", DIPAttributeName, DIP);
						
						index++;
					}
				}
			
				sprintf(SIPCount,"%s",itoa(index)); 
				tcapi_set("mobile_DnsRedirect", "DNSIPCount", SIPCount);
			}

			system("killall -9 dnsmasq");
			system("/userfs/bin/dnsmasq &");

			ret = 0;
			fclose(fp);
		}	
	}
	return NULL;

}


json_t* DNSRedirect(json_t* req_root, json_t* req_para)
{
	json_t* reply_root = NULL, *reply_para = NULL;
	int ret = 0;
	char *url = NULL, *temp = NULL;
	worker_data_t dataptr;
	
	mobile_printf(MOBILE_INFO,"\n%s:start!...\n",__FUNCTION__);

#if defined(TCSUPPORT_CT_JOYME)
#ifdef TCSUPPORT_SYSLOG
	openlog("TCSysLog", 0, LOG_LOCAL0);
	syslog(LOG_EMERG, "Received SET_REDIRECT_INFO RPC\n");
	closelog();
#endif
#endif

	url = ParameterSearchValue(req_para, "downurl");
	if(url != NULL)
	{
		temp = malloc(strlen(url)+1);
		if(temp){
			dataptr.ptr=temp;
			strcpy(temp, url);
			ret = ThreadPoolAddWork(&gThreadPool, RedirectTask, &dataptr);
		}
	}
	else{
		ret = -1;
	}

	//build parameter
	if(ret == -1){
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
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

