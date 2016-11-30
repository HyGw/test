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
#ifndef _WEB_H
#define _WEB_H

#include "cfg_manager.h"
#if defined(TCSUPPORT_PRODUCTIONLINE) || defined(TCSUPPORT_CT_PON_SC) || defined(TCSUPPORT_CUC)
#include "../../public/mtd/tc_partition.h"
#endif
#if defined(TCSUPPORT_CT_PON_SC)
#include "../opensource_lib/inc/libmtdpartition.h"
#endif

#define SEC_PER_DAY 86400 /*60*60*24  for ppp link-up time*/
#define SEC_PER_HOUR 3600 /*60*60         shnwind 2008.4.14*/
#define SEC_PER_MIN 60

#define MAX_BUF_SIZE	2048
#if defined(TCSUPPORT_CT_E8GUI)
#define BUF_SIZE_512	512
#endif
#define SYSTEMLOG_PATH	"/var/log/kmsg"
#ifndef PURE_BRIDGE
#define ETHERINFO_PATH	"/proc/tc3162/eth_stats"
#else
#define USBINFO_PATH	"/proc/tc3162/usb_link_state"
#endif
#define ADSLINFO_PATH	"/proc/tc3162/tsarm_stats"
#define ADSL_FWVER_PATH	"/proc/tc3162/adsl_fwver"
#define ADSL_STATE_PATH	"/proc/tc3162/adsl_stats"
#define WLANINFO_PATH	"/tmp/wlan_stats"
#if defined(TCSUPPORT_WLAN_AC)
#define WLAN11ACINFO_PATH	"/tmp/wlan11ac_stats"
#endif
#ifdef CWMP
#define ADSL_STATE_SHOW_PATH "/proc/tc3162/adsl_stas_show"
#define ADSL_STATE_LAST_PATH "/proc/tc3162/adsl_stas_last"
#define ADSL_STATE_TOTAL_PATH "/proc/tc3162/adsl_stas_total"
#define ADSL_STATE_CURDAYST_PATH "/proc/tc3162/adsl_stas_curdaySt"
#define ADSL_STATE_QUTHOURST_PATH "/proc/tc3162/adsl_stas_quthourSt"
#endif

/*xyzhu_091116:Get if stats from ifconfig,ra0-ra3/nas0-nas7*/
#define IFINFO_PATH		"/tmp/ifinfo_stats"
#define TIME_PATH	"/tmp/time"
#define SYSROUTE_PATH	"/tmp/route"
#define OAM_PING_PATH	"/proc/tc3162/oam_ping"
#define FWVER_PATH	"/etc/fwver.conf"
#if defined(TCSUPPORT_CT_E8GUI)
#define IFINFO_ETHCMD_PATH	"/tmp/ifinfo_ethcmd_stats"
#define DVID_PATH	"/etc/dvid.conf"
#define HDVER_PATH	"/etc/hdver.conf"
#define SDVER_PATH	"/etc/sdver.conf"
#define BATCODE_PATH	"/etc/batcode.conf"
#if defined(TCSUPPORT_CT_JOYME)
#define GATEWAY_PATH	"/usr/osgi/config/gateway.conf"
#else
#define GATEWAY_PATH	"/etc/gateway.conf"
#endif

#define MODEL_PATH	"/etc/model.conf"
#if defined(TCSUPPORT_CT_PON)
#define ETH_PORT_STATUS_FILE_PATH "/proc/tc3162/eth_port_status"
#endif
#endif
/*xyzhu_100114: get default device info*/
#define DEVDEF_PATH	"/etc/devInf.conf"
#if defined(TCSUPPORT_CT_PROLINE_SUPPORT) || defined(TCSUPPORT_CT_PON_CZ_GD)
#define COMPILE_TIME_PATH	"/etc/compile_time"
#endif

//#define DEVICEPARADYNAMIC_PATH	"/etc/deviceParaDynamic.conf"
#define DEVICEPARASTATIC_PATH	"/etc/deviceParaStatic.conf"
#define DEVICEPARADYNAMICNAME "devParaDynamic"
#define DEVICEPARASTATICNAME "devParaStatic"
#define SYSTEM		"System"
#define DEVICEINFO	"DeviceInfo"
#define DIAGNOSTIC	"Diagnostic"
#define WEBCURSET	"WebCurSet"
#define DYNDISP		"dynDisp"
#if defined(TCSUPPORT_CT_GUIACCESSLIMIT)
#define DYNCWMPATTR		"dynCwmpAttr"
#endif

#define TIME_CMD	"/bin/date > /tmp/time"
#define SYSROUTE_CMD	"/sbin/route -n > /tmp/route"
#define SKBMGR_CMD	"echo %s > /proc/net/skbmgr_hot_list_len"
#define REBOOT_CMD	"/userfs/bin/mtd -r write %s romfile"
#define PING_CMD	"/bin/ping -c 1 -W 2 %s > %s"
#if defined(TCSUPPORT_CT_E8GUI)
#if defined(TCSUPPORT_CT_PON_GD)
#define REG_PING_CMD_TIMES	"/bin/ping -t -c %s -W 2 %s > %s"
#define REG_PING6_CMD_TIMES	"/bin/ping6 -t -I %s -c %s %s > %s"
#endif
#define PING_CMD_TIMES	"/bin/ping -c %s -W 2 %s > %s"
#define PING6_CMD_TIMES	"/bin/ping6 -I %s -c %s %s > %s"
#define ROUTE6_ADD_CMD  "ip -6 route add %s via %s dev %s"
#define ROUTE6_DEL_CMD  "ip -6 route del %s"
#define TRACE_CMD	"traceroute -n -m 10 -a -w 2 -i %s > %s"
#define TRACE6_CMD	"/userfs/bin/traceroute6 -n -m 10 -w 2  -I --icmp -i %s  %s > %s"
#define ADDROUTE_CMD	"route add -host %s dev %s"
#define DELROUTE_CMD	"route del %s"
#endif
/*krammer add*/
#define ISPCONFIG_PATH "/etc/isp%d.conf"
/*krammer add*/

