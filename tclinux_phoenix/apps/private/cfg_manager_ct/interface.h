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
#ifndef _INTREFACE_H
#define _INTREFACE_H

/*LAN Page */
#define WAN	"Wan"
#define WAN_PATH "/etc/isp%d.conf"
#define WAN_ISP	"ISP"
#define WAN_RESERVED "PVCScanReserved"
#define WAN_VPI "VPI"
#define WAN_VCI "VCI"
#define WAN_ENCAP "ENCAP"
#define WAN_LLC "LLC"
#define WAN_VC "VC"
/*#define WAN_STATE	"ACTIVE"*/
#define WAN_DEACTIVE	"No"
#define WAN_ENCAP_DYN	0
#define WAN_ENCAP_STATIC	1
#define WAN_ENCAP_PPP	2
#define WAN_ENCAP_BRIDGE	3
#define WAN_ENCAP_NONE    4  /*rodney_20090506 added*/

#ifdef IPV6 //jrchen add for IPV6
#define WAN_IPV6 "IP6"
#define WAN_PREFIXV6 "PREFIX6"
#endif

#define LAN	"Lan"
#define LAN_ENTRY0	"Lan_Entry0"
#define LAN_ENTRY_0	"Entry0"
#define LAN_IP	"IP"
#define LAN_MASK	"netmask"
#define LAN_CFG_IP_INDEX	2
#define LAN_ATTR_MASK_INDEX	3
#define LAN_CFG_MASK_INDEX	4
#define LAN_IFCONFG_CMD  "/sbin/ifconfig"
#define LAN_IF_INDEX  0
#define LAN_MASK_NAME_INDEX  1
#define LAN_MASK_INDEX  2
#define LAN_IF  "br0"
#define LAN_PATH  "/etc/lanconfig.sh"
#define LAN_DHCP	"Dhcp"
#define LAN_DHCP_TYPE	"type"
#ifdef IPV6
#define LAN_IPV6	"IP6"
#define LAN_PREFIXV6    "PREFIX6"
//added by rclv 20100517

/* features of radvd*/
#define IPV6_CONFPATH "/etc/radvd.conf"
#define PREFIX_SIMBOL "PrefixIPv6"
#define PREFIX_LEN_SIMBOL "Prefixv6Len"
#define IPV6_ENABLE_SIMBOL "Enable"

#define RADVD_MODE "Mode"
#if defined(TCSUPPORT_CT_E8GUI)
#define RADVD_MANAGE_ENABLE	"ManagedEnable"
#define RADVD_OTHER_ENABLE	"OtherEnable"
#define RADVD_MINRAINTERVAL	"MinRAInterval"
#define RADVD_MAXRAINTERVAL	"MaxRAInterval"
#define RADVD_MINRAINTERVAL_TIME	"200"
#define RADVD_MAXRAINTERVAL_TIME	"600"
#endif
/* common features by radvd & dhcp6s */
#define PREFFERED_LIFETIME_SIMBOL "PreferredLifetime"
#define VALID_LIFETIME_SIMBOL "ValidLifetime"
#define BR0_ADDR "Br0Addr"

/* dhcp6s feature */
#define DHCP6S_CONFPATH "/etc/dhcp6s.conf"
#define DHCP6S_ENABLE_SIMBOL "Enable"
#define DHCP6S_MODE "Mode"
#define DNS1_SERVER "DNSserver"
#define DNS2_SERVER "SecDNSserver"
#define DHCP6S_DEFAULT_DNS "fe80::1"
#define DHCP6S_DEF_PREFER_LIFE "3600"
#define DHCP6S_DEF_VALID_LIFE "7200"
#define DHCP6S_DOMAIN "DomainName"
#define DHCP6S_DNSTYPE "DNSType"
#define DHCP6S_DNSSOURCE "DNSWANConnection"


//end of added code

#endif

#ifdef ALIAS_IP
#define LAN_ALIAS "LanAlias"
#define LAN_ALIAS_NUM 1
#define LAN_ALIAS_PATH "/etc/lanAlias%d.conf"
#define LAN_ALIAS_START_SH "/usr/script/lanAlias_start.sh"
#define LAN_ALIAS_STOP_SH "/usr/script/lanAlias_stop.sh"
#endif

