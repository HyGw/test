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
//#ifdef TCSUPPORT_QOS
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/utsname.h>
#include <arpa/inet.h>

#include <fcntl.h>
#include <net/if.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/in.h>
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
#include "sim.h"
#endif
#include <linux/qos_type.h>
//#endif

//#include "../TR69_64/rompage/includes/AsEngine.h"

#ifndef PURE_BRIDGE
#if QOS_REMARKING  /*Rodney_20090724*/

enum qos_attr_index{
	QOS_PHYPORTETH0 = 0,
	QOS_PHYPORTETH1,
	QOS_PHYPORTETH2,
	QOS_PHYPORTETH3,
	QOS_PHYPORTUSB0,
	QOS_PHYPORTWLAN0,
#ifdef MBSS_SUPPORT
	QOS_PHYPORTWLAN1,
	QOS_PHYPORTWLAN2,
	QOS_PHYPORTWLAN3,
#endif
	/*physical port should be added above*/
	QOS_DESMAC,
	QOS_SRCMAC,
	QOS_PROTO,
	QOS_DESIP,
	QOS_DESMASK,
	QOS_DESPORTBEGIN,
	QOS_DESPORTEND,
	QOS_SRCIP,
	QOS_SRCMASK,
	QOS_SRCPORTBEGIN,
	QOS_SRCPORTEND,
	QOS_IPPBEGIN,
	QOS_IPPEND,
	QOS_TOS,
	QOS_DSCPBEGIN,
	QOS_DSCPEND,
	/*IPv4 parameter should be added above*/
	QOS_VLANIDBEGIN,
	QOS_VLANIDEND,
	QOS_VLANPRIOBEGIN,
	QOS_VLANPRIOEND,
	/*802.1Q parameter shoud be added above*/
	QOS_ACT_IPP,
	QOS_ACT_TOS,
	QOS_ACT_DSCP,
	QOS_ACT_VLANPRIO,
	QOS_ACT_QUEUE,
	/*remarking parameter should be added above*/
	QOS_ATTRLEN
};

int qos_check[QOS_ATTRLEN];  /*Rodney_20100222*/
char qos_attr_val[QOS_ATTRLEN][32];  /*Rodney_20100222*/

#endif
#endif

#if defined(TCSUPPORT_CT_VOIP_QOS)
#define RTP_PRIORITY_QUEUE_PATH		"/proc/tc3162/RtpPriority"
#endif


#if defined(TCSUPPORT_CT_QOS)
char appmode[64] = {0};
int is_qos_boot = 0;
#endif
#if 0  /*Orginal ci-command mode */
const char
firewall_attribute[][ATTR_SIZE]=
{
	{"firewall_status"},
	{"spi_status"},
	{""},
};

const char
dmz_attribute[][ATTR_SIZE]=
{
	{"DMZ_ACTIVE"},
	{"DMZ_IP"},
	{""},
};


const char
adsl_attribute[][ATTR_SIZE]=
{
	{"MODULATIONTYPE"},
	{"ANNEXTYPEA"},
	{""},
};


const char
virServ_attribute[][ATTR_SIZE]=
{
	{"STARTPORT"},
	{"ENDPORT"},
	{"LOCALIP"},
	{""},
};

const char
ipmap_attribute[][ATTR_SIZE]=
{
	{"RULETYPE"},
	{"LOCALSTARTIP"},
	{"LOCALENDIP"},
	{"PUBLICSTARTIP"},
	{"PUBLICENDIP"},
	{""},
};

enum ipmap_attr_en{
	RuleType=0,
	LocalSIP,
	LocalEIP,
	PubSIP,
	PubEIP
};

const char
adsl_mode[][2][ATTR_SIZE]=
{
	{"Auto Sync-Up", "multimode"},
	{"ADSL2+", "adsl2plus"},
	{"ADSL2", "adsl2"},
	{"G.DMT", "gdmt"},
	{"T1.413", "t1.413"},
	{"G.lite", "glite"},
	{"",""},
};

const char
adsl_type[][2][ATTR_SIZE]=
{
	{"ANNEX A","a"},
	{"ANNEX I","i"},
	{"ANNEX A/L","al"},
	{"ANNEX M","m"},
	{"ANNEX A/I/J/L/M","aijlm"},
	{"",""},
};
#endif

void
updateVirServ(int pvc);

#if 0  /*Orginal ci-command mode */
/*______________________________________________________________________________
**	firewall_read
**
**	descriptions:
**		Read firewall system parameter from configuration file and record those information
**		to mxml_node_t.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
firewall_read(mxml_node_t *top){
	int retval=0;
	char *pAttr= NULL;

	pAttr=(char *)firewall_attribute;
	retval=node_read(top, FIREWALL, FIREWALL_PATH, pAttr, QMARKS);
	return retval;
}/* end firewall_read */
#endif

#ifndef PURE_BRIDGE
/*______________________________________________________________________________
**	firewall_init
**
**	descriptions:
**		the init function of firewall.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
firewall_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="Firewall",
			.type=SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
#if defined(TCSUPPORT_CT_E8GUI)
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=firewall_execute,
			.cfg_boot=firewall_boot,
#else
			.cfg_write=firewall_write,
			.cfg_verify=firewall_verify,
			.cfg_execute=firewall_execute,
			.cfg_boot=firewall_boot,
#endif
	};
	ret=register_cfg_node(&node);
	return ret;
}
#if defined(TCSUPPORT_CT_E8GUI)
int
firewall_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	unsigned int new_filter_state=0;
	char firewall_status[2]={0}, nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
#if defined(TCSUPPORT_CT_NEWGUI)
	char DosAttackProtect_status[10]={0};	
#endif
	char firewall_level[10] = {0};
	char firewallcmd[40] = {0};
	
	memset(nodeName,0,sizeof(nodeName));

	strcpy(nodeName[0],FIREWALL);
	strcpy(nodeName[1],SUB_NODE_NAME);
	
	if(getAttrValue(top, nodeName, "firewall_level", firewall_level) != TCAPI_PROCESS_OK)
		strcpy(firewall_level, "medium");
	/*get now information*/
	if((getAttrValue(top, nodeName, "firewall_status", firewall_status) != TCAPI_PROCESS_OK)){
		return FAIL;
	}
#if defined(TCSUPPORT_CT_NEWGUI)
	if((getAttrValue(top, nodeName, "DosAttackProtect_status", DosAttackProtect_status) != TCAPI_PROCESS_OK)){
		return FAIL;
	}
#endif
	if(!strcmp(firewall_status,"1")){/*firewall active*/
		system(FIREWALL_OFF);
		sprintf(firewallcmd, FIREWALL_LEVELCMD, firewall_level);
		system(FIREWALL_ON);
		system(firewallcmd);
#if defined(TCSUPPORT_CT_NEWGUI)	
		system(FIREWALL_OFF);
		sprintf(firewallcmd, FIREWALL_DoSProtectCMD, DosAttackProtect_status);
		system(FIREWALL_ON);
		system(firewallcmd);
#endif
	}else{	
		system(FIREWALL_OFF);
	}
	return SUCCESS;
}/* end firewall_execute*/
int
firewall_boot(mxml_node_t *top){	
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],FIREWALL);
	strcpy(nodeName[1],SUB_NODE_NAME);
	firewall_execute(top,nodeName);
	return SUCCESS;
}
#else

//"Firewall",	SINGLE_TYPE,	NULL, NULL,NULL, firewall_write, firewall_verify, firewall_execute},
/*______________________________________________________________________________
**	firewall_boot
**
**	descriptions:
**		the boot function of firewall.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
firewall_boot(mxml_node_t *top){
	unsigned int new_filter_state=0;
	FILE *startupSh=NULL;
	char startupStatus[4]={0};
//	setNewFilterState(top,firewall_check_filter,FIREWALL_VECTOR,&new_filter_state);
	if(firewall_check_filter(top)){//filter on
		new_filter_state=pre_sys_state.filter_m_state|FIREWALL_VECTOR;
	}
	else{//filter down
		new_filter_state=pre_sys_state.filter_m_state&(~FIREWALL_VECTOR);
	}
	pre_sys_state.filter_m_state = check_and_set_filter(new_filter_state);
	startupSh=fopen("/etc/firewall.conf","r");
	if(startupSh){
		get_profile_str("firewall_status=", startupStatus, sizeof(startupStatus), QMARKS, "/etc/firewall.conf");
		if(!strcmp(startupStatus,"1")){
			firewall_scan_port_and_add_rule();
			system("/usr/script/fw_start.sh");
		}
		get_profile_str("spi_status=", startupStatus, sizeof(startupStatus), QMARKS, "/etc/firewall.conf");
		if(!strcmp(startupStatus,"1")){
			system("/usr/script/spi_fw_start.sh");
		}
		fclose(startupSh);
	}
	return SUCCESS;
}
/*______________________________________________________________________________
**	firewall_write
**
**	descriptions:
**		Find firewall element from mxml tree then write firewall's system parameter to specify
**		configuration files.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  firewall cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
firewall_write(mxml_node_t *top, mxml_node_t *parant){
	int retval=0/*, fd*/;
	mxml_node_t *node = NULL;
//	char cmd_buf[128];
	/*
	node = mxmlFindElement(tree, tree, FIREWALL_ENTRY,
		NULL, NULL,
		MXML_DESCEND);
	*/
	node = mxmlFindElement(parant, parant, SUB_NODE_NAME,
		NULL, NULL, MXML_DESCEND);
#if 0
  /*template the information */
	fd = open(FIREWALL_PATH,O_RDONLY);
	if(fd != -1){
    close(fd);
    sprintf(cmd_buf,"mv %s %s\n",FIREWALL_PATH,FIREWALL_TMP_PATH);
    system(cmd_buf);
  }
#endif
	retval=node_write(node, FIREWALL_PATH, QMARKS);
	return retval;
}/* end firewall_write */

/*______________________________________________________________________________
**	firewall_verify
**
**	descriptions:
**		To check the system parameter of firewall element  is validly or not.
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
firewall_verify(mxml_node_t *node){
	return SUCCESS;
}/* end firewall_verify */


/*_____________________________________________________________________________
**      function name: firewall_execute
**      descriptions:
**            To execute firewall node service.
**
**      parameters:
**            node:   Put the firewall of cfg node tree that is used to execute firewall settings.
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
firewall_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	unsigned int new_filter_state=0;
	char firewall_status[2]={0}, spi_status[2]={0}, nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
//	char firewall_status_pre[5]={0}, spi_status_pre[5]={0};

	
	memset(nodeName,0,sizeof(nodeName));

	strcpy(nodeName[0],FIREWALL);
	strcpy(nodeName[1],SUB_NODE_NAME);

	/*get now information*/
	if((getAttrValue(top, nodeName, "firewall_status", firewall_status) != TCAPI_PROCESS_OK)
	||(getAttrValue(top, nodeName, "spi_status", spi_status) != TCAPI_PROCESS_OK)){
		return FAIL;
	}
#if 0
	/*get previrous information*/
	get_profile_str("firewall_status=", firewall_status_pre, sizeof(firewall_status_pre), QMARKS, FIREWALL_TMP_PATH);
	get_profile_str("spi_status=", spi_status_pre, sizeof(spi_status_pre), QMARKS, FIREWALL_TMP_PATH);
	unlink(FIREWALL_TMP_PATH);
#endif

//	setNewFilterState(top,firewall_check_filter,FIREWALL_VECTOR,&new_filter_state);
	//     		pre_sys_state.filter_m_state = check_and_set_filter(top,pre_sys_state.filter_m_state);

	if(firewall_check_filter(top)){//filter on
		new_filter_state=pre_sys_state.filter_m_state|FIREWALL_VECTOR;
	}
	else{//filter down
		new_filter_state=pre_sys_state.filter_m_state&(~FIREWALL_VECTOR);
	}

	if(!strcmp(firewall_status,"1")){/*firewall active*/

		pre_sys_state.filter_m_state = check_and_set_filter(new_filter_state);
		system("/usr/script/fw_stop.sh");
		firewall_scan_port_and_add_rule();
		system("/usr/script/fw_start.sh");
	}else{	
		system("/usr/script/fw_stop.sh");
		pre_sys_state.filter_m_state = check_and_set_filter(new_filter_state);
	}

	if(!strcmp(spi_status,"1")){/*spi active*/
		pre_sys_state.filter_m_state = check_and_set_filter(new_filter_state);
		system("/usr/script/spi_fw_stop.sh");
		system("/usr/script/spi_fw_start.sh");
	}else{
		system("/usr/script/spi_fw_stop.sh");
		pre_sys_state.filter_m_state = check_and_set_filter(new_filter_state);
	}
#if 0
	/*if change firewall active state*/
	if(strcmp(firewall_status,firewall_status_pre)){
		if(!strcmp(firewall_status,"1")){/*firewall active*/
			system("/usr/script/fw_start.sh");
		}else{
			system("/usr/script/fw_stop.sh");
		}
	}
	/*if change spi active state*/
	/* krammer 20081027 changes
	compare firewall_status with spi_status_pre will have wrong behavior,
	so i change firewall_status with spi_status.
	if(strcmp(firewall_status,spi_status_pre)){
	*/
	if(strcmp(spi_status,spi_status_pre)){
		if(!strcmp(spi_status,"1")){/*spi active*/
			pre_sys_state.filter_m_state = check_and_set_filter(new_filter_state);
			system("/usr/script/spi_fw_start.sh");
		}else{
			system("/usr/script/spi_fw_stop.sh");
			pre_sys_state.filter_m_state = check_and_set_filter(new_filter_state);
		}
	}

//	krammer move up : load the module before run script
	setNewFilterState(top,firewall_check_filter,FIREWALL_VECTOR,&new_filter_state);
	//     		pre_sys_state.filter_m_state = check_and_set_filter(top,pre_sys_state.filter_m_state);
	pre_sys_state.filter_m_state = check_and_set_filter(new_filter_state);
#endif
	return SUCCESS;
}/* end firewall_execute*/
#endif

//"Route",		ENTRY_TYPE|UPDATE_TYPE|MAX_ROUTE_NUM,	NULL, route_merge_table,route_read, route_write,route_verify, route_execute},
char *
route_merge_table[MAX_NODE_NAME]=
{
	"route_id","",
};
/*______________________________________________________________________________
**	route_init
**
**	descriptions:
**		the init function of route.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
route_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="Route",
			.type=ENTRY_TYPE|UPDATE_TYPE|MAX_ROUTE_NUM,
			.adv_table=NULL,
			.merge_table=route_merge_table,
			.cfg_read=route_read,
			.cfg_write=route_write,
			.cfg_verify=route_verify,
			.cfg_execute=route_execute,
#if defined(TCSUPPORT_CT_DSL_EX)
			.cfg_boot=route_boot,
#else
			.cfg_boot=NULL,
#endif
			.cfg_pre_unset=route_pre_unset,
	};
	ret=register_cfg_node(&node);
	return ret;
}

#if defined(TCSUPPORT_CT_DSL_EX)
int
route_boot(mxml_node_t *top){
	copy_to_route_node();
	return SUCCESS;
}

#endif

/*______________________________________________________________________________
**function name:route_read
**
**description:
*    read all system routing entry.
**parameters:
*    top:Specify the root of mxml_node_t structure.
**global:
*    none
**return:
*    0:success
*    -1:fail
**call:
*    none
**revision:
*     1.shnwind 20080526
**____________________________________________________________________________*/
int
route_read(mxml_node_t *top, char nodeName[][MAX_NODE_NAME], char *attr){

	char routeNodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	mxml_node_t *node = NULL;   /* <element node> */
	mxml_node_t *group = NULL;  /* <Route Node> */
	int static_route_index = 0;//	dynamic_route_index = 0;
	//char buf[MAXSIZE] = {0}, tmp[MAXSIZE] = {0},addnum[4] = {0};
	char addnum[4] = {0};
	char count[4] = {0};
	//char desip[16] = {0};
	int i = 0;
	//FILE *fp = NULL;
	//int route_number = 0;
	int norecord = 0;

	//tcdbg_printf("\r\nroute_read");

	if(strcmp(attr,"Route_num") && strcmp(attr,"add_num"))
	{
			return SUCCESS;
	}	

	group = mxmlFindElement(top, top, ROUTE,
			NULL, NULL, MXML_DESCEND);
	if(group == NULL){
		tcdbg_printf("\r\nreturn fail");
		return FAIL;
	}

	memset(routeNodeName, 0, sizeof(routeNodeName));
	strcpy(routeNodeName[0],ROUTE);
	/*the circulation do three things*/
	for(i = 0; i < MAX_STATIC_ROUTE_NUM; i++)
	{
		sprintf(routeNodeName[1],"%s%d",SUB_NODE_NAME, i);
		node = mxmlFindElement(group, group, routeNodeName[1],
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
			#if 0
			/*2.change the device parameter value*/
			p = mxmlElementGetAttr(node, "Device");
			if(p)
			{
				strcpy(dev,p);
				/*change device name*/

				sprintf(tmpbuf,"PVC%d",atoi(p+strlen(dev)-1));
				memset(dev,0,sizeof(dev));
				if(change_device_name(top, tmpbuf, dev) == 0)
				{
					mxmlElementSetAttr(node,"Device",dev);
				}
				else
				{
					mxmlElementSetAttr(node,"Device",tmpbuf);
				}
			}
			#endif
			/*3.compute the static route number*/
			static_route_index++;
		}	
	}

	
	/*set route number now*/
	//sprintf(count,"%d",dynamic_route_index+static_route_index-MAX_STATIC_ROUTE_NUM); //remove dynamic route
	sprintf(count,"%d",static_route_index);
	mxmlElementSetAttr(group, "Route_num", count);
	//sprintf(count,"%d",static_route_index);
	mxmlElementSetAttr(group, "User_def_num", count);
	//fclose(fp);
	//unlink(ROUTE_TMP_PATH);

	return SUCCESS;
}/* end route_read */

/*______________________________________________________________________________
**	route_write
**
**	descriptions:
**		Find route element from mxml tree then write route's system parameter to specify
**		configuration files.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  firewall cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
route_write(mxml_node_t *top, mxml_node_t *parant){
	#if 0
	FILE *fp = NULL;
	int       i;          /* Looping var */
	mxml_attr_t   *attr= NULL;          /* Cirrent attribute */
	mxml_node_t *node = NULL;
	char route_info[5][16];
	char route_entry[MAXSIZE];
	char element_name[32];
	char if_type[8]={0};/* Device or Gateway*/
	int route_count=0;
	char id[3];

	fp = fopen(ROUTE_PATH,"w");
	if(fp == NULL){
		return FAIL;
	}

	 /*sprintf(element_name, ROUTE_ENTRY, route_count);*/
	 sprintf(element_name,"%s%d", SUB_NODE_NAME, route_count);
	 while((node = mxmlFindElement(parant, parant, element_name,
	  	NULL , NULL,MXML_DESCEND)) != NULL){
			/*
				<Route_0_Entry  DST_IP="100.100.100.0" Sub_mask="255.255.255.0" Device="nas0" metric="1" />
				<Route_0_Entry  DST_IP="100.100.100.0" Sub_mask="255.255.255.0" Gateway="192.168.1.1" metric="1" />
			*/

			memset(route_info, 0, sizeof(route_info));
			attr = node->value.element.attrs;
			for (i = 0; i < node->value.element.num_attrs; i++){
				if(i == ROUTE_IF_TYPE_INDEX){
					sprintf(if_type, "%s", attr->name);
				}
				strcpy(route_info[i], attr->value);
				attr ++;
			}
			/*route.sh : /sbin/route add -net 100.100.100.0 netmask 255.255.255.0 dev nas0 metric 1
						 or /sbin/route add -net 20.20.20.0 netmask 255.255.255.0 gw 192.168.1.1
			*/
			if(strlen(route_info[ROUTE_METRIC_INDEX]) == 0){
				if(strcmp(if_type, ROUTE_ATTR_DEVICE) == 0){/*modify to create correct route.sh.shnwind 2008.4.17*/
					sprintf(route_entry, ROUTE_CMD_NO_METRIC_DEV,
					route_info[ROUTE_IP_INDEX] ,route_info[ROUTE_MASK_INDEX] ,route_info[ROUTE_GW_INDEX]);
				}
				else{
					sprintf(route_entry, ROUTE_CMD_NO_METRIC_GW,
					route_info[ROUTE_IP_INDEX] ,route_info[ROUTE_MASK_INDEX] ,route_info[ROUTE_GW_INDEX]);
				}
			}else{
				if(strcmp(if_type, ROUTE_ATTR_DEVICE) == 0){
					sprintf(route_entry, ROUTE_CMD_WITH_METRIC_DEV,
					route_info[ROUTE_IP_INDEX] ,route_info[ROUTE_MASK_INDEX] ,
					route_info[ROUTE_GW_INDEX], route_info[ROUTE_METRIC_INDEX]);
				}
				else{
					sprintf(route_entry, ROUTE_CMD_WITH_METRIC_GW,
					route_info[ROUTE_IP_INDEX] ,route_info[ROUTE_MASK_INDEX] ,
					route_info[ROUTE_GW_INDEX], route_info[ROUTE_METRIC_INDEX]);
				}
			}
			fputs(route_entry, fp);
			route_count++;
			/*sprintf(element_name, ROUTE_ENTRY, route_count);*/
			 sprintf(element_name,"%s%d", SUB_NODE_NAME, route_count);
	}
	fclose(fp);
	chmod(ROUTE_PATH,755);/*script must can execute*/
	/*Create route.count file*/
	sprintf(id, "%d", route_count);
	if(write2file(id,ROUTE_COUNT_PATH) == FAIL){
		return FAIL;
	}
	#endif
	FILE *fp;
	/*krammer move create_route_exe_sh up for update_route_sh*/
	fp = fopen(ROUTE_EXE_SH,"r");
	if(fp == NULL){
 		 create_route_exe_sh(top, ROUTE_ADD);
	}else{
		fclose(fp);
	}

	#if 0
	/*create ROUTE_SH for record user define entry*/
  create_route_sh(top, parant);
	/*create ROUTE_EXE_SH for execution*/
	/*if ROUTE_EXE_SH is ecist means user do unset, cfg_manager do not need do this function.
	    shnwind 2008.9.25*/
	#endif

	fp=fopen(ROUTE_PATH,"r");
	if(fp == NULL){
		create_route_sh(top, parant);/*only for boot time*/
	}
	else{
		update_route_sh(top, parant);/*after boot time*/
		fclose(fp);
	}
	#if 0
	fp = fopen(ROUTE_EXE_SH,"r");
	if(fp == NULL){
  create_route_exe_sh(top, ROUTE_ADD);
	}else{
		fclose(fp);
	}
	#endif

	return SUCCESS;
}/* end route_write */

/*______________________________________________________________________________
**	route_verify
**
**	descriptions:
**		To check the system parameter of route element  is validly or not.
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
route_verify(mxml_node_t *node){
	return SUCCESS;
}/* end route_verify */

/*_____________________________________________________________________________
**      function name: route_execute
**      descriptions:
**            To execute route node service.
**
**      parameters:
**            node:   Put the route of cfg node tree that is used to execute route settings.
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
route_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
  FILE *fp;
  FILE *fp_r = NULL;
  char line[MAXSIZE]={0};
  char wanNode[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
  int ii = 0, pvc_index = 0, entry_index = 0;
  char def_route_index_v4[10] = {0};
  int i_def_route_index_v4 = -1;
  char str_servicelist[64] = {0}, iface_name[16] = {0};
  int store_add_idx[MAX_WAN_IF_INDEX] = {0};

  /* get default route for ipv4 */
  bzero(def_route_index_v4, sizeof(def_route_index_v4));
  bzero(wanNode, sizeof(wanNode));
  strcpy(wanNode[0], "WanInfo");
  strcpy(wanNode[1], "Common");
  if ( TCAPI_PROCESS_OK == getAttrValue(top, wanNode, "DefRouteIndexv4"
  		, def_route_index_v4)
	  && '\0' != def_route_index_v4[0] && 0 != strcmp(def_route_index_v4, "N/A") )
	  i_def_route_index_v4 = atoi(def_route_index_v4);

  /* Add Link Route for backup INTERNET connection, 
  otherwise static route will be added failed. */
  bzero(store_add_idx, sizeof(store_add_idx));
  for ( ii = 0; ii < MAX_WAN_IF_INDEX; ii++ )
  {
      if ( i_def_route_index_v4 == ii )
	  	continue;

	  pvc_index = ii / MAX_SMUX_NUM;
	  entry_index = ii - pvc_index * MAX_SMUX_NUM;
	  bzero(wanNode, sizeof(wanNode));
	  strcpy(wanNode[0], "Wan");
	  sprintf(wanNode[1], "PVC%d", pvc_index);
	  sprintf(wanNode[2], "Entry%d", entry_index);
	  if( TCAPI_PROCESS_OK != getAttrValue(top, wanNode, "ServiceList"
	  	, str_servicelist) )
		continue;

	  if ( strstr(str_servicelist, INTERNET) )
	  {
		  if ( TCAPI_PROCESS_OK != getAttrValue(top, wanNode, "IFName"
		  	, iface_name) )
		  	continue;

		  dealWithLinkLocalRoute(top, iface_name, ii, 2);
		  store_add_idx[ii] = 1;
	  }
  }

  fp=fopen(ROUTE_EXE_SH,"r");
  if(fp != NULL){
  	fp_r = fopen(ROUTE_EXE_PRE_SH, "r");
	if ( fp_r )
	{
		fclose(fp_r);
		fp_r = NULL;
		system(ROUTE_EXE_PRE_SH);
		unlink(ROUTE_EXE_PRE_SH);
	}


#if 1
	while(fgets(line,MAXSIZE,fp) && strlen(line) != 0){		
		system(line);
	}
	
	fclose(fp);
#else	
    system(ROUTE_EXE_SH);
#endif
    unlink(ROUTE_EXE_SH);

	fp_r = fopen(ROUTE_EXE_AFT_SH, "r");
	if ( fp_r )
	{
	  fclose(fp_r);
	  fp_r = NULL;
	  system(ROUTE_EXE_AFT_SH);
	  unlink(ROUTE_EXE_AFT_SH);
	}
  }

  /* Remove temp Link Route. */
	for ( ii = 0; ii < MAX_WAN_IF_INDEX; ii++ )
	{
	  	if ( 1 != store_add_idx[ii] )
	  		continue;

		pvc_index = ii / MAX_SMUX_NUM;
		entry_index = ii - pvc_index * MAX_SMUX_NUM;
		bzero(wanNode, sizeof(wanNode));
		strcpy(wanNode[0], "Wan");
		sprintf(wanNode[1], "PVC%d", pvc_index);
		sprintf(wanNode[2], "Entry%d", entry_index);

		if ( TCAPI_PROCESS_OK != getAttrValue(top, wanNode, "IFName"
					, iface_name) )
					continue;
		dealWithLinkLocalRoute(top, iface_name, ii, 0);
	}

	return SUCCESS;
}/* end route_execute*/
/*_____________________________________________________________________________
**      function name: route_pre_unset
**      descriptions:
**            To delete static route before deleting route6 node.
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
**      1. jlliu 2010/8/31
**____________________________________________________________________________
*/
int 
route_pre_unset(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	create_route_exe_sh(top, ROUTE_DEL);
	return SUCCESS;	
}

#ifdef IPV6
//#define ROUTE_DEBUG 1

/*_____________________________________________________________________________
**      function name: route6_cmd
**      descriptions:
**            execute "route -A inet6 add/del" cmd
**
**      parameters:
**           action: ROUTE6_ADD or ROUTE6_DEL
**		route6_para: 	route6_para[0]-des ip
**					route6_para[1]-prefix len
**					route6_para[2]-gateway
**					route6_para[3]-device name
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
int route6_cmd(int action, char route6_para[][MAX_ROUTE6_PARA_LEN])
{
	char cmd[130] = {0}, tmp[45] = {0};

	switch(action){
		case ROUTE6_ADD:
			sprintf(cmd, "/sbin/route -A inet6 add ");
			break;
		case ROUTE6_DEL:
			sprintf(cmd, "/sbin/route -A inet6 del ");
			break;
		default:
			return FAIL;
	}
	
	#ifdef ROUTE_DEBUG
	tcdbg_printf("\n %s:dst_ip:%s,prefix_len:%s,gateway:%s, device:%s!\n", __func__, route6_para[DST_IP_TYPE], route6_para[PREFIX_LEN_TYPE], route6_para[GATEWAY_TYPE], route6_para[DEVICE_TYPE]);
	#endif

	if(strlen(route6_para[DST_IP_TYPE]) == 0 || strlen(route6_para[PREFIX_LEN_TYPE]) == 0){
		tcdbg_printf("\n %s:no des ip or prefix length\n", __func__);
		return FAIL;
	}
	sprintf(tmp, "%s/%s ", route6_para[DST_IP_TYPE], route6_para[PREFIX_LEN_TYPE]);
	strcat(cmd, tmp);

	if(strlen(route6_para[GATEWAY_TYPE]) == 0 && strlen(route6_para[DEVICE_TYPE]) == 0){
		tcdbg_printf("\n %s:no gateway or device\n", __func__);
		return FAIL;
	}

	/*Add gateway*/
	if(strcmp(route6_para[GATEWAY_TYPE], "*") && strcmp(route6_para[GATEWAY_TYPE], "") && strcmp(route6_para[GATEWAY_TYPE], "::")){
			sprintf(tmp, "gw %s ", route6_para[GATEWAY_TYPE]);
			strcat(cmd, tmp);
	}

	/*Add dev*/
	if(strcmp(route6_para[DEVICE_TYPE], "*") && strcmp(route6_para[DEVICE_TYPE], "")){
			sprintf(tmp, "dev %s ", route6_para[DEVICE_TYPE]);
			strcat(cmd, tmp);
	}
	#ifdef ROUTE_DEBUG
	tcdbg_printf("\n %s:cmd:%s!\n", __func__, cmd);
	#endif
	system(cmd);	

	return SUCCESS;
	
}

/*_____________________________________________________________________________
**      function name: create_route6_backup_file
**      descriptions:
**            create ipv6 static route backup file.
**
**      parameters:
**           None
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
create_route6_backup_file()
{
	mxml_node_t *curNode = NULL;
	FILE *fp;
	const char *Value=NULL;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char cmd[128] = {0}, tmp[60] = {0};
	int i;
	
	/*Back up file format:
	dest_ip	prefix_len gateway device
	2002:: 64 2001::2 nas2_1
	#						//means this entry is empty
	2002:: 64 * nas2_1		//means gateway is empty
	2002:: 64 2001::2 *		//means device is empty
	#
	#
	#
	*/

	fp = fopen(ROUTE6_BACK_UP_PATH, "w");
	if(fp == NULL){
		tcdbg_printf("\n %s:open %s failed!\n", __func__, ROUTE6_BACK_UP_PATH);
		return FAIL;
	}
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],ROUTE6);
	for(i = 0; i < MAX_STATIC_ROUTE6_NUM; i++){
		sprintf(nodeName[1], "Entry%d", i);
		memset(cmd, 0,sizeof(cmd));
		if((curNode = search_node(nodeName)) == NULL){
			strcpy(cmd, "#");	
		}
		else{
			/*write destnation IP*/
			Value = mxmlElementGetAttr(curNode, "DST_IP");
			if(Value){
				sprintf(tmp,"%s ",Value);
				strcat(cmd, tmp);
			}
			/*write prefix length*/
			Value = mxmlElementGetAttr(curNode, "Prefix_len");
			if(Value){
				sprintf(tmp,"%s ",Value);
				strcat(cmd, tmp);
			}
			/*write Gateway information*/
			Value = mxmlElementGetAttr(curNode, "Gateway");
			if(Value ){
				sprintf(tmp,"%s ",Value);
				strcat(cmd, tmp);
			}
			else{				
				strcat(cmd, "* ");
			}
			/*write device information*/
			Value = mxmlElementGetAttr(curNode, "Device");
			if(Value){
				sprintf(tmp,"%s ",Value);
				strcat(cmd, tmp);
			}
			else{				
				strcat(cmd, "* ");
			}

		}
		strcat(cmd, "\n");
		fputs(cmd, fp);
	}
	fclose(fp);

	return SUCCESS;
}
//"Route",		ENTRY_TYPE|UPDATE_TYPE|MAX_ROUTE_NUM,	NULL, route_merge_table,route_read, route_write,route_verify, route_execute},
char *
route6_merge_table[MAX_NODE_NAME]=
{
	"route6_id","",
};
/*______________________________________________________________________________
**	route6_init
**
**	descriptions:
**		the init function of route.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
route6_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="Route6",
			.type=ENTRY_TYPE|UPDATE_TYPE|MAX_STATIC_ROUTE6_NUM,
			.adv_table=NULL,
			.merge_table=route6_merge_table,
			.cfg_read=route6_read,
			.cfg_write=route6_write,
			.cfg_verify=route6_verify,
			.cfg_execute=route6_execute,
			.cfg_boot=route6_boot,
			.cfg_pre_unset=route6_pre_unset,
	};
	ret=register_cfg_node(&node);
	return ret;
}

/*______________________________________________________________________________
**	route6_boot
**
**	descriptions:
**		the boot function of route6.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
route6_boot(mxml_node_t *top){
#if defined(TCSUPPORT_CT_DSL_EX)
	copy_to_route6_node();
#endif
	create_route6_backup_file();
	return SUCCESS;
}

/*______________________________________________________________________________
**function name:route6_read
**
**description:
*    read all system routing entry.
**parameters:
*    top:Specify the root of mxml_node_t structure.
**global:
*    none
**return:
*    0:success
*    -1:fail
**call:
*    none
**revision:
*     
**____________________________________________________________________________*/
int
route6_read(mxml_node_t *top, char nodeName[][MAX_NODE_NAME], char *attr){
	char routeNodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	mxml_node_t *node = NULL;   /* <element node> */
	mxml_node_t *group = NULL;  /* <Route Node> */
	int static_route_index = 0;
	char addnum[4] = {0};
	char count[4] = {0};	
	int i = 0;	
	int norecord = 0;
	
	
	
	/* do this function only the attribute is "Route_num" or "add_num".*/
	if(strcmp(attr,"Route_num") && strcmp(attr,"add_num"))
	{
		return SUCCESS;
	}	

	group = mxmlFindElement(top, top, ROUTE6,
			NULL, NULL, MXML_DESCEND);
	if(group == NULL){
		tcdbg_printf("\r\nroute6_read:get Route6 node failed\n");
		return FAIL;
	}

	memset(routeNodeName, 0, sizeof(routeNodeName));
	strcpy(routeNodeName[0],ROUTE);

	for(i = 0; i < MAX_STATIC_ROUTE6_NUM; i++)
	{
		sprintf(routeNodeName[1],"%s%d",SUB_NODE_NAME, i);
		node = mxmlFindElement(group, group, routeNodeName[1],
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
			/*2.compute the static route number*/
			static_route_index++;
		}	
	}
	/*set route number now*/
	sprintf(count,"%d",static_route_index);
	mxmlElementSetAttr(group, "Route_num", count);

	return SUCCESS;
}/* end route_read */

/*______________________________________________________________________________
**	route6_write
**
**	descriptions:
**		Find route element from mxml tree then write route's system parameter to specify
**		configuration files.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  firewall cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
route6_write(mxml_node_t *top, mxml_node_t *parant){
	return SUCCESS;
}/* end route_write */

/*______________________________________________________________________________
**	route6_verify
**
**	descriptions:
**		To check the system parameter of route element  is validly or not.
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
route6_verify(mxml_node_t *node){
	return SUCCESS;
}/* end route_verify */