#define ETHLINK_PATH	"/proc/tc3162/eth_link_st"
#define ADSLSTATE_PATH	"/proc/tc3162/adsl_stats"
#define YAHOO_DNS	"www.yahoo.com"
#define TMP_PING_DNS_PATH "/tmp/pingDns"
#define TMP_PING_YAH_PATH "/tmp/pingYah"
#define TMP_PING_OTHER_PATH "tmp/pingOth"  /*rodney_20090506 added*/
#define TMP_PING_TS_PATH  "/tmp/pingTs"  /*rodney_20090507 added*/
#define TMP_DNS_DIAG_PATH "/tmp/diagdns.conf"

#if defined(TCSUPPORT_CT_E8GUI)
#define TMP_TRACE_PATH "tmp/traceOth"
#endif
#define ETHINFO_RXFRAM	"rxFrames"
#define ETHINFO_TXFRAM	"txFrames"
#ifndef PURE_BRIDGE
/*krammer add*/
#define ETHINFO_IP "ip"
#define ETHINFO_MAC "mac"
#else
#define USBINFO_IP "ip"
#define USBINFO_MAC "mac"
#endif
#define PPPoE_LLC "PPPoE LLC"
#define PPPoE_VC_Mux "PPPoE VC-Mux"
#define PPPoA_LLC "PPPoA LLC"
#define PPPoA_VC_Mux "PPPoA VC-Mux"
/*krammer add*/
//added by  renyu_20091223
#define CONNECT_TYPE_MANUALLY "Connect_Manually"
#define PPP_START_SH "/usr/script/ppp_start.sh"

#if defined (CWMP) && defined(TR111)
#define DEVICE_INFO_FILE         "/etc/devices.conf"
#define COUNT_LENGTH 8
#define OUI_LENGTH 6
#define SN_LENGTH 64
#define PCLASS_LENGTH 64
#endif

#define ATM_OAM_PING_SEGM	0
#define ATM_OAM_PING_E2E	1

/*shnwind add. for html_post_parser()*/
#define GET_BOUNDARY        0x01
#define GET_DESCRIBE    0x02
#define GET_CRLF            0x04
#define GET_DATA            0x08
#define GET_VAL             0x10

#define NOT_CONNECTED	"0"
#define CONNECTED	"1"
#define CONNECT_DISP_BUTTON "1"
#define CONNECT    1
#define DISCONNECT 2

#define RENEW_IP	1
#define RELEASE_IP	2

#define ROUTE_ATTR_METRIC "metric"
#define ROUTE_ATTR_USE "Use"

#define TEST_OK	"PASS"
#define TEST_SKIP "Skipped"
#define TEST_FAIL	"Fail"
#if defined(TCSUPPORT_CT_E8GUI)
#define RECV0ICMPPKT	"0 packets received"
#define RECVICMPPKT	"packets received"
#endif
#define RECV1ICMPPKT	"1 packets received"


#define DEF_GW_FLAG	"UG"
#define TMP_IF_PATH	"/tmp/if.conf"
#define TMP_CHECK_IF "tmp/iface.conf"
#define DNS_INFO_PATH	"/etc/resolv.conf"
#define PPP_DNS_INFO_PATH	"/etc/ppp/resolv.conf"
#define DNS_ATTR	"nameserver"

#define UDHCPC_PID_PATH	"/var/run/udhcpc-nas%s.pid"
/*We should get wireless state by use  /userfs/bin/iwpriv ra0 stat. shnwind modify*/
#define GET_WLAN_INFO_CMD	"/userfs/bin/iwpriv ra0 stat"
#if defined(TCSUPPORT_WLAN_AC)
#define GET_WLAN_AC_INFO_CMD	"/userfs/bin/iwpriv rai0 stat"
#endif
/*krammer add 2008 11 4*/
#define ISEXIST "isExist"
/*krammer add*/

/* add web Custom node by TonyDu 20111027 */
#define WEBCURT     "WebCustom"
#define SUPPORTED_YES   "Yes"
/* end web Custom node */
/*System cfg node*/
#define BOOT2CURSET	1
#define BOOT2DEFSET	2
#define WARM_REBOOT	3
#define BOOT2DEFSETTR69	4
#if defined(TCSUPPORT_CT_LONG_RESETBTN)
#define	LONGRESET_BOOT2DEFSET	5
#endif
#if defined(TCSUPPORT_CT_PON_SK)
#define BOOT2PUREFACTORY 7
#endif
#if defined(TCSUPPORT_CT_PON_SC)
#define	BOOT2DEFSETITMS 6
#endif
#if defined(TCSUPPORT_CUC)
#define	CUC_LONGRESET_BOOT2DEFSET 7
#endif

#define BAKROM_ON	1
#define UG_ROMFILE	1
#if 0
#define UG_KERNEL	2
#define UG_FS	3
#endif
#define UG_TCLINUX	4
#if defined(TCSUPPORT_CT_UPLOAD_ALLINONE)
#define UG_TCLINUX_ALLINONE	5
#define CKSUM_LENGTH 4
#ifdef TCSUPPORT_BOOTROM_LARGE_SIZE
#define TCBOOT_SIZE 0x20000 	//128K
#else
#define TCBOOT_SIZE 0x10000
#endif
#define ROMFILE_SIZE 0x10000
#define MAC_IN_TCBOOT 0x0000ff48
#if defined(TCSUPPORT_CT_DUAL_IMAGE)
#if defined(TCSUPPORT_CT_PON) && defined(TCSUPPORT_RESERVEAREA_EXTEND)
#define TCLINUX_SIZE 0x7B0000
#else
#define TCLINUX_SIZE 0x780000
#endif
#else
#if defined(TCSUPPORT_CUC_DUAL_IMAGE)
#define TCLINUX_SIZE 0x700000
#endif
#endif
#endif

/*shnwind add for firmware upgrade.2009.4.3*/
#define NO_OPERATION  	0
#define NO_HEADER	1
#define HTML_HEADER	2
#if defined(TCSUPPORT_CT_PHONEAPP)
#define PHONE_NO_HEADER	3
#endif
#define HTMLFINDEND_TEMP_LEN 4096
#define STRING_BUF_SIZE	256
#define MULTI_TO_NORMAL_TEMP "/tmp/parse_temp"
/*end System cfg node*/

