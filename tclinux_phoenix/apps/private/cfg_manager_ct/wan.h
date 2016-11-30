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
#ifndef _WAN_H
#define _WAN_H


#define PPPOA_PVC_ENTRY_NUM  1

#define WAN	"Wan"
#define WAN_COMMON "Common"
#define WAN_PVC "PVC"
#define WAN_INTERFACE "Entry"
#define WAN_IFNAME "IFName"
#define WAN_NASNAME "NASName"
#define WAN_PATH "/etc/isp%d.conf"
#define WAN_STATE	"ACTIVE"
#define WAN_DEACTIVE	"No"
#define WAN_ENCAP_DYN	0
#define WAN_ENCAP_STATIC	1
#define WAN_ENCAP_PPP	2
#define WAN_ENCAP_BRIDGE	3

#define DYNMIC_MODE	"0"
#define STATIC_MODE		"1"
#define PPP_MODE		"2"
#define BRIDGE_MODE	"3"
#if defined(TCSUPPORT_CT_RECONNECT_NODE)
#define RECONNECT_ENABLE   "1"
#endif
#if defined(TCSUPPORT_CT_ACCESSLIMIT)
#define NOTBINDSTRING "NOTBIND"
#endif

#define ENCAP_PPPOA_LLC   "PPPoA LLC"
#define ENCAP_PPPOA_VC    "PPPoA VC-Mux"

#define PVC_PID_PATH "/var/run/"
#define WAN_IF_PATH "/var/run/"
#define WAN_START_SH "/usr/script/wan_start.sh"
#define WAN_STOP_SH "/usr/script/wan_stop.sh"

#define DEFAULT_NASNAME_FORMAT  "nas%d_%d"  /*nas(PVC_index)_(EntryIndex),ext:nas0_1*/
#define DEFAULT_NASNAME_FORMAT_S  "nas%s_%s"  /*nas(PVC_index)_(EntryIndex),ext:nas0_1*/
#define DEFAULT_IFNAME_FORMAT "nas%d_%d"

/*WanInfo Node*/
#define MAX_WANIF_NUM   64

#define VALID_IFINDEX_STR_LEN 192
#define MAX_INFO_LENGTH 128
#define MAX_BUFF_SIZE 1024
#define SMALL_BUFF_SIZE 256
#if defined(TCSUPPORT_CT_PON)
#define MAX_BUFF_SIZE_CNT 15
#define SMALL_BUFF_CNT 12
#define CYCLEVALUE_MAXCNT 27
#else
#if defined(TCSUPPORT_CT_WAN_PTM) || defined(TCSUPPORT_CT_E8B_ADSL)
#define MAX_BUFF_SIZE_CNT 15
#define SMALL_BUFF_CNT 14
#define CYCLEVALUE_MAXCNT 29
#endif
#endif
#if defined(TCSUPPORT_CT_WAN_PTM) || defined(TCSUPPORT_CT_E8B_ADSL)
#define ATM_PVC_MAXCNT 8
#define PTM_PVC_MAXCNT 8
#define ATM_CONN_MAXCNT 8
#define PTM_CONN_MAXCNT 8
#define ERROR_ATM_PVC_FULL 6
#define ERROR_PTM_PVC_FULL 7
#define UI_CONNECTION_MAXCNT 8
#endif


