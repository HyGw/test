/* dnsmasq is Copyright (c) 2000-2010 Simon Kelley

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 dated June, 1991, or
   (at your option) version 3 dated 29 June, 2007.
 
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
     
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "dnsmasq.h"
#include "../../../private/lib/libtcapi.h"
#if defined(TCSUPPORT_CT_JOYME)
#include <regex.h>
#endif

struct daemon *daemon;

static char *compile_opts = 
#ifndef HAVE_IPV6
"no-"
#endif
"IPv6 "
#ifndef HAVE_GETOPT_LONG
"no-"
#endif
"GNU-getopt "
#ifdef HAVE_BROKEN_RTC
"no-RTC "
#endif
#ifdef NO_FORK
"no-MMU "
#endif
#ifndef HAVE_DBUS
"no-"
#endif
"DBus "
#ifndef LOCALEDIR
"no-"
#endif
"I18N "
#ifndef HAVE_DHCP
"no-"
#endif
"DHCP "
#if defined(HAVE_DHCP) && !defined(HAVE_SCRIPT)
"no-scripts "
#endif
#ifndef HAVE_TFTP
"no-"
#endif
"TFTP";



static volatile pid_t pid = 0;
static volatile int pipewrite;

static int set_dns_listeners(time_t now, fd_set *set, int *maxfdp);
static void check_dns_listeners(fd_set *set, time_t now);
static void sig_handler(int sig);
static void async_event(int pipe, time_t now);
static void fatal_event(struct event_desc *ev);
static void poll_resolv(void);

//#ifdef TCSUPPORT_DNSEACHPVC
#if defined(TCSUPPORT_DNSEACHPVC) || defined(TCSUPPORT_TR69_BIND_PVC)
static void resolve_dnsServerConf();
void printfList();
struct DNS_Param gDNS_Param;
#endif



#if defined(TCSUPPORT_CT_DNSBIND)
struct URLFilter_Info gURLFilter_Param;
void fetchURLList(void);
int whitelistflag = 0;
#endif

#ifdef CT_COM_DEVICEREG 
extern int checkDevRegisterFlag(void);
int devreg = 0;
#endif
#if defined(TCSUPPORT_CUC_LANDING_PAGE)
int ponstate = 0;
extern int checkPonState(void);
#endif
#if defined(TCSUPPORT_CT_JOYME)
extern int pushViewstate;
extern int checkPushViewState(void);
#endif


#if defined(TCSUPPORT_CT_PHONEAPP)
struct dns_ip_group *gDNSIPGroup = NULL;
int gDNS_ERRFlag = 0;
struct all_addr gDNS_ERRIPv4 = {0};
struct all_addr gDNS_ERRIPv6 = {0};

int initDNSRedirectInfo()
{
	char mode[12] = {0};
	int i = 0, count = 0;
	char dnsCount[12] = {0}, buf[32] = {0}, dnsSIP[MAX_GRP_IPLEN] = {0}, dnsDIP[MAX_GRP_IPLEN] = {0};
	char dnsERRDIP[MAX_GRP_IPLEN] = {0};
	struct dns_ip_group **new_ip_group = NULL;


	memset(mode, 0, sizeof(mode));
	if ( 0 != tcapi_get("mobile_DnsRedirect", "DNSERRDIPV4MOD", mode) )
		memset(mode, 0, sizeof(mode));
	memset(dnsERRDIP, 0, sizeof(dnsERRDIP));
	if ( 0 == tcapi_get("mobile_DnsRedirect", "DNSERRDIPV4", dnsERRDIP)
		&& 0 != dnsERRDIP[0]
		&& 1 == atoi(mode) )
	{
		if ( 1 == inet_pton(AF_INET, dnsERRDIP, &gDNS_ERRIPv4) )
			gDNS_ERRFlag |= F_IPV4;
	}

#ifdef HAVE_IPV6
	memset(mode, 0, sizeof(mode));
	if ( 0 != tcapi_get("mobile_DnsRedirect", "DNSERRDIPV6MOD", mode) )
		memset(mode, 0, sizeof(mode));
	memset(dnsERRDIP, 0, sizeof(dnsERRDIP));
	if ( 0 == tcapi_get("mobile_DnsRedirect", "DNSERRDIPV6", dnsERRDIP)
		&& 0 != dnsERRDIP[0]
		&& 1 == atoi(mode) )
	{
		if ( 1 == inet_pton(AF_INET6, dnsERRDIP, &gDNS_ERRIPv6) )
			gDNS_ERRFlag |= F_IPV6;
	}
#endif

	if ( 0 != tcapi_get("mobile_DnsRedirect", "DNSIPCount", dnsCount) )
		return -3;
	count = atoi(dnsCount);
	if ( count <= 0 ) // no need to init IP info.
		return -4;

	new_ip_group = &gDNSIPGroup;

	for ( i = 0; i < count; i ++ )
	{
		sprintf(buf, "DNSSIP%d", i);
		if ( 0 != tcapi_get("mobile_DnsRedirect", buf, dnsSIP)
			|| 0 == dnsSIP[0] )
			continue;
		sprintf(buf, "DNSDIP%d", i);
		if ( 0 != tcapi_get("mobile_DnsRedirect", buf, dnsDIP)
			|| 0 == dnsDIP[0] )
			continue;
		sprintf(buf, "DNSMOD%d", i);
		if ( 0 != tcapi_get("mobile_DnsRedirect", buf, mode)
			|| 0 == mode[0] )
			continue;

		if ( !(*new_ip_group) )
			*new_ip_group = safe_malloc(sizeof(struct dns_ip_group));
		if ( !(*new_ip_group) ) // malloc failed.
			return -5;
		(*new_ip_group)->mode = atoi(mode);
		(*new_ip_group)->flags = 0;
		if ( 1 == inet_pton(AF_INET, dnsSIP, &((*new_ip_group)->SIP))
			&& 1 == inet_pton(AF_INET, dnsDIP, &((*new_ip_group)->DIP)) )
		{
			(*new_ip_group)->flags |= F_IPV4;
		}
#ifdef HAVE_IPV6
		else
		{
			if ( 1 == inet_pton(AF_INET6, dnsSIP, &((*new_ip_group)->SIP))
				&& 1 == inet_pton(AF_INET6, dnsDIP, &((*new_ip_group)->DIP)) )
				(*new_ip_group)->flags |= F_IPV6;
		}
#endif
		(*new_ip_group)->next = NULL;
		new_ip_group = &((*new_ip_group)->next);
	}

	return 0;
}

int getERRDNSDIP(struct all_addr **addr, int qtype, int *flags)
{
	if ( !addr || !flags )
		return -1;

	if ( T_A == qtype )
	{
		if ( gDNS_ERRFlag & F_IPV4 )
		{
			*addr = &gDNS_ERRIPv4;
			*flags |= F_IPV4;
			return 0;
		}
	}
#ifdef HAVE_IPV6
	else if ( T_AAAA == qtype )
	{
		if ( gDNS_ERRFlag & F_IPV6 )
		{
			*addr = &gDNS_ERRIPv6;
			*flags |= F_IPV6;
			return 0;
		}
	}
#endif
	else
		return -1;

	return -1;
}

int redirectHOST(struct all_addr *addr, int addrlen, int flags)
{
	struct dns_ip_group *cur_ip_group = NULL;

	if ( !addr )
		return -1;

	for ( cur_ip_group = gDNSIPGroup; cur_ip_group; )
	{
		if ( 1 == cur_ip_group->mode )
		{
			if ( (flags & F_IPV4)
				&& (cur_ip_group->flags & F_IPV4) )
			{
				if ( 0 == memcmp(addr, &(cur_ip_group->SIP), addrlen) )
				{
					memcpy(addr, &(cur_ip_group->DIP), addrlen);
#if defined(TCSUPPORT_CT_JOYME)
#ifdef TCSUPPORT_SYSLOG
					openlog("TCSysLog", 0, LOG_LOCAL0);
					syslog(LOG_EMERG, "DNS redirect for IPv4 address\n");
					closelog();
#endif
#endif
					return 0;
				}
			}
#ifdef HAVE_IPV6
			else if ( (flags & F_IPV6)
				&& (cur_ip_group->flags & F_IPV6) )
			{
				if ( 0 == memcmp(addr, &(cur_ip_group->SIP), addrlen) )
				{
					memcpy(addr, &(cur_ip_group->DIP), addrlen);
#if defined(TCSUPPORT_CT_JOYME)
#ifdef TCSUPPORT_SYSLOG
					openlog("TCSysLog", 0, LOG_LOCAL0);
					syslog(LOG_EMERG, "DNS redirect for IPv6 address\n");
					closelog();
#endif
#endif
					return 0;
				}
			}
#endif
		}
		cur_ip_group = cur_ip_group->next;
	}

	return -1;
}

#endif

#if defined(TCSUPPORT_CT_JOYME)
/* attach wan dns tunne server ip header */
struct attach_server_group *gAttachSvrIPGrp = NULL;

/*
init domain to server group.
*/
int initAttachDns(int pvc
	,struct attach_domain_group **dnsheader )
{
	struct attach_domain_group **new_dns_set = dnsheader;
	int idx = 0, dnsbufLen = 0;
	char nodename[64] = {0};
	char dns_buf[256] = {0};

	for ( idx = 0; idx < AWN_DOMAIN_NUM; idx ++ )
	{
		snprintf(nodename, sizeof(nodename) - 1
			, "AttachWanDNSTunnel_PVC%d_Entry%d", pvc, idx);

		if ( 0 == 
		tcapi_get(nodename, "Domain", dns_buf)
		&& 0 != dns_buf[0] )
		{
			if ( !(*new_dns_set) )
				*new_dns_set =
				safe_malloc(sizeof(struct attach_domain_group));
			/* malloc failed. */
			if ( !(*new_dns_set) )
				return -1;
			(*new_dns_set)->domain = NULL;
			dnsbufLen = strlen(dns_buf)+1;
			if ( !((*new_dns_set)->domain) )
				(*new_dns_set)->domain = 
				safe_malloc(dnsbufLen);
			/* malloc failed. */
			if ( !((*new_dns_set)->domain) )
				return -1;
			bzero((*new_dns_set)->domain, dnsbufLen);
			strncpy((*new_dns_set)->domain, dns_buf, dnsbufLen);
			(*new_dns_set)->next = NULL;
			new_dns_set = &((*new_dns_set)->next);
		}
	}

	return 0;
}

