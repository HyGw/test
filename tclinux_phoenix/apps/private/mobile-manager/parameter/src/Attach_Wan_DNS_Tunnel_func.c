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

#include "Cwmp_RPC_func.h"
#include "Attach_Wan_DNS_Tunnel_func.h"
#include <sys/socket.h> 
#include <netinet/in.h>
#include <syslog.h>

#if defined(TCSUPPORT_CT_JOYME)
/*
check ip version
-1: check fail, 1:IPv4, 2:IPv6
*/
int checkIPVer
(char *ip, struct in_addr *in_v4addr, struct in6_addr *in_v6addr)
{
	if ( !ip )
		return -1;

	if ( 1 == inet_pton(AF_INET, ip, in_v4addr) )
		return 1;

	if ( 1 == inet_pton(AF_INET6, ip, in_v6addr) )
		return 2;

	return -1;
}

/*
save domain to ip entry
-1: paramter NULL, -2: entry full
0: succeed
*/
int saveDomain(int pvc, char *domain)
{
	char nodename[64] = {0};
	int idx = 0, free_entry = -1;
	char dns_buf[256];

	if ( !domain )
		return -1;

	for ( idx = 0; idx < AWN_DOMAIN_NUM; idx ++ )
	{
		snprintf(nodename, sizeof(nodename) - 1
			, "AttachWanDNSTunnel_PVC%d_Entry%d", pvc, idx);

		if ( 0 == 
		tcapi_get(nodename, "Domain", dns_buf)
		&& 0 != dns_buf[0] )
		{
			/* already exist */
			if ( 0 == strcmp(dns_buf, domain) )
				return 0;
		}
		else
		{
			/* store free entry idx */
			if ( -1 == free_entry )
				free_entry = idx;
		}
	}

	if ( -1 != free_entry )
	{
		snprintf(nodename, sizeof(nodename) - 1
			, "AttachWanDNSTunnel_PVC%d_Entry%d", pvc, free_entry);
		/* keep dns max buffer size */
		bzero(dns_buf, sizeof(dns_buf));
		snprintf(dns_buf, sizeof(dns_buf) - 1
			, "%s", domain);
		tcapi_set(nodename, "Domain", dns_buf);
	}
	else
		return -2;

	return 0;
}

/*
add domain to V4 ip entry
-1: paramter NULL, -2: ip pool full -3: dns pool fail
0: succeed
*/
int addDNS2EntryV4(struct in_addr in_v4addr, char *ip, char *domain)
{
	char ipaddr[20] = {0};
	char ipver[4] = {0};
	char nodename_pvc[64] = {0};
	int idx = 0;
	struct in_addr in_local_v4addr = {0};
	int free_pvc = -1;

	if ( !domain || !ip )
		return -1;

	for ( idx = 0; idx < AWN_IP_NUM; idx ++ )
	{
		snprintf(nodename_pvc, sizeof(nodename_pvc) - 1
			, "AttachWanDNSTunnel_PVC%d", idx);
		/* get IPv4 PVC */
		if ( 0 == 
			tcapi_get(nodename_pvc, "IPVer", ipver)
			&& 0 != ipver[0] )
		{
			/* IPv4 */
			if ( 1 == atoi(ipver) )
			{
				if ( 0 == 
				tcapi_get(nodename_pvc, "IP", ipaddr)
				&& 0 != ipaddr[0] )
				{
					if ( 1 == inet_pton(AF_INET, ipaddr, &in_local_v4addr) )
					{
						/* found IP PVC */
						if ( in_local_v4addr.s_addr == in_v4addr.s_addr )
						{
							if ( -2 == saveDomain(idx, domain) )
								return -3; /* dns pool full */

							return 0;
						}
					}
					else
					{
						/* store free pvc idx */
						if ( -1 == free_pvc )
							free_pvc = idx;
					}
				}
				else
				{
					/* store free pvc idx */
					if ( -1 == free_pvc )
						free_pvc = idx;
				}
			}
		}
		else
		{
			/* store free pvc idx */
			if ( -1 == free_pvc )
				free_pvc = idx;
		}
	}

	if ( -1 != free_pvc )
	{
		snprintf(nodename_pvc, sizeof(nodename_pvc) - 1
			, "AttachWanDNSTunnel_PVC%d", free_pvc);
		tcapi_set(nodename_pvc, "IPVer", "1");
		tcapi_set(nodename_pvc, "IP", ip);

		saveDomain(free_pvc, domain);
	}
	else
		return -2;
	
	return 0;
}