/*_____________________________________________________________________________
**      function name: route6_execute
**      descriptions:
**            To execute route node service.
**
**      parameters:
**            node:   Put the route of cfg node tree that is used to execute route settings.
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
route6_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	int route_index = 0, wan_device_index = -1, i = 0;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char str_route_index[5] = {0};	
	char route6_para[4][MAX_ROUTE6_PARA_LEN];
	char wan_active[5] = {0}, cmd[128] = {0};
	mxml_node_t *curNode = NULL;
	FILE *fp;
		
	if(get_entry_number(name[1],"Entry",&route_index) != SUCCESS ){
		/*get route index from webcurset if resolve Entry failed*/
		memset(nodeName,0,sizeof(nodeName));
		strcpy(nodeName[0],WEBCURSET);
		strcpy(nodeName[1],SUB_NODE_NAME);
		/*get route id*/
		if(getAttrValue(top, nodeName, "route6_id", str_route_index) == TCAPI_PROCESS_OK){
			route_index = atoi(str_route_index);
		}else{
			tcdbg_printf("\n %s:get route index failed!\n", __func__);
			return FAIL;
		}
	}

	/*Step1:Delete static route*/
	fp = fopen(ROUTE6_BACK_UP_PATH, "r");
	if(fp == NULL){
		tcdbg_printf("\n %s:open %s failed!\n", __func__, ROUTE6_BACK_UP_PATH);
		return FAIL;
	}
	/*Get static route rule from back up file base on route_index*/
	for(i = 0; i <= route_index; i++){
		if(fgets(cmd, 128, fp) == NULL){
			tcdbg_printf("\n %s:can not read line %d from %s !\n", __func__, route_index, ROUTE6_BACK_UP_PATH);
			fclose(fp);
			return FAIL;
		}
	}
	fclose(fp);
	memset(route6_para, 0, sizeof(route6_para));
	sscanf(cmd, "%s %s %s %s", route6_para[DST_IP_TYPE], route6_para[PREFIX_LEN_TYPE], route6_para[GATEWAY_TYPE], route6_para[DEVICE_TYPE]);
	#ifdef ROUTE_DEBUG
	tcdbg_printf("\n %s:S1:dst_ip:%s,prefix_len:%s,gateway:%s, device:%s!\n", __func__, route6_para[DST_IP_TYPE], route6_para[PREFIX_LEN_TYPE], route6_para[GATEWAY_TYPE], route6_para[DEVICE_TYPE]);
	#endif
	if(strcmp(route6_para[DST_IP_TYPE], "#")){	//"#" means this entry is emtry befor commiting
		route6_cmd(ROUTE6_DEL, route6_para);
	}

	/*Step2:Add new static route*/
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],ROUTE6);
	sprintf(nodeName[1], "Entry%d", route_index);

		/*Check entry is existed or not*/
	if((curNode = search_node(nodeName)) != NULL){
		memset(route6_para, 0, sizeof(route6_para));
		if(getAttrValue(top, nodeName, "DST_IP", route6_para[DST_IP_TYPE]) != TCAPI_PROCESS_OK ||
			getAttrValue(top, nodeName, "Prefix_len", route6_para[PREFIX_LEN_TYPE]) != TCAPI_PROCESS_OK){
			tcdbg_printf("\n %s:get attribute failed,dst_ip,prefix_len!\n", __func__);
			return FAIL;
		}
		getAttrValue(top, nodeName, "Gateway", route6_para[GATEWAY_TYPE]);

			/*Check device status*/	
		if(getAttrValue(top, nodeName, "Device", route6_para[DEVICE_TYPE]) == TCAPI_PROCESS_OK){
			if((wan_device_index = get_wanindex_by_name(route6_para[DEVICE_TYPE])) < 0){
				tcdbg_printf("\n %s:get wan index from %s failed!\n", __func__, route6_para[DEVICE_TYPE]);
				return FAIL;
			}
			if(get_waninfo_by_index(wan_device_index, "Active", wan_active) == SUCCESS){
				if(!strcmp(wan_active, DEACTIVE)){
					tcdbg_printf("\n %s:%s is not active,add static route failed!\n", __func__, route6_para[DEVICE_TYPE]);
					//mxmlDelete(curNode);
					return FAIL;			
				}		
			}
			else{
				tcdbg_printf("\n %s:%s is not exist,add static route failed!\n", __func__, route6_para[DEVICE_TYPE]);
				mxmlDelete(curNode);
				return FAIL;	
			}
		}
		route6_cmd(ROUTE6_ADD, route6_para);	
	}
	else{
		tcdbg_printf("\n %s:%s_%s is not exit!\n", __func__, nodeName[0], nodeName[1]);
	}
	

	
	/*Step3: Backup static route rule*/
	if(create_route6_backup_file() != SUCCESS){
		tcdbg_printf("\n %s:create backup file failed!\n", __func__);
		return FAIL;
	}
		
	return SUCCESS;
}/* end route_execute*/

/*_____________________________________________________________________________
**      function name: route6_pre_unset
**      descriptions:
**            To delete static route before deleting route6 node.
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
**      1. jlliu 2010/8/31
**____________________________________________________________________________
*/
int 
route6_pre_unset(mxml_node_t *top, char name[][MAX_NODE_NAME])
{
	int route_index = 0;
	char route6_para[4][MAX_ROUTE6_PARA_LEN];
	
	if(get_entry_number(name[1],"Entry",&route_index) != SUCCESS ){
		tcdbg_printf("\n %s:get Entry index from %s_%s failed!\n", __func__, name[0], name[1]);
		return FAIL;
	}
	
	memset(route6_para, 0, sizeof(route6_para));
	if(getAttrValue(top, name, "DST_IP", route6_para[DST_IP_TYPE]) == TCAPI_PROCESS_OK &&
		getAttrValue(top, name, "Prefix_len", route6_para[PREFIX_LEN_TYPE]) == TCAPI_PROCESS_OK){
		getAttrValue(top, name, "Gateway", route6_para[GATEWAY_TYPE]);
		getAttrValue(top, name, "Device", route6_para[DEVICE_TYPE]);

		route6_cmd(ROUTE6_DEL, route6_para);
	}
	else{
		tcdbg_printf("\n %s:get attribute failed,dst_ip,prefix_len!\n", __func__);
		return FAIL;
	}

	return SUCCESS;
}
#endif

#if 0  /*Orginal ci-command mode */
/*______________________________________________________________________________
**	nat_read
**
**	descriptions:
**		Read NAT system parameter from configuration file and record those information
**		to mxml_node_t.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
nat_read(mxml_node_t *top){
	int fd=0;
	int i=0;
	char ipmdoe_file[16];
	char ipmode[16];
	char element_name[16];
	char attr_name[16];

	mxml_node_t *group;  /* <Nat> */
	mxml_node_t *node;   /* <node> */

	group = mxmlNewElement(top, NAT);

	for(i=0; i < PVC_NUM; i++){
		sprintf(ipmdoe_file, NAT_PATH, i);
		fd=open(ipmdoe_file, O_RDONLY);
		if(fd == FAIL){
			continue;
		}
		sprintf(attr_name,"%s=", NAT_IPMODE);
		get_profile_str(attr_name, ipmode, NO_QMARKS,fd);
		/*NAT_SUB_ENTRY= %s_PVC%d_Entry*/
		sprintf(element_name, NAT_SUBPVC_ENTRY, NAT,i);
		node = mxmlNewElement(group, element_name);
		mxmlElementSetAttr(node, NAT_IPMODE, ipmode);
		close(fd);
	}
	return 0;
}/* end nat_read */
#endif


//"Nat",		PVC_TYPE,	NULL, nat_merge_table,NULL, nat_write, nat_verify, NULL},
char *
nat_merge_table[MAX_NODE_NAME]=
{
	"nat_pvc","",
};
/*______________________________________________________________________________
**	nat_init
**
**	descriptions:
**		the init function of nat.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
nat_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="Nat",
			.type=PVC_TYPE,
			.adv_table=NULL,
			.merge_table=nat_merge_table,
			.cfg_read=NULL,
			.cfg_write=nat_write,
			.cfg_verify=nat_verify,
			.cfg_execute=NULL,
			.cfg_boot=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}
/*______________________________________________________________________________
**	nat_write
**
**	descriptions:
**		Find nat element from mxml tree then write nat's system parameter to specify
**		configuration files.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  nat cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
nat_write(mxml_node_t *top, mxml_node_t *parant){

	int i=0;
	char file_name[32]={0};
	char element_name[16]={0};
	mxml_node_t *node = NULL;
	for( i = 0; i < PVC_NUM; i++){
		/*sprintf(element_name, NAT_SUBPVC_ENTRY, NAT,i);*/
		sprintf(element_name, SUB_PVC_NAME,i);
		node = mxmlFindElement(parant, parant, element_name,
			NULL, NULL, MXML_DESCEND);
		sprintf(file_name, NAT_PATH, i);
		node_write(node, file_name, NO_QMARKS);
	}

	return SUCCESS;
}/* end nat_write */

/*______________________________________________________________________________
**	nat_verify
**
**	descriptions:
**		To check the system parameter of NAT element  is validly or not.
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
nat_verify(mxml_node_t *node){
	return SUCCESS;
}/* end nat_verify */

/*_____________________________________________________________________________
**      function name: nat_execute
**      descriptions:
**            To execute nat node service.
**
**      parameters:
**            node:   Put the nat of cfg node tree that is used to execute nat settings.
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
nat_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	return SUCCESS;
}/* end nat_execute*/
#if 0  /*Orginal ci-command mode */
/*______________________________________________________________________________
**	dmz_read
**
**	descriptions:
**		Read DMZ system parameter from configuration file and record those information
**		to mxml_node_t.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
dmz_read(mxml_node_t *top){
	int fd=0;
	int i=0;
	int j=0;
	char value[64];
	char tmp_attr[64];
	char dmz_file[32];
	char element_name[16];

	mxml_node_t *group= NULL;  /* <Dmz> */
	mxml_node_t *node= NULL;  /* <node> */

	group = mxmlNewElement(top, DMZ);

	for(i = 0; i < PVC_NUM; i++){
		sprintf(dmz_file,DMZ_PATH , i);
		fd = open(dmz_file, O_RDONLY);
		if(fd == FAIL){
			continue;
		}
		else{
			/*NAT_SUB_ENTRY= %s_PVC%d_Entry*/
			sprintf(element_name, NAT_SUBPVC_ENTRY, DMZ,i);
			node = mxmlNewElement(group, element_name);
			for(j = 0; strlen(dmz_attribute[j]) !=0; j++){
				sprintf(tmp_attr,"%s=", dmz_attribute[j]);
				get_profile_str(tmp_attr, value, NO_QMARKS, fd);
				mxmlElementSetAttr(node, dmz_attribute[j], value);
			}
		}
		close(fd);
	}
	return 0;
}/* end dmz_read */
#endif

/*______________________________________________________________________________
**	algswitch_check_set_filter
**
**	descriptions:
**		check if the algswitch need filter module or not.
**		then if need, insmod it.
**
**	parameters:
**		top: 		top of node tree.
**
**	return:
**		Success: 	0
**____________________________________________________________________________
*/
int
algswitch_check_set_filter(mxml_node_t *top){
	unsigned int new_filter_state=0;
	int filter_need=0;
	char l2tp_sw_status[4]={0}, ipsec_sw_status[4]={0}, pptp_sw_status[4]={0};//yzzhu
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];

	memset(nodeName,0,sizeof(nodeName));

	/*check if need filter module*/
	strcpy(nodeName[0], ALGSW_NODE_NAME);
	strcpy(nodeName[1],SUB_NODE_NAME);
	if((getAttrValue(top, nodeName, "L2TPSW", l2tp_sw_status) == TCAPI_PROCESS_OK)
	&&(getAttrValue(top, nodeName, "IPSECSW", ipsec_sw_status) == TCAPI_PROCESS_OK)
	&&(getAttrValue(top, nodeName, "PPTPSW", pptp_sw_status) == TCAPI_PROCESS_OK)){
		if(!strcmp(l2tp_sw_status,"on") || !strcmp(ipsec_sw_status,"on") || !strcmp(pptp_sw_status,"on")){
			filter_need = 1;
		}
	}
	
	/*set iptables filter*/
	if(filter_need){//filter on
		new_filter_state=pre_sys_state.filter_m_state|ALGSW_VECTOR;
	}
	else{//filter down
		new_filter_state=pre_sys_state.filter_m_state&(~ALGSW_VECTOR);
	}
	pre_sys_state.filter_m_state = check_and_set_filter(new_filter_state);

	return SUCCESS;
}

/*______________________________________________________________________________
**	set_alg_switch
**
**	descriptions:
**		get attributes value and call algcmd to enable or disable alg switch.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 	Specify the parant node of  algswitch cfg node.
**	return:
**		Success: 	0
**_______________________________________________________________________________
*/
int
set_alg_switch(mxml_node_t *top) {
	int i = 0;
	char switch_stat[4] = {0};
	char cmd_buf[32] = {0};
	char element_name[16] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], ALGSW_NODE_NAME);
	sprintf(element_name,"%s", SUB_NODE_NAME);
	strcpy(nodeName[1], element_name);

	char algsw_proto_name[][ATTR_SIZE]=
	{
		{"ftp"},
		{"sip"},
		{"h323"},
		{"rtsp"},  
		{"l2tp"},
		{"ipsec"},
		{"pptp"},
		{""},
	};
	
	char algsw_attr_parm[][ATTR_SIZE]=
	{
		{"FTPSW"},
		{"SIPSW"},
		{"H323SW"},
		{"RTSPSW"},  
		{"L2TPSW"},
		{"IPSECSW"},
		{"PPTPSW"},
		{""},
	};

	for(i=0; strlen(algsw_attr_parm[i]) != 0; i++) {
		getAttrValue(top, nodeName, algsw_attr_parm[i], switch_stat);
		if(strcmp(switch_stat, "on") == 0){
			sprintf(cmd_buf, "algcmd switch %s on", algsw_proto_name[i]);
		}
		else
			sprintf(cmd_buf, "algcmd switch %s off", algsw_proto_name[i]);

		system(cmd_buf);
		memset(switch_stat, 0, sizeof(switch_stat));
		memset(cmd_buf, 0, sizeof(cmd_buf));
	}

	return SUCCESS;
}
//"algswitch",	SINGLE_TYPE,	NULL, NULL,NULL, algswitch_write, algswitch_verify, algswitch_execute},
/*______________________________________________________________________________
**	algswitch_boot
**
**	descriptions:
**		the boot function of algswitch.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**_______________________________________________________________________________
*/
int
algswitch_boot(mxml_node_t *top){
	algswitch_check_set_filter(top);
	set_alg_switch(top);
	return SUCCESS;
}

/*______________________________________________________________________________
**	algswitch_init
**
**	descriptions:
**		the init function of algswitch.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
algswitch_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="ALGSwitch",
			.type=SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=algswitch_write,
			.cfg_verify=algswitch_verify,
			.cfg_execute=algswitch_execute,
			.cfg_boot=algswitch_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}

/*______________________________________________________________________________
**	algswitch_write
**
**	descriptions:
**		
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 	Specify the parant node of  algswitch cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
algswitch_write(mxml_node_t *top, mxml_node_t *parant){
	return SUCCESS;
}/* end algswitch_write */

/*______________________________________________________________________________
**	algswitch_verify
**
**	descriptions:
**		To check the system parameter of algswitch element  is validly or not.
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
algswitch_verify(mxml_node_t *node){
	return SUCCESS;
}/* end algswitch_verify */

/*_____________________________________________________________________________
**      function name: algswitch_execute
**      descriptions:
**            To execute algswitch node service.
**
**      parameters:
**            node:   Put the ALGSwitch of cfg node tree that is used to algswitch information.
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
**      1. yzwang 2010/3/31
**____________________________________________________________________________
*/
int
algswitch_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	algswitch_check_set_filter(top);
	set_alg_switch(top);
	return SUCCESS;
}/* end algswitch_execute */

//"Dmz",		PVC_TYPE,	NULL, nat_merge_table,NULL, dmz_write,dmz_verify, dmz_execute},
#if defined(TCSUPPORT_AUTO_DMZ) || defined(TCSUPPORT_CUC_CWMP_PARAMETER)		
char *
dmz_adv_table[MAX_NODE_NAME]=
{
	"Common","",
};
#endif
/*______________________________________________________________________________
**	dmz_boot
**
**	descriptions:
**		the boot function of dmz.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**_______________________________________________________________________________
*/
int
dmz_boot(mxml_node_t *top){
	int i;
	char name[MAX_ARG_NUM][MAX_NODE_NAME];
#if defined(TCSUPPORT_AUTO_DMZ)	
	char tmp[16] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], "Dmz");
	strcpy(nodeName[1], "Common");

	if(getAttrValue(top, nodeName, DMZ_INTERVAL, tmp) != TCAPI_PROCESS_OK){
		setAttrValue(top, nodeName, DMZ_INTERVAL, DEFAULT_DMZ_INTERVAL);
	}

	if(getAttrValue(top, nodeName, IS_SUPPORT_AUTO_DMZ, tmp) != TCAPI_PROCESS_OK){
		setAttrValue(top, nodeName, IS_SUPPORT_AUTO_DMZ, "Yes");
	}
#endif
#if defined(TCSUPPORT_CT_DSL_EX)
	copy_to_dmz_node();
#endif

	for(i = 0; i < MAX_WAN_IF_INDEX; i++){		
		memset(name, 0, sizeof(name));
		strcpy(name[0], "Dmz");
		sprintf(name[1], "Entry%d", i);
		dmz_execute(top, name);		
	}
	return SUCCESS;
}
/*______________________________________________________________________________
**	dmz_init
**
**	descriptions:
**		the init function of dmz.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
dmz_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="Dmz",
#if defined(TCSUPPORT_AUTO_DMZ) || defined(TCSUPPORT_CUC_CWMP_PARAMETER)		
			.type= ADV_FLAG  | ENTRY_TYPE | MAX_WAN_IF_INDEX,
			.adv_table=dmz_adv_table,
#else
			.type=  ENTRY_TYPE | MAX_WAN_IF_INDEX,
			.adv_table=NULL,
#endif
			.merge_table=nat_merge_table,
			.cfg_read=NULL,
			.cfg_write=dmz_write,
			.cfg_verify=dmz_verify,
			.cfg_execute=dmz_execute,
			.cfg_boot=dmz_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}
/*______________________________________________________________________________
**	dmz_write
**
**	descriptions:
**		Find DMZ element from mxml tree then write dmz's system parameter to specify
**		configuration files.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  dmz cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
dmz_write(mxml_node_t *top, mxml_node_t *parant){
	return SUCCESS;
}/* end dmz_write */

/*______________________________________________________________________________
**	dmz_verify
**
**	descriptions:
**		To check the system parameter of DMZ element  is validly or not.
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
dmz_verify(mxml_node_t *node){
	return SUCCESS;
}/* end dmz_verify */

/*_____________________________________________________________________________
**      function name: dmz_execute
**      descriptions:
**            To execute dmz nat node service.
**
**      parameters:
**            top:   Put the dmz of cfg node tree that is used to execute snmpd information.
**            name: Put the dmz cfg node name.
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
dmz_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char cur_pvc[4]={0};
	char cmd[32]={0};
	char status[8]={0}, if_name[32]={0};
	char dmz_file[32]={0};
	mxml_node_t *node = NULL;
	int pvcIndex;

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], WEBCURSET);
	strcpy(nodeName[1], SUB_NODE_NAME);

	if ( get_entry_number(name[1],"Entry", &pvcIndex) != SUCCESS ) {
		getAttrValue(top, nodeName, "nat_pvc", cur_pvc);
		sprintf(name[1], "%s%s", "Entry", cur_pvc);
	}
	if(getAttrValue(top, name, "IFName", if_name) ==  TCAPI_PROCESS_OK &&
		getAttrValue(top, name, NODE_ACTIVE, status) ==  TCAPI_PROCESS_OK){
		sprintf(dmz_file, DMZ_PATH, if_name);
		if(strcmp(status,DEACTIVE)==0){			
			unlink(dmz_file);
		}
		else{
		#if 0
			node = mxmlFindElement(top, top, name[1],
			    NULL, NULL ,MXML_DESCEND);	
		#else
			node = mxmlFindElement(top, top, name[0],
			    NULL, NULL ,MXML_DESCEND_FIRST);
			if (node) {
		#endif
				node = mxmlFindElement(node, node, name[1],
			    NULL, NULL ,MXML_DESCEND);
		
				node_write(node, dmz_file,NO_QMARKS);	
		//		sprintf(cmd, "%s %s", DMZ_SH, if_name);
				//tcdbg_printf("dmz_execute:cmd %s \n", cmd);
		//		system(cmd);
			}
			else {
				return FAIL;
			}
		}
		sprintf(cmd, "%s %s", DMZ_SH, if_name);
		system(cmd);
		
		return SUCCESS;
	}
	return FAIL;
}/* end dmz_execute*/
#if 0  /*Orginal ci-command mode */
/*______________________________________________________________________________
**	virServer_read
**
**	descriptions:
**		Read virtual Server's  system parameter from configuration file and record those information
**		to mxml_node_t.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
virServer_read(mxml_node_t *top){
	int fd=0;
	int i=0;
	int j=0;
	int k=0;

	char value[64];
	char tmp_attr[64];
	char virSer_file[32]={0};
	char element_name[32]={0};

	mxml_node_t *group= NULL;  /* <VirServer> */
	mxml_node_t *node= NULL;   /* <node> */

	group = mxmlNewElement(top, VIRSERV);

	for(i = 0; i < PVC_NUM; i++){
		/*read vserver* file */
		for(j = 0; j < MAX_VIRSERV_RULE; j++){
			sprintf(virSer_file,VIRSERV_PATH, i, j);
			fd = open(virSer_file, O_RDONLY);
			if(fd ==-1){
				continue;
			}
			else{
				/*NAT_SUBRULE_ENTRY= %s_PVC%d:%d_Entry*/
				sprintf(element_name, NAT_SUBRULE_ENTRY, VIRSERV, i, j);
				node = mxmlNewElement(group, element_name);
				for(k = 0; strlen(virServ_attribute[k]) !=0; k++){
					sprintf(tmp_attr,"%s=", virServ_attribute[k]);
					get_profile_str(tmp_attr, value, NO_QMARKS, fd);
					mxmlElementSetAttr(node, virServ_attribute[k], value);
				}
			}
			close(fd);
		}
	}
	return 0;
}/* end virServer_read */
#endif

/*______________________________________________________________________________
**	virServ_boot
**
**	descriptions:
**		the boot function of dmz.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**_______________________________________________________________________________
*/
int
virServ_boot(mxml_node_t *top){
	int i,j;
	char name[MAX_ARG_NUM][MAX_NODE_NAME];

	for(i = 0; i < MAX_WAN_IF_INDEX; i++){
		for(j = 0; j < MAX_VIRSERV_RULE; j++){
			memset(name, 0, sizeof(name));
			strcpy(name[0], "VirServer");
			sprintf(name[1], "Entry%d", i);
			sprintf(name[2], "Entry%d", j);
			virServer_execute(top, name);
		}
	}
#if defined(TCSUPPORT_CT_DSL_EX)
	copy_to_VirServer_node();
#endif

	return SUCCESS;
}

//"VirServer",	ENTRY_TYPE|PVC_TYPE|MAX_VIRSERV_RULE,	NULL, virServ_merge_table,NULL, virServer_write,virServer_verify, virServer_execute},
char *
virServ_merge_table[MAX_NODE_NAME]=
{
	"nat_pvc","virServ_id",""
};
/*______________________________________________________________________________
**	virServ_init
**
**	descriptions:
**		the init function of virServ.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
virServ_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="VirServer",
			//.type=ENTRY_TYPE|PVC_TYPE|MAX_VIRSERV_RULE,
			.type=ENTRY_TYPE|MAX_WAN_IF_INDEX,
			.adv_table=NULL,
			.merge_table=virServ_merge_table,
			.cfg_read=NULL,
			.cfg_write=NULL,
		#if 0
			.cfg_write=virServer_write,
		#endif
			.cfg_verify=virServer_verify,
			.cfg_execute=virServer_execute,
			.cfg_boot=virServ_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}
/*______________________________________________________________________________
**	virServer_write
**
**	descriptions:
**		Find virtual server element from mxml tree then write virServer's system parameter to specify
**		configuration files.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  virServer cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
virServer_write(mxml_node_t *top, mxml_node_t *parant){
	return SUCCESS;
}/* end virServer_write */

/*______________________________________________________________________________
**	virServer_verify
**
**	descriptions:
**		To check the system parameter of virtual Server element  is validly or not.
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
virServer_verify(mxml_node_t *node){
	return SUCCESS;
}/* end virServer_verify */

/*_____________________________________________________________________________
**      function name: virServer_execute
**      descriptions:
**            To  execute virServer node service.
**
**      parameters:
**            top:   Put the virServer of cfg node tree that is used to execute virtual server settings.
**            name:  Put the virServer cfg node name , VirServer_Entry0_Entry0~9
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
**      2. jlliu 2010/8/10
**____________________________________________________________________________
*/
int
virServer_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char cur_pvc[4]={0};
	char cur_id_virSer[4]={0};
	char buf[32]={0};
	mxml_node_t *curNode=top;
	int pvcIndex = 0, entryIndex = 0;
	char virServ_file[32]={0};
	char if_name[32] = {0};
	char virServ_Active[10];

	memset(nodeName, 0,sizeof(nodeName));
	strcpy(nodeName[0], WEBCURSET);
	strcpy(nodeName[1], SUB_NODE_NAME);

	/*VirServer_EntryX_EntryX*/
	if ( get_entry_number(name[1],"Entry", &pvcIndex) != SUCCESS ) {
		getAttrValue(top, nodeName, "nat_pvc", cur_pvc);
		sprintf(name[1], "%s%s", "Entry", cur_pvc);
	}
	else {
		sprintf( cur_pvc, "%d", pvcIndex );
	}

	if ( get_entry_number(name[2],SUB_NODE_NAME, &entryIndex) != SUCCESS ) {
		getAttrValue(top, nodeName, "virServ_id", cur_id_virSer);
		sprintf(name[2], "%s%s", SUB_NODE_NAME, cur_pvc);
	}
	else {
		sprintf( cur_id_virSer, "%d", entryIndex );
	}

	/*Get interface name*/
	memset(nodeName, 0,sizeof(nodeName));
	strcpy(nodeName[0], name[0]);
	strcpy(nodeName[1], name[1]);
	if(getAttrValue(top, nodeName, "IFName", if_name) !=  TCAPI_PROCESS_OK){
		//tcdbg_printf("virServer_execute:get IF name from %s_%s failed \n", nodeName[0], nodeName[1]);
		return FAIL;
	}
#ifdef WAN_DBUG
	tcdbg_printf("virServer_execute:%s %s %s \n", cur_pvc, cur_id_virSer, if_name);
#endif
#if 0
	for(i=0; i < MAX_ARG_NUM; i++){
		if(strlen(name[i])==0){
			break;
		}
		curNode = mxmlFindElement(curNode, curNode, name[i],
				NULL, NULL,MXML_DESCEND);
	}
#endif
	curNode = search_node(name);

	memset(virServ_Active,0,sizeof(virServ_Active));
	if(getAttrValue(top, name, "Active", virServ_Active) !=  TCAPI_PROCESS_OK){
		strcpy(virServ_Active, "No");
	}

	if(curNode){
		/*Delete virserver rule firstly before adding new one*/
		sprintf(buf, "%s del %s %s", VIRSERV_SH, if_name, cur_id_virSer);
		system(buf);
		/*Write virServ_file : /var/run/nas0_0/vserver0 */
		sprintf(virServ_file, VIRSERV_PATH, if_name, cur_id_virSer);
		node_write(curNode, virServ_file, NO_QMARKS);
		if (!strcmp(virServ_Active, "Yes")){
			sprintf(buf, "%s add %s %s", VIRSERV_SH, if_name, cur_id_virSer);
			system(buf);
		}
	}
	else{
		/*
			There is no sub cfg node information,
			it's mean user to delete virserver entry
		*/
		sprintf(buf, "%s del %s %s", VIRSERV_SH, if_name, cur_id_virSer);
		system(buf);
		/*Delete the configuration file*/
		sprintf(virServ_file, VIRSERV_PATH, if_name, cur_id_virSer);
		unlink(virServ_file);
	}

	return SUCCESS;
}/* end virServer_execute*/

#if 0  /*Orginal ci-command mode */
/*______________________________________________________________________________
**	ipAddrMap_read
**
**	descriptions:
**		Read IP Address Mapping  system parameter from configuration file and record those
**		information to mxml_node_t.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
ipAddrMap_read(mxml_node_t *top){

	int fd=0;
	int i=0;
	int j=0;
	int k=0;
	char file_name[64];
	char element_name[32];
	char attr_tmp[64];
	char tmp[64];

	mxml_node_t *group= NULL;
	mxml_node_t *node= NULL;

	group = mxmlNewElement(top, IPADDRMAP);

	for(i = 0; i < PVC_NUM; i++){
		/*read ipaddr_mapping0~ipaddr_mapping15 file */
		for(j = 0; j < MAX_IPMAP_RULE; j++){
			sprintf(file_name, IPADDRMAP_PATH, i, j);
			fd = open(file_name, O_RDONLY);
			if(fd == FAIL){
				continue;
			}
			else{
				sprintf(element_name, NAT_SUBRULE_ENTRY, IPADDRMAP, i, j);
				node = mxmlNewElement(group, element_name);
				for(k = 0; strlen(ipmap_attribute[k]) !=0; k++){
					sprintf(attr_tmp,"%s=", ipmap_attribute[k]);
					memset(tmp, 0, sizeof(tmp));
					get_profile_str(attr_tmp, tmp, QMARKS, fd);
					mxmlElementSetAttr(node, ipmap_attribute[k], tmp);
				}
			}
			close(fd);
		}
		memset(tmp, 0, sizeof(tmp));
		/*read server_ip file */
		sprintf(file_name, IPADDRMAP_SERVERIP_PATH, i);
		fd = open(file_name, O_RDONLY);
		if(fd == FAIL){
			continue;
		}
		sprintf(attr_tmp,"%s=", IPADDRMAP_ATTR_SERVERIP);
		get_profile_str(attr_tmp, tmp, NO_QMARKS, fd);
		sprintf(element_name, NAT_SUBPVC_ENTRY, IPADDRMAP, i);
		node = mxmlNewElement(group, element_name);
		mxmlElementSetAttr(node, IPADDRMAP_ATTR_SERVERIP, tmp);
		close(fd);
	}
	return 0;
}/* end ipAddrMap_read */
#endif
#if 0
/*______________________________________________________________________________
**	ipAddrMap_write
**
**	descriptions:
**		Find IP Address Mapping element from mxml tree then write ipAddrMap's system parameter to
**		specify configuration files.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  ipAddrMap cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
ipAddrMap_write(mxml_node_t *top, mxml_node_t *parant){

	int 	i=0;
	int   j=0;
	mxml_node_t *node = NULL;
	mxml_node_t *childNode = NULL;
	char ipmap_file[32];
	char element_name[32];
	const char *pValue=NULL;

	char ipmap_attribute[][ATTR_SIZE]=
	{
		{"RULETYPE"},
		{"LOCALSTARTIP"},
		{"LOCALENDIP"},
		{"PUBLICSTARTIP"},
		{"PUBLICENDIP"},
		{""},
	};
	enum ipmap_attr_en{
		RuleType=0,
		LocalSIP,
		LocalEIP,
		PubSIP,
		PubEIP
	};
	for(i = 0; i < PVC_NUM; i++){
		sprintf(element_name, SUB_PVC_NAME, i);
		childNode = mxmlFindElement(parant, parant, element_name,
				NULL, NULL,MXML_DESCEND);
		if(!childNode){
			continue;
		}

		for(j = 0; j < MAX_IPMAP_RULE; j++){
			/*
			sprintf(element_name, NAT_SUBRULE_ENTRY, IPADDRMAP, i, j);
			node = mxmlFindElement(tree, tree, element_name,
				NULL, NULL ,MXML_DESCEND);
			*/
			sprintf(element_name, "%s%d", SUB_NODE_NAME, j);
			node = mxmlFindElement(childNode, childNode, element_name,
				NULL, NULL,MXML_DESCEND);

			if(node == NULL){
				continue;
			}
			/*
				If the rule type is equal with One-to-One,,
				then we must be set defalut value with LOCALENDIP and PUBLICENDIP
				value that is 0.0.0.0.
				If the rule type is equal with Many-to-One,
				then we must be set defalut value with PUBLICENDIP
				value that is 0.0.0.0.
			*/
			pValue=mxmlElementGetAttr(node, ipmap_attribute[RuleType]);
			if(pValue){
				if((strcmp(pValue,ONE2ONE)==0)
				|| (strcmp(pValue, MANY2ONE)==0)){
					mxmlElementSetAttr(node, ipmap_attribute[PubEIP], DEF_IP);
				}
				if(strcmp(pValue, ONE2ONE)==0){
					mxmlElementSetAttr(node, ipmap_attribute[LocalEIP], DEF_IP);
				}
			}

			/*ipmap_file : /etc/nat_pvc0/ipaddr_mapping0 */
			sprintf(ipmap_file, IPADDRMAP_PATH, i, j);
			node_write(node, ipmap_file, QMARKS);
		}

		/*server_ip file : /etc/nat_pvc0/server_ip */
		sprintf(ipmap_file,IPADDRMAP_SERVERIP_PATH, i);
		/*sprintf(element_name, "SERVER", IPADDRMAP, i);*/
		node = mxmlFindElement(childNode, childNode, IPADDRMAP_SERVER,
			    NULL, NULL, MXML_DESCEND);
		node_write(node, ipmap_file, NO_QMARKS);

	}
	return SUCCESS;
}/* end ipAddrMap_write */

/*______________________________________________________________________________
**	ipAddrMap_verify
**
**	descriptions:
**		To check the system parameter of IP Address Mapping element  is validly or not.
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
ipAddrMap_verify(mxml_node_t *node){
	return SUCCESS;
}/* end ipAddrMap_verify */

/*_____________________________________________________________________________
**      function name: ipAddrMap_execute
**      descriptions:
**            To  execute ipAddrMap node service.
**
**      parameters:
**            top:   Put the ipAddrMap of cfg node tree that is used to execute virtual server settings.
**            name:  Put the ipAddrMap cfg node name , IpAddrMap_PVC0_Entry0~15
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
ipAddrMap_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char nat_pvc[4]={0};
	char ipAddr_id[4]={0};
	char cmd[32]={0};
	mxml_node_t *curNode=top;
	int i=0;

	memset(nodeName, 0,sizeof(nodeName));
	strcpy(nodeName[0], WEBCURSET);
	strcpy(nodeName[1], SUB_NODE_NAME);

	getAttrValue(top, nodeName, "nat_pvc", nat_pvc);
	getAttrValue(top, nodeName, "ipAddr_id", ipAddr_id);

	for(i=0; i < MAX_ARG_NUM; i++){
		if(strlen(name[i]) == 0){
			break;
		}
		curNode = mxmlFindElement(curNode, curNode, name[i],
				NULL, NULL,MXML_DESCEND);
	}

	/*To execute*/
	if(curNode){
		if(strcmp(name[2],IPADDRMAP_SERVER)==0){
			updateVirServ(atoi(nat_pvc));
		}else{
			/*Add IPAddrMap rule into route table*/
			sprintf(cmd, "%s add %d %d", IPADDRMAP_SH, atoi(nat_pvc), atoi(ipAddr_id));
			system(cmd);
		}
	}/*end if(curNode) */
	else{
		/*
			There is no sub cfg node information,
			it's mean user want to delete ipaddrmap entry or server_ip information
		*/
		if(strcmp(name[2], IPADDRMAP_SERVER)==0){
			sprintf(cmd, "/bin/rm -f /etc/nat_pvc%d/server_ip",  atoi(nat_pvc));
			system(cmd);
			updateVirServ(atoi(nat_pvc));
		}
		else{
			sprintf(cmd, "%s del %d %d", IPADDRMAP_SH, atoi(nat_pvc), atoi(ipAddr_id));
			system(cmd);
			/*Delete the configuration file*/
			sprintf(cmd, IPADDRMAP_PATH, atoi(nat_pvc), atoi(ipAddr_id));
			unlink(cmd);
		}
	}/*end if(curNode)*/

	return SUCCESS;
}/* end ipAddrMap_execute*/
#endif
/*_____________________________________________________________________________
**      function name: updateVirServ
**      descriptions:
**            To flush  the specify PVC NAT virserver tables and rebuild the
**            specify PVC NAT virserver tables.
**
**      parameters:
**            pvc:   Specify you want to update PVC NAT virserver tables.
**
**      global:
**             None
**
**      return:
**             None
**
**      call:
**   	 None
**
**      revision:
**      1. Here 2008/6/6
**____________________________________________________________________________
*/
void
updateVirServ(int pvc){
	struct stat fd_st;
	char file_path[32]={0};
	char cmd[32]={0};
	int i=0;

	sprintf(cmd,"iptables -t nat -F VS_PRE%d",pvc);
	system(cmd);
	for(i = 0; i < MAX_VIRSERV_RULE; i++){
		sprintf(file_path, "/etc/nat_pvc%d/vserver%d", pvc, i);
		if(stat(file_path, &fd_st)!=-1){
			/*Update virserver rule*/
			sprintf(cmd, "%s add %d %d",VIRSERV_SH,pvc, i);
			system(cmd);
		}
	}
}/*end updateVirServ*/

