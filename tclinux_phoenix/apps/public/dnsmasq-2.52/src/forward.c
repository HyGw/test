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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../../../private/lib/libtcapi.h"

#if !KERNEL_3_18_21
#if defined(TCSUPPORT_CT_DNSBIND)
#include <linux/version.h>
#define KERNEL_2_6_36 		(LINUX_VERSION_CODE > KERNEL_VERSION(2,6,31))
#if KERNEL_2_6_36
#define IP_SKB_MARK_FLAG		23
#else
#define IP_SKB_MARK_FLAG		19
#endif
#endif
#if defined(TCSUPPORT_CT_VLAN_BIND)
#define	IP_SKB_VLAN_ID_FLAG				24
#endif
#endif
#if defined(TCSUPPORT_CT_JOYME)
int pushViewstate = 0; /* push view upgrade flag*/
/* check osgi push view upgrade
* 1:Yes, 0: No
*/
int checkPushViewState(void);
#endif

static struct frec *lookup_frec(unsigned short id, unsigned int crc);
static struct frec *lookup_frec_by_sender(unsigned short id,
					  union mysockaddr *addr,
					  unsigned int crc);
static unsigned short get_id(unsigned int crc);
static void free_frec(struct frec *f);
static struct randfd *allocate_rfd(int family);

#ifdef CT_COM_DEVICEREG 
int checkDevRegisterFlag(void);
#endif

//#ifdef TCSUPPORT_DNSEACHPVC
#if defined(TCSUPPORT_DNSEACHPVC) || defined(TCSUPPORT_TR69_BIND_PVC)
static int checkTR69Url(int *flag, char *packetUrl);
#if defined(TCSUPPORT_CT)
//static int choose_dns_server(struct server** start, int iDiagFlag, int iTR69Flag, __u32 mark);
static int get_url_info(struct url *request_url, char *namebuf);
static int check_dns_server(struct server** start, struct url *request_url,  __u32 mark);
#else
static int choose_dns_server(struct server** start, int iTR69Flag);
#endif
#if defined(TCSUPPORT_CT_DNSBIND)
static int check_bind(char *wan_if, __u32 mark);
#endif
extern void printfList();
extern struct DNS_Param gDNS_Param;
#endif

#if defined(TCSUPPORT_CT_DNSBIND)
extern struct URLFilter_Info gURLFilter_Param;
extern int whitelistflag;
static int checkUrlFilter(char *requestname,__u32 mark);
#endif

#ifdef CT_COM_DEVICEREG 
extern int devreg;
#endif
#if defined(TCSUPPORT_CUC_LANDING_PAGE)
extern int ponstate;
#endif
#if defined(TCSUPPORT_CT_PON)
int doValPut(char *path, char *value)
{
	FILE *fp;

	fp = fopen(path,"w");
	if(fp != NULL){
		fprintf(fp, value);
		fclose(fp);
	}else{
		return -1;
	}
	return 0;
}
#endif

/* Send a UDP packet with its source address set as "source" 
   unless nowild is true, when we just send it with the kernel default */
static void send_from(int fd, int nowild, char *packet, size_t len, 
		      union mysockaddr *to, struct all_addr *source,
		      unsigned int iface)
{
  struct msghdr msg;
  struct iovec iov[1]; 
  union {
    struct cmsghdr align; /* this ensures alignment */
#if defined(HAVE_LINUX_NETWORK)
    char control[CMSG_SPACE(sizeof(struct in_pktinfo))];
#elif defined(IP_SENDSRCADDR)
    char control[CMSG_SPACE(sizeof(struct in_addr))];
#endif
#ifdef HAVE_IPV6
    char control6[CMSG_SPACE(sizeof(struct in6_pktinfo))];
#endif
  } control_u;
  
  iov[0].iov_base = packet;
  iov[0].iov_len = len;

  msg.msg_control = NULL;
  msg.msg_controllen = 0;
  msg.msg_flags = 0;
  msg.msg_name = to;
  msg.msg_namelen = sa_len(to);
  msg.msg_iov = iov;
  msg.msg_iovlen = 1;
  
  if (!nowild)
    {
      struct cmsghdr *cmptr;
      msg.msg_control = &control_u;
      msg.msg_controllen = sizeof(control_u);
      cmptr = CMSG_FIRSTHDR(&msg);

      if (to->sa.sa_family == AF_INET)
	{
#if defined(HAVE_LINUX_NETWORK)
	  struct in_pktinfo *pkt = (struct in_pktinfo *)CMSG_DATA(cmptr);
	  pkt->ipi_ifindex = 0;
	  pkt->ipi_spec_dst = source->addr.addr4;
	  msg.msg_controllen = cmptr->cmsg_len = CMSG_LEN(sizeof(struct in_pktinfo));
	  cmptr->cmsg_level = SOL_IP;
	  cmptr->cmsg_type = IP_PKTINFO;
#elif defined(IP_SENDSRCADDR)
	  struct in_addr *a = (struct in_addr *)CMSG_DATA(cmptr);
	  *a = source->addr.addr4;
	  msg.msg_controllen = cmptr->cmsg_len = CMSG_LEN(sizeof(struct in_addr));
	  cmptr->cmsg_level = IPPROTO_IP;
	  cmptr->cmsg_type = IP_SENDSRCADDR;
#endif
	}
      else
#ifdef HAVE_IPV6
	{
	  struct in6_pktinfo *pkt = (struct in6_pktinfo *)CMSG_DATA(cmptr);
	  pkt->ipi6_ifindex = iface; /* Need iface for IPv6 to handle link-local addrs */
	  pkt->ipi6_addr = source->addr.addr6;
	  msg.msg_controllen = cmptr->cmsg_len = CMSG_LEN(sizeof(struct in6_pktinfo));
	  cmptr->cmsg_type = daemon->v6pktinfo;
	  cmptr->cmsg_level = IPV6_LEVEL;
	}
#else
      iface = 0; /* eliminate warning */
#endif
    }
  
 retry:
  if (sendmsg(fd, &msg, 0) == -1)
    {
      /* certain Linux kernels seem to object to setting the source address in the IPv6 stack
	 by returning EINVAL from sendmsg. In that case, try again without setting the
	 source address, since it will nearly alway be correct anyway.  IPv6 stinks. */
      if (errno == EINVAL && msg.msg_controllen)
	{
	  msg.msg_controllen = 0;
	  goto retry;
	}
      if (retry_send())
	goto retry;
    }
}
          
static unsigned short search_servers(time_t now, struct all_addr **addrpp, 
				     unsigned short qtype, char *qdomain, int *type, char **domain)
			      
{
  /* If the query ends in the domain in one of our servers, set
     domain to point to that name. We find the largest match to allow both
     domain.org and sub.domain.org to exist. */
  
  unsigned int namelen = strlen(qdomain);
  unsigned int matchlen = 0;
  struct server *serv;
  unsigned short flags = 0;
  
  for (serv = daemon->servers; serv; serv=serv->next)
    /* domain matches take priority over NODOTS matches */
    if ((serv->flags & SERV_FOR_NODOTS) && *type != SERV_HAS_DOMAIN && !strchr(qdomain, '.') && namelen != 0)
      {
	unsigned short sflag = serv->addr.sa.sa_family == AF_INET ? F_IPV4 : F_IPV6; 
	*type = SERV_FOR_NODOTS;
	if (serv->flags & SERV_NO_ADDR)
	  flags = F_NXDOMAIN;
	else if (serv->flags & SERV_LITERAL_ADDRESS) 
	  { 
	    if (sflag & qtype)
	      {
		flags = sflag;
		if (serv->addr.sa.sa_family == AF_INET) 
		  *addrpp = (struct all_addr *)&serv->addr.in.sin_addr;
#ifdef HAVE_IPV6
		else
		  *addrpp = (struct all_addr *)&serv->addr.in6.sin6_addr;
#endif 
	      }
	    else if (!flags || (flags & F_NXDOMAIN))
	      flags = F_NOERR;
	  } 
      }
    else if (serv->flags & SERV_HAS_DOMAIN)
      {
	unsigned int domainlen = strlen(serv->domain);
	char *matchstart = qdomain + namelen - domainlen;
	if (namelen >= domainlen &&
	    hostname_isequal(matchstart, serv->domain) &&
	    domainlen >= matchlen &&
	    (domainlen == 0 || namelen == domainlen || *(serv->domain) == '.' || *(matchstart-1) == '.' ))
	  {
	    unsigned short sflag = serv->addr.sa.sa_family == AF_INET ? F_IPV4 : F_IPV6;
	    *type = SERV_HAS_DOMAIN;
	    *domain = serv->domain;
	    matchlen = domainlen;
	    if (serv->flags & SERV_NO_ADDR)
	      flags = F_NXDOMAIN;
	    else if (serv->flags & SERV_LITERAL_ADDRESS)
	      {
		if (sflag & qtype)
		  {
		    flags = sflag;
		    if (serv->addr.sa.sa_family == AF_INET) 
		      *addrpp = (struct all_addr *)&serv->addr.in.sin_addr;
#ifdef HAVE_IPV6
		    else
		      *addrpp = (struct all_addr *)&serv->addr.in6.sin6_addr;
#endif
		  }
		else if (!flags || (flags & F_NXDOMAIN))
		  flags = F_NOERR;
	      }
	  } 
      }

  if (flags == 0 && !(qtype & F_BIGNAME) && 
      (daemon->options & OPT_NODOTS_LOCAL) && !strchr(qdomain, '.') && namelen != 0)
    /* don't forward simple names, make exception for NS queries and empty name. */
    flags = F_NXDOMAIN;
    
  if (flags == F_NXDOMAIN && check_for_local_domain(qdomain, now))
    flags = F_NOERR;

  if (flags)
    {
      int logflags = 0;
      
      if (flags == F_NXDOMAIN || flags == F_NOERR)
	logflags = F_NEG | qtype;
  
      log_query(logflags | flags | F_CONFIG | F_FORWARD, qdomain, *addrpp, NULL);
    }

  return  flags;
}

#ifdef TC_SUPPORT_3G //johnma
/* Add function to get iface address, including ipv4 and ipv6 */
/*return  0:successful ; otherwise :fail*/
int getifaddr_ipv4(char *ifname, struct in_addr *ifaddr) 
{ 
    struct ifreq ifreq; 
    int sockfd; 
    int ret = -1; 
                                                                                 
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) > 0) 
    { 
        strncpy(ifreq.ifr_name, ifname, IFNAMSIZ); 
        if (ioctl(sockfd, SIOCGIFADDR, &ifreq) >= 0) { 
			if (ifaddr)
            	*ifaddr = ((struct sockaddr_in *)&ifreq.ifr_addr)->sin_addr; 
			ret = 0;
		}
        close(sockfd); 
    } 
    return ret; 
} 
/*return  0:successful ; otherwise :fail*/
int getifaddr(int family, char *ifname, void *ifaddr,int type) 
{ 
	FILE *fp;
	char pppUptimePath[30] = {0};
	int ret = -1;

	if(!ifname){
		tcdbg_printf("get_if_addr:iface is NULL\n");
		return -1;
	}

	/*check pppuptim for ppp on-demand mode*/
	if(strstr(ifname, "ppp")){
		sprintf(pppUptimePath,"/tmp/pppuptime-%s",ifname);
		if((fp = fopen(pppUptimePath,"r")) == NULL)
		{			
			return -1;
		}
		else{
			fclose(fp);
		}
	}
	/*there is no /tmp/PPP11_demand_auth_info for ppp11*/
	if(strcmp(ifname,"ppp11"))
	{
		/*add check for PPP%d_demand_auth_info for ppp on-demand mode*/
		if(strstr(ifname, "ppp"))
		{
#if (defined(TCSUPPORT_WAN_ETHER) || defined(TCSUPPORT_WAN_PTM)) && defined(TCSUPPORT_MULTISERVICE_ON_WAN)
			// ppp device name pppXX. for ex: ppp82
			sprintf(pppUptimePath,"/tmp/PPP%s_demand_auth_info",&ifname[3]);
#else
			sprintf(pppUptimePath,"/tmp/PPP%c_demand_auth_info",*(ifname + 3));
#endif		
			if((fp = fopen(pppUptimePath,"r")) == NULL)
			{			
				return -1;
			}
			else
			{
				fclose(fp);
			}
		}
	}

    switch (family) 
    { 
    case AF_INET:
         ret = getifaddr_ipv4(ifname, (struct in_addr *)ifaddr); 

	 return ret;

    default:
        return -1; 
    } 
}  
#endif

#if defined(TCSUPPORT_URL_ROUTEPOLICY)
#define ROUTEPOLICY_MAX_ENTRY_NUM 8

