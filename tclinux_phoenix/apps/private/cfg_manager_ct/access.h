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
#ifndef _ACCESS_H
#define _ACCESS_H

#include "cfg_manager.h"

#define SNMPD "Snmpd"
#define SNMPD_PATH 	"/etc/snmp/snmpd.conf"
#define SNMPD_STAT_PATH 	"/etc/snmp/snmpd_stat.conf"
#define SNMPD_TMP_PATH 	"/usr/etc/snmp/snmpd.conf.tmp"
#define SNMPD_PID_PATH	"/var/log/snmpd.pid"
#define SNMPD_CMD	"/userfs/bin/snmpd -c %s -p %s"
/*#define SNMPD_ENTRY 	"Snmpd_Entry"*/
#define SNMPD_ACTIVE  "ACTIVE=\"Yes\""
#define ACTIVE		"Yes"
#define DEACTIVE		"No"
#define SNMPD_STAT_INDEX  0
#define SNMPD_ATTR_INDEX  0
#define SNMPD_VALUE_INDEX  1

#define UPNPD	"Upnpd"
#define UPNPD_ACTIVE	"enable"
#if 0  /*Orginal ci-command mode */
#define UPNPD_ENTRY 	"Upnpd_Entry"
#endif
#define UPNPD_CMD 	"/userfs/bin/upnpd &"
#define UPNPD_PATH 	"/etc/igd/igd.conf"
#define UPNPD_PID_PATH "/var/log/upnpd.pid"
#define DDNS		"Ddns"
#define DDNS_PATH "/etc/ddns.conf"
#define DDNS_PID_PATH "/var/log/ez-ipupdate.pid"

#if defined(TCSUPPORT_CT_JOYME)
#define ORAY_CONFIG_PATH "/usr/osgi/plugin-b/com.chinatelecom.econet.smartgateway.ddns/data/phlinux.conf"
#define ORAY_LOG_PATH "/usr/osgi/plugin-b/com.chinatelecom.econet.smartgateway.ddns/data/phddns.log"
#define ORAY_PATH "/usr/osgi/plugin-b/com.chinatelecom.econet.smartgateway.ddns/phddns"
#endif

#define DDNS_SH	"/usr/script/ddns.sh;/usr/script/ddns_run.sh &"
/**********20080825 racing add************/
#define ACL_SH	"/tmp/etc/acl.sh"
#define ACL_STOP_SH "/usr/script/acl_stop.sh"
#define ACL "ACL"
#define ACL_COMMON "Common"
/**********20080825 racing add************/

/**********20080926 racing add************/
#if 0
#define IPMACFILTER_SH	"/tmp/etc/ipmacfilter.sh"
#define IPMACFILTER_STOP_SH "/usr/script/ipmacfilter_stop.sh"
#else
#define MACFILTER_SH	"/tmp/etc/macfilter.sh"
#define IPUPFILTER_SH	"/tmp/etc/ipupfilter.sh"
#define IPDOWNFILTER_SH	"/tmp/etc/ipdownfilter.sh"
#define IPMACFILTER_STOP_SH "/usr/script/ipmacfilter_stop.sh"
#endif

#define IPMACFILTER "IpMacFilter"
#define IPMACFILTER_COMMON "Common"
/**********20080926 racing add************/

/**********20081021 krammer add************/
#define MAX_APP_RULE 4
#if defined(TCSUPPORT_CT_E8GUI)
#define MAX_URL_RULE 100
#else
#define MAX_URL_RULE 16
#endif
#define APPFILTER "AppFilter"
#define URLFILTER "UrlFilter"
#define APP_FILTER_SH	"/tmp/etc/AppFilter.sh"
#define URL_FILTER_SH	"/tmp/etc/UrlFilter.sh"
#define URL_FILTER_BL_CLEAR	"/tmp/etc/UrlFilterClear.sh"
#define URL_PATTER_PATH "/etc/l7-protocols/Url.pat"
#define APP_STOP_SH	"/usr/script/AppFilterStop.sh "
#define URL_STOP_SH	"/usr/script/UrlFilterStop.sh "
#define STRING_COMMON 	"Common"
#define HTTP "http://"
#define MAX_URL_INPUT_LENGTH 48
#define SPCHNUMBER 8