#if defined(TCSUPPORT_CT_ACCESSLIMIT)
#define LAN_ACCESSLIMIT_CMD  "/usr/bin/accesslimitcmd laninfo"
#endif

#define DHCPD_PATH "/etc/udhcpd.conf"
#ifdef DHCP_PROFILE
#define DHCPD_OPTION_PATH "/etc/udhcpd_option.conf"
#define SUB_NODE_OPT60_NAME "Option60"
#define MAX_VENDOR_CLASS_ID_LEN	64
#define SUB_NODE_OPT240_NAME "Option240"
#endif

#if defined(TCSUPPORT_CT_E8GUI)
#define DHCPD_OPTION_STB_PATH "/etc/udhcpd_stb_option.conf"
#define DHCPD_OPTION_PHONE_PATH "/etc/udhcpd_phone_option.conf"
#define DHCPD_OPTION_CAMERA_PATH "/etc/udhcpd_camera_option.conf"
#define DHCPD_OPTION_HGW_PATH "/etc/udhcpd_hgw_option.conf"
#endif

#define DHCPD_PUT_INS "%s %s\n"
#define DHCPD_ATTR_INDEX	0
#define DHCPD_VALUE_INDEX	1
#define DHCPD_ATTR_IF_INDEX	0
#define DHCPD_ATTR_LEASE_INDEX	1
#ifdef STATIC_DHCP
#define DHCPD "Dhcpd"
#define MAX_STATIC_NUM 8
#endif
#define DHCPLEASE "DhcpLease"
#define MAX_LEASE_NUM 254
#define DHCPLEASE_PATH "/etc/udhcp_lease"
//added by rclv_nj 20100517
#define RADVDCONF_PATH  "/etc/radvd.conf"
// added end

/*Add for LanHost TR069 Nodes*/
#define MAX_LANHOST_NUM	254
#define LANHOST			"LanHost"
#define LEASENUM_ATTR		"LeaseNum"
#if defined(TCSUPPORT_CT_STB_TEST)
#define STBMAC_ATTR		"StbMAC"
#endif
#if defined(TCSUPPORT_CT_PMINFORM)
#define ETHDEVMAC_ATTR		"Eth%dDevMAC"
#define WLANDEVMAC_ATTR		"WlanDevMAC"
#endif
#define WLAN_COMMON_NODE	"WLan_Common"
#define WIFIMACTAB_COMMON_NODE	"wifiMacTab_Common"
#define ARP_INFO_PATH		"/tmp/arp_info"
#define HOST_MODE_DHCP	0
#define HOST_MODE_STATIC	1
#define HOST_MODE_AUTOIP	1
#define LANHOST_GET_INTERVAL	3	//seconds

#define DHCPRELAY_PATH 	"/etc/config/dhcprelay.sh"
#define DHCPRELAY_CMD 	"/userfs/bin/dhcrelay -pf /var/log/dhcrelay.pid %s\n"

#define DPROXY          "Dproxy"  /*rodney_20090506 added*/
#define DPROXY_PATH 	"/etc/dproxy.conf"
#define DPROXYAUTO_PATH 	"/etc/dproxy.auto"
#define DPROXY_TMP_PATH "/etc/dproxy.conf.tmp"
#define DPROXY_TYPE_INDEX	0

#define IGMPPROXY_CONF_PATH  "/etc/igmpproxy.conf"  /*rodney_20090420 add*/
#define IGMPPROXY_DEA_PATH   "/userfs/bin/igmpproxy" /*rodney_20090420 add*/

/*#define ZEBRA_ENTRY 	"Zebra_Entry"*/
#define ZEBRA_PATH "/etc/zebra.conf"
#define ZEBRA_IF "interface"