/*
add domain to V6 ip entry
-1: paramter NULL, -2: ip pool full -3: dns pool fail
0: succeed
*/
int addDNS2EntryV6(struct in6_addr in_v6addr, char *ip, char *domain)
{
	char ipaddr[64] = {0};
	char ipver[4] = {0};
	char nodename_pvc[64] = {0};
	int idx = 0;
	struct in6_addr in_local_v6addr = {0};
	int free_pvc = -1;

	if ( !domain || !ip )
		return -1;

	for ( idx = 0; idx < AWN_IP_NUM; idx ++ )
	{
		snprintf(nodename_pvc, sizeof(nodename_pvc) - 1
			, "AttachWanDNSTunnel_PVC%d", idx);
		/* get IPv6 PVC */
		if ( 0 == 
			tcapi_get(nodename_pvc, "IPVer", ipver)
			&& 0 != ipver[0] )
		{
			/* IPv6 */
			if ( 2 == atoi(ipver) )
			{
				if ( 0 == 
				tcapi_get(nodename_pvc, "IP", ipaddr)
				&& 0 != ipaddr[0] )
				{
					if ( 1 == inet_pton(AF_INET6, ipaddr, &in_local_v6addr) )
					{
						/* found IP PVC */
						if ( 0 ==
							 memcmp(&in_local_v6addr
							 , &in_v6addr
							 , sizeof(struct in6_addr)) )
						{
							if ( -2 == saveDomain(idx, domain) )
								return -3; /* dns pool full */

							return 0;
						}
					}
					else
					{
						/* store free pvc idx */
						if ( -1 == free_pvc )
							free_pvc = idx;
					}
				}
				else
				{
					/* store free pvc idx */
					if ( -1 == free_pvc )
						free_pvc = idx;
				}
			}
		}
		else
		{
			/* store free pvc idx */
			if ( -1 == free_pvc )
				free_pvc = idx;
		}
	}

	if ( -1 != free_pvc )
	{
		snprintf(nodename_pvc, sizeof(nodename_pvc) - 1
			, "AttachWanDNSTunnel_PVC%d", free_pvc);
		tcapi_set(nodename_pvc, "IPVer", "2");
		tcapi_set(nodename_pvc, "IP", ip);

		saveDomain(free_pvc, domain);
	}
	else
		return -2;
	
	return 0;
}


/*
Attach_Wan_DNS_Tunnel_func process
*/
json_t* Attach_Wan_DNS_Tunnel_func
(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL, *reply_err_msg = NULL;
	json_t *listnodes = NULL, *cursor = NULL;
	json_t *curnode = NULL, *curnodeval = NULL;
	char *server_ip = NULL;
	char errorMSG[128] = {0};
	int ipver = 0, retcd = 0;
	struct in_addr in_v4addr = {0};
	struct in6_addr in_v6addr = {0};

	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__, __LINE__);

#ifdef TCSUPPORT_SYSLOG
	openlog("TCSysLog", 0, LOG_LOCAL0);
	syslog(LOG_EMERG, "Received ATTACH_WAN_DNS_TUNNEL RPC\n");
	closelog();
