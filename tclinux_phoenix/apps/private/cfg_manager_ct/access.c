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
#include "utility.h"
#include <dirent.h>

#if defined(TCSUPPORT_CT_E8GUI)
#include "../../public/mtd/tc_partition.h"
#endif
#if defined(TCSUPPORT_CT_MIDWARE)
int uTr069Enable;
#endif
//#include "../lib/libtcapi.h"
#if 0  /*Orginal ci-command mode */
/*==2008.12.19 pork modified==*/
const char
snmpd_attribute[][ATTR_SIZE]=
{

	{"Active"},
	{"rwcommunity"},
	{"rocommunity"},
	{""},
};
const char
snmpd_keyword[][ATTR_SIZE]=
{
	{"rwcommunity "},
	{"rocommunity "},
	{""},
};
/*==2008.12.19 pork modified==*/
const char
upnpd_attribute[][ATTR_SIZE]=
{
	{"autoconf"},
	{"igd"},
	/*
	{"lanif"},
	{"wanif"},
	{"nat"},
	*/
	{""},
};

const char
ddns_attribute[][ATTR_SIZE]=
{
	{"SERVERNAME"},
	{"MYHOST"},
	{"EMAIL"},
	{"USERNAME"},
	{"PASSWORD"},
	{"WILDCARD"},
	{""},
};
#endif
#if 0  /*Orginal ci-command mode */
/*______________________________________________________________________________
**	snmpd_read
**
**	descriptions:
**		Read snmpd  system parameter from configuration file and record those
**		information to mxml_node_t.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
snmpd_read(mxml_node_t *top){

	FILE  *fp = NULL;
	char tmp[MAXSIZE];
	int i=0;
	char snmpd_info[2][32];

	struct stat buf;

	mxml_node_t *group;  /* <Wan> */
	mxml_node_t *node;   /* <node> */

	group = mxmlNewElement(top, SNMPD);

	fp = fopen(SNMPD_PATH, "r");
	if(fp == NULL){
		return FAIL;
	}
	else{
		/*If the snmpd_stat.conf is exit, it's mean snmpd is avtived.*/
		if(stat(SNMPD_STAT_PATH,&buf) == 0){
			node = mxmlNewElement(group, SNMPD_ENTRY);
			mxmlElementSetAttr(node, snmpd_attribute[SNMPD_STAT_INDEX], ACTIVE);
			while(fgets(tmp, MAXSIZE,fp)){
				if(strstr(tmp, snmpd_keyword[i]) != NULL && strlen(snmpd_keyword[i]) != 0){
					memset(snmpd_info, 0, sizeof(snmpd_info));
					sscanf(tmp,"%s %s",snmpd_info[0],snmpd_info[1]);
					mxmlElementSetAttr(node, snmpd_info[SNMPD_ATTR_INDEX], snmpd_info[SNMPD_VALUE_INDEX]);
					i++;
				}
			}
		}
		fclose(fp);
	}
	return 0;
} /* end snmpd_read */
#endif

#ifndef PURE_BRIDGE
//"Snmpd",	SINGLE_TYPE,	NULL, NULL,NULL, snmpd_write, snmpd_verify, snmpd_execute},
/*______________________________________________________________________________
**	snmpd_boot
**
**	descriptions:
**		the boot function of snmp.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**_______________________________________________________________________________
*/
int
snmpd_boot(mxml_node_t *top){
	FILE *startupSh=NULL;
	startupSh=fopen("/etc/snmp/snmpd_stat.conf","r");
	if(startupSh){
		fclose(startupSh);
		system("/userfs/bin/snmpd -f -c /etc/snmp/snmpd.conf -P /var/log/snmpd.pid &");
	}
	return SUCCESS;
}
/*______________________________________________________________________________
**	snmpd_init
**
**	descriptions:
**		the init function of snmp.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
snmpd_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="Snmpd",
			.type=SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=snmpd_write,
			.cfg_verify=snmpd_verify,
			.cfg_execute=snmpd_execute,
			.cfg_boot=snmpd_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}
/*______________________________________________________________________________
**	snmpd_write
**
**	descriptions:
**		Find snmpd element from mxml tree then write snmp's system parameter to specify
**		configuration files.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  Snmpd cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
snmpd_write(mxml_node_t *top, mxml_node_t *parant){

	char snmp_info[3][ATTR_SIZE];
	int i=0;
	char *pSnmp_info=NULL;

	mxml_attr_t   *attr= NULL;         /* Cirrent attribute */
	mxml_node_t *node = NULL;
	/*==2008.12.19 pork modified==*/
	/*Notice: dont change the elemnet sequence of snmpd_ketword. The rwcommunity has to write prior to rocommunity*/
	char snmpd_keyword[][ATTR_SIZE]=
	{
		{"rwcommunity "},
		{"rocommunity "},
		{""},
	};
	char trimed_snmpd_keyword[ATTR_SIZE]={0};
         /*==2008.12.19 pork modified==*/
	const char* snmpd_attr_value = NULL;
	/*krammer add for firewall*/
	char keyword_for_firewall[][256]={
		{"sysLocation"},
		{"sysContact"},
		{"sysName"},
		{""},
	};
	char snmp_config_path[]="/var/tmp/snmp.tmp";
	char snmp_location_and_contact[512]={0};

 	/*
	node = mxmlFindElement(tree, tree, SNMPD_ENTRY,
	NULL, NULL,
	MXML_DESCEND);
	*/
	node = mxmlFindElement(parant, parant, SUB_NODE_NAME,
		NULL, NULL, MXML_DESCEND);

	if(node == NULL){
		return FAIL;
	}
	attr = node->value.element.attrs;
	/*==2008.12.28 pork modified==*/
	for(i = 0; i < sizeof(snmpd_keyword)/ATTR_SIZE ; i++){
		if(i == SNMPD_STAT_INDEX){
			sprintf(snmp_info[SNMPD_STAT_INDEX],"%s", attr->value);
		}
		else{
			/*there are space word after each snmpd_keyword element.
			 *mxmlElementGetAttr could not found attr's value if there are additional space word in attr name.
			 *Consequently,use another string trimed_snmpd_keyword to store trimed snmpd_keyword
			 *and pass it to mxmlElementGetAttr
			 * */
			strncpy(trimed_snmpd_keyword,snmpd_keyword[i-1],strlen(snmpd_keyword[i-1]));
			trimed_snmpd_keyword[strlen(snmpd_keyword[i-1])-1] = '\0';
			snmpd_attr_value = mxmlElementGetAttr(node,(char *)trimed_snmpd_keyword);
			if(snmpd_attr_value != NULL){
				sprintf(snmp_info[i],"%s  %s \n",trimed_snmpd_keyword,snmpd_attr_value);
			}
			else{
				return FAIL;
			}
			memset(trimed_snmpd_keyword,0,sizeof(trimed_snmpd_keyword));
		}
	}

	for(i=0; strlen(keyword_for_firewall[i]); i++){
		snmpd_attr_value = mxmlElementGetAttr(node,(char *)keyword_for_firewall[i]);
		if(snmpd_attr_value != NULL){
			sprintf(snmp_location_and_contact, "%s%s\n", snmp_location_and_contact, snmpd_attr_value);
		}
		else{
			//tcdbg_printf("snmp write return fail(%s)\n", keyword_for_firewall[i]);
			return FAIL;
		}
	}

	/*==2008.12.28 pork modified==*/
	if(strcmp(snmp_info[SNMPD_STAT_INDEX], ACTIVE) == 0){
		if(write2file(SNMPD_ACTIVE, SNMPD_STAT_PATH) == FAIL)
			return FAIL;
		if(write2file(snmp_location_and_contact, snmp_config_path) == FAIL){
			return FAIL;
		}
	}

	pSnmp_info = (char *)snmp_info;
	/*Only snmp_info[1] && snmp_info[2] parameters need to update snmpd.conf file*/
	updateString(SNMPD_PATH, (char *)snmpd_keyword, pSnmp_info + ATTR_SIZE);
	return SUCCESS;
}/* end snmpd_write */

/*______________________________________________________________________________
**	snmpd_verify
**
**	descriptions:
**		To check the system parameter of snmpd element  is validly or not.
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
snmpd_verify(mxml_node_t *node){
	return SUCCESS;
}/* end snmpd_verify */

/*_____________________________________________________________________________
**      function name: snmpd_execute
**      descriptions:
**            To execute snmpd node service.
**
**      parameters:
**            top:   Put the snmpd of cfg node tree that is used to execute snmpd information.
**            name: Put the snmpd cfg node name.
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
snmpd_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	char snmpd_stat[8]={0};
	char cmd[64]={0};
	
	tcapi_msg_t msg;
	
	getAttrValue(top, name, NODE_ACTIVE, snmpd_stat);
	kill_process(SNMPD_PID_PATH);

	if(strcmp(snmpd_stat, ACTIVE) == 0){
		sprintf(cmd, SNMPD_CMD, SNMPD_PATH, SNMPD_PID_PATH);
		system(cmd);
	}
	/*reset firewall*/
	sleep(1);
	memset(&msg,0,sizeof(msg)) ;
	strcpy(msg.node, FIREWALL);
	tcapi_commit_req(top, &msg);

	return SUCCESS;
}/* end snmpd_execute */

#if 0  /*Orginal ci-command mode */
/*______________________________________________________________________________
**	upnpd_read
**
**	descriptions:
**		Read upnpd  system parameter from configuration file and record those
**		information to mxml_node_t.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
upnpd_read(mxml_node_t *top){
	int retval=0;
	char *pAttr=NULL;

	pAttr=(char *)upnpd_attribute;
	retval=node_read(top,UPNPD, UPNPD_PATH, pAttr,NO_QMARKS);
	return retval;
}/* end upnpd_read */
#endif

//"Upnpd",	SINGLE_TYPE,	NULL, NULL,NULL, upnpd_write, upnpd_verify, upnpd_execute},
/*______________________________________________________________________________
**	upnpd_boot
**
**	descriptions:
**		the boot function of upnp.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**_______________________________________________________________________________
*/
int
upnpd_boot(mxml_node_t *top){
#if defined(TCSUPPORT_SIMPLE_UPNP)
	FILE *startupSh=NULL;
	system("iptables -t nat -N UPNP_PRE");
	system("iptables -t nat -A PREROUTING -j UPNP_PRE");
	startupSh=fopen("/etc/igd/igd.conf","r");
	if(startupSh){
		fclose(startupSh);
		system("/userfs/bin/upnpd &");
	}
#endif
	return SUCCESS;
}
/*______________________________________________________________________________
**	upnpd_init
**
**	descriptions:
**		the init function of upnpd.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
upnpd_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="Upnpd",
			.type=SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=upnpd_write,
			.cfg_verify=upnpd_verify,
			.cfg_execute=upnpd_execute,
			.cfg_boot=upnpd_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}
/*______________________________________________________________________________
**	upnpd_write
**
**	descriptions:
**		Find upnpd element from mxml tree then write upnp's system parameter to specify
**		configuration files.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  upnpd cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
upnpd_write(mxml_node_t *top, mxml_node_t *parant){

	int retval=0;

	/*mxml_node_t *node = NULL;

	node = mxmlFindElement(tree, tree, UPNPD_ENTRY,
		NULL, NULL,
		MXML_DESCEND);

	node = mxmlFindElement(parant, parant, SUB_NODE_NAME,
		NULL, NULL, MXML_DESCEND);
	retval = node_write(node, UPNPD_PATH, NO_QMARKS);*/
	/*write upnp config file*/
#if defined(TCSUPPORT_SIMPLE_UPNP)
	retval = write_upnp_conf(top);
#endif
	return retval;
}/* end upnpd_write */

/*______________________________________________________________________________
**	upnpd_verify
**
**	descriptions:
**		To check the system parameter of upnpd element  is validly or not.
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
upnpd_verify(mxml_node_t *node){
	return SUCCESS;
}/* end upnpd_verify */

/*_____________________________________________________________________________
**      function name: upnpd_execute
**      descriptions:
**            To execute upnpd node service.
**
**      parameters:
**            node:   Put the ripd of cfg node tree that is used to upnpd information.
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
upnpd_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	char upnpd_stat[8]={0};
	tcapi_msg_t msg;

#if defined(TCSUPPORT_SIMPLE_UPNP)
	getAttrValue(top, name, NODE_ACTIVE, upnpd_stat);
	kill_process(UPNPD_PID_PATH);
	if(strcmp(upnpd_stat, ACTIVE) == 0){
		system(UPNPD_CMD);
	}
	else{
		unlink(UPNPD_PATH);
	}
#endif

#ifdef TCSUPPORT_WLAN
#ifdef WSC_AP_SUPPORT
		dowscd(top);
#endif
#endif

	//reset firewall
	sleep(1);
	memset(&msg,0,sizeof(msg)) ;
	strcpy(msg.node, FIREWALL);
	tcapi_commit_req(top, &msg);

	return SUCCESS;
}/* end upnpd_execute */
#ifdef CWMP
/*______________________________________________________________________________
**	cwmpRoute_init
**
**	descriptions:
**		store cwmp route info.
**	parameters:
**		void
**	return:
**		Success: 	0
**_______________________________________________________________________________
*/
int
cwmpRoute_init(void){
	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="CwmpRoute",
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

/*______________________________________________________________________________
**	update_connReq_rule
**
**	descriptions:
**		update connection request rule if connection request port changed
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**_______________________________________________________________________________
*/

char connReq_old_port[10];
void update_connReq_rule(mxml_node_t *top)
{
	char cwmp_node[MAX_ARG_NUM][MAX_NODE_NAME], 
		 wan_node[MAX_ARG_NUM][MAX_NODE_NAME], 
		 wanInfo_node[MAX_ARG_NUM][MAX_NODE_NAME];
	char connReq_port[10], isp[4], status[4], svc_list[SERVICE_LIST_LEN], if_name[16];
	char cmd_buf[128];
	int i, pvc_index, entry_index;

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

	//tcdbg_printf("connReq_old_port is %s, connReq_port is %s", connReq_old_port, connReq_port);

	/* if connection port is not changed, it should not set iptables rules again */
	if (!strcmp(connReq_old_port, connReq_port)) {
		return;
	}

	/* set iptables rule to let connection request packet come in */
	memset(wan_node, 0, sizeof(wan_node));
	memset(wanInfo_node, 0, sizeof(wanInfo_node));
	strcpy(wan_node[0], "Wan");
	strcpy(wanInfo_node[0], "WanInfo");
	for (i = 0; i < MAX_WAN_IF_INDEX; i++) {
		pvc_index = i / MAX_SMUX_NUM;
		entry_index = i - pvc_index * MAX_SMUX_NUM;

		sprintf(wan_node[1], "PVC%d", pvc_index);
		sprintf(wan_node[2], "Entry%d", entry_index);

		/* check isp, only for routing mode */
		memset(isp, 0, sizeof(isp));
		if (getAttrValue(top, wan_node, "ISP", isp) == TCAPI_PROCESS_OK) {
			if (!strcmp(isp, "3")) {
				/* only for routing mode */
				continue;
			}
		}
		else {
			continue;
		}

		/* check status */
		sprintf(wanInfo_node[1], "Entry%d", i);
		memset(status, 0, sizeof(status));
		if (getAttrValue(top, wanInfo_node, "Status", status) == TCAPI_PROCESS_OK) {
			if (0 != strcmp(status, "up")) {
				/* only for up wan interface */
				continue;
			}
		}
		else {
			continue;
		}
		
		/* get interface name */
		memset(if_name, 0, sizeof(if_name));
		if (getAttrValue(top, wan_node, "IFName", if_name) != TCAPI_PROCESS_OK) {
			continue;
		}
		
		/* check service list only for TR069 and TR069_INTERNET */
		memset(svc_list, 0, sizeof(svc_list));
		if (getAttrValue(top, wan_node, "ServiceList", svc_list) == TCAPI_PROCESS_OK) {
#if defined(TCSUPPORT_CT_SERVICELIST_E8C)
			if (strstr(svc_list, "TR069") == NULL) {
#else
			if (0 != strcmp(svc_list, "TR069") && 0 != strcmp(svc_list, "TR069_INTERNET")) {
#endif
				/* only for tr069 and tr069_internet */
				continue;
			}
		}
		else {
			continue;
		}

		//tcdbg_printf("connReq_old_port is %s, connReq_port is %s", connReq_old_port, connReq_port);
		
		/* set iptables rules */
		sprintf(cmd_buf, "iptables -t nat -D PRE_SERVICE -i %s -p tcp --dport %s -j ACCEPT 2>/dev/null", if_name, connReq_old_port);
		system(cmd_buf);
		sprintf(cmd_buf, "iptables -t nat -A PRE_SERVICE -i %s -p tcp --dport %s -j ACCEPT", if_name, connReq_port);
		system(cmd_buf);
	}
	

	/* update connection request port */
	strcpy(connReq_old_port, connReq_port);

	return;
	
}
#if defined(TCSUPPORT_CT_MIDWARE)
#define MTD_BLOCK_PROC "/proc/mtd"
#define MOUNT_JFFS2_SH "/usr/script/mount_jffs2.sh"
void mount_jffs2(){
	FILE *fp = NULL;
	int index = 0;
	char buf[256] = "\0";
	
	fp=fopen(MTD_BLOCK_PROC,"r");
	if(fp != NULL){
		while(fgets(buf,sizeof(buf),fp)){
			//tcdbg_printf("fp=%s!\n",buf);
			if(strstr(buf,"jffs2")){
				sscanf(buf,"mtd%d:",&index);
				tcdbg_printf("mount jffs2 mtdblock%d!\n",index);
				if(index != 0){
					//snprintf(buf,sizeof(buf),"chmod 755 %s",MOUNT_JFFS2_SH);
					//system(buf);
					snprintf(buf,sizeof(buf),"%s %d",MOUNT_JFFS2_SH,index);
					system(buf);
				}
				break;
			}
		}
		fclose(fp);
	}	
}
#endif

/*______________________________________________________________________________
**	cwmp_boot
**
**	descriptions:
**		the boot function of cwmp.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**_______________________________________________________________________________
*/
int
cwmp_boot(mxml_node_t *top){
#if defined(TCSUPPORT_CT_MIDWARE)
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char value[4] ="";
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], "Cwmp");
	strcpy(nodeName[1], "Entry");
	char buf[100] = "";
#endif

#if defined(TCSUPPORT_CT_UPNP_DM)
	system("/userfs/bin/upnp-dm &");
#endif

#if defined(TCSUPPORT_CT_MIDWARE)

	mount_jffs2();

	if (getAttrValue(top, nodeName, "Tr069Enable", value) == TCAPI_PROCESS_OK) {
		if (!strcmp(value, "0")) {
			uTr069Enable = 0;
		}
		else if (!strcmp(value, "2")) {
			uTr069Enable = 2;
		}else uTr069Enable = 1;
		sprintf(buf,"/usr/script/mw_restart.sh %d &",uTr069Enable);
		system(buf);
	}
#endif
	tcdbg_printf("\r\nEnter cwmp boot, we will start tr69 Process\n");
#if !defined(TCSUPPORT_CT_MIDWARE)
	system("/userfs/bin/tr69 &");
#endif
#if defined(TCSUPPORT_CT_E8GUI)
#ifdef SSL
	checkCertificateswitch();
#endif
#endif
	return SUCCESS;
}

char *
cwmp_adv_table[MAX_NODE_NAME]= 
{
	"tr143UDPEcho", "",
};
/*______________________________________________________________________________
**	cwmp_init
**
**	descriptions:
**		the init function of cwmp.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
cwmp_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="Cwmp",
			.type=ADV_FLAG|SINGLE_TYPE,
			.adv_table=cwmp_adv_table,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=cwmp_write,
			.cfg_verify=cwmp_verify,
			.cfg_execute=cwmp_execute,
			.cfg_boot=cwmp_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}
/*______________________________________________________________________________
**	cwmp_write
**
**	descriptions:
**	parameters:
**		top: 		
**		parant: 		
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
cwmp_write(mxml_node_t *top, mxml_node_t *parant){

/* 1. get acsUrl from node;
 * 2. parser url to get pure website
 *    e.g parser www.test.com from http://www.test.com:80/tr69
 * 3. write pure website to /etc/tr69Url.conf file
 * */
	int retval=0;
#ifdef TCSUPPORT_DNSEACHPVC
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char value[64] = {0};
	//char result[64] = {0};
	char *urlTemp1 = NULL, *urlTemp2 = NULL;
	int length = 0;
	FILE *fp = NULL;
	char buf[64] = {0};

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], "Cwmp");
	strcpy(nodeName[1], "Entry");
	
	if(getAttrValue(top, nodeName, "Active", value) != TCAPI_PROCESS_OK){
		return -1;
	}
	if(strcmp(value, "Yes") != 0)
		return 0;
		
	memset(value, 0, sizeof(value));
	if(getAttrValue(top, nodeName, "acsUrl", value) != TCAPI_PROCESS_OK){
		return -1;
	}
	urlTemp1 = value;
	strncpy(buf, value, 4);
	if(strcmp(buf, "http") == 0){
		if(*(urlTemp1+5) == ':')//https
			urlTemp1 += strlen("https://");
		else
			urlTemp1 += strlen("http://");
	}
	memset(buf, 0, sizeof(buf));
	urlTemp2 = urlTemp1;
	while(*urlTemp1 != '\0'){
		if(*urlTemp1 != ':' && *urlTemp1 != '/'){
			length++;
			urlTemp1++;
		}else
			break;
	}
	memcpy(buf, urlTemp2, length);
	fp = fopen("/etc/tr69Url.conf", "w");
	if(fp == NULL){
		tcdbg_printf("\n open failed");
		retval = -1;
	}
	fwrite(buf, sizeof(char), strlen(buf), fp);
	fclose(fp);
#if defined(TCSUPPORT_CT_MIDWARE)	
	if(getAttrValue(top, nodeName, "MWMgtUrl", value) != TCAPI_PROCESS_OK){
		return -1;
	}

	fp = fopen("/etc/midwareUrl.conf", "w");
	if(fp == NULL){
		tcdbg_printf("\n open failed");
		return -1;
	}
	fwrite(value, sizeof(char), strlen(value), fp);
	fclose(fp);
#endif
#endif	
	return retval;
}/* end cwmp_write */

/*______________________________________________________________________________
**	cwmp_verify
**
**	descriptions:
**		To check the system parameter of cwmp element  is validly or not.
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
cwmp_verify(mxml_node_t *node){
	return SUCCESS;
}/* end cwmp_verify */


#if defined(TCSUPPORT_CT_MIDWARE)
int midware_start(mxml_node_t *top){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char value[4];
	int enable=0;
	char cmdbuf[100] = "";
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], "Cwmp");
	strcpy(nodeName[1], "Entry");
	
	if (getAttrValue(top, nodeName, "Tr069Enable", value) == TCAPI_PROCESS_OK) {
		enable = atoi(value);
		if(uTr069Enable != enable){
			if((uTr069Enable == 0 && enable == 2) || (uTr069Enable == 2 && enable == 0)) return -1;
			if((uTr069Enable == 1 && enable == 0) || (uTr069Enable == 0 && enable == 1)){
				tcdbg_printf("midware_start:need reboot for this change!!\n");
				if(uTr069Enable == 0 && enable == 1) return 0;
				return -1;
			}
			if((uTr069Enable == 1 && enable == 2) || (uTr069Enable == 2 && enable == 1)){
				snprintf(cmdbuf,sizeof(cmdbuf),"echo 1 > /var/run/restart_tr069_itself");
				system(cmdbuf);
				snprintf(cmdbuf,sizeof(cmdbuf),"/usr/script/mw_restart.sh %d",enable);
				system(cmdbuf);
				uTr069Enable = enable;
				
				return 0;
			}
		}
	}
	return -1;
}
#endif

/*_____________________________________________________________________________
**      function name: cwmp_execute
**      descriptions:
**            To execute cwmp node service.
**
**      parameters:
**            node:   
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
**      1. xyzhu 2009/11/15
**		2. brian 2009/12/04 add the context of sending messages
**____________________________________________________________________________
*/
int
cwmp_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
//	key_t mqkey;
//	int oflag,mqid;
	int ret;
//	char text[256];
//	char acsurl[200];
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char cwmp_route_node[MAX_ARG_NUM][MAX_NODE_NAME];
	char value[64];
	long type = 1;
	int flag = 0;
	int mq = 1;

#if defined(TCSUPPORT_CT_INFORM_NODE)
	char ManuaValue[4];
	char MaintainValue[4];
	char changeFlagStr[4] = {0};
#endif
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], "Cwmp");
	strcpy(nodeName[1], "Entry");
	
	/* if tr069 is deactive, del cwmp route */
	if (getAttrValue(top, nodeName, "Active", value) == TCAPI_PROCESS_OK) {
	#if 0
		if (!strcmp(value, "No")) {
			del_tr069_route();
		}
		else 
	#endif
		if (!strcmp(value, "Yes")) {
			memset(cwmp_route_node, 0, sizeof(cwmp_route_node));
			strcpy(cwmp_route_node[0], "CwmpRoute");
			strcpy(cwmp_route_node[1], "Entry");
			/* tr069 will use the first route, so set it to new */
			setAttrValue(top, cwmp_route_node, "ifStatus0", "new");
#if defined(TCSUPPORT_CT_E8GUI)
#ifdef SSL
		ret = checkCertificateswitch();	
#endif
#endif
		}
	}
	/* set connection request rule */
	update_connReq_rule(top);
	
	cwmp_msg_t message;
	memset(&message,0,sizeof(cwmp_msg_t));
#if defined(TCSUPPORT_CT_INFORM_NODE)
	setAttrValue(top, nodeName, "Inform_Status","0");
	memset(ManuaValue, 0, sizeof(ManuaValue));
	getAttrValue(top, nodeName, "Manual_Inform", ManuaValue) ;

	memset(MaintainValue, 0, sizeof(MaintainValue));
	getAttrValue(top, nodeName, "Maintain_Inform", MaintainValue) ;

	getAttrValue(top, nodeName, "account_change_flag", changeFlagStr);
	if(strcmp(ManuaValue, "1") == 0){
		strcpy(message.text.reserved ,"1");
		setAttrValue(top, nodeName, "Manual_Inform","0");
	}
	if((strcmp(MaintainValue, "1") == 0) || (strcmp(changeFlagStr, "1") == 0)){
		strcpy(message.text.reserved ,"2");
		setAttrValue(top, nodeName, "Maintain_Inform","0");
	}
#endif

#if defined(TCSUPPORT_CT_MIDWARE)
	if(midware_start(top) == 1){
		strcpy(message.text.reserved ,"8");
	}
#endif
#if defined(TCSUPPORT_CT_PPPOE_EMULATOR)	
	getAttrValue(top, nodeName, "pppoe_emulator_finish_flag", changeFlagStr);
	if(strcmp(changeFlagStr, "1") == 0)
	{
		strcpy(message.text.reserved ,"5");
		setAttrValue(top, nodeName, "pppoe_emulator_finish_flag","0");
	}
#endif
	message.cwmptype = mq;
	//tcdbg_printf("\r\nsend begin");
	ret = sendmegq(type,&message,flag);
	//tcdbg_printf("\r\nsend end");
	if(ret < 0)
	{
		tcdbg_printf("\r\nsend message error!!");
		return ret;
	}
	else
	{
		tcdbg_printf("\r\nsend message ok!");
		return SUCCESS;
	}


}/* end cwmp_execute */

#if defined(TCSUPPORT_CT_E8GUI)
#ifdef SSL
/*______________________________________________________________________________
**function name:checkCertificateswitch
**
**description:
*    check the certificate swtich and update the CA_PATH file
**parameters:
*    none
**global:
*    none
**return:
*    0:success
*    -1:fail
**call:
*    none
**revision:
*     1. dgk 20100917
**____________________________________________________________________________*/
int
checkCertificateswitch(void)
{
	int ret = SUCCESS;
	mxml_node_t 	*node = NULL, *psubnode = NULL;
	char 		nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char 		subNode[9] = {0};
	char		value[16] = {0};
	int		index = 0;
	unsigned long ulSize[4] = {CERM1_RA_SIZE, CERM2_RA_SIZE, CERM3_RA_SIZE, CERM4_RA_SIZE};
	unsigned long ulOffsite[4] = {CERM1_RA_OFFSET, CERM2_RA_OFFSET, CERM3_RA_OFFSET, CERM4_RA_OFFSET};
	
	mxml_node_t *top = get_node_top();
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], "SslCA");
	strcpy(nodeName[1], "Common");
	
	if (getAttrValue(top, nodeName, "CurIndex", value) == TCAPI_PROCESS_OK)
	{
		index = atoi(value);
		strcpy(subNode, "SslCA");
		node = mxmlFindElement(top, top, subNode,
			NULL, NULL, MXML_DESCEND);
		if(node){
			memset(subNode, 0, sizeof(subNode));
			sprintf(subNode,"%s%d",SUB_NODE_NAME, index);
			psubnode = mxmlFindElement(node, node, subNode,
			NULL, NULL, MXML_DESCEND);
			if(psubnode){
				memset(nodeName, 0, sizeof(nodeName));
				strcpy(nodeName[0], "Cwmp");
				strcpy(nodeName[1], "Entry");
				memset(value, 0x00, sizeof(value));
				if (getAttrValue(top, nodeName, "EnableCert", value) == TCAPI_PROCESS_OK)
				{
					if(!strcmp(value, "0")) {
						unlink(CA_PATH);
						setCertistatus(2);
					}
					else if(!strcmp(value, "1"))
					{
						FILE	*fw = NULL;
						FILE	*fr = NULL;
						char	cmd_buf[128] = {0};
						char	*pbuf = NULL;
						char	*pCAfile = NULL;
						char	*pEnd = NULL;
						int	nLength = 0;
						fw = fopen("/var/tmp/ca_temp.pem", "w");
						if(fw == NULL){
							tcdbg_printf("checkCertificateswitch:CA file /var/tmp/ca_temp.pem open failed\n");
							ret = FAIL;
							return ret;
						}
						sprintf(cmd_buf, TC_FLASH_READ_CMD, "/var/tmp/ca_temp.pem", ulSize[index], ulOffsite[index] ,RESERVEAREA_NAME);
						//tcdbg_printf("%s\n",cmd_buf);
						system(cmd_buf);
						if(fw){
							fclose(fw);
							fw = NULL;
						}
						//parse the file of read from flash 
						fr = fopen("/var/tmp/ca_temp.pem", "r");
						if(fr == NULL){
							tcdbg_printf("checkCertificateswitch:CA file /var/tmp/ca_temp.pem open failed\n");
							ret = FAIL;
							return ret;
						}
						pbuf = (char*)malloc(ulSize[index]);
						if(!pbuf){
							ret = FAIL;
							tcdbg_printf("checkCertificateswitch:malloc size failed\n");
							if(fr){
								fclose(fr);
								fr = NULL;
							}
							return ret;
						}
						memset(pbuf, 0x00, ulSize[index]);
						fseek(fr,0,SEEK_SET);
						fread(pbuf,sizeof(char), ulSize[index], fr);
						pEnd = strstr(pbuf, "-----END CERTIFICATE-----");
						fseek(fr,0,SEEK_SET);
						nLength = (int)(pEnd+25-pbuf);
						if(pbuf){
							free(pbuf);
							pbuf = NULL;
						}
						pCAfile = (char*)malloc(nLength+1);
						if(!pCAfile){
							ret = FAIL;
							tcdbg_printf("checkCertificateswitch:malloc size failed\n");
							if(fr){
								fclose(fr);
								fr = NULL;
							}
							return ret;
						}
						memset(pCAfile, 0x00, nLength+1);
						fread(pCAfile, sizeof(char), nLength, fr);
						pCAfile[nLength] = '\n';
						if(fr){
							fclose(fr);
							fr = NULL;
						}
						unlink("/var/tmp/ca_temp.pem");
						//write to CA_PATH
						fw = fopen(CA_PATH, "w");
						if(fw == NULL){
							tcdbg_printf("checkCertificateswitch:CA_PATH file open failed\n");
							ret = FAIL;
							if(pCAfile){
								free(pCAfile);
								pCAfile = NULL;
							}
							return ret;
						}
						fwrite(pCAfile, sizeof(char), nLength+1, fw);
						if(fw){
							fclose(fw);
							fw = NULL;
						}
						if(pCAfile){
							free(pCAfile);
							pCAfile = NULL;
						}
						setCertistatus(3);
					}
				}
			}
			else{
				tcdbg_printf("\r\nCertificate file isn't exist!\n");
				setCertistatus(4);
				ret = FAIL;
			}
		}
	}
	return ret;
}
/*_____________________________________________________________________________
**      function name: setCertistatus
**      descriptions:
**            Set Upgrade certificate status code.
**            	0:   	Success
**		1:	Certificate file incorrect
**		2:	Certificate file disabled
**		3:	Certificate file enabled
**		4:	the certificate file is not exist
**		5:	the certificate file is too large	
**      parameters:
**            ista:   status code.
**
**      global:
**             None
**
**      return:
**            Success:        0
**
**      call:
**		None
**      revision:
**      1. dgk 2010/9/17
**____________________________________________________________________________
*/
int
setCertistatus(int ista)
{
	mxml_node_t *top = get_node_top();
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char str[4] = {0};
	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], "Cwmp");
	strcpy(nodeName[1], "Entry");
	sprintf(str, "%d", ista);
	setAttrValue(top, nodeName, "CertCode", str);

	return SUCCESS;
}
#endif
#endif
/*_____________________________________________________________________________
**      function name: del_tr069_route
**      descriptions:
**			 delete tr069 route.
**
**      parameters:
**            void
**
**      global:
**             None
**
**      return:
**             0: success
**			   -1: fail
**
**      call:
**
**      revision:
**      1. xyzhu_nj 20100831
**____________________________________________________________________________
*/
void del_tr069_route(void)
{
	FILE *fp = NULL;
	char line[128];

	fp = fopen(TR069_ROUTE_CONF, "r");
	if (NULL == fp) {
		return;
	}

	memset(line, 0, sizeof(line));
	while (fgets(line, sizeof(line), fp)) {
		if ('\0' != line[0]) {
		#ifdef WAN_DEBUG2
			tcdbg_printf("%s: line is %s\n", __FUNCTION__, line);
		#endif
			system(line);
		}
	}

	fclose(fp);

	unlink(TR069_ROUTE_CONF);
	return;
}
#endif
#if 0  /*Orginal ci-command mode */
/*______________________________________________________________________________
**	ddns_read
**
**	descriptions:
**		Read ddns  system parameter from configuration file and record those
**		information to mxml_node_t.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
ddns_read(mxml_node_t *top){
	int retval=0;
	char *pAttr=NULL;

	pAttr=(char *)ddns_attribute;
	retval=node_read(top,DDNS, DDNS_PATH, pAttr,NO_QMARKS);
	return retval;
}/* end ddns_read */
#endif

