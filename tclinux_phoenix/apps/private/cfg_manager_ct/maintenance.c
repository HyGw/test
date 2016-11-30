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
#include <crypt.h>
#include <sys/time.h>
#include <time.h>
#include "maintenance.h"
#include "utility.h"
//#include "../lib/libtcapi.h"

#ifdef TCSUPPORT_SYSLOG
#include <syslog.h>
#endif
#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC) || defined(TCSUPPORT_EPON_OAM_CUC)
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#endif
#define ACCOUNTNUM	0x03

#ifdef CT_COM_DEVICEREG
/* prev flag for device registration*/
static int prevDevRegFlag;
#endif

#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
int isTimeZoneCommit = 0;
#endif


int ntp_server_ret=-1;
#if defined(TCSUPPORT_CT_2NTP)
#if defined(TCSUPPORT_CT_NTPSERVERTYPE)
extern unsigned int ntpdelayflag1_all[4];
extern unsigned int ntpdelayflag2_all[4];
#endif
#endif

#if 0  /*Orginal ci-command mode */
const char
account_attribute[][ATTR_SIZE]=
{
	{"web_passwd"},
	{"console_passwd"},
	{""},
};

const char
timezone_attribute[][ATTR_SIZE]=
{
	{"TZ"},
	{"DAYLIGHT"},
	{"SERVER"},
	{""},
};

/*mac information will be removed from romfile configureation file*/
const char
mac_attribute[][ATTR_SIZE]=
{
	{"WAN_MAC"},
	{"LAN_MAC"},
	{""},
};

/*led information will be removed from romfile configureation file*/
const char
led_attribute[][ATTR_SIZE]=
{
	{"LAN"},
	{""},
};
#endif
#if 0  /*Orginal ci-command mode */
/*______________________________________________________________________________
**	account_read
**
**	descriptions:
**		Read account information from configuration file and record those information
**		to mxml_node_t.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
account_read(mxml_node_t *top){
	int retval=0;
	char *pAttr=NULL;
	pAttr=(char *)account_attribute;

	retval=node_read(top,ACCOUNT, ACCOUNT_PATH, pAttr,NO_QMARKS);
	return retval;
}/* end account_read */
#endif

#if defined(TCSUPPORT_CT_NETWORKMANAGESERVICE)
char *account_merge_table[MAX_NODE_NAME]=
{
	"account_id","",
};

char *account_adv_table[MAX_NODE_NAME]=
{
	"TelnetEntry",
	"FtpEntry",
#if defined(TCSUPPORT_CT_PON_SC)
	"SshEntry",
	"TftpEntry",
	"SnmpEntry",
#endif
#if defined(TCSUPPORT_CT_DEFAULTPARA_STORE)
#ifndef TCSUPPORT_RESERVEAREA_EXTEND
	"CTDefParaEntry",
#endif
#endif
	"ConsoleEntry",
	"",
};
#endif
//"Account",	SINGLE_TYPE,	NULL, NULL,NULL, account_write, account_verify, NULL},
/*______________________________________________________________________________
**	account_init
**
**	descriptions:
**		the init function of account.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
account_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="Account",
#if defined(TCSUPPORT_CT_NETWORKMANAGESERVICE)	
			.type=ADV_FLAG|ENTRY_TYPE|ACCOUNTNUM,
			.adv_table=account_adv_table,
			.merge_table=account_merge_table,
#else
			.type=ENTRY_TYPE|ACCOUNTNUM,
			.adv_table=NULL,
			.merge_table=NULL,
#endif
			.cfg_read=NULL,
			.cfg_write=account_write,
			.cfg_verify=account_verify,
#if defined(TCSUPPORT_CT_NETWORKMANAGESERVICE)		
			.cfg_execute=account_execute,
			.cfg_boot=account_boot,	
#else
			.cfg_execute=NULL,
			.cfg_boot=NULL,	
#endif
	};
	ret=register_cfg_node(&node);
	return ret;
}

#if defined(TCSUPPORT_CT_NETWORKMANAGESERVICE)
int parseftpport(const char *port)
{
	FILE *servicesfp=NULL;
	FILE *servicesfptmp=NULL;
	char servicestr[128]={0};
	char servicestrtmp[128]={0};
	char *ptr = NULL;
	char *ptrbegin = NULL;
	char *ptrend = NULL;
	char porttmp[10] = {0};
	char tmp[50] = {0};
	char buf[100] = {0};
	
	servicesfp=fopen(FTPSERVER_FILE_PATH, "r");

	if(servicesfp)
	{
		servicesfptmp = fopen(FTPSERVER_FILE_PATH_TMP, "w");
		if(!servicesfptmp)
		{
			fclose(servicesfp);
			tcdbg_printf("parseftpport:open file(%s) error!",FTPSERVER_FILE_PATH_TMP);
			return -1;
		}
		while(fgets(servicestr, 128, servicesfp))
		{
			if(servicestr[0] != '#' && strstr(servicestr,"PORT="))
			{
				ptr = servicestr;
				ptrbegin = strchr(servicestr,'\"');
				ptrend = strrchr(servicestr,'\"');
				if(!ptrbegin ||!ptrend ||ptrbegin == ptrend)
				{
					tcdbg_printf("\r\nparseftpport:parse or file error!");
					fclose(servicesfp);
					fclose(servicesfptmp);
					return -1;
				}
				strncpy(servicestrtmp,ptr,ptrbegin-ptr);
				strncpy(porttmp,ptrbegin+1,ptrend-ptrbegin-1);
				sprintf(buf,"echo %s >/proc/sys/net/netfilter/nf_conntrack_ftp_port",port);
				system(buf);
				if(!strcmp(porttmp,port))
				{
					//tcdbg_printf("parseftpport:the port is the same as before!");
					fclose(servicesfptmp);
					fclose(servicesfp);
					unlink(FTPSERVER_FILE_PATH_TMP);
					return 0;
				}
				else
				{
					sprintf(tmp,"\"%s\"\n",port);
					strcat(servicestrtmp,tmp);
					tcdbg_printf("\r\nthe new string is %s",servicestrtmp);
					fputs(servicestrtmp, servicesfptmp);
					continue;		
				}			
			}
			fputs(servicestr, servicesfptmp);
		}
		fclose(servicesfptmp);
		fclose(servicesfp);
		//renname
		unlink(FTPSERVER_FILE_PATH);
		rename(FTPSERVER_FILE_PATH_TMP, FTPSERVER_FILE_PATH);
		unlink(FTPSERVER_FILE_PATH_TMP);
		return 0;
	}
	else
	{

		fprintf(stderr, "parseftpport:open file(%s) error!",FTPSERVER_FILE_PATH);
		return -1;
	}
}
/*______________________________________________________________________________
**	account_write
**
**	descriptions:
**		Find account element from mxml tree then create two password file, passwd file : It is
**		used for web/ftp account. usertty file: It is used for console/telnet account.
**
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  account cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
account_write(mxml_node_t *top, mxml_node_t *parant){

	int retval=0;
	int i=0;
	mxml_node_t *webnode[3] = {NULL, NULL, NULL};
	mxml_node_t *tmpnode = NULL;
	char *cpwd = NULL;
	char pwd_buf[256] = {0}; 
	char username[129] = {0};
	FILE *fp = NULL;
	const char *tmpattr = NULL;
	
	for(i = 0; i < 3; i++)
	{
		char Entry[7];
		sprintf(Entry, "Entry%d", i);
		webnode[i] = mxmlFindElement(parant, parant, Entry,
									NULL, NULL, MXML_DESCEND);
		if(webnode[i]==NULL)
		{
			tcdbg_printf("\r\n%s not found!", Entry);
			return FAIL;
		}
	}

	//operate web login info
	/*Create passwd file */
	fp = fopen(WEBPASSWD_PATH, "w");
	if(fp == NULL)
	{
		tcdbg_printf("\r\naccount_write:create file(%s) fail!", WEBPASSWD_PATH);
		return FAIL;
	}

	for(i = 0; i < 3; i++ )
	{
		memset(pwd_buf, 0, sizeof(pwd_buf));
		memset(username, 0, sizeof(username));
		tmpattr = mxmlElementGetAttr(webnode[i], "Active");
		if(tmpattr == NULL)
		{
			tcdbg_printf("\r\nAttribute Active not fetch value!");
			fclose(fp);
			return FAIL;
		}
		if(strcmp(tmpattr,"Yes"))
		{
			tcdbg_printf("\r\naccount is deactive!");
			continue;
		}
		tmpattr = mxmlElementGetAttr(webnode[i], "username");
		if(tmpattr == NULL)
		{
			tcdbg_printf("\r\nAttribute username not fetch value!");
			fclose(fp);
			return FAIL;
		}
		sprintf(username, "%s", tmpattr);
		tmpattr = mxmlElementGetAttr(webnode[i], "web_passwd");
		if(tmpattr == NULL)
		{
			tcdbg_printf("\r\nAttribute %s not fetch value!",username);
			fclose(fp);
			return FAIL;
		}
		cpwd = crypt(tmpattr, SLAT);
		sprintf(pwd_buf, ACCOUNT_DEFAULT_STRING,username, cpwd);
		fputs(pwd_buf, fp);
	}
		
	fclose(fp);
	
	//operate telnet info
	memset(pwd_buf, 0, sizeof(pwd_buf));
	memset(username, 0, sizeof(username));
	tmpnode = mxmlFindElement(parant, parant, "TelnetEntry",NULL, NULL, MXML_DESCEND);
	if(tmpnode == NULL)
	{
		tcdbg_printf("\r\nTelnetEntry not found!");
		return FAIL;
	}

	tmpattr = mxmlElementGetAttr(tmpnode, "telnet_username");
	if(tmpattr == NULL)
	{
		tcdbg_printf("\r\nAttribute %s not fetch value!",username);
		return FAIL;
	}
	sprintf(username, "%s", tmpattr);
	tmpattr = mxmlElementGetAttr(tmpnode, "telnet_passwd");
	if(tmpattr == NULL)
	{
		tcdbg_printf("\r\nAttribute %s not fetch value!",username);
		return FAIL;
	}
	cpwd = crypt(tmpattr, SLAT);
	sprintf(pwd_buf, ACCOUNT_DEFAULT_STRING,username, cpwd);

	/*Create usertty file,store username and passwd*/
	if(write2file(pwd_buf, CONSOLE_PATH) == FAIL)
	{
		tcdbg_printf("\r\naccount_write:create file(%s) fail!", CONSOLE_PATH);
		return FAIL;
	}

	//operate ftp info
	memset(pwd_buf, 0, sizeof(pwd_buf));
	memset(username, 0, sizeof(username));
	tmpnode = mxmlFindElement(parant, parant, "FtpEntry",NULL, NULL, MXML_DESCEND);
	if(tmpnode == NULL)
	{
		tcdbg_printf("\r\nFtpEntry not found!");
		return FAIL;
	}
	tmpattr = mxmlElementGetAttr(tmpnode, "ftp_username");
	if(tmpattr == NULL)
	{
		tcdbg_printf("\r\nAttribute %s not fetch value!",username);
		return FAIL;
	}
	sprintf(username, "%s", tmpattr);
	tmpattr = mxmlElementGetAttr(tmpnode, "ftp_passwd");
	if(tmpattr == NULL)
	{
		tcdbg_printf("\r\nAttribute %s not fetch value!",username);
		return FAIL;
	}
	cpwd = crypt(tmpattr, SLAT);
	sprintf(pwd_buf, ACCOUNT_DEFAULT_STRING,username, cpwd);
	
	/*Create ftp file,store username and passwd*/
	if(write2file(pwd_buf, FTPPASSWD_PATH) == FAIL)
	{
		tcdbg_printf( "\r\naccount_write:create file(%s) fail!", FTPPASSWD_PATH);
		return FAIL;
	}
	
	/*modify ftp port*/
	tmpattr = mxmlElementGetAttr(tmpnode, "ftp_port");
	if(tmpattr == NULL)
	{
		tcdbg_printf("\r\nAttribute ftp_port not fetch value!");
		return FAIL;
	}
	if(parseftpport(tmpattr) < 0)
	{
		tcdbg_printf("\r\naccount_write:parse ftp port fail!");
		return FAIL;
	}

	//operate console info
	memset(pwd_buf, 0, sizeof(pwd_buf));
	memset(username, 0, sizeof(username));
	tmpnode = mxmlFindElement(parant, parant, "ConsoleEntry",NULL, NULL, MXML_DESCEND);
	if(tmpnode == NULL)
	{
		tcdbg_printf("\n ConsoleEntry not found!");
		return FAIL;
	}
	tmpattr = mxmlElementGetAttr(tmpnode, "console_username");
	if(tmpattr == NULL)
	{
		tcdbg_printf("\n Attribute console_username not fetch value!");
		return FAIL;
	}
	sprintf(username, "%s", tmpattr);
	tmpattr = mxmlElementGetAttr(tmpnode, "console_passwd");
	if(tmpattr == NULL)
	{
		tcdbg_printf("\n Atrribute console_passwd not fetch value!" );
		return FAIL;
	}
	cpwd = crypt(tmpattr, SLAT);
	sprintf(pwd_buf, ACCOUNT_DEFAULT_STRING,username, cpwd);

	/*Create usertty file,store username and passwd*/
	if(write2file(pwd_buf, CONSOLE_PATH2) == FAIL)
	{
		tcdbg_printf("\n Account_write:create file(%s) fail!", CONSOLE_PATH2);
		return FAIL;
	}

	
	return retval;
}/* end account_write */
#else
int
account_write(mxml_node_t *top, mxml_node_t *parant){

	int retval=0;
	int i=0;
	mxml_node_t *node[3] = {NULL, NULL, NULL};
	mxml_attr_t *attr = NULL;
	char *cpwd=NULL;
	char pwd_buf[4][256]; /*pwd_buf[0] to pwd_buf[2]= web password
						    pwd_buf[3]= console_password */
	char *username[3][129];
	FILE *fp = NULL;

	/*
	node = mxmlFindElement(tree, tree, ACCOUNT_ENTRY,
		NULL, NULL,
		MXML_DESCEND);
	*/
	for(i = 0; i < 3; i++){
		char Entry[7];

		sprintf(Entry, "Entry%d", i);
		node[i] = mxmlFindElement(parant, parant, Entry,
									NULL, NULL, MXML_DESCEND);
		if(node[i]==NULL){
			fprintf(stderr, "%s not found!", Entry);
			return FAIL;
		}
	}

	memset(pwd_buf, 0, sizeof(pwd_buf));
#if 0 // /etc/passwd.conf is no longer used
	retval = node_write(node, ACCOUNT_PATH, NO_QMARKS);

	if(retval == 0){
#endif
		for(i = 0; i < 3; i++ ){
			attr = node[i]->value.element.attrs;
			sprintf(username[i], "%s", attr->value);
			attr++;
			cpwd = crypt(attr->value, SLAT);
			sprintf(pwd_buf[i], ACCOUNT_DEFAULT_STRING, username[i], cpwd);
		}
		attr = node[0]->value.element.attrs;
		attr = attr + 2;
		cpwd = crypt(attr->value, SLAT);
		sprintf(pwd_buf[3], ACCOUNT_DEFAULT_STRING, "admin", cpwd);
#if 0
	}
	else{
		return FAIL;
	}

#endif
	/*Create passwd file */
#if 0	
	if(write2file(pwd_buf[ACCOUNT_WEB_INDEX], WEBPASSWD_PATH) == FAIL){
		return FAIL;
	}
#else
	fp = fopen(WEBPASSWD_PATH, "w");

	if(fp == NULL){
		return FAIL;
	}

	for(i = 0; i < 3; i++)
		fputs(pwd_buf[i], fp);
	fclose(fp);
#endif	

	/*Create usertty file*/
	if(write2file(pwd_buf[3], CONSOLE_PATH) == FAIL){
		return FAIL;
	}

	return retval;
}/* end account_write */

#endif
/*______________________________________________________________________________
**	account_verify
**
**	descriptions:
**		To check the system parameter of account element  is validly or not.
**
**	parameters:
**		node: 		Specify the root of mxml_node_t structure.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
account_verify(mxml_node_t *node){
	return 0;
}/* end account_verify */

#if defined(TCSUPPORT_CT_NETWORKMANAGESERVICE)
int
account_execute(mxml_node_t *top,char name[][MAX_NODE_NAME]){
	char active[10] = {0};
	char portbuf[10] = {0};
	char cmdbuf[100] = {0};
	char pidbuf[64] = {0};
	char pidallbuf[128] = {0};
	int portnum = 0;

	if(getAttrValue(top, name, ACCOUNT_NODE_ACTIVE, active) != TCAPI_PROCESS_OK )
	{
		goto getattrerror;
	}

	//if operate web info,do nothing just return 0
	if(!strcmp(name[1],FTP_ENTRY))
	{
		system("killall -9 bftpd");
		if(!strcmp(active,ACTIVE))
		{
			system("/userfs/bin/bftpd -d");
		}
	}
	else if(!strcmp(name[1],TELNET_ENTRY))
	{
		system("killall -9 utelnetd");
		if(!strcmp(active,ACTIVE))
		{
			if(getAttrValue(top, name, "telnet_port", portbuf) != TCAPI_PROCESS_OK)
			{
				goto getattrerror;
			}
			else
			{
				portnum = atoi(portbuf);
				if(portnum == 0)
				{
					strcpy(portbuf,TELNETDEFAULTPORT);
					portnum = 23;
				}
				sprintf(cmdbuf,"/usr/bin/utelnetd -p %d -l /bin/login -d",portnum);
				system(cmdbuf);
			}
		}
	}
	else if( 0 == strcmp(name[1], CONSOLE_ENTRY) )
	{
		memset(pidbuf, 0, sizeof(pidbuf));
		fileRead(CONSOLE_PID_PATH, pidbuf, sizeof(pidbuf) -1);
		if ( '\0' != pidbuf[0] )
		{
			memset(cmdbuf, 0, sizeof(cmdbuf));
			snprintf(cmdbuf, sizeof(cmdbuf), "pidof sh > %s", CONSOLE_PID_TEMP_PATH);
			system(cmdbuf);
			fileRead(CONSOLE_PID_TEMP_PATH, pidallbuf, sizeof(pidallbuf) -1);
			unlink(CONSOLE_PID_TEMP_PATH);
			if ( NULL != strstr(pidallbuf, pidbuf) )
				kill_process(CONSOLE_PID_PATH);
		}
	}

	else
	{
		#ifdef TCSUPPORT_SYSLOG
		char ip[32];
		char times[4];
		char log[128];
		if(getAttrValue(top, name, "LoginTimes", times) != TCAPI_PROCESS_OK)
		{
			goto getattrerror;
		}		
		if(!strcmp(times,"3"))
		{
			if(getAttrValue(top, name, "LoginIp", ip) != TCAPI_PROCESS_OK)
			{
				goto getattrerror;
			}	
#if defined(TCSUPPORT_C1_CUC)
			snprintf(log, sizeof(log), " ALARM LEV-3 ASTATUS-2 EVENTID-104032 WEB ip <%s> login failed 3 times!\n", ip);
#else
			snprintf(log, sizeof(log), "104032 WEB ip <%s> login failed 3 times!\n", ip);
#endif
			openlog("TCSysLog WEB", 0, LOG_LOCAL2);
			syslog(LOG_ALERT, log);
			closelog();
		}
		#else
		tcdbg_printf("\r\noperate web info,do nothing!");
		#endif
	}
		
	return SUCCESS;

getattrerror:	
	tcdbg_printf("\r\naccount_execute:getattr error!");
	return FAIL;	
	
}/* end account_execute*/
int
account_boot(mxml_node_t *top){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char active[10] = {0};
	char port[10] = {0};
	char tmp[150] = {0};
	int portnum = 0;
#if !defined(TCSUPPORT_CUC)
#if defined(TCSUPPORT_CT_PON)
	int disp_mask[9] = {0};
	int attr_mask[32] = {0};
#endif
#endif

	//add by brian for not allow to login in in the wan side
	sprintf(tmp,"iptables -t filter -A INPUT -p TCP --dport 80 -i ! br+ -j DROP");
	system(tmp);
#if defined(TCSUPPORT_CT_PORTSLIMIT)
	system("iptables -t filter -N PORTS_LIMIT");
	system("iptables -t filter -A INPUT -j PORTS_LIMIT");
	system("iptables -t filter -F PORTS_LIMIT");
	system("iptables -t filter -Z PORTS_LIMIT");

	// DROP wan ports packets for router mode
	system("iptables -t filter -A PORTS_LIMIT -p TCP -m multiport --dport 53,3555,5555,49152,49153 -i ppp+ -j DROP");
	system("iptables -t filter -A PORTS_LIMIT -p TCP -m multiport --dport 53,3555,5555,49152,49153 -i nas+ -j DROP");
	system("iptables -t filter -A PORTS_LIMIT -p UDP -m multiport --dport 53,3555,67,1900 -i ppp+ -j DROP");
	system("iptables -t filter -A PORTS_LIMIT -p UDP -m multiport --dport 53,3555,67,1900 -i nas+ -j DROP");
#ifdef TCSUPPORT_CT_JOYME	
	system("iptables -t filter -A PORTS_LIMIT -p TCP -m multiport --dport 3456,3457,8080 -i ppp+ -j DROP");
	system("iptables -t filter -A PORTS_LIMIT -p TCP -m multiport --dport 3456,3457,8080 -i nas+ -j DROP");
	system("iptables -t filter -A PORTS_LIMIT -p UDP -m multiport --dport 3456,3457,8080 -i ppp+ -j DROP");
	system("iptables -t filter -A PORTS_LIMIT -p UDP -m multiport --dport 3456,3457,8080 -i nas+ -j DROP");
#endif

	system("ebtables -t filter -N PORTS_LIMIT");
	system("ebtables -t filter -P PORTS_LIMIT RETURN");
	system("ebtables -A INPUT -j PORTS_LIMIT");
	system("ebtables -t filter -F PORTS_LIMIT");
	system("ebtables -t filter -Z PORTS_LIMIT");

	// DROP wan ports packets for bridge mode
	system("ebtables -t filter -A PORTS_LIMIT -p IPv4 --ip-proto 6 --ip-dport 80 -i nas+ -j DROP");
	system("ebtables -t filter -A PORTS_LIMIT -p IPv4 --ip-proto 6 --ip-dport 5555 -i nas+ -j DROP");
	system("ebtables -t filter -A PORTS_LIMIT -p IPv4 --ip-proto 6 --ip-dport 49152 -i nas+ -j DROP");
	system("ebtables -t filter -A PORTS_LIMIT -p IPv4 --ip-proto 6 --ip-dport 49153 -i nas+ -j DROP");
	system("ebtables -t filter -A PORTS_LIMIT -p IPv6 --ip6-proto 6 --ip6-dport 80 -i nas+ -j DROP");

	system("ebtables -t filter -A PORTS_LIMIT -p IPv4 --ip-proto 17 --ip-dport 53 -i nas+ -j DROP");
	system("ebtables -t filter -A PORTS_LIMIT -p IPv4 --ip-proto 17 --ip-dport 3555 -i nas+ -j DROP");
	system("ebtables -t filter -A PORTS_LIMIT -p IPv4 --ip-proto 17 --ip-dport 67 -i nas+ -j DROP");
	system("ebtables -t filter -A PORTS_LIMIT -p IPv6 --ip6-proto 17 --ip6-dport 53 -i nas+ -j DROP");
	system("ebtables -t filter -A PORTS_LIMIT -p IPv6 --ip6-proto 17 --ip6-dport 3555 -i nas+ -j DROP");
	system("ebtables -t filter -A PORTS_LIMIT -p IPv6 --ip6-proto 17 --ip6-dport 547 -i nas+ -j DROP");
	system("ebtables -t filter -A PORTS_LIMIT -p IPv4 --ip-proto 17 --ip-dport 1900 -i nas+ -j DROP");
#ifdef TCSUPPORT_CT_JOYME
	system("ebtables -t filter -A PORTS_LIMIT -p IPv4 --ip-proto 6 --ip-dport 3456 -i nas+ -j DROP");
	system("ebtables -t filter -A PORTS_LIMIT -p IPv4 --ip-proto 6 --ip-dport 3457 -i nas+ -j DROP");
	system("ebtables -t filter -A PORTS_LIMIT -p IPv4 --ip-proto 6 --ip-dport 8080 -i nas+ -j DROP");
	system("ebtables -t filter -A PORTS_LIMIT -p IPv4 --ip-proto 17 --ip-dport 3456 -i nas+ -j DROP");
	system("ebtables -t filter -A PORTS_LIMIT -p IPv4 --ip-proto 17 --ip-dport 3457 -i nas+ -j DROP");
	system("ebtables -t filter -A PORTS_LIMIT -p IPv4 --ip-proto 17 --ip-dport 8080 -i nas+ -j DROP");
	system("ebtables -t filter -A PORTS_LIMIT -p IPv6 --ip6-proto 6 --ip6-dport 3456 -i nas+ -j DROP");
	system("ebtables -t filter -A PORTS_LIMIT -p IPv6 --ip6-proto 6 --ip6-dport 3457 -i nas+ -j DROP");
	system("ebtables -t filter -A PORTS_LIMIT -p IPv6 --ip6-proto 6 --ip6-dport 8080 -i nas+ -j DROP");
	system("ebtables -t filter -A PORTS_LIMIT -p IPv6 --ip6-proto 17 --ip6-dport 3456 -i nas+ -j DROP");
	system("ebtables -t filter -A PORTS_LIMIT -p IPv6 --ip6-proto 17 --ip6-dport 3457 -i nas+ -j DROP");
	system("ebtables -t filter -A PORTS_LIMIT -p IPv6 --ip6-proto 17 --ip6-dport 8080 -i nas+ -j DROP");
#endif
	system("ebtables -t filter -N PORTS_LIMIT_OUT");
	system("ebtables -t filter -P PORTS_LIMIT_OUT RETURN");
	system("ebtables -A OUTPUT -j PORTS_LIMIT_OUT");
	system("ebtables -t filter -F PORTS_LIMIT_OUT");
	system("ebtables -t filter -Z PORTS_LIMIT_OUT");

	// FORBIDDEN CPE RA packets to wan for bridge mode
	system("ebtables -t filter -A PORTS_LIMIT_OUT -o nas+ -p IPv6 --ip6-proto 58 --ip6-icmpv6type 134 -j DROP");
#endif

#if !defined(TCSUPPORT_CUC)
#if defined(TCSUPPORT_CT_PON)
	// 1. get old display mask
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], ACCOUNT);
	sprintf(nodeName[1], "%s%d", ACCOUNT_ENTRY, 1);

	if ( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, "display_mask", attr_mask)
		&& 0 != attr_mask[0] )
	{
		if ( 9 == sscanf(attr_mask, "%02X %02X %02X %02X %02X %02X %02X %02X %02X",
			&disp_mask[0], &disp_mask[1], &disp_mask[2], &disp_mask[3], &disp_mask[4],
			&disp_mask[5], &disp_mask[6], &disp_mask[7], &disp_mask[8]) )
		{
			// 2. add net-tr069.asp display mask
			disp_mask[3] |= 0x10;
			disp_mask[0] |= 0x04;

			// 3. save it
			sprintf(attr_mask, "%02X %02X %02X %02X %02X %02X %02X %02X %02X",
			disp_mask[0], disp_mask[1], disp_mask[2], disp_mask[3], disp_mask[4],
			disp_mask[5], disp_mask[6], disp_mask[7], disp_mask[8]);
			setAttrValue(top, nodeName, "display_mask", attr_mask);
		}
	}
#endif
#endif

	//start up telnet application
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],ACCOUNT);
  	strcpy(nodeName[1],TELNET_ENTRY);
	if(getAttrValue(top, nodeName, ACCOUNT_NODE_ACTIVE, active) == TCAPI_PROCESS_OK )
	{
		if(!strcmp(active,ACTIVE))
		{
			if(getAttrValue(top, nodeName, "telnet_port", port) == TCAPI_PROCESS_OK )
			{
				portnum = atoi(port);
				if(portnum == 0)
				{
					strcpy(port,TELNETDEFAULTPORT);
					portnum = 23;
				}
				sprintf(tmp,"/usr/bin/utelnetd -p %d -l /bin/login -d",portnum);
				system(tmp);
			}
			else
			{
				goto getattrerror;
			}	
		}
	}
	else
	{
		goto getattrerror;
	}

	//start up tftp application
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],ACCOUNT);
  	strcpy(nodeName[1],FTP_ENTRY);
	if(getAttrValue(top, nodeName, ACCOUNT_NODE_ACTIVE, active) == TCAPI_PROCESS_OK )
	{
		if(!strcmp(active,ACTIVE))
		{
			if(getAttrValue(top, nodeName, "ftp_port", port) == TCAPI_PROCESS_OK )
			{
				portnum = atoi(port);
				if(portnum == 0)
				{
					strcpy(port,FTPDEFAULTPORT);
				}
				if(parseftpport(port) < 0)
				{
					tcdbg_printf("\r\naccount_write:parse ftp port fail!");
					return -1;
				}
				system("/userfs/bin/bftpd -d");
			}
			else
			{
				goto getattrerror;
			}
		}
	}
	else
	{
		goto getattrerror;
	}

	return 0;
	