#if 0  /*Orginal ci-command mode */
/*______________________________________________________________________________
**	adsl_read
**
**	descriptions:
**		Read xdsl  system parameter from configuration file and record those
**		information to mxml_node_t.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
adsl_read(mxml_node_t *top){
	int retval=0;
	char *pAttr;

	pAttr=(char *)adsl_attribute;
	retval=node_read(top,ADSL, ADSL_PATH, pAttr, QMARKS);
	return retval;
}/* end adsl_read */
#endif

#endif
//"Adsl",		SINGLE_TYPE,	NULL, NULL,NULL, adsl_write, adsl_verify, adsl_execute},
/*______________________________________________________________________________
**	adsl_boot
**
**	descriptions:
**		the boot function of adsl.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
adsl_boot(mxml_node_t *top){
	FILE *startupSh=NULL;

	startupSh=fopen("/etc/adsl.sh","r");
	if(startupSh){
		fclose(startupSh);
		system("chmod 755 /etc/adsl.sh");
		system("/etc/adsl.sh");
	}
	else{
		system("wan ghs set annex a");
		system("wan adsl reset");
	}
	return SUCCESS;
}
/*______________________________________________________________________________
**	adsl_init
**
**	descriptions:
**		the init function of adsl.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
adsl_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="Adsl",
			.type=SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=adsl_write,
			.cfg_verify=adsl_verify,
			.cfg_execute=adsl_execute,
			.cfg_boot=adsl_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}
/*______________________________________________________________________________
**	adsl_write
**
**	descriptions:
**		Find adsl element from mxml tree then write adsl's system parameter to specify
**		configuration files.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  adsl cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
adsl_write(mxml_node_t *top, mxml_node_t *parant){
	int retval = 0;
	mxml_node_t *node = NULL;
	char adsl_cmd[2][32];
	char buf[128];
	const char *tmp=NULL;
	int i=0;

	char adsl_mode[][2][ATTR_SIZE]=
	{
		{"Auto Sync-Up", "multimode"},
#if defined(TCSUPPORT_CT_WAN_PTM)
		{"ADSL Auto Sync-Up", "adsl2plus_multi"},	//support ADSL AUTO
		{"VDSL2", "vdsl2"},
#endif
		{"ADSL2+", "adsl2plus"},
		{"ADSL2", "adsl2"},
		{"G.DMT", "gdmt"},
		{"T1.413", "t1.413"},
		{"G.lite", "glite"},
		{"",""},
	};

	char adsl_type[][2][ATTR_SIZE]=
	{
		{"ANNEX A","a"},
		{"ANNEX I","i"},
		{"ANNEX A/L","al"},
		{"ANNEX M","m"},
		{"ANNEX A/I/J/L/M","aijlm"},
		{"",""},
	};
	/*
	node = mxmlFindElement(tree, tree, ADSL_ENTRY,
		NULL, NULL,
		MXML_DESCEND);
	*/
	node = mxmlFindElement(parant, parant, SUB_NODE_NAME,
		NULL, NULL, MXML_DESCEND);
	retval = node_write(node, ADSL_PATH, QMARKS);

	/*Create adsl.sh*/
	if(node != NULL){

		for(i = 0; strlen(adsl_mode[i][0])!=0; i++){
			tmp=mxmlElementGetAttr(node, ADSL_ATTR_MODE);
			if(tmp==NULL){
				fprintf(stderr,"Can't get adsl attr mode\n");
				continue;
			}
			#if 0
			if(strcmp(mxmlElementGetAttr(node, ADSL_ATTR_MODE), adsl_mode[i][ADSL_NAME_INDEX]) == 0){
			#endif
			if(strcmp(tmp, adsl_mode[i][ADSL_NAME_INDEX]) == 0){
				sprintf(adsl_cmd[0],"%s %s\n",ADSL_OPEN_CMD, adsl_mode[i][ADSL_CMD_INDEX]);
				break;
			}
		}

		for(i = 0; strlen(adsl_type[i][0])!=0; i++){
			tmp=mxmlElementGetAttr(node, ADSL_ATTR_TYPE);
			if(tmp==NULL){
				fprintf(stderr,"Can't get adsl attr type\n");
				continue;
			}
			#if 0
			if(strcmp(mxmlElementGetAttr(node, ADSL_ATTR_TYPE), adsl_type[i][ADSL_NAME_INDEX]) == 0){
			#endif
			if(strcmp(tmp, adsl_type[i][ADSL_NAME_INDEX]) == 0){
				sprintf(adsl_cmd[1],"%s %s\n", ADSL_SETANNEX_CMD, adsl_type[i][ADSL_CMD_INDEX]);
				break;
			}
		}

		sprintf(buf,"%s%s", adsl_cmd[0], adsl_cmd[1]);
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
		tmp=mxmlElementGetAttr(node, ADSL_ACTIVE);
		if(tmp!=NULL){
			if(strcmp(tmp, "Yes") == 0)
				sprintf(adsl_cmd[2],"%s\n", SAR_OPEN_CMD);	
			if(strcmp(tmp, "No") == 0)
				sprintf(adsl_cmd[2],"%s\n", SAR_CLOSE_CMD);
                
            sprintf(buf,"%s%s", buf, adsl_cmd[2]);	
		}
#endif
		write2file(buf, ADSL_SH);
	}
	return retval;
}/* end adsl_write */

/*______________________________________________________________________________
**	adsl_verify
**
**	descriptions:
**		To check the system parameter of adsl element  is validly or not.
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
adsl_verify(mxml_node_t *node){
	return SUCCESS;
}/* end adsl_verify */

/*_____________________________________________________________________________
**      function name: adsl_execute
**      descriptions:
**            To  execute adsl node service.
**
**      parameters:
**            node:   Put the adsl of cfg node tree that is used to execute adsl settings.
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
adsl_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
  char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
  char temp[4]={0};
  memset(nodeName,0,sizeof(nodeName));
  strcpy(nodeName[0],ADSL_NAME);
  strcpy(nodeName[1],SUB_NODE_NAME);
  getAttrValue(top, nodeName, ADSL_ACTIVE, temp);
#endif
  system(ADSL_SH);
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
	if(strcmp(temp, "No") != 0)
#endif
  system("wan adsl reset");
	return SUCCESS;
}/* end adsl_execute*/
#ifndef PURE_BRIDGE
int checkInternetGw(mxml_node_t *top, char *gw, int action)
{
	int idx = 0;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char cmdbuf[128] = {0};
	char gateWay[32] = {0}, net_mask[32] = {0}, sub_ip[20] = {0};
	char gui_ifaceName[64] = {0}, if_name[32] = {0};
	char linkmode[32] = {0};
	char def_route_index_v4[10] = {0};
	int i_def_route_index_v4 = -1;
	int pvc_index = 0, entry_index = 0;
	int isPPP = 0;
	FILE *fp = NULL;
	struct in_addr gatewayIP = {0};
	struct in_addr netmask = {0};
	struct in_addr subIP = {0};

	if ( NULL == gw || NULL == top
		|| ROUTE_ADD != action )
		return -1;

	/* get default route for ipv4 */
	memset(def_route_index_v4, 0, sizeof(def_route_index_v4));
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], WAN_IF_NODE);
	strcpy(nodeName[1], WAN_IF_COMMON);
	if ( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, "DefRouteIndexv4", def_route_index_v4)
		&& '\0' != def_route_index_v4[0] && 0 != strcmp(def_route_index_v4, "N/A") )
		i_def_route_index_v4 = atoi(def_route_index_v4);

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], WAN_IF_NODE);

	for( idx = 0; idx < MAX_WAN_IF_INDEX; idx ++ )
	{
		if ( idx == i_def_route_index_v4 ) // no need check for default route.
			continue;

		sprintf(nodeName[1], "%s%d", SUB_NODE_NAME, idx);

		memset(gateWay, 0, sizeof(gateWay));
		if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "GateWay", gateWay)
			|| '\0' == gateWay[0] )
			continue;
		if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "NetMask", net_mask)
			|| '\0' == net_mask[0] )
			continue;

		if ( 0 != strcmp(gw, gateWay) )
			continue;
	
		memset(gui_ifaceName, 0, sizeof(gui_ifaceName));
		if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "GUIInterfaceName", gui_ifaceName) )
			continue;
		if ( NULL == strstr(gui_ifaceName, INTERNET) )
			continue;

		pvc_index = idx / MAX_SMUX_NUM;
		entry_index = idx % MAX_SMUX_NUM;
		
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0], WAN);
		sprintf(nodeName[1], SUB_PVC_NAME, pvc_index);
		sprintf(nodeName[2], "%s%d", SUB_NODE_NAME, entry_index);

		if ( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "LinkMode", linkmode)
			|| '\0' == linkmode[0] )
			return 1;
		if ( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, WAN_IFNAME, if_name)
			|| '\0' == if_name[0] )
			return 2;

		if( 0 == strcmp(linkmode,"linkPPP") )
			isPPP = 1;

		if( isPPP )
		{
			strcpy(sub_ip, gateWay);
			strcpy(net_mask, "255.255.255.255");
		}
		else
		{
			inet_aton(gateWay, &gatewayIP);
			inet_aton(net_mask, &netmask);
			subIP.s_addr = (gatewayIP.s_addr & netmask.s_addr);
			strcpy(sub_ip, inet_ntoa(subIP));
		}

		sprintf(cmdbuf, "/sbin/route add -net %s netmask %s dev %s", sub_ip, net_mask, if_name);
		fp = fopen(ROUTE_EXE_PRE_SH, "w");
		if ( fp )
		{
			fputs(cmdbuf, fp);
			fclose(fp);
			fp = NULL;
			chmod(ROUTE_EXE_PRE_SH, 777);
		}
		sprintf(cmdbuf, "/sbin/route del -net %s netmask %s dev %s", sub_ip, net_mask, if_name);
		fp = fopen(ROUTE_EXE_AFT_SH, "w");
		if ( fp )
		{
			fputs(cmdbuf, fp);
			fclose(fp);
			fp = NULL;
			chmod(ROUTE_EXE_AFT_SH, 777);
		}

		break;
	}

	return 0;
}

/*______________________________________________________________________________
**function name:create_route_exe_sh
**
**description:
*    create the route execute script
**parameters:
*    top:Specify the root of mxml_node_t structure.
*    action:0:del
*           1:add
**global:
*    none
**return:
*    none
**call:
*    none
**revision:
*     1.shnwind 20080526
**____________________________________________________________________________*/
void
create_route_exe_sh(mxml_node_t *top, int action){

	/*I modify the tmp size from 24 to 32 because 24 bit is too short to put netmask.
	   shnwind 2008.9.25*/
	char nodeName[3][MAX_NODE_NAME], route_tmp[16]={0}, /*tmp[24]={0}, */tmp[32]={0}, string[128]={0};
	FILE *fp;
	int route_id;
	int wan_device_index;
	char wan_active[6] = {0}, route_active[8] = {0};

	mxml_node_t *group = NULL;
	mxml_node_t *node = NULL;
	
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],WEBCURSET);
	strcpy(nodeName[1],SUB_NODE_NAME);
	/*get route id*/
	if(getAttrValue(top, nodeName, "route_id", route_tmp) == TCAPI_PROCESS_OK){
		route_id = atoi(route_tmp);
	}else{
		return;
	}
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],ROUTE);
	sprintf(nodeName[1],"Entry%d",route_id);
	/*	unset 	-> del
		commit  	-> add*/
	if(action == ROUTE_ADD){
		strcpy(string, "/sbin/route add ");

		bzero(route_active, sizeof(route_active));
		/* default will be active. */
		if ( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "Active"
			, route_active) )
		{
			strcpy(route_active, "Yes");
			setAttrValue(top, nodeName, "Active", route_active);
		}

		if ( 0 != strcmp(route_active, "Yes") )
		{
			strcpy(string, "#/sbin/route add "); /* mark add operation. */
		}
	}else if(action == ROUTE_DEL){
		strcpy(string, "/sbin/route del ");
	}
	/*write dest ip address*/
	if(getAttrValue(top, nodeName, "DST_IP", route_tmp)== TCAPI_PROCESS_OK){
		sprintf(tmp,"-net %s ",route_tmp);
		strcat(string, tmp);
	}else{
	/*node has been delete*/
		return ;
	}
	/*write sub mask*/
	if(getAttrValue(top, nodeName, "Sub_mask", route_tmp)== TCAPI_PROCESS_OK){
	sprintf(tmp,"netmask %s ",route_tmp);
	strcat(string, tmp);
        }
	/*write device info*/
	if(getAttrValue(top, nodeName, "Device", route_tmp) == TCAPI_PROCESS_OK){
		if(strcmp(route_tmp,"br0") != 0){		// not br0			
			#if 0
			if(strstr(route_tmp, "PVC")){//device PVC0
				if(change_device_name(top, route_tmp, route_tmp) == 0){
					sprintf(tmp,"dev %s ",route_tmp);
					strcat(string, tmp);
				}else{
					/*change_device_name fail*/
					//because the pvc is not active ,add the route fail,so delete the node
					group = mxmlFindElement(top, top, ROUTE,NULL, NULL, MXML_DESCEND);
					if(group == NULL)
					{
						return;
					}

					node = mxmlFindElement(group, group, nodeName[1],
					NULL, NULL, MXML_DESCEND);
					if(node)
					{
						mxmlDelete(node);
					}
					return;
				}
			}
			#endif

			/*Device value is wan interface name, eg:nas2_1*/
			/*Check wan status*/
			if((wan_device_index = get_wanindex_by_name(route_tmp)) < 0){
					tcdbg_printf("\n %s:get wan index from %s failed!\n", __func__, route_tmp);
					return;
			}
			if(get_waninfo_by_index(wan_device_index, "Active", wan_active) == SUCCESS){
				if(!strcmp(wan_active, DEACTIVE)){
					tcdbg_printf("\n %s:%s is not active,add static route failed!\n", __func__, route_tmp);
					//mxmlDelete(curNode);
					return;			
				}		
			}
			else{
				/*change_device_name fail*/
				//because the pvc is not active ,add the route fail,so delete the node
				group = mxmlFindElement(top, top, ROUTE,NULL, NULL, MXML_DESCEND);
				if(group == NULL)
				{
					return;
				}

				node = mxmlFindElement(group, group, nodeName[1],
				NULL, NULL, MXML_DESCEND);
				if(node)
				{
					mxmlDelete(node);
				}
				return;
			}
		}
		
		sprintf(tmp,"dev %s ",route_tmp);
		strcat(string, tmp);
	}
	/*write gateway info*/
	if(getAttrValue(top, nodeName, "Gateway", route_tmp) == TCAPI_PROCESS_OK){
		if(strcmp(route_tmp,"0.0.0.0")){
			sprintf(tmp,"gw %s ",route_tmp);
			strcat(string, tmp);
			checkInternetGw(top, route_tmp, action);
		}
	}
	/*write metric value*/
	if(getAttrValue(top, nodeName, "metric", route_tmp) == TCAPI_PROCESS_OK){
		if(atoi(route_tmp) > 0){
			sprintf(tmp,"metric %s ",route_tmp);
			strcat(string, tmp);
		}
	}

	fp = fopen(ROUTE_EXE_SH,"a+");
	if(fp != NULL){
		strcat(string, "\n");
		fputs(string, fp);
		fclose(fp);
		chmod(ROUTE_EXE_SH,777);
	}

}
/*______________________________________________________________________________
**function name:create_route_sh
**
**description:
*    save all user define entry to the script.
**parameters:
*    top:Specify the root of mxml_node_t structure.
*    parant:Specify the parant node of  route cfg node.
**global:
*    none
**return:
*    none
**call:
*    none
**revision:
*     1.shnwind 20080526
**____________________________________________________________________________*/
void
create_route_sh(mxml_node_t *top, mxml_node_t *parant){

	/*I modify the tmp size from 24 to 32 because 24 bit is too short to put netmask.
	   shnwind 2008.9.25*/
	char /*tmp[24]={0},*/tmp[32]={0}, string[128]={0}, element_name[16]={0};
	const char *Value=NULL;
	FILE *fp;
	int route_count=0;
	mxml_node_t *node = NULL;

	
	unlink(ROUTE_PATH);
	#if 0//modify by brian
	while(1){
	#else
	for(route_count = 0; route_count < MAX_STATIC_ROUTE_NUM; route_count++){
	#endif
		/*sprintf(element_name, ROUTE_ENTRY, route_count);*/
		sprintf(element_name,"%s%d", SUB_NODE_NAME, route_count);
		/*do not find node*/
		node = mxmlFindElement(parant, parant, element_name, NULL , NULL, MXML_DESCEND);
		if(node == NULL){
			#if 0//modify by brian
			break;
			#else
			continue;
			#endif
		}
		#if 0
		/*get user define num*/
		Value = mxmlElementGetAttr(node, "User_def");
		if(!strcmp(Value, "0") || (Value == NULL)){
			continue;
		}
		#endif
		memset(string, 0,sizeof(string));
		strcpy(string, "/sbin/route add ");
		Value = mxmlElementGetAttr(node, "Active");
		if ( 0 != strcmp(Value, "Yes") )
		{
			strcpy(string, "#/sbin/route add "); /* mark add operation. */
		}
		/*write destnation IP*/
		Value = mxmlElementGetAttr(node, "DST_IP");
		if(Value){
			sprintf(tmp,"-net %s ",Value);
			strcat(string, tmp);
		}
		/*write mask*/
		Value = mxmlElementGetAttr(node, "Sub_mask");
		if(Value){
			sprintf(tmp,"netmask %s ",Value);
			strcat(string, tmp);
		}
		/*write device information*/
		Value = mxmlElementGetAttr(node, "Device");
		if(Value){
			#if 0
			if(strstr(Value, "PVC")){/*save from web page*/
				strcpy(tmp,Value);
				/*change device name from PVC to nas or ppp*/
				if(change_device_name(top, tmp, dev) == 0){
					sprintf(tmp,"dev %s ",dev);
					strcat(string, tmp);
				}
			}else{
				/*not save from web page*/
				sprintf(tmp,"dev %s ",Value);
				strcat(string, tmp);
			}
			#endif
			sprintf(tmp,"dev %s ",Value);
			strcat(string, tmp);
		}
		/*write Gateway information*/
		Value = mxmlElementGetAttr(node, "Gateway");
		if((Value != NULL) && (strcmp(Value,"0.0.0.0"))){
			sprintf(tmp,"gw %s ",Value);
			strcat(string, tmp);
		}
		/*write metric number if number is bigger than 0.*/
		Value = mxmlElementGetAttr(node, "metric");
		if((Value != NULL) && (strcmp(Value,"0"))){
			sprintf(tmp,"metric %s ",Value);
			strcat(string, tmp);
		}
		strcat(string, "\n");
		fp = fopen(ROUTE_PATH, "a");
		if(fp != NULL){
			fputs(string, fp);
			fclose(fp);
		}
	}
	chmod(ROUTE_PATH,777);

}

/*krammer add for bug 1427*/
/*______________________________________________________________________________
**function name:update_route_sh
**
**description:
*    Update the route script.
**parameters:
*    top:Specify the root of mxml_node_t structure.
*    parant:Specify the parant node of  route cfg node.
**global:
*    none
**return:
*    none
**call:
*    none
**revision:
*     1.krammer 20080526
**____________________________________________________________________________*/
void
update_route_sh(mxml_node_t *top, mxml_node_t *parant){
	FILE *exeSh=NULL;
	FILE *routeSh=NULL;
	FILE *tmpFp=NULL;
	char line[MAXSIZE]={0};
	char tmp[MAXSIZE]={0};
	char *dest_ip_exe=NULL;
	char *p=NULL;
	char *delim=" ";
	int action=0;


	exeSh=fopen(ROUTE_EXE_SH,"r");
	if(!exeSh){
		return;
	}
	routeSh=fopen(ROUTE_PATH,"a+");
	if(!routeSh){
		fclose(exeSh);
		return;
	}

	fgets(line,MAXSIZE,exeSh);

	/*use the route_exe_sh to determin the action.*/
	if(strlen(line)){
		if('#' != line[0] && strstr(line,"/sbin/route add")){
			action=ROUTE_ADD;
		}
		else{
			action=ROUTE_DEL;
		}
	}
	else{
		fclose(exeSh);
		fclose(routeSh);
		return;
	}

	/*action add: just put the line of route_exe_sh into route.sh*/
	if(action==ROUTE_ADD){
		strcat(line,"\n");
		fputs(line,routeSh);
		fclose(exeSh);
		fclose(routeSh);
	}
	/*action del*/
	else if(action==ROUTE_DEL){
		strcpy(tmp,line);
		p=strtok(tmp,delim);
		/*get the ip address */
		while(p != NULL){
			p = strtok(NULL,delim);
			if(!strcmp(p,"-net")){
				dest_ip_exe= strtok(NULL,delim);
				break;
			}
		}

		tmpFp=fopen(ROUTE2_PATH,"w");
		if(!tmpFp){
			fclose(exeSh);
			fclose(routeSh);
			return;
		}

		while(fgets(line,MAXSIZE,routeSh)){
			if(!strstr(line,dest_ip_exe)){
				/*if the dest_ip is not the same, we can put it into route2.sh*/
				fputs(line,tmpFp);
			}
		}

		fclose(exeSh);
		fclose(routeSh);
		fclose(tmpFp);
		unlink(ROUTE_PATH);
		rename(ROUTE2_PATH, ROUTE_PATH);
	}
  	return;
}

/*______________________________________________________________________________
**function name:change_device_name
**
**description:
*    change the device name from PVCn to pppn or nasn
**parameters:
*    	top:Specify the root of mxml_node_t structure.
*	dev:device name
*	ret:ret the device name after change
**global:
*    none
**return:
*    0:success
*    -1:fail
**call:
*    none
**revision:
*     1.shnwind 20080526
**____________________________________________________________________________*/
int
change_device_name(mxml_node_t *top, char *dev, char *ret){

	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], route_tmp[8]={0}, isp[4]={0}, active[4]={0};

	strcpy(route_tmp, dev);
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],WAN);
	strcpy(nodeName[1],dev);//dev:PVCn

	if((getAttrValue(top, nodeName, WAN_ISP, isp) == TCAPI_PROCESS_OK)
	&&(getAttrValue(top, nodeName, NODE_ACTIVE, active) == TCAPI_PROCESS_OK)){
		/*the pvc must active*/
		if(!strcmp(active, ACTIVE)){
			/*	ppp mode -> pppn
				other        -> nas0*/
			if(atoi(isp) == WAN_ENCAP_PPP){
				route_tmp[0] = 'p';
				route_tmp[1] = 'p';
				route_tmp[2] = 'p';
			}else{
				route_tmp[0] = 'n';
				route_tmp[1] = 'a';
				route_tmp[2] = 's';
			}
		}else{
		return FAIL;
		}
	}else{
		return FAIL;
	}
	strcpy(ret,route_tmp);
	return SUCCESS;
}
/*______________________________________________________________________________
**function name:rebuild_route
**
**description:
*    rebuild the route node only has user define entry.
*	copy from original route_read.
**parameters:
*    top:Specify the root of mxml_node_t structure.
**global:
*    none
**return:
*    0:success
*    -1:fail
**call:
*    none
**revision:
*     1.shnwind 20080526
**____________________________________________________________________________*/
int
rebuild_route(mxml_node_t *top){
	FILE *fp = NULL;
	char tmp[MAXSIZE];
	char routeInfo[10][16];
	char element_name[32];
	int i=0;

	mxml_node_t *group= NULL;  /* <Route> */
	mxml_node_t *node= NULL;   /* <node> */

	/*group = mxmlNewElement(top, ROUTE);*/
	group = mxmlFindElement(top, top, ROUTE,
	  	NULL , NULL,MXML_DESCEND);
	fp = fopen(ROUTE_PATH, "r");
	if(fp == NULL){
		return FAIL;
	}
	else{
		while(fgets(tmp, MAXSIZE, fp)){
			memset(routeInfo, 0, sizeof(routeInfo));
			/*
			route.sh : /sbin/route add -net 100.100.100.0 netmask 255.255.255.0 dev nas0 metric 1
					 or /sbin/route add -net 20.20.20.0 netmask 255.255.255.0 gw 192.168.1.1
			*/
			sscanf(tmp, "%s %s %s %s %s %s %s %s %s %s",
			routeInfo[0], routeInfo[1], routeInfo[2],
			routeInfo[3], routeInfo[4], routeInfo[5],
			routeInfo[6], routeInfo[7], routeInfo[8],routeInfo[9]);

			sprintf(element_name,"%s%d",SUB_NODE_NAME,i);
			node = mxmlNewElement(group, element_name);
			mxmlElementSetAttr(node, ROUTE_ATTR_DST_IP, routeInfo[ROUTE_CFG_IP_INDEX]);
			mxmlElementSetAttr(node, ROUTE_ATTR_SUB_MASK, routeInfo[ROUTE_CFG_SUB_MASK_INDEX]);
			if(strcmp(routeInfo[ROUTE_CFG_DEV_INDEX], ROUTE_CMD_DEV) == 0){
				mxmlElementSetAttr(node,  ROUTE_ATTR_DEVICE, routeInfo[ROUTE_CFG_GW_INDEX]);
			}
			else{
				mxmlElementSetAttr(node,  ROUTE_ATTR_GW, routeInfo[ROUTE_CFG_GW_INDEX]);
			}

			if(strlen(routeInfo[ROUTE_CFG_METRIC_NAME_INDEX]) !=0){
				mxmlElementSetAttr(node,  routeInfo[ROUTE_CFG_METRIC_NAME_INDEX], routeInfo[ROUTE_CFG_METRIC_INDEX]);
			}
			mxmlElementSetAttr(node,  "User_def", "1");
			i++;
		}
		fclose(fp);
		return SUCCESS;
	}
}/* end rebuild_route */

/*______________________________________________________________________________
**	firewall_check_filter
**
**	descriptions:
**		check if the firewall need filter module or not.
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
firewall_check_filter(mxml_node_t *top){

//tcdbg_printf("run firewall check filter\n");
	int filter_on=0;
	char firewall_status[2]={0}, spi_status[2]={0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	memset(nodeName,0,sizeof(nodeName));

	strcpy(nodeName[0],FIREWALL);
	strcpy(nodeName[1],SUB_NODE_NAME);
	if((getAttrValue(top, nodeName, "firewall_status", firewall_status) == TCAPI_PROCESS_OK)
	&&(getAttrValue(top, nodeName, "spi_status", spi_status) == TCAPI_PROCESS_OK)){
	if(!strcmp(firewall_status,"1") || !strcmp(spi_status,"1")){
			filter_on = 1;
		}
	}
	return filter_on;
}


void
firewall_scan_port_and_add_rule(){

	FILE *firewall_tmp=NULL;
	FILE *firewall_script=NULL;
	char str[128]={0};
	char *p=NULL;
	int index=0, i=0;
	char port[8]={0};
	char firewall_path[32]="/var/tmp/firewall.tmp";
	char firewall_script_path[32]="/var/tmp/firewall_exe.sh";
	char netstate_tcp[16]="netstat -tnl > ";
	char netstate_udp[16]="netstat -unl > ";
	char netstate_cmd[64]={0};
	char iptables[64]="iptables -t filter -A FIREWALL -p";
	char cmd[256]={0};
	/*
	* write firewall rule according to netstat -nl
	*
	* Active Internet connections (only servers)
	*	Proto Recv-Q Send-Q Local Address               Foreign Address             State
	*	tcp        0      0 0.0.0.0:2049                0.0.0.0:*                   LISTEN
	*
	*
	*/

	firewall_script=fopen(firewall_script_path, "w");
	if(firewall_script){
		for(i=0; i<2; i++){//write 0:tcp 1:udp
			memset(cmd,0,sizeof(cmd));
			memset(netstate_cmd, 0, sizeof(netstate_cmd));
			strcat(cmd, iptables);
			index=0;
			if(i==0){
				strcat(netstate_cmd, netstate_tcp);
				strcat(cmd, " TCP -m multiport --dport ");
			}
			else{
				strcat(netstate_cmd, netstate_udp);
				strcat(cmd, " UDP -m multiport --dport ");
			}
			strcat(netstate_cmd, firewall_path);
			system(netstate_cmd);
			firewall_tmp=fopen(firewall_path, "r");

			if(firewall_tmp){
				while(fgets(str, 128, firewall_tmp)){
					index++;
					p=strstr(str, ":");
					if(p){
						/*Begin----------Judge ipv6*/
						if(*(p+1) == ':')
							continue;//The colon num in IPv6 address is large than 1, such as ":::53",so do not process it. Maybe we need to modify this method in further. 
						/*------------End*/
						sscanf((p+1), "%s", port);
						if(index > 3){//add "," when the line more than 3
							strcat(cmd, ",");
						}
						strcat(cmd, port);
					}
				}
				if(index > 2){//skip first 2 line
					strcat(cmd, " -j ACCEPT\n");
					fputs(cmd, firewall_script);
				}
				fclose(firewall_tmp);
				unlink(firewall_path);
			}
		}
		fputs("iptables -t filter -A FIREWALL -p icmp -j ACCEPT\n", firewall_script);
		fputs("iptables -t filter -A FIREWALL -j DROP\n", firewall_script);
		fclose(firewall_script);
		chmod(firewall_script_path, 777);
		system(firewall_script_path);
		unlink(firewall_script_path);
	}
}


#endif

#ifndef PURE_BRIDGE
#if 1
/*______________________________________________________________________________
**  checkName
**    This function detect if input is a domain name or a IP addr.
**  descriptions:
**  parameters:
**  local:
**  global:
**  return:  0 : this is a domain name
		   1 : this is a IP
**  called by:resolveName()
**  call:
**  revision:
**____________________________________________________________________________*/
unsigned char checkName(char *Name){
	char c;

	if(Name == NULL)
		return 0;	/* Can't happen */
	
	while((c = *Name++) != '\0'){
		if((!isdigit(c)) && (c != '.') && (c != ':')){
			if(c == '/'){
				return 1;
			}
			return 0;
		}
	}
	return 1;

}

//add by brian (copy from linos)
int resolve(char *ipstr, unsigned long *addr, unsigned short *netmask)
{
	unsigned long ipaddr = 0;
	int i, bits;
	unsigned long digit;
	char *index;
	short mask = -1;

	for (i = 0; i < 3; i++) {
		index = strchr(ipstr, '.');
		if (index == NULL) break;
		*index = '\0';
		digit = atoi(ipstr);
		*index = '.';
		if (digit > 255) return -1;
		ipaddr = (ipaddr << 8) | digit;
		ipstr = index + 1;
	}

	index = strchr(ipstr, '/');     /* netmask ? */
	if (index != NULL) {
		mask = atoi(&index[1]);
		if (mask < 0 || mask > 32) return -1;
		*index = '\0';
	}

	digit = atoi(ipstr);
	bits = (4 - i) << 3;
	if (digit > ((1 << bits) - 1)) return -1;
	ipaddr = (ipaddr << bits) | digit;

	digit = ipaddr >> 28;		/* check multicast and reserved IP */
	if (digit >= 14) return -1;
	if (mask < 0) {
		if (digit < 8)
			mask = 8;
		else if (digit < 12)
			mask = 16;
		else if (digit < 14)
			mask = 24;
	}
	else
		*index = '/';

	*addr = ipaddr;
	if (netmask != NULL) *netmask = mask;
	return 0;
}

unsigned long resolveName(char *name)
{
	struct hostent *h = NULL;
	struct sockaddr_in addr = {0};
	unsigned long ipAddr = 0;

	if(name == NULL)
		return 0;
	tcdbg_printf("\r\nEnter reslove name: [%s]", name);
	/*add by brian*/
	if(checkName(name))
	{
		resolve((char *)name, &ipAddr, NULL);
		if(ipAddr != 0)
		{
			return ipAddr;
		}
	}

	h = gethostbyname(name);

	if(h != NULL)
	{
		memcpy(&addr.sin_addr, h->h_addr, sizeof(addr.sin_addr));
		tcdbg_printf("\r\nreslove name success: name [%s] IP is [%s]", name, inet_ntoa(addr.sin_addr));	
	}
	else
	{

		tcdbg_printf("\r\nresolve Name failed!");
	}

	return (unsigned long)addr.sin_addr.s_addr;
}

int parseUrl(const char *theUrlPtr, char *theHostNamePtr, unsigned int *thePortPtr) 
{
    unsigned short  theLength;
    int    theResult = 0;
    char  *theSourcePtr;
	unsigned long theHostAddressPtr;
	struct sockaddr_in addr = {0};

    *theHostNamePtr = '\0';
    *thePortPtr = 80;

#ifdef SSL
	if( strncmp(theUrlPtr, "https", 5) == 0){ 
		*thePortPtr = 443;
	}
#endif

	if ( theUrlPtr == NULL ) {
		return -1;
	}
	
    if ( (theSourcePtr = strstr(theUrlPtr, ":")) != NULL ) {
		/*
		    The path is absolute, so skip over the "http://".
        */
        theSourcePtr += 3;
		theUrlPtr = theSourcePtr;
		
        theLength = 0;

        while (*theSourcePtr != ':' && *theSourcePtr != '/' &&
                *theSourcePtr != '\0') {
            theSourcePtr++;
            theLength++;
        }

        if (theLength < 64) {
            strncpy(theHostNamePtr, theUrlPtr, theLength);
            *(theHostNamePtr + theLength) = '\0';
        }
        else {
            theResult = -1;
        }

        /*
            If there was a port specified, convert it to numeric
            otherwise return the default port value.
        */
        if (*theSourcePtr == ':') {
            theSourcePtr += 1;
            *thePortPtr = atoi(theSourcePtr);
        }
    }
	else {
		theResult = -1;
	}

	theHostAddressPtr = resolveName(theHostNamePtr);
	//addr.sin_addr = theHostAddressPtr;
	memcpy(&addr.sin_addr, &theHostAddressPtr, sizeof(addr.sin_addr));
	strcpy(theHostNamePtr, inet_ntoa(/*theHostAddressPtr*/addr.sin_addr));
	
#ifdef QOS_DEBUG
	tcdbg_printf("The theHostNamePtr is %s\n", theHostNamePtr);
#endif

    return theResult;
}
#endif