//"Ddns",		SINGLE_TYPE,	NULL, NULL,NULL, ddns_write, ddns_verify, ddns_execute},
/*______________________________________________________________________________
**	ddns_init
**
**	descriptions:
**		the init function of ddns.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
/*DDNS marge Table             */
#if defined(TCSUPPORT_CT_E8GUI)
char *
ddns_merge_table[MAX_NODE_NAME]=
{
	"ddns_id","",
};
#endif
#if defined(TCSUPPORT_CT_E8DDNS)
char *
ddns_adv_table[MAX_NODE_NAME]=
{
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	"tr69Entry",		
#endif
	"Common","",
};
#endif
int
ddns_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="Ddns",
#if defined(TCSUPPORT_CT_E8DDNS)
			.type=ADV_FLAG|ENTRY_TYPE|MAX_DDNS_NUM,
			.adv_table=ddns_adv_table,
#else
			.type=SINGLE_TYPE,
			.adv_table=NULL,
#endif

#if defined(TCSUPPORT_CT_E8GUI)
			.merge_table=ddns_merge_table,
#else
			.merge_table=NULL,
#endif
			.cfg_read=NULL,
#if defined(TCSUPPORT_CT_E8DDNS)
			.cfg_write=ddns_write,
			.cfg_verify=ddns_verify,
			.cfg_execute=ddns_execute,
#else
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=NULL,
#endif
#if defined(TCSUPPORT_CT_DSL_EX)
			.cfg_boot=ddns_boot,
#else
			.cfg_boot=NULL,
#endif
	};
	ret=register_cfg_node(&node);
	
	return ret;
}
#if defined(TCSUPPORT_CT_DSL_EX)
int
ddns_boot(mxml_node_t *top){
	copy_to_ddns_node();
	return SUCCESS;
}

#if defined(TCSUPPORT_CT_E8DDNS)
int
ddnsATM_init(void){
	int ret=0;
	cfg_node_t node={
			.name="DdnsATM",
			.type=ADV_FLAG|ENTRY_TYPE|MAX_DDNS_NUM,
			.adv_table=ddns_adv_table,
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
ddnsPTM_init(void){
	int ret=0;
	cfg_node_t node={
			.name="DdnsPTM",
			.type=ADV_FLAG|ENTRY_TYPE|MAX_DDNS_NUM,
			.adv_table=ddns_adv_table,
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
#endif

#if defined(TCSUPPORT_CT_E8DDNS)
/*_____________________________________________________________________________
**      function name: add_DDNS_CommonInfo
**      descriptions:
**            To add ddns common node after add ddns Entry node.
**
**      parameters:
**            node:   Put the ddns of cfg node tree that is used to execute ddns settings.
**
**      global:
**             None
**
**      return:
**
**      call:
**
**      revision:
**      1. jiehao 2010/11/02
**____________________________________________________________________________
*/
void add_DDNS_CommonInfo(mxml_node_t *top, int index)
{
	char Entry_index[3]={0};
	tcapi_msg_t msg;
	memset( &msg, 0, sizeof(msg) );
	strncpy( msg.node, "Ddns_Common", sizeof(msg.node));
	if( index < 40 )
	{
		strncpy( msg.attr, "Entrylist1", sizeof(msg.attr) );
	}
	else
	{
		strncpy( msg.attr, "Entrylist2", sizeof(msg.attr) );
	}
	tcapi_get_req( top, &msg );
	snprintf( Entry_index, sizeof(Entry_index), "%d", index);
	strcat( msg.value, Entry_index );
	strcat( msg.value, " ");
	tcapi_set_req( top, &msg);
}

/*______________________________________________________________________________
**	ddns_write
**
**	descriptions:
**		Find ddns element from mxml tree then write ddns's system parameter to specify
**		configuration files.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  ddns cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
ddns_write(mxml_node_t *top, mxml_node_t *parant){

	int retval = 0, j = 0, i, flag = 1;
	mxml_node_t *node = NULL;
	mxml_attr_t   *attr=NULL;    
	char DDNS_ENTRY[10];
	char buf[256]={0};
	FILE *fp=NULL;
	tcapi_msg_t msg;
	
#if defined(TCSUPPORT_CT_JOYME)
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], value[16] = {0}, tmp[32] = {0}, actbuf[32] = {0}, fpbuf[32] = {0};
	FILE *orayFP = NULL;
#endif
	
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	char srcNode[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char dstNode[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	int setindex = -1;
#endif
	
	memset( &msg, 0, sizeof(msg) );
	strncpy( msg.node, "Ddns_Common", sizeof(msg.node) );
	strncpy( msg.attr, "Entrylist1", sizeof(msg.attr) );
	tcapi_set_req( top, &msg);
	strncpy( msg.attr, "Entrylist2", sizeof(msg.attr) );
	tcapi_set_req( top, &msg);

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	strncpy( msg.attr, "tr69Commit", sizeof(msg.attr) );
	tcapi_get_req( top, &msg);
	if(!strcmp(msg.value, "1"))
	{		
		strncpy( msg.attr, "tr69SetEntry", sizeof(msg.attr) );
		tcapi_get_req( top, &msg);
		setindex = atoi(msg.value);
		if(setindex>=0 && setindex<MAX_DDNS_NUM)
		{
			strcpy(dstNode[0], "Ddns");
			sprintf(dstNode[1], "%s%d", SUB_NODE_NAME, setindex);
			if(NULL == mxmlFindElement(parant, parant, dstNode[1], NULL, NULL, MXML_DESCEND))
				mxmlNewElement(parant, dstNode[1]);
			strcpy(srcNode[0], "Ddns");
			strcpy(srcNode[1], "tr69Entry");			
			copy_node(dstNode,srcNode);
		}
		memset( &msg, 0, sizeof(msg) );
		strncpy( msg.node, "Ddns_Common", sizeof(msg.node) );
		strncpy( msg.attr, "tr69Commit", sizeof(msg.attr) );
		tcapi_set_req( top, &msg);		
	}	
#endif

	fp = fopen(DDNS_PATH,"w");
	
	if(fp == NULL)
	{
		tcdbg_printf("Fail to open file %s in %s %s %d\n", DDNS_PATH, __FUNCTION__,__FILE__, __LINE__);
		return FAIL;
	}
	
	for(j=0; j<MAX_DDNS_NUM; j++)
	{	
#if defined(TCSUPPORT_CT_JOYME)
		memset(nodeName, 0, sizeof(nodeName));	
		strcpy(nodeName[0], "Ddns");	
		sprintf(nodeName[1], "Entry%d", j);		

		memset(value, 0, sizeof(value));
		getAttrValue(top,nodeName,"type", value);
		
		if (!strcmp(value, "plugin")){
			
			getAttrValue(top, nodeName, "DDNS_Name", tmp);	
			if (!strcmp(tmp, "ORAY")){
				getAttrValue(top, nodeName, "Active", actbuf);
				if (!strcmp(actbuf, "Yes")){
					
					orayFP = fopen(ORAY_CONFIG_PATH, "w+");				
					if (orayFP == NULL){				
						tcdbg_printf("\r\n@@ddns_write:Fail to open file %s in %s %s %d\n", ORAY_CONFIG_PATH, __FUNCTION__,__FILE__, __LINE__);	
						break;				
					}	
					

					fputs("[settings]\n", orayFP);			
					fputs("szHost = Phlinux3.Oray.Net\r\n", orayFP);						
					getAttrValue(top, nodeName, "USERNAME", fpbuf);			
					sprintf(buf, "szUserID = %s\r\n", fpbuf);			
					fputs(buf, orayFP);								
					memset(fpbuf, 0, sizeof(fpbuf));			
					memset(buf, 0, sizeof(buf));				
					getAttrValue(top, nodeName, "PASSWORD", fpbuf);				
					sprintf(buf, "szUserPWD = %s\r\n",fpbuf);			
					fputs(buf, orayFP);				
					memset(fpbuf, 0, sizeof(fpbuf));			
					memset(buf, 0, sizeof(buf));			
					getAttrValue(top, nodeName, "DDNS_Interface", fpbuf);			
					sprintf(buf, "nicName = %s\r\n",fpbuf);				
					fputs(buf, orayFP);			
					memset(buf, 0, sizeof(buf));			
					sprintf(buf, "szLog = %s\r\n",ORAY_LOG_PATH);			
					fputs(buf, orayFP);				
					fclose(orayFP);						
				}
			}
		}
		else{
		
		memset(DDNS_ENTRY, 0, sizeof(DDNS_ENTRY));
		snprintf(DDNS_ENTRY, sizeof(DDNS_ENTRY), "%s%d", SUB_NODE_NAME, j);
		node = mxmlFindElement(parant, parant, DDNS_ENTRY, NULL, NULL, MXML_DESCEND);
		if(node)
		{
			flag = 0;
			attr = node->value.element.attrs;
			for(i = 0; i < node->value.element.num_attrs; i++ )
			{
				memset(buf, 0, sizeof(buf));
				snprintf(buf, sizeof(buf), "%s=%s\r\n", attr->name, attr->value);
				fputs(buf, fp);
				attr ++;
			}
			fputs(END_OF_ENTRY, fp);
			add_DDNS_CommonInfo(top, j);
		}
	}
#else
		memset(DDNS_ENTRY, 0, sizeof(DDNS_ENTRY));
		snprintf(DDNS_ENTRY, sizeof(DDNS_ENTRY), "%s%d", SUB_NODE_NAME, j);
		node = mxmlFindElement(parant, parant, DDNS_ENTRY, NULL, NULL, MXML_DESCEND);
		if(node)
		{
			flag = 0;
			attr = node->value.element.attrs;
			for(i = 0; i < node->value.element.num_attrs; i++ )
			{
				memset(buf, 0, sizeof(buf));
				snprintf(buf, sizeof(buf), "%s=%s\r\n", attr->name, attr->value);
				fputs(buf, fp);
				attr ++;
			}
			fputs(END_OF_ENTRY, fp);
			add_DDNS_CommonInfo(top, j);
		}
#endif

	}
	
	fclose(fp);
	
	if(flag)
	{
		unlink(DDNS_PATH);
	}	

	return SUCCESS;
}/* end ddns_write */

/*______________________________________________________________________________
**	ddns_verify
**
**	descriptions:
**		To check the system parameter of ddns element  is validly or not.
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
ddns_verify(mxml_node_t *node){
	return SUCCESS;
}/* end ddns_verify */

/*_____________________________________________________________________________
**      function name: ddns_execute
**      descriptions:
**            To execute ddns node service.
**
**      parameters:
**            top:   Put the ddns of cfg node tree that is used to execute snmpd information.
**            name: Put the ddns cfg node name.
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
ddns_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){

	//getAttrValue(top, name, NODE_ACTIVE, ddns_stat);
	kill_process(DDNS_PID_PATH);
	char cmd[128] = {0};

#if defined(TCSUPPORT_CT_JOYME)
	char value[32] = {0},tmp[32] = {0}, probuf[32] = {0};	
	system("killall -9 phddns");

	if(getAttrValue(top, name, "type", value) == TCAPI_PROCESS_OK && !strcmp(value, "plugin")){	
		getAttrValue(top, name, "DDNS_Name", probuf);
		if (!strcmp(probuf, "ORAY")){
			getAttrValue(top, name, "Active", tmp);	
			if (!strcmp(tmp, "Yes")){			
				snprintf(cmd, sizeof(cmd), "cp %s /etc/", ORAY_CONFIG_PATH);
				system(cmd);
				snprintf(cmd, sizeof(cmd), "chmod 777 %s", ORAY_PATH);
				system(cmd);
				snprintf(cmd, sizeof(cmd), "%s -c /etc/phlinux.conf -d", ORAY_PATH);			
				system(cmd);		
				unlink("/etc/phlinux.conf");
			}	
		}
	}	
	else{				 	
 	snprintf(cmd, sizeof(cmd), "/userfs/bin/ez-ipupdate -d -F %s -P 60", DDNS_PID_PATH );
	system(cmd);
	}
#else
	snprintf(cmd, sizeof(cmd), "/userfs/bin/ez-ipupdate -d -F %s -P 60", DDNS_PID_PATH );
	system(cmd);
#endif
	return SUCCESS;
}/* end ddns_execute */
#endif
#if defined(TCSUPPORT_SIMPLE_UPNP)
/*______________________________________________________________________________
**function name:write_upnp_conf
**
**description:
*    create upnp conf
**parameters:
*    top: 		Specify the root of mxml_node_t structure.
**global:
*    none
**return:
*    0:success
*    -1:fail
**call:
*    none
**revision:
*     1.shnwind 20080601
*	2.wangyz 20100915
**____________________________________________________________________________*/
int
write_upnp_conf(mxml_node_t *top){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME],tmp[64]={0};

	char Wan_if[16]={0}, default_route[16]={0}, active[8]={0}, isp[2]={0}, autoconf[2]={0}, igd[8]={0};
	int i=0 , fd, active_pvc =- 1;
	int defRouteIndex = -1;
	char serviceApp[SERVICE_LIST_LEN] = {0};

	unlink(UPNPD_PATH);
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], UPNPD);
	strcpy(nodeName[1], SUB_NODE_NAME);

	/*query upnp attribute value*/
	if((getAttrValue(top, nodeName, "autoconf", autoconf) != TCAPI_PROCESS_OK)
	||(getAttrValue(top, nodeName, NODE_ACTIVE, igd)!= TCAPI_PROCESS_OK)){
		return FAIL;
	}
	/*if deactive, remove configure file*/
	if(!strcmp(igd, DEACTIVE)){
		unlink(UPNPD_PATH);
		return SUCCESS;
	}
	else{
		/*
			If user actived upnpd service, the igd.conf
			must be set "enable"  strings not "Yes".
		*/
		strcpy(igd, UPNPD_ACTIVE);
	}

	memset(nodeName,0,sizeof(nodeName));
	memset(default_route,0,sizeof(default_route));
	strcpy(nodeName[0], "WanInfo");
	strcpy(nodeName[1], "Common");

	getAttrValue(top, nodeName, "DefRouteIndexv4", default_route);
	if(strlen(default_route) != 0 && strncmp(default_route, "N/A", 3)) {
		defRouteIndex = atoi(default_route);
		memset(active, 0, sizeof(active));
		memset(Wan_if, 0, sizeof(Wan_if));

		if(get_waninfo_by_index(defRouteIndex, "Active", active) == SUCCESS) {
			if(!strcmp(active, "Yes")) {
				get_waninfo_by_index(defRouteIndex, WAN_IFNAME, Wan_if);
			}
		}
	}
	
	/*no pvc active default route, find the first actived Internet interface*/
	if((strlen(default_route) == 0) || (!strncmp(default_route, "N/A", 3)) || (strlen(Wan_if) == 0)){
		for(i = 0; i < MAX_WAN_IF_INDEX; i++){
			memset(active, 0, sizeof(active));
			memset(Wan_if, 0, sizeof(Wan_if));
			if(get_waninfo_by_index(i, "Active", active) == SUCCESS &&
				get_waninfo_by_index(i, "ServiceList", serviceApp) == SUCCESS){
				if(!strcmp(active, "Yes") &&
#if defined(TCSUPPORT_CT_SERVICELIST_E8C)
					(strstr(serviceApp, INTERNET)!=NULL)){
#else
					(!strcmp(serviceApp, INTERNET) || !strcmp(serviceApp, TR069_INTERNET))){
#endif
					get_waninfo_by_index(i, WAN_IFNAME, Wan_if);
					break;
				}
			}
		}
	}
	/*write config file*/
	fd = open(UPNPD_PATH,O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
	if(fd != -1){
		//sprintf(tmp,"autoconf=%s\n",autoconf);
		sprintf(tmp,"autoconf=1\n");
		write(fd,tmp,strlen(tmp));
		sprintf(tmp,"igd=%s\n",igd);
		write(fd,tmp,strlen(tmp));
		sprintf(tmp,"lanif=br0\n");
		write(fd,tmp,strlen(tmp));
		sprintf(tmp,"wanif=%s\n",Wan_if);
		write(fd,tmp,strlen(tmp));
		sprintf(tmp,"nat=1\n");
		write(fd,tmp,strlen(tmp));
		close(fd);
	}

	return SUCCESS;
}

/**********20080825 racing add************/
#endif

#if 0  /*Orginal ci-command mode */
/*______________________________________________________________________________
**	acl_read
**
**	descriptions:
**		Read acl system parameter from configuration file and record those information
**		to mxml_node_t.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
acl_read(mxml_node_t *top){
	return 0;
}/* end acl_read */
#endif

//"ACL", ADV_FLAG|ENTRY_TYPE|MAX_ACL_RULE, acl_adv_table, acl_merge_table, NULL, acl_write, NULL, acl_execute},
char *
acl_adv_table[MAX_NODE_NAME]=
{
	"Common","",
};
char *
acl_merge_table[MAX_NODE_NAME]=
{
	"acl_id","",
};
/*______________________________________________________________________________
**	acl_boot
**
**	descriptions:
**		the boot function of acl.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**_______________________________________________________________________________
*/
int
acl_boot(mxml_node_t *top){
	unsigned int new_filter_state=0;
	FILE *startupSh=NULL;
//	setnew_filter_state(top,acl_check_filter,ACL_VECTOR,&new_filter_state);
	if(acl_check_filter(top)){//filter on
		new_filter_state=pre_sys_state.filter_m_state|ACL_VECTOR;
	}
	else{//filter down
		new_filter_state=pre_sys_state.filter_m_state&(~ACL_VECTOR);
	}
	pre_sys_state.filter_m_state = check_and_set_filter(new_filter_state);
	startupSh=fopen("/tmp/etc/acl.sh","r");
	if(startupSh){
		fclose(startupSh);
		system("chmod 755 /tmp/etc/acl.sh");
		system("/tmp/etc/acl.sh");
	}

	return SUCCESS;
}
/*______________________________________________________________________________
**	acl_init
**
**	descriptions:
**		the init function of acl.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
acl_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="ACL",
			.type=ADV_FLAG|ENTRY_TYPE|MAX_ACL_RULE,
			.adv_table=acl_adv_table,
			.merge_table=acl_merge_table,
			.cfg_read=NULL,
			.cfg_write=acl_write,
			.cfg_verify=NULL,
			.cfg_execute=acl_execute,
			.cfg_boot=acl_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}

/*______________________________________________________________________________
**	acl_write
**
**	descriptions:
**		Find acl element from mxml tree then write acl's system parameter to specify
**		configuration files.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  qos cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
acl_write(mxml_node_t *top, mxml_node_t *parant){
	int i=0, j=0;
	int rule_count=0;
	char element_name[16]={0};
	char lower_value[32]={0};
	//char interfaces[PVC_NUM+1][8]; 
	char interfaces[MAX_WAN_IF_INDEX+1][8];  //jlliu_20100803
	int active_if_num = 0;
	/*When all pvc are avtived in wan interface and a ACL rule actived with interface "Both", 
	we need to store 9 interface information.*/ 
	char acl_cmd[192]={0};
	char iprange[32]={0};
	const char *value=NULL;
	char *ip_value1=NULL, *ip_value2=NULL;
	FILE *fp;
	mxml_node_t *node = NULL;

	/*initialization*/
	memset(interfaces,0,sizeof(interfaces));

	/*node = acl_common*/
	node = mxmlFindElement(parant, parant, ACL_COMMON, NULL , NULL, MXML_DESCEND);
	if(node == NULL){
		fprintf(stderr, "Error--ACL value is NULL.\n");
		return FAIL;
	}

	/*get activate value under acl_common element*/
	value = mxmlElementGetAttr(node, "Activate");

	/*remove previous file(settings)*/
	unlink(ACL_SH);

	/*if there is no activate value(NULL)*/
	if(value == NULL){
		fprintf(stderr, "Error--ACL activate value is NULL.\n");
		return FAIL;
	}

	/*open acl_start.sh to write*/
	fp = fopen(ACL_SH, "w+");
	if(fp == NULL){
		fprintf(stderr, "Error--file pointer is NULL.\n");
		return FAIL;
	}

	/*if ACL function is activated, compose iptables command, else do nothing*/
	if(!strcasecmp(value, "yes")){
		/*flush previous settings of acl_chain*/
		fputs("iptables -F acl_chain\n", fp);
		/*set counters of acl_chain as zero*/
		fputs("iptables -Z acl_chain\n", fp);
		/*remove acl_chain*/
		fputs("iptables -X acl_chain\n", fp);
		/*create acl_chain*/
		fputs("iptables -N acl_chain\n", fp);

#if 0
		/*remove rule in INPUT chain which jumps to acl_chain*/
		fputs("iptables -t filter -D INPUT -p TCP -m multiport --dport ftp,telnet,http,snmp -j acl_chain\n", fp);
		fputs("iptables -t filter -D INPUT -p UDP -m multiport --dport ftp,telnet,http,snmp -j acl_chain\n", fp);
		fputs("iptables -t filter -D INPUT -p ICMP --icmp-type 8 -j acl_chain\n", fp);

		/*create rule in INPUT chain which jumps to acl_chain*/
		fputs("iptables -t filter -I INPUT 1 -p TCP -m multiport --dport ftp,telnet,http,snmp -j acl_chain \n", fp);
		fputs("iptables -t filter -I INPUT 2 -p UDP -m multiport --dport ftp,telnet,http,snmp -j acl_chain \n", fp);
		fputs("iptables -t filter -I INPUT 3 -p ICMP --icmp-type 8 -j acl_chain \n", fp);
#endif
		/*remove rule in INPUT chain which jumps to acl_chain*/
		fputs("iptables -t filter -D ACL -p TCP -m multiport --dport ftp,telnet,http,snmp -j acl_chain\n", fp);
		fputs("iptables -t filter -D ACL -p UDP -m multiport --dport ftp,telnet,http,snmp -j acl_chain\n", fp);
		fputs("iptables -t filter -D ACL -p ICMP --icmp-type 8 -j acl_chain\n", fp);

		/*create rule in INPUT chain which jumps to acl_chain*/
		fputs("iptables -t filter -I ACL 1 -p TCP -m multiport --dport ftp,telnet,http,snmp -j acl_chain \n", fp);
		fputs("iptables -t filter -I ACL 2 -p UDP -m multiport --dport ftp,telnet,http,snmp -j acl_chain \n", fp);
		fputs("iptables -t filter -I ACL 3 -p ICMP --icmp-type 8 -j acl_chain \n", fp);

		/*for all rules in ACL configure node tree, retrieve data and compose corresponding iptables command*/
		for(i=0; i<MAX_ACL_RULE; i++){
			/*set query data, indicate entry*/
			sprintf(element_name,"%s%d", SUB_NODE_NAME, rule_count++);
			/*retrieve Entry data in ACL configure node tree*/
			node = mxmlFindElement(parant, parant, element_name, NULL , NULL, MXML_DESCEND);
			if(node == NULL){
				/*fprintf(stderr, "Error--Entry value is NULL.\n");*/
				continue;
			}

			/*retrieve Activate value in ACL Entry*/
			value = mxmlElementGetAttr(node, "Activate");

			if(value != NULL){
				/*if activate value isn't yes, do nothing and jump to next rule*/
				if(strcasecmp(value, "yes")){
					continue;
				}
			}else{
				fprintf(stderr, "Error--Entry activate value is NULL.\n");
				continue;
			}

			/*check ip format--begin*/
			/*retrieve ScrIPAddrBegin and ScrIPAddrEnd value in ACL Entry*/
			ip_value1 = (char *)mxmlElementGetAttr(node, "ScrIPAddrBegin");
			ip_value2 = (char *)mxmlElementGetAttr(node, "ScrIPAddrEnd");

			if((ip_value1 != NULL) && (ip_value2 != NULL)){
				/*check ip format respectively*/
				if(!(acl_check_ip_format(ip_value1)) || !(acl_check_ip_format(ip_value2))){
					continue;
				}

				/*check if ip range is valid, if not, do nothing and jump to next rule*/
				if(!acl_check_ip_range(ip_value1, ip_value2)){
					continue;
				}

				/*compose iprange string for latter usage*/
				sprintf(iprange,"%s-%s", ip_value1, ip_value2);

				/*0.0.0.0-0.0.0.0 means all ip*/
				if(!strcmp(iprange, "0.0.0.0-0.0.0.0")){
					strcpy(iprange, "0.0.0.0-223.255.255.255");
				}
			}else{/*ip value is NULL, do nothing and jump to next rule*/
				fprintf(stderr, "Error--IP value is NULL.\n");
				continue;
			}
			/*check ip format--endt*/

			/*retrieve Interface value in ACL Entry*/
			value = mxmlElementGetAttr(node, "Interface");
			if(value != NULL){
				/*if interface vaule is valid, retrieve currently activated interface*/
				if((!strcasecmp(value, "Wan")) || (!strcasecmp(value, "Lan")) || (!strcasecmp(value, "Both"))){
					active_if_num = acl_get_active_interface(top, interfaces, value);
				}else{/*interface vaule is invalid, do nothing and jump to next rule*/
					fprintf(stderr, "Wrong interface type.\n");
					continue;
				}
			}else{/*Interface value is NULL, do nothing and jump to next rule*/
				fprintf(stderr, "Error--Interface value is NULL.\n");
				continue;
			}

			/*retrieve Application value in ACL Entry*/
			value = mxmlElementGetAttr(node, "Application");

			if(value != NULL){
				/*because iptables command can only read lowercase application(port) data, so we change case before use*/
				strcpy(lower_value, value);
				string_tolower(lower_value);

				/*iptables command cannot recognize web, so we change it to http*/
				if(!strcasecmp(value, "web")){
					strcpy(lower_value, "http");
				}

				/*if Application value is all, it means ftp,telnet,http,snmp. for icmp, it is handled individually */
				if(!strcasecmp(value, "all")){
					strcpy(lower_value, "ftp,telnet,http,snmp");
				}

				/*if Application value is valid, start to compose iptables commands*/
				if((!strcasecmp(value, "ftp")) || (!strcasecmp(value, "telnet")) || (!strcasecmp(value, "web"))
					|| (!strcasecmp(value, "snmp")) || (!strcasecmp(value, "ping")) || (!strcasecmp(value, "all"))){

					/*for each active interface, compose iptables commands*/
					//for(j=0; (j <= PVC_NUM) && (strlen(interfaces[j])!=0); j++){
					for(j=0; (j < active_if_num) && (strlen(interfaces[j])!=0); j++){     //jlliu_20100803
					/*if Application value is not ping(icmp), compose iptables commands for TCP and UDP respectively*/
							if(strcasecmp(value, "ping")){
							/*compose iptables commands which using TCP*/
							strcpy(acl_cmd, "iptables -t filter -A acl_chain -i ");
							strcat(acl_cmd, interfaces[j]);
							strcat(acl_cmd, " -p TCP -m multiport --dport ");
							strcat(acl_cmd, lower_value);
							strcat(acl_cmd, " -m iprange --src-range ");
							strcat(acl_cmd, iprange);
							strcat(acl_cmd, " -j ACCEPT\n");
							fputs(acl_cmd, fp);
							fprintf(stderr,"acl_cmd=%s\n", acl_cmd);

							/*compose iptables commands which using UDP*/
							strcpy(acl_cmd, "iptables -t filter -A acl_chain -i ");
							strcat(acl_cmd, interfaces[j]);
							strcat(acl_cmd, " -p UDP -m multiport --dport ");
							strcat(acl_cmd, lower_value);
							strcat(acl_cmd, " -m iprange --src-range ");
							strcat(acl_cmd, iprange);
							strcat(acl_cmd, " -j ACCEPT\n");
							fputs(acl_cmd, fp);
							fprintf(stderr,"acl_cmd=%s\n", acl_cmd);
						}

						/*if Application value is not ping(icmp) or all, compose iptables commands for ICMP*/
						if((!strcasecmp(value, "ping"))||(!strcasecmp(value, "all"))){
							strcpy(acl_cmd, "iptables -t filter -A acl_chain -i ");
							strcat(acl_cmd, interfaces[j]);
							strcat(acl_cmd, " -p ICMP --icmp-type 8 -m iprange --src-range ");
							strcat(acl_cmd, iprange);
							strcat(acl_cmd, " -j ACCEPT\n");
							fputs(acl_cmd, fp);
							fprintf(stderr,"acl_cmd=%s\n", acl_cmd);
						}
					}
				}else{/*Application vaule is invalid, do nothing and jump to next rule*/
					fprintf(stderr, "Wrong application type.\n");
					continue;
				}
			}else{/*Application value is NULL, do nothing and jump to next rule*/
				fprintf(stderr, "Error--Application value is NULL.\n");
				continue;
			}
		}

		/*set policy of acl_chain, if no rule is meet, drop this pocket*/
		fputs("iptables -A acl_chain -j DROP\n", fp);

//		fclose(fp);
	}

	fclose(fp);
	chmod(ACL_SH ,777);

	return SUCCESS;
}/* end acl_write */

/*______________________________________________________________________________
**	acl_verify
**
**	descriptions:
**		To check the system parameter of acl element is valide or not.
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
acl_verify(mxml_node_t *node){
	return SUCCESS;
}/* end acl_verify */

/*_____________________________________________________________________________
**      function name: acl_execute
**      descriptions:
**            To execute acl node service.
**
**      parameters:
**            node:   Put the acl of cfg node tree that is used to execute acl settings.
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
**      1. racing 2008/5/19
**____________________________________________________________________________
*/
int
acl_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	unsigned int new_filter_state=0;
	char acl_status[8]={0}, acl_rule_status[8]={0}, element_name[16]={0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	int i=0;
	int acl_flag=0;

	/*initializatize nodeName*/
	memset(nodeName,0,sizeof(nodeName));
	/*set qurey data, indicate ACL*/
	strcpy(nodeName[0], ACL);

	/*for all rules in ACL configure node tree, check if any active rule exists*/
	for(i=0;i<MAX_ACL_RULE; i++){
		/*set query data, indicate entry*/
		sprintf(element_name,"%s%d", SUB_NODE_NAME, i);
		strcpy(nodeName[1], element_name);

		/*retrieve Activate value in ACL Entry*/
		if(getAttrValue(top, nodeName, "Activate", acl_rule_status) != TCAPI_PROCESS_OK){
			fprintf(stderr, "Error occurs while getting ACL_Entry Activate value.\n");
			continue;
		}

		/*if any active rule exists, turn on flag*/
		if(!strcasecmp(acl_rule_status, "Yes")){
			acl_flag=1;
			break;
		}
	}

	/*initializatize nodeName*/
	memset(nodeName,0,sizeof(nodeName));
	/*set qurey data, indicate ACL configure node and Common Element*/
	strcpy(nodeName[0], ACL);
	strcpy(nodeName[1], ACL_COMMON);

	/*get current information*/
	if(getAttrValue(top, nodeName, "Activate", acl_status) != TCAPI_PROCESS_OK){
		/*fprintf(stderr, "Error occurs while getting ACL Activate value.\n");*/
		return FAIL;
	}

	if((!strcasecmp(acl_status, "Yes"))&&(acl_flag==1)){
		/*check if iptable_filter module is loaded or not*/
//		setnew_filter_state(top,acl_check_filter,ACL_VECTOR,&new_filter_state);
		if(acl_check_filter(top)){//filter on
			new_filter_state=pre_sys_state.filter_m_state|ACL_VECTOR;
		}
		else{//filter down
			new_filter_state=pre_sys_state.filter_m_state&(~ACL_VECTOR);
		}
		//     		pre_sys_state.filter_m_state = check_and_set_filter(top,pre_sys_state.filter_m_state);
		pre_sys_state.filter_m_state = check_and_set_filter(new_filter_state);
		/*set iptable settings*/
		system(ACL_SH);
	}else{
		/*remove iptable settings*/
		system(ACL_STOP_SH);
		/*system("iptables -X acl_chain");*/
		/*check if iptable_filter module is removed or not*/
//		setnew_filter_state(top,acl_check_filter,ACL_VECTOR,&new_filter_state);
		if(acl_check_filter(top)){//filter on
			new_filter_state=pre_sys_state.filter_m_state|ACL_VECTOR;
		}
		else{//filter down
			new_filter_state=pre_sys_state.filter_m_state&(~ACL_VECTOR);
		}
		//     		pre_sys_state.filter_m_state = check_and_set_filter(top,pre_sys_state.filter_m_state);
		pre_sys_state.filter_m_state = check_and_set_filter(new_filter_state);
	}
	return SUCCESS;
}/* end acl_execute*/
/**********20080825 racing add************/

/*______________________________________________________________________________
**	acl_check_filter
**
**	descriptions:
**		check if the acl filter need filter module or not.
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
acl_check_filter(mxml_node_t *top){
	int i=0,acl_flag=0,filter_on=0;
	char acl_status[4]={0},acl_rule_status[8]={0};
	char element_name[16]={0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], ACL);
	for(i=0; i<MAX_ACL_RULE; i++){
		sprintf(element_name,"%s%d", SUB_NODE_NAME, i);
		strcpy(nodeName[1], element_name);
		if(getAttrValue(top, nodeName, "Activate", acl_rule_status) != TCAPI_PROCESS_OK){
			/*fprintf(stderr, "Error occurs while getting ACL_Entry Activate value.\n");*/
			continue;
		}
		if(!strcasecmp(acl_rule_status, "Yes")){
			acl_flag=1;
			break;
		}
	}

	memset(nodeName,0,sizeof(nodeName));

	strcpy(nodeName[0], ACL);
	strcpy(nodeName[1], ACL_COMMON);
	/*get current information*/
	if(getAttrValue(top, nodeName, "Activate", acl_status) == TCAPI_PROCESS_OK){
		if((!strcasecmp(acl_status, "yes"))&&(acl_flag!=0)){
			filter_on = 1;
		}
	}
	return filter_on;
}

