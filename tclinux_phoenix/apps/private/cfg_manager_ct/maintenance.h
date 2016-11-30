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
#ifndef _MAINTENANCE_H
#define _MAINTENANCE_H

#include "cfg_manager.h"
/*
#define ACCOUNT		"Account"
#define ACCOUNT_ENTRY 	"Account_Entry"
*/
#define ACCOUNT_PATH 	"/etc/passwd.conf"
#define WEBPASSWD_PATH 	"/etc/passwd"
#define CONSOLE_PATH 	"/etc/usertty"
#define CONSOLE_PATH2	"/etc/userttyconsole"
#if defined(TCSUPPORT_CT_NETWORKMANAGESERVICE)
#define FTPPASSWD_PATH 	"/etc/ftppasswd"
#define FTPSERVER_FILE_PATH "/etc/bftpd.conf"
#define FTPSERVER_FILE_PATH_TMP "/etc/bftpdtmp.conf"
#define CONSOLE_PID_PATH	"/etc/console.pid"
#define CONSOLE_PID_TEMP_PATH "/tmp/console_pid"
#define  ACCOUNT		"Account"
#define  ACCOUNT_ENTRY		"Entry"
#define  TELNET_ENTRY		"TelnetEntry"
#define  FTP_ENTRY		"FtpEntry"
#define CONSOLE_ENTRY		"ConsoleEntry"
#define ACCOUNT_NODE_ACTIVE	"Active"
#define TELNETDEFAULTPORT	"23"
#define FTPDEFAULTPORT	"21"
#endif
#define SLAT "$1$"
#define ACCOUNT_DEFAULT_STRING "%s:%s:0:0:root:/:/bin/sh\n"
#define ACCOUNT_WEB_INDEX 0
#define ACCOUNT_CONSOLE_INDEX 1

#define DEVACCOUNT_STATUS    "registerStatus"
#define DEVACCOUNT_RESULT  "registerResult"
#define DEVACCOUNT_RETRYLIMIT  "retryLimit"
#define DEVACCOUNT_RETRYTIMES    "retryTimes"
#define DEVACCOUNT_USERNAME    "userName"
#define DEVACCOUNT_USERPASSWD    "userPasswordDEV"
#define DEVACCOUNT_WEBPAGELOCK  "webpageLock"
#define DEVACCOUNT_FAILUREWARNING  "failureWarning"
#define RESETRETRY	180
#if defined(TCSUPPORT_CT_PON)
#define EPON_LOID_USR    "LOID0"
#define GPON_LOID_USR    "LOID"
#define EPON_LOID_PSW    "Password0"
#define GPON_LOID_PSW    "Password"
#endif

#if defined(TCSUPPORT_CT_MONITORCOLLECTOR)
#define MONITOR_COLLECTOR_NODENAME 		"MonitorCollector"
#define MONITOR_COLLECTOR_TFTP_PATH 	"/usr/script/tftp.sh"
#define MONITOR_COLLECTOR_MAXNUM	5
#define MONITOR_COLLECTOR_SWITCH 20
#endif

/*
#define TIMEZONE_ENTRY 	"Timezone_Entry"
*/
#define TIMEZONE	"Timezone"
#define TZ_NTP_TYPE	0
#define TZ_PC_TYPE	1
#define TZ_MANUAL_TYPE	2
#if 0  /*Orginal ci-command mode */
#define TIMEZONE_PATH "/etc/timezone.conf"
#endif
#define NTP_PATH "/etc/ntp.sh"
#define NTP_DEF_SERVER 	"ntp1.cs.wisc.edu"
#define NTP_DEF_SERVERIP 	"0.0.0.0"
#if defined(TCSUPPORT_CT_2NTP)
#define TIMEZONE_ATTR_SERVER2 	"SERVER2"
#if defined(TCSUPPORT_CT_5NTP)
#define TIMEZONE_ATTR_SERVER3 	"SERVER3"
#define TIMEZONE_ATTR_SERVER4 	"SERVER4"
#define TIMEZONE_ATTR_SERVER5 	"SERVER5"
#endif
#if defined(TCSUPPORT_CT_NTPSERVERTYPE)
#define TIMEZONE_ATTR_NTPSERVERTYPE 	"NTPServerType"
#else
#define TIMEZONE_ATTR_ADDROUTE 	"AddRoute"
#define TIMEZONE_ATTR_IFINDEX 	"IFIndex"
#endif
#define TMP_DNS_NTP_PATH "/tmp/ntpdns.conf"
#define NTP_PATH2 "/etc/ntp2.sh"
#if defined(TCSUPPORT_CT_5NTP)
#define NTP_PATH3 "/etc/ntp3.sh"
#define NTP_PATH4 "/etc/ntp4.sh"
#define NTP_PATH5 "/etc/ntp5.sh"
#endif

