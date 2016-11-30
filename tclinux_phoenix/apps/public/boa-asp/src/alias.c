/*
 *  Boa, an http server
 *  Copyright (C) 1995 Paul Phillips <paulp@go2net.com>
 *  Some changes Copyright (C) 1996 Larry Doolittle <ldoolitt@boa.org>
 *  Some changes Copyright (C) 1996 Russ Nelson <nelson@crynwr.com>
 *  Some changes Copyright (C) 1996-2002 Jon Nelson <jnelson@boa.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 1, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

/* $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/apps/public/boa-asp/src/alias.c#1 $ */

#include "boa.h"
#include "../../lib/libtcapi.h"



#if defined(TCSUPPORT_TRUE_LANDING_PAGE)
#define USER_AGENT_MSN_KEYWORD 	"Windows Live Messenger"
#define HOST_MSN_KEYWORD		"msn."
#define ARP_TABLE_PATH			"/proc/net/arp"
int getMacFromArp(char *macAddr, char *ip);
#endif
static alias *alias_hashtable[ALIAS_HASHTABLE_SIZE];

int get_alias_hash_value(char *file);

/*
 * Name: get_alias_hash_value
 *
 * Description: adds the ASCII values of the file letters
 * and mods by the hashtable size to get the hash value
 * Note: stops at first '/' (or '\0')
 */

int get_alias_hash_value(char *file)
{
    unsigned int hash = 0;
    unsigned int index = 0;
    unsigned char c;

    hash = file[index++];
    while ((c = file[index++]) && c != '/')
        hash += (unsigned int) c;

    return hash % ALIAS_HASHTABLE_SIZE;
}

/*
 * Name: add_alias
 *
 * Description: add an Alias, Redirect, or ScriptAlias to the
 * alias hash table.
 */

void add_alias(char *fakename, char *realname, int type)
{
    int hash;
    alias *old, *new;
    int fakelen, reallen;

    /* sanity checking */
    if (fakename == NULL || realname == NULL) {
        DIE("NULL values sent to add_alias");
    }

    fakelen = strlen(fakename);
    reallen = strlen(realname);
    if (fakelen == 0 || reallen == 0) {
        DIE("empty values sent to add_alias");
    }

    hash = get_alias_hash_value(fakename);

    old = alias_hashtable[hash];

    if (old) {
        while (old->next) {
            if (!strcmp(fakename, old->fakename)) /* don't add twice */
                return;
            old = old->next;
        }
    }

    new = (alias *) malloc(sizeof (alias));
    if (!new) {
        DIE("out of memory adding alias to hash");
    }

    if (old)
        old->next = new;
    else
        alias_hashtable[hash] = new;

    new->fakename = strdup(fakename);
    if (!new->fakename) {
        DIE("failed strdup");
    }
    new->fake_len = fakelen;
    /* check for "here" */
    if (realname[0] == '.' && realname[1] == '/') {
        new->realname = normalize_path(realname+2);
        if (!new->realname) {
            /* superfluous - normalize_path checks for NULL return values. */
            DIE("normalize_path returned NULL");
        }
        reallen = strlen(new->realname);
    } else {
        new->realname = strdup(realname);
        if (!new->realname) {
            DIE("strdup of realname failed");
        }
    }
    new->real_len = reallen;

    new->type = type;
    new->next = NULL;
}

/*
 * Name: find_alias
 *
 * Description: Locates uri in the alias hashtable if it exists.
 *
 * Returns:
 *
 * alias structure or NULL if not found
 */

alias *find_alias(char *uri, int urilen)
{
    alias *current;
    int hash;

    /* Find ScriptAlias, Alias, or Redirect */

    if (urilen == 0)
        urilen = strlen(uri);
    hash = get_alias_hash_value(uri);

    current = alias_hashtable[hash];
    while (current) {
#ifdef FASCIST_LOGGING
        fprintf(stderr,
                "%s:%d - comparing \"%s\" (request) to \"%s\" (alias): ",
                __FILE__, __LINE__, uri, current->fakename);
#endif
        /* current->fake_len must always be:
         *  shorter or equal to the uri
         */
        /*
         * when performing matches:
         * If the virtual part of the url ends in '/', and
         * we get a match, stop there.
         * Otherwise, we require '/' or '\0' at the end of the url.
         * We only check if the virtual path does *not* end in '/'
         */
        if (current->fake_len <= urilen &&
            !memcmp(uri, current->fakename, current->fake_len) &&
            (current->fakename[current->fake_len - 1] == '/' ||
             (current->fakename[current->fake_len - 1] != '/' &&
              (uri[current->fake_len] == '\0' ||
               uri[current->fake_len] == '/')))) {
#ifdef FASCIST_LOGGING
                fprintf(stderr, "Got it!\n");
#endif
                return current;
        }
#ifdef FASCIST_LOGGING
        else
            fprintf(stderr, "Don't Got it!\n");
#endif
        current = current->next;
    }
    return current;
}