#define LANRIPD	"LanRipd"
#define LANRIPD_PATH 	"/etc/lan_rip.conf"
/*#define LANRIPD_ENTRY 	"LanRipd_Entry"*/
#define RIPD_PATH 	"/etc/ripd.conf"
#define RIPD_INTERFACE	"INTERFACE"
#define RIPD_VERSION	"RIPVERSION"
#define RIPD_DIRECTION	"DIRECTION"
#define RIPD_DIRECTION_NONE	"None"
#define RIPD_DEFAULT_LANIF	"br0"
#define RIPD_VER1	"RIP1"
#define RIPD_NO_AUTH_MODE	"no ip rip authentication mode\n"
#define RIPD_ROUTE_CMD	"router rip\n"
#define RIPD_REDISTRIBUTE_CMD	"redistribute connected\n"
#define RIPD_NETWORK	"network"
#define RIPD_ACCESS_DENY_CMD	"access-list DENY deny any\n"
#define RIPD_IN_ONLY_DIRECTION "IN Only"
#define RIPD_OUT_ONLY_DIRECTION "OUT Only"
#define RIPD_IN_DIRECTION "in"
#define RIPD_OUT_DIRECTION "out"
#define RIPD_V1	1
#define RIPD_V2	2
#define RIPD_IS_DENY 1

#define RIPD_MULTICAST "multicast"
#define RIPD_NO	"no"
#define RIPD_DENY_LIST	"distribute-list DENY %s %s\n"

#define RIPD_LAN_CONF_NODE "LanIF"

/*sys node*/
#ifdef TCSUPPORT_IPV6
#define SYS_NODE "Sys"
#define SYS_IP_VERSION 	"IPProtocolVersion"
#endif

/*WLAN page*/
#define WLAN	"WLan"
#define WLAN_ENTRY_0 "WLan_Entry0"
#define WLAN_ENTRY0 "Entry0"
#define WLAN_COMMON "WLan_Common"
#define WLAN_BUTTON_PATH "/proc/tc3162/wlan_button"
#define WLANSWITCHNAME  "APOn"
#define WLAN_HT_EXTCHA	"HT_EXTCHA"
#define WLAN_CHANNEL  "Channel"
#define WLAN_HT_BW		"HT_BW"
#define NOT_AUTO_CHANNEL	0
#define IS_AUTO_CHANNEL	1
#define MAX_WDS_ENTRY 4
/*Info node*/
#define INFO	"Info"
#define CURRENT_CHANNEL	"CurrentChannel"
#if defined(TCSUPPORT_CT_BUTTONDETECT)
#include<linux/ioctl.h>
#define LEDBUTTON_IOCTL_MAGIC 0x99
typedef enum e_ledbutton_ioctl
{
	LEDBUTTON_IOCTL_SELECT = _IOW(LEDBUTTON_IOCTL_MAGIC, 0, int[2]),
	LEDBUTTON_MAX_IOCTL		/* please define I/O controls above this item */	
	
} E_LEDBUTTON_IOCTL;
#endif

#define LED_SWITCH_BUTTON_PATH "/proc/tc3162/led_switch_button"
#define LED_SWITCH_ON 1
#define LED_SWITCH_OFF 0
#define LED_LOS_PATH "/proc/pon_phy/led_los"

#define LOS_STATUS_PATH	"/proc/tc3162/los_status"

/*
#define WLAN_ENTRY 	"WLan_Entry"
#define WLAN_KETSTR_K_CHAR	'K'
#define WLAN_KETSTR_S_CHAR	'S'
*/

#if	defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7592)//add by xyyou
#ifdef WLAN_MULTIDRIVER
#define WLAN_PATH "/etc/Wireless/RT2860AP/RT2860AP0.dat"
#else
#define WLAN_PATH "/etc/Wireless/RT2860AP/RT2860AP.dat"
#endif
#else
#define WLAN_PATH "/etc/Wireless/RT61AP/RT61AP.dat"

#endif

#define WLAN_AC_PATH "/etc/Wireless/RT2860AP_AC/RT2860AP.dat"

#define RIP_INTERFACE 0
#define RIP_VERSION 1
#define RIP_DIRECTION 2

//add by xyyou
#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7592)
#define	PHY_MODE_BG_MIXED	0
#define	PHY_MODE_B_ONLY	1
#define	PHY_MODE_G_ONLY	4
#define	PHY_MODE_N_ONLY	6
#define	PHY_MODE_GN_MIXED	7
#define	PHY_MODE_BGN_MIXED	9
#else
#define	PHY_MODE_BG_MIXED	0
#define	PHY_MODE_B_ONLY	1
#define	PHY_MODE_G_ONLY	2
#endif
//end xyyou