/**********20080926 racing add************/
#if 0  /*Orginal ci-command mode */
/*______________________________________________________________________________
**	ipmacfilter_read
**
**	descriptions:
**		Read ipmacfilter system parameter from configuration file and record those information
**		to mxml_node_t.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
ipfilter_read(mxml_node_t *top){
	return 0;
}/* end acl_read */
#endif

//"IpMacFilter", ADV_FLAG|ENTRY_TYPE|MAX_IPMACFILTER_RULE, ipfilter_adv_table, ipfilter_merge_table, NULL, ipfilter_write, NULL, ipfilter_execute},
char *
ipfilter_adv_table[MAX_NODE_NAME]=
{
	"Common",
#if defined(TCSUPPORT_CMCC)
	"EtherTypeFilter",
#endif
	"",
};

char *
ipfilter_merge_table[MAX_NODE_NAME]=
{
	"ipfilter_id","",
};

/*______________________________________________________________________________
**	ipfilter_boot
**
**	descriptions:
**		the boot function of ipfilter.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**_______________________________________________________________________________
*/
int
ipfilter_boot(mxml_node_t *top){
	char MACFilterInputTable[64] = {0};
	char ebtablesCmd[128] = {0};
#if defined(TCSUPPORT_CMCC)
	char ipv4FilterInputTable[128] = {0};
#endif
	unsigned int new_filter_state=0;
	//FILE *startupSh=NULL;
//	setnew_filter_state(top,ipfilter_check_filter,IPFILTER_VECTOR,&new_filter_state);
	if(ipfilter_check_filter(top)){//filter on
		new_filter_state=pre_sys_state.filter_m_state|IPFILTER_VECTOR;
	}
	else{//filter down
		new_filter_state=pre_sys_state.filter_m_state&(~IPFILTER_VECTOR);
	}
	pre_sys_state.filter_m_state = check_and_set_filter(new_filter_state);
#if 0
	startupSh=fopen("/tmp/etc/ipmacfilter.sh","r");
	if(startupSh){
		fclose(startupSh);
		system("chmod 755 /tmp/etc/ipmacfilter.sh");
		system("/tmp/etc/ipmacfilter.sh");
	}
#endif
	strcpy(MACFilterInputTable, "ebmacfileter");
	memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
	sprintf(ebtablesCmd,"ebtables -t filter -N %s 2>/dev/null", MACFilterInputTable);
	system(ebtablesCmd);

	memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
	sprintf(ebtablesCmd,"ebtables -t filter -P %s RETURN", MACFilterInputTable);
	system(ebtablesCmd);

	memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
	sprintf(ebtablesCmd,"ebtables -t filter -A INPUT -j %s", MACFilterInputTable);
	system(ebtablesCmd);
	
#if defined(TCSUPPORT_CMCC)
	strcpy(ipv4FilterInputTable, "ipv4upfilter");
	memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
	sprintf(ebtablesCmd,"ebtables -t filter -N %s 2>/dev/null", ipv4FilterInputTable);
	system(ebtablesCmd);
	
	memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
	sprintf(ebtablesCmd,"ebtables -t filter -P %s RETURN", ipv4FilterInputTable);
	system(ebtablesCmd);
	
	memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
	sprintf(ebtablesCmd,"ebtables -t filter -A FORWARD -j %s", ipv4FilterInputTable);
	system(ebtablesCmd);
	
	memset(ipv4FilterInputTable, 0, sizeof(ipv4FilterInputTable));
	strcpy(ipv4FilterInputTable, "ipv4downfilter");
	memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
	sprintf(ebtablesCmd,"ebtables -t filter -N %s 2>/dev/null", ipv4FilterInputTable);
	system(ebtablesCmd);
	
	memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
	sprintf(ebtablesCmd,"ebtables -t filter -P %s RETURN", ipv4FilterInputTable);
	system(ebtablesCmd);
	
	memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
	sprintf(ebtablesCmd,"ebtables -t filter -A FORWARD -j %s", ipv4FilterInputTable);
	system(ebtablesCmd);	

	strcpy(MACFilterInputTable, "ebmacfileter_chain");
	memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
	sprintf(ebtablesCmd,"ebtables -t filter -N %s 2>/dev/null", MACFilterInputTable);
	system(ebtablesCmd);

	memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
	sprintf(ebtablesCmd,"ebtables -t filter -P %s RETURN", MACFilterInputTable);
	system(ebtablesCmd);

	memset(ebtablesCmd, 0, sizeof(ebtablesCmd));
	sprintf(ebtablesCmd,"ebtables -t filter -A FORWARD -j %s", MACFilterInputTable);
	system(ebtablesCmd);
#endif
	
	system(MACFILTER_SH);
	system(IPUPFILTER_SH);
	system(IPDOWNFILTER_SH);

	return SUCCESS;
}
/*______________________________________________________________________________
**	ipfilter_init
**
**	descriptions:
**		the init function of ipfilter.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
ipfilter_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="IpMacFilter",
#if defined(TCSUPPORT_CT_E8GUI)
			.type=ADV_FLAG|ENTRY_TYPE|MAX_IPMACFILTER_RULE|UPDATE_TYPE,
#else
			.type=ADV_FLAG|ENTRY_TYPE|MAX_IPMACFILTER_RULE,
#endif
			.adv_table=ipfilter_adv_table,
			.merge_table=ipfilter_merge_table,
#if defined(TCSUPPORT_CT_E8GUI)
			.cfg_read=ipfilter_read,
#else
			.cfg_read=NULL,
#endif
			.cfg_write=ipfilter_write,
			.cfg_verify=NULL,
			.cfg_execute=ipfilter_execute,
			.cfg_boot=ipfilter_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}

int g_ip_mac_flag = 0;
/*______________________________________________________________________________
**	ipmacfilter_write
**
**	descriptions:
**		Find ipmacfilter element from mxml tree then write ipmacfilter's system parameter to specify
**		configuration files.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  qos cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
ipfilter_write(mxml_node_t *top, mxml_node_t *parant){
	int i=0;
	//int rule_count=0;
	//int ipmacfilter_flag=0;
	char element_name[16]={0};
	char interface[16]={0};
	char ipmacfilter_cmd[192]={0}, ipmacfilter_cmd_head[48]={0}, ipmacfilter_cmd_tail[96]={0};
	char ebmacfilter_cmd_ipv4[256]={0}, ebmacfilter_cmd_head[48]={0}, ebmacfilter_cmd_tail_ipv4[128]={0};
	char ebmacfilter_cmd_ipv6[256]={0},ebmacfilter_cmd_tail_ipv6[128]={0};
	char target[8] = {0}, target_mac[8]={0}, target_ipup[8] = {0}, target_ipdown[8] = {0};
	char src_mask_dec[2]={0}, des_mask_dec[2]={0};
	const char *direction=NULL;
	const char *mac_addr=NULL;
	const char *interface_tmp=NULL;
	const char *list_type_mac = NULL, *list_type_ipup = NULL, *list_type_ipdown = NULL;
	const char *value=NULL;
	char *src_ip=NULL, *des_ip=NULL;
#if defined(TCSUPPORT_CMCC)
	char ipv4filter_cmd[512]={0}, ipv4filter_cmd_head[48]={0}, ipv4filter_cmd_tail[256]={0};
	char target_ipv4down[8] = {0},target_ipv4up[8] = {0},target_ipv4[8] = {0};
	char *ip_end=NULL;
	char cmdBuf[64]={0};
	const char *tmp=NULL;
	char buf[64]={0};
	char smacfilter_cmd[256]={0}, smacfilter_cmd_tail[128]={0}, macfilter_cmd_head[48]={0};
	char dmacfilter_cmd[256]={0}, dmacfilter_cmd_tail[128]={0};
#endif
	char *src_mask=NULL, *des_mask=NULL;
	char *src_port=NULL, *des_port=NULL;
	char *protocol=NULL;
	FILE *fp = NULL, *fp_mac = NULL, *fp_ipup = NULL, *fp_ipdown = NULL;
	mxml_node_t *node_common = NULL, *node = NULL;
	char active[6] = {0};
	const char *mac_on_off = NULL, *ipup_on_off = NULL, *ipdown_on_off = NULL, *mac_ip_type = NULL;
	/* mac_ip = 0(mac filter) 1(ip uplink filter) 2(ip down link filter) */
	int mac_ip = -1, start_index, end_index;

#if defined(TCSUPPORT_CMCC)
	node = mxmlFindElement(parant, parant, "EtherTypeFilter", NULL, NULL,MXML_DESCEND);
	if(node != NULL){
		for(i = 0; i < 8; i++){
			sprintf(buf,"P%d",i);
			tmp=mxmlElementGetAttr(node, buf);
			if(tmp!=NULL){			
				sprintf(cmdBuf, "echo %d %s > /proc/tc3162/ethertypefilter_array\n", i, tmp);
				system(cmdBuf);
			}
		}		
	}
#endif

	/*node = IpMacFilter_common*/
	node_common = mxmlFindElement(parant, parant, IPMACFILTER_COMMON, NULL , NULL, MXML_DESCEND);
	if(node_common == NULL){
		fprintf(stderr, "Error--IpMacFilter value is NULL.\n");
		return FAIL;
	}

	/* not reboot */
	if (g_ip_mac_flag == 1) {
		mac_ip_type = mxmlElementGetAttr(node_common, "IpMacType");
		if (mac_ip_type == NULL) {
			fprintf(stderr, "Error mac or ip type info is NULL.\n");
			return FAIL;
		}
		if (!strcmp(mac_ip_type, "Mac")) {
			mac_ip = 0;
		}
		else if (!strcmp(mac_ip_type, "IpUp")) {
			mac_ip = 1;
		}
		else if (!strcmp(mac_ip_type, "IpDown")) {
			mac_ip = 2;
		}
		else {
			fprintf(stderr, "Error mac or ip type value is wrong\n");
			return FAIL;
		}
	}

	/* when reboot, set mac filter, ip uplink filter and ipdown filter all */
	if (g_ip_mac_flag == 0) {
		mac_ip = 0;
		goto ip_mac_handle;

next_ip_mac_handle:
		mac_ip++;
		if (mac_ip < 3)
			goto ip_mac_handle;
		
		g_ip_mac_flag = 1;
		return SUCCESS;
	}

ip_mac_handle:	
	if (mac_ip == 0) {
		/* mac list type */
		list_type_mac = mxmlElementGetAttr(node_common, "ListTypeMac");
		if (list_type_mac) {
			if (!strcasecmp(list_type_mac, "black")) {
				strcpy(target_mac, "DROP");
			}
			else if (!strcasecmp(list_type_mac, "white")) {
				strcpy(target_mac, "RETURN");
			}
			else {
				if (g_ip_mac_flag == 0) {
					goto next_ip_mac_handle;
				}
				else {
					return FAIL;
				}
			}
		}
		else {
			if (g_ip_mac_flag == 0) {
				goto next_ip_mac_handle;
			}
			else {
				return FAIL;
			}
		}
		start_index = 0;
		end_index = MAX_MAC_FILTER_RULE;
	}
	else if (mac_ip == 1) {
		/* port uplink list type */

		list_type_ipup = mxmlElementGetAttr(node_common, "ListTypeIpUp");
		
		if (list_type_ipup) {
			
			if (!strcasecmp(list_type_ipup, "black")) {
				strcpy(target_ipup, "DROP");
#if defined(TCSUPPORT_CMCC)
				strcpy(target_ipv4up, "DROP");
#endif
			}
			else if (!strcasecmp(list_type_ipup, "white")) {
				strcpy(target_ipup, "RETURN");
#if defined(TCSUPPORT_CMCC)
				strcpy(target_ipv4up, "ACCEPT");
#endif
			}
			else {
				if (g_ip_mac_flag == 0) {
					goto next_ip_mac_handle;
				}
				else {
					return FAIL;
				}
			}
		}
		else {
			if (g_ip_mac_flag == 0) {
				goto next_ip_mac_handle;
			}
			else {
				return FAIL;
			}
		}
		start_index = MAX_MAC_FILTER_RULE;
		end_index = MAX_MAC_FILTER_RULE + MAX_IPUP_FILTER_RULE;
	}
	else if (mac_ip == 2) {
		/* port down link list type */
		list_type_ipdown = mxmlElementGetAttr(node_common, "ListTypeIpDown");
		if (list_type_ipdown) {
			if (!strcasecmp(list_type_ipdown, "black")) {
				strcpy(target_ipdown, "DROP");
#if defined(TCSUPPORT_CMCC)
				strcpy(target_ipv4down, "DROP");
#endif
			}
			else if (!strcasecmp(list_type_ipdown, "white")) {
				strcpy(target_ipdown, "RETURN");
#if defined(TCSUPPORT_CMCC)
				strcpy(target_ipv4down, "ACCEPT");
#endif
			}
			else {
				if (g_ip_mac_flag == 0) {
					goto next_ip_mac_handle;
				}
				else {
					return FAIL;
				}
			}
		}
		else {
			if (g_ip_mac_flag == 0) {
				goto next_ip_mac_handle;
			}
			else {
				return FAIL;
			}
		}
		start_index = MAX_MAC_FILTER_RULE + MAX_IPUP_FILTER_RULE;
		end_index = MAX_IPMACFILTER_RULE;
	}
	else {
		fprintf(stderr, "Error mac or ip type info is wrong.\n");
		if (g_ip_mac_flag == 0) {
			goto next_ip_mac_handle;
		}
		else {
			return FAIL;
		}
	}
	
#if defined(TCSUPPORT_CT_E8GUI)
	const char 	*pAttr = NULL;
	char		strDel[400] = {0};
	//ipmacfilter_flag = ipfilter_check_switch(top);
	/*
	if(ipmacfilter_flag == 0)//switch is off
	{
		tcdbg_printf("ipfilter_write():switch is off.\n");
		if (g_ip_mac_flag == 0) {
			goto next_ip_mac_handle;
		}
		else {
			return SUCCESS;
		}
	}
	*/
	//ipmacfilter_flag = 0;
	if (mac_ip == 0) {
		mac_on_off = mxmlElementGetAttr(node_common, "ActiveMac");
		unlink(MACFILTER_SH);
	}
	else if (mac_ip == 1) {
		ipup_on_off = mxmlElementGetAttr(node_common, "ActivePortOut");
		unlink(IPUPFILTER_SH);
		
	}
	else if (mac_ip == 2) {
		ipdown_on_off = mxmlElementGetAttr(node_common, "ActivePortIn");
		unlink(IPDOWNFILTER_SH);
	}
	//check attributea of ction and do unset action
	pAttr = mxmlElementGetAttr(node_common, "Action");
	if(pAttr != NULL){
		if(!strcmp(pAttr, "Del")){
			pAttr = NULL;
			pAttr = mxmlElementGetAttr(node_common, "DeleteIndex");
			if(pAttr != NULL){
				strcpy(strDel, pAttr);
				unset_action(strDel, DEL_IP);
			}
		}			
	}
#endif
	
#if 0
	/* no need check */
	ipmacfilter_flag = ipfilter_check_active(top);
	if(ipmacfilter_flag==0){
		return SUCCESS;
	}