getattrerror:	
		tcdbg_printf("account_boot:getattr error!");
		return -1;

}
#endif
#if defined(CT_COM_DEVICEREG) || defined(TCSUPPORT_CT_PON_BIND2_WEB)
int
deviceAccount_boot(mxml_node_t *top)
{
	//FILE *startupSh=NULL;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char tmpVar[4] = {0};
	int status = -1, result = -1, no_landingpage = 0;
	char v_value[128] = {0};
	int curflag = 0;

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], "deviceAccount");
	strcpy(nodeName[1], "Entry");

	setAttrValue(top, nodeName, "recTime", "-180");
	setAttrValue(top, nodeName, "retryTimes", "0");

	 memset(tmpVar, 0, sizeof(tmpVar) );
	if(getAttrValue(top, nodeName, DEVACCOUNT_STATUS, tmpVar) != TCAPI_PROCESS_OK){
		printf("error:deviceAccount function Get status value fail!\n");
		return FAIL;
	}
	else
	{
		status = atoi(tmpVar);
	}

	memset(tmpVar, 0, sizeof(tmpVar) );
	if(getAttrValue(top, nodeName, DEVACCOUNT_RESULT, tmpVar) != TCAPI_PROCESS_OK){
		printf("error:deviceAccount function Get result value fail!\n");
		return FAIL;
	}
	else
	{
		result = atoi(tmpVar);
	}

	memset(tmpVar, 0, sizeof(tmpVar) );
	if ( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "isNOLandingPage", tmpVar) )
	  strcpy(tmpVar, "0");
	no_landingpage = atoi(tmpVar);

	curflag = (status == 99 || (status == 0 && result == 2) || (status == 0 && result == 99) 	
 	 || (status == 1) || (status == 2) || (status == 3) || (status == 4) || (status == 0 && result == 0));
	if( curflag && (1 != no_landingpage) )
	{
//		setAttrValue(top, nodeName, DEVACCOUNT_WEBPAGELOCK, "1");
		system("iptables -t nat -D PREROUTING -i br+ -p tcp --dport 80 -j REDIRECT --to-ports 80");
		system("iptables -t nat -A PREROUTING -i br+ -p tcp --dport 80 -j REDIRECT --to-ports 80");
#ifdef CT_COM_DEVICEREG
		prevDevRegFlag = 1;
#endif
		tcdbg_printf("\n\n\ndevice registration webpage LOCKED!\n\n\n");
	}
	else
	{
//		setAttrValue(top, nodeName, DEVACCOUNT_WEBPAGELOCK, "0");
		system("iptables -t nat -D PREROUTING -i br+ -p tcp --dport 80 -j REDIRECT --to-ports 80");
		tcdbg_printf("\n\n\ndevice registration webpage UNLOCKED!\n\n\n");
		#ifdef CT_COM_DEVICEREG	
		unlink("/etc/hosts");
		prevDevRegFlag = 0;
		#endif
	}

#if defined(TCSUPPORT_CT_ADSL_BIND1)
	if ( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, "XBINDResult", v_value)
		&& 0 != strcmp(v_value, "SUCC") )
	{
		setAttrValue(top, nodeName, "XBINDResult", "");
		setAttrValue(top, nodeName, "XBINDType", "");
	}
#endif

	return SUCCESS;
}

int
deviceAccount_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="deviceAccount",
#if defined(TCSUPPORT_CT_PON_BIND2_WEB)
			.type=SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=NULL,
			.cfg_boot=NULL,
#else
			.type=SINGLE_TYPE|UPDATE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=deviceAccount_read,
			.cfg_write=deviceAccount_write,
			.cfg_verify=deviceAccount_verify,
			.cfg_execute=deviceAccount_execute,
			.cfg_boot=deviceAccount_boot,
#endif
	};
	ret=register_cfg_node(&node);
	return ret;
}

int
deviceAccount_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char tmpValue[4] = {0};
	int retryTimes = 0, retryLimit = 0, recTime = 0, nowTime = 0;
	mxml_node_t *curNode = NULL;

	curNode=get_web_node(top, name);
	if(curNode==NULL){
		return FAIL;
	}

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], "deviceAccount");
	strcpy(nodeName[1], "Entry");

	if(attr != NULL){
		if(strcmp(attr,"retryTimes") == 0){
			memset(tmpValue, 0, sizeof(tmpValue));
			if(getAttrValue(top, nodeName, "retryTimes", tmpValue) != TCAPI_PROCESS_OK ){
				return FAIL;
			}	

			retryTimes = atoi(tmpValue);

			memset(tmpValue, 0, sizeof(tmpValue));
			if(getAttrValue(top, nodeName, "retryLimit", tmpValue) != TCAPI_PROCESS_OK ){
				return FAIL;
			}	

			retryLimit = atoi(tmpValue);

			if(retryTimes >= retryLimit )
            {
	            struct timespec t1;
            	memset(tmpValue, 0, sizeof(tmpValue));
				if(getAttrValue(top, nodeName, "recTime", tmpValue) != TCAPI_PROCESS_OK ){
					return FAIL;
				}

				recTime = atoi(tmpValue);

				clock_gettime(CLOCK_MONOTONIC, &t1);
				nowTime = t1.tv_sec;
				
				//tcdbg_printf("timerec = %d, nowtime = %d, nowtime - timerec = %d\n", recTime, nowTime, nowTime - recTime);

				if(nowTime - recTime > 180)
				{
					mxmlElementSetAttr(curNode, "retryTimes", "0");
				}

			}	
		}
	}

	return SUCCESS;

}

int
deviceAccount_write(mxml_node_t *top, mxml_node_t *parant){
	return 0;
}

int
deviceAccount_verify(mxml_node_t *node){
	return 0;
}

int
deviceAccount_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char lan_nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0}, dhcpType[8] = {0};
	int status=-1,result = -1;

	int retryTimes = -1, retryLimit = -1, no_landingpage = 0;
	char time_sec[16] = {0};
 	struct timespec tcur;

	//int retryTimes = -1, retryLimit = -1;
	char tmp[8] = {0};
	int curflag = 0, isDhcpActive = 0;

	clock_gettime(CLOCK_MONOTONIC, &tcur);

	memset(nodeName, 0, sizeof(nodeName));	
	strcpy(nodeName[0], "deviceAccount");
	strcpy(nodeName[1], "Entry");

	memset(lan_nodeName, 0, sizeof(lan_nodeName));
	strcpy(lan_nodeName[0], LAN);
	strcpy(lan_nodeName[1], LAN_DHCP);
	if ( TCAPI_PROCESS_OK == 
		getAttrValue(top, lan_nodeName, LAN_DHCP_TYPE, dhcpType)
		&& 0 == strcmp(dhcpType, "1") )
		isDhcpActive = 1;
	
	memset(tmp, 0, sizeof(tmp) );
	if(getAttrValue(top, nodeName, DEVACCOUNT_STATUS, tmp) != TCAPI_PROCESS_OK){
		tcdbg_printf("error:deviceAccount function Get status value fail!\n");
		return FAIL;
	}
	else
	{
		status = atoi(tmp);
	}

	memset(tmp, 0, sizeof(tmp) );
	if(getAttrValue(top, nodeName, DEVACCOUNT_RESULT, tmp) != TCAPI_PROCESS_OK){
		tcdbg_printf("error:deviceAccount function Get result value fail!\n");
		return FAIL;
	}
	else
	{
		result = atoi(tmp);
	}

	memset(tmp, 0, sizeof(tmp));
	if(getAttrValue(top, nodeName, DEVACCOUNT_RETRYTIMES, tmp) != TCAPI_PROCESS_OK){
                tcdbg_printf("error:deviceAccount function Get retry times value fail!\n");
                return FAIL;
        }
        else
        {
                retryTimes = atoi(tmp);
        }

	memset(tmp, 0, sizeof(tmp));
        if(getAttrValue(top, nodeName, DEVACCOUNT_RETRYLIMIT, tmp) != TCAPI_PROCESS_OK){
                tcdbg_printf("error:deviceAccount function Get retry limit value fail!\n");
                return FAIL;
        }
        else
        {
                retryLimit = atoi(tmp);
        }

	memset(tmp, 0, sizeof(tmp) );
	if ( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "isNOLandingPage", tmp) )
	  strcpy(tmp, "0");
	no_landingpage = atoi(tmp);

	curflag = (status == 99 || (status == 0 && result == 2) || (status == 0 && result == 99) 	
 	 || (status == 1) || (status == 2) || (status == 3) || (status == 4) || (status == 0 && result == 0));
	if( curflag && (1 != no_landingpage) )
	{
		setAttrValue(top, nodeName, DEVACCOUNT_WEBPAGELOCK, "1");
		system("iptables -t nat -D PREROUTING -i br+ -p tcp --dport 80 -j REDIRECT --to-ports 80");
		system("iptables -t nat -A PREROUTING -i br+ -p tcp --dport 80 -j REDIRECT --to-ports 80");
	
		system("ebtables -t filter -D FORWARD -p IPv4 --ip-proto udp --ip-dport 53 -j DROP");
		if ( isDhcpActive )
			system("ebtables -t filter -A FORWARD -p IPv4 "
					"--ip-proto udp --ip-dport 53 -j DROP");
	}
	else
	{
		setAttrValue(top, nodeName, DEVACCOUNT_WEBPAGELOCK, "0");
		system("iptables -t nat -D PREROUTING -i br+ -p tcp --dport 80 -j REDIRECT --to-ports 80");
		system("ebtables -t filter -D FORWARD -p IPv4 --ip-proto udp --ip-dport 53 -j DROP");
		///tcapi_set("WebCurSet_Entry", "isLoidRegistering", "No");
		{
			tcapi_msg_t msg;
			memset(&msg, 0, sizeof(msg) );
			strcpy(msg.node, "WebCurSet_Entry" );
			strcpy(msg.attr, "isLoidRegistering");
			sprintf(msg.value, "No");
		 	tcapi_set_req(top, &msg );
		}
		#ifdef CT_COM_DEVICEREG		
		unlink("/etc/hosts");
		#endif
	}
	#ifdef CT_COM_DEVICEREG
	if(prevDevRegFlag != curflag)
        {
             system("killall -9 dnsmasq");
             system("/userfs/bin/dnsmasq &");
             prevDevRegFlag = curflag;
        }
	#endif
	if( retryTimes >= retryLimit )
	{

			sprintf(time_sec, "%d", tcur.tv_sec);
			setAttrValue(top, nodeName, "recTime", time_sec);
	}

#if 0
	memset(tmp, 0, sizeof(tmp) );
	if(getAttrValue(top, nodeName, DEVACCOUNT_RETRYLIMIT, tmp) != TCAPI_PROCESS_OK){
		tcdbg_printf("error:deviceAccount function Get retryLimit value fail!\n");
		return FAIL;
	}
	else
	{
		retryLimit = atoi(tmp);
	}

	memset(tmp, 0, sizeof(tmp) );
	if(getAttrValue(top, nodeName, DEVACCOUNT_RETRYTIMES, tmp) != TCAPI_PROCESS_OK){
		tcdbg_printf("error:deviceAccount function Get retryTimes value fail!\n");
		return FAIL;
	}
	else
	{
		retryTimes = atoi(tmp);
	}

	if( retryTimes < retryLimit )
	{
		setAttrValue(top, nodeName, DEVACCOUNT_FAILUREWARNING, "0");
	}
	else
	{
		setAttrValue(top, nodeName, DEVACCOUNT_FAILUREWARNING, "1");
		tcdbg_printf("error:deviceAccount fatal error, contact CT 10000 !\n");
	}
#endif	
	return SUCCESS;

}

#endif




#if 0  /*Orginal ci-command mode */
/*______________________________________________________________________________
**	timezone_read
**
**	descriptions:
**		Read timezone information from configuration file and record those information
**		to mxml_node_t.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
timezone_read(mxml_node_t *top){
	int retval=0;
	char *pAttr;

	pAttr=(char *)timezone_attribute;
	retval=node_read(top,TIMEZONE, TIMEZONE_PATH, pAttr,NO_QMARKS);
	return retval;
}/* end timezone_read */
#endif

#ifndef PURE_BRIDGE
//"Timezone",	SINGLE_TYPE,	NULL, NULL,NULL, timezone_write, timezone_verify,  timezone_execute},
/*______________________________________________________________________________
**	timezone_init
**
**	descriptions:
**		the init function of timezone.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
timezone_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="Timezone",
#ifdef TCSUPPORT_CUC_TIME_DISPLAY							
			.type=SINGLE_TYPE | UPDATE_TYPE,
#else
			.type=SINGLE_TYPE,
#endif			
			.adv_table=NULL,
			.merge_table=NULL,
#ifdef TCSUPPORT_CUC_TIME_DISPLAY			
			.cfg_read=timezone_read,
#else
			.cfg_read=NULL,
#endif
			.cfg_write=timezone_write,
			.cfg_verify=timezone_verify,
			.cfg_execute=timezone_execute,
			.cfg_boot=timezone_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}

/*______________________________________________________________________________
**	timezone_boot
**
**	descriptions:
**		the boot function of Timezone.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
timezone_boot(mxml_node_t *top){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];

	memset(nodeName,0, sizeof(nodeName));
	strcpy(nodeName[0], TIMEZONE);
	strcpy(nodeName[1], SUB_NODE_NAME);	
	setAttrValue(top,nodeName, "SyncResult", "0");	

	return SUCCESS;
}

#ifdef TCSUPPORT_CUC_TIME_DISPLAY		
/*_____________________________________________________________________________
**      function name: timezone_read
**      descriptions:
**           To create Timezone cfg node.
**
**      parameters:
**            top:  Put the top of cfg node tree.
**            name: The name of Timezone cfg node.
**
**      global:
**             None
**
**      return:
**             None
**
**      call:
**             setAttrValue
**
**      revision:
**      1. yu.zhang 2013/12/25
**____________________________________________________________________________
*/
int
timezone_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr){
	time_t	now;
	struct tm * timenow = NULL;
  	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];

	time(&now);
	timenow = localtime(&now);


	memset(nodeName,0, sizeof(nodeName));
	strcpy(nodeName[0], TIMEZONE);
	strcpy(nodeName[1], SUB_NODE_NAME);	
	setAttrValue(top,nodeName, "CURTIME", asctime(timenow));	

	return SUCCESS;
}
#endif
/*______________________________________________________________________________
**	timezone_write
**
**	descriptions:
**		Find timezone element from mxml tree then write timezone's system parameter to specify
**		configuration files.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  timezone cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
timezone_write(mxml_node_t *top, mxml_node_t *parant){
#if 0
	char buf[64];
	char tz_type[4]={0};
	char server[64]={0};
	char nodeName[3][MAX_NODE_NAME];

	#if 0  /*Orginal ci-command mode */
	int retval = 0;
	mxml_node_t *node = NULL;
	const char *pValue=NULL;
	node = mxmlFindElement(tree, tree, TIMEZONE_ENTRY,
		NULL, NULL,
		MXML_DESCEND);

	node = mxmlFindElement(parant, parant, SUB_NODE_NAME,
		NULL, NULL, MXML_DESCEND);
	retval = node_write(node, TIMEZONE_PATH, NO_QMARKS);
	#endif

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], TIMEZONE);
	strcpy(nodeName[1], SUB_NODE_NAME);
	if(getAttrValue(top, nodeName, "TYPE", tz_type) != TCAPI_PROCESS_OK ){
		return FAIL;
	}

	if(atoi(tz_type) == TZ_NTP_TYPE){
		/*NTP Server automatically*/
		/*create ntp.sh file*/
		/*if(retval != FAIL){*/
			/*Get NTP server ip information*/
			/*pValue=mxmlElementGetAttr(node, TIMEZONE_ATTR_SERVER);*/
			/*pvalue is equal to null because the node is null so i use getAttrValue to get value.
			    server 0.0.0.0 must use default server. shnwind*/
			getAttrValue(top, nodeName, TIMEZONE_ATTR_SERVER, server);
			if(strcmp(server,"0.0.0.0")){
				sprintf(buf, NTP_CMD, server);
			}
			else{
				sprintf(buf, NTP_CMD, NTP_DEF_SERVER);
			}

			if(write2file(buf,NTP_PATH) == FAIL){
				return FAIL;
			}
			chmod(NTP_PATH,755);/*Script must can execution*/

#if defined(TCSUPPORT_CT_2NTP)
			memset(server, 0, sizeof(server));
			memset(buf, 0, sizeof(buf));
			getAttrValue(top, nodeName, TIMEZONE_ATTR_SERVER2, server);
			if(strcmp(server,"0.0.0.0")){
				sprintf(buf, NTP_CMD, server);
			}
			else{
				sprintf(buf, NTP_CMD, NTP_DEF_SERVER);
			}

			if(write2file(buf,NTP_PATH2) == FAIL){
				return FAIL;
			}
			chmod(NTP_PATH2,755);/*Script must can execution*/	

#if defined(TCSUPPORT_CT_5NTP)
			memset(server, 0, sizeof(server));
			memset(buf, 0, sizeof(buf));
			getAttrValue(top, nodeName, TIMEZONE_ATTR_SERVER3, server);
			if(strcmp(server,"0.0.0.0"))
				sprintf(buf, NTP_CMD, server);
			else
				sprintf(buf, NTP_CMD, NTP_DEF_SERVER);
			if(write2file(buf,NTP_PATH3) == FAIL)
				return FAIL;
			chmod(NTP_PATH3,755);

			memset(server, 0, sizeof(server));
			memset(buf, 0, sizeof(buf));
			getAttrValue(top, nodeName, TIMEZONE_ATTR_SERVER4, server);
			if(strcmp(server,"0.0.0.0"))
				sprintf(buf, NTP_CMD, server);
			else
				sprintf(buf, NTP_CMD, NTP_DEF_SERVER);
			if(write2file(buf,NTP_PATH4) == FAIL)
				return FAIL;
			chmod(NTP_PATH4,755);

			memset(server, 0, sizeof(server));
			memset(buf, 0, sizeof(buf));
			getAttrValue(top, nodeName, TIMEZONE_ATTR_SERVER5, server);
			if(strcmp(server,"0.0.0.0"))
				sprintf(buf, NTP_CMD, server);
			else
				sprintf(buf, NTP_CMD, NTP_DEF_SERVER);
			if(write2file(buf,NTP_PATH5) == FAIL)
				return FAIL;
			chmod(NTP_PATH5,755);
#endif

#endif
		/*}*/
		}
#endif
	return SUCCESS;
}/* end timezone_write */

/*______________________________________________________________________________
**	timezone_verify
**
**	descriptions:
**		To check the system parameter of timezone element  is validly or not.
**
**	parameters:
**		node: 		Specify the root of mxml_node_t structure.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
timezone_verify(mxml_node_t *node){
	return SUCCESS;
}/* end timezone_verify */

/*_____________________________________________________________________________
**      function name: timezone_execute
**      descriptions:
**            To  execute timezone node service.
**
**      parameters:
**            top:   Put the timezone of cfg node tree that is used to execute snmpd information.
**            name: Put the timezone cfg node name.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**
**      revision:
**      1. Here 2008/5/19
**____________________________________________________________________________
*/
int
timezone_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	char *pValue=NULL;
	char *delim="/: ";
	char time_info[6][8];
	char tz_type[4]={0};
	char tz_pc_clock[32]={0};
	int i=0;
	int ret=-1;
	char time_attr[][8]={"Year","Month","Date","Hour","Min","Sec",""};
	time_t tm;
	struct tm tm_time;
	char cmd[256] = {0};
	char server[64]={0};
	int now_time=0,correct_time=0;//,oldtime=0;
#if defined(TCSUPPORT_CT_2NTP)
	char server2[64]={0};
#if defined(TCSUPPORT_CT_5NTP)
	char server3[64]={0};
	char server4[64]={0};
	char server5[64]={0};
#endif
#if defined(TCSUPPORT_CT_NTPSERVERTYPE)
	char ntpservertype[4]={0};
	int flag1 =0;
	int flag2 =0;
#endif
	char route[32]={0};
	struct sockaddr_in cvtaddr;
	char addrouteCMD[128]={0};
	char convertIP[64]={0};	
	char convertIP2[64]={0};
#if defined(TCSUPPORT_CT_5NTP)
	char convertIP3[64]={0}, convertIP4[64]={0}, convertIP5[64]={0};
#endif
	struct sockaddr_in addr = {0};
	unsigned long theHostAddressPtr[5]={0};
	int if_index=0;
	char tmp[8]={0};
#endif
#if defined(TCSUPPORT_CT_FJ)
	char ntpStatus[4]={0};
	char ntpName[3][MAX_NODE_NAME];
#endif
#if defined(TCSUPPORT_CT_MONITORCOLLECTOR)
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
#endif
#if defined(TCSUPPORT_CT_PON)
	int svrType = 0;
