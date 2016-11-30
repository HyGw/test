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
#ifndef _ADVANCED_H
#define _ADVANCED_H
#include <linux/version.h>
#define KERNEL_2_6_36 	(LINUX_VERSION_CODE > KERNEL_VERSION(2,6,31))
#define KERNEL_3_18_21 	(LINUX_VERSION_CODE >= KERNEL_VERSION(3,18,19))

#define FIREWALL  "Firewall"
#if 0  /*Orginal ci-command mode */
#define FIREWALL_ENTRY 	"Firewall_Entry"
#endif
#define FIREWALL_PATH "/etc/firewall.conf"

#if defined(TCSUPPORT_CT_E8GUI)
#define FIREWALL_LEVELCMD "/usr/bin/firewallcmd level %s"
#define FIREWALL_ON "/usr/bin/firewallcmd switch on"
#define FIREWALL_OFF "/usr/bin/firewallcmd switch off"
#if defined(TCSUPPORT_CT_NEWGUI)
#define FIREWALL_DoSProtectCMD "/usr/bin/firewallcmd DoSSwitch %s"
#endif
#endif
#define ROUTE	"Route"
#define ROUTE_NUM "Route_num"
#define ROUTE_COUNT_PATH 	"/etc/route.count"
#define ROUTE_PATH 	"/etc/route.sh"
/*#define ROUTE_ENTRY 	"Route_%d_Entry"*/
#define ROUTE_ATTR_ID		"ID"
#define ROUTE_ATTR_DST_IP	"DST_IP"
#define ROUTE_ATTR_SUB_MASK	"Sub_mask"
#define ROUTE_ATTR_DEVICE	"Device"
#define ROUTE_ATTR_GW	"Gateway"
#define ROUTE_CMD_DEV	"dev"
#define ROUTE_CFG_IP_INDEX	3
#define ROUTE_CFG_SUB_MASK_INDEX	5
#define ROUTE_CFG_DEV_INDEX	6
#define ROUTE_CFG_GW_INDEX	7
#define ROUTE_CFG_METRIC_NAME_INDEX	8
#define ROUTE_CFG_METRIC_INDEX	9
/*shnwind modify 2008.4.17*/
#define ROUTE_CMD_NO_METRIC_DEV "/sbin/route add -net %s netmask %s dev %s\n"
#define ROUTE_CMD_NO_METRIC_GW "/sbin/route add -net %s netmask %s gw %s\n"
#define ROUTE_CMD_WITH_METRIC_DEV "/sbin/route add -net %s netmask %s dev %s metric %s\n"
#define ROUTE_CMD_WITH_METRIC_GW "/sbin/route add -net %s netmask %s gw %s metric %s\n"
#define ROUTE_IP_INDEX	0
#define ROUTE_MASK_INDEX	1
#define ROUTE_IF_TYPE_INDEX	2
#define ROUTE_GW_INDEX	2
#define ROUTE_METRIC_INDEX	3
#if defined(TCSUPPORT_CMCC)
#define MAX_STATIC_ROUTE_NUM 32
#else
#define MAX_STATIC_ROUTE_NUM 16 //add by brian for change route mechanism
#endif

#ifdef IPV6
#if defined(TCSUPPORT_CMCC)
#define MAX_STATIC_ROUTE6_NUM 32
#else
#define MAX_STATIC_ROUTE6_NUM 16 
#endif
#define ROUTE6	"Route6"

#define ROUTE6_CMD_WITH_GW_DEV "/sbin/route -A inet6 add %s/%s gw %s dev %s\n"
#define ROUTE6_CMD_NO_DEV "/sbin/route -A inet6 add %s/%s gw %s\n"
#define ROUTE6_CMD_NO_GW "/sbin/route -A inet6 add %s/%s dev %s\n"

#define ROUTE6_BACK_UP_PATH "/etc/route6_backup"
#define MAX_ROUTE6_PARA_LEN 45
#define ROUTE6_ADD 1
#define ROUTE6_DEL 2

enum route6_para_type{
	DST_IP_TYPE=0,
	PREFIX_LEN_TYPE,
	GATEWAY_TYPE,
	DEVICE_TYPE,
};
#endif