#endif

	if (mac_ip == 0) {
		/* for mac filter */
		fp_mac = fopen(MACFILTER_SH, "w+");
		if (fp_mac == NULL) {
			fprintf(stderr, "Error--mac file pointer is NULL.\n");
			if (g_ip_mac_flag == 0) {
				goto next_ip_mac_handle;
			}
			else {
				return FAIL;
			}
		}

		/* flush mac filter */
		fputs("iptables -F macfilter_chain\n", fp_mac);
		fputs("iptables -Z macfilter_chain\n", fp_mac);
		fputs("ebtables -F ebmacfileter\n", fp_mac);
		fputs("ebtables -Z ebmacfileter\n", fp_mac);
		
#if defined(TCSUPPORT_CMCC)
		fputs("ebtables -F ebmacfileter_chain\n", fp_mac);
		fputs("ebtables -Z ebmacfileter_chain\n", fp_mac);
#endif
#if defined(TCSUPPORT_CT_PON_C7)
		fputs("iptables -F dhcpfilter_chain\n", fp_mac);
		fputs("iptables -Z dhcpfilter_chain\n", fp_mac);
#endif
		if (mac_on_off && mac_on_off[0] != '\0') {
			if (mac_on_off[0] != '1') {
				fclose(fp_mac);
				chmod(MACFILTER_SH, 777);
				if (g_ip_mac_flag == 0) {
					goto next_ip_mac_handle;
				}
				else {
					return SUCCESS;
				}
			}
		}
	}
	else if (mac_ip == 1) {
		/* ip up link */
		fp_ipup = fopen(IPUPFILTER_SH, "w+");
		if (fp_ipup == NULL) {
			fprintf(stderr, "Error--ip uplink file pointer is NULL.\n");
			if (g_ip_mac_flag == 0) {
				goto next_ip_mac_handle;
			}
			else {
				return FAIL;
			}
		}
		/* flush ip uplink filter */
		fputs("iptables -F ipupfilter_chain\n", fp_ipup);
		fputs("iptables -Z ipupfilter_chain\n", fp_ipup);
#if defined(TCSUPPORT_CMCC)
		fputs("ebtables -F ipv4upfilter\n", fp_ipup);
		fputs("ebtables -Z ipv4upfilter\n", fp_ipup);
#endif

		if (ipup_on_off && ipup_on_off[0] != '\0') {
			if (ipup_on_off[0] != '1') {
				fclose(fp_ipup);
				chmod(IPUPFILTER_SH, 777);
				if (g_ip_mac_flag == 0) {
					goto next_ip_mac_handle;
				}
				else {
					return SUCCESS;
				}
			}
		}

	}
	else if (mac_ip == 2) {
		/* ip down link */
		fp_ipdown = fopen(IPDOWNFILTER_SH, "w+");
		if (fp_ipdown == NULL) {
			fprintf(stderr, "Error--ip uplink file pointer is NULL.\n");
			if (g_ip_mac_flag == 0) {
				goto next_ip_mac_handle;
			}
			else {
				return FAIL;
			}
		}
		/* flush ip down link filter */
		fputs("iptables -F ipdownfilter_chain\n", fp_ipdown);
		fputs("iptables -Z ipdownfilter_chain\n", fp_ipdown);
#if defined(TCSUPPORT_CMCC)
		fputs("ebtables -F ipv4downfilter\n", fp_ipdown);
		fputs("ebtables -Z ipv4downfilter\n", fp_ipdown);
#endif
		if (ipdown_on_off && ipdown_on_off[0] != '\0') {
			if (ipdown_on_off[0] != '1') {
				fclose(fp_ipdown);
				chmod(IPDOWNFILTER_SH, 777);
				if (g_ip_mac_flag == 0) {
					goto next_ip_mac_handle;
				}
				else {
					return SUCCESS;
				}
			}
		}
	}
	else {
		if (g_ip_mac_flag == 0) {
			goto next_ip_mac_handle;
		}
		else {
			return FAIL;
		}
	}
	
	/*for sequence*/
	#if 0
	/*remove ipfilter_chain*/
	fputs("iptables -X ipfilter_chain\n", fp);
	/*create ipfilter_chain*/
	fputs("iptables -N ipfilter_chain\n", fp);

	/*remove rule in FORWARD chain which jumps to ipfilter_chain*/
	fputs("iptables -t filter -D FORWARD -j ipfilter_chain\n", fp);
	/*create rule in FORWARD chain which jumps to ipfilter_chain*/
	fputs("iptables -t filter -I FORWARD 1 -j ipfilter_chain\n", fp);
	#endif


	for (i = start_index; i < end_index ; i++) {
		sprintf(element_name,"%s%d", SUB_NODE_NAME, i);
		node = mxmlFindElement(parant, parant, element_name, NULL , NULL, MXML_DESCEND);	
		if(node == NULL){
		//	fprintf(stderr, "Error--Entry value is NULL.rule_count is %d\n", i);
			continue;
		}

		/* check active */
		value = mxmlElementGetAttr(node, "Active");
		if(value != NULL){
			if(strcasecmp(value, "yes")){
				fprintf(stderr, "Error --Entry active is not yes.\n");
				continue;
			}
		}else{
			fprintf(stderr, "Error--Entry Active value is NULL.\n");
			continue;
		}

		/* uplink is outgoing, downlink is incoming, but it is not important */
		direction = mxmlElementGetAttr(node, "Direction");
		if(direction != NULL && direction[0] != '\0'){
			if(strcasecmp(direction, "Both") && strcasecmp(direction, "Incoming") && strcasecmp(direction, "Outgoing")){
				fprintf(stderr, "Error --direction is not Both, Incoming and Outgoing.\n");
				continue;
			}
		}else{
			fprintf(stderr, "Error--Entry direction value is NULL.\n");
			continue;
		}

		/* check interface */
		memset(interface,0,sizeof(interface));
		interface_tmp = mxmlElementGetAttr(node, "Interface");
		if(interface_tmp==NULL){
			fprintf(stderr, "interface is null.\n");
			continue;
		}
		strcpy(interface, interface_tmp);
		#if 0
		if(get_interface_by_PVC(top, interface_tmp, interface)!=SUCCESS){
			fprintf(stderr, "Error--Error occur while retrieving interface data.\n");
			continue;
		}
		#endif
		/* interface value should be br0/nasx_y/pppx, check wan interface */
		if (interface[0] == 'n' || interface[0] == 'p') {
			if(get_waninfo_by_name(interface, "Active", active) == SUCCESS){
				/*if this interface is not activated, jump to next rule*/		
				if(strcmp(active, ACTIVE))
					continue;
			}
			else{
				continue;
			}
		}

		value = mxmlElementGetAttr(node, "RuleType");

		if(value != NULL){
			if (!strcasecmp(value, "IP")) {
				
				
				/* check if uplink or down link */
				if (mac_ip == 1) {
					strcpy(target, target_ipup);
					fp = fp_ipup;
					strcpy(ipmacfilter_cmd_head, "iptables -t filter -A ipupfilter_chain");
#if defined(TCSUPPORT_CMCC)
					strcpy(target_ipv4, target_ipv4up);
					strcpy(ipv4filter_cmd_head, "ebtables -t filter -A ipv4upfilter");
#endif
				}

				else if (mac_ip == 2) {
					strcpy(target, target_ipdown);
					fp = fp_ipdown;
					strcpy(ipmacfilter_cmd_head, "iptables -t filter -A ipdownfilter_chain");
					
#if defined(TCSUPPORT_CMCC)
					strcpy(target_ipv4, target_ipv4down);
					strcpy(ipv4filter_cmd_head, "ebtables -t filter -A ipv4downfilter");
#endif
				}
				else {
					/* impossible happen */
					continue;
				}

				/* it will happen in general */
				if (fp == NULL) {
					tcdbg_printf("port filter.\n");
					continue;
				}

				memset(ipmacfilter_cmd_tail, 0, sizeof(ipmacfilter_cmd_tail));
				
#if defined(TCSUPPORT_CMCC)
				memset(ipv4filter_cmd_tail, 0, sizeof(ipv4filter_cmd_tail));
#endif
				/* source ip address */
				src_ip = (char *)mxmlElementGetAttr(node, "SrcIPAddr");
				if (src_ip != NULL && src_ip[0] != '\0') {
					if (!check_ip_format(src_ip)) {
						tcdbg_printf("ipfilter_write():check src ip failed.\n");
						continue;
					}
#if defined(TCSUPPORT_CMCC)
					ip_end = (char *)mxmlElementGetAttr(node, "SrcIPEndAddr");
					if (ip_end != NULL && ip_end[0] != '\0') {
						if (!check_ip_format(ip_end)) {
							tcdbg_printf("ipfilter_write():check src end ip failed.\n");
							continue;
						}
						strcat(ipmacfilter_cmd_tail, " -m iprange --src-range ");
						strcat(ipmacfilter_cmd_tail, src_ip);
						strcat(ipmacfilter_cmd_tail, "-");
						strcat(ipmacfilter_cmd_tail, ip_end);

						strcat(ipv4filter_cmd_tail, " -p IPv4 --ip-src ");
						strcat(ipv4filter_cmd_tail, src_ip);
						strcat(ipv4filter_cmd_tail, "-");
						strcat(ipv4filter_cmd_tail, ip_end);
					}
					else{
#endif
					strcat(ipmacfilter_cmd_tail, " -s ");
					strcat(ipmacfilter_cmd_tail, src_ip);

					src_mask = (char *)mxmlElementGetAttr(node, "SrcIPMask");
					if (src_mask != NULL && src_mask[0] != '\0') {
						if (!check_mask_format(src_mask, src_mask_dec)) {
							tcdbg_printf("ipfilter_write():check src ip mask failed.\n");
							continue;
						}

						strcat(ipmacfilter_cmd_tail, "/");
						strcat(ipmacfilter_cmd_tail, src_mask_dec);
					}
#if defined(TCSUPPORT_CMCC)
					}
#endif
				}

				/* des ip address */
				des_ip = (char *)mxmlElementGetAttr(node, "DesIPAddr");
				if (des_ip != NULL && des_ip[0] != '\0') {
					if (!check_ip_format(des_ip)) {
						tcdbg_printf("ipfilter_write():check des ip failed.\n");
						continue;
					}
#if defined(TCSUPPORT_CMCC)
					ip_end = (char *)mxmlElementGetAttr(node, "DesIPEndAddr");
					if (ip_end != NULL && ip_end[0] != '\0') {
						if (!check_ip_format(ip_end)) {
							tcdbg_printf("ipfilter_write():check des end ip failed.\n");
							continue;
						}
						strcat(ipmacfilter_cmd_tail, " -m iprange --dst-range ");
						strcat(ipmacfilter_cmd_tail, des_ip);
						strcat(ipmacfilter_cmd_tail, "-");
						strcat(ipmacfilter_cmd_tail, ip_end);

						strcat(ipv4filter_cmd_tail, " --ip-dst ");
						strcat(ipv4filter_cmd_tail, des_ip);
						strcat(ipv4filter_cmd_tail, "-");
						strcat(ipv4filter_cmd_tail, ip_end);
					}
					else{
#endif
					strcat(ipmacfilter_cmd_tail, " -d ");
					strcat(ipmacfilter_cmd_tail, des_ip);

					des_mask = (char *)mxmlElementGetAttr(node, "DesIPMask");
					if (des_mask != NULL && des_mask[0] != '\0') {
						if (!check_mask_format(des_mask, des_mask_dec)) {
							tcdbg_printf("ipfilter_write():check des ip mask failed.\n");
							continue;
						}

						strcat(ipmacfilter_cmd_tail, "/");
						strcat(ipmacfilter_cmd_tail, des_mask_dec);
					}
#if defined(TCSUPPORT_CMCC)
					}
#endif
				}
			#if 0	
				if (!strcasecmp(direction, "Incoming")) {
					//sprintf(ipmacfilter_cmd_tail,"%s%s%s%s", ipmacfilter_cmd_head, " -i ", interface, ipmacfilter_cmd_tail);
					strcat(ipmacfilter_cmd_tail, " -i ");
					strcat(ipmacfilter_cmd_tail, interface);
				}
				else if (!strcasecmp(direction, "Outgoing")) {
					//sprintf(ipmacfilter_cmd_tail,"%s%s%s%s", ipmacfilter_cmd_head, " -i ", interface, ipmacfilter_cmd_tail);
					strcat(ipmacfilter_cmd_tail, " -i ");
					strcat(ipmacfilter_cmd_tail, interface);
				}
				else{
					sprintf(ipmacfilter_cmd_tail,"%s%s%s%s%s%s%s%s", ipmacfilter_cmd_head, "-i ", interface, ipmacfilter_cmd_tail, ipmacfilter_cmd_head, "-o ", interface, ipmacfilter_cmd_tail);
				}
			#else
				if (interface && interface[0] != '\0') {
					strcat(ipmacfilter_cmd_tail, " -i ");
					strcat(ipmacfilter_cmd_tail, interface);
				}
			#endif

				/* ip protocol tcp/udp/icmp/... */
				protocol = (char *)mxmlElementGetAttr(node, "Protocol");
				if (protocol != NULL && protocol[0] != '\0') {
					if (!strcasecmp(protocol, "TCP") ||
						 !strcasecmp(protocol, "UDP") ||
						 !strcasecmp(protocol, "ICMP")) {
						strcat(ipmacfilter_cmd_tail, " -p ");
						strcat(ipmacfilter_cmd_tail, protocol);
#if defined(TCSUPPORT_CMCC)
						if (src_ip != NULL && src_ip[0] != '\0') {
							strcat(ipv4filter_cmd_tail, " --ip-proto ");
						}
						else{
							strcat(ipv4filter_cmd_tail, " -p IPv4 --ip-proto ");
						}
						strcat(ipv4filter_cmd_tail, protocol);
#endif
						
						/* if its not icmp protocol */
						if (0 != strcasecmp(protocol, "ICMP")) {
							/* source  port */
							src_port = (char *)mxmlElementGetAttr(node, "SrcPort"); 
							if (src_port != NULL && src_port[0] != '\0') {
								if (atoi(src_port) < 0 || atoi(src_port) > 65535) {
									tcdbg_printf("ipfilter_write():check src port failed.\n");
									continue;
								}
								
								strcat(ipmacfilter_cmd_tail, " --sport ");
								strcat(ipmacfilter_cmd_tail, src_port);
#if defined(TCSUPPORT_CMCC)
								strcat(ipv4filter_cmd_tail, " --ip-sport ");
								strcat(ipv4filter_cmd_tail, src_port);
								src_port = (char *)mxmlElementGetAttr(node, "SrcPortEnd"); 
								if (src_port != NULL && src_port[0] != '\0') {
									if (atoi(src_port) < 0 || atoi(src_port) > 65535) {
										tcdbg_printf("ipfilter_write():check src portend failed.\n");
										continue;
									}
									strcat(ipmacfilter_cmd_tail, ":");
									strcat(ipmacfilter_cmd_tail, src_port);
									strcat(ipv4filter_cmd_tail, ":");
									strcat(ipv4filter_cmd_tail, src_port);
								}
#endif
							}

							/* des port */
							des_port = (char *)mxmlElementGetAttr(node, "DesPort");
							if (des_port != NULL && des_port[0] != '\0') {
								if (atoi(des_port) < 0 || atoi(des_port) > 65535) {
									tcdbg_printf("ipfilter_write():check des port failed.\n");
									continue;
								}

								strcat(ipmacfilter_cmd_tail, " --dport ");
								strcat(ipmacfilter_cmd_tail, des_port);
#if defined(TCSUPPORT_CMCC)
								strcat(ipv4filter_cmd_tail, " --ip-dport ");
								strcat(ipv4filter_cmd_tail, des_port);
								des_port = (char *)mxmlElementGetAttr(node, "DesPortEnd");
								if (des_port != NULL && des_port[0] != '\0') {
									if (atoi(des_port) < 0 || atoi(des_port) > 65535) {
										tcdbg_printf("ipfilter_write():check des portend failed.\n");
										continue;
									}
									strcat(ipmacfilter_cmd_tail, ":");
									strcat(ipmacfilter_cmd_tail, des_port);
									strcat(ipv4filter_cmd_tail, ":");
									strcat(ipv4filter_cmd_tail, des_port);
								}
#endif
							}
						}
				
						sprintf(ipmacfilter_cmd, "%s%s", ipmacfilter_cmd_head, ipmacfilter_cmd_tail);
						
						strcat(ipmacfilter_cmd, " -j ");
						strcat(ipmacfilter_cmd, target);
						strcat(ipmacfilter_cmd, "\n");

						fputs(ipmacfilter_cmd, fp);
#if defined(TCSUPPORT_CMCC)
						sprintf(ipv4filter_cmd, "%s%s", ipv4filter_cmd_head, ipv4filter_cmd_tail);
						
						strcat(ipv4filter_cmd, " -j ");
						strcat(ipv4filter_cmd, target_ipv4);
						strcat(ipv4filter_cmd, "\n");			
						fputs(ipv4filter_cmd, fp);
#endif
					}
					else if (!strcasecmp(protocol, "TCP/UDP")) {
						/* tcp */
						strcat(ipmacfilter_cmd_tail, " -p ");
						sprintf(ipmacfilter_cmd, "%s%s%s", ipmacfilter_cmd_head, ipmacfilter_cmd_tail, "TCP");
#if defined(TCSUPPORT_CMCC)
						if (src_ip != NULL && src_ip[0] != '\0') {
							strcat(ipv4filter_cmd_tail, " --ip-proto ");
						}
						else{
							strcat(ipv4filter_cmd_tail, " -p IPv4 --ip-proto ");
						}
						sprintf(ipv4filter_cmd, "%s%s%s", ipv4filter_cmd_head, ipv4filter_cmd_tail, "TCP");
#endif

						/* source  port */
						src_port = (char *)mxmlElementGetAttr(node, "SrcPort"); 
						if (src_port != NULL && src_port[0] != '\0') {
							if (atoi(src_port) < 0 || atoi(src_port) > 65535) {
								tcdbg_printf("ipfilter_write():(tcp)check src port failed.\n");
								continue;
							}
							
							strcat(ipmacfilter_cmd, " --sport ");
							strcat(ipmacfilter_cmd, src_port);
#if defined(TCSUPPORT_CMCC)
							strcat(ipv4filter_cmd, " --ip-sport ");
							strcat(ipv4filter_cmd, src_port);
							src_port = (char *)mxmlElementGetAttr(node, "SrcPortEnd"); 
							if (src_port != NULL && src_port[0] != '\0') {
								if (atoi(src_port) < 0 || atoi(src_port) > 65535) {
									tcdbg_printf("ipfilter_write():(TCP/UDP-tcp)check src portend failed.\n");
									continue;
								}								
								strcat(ipmacfilter_cmd, ":");
								strcat(ipmacfilter_cmd, src_port);
								strcat(ipv4filter_cmd, ":");
								strcat(ipv4filter_cmd, src_port);
							}
#endif
						}

						/* des port */
						des_port = (char *)mxmlElementGetAttr(node, "DesPort");
						if (des_port != NULL && des_port[0] != '\0') {
							if (atoi(des_port) < 0 || atoi(des_port) > 65535) {
								tcdbg_printf("ipfilter_write():(tcp)check des port failed.\n");
								continue;
							}

							strcat(ipmacfilter_cmd, " --dport ");
							strcat(ipmacfilter_cmd, des_port);
#if defined(TCSUPPORT_CMCC)
							strcat(ipv4filter_cmd, " --ip-dport ");
							strcat(ipv4filter_cmd, des_port);
							des_port = (char *)mxmlElementGetAttr(node, "DesPortEnd");
							if (des_port != NULL && des_port[0] != '\0') {
								if (atoi(des_port) < 0 || atoi(des_port) > 65535) {
									tcdbg_printf("ipfilter_write():(TCP/UDP-tcp)check des portend failed.\n");
									continue;
								}							
								strcat(ipmacfilter_cmd, ":");
								strcat(ipmacfilter_cmd, des_port);
								strcat(ipv4filter_cmd, ":");
								strcat(ipv4filter_cmd, des_port);
							}
#endif
						}
							
						strcat(ipmacfilter_cmd, " -j ");
						strcat(ipmacfilter_cmd, target);
						strcat(ipmacfilter_cmd, "\n");
						
						fputs(ipmacfilter_cmd, fp);
#if defined(TCSUPPORT_CMCC)						
						strcat(ipv4filter_cmd, " -j ");
						strcat(ipv4filter_cmd, target_ipv4);
						strcat(ipv4filter_cmd, "\n");
						
						fputs(ipv4filter_cmd, fp);
#endif

						/* udp */
						sprintf(ipmacfilter_cmd, "%s%s%s", ipmacfilter_cmd_head, ipmacfilter_cmd_tail, "UDP");
#if defined(TCSUPPORT_CMCC)
						sprintf(ipv4filter_cmd, "%s%s%s", ipv4filter_cmd_head, ipv4filter_cmd_tail, "UDP");
#endif

						/* source  port */
						src_port = (char *)mxmlElementGetAttr(node, "SrcPort"); 
						if (src_port != NULL && src_port[0] != '\0') {
							if (atoi(src_port) < 0 || atoi(src_port) > 65535) {
								tcdbg_printf("ipfilter_write():(udp)check src port failed.\n");
								continue;
							}
							
							strcat(ipmacfilter_cmd, " --sport ");
							strcat(ipmacfilter_cmd, src_port);
#if defined(TCSUPPORT_CMCC)
							strcat(ipv4filter_cmd, " --ip-sport ");
							strcat(ipv4filter_cmd, src_port);
							src_port = (char *)mxmlElementGetAttr(node, "SrcPortEnd"); 
							if (src_port != NULL && src_port[0] != '\0') {
								if (atoi(src_port) < 0 || atoi(src_port) > 65535) {
									tcdbg_printf("ipfilter_write():(TCP/UDP-udp)check src portend failed.\n");
									continue;
								}								
								strcat(ipmacfilter_cmd, ":");
								strcat(ipmacfilter_cmd, src_port);
								strcat(ipv4filter_cmd, ":");
								strcat(ipv4filter_cmd, src_port);
							}
#endif
						}

						/* des port */
						des_port = (char *)mxmlElementGetAttr(node, "DesPort");
						if (des_port != NULL && des_port[0] != '\0') {
							if (atoi(des_port) < 0 || atoi(des_port) > 65535) {
								tcdbg_printf("ipfilter_write():(udp)check des port failed.\n");
								continue;
							}

							strcat(ipmacfilter_cmd, " --dport ");
							strcat(ipmacfilter_cmd, des_port);
#if defined(TCSUPPORT_CMCC)
							strcat(ipv4filter_cmd, " --ip-dport ");
							strcat(ipv4filter_cmd, des_port);
							des_port = (char *)mxmlElementGetAttr(node, "DesPortEnd");
							if (des_port != NULL && des_port[0] != '\0') {
								if (atoi(des_port) < 0 || atoi(des_port) > 65535) {
									tcdbg_printf("ipfilter_write():(TCP/UDP-udp)check des portend failed.\n");
									continue;
								}
								strcat(ipmacfilter_cmd, ":");
								strcat(ipmacfilter_cmd, des_port);
								strcat(ipv4filter_cmd, ":");
								strcat(ipv4filter_cmd, des_port);
							}
#endif
						}
						
						strcat(ipmacfilter_cmd, " -j ");
						strcat(ipmacfilter_cmd, target);
						strcat(ipmacfilter_cmd, "\n");
						
						fputs(ipmacfilter_cmd, fp);
#if defined(TCSUPPORT_CMCC)
						
						strcat(ipv4filter_cmd, " -j ");
						strcat(ipv4filter_cmd, target_ipv4);
						strcat(ipv4filter_cmd, "\n");	
						fputs(ipv4filter_cmd, fp);
#endif
					}
					else {
						continue;
					}

				}

				else {
					sprintf(ipmacfilter_cmd, "%s%s", ipmacfilter_cmd_head, ipmacfilter_cmd_tail);
					
					strcat(ipmacfilter_cmd, " -j ");
					strcat(ipmacfilter_cmd, target);
					strcat(ipmacfilter_cmd, "\n");
					
					fputs(ipmacfilter_cmd, fp);
#if defined(TCSUPPORT_CMCC)
					sprintf(ipv4filter_cmd, "%s%s", ipv4filter_cmd_head, ipv4filter_cmd_tail);
					
					strcat(ipv4filter_cmd, " -j ");
					strcat(ipv4filter_cmd, target_ipv4);
					strcat(ipv4filter_cmd, "\n");			
					fputs(ipv4filter_cmd, fp);
#endif
				}

			}else if(!strcasecmp(value, "MAC")){

				strcpy(target, target_mac);
				fp = fp_mac;
				if (fp == NULL) {
					tcdbg_printf("mac failed");
					continue;
				}
				strcpy(ipmacfilter_cmd_head, "iptables -t filter -A macfilter_chain");
				strcpy(ebmacfilter_cmd_head, "ebtables -t filter -A ebmacfileter");
#if defined(TCSUPPORT_CMCC)
				strcpy(macfilter_cmd_head, "ebtables -t filter -A ebmacfileter_chain");
#endif
				
				mac_addr = mxmlElementGetAttr(node, "MacAddr");
				if(mac_addr==NULL){
					continue;
				}
				if(!check_mac_format(mac_addr)){
					fprintf(stderr, "Error--Mac Address is invalide.\n");
					continue;
				}else{
					/*compose iptables command*/
					strcpy(ipmacfilter_cmd_tail,  " -m mac --mac-source ");
					strcat(ipmacfilter_cmd_tail, mac_addr);
					strcat(ipmacfilter_cmd_tail, " -j ");
					strcat(ipmacfilter_cmd_tail, target);
					strcat(ipmacfilter_cmd_tail, "\n");

					strcpy(ebmacfilter_cmd_tail_ipv4,  " -s ");
					strcat(ebmacfilter_cmd_tail_ipv4, mac_addr);
				    	strcat(ebmacfilter_cmd_tail_ipv4,  " -p IPv4 --ip-proto 17 --ip-dport 67 -j ");
					strcat(ebmacfilter_cmd_tail_ipv4, target);				
					strcat(ebmacfilter_cmd_tail_ipv4, "\n");

					strcpy(ebmacfilter_cmd_tail_ipv6,  " -s ");
					strcat(ebmacfilter_cmd_tail_ipv6, mac_addr);
				    	strcat(ebmacfilter_cmd_tail_ipv6,  " -p IPv6 --ip6-proto 17 --ip6-dport 547 -j ");
					strcat(ebmacfilter_cmd_tail_ipv6, target);				
					strcat(ebmacfilter_cmd_tail_ipv6, "\n");

#if defined(TCSUPPORT_CMCC)
					strcpy(smacfilter_cmd_tail,  " -s ");
					strcat(smacfilter_cmd_tail, mac_addr);
					strcat(smacfilter_cmd_tail, " -p IPv4 -j ");
					strcat(smacfilter_cmd_tail, target);
					strcat(smacfilter_cmd_tail, "\n");
					
					strcpy(dmacfilter_cmd_tail,  " -d ");
					strcat(dmacfilter_cmd_tail, mac_addr);
					strcat(dmacfilter_cmd_tail, " -p IPv4 -j ");
					strcat(dmacfilter_cmd_tail, target);
					strcat(dmacfilter_cmd_tail, "\n");
#endif
				}

			#if 0
				if(!strcasecmp(direction, "Incoming")){
					sprintf(ipmacfilter_cmd,"%s%s%s%s", ipmacfilter_cmd_head, " -i ", interface, ipmacfilter_cmd_tail);
				}
				else if(!strcasecmp(direction, "Outgoing")){
					sprintf(ipmacfilter_cmd,"%s%s%s%s", ipmacfilter_cmd_head, " -i ", interface, ipmacfilter_cmd_tail);
				}
			
				else{
					sprintf(ipmacfilter_cmd,"%s%s%s%s%s%s%s%s", ipmacfilter_cmd_head, "-i ", interface, ipmacfilter_cmd_tail, ipmacfilter_cmd_head, "-o ", interface, ipmacfilter_cmd_tail);
				}
			#else
				if (interface && interface[0] != '\0') {
					if (!strcmp(interface, "LAN")) {
						strcpy(interface, "br0");
					}
					sprintf(ipmacfilter_cmd,"%s%s%s%s", ipmacfilter_cmd_head, " -i ", interface, ipmacfilter_cmd_tail);
					sprintf(ebmacfilter_cmd_ipv4,"%s%s", ebmacfilter_cmd_head, ebmacfilter_cmd_tail_ipv4);
					sprintf(ebmacfilter_cmd_ipv6,"%s%s", ebmacfilter_cmd_head, ebmacfilter_cmd_tail_ipv6);
#if defined(TCSUPPORT_CMCC)
					sprintf(smacfilter_cmd,"%s%s", macfilter_cmd_head, smacfilter_cmd_tail);
					sprintf(dmacfilter_cmd,"%s%s", macfilter_cmd_head, dmacfilter_cmd_tail);
#endif
				}
			#endif

				fputs(ipmacfilter_cmd, fp);
				fputs(ebmacfilter_cmd_ipv4, fp);
				fputs(ebmacfilter_cmd_ipv6, fp);
#if defined(TCSUPPORT_CMCC)
				fputs(smacfilter_cmd, fp);
				fputs(dmacfilter_cmd, fp);
#endif
#if defined(TCSUPPORT_CT_PON_C7)
				sprintf(ipmacfilter_cmd,"iptables -t filter -A dhcpfilter_chain -i %s -m mac --mac-source %s -p udp --dport 67 -j %s \n", interface, mac_addr, target);		
				fputs(ipmacfilter_cmd, fp);
#endif
				//fprintf(stderr,"ipmacfilter_cmd = %s\n", ipmacfilter_cmd);
			}else{
				/*not valid RuleType value, jump to next rule*/
				continue;
			}
		}else{
			fprintf(stderr, "Error--Entry RuleType value is NULL.\n");
			continue;
		}
	}
	#if 0
	if(!strcasecmp(list_type, "black")){

		/*accept all connections which not in black list*/
		fputs("iptables -A ipfilter_chain -j ACCEPT\n", fp);

	}else
	#endif
	if (mac_ip == 0) {
		/* for mac filter */
		if (mac_on_off && mac_on_off[0] == '1' && list_type_mac) {
			if(!strcasecmp(list_type_mac, "white")){
				/*deny all connections which not in white list*/
				fputs("iptables -A macfilter_chain -i br0 -j DROP\n", fp_mac);
			}
		}
#if defined(TCSUPPORT_RA_HWNAT)
		fputs("/userfs/bin/hw_nat -!\n", fp_mac); // clean all hwnat rules.
#endif
		fclose(fp_mac);
		chmod(MACFILTER_SH, 777);
	}
	else if (mac_ip == 1) {
		/* for ip uplink filter */
		if (ipup_on_off && ipup_on_off[0] == '1' && list_type_ipup) {
			if(!strcasecmp(list_type_ipup, "white")){
				/*deny all connections which not in white list*/
				fputs("iptables -A ipupfilter_chain -i br0 -j DROP\n", fp_ipup);
#if defined(TCSUPPORT_CMCC)
				fputs("ebtables -A ipv4upfilter -p IPv4 -i eth0.+ -j DROP\n", fp_ipup);
#endif
			}
		}
#if defined(TCSUPPORT_RA_HWNAT)
		fputs("/userfs/bin/hw_nat -!\n", fp_ipup); // clean all hwnat rules.
#endif
		fclose(fp_ipup);
		chmod(IPUPFILTER_SH, 777);
	}
	else if (mac_ip == 2) {
		/* for ip down link filter */
		if (ipdown_on_off && ipdown_on_off[0] == '1' && list_type_ipdown) {
			if(!strcasecmp(list_type_ipdown, "white")){
				/*deny all connections which not in white list*/
				fputs("iptables -A ipdownfilter_chain -i ! br0 -j DROP\n", fp_ipdown);
#if defined(TCSUPPORT_CMCC)
				fputs("ebtables -A ipv4downfilter -p IPv4 -o eth0.+ -j DROP\n", fp_ipdown);
#endif
			}
		}
#if defined(TCSUPPORT_RA_HWNAT)
		fputs("/userfs/bin/hw_nat -!\n", fp_ipdown); // clean all hwnat rules.
#endif
		fclose(fp_ipdown);
		chmod(IPDOWNFILTER_SH, 777);
	}
	if (g_ip_mac_flag == 0) {
		goto next_ip_mac_handle;
	}
	else {
		return SUCCESS;
	}
}/* end ipmacfilter_write */

/*______________________________________________________________________________
**	ipmacfilter_verify
**
**	descriptions:
**		To check the system parameter of acl element is valide or not.
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
ipfilter_verify(mxml_node_t *node){
	return SUCCESS;
}/* end ipmacfilter_verify */

/*_____________________________________________________________________________
**      function name: ipmacfilter_execute
**      descriptions:
**            To execute ipmacfilter node service.
**
**      parameters:
**            node:   Put the ipmacfilter of cfg node tree that is used to execute ipmacfilter settings.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**____________________________________________________________________________
*/
int
ipfilter_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	unsigned int new_filter_state=0;
	int ipmacfilter_flag=0;
	ipmacfilter_flag = 1/*ipfilter_check_active(top)*/;

#if defined(TCSUPPORT_CT_E8GUI)
	int ipmacfilter_switch = 0;
	ipmacfilter_switch = ipfilter_check_switch(top);
	if((ipmacfilter_flag==1) && (ipmacfilter_switch == 1)){
#else
	if(ipmacfilter_flag==1){
#endif
		/*check if iptable_filter module is loaded or not*/
//		setnew_filter_state(top,ipfilter_check_filter,IPFILTER_VECTOR,&new_filter_state);
		if(ipfilter_check_filter(top)){//filter on
			new_filter_state=pre_sys_state.filter_m_state|IPFILTER_VECTOR;
		}
		else{//filter down
			new_filter_state=pre_sys_state.filter_m_state&(~IPFILTER_VECTOR);
		}
		//     		pre_sys_state.filter_m_state = check_and_set_filter(top,pre_sys_state.filter_m_state);
		pre_sys_state.filter_m_state = check_and_set_filter(new_filter_state);
	}else{
		/*remove iptable settings*/
		//system(IPMACFILTER_STOP_SH);
		/*check if iptable_filter module is removed or not*/
//		setnew_filter_state(top,ipfilter_check_filter,IPFILTER_VECTOR,&new_filter_state);
		if(ipfilter_check_filter(top)){//filter on
			new_filter_state=pre_sys_state.filter_m_state|IPFILTER_VECTOR;
		}
		else{//filter down
			new_filter_state=pre_sys_state.filter_m_state&(~IPFILTER_VECTOR);
		}
		//     		pre_sys_state.filter_m_state = check_and_set_filter(top,pre_sys_state.filter_m_state);
		pre_sys_state.filter_m_state = check_and_set_filter(new_filter_state);
	}
        /*set iptable settings*/		
	system(MACFILTER_SH);
	system(IPUPFILTER_SH);
	system(IPDOWNFILTER_SH);
	return 0;
}/* end ipmacfilter_execute*/
/**********20080926 racing add************/
/*______________________________________________________________________________
**	ipfilter_check_filter
**
**	descriptions:
**		check if the ipfilter filter need filter module or not.
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
ipfilter_check_filter(mxml_node_t *top){
	int filter_on=0;
	filter_on = ipfilter_check_active(top);
	return filter_on;
}

#if defined(TCSUPPORT_CT_E8GUI)
/*_____________________________________________________________________________
*name:
	ipfilter_read
*description:
*	Read ipfilter configure node tree data and then set add_num attributes value and compose a temp file which contains ipmac filter info for webpage display
*param:
*	*top: root of mxml_node_t structure
*	*attr: attribute of url filter cfg node
*global:
*	none
*return int:
*	success 0
*	otherwise -1
*call:
*	none
*revision:
*	1.0 dgk 20101020
*
*_____________________________________________________________________________
*/
const char
ipmac_attribute[][ATTR_SIZE]=
{
	{"MacName"},
	{"MacAddr"},
	{"Active"},
	{"Interface"},
	{"IPName"},
	{"Protocol"},
	{"SrcIPAddr"},
	{"SrcIPMask"},
	{"SrcPort"},
	{"DesIPAddr"},
	{"DesIPMask"},
	{"DesPort"},
#if defined(TCSUPPORT_CMCC)
	{"SrcIPEndAddr"},
	{"DesIPEndAddr"},
	{"SrcPortEnd"},
	{"DesPortEnd"},
#endif
	{""},
};
int
ipfilter_read(mxml_node_t *top, char nodeName[][MAX_NODE_NAME], char *attr){
	char ipmacNodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	mxml_node_t *node = NULL;   /* <element node> */
	mxml_node_t *group = NULL;  /* <IpMacFilter Node> */
	int 	static_ip_index = 0;//	static_url_index = 0;
	char 	addnum[4] = {0};
	char 	count[4] = {0};
	int 	norecord = 0;
	int	i, start_index, end_index, ip_flag = -1;
	int	j, s_attr = 2, e_attr = 12;
	char	cipmac[50] = {0};

	FILE 	*fp = NULL;
	char 	*p = NULL;
	if(!strcmp(attr,"mac_num") || !strcmp(attr,"add_macnum"))
	{
		ip_flag = 0;
		start_index = 0;
		end_index = MAX_MAC_FILTER_RULE;
		s_attr = 0; e_attr = 4;
	}
	else if(!strcmp(attr,"ipup_num") || !strcmp(attr,"add_ipupnum"))
	{
		ip_flag = 1;
		start_index = MAX_MAC_FILTER_RULE;
		end_index = MAX_MAC_FILTER_RULE + MAX_IPUP_FILTER_RULE;
#if defined(TCSUPPORT_CMCC)
		e_attr = 16;
#endif
	}
	else if(!strcmp(attr,"ipdown_num") || !strcmp(attr,"add_ipdownnum"))
	{
		ip_flag = 2;
		start_index = MAX_MAC_FILTER_RULE + MAX_IPUP_FILTER_RULE;
		end_index = MAX_IPMACFILTER_RULE;
#if defined(TCSUPPORT_CMCC)
		e_attr = 16;
#endif

	}
	else
	{
		return SUCCESS;
	}
	
	group = mxmlFindElement(top, top, IPMACFILTER,
			NULL, NULL, MXML_DESCEND);
	if(group == NULL){
		tcdbg_printf("\r\n%s: can't find Node\n", __FUNCTION__);
		return FAIL;
	}
	if(0 == ip_flag)
		fp= fopen(TMP_MAC_FILE, "w");
	else if(1 == ip_flag)
		fp= fopen(TMP_IPUP_FILE, "w");
	else if(2 == ip_flag)
		fp= fopen(TMP_IPDOWN_FILE, "w");
		
	if(fp== NULL){
		fprintf(stderr, "%s:Error--IpMacFilter's temp file is NULL.\n", __FUNCTION__);
		tcdbg_printf(stderr);
		return FAIL;
	}
	
	memset(ipmacNodeName, 0, sizeof(ipmacNodeName));
	strcpy(ipmacNodeName[0],IPMACFILTER);
	for(i=start_index; i<end_index; i++){
		sprintf(ipmacNodeName[1],"%s%d",SUB_NODE_NAME, i);
		node = mxmlFindElement(group, group, ipmacNodeName[1],
					NULL, NULL, MXML_DESCEND);
		if(node == NULL)
		{
			/*1.find the entry index which can be used*/
			if(!norecord)
			{
				sprintf(addnum,"%d",i);
				if(0 == ip_flag)
					mxmlElementSetAttr(group, "add_macnum", addnum);
				else if(1 == ip_flag)
					mxmlElementSetAttr(group, "add_ipupnum", addnum);
				else if(2 == ip_flag)
					mxmlElementSetAttr(group, "add_ipdownnum", addnum);
				norecord++;
			}
		}
		else
		{
			memset(count,0x00, sizeof(count));
			sprintf(count,"%d\n",i);
			fputs(count, fp);
			/*2.get ipmac value and save in file*/
			for(j= s_attr; j<e_attr; j++){
				memset(cipmac,0x00, sizeof(cipmac));
				p = mxmlElementGetAttr(node, ipmac_attribute[j]);
				if(p){
					strcpy(cipmac, p);
					strcat(cipmac, "\n");
					fputs(cipmac, fp);	
					}
				else{
					fputs("\n", fp);
				}
			}
			static_ip_index++;
		}
	}
	
	fclose(fp);fp=NULL;
	memset(count,0x00, sizeof(count));
	sprintf(count,"%d",static_ip_index);
	if(0 == ip_flag){
		mxmlElementSetAttr(group, "mac_num", count);
		chmod(TMP_MAC_FILE, 777);
	}
	else if(1 == ip_flag){
		mxmlElementSetAttr(group, "ipup_num", count);
		chmod(TMP_IPUP_FILE, 777);
	}
	else if(2 == ip_flag){
		mxmlElementSetAttr(group, "ipdown_num", count);
		chmod(TMP_IPDOWN_FILE, 777);
	}
	return SUCCESS;
}
#endif
//"AppFilter", SINGLE_TYPE, NULL, NULL, NULL, app_filter_write, app_filter_verify, app_filter_execute},
/*______________________________________________________________________________
**	app_filter_boot
**
**	descriptions:
**		the boot function of app filter.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**_______________________________________________________________________________
*/
int
app_filter_boot(mxml_node_t *top){
	unsigned int new_filter_state=0;
	FILE *startupSh=NULL;
//	setnew_filter_state(top,app_check_filter,APP_VECTOR,&new_filter_state);
	if(app_check_filter(top)){//filter on
		new_filter_state=pre_sys_state.filter_m_state|APP_VECTOR;
	}
	else{//filter down
		new_filter_state=pre_sys_state.filter_m_state&(~APP_VECTOR);
	}
	check_and_set_l7filter(new_filter_state);
	pre_sys_state.filter_m_state = check_and_set_filter(new_filter_state);
	startupSh=fopen("/tmp/etc/AppFilter.sh","r");
	if(startupSh){
		fclose(startupSh);
		system("chmod 755 /tmp/etc/AppFilter.sh");
		system("cat /tmp/etc/AppFilter.sh");
		system("/tmp/etc/AppFilter.sh");
	}

	return SUCCESS;
}
/*______________________________________________________________________________
**	app_filter_init
**
**	descriptions:
**		the init function of app_filter.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
app_filter_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="AppFilter",
			.type=SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=app_filter_write,
			.cfg_verify=app_filter_verify,
			.cfg_execute=app_filter_execute,
			.cfg_boot=app_filter_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}
/**********20081021 krammer add************/

/*_____________________________________________________________________________
*description:
*	Read app filter configure node tree data and then compose a shell script which contains iptables commands.
*param:
*	*top: root of mxml_node_t structure
*	*parant: parant node of app filter cfg node
*global:
*	none
*return int:
*	success 0
*	otherwise -1
*call:
*	none
*revision:
*	1.0 krammer
*_____________________________________________________________________________
*/
int
app_filter_write(mxml_node_t *top, mxml_node_t *parant){
	int blockAttributeIndex=0;
	char blockAttributeName[][15]=
	{
		"Block_ICQ","Block_MSN","Block_YMSG","Block_RTSP"
	};
	char patternName[][20]=
	{
		"aim","msnmessenger","yahoo","rtsp"
	};
	const char* attributeValue=NULL;
	const char* Activate=NULL;
	char iptableCmd[100]={0};
	FILE *fp;
	mxml_node_t *node = NULL;

	/*node = AppFilter_Entry*/
	node = mxmlFindElement(parant, parant, SUB_NODE_NAME, NULL , NULL, MXML_DESCEND);
	if(node == NULL){
		fprintf(stderr, "Error--AppFilter Entry is NULL.\n");
		return FAIL;
	}

	/*get Activate value*/
	Activate=mxmlElementGetAttr(node, "Activate");
	if(Activate == NULL){
		fprintf(stderr, "Error--AppFilter Activate value is NULL.\n");
		return FAIL;
	}

	fp = fopen(APP_FILTER_SH, "w+");
	if(fp == NULL){
		fprintf(stderr, "Error--AppFilter's file pointer is NULL.\n");
		return FAIL;
	}

	/*appFilter is activated*/
	if(!strcmp(Activate, "1")){

		/*write the iptables cmd into AppFilter.sh*/
		fputs("iptables -t filter -F app_filter_chain\n", fp);
		fputs("iptables -t filter -Z app_filter_chain\n", fp);



		for(blockAttributeIndex=0; blockAttributeIndex<MAX_APP_RULE; blockAttributeIndex++){
			/*get a attribute according to the attribute index*/
			attributeValue=mxmlElementGetAttr(node, blockAttributeName[blockAttributeIndex]);
			/*the value must be set*/
			if(attributeValue != NULL){
				/*if the value is 1 , we write the cmd into AppFilter.sh*/
				if(!strcmp(attributeValue, "1")){
					/*setup the iptableCmd and write it into shell script*/
					sprintf(iptableCmd, "iptables -t filter -A app_filter_chain -m layer7 --l7proto %s -j DROP\n", patternName[blockAttributeIndex]);
					fputs(iptableCmd, fp);
				}
				/*if the value is neither 1 nor 0, this is a wrong setting.*/
				else if(strcmp(attributeValue, "0")){
					/*if the value is not 1 and 0, it must be a wrong attribute value*/
					fprintf(stderr, "Error--AppFilter get a wrong attribute value of %s.\n", blockAttributeName[blockAttributeIndex]);
				}
			}
			else{
				fprintf(stderr, "Error--AppFilter get a NULL attribute value of %s.\n", blockAttributeName[blockAttributeIndex]);
			}
		}
	}

	fclose(fp);

	chmod(APP_FILTER_SH ,777);
	return SUCCESS;
}
int
app_filter_verify(mxml_node_t *node){
	return SUCCESS;
}