#endif

	/*krammer add for bug 907*/
	ret = -1;
	if(strlen(name[2])){
		if(!strcmp(name[2],"Success")){
			ret = 0;
		}
		else if(!strcmp(name[2],"Fail")){
			ret = 1;
		}
		name[2][0] = '\0';
	}
	now_time=time(&tm);
	memset(&tm, 0, sizeof(tm));

	memset(time_info, 0, sizeof(time_info));

	if(getAttrValue(top, name, "TYPE", tz_type) == TCAPI_PROCESS_OK){
	switch(atoi(tz_type)){
		case TZ_NTP_TYPE:/*NTP Server automatically*/
		if(0 == ret ){ /* success*/
			tcdbg_printf("\r\n timezone_execute:NTP Server Sync success!");
			ntp_server_ret = 0;

			if(correct_sys_time(top)){
				fprintf(stderr, "Cannot retrieve TimeZone data.\n");
			}
			
#if defined(TCSUPPORT_CT_MONITORCOLLECTOR)		
			memset(nodeName,0, sizeof(nodeName));
			strcpy(nodeName[0], MONITOR_COLLECTOR_NODENAME);
			strcpy(nodeName[1], COMMON_NODENAME);				
			setAttrValue(top, nodeName, "ntpSync", "1") ;
#endif
#if defined(TCSUPPORT_CT_PMINFORM)
			updateNtpState(top, ntp_server_ret);
#endif
			setAttrValue(top, name, "SyncResult","1");

		}
		else if(1 == ret){ /* fail */
			ntp_server_ret = -1;
			setAttrValue(top, name, "SyncResult","0");
			return SUCCESS;
		}
		else{ /* excute */
			ntp_server_ret = -1;
			setAttrValue(top, name, "SyncResult","0");
		
#if defined(TCSUPPORT_CT_FJ)
			memset(ntpName,0,sizeof(ntpName));
			strcpy(ntpName[0], "System");
			strcpy(ntpName[1], "Entry");
			getAttrValue(top,ntpName,"NTPStatus",ntpStatus);
			if(!strcmp(ntpStatus,"1")){
				return SUCCESS;
			}
			setAttrValue(top,ntpName,"NTPStatus","1");
#endif
#if defined(TCSUPPORT_CT_2NTP)
			memset(server,0,sizeof(server));
			memset(route,0,sizeof(route));
			memset(convertIP,0,sizeof(convertIP));
			memset(convertIP2,0,sizeof(convertIP2));
			if(getAttrValue(top, name, TIMEZONE_ATTR_SERVER, server) 
									== TCAPI_PROCESS_OK && 
				getAttrValue(top, name, TIMEZONE_ATTR_SERVER2, server2) 
									== TCAPI_PROCESS_OK &&
#if defined(TCSUPPORT_CT_5NTP)
				getAttrValue(top, name, TIMEZONE_ATTR_SERVER3, server3) 
									== TCAPI_PROCESS_OK &&
				getAttrValue(top, name, TIMEZONE_ATTR_SERVER4, server4) 
									== TCAPI_PROCESS_OK &&
				getAttrValue(top, name, TIMEZONE_ATTR_SERVER5, server5) 
									== TCAPI_PROCESS_OK &&
#endif
#if defined(TCSUPPORT_CT_NTPSERVERTYPE)
				getAttrValue(top, name, TIMEZONE_ATTR_NTPSERVERTYPE, ntpservertype) 
									== TCAPI_PROCESS_OK)
#else
				getAttrValue(top, name, TIMEZONE_ATTR_ADDROUTE, route) 
									== TCAPI_PROCESS_OK)
#endif
			{
#if defined(TCSUPPORT_CT_PON)
#if defined(TCSUPPORT_CT_5NTP)
				if ( 0 == strcmp(server, "0.0.0.0")
					&& 0 == strcmp(server2, "0.0.0.0")
					&& 0 == strcmp(server3, "0.0.0.0")
					&& 0 == strcmp(server4, "0.0.0.0")
					&& 0 == strcmp(server5, "0.0.0.0") )
					strcpy(server, NTP_DEF_SERVER);
#else
				if ( 0 == strcmp(server, "0.0.0.0")
					&& 0 == strcmp(server2, "0.0.0.0") )
					strcpy(server, NTP_DEF_SERVER);
#endif
#else
				if (!strcmp(server, "0.0.0.0"))
					strcpy(server, NTP_DEF_SERVER);
				if (!strcmp(server2, "0.0.0.0"))
					strcpy(server2, NTP_DEF_SERVER);
#if defined(TCSUPPORT_CT_5NTP)
				if (!strcmp(server3, "0.0.0.0"))
					strcpy(server3, NTP_DEF_SERVER);
				if (!strcmp(server4, "0.0.0.0"))
					strcpy(server4, NTP_DEF_SERVER);
				if (!strcmp(server5, "0.0.0.0"))
					strcpy(server5, NTP_DEF_SERVER);
#endif
#endif
#if defined(TCSUPPORT_CT_NTPSERVERTYPE)
				//get first match if_index
				/***
				NTPServerType:
				0:INTERNET
				1:VOICE
				2:TR069
				3:OTHER
				***/				
				i=atoi(ntpservertype);
#if defined(TCSUPPORT_CT_PON)
				svrType = i;
#endif
				if(i<0 || i>3)
					return FAIL;
				flag1=ntpdelayflag1_all[i];
				flag2=ntpdelayflag2_all[i];
				if(0 != flag1){
					for(i=0;i<32;i++){
						if(flag1&0x01)
							break;
						flag1=flag1>>1;
					}
					if_index=i;
				}
				else if(0 != flag2){
					for(i=0;i<32;i++){
						if(flag2&0x01)
							break;
						flag2=flag2>>1;
					}
					if_index=i+32;
				}
				else{
					return FAIL;
				}
				if(get_waninfo_by_index(if_index,WAN_IFNAME,route) < 0){
					return FAIL;
				}
#else
				if((if_index = get_wanindex_by_name(route)) < 0){
					return FAIL;
				}
				sprintf(tmp, "%d", if_index);
				setAttrValue(top, name, TIMEZONE_ATTR_IFINDEX,tmp);
#endif


#if defined(TCSUPPORT_CT_5NTP)
				sprintf(addrouteCMD, "-h \"%s %s %s %s %s\"", server, server2, server3, server4, server5);
#else
				sprintf(addrouteCMD, "-h \"%s %s\"", server, server2);
#endif
#if defined(TCSUPPORT_CT_PON)
				if ( 0 != svrType )
				{
#endif
					strcat(addrouteCMD, " -w ");
					strcat(addrouteCMD, route);					
					
#if defined(TCSUPPORT_CT_PON)
				}
#endif

				snprintf(cmd, sizeof(cmd),"/userfs/bin/ntpclient -s -c 1 -l %s &", addrouteCMD);
				//tcdbg_printf("ntp:%s!~~~~~\n\n", cmd);
				system(cmd);
				return SUCCESS;				
			}
#if 0
#if defined(TCSUPPORT_CT_PON)
				if ( 0 != svrType )
				{
#endif
#if defined(TCSUPPORT_CT_5NTP)
					sprintf(cmd, "/bin/echo -e \"%s %s\\n%s %s \\n%s %s\\n%s %s\\n%s %s\" > %s",
					server, route, server2, route, server3, route,  server4, route, server5, route, TMP_DNS_NTP_PATH);
#else
					sprintf(cmd, "/bin/echo -e \"%s %s\\n%s %s\" > %s", server, route, server2, route, TMP_DNS_NTP_PATH);
#endif
					system(cmd);
#if defined(TCSUPPORT_CT_PON)
				}
#endif

				if(inet_aton(server,&cvtaddr.sin_addr))
				{	
					//address is ipv4
					strcpy(convertIP,server);
				}
				else{
					//address is a domain name
					theHostAddressPtr[0] = resolveName(server);
					memcpy(&addr.sin_addr, &theHostAddressPtr[0], sizeof(addr.sin_addr));
					strcpy(convertIP, inet_ntoa(/*theHostAddressPtr*/addr.sin_addr));
				}
			
				if(inet_aton(server2,&cvtaddr.sin_addr))
				{	
					//address is ipv4
					strcpy(convertIP2,server2);
				}
				else{
					//address is a domain name
					theHostAddressPtr[1] = resolveName(server2);
					memcpy(&addr.sin_addr, &theHostAddressPtr[1], sizeof(addr.sin_addr));
					strcpy(convertIP2, inet_ntoa(/*theHostAddressPtr*/addr.sin_addr));
				}	

#if defined(TCSUPPORT_CT_5NTP)
				if(inet_aton(server3, &cvtaddr.sin_addr))
				{	
					//address is ipv4
					strcpy(convertIP3, server3);
				}
				else
				{
					//address is a domain name
					theHostAddressPtr[2] = resolveName(server3);
					memcpy(&addr.sin_addr, &theHostAddressPtr[2], sizeof(addr.sin_addr));
					strcpy(convertIP3, inet_ntoa(/*theHostAddressPtr*/addr.sin_addr));
				}

				if(inet_aton(server4, &cvtaddr.sin_addr))
				{	
					//address is ipv4
					strcpy(convertIP4, server4);
				}
				else
				{
					//address is a domain name
					theHostAddressPtr[3] = resolveName(server4);
					memcpy(&addr.sin_addr, &theHostAddressPtr[3], sizeof(addr.sin_addr));
					strcpy(convertIP4, inet_ntoa(/*theHostAddressPtr*/addr.sin_addr));
				}

				if(inet_aton(server5, &cvtaddr.sin_addr))
				{	
					//address is ipv4
					strcpy(convertIP5, server5);
				}
				else
				{
					//address is a domain name
					theHostAddressPtr[4] = resolveName(server5);
					memcpy(&addr.sin_addr, &theHostAddressPtr[4], sizeof(addr.sin_addr));
					strcpy(convertIP5, inet_ntoa(/*theHostAddressPtr*/addr.sin_addr));
				}
#endif

				//execute [route add cmd]
			if (0 != (theHostAddressPtr[0]+theHostAddressPtr[1]+theHostAddressPtr[2]+\
				                         theHostAddressPtr[3]+theHostAddressPtr[4])) {
#if defined(TCSUPPORT_CT_PON)
				if ( 0 != svrType )
				{
#endif
				sprintf(addrouteCMD,ADDROUTE_CMD, convertIP,route);
				ret = system(addrouteCMD);
#if defined(TCSUPPORT_CT_PON)
				}
				else
				{
					if ( 0 == strcmp(convertIP, "0.0.0.0") )
					{
						ret = -1;
						ntp_server_ret = -1;
					}
					else
						ret = 0;
				}
#endif
				if(0 == ret){
					ntp_server_ret = system(NTP_PATH);
					if(0 == ntp_server_ret){
						if(correct_sys_time(top)){
							fprintf(stderr, "Cannot retrieve TimeZone data.\n");
						}
					}
					//execute [route delete cmd]
#if defined(TCSUPPORT_CT_PON)
					if ( 0 != svrType )
					{
#endif
					sprintf(addrouteCMD,DELROUTE_CMD, convertIP);
					system(addrouteCMD);
#if defined(TCSUPPORT_CT_PON)
					}
#endif
				}
				
				if(0 == ret && 0 != ntp_server_ret){
#if defined(TCSUPPORT_CT_PON)
					if ( 0 != svrType )
					{
#endif
					sprintf(addrouteCMD,ADDROUTE_CMD, convertIP2,route);
					ret = system(addrouteCMD);
#if defined(TCSUPPORT_CT_PON)
					}
					else
					{
						if ( 0 == strcmp(convertIP2, "0.0.0.0") )
						{
							ret = -1;
							ntp_server_ret = -1;
						}
						else
							ret = 0;
					}
#endif
					if(0 == ret){
						if(0 == system(NTP_PATH2)){
							ntp_server_ret = 0;
							if(correct_sys_time(top)){
								fprintf(stderr, "2NTP-Cannot retrieve TimeZone data.\n");
							}
						}
#if defined(TCSUPPORT_CT_PON)
						if ( 0 != svrType )
						{
#endif
						sprintf(addrouteCMD,DELROUTE_CMD, convertIP2);
						system(addrouteCMD);
#if defined(TCSUPPORT_CT_PON)
						}
#endif
					}
				}
#if defined(TCSUPPORT_CT_5NTP)
				if(0 == ret && 0 != ntp_server_ret)
				{
#if defined(TCSUPPORT_CT_PON)
					if ( 0 != svrType )
					{
#endif
					sprintf(addrouteCMD,ADDROUTE_CMD, convertIP3,route);
					ret = system(addrouteCMD);
#if defined(TCSUPPORT_CT_PON)
					}
					else
					{
						if ( 0 == strcmp(convertIP3, "0.0.0.0") )
						{
							ret = -1;
							ntp_server_ret = -1;
						}
						else
							ret = 0;
					}
#endif
					if(0 == ret)
					{
						if(0 == system(NTP_PATH3))
						{
							ntp_server_ret = 0;
							if(correct_sys_time(top))
							{
								fprintf(stderr, "3NTP-Cannot retrieve TimeZone data.\n");
							}
						}
#if defined(TCSUPPORT_CT_PON)
						if ( 0 != svrType )
						{
#endif
						sprintf(addrouteCMD, DELROUTE_CMD, convertIP3);
						system(addrouteCMD);
#if defined(TCSUPPORT_CT_PON)
						}
#endif
					}
				}

				if(0 == ret && 0 != ntp_server_ret)
				{
#if defined(TCSUPPORT_CT_PON)
					if ( 0 != svrType )
					{
#endif
					sprintf(addrouteCMD, ADDROUTE_CMD, convertIP4,route);
					ret = system(addrouteCMD);
#if defined(TCSUPPORT_CT_PON)
					}
					else
					{
						if ( 0 == strcmp(convertIP4, "0.0.0.0") )
						{
							ret = -1;
							ntp_server_ret = -1;
						}
						else
							ret = 0;
					}
#endif
					if(0 == ret)
					{
						if(0 == system(NTP_PATH4))
						{
							ntp_server_ret = 0;
							if(correct_sys_time(top))
							{
								fprintf(stderr, "4NTP-Cannot retrieve TimeZone data.\n");
							}
						}
#if defined(TCSUPPORT_CT_PON)
						if ( 0 != svrType )
						{
#endif
						sprintf(addrouteCMD,DELROUTE_CMD, convertIP4);
						system(addrouteCMD);
#if defined(TCSUPPORT_CT_PON)
						}
#endif
					}
				}

				if(0 == ret && 0 != ntp_server_ret)
				{
#if defined(TCSUPPORT_CT_PON)
					if ( 0 != svrType )
					{
#endif
					sprintf(addrouteCMD, ADDROUTE_CMD, convertIP5,route);
					ret = system(addrouteCMD);
#if defined(TCSUPPORT_CT_PON)
					}
					else
					{
						if ( 0 == strcmp(convertIP5, "0.0.0.0") )
						{
							ret = -1;
							ntp_server_ret = -1;
						}
						else
							ret = 0;
					}
#endif
					if(0 == ret)
					{
						if(0 == system(NTP_PATH5))
						{
							ntp_server_ret = 0;
							if(correct_sys_time(top))
							{
								fprintf(stderr, "5NTP-Cannot retrieve TimeZone data.\n");
							}
						}
#if defined(TCSUPPORT_CT_PON)
						if ( 0 != svrType )
						{
#endif
						sprintf(addrouteCMD, DELROUTE_CMD, convertIP5);
						system(addrouteCMD);
#if defined(TCSUPPORT_CT_PON)
						}
#endif
					}
				}
#endif
				}
			else{
				ntp_server_ret=-1;
				}
				unlink(TMP_DNS_NTP_PATH);
			}			
#endif
#else
#if 0
			ntp_server_ret = system(NTP_PATH);
			/**********20081001 racing add************/
			if(correct_sys_time(top)){
				fprintf(stderr, "Cannot retrieve TimeZone data.\n");
			}
			/**********20081001 racing add************/
#else
			getAttrValue(top, name, TIMEZONE_ATTR_SERVER, server);
			snprintf(cmd, sizeof(cmd),"/userfs/bin/ntpclient -s -c 1 -l -h %s &", strcmp(server,"0.0.0.0") ? server : NTP_DEF_SERVER);
			system(cmd);
#endif
#endif
#if 0
#if defined(TCSUPPORT_CT_MONITORCOLLECTOR)
			if(ntp_server_ret == 0){
				tcdbg_printf("\r\n timezone_execute:NTP Server Sync success!");
			
				memset(nodeName,0, sizeof(nodeName));
				strcpy(nodeName[0], MONITOR_COLLECTOR_NODENAME);
				strcpy(nodeName[1], COMMON_NODENAME);				
				setAttrValue(top, nodeName, "ntpSync", "1") ;
			}
#endif
#if defined(TCSUPPORT_CT_PMINFORM)
			updateNtpState(top, ntp_server_ret);
#endif
#endif
		}
			break;
		case TZ_PC_TYPE:/*PC CLOCK*/
		case TZ_MANUAL_TYPE:/*Manually*/
			if(atoi(tz_type)==TZ_PC_TYPE){
				getAttrValue(top, name, "PC_CLOCK", tz_pc_clock);
				strtok(tz_pc_clock,delim);
				strcpy(time_info[0], tz_pc_clock);
				for(i=1;(pValue=strtok(NULL, delim))!= NULL; i++){
					strcpy(time_info[i], pValue);
				}
			}
			else{
				for(i=0; strlen(time_attr[i])!=0; i++){
					getAttrValue(top, name, time_attr[i], time_info[i]);
				}
			}

			time(&tm);
			memset(&tm_time, 0, sizeof(tm_time));

			tm_time.tm_year	= atoi(time_info[0]);
			tm_time.tm_mon	= atoi(time_info[1]);
			tm_time.tm_mday	= atoi(time_info[2]);
			tm_time.tm_hour	= atoi(time_info[3]);
			tm_time.tm_min	= atoi(time_info[4]);
			tm_time.tm_sec	= atoi(time_info[5]);

			/* correct for century  - minor Y2K problem here? */
			if (tm_time.tm_year >= 1900){
				tm_time.tm_year -= 1900;
			}
			/* adjust date */
			if (tm_time.tm_mon >= 1){
				tm_time.tm_mon -= 1;
			}

			tm = mktime(&tm_time);
			if (tm <0){
				fprintf(stderr,"mktime error\n");
			}
			else{
				if (stime(&tm) < 0){
					fprintf(stderr,"stime err!\n");
				}
			}
			break;
		default:
			break;
	}
		}
	/*krammer add for bug 907*/
	memset(&tm, 0, sizeof(tm));
	correct_time=time(&tm);/*get correct time after time synchronization*/
	if(abs(correct_time - now_time) > MAX_DIFFERENT_TIME){//ppp old connect time should be changed when difference >30
		correctPPPtime((correct_time - now_time));
	}

	return SUCCESS;

}/* end timezone_execute*/
#if 0  /*Orginal ci-command mode */
/*______________________________________________________________________________
**	mac_read
**
**	descriptions:
**		Read mac address system parameter from configuration file and record those
**		information to mxml_node_t.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
mac_read(mxml_node_t *top){
	int retval=0;
	char *pAttr=NULL;

	pAttr=(char *)mac_attribute;
	retval=node_read(top,MAC, MAC_PATH, pAttr,NO_QMARKS);
	return retval;
}/* end upnpd_read */
#endif

//"Mac",		SINGLE_TYPE,	NULL, NULL,NULL, mac_write, mac_verify, NULL},
/*______________________________________________________________________________
**	mac_init
**
**	descriptions:
**		the init function of mac.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
mac_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="Mac",
			.type=SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=mac_write,
			.cfg_verify=mac_verify,
			.cfg_execute=NULL,
			.cfg_boot=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}
/*______________________________________________________________________________
**	mac_write
**
**	descriptions:
**		Find upnpd element from mxml tree then write mac's system parameter to specify
**		configuration files.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  timezone cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
mac_write(mxml_node_t *top, mxml_node_t *parant){

	int retval=0;
#if 0
	mxml_node_t *node = NULL;
	/*
	node = mxmlFindElement(tree, tree, MAC_ENTRY,
		NULL, NULL,
		MXML_DESCEND);
	*/
	node = mxmlFindElement(parant, parant, SUB_NODE_NAME,
		NULL, NULL, MXML_DESCEND);
	retval = node_write1(node, MAC_PATH, NO_QMARKS);
#endif
	return retval;
}/* end upnpd_write */

/*______________________________________________________________________________
**	mac_verify
**
**	descriptions:
**		To check the system parameter of mac element  is validly or not.
**
**	parameters:
**		node: 		Specify the root of mxml_node_t structure.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
mac_verify(mxml_node_t *node){
	return SUCCESS;
}/* end upnpd_verify */
#endif
#if 0
/*_____________________________________________________________________________
**      function name: mac_execute
**      descriptions:
**            To  execute mac node service.
**
**      parameters:
**            node:   Put the mac of cfg node tree that is used to execute mac settings.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**
**      revision:
**      1. Here 2008/5/19
**____________________________________________________________________________
*/
int
mac_execute(mxml_node_t *node, char *name){
	return 0;
}/* end mac_execute*/
#endif
#if 0  /*Orginal ci-command mode */
/*______________________________________________________________________________
**	led_read
**
**	descriptions:
**		Read LED GPIO system parameters from configuration file and record those
**		information to mxml_node_t.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
led_read(mxml_node_t *top){
	int retval=0;
	char *pAttr=NULL;

	pAttr=(char *)led_attribute;
	retval=node_read(top,LED, LED_PATH, pAttr, QMARKS);
	return retval;
}/* end upnpd_read */
#endif
#if 0
int
led_boot(mxml_node_t *top){
	FILE *fp;
	FILE *proc;
	char tmp[128]={0};
	fp=fopen(LED_PATH,"r");
	if(fp){
				while(fgets(tmp,sizeof(tmp),fp)){
					/*20090717: pork modified for TC3262*/
					if(strchr(tmp,';')){
						memset(tmp,0,sizeof(tmp));
						continue;
					}
					proc=fopen(LED_PROC_PATH,"w+");
					fputs(tmp,proc);
					fclose(proc);
					memset(tmp,0,sizeof(tmp));
				}
		fclose(fp);
	}
	return SUCCESS;
}

//"Led",		SINGLE_TYPE,	NULL, NULL,NULL, led_write, led_verify, NULL},
/*______________________________________________________________________________
**	led_init
**
**	descriptions:
**		the init function of led.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
led_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="Led",
			.type=SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=led_write,
			.cfg_verify=led_verify,
			.cfg_execute=NULL,
			.cfg_boot=led_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}
/*______________________________________________________________________________
**	led_write
**
**	descriptions:
**		Find LED element from mxml tree then write led's system parameter to specify
**		configuration files.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  timezone cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
led_write(mxml_node_t *top, mxml_node_t *parant){

	int retval = 0;
//	mxml_node_t *node = NULL;
	//
//	char buf[16]={0};
//	mxml_attr_t   *attr=NULL;
//	int i=0,j=0;
//	FILE *fp=NULL;
//	char sub_node_name[32]={0};
	//
	/*
	node = mxmlFindElement(tree, tree, LED_ENTRY,
		NULL, NULL,
		MXML_DESCEND);
	*/
//	fp=fopen(LED_PATH,"w");
//	for(j=0;j<MAX_LED_NUM;j++){
//		sprintf(sub_node_name,"Entry%d",j);
//		node = mxmlFindElement(parant, parant, /*SUB_NODE_NAME*/sub_node_name,
//			NULL, NULL, MXML_DESCEND);
	//	retval = node_write(node, LED_PATH, QMARKS);
//		tcdbg_printf("%s!!!!!!!!!!!!!!!!!!!!!\n",__func__);

//		tcdbg_printf("write led path!!!!!!!!!!!!!!!!!!!!\n");
//		if(!node){
//			tcdbg_printf("why no led node 0rzzzzzzzzzzzzzzzzzzzzzzzzzzz\n");
//			continue;
//		}
//		attr = node->value.element.attrs;
//		tcdbg_printf("before go in for!!!!!!!!!!!!!!!!!!!!\n");
//		for(i = 0; i < node->value.element.num_attrs; i++ ){
//			tcdbg_printf("first of for!!!!!!!!!!!!!!!!!!!!\n");
//			sprintf(buf,"%s\n", attr->value);
//	//		tcdbg_printf("after sprintf!!!!!!!!!!!!!!!!!!!!\n");
//			tcdbg_printf("================%s====================\n",buf);
//			fputs(buf, fp);
//			attr ++;
//		}
//
//	}
//	fclose(fp);
	//
	return retval;
}/* end upnpd_write */

/*______________________________________________________________________________
**	led_verify
**
**	descriptions:
**		To check the system parameter of led element  is validly or not.
**
**	parameters:
**		node: 		Specify the root of mxml_node_t structure.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
led_verify(mxml_node_t *node){
	return SUCCESS;
}/* end upnpd_verify */
#endif 
#if 0
/*_____________________________________________________________________________
**      function name: led_execute
**      descriptions:
**            To  execute led node service.
**
**      parameters:
**            node:   Put the led of cfg node tree that is used to execute led settings.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**
**      revision:
**      1. Here 2008/5/19
**____________________________________________________________________________
*/
int
led_execute(mxml_node_t *node, char *name){
	return 0;
}/* end led_execute*/
#endif
#if 0  /*Orginal ci-command mode */
/*______________________________________________________________________________
**	autoexec_read
**
**	descriptions:
**		Read autoexec.sh configuration file and record those information to mxml_node_t.
**
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
autoexec_read(mxml_node_t *top){
	FILE *fp=NULL;
	char buf[MAXSIZE];
	char tmp_attr[16]={0};
	int i=0;
	mxml_node_t *group= NULL;
	mxml_node_t *node= NULL;   /* <node> */

	group = mxmlNewElement(top, AUTOEXEC);

	fp=fopen(AUTOEXEC_PATH,"r");
	if(fp==NULL){
		return FAIL;
	}

	node = mxmlNewElement(group, AUTOEXEC_ENTRY);
	while(fgets(buf, MAXSIZE, fp)){
		sprintf(tmp_attr, AUTOEXEC_ATTR, i);
		buf[strlen(buf)-1]='\0';
		mxmlElementSetAttr(node, tmp_attr, buf);
		i++;
	}
	fclose(fp);
	return 0;
}/* end autoexec_read */
#endif


//"Autoexec",	SINGLE_TYPE,	NULL, NULL,NULL, autoexec_write, autoexec_verify, NULL},
/*______________________________________________________________________________
**	autoexec_boot
**
**	descriptions:
**		the boot function of autoexec.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**_______________________________________________________________________________
*/
int
autoexec_boot(mxml_node_t *top){
	FILE *startupSh=NULL;
	
	startupSh=fopen("/etc/autoexec.sh","r");
	if(startupSh){
		fclose(startupSh);
		system("chmod 755 /etc/autoexec.sh");
		system("/etc/autoexec.sh");
	}
	//printf("autoexec_execute\n");

	return SUCCESS;
}
/*______________________________________________________________________________
**	autoexec_init
**
**	descriptions:
**		the init function of autoexec.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
autoexec_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="Autoexec",
			.type=SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=autoexec_write,
			.cfg_verify=autoexec_verify,
			.cfg_execute=NULL,
			.cfg_boot=autoexec_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}
/*______________________________________________________________________________
**	autoexec_write
**
**	descriptions:
**		Find autoexec element from mxml tree then write wan's system parameter to specify
**		configuration files.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  timezone cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
autoexec_write(mxml_node_t *top, mxml_node_t *parant){
	int retval = 0;
	mxml_node_t *node = NULL;
	/*
	node = mxmlFindElement(tree, tree, AUTOEXEC_ENTRY,
		NULL, NULL,
		MXML_DESCEND);
	*/
	node = mxmlFindElement(parant, parant, SUB_NODE_NAME,
		NULL, NULL, MXML_DESCEND);
	retval = node_write(node, AUTOEXEC_PATH, NO_ATTRIBUTE);
	return retval;
}/* end autoexec_write */

/*______________________________________________________________________________
**	autoexec_verify
**
**	descriptions:
**		To check the system parameter of autoexec element  is validly or not.
**
**	parameters:
**		node: 		Specify the root of mxml_node_t structure.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
autoexec_verify(mxml_node_t *node){
	return SUCCESS;
}/* end autoexec_verify */
#if 0
/*_____________________________________________________________________________
**      function name: autoexec_execute
**      descriptions:
**            To  autoexec led node service.
**
**      parameters:
**            node:   Put the autoexec of cfg node tree that is used to execute autoexec settings.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**
**      revision:
**      1. Here 2008/5/19
**____________________________________________________________________________
*/
int
autoexec_execute(mxml_node_t *node, char *name){
	return 0;
}/* end autoexec_execute*/
#endif

#ifndef PURE_BRIDGE
/**********20081001 racing add************/
/*_____________________________________________________________________________
**      function name: correct_sys_time
**      descriptions:
**            This function is used to correct system time.
**
**      parameters:
**            top:	Specify the root of mxml_node_t structure.
**
**      global:
**             None
**
**      return:
**            Active:        0
**            Otherwise:     -1
**
**      call:
**		None
**____________________________________________________________________________
*/
int
correct_sys_time(mxml_node_t *top){
	time_t tm;
	char optr;
	char value[16]={0};
	char dayLightStr[8];
	char nodeName[3][MAX_NODE_NAME];
	char *tmp;
	const char *delim = ":";
	int hour=0, min=0;
	int diff = 0;

	memset(nodeName,0,sizeof(nodeName));

	strcpy(nodeName[0], TIMEZONE);
	strcpy(nodeName[1], SUB_NODE_NAME);

	if(getAttrValue(top, nodeName, "TZ", value) != TCAPI_PROCESS_OK){
		return FAIL;
	}else{
		/*if return value is "GMT, we do not need to do anything."*/
		if(!strcasecmp(value, "GMT")||!strcasecmp(value, "GMT-1")){
			return SUCCESS;
		}

		/*    GMT-03:30    */
		/*get diff type, either '-' or '+'*/
		optr = (char)value[3];
		/* get time string, in this case, tmp = "03:30"*/
		tmp = &value[4];

		/*retrieve hour value, in this case, hour = "03"*/
		hour = atoi(strtok(tmp, delim));
		/*==20090118 pork added==*/
		/*If Day light saving is enabled, hour will be decreased 1.*/
		if(getAttrValue(top, nodeName, "DAYLIGHT", dayLightStr) == TCAPI_PROCESS_OK){
			if(!strcmp(dayLightStr,"Enable")){
				if(optr == '-'){
					hour--;
				}else if(optr == '+'){
					hour++;
				}else{
					return FAIL;
				}
			}
		}
		/*==20090118 pork added==*/
		/*retrieve minute value, in this case, min = "30"*/
		min = atoi(strtok(NULL, delim));

		/*calculate time difference*/
		diff = hour*SEC_PER_HOUR+ min*SEC_PER_MIN;
		/*retrieve system time*/
		time(&tm);

		/*minus or plus time difference*/
		if(optr == '-'){
			tm-=diff;
		}else if(optr == '+'){
			tm+=diff;
		}else{
			return FAIL;
		}

		/*set system time*/
		stime(&tm);
	}

	return SUCCESS;
}/* end correct_sys_time*/
/**********20081001 racing add************/

/*_____________________________________________________________________________
**      function name: correctPPPtime
**      descriptions:
**            This function is used to correct the time record in pppuptime-ppp.
**
**      parameters:
**            difference:	the difference between old time and correct time.
**
**      global:
**             None
**
**      return:
**            None
**
**      call:
**		None
**____________________________________________________________________________
*/
void
correctPPPtime(int difference){
	char pppUptimePath[32]={0};
	char pvc_info[16]={0};
	FILE *fp;
	int oldtime=0,i=0;

	for(i=0;i<PVC_NUM;i++){/*find all possible time stamp*/
		sprintf(pppUptimePath,"/tmp/pppuptime-ppp%d",i);
		fileRead(pppUptimePath, pvc_info, sizeof(pvc_info));
		oldtime=atoi(pvc_info);
		if(oldtime){
			fp = fopen(pppUptimePath, "w+");
			if(fp){
				oldtime=oldtime+difference;
				fprintf(fp,"%d",oldtime);
				fclose(fp);
			}
		}
	}
}

#endif

#if defined(TCSUPPORT_EPON_OAM_CUC)
void cuc_ping_test_reset(mxml_node_t *top){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], CUC_PING_TEST_NODE_NAME);
	strcpy(nodeName[1], CUC_PING_TEST_ENTRY);
	setAttrValue(top,nodeName,"Result",CUC_PING_TEST_RESULT_NOT_START);
	setAttrValue(top,nodeName,"TXPkt","0");
	setAttrValue(top,nodeName,"RxPkt","0");
	setAttrValue(top,nodeName,"LostPkt","0");
	setAttrValue(top,nodeName,"LostPktRatio","0");
	setAttrValue(top,nodeName,"MinDelay","0");
	setAttrValue(top,nodeName,"MaxDelay","0");
	setAttrValue(top,nodeName,"AvgDelay","0");
}

void cuc_ping_test_stop(mxml_node_t *top, int reason){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char destIP[32] = {0};
	FILE *fp= NULL;
	char tmp[64] = {0};
	int pid = 0;
	struct sockaddr_in ipAddr;

	//kill ping 
	fp = fopen(CUC_PING_TEST_PID_PATH, "r");
	if(fp){
		//kill the ping process
		fgets(tmp, sizeof(tmp), fp);
		pid = atoi(tmp);
		if(pid != 0){ 
			memset(tmp,0,sizeof(tmp));
			sprintf(tmp,"kill -9 %d", pid);
		   	system(tmp);
		}
		
		//delete the pid file
		fclose(fp);
		unlink(CUC_PING_TEST_PID_PATH);
	}
	
	//delete route
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], CUC_PING_TEST_NODE_NAME);
	strcpy(nodeName[1], CUC_PING_TEST_ENTRY);
	if((getAttrValue(top, nodeName, "DestIP", destIP) == TCAPI_PROCESS_OK) 
		&& strcmp(destIP,"")){
		if(inet_aton(destIP,&ipAddr.sin_addr)){
			memset(tmp,0,sizeof(tmp));
			sprintf(tmp,CUC_PING_TEST_DELROUTE_CMD, destIP);
			//tcdbg_printf("\ncuc_ping_test_stop, CMD = %s", tmp);
			system(tmp);
		}
	}
	else{
		tcdbg_printf("\ncuc_ping_test_stop, get DestIP fail");
	}
	
	if(reason == 2){
		return;
	}
	cuc_ping_test_reset(top);
}