#if defined(TCSUPPORT_CT_E8GUI)
#define	DEL_URL		1
#define	DEL_IP		2
#define TMP_MAC_FILE	"/tmp/macvalid"
#define TMP_IPUP_FILE	"/tmp/ipupvalid"
#define TMP_IPDOWN_FILE	"/tmp/ipdownvalid"
#define TMP_URL_FILE	"/tmp/urladdr"
#define URL_TMP_SH	"/tmp/Urltmp.sh"
#define URL_WHITE_POLICY_CMD	"iptables -A INPUT -m string --algo bm --string %s -j ACCEPT\n"
#endif
/**********20081021 krammer add************/
/**********20091221 jlliu add**************/
#ifdef SSL
#define SSL_CA "SslCA"
#define MAX_CA_NUM 4
#define MAX_CA_SIZE 4096
#define FRAG_SIZE 512
#if defined(TCSUPPORT_CT_ZIPROMFILE)
#define MAX_ROMFILE_SIZE (64*1024*2)
#else
#define MAX_ROMFILE_SIZE (64*1024)
#endif
/*Length of path should be less than 32 */
#define CA_PATH "/etc/CustomerCA.pem"
/*Length of path should be less than 32 */ 
#define TMP_CA_PATH "/var/tmp/CA.pem" 
#define UPLOAD_CA_PATH "/var/tmp/ca"
#define TMP_ROMFILE_PATH "/var/romfile.cfg"
#define CA_NODE_NAME "Entry"
#define CA_UPGRADE_FLAG "UpgradeFlag"
#endif
/**********20091221 jlliu add**************/
#if 0  /*Orginal ci-command mode */
int
snmpd_read(mxml_node_t *node);
#endif

#if defined(TCSUPPORT_CT_ACCESSLIMIT)
#define ACCESSLIMIT_NODE_NAME "Accesslimit"
#define ACCESSLIMIT_COMMON_NODE_NAME "Common"
#define ACCESSLIMIT_MODE "mode"
#define ACCESSLIMIT_DEBUG "debug"
#define ACCESSLIMIT_TOTALNUM "totalnum"
#define ACCESSLIMIT_LIVETIME "livetime"
#define ACCESSLIMIT_ARP_TIMEOUT "arptimeout"
#define ACCESSLIMIT_ARP_COUNT "arpcount"
#define MAX_ACCESSLIMIT_NUM 4

int
accesslimit_init(void);
int
accesslimit_boot(mxml_node_t *top);
int
accesslimit_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#endif
int
snmpd_boot(mxml_node_t *top);
int
snmpd_init(void);
int
snmpd_write(mxml_node_t *top, mxml_node_t *parant);
int
snmpd_verify(mxml_node_t *node);
int
snmpd_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#if 0  /*Orginal ci-command mode */
int
upnpd_read(mxml_node_t *node);
#endif

int
upnpd_boot(mxml_node_t *top);
int
upnpd_init(void);
int
upnpd_write(mxml_node_t *top, mxml_node_t *parant);
int
upnpd_verify(mxml_node_t *node);
int
upnpd_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#ifdef CWMP
int
cwmp_boot(mxml_node_t *top);
int
cwmp_init(void);
int
cwmp_write(mxml_node_t *top, mxml_node_t *parant);
int
cwmp_verify(mxml_node_t *node);
int
cwmp_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int
cwmpRoute_init(void);
#define 	TR069_ROUTE_CONF		"/var/run/cwmp_route"
void del_tr069_route(void);
void update_connReq_rule(mxml_node_t *top);
#if defined(TCSUPPORT_CT_E8GUI)
int
checkCertificateswitch(void);
int
setCertistatus(int ista);
#endif
#endif
#if 0  /*Orginal ci-command mode */
int
ddns_read(mxml_node_t *node);
#endif