/*
init attach wan dns tunnel data.
*/
int initAttachWanDNSGroup()
{
	struct all_addr laddr = {0};
	char ipaddr[MAX_GROUP_IPLEN] = {0};
	char nodename_pvc[64] = {0};
	char ipver[4] = {0};
	int idx = 0, serv_port = NAMESERVER_PORT;
	struct attach_server_group **new_attach_ip_group = NULL;

	new_attach_ip_group = &gAttachSvrIPGrp;
	for ( idx = 0; idx < AWN_IP_NUM; idx ++)
	{
		snprintf(nodename_pvc, sizeof(nodename_pvc) - 1
			, "AttachWanDNSTunnel_PVC%d", idx);

		/* get IP PVC */
		if ( 0 == 
			tcapi_get(nodename_pvc, "IPVer", ipver)
			&& 0 != ipver[0] )
		{
			/* get server ip */
			if ( 0 == 
			tcapi_get(nodename_pvc, "IP", ipaddr)
			&& 0 != ipaddr[0] )
			{
				/* check ip valid */
				if ( 1 == inet_pton(AF_INET, ipaddr, &laddr)
#ifdef HAVE_IPV6
					|| 1 == inet_pton(AF_INET6, ipaddr, &laddr)
#endif
					)
				{
					if ( !(*new_attach_ip_group) )
						*new_attach_ip_group =
						safe_malloc(sizeof(struct attach_server_group));
					/* malloc failed. */
					if ( !(*new_attach_ip_group) )
						return -1;
					(*new_attach_ip_group)->dnslist = NULL;

					/* create socket */
#ifdef HAVE_IPV6
					/* IPv6 */
					if ( 2 == atoi(ipver) )
					{
						memcpy(&((*new_attach_ip_group)->addr.in6.sin6_addr)
													, &laddr
													, sizeof(struct in6_addr));
						(*new_attach_ip_group)->addr.in6.sin6_port
								= htons(serv_port);
						(*new_attach_ip_group)->addr.sa.sa_family
								= AF_INET6;
#ifdef HAVE_SOCKADDR_SA_LEN
						(*new_attach_ip_group)->addr.in6.sin6_len
								= sizeof((*new_attach_ip_group)->addr.in6);
#endif
					}
					else
#endif
					/* IPv4 */
					{
						(*new_attach_ip_group)->addr.in.sin_addr.s_addr
							= inet_addr(ipaddr);
						(*new_attach_ip_group)->addr.in.sin_port
							= htons(serv_port);	
						(*new_attach_ip_group)->addr.sa.sa_family
							= AF_INET;
#ifdef HAVE_SOCKADDR_SA_LEN
						(*new_attach_ip_group)->source_addr.in.sin_len
							= sizeof(struct sockaddr_in);
#endif
					}

					initAttachDns(idx, &((*new_attach_ip_group)->dnslist));
					(*new_attach_ip_group)->next = NULL;
					new_attach_ip_group = &((*new_attach_ip_group)->next);
				}
			}
		}
	}

	dispAttachWanDNSGroup();

	return 0;
}

/* check domain server tunnel */
int checkDnsTunnel(struct url *request_url, char *domain)
{
	struct attach_server_group *cur_server_group = NULL;
	struct attach_domain_group *cur_dns_group = NULL;
	int cflags = REG_EXTENDED, status = 0, svrcnt = 0;
	regmatch_t pmatch[1];
	const size_t nmatch = 1;
	regex_t reg;
	char *pattern = NULL;

	if ( !domain || !request_url )
		return -1;

	for ( cur_server_group = gAttachSvrIPGrp; cur_server_group; )
	{
		status = -1;
		for ( cur_dns_group = cur_server_group->dnslist; cur_dns_group; )
		{
			pattern = cur_dns_group->domain;
			if ( pattern )
			{
				/* 1. compile regx */
				if ( regcomp(&reg, pattern, cflags) < 0 )
				{
					tcdbg_printf("\n[%s]==>regcomp err occur\n"
						,__FUNCTION__);
					continue;
				}
				/* 2. match it */
				status = regexec(&reg, domain, nmatch, pmatch, 0);
				/* 3.  free it */
				regfree(&reg);
				/* match */
				if ( 0 == status )
					break;
			}

			/* next domain name */
			cur_dns_group = cur_dns_group->next;
		}

		if ( 0 == status )
		{
			request_url->addr[svrcnt] = cur_server_group->addr;
			request_url->addr_used[svrcnt] = 1;
			svrcnt ++;
		}

		/* next server ip */
		cur_server_group = cur_server_group->next;
	}

	if ( 0 != svrcnt )
		return 1;

	return 0;
}


/* check reply server address */
int isAttachWanServer(union mysockaddr *src)
{
	struct attach_server_group *cur_server_group = NULL;
	if ( !src )
		return -1;

	for ( cur_server_group = gAttachSvrIPGrp; cur_server_group; )
	{
		if ( sockaddr_isequal(&cur_server_group->addr, src) )
			return 1;

		/* next server ip */
		cur_server_group = cur_server_group->next;
	}

	return 0;
}


/*
attach wan dns tunnel data display.
*/
int dispAttachWanDNSGroup()
{
	struct attach_server_group *cur_server_group = NULL;
	struct attach_domain_group *cur_dns_group = NULL;
	char ipaddr[64] = {0}, showGroup[8] = {0};

	if( !(0 ==
		tcapi_get("WebCurSet_Entry", "attachdnsdisp", showGroup)
		&& 0 == strcmp("Yes", showGroup)) )
		return 1;
	
	for ( cur_server_group = gAttachSvrIPGrp; cur_server_group; )
	{
		tcdbg_printf("\n\nserverip info => family=[%X], port=[%X] "
			, cur_server_group->addr.sa.sa_family
			, cur_server_group->addr.in.sin_port);

		if ( AF_INET == cur_server_group->addr.sa.sa_family)
		{
			inet_ntop(AF_INET, &(cur_server_group->addr.in.sin_addr.s_addr)
				, ipaddr, sizeof(ipaddr));
			tcdbg_printf("address=[%s]"
				, ipaddr);
		}
		else
		{
			inet_ntop(AF_INET6, &(cur_server_group->addr.in6.sin6_addr)
				, ipaddr, sizeof(ipaddr));
				tcdbg_printf("address=[%s]"
					, ipaddr);
		}

		tcdbg_printf("\ndomain info =>\n");
		for ( cur_dns_group = cur_server_group->dnslist; cur_dns_group; )
		{
			tcdbg_printf("domain=[%s]\n"
							, cur_dns_group->domain);
			cur_dns_group = cur_dns_group->next;
		}

		cur_server_group = cur_server_group->next;
	}

	return 0;
}


/* dns speed limit group header */
struct dns_speed_limit_group *gDNSSpeedLimitGrp = NULL;
/* dns speed limit mode. */
int gDNSSpeedLimitMode = DNS_SPEED_LIMIT_MODE_OFF;
/* dns speed limit action. */
int gDNSSpeedLimitAction = DNS_SPEED_LIMIT_ACTION_NONE;
/* dns speed limit all mode's limit count. */
int gDNSSpeedLimitAllModeCount = 0;
#define DNS_SPEED_LIMIT_REPORT "/tmp/dns_speed_limit_report"

/*
init dns speed limit data.
*/
int initDnsSpeedLimitGroup()
{
	char nodename_pvc[64] = {0};
	char domain[256] = {0}, limit_mode[12] = {0}, limit_action[32] = {0};
	char limit_cnt[12] = {0}, allperlimitcnt[12] = {0};
	int idx = 0, dnsbufLen = 0;
	struct dns_speed_limit_group **new_dns_speed_limit_group = NULL;

	new_dns_speed_limit_group = &gDNSSpeedLimitGrp;

	unlink(DNS_SPEED_LIMIT_REPORT);
	/*
	1. check limit range
	*/
	strcpy(nodename_pvc, "DnsSpeedLimit_Common");
	if ( 0 == 
		tcapi_get(nodename_pvc, "limitmode", limit_mode)
		&& 0 != limit_mode[0] )
	{
		if ( 0 == strcmp(limit_mode, "perdomain") )
			gDNSSpeedLimitMode = DNS_SPEED_LIMIT_MODE_PERDOMAIN;
		else if ( 0 == strcmp(limit_mode, "all") )
			gDNSSpeedLimitMode = DNS_SPEED_LIMIT_MODE_ALL;
	}

	/*
	2. check limit action
	*/
	if ( 0 == 
		tcapi_get(nodename_pvc, "limitaction", limit_action)
		&& 0 != limit_action[0] )
	{
		if ( 0 == strcmp(limit_action, "Alert") )
			gDNSSpeedLimitAction = DNS_SPEED_LIMIT_ACTION_ALERT;
		else if ( 0 == strcmp(limit_action, "Drop") )
			gDNSSpeedLimitAction = DNS_SPEED_LIMIT_ACTION_DROP;
	}

	if ( 0 == 
		tcapi_get(nodename_pvc, "allperlimitcnt", allperlimitcnt)
		&& 0 != allperlimitcnt[0] )
	{
		gDNSSpeedLimitAllModeCount = atoi(allperlimitcnt);
	}

	/* no need to init data when mode is off or limit all. */
	if ( DNS_SPEED_LIMIT_MODE_OFF == gDNSSpeedLimitMode
		|| DNS_SPEED_LIMIT_MODE_ALL == gDNSSpeedLimitMode
		|| DNS_SPEED_LIMIT_ACTION_NONE == gDNSSpeedLimitAction )
		goto DNS_LIMIT_DISP;

	/*
	3. init limit dns group.
	*/
	for ( idx = 0; idx < MAX_DNS_LIMIT_RULE; idx ++)
	{
		snprintf(nodename_pvc, sizeof(nodename_pvc) - 1
			, "DnsSpeedLimit_Entry%d", idx);

		/* get limit domain */
		if ( 0 == 
			tcapi_get(nodename_pvc, "domain", domain)
			&& 0 != domain[0] )
		{
			if ( !(*new_dns_speed_limit_group) )
				*new_dns_speed_limit_group =
				safe_malloc(sizeof(struct dns_speed_limit_group));

			/* malloc group failed. */
			if ( !(*new_dns_speed_limit_group) )
				return -1;

			(*new_dns_speed_limit_group)->domain = NULL;
			dnsbufLen = strlen(domain) + 1;
			(*new_dns_speed_limit_group)->domain = safe_malloc(dnsbufLen);

			/* malloc domain failed. */
			if ( !((*new_dns_speed_limit_group)->domain) )
			{
				free(*new_dns_speed_limit_group);
				return -1;
			}

			/* copy domain from node. */
			bzero((*new_dns_speed_limit_group)->domain, dnsbufLen);
			strncpy((*new_dns_speed_limit_group)->domain
				, domain, dnsbufLen);

			/* save limit count. */
			tcapi_get(nodename_pvc, "limitcnt", limit_cnt);
			(*new_dns_speed_limit_group)->limit_count = atoi(limit_cnt);
			if ( (*new_dns_speed_limit_group)->limit_count < 0 )
				(*new_dns_speed_limit_group)->limit_count = 0;
			/* init default value. */
			(*new_dns_speed_limit_group)->curr_count = 0;
			(*new_dns_speed_limit_group)->client_group = NULL;
			(*new_dns_speed_limit_group)->time = dnsmasq_time();

			(*new_dns_speed_limit_group)->next = NULL;
			new_dns_speed_limit_group = &((*new_dns_speed_limit_group)->next);
		}
	}

DNS_LIMIT_DISP:
	dispDNSSpeedLimitGroup();

	return 0;
}