#define IP_EQUAL	0
#define IP_NOT_EQUAL	1

/*Check two ip are equal or not. If both are IPv6 ip, 
conver the format and then do compare.*/
int check_ip_equal(char *ipBuf1, char *ipBuf2)	
{
#ifdef INET6
	struct sockaddr_in6 ipv6_addr_1;
	struct sockaddr_in6 ipv6_addr_2;
#endif

	struct sockaddr_in ipv4_addr_1;
	struct sockaddr_in ipv4_addr_2;

	if(inet_aton(ipBuf1, &ipv4_addr_1.sin_addr) 
	&& inet_aton(ipBuf2, &ipv4_addr_2.sin_addr)) {//The two ip both are IPv6 
		if(ipv4_addr_1.sin_addr.s_addr == ipv4_addr_2.sin_addr.s_addr) {
			//fprintf(stderr,"\n==>check_ip_equal, IPv4 ip equal\n");
			return IP_EQUAL;
		}
		else {
			//fprintf(stderr,"\n==>check_ip_equal, IPv4 ip not equal\n");
			return IP_NOT_EQUAL;
		}
	}
#ifdef INET6
	else if( inet_pton(AF_INET6, ipBuf1, (struct sockaddr *) &ipv6_addr_1.sin6_addr) >0
		&& inet_pton(AF_INET6, ipBuf2, (struct sockaddr *) &ipv6_addr_2.sin6_addr) >0) {//The two ip both are IPv6 
			if(memcmp(&ipv6_addr_1.sin6_addr, &ipv6_addr_2.sin6_addr, sizeof(struct in6_addr)) == 0){		
				//fprintf(stderr,"\n==>check_ip_equal, IPv6 ip equal\n");
				return IP_EQUAL;
			}
			else {
				//fprintf(stderr,"\n==>check_ip_equal, IPv6 ip not equal\n");
				return IP_NOT_EQUAL;
			}
	}
#endif
	else {
		//fprintf(stderr,"\n==>check_ip_equal, ip not equal\n");
		return IP_NOT_EQUAL;
	}

	return IP_NOT_EQUAL;
}


#if defined(TCSUPPORT_TRUE_LANDING_PAGE)
#if !defined(TCSUPPORT_CT)
static int isWanUp()
{
        int fd =0;
        int ret=0;
        int upStat=-1;
        char adslState[64];
        
        fd = open("/proc/tc3162/adsl_stats",O_RDONLY|O_NONBLOCK);
        if( fd <= 0 ){
                return upStat;
        }
        ret = read(fd, adslState, 64);
        close(fd);
        if( ret <= 0 ){
                return upStat;
        }
        if(strstr(adslState, "up"))
        {
                upStat = 1;
        }
        else
        {
                upStat = 0;             
        }
        
        return upStat;
}
#endif

static int adslDownJump(request * req , char* tempHostname )
{
#ifdef INET6
        if ( req->hostname && (check_ip_equal(tempHostname, req->local_ip_addr) != IP_EQUAL)  && (strcmp("true.true",req->hostname) != 0))      
#else
        if ( req->hostname && (strcmp(req->local_ip_addr, req->hostname) != 0)  && (strcmp("true.true",req->hostname) != 0))    
#endif
        {
                // adsl down
                if( 0 == isWanUp() )
                {
                        if( (NULL != strstr(req->request_uri,".js")) ||
                        (NULL != strstr(req->request_uri,".gif")) ||
                        (NULL != strstr(req->request_uri,".css")))
                        {
                                return 1;
                        }
                        
                        sprintf(req->request_uri, "/cgi-bin/AdslDownInfo_true.asp");
                        return 1;
                }
        }
        
        return 0;
}