/*************************
input parameter:
	char *domainName: domain name string
return value: 
	-1: get fail
	others: bind pvc index
************************/
int getBindIfByURL(char *domainName)
{
	char routeURL[64] = {0};
	int i=0;
	char nodeName[32]={0};
	char urlIP[32] = {0};
	char active[4]={0};
	char type[4]={0};
	int findMatchURL = 0;
	int bindPVC = -1;
	char pvcIndex[4]={0};	
	int ret = 0;
	
//	tcdbg_printf("getBindIfByURL:---domainName = %s\n", domainName);

	if((strlen(domainName) == 0))
	{
		return 1;
	}

	for(i=0; i<ROUTEPOLICY_MAX_ENTRY_NUM ; i++)
	{
		memset(nodeName, 0, sizeof(nodeName));
		sprintf(nodeName, "RoutePolicy_Entry%d", i);

		memset(active, 0, sizeof(active));
		if(tcapi_get(nodeName,"Active",active) != 0)
		{
//			tcdbg_printf("getBindIfByURL:--get RoutePolicy_Entry type error!\n");		
			continue;
		}	
		if(atoi(active) == 0)
		{
			//0: deactive,	1: active
			continue;
		}		
		
		memset(type, 0, sizeof(type));
		if(tcapi_get(nodeName,"Type",type) != 0)
		{
//			tcdbg_printf("getBindIfByURL:--get RoutePolicy_Entry type error!\n");		
			continue;
		}
//		tcdbg_printf("getBindIfByURL:---type=%d\n", atoi(type));
		
		if(atoi(type) != 0)
		{
			//1: IP mode,  0: URL mode
			continue;
		}

		memset(routeURL, 0, sizeof(routeURL));
		tcapi_get(nodeName,"URL",routeURL);
//		tcdbg_printf("getBindIfByURL:---routeURL = %s\n", routeURL);
		if(strlen(routeURL) != 0)
		{
			if(doRegularMatch(domainName, routeURL))
			{
	//			tcdbg_printf("getBindIfByURL:---findMatchURL\n");		
				findMatchURL = 1;
				memset(pvcIndex,0,sizeof(pvcIndex));
				if(tcapi_get(nodeName,"PolicyPVC",pvcIndex) == 0)
				{
					bindPVC = atoi(pvcIndex);	
				}	
				break;
			}
		}
		else
		{
			continue;
		}
	}

	return bindPVC;
}

/*************************
input parameter:
	int pvcIndex: pvc index
	struct server **dnsServer: used to store dnsServer1
	struct server **dnsServer: used to store dnsServer2

return value: 
	return dns server number of this pvcs,(0~2)
************************/
int getDNSServerByIf(int pvcIndex, struct server **urlDnsServer1, struct server **urlDnsServer2 )
{
	struct server *start = NULL;
	int serverNum = 0;
	start = daemon->servers;
	char ifName[8]={0};

	while(NULL != start)
	{
		strncpy(ifName, start->interface, sizeof(ifName)-1);
		if((((ifName[0]=='n')&&(ifName[1]=='a')&&(ifName[2]=='s'))
			||((ifName[0]=='p')&&(ifName[1]=='p')&&(ifName[2]=='p')))
			&&((ifName[3]-'0')== pvcIndex))
		{
			serverNum++;
			if(serverNum == 1)			
				*urlDnsServer1 = start;
			else if(serverNum >= 2)
			{
				*urlDnsServer2 = start;
				break;
			}
		}
		start = start->next;
	}

	return serverNum;
}


#endif
#if defined(TCSUPPORT_CT_JOYME)
int find_pid_by_name( char* ProcName, int* foundpid)  
{
	DIR *dir;  
    struct dirent *d;  
    int pid, i;  
    char *s;  
    int pnlen;  
    i = 0;  
    foundpid[0] = 0;  
    pnlen = strlen(ProcName);  
		 		
	/* Open the /proc directory. */  
	dir = opendir("/proc");  
	if (!dir)  
	{  
		tcdbg_printf("cannot open /proc");  
		return -1;  
	}  
  
	/* Walk through the directory. */  
	while ((d = readdir(dir)) != NULL) {  
		char exe [PATH_MAX+1];  
		char path[PATH_MAX+1];  
		int len;  
		int namelen;  
  
		/* See if this is a process */  
		if ((pid = atoi(d->d_name)) == 0)       
			continue;  
		snprintf(exe, sizeof(exe), "/proc/%s/exe", d->d_name);  
		if ((len = readlink(exe, path, PATH_MAX)) < 0)  
			continue;  
		path[len] = '\0';  

		/* Find ProcName */  
		s = strrchr(path, '/');  
		if(s == NULL) 
			continue;  
		s++;  
  
		/* we don't need small name len */  
		namelen = strlen(s);  
		if(namelen < pnlen)     
			continue;  
		if(!strncmp(ProcName, s, pnlen)) {  
        	/* to avoid subname like search proc tao but proc taolinke matched */  
			if(s[pnlen] == ' ' || s[pnlen] == '\0') {  
				foundpid[i] = pid;  
                i++;  
			}  
        }  
	}  
	foundpid[i] = 0;  
	closedir(dir);  
	return  0;  
}  
#endif

#if defined(TCSUPPORT_CT_DNSBIND)
#if defined(TCSUPPORT_CT_VLAN_BIND)
static int forward_query(int udpfd, union mysockaddr *udpaddr,
			 struct all_addr *dst_addr, unsigned int dst_iface,
			 HEADER *header, size_t plen, time_t now, struct frec *forward, __u32 mark, __u16 lVlanId)
#else
static int forward_query(int udpfd, union mysockaddr *udpaddr,
			 struct all_addr *dst_addr, unsigned int dst_iface,
			 HEADER *header, size_t plen, time_t now, struct frec *forward, __u32 mark)
#endif
#else
static int forward_query(int udpfd, union mysockaddr *udpaddr,
			 struct all_addr *dst_addr, unsigned int dst_iface,
			 HEADER *header, size_t plen, time_t now, struct frec *forward)