int
ddns_init(void);
int
ddns_write(mxml_node_t *top, mxml_node_t *parant);
int
ddns_verify(mxml_node_t *node);
int
ddns_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#if defined(TCSUPPORT_CT_DSL_EX)
int
ddns_boot(mxml_node_t *top);
#if defined(TCSUPPORT_CT_E8DDNS)
int ddnsATM_init(void);
int ddnsPTM_init(void);
#endif
#endif

#if defined(TCSUPPORT_CT_E8DDNS)
void add_DDNS_CommonInfo(mxml_node_t *top, int index);
#endif
/**shnwind add*****************/
#if defined(TCSUPPORT_SIMPLE_UPNP)
int
write_upnp_conf(mxml_node_t *top);
#endif
#if defined(TCSUPPORT_CT_JOYME)
#define MAX_SAR_RULE 64
#define STORAGE_ACCESS_SH "/tmp/storage_access_rules.sh"
int
storage_access_right_init(void);
int storage_access_right_write(mxml_node_t *top, mxml_node_t *parant);
int storage_access_right_boot(mxml_node_t *top);
int storage_access_right_execute
(mxml_node_t *top, char name[][MAX_NODE_NAME]);

#define AWN_DOMAIN_NUM 8
int
attach_wan_dnstunnel_init(void);
int attach_wan_dnstunne_boot(mxml_node_t *top);
int attach_wan_dnstunne_execute
(mxml_node_t *top, char name[][MAX_NODE_NAME]);

#define MAX_DNS_LIMIT_RULE 16
int dns_speed_limit_init(void);
int dns_speed_limit_boot(mxml_node_t *top);
int dns_speed_limit_execute
(mxml_node_t *top, char name[][MAX_NODE_NAME]);

#endif
#endif

/**********20080825 racing add************/
#if 0  /*Orginal ci-command mode */
int
acl_read(mxml_node_t *node);
#endif

int
acl_boot(mxml_node_t *top);
int
acl_init(void);
int
acl_write(mxml_node_t *top, mxml_node_t *parant);
int
acl_verify(mxml_node_t *node);
int
acl_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int
acl_check_filter(mxml_node_t *top);
#if defined(TCSUPPORT_CT_E8GUI)
int
url_filter_deletewhiterules(int ipolicy);
#endif
/**********20080825 racing add************/

/**********20080926 racing add************/
#if 0  /*Orginal ci-command mode */
int
ipfilter_read(mxml_node_t *node);
#endif

int
ipfilter_boot(mxml_node_t *top);
int
ipfilter_init(void);
int
ipfilter_write(mxml_node_t *top, mxml_node_t *parant);
int
ipfilter_verify(mxml_node_t *node);
int
ipfilter_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int
ipfilter_check_filter(mxml_node_t *top);
#if defined(TCSUPPORT_CT_E8GUI)
int
ipfilter_read(mxml_node_t *top, char nodeName[][MAX_NODE_NAME], char *attr);
#endif
/**********20080926 racing add************/

/**********20081021 krammer add************/

int
app_filter_boot(mxml_node_t *top);
int
app_filter_init(void);
int
app_filter_write(mxml_node_t *top, mxml_node_t *parant);
int
app_filter_verify(mxml_node_t *node);
int
app_filter_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int
app_check_filter(mxml_node_t *top);
/**********20081021 krammer add************/

/**********20081021 krammer add************/
int
url_filter_boot(mxml_node_t *top);
int
url_filter_init(void);
int
url_filter_write(mxml_node_t *top, mxml_node_t *parant);
int
url_filter_verify(mxml_node_t *node);
int
url_filter_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int
url_check_filter(mxml_node_t *top);
#if defined(TCSUPPORT_CT_E8GUI)
int
url_filter_read(mxml_node_t *top, char nodeName[][MAX_NODE_NAME], char *attr);
int
unset_action(const char *pstr, int itype);
#endif
/**********20081021 krammer add************/
/**********20091221 jlliu add**************/
#ifdef SSL
int
sslca_boot(mxml_node_t *top);
int
sslca_init(void);
int
sslca_write(mxml_node_t *top, mxml_node_t *parant);
int
sslca_verify(mxml_node_t *node);
int
sslca_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#if defined(TCSUPPORT_CT_E8GUI)
int
writecafiletoflash(int index);
#endif
#endif
/**********20091221 jlliu add**************/
#ifdef CWMP
/**********20091202 xyzhu add**************/
int
tr069Attr_init(void);
/**********20091202 xyzhu add**************/
#endif