/*pork 20090309 added*/
#define SYS_INFO_NODE_NAME "SysInfo"
#define SYSTEMSTARTUPTIME "sysStartTime"
#define INTERLEAVEMODE "0"
#define FASTMODE "1"
#define ADSLNODE "Adsl"
#define SUB_NODE_NAME "Entry"
#define ADSL_ATTR_MODE	"MODULATIONTYPE"
#define ADSL_ATTR_TYPE	"ANNEXTYPEA"
#define MODULATIONTYPE_T1_413 "T1.413"
#define MODULATIONTYPE_G_DMT "G.DMT"
#define MODULATIONTYPE_G_LITE "G.lite"
#define MODULATIONTYPE_ADSL2 "ADSL2"
#define MODULATIONTYPE_ADSL2PLUS "ADSL2+"
#define MODULATIONTYPE_AUTOSYNCUP "Auto Sync-Up"
#define MTENSTANDARD_ADSL2 "G.dmt.bis"
#define MTENSTANDARD_ADSL2PLUS "G.dmt.bisplus"
#define ANNEXTYPEA_L	"Annex L"
#define ANNEXTYPEA_M	"Annex M"
#define INIT			"0"
//added by xyzhu_nj_20091209
#define GET_WLAN_MAC_TABLE	"/userfs/bin/iwpriv ra%d get_mac_table > %s"
#define WLAN_MAC_TABLE_PATH		"/tmp/mac_table_info"
#define ARP_TABLE_PATH			"/tmp/arp_tab"
#define MAX_MAC_ENTRY	32
#define WIFIMACTABLE	"wifiMacTab"
#define WIFIMACTABLE_NUM	"NUM"
#define WIFIMACTABLE_IP		"IP"
#define WIFIMACTABLE_MAC	"MAC"
#if defined(TCSUPPORT_CT_PHONEAPP)
#define WIFIMACTABLE_RSSIA	"RSSIA"
#define WIFIMACTABLE_RSSIB	"RSSIB"
#define WIFIMACTABLE_RSSIC	"RSSIC"
#endif
#define COMMON			"Common"
#define ISCHANGED		"0"
#define	MACTABLEDATA	"1"

//add by brian
#ifdef CWMP
#define DSL_DIAGNOSTIC_PATH "/proc/tc3162/adsl_cwmp_diagnostic"
#define MAX_SPLIT_LEN 		512
#define MAX_SPLIT_OFFSET 	9// mod 512
#define CWMPDSLDIAGNOSTIC	"CDSLDiagnostic"
#define DIAGNOSTICSUB		"Common"
//because the statistic data is very large,so we use more proc file to transfer from kernal space to user space 
#define OTHER_PROFILE_INDEX 		0
#define HLINpsds_PROFILE_INDEX_1 	1
#define HLINpsds_PROFILE_INDEX_2 	2
#define QLINpsds_PROFILE_INDEX 		3
#define SNRpsds_PROFILE_INDEX 		4
#define BITSpsds_PROFILE_INDEX 		5
#define GAINSpsds_PROFILE_INDEX 	6

#if defined(TCSUPPORT_CT_WAN_PTM)
#define HLOGpsds_PROFILE_INDEX 	7
#define HLOGpsus_PROFILE_INDEX 	8
#define HLINpsus_PROFILE_INDEX_1 	9
#define HLINpsus_PROFILE_INDEX_2 	10
#define QLNpsus_PROFILE_INDEX 	11
#define SNRpsus_PROFILE_INDEX 	12
#define SATNds_PROFILE_INDEX 	13
#define SATNus_PROFILE_INDEX 	14
#define LATNds_PROFILE_INDEX 	15
#define LATNus_PROFILE_INDEX 	16

#define VDSLINFO_B0_PATH "/proc/tc3162/ptm_b0_stats"
#define VDSLINFO_B1_PATH "/proc/tc3162/ptm_b1_stats"

#define VDSL_INTERFACE_CFG_PATH "/proc/tc3162/vdsl_interface_config"
#endif


#define ATM_F5LOOPBACK_DIAGNOSTIC_PATH "/proc/tc3162/atm_f5_loopback_diagnostic"
#define ATM_F5LOOPBACK_DIAGNOSTIC_RESET_PATH "/proc/tc3162/atm_f5_loopback_reset_diagnostic"
#define CWMPATMDIAGNOSTIC	"CATMDiagnostic"
#endif

#define WEBPWDLEN 16
#define WEBACCOUNTLEN 16

#ifdef TCSUPPORT_PRODUCTIONLINE
#define PRODUCTLINECWMPFILEPATH "/tmp/productlinecwmpfile"
#define PRODUCTLINEPARAMAGIC 0x12344321

#if defined(TCSUPPORT_RESERVEAREA_EXTEND) || defined(TCSUPPORT_CT_WAN_PTM) || !(!defined(TCSUPPORT_CT_E8B_ADSL) || !defined(TCSUPPORT_CPU_MT7505))
#if defined(TCSUPPORT_RESERVEAREA_EXTEND) || defined(TCSUPPORT_C7) || !(!defined(TCSUPPORT_CT_E8B_ADSL) || !defined(TCSUPPORT_CPU_MT7505))
#define IMG_BOOT_FLAG_FILE 	"/tmp/image_boot_flag"

enum PL_CP_Flags {
	PL_CP_PRODUCTCLASS=0,
	PL_CP_MANUFACUREROUI,
	PL_CP_SERIALNUM,
	PL_CP_SSID,
	PL_CP_WPAKEY,
	PL_CP_WEBPWD,
	PL_CP_PPPUSRNAME,
	PL_CP_PPPPWD,
	PL_CP_CFEUSRNAME,
	PL_CP_CFEPWD,
	PL_CP_XPONSN,
	PL_CP_XPONPWD,
	PL_CP_MACADDR,
	PL_CP_TELNET,
	PL_FON_KEYWORD,
	PL_FON_MAC,
	PL_CP_BARCODE,
	PL_CP_XPONMODE,
	PL_CP_SSID2nd,
	PL_CP_WPAKEY2nd,
	PL_CP_WEB_ACCOUNT,
	PL_CP_HW_VER,
	PL_CP_ROMFILE_SELECT,
	PL_CP_END
};

#define FLAG_TELNET_DISABLE "DIS"
#define FLAG_TELNET_ENABLE "ENB"