#if defined(TCSUPPORT_WLAN_AC)
#define PHY_MODE_11A_ONLY 2
#define PHY_MODE_11AN_MIXED 8
#define PHY_11VHT_N_A_MIXED 14 
#define PHY_11VHT_N_MIXED 15
#endif

//add by xmdai_nj
#if defined(TCSUPPORT_CT_WLAN_NODE)
#define TX_POWER_LEVLE_1 1
#define TX_POWER_LEVLE_2 2
#define TX_POWER_LEVLE_3 3
#define TX_POWER_LEVLE_4 4
#define TX_POWER_LEVLE_5 5
#endif


#if 0  /*Orginal ci-command mode */
int
wan_read(mxml_node_t *node);


int
wan_boot(mxml_node_t *top);
int
wan_init(void);
int
wan_write(mxml_node_t *top, mxml_node_t *parant);
int
wan_verify(mxml_node_t *node);
int
wan_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int
wan_check_filter(mxml_node_t *top);
#endif
#if 0  /*Orginal ci-command mode */
int
lan_read(mxml_node_t *node);
#endif

int
lan_boot(mxml_node_t *top);
int
lan_init(void);
int
lan_write(mxml_node_t *top, mxml_node_t *parant);
int
lan_verify(mxml_node_t *node);
int
lan_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#if 0  /*Orginal ci-command mode */
int
wlan_read(mxml_node_t *node);
#endif

#ifdef ALIAS_IP
int
lanAlias_boot(mxml_node_t *top);
int
lanAlias_init(void);
int
lanAlias_write(mxml_node_t *top, mxml_node_t *parant);
int
lanAlias_verify(mxml_node_t *node);
int
lanAlias_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int
dhcp_main_alias(const char *mainMask, const char *mainIP, const char *aliasMask, const char *aliasIP, const char *startIP, short *main_or_alias);
#endif

int
wlan_init(void);
int
wlan_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr);
int
wlan_write(mxml_node_t *top, mxml_node_t *parant);
int
wlan_verify(mxml_node_t *node);
int 
wlan_pre_unset(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int
wlan_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int
wlan_boot(mxml_node_t *top);
void
dowscd(mxml_node_t *top);
#if defined(TCSUPPORT_WLAN_AC)
int
wlan11ac_init(void);
int
wlan11ac_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr);
int
wlan11ac_write(mxml_node_t *top, mxml_node_t *parant);
int
wlan11ac_verify(mxml_node_t *node);
int
wlan11ac_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int
wlan11ac_boot(mxml_node_t *top);
#endif
#if defined(TCSUPPORT_CT_E8GUI)
void
macparsedforSSID(const char *mac, char *dout, int len);
#endif
#if 0  /*Orginal ci-command mode */
int
ripd_read(mxml_node_t *node);
#endif

#if defined(TCSUPPORT_RIPD)
int
ripd_boot(mxml_node_t *top);
int
ripd_init(void);
int
ripd_write(mxml_node_t *top, mxml_node_t *parant);
int
ripd_verify(mxml_node_t *node);
int
ripd_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#endif

#if 0  /*Orginal ci-command mode */
int
dproxy_read(mxml_node_t *node);
#endif


int
dproxy_init(void);
int
dproxy_write(mxml_node_t *top, mxml_node_t *parant);
int
dproxy_verify(mxml_node_t *node);
int
dproxy_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

#if 0  /*Orginal ci-command mode */
int
dhcpd_read(mxml_node_t *node);
#endif

int
dhcpd_boot(mxml_node_t *top);
int
dhcpd_init(void);
#ifdef STATIC_DHCP
int
dhcpd_read(mxml_node_t *top, char nodeName[][MAX_NODE_NAME], char *attr);
#endif
int
dhcpd_write(mxml_node_t *top, mxml_node_t *parant);
int
dhcpd_verify(mxml_node_t *node);
int
dhcpd_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

int
dhcpLease_init(void);
int
dhcpLease_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr);

#if 0  /*Orginal ci-command mode */
int
dhcpRelay_read(mxml_node_t *node);
#endif