static int isLandingPageEnable()
{
        char buf[20] = "";
        
        memset(buf , 0 ,sizeof(buf));
#if defined(TCSUPPORT_CT)
	if(tcapi_get("Portal_Entry","landingPageEnableRunning",buf) < 0)
#else
        if(tcapi_get("GUI_Entry0","landingPageEnableRunning",buf) < 0)
#endif
        {
                return 0;
        }
        else
        {
                if (strcmp(buf,"Yes") == 0)
                {
                        return 1;
                }
                else
                {
                        return 0;
                }
        }
}

static int isHttpRequest(request * req)
{
	if(req->hostname){
		if((!strcmp(req->hostname, "crl.geotrust.com")) || (!strcmp(req->hostname, "www.gstatic.com")) ||
			(!strcmp(req->hostname, "ocsp.entrust.net")) || !strcmp(req->hostname, "crl.entrust.net") ||
			(!strcmp(req->hostname, "ocsp.digicert.com")) || (strstr(req->hostname, "clients1.google.com"))){
			return 0;
		}
	}
	if (!strcmp(req->request_uri,"/")){
		return 1;
	}
#if !defined(TCSUPPORT_C1_NEW_GUI)
	/* on win7, MSN will send http packet same as brower and with msn keyword in http header:
	 * user_agent and host
	 */
	if(req->header_user_agent){
		if(strstr(req->header_user_agent, USER_AGENT_MSN_KEYWORD))
			return 0;
	}
	if(req->hostname){
		if(strstr(req->hostname, HOST_MSN_KEYWORD))
			return 0;
	}
	/* skype will send http packet without http header: referer & accept_language*/
	if(req->header_referer != NULL){
		return 1;
	}
	if(req->method == M_GET){
		if(req->accept_language != NULL){
			return 1;
		}
	}
#endif
	return 0;
}

#if !defined(TCSUPPORT_CT)
int translate_landingPage_Url(request * req)
{
        char cmdbuf[128] = {0};
        char buf[8] = {0};
        int isManualDisconnect = 0;
	char ipv4string[30] = {0};
        char tempHostname[64] = {0};
	char mac[18];
        
#ifdef INET6
	char ipv6string[64] = {0};
#endif
	memset(mac, 0, sizeof(mac));

#ifdef INET6
        /*Check hostname format*/
        if(req->hostname != NULL) {
                if(req->hostname[0] == '[') {
                        memcpy(tempHostname, req->hostname+1, strlen(req->hostname)-2);
                        tempHostname[strlen(req->hostname)-2] = '\0';
                }
                else {
                        memcpy(tempHostname, req->hostname, strlen(req->hostname));
                        tempHostname[strlen(req->hostname)] = '\0';
                }
        }
#endif

        // when lading page is not enable translate the url the same as the main trunk
        if(1 != isLandingPageEnable())
        {
                if( 1 == adslDownJump( req , tempHostname) )
                {
                        return 0;
                }
                
                goto true_label;
        }
	else
	{
		tcapi_get("Lan_Entry0", "IP", ipv4string);
		#ifdef INET6	  	
		tcapi_get("Lan_Entry0", "IP6", ipv6string);
		#endif
		  if(check_ip_equal(req->local_ip_addr,ipv4string) 
			#ifdef INET6	  	
			&& check_ip_equal(req->local_ip_addr,ipv6string) 
			#endif
		  )
                {
                        if(!strcmp(req->request_uri, "/"))
                        {
                                sprintf(req->request_uri, "/cgi-bin/index.asp");
                        }
                        return 0;		 
                }
                
                if( 1 == adslDownJump( req , tempHostname) )
                {
                        return 0;
                }  
	}

#ifdef INET6
        if ( req->hostname && (check_ip_equal(tempHostname, req->local_ip_addr) != IP_EQUAL)  && (strcmp("true.true",req->hostname) != 0))	
#else
        if ( req->hostname && (strcmp(req->local_ip_addr, req->hostname) != 0)  && (strcmp("true.true",req->hostname) != 0))	
#endif
        {
                if(tcapi_get("System_Entry","manual_disconnected",buf) < 0)
                {
                        isManualDisconnect = 0;
                }
                else
                {
                        if (strcmp(buf,"1") == 0)
                        {
                                isManualDisconnect = 1;
                        }
                        else
                        {
                                isManualDisconnect = 0;
                        }
                }
                /*if in manuall connet mode and now is NOT connected, direct to "connection" page */
                if (isManualDisconnect)
                {        
                         /*do not redirect Logo.gif etc. in wzManualConnect_true.asp*/
                        if (!strstr(req->request_uri,".gif") && !strstr(req->request_uri,".css"))
                        { 
                                        sprintf(req->request_uri, "/connection");
                        }
                }
                 /*if now IS connected, directed to TRUE required page*/
                else 
                {
                        /*as TRUE's requirement*/
                        if (isHttpRequest(req) == 1) 
                        { 			
                                memset(cmdbuf, 0, sizeof(cmdbuf));      
				if(getMacFromArp(mac, req->remote_ip_addr) != 1)
					return 1;			
				snprintf(cmdbuf, sizeof(cmdbuf),"iptables -t nat -D PRE_PORTAL -m mac --mac-source %s -j RETURN", mac);					
                                system(cmdbuf); 
								
                                memset(cmdbuf, 0, sizeof(cmdbuf));      			
                                snprintf(cmdbuf, sizeof(cmdbuf),"iptables -t nat -I PRE_PORTAL -m mac --mac-source %s -j RETURN", mac); 
                                system(cmdbuf);
								
                                send_r_moved_temp(req, "http://landing.trueinternet.co.th/", "");
                        }
                        return 1;
                }
        }

true_label:
	
        if(!strcmp(req->request_uri, "/"))
        {
	    if(req->hostname != NULL)
	    {
                if (strcmp(req->hostname,"true.true") == 0)
                {
                }
                else
                {
                        sprintf(req->request_uri, "/cgi-bin/index.asp");
                }
            }
        }
        else if(!strcmp(req->request_uri, "/connection"))
        {
                sprintf(req->request_uri, "/cgi-bin/wzManualConnect_true.asp");
        } 
        return 0;
}
#endif