#define PL_CP_PRODUCTCLASS_FLAG (1<<PL_CP_PRODUCTCLASS)
#define PL_CP_MANUFACUREROUI_FLAG (1<<PL_CP_MANUFACUREROUI)
#define PL_CP_SERIALNUM_FLAG (1<<PL_CP_SERIALNUM)
#define PL_CP_SSID_FLAG (1<<PL_CP_SSID)
#define PL_CP_WPAKEY_FLAG (1<<PL_CP_WPAKEY)
#define PL_CP_WEBPWD_FLAG (1<<PL_CP_WEBPWD)
#define PL_CP_PPPUSRNAME_FLAG (1<<PL_CP_PPPUSRNAME)
#define PL_CP_PPPPWD_FLAG (1<<PL_CP_PPPPWD)
#define PL_CP_CFEUSRNAME_FLAG (1<<PL_CP_CFEUSRNAME)
#define PL_CP_CFEPWD_FLAG (1<<PL_CP_CFEPWD)
#define PL_CP_XPONSN_FLAG (1<<PL_CP_XPONSN)
#define PL_CP_XPONPWD_FLAG (1<<PL_CP_XPONPWD)
#define PL_CP_MACADDR_FLAG (1<<PL_CP_MACADDR)
#define PL_FON_KEYWORD_FLAG (1<<PL_FON_KEYWORD)
#define PL_FON_MAC_FLAG (1<<PL_FON_MAC)
#define PL_CP_BARCODE_FLAG (1<<PL_CP_BARCODE)
#define PL_CP_XPONMODE_FLAG (1<<PL_CP_XPONMODE)
#define PL_CP_TELNET_FLAG (1<<PL_CP_TELNET)
#define PL_CP_SSID2nd_FLAG (1<<PL_CP_SSID2nd)
#define PL_CP_WPAKEY2nd_FLAG (1<<PL_CP_WPAKEY2nd)
#define PL_CP_WEB_ACCOUNT_FLAG (1<<PL_CP_WEB_ACCOUNT)
#define PL_CP_HW_VER_FLAG (1<<PL_CP_HW_VER)
#define PL_CP_ROMFILE_SELECT_FLAG	(1<<PL_CP_ROMFILE_SELECT)

#define PRDDUCTCLASSLEN 64
#define MANUFACUREROUILEN 64
#define SERIALNUMLEN 128
#define SSIDLEN 32
#define WPAKEYLEN 64
#define PPPUSRNAMELEN 64
#define PPPPWDLEN 64
#define CFEUSRNAMELEN 64
#define CFEPWDLEN 64
#define XPONSNLEN 32
#define XPONPWDLEN 32
#define MACADDRLEN 32
#define XPONMODELEN	8
#define FONKEYWORDLEN 65
#define FONMACLEN 18
#define BARCODELEN	32
#define TELNETLEN 4
#define HWVERLEN 64
#define ROMFILESELECTLEN 64

typedef struct _proline_Para{
	int flag;
	int magic;
	char telnet[TELNETLEN];
	char barcode[BARCODELEN];
	char productclass[PRDDUCTCLASSLEN];
	char manufacturerOUI[MANUFACUREROUILEN];
	char serialnum[SERIALNUMLEN];
	char ssid[SSIDLEN];
	char wpakey[WPAKEYLEN];
	char webpwd[WEBPWDLEN];
	char pppusrname[PPPUSRNAMELEN];
	char ppppwd[PPPPWDLEN];
	char cfeusrname[CFEUSRNAMELEN];
	char cfepwd[CFEPWDLEN];
	char xponsn[XPONSNLEN];
	char xponpwd[XPONPWDLEN];
	char macaddr[MACADDRLEN];
	char xponmode[XPONMODELEN];
	char fonKeyword[FONKEYWORDLEN];
	char fonMac[FONMACLEN];
	char ssid2nd[SSIDLEN];
	char wpakey2nd[WPAKEYLEN];
	char webAcct[WEBACCOUNTLEN];
	char hwver[HWVERLEN];
	char romfileselect[ROMFILESELECTLEN];
	char reserve[1024];
} proline_Para;

#define DEFAULT_SSID "ChinaNet-AP"
#define DEFAULT_WPAKEY "12345678"
#define DEFAULT_WEBUSRNAME "admin"
#define DEFAULT_WEBPWD "1234"
#define DEFAULT_PPPUSRNAME "ppp"
#define DEFAULT_PPPPWD "ppp"
#define DEFAULT_CFEUSRNAME "telecomadmin"
#define DEFAULT_CFEPWD "nE7jA%5m"
#define DEFAULT_XPONSN "FHTT12345678"
#define DEFAULT_XPONPWD "00000001"
#define DEFAULT_MACADDR "00AABB112233"
#define DEFAULT_XPONMODE "GPON"
#else
#define PL_CP_PRODUCTCLASS_FLAG 1<<0
#define PL_CP_MANUFACUREROUI_FLAG 1<<1
#define PL_CP_SERIALNUM_FLAG 1<<2
#define PL_CP_BARCODE_FLAG	(1<<8)

#define PRDDUCTCLASSLEN 64
#define MANUFACUREROUILEN 64
#define SERIALNUMLEN 128
#define BARCODELEN	32
#define RESERVELEN (PROLINE_CWMPPARA_RA_SIZE-PRDDUCTCLASSLEN-MANUFACUREROUILEN-SERIALNUMLEN-BARCODELEN-2*sizeof(int))

typedef struct _proline_Para{
	int flag;
	char productclass[PRDDUCTCLASSLEN];
	char manufacturerOUI[MANUFACUREROUILEN];
	char serialnum[SERIALNUMLEN];
	char barcode[BARCODELEN];
	char reserve[RESERVELEN];
	int magic;
} proline_Para;

#endif
#else
#define PL_CP_PRODUCTCLASS_FLAG 1<<0
#define PL_CP_MANUFACUREROUI_FLAG 1<<1
#define PL_CP_SERIALNUM_FLAG 1<<2

#define PRDDUCTCLASSLEN 64
#define MANUFACUREROUILEN 64
#define SERIALNUMLEN 128

#if !defined(TCSUPPORT_PRODUCTIONLINE_DATE) 
#define PROGDATELEN 0
#define MFTDATELEN 0
#endif

#if !defined(TCSUPPORT_PRODUCTIONLINE_CONTENT) 
#define PRODUCTCONTENTLEN 0
#endif