/*_____________________________________________________________________________
*description:
*	Execute shell script composed by app_filterl_write function.
*param:
**	top: root of mxml_node_t structure
*	name[][]: record path of every configure node entries (ex. {{WAN_PVC0},{WAN_PVC1}�K})
*return int:
*	success 0
*	otherwise -1
*_____________________________________________________________________________
*/
int
app_filter_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	unsigned int new_filter_state=0;
	char appFilterState[5]={0};
	/*get the Activate value and see if it is ok*/
	if(getAttrValue(top, name, "Activate", appFilterState) != TCAPI_PROCESS_OK){
		fprintf(stderr, "Error--AppFilter get some errors when get activate value.\n");
	}
	/*if Activated, check filter state and run AppFilter.sh*/
	else if (!strcmp(appFilterState,"1")){
//		setnew_filter_state(top,app_check_filter,APP_VECTOR,&new_filter_state);
		if(app_check_filter(top)){//filter on
			new_filter_state=pre_sys_state.filter_m_state|APP_VECTOR;
		}
		else{//filter down
			new_filter_state=pre_sys_state.filter_m_state&(~APP_VECTOR);
		}
		//     		pre_sys_state.filter_m_state = check_and_set_filter(top,pre_sys_state.filter_m_state);
		check_and_set_l7filter(new_filter_state);
		pre_sys_state.filter_m_state = check_and_set_filter(new_filter_state);
		system(APP_FILTER_SH);
	}
	/*we will stop app filter if the Activate value is not 1*/
	else{
		system(APP_STOP_SH);
//		setnew_filter_state(top,app_check_filter,APP_VECTOR,&new_filter_state);
		if(app_check_filter(top)){//filter on
			new_filter_state=pre_sys_state.filter_m_state|APP_VECTOR;
		}
		else{//filter down
			new_filter_state=pre_sys_state.filter_m_state&(~APP_VECTOR);
		}
		//     		pre_sys_state.filter_m_state = check_and_set_filter(top,pre_sys_state.filter_m_state);
		check_and_set_l7filter(new_filter_state);
		pre_sys_state.filter_m_state = check_and_set_filter(new_filter_state);
	}
	return SUCCESS;
}
/*______________________________________________________________________________
**	app_check_filter
**
**	descriptions:
**		check if the app filter need filter module or not.
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
app_check_filter(mxml_node_t *top){
	int filter_on=0;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char appfilter_status[5]={0};
	/*check app filter state*/

	/*flush the buffer*/
	memset(nodeName,0,sizeof(nodeName));
	/*build the node name = AppFilter_Entry*/
	strcpy(nodeName[0], APPFILTER);
	strcpy(nodeName[1], SUB_NODE_NAME);

	/*check the Activate value*/
	if(getAttrValue(top, nodeName, "Activate", appfilter_status) != TCAPI_PROCESS_OK){
		fprintf(stderr,"get app filter activate error in check filter state\n");
	}
	/*see if appfilter's attribute(Activate) = 1*/
	else if(!strcmp(appfilter_status,"1")){
		filter_on=1;/*load the iptable_filter module*/
	}
	return filter_on;
}
//"UrlFilter", ADV_FLAG|ENTRY_TYPE|MAX_URL_FILTER_RULE, url_filter_adv_table, url_filter_merge_table, NULL, url_filter_write, url_filter_verify, url_filter_execute},
char *
url_filter_adv_table[MAX_NODE_NAME]=
{
	"Common","",
};
char *
url_filter_merge_table[MAX_NODE_NAME]=
{
	"url_filter_id","",
};

/*______________________________________________________________________________
**	url_boot
**
**	descriptions:
**		the boot function of url.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**_______________________________________________________________________________
*/
int
url_filter_boot(mxml_node_t *top){
	unsigned int new_filter_state=0;
	FILE *startupSh=NULL;
//	setnew_filter_state(top,url_check_filter,URL_VECTOR,&new_filter_state);
	if(url_check_filter(top)){//filter on
		new_filter_state=pre_sys_state.filter_m_state|URL_VECTOR;
	}
	else{//filter down
		new_filter_state=pre_sys_state.filter_m_state&(~URL_VECTOR);
	}
	check_and_set_l7filter(new_filter_state);
	pre_sys_state.filter_m_state = check_and_set_filter(new_filter_state);
	startupSh=fopen("/tmp/etc/UrlFilter.sh","r");
	if(startupSh){
		fclose(startupSh);
		system("chmod 755 /tmp/etc/UrlFilter.sh");
		system("/tmp/etc/UrlFilter.sh");
	}

#if defined(TCSUPPORT_CT_DNSBIND)
	restart_dnsmasq();
#endif
	return SUCCESS;
}
/*______________________________________________________________________________
**	url_filter_init
**
**	descriptions:
**		the init function of url_filter.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
url_filter_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="UrlFilter",
#if defined(TCSUPPORT_CT_E8GUI)
			.type=ADV_FLAG|ENTRY_TYPE|MAX_URL_RULE|UPDATE_TYPE,
#else
			.type=ADV_FLAG|ENTRY_TYPE|MAX_URL_FILTER_RULE,
#endif
			.adv_table=url_filter_adv_table,
			.merge_table=url_filter_merge_table,
#if defined(TCSUPPORT_CT_E8GUI)
			.cfg_read=url_filter_read,
#else
			.cfg_read=NULL,
#endif
			.cfg_write=url_filter_write,
			.cfg_verify=url_filter_verify,
			.cfg_execute=url_filter_execute,
			.cfg_boot=url_filter_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}

/*_____________________________________________________________________________
*description:
*	Read url configure node tree data and then compose a shell script which contains iptables commands.
*param:
*	*top: root of mxml_node_t structure
*	*parant: parant node of url filter cfg node
*global:
*	none
*return int:
*	success 0
*	otherwise -1
*call:
*	none
*revision:
*	1.0 krammer
*
*_____________________________________________________________________________
*/
#if !defined(TCSUPPORT_CT_URL_FILTER)
int
url_filter_write(mxml_node_t *top, mxml_node_t *parant){
	int i=0,j=0,k=0,isSpecial=0,activeRule=0;
	int firstRule=0;

	char specialCh[SPCHNUMBER]={'^','$','?','.','-','*','+','|'};
	char nodeName[MAX_NODE_NAME]={0};
	char temp[MAX_URL_INPUT_LENGTH*2+20]={0};
	char hostName[MAX_URL_INPUT_LENGTH*2+20]={0};
	char pattern[MAX_URL_INPUT_LENGTH*2+20]={0};
	char or_op[1]="";
	const char* Activate=NULL;
	const char* URL=NULL;

	FILE *fp1;
	FILE *fp2;
	mxml_node_t *node = NULL;

#if defined(TCSUPPORT_CT_E8GUI)
	FILE *fp3 = NULL;
	FILE *fp_clear = NULL;
	int index = 0;
	unsigned int flag[MAX_URL_RULE/9+1] = {0};
#endif

	/*set node to be UrlFilter_Common*/
	node = mxmlFindElement(parant, parant, STRING_COMMON, NULL , NULL, MXML_DESCEND);
	if(node == NULL){
		fprintf(stderr, "Error--UrlFilter Common is NULL.\n");
		return FAIL;
	}
	/*get UrlFilter_Common Activate value*/
	Activate=mxmlElementGetAttr(node, "Activate");
	if(Activate == NULL){
		fprintf(stderr, "Error--UrlFilter_Common Activate value is NULL.\n");
		return FAIL;
	}

#if defined(TCSUPPORT_CT_E8GUI)
	const char *list_type = NULL;
	const char *pAttr = NULL;
	char	strDel[300] = {0};
	char	target[5] = {0};
	char	cmdTemp[100] = {0};
	list_type = mxmlElementGetAttr(node, "Filter_Policy");
	if(list_type == NULL)
	{
		fprintf(stderr, "Error--UrlFilter_Common Filter_Policy value is NULL.\n");
		tcdbg_printf(stderr);
		return FAIL;
	}
	//delete rules at first
	strcpy(target,list_type);
	url_filter_deletewhiterules(atoi(target));
	//check attributea of ction and do unset action
	pAttr = mxmlElementGetAttr(node, "Action");
	if(pAttr != NULL){
		if(!strcmp(pAttr, "Del")){
			pAttr = NULL;
			pAttr = mxmlElementGetAttr(node, "DeleteIndex");
			if(pAttr != NULL){
				strcpy(strDel, pAttr);
				unset_action(strDel, DEL_URL);
			}
		}			
	}
#endif
	/*open UrlFilter.sh*/
	fp1= fopen(URL_FILTER_SH, "w+");
	if(fp1== NULL){
		fprintf(stderr, "Error--UrlFilter's file pointer is NULL.\n");
		return FAIL;
	}
	/*open Url.pat*/
	fp2= fopen(URL_PATTER_PATH, "w+");
	if(fp2== NULL){
		fclose(fp1);
		fprintf(stderr, "Error--Url pattern's file pointer is NULL.\n");
		return FAIL;
	}

	/*do when url filter is activated*/
	if(!strcmp(Activate,"1")){
		/*write UrlFilter.sh*/
#if defined(TCSUPPORT_CT_E8GUI)
		if(strcmp(list_type, "1"))//puts when is not white policy
		{
			fputs("iptables -t filter -F url_filter_chain\n", fp1);
			fputs("iptables -t filter -Z url_filter_chain\n", fp1);
		}
#else
		fputs("iptables -t filter -F url_filter_chain\n", fp1);
		fputs("iptables -t filter -Z url_filter_chain\n", fp1);
#endif

#if 0
		fputs("iptables -t filter -A url_filter_chain -m layer7 --l7proto Url -j DROP\n",fp1);
#endif

		/*write Url.pat*/
		fputs("Url\n", fp2);

		/*write depend on each rule*/
		for(i=0;i<MAX_URL_RULE;i++){
			memset(pattern,0,sizeof(pattern));
			memset(hostName,0,sizeof(hostName));
			memset(temp,0,sizeof(temp));
			strcpy(or_op,"|");
#if defined(TCSUPPORT_CT_E8GUI)
			if((i>8) && (i%9 ==0)){
				index = i /9;
				if(strcmp(list_type, "1")){ //black list
					sprintf(temp, "iptables -t filter -D FORWARD -p TCP -m multiport --dport http -j url%d_filter_chain\n", index);
					fputs(temp,fp1);
					sprintf(temp, "iptables -t filter -F url%d_filter_chain\n", index);
					fputs(temp, fp1);
					sprintf(temp, "iptables -t filter -X url%d_filter_chain\n", index);
					fputs(temp, fp1);
				}
				if(fp3){
					fclose(fp3);
					fp3= NULL;
				}
				sprintf(temp,"/etc/l7-protocols/Url%d.pat", index);
				fp3= fopen(temp, "w+");
				if(fp3== NULL){
					fprintf(stderr, "Error--Url index pattern's file pointer is NULL.\n");
					return FAIL;
				}
				sprintf(temp, "Url%d\n", index);
				fputs(temp, fp3);
			}
#endif
			/*chang node to UrlFilter_Entry+number(0~15)*/
			sprintf(nodeName,"%s%d",SUB_NODE_NAME,i);
			node = mxmlFindElement(parant, parant, nodeName, NULL , NULL, MXML_DESCEND);
			if(node == NULL){
				continue;
			}
			/*get UrlFilter_Entry Activate*/
			Activate=mxmlElementGetAttr(node, "Activate");
			if(Activate == NULL){
				/*we don't set rule if user does not set the Activate value*/
				continue;
			}

			/*get UrlFilter_Entry URL*/
			URL=mxmlElementGetAttr(node, "URL");
			//fprintf(stderr,"run error \n");
			if(URL==NULL||strlen(URL)==0){
				/*we don't set rule if user does not set the URL value*/
				continue;
			}
			/*48 is the longest string length on our web page*/
			else if(strlen(URL)>MAX_URL_INPUT_LENGTH){
				fprintf(stderr,"the URL string is too long, it can't be longer than 48 characters\n");
				continue;
			}
			/*write the rule into UrlFilter.sh when rule is activated*/
			else if(!strcmp(Activate,"1")){
				/*copy URL to temp*/
				strcpy(temp,URL);

				/*cut the "http://" string*/
				if(strstr(temp,HTTP)){
					if(strlen(temp)==strlen(HTTP)){
						fprintf(stderr,"null host name\n");
						continue;
					}
					else{
						strcpy(temp,temp+strlen(HTTP));
					}
				}
				
#if defined(TCSUPPORT_CT_E8GUI)
				if(!strcmp(list_type, "1"))//white policy
				{
					sprintf(cmdTemp, URL_WHITE_POLICY_CMD, temp);
					fputs(cmdTemp,fp1);
				}
#endif
				#if 0
				//fprintf(stderr,"in write 7.1!!!!!!\n");
				/*replayce the special ch={"^","$","?",".","-","*","+","|"}*/
				for(j=0;j<8;j++){
					strcpy(temp1,strtok(temp,specialCh[j]));
					//fprintf(stderr,"in write 7.1!!!!!!\n");
					while((p=strtok(NULL,specialCh[j]))){
						strcpy(replayce,"");
						/*combine the special ch with "\" */
						sprintf(replayce,"%s%s","\\",specialCh[j]);
						/*replayce the special ch with "\special ch"*/
						strcat(temp1,replayce);
						/*combine the left string*/
						strcat(temp1,p);
					}
					/*copy temp1 to temp*/
					strcpy(temp,temp1);
					//fprintf(stderr,"in write hostname %s  temp  %s!!!!!!\n",hostName,temp);
				}
				//fprintf(stderr,"in write 7.2!!!!!!\n");
				/*URL now may be tw.yahoo.com/ or tw.yahoo.com or
				tw.yahoo.com/adpoai/uras.htm...etc, so we find the first "/", and we put the
				prior term to hostName.*/
				if(strstr(temp,"/")){
					/*copy the string before "/" to hostName*/
					strncat(hostName,temp,(strcspn(temp,"/")));
					#if 0
					strcpy(get,strstr(temp,"/"));

					if(!strcmp(get,"/")){
						strcpy(get,"");
						//fprintf(stderr,"get %s temp %s \n",get, temp);
					}
					#endif

				}
				/*if we can't find a "/", that means all the url is host name*/
				else{
					strcpy(hostName,temp);
				}
				#endif
//				fprintf(stderr,"in write hostname %s  temp  %s!!!!!!\n",hostName,temp);
				for(j=0; j<strlen(temp) ; j++){
					if(temp[j] == '/'){
						break;
					}
					for(k=0; k<SPCHNUMBER ; k++){
						isSpecial=0;
						if(temp[j] == specialCh[k]){
							sprintf(hostName, "%s\\%c", hostName, specialCh[k]);
							isSpecial=1;
							break;
						}
					}
					if(isSpecial == 0){
						sprintf(hostName, "%s%c",hostName, temp[j]);
					}
				}

				/*if first rule: or_op="", else or_op="|"*/
				if(firstRule == 0){
					memset(or_op,0,sizeof(or_op));
				}

//				fprintf(stderr,"hostname %s\n",hostName);

				/*write the rule to Url.pat*/
				if(strlen(hostName) != 0){
					#if 0
					if(strlen(get)!=0){
						/*write "or_op(^GET[\x09-\x0d ]*"GET string"[\x09-\x0d -~]*Host:[\x09-\x0d ]*"Host name string")" into Url.pat*/
						sprintf(pattern,"%s(^GET[\\x09-\\x0d ]*%s[\\x09-\\x0d -~]*Host:[\\x09-\\x0d ]*%s)",or_op,get,hostName);
						fprintf(stderr,"pattern  %s!!!!!!\n",pattern);
						fputs(pattern,fp2);
					}
					/*if GET==NULL*/
					else{
					#endif
						/*write "or_op(^Host:[\x09-\x0d ]*"Host name string")" into Url.pat*/
						sprintf(pattern,"%s(Host:[\\x09-\\x0d ]*%s)",or_op,hostName);
						/*fprintf(stderr,"pattern  %s!!!!!!\n",pattern);*/
#if defined(TCSUPPORT_CT_E8GUI)
						if(i>8){
							if(flag[index] == 0){
								sprintf(pattern,"(Host:[\\x09-\\x0d ]*%s)",hostName);
								fputs(pattern,fp3);
								if(strcmp(list_type, "1")){	//black list
									sprintf(temp, "iptables -t filter -N url%d_filter_chain\n", index);
									fputs(temp,fp1);
									sprintf(temp, "iptables -t filter -A FORWARD -p TCP -m multiport --dport http -j url%d_filter_chain\n", index);
									fputs(temp,fp1);
									sprintf(temp, "iptables -t filter -A url%d_filter_chain -m layer7 --l7proto Url%d -j DROP\n", index, index);
									fputs(temp,fp1);
								}
								flag[index] = 1;
							}
							else{
								fputs(pattern,fp3);
							}
						}
						else{
#endif
						fputs(pattern,fp2);
#if defined(TCSUPPORT_CT_E8GUI)
						}
#endif
						activeRule++;
					#if 0
					}
					#endif
				}
				else{
					fprintf(stderr,"host name is null, the url is wrong!!\n");
				}
				firstRule++;

			}
			/*if the Activate value is neither 1 nor 0, the setting is wrong*/
			else if(strcmp(Activate,"0")){
				fprintf(stderr,"Error--Wrong setting about UrlFilter_Entry Activate value. It can only be 1 or 0.\n");
			}

		}
		if(activeRule != 0){
#if defined(TCSUPPORT_CT_E8GUI)
			if(!strcmp(list_type, "1")){
				//strcpy(cmdTemp, "iptables -A INPUT -i br0 -p udp --dport 53 -j DROP\n");
				//fputs(cmdTemp, fp1);
				strcpy(cmdTemp, "iptables -A FORWARD -i br0 -p udp --dport 53 -j DROP\n");
				fputs(cmdTemp, fp1);
				strcpy(cmdTemp, "iptables -P FORWARD ACCEPT\n");
				fputs(cmdTemp, fp1);
			}
			else{
				fputs("iptables -t filter -A url_filter_chain -m layer7 --l7proto Url -j DROP\n",fp1);
			}
#else
			fputs("iptables -t filter -A url_filter_chain -m layer7 --l7proto Url -j DROP\n",fp1);
#endif
		}
	}
#if defined(TCSUPPORT_CT_E8GUI)
	if(strcmp(Activate,"1") ||!strcmp(list_type, "1"))
	{ //url_filter off or white list active
		index = MAX_URL_RULE/9;
		fp_clear= fopen(URL_FILTER_BL_CLEAR, "w+");
		if(fp_clear== NULL){
			fprintf(stderr, "Error--UrlFilter's file pointer is NULL.\n");
			return FAIL;
		}
		for(i=1;i<= index;i++){
			sprintf(temp, "iptables -t filter -D FORWARD -p TCP -m multiport --dport http -j url%d_filter_chain\n", index);
			fputs(temp,fp_clear);
			sprintf(temp, "iptables -t filter -F url%d_filter_chain\n", i);
			fputs(temp, fp_clear);
			sprintf(temp, "iptables -t filter -X url%d_filter_chain\n", i);
			fputs(temp, fp_clear);
			if(fp3){
				fclose(fp3);
				fp3= NULL;
			}
			sprintf(temp,"/etc/l7-protocols/Url%d.pat", i);
			fp3= fopen(temp, "w+");
			if(fp3== NULL){
				fprintf(stderr, "Error--Url index pattern's file pointer is NULL.\n");
				return FAIL;
			}	
		}
		fclose(fp_clear);
		chmod(URL_FILTER_BL_CLEAR ,777);
		//system(URL_FILTER_BL_CLEAR);
	}
#endif
	fclose(fp1);
	fclose(fp2);

	chmod(URL_FILTER_SH ,777);
	chmod(URL_PATTER_PATH,777);
#if defined(TCSUPPORT_CT_E8GUI)
	if(fp3)
		fclose(fp3);
	for(i=1;i<= index;i++){
		sprintf(temp,"/etc/l7-protocols/Url%d.pat",i);
		chmod(temp,777);
	}
#endif

	return SUCCESS;
}

#else

#define URL_LEN	32

typedef struct _url_info
{
	unsigned char index;
	unsigned char type;		//host or path
	char host[URL_LEN];
	char path[URL_LEN];
}url_info;

#define URL_TYPE_HOST	1
#define URL_TYPE_PATH	2
#define URL_FILTER_IOC_MAGIC 			'h'              
#define URL_FILTER_IOC_SET_ENABLE _IOW(URL_FILTER_IOC_MAGIC,0,unsigned char)
#define URL_FILTER_IOC_SET_MODE _IOW(URL_FILTER_IOC_MAGIC,1,unsigned char)
#define URL_FILTER_SET_URL _IOW(URL_FILTER_IOC_MAGIC,2,struct _url_info)

int ioctl_url_filter(int cmd, void *data, int datealen)
{
	int fp = -1;
	int ret = -1;	
	fp = open("/dev/urlfilter", O_RDONLY);
	if(fp == -1)
	{
		printf("ioctl_urlfilter: fp is [%d]\n", fp);
		return -1;
	}
	
	ret = ioctl(fp, cmd, data);
	if(ret < 0)
	{
		printf("\r\nioctl_urlfilter failed!");
	}
	close(fp);
	return 0;
}

int
url_filter_write(mxml_node_t *top, mxml_node_t *parant){
	int i=0,j=0,k=0,isSpecial=0;

	//char specialCh[SPCHNUMBER]={'^','$','?','.','-','*','+','|'};
	char nodeName[MAX_NODE_NAME]={0};
	char temp[MAX_URL_INPUT_LENGTH*2+20]={0};
	char hostName[MAX_URL_INPUT_LENGTH*2+20]={0};
	char pattern[MAX_URL_INPUT_LENGTH*2+20]={0};
	const char* Activate=NULL;
	const char* URL=NULL;
	const char *list_type = NULL;

	mxml_node_t *node = NULL;

	unsigned char enable = 0;
	unsigned char mode = 0;
	//int has_path = 0;
	//char pathName[MAX_URL_INPUT_LENGTH*2+20] = {0};
	url_info urlInfo;
	int len = 0;
	const char *pAttr = NULL;
	char	strDel[300] = {0};

	//tcdbg_printf("\nurl_filter write start");
	/*set node to be UrlFilter_Common*/
	node = mxmlFindElement(parant, parant, STRING_COMMON, NULL , NULL, MXML_DESCEND);
	if(node == NULL){
		fprintf(stderr, "Error--UrlFilter Common is NULL.\n");
		return FAIL;
	}
	/*get UrlFilter_Common Activate value*/
	Activate=mxmlElementGetAttr(node, "Activate");
	if(Activate == NULL){
		fprintf(stderr, "Error--UrlFilter_Common Activate value is NULL.\n");
		return FAIL;
	}

	list_type = mxmlElementGetAttr(node, "Filter_Policy");
	if(list_type == NULL)
	{
		fprintf(stderr, "Error--UrlFilter_Common Filter_Policy value is NULL.\n");
		return FAIL;
	}
	if(!strcmp(list_type, "1")){	//white list
		mode = 1;
	}else{
		mode = 0;
	}
	//check attribute action and do unset action
	pAttr = mxmlElementGetAttr(node, "Action");
	if(pAttr != NULL){
		if(!strcmp(pAttr, "Del")){
			pAttr = NULL;
			pAttr = mxmlElementGetAttr(node, "DeleteIndex");
			if(pAttr != NULL){
				strcpy(strDel, pAttr);
				unset_action(strDel, DEL_URL);
			}
		}			
	}
	
	/*do when url filter is activated*/
	if(!strcmp(Activate,"1")){
		enable = 1;
		ioctl_url_filter(URL_FILTER_IOC_SET_ENABLE,&enable, sizeof(enable));
		ioctl_url_filter(URL_FILTER_IOC_SET_MODE, &mode, sizeof(mode));
		/*write depend on each rule*/
		for(i=0;i<MAX_URL_RULE;i++){
			memset(pattern,0,sizeof(pattern));
			memset(hostName,0,sizeof(hostName));
			memset(temp,0,sizeof(temp));
			sprintf(nodeName,"%s%d",SUB_NODE_NAME,i);
			node = mxmlFindElement(parant, parant, nodeName, NULL , NULL, MXML_DESCEND);
			if(node == NULL){
				continue;
			}
			/*get UrlFilter_Entry Activate*/
			Activate=mxmlElementGetAttr(node, "Activate");
			if(Activate == NULL){
				/*we don't set rule if user does not set the Activate value*/
				continue;
			}

			/*get UrlFilter_Entry URL*/
			URL=mxmlElementGetAttr(node, "URL");
			if(URL==NULL||strlen(URL)==0){
				/*we don't set rule if user does not set the URL value*/
				continue;
			}
			/*48 is the longest string length on our web page*/
			else if(strlen(URL)>MAX_URL_INPUT_LENGTH){
				fprintf(stderr,"the URL string is too long, it can't be longer than 48 characters\n");
				continue;
			}
			/*write the rule into UrlFilter.sh when rule is activated*/
			else if(!strcmp(Activate,"1")){
				/*copy URL to temp*/
				strcpy(temp,URL);

				/*cut the "http://" string*/
				if(strstr(temp,HTTP)){
					if(strlen(temp)==strlen(HTTP)){
						fprintf(stderr,"null host name\n");
						continue;
					}
					else{
						strcpy(temp,temp+strlen(HTTP));
					}
				}
				len = strlen(temp);
				for(j=0; j<len; j++){
					if(temp[j] == '/'){
						break;
					}
					sprintf(hostName, "%s%c",hostName, temp[j]);

				}

				/*write the rule to Url.pat*/
				if(strlen(hostName) != 0){
					memset(&urlInfo, 0, sizeof(urlInfo));
					urlInfo.index = i;
					urlInfo.type = URL_TYPE_HOST;
					strncpy(urlInfo.host, hostName, sizeof(urlInfo.host));
					ioctl_url_filter(URL_FILTER_SET_URL, &urlInfo, sizeof(urlInfo));	
					//tcdbg_printf("\r\naccess.c, 4149, set host, index = %d", i);
					if((j<len) && temp[j] == '/'){
						//tcdbg_printf("\r\naccess.c, 4152");
						strcpy(temp, &temp[j]);
						memset(&urlInfo, 0, sizeof(urlInfo));
						urlInfo.index = i;
						urlInfo.type = URL_TYPE_PATH;
						strncpy(urlInfo.path, temp, sizeof(urlInfo.path));
						ioctl_url_filter(URL_FILTER_SET_URL, &urlInfo, sizeof(urlInfo));
						//tcdbg_printf("\r\naccess.c, 4157, set path, index = %d", i);
					}
				}
				else{
					fprintf(stderr,"host name is null, the url is wrong!!\n");
				}

			}
			/*if the Activate value is neither 1 nor 0, the setting is wrong*/
			else if(strcmp(Activate,"0")){
				fprintf(stderr,"Error--Wrong setting about UrlFilter_Entry Activate value. It can only be 1 or 0.\n");
			}

		}
	}
	else{
		enable = 0;
		ioctl_url_filter(URL_FILTER_IOC_SET_ENABLE,&enable, sizeof(enable));
	}
	
	//tcdbg_printf("\nurl filter write end");
	return SUCCESS;
}
#endif
int
url_filter_verify(mxml_node_t *node){
	return SUCCESS;
}

#if defined(TCSUPPORT_CT_E8GUI)
/*_____________________________________________________________________________
*description:
*	Read url configure node tree data and then set add_num attributes value and compose a temp file which contains url info for webpage display
*param:
*	*top: root of mxml_node_t structure
*	*attr: attribute of url filter cfg node
*global:
*	none
*return int:
*	success 0
*	otherwise -1
*call:
*	none
*revision:
*	1.0 dgk 20100930
*
*_____________________________________________________________________________
*/
int
url_filter_read(mxml_node_t *top, char nodeName[][MAX_NODE_NAME], char *attr){
	
	char urlNodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	mxml_node_t *node = NULL;   /* <element node> */
	mxml_node_t *group = NULL;  /* <UrlFilter Node> */
	int static_url_index = 0;//	static_url_index = 0;
	char addnum[4] = {0};
	char count[4] = {0};
	int i = 0;
	int norecord = 0;
	
	FILE *fp = NULL;
	char *p = NULL;
	char curl[MAX_URL_INPUT_LENGTH] = {0};
	char tmpbuf[50] = {0};

	if(strcmp(attr,"Url_num") && strcmp(attr,"add_num"))
	{
		return SUCCESS;
	}
	group = mxmlFindElement(top, top, URLFILTER,
			NULL, NULL, MXML_DESCEND);
	if(group == NULL){
		tcdbg_printf("\r\nurl_filter_read:return fail\n");
		return FAIL;
	}

	fp= fopen(TMP_URL_FILE, "w");
	if(fp== NULL){
		fprintf(stderr, "url_filter_read:Error--UrlFilter's temp file is NULL.\n");
		tcdbg_printf(stderr);
		return FAIL;
	}
	
	memset(urlNodeName, 0, sizeof(urlNodeName));
	strcpy(urlNodeName[0],URLFILTER);
	for(i = 0; i < MAX_URL_RULE; i++)
	{
		sprintf(urlNodeName[1],"%s%d",SUB_NODE_NAME, i);
		node = mxmlFindElement(group, group, urlNodeName[1],
					NULL, NULL, MXML_DESCEND);
		if(node == NULL)
		{
			/*1.find the entry index which can be used*/
			if(!norecord)
			{
				sprintf(addnum,"%d",i);
				mxmlElementSetAttr(group, "add_num", addnum);
				norecord++;
			}
		}
		else
		{
			memset(count,0x00, sizeof(count));
			sprintf(count,"%d\n",i);
			fputs(count, fp);
			/*2.get Url value and save in file*/
			p = mxmlElementGetAttr(node, "URL");
			if(p){
				strcpy(curl, p);
				sprintf(tmpbuf, "%s\n", curl);
				fputs(tmpbuf, fp);	
			}
			else{
				fputs("\n", fp);
			}
			static_url_index++;
		}
	}
	fclose(fp);fp=NULL;
	memset(count,0x00, sizeof(count));
	sprintf(count,"%d",static_url_index);
	mxmlElementSetAttr(group, "Url_num", count);
	return SUCCESS;
}
/*_____________________________________________________________________________
*function name:unset_action
*description:
*	unset one or more entry indexs
*param:
*	*pstr: unset compose string
*global:
*	none
*return int:
*	success 0
*	otherwise -1
*call:
*	none
*revision:
*	1.0 dgk 20100930
*
*_____________________________________________________________________________
*/
int
unset_action(const char *pstr, int itype){
	char 		*p = pstr;
	char 		strnum[5] = {0};
	char		strCurfilter[20] = {0};
	char		strEntry[32] = {0};
	int		index = 0;
	tcapi_msg_t 	t_msg;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], WEBCURSET);
	strcpy(nodeName[1], SUB_NODE_NAME);
	mxml_node_t *top = get_node_top();
	
	if(DEL_URL == itype){
		strcpy(strCurfilter, "url_filter_id");
		strcpy(strEntry, "UrlFilter_Entry");
	}
	else if(DEL_IP == itype){
		strcpy(strCurfilter, "ipfilter_id");
		strcpy(strEntry, "IpMacFilter_Entry");
	}
	char *token = NULL;
	token = strstr(p, ",");
	while(token != NULL){
		memset(strnum, 0x00, sizeof(strnum));
		token[0] = '\0';
		strcpy(strnum,p);
		if(!strlen(strnum)) break;
		//unset action
		setAttrValue(top, nodeName, strCurfilter, strnum);
		memset(&t_msg,0,sizeof(t_msg));
		strcpy(t_msg.node, strEntry);
        	tcapi_unset_req(top, &t_msg);
		//get next token
		p = token + strlen(",");
		token = strstr(p, ",");
	}
	return SUCCESS;
}
#endif
/*_____________________________________________________________________________
*description:
*	Execute shell script composed by url_filter_write function.
*param:
**	top: root of mxml_node_t structure
*	name[][]: record path of every configure node entries (ex. {{WAN_PVC0},{WAN_PVC1}�K})
*return int:
*	success 0
*	otherwise -1
*_____________________________________________________________________________
*/
int
url_filter_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	unsigned int new_filter_state=0;
	char urlFilterState[5]={0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
#if defined(TCSUPPORT_CT_E8GUI)
	char list_type[5] = {0};
#endif

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], URLFILTER);
	strcpy(nodeName[1], STRING_COMMON);