#endif
{
  char *domain = NULL;
  int type = 0;
  struct all_addr *addrp = NULL;
  unsigned int crc = questions_crc(header, plen, daemon->namebuff);
  unsigned short flags = 0;
  unsigned short gotname = extract_request(header, plen, daemon->namebuff, NULL);
  struct server *start = NULL;
#if defined(TCSUPPORT_CT)
  struct url request_url;
#endif
#if defined(TCSUPPORT_TR69_BIND_PVC)
	char active[4]={0};
	char bindactive[4]={0};
#endif
#if defined(TCSUPPORT_CT_ADV_DNSPROXY)
	int svrfd;
	int ds_frec_delay = 0;
	int svr_sent_delay = 0;
#endif
#if defined(TCSUPPORT_CT_JOYME)
	int fwdidx = 0, isfwd = 0;		
 	int i, rv, pid_t[128]={0}; 
#endif
#if defined(TCSUPPORT_URL_ROUTEPOLICY)
  int pvcIndex = -1;
  struct server *urlDnsServer1 = NULL;
  struct server *urlDnsServer2 = NULL;
  int urlBindSerNum = 0;
#endif

#if defined(TCSUPPORT_CT_DNSBIND)
  unsigned int ret = 0;
ret = checkUrlFilter(daemon->namebuff,mark);
if(ret > 0)
{
#if 0
	if (!flags && forward)
    	{
     	  free_frec(forward); 
   	}
	return 0;
	#endif
		goto	notFindServer;
}
#endif

#if defined(TCSUPPORT_CT_JOYME)
if(!strcmp("felix.extensions",daemon->namebuff)){
 	rv = find_pid_by_name("jamvm" , pid_t);  
	if (!rv) {  
		/* there is 23 thread for jamvm, so when the 22th thread start ok, it mean jamvm start ok */
		if (pid_t[22]== 0) { 
			goto notFindServer;
		}
	} 
}
#endif

 //#ifdef TCSUPPORT_DNSEACHPVC
#if defined(TCSUPPORT_DNSEACHPVC) || defined(TCSUPPORT_TR69_BIND_PVC)
//Add for dns for each pvc
  //open file and check if the url come from tr69
#if !defined(TCSUPPORT_CT)
  int iTR69Flag = 0;
#if defined(TCSUPPORT_TR69_BIND_PVC)
tcapi_get("Cwmp_Entry","Active",active);
tcapi_get("Cwmp_Entry","BindActive",bindactive);

if(!strcmp(active,"Yes")&&!strcmp(bindactive,"Yes"))
	 checkTR69Url(&iTR69Flag, daemon->namebuff);
#else
	checkTR69Url(&iTR69Flag, daemon->namebuff);
 #endif
#endif

#if defined(TCSUPPORT_CT)
  memset(&request_url, 0, sizeof(request_url));
  get_url_info(&request_url, daemon->namebuff);
  //tcdbg_printf("get_url:%s", request_url.interface);
#endif

  #endif

#if defined(TCSUPPORT_CT) && defined(TCSUPPORT_VOIP)
	if( URL_VOIP == request_url.url_type
		&& 0 == request_url.forward )
	{
		tcdbg_printf("\n[%s] will drop url[%s]\n", __FUNCTION__, daemon->namebuff);
		return 0;
	}
#endif


  /* may be no servers available. */
  if (!daemon->servers)
    forward = NULL;
  else if (forward || (forward = lookup_frec_by_sender(ntohs(header->id), udpaddr, crc)))
    {
      /* retry on existing query, send to all available servers  */
      domain = forward->sentto->domain;
      forward->sentto->failed_queries++;
      if (!(daemon->options & OPT_ORDER))
	{
	  forward->forwardall = 1;
	  daemon->last_server = NULL;
	}
      type = forward->sentto->flags & SERV_TYPE;
      if (!(start = forward->sentto->next))
	start = daemon->servers; /* at end of list, recycle */
#if !defined(TCSUPPORT_CT)
	#ifdef TCSUPPORT_DNSEACHPVC
	//choose dns server by application
	if (0 == choose_dns_server(&start, iTR69Flag))
	{
		goto	notFindServer;
		//return 0;
	}
	#endif
#endif
      header->id = htons(forward->new_id);
    }
  else 
    {
#if defined(TCSUPPORT_URL_ROUTEPOLICY)
    pvcIndex = getBindIfByURL(daemon->namebuff);
    urlBindSerNum = getDNSServerByIf(pvcIndex, &urlDnsServer1, &urlDnsServer2);	  	  
#endif
      if (gotname)
	flags = search_servers(now, &addrp, gotname, daemon->namebuff, &type, &domain);
      
      if (!flags && !(forward = get_new_frec(now, NULL)))
	/* table full - server failure. */
	flags = F_NEG;
      
      if (forward)
	{
	  forward->source = *udpaddr;
	  forward->dest = *dst_addr;
	  forward->iface = dst_iface;
	  forward->orig_id = ntohs(header->id);
	  forward->new_id = get_id(crc);
	  forward->fd = udpfd;
	  forward->crc = crc;
	  forward->forwardall = 0;

#if defined(TCSUPPORT_CT_VLAN_BIND)
	  forward->lVlanId = lVlanId;
	  forward->mark = mark;
#endif
	  header->id = htons(forward->new_id);
#if defined(TCSUPPORT_URL_ROUTEPOLICY)
		forward->urlDnsServer1 = urlDnsServer1;
		forward->urlDnsServer2 = urlDnsServer2;
		struct server *tmp=NULL;
		tmp = daemon->servers;
		if(tmp != NULL)
		{
			while(tmp->next)
			{
				if((tmp->next == forward->urlDnsServer1)&&(forward->urlDnsServer1))
				{
					forward->pre_server = tmp;
				}
			  	tmp = tmp->next;
			}		
		}
		forward->last_server=tmp;
#endif
#if defined(TCSUPPORT_CT) && defined(TCSUPPORT_VOIP)
      if( URL_VOIP == request_url.url_type
		 && 1 == request_url.forward )
	  	forward->voip_frec = 1;
#endif


	  /* In strict_order mode, or when using domain specific servers
	     always try servers in the order specified in resolv.conf,
	     otherwise, use the one last known to work. */
	  
	  if (type != 0  || (daemon->options & OPT_ORDER)){
#if defined(TCSUPPORT_URL_ROUTEPOLICY)	  	
	if(urlDnsServer1 != NULL)
		start = urlDnsServer1;
	else
#endif
	    start = daemon->servers;
#if !defined(TCSUPPORT_CT)
		#ifdef TCSUPPORT_DNSEACHPVC
		//choose dns server by application
		if(0 == choose_dns_server(&start, iTR69Flag))
		{
			goto	notFindServer;
			//return 0;	
		}	
		#endif	
#endif
	  }
	  else if (!(start = daemon->last_server) ||
		   daemon->forwardcount++ > FORWARD_TEST ||
		   difftime(now, daemon->forwardtime) > FORWARD_TIME)
	    {
#if defined(TCSUPPORT_URL_ROUTEPOLICY)	  	
	      if(urlDnsServer1 != NULL)
	        start = urlDnsServer1;
	      else
#endif			
	      start = daemon->servers;
	      forward->forwardall = 1;
	      daemon->forwardcount = 0;
	      daemon->forwardtime = now;
	    }
	}
    }

  /* check for send errors here (no route to host) 
     if we fail to send to all nameservers, send back an error
     packet straight away (helps modem users when offline)  */
  
  if (!flags && forward)
    {
      struct server *firstsentto = start;
      int forwarded = 0;

      while (1)
	{ 
#if defined(TCSUPPORT_CT)
		#ifdef TCSUPPORT_DNSEACHPVC
		//choose dns server by application
#if defined(TCSUPPORT_CT_DNSBIND)
		if (0 == check_dns_server(&start, &request_url, mark))
		{
			//tcdbg_printf("go to next server\n");

			goto next_server;
		}
#else
		if(0 == check_dns_server(&start, &request_url, 0))
		{
			//tcdbg_printf("go to next server\n");
			goto next_server;
		}
#endif
		#endif
#endif

		#if defined(TCSUPPORT_TR69_BIND_PVC)
		
		if(start==NULL)
			break;
		else
			{
			if(0 == choose_dns_server(&start, iTR69Flag))
			{
				
				goto	next_server;
		
			}
		}
		#endif
		
	  /* only send to servers dealing with our domain.
	     domain may be NULL, in which case server->domain 
	     must be NULL also. */
	  
	  if (type == (start->flags & SERV_TYPE) &&
	      (type != SERV_HAS_DOMAIN || hostname_isequal(domain, start->domain)) &&
	      !(start->flags & SERV_LITERAL_ADDRESS))
	    {
	      int fd;

	      /* find server socket to use, may need to get random one. */
	      if (start->sfd){
		fd = start->sfd->fd;
	      	}
	      else 
		{
		    {
		      if (!forward->rfd4 &&
			  !(forward->rfd4 = allocate_rfd(AF_INET)))
			break;
		      daemon->rfd_save = forward->rfd4;
		      fd = forward->rfd4->fd;
		    }
		}
		
#if defined(TCSUPPORT_CT_JOYME)
		for ( fwdidx = 0; fwdidx < AWN_IP_NUM; fwdidx ++)
		{
			if ( request_url.addr_used[fwdidx] )
			{
				isfwd = 1;

				if ( start->addr.sa.sa_family
					!= request_url.addr[fwdidx].sa.sa_family )
					continue;

				if ( -1 != sendto(fd, (char *)header, plen, 0,
				 &(request_url.addr[fwdidx].sa),
				 sa_len(&(request_url.addr[fwdidx]))) )
				{
					start->queries++;
					forwarded = 1;
					forward->sentto = start;
					if ( forward->forwardall )
						forward->forwardall++;
				}
			}
			else
				break; /* no need to check other */
		}

		if ( isfwd )
		{
#ifdef TCSUPPORT_SYSLOG
			openlog("TCSysLog", 0, LOG_LOCAL0);
			syslog(LOG_EMERG, "Domain[%s] will be resolved"
				" from attached wan dns tunnel\n", daemon->namebuff);
			closelog();
#endif
			goto next_server;
		}
#endif
#if defined(TCSUPPORT_CT_ADV_DNSPROXY)
		if ( start->flags & SERV_DUAL_STACK )
		{
			if ( start->flags & SERV_DUAL_STACK_DOUBLE )
			{
				if ( forward->source.sa.sa_family != start->addr.sa.sa_family )
				{
					addserver2frec(forward, start);
					if ( !(forward->flags & FREC_DELAY_SENT) )
						addpacket2frec(forward, (char *)header, plen);
					forward->flags |= FREC_DELAY_SENT;
					if ( !(forward->flags & FREC_LOOKUP_OK) )
						forward->time = dnsmasq_time(); // update time
					start->flags |= SERV_FREC_DELAY_SENT;
					goto next_server;
				}
			}
		}
		else
		{
			// A query shoud be sent to IPv4 first
			if ( T_A == daemon->qtype )
			{
				if ( AF_INET != start->addr.sa.sa_family )
				{
					start->flags |= SERV_DELAY_SENT;
					goto next_server;
				}
			}
			// A query shoud be sent to IPv6 first
			else if ( T_AAAA == daemon->qtype )
			{
				if ( AF_INET6 != start->addr.sa.sa_family )
				{	 
					start->flags |= SERV_DELAY_SENT;
					goto next_server;
				}
			}
		}
#endif

	      if (sendto(fd, (char *)header, plen, 0,
			 &start->addr.sa,
			 sa_len(&start->addr)) == -1)
		{
		  if (retry_send())
		    continue;
		}
	      else
		{
		  /* Keep info in case we want to re-send this packet */
		  daemon->srv_save = start;
		  daemon->packet_len = plen;
		  
		  if (!gotname)
		    strcpy(daemon->namebuff, "query");
		  if (start->addr.sa.sa_family == AF_INET)
		    log_query(F_SERVER | F_IPV4 | F_FORWARD, daemon->namebuff, 
			      (struct all_addr *)&start->addr.in.sin_addr, NULL); 
		  start->queries++;
		  forwarded = 1;

#if defined(TCSUPPORT_URL_ROUTEPOLICY)	  	
	if((forward->urlDnsServer1 == NULL)||(forward->urlDnsServer1 == daemon->servers)
		||(forward->urlDnsServer1 == forward->last_server)||(forward->urlDnsServer2 == forward->last_server))
	{
		
	//	tcdbg_printf("dnsmasq:%s, line 842--url dns server is head\n", __FUNCTION__);
		forward->sentto = start;
	}
	else
	{	
		if((start == forward->urlDnsServer1))
		{

			if(forward->urlDnsServer2 == NULL)
			{
				forward->sentto=forward->last_server;
			}
			else
			{
		  		forward->sentto = start;
			}

	//		tcdbg_printf("dnsmasq:%s, line 842--1111\n", __FUNCTION__);
		}
		else if((start == forward->urlDnsServer2)&&(forward->urlDnsServer2 != NULL))
		{
		
			tcdbg_printf("dnsmasq:%s, line 842--2222\n", __FUNCTION__);
			forward->sentto=forward->last_server;
		}
		else if((start->next == forward->urlDnsServer1))
		{
		
	//		tcdbg_printf("dnsmasq:%s, line 842--3333\n", __FUNCTION__);
			if(forward->urlDnsServer2 == NULL)
			{
				forward->sentto=forward->urlDnsServer1;
			}
			else
			{
				forward->sentto=forward->urlDnsServer2;
			}

		}		
		else if((start == forward->last_server))
		{
	//		tcdbg_printf("dnsmasq:%s, line 842--555\n", __FUNCTION__);
			forward->sentto=forward->pre_server;
		}			
		else
		{
			
	//		tcdbg_printf("dnsmasq:%s, line 842--4444\n", __FUNCTION__);
		  forward->sentto = start;
		}
	}
#else
		  forward->sentto = start;
#endif

		  /* Keep info in case we want to re-send this packet */		  
		  if (!forward->forwardall){ 
		    break;
		  }
		  forward->forwardall++;
		}
	    } 
#if defined(TCSUPPORT_CT) || defined(TCSUPPORT_TR69_BIND_PVC)
next_server:	  
#endif
	  if (!(start = start->next))
 	    start = daemon->servers;//have not find a server!
	   if (start == firstsentto)
	  	{
			
			break;
	  	}
	}
      
#if defined(TCSUPPORT_CT_JOYME)
if ( isfwd )
	goto forward_end_pos1;
#endif

#if defined(TCSUPPORT_CT_ADV_DNSPROXY)
	// send query if dns server is marked
	start = daemon->servers;
	while (1)
	{
		if ( start->flags & SERV_DELAY_SENT )
			svr_sent_delay = 1;
		else
			svr_sent_delay = 0;	

		if ( (start->flags & SERV_FREC_DELAY_SENT) && ( 1 != forwarded) )
			ds_frec_delay = 1;
		else
			ds_frec_delay = 0;

		if ( svr_sent_delay
			|| ds_frec_delay )
		{
			if ( NULL == start->sfd )
				continue;
			svrfd = start->sfd->fd;
			if (sendto(svrfd, (char *)header, plen, 0,
			&start->addr.sa,
			sa_len(&start->addr)) == -1)
			{
				if (retry_send())
					continue;
			}
			else
			{
				/* Keep info in case we want to re-send this packet */
				daemon->srv_save = start;
				daemon->packet_len = plen;

				if (!gotname)
					strcpy(daemon->namebuff, "query");
				if (start->addr.sa.sa_family == AF_INET)
					log_query(F_SERVER | F_IPV4 | F_FORWARD, daemon->namebuff, 
							(struct all_addr *)&start->addr.in.sin_addr, NULL); 
				start->queries++;
				forwarded = 1;
				forward->sentto = start;
				if (forward->forwardall)
					forward->forwardall++;
			}
		}

		if ( svr_sent_delay )
			start->flags &= (~SERV_DELAY_SENT);

		if ( ds_frec_delay )
		{
			start->flags &= (~SERV_FREC_DELAY_SENT);
			free_frec_delay_info(forward);
		}
		
		if (!(start = start->next))
		  break;
	}

	if ( forward->flags & FREC_LOOKUP_OK )
		forward->flags &= (~FREC_LOOKUP_OK);
#endif

#if defined(TCSUPPORT_CT_JOYME)
forward_end_pos1:
#endif

      if (forwarded){
	return 1;
      	}
      
      /* could not send on, prepare to return */ 
      header->id = htons(forward->orig_id);
      free_frec(forward); /* cancel */
    }	  
  
  /* could not send on, return empty answer or address if known for whole domain */
  if (udpfd != -1)
    {
      plen = setup_reply(header, plen, addrp, flags, daemon->local_ttl);
      send_from(udpfd, daemon->options & OPT_NOWILD, (char *)header, plen, udpaddr, dst_addr, dst_iface);
    }

  return 0;
  
#ifdef TCSUPPORT_DNSEACHPVC
notFindServer:  
    if (!flags && forward)
    {
       free_frec(forward); /* cancel */
   }
#if defined(TCSUPPORT_CT)
	  /* could not send on, return empty answer or address if known for whole domain */
  if (udpfd != -1)
    {
      plen = setup_reply(header, plen, addrp, flags, daemon->local_ttl);
      send_from(udpfd, daemon->options & OPT_NOWILD, (char *)header, plen, udpaddr, dst_addr, dst_iface);
    }
#endif

	return 0;
#endif
}

static size_t process_reply(HEADER *header, time_t now, 
			    struct server *server, size_t n)
{
  unsigned char *pheader, *sizep;
  int munged = 0, is_sign;
  size_t plen; 

  /* If upstream is advertising a larger UDP packet size
     than we allow, trim it so that we don't get overlarge
     requests for the client. We can't do this for signed packets. */

  if ((pheader = find_pseudoheader(header, n, &plen, &sizep, &is_sign)) && !is_sign)
    {
      unsigned short udpsz;
      unsigned char *psave = sizep;
      
      GETSHORT(udpsz, sizep);
      if (udpsz > daemon->edns_pktsz)
	PUTSHORT(daemon->edns_pktsz, psave);
    }

  if (header->opcode != QUERY || (header->rcode != NOERROR && header->rcode != NXDOMAIN))
    return n;
  
  /* Complain loudly if the upstream server is non-recursive. */
  if (!header->ra && header->rcode == NOERROR && ntohs(header->ancount) == 0 &&
      server && !(server->flags & SERV_WARNED_RECURSIVE))
    {
      prettyprint_addr(&server->addr, daemon->namebuff);
      my_syslog(LOG_WARNING, _("nameserver %s refused to do a recursive query"), daemon->namebuff);
      if (!(daemon->options & OPT_LOG))
	server->flags |= SERV_WARNED_RECURSIVE;
    }  
    
  if (daemon->bogus_addr && header->rcode != NXDOMAIN &&
      check_for_bogus_wildcard(header, n, daemon->namebuff, daemon->bogus_addr, now))
    {
      munged = 1;
      header->rcode = NXDOMAIN;
      header->aa = 0;
    }
  else 
    {
      if (header->rcode == NXDOMAIN && 
	  extract_request(header, n, daemon->namebuff, NULL) &&
	  check_for_local_domain(daemon->namebuff, now))
	{
	  /* if we forwarded a query for a locally known name (because it was for 
	     an unknown type) and the answer is NXDOMAIN, convert that to NODATA,
	     since we know that the domain exists, even if upstream doesn't */
	  munged = 1;
	  header->aa = 1;
	  header->rcode = NOERROR;
	}
      
      if (extract_addresses(header, n, daemon->namebuff, now))
	{
	  my_syslog(LOG_WARNING, _("possible DNS-rebind attack detected"));
	  munged = 1;
	}
    }
  
  /* do this after extract_addresses. Ensure NODATA reply and remove
     nameserver info. */
  
  if (munged)
    {
      header->ancount = htons(0);
      header->nscount = htons(0);
      header->arcount = htons(0);
    }
  
  /* the bogus-nxdomain stuff, doctor and NXDOMAIN->NODATA munging can all elide
     sections of the packet. Find the new length here and put back pseudoheader
     if it was removed. */
  return resize_packet(header, n, pheader, plen);
}