/*
dns speed limit display.
*/
int dispDNSSpeedLimitGroup()
{
	struct dns_speed_limit_group *cur_limit_group = NULL;
	char showGroup[8] = {0};

	if( !(0 ==
		tcapi_get("WebCurSet_Entry", "dnsspeedlimitdisp", showGroup)
		&& 0 == strcmp("Yes", showGroup)) )
	{
		return 1;
	}

	switch ( gDNSSpeedLimitMode )
	{
		case DNS_SPEED_LIMIT_MODE_PERDOMAIN:
			tcdbg_printf("\ndns speed limit mode: per domain =>\n");
			break;
		case DNS_SPEED_LIMIT_MODE_ALL:
			tcdbg_printf("\ndns speed limit mode: all domain =>\n");
			break;
		case DNS_SPEED_LIMIT_MODE_OFF:
		default:
			tcdbg_printf("\ndns speed limit mode: off mode =>\n");
			return 1;
			break;
	}

	switch ( gDNSSpeedLimitAction )
	{
		case DNS_SPEED_LIMIT_ACTION_ALERT:
			tcdbg_printf("\ndns speed limit action: Alert =>\n");
			break;
		case DNS_SPEED_LIMIT_ACTION_DROP:
			tcdbg_printf("\ndns speed limit action: Drop =>\n");
			break;
		case DNS_SPEED_LIMIT_ACTION_NONE:
		default:
			tcdbg_printf("\ndns speed limit action: None =>\n");
			break;
	}

	for ( cur_limit_group = gDNSSpeedLimitGrp; cur_limit_group; )
	{
		tcdbg_printf("\ndns speed limit info =>\n");
		tcdbg_printf("dns=[%s], limit=[%d] \n"
			, cur_limit_group->domain
			, cur_limit_group->limit_count);

		cur_limit_group = cur_limit_group->next;
	}

	return 0;
}

/*
get MAC from arp entry
*/
int getHWAddrByIP
(uint8_t *hwaddr, int family, union mysockaddr *addr)
{
	int sd = 0, rc = 0;
	struct arpreq arpreq = {0};
	int idx = 0;
#ifdef HAVE_IPV6
#define NEIGH_FILE "/tmp/dnslimit_neigh"
	char neighcmd[] = "/usr/bin/ip -f inet6 neigh show dev br0 > "NEIGH_FILE;
	FILE *fp_neigh = NULL;
	struct all_addr laddrv6 = {0};
	char neigin_line[128] = {0};
	char ip6addr[64] = {0}, lladdr[24] = {0};
	char macaddr[24] = {0}, state[24] = {0};
	int mac_in[ETH_ALEN] = {0}, mac_found = 0;
#endif

	if ( AF_INET == family )
	{
		sd = socket(AF_INET, SOCK_DGRAM, 0);
	    if ( sd < 0 )
	    	return -1;

		bzero(&arpreq, sizeof(arpreq));
		memcpy(&arpreq.arp_pa, &(addr->in), sizeof(struct sockaddr_in));
		strcpy(arpreq.arp_dev, "br0");

		rc = ioctl(sd, SIOCGARP, &arpreq);
		if ( rc < 0 )
			return -2;

		memcpy(hwaddr, (unsigned char *) arpreq.arp_ha.sa_data, ETH_ALEN);

		return 0;
	}
#ifdef HAVE_IPV6
	else if ( AF_INET6 == family )
	{
		/* get v6 mac via ip neigh (or netlink method->complex) */
		system(neighcmd);
		
		
		fp_neigh = fopen(NEIGH_FILE, "r");
		if ( !fp_neigh )
			return -6;

		while ( fgets(neigin_line, sizeof(neigin_line), fp_neigh) )
		{
			if( 0 == neigin_line[0]
				|| strstr(neigin_line, "router") )
				continue;

			if (4 == sscanf(neigin_line, "%s %s %s %s"
				, ip6addr, lladdr, macaddr, state)
				&& 1 == inet_pton(AF_INET6, ip6addr, &laddrv6)
				&& 0 == memcmp(&laddrv6
						, &(addr->in6.sin6_addr)
						, sizeof(struct in6_addr)))
			{
				sscanf(macaddr, "%02x:%02x:%02x:%02x:%02x:%02x"
					, &mac_in[0], &mac_in[1], &mac_in[2]
					, &mac_in[3], &mac_in[4], &mac_in[5]);
				for ( idx = 0; idx < ETH_ALEN; idx ++ )
					hwaddr[idx] = (uint8_t)mac_in[idx];

				mac_found = 1;
				break;
			}
		}
		fclose(fp_neigh);
		unlink(NEIGH_FILE);

		if ( mac_found )
			return 0;
	}
#endif

	return -9;
}

/*
check local querny
*/
int isLocal(int family, union mysockaddr *addr)
{
	unsigned int local_h = 0x7f000001;

	if ( AF_INET == family
		&& local_h == addr->in.sin_addr.s_addr )
	{
		return 1;
	}

	return 0;
}

/*
check dns speed limit for per domain.
*/
int checkDNS_SpeedLimitPerDomain
(char *domain, int family, union mysockaddr *addr)
{
	struct dns_speed_limit_group *cur_limit_group = NULL;
	struct dns_speed_limit_client_group *cur_client_group = NULL;
	struct dns_speed_limit_client_group *new_client_group = NULL;
	struct dns_speed_limit_client_group *free_client_group = NULL;
	int isExist = 0;
	unsigned char hw_addr[ETH_ALEN];

	time_t now = dnsmasq_time();

	if ( isLocal(family, addr) )
		return DNS_SPEED_LIMIT_ACTION_NONE;

	for ( cur_limit_group = gDNSSpeedLimitGrp; cur_limit_group; )
	{
		if ( NULL != strstr(domain, cur_limit_group->domain) )
		{
			/* check timeout */
			if ( difftime(now, cur_limit_group->time) >= 60 )
			{
				/* free all client */
				for ( cur_client_group = cur_limit_group->client_group;
						cur_client_group; )
				{
					free_client_group = cur_client_group;
					cur_client_group = cur_client_group->next;

					free(free_client_group);
					free_client_group = NULL;
				}
				cur_limit_group->client_group = NULL;
				cur_limit_group->curr_count = 0;
				cur_limit_group->time = dnsmasq_time();
			}

			/* get mac address via ip */
			bzero(hw_addr, sizeof(hw_addr));
			if ( 0 != getHWAddrByIP(hw_addr, family, addr) )
				return DNS_SPEED_LIMIT_ACTION_NONE;

			/* search client */
			for ( cur_client_group = cur_limit_group->client_group;
					cur_client_group; )
			{
				if ( 0 == memcmp(hw_addr, cur_client_group->hwaddr, ETH_ALEN) )
				{
					isExist = 1;

					/* IPv4 */
					if ( AF_INET == family )
					{
						memcpy(&(cur_client_group->addrv4)
							, addr, sizeof(union mysockaddr));

						if ( DNS_SPEED_LIMIT_HOST_IPV4
							== cur_client_group->ipver 
							|| DNS_SPEED_LIMIT_HOST_UNKNOW
							== cur_client_group->ipver )
							cur_client_group->ipver = DNS_SPEED_LIMIT_HOST_IPV4;
						else
							cur_client_group->ipver = DNS_SPEED_LIMIT_HOST_BOTH;
					}
#ifdef HAVE_IPV6
					/* IPv6 */
					else if ( AF_INET6 == family )
					{
						memcpy(&(cur_client_group->addrv6)
							, addr, sizeof(union mysockaddr));

						if ( DNS_SPEED_LIMIT_HOST_IPV6
							== cur_client_group->ipver 
							|| DNS_SPEED_LIMIT_HOST_UNKNOW
							== cur_client_group->ipver )
							cur_client_group->ipver = DNS_SPEED_LIMIT_HOST_IPV6;
						else
							cur_client_group->ipver = DNS_SPEED_LIMIT_HOST_BOTH;
					}
#endif

					if ( cur_client_group->grp_index > 
						cur_limit_group->limit_count )
						return gDNSSpeedLimitAction;

					break;
				}

				cur_client_group = cur_client_group->next;
			}

			/* need add it */
			if ( 0 == isExist )
			{
				if ( cur_limit_group->curr_count >= MAX_DNS_LIMIT_CLIENTS )
					return gDNSSpeedLimitAction;

				new_client_group = 
					safe_malloc(sizeof(struct dns_speed_limit_client_group));
				if ( !new_client_group )
					return DNS_SPEED_LIMIT_ACTION_NONE;

				memcpy(new_client_group->hwaddr, hw_addr, ETH_ALEN);
				/* IPv4 */
				if ( AF_INET == family )
				{
					memcpy(&(new_client_group->addrv4)
						, addr, sizeof(union mysockaddr));

					new_client_group->ipver = DNS_SPEED_LIMIT_HOST_IPV4;
				}
#ifdef HAVE_IPV6
				/* IPv6 */
				else if ( AF_INET6 == family )
				{
					memcpy(&(new_client_group->addrv6)
						, addr, sizeof(union mysockaddr));
				
					new_client_group->ipver = DNS_SPEED_LIMIT_HOST_IPV6;
				}
#endif

				new_client_group->grp_index = (++cur_limit_group->curr_count);
				new_client_group->next = cur_limit_group->client_group;
				cur_limit_group->client_group = new_client_group;

				if ( new_client_group->grp_index >
					cur_limit_group->limit_count )
					return gDNSSpeedLimitAction;
			}

			break;
		}
	
		cur_limit_group = cur_limit_group->next;
	}

	return DNS_SPEED_LIMIT_ACTION_NONE;
}