#if defined(TCSUPPORT_CT_DNSBIND)
	restart_dnsmasq();
#endif
	if(getAttrValue(top, nodeName, "Activate", urlFilterState)!=TCAPI_PROCESS_OK){
		/*fprintf(stderr, "Error--UrlFilter get some errors when get activate value.\n");*/
	}
	else if (!strcmp(urlFilterState,"1")){
//		setnew_filter_state(top,url_check_filter,URL_VECTOR,&new_filter_state);
		if(url_check_filter(top)){//filter on
			new_filter_state=pre_sys_state.filter_m_state|URL_VECTOR;
		}
		else{//filter down
			new_filter_state=pre_sys_state.filter_m_state&(~URL_VECTOR);
		}
		//     		pre_sys_state.filter_m_state = check_and_set_filter(top,pre_sys_state.filter_m_state);
		check_and_set_l7filter(new_filter_state);
		pre_sys_state.filter_m_state = check_and_set_filter(new_filter_state);
#if 0
		if(pre_sys_state.filter_m_state==1){
#endif
			system(URL_FILTER_SH);
#if 0
		}
#endif
	}
	else{		
#if defined(TCSUPPORT_CT_E8GUI)
		char  urlFilterpolicy[5] = {0};
		if(getAttrValue(top, nodeName, "Filter_Policy", urlFilterpolicy)==TCAPI_PROCESS_OK){
			if(!strcmp(urlFilterpolicy, "1")){
				url_filter_deletewhiterules(1);
			}
			else{
				system(URL_STOP_SH);
			}
		}
#else
		system(URL_STOP_SH);
#endif
//		setnew_filter_state(top,url_check_filter,URL_VECTOR,&new_filter_state);
		if(url_check_filter(top)){//filter on
			new_filter_state=pre_sys_state.filter_m_state|URL_VECTOR;
		}
		else{//filter down
			new_filter_state=pre_sys_state.filter_m_state&(~URL_VECTOR);
		}
		//     		pre_sys_state.filter_m_state = check_and_set_filter(top,pre_sys_state.filter_m_state);
		check_and_set_l7filter(new_filter_state);
		pre_sys_state.filter_m_state = check_and_set_filter(new_filter_state);
	}
#if defined(TCSUPPORT_CT_E8GUI)
	getAttrValue(top, nodeName, "Filter_Policy", list_type);
	if(strcmp(urlFilterState,"1") || !strcmp(list_type, "1")){ //url_filter off or white list active
		tcdbg_printf("\nurl_filter_execute url_filter_clear");
		system(URL_FILTER_BL_CLEAR);
	}
#endif
	return SUCCESS;
}
/*______________________________________________________________________________
**	url_check_filter
**
**	descriptions:
**		check if the url filter need filter module or not.
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
url_check_filter(mxml_node_t *top){
	int filter_on=0;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char urlfilter_status[5]={0};
	/*check url filter state*/

	/*flush the buffer*/
	memset(nodeName,0,sizeof(nodeName));

	/*let the node name = UrlFilter_Common*/
	strcpy(nodeName[0], URLFILTER);
	strcpy(nodeName[1], STRING_COMMON);
	/*check the UrlFilter_Common Activate value*/
	if(getAttrValue(top, nodeName, "Activate", urlfilter_status) != TCAPI_PROCESS_OK){
		//fprintf(stderr,"get url filter activate error in check filter state\n");
		fprintf(stderr,"nodename 0 %s nodename 1 %s Activate urlfilter_status %s\n",nodeName[0],nodeName[1],urlfilter_status);
	}
	/*if UrlFilter_Common Activate value=1 then do follows*/
	else if(!strcmp(urlfilter_status,"1")){
		/*flush the node name [1] field*/
		memset(nodeName[1],0,sizeof(nodeName[1]));
		int urlIndex=0;
		/*check url rule activate one by one, if any one is activated, then load the filter module*/
		for(urlIndex=0;urlIndex<MAX_URL_RULE; urlIndex++){
			/*reset the state buffer*/
			strcpy(urlfilter_status,"");
			/*set node name[1]=Entry+number(0~15)*/
			sprintf(nodeName[1], "%s%d", SUB_NODE_NAME, urlIndex);
			if(getAttrValue(top, nodeName, "Activate", urlfilter_status) != TCAPI_PROCESS_OK){
				//fprintf(stderr,"get url filter activate error in check filter state\n");
				continue;
			}
			/*find a rule is activated, load the l7 filter module and break for loop*/
			else if(!strcmp(urlfilter_status,"1")){
				filter_on=1;
				break;
			}
			else{
				/*nothing to do*/
			}
		}
	}
	return filter_on;
}
#if defined(TCSUPPORT_CT_E8GUI)
/*______________________________________________________________________________
**function name:url_filter_deletewhiterules
**
**description:
*    read the UrlFilter.sh file and execute a shell script which contains delete filters commands
**parameters:
*    ipolicy: 		the Url filter policy value 0 or 1..
**global:
*    none
**return:
*    0:success
*    -1:fail
**call:
*    none
**revision:
*     1. dgk 20100927
**____________________________________________________________________________*/
int
url_filter_deletewhiterules(int ipolicy){
	int 		ret = SUCCESS;
	FILE 		*fr = NULL;
	FILE		*fw = NULL;
	int		nCount = 0;
	char		*pStart = NULL;
	char		line[128] = {0};
	
	fr = fopen(URL_FILTER_SH, "r");
	if(fr == NULL){
		ret = FAIL;
		return ret;
	}
	memset(line, 0x00, sizeof(line));
	fseek(fr,0,SEEK_SET);
	fgets(line, sizeof(line), fr);
	if((strstr(line, "--algo")) == NULL){
		if(1 == ipolicy)
			system(URL_STOP_SH);
		fclose(fr);
		fr = NULL;
		ret = SUCCESS;
		return ret;
	}
	fw = fopen(URL_TMP_SH, "w");
	if(fw == NULL){
		tcdbg_printf("url_filter_deletewhiterules:create %s failed!!\n", URL_TMP_SH);
		fclose(fr);
		fr = NULL;
		return FAIL;
	}
	fseek(fr,0,SEEK_SET);
	memset(line, 0x00, sizeof(line));
	while (fgets(line, sizeof(line), fr)) {
		if(strstr(line, "iptables -P FORWARD ACCEPT") != NULL)
			break;
		if((pStart = strstr(line, "-A")) != NULL){
			nCount = pStart - &line[0];
			line[nCount+1] = 'D';
		}
		fputs(line, fw);
		memset(line, 0x00, sizeof(line));
	}
	fclose(fr);
	fr = NULL;
	fclose(fw);
	fw = NULL;
	chmod(URL_TMP_SH ,777);
	system(URL_TMP_SH);
	usleep(100000);
	unlink(URL_TMP_SH);
	return ret;	
}
#endif
/**********20081021 krammer add************/
#endif

#ifdef SSL
char *
sslca_adv_table[MAX_NODE_NAME]=
{
	"Common","Flag","",
};
char *
sslca_merge_table[MAX_NODE_NAME]=
{
	"sslca_id","",
};

/*______________________________________________________________________________
**	sslca_boot
**
**	descriptions:
**		the boot function of sslca.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
sslca_boot(mxml_node_t *top){

	return SUCCESS;
}
/*______________________________________________________________________________
**	sslca_init
**
**	descriptions:
**		the init function of sslca.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
sslca_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="SslCA",
			.type=ADV_FLAG|ENTRY_TYPE|MAX_CA_NUM,
			.adv_table=sslca_adv_table,
			.merge_table=sslca_merge_table,
			.cfg_read=NULL,
			.cfg_write=sslca_write,
			.cfg_verify=sslca_verify,
			.cfg_execute=sslca_execute,
			.cfg_boot=sslca_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}

/*______________________________________________________________________________
**	sslca_write
**
**	descriptions:
**		
**	parameters:
**		top: 		
**		parant: 		
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
sslca_write(mxml_node_t *top, mxml_node_t *parant){
	FILE *fp = NULL;
	int       index=0, maxCaNum, FragNum;          
	int       i=0, randomFlag=0, reVal=SUCCESS;                      /* Looping var */

	char tmpbuf[33], buf[513]={0}, caPath[33];
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	tcapi_msg_t msg;

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],SSL_CA);
	strcpy(nodeName[1],"Flag");
	
	if(getAttrValue(top, nodeName, CA_UPGRADE_FLAG, tmpbuf) == TCAPI_PROCESS_OK){
		tcdbg_printf("sslca_write:excute upgrade action!\n");
		return SUCCESS;
	}
	
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],SSL_CA);
	strcpy(nodeName[1],"Common");
	
	if(getAttrValue(top, nodeName, "CurIndex", tmpbuf) != TCAPI_PROCESS_OK){
		tcdbg_printf("sslca_write:get CurIndex index failed!\n");
		return FAIL;
	}
	/*if CurIndex !=0,means user want to read a random CA file */
	if((index = atoi(tmpbuf)) == 0){
		if(getAttrValue(top, nodeName, "ValidIndex", tmpbuf) != TCAPI_PROCESS_OK){
			tcdbg_printf("sslca_write:get valid index failed!\n");
			return FAIL;
		}
		index = atoi(tmpbuf);
		strcpy(caPath, CA_PATH);
	}
	else{
		strcpy(caPath, TMP_CA_PATH);
		randomFlag = 1;
	}
	/*Check index and maxCaNum*/
	memset(tmpbuf,0,sizeof(tmpbuf));
	if(getAttrValue(top, nodeName, "MaxCANum", tmpbuf) != TCAPI_PROCESS_OK){
		tcdbg_printf("sslca_write:get Max Number failed!\n");
		reVal = FAIL;
		goto Error;
	}
	maxCaNum = atoi(tmpbuf);
	if(maxCaNum < 1 || maxCaNum > MAX_CA_NUM){
		tcdbg_printf("sslca_write:System do not support Max number in romfile(1~%d)\n",MAX_CA_NUM);
		reVal = FAIL;
		goto Error;
	}
	if(index == 0 || index > maxCaNum){
		tcdbg_printf("sslca_write:use default CA or index too large!(index=%d)\n",index);
		reVal = FAIL;
		goto Error;
	}
	//tcdbg_printf("sslca_write:get index =%d\n",index);	


	sprintf(buf,"%s%d",CA_NODE_NAME,index-1);
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],SSL_CA);
	strcpy(nodeName[1],buf);
	
	if(getAttrValue(top, nodeName, "FragNum", tmpbuf) != TCAPI_PROCESS_OK){
		tcdbg_printf("sslca_write:get Frag Number failed!\n");
		reVal = FAIL;
		goto Error;
	}
	FragNum = atoi(tmpbuf);
	//tcdbg_printf("sslca_write:get FragNum =%d\n",FragNum);
	
	fp = fopen(caPath,"w");
	if(fp == NULL){
		tcdbg_printf("sslca_write:open ca file failed!!\n");	
		reVal = FAIL;
		goto Error;
	}
	else{
			for(i = 0; i < FragNum; i++){
				memset(buf,0,sizeof(buf));
				memset(tmpbuf,0,sizeof(tmpbuf));
				sprintf(tmpbuf,"Frag%d",i);
				if(getAttrValue(top, nodeName, tmpbuf, buf) != TCAPI_PROCESS_OK){
					tcdbg_printf("sslca_write:get Frag%d failed!\n",i);
					reVal = FAIL;
					goto Error;
				}
				fputs(buf, fp);
			}			
			fclose(fp);
			fp = NULL;
	}
	
	chmod(caPath,777);
Error:
	/*Clear CurIndex to 0*/
	if(randomFlag){
		memset(&msg,0,sizeof(msg));
		strcpy(msg.node,"SslCA_Common");
		strcpy(msg.attr,"CurIndex");
          	strcpy(msg.value,"0");
          	tcapi_set_req(top, &msg);		
	}
	if(reVal == FAIL)
		unlink(caPath);
	if(fp){
		fclose(fp);
		fp = NULL;
	}
	//tcdbg_printf("sslca_write!\n");
	return reVal;
}/* end sslca_write */

/*______________________________________________________________________________
**	sslca_verify
**
**	descriptions:
**		To check the system parameter of sslca element  is validly or not.
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
sslca_verify(mxml_node_t *node){
	tcdbg_printf("sslca_verify!\n");
	return SUCCESS;
}/* end lan_verify */

/*_____________________________________________________________________________
**      function name: sslca_execute
**      descriptions:
**            To execute lan node service.
**
**      parameters:
** 
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
sslca_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	char  tmp[32], nodeName[MAX_ARG_NUM][MAX_NODE_NAME], fileBuf[513],filePath[33];
	struct stat fstat;
	FILE  *fp = NULL;
	int i, index, maxCANum, caSize, validIndex, fragNum, reVal = SUCCESS;
	tcapi_msg_t msg;
	
	//memset(&msg,0,sizeof(msg)) ;
	memset(nodeName,0,sizeof(nodeName));
  	strcpy(nodeName[0],SSL_CA);
  	strcpy(nodeName[1],"Flag");
	if(getAttrValue(top, nodeName, CA_UPGRADE_FLAG, tmp) != TCAPI_PROCESS_OK){
		//tcdbg_printf("sslca_execute:do not upgrade CA node!!\n");
		return SUCCESS;
	}
	index = atoi(tmp);
	
	memset(tmp,0,sizeof(tmp));
	memset(nodeName,0,sizeof(nodeName));
  	strcpy(nodeName[0],SSL_CA);
  	strcpy(nodeName[1],"Common");
	if(getAttrValue(top, nodeName, "MaxCANum", tmp) != TCAPI_PROCESS_OK){
		tcdbg_printf("sslca_execute:do not upgrade CA node!!\n");
		reVal = FAIL;
		goto Error;
	}
	maxCANum = atoi(tmp);

	if(getAttrValue(top, nodeName, "ValidIndex", tmp) != TCAPI_PROCESS_OK){
		tcdbg_printf("sslca_execute:do not upgrade CA node!!\n");
		reVal = FAIL;
		goto Error;
	}
	validIndex = atoi(tmp);
	
	if(index < 1 || index > maxCANum){
		tcdbg_printf("sslca_execute:Upgrade CA failed,ca index is too large!!\n");
		reVal = FAIL;
		goto Error;
	}
	
#if defined(TCSUPPORT_CT_E8GUI)
	if(FAIL == writecafiletoflash(index)){
		tcdbg_printf("sslca_execute:write ca file to flash failed!!\n");
	}
#endif
	memset(filePath,0,sizeof(filePath));
	sprintf(filePath, "%s%d.pem", UPLOAD_CA_PATH, index);
	if(stat(filePath,&fstat) != 0 || (fp = fopen(filePath, "r")) == NULL){
		tcdbg_printf("sslca_execute:open %s failed!!\n", filePath);
		reVal = FAIL;
		goto Error;
	}
	/*Check CA size and content*/
	if((caSize = fstat.st_size) > MAX_CA_SIZE){
		tcdbg_printf("sslca_execute:Save ca failed,%s is too large!!\n", filePath);
		reVal = FAIL;
		goto Error;
	}	
	/*Check romfile size*/
	if(stat(TMP_ROMFILE_PATH,&fstat) != 0 ){
		tcdbg_printf("sslca_execute:open %s failed!!\n", TMP_ROMFILE_PATH);
		reVal = FAIL;
		goto Error;
	}
	/*(caSize + 100) means there are node information in romfile*/
	if((MAX_ROMFILE_SIZE - fstat.st_size) < (caSize + 100)){
		tcdbg_printf("sslca_execute:Save ca failed:There is no enough room in romfile(romfile size=%d)!!\n", fstat.st_size);
		reVal = FAIL;
		goto Error;
	}
	
	memset(fileBuf,0,sizeof(fileBuf));
	/*Read the start and end of CA*/
	fread(fileBuf,sizeof(char), 27, fp);
	fseek(fp,caSize - 25,SEEK_SET);
	fread(fileBuf + 27,sizeof(char), 25, fp);
	if(strstr(fileBuf, "-----BEGIN CERTIFICATE-----") == NULL || 
		strstr(fileBuf, "-----END CERTIFICATE-----") == NULL){
		tcdbg_printf("sslca_execute:Save ca failed,%s is not a correct CA!!\n", filePath);
		reVal = FAIL;
		goto Error;
	}
	/*Clean node data before set node*/
	memset(&msg,0,sizeof(msg));
	sprintf(msg.node,"SslCA_Entry%d",index - 1);	
        tcapi_unset_req(top, &msg);
	
	fseek(fp,0,SEEK_SET);
	fragNum = (caSize + FRAG_SIZE - 1) / FRAG_SIZE;
	tcdbg_printf("sslca_execute:file fragNum=%d,fileSize=%d!!\n", fragNum,caSize);
	for(i = 0; i < fragNum; i++){
		memset(fileBuf,0,sizeof(fileBuf));
		fread(fileBuf,sizeof(char), FRAG_SIZE, fp);
		memset(&msg,0,sizeof(msg)) ;
		sprintf(msg.node,"SslCA_Entry%d",index - 1);
		sprintf(msg.attr,"Frag%d", i);
          	strcpy(msg.value,fileBuf);
          	tcapi_set_req(top, &msg);
          	if( msg.retval != TCAPI_PROCESS_OK){
			tcdbg_printf("sslca_execute:set failed!!\n");   
          		reVal = FAIL;
			goto Error;
          	}		
	}
	
	memset(&msg,0,sizeof(msg)) ;
	sprintf(msg.node,"SslCA_Entry%d",index - 1);
	strcpy(msg.attr,"FragNum");
	sprintf(msg.value,"%d",fragNum);
	tcapi_set_req(top, &msg);
	if( msg.retval != TCAPI_PROCESS_OK){
			tcdbg_printf("sslca_execute:set fragNum failed!!\n");   
          		reVal = FAIL;
			goto Error;
        }
	//tcdbg_printf("sslca_execute:set FragNum =%d\n",fragNum);
Error:
	if(fp){
		fclose(fp);
		fp = NULL;
	}
	unlink(filePath);
	/*Delete the flag node*/
	memset(&msg,0,sizeof(msg));
	strcpy(msg.node,"SslCA_Flag");
	strcpy(msg.attr,CA_UPGRADE_FLAG);
	tcapi_unset_req(top, &msg);	
	
	return reVal;
}/* end sslca_execute */
#if defined(TCSUPPORT_CT_E8GUI)
/*______________________________________________________________________________
**function name:writecafiletoflash
**
**description:
*    write the CA file to flash and update the current CA_PATH file
**parameters:
*    index: 		the index number of certificate file.
**global:
*    none
**return:
*    0:success
*    -1:fail
**call:
*    none
**revision:
*     1. dgk 20100917
**____________________________________________________________________________*/
int	
writecafiletoflash(int index)
{
	FILE	*fw = NULL;
	FILE	*fr = NULL;
	struct 	stat fstat;
	char 	nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char 	*pCAfile = NULL;
	char	cmd_buf[128] = {0};
	char	filePath[64] = {0};
	char	value[16] = {0};
	char	*pStart, *pEnd = NULL;
	char 	*pBuf = NULL;
	int	nLength = 0;
	int	caSize = 0;
	int	reVal = SUCCESS;
	mxml_node_t *top = get_node_top();
	
	unsigned long ulSize[4] = {CERM1_RA_SIZE, CERM2_RA_SIZE, CERM3_RA_SIZE, CERM4_RA_SIZE};
	unsigned long ulOffsite[4] = {CERM1_RA_OFFSET, CERM2_RA_OFFSET, CERM3_RA_OFFSET, CERM4_RA_OFFSET};
	
	memset(filePath,0,sizeof(filePath));
	strcpy(filePath, "/tmp/boa-temp");
	
	if(stat(filePath,&fstat) != 0 || (fr = fopen(filePath, "r")) == NULL){
		tcdbg_printf("writecafiletoflash:open %s failed!!\n", filePath);
		reVal = FAIL;
		goto Error;
	}
	/*Check CA size and content*/
	if((caSize = fstat.st_size) > MAX_CA_SIZE){
		tcdbg_printf("writecafiletoflash:Save ca failed,%s is too large!!\n", filePath);
		reVal = FAIL;
		setCertistatus(5);
		goto Error;
	}
	
	pBuf = (char*)malloc(caSize + 1);
	if(!pBuf)
	{
		tcdbg_printf("writecafiletoflash:malloc size failed !!\n");
		reVal = FAIL;
		goto Error;
	}
	memset(pBuf, 0x00, caSize + 1);
	fseek(fr,0,SEEK_SET);
	fread(pBuf,sizeof(char), caSize, fr);

	if((pStart = strstr(pBuf, "-----BEGIN CERTIFICATE-----")) == NULL)
	{
		tcdbg_printf("writecafiletoflash:write ca failed,%s start is not a correct CA!!\n", filePath);
		reVal = FAIL;
		setCertistatus(1);
		goto Error;
	}
	else
	{
		if((pEnd = strstr(pBuf, "-----END CERTIFICATE-----")) == NULL)
		{
			tcdbg_printf("writecafiletoflash:write ca failed,%s end is not a correct CA !!\n", filePath);
			reVal = FAIL;
			setCertistatus(1);
			goto Error;
		}
		else
		{
			fseek(fr,(int)(pStart - pBuf),SEEK_SET);
			nLength = (int)(pEnd+25-pStart);
			pCAfile = (char*)malloc(nLength+1);
			if(!pCAfile)
			{
				tcdbg_printf("writecafiletoflash:malloc size failed !!\n");
				reVal = FAIL;
				goto Error;
			}
			memset(pCAfile, 0x00, nLength+1);
			fread(pCAfile, sizeof(char), nLength, fr);
		}
	}
	
	memset(filePath,0,sizeof(filePath));
	sprintf(filePath, "%s%d.pem", UPLOAD_CA_PATH, index);
	
	fw = fopen(filePath, "w");
	if(fw == NULL){
		tcdbg_printf("writecafiletoflash:CA temp file %s open failed\n", filePath);
	}
	pCAfile[nLength] = '\n';
	fwrite(pCAfile, sizeof(char), nLength+1, fw);
	if(fw){
		fclose(fw);
		fw = NULL;
	}

	sprintf(cmd_buf, TC_FLASH_WRITE_CMD, filePath, ulSize[index-1], ulOffsite[index-1] ,RESERVEAREA_NAME);

	//tcdbg_printf("%s\n",cmd_buf);
	system(cmd_buf);
	setCertistatus(0);
	//write ca to CA_PATH
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], "Cwmp");
	strcpy(nodeName[1], "Entry");
	memset(value, 0x00, sizeof(value));
	if (getAttrValue(top, nodeName, "EnableCert", value) == TCAPI_PROCESS_OK)
	{
		if(!strcmp(value,"1")){
			fw = fopen(CA_PATH, "w");
			if(fw == NULL){
				tcdbg_printf("writecafiletoflash:CA_PATH file open failed\n");
			}
			fwrite(pCAfile, sizeof(char), nLength+1, fw);
			if(fw){
				fclose(fw);
				fw = NULL;
			}
		}
	}
Error:
	if(fr){
		fclose(fr);
		fr = NULL;
	}
	if(pBuf){
		free(pBuf);
		pBuf = NULL;
	}
	if(pCAfile){
		free(pCAfile);
		pCAfile = NULL;
	}
	
	return reVal;
}
#endif
#endif

#ifdef CWMP
char *
tr069Attr_adv_table[MAX_NODE_NAME]=
{
	"SpeAttr","",
};

/*______________________________________________________________________________
**	tr069Attr_init
**
**	descriptions:
**		the init function of ipfilter.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
tr069Attr_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="TR069Attr",
			.type=ADV_FLAG|SINGLE_TYPE,
			.adv_table=tr069Attr_adv_table,
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
#if defined(TCSUPPORT_CT_ACCESSLIMIT)
char *
accesslimit_adv_table[MAX_NODE_NAME]=
{
	"Common","",
};

char *
accesslimit_merge_table[MAX_NODE_NAME]=
{
	"accesslimit_id","",
};

int
accesslimit_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="Accesslimit",
			.type=ADV_FLAG|ENTRY_TYPE|MAX_ACCESSLIMIT_NUM,
			.adv_table=accesslimit_adv_table,
			.merge_table=accesslimit_merge_table,
			.cfg_read=NULL,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=accesslimit_execute,
			.cfg_boot=accesslimit_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}

int
accesslimit_execute(mxml_node_t *top, char name[][MAX_NODE_NAME])
{
	int ret = -1;
	ret = accesslimit_boot(top);
	return ret;
}

int
accesslimit_boot(mxml_node_t *top)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char tmp[32] = {0};
	char tmp1[32] = {0};
	char tmp2[32] = {0};
	char cmdbuf[128] = {0};
	int i;
	
	memset(nodeName,0,sizeof(nodeName));
  	strcpy(nodeName[0],ACCESSLIMIT_NODE_NAME);
  	strcpy(nodeName[1],ACCESSLIMIT_COMMON_NODE_NAME);
	//set totalnum
	if(getAttrValue(top, nodeName, ACCESSLIMIT_TOTALNUM, tmp) == TCAPI_PROCESS_OK)
	{
		sprintf(cmdbuf,"/usr/bin/accesslimitcmd totalnum %s",tmp);
		system(cmdbuf);
	}
	
	//set debug
	memset(cmdbuf,0,sizeof(cmdbuf));
	memset(tmp,0,sizeof(tmp));
	if(getAttrValue(top, nodeName, ACCESSLIMIT_DEBUG, tmp) == TCAPI_PROCESS_OK)
	{
		sprintf(cmdbuf,"/usr/bin/accesslimitcmd debug %s",tmp);
		system(cmdbuf);
	}

	//set livetime
	memset(cmdbuf,0,sizeof(cmdbuf));
	memset(tmp,0,sizeof(tmp));
	if(getAttrValue(top, nodeName, ACCESSLIMIT_LIVETIME, tmp) == TCAPI_PROCESS_OK)
	{
		sprintf(cmdbuf,"/usr/bin/accesslimitcmd livetime %s",tmp);
		system(cmdbuf);
	}

	//set arp leavecount and timeout
	memset(cmdbuf,0,sizeof(cmdbuf));
	memset(tmp,0,sizeof(tmp));
	if(getAttrValue(top, nodeName, ACCESSLIMIT_ARP_TIMEOUT, tmp) == TCAPI_PROCESS_OK)
	{
		if(getAttrValue(top, nodeName, ACCESSLIMIT_ARP_COUNT, tmp1) == TCAPI_PROCESS_OK)
		{
			sprintf(cmdbuf,"/usr/bin/accesslimitcmd arpinfo %s %s",tmp,tmp1);
			system(cmdbuf);
		}
	}

	//set mode (because mode is switch ,so execute at last!)
	memset(cmdbuf,0,sizeof(cmdbuf));
	memset(tmp,0,sizeof(tmp));
	if(getAttrValue(top, nodeName, ACCESSLIMIT_MODE, tmp) == TCAPI_PROCESS_OK)
	{
		sprintf(cmdbuf,"/usr/bin/accesslimitcmd mode %s",tmp);
		system(cmdbuf);
	}
	
	//set device type info(accesslimitcmd deviceinfo <switch> <devicename> <limitnum> on/off computer/stb/phone/camera)
	for(i=0; i<5;i++){
		sprintf(nodeName[1],"Entry%d", i);
		memset(cmdbuf,0,sizeof(cmdbuf));
		memset(tmp,0,sizeof(tmp));
		memset(tmp1,0,sizeof(tmp1));
		memset(tmp2,0,sizeof(tmp2));
		if((getAttrValue(top, nodeName, "Active", tmp) == TCAPI_PROCESS_OK)
		     &&(getAttrValue(top, nodeName, "devtype", tmp1) == TCAPI_PROCESS_OK) 
		     && (getAttrValue(top, nodeName, "limitnum", tmp2) == TCAPI_PROCESS_OK)){
			if(!strcasecmp(tmp,"Yes")){
				sprintf(cmdbuf,"/usr/bin/accesslimitcmd devicetypeinfo on %s %s",tmp1, tmp2);
				system(cmdbuf);
			}
			 else{
				sprintf(cmdbuf,"/usr/bin/accesslimitcmd devicetypeinfo off  %s %s",tmp1, tmp2);
				system(cmdbuf);
			 }	 	
		}
	}
	
	return SUCCESS;
}
#endif

#if defined(TCSUPPORT_CT_FTP_DOWNLOADCLIENT)
/* FTP Client Download */
//"appFTP", ADV_FLAG|ENTRY_TYPE|APPFTPHISTORY_NUM, appftp_adv_table, NULL, NULL, NULL, NULL, appftp_execute, appftp_boot},
char *
appftp_adv_table[MAX_NODE_NAME]=
{
	APPFTP_DOWNLOADING,
	"",
};

/*______________________________________________________________________________
**	appftp_init
**
**	descriptions:
**		the init function of appftp.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
appftp_init(void){
	int ret=0;
	cfg_node_t node={
			.name=APPFTP_NODE,
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
			.type=ADV_FLAG|ENTRY_TYPE|(APPFTPHISTORY_NUM + APPFTPCLIENT_NUM),
#else
			.type=ADV_FLAG|ENTRY_TYPE|APPFTPHISTORY_NUM,
#endif
			.adv_table=appftp_adv_table,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=appftp_execute,
			.cfg_boot=appftp_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}/* end appftp_init */

/*_____________________________________________________________________________
*description:
*	init appFTP node
*param:
**	top: root of mxml_node_t structure
*return int:
*	success 0
*	otherwise -1
*_____________________________________________________________________________
*/
int
appftp_boot(mxml_node_t *top){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	int i;
	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], APPFTP_NODE);
	strcpy(nodeName[1], APPFTP_DOWNLOADING);
	setAttrValue(top, nodeName, APPFTP_USERNAME, APPFTP_INVALID);
	setAttrValue(top, nodeName, APPFTP_PASSWORD, APPFTP_INVALID);
	setAttrValue(top, nodeName, APPFTP_URL, APPFTP_INVALID);
	setAttrValue(top, nodeName, APPFTP_PORT, APPFTP_INVALID);
	setAttrValue(top, nodeName, APPFTP_DEVICEID, APPFTP_INVALID);
	setAttrValue(top, nodeName, APPFTP_SAVEPATH, APPFTP_INVALID);
	setAttrValue(top, nodeName, APPFTP_HISTORYCOUNT, "0");

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], APPFTP_NODE);
	for(i=0;i<APPFTPHISTORY_NUM;i++){
		sprintf(nodeName[1], "%s%d", APPFTP_ENTRY, i);
		setAttrValue(top, nodeName, APPFTP_HISTORYID, APPFTP_INVALID);
		setAttrValue(top, nodeName, APPFTP_USERNAME, APPFTP_INVALID);
		setAttrValue(top, nodeName, APPFTP_PASSWORD, APPFTP_INVALID);
		setAttrValue(top, nodeName, APPFTP_URL, APPFTP_INVALID);
		setAttrValue(top, nodeName, APPFTP_PORT, APPFTP_INVALID);
		setAttrValue(top, nodeName, APPFTP_DEVICEID, APPFTP_INVALID);
		setAttrValue(top, nodeName, APPFTP_SAVEPATH, APPFTP_INVALID);
		setAttrValue(top, nodeName, APPFTP_RESULT, APPFTP_INVALID);
	}
	return SUCCESS;
}/* end appftp_boot */