#endif
#define NTP_CMD	"/userfs/bin/ntpclient -s -c 1 -l -h %s"
#define TIMEZONE_ATTR_SERVER 	"SERVER"
#define MAX_DIFFERENT_TIME 30
/*
#define AUTOEXEC	"Autoexec"
#define AUTOEXEC_ENTRY "Autoexec_Entry"
#define AUTOEXEC_ATTR "command%d"
*/
#define AUTOEXEC_PATH "/etc/autoexec.sh"

/*
#define MAC	"Mac"
#define MAC_ENTRY "Mac_Entry"
*/
#define MAC_PATH "/etc/mac.conf"
/*
#define LED	"Led"
#define LED_ENTRY "Led_Entry"
*/
#define LED_PATH "/userfs/led.conf"
#define LED_PROC_PATH "/proc/tc3162/led_def"

#if defined(TCSUPPORT_EPON_OAM_CUC)
#define CUC_PING_TEST_NODE_NAME "CucPing"
#define CUC_PING_TEST_ENTRY "Entry"
#define CUC_PING_TEST_RESULT_RUNNING	"1"
#define CUC_PING_TEST_RESULT_NOT_START	"2"
#define CUC_PING_TEST_RESULT_UNKNOWN_IP	"4"
#define CUC_PING_TEST_RESULT_NO_ROUTE		"5"
#define CUC_PING_TEST_PID_PATH "/var/run/cuc_voip_ping.pid"
#define CUC_PING_TEST_ADDROUTE_CMD	"route add -host %s dev %s"
#define CUC_PING_TEST_DELROUTE_CMD	"route del %s"
#define CUC_PING_TEST_CMD	"/bin/ping -U 1 -c %s  -s %s -W %s %s &"
#define CUC_PING_TEST_ACTION_COMPLETE	"2"
#endif

#if defined(TCSUPPORT_PPPOE_SIMULATE)
#define PPPOE_SIMULATE_ENTRY "Entry"
#define PPPOE_SIMULATE_NODE_NAME "PPPoESimulate"
#define PPPOE_SIMULATE_PID_PATH "/var/run/pppoe_sim.pid"
#define PPPOE_SIMULATE_CONF		"/var/run/pppoe_sim.conf"
#define PPPOE_SIMULATE_SH "/usr/script/pppoe_simulate.sh"
#define PPPOE_SIMULATE_USER_STOP		1
#define PPPOE_SIMULATE_UNKNOW_STOP 	2
#define PPPOE_SIMULATE_COMPLETE			3
#endif

#if defined(TCSUPPORT_CT_PPPOE_EMULATOR)
enum PPPOE_EMULATOR_STOP_Flags {
	PPPOE_EMULATOR_STOP_WAIT_FLAG=0,
	PPPOE_EMULATOR_STOP_NOWAIT_FLAG,

	PPPOE_EMULATOR_STOP_FLAG_END
};