#endif

	server_ip = ParameterSearchValue(req_para, "server_ip");
	if ( !server_ip )
	{
		strcpy(errorMSG, "Parameter server_ip NOT FOUND!");
		goto REPLY_ERR_MSG;
	}

	/* check ip */
	if ( 1 == inet_pton(AF_INET, server_ip, &in_v4addr) )
		ipver = 1; /* IPv4 */
	else if ( 1 == inet_pton(AF_INET6, server_ip, &in_v6addr) )
		ipver = 2; /* IPv6 */
	else
	{
		strcpy(errorMSG, "Parameter server_ip invalid!");
		goto REPLY_ERR_MSG;
	}

	/*
	find node list
	*/
	listnodes = ParameterSearchFirstLabel(req_para, "List");
	if ( !listnodes || !listnodes->child
		|| !listnodes->child->child )
	{
		strcpy(errorMSG, "Not found List objects");
		goto REPLY_ERR_MSG;
	}	

	/*
		listnodes->child meas:	"List":[   type->JSON_ARRAY
		listnodes->child->child meas:  "List":[{   type->JSON_OBJECT
	*/
	for (cursor = listnodes->child->child;
		cursor != NULL; cursor = cursor->next)
	{
		/*
			cursor->child meas:  "List":[{ "DomainName",
			cursor->child->child  meas:  "List":[{ "DomainName":"Val"
		*/
		if ( cursor->child /* Name string */
			&& JSON_STRING == cursor->child->type
			&& cursor->child->child /* Name value string */
			&& JSON_STRING == cursor->child->child->type )
		{
			curnode = cursor->child;
			curnodeval = cursor->child->child;

			if ( 0 == strcmp (curnode->text, "DomainName")
				&& 0 != curnodeval->text[0] )
			{
				retcd = 0;
				/* IPv4 */
				if ( 1 == ipver )
				{
					retcd =
					addDNS2EntryV4(in_v4addr, server_ip, curnodeval->text);
				}
				/* IPv6 */
				else if ( 2 == ipver )
				{
					retcd =
					addDNS2EntryV6(in_v6addr, server_ip, curnodeval->text);
				}

				switch( retcd )
				{
					case -1:
						goto err;
						break;
					case -2:
						strcpy(errorMSG, "Max support 8 server_ip");
						goto REPLY_ERR_MSG;
						break;
					case -3:
						strcpy(errorMSG, "Max support 8 domain per server_ip");
						goto REPLY_ERR_MSG;
						break;
					default:
						break;
				}
			}/* if ( 0 == strcmp (cursor->text ..*/
		}/* if ( cursor->child && JSON_STRING ...*/
	}/* for (cursor = listnodes-> ...*/

	tcapi_commit("AttachWanDNSTunnel");
	tcapi_save();

	/* set return parameter status to ZERO default ok */
	if(!(reply_para = ParameterBuildJSONReplyPara(req_para
		, PARA_RESULT_SUCCESS)))
	{
		mobile_printf(MOBILE_INFO,
			"%s:ParameterBuildJSONReplyPara Err!...\n", __FUNCTION__);
		goto err;
	}

	if( !(reply_root = ParameterBuildJSONReplyHead(req_root
		, PARA_RESULT_SUCCESS)) )
	{
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyHead Err!...\n"
			,__FUNCTION__);
		goto err;
	}
	if ( ParameterBuildJSONAddPara2Head(reply_root, reply_para) )
	{
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONAddPara2Head Err!...\n"
			,__FUNCTION__);
		goto err;
	}

	if ( reply_para ) ParameterFreeJSON(&reply_para);

	return reply_root;

REPLY_ERR_MSG:
	if(!(reply_err_msg = ParameterBuildJSONReplyPara(req_para, 1)))
	{
		mobile_printf(MOBILE_INFO,
			"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_err_msg
				, PARA_FAIL_REASON, errorMSG))
	{
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n"
			,__FUNCTION__);
		goto err;
	}
	if(!(reply_root = ParameterBuildJSONReplyHead(req_root
		, PARA_RESULT_SUCCESS)))
	{
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyHead Err!...\n"
			,__FUNCTION__);
		goto err;
	}
	if(ParameterBuildJSONAddPara2Head(reply_root, reply_err_msg))
	{
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONAddPara2Head Err!...\n"
			,__FUNCTION__);
		goto err;
	}

	if ( reply_err_msg ) ParameterFreeJSON(&reply_err_msg);

	return reply_root;

err:
	if ( reply_root ) ParameterFreeJSON(&reply_root);
	if ( reply_para ) ParameterFreeJSON(&reply_para);

	return NULL;
}


/*
delete domain from ip entry
-1: paramter NULL
0: succeed
*/
int delDomain(int pvc, char *domain)
{
	char nodename[64] = {0};
	int idx = 0, free_entry = -1, entry_cnt = 0;
	char dns_buf[256];

	if ( !domain )
		return -1;

	for ( idx = 0; idx < AWN_DOMAIN_NUM; idx ++ )
	{
		snprintf(nodename, sizeof(nodename) - 1
			, "AttachWanDNSTunnel_PVC%d_Entry%d", pvc, idx);

		if ( 0 == 
		tcapi_get(nodename, "Domain", dns_buf)
		&& 0 != dns_buf[0] )
		{
			/* already exist */
			if ( 0 == strcmp(dns_buf, domain) )
				tcapi_unset(nodename);
			else
				entry_cnt++;
		}
	}

	/* there is no domain, delete the IP entry. */
	if ( 0 == entry_cnt )
	{
		snprintf(nodename, sizeof(nodename) - 1
			, "AttachWanDNSTunnel_PVC%d", pvc);

		tcapi_unset(nodename);
	}

	return 0;
}