/*_____________________________________________________________________________
*description:
*	Execute ftpget command by appftp_execute function.
*param:
**	top: root of mxml_node_t structure
*	name[][]: record path of every configure node entries (ex. {{WAN_PVC0},{WAN_PVC1}?K})
*return int:
*	success 0
*	otherwise -1
*_____________________________________________________________________________
*/
int
appftp_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	char downloadNodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char ftpCommand[APPFTP_MAX_COMMAND_LENGTH];
	char url[APPFTP_MAX_URL_LENGTH];
	char *host = NULL;
	char *hostPath = NULL;
	char username[APPFTP_MAX_USERNAME_LENGTH];
	char password[APPFTP_MAX_PASSWORD_LENGTH];
	char port[APPFTP_MAX_PORT_LENGTH];
	char deviceId[APPFTP_MAX_DEVICEID_LENGTH];
	char savePath[APPFTP_MAX_SAVEPATH_LENGTH];
	char historyCount[APPFTP_MAX_HISTORYCOUNT_LENGTH];

	char entryNodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char entryId[APPFTP_MAX_ID_LENGTH];
	char entryUrl[APPFTP_MAX_URL_LENGTH];
	char entryUsername[APPFTP_MAX_USERNAME_LENGTH];
	char entryPassword[APPFTP_MAX_PASSWORD_LENGTH];
	char entryPort[APPFTP_MAX_PORT_LENGTH];
	char entryDeviceId[APPFTP_MAX_DEVICEID_LENGTH];
	char entrySavePath[APPFTP_MAX_SAVEPATH_LENGTH];
	char entryResult[APPFTP_MAX_RESULT_LENGTH];
	int entryIndex = 0;
	int entryIdValue = -1;
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	char enable[8] = {0};
	int tr69FTPClientID = 0;
#endif
	int i;

	//read ftp information which will be downloaded
	memset(downloadNodeName, 0, sizeof(downloadNodeName));
	strcpy(downloadNodeName[0], APPFTP_NODE);
	strcpy(downloadNodeName[1], APPFTP_DOWNLOADING);
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	if(strstr(name[1], "Entry")){
		if(get_entry_number(name[1],"Entry",&tr69FTPClientID) == SUCCESS ){
			if(tr69FTPClientID >= APPFTPHISTORY_NUM && tr69FTPClientID < (APPFTPHISTORY_NUM + APPFTPCLIENT_NUM))
			{
				if(getAttrValue(top, name, APPFTP_PORT, entryPort) != TCAPI_PROCESS_OK){
					setAttrValue(top, name, APPFTP_PORT, "21");
				}
				memset(entryPort, 0, sizeof(entryPort));
				getAttrValue(top, name, "Enable", enable);
				if(strcmp(enable,"1"))
					return SUCCESS;				
				copy_node(downloadNodeName,name);
			}
			else
				return SUCCESS;
		}
		else
			return SUCCESS;
	}
#endif

	
	if(getAttrValue(top, downloadNodeName, APPFTP_USERNAME, username) != TCAPI_PROCESS_OK){
		return FAIL;
	}
	if(getAttrValue(top, downloadNodeName, APPFTP_PASSWORD, password) != TCAPI_PROCESS_OK){
		return FAIL;
	}
	if(getAttrValue(top, downloadNodeName, APPFTP_URL, url) != TCAPI_PROCESS_OK){
		return FAIL;
	}
	if(getAttrValue(top, downloadNodeName, APPFTP_PORT, port) != TCAPI_PROCESS_OK){
		return FAIL;
	}
	if(getAttrValue(top, downloadNodeName, APPFTP_DEVICEID, deviceId) != TCAPI_PROCESS_OK){
		return FAIL;
	}
	if(getAttrValue(top, downloadNodeName, APPFTP_SAVEPATH, savePath) != TCAPI_PROCESS_OK){
		return FAIL;
	}
	if(getAttrValue(top, downloadNodeName, APPFTP_HISTORYCOUNT, historyCount) != TCAPI_PROCESS_OK){
		return FAIL;
	}

	memset(entryNodeName, 0, sizeof(entryNodeName));
	strcpy(entryNodeName[0], APPFTP_NODE);

	entryIndex = atoi(historyCount);

	//if the number of histories is 10; delete the first
	if(entryIndex >= APPFTPHISTORY_NUM){
		entryIndex = APPFTPHISTORY_NUM - 1;

		for(i=0;i<entryIndex;i++){
			sprintf(entryNodeName[1], "%s%d", APPFTP_ENTRY, i+1);
			if(getAttrValue(top, entryNodeName, APPFTP_HISTORYID, entryId) != TCAPI_PROCESS_OK){
				return FAIL;
			}
			if(getAttrValue(top, entryNodeName, APPFTP_USERNAME, entryUsername) != TCAPI_PROCESS_OK){
				return FAIL;
			}
			if(getAttrValue(top, entryNodeName, APPFTP_PASSWORD, entryPassword) != TCAPI_PROCESS_OK){
				return FAIL;
			}
			if(getAttrValue(top, entryNodeName, APPFTP_URL, entryUrl) != TCAPI_PROCESS_OK){
				return FAIL;
			}
			if(getAttrValue(top, entryNodeName, APPFTP_PORT, entryPort) != TCAPI_PROCESS_OK){
				return FAIL;
			}
			if(getAttrValue(top, entryNodeName, APPFTP_DEVICEID, entryDeviceId) != TCAPI_PROCESS_OK){
				return FAIL;
			}
			if(getAttrValue(top, entryNodeName, APPFTP_SAVEPATH, entrySavePath) != TCAPI_PROCESS_OK){
				return FAIL;
			}
			if(getAttrValue(top, entryNodeName, APPFTP_RESULT, entryResult) != TCAPI_PROCESS_OK){
				return FAIL;
			}
			sprintf(entryNodeName[1], "%s%d", APPFTP_ENTRY, i);
			setAttrValue(top, entryNodeName, APPFTP_HISTORYID, entryId);
			setAttrValue(top, entryNodeName, APPFTP_USERNAME, entryUsername);
			setAttrValue(top, entryNodeName, APPFTP_PASSWORD, entryPassword);
			setAttrValue(top, entryNodeName, APPFTP_URL, entryUrl);
			setAttrValue(top, entryNodeName, APPFTP_PORT, entryPort);
			setAttrValue(top, entryNodeName, APPFTP_DEVICEID, entryDeviceId);
			setAttrValue(top, entryNodeName, APPFTP_SAVEPATH, entrySavePath);
			setAttrValue(top, entryNodeName, APPFTP_RESULT, entryResult);
		}

		entryIdValue = atoi(entryId);
		entryIdValue = (entryIdValue + 1) % APPFTP_HISTORYMAXID;
	}

	//insert the Downloading node to history
	sprintf(entryNodeName[1], "%s%d", APPFTP_ENTRY, entryIndex);

	if(entryIdValue == -1){
		entryIdValue = entryIndex;
	}
	sprintf(entryId, "%d", entryIdValue);
	setAttrValue(top, entryNodeName, APPFTP_HISTORYID, entryId);
	setAttrValue(top, entryNodeName, APPFTP_USERNAME, username);
	setAttrValue(top, entryNodeName, APPFTP_PASSWORD, password);
	setAttrValue(top, entryNodeName, APPFTP_URL, url);
	setAttrValue(top, entryNodeName, APPFTP_PORT, port);
	setAttrValue(top, entryNodeName, APPFTP_DEVICEID, deviceId);
	setAttrValue(top, entryNodeName, APPFTP_SAVEPATH, savePath);
	setAttrValue(top, entryNodeName, APPFTP_RESULT, APPFTP_RES_DOWNLOADING);

	//set all the values of Downloading node to "N/A"
	setAttrValue(top, downloadNodeName, APPFTP_USERNAME, APPFTP_INVALID);
	setAttrValue(top, downloadNodeName, APPFTP_PASSWORD, APPFTP_INVALID);
	setAttrValue(top, downloadNodeName, APPFTP_URL, APPFTP_INVALID);
	setAttrValue(top, downloadNodeName, APPFTP_PORT, APPFTP_INVALID);
	setAttrValue(top, downloadNodeName, APPFTP_DEVICEID, APPFTP_INVALID);
	setAttrValue(top, downloadNodeName, APPFTP_SAVEPATH, APPFTP_INVALID);
	entryIndex ++;
	sprintf(historyCount, "%d", entryIndex);
	setAttrValue(top, downloadNodeName, APPFTP_HISTORYCOUNT, historyCount);

	// parse the url
	host = url + 6;
	hostPath = strchr(host, '/');
	if (hostPath){
		*hostPath++ = '\0';
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
		if(tr69FTPClientID)
		sprintf(ftpCommand, "/usr/script/ftpdown.sh '%s' '%s' '%s' '%s' '%s' '%s' '%s' '%s' '%d' &", entryId, host, port, username, password,
		hostPath, deviceId, savePath, tr69FTPClientID);
		else
#endif
		sprintf(ftpCommand, "/usr/script/ftpdown.sh '%s' '%s' '%s' '%s' '%s' '%s' '%s' '%s' &", entryId, host, port, username, password,
				hostPath, deviceId, savePath);
		system(ftpCommand);
	}else{
		setAttrValue(top, entryNodeName, APPFTP_RESULT, APPFTP_RES_BAD_URL);
	}

	sleep(2);
	system("sync");// sync data change to usb device

	return SUCCESS;
}/* end appftp_execute */
#endif

#if defined(TCSUPPORT_CT_USB_BACKUPRESTORE)
/* USB Backup and  Restore */
/*______________________________________________________________________________
**	usbrestore_init
**
**	descriptions:
**		the init function of usbrestore.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int 
usbrestore_init(void){
	int ret=0;
	cfg_node_t node={
			.name = USBRESTORE_NODE,
			.type =  SINGLE_TYPE,
			.adv_table = NULL,
			.merge_table = NULL,
			.cfg_read = NULL,
			.cfg_write = NULL,
			.cfg_verify = NULL,
			.cfg_execute = usbrestore_execute,
			.cfg_boot = NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}/* end usbrestore_init */

/*_____________________________________________________________________________
*description:
*	Execute backuprestorecmd command by usbrestore_execute function.
*param:
**	top: root of mxml_node_t structure
*	name[][]: record path of every configure node entries (ex. {{WAN_PVC0},{WAN_PVC1}?K})
*return int:
*	success 0
*	otherwise -1
*_____________________________________________________________________________
*/
int 
usbrestore_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char webcurnodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char execCMD[USBRESTORE_MAX_COMMAND_LENGTH];
	char target[USBRESTORE_TARGET_LENGTH];
	char dev[USBRESTORE_DEV_LENGTH];
	char opt[USBRESTORE_OPT_LENGTH];
	int ret = -1;
	char result[200];
	char devInfoNodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char ModelName[32];

	memset(nodeName, 0x00, sizeof(nodeName));
	strcpy(nodeName[0], USBRESTORE_NODE);
	strcpy(nodeName[1], "Entry");

	memset(webcurnodeName, 0x00, sizeof(webcurnodeName));
	strcpy(webcurnodeName[0], WEBCURSET);
	strcpy(webcurnodeName[1], SUB_NODE_NAME);

	memset(target, 0x00, sizeof(target));
	memset(dev, 0x00, sizeof(dev));
	memset(opt, 0x00, sizeof(opt));

	if(getAttrValue(top, webcurnodeName, USBRESTORE_OPT, opt) != TCAPI_PROCESS_OK){
		return FAIL;
	}

	memset(devInfoNodeName, 0x00, sizeof(devInfoNodeName));
	strcpy(devInfoNodeName[0], "DeviceInfo");
	strcpy(devInfoNodeName[1], "devParaStatic");
	if(getAttrValue(top, devInfoNodeName, "ModelName", ModelName) == TCAPI_PROCESS_OK){
		sprintf(target,"e8_Config_backup/ctce8_%s.cfg",ModelName);
		if(setAttrValue(top, nodeName, USBRESTORE_TARGET, target) != TCAPI_PROCESS_OK){
			return FAIL;
		}
	}else{
	if(getAttrValue(top, nodeName, USBRESTORE_TARGET, target) != TCAPI_PROCESS_OK){
		return FAIL;
	}
	}

	if(getAttrValue(top, nodeName, USBRESTORE_DEV, dev) != TCAPI_PROCESS_OK){
		return FAIL;
	}

	int op = atoi(opt);
	memset(execCMD, 0x00, sizeof(execCMD));

	switch(op){
		case OPT_NONE:
			return SUCCESS;
		case OPT_DELSAMEFILE:
			sprintf(execCMD, USBRESTORE_EXECCMDDEL, dev, target);
			system(execCMD);
			break;
		case OPT_SETRESULT:
			memset(result, 0x00, sizeof(result));
			sprintf(result, "%d", RESULT_NOACT);
			setAttrValue(top, webcurnodeName, USBRESTORE_RESULT, result);

			memset(opt, 0x00, sizeof(opt));
			sprintf(opt, "%d", OPT_NONE);
			setAttrValue(top, webcurnodeName, USBRESTORE_OPT, opt);
			return SUCCESS;
		default:
			break;
	}

	memset(opt, 0x00, sizeof(opt));
	sprintf(opt, "%d", OPT_NONE);
	setAttrValue(top, webcurnodeName, USBRESTORE_OPT, opt);
	
	memset(result, 0x00, sizeof(result));
	sprintf(result, "%d", RESULT_NOACT);
	setAttrValue(top, webcurnodeName, USBRESTORE_RESULT, result);
	
	memset(execCMD, 0x00, sizeof(execCMD));
	sprintf(execCMD, "/userfs/bin/mtd readflash %s 65536 0 romfile", USBRESTORE_SOURCE);
	system(execCMD);

	memset(execCMD, 0x00, sizeof(execCMD));
	sprintf(execCMD, USBRESTORE_EXECCMDFORMAT, USBRESTORE_SOURCE, dev, target);
	ret = system(execCMD);
	sleep(1);
	ret = WEXITSTATUS(ret);

	memset(execCMD, 0x00, sizeof(execCMD));
	sprintf(execCMD, "rm -f %s", USBRESTORE_SOURCE);
	system(execCMD);

	memset(result, 0x00, sizeof(result));
	sprintf(result, "%d", ret);
	setAttrValue(top, webcurnodeName, USBRESTORE_RESULT, result);

	system("sync");// sync data change to usb device

	return SUCCESS;
}/* end usbrestore_execute */
#endif

#if defined(TCSUPPORT_CT_L2TP_VPN)
#define VPN_PPP_UNIT_START 100
#define VPN_POLICY_TABLE_START 300

static int VPN_Boot = 0;
static int VPN_RestartDNS = 0;
static int VPN_dns_count = 0;

/*
	check VPN PPP tunnel
*/
int checkVPNPPPTunnels(int ponState)
{
#define VPN_PPP_TIMER 15 /* 15 seconds */
#define VPN_RESTART_DNS_TIMER 5 /* 5 seconds */
	static int vpn_ppp_count = 0;
	char nodeName[64] = {0};
	char userIdBuf[128] = {0};
	char lns_addr[64] = {0};
	char vpnstatus[32] = {0};
	char cmdbuf[256] = {0};
	int idx = 0;

	if ( 1 != VPN_Boot || 1 != ponState )
		return -1;

	if ( VPN_RestartDNS && ++VPN_dns_count >= VPN_RESTART_DNS_TIMER )
	{
		VPN_RestartDNS = 0;
		VPN_dns_count = 0;

		system("/usr/bin/killall -9 dnsmasq");
		system("/userfs/bin/dnsmasq &");
		tcdbg_printf("\n vpn restart dnsmasq \n");
	}

	if ( ++vpn_ppp_count >= VPN_PPP_TIMER )
	{
		vpn_ppp_count = 0;
	}
	else
		return -2;


	for ( idx = 0; idx < VPN_INSTANCE_NUM; idx ++ )
	{
		bzero(nodeName, sizeof(nodeName));
		snprintf(nodeName, sizeof(nodeName) - 1, 
			"%s_%s%d", VPNSTR, VPN_ENTRY, idx); 
		bzero(userIdBuf, sizeof(userIdBuf));
		bzero(lns_addr, sizeof(lns_addr));
		bzero(vpnstatus, sizeof(vpnstatus));
		if ( 0 == tcapi_get(nodeName, "userId", userIdBuf)
			&& 0 != userIdBuf[0]
			&& 0 == tcapi_get(nodeName, "serverIpAddr", lns_addr)
			&& 0 != lns_addr[0]
			&& 0 == tcapi_get(nodeName, "vpnstatus", vpnstatus)
			&& 0 != strcmp(vpnstatus, "0") )
		{
			bzero(cmdbuf, sizeof(cmdbuf));
			snprintf(cmdbuf, sizeof(cmdbuf) - 1
				, "echo 'c "VPN_TUNNEL_NAME_PRE"%d' "
				">/var/run/xl2tpd/l2tp-control"
				, idx);
			system(cmdbuf);
		}
	}


	return 0;
}


/*
	clear ebtables for VPN dest IPs
*/
int clear_VPN_LAN_rules()
{
	int idx  = 0;
	char cmdbuf[256] = {0};

	/* flush and zero it */
	for ( idx = 0; idx < VPN_INSTANCE_NUM; idx ++ )
	{
		bzero(cmdbuf, sizeof(cmdbuf));
		snprintf(cmdbuf, sizeof(cmdbuf) - 1
			, "ebtables -t broute -F vpn_lan_entry%d", idx);
		system(cmdbuf);

		bzero(cmdbuf, sizeof(cmdbuf));
		snprintf(cmdbuf, sizeof(cmdbuf) - 1
			, "ebtables -t broute -Z vpn_lan_entry%d", idx);
		system(cmdbuf);
	}

	return 0;
}

/*
	clear nat for vpn ppp
*/
int clear_VPN_PPP_rules()
{
	int idx  = 0, ppp_unit_start = VPN_PPP_UNIT_START;
	char cmdbuf[256] = {0};

	/* delete all rules */
	for ( idx = 0; idx < VPN_INSTANCE_NUM; idx ++ )
	{
		bzero(cmdbuf, sizeof(cmdbuf));
		snprintf(cmdbuf, sizeof(cmdbuf) - 1
			, "iptables -t nat -D POSTROUTING -o ppp%d -j MASQUERADE"
			, ppp_unit_start + idx);
		system(cmdbuf);

		bzero(cmdbuf, sizeof(cmdbuf));
		snprintf(cmdbuf, sizeof(cmdbuf) - 1
			, "/usr/bin/iptables -D FORWARD -o ppp%d"
			" -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu"
			, ppp_unit_start + idx);
		system(cmdbuf);
	}

	return 0;
}

/*
	create nat for vpn ppp
*/
int create_VPN_PPP_rules(int unit)
{
	char cmdbuf[256] = {0};
	int idx = 0;
#define VPN_PPP_FWMAKR_START 0x6e0000
	bzero(cmdbuf, sizeof(cmdbuf));
	snprintf(cmdbuf, sizeof(cmdbuf) - 1
		, "iptables -t nat -A POSTROUTING -o ppp%d -j MASQUERADE"
		, unit);
	system(cmdbuf);

	bzero(cmdbuf, sizeof(cmdbuf));
	snprintf(cmdbuf, sizeof(cmdbuf) - 1
		, "/usr/bin/iptables -A FORWARD -o ppp%d"
		" -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu"
		, unit);
	system(cmdbuf);

	bzero(cmdbuf, sizeof(cmdbuf));
	snprintf(cmdbuf, sizeof(cmdbuf) - 1
		, "iptables -t mangle -D OUTPUT -o ppp%d"
		" -j MARK --set-mark 0x%x/0x7f0000"
		, unit, VPN_PPP_FWMAKR_START + unit - VPN_PPP_UNIT_START);
	system(cmdbuf);	

	/* delete fwmark and ip source table. */
	for ( idx = 0; idx < 2; idx ++)
	{
		bzero(cmdbuf, sizeof(cmdbuf));
		snprintf(cmdbuf, sizeof(cmdbuf) - 1
			, "/usr/bin/ip rule del table ppp%d"
			, VPN_POLICY_TABLE_START + unit - VPN_PPP_UNIT_START);
		system(cmdbuf);	
	}

	return 0;
}

/*
	open all vpn ppp tunnel
*/
int open_VPN_PPP_tunnel(mxml_node_t *top)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char userIdBuf[128] = {0};
	char lns_addr[64] = {0};
	char vpnstatus[32] = {0};
	char cmdbuf[256] = {0};
	int idx = 0;

	bzero(nodeName, sizeof(nodeName));
	strcpy(nodeName[0], VPNSTR);
	for ( idx = 0; idx < VPN_INSTANCE_NUM; idx ++ )
	{
		sprintf(nodeName[1], "%s%d", VPN_ENTRY, idx);
		bzero(userIdBuf, sizeof(userIdBuf));
		bzero(lns_addr, sizeof(lns_addr));
		bzero(vpnstatus, sizeof(vpnstatus));
		if ( 0 == getAttrValue(top, nodeName, "userId", userIdBuf)
			&& 0 != userIdBuf[0]
			&& 0 == getAttrValue(top, nodeName, "serverIpAddr", lns_addr)
			&& 0 != lns_addr[0]
			&& 0 == getAttrValue(top, nodeName, "vpnstatus", vpnstatus)
			&& 0 != strcmp(vpnstatus, "0") )
		{
			bzero(cmdbuf, sizeof(cmdbuf));
			snprintf(cmdbuf, sizeof(cmdbuf) - 1
				, "echo 'c "VPN_TUNNEL_NAME_PRE"%d' "
				">/var/run/xl2tpd/l2tp-control"
				, idx);
			system(cmdbuf);
		}
	}

	return 0;
}


char *vpnlist_adv_table[MAX_NODE_NAME]=
{
	"Common", "DelDomainEntry", "",
};

int
vpnlist_init(void){
	int ret=0;
	cfg_node_t node={
			.name=VPNSTR,
			.type=ADV_FLAG|ENTRY_TYPE|VPN_INSTANCE_NUM,
			.adv_table=vpnlist_adv_table,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=vpnlist_write,
			.cfg_verify=vpnlist_verify,
			.cfg_execute=vpnlist_execute,
			.cfg_boot=vpnlist_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}
int
vpnlist_write(mxml_node_t *top, mxml_node_t *parant)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	int idx = 0, ppp_unit_start = VPN_PPP_UNIT_START;
	int vpn_ppp_unit = 0, dbg_on = 0;
	char userIdBuf[128] = {0};
	char vpn_ppp_file[256] = {0};
	char lns_addr[64] = {0};
	char username[256] = {0}, password[256] = {0};
	char tunnelname[256] = {0}, action[64] = {0};
	char cmdbuf[256] = {0}, dbg_sw[12] = {0}, dbg_buf[64] = {0};
	FILE *fp_conf = NULL, *fp_ppp_conf = NULL;
	const char *global_header = "[global]\n"
							"access control = no\n"
							"port = 1701\n";
	const char *tunne_param = "[lac %s]\n"
							"name = %s\n"
							"lns = %s\n"
							"pppoptfile = "VPN_PPP_PREPATH"vpn_tunnel_%d.l2tpd\n"
							"ppp debug = yes\n";
	const char *ppp_optfile = 
							"user %s\n"
							"password %s\n"
							"unit %d\n"
							"persist\n"
							"mtu 1410\n"
							"mru 1410\n"
							"ipcp-accept-local\n"
							"ipcp-accept-remote\n"
							"noccp\n"
							"noauth\n"
							"usepeerdns\n"
							"connect-delay 500\n"
							"nodeflate\n"
							"nobsdcomp\n"
							"nopcomp\n"
							"noaccomp\n"
							"vpn-pppup-script /etc/ppp/vpn_ip-up\n"
							"vpn-pppdw-script /etc/ppp/vpn_ip-down\n"
							"%s";
							/*
							"maxfail 3\n"
							"idle 1800\n"
							*/

	bzero(nodeName, sizeof(nodeName));
	strcpy(nodeName[0], VPNSTR);
	strcpy(nodeName[1], VPN_COMMON);
	getAttrValue(top, nodeName, "debug", dbg_sw);
	if ( 0 == strcmp(dbg_sw, "Yes") )
		dbg_on = 1;
	if ( 0 == getAttrValue(top, nodeName, VPN_ATTR_ACTION, action)
		&& 0 != action[0] )
	{
		if ( 0 == strcmp(action, VPN_ACT_ATTACH) )
			return SUCCESS;
	}

	/* 1. close vpn ppp connections. */
	for ( idx = 0; idx < VPN_INSTANCE_NUM; idx ++ )
	{
		bzero(cmdbuf, sizeof(cmdbuf));
		snprintf(cmdbuf, sizeof(cmdbuf) - 1
			, "echo 'd "VPN_TUNNEL_NAME_PRE"%d' "
			">/var/run/xl2tpd/l2tp-control"
			, idx);
		system(cmdbuf);
	}

	/* 2. kill xl2tpd */
	system("/usr/bin/killall -SIGTERM xl2tpd");

	/* 3. remove file & create folder */
	system("/bin/rm -rf "VPN_PREPATH"*");
	system("/bin/mkdir -p "VPN_PPP_PREPATH);

	/* 4. clear all rules and re-create it. */
	clear_VPN_LAN_rules();
	clear_VPN_PPP_rules();

	/* 5. create xl2tpd conf */
	fp_conf = fopen(VPN_CONF_PATH, "w");
	if ( !fp_conf )
		return FAIL;

	/* a. write global data. */
	fprintf(fp_conf, "%s", global_header);
	/* b. write tunnel data. */
	bzero(nodeName, sizeof(nodeName));
	strcpy(nodeName[0], VPNSTR);
	for ( idx = 0; idx < VPN_INSTANCE_NUM; idx ++ )
	{
		sprintf(nodeName[1], "%s%d", VPN_ENTRY, idx);
		if ( 0 == getAttrValue(top, nodeName, "userId", userIdBuf)
			&& 0 != userIdBuf[0]
			&& 0 == getAttrValue(top, nodeName, "serverIpAddr", lns_addr)
			&& 0 != lns_addr[0])
		{
			/* c. generate tunnel name */
			bzero(tunnelname, sizeof(tunnelname));
			snprintf(tunnelname, sizeof(tunnelname) - 1
				, VPN_TUNNEL_NAME_PRE"%d", idx);
			setAttrValue(top, nodeName, "tunnelName", tunnelname);
			fprintf(fp_conf, tunne_param, tunnelname
				, tunnelname, lns_addr, idx);
			/* d. write tunnel ppp config file. */
			bzero(vpn_ppp_file, sizeof(vpn_ppp_file));
			snprintf(vpn_ppp_file, sizeof(vpn_ppp_file) - 1
				, VPN_PPP_PREPATH"vpn_tunnel_%d.l2tpd"
				, idx);
			fp_ppp_conf = fopen(vpn_ppp_file, "w");
			if ( fp_ppp_conf )
			{
				getAttrValue(top, nodeName, "username", username);
				getAttrValue(top, nodeName, "password", password);

				if ( 1 == dbg_on )
				{
					snprintf(dbg_buf, sizeof(dbg_buf) - 1
						, "logfile /var/run/xl2tpd/log%d.log\n"
						  "debug\n"
						, idx);
				}

				vpn_ppp_unit = ppp_unit_start + idx;
				fprintf(fp_ppp_conf, ppp_optfile, username, password
					, vpn_ppp_unit
					, ( (1 == dbg_on) ? dbg_buf : "") );
				create_VPN_PPP_rules(vpn_ppp_unit);

				fclose(fp_ppp_conf);
			}

			setAttrValue(top, nodeName, "vpnstatus", "1");
		}
	}

	fclose(fp_conf);

	return SUCCESS;
}

int
vpnlist_verify(mxml_node_t *node){
	return SUCCESS;
}

int
vpnlist_execute(mxml_node_t *top, char name[][MAX_NODE_NAME])
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char action[64] = {0};

	bzero(nodeName, sizeof(nodeName));
	strcpy(nodeName[0], VPNSTR);
	strcpy(nodeName[1], VPN_COMMON);
	if ( 0 == getAttrValue(top, nodeName, VPN_ATTR_ACTION, action)
		&& 0 != action[0] )
	{
		if ( 0 == strcmp(action, VPN_ACT_ATTACH) )
		{
			setAttrValue(top, nodeName, VPN_ATTR_ACTION, "");
			VPN_RestartDNS = 1;
			VPN_dns_count = 0;

			return SUCCESS;
		}
		else if ( 0 == strcmp(action, VPN_ACT_WANRESTART) )
		{
			setAttrValue(top, nodeName, VPN_ATTR_ACTION, "");
			VPN_RestartDNS = 1;
			VPN_dns_count = 5;
		}
	}

	system("/userfs/bin/xl2tpd -c "VPN_CONF_PATH);
	open_VPN_PPP_tunnel(top);

	return SUCCESS;	
}
	
int
vpnlist_boot(mxml_node_t *top)
{
	int idx = 0;
	char cmdbuf[256] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char strValue[64]={0};

	bzero(nodeName, sizeof(nodeName));
	strcpy(nodeName[0], VPNSTR);
	strcpy(nodeName[1], VPN_COMMON);
	setAttrValue(top, nodeName, VPN_ATTR_ACTION, "");

	bzero(nodeName, sizeof(nodeName));
	strcpy(nodeName[0], VPNSTR);
	/* create vpn chains for LAN */
	for ( idx = VPN_INSTANCE_NUM - 1; idx >=0; idx -- )
	{
		bzero(cmdbuf, sizeof(cmdbuf));
		snprintf(cmdbuf, sizeof(cmdbuf) - 1
			, "ebtables -t broute -N vpn_lan_entry%d -P RETURN", idx);
		system(cmdbuf);

		bzero(cmdbuf, sizeof(cmdbuf));
		snprintf(cmdbuf, sizeof(cmdbuf) - 1
			, "ebtables -t broute -I BROUTING -j vpn_lan_entry%d", idx);
		system(cmdbuf);

		/* flush and zero it */
		bzero(cmdbuf, sizeof(cmdbuf));
		snprintf(cmdbuf, sizeof(cmdbuf) - 1
			, "ebtables -t broute -F vpn_lan_entry%d", idx);
		system(cmdbuf);

		bzero(cmdbuf, sizeof(cmdbuf));
		snprintf(cmdbuf, sizeof(cmdbuf) - 1
			, "ebtables -t broute -Z vpn_lan_entry%d", idx);
		system(cmdbuf);

		/* clear ip status */
		bzero(strValue, sizeof(strValue));
		sprintf(nodeName[1], "%s%d", VPN_ENTRY, idx);
		if ( 0 == getAttrValue(top, nodeName, "vpnstatus", strValue)
			&& 0 != strValue[0] )
		{
			setAttrValue(top, nodeName, "ip", "");
			setAttrValue(top, nodeName, "netmask", "");
			setAttrValue(top, nodeName, "gateway", "");
			setAttrValue(top, nodeName, "status", "down");
			setAttrValue(top, nodeName, "vpnstatus", "1");
		}
	}

	system("/userfs/bin/xl2tpd -c "VPN_CONF_PATH);
	VPN_Boot = 1;

	return SUCCESS;
	
}
#endif

#if defined(TCSUPPORT_CT_PHONEAPP)
#if defined(TCSUPPORT_CT_JOYME)
#define PLUGIN_C_PATHS			"/usr/osgi/plugin-c" 	/* path for plugin c*/
#define PLUGIN_B_PATHS			"/usr/osgi/plugin-b" 	/* path for plugin b*/
#define PLUGIN_C_PIDS			"/usr/osgi/plugin-c/%s/pid"  /* pid for plugin c*/
#define PLUGIN_B_STATES			"/usr/osgi/plugin-b/%s/state"  /* state for plugin b*/