#define PPPOE_EMULATOR_STOP_FLAG
#define PPPOE_EMULATOR_ENTRY "Entry"
#define PPPOE_EMULATOR_NODE_NAME "PppoeEmulator"
#define PPPOE_EMULATOR_PID_PATH "/var/run/pppoe_sim.pid"
#define PPPOE_EMULATOR_CONF		"/var/run/pppoe_sim.conf"
#define PPPOE_EMULATOR_SH "/usr/script/pppoe_simulate.sh"
#define PPPOE_EMULATOR_USER_STOP		1
#define PPPOE_EMULATOR_UNKNOW_STOP 	2
#define PPPOE_EMULATOR_COMPLETE			3
#if defined(TCSUPPORT_CT_PPPOE_IPV6_EMULATOR)
#define PPPOE_EMULATOR_ATTR_NASNAME "NASName"
#define PPPOE_EMULATOR_NAS_NAME "emulator"
#define PPPOE_EMULATOR_PPPNAME "ppp199"
#define PPPOE_EMULATOR_PPPIF_PATH "/var/run/ppp199/"
#define PPPOE_EMULATOR_PPPV6UP_PRE_SCRIPT "/usr/etc/ppp/emu_ipv6-up_pre"
#define PPPOE_EMULATOR_PPPV6UP_SCRIPT "/usr/etc/ppp/emu_ipv6-up"
#define PPPOE_EMULATOR_PPPDHCP6C_SCRIPT "/usr/script/emu_dhcp6c_script"
#define PPPOE_EMULATOR_PPPDHCP6C_PATH PPPOE_EMULATOR_PPPIF_PATH"dhcp6c.pid"
#define PPPOE_EMULATOR_PPPDHCP6C_CONF PPPOE_EMULATOR_PPPIF_PATH"dhcp6c.conf"
#define PPPOE_EMULATOR_GWV6_PATH "/proc/sys/net/ipv6/neigh/"PPPOE_EMULATOR_PPPNAME"/default_route"
#define PPPOE_EMULATOR_SLAACV6_PATH "/proc/sys/net/ipv6/conf/"PPPOE_EMULATOR_PPPNAME"/slaac_addr"
#define PPPOE_EMULATOR_RAFLAGS_PATH "/proc/sys/net/ipv6/neigh/"PPPOE_EMULATOR_PPPNAME"/ra_flags"
#define PPPOE_EMULATOR_SLAACPREFIX_PATH "/proc/sys/net/ipv6/conf/"PPPOE_EMULATOR_PPPNAME"/slaac_prefix"
#define	PPPOE_EMULATOR_MFLAG (1<<7)
#define	PPPOE_EMULATOR_OFLAG (1<<6)
#define PPPOE_EMULATOR_DOMAIN_FILE "/tmp/pppemulatordomain"
#define PPPOE_EMULATOR_DNSROUTE_ADD "/usr/bin/ip -6 route add %s/128 via %s dev "PPPOE_EMULATOR_PPPNAME
#endif

#define PPPOE_EMULATOR_BRIDGE_NODE_NAME "PppoeEmulatorBridge"
#define PPPOE_EMULATOR_BRIDGE_INFO_NODE_NAME "PppoeEmulatorBridgeInfo"
#define PPPOE_EMULATOR_BRIDGE_CONF		"/var/run/pppoe_sim_bridge.conf"
#define PPPOE_EMULATOR_BRIDGE_SH "/usr/script/pppoe_simulate_bridge.sh"
#define PPPOE_EMULATOR_BRIDGE_PID_PATH "/var/run/pppoe_sim_bridge.pid"
#define PPPOE_EMULATOR_BRIDGE_WAIT_TIMEOUT 50

void pppoe_Emulator_Bridge_stop(mxml_node_t *top, int flag);

#endif

#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
#define IPPINGDIAGNOSTIC_ENTRY "CtDiagnostic_Entry%d"
#define CTCOM_PING_COMMON "Common"
#define CTCOM_PING_PID_PATH "/tmp/cwmp/ct_ping%d.pid"
#define CTCOM_MAX_IPPINGDIAGNOSTIC_NUM 3
#define CTCOM_IPPINGDIAGNOSTIC_NODE_NAME "CtDiagnostic"
#define CTCOM_PING_ROUTE_PATH "/tmp/cwmp/ct_ping_route"
#define CTCOM_STOPPING_FROM_CWMP "1"
#define CTCOM_STOPPING_FROM_PING "2"
#define IPPINGDIAGNOSTIC_DNSURL "/tmp/ippingdiag.conf"
#endif
#if 0  /*Orginal ci-command mode */
#define _XOPEN_SOURCE

char *
crypt(const char*key, const char*slat);

int
account_read(mxml_node_t *node);
#endif