#define WAN_IF_NODE   "WanInfo"
#define WAN_IF_COMMON  "Common"
#define WAN_IF_COM_VALID_INDEX "ValidIFIndex"
#define WAN_IF_COM_VALID_NUM  "ValidIFNum"
#define WAN_IF_COM_CUR_INDEX "CurIFIndex"
#define WAN_IF_MSG   "Message"
#define WAN_IF_CUR_PVC   "WanPVC"
#define WAN_IF_CUR_IF   "WanIF"
#define WAN_IF_CUR_Action   "Action"
#define WAN_IF_MODIFY  "Modify"
#define WAN_IF_ADD  "Add"
#define WAN_IF_DEL  "Del"
#define WAN_IF_PPPMANUALSTATUS	"PPPManualStatus"
#if defined(TCSUPPORT_CT_PON) || defined(TCSUPPORT_CT_WAN_PTM) || defined(TCSUPPORT_CT_E8B_ADSL)
#define WAN_IF_VLANID	"VLANID"
#define WAN_IF_PBIT		"DOT1P"
#define WAN_IF_INDEX  	"IFIdx"
#endif
#if defined(TCSUPPORT_CT_WAN_PTM) || defined(TCSUPPORT_CT_E8B_ADSL)
#define WAN_IF_ATMENABLE		"ATMEnable"
#define WAN_IF_PTMENABLE		"PTMEnable"
#define	WAN_IF_UICUSTOM			"UICustomInfo"
#define WAN_IF_STOPWANUPDATE 	"PauseUpdateWanInfo"
#endif
#define WAN_IF_CYCLEJUMP		"CycleJump"
#define	WAN_IF_ALLROUTE			"AllRoute"
#if defined(TCSUPPORT_CT_PPP_ONDEMAND)
#define WAN_COMMON_DEFROUTEIDXV4  "DefRouteIndexv4"
#define WAN_COMMON_DEFROUTEIDXV6  "DefRouteIndexv6"
#define WAN_COMMON_ONDEMANDIDXV4  "OnDemandRouteIndexv4"
#define WAN_COMMON_ONDEMANDIDXV6  "OnDemandRouteIndexv6"
#endif


#define SERVICE_LIST_LEN  		64

//DHCPRealy xmdai_nj
#define WAN_DHCPREALY   "DHCPRealy"
#define WAN_MODE  "WanMode"
#define WAN_BRIDGE_MODE  "Bridge"
#define WAN_ROUTE_MODE  "Route"

enum wan_info_type{
	WAN_STATUS_TYPE=0,
	WAN_IP_TYPE,
	WAN_NETMASK_TYPE,
	WAN_DNS_TYPE,
	WAN_DNS2_TYPE,
	WAN_GATEWAY_TYPE,
#ifdef IPV6
	WAN_STATUS6_TYPE,
	WAN_IP6_TYPE,
	WAN_PFXLEN_TYPE,
	WAN_6DNS_TYPE,
	WAN_6DNS2_TYPE,
	WAN_GATEWAY6_TYPE,
	WAN_PD6_TYPE,
	WAN_PD6_PTIME_TYPE,
	WAN_PD6_VTIME_TYPE,
#if defined(TCSUPPORT_CT_DSLITE)
	WAN_DSLITE_ADDR_TYPE,
	WAN_DSLITE_NAME_TYPE,
#endif
	WAN_ORGPD6_TYPE,
#endif
};

/* add for notify some interface is down */
void notify_down_info(mxml_node_t *top, int if_index, char *ifname);

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
int 
wan_pre_unset(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#if defined(TCSUPPORT_CT_E8DDNS)
void Unset_DDNS_Entry(mxml_node_t *top, int pvcIndex, int entryIndex);
#endif
int
wanInfo_init(void);
int
wanInfo_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr);
int
wanInfo_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

int
set_wan_err(int err);
int
get_pvc_totalNumber(int indexbuf[]);
int
get_entry_numberByPvc(int pvc_index, int idle_entry[]);
int
copy_wan_info(int dst_pvcindex, int dst_entryindex);
//#define	CT_COM
#define 	MAX_INTERFACE_NUM		64
#define         MAX_PVC_NUM			8
#if defined(TCSUPPORT_CT)
//#define		CT_COM_DBUG
//#define		WAN_DBUG

/* service list */
#define		INTERNET			"INTERNET"
#define		TR069          	 	"TR069"
#define		TR069_INTERNET   	"TR069_INTERNET"
#define		VOICE          	 	"VOICE"
#define     OTHER               "OTHER"

#if defined(TCSUPPORT_CT_DNSBIND)
void set_policy_route(mxml_node_t *top, char *if_name, int if_index, int state);
#endif