#if defined(TCSUPPORT_WAN_GPON) || defined(TCSUPPORT_WAN_EPON)
#define PL_PON_MACNUM_FLAG  (1<<8)
#define PL_PON_USERADMIN_PASSWD_FLAG (1<<9)
#if defined(TCSUPPORT_WAN_GPON)
#define PL_GPON_SN_FLAG  (1<<10)
#define PL_GPON_PASSWD_FLAG (1<<11)
#endif
#endif

#if defined(TCSUPPORT_FON) || defined(TCSUPPORT_FON_V2)
#define FONKEYWORDLEN 65
#define FONMACLEN 18
#else
#define FONKEYWORDLEN 0
#define FONMACLEN 0
#endif

#if defined(TCSUPPORT_WAN_GPON) || defined(TCSUPPORT_WAN_EPON)
#define PONMACNUMLEN  3
#define PONUSERADMINPASSWDLEN 32
#if defined(TCSUPPORT_WAN_GPON)
#define GPONSNLEN  13
#define GPONPASSWDLEN 9
#else
#define GPONSNLEN  0
#define GPONPASSWDLEN 0
#endif
#else
#define PONMACNUMLEN  0
#define PONUSERADMINPASSWDLEN 0
#define GPONSNLEN  0
#define GPONPASSWDLEN 0
#endif

#define RESERVELEN (PROLINE_CWMPPARA_RA_SIZE     \
                    - 2*sizeof(int)              \
                    - PRDDUCTCLASSLEN            \
                    - MANUFACUREROUILEN          \
                    - SERIALNUMLEN               \
                    - PROGDATELEN                \
                    - MFTDATELEN                 \
                    - PRODUCTCONTENTLEN          \
                    - FONKEYWORDLEN              \
                    - FONMACLEN                  \
                    - PONMACNUMLEN               \
                    - PONUSERADMINPASSWDLEN      \
                    - GPONSNLEN                  \
                    - GPONPASSWDLEN              \
                    - WEBPWDLEN                  \
                    - WEBACCOUNTLEN)
typedef struct _proline_Para{
	int flag;
	int magic;	
	char productclass[PRDDUCTCLASSLEN];
	char manufacturerOUI[MANUFACUREROUILEN];
	char serialnum[SERIALNUMLEN];

/**************************************************
 * Attention: 
 * remove compile for reserved area when upgrade on using device 
 *************************************************/
#if defined(TCSUPPORT_FON) || defined(TCSUPPORT_FON_V2)
	char fonKeyword[FONKEYWORDLEN];
	char fonMac[FONMACLEN];
#endif
#if defined(TCSUPPORT_WAN_GPON) || defined(TCSUPPORT_WAN_EPON)
    char ponMacNum[PONMACNUMLEN];
	char ponUseradminPasswd[PONUSERADMINPASSWDLEN];
#if defined(TCSUPPORT_WAN_GPON)
	char gponSn[GPONSNLEN];
	char gponPasswd[GPONPASSWDLEN];
#endif
#endif
    char webpwd[WEBPWDLEN];
    char webAcct[WEBACCOUNTLEN];
	char reserve[RESERVELEN];
} proline_Para;
#endif
#endif

#if defined(TCSUPPORT_CT_DEFAULTPARA_STORE) || defined(TCSUPPORT_KEYPARA_STORE)
#define OP_AT_END (1<<0)	//flag to show at the end of key parameter list
#define OP_NODE  (1<<1) 	//operate node 
//follow two defines are according to OP_NODE
#define OP_NODE_LEVEL_ALL  (1<<2)//all node information include sub node 
#define OP_NODE_LEVEL_SINGLE  (1<<3)//single node information
#define OP_ATTRIBUTE  (1<<4)//operate attribute 
#if defined(TCSUPPORT_CT_LONG_RESETBTN)
#define OP_LONG_RESETBAK (1<<5)
#else
#define OP_LONG_RESETBAK OP_ATTRIBUTE
#endif
#define OP_CWMP_ONLY  (1<<7) // key will be reserved via TR69 factory default.
#if defined(TCSUPPORT_CT_PON_JS)
#define OP_JSCWMP_ONLY OP_CWMP_ONLY
#define OP_JSAccount_ONLY OP_CWMP_ONLY
#else
#define OP_JSCWMP_ONLY OP_ATTRIBUTE
#define OP_JSAccount_ONLY OP_NODE
#endif
#define OP_ATTRIBUTE_IGNORE_LIST (1<<8) /* only support for OP_NODE_LEVEL_SINGLE.*/

typedef struct _ignore_list_
{
	char **attrlist;
	int nodelen;
}ignore_list, *pignore_list_ptr;

//typedef struct _Param_Name
//{
//	char *node;		/*node name-----for example:Cwmp_Entry*/
//	char *attr;		/*attribute name----for example:acsUrl*/	
//}Param_Name;	/*name of key parameters*/

typedef struct _Key_Parameters_Info
{
	int flag;					  /*flag to judge if node operation or attribute operation*/	
	char *node;		/*node name-----for example:Cwmp_Entry*/
	char *attr;		/*attribute name----for example:acsUrl*/	
	int	(*funp)(struct _Key_Parameters_Info* parainfo,mxml_node_t* oldtree,mxml_node_t* newtree);     /* handle function poiter*/
}Key_Parameters_Info,*Key_Parameters_Info_Ptr;
#if (defined(TCSUPPORT_CT_PON) || defined(TCSUPPORT_CT_E8B_ADSL)) || defined(TCSUPPORT_CUC)
typedef struct _Key_Parameters_Info_Enhance
{
	int flag;					  /*flag to judge if node operation or attribute operation*/	
	char *node;		/*node name-----for example:Cwmp_Entry*/
	char *nodenew;	/*node name-----for example:Cwmp_Entry*/
	char *attr;		/*attribute name----for example:acsUrl*/	
	int	(*funp)(struct _Key_Parameters_Info_Enhance* parainfo,mxml_node_t* oldtree,mxml_node_t* newtree);     /* handle function poiter*/
}Key_Parameters_Info_Enhance,*Key_Parameters_Info_Enhance_Ptr;
#endif
#endif