/*
*description: load all bundle list
*return	0:success -1:fail
*/
int load_pluginlist(mxml_node_t *top){
	DIR *dir;
	FILE *fp = NULL;
	struct dirent *dirptr;
	unsigned int num = 0; 
	int ret = 0;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char buffer[256] = {0};

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], PLUGINSTR);
	strcpy(nodeName[1], PLUGIN_ENTRY);

	dir = opendir(PLUGIN_C_PATHS);
	if (NULL == dir){
		tcdbg_printf("%s:open C dir err!!\n", __FUNCTION__);
		return 0;
	}
	while ((dirptr = readdir(dir)) != NULL) {
		if ((strcmp(dirptr->d_name, ".") == 0) || (strcmp(dirptr->d_name, "..") == 0))
			continue;
		if (DT_DIR != dirptr->d_type) 
			continue;
		sprintf(nodeName[1]+5, "%d", num++);
		setAttrValue(top,nodeName,"Name",dirptr->d_name);
		snprintf(buffer, sizeof(buffer), PLUGIN_C_PIDS, dirptr->d_name);	
		ret = access( buffer, F_OK );
		setAttrValue(top,nodeName,"Run", ret ? "0" : "1");
	}
	closedir(dir);

	dir = opendir(PLUGIN_B_PATHS);
	if (NULL == dir){
		tcdbg_printf("%s:open B dir err!!\n", __FUNCTION__);
		return 0;
	}
	while ((dirptr = readdir(dir)) != NULL) {
		if ((strcmp(dirptr->d_name, ".") == 0) || (strcmp(dirptr->d_name, "..") == 0))
			continue;
		if (DT_DIR != dirptr->d_type) 
			continue;
		sprintf(nodeName[1]+5, "%d", num++);
		setAttrValue(top,nodeName,"Name",dirptr->d_name);
		snprintf(buffer, sizeof(buffer), PLUGIN_B_STATES, dirptr->d_name);
		fp = fopen(buffer, "r");
		if(fp != NULL){
			if(fgets(buffer, sizeof(buffer), fp) != NULL){
				ret = atoi(buffer);
			}
			fclose(fp);
		}
		setAttrValue(top,nodeName,"Run", (ret == 1) ? "1" : "0");
	}
	closedir(dir);
	
	sprintf(buffer, "%d", num);
	strcpy(nodeName[1], PLUGIN_COMMON);
	setAttrValue(top, nodeName, "Num", buffer);
	
	return 0;	
}

char *pluginlist_adv_table[MAX_NODE_NAME]=
{
	"Common","",
};
/*_____________________________________________________________________________
**      function name: pluginlist_read
**      descriptions:
**            To create pluginlist cfg node.
**
**      parameters:
**            top:    Put the top of cfg node tree.
**            name:  NULL
**
**      global:
**             None
**
**      return:
**             Success: 	0
**		   FAIL: -1
**
**____________________________________________________________________________
*/
int
pluginlist_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr){
	
	if(!strcmp(name[1], PLUGIN_COMMON)){
		load_pluginlist(top);
	}	
	
	return SUCCESS;
}/*end pluginlist_read*/

/*______________________________________________________________________________
**	pluginlist_init
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
pluginlist_init(void){
	int ret=0;
	cfg_node_t node={
			.name=PLUGINSTR,
			.type=ADV_FLAG|WEB_TYPE|ENTRY_TYPE|UPDATE_TYPE|PLUGIN_INSTANCE_NUM,
			.adv_table=pluginlist_adv_table,
			.merge_table=NULL,
			.cfg_read=pluginlist_read,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=NULL,
			.cfg_boot=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}

char *usbmount_adv_table[MAX_NODE_NAME]=
{
	"Common","",
};

int
usbmount_init(void){
	int ret=0;
	cfg_node_t node={
			.name=USBMOUNTSTR,
			.type=ADV_FLAG|ENTRY_TYPE|USBMOUNT_INSTANCE_NUM|WEB_TYPE,
			.adv_table=usbmount_adv_table,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=usbmount_write,
			.cfg_verify=usbmount_verify,
			.cfg_execute=usbmount_execute,
			.cfg_boot=usbmount_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}
int
usbmount_write(mxml_node_t *top, mxml_node_t *parant){
	return SUCCESS;
}

int
usbmount_verify(mxml_node_t *node){
	return SUCCESS;
}

int
usbmount_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	return SUCCESS;
	
}
	
int
usbmount_boot(mxml_node_t *top){
	return SUCCESS;
	
}
#endif
char *
mobile_adv_table[MAX_NODE_NAME]= 
{
#if defined(TCSUPPORT_CT_JOYME)
	"DnsRedirect", "Sleep", "WifiEx", "OSRight","PluginMonitor","",
#else
	"Entry", "DnsRedirect", "",
#endif
};

int
mobile_init(void){
	int ret=0;
	cfg_node_t node={
			.name="mobile",
			.type=ADV_FLAG|SINGLE_TYPE,
			.adv_table=mobile_adv_table,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=mobile_write,
			.cfg_verify=mobile_verify,
			.cfg_execute=mobile_execute,
			.cfg_boot=mobile_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}

int
mobile_write(mxml_node_t *top, mxml_node_t *parant){
	return SUCCESS;
}

int
mobile_verify(mxml_node_t *node){
	return SUCCESS;
}

int
mobile_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){

	return SUCCESS;
}
	
int
mobile_boot(mxml_node_t *top){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char buffer[4] = "\0"; 

	memset(nodeName,0,sizeof(nodeName));
  	strcpy(nodeName[0],"mobile");
  	strcpy(nodeName[1],"Entry");
	
	if(getAttrValue(top, nodeName, "test", buffer) == TCAPI_PROCESS_OK){
		if(atoi(buffer) == 1){
			system("/usr/local/ct/mobile-manager &");
			return SUCCESS;
		}
	}
	system("/userfs/bin/mobile-manager &");
	return SUCCESS;
}
#endif
#if defined(TCSUPPORT_CT_UPNP_DM)
/*______________________________________________________________________________
**	upnpddm_init
**
**	descriptions:
**		the init function of upnpd.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
upnp_dm_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="UPnPDM",
			.type=SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=upnp_dm_write,
			.cfg_verify=upnp_dm_verify,
			.cfg_execute=upnp_dm_execute,
			.cfg_boot=upnp_dm_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}
/*______________________________________________________________________________
**	upnp_dm_write
**
**	descriptions:
**		
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  upnpd cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
upnp_dm_write(mxml_node_t *top, mxml_node_t *parant){
	return SUCCESS;
}/* end upnp_dm_write */

/*______________________________________________________________________________
**	upnpd_verify
**
**	descriptions:
**		To check the system parameter of upnp_dm element  is validly or not.
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
upnp_dm_verify(mxml_node_t *node){
	return SUCCESS;
}/* end upnpd_verify */

/*_____________________________________________________________________________
**      function name: upnp_dm_execute
**      descriptions:
**            To execute upnpd node service.
**
**      parameters:
**            node:   Put the ripd of cfg node tree that is used to upnp_dm information.
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
**____________________________________________________________________________
*/
int
upnp_dm_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){

	return SUCCESS;
}/* end upnp_dm_execute */

int
upnp_dm_boot(mxml_node_t *top){
	return SUCCESS;
}

/*______________________________________________________________________________
**	upnpdm_SoftwareProfile_boot
**
**	descriptions:
**		the boot function of UPnPDM_SoftwareProfile.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**_______________________________________________________________________________
*/
int
upnpdm_SoftwareProfile_boot(mxml_node_t *top){
	return SUCCESS;
}

char *
softwareProfile_adv_table[MAX_NODE_NAME]=
{
	"Common","",
};

#define SOFTWARE_FILE_PROFILE_MAXNUM 				4		
/*______________________________________________________________________________
**	upnpdm_SoftwareProfile_init
**
**	descriptions:
**		the init function of UPnPDM_SoftwareProfile.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
upnpdm_SoftwareProfile_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="UPnPDMSWProfile",
			.type=ADV_FLAG|ENTRY_TYPE|SOFTWARE_FILE_PROFILE_MAXNUM,
			.adv_table=softwareProfile_adv_table,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=upnpdm_SoftwareProfile_write,
			.cfg_verify=upnpdm_SoftwareProfile_verify,
			.cfg_execute=upnpdm_SoftwareProfile_execute,
			.cfg_boot=upnpdm_SoftwareProfile_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}
/*______________________________________________________________________________
**	upnp_SoftwareProfile_write
**
**	descriptions:
**		
**		
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  upnpd cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
upnpdm_SoftwareProfile_write(mxml_node_t *top, mxml_node_t *parant){
	return SUCCESS;
}

/*______________________________________________________________________________
**	upnpdm_SoftwareProfile_verify
**
**	descriptions:
**		To check the system parameter of upnpd element  is validly or not.
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
upnpdm_SoftwareProfile_verify(mxml_node_t *node){
	return SUCCESS;
}
/*_____________________________________________________________________________
**      function name: upnpdm_SoftwareProfile_execute
**      descriptions:
**            To execute UPnPDM_SoftwareProfile.
**
**      parameters:
**            node:   Put the ripd of cfg node tree that is used to upnpd information.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**____________________________________________________________________________
*/
int
upnpdm_SoftwareProfile_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	return SUCCESS;
}


enum templateType{
	AP_Internet=0,
	AP_IPTV,
	AP_VoIP,
	STB_Internet,
	STB_IPTV,
	STB_VoIP,
	Screen4_Internet,
	Screen4_IPTV,
	Screen4_VoIP,
	HomeCenter_Internet,
	HomeCenter_NAS,
	HomeCenter_IPTV,
	HomeCenter_VoIP,
	Plc
};


//Parse the node which is modify, eg. NodeModify{i}="Enable,SSID,BeaconType"
int serviceProfileNodeModifyParse(mxml_node_t *top, int templateType, int entryIdx){	
	char node_name[MAX_ARG_NUM][MAX_NODE_NAME];
	char node_attr[MAXLEN_ATTR_NAME];
	char node_value[MAXLEN_TCAPI_MSG];
	char tmp[MAXLEN_TCAPI_MSG];
	int num = 0;
	int i,len;
	char* p = NULL;
	char* delim = ",";
	
	tcdbg_printf("%s:templateType=%d,entryIdx=%d\n",__FUNCTION__,templateType,entryIdx);
	switch(templateType){
		case AP_Internet:
			//get the current number of NodeModify				
			memset(tmp, 0, MAXLEN_TCAPI_MSG);
			memset(node_name, 0, sizeof(node_name));
			strcpy(node_name[0], "UPnPDMAPInt");
			sprintf(node_name[1], "Entry%d", entryIdx); 	
			if (getAttrValue(top, node_name, "num", tmp) == TCAPI_PROCESS_OK) {
				num = atoi(tmp);
				tcdbg_printf("%s:num=%d\n",__FUNCTION__,num);
				for(i = 0; i < num; i++){
					//get the NodeModify{num}					
					memset(tmp, 0, MAXLEN_TCAPI_MSG);
					memset(node_attr, 0, MAXLEN_ATTR_NAME);
					sprintf(node_attr, "NodeModify%d", i+1);	
					if (getAttrValue(top, node_name, node_attr, tmp) == TCAPI_PROCESS_OK) {
						tcdbg_printf("%s:tmp=%s\n",__FUNCTION__,tmp);
						p = strtok(tmp,delim);
						while(p){								
							memset(node_value, 0, MAXLEN_TCAPI_MSG);
							if (getAttrValue(top, node_name, p, node_value) == TCAPI_PROCESS_OK) {
								tcdbg_printf("%s:%s=%s\n",__FUNCTION__,p,node_value);
							}else{
								tcdbg_printf("%s:%s not exist\n",__FUNCTION__,p);
							}
							p = strtok(NULL,delim);
						}
					}else{
						tcdbg_printf("%s:node_attr=%s not exist\n",__FUNCTION__,node_attr);
					}
					
				}
			}	
			else{
				tcdbg_printf("%s:node_name=%s\n",__FUNCTION__,node_name,p);
			}
			break;
		case AP_IPTV:
			break;
		default:
			break;
	}

	return 0;
}


/*______________________________________________________________________________
**	upnpdm_ServiceProfile_boot
**
**	descriptions:
**		the boot function of UPnPDM_ServiceProfile.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**_______________________________________________________________________________
*/
int
upnpdm_ServiceProfile_boot(mxml_node_t *top){
	return SUCCESS;
}

char *
serviceProfile_adv_table[MAX_NODE_NAME]=
{
	"CfgTemplate","CfgProfile","",
};

#define SERVICE_CONFIG_PROFILE_MAXNUM				4		
/*______________________________________________________________________________
**	upnpdm_ServiceProfile_init
**
**	descriptions:
**		the init function of UPnPDM_ServiceProfile.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
upnpdm_ServiceProfile_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="UPnPDMService",
			.type=ADV_FLAG|ENTRY_TYPE|SERVICE_CONFIG_PROFILE_MAXNUM,
			.adv_table=serviceProfile_adv_table,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=upnpdm_ServiceProfile_write,
			.cfg_verify=upnpdm_ServiceProfile_verify,
			.cfg_execute=upnpdm_ServiceProfile_execute,
			.cfg_boot=upnpdm_ServiceProfile_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}
/*______________________________________________________________________________
**	upnpdm_ServiceProfile_write
**
**	descriptions:
**		
**		
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  upnpd cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
upnpdm_ServiceProfile_write(mxml_node_t *top, mxml_node_t *parant){
	return SUCCESS;
}
/*______________________________________________________________________________
**	upnpdm_ServiceProfile_verify
**
**	descriptions:
**		To check the system parameter of upnpd element  is validly or not.
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
upnpdm_ServiceProfile_verify(mxml_node_t *node){
	return SUCCESS;
}

/*_____________________________________________________________________________
**      function name: upnpdm_ServiceProfile_execute
**      descriptions:
**            To execute UPnPDM_SoftwareProfile.
**
**      parameters:
**            node:   Put the ripd of cfg node tree that is used to upnpd information.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**____________________________________________________________________________
*/
int
upnpdm_ServiceProfile_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	return SUCCESS;
}

/*______________________________________________________________________________
**	upnpdm_APInternet_boot
**
**	descriptions:
**		the boot function of UPnPDM_APInternet.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**_______________________________________________________________________________
*/
int
upnpdm_APInternet_boot(mxml_node_t *top){
	return SUCCESS;
}

#define AP_MAX_WLANNUM				4		
/*______________________________________________________________________________
**	upnpdm_APInternet_init
**
**	descriptions:
**		the init function of UPnPDM_ServiceProfile.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
upnpdm_APInternet_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="UPnPDMAPInt",
			.type=ENTRY_TYPE|AP_MAX_WLANNUM,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=upnpdm_APInternet_write,
			.cfg_verify=upnpdm_APInternet_verify,
			.cfg_execute=upnpdm_APInternet_execute,
			.cfg_boot=upnpdm_APInternet_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}
/*______________________________________________________________________________
**	upnpdm_APInternet_write
**
**	descriptions:
**		
**		
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  upnpd cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
upnpdm_APInternet_write(mxml_node_t *top, mxml_node_t *parant){
	return SUCCESS;
}

/*______________________________________________________________________________
**	upnpdm_APInternet_verify
**
**	descriptions:
**		To check the system parameter of upnpd element  is validly or not.
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
upnpdm_APInternet_verify(mxml_node_t *node){
	return SUCCESS;
}

/*_____________________________________________________________________________
**      function name: upnpdm_APInternet_execute
**      descriptions:
**            To execute UPnPDM_SoftwareProfile.
**
**      parameters:
**            node:   Put the ripd of cfg node tree that is used to upnpd information.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**____________________________________________________________________________
*/
int
upnpdm_APInternet_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	serviceProfileNodeModifyParse(top,AP_Internet,0);
	return SUCCESS;
}


int
upnpdm_APIptv_boot(mxml_node_t *top){
	return SUCCESS;
}

int
upnpdm_APIptv_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="UPnPDMAPIptv",
			.type=ENTRY_TYPE|AP_MAX_WLANNUM,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=upnpdm_APIptv_write,
			.cfg_verify=upnpdm_APIptv_verify,
			.cfg_execute=upnpdm_APIptv_execute,
			.cfg_boot=upnpdm_APIptv_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}
int
upnpdm_APIptv_write(mxml_node_t *top, mxml_node_t *parant){
	return SUCCESS;
}


int
upnpdm_APIptv_verify(mxml_node_t *node){
	return SUCCESS;
}


int
upnpdm_APIptv_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	return SUCCESS;
}


#endif

#if defined(TCSUPPORT_CT_PON_JS) || defined(TCSUPPORT_CT_NAMECHGNOTIFY)
#if defined(TCSUPPORT_CT_NAMECHGNOTIFY)
#define __zero(x) memset(x, 0, sizeof(x))
#endif
#ifdef TCSUPPORT_WLAN
#define MAX_BSSID_LEN 33
static char nameChangeSsid[MAX_BSSID_NUM][MAX_BSSID_LEN] = {0};
void doNameChangeWLanInit(mxml_node_t *top)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char ssid[MAX_BSSID_LEN] = {0};
	int i = 0;

	for(i = 0; i < MAX_BSSID_NUM; i++){
		memset(nameChangeSsid[i], 0, sizeof(nameChangeSsid[i]));
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0],WLAN);
		sprintf(nodeName[1], "%s%d", ENTRY_NODENAME, i);
		if(getAttrValue(top, nodeName, "SSID", ssid) == TCAPI_PROCESS_OK ){
			strncpy(nameChangeSsid[i], ssid, MAX_BSSID_LEN - 1);
		}
	}
	
	return;
}
#endif

#define MAX_VLANIDMark_LEN 16
static char nameChangeVLANIDMark[PVC_NUM][MAX_VLANIDMark_LEN] = {0};
static char nameChangeServiceList[PVC_NUM*MAX_SMUX_NUM][SERVICE_LIST_LEN] = {0};
static char nameChangePppUsername[PVC_NUM*MAX_SMUX_NUM][128] = {0};
#ifdef TCSUPPORT_CT_PON_CN_JS
static int  nameChangeWanValid[PVC_NUM] = {0};
#endif

void doNameChangeWanInit(mxml_node_t *top)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char vlanid[MAX_VLANIDMark_LEN] = {0};
	char srvList[SERVICE_LIST_LEN] = {0};
	char mode[32] = {0};
	int pvc_index = 0;
	int entry_index = 0;
	int if_index = 0;
	char isp[32] = {0};
	char pppusername[128] = {0};
#if defined(TCSUPPORT_CT_E8B_ADSL)
	char vpi[8] = {0}, vci[8] = {0}, dest[16] = {0};
#endif
	
	memset(nameChangeServiceList, 0, sizeof(nameChangeServiceList));
	memset(nameChangeVLANIDMark, 0, sizeof(nameChangeVLANIDMark));
	memset(nameChangePppUsername, 0, sizeof(nameChangePppUsername));
	for(pvc_index = 0; pvc_index < PVC_NUM; pvc_index++){
		//init VLANIDMARK
		memset(mode, 0, sizeof(mode));		
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0],WAN);
		sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
		
#if defined(TCSUPPORT_CT_E8B_ADSL)
		/* ad wan isn't exist */
		if ( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "VPI", vpi) )
			continue;

		getAttrValue(top, nodeName, "VCI", vci);
		snprintf(dest, sizeof(dest) - 1, "%s:%s", vpi, vci);
		strcpy(nameChangeVLANIDMark[pvc_index], dest);
#else
		if(getAttrValue(top, nodeName, "VLANMode", mode) != TCAPI_PROCESS_OK ){
			continue;
		}
#ifdef TCSUPPORT_CT_PON_CN_JS		
		nameChangeWanValid[pvc_index] = 1;
#endif		
		if ( 0 == strcmp(mode, "TAG") )
		{
			if(getAttrValue(top, nodeName, "VLANID", vlanid) != TCAPI_PROCESS_OK ){
				tcdbg_printf("[%s]VLANID is not exist!if_index=%d\n",__FUNCTION__,if_index);
				continue;
			}else{
				strncpy(nameChangeVLANIDMark[pvc_index], vlanid,MAX_VLANIDMark_LEN - 1);
			}
		}			
#endif

		//init ServiceList,USERNAME
		for(entry_index = 0; entry_index < MAX_SMUX_NUM; entry_index++){
			if_index = pvc_index * PVC_NUM + entry_index;
			memset(srvList, 0, sizeof(srvList));			
			sprintf(nodeName[2], "%s%d", WAN_INTERFACE, entry_index);
			if(getAttrValue(top, nodeName, "ServiceList", srvList) != TCAPI_PROCESS_OK ){
				continue;
			}else{
				strncpy(nameChangeServiceList[if_index], srvList,SERVICE_LIST_LEN - 1);
				if (NULL != strstr(srvList, INTERNET)) {/* only for INTERNET type connection */
					memset(isp, 0, sizeof(isp));
					getAttrValue(top, nodeName, "ISP", isp);
					if (0 == strcmp(isp, "2")) {/* only for ppp connetion */
						if(getAttrValue(top, nodeName, "USERNAME", pppusername) != TCAPI_PROCESS_OK ){
							continue;
						}else{	
							strncpy(nameChangePppUsername[if_index], pppusername,128);
			}
		}
	}
			}
		}
	}

	return;
}

void checkNameChange(mxml_node_t *top)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char ssid[32] = {0};
	char mode[32] = {0};
	char vlanid[20] = {0};
	char srvList[SERVICE_LIST_LEN] = {0};
	int pvc_index = 0;
	int entry_index = 0;
	int if_index = 0;
	int i = 0;
	int isChange =0;
	char isp[32] = {0};
	char pppusername[128] = {0};
#if defined(TCSUPPORT_CT_E8B_ADSL)
	char vpi[8] = {0}, vci[8] = {0}, dest[16] = {0};
#endif

	for(pvc_index = 0; pvc_index < PVC_NUM; pvc_index++){
		memset(mode, 0, sizeof(mode));		
		memset(vlanid, 0, sizeof(vlanid));			
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0],WAN);
		sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);

#if defined(TCSUPPORT_CT_E8B_ADSL)
		/* ad wan isn't exist */
		if ( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "VPI", vpi) )
		{
			if ( 0 != nameChangeVLANIDMark[pvc_index][0] )
			{
				isChange = 1;
				__zero(nameChangeVLANIDMark[pvc_index]);
			}

			for ( entry_index = 0; entry_index < MAX_SMUX_NUM; entry_index++ )
			{
				if_index = pvc_index * MAX_SMUX_NUM + entry_index;
				if ( 0 != nameChangeServiceList[if_index][0] )
				{
					isChange = 1;
					__zero(nameChangeServiceList[if_index]);
				}
			}
			continue;
		}

		/* check vpi/vci whether changed */
		getAttrValue(top, nodeName, "VCI", vci);
		snprintf(dest, sizeof(dest) - 1, "%s:%s", vpi, vci);
		if ( 0 != strcmp(dest, nameChangeVLANIDMark[pvc_index]) )
		{
			isChange = 1;
			strcpy(nameChangeVLANIDMark[pvc_index], dest);
		}
#else
		if(getAttrValue(top, nodeName, "VLANMode", mode) != TCAPI_PROCESS_OK){
			//wan is not exist
#ifdef TCSUPPORT_CT_PON_CN_JS
			if(1 == nameChangeWanValid[pvc_index]){
				isChange = 1;
				nameChangeWanValid[pvc_index] = 0;
			}
#endif					

			if(strlen(nameChangeVLANIDMark[pvc_index]) != 0){
				isChange = 1;				
				strcpy(nameChangeVLANIDMark[pvc_index], "");
			}
			
			for(entry_index = 0; entry_index < MAX_SMUX_NUM; entry_index++){
				if_index = pvc_index * PVC_NUM + entry_index;
				if(strlen(nameChangeServiceList[if_index]) != 0){					
					isChange = 1;					
					strcpy(nameChangeServiceList[if_index], "");
				}
			}
			continue;
		}
#ifdef TCSUPPORT_CT_PON_CN_JS
		else{
			if(0 == nameChangeWanValid[pvc_index]){
				isChange = 1;
				nameChangeWanValid[pvc_index] = 1;
			}
		}		
#endif
		//check VLANID
		if(0 == strcmp(mode, "TAG") && getAttrValue(top, nodeName, "VLANID", vlanid) == TCAPI_PROCESS_OK){	
			if(strcmp(nameChangeVLANIDMark[pvc_index],vlanid) != 0){
				isChange = 1;						
				strcpy(nameChangeVLANIDMark[pvc_index], vlanid);
			}
		}else{
			if(strlen(nameChangeVLANIDMark[pvc_index]) != 0){
				isChange = 1;				
			}
			
			strcpy(nameChangeVLANIDMark[pvc_index], "");
		}
#endif
		
		//check ServiceList,USERNAME 
		for(entry_index = 0; entry_index < MAX_SMUX_NUM; entry_index++){
			if_index = pvc_index * PVC_NUM + entry_index;
			memset(srvList, 0, sizeof(srvList));			
			sprintf(nodeName[2], "%s%d", WAN_INTERFACE, entry_index);
			if(getAttrValue(top, nodeName, "ServiceList", srvList) != TCAPI_PROCESS_OK ){
				//wan is not exist
				if(strlen(nameChangeServiceList[if_index]) != 0){
					isChange = 1;				
					strcpy(nameChangeServiceList[if_index], "");
				}
			}else{
				if(strcmp(nameChangeServiceList[if_index],srvList) != 0){
					isChange = 1;		
					strcpy(nameChangeServiceList[if_index], srvList);
				}
			}
			if((getAttrValue(top, nodeName, "USERNAME", pppusername) != TCAPI_PROCESS_OK )
				|| (getAttrValue(top, nodeName, "ISP", isp) != TCAPI_PROCESS_OK )
				|| (NULL == strstr(srvList, INTERNET) )
				|| (0 != strcmp(isp, "2"))){
				if(strlen(nameChangePppUsername[if_index]) != 0){
					isChange = 1;
					strcpy(nameChangePppUsername[if_index], "");
				}
			}else{
				if(strcmp(nameChangePppUsername[if_index],pppusername) != 0){
					isChange = 1;
					strcpy(nameChangePppUsername[if_index], pppusername);
				}
			}			
		}
	}

#ifdef TCSUPPORT_WLAN
	for(i=0; i<MAX_BSSID_NUM; i++){
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0],WLAN);
		sprintf(nodeName[1], "%s%d", ENTRY_NODENAME, i);
		if(getAttrValue(top, nodeName, "SSID", ssid) == TCAPI_PROCESS_OK ){
			if(strcmp(nameChangeSsid[i],ssid) != 0){	
				isChange = 1;						
				strcpy(nameChangeSsid[i], ssid);
			}
		}else{
			if(strlen(nameChangeSsid[i]) != 0){
				isChange = 1;						
			}
			strcpy(nameChangeSsid[i],"");
		}
	}
#endif

	if(isChange == 1){
		signalCwmpInfo(13,1,0);
	}
}

#endif

#if defined(TCSUPPORT_CT_JOYME)
char *
sar_adv_table[MAX_NODE_NAME]=
{
	"Common", "",
};

int
storage_access_right_init(void)
{
	int ret=0;
	cfg_node_t node = {
		.name="StorageAccessRight",
		.type=ADV_FLAG|ENTRY_TYPE|MAX_SAR_RULE,
		.adv_table=sar_adv_table,
		.merge_table=NULL,
		.cfg_read=NULL,
		.cfg_write=storage_access_right_write,
		.cfg_verify=NULL,
		.cfg_execute=storage_access_right_execute,
		.cfg_boot=storage_access_right_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}

/*______________________________________________________________________________
**	storage_access_right_write
**
**	descriptions:
**		the write config function of StorageAccessRight.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
storage_access_right_write(mxml_node_t *top, mxml_node_t *parant)
{
	FILE *fp;
	int idx = 0;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char macBuf[20] = {0};

	fp = fopen(STORAGE_ACCESS_SH, "w+");
	if ( !fp )
		return FAIL;

	fputs("iptables -t filter -F STORAGE_ACCESS_LIMIT \n", fp);
	fputs("iptables -t filter -Z STORAGE_ACCESS_LIMIT \n", fp);

	bzero(nodeName, sizeof(nodeName));
	snprintf(nodeName[0], sizeof(nodeName[0]) - 1
			, "%s", "StorageAccessRight");
	for ( idx = 0; idx < MAX_SAR_RULE; idx++ )
	{
		snprintf(nodeName[1], sizeof(nodeName[1]) - 1
			,ENTRY_NODENAME, idx);
		if ( 0 == getAttrValue(top, nodeName, "MacAddr", macBuf)
			&& 0 != macBuf[0] )
		{
			fprintf(fp
			,"iptables -t filter -A STORAGE_ACCESS_LIMIT -i br0 "
			"-m mac --mac-source %s -p tcp --dport 21 -j DROP \n", macBuf);
		}
	}
	fclose(fp);

	chmod(STORAGE_ACCESS_SH, 777);

	return SUCCESS;
}

/*______________________________________________________________________________
**	storage_access_right_boot
**
**	descriptions:
**		the boot function of StorageAccessRight.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int storage_access_right_boot(mxml_node_t *top)
{
	system("iptables -t filter -N STORAGE_ACCESS_LIMIT");
	system("iptables -t filter -A INPUT -j STORAGE_ACCESS_LIMIT");
	system("iptables -t filter -F STORAGE_ACCESS_LIMIT");
	system("iptables -t filter -Z STORAGE_ACCESS_LIMIT");

	system(STORAGE_ACCESS_SH);
	unlink(STORAGE_ACCESS_SH);

	return SUCCESS;
}
/*______________________________________________________________________________
**	storage_access_right_execute
**
**	descriptions:
**		the execute function of StorageAccessRight.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int storage_access_right_execute
(mxml_node_t *top, char name[][MAX_NODE_NAME])
{
	system(STORAGE_ACCESS_SH);
	unlink(STORAGE_ACCESS_SH);

	return SUCCESS;
}

char *
awd_adv_table[MAX_NODE_NAME]=
{
	"Common", "",
};

int
attach_wan_dnstunnel_init(void)
{
	int ret=0;
	cfg_node_t node = {
		.name="AttachWanDNSTunnel",
		.type=PVC_TYPE|ADV_FLAG|ENTRY_TYPE|AWN_DOMAIN_NUM,
		.adv_table=awd_adv_table,
		.merge_table=NULL,
		.cfg_read=NULL,
		.cfg_write=NULL,
		.cfg_verify=NULL,
		.cfg_execute=attach_wan_dnstunne_execute,
		.cfg_boot=attach_wan_dnstunne_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}


int attach_wan_dnstunne_boot(mxml_node_t *top)
{
	return SUCCESS;
}

int attach_wan_dnstunne_execute
(mxml_node_t *top, char name[][MAX_NODE_NAME])
{
	system("/usr/bin/killall -9 dnsmasq");
	system("/userfs/bin/dnsmasq &");

	return SUCCESS;
}

char *
dnslimit_adv_table[MAX_NODE_NAME]=
{
	"Common", "",
};

int
dns_speed_limit_init(void)
{
	int ret=0;
	cfg_node_t node = {
		.name="DnsSpeedLimit",
		.type=ADV_FLAG|ENTRY_TYPE|MAX_DNS_LIMIT_RULE,
		.adv_table=dnslimit_adv_table,
		.merge_table=NULL,
		.cfg_read=NULL,
		.cfg_write=NULL,
		.cfg_verify=NULL,
		.cfg_execute=dns_speed_limit_execute,
		.cfg_boot=dns_speed_limit_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}


int dns_speed_limit_boot(mxml_node_t *top)
{
	return SUCCESS;
}

int dns_speed_limit_execute
(mxml_node_t *top, char name[][MAX_NODE_NAME])
{
	system("/usr/bin/killall -9 dnsmasq");
	system("/userfs/bin/dnsmasq &");

	return SUCCESS;
}

#endif