/* sets new last_server */
void reply_query(int fd, int family, time_t now)
{
  /* packet from peer server, extract data for cache, and send to
     original requester */
  HEADER *header;
  union mysockaddr serveraddr;
  struct frec *forward;
  socklen_t addrlen = sizeof(serveraddr);
  ssize_t n = recvfrom(fd, daemon->packet, daemon->edns_pktsz, 0, &serveraddr.sa, &addrlen);
  size_t nn;
  struct server *server;
#if defined(TCSUPPORT_CT_PHONEAPP)
  struct all_addr *dnsERRAddr = NULL;
  int flags = 0, retcode = 0;
  unsigned short q_type = 0;
#endif
  
  /* packet buffer overwritten */
  daemon->srv_save = NULL;
  
  /* Determine the address of the server replying  so that we can mark that as good */
  serveraddr.sa.sa_family = family;
#ifdef HAVE_IPV6
  if (serveraddr.sa.sa_family == AF_INET6)
    serveraddr.in6.sin6_flowinfo = 0;
#endif
  
  /* spoof check: answer must come from known server, */
  for (server = daemon->servers; server; server = server->next)
    if (!(server->flags & (SERV_LITERAL_ADDRESS | SERV_NO_ADDR)) &&
	sockaddr_isequal(&server->addr, &serveraddr))
      break;
   
  header = (HEADER *)daemon->packet;
  
  if ((!server
#if defined(TCSUPPORT_CT_JOYME)
		&& 1 != isAttachWanServer(&serveraddr)
#endif
  		) ||
      n < (int)sizeof(HEADER) || !header->qr ||
      !(forward = lookup_frec(ntohs(header->id), questions_crc(header, n, daemon->namebuff))))
    return;
   
  server = forward->sentto;
  
  if ((header->rcode == SERVFAIL || header->rcode == REFUSED) &&
      !(daemon->options & OPT_ORDER) &&
      forward->forwardall == 0)
    /* for broken servers, attempt to send to another one. */
    {
      unsigned char *pheader;
      size_t plen;
      int is_sign;
      
      /* recreate query from reply */
      pheader = find_pseudoheader(header, (size_t)n, &plen, NULL, &is_sign);
      if (!is_sign)
	{
	  header->ancount = htons(0);
	  header->nscount = htons(0);
	  header->arcount = htons(0);
	  if ((nn = resize_packet(header, (size_t)n, pheader, plen)))
	    {
	      header->qr = 0;
	      header->tc = 0;

#if defined(TCSUPPORT_CT_DNSBIND)
#if defined(TCSUPPORT_CT_VLAN_BIND)
	   forward_query(-1, NULL, NULL, 0, header, nn, now, forward, 0, INVALID_VLAN_ID);
#else
	      forward_query(-1, NULL, NULL, 0, header, nn, now, forward, 0);
#endif
#else
	      forward_query(-1, NULL, NULL, 0, header, nn, now, forward);
#endif
	      return;
	    }
	}
    }   
  
  if ((forward->sentto->flags & SERV_TYPE) == 0)
    {
      if (header->rcode == SERVFAIL || header->rcode == REFUSED)
	server = NULL;
      else
	{
	  struct server *last_server;
	  
	  /* find good server by address if possible, otherwise assume the last one we sent to */ 
	  for (last_server = daemon->servers; last_server; last_server = last_server->next)
	    if (!(last_server->flags & (SERV_LITERAL_ADDRESS | SERV_HAS_DOMAIN | SERV_FOR_NODOTS | SERV_NO_ADDR)) &&
		sockaddr_isequal(&last_server->addr, &serveraddr))
	      {
		server = last_server;
		break;
	      }
	} 
      if (!(daemon->options & OPT_ALL_SERVERS))
	daemon->last_server = server;
    }
  
  /* If the answer is an error, keep the forward record in place in case
     we get a good reply from another server. Kill it when we've
     had replies from all to avoid filling the forwarding table when
     everything is broken */
#if defined(TCSUPPORT_CZ_GENERAL) || defined(TCSUPPORT_CT)
  if (forward->forwardall == 0 || --forward->forwardall == 1 || 
      (header->rcode != REFUSED && header->rcode != SERVFAIL  && header->rcode != 3))
#else
if (forward->forwardall == 0 || --forward->forwardall == 1 || 
	(header->rcode != REFUSED && header->rcode != SERVFAIL	))
#endif
    {
#if defined(TCSUPPORT_CT) && defined(TCSUPPORT_VOIP)
		if ( /*NOERROR != header->rcode && */1 == forward->voip_frec )
		{
			/*free_frec(forward);*/
			tcdbg_printf("[%s], drop voip domain error reply, error:%d!\n", __FUNCTION__, header->rcode);
			if (header->rcode == REFUSED){
			    header->rcode = 3;
			    tcdbg_printf("[%s], refused,drop voip domain error reply, error:%d!\n", __FUNCTION__, header->rcode);
			}
			/*return;*/
		}
#endif

      if ((nn = process_reply(header, now, server, (size_t)n)))
	{
#if defined(TCSUPPORT_CT_VLAN_BIND)
		if ( forward->mark & 0xf0000000 )
		{
#if defined(TCSUPPORT_CT_ADV_DNSPROXY)
			// we should check the family of listener, because listener maybe v4 and wan dest address may be v6
			if ( AF_INET6 == forward->source.sa.sa_family )
			{
				if ( setsockopt(forward->fd, SOL_IPV6, IPV6_SKB_VLAN_ID, &forward->lVlanId, sizeof(forward->lVlanId)) < 0 )
				{
					tcdbg_printf("dnsmasq:%s, setsockopt v4 error!\n", __FUNCTION__);
					return;
				}
			}
			else
			{
				if (setsockopt(forward->fd, SOL_IP, IP_SKB_VLAN_ID, &forward->lVlanId, sizeof(forward->lVlanId)) < 0 )
				{
					tcdbg_printf("dnsmasq:%s, setsockopt v6 error!\n", __FUNCTION__);
					return;
				}
			}
#else
			if (family == AF_INET6) {
				if (setsockopt(forward->fd, SOL_IPV6, IPV6_SKB_VLAN_ID, &forward->lVlanId, sizeof(forward->lVlanId)) == -1) {
					tcdbg_printf("ztz_dbg:%s, setsockopt error!\n", __FUNCTION__);
					return;
				}
			}
			else {
				if (setsockopt(forward->fd, SOL_IP, IP_SKB_VLAN_ID, &forward->lVlanId, sizeof(forward->lVlanId)) == -1) {
					tcdbg_printf("ztz_dbg:%s, setsockopt error!\n", __FUNCTION__);
					return;
				}
			}
#endif
		}
#endif
	  header->id = htons(forward->orig_id);
	  header->ra = 1; /* recursion if available */
#if defined(TCSUPPORT_CT_PHONEAPP)
		if (  0 == header->ancount
			&& ( 0 != (retcode = extract_request(header, (size_t)n, daemon->namebuff, &q_type)) )
			&& 0 == getERRDNSDIP(&dnsERRAddr, q_type, &flags) ) // if no answer
		{
			nn = setup_reply(header, (size_t)n, dnsERRAddr, flags, daemon->local_ttl);
			send_from(forward->fd, daemon->options & OPT_NOWILD, (char *)header, nn, 
					&forward->source, &forward->dest, forward->iface);
		}
		else
		{
#endif
	  send_from(forward->fd, daemon->options & OPT_NOWILD, daemon->packet, nn, 
		    &forward->source, &forward->dest, forward->iface);
#if defined(TCSUPPORT_CT_PHONEAPP)
	}
#endif
	}
      free_frec(forward); /* cancel */
    }
}