#if defined(TCSUPPORT_CT_FTP_DOWNLOADCLIENT)
/* FTP Client Download */
#define APPFTPHISTORY_NUM 				10		//how many history items
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
#define APPFTPCLIENT_NUM 				10		//how many ftp client items
#endif
#define APPFTP_HISTORYMAXID 			4096	//max history id

//node name
#define APPFTP_NODE 					"appFTP"
#define APPFTP_DOWNLOADING 			"Downloading"
#define APPFTP_ENTRY 					"Entry"

//node attribute name
#define APPFTP_USERNAME				"username"
#define APPFTP_PASSWORD 				"password"
#define APPFTP_URL 						"url"
#define APPFTP_PORT 					"port"
#define APPFTP_DEVICEID 				"deviceid"
#define APPFTP_SAVEPATH 				"savepath"
#define APPFTP_RESULT 					"result"
#define APPFTP_HISTORYID 				"id"
#define APPFTP_HISTORYCOUNT 			"historycount"

#define APPFTP_INVALID 					"N/A"

#define APPFTP_RES_DOWNLOADING 		"10"    //dowanloading id
#define APPFTP_RES_BAD_URL	 			"3"		//url error id

//string max length define (MUST >=4)
#define APPFTP_MAX_ID_LENGTH				5
#define APPFTP_MAX_COMMAND_LENGTH		500
#define APPFTP_MAX_URL_LENGTH				260		//wide character
#define APPFTP_MAX_USERNAME_LENGTH		25
#define APPFTP_MAX_PASSWORD_LENGTH		25
#define APPFTP_MAX_PORT_LENGTH			10
#define APPFTP_MAX_DEVICEID_LENGTH		10
#define APPFTP_MAX_SAVEPATH_LENGTH		90		//wide character
#define APPFTP_MAX_RESULT_LENGTH			5
#define APPFTP_MAX_HISTORYCOUNT_LENGTH	5

int
appftp_init(void);
int
appftp_boot(mxml_node_t *top);
int
appftp_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#endif

#if defined(TCSUPPORT_CT_USB_BACKUPRESTORE)
/* USB Backup and  Restore */
#define USBRESTORE_NODE 					"usbRestore"
#define USBRESTORE_MAX_COMMAND_LENGTH	500
#define USBRESTORE_SRCPATH_LENGTH 		100
#define USBRESTORE_DEV_LENGTH				50
#define USBRESTORE_TARGET_LENGTH			50
#define USBRESTORE_OPT_LENGTH 			3
#define USBRESTORE_DEV 						"dev"
#define USBRESTORE_TARGET 					"target"
#define USBRESTORE_RESULT 					"rstresult"
#define USBRESTORE_OPT 						"opt"
#define USBRESTORE_EXECCMDFORMAT 		"backuprestorecmd -b -%s -/tmp/mnt/%s/%s"
#define USBRESTORE_EXECCMDDEL 			"rm -f /tmp/mnt/%s/%s"
#define USBRESTORE_SOURCE					"/tmp/var/romfile.cfg~~"

enum OPTION{
	OPT_NONE,
	OPT_DOACT,
	OPT_DELSAMEFILE,
	OPT_SETRESULT,
};

enum RESULT{
	RESULT_NOACT = 0,
	RESULT_SUCCESS,
	RESULT_USBCONERR,
	RESULT_SPACENOTENOUGH,
	RESULT_SAMEFILE,
	RESULT_NOBKFILE,
	RESULT_NOROMFILE,
	RESULT_BKFILECHGED,
	RESULT_FAIL,
};