extern mxml_node_t *get_web_node(mxml_node_t *top, char name[][MAX_NODE_NAME]);
void wan_cmd_excute(char *cmd);
#if defined(TCSUPPORT_CT_FULL_ROUTE)
int full_route_execute(int pvc_index,int entry_index,char * strWanMode);
int is_full_route_enable(void);
void remove_all_tables(void);
void close_table_by_ifIndex(int index);
#endif

#ifdef TCSUPPORT_PORTBIND
#define		MAX_BINDLIST_NUM		64
struct bindlist_map
{
	char *bindif;
	char *realif;
};
void set_portbinding_info(mxml_node_t *top, char *if_name, int if_index, int state);
void set_portbinding_info2(mxml_node_t *top, int pvc_index, int entry_index, int type);
#endif

#if defined(TCSUPPORT_CT_PPPOEPROXY)
void set_blackhole_route(mxml_node_t *top, char *if_name, int if_index);
#endif

//DHCPRealy xmdai_nj
void set_DHCPRealy_info(mxml_node_t *top, int pvc_index, int entry_index, int type);
#if defined(TCSUPPORT_CT_DHCP_PORT_FILTER)
void set_DHCP_PortFilter_info(mxml_node_t *top, int pvc_index, int entry_index, int iType);
enum wan_dhcpfilter_type{
	DEL_DHCP_PORTFILTER_CMD = 0,
	ADD_DHCP_PORTFILTER_CMD,
};
#endif

#define ADD_DHCPREALY_CMD			 1
#define	DEL_DHCPREALY_CMD			 0

void route_switch(mxml_node_t *top, char *route_index, int state);
void set_default_route(mxml_node_t *top, char *if_name, int if_index, int state);
#ifdef CWMP
#define TRO69_IF_CONF		"/var/run/cwmp_if"
#define TRO69_IF_CONF2		"/var/run/cwmp_if2"

void set_tr069_nat(mxml_node_t *top, char *if_name, int if_index, int state);
void del_if_from_conf(char *if_name);
void update_cwmp_route(mxml_node_t *top, char *if_name, int if_index, int state);
void update_cwmp_route_node(mxml_node_t *top);
#endif

#ifdef TCSUPPORT_QOS
#if defined(TCSUPPORT_CT_QOS)
#define  MAX_TYPE_ENTRY  10
#define  MAX_TYPE_NUM  	 10
void update_qos_vlan_rule(mxml_node_t *top, int if_index);
#endif
#endif
/* add iptables rule make sure connection request, wan side web server, tftp etc. success */
void set_preService_rule(mxml_node_t *top, char *if_name, int if_index, int state);

/* add/del portbinding group */
#define 	ADD_PORTBIND			 1
#define		DEL_PORTBIND			 0
#if defined(TCSUPPORT_CT_WAN_PTM) || defined(TCSUPPORT_CT_E8B_ADSL)
int isPVCFullviaTransMode(int transmode, int *full_errCode);
#endif
#endif

#if defined(TCSUPPORT_CT_WAN_CHILD_PREFIX)
#define PREFIX_ORIGN_DFT	0
#define PREFIX_ORIGN_SLLA	1
#define PREFIX_ORIGN_DHCP	2
#define PREFIX_ORIGN_STATIC	3
#define PREFIX_ORIGN_NONE	4
#define TMP_PREFIX_IF_PATH	"/tmp/prefix_if.conf"

#define MAX_PREFIX_ADDR_LEN	44 //include mask
#define MAX_PREFIX_MASK_LEN	64

#define LAN_IPV6_2	"IP6_2"
#define LAN_IPV6_3	"IP6_3"
#define START_NEW_ADD_lAN_IPV6	2
#define MAX_NEW_ADD_lAN_IPV6	3

#define RADVD_DEF_PREFER_LIFE "3600"
#define RADVD_DEF_VALID_LIFE "7200"

#define IFC_MAC_ADDR_ATTR	"IFC_MAC"
#define CHILD_ORIGN_TYPE	"Child_orign_type"

#define RESERVE_MIN_IP6_ADDR "::1"
#define DEFAULT_START_IP6_ADDR "::2"

#endif