void receive_query(struct listener *listen, time_t now)
{
  HEADER *header = (HEADER *)daemon->packet;
  union mysockaddr source_addr;
  unsigned short type;
  struct all_addr dst_addr;
#if defined(TCSUPPORT_CT_DNSBIND)
  __u32			skb_mark = 0;
#endif
#if defined(TCSUPPORT_CT_FJ) || defined(TCSUPPORT_CT_REMOTE_UPG_WEBREDIRECT)
	struct all_addr rep_addr;
	size_t t_plen = 0;
#endif
#if defined(TCSUPPORT_WEB_INTERCEPTION) || defined(TCSUPPORT_REDIRECT_WLAN_WEB)
  struct all_addr reply_addr;
  char adslState[64] = {0};
  int upStat = 0;
  int fd = 0;
  int ret = 0;
  size_t plen = 0;
#ifdef TC_SUPPORT_3G
	struct in_addr ifaddr;
  #endif
#endif
  struct in_addr netmask, dst_addr_4;
  size_t m;
  ssize_t n;
  int if_index = 0;
  struct iovec iov[1];
  struct msghdr msg;
  struct cmsghdr *cmptr;
  union {
    struct cmsghdr align; /* this ensures alignment */
#ifdef HAVE_IPV6
#if defined(TCSUPPORT_CT_DNSBIND)
char control6[CMSG_SPACE(sizeof(struct in6_pktinfo))+ 
    	 CMSG_SPACE(sizeof(__u32))
#if defined(TCSUPPORT_CT_VLAN_BIND)
	+ CMSG_SPACE(sizeof(__u16))
#endif
    	 ];
#else
    char control6[CMSG_SPACE(sizeof(struct in6_pktinfo))];
#endif
#endif
#if defined(HAVE_LINUX_NETWORK)
#if defined(TCSUPPORT_CT_DNSBIND)
char control[CMSG_SPACE(sizeof(struct in_pktinfo)) + 
    	 CMSG_SPACE(sizeof(__u32)) 
#if defined(TCSUPPORT_CT_VLAN_BIND)
		+ CMSG_SPACE(sizeof(__u16))
#endif
    	 ];
#else
    char control[CMSG_SPACE(sizeof(struct in_pktinfo))];
#endif
#elif defined(IP_RECVDSTADDR) && defined(HAVE_SOLARIS_NETWORK)
    char control[CMSG_SPACE(sizeof(struct in_addr)) +
		 CMSG_SPACE(sizeof(unsigned int))];
#elif defined(IP_RECVDSTADDR)
    char control[CMSG_SPACE(sizeof(struct in_addr)) +
		 CMSG_SPACE(sizeof(struct sockaddr_dl))];
#endif
  } control_u;
#if defined(TCSUPPORT_CT)
    char entry_buf[256] = {0};  
    int tr69_flag = 0;
    //int registerFlag = 0;
#if defined(TCSUPPORT_CT_PON)
	char dnsbuf[560] = {0};
#endif
#endif

#if defined(TCSUPPORT_CT_VLAN_BIND)
	__u16	lVlanId = INVALID_VLAN_ID;
#endif

  /* packet buffer overwritten */
  daemon->srv_save = NULL;
  
  if (listen->family == AF_INET && (daemon->options & OPT_NOWILD))
    {
      dst_addr_4 = listen->iface->addr.in.sin_addr;
      netmask = listen->iface->netmask;
    }
  else
    {
      dst_addr_4.s_addr = 0;
      netmask.s_addr = 0;
    }

  iov[0].iov_base = daemon->packet;
  iov[0].iov_len = daemon->edns_pktsz;
    
  msg.msg_control = control_u.control;
  msg.msg_controllen = sizeof(control_u);
  msg.msg_flags = 0;
  msg.msg_name = &source_addr;
  msg.msg_namelen = sizeof(source_addr);
  msg.msg_iov = iov;
  msg.msg_iovlen = 1;
  
  if ((n = recvmsg(listen->fd, &msg, 0)) == -1)
    return;
  
  if (n < (int)sizeof(HEADER) || 
      (msg.msg_flags & MSG_TRUNC) ||
      header->qr)
    return;
  
  source_addr.sa.sa_family = listen->family;
#ifdef HAVE_IPV6
  if (listen->family == AF_INET6)
    source_addr.in6.sin6_flowinfo = 0;
#endif
  
  if (!(daemon->options & OPT_NOWILD))
    {
      struct ifreq ifr;

      if (msg.msg_controllen < sizeof(struct cmsghdr))
	return;

#if defined(HAVE_LINUX_NETWORK)
      if (listen->family == AF_INET)
	for (cmptr = CMSG_FIRSTHDR(&msg); cmptr; cmptr = CMSG_NXTHDR(&msg, cmptr)) {
	  if (cmptr->cmsg_level == SOL_IP && cmptr->cmsg_type == IP_PKTINFO)
	    {
	      dst_addr_4 = dst_addr.addr.addr4 = ((struct in_pktinfo *)CMSG_DATA(cmptr))->ipi_spec_dst;
	      if_index = ((struct in_pktinfo *)CMSG_DATA(cmptr))->ipi_ifindex;
	    }
	  
#if defined(TCSUPPORT_CT_DNSBIND)
	 if (cmptr && cmptr->cmsg_level == SOL_IP && cmptr->cmsg_type == IP_SKB_MARK_FLAG) {

		skb_mark = *((__u32*)CMSG_DATA(cmptr));

	 }
	
#endif

#if defined(TCSUPPORT_CT_VLAN_BIND)
	if (cmptr && cmptr->cmsg_level == SOL_IP && cmptr->cmsg_type == IP_SKB_VLAN_ID_FLAG) {
	   lVlanId = *((__u16*)CMSG_DATA(cmptr));
	}
#endif

    }

#elif defined(IP_RECVDSTADDR) && defined(IP_RECVIF)
      if (listen->family == AF_INET)
	{
	  for (cmptr = CMSG_FIRSTHDR(&msg); cmptr; cmptr = CMSG_NXTHDR(&msg, cmptr))
	    if (cmptr->cmsg_level == IPPROTO_IP && cmptr->cmsg_type == IP_RECVDSTADDR)
	      dst_addr_4 = dst_addr.addr.addr4 = *((struct in_addr *)CMSG_DATA(cmptr));
	    else if (cmptr->cmsg_level == IPPROTO_IP && cmptr->cmsg_type == IP_RECVIF)
#ifdef HAVE_SOLARIS_NETWORK
	      if_index = *((unsigned int *)CMSG_DATA(cmptr));
#else
	      if_index = ((struct sockaddr_dl *)CMSG_DATA(cmptr))->sdl_index;
#endif
	}
#endif
      
#ifdef HAVE_IPV6
      if (listen->family == AF_INET6)
	{
	  for (cmptr = CMSG_FIRSTHDR(&msg); cmptr; cmptr = CMSG_NXTHDR(&msg, cmptr)) {
	    if (cmptr->cmsg_level == IPV6_LEVEL && cmptr->cmsg_type == daemon->v6pktinfo)
	      {
		dst_addr.addr.addr6 = ((struct in6_pktinfo *)CMSG_DATA(cmptr))->ipi6_addr;
		if_index =((struct in6_pktinfo *)CMSG_DATA(cmptr))->ipi6_ifindex;
	      }
#if defined(TCSUPPORT_CT_DNSBIND)
		if (cmptr && cmptr->cmsg_level == SOL_IP && cmptr->cmsg_type == IP_SKB_MARK_FLAG) {

		skb_mark = *((__u32*)CMSG_DATA(cmptr));

		printf("v6v6 sk_mark is %x\n", skb_mark);	
	 }
#endif

#if defined(TCSUPPORT_CT_VLAN_BIND)
		if (cmptr && cmptr->cmsg_level == SOL_IP && cmptr->cmsg_type == IP_SKB_VLAN_ID_FLAG) {
		   lVlanId = *((__u16*)CMSG_DATA(cmptr));
		}
#endif
	  }
	}
#endif
      
      /* enforce available interface configuration */
      
      if (!indextoname(listen->fd, if_index, ifr.ifr_name) ||
	  !iface_check(listen->family, &dst_addr, ifr.ifr_name, &if_index))
	return;
      
      if (listen->family == AF_INET &&
	  (daemon->options & OPT_LOCALISE) &&
	  ioctl(listen->fd, SIOCGIFNETMASK, &ifr) == -1)
	return;
      
      netmask = ((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr;
    }

#if defined(TCSUPPORT_WEB_INTERCEPTION) || defined(TCSUPPORT_REDIRECT_WLAN_WEB)
	fd = open("/proc/tc3162/adsl_stats",O_RDONLY|O_NONBLOCK);
	if( fd <= 0 ){
		return;
	}
	ret = read(fd, adslState, 64);
	close(fd);
	if( ret <= 0 ){
		return;
	}
	if(strstr(adslState, "up"))
	{
#if !defined(TCSUPPORT_C1_OBM) && !defined(TCSUPPORT_WIZARD_FON_OI) 
		upStat = 1;
#endif
	}
	else
	{
		upStat = 0;		
	}		
#ifdef TC_SUPPORT_3G	
		// if get ip address from ppp11(only for 3g dongle) ,not redirect it
		if(getifaddr(AF_INET, "ppp11", &ifaddr,0) == 0)
		{
			upStat = 1;
		}
#endif
#endif

#if defined(TCSUPPORT_WEB_INTERCEPTION)
#if !defined(TSUPPORT_C1_OBM) && !defined(TCSUPPORT_WIZARD_FON_OI) 
	if((upStat == 0) || (0 == access("/tmp/firmwareupgrade", F_OK)))
#endif
	{
		extract_request(header, (size_t)n, daemon->namebuff, &type);
		
		reply_addr.addr.addr4.s_addr = htonl(inet_addr("212.175.13.169"));
		plen = setup_reply(header, (size_t)n, &reply_addr, F_IPV4, daemon->local_ttl);
  		send_from(listen->fd, daemon->options & OPT_NOWILD, (char *)header, plen, &source_addr, &dst_addr, if_index);
		return;
	}
#endif

#if defined(TCSUPPORT_CT_FJ) || defined(TCSUPPORT_CT_REMOTE_UPG_WEBREDIRECT)
	if(0 == access("/tmp/cwmpfirmwareupgrade", F_OK)){
		extract_request(header, (size_t)n, daemon->namebuff, &type);
		rep_addr.addr.addr4.s_addr = htonl(inet_addr("192.168.1.1"));
		t_plen = setup_reply(header, (size_t)n, &rep_addr, F_IPV4, daemon->local_ttl);
  		send_from(listen->fd, daemon->options & OPT_NOWILD, (char *)header, t_plen, &source_addr, &dst_addr, if_index);
		return;
	}
#endif

#if defined(TCSUPPORT_CT_ADV_DNSPROXY)
  daemon->qtype = 0;
#endif
  if (extract_request(header, (size_t)n, daemon->namebuff, &type))
    {
      char types[20];

      querystr(types, type);

      if (listen->family == AF_INET) 
	log_query(F_QUERY | F_IPV4 | F_FORWARD, daemon->namebuff, 
		  (struct all_addr *)&source_addr.in.sin_addr, types);
#ifdef HAVE_IPV6
      else
	log_query(F_QUERY | F_IPV6 | F_FORWARD, daemon->namebuff, 
		  (struct all_addr *)&source_addr.in6.sin6_addr, types);
#endif
#if defined(TCSUPPORT_CT_ADV_DNSPROXY)
	  switch ( type )
	  {
	  	case T_A:
		case T_AAAA:
			daemon->qtype = type;
		default:
			break;
    }
#endif
    }

#if defined(TCSUPPORT_CT_JOYME)
	if ( DNS_SPEED_LIMIT_ACTION_DROP == 
		checkDNS_SpeedLimit(daemon->namebuff, listen->family, &source_addr) )
		return; /* drop the query. */
#endif
  
#if defined(TCSUPPORT_CT)
   //tcdbg_printf("namebuf: %s\n", daemon->namebuff);
   checkTR69Url(&tr69_flag, daemon->namebuff);
 //  registerFlag = checkDevRegisterFlag();

#ifdef CT_COM_DEVICEREG 

   if(/*registerFlag*/ devreg)
  {
     if( !tr69_flag && (skb_mark & 0xf0000000) )
     {	
#if defined(TCSUPPORT_CT_PON)
#ifdef HAVE_IPV6
	sprintf(entry_buf, "fe80::1 %s def", daemon->namebuff);
#else
	memset(entry_buf, 0, sizeof(entry_buf));
#endif
	sprintf(dnsbuf, "192.168.1.1 %s def\n%s", daemon->namebuff, entry_buf);
	doValPut("/etc/hosts", dnsbuf);
#else
  //	tcdbg_printf("namebuf: %s\n", daemon->namebuff);
  	sprintf(entry_buf, "echo \"192.168.1.1 %s def\" > /etc/hosts", daemon->namebuff);
  	system(entry_buf);
#ifdef HAVE_IPV6
	memset(entry_buf, 0, sizeof(entry_buf));
  	sprintf(entry_buf, "echo \"fe80::1 %s def\" >> /etc/hosts", daemon->namebuff);
  	system(entry_buf);
#endif
#endif
  	cache_reload();
     }
  }
#endif
#if defined(TCSUPPORT_CUC_LANDING_PAGE)
  if ( 0 == ponstate )
  {
	  if( !tr69_flag && (skb_mark & 0xf0000000) )
	  {
		  sprintf(entry_buf, "echo \"192.168.1.1 %s def\" > /etc/hosts", daemon->namebuff);
		  system(entry_buf);
#ifdef HAVE_IPV6
		  memset(entry_buf, 0, sizeof(entry_buf));
		  sprintf(entry_buf, "echo \"fe80::1 %s def\" >> /etc/hosts", daemon->namebuff);
		  system(entry_buf);
#endif
		  cache_reload();
	  }
  }
#endif
#if defined(TCSUPPORT_CT_JOYME)
  if ( 1 == pushViewstate )
  {
	  if( !tr69_flag && (skb_mark & 0xf0000000) )
	  {
		  sprintf(entry_buf, "echo \"192.168.1.1 %s def\" > /etc/hosts", daemon->namebuff);
		  system(entry_buf);
#ifdef HAVE_IPV6
		  memset(entry_buf, 0, sizeof(entry_buf));
		  sprintf(entry_buf, "echo \"fe80::1 %s def\" >> /etc/hosts", daemon->namebuff);
		  system(entry_buf);
#endif
		  cache_reload();
	  }
  }
#endif
#endif

  m = answer_request (header, ((char *) header) + PACKETSZ, (size_t)n, 
		      dst_addr_4, netmask, now);
  if (m >= 1)
    {
      send_from(listen->fd, daemon->options & OPT_NOWILD, (char *)header, 
		m, &source_addr, &dst_addr, if_index);
      daemon->local_answer++;
    }

#if defined(TCSUPPORT_CT_DNSBIND)
#if defined(TCSUPPORT_CT_VLAN_BIND)
	 else if (forward_query(listen->fd, &source_addr, &dst_addr, if_index,
				header, (size_t)n, now, NULL, skb_mark, lVlanId))
#else
  else if (forward_query(listen->fd, &source_addr, &dst_addr, if_index,
			 header, (size_t)n, now, NULL, skb_mark))
#endif
#else
  else if (forward_query(listen->fd, &source_addr, &dst_addr, if_index,
			 header, (size_t)n, now, NULL))
#endif
    daemon->queries_forwarded++;
  else
    daemon->local_answer++;
}

/* The daemon forks before calling this: it should deal with one connection,
   blocking as neccessary, and then return. Note, need to be a bit careful
   about resources for debug mode, when the fork is suppressed: that's
   done by the caller. */
unsigned char *tcp_request(int confd, time_t now,
			   struct in_addr local_addr, struct in_addr netmask)
{
  int size = 0;
  size_t m;
  unsigned short qtype, gotname;
  unsigned char c1, c2;
  /* Max TCP packet + slop */
  unsigned char *packet = whine_malloc(65536 + MAXDNAME + RRFIXEDSZ);
  HEADER *header;
  struct server *last_server;
  
  while (1)
    {
      if (!packet ||
	  !read_write(confd, &c1, 1, 1) || !read_write(confd, &c2, 1, 1) ||
	  !(size = c1 << 8 | c2) ||
	  !read_write(confd, packet, size, 1))
       	return packet; 
  
      if (size < (int)sizeof(HEADER))
	continue;
      
      header = (HEADER *)packet;
      
      if ((gotname = extract_request(header, (unsigned int)size, daemon->namebuff, &qtype)))
	{
	  union mysockaddr peer_addr;
	  socklen_t peer_len = sizeof(union mysockaddr);
	  
	  if (getpeername(confd, (struct sockaddr *)&peer_addr, &peer_len) != -1)
	    {
	      char types[20];

	      querystr(types, qtype);

	      if (peer_addr.sa.sa_family == AF_INET) 
		log_query(F_QUERY | F_IPV4 | F_FORWARD, daemon->namebuff, 
			  (struct all_addr *)&peer_addr.in.sin_addr, types);
#ifdef HAVE_IPV6
	      else
		log_query(F_QUERY | F_IPV6 | F_FORWARD, daemon->namebuff, 
			  (struct all_addr *)&peer_addr.in6.sin6_addr, types);
#endif
	    }
	}
      
      /* m > 0 if answered from cache */
      m = answer_request(header, ((char *) header) + 65536, (unsigned int)size, 
			 local_addr, netmask, now);

      /* Do this by steam now we're not in the select() loop */
      check_log_writer(NULL); 
      
      if (m == 0)
	{
	  unsigned short flags = 0;
	  struct all_addr *addrp = NULL;
	  int type = 0;
	  char *domain = NULL;
	  
	  if (gotname)
	    flags = search_servers(now, &addrp, gotname, daemon->namebuff, &type, &domain);
	  
	  if (type != 0  || (daemon->options & OPT_ORDER) || !daemon->last_server)
	    last_server = daemon->servers;
	  else
	    last_server = daemon->last_server;
      
	  if (!flags && last_server)
	    {
	      struct server *firstsendto = NULL;
	      unsigned int crc = questions_crc(header, (unsigned int)size, daemon->namebuff);

	      /* Loop round available servers until we succeed in connecting to one.
	         Note that this code subtley ensures that consecutive queries on this connection
	         which can go to the same server, do so. */
	      while (1) 
		{
		  if (!firstsendto)
		    firstsendto = last_server;
		  else
		    {
		      if (!(last_server = last_server->next))
			last_server = daemon->servers;
		      
		      if (last_server == firstsendto)
			break;
		    }
	      
		  /* server for wrong domain */
		  if (type != (last_server->flags & SERV_TYPE) ||
		      (type == SERV_HAS_DOMAIN && !hostname_isequal(domain, last_server->domain)))
		    continue;
		  
		  if ((last_server->tcpfd == -1) &&
		      (last_server->tcpfd = socket(last_server->addr.sa.sa_family, SOCK_STREAM, 0)) != -1 &&
		      (!local_bind(last_server->tcpfd,  &last_server->source_addr, last_server->interface, 1) ||
		       connect(last_server->tcpfd, &last_server->addr.sa, sa_len(&last_server->addr)) == -1))
		    {
		      close(last_server->tcpfd);
		      last_server->tcpfd = -1;
		    }
		  
		  if (last_server->tcpfd == -1)	
		    continue;

		  c1 = size >> 8;
		  c2 = size;
		  
		  if (!read_write(last_server->tcpfd, &c1, 1, 0) ||
		      !read_write(last_server->tcpfd, &c2, 1, 0) ||
		      !read_write(last_server->tcpfd, packet, size, 0) ||
		      !read_write(last_server->tcpfd, &c1, 1, 1) ||
		      !read_write(last_server->tcpfd, &c2, 1, 1))
		    {
		      close(last_server->tcpfd);
		      last_server->tcpfd = -1;
		      continue;
		    } 
		  
		  m = (c1 << 8) | c2;
		  if (!read_write(last_server->tcpfd, packet, m, 1))
		    return packet;
		  
		  if (!gotname)
		    strcpy(daemon->namebuff, "query");
		  if (last_server->addr.sa.sa_family == AF_INET)
		    log_query(F_SERVER | F_IPV4 | F_FORWARD, daemon->namebuff, 
			      (struct all_addr *)&last_server->addr.in.sin_addr, NULL); 
#ifdef HAVE_IPV6
		  else
		    log_query(F_SERVER | F_IPV6 | F_FORWARD, daemon->namebuff, 
			      (struct all_addr *)&last_server->addr.in6.sin6_addr, NULL);
#endif 
		  
		  /* There's no point in updating the cache, since this process will exit and
		     lose the information after a few queries. We make this call for the alias and 
		     bogus-nxdomain side-effects. */
		  /* If the crc of the question section doesn't match the crc we sent, then
		     someone might be attempting to insert bogus values into the cache by 
		     sending replies containing questions and bogus answers. */
		  if (crc == questions_crc(header, (unsigned int)m, daemon->namebuff))
		    m = process_reply(header, now, last_server, (unsigned int)m);
		  
		  break;
		}
	    }
	  
	  /* In case of local answer or no connections made. */
	  if (m == 0)
	    m = setup_reply(header, (unsigned int)size, addrp, flags, daemon->local_ttl);
	}

      check_log_writer(NULL);
      
      c1 = m>>8;
      c2 = m;
      if (!read_write(confd, &c1, 1, 0) ||
	  !read_write(confd, &c2, 1, 0) || 
	  !read_write(confd, packet, m, 0))
	return packet;
    }
}

static struct frec *allocate_frec(time_t now)
{
  struct frec *f;
  
  if ((f = (struct frec *)whine_malloc(sizeof(struct frec))))
    {
      f->next = daemon->frec_list;
      f->time = now;
      f->sentto = NULL;
      f->rfd4 = NULL;
#ifdef HAVE_IPV6
      f->rfd6 = NULL;
#endif
#if defined(TCSUPPORT_CT_ADV_DNSPROXY)
	  f->frec_server = NULL;
	  f->packet = NULL;
	  f->plen = 0;
	  f->flags = 0;
#endif
#if defined(TCSUPPORT_CT) && defined(TCSUPPORT_VOIP)
      f->voip_frec = 0;
#endif
      daemon->frec_list = f;
    }

  return f;
}

static struct randfd *allocate_rfd(int family)
{
  static int finger = 0;
  int i;

  /* limit the number of sockets we have open to avoid starvation of 
     (eg) TFTP. Once we have a reasonable number, randomness should be OK */

  for (i = 0; i < RANDOM_SOCKS; i++)
    if (daemon->randomsocks[i].refcount == 0)
      {
	if ((daemon->randomsocks[i].fd = random_sock(family)) == -1)
	  break;
      
	daemon->randomsocks[i].refcount = 1;
	daemon->randomsocks[i].family = family;
	return &daemon->randomsocks[i];
      }

  /* No free ones or cannot get new socket, grab an existing one */
  for (i = 0; i < RANDOM_SOCKS; i++)
    {
      int j = (i+finger) % RANDOM_SOCKS;
      if (daemon->randomsocks[j].refcount != 0 &&
	  daemon->randomsocks[j].family == family && 
	  daemon->randomsocks[j].refcount != 0xffff)
	{
	  finger = j;
	  daemon->randomsocks[j].refcount++;
	  return &daemon->randomsocks[j];
	}
    }

  return NULL; /* doom */
}

#if defined(TCSUPPORT_CT_ADV_DNSPROXY)
int addserver2frec(struct frec *pfrec, struct server *psvr)
{
	struct frec_svr **new_frec_svr = NULL;

	if ( !pfrec || !psvr )
		return -1;

	new_frec_svr = &pfrec->frec_server;
	while ( *new_frec_svr )
	{
		new_frec_svr = &((*new_frec_svr)->next);
	}

	*new_frec_svr = safe_malloc(sizeof(struct frec_svr));
	if ( !(*new_frec_svr) )
		return -1;

	(*new_frec_svr)->next = NULL;
	(*new_frec_svr)->dstsvr = psvr;

	return 0;
}

int addpacket2frec(struct frec *pfrec, char *packet, size_t len)
{
	if ( !pfrec || !packet )
		return -1;

	if ( !pfrec->packet )
		pfrec->packet = safe_malloc(DNSMASQ_PACKETSZ);

	if ( !pfrec->packet )
		return -1;

	memset(pfrec->packet, 0, DNSMASQ_PACKETSZ);
	memcpy(pfrec->packet, packet, len);
	pfrec->plen = len;
	return 0;
}

void sendpkt2server(struct frec *f)
{
	struct server *start = NULL;
	struct frec_svr *start_frec_svr = NULL;
	int svrfd;

	if ( !f || !f->frec_server || !f->packet
		|| 0 == f->plen )
		return;

	for ( start_frec_svr = f->frec_server; start_frec_svr; )
	{
		if ( start_frec_svr->dstsvr )
		{
			start = start_frec_svr->dstsvr;
			if ( NULL == start || NULL == start->sfd )
					continue;
			svrfd = start->sfd->fd;
			if (sendto(svrfd, (char *)f->packet, f->plen, 0,
			&start->addr.sa,
			sa_len(&start->addr)) == -1)
			{
				if (retry_send())
					continue;
			}
			else
			{
				/* Keep info in case we want to re-send this packet */
				daemon->srv_save = start;
				daemon->packet_len = f->plen;

				if (start->addr.sa.sa_family == AF_INET)
					log_query(F_SERVER | F_IPV4 | F_FORWARD, daemon->namebuff, 
							(struct all_addr *)&start->addr.in.sin_addr, NULL); 
				start->queries++;
				f->sentto = start;
				f->forwardall++;
			}
			start_frec_svr->dstsvr = NULL;
		}

		start_frec_svr = start_frec_svr->next;
	}

}

void check_frec_packets()
{
	struct frec *f = NULL;
	int count = 0;
	time_t now = dnsmasq_time();

	for ( f = daemon->frec_list, count = 0; f; f = f->next, count++ )
	{
		if ( f->flags & FREC_DELAY_SENT )
		{
			if ( difftime(now, f->time) >= daemon->delay_timeout )
			{
				sendpkt2server(f);
				f->flags &= (~FREC_DELAY_SENT);
			}
		}
		 
		if (count > daemon->ftabsize )
			break;
	}

}

int is_exist_frec_packets()
{
	struct frec *f = NULL;
	int count = 0;

	for ( f = daemon->frec_list, count = 0; f; f = f->next, count++ )
	{
		if ( f->flags & FREC_DELAY_SENT )
			return 1;
	}

	return 0;
}

// check frec packet when receive data
int check_frec_packets_recvmod(time_t now)
{
	static time_t old;
	static int init_time = 0;

	if ( 0 == init_time )
	{
		old = dnsmasq_time();
		init_time = 1;
	}

	if ( difftime(now, old) >= daemon->delay_timeout )
	{
		check_frec_packets();
		old = dnsmasq_time();
	}	
}

int free_frec_delay_info(struct frec *f)
{
	free_frec_servers(f);
	f->frec_server = NULL;
	f->flags &= (~FREC_DELAY_SENT);
	if ( f->packet )
	{
		free(f->packet);
		f->packet = NULL;
	}
}

void free_frec_servers(struct frec *f)
{
	struct frec_svr *start = NULL;
	struct frec_svr *temp = NULL;

	if ( !f )
		return;

	for ( start = f->frec_server; start;  )
	{
		temp = start->next;
		free(start);
		start = temp;
	}
}
#endif

static void free_frec(struct frec *f)
{
  if (f->rfd4 && --(f->rfd4->refcount) == 0)
    close(f->rfd4->fd);
    
  f->rfd4 = NULL;
  f->sentto = NULL;
  
#ifdef HAVE_IPV6
  if (f->rfd6 && --(f->rfd6->refcount) == 0)
    close(f->rfd6->fd);
    
  f->rfd6 = NULL;
#endif
#if defined(TCSUPPORT_CT_ADV_DNSPROXY)
  free_frec_servers(f);
  f->frec_server = NULL;
  if ( f->packet )
  {
	free(f->packet);
	f->packet = NULL;
  }
  f->plen = 0;
  f->flags = 0;
#endif
#if defined(TCSUPPORT_CT) && defined(TCSUPPORT_VOIP)
  f->voip_frec = 0;
#endif
}

/* if wait==NULL return a free or older than TIMEOUT record.
   else return *wait zero if one available, or *wait is delay to
   when the oldest in-use record will expire. Impose an absolute
   limit of 4*TIMEOUT before we wipe things (for random sockets) */
struct frec *get_new_frec(time_t now, int *wait)
{
  struct frec *f, *oldest, *target;
  int count;
  
  if (wait)
    *wait = 0;

  for (f = daemon->frec_list, oldest = NULL, target =  NULL, count = 0; f; f = f->next, count++)
    if (!f->sentto)
      target = f;
    else 
      {
	if (difftime(now, f->time) >= 4*TIMEOUT)
	  {
	    free_frec(f);
	    target = f;
	  }
	
	if (!oldest || difftime(f->time, oldest->time) <= 0)
	  oldest = f;
      }

  if (target)
    {
      target->time = now;
      return target;
    }
  
  /* can't find empty one, use oldest if there is one
     and it's older than timeout */
  if (oldest && ((int)difftime(now, oldest->time)) >= TIMEOUT)
    { 
      /* keep stuff for twice timeout if we can by allocating a new
	 record instead */
      if (difftime(now, oldest->time) < 2*TIMEOUT && 
	  count <= daemon->ftabsize &&
	  (f = allocate_frec(now)))
	return f;

      if (!wait)
	{
	  free_frec(oldest);
	  oldest->time = now;
	}
      return oldest;
    }
  
  /* none available, calculate time 'till oldest record expires */
  if (count > daemon->ftabsize)
    {
      if (oldest && wait)
	*wait = oldest->time + (time_t)TIMEOUT - now;
      return NULL;
    }
  
  if (!(f = allocate_frec(now)) && wait)
    /* wait one second on malloc failure */
    *wait = 1;

  return f; /* OK if malloc fails and this is NULL */
}
 
/* crc is all-ones if not known. */
static struct frec *lookup_frec(unsigned short id, unsigned int crc)
{
  struct frec *f;

  for(f = daemon->frec_list; f; f = f->next)
    if (f->sentto && f->new_id == id && 
	(f->crc == crc || crc == 0xffffffff))
      return f;
      
  return NULL;
}

static struct frec *lookup_frec_by_sender(unsigned short id,
					  union mysockaddr *addr,
					  unsigned int crc)
{
  struct frec *f;
  
  for(f = daemon->frec_list; f; f = f->next)
    if (f->sentto &&
	f->orig_id == id && 
	f->crc == crc &&
	sockaddr_isequal(&f->source, addr))
    {
#if defined(TCSUPPORT_CT_ADV_DNSPROXY)
		f->flags |= FREC_LOOKUP_OK;
#endif
      return f;
    }
   
  return NULL;
}

/* A server record is going away, remove references to it */
void server_gone(struct server *server)
{
  struct frec *f;
  
  for (f = daemon->frec_list; f; f = f->next)
    if (f->sentto && f->sentto == server)
      free_frec(f);
  
  if (daemon->last_server == server)
    daemon->last_server = NULL;

  if (daemon->srv_save == server)
    daemon->srv_save = NULL;
}

/* return unique random ids. */
static unsigned short get_id(unsigned int crc)
{
  unsigned short ret = 0;
  
  do 
    ret = rand16();
  while (lookup_frec(ret, crc));
  
  return ret;
}

#ifdef CT_COM_DEVICEREG 
int checkDevRegisterFlag(void)
{
  char buf[5] = {0};
  int status, result, no_landingpage = 0;

  tcapi_get("deviceAccount_Entry","registerStatus",buf);
  status = atoi(buf);

  memset(buf, 0, sizeof(buf) );
  tcapi_get("deviceAccount_Entry","registerResult",buf);
  result = atoi(buf);

  memset(buf, 0, sizeof(buf) );
  if ( 0 != tcapi_get("deviceAccount_Entry", "isNOLandingPage", buf) )
  	strcpy(buf, "0");
  no_landingpage = atoi(buf);

  if ( 1 == no_landingpage )
  	return 0;

  if(status == 99 || (status == 0 && result == 2) || (status == 0 && result == 99) 	
 	 || (status == 1) || (status == 2) || (status == 3) || (status == 4) || (status == 0 && result == 0))
        return 1;
  else
        return 0;
}
#endif
#if defined(TCSUPPORT_CUC_LANDING_PAGE)
int checkPonState()
{
	int regState = 0;
	int ponState = 0;
	char tmp[64] = {0};

#ifdef CT_COM_DEVICEREG
	regState = checkDevRegisterFlag();
#endif

	if ( regState )
		return 2;

	tcapi_get("XPON_Common", "trafficStatus", tmp);
	if (0 == strcmp(tmp, "up"))
		return 1;
	else
		return 0;
}
#endif
#if defined(TCSUPPORT_CT_JOYME)
/* check osgi push view upgrade
* 1:Yes, 0: No
*/
int checkPushViewState(void)
{
	char pusview[12] = {0};
	int regState = 0;

#ifdef CT_COM_DEVICEREG
	regState = checkDevRegisterFlag();
#endif

	if ( regState )
		return 2;

	bzero(pusview, sizeof(pusview));
	if ( 0 == tcapi_get("OSGIUpgrade_Entry"
			, "pushview", pusview)
		&& 0 == strcmp(pusview, "Yes") )
		return 1;

	return 0;

}
#endif

//#ifdef TCSUPPORT_DNSEACHPVC
#if defined(TCSUPPORT_DNSEACHPVC) || defined(TCSUPPORT_TR69_BIND_PVC)
#ifdef HAVE_IPV6
/* Utility function for making IPv6 address string. */
const char *
inet6_ntoa (struct in6_addr addr)
{
  static char buf[INET6_ADDRSTRLEN];

  inet_ntop (AF_INET6, &addr, buf, INET6_ADDRSTRLEN);
  return buf;
}
#endif /* HAVE_IPV6 */

static int checkTR69Url(int *flag, char *packetUrl)
{
  FILE *fp = NULL;
  char dnsValue[50] = {0};

  fp = fopen("/etc/tr69Url.conf", "r");
  if(fp == NULL){
	  printf("\n tr69Url.conf not exit or open file failed");
  }else{
	  while(fgets(dnsValue, 2048, fp)){
	  	if(strlen(dnsValue) == 0)
			  printf("\n content is empty.");
	  	else{
			  if(strcmp(dnsValue, packetUrl) == 0){
				  *flag = 1;
				  break;
			  }
	  	 }
	  }
	  fclose(fp);
  }

#if defined(TCSUPPORT_CT_MIDWARE)
  fp = fopen("/etc/midwareUrl.conf", "r");
  if(fp == NULL){
	  tcdbg_printf("\n midwareUrl.conf not exit or open file failed");
  }else{
	  while(fgets(dnsValue, 2048, fp)){
	  	if(strlen(dnsValue) == 0)
			  printf("\n content is empty.");
	  	else{
			  if(strcmp(dnsValue, packetUrl) == 0){
				  *flag = 1;
				  break;
			  }
	  	 }
	  }
	  fclose(fp);
  }
#endif

#if defined(TCSUPPORT_CT_PHONEAPP)
  fp = fopen("/etc/mobileUrl.conf", "r");
  if(fp == NULL){
	  printf("\n mobileUrl.conf not exit or open file failed");
  }else{
	  while(fgets(dnsValue, 2048, fp)){
	  	if(strlen(dnsValue) == 0)
			  printf("\n content is empty.");
	  	else{
			  if(strcmp(dnsValue, packetUrl) == 0){
				  *flag = 1;
				  break;
			  }
	  	 }
	  }
	  fclose(fp);
  }
#endif

}

#if defined(TCSUPPORT_CT)
static int get_url_info(struct url *request_url, char *namebuf)
{
	FILE *fp = NULL;
	char dnsValue[MAXSIZE] = {0};
	char url[MAXSIZE] = {0}, interface[IF_NAMESIZE + 1] = {0};
#if defined(TCSUPPORT_VOIP)
	char forward[4] = {0};
#endif

	request_url->url_type = URL_INTERNET;
#if defined(TCSUPPORT_CT_JOYME)
	checkDnsTunnel(request_url, namebuf);
#endif

	fp = fopen("/tmp/diagdns.conf", "r");
	if(fp == NULL){
		//tcdbg_printf("\n open /tmp/diagdns.conf failed");
	}
	else{
		//tcdbg_printf("read diagdns success!\n");
		while(fgets(dnsValue, MAXSIZE - 1, fp)){
			if(strlen(dnsValue) == 0)
				continue;
			
			if(2 == sscanf(dnsValue, "%s %s", url, interface))
			{
			//tcdbg_printf("read diagdns:url %s,if:%s\n", url, interface);
				if(!strcmp(url, namebuf)){
					request_url->url_type = URL_DIAGNOSTIC;
					strncpy(request_url->interface, interface, IF_NAMESIZE);
					fclose(fp);
					return 0;

				}
			}
			fclose(fp);
		}
	}
#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
	fp = fopen("/tmp/ippingdiag.conf", "r");
	if(fp == NULL){
		//tcdbg_printf("\n open /tmp/ippingdiag.conf failed");
	}
	else{
		//tcdbg_printf("read ippingdiag success!\n");
		while(fgets(dnsValue, MAXSIZE - 1, fp)){
			if(strlen(dnsValue) == 0)
				continue;
			
			if(2 == sscanf(dnsValue, "%s %s", url, interface))
			{
				//tcdbg_printf("read ippingdiag:url %s,if:%s\n", url, interface);
				if(!strcmp(url, namebuf)){
					request_url->url_type = URL_IPPINGDIAGNOSTIC;
					strncpy(request_url->interface, interface, IF_NAMESIZE);
					fclose(fp);
					return 0;
				}
			}
			fclose(fp);
		}
	}
#endif
	
#if defined(TCSUPPORT_VOIP)
	fp = fopen("/tmp/voicedns.conf", "r+");
	if(fp == NULL){
		//tcdbg_printf("\n open /tmp/voicedns.conf failed");
	}
	else{
		//tcdbg_printf("read voicedns success!\n");
		while(fgets(dnsValue, MAXSIZE - 1, fp)){
			if(strlen(dnsValue) == 0)
				continue;
			
			if(3 == sscanf(dnsValue, "%s %s %s", url, interface, forward))
			{
				//tcdbg_printf("read voicedns:url %s,if:%s\n", url, interface);
				if(!strcmp(url, namebuf)){
					request_url->url_type = URL_VOIP;
					strncpy(request_url->interface, interface, IF_NAMESIZE);
					request_url->forward = atoi(forward);
					snprintf(dnsValue, sizeof(dnsValue) - 1, "%s %s 0\n", url, interface);
					fseek(fp, 0, SEEK_SET);
					fputs(dnsValue, fp);
					fclose(fp);
					return 0;
				}
			}
		}
		fclose(fp);
	}
#endif
#if defined(TCSUPPORT_CT_MONITORCOLLECTOR)
	fp = fopen("/tmp/monitorCollector.conf", "r");
	if(fp == NULL){
		//tcdbg_printf("\n open /tmp/monitorCollector.conf failed");
	}
	else{
		//tcdbg_printf("read monitorCollector success!\n");
		while(fgets(dnsValue, MAXSIZE - 1, fp)){
			if(strlen(dnsValue) == 0)
				continue;
	
			if(2 == sscanf(dnsValue, "%s %s", url, interface))
			{
				//tcdbg_printf("read monitorCollector:url %s,if:%s\n", url, interface);
				if(!strcmp(url, namebuf)){
					request_url->url_type = URL_MONITORCOLLECTOR;
					strncpy(request_url->interface, interface, IF_NAMESIZE);
					fclose(fp);
					return 0;
				}
			}
			fclose(fp);
		}
	}
#endif
#if defined(TCSUPPORT_CT_2NTP)
	fp = fopen("/tmp/ntpdns.conf", "r");
	if(fp == NULL){
		//tcdbg_printf("\n open /tmp/ntpdns.conf failed");
	}
	else{
		while(fgets(dnsValue, MAXSIZE - 1, fp)){
			if(strlen(dnsValue) == 0)
				continue;
			
			if(2 == sscanf(dnsValue, "%s %s", url, interface))
			{
				if(!strcmp(url, namebuf)){
					request_url->url_type = URL_NTP;
					strncpy(request_url->interface, interface, IF_NAMESIZE);
					fclose(fp);
					return 0;
				}
			}			
		}
		fclose(fp);
	}	
#endif
	
#if defined(TCSUPPORT_CT_MIDWARE)	
	fp = fopen("/etc/midwareUrl.conf", "r");
	if(fp == NULL){
	  //tcdbg_printf("\n midwareUrl.conf not exit or open file failed");
	}
	else{
		while(fgets(dnsValue, MAXSIZE - 1, fp)){
			if(strlen(dnsValue) == 0)
			  printf("\n content is empty.");
			else{
			  if(strcmp(dnsValue, namebuf) == 0){
				  request_url->url_type = URL_TR069;
				  break;
			  }
			 }
		}
		fclose(fp);
	}
#endif

#if defined(TCSUPPORT_CT_PHONEAPP)
	fp = fopen("/etc/mobileUrl.conf", "r");
	if(fp == NULL){
	  //tcdbg_printf("\n midwareUrl.conf not exit or open file failed");
	}
	else{
		while(fgets(dnsValue, MAXSIZE - 1, fp)){
			if(strlen(dnsValue) == 0)
			  printf("\n content is empty.");
			else{
			  if(strcmp(dnsValue, namebuf) == 0){
				  request_url->url_type = URL_TR069;
				  break;
			  }
			 }
		}
		fclose(fp);
	}
#endif

#if defined(TCSUPPORT_CT_PPPOE_IPV6_EMULATOR)
	fp = fopen("/tmp/pppemulatordomain", "r");
	if ( fp )
	{
		while(fgets(dnsValue, MAXSIZE - 1, fp))
		{
			if(strlen(dnsValue) == 0)
				continue;

			if(2 == sscanf(dnsValue, "%s %s", url, interface))
			{
				if(!strcmp(url, namebuf)){
					request_url->url_type = URL_PPPEMULATOR;
					strncpy(request_url->interface, interface, IF_NAMESIZE);
					fclose(fp);
					return 0;
				}
			}			
		}
		fclose(fp);
	}
#endif

#if defined(TCSUPPORT_CT_PMINFORM)
	fp = fopen("/tmp/PMInformDomain.conf", "r");
	if ( fp )
	{
		while(fgets(dnsValue, MAXSIZE - 1, fp))
		{
			if(strlen(dnsValue) == 0)
				continue;

			if(2 == sscanf(dnsValue, "%s %s", url, interface))
			{
				if(!strcmp(url, namebuf)){
					request_url->url_type = URLPMINFORM;
					strncpy(request_url->interface, interface, IF_NAMESIZE);
					fclose(fp);
					return 0;
				}
			}			
		}
		fclose(fp);
	}
#endif

	fp = fopen("/etc/tr69Url.conf", "r");
	if(fp == NULL){
	  //tcdbg_printf("\n tr69Url.conf not exit or open file failed");
	}
	else{
		while(fgets(dnsValue, MAXSIZE - 1, fp)){
			if(strlen(dnsValue) == 0)
			  printf("\n content is empty.");
			else{
			  if(strcmp(dnsValue, namebuf) == 0){
				  request_url->url_type = URL_TR069;
				  break;
			  }
			 }
		}
		fclose(fp);
	}

#if defined(TCSUPPORT_CT)
	fp = fopen("/etc/tr69DWUPUrl.conf", "r");
	if(fp == NULL){
	  //tcdbg_printf("\n tr69DWUPUrl.conf not exit or open file failed");
	}
	else{
		while(fgets(dnsValue, MAXSIZE - 1, fp)){
			if(strlen(dnsValue) == 0)
			  printf("\n content is empty.");
			else{
			  if(strcmp(dnsValue, namebuf) == 0){
				  request_url->url_type = URL_TR069;
				  break;
			  }
			 }
		}
		fclose(fp);
	}
#endif

	return 0;

}
#endif

#if !defined(TCSUPPORT_CT)
#if !defined(TCSUPPORT_TR69_BIND_PVC)
static int choose_dns_server(struct server** start, int iTR69Flag)
{	
	struct DNS_Info* currNode = NULL;
	int iMatch = 0; //indicate whether dns server match conf file
	//struct server* firstsendto = *start;
	int recycle = 0;
	//printfList(); //for debug
	
	if(1 == iTR69Flag) //request come from tr69
	{
		while(NULL != *start)
	    	{
	    		currNode = gDNS_Param.pHead->next;
		    	while(NULL != currNode)
		    	{
		    		if(0 == strcmp(currNode->strDNS, inet_ntoa((*start)->addr.in.sin_addr)) && 
							0 == strcmp(currNode->strInterface, (*start)->interface) &&
					0 == strcmp(currNode->strApplication, "TR69"))
		    		{
		    			iMatch = 1;
					break;
		    		}
				currNode = currNode->next;
		    	}
			if(1 == iMatch)
				break;
			*start = (*start)->next;
			if(recycle == 0 && *start == NULL){	/* at end of list, recycle */
				*start = daemon->servers;
				recycle = 1;
			}
	    	}
	    }else //request come from other application
	    {
	    	while(NULL != *start)
	    	{
	    		currNode = gDNS_Param.pHead->next;
		    	while(NULL != currNode)
		    	{
		    		if(0 == strcmp(currNode->strDNS, inet_ntoa((*start)->addr.in.sin_addr)) &&
							0 == strcmp(currNode->strInterface, (*start)->interface) && 
							0 != strcmp(currNode->strApplication, "TR69"))
		    		{
		    			iMatch = 1;
					break;
		    		}
				currNode = currNode->next;
		    	}
			if(1 == iMatch)
				break;
			*start = (*start)->next;
			if(recycle == 0 && *start == NULL){	/* at end of list, recycle */
				*start = daemon->servers;
				recycle = 1;
			}
	    	}
	    }
	//start is null means cannot find match dns server
	if(NULL == *start)
	{
		printf("\n Cannot find match dns server.");
		return 0;
	}
	return 1;
}
#else
static int choose_dns_server(struct server** start, int iTR69Flag)
{	
	struct DNS_Info* currNode = NULL;
	char dnsIpAddr[40] = {0};

	if((*start)->addr.sa.sa_family == AF_INET) {//dns server ip is ipv4
			strcpy(dnsIpAddr, inet_ntoa((*start)->addr.in.sin_addr));
	}
	#ifdef HAVE_IPV6
	else {//dns server ip is ipv6
		strcpy(dnsIpAddr, inet6_ntoa((*start)->addr.in6.sin6_addr));
	}
	#endif
	currNode = gDNS_Param.pHead->next;
	while(currNode!=NULL)
	{
		if(0 == strcmp(currNode->strDNS, dnsIpAddr) && 
					0 == strcmp(currNode->strInterface, (*start)->interface))
		{
			if(1 == iTR69Flag)
			{
				if(0 == strcmp(currNode->strApplication, "TR069")|| 0 == strcmp(currNode->strApplication, "BOTH"))
					break;
			}
			else
			{
				if(0 == strcmp(currNode->strApplication, "DEFAULT")|| 0 == strcmp(currNode->strApplication, "BOTH"))
					break;

			}
		}

		currNode=currNode->next;
	}
	
	if(NULL == currNode)//the server is not appropriate
	{
		return 0;
	}
	return 1;
}
#endif
#endif

#if defined(TCSUPPORT_CT)
static int check_dns_server(struct server** start, struct url *request_url,  __u32 mark)
{	
	struct DNS_Info* currNode = NULL;
	int allowed= 0, bind_reval = 0; 
	char dnsIpAddr[40] = {0};

	if((*start)->addr.sa.sa_family == AF_INET) {//dns server ip is ipv4
			strcpy(dnsIpAddr, inet_ntoa((*start)->addr.in.sin_addr));
	}
	#ifdef HAVE_IPV6
	else {//dns server ip is ipv6
		strcpy(dnsIpAddr, inet6_ntoa((*start)->addr.in6.sin6_addr));
	}
	#endif

	currNode = gDNS_Param.pHead->next;
    	while(NULL != currNode)
    	{
    		if(0 == strcmp(currNode->strDNS, dnsIpAddr) && 
					0 == strcmp(currNode->strInterface, (*start)->interface) )
    		{
				//tcdbg_printf("check_dns_server:Node=%s\n", currNode->strInterface);
				//tcdbg_printf("check_dns_server:Start=%s\n", (*start)->interface);
					//tcdbg_printf("check_dns_server:url_type=%d\n",request_url->url_type );

    			if(request_url->url_type == URL_TR069){
				if(NULL != strstr(currNode->strApplication, "TR069")){
					allowed = 1;
					break;
				}
    			}
			else if(request_url->url_type == URL_DIAGNOSTIC){
				if(strlen(request_url->interface) > 0){
						if(0 == strcmp(request_url->interface, (*start)->interface)){
							//tcdbg_printf("check_dns_server:diagnostic\n");

							allowed = 1;
							break;
						}
					}

			}
#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
			else if(request_url->url_type == URL_IPPINGDIAGNOSTIC){
				if(strlen(request_url->interface) > 0){
						if(0 == strcmp(request_url->interface, (*start)->interface)){
							//tcdbg_printf("check_dns_server:ip ping diagnostic \n");
							allowed = 1;
							break;
						}
					}
			}
#endif
#if defined(TCSUPPORT_CT_2NTP)
			else if(request_url->url_type == URL_NTP){
				if(strlen(request_url->interface) > 0){
					if(0 == strcmp(request_url->interface, (*start)->interface)){
						allowed = 1;
						break;
					}
				}
			}
#endif
#if defined(TCSUPPORT_CT_MONITORCOLLECTOR)
			else if(request_url->url_type == URL_MONITORCOLLECTOR){
				if(strlen(request_url->interface) > 0){
						if(0 == strcmp(request_url->interface, (*start)->interface)){
							//tcdbg_printf("check_dns_server:ip ping diagnostic \n");
							allowed = 1;
							break;
						}
					}
			}
#endif
#if defined(TCSUPPORT_CT_PPPOE_IPV6_EMULATOR)
			else if(request_url->url_type == URL_PPPEMULATOR){
				if(strlen(request_url->interface) > 0){
					if(0 == strcmp(request_url->interface, (*start)->interface)){
						allowed = 1;
						break;
					}
				}
			}
#endif
#if defined(TCSUPPORT_CT_PMINFORM)
			else if(request_url->url_type == URLPMINFORM){
				if(strlen(request_url->interface) > 0){
					if(0 == strcmp(request_url->interface, (*start)->interface)){
						allowed = 1;
						break;
					}
				}
			}
#endif
#if defined(TCSUPPORT_VOIP)
			else if(request_url->url_type == URL_VOIP){
				if(strlen(request_url->interface) > 0){
					if ( 0 == request_url->forward )
						tcdbg_printf("\n[%s] will drop url[%s]\n", __FUNCTION__, daemon->namebuff);
					else
					{
						if(0 == strcmp(request_url->interface, (*start)->interface)){
							allowed = 1;
							break;
						}
					}
				}
			}
#endif
			else{
				if(0 != strcmp(currNode->strApplication, "TR069")){					
					
#if defined(TCSUPPORT_CT_DNSBIND)
		    			/* search bind wan interface of this lan interface */
					bind_reval = check_bind(currNode->strInterface, mark);
					if (bind_reval == 0){/*1.lan if is binded, and wan interface match*/						
		    				allowed = 1;
						break;
					}				
					else if(bind_reval > 0){/*2. lan if is not  binded or from CPE, select internet wan if*/						
						if(0 != strcmp(currNode->strApplication, "OTHER")){	
							if (strstr(currNode->strApplication, "INTERNET") != NULL)
							{
								allowed = 1;
								break;
							}
						}
					}
					/*3. lan if is binded, and not match wan if*/
					#if 0
		    			if (!check_bind(currNode->strInterface, mark)) {
						printf("internet:ifname=%s\n", currNode->strInterface);
		    				allowed = 1;
						break;
					}
					#endif
					
#else
					
					allowed = 1;
					break;
					
#endif
				}
			}    			
    		}
		currNode = currNode->next;
    	}
					//tcdbg_printf("check_dns_server:allowd=%d\n", allowed);

	return allowed;
}
#endif

#if defined(TCSUPPORT_CT_DNSBIND)
/*_____________________________________________________________________________
**      function name: check_bind
**      descriptions:
**           check if the lan interface and wan interface in the same group.
**
**      parameters:
**            wan_if: 			wan interface
**			  mark:				skb->mark from socketopt, lan port info 
**								is stored in this mark
**
**      global:
**             None
**
**      return:
**             0 	wan interface match or dns packet from cpe
**			   -1   wan interfac not match and lan if bind
**		   1  lan if not bind or dns packet from cpe
**
**      call:
**
**      revision:
**      1. xyzhu_nj 20100928
**	   2.jlliu_nj 20110321
**____________________________________________________________________________
*/
#define MAX_PVC_NUM	 8
#define MAX_SMUX_NUM 8
static int check_bind(char *wan_if, __u32 mark)
{
	char wan_node[32], wan_if_val[20];
	__u32 pt_mark = (mark & 0xf0000000) >> 28;
	char lan_if[10], lan_if_val[10];
	int i, j, ret, bind_flag = 0;
#if defined(TCSUPPORT_CT_VLAN_BIND)
	int fd=0, switchOn=0, ifaceIndex=0, i_pvc = 0, i_entry = 0;
	char buf[4]={0}, vbindActive[5]={0},index[5]={0}, pvc[5]={0}, entry[5]={0} ;
	__u32 vbind_mark = (mark & 0x7f0000) >> 16;
#endif

	printf("check_bind: pt_mask is %u\n", pt_mark);
	
	/* the packet is from CPE, don't care binding info */
	if (pt_mark == 0) {
		return 1;
	}

#if defined(TCSUPPORT_CT_VLAN_BIND)
	fd = open(VBIND_ACTIVE_PATH,O_RDONLY|O_NONBLOCK);
	if (fd<0) {
		tcdbg_printf("check_bind:open %s error.\n",VBIND_ACTIVE_PATH);
		return -1;
	}
	ret = read(fd, buf, sizeof(buf) );
	close(fd);
	if (ret <= 0) {
		tcdbg_printf("check_bind:read %s error.\n",VBIND_ACTIVE_PATH);
		return -1;
	}	
	switchOn = atoi(buf);
	if (pt_mark>0 && pt_mark<=MAX_LAN_PORT_NUM) {
		sprintf(wan_node,  "VlanBind_Entry%d", pt_mark-1);
		ret = tcapi_get(wan_node, "Active", vbindActive);
		if (ret < 0) {
			strcpy(vbindActive, "No");
			/*
			tcdbg_printf("check_bind:tcapi_get %s Active error.\n",wan_node);
			return -1;
			*/
		}
	}
	if (switchOn && !strcmp(vbindActive, "Yes")) {
		if (vbind_mark == 0) {
			return 1;
		}
		else {
			if(strstr(wan_if, "ppp")){
				sscanf(wan_if, "ppp%d", &ifaceIndex);	
			}
			else{   /*nas interface,ext:nas0_1*/
				sscanf(wan_if, "nas%d_%d", &i_pvc, &i_entry);
				ifaceIndex = i_pvc * MAX_SMUX_NUM + i_entry;
			}
			if(ifaceIndex < 0 || ifaceIndex >= MAX_WAN_IF_INDEX) {
				tcdbg_printf("check_bind:get wan interface index error.\n");
				return -1;
			}
			if ((ifaceIndex + 1) == vbind_mark)
				return 0;
		}
	}
	else
#endif
	{
	/* find lan if */
	switch (pt_mark) {
	#ifdef TCSUPPORT_TC2031
		case 1:
			strcpy(lan_if, "LAN1");
			break;
	#else
		case 1:
			strcpy(lan_if, "LAN1");
			break;
		case 2:
			strcpy(lan_if, "LAN2");
			break;
		case 3:
			strcpy(lan_if, "LAN3");
			break;
		case 4:
			strcpy(lan_if, "LAN4");
			break;
	#endif
	#if 0
		case 5:
			strcpy(lan_if, "SSID0");
			break;
		case 6:
			strcpy(lan_if, "SSID1");
			break;
		case 7:
			strcpy(lan_if, "SSID2");
			break;
		case 8:
			strcpy(lan_if, "SSID3");
			break;
	#else
		case 5:
			strcpy(lan_if, "SSID1");
			break;
		case 6:
			strcpy(lan_if, "SSID2");
			break;
		case 7:
			strcpy(lan_if, "SSID3");
			break;
		case 8:
			strcpy(lan_if, "SSID4");
			break;
	#endif
		case 9:
			strcpy(lan_if, "USB0");
			break;
		case 11:
			strcpy(lan_if, "SSIDAC1");
			break;
		case 12:
			strcpy(lan_if, "SSIDAC2");
			break;
		case 13:
			strcpy(lan_if, "SSIDAC3");
			break;
		case 14:
			strcpy(lan_if, "SSIDAC4");
			break;
		default:
			return -1;
	}

	printf("check_bind: lan port is %s\n", lan_if);

	memset(lan_if_val, 0, sizeof(lan_if_val));
	memset(wan_if_val, 0, sizeof(wan_if_val));
	for (i = 0; i < MAX_PVC_NUM; i++) {
		for (j = 0; j < MAX_SMUX_NUM; j++) {
			sprintf(wan_node,  "Wan_PVC%d_Entry%d", i, j);
			ret = tcapi_get(wan_node, lan_if, lan_if_val);
			if (ret < 0) {
				continue;
			}

			if (!strcmp(lan_if_val, "Yes")) {
				tcapi_get(wan_node, "IFName", wan_if_val);
				bind_flag = 1;
				break;
			}
		}
	}

	printf("check_bind: bind_flag is %d\n", bind_flag);
	
	if (1 == bind_flag) {
		/* check wan if */
		if (!strcmp(wan_if_val, wan_if)) {
			return 0;
		}
	}
	else {
		/* the lan if no bind */
		return 1;
	}
	}
	printf("check_bind():wan interface not match.\n");
	return -1;
}
static int checkUrlFilter(char*requestname,__u32 mark)
{
	struct URLFilter_Info *curNode = NULL;
	__u32 pt_mark = (mark & 0xf0000000) >> 28;
	
	curNode = gURLFilter_Param.next;
	//printf("\r\ncheckUrlFilter:requestname is %s",requestname);
	if(0 == pt_mark  || 0 == whitelistflag)
	{	
		//printf("\r\ncheckUrlFilter:no need check!");
		return 0;
	}
	
	while(NULL != curNode)
	{
		//printf("\r\ncurNode->strUrlName is %s",curNode->strUrlName);
		if(strstr(requestname,curNode->strUrlName))
			return 0;
		curNode = curNode->next;
	}
	return 1;
}

#endif


#endif