int 
usbrestore_init(void);
int 
usbrestore_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#endif

#if defined(TCSUPPORT_CT_L2TP_VPN)
#define VPNSTR				"VPN"
#define VPN_COMMON			"Common"
#define VPN_ENTRY			"Entry"
#define VPN_ATTR_ACTION		"vpnaction"
#define VPN_ACT_ATTACH		"attach_chg"
#define VPN_ACT_WANRESTART	"wan_restart"
#define VPN_INSTANCE_NUM	16

#define VPN_PREPATH		"/var/run/xl2tpd/"
#define VPN_CONF_PATH		VPN_PREPATH"xl2tpd.conf"
#define VPN_PPP_PREPATH		VPN_PREPATH"ppp/peers/"
#define VPN_TUNNEL_NAME_PRE	"vpn_tunnel_"
/*
	open all vpn ppp tunnel
*/
int open_VPN_PPP_tunnel(mxml_node_t *top);
int
vpnlist_init(void);
int
vpnlist_write(mxml_node_t * top,mxml_node_t * parant);
int
vpnlist_verify(mxml_node_t *node);
int
vpnlist_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);	
int
vpnlist_boot(mxml_node_t *top);
#endif
#if defined(TCSUPPORT_CT_PHONEAPP)
#if defined(TCSUPPORT_CT_JOYME)
#define PLUGINSTR			"Plugin"
#define PLUGIN_COMMON		"Common"
#define PLUGIN_ENTRY		"Entry"
#define PLUGIN_INSTANCE_NUM	100

#define USBMOUNTSTR			"UsbMount"
#define USBMOUNT_COMMON		"Common"
#define USBMOUNT_ENTRY		"Entry"
#define USBMOUNT_INSTANCE_NUM	3

int
pluginlist_init(void);

int
usbmount_init(void);

int
usbmount_write(mxml_node_t * top,mxml_node_t * parant);

int
usbmount_verify(mxml_node_t *node);
int
usbmount_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);	
int
usbmount_boot(mxml_node_t *top);

#endif
int
mobile_init(void);
int
mobile_write(mxml_node_t *top, mxml_node_t *parant);
int
mobile_verify(mxml_node_t *node);
int
mobile_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);	
int
mobile_boot(mxml_node_t *top);
#endif
#if defined(TCSUPPORT_CT_UPNP_DM)
int
upnp_dm_init(void);
int
upnp_dm_write(mxml_node_t *top, mxml_node_t *parant);
int
upnp_dm_verify(mxml_node_t *node);
int
upnp_dm_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
upnp_dm_boot(mxml_node_t *top);

int
upnpdm_SoftwareProfile_boot(mxml_node_t *top);
int 
upnpdm_SoftwareProfile_init(void);
int
upnpdm_SoftwareProfile_write(mxml_node_t *top, mxml_node_t *parant);
int
upnpdm_SoftwareProfile_verify(mxml_node_t *node);
int
upnpdm_SoftwareProfile_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);


int
upnpdm_ServiceProfile_boot(mxml_node_t *top);
int
upnpdm_ServiceProfile_init(void);
int
upnpdm_ServiceProfile_write(mxml_node_t *top, mxml_node_t *parant);
int
upnpdm_ServiceProfile_verify(mxml_node_t *node);
int
upnpdm_ServiceProfile_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);


int
upnpdm_APInternet_boot(mxml_node_t *top);
int
upnpdm_APInternet_init(void);
int
upnpdm_APInternet_write(mxml_node_t *top, mxml_node_t *parant);
int
upnpdm_APInternet_verify(mxml_node_t *node);
int
upnpdm_APInternet_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

int
upnpdm_APIptv_boot(mxml_node_t *top);
int
upnpdm_APIptv_init(void);
int
upnpdm_APIptv_write(mxml_node_t *top, mxml_node_t *parant);
int
upnpdm_APIptv_verify(mxml_node_t *node);
int
upnpdm_APIptv_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

#endif