#if defined(TCSUPPORT_CT_DHCP6C_STATUS_CODE)
#define ENABLE_DHCP6C_ERROR_HANDLE	1
#define DISABLE_DHCP6C_ERROR_HANDLE	0
#define DHCP6C_HANDLE_STATUS_CODE_PATH	"/etc/dhcp6c_handle_err.conf"
#endif

#if defined(TCSUPPORT_CT_VLAN_BIND)
#if defined(TCSUPPORT_WLAN_AC)
#define MAX_LAN_PORT_NUM     14
#else
#define MAX_LAN_PORT_NUM     10
#endif
#define MAX_MAP_ONE_ENTRY    20
#define MAX_PVC_ENTRY			8

#define VLAN_BIND "VlanBind"
#define VLAN_BIND_ENTRY "Entry"
#define VLAN_BIND_SH	"/etc/vlan_bind_rule.sh"
#define EBTABLES_BROUTE  "ebtables -t broute"
#define VBIND_ACTIVE_PATH "/proc/tc3162/vbind_active"
#define VBIND_ENTRY_ACTIVE_PATH "/proc/tc3162/vbind_entry_active"
#define VBIND_ENTRY_ARRAY_PATH "/proc/tc3162/vbind_entry_array"

#define VLAN_MODE "VLANMode"
#define VLAN_ID "VLANID"
typedef struct vlan_map_s		
{
	  int m;	/*VLAN ID of LAN side */
	  int n;	/*VLAN ID of WAN side */
} vlan_map_t;

int
vlanbind_boot(mxml_node_t *top);
int
vlanbind_init(void);
int
vlanbind_write(mxml_node_t *top, mxml_node_t *parant);
int
vlanbind_verify(mxml_node_t *node);
int
vlanbind_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#endif

#if defined(TCSUPPORT_CT_DSL_EX)
#define WAN_ATM		"WanATM"
#define WAN_PTM		"WanPTM"
#define WAN_DSLMODE	"DslMode"
#define WAN_EXED	"WanExecuted"
#define WAN_LOCKED	"WanLocked"
#define WAN_COPY_FLAG "WanCPFLAG"
#define WEB_NODE "WebCustom"
#define SYS_NODE "Sys"
#define ENTRY 	"Entry"
#define VIRSERVER 	"VirServer"
#define VIRSERVER_ATM 	"VirServerATM"
#define VIRSERVER_PTM 	"VirServerPTM"
#define ROUTE_ATM 	"RouteATM"
#define ROUTE_PTM 	"RoutePTM"
#define ROUTE6_ATM 	"Route6ATM"
#define ROUTE6_PTM 	"Route6PTM"
#define DMZ_ATM 	"DmzATM"
#define DMZ_PTM 	"DmzPTM"
#define IGMPPROXY_ATM 	"IGMPproxyATM"
#define IGMPPROXY_PTM 	"IGMPproxyPTM"
#define MLDPROXY_ATM 	"MLDproxyATM"
#define MLDPROXY_PTM 	"MLDproxyPTM"
#define DDNS_ATM 	"DdnsATM"
#define DDNS_PTM 	"DdnsPTM"

int copy_wan_node(int state);
int update_wan_node();

int
wanATM_boot(mxml_node_t *top);
int
wanATM_init(void);
int
wanATM_write(mxml_node_t *top, mxml_node_t *parant);
int
wanATM_verify(mxml_node_t *node);
int
wanATM_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

int
wanPTM_boot(mxml_node_t *top);
int
wanPTM_init(void);
int
wanPTM_write(mxml_node_t *top, mxml_node_t *parant);
int
wanPTM_verify(mxml_node_t *node);
int
wanPTM_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

#endif
#if defined(TCSUPPORT_CT_WANNINDEX_INCREASE)
void setLastUsedPvcIndex(int pvc);
void setLastUsedEntryIndex(int pvc, int entry);
#endif	

int updateDhcpRules();

#if defined(TCSUPPORT_CT_PON_CN_JS) || defined(TCSUPPORT_CT_PON_CZ_GD)
struct pvc_entry_list
{
	int pvcIdx;
	int entryIdx;
};
static void wan_check_portbind(mxml_node_t *top, int cur_if, int addFlag);
#endif

#endif