/*
check dns speed limit
*/
int checkDNS_SpeedLimitAll
(char *domain, int family, union mysockaddr *addr)
{
	struct dns_speed_limit_group *cur_limit_group = NULL;
	struct dns_speed_limit_group *new_dns_speed_limit_group = NULL;
	int isFound = 0, totalGrp = 0, dnsbufLen = 0;

	if ( isLocal(family, addr) )
		return DNS_SPEED_LIMIT_ACTION_NONE;

	for ( cur_limit_group = gDNSSpeedLimitGrp; cur_limit_group; )
	{
		totalGrp++;
		if ( 0 == strcmp(domain, cur_limit_group->domain) )
		{
			isFound = 1;
			break;
		}
		cur_limit_group = cur_limit_group->next;
	}

	if ( 0 == isFound
		&& totalGrp < MAX_DNS_LIMIT_RULE )
	{
		new_dns_speed_limit_group = 
			safe_malloc(sizeof(struct dns_speed_limit_group));

		/* malloc group failed. */
		if ( !new_dns_speed_limit_group )
			return DNS_SPEED_LIMIT_ACTION_NONE;

		new_dns_speed_limit_group->domain = NULL;
		dnsbufLen = strlen(domain) + 1;
		new_dns_speed_limit_group->domain = safe_malloc(dnsbufLen);

		/* malloc domain failed. */
		if ( !(new_dns_speed_limit_group->domain) )
		{
			free(new_dns_speed_limit_group);
			return DNS_SPEED_LIMIT_ACTION_NONE;
		}

		/* copy domain from node. */
		bzero(new_dns_speed_limit_group->domain, dnsbufLen);
		strncpy(new_dns_speed_limit_group->domain
			, domain, dnsbufLen);

		/* save limit count. */
		new_dns_speed_limit_group->limit_count = gDNSSpeedLimitAllModeCount;
		if ( new_dns_speed_limit_group->limit_count < 0 )
			new_dns_speed_limit_group->limit_count = 0;
		/* init default value. */
		new_dns_speed_limit_group->curr_count = 0;
		new_dns_speed_limit_group->client_group = NULL;
		new_dns_speed_limit_group->time = dnsmasq_time();

		new_dns_speed_limit_group->next = gDNSSpeedLimitGrp;
		gDNSSpeedLimitGrp = new_dns_speed_limit_group;
	}

	return checkDNS_SpeedLimitPerDomain(domain, family, addr);
}

/*
check report is ok
*/
#define DNS_SPEED_LIMIT_INFORMCHECK "/tmp/dns_speed_limit_inform_check"
int checkCwmpReport()
{
	FILE *fp = NULL;

	fp = fopen(DNS_SPEED_LIMIT_INFORMCHECK, "r");
	if ( !fp )
		return 0;

	fclose(fp);
	return 1;
}

/*
report limit host to file
*/
int writeDeviceInfoReport()
{
	FILE *fp = NULL;
	unsigned char *p_addr = NULL;
	int isFirst = 1;
	char ip4addr[64] = {0}, ip6addr[64] = {0};
	struct dns_speed_limit_group *cur_limit_group = NULL;
	struct dns_speed_limit_client_group *cur_client_group = NULL;

	fp = fopen(DNS_SPEED_LIMIT_REPORT, "w");
	if ( !fp )
		return -1;

	for ( cur_limit_group = gDNSSpeedLimitGrp; cur_limit_group; )
	{
		if ( cur_limit_group->curr_count > 
						cur_limit_group->limit_count )
		{
			/* search client */
			for ( cur_client_group = cur_limit_group->client_group;
					cur_client_group; )
			{
				if ( DNS_SPEED_LIMIT_HOST_IPV4 ==
						cur_client_group->ipver
					|| DNS_SPEED_LIMIT_HOST_BOTH ==
						cur_client_group->ipver )
				{
					inet_ntop(AF_INET, &(cur_client_group->addrv4.in.sin_addr)
						, ip4addr, sizeof(ip4addr));
					p_addr = cur_client_group->hwaddr;
					fprintf(fp, "%s%s/%s/%02X:%02X:%02X:%02X:%02X:%02X"
						, (isFirst ? "" : ",")
						, cur_limit_group->domain
						, ip4addr
						, p_addr[0], p_addr[1], p_addr[2]
						, p_addr[3], p_addr[4], p_addr[5]);
					isFirst = 0;
				}

				if ( DNS_SPEED_LIMIT_HOST_IPV6 ==
						cur_client_group->ipver
						|| DNS_SPEED_LIMIT_HOST_BOTH ==
						cur_client_group->ipver )
				{
					inet_ntop(AF_INET6, &(cur_client_group->addrv6.in6.sin6_addr)
						, ip6addr, sizeof(ip6addr));
					p_addr = cur_client_group->hwaddr;
					fprintf(fp, "%s%s/%s/%02X:%02X:%02X:%02X:%02X:%02X"
						, (isFirst ? "" : ",")
						, cur_limit_group->domain
						, ip6addr
						, p_addr[0], p_addr[1], p_addr[2]
						, p_addr[3], p_addr[4], p_addr[5]);
					isFirst = 0;
				}

				cur_client_group = cur_client_group->next;
			}
		}

		cur_limit_group = cur_limit_group->next;
	}

	fflush(fp);
	fclose(fp);

	return 0;
}



/*
check dns speed limit
*/
int checkDNS_SpeedLimit
(char *domain, int family, union mysockaddr *addr)
{
	int ret = DNS_SPEED_LIMIT_ACTION_NONE;
	cwmp_msg_t message;
#define DNS_LIMIT_ALERT 21

	/* dns speed limit off. */
	if ( DNS_SPEED_LIMIT_MODE_OFF == gDNSSpeedLimitMode )
		return DNS_SPEED_LIMIT_ACTION_NONE;
	/* dns speed limit for all domain. */
	else if ( DNS_SPEED_LIMIT_MODE_ALL == gDNSSpeedLimitMode )
		ret = checkDNS_SpeedLimitAll(domain, family, addr);
	/* dns speed limit for per domain. */
	else if ( DNS_SPEED_LIMIT_MODE_PERDOMAIN == gDNSSpeedLimitMode )
		ret = checkDNS_SpeedLimitPerDomain(domain, family, addr);

	if ( DNS_SPEED_LIMIT_ACTION_ALERT == ret )
	{
		if ( 0 == checkCwmpReport() )
		{
			writeDeviceInfoReport();

			memset(&message, 0, sizeof(message));
			message.cwmptype = DNS_LIMIT_ALERT;
			if ( sendmegq(1, &message, 0) < 0 )
				tcdbg_printf("\nsend message to cwmp error!\n");
			else
				doValPut(DNS_SPEED_LIMIT_INFORMCHECK, "1");
		}
	}
	else if ( DNS_SPEED_LIMIT_ACTION_DROP == ret )
	{
		writeDeviceInfoReport();
	}

	return ret;
}


#endif



int main (int argc, char **argv)
{
  int bind_fallback = 0;
  time_t now;
  struct sigaction sigact;
  struct iname *if_tmp;
  int piperead, pipefd[2], err_pipe[2];
  struct passwd *ent_pw = NULL;
#if defined(HAVE_DHCP) && defined(HAVE_SCRIPT)
  uid_t script_uid = 0;
  gid_t script_gid = 0;
#endif
  struct group *gp = NULL;
  long i, max_fd = sysconf(_SC_OPEN_MAX);
  char *baduser = NULL;
  int log_err;
#if defined(HAVE_LINUX_NETWORK)
  cap_user_header_t hdr = NULL;
  cap_user_data_t data = NULL;
#endif 
#if defined(TCSUPPORT_CT_ADV_DNSPROXY)
  int retcd = 0;
  int has_frec_pkt = 0;
#endif

#ifdef LOCALEDIR
  setlocale(LC_ALL, "");
  bindtextdomain("dnsmasq", LOCALEDIR); 
  textdomain("dnsmasq");
#endif
//#ifdef TCSUPPORT_DNSEACHPVC
#if defined(TCSUPPORT_DNSEACHPVC) || defined(TCSUPPORT_TR69_BIND_PVC)
gDNS_Param.pHead = (struct DNS_Info*)malloc(sizeof(struct DNS_Info));
  if(!gDNS_Param.pHead)
  {
  	printf("\n malloc head node failed.");
	exit(1);
  }
  memset(gDNS_Param.pHead, 0, sizeof(struct DNS_Info));
  gDNS_Param.pTail =  gDNS_Param.pHead;
  gDNS_Param.pHead->next = NULL;
#endif




  sigact.sa_handler = sig_handler;
  sigact.sa_flags = 0;
  sigemptyset(&sigact.sa_mask);
  sigaction(SIGUSR1, &sigact, NULL);
  sigaction(SIGUSR2, &sigact, NULL);
  sigaction(SIGHUP, &sigact, NULL);
  sigaction(SIGTERM, &sigact, NULL);
  sigaction(SIGALRM, &sigact, NULL);
  sigaction(SIGCHLD, &sigact, NULL);

  /* ignore SIGPIPE */
  sigact.sa_handler = SIG_IGN;
  sigaction(SIGPIPE, &sigact, NULL);

  umask(022); /* known umask, create leases and pid files as 0644 */

  read_opts(argc, argv, compile_opts);
    
  if (daemon->edns_pktsz < PACKETSZ)
    daemon->edns_pktsz = PACKETSZ;
  daemon->packet_buff_sz = daemon->edns_pktsz > DNSMASQ_PACKETSZ ? 
    daemon->edns_pktsz : DNSMASQ_PACKETSZ;
  daemon->packet = safe_malloc(daemon->packet_buff_sz);

#ifdef HAVE_DHCP
  if (!daemon->lease_file)
    {
      if (daemon->dhcp)
	daemon->lease_file = LEASEFILE;
    }
#endif
  
  /* Close any file descriptors we inherited apart from std{in|out|err} */
  for (i = 0; i < max_fd; i++)
    if (i != STDOUT_FILENO && i != STDERR_FILENO && i != STDIN_FILENO)
      close(i);

#ifdef HAVE_LINUX_NETWORK
  netlink_init();
#elif !(defined(IP_RECVDSTADDR) && \
	defined(IP_RECVIF) && \
	defined(IP_SENDSRCADDR))
  if (!(daemon->options & OPT_NOWILD))
    {
      bind_fallback = 1;
      daemon->options |= OPT_NOWILD;
    }
#endif

#ifndef HAVE_TFTP
  if (daemon->options & OPT_TFTP)
    die(_("TFTP server not available: set HAVE_TFTP in src/config.h"), NULL, EC_BADCONF);
#endif

#ifdef HAVE_SOLARIS_NETWORK
  if (daemon->max_logs != 0)
    die(_("asychronous logging is not available under Solaris"), NULL, EC_BADCONF);
#endif
  
  rand_init();
  
  now = dnsmasq_time();
  
#ifdef HAVE_DHCP
  if (daemon->dhcp)
    {
      /* Note that order matters here, we must call lease_init before
	 creating any file descriptors which shouldn't be leaked
	 to the lease-script init process. */
      lease_init(now);
      dhcp_init();
    }