int
cuc_ping_test_get_wanif(mxml_node_t *top, char *ifName){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char str_valid_if[128] = {0};
	char* p=NULL;
	int i, pvc_index, entry_index;	
	char value[64] = {0};
	int ret;

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], "WanInfo");
	strcpy(nodeName[1], "Common");
	if((ret = getAttrValue(top, nodeName, "ValidIFIndex", str_valid_if)) != TCAPI_PROCESS_OK){
		tcdbg_printf("\ncuc_ping_test_get_wanif, get WanInfo ValidIFIndex fail!!, %d", ret);
		return 0;
	}
	
	p=strtok(str_valid_if, ",");
	while(p)
	{
		i = atoi(p);
		pvc_index = i / 8;
		entry_index = i - pvc_index * 8;	

		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0],"Wan");
		sprintf(nodeName[1], "%s%d", "PVC", pvc_index);
		sprintf(nodeName[2], "%s%d", "Entry", entry_index);
		if((getAttrValue(top, nodeName, "ServiceList", value) == TCAPI_PROCESS_OK)
			&& (strstr(value, "VOICE"))){
			if((getAttrValue(top, nodeName, "IFName", ifName) == TCAPI_PROCESS_OK)
				&& (strcmp(ifName, ""))){
				//check if wan is active and have ip
				memset(value, 0, sizeof(value));
				memset(nodeName, 0, sizeof(nodeName));
				strcpy(nodeName[0],"WanInfo");
				sprintf(nodeName[1], "Entry%d", i);	
				if((getAttrValue(top, nodeName, "GateWay", value) == TCAPI_PROCESS_OK)
					&& (strlen(value) != 0) && (strcmp(value,"N/A"))){
					tcdbg_printf("\ncuc_ping_test_get_wanif, find wan interface");
					return 1;
				}
				else{
					tcdbg_printf("\ncuc_ping_test_get_wanif, checkWan_by_name fail!!");
				}
			}
			else{
				tcdbg_printf("\ncuc_ping_test_get_wanif, get IFName fail");
			}
		}
		p=strtok(NULL, ",");
	}
	tcdbg_printf("\ncuc_ping_test_get_wanif, not found VOICE Wan");
	return 0;	
}
/*______________________________________________________________________________
**	cuc_ping_test_boot
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
cuc_ping_test_boot(mxml_node_t *top){

	cuc_ping_test_reset(top);
	
	return 0;
}

/*______________________________________________________________________________
**	cuc_ping_test_execute
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
cuc_ping_test_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char tmp[64] = {0};
	char count[16] = {0}, datalen[16]={0}, timeout[16] = {0}, destIP[32] = {0};
	struct sockaddr_in ipAddr;
	char ifName[16] = {0};
	int ret;

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], CUC_PING_TEST_NODE_NAME);
	strcpy(nodeName[1], CUC_PING_TEST_ENTRY);
	
	if((ret = getAttrValue(top, nodeName, "PingAction", tmp)) == TCAPI_PROCESS_OK){
		if(0 == strcmp(tmp,"1")){
			//tcdbg_printf("\nPing test start");
			cuc_ping_test_stop(top,1);
			setAttrValue(top,nodeName,"Result", CUC_PING_TEST_RESULT_RUNNING);
			
			if(getAttrValue(top, nodeName, "PingTimes", count) == TCAPI_PROCESS_OK
			    && getAttrValue(top, nodeName, "PingSize", datalen) == TCAPI_PROCESS_OK
			    && getAttrValue(top, nodeName, "PingTimeout", timeout) == TCAPI_PROCESS_OK
			    && getAttrValue(top, nodeName, "DestIP", destIP) == TCAPI_PROCESS_OK){
				if(!strcmp(destIP,"") ||! inet_aton(destIP,&ipAddr.sin_addr)){
					tcdbg_printf("\nPing test, destIP format error, destIP = %s", destIP);
					setAttrValue(top,nodeName,"Result", CUC_PING_TEST_RESULT_UNKNOWN_IP);
					return 0;
				}
				if(cuc_ping_test_get_wanif(top, ifName)){// get VOIP interface
					//route add
					memset(tmp,0,sizeof(tmp));
					sprintf(tmp,CUC_PING_TEST_ADDROUTE_CMD, destIP, ifName);
					//tcdbg_printf("\nPing test CMD1 = %s", tmp);
					system(tmp);
					//ping cmd
					memset(tmp,0,sizeof(tmp));
					sprintf(tmp,CUC_PING_TEST_CMD, count, datalen, timeout, destIP);
					//tcdbg_printf("\nPing test CMD2 = %s", tmp);
					system(tmp);
					return 0;
				}
				else{
					tcdbg_printf("\nPing test, not found VOIP interface!!!");
					setAttrValue(top,nodeName,"Result", CUC_PING_TEST_RESULT_NO_ROUTE);
					return 0;
				}
			}
			else{
				tcdbg_printf("\nPing test, get Attribute  value fail");
				return -1;
			}
		}
		else if(0 == strcmp(tmp,"0")){
			//tcdbg_printf("\nPing test stop");
			cuc_ping_test_stop(top,0);
			return 0;
		}
		else if(0 == strcmp(tmp,CUC_PING_TEST_ACTION_COMPLETE)){
			tcdbg_printf("\nPing test complete");
			cuc_ping_test_stop(top,2);
			return 0;
		}
		else{
			tcdbg_printf("\nPing test, PingAction not 0,1,2");
			return 0;
		}
	}
	else{
		tcdbg_printf("\nPing test, get PingAction value faill!%d", ret);
		return -1;
	}
}


int
cuc_ping_test_init(void){
	int ret=0;
	cfg_node_t node={
			.name="CucPing",
			.type=SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=cuc_ping_test_execute,
			.cfg_boot=cuc_ping_test_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}

#endif


#if defined(TCSUPPORT_PPPOE_SIMULATE)
void pppoe_simulate_stop(mxml_node_t *top, int reason){
	FILE *fp = NULL;
	char pid_tmp[50] = {0};
	int pid = 0;
	char cmdbuf[128] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char vlan_id[8] = {0}, nas_name[16] = {0};

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], PPPOE_SIMULATE_NODE_NAME);
	strcpy(nodeName[1], PPPOE_SIMULATE_ENTRY);
	fp = fopen(PPPOE_SIMULATE_PID_PATH, "r");
	if(fp)
	{
		//kill the ping process
		fgets(pid_tmp, sizeof(pid_tmp), fp);
		pid = atoi(pid_tmp);
		if(pid != 0)
		{ 
			memset(cmdbuf,0,sizeof(cmdbuf));
			sprintf(cmdbuf,"kill -9 %d", pid);
		   	system(cmdbuf);
		}
		
		//delete the pid file
		fclose(fp);
		unlink(PPPOE_SIMULATE_PID_PATH);
	}
	if((getAttrValue(top, nodeName, "VLANID", vlan_id) == TCAPI_PROCESS_OK)
		&& (getAttrValue(top, nodeName, "NASName", nas_name) == TCAPI_PROCESS_OK)){
		memset(cmdbuf, 0, sizeof(cmdbuf));
		sprintf(cmdbuf, "/sbin/ifconfig %s down", nas_name);
		system(cmdbuf);
		//tcdbg_printf("\npppoe sim stop cmdbuf1 = %s", cmdbuf);

		memset(cmdbuf, 0, sizeof(cmdbuf));
		sprintf(cmdbuf, "/usr/bin/smuxctl rem %s %s", nas_name, vlan_id);
		system(cmdbuf);
		//tcdbg_printf("\npppoe sim stop cmdbuf2 = %s", cmdbuf);
	}else{
		tcdbg_printf("\npppoe_simulate_stop, get VLANID or NASName error!!!");
	}
	if(reason == PPPOE_SIMULATE_COMPLETE){
		return;
	}
#if !defined(TCSUPPORT_CT_PPPOE_IPV6_EMULATOR)
	setAttrValue(top,nodeName,"PPPSessionID","");
	setAttrValue(top,nodeName,"ExternalIPAddress","");
	setAttrValue(top,nodeName,"DefaultGateway","");
#endif
	if(reason == PPPOE_SIMULATE_USER_STOP){
		setAttrValue(top,nodeName,"DiagnosticsState","None");
		setAttrValue(top,nodeName,"Result","UserStop");
	}
	if(reason == PPPOE_SIMULATE_UNKNOW_STOP){
		setAttrValue(top,nodeName,"Result","unknown");
	}
}

/*______________________________________________________________________________
**	pppoe_simulate_boot
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
pppoe_simulate_boot(mxml_node_t *top){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], PPPOE_SIMULATE_NODE_NAME);
	strcpy(nodeName[1], PPPOE_SIMULATE_ENTRY);
	setAttrValue(top,nodeName,"DiagnosticsState","None");
	setAttrValue(top,nodeName,"Result","unknown");
	setAttrValue(top,nodeName,"PPPSessionID","");
	setAttrValue(top,nodeName,"ExternalIPAddress","");
	setAttrValue(top,nodeName,"DefaultGateway","");

	pppoe_simulate_write(top,top);
	return 0;
}
/*______________________________________________________________________________
**	pppoe_simulate_write
**
**	descriptions:
**		Find PPPoESimulate element from mxml tree then write pppoe simulate's system parameter to 
**		specify configuration files.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
pppoe_simulate_write(mxml_node_t *top, mxml_node_t *parant){

	char fpath[64];
	mxml_node_t *node = NULL;

	/*create pppoe_sim.conf*/
	sprintf(fpath, "%s", PPPOE_SIMULATE_CONF);

	node = mxmlFindElement(top, top, PPPOE_SIMULATE_NODE_NAME,
	  		NULL, NULL,
	  		MXML_DESCEND_FIRST);
	if (node == NULL){
		tcdbg_printf("\npppoe_simulate_write not find PPPoESimulate node");
		return -1;
	}
	node = mxmlFindElement(node, node, PPPOE_SIMULATE_ENTRY,
	  		NULL, NULL,
	  		MXML_DESCEND);
	if(node != NULL){
		if(node_write(node, fpath, QMARKS) != 0){
			tcdbg_printf("\npppoe_simulate_write write PPPoESimulate_Entry error!");
			return -1;
		}
	}

	return 0;
}/* end wan_write */

/*______________________________________________________________________________
**	pppoe_simulate_execute
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
pppoe_simulate_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char diag_state[16] = {0};
	
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], PPPOE_SIMULATE_NODE_NAME);
	strcpy(nodeName[1], PPPOE_SIMULATE_ENTRY);
	if(getAttrValue(top, nodeName, "DiagnosticsState", diag_state) == TCAPI_PROCESS_OK){
		if(0 == strcasecmp(diag_state,"Start")){
			tcdbg_printf("\nPPPoE Simulate Start");
			pppoe_simulate_stop(top, PPPOE_SIMULATE_UNKNOW_STOP);
			setAttrValue(top,nodeName,"DiagnosticsState","Running");
			system(PPPOE_SIMULATE_SH);
			return 0;
		}
		else if(0 == strcasecmp(diag_state,"Stop")){
			tcdbg_printf("\nPPPoE Simulate Stop");
			pppoe_simulate_stop(top, PPPOE_SIMULATE_USER_STOP);
			return 0;
		}else if(0 == strcasecmp(diag_state,"Complete")){
			tcdbg_printf("\nPPPoE Simulate Complete");
			pppoe_simulate_stop(top, PPPOE_SIMULATE_COMPLETE);
			return 0;
		}
		else{
			tcdbg_printf("\nPPPoE Simulate DiagnosticsState Other State");
			return 0;
		}
	}
	else{
		tcdbg_printf("\nPPPoE Simulate get DiagnosticsState Fail!");
		return -1;
	}
}

int
pppoe_simulate_init(void){
	int ret=0;
	cfg_node_t node={
			.name="PPPoESimulate",
			.type=SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=pppoe_simulate_write,
			.cfg_verify=NULL,
			.cfg_execute=pppoe_simulate_execute,
			.cfg_boot=pppoe_simulate_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}
#endif

#if defined(TCSUPPORT_CT_PPPOE_EMULATOR)
#if defined(TCSUPPORT_CT_PPPOE_IPV6_EMULATOR)
int pppoe_emu_boot = 0;
int getV6SlaacPrefix(char *outaddr)
{
	FILE *fp = NULL;
	char addr_all[64] = {0}, slaac_addr[64] = {0}, tmp[40] = {0}, prelen[12] = {0};

	if ( !outaddr )
		return -1;

	fp = fopen(PPPOE_EMULATOR_SLAACPREFIX_PATH, "r");
	if ( fp )
	{
		memset(addr_all, 0, sizeof(addr_all));
		memset(slaac_addr, 0, sizeof(slaac_addr));
		memset(tmp, 0, sizeof(tmp));
		memset(prelen, 0, sizeof(prelen));
		fgets(addr_all, sizeof(addr_all), fp);
		close(fp);

		if ( 0 != addr_all[0] && '\n' != addr_all[0] )
		{
			sscanf(addr_all, "%39s/%s", tmp, prelen);
			shrink_ipv6addr(tmp, slaac_addr, sizeof(slaac_addr));
			strcpy(outaddr, slaac_addr);
			return 0;
		}
	}

	return -1;
}

int saveWanV6Prefix(int mode, char *nodeName, char *wanip)
{
	struct sockaddr_in6 in_wan_v6addr = {0};
	int i = 0, prefixLEN = 64;
	char val[64] = {0};

	if ( !nodeName || !wanip )
		return -1;

	if ( 0 == mode ) // dhcp mode
	{
		if ( 1 != inet_pton(AF_INET6, wanip, &in_wan_v6addr.sin6_addr) )
		return -1;
		for ( i = prefixLEN/8; i < 16; i ++)
			in_wan_v6addr.sin6_addr.s6_addr[i] = 0;

		in_wan_v6addr.sin6_family = AF_INET6;
		getnameinfo((struct sockaddr *)&in_wan_v6addr,
					sizeof(struct sockaddr_in6),
					val,
					sizeof(val),
					NULL, 0, NI_NUMERICHOST);
	}
	else if ( 1 == mode ) // slaac mode
	{
		if ( 0 != getV6SlaacPrefix(val) )
			return -1;
	}
	else
		return -1;

	tcapi_set(nodeName, "WANIPv6Prefix", val);

	return 0;
}

int getV6geteway(char *outgw)
{
	FILE *fp = NULL;
	char gateway[64] = {0}, tmp[64] = {0};

	if ( !outgw )
		return -1;

	fp = fopen(PPPOE_EMULATOR_GWV6_PATH, "r");
	if ( fp )
	{
		memset(tmp, 0, sizeof(tmp));
		memset(gateway, 0, sizeof(gateway));
		fread(tmp, sizeof(char), 39, fp);
		close(fp);

		if ( 0 != tmp[0] && '\n' != tmp[0] )
		{
			shrink_ipv6addr(tmp, gateway, sizeof(gateway));
			strcpy(outgw, gateway);
			return 0;
		}
	}

	return -1;
}

int getV6SlaacAddr(char *outaddr)
{
	FILE *fp = NULL;
	char addr_all[64] = {0}, slaac_addr[64] = {0}, tmp[40] = {0}, prelen[12] = {0};

	if ( !outaddr )
		return -1;

	fp = fopen(PPPOE_EMULATOR_SLAACV6_PATH, "r");
	if ( fp )
	{
		memset(addr_all, 0, sizeof(addr_all));
		memset(slaac_addr, 0, sizeof(slaac_addr));
		memset(tmp, 0, sizeof(tmp));
		memset(prelen, 0, sizeof(prelen));
		fgets(addr_all, sizeof(addr_all), fp);
		close(fp);

		if ( 0 != addr_all[0] && '\n' != addr_all[0] )
		{
			sscanf(addr_all, "%39s %s", tmp, prelen);
			shrink_ipv6addr(tmp, slaac_addr, sizeof(slaac_addr));
			strcpy(outaddr, slaac_addr);
			return 0;
		}
	}

	return -1;
}

int checkRA_MFlags()
{
	FILE *fp = NULL;
	char temp[64] = {0};
	int ra_flags = 0;

	fp = fopen(PPPOE_EMULATOR_RAFLAGS_PATH, "r");
	if ( fp )
	{
		memset(temp, 0, sizeof(temp));
		fgets(temp, sizeof(temp), fp);
		close(fp);

		if ( 0 != temp[0] )
		{
			sscanf(temp, "%d", &ra_flags);
			if ( 0 != ra_flags )
				return ra_flags;
		}
	}

	return 0;
}


int pppoeEmulatorStart = 0;
int pppoeEmuIfaceDel = 0;
int pppoeEmuIfaceDel_timercnt = 0;
int pppoeEmudnsroute_add = 0;
int pppoeEmuDsliteOK = 0;
char pppoeEmudns[2][64] = {0};
struct timespec pppemu_Start = {0};
extern void restart_dnsmasq();

int startPPPoE_Emulator_timer()
{
	pppoeEmudnsroute_add = 0;
	memset(pppoeEmudns, 0, sizeof(pppoeEmudns));
	memset(&pppemu_Start, 0, sizeof(pppemu_Start));
	clock_gettime(CLOCK_MONOTONIC, &pppemu_Start);
	pppoeEmuDsliteOK = 0;
	pppoeEmulatorStart = 1;

	return 0;
}

int stopPPPoE_Emulator_timer()
{
	pppoeEmulatorStart = 0;
	memset(pppoeEmudns, 0, sizeof(pppoeEmudns));
	unlink(PPPOE_EMULATOR_DOMAIN_FILE);
	if ( pppoeEmudnsroute_add )
		restart_dnsmasq();
	pppoeEmudnsroute_add = 0;
	pppoeEmuDsliteOK = 0;

	return 0;
}

int isPPPoE_Emulator_Start()
{
	return pppoeEmudnsroute_add;
}

int addEMUDNSConfig(FILE* fp, FILE* fpInfo)
{
	char  buf[256] = {0};

	if ( !fp || !fpInfo )
		return -1;

	if ( 0 != pppoeEmudns[0][0] )
	{
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "server=%s@%s\n", pppoeEmudns[0], PPPOE_EMULATOR_PPPNAME); 		
		fwrite(buf, sizeof(char), strlen(buf), fp);

		memset(buf, 0, sizeof(buf));
		sprintf(buf, "%s %s %s\n", pppoeEmudns[0], PPPOE_EMULATOR_PPPNAME, "PPPEMU");
		fwrite(buf, sizeof(char), strlen(buf), fpInfo);
	}

	if ( 0 != pppoeEmudns[1][0] )
	{
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "server=%s@%s\n", pppoeEmudns[1], PPPOE_EMULATOR_PPPNAME); 		
		fwrite(buf, sizeof(char), strlen(buf), fp);

		memset(buf, 0, sizeof(buf));
		sprintf(buf, "%s %s %s\n", pppoeEmudns[1], PPPOE_EMULATOR_PPPNAME, "PPPEMU");
		fwrite(buf, sizeof(char), strlen(buf), fpInfo);

	}

	return 0;
}

int resolveDSLiteDomain(char *domain, char dns[][64], char *gw6, int gotanip6,  char *nodeName)
{
	char  cmdbuf[256] = {0};
	struct addrinfo hints = {0};
	struct addrinfo *res = NULL;
	int retCode = -1;
	struct sockaddr_in6 *p_dslite_v6addr = NULL;
	char dslite_addr_buf[64] = {0};

	if ( pppoeEmuDsliteOK )
		return 0;

	if ( !domain || !gw6 || !nodeName )
		return retCode;

	if ( 0 == domain[0] || 0 == dns[0][0]
		|| 0 == gw6[0] || 0 == gotanip6 )
		return retCode;

	snprintf(cmdbuf, sizeof(cmdbuf) - 1, "%s %s\n", domain, PPPOE_EMULATOR_PPPNAME);
	doValPut(PPPOE_EMULATOR_DOMAIN_FILE, cmdbuf);
	if ( 0 == pppoeEmudnsroute_add )
	{
		pppoeEmudnsroute_add = 1;
		if ( 0 == dns[1][0] )
		{
			snprintf(cmdbuf, sizeof(cmdbuf) - 1, PPPOE_EMULATOR_DNSROUTE_ADD, dns[0], gw6);
			system(cmdbuf);
			strcpy(pppoeEmudns[0], dns[0]);
		}
		else
		{
			snprintf(cmdbuf, sizeof(cmdbuf) - 1, PPPOE_EMULATOR_DNSROUTE_ADD, dns[0], gw6);
			system(cmdbuf);
			snprintf(cmdbuf, sizeof(cmdbuf) - 1, PPPOE_EMULATOR_DNSROUTE_ADD, dns[1], gw6);
			system(cmdbuf);

			strcpy(pppoeEmudns[0], dns[0]);
			strcpy(pppoeEmudns[1], dns[1]);
		}
		restart_dnsmasq();
	}

	// resolve dns
	memset(&hints, 0 , sizeof(hints));
	hints.ai_family = AF_INET6;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	if ( tc_getaddrinfo(domain, NULL, &hints, &res, 3) )
	{
		retCode = -1;
	}
	else
	{
		if ( NULL == res || AF_INET6 != res->ai_family )
			retCode = -1;
		else
		{
			p_dslite_v6addr = (struct sockaddr_in6 *)res->ai_addr;
			if ( NULL == p_dslite_v6addr )
				return -1;
			inet_ntop(AF_INET6, &p_dslite_v6addr->sin6_addr, dslite_addr_buf, INET6_ADDRSTRLEN);
			tcapi_set(nodeName, "AftrAddress", dslite_addr_buf);
			pppoeEmuDsliteOK = 1;
			retCode = 0;
		}
	}

	return retCode;
}

int checkPPPoE_Emulator_timer()
{
#define EMULATOR_TIMEOUT 30
	char nodeinfo[32] = {0};
	char buf[128] = {0}, ipmode[12] = {0}, dhcpv6mode[12] = {0}, wan_mode[32] = {0};
	mxml_node_t *top = NULL;
	char info_string[2][MAX_INFO_LENGTH];
	char path[128] = {0}, nas_name[64] = {0};
	int all_done = 1, ra_flags = 0, got_wanip6 = 0;
	char dns_svr6[2][64] = {0}, gw6_addr[64] = {0};
	struct timespec pppemu_Current = {0}, pppemu_diff = {0};
#if defined(TCSUPPORT_CT_PPPERRCODE_ENHANCE)
	char wan_ip[64] = {0};
#endif
#if defined(TCSUPPORT_CT_PON_CZ_GDCS)
	char auth_success[12] = {0};
#endif

	if ( pppoeEmuIfaceDel && 0 == pppoeEmulatorStart
		&& pppoeEmuIfaceDel_timercnt++ >= 1 )
	{
		pppoeEmuIfaceDel = 0;
		pppoeEmuIfaceDel_timercnt = 0;

		memset(nodeinfo, 0, sizeof(nodeinfo));
		sprintf(nodeinfo, "%s_%s", PPPOE_EMULATOR_NODE_NAME, PPPOE_EMULATOR_ENTRY);
		tcapi_get(nodeinfo, "NASName", nas_name);

		// remove interface
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "/sbin/ifconfig %s down", nas_name);
		system(buf);
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "/usr/bin/smuxctl rem %s", nas_name);
		system(buf);

		memset(buf, 0, sizeof(buf));
		sprintf(buf, "/bin/rm -r %s", PPPOE_EMULATOR_PPPIF_PATH);
		system(buf);
	}

	if ( pppoeEmulatorStart )
	{
		memset(nodeinfo, 0, sizeof(nodeinfo));
		sprintf(nodeinfo, "%s_%s", PPPOE_EMULATOR_NODE_NAME, PPPOE_EMULATOR_ENTRY);

		memset(&pppemu_Current, 0, sizeof(pppemu_Current));
		clock_gettime(CLOCK_MONOTONIC, &pppemu_Current);
		memset(&pppemu_diff, 0, sizeof(pppemu_diff));
		pppemu_diff.tv_sec = pppemu_Current.tv_sec - pppemu_Start.tv_sec;
		pppemu_diff.tv_nsec += pppemu_Current.tv_nsec - pppemu_Start.tv_nsec;
		while ( pppemu_diff.tv_nsec > 1000000000 )
		{
			pppemu_diff.tv_sec++;
			pppemu_diff.tv_nsec -= 1000000000;
		}
		if ( pppemu_diff.tv_nsec < 0 )
		{
			pppemu_diff.tv_sec--;
			pppemu_diff.tv_nsec += 1000000000;
		}

		if ( pppemu_diff.tv_sec >= EMULATOR_TIMEOUT )
		{
			stopPPPoE_Emulator_timer();

			if ( TCAPI_PROCESS_OK == tcapi_get(nodeinfo, "Result", buf)
				&& 0 == strcmp(buf, "unknown") )
			{
				if( TCAPI_PROCESS_OK == tcapi_get(nodeinfo, "WANPPPConnectionIPMode", ipmode)
					&& ( 0 == strcmp(ipmode, "2")
					|| 0 == strcmp(ipmode, "3") ))
				{
					tcapi_set(nodeinfo, "Result", ERROR_OUT_OF_RESOURCES);
#if defined(TCSUPPORT_CT_PPPERRCODE_ENHANCE)
					memset(buf, 0, sizeof(buf));
					if( TCAPI_PROCESS_OK ==
						tcapi_get(nodeinfo, "Dslite_Enable", buf)
						&& 0 == strcmp(buf, "0")
						&& TCAPI_PROCESS_OK ==
						tcapi_get(nodeinfo, "ExternalIPAddress", wan_ip)
						&& 0 == strcmp(wan_ip, "0.0.0.0") )
					{
						tcapi_set(nodeinfo, "Result", ERROR_PARAMNEGOFAILV4);
					}
#endif

				}
				else
				{
#if defined(TCSUPPORT_CT_PON_CZ_GDCS)
					if( TCAPI_PROCESS_OK == tcapi_get(nodeinfo, "Auth_Success", auth_success)
						&& ( 0 == strcmp(auth_success, "1")))
						tcapi_set(nodeinfo, "Result", "ParamNegoFail");
					else
#endif
					tcapi_set(nodeinfo, "Result", "Timeout");
			}
			}

			tcapi_set(nodeinfo, "DiagnosticsState", "Complete");
			tcapi_commit(nodeinfo);
			return 0;
		}

		// only check for IPv6 in 30s
		if( TCAPI_PROCESS_OK == tcapi_get(nodeinfo, "WANPPPConnectionIPMode", ipmode)
			&& 0 != ipmode[0] )
		{
			if ( 0 == strcmp(ipmode, "2") // IPv6
				|| 0 == strcmp(ipmode, "3") ) // IPv4/IPv6
			{
				if( TCAPI_PROCESS_OK == tcapi_get(nodeinfo, "WanMode", wan_mode)
					&& 0 == strcmp(wan_mode, "Route") )
				{
					// get IPv6 info from file
					if( TCAPI_PROCESS_OK == tcapi_get(nodeinfo, "DHCPv6", dhcpv6mode)
						&& 0 == strcmp(dhcpv6mode, "Yes") )
					{
						sprintf(path, "%s%s", PPPOE_EMULATOR_PPPIF_PATH, "ip6");
						memset(info_string, 0, sizeof(info_string));
						if ( SUCCESS == get_file_string(path, info_string, 1) )
						{
							got_wanip6 = 1;
							tcapi_set(nodeinfo, "WANIPv6IPAddress", info_string[0]);
							if ( 0 != saveWanV6Prefix(0, nodeinfo, info_string[0]) )
								all_done = 0;
						}
						else
							all_done = 0;
					}
					else
					{
						memset(info_string, 0, sizeof(info_string));
						if ( 0 == getV6SlaacAddr(info_string[0]) )
						{
							got_wanip6 = 1;
							tcapi_set(nodeinfo, "WANIPv6IPAddress", info_string[0]);
							if ( 0 != saveWanV6Prefix(1, nodeinfo, info_string[0]) )
								all_done = 0;
						}
						else
							all_done = 0;
					}
				}
				else // bridge mode
				{
					ra_flags = checkRA_MFlags();
					if ( 0 == ra_flags )
						all_done = 0;
					else
					{
						// get IPv6 info from file
						if( ra_flags & PPPOE_EMULATOR_MFLAG )
						{
							sprintf(path, "%s%s", PPPOE_EMULATOR_PPPIF_PATH, "ip6");
							memset(info_string, 0, sizeof(info_string));
							if ( SUCCESS == get_file_string(path, info_string, 1) )
							{
								got_wanip6= 1; 
								tcapi_set(nodeinfo, "WANIPv6IPAddress", info_string[0]);
								if ( 0 != saveWanV6Prefix(0, nodeinfo, info_string[0]) )
									all_done = 0;
							}
							else
								all_done = 0;
						}
						else
						{
							memset(info_string, 0, sizeof(info_string));
							if ( 0 == getV6SlaacAddr(info_string[0]) )
							{
								got_wanip6 = 1;
								tcapi_set(nodeinfo, "WANIPv6IPAddress", info_string[0]);
								if ( 0 != saveWanV6Prefix(1, nodeinfo, info_string[0]) )
									all_done = 0;	
							}
							else
								all_done = 0;
						}
					}
				}

				sprintf(path, "%s%s", PPPOE_EMULATOR_PPPIF_PATH, "pd6");
				memset(info_string, 0, sizeof(info_string));
				if ( SUCCESS == get_file_string(path, info_string, 1) )
				{
					sscanf(info_string[0], "%[^/]", buf);
					tcapi_set(nodeinfo, "LANIPv6Prefix", buf);
				}
				else
					all_done = 0;

				sprintf(path, "%s%s", PPPOE_EMULATOR_PPPIF_PATH, "dns6");
				memset(info_string, 0, sizeof(info_string));
				if ( SUCCESS == get_file_string(path, info_string, 2) )
				{
					memset(dns_svr6, 0, sizeof(dns_svr6));
					if ( 0 != info_string[1][0] )
					{
						sprintf(buf, "%s,%s", info_string[0], info_string[1]);
						tcapi_set(nodeinfo, "WANIPv6DNSServers", buf);
						snprintf(dns_svr6[0], sizeof(dns_svr6[0]) - 1, "%s", info_string[0]);
						snprintf(dns_svr6[1], sizeof(dns_svr6[1]) - 1, "%s", info_string[1]);
					}
					else
					{
						tcapi_set(nodeinfo, "WANIPv6DNSServers", info_string[0]);
						snprintf(dns_svr6[0], sizeof(dns_svr6[0]) - 1, "%s", info_string[0]);
					}
				}
				else
					all_done = 0;
				
				memset(info_string, 0, sizeof(info_string));
				if ( 0 == getV6geteway( info_string[0]) )
				{
					tcapi_set(nodeinfo, "WANDefaultIPv6Gateway", info_string[0]);
					snprintf(gw6_addr, sizeof(gw6_addr) - 1, "%s", info_string[0]);
				}
				else
					all_done = 0;

				memset(buf, 0, sizeof(buf));
				if( TCAPI_PROCESS_OK == tcapi_get(nodeinfo, "Dslite_Enable", buf)
					&& 0 == strcmp(buf, "1") )
				{
					sprintf(path, "%s%s", PPPOE_EMULATOR_PPPIF_PATH, "dsliteaddr");
					memset(info_string, 0, sizeof(info_string));
					if ( SUCCESS == get_file_string(path, info_string, 1) )
						tcapi_set(nodeinfo, "AftrAddress", info_string[0]);
					else
					{
						sprintf(path, "%s%s", PPPOE_EMULATOR_PPPIF_PATH, "dslitename");
						memset(info_string, 0, sizeof(info_string));
						if ( SUCCESS == get_file_string(path, info_string, 1) )
						{
							if ( 0 == pppoeEmuDsliteOK )
								tcapi_set(nodeinfo, "AftrAddress", info_string[0]);
							if ( 0 != resolveDSLiteDomain(info_string[0], dns_svr6, gw6_addr, got_wanip6, nodeinfo) )
								all_done = 0;
						}
						else
							all_done = 0;
					}
				}

				if ( all_done )
				{
					if ( 0 == strcmp(ipmode, "2") /* ipv6 */
						|| (0 == strcmp(ipmode, "3") /* ipv4/ipv6 */
						&& TCAPI_PROCESS_OK == tcapi_get(nodeinfo, "Result", buf)
						&& 0 == strcmp(buf, ERROR_OUT_OF_RESOURCES)) )
					{
						stopPPPoE_Emulator_timer();
						tcapi_set(nodeinfo, "DiagnosticsState", "Complete");
						tcapi_set(nodeinfo, "Result", "Success");
#if defined(TCSUPPORT_CT_PPPERRCODE_ENHANCE)
						memset(buf, 0, sizeof(buf));
						if( TCAPI_PROCESS_OK ==
							tcapi_get(nodeinfo, "Dslite_Enable", buf)
							&& 0 == strcmp(buf, "0")
							&& TCAPI_PROCESS_OK ==
							tcapi_get(nodeinfo, "ExternalIPAddress", wan_ip)
							&& 0 == strcmp(wan_ip, "0.0.0.0") )
						{
							tcapi_set(nodeinfo, "Result", ERROR_PARAMNEGOFAILV4);
						}
#endif
						tcapi_commit(nodeinfo);
					}
				}
			}
		}
	}

	return 0;
}

