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

#include "cfg_manager.h"
#include "utility.h"
#include "wan.h"
#include "cc.h"
#include "wan_monitor.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#if defined(TCSUPPORT_C1_CUC) && defined(TCSUPPORT_SYSLOG)
#include <syslog.h>
#endif
#if defined(TCSUPPORT_CT_PON)
#ifdef TCSUPPORT_PONMGR
#include "xpon.h"
#endif
#endif
#if defined(TCSUPPORT_CT_PPPINFORM)
#include <string.h>
#endif

#if defined(TCSUPPORT_CT_PMINFORM)
extern int updateCwmpInfo(mxml_node_t *top, char *wanip, int state);
#endif
#if defined(TCSUPPORT_CT_PPPOEPROXY)
#define PPPOEPROXYOFFSET 0
void pppoeproxy(mxml_node_t *top);
#endif
#if defined(TCSUPPORT_CT_WAN_PTM)
int g_ptm_refcnt[2] = {0};
#endif
#if defined(TCSUPPORT_CT_DSLITE)
extern void set_dslite_internal_state(int if_index, int state);
extern void set_dslite_internal_bind_state(int if_index, int state);
extern int stop_dslite(int pvc_index, int entry_index);
extern int get_dslite_stateviacfg(mxml_node_t *top, char nodeName[][MAX_NODE_NAME]);
#endif
#if defined(TCSUPPORT_CT_BRIDGEARP_NOFWD_LAN)
extern int isNeedChgArpRule;
#endif
#if defined(TCSUPPORT_CT_DHCP_PORT_FILTER)
int isNeedUpdatePortFilter = 0;
#endif
#if defined(TCSUPPORT_CT_PON_JS) || defined(TCSUPPORT_CT_NAMECHGNOTIFY)
extern void doNameChangeWanInit(mxml_node_t *top);
#endif

extern int g_adsl_state;


//#define WAN_DBUG

int g_pvc_refcnt[PVC_NUM] = {0};
int g_wan_boot_finish = 0;           /*is wan_boot excuted*/	


char wan_info_node[][MAX_NODE_NAME]=
{
	{"Status"},
	{"IP"},
	{"NetMask"},
	{"DNS"},
	{"SecDNS"},
	{"GateWay"}, 
#ifdef IPV6
	{"Status6"},
	{"IP6"},
	{"PrefixLen6"},
	{"DNS6"},
	{"SecDNS6"},
	{"GateWay6"}, 
	{"PD6"},
	{"PD6ptime"},
	{"PD6vtime"},
#if defined(TCSUPPORT_CT_DSLITE)
	{"DsliteAddr"},
	{"DsliteName"},
#endif
	{"OrgPD6"},
#endif
	{""},
};

#ifdef TCSUPPORT_PORTBIND
/* support four lan port and four wlan port */
struct bindlist_map bl_map_data[] = 
{	
	{"LAN1", 	"eth0.1"},
	{"LAN2", 	"eth0.2"},
	{"LAN3", 	"eth0.3"},
	{"LAN4", 	"eth0.4"},
	{"SSID1",	"ra0"},
	{"SSID2",	"ra1"},
	{"SSID3",	"ra2"},
	{"SSID4", 	"ra3"},
#ifdef TCSUPPORT_WLAN_AC
	{"USB",		"usb0"},
	{"WDS",		"wds0"},
	{"SSIDAC1",	"rai0"},
	{"SSIDAC2",	"rai1"},
	{"SSIDAC3",	"rai2"},
	{"SSIDAC4",	"rai3"},
#endif
};
#endif

#if defined(TCSUPPORT_CT_WAN_STAT)||defined(TCSUPPORT_CT_PON_CZ_NX)

enum wanIFType_en{
	Ip_type=0,
	Netmask_type,
	Pridns_type,
	Secdns_type, //xyzhu_20091225 support Secondary dns server.
	Mac_type,
	PTP_type,	//get PPP Peer address for gateway
};
#endif



extern int atmcmdc_create_pvc(int itfnum, int vpi, int vci, int encap, int payload,
		int qos_type, int pcr, int scr, int mbs, int sndbuf);
extern int atmcmdc_close_pvc(int vpi, int vci);

#if defined(TCSUPPORT_CT_PON_SC)
/* check ppp username and password whether empty */
int checkPPPUserPSW(mxml_node_t *top, char nodeName[][MAX_NODE_NAME])
{
	char isp[5] = {0};
	char tempbuf[256] = {0};

	if ( NULL == top )
		return 0;

	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "ISP", isp)
		|| 0 != strcmp(isp, PPP_MODE) )
		return 0; // don't check when not ppp mode

	memset(tempbuf, 0, sizeof(tempbuf));
	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "USERNAME", tempbuf)
		|| 0 == tempbuf[0] )
		return -1;
	memset(tempbuf, 0, sizeof(tempbuf));
	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "PASSWORD", tempbuf)
		|| 0 == tempbuf[0] )
		return -1;

	return 0;
}

/* bridge dhcp option check on/off  */
int setBRDHCPChk(mxml_node_t *top)
{
	char idBRDhcpChk[12] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
#define BRDHCPCHK_PROC "/proc/tc3162/br_dhcp_check"
	if ( !top )
		return -1;

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], WAN);
	strcpy(nodeName[1], WAN_COMMON);

	if ( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, "BrDHCPChk", idBRDhcpChk)
		&& 0 == strcmp(idBRDhcpChk, "Yes") )
		doValPut(BRDHCPCHK_PROC, "1");
	else
		doValPut(BRDHCPCHK_PROC, "0");

	return 0;
}

#endif

void add_waninfo_entry(int pvc_index, int entry_index)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	mxml_node_t *parentNode = NULL, *curNode = NULL;
	mxml_node_t *top = get_node_top();
	int if_index = pvc_index * MAX_SMUX_NUM + entry_index;

	parentNode = mxmlFindElement(top, top, WAN_IF_NODE,
				NULL, NULL, MXML_DESCEND);
	if(parentNode == NULL)
		return;
	
	/*Creat wan info entry if not exist*/
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],WAN_IF_NODE);
	sprintf(nodeName[1], "Entry%d", if_index);
	if((curNode = search_node(nodeName)) != NULL)
		mxmlDelete(curNode);
	mxmlNewElement(parentNode,  nodeName[1]);
	
}

void del_waninfo_entry(int pvc_index, int entry_index)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	mxml_node_t *parentNode = NULL, *curNode = NULL;
	mxml_node_t *top = get_node_top();
	int if_index = pvc_index * MAX_SMUX_NUM + entry_index;

	parentNode = mxmlFindElement(top, top, WAN_IF_NODE,
				NULL, NULL, MXML_DESCEND);
	if(parentNode == NULL)
		return;
	
	/*Creat wan info entry if not exist*/
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],WAN_IF_NODE);
	sprintf(nodeName[1], "Entry%d", if_index);
	if((curNode = search_node(nodeName)) != NULL)
		mxmlDelete(curNode);
}

#ifdef TCSUPPORT_QOS
#if defined(TCSUPPORT_CT_QOS)
void update_qos_rule(mxml_node_t *top, int pvc_index, int entry_index)
{	
	char qos_node[MAX_ARG_NUM][MAX_NODE_NAME];
	char qosNode[32];
	char typeName[20], typeVal[20], wan_index[6];
	char MaxName[10], MinName[10], MaxVal[64], MinVal[64];
	int i,j ;
#if defined(TCSUPPORT_CT_DSL_EX)
	char wanCpFlag[8] = {0};
#endif

#if defined(TCSUPPORT_CT_DSL_EX)
	memset(qos_node, 0, sizeof(qos_node));
	strcpy(qos_node[0],	WEB_NODE);
	strcpy(qos_node[1],	ENTRY);
	if( TCAPI_PROCESS_OK == getAttrValue(top, qos_node, WAN_COPY_FLAG, wanCpFlag)
		&& 0 == strcmp("1", wanCpFlag) )
		return;
#endif

	sprintf(wan_index, "%d,%d", pvc_index + 1, entry_index + 1);
		
	memset(qos_node, 0, sizeof(qos_node));
	strcpy(qos_node[0], "QoS");
	for (i = 0; i < MAX_TYPE_ENTRY; i++) {
		sprintf(qos_node[1], "Entry%d", i);
		for (j = 0; j < MAX_TYPE_NUM; j++) {
			sprintf(typeName, "Type%d", j + 1);
			sprintf(MaxName, "Max%d", j + 1);
			sprintf(MinName, "Min%d", j + 1);
			if (getAttrValue(top, qos_node, typeName, typeVal) == TCAPI_PROCESS_OK) {
				if (!strcmp(typeVal, "WANInterface")) {
					if (getAttrValue(top, qos_node, MaxName, MaxVal) == TCAPI_PROCESS_OK && 
						getAttrValue(top, qos_node, MinName, MinVal) == TCAPI_PROCESS_OK) {

						if (!strcmp(MaxVal, wan_index) || !strcmp(MinVal, wan_index)) {

							/* if the interface is delete, set the response attrbuite to N/A again */
							setAttrValue(top, qos_node, typeName, "N/A");
							setAttrValue(top, qos_node, MaxName, "N/A");
							setAttrValue(top, qos_node, MinName, "N/A");

							/* commit QoS_Entry{i} again */
							sprintf(qosNode, "QoS_Entry%d", i);
							tcapi_set_internal(top, qosNode, "QoSOptType", "typeRule");
							tcapi_commit_internal(top, qosNode);
						}
					}
				}
			}
		}
	}
	
	return;
}
#endif
#endif
void add_dmz_entry(int pvc_index, int entry_index, char *if_name)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	mxml_node_t *top = get_node_top();
	char subNode[9] = {0};
	mxml_node_t *node = NULL, *psubnode = NULL;
	int if_index = pvc_index * MAX_SMUX_NUM + entry_index;

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],"Dmz");
	sprintf(nodeName[1],"%s%d", SUB_NODE_NAME, if_index);
#ifdef WAN_DBUG
	tcdbg_printf("add_dmz_entry:%s_%s (ifname=%s)\n", nodeName[0], nodeName[1], if_name);
#endif
	sprintf(subNode,"%s","Dmz");
	node = mxmlFindElement(top, top, subNode,
		NULL, NULL, MXML_DESCEND);
	if(node){
		memset(subNode, 0, sizeof(subNode));
		sprintf(subNode,"%s%d",SUB_NODE_NAME, if_index);
		psubnode = mxmlFindElement(node, node, subNode,
			NULL, NULL, MXML_DESCEND);
		if(psubnode){
			#ifdef WAN_DBUG
				tcdbg_printf("add_dmz_entry:Dmz_%s is exited,just update!\n", subNode);
			#endif
				//setAttrValue(top, nodeName, "IFName", if_name);
				return;
		}
		else{
			mxmlNewElement(node, subNode);				
			setAttrValue(top, nodeName, "IFName", if_name);
			setAttrValue(top, nodeName, "Active", "No");
			setAttrValue(top, nodeName, "DMZ_IP", "0.0.0.0");
			setAttrValue(top, nodeName, "IPMODE", "Single");
		}
	}

	return;
}

void del_dmz_entry(int pvc_index, int entry_index, char *if_name)
{
	char subNode[9] = {0};
	mxml_node_t *top = get_node_top();
	mxml_node_t *node = NULL;
	int if_index = pvc_index * MAX_SMUX_NUM + entry_index;

	sprintf(subNode,"%s","Dmz");
	node = mxmlFindElement(top, top, subNode,
		NULL, NULL, MXML_DESCEND);
	if(node){
		memset(subNode, 0, sizeof(subNode));
		sprintf(subNode,"%s%d",SUB_NODE_NAME, if_index);
		node = mxmlFindElement(node, node, subNode,
			NULL, NULL, MXML_DESCEND);
		if(node){
			#ifdef WAN_DBUG
				tcdbg_printf("del_dmz_entry:Dmz_%s \n", subNode);
			#endif
				mxmlDelete(node);
		}
	}
	return;
}

void add_vserver_entry(int pvc_index, int entry_index, char *if_name)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	mxml_node_t *top = get_node_top();
	char subNode[9] = {0};
	mxml_node_t *node = NULL, *psubnode = NULL;
	int if_index = pvc_index * MAX_SMUX_NUM + entry_index;

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],"VirServer");
	sprintf(nodeName[1],"%s%d", SUB_NODE_NAME, if_index);	
#ifdef WAN_DBUG
	tcdbg_printf("add_vserver_entry:%s_%s (ifname=%s)\n", nodeName[0], nodeName[1], if_name);
#endif

	sprintf(subNode,"%s","VirServer");
	node = mxmlFindElement(top, top, subNode,
		NULL, NULL, MXML_DESCEND);
	if(node){
		memset(subNode, 0, sizeof(subNode));
		sprintf(subNode,"%s%d",SUB_NODE_NAME, if_index);
		psubnode = mxmlFindElement(node, node, subNode,
			NULL, NULL, MXML_DESCEND);
		if(psubnode){
			#ifdef WAN_DBUG
				tcdbg_printf("add_vserver_entry:Virserver_%s is exited,just update!\n", subNode);
			#endif
				//setAttrValue(top, nodeName, "IFName", if_name);
				return;
		}
		else{
			mxmlNewElement(node, subNode);	
			setAttrValue(top, nodeName, "IFName", if_name);
			setAttrValue(top, nodeName, "IPMODE", "Single");
		}
	}


	return;
}

void del_vserver_entry(int pvc_index, int entry_index, char *if_name)
{
	char subNode[9] = {0};
	mxml_node_t *top = get_node_top();
	mxml_node_t *node = NULL;
	int if_index = pvc_index * MAX_SMUX_NUM + entry_index;

	sprintf(subNode,"%s", "VirServer");
	node = mxmlFindElement(top, top, subNode,
		NULL, NULL, MXML_DESCEND);
	if(node){
		memset(subNode, 0, sizeof(subNode));
		sprintf(subNode,"%s%d",SUB_NODE_NAME, if_index);
		node = mxmlFindElement(node, node, subNode,
			NULL, NULL, MXML_DESCEND);
		if(node){
			#ifdef WAN_DBUG
				tcdbg_printf("del_vserver_entry:VirServer_%s \n", subNode);
			#endif
				mxmlDelete(node);
		}
	}
	return;
}

void del_static_route(char *if_name)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	int i = 0;
	mxml_node_t *curNode = NULL;
	const char *value=NULL;

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],ROUTE);
	for(i = 0; i < MAX_STATIC_ROUTE_NUM; i++){
		sprintf(nodeName[1], "Entry%d", i);
		if((curNode = search_node(nodeName)) != NULL){
			value = mxmlElementGetAttr(curNode, "Device");

			if(value && !strcmp(if_name, value)){
				mxmlDelete(curNode);
			}
		}
	}

#ifdef IPV6
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],ROUTE6);
	for(i = 0; i < MAX_STATIC_ROUTE6_NUM; i++){
		sprintf(nodeName[1], "Entry%d", i);
		if((curNode = search_node(nodeName)) != NULL){
			value = mxmlElementGetAttr(curNode, "Device");

			if(value && !strcmp(if_name, value)){
				mxmlDelete(curNode);
			}
		}
	}
#endif
}

#if defined(TCSUPPORT_CT_2NTP)
#if defined(TCSUPPORT_CT_NTPSERVERTYPE)
#else
void change_ntp_route(char *if_name,char *new_if_name)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	mxml_node_t *curNode = NULL;
	const char *value=NULL;

	tcdbg_printf("change_ntp_route: old if name is [%s]\n", if_name);
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], TIMEZONE);
	strcpy(nodeName[1], SUB_NODE_NAME);
	if((curNode = search_node(nodeName)) != NULL){
		value = mxmlElementGetAttr(curNode, TIMEZONE_ATTR_ADDROUTE);
		if(value && !strcmp(if_name, value)){
			mxmlElementSetAttr(curNode,  TIMEZONE_ATTR_ADDROUTE, new_if_name);
			tcdbg_printf("change_ntp_route: matched, set new if name is [%s]\n", new_if_name);
		}
	}
}
#endif
#endif

void del_igmp_mld_proxy(char *if_name)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	int i = 0;
	mxml_node_t *curNode = NULL;
	const char *value=NULL;

	tcdbg_printf("del_igmp_mld_proxy: if name is [%s]\n", if_name);
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], "IGMPproxy");
	strcpy(nodeName[1], "Entry");
	if((curNode = search_node(nodeName)) != NULL){
		value = mxmlElementGetAttr(curNode, "UpstreamIF");
		if(value && !strcmp(if_name, value)){
			mxmlElementSetAttr(curNode,  "UpstreamIF", "");
			mxmlElementSetAttr(curNode,  "Active", "No");
				/*kill igmpproxy process*/
			system("killall igmpproxy");
			tcdbg_printf("del_igmp_mld_proxy: igmp proxy matched set deactive\n");
		}
	}

#ifdef IPV6
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], "MLDproxy");
	strcpy(nodeName[1], "Entry");
	if((curNode = search_node(nodeName)) != NULL){
		value = mxmlElementGetAttr(curNode, "UpstreamIF");
		if(value && !strcmp(if_name, value)){
			mxmlElementSetAttr(curNode,  "UpstreamIF", "");
			mxmlElementSetAttr(curNode,  "Active", "No");
			/*kill mld proxy process*/
			system("killall ecmh");			
			tcdbg_printf("del_igmp_mld_proxy: mld proxy matched set deactive\n");
		}
	}
#endif	
}

/*______________________________________________________________________________
**	wan_check_filter
**
**	descriptions:
**		check if the wan need filter module or not.
**
**	parameters:
**		top: 		top of node tree.
**
**	return:
**		filter_on: 0 means not
**		filter_on: 1 means yes
**____________________________________________________________________________
*/
int
wan_check_filter(mxml_node_t *top){
	int filter_on = 0, i;
	char active[4]={0}, isp[4]={0};

	for(i = 0; i < MAX_WAN_IF_INDEX; i++){
		if(get_waninfo_by_index(i, "Active", active) == SUCCESS &&
			get_waninfo_by_index(i, "ISP", isp) == SUCCESS){
			/*interface is active and is not bridge mode*/
			if(!strcmp(active, "Yes") && strcmp(isp, BRIDGE_MODE)){
				filter_on = 1;
				break;
			}
		}
			
	}
	return filter_on;
}

/*______________________________________________________________________________
**	verify_interface
**
**	descriptions:
**		verify interface node is valid or not
**	parameters:
**		
**	return:
**		valid: 	0
**		unvalid:     -1
**_______________________________________________________________________________
*/
int verify_interface(mxml_node_t *top, char name[][MAX_NODE_NAME])
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], entry[MAX_NODE_NAME];
	char encap[32] = {0}, isp[5] = {0};
	mxml_node_t *curNode = NULL;
	mxml_node_t *childNode = NULL;
	int i = 0;

	memset(nodeName, 0, sizeof(nodeName));
  	strcpy(nodeName[0],name[0]);
  	strcpy(nodeName[1], name[1]);

	/*PPPoA mode,only one entry allowed to existed when PVC is PPPoA type*/
	if(getAttrValue(top, nodeName, "ENCAP", encap) != TCAPI_PROCESS_OK ){
		return FAIL;
	}
	
	
	if(!strcmp(encap, ENCAP_PPPOA_LLC) || !strcmp(encap, ENCAP_PPPOA_VC)){
	#if 0
		curNode = mxmlFindElement(top, top, nodeName[1],
			NULL, NULL, MXML_DESCEND);
	#else
		curNode = mxmlFindElement(top, top, nodeName[0],
			NULL, NULL, MXML_DESCEND_FIRST);
		if (curNode == NULL) {
			tcdbg_printf("verify_interface:can not find node %s!\n", nodeName[0]);
			return FAIL;
		}
		else {
			curNode = mxmlFindElement(curNode, curNode, nodeName[1],
			NULL, NULL, MXML_DESCEND);
		}
	#endif
		
		if(curNode == NULL){
			tcdbg_printf("verify_interface:can not find node %s!\n", nodeName[1]);
			return FAIL;
		}
		else{
			/*Wan_PVCx_Entry0 is a unique valid entry for PPPoA PVC*/
			for(i = 1; i < MAX_SMUX_NUM; i++){
				sprintf(entry, "%s%d", WAN_INTERFACE, i);
				childNode = mxmlFindElement(curNode, curNode, entry,
					NULL, NULL, MXML_DESCEND);
				if(childNode){
					return FAIL;
				}
			}
		}
		/*Verify Wan_PVCx_Entry0 if existed*/
		sprintf(nodeName[2], "%s%d", WAN_INTERFACE, 0);
		childNode = mxmlFindElement(curNode, curNode, nodeName[2],
					NULL, NULL, MXML_DESCEND);
		if(childNode){			
			if(getAttrValue(top, nodeName, "ISP", isp) != TCAPI_PROCESS_OK ){
				return FAIL;
			}
			/*Wan_PVCx_Entry0 should be ppp mode*/
			if(strcmp(isp, PPP_MODE)){
				return FAIL;
			}
		}
	}
	
	return SUCCESS;
}

/*______________________________________________________________________________
**	create_dhcp6c_conf
**
**	descriptions:
**		create dhcp6c.conf for DHCPv6 client
**	parameters:
**		ifname:wan interface name
**		PD: 1 means enable request prefix delegation
**		NA: 1 means enable request non-temporary address
**		
**	return:
**		Success: 	0
**		Otherwise:     -1
**	dhcp6c.conf:
**	interface $WAN_IF {
**		send ia-pd 213;
**		send ia-na 210;
**		request domain-name;
**		request domain-name-servers;
**		script "/usr/script/dhcp6c_script";
**	};
**	id-assoc pd 213 {
**		prefix-interface br0
**		{
**			sla-id 1;
**		};
**	};
**	id-assoc na 210 { };
**_______________________________________________________________________________
*/
#ifdef IPV6
#if defined(TCSUPPORT_CT_DSLITE)
int create_dhcp6c_conf(char *ifname, int PD, int NA, int DSLite)
#else
int create_dhcp6c_conf(char *ifname, int PD, int NA)
#endif
{
	FILE *fp=NULL;
	char fpath[64], buf[64] = {0};

	sprintf(fpath, "%s%s/dhcp6c.conf", WAN_IF_PATH, ifname);
	fp = fopen(fpath,"w");
	if(fp){
		sprintf(buf, "interface %s {\n", ifname);
		fputs(buf, fp);
		if(PD)
			fputs("\tsend ia-pd 213;\n", fp);
		if(NA){
			fputs("\tsend ia-na 210;\n", fp);
		}
#if defined(TCSUPPORT_CT_DHCP6_OPTION)
		fputs("\trequest domain-name;\n\trequest domain-name-servers;\n\tscript \"/usr/script/dhcp6c_script\";\n\trequest vendor_opts;\n", fp);
#else
		fputs("\trequest domain-name;\n\trequest domain-name-servers;\n\tscript \"/usr/script/dhcp6c_script\";\n", fp);
#endif
#if defined(TCSUPPORT_CT_DSLITE)
		if( DSLite )
			fputs("\trequest dslite-name;\n", fp);
#endif
		fputs("};\n", fp);
		if(PD){
			fputs("id-assoc pd 213 {\n\tprefix-interface br0\n\t{\n\t\tsla-len 0;\n\t};\n};\n", fp);
		}
		if(NA){
			fputs("id-assoc na 210 { };\n", fp);
		}
		fclose(fp);
			
	}
	else{
		return FAIL;
	}
	
	return SUCCESS;
}
#endif
#if defined(TCSUPPORT_CT_PHONEAPP)
int phoneapp_node_write(mxml_node_t *node, char *file, char *username, char *password){
	FILE *fp=NULL;
	char buf[MAXLEN_TCAPI_MSG + 128]={0};
	int       i=0;          /* Looping var */
	mxml_attr_t   *attr=NULL;          /* Current attribute */

	if(node == NULL){
		unlink(file);
		return FAIL;
	}
	else{
		fp = fopen(file,"w");
		if(fp == NULL){
			return FAIL;
		}
		else{
			attr = node->value.element.attrs;
			for(i = 0; i < node->value.element.num_attrs; i++ ){
				if( strcmp(attr->name, "USERNAME")==0 )
					sprintf(buf,"%s=\"%s\"\n", attr->name, username);
				else if( strcmp(attr->name, "PASSWORD")==0 )
					sprintf(buf,"%s=\"%s\"\n", attr->name, password);
				else
					sprintf(buf,"%s=\"%s\"\n", attr->name, attr->value);

				fputs(buf, fp);
				attr ++;
			}
		}
		fclose(fp);
		/* If this node hasn't any attribute, we need to remove this file*/
		if(i == 0){
			unlink(file);
		}
	}
	return SUCCESS;
}
#endif
/*______________________________________________________________________________
**	create_interface_conf
**
**	descriptions:
**		create interface conf,including pvc.conf and interface.conf, which will be putted in /var/run/interface 
**		name folder
**	parameters:
**		name: wan_pvc_entry node name
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int create_interface_conf(char name[][MAX_NODE_NAME])
{
	char iffolder[32] = {0}, ifname[16] = {0}, string[64] = {0};
	mxml_node_t *node = NULL;
	mxml_node_t *top = get_node_top();
#ifdef IPV6
	char ipversion[16] = {0}, isp[5] = {0};
	int pd_enable = 0, na_enable = 0;
#if defined(TCSUPPORT_CT_DSLITE)
	int dslite = 0;
#endif
#endif
#if defined(TCSUPPORT_CT_PPP_ONDEMAND)
	char idletime[32] = {0};
#endif
#if defined(TCSUPPORT_CT_PHONEAPP)
	char name_web[MAX_ARG_NUM][MAX_NODE_NAME];
	char pppdialval[10] = {0};
	char usernameval[256] = {0};
	char passwrdval[256] = {0};
#endif

			if(getAttrValue(top, name, WAN_IFNAME, ifname) != TCAPI_PROCESS_OK ){
				return FAIL;
			}
			/*create wan interface folder by interface name,ext:/var/run/nas0_1*/
			sprintf(iffolder, "%s%s", WAN_IF_PATH, ifname);
			if(mkdir(iffolder, 0777) != 0){
				tcdbg_printf("create_interface_conf create folder failed!\n" );
				return FAIL;
			}

	/*create pvc.conf*/
	sprintf(string, "%s/%s", iffolder, "pvc.conf");
	node = mxmlFindElement(top, top, name[0],
	  		NULL, NULL,
	  		MXML_DESCEND);
	node = mxmlFindElement(node, node, name[1],
	  		NULL, NULL,
	  		MXML_DESCEND);
	if(node_write(node, string, QMARKS) != 0)
		return FAIL;

	/*create interface.conf*/
#if defined(TCSUPPORT_CT_PPP_ONDEMAND)
	if ( TCAPI_PROCESS_OK != getAttrValue(top, name, "CLOSEIFIDLE", idletime) )
#if defined(TCSUPPORT_CT_PON_CZ_QH)
	setAttrValue(top, name, "CLOSEIFIDLE", "900");
#else
#if defined(TCSUPPORT_CT_ADSL_HN)
		setAttrValue(top, name, "CLOSEIFIDLE", "600");
#else
		setAttrValue(top, name, "CLOSEIFIDLE", "1800");// default idle time to 30min
#endif
#endif
#endif

	sprintf(string, "%s/%s", iffolder, "interface.conf");
	node = mxmlFindElement(node, node, name[2],
	  		NULL, NULL,
	  		MXML_DESCEND);
#if defined(TCSUPPORT_CT_PHONEAPP)
	memset(name_web, 0, sizeof(name_web));
	strcpy(name_web[0], "WebCustom");
	strcpy(name_web[1], "Entry");
	if((getAttrValue(top, name_web, "pppdialflag", pppdialval) == TCAPI_PROCESS_OK )
		&& (strcmp(pppdialval, ifname) == 0)
		&& (getAttrValue(top, name_web, "pppdialusername", usernameval) == TCAPI_PROCESS_OK )
		&& (getAttrValue(top, name_web, "pppdialpassword", passwrdval) == TCAPI_PROCESS_OK ))
	{
		if(phoneapp_node_write(node, string, usernameval, passwrdval) != 0)
			return FAIL;
	}
	else
	{
#endif
	if(node_write(node, string, QMARKS) != 0)
		return FAIL;
#if defined(TCSUPPORT_CT_PHONEAPP)
	}
#endif
	
#ifdef IPV6
	if(getAttrValue(top, name, "IPVERSION", ipversion) != TCAPI_PROCESS_OK ){
		return FAIL;
	}
	memset(string, 0, sizeof(string));
	if(!strcmp(ipversion, "IPv4/IPv6") || !strcmp(ipversion, "IPv6")){
		if(getAttrValue(top, name, "ISP", isp) != TCAPI_PROCESS_OK ){
			return FAIL;
		}
#if defined(TCSUPPORT_CT_DSLITE)
		dslite = get_dslite_stateviacfg(top, name);
#endif

		if(!strcmp(isp, DYNMIC_MODE) || !strcmp(isp, PPP_MODE)){
			if(getAttrValue(top, name, "DHCPv6", string) != TCAPI_PROCESS_OK )
				return FAIL;
			if(!strcmp(string, ACTIVE))
				na_enable = 1;
			if(getAttrValue(top, name, "DHCPv6PD", string) != TCAPI_PROCESS_OK )
				return FAIL;
			if(!strcmp(string, ACTIVE))
				pd_enable = 1;
#if defined(TCSUPPORT_CT_DSLITE)
			create_dhcp6c_conf(ifname, pd_enable, na_enable, dslite);
#else
			create_dhcp6c_conf(ifname, pd_enable, na_enable);
#endif
		}
	}
#endif
#ifdef WAN_DBUG
	tcdbg_printf("create_interface_conf done!!!\n" );
#endif
	return SUCCESS;	
}

#if defined(TCSUPPORT_CT_WAN_CHILD_PREFIX)
/*******************************************************************************************
**function name
	set_childprefx_file
**description:
	save information to childprefix file
 **retrun 
	0:success
	-1:failure
**parameter:
	ifname: interface name
	pvc_index: pvc index
	entry_index: entry index
********************************************************************************************/
int set_childprefx_file(int pvc_index, int entry_index, char *ifname){
	int ret = -1;
	char tmp_str[64] = {0}, path[64]={0};
	char child_prefix[64]={0}, child_orign_str[16] = {0};
	char *tmp_p = NULL;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	mxml_node_t *top = get_node_top();
	FILE *fp = NULL;
	int child_orign = PREFIX_ORIGN_DFT;
	int wanInfo_entry_idx = 0;	
	char tmpBuf[512]={0};
	char macInfo[2][18];

	/*init*/
	if(ifname == NULL){
		goto end;
	}
	memset(macInfo,0, sizeof(macInfo));
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],WAN);
  	sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
	sprintf(nodeName[2], "%s%d", WAN_INTERFACE, entry_index);

	/*1.get child prefix*/	
	if(getAttrValue(top, nodeName, "ChildPrefixBits", child_prefix) != TCAPI_PROCESS_OK){
		goto end;
	}

	/*2. get child prefix orgin*/
	if(getAttrValue(top, nodeName, "DHCPv6", tmp_str) != TCAPI_PROCESS_OK){
		goto end;
	}
	if(strcmp(tmp_str,"Yes")==0){
		child_orign = PREFIX_ORIGN_DHCP;
	}else if(strcmp(tmp_str,"No")==0){
		child_orign = PREFIX_ORIGN_SLLA;
	}else if(strcmp(tmp_str,"N/A")==0){
		child_orign = PREFIX_ORIGN_STATIC;
	}else{
		child_orign = PREFIX_ORIGN_DFT;
	}
	/*3. get mac adddr from ifconfig*/
	sprintf(tmp_str,"ifconfig nas%d_%d > %s", pvc_index, entry_index, TMP_PREFIX_IF_PATH);
	system(tmp_str);	
	if((fp=fopen(TMP_PREFIX_IF_PATH, "r"))==NULL){
		goto end;
	}
	while (fgets (tmpBuf, sizeof(tmpBuf), fp) != NULL){
		/*krammer add*/
		if((tmp_p=strstr(tmpBuf,"HWaddr"))){
			/*tmpBuf format: HWaddr 00:AA:BB:01:23:45*/
			sscanf(tmp_p,"%s %s", macInfo[0],macInfo[1]);
			break;
		}
	}
	fclose(fp);
	
	/*write info to file*/
	/*1.write mac info*/
	sprintf(path, "/var/run/%s/child_prefix_hwaddr", ifname);
	if((fp = fopen(path, "w")) == NULL){
		goto end;
	}	
	fputs(macInfo[1], fp);
	fclose(fp);
	fp = NULL;

	/*2.return in ppp*/
	if(strstr(ifname,"ppp") != NULL){		
		ret = 0;
		goto end;
	}
	/*3.write child prefix info*/
	sprintf(path, "/proc/sys/net/ipv6/conf/%s/child_prefix", ifname);
	if((fp = fopen(path, "w")) == NULL){
		goto end;
	}
	fputs(child_prefix, fp);
	fclose(fp);
	fp = NULL;
	/*4.write child prefix orign*/
	sprintf(path, "/proc/sys/net/ipv6/conf/%s/child_prefix_orign", ifname);
	if((fp = fopen(path, "w")) == NULL){ 
		goto end;
	}
	sprintf(child_orign_str, "%d",child_orign);
	fputs(child_orign_str, fp);
	fclose(fp);
	fp = NULL;

	ret = 0;
end:
	return ret;

}
#endif
#if defined(TCSUPPORT_CT_DHCP6C_STATUS_CODE)
/*******************************************************************************************
**function name
	set_dhcp6c_error_sw
**description:
	save switch value from romfile to file for dhcp6c handling status code.
 **retrun 
	0:success
	-1:failure
**parameter:
	top: romfile top pointer
********************************************************************************************/
int set_dhcp6c_error_sw(mxml_node_t *top){
	int ret = -1;
	int sw = DISABLE_DHCP6C_ERROR_HANDLE;
	char tmp_str[64] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	FILE *fp = NULL;

	if(top == NULL){
		goto end;
	}
	
	/*1.get switch value for handling the dhcp6c status code*/	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],WAN);
  	strcpy(nodeName[1], WAN_COMMON);
	if(getAttrValue(top, nodeName, "Dhcp6cErrorSw", tmp_str) != TCAPI_PROCESS_OK){
		goto end;
	}

	sw = atoi(tmp_str);
	if(sw != ENABLE_DHCP6C_ERROR_HANDLE){
		sw = DISABLE_DHCP6C_ERROR_HANDLE;
	}
	
	/*2. save switch value to file*/
	sprintf(tmp_str,"%d\n",sw);
	if((fp=fopen(DHCP6C_HANDLE_STATUS_CODE_PATH, "w"))==NULL){
		goto end;
	}
	fputs(tmp_str, fp);
	fclose(fp);

	ret = 0;
end:
	return ret;

}
#endif
#if defined(TCSUPPORT_CT_PPP_ONDEMAND)
extern void set_ondemand_route_state(int if_index, int state);
void set_portbinding_info_for_ondemand_wan(mxml_node_t *top,
									int pvc_index, int entry_index,
									char *ifname)
{
	int idx = 0;
	char bl_buf[12] = {0};
	char portbind_cmd[128] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char connection[32] = {0};
	char ispMode[12] = {0};
	char ip_version[20] = {0};
	int ipVersion = 0;

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], WAN);
  	sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
	sprintf(nodeName[2], "%s%d", WAN_INTERFACE, entry_index);

	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "ISP", ispMode)
		|| 0 != strcmp(ispMode, PPP_MODE) )
		return -1;
	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "CONNECTION", connection)
		|| 0 != strcmp(connection, "Connect_on_Demand") )
		return -1;
	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "IPVERSION", ip_version) )
		return -1;

	if ( 0 == strcmp(ip_version, "IPv4") )
		ipVersion = 0;
	else if ( 0 == strcmp(ip_version, "IPv6") )
		ipVersion = 1;
	else if ( 0 == strcmp(ip_version, "IPv4/IPv6") )
		ipVersion = 2;
	else
		return -1;

	for (idx = 0; idx < sizeof(bl_map_data) / sizeof(bl_map_data[0]); idx++)
	{
		if ( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, bl_map_data[idx].bindif, bl_buf) )
		{
			if ( 0 == strcmp(bl_buf, "Yes"))
			{
				if ( 0 == ipVersion || 2 == ipVersion )
				{
					sprintf(portbind_cmd, "portbindcmd addroute v4 %s %s ''", bl_map_data[idx].realif, ifname);
					wan_cmd_excute(portbind_cmd);
				}
#ifdef IPV6
				if ( 1 == ipVersion || 2 == ipVersion )
				{
					sprintf(portbind_cmd, "portbindcmd addroute v6 %s %s ''", bl_map_data[idx].realif, ifname);
					wan_cmd_excute(portbind_cmd);
				}
#endif
			}
		}
	}
}
int ClearOndemandWanHisAddr(mxml_node_t *top,
									int pvc_index, int entry_index)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	int if_index = -1;

	if ( NULL == top)
		return -1;

	if_index = pvc_index * MAX_SMUX_NUM + entry_index;

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], WAN_IF_NODE);
	sprintf(nodeName[2], "Entry%d", if_index);
	setAttrValue(top, nodeName, "OndemandHisAddr", "N/A");

	return 0;
}
int isOndemandWan(mxml_node_t *top,
						int pvc_index, int entry_index)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char connection[32] = {0};
	char ispMode[12] = {0};

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], WAN);
  	sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
	sprintf(nodeName[2], "%s%d", WAN_INTERFACE, entry_index);

	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "ISP", ispMode)
		|| 0 != strcmp(ispMode, PPP_MODE) )
		return 0;
	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "CONNECTION", connection)
		|| 0 != strcmp(connection, "Connect_on_Demand") )
		return 0;

	return 1;
}
int AddRouteforOnDemandWan(mxml_node_t *top,
						  int pvc_index, int entry_index)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char cmdbuf[128] = {0};
	char connection[32] = {0};
	char ispMode[12] = {0};
	char ifidxbuf[12] = {0};
	char serviceList[64] = {0};
	char ifname[32] = {0};
	char ip_version[20] = {0};
	int ipVersion = 0;
	int isInternet = 0;
	int if_index = -1;
	char def_route_index_v4[10] = {0};
	int i_def_route_index_v4 = -1;
	char def_route_index_v6[10] = {0};
	int i_def_route_index_v6 = -1;
	char ondemand_route_index_v4[10] = {0};
	int i_ondemand_route_index_v4 = -1;
	char ondemand_route_index_v6[10] = {0};
	int i_ondemand_route_index_v6 = -1;

	if ( NULL == top)
		return -1;

	if_index = pvc_index * MAX_SMUX_NUM + entry_index;
	sprintf(ifidxbuf, "%d", if_index);

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], WAN);
  	sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
	sprintf(nodeName[2], "%s%d", WAN_INTERFACE, entry_index);

	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "ISP", ispMode)
		|| 0 != strcmp(ispMode, PPP_MODE) )
		return -1;
	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "CONNECTION", connection)
		|| 0 != strcmp(connection, "Connect_on_Demand") )
		return -1;
	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "IPVERSION", ip_version) )
		return -1;
	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "IFName", ifname)
		|| '\0' == ifname[0] )
		return -1;
	if( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, "ServiceList", serviceList)
		&& NULL != strstr(serviceList, "INTERNET"))
		isInternet = 1;

	if ( 0 == strcmp(ip_version, "IPv4") )
		ipVersion = 0;
	else if ( 0 == strcmp(ip_version, "IPv6") )
		ipVersion = 1;
	else if ( 0 == strcmp(ip_version, "IPv4/IPv6") )
		ipVersion = 2;
	else
		return -1;

	// get default route index for V4&V6
	memset(def_route_index_v4, 0, sizeof(def_route_index_v4));
	memset(def_route_index_v6, 0, sizeof(def_route_index_v6));
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], WAN_IF_NODE);
	strcpy(nodeName[1], WAN_IF_COMMON);
	if ( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, WAN_COMMON_DEFROUTEIDXV4, def_route_index_v4)
		&& '\0' != def_route_index_v4[0] && 0 != strcmp(def_route_index_v4, "N/A") )
		i_def_route_index_v4 = atoi(def_route_index_v4);
	if ( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, WAN_COMMON_DEFROUTEIDXV6, def_route_index_v6)
		&& '\0' != def_route_index_v6[0] && 0 != strcmp(def_route_index_v6, "N/A") )
		i_def_route_index_v6 = atoi(def_route_index_v6);

	// get on-demand wan connection index for V4&V6
	memset(ondemand_route_index_v4, 0, sizeof(ondemand_route_index_v4));
	memset(ondemand_route_index_v6, 0, sizeof(ondemand_route_index_v6));
	if ( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, WAN_COMMON_ONDEMANDIDXV4, ondemand_route_index_v4)
		&& '\0' != ondemand_route_index_v4[0] && 0 != strcmp(ondemand_route_index_v4, "N/A") )
		i_ondemand_route_index_v4 = atoi(ondemand_route_index_v4);
	if ( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, WAN_COMMON_ONDEMANDIDXV6, ondemand_route_index_v6)
		&& '\0' != ondemand_route_index_v6[0] && 0 != strcmp(ondemand_route_index_v6, "N/A") )
		i_ondemand_route_index_v6 = atoi(ondemand_route_index_v6);

	if ( 1 == isInternet
		&& ( 0 == ipVersion || 2 == ipVersion )
		&& -1 == i_def_route_index_v4
		&& ( -1 == i_ondemand_route_index_v4 || ( i_ondemand_route_index_v4 >= 0 && if_index == i_ondemand_route_index_v4 ) ))
	{
		if ( i_ondemand_route_index_v4 >= 0 && if_index == i_ondemand_route_index_v4 )
		{
			sprintf(cmdbuf, "/sbin/route del default dev %s", ifname);
			wan_cmd_excute(cmdbuf);
		}
		sprintf(cmdbuf, "/sbin/route add default dev %s", ifname);
		wan_cmd_excute(cmdbuf);
		setAttrValue(top, nodeName, WAN_COMMON_ONDEMANDIDXV4, ifidxbuf);
	}
#ifdef IPV6
	if ( 1 == isInternet
		&& ( 1 == ipVersion || 2 == ipVersion )
		&& -1 == i_def_route_index_v6
		&& ( -1 == i_ondemand_route_index_v6 || ( i_ondemand_route_index_v6 >= 0 && if_index == i_ondemand_route_index_v6 ) ))
	{
		if ( i_ondemand_route_index_v6 >= 0 && if_index == i_ondemand_route_index_v6 )
		{
			sprintf(cmdbuf, "/sbin/route -A inet6 del default dev %s", ifname);
			wan_cmd_excute(cmdbuf);
		}
		sprintf(cmdbuf, "/sbin/route -A inet6 add default dev %s", ifname);
		wan_cmd_excute(cmdbuf);
		setAttrValue(top, nodeName, WAN_COMMON_ONDEMANDIDXV6, ifidxbuf);
	}
#endif

	set_portbinding_info_for_ondemand_wan(top, pvc_index, entry_index, ifname);

	return 0;
}
int delRoute6ViaOndemand(mxml_node_t *top,
						int pvc_index, int entry_index, char *ifname)
{
	char ondemand_route_index_v6[10] = {0};
	int i_ondemand_route_index_v6 = -1;
	int if_index = -1;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char cmdbuf[128] = {0};
	char isDelAttr[32] = {0};

	if_index = pvc_index * MAX_SMUX_NUM + entry_index;

	if ( 1 != isOndemandWan(top, pvc_index, entry_index) )
		return -1;

	memset(ondemand_route_index_v6, 0, sizeof(ondemand_route_index_v6));
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], WAN_IF_NODE);
	strcpy(nodeName[1], WAN_IF_COMMON);

	if ( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, WAN_COMMON_ONDEMANDIDXV6, ondemand_route_index_v6)
		&& '\0' != ondemand_route_index_v6[0] && 0 != strcmp(ondemand_route_index_v6, "N/A") )
		i_ondemand_route_index_v6 = atoi(ondemand_route_index_v6);

	if ( (-1 == i_ondemand_route_index_v6)
		|| (i_ondemand_route_index_v6 != if_index) )
		return -1;

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], "WanInfo");
	sprintf(nodeName[1], "Entry%d", if_index);

	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "isOndemandv6Del", isDelAttr) )
	{
		setAttrValue(top, nodeName, "isOndemandv6Del", "1");
		sprintf(cmdbuf, "/sbin/route -A inet6 del default dev %s", ifname);
		wan_cmd_excute(cmdbuf);
	}

	return 0;
}
int addDnsRoute6forOtherWan(char *ifname, char *ifsvrapp, char *hisaddrV6)
{
	char cmdbuf[128] = {0};

	if ( NULL == ifname || NULL == ifsvrapp || NULL == hisaddrV6 )
		return -1;

	if ( NULL == strstr(ifsvrapp, "INTERNET") )
	{
		sprintf(cmdbuf, "/usr/bin/ip -6 route add %s dev %s", hisaddrV6, ifname);
		wan_cmd_excute(cmdbuf);
	}

	return 0;
}
#endif

/*______________________________________________________________________________
**	start_interface
**
**	descriptions:
**		make a interface work
**	parameters:
**		pvc_index: interface pvc(0~7)
**		ifname: interface name,ext:nas0_1
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int start_interface(int pvc_index, int entry_index, char *ifname)
{
	char cmd[64];
	int start_pvc = 0;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char str_qos[12] = {0}, str_encap[30] = {0}, str_vpi[5] = {0}, str_vci[5] = {0}, str_pcr[7] = {0}, str_scr[7] = {0}, str_mbs[10] = {0};
	mxml_node_t *top = get_node_top();
	int vpi, vci, encap, payload = 1, qos_type = 1, pcr = 0, scr = 0, mbs = 0;
#if 0 //move to wan_monitor	
	char dmz_node[32];
#endif	
	int if_index = pvc_index * MAX_SMUX_NUM + entry_index;
#if defined(TCSUPPORT_CT_PON)
	char xPONLinkStaAttr[8] = {0};
	int linkSta = 0;
	char gponEnable[8] = {0};
	char eponEnable[8] = {0};
#endif

#if defined(TCSUPPORT_CT_WAN_PTM)
	int transMode = 0;
	int barrier = 0;
	char str_barrier[4] = {0},str_PTMWan[8] = {0};
#endif
 
	if(pvc_index < 0 || pvc_index > PVC_NUM){
		tcdbg_printf("start_interface:pvc index is out of range\n" );
		return FAIL;
	}
	/*Creat interface config file*/
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],WAN);
  	sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
	sprintf(nodeName[2], "%s%d", WAN_INTERFACE, entry_index);
	if(create_interface_conf(nodeName) != SUCCESS)
		return FAIL;
	
	/* check pvc reference number*/
	start_pvc = (g_pvc_refcnt[pvc_index] == 0) ? 1 : 0;
	
	tcdbg_printf("start_interface:start_pvc=%d , g_pvc_refcnt[pvc_index]=%d\n", start_pvc, g_pvc_refcnt[pvc_index]);
#if defined(TCSUPPORT_CT_WAN_PTM)
	transMode = getWanTransModeType(pvc_index);
	if(transMode & 0x2 ) {		//ptm
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0],WAN);
		sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);

		
		getAttrValue(top, nodeName, "PTMEnable", str_PTMWan);
		if(!strcmp(str_PTMWan, "Yes")){//PTM wan		
			sprintf(nodeName[2], "%s%d", WAN_INTERFACE, entry_index);
			getAttrValue(top, nodeName, "Barrier", str_barrier); 
			setAttrValue(top, nodeName, "preBarrier", str_barrier); 
			barrier = atoi(str_barrier);
			
			start_pvc = (g_ptm_refcnt[barrier] == 0) ? 1 : 0;
			if(start_pvc){							
				memset(cmd, 0, sizeof(cmd));
				sprintf(cmd, "ifconfig ptm%d up",barrier);
				system(cmd);
			}
			g_ptm_refcnt[barrier] += 1;
		}
	}
#endif

#if defined(TCSUPPORT_CT_PON)
	if(start_pvc)
	{
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0],XPON_NODE);
		strcpy(nodeName[1],PON_LINK_CFG); 
		if(getAttrValue(top, nodeName, PONCOMMON_ATTR_XPON_LINK_STA, xPONLinkStaAttr) == TCAPI_PROCESS_OK){
			linkSta = atoi(xPONLinkStaAttr);
		}
	
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0],WAN);
	  	sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
		
		setAttrValue(top, nodeName, "PVCACTIVE", "Yes");

		if(linkSta == 1)//GPON
		{
			if(getAttrValue(top, nodeName, "GPONEnable", gponEnable) != TCAPI_PROCESS_OK ){
				tcdbg_printf("start_interface:fail get GPONEnable\n" );
				return FAIL;
			}	
			if(strcmp(gponEnable,"Yes") != 0)
				return FAIL;			
		}
		else if(linkSta == 2)//EPON
		{
			if(getAttrValue(top, nodeName, "EPONEnable", eponEnable) != TCAPI_PROCESS_OK ){
				tcdbg_printf("start_interface:fail get GPONEnable\n" );
				return FAIL;
			}	

			if(strcmp(eponEnable,"Yes") != 0)
				return FAIL;
			
		}
		else
		{
			//do nothing
		}		
	}

#else
#if defined(TCSUPPORT_CT_WAN_PTM)
	if(start_pvc && (transMode & 0x1) )			//atm
#else
	if(start_pvc)
#endif
	{
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0],WAN);
	  	sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);

		if(getAttrValue(top, nodeName, "QOS", str_qos) != TCAPI_PROCESS_OK ){
			tcdbg_printf("start_interface:fail get QOS\n" );
			return FAIL;
		}
		if(getAttrValue(top, nodeName, "ENCAP", str_encap) != TCAPI_PROCESS_OK ){
			tcdbg_printf("start_interface:fail get ENCAP\n" );
			return FAIL;
		}
		if(getAttrValue(top, nodeName, "VPI", str_vpi) != TCAPI_PROCESS_OK ){
			tcdbg_printf("start_interface:fail get VPI\n" );
			return FAIL;
		}
		if(getAttrValue(top, nodeName, "VCI", str_vci) != TCAPI_PROCESS_OK ){
			tcdbg_printf("start_interface:fail get VCI\n" );
			return FAIL;
		}
		getAttrValue(top, nodeName, "PCR", str_pcr); 
		getAttrValue(top, nodeName, "SCR", str_scr);
		getAttrValue(top, nodeName, "MBS", str_mbs);

		if(0 == strlen(str_qos) || 0 == strlen(str_encap) || 0 == strlen(str_vpi) || 0 == strlen(str_vci)){
			tcdbg_printf("start_interface:fail get PVC attribute\n" );
			return FAIL;
		}

		vpi = atoi(str_vpi);
		vci = atoi(str_vci);
		if(vpi == 0 && vci == 0){
			tcdbg_printf("start_interface:wrong VPI,VCI(%s, %s)\n", str_vpi, str_vci);
			return FAIL;
		}

		if(!strcmp(str_encap, "1483 Bridged IP LLC") || !strcmp(str_encap, "PPPoE LLC")){
			encap = 0;
			payload = 1;
		}
		else if(!strcmp(str_encap, "1483 Bridged IP VC-Mux") || !strcmp(str_encap, "PPPoE VC-Mux")){
			encap = 1;
			payload = 1;
		}
		else{
			tcdbg_printf("start_interface:not support encap:%s\n", str_encap);
			return FAIL;
		}

		if(!strcmp(str_qos, "ubr")){
			qos_type = 1;
		}
		else if(!strcmp(str_qos, "ubr+")){
			qos_type = 1;
			pcr = atoi(str_pcr);
		}
		else if(!strcmp(str_qos, "cbr")){
			qos_type = 2;
			pcr = atoi(str_pcr);
		}
		else if(!strcmp(str_qos, "rt-vbr")){
			qos_type = 3;
			pcr = atoi(str_pcr);
			scr = atoi(str_scr);
			mbs = atoi(str_mbs);
		}
		else if(!strcmp(str_qos, "nrt-vbr")){
			qos_type = 6;
			pcr = atoi(str_pcr);
			scr = atoi(str_scr);
			mbs = atoi(str_mbs);
		}
		else{
			tcdbg_printf("start_interface:not support qos:%s\n", str_qos);
			return FAIL;
		}

		//tcdbg_printf("start_interface:atmcmd:index:%d,encap:%d,payload:%d,qos:%d,pcr:%d,scr:%d,mbs:%d\n", pvc_index, encap, payload, qos_type, pcr, scr, mbs);
		atmcmdc_create_pvc(pvc_index, vpi, vci, encap, payload, qos_type, pcr, scr, mbs, 8192);
		setAttrValue(top, nodeName, "PVCACTIVE", "Yes");
	}
#endif
		

	sprintf(cmd, "%s %s %d %d",WAN_START_SH, ifname, pvc_index, start_pvc);
#ifdef WAN_DBUG
	tcdbg_printf("start_interface:cmd:%s\n", cmd );
#endif
	system(cmd);
	g_pvc_refcnt[pvc_index] += 1;
#if defined(TCSUPPORT_CT_PPP_ONDEMAND)
	if ( 1 == isOndemandWan(top, pvc_index, entry_index) )
		set_ondemand_route_state(if_index, 1);
	ClearOndemandWanHisAddr(top, pvc_index, entry_index);
#endif
	#if 0 //move to wan_monitor
	/* restart dmz based this wan interface */
	sprintf(dmz_node, "Dmz_Entry%d", if_index);
	tcapi_commit_internal(top, dmz_node);
	#endif
	return SUCCESS;
}

/*______________________________________________________________________________
**	notify_down_info
**
**	descriptions:
**		notify interface is down.
**	parameters:
**		top: top cfg node
**		if_index: interface index: 0 - 63
**		ifname: interface name,ext:nas0_1
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
void notify_down_info(mxml_node_t *top, int if_index, char *ifname)
{
	FILE *fp = NULL;
	char line[64];
	char isp[4], ipversion[10];
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	
	memset(line, 0, sizeof(line));
	fp = fopen("/var/run/down.conf", "r");
	if (NULL == fp) {
#ifdef CT_COM_DBUG
		tcdbg_printf("%s: no this file.\n", __FUNCTION__);
#endif
		return;
	}

	memset(isp, 0, sizeof(isp));
	memset(ipversion, 0, sizeof(ipversion));
	while (fgets(line, sizeof(line), fp)) {
		if ('\0' != line[0]) {
			sscanf(line, "%s %s", isp, ipversion);
		}
	}

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],WAN_IF_NODE);
	sprintf(nodeName[1], "Entry%d", if_index);

#ifdef CT_COM_DBUG
	tcdbg_printf("%s: isp is %s, ipversion is %s\n", __FUNCTION__, isp, ipversion);
#endif
	
	if (!strcmp(isp, "B")) {
		/* bride mode */
		setAttrValue(top, nodeName, wan_info_node[WAN_STATUS_TYPE], "down");
		interface_state_change(top, BRIDGE_IF_STATE_DOWN, ifname);
	}
	else if (!strcmp(isp, "R")) {
		/* route mode */
		if (!strcmp(ipversion, "IPv4")) {
			/* ipv4 */
			setAttrValue(top, nodeName, wan_info_node[WAN_STATUS_TYPE], "down");
			interface_state_change(top, IF_STATE_DOWN, ifname);
		}
#ifdef IPV6
		else if (!strcmp(ipversion, "IPv6")) {
			/* ipv6 */
			setAttrValue(top, nodeName, wan_info_node[WAN_STATUS6_TYPE], "down");
			interface_state_change(top, IF_STATE_DOWN6, ifname);
		}
		else {
			/* ipv4 and ipv6 */
			setAttrValue(top, nodeName, wan_info_node[WAN_STATUS_TYPE], "down");
			interface_state_change(top, IF_STATE_DOWN, ifname);

			setAttrValue(top, nodeName, wan_info_node[WAN_STATUS6_TYPE], "down");
			interface_state_change(top, IF_STATE_DOWN6, ifname);
		}
#endif
	}

	fclose(fp);
	unlink("/var/run/down.conf");
	return;
}
/*______________________________________________________________________________
**	stop_interface
**
**	descriptions:
**		stop a interface
**	parameters:
**		pvc_index: interface pvc(0~7)
**		ifname: interface name,ext:nas0_1
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int stop_interface(int pvc_index, int entry_index, char *ifname)
{
	char cmd[64] = {0};
	char iffolder[32] = {0};
	int stop_pvc = 0;
	DIR *dir = NULL;
	int if_index = pvc_index * MAX_SMUX_NUM + entry_index;
	mxml_node_t *top = get_node_top();
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char str_vpi[5] = {0}, str_vci[5] = {0};
	int vpi, vci;
	//char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
//#ifdef IPV6
	//char ipversion[12] = {0};
//#endif
#if defined(TCSUPPORT_CT_PON)
	char xPONLinkStaAttr[8] = {0};
	int linkSta = 0;
#endif

#if defined(TCSUPPORT_CT_WAN_PTM)
	int transMode = 0;
	int barrier = 0;
	char str_barrier[4] = {0},str_prebarrier[4] = {0},str_PTMWan[8] = {0};
#endif

	tcdbg_printf("stop_interface:come into stop_interface\n" );
	system("hw_nat -!");

	if(pvc_index < 0 || pvc_index > PVC_NUM){
		tcdbg_printf("stop_interface:pvc index is out of range\n" );
		return FAIL;
	}

#if defined(TCSUPPORT_CT_DHCP_PORT_FILTER)
	set_DHCP_PortFilter_info(top, pvc_index, entry_index, DEL_DHCP_PORTFILTER_CMD);
#endif
#if defined(TCSUPPORT_CT_BRIDGE_PPPSTATUS)
	bzero(cmd, sizeof(cmd));
	sprintf(cmd, "%d", pvc_index*MAX_SMUX_NUM + entry_index);
	doValPut("/proc/tc3162/bridge_ppp_sate", cmd);
#endif

	/*Check interface folder status*/
	sprintf(iffolder, "%s%s", WAN_IF_PATH, ifname);
	if(!(dir = opendir(iffolder))){
		tcdbg_printf("stop_interface:%s is not existed\n", iffolder);
		return FAIL;
	}
	else{
		closedir(dir);
	}

	//Delete DHCPRealy ebtables rule : xmdai
	set_DHCPRealy_info(top, pvc_index, entry_index, DEL_DHCPREALY_CMD);
	
	/* stop wan*/
	/*if this is last interface on this pvc,should stop pvc, otherwise not*/
	stop_pvc = (g_pvc_refcnt[pvc_index] == 1) ? 1 : 0;
#if defined(TCSUPPORT_CT_WAN_PTM)
	transMode = getWanTransModeType(pvc_index);
	if(transMode & 0x2 ) {		//ptm
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0],WAN);
		sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);

		
		getAttrValue(top, nodeName, "PTMEnable", str_PTMWan);
		if(!strcmp(str_PTMWan, "Yes")){//PTM wan		
			sprintf(nodeName[2], "%s%d", WAN_INTERFACE, entry_index);
			getAttrValue(top, nodeName, "Barrier", str_barrier); 
			getAttrValue(top, nodeName, "preBarrier", str_prebarrier); 
			if(strcmp(str_prebarrier,str_barrier)){ //modify
				/* when vdsl is from down to up, PTM MAC and TC will have chance to be un-sync, so reset is needed */
				system("echo 1 >/proc/tc3162/ptm_do_reset_sequence");
			}
			
			barrier = atoi(str_prebarrier);			
			stop_pvc = (g_ptm_refcnt[barrier] == 1) ? 1 : 0;
			if(stop_pvc){
				memset(cmd, 0, sizeof(cmd));
				sprintf(cmd, "ifconfig ptm%d down",barrier);
				system(cmd);
			}
			g_ptm_refcnt[barrier] -= 1;
			tcdbg_printf("stop_interface:stop_pvc=%d,g_ptm_refcnt[0]=%d,g_ptm_refcnt[1]=%d\n",stop_pvc,g_ptm_refcnt[0],g_ptm_refcnt[1]);
		}
	}
#endif
#if defined(TCSUPPORT_CT_DSLITE)
	stop_dslite(pvc_index, entry_index);
#endif

#if defined(TCSUPPORT_CT_PPPOE_EMULATOR)
	FILE *fp = NULL;
	char pid_tmp[50] = {0};
	int pid = 0;
	char wanName[32] = {0};
	int emu_pvc = 0;
	int emu_entry = 0;

	memset(nodeName,0,sizeof(nodeName));
	snprintf(nodeName[0],MAX_NODE_NAME,PPPOE_EMULATOR_BRIDGE_INFO_NODE_NAME);
	snprintf(nodeName[1],MAX_NODE_NAME,PPPOE_EMULATOR_ENTRY);
	if(getAttrValue(top, nodeName, "Wan", wanName) == TCAPI_PROCESS_OK)
	{
		sscanf(wanName, "Wan_PVC%d_Entry%d", &emu_pvc, &emu_entry);

		if((emu_pvc == pvc_index) && (emu_entry == entry_index))
		{
			fp = fopen(PPPOE_EMULATOR_BRIDGE_PID_PATH, "r");
			if(fp)
			{
				//kill the ping process
				fgets(pid_tmp, sizeof(pid_tmp), fp);
				pid = atoi(pid_tmp);
				fclose(fp);
				if(pid != 0)
				{			
					tcdbg_printf("%s kill bridge ppp emulator\n",__FUNCTION__);
	memset(cmd, 0, sizeof(cmd));
					snprintf(cmd, sizeof(cmd), "kill -9 %d", pid);
					system(cmd);
				}
			}
		}
		
	}
	//else
	//{
	//	tcdbg_printf("stop_interface:get pppoe emulator bridge info fail.");
	//}
	
#endif

	memset(cmd, 0, sizeof(cmd));
	sprintf(cmd, "%s %s %d %d",WAN_STOP_SH, ifname, pvc_index, stop_pvc);
#ifdef WAN_DBUG
	tcdbg_printf("stop_interface:cmd:%s\n", cmd );
#endif
	system(cmd);
	g_pvc_refcnt[pvc_index] -= 1;
	tcdbg_printf("stop_interface:stop_pvc=%d, pvc_index=%d, g_pvc_refcnt[pvc_index]=%d\n", stop_pvc , pvc_index, g_pvc_refcnt[pvc_index]);
	
#if defined(TCSUPPORT_CT_DSLITE)
	set_dslite_internal_state(if_index, 0);
#endif
#if defined(TCSUPPORT_CT_PON)
	if(stop_pvc)
	{	
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0],XPON_NODE);
		strcpy(nodeName[1],PON_LINK_CFG); 
		if(getAttrValue(top, nodeName, PONCOMMON_ATTR_XPON_LINK_STA, xPONLinkStaAttr) == TCAPI_PROCESS_OK){
			linkSta = atoi(xPONLinkStaAttr);
		}
			
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0],WAN);
	  	sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
		
		setAttrValue(top, nodeName, "PVCACTIVE", "No");
	}
#else

#if defined(TCSUPPORT_CT_WAN_PTM)
	if(stop_pvc && (transMode & 0x1)){			//atm
#else
	if(stop_pvc){
#endif
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0],WAN);
	  	sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
		
		if(getAttrValue(top, nodeName, "VPI", str_vpi) != TCAPI_PROCESS_OK ){
			tcdbg_printf("stop_interface:fail get VPI\n" );
			return FAIL;
		}
		if(getAttrValue(top, nodeName, "VCI", str_vci) != TCAPI_PROCESS_OK ){
			tcdbg_printf("stop_interface:fail get VCI\n" );
			return FAIL;
		}
		if( 0 == strlen(str_vpi) || 0 == strlen(str_vci)){
			tcdbg_printf("stop_interface:fail get PVC attribute\n" );
			return FAIL;
		}
		vpi = atoi(str_vpi);
		vci = atoi(str_vci);
		
		atmcmdc_close_pvc(vpi, vci);
		setAttrValue(top, nodeName, "PVCACTIVE", "No");
	}
#endif
	

#if 0
	/*Notify wan_monitor*/
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],WAN_IF_NODE);
	sprintf(nodeName[1], "Entry%d", if_index);
	
	setAttrValue(top, nodeName, wan_info_node[WAN_STATUS_TYPE], "down");
	interface_state_change(top, IF_STATE_DOWN, ifname);

#ifdef IPV6
	if(get_waninfo_by_index(if_index, "IPVERSION", ipversion) == SUCCESS){
		if(!strcmp(ipversion, "IPv4/IPv6") || !strcmp(ipversion, "IPv6")){
			setAttrValue(top, nodeName, wan_info_node[WAN_STATUS6_TYPE], "down");	
			interface_state_change(top, IF_STATE_DOWN6, ifname);
		}
	}
#endif
#endif
	/*Notify wan_monitor*/
	notify_down_info(top, if_index, ifname);

#ifdef WAN_DBUG
	tcdbg_printf("stop_interface: %s has been stop\n", ifname);
#endif
	
	return SUCCESS;
}

#if defined(TCSUPPORT_CT_PON_GDV20)
static char MonitorCollector_Enable[4] = {0};
static char Wan_IPMode[MAX_INTERFACE_NUM][4] = {0};
static char Wan_ConnectionType[MAX_INTERFACE_NUM][16] = {0};
static char Voice_ServerType[VOIP_LINE_NUM][8] = {0};
static char WLan_Enable[MAX_BSSID_NUM][4] = {0};

/*_____________________________________________________________________________
**      function name: getInformValue
**      descriptions:
**            Get inform value.
**
**      parameters:
**             top: 	Specify the root of mxml_node_t structure.
**             p_MonitorCollector_Enable: 	Enable value of MonitorCollector node.
**             p_Wan_IPMode: 				IPMode value of Wan node.
**             p_Wan_ConnectionType: 		ConnectionType value of Wan node.
**             p_Voice_ServerType: 			ServerType value of Voice node.
**             p_WLan_Enable: 				Enable value of WLan node.
**
**      return:
**            None
**____________________________________________________________________________
*/
void getInformValue(mxml_node_t *top, char *p_MonitorCollector_Enable,
	char p_Wan_IPMode[][4], char p_Wan_ConnectionType[][16],
	char p_Voice_ServerType[][8], char p_WLan_Enable[][4])
{
	int if_index = 0;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char isp[8] = {0}, service_app[SERVICE_LIST_LEN] = {0}, active[8] = {0};
	int pvc_index = 0, entry_index = 0, ret = 0;
	char attrValue[64] = {0};
	int i = 0;

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],"MonitorCollector");
	strcpy(nodeName[1],"Common");
	memset(attrValue, 0, sizeof(attrValue));
	if(getAttrValue(top, nodeName, "Enable", attrValue) == TCAPI_PROCESS_OK ){			
		strcpy(p_MonitorCollector_Enable, attrValue);
	}else{
		strcpy(p_MonitorCollector_Enable,"");
	}
	for(if_index = 0; if_index < MAX_INTERFACE_NUM; if_index++)
	{
		strcpy(p_Wan_IPMode[if_index],"");
		strcpy(p_Wan_ConnectionType[if_index],"");
		pvc_index = if_index / MAX_PVC_NUM;
		entry_index = if_index - pvc_index * MAX_PVC_NUM;
		memset(nodeName, 0, sizeof(nodeName));
	  	strcpy(nodeName[0],WAN);
	  	sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
		sprintf(nodeName[2], "%s%d", WAN_INTERFACE, entry_index);

		/* only for active connection */
		memset(active, 0, sizeof(active));
		if(getAttrValue(top, nodeName, "Active", active)!= TCAPI_PROCESS_OK){
			continue;
		}
		if (0 != strcmp(active, "Yes"))
		{
			continue;
		}
		
		/* only for ppp connetion with route mode */
		memset(isp, 0, sizeof(isp));
		if(getAttrValue(top, nodeName, "ISP", isp)!= TCAPI_PROCESS_OK){
			continue;
		}
		if (0 != strcmp(isp, PPP_MODE))/* 2 */
		{
			continue;
		}

		/* only for INTERNET type connection */
		memset(service_app, 0, sizeof(service_app));
		if(getAttrValue(top, nodeName, "ServiceList", service_app)
			!= TCAPI_PROCESS_OK){
			continue;
		}
		if (NULL == strstr(service_app, INTERNET))
		{
			continue;
		}
		else
		{
			memset(attrValue, 0, sizeof(attrValue));
			if(getAttrValue(top, nodeName, "IPVERSION", attrValue) 
				== TCAPI_PROCESS_OK ){	
				if ( 0 == strcmp(attrValue, "IPv4") )
					strcpy(p_Wan_IPMode[if_index], "1");
				else if ( 0 == strcmp(attrValue, "IPv6") )
					strcpy(p_Wan_IPMode[if_index], "2");
				else if ( 0 == strcmp(attrValue, "IPv4/IPv6") )
					strcpy(p_Wan_IPMode[if_index], "3");
				else
					strcpy(p_Wan_IPMode[if_index], "1");
			}
			if(strcmp(isp, DYNMIC_MODE)==0 || strcmp(isp, STATIC_MODE)==0 ||
				strcmp(isp, PPP_MODE)==0){
				strcpy(p_Wan_ConnectionType[if_index], "IP_Routed");
				if(getAttrValue(top, nodeName, "BridgeInterface", attrValue) 
					== TCAPI_PROCESS_OK ){			
					if ( 0 == strcmp("Yes", attrValue) )
						strcpy(p_Wan_ConnectionType[if_index], "PPPoE_Hybrid");

				}			
			}
			else if(strcmp(isp, BRIDGE_MODE)==0){
				memset(attrValue, 0, sizeof(attrValue));
				if(getAttrValue(top, nodeName, "BridgeMode", attrValue) 
					== TCAPI_PROCESS_OK ){			
					if(strcmp(attrValue, "IP_Bridged")==0) {
						strcpy(p_Wan_ConnectionType[if_index], "IP_Bridged");
					}
					else if(strcmp(attrValue, "PPPoE_Bridged")==0) {
						strcpy(p_Wan_ConnectionType[if_index], "PPPoE_Bridged");
					}
				}	
			}
		}		
	}
	for (i=0; i < VOIP_LINE_NUM; i++){
		strcpy(p_Voice_ServerType[i],"");
#if VOIP
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0],"VoIPBasic");
		strcpy(nodeName[1],"Common");
		memset(attrValue, 0, sizeof(attrValue));
		if(getAttrValue(top, nodeName, "SIPProtocol", attrValue)==TCAPI_PROCESS_OK ){			
			if(strcmp(attrValue,"IMSSIP")==0){
				strcpy(p_Voice_ServerType[i],"0");
			}else if(strcmp(attrValue,"SIP")==0){
				strcpy(p_Voice_ServerType[i],"1");
			}else if(strcmp(attrValue,"H.248")==0){
				strcpy(p_Voice_ServerType[i],"2");
			}
		}	
#endif
	}

	for (i=0; i < MAX_BSSID_NUM; i++){
		strcpy(p_WLan_Enable[i],"");
#ifdef TCSUPPORT_WLAN
		#if 0
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0],"WLan");
		sprintf(nodeName[1],"Entry%d", i);
		memset(attrValue, 0, sizeof(attrValue));

		if(getAttrValue(top, nodeName, "EnableSSID", attrValue)==TCAPI_PROCESS_OK ){			
			strcpy(p_WLan_Enable[i], attrValue);
		}	
		#else
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0],"WLan");
		sprintf(nodeName[1],"Common");
		memset(attrValue, 0, sizeof(attrValue));

		if(getAttrValue(top, nodeName, "APOn", attrValue)==TCAPI_PROCESS_OK ){			
			strcpy(p_WLan_Enable[i], attrValue);
		}	
		#endif
#endif
	}
}

/*_____________________________________________________________________________
**      function name: doInformValueInit
**      descriptions:
**            Init inform value.
**
**      parameters:
**             top: 	Specify the root of mxml_node_t structure.
**
**      return:
**            None
**____________________________________________________________________________
*/
void doInformValueInit(mxml_node_t *top)
{
	getInformValue(top, MonitorCollector_Enable, Wan_IPMode, 
		Wan_ConnectionType, Voice_ServerType, WLan_Enable);
}

/*_____________________________________________________________________________
**      function name: checkInformValueChange
**      descriptions:
**            Check inform value change or not.
**
**      parameters:
**            top: 	Specify the root of mxml_node_t structure.
**
**      return:
**            None
**____________________________________________________________________________
*/
void checkInformValueChange(mxml_node_t *top)
{
	int isChange =0;
	char l_MonitorCollector_Enable[4] = {0};
	char l_Wan_IPMode[MAX_INTERFACE_NUM][4] = {0};
	char l_Wan_ConnectionType[MAX_INTERFACE_NUM][16] = {0};
	char l_Voice_ServerType[VOIP_LINE_NUM][8] = {0};
	char l_WLan_Enable[MAX_BSSID_NUM][4] = {0};
	int i = 0;

	getInformValue(top, l_MonitorCollector_Enable, l_Wan_IPMode, 
		l_Wan_ConnectionType, l_Voice_ServerType, l_WLan_Enable);

	if(strcmp(MonitorCollector_Enable,l_MonitorCollector_Enable) != 0){	
		isChange = 1;						
		strcpy(MonitorCollector_Enable, l_MonitorCollector_Enable);
	}

	for(i = 0; i < MAX_INTERFACE_NUM; i++)
	{
		if(strcmp(Wan_IPMode[i],l_Wan_IPMode[i]) != 0){	
			isChange = 1;						
			strcpy(Wan_IPMode[i], l_Wan_IPMode[i]);
		}
		if(strcmp(Wan_ConnectionType[i],l_Wan_ConnectionType[i]) != 0){	
			isChange = 1;						
			strcpy(Wan_ConnectionType[i], l_Wan_ConnectionType[i]);
		}
	}
#if VOIP
	for(i = 0; i < VOIP_LINE_NUM; i++)
	{
		if(strcmp(Voice_ServerType[i],l_Voice_ServerType[i]) != 0){	
			isChange = 1;						
			strcpy(Voice_ServerType[i], l_Voice_ServerType[i]);
		}
	}
#endif
#ifdef TCSUPPORT_WLAN
	for(i = 0; i < MAX_BSSID_NUM; i++)
	{
		if(strcmp(WLan_Enable[i],l_WLan_Enable[i]) != 0){	
			isChange = 1;						
			strcpy(WLan_Enable[i], l_WLan_Enable[i]);
		}
	}
#endif

	if(isChange == 1){
		signalCwmpInfo(2,2,0);/* VALUE_CHANGED */
	}
}
#endif

char *
wan_merge_table[MAX_NODE_NAME]=
{
	"wan_pvc","",
};
/*______________________________________________________________________________
**	wan_boot
**
**	descriptions:
**		the boot function of wan.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
wan_boot(mxml_node_t *top){
	int pvc_index = 0;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	unsigned int new_filter_state=0;

#if defined(TCSUPPORT_CT_PON)
#if (defined(TCSUPPORT_WAN_GPON) || defined(TCSUPPORT_WAN_EPON) || defined(TCSUPPORT_PONMGR))
	char xPONModeAttr[8] = {0};
	int xPONMode = -1;
#endif
#endif

#if defined(TCSUPPORT_CT_PON_JS) || defined(TCSUPPORT_CT_NAMECHGNOTIFY)
	doNameChangeWanInit(top);
#endif

#if defined(TCSUPPORT_CT_PON_GDV20)
	doInformValueInit(top);
#endif

#if defined(TCSUPPORT_CT_PON)
#if defined(TCSUPPORT_CT_PON_GD) || defined(TCSUPPORT_CT_PON_CQ)
#ifndef PURE_BRIDGE
	if(wan_check_filter(top)){/*filter on*/
		new_filter_state=pre_sys_state.filter_m_state|WAN_VECTOR;
	}
	else{/*filter down*/
		new_filter_state=pre_sys_state.filter_m_state&(~WAN_VECTOR);
	}
	pre_sys_state.filter_m_state = check_and_set_filter(new_filter_state);
#endif

#if defined(TCSUPPORT_CT_DHCP6C_STATUS_CODE)
	set_dhcp6c_error_sw(top);
#endif

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],WAN);
	for(pvc_index = 0; pvc_index < PVC_NUM; pvc_index++){		
		sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
		wan_execute(top, nodeName);
	}
#endif

#if !defined(TCSUPPORT_CT_OMCI_CFG_SHRINK)
#ifdef TCSUPPORT_PONMGR
	/* move the ponapi_init from main() temporary by kevin, because the ponapi_init()  
	   must be called after wan module (GPON/EPON) ready */
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],XPON_NODE);
	strcpy(nodeName[1],PON_LINK_CFG); 
	if(getAttrValue(top, nodeName, PON_LINK_CFG_ATTR_XPON_MODE, xPONModeAttr) == TCAPI_PROCESS_OK){
		xPONMode = atoi(xPONModeAttr);
	}
	if(ponapi_init(xPONMode) != EXEC_OK) {
		tcdbg_printf("PONAPI initial error\n") ;
	}
#endif

#if defined(TCSUPPORT_WAN_GPON) || defined(TCSUPPORT_WAN_EPON)
	strcpy(nodeName[0],XPON_NODE);
	strcpy(nodeName[1],PON_LINK_CFG); 
	if(getAttrValue(top, nodeName, PON_LINK_CFG_ATTR_XPON_MODE, xPONModeAttr) == TCAPI_PROCESS_OK){
		xPONMode = atoi(xPONModeAttr);
#ifdef TCSUPPORT_EPON_OAM
		if((xPONMode == XMCS_IF_WAN_DETECT_MODE_AUTO) || (xPONMode == XMCS_IF_WAN_DETECT_MODE_EPON)){
			system("/userfs/bin/epon_oam &");
		}
#endif		
#ifdef TCSUPPORT_OMCI
		if((xPONMode == XMCS_IF_WAN_DETECT_MODE_AUTO) || (xPONMode == XMCS_IF_WAN_DETECT_MODE_GPON)){
			/* move /userfs/bin/omci & from rcS to here: 
			must need after ponapi_init() */
#if defined(TCSUPPORT_CT_PON_CY) || defined(TCSUPPORT_CT_PON_C5)
			system("taskset 0x8 /userfs/bin/omci &");
			system("renice -10 `pidof omci`");
#else
			system("/userfs/bin/omci &");
#endif
		}
#endif
#if 0
		if(xPONMode != -1){
			setXponMode(xPONMode);
		}
#endif
    }else{
		tcdbg_printf("getAttrValue-> PON_LINK_CFG_ATTR_XPON_MODE error\n") ;
	}
/*check the onu type 
if it is SFU, brctl addif br0 pon.
*/
	checkOnuType(top);	
#endif
#endif
#endif

#if !defined(TCSUPPORT_CT_PON_GD) && !defined(TCSUPPORT_CT_PON_CQ)
#ifndef PURE_BRIDGE
//	setNewFilterState(top,wan_check_filter,WAN_VECTOR,&new_filter_state);
	if(wan_check_filter(top)){//filter on
		new_filter_state=pre_sys_state.filter_m_state|WAN_VECTOR;
	}
	else{//filter down
		new_filter_state=pre_sys_state.filter_m_state&(~WAN_VECTOR);
	}
	pre_sys_state.filter_m_state = check_and_set_filter(new_filter_state);
#endif
	
#if defined(TCSUPPORT_CT_DHCP6C_STATUS_CODE)
	set_dhcp6c_error_sw(top);
#endif
#endif
#if defined(TCSUPPORT_CT_PON_SC)
	setBRDHCPChk(top);
#endif

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],WAN);	

#ifndef TCSUPPORT_CT_PON
	for(pvc_index = 0; pvc_index < PVC_NUM; pvc_index++){		
		sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
		wan_execute(top, nodeName);
	}
#endif

	/*Init wan monitor status*/
	wan_monitor_init();


#if 0
#ifndef PURE_BRIDGE
	startupSh=fopen("/etc/zebra.conf","r");
	if(startupSh){
		fclose(startupSh);
		system("/userfs/bin/zebra -f /etc/zebra.conf -i /var/run/zebra.pid &");
	}
	startupSh=fopen("/etc/ripd.conf","r");
	if(startupSh){
		fclose(startupSh);
		system("/userfs/bin/ripd -f /etc/ripd.conf -i /var/run/ripd.pid &");
	}

#endif
#endif
#if 0 //defined(TCSUPPORT_CT_PPPOEPROXY)
	pppoeproxy(top);
#endif

	g_wan_boot_finish = 1;
	return SUCCESS;
}
#if defined(TCSUPPORT_CT_RECONNECT_NODE)
char *
wan_adv_table[MAX_NODE_NAME]=
{
	"Common","",
};
#endif
/*______________________________________________________________________________
**	wan_init
**
**	descriptions:
**		the init function of wan.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
wan_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="Wan",
#if defined(TCSUPPORT_CT_RECONNECT_NODE)
			.type=PVC_TYPE | ADV_FLAG|ENTRY_TYPE | MAX_SMUX_NUM,
			.adv_table=wan_adv_table,
#else
                        .type=PVC_TYPE | ENTRY_TYPE | MAX_SMUX_NUM,
			.adv_table=NULL,
#endif
			.merge_table=wan_merge_table,
			.cfg_read=NULL,
			.cfg_write=wan_write,
			.cfg_verify=wan_verify,
			.cfg_execute=wan_execute,
			.cfg_boot=wan_boot,
			.cfg_pre_unset= wan_pre_unset,
	};
	ret=register_cfg_node(&node);
	return ret;
}

#if defined(TCSUPPORT_CT_PPPOEPROXY)
void pppoeproxy(mxml_node_t *top){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0}; 
	int i = 0;
	int j = 0;
	char proxyenable[4] = {0};
	char active[4] = {0};
	char username[256] = {0};
	char password[256] = {0};
	char buf[600] = {0};
	FILE *fp = NULL;
	int hasproxyenable = 0;
	char *output = NULL;
	FILE *fp2 = NULL;
	char temp[50] = {0};
	char pvcname[50] = {0};
	int pid = 0;
	char temp2[50] = {0};
	int pvccnt[8][8] = {0};
	char pidfile[20] = {0};
	int pppoe_exist = 0;
	int flag = 0;
	char if_name[20] = {0};
	int if_index = 0;

	fp = fopen("/etc/ppp/pap-secrets","w");
	if(fp == NULL){
		return;
	}

	memset(nodeName,0,sizeof(nodeName));
	strncpy(nodeName[0],WAN, sizeof(nodeName[0]));

	/*update file pap-secrets*/
	for(i = 0 ; i < PVC_NUM; i++){
		for(j=0 ; j < PVC_ENTRY_NUM; j++){
	  	snprintf(nodeName[1], sizeof(nodeName[1]), "%s%d", WAN_PVC, i);
		snprintf(nodeName[2], sizeof(nodeName[2]), "%s%d", WAN_INTERFACE, j); 
		if(getAttrValue(top,nodeName, "Active", active)!= TCAPI_PROCESS_OK){
			continue;
			}	
		if(getAttrValue(top,nodeName, "ProxyEnable", proxyenable)!= TCAPI_PROCESS_OK){
			continue;
			}
		if(getAttrValue(top,nodeName, "USERNAME", username)!= TCAPI_PROCESS_OK){
			continue;
			}
		if(getAttrValue(top,nodeName, "PASSWORD", password)!= TCAPI_PROCESS_OK){
			continue;
			}
		if((!strcmp(active, "Yes"))&&(!strcmp(proxyenable, "1"))){
			if_index = i * PVC_ENTRY_NUM + j;
#if !defined(TCSUPPORT_CT_PON_JS)
			cmdPipe("ifconfig", &output);
			snprintf(if_name, sizeof(if_name), "ppp%d", if_index);
			if (!strstr(output, if_name)){
				set_blackhole_route(top, if_name, if_index);
			}
			free(output);
#endif
			hasproxyenable = 1;
				//snprintf(buf, sizeof(buf), "%s  *  %s  *\n",username,password);
				snprintf(buf, sizeof(buf), "*  *  \"\"  *\n");
			fputs(buf,fp);
			}
		}

	}
	fclose(fp);


	/*up or kill pppoe-server due to different states*/
	cmdPipe("ps", &output);
	if(strstr(output,"pppoe-server")){	
		pppoe_exist=1;
	}
	free(output);
	/*has proxy enabled wan interface*/
	if(hasproxyenable){
		if(pppoe_exist){			
		}
		else{
			system("/usr/bin/pppoe-server -I br0 -L 192.168.239.1 -R 192.168.239.2 -N 64");
		}
	}
	/*has no proxy enabled wan interface*/
	else{
		if(pppoe_exist)
			system("killall pppoe-server");
		/*kill all msg rule in pppoeproxymsg*/
		fp2=fopen("/tmp/pppoeproxymsg","r");
		if(fp2){
			/*kill and count lan pids*/
			while(fgets(temp,sizeof(temp),fp2)!=NULL){
				sscanf(temp,"%s %d",pvcname,&pid);
				if(pid!=0){
					kill(pid,SIGKILL);
				}
				for(i = 0; i < PVC_NUM; i++){
					for(j = 0; j < PVC_ENTRY_NUM; j++){
						snprintf(temp2, sizeof(temp2), "PVC%d_Entry%d",i,j);
						if(!strcmp(pvcname,temp2)){
							flag = 1;
							pvccnt[i][j] ++;
							break;
						}
					}
					if(flag){
						flag = 0;
						break;
					}
				}
			}
			/*kill no proxyenabled wan pid*/
			for(i = 0; i < PVC_NUM; i++){
				for(j = 0; j < PVC_ENTRY_NUM; j++){
					if(pvccnt[i][j]!= 0){
					snprintf(pidfile, sizeof(pidfile), "/var/run/ppp%d.pid",(i*PVC_ENTRY_NUM+j+PPPOEPROXYOFFSET));
					kill_process(pidfile);
					}
				}
			}
			fclose(fp2);
			unlink("/tmp/pppoeproxymsg");
		}	
	}
	return;
}
#endif

/*______________________________________________________________________________
**	wan_write
**
**	descriptions:
**		Find wan element from mxml tree then write wan's system parameter to specify
**		configuration files.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  wan cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
wan_write(mxml_node_t *top, mxml_node_t *parant){
#if defined(TCSUPPORT_CT_RECONNECT_NODE)
	char fpath[64];
	mxml_node_t *node = NULL;

	/*create wan_common.conf*/
	sprintf(fpath, "%swan_common.conf", WAN_IF_PATH);
#if 0
	node = mxmlFindElement(top, top, "Common",
	  		NULL, NULL,
	  		MXML_DESCEND);
#else
	node = mxmlFindElement(top, top, WAN,
	  		NULL, NULL,
	  		MXML_DESCEND_FIRST);
	if (node == NULL) {
		return FAIL;
	}
	node = mxmlFindElement(node, node, WAN_COMMON,
	  		NULL, NULL,
	  		MXML_DESCEND);
#endif
	if(node != NULL)
		if(node_write(node, fpath, QMARKS) != 0)
			return FAIL;
#endif
#ifndef PURE_BRIDGE
	/*Create zebra.conf  & ripd.conf file*/
	//retval += create_rip_conf(top);
#endif
	//return retval;
	return SUCCESS;
}/* end wan_write */

/*______________________________________________________________________________
**	wan_verify
**
**	descriptions:
**		To check the system parameter of wan element  is validly or not.
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
wan_verify(mxml_node_t *node){
	return SUCCESS;
}/* end wan_verify */

#ifdef TCSUPPORT_CUC
void  set_igmp_mld_proxy(mxml_node_t * top, const char * ifName)
{
    char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
   
    memset(nodeName, 0, sizeof(nodeName));
    strcpy(nodeName[0],"MLDproxy");
    strcpy(nodeName[1], "Entry");

    setAttrValue(top, nodeName, "Active", ACTIVE);
    setAttrValue(top, nodeName, "UpstreamIF", ifName);

	memset(nodeName, 0, sizeof(nodeName));
  	strcpy(nodeName[0],"IGMPproxy");
  	strcpy(nodeName[1], "Entry");

    setAttrValue(top, nodeName, "Active", ACTIVE);
    setAttrValue(top, nodeName, "UpstreamIF", ifName);  	
}
#endif

/*_____________________________________________________________________________
**      function name: wan_execute
**      descriptions:
**            To execute wan node service.
**
**      parameters:
**            node:   Put the wan of cfg node tree that is used to execute wan settings.
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
wan_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	int pvc_index = 0, entry_index = 0, ppp_unit = 0, entry_start = 0, entry_end = 0;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char active[5] = {0}, ifname[16] = {0}, nasname[16] = {0}, isp[5] = {0}, pppunit[5] = {0}, old_ifname[16] = {0};
	mxml_node_t *curNode = NULL;
#if defined(TCSUPPORT_CT_PON_CZ_NX)
	char tempHwaddr[64] = {0};
	char VoIP_nas[16] = {0};
	char cur_VoIP_Domain_Id[64] = {0};
	int  VoIP_Domain_Id_len=0;
#endif
#if defined(TCSUPPORT_CT)
	char dotq_active[6], dotq_data[4];
#endif

#ifdef IPV6
	char ipversion[16] = {0};
#endif
#if defined(TCSUPPORT_CT_WAN_PTM)
	int dslState = 0;	
	int transMode = 0;
#endif
#if defined(TCSUPPORT_CT_VLAN_TAG)
	char ptmEnable[16] ={0};
#endif
	char attrVal[128] = {0};
	char nodebuf[32] = {0};
	int isTR69Commit = 0;
#if defined(TCSUPPORT_CT_L2TP_VPN)
	int isRestartVPN = 0;
#endif
	//DHCPRealy xmdai_nj
	char strWanMode[16];
#ifdef TCSUPPORT_CUC
    unsigned char igmp_mld_proxy_set = 0;    
#endif

#if defined(TCSUPPORT_CT_PON_SC)
	if ( 0 == strcmp(name[1], WAN_IF_COMMON) )
	{
		setBRDHCPChk(top);

		return SUCCESS;
	}
#endif

	/*
	*Step 1:Get pvc index and entry index
	*/
	if(verify_interface(top, name) != SUCCESS){
		tcdbg_printf(" wan_execute:verify Wan_%s failed\n", name[1]);
		return FAIL;
	}

	if(get_entry_number(name[1],WAN_PVC,&pvc_index) != SUCCESS ){
		return FAIL;
	}

	/*User just commit Wan_PVCx,no Entry attribute,so need to restart every Entry under PVC*/
	if(strlen(name[2]) == 0){
		entry_start = 0;
		entry_end = MAX_SMUX_NUM - 1;
		memset(nodeName, 0, sizeof(nodeName));
	  	strcpy(nodeName[0], "WebCustom");
	  	strcpy(nodeName[1], "Entry");
		memset(attrVal, 0, sizeof(attrVal));
		if( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, "tr69CommitWan", attrVal)
			&& '\0' != attrVal[0] )
		{
			isTR69Commit = atoi(attrVal);
			if ( 1 == isTR69Commit )
				setAttrValue(top, nodeName, "tr69CommitWan", "0");
		}
	}
	else{	
		if(get_entry_number(name[2],WAN_INTERFACE,&entry_index) != SUCCESS){
			return FAIL;
		}
		entry_start = entry_index;
		entry_end = entry_index;
	}
#ifdef WAN_DBUG
	tcdbg_printf("wan_execute:entry_start=%d :entry_end=%d!!!\n", entry_start, entry_end);
#endif
	/*
	*Step 2:Stop wan interface base on entry index
	*/
	for(entry_index = entry_start; entry_index <= entry_end; entry_index++){		
		
		memset(nodeName, 0, sizeof(nodeName));
	  	strcpy(nodeName[0],WAN);
	  	sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
		sprintf(nodeName[2], "%s%d", WAN_INTERFACE, entry_index);

		/*Check entry is existed or not*/
		if((curNode = search_node(nodeName)) == NULL)
		{
#ifdef WAN_DBUG		
			tcdbg_printf("wan_execute: pvc_index = %d, entry_index = %d i not existed!!!\n", pvc_index,entry_index);
#endif
			continue;	
		}

		if ( 1 == isTR69Commit )
		{
			memset(attrVal, 0, sizeof(attrVal));
			if ( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, "ServiceList", attrVal) )
			{
				if ( NULL != strstr(attrVal, "TR069") )
				{
					snprintf(nodebuf, sizeof(nodebuf), "%s_%s%d_%s%d", WAN, WAN_PVC, pvc_index, WAN_INTERFACE, entry_index );
					tcdbg_printf("\r\n don't commit TR069 Wan %s !", nodebuf);
					continue;
				}
			}

		}

#ifdef WAN_DBUG		
		tcdbg_printf("wan_execute:  pvc_index=%d, entry_index=%d is existed!!!\n",pvc_index, entry_index);
#endif

		memset(ifname, 0, sizeof(ifname));
		if(getAttrValue(top, nodeName, WAN_IFNAME, ifname) == TCAPI_PROCESS_OK ){
			/*Stop Wan interface firstly when commit*/

#ifdef WAN_DBUG	
			tcdbg_printf("wan_execute: stop Wan %s first!!!\n", ifname);
#endif
			stop_interface(pvc_index, entry_index, ifname);
			/*Backup interface name*/
			strcpy(old_ifname, ifname);
		}
//		else{
		/*Create or Update interface name*/
		/*use default interface name if IFName is not setted*/			
		sprintf(nasname, DEFAULT_NASNAME_FORMAT, pvc_index, entry_index);			
		setAttrValue(top, nodeName, WAN_NASNAME, nasname);
		
		
#if defined(TCSUPPORT_CT_PON_CZ_NX)
		if ( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, "ServiceList", attrVal) )
		{
			if ( NULL != strstr(attrVal, "VOICE") )
			{
				strcpy(VoIP_nas,nasname);	
			}
		}		
#endif
		
		/*generally,interface name is equal nas name except for ppp mode,ext:nas0_2*/
		strcpy(ifname, nasname);

		if(getAttrValue(top, nodeName, "ISP", isp) != TCAPI_PROCESS_OK ){
			tcdbg_printf("wan_execute:get ISP failed\n");
			return FAIL;

		}
#ifdef WAN_DBUG		
		tcdbg_printf("wan_execute:ISP:%s\n", isp);
#endif

		/*Creat ppp unit for ppp mode*/
		if(!strcmp(isp, PPP_MODE)){
			//if(getAttrValue(top, nodeName, "PPPUNIT", pppunit) != TCAPI_PROCESS_OK ){
				/*we use static ppp unit number for every entry*/
				ppp_unit = pvc_index * MAX_SMUX_NUM + entry_index;
				sprintf(pppunit, "%d", ppp_unit);
				//tcdbg_printf("wan_execute PPP Unit:%s\n", pppunit);
				setAttrValue(top, nodeName, "PPPUNIT", pppunit);
				/*interface name is pppX when ppp mode*/
				sprintf(ifname, "ppp%d", ppp_unit);
			//}
		}
#ifdef WAN_DBUG
		tcdbg_printf("wan_execute :create new nas name:%s and ifname:%s\n", nasname, ifname);
#endif
		setAttrValue(top, nodeName, WAN_IFNAME, ifname);
//		}

		/*Add related node:WanInfo, DMZ, Virtual server*/
		add_waninfo_entry(pvc_index, entry_index);
		/*Delete related node when interface name change*/
		if(strlen(old_ifname) != 0  && strcmp(old_ifname, ifname)){
			//update_static_route(old_ifname, ifname);
			del_static_route(old_ifname);
#if defined(TCSUPPORT_CT_2NTP)
#if defined(TCSUPPORT_CT_NTPSERVERTYPE)
#else
			change_ntp_route(old_ifname,ifname);
#endif
#endif
			del_dmz_entry(pvc_index, entry_index, ifname);
			del_vserver_entry(pvc_index, entry_index, ifname);
			/*delete related igmp & mld proxy info*/
			del_igmp_mld_proxy(old_ifname);
		}
		/*This case check if service list change from INTERNET to None INTERNET*/
		else
		{
			/*Not check add WAN Interface case*/
			if(strlen(old_ifname) != 0)
			{
				char serviceList[SERVICE_LIST_LEN] = {0};
				if(getAttrValue(top, nodeName, "ServiceList", serviceList) == TCAPI_PROCESS_OK )
				{
					if(strstr(serviceList, "INTERNET") == NULL && strstr(serviceList, "OTHER") == NULL)
					{
						/*delete related igmp & mld proxy info*/
						del_igmp_mld_proxy(old_ifname);
					}
				}
			}
		}
		add_dmz_entry(pvc_index, entry_index, ifname);
		add_vserver_entry(pvc_index, entry_index, ifname);
	}

#if defined(TCSUPPORT_CT_PON) || defined(TCSUPPORT_CT_DSL_EX)
#if !defined(TCSUPPORT_CT_PON_GD) && !defined(TCSUPPORT_CT_PON_CQ)
		if (g_adsl_state == ADSL_STATE_DOWN) {
			updateDhcpRules(top);
			tcdbg_printf("\r\nwan_execute(): dsl is down, so just return");
			return SUCCESS;
		}
#endif
#endif

	/*
	*Step 3:Start wan interface base on entry index if it is active
	*/
#if defined(TCSUPPORT_CT_VLAN_TAG)
	memset(ptmEnable, 0, sizeof(ptmEnable));
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],WAN);
	sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
	getAttrValue(top, nodeName, "PTMEnable", ptmEnable);
#endif

	for(entry_index = entry_start; entry_index <= entry_end; entry_index++){
		memset(nodeName, 0, sizeof(nodeName));
	  	strcpy(nodeName[0],WAN);
	  	sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
		sprintf(nodeName[2], "%s%d", WAN_INTERFACE, entry_index);

		/*Check entry is existed or not*/
		if((curNode = search_node(nodeName)) == NULL)
			continue;

#if defined(TCSUPPORT_CT_PON_SC)
		if ( 0 != checkPPPUserPSW(top, nodeName) )
		{
			tcdbg_printf("\n[%s]: ppp no user or psw, so just continue\n", __FUNCTION__ );
			continue;
		}
#endif
#if defined(TCSUPPORT_CT_L2TP_VPN)
		bzero(attrVal, sizeof(attrVal));
		if ( TCAPI_PROCESS_OK == getAttrValue(top, nodeName
			, "ServiceList", attrVal) )
		{
			if ( NULL != strstr(attrVal, "INTERNET") )
			{
				isRestartVPN = 1;
			}
		}
#endif
#ifdef WAN_DBUG
		tcdbg_printf("wan_execute :pvc_index =%d,entry_index=%d, is existed!\n",pvc_index, entry_index);
#endif
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
		if(isSimCardExist(top) == 0 && getCpeFlag(top) == 1){ 
			char serviceList[SERVICE_LIST_LEN] = {0};
			getAttrValue(top, nodeName, "ServiceList", serviceList);
			if(strstr(serviceList, "TR069") == NULL)
			{
				tcdbg_printf("wan_execute:cpeFlag =1 & not tr069, no need up(pvc_index=%d,entry_index=%d)!\n",pvc_index,entry_index);
				continue;
			}
		}	
#endif

		if(getAttrValue(top, nodeName, WAN_IFNAME, ifname) != TCAPI_PROCESS_OK ){
			tcdbg_printf("wan_execute:get if name failed!\n");
			return FAIL;
		}

#if defined(TCSUPPORT_CT)
		if(getAttrValue(top, nodeName, "ISP", isp) != TCAPI_PROCESS_OK ){
			tcdbg_printf("wan_execute:get ISP failed\n");
			return FAIL;
		}
		
#if !defined(TCSUPPORT_CT_VLAN_TAG)
		if (0 != strcmp(isp, PPP_MODE)) {
			memset(dotq_active, 0, sizeof(dotq_active));
			memset(dotq_data, 0, sizeof(dotq_data));
			if (getAttrValue(top, nodeName, "dot1q", dotq_active) == TCAPI_PROCESS_OK) {
				#ifdef CT_COM_DBUG
					tcdbg_printf("%s:vlan dotq_active is %s.\n", __FUNCTION__, dotq_active);
				#endif
				if (!strcmp(dotq_active, "Yes")) {
					/* if open vlan, change interface name, for dhcpc/acl etc may use. */
					if (getAttrValue(top, nodeName, "VLANID", dotq_data) == TCAPI_PROCESS_OK) {
						sprintf(ifname, "%s.%s", ifname, dotq_data);	
						#ifdef CT_COM_DBUG
						tcdbg_printf("%s: vlan devcie name is %s.\n", __FUNCTION__, ifname);
						#endif
					}

					/* update interface name in wan_pvc{x}_entry{y} cfg node */
					setAttrValue(top, nodeName, WAN_IFNAME, ifname);
				}
			}
		}
#endif

#endif


		//Add DHCPRealy ebtables rules : xmdai_nj
		memset(strWanMode, 0, sizeof(strWanMode));
		if(getAttrValue(top, nodeName, WAN_MODE, strWanMode) != TCAPI_PROCESS_OK ){
			tcdbg_printf("wan_execute:get WanMode failed\n");
			return FAIL;
		}

#if defined(TCSUPPORT_CT_FULL_ROUTE)
		if(0 == full_route_execute(pvc_index, entry_index, strWanMode))
		{
#endif
#ifdef TCSUPPORT_PORTBIND
		/* set port binding info */
		set_portbinding_info2(top, pvc_index, entry_index, DEL_PORTBIND);
		set_portbinding_info2(top, pvc_index, entry_index, ADD_PORTBIND);
#endif
#if defined(TCSUPPORT_CT_FULL_ROUTE)
		}
#endif

		//set_DHCPRealy_info(top, pvc_index, entry_index, DEL_DHCPREALY_CMD);
		if(!strcmp(strWanMode,WAN_BRIDGE_MODE)){
			set_DHCPRealy_info(top, pvc_index, entry_index, ADD_DHCPREALY_CMD);
		}
#if defined(TCSUPPORT_CT_DHCP_PORT_FILTER)
		set_DHCP_PortFilter_info(top, pvc_index, entry_index, ADD_DHCP_PORTFILTER_CMD);
#endif
		
		/*check active*/
		if(getAttrValue(top, nodeName, "Active", active) != TCAPI_PROCESS_OK ){			
			tcdbg_printf("wan_execute :get Active status failed!\n");
			return FAIL;
		}
#if defined(TCSUPPORT_CT_WAN_PTM)
		dslState = get_dsl_state();
		transMode = getWanTransModeType(pvc_index);
		if(!strcmp(active,ACTIVE) &&
			(((dslState == 1)&&(transMode&0x1))		//atm
			|| ((dslState == 2)&&(transMode&0x2))))	//ptm
#else
		/*Is active*/
		if(!strcmp(active,ACTIVE))
#endif
		{
#if defined(TCSUPPORT_CT_PPPCONN_ERROR)
#if defined(TCSUPPORT_CT_PON_GD)
			if(0 == strcmp(strWanMode,WAN_BRIDGE_MODE))
			{
				setAttrValue(top, nodeName, "ConnectionError", "ERROR_NONE");
			}
#else
			setAttrValue(top, nodeName, "ConnectionError", "ERROR_NO_ANSWER");
#endif
#if defined(TCSUPPORT_CT_PPPERRCODE_ENHANCE)
			setAttrValue(top, nodeName, "ConnectionErrorCode", "");
#endif
#endif
#ifdef WAN_DBUG
			tcdbg_printf("wan_execute: %s is active!!!\n", ifname);
#endif
			//add by brian for nat enable rules
			pre_sys_state.nat_raw_state = check_nat_enable(top,pre_sys_state.nat_raw_state);
#ifdef WAN_DBUG
					tcdbg_printf("wan_execute :start_interface--line 1995!\n");
#endif

			if(start_interface(pvc_index, entry_index, ifname) != SUCCESS)
				return FAIL;
#ifdef IPV6
#if defined(TCSUPPORT_CT_WAN_CHILD_PREFIX)
			/*set child prefix to /var/run/ifname/*/
			set_childprefx_file(pvc_index, entry_index,ifname);
#endif

			if(getAttrValue(top, nodeName, "IPVERSION", ipversion) != TCAPI_PROCESS_OK ){			
				tcdbg_printf("wan_execute failed:get IPVERSION attribute failed\n");
				return FAIL;
			}
			/*Active start_cc polling mode for get IPv6 gateway and SLAAC address*/
			if((!strcmp(ipversion, "IPv4/IPv6") || !strcmp(ipversion, "IPv6")) &&
				strcmp(isp, BRIDGE_MODE))		/*not bridge mode*/
				set_cc_wan_state((pvc_index * MAX_SMUX_NUM + entry_index), 1);
#endif
#if defined(TCSUPPORT_RIPD)
			if(g_wan_boot_finish){
				/*start ripd*/	
				tcapi_commit_internal(top, LANRIPD);
			}
#endif
		}					
		else{/*The interface is not active*/
#ifdef IPV6
			set_cc_wan_state((pvc_index * MAX_SMUX_NUM + entry_index), 0);
#endif
#if defined(TCSUPPORT_CT_PPPCONN_ERROR)
			setAttrValue(top, nodeName, "ConnectionError", "ERROR_NOT_ENABLED_FOR_INTERNET");
#endif
		}		
#ifdef TCSUPPORT_CUC
        if ( 0 ==  igmp_mld_proxy_set ){
			if(0 == strcmp(strWanMode,WAN_ROUTE_MODE)){
	            memset(attrVal, 0, sizeof(attrVal));
	            if ( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, "ServiceList", attrVal) ){           
	                if ( NULL != strstr(attrVal, "IPTV")){
	                    set_igmp_mld_proxy(top, ifname);
	                    igmp_mld_proxy_set = 1;
	                }else{
	                    del_igmp_mld_proxy(ifname); // TOdo: should delete if it is "INTERNET" or "OTHERS"
					}
	            }
			}
        }
#endif
    }

	if(g_wan_boot_finish){
		/*Modify related app*/
		#ifndef PURE_BRIDGE
		/**********20080905 racing add************/
		/*restart acl*/	
		tcapi_commit_internal(top, ACL);
		/**********20080905 racing add************/

		/**********20081005 racing add************/
		/*restart ipfilter*/
		tcapi_commit_internal(top, IPMACFILTER);
		/**********20081005 racing add************/

		/*krammer add for firewall*/
		tcapi_commit_internal(top, FIREWALL);
		/**/
#if defined(TCSUPPORT_CT_VLAN_BIND)
		tcapi_commit_internal(top, VLAN_BIND);
#endif
#if defined(TCSUPPORT_CT_BRIDGEARP_NOFWD_LAN)
		isNeedChgArpRule = 1;
#endif
#if defined(TCSUPPORT_CT_L2TP_VPN)
		if ( isRestartVPN )
		{
			bzero(nodeName, sizeof(nodeName));
			strcpy(nodeName[0],	VPNSTR);
			strcpy(nodeName[1],	VPN_COMMON);
			setAttrValue(top, nodeName, VPN_ATTR_ACTION, VPN_ACT_WANRESTART);
			tcapi_commit_internal(top, VPNSTR);
		}
#endif

		#endif
	}
	
#if defined(TCSUPPORT_CT_PPPOEPROXY)
	pppoeproxy(top);
#endif

#if defined(TCSUPPORT_CT_DSL_EX)
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],	WEB_NODE);
	strcpy(nodeName[1],	ENTRY);
	setAttrValue(top, nodeName, WAN_EXED, "1");
#endif

//set MG Domain Name
#if defined(TCSUPPORT_CT_PON_CZ_NX)


	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], VOIPH248);
	strcpy(nodeName[1], VOIPCOMMON);
	getAttrValue(top, nodeName, "DeviceID", cur_VoIP_Domain_Id);
	VoIP_Domain_Id_len=strlen(cur_VoIP_Domain_Id);
	if((strlen(VoIP_nas)!=0)&&(VoIP_Domain_Id_len==0)){
		getWanInfo(Mac_type, VoIP_nas, tempHwaddr);
		setAttrValue(top, nodeName, "DeviceID", tempHwaddr);

	}



#endif
	return SUCCESS;
}/* end wan_execute */

/*_____________________________________________________________________________
**      function name: wan_pre_unset
**      descriptions:
**            To stop wan service before deleting wan node.
**
**      parameters:
**            node:   Put the wan of cfg node tree that is used to execute wan settings.
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
**      1. jlliu 2010/7/19
**____________________________________________________________________________
*/
int wan_pre_unset(mxml_node_t *top, char name[][MAX_NODE_NAME])
{
	int pvc_index, entry_index;
	char ifname[16] = {0}, attrVal[128] = {0};
	int wanPVCFlag = 0;
	int failFlag = 0;
	int pvcEntryStart, pvcEntryEnd;
	int i;
	int valid_if[MAX_WAN_IF_INDEX] = {0}, valid_if_num = 0;	
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
#ifdef WAN_DBUG
	tcdbg_printf("wan_pre_unset start!\n");
#endif
	if(get_entry_number(name[1],WAN_PVC,&pvc_index) != SUCCESS ){
		tcdbg_printf("wan get WAN_PVC%d fails\n", pvc_index);
		return FAIL;
	}
	else
	{
#ifdef WAN_DBUG
		tcdbg_printf("wan get WAN_PVC%d success\n", pvc_index);
#endif
	}
	if(get_entry_number(name[2],WAN_INTERFACE,&entry_index) != SUCCESS){
		wanPVCFlag = 1;
		//	return FAIL;
	}
	
	if(wanPVCFlag == 1)
	{
		pvcEntryStart = 0;
		pvcEntryEnd = MAX_SMUX_NUM;
	}
	else
	{
		pvcEntryStart = entry_index;
		pvcEntryEnd = entry_index + 1;		
	}

	memset(name[2], 0, sizeof(name[2]) );

	for( i = pvcEntryStart; i < pvcEntryEnd; i++ )
	{
		sprintf(name[2],"%s%d", WAN_INTERFACE, i);	
		if(getAttrValue(top, name, "IFName", ifname) == TCAPI_PROCESS_OK ){
                       	/*Stop Wan interface*/
                       	stop_interface(pvc_index, i, ifname);

#if defined(TCSUPPORT_CT_E8DDNS)
		/* Unset DDNS Entry releate to this wan interface */
		Unset_DDNS_Entry(top, pvc_index, entry_index);
#endif

             		 /*Delete dmz entry for this interface*/
                       	del_dmz_entry(pvc_index, i, ifname);

                      	/*Delete virtual server entry for this interface*/
                       	del_vserver_entry(pvc_index, i, ifname);

                       	/*Delete WanInfo node*/
                       	del_waninfo_entry(pvc_index, i);
                       	/*Delet static route if needed*/
              		del_static_route(ifname);
				/*delete related igmp & mld proxy info*/
				del_igmp_mld_proxy(ifname);

#ifdef TCSUPPORT_QOS
#if defined(TCSUPPORT_CT_QOS)
                       	/* update qos rule */
                       	update_qos_rule(top, pvc_index, i);
#endif
#endif

#if defined(TCSUPPORT_CT_FULL_ROUTE)
		close_table_by_ifIndex(get_wanindex_by_name(ifname));
#endif
#ifdef TCSUPPORT_PORTBIND
                       	set_portbinding_info2(top, pvc_index, i, DEL_PORTBIND);
#endif
#if defined(TCSUPPORT_CT_L2TP_VPN)
		bzero(attrVal, sizeof(attrVal));
		if ( TCAPI_PROCESS_OK == getAttrValue(top, name
			, "ServiceList", attrVal) )
		{
			if ( NULL != strstr(attrVal, "INTERNET") )
			{
				bzero(nodeName, sizeof(nodeName));
				strcpy(nodeName[0], VPNSTR);
				strcpy(nodeName[1], VPN_COMMON);
				setAttrValue(top, nodeName, VPN_ATTR_ACTION, VPN_ACT_WANRESTART);
				tcapi_commit_internal(top, VPNSTR);
			}
		}
#endif
#ifdef WAN_DBUG
			tcdbg_printf("wan get WAN_PVC%d_Entry%d success\n", pvc_index, i);
#endif
        }else{
			tcdbg_printf("wan get WAN_PVC%d_Entry%d fails\n", pvc_index, i);
			failFlag = 1;
                       	//return FAIL;
               	}
	}

	if(wanPVCFlag)
	{
		memset(name[2], 0, sizeof(name[2]) );
	}
	valid_if_num = get_all_wan_index(valid_if, MAX_WAN_IF_INDEX);
	if ( valid_if_num <= 1 )
	{
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0],"Wan");
		strcpy(nodeName[1],"Common");
		setAttrValue(top, nodeName, "LatestIFIdx", "0");
	}

#ifdef WAN_DBUG
	tcdbg_printf("wan_pre_unset done!\n");
#endif
	if(failFlag)
		return FAIL;
	else
		return SUCCESS;
}

#if defined(TCSUPPORT_CT_E8DDNS)
void Unset_DDNS_Entry(mxml_node_t *top, int pvcIndex, int entryIndex)
{
	int DDNS_Entry_Index = pvcIndex*PVC_ENTRY_NUM+entryIndex;
	tcapi_msg_t msg;

	memset(&msg,0,sizeof(msg));
	snprintf(msg.node, sizeof(msg.node), "Ddns_Entry%d", DDNS_Entry_Index);	
	tcapi_unset_req(top, &msg);
	
	memset(&msg,0,sizeof(msg));
	strncpy(msg.node, "Ddns", sizeof(msg.node));	
	tcapi_commit_req(top, &msg);
}
#endif
#if defined(TCSUPPORT_C1_CUC) && defined(TCSUPPORT_SYSLOG)
void AddSyslogForNewIFConfig(mxml_node_t *top, int pvc_index, int entry_index, char *state)
{
	char log[128] = {0};
	char servicelist[SERVICE_LIST_LEN] = {0}, wanmode[16] = {0};
	char vpi[4] = {0}, vci[4] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],"Wan");
	sprintf(nodeName[1],"PVC%d", pvc_index);
	if(getAttrValue(top, nodeName, "VPI", vpi) != TCAPI_PROCESS_OK ){
			tcdbg_printf("Get New Interface parameter fail.\n");
	}

	if(getAttrValue(top, nodeName, "VCI", vci) != TCAPI_PROCESS_OK ){
			tcdbg_printf("Get New Interface parameter fail.\n");
	}
	
	sprintf(nodeName[2], "Entry%d", entry_index);

	if(getAttrValue(top, nodeName, "WanMode", wanmode) != TCAPI_PROCESS_OK ){
			tcdbg_printf("Get New Interface parameter fail.\n");
	}
	
	if(getAttrValue(top, nodeName, "ServiceList", servicelist) != TCAPI_PROCESS_OK ){
			tcdbg_printf("Get New Interface parameter fail.\n");
	}
	if(!strcmp(wanmode, "Route"))
	snprintf(log, sizeof(log), "Interface %s_R_%s_%s index%d is %s.",servicelist, vpi, vci, entry_index, state);
	else
	snprintf(log, sizeof(log), "Interface %s_B_%s_%s index%d is %s.",servicelist, vpi, vci, entry_index, state);

	openlog("TCSysLog syslog", 0, LOG_SYSLOG);
	syslog(LOG_INFO, log);
	closelog();
	return;
}
#endif

char *
wanInfo_adv_table[MAX_NODE_NAME]=
{
	"Common","Message","WanIF","WanPVC",
#if defined(TCSUPPORT_CT_ADSL_HN) || defined(TCSUPPORT_CT_PON_CZ_NX)
	"ModeSwitch",
#endif
#if defined(TCSUPPORT_CT_PHONEAPP)
	"MannualDNS",
#endif
	"",
};

char *
wanInfo_merge_table[MAX_NODE_NAME]=
{
	"waninfo_index","",
};

/*______________________________________________________________________________
**	wanInfo_init
**
**	descriptions:
**		the init function of wanInfo.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
wanInfo_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="WanInfo",
			.type=UPDATE_TYPE|WEB_TYPE|ADV_FLAG|ENTRY_TYPE|MAX_WANIF_NUM,
			.adv_table=wanInfo_adv_table,
			.merge_table=wanInfo_merge_table,
			.cfg_read=wanInfo_read,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=wanInfo_execute,
			.cfg_boot=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}
/*_____________________________________________________________________________
**      function name: wanInfo_read
**      descriptions:
**           To create wanInfo cfg node.
**
**      parameters:
**            top:  Put the top of cfg node tree.
**            name: The name of deviceInfo cfg node.
**
**      global:
**             None
**
**      return:
**             None
**
**      call:
**
**
**      revision:
**      1. jlliu 20100720
**____________________________________________________________________________
*/
int
wanInfo_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr){
	mxml_node_t *parentNode = NULL;
	mxml_node_t *curNode = NULL;

	int pvc_index, entry_index, if_index;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char dst_nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char src_nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char str_valid_if[128] = {0}, tmp[12] = {0}, str_cur_if[5] = {0}, str_valid_num[5] = {0};
	int valid_if[MAX_WAN_IF_INDEX] = {0}, valid_if_num = 0, i;
	char str_guiinterfacename[64] = {0}, str_vpi[4] = {0}, str_vci[4] = {0};
	char str_servicelist[SERVICE_LIST_LEN] = {0}, str_wanmode[8] = {0};
	char str_ifname[16] = {0};
	char str_active[8] = {0};
	char str_natenable[12] = {0};
	char str_nasname[8] = {0};
	char str_ipversion[16] = {0};
	char str_status[8] = {0};
	char str_ip[32] = {0};
	char str_netmask[32] = {0};
	char str_encap[32] = {0};
	char str_gateway[32] = {0};
	char str_dns[32] = {0};
	char str_secdns[32] = {0};
	char str_status6[8] = {0};
	char str_ip6[64] = {0};
	char str_gateway6[64] = {0};
	char str_dns6[64] = {0};
	char str_secdns6[64] = {0};
	char str_pd6[64] = {0}, str_orgpd6[64] = {0};
	char str_isp[16] = {0};
#if defined(TCSUPPORT_CT_PON) || defined(TCSUPPORT_CT_WAN_PTM) || defined(TCSUPPORT_CT_E8B_ADSL)
	char str_connection[30] = {0};
	char str_obtipmode[20] = {0};
	char str_vlanEnable[6] = {0};
	char str_priEnable[6] = {0};
	char str_linkmode[16] = {0};
	char str_mac[20] = {0};
	char str_ispvalue[6] = {0};
	char str_vid[6] = {0};
	char str_pri[2] = {0};
	char str_ifIdx[4] = {0};
	char str_mcvid[6] = {0};
#endif
#if defined(TCSUPPORT_CT_WAN_PTM) || defined(TCSUPPORT_CT_E8B_ADSL)
	char str_atmenable[4] = {0};
	char str_ptmenable[4] = {0};
	char str_uicustom[32] = {0};
	char str_stopupdate[4] = {0};
	int pause_updatewan = 0;
	int transmode = 0; // ATM:1, PTM:2
#endif
	char str_ppp_bi[12] = {0};
	char str_cyclejump[4] = {0};
	int cycle_jump = 0;
	int allroute = 1;

#if defined(TCSUPPORT_CT_ADSL_HN) || defined(TCSUPPORT_CT_PON_CZ_NX)
	if(!strcmp(name[1], "ModeSwitch"))
	{
		return ModeSwitch_read(top, name, attr);
	}
#endif

#if defined(TCSUPPORT_CT_PON) || defined(TCSUPPORT_CT_WAN_PTM) || defined(TCSUPPORT_CT_E8B_ADSL)
	char* str_cycle_value[MAX_BUFF_SIZE_CNT];
	for(i = 0; i < MAX_BUFF_SIZE_CNT; i++)
		str_cycle_value[i] = NULL;
	char* str_cycle_small_value[SMALL_BUFF_CNT];
	for(i = 0; i < SMALL_BUFF_CNT; i++)
		str_cycle_small_value[i] = NULL;

	for(i = 0; i < MAX_BUFF_SIZE_CNT; i++)
	{
		str_cycle_value[i] = (char *)malloc(MAX_BUFF_SIZE);
		if(str_cycle_value[i] == NULL)
			goto FreeBuff;
		memset(str_cycle_value[i], 0, MAX_BUFF_SIZE);
	}

	for(i = 0; i < SMALL_BUFF_CNT; i++)
	{
		str_cycle_small_value[i] = (char *)malloc(SMALL_BUFF_SIZE);
		if(str_cycle_small_value[i] == NULL)
			goto FreeBuff;
		memset(str_cycle_small_value[i], 0, SMALL_BUFF_SIZE);
	}
#else
	char* str_cycle_value[15];
	for(i = 0; i < 15; i++){
		str_cycle_value[i] = (char *)malloc(MAX_BUFF_SIZE);
		if(str_cycle_value[i] == NULL)
			goto FreeBuff;
		memset(str_cycle_value[i], 0, MAX_BUFF_SIZE);
	}
	char* str_cycle_small_value[9];
	for(i = 0; i < 9; i++){
		str_cycle_small_value[i] = (char *)malloc(SMALL_BUFF_SIZE);
		if(str_cycle_small_value[i] == NULL)
			goto FreeBuff;
		memset(str_cycle_small_value[i], 0, SMALL_BUFF_SIZE);
	}	
#endif

	static int cycle_index = 0;
	char str_cycle_tmp[32] = {0};
	char str_dmz_active[8] = {0};
	char str_dmz_ip[32] = {0};
#if defined(TCSUPPORT_AUTO_DMZ)
char dmz_flag[2] = {0};
#endif
	//for VirServer
	int vs_entry_index = 0;
	char vs_attrName[32] = {0};
	int MAX_VIRSERVER_NUM = 10;
	char str_vs_unit[32] = {0};
#ifdef WAN_DBUG
//	tcdbg_printf("wanInfo_read begin:%s,%s!\n", name[0], name[1]);
#endif
	parentNode = mxmlFindElement(top, top, name[0],
				NULL, NULL, MXML_DESCEND);
	if(parentNode==NULL){
		parentNode =mxmlNewElement(top,  name[0]);
		mxmlNewElement(parentNode,  WAN_IF_COMMON);
		mxmlNewElement(parentNode,  WAN_IF_MSG);
		mxmlNewElement(parentNode,  WAN_IF_CUR_IF);
		mxmlNewElement(parentNode,  WAN_IF_CUR_PVC);
	}

#ifdef TCSUPPORT_PORTBIND
	//initial portbind status string.
	int 	n = 0;
	char	strbinding[256] = {0};
	char	strbindtemp[128] = {0};
	int	iIPv4[14] = {0}, iIPv6[14] = {0};
	char 	portbinding_status[][5] = 
	{
		{"No"},
		{"No"},
		{"No"},
		{"No"},
		{"No"},
		{"No"},
		{"No"},
		{"No"},
#ifdef TCSUPPORT_WLAN_AC
		{"No"},
		{"No"},
		{"No"},
		{"No"},
		{"No"},
		{"No"},
#endif
	};
#if defined(TCSUPPORT_CT_DSLITE)
	int dslite = 0;
	int iIfindexV4[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
	int iIfindexV6[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
#endif
#endif

	/*Update valid interface index everytime*/
	valid_if_num = get_all_wan_index(valid_if, MAX_WAN_IF_INDEX);

#if defined(TCSUPPORT_CT_PMINFORM)
	if ( 0 == strcmp("INTERNET", name[1]) )
	{
		/* clear first */
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0], WAN_IF_NODE);
		strcpy(nodeName[1], "INTERNET");
		setAttrValue(top, nodeName, "UserID", "");

		for(i = 0; i < valid_if_num; i++)
		{
			pvc_index = valid_if[i] / MAX_SMUX_NUM;
			entry_index = valid_if[i] % MAX_SMUX_NUM;

			memset(nodeName, 0, sizeof(nodeName));
			strcpy(nodeName[0], WAN);
			sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
			sprintf(nodeName[2], "%s%d", WAN_INTERFACE, entry_index);

			if ( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "Active", str_active)
				|| 0 != strcmp(str_active, "Yes") )
				continue;
			if ( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "ServiceList", str_servicelist)
				|| NULL == strstr(str_servicelist, "INTERNET") )
				continue;
			if ( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "ISP", str_ispvalue)
				|| 0 != strcmp(str_ispvalue, PPP_MODE) )
				continue;

			getAttrValue(top, nodeName, "USERNAME", str_cycle_value[1]);

			memset(nodeName, 0, sizeof(nodeName));
			strcpy(nodeName[0], WAN_IF_NODE);
			strcpy(nodeName[1], "INTERNET");
			/* update info */
			setAttrValue(top, nodeName, "UserID", str_cycle_value[1]);

			break; /* only store the first internet info. */
		}

		goto FreeBuff;
	}
#endif

#if defined(TCSUPPORT_CT_WAN_PTM) || defined(TCSUPPORT_CT_E8B_ADSL)
	if ( 0 == strcmp(WAN_IF_COMMON, name[1]) ) // for enhance wan page visit fastly
	{
#endif
		for(i = 0; i < valid_if_num; i++){
			sprintf(tmp, "%d,", valid_if[i]);
			strcat(str_valid_if, tmp);
			
			/*Init wan info entry:update GUI interface name*/		
			pvc_index = valid_if[i] / MAX_SMUX_NUM;
			entry_index = valid_if[i] % MAX_SMUX_NUM;

		if(!strcmp(attr, "CycleValue")){
			memset(nodeName, 0, sizeof(nodeName));
			strcpy(nodeName[0],WAN);
			sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);

#if defined(TCSUPPORT_CT_WAN_PTM)
			if ( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, WAN_IF_ATMENABLE, str_atmenable)
				&& 0 == strcmp("Yes", str_atmenable) )
					transmode = 1;

			if ( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, WAN_IF_PTMENABLE, str_ptmenable)
				&& 0 == strcmp("Yes", str_ptmenable) )
					transmode = 2;

			if ( 0 == transmode )
				transmode = 1; // default for ATM mode.
#endif
#if defined(TCSUPPORT_CT_E8B_ADSL)
			transmode = 1;
#endif

#if !defined(TCSUPPORT_CT_PON)
			if(getAttrValue(top, nodeName, "VPI", str_vpi) != TCAPI_PROCESS_OK ){
				strcpy(str_vpi, "0");
			}
			if(getAttrValue(top, nodeName, "VCI", str_vci) != TCAPI_PROCESS_OK ){
				strcpy(str_vci, "35");
			}
			if(getAttrValue(top, nodeName, WAN_ENCAP, str_encap) != TCAPI_PROCESS_OK ){
				strcpy(str_encap, "N/A");
			}
#endif

			sprintf(nodeName[2], "%s%d", WAN_INTERFACE, entry_index);
			if(getAttrValue(top, nodeName, "WanMode", str_wanmode) != TCAPI_PROCESS_OK ){
				strcpy(str_wanmode, "Route");
			}				
			if(getAttrValue(top, nodeName, "ServiceList", str_servicelist) != TCAPI_PROCESS_OK ){
				strcpy(str_servicelist, "INTERNET");
			}
			if(getAttrValue(top, nodeName, WAN_IFNAME, str_ifname) != TCAPI_PROCESS_OK ){
				strcpy(str_ifname, "nasx_x");
			}	
			if(getAttrValue(top, nodeName, "Active", str_active) != TCAPI_PROCESS_OK ){
				strcpy(str_active, "N/A");
			}
			if(getAttrValue(top, nodeName, "NATENABLE", str_natenable) != TCAPI_PROCESS_OK ){
				strcpy(str_natenable, "N/A");
			}
			if(getAttrValue(top, nodeName, WAN_NASNAME, str_nasname) != TCAPI_PROCESS_OK ){
				strcpy(str_nasname, "nasx_x");
			}
			if(getAttrValue(top, nodeName, "IPVERSION", str_ipversion) != TCAPI_PROCESS_OK ){
				strcpy(str_ipversion, "N/A");
			}
			if ( 0 == strcmp(str_active, "Yes")
				&& 0 != strcmp(str_wanmode, "Route"))
			{
				allroute= 0;
			}
#if defined(TCSUPPORT_CT_PPPOEPROXY)
			strcpy(str_ppp_bi, "0");
			if ( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "pppProxyBiMode", str_ppp_bi)
				|| 0 == str_ppp_bi[0] )
				strcpy(str_ppp_bi, "0");
#else
			strcpy(str_ppp_bi, "No");
			if ( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "BridgeInterface", str_ppp_bi)
				|| 0 == str_ppp_bi[0] )
				strcpy(str_ppp_bi, "No");
#endif
#ifdef TCSUPPORT_PORTBIND			
#if defined(TCSUPPORT_CT_DSLITE)
			dslite = get_dslite_stateviacfg(top, nodeName);
#endif
			for (n = 0; n < sizeof(bl_map_data) / sizeof(bl_map_data[0]); n++) {
				if (getAttrValue(top, nodeName, bl_map_data[n].bindif, strbinding) == TCAPI_PROCESS_OK) {
					if (!strcmp(strbinding, "Yes")) {
						if(!strcmp(str_ipversion,"IPv4"))
						{
							iIPv4[n]++;
#if defined(TCSUPPORT_CT_DSLITE)
							iIfindexV4[n] = valid_if[i];
#endif
						}
						else if(!strcmp(str_ipversion,"IPv6"))
						{
#if defined(TCSUPPORT_CT_DSLITE)
							if ( dslite )
							{
								iIPv4[n]++;
								iIfindexV4[n] = valid_if[i];
							}
							iIfindexV6[n] = valid_if[i];
#endif
							iIPv6[n]++;
						}
						else if(!strcmp(str_ipversion,"IPv4/IPv6")){
							iIPv4[n]++;
							iIPv6[n]++;
#if defined(TCSUPPORT_CT_DSLITE)
							iIfindexV4[n] = valid_if[i];
							iIfindexV6[n] = valid_if[i];
#endif
						}
						strcpy(portbinding_status[n], "Yes");
					}
				}
			}
#endif
#if defined(TCSUPPORT_CT_PON) || defined(TCSUPPORT_CT_WAN_PTM) || defined(TCSUPPORT_CT_E8B_ADSL)
			if( getAttrValue(top, nodeName, WAN_IF_INDEX, str_ifIdx) != TCAPI_PROCESS_OK )
				sprintf(str_ifIdx, "%d", i+1);

			if( getAttrValue(top, nodeName, "CONNECTION", str_connection) != TCAPI_PROCESS_OK )
				strcpy(str_connection, "N/A");

			if( getAttrValue(top, nodeName, "LinkMode", str_linkmode) != TCAPI_PROCESS_OK )
				strcpy(str_linkmode, "N/A");

			if( getAttrValue(top, nodeName, "ISP", str_ispvalue) != TCAPI_PROCESS_OK )
				strcpy(str_ispvalue, "0");		

			if( getAttrValue(top, nodeName, "dot1q", str_vlanEnable) != TCAPI_PROCESS_OK )
				strcpy(str_vlanEnable, "N/A");

			if( getAttrValue(top, nodeName, "dot1p", str_priEnable) != TCAPI_PROCESS_OK )
				strcpy(str_priEnable, "N/A");

			strcpy(str_vid, "-");
			if( 0 == strcmp(str_vlanEnable, "Yes") )
			{
				if( getAttrValue(top, nodeName, "VLANID", str_vid) != TCAPI_PROCESS_OK )
					strcpy(str_vid, "-");
			}

			strcpy(str_pri, "-");
			if( 0 == strcmp(str_priEnable, "Yes") )
			{
				if( getAttrValue(top, nodeName, "dot1pData", str_pri) != TCAPI_PROCESS_OK )
					strcpy(str_pri, "-");
			}
			
			strcpy(str_mcvid, "-");
			if( getAttrValue(top, nodeName, "MulticastVID", str_mcvid) != TCAPI_PROCESS_OK )	//MulticastVID
				strcpy(str_mcvid, "-");

#endif

			memset(nodeName, 0, sizeof(nodeName));
			strcpy(nodeName[0],WAN_IF_NODE);
			sprintf(nodeName[1], "Entry%d", valid_if[i]);
#if defined(TCSUPPORT_CT_PON) || defined(TCSUPPORT_CT_WAN_PTM) || defined(TCSUPPORT_CT_E8B_ADSL)
#if defined(TCSUPPORT_CT_WAN_PTM) || defined(TCSUPPORT_CT_E8B_ADSL)
			if ( 2 == transmode ) // PTM
			{
#endif
				if( 0 == strcmp(str_wanmode, "Route") )
				{
					if( 0 == strcmp(str_vlanEnable, "Yes") )
						sprintf(str_guiinterfacename, "%s_%s_R_VID_%s", str_ifIdx, str_servicelist, str_vid);
					else
						sprintf(str_guiinterfacename, "%s_%s_R_VID_", str_ifIdx, str_servicelist);

					if( 0 == strcmp(str_linkmode, "linkIP") )
					{
						strcpy(str_connection, "Connect_Keep_Alive");
						if( 0 == strcmp(str_ispvalue, "0") )
							strcpy(str_obtipmode, "DHCP");
						else
							strcpy(str_obtipmode, "Static");
					}
					else
						strcpy(str_obtipmode, "PPPoE");
				}
				else
				{
					strcpy(str_connection, "Connect_Keep_Alive");
					if( 0 == strcmp(str_vlanEnable, "Yes") )
						sprintf(str_guiinterfacename, "%s_%s_B_VID_%s", str_ifIdx, str_servicelist, str_vid);
					else
						sprintf(str_guiinterfacename, "%s_%s_B_VID_", str_ifIdx, str_servicelist);

					str_obtipmode[0] = '\0';
				}
#if defined(TCSUPPORT_CT_WAN_PTM) || defined(TCSUPPORT_CT_E8B_ADSL)
			}
			else if ( 1 == transmode ) // ATM
			{
				if( 0 == strcmp(str_wanmode, "Route") )
				{
					sprintf(str_guiinterfacename, "%s_%s_R_%s_%s", str_ifIdx, str_servicelist, str_vpi, str_vci);

					if( 0 == strcmp(str_linkmode, "linkIP") )
					{
						strcpy(str_connection, "Connect_Keep_Alive");
						if( 0 == strcmp(str_ispvalue, "0") )
							strcpy(str_obtipmode, "DHCP");
						else
							strcpy(str_obtipmode, "Static");
					}
					else
						strcpy(str_obtipmode, "PPPoE");
				}
				else
				{
					strcpy(str_connection, "Connect_Keep_Alive");
					sprintf(str_guiinterfacename, "%s_%s_B_%s_%s",str_ifIdx, str_servicelist, str_vpi, str_vci);
					str_obtipmode[0] = '\0';
				}
			}
#endif
#else
			if(!strcmp(str_wanmode,"Route"))
				sprintf(str_guiinterfacename, "%d_%s_R_%s_%s", i+1, str_servicelist, str_vpi, str_vci);			
			else
				sprintf(str_guiinterfacename, "%d_%s_B_%s_%s", i+1, str_servicelist, str_vpi, str_vci);
#endif
			setAttrValue(top, nodeName, "GUIInterfaceName", str_guiinterfacename);
			
			if(getAttrValue(top, nodeName, "Status", str_status) != TCAPI_PROCESS_OK ){
				strcpy(str_status, "N/A");
			}			
			if(getAttrValue(top, nodeName, "IP", str_ip) != TCAPI_PROCESS_OK ){
				strcpy(str_ip, "N/A");
			}			
			if(getAttrValue(top, nodeName, "NetMask", str_netmask) != TCAPI_PROCESS_OK ){
				strcpy(str_netmask, "N/A");
			}			
			if(getAttrValue(top, nodeName, "GateWay", str_gateway) != TCAPI_PROCESS_OK ){
				strcpy(str_gateway, "N/A");
			}			
			if(getAttrValue(top, nodeName, "DNS", str_dns) != TCAPI_PROCESS_OK ){
				strcpy(str_dns, "N/A");
			}			
			if(getAttrValue(top, nodeName, "SecDNS", str_secdns) != TCAPI_PROCESS_OK ){
				strcpy(str_secdns, "N/A");
			}			
			
			if(getAttrValue(top, nodeName, "Status6", str_status6) != TCAPI_PROCESS_OK ){
				strcpy(str_status6, "N/A");
			}
			if( 0 != strcmp(str_wanmode, "Route") )
				strcpy(str_status6, str_status);

			if(getAttrValue(top, nodeName, "IP6", str_ip6) != TCAPI_PROCESS_OK ){
				strcpy(str_ip6, "N/A");
			}			
			
			if(getAttrValue(top, nodeName, "GateWay6", str_gateway6) != TCAPI_PROCESS_OK ){
				strcpy(str_gateway6, "N/A");
			}			
			
			if(getAttrValue(top, nodeName, "DNS6", str_dns6) != TCAPI_PROCESS_OK ){
				strcpy(str_dns6, "N/A");
			}			
			
			if(getAttrValue(top, nodeName, "SecDNS6", str_secdns6) != TCAPI_PROCESS_OK ){
				strcpy(str_secdns6, "N/A");
			}			
			
			if(getAttrValue(top, nodeName, "PD6", str_pd6) != TCAPI_PROCESS_OK ){
				strcpy(str_pd6, "N/A");
			}

			if ( TCAPI_PROCESS_OK 
				== getAttrValue(top, nodeName, "OrgPD6", str_orgpd6)
				&& 0 != str_orgpd6[0] )
				strcpy(str_pd6, str_orgpd6);

#if defined(TCSUPPORT_CT_PON) || defined(TCSUPPORT_CT_WAN_PTM) || defined(TCSUPPORT_CT_E8B_ADSL)
			if(getAttrValue(top, nodeName, "hwaddr", str_mac) != TCAPI_PROCESS_OK ){
				strcpy(str_mac, "N/A");
			}
#endif
		}

		if(!strcmp(attr, "CycleValue")){//WanInterface		
			sprintf(str_cycle_tmp, "%s,", str_guiinterfacename);
			strcat(str_cycle_value[13], str_cycle_tmp);
			sprintf(str_cycle_tmp, "%s,", str_ifname);
			strcat(str_cycle_small_value[4], str_cycle_tmp);
			sprintf(str_cycle_tmp, "%s,", str_active);
			strcat(str_cycle_small_value[5], str_cycle_tmp);
			sprintf(str_cycle_tmp, "%s,", str_natenable);
			strcat(str_cycle_small_value[6], str_cycle_tmp);
			sprintf(str_cycle_tmp, "%s,", str_nasname);
			strcat(str_cycle_small_value[7], str_cycle_tmp);
			sprintf(str_cycle_tmp, "%s,", str_ipversion);
			strcat(str_cycle_value[1], str_cycle_tmp);
			sprintf(str_cycle_tmp, "%s,", str_status);
			strcat(str_cycle_small_value[0], str_cycle_tmp);
			sprintf(str_cycle_tmp, "%s,", str_ip);
			strcat(str_cycle_value[2], str_cycle_tmp);
			sprintf(str_cycle_tmp, "%s,", str_netmask);
			strcat(str_cycle_value[3], str_cycle_tmp);
#if defined(TCSUPPORT_CT_PON)
			sprintf(str_cycle_tmp, "%s,", str_obtipmode);
			strcat(str_cycle_value[4], str_cycle_tmp);
#else
			sprintf(str_cycle_tmp, "%s,", str_encap);
			strcat(str_cycle_value[4], str_cycle_tmp);
#endif
			sprintf(str_cycle_tmp, "%s,", str_gateway);
			strcat(str_cycle_value[5], str_cycle_tmp);
			sprintf(str_cycle_tmp, "%s,", str_dns);
			strcat(str_cycle_value[6], str_cycle_tmp);
			sprintf(str_cycle_tmp, "%s,", str_secdns);
			strcat(str_cycle_value[7], str_cycle_tmp);
			sprintf(str_cycle_tmp, "%s,", str_status6);
			strcat(str_cycle_small_value[1], str_cycle_tmp);
			sprintf(str_cycle_tmp, "%s,", str_ip6);
			strcat(str_cycle_value[8], str_cycle_tmp);
			sprintf(str_cycle_tmp, "%s,", str_gateway6);
			strcat(str_cycle_value[9], str_cycle_tmp);
			sprintf(str_cycle_tmp, "%s,", str_dns6);
			strcat(str_cycle_value[10], str_cycle_tmp);
			sprintf(str_cycle_tmp, "%s,", str_secdns6);
			strcat(str_cycle_value[11], str_cycle_tmp);
			sprintf(str_cycle_tmp, "%s,", str_pd6);
			strcat(str_cycle_value[12], str_cycle_tmp);
#if defined(TCSUPPORT_CT_PON)
			sprintf(str_cycle_tmp, "%s,", str_ifIdx);
			strcat(str_cycle_small_value[2], str_cycle_tmp);
			sprintf(str_cycle_tmp, "%s,", "");
			strcat(str_cycle_small_value[3], str_cycle_tmp);
#else
			sprintf(str_cycle_tmp, "%s,", str_vpi);
			strcat(str_cycle_small_value[2], str_cycle_tmp);
			sprintf(str_cycle_tmp, "%s,", str_vci);
			strcat(str_cycle_small_value[3], str_cycle_tmp);
#endif

#if defined(TCSUPPORT_CT_PON) || defined(TCSUPPORT_CT_WAN_PTM) || defined(TCSUPPORT_CT_E8B_ADSL)
			sprintf(str_cycle_tmp, "%s,", str_connection);
			strcat(str_cycle_value[14], str_cycle_tmp);
			sprintf(str_cycle_tmp, "%s/%s,", str_vid, str_pri);
			strcat(str_cycle_small_value[8], str_cycle_tmp);
			sprintf(str_cycle_tmp, "%s,", str_mac);
			strcat(str_cycle_small_value[9], str_cycle_tmp);
			sprintf(str_cycle_tmp, "%s,", str_mcvid);
			strcat(str_cycle_small_value[10], str_cycle_tmp);
#endif
#if defined(TCSUPPORT_CT_PON)
			sprintf(str_cycle_tmp, "%s,", str_ppp_bi);
			strcat(str_cycle_small_value[11], str_cycle_tmp);
#endif
#if defined(TCSUPPORT_CT_WAN_PTM) || defined(TCSUPPORT_CT_E8B_ADSL)
			sprintf(str_cycle_tmp, "%s,", str_obtipmode);
			strcat(str_cycle_small_value[11], str_cycle_tmp);
			sprintf(str_cycle_tmp, "%s,", str_ifIdx);
			strcat(str_cycle_small_value[12], str_cycle_tmp);
			sprintf(str_cycle_tmp, "%d,", transmode);
			strcat(str_cycle_small_value[13], str_cycle_tmp);
#endif
			}
#if defined(TCSUPPORT_CT_FJ)
			else if(!strcmp(attr,"ITVInfo")){
				strcpy(nodeName[0],WAN);
				sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
				sprintf(nodeName[2], "%s%d", WAN_INTERFACE, entry_index);				
				if(getAttrValue(top, nodeName, "LAN2", tmp) == TCAPI_PROCESS_OK ){
					if ( 0 == strcmp(tmp, "Yes"))
					{
						if(getAttrValue(top, nodeName, WAN_IFNAME, str_ifname) != TCAPI_PROCESS_OK ){
							strcpy(str_ifname, "nasx_x");
						}
					}
				}
			}
			else if(!strcmp(attr,"ConnectionError")){
				strcpy(nodeName[0],WAN);
				sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
				sprintf(nodeName[2], "%s%d", WAN_INTERFACE, entry_index);				
				if(getAttrValue(top, nodeName, "ConnectionError", str_dmz_ip) != TCAPI_PROCESS_OK ){
					strcpy(str_dmz_ip, "N/A");
				}
				sprintf(str_cycle_tmp, "%s,", str_dmz_ip);
				strcat(str_cycle_value[14], str_cycle_tmp);
			}
#endif
			else if(!strcmp(attr, "DmzCycleValue")){//Dmz
				memset(nodeName, 0, sizeof(nodeName));
				strcpy(nodeName[0], "Dmz");
				sprintf(nodeName[1], "Entry%d", valid_if[i]);
				if(getAttrValue(top, nodeName, "Active", str_dmz_active) != TCAPI_PROCESS_OK ){
					strcpy(str_dmz_active, "N/A");
				}
				if(getAttrValue(top, nodeName, "DMZ_IP", str_dmz_ip) != TCAPI_PROCESS_OK ){
					strcpy(str_dmz_ip, "N/A");
				}
				sprintf(str_cycle_tmp, "%s,", str_dmz_active);
				strcat(str_cycle_small_value[8], str_cycle_tmp);
				sprintf(str_cycle_tmp, "%s,", str_dmz_ip);
				strcat(str_cycle_value[14], str_cycle_tmp);
			}
#if defined(TCSUPPORT_AUTO_DMZ)
			else if(!strcmp(attr,"AutoDMZ_Flag")){
				memset(nodeName, 0, sizeof(nodeName));
				strcpy(nodeName[0], "Dmz");
				sprintf(nodeName[1], "Entry%d", valid_if[i]);
				if(getAttrValue(top,nodeName,AUTO_DMZ_FLAG,dmz_flag) != TCAPI_PROCESS_OK){
					strcpy(dmz_flag,"0");
				}
				sprintf(str_cycle_tmp, "%s,", dmz_flag);
				strcat(str_cycle_small_value[8], str_cycle_tmp);
			}
#endif
		}
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0],WAN_IF_NODE);
		strcpy(nodeName[1], WAN_IF_COMMON);

#ifdef TCSUPPORT_PORTBIND	
		if(!strcmp(attr, "CycleValue")){
			memset(strbinding, 0, sizeof(strbinding));
			memset(strbindtemp, 0, sizeof(strbindtemp));
			for(n=0; n<(sizeof(portbinding_status)/(sizeof(portbinding_status[0]))); n++)
			{
				if('\0' == strbindtemp[0])
					strcpy(strbindtemp, portbinding_status[n]);
				else{
					strcat(strbindtemp, ",");
					strcat(strbindtemp, portbinding_status[n]);
				}
			}
			strcpy(strbinding, strbindtemp);
			memset(strbindtemp, 0, sizeof(strbindtemp));
			for(n=0; n<sizeof(bl_map_data) / sizeof(bl_map_data[0]); n++){
				sprintf(strbindtemp, ",%d,%d", iIPv4[n], iIPv6[n]);
				strcat(strbinding, strbindtemp);
			}
#if defined(TCSUPPORT_CT_DSLITE)
			for(n=0; n<sizeof(bl_map_data) / sizeof(bl_map_data[0]); n++)
			{
				sprintf(strbindtemp, ",%d,%d", iIfindexV4[n], iIfindexV6[n]);
				strcat(strbinding, strbindtemp);
			}
#endif
			//tcdbg_printf("%s : %s\r\n", __FUNCTION__, strbinding);
			setAttrValue(top, nodeName, "BindStatus", strbinding);
		}
#endif	
		setAttrValue(top, nodeName, WAN_IF_COM_VALID_INDEX, str_valid_if);
		sprintf(str_valid_num, "%d", valid_if_num);
		setAttrValue(top, nodeName, WAN_IF_COM_VALID_NUM, str_valid_num);

		/*Cycle read*/
		if(!strcmp(attr, "CycleNum")){
			setAttrValue(top, nodeName, "CycleNum", str_valid_num);
			cycle_index = 0;
		}
		if(!strcmp(attr, "CycleValue")){//WanInterface
			// jump cycle value index
			if ( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, WAN_IF_CYCLEJUMP, str_cyclejump) )
			{
				cycle_jump = atoi(str_cyclejump);
				setAttrValue(top, nodeName, WAN_IF_CYCLEJUMP, "0");
				if ( cycle_jump > 0 )
					cycle_index += cycle_jump;
				cycle_jump = 0;
			}
#if defined(TCSUPPORT_CT_PON) || defined(TCSUPPORT_CT_WAN_PTM) || defined(TCSUPPORT_CT_E8B_ADSL)
			if( cycle_index >= CYCLEVALUE_MAXCNT )
#else
			if(cycle_index >= 22)
#endif
				cycle_index = 0;
			if(cycle_index ==  0){
				strcpy(str_cycle_value[0], str_cycle_value[13]);
			}
			else if(cycle_index == 1){
				strcpy(str_cycle_value[0], str_cycle_small_value[4]);
			}
			else if(cycle_index == 2){
				strcpy(str_cycle_value[0], str_valid_if);
			}		
			else if(cycle_index == 3){
				strcpy(str_cycle_value[0], str_cycle_small_value[5]);
			}
			else if(cycle_index == 4){
				strcpy(str_cycle_value[0], str_cycle_small_value[6]);
			}
			else if(cycle_index == 5){
				strcpy(str_cycle_value[0], str_cycle_small_value[7]);
			}
			else if(cycle_index == 6){
				strcpy(str_cycle_value[0], str_cycle_value[1]);}
			else if(cycle_index == 7){
				strcpy(str_cycle_value[0], str_cycle_small_value[0]);}
			else if(cycle_index == 8){
				strcpy(str_cycle_value[0], str_cycle_value[2]);}
			else if(cycle_index == 9){
				strcpy(str_cycle_value[0], str_cycle_value[4]);}
			else if(cycle_index == 10){
				strcpy(str_cycle_value[0], str_cycle_value[5]);}
			else if(cycle_index == 11){
				strcpy(str_cycle_value[0], str_cycle_value[6]);}
			else if(cycle_index == 12){
				strcpy(str_cycle_value[0], str_cycle_value[7]);}
			else if(cycle_index == 13){
				strcpy(str_cycle_value[0], str_cycle_small_value[1]);}
			else if(cycle_index == 14){
				strcpy(str_cycle_value[0], str_cycle_value[8]);}
			else if(cycle_index == 15){
				strcpy(str_cycle_value[0], str_cycle_value[9]);}
			else if(cycle_index == 16){
				strcpy(str_cycle_value[0], str_cycle_value[10]);}
			else if(cycle_index == 17){
				strcpy(str_cycle_value[0], str_cycle_value[11]);}
			else if(cycle_index == 18){
				strcpy(str_cycle_value[0], str_cycle_value[12]);}
			else if(cycle_index == 19){
				strcpy(str_cycle_value[0], str_cycle_value[3]);}
			else if(cycle_index == 20){
				strcpy(str_cycle_value[0], str_cycle_small_value[2]);}
			else if(cycle_index == 21){
				strcpy(str_cycle_value[0], str_cycle_small_value[3]);}
#if defined(TCSUPPORT_CT_PON) || defined(TCSUPPORT_CT_WAN_PTM) || defined(TCSUPPORT_CT_E8B_ADSL)
			else if(cycle_index == 22)
				strcpy(str_cycle_value[0], str_cycle_value[14]);
			else if(cycle_index == 23)
				strcpy(str_cycle_value[0], str_cycle_small_value[8]);
			else if(cycle_index == 24)
				strcpy(str_cycle_value[0], str_cycle_small_value[9]);
			else if(cycle_index == 25)
				strcpy(str_cycle_value[0], str_cycle_small_value[10]);
#endif
#if defined(TCSUPPORT_CT_PON) || defined(TCSUPPORT_CT_WAN_PTM) || defined(TCSUPPORT_CT_E8B_ADSL)
			else if(cycle_index == 26)
				strcpy(str_cycle_value[0], str_cycle_small_value[11]);
#endif
#if defined(TCSUPPORT_CT_WAN_PTM) || defined(TCSUPPORT_CT_E8B_ADSL)
			else if(cycle_index == 27)
				strcpy(str_cycle_value[0], str_cycle_small_value[12]);
			else if(cycle_index == 28)
				strcpy(str_cycle_value[0], str_cycle_small_value[13]);
#endif
				setAttrValue(top, nodeName, "CycleValue", str_cycle_value[0]);
				cycle_index++;
			setAttrValue(top, nodeName, WAN_IF_ALLROUTE, allroute ? "1" : "0");
		}
#if defined(TCSUPPORT_CT_FJ)
		else if(!strcmp(attr,"ITVInfo")){
			strcpy(str_cycle_value[0], str_ifname);
			setAttrValue(top,nodeName,"ITVInfo",str_cycle_value[0]);
		}
		else if(!strcmp(attr,"ConnectionError")){
			strcpy(str_cycle_value[0], str_cycle_value[14]);
			setAttrValue(top,nodeName,"ConnectionError",str_cycle_value[0]);
		}
#endif
		else if(!strcmp(attr, "DmzCycleValue")){//Dmz
			if(cycle_index >= 2)
				cycle_index = 0;
			if(cycle_index == 0){
				strcpy(str_cycle_value[0], str_cycle_small_value[8]);
			}
			else if(cycle_index == 1){
				strcpy(str_cycle_value[0], str_cycle_value[14]);
			}
			setAttrValue(top, nodeName, "DmzCycleValue", str_cycle_value[0]);
			cycle_index++;	
		}
#if defined(TCSUPPORT_AUTO_DMZ)
		else if(!strcmp(attr,"AutoDMZ_Flag")){
			strcpy(str_cycle_value[0],str_cycle_small_value[8]);
			setAttrValue(top,nodeName,"AutoDMZ_Flag",str_cycle_value[0]);
		}
#endif
		else if(!strcmp(attr, "VirServerCycleValue")){//VirServer
			if(cycle_index >= 7)
				cycle_index = 0;	
			memset(nodeName, 0, sizeof(nodeName));
			strcpy(nodeName[0],WEBCURSET);
			strcpy(nodeName[1], SUB_NODE_NAME);	
#if defined(TCSUPPORT_VIR_SERVER)
			if(getAttrValue(top, nodeName, "nat_ifidx", str_cycle_tmp) != TCAPI_PROCESS_OK )
#else
			if(getAttrValue(top, nodeName, "nat_pvc", str_cycle_tmp) != TCAPI_PROCESS_OK )
#endif
			{
				strcpy(str_cycle_tmp, "0");
			}
			vs_entry_index = atoi(str_cycle_tmp);
			memset(nodeName, 0, sizeof(nodeName));
			strcpy(nodeName[0], "VirServer");
			sprintf(nodeName[1], "Entry%d", vs_entry_index);
			for(i = 0; i < MAX_VIRSERVER_NUM; i++){
				sprintf(nodeName[2], "Entry%d", i);
				if(cycle_index == 0){
					strcpy(vs_attrName, "Active");
				}
				else if(cycle_index == 1){
					strcpy(vs_attrName, "Protocal");
				}
				else if(cycle_index == 2){
					strcpy(vs_attrName, "SrcIP");
				}
				else if(cycle_index == 3){
					strcpy(vs_attrName, "STARTPORT");
				}
				else if(cycle_index == 4){
					strcpy(vs_attrName, "ENDPORT");
				}
				else if(cycle_index == 5){
					strcpy(vs_attrName, "LOCALIP");
				}
				else if(cycle_index == 6){
					strcpy(vs_attrName, "MapName");
				}
				if(getAttrValue(top, nodeName, vs_attrName, str_vs_unit) != TCAPI_PROCESS_OK ){
					strcpy(str_vs_unit, "N/A");
				}			
				sprintf(str_cycle_tmp, "%s,", str_vs_unit);
				strcat(str_cycle_value[0], str_cycle_tmp);
			}
			memset(nodeName, 0, sizeof(nodeName));
			strcpy(nodeName[0],WAN_IF_NODE);
			strcpy(nodeName[1], WAN_IF_COMMON);
			setAttrValue(top, nodeName, "VirServerCycleValue", str_cycle_value[0]);
			cycle_index++;			
		}
#if defined(TCSUPPORT_CT_WAN_PTM) || defined(TCSUPPORT_CT_E8B_ADSL)
		else if( 0 == strcmp(WAN_IF_UICUSTOM, attr) )
		{
			memset(nodeName, 0, sizeof(nodeName));
			strcpy(nodeName[0], WAN_IF_NODE);
			strcpy(nodeName[1], WAN_IF_COMMON);

			sprintf(str_uicustom, "%d,%d,%d,%d,%d,%d,%d", PVC_NUM, MAX_SMUX_NUM, UI_CONNECTION_MAXCNT,
														ATM_PVC_MAXCNT,PTM_PVC_MAXCNT, ATM_CONN_MAXCNT, PTM_CONN_MAXCNT);
			setAttrValue(top, nodeName, WAN_IF_UICUSTOM, str_uicustom);
		}
#endif

#if defined(TCSUPPORT_CT_WAN_PTM) || defined(TCSUPPORT_CT_E8B_ADSL)
	} // end for enhance wan page visit fastly
#endif

	/*Update WanIF node based on WanInfo_Common_CurIFIndex*/
	if(!strcmp(name[1], WAN_IF_CUR_IF) || !strcmp(name[1], WAN_IF_CUR_PVC)){
#if defined(TCSUPPORT_CT_WAN_PTM) || defined(TCSUPPORT_CT_E8B_ADSL)
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0], WAN_IF_NODE);
		strcpy(nodeName[1], WAN_IF_COMMON);

		if ( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, WAN_IF_STOPWANUPDATE, str_stopupdate) ) // for enhance wan page visit fastly
		{
			pause_updatewan = atoi(str_stopupdate);
			if ( 1 == pause_updatewan )
				goto FreeBuff;
		}
#endif
		
		/*Delete old WanIF node and create new*/
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0],WAN_IF_NODE);
		strcpy(nodeName[1], WAN_IF_CUR_IF);
		if((curNode = search_node(nodeName)) != NULL){		
			mxmlDelete(curNode);
		}		
		mxmlNewElement(parentNode,  nodeName[1]);		
		
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0],WAN_IF_NODE);
		strcpy(nodeName[1], WAN_IF_CUR_PVC);
		if((curNode = search_node(nodeName)) != NULL){		
			mxmlDelete(curNode);
		}		
		mxmlNewElement(parentNode,  nodeName[1]);
				
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0],WAN_IF_NODE);
		strcpy(nodeName[1], WAN_IF_COMMON);
		if(getAttrValue(top, nodeName, WAN_IF_COM_CUR_INDEX, str_cur_if) != TCAPI_PROCESS_OK ){
			/*if no CurIFIndex, set first valid index as default value*/
#if defined(TCSUPPORT_CUC_SFU)
			tcdbg_printf("wanInfo_read:No WanInfo_Common_CurIFIndex, choose the first valid index:%d\n", 1);
			sprintf(str_cur_if, "%d", 1);
#else
			tcdbg_printf("wanInfo_read:No WanInfo_Common_CurIFIndex, choose the first valid index:%d\n", valid_if[0]);
			sprintf(str_cur_if, "%d", valid_if[0]);
#endif
			setAttrValue(top, nodeName, WAN_IF_COM_CUR_INDEX, str_cur_if);
		}
		else
		{
			for(i = 0; i < valid_if_num; i++)
			{
				if(atoi(str_cur_if) == valid_if[i])
					break;
			}
		
			if(i == valid_if_num)
			{
				/*if CurIFIndex value is invalid, set first valid index as default value*/
				tcdbg_printf("wanInfo_read:Invalid WanInfo_Common_CurIFIndex, choose the first valid index:%d\n", valid_if[0]);
				sprintf(str_cur_if, "%d", valid_if[0]);
				setAttrValue(top, nodeName, WAN_IF_COM_CUR_INDEX, str_cur_if);
			}
		}
		
		if_index = atoi(str_cur_if);
		pvc_index = if_index / MAX_SMUX_NUM;
		entry_index = if_index % MAX_SMUX_NUM;
		
		/*Copy wan information from Wan_PVCx to WanInfo_WanIF*/
		memset(src_nodeName, 0, sizeof(src_nodeName));
		strcpy(src_nodeName[0],WAN);
		sprintf(src_nodeName[1], "%s%d", WAN_PVC, pvc_index);
		//if current wan pvc index is null, choose the first valid index 
		if(search_node(src_nodeName) == NULL) {
			//tcdbg_printf("wanInfo_read:WanInfo_Common_CurIFIndex has changed or deleted, choose the first valid index:%d\n", valid_if[0]);
			sprintf(str_cur_if, "%d", valid_if[0]);
			setAttrValue(top, nodeName, WAN_IF_COM_CUR_INDEX, str_cur_if);
			if_index = atoi(str_cur_if);
			pvc_index = if_index / MAX_SMUX_NUM;
			entry_index = if_index % MAX_SMUX_NUM;
			sprintf(src_nodeName[1], "%s%d", WAN_PVC, pvc_index);
		}
		
		memset(dst_nodeName, 0, sizeof(dst_nodeName));
		strcpy(dst_nodeName[0],WAN_IF_NODE);
		strcpy(dst_nodeName[1], WAN_IF_CUR_PVC);
		if(search_node(src_nodeName) != NULL)
			copy_node(dst_nodeName, src_nodeName);		
		/*Copy wan information from Wan_PVCx_Entryx to WanInfo_WanIF*/
		sprintf(src_nodeName[2], "%s%d", WAN_INTERFACE, entry_index);
		memset(dst_nodeName, 0, sizeof(dst_nodeName));
		strcpy(dst_nodeName[0],WAN_IF_NODE);
		strcpy(dst_nodeName[1], WAN_IF_CUR_IF);

		if(search_node(src_nodeName) != NULL)
			copy_node(dst_nodeName, src_nodeName);

		/*Copy wan information from WanInfo_EntryX when Dynamic or PPP mode*/
		
	}

FreeBuff:
#if defined(TCSUPPORT_CT_PON) || defined(TCSUPPORT_CT_WAN_PTM) || defined(TCSUPPORT_CT_E8B_ADSL)
	for(i = 0; i < MAX_BUFF_SIZE_CNT; i++)
	{
		if(str_cycle_value[i])
			free(str_cycle_value[i]);			
	}

	for(i = 0; i < SMALL_BUFF_CNT; i++)
	{
		if(str_cycle_small_value[i])
			free(str_cycle_small_value[i]);
	}
#else
	for(i = 0; i < 15; i++){
		if(str_cycle_value[i])
			free(str_cycle_value[i]);			
	}
	for(i = 0; i < 9; i++){
		if(str_cycle_small_value[i])
			free(str_cycle_small_value[i]);			
	}
#endif

#if defined(TCSUPPORT_CT_DHCPC_OPTION60)
	memset(nodeName, 0, sizeof(nodeName));	
	strcpy(nodeName[0], WEBCURSET);
	strcpy(nodeName[1], SUB_NODE_NAME);
	setAttrValue(top, nodeName, "isSupportDCOption60", "1");
#endif
	return SUCCESS;
}

int get_file_string(char *file_path, char line_return[][MAX_INFO_LENGTH], int line_num)
{
	FILE *fp = NULL;
	int i = 0;
	char line_buf[MAX_INFO_LENGTH] = {0};

	if(!file_path || line_num < 0)
		return FAIL;
	
	fp=fopen(file_path,"r");
	if(fp==NULL){		
		return FAIL;
	}
	while(fgets(line_buf, MAX_INFO_LENGTH, fp) != NULL && (line_num > 0)){
		if(line_buf[0] != '\n'){
			/*replace "\n" by NULL*/
			strtok(line_buf, "\n");
			strcpy(line_return[i], line_buf);
		}
		else{
			strcpy(line_return[i], "");
		}
		line_num--;
		i++;
		memset(line_buf, 0, sizeof(line_buf));
	}
	fclose(fp);
	/*Get nothing*/
	if(i == 0)
		return FAIL;
	
	return SUCCESS;
}

int save_wan_info(mxml_node_t *top, char *if_name)
{
	int if_index;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char info_string[2][MAX_INFO_LENGTH];
	char path[64] = {0}, pathtmp[128] = {0};

	if((if_index = get_wanindex_by_name(if_name)) < 0){
		return FAIL;
	}

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],WAN_IF_NODE);
	sprintf(nodeName[1], "Entry%d", if_index);		

	sprintf(path, "%s%s/", WAN_IF_PATH, if_name);
	
	/*Get interface ip */
	sprintf(pathtmp, "%s%s", path, "ip");
	memset(info_string, 0, sizeof(info_string));
	if(get_file_string(pathtmp, info_string, 1) == SUCCESS){		
		setAttrValue(top, nodeName, wan_info_node[WAN_IP_TYPE], info_string[0]);		
	}
	/*Get interface netmask*/
	sprintf(pathtmp, "%s%s", path, "netmask");
	memset(info_string, 0, sizeof(info_string));
	if(get_file_string(pathtmp, info_string, 1) == SUCCESS){	
		setAttrValue(top, nodeName, wan_info_node[WAN_NETMASK_TYPE], info_string[0]);
	}
	/*Get interface dns*/
	sprintf(pathtmp, "%s%s", path, "dns");
	memset(info_string, 0, sizeof(info_string));
	if(get_file_string(pathtmp, info_string, 2) == SUCCESS){		
		setAttrValue(top, nodeName, wan_info_node[WAN_DNS_TYPE], info_string[0]);
		setAttrValue(top, nodeName, wan_info_node[WAN_DNS2_TYPE], info_string[1]);
	}
	/*Get interface gateway*/
	sprintf(pathtmp, "%s%s", path, "gateway");
	memset(info_string, 0, sizeof(info_string));
	if(get_file_string(pathtmp, info_string, 1) == SUCCESS){		
		setAttrValue(top, nodeName, wan_info_node[WAN_GATEWAY_TYPE], info_string[0]);
	}
#if defined(TCSUPPORT_CT_PHONEAPP)
	updateMannualDNS(top, if_name, if_index);
#endif
	return SUCCESS;
}

int clear_wan_info(mxml_node_t *top, char *if_name)
{
	int if_index, i;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];

	if((if_index = get_wanindex_by_name(if_name)) < 0){
		return FAIL;
	}

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],WAN_IF_NODE);
	sprintf(nodeName[1], "Entry%d", if_index);	

	for(i = 1; i < 6; i++){
		setAttrValue(top, nodeName, wan_info_node[i], "");
	}
	return SUCCESS;
}

#ifdef IPV6
int isGetStaticWanPDMode(mxml_node_t *top, int ifindex,
							char *pd, char *pd_ptime, char *pd_vtime)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char dhcpv6_getpd[8] = {0}, dhcpv6_pdorig[64] = {0};
	int is_dhcpv6_static = 0;

	if ( !top || !pd || !pd_ptime || !pd_vtime )
		return -1;

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], "Wan");
	sprintf(nodeName[1], "PVC%d", ifindex / MAX_SMUX_NUM);
	sprintf(nodeName[2], "Entry%d", ifindex % MAX_SMUX_NUM);
	getAttrValue(top, nodeName, "DHCPv6PD", dhcpv6_getpd);
	getAttrValue(top, nodeName, "PDOrigin", dhcpv6_pdorig);

	if ( 0 == strcmp("No", dhcpv6_getpd)
		&& 0 == strcmp("Static", dhcpv6_pdorig) ) // static PD from wan interface
	{
		is_dhcpv6_static = 1;
		getAttrValue(top, nodeName, "PDPrefix", pd);
		getAttrValue(top, nodeName, "PrefixPltime", pd_ptime);
		getAttrValue(top, nodeName, "PrefixVltime", pd_vtime);
	}
	else
		is_dhcpv6_static = 0;

	return is_dhcpv6_static;
}

int save_wan_info6(mxml_node_t *top, char *if_name)
{
	int if_index;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char info_string[2][MAX_INFO_LENGTH];
	char path[64] = {0}, pathtmp[128] = {0};
#if defined(TCSUPPORT_CT_DHCP6C_STATUS_CODE)	
	char dhcp6cStr[16]={0};
	int dhcp6cErrorSw = DISABLE_DHCP6C_ERROR_HANDLE;
#endif
	char dhcpv6_static_pd[64] = {0};
	char dhcpv6_static_pd_ptime[16] = {0}, dhcpv6_static_pd_vtime[16] = {0};

	if((if_index = get_wanindex_by_name(if_name)) < 0){
		return FAIL;
	}

#if defined(TCSUPPORT_CT_DHCP6C_STATUS_CODE)	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],WAN);
	strcpy(nodeName[1], WAN_COMMON);
	if(getAttrValue(top, nodeName, "Dhcp6cErrorSw", dhcp6cStr) == TCAPI_PROCESS_OK){
		dhcp6cErrorSw = atoi(dhcp6cStr);	
	}	
#endif
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],WAN_IF_NODE);
	sprintf(nodeName[1], "Entry%d", if_index);		

	sprintf(path, "%s%s/", WAN_IF_PATH, if_name);

	/*Get interface ip6*/
	sprintf(pathtmp, "%s%s", path, "ip6");
	memset(info_string, 0, sizeof(info_string));
	if(get_file_string(pathtmp, info_string, 1) == SUCCESS){		
		setAttrValue(top, nodeName, wan_info_node[WAN_IP6_TYPE], info_string[0]);		
	}
	else{
#if defined(TCSUPPORT_CT_DHCP6C_STATUS_CODE)	
		if(dhcp6cErrorSw == DISABLE_DHCP6C_ERROR_HANDLE)
#endif
		{
			return FAIL;
		}
	}
	/*Get interface ip6 prefix length*/
	sprintf(pathtmp, "%s%s", path, "prefix6");
	memset(info_string, 0, sizeof(info_string));
	if(get_file_string(pathtmp, info_string, 1) == SUCCESS){
		setAttrValue(top, nodeName, wan_info_node[WAN_PFXLEN_TYPE], info_string[0]);
		
	}
	else{
#if defined(TCSUPPORT_CT_DHCP6C_STATUS_CODE)	
		if(dhcp6cErrorSw == DISABLE_DHCP6C_ERROR_HANDLE)
#endif
		{
			return FAIL;
		}
	}


	/*Get interface ipv6 PD*/
	sprintf(pathtmp, "%s%s", path, "pd6");
	memset(info_string, 0, sizeof(info_string));
	if(get_file_string(pathtmp, info_string, 1) == SUCCESS){
		setAttrValue(top, nodeName, wan_info_node[WAN_PD6_TYPE], info_string[0]);
			
	}

	sprintf(pathtmp, "%s%s", path, "orgpd6");
	memset(info_string, 0, sizeof(info_string));
	if ( get_file_string(pathtmp, info_string, 1) == SUCCESS )
		setAttrValue(top, nodeName, wan_info_node[WAN_ORGPD6_TYPE], info_string[0]);

	sprintf(pathtmp, "%s%s", path, "pd6_ptime");
	memset(info_string, 0, sizeof(info_string));
	if(get_file_string(pathtmp, info_string, 1) == SUCCESS)
		setAttrValue(top, nodeName, wan_info_node[WAN_PD6_PTIME_TYPE], info_string[0]);

	sprintf(pathtmp, "%s%s", path, "pd6_vtime");
	memset(info_string, 0, sizeof(info_string));
	if(get_file_string(pathtmp, info_string, 1) == SUCCESS)
		setAttrValue(top, nodeName, wan_info_node[WAN_PD6_VTIME_TYPE], info_string[0]);

	if ( 1 == isGetStaticWanPDMode(top, if_index, dhcpv6_static_pd,
								dhcpv6_static_pd_ptime, dhcpv6_static_pd_vtime) ) // static pd
	{
		setAttrValue(top, nodeName, wan_info_node[WAN_PD6_TYPE], dhcpv6_static_pd);
		setAttrValue(top, nodeName, wan_info_node[WAN_PD6_PTIME_TYPE], dhcpv6_static_pd_ptime);
		setAttrValue(top, nodeName, wan_info_node[WAN_PD6_VTIME_TYPE], dhcpv6_static_pd_vtime);
	}

#if defined(TCSUPPORT_CT_DSLITE)
	/* Get interface ds-lite info */
	sprintf(pathtmp, "%s%s", path, "dsliteaddr");
	memset(info_string, 0, sizeof(info_string));
	if( get_file_string(pathtmp, info_string, 1) == SUCCESS )
		setAttrValue(top, nodeName, wan_info_node[WAN_DSLITE_ADDR_TYPE], info_string[0]);

	sprintf(pathtmp, "%s%s", path, "dslitename");
	memset(info_string, 0, sizeof(info_string));
	if( get_file_string(pathtmp, info_string, 1) == SUCCESS )
		setAttrValue(top, nodeName, wan_info_node[WAN_DSLITE_NAME_TYPE], info_string[0]);
#endif

	/*Get interface ipv6 dns*/
	sprintf(pathtmp, "%s%s", path, "dns6");
	memset(info_string, 0, sizeof(info_string));
	if(get_file_string(pathtmp, info_string, 2) == SUCCESS){
		setAttrValue(top, nodeName, wan_info_node[WAN_6DNS_TYPE], info_string[0]);
		setAttrValue(top, nodeName, wan_info_node[WAN_6DNS2_TYPE], info_string[1]);
		
	}
	else
		return FAIL;
	/*Get interface ipv6 gateway*/
	sprintf(pathtmp, "%s%s", path, "gateway6");
	memset(info_string, 0, sizeof(info_string));
	if(get_file_string(pathtmp, info_string, 1) == SUCCESS){
		setAttrValue(top, nodeName, wan_info_node[WAN_GATEWAY6_TYPE], info_string[0]);
		
	}
	else
		return FAIL;

	return SUCCESS;
}

int clear_wan_info6(mxml_node_t *top, char *if_name)
{
	int if_index, i;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
#if defined(TCSUPPORT_CT_PPPERRCODE_ENHANCE)
	char pathtmp[128] = {0};
#endif

	if((if_index = get_wanindex_by_name(if_name)) < 0){
		return FAIL;
	}

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],WAN_IF_NODE);
	sprintf(nodeName[1], "Entry%d", if_index);	

	for(i = 7; strlen(wan_info_node[i]) != 0; i++){	
		setAttrValue(top, nodeName, wan_info_node[i], "");
	}
#if defined(TCSUPPORT_CT_PPPERRCODE_ENHANCE)
	sprintf(pathtmp, "rm -f %s%s/ipv6cp", WAN_IF_PATH, if_name);
	system(pathtmp);
#endif
	return SUCCESS;
}
#endif

#ifdef  TCSUPPORT_CT_WANNODE_MAPPING//tony add
int UpdateWanIndexInfo(mxml_node_t *top)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char PVCIndexTmp[10] = {'\0'};
	char EntryIndexTmp[10] = {'\0'};
	char VLanID[10] = {'\0'};
	char Active[10] = {'\0'};
	int PVCValue, EntryValue;
	int i, j, ret=0;
	char cWanpvc[50] = {0};
	memset(cWanpvc, 0, sizeof(cWanpvc));
		
	for(i = 0; i < PVC_NUM; i++){
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0],"Wan");
		sprintf(nodeName[1], "PVC%d", i);
		PVCValue = i+1;
		ret = getAttrValue(top, nodeName, "VLANID", VLanID);
		if(!ret)
		{
			memset(PVCIndexTmp, 0, sizeof(PVCIndexTmp));
			sprintf(PVCIndexTmp, "%d", PVCValue);
			setAttrValue(top, nodeName, PVC_INDEX, PVCIndexTmp);
		}
		
		for(j = 0; j < MAX_SMUX_NUM; j++){
			memset(nodeName[2], 0, sizeof(nodeName[2]));
			sprintf(nodeName[2], "Entry%d", j);
			EntryValue = j+1;
			ret = getAttrValue(top, nodeName, "Active", Active);
			if(!ret)
			{
				memset(EntryIndexTmp, 0, sizeof(EntryIndexTmp));
				sprintf(EntryIndexTmp, "%d", EntryValue);
				setAttrValue(top, nodeName, ENTRY_INDEX, EntryIndexTmp);
			}
		}
	}
	
	return 1;
}

#endif

#if defined(TCSUPPORT_CT_MONITORCOLLECTOR)
int isPPPParaList(mxml_node_t *top,int pvc, int entry){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];	
	char bridgeType[32]={0};
	char tmp[32] = {0};
	
	memset(tmp,0,sizeof(tmp));
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],WAN);
	sprintf(nodeName[1], "%s%d",WAN_PVC,pvc);			
	sprintf(nodeName[2], ENTRY_NODENAME,entry);		


	getAttrValue(top, nodeName, "ISP", tmp);
	
	tcdbg_printf("isPPPParaList:tmp=%s\n",tmp);
	if (0 == strcmp(tmp, PPP_MODE)){
		return 1;
	}

	
	if (0 == strcmp(tmp, BRIDGE_MODE)){
		getAttrValue(top,nodeName,"BridgeMode",bridgeType);
		
		if( 0 == strcmp(bridgeType,"PPPoE_Bridged"))
			return 1;
	}

	return 0;
}


int getParaListItem(char* paraList, int ip_ppp, int pvcIndex, int entryIndex, int item){
	char tmp[256] = {0};		
	char tmpValue[8] = {0};
	
	if(pvcIndex == -1 || entryIndex == -1){		
		tcdbg_printf("[%s:%d]pvcIndex = -1,entryIndex == -1,ERROR!!!\n",__FUNCTION__,__LINE__);
		return -1;
	}

	strcat(tmp,"InternetGatewayDevice.WANDevice.1.WANConnectionDevice.");
	memset(tmpValue, 0, sizeof(tmpValue));
	sprintf(tmpValue,"%d",pvcIndex + 1);
	strcat(tmp,tmpValue);

	switch(ip_ppp){
		case IP_WAN_TYPE:			
			strcat(tmp,".WANIPConnection.");
			break;
		case PPP_WAN_TYPE:
			strcat(tmp,".WANPPPConnection.");
			break;	
		default:			
			strcpy(paraList,"");
			tcdbg_printf("[%s:%d]ip_ppp = %d,ERROR!!!\n",__FUNCTION__,__LINE__,ip_ppp);
			return -1;
	}

	memset(tmpValue, 0, sizeof(tmpValue));
	sprintf(tmpValue,"%d.",entryIndex + 1);
	strcat(tmp,tmpValue);

	switch(item){
		case wan_ConnectionStatus:			
			strcat(tmp,"ConnectionStatus");
			break;
		case wan_LastConnectionError:
			strcat(tmp,"LastConnectionError");
			break;	
#if defined(TCSUPPORT_CT_PPPERRCODE_ENHANCE)
		case wan_IPv6ConnectionStatus:
#if defined(TCSUPPORT_CT_PON_GDV20)
			strcat(tmp,"X_CT-COM_IPv6ConnStatus");
#else
			strcat(tmp,"X_CT-COM_IPv6ConnectionStatus");
#endif
			break;
#if defined(TCSUPPORT_CT_PON_GDV20)
		case wan_LastConnectionErrorCode:
			strcat(tmp,"LastConnectionErrorCode");
			break;
#else
		case wan_ConnectionType:
			strcat(tmp,"ConnectionType");
			break;
#endif
#endif
		default:			
			strcpy(paraList,"");
			tcdbg_printf("[%s:%d]item = %d, not support!!!\n",__FUNCTION__,__LINE__,item);
			return -1;
	}

	strcpy(paraList,tmp);
	return 0;
}


void addWanInParaList(mxml_node_t *top, int ip_ppp, int pvcIndex, int entryIndex){
	char tmp[256] = {0};	
	char paraList[256] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char attr[32] = {0};	
	int iParaListNum = 0;
	int iParaListLen = 0;
	int i = 0;
	int j = 0;
	int ret = 0;
	int addWanItem = wan_ConnectionStatus;
	int len = 0;
	int len_add = 0;
	int addNum = 0;

	for(i = 0; i < MONITOR_COLLECTOR_MAXNUM; i++){		
		memset(tmp,0,sizeof(tmp));
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0],MONITOR_COLLECTOR_NODENAME);
		sprintf(nodeName[1], ENTRY_NODENAME,i);		
		ret = getAttrValue(top, nodeName, "ParaListNum", tmp);		
		if(ret != TCAPI_PROCESS_OK){
			continue;
		}

		iParaListNum = atoi(tmp);
		for(j = 0; j < iParaListNum; j++){	
			memset(attr, 0, sizeof(attr));
			sprintf(attr,"ParaList%d",j);
			getAttrValue(top, nodeName, attr, tmp);		
			if(strlen(tmp) == 0){			//add in the empty item
				memset(paraList,0,sizeof(paraList));
				getParaListItem(paraList,ip_ppp, pvcIndex, entryIndex,addWanItem);	
				if(strlen(paraList) != 0){					
					setAttrValue(top, nodeName, attr, paraList); 	
					len = strlen(paraList);
					len_add += len;
					addNum++;
					addWanItem++;
				}
			}		
			if(addWanItem >= max_paraList_wan_item){	
				//the wan instance(ConnectionStatus,LastConnectionError) is already added in paraList.
				break;
			}					
		}

		if(j >= iParaListNum){
			//the wan instance(ConnectionStatus,LastConnectionError) is not add into paraList yet.
			for(; addWanItem < max_paraList_wan_item; ){				
				memset(paraList,0,sizeof(paraList));
				getParaListItem(paraList,ip_ppp, pvcIndex, entryIndex,addWanItem);		
				if(strlen(paraList) != 0){
					memset(attr, 0, sizeof(attr));
					sprintf(attr,"ParaList%d",j);					
					setAttrValue(top, nodeName, attr, paraList); 	
					len = strlen(paraList);
					len_add += len;
					addNum++;
					j++;
					addWanItem++;
				}
			}
		}

		//set ParaListLen
		memset(tmp,0,sizeof(tmp));		
		getAttrValue(top, nodeName, "ParaListLen", tmp);	
		iParaListLen = atoi(tmp);
		iParaListLen += len_add + addNum ;
		sprintf(tmp,"%d",iParaListLen);
		setAttrValue(top, nodeName, "ParaListLen", tmp);	

		if(j > iParaListNum){		
			//add new ParaList item, need set ParaListNum
			memset(tmp,0,sizeof(tmp));
			sprintf(tmp,"%d",j);
			setAttrValue(top, nodeName, "ParaListNum", tmp);	
		}

		//only add in the first ParaList
		return;
	}

	return;
}


int isParaListItemNeedDel(char* paraList, int pvc_in, int entry_in){
	char tempvalue[8] = {0};
	char* ptr = NULL;
	int pvcIndex = 0;
	int entryIndex = 0;

	if((ptr = strstr(paraList,"WANConnectionDevice")) != NULL){
		memset(tempvalue, 0, sizeof(tempvalue));
		tempvalue[0] = *(ptr + strlen("WANConnectionDevice") + 1);
		tempvalue[1] = '\0';
		pvcIndex = atoi(tempvalue);

		//check pvc index
		if(pvc_in != pvcIndex - 1){
			return 0;
		}
		
			
		memset(tempvalue, 0, sizeof(tempvalue));
		if((ptr = strstr(paraList,"WANPPPConnection"))!= NULL){
			tempvalue[0] = *(ptr + strlen("WANPPPConnection") + 1);		
			tempvalue[1] = '\0';
			entryIndex = atoi(tempvalue);	
		}else if((ptr = strstr(paraList,"WANIPConnection"))!= NULL){
			tempvalue[0] = *(ptr + strlen("WANIPConnection") + 1);	
			tempvalue[1] = '\0';
			entryIndex = atoi(tempvalue);	
		}else{
			entryIndex = -1;
		}

		//check entry index
		if(entry_in != entryIndex - 1){
			return 0;
		}

		return 1;
	}

	return 0;
}



void delWanInParaList(mxml_node_t *top, int pvcIndex, int entryIndex){	
	char tmp[256] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char attr[32] = {0};	
	int iParaListNum = 0;
	int iParaListLen = 0;
	int i = 0;
	int j = 0;
	int ret = 0;
	int len = 0;
	int len_del = 0;
	int delNum = 0;

	for(i = 0; i < MONITOR_COLLECTOR_MAXNUM; i++){	
		delNum = 0;
		memset(tmp,0,sizeof(tmp));
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0],MONITOR_COLLECTOR_NODENAME);
		sprintf(nodeName[1], ENTRY_NODENAME,i);		
		ret = getAttrValue(top, nodeName, "ParaListNum", tmp);
		if(ret != TCAPI_PROCESS_OK){
			continue;
		}

		iParaListNum = atoi(tmp);
		for(j = 0; j < iParaListNum; j++){				
			memset(tmp,0,sizeof(tmp));
			memset(attr, 0, sizeof(attr));
			sprintf(attr,"ParaList%d",j);			
			getAttrValue(top, nodeName, attr, tmp);
			len = strlen(tmp);
			if(len != 0 && isParaListItemNeedDel(tmp, pvcIndex,entryIndex) == 1){				
				setAttrValue(top, nodeName, attr, "");
				len_del += len;
				delNum++;
			}			
		}


		if(delNum != 0){
			memset(tmp,0,sizeof(tmp));
			getAttrValue(top, nodeName, "ParaListLen", tmp);
			iParaListLen = atoi(tmp);
			iParaListLen = iParaListLen - len_del - delNum;
			sprintf(tmp,"%d",iParaListLen);			
			setAttrValue(top, nodeName, "ParaListLen", tmp);
#if defined(TCSUPPORT_CT_PON_GDV20)	
			iParaListNum =iParaListNum - delNum;
			sprintf(tmp,"%d",iParaListNum);	
			setAttrValue(top, nodeName, "ParaListNum", tmp);
#endif
		}
	}

	return;
}

void setMonitorCollectorParaList(mxml_node_t *top, int ip_ppp,int pvcIndex, int entryIndex, int type){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char srvList[64] = {0};	


	//check serviceList, only INTERNET	
	switch(type){
		case WAN_DEV_ADD:
			memset(nodeName, 0, sizeof(nodeName));
			strcpy(nodeName[0],WAN);
			sprintf(nodeName[1], "%s%d", WAN_PVC, pvcIndex);
			sprintf(nodeName[2], "%s%d", WAN_INTERFACE, entryIndex);
			getAttrValue(top, nodeName, "ServiceList", srvList);
			if(strstr(srvList,INTERNET) == NULL){
				//not INTERNET wan
				return;
			}
			addWanInParaList(top,ip_ppp,pvcIndex,entryIndex);
			break;
		case WAN_DEV_DEL:
			delWanInParaList(top,pvcIndex,entryIndex);
			break;	
		default:
			break;

	}

	return;
}
#endif

/*_____________________________________________________________________________
**      function name: deviceInfo_execute
**      descriptions:
**            Trigger CPE to update wan interface's  IP address or
**            Trigger CPE to release wan interface's IP address.
**
**      parameters:
**            top:   Put the top of cfg node tree that is used to execute system information.
**            name: Put the deviceInfo cfg node name.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**      	getAttrValue
**      	fileRead
**
**      revision:
**      1. jlliu 20100720
**____________________________________________________________________________
*/

#if defined(TCSUPPORT_CT_ADSL_HN) || defined(TCSUPPORT_CT_PON_CZ_NX)
int ModeSwitch_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr);
int ModeSwitch_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
void ToUpperCase(
	/*! Input string to be converted. */
	char *s)
{
	while (*s) {
		*s = (char)toupper(*s);
		++s;
	}
}
#endif

#if defined(TCSUPPORT_CT_PHONEAPP)
#define PHONE_MANNUALDNS "MannualDNS"

void setMannualDNS(mxml_node_t *top, int if_index){
	char wanDNSNode[MAX_ARG_NUM][MAX_NODE_NAME];
	char wanInfoNode[MAX_ARG_NUM][MAX_NODE_NAME];
	char dns1[20] = {0}, dns2[20] = {0};

	memset(wanInfoNode, 0, sizeof(wanInfoNode));
	strcpy(wanInfoNode[0], "WanInfo");
	sprintf(wanInfoNode[1], "Entry%d", if_index);

	if (getAttrValue(top, wanInfoNode, "GateWay", dns1) != TCAPI_PROCESS_OK || strlen(dns1) == 0) {
	#ifdef CT_COM_DBUG
		tcdbg_printf("%s:get gateway failed.\n", __FUNCTION__);
	#endif
		return;
	}
	
	memset(wanDNSNode, 0, sizeof(wanDNSNode));
	strcpy(wanDNSNode[0], "WanInfo");
	strcpy(wanDNSNode[1], PHONE_MANNUALDNS);
	memset(dns1, 0, sizeof(dns1));
	if (getAttrValue(top, wanDNSNode, "DNS1", dns1) == TCAPI_PROCESS_OK && strlen(dns1) > 0) {
		setAttrValue(top, wanInfoNode, wan_info_node[WAN_DNS_TYPE], dns1);
	}	
	
	if (getAttrValue(top, wanDNSNode, "DNS2", dns2) == TCAPI_PROCESS_OK && strlen(dns2) > 0) {
		setAttrValue(top, wanInfoNode, wan_info_node[WAN_DNS2_TYPE], dns2);
	}	
}

void updateMannualDNS(mxml_node_t *top, char *if_name, int if_index)
{
	char wanNode[MAX_ARG_NUM][MAX_NODE_NAME];
	char serviceApp[SERVICE_LIST_LEN] = {0};
	int pvc_index, entry_index;

	pvc_index = if_index / MAX_SMUX_NUM;
	entry_index = if_index - pvc_index * MAX_SMUX_NUM;
	
	memset(wanNode, 0, sizeof(wanNode));
	strcpy(wanNode[0], "Wan");
	sprintf(wanNode[1], "PVC%d", pvc_index);
	sprintf(wanNode[2], "Entry%d", entry_index);

	if (getAttrValue(top, wanNode, "ServiceList", serviceApp) != TCAPI_PROCESS_OK) {
	#ifdef CT_COM_DBUG
		tcdbg_printf("%s:get service list failed.\n", __FUNCTION__);
	#endif
		return;
	}
	
#if defined(TCSUPPORT_CT_SERVICELIST_E8C)
	if (strstr(serviceApp, INTERNET)!=NULL) {
#else
	if (!strcmp(serviceApp, INTERNET) || !strcmp(serviceApp, TR069_INTERNET)) {
#endif
		setMannualDNS(top, if_index);
	}
	
	return;
}

int MannualDNS_execute(mxml_node_t *top, char name[][MAX_NODE_NAME])
{
	char wanInfoNode[MAX_ARG_NUM][MAX_NODE_NAME];
	char def_route_index_v4[10] = {0};
	int if_index =0;

	//tcdbg_printf("%s:start!!\n", __FUNCTION__);
	memset(wanInfoNode, 0, sizeof(wanInfoNode));
	strcpy(wanInfoNode[0], "WanInfo");
	strcpy(wanInfoNode[1], "Common");
	/* get default route for ipv4 */
	memset(def_route_index_v4, 0, sizeof(def_route_index_v4));
	if(getAttrValue(top, wanInfoNode, "DefRouteIndexv4", def_route_index_v4) == TCAPI_PROCESS_OK){
		//tcdbg_printf("%s:route %s,%d!!\n", __FUNCTION__, def_route_index_v4, if_index);
		if(strlen(def_route_index_v4) > 0 && strcmp(def_route_index_v4, "N/A")){
			if_index = atoi(def_route_index_v4);
			//tcdbg_printf("%s:route %s,%d!!\n", __FUNCTION__, def_route_index_v4, if_index);
			setMannualDNS(top, if_index);
			#ifdef TCSUPPORT_DNSEACHPVC
			restart_dnsmasq();
			#endif
			return 0;
		}		
	}
	return -1;
}	
#endif


#if defined(TCSUPPORT_CT_PON_CN_JS) || defined(TCSUPPORT_CT_PON_CZ_GD)
#define PORT_MAP(x)	(1 << x)
/*_____________________________________________________________________________
**      function name: wan_check_portbind
**      descriptions:
**     	check the current wan port bind is the same as existed wan ?
**
**      parameters:
**            top: 			top cfg node
**		  cur_if:			current pvc
**
**      global:
**             bl_map_data
**
**      return:
**             void
**
**      call:
**
**      revision:
**      1. kevin.shi 20150610
**____________________________________________________________________________
*/
static void wan_check_portbind(mxml_node_t *top,int cur_if,int addFlag)
{
	char wanNode[MAX_ARG_NUM][MAX_NODE_NAME];
	char bl_buf[8] = {0};
	unsigned int portMask = 0;
	unsigned char pvc_idx = 0;
	unsigned char entry_idx = 0;
	unsigned int i,j,k= 0;
	int valid_if[MAX_WAN_IF_INDEX] = {0}, valid_if_num = 0;
	int array_size = (sizeof(bl_map_data) / sizeof(bl_map_data[0]));
	struct pvc_entry_list pelist[] = 
	{{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}
#ifdef TCSUPPORT_WLAN_AC
	 ,{-1,-1}
	 ,{-1,-1}
	 ,{-1,-1}
	 ,{-1,-1}
	 ,{-1,-1}
	 ,{-1,-1}
#endif
	};
	
	/* current wan */
	memset(wanNode, 0, sizeof(wanNode));
	strcpy(wanNode[0], WAN_IF_NODE);
	sprintf(wanNode[1], "%s", WAN_IF_CUR_IF);

	/* current wan port bind */
	for (i = 0; i < array_size; i++)
	{
		if (TCAPI_PROCESS_OK == getAttrValue(top,wanNode,bl_map_data[i].bindif,bl_buf))
		{
			if (!strcmp(bl_buf, "Yes"))
			{
				portMask |= PORT_MAP(i);
			}
		}
	}

	/* return if no port bind */
	if (0 == portMask)
	{
		return;
	}

	/* get all the valid wan index */
	valid_if_num = get_all_wan_index(valid_if,MAX_WAN_IF_INDEX);
	for(i = 0; i < valid_if_num; i++)
	{
		/* ignore itself except Add wan */
		if (!addFlag && (valid_if[i] == cur_if))
		{
			continue;
		}
		pvc_idx = (valid_if[i] / MAX_SMUX_NUM);
		entry_idx = (valid_if[i] % MAX_SMUX_NUM);

		memset(wanNode,0,sizeof(wanNode));
		strcpy(wanNode[0], WAN);
		sprintf(wanNode[1], "%s%d", WAN_PVC, pvc_idx);
		sprintf(wanNode[2], "%s%d", WAN_INTERFACE, entry_idx);

		/* check port bind */
		for (j = 0; j < array_size; j++)
		{
			memset(bl_buf, 0, sizeof(bl_buf));
			getAttrValue(top,wanNode,bl_map_data[j].bindif,bl_buf);
			if (!strcmp(bl_buf, "Yes"))
			{
				if (portMask & PORT_MAP(j))
				{
					/* unset the bind */
					setAttrValue(top,wanNode,bl_map_data[j].bindif,"No");
					pelist[k].pvcIdx = pvc_idx;
					pelist[k++].entryIdx = entry_idx;
					#ifdef WAN_DBUG
					tcdbg_printf("wan_check_portbind: found %s_%s_%s has binded the port %s\n",wanNode,wanNode[1],wanNode[2],bl_map_data[j].bindif);
					#endif
				}
			}
		}
	}
	
	/* commit the found PVC */
	if (k > 0)
	{
		k = 0;
		while (pelist[k].pvcIdx >= 0)
		{
			memset(wanNode, 0, sizeof(wanNode));
			strcpy(wanNode[0], WAN);
			sprintf(wanNode[1], "%s%d", WAN_PVC,pelist[k].pvcIdx);
			sprintf(wanNode[2], "%s%d",WAN_INTERFACE,pelist[k].entryIdx);
			wan_execute(top,wanNode);
			k++;
		}
	}
}
#endif

int
wanInfo_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char msg_if[16] = {0};
	char msg[2][MAX_NODE_NAME];
	char info_string[2][MAX_INFO_LENGTH];
	char str_cur_if[5] = {0}, str_action[10] = {0};
	int if_index;
	char path[64] = {0};
	int cur_if, pvc_index = 0, entry_index = 0;
	int i;
#if defined(TCSUPPORT_CT_PON)
	char str_vlanid[6] = {0};
	char str_cur_vlanid[6] = {0};
	char str_pbit[6] = {0};
	char str_cur_pbit[6] = {0};
#else
	char str_vpi[4] = {0}, str_vci[8] = {0};
	char str_cur_vpi[4] = {0}, str_cur_vci[8] = {0};
#if defined(TCSUPPORT_CT_WAN_PTM) || defined(TCSUPPORT_CT_E8B_ADSL)
	char str_vlanid[6] = {0};
	char str_cur_vlanid[6] = {0};
	char str_pbit[6] = {0};
	char str_cur_pbit[6] = {0};
	char str_atmenable[4] = {0};
	char str_cur_atmenable[4] = {0};
	char str_ptmenable[4] = {0};
	char str_cur_ptmenable[4] = {0};
	int transmode = 0; // ATM:1, PTM:2
	int errorCode = 0;
#endif
#endif
	char str_pppmanualstatus[16] = {0};
	char iffolder[32] = {0}, ifname[16] = {0};
	char cmd[64] = {0};
	
	int matchVpiVci = 0;
#ifdef  TCSUPPORT_CT_WANNODE_MAPPING//tony add	
	int needUpdate = 0;
#endif
	int entryNum = 0;
	int empty_pvc[PVC_NUM] = {0};
	int empty_entry[MAX_SMUX_NUM] = {0};
	mxml_node_t *curNode = NULL;
	tcapi_msg_t t_msg;

#if defined(TCSUPPORT_CT_WAN_STAT)
	char dev[16] = {0};
	char Mac[20] = {0};
	int if_exist = 0;
#endif

#if defined(TCSUPPORT_CT_PPPINFORM)
	cwmp_msg_t message;
	char servicelist[20] = {0}, wanmode[10] = {0};
	int inform_message = 0;
	int ret = 0;
#endif
#if defined(TCSUPPORT_CT_PHONEAPP)
	if(!strcmp(name[1], PHONE_MANNUALDNS)){
		tcdbg_printf("wanInfo_execute:MannualDNS_execute!!\n");
		return MannualDNS_execute(top, name);
	}
#endif
	/*Message Node*/
#if defined(TCSUPPORT_CT_ADSL_HN) || defined(TCSUPPORT_CT_PON_CZ_NX)
	if(!strcmp(name[1], "ModeSwitch"))
	{
		return ModeSwitch_execute(top, name);
	}
#endif
	if(!strcmp(name[1], WAN_IF_MSG)){		
		/*Allowed user to commit node like this:tcapi commit WanInfo_Message_nas1_2/4*/
		if(strlen(name[2]) == 0){
			tcdbg_printf("wanInfo_execute:failed get interface information from Message\n");
			return FAIL;
		}
		memset(msg, 0, sizeof(msg));
		splitName(name[2], msg, "/");		
		strcpy(msg_if, msg[0]);
		
		if((if_index = get_wanindex_by_name(msg_if)) < 0){
			tcdbg_printf("wanInfo_execute:failed get interface index from %s \n", msg_if);
			return FAIL;
		}
#ifdef WAN_DBUG
		tcdbg_printf("\nwanInfo_execute:Message:interface: %s, index:%d, ipversion:%s!\n", msg_if, if_index, msg[1]);
#endif
		
		/*  bridge mode */
		if (!strcmp(msg[1], "B")) {
			/*Get interface status from status file,ext:/var/run/nas1_2/status*/
			sprintf(path, "%s%s/status", WAN_IF_PATH, msg_if);	
			if(get_file_string(path, info_string, 1) == SUCCESS){
				/*Save status to WanInfo_EntryX node*/
				memset(nodeName, 0, sizeof(nodeName));
				strcpy(nodeName[0],WAN_IF_NODE);
				sprintf(nodeName[1], "Entry%d", if_index);		

				setAttrValue(top, nodeName, wan_info_node[WAN_STATUS_TYPE], info_string[0]);

				if(!strcmp(info_string[0], "up")){
					interface_state_change(top, BRIDGE_IF_STATE_UP, msg_if);
#ifdef WAN_DBUG
					tcdbg_printf("wanInfo_execute:Message:interface %s is up!\n", msg_if);
#endif					
				}
				else if(!strcmp(info_string[0], "down")){
					interface_state_change(top, BRIDGE_IF_STATE_DOWN, msg_if);
				}
				else{
					tcdbg_printf("wanInfo_execute:Message:unknow status:%s in %s\n", info_string[0], path);
				}
			}
			else{
				tcdbg_printf("wanInfo_execute:no interface status from %s\n", path);
			}
		}

		if(atoi(msg[1]) == 4){  /*IPv4 version*/
			/*Get interface status from status file,ext:/var/run/nas1_2/status*/
			sprintf(path, "%s%s/status", WAN_IF_PATH, msg_if);
			memset(info_string, 0, sizeof(info_string));
			
			if(get_file_string(path, info_string, 1) == SUCCESS){
				/*Save status to WanInfo_EntryX node*/
				memset(nodeName, 0, sizeof(nodeName));
				strcpy(nodeName[0],WAN_IF_NODE);
				sprintf(nodeName[1], "Entry%d", if_index);		

				setAttrValue(top, nodeName, wan_info_node[WAN_STATUS_TYPE], info_string[0]);

				if(!strcmp(info_string[0], "up")){
					save_wan_info(top, msg_if);
					interface_state_change(top, IF_STATE_UP, msg_if);
#ifdef WAN_DBUG
					tcdbg_printf("wanInfo_execute:Message:interface %s is up!\n", msg_if);
#endif					
				}
				else if(!strcmp(info_string[0], "down")){
					interface_state_change(top, IF_STATE_DOWN, msg_if);
					clear_wan_info(top, msg_if);
				}
				else{
					tcdbg_printf("wanInfo_execute:Message:unknow status:%s in %s\n", info_string[0], path);
				}

				
#if defined(TCSUPPORT_CT_WAN_STAT)
				if_exist = 1;
#endif
			}
			else{
				tcdbg_printf("wanInfo_execute:no interface status from %s\n", path);
			}
		}

#ifdef IPV6
		if(atoi(msg[1]) == 6){  /*IPv6 version*/
			/*Get interface IPv6 status from status file,ext:/var/run/nas1_2/status6*/
			sprintf(path, "%s%s/status6", WAN_IF_PATH, msg_if);
			memset(info_string, 0, sizeof(info_string));
			if(get_file_string(path, info_string, 1) == SUCCESS){
				/*Save status to WanInfo_EntryX node*/
				memset(nodeName, 0, sizeof(nodeName));
				strcpy(nodeName[0],WAN_IF_NODE);
				sprintf(nodeName[1], "Entry%d", if_index);		

				setAttrValue(top, nodeName, wan_info_node[WAN_STATUS6_TYPE], info_string[0]);

				if(!strcmp(info_string[0], "up")){
					if(save_wan_info6(top, msg_if) == SUCCESS){
						interface_state_change(top, IF_STATE_UP6, msg_if);
#ifdef WAN_DBUG
						tcdbg_printf("wanInfo_execute:Message:interface %s is up!\n", msg_if);
#endif	
					}
					else{
#ifdef WAN_DBUG
						tcdbg_printf("wanInfo_execute:Message:interface %s save info failed!\n", msg_if);
#endif	
					}
				
				}
				else if(!strcmp(info_string[0], "down")){
					interface_state_change(top, IF_STATE_DOWN6, msg_if);
					clear_wan_info6(top, msg_if);
				}
				else{
					tcdbg_printf("wanInfo_execute:Message:unknow status:%s in %s\n", info_string[0], path);
				}

				
#if defined(TCSUPPORT_CT_WAN_STAT)
				if_exist = 1;
#endif
			}
			else{
				tcdbg_printf("wanInfo_execute:no interface status6 from %s\n", path);
			}
		}

		
#endif		

#if defined(TCSUPPORT_CT_WAN_STAT)
		if(if_exist){
			sprintf(dev, "nas%d_%d", if_index/MAX_SMUX_NUM,if_index%MAX_SMUX_NUM);					
			setAttrValue(top, nodeName, "hwaddr", getWanInfo(Mac_type, dev, Mac));
			tcdbg_printf("wanInfo_execute:nodeName=%s_%s_%s:dev %s mac is %s!\n",nodeName[0],nodeName[1],nodeName[2], dev,Mac);
		}
#endif

	}

	/*WanInfo_WanIF commited by Web Page*/
	if(!strcmp(name[1], WAN_IF_CUR_PVC)){
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0],WAN_IF_NODE);
		strcpy(nodeName[1], WAN_IF_CUR_PVC);
		if(getAttrValue(top, nodeName, WAN_IF_CUR_Action, str_action) != TCAPI_PROCESS_OK ){
			return FAIL;
		}
#if defined(TCSUPPORT_CT_WAN_PTM)
		if ( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, WAN_IF_ATMENABLE, str_atmenable)
			&& 0 == strcmp("Yes", str_atmenable) )
			transmode = 1;

		if ( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, WAN_IF_PTMENABLE, str_ptmenable)
			&& 0 == strcmp("Yes", str_ptmenable) )
			transmode = 2;

		if ( 0 == transmode )
			return FAIL;
#endif
#if defined(TCSUPPORT_CT_E8B_ADSL)
		transmode = 1;
#endif

#if defined(TCSUPPORT_CT_PON)
		if(getAttrValue(top, nodeName, WAN_IF_VLANID, str_cur_vlanid) != TCAPI_PROCESS_OK ){
			return FAIL;
		}
		if(getAttrValue(top, nodeName, WAN_IF_PBIT, str_cur_pbit) != TCAPI_PROCESS_OK ){
			return FAIL;
		}
#else
#if defined(TCSUPPORT_CT_WAN_PTM)
		if ( 1 == transmode ) // ATM
		{
			if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, WAN_IF_ATMENABLE, str_cur_atmenable) )
				return FAIL;
#endif
			if(getAttrValue(top, nodeName, "VPI", str_cur_vpi) != TCAPI_PROCESS_OK ){
				return FAIL;
			}
			if(getAttrValue(top, nodeName, "VCI", str_cur_vci) != TCAPI_PROCESS_OK ){
				return FAIL;
			}
#if defined(TCSUPPORT_CT_WAN_PTM)
		}
		else if ( 2 == transmode ) // PTM
		{
			if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, WAN_IF_PTMENABLE, str_cur_ptmenable) )
				return FAIL;
			if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, WAN_IF_VLANID, str_cur_vlanid) )
				return FAIL;
			if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, WAN_IF_PBIT, str_cur_pbit) )
				return FAIL;
		}
#endif
#endif

		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0],WAN_IF_NODE);
		strcpy(nodeName[1], WAN_IF_COMMON);
		if(getAttrValue(top, nodeName, WAN_IF_COM_CUR_INDEX, str_cur_if) != TCAPI_PROCESS_OK ){
			return FAIL;
		}
		cur_if = atoi(str_cur_if);
				
		for(i = 0; i < PVC_NUM; i++){
			pvc_index = i;
			memset(nodeName, 0, sizeof(nodeName));
			strcpy(nodeName[0],WAN);
			sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);	
#if defined(TCSUPPORT_CT_PON)
			if(getAttrValue(top, nodeName, WAN_IF_VLANID, str_vlanid) != TCAPI_PROCESS_OK ){
				continue;
			}
			if(getAttrValue(top, nodeName, WAN_IF_PBIT, str_pbit) != TCAPI_PROCESS_OK ){
				continue;
			}

			if( 0 == strcmp(str_vlanid, str_cur_vlanid)
				&& 0 == strcmp(str_pbit, str_cur_pbit))
			{
				matchVpiVci = 1;
				break;
			}
#else
#if defined(TCSUPPORT_CT_WAN_PTM)
			if ( 1 == transmode ) // ATM
			{
				if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, WAN_IF_ATMENABLE, str_atmenable) )
					continue;
				if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "VPI", str_vpi) )
					continue;
				if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "VCI", str_vci) )
					continue;

				if( 0 == strcmp(str_vpi, str_cur_vpi) &&  0 == strcmp(str_vci, str_cur_vci)
					&& 0 == strcmp(str_atmenable, str_cur_atmenable) )
				{
					matchVpiVci = 1;
					break;
				}
			}
			else if ( 2 == transmode ) // PTM
			{
				if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, WAN_IF_PTMENABLE, str_ptmenable) )
					continue;
				if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, WAN_IF_VLANID, str_vlanid) )
					continue;
				if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, WAN_IF_PBIT, str_pbit) )
					continue;

				if( 0 == strcmp(str_vlanid, str_cur_vlanid) && 0 == strcmp(str_pbit, str_cur_pbit)
					&& 0 == strcmp(str_ptmenable, str_cur_ptmenable) )
				{
					matchVpiVci = 1;
					break;
				}
			}
#else
			if(getAttrValue(top, nodeName, "VPI", str_vpi) != TCAPI_PROCESS_OK ){
				continue;
			}
			if(getAttrValue(top, nodeName, "VCI", str_vci) != TCAPI_PROCESS_OK ){
				continue;
			}
			if(!strcmp(str_vpi,str_cur_vpi) && !strcmp(str_vci,str_cur_vci)){
				matchVpiVci = 1;
				break;
			}
#endif
#endif
		}

		if(!strcmp(str_action, WAN_IF_MODIFY)){
			if(strlen(str_cur_if) == 0)
				return FAIL;
#if defined(TCSUPPORT_CT_PON_CN_JS) || defined(TCSUPPORT_CT_PON_CZ_GD)
			wan_check_portbind(top,cur_if,0);
#endif

#if defined(TCSUPPORT_CT_PON_GS)
			matchVpiVci = 1;
			pvc_index = cur_if / MAX_SMUX_NUM;
#endif
			/*Modify a wan interface config*/
			if(matchVpiVci == 1){
				if(pvc_index == cur_if / MAX_SMUX_NUM){
					#if/*TCSUPPORT_COMPILR*/ defined(TCSUPPORT_CT_PPPINFORM)
					ret = check_modifyinterface_wanmode(cur_if);
					if(ret < 0)
						return FAIL;
					#endif/*TCSUPPORT_COMPILR*/
					if(copy_wan_info(pvc_index, cur_if % MAX_SMUX_NUM) != SUCCESS)
						return FAIL;
					else{
						char cWanpvc[50] = {0};
						sprintf(cWanpvc, "Wan_PVC%d_Entry%d", pvc_index,cur_if%MAX_SMUX_NUM);
						tcapi_commit_internal(top, cWanpvc);
#if defined(TCSUPPORT_C1_CUC) && defined(TCSUPPORT_SYSLOG)
						AddSyslogForNewIFConfig(top, pvc_index, cur_if%MAX_SMUX_NUM, "modified");
#endif
					}
#if defined(TCSUPPORT_CT_MONITORCOLLECTOR)
#if defined(TCSUPPORT_CT_PON_CZ_GD)
					setMonitorCollectorParaList(top,WAN_TYPE_NOCARE, cur_if / MAX_SMUX_NUM, cur_if % MAX_SMUX_NUM, WAN_DEV_DEL);
					if(isPPPParaList(top,pvc_index, cur_if % MAX_SMUX_NUM))
						setMonitorCollectorParaList(top,PPP_WAN_TYPE, pvc_index, cur_if % MAX_SMUX_NUM, WAN_DEV_ADD);
					else
						setMonitorCollectorParaList(top,IP_WAN_TYPE, pvc_index, cur_if % MAX_SMUX_NUM, WAN_DEV_ADD);
#endif
#endif
				}
				else{
					entryNum = get_entry_numberByPvc(pvc_index, empty_entry);
					if(entryNum == MAX_SMUX_NUM){
						set_wan_err(1);
						return FAIL;
					}
					else{
						if(copy_wan_info(pvc_index, empty_entry[0]) != SUCCESS)
							return FAIL;
						else{
							char cWanpvc[50] = {0};
							sprintf(cWanpvc, "Wan_PVC%d_Entry%d", pvc_index,entryNum);
							tcapi_commit_internal(top, cWanpvc);
#if defined(TCSUPPORT_C1_CUC) && defined(TCSUPPORT_SYSLOG)
							AddSyslogForNewIFConfig(top, pvc_index,entryNum,"modified");
#endif
							//set curifindex is the new added
							memset(nodeName, 0, sizeof(nodeName));
							memset(cWanpvc, 0, sizeof(cWanpvc));
							strcpy(nodeName[0],WAN_IF_NODE);
							strcpy(nodeName[1], WAN_IF_COMMON);
							sprintf(cWanpvc, "%d", (int)(pvc_index*MAX_SMUX_NUM+empty_entry[0]));
							setAttrValue(top, nodeName, WAN_IF_COM_CUR_INDEX, cWanpvc);

#if defined(TCSUPPORT_CT_MONITORCOLLECTOR)
#if defined(TCSUPPORT_CT_PON_CZ_GD)
							setMonitorCollectorParaList(top,WAN_TYPE_NOCARE, cur_if / MAX_SMUX_NUM, cur_if % MAX_SMUX_NUM, WAN_DEV_DEL);
#endif
							if(isPPPParaList(top,pvc_index,empty_entry[0]))
								setMonitorCollectorParaList(top,PPP_WAN_TYPE,pvc_index, empty_entry[0], WAN_DEV_ADD);
							else
								setMonitorCollectorParaList(top,IP_WAN_TYPE,pvc_index, empty_entry[0], WAN_DEV_ADD);
#endif
#if defined(TCSUPPORT_CT_WANNINDEX_INCREASE)
													setLastUsedEntryIndex(pvc_index,empty_entry[0]);
#endif

						//delete old entry
						/*************************
						if vpi and vci appear before. but not equal current pvc, so add a new interface at fiest,
						then delete the current interface,to go the delete action.
						*************************/
							strcpy(str_action, WAN_IF_DEL);
							pvc_index = cur_if/MAX_SMUX_NUM;
							matchVpiVci = 1;
#ifdef  TCSUPPORT_CT_WANNODE_MAPPING//tony add
							needUpdate = 1;
#endif
						}
					}				
				}
			}
			else{
#if defined(TCSUPPORT_CT_WAN_PTM) || defined(TCSUPPORT_CT_E8B_ADSL)
				if ( 1 == isPVCFullviaTransMode(transmode, &errorCode) )
				{
					char id_entry[MAX_SMUX_NUM] = {0};
					int entryNum = 0;
					pvc_index = cur_if / MAX_SMUX_NUM;

					entryNum = get_entry_numberByPvc(pvc_index,id_entry);
					if (entryNum >=2) {
						set_wan_err(errorCode);
						return FAIL;
					}
					else {/* only one entry case */
						entry_index = cur_if%MAX_SMUX_NUM;
						strcpy(nodeName[0],WAN);
						sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
						sprintf(nodeName[2], "%s%d", WAN_INTERFACE, entry_index);
						wan_pre_unset(top,nodeName);
						if(copy_wan_info(pvc_index, entry_index) != SUCCESS)
							return FAIL;
						else{
							char cWanpvc[50] = {0};
							sprintf(cWanpvc, "Wan_PVC%d_Entry%d", pvc_index, entry_index);
							tcapi_commit_internal(top, cWanpvc);
#if defined(TCSUPPORT_C1_CUC) && defined(TCSUPPORT_SYSLOG)
							AddSyslogForNewIFConfig(top, pvc_index, entry_index, "added");
#endif
							//set curifindex is the new added
							memset(nodeName, 0, sizeof(nodeName));
							memset(cWanpvc, 0, sizeof(cWanpvc));
							strcpy(nodeName[0],WAN_IF_NODE);
							strcpy(nodeName[1], WAN_IF_COMMON);
							sprintf(cWanpvc, "%d", pvc_index*MAX_SMUX_NUM);
							setAttrValue(top, nodeName, WAN_IF_COM_CUR_INDEX, cWanpvc);
							//delete old entry
							/*************************
							if vpi and vci didn't appear before. add a new interface at fiest,
							then delete the current interface,to go the delete action.
							*************************/
							//strcpy(str_action, WAN_IF_DEL);
							pvc_index = cur_if/MAX_SMUX_NUM;
							matchVpiVci = 1;

#if defined(TCSUPPORT_CT_BRIDGEARP_NOFWD_LAN)
							isNeedChgArpRule = 1;
#endif
							set_wan_err(0);	
							return SUCCESS;
						}
					}
				}
#endif
				if(get_pvc_totalNumber(empty_pvc) == PVC_NUM){
#if defined(TCSUPPORT_CT_PON)
					int id_entry[MAX_SMUX_NUM] = {0};
					int entryNum = 0;
					pvc_index = cur_if / MAX_SMUX_NUM;

					entryNum = get_entry_numberByPvc(pvc_index,id_entry);
					if (entryNum >=2) {
						set_wan_err(2);
						return FAIL;
					}
					else {/* only one entry case */
						pvc_index = cur_if / MAX_SMUX_NUM;
						entry_index = cur_if%MAX_SMUX_NUM;
						strcpy(nodeName[0],WAN);
						sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
						sprintf(nodeName[2], "%s%d", WAN_INTERFACE, entry_index);
						wan_pre_unset(top,nodeName);
						if(copy_wan_info(pvc_index, entry_index) != SUCCESS)
							return FAIL;
						else{
							char cWanpvc[50] = {0};
							sprintf(cWanpvc, "Wan_PVC%d_Entry%d", pvc_index,entry_index);
							tcapi_commit_internal(top, cWanpvc);
#if defined(TCSUPPORT_C1_CUC) && defined(TCSUPPORT_SYSLOG)
							AddSyslogForNewIFConfig(top, pvc_index, entry_index, "added");
#endif
							memset(nodeName, 0, sizeof(nodeName));
							memset(cWanpvc, 0, sizeof(cWanpvc));
							strcpy(nodeName[0],WAN_IF_NODE);
							strcpy(nodeName[1], WAN_IF_COMMON);
							sprintf(cWanpvc, "%d", pvc_index*MAX_SMUX_NUM);
							setAttrValue(top, nodeName, WAN_IF_COM_CUR_INDEX, cWanpvc);
							pvc_index = cur_if/MAX_SMUX_NUM;
							matchVpiVci = 1;
						}
					}
#else
					set_wan_err(2);
					return FAIL;					
#endif
				}
				else{
					if(copy_wan_info(empty_pvc[0], 0) != SUCCESS)
						return FAIL;	
					else{						
						char cWanpvc[50] = {0};
						sprintf(cWanpvc, "Wan_PVC%d_Entry0", empty_pvc[0]);
						tcapi_commit_internal(top, cWanpvc);
#if defined(TCSUPPORT_C1_CUC) && defined(TCSUPPORT_SYSLOG)
						AddSyslogForNewIFConfig(top, empty_pvc[0], 0, "added");
#endif
						//set curifindex is the new added
						memset(nodeName, 0, sizeof(nodeName));
						memset(cWanpvc, 0, sizeof(cWanpvc));
						strcpy(nodeName[0],WAN_IF_NODE);
						strcpy(nodeName[1], WAN_IF_COMMON);
						sprintf(cWanpvc, "%d", empty_pvc[0]*MAX_SMUX_NUM);
						setAttrValue(top, nodeName, WAN_IF_COM_CUR_INDEX, cWanpvc);
						
#if defined(TCSUPPORT_CT_MONITORCOLLECTOR)
#if defined(TCSUPPORT_CT_PON_CZ_GD)
						setMonitorCollectorParaList(top,WAN_TYPE_NOCARE, cur_if / MAX_SMUX_NUM, cur_if % MAX_SMUX_NUM, WAN_DEV_DEL);
#endif
						if(isPPPParaList(top,empty_pvc[0],0))
							setMonitorCollectorParaList(top,PPP_WAN_TYPE,empty_pvc[0], 0, WAN_DEV_ADD);
						else
							setMonitorCollectorParaList(top,IP_WAN_TYPE,empty_pvc[0], 0, WAN_DEV_ADD);
#endif
#if defined(TCSUPPORT_CT_WANNINDEX_INCREASE)
							setLastUsedPvcIndex(empty_pvc[0]);
#endif
						//delete old entry
						/*************************
						if vpi and vci didn't appear before. add a new interface at fiest,
						then delete the current interface,to go the delete action.
						*************************/
						strcpy(str_action, WAN_IF_DEL);
						pvc_index = cur_if/MAX_SMUX_NUM;
						matchVpiVci = 1;
#ifdef  TCSUPPORT_CT_WANNODE_MAPPING//tony add
						needUpdate = 1;
#endif
					}	
				}			
			}
		}
		else if(!strcmp(str_action, WAN_IF_ADD)){
#if defined(TCSUPPORT_CT_PON_CN_JS) || defined(TCSUPPORT_CT_PON_CZ_GD)
			wan_check_portbind(top,cur_if,1);
#endif

#if defined(TCSUPPORT_CT_PON_GS)
			matchVpiVci = 0;
#endif
			/*Add a new wan interface*/
			if(matchVpiVci == 1){
				entryNum = get_entry_numberByPvc(pvc_index, empty_entry);
				if(entryNum == MAX_SMUX_NUM){
					set_wan_err(3);
					return FAIL;
				}
				else{
					if(copy_wan_info(pvc_index, empty_entry[0]) != SUCCESS)
						return FAIL;

					char cWanpvc[50] = {0};
					memset(nodeName, 0, sizeof(nodeName));
					strcpy(nodeName[0],WAN_IF_NODE);
					strcpy(nodeName[1], WAN_IF_COMMON);
					sprintf(cWanpvc, "%d", pvc_index*MAX_SMUX_NUM+empty_entry[0]);
					setAttrValue(top, nodeName, WAN_IF_COM_CUR_INDEX, cWanpvc);
						

#if defined(TCSUPPORT_CT_MONITORCOLLECTOR)
					if(isPPPParaList(top,pvc_index,empty_entry[0]))
						setMonitorCollectorParaList(top,PPP_WAN_TYPE,pvc_index, empty_entry[0], WAN_DEV_ADD);
					else
						setMonitorCollectorParaList(top,IP_WAN_TYPE,pvc_index, empty_entry[0], WAN_DEV_ADD);
#endif
#if defined(TCSUPPORT_CT_WANNINDEX_INCREASE)
					setLastUsedEntryIndex(pvc_index,empty_entry[0]);
#endif						
					memset(cWanpvc, 0, sizeof(cWanpvc));
					sprintf(cWanpvc, "Wan_PVC%d_Entry%d", pvc_index,empty_entry[0]);
					tcapi_commit_internal(top, cWanpvc);
#if defined(TCSUPPORT_C1_CUC) && defined(TCSUPPORT_SYSLOG)
					AddSyslogForNewIFConfig(top, pvc_index, entryNum,"added");
#endif
#ifdef  TCSUPPORT_CT_WANNODE_MAPPING//tony add
					needUpdate = 1;
#endif
				}
			}
			else{
#if defined(TCSUPPORT_CT_WAN_PTM) || defined(TCSUPPORT_CT_E8B_ADSL)
				if ( 1 == isPVCFullviaTransMode(transmode, &errorCode) )
				{
					set_wan_err(errorCode);
					return FAIL;
				}
#endif
				if(get_pvc_totalNumber(empty_pvc) == PVC_NUM){
					set_wan_err(4);
					return FAIL;
				}
				else{
					if(copy_wan_info(empty_pvc[0], 0) != SUCCESS)
					{
						tcdbg_printf("Waninfo_execute:add entry fail\n");
						return FAIL;
					}
					else
					{
						char cWanpvc[50] = {0};
						memset(nodeName, 0, sizeof(nodeName));
						strcpy(nodeName[0],WAN_IF_NODE);
						strcpy(nodeName[1], WAN_IF_COMMON);
						sprintf(cWanpvc, "%d", empty_pvc[0]*MAX_SMUX_NUM);
						setAttrValue(top, nodeName, WAN_IF_COM_CUR_INDEX, cWanpvc);
						
						
#if defined(TCSUPPORT_CT_MONITORCOLLECTOR)
						if(isPPPParaList(top,empty_pvc[0],0))
							setMonitorCollectorParaList(top,PPP_WAN_TYPE,empty_pvc[0], 0, WAN_DEV_ADD);
						else
							setMonitorCollectorParaList(top,IP_WAN_TYPE,empty_pvc[0], 0, WAN_DEV_ADD);
#endif
#if defined(TCSUPPORT_CT_WANNINDEX_INCREASE)
						setLastUsedPvcIndex(empty_pvc[0]);
#endif
						
						memset(cWanpvc, 0, sizeof(cWanpvc));
						sprintf(cWanpvc, "Wan_PVC%d_Entry0", empty_pvc[0]);
						tcapi_commit_internal(top, cWanpvc);
#if defined(TCSUPPORT_C1_CUC) && defined(TCSUPPORT_SYSLOG)
						AddSyslogForNewIFConfig(top, empty_pvc[0],0, "added");
#endif
#ifdef  TCSUPPORT_CT_WANNODE_MAPPING//tony add
						needUpdate = 1;
#endif
					}	
				}
			}
#if defined(TCSUPPORT_CT_PPPINFORM)
			inform_message = 1;
#endif
		}
		if(!strcmp(str_action, WAN_IF_DEL)){
			if(strlen(str_cur_if) == 0)
				return FAIL;
#if defined(TCSUPPORT_CT_PON_GS)
			matchVpiVci = 1;
			pvc_index = cur_if / MAX_SMUX_NUM;
#endif
			/*Del a wan interface*/
			if(matchVpiVci == 1){
				memset(nodeName, 0, sizeof(nodeName));
				strcpy(nodeName[0],WAN);
				sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
				sprintf(nodeName[2], "%s%d", WAN_INTERFACE, cur_if % MAX_SMUX_NUM);
				
				memset(&t_msg,0,sizeof(t_msg));
				sprintf(t_msg.node,"Wan_PVC%d_Entry%d",pvc_index, cur_if % MAX_SMUX_NUM);
#if defined(TCSUPPORT_C1_CUC) && defined(TCSUPPORT_SYSLOG)
				AddSyslogForNewIFConfig(top, pvc_index, cur_if % MAX_SMUX_NUM,"deleted");
#endif
#if defined(TCSUPPORT_CT_MONITORCOLLECTOR)
				//if(isNeedUpdateParaList(top,pvc_index, cur_if % MAX_SMUX_NUM))
					setMonitorCollectorParaList(top,WAN_TYPE_NOCARE,pvc_index, cur_if % MAX_SMUX_NUM, WAN_DEV_DEL);
#endif
        			tcapi_unset_req(top, &t_msg);
				//delete pvc if the pvc entry is empty
				if(0 == get_entry_numberByPvc(pvc_index, empty_entry)){
					/* find wan node */
					curNode = mxmlFindElement(top, top, nodeName[0],
						NULL, NULL, MXML_DESCEND_FIRST);
					/* find pvc node */
					curNode = mxmlFindElement(curNode, curNode, nodeName[1],
			NULL, NULL, MXML_DESCEND);
					if(curNode)
						mxmlDelete(curNode);
				}
#ifdef  TCSUPPORT_CT_WANNODE_MAPPING//tony add
				needUpdate = 1;
#endif
			}
			else{
				set_wan_err(99);
				return FAIL;	
			}
#if defined(TCSUPPORT_CT_PPPINFORM)
			inform_message = 1;
#endif
		}		
#if defined(TCSUPPORT_CT_BRIDGEARP_NOFWD_LAN)
		isNeedChgArpRule = 1;
#endif
	}
	if(!strcmp(name[1], WAN_IF_CUR_IF)){
		//1. get PPPManualStatus
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0],WAN_IF_NODE);
		strcpy(nodeName[1], WAN_IF_CUR_IF);
		if(getAttrValue(top, nodeName, WAN_IF_PPPMANUALSTATUS, str_pppmanualstatus) != TCAPI_PROCESS_OK ){
			return FAIL;
		}
		//2.set to Wan_PVCx_Entryx
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0],WAN_IF_NODE);
		strcpy(nodeName[1], WAN_IF_COMMON);
		if(getAttrValue(top, nodeName, WAN_IF_COM_CUR_INDEX, str_cur_if) != TCAPI_PROCESS_OK ){
			return FAIL;
		}
		cur_if = atoi(str_cur_if);
		pvc_index = cur_if/MAX_SMUX_NUM;
		entry_index = cur_if%MAX_SMUX_NUM;
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0],WAN);
		sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
		sprintf(nodeName[2], "%s%d", WAN_INTERFACE, entry_index);
		setAttrValue(top, nodeName, WAN_IF_PPPMANUALSTATUS, str_pppmanualstatus);
		//3.create interface.conf
		if(getAttrValue(top, nodeName, WAN_IFNAME, ifname) != TCAPI_PROCESS_OK ){
			return FAIL;
		}
		/*create wan interface folder by interface name,ext:/var/run/nas0_1*/
		sprintf(iffolder, "%s%s", WAN_IF_PATH, ifname);
		/*if(mkdir(iffolder, 0777) != 0){
			tcdbg_printf("%s: create %s folder failed!\n", __FUNCTION__, iffolder);
			return FAIL;
		}*/
		sprintf(path, "%s/%s", iffolder, "interface.conf");
		curNode = mxmlFindElement(top, top, nodeName[0],
	  		NULL, NULL,
	  		MXML_DESCEND);
		curNode = mxmlFindElement(curNode, curNode, nodeName[1],
	  		NULL, NULL,
	  		MXML_DESCEND);
		curNode = mxmlFindElement(curNode, curNode, nodeName[2],
	  		NULL, NULL,
	  		MXML_DESCEND);
		if(node_write(curNode, path, QMARKS) != 0){
			return FAIL;
			
		}
		//4.execute wan_start.sh
		sprintf(cmd, "%s %s %d %d %d",WAN_START_SH, ifname, pvc_index, 0, 1);
#ifdef WAN_DBUG
		tcdbg_printf("start_interface:cmd:%s\n", cmd );
#endif
		system(cmd);
#if defined(TCSUPPORT_CT_PPPCONN_ERROR)
		if(strcmp(str_pppmanualstatus,"disconnect") == 0){
			setAttrValue(top, nodeName, "ConnectionError", "ERROR_USER_DISCONNECT");
		}
#endif
	}
#ifdef  TCSUPPORT_CT_WANNODE_MAPPING//tony add	
	if(needUpdate)
	{
		UpdateWanIndexInfo(top);
	}
#endif
#if defined(TCSUPPORT_CT_PPPINFORM)
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0],WAN_IF_NODE);
		strcpy(nodeName[1], WAN_IF_CUR_IF);
		memset(servicelist, 0, sizeof(servicelist));
		if(getAttrValue(top, nodeName, "ServiceList", servicelist) != TCAPI_PROCESS_OK ){
			return FAIL;
		}
		memset(wanmode, 0, sizeof(wanmode));
		if(getAttrValue(top, nodeName, "WanMode", wanmode) != TCAPI_PROCESS_OK ){
			return FAIL;
		}
		if((inform_message == 1 && strcmp(servicelist, "INTERNET") == 0 && strcmp(wanmode, "Route") == 0) || ret == 1){
			memset(&message, 0, sizeof(message));
			message.cwmptype = 18;
			if(sendmegq(1, &message, 0) < 0){
				tcdbg_printf("\r\nsend message to cwmp error!!");
			}
		}
#endif

	set_wan_err(0);	
	return SUCCESS;
}

#if defined(TCSUPPORT_CT_ADSL_HN) || defined(TCSUPPORT_CT_PON_CZ_NX)
/*_____________________________________________________________________________
**      function name: ModeSwitch_read
**      descriptions:
**            do ModeSwitch attr read.
**
**      parameters:
**            top: 		Specify the root of mxml_node_t structure.
**		  name: 		attribute name
**		  attr: 		return attribute value
**
**      global:
**             None
**
**      return:
**		Success: 	0
**
**      revision:
**      1. haixiong.zhang 2014/12/28
**____________________________________________________________________________
*/
int ModeSwitch_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char tmp[129]={0};
	unsigned char found = 0;
	int i = 0;
	int retval = 0;
	int pvc_index = 0;
	int entry_index = 0;
	int valid_if[MAX_WAN_IF_INDEX] = {0};
	int valid_if_num = 0;

	if(!strcmp(attr, "CurMode"))
	{
		memset(nodeName,0,sizeof(nodeName));
		strcpy(nodeName[0], "WanInfo");
		strcpy(nodeName[1], "ModeSwitch");
		setAttrValue(top, nodeName, "CurMode", "None");

		valid_if_num = get_all_wan_index(valid_if, MAX_WAN_IF_INDEX);
		for(i = 0; i < valid_if_num; i++)
		{
			pvc_index = valid_if[i] / MAX_SMUX_NUM;
			entry_index = valid_if[i] % MAX_SMUX_NUM;
			memset(nodeName,0,sizeof(nodeName));
			strcpy(nodeName[0], WAN);
			sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
			sprintf(nodeName[2], "%s%d", WAN_INTERFACE, entry_index);
			memset(tmp, 0, sizeof(tmp));
			retval = getAttrValue(top, nodeName, "ServiceList", tmp);
			if(retval == TCAPI_PROCESS_OK)
			{
				ToUpperCase(tmp);
				if(strstr(tmp , "INTERNET"))
				{
					found = 1;
					break;
				}
			}
		}
		if(found)
		{
			memset(tmp, 0, sizeof(tmp));
			retval = getAttrValue(top, nodeName, "WanMode", tmp);
			if(retval == TCAPI_PROCESS_OK)
			{
				
				memset(nodeName,0,sizeof(nodeName));
				strcpy(nodeName[0], "WanInfo");
				strcpy(nodeName[1], "ModeSwitch");
				if(!strcmp(tmp , "Bridge"))
				{
					setAttrValue(top, nodeName, "CurMode", "Bridge");
				}
				else if(!strcmp(tmp , "Route"))
				{
					setAttrValue(top, nodeName, "CurMode", "Route");
				}
			}
		}
	}
	else if(!strcmp(attr, "ShowMode"))
	{
		memset(nodeName,0,sizeof(nodeName));
		strcpy(nodeName[0], WAN);
		strcpy(nodeName[1], "Common");
		memset(tmp, 0, sizeof(tmp));
		retval = getAttrValue(top, nodeName, "ShowEnable", tmp);
		if(retval == TCAPI_PROCESS_OK)
		{
			
			memset(nodeName,0,sizeof(nodeName));
			strcpy(nodeName[0], "WanInfo");
			strcpy(nodeName[1], "ModeSwitch");
			if(!strcmp(tmp , "Yes"))
			{
				setAttrValue(top, nodeName, "ShowMode", "Enable");
			}
			else
			{
				setAttrValue(top, nodeName, "ShowMode", "Disable");
			}
		}
		else
		{
			memset(nodeName,0,sizeof(nodeName));
			strcpy(nodeName[0], "WanInfo");
			strcpy(nodeName[1], "ModeSwitch");
			setAttrValue(top, nodeName, "ShowMode", "Disable");
		}
	}
	
	return SUCCESS;
}

/*_____________________________________________________________________________
**      function name: ModeSwitch_execute
**      descriptions:
**            To execute ModeSwitch node service.
**
**      parameters:
**            top: 		Specify the root of mxml_node_t structure.
**		  name: 		attribute name
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
**      1. haixiong.zhang 2014/12/28
**____________________________________________________________________________
*/
int ModeSwitch_execute(mxml_node_t *top, char name[][MAX_NODE_NAME])
{
	int retval = SUCCESS;
	char wanmode[32]={0};
	char isp[32]={0};
	char encapMode[32]={0};
	char tmp[129]={0};
	char usrname[129]={0};
	char pwd[129]={0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	unsigned char found = 0;
	unsigned char redo = 1;
	int backup = 0;
	int i = 0;
	int pvc_index = 0;
	int entry_index = 0;
	int valid_if[MAX_WAN_IF_INDEX] = {0};
	int valid_if_num = 0;
	tcapi_msg_t msg;

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], "WanInfo");
	strcpy(nodeName[1], "ModeSwitch");
	memset(tmp, 0, sizeof(tmp));
	retval = getAttrValue(top, nodeName, "wanmode", tmp);
	if(retval == TCAPI_PROCESS_OK)
	{
		memset(wanmode, 0 , sizeof(wanmode));
		strcpy(wanmode, tmp);
		if(!strcmp(wanmode , "Route"))
		{
			memset(tmp, 0, sizeof(tmp));
			retval = getAttrValue(top, nodeName, "username", tmp);
			if(retval == TCAPI_PROCESS_OK)
			{
				memset(usrname, 0 , sizeof(usrname));
				strcpy(usrname, tmp);
			}
			else
			{
				memset(nodeName,0,sizeof(nodeName));
				strcpy(nodeName[0], "WanInfo");
				strcpy(nodeName[1], "ModeSwitch");
				setAttrValue(top, nodeName, "Result", "Fail");
				return FAIL;
			}
			memset(tmp, 0, sizeof(tmp));
			retval = getAttrValue(top, nodeName, "password", tmp);
			if(retval == TCAPI_PROCESS_OK)
			{
				memset(pwd, 0 , sizeof(pwd));
				strcpy(pwd, tmp);
			}
			else
			{
				memset(nodeName,0,sizeof(nodeName));
				strcpy(nodeName[0], "WanInfo");
				strcpy(nodeName[1], "ModeSwitch");
				setAttrValue(top, nodeName, "Result", "Fail");
				return FAIL;
			}
		}
	}
	else
	{
		memset(nodeName,0,sizeof(nodeName));
		strcpy(nodeName[0], "WanInfo");
		strcpy(nodeName[1], "ModeSwitch");
		setAttrValue(top, nodeName, "Result", "Fail");
		return FAIL;
	}
	
	valid_if_num = get_all_wan_index(valid_if, MAX_WAN_IF_INDEX);
	for(i = 0; i < valid_if_num; i++)
	{
		pvc_index = valid_if[i] / MAX_SMUX_NUM;
		entry_index = valid_if[i] % MAX_SMUX_NUM;
		memset(nodeName,0,sizeof(nodeName));
		strcpy(nodeName[0], WAN);
		sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
		sprintf(nodeName[2], "%s%d", WAN_INTERFACE, entry_index);
		memset(tmp, 0, sizeof(tmp));
		retval = getAttrValue(top, nodeName, "ServiceList", tmp);
		if(retval == TCAPI_PROCESS_OK)
		{
			ToUpperCase(tmp);
			if(strstr(tmp , "INTERNET"))
			{
				found = 1;
				break;
			}
		}
	}
	if(found)
	{
		found = 0;
		if(!strcmp(wanmode , "Route"))
		{
			/* bridge -> route */
			if(backup)
			{}
			else
			{
				/* new bridge -> route, set default value */
				setAttrValue(top, nodeName, "ENCAP",           "PPPoE LLC");
				setAttrValue(top, nodeName, "Action",          "Modify");
				setAttrValue(top, nodeName, "WanMode",         "Route");
				setAttrValue(top, nodeName, "ISP",             "2");
				setAttrValue(top, nodeName, "NATENABLE",       "Enable");
				setAttrValue(top, nodeName, "PDEnable",        "Yes");
				setAttrValue(top, nodeName, "DHCPv6PD",        "Yes");
				setAttrValue(top, nodeName, "PDOrigin",        "PrefixDelegation");
				setAttrValue(top, nodeName, "MTU",             "1492");
				setAttrValue(top, nodeName, "PPPGETIP",        "Dynamic");
				setAttrValue(top, nodeName, "CONNECTION",      "Connect_on_Demand");
				setAttrValue(top, nodeName, "CLOSEIFIDLE",     "600");
				setAttrValue(top, nodeName, "USERNAME", 	   usrname);
				setAttrValue(top, nodeName, "PASSWORD",        pwd);
				setAttrValue(top, nodeName, "PPPManualStatus", "disconnect");
				setAttrValue(top, nodeName, "DsliteMode",      "0");
				setAttrValue(top, nodeName, "PPPUNIT",         "0");
				setAttrValue(top, nodeName, "IPVERSION", 	"IPv4");
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%s_%s_%s", nodeName[0],nodeName[1], nodeName[2]);
				/* commit */
				memset(&msg,0,sizeof(msg)) ;
				strcpy(msg.node, tmp);
				tcapi_commit_req(top, &msg);
				/* save */
				memset(&msg,0,sizeof(msg)) ;
				tcapi_save_req(top, &msg);
				found = 1;
			}
		}	
		if(!strcmp(wanmode , "Bridge"))
		{
			/* route -> bridge */
			if(backup)
			{}
			else
			{
				/* new route -> bridge ,set default value */
				setAttrValue(top, nodeName, "ENCAP",      "1483 Bridged IP LLC");
				setAttrValue(top, nodeName, "Action",     "Modify");
				setAttrValue(top, nodeName, "WanMode",    "Bridge");
				setAttrValue(top, nodeName, "ISP",        "3");
				setAttrValue(top, nodeName, "NATENABLE",  "Disabled");
				setAttrValue(top, nodeName, "PDEnable",   "No");
				setAttrValue(top, nodeName, "DHCPv6PD",   "No");
				setAttrValue(top, nodeName, "PDOrigin",   "None");
				setAttrValue(top, nodeName, "BridgeMode", "PPPoE_Bridged");
				setAttrValue(top, nodeName, "DHCPRealy",  "No");
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%s_%s_%s", nodeName[0],nodeName[1], nodeName[2]);
				/* commit */
				memset(&msg,0,sizeof(msg)) ;
				strcpy(msg.node, tmp);
				tcapi_commit_req(top, &msg);
				/* save */
				memset(&msg,0,sizeof(msg)) ;
				tcapi_save_req(top, &msg);
				found = 1;
			}
		}
	}

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], "WanInfo");
	strcpy(nodeName[1], "ModeSwitch");
	if(found)
	{
		setAttrValue(top, nodeName, "Result", "Succ");
		return SUCCESS;
	}
	else
	{
		
		setAttrValue(top, nodeName, "Result", "Fail");
		return FAIL;
	}
}
#endif


/*_____________________________________________________________________________
**      function name: set_wan_err
**      descriptions:
**            Set WanInfo commit error code.
**            0:   	Success
**            1:   	Modify status:entry is full
**            2:   	Modify status:pvc is full
**            3:   	Add status:entry is full
**            4:   	Add status:pvc is full
**            99:   	Delete status:can not find matched VPI/VCI
**
**      parameters:
**            err:   error code.
**
**      global:
**             None
**
**      return:
**            Success:        0
**
**      call:
**
**      revision:
**      1. xqxu 2010/9/1
**____________________________________________________________________________
*/
int
set_wan_err(int err){
	mxml_node_t *top = get_node_top();
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char str_err[4] = {0};
	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],WAN_IF_NODE);
	strcpy(nodeName[1], WAN_IF_COMMON);
	sprintf(str_err, "%d", err);
	setAttrValue(top, nodeName, "ErrCode", str_err);

	return SUCCESS;
}/* end set_wan_err */


#if defined(TCSUPPORT_CT_WANNINDEX_INCREASE)
void setLastUsedPvcIndex(int pvc){
	char lastUsedPvcIndex[8] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];	
	mxml_node_t *top = get_node_top();
	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],WAN);
	strcpy(nodeName[1],COMMON_NODENAME);	

	sprintf(lastUsedPvcIndex,"%d",pvc);
	setAttrValue(top, nodeName, "lastUsedPVCIndex", lastUsedPvcIndex);


	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],WAN);
	sprintf(nodeName[1], "%s%d", WAN_PVC, pvc);
	setAttrValue(top, nodeName, "lastUsedEntryIndex", "0"); 
	
	return;
}

void setLastUsedEntryIndex(int pvc, int entry){
	char lastUsedEntryIndex[8] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];	
	mxml_node_t *top = get_node_top();
	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],WAN);
	sprintf(nodeName[1], "%s%d", WAN_PVC, pvc);
	sprintf(lastUsedEntryIndex,"%d",entry);
	setAttrValue(top, nodeName, "lastUsedEntryIndex", lastUsedEntryIndex); 
	return;
}

#endif	


/*_____________________________________________________________________________
**      function name: get_pvc_totalNumber
**      descriptions:
**            Get the number of exist PVC.
**
**      parameters:
**            indexbuf:   the exist PVC index.
**
**      global:
**             None
**
**      return:
**            The number of exist PVC.
**
**      call:
**
**      revision:
**      1. xqxu 2010/9/1
**____________________________________________________________________________
*/
int
get_pvc_totalNumber(int indexbuf[]){
	int pvc_index;
	int num = 0;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	mxml_node_t *parentNode = NULL;
	mxml_node_t *curNode = NULL;
	mxml_node_t *top = get_node_top();
	int start = 0;
	int end = 0;

#if defined(TCSUPPORT_CT_WANNINDEX_INCREASE)
	char lastUsedPvcIndex[8] = {0};
	int iLastUsedPvcIndex = 0;
	int isFromFirst = 1;
#endif	

	if(!indexbuf)
		return 0;		
	parentNode = mxmlFindElement(top, top, WAN,
				NULL, NULL, MXML_DESCEND);
	if(parentNode == NULL)
		return 0;				


#if defined(TCSUPPORT_CT_WANNINDEX_INCREASE)
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],WAN);
	strcpy(nodeName[1],COMMON_NODENAME);	

	if(getAttrValue(top, nodeName, "lastUsedPVCIndex", lastUsedPvcIndex) != TCAPI_PROCESS_OK ){
		iLastUsedPvcIndex = 0;
	}else{
		iLastUsedPvcIndex =atoi(lastUsedPvcIndex);
	}		

	start = iLastUsedPvcIndex + 1;
	end = PVC_NUM;
	
	findPVC:		
#else	
	start= 0;
	end = PVC_NUM;
#endif	

	tcdbg_printf("[%s]start=%d\n",__FUNCTION__,start);
	for(pvc_index = start; pvc_index < end; pvc_index++){
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0],WAN);
		sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
		curNode = mxmlFindElement(parentNode, parentNode, nodeName[1],
			NULL, NULL, MXML_DESCEND);
		if(curNode){
			num++;
		}
		else{
			*indexbuf = pvc_index;
			indexbuf++;
		}	
	}

#if defined(TCSUPPORT_CT_WANNINDEX_INCREASE)
	if(isFromFirst){
		start = 0;
		end = iLastUsedPvcIndex + 1;
		isFromFirst = 0;
		goto findPVC;
	}
#endif		

	
	return num;
}/* end get_pvc_totalNumber */

/*_____________________________________________________________________________
**      function name: get_entry_numberByPvc
**      descriptions:
**            Get the number of exist entry by given PVC index.
**
**      parameters:
**            pvc_index:   PVC index.
**
**      global:
**             None
**
**      return:
**            The number of exist entry.
**
**      call:
**
**      revision:
**      1. xqxu 2010/9/1
**____________________________________________________________________________
*/
int
get_entry_numberByPvc(int pvc_index, int idle_entry[]){
	int num = 0;
	int entry_index;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	mxml_node_t *curNode = NULL;
	mxml_node_t *top = get_node_top();
	int start = 0;
	int end = 0;


#if defined(TCSUPPORT_CT_WANNINDEX_INCREASE)
	char lastUsedEntryIndex[8] = {0};
	int iLastUsedEntryIndex = 0;
	int isFromFirst = 1;

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],WAN);
	sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);

	if(getAttrValue(top, nodeName, "lastUsedEntryIndex", lastUsedEntryIndex) != TCAPI_PROCESS_OK ){
		iLastUsedEntryIndex = 0;
	}else{
		iLastUsedEntryIndex =atoi(lastUsedEntryIndex);
	}		

	start = iLastUsedEntryIndex + 1;
	end = MAX_SMUX_NUM;
	
	findEntry:		
#else	
	start= 0;
	end = MAX_SMUX_NUM;
#endif	

	for(entry_index = start; entry_index < end; entry_index++){
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0],WAN);
		sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
		sprintf(nodeName[2], "%s%d", WAN_INTERFACE, entry_index);
#if 0
		curNode = mxmlFindElement(top, top, nodeName[1],
			NULL, NULL, MXML_DESCEND);
#else
		/* find wan node */
		curNode = mxmlFindElement(top, top, nodeName[0],
			NULL, NULL, MXML_DESCEND_FIRST);
		/* find pvc node */
		if (curNode == NULL) {
			tcdbg_printf("\r\nget_entry_numberByPvc(): no this node, pvc_index = %d, entry_index = %d", pvc_index, entry_index);
			continue;
		}
		curNode = mxmlFindElement(curNode, curNode, nodeName[1],
			NULL, NULL, MXML_DESCEND);
#endif
		if(curNode){
			curNode = mxmlFindElement(curNode, curNode, nodeName[2],
			NULL, NULL, MXML_DESCEND);
			if(curNode){
				num++;
			}
			else{
				*idle_entry++ = entry_index;
			}
		}			
	}

#if defined(TCSUPPORT_CT_WANNINDEX_INCREASE)
	if(isFromFirst){
		start = 0;
		end = iLastUsedEntryIndex + 1;
		isFromFirst = 0;
		goto findEntry;
	}
#endif		
	

	return num;
}/* end get_entry_numberByPvc */

/*_____________________________________________________________________________
**      function name: copy_wan_info
**      descriptions:
**            Copy wan info from WanInfo node to Wan node.
**
**      parameters:
**            dst_pvcindex:   PVC index
**            dst_entryindex:   entry index
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
**      1. xqxu 2010/9/1
**____________________________________________________________________________
*/
int
copy_wan_info(int dst_pvcindex, int dst_entryindex)
{
	mxml_node_t *top = get_node_top();
	char dst_nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char src_nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	mxml_node_t *parentNode = NULL;
		
	memset(src_nodeName, 0, sizeof(src_nodeName));
	strcpy(src_nodeName[0],WAN_IF_NODE);
	strcpy(src_nodeName[1], WAN_IF_CUR_PVC);	
	memset(dst_nodeName, 0, sizeof(dst_nodeName));
	strcpy(dst_nodeName[0],WAN);	
	sprintf(dst_nodeName[1], "%s%d", WAN_PVC, dst_pvcindex);
		
	parentNode = mxmlFindElement(top, top, WAN,
				NULL, NULL, MXML_DESCEND);
	if(parentNode == NULL)
		return FAIL;	
	if(search_node(dst_nodeName) == NULL)
		mxmlNewElement(parentNode,  dst_nodeName[1]);

	copy_node(dst_nodeName, src_nodeName);

	parentNode = mxmlFindElement(parentNode, parentNode, dst_nodeName[1],
				NULL, NULL, MXML_DESCEND);
	if(parentNode == NULL)
		return FAIL;	
	sprintf(dst_nodeName[2], "%s%d", WAN_INTERFACE, dst_entryindex);
	if(search_node(dst_nodeName) == NULL)
		mxmlNewElement(parentNode,  dst_nodeName[2]);
		
	memset(src_nodeName, 0, sizeof(src_nodeName));
	strcpy(src_nodeName[0],WAN_IF_NODE);
	strcpy(src_nodeName[1], WAN_IF_CUR_IF);
	copy_node(dst_nodeName, src_nodeName);
	
	return SUCCESS;
}/* end copy_wan_info */

#if/*TCSUPPORT_COMPILR*/ defined(TCSUPPORT_CT_PPPINFORM)
int check_modifyinterface_wanmode(int cur_if){
	int pvc_index, entry_index;
	mxml_node_t *top = get_node_top();
	char waninfo_nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char wan_nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char waninfo_wanmode[10] = {0}, wan_wanmode[10] = {0};

	pvc_index = cur_if / MAX_SMUX_NUM;
	entry_index = cur_if % MAX_SMUX_NUM;

	memset(waninfo_nodeName, 0, sizeof(waninfo_nodeName));
	strcpy(waninfo_nodeName[0],WAN_IF_NODE);
	strcpy(waninfo_nodeName[1], WAN_IF_CUR_IF);	
	memset(wan_nodeName, 0, sizeof(wan_nodeName));
	strcpy(wan_nodeName[0],WAN);	
	sprintf(wan_nodeName[1], "%s%d", WAN_PVC, pvc_index);	
	sprintf(wan_nodeName[2], "%s%d", WAN_INTERFACE, entry_index);

	if((getAttrValue(top, waninfo_nodeName, "WanMode", waninfo_wanmode) != TCAPI_PROCESS_OK )
		||(getAttrValue(top, wan_nodeName, "WanMode", wan_wanmode) != TCAPI_PROCESS_OK )){
		return FAIL;
	}
	
	if(strcmp(wan_wanmode, "Route") == 0 && strcmp(waninfo_wanmode, "Bridge") == 0)
		return 1;
	else
		return 0;	
}
#endif/*TCSUPPORT_COMPILR*/

#if defined(TCSUPPORT_CT)
#if defined(TCSUPPORT_CT_FULL_ROUTE)
#ifdef TCSUPPORT_IPV6_ADVANCE
/*
	return:
		0:  success.
		-1:	error
		1:	may be process next time.
*/
int ipv6_rule_exec(char *ip6Addr, int if_index)
{
	struct in6_addr in_s_v6addr = {0};
	struct in6_addr in_e_v6addr = {0};
	char full_route_cmd[128] = {0};
	char start_addr[80] = {0};
	char end_addr[80] = {0};	
	unsigned int mark = 0;

	mark = (if_index + 1) << 16;
	if ( NULL == strstr(ip6Addr, "-") ) // single ip
	{
		if ( strlen(ip6Addr) > 39)
			return -1;

		if ( 1 != inet_pton(AF_INET6, ip6Addr, &in_s_v6addr) )
			return 1;

		sprintf(full_route_cmd, "ip6tables -A full_route_%d -t mangle -d %s -j MARK --set-mark 0x%x/0x7f0000\n", if_index, ip6Addr, mark);
		system(full_route_cmd);		
	}
	else // ip range
	{
		sscanf(ip6Addr, "%[^-]-%s", start_addr, end_addr);
		if ( 1 != inet_pton(AF_INET6, start_addr, &in_s_v6addr))
			return 1;

		if ( 1 != inet_pton(AF_INET6, end_addr, &in_e_v6addr))
			return 1;
		if ( memcmp(&in_s_v6addr, &in_e_v6addr, sizeof(struct in6_addr)) > 0 )
			return -1;
		
		sprintf(full_route_cmd, "ip6tables -A full_route_%d -t mangle -m iprange --dst-range %s -j MARK --set-mark 0x%x/0x7f0000\n", if_index, ip6Addr, mark);
		system(full_route_cmd);

	}
	return 0;
}
#endif
/*
	return:
		0:  success.
		-1:	error
		1:	may be process next time.
*/
int ipv4_rule_exec(char *ip4Addr, int if_index)
{
	struct in_addr in_s_v4addr = {0};
	struct in_addr in_e_v4addr = {0};
	char full_route_cmd[128] = {0};
	char start_addr[80] = {0};
	char end_addr[80] = {0};
	unsigned int mark = 0;

	mark = (if_index + 1) << 16;
	if (NULL == strstr(ip4Addr, "-")){
		if ( strlen(ip4Addr) > 15 )
			return -1;
		
		if ( 1 != inet_pton(AF_INET, ip4Addr, &in_s_v4addr) )
			return 1;

		sprintf(full_route_cmd, "iptables -A full_route_%d -t mangle -d %s -j MARK --set-mark 0x%x/0x7f0000\n", if_index, ip4Addr, mark);
		system(full_route_cmd);
	}else{
		sscanf(ip4Addr, "%[^-]-%s", start_addr, end_addr);
		if ( 1 != inet_pton(AF_INET, start_addr, &in_s_v4addr) )
			return 1;
		if ( 1 != inet_pton(AF_INET, end_addr, &in_e_v4addr) )
			return 1;
		if ( in_s_v4addr.s_addr > in_e_v4addr.s_addr )
			return -1;

		sprintf(full_route_cmd, "iptables -A full_route_%d -t mangle -m iprange --dst-range %s -j MARK --set-mark 0x%x/0x7f0000\n",if_index, ip4Addr, mark);
		system(full_route_cmd);
	}	
	return 0;
}

int full_route_table[MAX_WAN_IF_INDEX] = {0};// if  wan interface i's table is created, set full_route_table[i] to 1.
int onOff[2] = {0};
inline int get_table_status(int if_index){
	return full_route_table[if_index];
}
inline void set_table_status(int if_index, int value){
	full_route_table[if_index] = value;	
}

/*
	return:
		returnPtr:point the rest of  "ipStr",
		because ipStr may be "....,192.168", then returnPtr will be "192.168"
		
	ipv4Str format: x.x.x.x,x.x.x.a-x.x.x.b
	ipv6Str format: x:x:x:x,x:x:x:a-x:x:x:b
*/
char *set_forward_rule(char *ipStr, int if_index, char flag)
{
	char *tempStr = NULL;
	char *tempP = ipStr;
	char *returnPtr = NULL;
	int ret = 0;

	while(tempStr = strsep(&tempP, ","))
	{
		returnPtr = tempStr;
		if(tempP ==NULL){//the last part
			if(flag != ',')// the rest of ipStr is not NULL, so return for next time to process.
				break;
			returnPtr = NULL;//flag==',' mean the rest of ipStr is NULL
		}
		if(NULL != strstr(tempStr, "."))
			ret = ipv4_rule_exec(tempStr, if_index);
		else if(NULL != strstr(tempStr, ":")){
			#ifdef TCSUPPORT_IPV6_ADVANCE
			ret = ipv6_rule_exec(tempStr, if_index);
			#endif
		}
		if(ret == 0)
			returnPtr = NULL;
	}
	
	return returnPtr;
}

void close_table_by_ifIndex(int index)
{
	char cmd_buf[128] = {0};

	if(index<0 || index > MAX_WAN_IF_INDEX-1)
		return;
	if(1 == get_table_status(index)){
		sprintf(cmd_buf, "iptables -t mangle -D PREROUTING -j  full_route_%d\n", index);
		system(cmd_buf);
		sprintf(cmd_buf, "iptables -t mangle -F full_route_%d\n", index);
		system(cmd_buf);
		sprintf(cmd_buf, "iptables -t mangle -X full_route_%d\n", index);
		system(cmd_buf);
#ifdef	TCSUPPORT_IPV6_ADVANCE
		sprintf(cmd_buf, "ip6tables -t mangle -D PREROUTING -j	full_route_%d\n", index);
		system(cmd_buf);
		sprintf(cmd_buf, "ip6tables -t mangle -F full_route_%d\n", index);
		system(cmd_buf);
		sprintf(cmd_buf, "ip6tables -t mangle -X full_route_%d\n", index);
		system(cmd_buf);
#endif
		set_table_status(index, 0);
	}

}

void full_route_on(void)
{
	char vername[15] = {0};
	char type[5] = {0};
	char cmd_buf[128] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	mxml_node_t *top = get_node_top();

	/*if lan-side Dhcpv4 server is not running,  run it*/
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],LAN);
	strcpy(nodeName[1], LAN_DHCP);
	if(getAttrValue(top, nodeName, "type", type) == TCAPI_PROCESS_OK
		&& (0==strncmp(type, "0", 1)))
	{
		setAttrValue(top, nodeName, "type", "1");
		tcapi_commit_internal(top, "Dhcpd_Common");
	}

	if(onOff[0] == 1)
		return;
#ifdef TCSUPPORT_IPV6_ADVANCE
	decideModulePath(vername);
	sprintf(cmd_buf, "insmod /lib/modules/%s/kernel/net/ipv6/netfilter/ip6_tables.ko", vername);
	system(cmd_buf);
	sprintf(cmd_buf, "insmod /lib/modules/%s/kernel/net/ipv6/netfilter/ip6table_filter.ko", vername);
	system(cmd_buf);
	sprintf(cmd_buf, "insmod /lib/modules/%s/kernel/net/ipv6/netfilter/ip6table_mangle.ko", vername);
	system(cmd_buf);
#endif
	onOff[0] = 1;
	onOff[1] = 0;
}

void remove_all_tables(void)
{
	mxml_node_t *top = get_node_top();
	int i = 0;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char fr_enable[8] = {0};

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],WAN);
	strcpy(nodeName[1], WAN_COMMON);
	setAttrValue(top, nodeName, "IPForwardModeEnable","No");
#if defined(TCSUPPORT_CT_VLAN_BIND)	
	/*reset vlan bind entry & open portbind function*/
	tcapi_commit_internal(top, "VlanBind");
#endif
#ifdef TCSUPPORT_PORTBIND
	system("portbindcmd enable");
#endif
	
	for(i=0; i<MAX_WAN_IF_INDEX; i++){
		close_table_by_ifIndex(i);
	}
}

void full_route_off(void)
{
	char vername[15] = {0};
	char cmd_buf[128] = {0};
	int i = 0;

	if(onOff[1] == 1)
		return;
#ifdef	TCSUPPORT_IPV6_ADVANCE
	decideModulePath(vername);
	sprintf(cmd_buf, "rmmod /lib/modules/%s/kernel/net/ipv6/netfilter/ip6_tables.ko", vername);
	system(cmd_buf);
	sprintf(cmd_buf, "rmmod /lib/modules/%s/kernel/net/ipv6/netfilter/ip6table_filter.ko", vername);
	system(cmd_buf);
	sprintf(cmd_buf, "rmmod /lib/modules/%s/kernel/net/ipv6/netfilter/ip6table_mangle.ko", vername);
	system(cmd_buf);
#endif
	remove_all_tables();
	onOff[0] = 0;
	onOff[1] = 1;
}

/*
**	function name: is_full_route_enable
**	descriptions:
**		 To check full route function is open or not.

**	return,
**		1: full route function is open.
**		0:full route function is closed.
*/
int is_full_route_enable(void)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char fr_enable[8] = {0};
	char cmd_buf[128] = {0};
	mxml_node_t *top = get_node_top();
	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],WAN);
	strcpy(nodeName[1], WAN_COMMON);

	if(getAttrValue(top, nodeName, "IPForwardModeEnable", fr_enable) != TCAPI_PROCESS_OK)
		return 0;
	
	if(0 == strncmp(fr_enable, "Yes", 3))
	{
		full_route_on();
		return 1;
	}
	full_route_off();
	return 0;
}

/*_____________________________________________________________________________
**      function name: full_route_execute
**      descriptions:
**			 to do full route function.
**
**      parameters:
** 			pvc_index:	pvc num.
**			entry_index:	entry num.
**      global:
**             None
**
**      return:
**             	0:    not open this function, do others.
**			1:	set full route bind rule.
**      call:
**
**      revision:
**      1. Sofree.meng 20130828
**____________________________________________________________________________
*/
int full_route_execute(int pvc_index, int entry_index, char *strWanMode)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char ipforwardVal[MAXLEN_TCAPI_MSG+80]={0};
	char ipforwardTemp[MAXLEN_TCAPI_MSG+80]={0};
	char ipforwardStr[20]={0};
	char nas_name[10] = {0};
	char type[5] = {0};
	char cmd_buf[128] = {0};
	char value_buf[80] = {0};
	char *part_str = NULL;
	int i = 0;
	int len = 0;
	int total_attr = 0;
	int if_index = 0;
	mxml_node_t *top = get_node_top();

	/*check enable or not first*/
	if(0 == is_full_route_enable())
		return 0;
	if(0 == strcmp(strWanMode, WAN_BRIDGE_MODE)){
		remove_all_tables();
		return 1;
	}

#if defined(TCSUPPORT_CT_VLAN_BIND)
	/*clean all vlan bind's mark & close vlan bind and port bind*/
	for(i=0; i<MAX_LAN_PORT_NUM; i++) 
	{
		sprintf(cmd_buf,"%s -D BROUTING -j vbind_entry%d_chain\n", EBTABLES_BROUTE, i);
		system(cmd_buf);
		sprintf(cmd_buf,"%s -X vbind_entry%d_chain\n", EBTABLES_BROUTE, i);
		system(cmd_buf);
	}
	sprintf(cmd_buf, "echo 0 > /proc/tc3162/vbind_active");
	system(cmd_buf);
#endif
#ifdef TCSUPPORT_PORTBIND
	system("portbindcmd disable");
#endif

	/*full route function begin.....*/
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], WAN);
	sprintf(nodeName[1], "PVC%d", pvc_index);
	sprintf(nodeName[2], "Entry%d", entry_index);
	if(getAttrValue(top, nodeName, "NASName", nas_name) != TCAPI_PROCESS_OK 
		|| (0 == strlen(nas_name))){
			goto out;
	}
	if_index = get_wanindex_by_name(nas_name);

	/*create chain first if it is not exist..*/
	if(0 == get_table_status(if_index))
	{
		sprintf(cmd_buf, "iptables -t mangle -N full_route_%d\n", if_index);
		system(cmd_buf);
		sprintf(cmd_buf, "iptables -t mangle -I PREROUTING 1 -j full_route_%d\n", if_index);
		system(cmd_buf);
#ifdef	TCSUPPORT_IPV6_ADVANCE	
		sprintf(cmd_buf, "ip6tables -t mangle -N full_route_%d\n", if_index);
		system(cmd_buf);
		sprintf(cmd_buf, "ip6tables -t mangle -I PREROUTING 1 -j full_route_%d\n", if_index);
		system(cmd_buf);
#endif	 
		set_table_status(if_index, 1);
	}
	/*flush this nas's chain*/
	sprintf(cmd_buf, "iptables -t mangle -F full_route_%d\n", if_index);
	system(cmd_buf);
#ifdef	TCSUPPORT_IPV6_ADVANCE
	sprintf(cmd_buf, "ip6tables -t mangle -F full_route_%d\n", if_index);
	system(cmd_buf);
#endif

	/*add rule to the chain*/
	// max ipv6 addr pair is 80, total 1024, so we need 80*1024/MAXLEN_TCAPI_MSG +1 attr to restore this value.
	total_attr = 80*1024/MAXLEN_TCAPI_MSG + 1;
	if(getAttrValue(top, nodeName, "IPForwardList0", ipforwardVal) != TCAPI_PROCESS_OK 
		|| (0 == strlen(ipforwardVal))){
		goto out;
	}

	if(strlen(ipforwardVal) < MAXLEN_TCAPI_MSG - 1)
		goto normal;
	
	for(i=1; i<total_attr; i++){
		len = 0;
		sprintf(ipforwardStr, "IPForwardList%d", i);
		memset(ipforwardTemp, 0, sizeof(ipforwardTemp));
		if(getAttrValue(top, nodeName, ipforwardStr, ipforwardTemp) != TCAPI_PROCESS_OK 
			|| (0 == strlen(ipforwardTemp))){
				ipforwardTemp[0] = ',';
		}
		part_str = set_forward_rule(ipforwardVal, if_index, ipforwardTemp[0]);
		if((part_str!=NULL) && (0 != (len=strlen(part_str)))){
			strncpy(value_buf, part_str, len);
		}
		memset(ipforwardVal, 0, sizeof(ipforwardVal));
		if(len)
			strncpy(ipforwardVal, value_buf, len);
		strncpy(ipforwardVal+len, ipforwardTemp, strlen(ipforwardTemp));
		if(strlen(ipforwardTemp) < MAXLEN_TCAPI_MSG - 1)
			break;
	}	
normal:	
	set_forward_rule(ipforwardVal, if_index, ',');

out:	
	return 1;
}
#endif
#ifdef TCSUPPORT_PORTBIND
#if defined(TCSUPPORT_CT_DSLITE)
void set_portbinding_info_for_dslite(int pvc_index, int entry_index)
{
	int idx = 0;
	char bl_buf[12] = {0};
	char portbind_cmd[128] = {0};
	char str_ifname[16] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	mxml_node_t *top = get_node_top();

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], "Wan");
	sprintf(nodeName[1], "PVC%d", pvc_index);
	sprintf(nodeName[2], "Entry%d", entry_index);

	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "IFName", str_ifname) )
		return;

	for (idx = 0; idx < sizeof(bl_map_data) / sizeof(bl_map_data[0]); idx++)
	{
		if ( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, bl_map_data[idx].bindif, bl_buf) )
		{
			if ( 0 == strcmp(bl_buf, "Yes"))
			{
				// add default for ds-lite tunnel
				sprintf(portbind_cmd, "portbindcmd addroute v4 %s ds.%s ''", bl_map_data[idx].realif, str_ifname);
				wan_cmd_excute(portbind_cmd);
			}
		}
	}
}
#endif
void dealWithLinkLocalRoute(mxml_node_t *top, char* if_name, int if_index, int isDefaultRoute){
	//isDefaultRoute[0:not default route, 1:add new default route; 2: default route switch]	
	char wanNode[MAX_ARG_NUM][MAX_NODE_NAME];	
	char wanInfoNode[MAX_ARG_NUM][MAX_NODE_NAME];
	char gate_way[20];
	char net_mask[20];
	char sub_ip[20];
	char linkmode[32] = {0};
	char route_cmd[128] = {0};
	int pvc_index, entry_index;
	int isPPP = 0;
	struct in_addr gatewayIP;
	struct in_addr netmask;
	struct in_addr subIP;


	pvc_index = if_index / MAX_SMUX_NUM;
	entry_index = if_index - pvc_index * MAX_SMUX_NUM;
	
	memset(wanNode, 0, sizeof(wanNode));
	strcpy(wanNode[0], "Wan");
	sprintf(wanNode[1], "PVC%d", pvc_index);
	sprintf(wanNode[2], "Entry%d", entry_index);

	if (getAttrValue(top, wanNode, "LinkMode", linkmode) != TCAPI_PROCESS_OK) {
		return;
	}
	if(strcmp(linkmode,"linkPPP") == 0){
		isPPP = 1;
	}

	memset(wanInfoNode, 0, sizeof(wanInfoNode));
	strcpy(wanInfoNode[0], "WanInfo");
	sprintf(wanInfoNode[1], "Entry%d", if_index);
	
	if (getAttrValue(top, wanInfoNode, "GateWay", gate_way) != TCAPI_PROCESS_OK || strlen(gate_way) == 0) {
		return;
	}

	if (getAttrValue(top, wanInfoNode, "NetMask", net_mask) != TCAPI_PROCESS_OK || strlen(net_mask) == 0) {
		return;
	}

	if(isPPP){
		strcpy(sub_ip,gate_way);
		strcpy(net_mask,"255.255.255.255");
	}else{
		inet_aton(gate_way,&gatewayIP);
		inet_aton(net_mask,&netmask);
		subIP.s_addr = (gatewayIP.s_addr & netmask.s_addr);
		strcpy(sub_ip,inet_ntoa(subIP));
	}


	//del link route if it is not default wan, and add link route when it is default route
	if(isDefaultRoute){
		if(isDefaultRoute == 2)
			sprintf(route_cmd, "/sbin/route add -net %s netmask %s dev %s", sub_ip, net_mask, if_name);
	}else{	
		sprintf(route_cmd, "/sbin/route del -net %s netmask %s dev %s", sub_ip, net_mask, if_name);
	}
	
	wan_cmd_excute(route_cmd);
	return;
}


/*_____________________________________________________________________________
**      function name: set_portbinding_info
**      descriptions:
**           set portbinding info.
**
**      parameters:
**            top: 			top cfg node
**			  if_name:		interfce name
**			  if_index:		interface index
*			  state:		interface state
**
**      global:
**             None
**
**      return:
**             void
**
**      call:
**
**      revision:
**      1. xyzhu_nj 20100827
**____________________________________________________________________________
*/
void set_portbinding_info(mxml_node_t *top, char *if_name, int if_index, int state)
{
	char wanNode[MAX_ARG_NUM][MAX_NODE_NAME];
	char wanInfoNode[MAX_ARG_NUM][MAX_NODE_NAME];
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char nas_name[10], ip_version[20], isp[10];
	char bind_ifName_list[64], bind_ifType_list[20], bl_buf[10];
	char gate_way[20], gate_way6[256], status[10], status6[10];
	char portbind_cmd[128];
	int ii, i, pvc_index, entry_index, ip_flag = -1; /* 0:IPv4, 1:IPv6, 2:IPv4/IPv6 */
	char lanIP6[64] = {0}, lanPrefix6[12] = {0};
	int iGetLan6Info = 0;

#if defined(TCSUPPORT_CT_PPPOEPROXY)
	int flag[8] = {0};
	char *output = NULL;
	char *pt = NULL;
	char proxyenable[4] = {0};
	int flag_enable[MAX_WAN_IF_INDEX] = {0};
#endif
#if defined(TCSUPPORT_CT_DSLITE)
	int dsliteEnable = 0;
#endif
	unsigned char add_linkroute[MAX_WAN_IF_INDEX] = {0};
	char def_route_index_v4[10] = {0};
	int i_def_route_index_v4 = -1;
	int ipv4up=0, ipv6up=0;

	/* delete ip rule first */
	for (i = 0; i < sizeof(bl_map_data) / sizeof(bl_map_data[0]); i++) {
		
#if defined(TCSUPPORT_CT_PPPOEPROXY) && !defined(TCSUPPORT_CT_PON_JS)
		sprintf(portbind_cmd, "ip route list table %s", bl_map_data[i].realif);
		cmdPipe(portbind_cmd, &output);
		pt = strstr(output, "blackhole");
		if(!pt){
			free(output);
			sprintf(portbind_cmd, "portbindcmd delroute all %s", bl_map_data[i].realif);
			wan_cmd_excute(portbind_cmd);
		}
		else{
			flag[i]=1;
			free(output);
			continue;
		}
#else
		sprintf(portbind_cmd, "portbindcmd delroute all %s", bl_map_data[i].realif);
		wan_cmd_excute(portbind_cmd);
#endif

	}

	/* get ipv6 lan info from node. */
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], LAN);
	strcpy(nodeName[1], LAN_ENTRY_0);
	if ( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, "IP6", lanIP6)
		&& TCAPI_PROCESS_OK ==
		getAttrValue(top, nodeName, "PREFIX6", lanPrefix6) )
		iGetLan6Info = 1;

	/* get default route for ipv4 */
	memset(def_route_index_v4, 0, sizeof(def_route_index_v4));
	memset(wanNode, 0, sizeof(wanNode));
	strcpy(wanNode[0], "WanInfo");
	strcpy(wanNode[1], "Common");
	if ( TCAPI_PROCESS_OK == getAttrValue(top, wanNode, "DefRouteIndexv4", def_route_index_v4)
		&& '\0' != def_route_index_v4[0] && 0 != strcmp(def_route_index_v4, "N/A") )
		i_def_route_index_v4 = atoi(def_route_index_v4);

	/* add ip rule then */
	for (ii = 0; ii < MAX_WAN_IF_INDEX; ii++) {
		pvc_index = ii / MAX_SMUX_NUM;
		entry_index = ii - pvc_index * MAX_SMUX_NUM;

		memset(wanNode, 0, sizeof(wanNode));
		strcpy(wanNode[0], "Wan");
		sprintf(wanNode[1], "PVC%d", pvc_index);
		sprintf(wanNode[2], "Entry%d", entry_index);

		//tcdbg_printf("bbbubybybybybybyybyb.ii = %d\n", ii);
		//tcdbg_printf("wanNode[1] is %s\n", wanNode[1]);
		//tcdbg_printf("wanNode[2] is %s\n", wanNode[2]);

#if defined(TCSUPPORT_CT_PPPOEPROXY)
		if (getAttrValue(top, wanNode, "ProxyEnable", proxyenable) != TCAPI_PROCESS_OK){
			flag_enable[ii] = 0;
		}
		else{
			if (atoi(proxyenable) == 1 )
				flag_enable[ii] = 1;
			else
				flag_enable[ii] = 0;
		}
#endif
#if defined(TCSUPPORT_CT_DSLITE)
		dsliteEnable = get_dslite_stateviacfg(top, wanNode);
#endif
		add_linkroute[ii] = 0;

		memset(isp, 0, sizeof(isp));
		/* get isp, add ip rule only for route mode */
		if (getAttrValue(top, wanNode, "ISP", isp) != TCAPI_PROCESS_OK) {
			continue;
		}
		else {

			if (isp[0] == '3') {
				/* bridge mode, just continue */
				continue;
			}
		}
		memset(nas_name, 0, sizeof(nas_name));
		/* get nas name from wan node */
		if (getAttrValue(top, wanNode, "IFName", nas_name) != TCAPI_PROCESS_OK) {
		#ifdef CT_COM_DBUG
			tcdbg_printf("%s: get nas name failed.\n", __FUNCTION__);
		#endif
		#if 0
			return;
		#else
			continue;
		#endif
		}

		memset(ip_version, 0, sizeof(ip_version));
		/* get service list from wan node */
		if (getAttrValue(top, wanNode, "IPVERSION", ip_version) != TCAPI_PROCESS_OK) {
		#ifdef CT_COM_DBUG
			tcdbg_printf("%s: get ip version failed.\n", __FUNCTION__);
		#endif
		#if 0
			return;
		#else
			continue;
		#endif
		}
		
		/* check ip version */
		if (!strcmp(ip_version, "IPv4")) {
			ip_flag = 0;
		}
		else if (!strcmp(ip_version, "IPv6")) {
			ip_flag = 1;
		}
		else if (!strcmp(ip_version, "IPv4/IPv6")) {
			ip_flag = 2;
		}
		else {
			/* may bridge mode */
			continue;
		}
		//tcdbg_printf("ip flag is %d\n", ip_flag);
	#if 0
		/* route mode, get gateway */
		if (BRIDGE_IF_STATE_DOWN != state && BRIDGE_IF_STATE_UP != state) {
			memset(wanInfoNode, 0, sizeof(wanInfoNode));
			strcpy(wanInfoNode[0], "WanInfo");
			sprintf(wanInfoNode[1], "Entry%d", ii);

			/* get ipv4 gateway */
			memset(gate_way, 0, sizeof(gate_way));
			if (getAttrValue(top, wanInfoNode, "GateWay", gate_way) != TCAPI_PROCESS_OK) {
			#ifdef CT_COM_DBUG
				tcdbg_printf("%s: get gateway failed.\n", __FUNCTION__);
			#endif
			}
			/* get ipv6 gateway */
			memset(gate_way6, 0, sizeof(gate_way6));
			if (getAttrValue(top, wanInfoNode, "GateWay6", gate_way6) != TCAPI_PROCESS_OK) {
			#ifdef CT_COM_DBUG
				tcdbg_printf("%s: get gateway6 failed.\n", __FUNCTION__);
			#endif

			}
		}
	#endif	
#if defined(TCSUPPORT_CT_DSLITE)
		if ( dsliteEnable )
			ip_flag = 1;
#endif

		/* get wan gate way */
		memset(wanInfoNode, 0, sizeof(wanInfoNode));
		strcpy(wanInfoNode[0], "WanInfo");
		sprintf(wanInfoNode[1], "Entry%d", ii);

		
		memset(gate_way, 0, sizeof(gate_way));
		memset(gate_way6, 0, sizeof(gate_way6));
		ipv4up = 0;
		ipv6up = 0;
		/* get ipv4 gateway */
		/* ipv4 */
		if (ip_flag == 0 || ip_flag == 2) {
			/* get ipv4 status */
			ipv4up = 1;
			if ( TCAPI_PROCESS_OK != getAttrValue(top, wanInfoNode, "Status", status)
				|| (0 != strcmp(status, "up")) )
			{
#if defined(TCSUPPORT_CT_PPP_ONDEMAND)
				set_portbinding_info_for_ondemand_wan(top, pvc_index, entry_index, nas_name);
#endif
					ipv4up = 0;
				}
			else{			
			if (getAttrValue(top, wanInfoNode, "GateWay", gate_way) != TCAPI_PROCESS_OK) {
			#ifdef CT_COM_DBUG
				tcdbg_printf("%s: get gateway failed.\n", __FUNCTION__);
			#endif
				ipv4up = 0;
			}
			else {
				if (gate_way[0] == '\0') {
					ipv4up = 0;
				}
				}
			}
			
		}
		/* ipv6 */
		if (ip_flag == 1 || ip_flag == 2) {
			/* get ipv6 status */
			ipv6up = 1;
			if ( TCAPI_PROCESS_OK != getAttrValue(top, wanInfoNode, "Status6", status6)
				|| (0 != strcmp(status6, "up")) )
			{
#if defined(TCSUPPORT_CT_PPP_ONDEMAND)
				set_portbinding_info_for_ondemand_wan(top, pvc_index, entry_index, nas_name);
#endif
					ipv6up = 0;
				}
			else{
			if (getAttrValue(top, wanInfoNode, "GateWay6", gate_way6) != TCAPI_PROCESS_OK) {
			#ifdef CT_COM_DBUG
				tcdbg_printf("%s: get gateway6 failed.\n", __FUNCTION__);
			#endif
				ipv6up = 0;
			}
			else {
				if (gate_way6[0] == '\0') {
					ipv6up = 0;
				}
				}
			}
		}
		
		if((ipv4up == 0) && (ipv6up == 0))
			continue;
		
		/* get bindlist for every wan interface */
		memset(bind_ifName_list, 0, sizeof(bind_ifName_list));
		memset(bind_ifType_list, 0, sizeof(bind_ifType_list));
		for (i = 0; i < sizeof(bl_map_data) / sizeof(bl_map_data[0]); i++) {
			if (getAttrValue(top, wanNode, bl_map_data[i].bindif, bl_buf) == TCAPI_PROCESS_OK) {
				if (!strcmp(bl_buf, "Yes")) {
#if defined(TCSUPPORT_CT_PPPOEPROXY) && !defined(TCSUPPORT_CT_PON_JS)
					if ((flag[i] == 1)&&(flag_enable[ii] == 1)){
						break;
					}
#endif
					/* add ip rule for every wan interface */
					if (('\0' != gate_way[0]) && ipv4up) {
						if ( i_def_route_index_v4 != ii
							&& 0 == add_linkroute[ii] )
						{
							add_linkroute[ii] = 1;
							dealWithLinkLocalRoute(top, nas_name, ii, 2);
						}
						/* for ipv4 */
						sprintf(portbind_cmd, "portbindcmd addroute v4 %s %s %s", bl_map_data[i].realif, nas_name, gate_way);
						wan_cmd_excute(portbind_cmd);
					}
					
					if (('\0' != gate_way6[0]) && ipv6up) {
						/* for ipv6 */
						sprintf(portbind_cmd, "portbindcmd addroute v6 %s %s %s", bl_map_data[i].realif, nas_name, gate_way6);
						wan_cmd_excute(portbind_cmd);
#if defined(TCSUPPORT_CT_DSLITE)
						if ( dsliteEnable )
						{
							// record default for ds-lite tunnel, can't do portbindcmd because dslite interface maybe isn't up.
							set_dslite_internal_bind_state(ii, 1);
						}
#endif
					}
					if ( iGetLan6Info )
					{
						sprintf(portbind_cmd, "/usr/bin/ip -6 route add %s/%s"
								" dev br0 table %s",
								lanIP6, lanPrefix6,
								bl_map_data[i].realif);
						wan_cmd_excute(portbind_cmd);
					}
					else
					{
						sprintf(portbind_cmd, "/usr/bin/ip -6 route add"
								" fe80::1/64 dev br0 table %s",
								bl_map_data[i].realif);
						wan_cmd_excute(portbind_cmd);
					}

				#if 0
					/* if it's route mode, set policy route, only for ipv4. */
					switch (state) {
						case IF_STATE_UP:
						case IF_STATE_UP6:
						#if 0
							/* delete policy route first */
							sprintf(portbind_cmd, "portbindcmd delroute all %s", bl_map_data[i].realif);
							wan_cmd_excute(portbind_cmd);
						#endif
							if ('\0' != gate_way[0]) {
								if ('\0' != gate_way6[0]) {
									/* for ipv4 and ipv6 */
									sprintf(portbind_cmd, "portbindcmd addroute all %s %s %s %s", bl_map_data[i].realif, if_name, gate_way, gate_way6);
									wan_cmd_excute(portbind_cmd);
								}
								else {
									/* only for ipv4 */
									sprintf(portbind_cmd, "portbindcmd addroute v4 %s %s %s", bl_map_data[i].realif, if_name, gate_way);
									wan_cmd_excute(portbind_cmd);
								}
							}
							else if ('\0' != gate_way6[0]) {
								/* only for ipv6 */
								sprintf(portbind_cmd, "portbindcmd addroute v6 %s %s %s", bl_map_data[i].realif, if_name, gate_way6);
								wan_cmd_excute(portbind_cmd);
							}
							break;
						case IF_STATE_DOWN:
							if (!strcmp(bl_buf, "Yes") && '\0' != gate_way6[0]) {
								/* add policy route for ipv6 again */
								sprintf(portbind_cmd, "portbindcmd addroute v6 %s %s %s", bl_map_data[i].realif, if_name, gate_way6);
								wan_cmd_excute(portbind_cmd);
							}
							break;
						case IF_STATE_DOWN6:
							if (!strcmp(bl_buf, "Yes") && '\0' != gate_way[0]) {
								/* add policy route for ipv4 again */
								sprintf(portbind_cmd, "portbindcmd addroute v4 %s %s %s", bl_map_data[i].realif, if_name, gate_way);
								wan_cmd_excute(portbind_cmd);
							}
						default:
							break;
					}
				#endif
				}
		#if 0
				/* delete policy route for ipv4 */
				if (IF_STATE_DOWN == state) {
				#if 0
					/* delete policy route first */
					sprintf(portbind_cmd, "portbindcmd delroute all %s", bl_map_data[i].realif);
					wan_cmd_excute(portbind_cmd);
				#endif
					if (!strcmp(bl_buf, "Yes") && '\0' != gate_way6[0]) {
						/* add policy route for ipv6 again */
						sprintf(portbind_cmd, "portbindcmd addroute v6 %s %s %s", bl_map_data[i].realif, if_name, gate_way6);
						wan_cmd_excute(portbind_cmd);
					}
				}

				/* delete policy route for ipv6 */
				if (IF_STATE_DOWN6 == state) {
				#if 0
					/* delete policy route first */
					sprintf(portbind_cmd, "portbindcmd delroute all %s", bl_map_data[i].realif);
					wan_cmd_excute(portbind_cmd);
				#endif
					if (!strcmp(bl_buf, "Yes") && '\0' != gate_way[0]) {
						/* add policy route for ipv4 again */
						sprintf(portbind_cmd, "portbindcmd addroute v4 %s %s %s", bl_map_data[i].realif, if_name, gate_way);
						wan_cmd_excute(portbind_cmd);
					}
				}
		#endif
			}
		}

		if ( 1 == add_linkroute[ii] )
		{
			add_linkroute[ii] = 0;
			dealWithLinkLocalRoute(top, nas_name, ii, 0);
		}
	}

	
	return;
}

/*_____________________________________________________________________________
**      function name: set_portbinding_info2
**      descriptions:
**           set portbinding info.
**
**      parameters:
**            top: 			top cfg node
**			  if_name:		interfce name
**			  if_index:		interface index
*			  state:		interface state
**
**      global:
**             None
**
**      return:
**             void
**
**      call:
**
**      revision:
**      1. xyzhu_nj 20100827
**____________________________________________________________________________
*/
void set_portbinding_info2(mxml_node_t *top, int pvc_index, int entry_index, int type)
{
	char wanNode[MAX_ARG_NUM][MAX_NODE_NAME];
	char nas_name[10], serviceApp[SERVICE_LIST_LEN];
	char bind_ifName_list[128], bind_ifType_list[64], bl_buf[10];
	char portbind_cmd[128];
	int i, bind_flag = 0;
	int if_index = pvc_index * MAX_PVC_NUM + entry_index;

//#ifdef TCSUPPORT_CT_ACCESSLIMIT
	char bind_lanif_list[128] = {0};
	int modeflag = 0;
	char isp[5] = {0};
	char str_active[12] = {0};
//#endif
	memset(wanNode, 0, sizeof(wanNode));
	strcpy(wanNode[0], "Wan");
	sprintf(wanNode[1], "PVC%d", pvc_index);
	sprintf(wanNode[2], "Entry%d", entry_index);

	/* get nas name from wan node */
	if (getAttrValue(top, wanNode, /*"IFName"*/"NASName", nas_name) != TCAPI_PROCESS_OK) {
	#ifdef CT_COM_DBUG
		tcdbg_printf("%s: get nas name failed.\n", __FUNCTION__);
	#endif
		return;
	}
	/* get service list from wan node */
	if (getAttrValue(top, wanNode, "ServiceList", serviceApp) != TCAPI_PROCESS_OK) {
	#ifdef CT_COM_DBUG
		tcdbg_printf("%s: get service list failed.\n", __FUNCTION__);
	#endif
		return;	
	}
	/* get isp from wan node */
	//#ifdef TCSUPPORT_CT_ACCESSLIMIT
	if(getAttrValue(top, wanNode, "ISP", isp) != TCAPI_PROCESS_OK ){
	//#ifdef CT_COM_DBUG
			tcdbg_printf("%s: get ISP failed\n", __FUNCTION__);
	//#endif
		return;
	}
	if(getAttrValue(top, wanNode, "Active", str_active) != TCAPI_PROCESS_OK ){
	//#ifdef CT_COM_DBUG
			tcdbg_printf("%s: get Active failed\n", __FUNCTION__);
	//#endif
		return;
	}

	if(!strcmp(isp,BRIDGE_MODE))
		modeflag = 1;		
	//#endif

	/* get bindlist */
	memset(bind_ifName_list, 0, sizeof(bind_ifName_list));
	memset(bind_ifType_list, 0, sizeof(bind_ifType_list));
	for (i = 0; i < sizeof(bl_map_data) / sizeof(bl_map_data[0]); i++) {
		if (getAttrValue(top, wanNode, bl_map_data[i].bindif, bl_buf) == TCAPI_PROCESS_OK) {
			if (!strcmp(bl_buf, "Yes")) {
				if ('\0' == bind_ifName_list[0]) {
					strcpy(bind_ifName_list, bl_map_data[i].realif);
					strcpy(bind_ifType_list, "O_O");
				}
				else {
					sprintf(bind_ifName_list, "%s,%s", bind_ifName_list, bl_map_data[i].realif);
					strcat(bind_ifType_list, ",O_O");
				}
			}
		}
	}

#if defined(TCSUPPORT_CT_ACCESSLIMIT)
	strcpy(bind_lanif_list,bind_ifName_list);
	if('\0' == bind_lanif_list[0])
		strcpy(bind_lanif_list,NOTBINDSTRING);
#endif
	/* combine bind list */
	if ('\0' != bind_ifName_list[0]) {
		sprintf(bind_ifName_list, "%s,%s", bind_ifName_list, nas_name);
		bind_flag = 1;
	}
	else {
		strcpy(bind_ifName_list, nas_name);
		bind_flag = 0;
	}

	/* 'I(INTERNET)' means INTERNET or TR069_INTERNET type, 'O(OTHER)' means other */
#if defined(TCSUPPORT_CT_SERVICELIST_E8C)
	if (strstr(serviceApp, INTERNET) != NULL){
#else
	if (!strcmp(serviceApp, INTERNET) || !strcmp(serviceApp, TR069_INTERNET)){
#endif
		if (1 == bind_flag) {
			if (modeflag == 1) {
				strcat(bind_ifType_list, ",I_B");
			}
			else {
				strcat(bind_ifType_list, ",I_R");
			}
		}
		else {
			if (modeflag == 1) {
				strcpy(bind_ifType_list, "I_B");
			}
			else {
				strcpy(bind_ifType_list, "I_R");
			}
		}
	}
	else {
		if (1 == bind_flag) {
			if (modeflag == 1) {
				strcat(bind_ifType_list, ",O_B");
			}
			else {
				strcat(bind_ifType_list, ",O_R");
			}
		}
		else {
			if (modeflag == 1) {
				strcpy(bind_ifType_list, "O_B");
			}
			else {
				strcpy(bind_ifType_list, "O_R");
			}
		}
	}

	switch (type) {
		case ADD_PORTBIND:
			if(0 != strcmp(str_active, "Yes"))
				break;
			/* excute portbinding cmd */
			sprintf(portbind_cmd, "portbindcmd addgroup %d %s %s", if_index, bind_ifName_list, bind_ifType_list);
			wan_cmd_excute(portbind_cmd);

			/* open portbind */
			strcpy(portbind_cmd, "portbindcmd enable");
			wan_cmd_excute(portbind_cmd);
#if defined(TCSUPPORT_CT_ACCESSLIMIT)
			//below is used for access limit
			memset(portbind_cmd,0,sizeof(portbind_cmd));
			sprintf(portbind_cmd, "/usr/bin/accesslimitcmd addbindinfo %d %d %s", if_index, modeflag, bind_lanif_list);
			wan_cmd_excute(portbind_cmd);
#endif
			break;
		case DEL_PORTBIND:
			/* excute portbinding cmd */
			sprintf(portbind_cmd, "portbindcmd delgroup %d", if_index);
			wan_cmd_excute(portbind_cmd);

#if defined(TCSUPPORT_CT_ACCESSLIMIT)
			//below is used for access limit
			memset(portbind_cmd,0,sizeof(portbind_cmd));
			sprintf(portbind_cmd, "/usr/bin/accesslimitcmd deletebindinfo %d", if_index);
			wan_cmd_excute(portbind_cmd);
#endif

			/* ok portbinding open always */
			break;
		default:
			break;
	}

	return;
}
#endif

//DHCPRealy xmdai_nj
/*_____________________________________________________________________________
**      function name: set_portbinding_info2
**      descriptions:
**           Used for Wan DHCP realy function,set ebtables rules for DHCP Realy
**
**      parameters:
**            top: 			top cfg node
**			  if_name:		interfce name
**			  if_index:		interface index
*			  state:		interface state
**
**      global:
**             None
**
**      return:
**             void
**
**      call:
**
**      revision:
**      1. xmdai_nj 20101012
**____________________________________________________________________________
*/
void set_DHCPRealy_info(mxml_node_t *top, int pvc_index, int entry_index, int iType)
{
	char wanNode[MAX_ARG_NUM][MAX_NODE_NAME];
	char ebtablesCmd[128],RealyInputTabe[64],RealyForwardTabe[64];
	char dhcpRealy[4],bl_buf[10], nas_name[10],ipversion[10];
        /* add by xyzhu to fix dhcp dial fail from lan when vlan bind enable */
	char vlan_bind_node[MAX_ARG_NUM][MAX_NODE_NAME];
	char bind_sw[10];
	char str_servicelist[32] = {0};
	char strAddDel;
	int i;
	int isInternetTrans = 0;
	char strWanMode[16] = {0};

	memset(wanNode, 0, sizeof(wanNode));
	strcpy(wanNode[0], "Wan");
	sprintf(wanNode[1], "PVC%d", pvc_index);
	sprintf(wanNode[2], "Entry%d", entry_index);

        /* add by xyzhu to fix dhcp dial fail from lan when vlan bind enable */
	memset(vlan_bind_node, 0, sizeof(vlan_bind_node));
	strcpy(vlan_bind_node[0], "VlanBind");

	/* get nas name from wan node */
	if (getAttrValue(top, wanNode, WAN_NASNAME, nas_name) != TCAPI_PROCESS_OK) {
		//tcdbg_printf("%s: get nas name failed.\n", __FUNCTION__);
		return;
	}	
	if (getAttrValue(top, wanNode, WAN_DHCPREALY, dhcpRealy) != TCAPI_PROCESS_OK) {
		tcdbg_printf("%s: get DHCPRealy failed.\n", __FUNCTION__);
		return;	
	}

	if( TCAPI_PROCESS_OK != getAttrValue(top, wanNode, "ServiceList", str_servicelist) )
		str_servicelist[0] = '\0';

	if ( (0 == strcmp(dhcpRealy, "Yes")) && (NULL != strstr(str_servicelist, "INTERNET")) )
		isInternetTrans = 1;
#if !defined(TCSUPPORT_CT_DHCP_PORT_FILTER)
	sprintf(RealyInputTabe,"RealyInput%s",nas_name);
#endif
	sprintf(RealyForwardTabe,"RealyForward%s",nas_name);


	memset(strWanMode, 0, sizeof(strWanMode));
	getAttrValue(top, wanNode, WAN_MODE, strWanMode);
	if(ADD_DHCPREALY_CMD == iType &&  0 == strcmp(strWanMode,WAN_BRIDGE_MODE)){
		//new input chain for DHCP realy
#if !defined(TCSUPPORT_CT_DHCP_PORT_FILTER)
		memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
		sprintf(ebtablesCmd,"ebtables -t filter -N %s 2>/dev/null",RealyInputTabe);
		system(ebtablesCmd);

		memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
		sprintf(ebtablesCmd,"ebtables -t filter -P %s RETURN",RealyInputTabe);
		system(ebtablesCmd);
		
		memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
		sprintf(ebtablesCmd,"ebtables -t filter -A INPUT -j %s",RealyInputTabe);
		system(ebtablesCmd);
#endif

		//new forward chain for DHCP realy
		memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
		sprintf(ebtablesCmd,"ebtables -t filter -N %s 2>/dev/null",RealyForwardTabe);
		system(ebtablesCmd);

		memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
		sprintf(ebtablesCmd,"ebtables -t filter -P %s RETURN",RealyForwardTabe);
		system(ebtablesCmd);

		memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
		sprintf(ebtablesCmd,"ebtables -t filter -A FORWARD -j %s",RealyForwardTabe);
		system(ebtablesCmd);

#ifdef TCSUPPORT_PORTBIND

		for (i = 0; i < sizeof(bl_map_data) / sizeof(bl_map_data[0]); i++) {
		/* add by xyzhu to fix dhcp dial fail from lan when vlan bind enable */
			sprintf(vlan_bind_node[1], "Entry%d", i);
			if (getAttrValue(top, vlan_bind_node, "Active", bind_sw) != TCAPI_PROCESS_OK) {
				strcpy(bind_sw, "No");
			}
			if (getAttrValue(top, wanNode, bl_map_data[i].bindif, bl_buf) == TCAPI_PROCESS_OK) {
				if (!strcmp(bl_buf, "Yes")) {
					if (!strcmp(dhcpRealy, "Yes")){   //DHCPRealy enable
#if !defined(TCSUPPORT_CT_DHCP_PORT_FILTER)
						memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
						sprintf(ebtablesCmd,"ebtables -t filter -A %s -i %s -p IPv4 --ip-proto 17 --ip-dport 67 -j DROP", RealyInputTabe,  bl_map_data[i].realif);
						system(ebtablesCmd);
#endif

						memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
						sprintf(ebtablesCmd,"ebtables -t filter -A %s -i %s -o %s -p IPv4 --ip-proto 17 --ip-dport 67 -j ACCEPT", RealyForwardTabe,  bl_map_data[i].realif, nas_name);						
						system(ebtablesCmd);

#if !defined(TCSUPPORT_CT_DHCP_PORT_FILTER)
						memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
						sprintf(ebtablesCmd,"ebtables -t filter -A %s -i %s -p IPv6 --ip6-proto 17 --ip6-dport 547 -j DROP", RealyInputTabe,  bl_map_data[i].realif);
						system(ebtablesCmd);
#endif

						memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
						sprintf(ebtablesCmd,"ebtables -t filter -A %s -i %s -o %s -p IPv6 --ip6-proto 17 --ip6-dport 547 -j ACCEPT", RealyForwardTabe,  bl_map_data[i].realif, nas_name);						
						system(ebtablesCmd);
					}
				}
			/* add by xyzhu to fix dhcp dial fail from lan when vlan bind enable */
				else { /* this lan is not bind but enable vlan bind, it should forward to wan side */
					if (!strcmp(dhcpRealy, "Yes") && !strcmp(bind_sw, "Yes")) { /* dhcp relay enable */
						/* for IPv4 */
						memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
						sprintf(ebtablesCmd,"ebtables -t filter -A %s -i %s -o %s -p IPv4 --ip-proto 17 --ip-dport 67 -j ACCEPT", RealyForwardTabe,  bl_map_data[i].realif, nas_name);						
						system(ebtablesCmd);

						/* for IPv6 */
						memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
						sprintf(ebtablesCmd,"ebtables -t filter -A %s -i %s -o %s -p IPv6 --ip6-proto 17 --ip6-dport 547 -j ACCEPT", RealyForwardTabe,  bl_map_data[i].realif, nas_name);						
						system(ebtablesCmd);
					}
				}
			}
		}
#endif

		if ( 0 == isInternetTrans )
		{
		memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
		sprintf(ebtablesCmd,"ebtables -t filter -A %s -o %s -p IPv4 --ip-proto 17 --ip-dport 67 -j DROP",RealyForwardTabe,nas_name);
		system(ebtablesCmd);

		memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
		sprintf(ebtablesCmd,"ebtables -t filter -A %s -o %s -p IPv6 --ip6-proto 17 --ip6-dport 547 -j DROP",RealyForwardTabe,nas_name);
		system(ebtablesCmd);				
	}
	}
	else if(DEL_DHCPREALY_CMD == iType){
#if !defined(TCSUPPORT_CT_DHCP_PORT_FILTER)
		memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
		sprintf(ebtablesCmd,"ebtables -t filter -F %s 2>/dev/null",RealyInputTabe);
		system(ebtablesCmd);
#endif

		memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
		sprintf(ebtablesCmd,"ebtables -t filter -F %s 2>/dev/null",RealyForwardTabe);
		system(ebtablesCmd);
	
#if !defined(TCSUPPORT_CT_DHCP_PORT_FILTER)
		memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
		sprintf(ebtablesCmd,"ebtables -t filter -D INPUT -j %s",RealyInputTabe);
		system(ebtablesCmd);
#endif

		memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
		sprintf(ebtablesCmd,"ebtables -t filter -D FORWARD -j %s",RealyForwardTabe);
		system(ebtablesCmd);

#if !defined(TCSUPPORT_CT_DHCP_PORT_FILTER)
		memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
		sprintf(ebtablesCmd,"ebtables -t filter -X %s 2>/dev/null",RealyInputTabe);
		system(ebtablesCmd);
#endif

		memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
		sprintf(ebtablesCmd,"ebtables -t filter -X %s 2>/dev/null",RealyForwardTabe);
		system(ebtablesCmd);
	}

	return;
}

#if defined(TCSUPPORT_CT_DHCP_PORT_FILTER)
void set_DHCP_PortFilter_info(mxml_node_t *top, int pvc_index, int entry_index, int iType)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char nodebindName[MAX_ARG_NUM][MAX_NODE_NAME] = {0}; 
	char ebtablesCmd[128] = {0};
	char PortFilterInputTable[64] = {0};
	char str_servicelist[32] = {0};
	char str_dhcpenable[12] = {0};
	char str_active[12] = {0};
	char nas_name[10] = {0}, bl_buf[10] = {0};
	char vidbind_sw[10] = "No";
	int i = 0;

	if ( !top )
		return;

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], WAN);
	sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
	sprintf(nodeName[2], "%s%d", WAN_INTERFACE, entry_index);

	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "ServiceList", str_servicelist) )
		str_servicelist[0] = '\0';
	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "DHCPEnable", str_dhcpenable) )
		str_dhcpenable[0] = '\0';
	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "Active", str_active) )
		str_active[0] = '\0';
	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, WAN_NASNAME, nas_name) )
		return;

	sprintf(PortFilterInputTable, "DhcpFilterInput%s", nas_name);
	if( ADD_DHCP_PORTFILTER_CMD == iType )
	{
		memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
		sprintf(ebtablesCmd,"ebtables -t filter -N %s 2>/dev/null", PortFilterInputTable);
		system(ebtablesCmd);

		memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
		sprintf(ebtablesCmd,"ebtables -t filter -P %s RETURN", PortFilterInputTable);
		system(ebtablesCmd);

		memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
		sprintf(ebtablesCmd,"ebtables -t filter -A INPUT -j %s", PortFilterInputTable);
		system(ebtablesCmd);

		if ( 0 == strcmp(str_dhcpenable, "0")
			&& (0 == strcmp(str_active, "Yes"))
			&& (NULL != strstr(str_servicelist, "INTERNET") || NULL != strstr(str_servicelist, "OTHER")) )
		{
#ifdef TCSUPPORT_PORTBIND
			for (i = 0; i < sizeof(bl_map_data) / sizeof(bl_map_data[0]); i++)
			{
#if defined(TCSUPPORT_CT_VLAN_BIND)
				memset(nodebindName, 0, sizeof(nodebindName));
				strcpy(nodebindName[0], VLAN_BIND);
				sprintf(nodebindName[1], "%s%d", VLAN_BIND_ENTRY, i);
				if( TCAPI_PROCESS_OK != getAttrValue(top, nodebindName, "Active", vidbind_sw) )
					strcpy(vidbind_sw, "No");
#endif

				if ( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, bl_map_data[i].bindif, bl_buf)
					&& 0 == strcmp(bl_buf, "Yes")
					&& 0 == strcmp(vidbind_sw, "No") )
				{
					memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
					sprintf(ebtablesCmd,"ebtables -t filter -A %s -i %s -p IPv4 --ip-proto 17 --ip-dport 67 -j DROP", PortFilterInputTable, bl_map_data[i].realif);
					system(ebtablesCmd);

					memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
					sprintf(ebtablesCmd,"ebtables -t filter -A %s -i %s -p IPv6 --ip6-proto 17 --ip6-dport 547 -j DROP", PortFilterInputTable, bl_map_data[i].realif);
					system(ebtablesCmd);
				}
			}
#endif
		}
	}
	else if( DEL_DHCP_PORTFILTER_CMD == iType )
	{
		memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
		sprintf(ebtablesCmd,"ebtables -t filter -F %s 2>/dev/null", PortFilterInputTable);
		system(ebtablesCmd);

		memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
		sprintf(ebtablesCmd,"ebtables -t filter -D INPUT -j %s", PortFilterInputTable);
		system(ebtablesCmd);

		memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
		sprintf(ebtablesCmd,"ebtables -t filter -X %s 2>/dev/null", PortFilterInputTable);
		system(ebtablesCmd);
	}

}

int UpdateDHCP_PortfilterRules()
{
	mxml_node_t *top = NULL;
	char bufCmd[128] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char active[8] = {0};
	char strWanMode[16] = {0};
	int pvc_index = 0;
	int entry_index = 0;

	if ( !isNeedUpdatePortFilter || !g_wan_boot_finish )
		return FAIL;
	isNeedUpdatePortFilter = 0;
	top = get_node_top();
	if ( NULL == top )
		return FAIL;

	for(pvc_index = 0; pvc_index < PVC_NUM; pvc_index++)
	{
		for(entry_index = 0; entry_index < MAX_SMUX_NUM; entry_index++)
		{
			memset(nodeName, 0, sizeof(nodeName));
			strcpy(nodeName[0], WAN);
			sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
			sprintf(nodeName[2], "%s%d", WAN_INTERFACE, entry_index);

			if ( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, NODE_ACTIVE, active) )
				continue;
			if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, WAN_MODE, strWanMode) )
				continue;

			set_DHCPRealy_info(top, pvc_index, entry_index, DEL_DHCPREALY_CMD);
			set_DHCP_PortFilter_info(top, pvc_index, entry_index, DEL_DHCP_PORTFILTER_CMD);

			if( 0 == strcmp(strWanMode,WAN_BRIDGE_MODE) )
				set_DHCPRealy_info(top, pvc_index, entry_index, ADD_DHCPREALY_CMD);
			set_DHCP_PortFilter_info(top, pvc_index, entry_index, ADD_DHCP_PORTFILTER_CMD);

		}
	}
}

#endif


/*_____________________________________________________________________________
**      function name: route_switch
**      descriptions:
**           switch other internet interface to default gateway.
**
**      parameters:
**            top: 			top cfg node
**			  route_index:		store curent default route index
**
**      global:
**             None
**
**      return:
**             void
**
**      call:
**
**      revision:
**      1. xyzhu_nj 20100901
**____________________________________________________________________________
*/
void route_switch(mxml_node_t *top, char *route_index, int state)
{
	char wan_info_node[MAX_ARG_NUM][MAX_NODE_NAME];
	char wan_node[MAX_ARG_NUM][MAX_NODE_NAME];
	char status[10], service_app[SERVICE_LIST_LEN];
	char if_name[10], gateway[45];
	char route_cmd[128];
	int i, pvc_index, entry_index;
#if defined(TCSUPPORT_CT_DSLITE)
	int dslite = 0;
#endif

	if (NULL == route_index) {
		return;
	}
	/* init the route index */
	strcpy(route_index, "N/A");
	
	memset(wan_info_node, 0, sizeof(wan_info_node));
	memset(wan_node, 0, sizeof(wan_node));
	strcpy(wan_info_node[0], "WanInfo");
	strcpy(wan_node[0], "Wan");
	for (i = 0; i < MAX_WAN_IF_INDEX; i++) {
		sprintf(wan_info_node[1], "Entry%d", i);

#if 0
		if (getAttrValue(top, wan_info_node, "Status", status) != TCAPI_PROCESS_OK) {
			continue;
		}
		if (0 != strcmp(status, "up")) {
			continue;
		}

		if (getAttrValue(top, wan_info_node, "GateWay", gateway) != TCAPI_PROCESS_OK) {
			continue;
		}

#endif
		/* we have find a default route */
		if (0 != strcmp(route_index, "N/A")) {
			break;
		}
		
		pvc_index = i / MAX_SMUX_NUM;
		entry_index = i - pvc_index * MAX_SMUX_NUM;
		sprintf(wan_node[1], "PVC%d", pvc_index);
		sprintf(wan_node[2], "Entry%d", entry_index);

		if (getAttrValue(top, wan_node, "ServiceList", service_app) != TCAPI_PROCESS_OK) {
			continue;
		}
#if defined(TCSUPPORT_CT_SERVICELIST_E8C)
		if ((NULL == strstr(service_app, INTERNET))) {
#else
		if ((0 != strcmp(service_app, INTERNET)) && (0 != strcmp(service_app, TR069_INTERNET))) {
#endif
			continue;
		}

		memset(if_name, 0, sizeof(if_name));
		if (getAttrValue(top, wan_node, "IFName", if_name) != TCAPI_PROCESS_OK) {
			continue;
		}
		if ('\0' == if_name) {
			continue;
		}

#if defined(TCSUPPORT_CT_DSLITE)
		dslite = get_dslite_stateviacfg(top, wan_node);
#endif

#ifdef CT_COM_DBUG
		tcdbg_printf("%s:add route here.", __FUNCTION__);
#endif

		switch (state) {
			case IF_STATE_DOWN:
#if defined(TCSUPPORT_CT_DSLITE)
				if ( dslite )
				{
					if( TCAPI_PROCESS_OK != getAttrValue(top, wan_info_node, "Status6", status) )
						continue;
					if (0 != strcmp(status, "up"))
						continue;
					if ( TCAPI_PROCESS_OK != getAttrValue(top, wan_info_node, "GateWay6", gateway) )
						continue;

					sprintf(route_cmd, "/sbin/route add default dev ds.%s", if_name);
					wan_cmd_excute(route_cmd);
				}
				else
				{
#endif
				if(getAttrValue(top, wan_info_node, "Status", status) != TCAPI_PROCESS_OK){
					continue;
				}
				if (0 != strcmp(status, "up")) {
					continue;
				}

				if (getAttrValue(top, wan_info_node, "GateWay", gateway) != TCAPI_PROCESS_OK) {
					continue;
				}
				
				dealWithLinkLocalRoute(top,if_name,i,2);				
				sprintf(route_cmd, "/sbin/route add default gw %s dev %s", gateway, if_name);
				wan_cmd_excute(route_cmd);
#if defined(TCSUPPORT_CT_DSLITE)
				}
#endif
				sprintf(route_index, "%d", i);
				break;
			case IF_STATE_DOWN6:
				if(getAttrValue(top, wan_info_node, "Status6", status) != TCAPI_PROCESS_OK){
					continue;
				}
				if (0 != strcmp(status, "up")) {
					continue;
				}

				if (getAttrValue(top, wan_info_node, "GateWay6", gateway) != TCAPI_PROCESS_OK) {
					continue;
				}
				sprintf(route_cmd, "/sbin/route -A inet6 add default gw %s dev %s", gateway, if_name);
				wan_cmd_excute(route_cmd);
				sprintf(route_index, "%d", i);
				break;
			default:
				break;
		}
	}

	return;
}

/*_____________________________________________________________________________
**      function name: set_default_route
**      descriptions:
**           add default route for ipv4
** 		     del default route for ipv6
**
**      parameters:
**            top: 			top cfg node
**			  if_name:		interfce name
**			  if_index:		interface index
*			  state:		interface state
**
**      global:
**             None
**
**      return:
**             void
**
**      call:
**
**      revision:
**      1. xyzhu_nj 20100827
**____________________________________________________________________________
*/
void set_default_route(mxml_node_t *top, char *if_name, int if_index, int state)
{
	char wanNode[MAX_ARG_NUM][MAX_NODE_NAME];
	char wanInfoNode[MAX_ARG_NUM][MAX_NODE_NAME];
	char gate_way[20], serviceApp[SERVICE_LIST_LEN], def_route_index_v4[10], def_route_index_v6[10], gate_way6[45] = {0};
	char route_cmd[128];
	int pvc_index, entry_index;
#if defined(TCSUPPORT_CT_DSLITE)
	int dslite = 0;
#endif
	long type = 1;//tr69 must be 1
	int mq = 2; //value change type 
	int msgFlag = IPC_NOWAIT;
	cwmp_msg_t message;

	pvc_index = if_index / MAX_SMUX_NUM;
	entry_index = if_index - pvc_index * MAX_SMUX_NUM;
	
	memset(wanNode, 0, sizeof(wanNode));
	strcpy(wanNode[0], "Wan");
	sprintf(wanNode[1], "PVC%d", pvc_index);
	sprintf(wanNode[2], "Entry%d", entry_index);

	if (getAttrValue(top, wanNode, "ServiceList", serviceApp) != TCAPI_PROCESS_OK) {
	#ifdef CT_COM_DBUG
		tcdbg_printf("%s:get service list failed.\n", __FUNCTION__);
	#endif
		return;
	}

#if defined(TCSUPPORT_CT_DSLITE)
	dslite = get_dslite_stateviacfg(top, wanNode);
#endif

	memset(wanInfoNode, 0, sizeof(wanInfoNode));
	strcpy(wanInfoNode[0], "WanInfo");
	sprintf(wanInfoNode[1], "Entry%d", if_index);
	if (IF_STATE_UP == state) {
		if (getAttrValue(top, wanInfoNode, "GateWay", gate_way) != TCAPI_PROCESS_OK || strlen(gate_way) == 0) {
		#ifdef CT_COM_DBUG
			tcdbg_printf("%s:get gateway failed.\n", __FUNCTION__);
		#endif
			return;
		}
	}
	else if( IF_STATE_UP6 == state ){
		if (getAttrValue(top, wanInfoNode, "GateWay6", gate_way6) != TCAPI_PROCESS_OK || strlen(gate_way6) == 0) {
		#ifdef CT_COM_DBUG
			tcdbg_printf("%s:get gateway failed.\n", __FUNCTION__);
		#endif
			return;
		}
	}

	strcpy(wanInfoNode[1], "Common");
	/* get default route for ipv4 */
	memset(def_route_index_v4, 0, sizeof(def_route_index_v4));
	if (getAttrValue(top, wanInfoNode, "DefRouteIndexv4", def_route_index_v4) != TCAPI_PROCESS_OK) {
		strcpy(def_route_index_v4, "N/A");
		setAttrValue(top, wanInfoNode, "DefRouteIndexv4", def_route_index_v4);
	}
	/* get default route for ipv6 */
	memset(def_route_index_v6, 0, sizeof(def_route_index_v6));
	if (getAttrValue(top, wanInfoNode, "DefRouteIndexv6", def_route_index_v6) != TCAPI_PROCESS_OK) {
		strcpy(def_route_index_v6, "N/A");
		setAttrValue(top, wanInfoNode, "DefRouteIndexv6", def_route_index_v6);
	}

#if 0
	/* for ipv6 interface up, delete it's default route first */
	if (IF_STATE_UP6 == state) {
		//sprintf(route_cmd, "/sbin/route -A inet6 del default gw %s dev %s", gate_way6, if_name);
		//wan_cmd_excute(route_cmd);
		//sprintf(route_cmd, "/sbin/route -A inet6 del default dev %s", if_name);
		sprintf(route_cmd, "/usr/bin/ip -6 route del  default via %s dev %s", gate_way6, if_name);
		wan_cmd_excute(route_cmd);		
	}
#endif	
	message.cwmptype = mq; 
	sendmegq(type,&message,msgFlag);

		/* only internet interface will we add default route */
	switch (state) {
		case IF_STATE_UP:
#if defined(TCSUPPORT_CT_SERVICELIST_E8C)
			if (strstr(serviceApp, INTERNET)!=NULL) {
#else
			if (!strcmp(serviceApp, INTERNET) || !strcmp(serviceApp, TR069_INTERNET)) {
#endif
#if defined(TCSUPPORT_CT_L2TP_VPN)
			open_VPN_PPP_tunnel(top);
#endif
				if (!strcmp(def_route_index_v4, "N/A")) {
					sprintf(route_cmd, "/sbin/route add default gw %s dev %s", gate_way, if_name);
					wan_cmd_excute(route_cmd);

					/* update if_index to wanInfo node */
					sprintf(def_route_index_v4, "%d", if_index);
					setAttrValue(top, wanInfoNode, "DefRouteIndexv4", def_route_index_v4);
					dealWithLinkLocalRoute(top,if_name,if_index,1);
				}else{
					dealWithLinkLocalRoute(top,if_name,if_index,0);
				}
			}else{
				dealWithLinkLocalRoute(top,if_name,if_index,0);
			}
			break;
		case IF_STATE_DOWN:
			if (0 != strcmp(def_route_index_v4, "N/A")) {
				/* default route interface is down, find another INTERNET interface to replace it. */
				if (if_index == atoi(def_route_index_v4)) {
					route_switch(top, def_route_index_v4, state);
				}
				setAttrValue(top, wanInfoNode, "DefRouteIndexv4", def_route_index_v4);
			}
			break;
		case IF_STATE_UP6:
#if defined(TCSUPPORT_CT_SERVICELIST_E8C)
			if (strstr(serviceApp, INTERNET)!=NULL) {
#else
			if (!strcmp(serviceApp, INTERNET) || !strcmp(serviceApp, TR069_INTERNET)) {
#endif
				if (!strcmp(def_route_index_v6, "N/A")) {
					sprintf(route_cmd, "/sbin/route -A inet6 add default gw %s dev %s", gate_way6, if_name);
					wan_cmd_excute(route_cmd);
				
					/* update if_index to wanInfo node */
					sprintf(def_route_index_v6, "%d", if_index);					
					setAttrValue(top, wanInfoNode, "DefRouteIndexv6", def_route_index_v6);
				}
			}
			break;
		case IF_STATE_DOWN6:
			if (0 != strcmp(def_route_index_v6, "N/A")) {
				/* default route interface is down, fine another INTERNET interface to replace it. */
				if (if_index == atoi(def_route_index_v6)) {
					route_switch(top, def_route_index_v6, state);
				}
					setAttrValue(top, wanInfoNode, "DefRouteIndexv6", def_route_index_v6);
			}
#if defined(TCSUPPORT_CT_DSLITE)
			if ( dslite
				&& 0 != strcmp(def_route_index_v4, "N/A")) 
			{
				/* default route for dslite interface is down, find another V4 INTERNET interface to replace it. */
				if (if_index == atoi(def_route_index_v4))
				{
					route_switch(top, def_route_index_v4, IF_STATE_DOWN);
					setAttrValue(top, wanInfoNode, "DefRouteIndexv4", def_route_index_v4);
				}
			}
#endif
			break;
		default:
			break;
	}
		
	return;
}

#ifdef CWMP
/*_____________________________________________________________________________
**      function name: set_tr069_nat
**      descriptions:
**			 del nat for tr069 interface.
**
**      parameters:
**            top: 			top cfg node
**			  if_name:		interfce name
**			  if_index:		interface index
*			  state:		interface state
**
**      global:
**             None
**
**      return:
**             void
**
**      call:
**
**      revision:
**      1. xyzhu_nj 20100827
**____________________________________________________________________________
*/
void set_tr069_nat(mxml_node_t *top, char *if_name, int if_index, int state)
{
	char wanNode[MAX_ARG_NUM][MAX_NODE_NAME];
	char serviceApp[SERVICE_LIST_LEN];
	int pvc_index, entry_index;
	char nat_cmd[128];

	pvc_index = if_index / MAX_SMUX_NUM;
	entry_index = if_index - pvc_index * MAX_SMUX_NUM;
	
	memset(wanNode, 0, sizeof(wanNode));
	strcpy(wanNode[0], "Wan");
	sprintf(wanNode[1], "PVC%d", pvc_index);
	sprintf(wanNode[2], "Entry%d", entry_index);

	/* if the interface is TR069 type, remove nat rule */
	if (getAttrValue(top, wanNode, "ServiceList", serviceApp) == TCAPI_PROCESS_OK) {
		if (!strcmp(serviceApp, TR069)) {
			sprintf(nat_cmd, "iptables -t nat -D POSTROUTING -o %s -j MASQUERADE 2>/dev/null", if_name);
			wan_cmd_excute(nat_cmd);
		}
	}

	return;
}

/*_____________________________________________________________________________
**      function name: del_if_from_conf
**      descriptions:
**			 delete the specific interface entry from config file.
**
**      parameters:
**			  if_name:		interfce name
**
**      global:
**             None
**
**      return:
**             void
**
**      call:
**
**      revision:
**      1. xyzhu_nj 20100828
**____________________________________________________________________________
*/
void del_if_from_conf(char *if_name)
{
	FILE *fp = NULL, *fp2 = NULL;
	char line[64];

	memset(line, 0, sizeof(line));
	fp = fopen(TRO69_IF_CONF, "r");
	if (NULL == fp) {
		return;
	}

	fp2 = fopen(TRO69_IF_CONF2, "w");
	if (NULL == fp2) {
		fclose(fp);
		return;
	}

	while (fgets(line, sizeof(line), fp)) {
		if (NULL == strstr(line, if_name)) {
			fputs(line, fp2);
		}
	}

	fclose(fp);
	fclose(fp2);
	unlink(TRO69_IF_CONF);
	rename(TRO69_IF_CONF2, TRO69_IF_CONF);

	return;
}

/*_____________________________________________________________________________
**      function name: update_cwmp_route_node
**      descriptions:
**			 update cwmp route node from route config file.
**
**      parameters:
**            top: 			top cfg node
**			  if_name:		interfce name
**			  if_index:		interface index
*			  state:		interface state
**
**      global:
**             None
**
**      return:
**             void
**
**      call:
**
**      revision:
**      1. xyzhu_nj 20100828
**____________________________________________________________________________
*/
void update_cwmp_route_node(mxml_node_t *top)
{
	char cwmp_route_node[MAX_ARG_NUM][MAX_NODE_NAME];
	char line[64], if_name[10], if_gateway[20];
	char if_name_attr[10], if_gateway_attr[20], if_status_attr[20], route_num[10];
	FILE *fp = NULL;
	int i = 0;
	mxml_node_t *curNode = NULL;

	memset(cwmp_route_node, 0, sizeof(cwmp_route_node));
	strcpy(cwmp_route_node[0], "CwmpRoute");
	strcpy(cwmp_route_node[1], "Entry");
	/* delete this node first */
	curNode = mxmlFindElement(top, top, cwmp_route_node[0],
	  		NULL, NULL,
	  		MXML_DESCEND);
	if (curNode) {
		mxmlDelete(curNode);
	}
	
	fp = fopen(TRO69_IF_CONF, "r");
	if (NULL == fp) {
	#ifdef CT_COM_DBUG
		tcdbg_printf("%s %s can not be open.\n", __FUNCTION__, TRO69_IF_CONF);
	#endif
		return;
	}

	memset(line, 0, sizeof(line));
	memset(if_name, 0, sizeof(if_name));
	memset(if_gateway, 0, sizeof(if_gateway));
	
	curNode = get_web_node(top, cwmp_route_node);
	if (NULL == curNode) {
		fclose(fp);
		tcdbg_printf("%s: cwmp route node create failed.\n", __FUNCTION__);
		return;
	}
	/* init route num to 0 */
	mxmlElementSetAttr(curNode, "RouteNum", "0");
	
	while (fgets(line, sizeof(line), fp)) {
		if ('\0' != line[0]) {
			sscanf(line, "%s %s", if_name, if_gateway);
	#ifdef CT_COM_DBUG
			tcdbg_printf("%s: line is %s\n", __FUNCTION__, line);
			tcdbg_printf("%s: interface name is %s\n", __FUNCTION__, if_name);
			tcdbg_printf("%s: interface gateway is %s\n", __FUNCTION__, if_gateway);
	#endif
			sprintf(if_name_attr, "ifName%d", i);
			sprintf(if_gateway_attr, "ifGateway%d", i);
			sprintf(if_status_attr, "ifStatus%d", i);
			mxmlElementSetAttr(curNode, if_name_attr, if_name);
			mxmlElementSetAttr(curNode, if_gateway_attr, if_gateway);
			mxmlElementSetAttr(curNode, if_status_attr, "new");

			i++;
			memset(line, 0, sizeof(line));
			memset(if_name, 0, sizeof(if_name));
			memset(if_gateway, 0, sizeof(if_gateway));
		}
	}

	/* update route num */
	sprintf(route_num, "%d", i);
	mxmlElementSetAttr(curNode, "RouteNum", route_num);
	fclose(fp);
	return;
}

/*_____________________________________________________________________________
**      function name: update_cwmp_route
**      descriptions:
**			 update route config file.
**
**      parameters:
**            top: 			top cfg node
**			  if_name:		interfce name
**			  if_index:		interface index
*			  state:		interface state
**
**      global:
**             None
**
**      return:
**             void
**
**      call:
**
**      revision:
**      1. xyzhu_nj 20100828
**____________________________________________________________________________
*/
void update_cwmp_route(mxml_node_t *top, char *if_name, int if_index, int state)
{
	char wan_info_node[MAX_ARG_NUM][MAX_NODE_NAME];
	char wan_node[MAX_ARG_NUM][MAX_NODE_NAME];
	int pvc_index, entry_index;
	char gateway[32], service_app[SERVICE_LIST_LEN];
	char route_cmd[128];
#if defined(TCSUPPORT_CT_PMINFORM)
	char wanip[20] = {0};
#endif

	/* get service list of this interface */
	pvc_index = if_index / MAX_SMUX_NUM;
	entry_index = if_index - pvc_index * MAX_SMUX_NUM;
	memset(wan_node, 0, sizeof(wan_node));
	strcpy(wan_node[0], "Wan");
	sprintf(wan_node[1], "PVC%d", pvc_index);
	sprintf(wan_node[2], "Entry%d", entry_index);
	memset(service_app, 0, sizeof(service_app));
	if (getAttrValue(top, wan_node, "ServiceList", service_app) == TCAPI_PROCESS_OK) {
		if ('\0' == service_app[0]) {
			return;
		}
	}

	/* get gateway of this interface */
	memset(wan_info_node, 0, sizeof(wan_info_node));
	strcpy(wan_info_node[0], "WanInfo");
	sprintf(wan_info_node[1], "Entry%d", if_index);
	memset(gateway, 0, sizeof(gateway));
	if (getAttrValue(top, wan_info_node, "GateWay", gateway) == TCAPI_PROCESS_OK) {
		if ('\0' == gateway[0]) {
			return;
		}
	}
#if defined(TCSUPPORT_CT_PMINFORM)
	getAttrValue(top, wan_info_node, "IP", wanip);
#endif

#ifdef CT_COM_DBUG
	tcdbg_printf("%s:service list is %s, gateway is %s.\n", __FUNCTION__, service_app, gateway);
#endif

	/* update route config file, only for tr069 interface */
	memset(route_cmd, 0, sizeof(route_cmd));
#if defined(TCSUPPORT_CT_SERVICELIST_E8C)
	if (strstr(service_app, TR069) != NULL) {
#else
	if (!strcmp(service_app, TR069) || !strcmp(service_app, TR069_INTERNET)) {
#endif
		switch (state) {
			case IF_STATE_UP:
				sprintf(route_cmd, "/bin/echo \"%s %s\" >> %s", if_name, gateway, TRO69_IF_CONF);
				wan_cmd_excute(route_cmd);
				break;

			default:
				break;	
		}
#if defined(TCSUPPORT_CT_PMINFORM)
		updateCwmpInfo(top, wanip, state);
#endif
	}

	/* delete tr069 route from config file */
	if (IF_STATE_DOWN == state) {
	#ifdef CT_COM_DBUG
		tcdbg_printf("%s:delete route info for cwmp from config file.\n", __FUNCTION__);
	#endif
		del_if_from_conf(if_name);
	}
	
	/* update cwmpRoute node */
	update_cwmp_route_node(top);

	return;
}
#endif

#if defined(TCSUPPORT_CT_DNSBIND)
/*_____________________________________________________________________________
**      function name: set_policy_route
**      descriptions:
**			 set policy route for wan interface.
**
**      parameters:
**            top: 			top cfg node
**			  if_name:		interfce name
**			  if_index:		interface index
*			  state:		interface state
**
**      global:
**             None
**
**      return:
**             void
**
**      call:
**
**      revision:
**      1. xyzhu_nj 20100928
**____________________________________________________________________________
*/
//#define 	WAN_IF_MARK		0x3f0000
#define 	WAN_IF_MARK		0x7f0000 /* xyzhu_nj_20101008, lan pc can not ping cpe if wan interface up */
void set_policy_route(mxml_node_t *top, char *if_name, int if_index, int state)
{
	char wan_info_node[MAX_ARG_NUM][MAX_NODE_NAME], gateway[128], wanIp[128], netMask[128];
	char wan_pvc_node[MAX_ARG_NUM][MAX_NODE_NAME], linkMode[20];
	int pvc_index = 0, entry_index = 0;
	char tempIp[128] = {0}, mask_dec[4] = {0};
	struct in_addr tempWanIp, tempNetMask;
	char rule_buf[128];
	unsigned int mark;
	char wanMode[32] = {0};
	char lan_node[MAX_ARG_NUM][MAX_NODE_NAME];
	char lanIP[64] = {0},lannetmask[64] = {0};
	struct in_addr tempLanIp, tempLanNetMask;

	//mark = if_index << 16;
	mark = (if_index + 1) << 16; /* xyzhu_nj_20101008, lan pc can not ping cpe if wan interface up */

	//tcdbg_printf("set_policy_route: mark is %x\n", mark);
	
	/* set skb->mark match wan interface use iptables/ip6tables */
	switch (state) {
		case IF_STATE_UP:
			sprintf(rule_buf, "iptables -t mangle -D OUTPUT -o %s -j MARK --set-mark 0x%x/0x7f0000", if_name, mark);
			wan_cmd_excute(rule_buf);
			sprintf(rule_buf, "iptables -t mangle -A OUTPUT -o %s -j MARK --set-mark 0x%x/0x7f0000", if_name, mark);
			wan_cmd_excute(rule_buf);
			break;
		case IF_STATE_DOWN:
			sprintf(rule_buf, "iptables -t mangle -D OUTPUT -o %s -j MARK --set-mark 0x%x/0x7f0000", if_name, mark);
			wan_cmd_excute(rule_buf);
			break;
	#if 0
		case IF_STATE_UP6:
			/* ip6 tables does not support or mark, it need modify */
			sprintf(rule_buf, "ip6tables -t mangle -A OUTPUT -o %s -j MARK --set-mark 0x%x", if_name, mark);
			//wan_cmd_excute(rule_buf);
			break;
		case IF_STATE_DOWN6:
			/* ip6 tables does not support or mark, it need modify */
			sprintf(rule_buf, "ip6tables -t mangle -D OUTPUT -o %s -j MARK --set-mark 0x%x", if_name, mark);
			//wan_cmd_excute(rule_buf);
			break;
	#endif
		default:
			break;
	}
		
	/* set policy route for wan interface*/
	memset(wan_info_node, 0, sizeof(wan_info_node));
	strcpy(wan_info_node[0], "WanInfo");
	sprintf(wan_info_node[1], "Entry%d", if_index);

	pvc_index = if_index / MAX_SMUX_NUM;
	entry_index = if_index - pvc_index * MAX_SMUX_NUM;
	memset(wan_pvc_node, 0, sizeof(wan_pvc_node));
	strcpy(wan_pvc_node[0], "Wan");
	sprintf(wan_pvc_node[1], "PVC%d", pvc_index);
	sprintf(wan_pvc_node[2], "Entry%d", entry_index);

	switch (state) {
		case IF_STATE_UP:
			sprintf(rule_buf, "/usr/bin/ip rule del fwmark 0x%x/0x%x table %d", mark, WAN_IF_MARK, 100 + if_index);
			wan_cmd_excute(rule_buf);
			sprintf(rule_buf, "/usr/bin/ip rule add fwmark 0x%x/0x%x table %d", mark, WAN_IF_MARK, 100 + if_index);
			wan_cmd_excute(rule_buf);
			if (getAttrValue(top, wan_info_node, "GateWay", gateway)  == TCAPI_PROCESS_OK) {
				sprintf(rule_buf, "/usr/bin/ip route add default via %s dev %s table %d", gateway, if_name, 100 + if_index);
				wan_cmd_excute(rule_buf);
			}
			memset(linkMode, 0, sizeof(linkMode));
			memset(wanIp, 0, sizeof(wanIp));
			memset(netMask, 0, sizeof(netMask));
			memset(tempIp, 0, sizeof(tempIp));
			if (getAttrValue(top, wan_pvc_node, "LinkMode", linkMode)  == TCAPI_PROCESS_OK && strcmp(linkMode, "linkIP") == 0) {
				if(getAttrValue(top, wan_info_node, "IP", wanIp)  == TCAPI_PROCESS_OK && getAttrValue(top, wan_info_node, "NetMask", netMask)  == TCAPI_PROCESS_OK)
				{
					if ( !check_mask_format(netMask, mask_dec) )
						return FAIL;
					
					inet_aton(wanIp, &tempWanIp);
					inet_aton(netMask, &tempNetMask);
					tempWanIp.s_addr &= tempNetMask.s_addr;
					sprintf(tempIp, "%s", inet_ntoa(tempWanIp));
					
					sprintf(rule_buf, "/usr/bin/ip route add %s/%s dev %s table %d", tempIp, mask_dec, if_name, 100 + if_index);
					wan_cmd_excute(rule_buf);				
				}
			}

			if(getAttrValue(top, wan_pvc_node, "WanMode", wanMode)  == TCAPI_PROCESS_OK && strcmp(wanMode,"Route") == 0){
				if(getAttrValue(top, wan_info_node, "IP", wanIp)  == TCAPI_PROCESS_OK ){
					sprintf(rule_buf, "/usr/bin/ip rule add from %s/32 table %d", wanIp, 100 + if_index);
					wan_cmd_excute(rule_buf);

					memset(mask_dec, 0, sizeof(mask_dec));
					memset(tempIp, 0, sizeof(tempIp));
					memset(lan_node, 0, sizeof(lan_node));
					strcpy(lan_node[0], LAN);
					strcpy(lan_node[1], LAN_ENTRY_0);
					if(getAttrValue(top, lan_node, LAN_IP, lanIP)	== TCAPI_PROCESS_OK && getAttrValue(top, lan_node, LAN_MASK, lannetmask) == TCAPI_PROCESS_OK){
						if ( !check_mask_format(lannetmask, mask_dec) ){
							sprintf(mask_dec, "%d", 24);
						}
											
						inet_aton(lanIP, &tempLanIp);
						inet_aton(lannetmask, &tempLanNetMask);
						tempLanIp.s_addr &= tempLanNetMask.s_addr;
						sprintf(tempIp, "%s", inet_ntoa(tempLanIp));
						sprintf(rule_buf, "/usr/bin/ip route add %s/%s via %s dev br0 table %d", tempIp, mask_dec, lanIP, 100 + if_index);
					}else{
						sprintf(rule_buf, "/usr/bin/ip route add 192.168.1.0/24 via 192.168.1.1 dev br0 table %d", 100 + if_index);
					}
					wan_cmd_excute(rule_buf);
					setAttrValue(top, wan_pvc_node, "IP_last", wanIp);
				}
			}
			strcpy(rule_buf, "/usr/bin/ip route flush cache 2>/dev/null");
			wan_cmd_excute(rule_buf);
			break;
		case IF_STATE_DOWN:
			memset(wanIp, 0, sizeof(wanIp));
			if(getAttrValue(top, wan_pvc_node, "IP_last", wanIp)  == TCAPI_PROCESS_OK)
			{
				sprintf(rule_buf, "/usr/bin/ip rule del from %s/32 table %d", wanIp, 100 + if_index);
				wan_cmd_excute(rule_buf);
			}

			sprintf(rule_buf, "/usr/bin/ip rule del fwmark 0x%x/0x%x table %d", mark, WAN_IF_MARK, 100 + if_index);
			wan_cmd_excute(rule_buf);
			sprintf(rule_buf, "/usr/bin/ip route flush table %d", 100 + if_index);
			break;
		case IF_STATE_UP6:
			sprintf(rule_buf, "/usr/bin/ip -6 rule del fwmark 0x%x/0x%x table %d", mark, WAN_IF_MARK, 100 + if_index);
			wan_cmd_excute(rule_buf);
			sprintf(rule_buf, "/usr/bin/ip -6 rule add fwmark 0x%x/0x%x table %d", mark, WAN_IF_MARK, 100 + if_index);
			wan_cmd_excute(rule_buf);
			if (getAttrValue(top, wan_info_node, "GateWay6", gateway)  == TCAPI_PROCESS_OK) {
				sprintf(rule_buf, "/usr/bin/ip -6 route add default via %s dev %s table %d", gateway, if_name, 100 + if_index);
				wan_cmd_excute(rule_buf);
			}
		#if 0
			strcpy(rule_buf, "/usr/bin/ip -6 route flush cache 2>/dev/null");
			wan_cmd_excute(rule_buf);
		#endif
			break;
		case IF_STATE_DOWN6:
			sprintf(rule_buf, "/usr/bin/ip -6 rule del fwmark 0x%x/0x%x table %d", mark, WAN_IF_MARK, 100 + if_index);
			wan_cmd_excute(rule_buf);
			sprintf(rule_buf, "/usr/bin/ip -6 route flush table %d", 100 + if_index);
			wan_cmd_excute(rule_buf);
			break;
		default:
			break;
	}

	return;
}
#endif

/*_____________________________________________________________________________
**      function name: wan_cmd_excute
**      descriptions:
**			 excute cmd for wan interface.
**
**      parameters:
**            cmd:	the cmd need be excuted.
**
**      global:
**             None
**
**      return:
**             void
**
**      call:
**
**      revision:
**      1. xyzhu_nj 20100828
**____________________________________________________________________________
*/
void wan_cmd_excute(char *cmd)
{
	if (NULL == cmd) {
		tcdbg_printf("==>wan_cmd_excute(): cmd is null.\n");
		return;
	}

	if ('\0' == *cmd) {
		return;
	}
	
#ifdef CT_COM_DBUG
	tcdbg_printf("WANCMD=%s\n", cmd);
#endif
	system(cmd);	

	return;
}

#endif
#if defined(TCSUPPORT_CT_QOS)
/*_____________________________________________________________________________
**      function name: update_qos_vlan_rule
**      descriptions:
**			 update qos vlan rule
**
**      parameters:
**            top: 
**  		  if_index: interface index
**
**      global:
**             None
**
**      return:
**             void
**
**      call:
**
**      revision:
**      1. xyzhu_nj 20101025
**____________________________________________________________________________
*/
void update_qos_vlan_rule(mxml_node_t *top, int if_index)
{
	int pvc_index = if_index / MAX_SMUX_NUM;
	char qos_node[34];

	strcpy(qos_node, "QoS_Common");
	tcapi_set_internal(top, qos_node, "QoSOptType", "vlanidRule");
	
	sprintf(qos_node, "QoS_Entry%d", pvc_index + 22);
	tcapi_commit_internal(top, qos_node);

	return;
}
#endif


#if defined(TCSUPPORT_CT_PPPOEPROXY)

struct ifPolicyRoute
{
	char *ifName;
	char *mark;
	char *tabid;
};

struct ifPolicyRoute ifpr[10] = 
{
	/* ethernet interface */
#if !defined(TCSUPPORT_TC2031) 
	/* for four ports */
	{"eth0.1",	"0x10000000",	"204"},
	{"eth0.2",	"0x20000000",	"205"},
	{"eth0.3",	"0x30000000",	"206"},
	{"eth0.4",	"0x40000000",	"207"},
#endif
	/* wireless interface */
	{"ra0",		"0x50000000",	"208"},
	{"ra1",		"0x60000000",	"209"},
	{"ra2",		"0x70000000",	"210"},
	{"ra3",		"0x80000000",	"211"},
	/* usb interface */
	{"usb0",	"0x90000000",	"212"}
};

int find_if_in_pppoeproxymsg(char *realif, char *pvc){
	FILE *fp = NULL;
	char temp[64] = {0};
	int pid = 0;
	char pvcname[32] = {0};
	char lan_if[32] = {0};
	
	fp=fopen("/tmp/pppoeproxymsg","r+");
	if(fp == NULL){
		return 0;
	}
	while(fgets(temp, sizeof(temp), fp) != NULL){
		sscanf(temp,"%s %d %s",pvcname,&pid, lan_if);
		if((strcmp(lan_if, realif) == 0) && (strcmp(pvcname,pvc) != 0)){		
			tcdbg_printf("\nfind %s in pppoeproxymsg, and pvcname is %s not as %s", realif, pvcname, pvc);
			fclose(fp);
			return 1;
		}		
	}
	fclose(fp);
	return 0;
}

void set_blackhole_route(mxml_node_t *top, char *if_name, int if_index)
{
	char wanNode[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char wanInfoNode[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char ip_version[20] = {0};
	char bind_ifName_list[64] = {0}, bind_ifType_list[20] = {0}, bl_buf[10] = {0};
	char gate_way[20] = {0}, gate_way6[256] = {0};
	char portbind_cmd[128] = {0};
	int i = 0, j = 0, pvc_index = 0, entry_index=0, ip_flag = -1; /* 0:IPv4, 1:IPv6, 2:IPv4/IPv6 */
	char lanMk[20] = {0}, lanTabId[10] = {0};
	char prBuf[128];
	char pvcname[16] = {0};
	pvc_index = if_index / MAX_SMUX_NUM;
	entry_index = if_index - pvc_index * MAX_SMUX_NUM;


	memset(wanNode, 0, sizeof(wanNode));
	strncpy(wanNode[0], "Wan", sizeof(wanNode[0]));
	snprintf(wanNode[1], sizeof(wanNode[1]), "PVC%d", pvc_index);
	snprintf(wanNode[2], sizeof(wanNode[2]), "Entry%d", entry_index);
	sprintf(pvcname, "PVC%d_Entry%d", pvc_index, entry_index);

#ifdef TCSUPPORT_PORTBIND

	getAttrValue(top, wanNode, bl_map_data[i].bindif, bl_buf);
#endif
		memset(ip_version, 0, sizeof(ip_version));
		/* get service list from wan node */
		if (getAttrValue(top, wanNode, "IPVERSION", ip_version) != TCAPI_PROCESS_OK) {
		#ifdef CT_COM_DBUG
			tcdbg_printf("%s: get ip version failed.\n", __FUNCTION__);
		#endif
		}
		
		/* check ip version */
		if (!strcmp(ip_version, "IPv4")) {
			ip_flag = 0;
		}
		else if (!strcmp(ip_version, "IPv6")) {
			ip_flag = 1;
		}
		else if (!strcmp(ip_version, "IPv4/IPv6")) {
			ip_flag = 2;
		}
		else {
			/* may bridge mode */
			return;
		}
		/* get wan gate way */
		memset(wanInfoNode, 0, sizeof(wanInfoNode));
		strncpy(wanInfoNode[0], "WanInfo", sizeof(wanInfoNode[0]));
		snprintf(wanInfoNode[1], sizeof(wanInfoNode[1]), "Entry%d", if_index);
		
		memset(gate_way, 0, sizeof(gate_way));
		memset(gate_way6, 0, sizeof(gate_way6));
		/* get ipv4 gateway */
		/* ipv4 */
		if (ip_flag == 0 || ip_flag == 2) {
			getAttrValue(top, wanInfoNode, "GateWay", gate_way);
		}
		/* ipv6 */
		else if (ip_flag == 1 || ip_flag == 2) {
			getAttrValue(top, wanInfoNode, "GateWay6", gate_way6);
			
		}
#ifdef TCSUPPORT_PORTBIND
		/* get bindlist for wan interface */
		memset(bind_ifName_list, 0, sizeof(bind_ifName_list));
		memset(bind_ifType_list, 0, sizeof(bind_ifType_list));
		for (i = 0; i < sizeof(bl_map_data) / sizeof(bl_map_data[0]); i++) {
			if (getAttrValue(top, wanNode, bl_map_data[i].bindif, bl_buf) == TCAPI_PROCESS_OK) {
				if (!strcmp(bl_buf, "Yes") && 
					(!(find_if_in_pppoeproxymsg(bl_map_data[i].realif, pvcname)))) {
					/* add ip rule for wan interface */
					if ('\0' != gate_way[0]) {
						/* for ipv4 */
						snprintf(portbind_cmd, sizeof(portbind_cmd), "/usr/bin/portbindcmd delroute all %s", bl_map_data[i].realif);
						wan_cmd_excute(portbind_cmd);
						snprintf(portbind_cmd, sizeof(portbind_cmd), "/usr/bin/portbindcmd addroute v4 %s %s %s", bl_map_data[i].realif, if_name, gate_way);
						wan_cmd_excute(portbind_cmd);
						snprintf(portbind_cmd, sizeof(portbind_cmd), "/usr/bin/ip route del default table %s", bl_map_data[i].realif);
						wan_cmd_excute(portbind_cmd);
						snprintf(portbind_cmd, sizeof(portbind_cmd), "/usr/bin/ip route add blackhole default table %s", bl_map_data[i].realif);
						wan_cmd_excute(portbind_cmd);

					}

					
					else if ('\0' != gate_way6[0]) {
						/* for ipv6 */
						snprintf(portbind_cmd, sizeof(portbind_cmd), "/usr/bin/portbindcmd delroute all %s", bl_map_data[i].realif);
						wan_cmd_excute(portbind_cmd);
						snprintf(portbind_cmd, sizeof(portbind_cmd), "/usr/bin/portbindcmd addroute v6 %s %s %s", bl_map_data[i].realif, if_name, gate_way6);
						wan_cmd_excute(portbind_cmd);
						snprintf(portbind_cmd, sizeof(portbind_cmd), "/usr/bin/ip route del default table %s", bl_map_data[i].realif);
						wan_cmd_excute(portbind_cmd);
						snprintf(portbind_cmd, sizeof(portbind_cmd), "/usr/bin/ip route add blackhole default table %s", bl_map_data[i].realif);
						wan_cmd_excute(portbind_cmd);
					}
					else{
							for (j = 0; j < sizeof(ifpr) / sizeof(ifpr[0]); j++) {
							if (!strcmp(ifpr[j].ifName, bl_map_data[i].realif)) {
								strcpy(lanMk, ifpr[j].mark);
								strcpy(lanTabId, ifpr[j].tabid);
								break;
							}
						}

						snprintf(portbind_cmd, sizeof(portbind_cmd), "/usr/bin/portbindcmd delroute all %s", bl_map_data[i].realif);
						wan_cmd_excute(portbind_cmd);
						if(ip_flag == 0){
							/* 1. flush route table */
							snprintf(prBuf, sizeof(prBuf), "/usr/bin/ip route flush table %s 2>/dev/null", lanTabId);
							wan_cmd_excute(prBuf);

							/* 2. flush rule */
							snprintf(prBuf, sizeof(prBuf), "/usr/bin/ip rule del fwmark %s/0xf0000000 table %s 2>/dev/null", lanMk, lanTabId);
							wan_cmd_excute(prBuf);
							snprintf(prBuf, sizeof(prBuf),"/usr/bin/ip rule add fwmark %s/0xf0000000 table %s 2>/dev/null", lanMk, lanTabId);
							wan_cmd_excute(prBuf);

							/* 3. add route */
							snprintf(prBuf, sizeof(prBuf), "/usr/bin/ip route add 192.168.1.0/24 via 192.168.1.1 dev br0 table %s 2>/dev/null", lanTabId);
							wan_cmd_excute(prBuf);
							snprintf(prBuf, sizeof(prBuf), "/usr/bin/ip route add blackhole default table %s 2>/dev/null", lanTabId);
							wan_cmd_excute(prBuf);

							/* ipv4 end */
							snprintf(prBuf, sizeof(prBuf), "/usr/bin/ip route flush cache 2>/dev/null");
							wan_cmd_excute(prBuf);}
							
						if(ip_flag == 1 || ip_flag == 2){
							snprintf(prBuf, sizeof(prBuf), "/usr/bin/ip -6 route flush table %s 2>/dev/null", lanTabId);
							wan_cmd_excute(prBuf);
		
							snprintf(prBuf, sizeof(prBuf), "/usr/bin/ip -6 rule del fwmark %s/0xf0000000 table %s 2>/dev/null", lanMk, lanTabId);
							wan_cmd_excute(prBuf);
							snprintf(prBuf, sizeof(prBuf), "/usr/bin/ip -6 rule add fwmark %s/0xf0000000 table %s 2>/dev/null", lanMk, lanTabId);
							wan_cmd_excute(prBuf);

							snprintf(prBuf, sizeof(prBuf), "/usr/bin/ip -6 route add blackhole default table %s 2>/dev/null", lanTabId);
							wan_cmd_excute(prBuf);
							
						}
					}
				}
			}
		}
	#endif
	return;
}

#endif

/*______________________________________________________________________________
**	update_connReq_rule
**
**	descriptions:
**		update connection request rule if connection request port changed
**	parameters:
**		top: 		
**		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**_______________________________________________________________________________
*/
extern char connReq_old_port[10];
void set_preService_rule(mxml_node_t *top, char *if_name, int if_index, int state)
{
	char cwmp_node[MAX_ARG_NUM][MAX_NODE_NAME], wan_node[MAX_ARG_NUM][MAX_NODE_NAME];
	char connReq_port[10], svc_list[SERVICE_LIST_LEN];
	char cmd_buf[128];
	int pvc_index, entry_index;

	/* get connection request port */
	memset(cwmp_node, 0, sizeof(cwmp_node));
	strcpy(cwmp_node[0], "Cwmp");
	strcpy(cwmp_node[1], "Entry");

	memset(connReq_port, 0, sizeof(connReq_port));
	if (getAttrValue(top, cwmp_node, "conReqPort", connReq_port) != TCAPI_PROCESS_OK) {
		strcpy(connReq_port, "7547");
	}
	/* use port by tr69 spec */
	if (connReq_port[0] == '\0') {
		strcpy(connReq_port, "7547");
	}

	/* set iptables rule to let connection request packet come in */
	memset(wan_node, 0, sizeof(wan_node));
	strcpy(wan_node[0], "Wan");
	
	pvc_index = if_index / MAX_SMUX_NUM;
	entry_index = if_index - pvc_index * MAX_SMUX_NUM;

	sprintf(wan_node[1], "PVC%d", pvc_index);
	sprintf(wan_node[2], "Entry%d", entry_index);

#if 0
	switch (state) {
		case IF_STATE_UP:
			/* 1. rules for icmp */
			sprintf(cmd_buf, "iptables -t nat -D PRE_SERVICE -i %s -p icmp -j ACCEPT 2>/dev/null", if_name);
			system(cmd_buf);
			sprintf(cmd_buf, "iptables -t nat -A PRE_SERVICE -i %s -p icmp -j ACCEPT", if_name);
			system(cmd_buf);

			
			/* 2. rules for dhcp */
			sprintf(cmd_buf, "iptables -t nat -D PRE_SERVICE -i %s -p udp --dport 67:68 -j ACCEPT 2>/dev/null", if_name);
			system(cmd_buf);
			sprintf(cmd_buf, "iptables -t nat -A PRE_SERVICE -i %s -p udp --dport 67:68 -j ACCEPT", if_name);
			system(cmd_buf);

		
			/* 3. rules for web service */
			sprintf(cmd_buf, "iptables -t nat -D PRE_SERVICE -i %s -p tcp --dport 80 -j ACCEPT 2>/dev/null", if_name);
			system(cmd_buf);
			sprintf(cmd_buf, "iptables -t nat -A PRE_SERVICE -i %s -p tcp --dport 80 -j ACCEPT", if_name);
			system(cmd_buf);

			/* 4. rules for telnet */
			sprintf(cmd_buf, "iptables -t nat -D PRE_SERVICE -i %s -p tcp --dport 23 -j ACCEPT 2>/dev/null", if_name);
			system(cmd_buf);
			sprintf(cmd_buf, "iptables -t nat -A PRE_SERVICE -i %s -p tcp --dport 23 -j ACCEPT", if_name);
			system(cmd_buf);
		
			break;
		case IF_STATE_DOWN:
			/* 1. rules for icmp */
			sprintf(cmd_buf, "iptables -t nat -D PRE_SERVICE -i %s -p icmp -j ACCEPT 2>/dev/null", if_name);
			system(cmd_buf);

			
			/* 2. rules for dhcp */
			sprintf(cmd_buf, "iptables -t nat -D PRE_SERVICE -i %s -p udp --dport 67:68 -j ACCEPT 2>/dev/null", if_name);
			system(cmd_buf);

			/* 3. rules for web service */
			sprintf(cmd_buf, "iptables -t nat -D PRE_SERVICE -i %s -p tcp --dport 80 -j ACCEPT 2>/dev/null", if_name);
			system(cmd_buf);

			/* 4. rules for telnet */
			sprintf(cmd_buf, "iptables -t nat -D PRE_SERVICE -i %s -p tcp --dport 23 -j ACCEPT 2>/dev/null", if_name);
			system(cmd_buf);
		
			break;
		default:
			break;
	}
#endif
	
	/* check service list only for TR069 and TR069_INTERNET */
	memset(svc_list, 0, sizeof(svc_list));
	if (getAttrValue(top, wan_node, "ServiceList", svc_list) == TCAPI_PROCESS_OK) {
#if defined(TCSUPPORT_CT_SERVICELIST_E8C)
		if (strstr(svc_list, "TR069") == NULL) {
#else
		if (0 != strcmp(svc_list, "TR069") && 0 != strcmp(svc_list, "TR069_INTERNET")) {
#endif
			/* only for tr069 and tr069_internet */
			return;
		}
	}
	else {
		return;
	}

	/* 5. rules for  connection request */
	switch (state) {
		case IF_STATE_UP:
			sprintf(cmd_buf, "iptables -t nat -D PRE_SERVICE -i %s -p tcp --dport %s -j ACCEPT 2>/dev/null", if_name, connReq_port);
			system(cmd_buf);
			sprintf(cmd_buf, "iptables -t nat -A PRE_SERVICE -i %s -p tcp --dport %s -j ACCEPT", if_name, connReq_port);
			system(cmd_buf);
			break;
		case IF_STATE_DOWN:
			sprintf(cmd_buf, "iptables -t nat -D PRE_SERVICE -i %s -p tcp --dport %s -j ACCEPT 2>/dev/null", if_name, connReq_port);
			system(cmd_buf);
			break;
		default:
			break;
	}

#ifdef CWMP
	/* update connection request port */
	strcpy(connReq_old_port, connReq_port);
#endif
	return;
	
}

#if defined(TCSUPPORT_CT_WAN_PTM) || defined(TCSUPPORT_CT_E8B_ADSL)
/*______________________________________________________________________________
**	isPVCFullviaTransMode
**
**	descriptions:
**		Check PVC whether full via transfer mode.
**	parameters:
**		
**	return:
**		1: PVC is full by transmode
**_______________________________________________________________________________
*/
int isPVCFullviaTransMode(int transmode, int *full_errCode)
{
	int pvc_index;
	int num = 0;
	int maxcnt = 0;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char str_atmenable[4] = {0};
	char str_ptmenable[4] = {0};
	mxml_node_t *top = get_node_top();

	if ( NULL == top || NULL == full_errCode )
		return -1;

	switch ( transmode )
	{
		case 1 : //ATM
			maxcnt = ATM_PVC_MAXCNT;
			*full_errCode = ERROR_ATM_PVC_FULL;
			break;
		case 2 : // PTM
			maxcnt = PTM_PVC_MAXCNT;
			*full_errCode = ERROR_PTM_PVC_FULL;
			break;
		default:
			return -1;
	}

	for( pvc_index = 0; pvc_index < PVC_NUM; pvc_index++ )
	{
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0],WAN);
		sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);

#if defined(TCSUPPORT_CT_E8B_ADSL)
		if ( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, "Active", str_atmenable) )
			num++;
#else
		if ( 1 == transmode )
		{
			if ( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, WAN_IF_ATMENABLE, str_atmenable)
				&& 0 == strcmp("Yes", str_atmenable) )
				num++;
		}
		else if ( 2 == transmode )
		{
			if ( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, WAN_IF_PTMENABLE, str_ptmenable)
				&& 0 == strcmp("Yes", str_ptmenable) )
				num++;
		}
#endif
	}

	if ( num >= maxcnt )
		return 1;

	*full_errCode = 0;
	return 0;
}
#endif

#if defined(TCSUPPORT_CT_VLAN_BIND)
#if 0
int gVbindDbg = 1;
#else
int gVbindDbg = 0;
#endif

char *vlanbind_adv_table[MAX_NODE_NAME]=
{
	"",
};
char *vlanbind_merge_table[MAX_NODE_NAME]=
{
	"VlanBind_id","",
};
int isVlanBindActive(mxml_node_t *top, int index){
	int control_on=0;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char control_status[5]={0};

	if(index<0 || index>=MAX_LAN_PORT_NUM){
		tcdbg_printf("isVlanBindActive LAN port index error\n");
		return -1;
	}
		
	/*flush the buffer*/
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], VLAN_BIND);
	sprintf(nodeName[1], "%s%d", VLAN_BIND_ENTRY, index);

	/*check the Active value*/
	if(getAttrValue(top, nodeName, "Active", control_status) != TCAPI_PROCESS_OK){
		fprintf(stderr,"Get VlanBind Active error in isVlanBindActive\n");
	}
	/*see if control's attribute(Active) = Yes*/
	else if( (!strcmp(control_status,"Yes")) || (!strcmp(control_status,"yes"))){
		control_on=1;
	}
	return control_on;
}
int getVlanMap(mxml_node_t *top, int index, vlan_map_t* vlanMap){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
//	vlan_map_t vlanMap = {0,0};
	char control_status[5]={0};
	

	if(index<0 || index>=MAX_LAN_PORT_NUM){
		tcdbg_printf("getVlanMap LAN port index error\n");
		return -1;
	}
		
	/*flush the buffer*/
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], VLAN_BIND);
	sprintf(nodeName[1], "%s%d", VLAN_BIND_ENTRY, index);

	return 0;
}


/*______________________________________________________________________________
**	vlanbind_boot
**
**	descriptions:
**		the boot function of vlanbind.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
vlanbind_boot(mxml_node_t *top){
//check if vlan bind switch is on or off
	chmod(VLAN_BIND_SH, 777); 
	system(VLAN_BIND_SH);

	return SUCCESS;
}
/*______________________________________________________________________________
**	vlanbind_init
**
**	descriptions:
**		the init function of vlanbind.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
vlanbind_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="VlanBind",
			.type=ADV_FLAG|ENTRY_TYPE|MAX_LAN_PORT_NUM,
			.adv_table=vlanbind_adv_table,
			.merge_table=vlanbind_merge_table,
			.cfg_read=NULL,
			.cfg_write=vlanbind_write,
			.cfg_verify=vlanbind_verify,
			.cfg_execute=vlanbind_execute,
			.cfg_boot=vlanbind_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}

/*______________________________________________________________________________
**	vlanbind_write
**
**	descriptions:
**		Find vlanbind element from mxml tree then write vlanbind's system parameter to specify
**		configuration files.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  VlanBind cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
vlanbind_write(mxml_node_t *top, mxml_node_t *parant){
	FILE *fp;
	int i=0, j=0, k=0, isActive=0,isBridgeEntry=0;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	vlan_map_t vlanMap = {0,0};
	char strVlanMap[MAX_MAP_ONE_ENTRY*10]={0};
	char cmdBuf[128]={0};
	char *pStrVlanMap = NULL;
	char delims[] = ";";
	char lanIf[7]={0}, vlanMode[15]={0}, vlanID[10]={0}, buf[4]={0};
	int mark = 0;
	char entry_active[5]={0},entry_wanMode[10]={0};//,entry_active[5]={0};
	unsigned int entryActive = 0;
	int ii =0;


	/*open vlan_bind_rule.sh to write*/
	fp = fopen(VLAN_BIND_SH, "w+");
	if(fp == NULL){
		fprintf(stderr, "vlan_bind_rule.sh file pointer is NULL.\n");
		return FAIL;
	}

	sprintf(cmdBuf, "echo reset > %s\n", VBIND_ENTRY_ARRAY_PATH);
	system(cmdBuf);
	for(i=0; i<MAX_LAN_PORT_NUM; i++) 
	{
		if(gVbindDbg)
			tcdbg_printf("vlanbind_write:LAN port i=%d\n",i);
		isActive = isVlanBindActive(top, i);
		if (isActive>0){
			entryActive |= (1<<i);
			sprintf(cmdBuf,"%s -D BROUTING -j vbind_entry%d_chain\n", EBTABLES_BROUTE, i);
			fputs(cmdBuf, fp); 	
			sprintf(cmdBuf,"%s -X vbind_entry%d_chain\n", EBTABLES_BROUTE, i);
			fputs(cmdBuf, fp);
			sprintf(cmdBuf,"%s -N vbind_entry%d_chain -P RETURN\n", EBTABLES_BROUTE, i);
			fputs(cmdBuf, fp);
			sprintf(cmdBuf,"%s -I BROUTING -j vbind_entry%d_chain\n", EBTABLES_BROUTE, i);
			fputs(cmdBuf, fp); 
			
			pStrVlanMap = strVlanMap;
			memset(strVlanMap, 0, sizeof(strVlanMap));
			memset(nodeName, 0, sizeof(nodeName));			
			strcpy(nodeName[0], VLAN_BIND);
			sprintf(nodeName[1], "%s%d", VLAN_BIND_ENTRY, i);
			
			if(getAttrValue(top, nodeName, "mn", strVlanMap) != TCAPI_PROCESS_OK){
				tcdbg_printf("vlanbind_write:get mn failed\n");
				goto ERROR;
			}
			pStrVlanMap = strtok(strVlanMap, delims);
			ii = 0; //Reset vlan group index;
			while(pStrVlanMap != NULL){
				if(gVbindDbg)
					tcdbg_printf("pStrVlanMap is %s \n", pStrVlanMap);
				sscanf(pStrVlanMap, "%d/%d", &vlanMap.m, &vlanMap.n);
				if(gVbindDbg)
					tcdbg_printf("m is %d, n is %d \n", vlanMap.m, vlanMap.n);
				if(vlanMap.m<1 || vlanMap.m>4094 || vlanMap.n<1 || vlanMap.n>4094){
					tcdbg_printf("vlanbind_write:mn value error.\n");
					goto ERROR;
				}
				sprintf(cmdBuf, "echo %d %d %d %d > %s\n", i, ii, vlanMap.m, vlanMap.n, VBIND_ENTRY_ARRAY_PATH);
				system(cmdBuf);
				ii ++;				
#if 1				
				if(i<4)
					sprintf(lanIf,"eth0.%d", i+1);
#if defined(TCSUPPORT_WLAN_AC)
				else if ( i >= 10 )
					sprintf(lanIf,"rai%d", i-10);
#endif
				else
					sprintf(lanIf,"ra%d", i-4);
#else
				strcpy(lanIf, "eth0");
#endif
				/*get PVC route wan interface then get mark value.
				   if n is not matched, do not mark the packets.*/
				for(j=0; j<MAX_PVC_NUM; j++) {
					if(gVbindDbg)
						tcdbg_printf("vlanbind_write:PVC j=%d\n",j);
					memset(nodeName, 0, sizeof(nodeName));			
					strcpy(nodeName[0], WAN);
					sprintf(nodeName[1], "%s%d", WAN_PVC, j);
#if !defined(TCSUPPORT_CT_E8B_ADSL)
					if(getAttrValue(top, nodeName, VLAN_MODE, vlanMode) == TCAPI_NONE_NODE ){
						if(gVbindDbg)
							tcdbg_printf("vlanbind_write:no PVC node\n");
						continue;
					}	
					if( strcmp(vlanMode,"TAG") && strcmp(vlanMode,"tag")){
						continue;
					}
					if(getAttrValue(top, nodeName, VLAN_ID, vlanID) != TCAPI_PROCESS_OK ){
						tcdbg_printf("vlanbind_write:get VLANID failed\n");
						goto ERROR;
					}	
					if( vlanMap.n != atoi(vlanID)){
						continue;
					}
#endif
					/* Here got the matched PVC according to the VLAN ID.*/
					isBridgeEntry = 0;
					for(k=0; k<MAX_PVC_ENTRY; k++) {
						if(gVbindDbg)
							tcdbg_printf("vlanbind_write:PVC_ENTRY k=%d\n",k);
						sprintf(nodeName[2], "%s%d", WAN_INTERFACE, k);
#if defined(TCSUPPORT_CT_E8B_ADSL)
						if ( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "dot1q", vlanMode)
							|| 0 != strcmp("Yes", vlanMode) )
						{
							continue;
						}	

						if ( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, VLAN_ID, vlanID) )
						{
							tcdbg_printf("vlanbind_write:get VLANID failed\n");
							goto ERROR;
						}
						if( vlanMap.n != atoi(vlanID)){
							continue;
						}									
#endif
						/*If there is no active route entry, use the first interface id to mark the packets.*/
						if(getAttrValue(top, nodeName, "Active", entry_active) == TCAPI_NONE_NODE ){
							if(gVbindDbg)
								tcdbg_printf("vlanbind_write:have no PVC_ENTRY node\n");
							continue;
						}	
						if(getAttrValue(top, nodeName, "WanMode", entry_wanMode) != TCAPI_PROCESS_OK ){
							tcdbg_printf("vlanbind_write:get entry active failed\n");
							goto ERROR;
						}	
						if( !strcmp(entry_wanMode,"Route") || !strcmp(entry_wanMode,"route")){
							if( !strcmp(entry_active,"Yes") || !strcmp(entry_active,"yes")){
								isBridgeEntry = 0;
								mark = (j*8 + k + 1) << 16;
								sprintf(cmdBuf,"%s -A vbind_entry%d_chain -i %s -p 802_1Q --vlan-id %d -j mark --mark-or 0x%x --mark-target CONTINUE\n", 
									EBTABLES_BROUTE, i, lanIf, vlanMap.m, mark);
								fputs(cmdBuf, fp);
								break;	
							}
						}
						else {
							if( !strcmp(entry_active,"Yes") || !strcmp(entry_active,"yes")){
								isBridgeEntry = 1;
#if defined(TCSUPPORT_CT_E8B_ADSL)
								mark = (j*8 + 1) << 16;
								sprintf(cmdBuf,"%s -A vbind_entry%d_chain -i %s -p 802_1Q --vlan-id %d -j mark --mark-or 0x%x --mark-target CONTINUE\n", 
									EBTABLES_BROUTE, i, lanIf, vlanMap.m, mark);
								fputs(cmdBuf, fp);
								break;
#endif
							}
						}
					}
					if((k == MAX_PVC_ENTRY)) {
						if ( isBridgeEntry )
						{
						mark = (j*8 + 1) << 16;
						sprintf(cmdBuf,"%s -A vbind_entry%d_chain -i %s -p 802_1Q --vlan-id %d -j mark --mark-or 0x%x --mark-target CONTINUE\n", 
							EBTABLES_BROUTE, i, lanIf, vlanMap.m, mark);
						fputs(cmdBuf, fp);
					}					
						else
							continue;
					}					
					/*Already got the PVC according VLAN ID, no need to check the rest of them.*/
					break;
				}
				pStrVlanMap = strtok(NULL, delims);
			}
		}
		else {
			entryActive &= ~(1<<i);
			/*eg:Change vlan bind to port bind, we should clean the ebtables entries.*/
			sprintf(cmdBuf,"%s -D BROUTING -j vbind_entry%d_chain\n", EBTABLES_BROUTE, i);
			fputs(cmdBuf, fp); 	
			sprintf(cmdBuf,"%s -X vbind_entry%d_chain\n", EBTABLES_BROUTE, i);
			fputs(cmdBuf, fp);
		}
		
	}
	sprintf(cmdBuf, "echo %d > %s\n", entryActive, VBIND_ENTRY_ACTIVE_PATH);
	system(cmdBuf);
	if(gVbindDbg)
		tcdbg_printf("vlanbind_write:entryActive=0x%x\n",entryActive);			
	fclose(fp);
	set_portbinding_info(top, NULL, 0, IF_STATE_UP);

/* add by xyzhu to fix dhcp dial fail from lan when vlan bind enable */
	/* update dhcp relay rule */
	for (i = 0; i < PVC_NUM; i++) 
		for (j = 0; j < MAX_SMUX_NUM; j++) {
			set_DHCPRealy_info(top, i, j, DEL_DHCPREALY_CMD);
#if defined(TCSUPPORT_CT_DHCP_PORT_FILTER)
			set_DHCP_PortFilter_info(top, i, j, DEL_DHCP_PORTFILTER_CMD);
#endif
			set_DHCPRealy_info(top, i, j, ADD_DHCPREALY_CMD);
#if defined(TCSUPPORT_CT_DHCP_PORT_FILTER)
			set_DHCP_PortFilter_info(top, i, j, ADD_DHCP_PORTFILTER_CMD);
#endif
		}

	return SUCCESS;

ERROR:
	fclose(fp);
	return FAIL;
}/* end vlanbind_write */

/*______________________________________________________________________________
**	vlanbind_verify
**
**	descriptions:
**		To check the system parameter of vlanbind element  is validly or not.
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
vlanbind_verify(mxml_node_t *node){
	return SUCCESS;
}/* end vlanbind_verify */

/*_____________________________________________________________________________
**      function name: vlanbind_execute
**      descriptions:
**            To execute vlanbind node service.
**
**      parameters:
**            node:   Put the vlanbind of cfg node tree that is used to execute vlanbind settings.
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
**     
**____________________________________________________________________________
*/
int
vlanbind_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	chmod(VLAN_BIND_SH, 777); 
	system(VLAN_BIND_SH);
#if defined(TCSUPPORT_CT_DHCP_PORT_FILTER)
	isNeedUpdatePortFilter = 1;
#endif
	return SUCCESS;
}/* end vlanbind_execute */
#endif
#if defined(TCSUPPORT_CT_DSL_EX)
int
copy_from_VirServer_node()
{
	char src_nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char dst_nodeName[MAX_ARG_NUM][MAX_NODE_NAME];	
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char node[MAX_NODE_NAME];
	char dslMode[8] = {0};
	int i,j,k;
	tcapi_msg_t msg;
	mxml_node_t *parentNode = NULL;
	mxml_node_t *top = get_node_top();

	//check ATM or PTM	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],SYS_NODE);	
	strcpy(nodeName[1],ENTRY); 			
	getAttrValue(top, nodeName, WAN_DSLMODE, dslMode);
	if(strcmp(dslMode,"ATM") == 0){	
		strcpy(node,VIRSERVER_ATM);		
	}else{		//PTM
		strcpy(node,VIRSERVER_PTM); 	
	}

	//clear VirServerATM or VirSerVerPTM
	memset(dst_nodeName,0,sizeof(dst_nodeName));
	strcpy(dst_nodeName[0],node);
	for(i = 0 ; i < PVC_NUM*PVC_ENTRY_NUM; i++){
		memset(&msg, 0, sizeof(msg));
		sprintf(msg.node, "%s_%s%d",node,SUB_NODE_NAME,i);	
		tcapi_unset_req(top, &msg);
	}
	
	//copy VirServer to  VirServerATM or VirSerVerPTM	
	for(i = 0 ; i < PVC_NUM; i++){	
		for(j = 0 ; j < PVC_ENTRY_NUM; j++){			
			memset(src_nodeName, 0, sizeof(src_nodeName));		
			strcpy(src_nodeName[0],VIRSERVER);				
			sprintf(src_nodeName[1], "%s%d", SUB_NODE_NAME, i*PVC_ENTRY_NUM+j);
			if(search_node(src_nodeName) == NULL){		//node not exist
				continue;
			}
			
			memset(dst_nodeName, 0, sizeof(dst_nodeName));
			strcpy(dst_nodeName[0],node);	
			parentNode = mxmlFindElement(top, top, dst_nodeName[0],
						NULL, NULL, MXML_DESCEND);
			if(parentNode == NULL){
				tcdbg_printf("copy_to_VirServerATMPTM_node():find %s fail\r\n",node);
				return FAIL;	
			}
			sprintf(dst_nodeName[1], "%s%d", SUB_NODE_NAME, i*PVC_ENTRY_NUM+j);				
			mxmlNewElement(parentNode,	dst_nodeName[1]);
			copy_node(dst_nodeName, src_nodeName);

			
			//Entry*_Entry*
			parentNode = mxmlFindElement(parentNode, parentNode, dst_nodeName[1],
						NULL, NULL, MXML_DESCEND);
			if(parentNode == NULL){
				tcdbg_printf("copy_wan_node():find %s_%s node fail\r\n",dst_nodeName[0],dst_nodeName[1]);
				return FAIL;	
			}
			
			for(k=0 ; k < MAX_VIRSERV_RULE; k++){
				sprintf(src_nodeName[2], "%s%d", SUB_NODE_NAME, k);				
				if(search_node(src_nodeName) == NULL){		//node not exist'
					continue;
				}

				sprintf(dst_nodeName[2], "%s%d", SUB_NODE_NAME, k);				
				mxmlNewElement(parentNode,	dst_nodeName[2]);				
				copy_node(dst_nodeName, src_nodeName);
			}
		}
	}
	return;
}

int
copy_to_VirServer_node()
{
	char src_nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char dst_nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char node[MAX_NODE_NAME];
	char dslMode[8] = {0};
	int i,j,k;
	tcapi_msg_t msg;
	mxml_node_t *parentNode = NULL;
	mxml_node_t *top = get_node_top();

	//clear VirServer
	for(i = 0 ; i < PVC_NUM*PVC_ENTRY_NUM; i++){	
		memset(&msg, 0, sizeof(msg));
		sprintf(msg.node, "%s_%s%d",VIRSERVER,SUB_NODE_NAME,i);	
		tcapi_unset_req(top, &msg);
	}

	//check ATM or PTM	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],SYS_NODE);	
	strcpy(nodeName[1],ENTRY); 			
	getAttrValue(top, nodeName, WAN_DSLMODE, dslMode);
	if(strcmp(dslMode,"ATM") == 0){	
		strcpy(node,VIRSERVER_ATM);		
	}else{		//PTM
		strcpy(node,VIRSERVER_PTM); 	
	}
	
	//copy VirServerATM or VirSerVerPTM	 to VirServer 	
	for(i = 0 ; i < PVC_NUM; i++){
		for(j = 0 ; j < PVC_NUM; j++){			
			memset(src_nodeName, 0, sizeof(src_nodeName));		
			strcpy(src_nodeName[0],node);				
			sprintf(src_nodeName[1], "%s%d", SUB_NODE_NAME, i*PVC_ENTRY_NUM+j);
			if(search_node(src_nodeName) == NULL){		//node not exist
				continue;
			}
			
			memset(dst_nodeName, 0, sizeof(dst_nodeName));
			strcpy(dst_nodeName[0],VIRSERVER);	
			parentNode = mxmlFindElement(top, top, dst_nodeName[0],
						NULL, NULL, MXML_DESCEND);
			if(parentNode == NULL){
				tcdbg_printf("copy_to_VirServerATMPTM_node():find %s fail\r\n",node);
				return FAIL;	
			}
			sprintf(dst_nodeName[1], "%s%d", SUB_NODE_NAME, i*PVC_ENTRY_NUM+j);				
			mxmlNewElement(parentNode,	dst_nodeName[1]);
			copy_node(dst_nodeName, src_nodeName);

			
			//Entry*_Entry*
			parentNode = mxmlFindElement(parentNode, parentNode, dst_nodeName[1],
						NULL, NULL, MXML_DESCEND);
			if(parentNode == NULL){
				tcdbg_printf("copy_wan_node():find %s_%s node fail\r\n",dst_nodeName[0],dst_nodeName[1]);
				return FAIL;	
			}
			
			for(k=0 ; k < MAX_VIRSERV_RULE; k++){
				sprintf(src_nodeName[2], "%s%d", SUB_NODE_NAME, k);				
				if(search_node(src_nodeName) == NULL){		//node not exist
					continue;
				}

				sprintf(dst_nodeName[2], "%s%d", SUB_NODE_NAME, k);				
				mxmlNewElement(parentNode,	dst_nodeName[2]);				
				copy_node(dst_nodeName, src_nodeName);
			}
		}
	}
	return;
}

int copy_from_route_node()
{
	char src_nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char dst_nodeName[MAX_ARG_NUM][MAX_NODE_NAME];	
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char node[MAX_NODE_NAME];
	char dslMode[8] = {0};
	int i=0;
	tcapi_msg_t msg;
	mxml_node_t *parentNode = NULL;
	mxml_node_t *top = get_node_top();

	//check ATM or PTM	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],SYS_NODE);	
	strcpy(nodeName[1],ENTRY); 			
	getAttrValue(top, nodeName, WAN_DSLMODE, dslMode);
	if(strcmp(dslMode,"ATM") == 0){	
		strcpy(node, ROUTE_ATM);		
	}else{		//PTM
		strcpy(node, ROUTE_PTM); 	
	}

	//clear RouteATM or RoutePTM
	for(i = 0 ; i < MAX_STATIC_ROUTE_NUM; i++)
	{
		memset(&msg, 0, sizeof(msg));
		sprintf(msg.node, "%s_%s%d",node,SUB_NODE_NAME,i);	
		tcapi_unset_req(top, &msg);
	}

	//copy Route to RouteATM or RoutePTM
	for(i = 0 ; i < MAX_STATIC_ROUTE_NUM; i++)
	{
		memset(src_nodeName, 0, sizeof(src_nodeName));		
		strcpy(src_nodeName[0],ROUTE);				
		sprintf(src_nodeName[1], "%s%d", SUB_NODE_NAME, i);
		if(search_node(src_nodeName) == NULL)		//node not exist
			continue;

		memset(dst_nodeName, 0, sizeof(dst_nodeName));
		strcpy(dst_nodeName[0],node);	
		parentNode = mxmlFindElement(top, top, dst_nodeName[0],
					NULL, NULL, MXML_DESCEND);
		if(parentNode == NULL){
			tcdbg_printf("copy_from_interfaces_node():find %s fail\r\n",node);
			return FAIL;
		}
		sprintf(dst_nodeName[1], "%s%d", SUB_NODE_NAME, i);				
		mxmlNewElement(parentNode,	dst_nodeName[1]);
		copy_node(dst_nodeName, src_nodeName);
	}

	return SUCCESS;
}

int copy_to_route_node()
{
	char src_nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char dst_nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char node[MAX_NODE_NAME];
	char dslMode[8] = {0};
	int i=0;
	tcapi_msg_t msg;
	mxml_node_t *parentNode = NULL;
	mxml_node_t *top = get_node_top();

	//clear Route
	for(i = 0 ; i < MAX_STATIC_ROUTE_NUM; i++)
	{	
		memset(&msg, 0, sizeof(msg));
		sprintf(msg.node, "%s_%s%d",ROUTE,SUB_NODE_NAME,i);	
		tcapi_unset_req(top, &msg);
	}

	//check ATM or PTM	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],SYS_NODE);	
	strcpy(nodeName[1],ENTRY); 			
	getAttrValue(top, nodeName, WAN_DSLMODE, dslMode);
	if(strcmp(dslMode,"ATM") == 0){	
		strcpy(node,ROUTE_ATM);
	}else{		//PTM
		strcpy(node,ROUTE_PTM);
	}
	
	//copy RouteATM or RoutePTM to Route 	
	for(i = 0 ; i < MAX_STATIC_ROUTE_NUM; i++)
	{
		memset(src_nodeName, 0, sizeof(src_nodeName));		
		strcpy(src_nodeName[0],node);				
		sprintf(src_nodeName[1], "%s%d", SUB_NODE_NAME, i);
		if(search_node(src_nodeName) == NULL){		//node not exist
			continue;
		}
		
		memset(dst_nodeName, 0, sizeof(dst_nodeName));
		strcpy(dst_nodeName[0],ROUTE);	
		parentNode = mxmlFindElement(top, top, dst_nodeName[0],
					NULL, NULL, MXML_DESCEND);
		if(parentNode == NULL){
			tcdbg_printf("copy_to_interfaces_node():find %s fail\r\n",node);
			return FAIL;	
		}
		sprintf(dst_nodeName[1], "%s%d", SUB_NODE_NAME, i);				
		mxmlNewElement(parentNode,	dst_nodeName[1]);
		copy_node(dst_nodeName, src_nodeName);
	}

	return SUCCESS;
}

int copy_from_route6_node()
{
	char src_nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char dst_nodeName[MAX_ARG_NUM][MAX_NODE_NAME];	
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char node[MAX_NODE_NAME];
	char dslMode[8] = {0};
	int i=0;
	tcapi_msg_t msg;
	mxml_node_t *parentNode = NULL;
	mxml_node_t *top = get_node_top();

	//check ATM or PTM	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],SYS_NODE);	
	strcpy(nodeName[1],ENTRY); 			
	getAttrValue(top, nodeName, WAN_DSLMODE, dslMode);
	if(strcmp(dslMode,"ATM") == 0){	
		strcpy(node, ROUTE6_ATM);		
	}else{		//PTM
		strcpy(node, ROUTE6_PTM); 	
	}

	//clear Route6ATM or Route6PTM
	for(i = 0 ; i < MAX_STATIC_ROUTE6_NUM; i++)
	{
		memset(&msg, 0, sizeof(msg));
		sprintf(msg.node, "%s_%s%d",node,SUB_NODE_NAME,i);	
		tcapi_unset_req(top, &msg);
	}

	//copy Route6 to Route6ATM or Route6PTM
	for(i = 0 ; i < MAX_STATIC_ROUTE6_NUM; i++)
	{
		memset(src_nodeName, 0, sizeof(src_nodeName));		
		strcpy(src_nodeName[0],ROUTE6);				
		sprintf(src_nodeName[1], "%s%d", SUB_NODE_NAME, i);
		if(search_node(src_nodeName) == NULL)		//node not exist
			continue;

		memset(dst_nodeName, 0, sizeof(dst_nodeName));
		strcpy(dst_nodeName[0],node);	
		parentNode = mxmlFindElement(top, top, dst_nodeName[0],
					NULL, NULL, MXML_DESCEND);
		if(parentNode == NULL){
			tcdbg_printf("copy_from_interfaces_node():find %s fail\r\n",node);
			return FAIL;
		}
		sprintf(dst_nodeName[1], "%s%d", SUB_NODE_NAME, i);				
		mxmlNewElement(parentNode,	dst_nodeName[1]);
		copy_node(dst_nodeName, src_nodeName);
	}

	return SUCCESS;
}

int copy_to_route6_node()
{
	char src_nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char dst_nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char node[MAX_NODE_NAME];
	char dslMode[8] = {0};
	int i=0;
	tcapi_msg_t msg;
	mxml_node_t *parentNode = NULL;
	mxml_node_t *top = get_node_top();

	//clear Route6
	for(i = 0 ; i < MAX_STATIC_ROUTE6_NUM; i++)
	{	
		memset(&msg, 0, sizeof(msg));
		sprintf(msg.node, "%s_%s%d",ROUTE6,SUB_NODE_NAME,i);	
		tcapi_unset_req(top, &msg);
	}

	//check ATM or PTM	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],SYS_NODE);	
	strcpy(nodeName[1],ENTRY); 			
	getAttrValue(top, nodeName, WAN_DSLMODE, dslMode);
	if(strcmp(dslMode,"ATM") == 0){	
		strcpy(node,ROUTE6_ATM);
	}else{		//PTM
		strcpy(node,ROUTE6_PTM);
	}
	
	//copy Route6ATM or Route6PTM to Route 	
	for(i = 0 ; i < MAX_STATIC_ROUTE6_NUM; i++)
	{
		memset(src_nodeName, 0, sizeof(src_nodeName));		
		strcpy(src_nodeName[0],node);				
		sprintf(src_nodeName[1], "%s%d", SUB_NODE_NAME, i);
		if(search_node(src_nodeName) == NULL){		//node not exist
			continue;
		}
		
		memset(dst_nodeName, 0, sizeof(dst_nodeName));
		strcpy(dst_nodeName[0],ROUTE6);	
		parentNode = mxmlFindElement(top, top, dst_nodeName[0],
					NULL, NULL, MXML_DESCEND);
		if(parentNode == NULL){
			tcdbg_printf("copy_to_interfaces_node():find %s fail\r\n",node);
			return FAIL;	
		}
		sprintf(dst_nodeName[1], "%s%d", SUB_NODE_NAME, i);				
		mxmlNewElement(parentNode,	dst_nodeName[1]);
		copy_node(dst_nodeName, src_nodeName);
	}

	return SUCCESS;
}

int copy_from_dmz_node()
{
	char src_nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char dst_nodeName[MAX_ARG_NUM][MAX_NODE_NAME];	
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char node[MAX_NODE_NAME];
	char dslMode[8] = {0};
	int i=0;
	tcapi_msg_t msg;
	mxml_node_t *parentNode = NULL;
	mxml_node_t *top = get_node_top();

	//check ATM or PTM	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],SYS_NODE);	
	strcpy(nodeName[1],ENTRY); 			
	getAttrValue(top, nodeName, WAN_DSLMODE, dslMode);
	if(strcmp(dslMode,"ATM") == 0){	
		strcpy(node, DMZ_ATM);		
	}else{		//PTM
		strcpy(node, DMZ_PTM); 	
	}

	//clear DmzATM or DmzPTM
	for(i = 0 ; i < MAX_WAN_IF_INDEX; i++)
	{
		memset(&msg, 0, sizeof(msg));
		sprintf(msg.node, "%s_%s%d",node,SUB_NODE_NAME,i);	
		tcapi_unset_req(top, &msg);
	}

	//copy Dmz to DmzATM or DmzPTM
	for(i = 0 ; i < MAX_WAN_IF_INDEX; i++)
	{
		memset(src_nodeName, 0, sizeof(src_nodeName));		
		strcpy(src_nodeName[0],DMZ);				
		sprintf(src_nodeName[1], "%s%d", SUB_NODE_NAME, i);
		if(search_node(src_nodeName) == NULL)		//node not exist
			continue;

		memset(dst_nodeName, 0, sizeof(dst_nodeName));
		strcpy(dst_nodeName[0],node);	
		parentNode = mxmlFindElement(top, top, dst_nodeName[0],
					NULL, NULL, MXML_DESCEND);
		if(parentNode == NULL){
			tcdbg_printf("copy_from_interfaces_node():find %s fail\r\n",node);
			return FAIL;
		}
		sprintf(dst_nodeName[1], "%s%d", SUB_NODE_NAME, i);				
		mxmlNewElement(parentNode,	dst_nodeName[1]);
		copy_node(dst_nodeName, src_nodeName);
	}

	return SUCCESS;
}

int copy_to_dmz_node()
{
	char src_nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char dst_nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char node[MAX_NODE_NAME];
	char dslMode[8] = {0};
	int i=0;
	tcapi_msg_t msg;
	mxml_node_t *parentNode = NULL;
	mxml_node_t *top = get_node_top();

	//clear Dmz
	for(i = 0 ; i < MAX_WAN_IF_INDEX; i++)
	{	
		memset(&msg, 0, sizeof(msg));
		sprintf(msg.node, "%s_%s%d",DMZ,SUB_NODE_NAME,i);	
		tcapi_unset_req(top, &msg);
	}

	//check ATM or PTM	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],SYS_NODE);	
	strcpy(nodeName[1],ENTRY); 			
	getAttrValue(top, nodeName, WAN_DSLMODE, dslMode);
	if(strcmp(dslMode,"ATM") == 0){	
		strcpy(node,DMZ_ATM);
	}else{		//PTM
		strcpy(node,DMZ_PTM);
	}
	
	//copy DmzATM or DmzPTM to Dmz 	
	for(i = 0 ; i < MAX_WAN_IF_INDEX; i++)
	{
		memset(src_nodeName, 0, sizeof(src_nodeName));		
		strcpy(src_nodeName[0],node);				
		sprintf(src_nodeName[1], "%s%d", SUB_NODE_NAME, i);
		if(search_node(src_nodeName) == NULL){		//node not exist
			continue;
		}
		
		memset(dst_nodeName, 0, sizeof(dst_nodeName));
		strcpy(dst_nodeName[0],DMZ);	
		parentNode = mxmlFindElement(top, top, dst_nodeName[0],
					NULL, NULL, MXML_DESCEND);
		if(parentNode == NULL){
			tcdbg_printf("copy_to_interfaces_node():find %s fail\r\n",node);
			return FAIL;	
		}
		sprintf(dst_nodeName[1], "%s%d", SUB_NODE_NAME, i);				
		mxmlNewElement(parentNode,	dst_nodeName[1]);
		copy_node(dst_nodeName, src_nodeName);
	}

	return SUCCESS;
}

int copy_from_igmpproxy_node()
{
	char src_nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char dst_nodeName[MAX_ARG_NUM][MAX_NODE_NAME];	
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char node[MAX_NODE_NAME];
	char dslMode[8] = {0};
	tcapi_msg_t msg;
	mxml_node_t *parentNode = NULL;
	mxml_node_t *top = get_node_top();

	//check ATM or PTM	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],SYS_NODE);	
	strcpy(nodeName[1],ENTRY); 			
	getAttrValue(top, nodeName, WAN_DSLMODE, dslMode);
	if(strcmp(dslMode,"ATM") == 0){	
		strcpy(node, IGMPPROXY_ATM);		
	}else{		//PTM
		strcpy(node, IGMPPROXY_PTM); 	
	}

	//clear IGMPproxyATM or IGMPproxyPTM
	memset(&msg, 0, sizeof(msg));
	sprintf(msg.node, "%s_%s",node,SUB_NODE_NAME);	
	tcapi_unset_req(top, &msg);


	//copy IGMPproxy to IGMPproxyATM or IGMPproxyPTM
	memset(src_nodeName, 0, sizeof(src_nodeName));		
	strcpy(src_nodeName[0], "IGMPproxy");			
	sprintf(src_nodeName[1], "%s", SUB_NODE_NAME);
	if(search_node(src_nodeName) == NULL)		//node not exist
		return FAIL;

	memset(dst_nodeName, 0, sizeof(dst_nodeName));
	strcpy(dst_nodeName[0], node);	
	parentNode = mxmlFindElement(top, top, dst_nodeName[0],
				NULL, NULL, MXML_DESCEND);
	if(parentNode == NULL){
		tcdbg_printf("copy_from_igmpproxy_node():find %s fail\r\n",node);
		return FAIL;
	}
	sprintf(dst_nodeName[1], "%s", SUB_NODE_NAME); 			
	mxmlNewElement(parentNode,	dst_nodeName[1]);
	copy_node(dst_nodeName, src_nodeName);

	return SUCCESS;
}

int copy_to_igmpproxy_node()
{
	char src_nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char dst_nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char node[MAX_NODE_NAME];
	char dslMode[8] = {0};
	tcapi_msg_t msg;
	mxml_node_t *parentNode = NULL;
	mxml_node_t *top = get_node_top();

	//clear Route
	memset(&msg, 0, sizeof(msg));
	sprintf(msg.node, "IGMPproxy_%s",SUB_NODE_NAME); 
	tcapi_unset_req(top, &msg);

	//check ATM or PTM	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],SYS_NODE);	
	strcpy(nodeName[1],ENTRY); 			
	getAttrValue(top, nodeName, WAN_DSLMODE, dslMode);
	if(strcmp(dslMode,"ATM") == 0){	
		strcpy(node,IGMPPROXY_ATM);
	}else{		//PTM
		strcpy(node,IGMPPROXY_PTM);
	}
	
	//copy IGMPproxyATM or IGMPproxyPTM to IGMPproxy
	memset(src_nodeName, 0, sizeof(src_nodeName));		
	strcpy(src_nodeName[0],node);				
	sprintf(src_nodeName[1], "%s", SUB_NODE_NAME);
	if(search_node(src_nodeName) == NULL)		//node not exist
		return FAIL;

	memset(dst_nodeName, 0, sizeof(dst_nodeName));
	strcpy(dst_nodeName[0], "IGMPproxy");	
	parentNode = mxmlFindElement(top, top, dst_nodeName[0],
				NULL, NULL, MXML_DESCEND);
	if(parentNode == NULL){
		tcdbg_printf("copy_to_igmpproxy_node():find %s fail\r\n",node);
		return FAIL;	
	}
	sprintf(dst_nodeName[1], "%s", SUB_NODE_NAME); 			
	mxmlNewElement(parentNode,	dst_nodeName[1]);
	copy_node(dst_nodeName, src_nodeName);

	return SUCCESS;
}

int copy_from_mldproxy_node()
{
	char src_nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char dst_nodeName[MAX_ARG_NUM][MAX_NODE_NAME];	
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char node[MAX_NODE_NAME];
	char dslMode[8] = {0};
	tcapi_msg_t msg;
	mxml_node_t *parentNode = NULL;
	mxml_node_t *top = get_node_top();

	//check ATM or PTM	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],SYS_NODE);	
	strcpy(nodeName[1],ENTRY); 			
	getAttrValue(top, nodeName, WAN_DSLMODE, dslMode);
	if(strcmp(dslMode,"ATM") == 0){	
		strcpy(node, MLDPROXY_ATM);		
	}else{		//PTM
		strcpy(node, MLDPROXY_PTM); 	
	}

	//clear MLDproxyATM or MLDproxyPTM
	memset(&msg, 0, sizeof(msg));
	sprintf(msg.node, "%s_%s",node,SUB_NODE_NAME);	
	tcapi_unset_req(top, &msg);


	//copy MLDproxy to MLDproxyATM or MLDproxyPTM
	memset(src_nodeName, 0, sizeof(src_nodeName));		
	strcpy(src_nodeName[0], "MLDproxy");			
	sprintf(src_nodeName[1], "%s", SUB_NODE_NAME);
	if(search_node(src_nodeName) == NULL)		//node not exist
		return FAIL;

	memset(dst_nodeName, 0, sizeof(dst_nodeName));
	strcpy(dst_nodeName[0], node);	
	parentNode = mxmlFindElement(top, top, dst_nodeName[0],
				NULL, NULL, MXML_DESCEND);
	if(parentNode == NULL){
		tcdbg_printf("copy_from_mldproxy_node():find %s fail\r\n",node);
		return FAIL;
	}
	sprintf(dst_nodeName[1], "%s", SUB_NODE_NAME); 			
	mxmlNewElement(parentNode,	dst_nodeName[1]);
	copy_node(dst_nodeName, src_nodeName);

	return SUCCESS;
}

int copy_to_mldproxy_node()
{
	char src_nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char dst_nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char node[MAX_NODE_NAME];
	char dslMode[8] = {0};
	tcapi_msg_t msg;
	mxml_node_t *parentNode = NULL;
	mxml_node_t *top = get_node_top();

	//clear Route
	memset(&msg, 0, sizeof(msg));
	sprintf(msg.node, "MLDproxy_%s",SUB_NODE_NAME); 
	tcapi_unset_req(top, &msg);

	//check ATM or PTM	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],SYS_NODE);	
	strcpy(nodeName[1],ENTRY); 			
	getAttrValue(top, nodeName, WAN_DSLMODE, dslMode);
	if(strcmp(dslMode,"ATM") == 0){	
		strcpy(node,MLDPROXY_ATM);
	}else{		//PTM
		strcpy(node,MLDPROXY_PTM);
	}
	
	//copy MLDproxyATM or MLDproxyPTM to MLDproxy
	memset(src_nodeName, 0, sizeof(src_nodeName));		
	strcpy(src_nodeName[0],node);				
	sprintf(src_nodeName[1], "%s", SUB_NODE_NAME);
	if(search_node(src_nodeName) == NULL)		//node not exist
		return FAIL;

	memset(dst_nodeName, 0, sizeof(dst_nodeName));
	strcpy(dst_nodeName[0], "MLDproxy");	
	parentNode = mxmlFindElement(top, top, dst_nodeName[0],
				NULL, NULL, MXML_DESCEND);
	if(parentNode == NULL){
		tcdbg_printf("copy_to_mldproxy_node():find %s fail\r\n",node);
		return FAIL;	
	}
	sprintf(dst_nodeName[1], "%s", SUB_NODE_NAME); 			
	mxmlNewElement(parentNode,	dst_nodeName[1]);
	copy_node(dst_nodeName, src_nodeName);

	return SUCCESS;
}

int copy_from_ddns_node()
{
#if defined(TCSUPPORT_CT_E8DDNS)
	char src_nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char dst_nodeName[MAX_ARG_NUM][MAX_NODE_NAME];	
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char node[MAX_NODE_NAME];
	char dslMode[8] = {0};
	int i=0;
	tcapi_msg_t msg;
	mxml_node_t *parentNode = NULL;
	mxml_node_t *top = get_node_top();

	//check ATM or PTM	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],SYS_NODE);	
	strcpy(nodeName[1],ENTRY); 			
	getAttrValue(top, nodeName, WAN_DSLMODE, dslMode);
	if(strcmp(dslMode,"ATM") == 0){	
		strcpy(node, DDNS_ATM);		
	}else{		//PTM
		strcpy(node, DDNS_PTM); 	
	}

	//clear DdnsATM or DdnsPTM
	for(i = 0 ; i < MAX_DDNS_NUM; i++)
	{
		memset(&msg, 0, sizeof(msg));
		sprintf(msg.node, "%s_%s%d",node,SUB_NODE_NAME,i);	
		tcapi_unset_req(top, &msg);
	}
	memset(&msg, 0, sizeof(msg));
	sprintf(msg.node, "%s_Common", node);	
	tcapi_unset_req(top, &msg);

	//copy Ddns to DdnsATM or DdnsPTM
	for(i = 0 ; i < MAX_DDNS_NUM; i++)
	{
		memset(src_nodeName, 0, sizeof(src_nodeName));		
		strcpy(src_nodeName[0],DDNS);				
		sprintf(src_nodeName[1], "%s%d", SUB_NODE_NAME, i);
		if(search_node(src_nodeName) == NULL)		//node not exist
			continue;

		memset(dst_nodeName, 0, sizeof(dst_nodeName));
		strcpy(dst_nodeName[0],node);	
		parentNode = mxmlFindElement(top, top, dst_nodeName[0],
					NULL, NULL, MXML_DESCEND);
		if(parentNode == NULL){
			tcdbg_printf("copy_from_ddns_node():find %s fail\r\n",node);
			return FAIL;
		}
		sprintf(dst_nodeName[1], "%s%d", SUB_NODE_NAME, i);				
		mxmlNewElement(parentNode,	dst_nodeName[1]);
		copy_node(dst_nodeName, src_nodeName);
	}

	memset(src_nodeName, 0, sizeof(src_nodeName));		
	strcpy(src_nodeName[0],DDNS);
	strcpy(src_nodeName[1], "Common");
	if(search_node(src_nodeName) == NULL)		//node not exist
		return FAIL;

	memset(dst_nodeName, 0, sizeof(dst_nodeName));
	strcpy(dst_nodeName[0], node);	
	parentNode = mxmlFindElement(top, top, dst_nodeName[0],
				NULL, NULL, MXML_DESCEND);
	if(parentNode == NULL){
		tcdbg_printf("copy_from_ddns_node():find %s fail\r\n",node);
		return FAIL;
	}
	strcpy(dst_nodeName[1], "Common"); 			
	mxmlNewElement(parentNode,	dst_nodeName[1]);
	copy_node(dst_nodeName, src_nodeName);

#endif
	return SUCCESS;
}

int copy_to_ddns_node()
{
#if defined(TCSUPPORT_CT_E8DDNS)
	char src_nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char dst_nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char node[MAX_NODE_NAME];
	char dslMode[8] = {0};
	int i=0;
	tcapi_msg_t msg;
	mxml_node_t *parentNode = NULL;
	mxml_node_t *top = get_node_top();

	//clear Ddns
	for(i = 0 ; i < MAX_DDNS_NUM; i++)
	{
		memset(&msg, 0, sizeof(msg));
		sprintf(msg.node, "%s_%s%d",DDNS,SUB_NODE_NAME,i);	
		tcapi_unset_req(top, &msg);
	}
	memset(&msg, 0, sizeof(msg));
	sprintf(msg.node, "%s_Common", DDNS);	
	tcapi_unset_req(top, &msg);

	//check ATM or PTM
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],SYS_NODE);	
	strcpy(nodeName[1],ENTRY); 			
	getAttrValue(top, nodeName, WAN_DSLMODE, dslMode);
	if(strcmp(dslMode,"ATM") == 0){	
		strcpy(node,DDNS_ATM);
	}else{		//PTM
		strcpy(node,DDNS_PTM);
	}
	
	//copy DdnsATM or DdnsPTM to Ddns 	
	for(i = 0 ; i < MAX_DDNS_NUM; i++)
	{
		memset(src_nodeName, 0, sizeof(src_nodeName));		
		strcpy(src_nodeName[0],node);				
		sprintf(src_nodeName[1], "%s%d", SUB_NODE_NAME, i);
		if(search_node(src_nodeName) == NULL){		//node not exist
			continue;
		}
		
		memset(dst_nodeName, 0, sizeof(dst_nodeName));
		strcpy(dst_nodeName[0],DDNS);	
		parentNode = mxmlFindElement(top, top, dst_nodeName[0],
					NULL, NULL, MXML_DESCEND);
		if(parentNode == NULL){
			tcdbg_printf("copy_to_ddns_node():find %s fail\r\n",node);
			return FAIL;	
		}
		sprintf(dst_nodeName[1], "%s%d", SUB_NODE_NAME, i);				
		mxmlNewElement(parentNode,	dst_nodeName[1]);
		copy_node(dst_nodeName, src_nodeName);
	}

	memset(src_nodeName, 0, sizeof(src_nodeName));		
	strcpy(src_nodeName[0], node);				
	strcpy(src_nodeName[1], "Common");
	if(search_node(src_nodeName) == NULL)		//node not exist
		return FAIL;

	memset(dst_nodeName, 0, sizeof(dst_nodeName));
	strcpy(dst_nodeName[0],DDNS);	
	parentNode = mxmlFindElement(top, top, dst_nodeName[0],
				NULL, NULL, MXML_DESCEND);
	if(parentNode == NULL){
		tcdbg_printf("copy_to_ddns_node():find %s fail\r\n",node);
		return FAIL;	
	}
	strcpy(dst_nodeName[1], "Common");
	mxmlNewElement(parentNode,	dst_nodeName[1]);
	copy_node(dst_nodeName, src_nodeName);
#endif

	return SUCCESS;
}


/*_____________________________________________________________________________
**      function name: copy_wan_node
**      descriptions:
**            Copy WanATM or WanPTM to current wan node by dsl status.
**
**      parameters:
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
**      1. lisa.dai 2013/7/22
**____________________________________________________________________________
*/
int
copy_wan_node(int state)
{
	mxml_node_t *top = get_node_top();
	char dst_nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char src_nodeName[MAX_ARG_NUM][MAX_NODE_NAME];	
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];	
	mxml_node_t *parentNode = NULL;
	int i,j;
	int dsl_state = 0;		
	tcapi_msg_t msg;	
	char dslMode[8] = {0};
	char node[MAX_NODE_NAME];
	int mode;

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], SYS_NODE);
	strcpy(nodeName[1], ENTRY);
	getAttrValue(top, nodeName, WAN_DSLMODE, dslMode);
	if ( 1 == state ) // ATM
	{
		if ( 0 == strcmp(dslMode,"ATM") )
			return SUCCESS;
	}
	else if ( 2 == state ) // PTM
	{
		if ( 0 == strcmp(dslMode,"PTM") )
			return SUCCESS;
	}

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],	WEB_NODE);
	strcpy(nodeName[1],	ENTRY);
	setAttrValue(top, nodeName, WAN_LOCKED, "1");
	
	//clear wan pvc
	setAttrValue(top, nodeName, WAN_COPY_FLAG, "1");
	for(i = 0 ; i < PVC_NUM; i++){		
		memset(&msg, 0, sizeof(msg));
		sprintf(msg.node, "%s_%s%d",WAN,WAN_PVC,i);		
		tcapi_unset_req(top, &msg);
	}
	setAttrValue(top, nodeName, WAN_COPY_FLAG, "0");

	//clear wan common
	memset(&msg, 0, sizeof(msg));
	sprintf(msg.node, "%s_%s",WAN,COMMON); 	
	tcapi_unset_req(top, &msg);
	
	//check: WanATM/WanPTM
	dsl_state = get_dsl_state();
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],SYS_NODE);	
	strcpy(nodeName[1],ENTRY); 			
	if(dsl_state == 1) {	//ADSL	
		strcpy(node,WAN_ATM);		
		setAttrValue(top, nodeName, WAN_DSLMODE, "ATM");
	}else if(dsl_state == 2) {	//VDSL
		strcpy(node,WAN_PTM);	
		setAttrValue(top, nodeName, WAN_DSLMODE, "PTM");
	}else{	//down			
		getAttrValue(top, nodeName, WAN_DSLMODE, dslMode);
		if(strcmp(dslMode,"ATM") == 0){
			strcpy(node,WAN_ATM);		
		}else{		
			strcpy(node,WAN_PTM);	
		}
	}

	//copy wan common node	
	memset(src_nodeName, 0, sizeof(src_nodeName));		
	strcpy(src_nodeName[0],node);	
	strcpy(src_nodeName[1],COMMON);				
	if(search_node(src_nodeName)){		//node  exist
		memset(dst_nodeName, 0, sizeof(dst_nodeName));
		strcpy(dst_nodeName[0],WAN);	
		strcpy(dst_nodeName[1],COMMON);		
		parentNode = mxmlFindElement(top, top, dst_nodeName[0],
				NULL, NULL, MXML_DESCEND);
		if(parentNode == NULL){
			tcdbg_printf("copy_wan_node():find wan node fail\r\n");
			return FAIL;	
		}
		mxmlNewElement(parentNode,	dst_nodeName[1]);
		copy_node(dst_nodeName, src_nodeName);		
	}

	//copy wan pvc node
	for(i = 0 ; i < PVC_NUM; i++){
		memset(src_nodeName, 0, sizeof(src_nodeName));		
		strcpy(src_nodeName[0],node);				
		sprintf(src_nodeName[1], "%s%d", WAN_PVC, i);
		if(search_node(src_nodeName) == NULL){		//node not exist
			continue;
		}

		memset(dst_nodeName, 0, sizeof(dst_nodeName));
		strcpy(dst_nodeName[0],WAN);	
		parentNode = mxmlFindElement(top, top, dst_nodeName[0],
					NULL, NULL, MXML_DESCEND);
		if(parentNode == NULL){
			tcdbg_printf("copy_wan_node():find wan node fail\r\n");
			return FAIL;	
		}
		sprintf(dst_nodeName[1], "%s%d", WAN_PVC, i);				
		mxmlNewElement(parentNode,	dst_nodeName[1]);
		copy_node(dst_nodeName, src_nodeName);


		//Entry
		parentNode = mxmlFindElement(parentNode, parentNode, dst_nodeName[1],
					NULL, NULL, MXML_DESCEND);
		if(parentNode == NULL){
			tcdbg_printf("copy_wan_node():find wan_pvc node fail\r\n");
			return FAIL;	
		}
		
		for(j=0 ; j < PVC_ENTRY_NUM; j++){
			sprintf(src_nodeName[2], "%s%d", WAN_INTERFACE, j);				
			if(search_node(src_nodeName) == NULL){		//node not exist
				continue;
			}

			sprintf(dst_nodeName[2], "%s%d", WAN_INTERFACE, j);				
			mxmlNewElement(parentNode,	dst_nodeName[2]);				
			copy_node(dst_nodeName, src_nodeName);
		}
	}

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],	WEB_NODE);
	strcpy(nodeName[1],	ENTRY);
	setAttrValue(top, nodeName, WAN_LOCKED, "0");

	copy_to_VirServer_node();
	copy_to_route_node();
	copy_to_route6_node();
	copy_to_dmz_node();
	copy_to_igmpproxy_node();
	copy_to_mldproxy_node();
	copy_to_ddns_node();
	return SUCCESS;
}

/*_____________________________________________________________________________
**      function name: update_wan_node
**      descriptions:
**            Copy wan node to WanATM or WanPTM  dsl status.
**
**      parameters:
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
**      1. lisa.dai 2013/7/22
**____________________________________________________________________________
*/

int
update_wan_node()
{
	mxml_node_t *top = get_node_top();
	char dst_nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char src_nodeName[MAX_ARG_NUM][MAX_NODE_NAME];	
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];	
	mxml_node_t *parentNode = NULL;
	int i,j;
	int dsl_state = 0, mode;		
	tcapi_msg_t msg;		
	char dslMode[8] = {0};
	char node[MAX_NODE_NAME];

	//check: WanATM/WanPTM
	dsl_state = get_dsl_state();
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],SYS_NODE);	
	strcpy(nodeName[1],ENTRY); 			
	if(dsl_state == 1) {	//ADSL	
		strcpy(node,WAN_ATM);		
	}else if(dsl_state == 2) {	//VDSL
		strcpy(node,WAN_PTM);	
	}else{	//down		
		getAttrValue(top, nodeName, WAN_DSLMODE, dslMode);
		if(strcmp(dslMode,"ATM") == 0){
			strcpy(node,WAN_ATM);		
		}else{		
			strcpy(node,WAN_PTM);	
		}
	}
	
	//clean pvc node before backup
	for(i = 0 ; i < PVC_NUM; i++){		
		memset(&msg, 0, sizeof(msg));		
		sprintf(msg.node, "%s_%s%d",node,WAN_PVC,i);			
		tcapi_unset_req(top, &msg);
	}

	//clear  common
	memset(&msg, 0, sizeof(msg));
	sprintf(msg.node, "%s_%s",node,COMMON); 	
	tcapi_unset_req(top, &msg);

	//copy common node	
	memset(src_nodeName, 0, sizeof(src_nodeName));		
	strcpy(src_nodeName[0],WAN);	
	strcpy(src_nodeName[1],COMMON);				
	if(search_node(src_nodeName)){		//node  exist
		memset(dst_nodeName, 0, sizeof(dst_nodeName));
		strcpy(dst_nodeName[0],node);	
		strcpy(dst_nodeName[1],COMMON);	
		parentNode = mxmlFindElement(top, top, dst_nodeName[0],
					NULL, NULL, MXML_DESCEND);
		if(parentNode == NULL){
			tcdbg_printf("update_wan_node():find wan node fail\r\n");
			return FAIL;	
		}
		mxmlNewElement(parentNode,	dst_nodeName[1]);
		copy_node(dst_nodeName, src_nodeName);		
	}


	//copy pvc node	
	for(i = 0 ; i < PVC_NUM; i++){
		memset(src_nodeName, 0, sizeof(src_nodeName));	
		strcpy(src_nodeName[0],WAN);				
		sprintf(src_nodeName[1], "%s%d", WAN_PVC, i);
		if(search_node(src_nodeName) == NULL){		//node not exist
			continue;
		}

		memset(dst_nodeName, 0, sizeof(dst_nodeName));
		strcpy(dst_nodeName[0],node);	
		
		parentNode = mxmlFindElement(top, top, dst_nodeName[0],
					NULL, NULL, MXML_DESCEND);
		if(parentNode == NULL){
			tcdbg_printf("update_wan_node():find wan node fail\r\n");
			return FAIL;	
		}
		sprintf(dst_nodeName[1], "%s%d", WAN_PVC, i);				
		mxmlNewElement(parentNode,	dst_nodeName[1]);
		copy_node(dst_nodeName, src_nodeName);

		
		//Entry
		parentNode = mxmlFindElement(parentNode, parentNode, dst_nodeName[1],
					NULL, NULL, MXML_DESCEND);
		if(parentNode == NULL){
			tcdbg_printf("update_wan_node():find wan_pvc node fail\r\n");
			return FAIL;	
		}
		
		for(j=0 ; j < PVC_ENTRY_NUM; j++){
			sprintf(src_nodeName[2], "%s%d", WAN_INTERFACE, j);				
			if(search_node(src_nodeName) == NULL){		//node not exist
				continue;
			}

			sprintf(dst_nodeName[2], "%s%d", WAN_INTERFACE, j);				
			mxmlNewElement(parentNode,	dst_nodeName[2]);				
			copy_node(dst_nodeName, src_nodeName);
		}
	}

	return SUCCESS;
}

/*______________________________________________________________________________
**	wanATM_boot
**
**	descriptions:
**		the boot function of wanATM.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
wanATM_boot(mxml_node_t *top){
	return SUCCESS;
}

/*______________________________________________________________________________
**	wanATM_init
**
**	descriptions:
**		the init function of wanATM.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
wanATM_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="WanATM",
#if defined(TCSUPPORT_CT_RECONNECT_NODE)
			.type=PVC_TYPE | ADV_FLAG|ENTRY_TYPE | MAX_SMUX_NUM,
			.adv_table=wan_adv_table,
#else
			.type=PVC_TYPE | ENTRY_TYPE | MAX_SMUX_NUM,
			.adv_table=NULL,
#endif
			.merge_table=wan_merge_table,
			.cfg_read=NULL,
			.cfg_write=wanATM_write,
			.cfg_verify=wanATM_verify,
			.cfg_execute=wanATM_execute,
			.cfg_boot=wanATM_boot,
			.cfg_pre_unset= NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}

/*______________________________________________________________________________
**	vlanbind_write
**
**	descriptions:
**		Find vlanbind element from mxml tree then write vlanbind's system parameter to specify
**		configuration files.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  VlanBind cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
wanATM_write(mxml_node_t *top, mxml_node_t *parant){
	return SUCCESS;
}
/*______________________________________________________________________________
**	wanATM_verify
**
**	descriptions:
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
wanATM_verify(mxml_node_t *node){
	return SUCCESS;
}

/*_____________________________________________________________________________
**      function name: wanATM_execute
**      descriptions:
**            To execute wanATM node service.
**
**      parameters:
**            node:   Put the vlanbind of cfg node tree that is used to execute vlanbind settings.
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
**     
**____________________________________________________________________________
*/
int
wanATM_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	return SUCCESS;
}


/*______________________________________________________________________________
**	wanPTM_boot
**
**	descriptions:
**		the boot function of wanPTM.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
wanPTM_boot(mxml_node_t *top){
	return SUCCESS;
}

/*______________________________________________________________________________
**	wanPTM_init
**
**	descriptions:
**		the init function of wanATM.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
wanPTM_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="WanPTM",
#if defined(TCSUPPORT_CT_RECONNECT_NODE)
			.type=PVC_TYPE | ADV_FLAG|ENTRY_TYPE | MAX_SMUX_NUM,
			.adv_table=wan_adv_table,
#else
						.type=PVC_TYPE | ENTRY_TYPE | MAX_SMUX_NUM,
			.adv_table=NULL,
#endif
			.merge_table=wan_merge_table,
			.cfg_read=NULL,
			.cfg_write=wanPTM_write,
			.cfg_verify=wanPTM_verify,
			.cfg_execute=wanPTM_execute,
			.cfg_boot=wanPTM_boot,
			.cfg_pre_unset= NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}

/*______________________________________________________________________________
**	wanPTM_write
**
**	descriptions:
**		Find vlanbind element from mxml tree then write vlanbind's system parameter to specify
**		configuration files.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  VlanBind cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
wanPTM_write(mxml_node_t *top, mxml_node_t *parant){
	return SUCCESS;
}
/*______________________________________________________________________________
**	wanPTM_verify
**
**	descriptions:
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
wanPTM_verify(mxml_node_t *node){
	return SUCCESS;
}

/*_____________________________________________________________________________
**      function name: wanPTM_execute
**      descriptions:
**            To execute wanPTM node service.
**
**      parameters:
**            node:   Put the vlanbind of cfg node tree that is used to execute vlanbind settings.
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
**     
**____________________________________________________________________________
*/
int
wanPTM_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	return SUCCESS;
}

#endif

/*_____________________________________________________________________________
**      function name: updateDhcpRules
**      descriptions:
**            update Lan DHCP rules via ebtables
**
**      parameters:
**            none
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
**     
**____________________________________________________________________________
*/
int updateDhcpRules(mxml_node_t *top)
{
	int i = 0, j = 0;

	if ( !top )
		return -1;

	/* update dhcp relay rule */
	for ( i = 0; i < PVC_NUM; i++ )
	{
		for ( j = 0; j < MAX_SMUX_NUM; j++ )
		{
			set_DHCPRealy_info(top, i, j, DEL_DHCPREALY_CMD);
#if defined(TCSUPPORT_CT_DHCP_PORT_FILTER)
			set_DHCP_PortFilter_info(top, i, j, DEL_DHCP_PORTFILTER_CMD);
#endif
			set_DHCPRealy_info(top, i, j, ADD_DHCPREALY_CMD);
#if defined(TCSUPPORT_CT_DHCP_PORT_FILTER)
			set_DHCP_PortFilter_info(top, i, j, ADD_DHCP_PORTFILTER_CMD);
#endif
		}
	}

	return 0;
}