/*
delete domain from V4 ip entry
-1: paramter NULL
0: succeed
*/
int delDNSfromEntryV4(struct in_addr in_v4addr, char *domain)
{
	char ipaddr[20] = {0};
	char ipver[4] = {0};
	char nodename_pvc[64] = {0};
	int idx = 0;
	struct in_addr in_local_v4addr = {0};

	if ( !domain )
		return -1;

	for ( idx = 0; idx < AWN_IP_NUM; idx ++ )
	{
		snprintf(nodename_pvc, sizeof(nodename_pvc) - 1
			, "AttachWanDNSTunnel_PVC%d", idx);
		/* get IPv4 PVC */
		if ( 0 == 
			tcapi_get(nodename_pvc, "IPVer", ipver)
			&& 0 != ipver[0] )
		{
			/* IPv4 */
			if ( 1 == atoi(ipver)
				&& 0 == 
				tcapi_get(nodename_pvc, "IP", ipaddr)
				&& 0 != ipaddr[0] )
			{
				if ( 1 == inet_pton(AF_INET, ipaddr, &in_local_v4addr) )
				{
					/* found IP PVC */
					if ( in_local_v4addr.s_addr == in_v4addr.s_addr )
					{
						return delDomain(idx, domain);
					}
				}
			}
		}
	}

	return 0;
}

/*
delete domain from V6 ip entry
-1: paramter NULL
0: succeed
*/
int delDNSfromEntryV6(struct in6_addr in_v6addr, char *domain)
{
	char ipaddr[64] = {0};
	char ipver[4] = {0};
	char nodename_pvc[64] = {0};
	int idx = 0;
	struct in6_addr in_local_v6addr = {0};

	if ( !domain )
		return -1;

	for ( idx = 0; idx < AWN_IP_NUM; idx ++ )
	{
		snprintf(nodename_pvc, sizeof(nodename_pvc) - 1
			, "AttachWanDNSTunnel_PVC%d", idx);
		/* get IPv6 PVC */
		if ( 0 == 
			tcapi_get(nodename_pvc, "IPVer", ipver)
			&& 0 != ipver[0] )
		{
			/* IPv6 */
			if ( 2 == atoi(ipver)
				&& 0 == 
				tcapi_get(nodename_pvc, "IP", ipaddr)
				&& 0 != ipaddr[0] )
			{
				if ( 1 == inet_pton(AF_INET6, ipaddr, &in_local_v6addr) )
				{
					/* found IP PVC */
					if ( 0 ==
						 memcmp(&in_local_v6addr
						 , &in_v6addr
						 , sizeof(struct in6_addr)) )
					{
						return delDomain(idx, domain);
					}
				}
			}
		}
	}

	return 0;
}

/*
delete all domain
0: succeed
*/
int delAllDNSEntry()
{
	char nodename_pvc[64] = {0};
	int idx = 0;

	for ( idx = 0; idx < AWN_IP_NUM; idx ++ )
	{
		snprintf(nodename_pvc, sizeof(nodename_pvc) - 1
			, "AttachWanDNSTunnel_PVC%d", idx);
		tcapi_unset(nodename_pvc);
	}

	return 0;
}

/*
delete domain ip entry via domain
-1: paramter NULL
0: succeed
*/
int delDNSfromEntrybydomain(char *domain)
{
	char ipver[4] = {0};
	char nodename_pvc[64] = {0};
	int idx = 0;

	if ( !domain )
		return -1;

	for ( idx = 0; idx < AWN_IP_NUM; idx ++ )
	{
		snprintf(nodename_pvc, sizeof(nodename_pvc) - 1
			, "AttachWanDNSTunnel_PVC%d", idx);
		/* get IPv4 PVC */
		if ( 0 == 
			tcapi_get(nodename_pvc, "IPVer", ipver)
			&& 0 != ipver[0] )
		{
			delDomain(idx, domain);
		}
	}

	return 0;
}




/*
Detach_Wan_DNS_Tunnel_func process
*/
json_t* Detach_Wan_DNS_Tunnel_func
(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL, *reply_err_msg = NULL;
	json_t *listnodes = NULL, *cursor = NULL;
	json_t *curnode = NULL, *curnodeval = NULL;
	char *server_ip = NULL;
	char errorMSG[128] = {0};
	int ipver = 0, remove_type = 0;
	struct in_addr in_v4addr = {0};
	struct in6_addr in_v6addr = {0};

	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__, __LINE__);

#ifdef TCSUPPORT_SYSLOG
	openlog("TCSysLog", 0, LOG_LOCAL0);
	syslog(LOG_EMERG, "Received DETACH_WAN_DNS_TUNNEL RPC\n");
	closelog();