int create_pppoe_dhcp6c_conf(mxml_node_t *top)
{
	FILE *fp = NULL;
	char buf[64] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	int DSLite = 0;

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], PPPOE_EMULATOR_NODE_NAME);
	strcpy(nodeName[1], PPPOE_EMULATOR_ENTRY);

	if( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, "Dslite_Enable", buf)
		&& 0 == strcmp(buf, "1") )
		DSLite = 1;

	fp = fopen(PPPOE_EMULATOR_PPPDHCP6C_CONF, "w");
	if ( fp )
	{
		sprintf(buf, "interface %s {\n", PPPOE_EMULATOR_PPPNAME);
		fputs(buf, fp);

			fputs("\tsend ia-pd 213;\n", fp);
			fputs("\tsend ia-na 210;\n", fp);
	
		fputs("\trequest domain-name;\n\trequest domain-name-servers;\n\tscript \""PPPOE_EMULATOR_PPPDHCP6C_SCRIPT"\";\n", fp);
		if( DSLite )
			fputs("\trequest dslite-name;\n", fp);
		fputs("};\n", fp);

			fputs("id-assoc pd 213 {\n\tprefix-interface "PPPOE_EMULATOR_PPPNAME"\n\t{\n\t\tsla-len 0;\n\t};\n};\n", fp);
			fputs("id-assoc na 210 { };\n", fp);
	
		fclose(fp);
	}

	return 0;
}

int pppoeKillviafile(char *file)
{
	FILE *fp = NULL;
	char pid_tmp[50] = {0};
	int pid = 0;

	if ( !file )
		return -1;

	fp = fopen(file, "r");
	if ( fp )
	{
		fgets(pid_tmp, sizeof(pid_tmp), fp);
		close(fp);
		unlink(file);

		pid = atoi(pid_tmp);
		if( 0 != pid
			&& 0 != kill(pid, SIGTERM) )
		{
			if ( 0 !=  kill(pid, SIGKILL) )
				tcdbg_printf("\n%s kill old pppoe client failed(%s)!\n", __FUNCTION__, file);
		}
	}

	return 0;
}
#endif

void pppoe_Emulator_stop(mxml_node_t *top, int reason){
	FILE *fp = NULL;
	char pid_tmp[50] = {0};
	int pid = 0;
	char cmdbuf[128] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char vlan_id[8] = {0}, nas_name[16] = {0};
	char result[64] = {0};
	int i = 0;
 
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], PPPOE_EMULATOR_NODE_NAME);
	strcpy(nodeName[1], PPPOE_EMULATOR_ENTRY);

#if defined(TCSUPPORT_CT_PPPOE_IPV6_EMULATOR)
	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, PPPOE_EMULATOR_ATTR_NASNAME, nas_name)
		|| 0 == nas_name[0] )
		strcpy(nas_name, PPPOE_EMULATOR_NAS_NAME);
	stopPPPoE_Emulator_timer();

	pppoeKillviafile(PPPOE_EMULATOR_PPPDHCP6C_PATH);
	pppoeKillviafile(PPPOE_EMULATOR_PID_PATH);
	sleep(1);

	if ( PPPOE_EMULATOR_USER_STOP == reason
		|| PPPOE_EMULATOR_COMPLETE == reason )
	{
		pppoeEmuIfaceDel = 1;
		pppoeEmuIfaceDel_timercnt = 0;
	}
	else if ( PPPOE_EMULATOR_UNKNOW_STOP )
	{
		// remove interface
		memset(cmdbuf, 0, sizeof(cmdbuf));
		sprintf(cmdbuf, "/sbin/ifconfig %s down", nas_name);
		system(cmdbuf);
		memset(cmdbuf, 0, sizeof(cmdbuf));
		sprintf(cmdbuf, "/usr/bin/smuxctl rem %s", nas_name);
		system(cmdbuf);

		memset(cmdbuf, 0, sizeof(cmdbuf));
		sprintf(cmdbuf, "/bin/rm -r %s", PPPOE_EMULATOR_PPPIF_PATH);
		system(cmdbuf);
	}
#else
	fp = fopen(PPPOE_EMULATOR_PID_PATH, "r");
	if(fp)
	{
		//kill the ping process
		fgets(pid_tmp, sizeof(pid_tmp), fp);
		pid = atoi(pid_tmp);
		if(pid != 0)
		{ 
			if(getAttrValue(top, nodeName, "Result", result) == TCAPI_PROCESS_OK)
			{
				if(0 == strcmp(result, "Success"))
				{
			memset(cmdbuf,0,sizeof(cmdbuf));
					sprintf(cmdbuf,"kill -SIGTERM %d", pid);
		   	system(cmdbuf);

					goto CloseFile;
				}
				else
				{
					goto KillPPP;
					
				}
			}
			else
			{
				goto KillPPP;
		}
		
	}

KillPPP:
		memset(cmdbuf,0,sizeof(cmdbuf));
		sprintf(cmdbuf,"kill -9 %d", pid);
	   	system(cmdbuf);

		if(getAttrValue(top, nodeName, "NASName", nas_name) == TCAPI_PROCESS_OK)
		{
		memset(cmdbuf, 0, sizeof(cmdbuf));
		sprintf(cmdbuf, "/sbin/ifconfig %s down", nas_name);
		system(cmdbuf);
		//tcdbg_printf("\npppoe sim stop cmdbuf1 = %s", cmdbuf);

		memset(cmdbuf, 0, sizeof(cmdbuf));
		//sprintf(cmdbuf, "/usr/bin/smuxctl rem %s %s", nas_name, vlan_id);
		sprintf(cmdbuf, "/usr/bin/smuxctl rem %s", nas_name);
		system(cmdbuf);
		//tcdbg_printf("\npppoe sim stop cmdbuf2 = %s", cmdbuf);
	}
		else
		{
			memset(cmdbuf, 0, sizeof(cmdbuf));
			system("/sbin/ifconfig emulator down");
			system("/usr/bin/smuxctl rem emulator");
			
			tcdbg_printf("\npppoe_simulate_stop, get NASName error!!!");
		}

CloseFile:
		//delete the pid file
		fclose(fp);
		unlink(PPPOE_EMULATOR_PID_PATH);
	}
#endif
	if(reason == PPPOE_EMULATOR_COMPLETE){
#if defined(TCSUPPORT_CT_PPPOE_IPV6_EMULATOR)
		if ( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, "Result", result)
			&& 0 == strcmp(result, "UserAuthenticationFail") )
		{
			setAttrValue(top, nodeName, "WANIPv6IPAddress", "::");
			setAttrValue(top, nodeName, "WANDefaultIPv6Gateway", "::");
		}
#endif
		return;
	}
	setAttrValue(top,nodeName,"PPPSessionID","");
	setAttrValue(top,nodeName,"ExternalIPAddress","");
	setAttrValue(top,nodeName,"DefaultGateway","");
	if(reason == PPPOE_EMULATOR_USER_STOP){
		setAttrValue(top,nodeName,"DiagnosticsState","None");
		setAttrValue(top,nodeName,"Result","UserStop");
	}
	if(reason == PPPOE_EMULATOR_UNKNOW_STOP){
		setAttrValue(top,nodeName,"Result","unknown");
#if defined(TCSUPPORT_CT_PPPOE_IPV6_EMULATOR)
		setAttrValue(top, nodeName, "ConnectionError", "");
		setAttrValue(top, nodeName, "ExternalIPAddress", "0.0.0.0");
		setAttrValue(top, nodeName, "DefaultGateway", "0.0.0.0");
		setAttrValue(top, nodeName, "AftrAddress", "::");
		setAttrValue(top, nodeName, "WANIPv6IPAddress", "::");
		setAttrValue(top, nodeName, "WANIPv6DNSServers", "::");
		setAttrValue(top, nodeName, "WANIPv6Prefix", "::");
		setAttrValue(top, nodeName, "WANDefaultIPv6Gateway", "::");
#if defined(TCSUPPORT_CT_PPPERRCODE_ENHANCE)
		setAttrValue(top, nodeName, "ConnectionErrorCode", "");
#endif
#endif
	}
}

/*______________________________________________________________________________
**	pppoe_simulate_boot
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
pppoe_Emulator_boot(mxml_node_t *top){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], PPPOE_EMULATOR_NODE_NAME);
	strcpy(nodeName[1], PPPOE_EMULATOR_ENTRY);
	setAttrValue(top,nodeName,"DiagnosticsState","None");
	setAttrValue(top,nodeName,"Result","unknown");
	setAttrValue(top,nodeName,"PPPSessionID","");
	setAttrValue(top,nodeName,"ExternalIPAddress","");
	setAttrValue(top,nodeName,"DefaultGateway","");
	setAttrValue(top,nodeName,"NASName","emulator");
	setAttrValue(top,nodeName,"NAS_IF","pon");
	setAttrValue(top,nodeName,"PPPUnit","199");
	setAttrValue(top, nodeName, "PPPAuthenticationProtocol", "");
#if defined(TCSUPPORT_CT_PPPOE_IPV6_EMULATOR)
	setAttrValue(top, nodeName, "Username", "");
	setAttrValue(top, nodeName, "Password", "");
	setAttrValue(top, nodeName, "WANInterface", "");
	setAttrValue(top, nodeName, "PPPAuthenticationProtocol", "");
#if defined(TCSUPPORT_C7)
	setAttrValue(top, nodeName, "RetryTimes", "1");
#else
	setAttrValue(top, nodeName, "RetryTimes", "2");
#endif
#if defined(TCSUPPORT_CT_PPPERRCODE_ENHANCE)
	setAttrValue(top, nodeName, "RetryTimes", "1");
	setAttrValue(top, nodeName, "ConnectionErrorCode", "");
	setAttrValue(top, nodeName, "fakeRetryTimes", "");
	setAttrValue(top, nodeName, "fakePPPAuthenticationProtocol", "");
#endif
	setAttrValue(top, nodeName, "ConnectionError", "");
	setAttrValue(top, nodeName, "PPPSessionID", "");
	setAttrValue(top, nodeName, "ExternalIPAddress", "0.0.0.0");
	setAttrValue(top, nodeName, "DefaultGateway", "0.0.0.0");
	setAttrValue(top, nodeName, "WANPPPConnectionIPMode", "0");
	setAttrValue(top, nodeName, "Dslite_Enable", "0");
	setAttrValue(top, nodeName, "AftrAddress", "::");
	setAttrValue(top, nodeName, "WANIPv6IPAddress", "::");
	setAttrValue(top, nodeName, "WANIPv6DNSServers", "::");
	setAttrValue(top, nodeName, "WANIPv6Prefix", "::");
	setAttrValue(top, nodeName, "WANDefaultIPv6Gateway", "::");
	setAttrValue(top, nodeName, "LANIPv6Prefix", "::");
	chmod(PPPOE_EMULATOR_SH, 777); 
	chmod(PPPOE_EMULATOR_PPPV6UP_PRE_SCRIPT, 777); 
	chmod(PPPOE_EMULATOR_PPPV6UP_SCRIPT, 777); 
	chmod(PPPOE_EMULATOR_PPPDHCP6C_SCRIPT, 777); 
	pppoe_emu_boot = 1;
#endif
#if defined(TCSUPPORT_CT_PON_CZ_GDCS)
	setAttrValue(top, nodeName, "Auth_Success", "0");
#endif

	pppoe_Emulator_write(top,top);
	return 0;
}
/*______________________________________________________________________________
**	pppoe_simulate_write
**
**	descriptions:
**		Find PPPoESimulate element from mxml tree then write pppoe simulate's system parameter to 
**		specify configuration files.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
pppoe_Emulator_write(mxml_node_t *top, mxml_node_t *parant){

	char fpath[64];
	mxml_node_t *node = NULL;

	/*create pppoe_sim.conf*/
	sprintf(fpath, "%s", PPPOE_EMULATOR_CONF);

	node = mxmlFindElement(top, top, PPPOE_EMULATOR_NODE_NAME,
	  		NULL, NULL,
	  		MXML_DESCEND_FIRST);
	if (node == NULL){
		tcdbg_printf("\npppoe_simulate_write not find PPPoESimulate node");
		return -1;
	}
	node = mxmlFindElement(node, node, PPPOE_EMULATOR_ENTRY,
	  		NULL, NULL,
	  		MXML_DESCEND);
	if(node != NULL){
		if(node_write(node, fpath, QMARKS) != 0){
			tcdbg_printf("\npppoe_simulate_write write PPPoESimulate_Entry error!");
			return -1;
		}
	}

	return 0;
}/* end wan_write */

/*______________________________________________________________________________
**	pppoe_simulate_execute
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
pppoe_Emulator_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char cwmpNodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char diag_state[16] = {0};
	tcapi_msg_t msg;
	 
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], PPPOE_EMULATOR_NODE_NAME);
	strcpy(nodeName[1], PPPOE_EMULATOR_ENTRY);
	memset(cwmpNodeName,0,sizeof(cwmpNodeName));
	strcpy(cwmpNodeName[0], "Cwmp");
	strcpy(cwmpNodeName[1], "Entry");
	if(getAttrValue(top, nodeName, "DiagnosticsState", diag_state) == TCAPI_PROCESS_OK){
		if(0 == strcasecmp(diag_state,"Running")){
			tcdbg_printf("\nPPPoE Emulator Start");
			pppoe_Emulator_stop(top, PPPOE_EMULATOR_UNKNOW_STOP);
			setAttrValue(top,nodeName,"DiagnosticsState","Running");
#if defined(TCSUPPORT_CT_PPPOE_IPV6_EMULATOR)
			if ( 0 != mkdir(PPPOE_EMULATOR_PPPIF_PATH, 0666) )
				tcdbg_printf("\n%s warning create folder failed!\n", __FUNCTION__);
			create_pppoe_dhcp6c_conf(top);
#endif
			system(PPPOE_EMULATOR_SH);
#if defined(TCSUPPORT_CT_PPPOE_IPV6_EMULATOR)
			startPPPoE_Emulator_timer();
#endif
			return 0;
		}
		else if(0 == strcasecmp(diag_state,"Stop")){
			tcdbg_printf("\nPPPoE Emulator Stop");
			pppoe_Emulator_stop(top, PPPOE_EMULATOR_USER_STOP);
			return 0;
		}else if(0 == strcasecmp(diag_state,"Complete")){
			tcdbg_printf("\nPPPoE Emulator Complete");
			pppoe_Emulator_stop(top, PPPOE_EMULATOR_COMPLETE);
			setAttrValue(top,cwmpNodeName,"pppoe_emulator_finish_flag","1");
			
			memset(&msg,0,sizeof(msg)) ;
			strcpy(msg.node, cwmpNodeName[0]);
			tcapi_commit_req(top, &msg);
			return 0;
		}
		else{
			tcdbg_printf("\nPPPoE Emulator DiagnosticsState Other State");
			return 0;
		}
	}
	else{
		tcdbg_printf("\nPPPoE Simulate get DiagnosticsState Fail!");
		return -1;
	}
}

int
pppoeEmulator_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="PppoeEmulator",
			.type=SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=pppoe_Emulator_write,
			.cfg_verify=NULL,
			.cfg_execute=pppoe_Emulator_execute,
			.cfg_boot=pppoe_Emulator_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}

pthread_t PppoeEmulatorBridgeThread_id = 0;
int PppoeEmulatorBridgeTick = 0;
extern mxml_node_t **gTreePtr;
int isFirstLinkup = 0;

void pppoe_Emulator_Bridge_wait(mxml_node_t *top, int pid)
{
	int i = 0;
	char cmd[64] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	 
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], PPPOE_EMULATOR_BRIDGE_INFO_NODE_NAME);
	strcpy(nodeName[1], PPPOE_EMULATOR_ENTRY);

	for(i = 0; i < PPPOE_EMULATOR_BRIDGE_WAIT_TIMEOUT; i++)
	{
		if(1 == CheckPid(pid))
		{	
			//tcdbg_printf("\nsleep 1 second, %d", i);
			sleep(1);
		}
		else
		{
			break;
		}
	}

	if(i == PPPOE_EMULATOR_BRIDGE_WAIT_TIMEOUT)      // timeout, kill the process
	{
		snprintf(cmd, sizeof(cmd), "kill -9 %d", pid);
		system(cmd);
	}
	else
	{
		// do nothing
	}
	
}

void pppoe_Emulator_Bridge_stop(mxml_node_t *top, int flag)
{
	FILE *fp = NULL;
	char pid_tmp[50] = {0};
	int pid = 0;
	char cmdbuf[128] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char vlan_id[8] = {0}, nas_name[16] = {0};
 
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], PPPOE_EMULATOR_BRIDGE_INFO_NODE_NAME);
	strcpy(nodeName[1], PPPOE_EMULATOR_ENTRY);
	fp = fopen(PPPOE_EMULATOR_BRIDGE_PID_PATH, "r");
	if(fp)
	{
		//kill the ping process
		fgets(pid_tmp, sizeof(pid_tmp), fp);
		pid = atoi(pid_tmp);
		fclose(fp);
		if(pid != 0)
		{
#if 1
			if(flag == PPPOE_EMULATOR_STOP_NOWAIT_FLAG)                  // pppoe_Emulator_Bridge_stop() is called before kill thread, just kill pppd would be ok, in case tcapi blocked
			{
				snprintf(cmdbuf, sizeof(cmdbuf), "kill -9 %d", pid);
				system(cmdbuf);
			}
			else                                                         // pppoe_Emulator_Bridge_stop() is called during emulating
			{
				pppoe_Emulator_Bridge_wait(top, pid);
			}
#else
			waitpid(pid, NULL, 0);
#endif
		}
		
		//delete the pid file
		unlink(PPPOE_EMULATOR_BRIDGE_PID_PATH);
	}
	
	if((getAttrValue(top, nodeName, "VLANID", vlan_id) == TCAPI_PROCESS_OK)
		&& (getAttrValue(top, nodeName, "NASName", nas_name) == TCAPI_PROCESS_OK)){
		memset(cmdbuf, 0, sizeof(cmdbuf));
		sprintf(cmdbuf, "/sbin/ifconfig %s down", nas_name);
		system(cmdbuf);
		//tcdbg_printf("\npppoe sim stop cmdbuf1 = %s", cmdbuf);

		memset(cmdbuf, 0, sizeof(cmdbuf));
//		sprintf(cmdbuf, "/usr/bin/smuxctl rem %s %s", nas_name, vlan_id);
		sprintf(cmdbuf, "/usr/bin/smuxctl rem %s", nas_name);
		system(cmdbuf);
		//tcdbg_printf("\npppoe sim stop cmdbuf2 = %s", cmdbuf);
	}
}


int
pppoe_Emulator_Bridge_write(mxml_node_t *top){

	char fpath[64];
	mxml_node_t *node = NULL;

	/*create pppoe_sim.conf*/
	sprintf(fpath, "%s", PPPOE_EMULATOR_BRIDGE_CONF);

	node = mxmlFindElement(top, top, PPPOE_EMULATOR_BRIDGE_INFO_NODE_NAME,
	  		NULL, NULL,
	  		MXML_DESCEND_FIRST);
	if (node == NULL){ 
		tcdbg_printf("\npppoe_simulate_bridge_write not find "PPPOE_EMULATOR_BRIDGE_INFO_NODE_NAME" node\r\n");
		return -1;
	}
	node = mxmlFindElement(node, node, PPPOE_EMULATOR_ENTRY,
	  		NULL, NULL,
	  		MXML_DESCEND);
	if(node != NULL){
		if(node_write(node, fpath, QMARKS) != 0){
			tcdbg_printf("\npppoe_simulate_bridge_write write PppoeEmulatorBridhr_Entry error!\r\n");
			return -1;
		}
	}

	return 0;
}/* end wan_write */


int PppBridgeEmulator(int index)
{
	char	 wan_node[MAX_ARG_NUM][MAX_NODE_NAME];
	char	 Info_node[MAX_ARG_NUM][MAX_NODE_NAME];
	char	 BridgeEmulator_node[MAX_ARG_NUM][MAX_NODE_NAME];
	int pvc_index, entry_index;
	mxml_node_t *top;
	char acWanMode[8] = {0};
	char acVlanid[8] = {0};
	char acTmp[65] = {0};
	int WanLastEmulator = 0;
	int TimeList = 0;
	char acWanName[16] = {0};
	char acActive[8] = {0};
	char acServiceList[32] = {0};
	char acVlanMode[16] = {0};
	int iRet = 0;
		
	top =  *gTreePtr;
	
	pvc_index = index / MAX_SMUX_NUM;
	entry_index = index - pvc_index * MAX_SMUX_NUM;
	
	memset(wan_node, 0, sizeof(wan_node));
	memset(Info_node, 0, sizeof(Info_node));
	memset(BridgeEmulator_node, 0, sizeof(BridgeEmulator_node));
	
	strcpy(wan_node[0], "Wan");
	sprintf(wan_node[1], "PVC%d", pvc_index);
	sprintf(wan_node[2], "Entry%d", entry_index);
	
	strcpy(BridgeEmulator_node[0], "PppoeEmulatorBridge");
	strcpy(BridgeEmulator_node[1], "Entry");
	
	getAttrValue(top, wan_node, "WanMode", acWanMode);
	getAttrValue(top, wan_node, "Active", acActive);
	getAttrValue(top, wan_node, "ServiceList", acServiceList);
	getAttrValue(top, wan_node, "LastEmulatorTick", acTmp);
	getAttrValue(top, wan_node, "VLANMode", acVlanMode);
	WanLastEmulator = atoi(acTmp);
	if ((0 == strcmp(acWanMode, "Bridge")) && (0 == strcmp(acActive,"Yes")) && (NULL != strstr(acServiceList,"INTERNET")))
	{
		if (1 == PppoeEmulatorBridgeTick || 0 == WanLastEmulator) //first tick, clean wan tick, run emulator
		{
			setAttrValue(top, wan_node, "LastEmulatorTick", "1");
		}
		else
		{		
			//check last error			
			iRet = getAttrValue(top, wan_node, "ConnectionError", acTmp);
			if ((0 != iRet) ||( (0 != strcmp("ERROR_NONE", acTmp)) && (0 != strcmp("ERROR_AUTHENTICATION_FAILURE", acTmp))))
			{
				getAttrValue(top, BridgeEmulator_node, "FailRetryTimeList", acTmp);				
			}
			else
			{
				getAttrValue(top, BridgeEmulator_node, "TimeList", acTmp);
			}
			
			TimeList = atoi(acTmp);
			if ((PppoeEmulatorBridgeTick - WanLastEmulator < TimeList) && (1 != isFirstLinkup))
			{
				return 0;
			}
		}
		
		getAttrValue(top, wan_node, "VLANID", acVlanid);
		strcpy(Info_node[0], PPPOE_EMULATOR_BRIDGE_INFO_NODE_NAME);
		strcpy(Info_node[1], PPPOE_EMULATOR_ENTRY);
		
		setAttrValue(top, Info_node, "NASName", "emulatorB"); 
		setAttrValue(top, Info_node, "NAS_IF", "pon"); 
		setAttrValue(top, Info_node, "PPPUnit", "198"); 
		setAttrValue(top, Info_node, "RetryTimes", "0"); 
		getAttrValue(top, BridgeEmulator_node, "Username", acTmp);
		setAttrValue(top, Info_node, "Username", acTmp); 
		getAttrValue(top, BridgeEmulator_node, "Password", acTmp);
		setAttrValue(top, Info_node, "Password", acTmp); 
		
		setAttrValue(top, Info_node, "VLANMode", acVlanMode);
		setAttrValue(top, Info_node, "VLANID", acVlanid); 
		//setAttrValue(top, Info_node, "PPPAuthenticationProtocol", "CHAP");  //auto mode
		sprintf(acWanName, "Wan_PVC%d_Entry%d", pvc_index, entry_index);
		setAttrValue(top, Info_node, "Wan", acWanName);

		pppoe_Emulator_Bridge_write(top);

		sprintf(acTmp, "%d", PppoeEmulatorBridgeTick);
		setAttrValue(top, wan_node, "LastEmulatorTick", acTmp);

		system(PPPOE_EMULATOR_BRIDGE_SH);  //wait for pppd finish
		pppoe_Emulator_Bridge_stop(top, PPPOE_EMULATOR_STOP_WAIT_FLAG);
		
	}
}

int lastRunTimeSec = 0;
int lastRunTimeUsec = 0;


void getSelectTime(struct timeval *tv)
{
	FILE *fp = NULL;
	int nowSec = 0;
	int nowUsec = 0;
	
	tv->tv_sec = 60;
	tv->tv_usec = 0;

	fp = fopen("/proc/uptime", "r");
	if (NULL == fp)
	{		
		return;
	}
	fscanf(fp, "%d.%d", &nowSec, &nowUsec);
	//tcdbg_printf("uptime:%d.%d\r\n", nowSec, nowUsec);
	if (lastRunTimeSec != 0)
	{
		if (nowSec - lastRunTimeSec >= 60)
		{
			tv->tv_sec = 0;
			tv->tv_usec = 1000;			
		}
		else
		{
			tv->tv_sec = 60 - (nowSec - lastRunTimeSec);
			tcdbg_printf("nowUsec:%d.lastRunTimeSec%d\r\n", nowSec, lastRunTimeSec);
			if (nowUsec > lastRunTimeUsec)
			{
				tv->tv_usec =999999 - (nowUsec - lastRunTimeUsec) * 10000;
				tv->tv_sec--;
			}
			else
			{
				tv->tv_usec = lastRunTimeUsec - nowUsec;
			}
		}
	}
	fclose(fp);
	lastRunTimeSec = nowSec;
	lastRunTimeUsec = nowUsec;
}


void start_PppoeEmulatorBridge(void* unused)
{
	struct timeval tv;
	int ret;
	int index = 0;
	char	 node_name[MAX_ARG_NUM][MAX_NODE_NAME];
	mxml_node_t *top;
	char acTmp[16] = {0};
	char acTrafficStatus[32] = {0};
	
	top =  *gTreePtr;	
	memset(node_name, 0, sizeof(node_name));
	strcpy(node_name[0], "XPON");
	strcpy(node_name[1], "Common");
	
	PppoeEmulatorBridgeTick = 0;
	isFirstLinkup = 0;

	while (1)
	{
		getSelectTime(&tv);
		PppoeEmulatorBridgeTick++;
		
		getAttrValue(top, node_name, "phyStatus", acTmp);
		getAttrValue(top, node_name, "trafficStatus", acTrafficStatus);

		if ((0 == strcmp(acTrafficStatus,"up")) && (0 == strcmp(acTmp, "gpon_phy_up") || 0 == strcmp(acTmp, "epon_phy_up")))
		{	
			if(0 == isFirstLinkup)
			{		
				isFirstLinkup = 1;             // link status from down to up
			}
			else
			{
				isFirstLinkup = 2;             // not the first time linkup
			}
			for (index = 0; index < MAX_WAN_IF_INDEX; index++) 
			{
				PppBridgeEmulator(index);
			}
		}
		else
		{
			isFirstLinkup = 0;                 // set to 0 while linkdown
		}
		getSelectTime(&tv);
		ret = select(0, NULL, NULL, NULL, &tv);
		if (0 != ret)
		{
			return;
		}
	}
	
}