#define NAT_PATH	"/etc/nat_pvc%d/ipmode"
#define DMZ_PATH "/var/run/%s/dmz"  //put dmz conf in interface folder
#define VIRSERV_PATH "var/run/%s/vserver%s"   //put vs conf in interface folder
#define IPADDRMAP_PATH "/etc/nat_pvc%d/ipaddr_mapping%d"
#define IPADDRMAP_SERVERIP_PATH "/etc/nat_pvc%d/server_ip"

//#define QOS_DROP  1  /*Rodney_20091115*/

/*QoS*/
#if 0  /*Rodney_20090724*/
#define QOS_SH	"/etc/qos_rule.sh"
#define QOS_MODULE_PATH "/lib/modules/%s/kernel/net/bridge/netfilter/"
#define QOS_MODULE_NUM  6
#define QOS "QoS"
#define QOS_COMMON "Common"
#define QOS_BROUTE  "ebtables -t broute"
#endif

#ifdef QOS_DROP  /*Rodney_20091115*/
#define QOS_DROP_PATH   "/lib/modules/%s/kernel/net/sched/"
#define QOS_DROP_TAIL   1
#define QOS_DROP_RED    2
#define QOS_DROP_WRED   4

#define QOS_PRIO_IPP    1
#define QOS_PRIO_DSCP   2
#define QOS_PRIO_QUEUE  4

#define QOS_PPP_INTERFACE 0
#define QOS_NAS_INTERFACE 1

#define DROP_LIMIT  20000
#define DROP_MIN    5000
#define DROP_MAX    10000
#define DROP_AVPKT  5000
#define DROP_BURST  1
#define DROP_PRO   1

#define ENABLE      1
#define DISABLE     0
#endif
#define QOS_SH					"/etc/qos_rule.sh"
#if KERNEL_2_6_36
#if KERNEL_3_18_21
#define	KERNEL_VER_PATH			"3.18.21"
#else
#define	KERNEL_VER_PATH			"2.6.36"
#endif
#define QOS_MODULE_PATH 		"/lib/modules/"KERNEL_VER_PATH"/kernel/net/bridge/netfilter/"
#else
#define QOS_MODULE_PATH 		"/lib/modules/2.6.22.15/kernel/net/bridge/netfilter/"
#endif
#define QOS_MODULE_NUM  		8
#if KERNEL_2_6_36
#define QOS_TC_MODULE_PATH		"/lib/modules/2.6.36/kernel/net/sched"
#else
#define QOS_TC_MODULE_PATH		"/lib/modules/2.6.22.15/kernel/net/sched"
#endif
#define QOS_TC_MODULE_NUM		4
#if KERNEL_2_6_36
#define QOS_EBT_MODULE_PATH 	"/lib/modules/"KERNEL_VER_PATH"/kernel/net/bridge/netfilter/"
#else
#define QOS_EBT_MODULE_PATH 	"/lib/modules/2.6.22.15/kernel/net/bridge/netfilter/"
#endif
#define QOS_EBT_MODULE_NUM		8
#define QOS 					"QoS"
#define QOS_COMMON 				"Common"
#define QOS_BROUTE  "ebtables -t broute"

#define ON						1
#define OFF						0
#define MAX_CMD_LENGTH			256
#define CMD_BUF_LENGTH			64
#define	INSMODE					1
#define RMMODE					0
#define ADD						1
#define DEL						0
#if defined(TCSUPPORT_CT_QOS)
#define MAX_TYPE_RULE_NUM			10
#define MAX_TYPE_NUM				10
#define MAX_APP_TYPE_NUM			15
#else
#define MAX_TYPE_RULE_NUM			16
#endif
#define MAX_APP_RULE_NUM			4
#define MAX_TEM_RULE_NUM			8
#define MAX_VLAN_RULE_NUM			8
#define PQ_QUEUE_NUM				4
#define	WRR_QUEUE_NUM				4
#define	CAR_QUEUE_NUM				6

//#define	QOS_DEBUG

#if 0  /*Orginal ci-command mode */
#define NAT	"Nat"