#if defined(TCSUPPORT_CT_ALARMMONITOR)
#define ALARM_NODENAME		"DeviceAlarm"
#define MONITOR_NODENAME		"DeviceMonitor"
#endif
#define ENTRY_NODENAME		"Entry%d"
#define COMMON_NODENAME		"Common"
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
#define MAX_USER_NUM		4
#define MAC_FILTER_NUM_PER_USER		8
#define IP_FILTER_NUM_PER_USER		8
#define MAX_MAC_FILTER_NUM				MAX_USER_NUM*MAC_FILTER_NUM_PER_USER
#define MAX_IP_FILTER_NUM				MAX_USER_NUM*IP_FILTER_NUM_PER_USER
#define WHITE_URL_NUM_PER_USER				8
#define BLACK_URL_NUM_PER_USER				8
#define MAX_WHITE_URL_NUM				MAX_USER_NUM*WHITE_URL_NUM_PER_USER
#define MAX_BLACK_URL_NUM				MAX_USER_NUM*BLACK_URL_NUM_PER_USER
#endif
int
account_init(void);
int
account_write(mxml_node_t *top, mxml_node_t *parant);
int
account_verify(mxml_node_t *node);
int
account_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#if defined(TCSUPPORT_CT_NETWORKMANAGESERVICE)
int getAccountCurrentIndex(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int parseftpport(const char *port);
int
account_boot(mxml_node_t *top);
#endif
#if defined(CT_COM_DEVICEREG) || defined(TCSUPPORT_CT_PON_BIND2_WEB)
int
deviceAccount_boot(mxml_node_t *top);
int
deviceAccount_init(void);
int
deviceAccount_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr);
int
deviceAccount_write(mxml_node_t *top, mxml_node_t *parant);
int
deviceAccount_verify(mxml_node_t *node);
int
deviceAccount_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#endif

#if 0  /*Orginal ci-command mode */
int
timezone_read(mxml_node_t *node);
#endif
int
timezone_init(void);
int
timezone_boot(mxml_node_t *top);
#ifdef TCSUPPORT_CUC_TIME_DISPLAY
int
timezone_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr);
#endif
int
timezone_write(mxml_node_t *top, mxml_node_t *parant);
int
timezone_verify(mxml_node_t *node);
int
timezone_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

#if 0  /*Orginal ci-command mode */
int
mac_read(mxml_node_t *node);
#endif
int
mac_init(void);
int
mac_write(mxml_node_t *top, mxml_node_t *parant);
int
mac_verify(mxml_node_t *node);
int
mac_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#if 0  /*Orginal ci-command mode */
int
led_read(mxml_node_t *node);
#endif
int
led_init(void);
int
led_write(mxml_node_t *top, mxml_node_t *parant);
int
led_verify(mxml_node_t *node);
int
led_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

#if 0  /*Orginal ci-command mode */
int
autoexec_read(mxml_node_t *node);
#endif

int
autoexec_boot(mxml_node_t *top);
int
autoexec_init(void);
int
autoexec_write(mxml_node_t *top, mxml_node_t *parant);
int
autoexec_verify(mxml_node_t *node);
#if 0
int
autoexec_execute(mxml_node_t *node,char *name);
#endif

int
correct_sys_time(mxml_node_t *top);

void
correctPPPtime(int difference);

#if defined(TCSUPPORT_EPON_OAM_CUC)
int cuc_ping_test_init(void);
#endif

#if defined(TCSUPPORT_PPPOE_SIMULATE)
int pppoe_simulate_init(void);
#endif

#if defined(TCSUPPORT_CT_PPPOE_EMULATOR)
int pppoeEmulator_init(void);
int pppoeEmulatorBridge_init(void);
int pppoeEmulatorBridgeInfo_init(void);

#if defined(TCSUPPORT_CT_PPPERRCODE_ENHANCE)
#define ERROR_OUT_OF_RESOURCES "ERROR_SERVER_OUT_OF_RESOURCES_IPv6"
#define ERROR_PARAMNEGOFAILV4 "ParamNegoFail_IPv4"
#else
#define ERROR_OUT_OF_RESOURCES "ERROR_SERVER_OUT_OF_RESOURCES"
#endif
#endif