/*______________________________________________________________________________
**	pppoe_Emulator_Bridge_execute
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int pppoe_Emulator_Bridge_execute(mxml_node_t *top, char name[][MAX_NODE_NAME])
{	
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char tmpStr[8] = {0};
	char Enable = 0;
	pthread_attr_t thread_attr;
	
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], PPPOE_EMULATOR_BRIDGE_NODE_NAME);
	strcpy(nodeName[1], PPPOE_EMULATOR_ENTRY);
	getAttrValue(top,nodeName,"Enable", tmpStr);
	Enable = atoi(tmpStr);
	
	if (PppoeEmulatorBridgeThread_id > 0)
	{
		pppoe_Emulator_Bridge_stop(top, PPPOE_EMULATOR_STOP_NOWAIT_FLAG);
		pthread_cancel(PppoeEmulatorBridgeThread_id);
		PppoeEmulatorBridgeThread_id = 0;
	}

	if (1 == Enable)
	{
		pthread_attr_init(&thread_attr);
		pthread_attr_setstacksize(&thread_attr, 16384);
		pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED); 

		if( pthread_create(&PppoeEmulatorBridgeThread_id, &thread_attr, (void *)start_PppoeEmulatorBridge, NULL) != 0 )
		{
			tcdbg_printf("pthread_create error!!\n");
			return -1;
		}
	}
	return 0;
}

int
pppoe_Emulator_Bridge_boot(mxml_node_t *top){
	char Info_node[MAX_ARG_NUM][MAX_NODE_NAME];
	char wan_node[MAX_ARG_NUM][MAX_NODE_NAME];
	int pvc_index,entry_index;
	char acWanMode[8] = {0};
	char acActive[8] = {0};
	memset(Info_node,0,sizeof(Info_node));
	strcpy(Info_node[0], PPPOE_EMULATOR_BRIDGE_INFO_NODE_NAME);
	strcpy(Info_node[1], PPPOE_EMULATOR_ENTRY);
	setAttrValue(top, Info_node, "NASName", "emulatorB"); 
	setAttrValue(top, Info_node, "NAS_IF", "pon"); 
	setAttrValue(top, Info_node, "PPPUnit", "198"); 
	setAttrValue(top, Info_node, "RetryTimes", "3"); 

	memset(wan_node,0,sizeof(wan_node));
	strcpy(wan_node[0], "Wan");
	for(pvc_index = 0; pvc_index < PVC_NUM; pvc_index++)
	{
		sprintf(wan_node[1], "PVC%d", pvc_index);
		for(entry_index = 0; entry_index < PVC_ENTRY_NUM; entry_index++)
		{
			sprintf(wan_node[2], "Entry%d", entry_index);

			if(TCAPI_PROCESS_OK == getAttrValue(top, wan_node, "WanMode", acWanMode))
			{
				if(TCAPI_PROCESS_OK == getAttrValue(top, wan_node, "Active", acActive))
				{
					if((0 == strcmp(acWanMode, "Bridge")) && (0 == strcmp(acActive,"Yes")))
					{
						setAttrValue(top, wan_node, "LastEmulatorTick", "0");
					}
				}
			}
		}
	}

//	pppoe_Emulator_Bridge_execute(top, NULL);
	
	return 0;
}


int
pppoeEmulatorBridge_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name=PPPOE_EMULATOR_BRIDGE_NODE_NAME,
			.type=SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=pppoe_Emulator_Bridge_execute,
			.cfg_boot=pppoe_Emulator_Bridge_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}

int pppoeEmulatorBridgeInfo_init(void){
	//tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name=PPPOE_EMULATOR_BRIDGE_INFO_NODE_NAME,
			.type=WEB_TYPE|SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=NULL,
			.cfg_boot=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}
#endif

#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
char *
ctcom_ippingdiagnotic_merge_table[MAX_NODE_NAME]=
{
	"ippdiag_id","",
};

char *
ctcom_ippingdiagnotic_adv_table[MAX_NODE_NAME]=
{
	"Common","",
};

int
ctcom_ippingdiagnotic_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="CtDiagnostic",
			.type=ADV_FLAG|ENTRY_TYPE|CTCOM_MAX_IPPINGDIAGNOSTIC_NUM,
			.adv_table=ctcom_ippingdiagnotic_adv_table,
			.merge_table=ctcom_ippingdiagnotic_merge_table,
			.cfg_read=NULL,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=ctcom_ippingdiagnotic_execute,
			.cfg_boot=ctcom_ippingdiagnotic_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}
/*______________________________________________________________________________
**	ctcom_ippingdiagnotic_execute
**
**	descriptions:
**		do maintenance of remote link function
**		
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  CtDiagnostic cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
ctcom_ippingdiagnotic_execute(mxml_node_t *top, char name[][MAX_NODE_NAME])
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char nodeNameTemp[MAX_ARG_NUM][MAX_NODE_NAME];
	char tmp[32] = {0}, isp[4] = {0},active[8] = {0},pingflag[4] = {0};
	char *temp = NULL;
	int interfaceindex = 0;
	char StopFlag[10] = {0};
	char DiagState[16] = {0};
	char InterfaceStr[16] = {0};
	char DevStr[32] = "None",GatewayStr[32] = "None",HostStr[32] = {0};
	char cmdbuf[200]  = {0};
	char errMessage[32] = {0};
	int blocksize = 0,timeout = 0,interval = 0,pingnum = 0,pingnumleave = 0, dscp = 0;
	struct hostent *retval;
	struct in_addr addr;
	tcapi_msg_t msg;
	int pingid = -1;
	FILE *fp = NULL;
	char wanNode[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char def_route_index_v4[10] = {0};
	int i_def_route_index_v4 = -1;
	struct sockaddr_in cvtaddr = {0};
	struct addrinfo hints = {0};
	struct addrinfo *res = NULL;
	char dns_wtime_buf[12] = {0};
	int dns_wtime = 5, dns_tmp_time = -1;

	//first,judge if total switch is on or off
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],CTCOM_IPPINGDIAGNOSTIC_NODE_NAME);
	strcpy(nodeName[1],CTCOM_PING_COMMON);
	if(getAttrValue(top, nodeName, "Active", active) == TCAPI_PROCESS_OK)
	{
		if(0 == strcmp(active,"No"))
		{
			//if switch == "No" ,stop all related ping info(include ping application and route info) if exist
			//tcdbg_printf("\r\nactive is set to No,call function stopAllPingOperation");
			stopAllPingOperation();
			return 0;
		}
	}
	else
	{
		//tcdbg_printf("\r\nnot fetch the active attribute or active value is no,so do nothing,return!");
		return 0;
	}

	/*get current ipping index*/
	if(get_entry_number(name[1],SUB_NODE_NAME,&pingid) != SUCCESS)
	{
		memset(nodeName,0,sizeof(nodeName));
		strcpy(nodeName[0],WEBCURSET);
		strcpy(nodeName[1],SUB_NODE_NAME);
		if(getAttrValue(top, nodeName, "ippdiag_id", tmp) == TCAPI_PROCESS_OK)
		{
			pingid = atoi(tmp);
		}
		else
		{
			tcdbg_printf("\r\nctcom_ippingdiagnotic_execute:not fetch index!");
			return FAIL;
		}
	}

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],CTCOM_IPPINGDIAGNOSTIC_NODE_NAME);
	sprintf(nodeName[1],"%s%d",SUB_NODE_NAME,pingid);

	if(getAttrValue(top, nodeName, "DiagnosticsState", DiagState) != TCAPI_PROCESS_OK)
	{
			tcdbg_printf("\r\nctcom_ippingdiagnotic_execute:fetch DiagnosticsState error!");
			strcpy(errMessage,"None");
			goto errHandle;
	}

	if(getAttrValue(top, nodeName, "StopPingFlag", pingflag) == TCAPI_PROCESS_OK)
	{
		if(!strcmp(pingflag,CTCOM_STOPPING_FROM_CWMP) || !strcmp(pingflag,CTCOM_STOPPING_FROM_PING))
		{
			//tcdbg_printf("\r\nctcom_ippingdiagnotic_execute:stop from tr069 or ping application!");
			stopCwmpPing(pingid);
			setAttrValue(top,nodeName,"StopPingFlag","0");
			strcpy(errMessage,DiagState);
			goto errHandle;
		}
	}

	//if(!strcmp(DiagState,"Requested"))
	if(strlen(DiagState) != 0 && strcmp(DiagState,"None") && strcmp(DiagState,"Complete"))
	{
		//if need to stop,when do stop ,just return
		if(getAttrValue(top, nodeName, "Stop", StopFlag) == TCAPI_PROCESS_OK)
		{
			if(strcmp(StopFlag,"1") == 0)
			{
				stopCwmpPing(pingid);
				//setAttrValue(top,nodeName,"Stop","0");
				strcpy(errMessage,"None");
				goto errHandle;
			}	
		}

		//judge if the ping is exist,if exist,just return
		/*why add this judgement??  to settle this case: active is yes,and requested one entry and recomit active to yes,using "ps",it will show two ping aplications*/
		memset(cmdbuf,0,sizeof(cmdbuf));
		sprintf(cmdbuf,CTCOM_PING_PID_PATH,pingid);
		fp = fopen(cmdbuf, "r");
		if(fp)
		{
			fclose(fp);
			fp = NULL;
			return 0;
		}
		
		//NumberOfRepetitions
		if(getAttrValue(top, nodeName, "NumberOfRepetitions", tmp) != TCAPI_PROCESS_OK)
		{
			tcdbg_printf("\r\nctcom_ippingdiagnotic_execute:fetch NumberOfRepetitions error!");
		}
		else
		{
			pingnum = atoi(tmp);
			if(getAttrValue(top, nodeName, "PingNum", tmp) != TCAPI_PROCESS_OK)
			{
				tcdbg_printf("\r\nctcom_ippingdiagnotic_execute:fetch PingNum error!");
			}
			else
			{
				pingnumleave = atoi(tmp);
			}
		}
		if(pingnum != 0 && pingnum <= pingnumleave)
		{
			tcdbg_printf("\r\nthe ping num is not correct,just return!");
			return 0;
		}
		
		//operation interface
		if(getAttrValue(top, nodeName, "Interface", InterfaceStr) != TCAPI_PROCESS_OK)
		{
			tcdbg_printf("\r\nctcom_ippingdiagnotic_execute:fetch Interface error!");
			strcpy(errMessage,"Error_Internal");
			goto errHandle;
		}
 
		if((temp = strstr(InterfaceStr,"smux")) != NULL)//ping is wan operation
		{
			temp += strlen("smux");
			interfaceindex = atoi(temp);
			if(get_waninfo_by_index(interfaceindex, "Active", active) == SUCCESS &&
				get_waninfo_by_index(interfaceindex, "ISP", isp) == SUCCESS &&
				get_waninfo_by_index(interfaceindex, WAN_IFNAME, DevStr) == SUCCESS)
			{
				//pvc is active,and not bridge mode 
				if(!strcmp(active, "Yes") && strcmp(isp, BRIDGE_MODE))
				{
					memset(nodeNameTemp,0,sizeof(nodeNameTemp));
					strcpy(nodeNameTemp[0],WAN_IF_NODE);
					sprintf(nodeNameTemp[1], "Entry%d", interfaceindex);	
					if(getAttrValue(top, nodeNameTemp, "GateWay", GatewayStr) != TCAPI_PROCESS_OK)
					{
						tcdbg_printf("\r\nctcom_ippingdiagnotic_execute:fetch GateWay error!");
						strcpy(errMessage,"Error_Internal");
						goto errHandle;
					}
					if(strlen(DevStr) == 0 || strlen(GatewayStr) == 0)
					{
						strcpy(DevStr,"None");
						strcpy(GatewayStr,"None");
					}
				}
			}
		}
		else if(strlen(InterfaceStr) == 0/* ||strstr(InterfaceStr,LAN_IF)*/)//no need to add default route
		{
			// should get the info from default route internet wan.
			memset(def_route_index_v4, 0, sizeof(def_route_index_v4));
			memset(wanNode, 0, sizeof(wanNode));
			strcpy(wanNode[0], "WanInfo");
			strcpy(wanNode[1], "Common");
			// get default route for ipv4
			if ( TCAPI_PROCESS_OK == getAttrValue(top, wanNode, "DefRouteIndexv4", def_route_index_v4)
				&& '\0' != def_route_index_v4[0] && 0 != strcmp(def_route_index_v4, "N/A") )
				i_def_route_index_v4 = atoi(def_route_index_v4);
			if ( i_def_route_index_v4 >= 0 )
			{
				interfaceindex = i_def_route_index_v4;
				if(get_waninfo_by_index(interfaceindex, "Active", active) == SUCCESS &&
					get_waninfo_by_index(interfaceindex, "ISP", isp) == SUCCESS &&
					get_waninfo_by_index(interfaceindex, WAN_IFNAME, DevStr) == SUCCESS)
				{
					//pvc is active,and not bridge mode 
					if(!strcmp(active, "Yes") && strcmp(isp, BRIDGE_MODE))
					{
						memset(nodeNameTemp,0,sizeof(nodeNameTemp));
						strcpy(nodeNameTemp[0],WAN_IF_NODE);
						sprintf(nodeNameTemp[1], "Entry%d", interfaceindex);	
						if(getAttrValue(top, nodeNameTemp, "GateWay", GatewayStr) != TCAPI_PROCESS_OK)
						{
							tcdbg_printf("\r\nctcom_ippingdiagnotic_execute:fetch GateWay error!");
							strcpy(errMessage,"Error_Internal");
							goto errHandle;
						}
						if(strlen(DevStr) == 0 || strlen(GatewayStr) == 0)
		{
							strcpy(DevStr,"None");
							strcpy(GatewayStr,"None");
						}
					}
				}
			}
		}

		//operate host
		if(getAttrValue(top, nodeName, "Host", HostStr) != TCAPI_PROCESS_OK)
		{
			tcdbg_printf("\r\nctcom_ippingdiagnotic_execute:fetch Host error!");
			strcpy(errMessage,"Error_CannotResolveHostName");
			goto errHandle;
		}
	//gethostbyname will be excuted by ping app 
		if (strlen(HostStr) == 0/* || (retval = gethostbyname(HostStr)) == NULL*/)
		{
			tcdbg_printf("\r\nctcom_ippingdiagnotic_execute:fetch host error!");
			strcpy(errMessage,"Error_CannotResolveHostName");
			goto errHandle;
		}
		//memcpy(&addr,retval->h_addr,sizeof(struct in_addr));
		if ( NULL == strstr(InterfaceStr,LAN_IF) )
		{
		if(!strcmp(DevStr,"None") || !strcmp(GatewayStr,"None"))
		{
			tcdbg_printf("\r\nctcom_ippingdiagnotic_execute:fetch host error!");
			strcpy(errMessage,"Error_Internal");
			goto errHandle;
		}
		}

		//DateBlockSize
		if(getAttrValue(top, nodeName, "DataBlockSize", tmp) != TCAPI_PROCESS_OK)
		{
			tcdbg_printf("\r\nctcom_ippingdiagnotic_execute:fetch DateBlockSize error!");
		}
		else
		{
			blocksize = atoi(tmp);
		}

		//Timeout
		if(getAttrValue(top, nodeName, "Timeout", tmp) != TCAPI_PROCESS_OK)
		{
			tcdbg_printf("\r\nctcom_ippingdiagnotic_execute:fetch Timeout error!");
		}
		else
		{
			timeout = atoi(tmp);
		}

		//Interval
		if(getAttrValue(top, nodeName, "Interval", tmp) != TCAPI_PROCESS_OK)
		{
			tcdbg_printf("\r\nctcom_ippingdiagnotic_execute:fetch Interval error!");
		}
		else
		{
			interval = atoi(tmp);
		}

		//DSCP
		if(getAttrValue(top, nodeName, "DSCP", tmp) != TCAPI_PROCESS_OK)
		{
			tcdbg_printf("\r\nctcom_ippingdiagnotic_execute:fetch DSCP error!");
		}
		else
		{
			dscp = atoi(tmp);
		}

		if( 0 != strcmp(GatewayStr,"None") && 0 != strcmp(DevStr,"None") )
		{
			if( 0 == inet_aton(HostStr, &cvtaddr.sin_addr) ) // failed
			{
				if( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, "DNS_WTime", tmp)
					&& '\0' != tmp[0] )
				{
					dns_tmp_time = atoi(tmp);
					if ( dns_tmp_time > 0 )
						dns_wtime = dns_tmp_time;

					setAttrValue(top, nodeName, "DNS_WTime", "");
				}

				memset(cmdbuf, 0, sizeof(cmdbuf));
				sprintf(cmdbuf, "/bin/echo \"%s %s\" > %s", HostStr, DevStr, IPPINGDIAGNOSTIC_DNSURL);
				system(cmdbuf);

				memset(&hints, 0 , sizeof(hints));
				hints.ai_family = AF_UNSPEC;
				hints.ai_socktype = SOCK_DGRAM; //DNS is UDP packet
				hints.ai_protocol = IPPROTO_UDP;

				if ( tc_getaddrinfo(HostStr, NULL, &hints, &res, dns_wtime)
					|| AF_INET != res->ai_family )
				{
					unlink(IPPINGDIAGNOSTIC_DNSURL);
					tcdbg_printf("\r\nctcom_ippingdiagnotic_execute:resolve host error!");
					strcpy(errMessage,"Error_CannotResolveHostName");
					goto dnsErrHandle;
				}
				unlink(IPPINGDIAGNOSTIC_DNSURL);

				strcpy(HostStr, inet_ntoa(((struct sockaddr_in *) res->ai_addr)->sin_addr));
			}
		}

		//do ping operation
		memset(cmdbuf,0,sizeof(cmdbuf));
		sprintf(cmdbuf,"ping -q -f 1 -c %d -s %d -W %d -i %d -Q %d -p %d -d %s -g %s %s &",pingnum-pingnumleave,blocksize,timeout,interval,dscp,pingid,DevStr,GatewayStr,HostStr);
		tcdbg_printf("\r\nthe buf is %s",cmdbuf);
		system(cmdbuf);
		
		//if DiagnosticsState is not "Requested",but ping is ping ok,set and save the value and 
		if(strcmp(DiagState,"Requested"))
		{
			setAttrValue(top,nodeName,"DiagnosticsState","Requested");
			tcapi_save_req(top,&msg);
		}
	}
	
	return 0;	

errHandle:
	setAttrValue(top,nodeName,"DiagnosticsState",errMessage);
	tcapi_save_req(top,&msg);
	return -1;
dnsErrHandle:
	setAttrValue(top,nodeName,"DiagnosticsState",errMessage);
	tcapi_save_req(top,&msg);
	return 0;

}
/*______________________________________________________________________________
**	ctcom_ippingdiagnotic_boot
**
**	descriptions:
**		do maintenance of remote link function
**		
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  CtDiagnostic cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
ctcom_ippingdiagnotic_boot(mxml_node_t *top)
{
	int i,flag = 0;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char diagnostic[20] = {0},pingtotal[15] = {0},pingnum[15] = {0},Interfacebuf[32] = {0};
	char active[10] = {0};
	
	strcpy(nodeName[0],CTCOM_IPPINGDIAGNOSTIC_NODE_NAME);
	strcpy(nodeName[1],CTCOM_PING_COMMON);
	if(getAttrValue(top, nodeName, "Active", active) == TCAPI_PROCESS_OK)
	{
		if(0 == strcmp(active,"Yes"))
		{
			flag = 1;
		}
	}
	if(!flag)
	{
		//tcdbg_printf("\r\nnot fetch the active attribute or active value is no,so do nothing,return!");
		return 0;
	}
	
	
	for(i = 0; i < CTCOM_MAX_IPPINGDIAGNOSTIC_NUM; i++)
	{
		memset(nodeName, 0, sizeof(nodeName));
	  	strcpy(nodeName[0],CTCOM_IPPINGDIAGNOSTIC_NODE_NAME);
	  	sprintf(nodeName[1], "Entry%d",i);
		if(getAttrValue(top, nodeName, "DiagnosticsState", diagnostic) == TCAPI_PROCESS_OK &&
			getAttrValue(top, nodeName, "NumberOfRepetitions", pingtotal) == TCAPI_PROCESS_OK	&&
			getAttrValue(top, nodeName, "Interface", Interfacebuf) == TCAPI_PROCESS_OK &&
			getAttrValue(top, nodeName, "PingNum", pingnum) == TCAPI_PROCESS_OK)
		{
			if(strlen(diagnostic) != 0 && strcmp(diagnostic, "None") && (0 == atoi(pingtotal) ||(pingtotal - pingnum) > 0))//only "None" mean not need to do ping operation
			{
				//only handle ping lan pc or use default route
				if(strlen(Interfacebuf) == 0 ||strstr(Interfacebuf,LAN_IF))
				{
					ctcom_ippingdiagnotic_execute(top,nodeName);
				}
			}
		}
	}

	return 0;
}

void stopCwmpPing(int entryindex)
{
	FILE *fp = NULL;
	char cmdbuf[128] = {0};
	char ping_pid_tmp[50] = {0};
	int ping_pid = 0;
	
	tcdbg_printf("\r\nstopCwmpPing");
	
	//operate ping application
	sprintf(cmdbuf,CTCOM_PING_PID_PATH,entryindex);
	fp = fopen(cmdbuf, "r");
	if(fp)
	{
		//kill the ping process
		fgets(ping_pid_tmp, sizeof(ping_pid_tmp), fp);
		ping_pid = atoi(ping_pid_tmp);
		if(ping_pid != 0)
		{ 
			/*if pid != 0, that's mean old ping diagnostic process already exist*/
			memset(cmdbuf,0,sizeof(cmdbuf));
			sprintf(cmdbuf,"kill -9 %d", ping_pid);  /*remove old ping diagnostic process*/
			//tcdbg_printf("ct com Ping Diagnostic process down, pid is [%d]\n", ping_pid);
		   	system(cmdbuf);
		}
		
		//delete the file/tmp/cwmp/ct_ping_xxx.pid
		fclose(fp);
		sprintf(cmdbuf,CTCOM_PING_PID_PATH,entryindex);
		unlink(cmdbuf);
	}

	//operation route information
	memset(cmdbuf,0,sizeof(cmdbuf));
	sprintf(cmdbuf,"%s%d",CTCOM_PING_ROUTE_PATH,entryindex);
	fp = fopen(cmdbuf, "r");
	if(fp)
	{
		//delete the route which add at the beginning of the ping 
		memset(cmdbuf,0,sizeof(cmdbuf));
		fgets(cmdbuf, sizeof(cmdbuf), fp);
		system(cmdbuf);

		//delete the file /tmp/cwmp/ct_ping_route
		fclose(fp);
		sprintf(cmdbuf,"%s%d",CTCOM_PING_ROUTE_PATH,entryindex);
		unlink(cmdbuf);
	}

	return;
}
void stopAllPingOperation()
{
	int i;
	
	for(i = 0; i < CTCOM_MAX_IPPINGDIAGNOSTIC_NUM; i++)
	{
		stopCwmpPing(i);
	}

	return;
}
#endif

#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
#define CONFIGAUTH_INSTANCE_NUM		4

int
phyPort_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="PhyPort",
			.type=SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=NULL,
			.cfg_boot=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}

int
configAuth_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="ConfigAuth",
			.type=ENTRY_TYPE|CONFIGAUTH_INSTANCE_NUM,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=NULL,
			.cfg_boot=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}
#endif

#if defined(TCSUPPORT_CT_ALARMMONITOR)

#define DEVALARM_INSTANCE_NUM	10

char *alarm_adv_table[MAX_NODE_NAME]=
{
	"Common","",
};
char *alarm_merge_table[MAX_NODE_NAME]=
{
	"alarm_id","",
};

//"DeviceAlarm",	ADV_FLAG|ENTRY_TYPE,	NULL, NULL,NULL, devAlarm_write, devAlarm_verify, NULL},
/*______________________________________________________________________________
**	devAlarm_init
**
**	descriptions:
**		
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
devAlarm_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="DeviceAlarm",
			.type=ADV_FLAG|ENTRY_TYPE|DEVALARM_INSTANCE_NUM,
			.adv_table=alarm_adv_table,
			.merge_table=alarm_merge_table,
			.cfg_read=NULL,
			.cfg_write=devAlarm_write,
			.cfg_verify=devAlarm_verify,
			.cfg_execute=NULL,
			.cfg_boot=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}

/*______________________________________________________________________________
**	devAlarm_write
**
**	descriptions:
**		
**		
**
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  DeviceAlarm cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
devAlarm_write(mxml_node_t *top, mxml_node_t *parant){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char tmp[128] = {0};
	int i, entryNum = 0, maxEntryNum = 0;

	memset(nodeName,0, sizeof(nodeName));
	strcpy(nodeName[0], ALARM_NODENAME);
	strcpy(nodeName[1], COMMON_NODENAME);

	/* Statistics the instance num */
	if(getAttrValue(top, nodeName, "MaxInstanceNum", tmp) == TCAPI_PROCESS_OK){
		maxEntryNum = atoi(tmp);
		
		for(i=0; i<maxEntryNum; i++) {
			memset(nodeName[1], 0, MAX_NODE_NAME);
			sprintf(nodeName[1], ENTRY_NODENAME, i);

			memset(tmp, 0, sizeof(tmp));
			if(getAttrValue(top, nodeName, "ParaList", tmp) == TCAPI_PROCESS_OK) {
				if(strlen(tmp) != 0)
					entryNum++;
			}
		}

		memset(nodeName[1], 0, MAX_NODE_NAME);
		strcpy(nodeName[1],COMMON_NODENAME);

		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%d", entryNum);
		setAttrValue(top, nodeName, "InstanceNum", tmp);
	}
	return 0;
}/* end devAlarm_write */

/*______________________________________________________________________________
**	devAlarm_verify
**
**	descriptions:
**		
**
**	parameters:
**		node: 		Specify the root of mxml_node_t structure.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
devAlarm_verify(mxml_node_t *node){
	return 0;
}/* end devAlarm_verify */


#define DEVMONITOR_INSTANCE_NUM	10

char *monitor_adv_table[MAX_NODE_NAME]=
{
	"Common","",
};
char *monitor_merge_table[MAX_NODE_NAME]=
{
	"monitor_id","",
};

//"DeviceMonitor",	ADV_FLAG|ENTRY_TYPE,	NULL, NULL,NULL, devAlarm_write, devAlarm_verify, NULL},
/*______________________________________________________________________________
**	devMonitor_init
**
**	descriptions:
**		the init function of DeviceMonitor.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
devMonitor_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="DeviceMonitor",
			.type=ADV_FLAG|ENTRY_TYPE|DEVMONITOR_INSTANCE_NUM,
			.adv_table=monitor_adv_table,
			.merge_table=monitor_merge_table,
			.cfg_read=NULL,
			.cfg_write=devMonitor_write,
			.cfg_verify=devMonitor_verify,
			.cfg_execute=NULL,
			.cfg_boot=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}

/*______________________________________________________________________________
**	devMonitor_write
**
**	descriptions:
**		
**		
**
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  DeviceMonitor cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
devMonitor_write(mxml_node_t *top, mxml_node_t *parant){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char tmp[256] = {0};
	int i, entryNum = 0, maxEntryNum = 0;

	memset(nodeName,0, sizeof(nodeName));
	strcpy(nodeName[0], MONITOR_NODENAME);
	strcpy(nodeName[1], COMMON_NODENAME);

	/* Statistics the instance num */
	if(getAttrValue(top, nodeName, "MaxInstanceNum", tmp) == TCAPI_PROCESS_OK) {
		maxEntryNum = atoi(tmp);
		
		for(i=0; i<maxEntryNum; i++) {
			memset(nodeName[1], 0, MAX_NODE_NAME);
			sprintf(nodeName[1], ENTRY_NODENAME, i);

			memset(tmp, 0, sizeof(tmp));
			if(getAttrValue(top, nodeName, "ParaList", tmp) == TCAPI_PROCESS_OK) {
				if(strlen(tmp) != 0)
					entryNum++;
			}
		}

		memset(nodeName[1], 0, MAX_NODE_NAME);
		strcpy(nodeName[1],COMMON_NODENAME);

		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%d", entryNum);
		setAttrValue(top, nodeName, "InstanceNum", tmp);
	}
	return 0;
}/* end devMonitor_write */

/*______________________________________________________________________________
**	devMonitor_verify
**
**	descriptions:
**		
**
**	parameters:
**		node: 		Specify the root of mxml_node_t structure.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
devMonitor_verify(mxml_node_t *node){
	return 0;
}/* end devMonitor_verify */

#endif
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
char *
user_merge_table[MAX_NODE_NAME]=
{
	"user_id","",
};