#ifdef IPV6

int
radvdCall_boot(mxml_node_t *top);
int
radvdCall_init(void);
int
radvdCall_write(mxml_node_t *top, mxml_node_t *parant);
int
radvdCall_verify(mxml_node_t *node);
int
radvdCall_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);



int
dhcp6sCall_boot(mxml_node_t *top);
int
dhcp6sCall_init(void);
int
dhcp6sCall_write(mxml_node_t *top, mxml_node_t *parant);
int
dhcp6sCall_verify(mxml_node_t *node);
int
dhcp6sCall_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

#endif


int
dhcpRelay_boot(mxml_node_t *top);
int
dhcpRelay_init(void);
int
dhcpRelay_write(mxml_node_t *top, mxml_node_t *parant);
int
dhcpRelay_verify(mxml_node_t *node);
int
dhcpRelay_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

#if defined(TCSUPPORT_IGMP_PROXY)
int
igmpproxy_init(void);
int
igmpproxy_write(mxml_node_t *top, mxml_node_t *parant);
int
igmpproxy_verify(mxml_node_t *node);
int
igmpproxy_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#if defined(TCSUPPORT_CT_DSL_EX)
int
igmpproxy_boot(mxml_node_t *top);
#endif
#endif

#ifdef TCSUPPORT_MLD_PROXY
int
mldproxy_init(void);
int
mldproxy_write(mxml_node_t *top, mxml_node_t *parant);
int
mldproxy_verify(mxml_node_t *node);
int
mldproxy_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#if defined(TCSUPPORT_CT_DSL_EX)
int
mldproxy_boot(mxml_node_t *top);
#endif
#endif

int
create_rip_conf(mxml_node_t *tree);
///////*******shnwind add ***************///
#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7592)
#define WLAN_SCRIPT_PATH "/etc/Wireless/RT2860AP/WLAN_exec.sh"
#define WLAN_MCS_SCRIPT_PATH "/etc/Wireless/RT2860AP/WLAN_mcs_exec.sh"
#else
#define WLAN_SCRIPT_PATH "/etc/Wireless/RT61AP/WLAN_exec.sh"
#endif
#define WLAN_AC_SCRIPT_PATH "/etc/Wireless/RT2860AP_AC/WLAN_exec.sh"
#define WLAN_AC_MCS_SCRIPT_PATH "/etc/Wireless/RT2860AP_AC/WLAN_mcs_exec.sh"
#define WLAN_AC_APON_PATH "/etc/Wireless/WLAN_APOn_AC"
#define WLAN_AC_SCRIPT_PREFIX "/userfs/bin/iwpriv rai%d set %s=%s\n"
//#define RESTART_BOA_SCRIPT "/usr/script/restart_boa.sh"
#define WLAN_SCRIPT_PREFIX "/userfs/bin/iwpriv ra%d set %s=%s\n"
//#define WLAN_APON_PATH "/etc/Wireless/RT61AP/WLAN_APOn"
#define WLAN_APON_PATH "/etc/Wireless/WLAN_APOn"
#define WAN_START_SH "/usr/script/wan_start.sh"
#define WAN_STOP_SH "/usr/script/wan_stop.sh"
#define ROUTE_PATH 	"/etc/route.sh"
#define ROUTE2_PATH 	"/etc/route2.sh"
#define BSSIDKEYNAME	"Bssid_num=" /*krammer add for bug 1093*/
#define CHANNELKEYNAME	"Channel=" /*xyyou add for AutoChannel*/
#define WLAN_MAC_NUM 	8
#define KEY_NUM 4
#if 0
#define WLAN_MBSSID_ENTRY_NUM 14
#ifdef RT3390//add by xyyou
#ifdef WSC_AP_SUPPORT
#define WLAN_MBSSID_ENTRY_NUM 19
#else
#define WLAN_MBSSID_ENTRY_NUM 16
#endif
#else
#ifdef WSC_AP_SUPPORT
#define WLAN_MBSSID_ENTRY_NUM 18
#else
#define WLAN_MBSSID_ENTRY_NUM 15
#endif
#endif
#endif
#define WLAN_APON "1"
#define WLAN_APOFF "0"
#if defined(TCSUPPORT_CT_WLAN_NODE)
#define WLAN_SSID_ON "1"
#define WLAN_SSID_OFF "0"
#endif
int  write_wlan_config(mxml_node_t *top, int BssidNum);
int  write_wlan_exe_sh(mxml_node_t *top, int BssidNum);
int Is11nWirelessMode(mxml_node_t *top);
int txBurst_or_not(mxml_node_t *top, int BssidNum);
#ifdef WSC_AP_SUPPORT//add by xyyou	
int run_wps(mxml_node_t *top);
int wps_oob(mxml_node_t *top);
int isWPSRunning(mxml_node_t *top);
int wps_genpincode(mxml_node_t *top);
#endif