#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
int
ctcom_ippingdiagnotic_init(void);

int
ctcom_ippingdiagnotic_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int
ctcom_ippingdiagnotic_boot(mxml_node_t *top);
void stopCwmpPing(int entryindex);
void stopAllPingOperation();

#endif
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
int
configAuth_init(void);
int
phyPort_init(void);
#endif

#if defined(TCSUPPORT_CT_ALARMMONITOR)
int
devAlarm_init(void);
int
devAlarm_write(mxml_node_t *top, mxml_node_t *parant);
int
devAlarm_verify(mxml_node_t *node);
int
devMonitor_init(void);
int
devMonitor_write(mxml_node_t *top, mxml_node_t *parant);
int
devMonitor_verify(mxml_node_t *node);
#endif
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
int
user_init(void);
int
user_boot(mxml_node_t *top);
int
user_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

int
usr_macfilter_init(void);
int
usr_macfilter_boot(mxml_node_t *top);
int
usr_macfilter_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

int
usr_ipfilter_init(void);
int
usr_ipfilter_boot(mxml_node_t *top);
int
usr_ipfilter_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

int
usr_whiteurl_init(void);
int
usr_whiteurl_boot(mxml_node_t *top);
int
usr_whiteurl_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

int
usr_blackurl_init(void);
int
usr_blackurl_boot(mxml_node_t *top);
int
usr_blackurl_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#endif

#if defined(TCSUPPORT_CT_MONITORCOLLECTOR)
enum wan_action_type{
	WAN_TYPE_NOCARE = 0,
	IP_WAN_TYPE,
	PPP_WAN_TYPE,
	WAN_MAX_ACTION_TYPE,
};

enum wan_dev_action{
	WAN_DEV_ADD = 1,
	WAN_DEV_DEL,
	WAN_DEV_MAX_ACTION,
};

enum paraList_wan_item{
	wan_ConnectionStatus = 0,
	wan_LastConnectionError,
#if defined(TCSUPPORT_CT_PPPERRCODE_ENHANCE)
	wan_IPv6ConnectionStatus,
#if defined(TCSUPPORT_CT_PON_GDV20)
	wan_LastConnectionErrorCode,
#else
	wan_ConnectionType,
#endif
#endif
	max_paraList_wan_item,
};


#define MONITOR_COLLECTOR_TFTP_PATH 	"/usr/script/tftp.sh"

int MonitorCollector_init(void);
int MonitorCollector_boot(mxml_node_t *top);	
int MonitorCollector_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#endif
#if defined(TCSUPPORT_CT_IPOE_EMULATOR)
#define IPOE_EMULATOR_ENTRY "Entry"
#define IPOE_EMULATOR_NODE_NAME "IpoeEmulator"
#define IPOE_EMULATOR_NAS_NAME "ipoe_emu"
#define IPOE_EMULATOR_PATH "/var/run/"IPOE_EMULATOR_NAS_NAME"/"
#define IPOE_EMULATOR_PID_PATH "/var/run/ipoe_emu.pid"
#define IPOE_EMULATOR_PING_PID_PATH "/var/run/ipoe_ping.pid"
#define IPOE_EMULATOR_CONF "/var/run/ipoe_emu.conf"
#define IPOE_EMULATOR_START "/usr/script/ipoe_simulate.sh"
#define IPOE_EMULATOR_DHCP_SH "/usr/script/udhcpc_ipoe_sim.sh"
#define IPOE_EMULATOR_ST_NONE "None"
#define IPOE_EMULATOR_ST_START "Start"
#define IPOE_EMULATOR_ST_STOP "Stop"
#define IPOE_EMULATOR_ST_COMPLETE "Complete"
#define IPOE_EMULATOR_ST_RUNNING "Running"
#define IPOE_EMULATOR_ATTR_DIGSTATE "DiagnosticsState"
#define IPOE_EMULATOR_ATTR_WANIFACE "WANInterface"
#define IPOE_EMULATOR_ATTR_USERMAC "UserMac"
#define IPOE_EMULATOR_ATTR_VENDORID "VendorClassID"
#define IPOE_EMULATOR_ATTR_RESULT "Result"
#define IPOE_EMULATOR_ATTR_LADDR "LocalAddress"
#define IPOE_EMULATOR_ATTR_DEFGW "DefaultGateway"
#define IPOE_EMULATOR_ATTR_PINGDSTIP "PingDestIPAddress"
#define IPOE_EMULATOR_ATTR_PINGNUM "PingNumberOfRepetitions"
#define IPOE_EMULATOR_ATTR_TIMEOUT "Timeout"
#define IPOE_EMULATOR_ATTR_SUCC_COUNT "SuccessCount"
#define IPOE_EMULATOR_ATTR_FAIL_COUNT "FailureCount"
#define IPOE_EMULATOR_ATTR_REP_AVRTIME "AverageResponseTime"
#define IPOE_EMULATOR_ATTR_REP_MINTIME "MinimumResponseTime"
#define IPOE_EMULATOR_ATTR_REP_MAXTIME "MaximumResponseTime"
#define IPOE_EMULATOR_ATTR_NASNAME "NASName"
#define IPOE_EMULATOR_RESULT_SUCC "Success"
#define IPOE_EMULATOR_RESULT_OTHER "Other"
#define IPOE_EMULATOR_RESULT_PINGFAIL "DestinationPingFail"
#define IPOE_EMULATOR_POLICY_TABLEID 234
#define IPOE_EMULATOR_PING_RESULT IPOE_EMULATOR_PATH"PingResult"