int
user_init(void){
	int ret=0;
	cfg_node_t node={
			.name="User",
			.type=ENTRY_TYPE|MAX_USER_NUM,
			.adv_table=NULL,
			.merge_table=user_merge_table,
			.cfg_read=NULL,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=user_execute,
			.cfg_boot=user_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}

/*______________________________________________________________________________
**	user_boot
**
**	descriptions:
**		the boot function of User.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**_______________________________________________________________________________
*/
int
user_boot(mxml_node_t *top){	

	return SUCCESS;
}

/*______________________________________________________________________________
**	user_execute
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
user_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	tcdbg_printf("enter %s\r\n",__func__);
	return SUCCESS;
}

char *
usr_macfilter_merge_table[MAX_NODE_NAME]=
{
	"macfilter_user","",
};

int
usr_macfilter_init(void){
	int ret=0;
	cfg_node_t node={
			.name="MACFilter",
			.type=ENTRY_TYPE|MAX_MAC_FILTER_NUM,
			.adv_table=NULL,
			.merge_table=usr_macfilter_merge_table,
			.cfg_read=NULL,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=usr_macfilter_execute,
			.cfg_boot=usr_macfilter_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}

/*______________________________________________________________________________
**	user_macfilter_boot
**
**	descriptions:
**		the boot function of MACFilter.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**_______________________________________________________________________________
*/
int
usr_macfilter_boot(mxml_node_t *top){	

	return SUCCESS;
}

/*______________________________________________________________________________
**	usr_macfilter_execute
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
usr_macfilter_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	tcdbg_printf("enter %s\r\n",__func__);
	return SUCCESS;
}

char *
usr_ipfilter_merge_table[MAX_NODE_NAME]=
{
	"ipfilter_user","",
};

int
usr_ipfilter_init(void){
	int ret=0;
	cfg_node_t node={
			.name="IPFilter",
			.type=ENTRY_TYPE|MAX_IP_FILTER_NUM,
			.adv_table=NULL,
			.merge_table=usr_ipfilter_merge_table,
			.cfg_read=NULL,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=usr_ipfilter_execute,
			.cfg_boot=usr_ipfilter_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}

/*______________________________________________________________________________
**	usr_ipfilter_boot
**
**	descriptions:
**		the boot function of IPFilter.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**_______________________________________________________________________________
*/
int
usr_ipfilter_boot(mxml_node_t *top){	

	return SUCCESS;
}

/*______________________________________________________________________________
**	usr_ipfilter_execute
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
usr_ipfilter_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	tcdbg_printf("enter %s\r\n",__func__);
	return SUCCESS;
}

char *
usr_whiteurl_merge_table[MAX_NODE_NAME]=
{
	"whiteurl_user","",
};

int
usr_whiteurl_init(void){
	int ret=0;
	cfg_node_t node={
			.name="WhiteURL",
			.type=ENTRY_TYPE|MAX_WHITE_URL_NUM,
			.adv_table=NULL,
			.merge_table=usr_whiteurl_merge_table,
			.cfg_read=NULL,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=usr_whiteurl_execute,
			.cfg_boot=usr_whiteurl_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}

/*______________________________________________________________________________
**	usr_whiteurl_boot
**
**	descriptions:
**		the boot function of WhiteURL.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**_______________________________________________________________________________
*/
int
usr_whiteurl_boot(mxml_node_t *top){	

	return SUCCESS;
}

/*______________________________________________________________________________
**	usr_whiteurl_execute
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
usr_whiteurl_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	tcdbg_printf("enter %s\r\n",__func__);
	return SUCCESS;
}

char *
usr_blackurl_merge_table[MAX_NODE_NAME]=
{
	"blackurl_user","",
};

int
usr_blackurl_init(void){
	int ret=0;
	cfg_node_t node={
			.name="BlackURL",
			.type=ENTRY_TYPE|MAX_BLACK_URL_NUM,
			.adv_table=NULL,
			.merge_table=usr_blackurl_merge_table,
			.cfg_read=NULL,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=usr_blackurl_execute,
			.cfg_boot=usr_blackurl_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}

/*______________________________________________________________________________
**	usr_blackurl_boot
**
**	descriptions:
**		the boot function of BlackURL.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**_______________________________________________________________________________
*/
int
usr_blackurl_boot(mxml_node_t *top){	

	return SUCCESS;
}

/*______________________________________________________________________________
**	usr_blackurl_execute
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
usr_blackurl_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	tcdbg_printf("enter %s\r\n",__func__);
	return SUCCESS;
}
#endif

#if defined(TCSUPPORT_CT_MONITORCOLLECTOR)
char *monitorCollector_adv_table[MAX_NODE_NAME]=
{
	"Common","",
};


/*______________________________________________________________________________
**	MonitorCollector_init
**
**	descriptions:
**		the init function of DeviceMonitor.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
MonitorCollector_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="MonitorCollector",
			.type=ADV_FLAG|ENTRY_TYPE|MONITOR_COLLECTOR_MAXNUM,
			.adv_table=monitorCollector_adv_table,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=MonitorCollector_execute,
			.cfg_boot=MonitorCollector_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}


int getInformItem(char* tmp, int instance, char* paraList){
	char tempvalue[8] = {0};
	char* ptr = NULL;
	int pvcIndex = 0;
	int entryIndex = 0;
	int ip_ppp = WAN_TYPE_NOCARE;
	
	//(%d,%d,%d,%d,%d) = (isDelete,ip_ppp,paraListItem,pvcIndex,entryIndex)		
	memset(tmp, 0, sizeof(tmp));
	if((ptr = strstr(paraList,"WANConnectionDevice")) != NULL){
		memset(tempvalue, 0, sizeof(tempvalue));
		tempvalue[0] = *(ptr + strlen("WANConnectionDevice") + 1);
		tempvalue[1] = '\0';
		pvcIndex = atoi(tempvalue);
			
		memset(tempvalue, 0, sizeof(tempvalue));
		if((ptr = strstr(paraList,"WANPPPConnection"))!= NULL){
			tempvalue[0] = *(ptr + strlen("WANPPPConnection") + 1);		
			tempvalue[1] = '\0';
			entryIndex = atoi(tempvalue);	
			ip_ppp = PPP_WAN_TYPE;
		}else if((ptr = strstr(paraList,"WANIPConnection"))!= NULL){
			tempvalue[0] = *(ptr + strlen("WANIPConnection") + 1);	
			tempvalue[1] = '\0';
			entryIndex = atoi(tempvalue);	
			ip_ppp = IP_WAN_TYPE;
		}else{
			entryIndex = -1;
		}
		
		sprintf(tmp,"(0,%d,%d,%d,%d)",instance,ip_ppp,pvcIndex,entryIndex);		
		tcdbg_printf("getInformItem:tmp = %s\n",tmp);
		return 0;
	}

	return -1;
}
/*______________________________________________________________________________
**	MonitorCollector_boot
**
**	descriptions:
**		the boot function of MonitorCollector.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**_______________________________________________________________________________
*/
int
MonitorCollector_boot(mxml_node_t *top){	
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char tmp[128] = {0}, tempValue[256] = {0};		
	char tmpValue[32] = {0};	
	char attr[32] = {0};
	int i = 0;
	int j = 0;	
	int entryNum = 0;
	int len = 0;
	

	memset(nodeName,0, sizeof(nodeName));
	strcpy(nodeName[0], MONITOR_COLLECTOR_NODENAME);
	strcpy(nodeName[1], COMMON_NODENAME);
	setAttrValue(top, nodeName, "ntpSync", "0") ;
		
	for(i=0; i<MONITOR_COLLECTOR_MAXNUM; i++) {
		memset(nodeName[1], 0, MAX_NODE_NAME);
		sprintf(nodeName[1], ENTRY_NODENAME, i);

		j = 0;
		len = 0;
		while(1){
			memset(tmp, 0, sizeof(tmp));
			sprintf(attr,"ParaList%d",j);
			if(getAttrValue(top, nodeName, attr, tmp) == TCAPI_PROCESS_OK) {
				j++;
				len += strlen(tmp);
			}else{
				break;
			}


		}

		if(j != 0){
			entryNum++;

			if(getAttrValue(top, nodeName, "ParaListNum", tmp) != TCAPI_PROCESS_OK) {
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp,"%d",j);
				setAttrValue(top, nodeName, "ParaListNum", tmp);

				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp,"%d",len+j);
				setAttrValue(top, nodeName, "ParaListLen", tmp);

			}
		}
	}

	memset(nodeName[1], 0, MAX_NODE_NAME);
	strcpy(nodeName[1],COMMON_NODENAME);

	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp, "%d", entryNum);
	setAttrValue(top, nodeName, "InstanceNum", tmp);

	return SUCCESS;
}

/*______________________________________________________________________________
**	MonitorCollector_execute
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
MonitorCollector_execute(mxml_node_t *top, char name[][MAX_NODE_NAME])
{
    cwmp_msg_t message;
    char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
    char enableValue[8]= {0};
#define QOE_RESTART 22

    memset(nodeName,0, sizeof(nodeName));
    strcpy(nodeName[0], MONITOR_COLLECTOR_NODENAME);
    strcpy(nodeName[1], COMMON_NODENAME);
    if( TCAPI_PROCESS_OK == getAttrValue(top,nodeName,"Enable", enableValue))
    {   
        tcdbg_printf("\n monitor collector Enable %s!\n", enableValue);
        memset(&message,0,sizeof(cwmp_msg_t));

        strcpy(message.text.reserved ,enableValue);
        message.cwmptype = MONITOR_COLLECTOR_SWITCH;
        sendmegq(1, &message, 0);

    }
    else
    {
        tcdbg_printf("\n fail to get monitor collector Enable!\n");
    }
    return SUCCESS;
}


#endif

#if defined(TCSUPPORT_CT_IPOE_EMULATOR)
int ipoeEmulator_init(void)
{
	int ret=0;
	cfg_node_t node={
			.name=IPOE_EMULATOR_NODE_NAME,
			.type=SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=ipoeEmulator_write,
			.cfg_verify=NULL,
			.cfg_execute=ipoeEmulator_execute,
			.cfg_boot=ipoeEmulator_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}

int ipoeKillviafile(char *file)
{
	FILE *fp = NULL;
	char pid_tmp[50] = {0};
	int pid = 0;

	if ( !file )
		return -1;

	fp = fopen(file, "r");
	if ( fp )
	{
		fgets(pid_tmp, sizeof(pid_tmp), fp);
		close(fp);
		unlink(file);

		pid = atoi(pid_tmp);
		if( 0 != pid
			&& 0 != kill(pid, SIGTERM) )
		{
			if ( 0 !=  kill(pid, SIGKILL) )
				tcdbg_printf("\n%s kill old ipoe client failed(%s)!\n", __FUNCTION__, file);
		}
	}

	return 0;
}

int reset_ipoeEmulator_statistic(mxml_node_t *top)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], IPOE_EMULATOR_NODE_NAME);
	strcpy(nodeName[1], IPOE_EMULATOR_ENTRY);

	setAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_LADDR, "0.0.0.0");
	setAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_DEFGW, "0.0.0.0");
	setAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_SUCC_COUNT, "0");
	setAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_FAIL_COUNT, "0");
	setAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_REP_AVRTIME, "0");
	setAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_REP_MINTIME, "0");
	setAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_REP_MAXTIME, "0");

	return 0;
}

int get_pingResult(mxml_node_t *top)
{
	FILE *fp = NULL;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0}, tmpbuf[64] = {0};
	int sent = 0, responses = 0, minrtt = 0, avrrtt = 0, maxrtt= 0;

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], IPOE_EMULATOR_NODE_NAME);
	strcpy(nodeName[1], IPOE_EMULATOR_ENTRY);

	fp = fopen(IPOE_EMULATOR_PING_RESULT, "r");
	if(fp != NULL)
	{
		fscanf(fp,"%ld,%ld,%lu,%lu,%lu", &sent, &responses, &minrtt, &avrrtt, &maxrtt);
		fclose(fp);
	}
	unlink(IPOE_EMULATOR_PING_RESULT);

	if ( 0 == sent
		&& 0 == responses )
	{
		setAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_RESULT, IPOE_EMULATOR_RESULT_PINGFAIL);
	}
	else
	{
		sprintf(tmpbuf, "%u", responses);
		setAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_SUCC_COUNT, tmpbuf);
		sprintf(tmpbuf, "%u", sent - responses);
		setAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_FAIL_COUNT, tmpbuf);
		sprintf(tmpbuf, "%u", avrrtt);
		setAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_REP_AVRTIME, tmpbuf);
		sprintf(tmpbuf, "%u", minrtt);
		setAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_REP_MINTIME, tmpbuf);
		sprintf(tmpbuf, "%u", maxrtt);
		setAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_REP_MAXTIME, tmpbuf);

		if ( 0 != responses )
			setAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_RESULT, IPOE_EMULATOR_RESULT_SUCC);
		else
			setAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_RESULT, IPOE_EMULATOR_RESULT_PINGFAIL);
	}

	return 0;
}

int get_dhcpResult(mxml_node_t *top)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0}, tmpbuf[64] = {0};
	char info_string[2][MAX_INFO_LENGTH];
	char path[128] = {0};

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], IPOE_EMULATOR_NODE_NAME);
	strcpy(nodeName[1], IPOE_EMULATOR_ENTRY);

	sprintf(path, "%s%s", IPOE_EMULATOR_PATH, IPOE_EMULATOR_ATTR_LADDR);
	memset(info_string, 0, sizeof(info_string));
	if ( SUCCESS == get_file_string(path, info_string, 1) )
		setAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_LADDR, info_string[0]);

	sprintf(path, "%s%s", IPOE_EMULATOR_PATH, IPOE_EMULATOR_ATTR_DEFGW);
	memset(info_string, 0, sizeof(info_string));
	if ( SUCCESS == get_file_string(path, info_string, 1) )
		setAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_DEFGW, info_string[0]);

	return 0;
}


int ipoeEmulator_stop(mxml_node_t *top, int method)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char nas_name[16] = {0}, cmdbuf[128] = {0}, localaddr[64] = {0};
	int pid = 0;
	char info_string[2][MAX_INFO_LENGTH];
	char path[128] = {0};

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], IPOE_EMULATOR_NODE_NAME);
	strcpy(nodeName[1], IPOE_EMULATOR_ENTRY);

	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_NASNAME, nas_name)
		|| 0 == nas_name[0] )
		strcpy(nas_name, IPOE_EMULATOR_NAS_NAME);

	// kill ping
	ipoeKillviafile(IPOE_EMULATOR_PING_PID_PATH);
	// kill client
	ipoeKillviafile(IPOE_EMULATOR_PID_PATH);
	sleep(1);

	// remove policy route
	sprintf(cmdbuf, "/usr/bin/iptables -t mangle -D OUTPUT -o %s -j MARK --set-mark 0x7e0000/0x7f0000", nas_name);
	system(cmdbuf);		
	sprintf(path, "%s%s", IPOE_EMULATOR_PATH, IPOE_EMULATOR_ATTR_LADDR);
	memset(info_string, 0, sizeof(info_string));
	if ( SUCCESS == get_file_string(path, info_string, 1)
		&& 0 != info_string[0] )
	{
		sprintf(cmdbuf, "/usr/bin/ip rule del from %s/32 table %d", info_string[0], IPOE_EMULATOR_POLICY_TABLEID);
		system(cmdbuf);
	}
	else if( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_LADDR, localaddr)
		&& 0 != localaddr[0]
		&& 0 != strcmp(localaddr, "0.0.0.0") )
	{
		sprintf(cmdbuf, "/usr/bin/ip rule del from %s/32 table %d", localaddr, IPOE_EMULATOR_POLICY_TABLEID);
		system(cmdbuf);
	}
		
	sprintf(cmdbuf, "/usr/bin/ip rule del fwmark 0x7e0000/0x7f0000 table %d", IPOE_EMULATOR_POLICY_TABLEID);
	system(cmdbuf);	
	sprintf(cmdbuf, "/usr/bin/ip route flush table %d", IPOE_EMULATOR_POLICY_TABLEID);
	system(cmdbuf);
	// remove interface
	memset(cmdbuf, 0, sizeof(cmdbuf));
	sprintf(cmdbuf, "/sbin/ifconfig %s down", nas_name);
	system(cmdbuf);
	memset(cmdbuf, 0, sizeof(cmdbuf));
	sprintf(cmdbuf, "/usr/bin/smuxctl rem %s", nas_name);
	system(cmdbuf);

	if ( IPOE_EMULATOR_STOP_OTHER == method )
	{
		setAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_DIGSTATE, IPOE_EMULATOR_ST_NONE);
		setAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_RESULT, IPOE_EMULATOR_RESULT_OTHER);
	}

	return 0;
}


int ipoeEmulator_boot(mxml_node_t *top)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};

	memset(nodeName,0, sizeof(nodeName));
	strcpy(nodeName[0], IPOE_EMULATOR_NODE_NAME);
	strcpy(nodeName[1], IPOE_EMULATOR_ENTRY);

	setAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_DIGSTATE, IPOE_EMULATOR_ST_NONE);
	setAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_WANIFACE, "");
	setAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_USERMAC, "");
	setAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_VENDORID, "");
	setAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_RESULT, IPOE_EMULATOR_RESULT_OTHER);
	setAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_LADDR, "0.0.0.0");
	setAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_DEFGW, "0.0.0.0");
	setAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_PINGDSTIP, "");
	setAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_PINGNUM, "5");
	setAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_TIMEOUT, "2000");
	setAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_SUCC_COUNT, "0");
	setAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_FAIL_COUNT, "0");
	setAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_REP_AVRTIME, "0");
	setAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_REP_MINTIME, "0");
	setAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_REP_MAXTIME, "0");
	setAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_NASNAME, IPOE_EMULATOR_NAS_NAME);
	setAttrValue(top, nodeName, "NAS_IF", "pon");

	reset_ipoeEmulator_statistic(top);

	return 0;
}

int ipoeEmulator_write(mxml_node_t *top, mxml_node_t *parant)
{
	char fpath[64] = {0};
	mxml_node_t *node = NULL;

	/*create ipoe_emu.conf*/
	sprintf(fpath, "%s", IPOE_EMULATOR_CONF);

	node = mxmlFindElement(top, top, IPOE_EMULATOR_NODE_NAME,NULL, NULL, MXML_DESCEND_FIRST);
	if ( NULL == node )
	{
		tcdbg_printf("\n%s can't find node %s\n", __FUNCTION__, IPOE_EMULATOR_NODE_NAME);
		return -1;
	}
	node = mxmlFindElement(node, node, IPOE_EMULATOR_ENTRY, NULL, NULL, MXML_DESCEND);
	if ( NULL == node )
	{
		tcdbg_printf("\n%s can't find sub node %s for node %s\n", __FUNCTION__, IPOE_EMULATOR_ENTRY, IPOE_EMULATOR_NODE_NAME);
		return -1;
	}

	if ( 0 != node_write(node, fpath, QMARKS) )
	{
		tcdbg_printf("\n%s node_write error!\n", __FUNCTION__);
		return -1;
	}

	chmod(IPOE_EMULATOR_START, 777);
	chmod(IPOE_EMULATOR_DHCP_SH, 777);

	return 0;
}

int ipoeEmulator_execute(mxml_node_t *top, char name[][MAX_NODE_NAME])
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char cwmpNodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char diag_state[16] = {0}, vlanmode[16] = {0};
	tcapi_msg_t msg = {0};
#define IPOE_EMULATOR_OK 16
	char info_string[2][MAX_INFO_LENGTH];
	char path[128] = {0}, cmdbuf[128] = {0};
	int rm_folder = 0;

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], IPOE_EMULATOR_NODE_NAME);
	strcpy(nodeName[1], IPOE_EMULATOR_ENTRY);
	memset(cwmpNodeName, 0, sizeof(cwmpNodeName));
	strcpy(cwmpNodeName[0], "Cwmp");
	strcpy(cwmpNodeName[1], "Entry");

	if( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_DIGSTATE, diag_state) )
	{
		if ( 0 == strcmp(IPOE_EMULATOR_ST_START, diag_state) )
		{
			// check old process first and stop it.
			ipoeEmulator_stop(top, IPOE_EMULATOR_STOP_OTHER);
			// remove dir
			memset(cmdbuf, 0, sizeof(cmdbuf));
			sprintf(cmdbuf, "/bin/rm -r %s", IPOE_EMULATOR_PATH);
			system(cmdbuf);
			// start it
			reset_ipoeEmulator_statistic(top);
			setAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_DIGSTATE, IPOE_EMULATOR_ST_RUNNING);
			// create dir
			if ( 0 != mkdir(IPOE_EMULATOR_PATH, 0666) )
			{
				tcdbg_printf("\n%s warning create folder failed!\n", __FUNCTION__);
				return -1;
			}
			// check vlan mode
			memset(vlanmode, 0, sizeof(vlanmode));
			if( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, "VLANMode", vlanmode)
				&& 0 != vlanmode[0] )
				system(IPOE_EMULATOR_START);
			else
				setAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_DIGSTATE, IPOE_EMULATOR_ST_COMPLETE);
		}
		else if ( 0 == strcmp(IPOE_EMULATOR_ST_STOP, diag_state) )
		{
			ipoeEmulator_stop(top, IPOE_EMULATOR_STOP_SUCC);
			rm_folder = 1;
		}
		else if ( 0 == strcmp(IPOE_EMULATOR_ST_RUNNING, diag_state) )
		{
			// get state from file
			sprintf(path, "%s%s", IPOE_EMULATOR_PATH, IPOE_EMULATOR_ATTR_DIGSTATE);
			memset(info_string, 0, sizeof(info_string));
			if ( SUCCESS == get_file_string(path, info_string, 1)
 				&& 0 == strcmp(IPOE_EMULATOR_ST_COMPLETE, info_string[0]) ) // Complete
 			{
				// get result
				sprintf(path, "%s%s", IPOE_EMULATOR_PATH, IPOE_EMULATOR_ATTR_RESULT);
				memset(info_string, 0, sizeof(info_string));
				if ( SUCCESS != get_file_string(path, info_string, 1) )
					strcpy(info_string[0], IPOE_EMULATOR_RESULT_OTHER);

				if ( 0 == strcmp("DHCPOK", info_string[0]) )
				{
					// get dhcp result
					get_dhcpResult(top);
					// get ping result
					get_pingResult(top);
				}
				else
					setAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_RESULT, info_string[0]);

				// stop it
				ipoeEmulator_stop(top, IPOE_EMULATOR_STOP_SUCC);
#ifdef CWMP
				signalCwmpInfo(IPOE_EMULATOR_OK, 0, 0);
#endif
				setAttrValue(top, nodeName, IPOE_EMULATOR_ATTR_DIGSTATE, IPOE_EMULATOR_ST_COMPLETE);

				rm_folder = 1;
			}
		}

		if ( rm_folder )
		{
			memset(cmdbuf, 0, sizeof(cmdbuf));
			sprintf(cmdbuf, "/bin/rm -r %s", IPOE_EMULATOR_PATH);
			system(cmdbuf);
		}
	}
	else
	{
		tcdbg_printf("\n%s get DiagnosticsState failed\n", __FUNCTION__);
		return FAIL;
	}

	return SUCCESS;	
}
#endif

#if defined(TCSUPPORT_CT_PMINFORM)
/* update ntp sync state */
int updateNtpState(mxml_node_t *top, int syncst)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};

	if ( !top )
		return -1;

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], PMINFORM_NODE_NAME);
	strcpy(nodeName[1], PMINFORM_NODE_ENTRY);

	if ( 0 == syncst )
		setAttrValue(top, nodeName, PMINFORM_ATTR_NTPST, PMINFORM_VAL_YES );

	return 0;
}
/* update cwmp wan interface name & wan ip */
int updateCwmpInfo(mxml_node_t *top, char *wanip, int state)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};

	if ( !top || !wanip )
		return -1;

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], PMINFORM_NODE_NAME);
	strcpy(nodeName[1], PMINFORM_NODE_ENTRY);

	setAttrValue(top, nodeName, PMINFORM_ATTR_CWMPIP, 
		( 1 == state ) ? wanip : "0.0.0.0" );

	return 0;

}
/* PMInform node init */
int PMInform_init(void)
{
	int ret=0;
	cfg_node_t node={
			.name=PMINFORM_NODE_NAME,
			.type=SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=NULL,
			.cfg_boot=PMInform_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}
/* PMInform node boot */
int PMInform_boot(mxml_node_t *top)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], PMINFORM_NODE_NAME);
	strcpy(nodeName[1], PMINFORM_NODE_ENTRY);

	/* other info clear */
	setAttrValue(top, nodeName, PMINFORM_ATTR_NTPST, "");
	setAttrValue(top, nodeName, PMINFORM_ATTR_CWMPIP, "");

	return 0;
}
#endif

#if defined(TCSUPPORT_CT_STB_TEST)
/* StbTest node init */
int StbTest_init(void)
{
	int ret=0;
	cfg_node_t node={
			.name=STBTEST_NODE_NAME,
			.type=WEB_TYPE|SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=StbTest_read,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=NULL,
			.cfg_boot=StbTest_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}

/* StbTest node read */
int StbTest_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];

	memset(nodeName,0, sizeof(nodeName));
	strcpy(nodeName[0], STBTEST_NODE_NAME);
	strcpy(nodeName[1], STBTEST_NODE_ENTRY);
	setAttrValue(top, nodeName, STBTEST_WORK_STATUS, "None") ;
	return SUCCESS;
}

/* StbTest node boot */
int StbTest_boot(mxml_node_t *top)
{
	system("/userfs/bin/stb_test &");
	return SUCCESS;
}
#endif

#if defined(TCSUPPORT_CT_JOYME)
static int jvmRunCheck = 0;
/* get java machine boot flag */
int getJVMBootflag()
{
	FILE *fp = NULL;
	char bootflag[8] = {0};

	fp = fopen(JAVA_VM_STATE_FILE, "r");
	if ( !fp )
		return 0;

	if ( fread(bootflag, sizeof(char), sizeof(bootflag), fp) <= 0 )
	{
		fclose(fp);
		return 0;
	}
	fclose(fp);

	if ( 1 == atoi(bootflag) )
		return 1;
	else
		return 0;
}
/* set java machine boot flag */
int setJVMBootflag(int flag)
{
	char bootflag[8] = {0};

	snprintf(bootflag, sizeof(bootflag) - 1,
		"%d", flag);
	doValPut(JAVA_VM_STATE_FILE, bootflag);

	return 0;
}
/* get framework flag */
int getFrameworkflag()
{
	FILE *fp = NULL;
	char frameflag[8] = {0};

	fp = fopen(JAVA_FRAMEWORK_STATE_FILE, "r");
	if ( !fp )
		return 0;

	if ( fread(frameflag, sizeof(char), sizeof(frameflag), fp) <= 0 )
	{
		fclose(fp);
		return 0;
	}
	fclose(fp);

	if ( 1 == atoi(frameflag) )
		return 1;
	else
		return 0;
}
/* set framework flag */
int setFrameworkflag(int flag)
{
	char frameflag[8] = {0};

	snprintf(frameflag, sizeof(frameflag) - 1,
		"%d", flag);
	doValPut(JAVA_FRAMEWORK_STATE_FILE, frameflag);

	return 0;
}

/* get ct gateway libs flag */
int getCTGWLibsflag()
{
	FILE *fp = NULL;
	char libflag[8] = {0};

	fp = fopen(CTGWLIBS_STATE_FILE, "r");
	if ( !fp )
		return 0;

	if ( fread(libflag, sizeof(char), sizeof(libflag), fp) <= 0 )
	{
		fclose(fp);
		return 0;
	}
	fclose(fp);

	if ( 1 == atoi(libflag) )
		return 1;
	else
		return 0;
}
/* set java machine boot flag */
int setCTGWLibsflag(int flag)
{
	char libflag[8] = {0};

	snprintf(libflag, sizeof(libflag) - 1,
		"%d", flag);
	doValPut(CTGWLIBS_STATE_FILE, libflag);

	return 0;
}
/* check CTGWLib valid */
int checkCTGWLib()
{
	FILE *fp = NULL;
	char libcontent[256] = {0};

	fp = fopen(CTGWLIBS_CHECK_FILE, "r");
	if ( !fp )
		return -1;

	if ( fread(libcontent, sizeof(char), sizeof(libcontent), fp) <= 0 )
	{
		fclose(fp);
		return -1;
	}
	fclose(fp);

	if ( NULL != strstr(libcontent, "ctgw lib test ok") )
		return 0;

	return -1;
}

/* get mobile-manager flag */
int getMobilesetflag()
{
	FILE *fp = NULL;
	char mobileflag[8] = {0};

	fp = fopen(MOBILE_STATE_FILE, "r");
	if ( !fp )
		return 0;

	if ( fread(mobileflag, sizeof(char), sizeof(mobileflag), fp) <= 0 )
	{
		fclose(fp);
		return 0;
	}
	fclose(fp);

	if ( 1 == atoi(mobileflag) )
		return 1;
	else
		return 0;
}
/* set mobile-manager boot flag */
int setMobilesetflag(int flag)
{
	char mobileflag[8] = {0};

	snprintf(mobileflag, sizeof(mobileflag) - 1,
		"%d", flag);
	doValPut(MOBILE_STATE_FILE, mobileflag);

	return 0;
}
/* check mobile-manager valid */
int checkMobile()
{
	FILE *fp = NULL;
	char mobcontent[256] = {0};

	fp = fopen(MOBILE_CHECK_FILE, "r");
	if ( !fp )
		return -1;

	if ( fread(mobcontent, sizeof(char), sizeof(mobcontent), fp) <= 0 )
	{
		fclose(fp);
		return -1;
	}
	fclose(fp);

	if ( NULL != strstr(mobcontent, "test ok") )
		return 0;

	return -1;
}