#ifdef TCSUPPORT_QOS
/*____________________________________________________________________________
**	QoS Node Attribute
**	QoS_Common:	
**				Active = {Yes,No}
**				Discpline = {PQ,WRR,CAR}
**				UplinkBandWidth
**				WeightHighest
**				WeightHigh
**				WeighMedium
**				WeighLow
**				RuleType = {Type, App, Template}
**	QoS_Entry: 0~15
**				Active = {Yes,No}
**				
**				
**
**
**
**_____________________________________________________________________________
*/
#if defined(TCSUPPORT_CT_QOS)
int setQueueMask ( char *qm, char sw_pri[][32], int qNum )
{
	int i, q = 0;
	if (qm == NULL)
		return -1;
	
	for (i = 0; i < qNum; i++) {
		if ( strcmp(sw_pri[i], "Yes") == 0 ) {
			q += (1 << i);
		}
	}
	
#ifdef QOS_DEBUG
	tcdbg_printf(" q is %d\n", q );
#endif

	sprintf( qm, "%d", q );
	return 0;
}
#endif

int qosTCModule( int type )
{
	int i;
	char modeCmd[128];
	char qos_tc_name[QOS_TC_MODULE_NUM][32] =
	{
		{"act_mirred.ko"},{"cls_fw.ko"},
		{"sch_htb.ko"},{"sch_prio.ko"}
	};

	memset( modeCmd, 0, sizeof(modeCmd) );
	for ( i=0; i<QOS_TC_MODULE_NUM; i++ ) {
		if ( type == INSMODE ) {
			sprintf( modeCmd,"insmod %s%s\n",QOS_TC_MODULE_PATH, qos_tc_name[i] );
		}
		else if ( type == RMMODE ) {
			sprintf( modeCmd,"rmmod %s%s\n",QOS_TC_MODULE_PATH, qos_tc_name[i] );
		}
		else {
			return -1;
		}
		system(modeCmd);
	}

	return 0;
}

int qosEBTModule( int type )
{
	int i;
	char modeCmd[128];
	char qos_ebt_name[QOS_EBT_MODULE_NUM][32] =
	{
#if defined(TCSUPPORT_CT_VLAN_BIND)
		{"ebt_ip.ko"},{"ebt_ftos.ko"}
#else
		{"ebtables.ko"},{"ebtable_broute.ko"},{"ebt_ip.ko"},
		{"ebt_vlan.ko"},{"ebt_ftos.ko"},{"ebt_mark.ko"}
#endif
#ifdef IPV6
		,{"ebt_ip6.ko"},{"ebt_tc.ko"}
#endif
	};

	memset( modeCmd, 0, sizeof(modeCmd) );
	for ( i=0; i<QOS_EBT_MODULE_NUM; i++ ) {
		if ( type == INSMODE ) {
			sprintf( modeCmd,"insmod %s%s\n",QOS_EBT_MODULE_PATH, qos_ebt_name[i] );
		}
		else if ( type == RMMODE ) {
			sprintf( modeCmd,"rmmod %s%s\n",QOS_EBT_MODULE_PATH, qos_ebt_name[i] );
		}
		else {
			return -1;
		}
		system(modeCmd);
	}

	return 0;
}

int excQosCmd( char *cmd )
{
	if ( cmd == NULL ) {
		printf( "==>excQosCmd(): cmd is NULL.\n" );
		return -1;
	}
	if ( strlen(cmd) == 0 ) {
		return -1;
	}
#ifdef QOS_DEBUG
	tcdbg_printf( "QOSCMD=%s\n", cmd );
#endif

	system( cmd );
	return 0;
}

#if defined(TCSUPPORT_CT_QOS)
void deleteQosTempRules()
{
	int i;
	char qosCmd[64] = {0};

	for(i = 0; i < MAX_APP_RULE_NUM; i++)
	{
		memset(qosCmd,0,sizeof(qosCmd));
		sprintf( qosCmd, "qoscmd app del -i %d", i);	
		system(qosCmd);
	}
	
	for(i = MAX_TYPE_RULE_NUM; i < MAX_APP_TYPE_NUM; i++)
	{
		memset(qosCmd,0,sizeof(qosCmd));
		sprintf( qosCmd, "qoscmd type del -i %d", i);	
		system(qosCmd);
	}
	
	for(i = MAX_TYPE_RULE_NUM; i < MAX_APP_TYPE_NUM; i++)
	{
		memset(qosCmd,0,sizeof(qosCmd));
		sprintf( qosCmd, "qoscmd remark del -i %d", i);	
		system(qosCmd);
	}
	return;	
}

/* set QoS template rules by template name */
#define INTERNET	4
#define TR069		1
#define VOIP		2
#define IPTV		3
void setQoSTempRules(mxml_node_t *top, int type)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char QoSNode[MAX_ARG_NUM][MAX_NODE_NAME];
	//char buf[CMD_BUF_LENGTH*2];
	char buf[CMD_BUF_LENGTH];
	char attr_buf[32];
	tcapi_msg_t msg;
#if defined(TCSUPPORT_CT_VOIP_QOS) 
	char qosCmd[MAX_CMD_LENGTH];
#endif
	char *p = NULL;
	unsigned char priority[4] = {0};
	int i = 1;
	int enable_que_num = 0;
	char temp[8] = {0};
	int tr69_index = 0, voip_index = 0;
	
	memset( nodeName, 0, sizeof(nodeName) );
	strcpy( nodeName[0], QOS );
	strcpy( nodeName[1], QOS_COMMON );

	memset( QoSNode, 0, sizeof(QoSNode) );
	strcpy( QoSNode[0], QOS );
	
	memset( buf, 0, sizeof(buf) );
	if ( getAttrValue(top, nodeName, "Mode", buf) == TCAPI_PROCESS_OK ) {
		if (strcmp(appmode,buf) == 0)
			return;	
		if (type == 1) {
			deleteQosTempRules();//when set template qos rules,delete the old rules first
		}
		tcdbg_printf("\nsetQoSTempRules: mode = %s", buf);
#if defined(TCSUPPORT_CT_VOIP_QOS) 
		//clear rtp priority first
		memset( qosCmd, 0, sizeof(qosCmd) );
		sprintf(qosCmd,"echo 0 > %s",RTP_PRIORITY_QUEUE_PATH);
		system(qosCmd);
#endif

		memset(&msg, 0, sizeof(msg));
		strcpy(appmode,buf);
		
		//reset attribute value
		setAttrValue(top, nodeName, "UplinkBandwidth", "0");
		setAttrValue(top, nodeName, "Discipline", "PQ");
		setAttrValue(top, nodeName, "EnableForceWeight", "No");
		setAttrValue(top, nodeName, "EnableDSCP", "No");
		setAttrValue(top, nodeName, "En8021PRemark", "0");
		
		p=strtok(buf, ",");	
		while(p){
			if(strstr(p, "TR069")){
				priority[TR069] = i;
				//tcdbg_printf("\nsetQoSTempRule: TR069 = %d", priority[TR069]);
				i++;
			}
			else if(strstr(p, "VOIP")){
				priority[VOIP] = i;
				//tcdbg_printf("\nsetQoSTempRule: VOIP = %d", priority[VOIP]);
				i++;
			}
			else if(strstr(p, "IPTV")){
				priority[IPTV] = i;
				//tcdbg_printf("\nsetQoSTempRule: IPTV = %d", priority[IPTV]);
				i++;
			}
			p=strtok(NULL, ",");
		}
		
		strcpy(QoSNode[1], "Common");
		/*enable queue(1~ enable_que_num)*/
		for(i = 1; i < 4; i++){
			if(priority[i] != 0){
				enable_que_num++;
				memset(attr_buf, 0, sizeof(attr_buf));
				sprintf(attr_buf, "QueueSW%d", priority[i]);
				setAttrValue(top, QoSNode, attr_buf, "Yes");
			}
		}
		//tcdbg_printf("\nsetQoSTempRule: enable_que_num = %d", enable_que_num);
		/* disable queue (enable_que_num+1 ~ 6) */
		for (i = enable_que_num+1; i < 7; i++) {
			memset(attr_buf, 0, sizeof(attr_buf));
			sprintf(attr_buf, "QueueSW%d", i);
			setAttrValue(top, QoSNode, attr_buf, "No");
		}
		if (type == 1) {
			qosDiscplineRule(top);
		}
		/*clean type and app rule*/
		for(i = 0; i < MAX_APP_TYPE_NUM; i++){
			sprintf(msg.node, "QoS_Entry%d", i);
			tcapi_unset_req(top, &msg);
		}
		/*set  type rule */
		for (i = 0; i < 4; i++) {
			sprintf(QoSNode[1], "Entry%d", i);
			if((priority[IPTV] != 0) && (0 == i)){
				setAttrValue(top, QoSNode, "Active", "Yes");
				setAttrValue(top, QoSNode, "ActDSCPRemarking", "0");
				setAttrValue(top, QoSNode, "Act8021pRemarkingNum", "0");
				setAttrValue(top, QoSNode, "Type1", "LANInterface");
				/*Lan2*/
				setAttrValue(top, QoSNode, "Max1", "2");
				setAttrValue(top, QoSNode, "Min1", "2");
				setAttrValue(top, QoSNode, "ProtocolID", "all");
				sprintf(attr_buf, "%d", priority[IPTV]);
				setAttrValue(top, QoSNode, "ActQueue", attr_buf);
			}
			else{
				setAttrValue(top, QoSNode, "Active", "No");
				setAttrValue(top, QoSNode, "ActDSCPRemarking", "0");
				setAttrValue(top, QoSNode, "Act8021pRemarkingNum", "0");
				sprintf(attr_buf, "%d", i + 1);
				setAttrValue(top, QoSNode, "ActQueue", attr_buf);
			}
			if (type == 1) {
				qosTypeRule(top, i);
			}
		}

		if((priority[TR069] != 0) && (priority[VOIP] != 0)){
			if(priority[TR069] < priority[VOIP]){
				tr69_index = 0;
				voip_index = 1;
			}
			else{
				voip_index = 0;
				tr69_index = 1;
			}
		}
		
		if(priority[TR069] != 0){
			/* tr069 to first queue */
			sprintf(QoSNode[1], "Entry%d", (MAX_TYPE_RULE_NUM+tr69_index));
			setAttrValue(top, QoSNode, "Active", "Yes");
			setAttrValue(top, QoSNode, "AppName", "TR069");
			memset(temp, 0, sizeof(temp));
			sprintf(temp, "%d", priority[TR069]);
			setAttrValue(top, QoSNode, "ActQueue", temp);
			if (type == 1) {
				qosAppRule(top, (MAX_TYPE_RULE_NUM+tr69_index), 0);
			}
		}
		
		if(priority[VOIP] != 0){
			/* voip to second queue */
			sprintf(QoSNode[1], "Entry%d", (MAX_TYPE_RULE_NUM+voip_index));
			setAttrValue(top, QoSNode, "Active", "Yes");
			setAttrValue(top, QoSNode, "AppName", "VOIP");
			setAttrValue(top, QoSNode, "ProtocolID", "rtp");
			memset(temp, 0, sizeof(temp));
			sprintf(temp, "%d", priority[VOIP]);
			setAttrValue(top, QoSNode, "ActQueue", temp);
			if (type == 1) {
				qosAppRule(top, (MAX_TYPE_RULE_NUM+voip_index), 0);
			}
		}
		
	#if 0
		/* dns to first queue */
		sprintf(QoSNode[1], "Entry%d", MAX_TYPE_RULE_NUM + MAX_APP_RULE_NUM);
		setAttrValue(top, QoSNode, "ProtocolID", "udp");
		setAttrValue(top, QoSNode, "DesPortRangeBegin", "53");
		setAttrValue(top, QoSNode, "ActQueue", "1");
		qosTemRule(top, MAX_TYPE_RULE_NUM + MAX_APP_RULE_NUM);

		/* dhcp to first queue */
		sprintf(QoSNode[1], "Entry%d", MAX_TYPE_RULE_NUM + MAX_APP_RULE_NUM + 1);
		setAttrValue(top, QoSNode, "ProtocolID", "udp");
		setAttrValue(top, QoSNode, "DesPortRangeBegin", "67");
		setAttrValue(top, QoSNode, "DesPortRangeEnd", "68");
		setAttrValue(top, QoSNode, "ActQueue", "1");
		qosTemRule(top, MAX_TYPE_RULE_NUM + MAX_APP_RULE_NUM + 1);
	#endif	
	}
	else {
		tcdbg_printf("get mode failed.\n");
	}
}
#endif

int setQoSSwitch( mxml_node_t *top, int sw )
{
	/* 0: qos is on 1: qos is off */
	static int qosFlag = 0;
	int oldQosFlag = -1;
	int i;
#if defined(TCSUPPORT_CT_QOS)
	unsigned int new_filter_state = 0;
#if 0//move outside and execute after ebtable modules insmod 
	if (sw == 1) {
		/* if QoS is from down to up, we set QoS template rules by template name */
		setQoSTempRules( top );
	}
	#endif
#endif
	if ( sw == qosFlag ) {
		return oldQosFlag;
	}
	
	char qosCmd[64];
	memset( qosCmd, 0, sizeof(qosCmd) );
	
	if ( sw == 1 ) {
		/* 0. insmode sch modules */
		/* qosTCModule( INSMODE ); */
		
		/* 1. insmode ebtables modules */
		qosEBTModule( INSMODE );
#if defined(TCSUPPORT_CT_QOS)
		/* 2. insmode iptables modules */
		new_filter_state = pre_sys_state.filter_m_state | QOS_VECTOR;
		pre_sys_state.filter_m_state = check_and_set_filter( new_filter_state );
#endif
		/* 3. create ebtables & iptables chains */
#ifdef IPV6
		strcpy( qosCmd, "qoscmd switch on IPVersion all" );
#else
		strcpy( qosCmd, "qoscmd switch on IPVersion v4" );
#endif
		excQosCmd( qosCmd );
#if defined(TCSUPPORT_CT_PON)
		system("echo pwan qos 1 > /proc/gpon/debug");
#endif
	}
	else if ( sw == 0 ) {
		/* 1. del ebtables & iptables chains */
#ifdef IPV6
		strcpy( qosCmd, "qoscmd switch off IPVersion all" );
#else
		strcpy( qosCmd, "qoscmd switch off IPVersion v4" );
#endif
		excQosCmd( qosCmd );
		
		/* 2. rmmode ebtables modules */
		qosEBTModule( RMMODE );
		
#if defined(TCSUPPORT_CT_QOS)		
		/* 3. rmmode iptables modules */
		new_filter_state = pre_sys_state.filter_m_state & (~QOS_VECTOR);
		pre_sys_state.filter_m_state = check_and_set_filter( new_filter_state );

		/* delete vlan rule */
		for ( i = 0; i < MAX_VLAN_RULE_NUM; i++ ) {
			qosVlanidRule(top, i + MAX_TYPE_RULE_NUM + MAX_TEM_RULE_NUM, 0);
		}
#endif

		/* 4. rmmode sch modules */
		/* qosTCModule( RMMODE ); */
#if defined(TCSUPPORT_CT_PON)
		system("echo pwan qos 0 > /proc/gpon/debug");
#endif

	}
	else {
		return -1;
	}

	/* save the current qos state */
	oldQosFlag = qosFlag;
	qosFlag = sw;
	return oldQosFlag;
}

int qosDiscplineRule( mxml_node_t *top )
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], startupCmd[MAX_CMD_LENGTH];
	char discipline[32], wrr_pri[4][32];
#if defined(TCSUPPORT_CT_QOS)
	char bandWidth[32], car_pri[6][32], sw_pri[6][32], qm[10], forcebw[10];
	int bandWidthFlag = 0;
	char buf[32];

#endif
#if defined(TCSUPPORT_CPU_EN7521)		
	char hwCar[8] = {0};
	char sys_node[MAX_ARG_NUM][MAX_NODE_NAME];
#endif
	unsigned long lbandwidth = 0;

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],QOS);
	strcpy(nodeName[1],QOS_COMMON);
	

#if defined(TCSUPPORT_CT_QOS)
	/* check if there is uplink bandwidth in cfg node */
	if ( getAttrValue(top, nodeName, "UplinkBandwidth", bandWidth) == TCAPI_PROCESS_OK ) {
		if ( strlen(bandWidth) > 0 && strcmp(bandWidth, "0") != 0 ) {
			/* delete the last three characters */
			if (strlen(bandWidth) > 3) {
				sscanf(bandWidth, "%lu", &lbandwidth);
				sprintf(bandWidth, "%lu", (lbandwidth / 1024));
#if !defined(TCSUPPORT_CT_SWQOS)
				/* if the bandwidth is bigger than 950, set it to 950 */
				if (atoi(bandWidth) > MAX_UPLINK_BANDWIDTH) {
					strcpy(bandWidth, MAX_UPLINK_BANDWIDTH_STRING);
				}
#endif
			}
			else {
				strcpy(bandWidth, "1");
			}
			if( getAttrValue(top, nodeName, "Active", buf) == TCAPI_PROCESS_OK){
				if( strcmp(buf, "No") == 0 )
					strcpy(bandWidth, "0");
				}

			bandWidthFlag  = 1;
		}
		else {
#if defined(TCSUPPORT_CT_HWQOS) || defined(TCSUPPORT_CPU_EN7521)		
			strcpy(bandWidth, "0");
#else
			strcpy(bandWidth, MAX_UPLINK_BANDWIDTH_STRING);
#endif
			bandWidthFlag = 1;
		}
	}
	else {
#if defined(TCSUPPORT_CT_HWQOS) || defined(TCSUPPORT_CPU_EN7521)		
		strcpy(bandWidth, "0");
#else
		strcpy(bandWidth, MAX_UPLINK_BANDWIDTH_STRING);
#endif
		bandWidthFlag = 1;
	}

#endif

	memset( startupCmd, 0, sizeof(startupCmd) );
	/* set qos discpline */
	if ( getAttrValue(top, nodeName, "Discipline", discipline) == TCAPI_PROCESS_OK ) {
		if (!strcmp(discipline,"SP") || !strcmp(discipline, "PQ")) {
			/* priority queue */
			strcpy( startupCmd, "qoscmd discpline PQ" );

#if defined(TCSUPPORT_CT_QOS)
			if ( bandWidthFlag == 1 ) {
				sprintf(startupCmd, "%s uplink-bandwidth %s", startupCmd, bandWidth);
			}
			if ( (getAttrValue(top, nodeName, "QueueSW1", sw_pri[0]) == TCAPI_PROCESS_OK) &&
				 (getAttrValue(top, nodeName, "QueueSW2", sw_pri[1]) == TCAPI_PROCESS_OK) &&
				 (getAttrValue(top, nodeName, "QueueSW3", sw_pri[2]) == TCAPI_PROCESS_OK) &&
				 (getAttrValue(top, nodeName, "QueueSW4", sw_pri[3]) == TCAPI_PROCESS_OK) ) {
				setQueueMask( qm, sw_pri, 4 );
				sprintf( startupCmd, "%s queuemask %s", startupCmd, qm );
			}
			else {
				return -1;
			}
#endif
		}
#if defined(TCSUPPORT_CT_QOS)
		else if ( !strcmp(discipline,"WRR") ) {  
			/* WRR */
			if(	(getAttrValue(top, nodeName, /*"WeightHighest"*/"QueueBW1", wrr_pri[0]) == TCAPI_PROCESS_OK) &&
				(getAttrValue(top, nodeName, /*"WeightHigh"*/"QueueBW2", wrr_pri[1]) == TCAPI_PROCESS_OK) &&
				(getAttrValue(top, nodeName, /*"WeightMedium"*/"QueueBW3", wrr_pri[2]) == TCAPI_PROCESS_OK) &&
				(getAttrValue(top, nodeName, /*"WeightLow"*/"QueueBW4", wrr_pri[3]) == TCAPI_PROCESS_OK) ) {
				sprintf( startupCmd, "qoscmd discpline WRR %s %s %s %s", wrr_pri[0], wrr_pri[1], wrr_pri[2], wrr_pri[3] );
				if ( bandWidthFlag == 1 ) {
					sprintf(startupCmd, "%s uplink-bandwidth %s", startupCmd, bandWidth);
				}
				
				if ( (getAttrValue(top, nodeName, "QueueSW1", sw_pri[0]) == TCAPI_PROCESS_OK) &&
				 	 (getAttrValue(top, nodeName, "QueueSW2", sw_pri[1]) == TCAPI_PROCESS_OK) &&
				     (getAttrValue(top, nodeName, "QueueSW3", sw_pri[2]) == TCAPI_PROCESS_OK) &&
				     (getAttrValue(top, nodeName, "QueueSW4", sw_pri[3]) == TCAPI_PROCESS_OK) ) {
				  		setQueueMask( qm, sw_pri, 4 );
						sprintf(startupCmd, "%s queuemask %s", startupCmd, qm);
				}
				else {
					return -1;
				}

				/* check if need force bandwidth */
				if (getAttrValue(top, nodeName, "EnableForceWeight", forcebw) == TCAPI_PROCESS_OK) {
					if (!strcmp(forcebw, "Yes")) {
						strcat(startupCmd, " forcebw");
					}
				}
			}
		}
		else if ( !strcmp(discipline, "CAR") ) {
		   /* CAR */
		   if ( (getAttrValue(top, nodeName, "QueueBW1", car_pri[0]) == TCAPI_PROCESS_OK) &&
		   		(getAttrValue(top, nodeName, "QueueBW2", car_pri[1]) == TCAPI_PROCESS_OK) &&
		   		(getAttrValue(top, nodeName, "QueueBW3", car_pri[2]) == TCAPI_PROCESS_OK) &&
		   		(getAttrValue(top, nodeName, "QueueBW4", car_pri[3]) == TCAPI_PROCESS_OK) &&
		   		(getAttrValue(top, nodeName, "QueueBW5", car_pri[4]) == TCAPI_PROCESS_OK) &&
		   		(getAttrValue(top, nodeName, "QueueBW6", car_pri[5]) == TCAPI_PROCESS_OK) ) {
		   		sprintf( startupCmd, "qoscmd discpline CAR %s %s %s %s %s %s", car_pri[0], car_pri[1], car_pri[2], car_pri[3], car_pri[4], car_pri[5] );

				if ( bandWidthFlag == 1 ) {
					sprintf(startupCmd, "%s uplink-bandwidth %s", startupCmd, bandWidth);
				}
				
				if ( (getAttrValue(top, nodeName, "QueueSW1", sw_pri[0]) == TCAPI_PROCESS_OK) &&
				 	 (getAttrValue(top, nodeName, "QueueSW2", sw_pri[1]) == TCAPI_PROCESS_OK) &&
				 	 (getAttrValue(top, nodeName, "QueueSW3", sw_pri[2]) == TCAPI_PROCESS_OK) &&
					 (getAttrValue(top, nodeName, "QueueSW4", sw_pri[3]) == TCAPI_PROCESS_OK) &&
					 (getAttrValue(top, nodeName, "QueueSW5", sw_pri[4]) == TCAPI_PROCESS_OK) &&
					 (getAttrValue(top, nodeName, "QueueSW6", sw_pri[5]) == TCAPI_PROCESS_OK) ) {
						setQueueMask( qm, sw_pri, 6 );
						sprintf( startupCmd, "%s queuemask %s", startupCmd, qm );
				}
				else {
					return -1;
				}

#if defined(TCSUPPORT_CPU_EN7521)		
				memset(sys_node,0,sizeof(sys_node));
				strcpy(sys_node[0],"Sys");
				strcpy(sys_node[1],"Entry");

				if ( getAttrValue(top, sys_node, "HWCAR", hwCar) == TCAPI_PROCESS_OK) {
					sprintf( startupCmd, "%s %s", startupCmd, hwCar );
				}else{
					sprintf( startupCmd, "%s 1", startupCmd);
				}						
#endif
		   	}
		}

#else
		else {  
			/* WRR */
			if(	(getAttrValue(top, nodeName, "WeightHighest", wrr_pri[0]) == TCAPI_PROCESS_OK) &&
				(getAttrValue(top, nodeName, "WeightHigh", wrr_pri[1]) == TCAPI_PROCESS_OK) &&
				(getAttrValue(top, nodeName, "WeightMedium", wrr_pri[2]) == TCAPI_PROCESS_OK) &&
				(getAttrValue(top, nodeName, "WeightLow", wrr_pri[3]) == TCAPI_PROCESS_OK) ) {
				sprintf( startupCmd, "qoscmd discpline WRR %s %s %s %s", wrr_pri[0], wrr_pri[1], wrr_pri[2], wrr_pri[3] );
			}
		}	
#endif

		excQosCmd( startupCmd );
	}
	else {
		return -1;
	}

	return 0;
}


#if defined(TCSUPPORT_CT_QOS)
char g_lanif[][16] = 
{
#ifdef TCSUPPORT_CT_2PWIFI
	"eth0.1", "eth0.2", "", "",
#else
	"eth0.1", "eth0.2", "eth0.3", "eth0.4", 
#endif
     "ra0", "ra1", "ra2", ""
#if defined(TCSUPPORT_WLAN_AC)
	,"rai0", "rai1", "rai2", "rai3"
#endif
};
#endif

#if defined(TCSUPPORT_CT_QOS)
void setWanIfQoS(mxml_node_t * top, char *qosCmd, char *MinVal, char *MaxVal
, int getWanName)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char ifBuf[64], ifName[10];
	int i,j, pvc_idx_begin = -1, if_idx_begin = -1, pvc_idx_end = -1, if_idx_end = -1;
	char pvc_arr[10], if_arr[10];
	char *p = NULL;
	
	if (qosCmd == NULL || MinVal == NULL || MaxVal == NULL) {
		return;
	}

	//tcdbg_printf("MinVal is %s, MaxVal is %s\n", MinVal, MaxVal);
	
	memset(pvc_arr, 0, sizeof(pvc_arr));
	memset(if_arr, 0, sizeof(if_arr));
	if ('\0' != MinVal[0]) {
		p = strstr(MinVal, ",");
		if (NULL != p) {
			strncpy(pvc_arr, MinVal, p - MinVal);
			strcpy(if_arr, p + 1);
			//tcdbg_printf("pvc_arr is %s, if_arr is %s\n", pvc_arr, if_arr);
		}

		pvc_idx_end = pvc_idx_begin = atoi(pvc_arr);
		if_idx_end = if_idx_begin = atoi(if_arr);

		/* end pvc and inteface index */
		if ('\0' != MaxVal[0]) {
			p = strstr(MaxVal, ",");
			if (NULL != p) {
				strncpy(pvc_arr, MaxVal, p - MaxVal);
				strcpy(if_arr, p + 1);
				//tcdbg_printf("pvc_arr is %s, if_arr is %s\n", pvc_arr, if_arr);
			}

			pvc_idx_end = atoi(pvc_arr);
			if_idx_end = atoi(if_arr);
		}
	}

//	tcdbg_printf("pvc_idx_begin is %d, pvc_idx_end is %d\n", pvc_idx_begin, pvc_idx_end);
//	tcdbg_printf("if_idx_begin is %d, if_idx_end is %d\n", if_idx_begin, if_idx_end);
	
	/* get interface from wan node */
	memset(ifBuf, 0, sizeof(ifBuf));
	if (pvc_idx_begin > 0 && if_idx_begin > 0 && pvc_idx_end > 0 && if_idx_end > 0) {
		for (i = pvc_idx_begin; i <= pvc_idx_end; i++) {
			for (j = if_idx_begin; j <= if_idx_end; j++) {
				strcpy(nodeName[0], "Wan");
				sprintf(nodeName[1], "PVC%d", i - 1);
				sprintf(nodeName[2], "Entry%d", j - 1);
				if (getAttrValue(top, nodeName, "IFName", ifName) == TCAPI_PROCESS_OK) {
					//tcdbg_printf("if name is %s\n", ifName);
					
					if ('\0' != ifName[0]) {
						if ('\0' == ifBuf[0]) {
							strcpy(ifBuf, ifName);
						}
						else {
							/* if buf can not be over then 64 */
							if (strlen(ifBuf) < 48) {
								sprintf(ifBuf, "%s,%s", ifBuf, ifName);
							}
						}

					}
				}
			}
		}
	}

	
	if ('\0' != ifBuf[0]) {
		if ( getWanName )
			strcpy(qosCmd, ifBuf);
		else
			sprintf(qosCmd, "%s WanIf %s", qosCmd, ifBuf);
	}

	return;
}

int ioctl_qostype(int cmd, void *data, int datealen)
{
	int fp = -1;
	int ret = -1;	
	fp = open("/dev/qostype", O_RDONLY);
	if (fp == -1) {
		tcdbg_printf("ioctl_qostype: fp is [%d]\n", fp);
	}
	ret = ioctl(fp, cmd, data);
	if (ret < 0) {
		tcdbg_printf("\r\nioctl_qostype failed!");
	}

	close(fp);
	return 0;
}

int qosWanInterfaceUpdate(mxml_node_t *top, int on_off)
{
	int idx = 0, i = 0;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char buf[CMD_BUF_LENGTH] = {0};
	char typeName[20] = {0}, typeVal[20] = {0};
	char MaxName[10] = {0}, MinName[10] = {0}, MaxVal[64] = {0}, MinVal[64] = {0};
	unsigned char wantype_set[MAX_TYPE_RULE_NUM] = {0};
	char wantype_name[MAX_TYPE_RULE_NUM][32] = {0};
	struct qos_type qt = {0};

	/* reset all entry */
	bzero(&qt, sizeof(struct qos_type));
	for ( idx = 0; idx < MAX_TYPE_RULE_NUM; idx++ )
	{
		ioctl_qostype(QOSTYPE_IOC_SET_TYPE, &qt, sizeof(qt));
	}

	if ( 0 == on_off )
		return 0;

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], QOS);

	bzero(wantype_set, sizeof(wantype_set));
	bzero(wantype_name, sizeof(wantype_name));
	for ( idx = 0; idx < MAX_TYPE_RULE_NUM; idx++ )
	{
		sprintf(nodeName[1], "Entry%d", idx);
		bzero(buf, sizeof(buf));
		if ( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, "ActQueue", buf)
			&& 0 != buf[0] )
		{
			for ( i = 0; i < MAX_TYPE_NUM; i++ )
			{
				sprintf(typeName, "Type%d", i + 1);
				sprintf(MaxName, "Max%d", i + 1);
				sprintf(MinName, "Min%d", i + 1);
				bzero(MaxVal, sizeof(MaxVal));
				bzero(MinVal, sizeof(MinVal));
				if ( TCAPI_PROCESS_OK ==
					getAttrValue(top, nodeName, typeName, typeVal)
					&& TCAPI_PROCESS_OK ==
					getAttrValue(top, nodeName, MaxName, MaxVal)
					&& TCAPI_PROCESS_OK ==
					getAttrValue(top, nodeName, MinName, MinVal) )
				{
					if (!strcmp(typeVal, "N/A") || !strcmp(MaxVal, "N/A")
						|| !strcmp(MinVal, "N/A"))
						continue;

					if ( !strcmp(typeVal, "WANInterface") )
					{
						/* wan interface */
						setWanIfQoS(top, wantype_name[idx], MinVal, MaxVal, 1);
						wantype_set[idx] = atoi(buf);
					}
				}
			}
		}
	}

	/* set wan interface */
	for ( idx = 0; idx < MAX_TYPE_RULE_NUM; idx++ )
	{
		if ( 0 != wantype_set[idx] )
		{
			bzero(&qt, sizeof(struct qos_type));
			qt.rule_no = wantype_set[idx];
			strcpy(qt.qm[0].type_name, "wan_if");
			strcpy(qt.qm[0].type_value, wantype_name[idx]);
			ioctl_qostype(QOSTYPE_IOC_SET_TYPE, &qt, sizeof(qt));
		}
	}

	return 0;
}