#if 0
typedef enum ifaceType
{
		ifra0=0,
		ifra1,
		ifra2,
		ifra3,
		ifnas0,
		ifnas1,
		ifnas2,
		ifnas3,
		ifnas4,
		ifnas5,
		ifnas6,
		ifnas7,
		ifeth0,
}ifaceType;
#endif
/*
#define PROC_TSARM_PKTS_CLEAR_LOCATION "/proc/tc3162/tsarm_pktsclear"
#define PROC_ETHER_PKTS_CLEAR_LOCATION "/proc/tc3162/eth_pktsclear"
*/
//yzwang_20091208
#define AUTOPVC_STATE_STOP 0
#define AUTOPVC_STATE_PROBE_PVC 1
#define AUTOPVC_STATE_PROBE_VC_POOL 2
#define AUTOPVC_STATE_FINISH 3

#ifdef TCSUPPORT_SYSLOG
//yzwang_20100719 for syslog
#define SYSLOG_VALUE_LEN	8
#define SYSLOG_CONF_PATH	"/var/log/log.conf"
#define SYSLOG_NODE_NAME	"SysLog"
#if defined(TCSUPPORT_C1_CUC)
#define ALARMLOG                          "AlarmLog"
#define COMMON_NODE_NAME       "Common"
#define ENTRY_NODE_NAME           "Entry"
#define ALARMLOG_INSTANCE_NUM	100
#endif
#endif

#ifdef CWMP
#define SWITCHPARA_NODE_NAME	"SwitchPara"
#define SWITCHPARA_ADV_NODE_NAME "Common"
#define SWITCHPARA_SCRIPT "/tmp/switchpara.sh"
#define SWITCHPARA_CMD "/userfs/bin/ethphxcmd eth0 media-type %s port %d"
#define SWITCHPARA_CMD_ENABLE "/userfs/bin/ethphxcmd eth0 lanchip %s port %d"
#ifdef TCSUPPORT_CT_2PWIFI
#define SWITCHPARA_MAX_ENTRY 2
#else
#define SWITCHPARA_MAX_ENTRY 4
#endif
#endif
/*function prototype*/
int
sysInfo_write(mxml_node_t *top,mxml_node_t *parant);

int
sysInfo_init(void);

int sysInfo_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
/*pork 20090309 added over*/

int
system_init(void);
int
system_init_boot(mxml_node_t *top);
int
system_read(mxml_node_t *node, char nodeName[][MAX_NODE_NAME], char *attr);
int
system_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

#if defined(TCSUPPORT_CT_PON_C9)
int
reginfo_init(void);
int
reginfo_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#endif

#if defined(TCSUPPORT_C7)
int
settinginfo_init(void);
int
settinginfo_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#endif

int
diagnostic_init(void);
int
diagnostic_read(mxml_node_t *node, char nodeName[][MAX_NODE_NAME], char *attr);
int
diagnostic_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

int
deviceInfo_init(void);
int
deviceInfo_read(mxml_node_t *node, char nodeName[][MAX_NODE_NAME], char *attr);
int
deviceInfo_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

int
dynDisp_init(void);
int
dynDisp_read(mxml_node_t *node, char nodeName[][MAX_NODE_NAME], char *attr);

#if defined(TCSUPPORT_CT_GUIACCESSLIMIT)
int
dynCwmpAttr_init(void);
int
dynCwmpAttr_read(mxml_node_t *node, char nodeName[][MAX_NODE_NAME], char *attr);
#endif

int
webCurSet_init(void);
int
webCurSet_read(mxml_node_t *node, char nodeName[][MAX_NODE_NAME], char *attr);
#if 0
int
sysRoute_read(mxml_node_t *top, char *name);

int
systemLog_read(mxml_node_t *top, char *name);
#endif

#ifdef TCSUPPORT_WLAN
#if defined(TCSUPPORT_WLAN_MAXSTANUM_GUI)
#if (defined(RT3390) && defined(VENDOR_FEATURE3_SUPPORT)) || (defined(RT5392) && defined(BB_SOC))
#define MAX_STA_NUM "7"
#else
#define MAX_STA_NUM "31"
#endif
#endif
#endif

int
info_init(void);
//added by xyzhu_nj_20091209
int
wifiMacTab_init(void);

int 
getIpFromArp(char *macAttr, char *ip);

int
info_read(mxml_node_t *node, char nodeName[][MAX_NODE_NAME], char *attr);

#ifndef PURE_BRIDGE
int
etherInfo_read(mxml_node_t *node);
#else
int
usbInfo_read(mxml_node_t *node);
#endif
int
adslInfo_read(mxml_node_t *top,mxml_node_t *node);
#if defined(TCSUPPORT_CT_PON)
int 
xponInfo_read(mxml_node_t *top, mxml_node_t *node);
#endif
#if defined(TCSUPPORT_CT_WAN_PTM)
int 
ptmInfo_read(mxml_node_t *top, mxml_node_t *node);
#endif


#ifdef CWMP
int
adslStatsShow_read(mxml_node_t *top,mxml_node_t *node);
int
adslStatsLast_read(mxml_node_t *top,mxml_node_t *node);
int
adslStatsTotal_read(mxml_node_t *top,mxml_node_t *node);
int
adslStatsCurdaySt_read(mxml_node_t *top,mxml_node_t *node);
int
adslStatsQutHourSt_read(mxml_node_t *top,mxml_node_t *node);
int
checkCwmpUploadedRomfile(mxml_node_t *top,mxml_node_t *node);
#endif
int
wlanInfo_read(mxml_node_t *node);
#if defined(TCSUPPORT_WLAN_AC)
int
wlan11acInfo_read(mxml_node_t *node);
#endif
int
autoPVCInfo_read(mxml_node_t *node);//yzwang_20091208
int
checkPpp(int cur_pvc);
//added by xyzhu_nj_20091209
int 
ifInfo_read(mxml_node_t *node, char *type);

#if defined(TCSUPPORT_CT_E8GUI)
void pingDiag(char *convertIP, char *buf_route, char *buf_times, int wanIPmode);
void ping6Diag(mxml_node_t *top, char *convertIP, char *buf_route, char *buf_times);

int 
ifInfo_ethcmd_read(mxml_node_t *node);
int get_dhcpLease_status(const char *strIP, char *pTime);
void trimIP(char* str);
#endif
int devDefInf_read(mxml_node_t *node);
int
wifiMacTab_read(mxml_node_t *node, char nodeName[][MAX_NODE_NAME], char *attr);
#if defined (CWMP) && defined(TR111)
int
dhcpClientLimit_init(void);