/* get version profile */
int getVersion(int mode, int bootflag, char *ver)
{
	char path[128] = {0}, attrVal[64] = {0};
	char attrName[32] = "version=";

	if ( !ver )
		return -1;

	switch ( mode )
	{
		case 1 : /* OSGI framework */
			sprintf(path, "/usr/osgi/framework%d/ver.txt", bootflag);
			break;
		case 2: /* JAVA machine */
			sprintf(path, "/usr/osgi/jvm%d/ver.txt", bootflag);
			break;
		case 5: /* Mobile manager */
			sprintf(path, "/usr/osgi/mobileapps%d/ver.txt", bootflag);
			break;
		case 6: /* Gateway Libs */
			sprintf(path, "/usr/osgi/ctgwlibs%d/ver.txt", bootflag);
			break;
		default:
			return -1;
	}

	get_profile_str_new( attrName, attrVal, sizeof(attrVal), path);
	strcpy(ver, attrVal);	

	return 0;
}

/* get download state */
int getDownloadstate()
{
	FILE *fp = NULL;
	char downstate[8] = {0};

	fp = fopen(UPGRADE_CHECK_FILE, "r");
	if ( !fp )
		return 2;

	if ( fread(downstate, sizeof(char), sizeof(downstate), fp) <= 0 )
	{
		fclose(fp);
		return 3;
	}
	fclose(fp);

	return atoi(downstate);
}

/* check flash space */
int checkFlashSpace()
{
	FILE *fp = NULL;
	char error[256] = {0};

	fp = fopen(TAR_CHECK_FILE, "r");
	if ( !fp )
		return -1;

	if ( fread(error, sizeof(char), sizeof(error), fp) <= 0 )
	{
		fclose(fp);
		return -1;
	}
	fclose(fp);

	if ( NULL != strstr(error, "No space left on device") )
		return 1;

	return 0;
}



/* OSGI upgrade node init */
int OSGI_Upgrade_init(void)
{
	int ret=0;
	cfg_node_t node = {
		.name="OSGIUpgrade",
		.type=SINGLE_TYPE,
		.adv_table=NULL,
		.merge_table=NULL,
		.cfg_read=NULL,
		.cfg_write=NULL,
		.cfg_verify=NULL,
		.cfg_execute=OSGI_Upgrade_execute,
		.cfg_boot=OSGI_Upgrade_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}

/* OSGI upgrade node boot */
int OSGI_Upgrade_boot(mxml_node_t *top)
{
	char lan_nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0}, dhcpType[8] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char pusview[12] = {0}, pushselect[12] = {0};
	int isDhcpActive = 0;

	bzero(lan_nodeName, sizeof(lan_nodeName));
	strcpy(lan_nodeName[0], LAN);
	strcpy(lan_nodeName[1], LAN_DHCP);
	if ( TCAPI_PROCESS_OK == 
		getAttrValue(top, lan_nodeName, LAN_DHCP_TYPE, dhcpType)
		&& 0 == strcmp(dhcpType, "1") )
		isDhcpActive = 1;

	bzero(nodeName, sizeof(nodeName));
	strcpy(nodeName[0], "OSGIUpgrade");
	strcpy(nodeName[1], "Entry");

	system("ebtables -N OSGIUPG_CHAIN");
	system("ebtables -P OSGIUPG_CHAIN RETURN");
	system("ebtables -I FORWARD -j OSGIUPG_CHAIN");
	system("ebtables -t filter -F OSGIUPG_CHAIN");
	system("ebtables -t filter -Z OSGIUPG_CHAIN");

	system("iptables -t nat -N OSGIUPG_CHAIN");
	system("iptables -t nat -A PREROUTING -j OSGIUPG_CHAIN");
	system("iptables -t nat -F OSGIUPG_CHAIN");
	system("iptables -t nat -Z OSGIUPG_CHAIN");

	bzero(pusview, sizeof(pusview));
	if( TCAPI_PROCESS_OK == 
		getAttrValue(top, nodeName, "pushview", pusview)
	  && 0 == strcmp(pusview, "Yes") )
	{
		/* push webui mode */
		if( TCAPI_PROCESS_OK == 
		getAttrValue(top, nodeName, "pushselected", pushselect)
	  	&& 0 == strcmp(pushselect, "unset") ) /* default set */
		{
			system("iptables -t nat -A OSGIUPG_CHAIN -i br+ -p tcp"
				" --dport 80 -j REDIRECT --to-ports 80");
			system("iptables -t nat -A OSGIUPG_CHAIN -p udp"
				" --dport 53  -j REDIRECT --to-ports 53");

			if ( isDhcpActive )
				system("ebtables -t filter -A OSGIUPG_CHAIN -p IPv4 "
						"--ip-proto udp --ip-dport 53 -j DROP");
		}
	}
	else
	{
		if( TCAPI_PROCESS_OK == 
		getAttrValue(top, nodeName, "pushselected", pushselect)
	  	&& 0 == strcmp(pushselect, "later") )
	  		return 0; /* won't clear flags. */

		/* clear all flags */
		setAttrValue(top, nodeName, "cmd", "");
		setAttrValue(top, nodeName, "mode", "");
		setAttrValue(top, nodeName, "pushview", "");
		setAttrValue(top, nodeName, "pushselected", "");
		setAttrValue(top, nodeName, "pushviewstart", "");
		setAttrValue(top, nodeName, "ntpfail", "");
		setAttrValue(top, nodeName, "upgrade_ID", "");
		setAttrValue(top, nodeName, "fwver", "");
	}

	return 0;
}

/* OSGI upgrade node execute */
int OSGI_Upgrade_execute(mxml_node_t *top, char name[][MAX_NODE_NAME])
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char lan_nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0}, dhcpType[8] = {0};
	char cmdbuf[256] = {0};
	char mode[12] = {0}, pusview[12] = {0};
	char pushselect[12] = {0}, later_time_buf[16] ={0};
	int isDhcpActive = 0, currJVMFlag = 0, upgJVMFlag = 0;
	int currFrameFlag = 0, upgFrameFlag = 0;
	int currGWLibFlag = 0, upgGWLibFlag = 0;
	int currMobileFlag = 0, upgMobileFlag = 0;
	struct timeval later_time = {0};
	char upgrade_ID[128] = {0};
	char version[64] = {0}, preversion[64] = {0};

	bzero(lan_nodeName, sizeof(lan_nodeName));
	strcpy(lan_nodeName[0], LAN);
	strcpy(lan_nodeName[1], LAN_DHCP);
	if ( TCAPI_PROCESS_OK == 
		getAttrValue(top, lan_nodeName, LAN_DHCP_TYPE, dhcpType)
		&& 0 == strcmp(dhcpType, "1") )
		isDhcpActive = 1;

	bzero(nodeName, sizeof(nodeName));
	strcpy(nodeName[0], "OSGIUpgrade");
	strcpy(nodeName[1], "Entry");

	bzero(upgrade_ID, sizeof(upgrade_ID));
	getAttrValue(top, nodeName, "upgrade_ID", upgrade_ID);

	bzero(mode, sizeof(mode));
	bzero(pusview, sizeof(pusview));
	if( TCAPI_PROCESS_OK == 
		getAttrValue(top, nodeName, "pushview", pusview)
	  && 0 == strcmp(pusview, "Yes") )
	{
		/* push webui mode */
		if( TCAPI_PROCESS_OK == 
		getAttrValue(top, nodeName, "pushselected", pushselect)
	  	&& 0 != pushselect[0] )
		{
			system("ebtables -t filter -F OSGIUPG_CHAIN");
			system("ebtables -t filter -Z OSGIUPG_CHAIN");
			system("iptables -t nat -F OSGIUPG_CHAIN");
			system("iptables -t nat -Z OSGIUPG_CHAIN");

			unlink("/etc/hosts");
			system("/usr/bin/killall -9 dnsmasq");
			system("/userfs/bin/dnsmasq &");

			/* default set */
			if ( 0 == strcmp(pushselect, "unset") )
			{
				system("iptables -t nat -A OSGIUPG_CHAIN -i br+ -p tcp"
					" --dport 80 -j REDIRECT --to-ports 80");
				system("iptables -t nat -A OSGIUPG_CHAIN -p udp"
					" --dport 53 -j REDIRECT --to-ports 53");

				if ( isDhcpActive )
					system("ebtables -t filter -A OSGIUPG_CHAIN -p IPv4 "
							"--ip-proto udp --ip-dport 53 -j DROP");
			}
			/* do upgrade */
			else if ( 0 == strcmp(pushselect, "upgrade") )
			{
				;
			}
			/* do not upgrade  */
			else if ( 0 == strcmp(pushselect, "ignore") )
			{
				/* clear all flags */
				setAttrValue(top, nodeName, "cmd", "");
				setAttrValue(top, nodeName, "pushview", "");
				/* clear dns hook */
				unlink("/etc/hosts");
				system("/usr/bin/killall -9 dnsmasq");
				system("/userfs/bin/dnsmasq &");
			}
			/* upgrade later  */
			else if ( 0 == strcmp(pushselect, "later") )
			{
				/* clear all flags */
				setAttrValue(top, nodeName, "pushview", "");

				/* save current times if ntp succeed*/
				if ( 0 == ntp_server_ret )
				{
					gettimeofday(&later_time, NULL);
					snprintf(later_time_buf, sizeof(later_time_buf) - 1
						, "%lu", later_time.tv_sec);
					setAttrValue(top, nodeName
						, "pushviewstart", later_time_buf);
					setAttrValue(top, nodeName, "ntpfail", "");
				}
				/* will cancel it */
				else
				{
					tcdbg_printf("\n>>> upgrade cancel, ntp not succeed.\n");
					/* clear all flags */
					setAttrValue(top, nodeName, "cmd", "");
					setAttrValue(top, nodeName, "pushview", "");
					setAttrValue(top, nodeName, "ntpfail", "Yes");
				}

				/* clear dns hook */
				unlink("/etc/hosts");
				system("/usr/bin/killall -9 dnsmasq");
				system("/userfs/bin/dnsmasq &");
			}
		}
	}
	else
	{
		if( TCAPI_PROCESS_OK ==
			getAttrValue(top, nodeName, "mode", mode)
		  && 0 != mode[0] )
		{
			/* OSGI framework */
			if ( 0 == strcmp(mode, "1") )
			{
				/* 1. check current ver. */
				currFrameFlag = getFrameworkflag();
				/* 2. extract pack */
				jvmRunCheck = 1;
				upgFrameFlag = (1 == currFrameFlag) ? 0 : 1;
				/* 3. report error if download fail */
				if ( 0 != getDownloadstate() )
				{
					getVersion(1, currFrameFlag, version);
					sendEventInform(INFORM_RESULT_FAIL, version
						, INFORM_ERR_DLDERR, upgrade_ID);

					system("reboot -d 3 &");
					return SUCCESS;
				}

				snprintf(cmdbuf, sizeof(cmdbuf) - 1,
				"%s framework framework%d", UPGRADE_JVM, upgFrameFlag);
				system(cmdbuf);
			}
			/* JAVA machine */
			else if ( 0 == strcmp(mode, "2") )
			{
				/* 1. check current ver. */
				currJVMFlag = getJVMBootflag();
				/* 2. extract pack */
				jvmRunCheck = 1;
				upgJVMFlag = (1 == currJVMFlag) ? 0 : 1;
				/* 3. report error if download fail */
				if ( 0 != getDownloadstate() )
				{
					getVersion(2, currFrameFlag, version);
					sendEventInform(INFORM_RESULT_FAIL, version
						, INFORM_ERR_DLDERR, upgrade_ID);

					system("reboot -d 3 &");
					return SUCCESS;
				}

				snprintf(cmdbuf, sizeof(cmdbuf) - 1,
				"%s jvm jvm%d", UPGRADE_JVM, upgJVMFlag);
				system(cmdbuf);
			}
			/* Mobile manager */
			else if ( 0 == strcmp(mode, "5") )
			{
				/* 1. check current ver. */
				currMobileFlag = getMobilesetflag();
				/* 2. extract pack */
				upgMobileFlag = (1 == currMobileFlag) ? 0 : 1;
				/* 3. report error if download fail */
				if ( 0 != getDownloadstate() )
				{
					getVersion(5, currFrameFlag, version);
					sendEventInform(INFORM_RESULT_FAIL, version
						, INFORM_ERR_DLDERR, upgrade_ID);

					system("reboot -d 3 &");
					return SUCCESS;
				}

				snprintf(cmdbuf, sizeof(cmdbuf) - 1,
				"%s mobilechk mobileapps%d", UPGRADE_JVM, upgMobileFlag);
				system(cmdbuf);

				/* 4. check mobile test */
				if (  0 != checkMobile() )
				{
					/* check failed, delete old and no need to restore it. */
					snprintf(cmdbuf, sizeof(cmdbuf) - 1,
					"%s remove mobileapps%d", UPGRADE_JVM, upgMobileFlag);
					system(cmdbuf);

					getVersion(5, currMobileFlag, version);
					/* space full */
					if ( 1 == checkFlashSpace() )
					{
						sendEventInform(INFORM_RESULT_FAIL, version
							, INFORM_ERR_SPACEFULL, upgrade_ID);
					}
					else
					{
						sendEventInform(INFORM_RESULT_FAIL, version
							, INFORM_ERR_BINERR, upgrade_ID);
					}
				}
				else
				{
					/* check whether the versions are same. */
					getVersion(5, currMobileFlag, preversion);
					getVersion(5, upgMobileFlag, version);
					if ( 0 == strcmp(preversion, version) )
					{
						sendEventInform(INFORM_RESULT_FAIL, preversion
							, INFORM_ERR_SAMEVERSION, upgrade_ID);
					}
					else
					{
						/* send inform first, because mobile-manager will stop*/
						sendEventInform(INFORM_RESULT_OK, version
							, "", upgrade_ID);

						sleep(1);
						/* check ok , we will do ln */
						snprintf(cmdbuf, sizeof(cmdbuf) - 1,
						"%s mobileln mobileapps%d", UPGRADE_JVM, upgMobileFlag);
						system(cmdbuf);

						setMobilesetflag(upgMobileFlag);
					}
				}
				system("reboot -d 3 &");
			}
			/* Gateway Libs */
			else if ( 0 == strcmp(mode, "6") )
			{
				/* 1. check current ver. */
				currGWLibFlag = getCTGWLibsflag();
				/* 2. extract pack */
				upgGWLibFlag = (1 == currGWLibFlag) ? 0 : 1;
				/* 3. report error if download fail */
				if ( 0 != getDownloadstate() )
				{
					getVersion(6, currFrameFlag, version);
					sendEventInform(INFORM_RESULT_FAIL, version
						, INFORM_ERR_DLDERR, upgrade_ID);

					system("reboot -d 3 &");
					return SUCCESS;
				}

				snprintf(cmdbuf, sizeof(cmdbuf) - 1,
				"%s ctgw ctgwlibs%d", UPGRADE_JVM, upgGWLibFlag);
				system(cmdbuf);
				/* 4. check ctsgw test */
				if (  0 != checkCTGWLib() )
				{
					/* check failed, delete old then restore it. */
					snprintf(cmdbuf, sizeof(cmdbuf) - 1,
					"%s remove ctgwlibs%d", UPGRADE_JVM, upgGWLibFlag);
					system(cmdbuf);

					snprintf(cmdbuf, sizeof(cmdbuf) - 1,
					"%s restorectgw ctgwlibs%d", UPGRADE_JVM, currGWLibFlag);
					system(cmdbuf);

					getVersion(6, currGWLibFlag, version);
					/* space full */
					if ( 1 == checkFlashSpace() )
					{
						sendEventInform(INFORM_RESULT_FAIL, version
							, INFORM_ERR_SPACEFULL, upgrade_ID);
					}
					else
					{
						sendEventInform(INFORM_RESULT_FAIL, version
							, INFORM_ERR_BINERR, upgrade_ID);
					}
				}
				else
				{
					/* check whether the versions are same. */
					getVersion(6, currGWLibFlag, preversion);
					getVersion(6, upgGWLibFlag, version);
					if ( 0 == strcmp(preversion, version) )
					{
							/* same ver, then restore it. */
							snprintf(cmdbuf, sizeof(cmdbuf) - 1,
							"%s restorectgw ctgwlibs%d", UPGRADE_JVM
							, currGWLibFlag);
							system(cmdbuf);

							sendEventInform(INFORM_RESULT_FAIL, preversion
								, INFORM_ERR_SAMEVERSION, upgrade_ID);
					}
					else
					{
						setCTGWLibsflag(upgGWLibFlag);
	
						getVersion(6, upgGWLibFlag, version);
						sendEventInform(INFORM_RESULT_OK, version
							, "", upgrade_ID);
					}
				}
				system("reboot -d 3 &");
			}
			else
				return SUCCESS;
		}

	}

	return SUCCESS;
}

/* clear all osgi upgrade flags */
void clearUPGFlags()
{
	/* clear all flags */
	tcapi_set("OSGIUpgrade_Entry", "cmd", "");
	tcapi_set("OSGIUpgrade_Entry", "mode", "");
	tcapi_set("OSGIUpgrade_Entry", "pushview", "");
	tcapi_set("OSGIUpgrade_Entry", "pushselected", "");
	tcapi_set("OSGIUpgrade_Entry", "pushviewstart", "");
	tcapi_set("OSGIUpgrade_Entry", "ntpfail", "");
	tcapi_save();
}
/* cc thread for osgi upgrade */
void notify_OSGI_UPG_change()
{
	char mode[12] = {0}, pusview[12] = {0};
	char pushselect[12] = {0}, cmdbuf[512] = {0};
	char ug_type[16] = {0}, later_time[12] = {0};
	char ntpfail[8] = {0};
	int rv = 0, pidnum = 0, pid_t[128] = {0};
	int currJVMFlag = 0, upgJVMFlag = 0;
	int currFrameFlag = 0, upgFrameFlag = 0;
	long ulater_time = 0;
	static int timecnt = 0;
	static int later_check = 0;
	struct timespec press_later_Start = {0};
	struct timeval cur_time = {0};
	char upgrade_ID[128] = {0};
	char version[64] = {0}, preversion[64] = {0};
#define SEVEN_DAYS (60*60*24*7)

	bzero(upgrade_ID, sizeof(upgrade_ID));
	tcapi_get("OSGIUpgrade_Entry", "upgrade_ID", upgrade_ID);

	/* check JVM state */
	if ( 1 == jvmRunCheck )
	{
		rv = find_pid_by_name("jamvm", pid_t, &pidnum);

		timecnt ++;
		/* max timeout */
		if ( timecnt >= 40 )
		{
			tcdbg_printf("\n>>> check jvm timeout.\n");
			jvmRunCheck = 0;
			system("/usr/bin/killall -9 jamvm");

			/* check upgrade type */
			if ( TCAPI_PROCESS_OK == 
				tcapi_get("OSGIUpgrade_Entry", "mode", mode)
				&& 0 != mode[0] )
			{
				 /* OSGI framework */
				if ( 0 == strcmp(mode, "1") )
				{
					currFrameFlag = getFrameworkflag();
					upgFrameFlag = (1 == currFrameFlag) ? 0 : 1;
					/* check failed, delete old then restore it. */
					snprintf(cmdbuf, sizeof(cmdbuf) - 1,
					"%s remove framework%d", UPGRADE_JVM, upgFrameFlag);
					system(cmdbuf);

					snprintf(cmdbuf, sizeof(cmdbuf) - 1,
					"%s restoreframework framework%d"
					, UPGRADE_JVM, currFrameFlag);
					system(cmdbuf);

					getVersion(1, currFrameFlag, version);
					/* space full */
					if ( 1 == checkFlashSpace() )
					{
						sendEventInform(INFORM_RESULT_FAIL, version
							, INFORM_ERR_SPACEFULL, upgrade_ID);
					}
					else
					{
						sendEventInform(INFORM_RESULT_FAIL, version
							, INFORM_ERR_BINERR, upgrade_ID);
					}
				}
				/* JAVA machine */
				else if ( 0 == strcmp(mode, "2") )
				{
					currJVMFlag = getJVMBootflag();
					upgJVMFlag = (1 == currJVMFlag) ? 0 : 1;
					/* check failed, delete old then restore it. */
					snprintf(cmdbuf, sizeof(cmdbuf) - 1,
					"%s remove jvm%d", UPGRADE_JVM, upgJVMFlag);
					system(cmdbuf);

					snprintf(cmdbuf, sizeof(cmdbuf) - 1,
					"%s restorejvm jvm%d", UPGRADE_JVM, currJVMFlag);
					system(cmdbuf);

					getVersion(2, currJVMFlag, version);
					/* space full */
					if ( 1 == checkFlashSpace() )
					{
						sendEventInform(INFORM_RESULT_FAIL, version
							, INFORM_ERR_SPACEFULL, upgrade_ID);
					}
					else
					{
						sendEventInform(INFORM_RESULT_FAIL, version
							, INFORM_ERR_BINERR, upgrade_ID);
					}
				}
			}
			
			/* clear all flags */
			clearUPGFlags();

			usleep(100000);
			system("reboot");
			return;
		}

		/* check ok */
		if ( 0 == rv 
			&& pidnum >= 1 )
		{
			if ( timecnt >= 15 )
			{
				jvmRunCheck = 0;
				tcdbg_printf("\n>>> check jvm succeed.\n");
				system("/usr/bin/killall -9 jamvm");

				/* check upgrade type */
				if ( TCAPI_PROCESS_OK == 
					tcapi_get("OSGIUpgrade_Entry", "mode", mode)
					&& 0 != mode[0] )
				{
					 /* OSGI framework */
					if ( 0 == strcmp(mode, "1") )
					{
						currFrameFlag = getFrameworkflag();
						upgFrameFlag = (1 == currFrameFlag) ? 0 : 1;
							
						/* check whether the versions are same. */
						getVersion(1, currFrameFlag, preversion);
						getVersion(1, upgFrameFlag, version);
						if ( 0 == strcmp(preversion, version) )
						{
								/* same ver, then restore it. */
								snprintf(cmdbuf, sizeof(cmdbuf) - 1,
								"%s restoreframework framework%d"
								, UPGRADE_JVM, currFrameFlag);
								system(cmdbuf);
	
								sendEventInform(INFORM_RESULT_FAIL, preversion
									, INFORM_ERR_SAMEVERSION, upgrade_ID);
						}
						else
						{
							setFrameworkflag(upgFrameFlag);
	
							getVersion(1, upgFrameFlag, version);
							sendEventInform(INFORM_RESULT_OK, version
								, "", upgrade_ID);
						}
					}
					/* JAVA machine */
					else if ( 0 == strcmp(mode, "2") )
					{
						currJVMFlag = getJVMBootflag();
						upgJVMFlag = (1 == currJVMFlag) ? 0 : 1;
							
						/* check whether the versions are same. */
						getVersion(2, currJVMFlag, preversion);
						getVersion(2, upgJVMFlag, version);
						if ( 0 == strcmp(preversion, version) )
						{
								/* same ver, then restore it. */
								snprintf(cmdbuf, sizeof(cmdbuf) - 1,
								"%s restorejvm jvm%d", UPGRADE_JVM
								, currJVMFlag);
								system(cmdbuf);
	
								sendEventInform(INFORM_RESULT_FAIL, preversion
									, INFORM_ERR_SAMEVERSION, upgrade_ID);
						}
						else
						{
							setJVMBootflag(upgJVMFlag);
	
							getVersion(2, upgJVMFlag, version);
							sendEventInform(INFORM_RESULT_OK, version
								, "", upgrade_ID);
						}
					}
				}

				/* clear all flags */
				clearUPGFlags();

				usleep(100000);
				system("reboot");
				return;
			}
		}
		else
		{
			/* jamvm exit after 5s. */
			if ( timecnt >= 5 )
			{
				jvmRunCheck = 0;
				tcdbg_printf("\n>>> check jvm failed.\n");

				/* check upgrade type */
				if ( TCAPI_PROCESS_OK == 
					tcapi_get("OSGIUpgrade_Entry", "mode", mode)
					&& 0 != mode[0] )
				{
					 /* OSGI framework */
					if ( 0 == strcmp(mode, "1") )
					{
						currFrameFlag = getFrameworkflag();
						upgFrameFlag = (1 == currFrameFlag) ? 0 : 1;
						/* check failed, delete old then restore it. */
						snprintf(cmdbuf, sizeof(cmdbuf) - 1,
						"%s remove framework%d", UPGRADE_JVM, upgFrameFlag);
						system(cmdbuf);

						snprintf(cmdbuf, sizeof(cmdbuf) - 1,
						"%s restoreframework framework%d"
						, UPGRADE_JVM, currFrameFlag);
						system(cmdbuf);

						getVersion(1, currFrameFlag, version);
						/* space full */
						if ( 1 == checkFlashSpace() )
						{
							sendEventInform(INFORM_RESULT_FAIL, version
								, INFORM_ERR_SPACEFULL, upgrade_ID);
						}
						else
						{
							sendEventInform(INFORM_RESULT_FAIL, version
								, INFORM_ERR_BINERR, upgrade_ID);
						}
					}
					/* JAVA machine */
					else if ( 0 == strcmp(mode, "2") )
					{
						currJVMFlag = getJVMBootflag();
						upgJVMFlag = (1 == currJVMFlag) ? 0 : 1;
						/* check failed, delete old then restore it. */
						snprintf(cmdbuf, sizeof(cmdbuf) - 1,
						"%s remove jvm%d", UPGRADE_JVM, upgJVMFlag);
						system(cmdbuf);

						snprintf(cmdbuf, sizeof(cmdbuf) - 1,
						"%s restorejvm jvm%d", UPGRADE_JVM, currJVMFlag);
						system(cmdbuf);

						getVersion(2, currJVMFlag, version);
						/* space full */
						if ( 1 == checkFlashSpace() )
						{
							sendEventInform(INFORM_RESULT_FAIL, version
								, INFORM_ERR_SPACEFULL, upgrade_ID);
						}
						else
						{
							sendEventInform(INFORM_RESULT_FAIL, version
								, INFORM_ERR_BINERR, upgrade_ID);
						}
					}
				}

				/* clear all flags */
				clearUPGFlags();

				usleep(100000);
				system("reboot");

			}
		}

		return;
	}

	if ( TCAPI_PROCESS_OK == 
		tcapi_get("OSGIUpgrade_Entry", "pushselected", pushselect)
		&& 0 != pushselect[0] )
	{
		/* when push upgrade button on web */
		if ( 0 == strcmp(pushselect, "upgrade") )
		{
			/* check upgrade type */
			if ( TCAPI_PROCESS_OK == 
				tcapi_get("OSGIUpgrade_Entry", "mode", mode)
				&& 0 != mode[0] )
			{
				/* firmware upgrade */
				if ( 0 == strcmp(mode, "0") )
				{
					/* 1. download bin via WGET */
					if ( TCAPI_PROCESS_OK == 
					tcapi_get("OSGIUpgrade_Entry", "cmd", cmdbuf)
					&& 0 != cmdbuf[0] )
						system(cmdbuf);
	
					/* 2. clear all flags */
					clearUPGFlags();

					/* 3. upgrade */
					sprintf(ug_type, "%d", PHONE_NO_HEADER);
					tcapi_set("System_Entry","upgrade_fw", ug_type);
					tcapi_commit("System_Entry");
				}
				else if ( 0 == strcmp(mode, "1") /* OSGI framework */
						|| 0 == strcmp(mode, "2") /* JAVA machine */
						|| 0 == strcmp(mode, "5") /* Mobile manager */
						|| 0 == strcmp(mode, "6") /* Gateway Libs */
						)
				{
					/* 1. download jvm or framework via WGET */
					if ( TCAPI_PROCESS_OK == 
					tcapi_get("OSGIUpgrade_Entry", "cmd", cmdbuf)
					&& 0 != cmdbuf[0] )
						system(cmdbuf);

					/* 2. upgrade */
					tcapi_set("OSGIUpgrade_Entry", "pushview", "");
					tcapi_set("OSGIUpgrade_Entry", "pushselected", "");
					tcapi_save();
					tcapi_commit("OSGIUpgrade_Entry");
				}
				else
					return SUCCESS;

			}
		}
		/* when push later button on web */
		else if ( 0 == strcmp(pushselect, "later") )
		{
			tcapi_get("OSGIUpgrade_Entry", "ntpfail", ntpfail);
			if ( 0 == strcmp("Yes", ntpfail) )
			{
				if ( later_check ++ >= 5 )
				{
					later_check = 0;
					/* clear all flags */
					clearUPGFlags();
				}
			}
			else
			{
				/* ntp succeed. */
				if ( 0 == ntp_server_ret )
				{
					if ( 0 == ulater_time )
					{
						tcapi_get("OSGIUpgrade_Entry", "pushviewstart"
							, later_time);
						sscanf(later_time, "%lu", &ulater_time);
					}

					gettimeofday(&cur_time, NULL);
					/* reach 7 days */
					if ( cur_time.tv_sec - ulater_time >= SEVEN_DAYS )
					{
						tcapi_set("OSGIUpgrade_Entry", "pushview", "Yes");
						tcapi_set("OSGIUpgrade_Entry", "pushselected", "unset");
						tcapi_save();
						tcapi_commit("OSGIUpgrade_Entry");
					}
				}
			}
		}
	}
}

#endif