int qosTypeRule(mxml_node_t *top, int index)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char qosCommon[MAX_ARG_NUM][MAX_NODE_NAME];
	char typeName[20], typeVal[20];
	char MaxName[10], MinName[10], MaxVal[64], MinVal[64];
	char qosCmd[MAX_CMD_LENGTH*2], qosCmdBuf[MAX_CMD_LENGTH], buf[CMD_BUF_LENGTH];
	char queueBuf[10], dotp[10], dscpEnable[10];
	int i, j, min, max, queue;

	//tcdbg_printf("index is %d\n", index);
	
	if (index < 0 || index > MAX_TYPE_RULE_NUM + MAX_APP_RULE_NUM - 1) {
		return -1;
	}

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], QOS);
	sprintf(nodeName[1], "Entry%d", index);
	
	memset(qosCmd, 0, sizeof(qosCmd));
	memset(qosCmdBuf, 0, sizeof(qosCmdBuf));
	memset(buf, 0, sizeof(buf));
	/* 1. have or no this entry */
	if (getAttrValue(top, nodeName, "ActQueue", buf) == TCAPI_PROCESS_OK) {
		if (strlen(buf) > 0) {
			//tcdbg_printf("buf is %s", buf);
			#ifdef IPV6
			sprintf(qosCmd, "qoscmd type add -i %d IPVersion all", index);
			#else
			sprintf(qosCmd, "qoscmd type add -i %d IPVersion v4", index);
			#endif
		}
		else {
			goto DelQosTypeRule;
		}
	}
	else {
		goto DelQosTypeRule;
	}
	
	/* 1. match  */
	for (i = 0; i < MAX_TYPE_NUM; i++) {
		sprintf(typeName, "Type%d", i + 1);
		sprintf(MaxName, "Max%d", i + 1);
		sprintf(MinName, "Min%d", i + 1);
		memset(MaxVal, 0, sizeof(MaxVal));
		memset(MinVal, 0, sizeof(MinVal));
		if ((getAttrValue(top, nodeName, typeName, typeVal) == TCAPI_PROCESS_OK) &&
			(getAttrValue(top, nodeName, MaxName, MaxVal) == TCAPI_PROCESS_OK) &&
			(getAttrValue(top, nodeName, MinName, MinVal) == TCAPI_PROCESS_OK)){
			/* this type may be has been deleted */
			if (!strcmp(typeVal, "N/A") || !strcmp(MaxVal, "N/A") || !strcmp(MinVal, "N/A")) {
				continue;
			}
			
			if (!strcmp(typeVal, "SMAC")) {
				/* source mac address match */
				if (!strcmp(MaxVal, MinVal) && '\0' != MaxVal[0]) {
					sprintf(qosCmd, "%s smac %s", qosCmd, MaxVal);
				}
			}
			else if (!strcmp(typeVal, "8021P")) {
				/* 802.1p match */
				if ('\0' != MinVal[0]) {
					strcpy(qosCmdBuf, MinVal);
					if ('\0' != MaxVal[0]) {
						sprintf(qosCmdBuf, "%s:%s", MinVal, MaxVal);
					}
					sprintf(qosCmd, "%s dot1p %s", qosCmd, qosCmdBuf);
				}
			}
			else if (!strcmp(typeVal, "SIP")) {
				/* source ip address match */
				if ('\0' != MinVal[0]) {
					strcpy(qosCmdBuf, MinVal);
					if ('\0' != MaxVal[0] && strcmp(MinVal, MaxVal) != 0) {
						sprintf(qosCmdBuf, "%s-%s", MinVal, MaxVal);
					}
					sprintf(qosCmd, "%s sip %s", qosCmd, qosCmdBuf);
				}
			}
			else if (!strcmp(typeVal, "DIP")) {
				/* dst ip address match */
				if ('\0' != MinVal[0]) {
					strcpy(qosCmdBuf, MinVal);
					if ('\0' != MaxVal[0] && strcmp(MinVal, MaxVal) != 0) {
						sprintf(qosCmdBuf, "%s-%s", MinVal, MaxVal);
					}
					sprintf(qosCmd, "%s dip %s", qosCmd, qosCmdBuf);
				}
			}
			else if (!strcmp(typeVal, "SPORT")) {
				/* source port match */
				if ('\0' != MinVal[0]) {
					strcpy(qosCmdBuf, MinVal);
					if ('\0' != MaxVal[0]) {
						sprintf(qosCmdBuf, "%s:%s", MinVal, MaxVal);
					}
					sprintf(qosCmd, "%s sport %s", qosCmd, qosCmdBuf);
				}
			}
			else if (!strcmp(typeVal, "DPORT")) {
				/* dst port match */
				if ('\0' != MinVal[0]) {
					strcpy(qosCmdBuf, MinVal);
					if ('\0' != MaxVal[0]) {
						sprintf(qosCmdBuf, "%s:%s", MinVal, MaxVal);
					}
					sprintf(qosCmd, "%s dport %s", qosCmd, qosCmdBuf);
				}
			}
			else if (!strcmp(typeVal, "DSCP")) {
				/* dscp match */
				if ('\0' != MinVal[0]) {
					strcpy(qosCmdBuf, MinVal);
					if ('\0' != MaxVal[0]) {
						sprintf(qosCmdBuf, "%s:%s", MinVal, MaxVal);
					}
					sprintf(qosCmd, "%s dscpORTC %s", qosCmd, qosCmdBuf);
				}
			}
			else if (!strcmp(typeVal, "LANInterface")) {
				/* lan interface */
				memset(qosCmdBuf, 0, sizeof(qosCmdBuf));
				if ('\0' != MinVal[0]) {
					max = min = atoi(MinVal);
					if ('\0' != MaxVal[0]) {
						max = atoi(MaxVal);
					}

					for (j = min; j <= max; j++) {
						if (strlen(g_lanif[j - 1]) > 0) {
							sprintf(qosCmdBuf, "%s,%s", qosCmdBuf, g_lanif[j - 1]);
						}
					}

					if ('\0' != qosCmdBuf[0]) {
						sprintf(qosCmd, "%s LanIf %s", qosCmd, qosCmdBuf);
					}
				}
			}
			else if (!strcmp(typeVal, "WANInterface")) {
				/* wan interface */
				setWanIfQoS(top, qosCmd, MinVal, MaxVal, 0);
			}
#if defined(TCSUPPORT_CUC_QOS)
			else if (!strcmp(typeVal, "EtherType")) {				
				if ('\0' != MinVal[0]) {
					if(strcmp(MinVal,"ARP")== 0){
						strcpy(qosCmdBuf, "0x0806");
					}else if(strcmp(MinVal,"IPv4")== 0){
						strcpy(qosCmdBuf, "0x0800");
					}else if(strcmp(MinVal,"IPv6")== 0){
						strcpy(qosCmdBuf, "0x86dd");
					}else{
						strcpy(qosCmdBuf, MinVal);
					}					
					sprintf(qosCmd, "%s EtherType %s", qosCmd, qosCmdBuf);
				}				
			}
#endif
			memset(buf, 0, sizeof(buf));
			/* get proto from QoS node */
			if (getAttrValue(top, nodeName, "ProtocolID", buf) == TCAPI_PROCESS_OK) {
				if ('\0' != buf[0]) {
					sprintf(qosCmd, "%s proto %s", qosCmd, buf);
				}
			}
		}

		
	}

	tcdbg_printf("qosTypeRule1:qosCmd=%s",qosCmd);
	/* excute "qoscmd type" rule */
	excQosCmd(qosCmd);
	qosWanInterfaceUpdate(top, 1);

	/* 2. remark queue and 8021p etc. */
	#ifdef IPV6
	sprintf(qosCmd, "qoscmd remark add -i %d IPVersion all", index);
	#else
	sprintf(qosCmd, "qoscmd remark add -i %d IPVersion v4", index);
	#endif

	/* queue priority */
	if (getAttrValue(top, nodeName, "ActQueue", buf) == TCAPI_PROCESS_OK) {
		if ('\0' != buf[0]) {
			queue = atoi(buf);
		}
		else {
			return -1;
		}

		sprintf(queueBuf, "QueueP%d", queue);
		/* get queue from QoS Common node */
		memset(qosCommon, 0, sizeof(qosCommon));
		strcpy(qosCommon[0], "QoS");
		strcpy(qosCommon[1], "Common");
		if (getAttrValue(top, qosCommon, queueBuf, buf) == TCAPI_PROCESS_OK) {
			if (atoi(buf) > 0 && atoi(buf) < 7) {
				sprintf(qosCmd, "%s queue %s", qosCmd, buf);
			}
			else {
				return -1;
			}
		}
	}

	/* 802.1p remark */
	if (getAttrValue(top, nodeName, "Act8021pRemarkingNum", buf) == TCAPI_PROCESS_OK) {
		if ('\0' != buf[0]) {
			if (getAttrValue(top, qosCommon, "En8021PRemark", dotp) == TCAPI_PROCESS_OK) {
				if (!strcmp(dotp, "0")) {
					/* zero mark */
					sprintf(qosCmd, "%s dot1p 8", qosCmd);
				}
				else if (!strcmp(dotp, "1")) {
					/* pass through */
					sprintf(qosCmd, "%s dot1p 9", qosCmd);
				}
				else {
					/* remark */
					sprintf(qosCmd, "%s dot1p %s", qosCmd, buf);
				}
			}
		}
	}

	/* dscp or tc remark */
	if ( getAttrValue(top, nodeName, "ActDSCPRemarking", buf) == TCAPI_PROCESS_OK ) {
		if (getAttrValue(top, qosCommon, "EnableDSCPMark", dscpEnable) == TCAPI_PROCESS_OK) {
			if (!strcmp(dscpEnable, "Yes")) {
				if ('\0' != buf[0]) {
					sprintf( qosCmd, "%s dscpORTC %s", qosCmd, buf );
				}
			}
		}
		
	}
	
	tcdbg_printf("qosTypeRule2:qosCmd=%s",qosCmd);
	/* excute "qoscmd remark" rule */
	excQosCmd(qosCmd);
	return 0;
	
DelQosTypeRule:
	sprintf(qosCmd, "qoscmd type del -i %d", index);
	excQosCmd(qosCmd);
	sprintf(qosCmd, "qoscmd remark del -i %d", index);
	excQosCmd(qosCmd);
	return 0;
}
#else
int qosTypeRule( mxml_node_t *top, int index )
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char wanNode[MAX_ARG_NUM][MAX_NODE_NAME];
	char qosCmd[MAX_CMD_LENGTH*2], qosCmdBuf[MAX_CMD_LENGTH], buf[CMD_BUF_LENGTH];
	int i;
	
	if ( index < 0 || index > MAX_TYPE_RULE_NUM - 1 ) {
		return -1;
	}

	memset( nodeName, 0, sizeof(nodeName) );
	strcpy( nodeName[0], QOS );
	sprintf( nodeName[1], "Entry%d", index );
	
	memset( qosCmd, 0, sizeof(qosCmd) );
	memset( qosCmdBuf, 0, sizeof(qosCmdBuf) );

	/* 1. active or deactive */
	if ( getAttrValue(top, nodeName, "Active", buf) == TCAPI_PROCESS_OK ) {
		if ( strcmp(buf, "Yes") == 0 ) {
		#ifdef IPV6
			sprintf( qosCmd, "qoscmd type add -i %d IPVersion all", index );
		#else
			sprintf( qosCmd, "qoscmd type add -i %d IPVersion v4", index );
		#endif
		}
		else {
			goto DelQosTypeRule;
		}
	}
	else {
		goto DelQosTypeRule;
	}
	
#if 0//def TCSUPPORT_E8B
	/* 2. lan interface */
	memset( qosCmdBuf, 0, sizeof(qosCmdBuf) );
	char lanif[][16] = 
	{
		{"lanIfIndexBegin"}, {"lanIfIndexEnd"}, {""}
	};
	int if_begin = 0, if_end = 0;
	
	if (getAttrValue(top, nodeName, lanif[0], buf) == TCAPI_PROCESS_OK) {
		if_begin = atoi(buf);
	}

	if (getAttrValue(top, nodeName, lanif[1], buf) == TCAPI_PROCESS_OK) {
		if_end = atoi(buf);
	}

	if (if_begin != 0 && if_end != 0 && if_end >= if_begin) {
		for (i = if_begin; i <= if_end; i++) {
			if ( strlen(qosCmdBuf) == 0 ) {
				strcpy( qosCmdBuf, g_lanif[i] );
			}
			else {
				sprintf( qosCmdBuf, "%s,%s", qosCmdBuf, g_lanif[i] );
			}
		}
	}

	if ( strlen(qosCmdBuf) > 0 ) {
		sprintf( qosCmd, "%s LanIf %s", qosCmd, qosCmdBuf );
	}
	
#else
	/* 2. lan interface */
	memset( qosCmdBuf, 0, sizeof(qosCmdBuf) );
	char lanif[][8] =
	{
#ifdef TCSUPPORT_TC2031
		{"eth0"},
#else
		{"eth0.1"},  {"eth0.2"},  {"eth0.3"},  {"eth0.4"},	
#endif
		{"ra0"},  {"ra1"},  {"ra2"},  {"ra3"},	
		{"usb0"}, {""}
	};
	for ( i = 0; strlen(lanif[i]) != 0; i++ ) {
		if ( getAttrValue( top, nodeName, lanif[i], buf ) == TCAPI_PROCESS_OK) {
			if ( !strcmp(buf, "Yes") ) {
				if ( strlen(qosCmdBuf) == 0 ) {
					strcpy( qosCmdBuf, lanif[i] );
				}
				else {
					sprintf( qosCmdBuf, "%s,%s", qosCmdBuf, lanif[i] );
				}
			}
		}
	}
	if ( strlen(qosCmdBuf) > 0 ) {
		sprintf( qosCmd, "%s LanIf %s", qosCmd, qosCmdBuf );
	}

#endif

#if 0 //def TCSUPPORT_E8B
	/* 3. wan interface */
	memset(qosCmdBuf, 0, sizeof(qosCmdBuf));
	int pvc_begin = 0, pvc_end = 0, j;
	if_begin = 0, if_end = 0;
	if (getAttrValue(top, nodeName, "pvcIndexBegin", buf) == TCAPI_PROCESS_OK) {
		pvc_begin = atoi(buf);
	}
	if (getAttrValue(top, nodeName, "wanIfIndexBegin", buf) == TCAPI_PROCESS_OK) {
		if_begin = atoi(buf);
	}
	if (getAttrValue(top, nodeName, "pvcIndexEnd", buf) == TCAPI_PROCESS_OK) {
		pvc_end = atoi(buf);
	}
	if (getAttrValue(top, nodeName, "pvcIndexEnd", buf) == TCAPI_PROCESS_OK) {
		if_end = atoi(buf);
	}

	memset(wanNode, 0, sizeof(wanNode));
	if (pvc_end > pvc_begin || (pvc_begin == pvc_end && if_end >= if_begin)) {
		for (i = pvc_begin - 1; i <= pvc_end - 1; i++) {
			for (j = if_begin - 1; j <= if_end - 1; j++) {
				strcpy(nodeName[0], "Wan");
				sprintf(nodeName[1], "PVC%d", i);
				sprintf(nodeName[2], "Entry%d", j);
				if (getAttrValue(top, wanNode, "IFName", buf) == TCAPI_PROCESS_OK) {
					if ( strlen(qosCmdBuf) == 0 ) {
						strcpy(qosCmdBuf, buf);
					}
					else {
						sprintf(qosCmdBuf, "%s,%s", qosCmdBuf, buf);
					}
				}
			}
		}
	}

	if ( strlen(qosCmdBuf) > 0 ) {
		sprintf( qosCmd, "%s WanIf %s", qosCmd, qosCmdBuf );
	}

#endif

	/* 4. proto */
	memset( qosCmdBuf, 0, sizeof(qosCmdBuf) );
	char protoid[32] = {"ProtocolID"};
	if ( getAttrValue(top, nodeName, protoid, buf) == TCAPI_PROCESS_OK ) {
		if ( strlen(buf) > 0 ) {
			sprintf( qosCmd, "%s proto %s", qosCmd, buf );
		}
	}
#if 0//def TCSUPPORT_E8B	
	/* 5. sip */
	memset( qosCmdBuf, 0, sizeof(qosCmdBuf) );
	char sip[][32] =
	{
		{"SrcIPRangeBegin"}, {"SrcMaskRangeBegin"},
		{"SrcIPRangeEnd"}, 	 {"SrcMaskRangeEnd"},	
		{""}
	};
	if ( getAttrValue(top, nodeName, sip[0], buf) == TCAPI_PROCESS_OK ) {
		if ( strlen(buf) > 0 ) {
			/* souce ip begin */
			strcpy( qosCmdBuf, buf );

			/* if there is mask of source ip begin */
			if ( getAttrValue(top, nodeName, sip[1], buf) == TCAPI_PROCESS_OK ) {
				if ( strlen(buf) > 0 ) {
					sprintf( qosCmdBuf, "%s/%s", qosCmdBuf, buf );
				}
			}

			/* check if there is source ip end */
			if ( getAttrValue(top, nodeName, sip[2], buf) == TCAPI_PROCESS_OK ) {
				if ( strlen(buf) > 0 ) {
					sprintf( qosCmdBuf, "%s-%s", qosCmdBuf, buf );

					/* if there is mask of source ip end */
					if ( getAttrValue(top, nodeName, sip[3], buf) == TCAPI_PROCESS_OK ) {
						if ( strlen(buf) > 0 ) {
							sprintf( qosCmdBuf, "%s/%s", qosCmdBuf, buf );
						}
					}
				}
			}
		}
	}
	if ( strlen(qosCmdBuf) > 0 ) {
		sprintf( qosCmd, "%s sip %s", qosCmd, qosCmdBuf );
	}

	/* 6. dip */
	memset( qosCmdBuf, 0, sizeof(qosCmdBuf) );
	char dip[][32] =
	{
		{"DesIPRangeBegin"}, {"DesMaskRangeBegin"},
		{"DesIPRangeEnd"}, 	 {"DesMaskRangeEnd"},	
		{""}
	};
	if ( getAttrValue(top, nodeName, dip[0], buf) == TCAPI_PROCESS_OK ) {
		if ( strlen(buf) > 0 ) {
			/* dst ip begin */
			strcpy( qosCmdBuf, buf );

			/* if there is mask of dst ip begin */
			if ( getAttrValue(top, nodeName, dip[1], buf) == TCAPI_PROCESS_OK ) {
				if ( strlen(buf) > 0 ) {
					sprintf( qosCmdBuf, "%s/%s", qosCmdBuf, buf );
				}
			}

			/* check if there is dst ip end */
			if ( getAttrValue(top, nodeName, dip[2], buf) == TCAPI_PROCESS_OK ) {
				if ( strlen(buf) > 0 ) {
					sprintf( qosCmdBuf, "%s-%s", qosCmdBuf, buf );

					/* if there is mask of dst ip end */
					if ( getAttrValue(top, nodeName, dip[3], buf) == TCAPI_PROCESS_OK ) {
						sprintf( qosCmdBuf, "%s/%s", qosCmdBuf, buf );
					}
				}
			}
		}
	}
	if ( strlen(qosCmdBuf) > 0 ) {
		sprintf( qosCmd, "%s dip %s", qosCmd, qosCmdBuf );
	}
#else
	/* 5. sip */
	memset( qosCmdBuf, 0, sizeof(qosCmdBuf) );
	char sip[][16] = 
	{
		{"SrcIP"}, {"SrcMask"},
	};
	if ( getAttrValue(top, nodeName, sip[0], buf) == TCAPI_PROCESS_OK ) {
		if ( strlen(buf) > 0 ) {
			if ( check_ip_format(buf) == 0 ) {
				return -1;
			}
			/* src ip */
			strcpy( qosCmdBuf, buf );

			/* if there is mask of src ip */
			if ( getAttrValue(top, nodeName, sip[1], buf) == TCAPI_PROCESS_OK ) {
				if ( strlen(buf) > 0 ) {
					sprintf( qosCmdBuf, "%s/%s", qosCmdBuf, buf );
				}
			}
		}
	}
	if ( strlen(qosCmdBuf) > 0 ) {
		sprintf( qosCmd, "%s sip %s", qosCmd, qosCmdBuf );
	}

	/* 6. dip */
	memset( qosCmdBuf, 0, sizeof(qosCmdBuf) );
	char dip[][16] = 
	{
		{"DesIP"}, {"DesMask"},
	};
	if ( getAttrValue(top, nodeName, dip[0], buf) == TCAPI_PROCESS_OK ) {
		if ( strlen(buf) > 0 ) {
			if ( check_ip_format(buf) == 0 ) {
				return -1;
			}
			/* dst ip  */
			strcpy( qosCmdBuf, buf );

			/* if there is mask of dst ip */
			if ( getAttrValue(top, nodeName, dip[1], buf) == TCAPI_PROCESS_OK ) {
				//tcdbg_printf( "qosCmdBuf is %s\n", qosCmdBuf );
				//tcdbg_printf( "buf2222 is %s\n", buf );
				if ( strlen(buf) > 0 ) {
					sprintf( qosCmdBuf, "%s/%s", qosCmdBuf, buf );
				}
			}
		}
	}
	if ( strlen(qosCmdBuf) > 0 ) {
		sprintf( qosCmd, "%s dip %s", qosCmd, qosCmdBuf );
	}
#endif

	/* 7. sport */
	memset( qosCmdBuf, 0, sizeof(qosCmdBuf) );
	char sport[][32] = 
	{
		{"SrcPortRangeBegin"}, {"SrcPortRangeEnd"}
	};
	if ( getAttrValue(top, nodeName, sport[0], buf) == TCAPI_PROCESS_OK ) {
		if ( strlen(buf) > 0 ) {
			strcpy( qosCmdBuf, buf );

			/* check if there src port end */
			if ( getAttrValue(top, nodeName, sport[1], buf) == TCAPI_PROCESS_OK ) {
				if ( strlen(buf) > 0 ) {
					sprintf( qosCmdBuf, "%s:%s", qosCmdBuf, buf );
				}
			}
		}
	}
	if ( strlen(qosCmdBuf) > 0 ) {
		sprintf( qosCmd, "%s sport %s", qosCmd, qosCmdBuf ) ;
	}

	/* 8. dport */
	memset( qosCmdBuf, 0, sizeof(qosCmdBuf) );
	char dport[][32] = 
	{
		{"DesPortRangeBegin"}, {"DesPortRangeEnd"}
	};
	if ( getAttrValue(top, nodeName, dport[0], buf) == TCAPI_PROCESS_OK ) {
		if ( strlen(buf) > 0 ) {
			strcpy( qosCmdBuf, buf );

			/* check if there src port end */
			if ( getAttrValue(top, nodeName, dport[1], buf) == TCAPI_PROCESS_OK ) {
				if ( strlen(buf) > 0 ) {
					sprintf( qosCmdBuf, "%s:%s", qosCmdBuf, buf );
				}
			}
		}
	}
	if ( strlen(qosCmdBuf) > 0 ) {
		sprintf( qosCmd, "%s dport %s", qosCmd, qosCmdBuf ) ;
	}

	/* 9. souce mac addr */
	memset( qosCmdBuf, 0, sizeof(qosCmdBuf) );
	char smac[8] = {"SrcMac"};
	if ( getAttrValue(top, nodeName, smac, buf) == TCAPI_PROCESS_OK ) {
		if ( strlen(buf) > 0 ) {
			sprintf( qosCmd, "%s smac %s", qosCmd, buf );
		}
	}

	/* 10. dest mac addr */
	memset( qosCmdBuf, 0, sizeof(qosCmdBuf) );
	char dmac[8] = {"DesMac"};
	if ( getAttrValue(top, nodeName, dmac, buf) == TCAPI_PROCESS_OK ) {
		if ( strlen(buf) > 0 ) {
			sprintf( qosCmd, "%s dmac %s", qosCmd, buf );
		}
	}

	
#if 0//def TCSUPPORT_E8B
	/* 11. dscpORTC */
	memset( qosCmdBuf, 0, sizeof(qosCmdBuf) );
	char dscpORTC[][32] = 
	{
		{"DSCPRangeBegin"}, {"DSCPRangeEnd"}
	};
	if ( getAttrValue(top, nodeName, dscpORTC[0], buf) == TCAPI_PROCESS_OK ) {
		if ( strlen(buf) > 0 ) {
			strcpy( qosCmdBuf, buf );
			if ( getAttrValue(top, nodeName, dscpORTC[1], buf) == TCAPI_PROCESS_OK ) {
				if ( strlen(buf) > 0 ) {
					sprintf( qosCmdBuf, "%s:%s", qosCmdBuf, buf );
				}
			}
		}
	}
	if ( strlen(qosCmdBuf) > 0 ) {
		sprintf( qosCmd, "%s dscpORTC %s", qosCmd, qosCmdBuf );
	}
#else
	/* 11. ipp/tos/dscp */
	char ippds[32] = {"IPPDSField"};
	if ( getAttrValue(top, nodeName, ippds, buf) == TCAPI_PROCESS_OK ) {
		if ( !strcmp(buf, "IPPTOS") ) {
			/* ipp */
			char ipp[][32] = 
			{
				{"IPPRangeBegin"}, {"IPPRangeEnd"}
			};
			memset( qosCmdBuf, 0, sizeof(qosCmdBuf) );
			if ( getAttrValue(top, nodeName, ipp[0], buf) == TCAPI_PROCESS_OK ) {
				if ( strlen(buf) > 0 ) {
					strcpy( qosCmdBuf, buf );
					if ( getAttrValue(top, nodeName, ipp[1], buf) == TCAPI_PROCESS_OK ) {
						if ( strlen(buf) > 0 ) {
							sprintf( qosCmdBuf, "%s:%s", qosCmdBuf, buf );
						}	
					}
				}
			}
			if ( strlen(qosCmdBuf) > 0 ) {
				sprintf( qosCmd, "%s ipp %s", qosCmd, qosCmdBuf );
			}

			/* tos */
			char tos[32] = {"TypeOfService"};
			memset( qosCmdBuf, 0, sizeof(qosCmdBuf) );
			if ( getAttrValue(top, nodeName, tos, buf) == TCAPI_PROCESS_OK ) {
				if ( strlen(buf) > 0 ) {
					if ( !strcmp(buf, "Minimize monetary cost") ) {
						sprintf( qosCmd, "%s tos 0x02", qosCmd );
					}
					else if ( !strcmp(buf, "Maximize reliability") ) {
						sprintf( qosCmd, "%s tos 0x04", qosCmd );
					}
					else if ( !strcmp(buf, "Maximize throughput") ) {
						sprintf( qosCmd, "%s tos 0x08", qosCmd );
					}
					else if ( !strcmp(buf, "Minimize delay") ) {
						sprintf( qosCmd, "%s tos 0x10", qosCmd );
					}
					else {
						sprintf( qosCmd, "%s tos 0x00", qosCmd );
					}
				}
			}
		}
		else if ( !strcmp(buf, "DSCP") ) {
			/* dscp */
			memset( qosCmdBuf, 0, sizeof(qosCmdBuf) );
			char dscpORTC[][32] = 
			{
				{"DSCPRangeBegin"}, {"DSCPRangeEnd"}
			};
			if ( getAttrValue(top, nodeName, dscpORTC[0], buf) == TCAPI_PROCESS_OK ) {
				if ( strlen(buf) > 0 ) {
					strcpy( qosCmdBuf, buf );
					if ( getAttrValue(top, nodeName, dscpORTC[1], buf) == TCAPI_PROCESS_OK ) {
						if ( strlen(buf) > 0 ) {
							sprintf( qosCmdBuf, "%s:%s", qosCmdBuf, buf );
						}
					}
				}
			}
			if ( strlen(qosCmdBuf) > 0 ) {
				sprintf( qosCmd, "%s dscpORTC %s", qosCmd, qosCmdBuf );
			}
		}
	}
#endif

	/* 12. 802.1p */
	memset( qosCmdBuf, 0, sizeof(qosCmdBuf) );
	char dot1p[][16] =
	{
		{"dotpBegin"}, {"dotpEnd"}
	};
	if ( getAttrValue(top, nodeName, dot1p[0], buf) == TCAPI_PROCESS_OK ) {
		if ( strlen(buf) > 0 ) {
			strcpy( qosCmdBuf, buf );
			if ( getAttrValue(top, nodeName, dot1p[1], buf) == TCAPI_PROCESS_OK ) {
				if ( strlen(buf) > 0 ) {
					sprintf( qosCmdBuf, "%s:%s", qosCmdBuf, buf ); 
				}
			}
		}
	}
	if ( strlen(qosCmdBuf) > 0 ) {
		sprintf( qosCmd, "%s dot1p %s", qosCmd, qosCmdBuf );
	}

	/* 13. vlan id */
	memset( qosCmdBuf, 0, sizeof(qosCmdBuf) );
	char vlanid[][32] =
	{
		{"VlanIDRangeBegin"}, {"VlanIDRangeEnd"}
	};
	if ( getAttrValue(top, nodeName, vlanid[0], buf) == TCAPI_PROCESS_OK ) {
		if ( strlen(buf) > 0 ) {
			strcpy( qosCmdBuf, buf );
			if ( getAttrValue(top, nodeName, vlanid[1], buf) == TCAPI_PROCESS_OK ) {
				if ( strlen(buf) > 0 ) {
					sprintf( qosCmdBuf, "%s:%s", qosCmdBuf, buf );
				}
			}
		}
	}
	if ( strlen(qosCmdBuf) > 0 ) {
		sprintf( qosCmd, "%s vlanid %s", qosCmd, qosCmdBuf );
	}

	/* excute "qoscmd type" rule */
	excQosCmd( qosCmd );

	/* remark rule */
#ifdef IPV6
	sprintf( qosCmd, "qoscmd remark add -i %d IPVersion all", index );
#else
	sprintf( qosCmd, "qoscmd remark add -i %d IPVersion v4", index );
#endif

#ifdef TCSUPPORT_QOS
	char queue[16] = {"ActQueue"};
	char nodeCommon[MAX_ARG_NUM][MAX_NODE_NAME];
	char queue_buf[10];

	memset(nodeCommon, 0, sizeof(nodeCommon ));
	strcpy(nodeCommon[0], "QoS");
	strcpy(nodeCommon[1], "Common");
	
	if (getAttrValue(top, nodeName, queue, buf) == TCAPI_PROCESS_OK) {
		if (atoi(buf) > 0 && atoi(buf) < 7) {
			sprintf(queue_buf, "QueueP%d", atoi(buf));
			if (getAttrValue(top, nodeCommon, queue_buf, buf) == TCAPI_PROCESS_OK) {
				if (atoi(buf) > 0 && atoi(buf) < 7) {
					sprintf(qosCmd, "%s queue %s", qosCmd, buf);
				}
				else {
					return -1;
				}
			}
			else {
				return -1;
			}
		}
		else {
			return -1;
		}
	}
	else {
		return -1;
	}
#else
	/* 1. queue mark */
	char queue[16] = {"ActQueue"};
	if ( getAttrValue(top, nodeName, queue, buf) == TCAPI_PROCESS_OK ) {
		if ( strlen(buf) > 0 ) {
			if ( !strcmp(buf, "Low") ) {
				sprintf( qosCmd, "%s queue 4", qosCmd );
			}
			else if ( !strcmp(buf, "Medium") ) {
				sprintf( qosCmd, "%s queue 3", qosCmd );
			}
			else if ( !strcmp(buf, "High") ) {
				sprintf( qosCmd, "%s queue 2", qosCmd );
			}
			else {
				sprintf( qosCmd, "%s queue 1", qosCmd );
			}
		}
		else {
			return -1;
		}
	}
	else {
		return -1;
	}

#endif

	/* 2. 802.1p remark */
	char dot1pRemark[32] = {"Act8021pRemarkingNum"};
	if ( getAttrValue(top, nodeName, dot1pRemark, buf) == TCAPI_PROCESS_OK ) {
		if ( strlen(buf) > 0 ) {
			sprintf( qosCmd, "%s dot1p %s", qosCmd, buf );
		}
	}

#if 0 //def TCSUPPORT_E8B
	/*3. dscpORTC remark */
	char dscpORTCRemark[32] = {"ActDSCPRemarking"};
	if ( getAttrValue(top, nodeName, dscpORTCRemark, buf) == TCAPI_PROCESS_OK ) {
		if ( strlen(buf) > 0 ) {
			sprintf( qosCmd, "%s dscpORTC %s", qosCmd, buf );
		}
	}
#else
	char ippdsRemark[32] = {"ActIPPDSField"};
	if ( getAttrValue(top, nodeName, ippdsRemark, buf) == TCAPI_PROCESS_OK ) {
		if ( !strcmp(buf, "IPPTOS") ) {
			/* ipp */
			char ippRemark[32] = {"ActIPPRemarking"};
			if ( getAttrValue(top, nodeName, ippRemark, buf) == TCAPI_PROCESS_OK ) {
				if ( strlen(buf) > 0 ) {
					sprintf( qosCmd, "%s ipp %s", qosCmd, buf );
				}
			}
			/* tos */
			char tosRemark[32] = {"ActToSRemarking"};
			if ( getAttrValue(top, nodeName, tosRemark, buf) == TCAPI_PROCESS_OK ) {
				if ( strlen(buf) > 0 ) {
					if ( !strcmp(buf, "Minimize monetary cost") ) {
						sprintf( qosCmd, "%s tos 0x02", qosCmd );
					}
					else if ( !strcmp(buf, "Maximize reliability") ) {
						sprintf( qosCmd, "%s tos 0x04", qosCmd );
					}
					else if ( !strcmp(buf, "Maximize throughput") ) {
						sprintf( qosCmd, "%s tos 0x08", qosCmd );
					}
					else if ( !strcmp(buf, "Minimize delay") ) {
						sprintf( qosCmd, "%s tos 0x10", qosCmd );
					}
					else {
						sprintf( qosCmd, "%s tos 0x00", qosCmd );
					}
				}
			}
		}
		else if ( !strcmp(buf, "DSCP") ) {
			char dscpORTCRemark[32] = {"ActDSCPRemarking"};
			if ( getAttrValue(top, nodeName, dscpORTCRemark, buf) == TCAPI_PROCESS_OK ) {
				if ( strlen(buf) > 0 ) {
					sprintf( qosCmd, "%s dscpORTC %s", qosCmd, buf );
				}
			}
		}
	}
#endif
	/* excute "qoscmd remark" rule */
	excQosCmd( qosCmd );
	
	return 0;
	
DelQosTypeRule:
	sprintf( qosCmd, "qoscmd type del -i %d", index );
	excQosCmd( qosCmd );
	sprintf( qosCmd, "qoscmd remark del -i %d", index );
	excQosCmd( qosCmd );
	return 0;
}
#endif



#if defined(TCSUPPORT_CT_QOS)
/* app entry attribute: Active AppName ActQueue */
int qosAppRule( mxml_node_t *top, int index, int type )
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char cwmpNode[MAX_ARG_NUM][MAX_NODE_NAME];
	char qosCmd[MAX_CMD_LENGTH], qosCmdBuf[MAX_CMD_LENGTH], buf[CMD_BUF_LENGTH];
	char url[128]={0}, dstIP[32]={0}, conreqport[12]={0};
	#if 1
	unsigned int dstPort;
	#else
	char dstPort[10];
	#endif
	char cwmp_commit[4];
#if defined(TCSUPPORT_CT_VOIP_QOS) 
	int activeQueue = 0;
#endif

	index -= MAX_TYPE_RULE_NUM;
	
	if ( index < 0 || index > MAX_APP_RULE_NUM - 1 ) {
		return -1;
	}
	
	memset( nodeName, 0, sizeof(nodeName) );
	strcpy( nodeName[0], QOS );
	sprintf( nodeName[1], "Entry%d", index + MAX_TYPE_RULE_NUM );
	
	memset( qosCmd, 0, sizeof(qosCmd) );
	memset( qosCmdBuf, 0, sizeof(qosCmdBuf) );

	/* 1. active or deactive */
	if (getAttrValue(top, nodeName, "AppName", buf) == TCAPI_PROCESS_OK) {
		if (strlen(buf) > 0) {
		/* we do not support ipv6 app qos */
		#if 0//def IPV6
			sprintf( qosCmd, "qoscmd app add -i %d IPVersion all", index );
		#else
			sprintf( qosCmd, "qoscmd app add -i %d IPVersion v4", index );
		#endif
		}
		else {
			goto DelQosAppRule;
		}
	}
	else {
		goto DelQosAppRule;
	}

	/* 2. app_name */
	if ( getAttrValue(top, nodeName, "AppName", buf) == TCAPI_PROCESS_OK ) {
		//tcdbg_printf("app name is %s.\n", buf);
		/* now we only handle tr069 app */
		/* delete type rule first for it may voip and iptv before */
		sprintf(qosCmd, "qoscmd type del -i %d", index + MAX_TYPE_RULE_NUM);
		excQosCmd(qosCmd);
		sprintf(qosCmd, "qoscmd remark del -i %d", index + MAX_TYPE_RULE_NUM);
		excQosCmd(qosCmd);
		
		sprintf( qosCmd, "qoscmd app add -i %d IPVersion v4", index );
		if (!strcmp(buf, "TR069")) {
			if (type == 0) {
				/* when boot or qos template, just return, set qos rule when send inform */
				return 0;
			}
			
			/* borrow this buf to store QoS_Common node */
			memset(cwmp_commit, 0, sizeof(cwmp_commit));
			memset(cwmpNode, 0, sizeof(cwmpNode));
			strcpy(cwmpNode[0], "QoS");
			strcpy(cwmpNode[1], "Common");
			if (getAttrValue(top, cwmpNode, "cwmpCommitFlag", cwmp_commit) == TCAPI_PROCESS_OK) {
				if (cwmp_commit[0] != '1') {
					setAttrValue(top, cwmpNode, "webCommitFlag", "1");
					return 0;
				}
			}
			else {
				setAttrValue(top, cwmpNode, "webCommitFlag", "1");
				return 0;
			}
			setAttrValue(top, cwmpNode, "cwmpCommitFlag", "0");
			memset(cwmpNode, 0, sizeof(cwmpNode));
			strcpy(cwmpNode[0], "Cwmp");
			strcpy(cwmpNode[1], "Entry");
		//follow need to modify!!!
		#if 1 /* should user parerurl xyzhu_nj_20100928 */
			if (getAttrValue(top, cwmpNode, "acsUrl", url) == TCAPI_PROCESS_OK) {
				if (parseUrl(url, dstIP, &dstPort) == 0) {
					if ( TCAPI_PROCESS_OK == getAttrValue(top, cwmpNode, "conReqPort", conreqport)
						&& '\0' != conreqport[0] )
						sprintf( qosCmd, "%s tr069 %s %s %u", qosCmd, dstIP, conreqport, dstPort );
					else
						sprintf( qosCmd, "%s tr069 %s tcp %u", qosCmd, dstIP, dstPort );
				}
				else {
					tcdbg_printf("%s:parser tr069 url failed. \n", __FUNCTION__);
					return -1;
				}
			}
		#else
			memset(dstIP, 0, sizeof(dstIP));
			memset(dstPort, 0, sizeof(dstPort));
			if (getAttrValue(top, cwmpNode, "CPHostName", dstIP) == TCAPI_PROCESS_OK) {
				if (getAttrValue(top, cwmpNode, "CpePort", dstPort) == TCAPI_PROCESS_OK) {
					if (dstIP[0] != '\0' && dstPort[0] != '\0') {
						sprintf( qosCmd, "%s tr069 %s tcp %s", qosCmd, dstIP, dstPort );
					}
				}
			}
		#endif
		}
		else if (!strcmp(buf, "IPTV")) {
			/* delete app rule first, for it may be tr069 rule before */
			sprintf( qosCmd, "qoscmd app del -i %d", index );
			excQosCmd( qosCmd );
			
			setAttrValue(top, nodeName, "ProtocolID", "igmp");
			qosTypeRule(top, index + MAX_TYPE_RULE_NUM);
			return 0;
		}
		else if (!strcmp(buf, "VOIP")) {
#if defined(TCSUPPORT_CT_VOIP_QOS)	
			if (getAttrValue(top, nodeName, "ActQueue", buf) == TCAPI_PROCESS_OK) {
				activeQueue= atoi(buf);
				if (activeQueue< 1 || activeQueue > 7) {
					return -1;
				}				
				sprintf(qosCmd,"echo %d > %s",activeQueue,RTP_PRIORITY_QUEUE_PATH);
				system(qosCmd);
			}
#else
			/* delete app rule first, for it may be tr069 rule before */
			sprintf( qosCmd, "qoscmd app del -i %d", index );
			excQosCmd( qosCmd );
			
			setAttrValue(top, nodeName, "ProtocolID", "rtp");
			qosTypeRule(top, index + MAX_TYPE_RULE_NUM);
#endif
			return 0;
		}
		else {
			return -1;
		}
	}
	else {
		return -1;
	}

	/* 3. queue */
	char wanNode[MAX_ARG_NUM][MAX_NODE_NAME];
	char queuebuf[10], queuenum = 0;

	memset(wanNode, 0, sizeof(wanNode));
	strcpy(wanNode[0], "QoS");
	strcpy(wanNode[1], "Common");
	
	if (getAttrValue(top, nodeName, "ActQueue", buf) == TCAPI_PROCESS_OK) {
		queuenum = atoi(buf);
		if (queuenum < 1 || queuenum > 7) {
			return -1;
		}
		
		sprintf(queuebuf, "QueueP%d", atoi(buf));
		if (getAttrValue(top, wanNode, queuebuf, buf) == TCAPI_PROCESS_OK) {
			queuenum = atoi(buf);
			if (queuenum < 0 || queuenum > 7) {
				return -1;
			}
			sprintf(qosCmd, "%s %d", qosCmd, queuenum);
		}
		else {
			return -1;
		}
	}
	else {
		return -1;
	}

	excQosCmd(qosCmd);
	return 0;
	