#define NAT_IPMODE	"IPMODE"
#define NAT_SUBPVC_ENTRY	"%s_PVC%d_Entry"
#define NAT_SUBRULE_ENTRY	"%s_PVC%d_%d_Entry"
/*#define NAT_RULE_INDEX	0*/
#define DMZ_ACTIVE_INDEX	0
/*#define IPADDRMAP_RULE_INDEX 0*/
#define ADSL	"Adsl"
#endif
#define DMZ	"Dmz"
#define DMZ_SH	"/usr/script/dmz.sh"
#define VIRSERV	"VirServer"
#define VIRSERV_SH	"/usr/script/vserver.sh"
#define IPADDRMAP	"IpAddrMap"
#define IPADDRMAP_SERVER "Server"
#define IPADDRMAP_SH	"/usr/script/ipaddr_mapping.sh"
#define ONE2ONE	"One-to-One"
#define MANY2ONE	"Many-to-One"
#if defined(TCSUPPORT_AUTO_DMZ)
#define DMZ_ENTRY "Dmz_Entry"
#define DMZ_INTERVAL		"Interval"
#define DMZ_COMMON	"Dmz_Common"
#define DEFAULT_DMZ_INTERVAL	"30"
#define AUTO_DMZ_FLAG	"AutoDMZ_Flag"
#define IS_SUPPORT_AUTO_DMZ	"isAutoDMZSupported"
#define UPDATE_DMZ_ACTIVE		"UpdateDMZ_Active"
#endif


#define ADSL_ATTR_MODE	"MODULATIONTYPE"
#define ADSL_ATTR_TYPE	"ANNEXTYPEA"
#define ADSL_OPEN_CMD "wan adsl opencmd"
#define ADSL_SETANNEX_CMD "wan ghs set annex"
#define ADSL_PATH "/etc/adsl.conf"
#define ADSL_SH	"/etc/adsl.sh"
/*#define ADSL_ENTRY 	"Adsl_Entry"*/
#define ADSL_NAME_INDEX 0
#define ADSL_CMD_INDEX 1

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
#define ADSL_NAME "Adsl"
#define ADSL_ACTIVE "Active"
#define SAR_OPEN_CMD "wan adsl open"
#define SAR_CLOSE_CMD "wan adsl close"
#endif

#if 0  /*Orginal ci-command mode */
int
firewall_read(mxml_node_t *node);
#endif

#define ALGSW_NODE_NAME	"ALGSwitch"

int
firewall_boot(mxml_node_t *top);
int
firewall_init(void);
int
firewall_write(mxml_node_t *top, mxml_node_t *parant);
int
firewall_verify(mxml_node_t *node);
int
firewall_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int
firewall_check_filter(mxml_node_t *top);
void
firewall_scan_port_and_add_rule();

int
route_init(void);
int
route_read(mxml_node_t *node, char nodeName[][MAX_NODE_NAME], char *attr);

int
route_write(mxml_node_t *top, mxml_node_t *parant);
int
route_verify(mxml_node_t *node);
int
route_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#if defined(TCSUPPORT_CT_DSL_EX)
int
route_boot(mxml_node_t *top);
#endif