int write_dhcpd_config(mxml_node_t *top, mxml_node_t *parant, char *configFile);
#if defined(TCSUPPORT_CT_PPPOEPROXY)
int pppoeproxy_init(void);
#endif
#ifdef TCSUPPORT_IGMP_SNOOPING
void operateIgmpSnooping(mxml_node_t *top);
#endif
#if defined(TCSUPPORT_WLAN_AC)
int  write_wlan11ac_config(mxml_node_t *top, int BssidNum);
int  write_wlan11ac_exe_sh(mxml_node_t *top, int BssidNum);
int Is11acWirelessMode(mxml_node_t *top);
int txBurst_or_not_ac(mxml_node_t *top, int BssidNum);
#ifdef WSC_AP_SUPPORT//add by xyyou	
int run_ac_wps(mxml_node_t *top);
int wps_ac_oob(mxml_node_t *top);
int isWPSACRunning(mxml_node_t *top);
#endif /* WSC_AP_SUPPORT */
int reset_ac_counter(mxml_node_t *top);
#ifdef WSC_AP_SUPPORT
int wps_ac_genpincode(mxml_node_t *top);
#endif /* WSC_AP_SUPPORT */
#endif /* TCSUPPORT_WLAN_AC */

#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7592)
int
doWlanRate(mxml_node_t *top, char *Rate);
int
doWlanMcs(mxml_node_t *top, char *Mcs);
#endif
#if defined(TCSUPPORT_WLAN_AC)
int
doWlan11acRate(mxml_node_t *top, char *Rate);
int
doWlan11acMcs(mxml_node_t *top, char *Mcs);
extern int
doCheck11acHtExtcha(mxml_node_t *top,int isAuto);
#endif
#ifdef TCSUPPORT_SYSLOG
extern void check_wireless_card(char *retval);/* check the CPE has wireless card or not */
int check_wlan_exist_for_syslog(mxml_node_t *top);
#endif
#ifdef TCSUPPORT_IPV6
int Sys_init(void);
int Sys_write(mxml_node_t *top, mxml_node_t *parant);
int Sys_verify(mxml_node_t *node);
int Sys_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int Sys_boot(mxml_node_t *top);
#endif

int
lanHost_init(void);
int
lanHost_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr);
#if defined(TCSUPPORT_CT)
#define LAN_IP_MASK_PATH "/proc/tc3162/lan_ip_mask"
#endif

#define BR_FDB_PATH "/proc/br_fdb_host/stb_list"

#define LOGICID_NODE			"LogicID"
#define LOGICID_NODE_ENTRY		"Entry"
#define LOGICID_USERNAME		"Username"
#define LOGICID_PSW				"Password"
#define LOGICID_REG_STATUS    	"registerStatus"
#define LOGICID_REG_RESULT  	"registerResult"
#if defined(TCSUPPORT_CT_PON_JS)
#define	LOGICID_IS_REGISTERED	"isRegistered"
#endif
#if !defined(TCSUPPORT_CUC)
int LogicID_init(void);
int LogicID_boot(mxml_node_t *top);
int LogicID_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int LogicID_check(mxml_node_t *top);

#endif

/*used by unopen code */
char *getDefine_WLAN_PATH();

/*used by unopen code */
char *getDefine_DEF_ROMFILE_PATH();

/*used by unopen code */
int getDefine_DEFAULTROMFILE_RA_SIZE();

/*used by unopen code */
int getDefine_DEFAULTROMFILE_RA_OFFSET();

#endif