DelQosAppRule:
	sprintf( qosCmd, "qoscmd app del -i %d", index );
	excQosCmd( qosCmd );
	return 0;
}
int qosTemRule( mxml_node_t *top, int index )
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char nodeCommon[MAX_ARG_NUM][MAX_NODE_NAME];
	char qosCmd[MAX_CMD_LENGTH], qosCmdBuf[MAX_CMD_LENGTH], buf[CMD_BUF_LENGTH];
	char queue_buf[10];

	index -= (MAX_TYPE_RULE_NUM + MAX_APP_RULE_NUM);
	
	if ( index < 0 || index > MAX_TEM_RULE_NUM - 1 ) {
		return -1;
	}


	
	memset( nodeName, 0, sizeof(nodeName) );
	strcpy( nodeName[0], QOS );
	sprintf( nodeName[1], "Entry%d", index + MAX_TYPE_RULE_NUM + MAX_APP_RULE_NUM );
	
	memset( qosCmd, 0, sizeof(qosCmd) );
	memset( qosCmdBuf, 0, sizeof(qosCmdBuf) );

	/* 1. active or deactive */
	if (getAttrValue(top, nodeName, "ProtocolID", buf) == TCAPI_PROCESS_OK) {
		if (strlen(buf) > 0) {
		#if 0//def IPV6
			sprintf( qosCmd, "qoscmd template add -i %d IPVersion all", index );
		#else
			sprintf( qosCmd, "qoscmd template add -i %d IPVersion v4", index );
		#endif
		}
		else {
			goto DelQosTemRule;
		}
	}
	else {
		goto DelQosTemRule;
	}

	/* 2. dport */
	memset( qosCmdBuf, 0, sizeof(qosCmdBuf) );
	char dport[][32] = 
	{
		{"DstPortRangeBegin"}, {"DstPortRangeEnd"}
	};
	if ( getAttrValue(top, nodeName, dport[0], buf) == TCAPI_PROCESS_OK ) {
		if ( strlen(buf) > 0 ) {
			strcpy( qosCmdBuf, buf );

			/* check if there src port end */
			if ( getAttrValue(top, nodeName, dport[1], buf) == TCAPI_PROCESS_OK ) {
				if ( strlen(buf) > 0 ) {
					sprintf( qosCmdBuf, "%s:%s", qosCmdBuf, buf );
				}
			}
		}
	}
	if ( strlen(qosCmdBuf) > 0 ) {
		sprintf( qosCmd, "%s dport %s", qosCmd, qosCmdBuf ) ;
	}

	/* 3. dip */
	memset( qosCmdBuf, 0, sizeof(qosCmdBuf) );
	char dip[][32] =
	{
		{"DstIPRangeBegin"}, {"DstMaskRangeBegin"},
		{"DstIPRangeEnd"}, 	 {"DstMaskRangeEnd"},	
		{""}
	};
	if ( getAttrValue(top, nodeName, dip[0], buf) == TCAPI_PROCESS_OK ) {
		if ( strlen(buf) > 0 ) {
			/* dst ip begin */
			strcpy( qosCmdBuf, buf );

			/* if there is mask of dst ip begin */
			if ( getAttrValue(top, nodeName, dip[1], buf) == TCAPI_PROCESS_OK ) {
				if ( strlen(buf) > 0 ) {
					sprintf( qosCmdBuf, "%s/%s", qosCmdBuf, buf );
				}
			}

			/* check if there is dst ip end */
			if ( getAttrValue(top, nodeName, dip[2], buf) == TCAPI_PROCESS_OK ) {
				if ( strlen(buf) > 0 ) {
					sprintf( qosCmdBuf, "%s-%s", qosCmdBuf, buf );

					/* if there is mask of dst ip end */
					if ( getAttrValue(top, nodeName, dip[3], buf) == TCAPI_PROCESS_OK ) {
						sprintf( qosCmdBuf, "%s/%s", qosCmdBuf, buf );
					}
				}
			}
		}
	}
	if ( strlen(qosCmdBuf) > 0 ) {
		sprintf( qosCmd, "%s dport %s", qosCmd, qosCmdBuf ) ;
	}
	
	/* 4. proto */
	memset( qosCmdBuf, 0, sizeof(qosCmdBuf) );
	char protoid[32] = {"ProtocolID"};
	if ( getAttrValue(top, nodeName, protoid, buf) == TCAPI_PROCESS_OK ) {
		if ( strlen(buf) > 0 ) {
			sprintf( qosCmd, "%s proto %s", qosCmd, buf );
		}
	}
	memset(nodeCommon, 0, sizeof(nodeCommon));
	strcpy(nodeCommon[0], "QoS");
	strcpy(nodeCommon[1], "Common");
	/* 5. queue */
	char queue[16] = {"ActQueue"};
	if ( getAttrValue(top, nodeName, queue, buf) == TCAPI_PROCESS_OK ) {
		if (atoi(buf) > 0 && atoi(buf) < 7) {
			sprintf(queue_buf, "QueueP%d", atoi(buf));
			if (getAttrValue(top, nodeCommon, queue_buf, buf) == TCAPI_PROCESS_OK) {
				if (atoi(buf) > 0 && atoi(buf) < 7) {
					sprintf(qosCmd, "%s queue %s", qosCmd, buf);
				}
				else {
					return -1;
				}
			}
			else {
				return -1;
			}
		}
		else {
			return -1;
		}
	}
	else {
		return -1;
	}

	excQosCmd( qosCmd );
	return 0;

DelQosTemRule:
	sprintf( qosCmd, "qoscmd template del -i %d", index );
	excQosCmd( qosCmd );
	return 0;
}
/* vlanid Entry attribute: Active vpidotvci vlanid*/
int qosVlanidRule( mxml_node_t *top, int index, int type )
{
	char qosCmd[MAX_CMD_LENGTH], buf[CMD_BUF_LENGTH];
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];

	index -= (MAX_TYPE_RULE_NUM + MAX_APP_RULE_NUM + MAX_TEM_RULE_NUM);
	
	if ( index < 0 || index > MAX_VLAN_RULE_NUM - 1 ) {
		return -1;
	}
	
	memset( nodeName, 0, sizeof(nodeName) );
	strcpy( nodeName[0], QOS );
	sprintf( nodeName[1], "Entry%d", index + MAX_TYPE_RULE_NUM + MAX_APP_RULE_NUM + MAX_TEM_RULE_NUM );

	strcpy( qosCmd, "qoscmd vlan" );
	if (type == 1) {
		/* 1. Active */
		if ( getAttrValue(top, nodeName, "Active", buf) == TCAPI_PROCESS_OK ) {
			if ( strcmp(buf, "Yes") == 0 ) {
				sprintf( qosCmd, "%s on", qosCmd );
			}
			else if ( strcmp(buf, "No") == 0 ) {
				sprintf( qosCmd, "%s off", qosCmd );
			}
			else {
				return -1;
			}
		}
		else {
			return -1;
		}
	}
	else {
		/* delete */
		sprintf( qosCmd, "%s off", qosCmd );
	}


	
#if 0
	/* 2. vpidotvci */
	if ( getAttrValue(top, nodeName, "vpidotvci", buf) == TCAPI_PROCESS_OK ) {
		if ( strlen(buf) > 0 ) {
			sprintf( qosCmd, "%s vpidotvci %s", qosCmd, buf );
		}
		else {
			return -1;
		}
	}
	else {
		return -1;
	}
#endif
	/* 2. vpidotvci get vpi and vci from wan node */
	char wanNode[MAX_ARG_NUM][MAX_NODE_NAME];
	char vpi[4], vci[4];
	if (getAttrValue(top, nodeName, "pvcIndex", buf) == TCAPI_PROCESS_OK) {
		if (!strlen(buf) || atoi(buf) < 0 || atoi(buf) > 8) {
			return -1;
		}

		strcpy(wanNode[0], "Wan");
		sprintf(wanNode[1], "PVC%d", atoi(buf));

		if (getAttrValue(top, wanNode, "VPI", vpi) == TCAPI_PROCESS_OK) {
			if (getAttrValue(top, wanNode, "VCI", vci) == TCAPI_PROCESS_OK) {
				sprintf(qosCmd, "%s vpidotvci %d.%d", qosCmd, atoi(vpi), atoi(vci));
			}
			else {
				return -1;
			}
		}
		else {
			return -1;
		}
	}

	/* 3. vlanid */
	if ( getAttrValue(top, nodeName, "vlanid", buf) == TCAPI_PROCESS_OK ) {
		if ( strlen(buf) > 0 ) {
			sprintf( qosCmd, "%s vlanid %s", qosCmd, buf );
		}
		else {
			return -1;
		}
	}
	else {
		return -1;
	}

	excQosCmd( qosCmd );
	return 0;

}
#endif

char *qos_adv_table[MAX_NODE_NAME]=
{
	"Common","",
};
char *qos_merge_table[MAX_NODE_NAME]=
{
	"qos_id","",
};

/*______________________________________________________________________________
**	qos_init
**
**	descriptions:
**		init function of qos.
**
**	parameters:
**		none.
**
**	return:
**		0 means success
**		1 means fail
**
**____________________________________________________________________________
*/
int
qos_init(void){
		//tcdbg_printf("%s ",__func__);
		int ret=0;
		cfg_node_t node={
				.name="QoS",
				/*__________________________________
				** rule index 0~9 	  <==>  type rule
				** rule index 10~13   <==>  app rule
				** rule index 14~21   <==>  template rule
				** rule index 22~29	  <==>  vlan id mark
				**__________________________________
				*/

#if defined(TCSUPPORT_CT_QOS)
				.type=ADV_FLAG|ENTRY_TYPE|UPDATE_TYPE|MAX_QOS_RULE,
#else
				.type=ADV_FLAG|ENTRY_TYPE|MAX_QOS_RULE,
#endif
				.adv_table=qos_adv_table,
				.merge_table=qos_merge_table,
#if defined(TCSUPPORT_CT_QOS)
				.cfg_read=qos_read,
#else
				.cfg_read=NULL,
#endif
				.cfg_write=NULL,
				.cfg_verify=NULL,
				.cfg_execute=qos_execute,
				.cfg_boot=qos_boot,
		};
		ret=register_cfg_node(&node);
		return ret;
}

#if defined(TCSUPPORT_CT_QOS)
int qos_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char* attr)
{
	int i, entry_index;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char temNode[MAX_NODE_NAME], buf[10], idx[10];
	char idxBuf[64], typeName[20], typeVal[20];

	memset( nodeName, 0, sizeof(nodeName) );
	strcpy( nodeName[0],QOS );
	memset( temNode, 0, sizeof(temNode) );

	if (!strcmp(attr, "curTypeIdx") != 0) {
		/* get curTypeIdx */
		for ( i = 0; i < MAX_TYPE_RULE_NUM; i++ ) {
			sprintf( temNode, "Entry%d", i );
			strcpy( nodeName[1], temNode );
			if ( getAttrValue( top, nodeName, "ActQueue", buf ) != TCAPI_PROCESS_OK ) {
				/* it means this Entry is empty */
				strcpy( nodeName[1], QOS_COMMON );
				sprintf( idx, "%d", i );
				setAttrValue( top, nodeName, "curTypeIdx", idx );
				break;
			}
		}
		if ( i == MAX_TYPE_RULE_NUM ) {
			strcpy( nodeName[1], QOS_COMMON );
			setAttrValue( top, nodeName, "curTypeIdx", "N/A" );
		}
	}
	else if (!strcmp(attr, "curAppIdx")) {
		/* get cufAppIdx */
		for ( i = MAX_TYPE_RULE_NUM; i < MAX_TYPE_RULE_NUM + MAX_APP_RULE_NUM; i++ ) {
			sprintf( temNode, "Entry%d", i );
			strcpy( nodeName[1], temNode );
			if ( getAttrValue( top, nodeName, "ActQueue", buf ) != TCAPI_PROCESS_OK ) {
				/* it means this Entry is empty */
				strcpy( nodeName[1], QOS_COMMON );
				sprintf( idx, "%d", i );
				setAttrValue( top, nodeName, "curAppIdx", idx );
				break;
			}
		}
		if ( i == MAX_TYPE_RULE_NUM + MAX_APP_RULE_NUM ) {
			strcpy( nodeName[1], QOS_COMMON );
			setAttrValue( top, nodeName, "cufAppIdx", "N/A" );
		}
	}
	else if (!strcmp(attr, "curSubTypeIdx")) {
		/* get curSubTypeIdx */
		
		memset(nodeName,0,sizeof(nodeName));
		strcpy(nodeName[0],WEBCURSET);
		strcpy( nodeName[1],SUB_NODE_NAME );
		if (getAttrValue(top, nodeName, "qos_id", idxBuf) == TCAPI_PROCESS_OK) {
			entry_index = atoi(idxBuf);
		}else{
			/* it will not happen in general */
			return 0;
		}

		memset(nodeName,0,sizeof(nodeName));
		strcpy(nodeName[0],QOS);
		sprintf(nodeName[1], "Entry%d", entry_index);
		for (i = 0; i < MAX_TYPE_NUM; i++) {
			sprintf(typeName, "Type%d", i + 1);
			if ((getAttrValue(top, nodeName, typeName, typeVal) != TCAPI_PROCESS_OK) || (!strcmp(typeVal, "N/A"))) {
				/* it means this Entry is empty */
				strcpy( nodeName[1], QOS_COMMON );
				sprintf( idx, "%d", i + 1 );
				setAttrValue( top, nodeName, "curSubTypeIdx", idx );
				break;
			}
		}
		if (i == MAX_TYPE_NUM) {
			strcpy( nodeName[1], QOS_COMMON );
			setAttrValue( top, nodeName, "curSubTypeIdx", "N/A" );
		}
	}

	return 0;
}
#endif

/*______________________________________________________________________________
**	qos_boot
**
**	descriptions:
**		the boot function of qos.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
qos_boot(mxml_node_t *top)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char buf[CMD_BUF_LENGTH];
	int ret, i;
#if defined(TCSUPPORT_CT_VOIP_QOS) 
	char qosCmd[MAX_CMD_LENGTH];
#endif

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],QOS);
	strcpy(nodeName[1],QOS_COMMON);

#if defined(TCSUPPORT_CT_QOS)
	/*Init qos boot*/
	memset(appmode,0,sizeof(appmode));
	if (getAttrValue(top, nodeName, "Mode", buf) == TCAPI_PROCESS_OK) 
	{
		tcdbg_printf("qos_boot: init app mode for boot [%s]\n", buf);
		strcpy(appmode, buf);

	}

	is_qos_boot = 1;
#endif

#if 0
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],QOS);
	strcpy(nodeName[1],QOS_COMMON);
#endif

	if ( getAttrValue(top, nodeName, NODE_ACTIVE, buf) == TCAPI_PROCESS_OK ) {
		if ( strcmp(buf,"Yes") == 0 ) {
			/* qos is on at boot time */
			ret = setQoSSwitch( top, ON );
			if ( ret == -1 ) {
				return FAIL;
			}


#if defined(TCSUPPORT_CT_QOS)			
			/* if QoS is from down to up, we set QoS template rules by template name*/
			setQoSTempRules(top, 1);
#endif
			/* 0. set qos discpline */
			ret = qosDiscplineRule( top );
			if ( ret == -1 ) {
				return FAIL;
			}
	
			/* 1. excute type rules */
			for ( i = 0; i < MAX_TYPE_RULE_NUM; i++ ) {
				qosTypeRule( top, i );
			}

			
#if defined(TCSUPPORT_CT_QOS)
			/* 2. excute app rules */
#if defined(TCSUPPORT_CT_VOIP_QOS)	
			//clear rtp priority first
			memset( qosCmd, 0, sizeof(qosCmd) );
			sprintf(qosCmd,"echo 0 > %s",RTP_PRIORITY_QUEUE_PATH);
			system(qosCmd);
#endif

			for ( i = 0; i < MAX_APP_RULE_NUM; i++ ) {
				qosAppRule(top, i + MAX_TYPE_RULE_NUM, 0);
			}

			/* 3. excute vlan id rules */
			for ( i = 0; i < MAX_VLAN_RULE_NUM; i++ ) {
				qosVlanidRule(top, i + MAX_TYPE_RULE_NUM + MAX_TEM_RULE_NUM, 1);
			}

			memset(buf,0,sizeof(buf));	
			if(getAttrValue(top, nodeName, "Mode", buf) == TCAPI_PROCESS_OK){
				strcpy(appmode,buf);
			}
#endif
		}
		else {
#if defined(TCSUPPORT_CT_QOS)
			/* qos is off, but set qos rule by qos template */
			setQoSTempRules(top, 0);
#endif
		}
	}

	return SUCCESS;
}

int getCurrentIndex(mxml_node_t *top, char name[][MAX_NODE_NAME])
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char idxBuf[64];
	int index = -1;
	
	if ( get_entry_number(name[1],"Entry",&index) != SUCCESS ) {
		memset( nodeName,0,sizeof(nodeName) );
		strcpy( nodeName[0],WEBCURSET );
		strcpy( nodeName[1],SUB_NODE_NAME );
		
		if ( getAttrValue(top, nodeName, "qos_id", idxBuf) == TCAPI_PROCESS_OK ) {
			index = atoi(idxBuf);
		}else{
			return -1;
		}
	}

	return index;
}

/* QoS_Common: QoSOptType = {discRule, typeRule, appRule, temRule, vlanidRule} */
int
qos_execute(mxml_node_t *top, char name[][MAX_NODE_NAME])
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char buf[CMD_BUF_LENGTH];
	int index;
	int downToUpFlag = 0;
	int i;
#if defined(TCSUPPORT_CT_SWQOS)	
	char attr_buf[32];
	int que_enable[6] = {0};	
#endif

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],QOS);
	strcpy(nodeName[1],QOS_COMMON);
	
#if defined(TCSUPPORT_CT_HWQOS)
	if ( !is_qos_boot )
	{
		if (getAttrValue(top, nodeName, "Mode", buf) == TCAPI_PROCESS_OK) 
			strcpy(appmode, buf);
	}
#endif

	/* check if there is uplink bandwidth in cfg node */
	if ( getAttrValue(top, nodeName, "Active", buf) == TCAPI_PROCESS_OK ) {
		if ( strcmp(buf, "Yes") == 0 ) {
#if defined(TCSUPPORT_CT_SWQOS) && !defined(TCSUPPORT_CPU_EN7521)		
			system("echo 1 > /proc/tc3162/qos_enable");
			//tcdbg_printf("\r\nqos_excute echo 1 > /proc/tc3162/qos_enable");
#endif
			if ( setQoSSwitch(top, 1) == 0 ) {
#if defined(TCSUPPORT_CT_QOS)
				/* reset vlan rule again */
				for ( i = 0; i < MAX_VLAN_RULE_NUM; i++ ) {
					qosVlanidRule( top, i + MAX_TYPE_RULE_NUM + MAX_TEM_RULE_NUM, 1);
				}
#endif
				downToUpFlag = 1;
			}	
#if defined(TCSUPPORT_CT_QOS)
			setQoSTempRules(top, 1);
#endif
#if defined(TCSUPPORT_CT_SWQOS)
			for(i = 0; i<6; i++){
				sprintf(attr_buf, "QueueSW%d", i+1);
				memset(buf, 0, sizeof(buf));
				getAttrValue(top, nodeName, attr_buf, buf);
				if(strcmp(buf, "Yes") == 0 )
					que_enable[i] = 1;
			}
			sprintf(buf,"echo %d %d %d %d %d %d 1 > /proc/tc3162/qos_queue_status",
				que_enable[0],que_enable[1],que_enable[2],que_enable[3],que_enable[4],que_enable[5]);
			system(buf);
#endif
		}
		else {
			qosWanInterfaceUpdate(top, 0);
#if defined(TCSUPPORT_CT_SWQOS)
			system("echo 0 > /proc/tc3162/qos_enable");
			//tcdbg_printf("\r\nqos_excute echo 0 > /proc/tc3162/qos_enable");
#endif
			/* qos is off, but set qos rule by qos template */
#if defined(TCSUPPORT_CT_QOS)
			qosDiscplineRule(top);
			setQoSTempRules(top, 0);
#endif
			
			setQoSSwitch(top, 0);
			goto QOS_EXE_SUCC;
		}
	}
	else {
		goto QOS_EXE_FAIL;
	}

	/* qos is active */
		if ( getAttrValue(top, nodeName, "QoSOptType", buf) == TCAPI_PROCESS_OK ) {
		/* if qos is frow disabled to enable, set QoS discpline */
		if ( strcmp(buf, "discRule") != 0 && downToUpFlag == 1 ) {
			qosDiscplineRule(top);
		}
		
		if ( strcmp(buf, "discRule") == 0 ) {
			qosDiscplineRule(top);
		}
		else if ( strcmp(buf, "typeRule") == 0 ) {
			/* get current Entry index */
			index = getCurrentIndex( top, name );
			if ( index == -1 ) {
				goto QOS_EXE_FAIL;
			}
			qosTypeRule( top, index);
		}
		
#if defined(TCSUPPORT_CT_QOS)
		else if ( strcmp(buf, "appRule") == 0 ) {
			/* get current Entry index */
			index = getCurrentIndex( top, name );
			if ( index == -1 ) {
				goto QOS_EXE_FAIL;
			}
			qosAppRule( top, index, 1);
		}

		
		/* if open QoS, we set QoS template rules by template name */
#if 0
		else if ( strcmp(buf, "temRule") == 0 ) {
			/* get current Entry index */
			index = getCurrentIndex( top, name );
			if ( index == -1 ) {
				return FAIL;
			}
			qosTemRule( top, index );
		}
#endif

		else if ( strcmp(buf, "vlanidRule") == 0 ) {
			/* get current Entry index */
			index = getCurrentIndex( top, name );
			if ( index == -1 ) {
				goto QOS_EXE_FAIL;
			}
			qosVlanidRule( top, index, 1 );
		}
#endif
		else {
			goto QOS_EXE_FAIL;
		}
	}
	else {
		goto QOS_EXE_FAIL;
	}

QOS_EXE_SUCC:
#if defined(TCSUPPORT_CT_HWQOS) || defined(TCSUPPORT_CPU_EN7521)
	system("/userfs/bin/hw_nat -!");
#endif
	return SUCCESS;

QOS_EXE_FAIL:
#if defined(TCSUPPORT_CT_HWQOS) || defined(TCSUPPORT_CPU_EN7521)
	system("/userfs/bin/hw_nat -!");
#endif
	return FAIL;
}

#endif

#if QOS_REMARKING  /*Rodney_20090724*/
char *qos_adv_table[MAX_NODE_NAME]=
{
	"Common","",
};
char *qos_merge_table[MAX_NODE_NAME]=
{
	"qos_id","",
};
#ifdef QOS_DROP  /*Rodney_20091115*/
/*_____________________________________________________________________________
**      function name: qos_drop_set
**
**      descriptions:  insmod/rmmod sch_red.ko and set/unsetparameter to driver
**
**      parameters:    mode:QOS Active or not
**                     drop_mode_id: drop mode(RED, WRED, tail drop)
**                     prio_mode_id: priority mode for WRED
**
**      global:
**             None
**
**      return:
**             None
**____________________________________________________________________________
*/
void qos_drop_set(int mode, int drop_mode_id, int prio_mode_id){
	char stringCmd[256]={0};	

	sprintf(stringCmd,"echo %d 0 0 0 0 > /proc/tc3162/tsarm_qosred\n",drop_mode_id);
	system(stringCmd);

	if(mode == ENABLE){
		char path[100], vername[15];
		extern void decideModulePath(char*);
		decideModulePath(vername);

		sprintf(path, QOS_DROP_PATH, vername);
		sprintf(stringCmd,"insmod %ssch_red.ko\n",path);
		system(stringCmd);
		sprintf(stringCmd,"echo %d %d 0 0 0 > /proc/sch_qosred\n",drop_mode_id, prio_mode_id);
		system(stringCmd);
	}
	else{
		qos_drop_unset_rule();
		sprintf(stringCmd,"rmmod sch_red.ko\n");
		system(stringCmd);	
	}
}
/*_____________________________________________________________________________
**      function name: qos_drop_reset
**
**      descriptions:  reset the packet drop machine
**
**      parameters:
**
**      global:
**             None
**
**      return:
**             None
**____________________________________________________________________________
*/
void qos_drop_reset(mxml_node_t *top){
	char encap_tmp[32], isp_tmp[32], tmp[32], active[32], drop_mode[32], prio_mode[32];
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	int isp, pvc, drop_mode_id, prio_mode_id;

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],QOS);
	strcpy(nodeName[1],QOS_COMMON);

	if(getAttrValue(top, nodeName, NODE_ACTIVE, active) != TCAPI_PROCESS_OK)
		goto unset_drop;

	if(strcmp(active,ACTIVE) == 0){ /*QoS active*/
		if(getAttrValue(top, nodeName, "Drop", drop_mode) != TCAPI_PROCESS_OK)
			goto unset_drop;
		if(getAttrValue(top, nodeName, "Prio", prio_mode) != TCAPI_PROCESS_OK)
			goto unset_drop;

		if(strcmp(drop_mode,"WRED") == 0)
			drop_mode_id = QOS_DROP_WRED;
		else if(strcmp(drop_mode,"RED") == 0)
			drop_mode_id = QOS_DROP_RED;
		else
			drop_mode_id = QOS_DROP_TAIL;	

		if(strcmp(prio_mode,"DSCP") == 0)
			prio_mode_id = QOS_PRIO_DSCP;
		else if(strcmp(prio_mode,"IPP") == 0)
			prio_mode_id = QOS_PRIO_IPP;
		else
			prio_mode_id = QOS_PRIO_QUEUE;	
		
		if((drop_mode_id == QOS_DROP_RED) || (drop_mode_id == QOS_DROP_WRED))
			qos_drop_set(ENABLE, drop_mode_id, prio_mode_id);
		else
			goto unset_drop;

		for(pvc=0 ; pvc<PVC_NUM ; pvc++){
	    	memset(nodeName,0,sizeof(nodeName));
   			strcpy(nodeName[0],WAN);
			sprintf(nodeName[1],SUB_PVC_NAME,pvc);

			if(getAttrValue(top, nodeName, "Active", tmp) != TCAPI_PROCESS_OK)
				continue;
			if(strcmp(tmp, ACTIVE) != 0)
				continue;
			if(getAttrValue(top, nodeName, "ISP", isp_tmp) != TCAPI_PROCESS_OK)
				continue;
			isp = atoi(isp_tmp);
			if(getAttrValue(top, nodeName, "ENCAP", encap_tmp) != TCAPI_PROCESS_OK)
				continue;

			if((strcmp(encap_tmp, "PPPoA LLC") == 0) || (strcmp(encap_tmp, "PPPoA VC-Mux") == 0))
				qos_drop_set_rule(QOS_PPP_INTERFACE, pvc);	
			else
				qos_drop_set_rule(QOS_NAS_INTERFACE, pvc);	
		}	
	}
	return;

unset_drop:
	qos_drop_set(DISABLE, QOS_DROP_TAIL, 0);
	return;
}
/*______________________________________________________________________________
**function name:qos_drop_set_rule
** 
**description:   set related qos drop parameter
*                
**parameters:
*                interface: 0:PPP  1:NAS
*                      pvc: PVC
**global:
*                none
**return:
*                none
**____________________________________________________________________________*/
void qos_drop_set_rule(int interface, int pvc){
	char stringCmd[512]={0}, string_tmp[16];
	char string_add[32] ={"tc qdisc add dev "};
	char string_1[32] = {"parent root handle 1:0 red "};
	char string_2[128];

	sprintf(string_2,"limit %d min %d max %d avpkt %d burst %d probability %d",DROP_LIMIT, DROP_MIN, DROP_MAX, DROP_AVPKT, DROP_BURST, DROP_PRO);
	sprintf(stringCmd, string_add);
	if(interface == QOS_PPP_INTERFACE)  /*ppp*/
		sprintf(string_tmp,"ppp%d ",pvc);
	else  /*nas*/
		sprintf(string_tmp,"nas%d ",pvc);
	strcat(stringCmd, string_tmp);
	strcat(stringCmd, string_1);
	strcat(stringCmd, string_2);
	tcdbg_printf("qos_drop_set_rule(%s)\n",stringCmd);  /*Rodney_20091115_test*/
	system(stringCmd);
}
/*______________________________________________________________________________
**function name:qos_drop_unset_rule
** 
**description:   remove related qos drop parameter
*                
**parameters:
*                none
**global:
*                none
**return:
*                none
**____________________________________________________________________________*/
void qos_drop_unset_rule(void){
	char stringCmd[512]={0}, string_tmp[16];
	char string_del[32] ={"tc qdisc del dev "};
	int pvc;

	for(pvc=0;pvc<PVC_NUM;pvc++){
		sprintf(stringCmd, string_del);
		sprintf(string_tmp,"ppp%d ",pvc);
		strcat(stringCmd,string_tmp);
		strcat(stringCmd,"root");
		system(stringCmd);
		sprintf(stringCmd, string_del);
		sprintf(string_tmp,"nas%d ",pvc);
		strcat(stringCmd,string_tmp);
		strcat(stringCmd,"root");
		system(stringCmd);
	}
}