enum IPOE_EMULATOR_STOP_FLAGS
{
	IPOE_EMULATOR_STOP_SUCC = 0,
	IPOE_EMULATOR_STOP_OTHER,
};

int ipoeEmulator_init(void);
int ipoeEmulator_boot(mxml_node_t *top);
int ipoeEmulator_write(mxml_node_t *top, mxml_node_t *parant);
int ipoeEmulator_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int ipoeEmulator_stop(mxml_node_t *top, int method);
#endif
#if defined(TCSUPPORT_CT_PMINFORM)
#define PMINFORM_NODE_NAME "PMInform"
#define PMINFORM_NODE_ENTRY "Entry"
#define PMINFORM_ATTR_NTPST "NTPSync"
#define PMINFORM_ATTR_CWMPIP "CwmpWanIP"
#define PMINFORM_VAL_YES "Yes"
#define PMINFORM_VAL_NO "No"
int updateNtpState(mxml_node_t *top, int syncst);
int updateCwmpInfo(mxml_node_t *top, char *wanip, int state);
int PMInform_init(void);
int PMInform_boot(mxml_node_t *top);
#endif
#if defined(TCSUPPORT_CT_STB_TEST)
#define STBTEST_NODE_NAME "StbTest"
#define STBTEST_NODE_ENTRY "Entry"
#define STBTEST_WORK_STATUS "WorkStatus"
int StbTest_init(void);
int StbTest_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr);
int StbTest_boot(mxml_node_t *top);
#endif
#if defined(TCSUPPORT_CT_JOYME)
int OSGI_Upgrade_init(void);
int OSGI_Upgrade_boot(mxml_node_t *top);
int OSGI_Upgrade_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

#define UPGRADE_JVM "/usr/script/osgi_upg.sh"
#define JAVA_VM_STATE_FILE "/usr/osgi/jvmboot"
#define JAVA_FRAMEWORK_STATE_FILE "/usr/osgi/frameworkset"
#define CTGWLIBS_STATE_FILE "/usr/osgi/ctgwlibset"
#define MOBILE_STATE_FILE "/usr/osgi/mobileset"
#define CTGWLIBS_CHECK_FILE "/tmp/ctsgw_test"
#define MOBILE_CHECK_FILE "/tmp/mobile_test"
#define UPGRADE_CHECK_FILE "/tmp/upgst_phone"
#define TAR_CHECK_FILE "/tmp/tarerr"

#define INFORM_ERR_BINERR		 "1"
#define INFORM_ERR_DLDERR		 "2"
#define INFORM_ERR_SPACEFULL	 "3"
#define INFORM_ERR_SAMEVERSION	 "4"

#define INFORM_RESULT_OK 	"0"
#define INFORM_RESULT_FAIL "1"
#endif
#endif