#endif

  if (!enumerate_interfaces())
    die(_("failed to find list of interfaces: %s"), NULL, EC_MISC);
    
  if (daemon->options & OPT_NOWILD) 
    {
      daemon->listeners = create_bound_listeners();

      for (if_tmp = daemon->if_names; if_tmp; if_tmp = if_tmp->next)
	if (if_tmp->name && !if_tmp->used)
	  die(_("unknown interface %s"), if_tmp->name, EC_BADNET);
  
      for (if_tmp = daemon->if_addrs; if_tmp; if_tmp = if_tmp->next)
	if (!if_tmp->used)
	  {
	    prettyprint_addr(&if_tmp->addr, daemon->namebuff);
	    die(_("no interface with address %s"), daemon->namebuff, EC_BADNET);
	  }
    }
  else if ((daemon->port != 0 || (daemon->options & OPT_TFTP)) &&
	   !(daemon->listeners = create_wildcard_listeners()))
    die(_("failed to create listening socket: %s"), NULL, EC_BADNET);
  
  if (daemon->port != 0)
    cache_init();
    
  if (daemon->options & OPT_DBUS)
#ifdef HAVE_DBUS
    {
      char *err;
      daemon->dbus = NULL;
      daemon->watches = NULL;
      if ((err = dbus_init()))
	die(_("DBus error: %s"), err, EC_MISC);
    }
#else
  die(_("DBus not available: set HAVE_DBUS in src/config.h"), NULL, EC_BADCONF);
#endif
  
  if (daemon->port != 0)
    pre_allocate_sfds();

#if defined(HAVE_DHCP) && defined(HAVE_SCRIPT)
  /* Note getpwnam returns static storage */
  if (daemon->dhcp && daemon->lease_change_command && daemon->scriptuser)
    {
      if ((ent_pw = getpwnam(daemon->scriptuser)))
	{
	  script_uid = ent_pw->pw_uid;
	  script_gid = ent_pw->pw_gid;
	 }
      else
	baduser = daemon->scriptuser;
    }
#endif
  
  if (daemon->username && !(ent_pw = getpwnam(daemon->username)))
    baduser = daemon->username;
  else if (daemon->groupname && !(gp = getgrnam(daemon->groupname)))
    baduser = daemon->groupname;

  if (baduser)
    //die(_("unknown user or group: %s"), baduser, EC_BADCONF);
   
  /* implement group defaults, "dip" if available, or group associated with uid */
  if (!daemon->group_set && !gp)
    {
      if (!(gp = getgrnam(CHGRP)) && ent_pw)
	gp = getgrgid(ent_pw->pw_gid);
      
      /* for error message */
      if (gp)
	daemon->groupname = gp->gr_name; 
    }

#if defined(HAVE_LINUX_NETWORK)
  /* determine capability API version here, while we can still
     call safe_malloc */
  if (ent_pw && ent_pw->pw_uid != 0)
    {
      int capsize = 1; /* for header version 1 */
      hdr = safe_malloc(sizeof(*hdr));

      /* find version supported by kernel */
      memset(hdr, 0, sizeof(*hdr));
      capget(hdr, NULL);
      
      if (hdr->version != LINUX_CAPABILITY_VERSION_1)
	{
	  /* if unknown version, use largest supported version (3) */
	  if (hdr->version != LINUX_CAPABILITY_VERSION_2)
	    hdr->version = LINUX_CAPABILITY_VERSION_3;
	  capsize = 2;
	}
      
      data = safe_malloc(sizeof(*data) * capsize);
      memset(data, 0, sizeof(*data) * capsize);
    }
#endif

  /* Use a pipe to carry signals and other events back to the event loop 
     in a race-free manner and another to carry errors to daemon-invoking process */
  safe_pipe(pipefd, 1);
  
  piperead = pipefd[0];
  pipewrite = pipefd[1];
  /* prime the pipe to load stuff first time. */
  send_event(pipewrite, EVENT_RELOAD, 0); 

  err_pipe[1] = -1;
  
  if (!(daemon->options & OPT_DEBUG))   
    {
      int nullfd;

      /* The following code "daemonizes" the process. 
	 See Stevens section 12.4 */
      
      if (chdir("/") != 0)
	die(_("cannot chdir to filesystem root: %s"), NULL, EC_MISC); 

#ifndef NO_FORK      
      if (!(daemon->options & OPT_NO_FORK))
	{
	  pid_t pid;
	  
	  /* pipe to carry errors back to original process.
	     When startup is complete we close this and the process terminates. */
	  safe_pipe(err_pipe, 0);
	  
	  if ((pid = fork()) == -1)
	    /* fd == -1 since we've not forked, never returns. */
	    send_event(-1, EVENT_FORK_ERR, errno);
	   
	  if (pid != 0)
	    {
	      struct event_desc ev;
	      
	      /* close our copy of write-end */
	      close(err_pipe[1]);
	      
	      /* check for errors after the fork */
	      if (read_write(err_pipe[0], (unsigned char *)&ev, sizeof(ev), 1))
		fatal_event(&ev);
	      
	      _exit(EC_GOOD);
	    } 
	  
	  close(err_pipe[0]);

	  /* NO calls to die() from here on. */
	  
	  setsid();
	 
	  if ((pid = fork()) == -1)
	    send_event(err_pipe[1], EVENT_FORK_ERR, errno);
	 
	  if (pid != 0)
	    _exit(0);
	}
#endif
            
      /* write pidfile _after_ forking ! */
      if (daemon->runfile)
	{
	  FILE *pidfile;
	  
	  /* only complain if started as root */
	  if ((pidfile = fopen(daemon->runfile, "w")))
	    {
	      fprintf(pidfile, "%d\n", (int) getpid());
	      fclose(pidfile);
	    }
	  else if (getuid() == 0)
	    {
	      send_event(err_pipe[1], EVENT_PIDFILE, errno);
	      _exit(0);
	    }
	}
         
      /* open  stdout etc to /dev/null */
      nullfd = open("/dev/null", O_RDWR);
      dup2(nullfd, STDOUT_FILENO);
      dup2(nullfd, STDERR_FILENO);
      dup2(nullfd, STDIN_FILENO);
      close(nullfd);
    }
  
   log_err = log_start(ent_pw, err_pipe[1]); 
   
   /* if we are to run scripts, we need to fork a helper before dropping root. */
  daemon->helperfd = -1;
#if defined(HAVE_DHCP) && defined(HAVE_SCRIPT) 
  if (daemon->dhcp && daemon->lease_change_command)
    daemon->helperfd = create_helper(pipewrite, err_pipe[1], script_uid, script_gid, max_fd);
#endif

  if (!(daemon->options & OPT_DEBUG) && getuid() == 0)   
    {
      int bad_capabilities = 0;
      gid_t dummy;
      
      /* remove all supplimentary groups */
      if (gp && 
	  (setgroups(0, &dummy) == -1 ||
	   setgid(gp->gr_gid) == -1))
	{
	  send_event(err_pipe[1], EVENT_GROUP_ERR, errno);
	  _exit(0);
	}
  
      if (ent_pw && ent_pw->pw_uid != 0)
	{     
#if defined(HAVE_LINUX_NETWORK)
	  /* On linux, we keep CAP_NETADMIN (for ARP-injection) and
	     CAP_NET_RAW (for icmp) if we're doing dhcp */
	  data->effective = data->permitted = data->inheritable =
	    (1 << CAP_NET_ADMIN) | (1 << CAP_NET_RAW) | (1 << CAP_SETUID);
	  
	  /* Tell kernel to not clear capabilities when dropping root */
	  if (capset(hdr, data) == -1 || prctl(PR_SET_KEEPCAPS, 1) == -1)
	    bad_capabilities = errno;
			  
#elif defined(HAVE_SOLARIS_NETWORK)
	  /* http://developers.sun.com/solaris/articles/program_privileges.html */
	  priv_set_t *priv_set;
	  
	  if (!(priv_set = priv_str_to_set("basic", ",", NULL)) ||
	      priv_addset(priv_set, PRIV_NET_ICMPACCESS) == -1 ||
	      priv_addset(priv_set, PRIV_SYS_NET_CONFIG) == -1)
	    bad_capabilities = errno;

	  if (priv_set && bad_capabilities == 0)
	    {
	      priv_inverse(priv_set);
	  
	      if (setppriv(PRIV_OFF, PRIV_LIMIT, priv_set) == -1)
		bad_capabilities = errno;
	    }

	  if (priv_set)
	    priv_freeset(priv_set);

#endif    

	  if (bad_capabilities != 0)
	    {
	      send_event(err_pipe[1], EVENT_CAP_ERR, bad_capabilities);
	      _exit(0);
	    }
	  
	  /* finally drop root */
	  if (setuid(ent_pw->pw_uid) == -1)
	    {
	      send_event(err_pipe[1], EVENT_USER_ERR, errno);
	      _exit(0);
	    }     

#ifdef HAVE_LINUX_NETWORK
	  data->effective = data->permitted = 
	    (1 << CAP_NET_ADMIN) | (1 << CAP_NET_RAW);
	  data->inheritable = 0;
	  
	  /* lose the setuid and setgid capbilities */
	  if (capset(hdr, data) == -1)
	    {
	      send_event(err_pipe[1], EVENT_CAP_ERR, errno);
	      _exit(0);
	    }
#endif
	  
	}
    }
  
#ifdef HAVE_LINUX_NETWORK
  if (daemon->options & OPT_DEBUG) 
    prctl(PR_SET_DUMPABLE, 1);
#endif

  if (daemon->port == 0)
    my_syslog(LOG_INFO, _("started, version %s DNS disabled"), VERSION);
  else if (daemon->cachesize != 0)
    my_syslog(LOG_INFO, _("started, version %s cachesize %d"), VERSION, daemon->cachesize);
  else
    my_syslog(LOG_INFO, _("started, version %s cache disabled"), VERSION);
  
  my_syslog(LOG_INFO, _("compile time options: %s"), compile_opts);
  
#ifdef HAVE_DBUS
  if (daemon->options & OPT_DBUS)
    {
      if (daemon->dbus)
	my_syslog(LOG_INFO, _("DBus support enabled: connected to system bus"));
      else
	my_syslog(LOG_INFO, _("DBus support enabled: bus connection pending"));
    }
