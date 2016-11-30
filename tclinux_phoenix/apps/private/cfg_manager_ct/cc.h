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
#ifndef _CC_H
#define _CC_H

#if defined(TCSUPPORT_AUTO_DMZ)
#include <linux/if_ether.h>
#endif
#if defined(TCSUPPORT_TRUE_LANDING_PAGE)
#define STATUS "Status"
#define SERVICELIST "ServiceList"
#define INTERNET "INTERNET"
#endif
void start_cc(void* unused);
#if defined(TCSUPPORT_CT)
void start_cc_new(void* unused);
#endif

#ifndef PURE_BRIDGE
#if 0
void restart_igmpproxy(char *interface, int pvc);  /*rodney_20090420*/
void restart_mldproxy(char *interface,int input_pvc,int cur_wan_isp);
#endif
#ifdef QOS_DROP  /*Rodney_20091115*/
void qos_drop_restart(void);
#endif
#endif
//void restart_nat(int PVC);
void restart_nat(char *wan_if_name);
void restart_ddns();
void restart_PPP(int PVC);
void restart_dhcp(int pvc);
/*void upgrade_firmware(int sig);*/
void check_reset(void);
#ifdef TCSUPPORT_WLAN
void checkWlanBtn(void);
#endif
#ifdef WSC_AP_SUPPORT
void check_wpsbtn(void);
#endif
#if defined(TCSUPPORT_LED_BTN_CHECK)
#if defined(TCSUPPORT_BTN_CHECK)
#define PTS_NUMBER 5
void check_button_type(void);
#endif
#endif

#if defined(TCSUPPORT_CT_BUTTONDETECT)
void check_wlan_wps_button(void);
#endif

#ifdef TCSUPPORT_LED_SWITCH_BUTTON
void check_led_switch_button(int pon_stats);
void recover_pon_led(int  pon_stat);
int led_file_write(char *path, char *led_st);
#endif

void
check_dhcprelay(void);

#if defined(TCSUPPORT_SIMPLE_UPNP)
//void start_upnp(char *ppp_interface);
#endif
//void check_route(int PVC, int isp);

/*krammer add for bug 1127*/
void dproxy_restart();
#ifdef TCSUPPORT_DNSEACHPVC
void dnsmasq_restart();
void dnsmasq_restart_down();
#endif
void start_autoPVC();
void
close_autopvc();
int
read_and_set_bit_map(void);
int
autopvcIsActive();
void
commit_and_save_autopvc(int *replayce);
void
set_success_pvc(int pvcIndex,int vcpoolIndex);
void
autopvc_state_machine(int adslState);
void
reset_fork_or_not(int pvc);  /*rodney_20090420 add*/
#if defined(TCSUPPORT_AUTO_DMZ)
typedef struct arpEntry{
	char ip[20];
	char hwa[20];
	char mask[20];
	char dev[10];
	int type;
	int flags;
	struct arpEntry *next;
};

typedef struct arpHead{
	struct arpEntry *first;
	struct arpEntry *tail;
};

typedef struct {
    unsigned char  tmac[ETH_ALEN];
    unsigned char  smac[ETH_ALEN];
    unsigned short type;
} __attribute__((packed)) ether_hdr_t ;

typedef struct {
    unsigned short hwtype;
    unsigned short prot;
    unsigned char  hwlen;
    unsigned char  protlen;
    unsigned short opcode;
    unsigned char  smac[ETH_ALEN];
    unsigned int   sip;
    unsigned char  tmac[ETH_ALEN];
    unsigned int   tip;
} __attribute__((packed)) arp_hdr_t ;

typedef struct {
    ether_hdr_t ethhdr;
    arp_hdr_t  arphdr;
} __attribute__((packed)) arp_packet_t ;

#define HW_TYPE_ETHERNET    0x0001
#define ARP_OPCODE_REQUEST 0x0001
#define ARP_OPCODE_REPLY   0x0002
#define _PATH_PROCNET_ARP "/proc/net/arp"
#define LAN_IF "br0"
void
autoDMZCheck(void);

#endif

#ifdef IPV6
void 
set_cc_wan_state(int ifindex, char is_active);
#endif

#define AUTOPVC_WAIT_TIME 4
#define MAX_REPEAT_TIME 1

#define AUTOPVC_STATE_STOP 0
#define AUTOPVC_STATE_PROBE_PVC 1
#define AUTOPVC_STATE_PROBE_VC_POOL 2
#define AUTOPVC_STATE_FINISH 3

#define PVC_ENTRY_NUM 8

#define LAN_MAC_KEYWORD "LAN_MAC="
#define MAC_PATH "/etc/mac.conf"

#ifdef TR068_LED
#define ISP_BRIDGE		"3"
void
check_internet_connect(void);
#endif

#if defined(TCSUPPORT_CT_PPPINFORM)
void check_bridge_connection(void);
#endif

#if defined(TCSUPPORT_CT_AUTOREGISTER)
void check_bridge_status(void);
#endif

#if defined(TCSUPPORT_CT_ADSL_BIND1)
void check_bridge_ppp_connection(void);
#endif

#ifdef TCSUPPORT_SYSLOG
extern int do_wlan_alarm_syslog(void);
#endif

#define LAN_PORT_NUM	4
#if defined(TCSUPPORT_CT_PORTAL_MANAGEMENT)
#define PORTNUM 4
int is_portal_on(void);
#endif
#if defined(TCSUPPORT_TRUE_LANDING_PAGE)
int checkIfInternetInterfaceUp(void);
#endif
#if defined(TCSUPPORT_CT)
int checkUILockState();
#endif
#if defined(TCSUPPORT_CT_DSLITE)
#define START_DSLITE "/usr/script/dslite_start.sh %s %s %s"
#define STOP_DSLITE "/usr/script/dslite_stop.sh %s"
void notify_dslite_state();
#define DS_ACTIVE		(1<<1)
#define DS_BINDSTATE_UP	(1<<2)
#endif
#if defined(TCSUPPORT_CT_PPP_ONDEMAND)
void notify_OnDemandWan_Switch();
#define ONDEMAND_NEED_ADDROUTE		(1<<0)
#define PPPD_PID_TEMP_PATH		"/tmp/allpppd_pid"
#endif
#if defined(TCSUPPORT_CT_PON_JS) || defined(TCSUPPORT_CT_STBMAC_REPORT)
#define STB_LIST_PATH "/proc/br_fdb_stb/stb_list"
void notify_STB_change();
#endif
#if defined(TCSUPPORT_CT_JOYME)
extern void notify_OSGI_UPG_change();
#define ATTACH_LANINFO_PATH "/usr/osgi/config/laninfo.conf"
#define MAX_LDEV_NUM 64
#endif
void call_notify_dslite_state();

#endif