int 
route_pre_unset(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#ifdef IPV6
int
route6_init(void);
int
route6_boot(mxml_node_t *top);
int
route6_read(mxml_node_t *node, char nodeName[][MAX_NODE_NAME], char *attr);
int
route6_write(mxml_node_t *top, mxml_node_t *parant);
int
route6_verify(mxml_node_t *node);
int
route6_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int 
route6_pre_unset(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#endif
/*krammer add for bug 1427*/
void
update_route_sh(mxml_node_t *top, mxml_node_t *parant);

#if 0  /*Orginal ci-command mode */
int
nat_read(mxml_node_t *node);
#endif

int
nat_init(void);
int
nat_write(mxml_node_t *top, mxml_node_t *parant);
int
nat_verify(mxml_node_t *node);
int
nat_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

int
algswitch_init(void);
int
algswitch_boot(mxml_node_t *top);
int
algswitch_write(mxml_node_t *top, mxml_node_t *parant);
int
algswitch_verify(mxml_node_t *node);
int
algswitch_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

#if 0  /*Orginal ci-command mode */
int
dmz_read(mxml_node_t *node);
#endif

int
dmz_init(void);
int
dmz_write(mxml_node_t *top, mxml_node_t *parant);
int
dmz_verify(mxml_node_t *node);
int
dmz_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

#if 0  /*Orginal ci-command mode */
int
virServer_read(mxml_node_t *node);
#endif

int
virServ_init(void);
int
virServer_write(mxml_node_t *top, mxml_node_t *parant);
int
virServer_verify(mxml_node_t *node);
int
virServer_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

#if 0  /*Orginal ci-command mode */
int
ipAddrMap_read(mxml_node_t *node);
#endif
int
ipAddrMap_write(mxml_node_t *top, mxml_node_t *parant);
int
ipAddrMap_verify(mxml_node_t *node);
int
ipAddrMap_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

#if 0  /*Orginal ci-command mode */
int
adsl_read(mxml_node_t *node);
#endif

int
adsl_boot(mxml_node_t *top);
int
adsl_init(void);
int
adsl_write(mxml_node_t *top, mxml_node_t *parant);
int
adsl_verify(mxml_node_t *node);
int
adsl_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
/****************shnwind add****************/
void create_route_exe_sh(mxml_node_t *top, int action);
void create_route_sh(mxml_node_t *top, mxml_node_t *parant);
int change_device_name(mxml_node_t *top, char *dev, char *ret);
//int rebuild_route(mxml_node_t *top);
#define FIREWALL_TMP_PATH "/etc/firewall.conf.tmp"
#define ROUTE_EXE_SH "/etc/route_exe.sh"
#define ROUTE_TMP_PATH	"/tmp/route"
#define ROUTE_ADD 1
#define ROUTE_DEL 0
#define ROUTE_EXE_PRE_SH "/etc/route_exe_pre.sh"
#define ROUTE_EXE_AFT_SH "/etc/route_exe_aft.sh"
/************shnwind add end***********/
#endif

#ifdef TCSUPPORT_QOS
int qosTCModule(int type);
int qosEBTModule(int type);
int excQosCmd(char *cmd);
void setQoSTempRules(mxml_node_t *top, int type);
int setQoSSwitch(mxml_node_t *top, int sw);
int qosDiscplineRule(mxml_node_t *top);
int qosTypeRule(mxml_node_t *top, int index);
int qosTemRule(mxml_node_t *top, int index);
int qosAppRule(mxml_node_t *top, int index, int type);
int qosVlanidRule(mxml_node_t *top, int index, int type);
#if defined(TCSUPPORT_CT_QOS)
int qos_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char* attr);
#endif
int qosWanInterfaceUpdate(mxml_node_t *top, int on_off);
#define MAX_UPLINK_BANDWIDTH			950
#define MAX_UPLINK_BANDWIDTH_STRING		"950"
#endif

#if 1  /*Rodney_20090724*/
int qos_init(void);
int qos_boot(mxml_node_t *top);
int qos_write(mxml_node_t *top, mxml_node_t *parant);
int qos_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int qos_paramcheck(mxml_node_t *top, char nodeName[][MAX_NODE_NAME], char qos_attr_name[][32], int index);
void qos_remarking(FILE *fp, int index, char *jumpchain);
void qos_IPv4rule(FILE *fp, int index, char *jumpchain);
void qos_8021Qrule(FILE *fp, int index, char *jumpchain);
int qos_checkRangeValue(unsigned int val1, unsigned int val2, unsigned int min, unsigned int max);
void qos_moduleInsert();
void qos_moduleRemove();

#else
/**********20080703 racing add************/
#if 0  /*Orginal ci-command mode */
int
qos_read(mxml_node_t *node);
#endif
int
qos_write(mxml_node_t *top, mxml_node_t *parant);
int
qos_verify(mxml_node_t *node);
int
qos_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#endif

#ifdef QOS_DROP  /*Rodney_20091115*/
void qos_drop_reset(mxml_node_t *top);
void qos_drop_set(int mode, int drop_mode_id, int prio_mode_id);
void qos_drop_set_rule(int interface, int pvc);
void qos_drop_unset_rule();
#endif

/**********20080703 racing add************/
/*autopvc*/
#define MAX_VCPOOL_NUM 16
#define AUTOPVC "AutoPVC"
#define TMP_MAC_PATH "/tmp/mac"

typedef struct pvc_pool_s{
	int vpi;
	int vci;
	int encap;
	int state;
}pvc_pool_t;

typedef struct vc_pool_s{
	int pvc;
	int vpi;
	int vci;
}vc_pool_t;

#define AUTOPVC_STOP 0
#define AUTOPVC_START 1
#define AUTOPVC_SUCCESS 2
#define AUTOPVC_SUCCESS_AND_REPLACE 3

#define ENCAP_LLC 0
#define ENCAP_VC 1

#define PROBE_OAM 1
#define PROBE_PPPOE (1<<1)
#define PROBE_ARP (1<<2)
#define PROBE_DHCP (1<<3)

#define AUTOPVC_SET_CMD "sys autopvc set"
#define AUTOPVC_ENCAP_CMD "sys autopvc encap"
#define AUTOPVC_SERVICE_CMD "sys autopvc service"

int
autopvc_init(void);
int
autopvc_write(mxml_node_t *top, mxml_node_t *parant);
int
autopvc_verify(mxml_node_t *node);
int
autopvc_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int check_vpi_vci(int vpi, int vci);
int get_current_pvc_setting(mxml_node_t *top);
int get_autopvc_setting(mxml_node_t *top);
int check_adsl_stat(void);
void
reset_all_autopvc_data();


/*autopvc*/
#if 0//def TCSUPPORT_PORTBIND

/*portbind*/
//#define  PORTBIND_DEBUG
#define  MAX_PORTBIND_NUM		16
#define	 CMD_BUF_LEN			128
#define  BUF_LEN				32
#define  PORTBIND				"PortBind"
#define	 PORTBIND_COMMON		"Common"

struct if_map
{
	char *webIfName;
	char *ifName;
};

int
portbind_init(void);
int
portbind_write(mxml_node_t *top, mxml_node_t *parant);
int
portbind_verify(mxml_node_t *node);
int
portbind_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int
portbind_boot(mxml_node_t *top);
int
syn_portbind_summary(mxml_node_t *node);
int 
exec_portbind_cmd(char *cmd);
#endif

#ifdef TCSUPPORT_DMS
#define DMSCFG_PATH "/etc/dms_basic.cfg"
#define DMSDEVICECFG_PATH "/etc/dms_device.cfg"
#define DMS_START_PATH "/usr/script/dms_start.sh"

#if defined(TCSUPPORT_CT_JOYME)
#define DMS_START_PLUGIN_PATH "/usr/script/dms_start_plugin.sh"
#define DLNA_PATH "/usr/osgi/plugin-b/com.chinatelecom.econet.smartgateway.dlna/"
#define DMSTCAPP_PLUGIN_PATH "/usr/osgi/plugin-b/com.chinatelecom.econet.smartgateway.dlna/userfs/dlna/bin/dlna_dmsTcApp"
#endif

#define DMS_NODE "DMS"
#define DMSCFG_NODE "Basic"
#define DMSCFG_IPADDR_NODE "IPAddress"
#define DMSDEVICECFG_NODE "Device"

int
dms_boot(mxml_node_t *top);
int
dms_init(void);
int
dms_write(mxml_node_t *top, mxml_node_t *parant);
int
dms_verify(mxml_node_t *node);
int
dms_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#endif

#if defined(TCSUPPORT_CT_DSL_EX)
int
virServATM_boot(mxml_node_t *top);
int
virServATM_init(void);
int
virServerATM_write(mxml_node_t *top, mxml_node_t *parant);
int
virServerATM_verify(mxml_node_t *node);
int
virServerATM_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

int
virServPTM_boot(mxml_node_t *top);
int
virServPTM_init(void);
int
virServerPTM_write(mxml_node_t *top, mxml_node_t *parant);
int
virServerPTM_verify(mxml_node_t *node);
int
virServerPTM_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

int routeATM_init(void);
int routePTM_init(void);
int route6ATM_init(void);
int route6PTM_init(void);
int dmzATM_init(void);
int dmzPTM_init(void);
int igmpproxyATM_init(void);
int igmpproxyPTM_init(void);
int mldproxyATM_init(void);
int mldproxyPTM_init(void);
#endif

#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
int SimCard_boot(mxml_node_t *top);
int SimCard_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int SimCard_init(void);
#endif