#endif

  if (log_err != 0)
    my_syslog(LOG_WARNING, _("warning: failed to change owner of %s: %s"), 
	      daemon->log_file, strerror(log_err));

  if (bind_fallback)
    my_syslog(LOG_WARNING, _("setting --bind-interfaces option because of OS limitations"));
  
  if (!(daemon->options & OPT_NOWILD)) 
    for (if_tmp = daemon->if_names; if_tmp; if_tmp = if_tmp->next)
      if (if_tmp->name && !if_tmp->used)
	my_syslog(LOG_WARNING, _("warning: interface %s does not currently exist"), if_tmp->name);
   
  if (daemon->port != 0 && (daemon->options & OPT_NO_RESOLV))
    {
      if (daemon->resolv_files && !daemon->resolv_files->is_default)
	my_syslog(LOG_WARNING, _("warning: ignoring resolv-file flag because no-resolv is set"));
      daemon->resolv_files = NULL;
      if (!daemon->servers)
	my_syslog(LOG_WARNING, _("warning: no upstream servers configured"));
    } 

  if (daemon->max_logs != 0)
    my_syslog(LOG_INFO, _("asynchronous logging enabled, queue limit is %d messages"), daemon->max_logs);

#ifdef HAVE_DHCP
  if (daemon->dhcp)
    {
      struct dhcp_context *dhcp_tmp;
      
      for (dhcp_tmp = daemon->dhcp; dhcp_tmp; dhcp_tmp = dhcp_tmp->next)
	{
	  prettyprint_time(daemon->dhcp_buff2, dhcp_tmp->lease_time);
	  strcpy(daemon->dhcp_buff, inet_ntoa(dhcp_tmp->start));
	  my_syslog(MS_DHCP | LOG_INFO, 
		    (dhcp_tmp->flags & CONTEXT_STATIC) ? 
		    _("DHCP, static leases only on %.0s%s, lease time %s") :
		    (dhcp_tmp->flags & CONTEXT_PROXY) ?
		    _("DHCP, proxy on subnet %.0s%s%.0s") :
		    _("DHCP, IP range %s -- %s, lease time %s"),
		    daemon->dhcp_buff, inet_ntoa(dhcp_tmp->end), daemon->dhcp_buff2);
	}
    }
#endif

#ifdef HAVE_TFTP
  if (daemon->options & OPT_TFTP)
    {
#ifdef FD_SETSIZE
      if (FD_SETSIZE < (unsigned)max_fd)
	max_fd = FD_SETSIZE;
#endif

      my_syslog(MS_TFTP | LOG_INFO, "TFTP %s%s %s", 
		daemon->tftp_prefix ? _("root is ") : _("enabled"),
		daemon->tftp_prefix ? daemon->tftp_prefix: "",
		daemon->options & OPT_TFTP_SECURE ? _("secure mode") : "");
      
      /* This is a guess, it assumes that for small limits, 
	 disjoint files might be served, but for large limits, 
	 a single file will be sent to may clients (the file only needs
	 one fd). */

      max_fd -= 30; /* use other than TFTP */
      
      if (max_fd < 0)
	max_fd = 5;
      else if (max_fd < 100)
	max_fd = max_fd/2;
      else
	max_fd = max_fd - 20;
      
      /* if we have to use a limited range of ports, 
	 that will limit the number of transfers */
      if (daemon->start_tftp_port != 0 &&
	  daemon->end_tftp_port - daemon->start_tftp_port + 1 < max_fd)
	max_fd = daemon->end_tftp_port - daemon->start_tftp_port + 1;

      if (daemon->tftp_max > max_fd)
	{
	  daemon->tftp_max = max_fd;
	  my_syslog(MS_TFTP | LOG_WARNING, 
		    _("restricting maximum simultaneous TFTP transfers to %d"), 
		    daemon->tftp_max);
	}
    }
#endif

  /* finished start-up - release original process */
  if (err_pipe[1] != -1)
    close(err_pipe[1]);
  
  if (daemon->port != 0)
    check_servers();
  
  pid = getpid();
  
  //#ifdef TCSUPPORT_DNSEACHPVC
#if defined(TCSUPPORT_DNSEACHPVC) || defined(TCSUPPORT_TR69_BIND_PVC)
  //resolve server.conf
  resolve_dnsServerConf();
  //printfList(); //for debug
  #endif

#if defined(TCSUPPORT_CT_DNSBIND)
  //resolve url list
  fetchURLList();
#endif
#ifdef CT_COM_DEVICEREG 
  devreg = checkDevRegisterFlag();
#endif
#if defined(TCSUPPORT_CUC_LANDING_PAGE)
  ponstate = checkPonState();
  if ( 1 == ponstate )
	system("rm -f /etc/hosts");
#endif
#if defined(TCSUPPORT_CT_PHONEAPP)
	initDNSRedirectInfo();
#endif
#if defined(TCSUPPORT_CT_JOYME)
	pushViewstate = checkPushViewState();
	if ( 0 == pushViewstate )
		system("rm -f /etc/hosts");

	initAttachWanDNSGroup();
	initDnsSpeedLimitGroup();
#endif
#if defined(TCSUPPORT_CT_L2TP_VPN)
	initVPNDnsGroup();
#endif

  while (1)
    {
      int maxfd = -1;
      struct timeval t, *tp = NULL;
      fd_set rset, wset, eset;
      
      FD_ZERO(&rset);
      FD_ZERO(&wset);
      FD_ZERO(&eset);
      
      /* if we are out of resources, find how long we have to wait
	 for some to come free, we'll loop around then and restart
	 listening for queries */
      if ((t.tv_sec = set_dns_listeners(now, &rset, &maxfd)) != 0)
	{
	  t.tv_usec = 0;
	  tp = &t;
	}

      /* Whilst polling for the dbus, or doing a tftp transfer, wake every quarter second */
      if (daemon->tftp_trans ||
	  ((daemon->options & OPT_DBUS) && !daemon->dbus))
	{
	  t.tv_sec = 0;
	  t.tv_usec = 250000;
	  tp = &t;
	}

#if defined(TCSUPPORT_CT_ADV_DNSPROXY)
	if ( 1 == ( has_frec_pkt = is_exist_frec_packets()) )
	{
		t.tv_sec = 1;
		t.tv_usec = 0;
		tp = &t;
	}
#endif

#ifdef HAVE_DBUS
      set_dbus_listeners(&maxfd, &rset, &wset, &eset);
#endif	
  
#ifdef HAVE_DHCP
      if (daemon->dhcp)
	{
	  FD_SET(daemon->dhcpfd, &rset);
	  bump_maxfd(daemon->dhcpfd, &maxfd);
	  if (daemon->pxefd != -1)
	    {
	      FD_SET(daemon->pxefd, &rset);
	      bump_maxfd(daemon->pxefd, &maxfd);
	    }
	}
#endif

#ifdef HAVE_LINUX_NETWORK
      FD_SET(daemon->netlinkfd, &rset);
      bump_maxfd(daemon->netlinkfd, &maxfd);
#endif
      
      FD_SET(piperead, &rset);
      bump_maxfd(piperead, &maxfd);

#ifdef HAVE_DHCP
#  ifdef HAVE_SCRIPT
      while (helper_buf_empty() && do_script_run(now));

      if (!helper_buf_empty())
	{
	  FD_SET(daemon->helperfd, &wset);
	  bump_maxfd(daemon->helperfd, &maxfd);
	}
#  else
      /* need this for other side-effects */
      while (do_script_run(now));
#  endif
#endif
   
      /* must do this just before select(), when we know no
	 more calls to my_syslog() can occur */
      set_log_writer(&wset, &maxfd);
      
#if defined(TCSUPPORT_CT_ADV_DNSPROXY)
      if ( ( retcd = select(maxfd+1, &rset, &wset, &eset, tp) ) < 0)
      {
	  	/* otherwise undefined after error */
	  	FD_ZERO(&rset); FD_ZERO(&wset); FD_ZERO(&eset);
      }
#else
      if (select(maxfd+1, &rset, &wset, &eset, tp) < 0)
	{
	  /* otherwise undefined after error */
	  FD_ZERO(&rset); FD_ZERO(&wset); FD_ZERO(&eset);
	}

#endif

      now = dnsmasq_time();

      check_log_writer(&wset);

      /* Check for changes to resolv files once per second max. */
      /* Don't go silent for long periods if the clock goes backwards. */
      if (daemon->last_resolv == 0 || 
	  difftime(now, daemon->last_resolv) > 1.0 || 
	  difftime(now, daemon->last_resolv) < -1.0)
	{
	  daemon->last_resolv = now;

	  if (daemon->port != 0 && !(daemon->options & OPT_NO_POLL))
	    poll_resolv();
	}
      
      if (FD_ISSET(piperead, &rset))
	async_event(piperead, now);
      
#ifdef HAVE_LINUX_NETWORK
      if (FD_ISSET(daemon->netlinkfd, &rset))
	netlink_multicast();
#endif
      
#ifdef HAVE_DBUS
      /* if we didn't create a DBus connection, retry now. */ 
     if ((daemon->options & OPT_DBUS) && !daemon->dbus)
	{
	  char *err;
	  if ((err = dbus_init()))
	    my_syslog(LOG_WARNING, _("DBus error: %s"), err);
	  if (daemon->dbus)
	    my_syslog(LOG_INFO, _("connected to system DBus"));
	}
      check_dbus_listeners(&rset, &wset, &eset);
#endif

#if defined(TCSUPPORT_CT_ADV_DNSPROXY)
	  if ( 1 == has_frec_pkt )
	  {
		  if ( 0 == retcd ) // timeout
	      {
			  check_frec_packets();
			  continue;
	      }
	      else
		 	  check_frec_packets_recvmod(now);
	  }
#endif

      check_dns_listeners(&rset, now);

#ifdef HAVE_TFTP
      check_tftp_listeners(&rset, now);
#endif      

#ifdef HAVE_DHCP
      if (daemon->dhcp)
	{
	  if (FD_ISSET(daemon->dhcpfd, &rset))
	    dhcp_packet(now, 0);
	  if (daemon->pxefd != -1 && FD_ISSET(daemon->pxefd, &rset))
	    dhcp_packet(now, 1);
	}

#  ifdef HAVE_SCRIPT
      if (daemon->helperfd != -1 && FD_ISSET(daemon->helperfd, &wset))
	helper_write();
#  endif
#endif

    }
}

static void sig_handler(int sig)
{
  if (pid == 0)
    {
      /* ignore anything other than TERM during startup
	 and in helper proc. (helper ignore TERM too) */
      if (sig == SIGTERM)
	exit(EC_MISC);
    }
  else if (pid != getpid())
    {
      /* alarm is used to kill TCP children after a fixed time. */
      if (sig == SIGALRM)
	_exit(0);
    }
  else
    {
      /* master process */
      int event, errsave = errno;
      
      if (sig == SIGHUP)
	event = EVENT_RELOAD;
      else if (sig == SIGCHLD)
	event = EVENT_CHILD;
      else if (sig == SIGALRM)
	event = EVENT_ALARM;
      else if (sig == SIGTERM)
	event = EVENT_TERM;
      else if (sig == SIGUSR1)
	event = EVENT_DUMP;
      else if (sig == SIGUSR2)
	event = EVENT_REOPEN;
      else
	return;

      send_event(pipewrite, event, 0); 
      errno = errsave;
    }
}