#endif
/*_____________________________________________________________________________
**      function name: qos_checkRangeValue
**      descriptions:
**           check value with range is valid or not
**
**      parameters:
**         value1: smaller value that need to check
**         value2: larger value that need to check
**            min: minimum value
**            max: maximum value
**
**      global:
**             None
**
**      return:
**             None
**____________________________________________________________________________
*/
int
qos_checkRangeValue(unsigned int val1, unsigned int val2, unsigned int min, unsigned int max){

	if((val1>val2) || (min>max))
		return 0;
	if((val1<min) || (val1>max) || (val2<min) || (val2>max))
		return 0;
	return 1;
}
/*______________________________________________________________________________
**	qos_paramcheck
**
**	descriptions:
**		check the qos parameter.
**
**	parameters:
**            top:  Specify the root of mxml_node_t structure.
**  qos_attr_name:  Name of qos attribute
**          index:  Index of current chain
**
**	return:
**		0 means invalid
**		1 means valid
**
**____________________________________________________________________________
*/
int
qos_paramcheck(mxml_node_t *top, char nodeName[][MAX_NODE_NAME], char qos_attr_name[][32], int index){
	char mask_dec[2]={0}, *end;
	unsigned int rule_tmp1, rule_tmp2, min, max;
	int i, act_tmp1, act_tmp2;

#ifdef MBSS_SUPPORT
	char BssidNumName[MAX_ARG_NUM][MAX_NODE_NAME];
	unsigned int bssid_num = 0;
	char temp[4] = {'\0'};
	strcpy(BssidNumName[0], "WLan");
	strcpy(BssidNumName[1], "Common");
	if(getAttrValue(top, BssidNumName, "BssidNum", temp) == TCAPI_PROCESS_OK){
		if(!strcmp(temp, "2")){
			bssid_num = 2;
		}
		else if(!strcmp(temp, "3")){
			bssid_num = 3;
		}
		else if(!strcmp(temp, "4")){
			bssid_num = 4;
		}
	}
#endif
	for(i=0 ; i<QOS_ATTRLEN; i++){
		qos_check[i] = 0;
		/*check if attribute exist or not*/
		if(getAttrValue(top, nodeName, qos_attr_name[i], qos_attr_val[i]) == TCAPI_PROCESS_OK){
			if(strcmp(qos_attr_val[i],"") != 0){
#ifdef MBSS_SUPPORT
				if ( bssid_num == 2 ) {
					switch(i){
						case QOS_PHYPORTWLAN1:
							if(strcmp(qos_attr_val[i], "Yes") == 0){
								//tcdbg_printf("222sssssss\n");
								qos_check[i] = 1;
							}
							break;
						default:
							break;
					}
				}
				else if ( bssid_num == 3 ) {
					switch(i){
						case QOS_PHYPORTWLAN1:
						case QOS_PHYPORTWLAN2:
							if(strcmp(qos_attr_val[i], "Yes") == 0){
								tcdbg_printf("333sssssss\n");
								qos_check[i] = 1;
							}
							break;
						default:
							break;
					}
				}
				else if ( bssid_num == 4 ) {
					switch(i){
						case QOS_PHYPORTWLAN1:
						case QOS_PHYPORTWLAN2:
						case QOS_PHYPORTWLAN3:
							if(strcmp(qos_attr_val[i], "Yes") == 0){
								tcdbg_printf("444sssssss\n");
								qos_check[i] = 1;
							}
							break;
						default:
							break;
					}
				}

#endif
				switch(i){  /*check the param format*/
					case QOS_PHYPORTETH0:
					case QOS_PHYPORTETH1:
					case QOS_PHYPORTETH2:
					case QOS_PHYPORTETH3:
					case QOS_PHYPORTUSB0:
					case QOS_PHYPORTWLAN0:
						if(strcmp(qos_attr_val[i], "Yes") == 0)
							qos_check[i] = 1;
						break;
					case QOS_PROTO:
						qos_check[QOS_PROTO] = 1;
						break;
					case QOS_DESMAC:
					case QOS_SRCMAC:
						if(check_mac_format(qos_attr_val[i]))
							qos_check[i] = 1;
						else
							 return 0;
						break;
					case QOS_DESIP:
					case QOS_SRCIP:
						if(check_ip_format(qos_attr_val[i]))
							qos_check[i] = 1;
						else
							return 0;
						break;
					case QOS_DESMASK:
						if(qos_check[QOS_DESIP] == 1){
							if(check_mask_format(qos_attr_val[i], mask_dec))
								qos_check[QOS_DESMASK] = 1;
							else
								return 0;
						}
						break;
					case QOS_SRCMASK:
						if(qos_check[QOS_SRCIP] == 1){
							if(check_mask_format(qos_attr_val[i], mask_dec))
								qos_check[QOS_SRCMASK] = 1;
							else
								return 0;
						}
						break;

					case QOS_DESPORTEND:
					case QOS_SRCPORTEND:
					case QOS_IPPEND:
					case QOS_DSCPEND:
					case QOS_VLANIDEND:
					case QOS_VLANPRIOEND:
						rule_tmp1 = (unsigned int)strtoul(qos_attr_val[i-1], &end, 0);
						rule_tmp2 = (unsigned int)strtoul(qos_attr_val[i], &end, 0);
						min = 0;
						if((i == QOS_DESPORTEND) || (i == QOS_SRCPORTEND))
							max = 65535;
						else if((i == QOS_IPPEND) || (i == QOS_VLANPRIOEND))
							max = 7;
						else if(i == QOS_DSCPEND)
							max = 63;
						else{  /*QOS_VLANIDEND*/
							min = 1;
							max = 4094;
						}
						if(qos_checkRangeValue(rule_tmp1, rule_tmp2, min, max)){
							qos_check[i-1] = 1;
							qos_check[i] = 1;
						}
						else
							return 0;
						break;
					case QOS_TOS:
						qos_check[QOS_TOS] = 1;
						break;
					case QOS_ACT_IPP:
						act_tmp1 = (unsigned int)strtoul(qos_attr_val[i], &end, 0);
						if((act_tmp1 >= 0) && (act_tmp1 <= 7)){
							qos_check[QOS_ACT_IPP] = 1;
							qos_check[QOS_ACT_TOS] = 1;
						}
						else
							return 0;
						break;
					case QOS_ACT_TOS:
					case QOS_ACT_QUEUE:
						qos_check[i] = 1;
						break;
					case QOS_ACT_DSCP:
					case QOS_ACT_VLANPRIO:
						act_tmp1 = (unsigned int)strtoul(qos_attr_val[i], &end, 0);
						if(i == QOS_ACT_DSCP)
							act_tmp2 = 63;
						else
							act_tmp2 = 7;
						if((act_tmp1 >= 0) && (act_tmp1 <= act_tmp2))
							qos_check[i] = 1;
						else
							return 0;
						break;
					default:
						break;
				}/*end switch*/
			}/*end if*/
		}/*end if*/
	}/*end for*/
	return 1;
}
/*______________________________________________________________________________
**	qos_remarking
**
**	descriptions:
**		create the qos rule of remarking.
**
**	parameters:
**             fp:  File pointer
**          index:  Index of current chain
**      jumpchain:  Name of chain for jump
**
**	return:
**
**____________________________________________________________________________
*/
void
qos_remarking(FILE *fp, int index, char *jumpchain){
	char startupCmd[128]={0}, *end;
	unsigned int act_tmp1=0, act_tmp2=0, vlan_prio;
	int i;

	for(i=QOS_ACT_IPP ; i<=QOS_ACT_QUEUE; i++){
		if(qos_check[i] == 1){
			switch(i){
				case QOS_ACT_TOS:
					if(qos_check[QOS_ACT_IPP] == 1){
						act_tmp1 |= (unsigned short int)strtoul(qos_attr_val[QOS_ACT_IPP], &end, 0)<<5;
						act_tmp2 |= 0xe0;
					}
					if(strcmp(qos_attr_val[QOS_ACT_TOS], "")){
						if(!strcmp(qos_attr_val[QOS_ACT_TOS], "Minimize monetary cost"))
							act_tmp1 |= 0x02;
						else if(!strcmp(qos_attr_val[QOS_ACT_TOS], "Maximize reliability"))
							act_tmp1 |= 0x04;
						else if(!strcmp(qos_attr_val[QOS_ACT_TOS], "Maximize throughput"))
							act_tmp1 |= 0x08;
						else if(!strcmp(qos_attr_val[QOS_ACT_TOS], "Minimize delay"))
							act_tmp1 |= 0x10;
						else  /*Normal service*/
							act_tmp1 |= 0x00;
						act_tmp2 |= 0x1e;
					}
					sprintf(startupCmd,"%s -A qos_remarking_rule%d -p IPv4 -j ftos --set-ftos 0x%x/0x%x\n",QOS_BROUTE,index, act_tmp1, act_tmp2);
					fputs(startupCmd, fp);
					break;
				case QOS_ACT_DSCP:
					sprintf(startupCmd,"%s -A qos_remarking_rule%d -p IPv4 -j ftos --set-ftos 0x%x/0xfc\n",QOS_BROUTE,index, ((unsigned short int)strtoul(qos_attr_val[QOS_ACT_DSCP], &end, 0))<<2);
					fputs(startupCmd, fp);
					break;
				case QOS_ACT_QUEUE:
					if(!strcmp(qos_attr_val[QOS_ACT_QUEUE], "Highest"))
						act_tmp1 = 0x01;  /*001*/
					else if(!strcmp(qos_attr_val[QOS_ACT_QUEUE], "High"))
						act_tmp1 = 0x03;  /*011*/
					else if(!strcmp(qos_attr_val[QOS_ACT_QUEUE], "Medium"))
						act_tmp1 = 0x05;  /*101*/
					else
						act_tmp1 = 0x07;  /*111*/

					if(qos_check[QOS_ACT_VLANPRIO] == 1){
						vlan_prio = (unsigned int)strtoul(qos_attr_val[QOS_ACT_VLANPRIO], &end, 0);
						/*bit0~2 for queue priority; bit4 for indication of meaningful value in bit18~20; bit18~20 for 802.1p*/
						act_tmp1 |= ((((vlan_prio & 0x7)) << 18) | 0x10);
					}
					sprintf(startupCmd,"%s -A qos_remarking_rule%d -j mark --mark-set 0x%x\n",QOS_BROUTE,index, act_tmp1);
					fputs(startupCmd, fp);
					break;
				default:
					break;
			}
		}
	}
}
/*______________________________________________________________________________
**	qos_IPv4rule
**
**	descriptions:
**		create the qos rule related to IPv4 parameter.
**
**	parameters:
**             fp:  File pointer
**          index:  Index of current chain
**      jumpchain:  Name of chain for jump
**
**	return:
**
**____________________________________________________________________________
*/
void
qos_IPv4rule(FILE *fp, int index, char *jumpchain){
	char string_mac[64]={0}, string_ipv4[512]={0};
	char string_tmp[128]={0}, string_total[576]={0}, *end;
	unsigned short i, len_string_mac, len_string_ipv4, len_string_total,flag_phy = 0;
#ifdef MBSS_SUPPORT
	char qos_physical_name[9][32]=
	{ {"eth0"},{"eth1"},{"eth2"},{"eth3"},{"usb0"},{"ra0"},{"ra1"},{"ra2"},{"ra3"} };
#else
	char qos_physical_name[6][32]=
	{ {"eth0"},{"eth1"},{"eth2"},{"eth3"},{"usb0"},{"ra0"} };

#endif
	if(qos_check[QOS_DESMAC] == 1){
		sprintf(string_tmp," -d %s", qos_attr_val[QOS_DESMAC]);
		strcat(string_mac, string_tmp);
	}
	if(qos_check[QOS_SRCMAC] == 1){
		sprintf(string_tmp," -s %s", qos_attr_val[QOS_SRCMAC]);
		strcat(string_mac, string_tmp);
	}
	len_string_mac = strlen(string_mac);

	for(i=QOS_PROTO ; i<=QOS_DSCPEND; i++){
		if(qos_check[i] == 1){
			switch(i){  /*create the qos rule*/
			case QOS_PROTO:
				if(strcmp(qos_attr_val[QOS_PROTO], "TCP/UDP") == 0)
					strcat(string_ipv4," --ip-proto 6:17");
				else if(strcmp(qos_attr_val[QOS_PROTO], "TCP") == 0)
					strcat(string_ipv4," --ip-proto 6");
				else if(strcmp(qos_attr_val[QOS_PROTO], "UDP") == 0)
					strcat(string_ipv4," --ip-proto 17");
				else if(strcmp(qos_attr_val[QOS_PROTO], "ICMP") == 0)
					strcat(string_ipv4," --ip-proto 1");
				else if(strcmp(qos_attr_val[QOS_PROTO], "IGMP") == 0)
					strcat(string_ipv4," --ip-proto 2");
				else
					qos_check[QOS_PROTO] = 0;
				break;
			case QOS_DESIP:
				if(qos_check[QOS_DESMASK] == 1)
					sprintf(string_tmp," --ip-dst %s/%s", qos_attr_val[QOS_DESIP], qos_attr_val[QOS_DESMASK]);
				else
					sprintf(string_tmp," --ip-dst %s", qos_attr_val[QOS_DESIP]);

				strcat(string_ipv4, string_tmp);
				break;
			case QOS_SRCIP:
				if(qos_check[QOS_SRCMASK] == 1)
					sprintf(string_tmp," --ip-src %s/%s", qos_attr_val[QOS_SRCIP], qos_attr_val[QOS_SRCMASK]);
				else
					sprintf(string_tmp," --ip-src %s",qos_attr_val[QOS_SRCIP]);

				strcat(string_ipv4, string_tmp);
				break;
			case QOS_DESPORTEND:
				if((qos_check[QOS_DESPORTBEGIN] == 1) && (qos_check[QOS_DESPORTEND] == 1) && (qos_check[QOS_PROTO])){
					if((!strcmp(qos_attr_val[QOS_PROTO], "TCP")) ||
					   (!strcmp(qos_attr_val[QOS_PROTO], "UDP")) ||
					   (!strcmp(qos_attr_val[QOS_PROTO], "TCP/UDP"))){
						sprintf(string_tmp," --ip-dport %s:%s", qos_attr_val[QOS_DESPORTBEGIN], qos_attr_val[QOS_DESPORTEND]);
						strcat(string_ipv4, string_tmp);
					}
				}
				break;
			case QOS_SRCPORTEND:
				if((qos_check[QOS_SRCPORTBEGIN] == 1) && (qos_check[QOS_SRCPORTEND] == 1) && (qos_check[QOS_PROTO])){
					if((!strcmp(qos_attr_val[QOS_PROTO], "TCP")) ||
					   (!strcmp(qos_attr_val[QOS_PROTO], "UDP")) ||
					   (!strcmp(qos_attr_val[QOS_PROTO], "TCP/UDP"))){
						sprintf(string_tmp," --ip-sport %s:%s", qos_attr_val[QOS_SRCPORTBEGIN], qos_attr_val[QOS_SRCPORTEND]);
						strcat(string_ipv4, string_tmp);
					}
				}
				break;
			case QOS_IPPEND:
				if((qos_check[QOS_IPPBEGIN] == 1) && (qos_check[QOS_IPPEND] == 1)){
					sprintf(string_tmp," --ip-ipp %d:%d", (unsigned short int)strtoul(qos_attr_val[QOS_IPPBEGIN], &end, 10), (unsigned short int)strtoul(qos_attr_val[QOS_IPPEND], &end , 10));
					strcat(string_ipv4, string_tmp);
				}
				break;
			case QOS_TOS:
				if(!strcmp(qos_attr_val[QOS_TOS], "Minimize monetary cost"))
					strcat(string_ipv4," --ip-tos 0x02");
				else if(!strcmp(qos_attr_val[QOS_TOS], "Maximize reliability"))
					strcat(string_ipv4," --ip-tos 0x04");
				else if(!strcmp(qos_attr_val[QOS_TOS], "Maximize throughput"))
					strcat(string_ipv4," --ip-tos 0x08");
				else if(!strcmp(qos_attr_val[QOS_TOS], "Minimize delay"))
					strcat(string_ipv4," --ip-tos 0x10");
				else  /*Normal service*/
					strcat(string_ipv4," --ip-tos 0x00");
				break;
			case QOS_DSCPEND:
				if((qos_check[QOS_DSCPBEGIN] == 1) && (qos_check[QOS_DSCPEND] == 1)){
					sprintf(string_tmp," --ip-dscp %d:%d", (unsigned short int)strtoul(qos_attr_val[QOS_DSCPBEGIN], &end, 10), (unsigned short int)strtoul(qos_attr_val[QOS_DSCPEND], &end, 10));
					strcat(string_ipv4, string_tmp);
				}
				break;
			default:
				break;
			}
		}
	}
	len_string_ipv4 = strlen(string_ipv4);

#ifdef MBSS_SUPPORT
	for(i=QOS_PHYPORTETH0 ; i<=QOS_PHYPORTWLAN3 ; i++){
#else
	for(i=QOS_PHYPORTETH0 ; i<=QOS_PHYPORTWLAN0 ; i++){
#endif
		if(qos_check[i]){
			if((len_string_ipv4 == 0) && (len_string_mac == 0)){
				sprintf(string_total,"%s -A qos_IPv4rule%d -i %s -j qos_8021Qrule%d\n",QOS_BROUTE, index, qos_physical_name[i], index);
			}
			else if((len_string_ipv4 != 0) && (len_string_mac == 0)){
				sprintf(string_total,"%s -A qos_IPv4rule%d -i %s -p IPv4 %s -j qos_8021Qrule%d\n",QOS_BROUTE, index, qos_physical_name[i], string_ipv4, index);
			}
			else if((len_string_ipv4 == 0) && (len_string_mac != 0)){
				sprintf(string_total,"%s -A qos_IPv4rule%d -i %s %s -j qos_8021Qrule%d\n",QOS_BROUTE, index, qos_physical_name[i], string_mac, index);
			}
			else{
				sprintf(string_total,"%s -A qos_IPv4rule%d -i %s %s -p IPv4 %s -j qos_8021Qrule%d\n",QOS_BROUTE, index, qos_physical_name[i], string_mac, string_ipv4, index);
		}
			fputs(string_total, fp);
			flag_phy = 1;
			len_string_total = strlen(string_total);	
		}
	}
/*
 	if(len_string_total != 0)  //at least one physical port is selected
		fputs(string_total, fp);
	else{ //no physical port is selected
*/
	if(flag_phy == 0){
		if((len_string_ipv4 == 0) && (len_string_mac == 0))
			sprintf(string_total,"%s -A qos_IPv4rule%d -j qos_8021Qrule%d\n",QOS_BROUTE, index, index);
		else if((len_string_ipv4 != 0) && (len_string_mac == 0))
			sprintf(string_total,"%s -A qos_IPv4rule%d -p IPv4 %s -j qos_8021Qrule%d\n",QOS_BROUTE, index, string_ipv4, index);
		else if((len_string_ipv4 == 0) && (len_string_mac != 0))
			sprintf(string_total,"%s -A qos_IPv4rule%d %s -j qos_8021Qrule%d\n",QOS_BROUTE, index, string_mac, index);
		else
			sprintf(string_total,"%s -A qos_IPv4rule%d %s -p IPv4 %s -j qos_8021Qrule%d\n",QOS_BROUTE, index, string_mac, string_ipv4, index);
		fputs(string_total, fp);
	}

	if((flag_phy) || (len_string_mac != 0) || (len_string_ipv4 != 0)){
		sprintf(string_total,"%s -A qos_IPv4rule%d -j %s\n",QOS_BROUTE, index, jumpchain);
		fputs(string_total, fp);
	}
}
/*______________________________________________________________________________
**	qos_8021Qrule
**
**	descriptions:
**		create the qos rule related to 802.1Q parameter.
**
**	parameters:
**             fp:  File pointer
**          index:  Index of current chain
**      jumpchain:  Name of chain for jump
**
**	return:
**
**____________________________________________________________________________
*/
void
qos_8021Qrule(FILE *fp, int index, char *jumpchain){
	char string_8021q[128]={0}, string_tmp[128]={0}, string_total[576]={0}, *end;
	unsigned short i, len_string_8021q;

	for(i=QOS_VLANIDBEGIN ; i<=QOS_VLANPRIOEND; i++){
		if(qos_check[i] == 1){
			switch(i){
				case QOS_VLANIDEND:
					if((qos_check[QOS_VLANIDBEGIN] == 1) && (qos_check[QOS_VLANIDEND] == 1)){
						sprintf(string_tmp," --vlan-id %d:%d", (unsigned int)strtoul(qos_attr_val[QOS_VLANIDBEGIN], &end, 0), (unsigned int)strtoul(qos_attr_val[QOS_VLANIDEND], &end, 0));
						strcat(string_8021q, string_tmp);
					}
					break;
				case QOS_VLANPRIOEND:
					if((qos_check[QOS_VLANPRIOBEGIN] == 1) && (qos_check[QOS_VLANPRIOEND] == 1)){
						sprintf(string_tmp," --vlan-prio %d:%d", (unsigned short int)strtoul(qos_attr_val[QOS_VLANPRIOBEGIN], &end, 0), (unsigned short int)strtoul(qos_attr_val[QOS_VLANPRIOEND], &end, 0));
						strcat(string_8021q, string_tmp);
					}
					break;
				default:
					break;
			}
		}
	}
	len_string_8021q = strlen(string_8021q);

	if(len_string_8021q == 0){
		sprintf(string_total,"%s -A qos_8021Qrule%d -j qos_remarking_rule%d\n",QOS_BROUTE, index, index);
		fputs(string_total, fp);
	}
	else{
		sprintf(string_total,"%s -A qos_8021Qrule%d -p 802_1Q %s -j qos_remarking_rule%d\n",QOS_BROUTE, index, string_8021q, index);
		fputs(string_total, fp);
		sprintf(string_total,"%s -A qos_8021Qrule%d -j %s\n",QOS_BROUTE, index, jumpchain);
		fputs(string_total, fp);
	}
}
/*______________________________________________________________________________
**	qos_moduleInsert
**
**	descriptions:
**		insert module related to qos.
**
**	parameters:
**		none.
**
**	return:
**      none.
**____________________________________________________________________________
*/
void qos_moduleInsert(){
	int i;
	char stringCmd[128]={0};
	char qos_module_name[QOS_MODULE_NUM][32]=
	{
#if defined(TCSUPPORT_CT_VLAN_BIND)
		{"ebt_ip.ko"},{"ebt_ftos.ko"}
#else
		{"ebtables.ko"},{"ebtable_broute.ko"},{"ebt_ip.ko"},
		{"ebt_vlan.ko"},{"ebt_ftos.ko"},{"ebt_mark.ko"}
#endif
	};
	char path[100], vername[15];
	extern void decideModulePath(char*);

	decideModulePath(vername);
	sprintf(path, QOS_MODULE_PATH, vername);
	for(i=0 ; i<QOS_MODULE_NUM ; i++){
		sprintf(stringCmd,"insmod %s%s\n", path, qos_module_name[i]);
		system(stringCmd);
	}
}
/*______________________________________________________________________________
**	qos_moduleRemove
**
**	descriptions:
**		Remove module related to qos.
**
**	parameters:
**		none.
**
**	return:
**      none.
**____________________________________________________________________________
*/
void qos_moduleRemove(){
	int i;
	char stringCmd[64]={0};
	char qos_module_name[QOS_MODULE_NUM][32]=
	{
#if defined(TCSUPPORT_CT_VLAN_BIND)
		{"ebt_ip.ko"},{"ebt_ftos.ko"}
#else
		{"ebt_mark.ko"},{"ebt_ftos.ko"},{"ebt_vlan.ko"},
		{"ebt_ip.ko"},{"ebtable_broute.ko"},{"ebtables.ko"}
#endif
	};

	for(i=0 ; i<QOS_MODULE_NUM ; i++){
		sprintf(stringCmd,"rmmod %s\n", qos_module_name[i]);
		system(stringCmd);
	}
}
/*______________________________________________________________________________
**	qos_init
**
**	descriptions:
**		init function of qos.
**
**	parameters:
**		none.
**
**	return:
**		0 means success
**		1 means fail
**
**____________________________________________________________________________
*/
int
qos_init(void){
	//	tcdbg_printf("%s ",__func__);
		int ret=0;
		cfg_node_t node={
				.name="QoS",
				.type=ADV_FLAG|ENTRY_TYPE|MAX_QOS_RULE,
				.adv_table=qos_adv_table,
				.merge_table=qos_merge_table,
				.cfg_read=NULL,
				.cfg_write=qos_write,
				.cfg_verify=NULL,
				.cfg_execute=qos_execute,
				.cfg_boot=qos_boot,
		};
		ret=register_cfg_node(&node);
		return ret;
}
/*______________________________________________________________________________
**	qos_boot
**
**	descriptions:
**		the boot function of qos.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
qos_boot(mxml_node_t *top){
	char startupCmd[48]={0}, tmp[32], nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char discipline[32], wrr_pri[4][32];

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],QOS);
	strcpy(nodeName[1],QOS_COMMON);

	#ifdef QOS_DROP  /*Rodney_20091115*/
	qos_drop_reset(top);
	#endif

	if(getAttrValue(top, nodeName, "Discipline", discipline) == TCAPI_PROCESS_OK){
		if(strcmp(discipline,"SP") == 0) /*strict priority*/
            if(TCSUPPORT_CPU_EN75XX_VAL) {
    			sprintf(startupCmd, "0 0 0 0 0");
            } else {
		        sprintf(startupCmd,"echo 0 0 0 0 0 > /proc/tc3162/tsarm_qoswrr\n");
            }
		else{  /*WRR*/
			if(getAttrValue(top, nodeName, "WeightHighest", wrr_pri[0]) == TCAPI_PROCESS_OK)
				if(getAttrValue(top, nodeName, "WeightHigh", wrr_pri[1]) == TCAPI_PROCESS_OK)
					if(getAttrValue(top, nodeName, "WeightMedium", wrr_pri[2]) == TCAPI_PROCESS_OK)
						if(getAttrValue(top, nodeName, "WeightLow", wrr_pri[3]) == TCAPI_PROCESS_OK)
                            if(TCSUPPORT_CPU_EN75XX_VAL) {
            					sprintf(startupCmd, "1 %s %s %s %s", wrr_pri[0], wrr_pri[1], wrr_pri[2], wrr_pri[3]);
                            } else {
            					sprintf(startupCmd,"echo 1 %s %s %s %s > /proc/tc3162/tsarm_qoswrr\n",wrr_pri[0], wrr_pri[1], wrr_pri[2], wrr_pri[3]);
                            }
		}
        if(TCSUPPORT_CPU_EN75XX_VAL) {
			doValPut("/proc/qdma_wan/mac_qos", startupCmd);
        } else {
			system(startupCmd);
        }
	}

	if(getAttrValue(top, nodeName, NODE_ACTIVE, tmp) == TCAPI_PROCESS_OK){
		if(strcmp(tmp,"Yes") == 0){ /*QoS active*/
			qos_moduleInsert();
			sprintf(startupCmd,"chmod 777 %s\n",QOS_SH);
			system(startupCmd);
			sprintf(startupCmd,"%s\n",QOS_SH);
			system(startupCmd);
		}
	}

	return SUCCESS;
}
/*______________________________________________________________________________
**	qos_write
**
**	descriptions:
**		Find qos element from mxml tree then write qos's system parameter to specify
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
qos_write(mxml_node_t *top, mxml_node_t *parant){
	char tmp[32], nodeName[MAX_ARG_NUM][MAX_NODE_NAME], wrr_pri[4][32];
	char startupCmd[128]={0}, jumpchain[32]={0}, discipline[32];
	FILE *fp;
	int i, next_index, final_index;
	static int qos_DowntoUp = 1, qos_UptoDown = 0;
	char qos_attr_name[QOS_ATTRLEN][32]=
	{
		{"eth0"},{"eth1"},{"eth2"},{"eth3"},{"usb0"},{"ra0"},
#ifdef MBSS_SUPPORT
		{"ra1"},{"ra2"},{"ra3"},
#endif
		{"DesMac"},{"SrcMac"},
		{"ProtocolID"},{"DesIP"},{"DesMask"},{"DesPortRangeBegin"},{"DesPortRangeEnd"},
		{"SrcIP"},{"SrcMask"},{"SrcPortRangeBegin"},{"SrcPortRangeEnd"},
		{"IPPRangeBegin"},{"IPPRangeEnd"},{"TypeOfService"},{"DSCPRangeBegin"},{"DSCPRangeEnd"},
		{"VlanIDRangeBegin"},{"VlanIDRangeEnd"},{"dotpBegin"},{"dotpEnd"},
		{"ActIPPRemarking"},{"ActToSRemarking"},{"ActDSCPRemarking"},
		{"Act8021pRemarkingNum"},{"ActQueue"}
	};

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],QOS);
	strcpy(nodeName[1],QOS_COMMON);
	if(getAttrValue(top, nodeName, "DisciplineFlag", tmp) == TCAPI_PROCESS_OK){
		if(strcmp(tmp,ACTIVE) == 0){
			#ifdef QOS_DROP  /*Rodney_20091115*/
			qos_drop_reset(top);
			#endif
			if(getAttrValue(top, nodeName, "Discipline", discipline) == TCAPI_PROCESS_OK){
				if(strcmp(discipline,"SP") == 0) /*strict priority*/
                    if(TCSUPPORT_CPU_EN75XX_VAL) {
            			sprintf(startupCmd, "0 0 0 0 0");
                    } else {
    					sprintf(startupCmd,"echo 0 0 0 0 0 > /proc/tc3162/tsarm_qoswrr\n");
                    }
				else{  /*WRR*/
					if(getAttrValue(top, nodeName, "WeightHighest", wrr_pri[0]) == TCAPI_PROCESS_OK)
						if(getAttrValue(top, nodeName, "WeightHigh", wrr_pri[1]) == TCAPI_PROCESS_OK)
							if(getAttrValue(top, nodeName, "WeightMedium", wrr_pri[2]) == TCAPI_PROCESS_OK)
								if(getAttrValue(top, nodeName, "WeightLow", wrr_pri[3]) == TCAPI_PROCESS_OK)
                                    if(TCSUPPORT_CPU_EN75XX_VAL) {
                    					sprintf(startupCmd, "1 %s %s %s %s", wrr_pri[0], wrr_pri[1], wrr_pri[2], wrr_pri[3]);
                                    } else {
        						        sprintf(startupCmd,"echo 1 %s %s %s %s > /proc/tc3162/tsarm_qoswrr\n",wrr_pri[0], wrr_pri[1], wrr_pri[2], wrr_pri[3]);
                                    }
				}
		        if(TCSUPPORT_CPU_EN75XX_VAL) {
					doValPut("/proc/qdma_wan/mac_qos", startupCmd);
		        } else {
					system(startupCmd);
		        }
			}
			return SUCCESS;
		}
	}

	/*open qos_rule.sh to write*/
	fp = fopen(QOS_SH, "w+");
	if(fp == NULL){
		fprintf(stderr, "qos_rule.sh file pointer is NULL.\n");
		return FAIL;
	}

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],QOS);
	strcpy(nodeName[1],QOS_COMMON);
	if(getAttrValue(top, nodeName, NODE_ACTIVE, tmp) == TCAPI_PROCESS_OK){
		if(strcmp(tmp,"Yes") == 0){ /*QoS active*/
			final_index = -1;
			next_index = -1;
			if(qos_DowntoUp){
				sprintf(startupCmd,"%s -N qos_start_chain\n",QOS_BROUTE);
				fputs(startupCmd, fp);
				sprintf(startupCmd,"%s -N qos_remarking_default\n",QOS_BROUTE);
				fputs(startupCmd, fp);
				sprintf(startupCmd,"%s -I BROUTING -i ! nas0 -j qos_start_chain\n",QOS_BROUTE);
				fputs(startupCmd, fp);
			}
			else{ /*Flush start chain*/
				sprintf(startupCmd,"%s -F qos_start_chain\n",QOS_BROUTE);
				fputs(startupCmd, fp);
				sprintf(startupCmd,"%s -F qos_remarking_default\n",QOS_BROUTE);
				fputs(startupCmd, fp);
			}
			memset(nodeName,0,sizeof(nodeName));
			strcpy(nodeName[0],QOS);
			for(i=(MAX_QOS_RULE-1) ; i>=0 ; i--){  /*create qos rule 0~15*/
				/*flush each chain before adding rule to make sure old rules are deleted*/
				if(qos_DowntoUp){
					sprintf(startupCmd,"%s -N qos_IPv4rule%d\n",QOS_BROUTE,i);
					fputs(startupCmd, fp);
					sprintf(startupCmd,"%s -N qos_8021Qrule%d\n",QOS_BROUTE,i);
					fputs(startupCmd, fp);
					sprintf(startupCmd,"%s -N qos_remarking_rule%d\n",QOS_BROUTE,i);
					fputs(startupCmd, fp);
				}
				else{
					sprintf(startupCmd,"%s -F qos_IPv4rule%d\n",QOS_BROUTE,i);
					fputs(startupCmd, fp);
					sprintf(startupCmd,"%s -F qos_8021Qrule%d\n",QOS_BROUTE,i);
					fputs(startupCmd, fp);
					sprintf(startupCmd,"%s -F qos_remarking_rule%d\n",QOS_BROUTE,i);
					fputs(startupCmd, fp);
				}
				sprintf(nodeName[1],"Entry%d",i);
				if(getAttrValue(top, nodeName, NODE_ACTIVE, tmp) == TCAPI_PROCESS_OK){
					if(strcmp(tmp,"Yes") == 0){ /*Entry$i active*/
						if(i>=final_index){
							final_index = i;
							sprintf(jumpchain,"qos_remarking_default");
						}
						else
							sprintf(jumpchain,"qos_IPv4rule%d",next_index);

						if(qos_paramcheck(top, nodeName, qos_attr_name, i)){
							qos_IPv4rule(fp, i, jumpchain);
							qos_8021Qrule(fp, i, jumpchain);
							qos_remarking(fp, i, jumpchain);
							next_index = i;
						}
						else
							tcdbg_printf("qos rule %d parameter check fail!!\n", i);
					}/*end if*/
				}/*end if*/
			}/*end for*/
			if(next_index >= 0){ /*It mean at least one rule is active and valid*/
				sprintf(startupCmd,"%s -A qos_start_chain -j qos_IPv4rule%d\n",QOS_BROUTE,next_index);
				fputs(startupCmd, fp);
			}

			sprintf(startupCmd,"%s -I qos_remarking_default -j mark --mark-set 0x00\n",QOS_BROUTE);  /*000*/
			fputs(startupCmd, fp);
			qos_DowntoUp = 0;
			qos_UptoDown = 1;
		}/*end if*/
		else{  /*QoS inactive. delete all QoS chain*/
			if(qos_UptoDown){
				sprintf(startupCmd,"%s -D BROUTING -i ! nas0 -j qos_start_chain\n",QOS_BROUTE);
				fputs(startupCmd, fp);
				sprintf(startupCmd,"%s -X qos_start_chain\n",QOS_BROUTE);
				fputs(startupCmd, fp);
				for(i=0 ; i<MAX_QOS_RULE ; i++){
					sprintf(startupCmd,"%s -X qos_IPv4rule%d\n",QOS_BROUTE,i);
					fputs(startupCmd, fp);
					sprintf(startupCmd,"%s -X qos_8021Qrule%d\n",QOS_BROUTE,i);
					fputs(startupCmd, fp);
					sprintf(startupCmd,"%s -X qos_remarking_rule%d\n",QOS_BROUTE,i);
					fputs(startupCmd, fp);
				}
				sprintf(startupCmd,"%s -X qos_remarking_default\n",QOS_BROUTE);
				fputs(startupCmd, fp);
			}
			qos_DowntoUp = 1;
			qos_UptoDown = 0;
		}
	}/*end if*/

	fclose(fp);

	return SUCCESS;
}/* end qos_write */

/*_____________________________________________________________________________
**      function name: qos_execute
**      descriptions:
**            To execute qos node service.
**
**      parameters:
**            node:   Put the qos of cfg node tree that is used to execute qos settings.
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
qos_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	char tmp[32], startupCmd[48]={0}, nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	static int qos_DowntoUp = 1, qos_UptoDown = 1;

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],QOS);
	strcpy(nodeName[1],QOS_COMMON);

	if(getAttrValue(top, nodeName, "DisciplineFlag", tmp) == TCAPI_PROCESS_OK){
		if(strcmp(tmp,ACTIVE) == 0){
			setAttrValue(top, nodeName, "DisciplineFlag", DEACTIVE);
			system("/userfs/bin/hw_nat -!");
			return SUCCESS;
		}
	}

	if(getAttrValue(top, nodeName, NODE_ACTIVE, tmp) == TCAPI_PROCESS_OK){
		if(strcmp(tmp,"Yes") == 0){ /*QoS active. Insert module then create rule*/
			if(qos_DowntoUp)
				qos_moduleInsert();
			sprintf(startupCmd,"chmod 777 %s\n",QOS_SH);
			system(startupCmd);
			sprintf(startupCmd,"%s\n",QOS_SH);
			system(startupCmd);
			qos_DowntoUp = 0;
			qos_UptoDown = 1;
		}
		else{  /*QoS inactive. Flush rule then remove module*/
			sprintf(startupCmd,"chmod 777 %s\n",QOS_SH);
			system(startupCmd);
			sprintf(startupCmd,"%s\n",QOS_SH);
			system(startupCmd);
			sprintf(startupCmd,"rm -r %s\n",QOS_SH);
			system(startupCmd);
			if(qos_UptoDown)
				qos_moduleRemove();
			qos_DowntoUp = 1;
			qos_UptoDown = 0;
		}
	}
	system("/userfs/bin/hw_nat -!");
	return SUCCESS;
}/* end qos_execute*/

#endif
#endif
/**********20080825 racing add************/

#if 0  /*Orginal ci-command mode */
/*______________________________________________________________________________
**	qos_read
**
**	descriptions:
**		Read qos system parameter from configuration file and record those information
**		to mxml_node_t.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
qos_read(mxml_node_t *top){
	return 0;
}/* end qos_read */


/*______________________________________________________________________________
**	qos_write
**
**	descriptions:
**		Find qos element from mxml tree then write qos's system parameter to specify
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
qos_write(mxml_node_t *top, mxml_node_t *parant){
	return SUCCESS;
}/* end qos_write */

/*______________________________________________________________________________
**	qos_verify
**
**	descriptions:
**		To check the system parameter of qos element  is validly or not.
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
qos_verify(mxml_node_t *node){
	return SUCCESS;
}/* end qos_verify */

/*_____________________________________________________________________________
**      function name: qos_execute
**      descriptions:
**            To execute qos node service.
**
**      parameters:
**            node:   Put the qos of cfg node tree that is used to execute qos settings.
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
qos_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	return SUCCESS;
}/* end qos_execute*/
#endif
/**********20080825 racing add************/
char *
autopvc_adv_table[MAX_NODE_NAME]=
{
	"Common","",
};
char *
autopvc_merge_table[MAX_NODE_NAME]=
{
	"autopvc_id","",
};