int
dhcpClient_init(void);
int
dhcpClient_read(mxml_node_t *node, char nodeName[][MAX_NODE_NAME], char *attr);
unsigned long
get_dhcpClient_count_vlaue(char *str, char *keyword, int length, int base);
int
get_dhcpClient_Info_vlaue(char *str, char *keyword, int length, char *val);
#endif
#ifdef CWMP
//add by brian for dsl diagnostic
int 
cwmp_dsl_diagnostic_init(void);
int
cwmp_dsl_diagnostic_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr);
void
splitValue(char *strbuf,char*attr,mxml_node_t *curNode);
//add by brian for atm f5 loopback diagnostic
int
cwmp_f5loopback_diagnostic_init(void);
int
cwmp_f5loopback_diagnostic_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr);
int
IPInterface_init(void);
#endif
#if defined(TCSUPPORT_CT_DEFAULTPARA_STORE) || defined(TCSUPPORT_KEYPARA_STORE)
int operateNodePara(Key_Parameters_Info_Ptr parainfo,mxml_node_t* oldtree,mxml_node_t* newtree);
int operateAttributePara(Key_Parameters_Info_Ptr parainfo,mxml_node_t* oldtree,mxml_node_t* newtree);
mxml_node_t *keypara_load_romfile(char *pathname);
#if (defined(TCSUPPORT_CT_PON) || defined(TCSUPPORT_CT_E8B_ADSL)) || defined(TCSUPPORT_CUC)
int operateNodeParaEnhance(Key_Parameters_Info_Enhance_Ptr parainfo,mxml_node_t* oldtree,mxml_node_t* newtree);
#endif
#endif
#if defined(TCSUPPORT_CT_BACKUPROMFILEENCHANCEMENT)
int xponRomfileMode();
#endif
#if defined(TCSUPPORT_KEYPARA_STORE)
int keyParaRestore(int mode);
#endif
int getDefaultRomfile(char *path);
#if defined(TCSUPPORT_CT_DEFAULTPARA_STORE)
int defaultParaRestore();
int defaultParaRomfileUpdate();
#endif

#ifdef TCSUPPORT_BACKUPROMFILE
int sys_update_backup_romfile(char *romfilePath);
#endif

#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7592)
#define MAX_LEN_OF_MAC_TABLE 32
#define MAC_ADDR_LENGTH		6

typedef union  _MACHTTRANSMIT_SETTING {
	struct	{
	unsigned short   	MCS:7;                 // MCS
	unsigned short		BW:1;	//channel bandwidth 20MHz or 40 MHz
	unsigned short		ShortGI:1;
	unsigned short		STBC:2;	//SPACE 
	unsigned short		rsv:3;	 
	unsigned short		MODE:2;	// Use definition MODE_xxx.  
	}	field;
	unsigned short		word;
 } MACHTTRANSMIT_SETTING, *PMACHTTRANSMIT_SETTING;

#ifdef WLAN_MULTIDRIVER
//typedef unsigned char UCHAR;
//typedef signed char CHAR;
//typedef unsigned int UINT32;
//typedef signed short SHORT;

typedef struct _RT_802_11_MAC_ENTRY {
	unsigned char ApIdx;
	unsigned char Addr[MAC_ADDR_LENGTH];
	unsigned char Aid;
	unsigned char Psm;		/* 0:PWR_ACTIVE, 1:PWR_SAVE */
	unsigned char MimoPs;		/* 0:MMPS_STATIC, 1:MMPS_DYNAMIC, 3:MMPS_Enabled */
	signed char AvgRssi0;
	signed char AvgRssi1;
	signed char AvgRssi2;
	unsigned int ConnectedTime;
	MACHTTRANSMIT_SETTING TxRate;
//#ifdef RTMP_RBUS_SUPPORT
	unsigned int		LastRxRate;
	signed short 		StreamSnr[3];				/* BF SNR from RXWI. Units=0.25 dB. 22 dB offset removed */
	signed short 		SoundingRespSnr[3];			/* SNR from Sounding Response. Units=0.25 dB. 22 dB offset removed */
/*	SHORT		TxPER;	*/					/* TX PER over the last second. Percent */
/*	SHORT		reserved;*/
//#endif /* RTMP_RBUS_SUPPORT */
} RT_802_11_MAC_ENTRY, *PRT_802_11_MAC_ENTRY;

#else// WLAN_MULTIDRIVER
typedef struct _RT_802_11_MAC_ENTRY {
#if defined(RT5392) || defined(MT7601E)|| defined(MT7592)
		unsigned char       ApIdx;
#endif
     	unsigned char       Addr[MAC_ADDR_LENGTH];
     	unsigned char       Aid;
     	unsigned char       Psm;     //used for ssidindex
     	unsigned char		MimoPs;  // 0:MMPS_STATIC, 1:MMPS_DYNAMIC, 3:MMPS_Enabled
   	signed char		AvgRssi0;
	signed char		AvgRssi1;
	signed char		AvgRssi2;
	unsigned int		ConnectedTime;
    MACHTTRANSMIT_SETTING	TxRate;
#ifdef MT7592
	unsigned long LastRxRate;
	signed short StreamSnr[3];			/* BF SNR from RXWI. Units=0.25 dB. 22 dB offset removed */
	signed short SoundingRespSnr[3];			/* SNR from Sounding Response. Units=0.25 dB. 22 dB offset removed */
#endif
} RT_802_11_MAC_ENTRY, *PRT_802_11_MAC_ENTRY;
#endif// WLAN_MULTIDRIVER


typedef struct _RT_802_11_MAC_TABLE {
    unsigned long       Num;
    RT_802_11_MAC_ENTRY Entry[MAX_LEN_OF_MAC_TABLE];
} RT_802_11_MAC_TABLE, *PRT_802_11_MAC_TABLE;

typedef struct mac_entry_s {
	unsigned char Addr[MAC_ADDR_LENGTH];
#if defined(TCSUPPORT_CT_PHONEAPP)
	signed char 	AvgRssi0;
	signed char 	AvgRssi1;
	signed char 	AvgRssi2;
#endif
}mac_entry_t;

typedef struct mac_table_s {
	unsigned long Num;
	mac_entry_t macEntry[MAX_LEN_OF_MAC_TABLE];
}mac_table_t;