void send_event(int fd, int event, int data)
{
  struct event_desc ev;
  
  ev.event = event;
  ev.data = data;
  
  /* error pipe, debug mode. */
  if (fd == -1)
    fatal_event(&ev);
  else
    /* pipe is non-blocking and struct event_desc is smaller than
       PIPE_BUF, so this either fails or writes everything */
    while (write(fd, &ev, sizeof(ev)) == -1 && errno == EINTR);
}

static void fatal_event(struct event_desc *ev)
{
  errno = ev->data;
  
  switch (ev->event)
    {
    case EVENT_DIE:
      exit(0);

    case EVENT_FORK_ERR:
      die(_("cannot fork into background: %s"), NULL, EC_MISC);
  
    case EVENT_PIPE_ERR:
      die(_("failed to create helper: %s"), NULL, EC_MISC);
  
    case EVENT_CAP_ERR:
      die(_("setting capabilities failed: %s"), NULL, EC_MISC);

    case EVENT_USER_ERR:
    case EVENT_HUSER_ERR:
      die(_("failed to change user-id to %s: %s"), 
	  ev->event == EVENT_USER_ERR ? daemon->username : daemon->scriptuser,
	  EC_MISC);

    case EVENT_GROUP_ERR:
      die(_("failed to change group-id to %s: %s"), daemon->groupname, EC_MISC);
      
    case EVENT_PIDFILE:
      die(_("failed to open pidfile %s: %s"), daemon->runfile, EC_FILE);

    case EVENT_LOG_ERR:
      die(_("cannot open %s: %s"), daemon->log_file ? daemon->log_file : "log", EC_FILE);
    }
}	
      
static void async_event(int pipe, time_t now)
{
  pid_t p;
  struct event_desc ev;
  int i;

  if (read_write(pipe, (unsigned char *)&ev, sizeof(ev), 1))
    switch (ev.event)
      {
      case EVENT_RELOAD:
	clear_cache_and_reload(now);
	if (daemon->port != 0 && daemon->resolv_files && (daemon->options & OPT_NO_POLL))
	  {
	    reload_servers(daemon->resolv_files->name);
	    check_servers();
	  }
#ifdef HAVE_DHCP
	rerun_scripts();
#endif
	break;
	
      case EVENT_DUMP:
	if (daemon->port != 0)
	  dump_cache(now);
	break;
	
      case EVENT_ALARM:
#ifdef HAVE_DHCP
	if (daemon->dhcp)
	  {
	    lease_prune(NULL, now);
	    lease_update_file(now);
	  }
#endif
	break;
		
      case EVENT_CHILD:
	/* See Stevens 5.10 */
	while ((p = waitpid(-1, NULL, WNOHANG)) != 0)
	  if (p == -1)
	    {
	      if (errno != EINTR)
		break;
	    }      
	  else 
	    for (i = 0 ; i < MAX_PROCS; i++)
	      if (daemon->tcp_pids[i] == p)
		daemon->tcp_pids[i] = 0;
	break;
	
      case EVENT_KILLED:
	my_syslog(LOG_WARNING, _("child process killed by signal %d"), ev.data);
	break;

      case EVENT_EXITED:
	my_syslog(LOG_WARNING, _("child process exited with status %d"), ev.data);
	break;

      case EVENT_EXEC_ERR:
	my_syslog(LOG_ERR, _("failed to execute %s: %s"), 
		  daemon->lease_change_command, strerror(ev.data));
	break;

	/* necessary for fatal errors in helper */
      case EVENT_HUSER_ERR:
      case EVENT_DIE:
	fatal_event(&ev);
	break;

      case EVENT_REOPEN:
	/* Note: this may leave TCP-handling processes with the old file still open.
	   Since any such process will die in CHILD_LIFETIME or probably much sooner,
	   we leave them logging to the old file. */
	if (daemon->log_file != NULL)
	  log_reopen(daemon->log_file);
	break;
	
      case EVENT_TERM:
	/* Knock all our children on the head. */
	for (i = 0; i < MAX_PROCS; i++)
	  if (daemon->tcp_pids[i] != 0)
	    kill(daemon->tcp_pids[i], SIGALRM);
	
#if defined(HAVE_DHCP) && defined(HAVE_SCRIPT)
	/* handle pending lease transitions */
	if (daemon->helperfd != -1)
	  {
	    /* block in writes until all done */
	    if ((i = fcntl(daemon->helperfd, F_GETFL)) != -1)
	      fcntl(daemon->helperfd, F_SETFL, i & ~O_NONBLOCK); 
	    do {
	      helper_write();
	    } while (!helper_buf_empty() || do_script_run(now));
	    close(daemon->helperfd);
	  }
#endif
	
	if (daemon->lease_stream)
	  fclose(daemon->lease_stream);

	if (daemon->runfile)
	  unlink(daemon->runfile);
	
	my_syslog(LOG_INFO, _("exiting on receipt of SIGTERM"));
	flush_log();
	exit(EC_GOOD);
      }
}

static void poll_resolv()
{
  struct resolvc *res, *latest;
  struct stat statbuf;
  time_t last_change = 0;
  /* There may be more than one possible file. 
     Go through and find the one which changed _last_.
     Warn of any which can't be read. */
  for (latest = NULL, res = daemon->resolv_files; res; res = res->next)
    if (stat(res->name, &statbuf) == -1)
      {
	if (!res->logged)
	  my_syslog(LOG_WARNING, _("failed to access %s: %s"), res->name, strerror(errno));
	res->logged = 1;
      }
    else
      {
	res->logged = 0;
	if (statbuf.st_mtime != res->mtime)
	  {
	    res->mtime = statbuf.st_mtime;
	    if (difftime(statbuf.st_mtime, last_change) > 0.0)
	      {
		last_change = statbuf.st_mtime;
		latest = res;
	      }
	  }
      }
  
  if (latest)
    {
      static int warned = 0;
      if (reload_servers(latest->name))
	{
	  my_syslog(LOG_INFO, _("reading %s"), latest->name);
	  warned = 0;
	  check_servers();
	  if (daemon->options & OPT_RELOAD)
	    cache_reload();
	}
      else 
	{
	  latest->mtime = 0;
	  if (!warned)
	    {
	      my_syslog(LOG_WARNING, _("no servers found in %s, will retry"), latest->name);
	      warned = 1;
	    }
	}
    }
}       

void clear_cache_and_reload(time_t now)
{
  if (daemon->port != 0)
    cache_reload();
  
#ifdef HAVE_DHCP
  if (daemon->dhcp)
    {
      if (daemon->options & OPT_ETHERS)
	dhcp_read_ethers();
      reread_dhcp();
      dhcp_update_configs(daemon->dhcp_conf);
      check_dhcp_hosts(0);
      lease_update_from_configs(); 
      lease_update_file(now); 
      lease_update_dns();
    }
#endif
}

static int set_dns_listeners(time_t now, fd_set *set, int *maxfdp)
{
  struct serverfd *serverfdp;
  struct listener *listener;
  int wait = 0, i;
  
#ifdef HAVE_TFTP
  int  tftp = 0;
  struct tftp_transfer *transfer;
  for (transfer = daemon->tftp_trans; transfer; transfer = transfer->next)
    {
      tftp++;
      FD_SET(transfer->sockfd, set);
      bump_maxfd(transfer->sockfd, maxfdp);
    }
#endif
  
  /* will we be able to get memory? */
  if (daemon->port != 0)
    get_new_frec(now, &wait);
  
  for (serverfdp = daemon->sfds; serverfdp; serverfdp = serverfdp->next)
    {
      FD_SET(serverfdp->fd, set);
      bump_maxfd(serverfdp->fd, maxfdp);
    }

  if (daemon->port != 0 && !daemon->osport)
    for (i = 0; i < RANDOM_SOCKS; i++)
      if (daemon->randomsocks[i].refcount != 0)
	{
	  FD_SET(daemon->randomsocks[i].fd, set);
	  bump_maxfd(daemon->randomsocks[i].fd, maxfdp);
	}
  
  for (listener = daemon->listeners; listener; listener = listener->next)
    {
      /* only listen for queries if we have resources */
      if (listener->fd != -1 && wait == 0)
	{
	  FD_SET(listener->fd, set);
	  bump_maxfd(listener->fd, maxfdp);
	}

      /* death of a child goes through the select loop, so
	 we don't need to explicitly arrange to wake up here */
      if  (listener->tcpfd != -1)
	for (i = 0; i < MAX_PROCS; i++)
	  if (daemon->tcp_pids[i] == 0)
	    {
	      FD_SET(listener->tcpfd, set);
	      bump_maxfd(listener->tcpfd, maxfdp);
	      break;
	    }

#ifdef HAVE_TFTP
      if (tftp <= daemon->tftp_max && listener->tftpfd != -1)
	{
	  FD_SET(listener->tftpfd, set);
	  bump_maxfd(listener->tftpfd, maxfdp);
	}
#endif

    }
  
  return wait;
}