int getMacFromArp(char *macAddr, char *ip)
{
	FILE *fp = NULL;
	char buf[100];
	char macp[20];	
	char strbuf[30];
	
	memset(buf, 0, sizeof(buf));
	memset(macp, 0, sizeof(macp));
	memset(strbuf, 0, sizeof(strbuf));

	if (macAddr==NULL || ip==NULL)
		return -1;
		
	fp = fopen(ARP_TABLE_PATH, "r");

	if(fp == NULL){
		return -1;
	}
	
	while(fgets(buf, 100, fp) != NULL){	
#if defined(TCSUPPORT_CT)	
		if((strstr(buf, ip+7) != NULL)||(strstr(buf, ip) != NULL)){
#else
		if(strstr(buf, ip) != NULL){
#endif
			sscanf(buf, "%s %s %s %s %s", strbuf, strbuf, strbuf, macp, strbuf);
			strncpy(macAddr, macp, 17);
			fclose(fp);
			return 1;
		}
		memset(buf,0,sizeof(buf));
	}
	fclose(fp);
	return 0;
}
#endif


#if defined(TCSUPPORT_TRUE_LANDING_PAGE)
#if defined(TCSUPPORT_CT)
static int isWanUp()
{
        int fd =0;
        int ret=0;
        int upStat=-1;
        char adslState[64];
        
        fd = open("/proc/tc3162/adsl_stats",O_RDONLY|O_NONBLOCK);
        if( fd <= 0 ){
                return upStat;
        }
        ret = read(fd, adslState, 64);
        close(fd);
        if( ret <= 0 ){
                return upStat;
        }
        if(strstr(adslState, "up"))
        {
                upStat = 1;
        }
        else
        {
                upStat = 0;
        }
        
        return upStat;
}


int translate_landingPage_Url(request * req)
{
	char cmdbuf[128] = {0};
	char buf[8] = {0};
	char ipv4string[30] = {0};
	char tempHostname[64] = {0};
	char mac[18] = {0};
	int isManualDisconnect = 0;        
#ifdef INET6
	char ipv6string[64] = {0};
#endif
	char urlValue[256]={0};

#ifdef INET6
        /*Check hostname format*/
	if(req->hostname != NULL) {
		if(req->hostname[0] == '[') {
			memcpy(tempHostname, req->hostname+1, strlen(req->hostname)-2);
			tempHostname[strlen(req->hostname)-2] = '\0';
		}
		else {
			memcpy(tempHostname, req->hostname, strlen(req->hostname));
			tempHostname[strlen(req->hostname)] = '\0';
		}
	}
#endif

        // when lading page is not enable translate the url the same as the main flow
	if(1 != isLandingPageEnable()){
		goto true_label;
	}
	else{
		tcapi_get(LAN_NODE, IPV4_ATTR, ipv4string);
#ifdef INET6	  	
		tcapi_get(LAN_NODE, IPV6_ATTR, ipv6string);
#endif
		if((NULL == strstr(req->local_ip_addr,ipv4string))
#ifdef INET6	  	
		&& (NULL == strstr(req->local_ip_addr,ipv6string))
#endif
		){
			if(!strcmp(req->request_uri, "/")){
				sprintf(req->request_uri, "/cgi-bin/index.asp");
			}
			return 0;		 
		}
		if( 0 == isWanUp() )
		{
			return 0;
		}

	}

#ifdef INET6
        if ( req->hostname && (strstr( req->local_ip_addr,tempHostname) == NULL) )	
#else
        if ( req->hostname && (strstr(req->local_ip_addr, req->hostname) == NULL))	
#endif
        {
		/*as TRUE's requirement*/
		if (isHttpRequest(req) == 1) { 			
			memset(cmdbuf, 0, sizeof(cmdbuf));    
			if(getMacFromArp(mac, req->remote_ip_addr) != 1)
				return 1;

			snprintf(cmdbuf, sizeof(cmdbuf),"iptables -t nat -D PRE_PORTAL -m mac --mac-source %s -j RETURN", mac);					
			system(cmdbuf); 
								
			memset(cmdbuf, 0, sizeof(cmdbuf));      			
			snprintf(cmdbuf, sizeof(cmdbuf),"iptables -t nat -I PRE_PORTAL -m mac --mac-source %s -j RETURN", mac); 
			system(cmdbuf);
						
			tcapi_get(PORTAL_NODE,REDIRECTURL,urlValue);
			send_r_moved_temp(req, urlValue, "");
		}
		return 1;
	}

true_label:
	
        if(!strcmp(req->request_uri, "/")){
        	sprintf(req->request_uri, "/cgi-bin/index.asp");
        }
      
        return 0;
}
#endif
#endif

/*
 * Name: translate_uri
 *
 * Description: Parse a request's virtual path.
 * Sets query_string, pathname directly.
 * Also sets path_info, path_translated, and script_name via
 *  init_script_alias
 *
 * Note: NPH in user dir is currently broken
 *
 * Note -- this should be broken up.
 *
 * Return values:
 *   0: failure, close it down
 *   1: success, continue
 */

int translate_uri(request * req)
{
    static char buffer[MAX_HEADER_LENGTH + 1];
    char *req_urip;
    alias *current;
    char *p;
    int uri_len;

    req_urip = req->request_uri;
    if (req_urip[0] != '/') {
        send_r_bad_request(req);
        return 0;
    }


#if defined(TCSUPPORT_TRUE_LANDING_PAGE)
        // modified the req>request_uri for landing page
        if(1 == translate_landingPage_Url(req))
        {
                return 0;
        }
#endif
#if defined(TCSUPPORT_CUC) || defined(TCSUPPORT_CT_PON_JX)
	if( 0 == strcmp(req->request_uri, "/") )
		strcpy(req->request_uri, "/index.html");
#endif


#if !defined(TCSUPPORT_TRUE_LANDING_PAGE) && !defined(TCSUPPORT_REDIRECT_WITH_PORTMASK) && !defined(TCSUPPORT_CZ_OTE)
        // maintrunk req>request_uri translate
        if(!strcmp(req->request_uri, "/"))
	{
                sprintf(req->request_uri, "/cgi-bin/index.asp");
	}

#endif

#if defined(TCSUPPORT_SUPPORT_FLASH)	
        if(!strcmp(req->request_uri, "/test_version.asp"))
	{
                sprintf(req->request_uri, "/cgi-bin/test_version.cgi");
	}
#endif 


#if defined(TCSUPPORT_CT_PON_CY) && defined(TCSUPPORT_CT_PON_JS)
	if(req->query_string && !strcmp(req->request_uri, "/itms"))
	{
		tcapi_set("WebCustom_Entry","querystring", req->query_string);
		sprintf(req->request_uri, "/cgi-bin/itms.cgi");					
	}
#endif

#if defined(TCSUPPORT_CT_PON_CZ_NX)
	if(req->query_string && !strcmp(req->request_uri, "/itms"))
	{
		tcapi_set("WebCustom_Entry","querystring", req->query_string);
		sprintf(req->request_uri, "/cgi-bin/itms.cgi"); 				
	}
#endif

#if defined(TCSUPPORT_CT_PON_CN_CN)
	if(req->query_string && !strcmp(req->request_uri, "/telnet"))
	{
		tcapi_set("WebCustom_Entry","querystring", req->query_string);
		sprintf(req->request_uri, "/cgi-bin/telnet.cgi");					
	}
#endif
    uri_len = strlen(req->request_uri);

    current = find_alias(req->request_uri, uri_len);
    if (current) {

        if (current->type == SCRIPTALIAS) /* Script */
            return init_script_alias(req, current, uri_len);

        /* not a script alias, therefore begin filling in data */

        {
            int len;
            len = current->real_len;
            len += uri_len - current->fake_len;
            if (len > MAX_HEADER_LENGTH) {
                log_error_doc(req);
                fputs("uri too long!\n", stderr);
                send_r_bad_request(req);
                return 0;
            }
            memcpy(buffer, current->realname, current->real_len);
            memcpy(buffer + current->real_len,
                   req->request_uri + current->fake_len,
                   uri_len - current->fake_len + 1);
        }

        if (current->type == REDIRECT) { /* Redirect */
            if (req->method == M_POST) { /* POST to non-script */
                /* it's not a cgi, but we try to POST??? */
                send_r_bad_request(req);
                return 0;       /* not a script alias, therefore begin filling in data */
            }
            send_r_moved_temp(req, buffer, "");
            return 0;
        } else {                /* Alias */
            req->pathname = strdup(buffer);
            if (!req->pathname) {
                send_r_error(req);
                WARN("unable to strdup buffer onto req->pathname");
                return 0;
            }
            return 1;
        }
    }

    /*
       The reason why this is *not* an 'else if' is that,
       after aliasing, we still have to check for '~' expansion
     */

    if (user_dir && req->request_uri[1] == '~') {
        char *user_homedir;

        req_urip = req->request_uri + 2;

        /* since we have uri_len which is from strlen(req->request_uri) */
        p = memchr(req_urip, '/', uri_len - 2);
        if (p)
            *p = '\0';

        user_homedir = get_home_dir(req_urip);
        if (p)                  /* have to restore request_uri in case of error */
            *p = '/';

        if (!user_homedir) {    /*no such user */
            send_r_not_found(req);
            return 0;
        }
        {
            int l1 = strlen(user_homedir);
            int l2 = strlen(user_dir);
            int l3 = (p ? strlen(p) : 0);

            if (l1 + l2 + l3 + 1 > MAX_HEADER_LENGTH) {
                log_error_doc(req);
                fputs("uri too long!\n", stderr);
                send_r_bad_request(req);
                return 0;
            }

            memcpy(buffer, user_homedir, l1);
            buffer[l1] = '/';
            memcpy(buffer + l1 + 1, user_dir, l2 + 1);
            if (p)
                memcpy(buffer + l1 + 1 + l2, p, l3 + 1);
        }
    } else if (document_root) {
        /* no aliasing, no userdir... */
        int l1, l2, l3;

        l1 = strlen(document_root);
        l2 = strlen(req->request_uri);
        if (virtualhost)
            l3 = strlen(req->local_ip_addr);
        else
            l3 = 0;

        if (l1 + l2 + l3 + 1 > MAX_HEADER_LENGTH) {
            log_error_doc(req);
            fputs("uri too long!\n", stderr);
            send_r_bad_request(req);
            return 0;
        }

        /* the 'l2 + 1' is there so we copy the '\0' as well */
        memcpy(buffer, document_root, l1);
        if (virtualhost) {
            buffer[l1] = '/';
            memcpy(buffer + l1 + 1, req->local_ip_addr, l3);
            memcpy(buffer + l1 + 1 + l3, req->request_uri, l2 + 1);
        } else
            memcpy(buffer + l1, req->request_uri, l2 + 1);
    } else {
        /* not aliased.  not userdir.  not part of document_root.  BAIL */
        send_r_not_found(req);
        return 0;
    }

    /* if here,
     * o it may be aliased but it's not a redirect or a script...
     * o it may be a homedir
     * o it may be a document_root resource (with or without virtual host)
     */

    req->pathname = strdup(buffer);
    if (!req->pathname) {
        WARN("Could not strdup buffer for req->pathname!");
        send_r_error(req);
        return 0;
    }

    /* below we support cgis outside of a ScriptAlias */
    if (strcmp(CGI_MIME_TYPE, get_mime_type(buffer)) == 0) { /* cgi */
#ifdef FASCIST_LOGGING
        log_error_time();
        fprintf(stderr, "%s:%d - buffer is: \"%s\"\n",
                __FILE__, __LINE__, buffer);
#endif
        /* FIXME */
        /* script_name could end up as /cgi-bin/bob/extra_path */
        req->script_name = strdup(req->request_uri);
        if (!req->script_name) {
            WARN("Could not strdup req->request_uri for req->script_name");
            send_r_error(req);
            return 0;
        }
        if (req->simple)
            req->is_cgi = NPH;
        else
            req->is_cgi = CGI;
        return 1;
    } else if (req->method == M_POST) { /* POST to non-script */
        /* it's not a cgi, but we try to POST??? */
        send_r_bad_request(req);
        return 0;
    } else                      /* we are done!! */
        return 1;
}

/*
 * Name: init_script_alias
 *
 * Description: Performs full parsing on a ScriptAlias request
 * Sets path_info and script_name
 *
 * Return values:
 *
 * 0: failure, shut down
 * 1: success, continue
 */

int init_script_alias(request * req, alias * current1, int uri_len)
{
    static char pathname[MAX_HEADER_LENGTH + 1];
    struct stat statbuf;
    static char buffer[MAX_HEADER_LENGTH + 1];

    int index = 0;
    char c;
    int err;

    /* copies the "real" path + the non-alias portion of the
       uri to pathname.
     */

    if (uri_len - current1->fake_len + current1->real_len >
        MAX_HEADER_LENGTH) {
        log_error_doc(req);
        fputs("uri too long!\n", stderr);
        send_r_bad_request(req);
        return 0;
    }

    memcpy(pathname, current1->realname, current1->real_len);
    memcpy(pathname + current1->real_len,
           &req->request_uri[current1->fake_len],
           uri_len - current1->fake_len + 1); /* the +1 copies the NUL */
#ifdef FASCIST_LOGGING
    log_error_time();
    fprintf(stderr,
            "%s:%d - pathname in init_script_alias is: \"%s\" (\"%s\")\n",
            __FILE__, __LINE__, pathname, pathname + current1->real_len);
#endif
    if (strncmp("nph-", pathname + current1->real_len, 4) == 0
        || req->simple) req->is_cgi = NPH;
    else
        req->is_cgi = CGI;


    /* start at the beginning of the actual uri...
     (in /cgi-bin/bob, start at the 'b' in bob */
    index = current1->real_len;

    /* go to first and successive '/' and keep checking
     * if it is a full pathname
     * on success (stat (not lstat) of file is a *regular file*)
     */
    do {
        c = pathname[++index];
        if (c == '/') {
            pathname[index] = '\0';
            err = stat(pathname, &statbuf);
            pathname[index] = '/';
            if (err == -1) {
                send_r_not_found(req);
                return 0;
            }

            /* is it a dir? */
            if (!S_ISDIR(statbuf.st_mode)) {
                /* check access */
                if (!(statbuf.st_mode &
                      (S_IFREG | /* regular file */
                       (S_IRUSR | S_IXUSR) |    /* u+rx */
                       (S_IRGRP | S_IXGRP) |    /* g+rx */
                       (S_IROTH | S_IXOTH)))) { /* o+rx */
                    send_r_forbidden(req);
                    return 0;
                }
                /* stop here */
                break;
            }
        }
    } while (c != '\0');

    req->script_name = strdup(req->request_uri);
    if (!req->script_name) {
        send_r_error(req);
        WARN("unable to strdup req->request_uri for req->script_name");
        return 0;
    }

    if (c == '\0') {
        err = stat(pathname, &statbuf);
        if (err == -1) {
            send_r_not_found(req);
            return 0;
        }

        /* is it a dir? */
        if (!S_ISDIR(statbuf.st_mode)) {
            /* check access */
            if (!(statbuf.st_mode &
                  (S_IFREG | /* regular file */
                   (S_IRUSR | S_IXUSR) |    /* u+rx */
                   (S_IRGRP | S_IXGRP) |    /* g+rx */
                   (S_IROTH | S_IXOTH)))) { /* o+rx */
                send_r_forbidden(req);
                return 0;
            }
            /* stop here */
        } else {
            send_r_forbidden(req);
            return 0;
        }
    }

    /* we have path_info if c == '/'... still have to check for query */
    else if (c == '/') {
        int hash;
        alias *current;
        int path_len;

        req->path_info = strdup(pathname + index);
        if (!req->path_info) {
            send_r_error(req);
            WARN("unable to strdup pathname + index for req->path_info");
            return 0;
        }
        pathname[index] = '\0'; /* strip path_info from path */
        path_len = strlen(req->path_info);
        /* we need to fix script_name here */
        /* index points into pathname, which is
         * realname/cginame/foo
         * and index points to the '/foo' part
         */
        req->script_name[strlen(req->script_name) - path_len] = '\0'; /* zap off the /foo part */

        /* now, we have to re-alias the extra path info....
           this sucks.
         */
        hash = get_alias_hash_value(req->path_info);
        current = alias_hashtable[hash];
        while (current && !req->path_translated) {
            if (!strncmp(req->path_info, current->fakename,
                         current->fake_len)) {

                if (current->real_len +
                    path_len - current->fake_len > MAX_HEADER_LENGTH) {
                    log_error_doc(req);
                    fputs("uri too long!\n", stderr);
                    send_r_bad_request(req);
                    return 0;
                }

                memcpy(buffer, current->realname, current->real_len);
                strcpy(buffer + current->real_len,
                       &req->path_info[current->fake_len]);
                req->path_translated = strdup(buffer);
                if (!req->path_translated) {
                    send_r_error(req);
                    WARN("unable to strdup buffer for req->path_translated");
                    return 0;
                }
            }
            current = current->next;
        }
        /* no alias... try userdir */
        if (!req->path_translated && user_dir && req->path_info[1] == '~') {
            char *user_homedir;
            char *p;

            p = strchr(pathname + index + 1, '/');
            if (p)
                *p = '\0';

            user_homedir = get_home_dir(pathname + index + 2);
            if (p)
                *p = '/';

            if (!user_homedir) { /* no such user */
                send_r_not_found(req);
                return 0;
            }
            {
                int l1 = strlen(user_homedir);
                int l2 = strlen(user_dir);
                int l3;
                if (p)
                    l3 = strlen(p);
                else
                    l3 = 0;

                req->path_translated = malloc(l1 + l2 + l3 + 2);
                if (req->path_translated == NULL) {
                    send_r_error(req);
                    WARN("unable to malloc memory for req->path_translated");
                    return 0;
                }
                memcpy(req->path_translated, user_homedir, l1);
                req->path_translated[l1] = '/';
                memcpy(req->path_translated + l1 + 1, user_dir, l2 + 1);
                if (p)
                    memcpy(req->path_translated + l1 + 1 + l2, p, l3 + 1);
            }
        }
        if (!req->path_translated && document_root) {
            /* no userdir, no aliasing... try document root */
            int l1, l2;
            l1 = strlen(document_root);
            l2 = path_len;

            req->path_translated = malloc(l1 + l2 + 1);
            if (req->path_translated == NULL) {
                send_r_error(req);
                WARN("unable to malloc memory for req->path_translated");
                return 0;
            }
            memcpy(req->path_translated, document_root, l1);
            memcpy(req->path_translated + l1, req->path_info, l2 + 1);
        }
    }

    req->pathname = strdup(pathname);
    if (!req->pathname) {
        send_r_error(req);
        WARN("unable to strdup pathname for req->pathname");
        return 0;
    }

    return 1;
}

/*
 * Empties the alias hashtable, deallocating any allocated memory.
 */

void dump_alias(void)
{
    int i;
    alias *temp;

    for (i = 0; i < ALIAS_HASHTABLE_SIZE; ++i) { /* these limits OK? */
        if (alias_hashtable[i]) {
            temp = alias_hashtable[i];
            while (temp) {
                alias *temp_next;

                if (temp->fakename)
                    free(temp->fakename);
                if (temp->realname)
                    free(temp->realname);
                temp_next = temp->next;
                free(temp);
                temp = temp_next;
            }
            alias_hashtable[i] = NULL;
        }
    }
}