/*______________________________________________________________________________
**	autopvc_init
**
**	descriptions:
**		init function of autopvc.
**
**	parameters:
**		none.
**
**	return:
**		0 means success
**		1 means fail
**____________________________________________________________________________
*/
int
autopvc_init(void){
	tcdbg_printf("%s\n",__func__);
	int ret=0;
	cfg_node_t node={
			.name="AutoPVC",
			.type=ADV_FLAG|ENTRY_TYPE|MAX_VCPOOL_NUM,
			.adv_table=autopvc_adv_table,
			.merge_table=autopvc_merge_table,
			.cfg_read=NULL,
			.cfg_write=autopvc_write,
			.cfg_verify=autopvc_verify,
			.cfg_execute=autopvc_execute,
			.cfg_boot=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}

int
autopvc_verify(mxml_node_t *node){
	return SUCCESS;
}

int
autopvc_write(mxml_node_t *top, mxml_node_t *parant){
	return SUCCESS;
}

/*global data for autopvc and cc*/
pvc_pool_t g_pvcPool[PVC_NUM];
vc_pool_t g_vcPool[MAX_VCPOOL_NUM];
unsigned long g_serviceFlag=0;
int g_vcPool_encap=0;

/*______________________________________________________________________________
**	autopvc_execute
**
**	descriptions:
**		do the start up action and the first probe.
**
**	parameters:
**		top:root of node tree.
**		name:node name(AutoPVC)
**	return:
**		0 means success
**		1 means fail
**____________________________________________________________________________
*/
int
autopvc_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){

	int i=0;
	char tmp[32],nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char lan_mac[128]={0};
	char *p=NULL;
	FILE *fd;

	/*read mac*/
	fd=fopen(TMP_MAC_PATH,"r");
	if(!fd){
		tcdbg_printf("no lan pc mac! autopvc stop\n");
		return FAIL;
	}
	else{
		fgets(lan_mac,sizeof(lan_mac),fd);
		if(strlen(lan_mac) == 0){
			tcdbg_printf("no lan pc mac! autopvc stop\n");
			return FAIL;
		}
		p=lan_mac;
		for(i=0;i<strlen(lan_mac);i++){
			if(p[i]==':'){
				p[i]=' ';
			}
		}
		fclose(fd);
	}
	/*clear buffer*/
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],AUTOPVC);
	strcpy(nodeName[1],STRING_COMMON);
	/*see if autopvc is open*/
	if(getAttrValue(top, nodeName, NODE_ACTIVE, tmp)!=SUCCESS){
		tcdbg_printf("can't get autopvc's active\n");
		return FAIL;
	}
	else if(!strcmp(tmp,"0")){//Active=0 means not open
		tcdbg_printf("autopvc is not active\n");
		return SUCCESS;
	}
	//0.reset anything
	reset_all_autopvc_data();
	//1.get current pvc setting
	if(get_current_pvc_setting(top)!=SUCCESS){
		tcdbg_printf("pvc setting fail\n");
		return FAIL;
	}
	//2.get autopvc setting(vc pool, service flag, encapsulation)
	if(get_autopvc_setting(top)!=SUCCESS){
		tcdbg_printf("autopvc setting fail\n");
		return FAIL;
	}
	system("sys autopvc active 1");
	//4.pass pvc data to sar driver
	for(i=0;i<PVC_NUM;i++){
		memset(tmp, 0, sizeof(tmp));
		if(/*(g_pvcPool[i].vci == 0) ||*/ (g_pvcPool[i].state == AUTOPVC_STOP)){/*not pass the setting if vci is 0*/
			continue;
		}
		sprintf(tmp,"%s %s %d %d %d %d",AUTOPVC_SET_CMD,"pvc",i,g_pvcPool[i].vpi,g_pvcPool[i].vci,g_pvcPool[i].encap);
		system(tmp);
//		tcdbg_printf("%s\n",tmp);
	}

	/*send service flag to sar*/
	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp,"%s %lx",AUTOPVC_SERVICE_CMD, g_serviceFlag);
	system(tmp);
	/*send encap flag to sar*/
	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp,"%s %d",AUTOPVC_ENCAP_CMD, g_vcPool_encap);
	system(tmp);
	/*send mac addr to sar*/
	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp,"%s %s %s",AUTOPVC_SET_CMD,"mac",lan_mac);
	system(tmp);
	tcdbg_printf("%s\n",tmp);

	/*pass vcpool data to sar driver*/
	for(i=0;i<MAX_VCPOOL_NUM;i++){
		memset(tmp, 0, sizeof(tmp));
		if(g_vcPool[i].vci == 0){
			continue;
		}
		sprintf(tmp,"%s %s %d %d %d",AUTOPVC_SET_CMD,"vcpool",i,g_vcPool[i].vpi,g_vcPool[i].vci);
		system(tmp);
	}
	system("sys autopvc probe pvc");


	return SUCCESS;
}

/*______________________________________________________________________________
**	check_vpi_vci
**
**	descriptions:
**		check if the vpi and vci is in right range.
**
**	parameters:
**		vpi:vpi.
**		vci:vci
**	return:
**		0 means SUCCESS
**		-1 means FAIL
**____________________________________________________________________________
*/
int check_vpi_vci(int vpi, int vci){
	if( (vpi<0) || (vpi>255)){
		return FAIL;
	}
	else if((vci<1)||(vci>65535)){
		return FAIL;
	}
	else{
		return SUCCESS;
	}
}

/*______________________________________________________________________________
**	get_current_pvc_setting
**
**	descriptions:
**		get the setting of current PVC.
**
**	parameters:
**		top:root of node tree.
**	return:
**		0 means SUCCESS
**		-1 means FAIL
**____________________________________________________________________________
*/
int get_current_pvc_setting(mxml_node_t *top){
	int i=0,j=0,active_pvc=0;
	char tmp[32],nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char reserved[32];

	char keyword[][ATTR_SIZE]={
			{"Active"},
			{"VPI"},
			{"VCI"},
			{"ENCAP"},
			{""},
	};

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],WAN);
	for(i=0; i<PVC_NUM ; i++){//all pvc
		memset(nodeName[1],0,sizeof(nodeName[1]));
		memset(reserved,0,sizeof(reserved));
		sprintf(nodeName[1],SUB_PVC_NAME,i);
		getAttrValue(top, nodeName, WAN_RESERVED, reserved);
		for(j=0; strlen(keyword[j]) != 0 ; j++){//all keywords
				memset(tmp, 0, sizeof(tmp));
				getAttrValue(top, nodeName, keyword[j], tmp);
				if(!strlen(tmp)){
					continue;
				}
				switch(j){
					case 0://active
						if((!strcmp(tmp,"Yes")) &&//check active and reserved .
								((!strlen(reserved)) || (strcmp(reserved,"No") == 0))){
							g_pvcPool[i].state=AUTOPVC_START;
							active_pvc++;
//							tcdbg_printf("pvc %d start\n",i);
						}
						break;
					case 1://vpi
						g_pvcPool[i].vpi=atoi(tmp);
						break;
					case 2://vci
						g_pvcPool[i].vci=atoi(tmp);
						if((g_pvcPool[i].state==AUTOPVC_START) &&
							(check_vpi_vci(g_pvcPool[i].vpi, g_pvcPool[i].vci) == FAIL)){
							g_pvcPool[i].state=AUTOPVC_STOP;
							active_pvc--;
//							tcdbg_printf("pvc %d stop\n",i);
						}
						break;
					case 3://encap
						if(strstr(tmp,"VC")){
							g_pvcPool[i].encap=ENCAP_VC;
						}
						else{//default llc
							g_pvcPool[i].encap=ENCAP_LLC;
						}
						break;
					default:
						break;
				}
		}
	}
	//if no active pvc , return fail and autopvc will terminate.
	if(active_pvc == 0){
		tcdbg_printf("in %s: no active pvc\n",__func__);
		return FAIL;
	}
	else{
		return SUCCESS;
	}
}

/*______________________________________________________________________________
**	get_autopvc_setting
**
**	descriptions:
**		get the setting of vcpool.
**
**	parameters:
**		top:root of node tree.
**	return:
**		0 means SUCCESS
**		-1 means FAIL
**____________________________________________________________________________
*/
int get_autopvc_setting(mxml_node_t *top){
	int i=0,j=0;
	char tmp[32],nodeName[MAX_ARG_NUM][MAX_NODE_NAME];

	char service[][ATTR_SIZE]={
			{"Probe_OAM"},
			{"Probe_PPPOE"},
			{"Probe_ARP"},
			{"Probe_DHCP"},
			{""},
	};

	char keyword[][ATTR_SIZE]={
			{"PVC"},
			{"VPI"},
			{"VCI"},
			{""},
	};

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],AUTOPVC);
	strcpy(nodeName[1],STRING_COMMON);
	for(i=0; strlen(service[i]) != 0; i++){//service flag
		memset(tmp,0,sizeof(tmp));
		if(getAttrValue(top, nodeName, service[i], tmp)!=SUCCESS){
			tcdbg_printf("in %s: can't get service flag\n",__func__);
			return FAIL;
		}
		if(atoi(tmp) == 1){
			g_serviceFlag|=(1<<i);
		}
	}

	memset(tmp,0,sizeof(tmp));
	if(getAttrValue(top, nodeName, "Encap", tmp)!=SUCCESS){//encap
		tcdbg_printf("in %s: can't get Encap\n",__func__);
		return FAIL;
	}
	if(atoi(tmp)!=0){//if the setting of encap is not 0, we set it to be 1.
		g_vcPool_encap=1;
	}
	/*get the setting of vcpool*/
	for(i=0; i<MAX_VCPOOL_NUM ; i++){
		memset(nodeName,0,sizeof(nodeName));
		strcpy(nodeName[0],AUTOPVC);
		sprintf(nodeName[1],"Entry%d",i);
		for(j=0; strlen(keyword[j]) != 0; j++){
				memset(tmp, 0, sizeof(tmp));
				getAttrValue(top, nodeName,keyword[j], tmp);
				if(!strlen(tmp)){
					continue;
				}
				switch(j){
					case 0://pvc
						g_vcPool[i].pvc=atoi(tmp);
						break;
					case 1://vpi
						g_vcPool[i].vpi=atoi(tmp);
						break;
					case 2://vci
						g_vcPool[i].vci=atoi(tmp);
						break;
					default:
						break;
				}
		}
	}
	return SUCCESS;
}

/*______________________________________________________________________________
**	check_adsl_stat
**
**	descriptions:
**		check the adsl state now.
**
**	parameters:
**		none
**	return:
**		0 means SUCCESS
**		-1 means FAIL
**____________________________________________________________________________
*/
//int check_adsl_stat(void){
//	int ret=0, fd=0;
//	char string[64]={0};
//	fd = open("/proc/tc3162/adsl_stats",O_RDONLY|O_NONBLOCK);
//	if( fd <= 0 ){
//		tcdbg_printf("in %s: can't open\n",__func__);
//		return FAIL;
//	}
//	ret = read(fd, string, 64);
//	close(fd);
//	if( ret <= 0 ){
//		tcdbg_printf("in %s: can't read\n",__func__);
//		return FAIL;
//	}
//	if(!strstr(string, "up")){
//		tcdbg_printf("in %s: adsl is down\n",__func__);
//		return FAIL;
//	}
//	return SUCCESS;
//}

/*______________________________________________________________________________
**	reset_all_autopvc_data
**
**	descriptions:
**		clear all global buffer.
**
**	parameters:
**		none
**	return:
**		none
**____________________________________________________________________________
*/
void
reset_all_autopvc_data(){
	memset(g_pvcPool, 0, sizeof(g_pvcPool));
	memset(g_vcPool, 0, sizeof(g_vcPool));
	g_serviceFlag=0;
	g_vcPool_encap=0;
}
#if 0//def TCSUPPORT_PORTBIND
/* portbind xyzhu_nj */
char *
portbind_adv_table[MAX_NODE_NAME]=
{
	"Common","",
};
char *
portbind_merge_table[MAX_NODE_NAME]=
{
	"portbind_id","",
};

/*__________________________________________________
** This table mapping wep interface name to actually 
** interface name, if interface name changed, it 
** should update this table. 
** eg. for signal port CPE, update to "e0 eth0"
**__________________________________________________
*/

struct if_map if_map_data[]=
{
	{"p0", "nas0"},
	{"p1", "nas1"},
	{"p2", "nas2"},
	{"p3", "nas3"},
	{"p4", "nas4"},
	{"p5", "nas5"},
	{"p6", "nas6"},
	{"p7", "nas7"},
#ifdef TCSUPPORT_TC2031
	/* for signal port */
	{"e0", "eth0"},
#else
   /* for four ports */
	{"e1", "eth0.1"},
	{"e2", "eth0.2"},
	{"e3", "eth0.3"},
	{"e4", "eth0.4"},
#endif
	{"w0", "ra0"},
	{"w1", "ra1"},
	{"w2", "ra2"},
	{"w3", "ra3"},
	{"u0", "usb0"}
};
/*______________________________________________________________________________
**	portbind_init
**
**	descriptions:
**		init function of autopvc.
**
**	parameters:
**		none.
**
**	return:
**		0 means success
**		1 means fail
**____________________________________________________________________________
*/
int
portbind_init(void){
	tcdbg_printf("%s\n",__func__);
	int ret=0;
	cfg_node_t node={
			.name="PortBind",
			.type=ADV_FLAG|ENTRY_TYPE|MAX_PORTBIND_NUM,
			.adv_table=portbind_adv_table,
			.merge_table=portbind_merge_table,
			.cfg_read=NULL,
			.cfg_write=portbind_write,
			.cfg_verify=portbind_verify,
			.cfg_execute=portbind_execute,
			.cfg_boot=portbind_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}

/*______________________________________________________________________________
**	portbind_write
**
**	descriptions:
**		syn entry node to common node.
**
**	parameters:
**		none.
**
**	return:
**		0 means success
**		1 means fail
**____________________________________________________________________________
*/

int
portbind_write(mxml_node_t *top, mxml_node_t *parant)
{
	syn_portbind_summary(parant);
	return SUCCESS;
}

/*______________________________________________________________________________
**	portbind_verify
**
**	descriptions:
**     check interface node name.		
**
**	parameters:
**		none.
**
**	return:
**		0 means success
**		1 means fail
**____________________________________________________________________________
*/

int
portbind_verify(mxml_node_t *node)
{
	return SUCCESS;
}

/*______________________________________________________________________________
**	portbind_execute
**
**	descriptions:
**     commit this entry node.
**
**	parameters:
**		none.
**
**	return:
**		0 means success
**		1 means fail
**____________________________________________________________________________
*/

int
portbind_execute(mxml_node_t *top, char name[][MAX_NODE_NAME])
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char buf[BUF_LEN];
	char cmd_buf[CMD_BUF_LEN], cmd[CMD_BUF_LEN];
	int cur_entry, i;

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], PORTBIND);
	strcpy(nodeName[1], PORTBIND_COMMON);
	memset(cmd_buf, 0, sizeof(cmd_buf));
	memset(cmd, 0, sizeof(cmd));

	memset(buf, 0, sizeof(buf));
	if (getAttrValue(top, nodeName, "Active", buf) == TCAPI_PROCESS_OK) {
		if (!strcmp(buf, "Yes")) {
			/* portbinding is active */
			strcpy(cmd_buf, "portbindcmd enable");
			exec_portbind_cmd(cmd_buf);
		}
		else {
			strcpy(cmd_buf, "portbindcmd disable");
			exec_portbind_cmd(cmd_buf);
			/* if disable portbinding, just return */
			return SUCCESS;
		}
	}
	else {
		return FAIL;
	}

	/* update the group info */
	if (get_entry_number(name[1], "Entry", &cur_entry) != SUCCESS) {
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0], WEBCURSET);
		strcpy(nodeName[1], SUB_NODE_NAME);

		/* get current entry */
		if (getAttrValue(top, nodeName, "portbind_id", buf) == TCAPI_PROCESS_OK) {
			cur_entry = atoi(buf);
		}
		else {
			return FAIL;
		}
	}

//	tcdbg_printf("cmd buf is %s\n", cmd_buf);
	/* delete bind info of this entry */
	sprintf(cmd_buf, "portbindcmd delgroup %d", cur_entry);
	exec_portbind_cmd(cmd_buf);

	/*_____________________________________________
	**
	** Add bind info of this entry.
	** 1. splice the group interfaces.
	** 2. exec portbindcmd addgroup cmd. 
	**_____________________________________________
	*/
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], "PortBind");
	sprintf(nodeName[1], "Entry%d", cur_entry);

	memset(cmd_buf, 0, sizeof(cmd_buf));
	for (i = 0; i < sizeof(if_map_data)/sizeof(if_map_data[0]); i++) {
		if (getAttrValue(top, nodeName, if_map_data[i].webIfName, buf) == TCAPI_PROCESS_OK) {
			if (!strcmp(buf, "Yes")) {
				if (strlen(cmd_buf) > 0) {
					strcat(cmd_buf, ",");
				}
				strcat(cmd_buf, if_map_data[i].ifName);
			}
		}
	}
	
//	tcdbg_printf("cmd_buf is %s\n", cmd_buf);
	if (strlen(cmd_buf) > 0) {
		sprintf(cmd, "portbindcmd addgroup %d %s", cur_entry, cmd_buf);
		exec_portbind_cmd(cmd);
	}	
	return SUCCESS;
}	

int
portbind_boot(mxml_node_t *top)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char buf[BUF_LEN];
	char cmd_buf[CMD_BUF_LEN],cmd[CMD_BUF_LEN];
	int i,j;

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], PORTBIND);
	strcpy(nodeName[1], PORTBIND_COMMON);
	memset(cmd_buf, 0, sizeof(cmd_buf));
	memset(cmd, 0, sizeof(cmd));

	memset(buf, 0, sizeof(buf));
	if (getAttrValue(top, nodeName, "Active", buf) == TCAPI_PROCESS_OK) {
		if (!strcmp(buf, "Yes")) {
			/* portbinding is active */
			strcpy(cmd_buf, "portbindcmd enable");
			exec_portbind_cmd(cmd_buf);
		}
		/* portbinding is deactive, do nothing */
	}
	else {
		return FAIL;
	}

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], PORTBIND);
	for (i = 0; i < MAX_PORTBIND_NUM; i++) {
		sprintf(nodeName[1], "Entry%d", i); 
		/* check if this entry node is exist */
		if (getAttrValue(top, nodeName, "p0", buf) == TCAPI_NONE_NODE) {
			continue;
		}
		/* this entry node is exist */
		memset(cmd_buf, 0, sizeof(cmd_buf));
		for (j = 0; j < sizeof(if_map_data)/sizeof(if_map_data[0]); j++) {
			if (getAttrValue(top, nodeName, if_map_data[j].webIfName, buf) == TCAPI_PROCESS_OK) {
				if (!strcmp(buf, "Yes")) {
					if (strlen(cmd_buf) > 0) {
						strcat(cmd_buf, ",");
					}
					strcat(cmd_buf, if_map_data[j].ifName);
				}
			}
		}

//		tcdbg_printf("cmd_buf is %s\n", cmd_buf);
		if (strlen(cmd_buf) > 0) {
			sprintf(cmd, "portbindcmd addgroup %d %s", i, cmd_buf);
			exec_portbind_cmd(cmd);
		}
	}	
	return SUCCESS;
}

/*______________________________________________________________________________
**	syn_portbind_summary
**
**	descriptions:
**		syn portbind entry node info to common node.
**
**	parameters:
**		none.
**
**	return:
**		0 means success
**		1 means fail
**____________________________________________________________________________
*/

int syn_portbind_summary(mxml_node_t *node)
{
	mxml_node_t *cldNode = NULL;
	mxml_node_t *commNode = NULL;
	mxml_attr_t *attr=NULL;
	
	int i;

	/* cfg node PortBind */
	if (node == NULL) {
		return FAIL;
	}
	
	cldNode= mxmlWalkNext(node, node, MXML_DESCEND);
	if (cldNode == NULL) {
		return FAIL;
	}
	
	/* find PortBind_Common node */
	while (cldNode) {
		if ((cldNode->type == MXML_ELEMENT) && (cldNode->value.element.name)) {
			if (!strcmp(cldNode->value.element.name, "Common")) {
				commNode = cldNode;
				break;
			}
		}
		
		cldNode = mxmlWalkNext(cldNode, node, MXML_DESCEND);
	}
	if (commNode == NULL) {
		commNode = mxmlNewElement(node, "Common");
		if (commNode == NULL) {
			return FAIL;
		}
	}
	
	/* init PortBind_Common node */
	mxmlElementSetAttr(commNode, "p0", "No");
	mxmlElementSetAttr(commNode, "p1", "No");
	mxmlElementSetAttr(commNode, "p2", "No");
	mxmlElementSetAttr(commNode, "p3", "No");
	mxmlElementSetAttr(commNode, "p4", "No");
	mxmlElementSetAttr(commNode, "p5", "No");
	mxmlElementSetAttr(commNode, "p6", "No");
	mxmlElementSetAttr(commNode, "p7", "No");
	
#ifdef TCSUPPORT_TC2031
	mxmlElementSetAttr(commNode, "e0", "No");
#else
	mxmlElementSetAttr(commNode, "e1", "No");
	mxmlElementSetAttr(commNode, "e2", "No");
	mxmlElementSetAttr(commNode, "e3", "No");
	mxmlElementSetAttr(commNode, "e4", "No");
#endif

	mxmlElementSetAttr(commNode, "w0", "No");
	mxmlElementSetAttr(commNode, "w1", "No");
	mxmlElementSetAttr(commNode, "w2", "No");
	mxmlElementSetAttr(commNode, "w3", "No");

	mxmlElementSetAttr(commNode, "u0", "No");

	/*__________________________________________________________
	**
	** find node PortBind_Entry{i}, and update PortBind_Common
	** from PortBind_Entry{i}
	**__________________________________________________________
	*/
	cldNode= mxmlWalkNext(node, node, MXML_DESCEND);

	while (cldNode) {
		if ((cldNode->type == MXML_ELEMENT) && (cldNode->value.element.name)) {
			if (!strcmp(cldNode->value.element.name, "Common")) {
				cldNode = mxmlWalkNext(cldNode, node, MXML_DESCEND);
				continue;
			}

			/* PortBind_Entry0,PortBind_Entry1 ... */
			attr = cldNode->value.element.attrs;
			for (i = 0; i < cldNode->value.element.num_attrs; i++) {
				if (!strcmp(attr->value, "Yes")) {
					mxmlElementSetAttr(commNode, attr->name, attr->value);
				}
				attr++;
			}
		}
		cldNode = mxmlWalkNext(cldNode, node, MXML_DESCEND);
	}

	return SUCCESS;
}

/*______________________________________________________________________________
**	exec_portbind_cmd
**
**	descriptions:
**		exec portbindcmd by this function.
**
**	parameters:
**		none.
**
**	return:
**		0 means success
**		1 means fail
**____________________________________________________________________________
*/

int 
exec_portbind_cmd(char *cmd)
{
	if (cmd == NULL) {
		printf("==>exec_portbind_cmd(): cmd is NULL.\n");
		return -1;
	}
	if (strlen(cmd) == 0) {
		return -1;
	}
#ifdef PORTBIND_DEBUG
	tcdbg_printf("PORTBINDCMD=%s\n", cmd);
#endif

	system(cmd);
	return 0;
}

#endif

#ifdef TCSUPPORT_DMS
char *
dms_adv_table[MAX_NODE_NAME]=
{
	"Basic",
	"Device",
	"",
};
/*______________________________________________________________________________
**	dms_boot
**
**	descriptions:
**		the boot function of dms.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
dms_boot(mxml_node_t *top){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char dms_en[6] = {0};
	char cmd[64] = {0};
	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], "DMS");
	strcpy(nodeName[1], "Entry");	
	
	if(getAttrValue(top, nodeName, "Enable", dms_en) == TCAPI_PROCESS_OK){
		if(!strcmp(dms_en, "Yes")){
			tcdbg_printf("dms_boot:start DMS service\n");
			sprintf(cmd, "%s %s %s", DMS_START_PATH, DMSDEVICECFG_PATH, DMSCFG_PATH);
			system(cmd);
		}
	}	
	
	return SUCCESS;
}
/*______________________________________________________________________________
**	dms_init
**
**	descriptions:
**		the init function of dms.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
dms_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="DMS",
			.type=SINGLE_TYPE | ADV_FLAG,
			.adv_table=dms_adv_table,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=dms_write,
			.cfg_verify=dms_verify,
			.cfg_execute=dms_execute,
			.cfg_boot=dms_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}
/*______________________________________________________________________________
**	dms_write
**
**	descriptions:
**		Find dms element from mxml tree then write dms's system parameter to specify
**		configuration files.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  dms cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
dms_write(mxml_node_t *top, mxml_node_t *parant){
	char if_addr[19] = {0};
	mxml_node_t *node = NULL;	
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
#ifdef TCSUPPORT_AUTO_IP
	char autoip_en[6] = {0};
#endif
	
	/*Get address of LAN interface*/	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], "Lan");
	strcpy(nodeName[1], "Entry0");
	
#ifdef TCSUPPORT_AUTO_IP	
	if(getAttrValue(top, nodeName, "AUTOIPEnable", autoip_en) == TCAPI_PROCESS_OK &&
			!strcmp(autoip_en, "Yes")){	
		if(getAttrValue(top, nodeName, "IPAuto", if_addr) != TCAPI_PROCESS_OK){
			tcdbg_printf("dms_write:get br0 auto address failed\n");
			return FAIL;
		}
	}
	else{
#endif
		if(getAttrValue(top, nodeName, "IP", if_addr) != TCAPI_PROCESS_OK){
			tcdbg_printf("dms_write:get br0 address failed\n");
			return FAIL;
		}
#ifdef TCSUPPORT_AUTO_IP	
	}
#endif
	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], DMS_NODE);
	strcpy(nodeName[1], DMSCFG_NODE);
	
	setAttrValue(top, nodeName, DMSCFG_IPADDR_NODE, if_addr);
	
	tcdbg_printf("dms:get new Lan ipaddr %s\n", if_addr);
	
	/*Create dms.cfg*/
	node = mxmlFindElement(parant, parant, DMSCFG_NODE,
		NULL, NULL,
		MXML_DESCEND);
	if(node){
		node_write(node, DMSCFG_PATH, QMARKS);
		//tcdbg_printf("dms:rewrite %s to %s\n", DMSCFG_NODE, DMSCFG_PATH);
	}
	else{
		tcdbg_printf("dms_write:can not find node %s\n", DMSCFG_NODE);
		return FAIL;
	}
	
	/*Create device.cfg*/
	node = NULL;
	node = mxmlFindElement(parant, parant, DMSDEVICECFG_NODE,
		NULL, NULL,
		MXML_DESCEND);
	if(node){
		node_write(node, DMSDEVICECFG_PATH, QMARKS);
	}
	else{
		tcdbg_printf("dms_write:can not find node %s\n", DMSDEVICECFG_NODE);
		return FAIL;
	}
	
	return SUCCESS;
}/* end dms_write */

/*______________________________________________________________________________
**	dms_verify
**
**	descriptions:
**		To check the system parameter of dms element  is validly or not.
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
dms_verify(mxml_node_t *node){
	return SUCCESS;
}/* end dms_verify */

/*_____________________________________________________________________________
**      function name: dms_execute
**      descriptions:
**            To  execute adsl node service.
**
**      parameters:
**            node:   Put the dms of cfg node tree that is used to execute adsl settings.
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
dms_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char dms_en[6] = {0};
	char cmd[64] = {0};
#if defined(TCSUPPORT_CT_JOYME)
	char dms_type[16] = {0};
	int ret = -1;
	FILE *fp = NULL;
#endif
	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], "DMS");
	strcpy(nodeName[1], "Entry");	
	
	system("kill -SIGTERM `cat /var/run/dms.pid`");
	sleep(2);
	
	if(getAttrValue(top, nodeName, "Enable", dms_en) == TCAPI_PROCESS_OK){
		if(!strcmp(dms_en, "Yes")){
#if defined(TCSUPPORT_CT_JOYME)
			getAttrValue(top, nodeName, "type", dms_type);
			if (!strcmp(dms_type, "plugin")){
				fp = fopen(DMSTCAPP_PLUGIN_PATH, "r");
				if(fp == NULL){
					sprintf(cmd, "cd %s", DLNA_PATH);
					ret = system(cmd);
					
					memset(cmd, 0, sizeof(cmd));
					sprintf(cmd, "tar -xvf %sdlna.tar -C %s", DLNA_PATH, DLNA_PATH);
					ret = system(cmd);
				}
				else {
					fclose(fp);
				}

				memset(cmd, 0, sizeof(cmd));
				tcdbg_printf("dms:plugin DMS service start...\n");
				sprintf(cmd, "%s %s %s", DMS_START_PLUGIN_PATH, DMSDEVICECFG_PATH, DMSCFG_PATH);
				system(cmd);
			}
			else{
			/*We need check does USB disk mount*/
			tcdbg_printf("dms:DMS service start...\n");
			sprintf(cmd, "%s %s %s", DMS_START_PATH, DMSDEVICECFG_PATH, DMSCFG_PATH);
			system(cmd);
			//tcdbg_printf("dms:please wait for about 1 minute for scanning media file\n");
		}
#else
			/*We need check does USB disk mount*/
			tcdbg_printf("dms:DMS service start...\n");
			sprintf(cmd, "%s %s %s", DMS_START_PATH, DMSDEVICECFG_PATH, DMSCFG_PATH);
			system(cmd);
			//tcdbg_printf("dms:please wait for about 1 minute for scanning media file\n");

#endif
		}
		else{
			tcdbg_printf("dms:DMS service stop\n");
		}
	}

	return SUCCESS;
}/* end dms_execute*/
#endif

#if defined(TCSUPPORT_CT_DSL_EX)
/*______________________________________________________________________________
**	virServATM_boot
**
**	descriptions:
**		the boot function of dmz.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**_______________________________________________________________________________
*/
int
virServATM_boot(mxml_node_t *top){
	return SUCCESS;
}

/*______________________________________________________________________________
**	virServATM_init
**
**	descriptions:
**		the init function of virServ.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
virServATM_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="VirServerATM",
			//.type=ENTRY_TYPE|PVC_TYPE|MAX_VIRSERV_RULE,
			.type=ENTRY_TYPE|MAX_WAN_IF_INDEX,
			.adv_table=NULL,
			.merge_table=virServ_merge_table,
			.cfg_read=NULL,
			.cfg_write=NULL,
		#if 0
			.cfg_write=virServer_write,
		#endif
			.cfg_verify=virServerATM_verify,
			.cfg_execute=virServerATM_execute,
			.cfg_boot=virServATM_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}
/*______________________________________________________________________________
**	virServerATM_write
**
**	descriptions:
**		Find virtual server element from mxml tree then write virServer's system parameter to specify
**		configuration files.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  virServer cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
virServerATM_write(mxml_node_t *top, mxml_node_t *parant){
	return SUCCESS;
}/* end virServer_write */

/*______________________________________________________________________________
**	virServerATM_verify
**
**	descriptions:
**		To check the system parameter of virtual Server element  is validly or not.
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
virServerATM_verify(mxml_node_t *node){
	return SUCCESS;
}/* end virServer_verify */

/*_____________________________________________________________________________
**      function name: virServerATM_execute
**      descriptions:
**            To  execute virServer node service.
**
**      parameters:
**            top:   Put the virServer of cfg node tree that is used to execute virtual server settings.
**            name:  Put the virServer cfg node name , VirServer_Entry0_Entry0~9
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
**      2. jlliu 2010/8/10
**____________________________________________________________________________
*/
int
virServerATM_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	return SUCCESS;
}/* end virServer_execute*/

/*______________________________________________________________________________
**	virServPTM_boot
**
**	descriptions:
**		the boot function of dmz.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**_______________________________________________________________________________
*/
int
virServPTM_boot(mxml_node_t *top){
	return SUCCESS;
}

/*______________________________________________________________________________
**	virServPTM_init
**
**	descriptions:
**		the init function of virServ.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
virServPTM_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="VirServerPTM",
			//.type=ENTRY_TYPE|PVC_TYPE|MAX_VIRSERV_RULE,
			.type=ENTRY_TYPE|MAX_WAN_IF_INDEX,
			.adv_table=NULL,
			.merge_table=virServ_merge_table,
			.cfg_read=NULL,
			.cfg_write=NULL,
		#if 0
			.cfg_write=virServer_write,
		#endif
			.cfg_verify=virServerPTM_verify,
			.cfg_execute=virServerPTM_execute,
			.cfg_boot=virServPTM_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}
/*______________________________________________________________________________
**	virServerPTM_write
**
**	descriptions:
**		Find virtual server element from mxml tree then write virServer's system parameter to specify
**		configuration files.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  virServer cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
virServerPTM_write(mxml_node_t *top, mxml_node_t *parant){
	return SUCCESS;
}/* end virServer_write */

/*______________________________________________________________________________
**	virServerPTM_verify
**
**	descriptions:
**		To check the system parameter of virtual Server element  is validly or not.
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
virServerPTM_verify(mxml_node_t *node){
	return SUCCESS;
}/* end virServer_verify */

/*_____________________________________________________________________________
**      function name: virServerPTM_execute
**      descriptions:
**            To  execute virServer node service.
**
**      parameters:
**            top:   Put the virServer of cfg node tree that is used to execute virtual server settings.
**            name:  Put the virServer cfg node name , VirServer_Entry0_Entry0~9
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
**      2. jlliu 2010/8/10
**____________________________________________________________________________
*/
int
virServerPTM_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	return SUCCESS;
}/* end virServer_execute*/

int
routeATM_init(void){
	int ret=0;
	cfg_node_t node={
			.name="RouteATM",
			.type=ENTRY_TYPE|UPDATE_TYPE|MAX_ROUTE_NUM,
			.adv_table=NULL,
			.merge_table=route_merge_table,
			.cfg_read=NULL,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=NULL,
			.cfg_boot=NULL,
			.cfg_pre_unset=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}

int
routePTM_init(void){
	int ret=0;
	cfg_node_t node={
			.name="RoutePTM",
			.type=ENTRY_TYPE|UPDATE_TYPE|MAX_ROUTE_NUM,
			.adv_table=NULL,
			.merge_table=route_merge_table,
			.cfg_read=NULL,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=NULL,
			.cfg_boot=NULL,
			.cfg_pre_unset=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}

int
route6ATM_init(void){
	int ret=0;
	cfg_node_t node={
			.name="Route6ATM",
			.type=ENTRY_TYPE|UPDATE_TYPE|MAX_STATIC_ROUTE6_NUM,
			.adv_table=NULL,
			.merge_table=route6_merge_table,
			.cfg_read=NULL,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=NULL,
			.cfg_boot=NULL,
			.cfg_pre_unset=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}

int
route6PTM_init(void){
	int ret=0;
	cfg_node_t node={
			.name="Route6PTM",
			.type=ENTRY_TYPE|UPDATE_TYPE|MAX_STATIC_ROUTE6_NUM,
			.adv_table=NULL,
			.merge_table=route6_merge_table,
			.cfg_read=NULL,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=NULL,
			.cfg_boot=NULL,
			.cfg_pre_unset=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}

int
dmzATM_init(void){
	int ret=0;
	cfg_node_t node={
			.name="DmzATM",
			.type=  ENTRY_TYPE | MAX_WAN_IF_INDEX,
			.adv_table=NULL,
			.merge_table=nat_merge_table,
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
dmzPTM_init(void){
	int ret=0;
	cfg_node_t node={
			.name="DmzPTM",
			.type=  ENTRY_TYPE | MAX_WAN_IF_INDEX,
			.adv_table=NULL,
			.merge_table=nat_merge_table,
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
igmpproxyATM_init(void){
	int ret=0;
	cfg_node_t node={
			.name="IGMPproxyATM",
			.type=SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=NULL,
			.cfg_boot=NULL,
			.cfg_pre_unset=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}

int
igmpproxyPTM_init(void){
	int ret=0;
	cfg_node_t node={
			.name="IGMPproxyPTM",
			.type=SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=NULL,
			.cfg_boot=NULL,
			.cfg_pre_unset=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}

int
mldproxyATM_init(void){
	int ret=0;
	cfg_node_t node={
			.name="MLDproxyATM",
			.type=SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=NULL,
			.cfg_boot=NULL,
			.cfg_pre_unset=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}

int
mldproxyPTM_init(void){
	int ret=0;
	cfg_node_t node={
			.name="MLDproxyPTM",
			.type=SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=NULL,
			.cfg_boot=NULL,
			.cfg_pre_unset=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}
#endif
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
/*_____________________________________________________________________________
**      function name: SimCard_execute
**      descriptions:
**            To  execute Sim Card services.
**
**      parameters:
**            node:   .
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
SimCard_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	return SUCCESS;
}/* end SimCard_execute*/

int 
SimCard_boot(mxml_node_t *top){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char  tempStatus[16] = {0};
	char  tempResult[16] = {0};
	char  tempCardExist[16] = {0};

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], "SimCard");
	strcpy(nodeName[1], "Entry");
	
	if(getAttrValue(top, nodeName, "cardExist", tempCardExist) == TCAPI_PROCESS_OK && (strcmp(tempCardExist, "1") == 0)){
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0], "deviceAccount");
		strcpy(nodeName[1], "Entry");
		if((getAttrValue(top, nodeName, "registerStatus", tempStatus) == TCAPI_PROCESS_OK) && (strcmp(tempStatus, "0") == 0)
			&& (getAttrValue(top, nodeName, "registerResult", tempResult) == TCAPI_PROCESS_OK) && (strcmp(tempResult, "1") == 0)){
			simcard_ledconf(SIMCARD_LED_CONFIG, SIMCARD_CONF_SUCCESS);
		}
	}
	return SUCCESS;
}/* end SimCard_boot*/

int
SimCard_init(void){
	int ret=0;
	cfg_node_t node={
			.name="SimCard",
			.type=SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=SimCard_execute,
			.cfg_boot=SimCard_boot,
			.cfg_pre_unset=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}
#endif