static void check_dns_listeners(fd_set *set, time_t now)
{
  struct serverfd *serverfdp;
  struct listener *listener;
  int i;
  
  for (serverfdp = daemon->sfds; serverfdp; serverfdp = serverfdp->next)
    if (FD_ISSET(serverfdp->fd, set))
      reply_query(serverfdp->fd, serverfdp->source_addr.sa.sa_family, now);
  
  if (daemon->port != 0 && !daemon->osport)
    for (i = 0; i < RANDOM_SOCKS; i++)
      if (daemon->randomsocks[i].refcount != 0 && 
	  FD_ISSET(daemon->randomsocks[i].fd, set))
	reply_query(daemon->randomsocks[i].fd, daemon->randomsocks[i].family, now);
  
  for (listener = daemon->listeners; listener; listener = listener->next)
    {
      if (listener->fd != -1 && FD_ISSET(listener->fd, set))
	receive_query(listener, now); 
      
#ifdef HAVE_TFTP     
      if (listener->tftpfd != -1 && FD_ISSET(listener->tftpfd, set))
	tftp_request(listener, now);
#endif

      if (listener->tcpfd != -1 && FD_ISSET(listener->tcpfd, set))
	{
	  int confd;
	  struct irec *iface = NULL;
	  pid_t p;
	  
	  while((confd = accept(listener->tcpfd, NULL, NULL)) == -1 && errno == EINTR);
	  
	  if (confd == -1)
	    continue;
	  
	  if (daemon->options & OPT_NOWILD)
	    iface = listener->iface;
	  else
	    {
	      union mysockaddr tcp_addr;
	      socklen_t tcp_len = sizeof(union mysockaddr);
	      /* Check for allowed interfaces when binding the wildcard address:
		 we do this by looking for an interface with the same address as 
		 the local address of the TCP connection, then looking to see if that's
		 an allowed interface. As a side effect, we get the netmask of the
		 interface too, for localisation. */
	      
	      /* interface may be new since startup */
	      if (enumerate_interfaces() &&
		  getsockname(confd, (struct sockaddr *)&tcp_addr, &tcp_len) != -1)
		for (iface = daemon->interfaces; iface; iface = iface->next)
		  if (sockaddr_isequal(&iface->addr, &tcp_addr))
		    break;
	    }
	  
	  if (!iface)
	    {
	      shutdown(confd, SHUT_RDWR);
	      close(confd);
	    }
#ifndef NO_FORK
	  else if (!(daemon->options & OPT_DEBUG) && (p = fork()) != 0)
	    {
	      if (p != -1)
		{
		  int i;
		  for (i = 0; i < MAX_PROCS; i++)
		    if (daemon->tcp_pids[i] == 0)
		      {
			daemon->tcp_pids[i] = p;
			break;
		      }
		}
	      close(confd);
	    }
#endif
	  else
	    {
	      unsigned char *buff;
	      struct server *s; 
	      int flags;
	      struct in_addr dst_addr_4;
	      
	      dst_addr_4.s_addr = 0;
	      
	       /* Arrange for SIGALARM after CHILD_LIFETIME seconds to
		  terminate the process. */
	      if (!(daemon->options & OPT_DEBUG))
		alarm(CHILD_LIFETIME);
	      
	      /* start with no upstream connections. */
	      for (s = daemon->servers; s; s = s->next)
		 s->tcpfd = -1; 
	      
	      /* The connected socket inherits non-blocking
		 attribute from the listening socket. 
		 Reset that here. */
	      if ((flags = fcntl(confd, F_GETFL, 0)) != -1)
		fcntl(confd, F_SETFL, flags & ~O_NONBLOCK);
	      
	      if (listener->family == AF_INET)
		dst_addr_4 = iface->addr.in.sin_addr;
	      
	      buff = tcp_request(confd, now, dst_addr_4, iface->netmask);
	       
	      shutdown(confd, SHUT_RDWR);
	      close(confd);
	      
	      if (buff)
		free(buff);
	      
	      for (s = daemon->servers; s; s = s->next)
		if (s->tcpfd != -1)
		  {
		    shutdown(s->tcpfd, SHUT_RDWR);
		    close(s->tcpfd);
		  }
#ifndef NO_FORK		   
	      if (!(daemon->options & OPT_DEBUG))
		{
		  flush_log();
		  _exit(0);
		}
#endif
	    }
	}
    }
}

#ifdef HAVE_DHCP
int make_icmp_sock(void)
{
  int fd;
  int zeroopt = 0;

  if ((fd = socket (AF_INET, SOCK_RAW, IPPROTO_ICMP)) != -1)
    {
      if (!fix_fd(fd) ||
	  setsockopt(fd, SOL_SOCKET, SO_DONTROUTE, &zeroopt, sizeof(zeroopt)) == -1)
	{
	  close(fd);
	  fd = -1;
	}
    }

  return fd;
}

int icmp_ping(struct in_addr addr)
{
  /* Try and get an ICMP echo from a machine. */

  /* Note that whilst in the three second wait, we check for 
     (and service) events on the DNS and TFTP  sockets, (so doing that
     better not use any resources our caller has in use...)
     but we remain deaf to signals or further DHCP packets. */

  int fd;
  struct sockaddr_in saddr;
  struct { 
    struct ip ip;
    struct icmp icmp;
  } packet;
  unsigned short id = rand16();
  unsigned int i, j;
  int gotreply = 0;
  time_t start, now;

#if defined(HAVE_LINUX_NETWORK) || defined (HAVE_SOLARIS_NETWORK)
  if ((fd = make_icmp_sock()) == -1)
    return 0;
#else
  int opt = 2000;
  fd = daemon->dhcp_icmp_fd;
  setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &opt, sizeof(opt));
#endif

  saddr.sin_family = AF_INET;
  saddr.sin_port = 0;
  saddr.sin_addr = addr;
#ifdef HAVE_SOCKADDR_SA_LEN
  saddr.sin_len = sizeof(struct sockaddr_in);
#endif
  
  memset(&packet.icmp, 0, sizeof(packet.icmp));
  packet.icmp.icmp_type = ICMP_ECHO;
  packet.icmp.icmp_id = id;
  for (j = 0, i = 0; i < sizeof(struct icmp) / 2; i++)
    j += ((u16 *)&packet.icmp)[i];
  while (j>>16)
    j = (j & 0xffff) + (j >> 16);  
  packet.icmp.icmp_cksum = (j == 0xffff) ? j : ~j;
  
  while (sendto(fd, (char *)&packet.icmp, sizeof(struct icmp), 0, 
		(struct sockaddr *)&saddr, sizeof(saddr)) == -1 &&
	 retry_send());
  
  for (now = start = dnsmasq_time(); 
       difftime(now, start) < (float)PING_WAIT;)
    {
      struct timeval tv;
      fd_set rset, wset;
      struct sockaddr_in faddr;
      int maxfd = fd; 
      socklen_t len = sizeof(faddr);
      
      tv.tv_usec = 250000;
      tv.tv_sec = 0; 
      
      FD_ZERO(&rset);
      FD_ZERO(&wset);
      FD_SET(fd, &rset);
      set_dns_listeners(now, &rset, &maxfd);
      set_log_writer(&wset, &maxfd);

      if (select(maxfd+1, &rset, &wset, NULL, &tv) < 0)
	{
	  FD_ZERO(&rset);
	  FD_ZERO(&wset);
	}

      now = dnsmasq_time();

      check_log_writer(&wset);
      check_dns_listeners(&rset, now);

#ifdef HAVE_TFTP
      check_tftp_listeners(&rset, now);
#endif

      if (FD_ISSET(fd, &rset) &&
	  recvfrom(fd, &packet, sizeof(packet), 0,
		   (struct sockaddr *)&faddr, &len) == sizeof(packet) &&
	  saddr.sin_addr.s_addr == faddr.sin_addr.s_addr &&
	  packet.icmp.icmp_type == ICMP_ECHOREPLY &&
	  packet.icmp.icmp_seq == 0 &&
	  packet.icmp.icmp_id == id)
	{
	  gotreply = 1;
	  break;
	}
    }
  
#if defined(HAVE_LINUX_NETWORK) || defined(HAVE_SOLARIS_NETWORK)
  close(fd);
#else
  opt = 1;
  setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &opt, sizeof(opt));
#endif

  return gotreply;
}
#endif

//#ifdef TCSUPPORT_DNSEACHPVC
#if defined(TCSUPPORT_DNSEACHPVC) || defined(TCSUPPORT_TR69_BIND_PVC)
static void resolve_dnsServerConf(){
	FILE* fp = NULL;
	struct DNS_Info* newNode = NULL;
	char buf[MAXSIZE] = {0};
	
	// open file and create new node
	fp = fopen("/etc/dnsInfo.conf", "r");
	if(fp == NULL){
		printf("\n open file failed");
		exit(1);
	}
	while(fgets(buf, MAXSIZE, fp)){
		if(strlen(buf) == 0)
			continue;
		newNode = (struct DNS_Info*)malloc(sizeof(struct DNS_Info)); 
		if(!newNode){
  			printf("\n malloc head node failed.");
			exit(1);
		}
		sscanf(buf, "%s %s %s", newNode->strDNS, newNode->strInterface, newNode->strApplication);
		newNode->next = NULL;
		gDNS_Param.pTail->next = newNode;
		gDNS_Param.pTail = newNode;
	}
	fclose(fp);
}
void printfList(){
	struct DNS_Info* newNode = gDNS_Param.pHead;
	newNode = newNode->next;
	while(newNode != NULL){
		printf("\n dns is %s; app is %s ", newNode->strDNS, newNode->strInterface);
		newNode = newNode->next;
	}
}
#endif

#if defined(TCSUPPORT_CT_DNSBIND)
 void fetchURLList(void)
{
	char tempbuf[63] = {0};
	int i,j= 0;
	struct URLFilter_Info *newNode = NULL;
	struct URLFilter_Info *curNode = NULL;
	int active = 0,filterpolicy = 0,urlnum = 0;
	char nodeName[32] = {0};
	char *ptr = NULL;
	
	tcapi_get("UrlFilter_Common","Activate", tempbuf);
	active = atoi(tempbuf);
	if(0 == active)
	{
		printf("\r\nURL filter not start!");
		return;
	}

	memset(tempbuf,0,sizeof(tempbuf));
	tcapi_get("UrlFilter_Common","Filter_Policy", tempbuf);
	filterpolicy = atoi(tempbuf);
	if(0 == filterpolicy)
	{
		//printf("\r\nUse blacklist!");
		return;
	}
	
	if((1 == active) &&(1 == filterpolicy))
		whitelistflag = 1;
	
	memset(tempbuf,0,sizeof(tempbuf));
	tcapi_get("UrlFilter","Url_num", tempbuf);
	urlnum = atoi(tempbuf);

	gURLFilter_Param.next = NULL;
	curNode = &gURLFilter_Param;
	
	for(i = 0; i < MAX_URL_RULE && j < urlnum ; i++)
	{
		memset(tempbuf,0,sizeof(tempbuf));
		sprintf(nodeName,"UrlFilter_Entry%d",i);
		if(tcapi_get(nodeName,"URL", tempbuf) < 0)
				continue;
		newNode = (struct URLFilter_Info*)malloc(sizeof(struct URLFilter_Info)); 
		if(!newNode)
		{
  			tcdbg_printf("\n [dnsmasq:fetchURLList]malloc failed!");
			exit(1);
		}
	
		//printf("\r\ntempbuf url:%s",tempbuf);
		memset(newNode,0,sizeof(struct URLFilter_Info));
		if(NULL !=  (ptr = strstr(tempbuf, WEPPREFIX)) )
		{
			memmove(tempbuf, ptr+4, strlen(ptr) + 1);
			#if 0
			ptr = strstr(tempbuf, ".");
			if(NULL != ptr)
			{
				*ptr = 0;
			}
			#endif
		}

		strncpy(newNode->strUrlName,tempbuf,sizeof(tempbuf));	
		newNode->next = NULL;
		curNode->next = newNode;
		curNode = newNode;
		j++;
	}

	//tcdbg_printf("\r\nThe urlname:");
	curNode = gURLFilter_Param.next;
	while(NULL != curNode)
	{
		//tcdbg_printf("\r\ncurNode->strUrlName is %s",curNode->strUrlName);
		curNode = curNode->next;
	}
	
	return;
}
#endif