#endif

	server_ip = ParameterSearchValue(req_para, "server_ip");
	if ( !server_ip )
	{
		strcpy(errorMSG, "Parameter server_ip NOT FOUND!");
		goto REPLY_ERR_MSG;
	}

	if ( 0 == strcmp (server_ip, "all") )
		remove_type = 1;
	else if ( 0 == strcmp (server_ip, "other") )
		remove_type = 2;

	if ( 0 == remove_type ) /* set ip mode */
	{
		/* check ip */
		if ( 1 == inet_pton(AF_INET, server_ip, &in_v4addr) )
			ipver = 1; /* IPv4 */
		else if ( 1 == inet_pton(AF_INET6, server_ip, &in_v6addr) )
			ipver = 2; /* IPv6 */
		else
		{
			strcpy(errorMSG, "Parameter server_ip invalid!");
			goto REPLY_ERR_MSG;
		}
	}

	if ( 0 == remove_type /* set ip mode */
		|| 2 == remove_type ) /* other mode */
	{
		/*
		find node list
		*/
		listnodes = ParameterSearchFirstLabel(req_para, "List");
		if ( !listnodes || !listnodes->child
			|| !listnodes->child->child )
		{
			strcpy(errorMSG, "Not found List objects");
			goto REPLY_ERR_MSG;
		}	

		/*
			listnodes->child meas:	"List":[   type->JSON_ARRAY
			listnodes->child->child meas:  "List":[{   type->JSON_OBJECT
		*/
		for (cursor = listnodes->child->child;
			cursor != NULL; cursor = cursor->next)
		{
			/*
				cursor->child meas:  "List":[{ "DomainName",
				cursor->child->child  meas:  "List":[{ "DomainName":"Val"
			*/
			if ( cursor->child /* Name string */
				&& JSON_STRING == cursor->child->type
				&& cursor->child->child /* Name value string */
				&& JSON_STRING == cursor->child->child->type )
			{
				curnode = cursor->child;
				curnodeval = cursor->child->child;

				if ( 0 == strcmp (curnode->text, "DomainName")
					&& 0 != curnodeval->text[0] )
				{
					if ( 2 == remove_type ) /* other mode */
					{
						delDNSfromEntrybydomain(curnodeval->text);
					}
					else
					{
						/* IPv4 */
						if ( 1 == ipver )
						{
							delDNSfromEntryV4(in_v4addr, curnodeval->text);
						}
						/* IPv6 */
						else if ( 2 == ipver )
						{
							delDNSfromEntryV6(in_v6addr, curnodeval->text);
						}
					}
				}/* if ( 0 == strcmp (cursor->text ..*/
			}/* if ( cursor->child && JSON_STRING ...*/
		}/* for (cursor = listnodes-> ...*/
	}
	else
	{
		/* remove all */
		delAllDNSEntry();
	}

	tcapi_commit("AttachWanDNSTunnel");
	tcapi_save();

	/* set return parameter status to ZERO default ok */
	if(!(reply_para = ParameterBuildJSONReplyPara(req_para
		, PARA_RESULT_SUCCESS)))
	{
		mobile_printf(MOBILE_INFO,
			"%s:ParameterBuildJSONReplyPara Err!...\n", __FUNCTION__);
		goto err;
	}

	if( !(reply_root = ParameterBuildJSONReplyHead(req_root
		, PARA_RESULT_SUCCESS)) )
	{
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyHead Err!...\n"
			,__FUNCTION__);
		goto err;
	}
	if ( ParameterBuildJSONAddPara2Head(reply_root, reply_para) )
	{
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONAddPara2Head Err!...\n"
			,__FUNCTION__);
		goto err;
	}

	if ( reply_para ) ParameterFreeJSON(&reply_para);

	return reply_root;

REPLY_ERR_MSG:
	if(!(reply_err_msg = ParameterBuildJSONReplyPara(req_para, 1)))
	{
		mobile_printf(MOBILE_INFO,
			"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
		goto err;
	}
	if(JSON_OK != ParameterAddNewJSONChild(reply_err_msg
				, PARA_FAIL_REASON, errorMSG))
	{
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n"
			,__FUNCTION__);
		goto err;
	}
	if(!(reply_root = ParameterBuildJSONReplyHead(req_root
		, PARA_RESULT_SUCCESS)))
	{
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyHead Err!...\n"
			,__FUNCTION__);
		goto err;
	}
	if(ParameterBuildJSONAddPara2Head(reply_root, reply_err_msg))
	{
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONAddPara2Head Err!...\n"
			,__FUNCTION__);
		goto err;
	}

	if ( reply_err_msg ) ParameterFreeJSON(&reply_err_msg);

	return reply_root;

err:
	if ( reply_root ) ParameterFreeJSON(&reply_root);
	if ( reply_para ) ParameterFreeJSON(&reply_para);

	return NULL;
}
#endif