void 
getMacEntryByIndex( mac_table_t *pMacTable, int index );
#ifdef BBUTOWBU
#ifdef WSC_AP_SUPPORT
#define GNU_PACKED __attribute__ ((packed))
typedef struct GNU_PACKED _WSC_CONFIGURED_VALUE {
	unsigned short WscConfigured; // 1 un-configured; 2 configured
	unsigned char	WscSsid[32 + 1];
	unsigned short WscAuthMode;	// mandatory, 0x01: open, 0x02: wpa-psk, 0x04: shared, 0x08:wpa, 0x10: wpa2, 0x20: wpa2-psk
	unsigned short WscEncrypType;	// 0x01: none, 0x02: wep, 0x04: tkip, 0x08: aes
	unsigned char	DefaultKeyIdx;
	unsigned char	WscWPAKey[64 + 1];
} WSC_CONFIGURED_VALUE;

#if defined(TCSUPPORT_WLAN_AC)
#ifdef MT7612E
typedef struct GNU_PACKED _WSC_CONFIGURED_VALUE_MT7612 {
	unsigned short  WscConfigured; /* 1 un-configured; 2 configured */
	unsigned char	WscSsid[32 + 1];
	unsigned short  WscAuthMode;	/* mandatory, 0x01: open, 0x02: wpa-psk, 0x04: shared, 0x08:wpa, 0x10: wpa2, 0x20: wpa2-psk */
	unsigned short 	WscEncrypType;	/* 0x01: none, 0x02: wep, 0x04: tkip, 0x08: aes */
	unsigned char	DefaultKeyIdx;
	unsigned char	WscWPAKey[64 + 1];
	unsigned char	WscSsidLen;
	unsigned char	WscWPAKeyLen;
} WSC_CONFIGURED_VALUE_MT7612;
#endif
#endif

//added by fredli
#define MAX_NUMBER_OF_ACL				64
typedef struct _RT_802_11_ACL_ENTRY {
    unsigned char   Addr[MAC_ADDR_LENGTH];
    unsigned short  Rsv;
} RT_802_11_ACL_ENTRY, *PRT_802_11_ACL_ENTRY;

typedef struct GNU_PACKED _RT_802_11_ACL {
    unsigned long   Policy;             // 0-disable, 1-positive list, 2-negative list
    unsigned long   Num;
    RT_802_11_ACL_ENTRY Entry[MAX_NUMBER_OF_ACL];
} RT_802_11_ACL, *PRT_802_11_ACL;
#endif
#endif

#endif

#ifdef TCSUPPORT_SYSLOG
/* add by yzwang for syslog*/
int
syslog_init(void);
int
syslog_boot(mxml_node_t *top);
int
syslog_read(mxml_node_t *node, char nodeName[][MAX_NODE_NAME], char *attr);
int
syslog_write(mxml_node_t *top, mxml_node_t *parant);
int
syslog_verify(mxml_node_t *node);
int
syslog_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

#if defined(TCSUPPORT_C1_CUC)
int
alarmlog_init(void);
int
alarmlog_read(mxml_node_t *node, char nodeName[][MAX_NODE_NAME], char *attr);
#endif

#endif
#ifdef CWMP
int
switchpara_init(void);
int
switchpara_boot(mxml_node_t *top);
int
switchpara_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#endif
#if defined(TCSUPPORT_CT_PORTAL_MANAGEMENT) || defined(TCSUPPORT_TRUE_LANDING_PAGE)
int 
portal_init(void);
int
portal_boot(mxml_node_t *top);
#endif
#if defined(TCSUPPORT_CT_PORTAL_MANAGEMENT) 
int
portal_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#endif
#ifdef TCSUPPORT_PRODUCTIONLINE
#if defined(TCSUPPORT_RESERVEAREA_EXTEND) || defined(TCSUPPORT_C7) || (defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505))
int parseProlinePara(int flag, proline_Para *pProline, char *value);
int checkParaValid(char *buf, int len);
int tcgetProLinePara(void*buf,int flag);
int proLineInfo_read(mxml_node_t *top,mxml_node_t *node);
#else
static int tcgetProLinePara(void*buf,int flag);
int
proLineInfo_read(mxml_node_t *top,mxml_node_t *node);
#endif
#endif
void handle_proline_paras(mxml_node_t* defaulttree);/*used by unopen code */

void GetEtherAddr(unsigned char *mac_addr);

#if defined(TCSUPPORT_CT_DS_LIMIT)
int dslimit_interfacemode(mxml_node_t *top, int up_dw);
int dslimit_vlanidmode(mxml_node_t *top, int up_dw);
int dslimit_iprangemode(mxml_node_t *top, int up_dw);

int dslimit_configure(mxml_node_t *top);
int dslimit_boot(mxml_node_t *top);
int dslimit_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#endif

#if defined(TCSUPPORT_CT_JOYME)
int
laninfo_init(void);
int
laninfo_read(mxml_node_t *node, char nodeName[][MAX_NODE_NAME], char *attr);
/* restore osgi right to factory default. */
int restoreOSGIRight(int mode);
/* restore API right1 to factory default. */
int restoreApiRight1(int mode);
/* restore plugin b/c to factory default. */
int restorePluginBC(int mode);

#define MOBILE_ENTRY "mobile_Entry"
#define MOBILE_OSRIGHTENTRY "mobile_OSRight"
#endif

#endif
int
webCustom_init(void);
int
webCustom_read(mxml_node_t *node, char nodeName[][MAX_NODE_NAME], char *attr);

#if defined(TCSUPPORT_CT_LOOPDETECT)
#define LOOPDETECT_CMD "echo %d %s > /proc/tc3162/loop_detect"
#define LOOP_ENABLE             0
#define LOOP_EXIST_PERIOD       1
#define LOOP_CANCEL_PERIOD      2
#define LOOP_VLANTAG            3
#define LOOP_ETHERNET_TYPE      4
#define LOOP_DETECT_EANBLE_PORT 5
#define LOOP_AUTO_SHUT_LAN      6
int loopbackdetect_init(void);
int loopdetect_boot(mxml_node_t *top);
int	loopdetect_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#endif
int dataspeedlimit_init(void);
#if defined(TCSUPPORT_CT_PHONEAPP)
unsigned long determine_type(char *file_path, unsigned char *firmware_type);
#endif


