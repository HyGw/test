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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include "cc.h"
//#include "../lib/libtcapi.h"
#include <linux/version.h>
#ifdef TCSUPPORT_SYSLOG
#include <syslog.h>
#endif
#include <sys/ioctl.h>
#include <linux/if.h>

#define KERNEL_2_6_36 	(LINUX_VERSION_CODE > KERNEL_VERSION(2,6,31))
#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7592) || defined(TCSUPPORT_WLAN_AC)
#ifdef BBUTOWBU
#include	<sys/socket.h>    /* for connect and socket*/
#include	<sys/stat.h>
#include	<err.h>
#include	<errno.h>
#include	<asm/types.h>
//#include	<linux/if.h>
#include	<linux/wireless.h>
//#include    <sys/ioctl.h>
#include	"oid.h"
#endif

#if defined(TCSUPPORT_CT)
#define WIFI_DEV_TXQUEUE_LEN 500
#endif
#ifdef TCSUPPORT_MTD_ENCHANCEMENT
#include "../../public/mtd/tc_partition.h"
int isFirstWrite=1;
#endif
#if defined(TCSUPPORT_WLAN_AC)
int isFirstWrite_ac=1;
#endif
#if defined(TCSUPPORT_CT_E8GUI)
void TCScrambler(uint8_t *din, uint8_t *dout, uint16_t data_len);
#endif

#if defined(TCSUPPORT_CT_PON_JS) || defined(TCSUPPORT_CT_NAMECHGNOTIFY)
extern void doNameChangeWLanInit(mxml_node_t *top);
#endif

typedef struct 
{
        char*           cChannelRate;
        int             iRateCount;
}TcWlanChannelMode;

TcWlanChannelMode	GVWlanChannelList[] =
{
	{"1", 	0},
	{"2", 	1},
	{"5.5",	2},
	{"11",	3},
	{"6",	0},
	{"9",	1},
	{"12",	2},
	{"18",	3},
	{"24",	4},
	{"36",	5},
	{"48",	6},
	{"54",	7},
	#if 0
	{"6.5", 	0},
	{"13",	1},
	{"19.5",	2},
	{"26",	3},
	{"39",	4},
	{"52",	5},
	{"58.5",	6},
	{"65",	7},
	{"13",	8},
	{"26",	9},
	{"39",	10},
	{"52",	11},
	{"78",	12},
	{"104",	13},
	{"117",	14},
	{"130",	15}, 
	{"300",	32},//(BW=1, SGI=0) HT duplicate 6Mbps
	#endif
	{"0",	33}
};
#endif

voip_simulate_state_t gVoipSimulateState[VOIP_LINE_NUM] = {0};

Backup_Parameters_Info BackupPara[]=
{
#ifdef TCSUPPORT_WLAN
	{1,"WLan_Entry0","SSID","ChinaNet-AP",NULL},//SSID
	{2,"WLan_Entry0","WPAPSK","123456789",NULL},//WPAKEY
#endif
	{3,"Account_Entry1","web_passwd","1234",NULL},// Attention: entry1 for user account
	{0,NULL,NULL,NULL}
};

#ifdef WSC_AP_SUPPORT//add by xyyou	
int WscOOBSeted=0;
#if defined(TCSUPPORT_WLAN_AC)
int WscOOBSeted_ac=0;
#endif
#ifdef BBUTOWBU
int iswpsstart=0;
#define WPSSTART 100
#define WPSSTOP 200
#define WLANSTOP 300
// Encryption type
#define WSC_ENCRTYPE_NONE    0x0001
#define WSC_ENCRTYPE_WEP     0x0002
#define WSC_ENCRTYPE_TKIP    0x0004
#define WSC_ENCRTYPE_AES     0x0008
#if defined(TCSUPPORT_CT_WLAN_WAPI)
#define WSC_ENCRTYPE_SMS4     0x0010
#endif

// Authentication types
#define WSC_AUTHTYPE_OPEN        0x0001
#define WSC_AUTHTYPE_WPAPSK      0x0002
#define WSC_AUTHTYPE_SHARED      0x0004
#define WSC_AUTHTYPE_WPA         0x0008
#define WSC_AUTHTYPE_WPA2        0x0010
#define WSC_AUTHTYPE_WPA2PSK     0x0020
#define WSC_AUTHTYPE_WPANONE     0x0080
#define WSC_AUTHTYPE_WPA1WPA2	 WSC_AUTHTYPE_WPA|WSC_AUTHTYPE_WPA2
#if defined(TCSUPPORT_CT_WLAN_WAPI)
#define WSC_AUTHTYPE_WAPI_CERT		0x0040
#define WSC_AUTHTYPE_WAPI_PSK		0x0100
#endif

char * WscGetAuthTypeStr(
    unsigned short authFlag)
{
	switch(authFlag)
	{
		case WSC_AUTHTYPE_OPEN:
			return "OPEN";
        default:
		case WSC_AUTHTYPE_WPAPSK:
			return "WPAPSK";
		case WSC_AUTHTYPE_SHARED:
			return "SHARED";
		case WSC_AUTHTYPE_WPANONE:
			return "WPANONE";
		case WSC_AUTHTYPE_WPA:
			return "WPA";
		case WSC_AUTHTYPE_WPA2:
			return "WPA2";
		case WSC_AUTHTYPE_WPA1WPA2:
			return "WPA1WPA2";
		case (WSC_AUTHTYPE_WPAPSK | WSC_AUTHTYPE_WPA2PSK):
		case WSC_AUTHTYPE_WPA2PSK:
			return "WPA2PSK";
#if defined(TCSUPPORT_CT_WLAN_WAPI)
		case WSC_AUTHTYPE_WAPI_CERT:
			return "WAICERT";
		case WSC_AUTHTYPE_WAPI_PSK:
			return "WAI-PSK";			
#endif
	}
}

char *  WscGetEncryTypeStr(
    unsigned short encryFlag)
{
	switch(encryFlag)
	{
		case WSC_ENCRTYPE_NONE:
			return "NONE";
		case WSC_ENCRTYPE_WEP:
			return "WEP";
        default:
		case WSC_ENCRTYPE_TKIP:
			return "TKIP";
		case (WSC_ENCRTYPE_TKIP | WSC_ENCRTYPE_AES):
		case WSC_ENCRTYPE_AES:
			return "AES";
#if defined(TCSUPPORT_CT_WLAN_WAPI)
		case WSC_ENCRTYPE_SMS4:
			return "SMS4";
#endif
	}
}

#endif

#endif

#if 0  /*Orginal ci-command mode */
const char
wan_attribute[][ATTR_SIZE]=
{
	{"ACTIVE"},
	{"VPI"},
	{"VCI"},
	{"QOS"},
	{"PCR"},
	{"SCR"},
	{"MBS"},
	{"ISP"},
	{"USERNAME"},
	{"PASSWORD"},
	{"ENCAP"},
	{"CONNECTION"},
	{"CLOSEIFIDLE"},
	{"LASTmss"},
	{"MSS"},
	{"PPPGETIP"},
	{"IPADDR"},
	{"NETMASK"},
	{"GATEWAY"},
	{"NATENABLE"},
	{"DEFAULTROUTE"},
	{"MTU"},
	{"RIPVERSION"},
	{"DIRECTION"},
	{"IGMP"},
	{""},
};

const char
dproxy_attribute[][ATTR_SIZE]=
{
	{"type"},
	{"Primary_DNS"},
	{"Secondary_DNS"},
	{""},
};

const char
dproxy_keyword[][ATTR_SIZE]=
{
	{"nameserver ="},
	{"nameserver ="},
	{""},
};

const char
zebra_attribute[][ATTR_SIZE]=
{
	{"interface"},
	{"multicast"},
	{""},
};

const char
zebra_default_parm[][2][ATTR_SIZE]=
{
	{"hostname","TCZebra"},
	{"password","1234"},
	{"enable password","1234"},
	{"",""},
};

const char
ripd_attribute[][ATTR_SIZE]=
{
	{"RIPVERSION"},
	{"DIRECTION"},
	{""},
};

const char
ripd_default_parm[][2][ATTR_SIZE]=
{
	{"hostname","TCRipd"},
	{"password","1234"},
	{"",""},
};

const char
wlan_attribute[][ATTR_SIZE]=
{
	{"APOn"},
	{"Country"},
	{"CountryRegion"},
	{"Channel"},
	{"BeaconPeriod"},
	{"RTSThreshold"},
	{"FragThreshold"},
	{"DtimPeriod"},
	{"WirelessMode"},
	{"BasicRate"},
	{"SSID"},
	{"HideSSID"},
	{"AuthMode"},
	{"EncrypType"},
	{"DefaultKeyID"},
	{"Key1Type"},
	{"Key1Str"},
	{"Key2Type"},
	{"Key2Str"},
	{"Key3Type"},
	{"Key3Str"},
	{"Key4Type"},
	{"Key4Str"},
	{"WPAPSK"},
	{"AccessPolicy0"},
	{"AccessControlList0"},
	{""},
};


const char
wlan_default_parm[][2][ATTR_SIZE]=
{
	{"AutoChannelSelect","0"},
	{"BssidNum","1"},
	{"TxRate","0"},
	{"TxPower","100"},
	{"BGProtection","0"},
	{"TxPreamble","0"},
	{"TxBurst","1"},
	{"PktAggregate","0"},
	{"TurboRate","0"},
	{"NoForwarding","0"},
	{"NoForwardingBTNBSSID","0"},
	{"ShortSlot","1"},
	{"PreAuth","0"},
	{"RekeyInterval","0"},
	{"RekeyMethod","DISABLE"},
	{"PMKCachePeriod","10"},
	{"",""},
};

const char
dhcpd_attribute[][2][ATTR_SIZE]=
{
	{"start",	"start"},
	{"max_leases",	"pool_count"},
	{"option lease","lease_time"},
	{"",""},
};
const char
dhcpd_sys_parm[][ATTR_SIZE]=
{
	{"opt subnet"},
	{"opt router"},
	{"opt dns"},
	{"opt wins"},
	{""},
};

const char
dhcpd_default_parm[][2][ATTR_SIZE]=
{
	{"interface", LAN_IF},
	{"lease_file", "/etc/udhcpd.leases"},
	{"",""},
};
#endif
#if 0  /*Orginal ci-command mode */
#define Byte_to_Hex(byte,hex) 	sprintf((hex),"%x",(byte))
/*______________________________________________________________________________
**	Chr_to_Hex
**
**	descriptions:
**		characters transfer to ascii code
**
**	parameters:
**		src: 			Input the string that you want to transfer it.
**		target: 		Output the string that you want to store it.
**
**	return:
**		None
**
**____________________________________________________________________________
*/
void
Chr_to_Hex(char *src,char *target){
	char hex[3],*tmp_target= NULL;
	int src_long=0,i,target_num=0;

	if(src==NULL||target==NULL)
		return;

	target[target_num++]='0';
	target[target_num++]='x';

	tmp_target=target;
	src_long=strlen(src);
	if(src_long <= 0){
		return;
	}

	for(i = 0; i < src_long; i++){
		Byte_to_Hex(src[i],hex);
		strncpy(tmp_target+target_num,hex,2);
		target_num+=2;
	}

	target[target_num]=0;

}/* end Chr_to_Hex*/

/*______________________________________________________________________________
**	Hex_to_Chr
**
**	descriptions:
**		Ascii code transfer characters strings.
**
**	parameters:
**		src: 			Input the string that you want to transfer it.
**		target: 		Output the string that you want to store it.
**
**	return:
**		None
**
**____________________________________________________________________________
*/
void
Hex_to_Chr(char *src,char *target){
	char *tmp_src= NULL;
	int i,src_long=0;

	char hex[3];

	if(src==NULL||target==NULL){
		return;
	}

	hex[2]=0;

	if(*src!='0'||*(src+1)!='x'){
		return;
	}

	tmp_src=src+2;
	src_long=strlen(src);

	if(src_long<=0){
		return;
	}

	for(i = 0; i < src_long; i++){
		strncpy(hex,tmp_src,2);
		target[i]=(char)strtol(hex,NULL,16);
		tmp_src+=2;
	}
	target[i]=0;
}/* end Hex_to_Chr*/

/*______________________________________________________________________________
**	wan_read
**
**	descriptions:
**		Read wan's system parameter from configuration file and record those information
**		to mxml_node_t.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**____________________________________________________________________________
*/
int
wan_read(mxml_node_t *top){
	int fd = 0;
	char tmp[128];
	int i=0,j=0;
	char filename[128];
	char element_name[32]={0};
	char attr_tmp[ATTR_SIZE]={0};

	mxml_node_t *group= NULL;  /* <Wan> */
	mxml_node_t *node= NULL;   /* <node> */

	group = mxmlNewElement(top, WAN);

	for(i = 0; i < PVC_NUM; i++){
		sprintf(filename, WAN_PATH, i);
		fd = open(filename, O_RDONLY);
		if(fd == -1){
			continue;
		}
		else{
			/*sprintf(element_name,"%s_%d", WAN_ISP, i);*/
			sprintf(element_name,WAN_PVC, WAN, i);
			node = mxmlNewElement(group, element_name);
			for(j = 0; strlen(wan_attribute[j]) !=0; j++){
				sprintf(attr_tmp,"%s=",wan_attribute[j]);
				memset(tmp, 0, sizeof(tmp));
				get_profile_str(attr_tmp, tmp, QMARKS, fd);
				if(strlen(tmp) != 0){
					mxmlElementSetAttr(node, wan_attribute[j], tmp);
				}
			}
			close(fd);
		}
	}
	return 0;
}/* end wan_read */
#endif

//{"Wan",		PVC_TYPE,	NULL, wan_merge_table, NULL, wan_write, wan_verify, wan_execute}
#if 0 
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
	int i;
	FILE *startupSh=NULL;
	char startupPath[24]={0};
	char startupCmd[48]={0};
	unsigned int new_filter_state=0;

#ifndef PURE_BRIDGE
//	setNewFilterState(top,wan_check_filter,WAN_VECTOR,&new_filter_state);
	if(wan_check_filter(top)){//filter on
		new_filter_state=pre_sys_state.filter_m_state|WAN_VECTOR;
	}
	else{//filter down
		new_filter_state=pre_sys_state.filter_m_state&(~WAN_VECTOR);
	}
	pre_sys_state.filter_m_state = check_and_set_filter(new_filter_state);
	for(i=0; i<PVC_NUM; i++){
		sprintf(startupCmd,"iptables -t nat -N VS_PRE%d",i);
		system(startupCmd);
		sprintf(startupCmd,"iptables -t nat -N DMZ_PRE%d",i);
		system(startupCmd);
	}
#endif
	for(i=0; i< PVC_NUM; i++){
		sprintf(startupPath,"/etc/isp%d.conf",i);
		startupSh=fopen(startupPath,"r");
		if(startupSh){
			fclose(startupSh);
			sprintf(startupCmd,"/usr/script/wan_start.sh %d",i);
			system(startupCmd);
		}
	}
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
#ifdef TCSUPPORT_CT_PPPOEPROXY//xyyou
	pppoeproxy(top);
#endif
	return SUCCESS;
}
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
			.type=PVC_TYPE,
			.adv_table=NULL,
			.merge_table=wan_merge_table,
			.cfg_read=NULL,
			.cfg_write=wan_write,
			.cfg_verify=wan_verify,
			.cfg_execute=wan_execute,
			.cfg_boot=wan_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}

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

	mxml_node_t *node = NULL;
	char pvc_name[16]={0};
	int i=0;
	char element_name[ATTR_SIZE];
	int retval=0;

	for(i = 0; i < PVC_NUM; i++){
		sprintf(pvc_name, WAN_PATH, i);
		/*sprintf(element_name,"%s_%d", WAN_ISP, i);*/
		sprintf(element_name, SUB_PVC_NAME, i);
		node = mxmlFindElement(parant, parant, element_name,
	  		NULL, NULL,
	  		MXML_DESCEND);
		retval = node_write(node, pvc_name, QMARKS);
	}
#ifndef PURE_BRIDGE
	/*Create zebra.conf  & ripd.conf file*/
	retval += create_rip_conf(top);
#endif
	return retval;
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
	unsigned int new_filter_state=0;
	char string[32], tmp[64], nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char autoconf[4]={0}, WAN_IFNAME[8]={0}, cur_wanif[5]={0}, default_route[5]={0}, active[5]={0}, cur_igmpproxy_active[5];  /*rodney_20090420 add*/
	char igmpproxy_pid_tmp[6];  /*rodney_20090420 add*/
	FILE *fp=NULL;  /*rodney_20090420 add*/
	int i, cur_vc, isp, fd, igmpproxy_pid;  /*rodney_20090420 modify*/
#ifdef IPV6
	char cur_mldproxy_active[5]={0};
#endif

	tcapi_msg_t msg;
	memset(&msg,0,sizeof(msg)) ;

	if(get_entry_number(name[1],WAN_PVC,&cur_vc) != SUCCESS){
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],WEBCURSET);
	strcpy(nodeName[1],SUB_NODE_NAME);
	/*get current PVC*/
	if(getAttrValue(top, nodeName, "wan_pvc", tmp) == TCAPI_PROCESS_OK){
		cur_vc = atoi(tmp);
	}else{
		return FAIL;
	}
	}
	/*stop wan PVCn*/
	sprintf(string, "%s %d",WAN_STOP_SH, cur_vc);
	system(string);

  	memset(nodeName,0,sizeof(nodeName));
  	strcpy(nodeName[0],WAN);
  	sprintf(nodeName[1],SUB_PVC_NAME,cur_vc);
	/*check active*/
	if(getAttrValue(top, nodeName, NODE_ACTIVE, active) != TCAPI_PROCESS_OK ){
		sprintf(tmp, WAN_PATH, cur_vc);
		remove(tmp);
		return FAIL;
	}


#ifndef PURE_BRIDGE
	/*get Wan_PVC IGMPproxy attribute*/  /*rodney_20090420 add*/
	if(getAttrValue(top,nodeName, "IGMPproxy",cur_igmpproxy_active)!= TCAPI_PROCESS_OK){
		memset(cur_igmpproxy_active,0,sizeof(cur_igmpproxy_active));
	}
#ifdef IPV6
	if(getAttrValue(top,nodeName, "MLDproxy",cur_mldproxy_active)!= TCAPI_PROCESS_OK){
		memset(cur_mldproxy_active,0,sizeof(cur_mldproxy_active));
	}
#endif
#ifdef QOS_DROP  /*Rodney_20091115*/
	qos_drop_set(DISABLE, QOS_DROP_TAIL, 0);
#endif
#endif
	if(!strcmp(active,ACTIVE)){
		/*get ISP ex:dynamic mode*/
		if(getAttrValue(top, nodeName, WAN_ISP, tmp)!= TCAPI_PROCESS_OK){
			return FAIL;
		}
		isp = atoi(tmp);
		/*get default route*/
		if(getAttrValue(top,nodeName, "DEFAULTROUTE", default_route)!= TCAPI_PROCESS_OK){
			strcpy(default_route,"");
		}
	  	/*If the current PVC actived default route then we need to delete other default route entry.*/
   		if(!strcmp(default_route, ACTIVE)){
    		memset(nodeName,0,sizeof(nodeName));
   			strcpy(nodeName[0],WAN);
			/*PVC 0~7*/
    		for(i = 0 ; i < PVC_NUM; i++){
    			/*do not check self*/
    			if(i != cur_vc)	{
			   		/*nodeName[0]="Wan", nodeName[1]="PVCn", nodeName[2]=""*/
					sprintf(nodeName[1],SUB_PVC_NAME,i);
          			if(getAttrValue(top, nodeName, "DEFAULTROUTE", tmp) != TCAPI_PROCESS_OK){
           				continue;
          			}
					/*PVC active*/
          			if(!strcmp(tmp, ACTIVE)){
          				/*use tcapi_set_req to set value*/
          				sprintf(msg.node,"Wan_PVC%d",i);
          				strcpy(msg.attr,"DEFAULTROUTE");
          				strcpy(msg.value,DEACTIVE);
          				tcapi_set_req(top, &msg);
          				if( msg.retval == TCAPI_PROCESS_OK){
          					/*if(setAttrValue(top, nodeName, "DEFAULTROUTE", DEACTIVE) == TCAPI_PROCESS_OK){*/
          					strcpy(tmp, "/sbin/route del default");
          					system(tmp);
          					break;
          				}
          			}else{
          				continue;
          			}
    			}else{
    				continue;
    			}
    		}
#ifndef PURE_BRIDGE
#ifdef TCSUPPORT_SIMPLE_UPNP
    		/*check upnp wan interface and use default route interface to wanif of upnpd.*/
    		if((get_profile_str("autoconf=", autoconf, sizeof(autoconf), NO_QMARKS, UPNPD_PATH) != FAIL)
    				&&(get_profile_str("wanif=", WAN_IFNAME, sizeof(WAN_IFNAME), NO_QMARKS, UPNPD_PATH) !=FAIL)){
    			/*check isp mode to decide cur_wanif.*/
    			if(isp == WAN_ENCAP_PPP){
    				sprintf(cur_wanif, PPP_IF, cur_vc);
    			}else{
    				sprintf(cur_wanif, NAS_IF, cur_vc);
    			}
    			/*wan_if change and autoconf active, restart upnp.*/
    			if((autoconf[0] == '1') && (strcmp(WAN_IFNAME, cur_wanif) != 0)){
    				write_upnp_conf(top);
    				kill_process("/var/log/upnpd.pid");
    				system("/userfs/bin/upnpd &");
    			}
    		}
#endif
#endif
		}
#ifndef PURE_BRIDGE
   		/*When current pvc enable IGMPproxy, the rest of PVC's IGMPproxy attribute should be clear to No*/
   		if(!strcmp(cur_igmpproxy_active, ACTIVE)){  /*rodney_20090420 add*/
    		memset(nodeName,0,sizeof(nodeName));
   			strcpy(nodeName[0],WAN);
   			for(i = 0 ; i < PVC_NUM; i++){
   				if(i != cur_vc){
					sprintf(nodeName[1],SUB_PVC_NAME,i);
          			if(getAttrValue(top, nodeName, "IGMPproxy", tmp) == TCAPI_PROCESS_OK){
          				if(!strcmp(tmp, ACTIVE)){
              				sprintf(msg.node,"Wan_PVC%d",i);
              				strcpy(msg.attr,"IGMPproxy");
              				strcpy(msg.value,DEACTIVE);
              				tcapi_set_req(top, &msg);
              				break;
          				}
          			}
   				}
   			}
   		}
#ifdef IPV6
		if(!strcmp(cur_mldproxy_active, ACTIVE)){  
    		memset(nodeName,0,sizeof(nodeName));
			memset(&msg, 0, sizeof(msg) );
   			strcpy(nodeName[0],WAN);
   			for(i = 0 ; i < PVC_NUM; i++){
   				if(i != cur_vc){
					memset(tmp, 0, sizeof(tmp));
					sprintf(nodeName[1],SUB_PVC_NAME,i);
          			if(getAttrValue(top, nodeName, "MLDproxy", tmp) == TCAPI_PROCESS_OK){
          				if(!strcmp(tmp, ACTIVE)){
              				sprintf(msg.node,"Wan_PVC%d",i);
              				strcpy(msg.attr,"MLDproxy");
              				strcpy(msg.value,DEACTIVE);
              				tcapi_set_req(top, &msg);
              				break;
          				}
          			}
   				}
   			}
   		}
#endif

		/*check NAT and  filter state*/
      	pre_sys_state.nat_raw_state = check_nat_enable(top,pre_sys_state.nat_raw_state);
//   	setNewFilterState(top,wan_check_filter,WAN_VECTOR,&new_filter_state);
      	if(wan_check_filter(top)){//filter on
     		new_filter_state=pre_sys_state.filter_m_state|WAN_VECTOR;
      	}
      	else{//filter down
      		new_filter_state=pre_sys_state.filter_m_state&(~WAN_VECTOR);
      	}
//     	pre_sys_state.filter_m_state = check_and_set_filter(top,pre_sys_state.filter_m_state);
     	pre_sys_state.filter_m_state = check_and_set_filter(new_filter_state);
#endif
		/* start wan*/
		sprintf(string, "%s %d",WAN_START_SH, cur_vc);
		system(string);
#ifndef PURE_BRIDGE
		/*restart zebra & ripd*/
		kill_process("/var/run/zebra.pid");
	    kill_process("/var/run/ripd.pid");
		fd = open(ZEBRA_PATH,O_RDONLY);
		if(fd != -1){
			close(fd);
			system("/userfs/bin/zebra -f /etc/zebra.conf -i /var/run/zebra.pid &");
			system("/userfs/bin/ripd -f /etc/ripd.conf -i /var/run/ripd.pid &");
		}

#endif
	}
	else{  /*rodney_20090420 add*/
#ifndef PURE_BRIDGE
		/*if current PVC is disable, the previous IGMPproxy process should be disabled also*/
		if(!strcmp(cur_igmpproxy_active, ACTIVE)){  /*rodney_20090420 add*/
				sprintf(msg.node,"Wan_PVC%d",cur_vc);
				strcpy(msg.attr,"IGMPproxy");
				strcpy(msg.value,DEACTIVE);
				tcapi_set_req(top, &msg);
				system("pidof igmpproxy > /tmp/igmpproxy.pid");
				fp = fopen("/tmp/igmpproxy.pid", "r");
				if(fp == NULL){
					igmpproxy_pid = 0;
				}
				else{
					fgets(igmpproxy_pid_tmp, 6, fp);
					fclose(fp);
					system("rm -r /tmp/igmpproxy.pid");
					igmpproxy_pid = atoi(igmpproxy_pid_tmp);
				}
			    if(igmpproxy_pid != 0){
			    	sprintf(string,"kill -9 %d",igmpproxy_pid);
			        system(string);
			        tcdbg_printf("IGMP proxy process down!\n");
			    }
		}
#ifdef IPV6
		if(!strcmp(cur_mldproxy_active, ACTIVE)){
				memset(&msg, 0, sizeof(msg));
				sprintf(msg.node,"Wan_PVC%d",cur_vc);
				strcpy(msg.attr,"MLDproxy");
				strcpy(msg.value,DEACTIVE);
				tcapi_set_req(top, &msg);
				//system("pidof ecmh > /tmp/igmpproxy.pid");
				system("kill -9 `cat /var/run/ecmh.pid`");
				unlink("/var/run/ecmh.pid");
			    tcdbg_printf("MLD proxy process down!\n");
		}

#endif
		
#endif
	}
#ifndef PURE_BRIDGE
	/**********20080905 racing add************/
	/*restart acl*/
	memset(&msg,0,sizeof(msg)) ;
	strcpy(msg.node, ACL);
	tcapi_commit_req(top, &msg);
	/**********20080905 racing add************/

	/**********20081005 racing add************/
	/*restart ipfilter*/
	memset(&msg,0, sizeof(msg)) ;
	strcpy(msg.node, IPMACFILTER);
	tcapi_commit_req(top, &msg);
	/**********20081005 racing add************/

	/*krammer add for firewall*/
	memset(&msg,0, sizeof(msg)) ;
	strcpy(msg.node, FIREWALL);
	tcapi_commit_req(top, &msg);
	/**/
#endif
    /*reset fork_or_not variable in cc.c to inform start_cc of Wan interface change*/
    reset_fork_or_not(cur_vc);  /*rodney_20090420 add*/
	return SUCCESS;
}/* end wan_execute */
#endif

#if 0  /*Orginal ci-command mode */
/*______________________________________________________________________________
**	lan_read
**
**	descriptions:
**		Read lan's system parameter from configuration file and record those information
**		to mxml_node_t.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**____________________________________________________________________________
*/
int
lan_read(mxml_node_t *top){

	FILE *fp = NULL;
	char tmp[MAXSIZE];
	char lanInfo[6][16];

	mxml_node_t *group=NULL;  /* <Lan> */
	mxml_node_t *node=NULL;   /* <node> */

	group = mxmlNewElement(top, LAN);
	/*Read lanconfig.sh file*/
	fp = fopen(LAN_PATH, "r");
	if(fp == NULL){
		return FAIL;
	}
	else{
		if(fgets(tmp,MAXSIZE,fp)){
			/* tmp="/sbin/ifconfig br0 10.10.10.1 netmask 255.255.255.0 up" */
			sscanf(tmp, "%s %s %s %s %s %s",
			lanInfo[0], lanInfo[1], lanInfo[2], lanInfo[3], lanInfo[4], lanInfo[5]);
			node = mxmlNewElement(group, LAN_ENTRY);
			mxmlElementSetAttr(node, LAN_IP, lanInfo[LAN_CFG_IP_INDEX]);
			mxmlElementSetAttr(node,  lanInfo[LAN_ATTR_MASK_INDEX], lanInfo[LAN_CFG_MASK_INDEX]);
		}
		fclose(fp);
	}
	return 0;
}/* end lan_read */
#endif

//"Lan",		ADV_FLAG|ENTRY_TYPE|LAN_NUM,	lan_adv_table, lan_merge_table, NULL, lan_write, lan_verify,  lan_execute},
char *
lan_adv_table[MAX_NODE_NAME]=
{
	"Dhcp",
#ifdef TCSUPPORT_IGMP_SNOOPING	
	"IgmpSnoop",
#endif
#if defined(TCSUPPORT_CT_NEWGUI)
	"Domain",
#endif
#ifdef TCSUPPORT_BRIDGE_MAC_LIMIT
	"BridgeMacLimit",
#endif
#if defined(TCSUPPORT_CMCC)
	"BSR",
#endif
	"",
};
char *
lan_merge_table[MAX_NODE_NAME]=
{
	"lan_id","",
};
#ifdef TCSUPPORT_IGMP_SNOOPING
void operateIgmpSnooping(mxml_node_t *top)
{
	char  tmp[32], nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char cmdbuf[100] = {0};	
#ifdef TCSUPPORT_MULTICAST_SPEED
	char cmdbuftemp[100] = {0};
#endif

	memset(nodeName,0,sizeof(nodeName));
  	strcpy(nodeName[0],LAN);
  	strcpy(nodeName[1],"IgmpSnoop");
	/*enable or disable igmp snooping*/
	if(getAttrValue(top, nodeName, "igmpsnoopEnable", tmp) == TCAPI_PROCESS_OK)
	{
		if(strcmp(tmp,"Yes") == 0)
		{	
#if KERNEL_2_6_36
			strcpy(cmdbuf,"brctl snoop br0 on");			
#else
#if defined(TCSUPPORT_CT_E8GUI)
			strcpy(cmdbuf,"brctl mldsnooping br0 on");
			system(cmdbuf);
			memset(cmdbuf,0,sizeof(cmdbuf));
#endif
			strcpy(cmdbuf,"brctl igmpsnoop br0 on");
#endif
#ifdef TCSUPPORT_MULTICAST_SPEED
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION) && defined(TCSUPPORT_CT_2PORTS) 
			strcpy(cmdbuftemp,"/userfs/bin/hw_nat -V 1");
			system(cmdbuftemp);
			
			memset(cmdbuftemp,0,sizeof(cmdbuftemp));
			strcpy(cmdbuftemp,"/userfs/bin/hw_nat  -@ 1");
			system(cmdbuftemp);
			
			memset(cmdbuftemp,0,sizeof(cmdbuftemp));
			strcpy(cmdbuftemp,"/userfs/bin/hw_nat  -!");
#else
			strcpy(cmdbuftemp,"/userfs/bin/hw_nat -V 1");
#endif
#endif

		}
		else
		{
#if KERNEL_2_6_36		
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION) && defined(TCSUPPORT_CT_2PORTS) 
			strcpy(cmdbuf,"brctl snoop br0 on");	
#else
			strcpy(cmdbuf,"brctl snoop br0 off");	
#endif
#else
#if defined(TCSUPPORT_CT_E8GUI)
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION) && defined(TCSUPPORT_CT_2PORTS) 
			strcpy(cmdbuf,"brctl mldsnooping br0 on");
#else
			strcpy(cmdbuf,"brctl mldsnooping br0 off");
#endif
			system(cmdbuf);
			memset(cmdbuf,0,sizeof(cmdbuf));
#endif
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION) && defined(TCSUPPORT_CT_2PORTS) 
			strcpy(cmdbuf,"brctl igmpsnoop br0 on");
#else
			strcpy(cmdbuf,"brctl igmpsnoop br0 off");
#endif

#endif
#ifdef TCSUPPORT_MULTICAST_SPEED
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION) && defined(TCSUPPORT_CT_2PORTS) 
			strcpy(cmdbuftemp,"/userfs/bin/hw_nat -V 1");
			system(cmdbuftemp);
			
			memset(cmdbuftemp,0,sizeof(cmdbuftemp));
			strcpy(cmdbuftemp,"/userfs/bin/hw_nat  -@ 0");
			system(cmdbuftemp);
			
			memset(cmdbuftemp,0,sizeof(cmdbuftemp));
			strcpy(cmdbuftemp,"/userfs/bin/hw_nat  -!");
#else
			strcpy(cmdbuftemp,"/userfs/bin/hw_nat -V 0");
#endif
#endif

		}
	}
	//printf("\r\nthe cmd is %s",cmdbuf);
	system(cmdbuf);
	
#ifdef TCSUPPORT_MULTICAST_SPEED
	system(cmdbuftemp);
#endif
	/*set age time*/
	memset(cmdbuf,0,sizeof(cmdbuf));
	if(getAttrValue(top, nodeName, "igmpsnoopAge", tmp) == TCAPI_PROCESS_OK)
	{
#if KERNEL_2_6_36		
		sprintf(cmdbuf,"brctl snoop br0 %s",tmp);
#else
		sprintf(cmdbuf,"brctl igmpsnoop br0 %s",tmp);
#endif
	}
	//printf("\r\nthe cmd is %s",cmdbuf);
	system(cmdbuf);
	/*set quick leave function*/
	memset(cmdbuf,0,sizeof(cmdbuf));
	if(getAttrValue(top, nodeName, "quickleaveEnable", tmp) == TCAPI_PROCESS_OK)
	{
		if(strcmp(tmp,"Yes") == 0)
		{	
			strcpy(cmdbuf,"brctl quickleave br0 on");
		}
		else
		{
			strcpy(cmdbuf,"brctl quickleave br0 off");
		}
	}
	else{
		strcpy(cmdbuf,"brctl quickleave br0 on");
	}
	//printf("\r\nthe cmd is %s",cmdbuf);
	system(cmdbuf);
	/*set debug function*/
	memset(cmdbuf,0,sizeof(cmdbuf));
	if(getAttrValue(top, nodeName, "igmpdbgEnable", tmp) == TCAPI_PROCESS_OK)
	{
		if(strcmp(tmp,"Yes") == 0)
		{	
#if KERNEL_2_6_36	
			strcpy(cmdbuf,"brctl snoopdbg br0 on");				
#else
			strcpy(cmdbuf,"brctl igmpdbg br0 on");
#endif
		}
		else
		{
#if KERNEL_2_6_36	
			strcpy(cmdbuf,"brctl snoopdbg br0 off");					
#else
			strcpy(cmdbuf,"brctl igmpdbg br0 off");
#endif
		}
	}
	//printf("\r\nthe cmd is %s",cmdbuf);
	system(cmdbuf);
	return;
}
#endif
#if defined(TCSUPPORT_CT_BRIDGEARP_NOFWD_LAN)
int isNeedChgArpRule = 0;
int AddBridgeArpRules ()
{
	mxml_node_t *top = NULL;
	char bufCmd[128] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char active[8] = {0};
	char lanIP[20] = {0};
	char netmask[20] = {0};
	char mask_dec[4] = {0};
	char isp[4] = {0};
	int pvc_index = 0;
	int entry_index = 0;
	int lan_index = 0;
#ifdef ALIAS_IP
	char aliasIPActive[LAN_ALIAS_NUM][8] = {0};
	char aliasLanIP[LAN_ALIAS_NUM][20] = {0};
	char aliasnetmask[LAN_ALIAS_NUM][20] = {0};
	char aliasmask_dec[LAN_ALIAS_NUM][4] = {0};
#endif

	if ( !isNeedChgArpRule )
		return FAIL;
	isNeedChgArpRule = 0;
	top = get_node_top();
	if ( NULL == top )
		return FAIL;

	memset(nodeName, 0, sizeof(nodeName));
  	strcpy(nodeName[0], LAN);
  	strcpy(nodeName[1], LAN_ENTRY_0);

	if ( getAttrValue(top, nodeName, LAN_IP, lanIP) < 0 )
		return FAIL;
	if ( getAttrValue(top, nodeName, LAN_MASK, netmask) < 0 )
		return FAIL;
	if ( 0 == strlen(netmask) )
		return FAIL;
	if ( !check_mask_format(netmask,mask_dec) )
		return FAIL;
#ifdef ALIAS_IP
	for (lan_index = 0; lan_index < LAN_ALIAS_NUM; lan_index++)
	{
		memset(nodeName, 0, sizeof(nodeName));
	  	strcpy(nodeName[0], LAN_ALIAS);
	  	sprintf(nodeName[1], "Entry%d", lan_index);

		if ( getAttrValue(top,nodeName, "Active", aliasIPActive[lan_index]) < 0 )
			break;

		if ( 0 == strcmp(aliasIPActive[lan_index],"Yes") )
		{
			if ( getAttrValue(top, nodeName, LAN_IP, aliasLanIP[lan_index]) < 0 )
				break;

			if ( getAttrValue(top, nodeName, LAN_MASK, aliasnetmask[lan_index]) < 0 )
				break;

			if ( 0 == strlen(aliasnetmask[lan_index]) ) 
				break;

			if ( !check_mask_format(aliasnetmask[lan_index], aliasmask_dec[lan_index]) )
				break;
		}

	}
#endif

	system("ebtables -t filter -F bridgeArp_chain");
	system("ebtables -t filter -Z bridgeArp_chain");
	for(pvc_index = 0; pvc_index < PVC_NUM; pvc_index++)
	{
		for(entry_index = 0; entry_index < MAX_SMUX_NUM; entry_index++)
		{

			memset(nodeName, 0, sizeof(nodeName));
			strcpy(nodeName[0], WAN);
			sprintf(nodeName[1], SUB_PVC_NAME, pvc_index);
			sprintf(nodeName[2], "Entry%d", entry_index);
			memset(active, 0, sizeof(active));
			memset(isp, 0, sizeof(isp));

			if ( getAttrValue(top, nodeName, NODE_ACTIVE, active) < 0
				|| 0 != strcmp(active, "Yes"))
				continue;

			if ( getAttrValue(top, nodeName, WAN_ISP, isp) < 0
				|| WAN_ENCAP_BRIDGE != atoi(isp) ) // only bridge mode need this rule
				continue;

			// when bridge,rule should apply to wan interface of nasx
			memset(bufCmd, 0, sizeof(bufCmd));
			sprintf(bufCmd,"ebtables -t filter -A bridgeArp_chain -p arp --arp-ip-src %s/%s -i nas%d_%d -j DROP",
							lanIP, mask_dec, pvc_index, entry_index);
			system(bufCmd);

			memset(bufCmd, 0, sizeof(bufCmd));
			sprintf(bufCmd,"ebtables -t filter -A bridgeArp_chain -p arp --arp-ip-dst %s/%s -i nas%d_%d -j DROP",
							lanIP, mask_dec, pvc_index, entry_index);
			system(bufCmd);

#ifdef ALIAS_IP
			for ( lan_index = 0; lan_index < LAN_ALIAS_NUM; lan_index++ )
			{
				if ( 0 == strcmp(aliasIPActive[lan_index], "Yes") )
				{
					memset(bufCmd, 0, sizeof(bufCmd));
					sprintf(bufCmd,"ebtables -t filter -A bridgeArp_chain -p arp --arp-ip-src %s/%s -i nas%d_%d -j DROP",
									aliasLanIP[lan_index], aliasmask_dec[lan_index], pvc_index, entry_index);
					system(bufCmd);

					memset(bufCmd, 0, sizeof(bufCmd));
					sprintf(bufCmd,"ebtables -t filter -A bridgeArp_chain -p arp --arp-ip-dst %s/%s -i nas%d_%d -j DROP",
									aliasLanIP[lan_index], aliasmask_dec[lan_index], pvc_index, entry_index);
					system(bufCmd);
				}
			}
#endif
		}
	}
}
#endif

/*______________________________________________________________________________
**	lan_boot
**
**	descriptions:
**		the boot function of lan.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
lan_boot(mxml_node_t *top){

	FILE *startupSh=NULL;
#if defined( TCSUPPORT_BRIDGE_MAC_LIMIT) || defined(TCSUPPORT_CMCC)
		char bml_attr[16];
		char bml_cmd[64];
		int i = 0;
		char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
		char tmp[32];
#endif

#if defined(TCSUPPORT_CMCC)
	int rate = 0, bsr_index = 0;
	char bsr_buf[10] = {0}, cmd[64] = {0};
#endif

	startupSh=fopen("/etc/lanconfig.sh","r");
	if(startupSh){
		fclose(startupSh);
		system("chmod 755 /etc/lanconfig.sh");
		system("/etc/lanconfig.sh");
	}
	else{
		system("/bin/ifconfig br0 192.168.1.1");
#if defined(TCSUPPORT_CT_ACCESSLIMIT)
		system("/usr/bin/accesslimitcmd laninfo br0 192.168.1.1 255.255.255.0");
#endif
	}
#ifdef TCSUPPORT_IGMP_SNOOPING	
	operateIgmpSnooping(top);
#endif
#if defined(TCSUPPORT_CT_BRIDGEARP_NOFWD_LAN)
	system("ebtables -N bridgeArp_chain");
	system("ebtables -P bridgeArp_chain RETURN");
	system("ebtables -I FORWARD 1 -j bridgeArp_chain");
	system("ebtables -I INPUT 1 -j bridgeArp_chain");
	isNeedChgArpRule = 1;
#endif

	check_route("br0");
#ifdef TCSUPPORT_BRIDGE_MAC_LIMIT
		memset(nodeName,0,sizeof(nodeName));
		strcpy(nodeName[0],LAN);
		strcpy(nodeName[1],"BridgeMacLimit");
	
		if(getAttrValue(top, nodeName, "macLimitTotal", tmp) == TCAPI_PROCESS_OK)
		{
			memset(bml_cmd,0,64);
			sprintf(bml_cmd,"echo %d > /proc/br_fdb/mac_limit_total",atoi(tmp));
			system(bml_cmd);
		}
	
		for(i = 1; i < 5;i++)
		{
			memset(bml_attr,0,16);
			sprintf(bml_attr,"macLimitPort%d",i);
			if(getAttrValue(top, nodeName, bml_attr, tmp) == TCAPI_PROCESS_OK)
			{
				memset(bml_cmd,0,64);
				sprintf(bml_cmd,"echo %d-%d > /proc/br_fdb/mac_limit_by_port",i+2,atoi(tmp));/*eth0.1~eth0.4=>port:3~6*/
				system(bml_cmd);
			}
		}

		system("/userfs/bin/hw_nat -!");
#endif



#if defined(TCSUPPORT_CMCC)
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],"Lan");
	strcpy(nodeName[1], "BSR");

	/* get boardcast storm rate */
	if (getAttrValue(top, nodeName, "Rate", tmp) == TCAPI_PROCESS_OK ) {
		rate = atoi(tmp);
	}
	else {
		rate = 3;
	}

	/* set broadcast storm rate control  */
	for (bsr_index = 0; bsr_index < 4; bsr_index++) {
		sprintf(bsr_buf, "LAN%d", bsr_index);
		if(getAttrValue(top, nodeName, bsr_buf, tmp) == TCAPI_PROCESS_OK ){
			if (strcmp(tmp, "1") == 0) {
				sprintf(cmd, "/userfs/bin/ethphxcmd gsww 2%d1c ce0%d0%d0%d", 
					bsr_index+1, rate, rate, rate);
				//tcdbg_printf("\r\nlan_execute(): cmd = %s\n", cmd);
				system(cmd);
			}
		}
	}
	
	return SUCCESS;
	
#endif

	return SUCCESS;
}
/*______________________________________________________________________________
**	lan_init
**
**	descriptions:
**		the init function of lan.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
lan_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="Lan",
			.type=ADV_FLAG|ENTRY_TYPE|LAN_NUM,
			.adv_table=lan_adv_table,
			.merge_table=lan_merge_table,
			.cfg_read=NULL,
			.cfg_write=lan_write,
			.cfg_verify=lan_verify,
			.cfg_execute=lan_execute,
			.cfg_boot=lan_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}

/*______________________________________________________________________________
**	lan_write
**
**	descriptions:
**		Find lan element from mxml tree then write lan's system parameter to specify
**		configuration files.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  lan cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
lan_write(mxml_node_t *top, mxml_node_t *parant){
	FILE *fp = NULL;
	int       i=0;          /* Looping var */
	mxml_node_t *node = NULL;
	const char *tmp=NULL;
	//char lan_info[3][16] = {0};
	char lan_ip[16] = {0}, lan_mask[16] = {0};
	char buf[128]={0};
	#ifdef IPV6
	char ip_v6[39]={0},prefix_v6[3]={0};	
	FILE *fptr = NULL;	
	char *ipToken = NULL;
	int j = 0, len = 0;
	#endif
#if defined(TCSUPPORT_CT_WAN_CHILD_PREFIX)
	char tmp_node_str[40] = {0};
#endif
#if defined(TCSUPPORT_CT)
	char cmdBuf[128]={0};
	char ip[10] = {0}, mask[10] = {0};
	unsigned int tmp1=0, tmp2=0, tmp3=0, tmp4=0;
#endif

	sprintf(buf,"%s%d",SUB_NODE_NAME,i);
	node = mxmlFindElement(parant, parant, buf,
	  	NULL, NULL,MXML_DESCEND);

	if(node != NULL){
		fp = fopen(LAN_PATH,"w");
		if(fp == NULL){
			return FAIL;
		}
		else{
		#if 0
			attr = node->value.element.attrs;
			for(i = 0; i < node->value.element.num_attrs; i++){
				if(strcmp(attr->name, LAN_IP) == 0){
					strcpy(lan_info[0],attr->value);
				}
				else{
					strcpy(lan_info[1],attr->name);
					strcpy(lan_info[2],attr->value);
				}
				attr ++;
			}
			/*Buf: /sbin/ifconfig br0 10.10.10.10 netmask 255.255.255.0 up*/
 			sprintf(buf,"%s %s %s %s %s up\n",
				LAN_IFCONFG_CMD, LAN_IF,lan_info[LAN_IF_INDEX],
				lan_info[LAN_MASK_NAME_INDEX] ,lan_info[LAN_MASK_INDEX]);
			fputs(buf, fp);
			#ifdef IPV6
			sprintf(buf,"%s%d",SUB_NODE_NAME,1);
			node = mxmlFindElement(parant, parant, buf,
				NULL, NULL,MXML_DESCEND);
			if(node != NULL){
				tmp=mxmlElementGetAttr(node, "IP6");
				if(tmp!=NULL){			
					strcpy(ip_v6, tmp);
				}
				tmp=mxmlElementGetAttr(node, "PREFIX6");
				if(tmp!=NULL){			
					strcpy(prefix_v6, tmp);
				}
				tmp=mxmlElementGetAttr(node, "Interface");
				if(tmp!=NULL){			
					strcpy(interface, tmp);
				}
				/*ifconfig eth0 inet6 ip_v6/prefix_v6*/
				sprintf(buf,"%s %s inet6 %s/%s\n",
				LAN_IFCONFG_CMD, interface, ip_v6,
				prefix_v6);
				fputs(buf, fp);
			}
			#endif
		#endif	
			tmp=mxmlElementGetAttr(node, LAN_IP);
			if(tmp!=NULL){			
				strcpy(lan_ip, tmp);
			}
			tmp=mxmlElementGetAttr(node, LAN_MASK);
			if(tmp!=NULL){			
				strcpy(lan_mask, tmp);
			}
			
			/*Buf: /sbin/ifconfig br0 10.10.10.10 netmask 255.255.255.0 up*/
 			sprintf(buf,"%s %s %s netmask %s up\n",
				LAN_IFCONFG_CMD, LAN_IF,lan_ip,
				lan_mask);
			fputs(buf, fp);

#if defined(TCSUPPORT_CT)
			sscanf(lan_ip, "%d.%d.%d.%d", &tmp1, &tmp2, &tmp3, &tmp4);
			sprintf(ip, "%02x%02x%02x%02x", tmp1, tmp2, tmp3, tmp4);
			sprintf(cmdBuf, "echo IP= %s > %s\n", ip, LAN_IP_MASK_PATH);
			system(cmdBuf);
			memset(cmdBuf, 0, 20);
			sscanf(lan_mask, "%d.%d.%d.%d", &tmp1, &tmp2, &tmp3, &tmp4);
			sprintf(mask, "%02x%02x%02x%02x", tmp1, tmp2, tmp3, tmp4);
			sprintf(cmdBuf, "echo Mask= %s > %s\n", mask, LAN_IP_MASK_PATH);
			system(cmdBuf);	
#endif
			
#if defined(TCSUPPORT_CT_ACCESSLIMIT)
			memset(buf,0,sizeof(buf));
			sprintf(buf,"%s %s %s %s\n",
				LAN_ACCESSLIMIT_CMD, LAN_IF,lan_ip,
				lan_mask);
			fputs(buf, fp);
#endif
			
			#ifdef IPV6
			#if 0
			fptr = fopen("/proc/net/if_inet6", "r");
			if(fptr == NULL){
				fclose(fp);
				return FAIL;
			}
			
			while(fgets(buf, 128, fptr) != NULL){
				if(strstr(buf, "br0") != NULL){
					ipToken = strtok(buf, " "); //IPv6 Address
					//local link, don't delete it
					if(strcasecmp(ipToken, "fe800000000000000000000000000001") == 0){						
						continue;
					}

					len = strlen(ipToken); //transform to formal IPv6 Address
					for(i = 0, j =0; i < len; i++, j++){
						if((i > 0) && ((i % 4) == 0)){
							ip_v6[j++] = ':';
						}
						ip_v6[j] = ipToken[i];
					}
					ip_v6[j] = '\0';	
					strtok(NULL, " "); //skip one token : dev num
					ipToken = strtok(NULL, " ");//IPv6 Prefix
					sprintf(prefix_v6, "%d", strtol(ipToken, NULL, 16)); //from hex to dec
		
		
					if(strlen(ip_v6) != 0 && strlen(prefix_v6) != 0){
						sprintf(buf, "%s %s del %s/%s\n", LAN_IFCONFG_CMD, LAN_IF, ip_v6, prefix_v6);
						fputs(buf, fp);
					}			
				}			
			}
			
			fclose(fptr);			
			#endif

			tmp=mxmlElementGetAttr(node, LAN_IPV6);
			if(tmp!=NULL){			
				strcpy(ip_v6, tmp);
			}
			tmp=mxmlElementGetAttr(node, LAN_PREFIXV6);
			if(tmp!=NULL){			
				strcpy(prefix_v6, tmp);
			}
			
			if(strlen(ip_v6) != 0 && strlen(prefix_v6) != 0){	
				/*Buf: /sbin/ifconfig br0 inet6 ip_v6/prefix_v6*/
				sprintf(buf,"%s %s add %s/%s\n",
				LAN_IFCONFG_CMD, LAN_IF, ip_v6,
				prefix_v6);
				fputs(buf, fp);
			}
			#endif
#if defined(TCSUPPORT_CT_WAN_CHILD_PREFIX)
			sprintf(prefix_v6,"%d", MAX_PREFIX_MASK_LEN);
			for(i = START_NEW_ADD_lAN_IPV6; i<= MAX_NEW_ADD_lAN_IPV6; i++){
				sprintf(tmp_node_str,"%s_%d",LAN_IPV6, i);
				tmp=mxmlElementGetAttr(node, tmp_node_str);
				if(tmp == NULL){
					continue;
				}
				strcpy(ip_v6, tmp);
				if(strlen(ip_v6) != 0 && strlen(prefix_v6) != 0){	
					/*Buf: /sbin/ifconfig br0 inet6 ip_v6/prefix_v6*/
					sprintf(buf,"%s %s add %s/%s\n",
					LAN_IFCONFG_CMD, LAN_IF, ip_v6,
					prefix_v6);
					fputs(buf, fp);
				}
			}
#endif

			fclose(fp);
		}
	}else{
		unlink(LAN_PATH);
	}	
	chmod(LAN_PATH,777);
	return SUCCESS;
}/* end lan_write */

/*______________________________________________________________________________
**	lan_verify
**
**	descriptions:
**		To check the system parameter of lan element  is validly or not.
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
lan_verify(mxml_node_t *node){
	return SUCCESS;
}/* end lan_verify */

/*_____________________________________________________________________________
**      function name: lan_execute
**      descriptions:
**            To execute lan node service.
**
**      parameters:
**            node:   Put the lan of cfg node tree that is used to execute lan settings.
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
lan_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
  	char  tmp[32], nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
  	//int pvc;  /*rodney_20090420 add*/
#ifdef IPV6
	char enable[5] = {0};
	char mode[5] = {0};	
#endif
#if defined(TCSUPPORT_DMS) || defined(CT_COM_DEVICEREG)
	tcapi_msg_t msg;
#endif

#ifdef TCSUPPORT_BRIDGE_MAC_LIMIT
	char bml_attr[16];
	char bml_cmd[64];
	int i = 0;
#endif
#if defined(TCSUPPORT_CMCC)
	/*FILE *fp;*/
	int bsr_index = 0, rate = 0;
	char bsr_buf[10] = {0};
	char cmd[64] = {0};

	if (strcmp(name[1], "BSR") == 0) {		
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0],"Lan");
		strcpy(nodeName[1], "BSR");

		/* get boardcast storm rate, default rate is 3M */
		if (getAttrValue(top, nodeName, "Rate", tmp) == TCAPI_PROCESS_OK ) {
			rate = atoi(tmp);
		}
		else {
			rate = 3;
		}

		/* set broadcast storm rate control  */
		for (bsr_index = 0; bsr_index < 4; bsr_index++) {
			sprintf(bsr_buf, "LAN%d", bsr_index);
			if(getAttrValue(top, nodeName, bsr_buf, tmp) == TCAPI_PROCESS_OK ){
				if (strcmp(tmp, "1") == 0) {
					sprintf(cmd, "/userfs/bin/ethphxcmd gsww 2%d1c ce0%d0%d0%d", 
						bsr_index+1, rate, rate, rate);
					//tcdbg_printf("\r\nlan_execute(): cmd = %s\n", cmd);
					system(cmd);
				}
			}
		}
		
		return SUCCESS;
	}
#endif

	system(LAN_PATH);
#ifdef TCSUPPORT_DMS
	/*Restart DMS for IP may be changed*/
	memset(&msg,0,sizeof(msg)) ;
	strcpy(msg.node, "DMS");
	tcapi_commit_req(top, &msg);	
#endif

#ifdef CT_COM_DEVICEREG
	memset(&msg, 0, sizeof(msg)) ;
	strcpy(msg.node, "deviceAccount_Entry");
	tcapi_commit_req(top, &msg);	
#endif
#if 0
	/*execution at background and restart boa*/
	fp = fopen(RESTART_BOA_SCRIPT,"r");
	if(fp != NULL){
 		fclose(fp);
		sprintf(tmp,"%s 1 &",RESTART_BOA_SCRIPT);
		system(tmp);
	}
#endif

#if 0
#ifndef PURE_BRIDGE
	for(pvc = 0; pvc < PVC_NUM; pvc++){  /*rodney_20090420 add*/
#ifdef IPV6
		int isp = WAN_ENCAP_BRIDGE;
#endif
	  	memset(nodeName,0,sizeof(nodeName));
	  	strcpy(nodeName[0],WAN);
	  	sprintf(nodeName[1],SUB_PVC_NAME,pvc);
#ifdef IPV6	
		memset(tmp, 0, sizeof(tmp));
	  	if(getAttrValue(top, nodeName, "ISP", tmp) == TCAPI_PROCESS_OK){
	  		isp = atoi(tmp);
	  		}

#endif
		memset(tmp, 0, sizeof(tmp));
	  	if(getAttrValue(top, nodeName, "IGMPproxy", tmp) == TCAPI_PROCESS_OK){
	  		if(strcmp(tmp,"Yes") == 0){
		        restart_igmpproxy("LAN",pvc);
	  		}
		}
#ifdef IPV6
		memset(tmp, 0, sizeof(tmp));
		if(getAttrValue(top, nodeName, "MLDproxy", tmp) == TCAPI_PROCESS_OK){
	  		if(strcmp(tmp,"Yes") == 0){
		       		restart_mldproxy("LAN",pvc,isp);
	  		}
	  	}
#endif
	}

#endif
#endif
#if defined(TCSUPPORT_IGMP_PROXY)
	/*restart IGMP proxy when LAN interface change*/
	memset(nodeName, 0, sizeof(nodeName));
  	strcpy(nodeName[0],"IGMPproxy");
  	strcpy(nodeName[1], "Entry");
	if(getAttrValue(top, nodeName, "Active", tmp) == TCAPI_PROCESS_OK ){
		if(strcmp(tmp,"Yes") == 0){
		        tcapi_commit_internal(top, "IGMPproxy");
	  	}
	}
#endif

#if VOIP
	/*restart voip service when modify lan setting. shnwind */
	voip_real_write(top);
	voip_real_execute(top,name);
#endif	
#ifdef TCSUPPORT_IGMP_SNOOPING	
	operateIgmpSnooping(top);
#endif
#if defined(TCSUPPORT_CT_BRIDGEARP_NOFWD_LAN)
	isNeedChgArpRule = 1;
#endif
	memset(nodeName,0,sizeof(nodeName));
  	strcpy(nodeName[0],LAN);
  	strcpy(nodeName[1],"Dhcp");
	if(getAttrValue(top, nodeName, "type", tmp) == TCAPI_PROCESS_OK){
    		if(!strcmp(tmp,"0")){/*0:disable*/
#if 0
			system("killall -9 dproxy");
#endif
#if 0//fix bug#5775
			system("killall -9 dnsmasq");
#endif
			kill_process("/var/run/udhcpd.pid");
			kill_process("/var/log/dhcrelay.pid");
		}

  	}else{
    		return FAIL;
  	}
#if 0
#ifdef IPV6
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],"Radvd");
	strcpy(nodeName[1],"Entry");
	if(
			(getAttrValue(top,nodeName,"Enable",enable) == TCAPI_PROCESS_OK) &&
			(getAttrValue(top,nodeName,"Mode",mode) == TCAPI_PROCESS_OK)
	  )
	{
		char ipaddr[64] = {0};
		char cmd[128] = {0};
		
		tcdbg_printf("Enter set first loop, IP addr by radvd");
		if (getAttrValue(top,nodeName,"PrefixIPv6",ipaddr) != TCAPI_PROCESS_OK)
		{
			return FAIL;
		}

		if( ('1' == enable[0]) && ('1' == mode[0]) )
		{
			sprintf(cmd, "ifconfig br0 add %s1/64",ipaddr);
			system(cmd);
			return SUCCESS;
		}
		else
		{
			sprintf(cmd,"ifconfig br0 del %s1/64", ipaddr);
			system(cmd);
		}

	}

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],"Dhcp6s");
	strcpy(nodeName[1],"Entry");
	if(
			(getAttrValue(top,nodeName,"Enable",enable) == TCAPI_PROCESS_OK) &&
			(getAttrValue(top,nodeName,"Mode",mode) == TCAPI_PROCESS_OK) 
	  )
	{
		char ipaddr[64] = {0};
		char cmd[128] = {0};
		if (getAttrValue(top,nodeName,"PrefixIPv6",ipaddr) != TCAPI_PROCESS_OK)
		{
			return FAIL;
		}

		tcdbg_printf("Enter set first loop, IP addr by Dhcp6s");
		if( ('1' == enable[0]) && ('1' == mode[0]) )
		{
			tcdbg_printf("Enter set br0 IP addr by Dhcp6s");
			sprintf(cmd, "ifconfig br0 add %s1/64",ipaddr);
			system(cmd);
			return SUCCESS;
		}
		else
		{
			sprintf(cmd,"ifconfig br0 del %s1/64", ipaddr);
			system(cmd);	
		}
		
	}
#endif
#endif
#ifdef TCSUPPORT_BRIDGE_MAC_LIMIT
	memset(nodeName,0,sizeof(nodeName));
  	strcpy(nodeName[0],LAN);
  	strcpy(nodeName[1],"BridgeMacLimit");

	if(getAttrValue(top, nodeName, "macLimitTotal", tmp) == TCAPI_PROCESS_OK)
	{
		memset(bml_cmd,0,64);
		sprintf(bml_cmd,"echo %d > /proc/br_fdb/mac_limit_total",atoi(tmp));
		system(bml_cmd);
	}

	for(i = 1; i < 5;i++)
	{
		memset(bml_attr,0,16);
		sprintf(bml_attr,"macLimitPort%d",i);
		if(getAttrValue(top, nodeName, bml_attr, tmp) == TCAPI_PROCESS_OK)
		{
			memset(bml_cmd,0,64);
			sprintf(bml_cmd,"echo %d-%d > /proc/br_fdb/mac_limit_by_port",i+2,atoi(tmp));/*eth0.1~eth0.4=>port:3~6*/
			system(bml_cmd);
		}
	}

	system("/userfs/bin/hw_nat -!");
#endif
	return SUCCESS;
}/* end lan_execute */
//

#ifdef ALIAS_IP
//"LanAlias",		ENTRY_TYPE|LAN_ALIAS_NUM, lan_merge_table, NULL, lan_write, lan_verify,  lan_execute},
char *
lanAlias_merge_table[MAX_NODE_NAME]=
{
	"lanAlias_id","",
};
/*______________________________________________________________________________
**	lanAlias_boot
**
**	descriptions:
**		the boot function of alias lan.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
lanAlias_boot(mxml_node_t *top){
	FILE *startupSh=NULL;
	char startupPath[24]={0};
	char startupCmd[48]={0};
	int i=0;
	for(i=0; i< LAN_ALIAS_NUM; i++){
		sprintf(startupPath,"/etc/lanAlias%d.conf",i);
		startupSh=fopen(startupPath,"r");
		if(startupSh){
			fclose(startupSh);
			sprintf(startupCmd,"/usr/script/lanAlias_start.sh %d",i);
			system(startupCmd);
		}
	}
	return SUCCESS;
}
/*______________________________________________________________________________
**	lanAlias_init
**
**	descriptions:
**		the init function of alias lan.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
lanAlias_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="LanAlias",
			.type=ENTRY_TYPE|LAN_ALIAS_NUM,
			.adv_table=NULL,
			.merge_table=lanAlias_merge_table,
			.cfg_read=NULL,
			.cfg_write=lanAlias_write,
			.cfg_verify=lanAlias_verify,
			.cfg_execute=lanAlias_execute,
			.cfg_boot=lanAlias_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}

/*______________________________________________________________________________
**	lanAlias_write
**
**	descriptions:
**		Find alias lan element from mxml tree then write alias **		lan's system parameter to specify
**		configuration files.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  alias lan cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
lanAlias_write(mxml_node_t *top, mxml_node_t *parant){
	mxml_node_t *node = NULL;
	char lanAlias_name[32]={0};
	int i=0;
	char element_name[ATTR_SIZE];
	int retval=0;
	for(i = 0; i < LAN_ALIAS_NUM; i++){
		sprintf(lanAlias_name, LAN_ALIAS_PATH, i);
		sprintf(element_name, "%s%d", SUB_NODE_NAME, i);
		node = mxmlFindElement(parant, parant, element_name,
		NULL, NULL, MXML_DESCEND);
		retval = node_write(node, lanAlias_name, QMARKS);
	}
	return retval;
}/* end lanAlias_write */

/*______________________________________________________________________________
**	lanAlias_verify
**
**	descriptions:
**		To check the system parameter of alias lan element  is validly or not.
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
lanAlias_verify(mxml_node_t *node){
	return SUCCESS;
}/* end lanAlias_verify */

/*_____________________________________________________________________________
**      function name: lanAlias_execute
**      descriptions:
**            To execute LanAlias node service.
**
**      parameters:
**            node:   Put the LanAlias of cfg node tree that is used to execute alias ip settings.
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
**      1. renyu 2009/10/12
**____________________________________________________________________________
*/
int
lanAlias_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	char string[32], tmp[64], nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char active[5]={0}; 
	int cur_alias=0;  
	if(get_entry_number(name[1],SUB_NODE_NAME,&cur_alias) != SUCCESS){
		memset(nodeName,0,sizeof(nodeName));
		strcpy(nodeName[0],WEBCURSET);
		strcpy(nodeName[1],SUB_NODE_NAME);
		/*get current AliasIP*/
		if(getAttrValue(top, nodeName, "lanAlias_id", tmp) == TCAPI_PROCESS_OK){
			cur_alias = atoi(tmp);
		}else{
			return FAIL;
		}
	}		
	/*stop lan alias Entryn*/
	sprintf(string, "%s %d",LAN_ALIAS_STOP_SH, cur_alias);
	system(string);
	
  	memset(nodeName,0,sizeof(nodeName));
  	strcpy(nodeName[0],LAN_ALIAS);	
	sprintf(nodeName[1], "%s%d", SUB_NODE_NAME, cur_alias);
	
	/*check active*/
	if(getAttrValue(top, nodeName, NODE_ACTIVE, active) != TCAPI_PROCESS_OK ){
		sprintf(tmp, LAN_ALIAS_PATH, cur_alias);
		remove(tmp);
		return FAIL;
	}

	if(!strcmp(active,ACTIVE)){
		/* start alias ip*/
		sprintf(string, "%s %d",LAN_ALIAS_START_SH, cur_alias);
		system(string);
	}
	return SUCCESS;
}/* end lanAlias_execute */
#endif
#if 0  /*Orginal ci-command mode */
/*______________________________________________________________________________
**	wlan_read
**
**	descriptions:
**		Read wlan's system parameter from configuration file and record those information
**		to mxml_node_t.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
wlan_read(mxml_node_t *top){
	int fd=0;
	int i=0;
 	char value[MAXSIZE];
	char tmp_attr[32];
	char tmp[64];


	mxml_node_t *group = NULL;  /* <Cfg Node> */
	mxml_node_t *node = NULL;   /* <element node> */

	group = mxmlNewElement(top, WLAN);
	fd = open(WLAN_PATH,O_RDONLY);
	if(fd == FAIL){
		return FAIL;
	}
	else{
		node = mxmlNewElement(group, WLAN_ENTRY);

		while(strlen(wlan_attribute[i]) !=0){
			memset(value, 0, sizeof(value));
			sprintf(tmp_attr,"%s=", wlan_attribute[i]);
			get_profile_str(tmp_attr, value, NO_QMARKS, fd);
			if(strlen(value) != 0){
				/*
				To store ascii value.
				attribute name  = Key1Str or Key2Str or Key3Str or Key4Str
				*/
				if((wlan_attribute[i][0]== WLAN_KETSTR_K_CHAR)
				&& (wlan_attribute[i][4]== WLAN_KETSTR_S_CHAR)){
					Chr_to_Hex(value, tmp);
					strcpy(value, tmp);
				}
				mxmlElementSetAttr(node, wlan_attribute[i], value);
			}
			i++;
		}
	}
	close(fd);
	return 0;
}/* end wlan_read */
#endif

#ifdef TCSUPPORT_SYSLOG
int do_wlan_alarm_syslog(void)
{
	openlog("TCSysLog", 0, LOG_LOCAL1);
#if defined(TCSUPPORT_C1_CUC)
	syslog(LOG_ALERT, " ALARM LEV-1 ASTATUS-2 EVENTID-104012 WLAN Hardware Error.\n");
#else
   	syslog(LOG_ALERT, "104012 WLAN Hardware Error.\n");
#endif
	closelog();

	return 0;
}

int check_wlan_exist_for_syslog(mxml_node_t *top)
{
	char existValue[16] = {0};

	check_wireless_card(existValue);
	if(strcmp(existValue, "On") != 0) {
		do_wlan_alarm_syslog();
	}


	return 0;
}
#endif

#ifndef PURE_BRIDGE

#if defined(TCSUPPORT_WLAN_AC)
char *
wlan11ac_adv_table[MAX_NODE_NAME]=
{
	"Common",
#ifdef TCSUPPORT_WLAN_WDS
	"WDS",
#endif
	"",
};
char *
wlan11ac_merge_table[MAX_NODE_NAME]=
{
	"wlan_ac_id","",
};
/*______________________________________________________________________________
**	wlan_init
**
**	descriptions:
**		the init function of wlan.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
wlan11ac_init(void){
/*	tcdbg_printf("%s ",__func__); */
	int ret=0;
	cfg_node_t node={
			.name="WLan11ac",
			.type=UPDATE_TYPE|ADV_FLAG|ENTRY_TYPE|MAX_BSSID_NUM,
			.adv_table=wlan11ac_adv_table,
			.merge_table=wlan11ac_merge_table,
			.cfg_read=wlan11ac_read,/*add read function by xyyou for wps information */
			.cfg_write=wlan11ac_write,
			.cfg_verify=wlan11ac_verify,
			.cfg_execute=wlan11ac_execute,
			.cfg_boot=wlan11ac_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}

#ifndef BBUTOWBU
int
wlan11ac_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr){
#ifdef WSC_AP_SUPPORT/*add by xyyou	*/
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char iwpriv[5][128];
	char old_status[5][128];
	char tmp[1024]={0};
	int i=0, yes=0;
	FILE *fp;
	static int laststatus=0;
	enum attr_index{
		configured=0,
		ssid,
		auth,
		encryp,
		key
	};
	char attr_name[5][ATTR_SIZE]={
		{""},
		{"SSID"},
		{"AuthMode"},
		{"EncrypType"},
		{"WPAPSK"}
	};
	char confMode[2]={'\0'};

	/*only these attribute need do read function. shnwind 20110330.*/
	#if 0
	if(!((strcmp(attr,"WPSConfStatus") == 0)
		|| (strcmp(attr,"SSID") == 0)
		|| (strcmp(attr,"AuthMode") == 0)
		|| (strcmp(attr,"EncrypType") == 0)
		|| (strcmp(attr,"WPAPSK") == 0))){
		return 0;	
	}

	#endif
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],"WLan11ac");
	strcpy(nodeName[1],"Entry0");
	if(getAttrValue(top, nodeName, "WPSConfMode", confMode) != TCAPI_PROCESS_OK)
		return FAIL;
	if(!strcmp(confMode,"0"))
		return SUCCESS;

	system("/userfs/bin/iwpriv ra0 get_wsc_profile > /tmp/wps");
	fp = fopen("/tmp/wps", "r");
	if(fp){		
		while(fgets(tmp, 1024, fp)){
			if(strstr(tmp, "Yes")){
				yes=1;
				break;
			}else if(strstr(tmp, "No")){
				break;
			}
		}
		if(!WscOOBSeted_ac){
			if(yes==0){
				laststatus=0;
				fclose(fp);
				return SUCCESS;
			}
			if((yes==1)&&(laststatus==1)){
				fclose(fp);
				return SUCCESS;
			}

			laststatus=1;
			setAttrValue(top, nodeName, "WPSConfStatus","2");
		}else{
			setAttrValue(top, nodeName, "WPSConfStatus","1");	
			WscOOBSeted_ac=0;
		}
		
		memset(iwpriv, 0, sizeof(iwpriv));
#if defined(RT5392)
		char wsc_file[16] = {0};
		sprintf(wsc_file, "/tmp/wps");
		get_profile_str("Configured=", iwpriv[configured], sizeof(iwpriv[configured]), NO_QMARKS, wsc_file);
		get_profile_str("WscSsid=", iwpriv[ssid], sizeof(iwpriv[ssid]), NO_QMARKS, wsc_file);
		get_profile_str("AuthMode=", iwpriv[auth], sizeof(iwpriv[auth]), NO_QMARKS, wsc_file);
		get_profile_str("EncrypType=", iwpriv[encryp], sizeof(iwpriv[encryp]), NO_QMARKS, wsc_file);
		get_profile_str("Key=", iwpriv[key], sizeof(iwpriv[key]), NO_QMARKS, wsc_file);		
#else		
		sscanf(tmp, "%s %s %s %s %s", iwpriv[configured], iwpriv[ssid], iwpriv[auth], iwpriv[encryp], iwpriv[key]);

#endif
		for(i=0; i<5; i++){
			if(i == 0){
				continue;
			}
			getAttrValue(top, nodeName, attr_name[i], old_status[i]);
			if(strlen(iwpriv[i]) && strcmp(iwpriv[i], old_status[i])){
				setAttrValue(top, nodeName, attr_name[i], iwpriv[i]);
			}
		}
		fclose(fp);
	}

	return SUCCESS;
#else
	return SUCCESS;
#endif
}
#else
extern mxml_node_t **gTreePtr;
int
wlan11ac_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr){
#ifdef WSC_AP_SUPPORT/*add by xyyou	*/
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char iwpriv[5][128];
	char old_status[5][128];
	int i=0, yes=0;
	int j=0;
	tcapi_msg_t msg;
	int sameaddrstat = 0;
	int needSave = 0;

		enum attr_index{
		configured=0,
		ssid,
		auth,
		encryp,
		key
	};
	char attr_name[5][ATTR_SIZE]={
		{""},
		{"SSID"},
		{"AuthMode"},
		{"EncrypType"},
		{"WPAPSK"}
	};
	char confMode[2]={'\0'};

	int socket_id=0;
	int socket_id_acl=0;
	struct iwreq wrq;
	int ret=0;
	char data[768];
#ifdef MT7612E	
		WSC_CONFIGURED_VALUE_MT7612 * pwscprofile=NULL;
#else
	WSC_CONFIGURED_VALUE * pwscprofile=NULL;
#endif
	unsigned short tmpauthmode=0,tmpencryp=0;
	
	/*for ACL infor update from driver,added by fredli,20110916*/
	char tmp1[2]={'\0'};
	char tmp2[2]={'\0'};
	
	RT_802_11_ACL * AclInfor=NULL;
	char AclList[WLAN_MAC_NUM][18];
	char WLan_MAC[11];
	char WLan_MAC_Addr[18];
	/*end for ACL*/

	/*only these attribute need do read function. shnwind 20110330.*/
#if 0
	if(!((strcmp(attr,"WPSConfStatus") == 0)
		|| (strcmp(attr,"SSID") == 0)
		|| (strcmp(attr,"AuthMode") == 0)
		|| (strcmp(attr,"EncrypType") == 0)
		|| (strcmp(attr,"WPAPSK") == 0))){
		return 0;	
	}
#endif

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],"WLan11ac");
	strcpy(nodeName[1],"Entry0");
	if(getAttrValue(top, nodeName, "WPSConfMode", confMode) != TCAPI_PROCESS_OK){
		return FAIL;
	}
	if(!strcmp(confMode,"0")){
		return SUCCESS;
	}
	socket_id=socket(AF_INET, SOCK_DGRAM, 0);
	if(socket_id==-1){
		printf("==>wlan_read:Create socket fail\n");
		return FAIL;
	}
	strcpy(wrq.ifr_name, "rai0");
	wrq.u.data.length=sizeof(data);
	wrq.u.data.pointer=data;
	wrq.u.data.flags = 0;
	ret=ioctl(socket_id, RTPRIV_IOCTL_WSC_PROFILE, &wrq);
	if(ret!=0){
		printf("==>wlan11ac_read:ioctl open fail\n");
		close(socket_id);
		return FAIL;
	}
#ifdef MT7612E	
		pwscprofile = (WSC_CONFIGURED_VALUE_MT7612 *)wrq.u.data.pointer;
#else
	pwscprofile = (WSC_CONFIGURED_VALUE *)wrq.u.data.pointer;
#endif

		if(pwscprofile->WscConfigured==2){

			yes=1;
		}
		
		if(!WscOOBSeted_ac){
			if(yes==0){
				/*laststatus=0;*/
				
				close(socket_id);
				return SUCCESS;
			}
			
			#if 0
			if((yes==1)&&(laststatus==1)){
				close(socket_id);
				return SUCCESS;
			}

			laststatus=1;
			#endif

			setAttrValue(top, nodeName, "WPSConfStatus","2");
		}else{
			setAttrValue(top, nodeName, "WPSConfStatus","1");
			WscOOBSeted_ac=0;
			needSave=1;
		}
		
		memset(iwpriv, 0, sizeof(iwpriv));

		sprintf(iwpriv[ssid],"%s",pwscprofile->WscSsid);
		memmove((unsigned char *)(&tmpauthmode), (unsigned char *)(&pwscprofile->WscAuthMode),sizeof(unsigned short));
		if(tmpauthmode==(WSC_AUTHTYPE_WPAPSK | WSC_AUTHTYPE_WPA2PSK)){
			sprintf(iwpriv[auth],"%s","WPAPSKWPA2PSK");
		}else{
			sprintf(iwpriv[auth],"%s",WscGetAuthTypeStr(tmpauthmode));
		}	
		
		memmove((unsigned char *)(&tmpencryp), (unsigned char *)(&pwscprofile->WscEncrypType),sizeof(unsigned short));
		if(tmpencryp==(WSC_ENCRTYPE_TKIP | WSC_ENCRTYPE_AES)){
			sprintf(iwpriv[encryp],"%s","TKIPAES");
		}else{
			sprintf(iwpriv[encryp],"%s",WscGetEncryTypeStr(tmpencryp));
		}	
		sprintf(iwpriv[key],"%s",pwscprofile->WscWPAKey);

		for(i=0; i<5; i++){
			if(i == 0){
				continue;
			}
			getAttrValue(top, nodeName, attr_name[i], old_status[i]);
			if(strlen(iwpriv[i]) && strcmp(iwpriv[i], old_status[i])){
				setAttrValue(top, nodeName, attr_name[i], iwpriv[i]);
				needSave = 1;
			}
		}

	close(socket_id);
	
	
	/*ACL infor update from driver,added by fred 
	ACL information update action excuted only when WPS 2.0 being
	enabled and Access policy "Allow" */
	if(getAttrValue(top, nodeName, "WscV2Support", tmp1)== TCAPI_PROCESS_OK 
		&& getAttrValue(top, nodeName, "AccessPolicy", tmp2)== TCAPI_PROCESS_OK)
	{
		/*tmp1=1 means wps 2.0 being enabled, tmp2=1 means Access policy is "Allow"*/
		if(!strcmp(tmp1,"1") && !strcmp(tmp2,"1"))
		{
			socket_id_acl=socket(AF_INET, SOCK_DGRAM, 0);
			if(socket_id_acl==-1)
			{
				printf("==>wlan11ac_read:Create ACL socket fail\n");
				return FAIL;
			}
			strcpy(wrq.ifr_name, "rai0");
			memset(data, 0, sizeof(data));
			wrq.u.data.length=sizeof(data);
			wrq.u.data.pointer=data;
			wrq.u.data.flags = OID_802_11_ACL_LIST;
			ret=ioctl(socket_id_acl, RT_PRIV_IOCTL, &wrq);
			if(ret!=0)
			{
				/*tcdbg_printf("ret!=0, ret=%d,\n",ret);*/
				printf("==>wlan11ac_read:ioctl open fail\n");
				close(socket_id_acl);
				return FAIL;
			}
			/*tcdbg_printf("ACLInfor AclInfor->Policy\n");*/
			AclInfor = (RT_802_11_ACL *) wrq.u.data.pointer;
		
			for (i=0;i<WLAN_MAC_NUM;i++)
			{
				sameaddrstat = 0;
				/*tcdbg_printf("AclInfor->Entry[%d].Addr: %02x"
				":%02x:%02x:%02x:%02x:%02x \n"
				,i,AclInfor->Entry[i].Addr[0],AclInfor->Entry[i].Addr[1],
				AclInfor->Entry[i].Addr[2],AclInfor->Entry[i].Addr[3],
				AclInfor->Entry[i].Addr[4],AclInfor->Entry[i].Addr[5]);*/
				sprintf(AclList[i],"%02x:%02x:%02x:%02x:%02x:%02x",
				AclInfor->Entry[i].Addr[0],AclInfor->Entry[i].Addr[1],
				AclInfor->Entry[i].Addr[2],AclInfor->Entry[i].Addr[3],
				AclInfor->Entry[i].Addr[4],AclInfor->Entry[i].Addr[5]);
				if(strcmp(AclList[i],"00:00:00:00:00:00")){
					for (j=0;j<WLAN_MAC_NUM;j++){
						sprintf(WLan_MAC,"WLan_MAC%d",j); 
						if(getAttrValue(top, nodeName, WLan_MAC
							, WLan_MAC_Addr) == TCAPI_PROCESS_OK){
							if(!strcmp(WLan_MAC_Addr,AclList[i])){
								sameaddrstat = 1;
								break;
							}
						}
					}
					if(!sameaddrstat){
						for (j=0;j<WLAN_MAC_NUM;j++){
							sprintf(WLan_MAC,"WLan_MAC%d",j);
							if((getAttrValue(top, nodeName, WLan_MAC
								, WLan_MAC_Addr) == TCAPI_PROCESS_OK)
								&& (!strcmp(WLan_MAC_Addr,"00:00:00:00:00:00")
								|| !strcmp(WLan_MAC_Addr,""))){
								if(setAttrValue(top, nodeName, WLan_MAC
									, AclList[i]) == TCAPI_PROCESS_OK){
									needSave =1;
									break;
								}
							}
						}
					}
				}
			  }
			
			close(socket_id_acl);
		  }
		}

	if(needSave){
		memset(&msg,0,sizeof(msg));
		tcapi_save_req(*gTreePtr, &msg);
	}
	return SUCCESS;
#else
	return SUCCESS;
#endif
}

#endif

/*______________________________________________________________________________
**	wlan_write
**
**	descriptions:
**		Find wlan element from mxml tree then write wlan's system parameter to specify
**		configuration files.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  wlan cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
wlan11ac_write(mxml_node_t *top, mxml_node_t *parant){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], APOn[4]={0}, BssidNum[4]={0}, tmp[128]={0},tmp2[128]={0};
	FILE *fp;
	int i = 0;
#if defined(RT3390) || defined(RT3090) || defined(RT5392)  || defined(MT7610E)  || defined(MT7612E) /*xyyou_efuse*/
#ifdef TCSUPPORT_MTD_ENCHANCEMENT
	int fd,len=0;
	char tempbuffer[512];
    unsigned short id;
#endif
#endif
	/*save the APOn and Bssid_num to one file and  this file will be read at cpe boot.*/
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],"WLan11ac");
	strcpy(nodeName[1],"Common");

	if((getAttrValue(top, nodeName, "APOn", APOn) != TCAPI_PROCESS_OK)
		|| (getAttrValue(top, nodeName, "BssidNum", BssidNum) 
		!= TCAPI_PROCESS_OK)){
		return FAIL;
	}

	fp = fopen(WLAN_AC_APON_PATH,"w");
	if(fp != NULL){
		sprintf(tmp,"APOn=%s\nBssid_num=%s\n",APOn,BssidNum);
		fputs(tmp, fp);
#ifdef TCSUPPORT_WLAN_PERSSID_SWITCH
		for(i=0;i<atoi(BssidNum);i++){
			memset(nodeName,0,sizeof(nodeName));
			strcpy(nodeName[0],"WLan11ac");
			sprintf(tmp,"%s%d", SUB_NODE_NAME, i);
			strcpy(nodeName[1], tmp);

			memset(tmp2,0,sizeof(tmp2));
			if((getAttrValue(top, nodeName, "EnableSSID", tmp2)
				== TCAPI_PROCESS_OK)){
				if(tmp2){
					sprintf(tmp,"ssid%d=%s\n",i,tmp2);
	 				fputs(tmp, fp);
				}
			}else{/*if no EnableSSID of entry in romfile,up the wifi interface*/
				sprintf(tmp,"ssid%d=%d\n",i,1);
 				fputs(tmp, fp);	
			}
		}
#endif		
#ifdef TCSUPPORT_WLAN_WDS
		memset(nodeName,0,sizeof(nodeName));
		strcpy(nodeName[0],"WLan11ac");
		strcpy(nodeName[1],"WDS");
		memset(tmp,0,sizeof(tmp));
		memset(tmp2,0,sizeof(tmp2));
		
		if(getAttrValue(top, nodeName, "WdsEnable", tmp) != TCAPI_PROCESS_OK){

			fclose(fp);
			return FAIL;
		}
		sprintf(tmp2,"WdsEnable=%s\n",tmp);
		fputs(tmp2, fp);
		if(getAttrValue(top, nodeName, "WdsEncrypType", tmp) != TCAPI_PROCESS_OK){
			fclose(fp);
			return FAIL;
		}	
		sprintf(tmp2,"WdsEncrypType=%s\n",tmp);
		fputs(tmp2, fp);

		if(getAttrValue(top, nodeName, "WdsKey", tmp) != TCAPI_PROCESS_OK){
			fclose(fp);
			return FAIL;
		}
		sprintf(tmp2,"WdsKey=%s\n",tmp);
		fputs(tmp2, fp);
	
		for(i = 0; i<MAX_WDS_ENTRY; i++){
			sprintf(tmp2,"Wds_MAC%d",i);
			if(getAttrValue(top, nodeName, tmp2, tmp) != TCAPI_PROCESS_OK){
				fclose(fp);
				return FAIL;
			}	
			sprintf(tmp2,"Wds_MAC%d=%s\n",i,tmp);
			fputs(tmp2, fp);			
		}
#endif
		
		fclose(fp);
	}
	if(!strcmp(APOn,WLAN_APOFF)){
		unlink(WLAN_AC_SCRIPT_PATH);
	}else if(!strcmp(APOn,WLAN_APON)){
	#ifdef WSC_AP_SUPPORT/*add by xyyou*/
		if(run_ac_wps(top)==1){/*first check if wps should run*/
			memset(nodeName,0,sizeof(nodeName));
			strcpy(nodeName[0],"Info");
			strcpy(nodeName[1],"WLan11ac");
			setAttrValue(top, nodeName, "WPSActiveStatus", "0");
			return SUCCESS;
		}	
		if(wps_ac_oob(top)==1){
			memset(nodeName,0,sizeof(nodeName));
			strcpy(nodeName[0],"Info");
			strcpy(nodeName[1],"WLan11ac");
			setAttrValue(top, nodeName, "WPSOOBActive", "0");
			return SUCCESS;
		}
	#endif
		if(reset_ac_counter(top) == 1){
			memset(nodeName,0,sizeof(nodeName));
			strcpy(nodeName[0],"Info");
			strcpy(nodeName[1],"WLan11ac");
			setAttrValue(top, nodeName, "ReCounterActive", "0");
			return SUCCESS;
		}
	#ifdef WSC_AP_SUPPORT
		if(wps_ac_genpincode(top) == 1){
			memset(nodeName,0,sizeof(nodeName));
			strcpy(nodeName[0],"Info");
			strcpy(nodeName[1],"WLan11ac");
			setAttrValue(top, nodeName, "WPSGenPinCode", "0");
			return SUCCESS;
		}
	#endif
	#if defined(RT3390) || defined(RT3090) || defined(RT5392)  || defined(MT7610E) || defined(MT7612E)
		doCheck11acHtExtcha(top,NOT_AUTO_CHANNEL);
	#endif
		write_wlan11ac_exe_sh(top, atoi(BssidNum));
	}
	write_wlan11ac_config(top, atoi(BssidNum));

#if defined(RT3390) || defined(RT3090) || defined(RT5392)  || defined(MT7610E) || defined(MT7612E) /*xyyou_efuse*/
#ifdef TCSUPPORT_MTD_ENCHANCEMENT
	if(isFirstWrite_ac){/*load from flash*/
		tcdbg_printf("xyyou:isfirstwrite...offset=%x\n",EEPROM_RA_AC_OFFSET);
		sprintf(tmp,TC_FLASH_READ_CMD,"/tmp/MT7610EEPROM.bin"
			,(long unsigned int)512
			,(long unsigned int)EEPROM_RA_AC_OFFSET,"reservearea");
		system(tmp);
		/*bin to buffer*/
		fd=open("/tmp/MT7610EEPROM.bin",O_RDONLY);
		if(fd==FAIL){
			tcdbg_printf("open tmp/MT7610EEPROM.bin fail!!!\n");
			return FAIL;
		}
		len=read(fd,tempbuffer,512);
		close(fd);
		if((len!=0)&&(len!=-1)){
			tcdbg_printf("xyyou:the tmp/bin is not empty!!!\n");
			memmove(&id,tempbuffer,2);
			tcdbg_printf("xyyou:id in the bin=%x\n",id);

			if(((id&0xF0FF)!=0x1076)&& ((id&0xFFFF)!=0x6276)){
				tcdbg_printf("use default "
					"etc/Wireless/RT2860AP_AC/RT30xxEEPROM.bin\n");
			}else{
				system("cat /tmp/MT7610EEPROM.bin > "
					"/etc/Wireless/RT2860AP_AC/RT30xxEEPROM.bin");	
			}
			unlink("/tmp/MT7610EEPROM.bin");
		}
		isFirstWrite_ac=0;
	}
#endif
#endif
	return SUCCESS;
}/* end wlan11ac_write */

/*______________________________________________________________________________
**	wlan_verify
**
**	descriptions:
**		To check the system parameter of wlan element  is validly or not.
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
wlan11ac_verify(mxml_node_t *node){
	return SUCCESS;
}/* end wlan_verify */

/*_____________________________________________________________________________
**      function name: wlan_execute
**      descriptions:
**            To execute wlan node service.
**
**      parameters:
**            node:   Put the wlan of cfg node tree that is used to execute wlan settings.
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
wlan11ac_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){

	FILE *fp;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], APOn[4]={0}, 
		BssidNum[4]={0}, tmp[32]={0},tmp2[128]={0},channel[4]={0};
	int i;

#ifdef TCSUPPORT_BRIDGE_FASTPATH
	int fastbridge_enable = 1;//default enable
#endif	
	#if defined(RT3390) || defined(RT3090) || defined(RT5392)  || defined(MT7610E) || defined(MT7612E) /*xyyou_efuse*/
	#ifdef TCSUPPORT_MTD_ENCHANCEMENT
	char writebintoflash[4]={0};
	char tempbuf[128]={0};
	#endif
#ifdef TCSUPPORT_WLAN_WDS	
	int isWDSReloadWIFI = 0;
	int WdsActive = 0;
#endif
	#endif
#ifdef TCSUPPORT_WLAN_PERSSID_SWITCH
		int ismasterra = 0;
#endif
#ifdef TCSUPPORT_SYSLOG_ENHANCE
	check_wlan_exist_for_syslog(top);
#endif
	/*commit WLan_Common --> do nothing*/
	/*should check name , not nodeName. shnwind .*/
	if(strcmp(name[1],"Common") == 0){
		return TCAPI_PROCESS_OK;
       }

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],"WLan11ac");
	strcpy(nodeName[1],"Common");
	if((getAttrValue(top, nodeName, "APOn", APOn) != TCAPI_PROCESS_OK)
		|| (getAttrValue(top, nodeName, "BssidNum", BssidNum) != TCAPI_PROCESS_OK)){
		return FAIL;
	}
	
/*gcyin*/
#ifdef TCSUPPORT_WLAN_WDS
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],"WLan11ac");
	strcpy(nodeName[1],"WDS");
	memset(tmp,0,sizeof(tmp));
	/*WdsEnable */
	if(getAttrValue(top, nodeName, "WdsEnable", tmp) != TCAPI_PROCESS_OK){
		return FAIL;
	}else{
		WdsActive = atoi(tmp);
	}
	if(WdsActive != pre_sys_state.WdsEnable_11ac 
		|| (!strcmp(APOn,WLAN_APON) && (WdsActive == 1))){
		isWDSReloadWIFI = 1;
		pre_sys_state.WdsEnable_11ac = WdsActive;
	}
	/*WdsEncrypType */
	if(getAttrValue(top, nodeName, "WdsEncrypType", tmp) != TCAPI_PROCESS_OK){
		return FAIL;
	}
	if(strcmp(tmp,pre_sys_state.WdsEncrypType_11ac) != 0){
		isWDSReloadWIFI = 1;
		strcpy(pre_sys_state.WdsEncrypType_11ac,tmp);
	}

	/*WdsKey */
	if(getAttrValue(top, nodeName, "WdsKey", tmp) != TCAPI_PROCESS_OK){
		return FAIL;
	}
	
	if(strcmp(tmp,pre_sys_state.WdsKey_11ac) != 0){
		isWDSReloadWIFI = 1;
		strcpy(pre_sys_state.WdsKey_11ac,tmp);
	}
	
	/*Wds_MAC*/
	for(i = 0; i<MAX_WDS_ENTRY; i++){
		sprintf(tmp2,"Wds_MAC%d",i);
		if(getAttrValue(top, nodeName, tmp2, tmp) != TCAPI_PROCESS_OK){
			return FAIL;
		}
		if(strcmp(tmp,pre_sys_state.Wds_MAC_11ac[i]) != 0){
			isWDSReloadWIFI = 1;
			strcpy(pre_sys_state.Wds_MAC_11ac[i],tmp);
		}		
	}

	/*main ssid wep key change
	if wds is not enable ,main ssid wep key change ,not reload wifi module
	if wds enable,main ssid wep key change, reload wifi module*/
	if(WdsActive != 0){/*wds enable*/
		memset(nodeName,0,sizeof(nodeName));
		strcpy(nodeName[0],"WLan11ac");
		sprintf(nodeName[1],"%s%d",SUB_NODE_NAME,0);

		if(getAttrValue(top, nodeName, "AuthMode", tmp) != TCAPI_PROCESS_OK){
			return FAIL;
		}
		if(strstr(tmp, "WEP")){
			for(i = 0; i<MAX_WDS_ENTRY; i++){
				sprintf(tmp2,"Key%dStr",i);
				if(getAttrValue(top, nodeName, tmp2, tmp) != TCAPI_PROCESS_OK){
					/*it will cause wifi unnormal,
					because the subsequent code will not be executed */
					/*return FAIL;*/
					continue;
				}
				if(strcmp(tmp,pre_sys_state.WepKeyStr_11ac[i]) != 0){
					isWDSReloadWIFI = 1;
					strcpy(pre_sys_state.WepKeyStr_11ac[i],tmp);
				}				
			}
		}
		
		
	}
#endif

	/*need reload module when Bssid_num change*/
#ifdef TCSUPPORT_WLAN_WDS
	if((atoi(BssidNum) != pre_sys_state.wlan11ac_Bssid_num) 
		|| (isWDSReloadWIFI == 1))
#else
	if(atoi(BssidNum) != pre_sys_state.wlan11ac_Bssid_num)
#endif
	{	
		for(i=0;i<pre_sys_state.wlan11ac_Bssid_num;i++){
			sprintf(tmp,"ifconfig rai%d down",i);
			system(tmp);
			sprintf(tmp,"brctl delif br0 rai%d",i);
			system(tmp);			
		}
#ifdef TCSUPPORT_WLAN_WDS
		for(i = 0;i<MAX_WDS_ENTRY;i++){
			sprintf(tmp,"ifconfig wdsi%d down",i);
			system(tmp);
			sprintf(tmp,"brctl delif br0 wdsi%d",i);
			system(tmp);		
		}
#endif
#ifdef TCSUPPORT_BRIDGE_FASTPATH
		/*check fastbridge enable or not.*/
		fp = fopen("/proc/net/fastbridge","r");
		if ( fp != NULL){
			fread(tmp, sizeof(char), sizeof(tmp), fp);
			fclose(fp);
			fastbridge_enable = atoi(tmp); 
		}
		if(fastbridge_enable == 1){
			/*stop fastbridge before wifi module reload. fix crash. shnwind 20110501*/
			doValPut("/proc/net/fastbridge","0");
		}
#endif
#ifdef MT7610E
	system("rmmod mt7610e_ap");
	system("insmod /lib/modules/mt7610e_ap.ko");
#elif MT7612E
	system("rmmod mt7662e_ap");
	system("insmod /lib/modules/mt7662e_ap.ko");
#endif
#ifdef TCSUPPORT_BRIDGE_FASTPATH 
		if(fastbridge_enable == 1){
			doValPut("/proc/net/fastbridge","1");
		}	
#endif
		/*add by xyyou */
		for(i=0;i<atoi(BssidNum);i++){
			int up = 1;
#if defined(TCSUPPORT_FON)
			if (i==1)
				continue;
#endif
#ifdef TCSUPPORT_WLAN_PERSSID_SWITCH
			memset(nodeName,0,sizeof(nodeName));
			strcpy(nodeName[0],"WLan11ac");
			sprintf(nodeName[1], "%s%d", SUB_NODE_NAME, i);
			
			memset(tmp,0,sizeof(tmp));
			if((getAttrValue(top, nodeName, "EnableSSID", tmp)
				== TCAPI_PROCESS_OK)){
				if(!strcmp(tmp, WLAN_SSID_OFF)){
					up = 0;
					if(0 == i)
						ismasterra = 1;
				}
			}
#endif
			if(up){
	                   sprintf(tmp,"ifconfig rai%d up",i);
				system(tmp);
				sprintf(tmp,"brctl addif br0 rai%d",i);
				system(tmp);
				tcdbg_printf("add rai%d to br0\n",i);
			}
			#ifdef TCSUPPORT_WLAN_PERSSID_SWITCH
			else
			{
				if(ismasterra)
				{
					/*need do up and down operation when disable rai0,enable other raix*/
					system("ifconfig rai0 up");
					system("ifconfig rai0 down");
				}
			}
			#endif
		}
#ifdef TCSUPPORT_WLAN_WDS
		if(WdsActive != 0){
			for(i = 0;i<MAX_WDS_ENTRY;i++){
				sprintf(tmp,"ifconfig wdsi%d up",i);
				system(tmp);
				sprintf(tmp,"brctl addif br0 wdsi%d",i);
				system(tmp);
				tcdbg_printf("addif wdsi%d to br0\n",i);
			}
		}
#endif

		/*system("brctl show br0");*/
		/*end xyyou*/
		pre_sys_state.wlan11ac_Bssid_num = atoi(BssidNum);

	}
	/*add by xyyou, when select AutoChannel, we must reload the RT61AP.dat*/
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],"WLan11ac");
	strcpy(nodeName[1],"Common");
	if(getAttrValue(top, nodeName, "Channel", channel) == TCAPI_PROCESS_OK){
		if((atoi(channel)==0)&&(pre_sys_state.wlan11ac_chann!=0)){
			for(i=0;i<atoi(BssidNum);i++){
				sprintf(tmp, "ifconfig rai%d down",i);
				system(tmp);
			}
		}
		
		pre_sys_state.wlan11ac_chann = atoi(channel);
	}
	/*end xyyou*/

	if(!strcmp(APOn,WLAN_APOFF)){
		/*close all interface*/
		for(i=0;i<pre_sys_state.wlan11ac_Bssid_num;i++){
			sprintf(tmp,"ifconfig rai%d down",i);
			system(tmp);
			sprintf(tmp,"brctl delif br0 rai%d",i);
			system(tmp);
		}
#ifdef TCSUPPORT_WLAN_WDS
		for(i = 0;i<MAX_WDS_ENTRY;i++){
				sprintf(tmp,"ifconfig wdsi%d down",i);
				system(tmp);
				sprintf(tmp,"brctl delif br0 wdsi%d",i);
				system(tmp);
		}
#endif	
		#ifdef BBUTOWBU
		#ifdef WSC_AP_SUPPORT
		memset(nodeName,0,sizeof(nodeName));
		strcpy(nodeName[0],"Info");
		strcpy(nodeName[1],"WLan11ac");
		if(getAttrValue(top, nodeName, "wlanWPStimerRunning", tmp) == TCAPI_PROCESS_OK){
			if(atoi(tmp)==1){
				setAttrValue(top, nodeName, "wlanWPStimerRunning", "0");
			}
		}
		#endif
		#endif
	}else if(!strcmp(APOn,WLAN_APON)){
		/*start all interface */
		for(i=0;i < atoi(BssidNum);i++){
#if defined(TCSUPPORT_FON)
			if (i==1)
				continue;
#endif
		#ifdef TCSUPPORT_WLAN_PERSSID_SWITCH
			memset(nodeName,0,sizeof(nodeName));
			strcpy(nodeName[0],"WLan11ac");
			sprintf(nodeName[1], "%s%d", SUB_NODE_NAME, i);
			if(getAttrValue(top, nodeName, "EnableSSID", tmp) == TCAPI_PROCESS_OK){
				if(atoi(tmp)==1){
					sprintf(tmp, "ifconfig rai%d up",i);
					system(tmp);
					sprintf(tmp, "brctl addif br0 rai%d",i);
					system(tmp);
				}else if(atoi(tmp)==0){
					sprintf(tmp, "ifconfig rai%d down",i);
					system(tmp);
					sprintf(tmp, "brctl delif br0 rai%d",i);
					system(tmp);
				}else{
					tcdbg_printf("the value of EnableSSID is wrong!\n");
					return FAIL;
				}
			}else{
				sprintf(tmp,"ifconfig rai%d up",i);
				system(tmp);
				sprintf(tmp,"brctl addif br0 rai%d",i);
				system(tmp);				
			}
		#else
			sprintf(tmp,"ifconfig rai%d up",i);
			system(tmp);
			sprintf(tmp,"brctl addif br0 rai%d",i);
			system(tmp);
		#endif
		}
		#if defined(RT3390) || defined(RT3090) || defined(RT5392)  || defined(MT7610E) || defined(MT7612E) /*xyyou_efuse*/
		#ifdef TCSUPPORT_MTD_ENCHANCEMENT
		memset(nodeName,0,sizeof(nodeName));
		strcpy(nodeName[0],"WLan11ac");
		strcpy(nodeName[1],"Common");
		if(getAttrValue(top,nodeName,"WriteBinToFlash",writebintoflash)
			==TCAPI_PROCESS_OK){
			if(atoi(writebintoflash)==1){
				sprintf(tempbuf,TC_FLASH_WRITE_CMD
					,"/etc/Wireless/RT2860AP_AC/RT30xxEEPROM.bin"
					,(long unsigned int)512
					,(long unsigned int)EEPROM_RA_AC_OFFSET
					,"reservearea");
				tcdbg_printf("xyyou:write bin to flash !\r\n");
				system(tempbuf);
				setAttrValue(top,nodeName,"WriteBinToFlash","0");
				return SUCCESS;
			}
		}
		#endif
		#endif
		dowscd(top);
		/*execute WLAN setup script*/
		fp = fopen(WLAN_AC_SCRIPT_PATH,"r");
		if(fp != NULL){
 			fclose(fp);
#if defined(RT3390) || defined(RT3090) || defined(RT5392)  || defined(MT7610E) || defined(MT7612E)
  			system(WLAN_AC_SCRIPT_PATH);
#endif
		}
#if defined(TCSUPPORT_WLAN_8021X)
                /*replaced by fred li for wifi 802.1x, 2011,08*/
                /*rt2860apd_stop();*/
                rtdot1xd_stop();
                /*rt2860apd_start();*/
                rtdot1xd_start();

#endif
	}else{
		return FAIL;
	}
	
#if defined(TCSUPPORT_FON)
		memset(nodeName,0,sizeof(nodeName));
                strcpy(nodeName[0],"FonNet");
                strcpy(nodeName[1],"Entry");
		if((getAttrValue(top, nodeName, "Status", tmp) == TCAPI_PROCESS_OK)){
			if(!strcmp(tmp, "1")){
                                system("ifconfig rai1 up");
			        system("/userfs/bin/iwpriv rai1 set NoForwarding=1");
        			system("/userfs/bin/iwpriv rai1 set IgmpSnEnable=1");
        			system("/userfs/bin/iwpriv rai1 set NoForwardingMBCast=1");
				system("/userfs/bin/iwpriv rai1 set WscConfMode=0");

                        }
		}
#endif

	return SUCCESS;
}/* end wlan_execute */
int
wlan11ac_boot(mxml_node_t *top){

#ifdef TCSUPPORT_WLAN_WDS
	char node_name[MAX_ARG_NUM][MAX_NODE_NAME];
	int wds_enable = 0, i = 0;
	char wds_tmp[128] = {0};
	char tmp2[16];
#endif

#if defined(RT3390) || defined(RT3090) || defined(RT5392)  || defined(MT7610E) || defined(MT7612E)         
	char tmp[1024]={0};
	FILE *fp;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];

	char *rt_driver = NULL;
	int flag = 0;

	system("/userfs/bin/iwpriv rai0 e2p 0 > /tmp/rt_device");
	fp=fopen("/tmp/rt_device","r");
	if(fp){
		while(fgets(tmp,1024,fp)){
			if((rt_driver = strstr(tmp,"3090")) 
				|| (rt_driver = strstr(tmp,"3091")) 
				|| (rt_driver = strstr(tmp,"3092")) 
			|| (rt_driver = strstr(tmp,"3390"))
			|| (rt_driver = strstr(tmp,"5392"))
			|| (rt_driver = strstr(tmp,"5390")) 
			|| (rt_driver = strstr(tmp,"7610"))
			|| (rt_driver = strstr(tmp,"7612"))){
				memset(nodeName,0,sizeof(nodeName));
				strcpy(nodeName[0],"WLan11ac");
				strcpy(nodeName[1],"Common");
				rt_driver[4] = '\0';
				setAttrValue(top, nodeName, "rt_device",rt_driver);
				flag = 1;
				tcdbg_printf("read WLAN driver from rt_device success!\n");
				break;
			}
		}
		fclose(fp);	
	}
	if(flag == 0){
		tcdbg_printf("read WLAN driver from rt_device failed"
			",set with default value!\n");
		memset(nodeName,0,sizeof(nodeName));
		strcpy(nodeName[0],"WLan11ac");
		strcpy(nodeName[1],"Common");
		#if defined(MT7610E)
			setAttrValue(top, nodeName, "rt_device","7610");
		#endif
		#if defined(MT7612E)
			setAttrValue(top, nodeName, "rt_device","7612");
		#endif
	}
#endif
	dowscd(top);


	/* init wds pre state */
#ifdef TCSUPPORT_WLAN_WDS
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],"WLan11ac");
	strcpy(nodeName[1],"Common");
	if(getAttrValue(top, nodeName, "APOn", tmp) == TCAPI_PROCESS_OK){
		if(!strcmp(tmp, WLAN_APON)){
			memset(node_name, 0, sizeof(node_name));
			strcpy(node_name[0],"WLan11ac");
			strcpy(node_name[1],"WDS");
			memset(wds_tmp,0,sizeof(wds_tmp));

			/* wds enable/disable */
			if (getAttrValue(top, node_name, "WdsEnable", wds_tmp)
				!= TCAPI_PROCESS_OK){
				return FAIL;
			} 
			else {
				wds_enable = atoi(wds_tmp);
			}
			pre_sys_state.WdsEnable_11ac = wds_enable;

			
			/* WdsEncrypType */
			if(getAttrValue(top, node_name, "WdsEncrypType", wds_tmp)
				!= TCAPI_PROCESS_OK) {
				return FAIL;
			}
			strcpy(pre_sys_state.WdsEncrypType_11ac, wds_tmp);

			/* wds key */
			if(getAttrValue(top, node_name, "WdsKey", wds_tmp)
				!= TCAPI_PROCESS_OK){
				return FAIL;
			}
			strcpy(pre_sys_state.WdsKey_11ac, wds_tmp);

			/* wds mac */
			for(i = 0; i<MAX_WDS_ENTRY; i++){
				sprintf(tmp2,"Wds_MAC%d",i);
				if(getAttrValue(top, node_name, tmp2, tmp) != TCAPI_PROCESS_OK){
					return FAIL;
				}
				strcpy(pre_sys_state.Wds_MAC_11ac[i], tmp); 
			}

			/* WepKeyStr */
			if (wds_enable != 0) {/* wds enable */
				memset(node_name,0,sizeof(node_name));
				strcpy(node_name[0],"WLan11ac");
				sprintf(node_name[1],"%s%d",SUB_NODE_NAME,0);

				if(getAttrValue(top, node_name, "AuthMode", tmp)
					!= TCAPI_PROCESS_OK){
					return FAIL;
				}
				if (strstr(tmp, "WEP")) {
					for(i = 0; i<MAX_WDS_ENTRY; i++) {
						sprintf(tmp2,"Key%dStr",i);
						if(getAttrValue(top, node_name, tmp2, tmp)
							!= TCAPI_PROCESS_OK) {
							/*it will cause wifi unnormal,because the 
							subsequent code will not be executed */
							/* return FAIL; */
							continue;
						}
						strcpy(pre_sys_state.WepKeyStr_11ac[i],tmp);	
					}
				}
			}
		}
	}
#endif


	return SUCCESS;

}
#endif /* TCSUPPORT_WLAN_AC */

//"WLan",		ADV_FLAG|ENTRY_TYPE|MAX_BSSID_NUM,wlan_adv_table, wlan_merge_table,NULL, wlan_write, wlan_verify, wlan_execute},
char *
wlan_adv_table[MAX_NODE_NAME]=
{
	"Common","",
};
char *
wlan_merge_table[MAX_NODE_NAME]=
{
	"wlan_id","",
};
/*______________________________________________________________________________
**	wlan_init
**
**	descriptions:
**		the init function of wlan.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
wlan_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="WLan",
			.type=UPDATE_TYPE|ADV_FLAG|ENTRY_TYPE|MAX_BSSID_NUM,
			.adv_table=wlan_adv_table,
			.merge_table=wlan_merge_table,
			.cfg_read=wlan_read,/*add read function by xyyou for wps information */
			.cfg_write=wlan_write,
			.cfg_verify=wlan_verify,
			.cfg_execute=wlan_execute,
			.cfg_boot=wlan_boot,
			.cfg_pre_unset= wlan_pre_unset,
	};
	ret=register_cfg_node(&node);
	return ret;
}
#ifndef BBUTOWBU
int
wlan_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr){
#ifdef WSC_AP_SUPPORT//add by xyyou	
		char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
		char iwpriv[5][128];
		char old_status[5][128];
		char tmp[1024]={0};
		int i=0, yes=0;
		FILE *fp;
		static int laststatus=0;
		enum attr_index{
			configured=0,
			ssid,
			auth,
			encryp,
			key
		};
		char attr_name[5][ATTR_SIZE]={
			{""},
			{"SSID"},
			{"AuthMode"},
			{"EncrypType"},
			{"WPAPSK"}
		};
	
		char confMode[2]={'\0'};
	
		memset(nodeName,0,sizeof(nodeName));
		strcpy(nodeName[0],WLAN);
		strcpy(nodeName[1],"Entry0");
		if(getAttrValue(top, nodeName, "WPSConfMode", confMode) != TCAPI_PROCESS_OK)
			return FAIL;
		if(!strcmp(confMode,"0"))
			return SUCCESS;
		
		system("/userfs/bin/iwpriv ra0 get_wsc_profile > /tmp/wps");
		fp = fopen("/tmp/wps", "r");
		if(fp){ 	
			while(fgets(tmp, 1024, fp)){
				if(strstr(tmp, "Yes")){
					yes=1;
					break;
				}else if(strstr(tmp, "No")){
					break;
				}
			}
			if(!WscOOBSeted){
				if(yes==0){
					laststatus=0;
					fclose(fp);
					return SUCCESS;
				}
				if((yes==1)&&(laststatus==1)){
					fclose(fp);
					return SUCCESS;
				}
	
				laststatus=1;
				setAttrValue(top, nodeName, "WPSConfStatus","2");
				
				tcdbg_printf("wlan_read: set WPSConfStatus 2!!!\n\n");
			}else{
				setAttrValue(top, nodeName, "WPSConfStatus","1");	
				
				tcdbg_printf("wlan_read: set WPSConfStatus 1!!!\n\n");
				WscOOBSeted=0;
			}
			
			memset(iwpriv, 0, sizeof(iwpriv));
#if defined(RT5392)
			char wsc_file[16] = {0};
			sprintf(wsc_file, "/tmp/wps");
			get_profile_str("Configured=", iwpriv[configured], sizeof(iwpriv[configured]), NO_QMARKS, wsc_file);
			get_profile_str("WscSsid=", iwpriv[ssid], sizeof(iwpriv[ssid]), NO_QMARKS, wsc_file);
			get_profile_str("AuthMode=", iwpriv[auth], sizeof(iwpriv[auth]), NO_QMARKS, wsc_file);
			get_profile_str("EncrypType=", iwpriv[encryp], sizeof(iwpriv[encryp]), NO_QMARKS, wsc_file);
			get_profile_str("Key=", iwpriv[key], sizeof(iwpriv[key]), NO_QMARKS, wsc_file); 	
#else		
			sscanf(tmp, "%s %s %s %s %s", iwpriv[configured], iwpriv[ssid], iwpriv[auth], iwpriv[encryp], iwpriv[key]);
	
#endif
	
			for(i=0; i<5; i++){
				if(i == 0){
					continue;
				}
				getAttrValue(top, nodeName, attr_name[i], old_status[i]);
				if(strlen(iwpriv[i]) && strcmp(iwpriv[i], old_status[i])){
					setAttrValue(top, nodeName, attr_name[i], iwpriv[i]);
				}
			}
			fclose(fp);
		}
	
		return SUCCESS;
#else
		return SUCCESS;
#endif
}
#else
extern mxml_node_t **gTreePtr;
int
wlan_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr){
#ifdef WSC_AP_SUPPORT//add by xyyou	
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char iwpriv[5][128];
	char old_status[5][128];
	//char tmp[1024]={0};
	int i=0, yes=0;
	int j=0;
	tcapi_msg_t msg;
	int sameaddrstat = 0;
	int needSave = 0;
	//FILE *fp;
	//static int laststatus=0;
		enum attr_index{
		configured=0,
		ssid,
		auth,
		encryp,
		key
	};
	char attr_name[5][ATTR_SIZE]={
		{""},
		{"SSID"},
		{"AuthMode"},
		{"EncrypType"},
		{"WPAPSK"}
	};
	char confMode[2]={'\0'};

	int socket_id=0;
	int socket_id_acl=0;
	struct iwreq wrq;
	int ret=0;
	char data[768];
	WSC_CONFIGURED_VALUE * pwscprofile=NULL;
	unsigned short tmpauthmode=0,tmpencryp=0;
	
	//for ACL infor update from driver,added by fredli,20110916
	char tmp1[2]={'\0'};
	char tmp2[2]={'\0'};
	
	RT_802_11_ACL * AclInfor=NULL;
	char AclList[WLAN_MAC_NUM][18];
	char WLan_MAC[11];
	char WLan_MAC_Addr[18];
	//end for ACL

	//only these attribute need do read function. shnwind 20110330.
#if 0
	if(!((strcmp(attr,"WPSConfStatus") == 0)
		|| (strcmp(attr,"SSID") == 0)
		|| (strcmp(attr,"AuthMode") == 0)
		|| (strcmp(attr,"EncrypType") == 0)
		|| (strcmp(attr,"WPAPSK") == 0))){
		return 0;	
	}
#endif

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],WLAN);
	strcpy(nodeName[1],"Entry0");
	if(getAttrValue(top, nodeName, "WPSConfMode", confMode) != TCAPI_PROCESS_OK){
		return FAIL;
	}
	if(!strcmp(confMode,"0")){
		return SUCCESS;
	}
	socket_id=socket(AF_INET, SOCK_DGRAM, 0);
	if(socket_id==-1){
		printf("==>wlan_read:Create socket fail\n");
		return FAIL;
	}
	strcpy(wrq.ifr_name, "ra0");
	wrq.u.data.length=sizeof(data);
	wrq.u.data.pointer=data;
	wrq.u.data.flags = 0;
	ret=ioctl(socket_id, RTPRIV_IOCTL_WSC_PROFILE, &wrq);
	if(ret!=0){
		printf("==>wlan_read:ioctl open fail\n");
		close(socket_id);
		return FAIL;
	}
	pwscprofile = (WSC_CONFIGURED_VALUE *)wrq.u.data.pointer;

		if(pwscprofile->WscConfigured==2){

			yes=1;
		}
		
		if(!WscOOBSeted){
			if(yes==0){
				//laststatus=0;
				
				close(socket_id);
				return SUCCESS;
			}
			
			#if 0
			if((yes==1)&&(laststatus==1)){
				close(socket_id);
				return SUCCESS;
			}

			laststatus=1;
			#endif

			setAttrValue(top, nodeName, "WPSConfStatus","2");
		}else{
			setAttrValue(top, nodeName, "WPSConfStatus","1");			
			WscOOBSeted=0;
			needSave=1;
		}
		
		memset(iwpriv, 0, sizeof(iwpriv));

		sprintf(iwpriv[ssid],"%s",pwscprofile->WscSsid);
		memmove((unsigned char *)(&tmpauthmode), (unsigned char *)(&pwscprofile->WscAuthMode),sizeof(unsigned short));
		if(tmpauthmode==(WSC_AUTHTYPE_WPAPSK | WSC_AUTHTYPE_WPA2PSK)){
			sprintf(iwpriv[auth],"%s","WPAPSKWPA2PSK");
		}else{
			sprintf(iwpriv[auth],"%s",WscGetAuthTypeStr(tmpauthmode));
		}	
		
		memmove((unsigned char *)(&tmpencryp), (unsigned char *)(&pwscprofile->WscEncrypType),sizeof(unsigned short));
		if(tmpencryp==(WSC_ENCRTYPE_TKIP | WSC_ENCRTYPE_AES)){
			sprintf(iwpriv[encryp],"%s","TKIPAES");
		}else{
			sprintf(iwpriv[encryp],"%s",WscGetEncryTypeStr(tmpencryp));
		}	
		sprintf(iwpriv[key],"%s",pwscprofile->WscWPAKey);

		for(i=0; i<5; i++){
			if(i == 0){
				continue;
			}
			getAttrValue(top, nodeName, attr_name[i], old_status[i]);
			if(strlen(iwpriv[i]) && strcmp(iwpriv[i], old_status[i])){
				setAttrValue(top, nodeName, attr_name[i], iwpriv[i]);
				needSave = 1;
			}
		}

	close(socket_id);
	
	
	//ACL infor update from driver,added by fred 
	// ACL information update action excuted only when WPS 2.0 being enabled and Access policy "Allow" 
	if(getAttrValue(top, nodeName, "WscV2Support", tmp1)== TCAPI_PROCESS_OK 
		&& getAttrValue(top, nodeName, "AccessPolicy", tmp2)== TCAPI_PROCESS_OK)
	{
		//tmp1=1 means wps 2.0 being enabled, tmp2=1 means Access policy is "Allow"
		if(!strcmp(tmp1,"1") && !strcmp(tmp2,"1"))
		{
			socket_id_acl=socket(AF_INET, SOCK_DGRAM, 0);
			if(socket_id_acl==-1)
			{
				printf("==>wlan_read:Create ACL socket fail\n");
				return FAIL;
			}
			strcpy(wrq.ifr_name, "ra0");
			memset(data, 0, sizeof(data));
			wrq.u.data.length=sizeof(data);
			wrq.u.data.pointer=data;
			wrq.u.data.flags = OID_802_11_ACL_LIST;
			ret=ioctl(socket_id_acl, RT_PRIV_IOCTL, &wrq);
			if(ret!=0)
			{
				//tcdbg_printf("ret!=0, ret=%d,\n",ret);
				printf("==>wlan_read:ioctl open fail\n");
				close(socket_id_acl);
				return FAIL;
			}
			//tcdbg_printf("ACLInfor AclInfor->Policy\n");
			AclInfor = (RT_802_11_ACL *) wrq.u.data.pointer;
		
			for (i=0;i<WLAN_MAC_NUM;i++)
			{
				sameaddrstat = 0;
				//tcdbg_printf("AclInfor->Entry[%d].Addr: %02x:%02x:%02x:%02x:%02x:%02x \n",i,AclInfor->Entry[i].Addr[0],AclInfor->Entry[i].Addr[1],AclInfor->Entry[i].Addr[2],AclInfor->Entry[i].Addr[3],AclInfor->Entry[i].Addr[4],AclInfor->Entry[i].Addr[5]);
				sprintf(AclList[i],"%02x:%02x:%02x:%02x:%02x:%02x",AclInfor->Entry[i].Addr[0],AclInfor->Entry[i].Addr[1],AclInfor->Entry[i].Addr[2],AclInfor->Entry[i].Addr[3],AclInfor->Entry[i].Addr[4],AclInfor->Entry[i].Addr[5]);
				if(strcmp(AclList[i],"00:00:00:00:00:00")){
					for (j=0;j<WLAN_MAC_NUM;j++){
						sprintf(WLan_MAC,"WLan_MAC%d",j); 
						if(getAttrValue(top, nodeName, WLan_MAC, WLan_MAC_Addr) == TCAPI_PROCESS_OK){
							if(!strcmp(WLan_MAC_Addr,AclList[i])){
								sameaddrstat = 1;
								break;
							}
						}
					}
					if(!sameaddrstat){
						for (j=0;j<WLAN_MAC_NUM;j++){
							sprintf(WLan_MAC,"WLan_MAC%d",j);
							if((getAttrValue(top, nodeName, WLan_MAC, WLan_MAC_Addr) == TCAPI_PROCESS_OK) && (!strcmp(WLan_MAC_Addr,"00:00:00:00:00:00") || !strcmp(WLan_MAC_Addr,""))){
								if(setAttrValue(top, nodeName, WLan_MAC, AclList[i]) == TCAPI_PROCESS_OK){
									needSave =1;
									break;
								}
							}
						}
					}
				}
			  }
			
			close(socket_id_acl);
		  }
		}

	if(needSave){
		memset(&msg,0,sizeof(msg));
		tcapi_save_req(*gTreePtr, &msg);
	}
	return SUCCESS;
#else
	return SUCCESS;
#endif
}

#endif


/*______________________________________________________________________________
**	wlan_write
**
**	descriptions:
**		Find wlan element from mxml tree then write wlan's system parameter to specify
**		configuration files.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  wlan cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
wlan_write(mxml_node_t *top, mxml_node_t *parant){
#if 0
	FILE *fp=NULL;
	mxml_node_t   *node=NULL;
	mxml_attr_t   *attr=NULL;          /* Cirrent attribute */
	char buf[176]={0};
	//char tmp[64]={0};
	int       i=0;          /* Looping var */
	/*
	node = mxmlFindElement(tree, tree, WLAN_ENTRY,
		NULL, NULL,
		MXML_DESCEND);
	*/
	node = mxmlFindElement(parant, parant, SUB_NODE_NAME,
		NULL, NULL,MXML_DESCEND);

	if(node == NULL){
		return FAIL;
	}
	else{
		fp = fopen(WLAN_PATH,"w");

		if(fp == NULL){
			return FAIL;
		}
		else{
			fputs("Default\n",fp);
			/*Write user setting parameter into RT61.dat*/
			attr = node->value.element.attrs;
			for(i = 0; i < node->value.element.num_attrs; i++ ){
				/*
				Ascii value converse to character.
				attr = Key1Str or Key2Str or Key3Str or Key4Str
				*/
				/*if((*(attr->name) == WLAN_KETSTR_K_CHAR)
				&& (*(attr->name+4) == WLAN_KETSTR_S_CHAR)){*/
				if((strstr(attr->name,"Key"))&&(strstr(attr->name,"Str"))){
					//Hex_to_Chr(attr->value, tmp);
					if(*(attr->value) == '0' && *(attr->value+1) == 'x'){//hex
					  //key_index = attr->name[3]; ex: name = Key1Str;
            sprintf(buf,"Key%cType=0\n%s=%s\n",attr->name[3],attr->name, attr->value+2);
          }else{
            sprintf(buf,"Key%cType=1\n%s=%s\n",attr->name[3],attr->name, attr->value);
          }
				}
				else{
					sprintf(buf,"%s=%s\n",attr->name, attr->value);
				}

				fputs(buf,fp);
				attr++;
			}

			/*Write default parameter into RT61.dat*/
			for(i = 0; strlen(wlan_default_parm[i][0]) != 0; i++){
				sprintf(buf,"%s=%s\n", wlan_default_parm[i][0],wlan_default_parm[i][1]);
				fputs(buf,fp);
			}
		}
		fclose(fp);
	}
#endif
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], APOn[4]={0}, BssidNum[4]={0}, tmp[128]={0};
	FILE *fp;
	
#if defined(TCSUPPORT_CT_WLAN_NODE)
        char EnableSSID[4]={0}, element_name[16]={0};
        int i;
#endif
#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7592)
#ifdef TCSUPPORT_MTD_ENCHANCEMENT
	int fd,len=0;
	char tempbuffer[512];
    unsigned short id;
#endif
#endif
	/*save the APOn and Bssid_num to one file and  this file will be read at cpe boot.*/
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],WLAN);
	strcpy(nodeName[1],"Common");

	if((getAttrValue(top, nodeName, "APOn", APOn) != TCAPI_PROCESS_OK)
		|| (getAttrValue(top, nodeName, "BssidNum", BssidNum) != TCAPI_PROCESS_OK)){
		return FAIL;
	}

	fp = fopen(WLAN_APON_PATH,"w");
	if(fp != NULL){
		sprintf(tmp,"APOn=%s\nBssid_num=%s\n",APOn,BssidNum);
		fputs(tmp, fp);
#if defined(TCSUPPORT_CT_WLAN_NODE)
		//added by xmdai_nj : per ssid enable/disable
		for(i=0;i<atoi(BssidNum);i++){
			memset(nodeName,0,sizeof(nodeName));
			strcpy(nodeName[0],WLAN);
			sprintf(tmp,"%s%d", SUB_NODE_NAME, i);
			strcpy(nodeName[1], tmp);

			memset(EnableSSID,0,sizeof(EnableSSID));
			if((getAttrValue(top, nodeName, "EnableSSID", EnableSSID) != TCAPI_PROCESS_OK)){
				continue;
			}

			if(!strcmp(EnableSSID, WLAN_SSID_ON)){
				sprintf(tmp,"ssid%d=%s\n",i,EnableSSID);
 				fputs(tmp, fp);
			}
		}
#endif
		fclose(fp);
	}
	if(!strcmp(APOn,WLAN_APOFF)){
		unlink(WLAN_SCRIPT_PATH);
	}else if(!strcmp(APOn,WLAN_APON)){
	#ifdef WSC_AP_SUPPORT//add by xyyou
		if(run_wps(top)==1){/*first check if wps should run*/
			memset(nodeName,0,sizeof(nodeName));
			strcpy(nodeName[0],"Info");
			strcpy(nodeName[1],WLAN);
			setAttrValue(top, nodeName, "WPSActiveStatus", "0");
			return SUCCESS;
		}	
		if(wps_oob(top)==1){
			memset(nodeName,0,sizeof(nodeName));
			strcpy(nodeName[0],"Info");
			strcpy(nodeName[1],WLAN);
			setAttrValue(top, nodeName, "WPSOOBActive", "0");
			return SUCCESS;
		}
		
		if(wps_genpincode(top) == 1){
			memset(nodeName,0,sizeof(nodeName));
			strcpy(nodeName[0],"Info");
			strcpy(nodeName[1],WLAN);
			setAttrValue(top, nodeName, "WPSGenPinCode", "0");
			return SUCCESS;
		}
	#endif
		write_wlan_exe_sh(top, atoi(BssidNum));
	}
	write_wlan_config(top, atoi(BssidNum));

#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7592) //xyyou_efuse
#ifdef TCSUPPORT_MTD_ENCHANCEMENT
	if(isFirstWrite){//load from flash
		//tcdbg_printf("xyyou:isfirstwrite...offset=%x\n",EEPROM_RA_OFFSET);
		sprintf(tmp,TC_FLASH_READ_CMD,"/tmp/RT30xxEEPROM.bin",512,EEPROM_RA_OFFSET,"reservearea");
		system(tmp);
		//bin to buffer
		fd=open("/tmp/RT30xxEEPROM.bin",O_RDONLY);
		if(fd==FAIL){
			tcdbg_printf("open tmp/RT30xxEEPROM.bin fail!!!\n");
			return;
		}
		len=read(fd,tempbuffer,512);
		close(fd);
		if((len!=0)&&(len!=-1)){
			//tcdbg_printf("xyyou:the tmp/bin is not empty!!!\n");
			memmove(&id,tempbuffer,2);
			//tcdbg_printf("xyyou:id in the bin=%d\n",id);
#ifdef WLAN_MULTIDRIVER 			
			if(((id&0xF0FF)!=0x9033)&&((id&0xF0FF)!=0x9030)&&((id&0xF0FF)!=0x9053)&&((id&0xFFFF)!=0x9255)&&((id&0xFFFF)!=0x9335)){				
				tcdbg_printf("use default /etc/Wireless/RT2860AP/E2P/RTxx.bin\n");			
			}else{		
				sprintf(tmp,"cat /tmp/RT30xxEEPROM.bin > /etc/Wireless/RT2860AP/E2P/RT%xE2P.bin", ((id&0x00ff)<<8|(id&0xff00)>>8)); 							
				tcdbg_printf("e2p:cat string=%s\n",tmp);				
				system(tmp);			
			}
#else

			if(((id&0xF0FF)!=0x9033)&&((id&0xF0FF)!=0x9030)&&((id&0xF0FF)!=0x9053) && ((id&0xFFFF)!=0x9275)){
				tcdbg_printf("use default RT30xxEEPROM.bin\n");
			}else{
				system("cat /tmp/RT30xxEEPROM.bin > /etc/RT30xxEEPROM.bin");
			}
#endif//WLAN_MULTIDRIVER
			unlink("/tmp/RT30xxEEPROM.bin");
		}
		isFirstWrite=0;
	}
#endif
#endif
	return SUCCESS;
}/* end wlan_write */

/*______________________________________________________________________________
**	wlan_verify
**
**	descriptions:
**		To check the system parameter of wlan element  is validly or not.
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
wlan_verify(mxml_node_t *node){
	return SUCCESS;
}/* end wlan_verify */

/*_____________________________________________________________________________
**      function name: wlan_pre_unset
**      descriptions:
**            To stop wan service before deleting wlan node.
**
**      parameters:
**            node:   Put the wlan of cfg node tree that is used to execute wlan settings.
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
wlan_pre_unset(mxml_node_t *top, char name[][MAX_NODE_NAME])
{
	int  entry_index;
	char ssidVar[16] = {0};
#ifdef WAN_DBUG
	tcdbg_printf("wan_pre_unset start!\n");
#endif
	
	if(get_entry_number(name[1],"Entry",&entry_index) != SUCCESS ){
		return FAIL;
	}
	
	
	if(getAttrValue(top, name, "SSID", ssidVar) == TCAPI_PROCESS_OK ){

		char cmd[64] = {0};
		
		/*Stop this interface*/	
		sprintf(cmd, "/sbin/ifconfig ra%d down", entry_index);
		system(cmd);
		
	}
	else{
		return FAIL;
	}
	
	
	return SUCCESS;
}

/*_____________________________________________________________________________
**      function name: wlan_execute
**      descriptions:
**            To execute wlan node service.
**
**      parameters:
**            node:   Put the wlan of cfg node tree that is used to execute wlan settings.
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
wlan_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){

	FILE *fp;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], APOn[4]={0}, BssidNum[4]={0}, tmp[32]={0}, channel[4]={0};
#if defined(TCSUPPORT_CT_WLAN_NODE)
	char EnableSSID[4]={0};
#endif
	int i;

	#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7592) //xyyou_efuse
	#ifdef TCSUPPORT_MTD_ENCHANCEMENT
	char writebintoflash[4]={0};
	char tempbuf[128]={0};
	#endif
	#endif
#ifdef BBUTOWBU
#ifdef WSC_AP_SUPPORT

	char tmp2[128]={0};
#endif
#endif
	int WpsStatus=0;

#ifdef TCSUPPORT_SYSLOG
	check_wlan_exist_for_syslog(top);
#endif
	/*commit WLan_Common --> do nothing*/
	/*should check name,not nodeName*/
	if(strcmp(name[1],"Common") == 0){
		return TCAPI_PROCESS_OK;
       }

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],WLAN);
	strcpy(nodeName[1],"Common");
	if((getAttrValue(top, nodeName, "APOn", APOn) != TCAPI_PROCESS_OK)
		|| (getAttrValue(top, nodeName, "BssidNum", BssidNum) != TCAPI_PROCESS_OK)){		
		return FAIL;
	}
	/*need reload module when Bssid_num change*/
	if(atoi(BssidNum) != pre_sys_state.wlan_Bssid_num){
		for(i=0;i<pre_sys_state.wlan_Bssid_num;i++){
			sprintf(tmp,"ifconfig ra%d down",i);
			system(tmp);
		}
#ifdef WLAN_MULTIDRIVER 	
		system("rmmod rt3593ap");		
#if defined(TCSUPPORT_CT_WLAN_WAPI)
		system("rmmod wapi_module");		
		system("insmod /lib/modules/wapi_module.ko");
#endif
		system("insmod /lib/modules/rt3593ap.ko");		
		//tcdbg_printf("xyyou: insmod /lib/modules/rt3593ap.ko\n"); 
#else
#ifdef RT3390
		system("rmmod rt3390ap");
		system("insmod /lib/modules/rt3390ap.ko");
#elif RT3090
		system("rmmod rt3090ap");
		system("insmod /lib/modules/rt3090ap.ko");
#elif RT5392
		system("rmmod rt5390ap");
		system("insmod /lib/modules/rt5390ap.ko");
#elif MT7592
		system("rmmod mt7603eap");
		system("insmod /lib/modules/mt7603eap.ko");
#else
		system("rmmod rt61ap");/*need reload module*/
		system("insmod /lib/modules/rt61ap.ko");/*krammer change the path*/
#endif
#endif
		
		//add by xyyou 
		
		tcdbg_printf("wlan_execute: BssidNum = %s\n",BssidNum);
		for(i=0;i<atoi(BssidNum);i++){
#if defined(TCSUPPORT_CT_WLAN_NODE)
			//added by xmdai_nj : per ssid enable/disable.
			memset(nodeName,0,sizeof(nodeName));
			strcpy(nodeName[0],WLAN);
			sprintf(nodeName[1], "%s%d", SUB_NODE_NAME, i);

			memset(EnableSSID,0,sizeof(EnableSSID));
			if((getAttrValue(top, nodeName, "EnableSSID", EnableSSID) != TCAPI_PROCESS_OK)){
				//return FAIL;
				continue;
			}

			if(!strcmp(EnableSSID, WLAN_SSID_ON)){
				sprintf(tmp,"ifconfig ra%d up",i);
				system(tmp);
				sprintf(tmp,"brctl addif br0 ra%d",i);
				system(tmp);			
			}else{
				sprintf(tmp, "ifconfig ra%d down",i);
				system(tmp);
				sprintf(tmp,"brctl addif br0 ra%d",i);
				system(tmp);			
			}
#else
                        sprintf(tmp,"ifconfig ra%d up",i);
			system(tmp);
			sprintf(tmp,"brctl addif br0 ra%d",i);
			system(tmp);
			tcdbg_printf("add ra%d to br0\n",i);
#endif
		}
		//system("brctl show br0");
		//end xyyou
		pre_sys_state.wlan_Bssid_num = atoi(BssidNum);

	}
	//add by xyyou, when select AutoChannel, we must reload the RT61AP.dat
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],WLAN);
	strcpy(nodeName[1],"Common");
	if(getAttrValue(top, nodeName, "Channel", channel) == TCAPI_PROCESS_OK){
		if((atoi(channel)==0)&&(pre_sys_state.wlan_chann!=0)){
			for(i=0;i<atoi(BssidNum);i++){
				sprintf(tmp, "ifconfig ra%d down",i);
				system(tmp);
			}
		}
		pre_sys_state.wlan_chann = atoi(channel);
	}
	//end xyyou

	if(!strcmp(APOn,WLAN_APOFF)){
		/*close all interface*/
		for(i=0;i<pre_sys_state.wlan_Bssid_num;i++){
			sprintf(tmp,"ifconfig ra%d down",i);
			system(tmp);
		}
#ifdef BBUTOWBU
#ifdef WSC_AP_SUPPORT
		sprintf(tmp2, "echo %d > /proc/tc3162/wps_button\n", WLANSTOP);
		system(tmp2);
		memset(nodeName,0,sizeof(nodeName));
		strcpy(nodeName[0],"Info");
		strcpy(nodeName[1],WLAN);
		if(getAttrValue(top, nodeName, "wlanWPStimerRunning", tmp) == TCAPI_PROCESS_OK){
			if(atoi(tmp)==1){
				setAttrValue(top, nodeName, "wlanWPStimerRunning", "0");
			}
		}
#endif
#endif
		
	}else if(!strcmp(APOn,WLAN_APON)){
		/*start all interface */
		for(i=0;i < atoi(BssidNum);i++){
			sprintf(tmp,"ifconfig ra%d up",i);
			system(tmp);
#if defined(TCSUPPORT_CT_WLAN_NODE)
			memset(nodeName,0,sizeof(nodeName));
			strcpy(nodeName[0],WLAN);
			sprintf(nodeName[1],"Entry%d",i);				
			if(getAttrValue(top, nodeName, "EnableSSID", EnableSSID) == TCAPI_PROCESS_OK){
				if(atoi(EnableSSID)==0){
					sprintf(tmp, "ifconfig ra%d down",i);
					system(tmp);
				}
			}
#endif
		}
		#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7592)//xyyou_efuse
		#ifdef TCSUPPORT_MTD_ENCHANCEMENT
		memset(nodeName,0,sizeof(nodeName));
		strcpy(nodeName[0],WLAN);
		strcpy(nodeName[1],"Common");
		if(getAttrValue(top,nodeName,"WriteBinToFlash",writebintoflash)==TCAPI_PROCESS_OK){
			if(atoi(writebintoflash)==1){
				sprintf(tempbuf,TC_FLASH_WRITE_CMD,"/etc/RT30xxEEPROM.bin",512,EEPROM_RA_OFFSET,"reservearea");
				//tcdbg_printf("xyyou:write bin to flash tempbuf=%s\n",tempbuf);
				system(tempbuf);
				setAttrValue(top,nodeName,"WriteBinToFlash","0");
				return SUCCESS;
			}
		}
		#endif
		#endif
		dowscd(top);
		/*execute WLAN setup script*/
		fp = fopen(WLAN_SCRIPT_PATH,"r");
		if(fp != NULL){
 			fclose(fp);
#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7592)
  			system(WLAN_SCRIPT_PATH);
#ifdef BBUTOWBU
#ifdef WSC_AP_SUPPORT
						if(iswpsstart){
							//sleep(1);
							sprintf(tmp, "echo %d > /proc/tc3162/wps_button\n", WPSSTART);
							system(tmp);
						//	tcdbg_printf("xyyou:cmd=%s\n",tmp);
							iswpsstart=0;
							WpsStatus=1;
						}
#endif
#endif

			fp = fopen(WLAN_MCS_SCRIPT_PATH,"r");
			if(fp != NULL){
				fclose(fp);
  				system(WLAN_MCS_SCRIPT_PATH);
				unlink(WLAN_MCS_SCRIPT_PATH);
			}
#endif
		}
#if 0
		/*execution at background and restart boa*/
		fp = fopen(RESTART_BOA_SCRIPT,"r");
		if(fp != NULL){
 			fclose(fp);
			sprintf(tmp,"%s 1 &",RESTART_BOA_SCRIPT);
			system(tmp);
		}
#endif
	}else{
		return FAIL;
	}
#if defined(TCSUPPORT_CT)
#if defined(RT3390) || defined(RT3090) || defined(RT5392) //xyyou_efuse
#ifdef TCSUPPORT_MTD_ENCHANCEMENT
	/*Set WiFi TX Queue len 500, origin is 1000. To avoid skbuff exhaust when WiFi IGMP test in dirty environment*/
	for(i=0;i<atoi(BssidNum);i++)
	{
		sprintf(tempbuf, "ifconfig ra%d txqueuelen %d", i, WIFI_DEV_TXQUEUE_LEN);
		system(tempbuf);
	}
#endif
#endif

	if(!strcmp(APOn,WLAN_APON) && !WpsStatus){
		memset(nodeName,0,sizeof(nodeName));
		strcpy(nodeName[0],WLAN);
		strcpy(nodeName[1],"Common");
		if(getAttrValue(top, nodeName, "Channel", channel) == TCAPI_PROCESS_OK){
			if( atoi(channel)==0 ) {
				for(i=0;i<atoi(BssidNum);i++)
				{
#if defined(TCSUPPORT_CT_WLAN_NODE)
					memset(nodeName,0,sizeof(nodeName));
					strcpy(nodeName[0],WLAN);
					sprintf(nodeName[1],"Entry%d",i);
					memset(EnableSSID, 0, sizeof(EnableSSID));
					getAttrValue(top, nodeName, "EnableSSID", EnableSSID);
					if (atoi(EnableSSID) == 1) {
						sprintf(tmp, "ifconfig ra%d down",i);
						system(tmp);
						sprintf(tmp, "ifconfig ra%d up",i);
						system(tmp);
						tcdbg_printf("restart ra%d \n",i);
					}
#else
					sprintf(tmp, "ifconfig ra%d down",i);
					system(tmp);
					sprintf(tmp, "ifconfig ra%d up",i);
					system(tmp);
					tcdbg_printf("restart ra%d \n",i);
#endif
				}
			}
		}
	}

#endif

	return SUCCESS;
}/* end wlan_execute */
#if defined(TCSUPPORT_C1_CUC)
static int str_to_macNum(unsigned char *macstr, unsigned char *macnum)
{
        int temp[6] = {0};
        int i;
        
        sscanf(macstr, "%02x:%02x:%02x:%02x:%02x:%02x", &temp[0],&temp[1],&temp[2],&temp[3],&temp[4],&temp[5]);
        for(i=0;i<6;i++){
                macnum[i] = (unsigned char)temp[i];
        }
        return 0;
}
static int MAC2RandomStr_True(unsigned char *randomStr, const unsigned char *macStr, const unsigned char ssidNum)
{
	unsigned char macTemp[50] = "";
	unsigned char temp[50] = "";
	unsigned char partialMacTemp[3] = "";
	unsigned char outputSymbol[36] = "";
	int i = 0;
	
	if(randomStr == NULL || macStr == NULL){
		return -1;
	}
	/*Just take the last 6 character in 12-character-mac*/
	memset(partialMacTemp, 0, sizeof(partialMacTemp));
	memcpy(partialMacTemp, macStr+3, 3);
		
	memset(macTemp, 0, sizeof(macTemp));
	for(i=0; i<11; i++){
		memcpy(&macTemp[3*i], partialMacTemp, 3);
	}

	memcpy(outputSymbol, "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ", 36);
	memset(temp, 0, sizeof(temp));
	TCScrambler(macTemp, temp, 33);
	/*force it to be a alphanumeric symbol*/ 
	sprintf(randomStr, "%c%c%c%c", outputSymbol[temp[5]%36], outputSymbol[temp[13]%36], outputSymbol[temp[30]%36],outputSymbol[temp[31]%36]);	

	return 0;
}
static int doWlanMAC2SSID_CUC(mxml_node_t *tree, int ssidnum, char *out_buf)
{
	unsigned char ssid[33] = "";
	unsigned char mactemp[20] = "";
	unsigned char nm_MAC[7] = "";
	int mssid = 0;
	unsigned char randomTemp[8] = "";
	char buf[80] = "";	
	tcapi_msg_t msg;
	
	memset(&msg,0,sizeof(tcapi_msg_t));
	snprintf(msg.node,MAXLEN_NODE_NAME,"%s_%s","WLan","Common");
	strcpy(msg.attr,"SSIDPre");
	tcapi_get_req(tree,&msg);

	mssid = ssidnum;
	if(mssid<0 || mssid>3){
		printf("\r\nmssid can be 0~3.\n");
		return -1;
	}

	memset(mactemp, 0, sizeof(mactemp));
	memset(nm_MAC, 0, sizeof(nm_MAC));
	if(get_profile_str("LAN_MAC=",mactemp,sizeof(mactemp),NO_QMARKS, MAC_PATH)==-1){
		return -1;
	}
	str_to_macNum(mactemp,nm_MAC);

	memset(ssid, 0, sizeof(ssid));
	memset(buf, 0, sizeof(buf));
	memset(randomTemp, 0, sizeof(randomTemp));
	MAC2RandomStr_True(randomTemp, nm_MAC, 0);	//MAC2RandomStr_True will generate 2 random characters
	sprintf(ssid,"%s%s", msg.value, randomTemp);
	
	sprintf(buf,WLAN_SCRIPT_PREFIX , mssid, "SSID", ssid);
	system(buf);
	strcpy(out_buf, ssid);

	return 0;
}
static int doWlanMAC2WPAPSKKey_CUC(mxml_node_t *tree, char *out_buf)
{
	unsigned char mactemp[20] = "";
	unsigned char nm_MAC[7] = "";
	unsigned char wepkey[128] = "";
	unsigned char outputSymbol[30] = "abcdefghijklmnopqrstuvwxyz";	
	unsigned char bufTemp[30] = "";
	int i = 0;
	int len = 0;
	
	memset(mactemp, 0, sizeof(mactemp));
	memset(wepkey, 0, sizeof(wepkey));
	memset(bufTemp, 0, sizeof(bufTemp));

	memset(mactemp, 0, sizeof(mactemp));
	memset(nm_MAC, 0, sizeof(nm_MAC));
	if(get_profile_str("LAN_MAC=",mactemp,sizeof(mactemp),NO_QMARKS, MAC_PATH)==-1){
		return -1;
	}
	str_to_macNum(mactemp,nm_MAC);
		
	/* feature required useing RAS checksum as the key to MD5 
	 * linux firmware has no checksum header
	 * "$1$" indicate crypt useing MD5 hashing encryption method
	 */
	/*because crypt algorithm is case sensitive*/
	sprintf(mactemp, "%02x%02x%02x%02x%02x%02x", nm_MAC[0], nm_MAC[1], nm_MAC[2], nm_MAC[3], nm_MAC[4], nm_MAC[5]);
	strcpy(bufTemp, (char *)crypt(mactemp, "$1$")+4);
	len=strlen(bufTemp);
	/*convert binary to ASCII*/
	for (i = 0; i < 8 ; i++)
		wepkey[i] = outputSymbol[bufTemp[(i<<1)%len]%26];
	wepkey[i] = '\0';

	strcpy(out_buf, wepkey);
	return 0;
}
#endif
int
wlan_boot(mxml_node_t *top){
#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7592)          
	char tmp[1024]={0};
	FILE *fp;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
#if defined(TCSUPPORT_CT_E8GUI)
	char BssidNum[4]={0},DefaultCfg[4]={0},SSID[16]={0};
	int i;
	char Mac[20]={0}, temp[70] ={0};
#endif

	char *rt_driver = NULL;
	int flag = 0;
#if defined(TCSUPPORT_CT_JOYME)
	char sleepon[8] = {0};
#endif

#if defined(TCSUPPORT_C1_CUC)
	char buf[128]={0};
	tcapi_msg_t msg;
	memset(&msg,0,sizeof(tcapi_msg_t));
#endif

#if defined(TCSUPPORT_CT_PON_JS) || defined(TCSUPPORT_CT_NAMECHGNOTIFY)
	doNameChangeWLanInit(top);
#endif

	system("/userfs/bin/iwpriv ra0 e2p 0 > /tmp/rt_device");
	fp=fopen("/tmp/rt_device","r");
	if(fp){
		while(fgets(tmp,1024,fp)){
			if((rt_driver = strstr(tmp,"3090"))|| (rt_driver = strstr(tmp,"3091")) || (rt_driver = strstr(tmp,"3092")) 
			|| (rt_driver = strstr(tmp,"3390")) || (rt_driver = strstr(tmp,"5392")) || (rt_driver = strstr(tmp,"5390"))
			|| (rt_driver = strstr(tmp,"7601")) || (rt_driver = strstr(tmp,"7592"))){
				memset(nodeName,0,sizeof(nodeName));
				strcpy(nodeName[0],WLAN);
				strcpy(nodeName[1],"Common");
				rt_driver[4] = '\0';
				setAttrValue(top, nodeName, "rt_device",rt_driver);
				flag = 1;
				tcdbg_printf("read WLAN driver from rt_device success!\n");
				break;
			}
		}
		fclose(fp);	
	}
	if(flag == 0){
		tcdbg_printf("read WLAN driver from rt_device failed,set with default value!\n");
		memset(nodeName,0,sizeof(nodeName));
		strcpy(nodeName[0],WLAN);
		strcpy(nodeName[1],"Common");
		#if defined(RT5392)
			setAttrValue(top, nodeName, "rt_device","5392");
		#endif
		#if defined(RT3090) 
			setAttrValue(top, nodeName, "rt_device","3090");
		#endif
		#if defined(RT3390)
			setAttrValue(top, nodeName, "rt_device","3390");
		#endif
		#if defined(MT7592)
			setAttrValue(top, nodeName, "rt_device","7592");
		#endif
	}

#if defined(TCSUPPORT_CT_E8GUI)
	enum wanIFType_en{
		Ip_type=0,
		Netmask_type,
		Pridns_type,
		Secdns_type, //xyzhu_20091225 support Secondary dns server.
		Mac_type,
		PTP_type,	//get PPP Peer address for gateway
	};
	#if 0
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],WLAN);
	strcpy(nodeName[1],"Common");
	if((getAttrValue(top, nodeName, "DefaultCfg", DefaultCfg) != TCAPI_PROCESS_OK)
		|| (getAttrValue(top, nodeName, "BssidNum", BssidNum) != TCAPI_PROCESS_OK)){
		//return FAIL;
		strcpy(DefaultCfg,"0");
	}
	if(atoi(DefaultCfg)==1){
		setAttrValue(top, nodeName, "DefaultCfg", "0");
		for(i=0;i < atoi(BssidNum);i++){
			memset(nodeName,0,sizeof(nodeName));
			strcpy(nodeName[0],WLAN);
			sprintf(nodeName[1],"Entry%d",i);
			getWanInfo(Mac_type, LAN_IF, Mac);
			sprintf(temp, "%c%c%c%c%c%c", Mac[9]+(char)i, Mac[10], Mac[12], Mac[13], Mac[15], Mac[16]);
			strcpy(Mac, temp);
			memset(temp, 0, sizeof(temp));
			strcpy(temp,"WWWW");
			macparsedforSSID(Mac, temp, 4);
			sprintf(SSID, "ChinaNet-%c%c%c%c",temp[0],temp[1],temp[2],temp[3]);
			setAttrValue(top, nodeName, "SSID",SSID);
		}
		//reactive current ssid
		tcapi_msg_t msg;
		memset(&msg,0,sizeof(msg));
		strcpy(msg.node, WLAN);
		tcapi_commit_req(top, &msg);
	}
	#else
#if !defined(TCSUPPORT_RESERVEAREA_EXTEND) && !defined(TCSUPPORT_C7)&& !defined(TCSUPPORT_CY) && !(defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505))
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],WLAN);
	strcpy(nodeName[1],WLAN_ENTRY0);
	if(getAttrValue(top, nodeName, "DefaultCfg", DefaultCfg) != TCAPI_PROCESS_OK){
		strcpy(DefaultCfg,"0");
	}
	if(atoi(DefaultCfg) == 1){
		/*need to auto ssid for ssid0!!!*/
		setAttrValue(top, nodeName, "DefaultCfg", "0");
#if defined(TCSUPPORT_C1_CUC)
		/* set ssid to driver, and copy ssid to buf*/
		doWlanMAC2SSID_CUC(top, 0, buf);
				
		/* set ssid1 to tcapi tree WLan_Entry0 */
		snprintf(msg.node,MAXLEN_NODE_NAME,"%s_%s%d",WLAN,SUB_NODE_NAME,0);
		strcpy(msg.attr,"SSID");
		strcpy(msg.value,buf);
		tcapi_set_req(top, &msg);
		/* set wpapskkey to driver */
		
		doWlanMAC2WPAPSKKey_CUC(top, buf);
		/* set wpapskkey to tcapi tree Wlan_Entry*/
		memset(msg.attr,0,sizeof(msg.attr));
		memset(msg.value,0,sizeof(msg.value));
		strcpy(msg.attr,"WPAPSK");
		strcpy(msg.value,buf);
		tcapi_set_req(top, &msg);
		
		tcapi_commit_req(top, &msg);
		memset(&msg,0,sizeof(tcapi_msg_t));
		snprintf(msg.node,MAXLEN_NODE_NAME,"%s_%s%d",ACCOUNT,SUB_NODE_NAME,1);
		strcpy(msg.attr,"web_passwd");
		strcpy(msg.value,buf);
		tcapi_set_req(top, &msg);
		
		/* tcapi commit */
		tcapi_commit_req(top, &msg);
#else
	
		getWanInfo(Mac_type, LAN_IF, Mac);
		sprintf(temp, "%c%c%c%c%c%c", Mac[9], Mac[10], Mac[12], Mac[13], Mac[15], Mac[16]);
		strcpy(Mac, temp);
		memset(temp, 0, sizeof(temp));
		strcpy(temp,"WWWW");
		macparsedforSSID(Mac, temp, 4);
#if defined(TCSUPPORT_CUC)	
		sprintf(SSID, "CU-%c%c%c%c",temp[0],temp[1],temp[2],temp[3]);
#else
		sprintf(SSID, "ChinaNet-%c%c%c%c",temp[0],temp[1],temp[2],temp[3]);
#endif
		setAttrValue(top, nodeName, "SSID",SSID);
		//reactive current ssid
		tcapi_msg_t msg;
		memset(&msg,0,sizeof(msg));
		strcpy(msg.node, WLAN);
		tcapi_commit_req(top, &msg);
#endif
	}
	#endif
#endif
#endif
#if defined(TCSUPPORT_CT)	
	/*Set WiFi TX Queue len 500, origin is 1000. To avoid skbuff exhaust when WiFi IGMP test in dirty environment*/
	for(i=0;i<atoi(BssidNum);i++)
	{
		sprintf(tmp, "ifconfig ra%d txqueuelen %d", i, WIFI_DEV_TXQUEUE_LEN);
		system(tmp);
	}
#endif
#if defined(TCSUPPORT_CT_JOYME)
	memset(sleepon, 0, sizeof(sleepon));
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],WLAN);
	strcpy(nodeName[1],"Common");
	if((getAttrValue(top, nodeName, "SleepFlag", sleepon) == TCAPI_PROCESS_OK) && (strcmp(sleepon,"1")==0))
	{
		setAttrValue(top, nodeName, "SleepFlag", "0");
		setAttrValue(top, nodeName, "APOn", "1");
		tcapi_msg_t msg;
		memset(&msg,0,sizeof(msg));
		strcpy(msg.node, WLAN);
		tcapi_commit_req(top, &msg);
		tcapi_save_req(top, &msg);
	}
#endif

#endif

	dowscd(top);
	return SUCCESS;

}
#if defined(TCSUPPORT_CT_E8GUI)
void
TCScrambler(uint8_t *din, uint8_t *dout, uint16_t data_len){

    uint32_t shreg=0x12345678;
    uint16_t i, byte_out;

    for(i = 0; i < data_len; i++){
        byte_out = (uint16_t)(din[i] ^ (shreg & 0xff) ^ ((shreg >> 5) & 0xff));
        dout[i] = (uint8_t)byte_out;
        shreg = (shreg >> 8) | (byte_out << 15);
    }
} // scrambler

void
macparsedforSSID(const char *mac, char *dout, int len){
	const char str_ignored[] = {"0oOB8I1l"};
	unsigned char temp[70];
	unsigned char temp_mac[70];
	char ctemp = 0;
	int iFlag = 0;
	int i=0, j=0, k=0;
	
	memset(temp, 0, sizeof(temp));
	memset(temp_mac, 0, sizeof(temp_mac));
	for(i=0; i<11; i++){
		memcpy(&temp_mac[6*i], mac, 6);
	}
	TCScrambler(temp_mac, temp, 66);
	for(i=0; i<66; i++){
		if(k == len)	
			break;
		
		if(0 == k){
			sprintf(&ctemp, "%c", ('A' + temp[i]%26));
		}
		else if(1 == k){
			sprintf(&ctemp, "%d", temp[i]%10);
		}
		else if(2 == k){
			sprintf(&ctemp, "%c", ('a' + temp[i]%26));
		}
		else if(3 == k){
			sprintf(&ctemp, "%d", temp[i]%10);
		}
		else{
			sprintf(&ctemp, "%c", ('a' + temp[i]%26));
		}
		
		for(j=0; j<8; j++){
			if(str_ignored[j] == ctemp){
				iFlag = 1;//find
				break;
			}
		}
		if(0 == iFlag){
			dout[k] = ctemp;
			k++;
		}
		iFlag = 0;
	}
}
#endif
void
dowscd(mxml_node_t *top){
	char value[4];
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	//char multiroute[128]={0};
	char lan_ip[32] = {0};
	char wscdpid[128]={0};
	char upnpd_stat[8]={0};

	system("killall -9 wscd");

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], "Upnpd");
	strcpy(nodeName[1], SUB_NODE_NAME);
	if (getAttrValue(top, nodeName, NODE_ACTIVE, upnpd_stat)==TCAPI_PROCESS_OK){
		if(strcmp(upnpd_stat, ACTIVE) != 0){
			return;
		}	
	}	
	
	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], WLAN);
	sprintf(nodeName[1], "%s%d", SUB_NODE_NAME,0);	
	if(getAttrValue(top,nodeName, "WPSConfMode", value)==TCAPI_PROCESS_OK){
		
		if(atoi(value)!=0){
			/*mark this route, because when switch upnp on page, it will cause the lan pc can not ping to the CPE. evan.jiang_20120517*/
			//sprintf(multiroute, "/sbin/route add -net 239.0.0.0 netmask 255.0.0.0 dev %s", "eth0");
			//system(multiroute);
                	memset(nodeName, 0, sizeof(nodeName));
	                strcpy(nodeName[0], LAN);
	                sprintf(nodeName[1], "%s", LAN_ENTRY_0);
		        if(getAttrValue(top,nodeName, LAN_IP, lan_ip)==TCAPI_PROCESS_OK)
		    	{
		    				sprintf(wscdpid, "/usr/bin/wscd -i ra0 -a %s -m 1 &",lan_ip);
						system(wscdpid);
		    	
		    	}
		}
	}

#if defined(TCSUPPORT_WLAN_AC)
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], "WLan11ac");
	sprintf(nodeName[1], "%s%d", SUB_NODE_NAME,0);	
	if(getAttrValue(top,nodeName, "WPSConfMode", value)==TCAPI_PROCESS_OK){
		
		if(atoi(value)!=0){
                	memset(nodeName, 0, sizeof(nodeName));
	                strcpy(nodeName[0], LAN);
	                sprintf(nodeName[1], "%s", LAN_ENTRY_0);
		        if(getAttrValue(top,nodeName, LAN_IP, lan_ip)==TCAPI_PROCESS_OK)
		    	{	
		    		memset(wscdpid, 0, sizeof(wscdpid));
		    		sprintf(wscdpid, "/usr/bin/wscd -i rai0 -a %s -m 1 &",lan_ip);
				system(wscdpid);
		    	}
		}
	}
#endif
	return;
	
}
#if 0  /*Orginal ci-command mode */
/*______________________________________________________________________________
**	ripd_read
**
**	descriptions:
**		Read ripd system parameter from configuration file and record those information
**		to mxml_node_t.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
ripd_read(mxml_node_t *top){

	int retval=0;
	char *pAttr=NULL;
	/*Read lan_rip.conf*/
	pAttr=(char *)ripd_attribute;
	retval=node_read(top,LANRIPD, LANRIPD_PATH, pAttr, QMARKS);
	return retval;
}/* end ripd_read */
#endif

#if defined(TCSUPPORT_RIPD)
char *
ripd_adv_table[MAX_NODE_NAME]=
{
	"LanIF","",
};

char *
ripd_merge_table[MAX_NODE_NAME]=
{
	"ripd_id","",
};

//"LanRipd",	SINGLE_TYPE,	NULL,	NULL, NULL, ripd_write, ripd_verify, ripd_execute},
/*______________________________________________________________________________
**	ripd_init
**
**	descriptions:
**		the init function of ripd.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
ripd_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="LanRipd",
			//.type=SINGLE_TYPE,
			.type= ADV_FLAG|ENTRY_TYPE|MAX_WAN_IF_INDEX,
			.adv_table=ripd_adv_table,
			.merge_table=ripd_merge_table,
			.cfg_read=NULL,
			.cfg_write=ripd_write,
			.cfg_verify=ripd_verify,
			.cfg_execute=ripd_execute,
			.cfg_boot=ripd_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}

/*______________________________________________________________________________
**	ripd_boot
**
**	descriptions:
**		the boot function of ripd.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**_______________________________________________________________________________
*/
int
ripd_boot(mxml_node_t *top){
	int fd;

	fd = open(ZEBRA_PATH,O_RDONLY);
	if(fd != -1){
		close(fd);
		system("/userfs/bin/zebra -f /etc/zebra.conf -i /var/run/zebra.pid &");
		system("/userfs/bin/ripd -f /etc/ripd.conf -i /var/run/ripd.pid &");
	}
	return SUCCESS;
}

/*______________________________________________________________________________
**	ripd_write
**
**	descriptions:
**		Find ripd element from mxml tree then write ripd's system parameter to specify
**		configuration files.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  LanRipd cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
ripd_write(mxml_node_t *top, mxml_node_t *parant){

	int retval=0;
	mxml_node_t *node = NULL;
	/*
	node = mxmlFindElement(tree, tree, LANRIPD_ENTRY,
		NULL, NULL,
		MXML_DESCEND);
	*/
	node = mxmlFindElement(parant, parant, RIPD_LAN_CONF_NODE,
		NULL, NULL, MXML_DESCEND);

	/*Create lan_rip.conf file*/
	retval = node_write(node, LANRIPD_PATH, QMARKS);

	/*Create zebra.conf  & ripd.conf file*/
	retval+=create_rip_conf(top);
	return retval;
}/* end ripd_write */

/*______________________________________________________________________________
**	ripd_verify
**
**	descriptions:
**		To check the system parameter of ripd element  is validly or not.
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
ripd_verify(mxml_node_t *node){
	return SUCCESS;
}/* end firewall_read */

/*_____________________________________________________________________________
**      function name: ripd_execute
**      descriptions:
**            To execute ripd node service.
**
**      parameters:
**            node:   Put the ripd of cfg node tree that is used to execute ripd settings.
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
ripd_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	int fd;

	kill_process("/var/run/zebra.pid");
	kill_process("/var/run/ripd.pid");
	fd = open(ZEBRA_PATH,O_RDONLY);
	if(fd != -1){
		close(fd);
		system("/userfs/bin/zebra -f /etc/zebra.conf -i /var/run/zebra.pid &");
		system("/userfs/bin/ripd -f /etc/ripd.conf -i /var/run/ripd.pid &");
	}
	return SUCCESS;
}/* end ripd_execute */
#endif

#if 0  /*Orginal ci-command mode */
/*______________________________________________________________________________
**	dproxy_read
**
**	descriptions:
**		Read dproxy system parameter from configuration file and record those information
**		to mxml_node_t.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
dproxy_read(mxml_node_t *top){

	FILE *fp=NULL;
 	char tmp[MAXSIZE];
	int i=0;
	mxml_node_t *group= NULL;
	mxml_node_t *node= NULL;   /* <node> */
	char dns_info[3][16];

	memset(dns_info, 0, sizeof(dns_info));

	group = mxmlNewElement(top, DPROXY);
	/* dproxy.conf dproxy.auto*/
	fp = fopen(DPROXY_PATH, "r");
	if(fp == NULL){
		return FAIL;
	}else{
		/*Get parameters from dproxy.conf file*/
		while(fgets(tmp, MAXSIZE, fp)!=NULL && strlen(dproxy_keyword[i]) != 0){
			if(strstr(tmp, dproxy_keyword[i]) != NULL){
				sprintf(dns_info[i+1],"%s", tmp+DNS_SHIFT_INDEX);
				dns_info[i+1][strlen(dns_info[i+1])-1]='\0';
				i++;
			}
		}
		fclose(fp);

		/*Get parameters from dproxy.auto file*/
		fp = fopen(DPROXYAUTO_PATH, "r");
		if(fp == NULL){
			return FAIL;
		}

		if(fgets(tmp, MAXSIZE, fp)){
			sprintf(dns_info[DPROXY_TYPE_INDEX],"%d",atoi(tmp));
		}
		fclose(fp);

		node = mxmlNewElement(group, DPROXY_ENTRY);
		for(i = 0; strlen(dproxy_attribute[i]) != 0; i++){
			mxmlElementSetAttr(node, dproxy_attribute[i], dns_info[i]);
		}
	}
	return 0;
}/* end dproxy_read */
#endif

#ifndef TCSUPPORT_DNSEACHPVC
//"Dproxy",	SINGLE_TYPE,	NULL,	NULL,NULL, dproxy_write, dproxy_verify, dproxy_execute, dproxy_boot},
/*______________________________________________________________________________
**	dproxy_boot
**
**	descriptions:
**		the boot function of dproxy.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
dproxy_boot(mxml_node_t *top){

	system("/userfs/bin/dnsmasq");
	
	return SUCCESS;
}

/*______________________________________________________________________________
**	dproxy_init
**
**	descriptions:
**		the init function of dproxy.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
dproxy_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="Dproxy",
			.type=SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=dproxy_write,
			.cfg_verify=dproxy_verify,
			.cfg_execute=dproxy_execute,
			.cfg_boot=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}
/*______________________________________________________________________________
**	dproxy_write
**
**	descriptions:
**		Find dproxy element from mxml tree then write dproxy's system parameter to specify
**		configuration files.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  dproxy cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
dproxy_write(mxml_node_t *top, mxml_node_t *parant){

	char dproxy_info[3][ATTR_SIZE];
	/*krammer add for bug 1318*/
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char default_route[5]={0};
	char isp[4]={0};
	char buf[4]={0};
	char tmpBuf[128]={0}, tmpBuf1[128]={0};  /*rodney_20090506 modified*/
	char dnsInfo[2][16];
//	char *pDproxy=NULL;
	FILE *fp=NULL;
	struct stat fbuf;  /*rodney_20090506 added*/
	int i=0, default_route_isp=WAN_ENCAP_NONE;  /*rodney_20090506 modified*/

	//mxml_attr_t   *attr= NULL;         /* Cirrent attribute */
	mxml_node_t *node = NULL;
    memset(dproxy_info ,0 ,sizeof(dproxy_info));
	memset(dnsInfo ,0 ,sizeof(dnsInfo));
	memset(nodeName, 0, sizeof(nodeName));
	char dproxy_attrName[][MAXLEN_ATTR_NAME]={
			{"Primary_DNS"},
			{"Secondary_DNS"},
			{""},
	};
	char dproxy_keyword[][ATTR_SIZE]=
	{
		{"nameserver ="},
		{"nameserver ="},
		{""},
	};
	/*
	node = mxmlFindElement(tree, tree, DPROXY_ENTRY,
	NULL, NULL,
	MXML_DESCEND);
	*/
	/*rodney_20090506 added*/
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],WAN);
	/*find default route*/
	for(i = 0 ; i < PVC_NUM; i++){
		sprintf(nodeName[1],SUB_PVC_NAME,i);
		if(getAttrValue(top,nodeName, "DEFAULTROUTE", default_route)!= TCAPI_PROCESS_OK){
			continue;
		}
		if(!strcmp(default_route, ACTIVE)){
			break;
		}
	}
	/*find the isp of default route*/
	if(getAttrValue(top,nodeName, "ISP", isp) == TCAPI_PROCESS_OK){
		default_route_isp = atoi(isp);
	}

	node = mxmlFindElement(parant, parant, SUB_NODE_NAME,
		NULL, NULL, MXML_DESCEND);

	if(node == NULL){
		unlink(DPROXYAUTO_PATH);
		/*remove information*/
		updateString(DPROXY_PATH, (char *)dproxy_keyword, (char *)dproxy_info+SHIFT_INDEX);
		if((default_route_isp == WAN_ENCAP_PPP) && (stat(PPP_DNS_INFO_PATH,&fbuf) == 0)){  /*rodney_20090506 added*/
			sprintf(tmpBuf1,"cp %s %s",PPP_DNS_INFO_PATH,"/etc/");  /*even isp is ppp, the real dns for system use is /etc/resolv.conf, so keep /etc/resolv.conf to be updated*/
			system(tmpBuf1);
		}
		return FAIL;
	}
/*krammer change for bug 1318*/
	strcpy(nodeName[0],LAN);
	strcpy(nodeName[1],LAN_DHCP);
	if(getAttrValue(top, nodeName, LAN_DHCP_TYPE, buf) != TCAPI_PROCESS_OK){
		return FAIL;
	}
	if(strcmp(buf, "1")){/*dhcp is not on*/
		return SUCCESS;
	}

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],DPROXY);
	strcpy(nodeName[1],SUB_NODE_NAME);
	memset(buf, 0, sizeof(buf));
	if(getAttrValue(top, nodeName, LAN_DHCP_TYPE, buf) != TCAPI_PROCESS_OK){
		return FAIL;
	}
	strcpy(dproxy_info[0], buf);
	if(strlen(dproxy_info[0]) == 0){
		return FAIL;
	}

	if(atoi(dproxy_info[0]) == 1){  /*DNS Relay Manually*/
		/*update resolv.conf*/ /*rodney_20090506 added*/
		fp = fopen(DNS_INFO_PATH, "w");
		/*for DNS Relay Manually, we need to update resolv.conf by manually*/
		if(fp != NULL){
			memset(nodeName, 0, sizeof(nodeName));
			strcpy(nodeName[0],DPROXY);
			strcpy(nodeName[1],SUB_NODE_NAME);
			getAttrValue(top, nodeName, dproxy_attrName[0], tmpBuf);
			sprintf(tmpBuf1,"nameserver %s\n",tmpBuf);
			fputs(tmpBuf1, fp);
			getAttrValue(top, nodeName, dproxy_attrName[1], tmpBuf);
			sprintf(tmpBuf1,"nameserver %s\n",tmpBuf);
			fputs(tmpBuf1, fp);
			fclose(fp);
		}
	}
	else{  /*DNS Relay Automatically*/
		/*for ppp, it will only generate resolv.conf at /etc/ppp but the default DNS server for system is at /etc.*/
		/*So in order to make sure system could look up the correct DNS server, copy /etc/ppp/resolv.conf to /etc.*/
		if((default_route_isp == WAN_ENCAP_PPP) && (stat(PPP_DNS_INFO_PATH,&fbuf) == 0)){  /*rodney_20090506 added*/
			sprintf(tmpBuf1,"cp %s %s",PPP_DNS_INFO_PATH,"/etc/");
			system(tmpBuf1);
		}
	}
	/*delete and new a node*/
	mxmlDelete(node);
	node=mxmlNewElement(parant,  SUB_NODE_NAME);
	if(!node){
		return FAIL;
	}
	mxmlElementSetAttr(node, "Active", "Yes");  /*rodney_20090506 added*/  /*Dproxy always enable at this moment*/
	mxmlElementSetAttr(node, "type", dproxy_info[0]);
	/*open the resolv.conf and put the dns server into our node*/
	fp = fopen(DNS_INFO_PATH, "r");
	if(fp){
		i=0;
		while(fgets(tmpBuf, MAXGET_PROFILE_SIZE, fp) != NULL){
			if(strstr(tmpBuf, DNS_ATTR) &&
				strlen(dproxy_attrName[i])>0 &&
				strlen(dproxy_keyword[i])>0){
				sscanf(tmpBuf,"%s %s",dnsInfo[0],dnsInfo[1]);
				mxmlElementSetAttr(node, dproxy_attrName[i], dnsInfo[1]);
				sprintf(dproxy_info[i+1],"%s  %s\n", dproxy_keyword[i], dnsInfo[1]);
			}
			/*fix bug. If number of nameserver is more then 4, cfg_manager will set wrong dproxy information. 
				shnwind modify 20091206.*/
			else{
				break;	
			}
			i++;
		}
		fclose(fp);
	}

	/*}*/ /*rodney_20090506 marked*/
	#if 0
	attr = node->value.element.attrs;
	for (i = 0; i < node->value.element.num_attrs; i++){
		/*
			dproxy_info[0] = Auto(0) or Manually(1)
			dproxy_info[1] = 1st_DNS(168.95.1.1)
			dproxy_info[2] = 2nd_DNS(168.95.1.2)
		*/
		if(i == DPROXY_TYPE_INDEX){
			strcpy(dproxy_info[i], attr->value);
		}
		else{
			sprintf(dproxy_info[i],"%s  %s\n", dproxy_keyword[i-1], attr->value);
		}
		attr++;
	}
	#endif
/*krammer change for bug 1318*/
#if 0
	/*Create dproxy.auto file*/
	if(write2file(dproxy_info[0], DPROXYAUTO_PATH) ==FAIL){
		return FAIL;
	}

	pDproxy=(char *)dproxy_info;
	/*Update dproxy.conf file*/
	updateString(DPROXY_PATH, (char *)dproxy_keyword, pDproxy+SHIFT_INDEX);
#endif
	return SUCCESS;
}/* end dproxy_write */

/*______________________________________________________________________________
**	dproxy_verify
**
**	descriptions:
**		To check the system parameter of dproxy element  is validly or not.
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
dproxy_verify(mxml_node_t *node){
	return 0;
}/* end dproxy_verify */

/*_____________________________________________________________________________
**      function name: dproxy_execute
**      descriptions:
**            To execute drpoxy node service.
**
**      parameters:
**            node:   Put the dproxy of cfg node tree that is used to execute dproxy settings.
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
**      1. shnwind 2008/5/19
**____________________________________________________________________________
*/
int
dproxy_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){

	char buf[64], nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
#if 0
	system("killall -9 dproxy");
#endif
	system("killall -9 dnsmasq");
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],LAN);
	strcpy(nodeName[1],LAN_DHCP);

	if(getAttrValue(top, nodeName, LAN_DHCP_TYPE, buf) != TCAPI_PROCESS_OK){
		return FAIL;
	}
	if(!strcmp(buf,"1")){/*dproxy and dhcpd*/
		kill_process("/var/log/dhcrelay.pid");
		unlink(DHCPRELAY_PATH);
#if 0
		system("/userfs/bin/dproxy -c /etc/dproxy.conf");
#endif
#if 0//fix bug#5775
		system("/userfs/bin/dnsmasq");
#endif
	}

	//fix bug#5775
	//dnsmasq is not related by dhcp enable or disable
	system("/userfs/bin/dnsmasq");
	
	return SUCCESS;
}/* end dproxy_execute */
#endif
#if 0  /*Orginal ci-command mode */
/*______________________________________________________________________________
**	dhcpd_read
**
**	descriptions:
**		Read dhcpd system parameter from configuration file and record those information
**		to mxml_node_t.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
dhcpd_read(mxml_node_t *top){

 	char tmp[MAXSIZE];
	FILE *fp = NULL;
	mxml_node_t *group= NULL;  /* <Wan> */
	mxml_node_t *node= NULL;  /* <node> */
	char dhcp_info[3][32];

	group = mxmlNewElement(top, DHCPD);
	/*
	udhcpd.conf content as flow
	start 10.10.10.2
	end 10.10.10.21
	interface br0
	lease_file      /etc/udhcpd.leases
	opt subnet 255.255.255.0
	opt router 10.10.10.1
	opt dns 10.10.10.1
	opt wins 10.10.10.1
	option lease 259200
	*/
	fp = fopen(DHCPD_PATH, "r");
	if(fp == NULL){
		return FAIL;
	}
	else{
		node = mxmlNewElement(group, DHCPD_ENTRY);

		while(fgets(tmp, MAXSIZE, fp)){
			memset(dhcp_info, 0,sizeof(dhcp_info));
			sscanf(tmp,"%s %s %s",dhcp_info[0],dhcp_info[1],dhcp_info[2]);

			if((strcmp(dhcp_info[DHCPD_ATTR_INDEX], dhcpd_default_parm[DHCPD_ATTR_IF_INDEX][DHCPD_ATTR_INDEX]) == 0)
			||(strcmp(dhcp_info[DHCPD_ATTR_INDEX], dhcpd_default_parm[DHCPD_ATTR_LEASE_INDEX][DHCPD_ATTR_INDEX])==0)){
				continue;
			}

			if(strlen(dhcp_info[DHCPD_VALUE_INDEX+1]) == 0){
				mxmlElementSetAttr(node, dhcp_info[DHCPD_ATTR_INDEX], dhcp_info[DHCPD_VALUE_INDEX]);
			}
			else{
				mxmlElementSetAttr(node, dhcp_info[DHCPD_ATTR_INDEX+1], dhcp_info[DHCPD_VALUE_INDEX+1]);
			}
		}
	}
	fclose(fp);
	return 0;
}/* end dhcpd_read */
#endif

//"Dhcpd",	SINGLE_TYPE,	NULL, 	NULL,NULL, dhcpd_write, dhcpd_verify, dhcpd_execute},
#ifdef STATIC_DHCP
char *
dhcpd_adv_table[MAX_NODE_NAME]=
{
	"Common",
#ifdef DHCP_PROFILE
	"Option60","Option240","Option125",
#endif
	"",
};

char *
dhcpd_merge_table[MAX_NODE_NAME]=
{
	"dhcpd_id","",
};
#endif
/*______________________________________________________________________________
**	dhcpd_boot
**
**	descriptions:
**		the boot function of dhcpd.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
dhcpd_boot(mxml_node_t *top){
	FILE *startupSh=NULL;
	startupSh=fopen("/etc/udhcpd.conf","r");
	if(startupSh){
		fclose(startupSh);
		system("killall -9 udhcpd");
		system("/usr/sbin/udhcpd &");
		startupSh=fopen("/etc/dproxy.conf","r");
		if(startupSh){
			fclose(startupSh);
			system("/userfs/bin/dproxy  -c /etc/dproxy.conf &");
		}
	}
	return SUCCESS;
}
/*______________________________________________________________________________
**	dhcpd_init
**
**	descriptions:
**		the init function of dhcpd.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
dhcpd_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="Dhcpd",
			#ifdef STATIC_DHCP
			.type=ADV_FLAG|ENTRY_TYPE|MAX_STATIC_NUM|UPDATE_TYPE,
			.adv_table=dhcpd_adv_table,
			.merge_table=dhcpd_merge_table,
			.cfg_read=dhcpd_read,
			#else
			.type=SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			#endif
			.cfg_write=dhcpd_write,
			.cfg_verify=dhcpd_verify,
			.cfg_execute=dhcpd_execute,
			.cfg_boot=dhcpd_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}
/*______________________________________________________________________________
**function name:dhcpd_read
**
**description:
*    reload dhcp static lease
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
*     1.liujilin 20100107
**____________________________________________________________________________*/
#ifdef STATIC_DHCP
int
dhcpd_read(mxml_node_t *top, char nodeName[][MAX_NODE_NAME], char *attr){

	char subNode[16]={0}, count[3]={0};	
	int i;
	mxml_node_t *group = NULL;  /* <Dhcpd Node> */
	mxml_node_t *node = NULL;   /* <element node> */
	mxml_node_t *entry_node = NULL;	
	int total = 0,empty_entry = -1;

	group = mxmlFindElement(top, top, DHCPD,
				NULL, NULL, MXML_DESCEND);
	if(group == NULL){
		return FAIL;
	}
	
	/*Delete the Entry node if Common node is not existed when attr is IP*/
	if(!strcmp(attr,"IP")){		
		entry_node = mxmlFindElement(group, group, nodeName[1], NULL , NULL, MXML_DESCEND);
		if(entry_node == NULL){
			/*Entry is not exited,need not to check*/
			return SUCCESS;
		}		
		
		node = mxmlFindElement(group, group, "Common", NULL , NULL, MXML_DESCEND);
		if(!node){
			mxmlDelete(entry_node);
			return SUCCESS;		
		}					
	}
	
	
	if(!strcmp(attr,"Empty_Entry") || !strcmp(attr,"Static_Num")){
		for(i=0; i<MAX_STATIC_NUM; i++){
			/*set query data, indicate entry*/		
			sprintf(subNode,"%s%d", SUB_NODE_NAME, i);		
			node = mxmlFindElement(group, group, subNode, NULL , NULL, MXML_DESCEND);		
			if(node != NULL){				
				total++;				
			}
			else{
				if(empty_entry < 0)
					empty_entry = i;
			}			
		}
		
	}

	if(empty_entry < 0){     /*static pool is full*/
		mxmlElementSetAttr(group, "Empty_Entry", "N/A");
	}
	else{
		sprintf(count,"%d",empty_entry);
		mxmlElementSetAttr(group, "Empty_Entry", count);
	}
	sprintf(count,"%d",total);
	mxmlElementSetAttr(group, "Static_Num", count);
	
	
	return SUCCESS;
}
#endif

/*______________________________________________________________________________
**	function name:write_dhcpd_config
**
**	description:
*     create dhcpd configure file
**	parameters:
*     top:xml top node address
**	global:
*     none
**	return:
*       0:success
*	  1:fail
**	call:
*     none
**	revision:
*     1. 
**____________________________________________________________________________*/
int
write_dhcpd_config(mxml_node_t *top, mxml_node_t *parant, char *configFile){
	FILE *fp = NULL;
	char buf[128]={0}, tmpBuf[512]={0}, dnsInfo[4][16];  /*rodney_20090506 modified*/
	int	i=0;          /* Looping var */
	/*mxml_attr_t   *attr= NULL;*/          /* Cirrent attribute */
	mxml_node_t *node = NULL;
	char lanInfo[2][16];/*0: netmask address,1: Lan IP address, */
	char start[16], poll_cnt[4], lease_time[11], end_ip[16], router[16];
	char domain[64];
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];  /*rodney_20090506 added*/
	const char *tmp=NULL;
#ifdef ALIAS_IP
	short main_or_alias = 0;   /*1: dhcp use main lan,2: dhcp user alias lan, */
	char lanAliasInfo[2][16];/*0: netmask address,1: Alias Lan IP address, */
#endif
#ifdef STATIC_DHCP
	char element_name[16] = {0}, static_mac[19], static_ip[16], count[3]={0};
	struct in_addr startIp,staticIp,mask;
#ifdef ALIAS_IP
	struct in_addr aliasMask;
#endif
	int isValid = 0;	
#endif
	char dhcpInfo[4] = {0};

#ifdef DHCP_PROFILE
	char active[8]={0};
	char vendorID[MAX_VENDOR_CLASS_ID_LEN]={0};
	char conPool_start[16] = {0};
	char conPool_cnt[4] = {0};
	char conPool_lease_time[11] = {0};
	char conPool_end_ip[16] = {0};
	struct in_addr conPool_end;
	char conPoolRouter[16] = {0};
	char conPoolSubnetMask[16] = {0};
	char conPoolDomain[64] = {0};
#endif
	struct in_addr end;
	/*char dhcpd_attribute[][2][ATTR_SIZE]=
	{
		{"start",	"start"},
		{"max_leases",	"pool_count"},
		{"option lease","lease"},
		{"",""},
	};*/
	char dhcpd_sys_parm[][ATTR_SIZE]=
	{
		{"opt subnet"},
		{"opt router"},
		{"opt dns"},
		{"opt dns"},  /*rodney_20090506 added*/
		{"opt wins"},
		{"opt domain"},
		{""},
	};

	char dhcpd_default_parm[][2][ATTR_SIZE]=
	{
		{"interface", LAN_IF},
		{"lease_file", "/etc/udhcpd.leases"},
		{"",""},
	};

	memset(lanInfo,0, sizeof(lanInfo));
	memset(dnsInfo,0, sizeof(dnsInfo));  /*rodney_20090506 added*/

	
#ifdef ALIAS_IP
//check whether dhcp is using alias ip
	node = mxmlFindElement(top, top, LAN_ALIAS,
		NULL, NULL,MXML_DESCEND);
	if(node){
		sprintf(buf,"%s%d",SUB_NODE_NAME,i);/*Buf: Entry0*/
		node = mxmlFindElement(node, node, buf,
			NULL, NULL,MXML_DESCEND);
		if(node){
			tmp=mxmlElementGetAttr(node, NODE_ACTIVE);
				if(!strcmp(tmp,ACTIVE)){
				tmp=mxmlElementGetAttr(node, LAN_MASK);
				if(tmp!=NULL){
					strcpy(lanAliasInfo[0], tmp);
				}
				tmp=mxmlElementGetAttr(node, LAN_IP);
				if(tmp!=NULL){
					strcpy(lanAliasInfo[1], tmp);
				}
			}
		}
	}	
	node = NULL;
#endif
	node = mxmlFindElement(top, top, LAN,
		NULL, NULL,MXML_DESCEND);
	if(node){
		sprintf(buf,"%s%d",SUB_NODE_NAME,i);/*Buf: Entry0*/
		node = mxmlFindElement(node, node, buf,
			NULL, NULL,MXML_DESCEND);
		if(node){
			tmp=mxmlElementGetAttr(node, LAN_MASK);
			if(tmp!=NULL){
			#if 0
			strcpy(lanInfo[0], mxmlElementGetAttr(node, LAN_MASK));
			#endif
				strcpy(lanInfo[0], tmp);
			}
			tmp=mxmlElementGetAttr(node, LAN_IP);
			if(tmp!=NULL){
			#if 0
			strcpy(lanInfo[1], mxmlElementGetAttr(node, LAN_IP));
			#endif
				strcpy(lanInfo[1], tmp);
			}
		}
	}

	node = NULL;
	node = mxmlFindElement(top, top, LAN,
		NULL, NULL,MXML_DESCEND);
	if(node)
	{
		node = mxmlFindElement(node, node, LAN_DHCP,
			NULL, NULL,MXML_DESCEND);
		if(node)
		{
			tmp=mxmlElementGetAttr(node, LAN_DHCP_TYPE);
			if(tmp != NULL)
			{
				strcpy(dhcpInfo, tmp);
			}
		}
	}

#ifdef DHCP_PROFILE
	if(strcmp(configFile, DHCPD_OPTION_PATH) == 0) {

		node = NULL;
		node = mxmlFindElement(parant, parant, SUB_NODE_OPT60_NAME,
			NULL, NULL, MXML_DESCEND);

		if(node != NULL){
			tmp=mxmlElementGetAttr(node, "start");
			if(tmp!=NULL){
				strcpy(conPool_start, tmp);
			}
			
			/*use LAN SubnetMask and IP as conditional pool subnetMask and router */
			strcpy(conPoolSubnetMask, lanInfo[0]);
			strcpy(conPoolRouter, lanInfo[1]);

#ifdef ALIAS_IP	
			/*check whether conditional dhcp is using alias ip*/	
			dhcp_main_alias(lanInfo[0],lanInfo[1], lanAliasInfo[0], lanAliasInfo[1], conPool_start, &main_or_alias);
			if(0 == main_or_alias){
				//tcdbg_printf("\n Conditional pool start ip is neith the same with main ip or alias ip. \n");
			}
			else if(2 == main_or_alias){//use Alias LAN IP and SubnetMask
				strcpy(conPoolSubnetMask, lanAliasInfo[0]);
				strcpy(conPoolRouter, lanAliasInfo[1]);

				main_or_alias = 0;
			}
#endif

			/*get conditional pool domain name*/
			tmp=mxmlElementGetAttr(node, "domainName");
			if(tmp != NULL) {
				strcpy(conPoolDomain, tmp);
			}
		}
		else
			return FAIL;
	}
#endif

	node=NULL;
	/*
	node = mxmlFindElement(tree, tree, DHCPD_ENTRY,
		NULL, NULL,
		MXML_DESCEND);
	*/
	#ifdef STATIC_DHCP
	sprintf(count,"%d",MAX_STATIC_NUM);
	mxmlElementSetAttr(parant, "MaxStaticNum", count);		
	node = mxmlFindElement(parant, parant, "Common",
		NULL, NULL, MXML_DESCEND);
	#else
	node = mxmlFindElement(parant, parant, SUB_NODE_NAME,
		NULL, NULL, MXML_DESCEND);
	#endif
#if 0
	if(node != NULL){
#else
	if(0 != strcmp(dhcpInfo,"0")){//0 means disable
#endif
	#ifdef ALIAS_IP
		//check dhcp use main lan or alias lan
		tmp=mxmlElementGetAttr(node, "start");
		if(tmp!=NULL){
			strcpy(start, tmp);
		}
		//lanAliasInfo[1]: alias ip
		//lanInfo[1]: main ip
		dhcp_main_alias(lanInfo[0],lanInfo[1], lanAliasInfo[0], lanAliasInfo[1], start, &main_or_alias);
		//tcdbg_printf("\n main_or_alias value is %d. \n", main_or_alias);
		if(0 == main_or_alias){
			tcdbg_printf("\n dhcp start ip is neith the same with main ip or alias ip. \n");
			return FAIL;
		}
		else if(2 == main_or_alias){
			strcpy(lanInfo[0], lanAliasInfo[0]);
			strcpy(lanInfo[1], lanAliasInfo[1]);
		}
	#endif

#if defined(TCSUPPORT_CT_E8GUI)
		int j = 0;
		for(j=0; j<5; j++){
			if(0 == j)
				fp = fopen(DHCPD_OPTION_STB_PATH,"w");
			else if(1 == j)
				fp = fopen(DHCPD_OPTION_PHONE_PATH,"w");
			else if(2 == j)
				fp = fopen(DHCPD_OPTION_CAMERA_PATH,"w");
			else if(3 == j)
				fp = fopen(DHCPD_OPTION_HGW_PATH,"w");
			else
				fp = fopen(configFile,"w");
#else
		fp = fopen(configFile,"w");
#endif

		if(fp == NULL){
			return FAIL;
		}
		else{
			/*Write default parameters*/
			for(i = 0; strlen(dhcpd_default_parm[i][DHCPD_ATTR_INDEX]) != 0; i++){
				sprintf(buf,DHCPD_PUT_INS, dhcpd_default_parm[i][DHCPD_ATTR_INDEX], dhcpd_default_parm[i][DHCPD_VALUE_INDEX]);
				fputs(buf,fp);
			}

			/*Write system parameters*/
			for(i = 0; strlen(dhcpd_sys_parm[i]) != 0; i++){
				if(i==0){
#ifdef DHCP_PROFILE
					if(strcmp(configFile, DHCPD_OPTION_PATH) == 0) {
						sprintf(buf,DHCPD_PUT_INS, dhcpd_sys_parm[i], conPoolSubnetMask);
					}
					else
#endif
					sprintf(buf,DHCPD_PUT_INS, dhcpd_sys_parm[i], lanInfo[0]);
				}
				else if(i==2){  /*DNS1*/  /*rodney_20090506 added*/
					memset(nodeName,0,sizeof(nodeName));
					strcpy(nodeName[0],DPROXY);
					strcpy(nodeName[1],SUB_NODE_NAME);
					if(getAttrValue(top, nodeName, NODE_ACTIVE, buf) != TCAPI_PROCESS_OK){
						return FAIL;
					}
					if(!strcmp(buf,"Yes")){  /*If Dproxy enable, use Lan IP for DHCP DNS*/
						sprintf(buf,DHCPD_PUT_INS, dhcpd_sys_parm[i], lanInfo[1]);
					}
					else{  /*If Dproxy disable, use /etc/resolv.conf info for DHCP DNS*/
						fileRead(DNS_INFO_PATH, tmpBuf, sizeof(tmpBuf));
						if(strstr(tmpBuf, DNS_ATTR)){
							sscanf(strstr(tmpBuf, DNS_ATTR),"%s %s %s %s",dnsInfo[0],dnsInfo[1],dnsInfo[2],dnsInfo[3]);
							sprintf(buf,DHCPD_PUT_INS, dhcpd_sys_parm[i], dnsInfo[1]);
						}
					}
				}
				else if(i==3){  /*DNS2*/  /*rodney_20090506 added*/
					if(strcmp(dnsInfo[2],DNS_ATTR) == 0){
						sprintf(buf,DHCPD_PUT_INS, dhcpd_sys_parm[i], dnsInfo[3]);
					}
					else{
						continue;
					}
				}
				else if(i == 1){ /*router*/
					tmp=mxmlElementGetAttr(node, "router");
					strcpy(router, lanInfo[1]);
					mxmlElementSetAttr(node, "router", router);
#ifdef DHCP_PROFILE
					if(strcmp(configFile, DHCPD_OPTION_PATH) == 0) {
						sprintf(buf,DHCPD_PUT_INS, dhcpd_sys_parm[i], conPoolRouter);
					}
					else
#endif
					sprintf(buf,DHCPD_PUT_INS, dhcpd_sys_parm[i], router);
				}
				else if(i == 5){ /*domain name*/
#ifdef DHCP_PROFILE
					if((strcmp(configFile, DHCPD_OPTION_PATH) == 0) && strlen(conPoolDomain) != 0) {//if conditional pool domain name is not null, use it
						sprintf(buf,DHCPD_PUT_INS, dhcpd_sys_parm[i], conPoolDomain);
					}
					else {//if conditional pool domain name is null, use main pool domain
#endif
					tmp=mxmlElementGetAttr(node, "DomainName");
					if(tmp!=NULL){
						strcpy(domain, tmp);
						sprintf(buf,DHCPD_PUT_INS, dhcpd_sys_parm[i], domain);
					}
#ifdef DHCP_PROFILE
					}
#endif
				}
				else{
#ifdef DHCP_PROFILE
					if(strcmp(configFile, DHCPD_OPTION_PATH) == 0) {
						sprintf(buf,DHCPD_PUT_INS, dhcpd_sys_parm[i], conPoolRouter);
					}
					else
#endif
					sprintf(buf,DHCPD_PUT_INS, dhcpd_sys_parm[i], lanInfo[1]);
				}
				fputs(buf,fp);
			}

			/*Write user setting parameters*/
			/*attr = node->value.element.attrs;
			for (i = 0; i < node->value.element.num_attrs; i++){
				sprintf(buf,"%s %s\n", dhcpd_attribute[i][DHCPD_ATTR_INDEX] , attr->value);
				fputs(buf, fp);
				attr ++;
			}*/
			/*If udhcpd.conf does not have end IP, the udhcpd may can not distribute IP in some IP section
			    like 192.168.1.1 ~ 192.168.1.10. shnwind  */
			/*get user setting parameters*/

#ifdef DHCP_PROFILE
			if(strcmp(configFile, DHCPD_OPTION_PATH) == 0) {

				/*get option60 parameters*/
				node = mxmlFindElement(parant, parant, SUB_NODE_OPT60_NAME,
					NULL, NULL, MXML_DESCEND);

				if(node != NULL){
					/*set conditional pool subnetmask and router*/
					mxmlElementSetAttr(node, "subnetMask", conPoolSubnetMask);
					mxmlElementSetAttr(node, "router", conPoolRouter);

					tmp=mxmlElementGetAttr(node, "Active");
					if(tmp!=NULL){
						strcpy(active, tmp);
					}

#if defined(TCSUPPORT_CT_E8GUI)
					if(0 == j) tmp=mxmlElementGetAttr(node, "VendorIDSTB");
					else if(1 == j) tmp=mxmlElementGetAttr(node, "VendorIDPhone");
					else if(2 == j) tmp=mxmlElementGetAttr(node, "VendorIDCamera");
					else if(3 == j) tmp=mxmlElementGetAttr(node, "VendorIDHGW");
					else tmp=mxmlElementGetAttr(node, "VendorID");
#else
					tmp=mxmlElementGetAttr(node, "VendorID");
#endif
					if(tmp!=NULL){
						strcpy(vendorID, tmp);
					}

					/*Write option60 parameters*/
					sprintf(buf,DHCPD_PUT_INS, "enable" , active);
					fputs(buf, fp);
					sprintf(buf,DHCPD_PUT_INS, "vendorClassID" , vendorID);
					fputs(buf, fp);

#if defined(TCSUPPORT_CT_E8GUI)
					if(0 == j) tmp=mxmlElementGetAttr(node, "startSTB");
					else if(1 == j) tmp=mxmlElementGetAttr(node, "startPhone");
					else if(2 == j) tmp=mxmlElementGetAttr(node, "startCamera");
					else if(3 == j) tmp=mxmlElementGetAttr(node, "startHGW");
					else tmp=mxmlElementGetAttr(node, "start");
#else
					tmp=mxmlElementGetAttr(node, "start");
#endif
					if(tmp!=NULL){
						strcpy(conPool_start, tmp);
					}
#if defined(TCSUPPORT_CT_E8GUI)
					if(0 == j) tmp=mxmlElementGetAttr(node, "pool_countSTB");
					else if(1 == j) tmp=mxmlElementGetAttr(node, "pool_countPhone");
					else if(2 == j) tmp=mxmlElementGetAttr(node, "pool_countCamera");
					else if(3 == j) tmp=mxmlElementGetAttr(node, "pool_countHGW");
					else tmp=mxmlElementGetAttr(node, "pool_count");
#else
					tmp=mxmlElementGetAttr(node, "pool_count");
#endif
					if(tmp!=NULL){
						strcpy(conPool_cnt, tmp);
					}
					tmp=mxmlElementGetAttr(node, "lease");
					if(tmp!=NULL){
						strcpy(conPool_lease_time, tmp);
					}
					 /*calculate end IP*/
					 if(strlen(conPool_start) != 0 && strlen(conPool_cnt) != 0) {
					 	inet_aton(conPool_start, &conPool_end);
				 		//conPool_end.s_addr += atoi(conPool_cnt);
				 		conPool_end.s_addr += atoi(conPool_cnt)-1;	              //suxia.wang
						strcpy(conPool_end_ip, inet_ntoa(conPool_end));
					 }
					/*Write user setting parameters*/
					sprintf(buf,DHCPD_PUT_INS, "start" , conPool_start);
					fputs(buf, fp);
					sprintf(buf,DHCPD_PUT_INS, "end" , conPool_end_ip);
					fputs(buf, fp);
					sprintf(buf,DHCPD_PUT_INS, "max_leases" , conPool_cnt);
					fputs(buf, fp);
					sprintf(buf,DHCPD_PUT_INS, "option lease" , conPool_lease_time);
					fputs(buf, fp);
				}

				node = NULL;
				/*get option240 parameters*/
				node = mxmlFindElement(parant, parant, SUB_NODE_OPT240_NAME,
					NULL, NULL, MXML_DESCEND);

				if(node != NULL){
					tmp=mxmlElementGetAttr(node, "Active");
					if(tmp!=NULL){
						strcpy(active, tmp);
					}

					tmp=mxmlElementGetAttr(node, "Value");
					if(tmp!=NULL){
						strcpy(vendorID, tmp);
					}

					/*Write option240 parameters*/
					sprintf(buf,DHCPD_PUT_INS, "opt240Enable" , active);
					fputs(buf, fp);
					sprintf(buf,DHCPD_PUT_INS, "opt240Value" , vendorID);
					fputs(buf, fp);
				}
				
			}
			else {
			
#endif
			tmp=mxmlElementGetAttr(node, "start");
			if(tmp!=NULL){
				#if 0
			strcpy(start, mxmlElementGetAttr(node, "start"));
				#endif
				strcpy(start, tmp);
			}
			tmp=mxmlElementGetAttr(node, "pool_count");
			if(tmp!=NULL){
				#if 0
			strcpy(poll_cnt, mxmlElementGetAttr(node, "pool_count"));
				#endif
				strcpy(poll_cnt, tmp);
			}
			tmp=mxmlElementGetAttr(node, "lease");
			if(tmp!=NULL){
				#if 0
			strcpy(lease_time, mxmlElementGetAttr(node, "lease"));
				#endif
				strcpy(lease_time, tmp);
			}

			 /*calculate end IP*/
			 inet_aton(start, &end);
			 //end.s_addr += atoi(poll_cnt);
			 end.s_addr += atoi(poll_cnt) -1; //suxia.wang
			strcpy(end_ip, inet_ntoa(end));
			/*Write user setting parameters*/
			sprintf(buf,DHCPD_PUT_INS, "start" , start);
			fputs(buf, fp);
			sprintf(buf,DHCPD_PUT_INS, "end" , end_ip);
			fputs(buf, fp);
			sprintf(buf,DHCPD_PUT_INS, "max_leases" , poll_cnt);
			fputs(buf, fp);
			sprintf(buf,DHCPD_PUT_INS, "option lease" , lease_time);
			fputs(buf, fp);
			/*User set Arp Wait Time. shnwind 20100112*/
			tmp=mxmlElementGetAttr(node, "ArpWaitTime");
			if(tmp!=NULL){
				remove("/etc/dhcpdarpwaittime");
				sprintf(tmpBuf, "echo %s >/etc/dhcpdarpwaittime", tmp);
			//	tcdbg_printf("buf : %s",tmpBuf);
				system(tmpBuf);
			}
			
#ifdef DHCP_PROFILE
			}
#endif			
			#ifdef STATIC_DHCP
			/*Write static lease DHCP*/
			for(i=0; i<MAX_STATIC_NUM; i++){
				/*set query data, indicate entry*/		
				sprintf(element_name,"%s%d", SUB_NODE_NAME, i);		
				node = mxmlFindElement(parant, parant, element_name, NULL , NULL, MXML_DESCEND);
				if(node == NULL){
					/*fprintf(stderr, "Error--Entry value is NULL.\n");*/
					continue;
				}
				
				tmp = mxmlElementGetAttr(node, "IP");
				strcpy(static_ip, tmp);
				
				inet_aton(start, &startIp);
				inet_aton(lanInfo[0], &mask);	
				inet_aton(static_ip, &staticIp);
				/*Check the static lease ip whether valid,or delete it*/
				if(((startIp.s_addr & mask.s_addr) == (staticIp.s_addr & mask.s_addr)) &&
					(staticIp.s_addr >= startIp.s_addr) && 
					(staticIp.s_addr <  (startIp.s_addr + atoi(poll_cnt)))){
					isValid = 1;			
					//tcdbg_printf("\n not same subnet,DELET entry_node\n");
				}
				#ifdef ALIAS_IP
				inet_aton(lanAliasInfo[0], &aliasMask);
				if(((startIp.s_addr & aliasMask.s_addr) == (staticIp.s_addr & aliasMask.s_addr)) &&
					(staticIp.s_addr >= startIp.s_addr) && 
					(staticIp.s_addr <  (startIp.s_addr + atoi(poll_cnt)))){
					isValid = 1;					
					//tcdbg_printf("\n not same subnet,DELET entry_node\n");
				}
				#endif
				/*Not valid ,delete it*/
				if(!isValid){
					mxmlDelete(node);
					continue;
				}							
				
				tmp = mxmlElementGetAttr(node, "MAC");
				strcpy(static_mac, tmp);
				
				sprintf(buf,"%s %s %s\n", "static_lease" , static_mac,static_ip);
				fputs(buf, fp);
			}
			#endif

			fclose(fp);
		}
#if defined(TCSUPPORT_CT_E8GUI)
		}
#endif
	}else{
      		unlink(configFile);
#if defined(TCSUPPORT_CT_E8GUI)
		unlink(DHCPD_OPTION_STB_PATH);
		unlink(DHCPD_OPTION_PHONE_PATH);
		unlink(DHCPD_OPTION_CAMERA_PATH);
		unlink(DHCPD_OPTION_HGW_PATH);
#endif
	}
	return SUCCESS;
}
/*______________________________________________________________________________
**	dhcpd_write
**
**	descriptions:
**		Find dhcpd element from mxml tree then write dhcpd's system parameter to specify
**		configuration files.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  dhcpd cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
dhcpd_write(mxml_node_t *top, mxml_node_t *parant){

	write_dhcpd_config(top, parant, DHCPD_PATH);
	
#ifdef DHCP_PROFILE
	write_dhcpd_config(top, parant, DHCPD_OPTION_PATH);
#endif	

	return SUCCESS;
}/* end dhcpd_write */

/*______________________________________________________________________________
**	dhcpd_verify
**
**	descriptions:
**		To check the system parameter of dhcpd element  is validly or not.
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
dhcpd_verify(mxml_node_t *node){
	return SUCCESS;
}/* end dhcpd_verify */

/*_____________________________________________________________________________
**      function name: dhcpd_execute
**      descriptions:
**            To execute dhcpd node service.
**
**      parameters:
**            node:   Put the dhcpd of cfg node tree that is used to execute dhcpd settings.
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
dhcpd_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){

	char buf[64], nodeName[MAX_ARG_NUM][MAX_NODE_NAME];

	system("killall -9 udhcpd");

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],LAN);
	strcpy(nodeName[1],LAN_DHCP);
	if(getAttrValue(top, nodeName, LAN_DHCP_TYPE, buf) != TCAPI_PROCESS_OK){
		return FAIL;
	}
	if(!strcmp(buf,"1")){
		kill_process("/var/log/dhcrelay.pid");
		unlink(DHCPRELAY_PATH);
		system("/usr/sbin/udhcpd &");
	}else {
		unlink(DHCPD_PATH);
		#ifdef STATIC_DHCP
		unlink(DHCPLEASE_PATH);
		#endif		
	}
	return SUCCESS;
}/* end dhcpd_execute */

//"DhcpLease",		UPDATE_TYPE|WEB_TYPE,	NULL, NULL, dhcpLease_read, NULL, NULL, NULL},
/*______________________________________________________________________________
**	dhcpLease_init
**
**	descriptions:
**		the init function of dhcpLease.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
dhcpLease_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="DhcpLease",
			.type=UPDATE_TYPE|WEB_TYPE|ENTRY_TYPE|MAX_LEASE_NUM,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=dhcpLease_read,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=NULL,
			.cfg_boot=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}

/*_____________________________________________________________________________
**      function name: dhcpLease_read
**      descriptions:
**            To create dhcpLease cfg node and filled those attribution value.
**
**      parameters:
**            top:   Put the top of cfg node tree.
**            name: Put the sub Info cfg node name.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:**   
**
**      revision:
**      1. liujilin 2010/01/11
**____________________________________________________________________________
*/
int
dhcpLease_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr){
	FILE *fp = NULL;
	mxml_node_t *group = NULL;  /* <DhcpLease Node> */
	mxml_node_t *node = NULL;   /* <element node> */
	int i,skipOnce = 0, total = 0;
	char buf[MAXSIZE]={0},mac[17], ip[16],expire[10], hostname[32];
	char subNode[16]={0}, count[3]={0};
	//time_t curTime = time(0);
	time_t curTime;
	struct  timespec  curtime;
	int timeLeft, days = 0,hours = 0, minutes = 0, seconds = 0;
	clock_gettime(CLOCK_MONOTONIC, &curtime);
	curTime = curtime.tv_sec;
	
		
	group = mxmlFindElement(top, top, DHCPLEASE,
			NULL, NULL, MXML_DESCEND);
	if(group == NULL ){		
		group = mxmlNewElement(top,  DHCPLEASE);		
		//tcdbg_printf("\ndhcpLease_read: Create node %s !\n", DHCPLEASE);	
	}
	
	/*delete all Entry subnode*/
	for(i=0;;i++){
		sprintf(subNode,"%s%d",SUB_NODE_NAME, i);
		node = mxmlFindElement(group, group, subNode,
		NULL, NULL, MXML_DESCEND);
		if(node){
			mxmlDelete(node);
		}else{
			if(skipOnce==0){
				/*skip once because there may be a null node in the Dhcpd node*/
				skipOnce++;
			}
			else{
				break;
			}
		}
	}
	
	fp=fopen(DHCPLEASE_PATH, "r");
	
	if(fp == NULL){
		//tcdbg_printf("\ndhcpLease_read:open %s  failed!\n",DHCPLEASE_PATH);
		sprintf(count,"%d",total);
		mxmlElementSetAttr(group, "LeaseNum", count);
		return FAIL;
	}
	/*Add lease item node from udhcp_lease*/
	while (fgets(buf, MAXSIZE, fp)){
		sprintf(subNode,"%s%d",SUB_NODE_NAME, total);
		sscanf(buf, "%s %s %s %s",mac, ip, expire, hostname);
		timeLeft = atoi(expire) - curTime;
		if(strcmp(mac, "00:00:00:00:00:00") && (timeLeft > 0)){
			days = timeLeft / (60*60*24);
			timeLeft %= 60*60*24;
			hours = timeLeft / (60*60);
			timeLeft %= 60*60;
			minutes = timeLeft / 60;
			seconds = timeLeft % 60;
			node=mxmlNewElement(group,  subNode);
			mxmlElementSetAttr(node, "MAC", mac);
			mxmlElementSetAttr(node, "IP", ip);
			mxmlElementSetAttr(node, "HostName", hostname);	
			sprintf(expire,"%d",days);
			mxmlElementSetAttr(node, "ExpireDay", expire);	
					
			sprintf(expire,"%d:%d:%d",hours,minutes,seconds);
			mxmlElementSetAttr(node, "ExpireTime", expire);	

			total++;
		}else
			continue;	
		memset(buf, 0, sizeof(buf));
	}
	fclose(fp);

	sprintf(count,"%d",total);
	mxmlElementSetAttr(group, "LeaseNum", count);
	return SUCCESS;
}/*end dhcpdlease_read*/

#if 0  /*Orginal ci-command mode */
/*______________________________________________________________________________
**	dhcpRelay_read
**
**	descriptions:
**		Read dhcp Relay system parameter from configuration file and record those information
**		to mxml_node_t.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
dhcpRelay_read(mxml_node_t *top){

	FILE *fp = NULL;
	char tmp[MAXSIZE];
	char dhcpRelayInfo[4][32];

	mxml_node_t *group= NULL;  /* <DhcpRelay> */
	mxml_node_t *node= NULL;   /* <node> */


	group = mxmlNewElement(top, DHCPRELAY);
	fp = fopen(DHCPRELAY_PATH, "r");
	if(fp == NULL){
		return FAIL;
	}
	else{
		if(fgets(tmp,MAXSIZE,fp)){/* /bin/dhcrelay -pf /var/log/dhcrelay.pid 10.10.10.100 */
			memset(dhcpRelayInfo, 0, sizeof(dhcpRelayInfo));
			sscanf(tmp, "%s %s %s %s",
			dhcpRelayInfo[0], dhcpRelayInfo[1], dhcpRelayInfo[2], dhcpRelayInfo[3]);

			node = mxmlNewElement(group, DHCPRELAY_ENTRY);
			mxmlElementSetAttr(node, DHCPRELAY_ATTR_SERVER, dhcpRelayInfo[DHCPRELAY_SERVER_INDEX]);
		}
		fclose(fp);
		return 0;
	}
}/* end dhcpRelay_read */
#endif

int check_arpIP_mode(char *strIP, char *strMAC, FILE *fp, char* getteddexpire, char* gettedhostname) 
{
	char buf[MAXSIZE]={0},mac[17], ip[40],expire[10], hostname[32];
	
	if(fp == NULL)
		return HOST_MODE_STATIC;//no dhcp

	fseek(fp, 0, 0);//seek back to the file beginning. 0:SEEK_SET
	
	while (fgets(buf, MAXSIZE, fp))
	{
		sscanf(buf, "%s %s %s %s",mac, ip, expire, hostname);
		trimIP(strIP);
		trimIP(ip);
		trimIP(strMAC);
		trimIP(mac);	
		if((!strcmp(strIP,ip)) || (! strcasecmp(strMAC, mac))) {
			//tcdbg_printf("\ncheck_arpIP_mode, same ip %s ,%s in dhcp lease\n", strIP, strMAC);	
			strcpy(getteddexpire,expire);
			strcpy(gettedhostname,hostname);
			return HOST_MODE_DHCP;
		}

		memset(buf, 0, sizeof(buf));
	}

	return HOST_MODE_STATIC;	

}

//modified by fredli for bug 11565, hostnum shown not correct on CWMP
//substitue gettimeofday() with clock_gettime()
//struct timeval gLastGetTime;
struct timespec gLastGetTime;
int gTheFirstGet = 0;

//"LanHost",		UPDATE_TYPE|WEB_TYPE,	NULL, NULL, lanHost_read, NULL, NULL, NULL},
/*______________________________________________________________________________
**	lanHost_init
**
**	descriptions:
**		the init function of lanHost.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
lanHost_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="LanHost",
			.type=UPDATE_TYPE|WEB_TYPE|ENTRY_TYPE|MAX_LANHOST_NUM,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=lanHost_read,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=NULL,
			.cfg_boot=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}

/*_____________________________________________________________________________
**      function name: lanHost_read
**      descriptions:
**            To create LanHost cfg node and filled those attribution value.
**
**      parameters:
**            top:   Put the top of cfg node tree.
**            name: Put the sub Info cfg node name.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:**   
**
**      revision:
**      1. wangyongzhi 2011/06/10
**____________________________________________________________________________
*/
int
lanHost_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr){
	FILE *fp = NULL;
	FILE *fp_dhcplease = NULL;
	mxml_node_t *lanHost_group = NULL;  /* <LanHost Node> */
	mxml_node_t *lanHost_node = NULL;   /* <element node> */
	mxml_node_t *dhcpLease_group = NULL;  /* <DhcpLease Node> */
	mxml_node_t *dhcpLease_node = NULL;   /* <element node> */	
	int i, j, k, skipOnce = 0, total = 0;	
	char buf[MAXSIZE]={0};
	char subNode[16]={0}, count[3]={0};
	const char *attrValue = NULL;
	int dhcpLeaseNum = 0;
#if defined(TCSUPPORT_CT_PMINFORM)
	char EthMacAttr[16]={0};
	char WlanDevMAC[512]={0};
#endif
	char cmd[64]={0};
	char ip[40]={0},lladdr[15]={0},mac[20]={0},stale[15]={0};
	mxml_node_t *node = NULL;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char mask[64] = {0};
	char instance[128] = {0};
	char mac_fdb[32] = {0};
	char ip_gateway[40]={0};
	int id = 0;
	FILE *fp_br_fdb = NULL;
	#ifdef TCSUPPORT_WLAN	
	int ssidNum = 0, ssidClientNum = 0;
	#endif	
	tcapi_msg_t msg;
	//modified by fredli for bug 11565, hostnum shown not correct on CWMP
	//struct timeval cur;
	struct timespec cur;
	
	int timeLeft, days = 0,hours = 0, minutes = 0, seconds = 0;
	time_t curTime;
	char expire[10]={0}, hostname[32]={0};
	int ret = 0;

	enum lanHost_en{
			dhcp_MAC=0,
			dhcp_IP,
			dhcp_HostName,
			dhcp_ExpireDay,
			dhcp_ExpireTime,
			host_AddressSrc,
			host_IfType,
			host_Active,
			host_Layer2Interface,
			dhcp_Max,
	};

	char lanHost_attr[][16] =
	{
		{"MAC"},
		{"IP"},
		{"HostName"},
		{"ExpireDay"},
		{"ExpireTime"},
		{"AddressSrc"},
		{"InterfaceType"},
		{"Active"},
		{"Layer2Interface"},
		{""},
	};
			
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],LAN);
	strcpy(nodeName[1],LAN_ENTRY_0);

	if((getAttrValue(top, nodeName, LAN_IP, ip_gateway) != TCAPI_PROCESS_OK) || (getAttrValue(top, nodeName, LAN_MASK, mask) != TCAPI_PROCESS_OK))
	{
		tcdbg_printf("Get Lan IP or mask fail!");
		return FAIL;
	}
			
	/*Add a time check*/
	if(gTheFirstGet == 0) {
		//modified by fredli for bug 11565, hostnum shown not correct on CWMP
		//substitue gettimeofday with clock_gettime
		//gettimeofday(&gLastGetTime, NULL);
		clock_gettime(CLOCK_MONOTONIC,&gLastGetTime);
		gTheFirstGet = 1;
	}
	else {
		//modified by fredli for bug 11565, hostnum shown not correct on CWMP
		//substitue gettimeofday with clock_gettime
		//gettimeofday(&cur, NULL);
		clock_gettime(CLOCK_MONOTONIC,&cur);
		if(cur.tv_sec - gLastGetTime.tv_sec < LANHOST_GET_INTERVAL) {
			//tcdbg_printf("\nlanHost_read: no need to do read!\n");
			return SUCCESS;
		}
		else {
			gLastGetTime.tv_sec = cur.tv_sec;
			gLastGetTime.tv_nsec = cur.tv_nsec;
			//gLastGetTime.tv_usec = cur.tv_usec; // structure timespec has no tv_usec
		}
	}
	
	curTime = gLastGetTime.tv_sec;
		
	lanHost_group = mxmlFindElement(top, top, LANHOST,
			NULL, NULL, MXML_DESCEND);
	if(lanHost_group == NULL ){		
		lanHost_group = mxmlNewElement(top,  LANHOST);		
		tcdbg_printf("\nlanHost_read: Create node %s !\n", LANHOST);	
		if(lanHost_group == NULL) {
			tcdbg_printf("\nlanHost_read: new node failed!\n");
			return FAIL;
	        }
	}

	/*delete all Entry subnode*/
	for(i=0;;i++){
		sprintf(subNode,"%s%d",SUB_NODE_NAME, i);
		lanHost_node = mxmlFindElement(lanHost_group, lanHost_group, subNode,
		NULL, NULL, MXML_DESCEND);
		if(lanHost_node){
			mxmlDelete(lanHost_node);
		}else{
			if(skipOnce==0){
				/*skip once because there may be a null node*/
				skipOnce++;
			}
			else{
				break;
			}
		}
	}
#if defined(TCSUPPORT_CT_PMINFORM)
	for(i=1;i<5;i++){
		sprintf(EthMacAttr, ETHDEVMAC_ATTR, i);
		mxmlElementSetAttr(lanHost_group, EthMacAttr, "");		
	}
	mxmlElementSetAttr(lanHost_group, WLANDEVMAC_ATTR, "");
#endif
#if defined(TCSUPPORT_CT_STB_TEST)
	mxmlElementSetAttr(lanHost_group, STBMAC_ATTR, "");
#endif

	//tcdbg_printf("\nlanHost_read: Extract static host info from arp table !\n");	
	/*2. Extract static host info from arp table*/
	sprintf(cmd,"/usr/bin/ip neigh show dev br0 > %s", ARP_INFO_PATH);
	system(cmd);
	
	fp=fopen(ARP_INFO_PATH, "r");	
	if(fp == NULL){
		tcdbg_printf("\nlanHost_read:open %s  failed!\n", ARP_INFO_PATH);
		goto setLeaseNumAttr;
	}

	fp_dhcplease=fopen(DHCPLEASE_PATH, "r");	
	if(fp_dhcplease == NULL){
		//tcdbg_printf("\nlanHost_read:open %s  failed!\n", DHCPLEASE_PATH);
	}
	
	while (fgets(buf, MAXSIZE, fp))
	{
		// get ip and mac info from file
		ret = sscanf(buf, "%s %s %s %s",ip,lladdr,mac,stale);

		if((ret == 4) && (inet_addr(ip) != INADDR_NONE) 
			&& (((inet_addr(ip))&(inet_addr(mask))) == ((inet_addr(ip_gateway))&(inet_addr(mask)))))
		{
				sprintf(subNode, "%s%d", SUB_NODE_NAME, total);/*Buf: Entryi*/
				lanHost_node = NULL;
				lanHost_node = mxmlNewElement(lanHost_group, subNode);/*Create LanHost Entry*/
				if(lanHost_node == NULL) {
					tcdbg_printf("\nlanHost_read: new a static enntry %d failed!\n", total);	
					break;
				}
				mxmlElementSetAttr(lanHost_node, lanHost_attr[dhcp_MAC], mac);
				mxmlElementSetAttr(lanHost_node, lanHost_attr[dhcp_IP], ip);
				mxmlElementSetAttr(lanHost_node, lanHost_attr[dhcp_HostName], " ");	
				mxmlElementSetAttr(lanHost_node, lanHost_attr[dhcp_ExpireDay], "0");	
				mxmlElementSetAttr(lanHost_node, lanHost_attr[dhcp_ExpireTime], "0");	
				mxmlElementSetAttr(lanHost_node, lanHost_attr[host_AddressSrc], "Static");
				mxmlElementSetAttr(lanHost_node, lanHost_attr[host_IfType], "Ethernet");
				mxmlElementSetAttr(lanHost_node, lanHost_attr[host_Active], "1");
				//tcdbg_printf("\nlanHost_read: add a static entry %d !\n", total);	
				/*2. check this node whether DHCP clinet*/
				if ((fp_dhcplease != NULL) && check_arpIP_mode(ip, mac, fp_dhcplease, expire, hostname) == HOST_MODE_DHCP)
				{
					timeLeft = atoi(expire) - curTime;

					days = timeLeft / (60*60*24);
					timeLeft %= 60*60*24;
					hours = timeLeft / (60*60);
					timeLeft %= 60*60;
					minutes = timeLeft / 60;
					seconds = timeLeft % 60;
					mxmlElementSetAttr(lanHost_node, lanHost_attr[dhcp_HostName], hostname);	
					sprintf(expire,"%d",days);
					mxmlElementSetAttr(lanHost_node, lanHost_attr[dhcp_ExpireDay], expire);	

					sprintf(expire,"%d:%d:%d",hours,minutes,seconds);
					mxmlElementSetAttr(lanHost_node, lanHost_attr[dhcp_ExpireTime], expire);					
					mxmlElementSetAttr(lanHost_node, lanHost_attr[host_AddressSrc], "DHCP");
				}

				fp_br_fdb = fopen(BR_FDB_PATH, "r");
				if(fp_br_fdb == NULL){
					tcdbg_printf("\nlanHost_read:open %s failed!\n", BR_FDB_PATH);
				}
				
				if(fp_br_fdb != NULL)
				{
					memset(buf, 0, sizeof(buf));
					while (fgets(buf, MAXSIZE, fp_br_fdb))
					{
						sscanf(buf, "%d=%s", &id, mac_fdb);

						if(0 == strcasecmp(mac,mac_fdb))
						{
							snprintf(instance, sizeof(instance),"InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.%d",id);
							mxmlElementSetAttr(lanHost_node, lanHost_attr[host_Layer2Interface], instance);
#if defined(TCSUPPORT_CT_PMINFORM)
							sprintf(EthMacAttr, ETHDEVMAC_ATTR, id);
							mxmlElementSetAttr(lanHost_group, EthMacAttr, mac);
#endif
#if defined(TCSUPPORT_CT_STB_TEST)
							if(2==id)
								mxmlElementSetAttr(lanHost_group, STBMAC_ATTR, mac);
#endif
							break;
						}
					}

					fclose(fp_br_fdb);
				}
				
				total++;	
			}
		memset(buf, 0, sizeof(buf));
	}
	fclose(fp);
	unlink(ARP_INFO_PATH);

	if(fp_dhcplease !=  NULL)
		fclose(fp_dhcplease);
	
setLeaseNumAttr:
	sprintf(count,"%d",total);
	mxmlElementSetAttr(lanHost_group, LEASENUM_ATTR, count);
	
#ifdef TCSUPPORT_WLAN
	/*3. Check the interface type for each host by look up wifi mac table*/
	for(i=0; i<total; i++) {
		lanHost_node = NULL;
		sprintf(subNode,"%s%d",SUB_NODE_NAME, i);/*Buf: Entryi*/
		lanHost_node = mxmlFindElement(lanHost_group, lanHost_group, subNode,
				NULL, NULL,MXML_DESCEND);
		if(lanHost_node){
			attrValue = NULL;
			attrValue = mxmlElementGetAttr(lanHost_node, lanHost_attr[dhcp_MAC]);
			memset(mac, 0, sizeof(mac));
			if(attrValue != NULL)
				memcpy(mac, attrValue, sizeof(mac));
			else
				tcdbg_printf("\nlanHost_read: check entry %d interface type, get mac failed!\n");
	
			//look for wifi mac table to check if it is 802.11
			/*Do tcapi_get to update WLan node*/
			memset(&msg,0,sizeof(tcapi_msg_t));
			sprintf(msg.node , "%s" ,WLAN_COMMON_NODE);
			strcpy(msg.attr, "BssidNum");
			tcapi_get_req(top,&msg);
			if(msg.retval == TCAPI_PROCESS_OK)
	   			ssidNum = atoi(msg.value);	   	
			else
 				ssidNum = 0;

			for(j=0; j<ssidNum; j++){
				/*Do tcapi_get to update wifiMacTab node*/
				memset(&msg,0,sizeof(tcapi_msg_t));
				sprintf(msg.node , "%s" , WIFIMACTAB_COMMON_NODE);
	         		sprintf(msg.attr,"NUM%d", j);
				tcapi_get_req(top,&msg);
				if(msg.retval == TCAPI_PROCESS_OK)
	   				ssidClientNum = atoi(msg.value);	   	
				else
 					ssidClientNum = 0;
	
				if(0 != ssidClientNum){
					for(k=0; k<ssidClientNum; k++){
						memset(&msg,0,sizeof(tcapi_msg_t));
						sprintf(msg.node , "wifiMacTab_Entry%d" , k);
			         		sprintf(msg.attr,"MAC%d", j);
						tcapi_get_req(top,&msg);
						if(msg.retval != TCAPI_PROCESS_OK)
	   						break;

						if(0 == strcasecmp(mac, msg.value)) {
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
							mxmlElementSetAttr(lanHost_node, lanHost_attr[host_IfType], "WLAN");
#else
							mxmlElementSetAttr(lanHost_node, lanHost_attr[host_IfType], "802.11");
#endif
	                                                snprintf(instance, sizeof(instance),"InternetGatewayDevice.LANDevice.1.WLANConfiguration.%d",j+1);
							mxmlElementSetAttr(lanHost_node, lanHost_attr[host_Layer2Interface], instance);
#if defined(TCSUPPORT_CT_PMINFORM)
							if(strlen(WlanDevMAC)>0)
								strcat(WlanDevMAC,"-");
							strcat(WlanDevMAC,mac);
#endif
							//tcdbg_printf("\nlanHost_read: entry %d type is 802.11!\n", i);	
							j = ssidNum; //break the upper loop
							break;
						}
					}
				}
			}
		}
	}
#if defined(TCSUPPORT_CT_PMINFORM)
	mxmlElementSetAttr(lanHost_group, WLANDEVMAC_ATTR, WlanDevMAC);
#endif
#endif

	return SUCCESS;
}

#ifdef IPV6
int create_br0_v6addr(char *pd, int pd_len, char *newV6addr)
{
	char ifname[] = "br0";
	int fd6 = 0;
	int i = 0;
	unsigned char ifid[16] = {0};
	char tmp_str[64] = {0};
	struct ifreq ifr = {0};
	struct in6_addr in_s_v6addr = {0};

	if ( NULL == pd || NULL == newV6addr )
		return -1;

	if ( pd_len < 16 )
	{
		tcdbg_printf("error:create_br0_v6addr() pd length is too short! \n");
		return -1;
	}

	if ((fd6 = socket(AF_INET6, SOCK_DGRAM, 0)) == -1)
		return -1;

	strcpy(ifr.ifr_name, ifname);	
	if(ioctl(fd6, SIOCGIFHWADDR, &ifr) < 0)
	{
		close(fd6);
		return -1;
	}

	ifid[2] = ifr.ifr_hwaddr.sa_data[0];
	ifid[2] ^= 0x02; /* reverse the u/l bit*/
	ifid[3] = ifr.ifr_hwaddr.sa_data[2];
	ifid[4] = ifr.ifr_hwaddr.sa_data[1];
	ifid[5] = 0xe3;
	ifid[6] = ifr.ifr_hwaddr.sa_data[4];
	ifid[7] = 0xef;

	ifid[8] = ifr.ifr_hwaddr.sa_data[0];
	ifid[8] ^= 0x02; /* reverse the u/l bit*/
	ifid[9] = ifr.ifr_hwaddr.sa_data[1];
	ifid[10] = ifr.ifr_hwaddr.sa_data[2];
	ifid[11] = 0xff;
	ifid[12] = 0xfe;
	ifid[13] = ifr.ifr_hwaddr.sa_data[3];
	ifid[14] = ifr.ifr_hwaddr.sa_data[4];
	ifid[15] = ifr.ifr_hwaddr.sa_data[5];

	close(fd6);

	if ( 1 != inet_pton(AF_INET6, pd, &in_s_v6addr) )
		return -1;


	for (i = 15; i >= pd_len / 8; i--)
		in_s_v6addr.s6_addr[i] = ifid[i];

	inet_ntop(AF_INET6, &in_s_v6addr.s6_addr, tmp_str, sizeof(tmp_str));
	strcpy(newV6addr, tmp_str);
	return 0;
}

int create_br0_pool_ipv6addr(mxml_node_t *top, char nodeName[][MAX_NODE_NAME], char *ipv6_addr)
{
	char prefix[50] = {0}, prefix_len[5] = {0};
	char v6addr_str[64] = {0};
	
	if(getAttrValue(top, nodeName, PREFIX_SIMBOL, prefix) != TCAPI_PROCESS_OK){
		tcdbg_printf("error:Dhcp6s function Get prefix value fail!\n");
		return -1;
	}
	if(getAttrValue(top, nodeName, PREFIX_LEN_SIMBOL, prefix_len) != TCAPI_PROCESS_OK){
		tcdbg_printf("error:Dhcp6s function Get prefix value fail!\n");
		return -1;
	}
	if ( 0 != create_br0_v6addr(prefix, atoi(prefix_len), v6addr_str) )
	{
		tcdbg_printf("error:Dhcp6s function create_br0_v6addr() fail!\n");
		return -1;
	}
	sprintf(ipv6_addr, "%s/%s", v6addr_str, prefix_len);
	return 0;
}

/*______________________________________________________________________________
**	radvdCall_boot
**
**	descriptions:
**		the boot function of radvdCall.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**_______________________________________________________________________________
*/
int
radvdCall_boot(mxml_node_t *top){
	//FILE *startupSh=NULL;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char value[8] = {0}, mode[5] = {0};
	char  ipv6_br0[50] = {0}, cmd[80] = {0};

	memset(nodeName, 0, sizeof(nodeName));

	strcpy(nodeName[0], "Radvd");
	strcpy(nodeName[1], "Entry");

	

	if(getAttrValue(top, nodeName, "Enable", value) != TCAPI_PROCESS_OK){
		printf("error:Radvd function Get enable value fail!\n");
		return -1;
	}

	if( 0 == strcmp(value, "1") )
	{
		if(getAttrValue(top, nodeName, "Mode", mode) != TCAPI_PROCESS_OK){
			tcdbg_printf("error:Dhcp6s function Get mode value fail!\n");
			return -1;
		}
		/*Manual mode*/
		if(mode[0] == '1'){			
			if(create_br0_pool_ipv6addr(top, nodeName, ipv6_br0) == 0){
				sprintf(cmd, "ifconfig br0 %s", ipv6_br0);
				system(cmd);
				/*save br0 address*/
				setAttrValue(top, nodeName, BR0_ADDR, ipv6_br0);
			}			
		}

		system("/userfs/bin/radvd -C /etc/radvd.conf -p /var/run/radvd.pid &");
		printf("Radvd function activated!\n");
	}
	else
	{
		printf("Radvd function NOT activated!\n");
		return FAIL;
	}
	return SUCCESS;
}

/*______________________________________________________________________________
**	radvdCall_init
**
**	descriptions:
**		the init function of radvd calls.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
radvdCall_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="Radvd",
			.type=SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=radvdCall_write,
			.cfg_verify=radvdCall_verify,
			.cfg_execute=radvdCall_execute,
			.cfg_boot=radvdCall_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}



/*_____________________________________________________________________________
**      function name: radvdCall_write
**      descriptions:
**            Write radvd options to /etc/radvd.conf configuration file.
**
**      parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  radvd cfg node.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:**   
**
**      revision:
**____________________________________________________________________________
*/
int
radvdCall_write(mxml_node_t *top, mxml_node_t *parant){

	mxml_node_t *node = NULL;
	char ip_v6[40]={0},
		prefix_v6len[5]={0},enable_flag[5] = {0},
              preffered_lifetime[16]={0},valid_lifetime[16]={0},mode[5] = {0};	
	char dhcpv6_wan_pd_ptime[16] = {0}, dhcpv6_wan_pd_vtime[16] = {0};
#if defined(TCSUPPORT_CT_E8GUI)
	char managed_flag[5] = {0}, other_flag[5] = {0},max_raInterval[8] = {0},min_raInterval[8] = {0};
	char state_other[5] = {0}, state_managed[5] = {0};
#endif
	char *tmp= NULL;
	char buf[512]={0};
	int len = 0;
#if defined(TCSUPPORT_CT_WAN_CHILD_PREFIX)				
	char pd_index_v6[5] = {0};
	char new_prefix[45] = {0};	
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char status6[8] = {0};
	char temp_str[45] = {0};
	char *temp2_p = NULL;
	int temp_len = 0;
	int pd_index = 0;
#endif
	
	node = mxmlFindElement(parant, parant, SUB_NODE_NAME,
		NULL, NULL, MXML_DESCEND);

	if(node != NULL){
		
		

			tmp= (char *) mxmlElementGetAttr(node, PREFIX_SIMBOL);
			if(tmp!=NULL){			
				strcpy(ip_v6, tmp);
			}
			tmp= (char *) mxmlElementGetAttr(node, PREFIX_LEN_SIMBOL);
			if(tmp!=NULL){			
				strcpy(prefix_v6len, tmp);
			}

			tmp= (char *) mxmlElementGetAttr(node, IPV6_ENABLE_SIMBOL);
			if(tmp!=NULL){			
				strcpy(enable_flag, tmp);
			}

			tmp= (char *) mxmlElementGetAttr(node, RADVD_MODE);
			if(tmp!=NULL){			
				strcpy(mode, tmp);
			}

#if defined(TCSUPPORT_CT_E8GUI)
			/*set M/O flag*/
			strcpy(state_managed,"off");
			strcpy(state_other, "on");
			strcpy(min_raInterval, RADVD_MINRAINTERVAL_TIME);
			strcpy(max_raInterval, RADVD_MAXRAINTERVAL_TIME);

			tmp= (char *) mxmlElementGetAttr(node, RADVD_MANAGE_ENABLE);
			if(tmp!=NULL){			
				strcpy(managed_flag, tmp);
			}
			tmp = (char *) mxmlElementGetAttr(node, RADVD_OTHER_ENABLE);
			if(tmp!=NULL){			
				strcpy(other_flag, tmp);
			}
				
			tmp= (char *) mxmlElementGetAttr(node, RADVD_MINRAINTERVAL);
			if(tmp!=NULL){			
				strcpy(min_raInterval, tmp);
			}
			tmp= (char *) mxmlElementGetAttr(node, RADVD_MAXRAINTERVAL);
			if(tmp!=NULL){			
				strcpy(max_raInterval, tmp);
			}
		
			if('1' == other_flag[0])
			{
				strcpy(state_other,"on");
			}
			else
			{
				strcpy(state_other,"off");
			}

			if('1' == managed_flag[0])
			{
				strcpy(state_managed,"on");
			}
			else
			{
				strcpy(state_managed,"off");
			}
#else
			/*Auto Mode*/
			if('0' == mode[0])
			{
				strcpy(state_mode,"on");
			}
			else/*Manual mode*/
			{
				strcpy(state_mode,"off");
			}
#endif
			
			tmp= (char *) mxmlElementGetAttr(node, PREFFERED_LIFETIME_SIMBOL);
			if(tmp!=NULL){			
				strcpy(preffered_lifetime, tmp);
			}

			tmp= (char *) mxmlElementGetAttr(node, VALID_LIFETIME_SIMBOL);
			if(tmp!=NULL){			
				strcpy(valid_lifetime, tmp);
			}
			
						
#if defined(TCSUPPORT_CT_E8GUI)
			sprintf(buf, "interface br0  \
 			\n{\n\tAdvSendAdvert on; \
 			\n\tAdvManagedFlag %s; \
 			\n\tAdvOtherConfigFlag %s; \
			\n\tMinRtrAdvInterval %s; \
 			\n\tMaxRtrAdvInterval %s; ",state_managed,state_other,min_raInterval,max_raInterval);
#else
 			sprintf(buf, "interface br0  \
 			\n{\n\tAdvSendAdvert on; \
 			\n\tAdvManagedFlag %s; \
 			\n\tAdvOtherConfigFlag %s; ",state_mode,state_mode);

#endif
			 //when mode == 1  mannual mode is enabled, prefix information is added into configuration
			 //when mode == 0 auto mode is enabled			
			if(mode[0] == '1')
			{
				len = strlen(buf);
	 			sprintf(buf+len, "\n\tprefix %s/%s\n\t{ \
	 			\n\tAdvOnLink on; \
	 			\n\tAdvAutonomous on; \
	 			\n\tAdvRouterAddr off; \
	 			\n\tAdvValidLifetime %s; \
	 			\n\t AdvPreferredLifetime %s; \
	 			\n\t};",
	 				ip_v6, prefix_v6len,
					valid_lifetime,preffered_lifetime);
			}
#if defined(TCSUPPORT_CT_E8GUI) && defined(TCSUPPORT_CT_WAN_CHILD_PREFIX)
			else if('0' == mode[0]){
				/*Get prefix delegation from Wan interface which is default route*/
				memset(ip_v6, 0, sizeof(ip_v6));
				memset(prefix_v6len, 0, temp_len);

				pd_index = getRaWanPDInterface();
				if ( pd_index >= 0 )
				{
					sprintf(pd_index_v6, "%d", pd_index);
					tcdbg_printf("radvdCall_write:pd_index_v6=%s\n",pd_index_v6);
							
					memset(nodeName, 0, sizeof(nodeName));
					strcpy(nodeName[0],"WanInfo");
					sprintf(nodeName[1], "Entry%s", pd_index_v6);

					memset(status6, 0, sizeof(status6));
					getAttrValue(top, nodeName, "Status6", status6);

					memset(temp_str, 0, sizeof(temp_str));			
					getAttrValue(top, nodeName, "PD6", temp_str);				
					tcdbg_printf("radvdCall_write:PD6=%s\n",temp_str);
								
					getAttrValue(top, nodeName, "PD6ptime", dhcpv6_wan_pd_ptime);
					getAttrValue(top, nodeName, "PD6vtime", dhcpv6_wan_pd_vtime);		
					/*generate prefix for lan DHCP*/
					if(lan_ipv6_prefix_gen(new_prefix, pd_index_v6) == 0){
						strcpy(temp_str, new_prefix);
					}
					
					tmp = strstr(temp_str, "/");				
					if((0 != strcmp(temp_str, ""))
						&& (tmp != NULL)
						&& !strcmp(status6, "up")){					
						/*Parse prefix and prefix-len from PD6,eg:3000:456:AEF:DCA::/64*/
						temp2_p = tmp+1;
						*tmp = '\0';
						
						strcpy(ip_v6, temp_str);
						temp_len = sizeof(prefix_v6len);					
						strncpy(prefix_v6len, temp2_p, temp_len-1);
						
						if ( '\0' != dhcpv6_wan_pd_ptime[0]
								&& '\0' != dhcpv6_wan_pd_vtime[0] )
						{
							strcpy(preffered_lifetime, dhcpv6_wan_pd_ptime);
							strcpy(valid_lifetime, dhcpv6_wan_pd_vtime);
						}
						else
						{
						strcpy(preffered_lifetime, RADVD_DEF_PREFER_LIFE);
						strcpy(valid_lifetime, RADVD_DEF_VALID_LIFE);
						}
						
						len = strlen(buf);
						sprintf(buf+len, "\n\tprefix %s/%s\n\t{ \
						\n\tAdvOnLink on; \
						\n\tAdvAutonomous on; \
						\n\tAdvRouterAddr off; \
						\n\tAdvValidLifetime %s; \
						\n\t AdvPreferredLifetime %s; \
						\n\t};",
							ip_v6, prefix_v6len,
							valid_lifetime,preffered_lifetime);
						
					}
				}
			}				
#endif

			len = strlen(buf);
			sprintf(buf+len, "\n};\n");

			   if(write2file(buf,IPV6_CONFPATH) == FAIL)
			  {
			     printf("fail to write file /etc/radvd.conf\n");
			     return FAIL;
		         }

		
	}
	else{
		unlink(IPV6_CONFPATH);
	}	
	chmod(IPV6_CONFPATH,777);
	return SUCCESS;
	
}/*  end radvdCall_write */

/*______________________________________________________________________________
**	radvdCall_verify
**
**	descriptions:
**		To check the system parameter of radvd call  element  is validly or not.
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
radvdCall_verify(mxml_node_t *node){
	return SUCCESS;
}/* end radvdCall_verify */

/*_____________________________________________________________________________
**      function name: radvdCall_execute
**      descriptions:
**            To execute radvd according to configuration.
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
**      1. rclv 20100517
**____________________________________________________________________________
*/
int
radvdCall_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){

	

	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char enable[3] = {0};
	char mode[5] = {0}, ipv6_br0[50] = {0}, cmd[80] = {0};
	char pid[8]={0};

	memset(nodeName, 0, sizeof(nodeName));

	strcpy(nodeName[0], "Radvd");
	strcpy(nodeName[1], "Entry");

	

	if(getAttrValue(top, nodeName, "Enable", enable) != TCAPI_PROCESS_OK){
		tcdbg_printf("error:Radvd function Get enable value fail!\n");
		return -1;
	}

	if(getAttrValue(top, nodeName, BR0_ADDR, ipv6_br0) == TCAPI_PROCESS_OK && strcmp(ipv6_br0, "N/A")){
			sprintf(cmd, "ifconfig br0 del %s", ipv6_br0);
			system(cmd);
			setAttrValue(top, nodeName, BR0_ADDR, "N/A");
	}	

#if 1
	fileRead("/var/run/radvd.pid", pid, sizeof(pid));
	if(strlen(pid) > 0){
		kill(atoi(pid), SIGTERM);
		unlink("/var/run/radvd.pid");
	}
#else	
	kill_process("/var/run/radvd.pid");
#endif

	
	if ( enable[0] == '1' )  
	{
		if(getAttrValue(top, nodeName, "Mode", mode) != TCAPI_PROCESS_OK){
			tcdbg_printf("error:Dhcp6s function Get mode value fail!\n");
			return -1;
		}
		/*Manual mode*/
		if(mode[0] == '1'){			
			if(create_br0_pool_ipv6addr(top, nodeName, ipv6_br0) == 0){
				sprintf(cmd, "ifconfig br0 %s", ipv6_br0);
				system(cmd);
				/*save br0 address*/
				setAttrValue(top, nodeName, BR0_ADDR, ipv6_br0);
			}			
		}

		system("/userfs/bin/radvd -C /etc/radvd.conf -p /var/run/radvd.pid &");
		tcdbg_printf("radvd parameter activated by exec!\n");
				
	}

	else if( '0' == enable[0] )
	{
		tcdbg_printf("radvd call closed by exec!\n");
	}
	return SUCCESS;
}/* end radvdCall_execute */






/*______________________________________________________________________________
**	dhcp6sCall_boot
**
**	descriptions:
**		the boot function of dhcp6sCall.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**_______________________________________________________________________________
*/
int
dhcp6sCall_boot(mxml_node_t *top){
	//FILE *startupSh=NULL;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char value_en[3] = {0};

	char value_mode[3] = {0};
	char  ipv6_br0[50] = {0}, cmd[80] = {0};

	memset(nodeName, 0, sizeof(nodeName));

	strcpy(nodeName[0], "Dhcp6s");
	strcpy(nodeName[1], "Entry");

	

      
	if(getAttrValue(top, nodeName, "Enable", value_en) != TCAPI_PROCESS_OK){
		printf("error:Dhcp6s function Get enable value fail!\n");
		return FAIL;
	}

	if( 0 != strcmp(value_en, "1") )
	{
		printf("Enable:Dhcp6s function NOT activated!\n");
		return SUCCESS;
	}

	
	/* judging the dhcp6s mode 1 for manual mode; 0 for auto mode
	   even dhcp6s is enabled, in auto mode  this function will 
	   NOT be started !
 	*/
	if(getAttrValue(top, nodeName, "Mode", value_mode) != TCAPI_PROCESS_OK){
		printf("error:Dhcp6s function Get enable value fail!\n");
		return FAIL;
	}

	if( 0 != strcmp(value_mode, "1") )
	{
		printf("Mode:Dhcp6s mode is not manual!\n");
		return SUCCESS;
	}


	if(create_br0_pool_ipv6addr(top, nodeName, ipv6_br0) == 0){
		sprintf(cmd, "ifconfig br0 %s", ipv6_br0);
		system(cmd);
		/*save br0 address*/
		setAttrValue(top, nodeName, BR0_ADDR, ipv6_br0);
	}
	

	system("/userfs/bin/dhcp6s -c /etc/dhcp6s.conf br0 -P /var/run/dhcp6s.pid &");
	printf("Dhcp6s function activated!\n");

	return SUCCESS;
}/* end dhcp6sCall_boot */

#if defined(TCSUPPORT_CT_DHCP6_OPTION)
char *
dhcp6s_adv_table[MAX_NODE_NAME]=
{
	"Option16","Option17",
	"",
};
#endif

/*______________________________________________________________________________
**	dhcp6sCall_init
**
**	descriptions:
**		the init function of dhcp6s calls.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
dhcp6sCall_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="Dhcp6s",
#if defined(TCSUPPORT_CT_DHCP6_OPTION)
			.type=SINGLE_TYPE|ADV_FLAG,
			.adv_table=dhcp6s_adv_table,
#else
			.type=SINGLE_TYPE,
			.adv_table=NULL,
#endif
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=dhcp6sCall_write,
			.cfg_verify=dhcp6sCall_verify,
			.cfg_execute=dhcp6sCall_execute,
			.cfg_boot=dhcp6sCall_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}/* end dhcp6sCall_init*/


/*______________________________________________________________________________
**	dhcp6sCall_init
**
**	descriptions:
**		setting the local IPv6 address to ::2 according to a given prefix
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int ipv6AddressTo01(char *Dest, char * Src)
{

	strncpy(Dest,Src,strlen(Src));

	Dest++;	

	while(*Dest != '\0')
	{

		if( (Dest[-1] == ':') && ( *Dest == ':' ) )
		{
			break;
		}
		Dest++;
	}

	
	Dest ++;	
	*Dest = '1';
	Dest ++;
	*Dest = '\0';

	return 0;
	
}


/*______________________________________________________________________________
**	dhcp6sCall_init
**
**	descriptions:
**		setting the local IPv6 address to ::1000 according to a given prefix
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int ipv6AddressToFF(char *Dest, char *Src)
{

	if( (*Src == ':') || (*Src == '\0') )
		return -1;


	
	strncpy( Dest, Src, strlen(Src) );
	while(*Dest++ );
	Dest--;

	*Dest = '1';Dest ++;  *Dest = '0';Dest ++;
	*Dest = '0';Dest ++;  *Dest = '0';Dest ++;  
	*Dest = '\0';
	Dest ++;

	return 0;
	
}


#if defined(TCSUPPORT_CT_WAN_CHILD_PREFIX)
/*******************************************************************************************
**function name
	lan_ipv6_prefix_gen
**description:
	get prefix for lan dhcp 
 **retrun 
	0:success
	-1:failure
**parameter:
	result_prefix: result prefix value
	entry:  entry index
********************************************************************************************/
int lan_ipv6_prefix_gen(char * result_prefix, char *entry){
	int ret = -1, fd = -1, result = -1;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	mxml_node_t *top = get_node_top();
	char tmp_str[64] = {0}, path[64] = {0};
	char prefix_str[64] = {0};
	char child_prefix_str[64] = {0};
	struct in6_addr prefix_addr;
	struct in6_addr child_prefix_addr;
	int prefix_len = 0, child_prefix_len = 0, child_prefix_orign = 0;
	int pvc_idx = 0, pvc_entry = 0,entry_idx = 0;
	int prfix_type = PREFIX_ORIGN_DFT;
	int tmp_val = 0;
	char * tmp_p = NULL;
	char * tmp2_p = NULL;
	int start_mask =0, end_mask = 0, mask_range = 0;
	int i,j,k;
	unsigned char tmp_u8 = 0;
	
	if(result_prefix == NULL || entry == NULL || (strlen(entry)<=0)){
		goto end;
	}
	/*init*/
	entry_idx = atoi(entry);
	pvc_idx = entry_idx / MAX_SMUX_NUM;
	pvc_entry = entry_idx % MAX_SMUX_NUM;
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],WAN);
  	sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_idx);
	sprintf(nodeName[2], "%s%d", WAN_INTERFACE, pvc_entry);

	/*get ipv6Prefix.ChildPrefixBits*/
	if(getAttrValue(top, nodeName, "PDChildPrefixBits", tmp_str) != TCAPI_PROCESS_OK){
		goto end;
	}
	tmp_val = strlen(tmp_str);
	if(tmp_val <= 0 || tmp_val > MAX_PREFIX_ADDR_LEN){
		goto end;
	}
	strcpy(child_prefix_str, tmp_str);
	
	/*get ipv6Prefix.origin*/
	if(getAttrValue(top, nodeName, "DHCPv6PD", tmp_str) == TCAPI_PROCESS_OK){
		if(strcmp(tmp_str,"Yes") == 0){
			prfix_type = PREFIX_ORIGN_DHCP;
			/*get prefix for dhcp*/
			memset(nodeName, 0, sizeof(nodeName));
		  	strcpy(nodeName[0],"WanInfo");
		  	sprintf(nodeName[1], "Entry%d", entry_idx);
			if(getAttrValue(top, nodeName, "PD6", tmp_str) != TCAPI_PROCESS_OK){
				goto end;
			}
			tmp_val = strlen(tmp_str);
			if(tmp_val <= 0 || tmp_val > MAX_PREFIX_ADDR_LEN){
				goto end;
			}
			strcpy(prefix_str, tmp_str);
		}
	}

	if(prfix_type == PREFIX_ORIGN_DFT){
		if(getAttrValue(top, nodeName, "PDOrigin", tmp_str) != TCAPI_PROCESS_OK){
			goto end;
		}
		if(strcmp(tmp_str,"RouterAdv") == 0){
			prfix_type = PREFIX_ORIGN_SLLA;
			/*get prefix for SLAAC*/
			/*get ifc name*/
			if(getAttrValue(top, nodeName, "IFName", tmp_str) != TCAPI_PROCESS_OK){
				goto end;
			}
			/*check parent prefix*/ 
			sprintf(path, "/proc/sys/net/ipv6/conf/%s/slaac_prefix", tmp_str);
			fd=open(path, O_RDONLY|O_NONBLOCK);
			if(fd == -1){
				goto end;
			}
			ret = read(fd, tmp_str, MAX_PREFIX_ADDR_LEN);
			close(fd);
			if((ret <= 1) || (strlen(tmp_str)<=0)){
				goto end;
			}
			strcpy(prefix_str, tmp_str);
		}else if (strcmp(tmp_str,"Static") == 0){
#if 0
			prfix_type = PREFIX_ORIGN_STATIC;
			/*get prefix for static*/
			if(getAttrValue(top, nodeName, "PDPrefix", tmp_str) != TCAPI_PROCESS_OK){
				goto end;
			}
			tmp_val = strlen(tmp_str);
			if(tmp_val <= 0 || tmp_val > MAX_PREFIX_ADDR_LEN){
				goto end;
			}
			strcpy(prefix_str, tmp_str);
#endif
			//nothing
		}
	}

	if(prfix_type == PREFIX_ORIGN_DFT){
		goto end;
	}
	
	/*get child prefix len*/
	tmp_p = strstr(child_prefix_str,"/");
	if(tmp_p == NULL){
		goto end;
	}
	tmp2_p = tmp_p +1;
	child_prefix_len = atoi(tmp2_p);

	if(child_prefix_len <= 0 || child_prefix_len > MAX_PREFIX_MASK_LEN){
		goto end;
	}

	*tmp_p = '\0';
	/*get child prefix ipv6 addr*/
	inet_pton(AF_INET6, child_prefix_str, (void *)&child_prefix_addr);

	/*get prefix len*/
	tmp_p = strstr(prefix_str,"/");
	if(tmp_p == NULL){
		goto end;
	}
	tmp2_p = tmp_p +1;
	prefix_len = atoi(tmp2_p);

	if(prefix_len <= 0 || prefix_len > MAX_PREFIX_MASK_LEN){
		goto end;
	}

	*tmp_p = '\0';
	/*get prefix ipv6 addr*/
	inet_pton(AF_INET6, prefix_str, (void *)&prefix_addr);

	/*calculate new prefix based on childprefix and parentprefix*/
	start_mask = prefix_len;
	end_mask = child_prefix_len;
	mask_range = end_mask - start_mask;
	if(mask_range <= 0 ){
		if(mask_range == 0){
			result = 0;
		}
		goto end;
	}

	for(i=start_mask; i<end_mask; i++){
		j = i/8;
		k = i%8;
		tmp_u8 = (1<<k);
		tmp_u8 &= child_prefix_addr.s6_addr[j];
		prefix_addr.s6_addr[j] |= tmp_u8;
	}	

	prefix_len = child_prefix_len;	
	result = 0;
end:
	if(result == 0){
		if(prefix_len > 64){
			prefix_len = 64;
		}
		memset(prefix_str, 0 , sizeof(prefix_str));
		inet_ntop(AF_INET6,&prefix_addr.s6_addr, prefix_str, sizeof(prefix_str));
		sprintf(tmp_str,"%s/%d", prefix_str, prefix_len);
		strncpy(result_prefix, tmp_str, MAX_PREFIX_ADDR_LEN);
		result_prefix[MAX_PREFIX_ADDR_LEN] = '\0';
	}
	return result;
}

/*******************************************************************************************
**function name
	lan_ipv6_addr_gen
**description:
	generate start or end ipv6 address for lan dhcpv6 pool.
 **retrun 
	0:success
	-1:failure
**parameter:
	ipv6_addr_str: saving the result 
	attr:  attribute value (MinAddress, MaxAddress)
	prefix_str: the current prefix string.
********************************************************************************************/
int lan_ipv6_addr_gen(char * ipv6_addr_str, char * attr, char * prefix_str){
	int ret = -1;
	struct in6_addr prefix_addr;
	struct in6_addr ipv6_addr;
	int ip6_len = sizeof(struct in6_addr);
	char tmp_str[64] = {0};
	char tmp_str2[64] = {0};
	char attr_str[64] = {0};
	char * tmp_p = NULL;
	char * tmp2_p = NULL;	
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	mxml_node_t *top = get_node_top();
	int i = 0;

	/*init*/
	if(ipv6_addr_str == NULL || attr == NULL || prefix_str == NULL){
		goto end;
	}
	tmp_p = strstr(prefix_str, "/");
	if(tmp_p != NULL){
		*tmp_p = '\0';
	}
	if(strlen(attr) == 0 || strlen(prefix_str) == 0){
		goto end;
	}
	memset(&prefix_addr, 0, ip6_len);
	memset(&ipv6_addr, 0, ip6_len);
	strcpy(ipv6_addr_str,"");

	/*get prefix ipv6 addr*/	
	inet_pton(AF_INET6, prefix_str, (void *)&prefix_addr);

	/*get min or max address*/	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],"Dhcp6s");
  	sprintf(nodeName[1], "Entry");
	if(getAttrValue(top, nodeName, attr, attr_str) != TCAPI_PROCESS_OK){
		goto end;		
	}

	tmp_p = strstr(attr_str, "/");
	if(tmp_p != NULL){
		*tmp_p = '\0';
	}

	if(strlen(attr_str) <= 0){
		goto end;
	}
	memset(tmp_str2, 0, sizeof(tmp_str2));
	if(strstr(attr_str,"::") == NULL){
		strcpy(tmp_str2,"::");
		strcat(tmp_str2, attr_str);
	}else{
		strcpy(tmp_str2,attr_str);
	}

	memset(tmp_str, 0, sizeof(tmp_str));
	shrink_ipv6addr(tmp_str2, tmp_str, sizeof(tmp_str));
	inet_pton(AF_INET6, tmp_str, (void *)&ipv6_addr);

	for(i = 8; i<16; i++){
		prefix_addr.s6_addr[i] |= ipv6_addr.s6_addr[i];
	}
	inet_ntop(AF_INET6,&prefix_addr.s6_addr, tmp_str, sizeof(tmp_str));
	strcpy(ipv6_addr_str,tmp_str);
	ret = 0;
end:
	return ret;

}
#endif

/*_____________________________________________________________________________
**      function name: dhcp6sCall_write
**      descriptions:
**            Writedhcp6s options to /etc/dhcp6s.conf configuration file.
**
**      parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 	Specify the parant node of  dhcp6s cfg node.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:**   
**
**      revision:
**____________________________________________________________________________
*/
int
dhcp6sCall_write(mxml_node_t *top, mxml_node_t *parant){

	mxml_node_t *node = NULL;
	char ip_v6[40]={0},dns1_v6[40]={0},dns2_v6[40]={0},
		prefix_v6len[5]={0},enable_flag[5] = {0},
              preffered_lifetime[16]={0},valid_lifetime[16]={0},
              mode[4]={0};
	char *tmp= NULL;
	char buf[512]={0};int len = 0;

	char ipv6_start[50]={0};
	char ipv6_end[50] = {0};
	char pd_index_v6[5] = {0}, temp[45] = {0}, status6[7] = {0};
	char dhcpv6_wan_pd_ptime[16] = {0}, dhcpv6_wan_pd_vtime[16] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];	
	char *p = NULL;
#if defined(TCSUPPORT_CT_WAN_CHILD_PREFIX)
	char new_prefix[45] = {0};
#endif
	int pd_index = 0;
	char domain_name[68] = {0};
	char dns_type[8] = {0};
	char dns_source[8] = {0};
	int ifidx = 0, dnsType = 0;

	node = mxmlFindElement(parant, parant, SUB_NODE_NAME,
		NULL, NULL, MXML_DESCEND);

	if(node != NULL){		


			tmp= (char *) mxmlElementGetAttr(node, DHCP6S_ENABLE_SIMBOL);
			if(tmp!=NULL){			
				strcpy(enable_flag, tmp);
			}
			
			tmp= (char *) mxmlElementGetAttr(node, DHCP6S_MODE);
			if(tmp!=NULL){			
				strcpy(mode, tmp);
			}			
			
			tmp = (char *) mxmlElementGetAttr(node, DHCP6S_DOMAIN);
			if( NULL != tmp )
				strcpy(domain_name, tmp);

		/*Manual mode*/	
		if(mode[0] == '1'){
			tmp= (char *) mxmlElementGetAttr(node, DNS1_SERVER);
			if(tmp!=NULL){			
				strcpy(dns1_v6, tmp);
			}
			tmp=(char *) mxmlElementGetAttr(node, DNS2_SERVER);
			if(tmp!=NULL){			
				strcpy(dns2_v6, tmp);
			}
			if( 
					( 0 != strcmp(dns1_v6, "") ) || 
					( 0 != strcmp(dns2_v6,"") )
			  )
			sprintf(buf, "option domain-name-servers %s  %s;\n", dns1_v6, dns2_v6);
		}
		else{
			tmp = (char *) mxmlElementGetAttr(node, DHCP6S_DNSTYPE);
			if( NULL != tmp )
				strcpy(dns_type, tmp);
			else
				strcpy(dns_type, "2");
			tmp = (char *) mxmlElementGetAttr(node, DHCP6S_DNSSOURCE);
			if( NULL != tmp )
				strcpy(dns_source, tmp);

			dnsType = atoi(dns_type);
			if ( 0 == dnsType )
			{
				ifidx = atoi(dns_source);
				memset(nodeName, 0, sizeof(nodeName));
			  	strcpy(nodeName[0], "WanInfo");
			  	sprintf(nodeName[1], "Entry%d", ifidx);
				if ( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "Status6", status6) )
					status6[0] = '\0';
				if ( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "DNS6", dns1_v6) )
					dns1_v6[0] = '\0';
				if ( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "SecDNS6", dns2_v6) )
					dns2_v6[0] = '\0';

				if ( 0 == strcmp(status6, "up") && '\0' != dns1_v6[0] )
					sprintf(buf, "option domain-name-servers %s  %s;\n", dns1_v6, dns2_v6);
				else
					sprintf(buf, "option domain-name-servers %s;\n", DHCP6S_DEFAULT_DNS);
			}
			else if ( 1 == dnsType )
			{
				tmp= (char *) mxmlElementGetAttr(node, DNS1_SERVER);
				if ( NULL != tmp )
					strcpy(dns1_v6, tmp);
				tmp=(char *) mxmlElementGetAttr(node, DNS2_SERVER);
				if ( NULL != tmp )
					strcpy(dns2_v6, tmp);
				if ( '\0' != dns1_v6[0] )
					sprintf(buf, "option domain-name-servers %s  %s;\n", dns1_v6, dns2_v6);
			}
			else
				sprintf(buf, "option domain-name-servers %s;\n", DHCP6S_DEFAULT_DNS);
		}

		if ( '\0' != domain_name[0] )
		{
			len = strlen(buf);
			sprintf(buf+len, "option domain-name \"%s\";\n", domain_name);
		}

		tcdbg_printf("dhcp6sCall_write:mode[0]=%c\n",mode[0]);
		if(mode[0] == '0'){ /*Dhcp6s auto DNS and prefix mode*/
			/*Get prefix delegation from Wan interface which is default route*/

			pd_index = get_run_pd_index();
			if ( pd_index >= 0 )
			{
				sprintf(pd_index_v6, "%d", pd_index);
				tcdbg_printf("dhcp6sCall_write:pd_index_v6=%s\n",pd_index_v6);
			
				memset(nodeName, 0, sizeof(nodeName));
			  	strcpy(nodeName[0],"WanInfo");
			  	sprintf(nodeName[1], "Entry%s", pd_index_v6);
				getAttrValue(top, nodeName, "Status6", status6);
				
				tcdbg_printf("dhcp6sCall_write:temp=%s\n",temp);
				getAttrValue(top, nodeName, "PD6", temp);	
				
				tcdbg_printf("dhcp6sCall_write2:temp=%s\n",temp);

				getAttrValue(top, nodeName, "PD6ptime", dhcpv6_wan_pd_ptime);
				getAttrValue(top, nodeName, "PD6vtime", dhcpv6_wan_pd_vtime);

#if defined(TCSUPPORT_CT_WAN_CHILD_PREFIX)
				/*generate prefix for lan DHCP*/
				if(lan_ipv6_prefix_gen(new_prefix, pd_index_v6) == 0){
					strcpy(temp, new_prefix);
				}
#endif
				
				if((0 != strcmp(temp, "")) && !strcmp(status6, "up")){
					/*Parse prefix and prefix-len from PD6,eg:3000:456:AEF:DCA::/64*/
					strtok(temp, "/");
					strcpy(ip_v6, temp);

					p = strtok(NULL, "/");
					if(!p)
						strncpy(prefix_v6len, p, sizeof(prefix_v6len));

					if ( '\0' != dhcpv6_wan_pd_ptime[0]
							&& '\0' != dhcpv6_wan_pd_vtime[0] )
					{
						strcpy(preffered_lifetime, dhcpv6_wan_pd_ptime);
						strcpy(valid_lifetime, dhcpv6_wan_pd_vtime);
					}
					else
					{
					strcpy(preffered_lifetime, DHCP6S_DEF_PREFER_LIFE);
					strcpy(valid_lifetime, DHCP6S_DEF_VALID_LIFE);			
				}
			}
		}
		}
		else if(mode[0] == '1'){ /*Dhcp6s manual mode*/
			tmp= (char *) mxmlElementGetAttr(node, PREFIX_SIMBOL);
			if(tmp!=NULL){			
				strcpy(ip_v6, tmp);
			}
			tmp=(char *) mxmlElementGetAttr(node, PREFIX_LEN_SIMBOL);
			if(tmp!=NULL){			
				strcpy(prefix_v6len, tmp);
			}
			tmp=(char *) mxmlElementGetAttr(node, PREFFERED_LIFETIME_SIMBOL);
			if(tmp!=NULL){			
				strcpy(preffered_lifetime, tmp);
			}

			tmp= (char *) mxmlElementGetAttr(node, VALID_LIFETIME_SIMBOL);
			if(tmp!=NULL){			
				strcpy(valid_lifetime, tmp);
			}
		}

		if(
				( 0 != strcmp(preffered_lifetime, "") ) &&	
				( 0 != strcmp(valid_lifetime, "") ) && 
				( 0 != strcmp(ip_v6, "") )
			)
		{
			/*setting the dhcp6s address pool range*/ 			
#if defined(TCSUPPORT_CT_WAN_CHILD_PREFIX)
			/*generate start address*/			
			if(lan_ipv6_addr_gen(ipv6_start, "MinAddress", ip_v6) != 0)
#endif
			{
			ipv6AddressTo01(ipv6_start, ip_v6);
			}
#if defined(TCSUPPORT_CT_WAN_CHILD_PREFIX)
			/*generate start address*/
			if(lan_ipv6_addr_gen(ipv6_end,"MaxAddress", ip_v6) != 0)
#endif
			{
			ipv6AddressToFF(ipv6_end, ip_v6);
			}
			
			len = strlen(buf);
			sprintf(buf+len,"interface br0 {\n\t address-pool pool1 %s %s;\n};\n",
			preffered_lifetime, valid_lifetime);

			len = strlen(buf);
			sprintf(buf+len, "\npool pool1 {\n\t range %s to %s;\n};\n",
			ipv6_start, ipv6_end);
		}
		else
		{
			len = strlen(buf);
			sprintf(buf+len,"interface br0 { };\n");
		}
	
		
		   if(write2file(buf,DHCP6S_CONFPATH) == FAIL)
		  {
		     printf("fail to write file /etc/dhcp6s.conf\n");
		     return FAIL;
	         }
	
	}
	else{
		unlink(DHCP6S_CONFPATH);
	}	
	chmod(DHCP6S_CONFPATH,777);
	return SUCCESS;
	
}

/*______________________________________________________________________________
**	radvdCall_verify
**
**	descriptions:
**		To check the system parameter of radvd call  element  is validly or not.
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
dhcp6sCall_verify(mxml_node_t *node){
	return SUCCESS;
}/* end radvdCall_verify */

/*_____________________________________________________________________________
**      function name: dhcp6sCall_execute
**      descriptions:
**            To execute dhcp6s relay node service.
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
**      1. rclv 20100517
**____________________________________________________________________________
*/
int
dhcp6sCall_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){

	
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char enable_value[8] = {0};
	char mode[5] = {0}, ipv6_br0[50] = {0}, cmd[80] = {0};

	memset(nodeName, 0, sizeof(nodeName));
	
	strcpy(nodeName[0], "Dhcp6s");
	strcpy(nodeName[1], "Entry");

	

	if(getAttrValue(top, nodeName, "Enable", enable_value) != TCAPI_PROCESS_OK){
		tcdbg_printf("error:Dhcp6s function Get enable value fail!\n");
		return -1;
	}

	if(getAttrValue(top, nodeName, BR0_ADDR, ipv6_br0) == TCAPI_PROCESS_OK && strcmp(ipv6_br0, "N/A")){
			sprintf(cmd, "ifconfig br0 del %s", ipv6_br0);
			system(cmd);
			setAttrValue(top, nodeName, BR0_ADDR, "N/A");
	}

	kill_process("/var/run/dhcp6s.pid");
	
	if (0 == strcmp(enable_value, "1")) 
	{
		if(getAttrValue(top, nodeName, DHCP6S_MODE, mode) != TCAPI_PROCESS_OK){
			tcdbg_printf("error:Dhcp6s function Get mode value fail!\n");
			return -1;
		}
		/*Manual mode*/
		if(mode[0] == '1'){			
			if(create_br0_pool_ipv6addr(top, nodeName, ipv6_br0) == 0){
				sprintf(cmd, "ifconfig br0 %s", ipv6_br0);
				system(cmd);
				/*save br0 address*/
				setAttrValue(top, nodeName, BR0_ADDR, ipv6_br0);
			}
			
		}

		system("/userfs/bin/dhcp6s -c /etc/dhcp6s.conf br0 -p /var/run/dhcp6s.pid &");
		tcdbg_printf("dhcp6s parameter activated by exec!\n");
		
	}

	else 
	{
		tcdbg_printf("dhcp6s call closed by exec!\n");
	}

	return SUCCESS;
}/* end dhcpRelay_execute */




#endif




//"DhcpRelay",	SINGLE_TYPE,	NULL,	NULL,NULL, dhcpRelay_write, dhcpRelay_verify, dhcpRelay_execute},
/*______________________________________________________________________________
**	dhcpRelay_boot
**
**	descriptions:
**		the boot function of dhcpRelay.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**_______________________________________________________________________________
*/
int
dhcpRelay_boot(mxml_node_t *top){
	FILE *startupSh=NULL;
	startupSh=fopen("/etc/config/dhcprelay.sh","r");
	if(startupSh){
		fclose(startupSh);
		system("chmod 755 /etc/config/dhcprelay.sh");
		system("/etc/config/dhcprelay.sh");
	}
	return SUCCESS;
}
/*______________________________________________________________________________
**	dhcpRelay_init
**
**	descriptions:
**		the init function of dhcpRelay.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
dhcpRelay_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="DhcpRelay",
			.type=SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=dhcpRelay_write,
			.cfg_verify=dhcpRelay_verify,
			.cfg_execute=dhcpRelay_execute,
			.cfg_boot=dhcpRelay_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}
/*______________________________________________________________________________
**	dhcpRelay_write
**
**	descriptions:
**		Find dhcpRelay element from mxml tree then write dhcpRelay's system parameter to specify
**		configuration files.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  dhcpRelay cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
dhcpRelay_write(mxml_node_t *top, mxml_node_t *parant){

	mxml_node_t *node = NULL;
	char buf[128]={0};
	char *tmp = NULL;
	/*
	node = mxmlFindElement(tree, tree, DHCPRELAY_ENTRY,
	  	NULL, NULL,
	  	MXML_DESCEND);
	*/
	node = mxmlFindElement(parant, parant, SUB_NODE_NAME,
		NULL, NULL, MXML_DESCEND);

	if(node != NULL){
		/*/userfs/bin/dhcrelay -pf /var/log/dhcrelay.pid 10.10.10.100 */
		tmp = mxmlElementGetAttr(node, "Server");
		if(NULL == tmp)
			return FAIL;
		sprintf(buf, DHCPRELAY_CMD,tmp);

		//sprintf(buf, DHCPRELAY_CMD, node->value.element.attrs->value);

		if(write2file(buf,DHCPRELAY_PATH) == FAIL){
			return FAIL;
		}
	}else{
		unlink(DHCPRELAY_PATH);
	}
	chmod(DHCPRELAY_PATH,777);
	return SUCCESS;
}/* end dhcpRelay_write */

/*______________________________________________________________________________
**	dhcpRelay_verify
**
**	descriptions:
**		To check the system parameter of dhcpRelay element  is validly or not.
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
dhcpRelay_verify(mxml_node_t *node){
	return SUCCESS;
}/* end dhcpRelay_verify */

/*_____________________________________________________________________________
**      function name: dhcpRelay_execute
**      descriptions:
**            To execute dhcp relay node service.
**
**      parameters:
**            node:   Put the dhcpRelay of cfg node tree that is used to execute dhcpRelay settings.
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
**      1. shnwind 2008/5/19
**____________________________________________________________________________
*/
int
dhcpRelay_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){

	char buf[64], nodeName[MAX_ARG_NUM][MAX_NODE_NAME];

	kill_process("/var/log/dhcrelay.pid");
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], LAN);
	strcpy(nodeName[1],LAN_DHCP);

	if(getAttrValue(top,nodeName, LAN_DHCP_TYPE, buf) != TCAPI_PROCESS_OK){
		return FAIL;
	}

	if(!strcmp(buf,"2")){/*2:dhcprelay*/
	#if 0	
		system("killall -9 dproxy");
	#endif	
		kill_process("/var/run/udhcpd.pid");
		unlink(DHCPD_PATH);
		unlink(DPROXYAUTO_PATH);
		system(DHCPRELAY_PATH);
	}else{
		unlink(DHCPRELAY_PATH);
	}

	return SUCCESS;
}/* end dhcpRelay_execute */

#if defined(TCSUPPORT_RIPD)
/*______________________________________________________________________________
**	create_rip_conf
**
**	descriptions:
**		To collect all parameters of ripd then to create a zebra.conf and ripd.conf files.
**
**	parameters:
**		tree: 		Specify the root of mxml_node_t structure.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
create_rip_conf(mxml_node_t *tree){
	FILE *fp=NULL;
	char buf[MAXSIZE];
	//char element_name[8];
	int i=0;
	int if_count=0;
	mxml_node_t *lanRip_node = NULL;
	//mxml_node_t *wanRip_node = NULL;
	const char* rip_version=NULL;
	const char* rip_direction=NULL;
	//const char* wanRip_stat=NULL;
	int deny = 0;
	char if_info[MAX_WAN_IF_INDEX+1][3][16];/*{"interface","rip_version","direction"}*/
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	mxml_node_t *curNode = NULL;
	char active[5] = {0}, interface[16] = {0}, ripversion[16] = {0}, direction[16] = {0};

	//char zebra_default_parm[][2][ATTR_SIZE]=
	//{
	//	{"hostname","TCZebra"},
	//	{"password","1234"},
	//	{"enable password","1234"},
	//	{"",""},
	//};

	//char ripd_default_parm[][2][ATTR_SIZE]=
	//{
	//	{"hostname","TCRipd"},
	//	{"password","1234"},
	//	{"",""},
	//};
	memset(if_info,0,sizeof(if_info));
	/*Find out the parant of LanRipd cfg node*/
	lanRip_node = mxmlFindElement(tree, tree, LANRIPD,
		NULL, NULL,MXML_DESCEND);
	if(lanRip_node != NULL){
		/*Get the child of  LanRipd cfg node*/
		lanRip_node = mxmlFindElement(lanRip_node, lanRip_node, RIPD_LAN_CONF_NODE,
			NULL, NULL,MXML_DESCEND);
		if(lanRip_node){
			rip_version = mxmlElementGetAttr(lanRip_node,RIPD_VERSION);
			rip_direction= mxmlElementGetAttr(lanRip_node,RIPD_DIRECTION);
			/*If the value of rip_direction is equal with "None" , we don't add br0 interface.*/
			if(rip_version!=NULL && rip_direction!=NULL){
			        if(strcmp(rip_direction, RIPD_DIRECTION_NONE) != 0){
				        strcpy(if_info[if_count][RIP_INTERFACE], RIPD_DEFAULT_LANIF);
				        strcpy(if_info[if_count][RIP_VERSION], rip_version);
				        strcpy(if_info[if_count][RIP_DIRECTION], rip_direction);
				        if_count++;
			        }
                        }
		}
	}

	memset(nodeName, 0, sizeof(nodeName));
  	strcpy(nodeName[0],LANRIPD); 

	for(i = 0; i < MAX_WAN_IF_INDEX; i++){
		sprintf(nodeName[1], "Entry%d", i);
		
		/*Check entry is existed or not*/
		if((curNode = search_node(nodeName)) == NULL)
			continue;

		memset(interface, 0, sizeof(interface));
		memset(ripversion, 0, sizeof(ripversion));
		memset(direction, 0, sizeof(direction));

		if(getAttrValue(tree, nodeName, RIPD_INTERFACE, interface) != TCAPI_PROCESS_OK ){
			continue;
		}
		if(get_waninfo_by_name(interface, "Active", active) == SUCCESS){
			/*if this interface is not activated, jump to next rule*/		
			if(strcmp(active, ACTIVE))
				continue;
		}
		else{
			continue;
		}

		if(getAttrValue(tree, nodeName, RIPD_DIRECTION, direction) != TCAPI_PROCESS_OK )
			continue;
		
		if(strcmp(direction, RIPD_DIRECTION_NONE) == 0){
			continue;
		}
		if(getAttrValue(tree, nodeName, RIPD_VERSION, ripversion) != TCAPI_PROCESS_OK )
			continue;

		strcpy(if_info[if_count][RIP_INTERFACE],interface);
		strcpy(if_info[if_count][RIP_DIRECTION],direction);
		strcpy(if_info[if_count][RIP_VERSION],ripversion);
		if_count++;
		
	}

#if 0
	for(i = 0; i < PVC_NUM; i++){
		memset(buf,0,sizeof(buf));
		/*Find out the parant of Wan cfg node*/
		wanRip_node=mxmlFindElement(tree, tree, WAN,
			NULL, NULL,MXML_DESCEND);
		if(wanRip_node){
			sprintf(element_name,SUB_PVC_NAME,i);
			/*Get the child of  Wan cfg node*/
			wanRip_node=mxmlFindElement(wanRip_node, wanRip_node, element_name,
				NULL, NULL,MXML_DESCEND);
			if(wanRip_node!=NULL && wanRip_node->value.element.num_attrs > 0){
				wanRip_stat=mxmlElementGetAttr(wanRip_node,NODE_ACTIVE);
				if(wanRip_stat != NULL){
					if(strcmp(wanRip_stat, WAN_DEACTIVE) == 0){
						continue;
					}
				}
				rip_direction=mxmlElementGetAttr(wanRip_node, RIPD_DIRECTION);
				if(rip_direction!=NULL){
					if(strcmp(rip_direction, RIPD_DIRECTION_NONE) == 0){
						continue;
					}
				}
				else{
					continue;
				}

				switch(atoi(mxmlElementGetAttr(wanRip_node, WAN_ISP))){
					case WAN_ENCAP_DYN:
					case WAN_ENCAP_STATIC:
						sprintf(buf,NAS_IF,i);
						break;
					case WAN_ENCAP_PPP:
						sprintf(buf,PPP_IF,i);
						break;
					case WAN_ENCAP_BRIDGE:
					default:
						continue;
				}
				rip_version= mxmlElementGetAttr(wanRip_node,RIPD_VERSION);
				strcpy(if_info[if_count][RIP_INTERFACE],buf);
				if(rip_version!=NULL){
				strcpy(if_info[if_count][RIP_VERSION],rip_version);
				}
				strcpy(if_info[if_count][RIP_DIRECTION],rip_direction);
				if_count++;
			}
		}
	}

#endif
	if(if_count > 0){
		/*create zebra.conf file*/
		fp=fopen(ZEBRA_PATH,"w");
		if(fp == NULL){
			return FAIL;
		}
		else{
			/*Write default parameters to zebra.conf*/
#ifdef TC_ZEBRA_FULL			
			for(i = 0; strlen(zebra_default_parm[i][ATTR_INDEX]) != 0; i++){
				sprintf(buf,"%s %s\n", zebra_default_parm[i][ATTR_INDEX], zebra_default_parm[i][VALUE_INDEX]);
				fputs(buf,fp);
			}

#endif
			for(i = 0;i < if_count; i++){
				sprintf(buf,"%s %s\n", ZEBRA_IF,if_info[i][RIP_INTERFACE]);
				sprintf(buf,"%s%s\n", buf, RIPD_MULTICAST);
				fputs(buf,fp);
			}
		}
		fclose(fp);

		/*create ripd.conf file*/
		fp=fopen(RIPD_PATH,"w");
		if(fp == NULL){
			return FAIL;
		}
		else{
			/*Write default parameters to rip.conf*/
#ifdef TC_ZEBRA_FULL			
			for(i = 0; strlen(ripd_default_parm[i][ATTR_INDEX]) != 0; i++){
				sprintf(buf,"%s %s\n", ripd_default_parm[i][ATTR_INDEX], ripd_default_parm[i][VALUE_INDEX]);
				fputs(buf,fp);
			}

#endif
			for(i = 0;i < if_count; i++){
				sprintf(buf,"%s %s\n", ZEBRA_IF,if_info[i][RIP_INTERFACE]);
				fputs(buf,fp);
				/*pork modified at 20090113*/
				if(strcmp(if_info[i][RIP_VERSION], RIPD_VER1)==0){
					sprintf(buf,"ip rip send version %d\nip rip receive version %d\n", RIPD_V1, RIPD_V1);
				}else{
					sprintf(buf,"ip rip send version %d\nip rip receive version %d %d\n", RIPD_V2,RIPD_V1,RIPD_V2);
				}
				/*pork modified at 20090113*/
				fputs(buf,fp);
#ifdef TC_ZEBRA_FULL				
				fputs(RIPD_NO_AUTH_MODE, fp);
#endif				
			}

			fputs(RIPD_ROUTE_CMD, fp);
#ifdef TC_ZEBRA_FULL			
			fputs(RIPD_REDISTRIBUTE_CMD, fp);
#endif			
			for(i = 0; i < if_count; i++){
				sprintf(buf,"%s %s\n", RIPD_NETWORK, if_info[i][RIP_INTERFACE]);
				fputs(buf, fp);
			}
			for(i = 0; i < if_count; i++){
				memset(buf,0,sizeof(buf));
				if(strcmp(if_info[i][RIP_DIRECTION], RIPD_IN_ONLY_DIRECTION) == 0){
					sprintf(buf, RIPD_DENY_LIST, RIPD_OUT_DIRECTION, if_info[i][RIP_INTERFACE]);
					deny = RIPD_IS_DENY;
				}
				else if(strcmp(if_info[i][RIP_DIRECTION], RIPD_OUT_ONLY_DIRECTION) == 0){
					sprintf(buf, RIPD_DENY_LIST, RIPD_IN_DIRECTION, if_info[i][RIP_INTERFACE]);
					deny = RIPD_IS_DENY;
				}
				else{
					continue;
				}
				fputs(buf, fp);
			}

			if(deny == RIPD_IS_DENY){
				fputs(RIPD_ACCESS_DENY_CMD, fp);
			}
		}
		fclose(fp);
	}else{
		/*None RIP interface ,so we need to delete zebra & rip configuration file.*/
		unlink(ZEBRA_PATH);
		unlink(RIPD_PATH);
	}
	return SUCCESS;
}/* end create_rip_conf */
#endif

/*______________________________________________________________________________
**	function name:write_wlan_script
**
**	description:
*     create wireless configure file
**	parameters:
*     top:xml top node address
**	global:
*     none
**	return:
*       0:success
*	  1:fail
**	call:
*     none
**	revision:
*     1.shnwind 20080530
**____________________________________________________________________________*/
int
write_wlan_config(mxml_node_t *top, int BssidNum){
  	int i, j;
  	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], tmp[160], tmp2[160],buf[160],strRate[32] = {0},strWirelessMode[8] = {0};
  	FILE *fp;
	char wirelessmode[16], encrytype[16];
  	char wlan_common_default_parm[][2][ATTR_SIZE]=
	{
		//{"AutoChannelSelect","0"},
#if !defined(TCSUPPORT_CT_WLAN_NODE)
		{"TxPower","100"},
#endif
		{"BGProtection","0"},
		{"TxPreamble","0"},
		{"TxBurst","1"},
		{"PktAggregate","0"},
		{"TurboRate","0"},
		{"NoForwardingBTNBSSID","0"},
		{"ShortSlot","1"},
		{"RekeyInterval","0"},
		{"RekeyMethod","DISABLE"},
		{"PMKCachePeriod","10"},
		{"APAifsn","3;7;1;1"},
		{"APCwmin","4;4;3;2"},
		{"APCwmax","6;10;4;3"},
		{"APTxop","0;0;94;47"},
		{"APACM","0;0;0;0"},
		{"BSSAifsn","3;7;2;2"},
		{"BSSCwmin","4;4;3;2"},
		{"BSSCwmax","10;10;4;3"},
		{"BSSTxop","0;0;94;47"},
		{"BSSACM","0;0;0;0"},
		{"AckPolicy","0;0;0;0"},
	#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7592) //add by xyyou 
		{"APSDCapable","0"},
		{"DLSCapable","0"},
		{"WirelessEvent","0"},
		{"HT_HTC","0"},
		{"HT_LinkAdapt","0"},
		{"HT_AMSDU","0"},
		{"HT_STBC","1"},	
		{"ShortSlot","1"},
		{"DisableOLBC","0"},
		{"HT_AutoBA","1"},
		{"HT_RDG","1"},//xyyou???
		//{"HT_OpMode","0"},//xyyou???
		{"HT_MpduDensity","5"},
		{"HT_BAWinSize","64"},
		{"HT_BSSCoexistence","1"},
	#endif//end xyyou
        #if defined(MT7601E)|| defined(MT7592) 
		{"PMFMFPC","0","0"},
		{"PMFMFPR","0","0"},
		{"PMFSHA256","0","0"},
	#endif
	#if defined(RT5392) && !defined(WLAN_MULTIDRIVER)
		{"TempComp","2"},
	#endif
#if defined(WLAN_MULTIDRIVER) && defined(RT5392)
		{"ackwait","20"},
		{"acklen","19"},
		{"enackq","1"},
#endif
#if defined(RT3092) || defined(RT5392)|| defined(MT7592) 
		{"HT_TxStream","2","1"},
		{"HT_RxStream","2","1"},
#else
		{"HT_TxStream","1","1"},
		{"HT_RxStream","1","1"},
#endif
		{"IEEE80211H","0","1"},
#if	defined(MT7592)
		{"PS_RETRIEVE","1","1"},
#endif
		{"",""},
	};
  	char wlan_entry_default_parm[][2][ATTR_SIZE]=
	{
		{"TxRate","0"},
		{"NoForwarding","0"},
		{"PreAuth","0"},
	#if defined(RT3390) || defined(RT3090) || defined(RT5392) //add by xyyou 
		{"EAPifname","br0"},
		{"PreAuthifname","br0"},
	#endif//end xyyou
	#if 0//def WSC_AP_SUPPORT//add by xyyou
		{"WscDefaultSSID1","Trendchip"},
		//{"WscConfMethods","0x0084"}
	#endif
		{"",""},
	};
  	char wlan_script_com_attr[][ATTR_SIZE]=
	{
		{"CountryRegion"},
		{"AutoChannelSelect"},
		{"Channel"},
		/*{"BssidNum"},*/
		{"BssidNum"},
		{"BeaconPeriod"},
		{"RTSThreshold"},
		{"FragThreshold"},
		{"DtimPeriod"},
		//{"WirelessMode"},
		{"BasicRate"},
	#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7592) //add by xyyou
		{"HT_GI"},
		{"HT_BW"},
		{"HT_EXTCHA"},
		{"HT_OpMode"},
		#ifdef TCSUPPORT_MTD_ENCHANCEMENT
		{"EfuseBufferMode"},
		#endif	
	#ifdef IGMP_SNOOP_SUPPORT//add by xyyou
	#if defined(RT5392) || defined(WLAN_MULTIDRIVER) || defined(MT7592) 
		{"IgmpSnEnable"},
	#endif
	#endif
	#endif //end xyyou
		{"AutoChannelSkipList"},
		{""}
	};
#if 0
	#if defined(RT3390) || defined(RT3090) //add by xyyou
	#ifdef WSC_AP_SUPPORT//add by xyyou
	enum wlan_script_entry_attr_index{
		ssid=0, hidessid, authmod, encyptype, defaultkeyid, Key1Type, key1str,
			Key2Type, key2str, Key3Type, key3str, Key4Type, key4str, wpapsk,wmm,mcs,wsconfmode,wscconfstatus,wsckeyascii
	};
	#else
	enum wlan_script_entry_attr_index{
		ssid=0, hidessid, authmod, encyptype, defaultkeyid, Key1Type, key1str,
			Key2Type, key2str, Key3Type, key3str, Key4Type, key4str, wpapsk,wmm,mcs
	};
	#endif
	#else
	#ifdef WSC_AP_SUPPORT//add by xyyou
	enum wlan_script_entry_attr_index{
		ssid=0, hidessid, authmod, encyptype, defaultkeyid, Key1Type, key1str,
			Key2Type, key2str, Key3Type, key3str, Key4Type, key4str, wpapsk, wmm,wsconfmode,wscconfstatus,wsckeyascii
	};
	#else
	enum wlan_script_entry_attr_index{
		ssid=0, hidessid, authmod, encyptype, defaultkeyid, Key1Type, key1str,
			Key2Type, key2str, Key3Type, key3str, Key4Type, key4str, wpapsk, wmm
	};
	#endif
	#endif//end xyyou
#endif
	enum wlan_script_entry_attr_index{
		ssid=0,
		hidessid,
		authmod,
		encyptype,
		defaultkeyid,
		Key1Type,
		key1str,
		Key2Type, 
		key2str, 
		Key3Type, 
		key3str, 
		Key4Type, 
		key4str, 
		wpapsk,
		wmm,
#if defined(TCSUPPORT_WLAN_MAXSTANUM_GUI)
		MaxStaNum,
#endif
		#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7601E) || defined(MT7592)  //add by xyyou
		mcs,
		fixedTxMode,
		#endif
		#ifdef WSC_AP_SUPPORT//add by xyyou
		wsconfmode,
		wscconfstatus,
		wsckeyascii,
		wscv2support, //added by fredli
		WscMaxPinAttack,
		WscSetupLockTime,
		#endif
		#ifdef IGMP_SNOOP_SUPPORT//add by xyyou
		#if !defined(RT5392) && !defined(MT7601E) && !defined(MT7592) 
		igmpsnooping,
		#endif
		#endif
#if defined(TCSUPPORT_CT_WLAN_WAPI)
		wapiIfName,
		wapiPsk1,
		wapiPskType,
		wapiAsCertPath,
		wapiUserCertPath,
		wapiAsIpAddr,
		wapiAsPort,
#endif
		
		wlan_entry_attr_len

	};

	char wlan_script_entry_attr_buf[wlan_entry_attr_len][270];

	char wlan_script_entry_attr_para[wlan_entry_attr_len][ATTR_SIZE]=
	{
		{"SSID"},
		{"HideSSID"},
		{"AuthMode"},
		{"EncrypType"},
		{"DefaultKeyID"},
		{"Key1Type"},
		{"Key1Str"},
		{"Key2Type"},
		{"Key2Str"},
		{"Key3Type"},
		{"Key3Str"},
		{"Key4Type"},
		{"Key4Str"},
		{"WPAPSK"},
		{"WmmCapable"},
#if defined(TCSUPPORT_WLAN_MAXSTANUM_GUI)
		{"MaxStaNum"},
#endif
	#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7601E) || defined(MT7592)  //add by xyyou
		{"HT_MCS"},	
		{"FixedTxMode"},
	#endif //end xyyou
	#ifdef WSC_AP_SUPPORT//add by xyyou
		{"WscConfMode"},	
		{"WscConfStatus"},	
		{"WscKeyASCII"},
		{"WscV2Support"}, //added by fredli
		{"WscMaxPinAttack"},
		{"WscSetupLockTime"},		
	#endif
	#ifdef IGMP_SNOOP_SUPPORT//add by xyyou
	#if (defined(RT3390) || defined(RT3090)) && !defined(WLAN_MULTIDRIVER)//just for 3390&3090, 5392 used as common attribute
		{"IgmpSnEnable"},
	#endif
	#endif
#if defined(TCSUPPORT_CT_WLAN_WAPI)
		{"WapiIfName"},
		{"WapiPsk1"},
		{"WapiPskType"},
		{"WapiAsCertPath"},
		{"WapiUserCertPath"},
		{"WapiAsIpAddr"},
		{"WapiAsPort"},
#endif
	
	};

	fp = fopen(WLAN_PATH,"w");

	if(fp == NULL){
		return FAIL;
	}
	/*write common default setting to RT61.dat*/
        memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],WLAN);
	strcpy(nodeName[1],"Common");
	fputs("Default\n",fp);
	for(i = 0;strlen(wlan_common_default_parm[i][0]) != 0;i++){
		/*
		  *  write cfg node value to RT61.dat, instead of default value. Thus the value in cfg node and RT61.dat can be 
		  *  synchronized
		 */	
		if (getAttrValue(top, nodeName, wlan_common_default_parm[i][0], tmp) == TCAPI_PROCESS_OK){
			if (strcmp(wlan_common_default_parm[i][0], "TxBurst") == 0){
				sprintf(buf,"%s=%d\n", wlan_common_default_parm[i][0], txBurst_or_not(top, BssidNum));
			}
			else{
				sprintf(buf, "%s=%s\n", wlan_common_default_parm[i][0], tmp);
			}
		}
		else{
			if (strcmp(wlan_common_default_parm[i][0], "TxBurst") == 0){
				sprintf(buf,"%s=%d\n", wlan_common_default_parm[i][0], txBurst_or_not(top, BssidNum));
			}
			else{
				sprintf(buf, "%s=%s\n", wlan_common_default_parm[i][0], wlan_common_default_parm[i][1]);
			}
		}
		fputs(buf,fp);
	}
	/*write common setting to RT61.dat */
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],WLAN);
	strcpy(nodeName[1],"Common");
	if(getAttrValue(top, nodeName, "WirelessMode", strWirelessMode) == TCAPI_PROCESS_OK){
		strncpy(wirelessmode, tmp, sizeof(wirelessmode));
		sprintf(buf,"%s=%s\n", "WirelessMode", strWirelessMode);
		fputs(buf,fp);
	#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7601E) || defined(MT7592)  //add by xyyou
		if((atoi(strWirelessMode)==PHY_MODE_N_ONLY)||(atoi(strWirelessMode)==PHY_MODE_GN_MIXED)||(atoi(strWirelessMode)==PHY_MODE_BGN_MIXED)){
			setAttrValue(top, nodeName, "11nMode", "1");
		}else{
			setAttrValue(top, nodeName, "11nMode", "0");
		}
	#endif
	}
	
	for(i = 0;strlen(wlan_script_com_attr[i]) != 0;i++){
		if(strcmp(wlan_script_com_attr[i], "AutoChannelSelect") == 0){
			if(getAttrValue(top, nodeName, "Channel", tmp) == TCAPI_PROCESS_OK){
				if(atoi(tmp)==0){
					#if defined(MT7592)  
					sprintf(buf,"%s=%s\n", wlan_script_com_attr[i],"3");
					#else
					sprintf(buf,"%s=%s\n", wlan_script_com_attr[i],"2");
					#endif
				}else{
					sprintf(buf,"%s=%s\n", wlan_script_com_attr[i],"0");
				}
			}
		}
		if(strcmp(wlan_script_com_attr[i], "AutoChannelSkipList") == 0){
			if(getAttrValue(top, nodeName, "AutoChannelSkipList", tmp) == TCAPI_PROCESS_OK){				
					sprintf(buf,"%s=%s\n", wlan_script_com_attr[i],tmp);
			}
		}
		if(strcmp(wlan_script_com_attr[i], "BasicRate") == 0){	
			#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7601E) || defined(MT7592)  
			switch(atoi(strWirelessMode)){
				case PHY_MODE_BG_MIXED:
				case PHY_MODE_BGN_MIXED:
				case PHY_MODE_N_ONLY:
					sprintf(buf,"%s=%s\n","BasicRate", "15");
					break;
				case PHY_MODE_B_ONLY:
					sprintf(buf,"%s=%s\n","BasicRate", "3");
					break;
				case PHY_MODE_G_ONLY:
				case PHY_MODE_GN_MIXED:
					sprintf(buf,"%s=%s\n","BasicRate", "351");
					break;
				default:
					tcdbg_printf("write_wlan_config:WLan wireless mode is not right when set BasicRate!(3390,3090)\n");
					fclose(fp);
					return FAIL;

			}
			#else
			switch(atoi(strWirelessMode)){
				case PHY_MODE_BG_MIXED:
					sprintf(buf,"%s=%s\n","BasicRate", "15");
					break;
				case PHY_MODE_B_ONLY:
					sprintf(buf,"%s=%s\n","BasicRate", "3");
					break;
				case PHY_MODE_G_ONLY:
					sprintf(buf,"%s=%s\n","BasicRate", "351");
					break;
				default:
					tcdbg_printf("write_wlan_config:WLan wireless mode is not right when set BasicRate!\n");
					fclose(fp);
					return FAIL;

			}
			#endif
		}
#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7601E) || defined(MT7592)  
		else if(strcmp(wlan_script_com_attr[i], "HT_BW") == 0){
			if(Is11nWirelessMode(top)){
				if(getAttrValue(top, nodeName, (char *)wlan_script_com_attr[i], tmp) == TCAPI_PROCESS_OK){
					sprintf(buf,"%s=%s\n", wlan_script_com_attr[i],tmp);
				}
			}else{
				sprintf(buf,"%s=%s\n", wlan_script_com_attr[i],"0");
			}
			
		}
		else if(strcmp(wlan_script_com_attr[i], "HT_EXTCHA") == 0){
			sprintf(buf,"%s=%s\n", wlan_script_com_attr[i],"0");
			if(Is11nWirelessMode(top)){
				if(getAttrValue(top, nodeName, "HT_BW", tmp) == TCAPI_PROCESS_OK)
				{
					if(atoi(tmp) == 1)
					{
						if(getAttrValue(top, nodeName, "Channel", tmp) == TCAPI_PROCESS_OK){
							if(atoi(tmp) <= 4){
								sprintf(buf,"%s=%s\n", wlan_script_com_attr[i],"1");
							}
						}
					}
				}
			}
		}
		else if(strcmp(wlan_script_com_attr[i], "HT_OpMode") == 0){		
			switch(atoi(strWirelessMode)){
					case PHY_MODE_BGN_MIXED:
					case PHY_MODE_GN_MIXED:
					case PHY_MODE_BG_MIXED:
					case PHY_MODE_B_ONLY:
						sprintf(buf,"%s=%s\n", wlan_script_com_attr[i],"0");
						break;
					case PHY_MODE_N_ONLY:
					case PHY_MODE_G_ONLY:
						sprintf(buf,"%s=%s\n", wlan_script_com_attr[i],"1");
						break;
					default:
						tcdbg_printf("write_wlan_config:WLan wireless mode is not right when set HT_OpMode!\n");
						fclose(fp);
						return FAIL;
			}
		}
#endif		
		else{
			if(getAttrValue(top, nodeName, (char *)wlan_script_com_attr[i], tmp) == TCAPI_PROCESS_OK){
				if(strlen(tmp) > 0){
					sprintf(buf,"%s=%s\n", wlan_script_com_attr[i],tmp);
				}
			}
		}
		fputs(buf,fp);
	 }
#if defined(TCSUPPORT_CT_WLAN_NODE)
	if(getAttrValue(top, nodeName, "TxPowerLevel", tmp) == TCAPI_PROCESS_OK){
		switch(atoi(tmp)){
		case TX_POWER_LEVLE_1:
			sprintf(buf,"%s=%s\n", "TxPower", "100");
			fputs(buf,fp);
			break;
		case TX_POWER_LEVLE_2:
			sprintf(buf,"%s=%s\n", "TxPower", "75");
			fputs(buf,fp);
			break;
		case TX_POWER_LEVLE_3:
			sprintf(buf,"%s=%s\n", "TxPower", "50");
			fputs(buf,fp);
			break;
		case TX_POWER_LEVLE_4:
			sprintf(buf,"%s=%s\n", "TxPower", "25");
			fputs(buf,fp);
			break;
		case TX_POWER_LEVLE_5:
			sprintf(buf,"%s=%s\n", "TxPower", "15");
			fputs(buf,fp);
			break;
		default:
			tcdbg_printf("write_wlan_config:the value of TxPowerLevel is not right!\n");
			fclose(fp);
			return FAIL;
		}
	}
#endif
	/*write entry default setting to RT61.dat, ex: SSID=SSID-A;SSID-B;SSID-C;SSID-D*/
	for(i = 0;strlen(wlan_entry_default_parm[i][0]) != 0;i++){
		sprintf(buf,"%s=",wlan_entry_default_parm[i][0]);
		for(j=0; j < BssidNum; j++){
 			if(j == BssidNum-1){/*last entry do not need ";"*/
				strcat(buf,wlan_entry_default_parm[i][1]);
			}else{
				sprintf(tmp,"%s;",wlan_entry_default_parm[i][1]);
				strcat(buf,tmp);
			}
		}
		strcat(buf,"\n");
		fputs(buf,fp);
	}
	/*write entry setting to RT61.dat*/
	memset(wlan_script_entry_attr_buf, 0,sizeof(wlan_script_entry_attr_buf));
	/*SSID=SSID-A;SSID-B;SSID-C;SSID-D*/
	for(j=0; j < BssidNum; j++){
		sprintf(nodeName[1],"%s%d",SUB_NODE_NAME,j);
		if(getAttrValue(top, nodeName, "SSID", tmp) == TCAPI_PROCESS_OK){
			strcat(wlan_script_entry_attr_buf[ssid],tmp);
			strcat(wlan_script_entry_attr_buf[ssid],";");
		}
		if(getAttrValue(top, nodeName, "HideSSID", tmp) == TCAPI_PROCESS_OK){
			strcat(wlan_script_entry_attr_buf[hidessid],tmp);
			strcat(wlan_script_entry_attr_buf[hidessid],";");
		}
		if(getAttrValue(top, nodeName, "AuthMode", tmp) == TCAPI_PROCESS_OK){
			if(strstr(tmp, "WEP")){/*WEP64-bits or WEP128-bits*/
#if defined(TCSUPPORT_CT_WLAN_NODE)
				//added by xmdai_nj: seperator WEP Encryption type and Authentication type
				if(getAttrValue(top, nodeName, "WEPAuthType", tmp) == TCAPI_PROCESS_OK){
					if(!strcmp(tmp, "OpenSystem")){
						strcat(wlan_script_entry_attr_buf[authmod],"OPEN;");
					}else if(!strcmp(tmp, "SharedKey")){
						strcat(wlan_script_entry_attr_buf[authmod],"SHARED;");
					}else{
				strcat(wlan_script_entry_attr_buf[authmod],"WEPAUTO;");
					}
				}
#else
				strcat(wlan_script_entry_attr_buf[authmod],"WEPAUTO;");
#endif
				strcat(wlan_script_entry_attr_buf[encyptype],"WEP;");
				if(getAttrValue(top, nodeName, "DefaultKeyID", tmp) != TCAPI_PROCESS_OK){
					strcpy(tmp,"");
				}
				strcat(wlan_script_entry_attr_buf[defaultkeyid],tmp);
				if ( strlen(tmp) == 0 ) {
					strcat(wlan_script_entry_attr_buf[defaultkeyid],"0;");
				}
				else {
					strcat(wlan_script_entry_attr_buf[defaultkeyid],";");
				}
				for(i=1;i<=4;i++){/*4 key string*/
					sprintf(tmp2,"Key%dStr",i);
					if(getAttrValue(top, nodeName, tmp2, tmp) != TCAPI_PROCESS_OK){
						strcpy(tmp,"");
					}
					/*krammer change for bug 1352*/
					#if 0
					if((tmp[0] == '0')&&(tmp[1] == 'x')){/*hex number*/
						strcat(wlan_script_entry_attr_buf[defaultkeyid +i*2-1],"0");
						strcat(wlan_script_entry_attr_buf[defaultkeyid +i*2],tmp+2);
					}else{
						strcat(wlan_script_entry_attr_buf[defaultkeyid +i*2-1],"1");
						strcat(wlan_script_entry_attr_buf[defaultkeyid +i*2],tmp);
					}
					#endif
					if(strlen(tmp)==10 || strlen(tmp)==26||strlen(tmp)==0){
						strcat(wlan_script_entry_attr_buf[defaultkeyid +i*2-1],"0;");
					}
					else{
						strcat(wlan_script_entry_attr_buf[defaultkeyid +i*2-1],"1;");
					}
					strcat(wlan_script_entry_attr_buf[defaultkeyid +i*2],tmp);
					
					if ( strlen(tmp) == 0 ) {
						strcat(wlan_script_entry_attr_buf[defaultkeyid +i*2],"0;");
					}
					else {
						strcat(wlan_script_entry_attr_buf[defaultkeyid +i*2],";");
					}
					#if 0
					strcat(wlan_script_entry_attr_buf[defaultkeyid +i],";");
					#endif
				}
				strcat(wlan_script_entry_attr_buf[wpapsk],"0;");
			}
			else if(!strcmp(tmp, "WPAPSK")||!strcmp(tmp, "WPA2PSK")||!strcmp(tmp, "WPAPSKWPA2PSK")){/*WPAPSK or WPA2PSK or Mixed mode*/
				strcat(wlan_script_entry_attr_buf[authmod],tmp);
				strcat(wlan_script_entry_attr_buf[authmod],";");
				if(getAttrValue(top, nodeName, "EncrypType", tmp) == TCAPI_PROCESS_OK){
					strcat(wlan_script_entry_attr_buf[encyptype],tmp);
					strcat(wlan_script_entry_attr_buf[encyptype],";");
				}
				strcat(wlan_script_entry_attr_buf[defaultkeyid],"2;");
				if(getAttrValue(top, nodeName, "WPAPSK", tmp) != TCAPI_PROCESS_OK){
					strcpy(tmp,"");
				}
				strcat(wlan_script_entry_attr_buf[wpapsk],tmp);
				/*krammer change for bug 1242*/
				strcat(wlan_script_entry_attr_buf[wpapsk],";");
				
				/*add by xyzhu_nj_20100211*/
				strcat(wlan_script_entry_attr_buf[Key1Type],"0;");
				strcat(wlan_script_entry_attr_buf[Key2Type],"0;");
				strcat(wlan_script_entry_attr_buf[Key3Type],"0;");
				strcat(wlan_script_entry_attr_buf[Key4Type],"0;");
				
				strcat(wlan_script_entry_attr_buf[key1str],"0;");
				strcat(wlan_script_entry_attr_buf[key2str],"0;");
				strcat(wlan_script_entry_attr_buf[key3str],"0;");
				strcat(wlan_script_entry_attr_buf[key4str],"0;");
			}
#if defined(TCSUPPORT_CT_WLAN_WAPI)
			else if((!strcmp(tmp, "WAICERT")) || (!strcmp(tmp, "WAIPSK"))){
				strcat(wlan_script_entry_attr_buf[authmod],tmp);
				strcat(wlan_script_entry_attr_buf[authmod],";");
				
				if(getAttrValue(top, nodeName, "EncrypType", tmp) == TCAPI_PROCESS_OK){
					strcat(wlan_script_entry_attr_buf[encyptype],tmp);
					strcat(wlan_script_entry_attr_buf[encyptype],";");
				}
				
				if(getAttrValue(top, nodeName, "WapiIfName", tmp) == TCAPI_PROCESS_OK){
					strcat(wlan_script_entry_attr_buf[wapiIfName],tmp);
					strcat(wlan_script_entry_attr_buf[wapiIfName],";");
				}	

				if(getAttrValue(top, nodeName, "WapiPsk1", tmp) == TCAPI_PROCESS_OK){
					strcat(wlan_script_entry_attr_buf[wapiPsk1],tmp);
					strcat(wlan_script_entry_attr_buf[wapiPsk1],";");
				}				

				if(getAttrValue(top, nodeName, "WapiPskType", tmp) == TCAPI_PROCESS_OK){
					strcat(wlan_script_entry_attr_buf[wapiPskType],tmp);
					strcat(wlan_script_entry_attr_buf[wapiPskType],";");
				}	

				if(getAttrValue(top, nodeName, "WapiAsCertPath", tmp) == TCAPI_PROCESS_OK){
					strcat(wlan_script_entry_attr_buf[wapiAsCertPath],tmp);
					strcat(wlan_script_entry_attr_buf[wapiAsCertPath],";");
				}

				if(getAttrValue(top, nodeName, "WapiUserCertPath", tmp) == TCAPI_PROCESS_OK){
					strcat(wlan_script_entry_attr_buf[wapiUserCertPath],tmp);
					strcat(wlan_script_entry_attr_buf[wapiUserCertPath],";");
				}

				if(getAttrValue(top, nodeName, "WapiAsIpAddr", tmp) == TCAPI_PROCESS_OK){
					strcat(wlan_script_entry_attr_buf[wapiAsIpAddr],tmp);
					strcat(wlan_script_entry_attr_buf[wapiAsIpAddr],";");
				}

				if(getAttrValue(top, nodeName, "WapiAsPort", tmp) == TCAPI_PROCESS_OK){
					strcat(wlan_script_entry_attr_buf[wapiAsPort],tmp);
					strcat(wlan_script_entry_attr_buf[wapiAsPort],";");
				}								
			}
#endif
			else{/*OPEN*/
			
				strcat(wlan_script_entry_attr_buf[authmod],"OPEN;");
				strcat(wlan_script_entry_attr_buf[encyptype],"NONE;");
				
				/*add by xyzhu_nj_20100211*/
				strcat(wlan_script_entry_attr_buf[defaultkeyid],"0;");
				strcat(wlan_script_entry_attr_buf[wpapsk],"0;");
				strcat(wlan_script_entry_attr_buf[Key1Type],"0;");
				strcat(wlan_script_entry_attr_buf[Key2Type],"0;");
				strcat(wlan_script_entry_attr_buf[Key3Type],"0;");
				strcat(wlan_script_entry_attr_buf[Key4Type],"0;");
				
				strcat(wlan_script_entry_attr_buf[key1str],"0;");
				strcat(wlan_script_entry_attr_buf[key2str],"0;");
		        strcat(wlan_script_entry_attr_buf[key3str],"0;");
		        strcat(wlan_script_entry_attr_buf[key4str],"0;");
			}
		}
#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7601E) || defined(MT7592)  
		if(getAttrValue(top, nodeName, "AuthMode", tmp) == TCAPI_PROCESS_OK){
			strncpy(encrytype,tmp,sizeof(encrytype));
		}
		if(getAttrValue(top, nodeName, "HT_MCS", tmp) == TCAPI_PROCESS_OK){
			if((strcmp(encrytype,"WPAPSK") == 0) &&(atoi(wirelessmode) == 9) && (atoi(tmp)> 7)){
				strcat(wlan_script_entry_attr_buf[mcs],"33;");	
			}else{
				strcat(wlan_script_entry_attr_buf[mcs],tmp);
				strcat(wlan_script_entry_attr_buf[mcs],";");
			}
		}
		if(Is11nWirelessMode(top)){
			strcat(wlan_script_entry_attr_buf[wmm],"0");
			strcat(wlan_script_entry_attr_buf[wmm],";");
		}else{
			if(getAttrValue(top, nodeName, "WMM", tmp) == TCAPI_PROCESS_OK){
				strcat(wlan_script_entry_attr_buf[wmm],tmp);
				strcat(wlan_script_entry_attr_buf[wmm],";");
			}
		}
		switch(atoi(strWirelessMode)){
			case PHY_MODE_BG_MIXED:
				if(getAttrValue(top, nodeName, "HT_RATE", strRate)== TCAPI_PROCESS_OK){
					if(strcmp(strRate,GVWlanChannelList[0].cChannelRate) == 0
						   || strcmp(strRate,GVWlanChannelList[1].cChannelRate) == 0
						   || strcmp(strRate,GVWlanChannelList[2].cChannelRate) == 0
						   || strcmp(strRate,GVWlanChannelList[3].cChannelRate) == 0)
					{	
						strcat(wlan_script_entry_attr_buf[fixedTxMode],"1");//CCK
						strcat(wlan_script_entry_attr_buf[fixedTxMode],";");
					}
				   	else{
						strcat(wlan_script_entry_attr_buf[fixedTxMode],"2");//OFDM
						strcat(wlan_script_entry_attr_buf[fixedTxMode],";");
					}
				}
				else{
					tcdbg_printf("WLan:get HT_RATE fail when wireless mode is BG!\n");		
					fclose(fp);
					return FAIL;
				}
				break;
			case PHY_MODE_B_ONLY:
				strcat(wlan_script_entry_attr_buf[fixedTxMode],"1");//CCK
				strcat(wlan_script_entry_attr_buf[fixedTxMode],";");
				break;
			case PHY_MODE_G_ONLY:
				strcat(wlan_script_entry_attr_buf[fixedTxMode],"2");//OFDM
				strcat(wlan_script_entry_attr_buf[fixedTxMode],";");
				break;
			default:
				strcat(wlan_script_entry_attr_buf[fixedTxMode],"0");//OFDM
				strcat(wlan_script_entry_attr_buf[fixedTxMode],";");
				break;
		}
	#else
		if(getAttrValue(top, nodeName, "WMM", tmp) == TCAPI_PROCESS_OK){
			strcat(wlan_script_entry_attr_buf[wmm],tmp);
			strcat(wlan_script_entry_attr_buf[wmm],";");
		}	
	#endif	
#if defined(TCSUPPORT_WLAN_MAXSTANUM_GUI)
		memset(tmp,0,sizeof(tmp));
		if((getAttrValue(top, nodeName, "MaxStaNum", tmp) == TCAPI_PROCESS_OK) && (strlen(tmp) > 0)){
			strcat(wlan_script_entry_attr_buf[MaxStaNum],tmp);
			strcat(wlan_script_entry_attr_buf[MaxStaNum],";");
		}
		else{//add default value for MaxStaNum into Wifi profile
			strcpy(tmp,"0");
			strcat(wlan_script_entry_attr_buf[MaxStaNum],tmp);
			strcat(wlan_script_entry_attr_buf[MaxStaNum],";");
		}
#endif
	
	#ifdef WSC_AP_SUPPORT//add by xyyou
		if(getAttrValue(top, nodeName, "WPSConfMode", tmp) == TCAPI_PROCESS_OK){
			strcat(wlan_script_entry_attr_buf[wsconfmode],tmp);
			strcat(wlan_script_entry_attr_buf[wsconfmode],";");
		}	
		if(getAttrValue(top, nodeName, "WPSConfStatus", tmp) == TCAPI_PROCESS_OK){
			strcat(wlan_script_entry_attr_buf[wscconfstatus],tmp);
			strcat(wlan_script_entry_attr_buf[wscconfstatus],";");
		}
		if(getAttrValue(top, nodeName, "WPSKeyASCII", tmp) == TCAPI_PROCESS_OK){
			strcat(wlan_script_entry_attr_buf[wsckeyascii],tmp);
			strcat(wlan_script_entry_attr_buf[wsckeyascii],";");
		}
		//added by fredli, for wsc 2.0
		if(getAttrValue(top, nodeName, "WscV2Support", tmp) == TCAPI_PROCESS_OK){
			strcat(wlan_script_entry_attr_buf[wscv2support],tmp);
			strcat(wlan_script_entry_attr_buf[wscv2support],";");
		}
		else{//add default value for WscV2Support into Wifi profile
			strcpy(tmp,"0");
			strcat(wlan_script_entry_attr_buf[wscv2support],tmp);
			strcat(wlan_script_entry_attr_buf[wscv2support],";");
		}
		if(getAttrValue(top, nodeName, "WscMaxPinAttack", tmp) == TCAPI_PROCESS_OK){
                        strcat(wlan_script_entry_attr_buf[WscMaxPinAttack],tmp);
                        strcat(wlan_script_entry_attr_buf[WscMaxPinAttack],";");
        }
		if(getAttrValue(top, nodeName, "WscSetupLockTime", tmp) == TCAPI_PROCESS_OK){
                        strcat(wlan_script_entry_attr_buf[WscSetupLockTime],tmp);
                        strcat(wlan_script_entry_attr_buf[WscSetupLockTime],";");
        }		
	#endif
	#ifdef IGMP_SNOOP_SUPPORT//add by xyyou
		//if(getAttrValue(top, nodeName, "IgmpSnEn", tmp) == TCAPI_PROCESS_OK){
			//strcat(wlan_script_entry_attr_buf[igmpsnooping],tmp);
		// setting default value of igmp snooping to 1
	#if (defined(RT3390) || defined(RT3090)) && !defined(WLAN_MULTIDRIVER)//just for 3390&3090, 5392 used as common attribute
			strcat(wlan_script_entry_attr_buf[igmpsnooping], "1");
			strcat(wlan_script_entry_attr_buf[igmpsnooping],";");
		#endif
		//}
	#endif
	}

	/*write all entry config to file*/
	#if 0
	#ifdef WSC_AP_SUPPORT//add by xyyou
	for(i=ssid;i <= wsckeyascii;i++)
	#else
	#if defined(RT3390) || defined(RT3090) 
	for(i=ssid;i <= mcs;i++)
	#else
	for(i=ssid;i <= wmm;i++)
	#endif//end xyyou
	#endif
	#endif
	for(i=ssid;i < wlan_entry_attr_len;i++)
	{
		if(strlen(wlan_script_entry_attr_buf[i]) > 0){
			wlan_script_entry_attr_buf[i][strlen(wlan_script_entry_attr_buf[i]) - 1] ='\0';/*remove last ";"*/
			sprintf(buf,"%s=%s\n",wlan_script_entry_attr_para[i],wlan_script_entry_attr_buf[i]);
			fputs(buf,fp);
		}
	}

	/*AccessPolicy0=.... AccessPolicy1=....*/
	for(j=0; j < BssidNum; j++){
		sprintf(nodeName[1],"%s%d",SUB_NODE_NAME,j);
		if(getAttrValue(top, nodeName, "AccessPolicy", tmp) == TCAPI_PROCESS_OK){
			if(strlen(tmp) > 0){
				sprintf(buf,"%s%d=%s\n","AccessPolicy",j,tmp);
				fputs(buf,fp);
			}
		}
	}
	/*AccessControlList0=MAC1;MAC2;.......*/

	for(j=0; j < BssidNum; j++){
		sprintf(nodeName[1],"%s%d",SUB_NODE_NAME,j);
		strcpy(buf,"");
		for(i=0;i<WLAN_MAC_NUM;i++){
			sprintf(tmp2,"%s%d","WLan_MAC",i);
			if(getAttrValue(top, nodeName, tmp2, tmp) == TCAPI_PROCESS_OK){
				if(strlen( tmp) > 0){
					strcat(buf,tmp);
					strcat(buf,";");
				}
			}
		}
		if(strlen(buf) > 0){
			buf[strlen(buf) - 1] ='\0';/*remove last ";"*/
			/*
				krammer change for bug 1359, the .dat has no need to add ""!
			*/
			#if 0
			sprintf(tmp,"%s%d=\"%s\"\n","AccessControlList",j,buf);/*add ""*/
			#endif
			sprintf(tmp,"%s%d=%s\n","AccessControlList",j,buf);
			fputs(tmp,fp);
		}
	}

	#ifdef WSC_AP_SUPPORT//add by xyyou
	/*WscDefaultSSID1=....WscDefaultSSID2=.......*/
	for(j=0; j < BssidNum; j++){
		//sprintf(buf,"%s%d=%s\n","WscDefaultSSID",j,"Trendchip");
		sprintf(nodeName[1],"%s%d",SUB_NODE_NAME,j);
		sprintf(tmp2,"%s%d","WscDefaultSSID",j+1);
		if(getAttrValue(top, nodeName, tmp2, tmp) == TCAPI_PROCESS_OK){
			if(strlen(tmp)>0){
				sprintf(buf,"%s%d=%s\n","WscDefaultSSID",j+1,tmp);
				fputs(buf,fp);				
			}
		}
	}
	#endif
	
#ifdef BBUTOWBU
{
   	FILE *fptmp;  
	unsigned char flash_mac_addr[6]={0};
	unsigned char mac_addr[]={0x00,0xaa,0xbb,0x01,0x23,0x45};
	
    	/* Read bootloader from flash */
    	system("cat /dev/mtd0 > /tmp/boot.bin");

    	/*Read MAC*/
    	fptmp = fopen("/tmp/boot.bin", "r+b");
    	if( fptmp == NULL ){
       	 tcdbg_printf("open boot.bin file error!!\n");
        	system("rm /tmp/boot.bin");        
        	return FAIL;
    	}

    	fseek(fptmp, 0x0000ff48L, SEEK_SET);
    	fread(flash_mac_addr, 6, 1, fptmp);
	if( (flash_mac_addr[0] == 0) && (flash_mac_addr[1] == 0) && (flash_mac_addr[2] == 0) &&
	    (flash_mac_addr[3] == 0) && (flash_mac_addr[4] == 0) && (flash_mac_addr[5] == 0) ) {
		tcdbg_printf("MacAddress for wifi chip is NULL, force to default!!!\n");
	}
	else {    
		 memcpy(mac_addr, flash_mac_addr, 6);  	
	}
	sprintf(buf, "MacAddress=%02x:%02x:%02x:%02x:%02x:%02x\n",mac_addr[0],mac_addr[1],mac_addr[2],mac_addr[3],mac_addr[4],mac_addr[5]);
	fputs(buf,fp);
		
    fclose(fptmp);
	system("rm /tmp/boot.bin");        
}	
#endif

	fclose(fp);
	chmod(WLAN_PATH,S_IRUSR|S_IWUSR|S_IXUSR);

  	return SUCCESS;
}

/*______________________________________________________________________________
**	function name:write_wlan_exe_sh
**
**	description:
*     create wireless execute script
**	parameters:
*     top:xml top node address
**	global:
*     none
**	return:
*       0:success
*	  1:fail
**	call:
*     none
**	revision:
*     1.shnwind 20080530
**____________________________________________________________________________*/
int
write_wlan_exe_sh(mxml_node_t *top, int BssidNum){
	int i, wlan_id;
  	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], tmp[160], tmp2[160],buf[160];
  	FILE *fp;
	char wirelessmode[16], encrytype[16];
	char wlan_script_com_attr[][ATTR_SIZE]=
	{
		{"CountryRegion"},
		{"Channel"},
		{"BeaconPeriod"},
		{"RTSThreshold"},
		{"FragThreshold"},
		{"DtimPeriod"},
		/*{"WirelessMode"},
		{"BasicRate"},*/
	#if defined(RT5392) || defined(WLAN_MULTIDRIVER) ||defined(MT7601E) || defined(MT7592) 
		{"IgmpSnEnable"},
	#endif
		{""}
	};

	const char wlan_script_com_attrs_other[][2][ATTR_SIZE]=
	{
		{"HT_RDG", "HtRdg"},
		{"HT_STBC", "HtStbc"},
		{"HT_AMSDU", "HtAmsdu"},
		{"HT_AutoBA", "HtAutoBa"},
		{"HT_BADecline", "BADecline"},
		{"HT_DisallowTKIP", "HtDisallowTKIP"},
		{"HT_TxStream", "HtTxStream"},
		{"HT_RxStream", "HtRxStream"},
		{"APSDCapable", "UAPSDCapable"},
		{"HT_BSSCoexistence","HtBssCoex"}, 
		{"", ""}
	};

	memset(nodeName,0,sizeof(nodeName));
  	strcpy(nodeName[0],WEBCURSET);
  	strcpy(nodeName[1],SUB_NODE_NAME);
  	if(getAttrValue(top, nodeName, "wlan_id", tmp) == TCAPI_PROCESS_OK){
    		wlan_id = atoi(tmp);
  	}else{
    		return FAIL;
 	 }


	/*write common setting WLAN_exec.sh*/
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],WLAN);
	 strcpy(nodeName[1],"Common");

	fp = fopen(WLAN_SCRIPT_PATH,"w");
	if(fp == NULL){
		return FAIL;
	}
	 for(i = 0;strlen(wlan_script_com_attr[i]) != 0;i++){
	 	if(strcmp(wlan_script_com_attr[i], "Channel")==0){
			if(getAttrValue(top, nodeName, wlan_script_com_attr[i], tmp) == TCAPI_PROCESS_OK){
				if(atoi(tmp)){
					sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, wlan_script_com_attr[i], tmp);
					fputs(buf,fp);
				}
			}
		}else{
			if(getAttrValue(top, nodeName, wlan_script_com_attr[i], tmp) == TCAPI_PROCESS_OK){
				sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, wlan_script_com_attr[i], tmp);
				fputs(buf,fp);
			}
		}
	 }
#if defined(TCSUPPORT_CT_WLAN_NODE)
	if(getAttrValue(top, nodeName, "TxPowerLevel", tmp) == TCAPI_PROCESS_OK){
		switch(atoi(tmp)){
		case TX_POWER_LEVLE_1:
			sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "TxPower", "100");
			fputs(buf,fp);
			break;
		case TX_POWER_LEVLE_2:
			sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "TxPower", "75");
			fputs(buf,fp);
			break;
		case TX_POWER_LEVLE_3:
			sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "TxPower", "50");
			fputs(buf,fp);
			break;
		case TX_POWER_LEVLE_4:
			sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "TxPower", "25");
			fputs(buf,fp);
			break;
		case TX_POWER_LEVLE_5:
			sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "TxPower", "15");
			fputs(buf,fp);
			break;
		default:
			tcdbg_printf("WLan:the valule of TxPowerLevel is not right!\n");
			fclose(fp);
			return FAIL;
		}
	}
#endif

	 for(i = 0;strlen(wlan_script_com_attrs_other[i]) != 0;i++){
		 if(getAttrValue(top, nodeName, wlan_script_com_attrs_other[i][0], tmp) == TCAPI_PROCESS_OK){
			 sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, wlan_script_com_attrs_other[i][1], tmp);
			 fputs(buf,fp);
		 }
	 }

	 if(getAttrValue(top, nodeName, "WirelessMode", tmp) == TCAPI_PROCESS_OK){
		sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id,"WirelessMode", tmp);
		fputs(buf,fp);
		strncpy(wirelessmode, tmp, sizeof(wirelessmode));
	#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7601E) || defined(MT7592)  //add by xyyou
		switch(atoi(tmp)){
			case PHY_MODE_BG_MIXED:
			case PHY_MODE_BGN_MIXED:
			case PHY_MODE_N_ONLY:
				sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id,"BasicRate", "15");
				fputs(buf,fp);
				break;
			case PHY_MODE_B_ONLY:
				sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id,"BasicRate", "3");
				fputs(buf,fp);
				break;
			case PHY_MODE_G_ONLY:
			case PHY_MODE_GN_MIXED:
				sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id,"BasicRate", "351");
				fputs(buf,fp);
				break;
			default:
				tcdbg_printf("WLan: wireless mode is not right!(3390,3090)\n");
				fclose(fp);
				return FAIL;

		}
		/*
		if((atoi(tmp)==PHY_MODE_N_ONLY)||(atoi(tmp)==PHY_MODE_GN_MIXED)||(atoi(tmp)==PHY_MODE_BGN_MIXED)){
			isNmode=1;
		}else{
			isNmode=0;
		}*/
	#else
		switch(atoi(tmp)){
			case PHY_MODE_BG_MIXED:
				sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id,"BasicRate", "15");
				fputs(buf,fp);
				break;
			case PHY_MODE_B_ONLY:
				sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id,"BasicRate", "3");
				fputs(buf,fp);
				break;
			case PHY_MODE_G_ONLY:
				sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id,"BasicRate", "351");
				fputs(buf,fp);
				break;
			default:
				tcdbg_printf("WLan: wireless mode is not right when set BasicRate!\n");
				fclose(fp);
				return FAIL;

		}
	#endif
	}
#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7601E) || defined(MT7592) //add by xyyou, for support 11n of RT3390
	if(Is11nWirelessMode(top)){
		if(getAttrValue(top, nodeName, "HT_BW", tmp) == TCAPI_PROCESS_OK){
			sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "HtBw", tmp);
			fputs(buf,fp);
		}
		if(atoi(tmp)==1){
			#if 0
			if(getAttrValue(top, nodeName, "HT_EXTCHA", tmp) == TCAPI_PROCESS_OK){
				sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "HtExtcha", tmp);
				fputs(buf,fp);
			}
			#endif
			sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "HtExtcha", "0");
			if(getAttrValue(top, nodeName, "Channel", tmp) == TCAPI_PROCESS_OK){
				if(atoi(tmp) <= 4 && (atoi(tmp) != 0)){
					sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "HtExtcha", "1");
				}
			}
			fputs(buf,fp);	
		}
		if(getAttrValue(top, nodeName, "HT_GI", tmp) == TCAPI_PROCESS_OK){
			sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "HtGi", tmp);
			fputs(buf,fp);
		}
	}else{
		sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "HtBw", "0");
		fputs(buf,fp);
	}
#endif //end xyyou

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],WLAN);
	sprintf(nodeName[1],"%s%d",SUB_NODE_NAME,wlan_id);
	if(getAttrValue(top, nodeName, "AuthMode", tmp) == TCAPI_PROCESS_OK){
		strncpy(encrytype,tmp,sizeof(encrytype));	
	}
#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7601E) || defined(MT7592) //add by xyyou, for support 11n of RT3390
	if(Is11nWirelessMode(top)){
		if(getAttrValue(top, nodeName, "HT_MCS", tmp) == TCAPI_PROCESS_OK){
			if(atoi(tmp)<=15){
				if((strcmp(encrytype,"WPAPSK") == 0) &&(atoi(wirelessmode) == 9) && (atoi(tmp)> 7)){
					sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "HtMcs", "33");
				}	
				else
				sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "HtMcs", tmp);
			}else
				sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "HtMcs", "33");//33 for auto selection,xyyou
			fputs(buf,fp);
		}
	}else{
		sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "HtMcs", "33");
		fputs(buf,fp);
	}
#endif //end xyyou
	/*write entry  setting to WLAN_exec.sh*/
/*
	2008 11 17 krammer change for bug 1242
    	SSID must be the last setting of all cmd, referenced in RT61 release note
*/
#if 0
	if(getAttrValue(top, nodeName, "SSID", tmp) == TCAPI_PROCESS_OK){
		sprintf(tmp2,"\"%s\"",tmp);/*need "..."*/
		sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "SSID", tmp2);
		fputs(buf,fp);
	}
#endif
	if(getAttrValue(top, nodeName, "HideSSID", tmp) == TCAPI_PROCESS_OK){
		sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "HideSSID", tmp);
		fputs(buf,fp);
	}
	/*AuthMode*/
 	if(getAttrValue(top, nodeName, "AuthMode", tmp) == TCAPI_PROCESS_OK){
		if(strstr(tmp, "WEP")){/*WEP64-bits or WEP128-bits*/
#if defined(TCSUPPORT_CT_WLAN_NODE)
				//added by xmdai_nj: seperator WEP Encryption type and Authentication type
				if(getAttrValue(top, nodeName, "WEPAuthType", tmp) == TCAPI_PROCESS_OK){
					if(!strcmp(tmp, "OpenSystem")){
						sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "AuthMode", "OPEN");
						fputs(buf,fp);
					}else if(!strcmp(tmp, "SharedKey")){
						sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "AuthMode", "SHARED");
						fputs(buf,fp);
					}else{
						sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "AuthMode", "WEPAUTO");
						fputs(buf,fp);
					}
				}
#else
				sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "AuthMode", "WEPAUTO");
				fputs(buf,fp);
#endif
				sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "EncrypType", "WEP");
				fputs(buf,fp);
				if(getAttrValue(top, nodeName, "DefaultKeyID", tmp) != TCAPI_PROCESS_OK){
					strcpy(tmp,"1");
				}
				sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "DefaultKeyID", tmp);
				fputs(buf,fp);
				/*key1~key4*/
				for(i=1;i<=KEY_NUM;i++){
					sprintf(tmp2,"Key%dStr",i);
					if(getAttrValue(top, nodeName, tmp2, tmp) == TCAPI_PROCESS_OK){
						/*check strlen because the key maybe empty*/
						if(strlen(tmp) > 0){
						specialCharacterHandle(tmp);
						sprintf(tmp2,"\"%s\"",tmp);/*need "..."*/
						
						sprintf(tmp,"Key%d",i);
						/*krammer change for bug 1352*/
						#if 0
						if(tmp[0] == '0' && tmp[1] == 'x'){/*if  hex, remove 0x.*/
            						sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, tmp2, tmp+2);
          					}else{
            						sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, tmp2, tmp);
          					}
						#endif
						sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, tmp, tmp2);
						fputs(buf,fp);
					}
				}
				}
        		}
		/*krammer change for bug 1242
		    WPAPSK and WPA2PSK cmd must be the formate as following,
		    1.AuthMode
		    2.EncrypType
		    3.SSID
		    4.WPAPSK
		    5.DefaultKeyID=2
		       .
		       .
		       .
		     n.SSID (referenced by RT61 release note)
		    so i change the sequence of following code and add a setting SSID cmd
		*/
			else if(!strcmp(tmp, "WPAPSK")||!strcmp(tmp, "WPA2PSK")||!strcmp(tmp, "WPAPSKWPA2PSK")){/*WPAPSK or WPA2PSK or Mixed mode*/
				sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "AuthMode", tmp);
				fputs(buf,fp);
				if(getAttrValue(top, nodeName, "EncrypType", tmp) == TCAPI_PROCESS_OK){
					sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "EncrypType", tmp);
					fputs(buf,fp);
				}
			#if 0
				sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "DefaultKeyID", "2");/*WPAPSK and WPA2PSK use DefaultKeyID = 2*/
				fputs(buf,fp);
			#endif
			/*krammer add for bug 1242*/
			if(getAttrValue(top, nodeName, "SSID", tmp) == TCAPI_PROCESS_OK){
				specialCharacterHandle(tmp);
				sprintf(tmp2,"\"%s\"",tmp);/*need "..."*/
				sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "SSID", tmp2);
				fputs(buf,fp);
			}
			/*krammer add for bug 1242*/
				if(getAttrValue(top, nodeName, "WPAPSK", tmp) == TCAPI_PROCESS_OK){
					specialCharacterHandle(tmp);
					sprintf(tmp2,"\"%s\"",tmp);/*need "..."*/
					sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "WPAPSK", tmp2);
					fputs(buf,fp);
				}
			/*krammer change*/
			sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "DefaultKeyID", "2");/*WPAPSK and WPA2PSK use DefaultKeyID = 2*/
			fputs(buf,fp);
			/*krammer change*/

			
			}
#if defined(TCSUPPORT_CT_WLAN_WAPI)
			else if((!strcmp(tmp, "WAICERT")) || (!strcmp(tmp, "WAIPSK"))){
				sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "AuthMode", tmp);
				fputs(buf,fp);
				if(getAttrValue(top, nodeName, "EncrypType", tmp) == TCAPI_PROCESS_OK){
					tcdbg_printf("write_wlan_exe_sh: EncrypType = %s\r\n",tmp);
					sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "EncrypType", tmp);
					fputs(buf,fp);
				}
				if(getAttrValue(top, nodeName, "WapiIfName", tmp) == TCAPI_PROCESS_OK){
					sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "WapiIfName", tmp);
					fputs(buf,fp);
				}	

				if(getAttrValue(top, nodeName, "WapiPsk1", tmp) == TCAPI_PROCESS_OK){
					sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "WapiPsk1", tmp);
					fputs(buf,fp);
				}				

				if(getAttrValue(top, nodeName, "WapiPskType", tmp) == TCAPI_PROCESS_OK){
					sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "WapiPskType", tmp);
					fputs(buf,fp);
				}	

				if(getAttrValue(top, nodeName, "WapiAsCertPath", tmp) == TCAPI_PROCESS_OK){
					sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "WapiAsCertPath", tmp);
					fputs(buf,fp);
				}

				if(getAttrValue(top, nodeName, "WapiUserCertPath", tmp) == TCAPI_PROCESS_OK){
					sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "WapiUserCertPath", tmp);
					fputs(buf,fp);
				}

				if(getAttrValue(top, nodeName, "WapiAsIpAddr", tmp) == TCAPI_PROCESS_OK){
					sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "WapiAsIpAddr", tmp);
					fputs(buf,fp);
				}

				if(getAttrValue(top, nodeName, "WapiAsPort", tmp) == TCAPI_PROCESS_OK){
					sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "WapiAsPort", tmp);
					fputs(buf,fp);
				}								
					
			}
#endif
			else{/*OPEN*/
			
				sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "AuthMode", "OPEN");
				fputs(buf,fp);
				sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "EncrypType", "NONE");
				fputs(buf,fp);
			}
 	}
	/*AccessPolicy=....*/
	if(getAttrValue(top, nodeName, "AccessPolicy", tmp) == TCAPI_PROCESS_OK){
		if(strlen(tmp) > 0){
			sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "AccessPolicy", tmp);
			fputs(buf,fp);
		}
	}
	/*AccessControlList=MAC1;MAC2;.......*/

	strcpy(buf,"");
	for(i=0;i<WLAN_MAC_NUM;i++){
		sprintf(tmp2,"%s%d","WLan_MAC",i);
		if(getAttrValue(top, nodeName, tmp2, tmp) == TCAPI_PROCESS_OK){
			if(strlen( tmp) > 0){
				strcat(buf,tmp);
				strcat(buf,";");
			}
		}
	}
	if(strlen(buf) > 0){
		buf[strlen(buf) - 1] ='\0';/*remove last ";"*/
		sprintf(tmp2,"\"%s\"",buf);/*add ""*/
	#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7601E) || defined(MT7592) //add by xyyou
		sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "ACLClearAll","1");
		fputs(buf,fp);
		sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "ACLAddEntry",tmp2);
	#else
		sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "AccessControlList",tmp2);
	#endif
		fputs(buf,fp);
	}
	#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7601E) || defined(MT7592)  //add by xyyou, for support 11n of RT3390
	if(!Is11nWirelessMode(top)){

		if(getAttrValue(top, nodeName, "WMM", tmp) == TCAPI_PROCESS_OK){
			sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "WmmCapable", tmp);
			fputs(buf,fp);
			if(txBurst_or_not(top, BssidNum)){
				sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "TxBurst", "1");
			}
			else{
				sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "TxBurst", "0");
			}
			fputs(buf,fp);
		}
	}
	#else
	if(getAttrValue(top, nodeName, "WMM", tmp) == TCAPI_PROCESS_OK){
		sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "WmmCapable", tmp);
		fputs(buf,fp);
		if(txBurst_or_not(top, BssidNum)){
			sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "TxBurst", "1");
		}
		else{
			sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "TxBurst", "0");
		}
		fputs(buf,fp);
	}
	#endif//end xyyou
		/*
		2008 11 17 krammer change for bug 1242
   	 	SSID must be the last setting of all cmd, referenced in RT61 release note
	*/
	#ifdef WSC_AP_SUPPORT//add by xyyou
	#ifdef TCSUPPORT_WLAN_MULTI_WPS
		setAttrValue(top, nodeName, "WPSConfStatus", "2");			
		sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "WscConfStatus", "2");
		
		fputs(buf,fp);
		if(getAttrValue(top, nodeName, "WPSConfMode", tmp) == TCAPI_PROCESS_OK){
			sprintf(buf, WLAN_SCRIPT_PREFIX, wlan_id, "WscConfMode", tmp);
			fputs(buf,fp);
		}			
		//added by fred, for wsc v2
		if(getAttrValue(top, nodeName, "WscV2Support", tmp) == TCAPI_PROCESS_OK){
			sprintf(buf, WLAN_SCRIPT_PREFIX, wlan_id, "WscV2Support", tmp);
			fputs(buf,fp);
		}
		if(getAttrValue(top, nodeName, "WscMaxPinAttack", tmp) == TCAPI_PROCESS_OK){
                            sprintf(buf, WLAN_SCRIPT_PREFIX, wlan_id, "WscMaxPinAttack", tmp);
                            fputs(buf,fp);
                    }
		if(getAttrValue(top, nodeName, "WscSetupLockTime", tmp) == TCAPI_PROCESS_OK){
                            sprintf(buf, WLAN_SCRIPT_PREFIX, wlan_id, "WscSetupLockTime", tmp);
                            fputs(buf,fp);
                    }
	#else
		if(wlan_id==0){//now we just support main SSID,xyyou
			setAttrValue(top, nodeName, "WPSConfStatus", "2");			
			sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "WscConfStatus", "2");
			
			fputs(buf,fp);
			if(getAttrValue(top, nodeName, "WPSConfMode", tmp) == TCAPI_PROCESS_OK){
				sprintf(buf, WLAN_SCRIPT_PREFIX, wlan_id, "WscConfMode", tmp);
				fputs(buf,fp);
			}			
			//added by fred, for wsc v2
			if(getAttrValue(top, nodeName, "WscV2Support", tmp) == TCAPI_PROCESS_OK){
				sprintf(buf, WLAN_SCRIPT_PREFIX, wlan_id, "WscV2Support", tmp);
				fputs(buf,fp);
			}
			if(getAttrValue(top, nodeName, "WscMaxPinAttack", tmp) == TCAPI_PROCESS_OK){
                                sprintf(buf, WLAN_SCRIPT_PREFIX, wlan_id, "WscMaxPinAttack", tmp);
                                fputs(buf,fp);
                        }
			if(getAttrValue(top, nodeName, "WscSetupLockTime", tmp) == TCAPI_PROCESS_OK){
                                sprintf(buf, WLAN_SCRIPT_PREFIX, wlan_id, "WscSetupLockTime", tmp);
                                fputs(buf,fp);
                        }


		}
	#endif
	#endif //end xyyou

	//if(getAttrValue(top, nodeName, "IgmpSnEn", tmp) == TCAPI_PROCESS_OK){
		//sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "IgmpSnEnable", tmp);
		// setting default value to 1 for igmp snooping
		#if (defined(RT3390) || defined(RT3090)) && !defined(WLAN_MULTIDRIVER)//just for 3390&3090, 5392 used as common attribute
		sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "IgmpSnEnable","1");
		fputs(buf,fp);
		#endif
	//}

	if(getAttrValue(top, nodeName, "SSID", tmp) == TCAPI_PROCESS_OK){
		specialCharacterHandle(tmp);
		sprintf(tmp2,"\"%s\"",tmp);/*need "..."*/
		sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "SSID", tmp2);
		fputs(buf,fp);
	}
#if defined(TCSUPPORT_WLAN_MAXSTANUM_GUI)
		if(getAttrValue(top, nodeName, "MaxStaNum", tmp) == TCAPI_PROCESS_OK){
			sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "MaxStaNum", tmp);
			fputs(buf,fp);
		}
#endif
	
/*Trigger AP resccan depends on the value of HT_BW and HT_BSSCoexistence*/
#if	defined(MT7592)
	strcpy(nodeName[1],"Common");
	getAttrValue(top, nodeName, "HT_BSSCoexistence", tmp);
	getAttrValue(top, nodeName, "HT_BW", tmp2);
	if (atoi(tmp) == 1 && atoi(tmp2) == 1)
	{
	sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "AP2040Rescan", "1");
	fputs(buf,fp);
	}	
#endif
  	fclose(fp);
 	chmod(WLAN_SCRIPT_PATH,S_IRUSR|S_IWUSR|S_IXUSR);
	/*sleep 1 and restart boa
		sleep 1 to let the web page finish getting information.
		restart boa to prevent error when user use change ssid and link CPE by using wireless.
	fp = fopen(WLAN_RESTART_BOA,"w");
	if(fp != NULL){
		sprintf(buf,"sleep 1\nkillall -9 boa\n/userfs/bin/boa -c /boaroot/ -d &\n");
		fputs(buf, fp);
		fclose(fp);
	}
	chmod(WLAN_RESTART_BOA,S_IRUSR|S_IWUSR|S_IXUSR);*/

#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7601E) || defined(MT7592)  //used to set WIFi Tx Rate
	if(Is11nWirelessMode(top)){
		if(getAttrValue(top, nodeName, "HT_MCS", tmp) == TCAPI_PROCESS_OK){
			if((strcmp(encrytype,"WPAPSK") == 0) &&(atoi(wirelessmode) == 9) && (atoi(tmp)> 7)){
				strcpy(tmp,"33");
			}	
			doWlanMcs(top, tmp);
		}
	}
	else{
		if(getAttrValue(top, nodeName, "HT_RATE", tmp) == TCAPI_PROCESS_OK){
			doWlanRate(top, tmp);
		}
	}
#endif 

  	return SUCCESS;
}

#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7601E) || defined(MT7592) //used to set WIFi Tx Rate
int
doWlanRate(mxml_node_t *top, char *Rate){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], tmp[160], tmp2[160], buf[160],Mcs[16];
	char *cIdeaWM; 
	int wlan_id, iWMIndex, iWMNumbers ;
	FILE *fp;

	memset(nodeName,0,sizeof(nodeName));
  	strcpy(nodeName[0],WEBCURSET);
  	strcpy(nodeName[1],SUB_NODE_NAME);
  	if(getAttrValue(top, nodeName, "wlan_id", tmp) == TCAPI_PROCESS_OK){
    		wlan_id = atoi(tmp);
  	}else{
    		return FAIL;
 	 }

	fp = fopen(WLAN_MCS_SCRIPT_PATH,"w");
	if(fp == NULL){
		return FAIL;
	}
		
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],WLAN);
	strcpy(nodeName[1],"Common");
 	if(getAttrValue(top, nodeName, "WirelessMode", tmp) == TCAPI_PROCESS_OK){
		switch(atoi(tmp)){
			case PHY_MODE_BG_MIXED:
				sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id,"HtOpMode", "1");
				fputs(buf,fp);
				sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id,"BasicRate", "15");
				fputs(buf,fp);
				if(strcmp(Rate,GVWlanChannelList[0].cChannelRate) == 0
					   || strcmp(Rate,GVWlanChannelList[1].cChannelRate) == 0
					   || strcmp(Rate,GVWlanChannelList[2].cChannelRate) == 0
					   || strcmp(Rate,GVWlanChannelList[3].cChannelRate) == 0)
				{	
					sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id,"FixedTxMode", "1");  //CCK
				}
			   	else{
	         			sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id,"FixedTxMode", "2"); //OFDM
				}
				fputs(buf,fp);
				break;
			case PHY_MODE_B_ONLY:
        			sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id,"HtOpMode", "1");
				fputs(buf,fp);
				sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id,"BasicRate", "3");
				fputs(buf,fp);
				sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id,"FixedTxMode", "1");
				fputs(buf,fp);		
				break;
			case PHY_MODE_G_ONLY:
				sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id,"HtOpMode", "0");
				fputs(buf,fp);
				sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id,"BasicRate", "351");
				fputs(buf,fp);
				sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id,"FixedTxMode", "2");
				fputs(buf,fp);
				break;
			default:
				tcdbg_printf("doWlanRate:wireless mode is not right!");
				fclose(fp);
				return FAIL;

		}
	}

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],WLAN);
	sprintf(nodeName[1],"%s%d",SUB_NODE_NAME,wlan_id);

	iWMNumbers = sizeof(GVWlanChannelList)/sizeof(GVWlanChannelList[0]);
	for( iWMIndex = 0 ; iWMIndex < iWMNumbers ; iWMIndex++ )
	{
		cIdeaWM = GVWlanChannelList[iWMIndex].cChannelRate;          
		if(strcmp(Rate, cIdeaWM) == 0)
		{	
			memset(Mcs, 0, sizeof(Mcs));
			sprintf(Mcs, "%d", GVWlanChannelList[iWMIndex].iRateCount);	
			setAttrValue(top, nodeName, "HT_MCS",Mcs);
			sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "HtMcs", Mcs);
			fputs(buf,fp);
			
		}
	}
	
	if(getAttrValue(top, nodeName, "SSID", tmp) == TCAPI_PROCESS_OK){
		specialCharacterHandle(tmp);
		sprintf(tmp2,"\"%s\"",tmp);/*need "..."*/
		sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "SSID", tmp2);
		fputs(buf,fp);
	}
  	fclose(fp);
 	chmod(WLAN_MCS_SCRIPT_PATH,S_IRUSR|S_IWUSR|S_IXUSR);

	return SUCCESS;
}

int
doWlanMcs(mxml_node_t *top, char *Mcs){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], tmp[160], tmp2[160], buf[160];
	int wlan_id;
	FILE *fp;

	memset(nodeName,0,sizeof(nodeName));
  	strcpy(nodeName[0],WEBCURSET);
  	strcpy(nodeName[1],SUB_NODE_NAME);
  	if(getAttrValue(top, nodeName, "wlan_id", tmp) == TCAPI_PROCESS_OK){
    		wlan_id = atoi(tmp);
  	}else{
    		return FAIL;
 	 }

	fp = fopen(WLAN_MCS_SCRIPT_PATH,"w");
	if(fp == NULL){
		return FAIL;
	}
		
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],WLAN);
	strcpy(nodeName[1],"Common");
 	if(getAttrValue(top, nodeName, "WirelessMode", tmp) == TCAPI_PROCESS_OK){
		switch(atoi(tmp)){
			case PHY_MODE_BGN_MIXED:
				sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id,"HtOpMode", "0");
				fputs(buf,fp);
				sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id,"BasicRate", "15");
				fputs(buf,fp);
				sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id,"FixedTxMode", "0");
				fputs(buf,fp);
				break;
			case PHY_MODE_N_ONLY:
				sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id,"HtOpMode", "1");
				fputs(buf,fp);
				sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id,"BasicRate", "15");
				fputs(buf,fp);
				sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id,"FixedTxMode", "0");
				fputs(buf,fp);
				break;
			case PHY_MODE_GN_MIXED:
				sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id,"HtOpMode", "0");
				fputs(buf,fp);
				sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id,"BasicRate", "351");
				fputs(buf,fp);
				sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id,"FixedTxMode", "0");
				fputs(buf,fp);
				break;
			default:
				tcdbg_printf("doWlanMcs: wireless mode is not right!\n");
				fclose(fp);
				return FAIL;

		}
	}

	sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "HtMcs", Mcs);
	fputs(buf,fp);
#if	defined(MT7592)	
	strcpy(nodeName[1],"Common");	
	getAttrValue(top, nodeName, "HT_BSSCoexistence", tmp);	
	getAttrValue(top, nodeName, "HT_BW", tmp2);	
	if (atoi(tmp) == 1 && atoi(tmp2) == 1){	
		sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "AP2040Rescan", "1");	
		fputs(buf,fp);	
	}		
#endif
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],WLAN);
	sprintf(nodeName[1],"%s%d",SUB_NODE_NAME,wlan_id);
	if(getAttrValue(top, nodeName, "SSID", tmp) == TCAPI_PROCESS_OK){
		specialCharacterHandle(tmp);
		sprintf(tmp2,"\"%s\"",tmp);/*need "..."*/
		sprintf(buf,WLAN_SCRIPT_PREFIX , wlan_id, "SSID", tmp2);
		fputs(buf,fp);
	}
  	fclose(fp);
 	chmod(WLAN_MCS_SCRIPT_PATH,S_IRUSR|S_IWUSR|S_IXUSR);

  	return SUCCESS;
}
#endif
#if 0
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
	int i=0,filter_on=0;
	char active[4]={0}, isp[4]={0}, mtu[6]={0}, mss[6]={0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], WAN);
	/*	in this case must load filter module:
		mss is not equal 0 in ppp mode | mtu  is not equal 0 in dynamic and static mode | firewall is active.
	*/
	for(i = 0 ; i < PVC_NUM; i++){
		/*nodeName[0]="Wan", nodeName[1]="PVCn", nodeName[2]=""*/
		sprintf(nodeName[1], SUB_PVC_NAME,i);
		if((getAttrValue(top, nodeName, NODE_ACTIVE, active) != TCAPI_PROCESS_OK)
		||(getAttrValue(top, nodeName, WAN_ISP, isp) != TCAPI_PROCESS_OK)){
			continue;
		}
		getAttrValue(top, nodeName, "MTU", mtu);
		getAttrValue(top, nodeName, "MSS", mss);
		/*must active and isp is not bridge mode*/
		if((!strcmp(active,ACTIVE)) && (atoi(isp) != WAN_ENCAP_BRIDGE)){
			/*check mtu is default value or not*/
			if((atoi(isp) == WAN_ENCAP_DYN) || (atoi(isp) == WAN_ENCAP_STATIC)){
#if 0
				if((atoi(mtu) == 0) || (atoi(mtu) == 1500)){
					continue;
				}else{
#endif
					filter_on = 1;
					break;
#if 0
				}
#endif
			}else if(atoi(isp) == WAN_ENCAP_PPP){
				/*check mss is default value or not*/
				/*the filter must on no matter mss is default or not in ppp mode
				krammer 20081024*/
				#if 0
				if((atoi(mss) == 0) || (atoi(mss) == 1492)){
					continue;
				}else{
				#endif
					filter_on = 1;
					break;
				#if 0
				}
				#endif
			}else{
				continue;
			}
		}else{
			continue;
		}
	}
	return filter_on;
}
#endif
/*check if wireless mode is 11n*/
#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7601E) || defined(MT7592)  
int
Is11nWirelessMode(mxml_node_t *top){
	int is11Nmode=0;
  	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], tmp[160];
	
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],WLAN);
	strcpy(nodeName[1],"Common");

	if(getAttrValue(top, nodeName, "WirelessMode", tmp) == TCAPI_PROCESS_OK){
		if((atoi(tmp)==PHY_MODE_N_ONLY)||(atoi(tmp)==PHY_MODE_GN_MIXED)||(atoi(tmp)==PHY_MODE_BGN_MIXED)){
			is11Nmode=1;
		}
	}
	
	return is11Nmode;
}
#endif
/*check if wmm enabled, txburst should disable when wmm enable.*/
int
txBurst_or_not(mxml_node_t *top, int BssidNum){
	int i=0, txBurst=1;
  	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], tmp[160];
	
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],WLAN);
	for(i=0; i<BssidNum; i++){
		sprintf(nodeName[1],"%s%d",SUB_NODE_NAME,i);
		if(getAttrValue(top, nodeName, "WMM", tmp) == TCAPI_PROCESS_OK){
			if(strcmp(tmp, "1") == 0){
				txBurst=0;
				break;
			}
		}
	}
	return txBurst;
}
#ifdef WSC_AP_SUPPORT
int
wps_genpincode(mxml_node_t *top){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], tmp[4]={0};
	FILE *fp;
#ifdef BBUTOWBU
	char cmd[50]={0};
#endif
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],"Info");
	strcpy(nodeName[1],WLAN);
	if(getAttrValue(top, nodeName, "WPSGenPinCode", tmp) == TCAPI_PROCESS_OK){
		if(strcmp(tmp, "0") == 0){/* no active */
			tcdbg_printf("WPSGenPinCode = 0\n");
		}
		else{ 
			memset(nodeName,0,sizeof(nodeName));
			strcpy(nodeName[0],WLAN);
			strcpy(nodeName[1],"Entry0");
			if(getAttrValue(top, nodeName, "WPSConfMode", tmp) != TCAPI_PROCESS_OK){
				tcdbg_printf("WPSConfMode = NULL\n");
			}
			else if((strcmp(tmp, "1") == 0)||(strcmp(tmp, "2") == 0)||(strcmp(tmp, "7") == 0)){
				fp = fopen(WLAN_SCRIPT_PATH,"w");
				if(fp != NULL){
					if(isWPSRunning(top)){
						fprintf(fp, "/userfs/bin/iwpriv ra0 set WscStop\n");
#ifdef BBUTOWBU
						sprintf(cmd, "echo %d > /proc/tc3162/wps_button\n", WPSSTOP);
						system(cmd);
#endif
					}else{
#if !defined(TCSUPPORT_CD_NEW_GUI) 
						fprintf(fp, "/userfs/bin/iwpriv ra0 set WscGenPinCode=1\n");
#endif
					}
					fclose(fp);
					chmod(WLAN_SCRIPT_PATH,S_IRUSR|S_IWUSR|S_IXUSR);
					return 1;
				}else{
					tcdbg_printf("can't open file in %s\n", __FUNCTION__);
				}
			}else{
				tcdbg_printf("WPSConfMode is wrong!!!\n");
			}
		}
	}else{
		tcdbg_printf("WPSGenPinCode = NULL\n");
	}
	return 0;
}
#endif

#ifdef WSC_AP_SUPPORT//add by xyyou	
int
isWPSRunning(mxml_node_t * top){
  	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], tmp[160];
	int isWPSrunning=0;

#ifdef TCSUPPORT_WLAN_MULTI_WPS
	char APOn[4]={0}, BssidNum[4]={0}, tmp2[128]={0};
	int i;
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],WLAN);
	strcpy(nodeName[1],"Common");
	if((getAttrValue(top, nodeName, "APOn", APOn) != TCAPI_PROCESS_OK)
		|| (getAttrValue(top, nodeName, "BssidNum", BssidNum) != TCAPI_PROCESS_OK)){
		return isWPSrunning;
	}
	
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],"Info");
	strcpy(nodeName[1],"WLan");
	//tcapi_get("Info_WLan", "wlanWPStimerRunning", tmp);
	
	for(i=0;i<atoi(BssidNum);i++){		
		sprintf(tmp2, "wlanWPStimerRunning_%d", i);					
		if(getAttrValue(top, nodeName, tmp2, tmp) == TCAPI_PROCESS_OK){
			if(atoi(tmp)==1){				
				isWPSrunning=1;
				break;
			}
		}
	}
#else	
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],"Info");
	strcpy(nodeName[1],"WLan");

	//tcapi_get("Info_WLan", "wlanWPStimerRunning", tmp);
	
	if(getAttrValue(top, nodeName, "wlanWPStimerRunning", tmp) == TCAPI_PROCESS_OK){
		if(atoi(tmp)==1){
			isWPSrunning=1;
		}
	}	
#endif

	return isWPSrunning;

}
int
run_wps(mxml_node_t *top){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], tmp[4]={0};
#ifdef BBUTOWBU
		char cmd[50]={0};
#endif
	
	FILE *fp;
#ifdef TCSUPPORT_WLAN_MULTI_WPS
	char APOn[4]={0}, BssidNum[4]={0};
	int i;
	int wlan_id;
	memset(nodeName,0,sizeof(nodeName));
  	strcpy(nodeName[0],WEBCURSET);
  	strcpy(nodeName[1],SUB_NODE_NAME);
  	if(getAttrValue(top, nodeName, "wlan_id", tmp) == TCAPI_PROCESS_OK){
    		wlan_id = atoi(tmp);
  	}else{
    		return 0;
 	 }
	
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],WLAN);
	strcpy(nodeName[1],"Common");
	if((getAttrValue(top, nodeName, "APOn", APOn) != TCAPI_PROCESS_OK)
		|| (getAttrValue(top, nodeName, "BssidNum", BssidNum) != TCAPI_PROCESS_OK)){
		return 0;
	}
		
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],"Info");
	strcpy(nodeName[1],WLAN);
	if(getAttrValue(top, nodeName, "WPSActiveStatus", tmp) == TCAPI_PROCESS_OK){
		if(strcmp(tmp, "0") == 0){/* no active */
			tcdbg_printf("WPSActiveStatus = 0\n");
		}
		else{ 
			memset(nodeName,0,sizeof(nodeName));
			strcpy(nodeName[0],WLAN);
			sprintf(nodeName[1], "Entry%d", wlan_id);					
			if(getAttrValue(top, nodeName, "WPSConfMode", tmp) != TCAPI_PROCESS_OK){
				tcdbg_printf("WPSConfMode = NULL\n");
			}
			else if((strcmp(tmp, "1") == 0)||(strcmp(tmp, "2") == 0)||(strcmp(tmp, "7") == 0)){
				fp = fopen(WLAN_SCRIPT_PATH,"w");
				if(fp != NULL){
					
						if(isWPSRunning(top)){	
							
							for(i=0;i<atoi(BssidNum);i++){		
								fprintf(fp, "/userfs/bin/iwpriv ra%d set WscStop\n",i);	
							}
#ifdef BBUTOWBU
													sprintf(cmd, "echo %d > /proc/tc3162/wps_button\n", WPSSTOP);
													system(cmd);
#endif
							
							//fclose(fp);
							//chmod(WLAN_SCRIPT_PATH,S_IRUSR|S_IWUSR|S_IXUSR);
							//return 2;
							//setAttrValue(top, nodeName, "WPSActiveStatus","0");
						}else{
							if(getAttrValue(top, nodeName, "WPSConfMode", tmp) == TCAPI_PROCESS_OK){
								fprintf(fp, WLAN_SCRIPT_PREFIX, wlan_id, "WscConfMode", tmp);/* set wsc conf mode */
							}
							
							if(getAttrValue(top, nodeName, "WPSConfStatus", tmp) == TCAPI_PROCESS_OK){
								if(strcmp(tmp, "1") == 0){
									fprintf(fp, WLAN_SCRIPT_PREFIX, wlan_id, "WscConfStatus", "1");
								}else if(strcmp(tmp, "2") == 0){
									fprintf(fp, WLAN_SCRIPT_PREFIX, wlan_id, "WscConfStatus", "2");
								}else{
									tcdbg_printf("error in WscConfStatus!!!\n\n");
									fclose(fp);
									return 0;
								}
							}
							else{
								fprintf(fp, WLAN_SCRIPT_PREFIX, wlan_id, "WscConfStatus", "1");/*unconfigured*/
							}
							if(getAttrValue(top, nodeName, "WPSMode", tmp) == TCAPI_PROCESS_OK){
								if(strcmp(tmp, "0") == 0){/* Pin code */
									fprintf(fp, WLAN_SCRIPT_PREFIX, wlan_id, "WscMode", "1");
									if(getAttrValue(top, nodeName, "enrolleePinCode", tmp) == TCAPI_PROCESS_OK){
										fprintf(fp, WLAN_SCRIPT_PREFIX, wlan_id, "WscPinCode", tmp);
									}
									
									/*
											set genPinCode
									*/
									if(getAttrValue(top, nodeName, "genPinCodeSupport", tmp) == TCAPI_PROCESS_OK)
									{
											if (strcmp(tmp, "Yes")==0)
													fprintf(fp, WLAN_SCRIPT_PREFIX, wlan_id, "WscGenPinCode", tmp);
									}		
									
								}
								else if(strcmp(tmp, "1") == 0){/* PBC */
									fprintf(fp, WLAN_SCRIPT_PREFIX, wlan_id, "WscMode", "2");
								}
							}
							fprintf(fp, WLAN_SCRIPT_PREFIX, wlan_id, "WscGetConf", "1");/* start WPS */
#ifdef BBUTOWBU
							iswpsstart=1;
#endif	
					}
					
					fclose(fp);
					chmod(WLAN_SCRIPT_PATH,S_IRUSR|S_IWUSR|S_IXUSR);
					return 1;/* wlan execute will only run wps*/
				}
				else{
					tcdbg_printf("can't open file \n");
				}
			}
			else{
				tcdbg_printf("WPSConfMode is wrong!!!\n");
			}
		}
	}
	else{
		tcdbg_printf("WPSActiveStatus = NULL\n");
	}
#else
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],"Info");
	strcpy(nodeName[1],WLAN);
	if(getAttrValue(top, nodeName, "WPSActiveStatus", tmp) == TCAPI_PROCESS_OK){
		if(strcmp(tmp, "0") == 0){/* no active */
			tcdbg_printf("WPSActiveStatus = 0\n");
		}
		else{ 
			memset(nodeName,0,sizeof(nodeName));
			strcpy(nodeName[0],WLAN);
			strcpy(nodeName[1],"Entry0");
			if(getAttrValue(top, nodeName, "WPSConfMode", tmp) != TCAPI_PROCESS_OK){
				tcdbg_printf("WPSConfMode = NULL\n");
			}
			else if((strcmp(tmp, "1") == 0)||(strcmp(tmp, "2") == 0)||(strcmp(tmp, "7") == 0)){
				fp = fopen(WLAN_SCRIPT_PATH,"w");
				if(fp != NULL){
					
						if(isWPSRunning(top)){
							fprintf(fp, "/userfs/bin/iwpriv ra0 set WscStop\n");
#ifdef BBUTOWBU
													sprintf(cmd, "echo %d > /proc/tc3162/wps_button\n", WPSSTOP);
													system(cmd);
#endif
							
							//fclose(fp);
							//chmod(WLAN_SCRIPT_PATH,S_IRUSR|S_IWUSR|S_IXUSR);
							//return 2;
							//setAttrValue(top, nodeName, "WPSActiveStatus","0");
						}else{
							if(getAttrValue(top, nodeName, "WPSConfMode", tmp) == TCAPI_PROCESS_OK){
								fprintf(fp, WLAN_SCRIPT_PREFIX, 0, "WscConfMode", tmp);/* set wsc conf mode */
							}
							
							if(getAttrValue(top, nodeName, "WPSConfStatus", tmp) == TCAPI_PROCESS_OK){
								if(strcmp(tmp, "1") == 0){
									fprintf(fp, WLAN_SCRIPT_PREFIX, 0, "WscConfStatus", "1");
								}else if(strcmp(tmp, "2") == 0){
									fprintf(fp, WLAN_SCRIPT_PREFIX, 0, "WscConfStatus", "2");
								}else{
									tcdbg_printf("error in WscConfStatus!!!\n\n");
									fclose(fp);
									return 0;
								}
							}
							else{
								fprintf(fp, WLAN_SCRIPT_PREFIX, 0, "WscConfStatus", "1");/*unconfigured*/
							}
							if(getAttrValue(top, nodeName, "WPSMode", tmp) == TCAPI_PROCESS_OK){
								if(strcmp(tmp, "0") == 0){/* Pin code */
									fprintf(fp, WLAN_SCRIPT_PREFIX, 0, "WscMode", "1");
									if(getAttrValue(top, nodeName, "enrolleePinCode", tmp) == TCAPI_PROCESS_OK){
										fprintf(fp, WLAN_SCRIPT_PREFIX, 0, "WscPinCode", tmp);
									}
									
									/*
											set genPinCode
									*/
									if(getAttrValue(top, nodeName, "genPinCodeSupport", tmp) == TCAPI_PROCESS_OK)
									{
											if (strcmp(tmp, "Yes")==0)
													fprintf(fp, WLAN_SCRIPT_PREFIX, 0, "WscGenPinCode", tmp);
									}		
									
								}
								else if(strcmp(tmp, "1") == 0){/* PBC */
									fprintf(fp, WLAN_SCRIPT_PREFIX, 0, "WscMode", "2");
								}
							}
							fprintf(fp, WLAN_SCRIPT_PREFIX, 0, "WscGetConf", "1");/* start WPS */
#ifdef BBUTOWBU
							iswpsstart=1;
#endif	
					}
					
					fclose(fp);
					chmod(WLAN_SCRIPT_PATH,S_IRUSR|S_IWUSR|S_IXUSR);
					return 1;/* wlan execute will only run wps*/
				}
				else{
					tcdbg_printf("can't open file \n");
				}
			}
			else{
				tcdbg_printf("WPSConfMode is wrong!!!\n");
			}
		}
	}
	else{
		tcdbg_printf("WPSActiveStatus = NULL\n");
	}
#endif	
	return 0;
}
int
wps_oob(mxml_node_t *top){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], tmp[4]={0};
	FILE *fp;
#ifdef BBUTOWBU
		char cmd[50]={0};
#endif
	
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],"Info");
	strcpy(nodeName[1],WLAN);
	if(getAttrValue(top, nodeName, "WPSOOBActive", tmp) == TCAPI_PROCESS_OK){
		if(strcmp(tmp, "0") == 0){/* no active */
			tcdbg_printf("WPSOOBActive = 0\n");
		}
		else{ 
			memset(nodeName,0,sizeof(nodeName));
			strcpy(nodeName[0],WLAN);
			strcpy(nodeName[1],"Entry0");
			if(getAttrValue(top, nodeName, "WPSConfMode", tmp) != TCAPI_PROCESS_OK){
				tcdbg_printf("WPSConfMode = NULL\n");
			}
			else if((strcmp(tmp, "1") == 0)||(strcmp(tmp, "2") == 0)||(strcmp(tmp, "7") == 0)){
				fp = fopen(WLAN_SCRIPT_PATH,"w");
				if(fp != NULL){
					if(isWPSRunning(top)){
						fprintf(fp, "/userfs/bin/iwpriv ra0 set WscStop\n");
#ifdef BBUTOWBU
												sprintf(cmd, "echo %d > /proc/tc3162/wps_button\n", WPSSTOP);
												system(cmd);
#endif
						
					}else{
#ifdef BBUTOWBU
												fprintf(fp, "/userfs/bin/iwpriv ra0 set WscOOB=1\n");
#else
												fprintf(fp, "/userfs/bin/iwpriv ra0 set WscOOB\n");
												//tcdbg_printf("xyyou:set WscOOBSeted = 1 in wps_oob\n");
#endif						
						tcdbg_printf("xyyou:set WscOOBSeted = 1 in wps_oob\n");
						WscOOBSeted=1;
					}
					fclose(fp);
					chmod(WLAN_SCRIPT_PATH,S_IRUSR|S_IWUSR|S_IXUSR);
					return 1;
				}else{
					tcdbg_printf("can't open file in wps_oob()\n");
				}
			}else{
				tcdbg_printf("WPSConfMode is wrong!!!\n");
			}
		}
	}else{
		tcdbg_printf("WPSOOBActive = NULL\n");
	}
	return 0;
}
#endif//WSC_AP_SUPPORT	
#ifdef TCSUPPORT_WLAN_AC
/*______________________________________________________________________________
**	function name:write_wlan11ac_config
**
**	description:
*     create wireless configure file
**	parameters:
*     top:xml top node address
**	global:
*     none
**	return:
*       0:success
*	  1:fail
**	call:
*     none
**	revision:
*     1.evan 20130801
**____________________________________________________________________________*/
int
write_wlan11ac_config(mxml_node_t *top, int BssidNum){
  	int i, j;
  	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], tmp[160]
		, tmp2[160],buf[160],strRate[32] = {0},strWirelessMode[8] = {0};
  	FILE *fp;
#if defined(TCSUPPORT_WLAN_8021X_EXT)
	struct hostent *hp = NULL;
	struct sockaddr_in RadiusAddr;
#endif

	char wirelessmode[16], encrytype[16];
  	char wlan_common_default_parm[][3][ATTR_SIZE]=
	{
		{"BGProtection","0","1"},
		{"TxBurst","1","1"},
		{"PktAggregate","0","1"},
		{"TurboRate","0","0"},
		{"NoForwardingBTNBSSID","0","0"},
		{"ShortSlot","1","1"},
		{"PMKCachePeriod","10","0"},
		{"APAifsn","3;7;1;1","1"},
		{"APCwmin","4;4;3;2","1"},
		{"APCwmax","6;10;4;3","1"},
		{"APTxop","0;0;94;47","1"},
		{"APACM","0;0;0;0","1"},
		{"BSSAifsn","3;7;2;2","1"},
		{"BSSCwmin","4;4;3;2","1"},
		{"BSSCwmax","10;10;4;3","1"},
		{"BSSTxop","0;0;94;47","1"},
		{"BSSACM","0;0;0;0","1"},
		{"AckPolicy","0;0;0;0","1"},
	#if defined(RT3390) || defined(RT3090) || defined(RT5392)  || defined(MT7610E) || defined(MT7612E) /*add by xyyou */
		{"APSDCapable","0","1"},
		{"DLSCapable","0","0"},
		{"WirelessEvent","0","0"},
		{"HT_HTC","0","0"},
		{"HT_LinkAdapt","0","0"},
		{"HT_AMSDU","0","1"},
		{"HT_STBC","1","1"},	
		{"DisableOLBC","0","0"},
		{"HT_AutoBA","1","1"},
		{"HT_RDG","1","1"},/*xyyou???*/
		{"HT_MpduDensity","5","0"},
		{"HT_BAWinSize","64","0"},
		{"HT_BSSCoexistence","1","1"},
		{"EAPifname","br0","0"},
		{"PreAuthifname","br0","0"},
	#endif/*end xyyou*/
#if defined(MT7612E)
		{"HT_LDPC","1","1"},
		{"VHT_LDPC","1","1"},
#endif
#if defined(MT7610E) || defined(MT7612E)
		{"VHT_STBC","0","1"}, /* add by evan*/
		{"VHT_BW_SIGNAL","0","1"},
		{"VHT_DisallowNonVHT","0","1"},
#endif
		{"HT_BADecline","0","1"},
		{"HT_DisallowTKIP","1","1"},
#if defined(MT7612E)
		{"HT_TxStream","2","1"},
		{"HT_RxStream","2","1"},
#else
		{"HT_TxStream","1","1"},
		{"HT_RxStream","1","1"},
#endif
		{"IEEE80211H","0","1"},
		#if defined(MT7610E)
		{"EDCCA_ED_TH","85","1"},
		#elif defined(MT7612E)
		{"EDCCA_ED_TH","90","1"},
		#endif
		{"EDCCA_FALSE_CCA_TH","180","1"},
		{"EDCCA_BLOCK_CHECK_TH","2","1"},
		{"ED_LEARN_TH","50","1"},
		{"ED_MODE","1","1"},
		{"","",""},
	};
  	char wlan_entry_default_parm[][3][ATTR_SIZE]=
	{
		/*AttrName,DefaultValue,Flag(1:SetToAttr)*/
		{"TxRate","0","0"},
		{"NoForwarding","0","1"},
		{"PreAuth","0","0"},
		#if defined(MT7612E)
		{"PMFMFPC","0","1"},
		{"PMFMFPR","0","1"},
		{"PMFSHA256","0","1"},
	#endif
		{"","",""},
	};
  	char wlan_script_com_attr[][ATTR_SIZE]=
	{
		{"CountryRegion"},
		{"CountryRegionABand"},
		{"AutoChannelSelect"},
		{"Channel"},
		/*{"BssidNum"},*/
		{"BssidNum"},
		{"BeaconPeriod"},
		{"RTSThreshold"},
		{"FragThreshold"},
		{"DtimPeriod"},
		{"TxPower"},
		{"TxPreamble"},
		/*{"WirelessMode"},*/
		{"BasicRate"},
	#if defined(RT3390) || defined(RT3090) || defined(RT5392)  || defined(MT7610E) || defined(MT7612E) /*add by xyyou*/
		{"HT_GI"},
		{"HT_BW"},
		{"HT_EXTCHA"},
		{"HT_OpMode"},
		#ifdef TCSUPPORT_MTD_ENCHANCEMENT
		{"EfuseBufferMode"},
		#endif
	#ifdef IGMP_SNOOP_SUPPORT/*add by xyyou*/
	#if defined(RT5392) || defined(MT7610E) || defined(WLAN_MULTIDRIVER) || defined(MT7612E) 
		{"IgmpSnEnable"},
	#endif
	#endif
	#endif /*end xyyou*/
#if defined(TCSUPPORT_WLAN_8021X)
		{"session_timeout_interval"},
		{"IdleTimeout"},
#endif
#if defined(MT7610E) || defined(MT7612E)
		{"VHT_BW"}, /*add by evan*/
		{"VHT_SGI"},
#endif		
		{""}
	};
	enum wlan_script_entry_attr_index{
		ssid=0,
		hidessid,
		authmod,
		encyptype,
		defaultkeyid,
		Key1Type,
		key1str,
		Key2Type, 
		key2str, 
		Key3Type, 
		key3str, 
		Key4Type, 
		key4str, 
		wpapsk,
		wmm,
#if defined(TCSUPPORT_WLAN_MAXSTANUM_GUI)
		MaxStaNum,
#endif
		#if defined(RT3390) || defined(RT3090) || defined(RT5392)  || defined(MT7610E) || defined(MT7612E) /*add by xyyou*/
		mcs,
		fixedTxMode,
		#endif
		#ifdef WSC_AP_SUPPORT/*add by xyyou*/
		wsconfmode,
		wscconfstatus,
		wsckeyascii,
		wscv2support, /*added by fredli*/
		WscMaxPinAttack,
		WscSetupLockTime,
		#endif
		#ifdef IGMP_SNOOP_SUPPORT/*add by xyyou*/
		#if !defined(RT5392) && !defined(MT7610E) && !defined(MT7612E)
		igmpsnooping,
		#endif
		#endif
		keypassphrase,
		RekeyInterval,/*should support PER SSID*/
		RekeyMethod,
#if defined(TCSUPPORT_WLAN_8021X)
		radius_server,
		radius_port,
		radius_key,
#if defined(TCSUPPORT_WLAN_8021X_EXT)
		bak_radius_server,
		bak_radius_port,
		bak_radius_key,
#endif
		ieee8021x,
#endif
		wlan_entry_attr_len

	};

	char wlan_script_entry_attr_buf[wlan_entry_attr_len][270];

	char wlan_script_entry_attr_para[wlan_entry_attr_len][ATTR_SIZE]=
	{
		{"SSID"},
		{"HideSSID"},
		{"AuthMode"},
		{"EncrypType"},
		{"DefaultKeyID"},
		{"Key1Type"},
		{"Key1Str"},
		{"Key2Type"},
		{"Key2Str"},
		{"Key3Type"},
		{"Key3Str"},
		{"Key4Type"},
		{"Key4Str"},
		{"WPAPSK"},
		{"WmmCapable"},
#if defined(TCSUPPORT_WLAN_MAXSTANUM_GUI)
		{"MaxStaNum"},
#endif
	#if defined(RT3390) || defined(RT3090) || defined(RT5392)  || defined(MT7610E) || defined(MT7612E) /*add by xyyou*/
		{"HT_MCS"},	
		{"FixedTxMode"},
	#endif /*end xyyou*/
	#ifdef WSC_AP_SUPPORT/*add by xyyou*/
		{"WscConfMode"},	
		{"WscConfStatus"},	
		{"WscKeyASCII"},	
		{"WscV2Support"}, /*added by fredli*/
		{"WscMaxPinAttack"},
		{"WscSetupLockTime"},
	#endif
	#ifdef IGMP_SNOOP_SUPPORT/*add by xyyou*/
		/*just for 3390&3090, 5392 used as common attribute*/
#if (defined(RT3390) || defined(RT3090)) && !defined(WLAN_MULTIDRIVER)
		{"IgmpSnEnable"},
	#endif
	#endif
		{"KeyPassphrase"},
		/*this value also used in WPAPSK WPA2PSK WPAPSKWPA2PSK mode*/
		{"RekeyInterval"},
		{"RekeyMethod"},
#if defined(TCSUPPORT_WLAN_8021X)
		{"RADIUS_Server"},
		{"RADIUS_Port"},
		{"RADIUS_Key"},
#if defined(TCSUPPORT_WLAN_8021X_EXT)
		{"RADIUS_Server"},
		{"RADIUS_Port"},
		{"RADIUS_Key"},
#endif
		{"IEEE8021X"},
#endif
	};

	fp = fopen(WLAN_AC_PATH,"w");

	if(fp == NULL){
		return FAIL;
	}
	/*write common default setting to RT61.dat*/
	fputs("Default\n",fp);
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],"WLan11ac");
	strcpy(nodeName[1],"Common");
	for(i = 0;strlen(wlan_common_default_parm[i][0]) != 0;i++){
		if((getAttrValue(top, nodeName, wlan_common_default_parm[i][0]
			, tmp) == TCAPI_PROCESS_OK)){
			if(strcmp(wlan_common_default_parm[i][0], "TxBurst") == 0){
				sprintf(buf,"%s=%d\n", wlan_common_default_parm[i][0]
					, txBurst_or_not_ac(top, BssidNum));
			}
			else{
				sprintf(buf,"%s=%s\n", wlan_common_default_parm[i][0],tmp);
			}
			
		}
		else{
			if(strcmp(wlan_common_default_parm[i][0], "TxBurst") == 0){
				sprintf(buf,"%s=%d\n", wlan_common_default_parm[i][0]
					, txBurst_or_not_ac(top, BssidNum));
			}
			else{
				sprintf(buf,"%s=%s\n", wlan_common_default_parm[i][0]
					,wlan_common_default_parm[i][1]);
			}
			if(wlan_common_default_parm[i][2][0]=='1'){
				setAttrValue(top, nodeName, wlan_common_default_parm[i][0]
					, wlan_common_default_parm[i][1]);
			}
		}
		fputs(buf,fp);
	}
	/*write common setting to RT61.dat */
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],"WLan11ac");
	strcpy(nodeName[1],"Common");
	
	if(getAttrValue(top, nodeName, "WirelessMode", strWirelessMode)
		== TCAPI_PROCESS_OK){
		strncpy(wirelessmode, tmp, sizeof(wirelessmode));
		sprintf(buf,"%s=%s\n", "WirelessMode", strWirelessMode);
		fputs(buf,fp);
	#if defined(RT3390) || defined(RT3090) || defined(RT5392)  || defined(MT7610E) || defined(MT7612E) /*add by xyyou*/
		if((atoi(strWirelessMode)==PHY_11VHT_N_A_MIXED) || (atoi(strWirelessMode)
			==PHY_11VHT_N_MIXED)){
			setAttrValue(top, nodeName, "11acMode", "1");
		}else{
			setAttrValue(top, nodeName, "11acMode", "0");
		}
		if((atoi(strWirelessMode)==PHY_11VHT_N_A_MIXED) 
			|| (atoi(strWirelessMode)==PHY_11VHT_N_MIXED)
			|| (atoi(strWirelessMode)==PHY_MODE_11AN_MIXED)){
			setAttrValue(top, nodeName, "11nMode", "1");
		}else{
			setAttrValue(top, nodeName, "11nMode", "0");
		}
	#endif
	}
	
	for(i = 0;strlen(wlan_script_com_attr[i]) != 0;i++){
		if(strcmp(wlan_script_com_attr[i], "AutoChannelSelect") == 0){
			if(getAttrValue(top, nodeName, "Channel", tmp) == TCAPI_PROCESS_OK){
				if(atoi(tmp)==0){
					sprintf(buf,"%s=%s\n", wlan_script_com_attr[i],"2");
				}else{
					sprintf(buf,"%s=%s\n", wlan_script_com_attr[i],"0");
				}
			}
		}
#if defined(RT3390) || defined(RT3090) || defined(RT5392)  || defined(MT7610E) || defined(MT7612E)
		else if(strcmp(wlan_script_com_attr[i], "HT_BW") == 0){
			switch(atoi(strWirelessMode)){
				case PHY_11VHT_N_A_MIXED:
				case PHY_11VHT_N_MIXED:
				case PHY_MODE_11AN_MIXED:
					if(getAttrValue(top, nodeName
						, (char *)wlan_script_com_attr[i]
						, tmp) == TCAPI_PROCESS_OK){
						sprintf(buf,"%s=%s\n", wlan_script_com_attr[i],tmp);
					}
					break;
				case PHY_MODE_11A_ONLY:
					sprintf(buf,"%s=%s\n", wlan_script_com_attr[i],"0");
					break;
				default:
					tcdbg_printf("write_wlan11ac_config:WLan wireless"
						" mode is not right when set HT_BW!\n");
					fclose(fp);
					return FAIL;
			}	
		}
		else if(strcmp(wlan_script_com_attr[i], "VHT_BW") == 0){
			if(Is11acWirelessMode(top)){
				if(getAttrValue(top, nodeName, "HT_BW", tmp)
					== TCAPI_PROCESS_OK){
					if (atoi(tmp) == 1) {
						if(getAttrValue(top, nodeName
							, (char *)wlan_script_com_attr[i]
							, tmp) == TCAPI_PROCESS_OK){
							sprintf(buf,"%s=%s\n", wlan_script_com_attr[i],tmp);
						}
					}
					else {
						sprintf(buf,"%s=%s\n", wlan_script_com_attr[i],"0");											
					}
				}
			}else{
				sprintf(buf,"%s=%s\n", wlan_script_com_attr[i],"0");
			}
		}
		else if(strcmp(wlan_script_com_attr[i], "VHT_SGI") == 0){
			if(Is11acWirelessMode(top)){
				if(getAttrValue(top, nodeName, "HT_BW", tmp)
					== TCAPI_PROCESS_OK){
					if (atoi(tmp) == 1) {
						if(getAttrValue(top, nodeName
							, (char *)wlan_script_com_attr[i]
							, tmp) == TCAPI_PROCESS_OK){
							sprintf(buf,"%s=%s\n", wlan_script_com_attr[i],tmp);
						}
					}
					else {
						sprintf(buf,"%s=%s\n", wlan_script_com_attr[i],"1");											
					}
				}
			}else{
				sprintf(buf,"%s=%s\n", wlan_script_com_attr[i],"1");
			}
		}
		else if(strcmp(wlan_script_com_attr[i], "HT_OpMode") == 0){		
			sprintf(buf,"%s=%s\n", wlan_script_com_attr[i],"0");
		}
#endif		
		else{
			if(getAttrValue(top, nodeName
				, (char *)wlan_script_com_attr[i]
				, tmp) == TCAPI_PROCESS_OK){
				if(strlen(tmp) > 0){
					sprintf(buf,"%s=%s\n", wlan_script_com_attr[i],tmp);
				}
			}
		}
		fputs(buf,fp);
	 }

	/*write entry default setting to RT61.dat, ex: SSID=SSID-A;SSID-B;SSID-C;SSID-D*/
	for(i = 0;strlen(wlan_entry_default_parm[i][0]) != 0;i++){
		sprintf(buf,"%s=",wlan_entry_default_parm[i][0]);
		for(j=0; j < BssidNum; j++){
			sprintf(nodeName[1],"%s%d",SUB_NODE_NAME,j);
 			if(j == BssidNum-1){/*last entry do not need ";"*/
				if((getAttrValue(top, nodeName, wlan_entry_default_parm[i][0], tmp) == TCAPI_PROCESS_OK)){
					strcat(buf,tmp);
				}
				else{
					strcat(buf,wlan_entry_default_parm[i][1]);
					if(wlan_entry_default_parm[i][2][0]=='1'){
						setAttrValue(top, nodeName, wlan_entry_default_parm[i][0], wlan_entry_default_parm[i][1]);
					}
				}
			}else{
				if((getAttrValue(top, nodeName, wlan_entry_default_parm[i][0], tmp2) == TCAPI_PROCESS_OK)){
					sprintf(tmp,"%s;",tmp2);
					strcat(buf,tmp);
				}
				else{
					sprintf(tmp,"%s;",wlan_entry_default_parm[i][1]);
					strcat(buf,tmp);
					if(wlan_entry_default_parm[i][2][0]=='1'){
						setAttrValue(top, nodeName, wlan_entry_default_parm[i][0], wlan_entry_default_parm[i][1]);
					}
				}
			}
		}
		strcat(buf,"\n");
		fputs(buf,fp);
	}
	/*write entry setting to RT61.dat*/
	memset(wlan_script_entry_attr_buf, 0,sizeof(wlan_script_entry_attr_buf));
	/*SSID=SSID-A;SSID-B;SSID-C;SSID-D*/
	for(j=0; j < BssidNum; j++){
		sprintf(nodeName[1],"%s%d",SUB_NODE_NAME,j);
		if(getAttrValue(top, nodeName, "SSID", tmp) == TCAPI_PROCESS_OK){
			strcat(wlan_script_entry_attr_buf[ssid],tmp);
			strcat(wlan_script_entry_attr_buf[ssid],";");
		}
		if(getAttrValue(top, nodeName, "HideSSID", tmp) == TCAPI_PROCESS_OK){
			strcat(wlan_script_entry_attr_buf[hidessid],tmp);
			strcat(wlan_script_entry_attr_buf[hidessid],";");
		}
		if(getAttrValue(top, nodeName, "AuthMode", tmp) == TCAPI_PROCESS_OK){
			if(strstr(tmp, "WEP")){/*WEP64-bits or WEP128-bits*/
#if defined(TCSUPPORT_WLAN_8021X)
				if(strstr(tmp, "Radius")){/*Radius-WEP64, Radius-WEP128*/
					strcat(wlan_script_entry_attr_buf[ieee8021x],"1;");
				}else{
					strcat(wlan_script_entry_attr_buf[ieee8021x],"0;");
				}
#endif	
				if(getAttrValue(top, nodeName, "WEPAuthType"
					, tmp) == TCAPI_PROCESS_OK){
					if(!strcmp(tmp, "OpenSystem")){
						strcat(wlan_script_entry_attr_buf[authmod],"OPEN;");
					}else if(!strcmp(tmp, "SharedKey")){
						strcat(wlan_script_entry_attr_buf[authmod],"SHARED;");
					}else{
						strcat(wlan_script_entry_attr_buf[authmod],"WEPAUTO;");
					}
				}
				//strcat(wlan_script_entry_attr_buf[authmod],"WEPAUTO;");
				strcat(wlan_script_entry_attr_buf[encyptype],"WEP;");
				if(getAttrValue(top, nodeName, "DefaultKeyID"
					, tmp) != TCAPI_PROCESS_OK){
					strcpy(tmp,"");
				}
				strcat(wlan_script_entry_attr_buf[defaultkeyid],tmp);
				if ( strlen(tmp) == 0 ) {
					strcat(wlan_script_entry_attr_buf[defaultkeyid],"0;");
				}
				else {
					strcat(wlan_script_entry_attr_buf[defaultkeyid],";");
				}
				for(i=1;i<=4;i++){/*4 key string*/
					sprintf(tmp2,"Key%dStr",i);
					if(getAttrValue(top, nodeName, tmp2, tmp)
						!= TCAPI_PROCESS_OK){
						strcpy(tmp,"");
					}
					/*krammer change for bug 1352*/
					if(strlen(tmp)==10 || strlen(tmp)==26||strlen(tmp)==0){
						strcat(wlan_script_entry_attr_buf[defaultkeyid +i*2-1]
							,"0;");
					}
					else{
						strcat(wlan_script_entry_attr_buf[defaultkeyid +i*2-1]
							,"1;");
					}
					strcat(wlan_script_entry_attr_buf[defaultkeyid +i*2],tmp);
					
					if ( strlen(tmp) == 0 ) {
						strcat(wlan_script_entry_attr_buf[defaultkeyid +i*2]
							,"0;");
					}
					else {
						strcat(wlan_script_entry_attr_buf[defaultkeyid +i*2]
							,";");
					}
					#if 0
					strcat(wlan_script_entry_attr_buf[defaultkeyid +i],";");
					#endif
				}
				if(getAttrValue(top, nodeName, "KeyPassphrase", tmp) 
					== TCAPI_PROCESS_OK){
					strcat(wlan_script_entry_attr_buf[keypassphrase],tmp);
					strcat(wlan_script_entry_attr_buf[keypassphrase],";");
				}
				strcat(wlan_script_entry_attr_buf[wpapsk],"0;");
			}
			else if(!strcmp(tmp, "WPAPSK")||!strcmp(tmp, "WPA2PSK")
				||!strcmp(tmp, "WPAPSKWPA2PSK")
#if defined(TCSUPPORT_WLAN_8021X)
				||!strcmp(tmp, "WPA")||!strcmp(tmp, "WPA2")
				||!strcmp(tmp, "WPA1WPA2")
#endif
			){/*WPAPSK or WPA2PSK or Mixed mode*/
#if defined(TCSUPPORT_WLAN_8021X)
				if(!strcmp(tmp, "WPA")||!strcmp(tmp, "WPA2")
					||!strcmp(tmp, "WPA1WPA2")){/*Radius-WEP64, Radius-WEP128*/
					strcat(wlan_script_entry_attr_buf[ieee8021x],"1;");
				}else{
					strcat(wlan_script_entry_attr_buf[ieee8021x],"0;");
				}
#endif

				strcat(wlan_script_entry_attr_buf[authmod],tmp);
				strcat(wlan_script_entry_attr_buf[authmod],";");
				if(getAttrValue(top, nodeName, "EncrypType"
					, tmp) == TCAPI_PROCESS_OK){
					strcat(wlan_script_entry_attr_buf[encyptype],tmp);
					strcat(wlan_script_entry_attr_buf[encyptype],";");
				}
				strcat(wlan_script_entry_attr_buf[defaultkeyid],"2;");
				if(getAttrValue(top, nodeName, "WPAPSK", tmp)
					!= TCAPI_PROCESS_OK){
					strcpy(tmp,"");
				}
				strcat(wlan_script_entry_attr_buf[wpapsk],tmp);
				/*krammer change for bug 1242*/
				strcat(wlan_script_entry_attr_buf[wpapsk],";");
				
				/*add by xyzhu_nj_20100211*/
				strcat(wlan_script_entry_attr_buf[Key1Type],"0;");
				strcat(wlan_script_entry_attr_buf[Key2Type],"0;");
				strcat(wlan_script_entry_attr_buf[Key3Type],"0;");
				strcat(wlan_script_entry_attr_buf[Key4Type],"0;");
				
				strcat(wlan_script_entry_attr_buf[key1str],"0;");
				strcat(wlan_script_entry_attr_buf[key2str],"0;");
				strcat(wlan_script_entry_attr_buf[key3str],"0;");
				strcat(wlan_script_entry_attr_buf[key4str],"0;");
			}else{/*OPEN*/
#if defined(TCSUPPORT_WLAN_8021X)
				strcat(wlan_script_entry_attr_buf[ieee8021x],"0;");
#endif
				strcat(wlan_script_entry_attr_buf[authmod],"OPEN;");
				strcat(wlan_script_entry_attr_buf[encyptype],"NONE;");
				
				/*add by xyzhu_nj_20100211*/
				strcat(wlan_script_entry_attr_buf[defaultkeyid],"0;");
				strcat(wlan_script_entry_attr_buf[wpapsk],"0;");
				strcat(wlan_script_entry_attr_buf[Key1Type],"0;");
				strcat(wlan_script_entry_attr_buf[Key2Type],"0;");
				strcat(wlan_script_entry_attr_buf[Key3Type],"0;");
				strcat(wlan_script_entry_attr_buf[Key4Type],"0;");
				
				strcat(wlan_script_entry_attr_buf[key1str],"0;");
				strcat(wlan_script_entry_attr_buf[key2str],"0;");
		        	strcat(wlan_script_entry_attr_buf[key3str],"0;");
		        	strcat(wlan_script_entry_attr_buf[key4str],"0;");
			}
		}
		memset(tmp,0,sizeof(tmp));
		if(getAttrValue(top, nodeName, "RekeyInterval"
			, tmp) == TCAPI_PROCESS_OK){
			if(strlen(tmp) > 0){
				strcat(wlan_script_entry_attr_buf[RekeyInterval],tmp);
				strcat(wlan_script_entry_attr_buf[RekeyInterval],";");
				strcat(wlan_script_entry_attr_buf[RekeyMethod],"TIME");
				strcat(wlan_script_entry_attr_buf[RekeyMethod],";");
			}
		}
#if defined(TCSUPPORT_WLAN_MAXSTANUM_GUI)
		memset(tmp,0,sizeof(tmp));
		if((getAttrValue(top, nodeName, "MaxStaNum"
			, tmp) == TCAPI_PROCESS_OK) && (strlen(tmp) > 0)){
			strcat(wlan_script_entry_attr_buf[MaxStaNum],tmp);
			strcat(wlan_script_entry_attr_buf[MaxStaNum],";");
		}
		else{/*add default value for MaxStaNum into Wifi profile*/
			strcpy(tmp,"0");
			strcat(wlan_script_entry_attr_buf[MaxStaNum],tmp);
			strcat(wlan_script_entry_attr_buf[MaxStaNum],";");
		}
#endif
#if defined(TCSUPPORT_WLAN_8021X)
/*RADIUS_Server*/
		memset(tmp,0,sizeof(tmp));
#if defined(TCSUPPORT_WLAN_8021X_EXT)
		if(getAttrValue(top, nodeName, "RADIUS_Server"
			, tmp) == TCAPI_PROCESS_OK){
			if(strlen(tmp) > 0){
				hp = gethostbyname(tmp);
				if(hp != (struct hostent *) NULL){
					memcpy((char *) &RadiusAddr.sin_addr
				, (char *) hp->h_addr_list[0],sizeof(RadiusAddr.sin_addr));
					strcat(wlan_script_entry_attr_buf[radius_server]
				,inet_ntoa(*(struct in_addr *) &RadiusAddr.sin_addr.s_addr));
					strcat(wlan_script_entry_attr_buf[radius_server],";");
				}else{
					strcat(wlan_script_entry_attr_buf[radius_server]
						,"0.0.0.0;");
				}
			}
		}
#else
		if(getAttrValue(top, nodeName, "RADIUS_Server", tmp)
			== TCAPI_PROCESS_OK){
			if(strlen(tmp) > 0){
				strcat(wlan_script_entry_attr_buf[radius_server],tmp);
				strcat(wlan_script_entry_attr_buf[radius_server],";");
			}
		}
#endif
/*RADIUS_Port*/
		memset(tmp,0,sizeof(tmp));
		if(getAttrValue(top, nodeName, "RADIUS_Port", tmp)
			== TCAPI_PROCESS_OK){
			if(strlen(tmp) > 0){
				strcat(wlan_script_entry_attr_buf[radius_port],tmp);
				strcat(wlan_script_entry_attr_buf[radius_port],";");
			}
		}
/*RADIUS_Key*/
		memset(tmp,0,sizeof(tmp));
		if(getAttrValue(top, nodeName, "RADIUS_Key", tmp) == TCAPI_PROCESS_OK){
			if(strlen(tmp) > 0){
				strcat(wlan_script_entry_attr_buf[radius_key],tmp);
				strcat(wlan_script_entry_attr_buf[radius_key],";");
			}
		}
#if defined(TCSUPPORT_WLAN_8021X_EXT)
/*BAK_RADIUS_Server*/
		memset(tmp,0,sizeof(tmp));
		if(getAttrValue(top, nodeName, "BAK_RADIUS_Server"
			, tmp) == TCAPI_PROCESS_OK){
			if(strlen(tmp) > 0){
				hp = gethostbyname(tmp);
				if(hp != (struct hostent *) NULL){
					memcpy((char *) &RadiusAddr.sin_addr
						, (char *) hp->h_addr_list[0]
						,sizeof(RadiusAddr.sin_addr));
					strcat(wlan_script_entry_attr_buf[bak_radius_server]
				,inet_ntoa(*(struct in_addr *) &RadiusAddr.sin_addr.s_addr));
					strcat(wlan_script_entry_attr_buf[bak_radius_server],";");
				}else{
					strcat(wlan_script_entry_attr_buf[bak_radius_server]
						,"0.0.0.0;");
				}
			}
		}
/*BAK_RADIUS_Port*/
		memset(tmp,0,sizeof(tmp));
		if(getAttrValue(top, nodeName, "BAK_RADIUS_Port", tmp) 
			== TCAPI_PROCESS_OK){
			if(strlen(tmp) > 0){
				strcat(wlan_script_entry_attr_buf[bak_radius_port],tmp);
				strcat(wlan_script_entry_attr_buf[bak_radius_port],";");
			}
		}
/*BAK_RADIUS_Key*/
		memset(tmp,0,sizeof(tmp));
		if(getAttrValue(top, nodeName, "BAK_RADIUS_Key", tmp) 
			== TCAPI_PROCESS_OK){
			if(strlen(tmp) > 0){
				strcat(wlan_script_entry_attr_buf[bak_radius_key],tmp);
				strcat(wlan_script_entry_attr_buf[bak_radius_key],";");
			}
		}
#endif
#endif
#if defined(RT3390) || defined(RT3090) || defined(RT5392)  || defined(MT7610E) || defined(MT7612E)
		if(getAttrValue(top, nodeName, "AuthMode", tmp) == TCAPI_PROCESS_OK){
			strncpy(encrytype,tmp,sizeof(encrytype));
		}
		if(getAttrValue(top, nodeName, "HT_MCS", tmp) == TCAPI_PROCESS_OK){
			{
				strcat(wlan_script_entry_attr_buf[mcs],tmp);
				strcat(wlan_script_entry_attr_buf[mcs],";");
			}
		}
		if(Is11nWirelessMode5G(top)){
			strcat(wlan_script_entry_attr_buf[wmm],"1");
			strcat(wlan_script_entry_attr_buf[wmm],";");
		}else{
			if(getAttrValue(top, nodeName, "WMM", tmp) == TCAPI_PROCESS_OK){
				strcat(wlan_script_entry_attr_buf[wmm],tmp);
				strcat(wlan_script_entry_attr_buf[wmm],";");
			}
		}
	
		strcat(wlan_script_entry_attr_buf[fixedTxMode],"0");
		strcat(wlan_script_entry_attr_buf[fixedTxMode],";");
	#else
		if(getAttrValue(top, nodeName, "WMM", tmp) == TCAPI_PROCESS_OK){
			strcat(wlan_script_entry_attr_buf[wmm],tmp);
			strcat(wlan_script_entry_attr_buf[wmm],";");
		}	
	#endif	
	#ifdef WSC_AP_SUPPORT/*add by xyyou*/
		if(getAttrValue(top, nodeName, "WPSConfMode", tmp)
			== TCAPI_PROCESS_OK){
			strcat(wlan_script_entry_attr_buf[wsconfmode],tmp);
			strcat(wlan_script_entry_attr_buf[wsconfmode],";");
		}	
		if(getAttrValue(top, nodeName, "WPSConfStatus", tmp)
			== TCAPI_PROCESS_OK){
			strcat(wlan_script_entry_attr_buf[wscconfstatus],tmp);
			strcat(wlan_script_entry_attr_buf[wscconfstatus],";");
		}
		if(getAttrValue(top, nodeName, "WPSKeyASCII", tmp) == TCAPI_PROCESS_OK){
			strcat(wlan_script_entry_attr_buf[wsckeyascii],tmp);
			strcat(wlan_script_entry_attr_buf[wsckeyascii],";");
		}
		/*added by fredli, for wsc 2.0*/
		if(getAttrValue(top, nodeName, "WscV2Support", tmp) 
			== TCAPI_PROCESS_OK){
			strcat(wlan_script_entry_attr_buf[wscv2support],tmp);
			strcat(wlan_script_entry_attr_buf[wscv2support],";");
		}
		else{/*add default value for WscV2Support into Wifi profile*/
			strcpy(tmp,"0");
			strcat(wlan_script_entry_attr_buf[wscv2support],tmp);
			strcat(wlan_script_entry_attr_buf[wscv2support],";");
		}
		if(getAttrValue(top, nodeName, "WscMaxPinAttack", tmp)
			== TCAPI_PROCESS_OK){
                        strcat(wlan_script_entry_attr_buf[WscMaxPinAttack],tmp);
                        strcat(wlan_script_entry_attr_buf[WscMaxPinAttack],";");
                }
		if(getAttrValue(top, nodeName, "WscSetupLockTime"
			, tmp) == TCAPI_PROCESS_OK){
                   strcat(wlan_script_entry_attr_buf[WscSetupLockTime],tmp);
                   strcat(wlan_script_entry_attr_buf[WscSetupLockTime],";");
                }


		
	#endif
	#ifdef IGMP_SNOOP_SUPPORT/*add by xyyou*/
#if (defined(RT3390) || defined(RT3090)) && !defined(WLAN_MULTIDRIVER)/*just for 3390&3090, 5392 used as common attribute*/
		if(getAttrValue(top, nodeName, "IgmpSnEn", tmp) == TCAPI_PROCESS_OK){
			strcat(wlan_script_entry_attr_buf[igmpsnooping],tmp);
			strcat(wlan_script_entry_attr_buf[igmpsnooping],";");
		}
	#endif
	#endif
	}

	/*write all entry config to file*/
	for(i=ssid;i < wlan_entry_attr_len;i++)
	{
		if(strlen(wlan_script_entry_attr_buf[i]) > 0){
			wlan_script_entry_attr_buf[i][strlen(wlan_script_entry_attr_buf[i]) - 1] ='\0';/*remove last ";"*/
			sprintf(buf,"%s=%s\n",wlan_script_entry_attr_para[i]
				,wlan_script_entry_attr_buf[i]);
			fputs(buf,fp);
		}
	}

	/*AccessPolicy0=.... AccessPolicy1=....*/
	for(j=0; j < BssidNum; j++){
		sprintf(nodeName[1],"%s%d",SUB_NODE_NAME,j);
		if(getAttrValue(top, nodeName, "AccessPolicy", tmp) == TCAPI_PROCESS_OK){
			if(strlen(tmp) > 0){
				sprintf(buf,"%s%d=%s\n","AccessPolicy",j,tmp);
				fputs(buf,fp);
			}
		}
	}
	/*AccessControlList0=MAC1;MAC2;.......*/

	for(j=0; j < BssidNum; j++){
		sprintf(nodeName[1],"%s%d",SUB_NODE_NAME,j);
		strcpy(buf,"");
		for(i=0;i<WLAN_MAC_NUM;i++){
			sprintf(tmp2,"%s%d","WLan_MAC",i);
			if(getAttrValue(top, nodeName, tmp2, tmp) == TCAPI_PROCESS_OK){
				if(strlen( tmp) > 0){
					strcat(buf,tmp);
					strcat(buf,";");
				}
			}
		}
		if(strlen(buf) > 0){
			buf[strlen(buf) - 1] ='\0';/*remove last ";"*/
			/*
				krammer change for bug 1359, the .dat has no need to add ""!
			*/
			sprintf(tmp,"%s%d=%s\n","AccessControlList",j,buf);
			fputs(tmp,fp);
		}
	}

	#ifdef WSC_AP_SUPPORT/*add by xyyou*/
	/*WscDefaultSSID1=....WscDefaultSSID2=.......*/
	for(j=0; j < BssidNum; j++){
		sprintf(nodeName[1],"%s%d",SUB_NODE_NAME,j);
		sprintf(tmp2,"%s%d","WscDefaultSSID",j+1);
		if(getAttrValue(top, nodeName, tmp2, tmp) == TCAPI_PROCESS_OK){
			if(strlen(tmp)>0){
				sprintf(buf,"%s%d=%s\n","WscDefaultSSID",j+1,tmp);
				fputs(buf,fp);				
			}
		}
	}
	#endif
	/*gcyin*/

#if defined(TCSUPPORT_WLAN_8021X)
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],LAN);
	sprintf(nodeName[1],"%s%d",SUB_NODE_NAME,0);
	memset(buf,0,strlen(buf));
	memset(tmp,0,sizeof(tmp));
	if(getAttrValue(top, nodeName, LAN_IP, tmp) == TCAPI_PROCESS_OK){
		if(strlen(tmp) > 0){
			 sprintf(buf,"%s=%s\n", "own_ip_addr",tmp);
			 fputs(buf,fp);
		}

	}
#endif
	/*gcyin begin*/
#ifdef TCSUPPORT_WLAN_WDS
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],"WLan11ac");
	strcpy(nodeName[1],"WDS");

	/*WdsEnable*/
	if(getAttrValue(top, nodeName, "WdsEnable", tmp) == TCAPI_PROCESS_OK){
		if(strlen(tmp) != 0){
			sprintf(buf,"WdsEnable=%s\n",tmp);
			fputs(buf,fp);
		}
	}
	/*WdsList*/
	sprintf(buf,"%s=", "WdsList");
	for(i = 0; i<MAX_WDS_ENTRY; i++){
		sprintf(tmp2,"Wds_MAC%d",i);
		if(getAttrValue(top, nodeName, tmp2, tmp) == TCAPI_PROCESS_OK){
			if(strlen(tmp) != 0){
				strcat(buf,tmp);
				strcat(buf,";");
			}else{
				strcat(buf,"00:00:00:00:00:00");
				strcat(buf,";");			
			}
		}			
	}
	buf[strlen(buf)-1] = '\0';
	strcat(buf,"\n");
	fputs(buf,fp);

	/*WdsEncrypType*/
	if(getAttrValue(top, nodeName, "WdsEncrypType", tmp) == TCAPI_PROCESS_OK){
		if(strlen(tmp) != 0){
			sprintf(buf,"WdsEncrypType=%s\n",tmp);
			fputs(buf,fp);
		}
	}
	/*WdsKey*/
	if(getAttrValue(top, nodeName, "WdsKey", tmp) == TCAPI_PROCESS_OK){
		if(strlen(tmp) != 0){
			sprintf(buf,"WdsKey=%s\n",tmp);
			fputs(buf,fp);
		}
	}
#endif
#ifdef BBUTOWBU
{
   	FILE *fptmp;  
	unsigned char flash_mac_addr[6]={0};
	unsigned char mac_addr[]={0x00,0xaa,0xbb,0x01,0x23,0x45};
	
    	/* Read bootloader from flash */
    	system("cat /dev/mtd0 > /tmp/boot.bin");

    	/*Read MAC*/
    	fptmp = fopen("/tmp/boot.bin", "r+b");
    	if( fptmp == NULL ){
       	 tcdbg_printf("open boot.bin file error!!\n");
		unlink("/tmp/boot.bin");
        	return FAIL;
    	}

    	fseek(fptmp, 0x0000ff48L, SEEK_SET);
    	fread(flash_mac_addr, 6, 1, fptmp);
	if( (flash_mac_addr[0] == 0) && (flash_mac_addr[1] == 0) 
		&& (flash_mac_addr[2] == 0) &&
	    (flash_mac_addr[3] == 0) && (flash_mac_addr[4] == 0)
	    && (flash_mac_addr[5] == 0) ) {
		tcdbg_printf("MacAddress for wifi chip is NULL, force to default!!!\n");
	}
	else {    
		 memcpy(mac_addr, flash_mac_addr, 6);  	
		#if defined(TCSUPPORT_UNIQUEMAC_ALL)
		 mac_addr[5]=mac_addr[5]+0x10;
		#endif
		#if defined(MT7592) 
		/*according to WCN suggestion to use global mac addr*/
		mac_addr[5]=mac_addr[5]+0x01;
		#else
		 mac_addr[0]=mac_addr[0]+0x02;
		mac_addr[3]=((mac_addr[3] & 0xF0) |(0x3));
		#endif
	}
	sprintf(buf, "MacAddress=%02x:%02x:%02x:%02x:%02x:%02x\n"
		,mac_addr[0],mac_addr[1],mac_addr[2],mac_addr[3],mac_addr[4],mac_addr[5]);
	fputs(buf,fp);
		
    fclose(fptmp);
	unlink("/tmp/boot.bin");
}	
#endif
	fclose(fp);
	chmod(WLAN_AC_PATH,S_IRUSR|S_IWUSR|S_IXUSR);

  	return SUCCESS;
}

/*______________________________________________________________________________
**	function name:write_wlan11ac_exe_sh
**
**	description:
*     create wireless execute script
**	parameters:
*     top:xml top node address
**	global:
*     none
**	return:
*       0:success
*	  1:fail
**	call:
*     none
**	revision:
*     1.evan 20130801
**____________________________________________________________________________*/
int
write_wlan11ac_exe_sh(mxml_node_t *top, int BssidNum){
	int i, wlan_id;
  	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], tmp[160], tmp2[160],buf[160],tmpChannel[8] = {0};
  	FILE *fp;

	char wirelessmode[16], encrytype[16];
#if defined(TCSUPPORT_WLAN_8021X_EXT)
	struct hostent *hp = NULL;
	struct sockaddr_in RadiusAddr;
#endif
	char wlan_script_com_attr[][ATTR_SIZE]=
	{
		{"CountryRegion"},
		{"CountryRegionABand"},
		{"Channel"},
		{"BeaconPeriod"},
		{"RTSThreshold"},
		{"FragThreshold"},
		{"DtimPeriod"},
		{"TxPower"},
		{"TxPreamble"},
		/*{"WirelessMode"},
		{"BasicRate"},*/
	#if defined(RT5392) || defined(MT7610E)  || defined(WLAN_MULTIDRIVER) || defined(MT7612E)
		{"IgmpSnEnable"},
	#endif
		{"BGProtection"},
		{"TxPower"},
		{"TxPreamble"},
		{"ShortSlot"},
		{"TxBurst"},
		{"PktAggregate"},
		{"IEEE80211H"},
		{""}
	};
	
	const char wlan_script_com_attrs_other[][2][ATTR_SIZE]=
	{
		{"HT_RDG", "HtRdg"},
		{"HT_STBC", "HtStbc"},
		{"HT_AMSDU", "HtAmsdu"},
		{"HT_AutoBA", "HtAutoBa"},
		{"HT_BADecline", "BADecline"},
		{"HT_DisallowTKIP", "HtDisallowTKIP"},
		{"HT_TxStream", "HtTxStream"},
		{"HT_RxStream", "HtRxStream"},
		{"APSDCapable", "UAPSDCapable"},
		{"HT_BSSCoexistence","HtBssCoex"}, 
#if 0 //defined(MT7612E)
		{"HT_LDPC", "HtStbc"},
		{"VHT_LDPC", "HtStbc"},
#endif
#if defined(MT7610E) || defined(MT7612E)
		{"VHT_STBC", "VhtStbc"},
		{"VHT_BW_SIGNAL", "VhtBwSignal"},
		{"VHT_DisallowNonVHT", "VhtDisallowNonVHT"},
#endif
		{"", ""}
	};

	memset(nodeName,0,sizeof(nodeName));
  	strcpy(nodeName[0],WEBCURSET);
  	strcpy(nodeName[1],SUB_NODE_NAME);
  	if(getAttrValue(top, nodeName, "wlan_ac_id", tmp) == TCAPI_PROCESS_OK){
    		wlan_id = atoi(tmp);
  	}else{
    		return FAIL;
 	 }


	/*write common setting WLAN_exec.sh*/
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],"WLan11ac");
	 strcpy(nodeName[1],"Common");

	fp = fopen(WLAN_AC_SCRIPT_PATH,"w");
	if(fp == NULL){
		return FAIL;
	}
	 for(i = 0;strlen(wlan_script_com_attr[i]) != 0;i++){
	 	if(strcmp(wlan_script_com_attr[i], "Channel")==0){
			if(getAttrValue(top, nodeName, wlan_script_com_attr[i], tmpChannel)
				== TCAPI_PROCESS_OK){
				if(atoi(tmpChannel)){
					sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id
						, wlan_script_com_attr[i], tmpChannel);
					fputs(buf,fp);
				}
			}
		}else{
			if(getAttrValue(top, nodeName, wlan_script_com_attr[i], tmp)
				== TCAPI_PROCESS_OK){
				sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id
					, wlan_script_com_attr[i], tmp);
				fputs(buf,fp);
			}
		}
	 }
	for(i = 0;strlen(wlan_script_com_attrs_other[i]) != 0;i++){
		if(getAttrValue(top, nodeName, wlan_script_com_attrs_other[i][0], tmp)
			== TCAPI_PROCESS_OK){
			sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id
				, wlan_script_com_attrs_other[i][1], tmp);
			fputs(buf,fp);
		}
	}

	 if(getAttrValue(top, nodeName, "WirelessMode", tmp) == TCAPI_PROCESS_OK){
		sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id,"WirelessMode", tmp);
		fputs(buf,fp);

		strncpy(wirelessmode, tmp, sizeof(wirelessmode));
	}
#if defined(RT3390) || defined(RT3090) || defined(RT5392)  || defined(MT7610E) || defined(MT7612E) /*add by xyyou, for support 11n of RT3390*/
	if(Is11nWirelessMode5G(top)){
		if(getAttrValue(top, nodeName, "HT_BW", tmp) == TCAPI_PROCESS_OK){
			sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "HtBw", tmp);
			fputs(buf,fp);
		}
		if((atoi(tmp)==1) && (atoi(tmpChannel)!=0)){
			if(getAttrValue(top, nodeName, "HT_EXTCHA", tmp)
				== TCAPI_PROCESS_OK){
				sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "HtExtcha", tmp);
				fputs(buf,fp);
			}
		}
		if(getAttrValue(top, nodeName, "HT_GI", tmp) == TCAPI_PROCESS_OK){
			sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "HtGi", tmp);
			fputs(buf,fp);
		}
		if (Is11acWirelessMode(top)) {
			if (getAttrValue(top, nodeName, "HT_BW", tmp) == TCAPI_PROCESS_OK) {
				if (atoi(tmp) == 1) {
					if (getAttrValue(top, nodeName, "VHT_BW", tmp)
						== TCAPI_PROCESS_OK) {
						sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "VhtBw", tmp);
						fputs(buf,fp);
					}
					if (getAttrValue(top, nodeName, "VHT_SGI", tmp)
						== TCAPI_PROCESS_OK) {
						sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "VhtGi", tmp);
						fputs(buf,fp);
					}
				}
				else {
					sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "VhtBw", "0");
					fputs(buf,fp);
				}
			}
		}
		else {
			sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "VhtBw", "0");
			fputs(buf,fp);
		}
	}else{
		sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "HtBw", "0");
		fputs(buf,fp);
		sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "VhtBw", "0");
		fputs(buf,fp);
	}
#endif /*end xyyou*/

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],"WLan11ac");
	sprintf(nodeName[1],"%s%d",SUB_NODE_NAME,wlan_id);
	if(getAttrValue(top, nodeName, "AuthMode", tmp) == TCAPI_PROCESS_OK){
		strncpy(encrytype,tmp,sizeof(encrytype));	
	}
#if defined(RT3390) || defined(RT3090) || defined(RT5392)  || defined(MT7610E) || defined(MT7612E) /*add by xyyou, for support 11n of RT3390*/
	if(Is11nWirelessMode5G(top)){
		if(getAttrValue(top, nodeName, "HT_MCS", tmp) == TCAPI_PROCESS_OK){
			if(atoi(tmp)<=15){
				sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "HtMcs", tmp);
			}else
				sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "HtMcs", "33");/*33 for auto selection,xyyou*/
			fputs(buf,fp);
		}
	}else{
		sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "HtMcs", "33");
		fputs(buf,fp);
	}
#endif /*end xyyou*/
	/*write entry  setting to WLAN_exec.sh*/
/*
	2008 11 17 krammer change for bug 1242
    	SSID must be the last setting of all cmd, referenced in RT61 release note
*/
	if(getAttrValue(top, nodeName, "HideSSID", tmp) == TCAPI_PROCESS_OK){
		sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "HideSSID", tmp);
		fputs(buf,fp);
	}
	if(getAttrValue(top, nodeName, "NoForwarding", tmp) == TCAPI_PROCESS_OK){
		sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "NoForwarding", tmp);
		fputs(buf,fp);
	}
	/* rekeyinterval rekeymethod */
	if(getAttrValue(top, nodeName, "RekeyInterval", tmp) == TCAPI_PROCESS_OK){
		sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "RekeyInterval", tmp);
		fputs(buf,fp);
		sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "RekeyMethod", "TIME");
		fputs(buf,fp);
	}

	/*AuthMode*/
 	if(getAttrValue(top, nodeName, "AuthMode", tmp) == TCAPI_PROCESS_OK){
		if(strstr(tmp, "WEP")){/*WEP64-bits or WEP128-bits*/
#if defined(TCSUPPORT_WLAN_8021X)
				if(strstr(tmp, "Radius")){/*Radius-WEP64, Radius-WEP128*/
					sprintf(buf,WLAN_AC_SCRIPT_PREFIX 
						, wlan_id, "IEEE8021X", "1");
					fputs(buf,fp);
				}else{
					sprintf(buf,WLAN_AC_SCRIPT_PREFIX
						, wlan_id, "IEEE8021X", "0");
					fputs(buf,fp);
				}
#endif
				if(getAttrValue(top, nodeName, "WEPAuthType", tmp)
					== TCAPI_PROCESS_OK){
					if(!strcmp(tmp, "OpenSystem")){
						sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id
							, "AuthMode", "OPEN");
						fputs(buf,fp);
					}else if(!strcmp(tmp, "SharedKey")){
						sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id
							, "AuthMode", "SHARED");
						fputs(buf,fp);
					}else{
						sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id
							, "AuthMode", "WEPAUTO");
						fputs(buf,fp);
					}
				}

				sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id
					, "EncrypType", "WEP");
				fputs(buf,fp);
				if(getAttrValue(top, nodeName, "DefaultKeyID", tmp)
					!= TCAPI_PROCESS_OK){
					strcpy(tmp,"1");
				}
				sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "DefaultKeyID", tmp);
				fputs(buf,fp);
				/*key1~key4*/
				for(i=1;i<=KEY_NUM;i++){
					sprintf(tmp2,"Key%dStr",i);
					if(getAttrValue(top, nodeName, tmp2, tmp) == TCAPI_PROCESS_OK){
						/*check strlen because the key maybe empty*/
						if(strlen(tmp) > 0){
						specialCharacterHandle(tmp);
						sprintf(tmp2,"\"%s\"",tmp);/*need "..."*/

						sprintf(tmp,"Key%d",i);
						/*krammer change for bug 1352*/
						sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, tmp, tmp2);
						fputs(buf,fp);
					}
				}
				}
        		}
		/*krammer change for bug 1242
		    WPAPSK and WPA2PSK cmd must be the formate as following,
		    1.AuthMode
		    2.EncrypType
		    3.SSID
		    4.WPAPSK
		    5.DefaultKeyID=2
		       .
		       .
		       .
		     n.SSID (referenced by RT61 release note)
		    so i change the sequence of following code and add a setting SSID cmd
		*/
			else if(!strcmp(tmp, "WPAPSK")||!strcmp(tmp, "WPA2PSK")
				||!strcmp(tmp, "WPAPSKWPA2PSK")
#if defined(TCSUPPORT_WLAN_8021X)
				||!strcmp(tmp, "WPA")||!strcmp(tmp, "WPA2")
				||!strcmp(tmp, "WPA1WPA2")
#endif
			){/*WPAPSK or WPA2PSK or Mixed mode*/
				sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "AuthMode", tmp);
				fputs(buf,fp);
#if defined(TCSUPPORT_WLAN_8021X)
				if(!strcmp(tmp, "WPA")||!strcmp(tmp, "WPA2")
					||!strcmp(tmp, "WPA1WPA2")){/*Radius-WEP64, Radius-WEP128*/
					sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id
						, "IEEE8021X", "1");
					fputs(buf,fp);
				}else{
					sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id
						, "IEEE8021X", "0");
					fputs(buf,fp);
				}
#endif				
				if(getAttrValue(top, nodeName, "EncrypType", tmp)
					== TCAPI_PROCESS_OK){
					sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "EncrypType", tmp);
					fputs(buf,fp);
				}

			/*krammer add for bug 1242*/
			if(getAttrValue(top, nodeName, "SSID", tmp) == TCAPI_PROCESS_OK){
				specialCharacterHandle(tmp);
				sprintf(tmp2,"\"%s\"",tmp);/*need "..."*/
				sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "SSID", tmp2);
				fputs(buf,fp);
			}
			/*krammer add for bug 1242*/
				if(getAttrValue(top, nodeName, "WPAPSK", tmp)
					== TCAPI_PROCESS_OK){
					specialCharacterHandle(tmp);
					sprintf(tmp2,"\"%s\"",tmp);/*need "..."*/
					sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "WPAPSK", tmp2);
					fputs(buf,fp);
				}
			/*krammer change*/
			sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "DefaultKeyID", "2");
			/*WPAPSK and WPA2PSK use DefaultKeyID = 2*/
			fputs(buf,fp);
			/*krammer change*/
			}else{/*OPEN*/
				sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "AuthMode", "OPEN");
				fputs(buf,fp);
				sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "EncrypType", "NONE");
				fputs(buf,fp);
#if defined(TCSUPPORT_WLAN_8021X)
				sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "IEEE8021X", "0");
				fputs(buf,fp);
#endif
			}
 	}

#ifdef TCSUPPORT_WLAN_PMF
	/*pmf configuretion*/
	if(getAttrValue(top, nodeName, "PMFMFPC", tmp) == TCAPI_PROCESS_OK){
		sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "PMFMFPC", tmp);
		fputs(buf,fp);
	}

	if(getAttrValue(top, nodeName, "PMFMFPR", tmp) == TCAPI_PROCESS_OK){
		sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "PMFMFPR", tmp);
		fputs(buf,fp);
	}

	if(getAttrValue(top, nodeName, "PMFSHA256", tmp) == TCAPI_PROCESS_OK){
		sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "PMFSHA256", tmp);
		fputs(buf,fp);
	}
#endif

#if defined(TCSUPPORT_WLAN_8021X_EXT)
	/*RADIUS_Server*/
		memset(tmp,0,sizeof(tmp));
		memset(tmp2,0,sizeof(tmp2));
		memset(buf,0,sizeof(buf));
		if(getAttrValue(top, nodeName, "RADIUS_Server", tmp) == TCAPI_PROCESS_OK){
			if(strlen(tmp) > 0 ){
				hp = gethostbyname(tmp);
				if(hp != (struct hostent *) NULL){
					memcpy((char *) &RadiusAddr.sin_addr,
						(char *) hp->h_addr_list[0]
						,sizeof(RadiusAddr.sin_addr));
					strcpy(tmp2,
					inet_ntoa(*(struct in_addr *) &RadiusAddr.sin_addr.s_addr));
					strcat(tmp2,";");
				}else{
					strcat(tmp2,"0.0.0.0;");
				}
			}
		}
		/*#if defined(TCSUPPORT_WLAN_8021X_EXT)*/
		memset(tmp,0,sizeof(tmp));
		if(getAttrValue(top, nodeName, "BAK_RADIUS_Server", tmp) == TCAPI_PROCESS_OK){
			if(strlen(tmp) > 0 ){
				hp = gethostbyname(tmp);
				if(hp != (struct hostent *) NULL){
					memcpy((char *) &RadiusAddr.sin_addr
						, (char *) hp->h_addr_list[0]
						,sizeof(RadiusAddr.sin_addr));
					strcat(tmp2,
					inet_ntoa(*(struct in_addr *) &RadiusAddr.sin_addr.s_addr));
					strcat(tmp2,";");
				}else{
					strcat(tmp2,"0.0.0.0;");
				}
			}
		}
		/*#endif*/
		if(strlen(tmp2) > 0){
			tmp2[strlen(tmp2)-1]='\0';/*remove last ";"*/
			sprintf(tmp,"\"%s\"",tmp2);
			sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "RADIUS_Server", tmp);
			fputs(buf,fp);
		}

		/*RADIUS_Port*/
		memset(tmp,0,sizeof(tmp));
		memset(tmp2,0,sizeof(tmp2));
		memset(buf,0,sizeof(buf));
		if(getAttrValue(top, nodeName, "RADIUS_Port", tmp) == TCAPI_PROCESS_OK){
			if(strlen(tmp) > 0){
				strcpy(tmp2,tmp);
				strcat(tmp2,";");
			}
		}
		/*#if defined(TCSUPPORT_WLAN_8021X_EXT)*/
		memset(tmp,0,sizeof(tmp));
		if(getAttrValue(top, nodeName, "BAK_RADIUS_Port", tmp) == TCAPI_PROCESS_OK){
			if(strlen(tmp) > 0){
				strcat(tmp2,tmp);
				strcat(tmp2,";");
			}
		}
		/*#endif*/
		if(strlen(tmp2) > 0){
			tmp2[strlen(tmp2)-1]='\0';/*remove last ";"*/
			sprintf(tmp,"\"%s\"",tmp2);
			sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "RADIUS_Port", tmp);
			fputs(buf,fp);
		}

		/*RADIUS_Key*/
		memset(tmp,0,sizeof(tmp));
		memset(tmp2,0,sizeof(tmp2));
		memset(buf,0,sizeof(buf));
		if(getAttrValue(top, nodeName, "RADIUS_Key", tmp) == TCAPI_PROCESS_OK){
			if(strlen(tmp) > 0){
				strcpy(tmp2,tmp);
				strcat(tmp2,";");
			}
 		}
		/*#if defined(TCSUPPORT_WLAN_8021X_EXT)*/
		memset(tmp,0,sizeof(tmp));
		if(getAttrValue(top, nodeName, "BAK_RADIUS_Key", tmp) == TCAPI_PROCESS_OK){
			if(strlen(tmp) > 0){
				strcat(tmp2,tmp);
				strcat(tmp2,";");
			}
		}
		/*#endif*/
		if(strlen(tmp2) > 0){
			tmp2[strlen(tmp2)-1]='\0';/*remove last ";"*/
			sprintf(tmp,"\"%s\"",tmp2);
			sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "RADIUS_Key", tmp);
			fputs(buf,fp);
		}
#else
	/*RADIUS_Server*/
		memset(tmp,0,sizeof(tmp));
		memset(tmp2,0,sizeof(tmp2));
		memset(buf,0,sizeof(buf));
		if(getAttrValue(top, nodeName, "RADIUS_Server", tmp) == TCAPI_PROCESS_OK){
			if(strlen(tmp) > 0 ){
				sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "RADIUS_Server", tmp);
				fputs(buf,fp);
			}
 		}

		/*RADIUS_Port*/
		memset(tmp,0,sizeof(tmp));
		memset(buf,0,sizeof(buf));
		if(getAttrValue(top, nodeName, "RADIUS_Port", tmp) == TCAPI_PROCESS_OK){
			if(strlen(tmp) > 0){
				sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "RADIUS_Port", tmp);
				fputs(buf,fp);
			}
		}

		/*RADIUS_Key*/
		memset(tmp,0,sizeof(tmp));
		memset(buf,0,sizeof(buf));
		if(getAttrValue(top, nodeName, "RADIUS_Key", tmp) == TCAPI_PROCESS_OK){
			if(strlen(tmp) > 0){
				sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "RADIUS_Key", tmp);
				fputs(buf,fp);
			}
		}
#endif
	/*AccessPolicy=....*/
	if(getAttrValue(top, nodeName, "AccessPolicy", tmp) == TCAPI_PROCESS_OK){
		if(strlen(tmp) > 0){
			sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "AccessPolicy", tmp);
			fputs(buf,fp);
		}
	}
	/*AccessControlList=MAC1;MAC2;.......*/

	strcpy(buf,"");
	for(i=0;i<WLAN_MAC_NUM;i++){
		sprintf(tmp2,"%s%d","WLan_MAC",i);
		if(getAttrValue(top, nodeName, tmp2, tmp) == TCAPI_PROCESS_OK){
			if(strlen( tmp) > 0){
				strcat(buf,tmp);
				strcat(buf,";");
			}
		}
	}
	if(strlen(buf) > 0){
		buf[strlen(buf) - 1] ='\0';/*remove last ";"*/
		sprintf(tmp2,"\"%s\"",buf);/*add ""*/
	#if defined(RT3390) || defined(RT3090) || defined(RT5392)  || defined(MT7610E) || defined(MT7612E) /*add by xyyou*/
		sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "ACLClearAll","1");
		fputs(buf,fp);
		sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "ACLAddEntry",tmp2);
	#else
		sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "AccessControlList",tmp2);
	#endif
		fputs(buf,fp);
	}
	#if defined(RT3390) || defined(RT3090) || defined(RT5392)  || defined(MT7610E) || defined(MT7612E) /*add by xyyou, for support 11n of RT3390*/
	if(!Is11nWirelessMode5G(top)){

		if(getAttrValue(top, nodeName, "WMM", tmp) == TCAPI_PROCESS_OK){
			sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "WmmCapable", tmp);
			fputs(buf,fp);
			if(txBurst_or_not_ac(top, BssidNum)){
				sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "TxBurst", "1");
			}
			else{
				sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "TxBurst", "0");
			}
			fputs(buf,fp);
		}
	}
	else {
		sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "WmmCapable", "1");
		fputs(buf,fp);
		sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "TxBurst", "0");
		fputs(buf,fp);
	}
	#else
	if(getAttrValue(top, nodeName, "WMM", tmp) == TCAPI_PROCESS_OK){
		sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "WmmCapable", tmp);
		fputs(buf,fp);
		if(txBurst_or_not_ac(top, BssidNum)){
			sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "TxBurst", "1");
		}
		else{
			sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "TxBurst", "0");
		}
		fputs(buf,fp);
	}
	#endif/*end xyyou*/
		/*
		2008 11 17 krammer change for bug 1242
   	 	SSID must be the last setting of all cmd, referenced in RT61 release note
	*/
	#ifdef WSC_AP_SUPPORT/*add by xyyou*/
		if(wlan_id==0){/*now we just support main SSID,xyyou*/
			setAttrValue(top, nodeName, "WPSConfStatus", "2");			
			sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "WscConfStatus", "2");									
			fputs(buf,fp);
			if(getAttrValue(top, nodeName, "WPSConfMode", tmp) 
				== TCAPI_PROCESS_OK){
				sprintf(buf, WLAN_AC_SCRIPT_PREFIX, wlan_id, "WscConfMode", tmp);
				fputs(buf,fp);
			}			
			//added by fred, for wsc v2
			if(getAttrValue(top, nodeName, "WscV2Support", tmp) 
				== TCAPI_PROCESS_OK){
				sprintf(buf, WLAN_AC_SCRIPT_PREFIX, wlan_id, "WscV2Support", tmp);
				fputs(buf,fp);
			}
			if(getAttrValue(top, nodeName, "WscMaxPinAttack", tmp) 
				== TCAPI_PROCESS_OK){
                                sprintf(buf, WLAN_AC_SCRIPT_PREFIX
									, wlan_id, "WscMaxPinAttack", tmp);
                                fputs(buf,fp);
                        }
			if(getAttrValue(top, nodeName, "WscSetupLockTime", tmp) 
				== TCAPI_PROCESS_OK){
                                sprintf(buf, WLAN_AC_SCRIPT_PREFIX
									, wlan_id, "WscSetupLockTime", tmp);
                                fputs(buf,fp);
                        }


		}
	#endif /*end xyyou*/

	#if (defined(RT3390) || defined(RT3090)) && !defined(WLAN_MULTIDRIVER)/*just for 3390&3090, 5392 used as common attribute*/
	if(getAttrValue(top, nodeName, "IgmpSnEn", tmp) == TCAPI_PROCESS_OK){
		sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "IgmpSnEnable", tmp);
		fputs(buf,fp);
	}
	#endif
	 if(getAttrValue(top, nodeName, "KeyPassphrase", tmp) == TCAPI_PROCESS_OK){
		sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id,"KeyPassphrase", tmp);
		fputs(buf,fp);
	 }	

	if(getAttrValue(top, nodeName, "SSID", tmp) == TCAPI_PROCESS_OK){
		specialCharacterHandle(tmp);
		sprintf(tmp2,"\"%s\"",tmp);/*need "..."*/
		sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "SSID", tmp2);
		fputs(buf,fp);
	}
#if defined(TCSUPPORT_WLAN_MAXSTANUM_GUI)
	if(getAttrValue(top, nodeName, "MaxStaNum", tmp) == TCAPI_PROCESS_OK){
		sprintf(buf,WLAN_AC_SCRIPT_PREFIX , wlan_id, "MaxStaNum", tmp);
		fputs(buf,fp);
	}
#endif
  	fclose(fp);
 	chmod(WLAN_AC_SCRIPT_PATH,S_IRUSR|S_IWUSR|S_IXUSR);
	/*sleep 1 and restart boa
		sleep 1 to let the web page finish getting information.
		restart boa to prevent error when user use change ssid and link CPE by using wireless.
	fp = fopen(WLAN_RESTART_BOA,"w");
	if(fp != NULL){
		sprintf(buf,"sleep 1\nkillall -9 boa\n/userfs/bin/boa -c /boaroot/ -d &\n");
		fputs(buf, fp);
		fclose(fp);
	}
	chmod(WLAN_RESTART_BOA,S_IRUSR|S_IWUSR|S_IXUSR);*/

#if 0 /*defined(RT3390) || defined(RT3090) || defined(RT5392)  || defined(MT7610E) //used to set WIFi Tx Rate*/
	if(Is11acWirelessMode(top)){
		if(getAttrValue(top, nodeName, "HT_MCS", tmp) == TCAPI_PROCESS_OK){
			if((strcmp(encrytype,"WPAPSK") == 0) &&(atoi(wirelessmode) == 9) && (atoi(tmp)> 7)){
				strcpy(tmp,"33");
			}
			doWlan11acMcs(top, tmp);
		}
	}
	else{
		if(getAttrValue(top, nodeName, "HT_RATE", tmp) == TCAPI_PROCESS_OK){
			doWlan11acRate(top, tmp);
		}
	}
#endif 

  	return SUCCESS;
}

int doCheck11acHtExtcha(mxml_node_t *top,int isAuto){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char tmpBW[8] = {0}, tmpChannel[8] = {0}, tmpHtExtcha[8] = {0}
	,curChannel[8] = {0};
	int tempChannel;
	char HT5GExtCh[22][2] = 
	{
		{"1"}, /* 36's extension channel -> 40 */
		{"0"}, /* 40's extension channel -> 36 */
		{"1"}, /* 44's extension channel -> 48 */
		{"0"}, /* 48's extension channel -> 44 */
		{"1"}, /* 52's extension channel -> 56 */
		{"0"}, /* 56's extension channel -> 52 */
		{"1"}, /* 60's extension channel -> 64 */
		{"0"}, /* 64's extension channel -> 60 */
		{"1"}, /* 100's extension channel -> 104 */
		{"0"}, /* 104's extension channel -> 100 */
		{"1"}, /* 108's extension channel -> 112 */
		{"0"}, /* 112's extension channel -> 108 */
		{"1"}, /* 116's extension channel -> 120 */
		{"0"}, /* 120's extension channel -> 116 */
		{"1"}, /* 124's extension channel -> 128 */
		{"0"}, /* 128's extension channel -> 124 */
		{"1"}, /* 132's extension channel -> 136 */
		{"0"}, /* 136's extension channel -> 132 */
		{"1"}, /* 149's extension channel -> 153 */
		{"0"}, /* 153's extension channel -> 149 */
		{"1"}, /* 157's extension channel -> 161 */
		{"0"}, /* 161's extension channel -> 157 */
	};
	
	if(Is11nWirelessMode5G(top)){
		memset(nodeName,0,sizeof(nodeName));
		strcpy(nodeName[0],"WLan11ac");
		strcpy(nodeName[1],COMMON);
		if((getAttrValue(top, nodeName, WLAN_HT_BW, tmpBW)
			== TCAPI_PROCESS_OK)
			&& (getAttrValue(top, nodeName, WLAN_CHANNEL, tmpChannel) 
			== TCAPI_PROCESS_OK)
			&& (getAttrValue(top, nodeName, WLAN_HT_EXTCHA, tmpHtExtcha) 
			== TCAPI_PROCESS_OK)){
			if((atoi(tmpBW)==1)){	
				if(atoi(tmpChannel) == 0 && isAuto == IS_AUTO_CHANNEL){ /*auto channel*/
					memset(nodeName,0,sizeof(nodeName));
					strcpy(nodeName[0],INFO);
					strcpy(nodeName[1],"WLan11ac");
					if(getAttrValue(top, nodeName, CURRENT_CHANNEL, curChannel)
						== TCAPI_PROCESS_OK){
						strcpy(tmpChannel,curChannel);
					}
				}					

				memset(nodeName,0,sizeof(nodeName));
				strcpy(nodeName[0],"WLan11ac");
				strcpy(nodeName[1],COMMON);

				tempChannel = atoi(tmpChannel);
				if ((tempChannel >= 36) && (tempChannel <= 64)) {
					tempChannel /= 4;
					tempChannel -= 9;
					setAttrValue(top, nodeName, WLAN_HT_EXTCHA, 
						HT5GExtCh[tempChannel]);
				}
				else if ((tempChannel >= 100) && (tempChannel <= 136)) {
					tempChannel /= 4;
					tempChannel -= 17;
					setAttrValue(top, nodeName, WLAN_HT_EXTCHA, 
						HT5GExtCh[tempChannel]);
				}
				else if ((tempChannel >= 149) && (tempChannel <= 161)) {
					tempChannel -= 1;
					tempChannel /= 4;
					tempChannel -= 19;
					setAttrValue(top, nodeName, WLAN_HT_EXTCHA,
						HT5GExtCh[tempChannel]);
				}
				else {
					setAttrValue(top, nodeName, WLAN_HT_EXTCHA, "0");
				}	
			}
		}
	}
	return SUCCESS;
}


/* check if wireless mode is 11ac */
int
Is11acWirelessMode(mxml_node_t *top){
	int is11acmode=0;
  	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], tmp[160];
	
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],"WLan11ac");
	strcpy(nodeName[1],"Common");

	if(getAttrValue(top, nodeName, "WirelessMode", tmp) == TCAPI_PROCESS_OK){
		if((atoi(tmp)==PHY_11VHT_N_A_MIXED) || (atoi(tmp)==PHY_11VHT_N_MIXED)){
			is11acmode=1;
		}
	}
	
	return is11acmode;
}

/* check if wireless mode is 11n 5G band */
int
Is11nWirelessMode5G(mxml_node_t *top){
	int is11nmode=0;
  	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], tmp[160];
	
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],"WLan11ac");
	strcpy(nodeName[1],"Common");

	if(getAttrValue(top, nodeName, "WirelessMode", tmp) == TCAPI_PROCESS_OK){
		if((atoi(tmp)==PHY_MODE_11AN_MIXED) || (atoi(tmp)==PHY_11VHT_N_A_MIXED) || (atoi(tmp)==PHY_11VHT_N_MIXED)){
			is11nmode=1;
		}
	}
	
	return is11nmode;
}

/*check if wmm enabled, txburst should disable when wmm enable.*/
int
txBurst_or_not_ac(mxml_node_t *top, int BssidNum){
	int i=0, txBurst=1;
  	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], tmp[160];
	
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],"WLan11ac");
	for(i=0; i<BssidNum; i++){
		sprintf(nodeName[1],"%s%d",SUB_NODE_NAME,i);
		if(getAttrValue(top, nodeName, "WMM", tmp) == TCAPI_PROCESS_OK){
			if(strcmp(tmp, "1") == 0){
				txBurst=0;
				break;
			}
		}
	}
	return txBurst;
}
int 
reset_ac_counter(mxml_node_t *top){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], tmp[4]={0};
	FILE *fp;
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],"Info");
	strcpy(nodeName[1],"WLan11ac");
	if(getAttrValue(top, nodeName, "ReCounterActive", tmp) == TCAPI_PROCESS_OK){
		if(strcmp(tmp, "0") == 0){/* no active */
			tcdbg_printf("ReCounterActive = 0\n");
		}
		else{
			fp = fopen(WLAN_AC_SCRIPT_PATH,"w");
			if(fp != NULL){					
				fprintf(fp, "/userfs/bin/iwpriv rai0 set ResetCounter=1\n");
				tcdbg_printf("dgk:set ResetCounter = 1 in %s\n", __FUNCTION__);
				fclose(fp);
				chmod(WLAN_AC_SCRIPT_PATH,S_IRUSR|S_IWUSR|S_IXUSR);
				return 1;
			}
			else{
				tcdbg_printf("can't open file in reset_counter()\n");
			}
		}
	}
	else{
		tcdbg_printf("ReCounterActive = NULL\n");
	}
	return 0;
}
#ifdef WSC_AP_SUPPORT
int
wps_ac_genpincode(mxml_node_t *top){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], tmp[4]={0};
	FILE *fp;
#ifdef BBUTOWBU
	char cmd[50]={0};
#endif
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],"Info");
	strcpy(nodeName[1],"WLan11ac");
	if(getAttrValue(top, nodeName, "WPSGenPinCode", tmp) == TCAPI_PROCESS_OK){
		if(strcmp(tmp, "0") == 0){/* no active */
			tcdbg_printf("WPSGenPinCode = 0\n");
		}
		else{ 
			memset(nodeName,0,sizeof(nodeName));
			strcpy(nodeName[0],"WLan11ac");
			strcpy(nodeName[1],"Entry0");
			if(getAttrValue(top, nodeName, "WPSConfMode", tmp) != TCAPI_PROCESS_OK){
				tcdbg_printf("WPSConfMode = NULL\n");
			}
			else if((strcmp(tmp, "1") == 0)||(strcmp(tmp, "2") == 0)
				||(strcmp(tmp, "7") == 0)){
				fp = fopen(WLAN_AC_SCRIPT_PATH,"w");
				if(fp != NULL){
					if(isWPSACRunning(top)){
						fprintf(fp, "/userfs/bin/iwpriv rai0 set WscStop\n");
					}else{
						fprintf(fp, "/userfs/bin/iwpriv rai0 set WscGenPinCode=1\n");
					}
					fclose(fp);
					chmod(WLAN_AC_SCRIPT_PATH,S_IRUSR|S_IWUSR|S_IXUSR);
					return 1;
				}else{
					tcdbg_printf("can't open file in %s\n", __FUNCTION__);
				}
			}else{
				tcdbg_printf("WPSConfMode is wrong!!!\n");
			}
		}
	}else{
		tcdbg_printf("WPSGenPinCode = NULL\n");
	}
	return 0;
}
#endif

#ifdef WSC_AP_SUPPORT/*add by xyyou	*/
int
isWPSACRunning(mxml_node_t * top){
  	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], tmp[160];
	int isWPSrunning=0;

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],"Info");
	strcpy(nodeName[1],"WLan11ac");

	/*tcapi_get("Info_WLan", "wlanWPStimerRunning", tmp);*/
	
	if(getAttrValue(top, nodeName, "wlanWPStimerRunning", tmp)
		== TCAPI_PROCESS_OK){
		if(atoi(tmp)==1){
			isWPSrunning=1;
		}
	}	
	return isWPSrunning;

}
int
run_ac_wps(mxml_node_t *top){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], tmp[4]={0};
	FILE *fp;
#ifdef BBUTOWBU
	char cmd[50]={0};
#endif
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],"Info");
	strcpy(nodeName[1],"WLan11ac");
	if(getAttrValue(top, nodeName, "WPSActiveStatus", tmp) == TCAPI_PROCESS_OK){
		if(strcmp(tmp, "0") == 0){/* no active */
			tcdbg_printf("WPSActiveStatus = 0\n");
		}
		else{ 
			memset(nodeName,0,sizeof(nodeName));
			strcpy(nodeName[0],"WLan11ac");
			strcpy(nodeName[1],"Entry0");
			if(getAttrValue(top, nodeName, "WPSConfMode", tmp) != TCAPI_PROCESS_OK){
				tcdbg_printf("WPSConfMode = NULL\n");
			}
			else if((strcmp(tmp, "1") == 0)||(strcmp(tmp, "2") == 0)
				||(strcmp(tmp, "7") == 0)){
				fp = fopen(WLAN_AC_SCRIPT_PATH,"w");
				if(fp != NULL){
					
						if(isWPSACRunning(top)){
							fprintf(fp, "/userfs/bin/iwpriv rai0 set WscStop\n");
						}else{
							if(getAttrValue(top, nodeName, "WPSConfMode", tmp)
								== TCAPI_PROCESS_OK){
								fprintf(fp, WLAN_AC_SCRIPT_PREFIX, 0
									, "WscConfMode", tmp);/* set wsc conf mode */
							}
							
							if(getAttrValue(top, nodeName, "WPSConfStatus", tmp)
								== TCAPI_PROCESS_OK){
								if(strcmp(tmp, "1") == 0){
									fprintf(fp, WLAN_AC_SCRIPT_PREFIX, 0
										, "WscConfStatus", "1");
								}else if(strcmp(tmp, "2") == 0){
									fprintf(fp, WLAN_AC_SCRIPT_PREFIX, 0
										, "WscConfStatus", "2");
								}else{
									tcdbg_printf("error in WscConfStatus!!!\n\n");
									return 0;
								}
							}
							else{
								fprintf(fp, WLAN_AC_SCRIPT_PREFIX, 0
									, "WscConfStatus", "1");/*unconfigured*/
							}
							if(getAttrValue(top, nodeName, "WPSMode", tmp) 
								== TCAPI_PROCESS_OK){
								if(strcmp(tmp, "0") == 0){/* Pin code */
									fprintf(fp, WLAN_AC_SCRIPT_PREFIX, 0, "WscMode", "1");
									if(getAttrValue(top, nodeName, "enrolleePinCode", tmp) == TCAPI_PROCESS_OK){
										fprintf(fp, WLAN_AC_SCRIPT_PREFIX, 0, "WscPinCode", tmp);
									}

									/*
									set genPinCode
									*/
									if(getAttrValue(top, nodeName,
										"genPinCodeSupport", tmp) 
										== TCAPI_PROCESS_OK)
									{
									if (strcmp(tmp, "Yes")==0)
									fprintf(fp, WLAN_AC_SCRIPT_PREFIX, 0,
									"WscGenPinCode", tmp);
									}       
								}
								else if(strcmp(tmp, "1") == 0){/* PBC */
									fprintf(fp, WLAN_AC_SCRIPT_PREFIX, 0,
										"WscMode", "2");
								}
							}
							fprintf(fp, WLAN_AC_SCRIPT_PREFIX,
								0, "WscGetConf", "1");/* start WPS */
				
					}
					
					fclose(fp);
					chmod(WLAN_AC_SCRIPT_PATH,S_IRUSR|S_IWUSR|S_IXUSR);
					return 1;/* wlan execute will only run wps*/
				}
				else{
					tcdbg_printf("can't open file \n");
				}
			}
			else{
				tcdbg_printf("WPSConfMode is wrong!!!\n");
			}
		}
	}
	else{
		tcdbg_printf(" run_ac_wps :WPSActiveStatus = NULL\n");
	}
	return 0;
}
int
wps_ac_oob(mxml_node_t *top){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], tmp[4]={0};
	FILE *fp;
#ifdef BBUTOWBU
	char cmd[50]={0};
#endif
	
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],"Info");
	strcpy(nodeName[1],"WLan11ac");
	if(getAttrValue(top, nodeName, "WPSOOBActive", tmp) == TCAPI_PROCESS_OK){
		if(strcmp(tmp, "0") == 0){/* no active */
			tcdbg_printf("WPSOOBActive = 0\n");
		}
		else{ 
			memset(nodeName,0,sizeof(nodeName));
			strcpy(nodeName[0],"WLan11ac");
			strcpy(nodeName[1],"Entry0");
			if(getAttrValue(top, nodeName, "WPSConfMode", tmp) != TCAPI_PROCESS_OK){
				tcdbg_printf("WPSConfMode = NULL\n");
			}
			else if((strcmp(tmp, "1") == 0)||(strcmp(tmp, "2") == 0)
				||(strcmp(tmp, "7") == 0)){
				fp = fopen(WLAN_AC_SCRIPT_PATH,"w");
				if(fp != NULL){
					if(isWPSACRunning(top)){
						fprintf(fp, "/userfs/bin/iwpriv rai0 set WscStop\n");
					}else{
#ifdef BBUTOWBU
						fprintf(fp, "/userfs/bin/iwpriv rai0 set WscOOB=1\n");
#else
						fprintf(fp, "/userfs/bin/iwpriv rai0 set WscOOB\n");
#endif						
						WscOOBSeted_ac=1;
					}
					fclose(fp);
					chmod(WLAN_AC_SCRIPT_PATH,S_IRUSR|S_IWUSR|S_IXUSR);
					return 1;
				}else{
					tcdbg_printf("can't open file in wps_oob()\n");
				}
			}else{
				tcdbg_printf("WPSConfMode is wrong!!!\n");
			}
		}
	}else{
		tcdbg_printf("WPSOOBActive = NULL\n");
	}
	return 0;
}
#endif /* WSC_AP_SUPPORT */	
#endif /* TCSUPPORT_WLAN_AC */
#endif
#ifdef ALIAS_IP
/*______________________________________________________________________________
**	dhcp_main_alias
**
**	descriptions:
**		decide dhcp uses main ip or alias ip.
**
**	parameters:
**		mainIP: 	main ip address.
**		aliasIP: 	alias ip address.
**		startIP: 	dhcp start ip address.
		main_or_alias:  indicate dhcp use main ip or alias ip.
				1: dhcp uses main ip; 2: dhcp users alias ip.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
dhcp_main_alias(const char *mainMask, const char *mainIP, const char *aliasMask, const char *aliasIP, const char *startIP, short *main_or_alias){
	struct in_addr iMainMask;
  	struct in_addr iMainIP;
	struct in_addr iAliasMask;
	struct in_addr iAliasIP;
	struct in_addr iStartIP;
	short iMain = 1, iAlias = 1;
	int ret;
	ret = inet_aton (mainMask, &iMainMask);
	if (ret == 0)
		return FAIL;
	ret = inet_aton (mainIP, &iMainIP);
	if (ret == 0)
		return FAIL;
	ret = inet_aton (aliasMask, &iAliasMask);
	if (ret == 0)
		return FAIL;
	ret = inet_aton (aliasIP, &iAliasIP);
	if (ret == 0)
		return FAIL;
	ret = inet_aton (startIP, &iStartIP);
	if (ret == 0)
		return FAIL;
	if((iMainMask.s_addr & iMainIP.s_addr) != (iMainMask.s_addr & iStartIP.s_addr))
			iMain = 0;
	
	if((iAliasMask.s_addr & iAliasIP.s_addr) != (iAliasMask.s_addr & iStartIP.s_addr))
			iAlias = 0;
	
	if(0 == (iMain | iAlias))
	{
		*main_or_alias = 0;
		return FAIL;
	}else if(1 == iMain)
	{
		*main_or_alias = 1;
		return SUCCESS;
	}else
	{
		*main_or_alias = 2;
		return SUCCESS;
	}
	return SUCCESS;
}
#endif
#if defined(TCSUPPORT_CT_PPPOEPROXY)

#define PPPOEPROXYOFFSET 0
int 
pppoeproxy_write(mxml_node_t *top, mxml_node_t *parant)
{
	return SUCCESS;
}

int 
pppoeproxy_verify(mxml_node_t *node)
{
	return SUCCESS;
}

int update_interface_conf(char name[][MAX_NODE_NAME])
{
	char iffolder[32] = {0}, ifname[16] = {0}, string[64] = {0};
	mxml_node_t *node = NULL;
	mxml_node_t *top = get_node_top();

	if(getAttrValue(top, name, WAN_IFNAME, ifname) != TCAPI_PROCESS_OK ){
		return FAIL;
	}
	/*create wan interface folder by interface name,ext:/var/run/nas0_1*/
	sprintf(iffolder, "%s%s", WAN_IF_PATH, ifname);
	
	/*create interface.conf*/
	sprintf(string, "%s/%s", iffolder, "interface.conf");
	node = mxmlFindElement(top, top, name[0],
	  		NULL, NULL,
	  		MXML_DESCEND);
	node = mxmlFindElement(node, node, name[1],
	  		NULL, NULL,
	  		MXML_DESCEND);
	node = mxmlFindElement(node, node, name[2],
	  		NULL, NULL,
	  		MXML_DESCEND);
	if(node_write(node, string, QMARKS) != 0){
		tcdbg_printf("\nupdate_interface_conf fail!%s", string);
		return FAIL;
	}

	return SUCCESS;	
}

int 
pppoeproxy_execute(mxml_node_t *top, char name[][MAX_NODE_NAME])
{
	char buf[64] = {0};
	char user[64] = {0}, passwd[64] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	int unit = 0;
	char pskeyword[20] = {0};
	int fd = 0, pid = 0, ID = -1;
	char content[5] = {0};
	char pidfile[20] = {0};
	FILE *fp = NULL;
	char proxycmd[50] = {0};
	int pvc_index = 0;
	int entry_index = 0;
	char *pt = NULL;
	char *output = NULL;
	
	//tcdbg_printf("pppoe: wan interface up...\n");	

	memset(nodeName,0,sizeof(nodeName));
	strncpy(nodeName[0],"PPPOEPROXY", sizeof(nodeName[0]));
	strncpy(nodeName[1],"Entry", sizeof(nodeName[1]));

	if(getAttrValue(top, nodeName, "Unit", buf) == TCAPI_PROCESS_OK){	
		unit = atoi(buf);
	}

	getAttrValue(top, nodeName, "User", user);
	getAttrValue(top, nodeName, "Passwd", passwd);
	
	ID = unit-PPPOEPROXYOFFSET;
	pvc_index = ID/PVC_ENTRY_NUM;
	entry_index = ID%PVC_ENTRY_NUM;
	
	cmdPipe("ps", &output);
	snprintf(pskeyword, sizeof(pskeyword), "pppd unit %d",unit);

	/*kill wan pppd if exists*/
	if(strstr(output, pskeyword)){
		snprintf(pidfile, sizeof(pidfile), "/var/run/pppd%d.pid",unit);
		fd = open(pidfile, O_RDONLY);
		
		if(fd!=-1){
			read(fd,content,5);
			pid=atoi(content);
			close(fd);
			kill(pid,SIGKILL);
		}
	}

	free(output);
	memset(nodeName,0,sizeof(nodeName));
	strncpy(nodeName[0],"Wan", sizeof(nodeName[0]));
	snprintf(nodeName[1],sizeof(nodeName[1]),"PVC%d", pvc_index);
	snprintf(nodeName[2],sizeof(nodeName[2]),"Entry%d", entry_index);
	setAttrValue(top, nodeName, "USERNAME", user);
	setAttrValue(top, nodeName, "PASSWORD", passwd);
	/*update wan interface conf*/
	update_interface_conf(nodeName);
	snprintf(proxycmd, sizeof(proxycmd), "%s ppp%d %d 1 0 1","/usr/script/wan_start.sh", ID, pvc_index);
	system(proxycmd);
	
	return SUCCESS;
}

/*______________________________________________________________________________
**	pppoeproxy_init
**
**	descriptions:
**		the init function of pppoeproxy.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
pppoeproxy_init(void){
	int ret=0;
	cfg_node_t node={
			.name="PPPOEPROXY",
			.type=SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=pppoeproxy_write,
			.cfg_verify=pppoeproxy_verify,
			.cfg_execute=pppoeproxy_execute,
			.cfg_boot=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}
#endif

#if defined(TCSUPPORT_IGMP_PROXY)
/*______________________________________________________________________________
**	igmpproxy_init
**
**	descriptions:
**		the init function of igmpproxy.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
igmpproxy_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="IGMPproxy",
			.type=SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=igmpproxy_write,
			.cfg_verify=igmpproxy_verify,
			.cfg_execute=igmpproxy_execute,
#if defined(TCSUPPORT_CT_DSL_EX)
			.cfg_boot=igmpproxy_boot,
#else
			.cfg_boot=NULL,
#endif
			.cfg_pre_unset=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}

#if defined(TCSUPPORT_CT_DSL_EX)
int
igmpproxy_boot(mxml_node_t *top){
	copy_to_igmpproxy_node();
	return SUCCESS;
}

#endif

/*______________________________________________________________________________
**	igmpproxy_write
**
**	descriptions:
**		Find igmpproxy element from mxml tree then write igmpproxy's system parameter to specify
**		configuration files.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  igmpproxy cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
igmpproxy_write(mxml_node_t *top, mxml_node_t *parant){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char quickleave[5] = {0}, upstreamif[16] = {0};
	FILE *fp=NULL;
	char buf[128]={0};

	memset(nodeName, 0, sizeof(nodeName));
  	strcpy(nodeName[0],"IGMPproxy");
  	strcpy(nodeName[1], "Entry");

	if( getAttrValue(top, nodeName, "UpstreamIF", upstreamif) == TCAPI_PROCESS_OK ){
		fp = fopen(IGMPPROXY_CONF_PATH,"w");
		if(fp == NULL){
			return FAIL;
		}

		if ( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "QuickLeave", quickleave) )
			strcpy(quickleave, "Yes");
		/*pass condition 1~4, then generate igmpproxy.conf according to isp, quickleave, pvc*/
		if(strcmp(quickleave,"Yes") == 0)
			sprintf(buf,"quickleave\n");
		else{
			sprintf(buf,"#quickleave\n");
		}
		fputs(buf, fp);		
		sprintf(buf,"phyint %s upstream  ratelimit 0  threshold 1\n",upstreamif);		
		fputs(buf, fp);
		sprintf(buf,"phyint br0 downstream  ratelimit 0  threshold 1 \n");
		fputs(buf, fp);
		fclose(fp);
	}
	return SUCCESS;
}/* end dproxy_write */

/*______________________________________________________________________________
**	igmpproxy_verify
**
**	descriptions:
**		To check the system parameter of igmpproxy element  is validly or not.
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
igmpproxy_verify(mxml_node_t *node){
	return SUCCESS;
}/* end dproxy_verify */

/*_____________________________________________________________________________
**      function name: igmpproxy_execute
**      descriptions:
**            To execute igmpproxy node service.
**
**      parameters:
**            node:   Put the igmpproxy of cfg node tree that is used to execute igmpproxy settings.
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
**____________________________________________________________________________
*/
int
igmpproxy_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char active[5] = {0}, upstreamif[16] = {0}, wan_ip[19] = {0}, lan_ip[19] = {0}, isp[3] = {0};
	int wan_if_index = 0;
	#if 0
	FILE *fp=NULL;
	char igmpproxy_pid_tmp[6] = {0};
	char buf[128]={0};
	int igmpproxy_pid = 0;
	#endif

	/*kill igmpproxy process*/
	system("killall igmpproxy");
	tcdbg_printf("IGMP proxy process down!\n");

	memset(nodeName, 0, sizeof(nodeName));
  	strcpy(nodeName[0],"IGMPproxy");
  	strcpy(nodeName[1], "Entry");

	if(getAttrValue(top, nodeName, "Active", active) == TCAPI_PROCESS_OK &&
		getAttrValue(top, nodeName, "UpstreamIF", upstreamif) == TCAPI_PROCESS_OK) {
		if(!strcmp(active, ACTIVE)){
			/*Check wan interface whether get IP*/
			if((wan_if_index = get_wanindex_by_name(upstreamif)) < 0){
				tcdbg_printf("%s:get wan index failed\n", __func__);
				return FAIL;
			}

			if(get_waninfo_by_index(wan_if_index, "ISP", isp) != SUCCESS){
				tcdbg_printf("%s:get wan ISP failed\n", __func__);
				return FAIL;
			}

			if(!strcmp(isp, BRIDGE_MODE)){
				tcdbg_printf("%s:upstream interface %s is bridge mode, igmpproxy will not be start\n", __func__, upstreamif);
				return FAIL;
			}						
			
			memset(nodeName, 0, sizeof(nodeName));
		  	strcpy(nodeName[0],"WanInfo");
		  	sprintf(nodeName[1], "Entry%d", wan_if_index);
			getAttrValue(top, nodeName, "IP", wan_ip);

			//tcdbg_printf("%s:upstream ip is %s(%d)\n", __func__, wan_ip, strlen(wan_ip));

			memset(nodeName, 0, sizeof(nodeName));
		  	strcpy(nodeName[0],"Lan");
		  	sprintf(nodeName[1], "Entry%d", 0);
			getAttrValue(top, nodeName, "IP", lan_ip);

			if(strlen(wan_ip) != 0 && strlen(lan_ip) != 0){								
				system(IGMPPROXY_DEA_PATH);
				tcdbg_printf("IGMP proxy process on!\n");
			}
		}
		
	}
	
	return SUCCESS;
}
#endif

#ifdef TCSUPPORT_MLD_PROXY
/*______________________________________________________________________________
**	mldproxy_init
**
**	descriptions:
**		the init function of mldproxy.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
mldproxy_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="MLDproxy",
			.type=SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=mldproxy_write,
			.cfg_verify=mldproxy_verify,
			.cfg_execute=mldproxy_execute,
#if defined(TCSUPPORT_CT_DSL_EX)
			.cfg_boot=mldproxy_boot,
#else
			.cfg_boot=NULL,
#endif
			.cfg_pre_unset=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}

#if defined(TCSUPPORT_CT_DSL_EX)
int
mldproxy_boot(mxml_node_t *top){
	copy_to_mldproxy_node();
	return SUCCESS;
}

#endif
/*______________________________________________________________________________
**	mldproxy_write
**
**	descriptions:
**		Find igmpproxy element from mxml tree then write mldproxy's system parameter to specify
**		configuration files.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  mldproxy cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
mldproxy_write(mxml_node_t *top, mxml_node_t *parant){	
	return SUCCESS;
}/* end dproxy_write */

/*______________________________________________________________________________
**	mldproxy_verify
**
**	descriptions:
**		To check the system parameter of mldproxy element  is validly or not.
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
mldproxy_verify(mxml_node_t *node){
	return SUCCESS;
}/* end dproxy_verify */

/*_____________________________________________________________________________
**      function name: mldproxy_execute
**      descriptions:
**            To execute imldproxy node service.
**
**      parameters:
**            node:   Put the imldproxy of cfg node tree that is used to execute mldproxy settings.
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
**____________________________________________________________________________
*/
int
mldproxy_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char active[5] = {0}, upstreamif[16] = {0}, status6[6] = {0}, isp[3] = {0};
#ifdef TCSUPPORT_CUC
    char ServiceList[48] = {0};
#endif
	int wan_if_index = 0;
	char buf[32]={0};


	/*kill mld proxy process*/
	system("killall ecmh");
	system("/userfs/bin/hw_nat -!");
	tcdbg_printf("MLD proxy process down!\n");

	memset(nodeName, 0, sizeof(nodeName));
  	strcpy(nodeName[0],"MLDproxy");
  	strcpy(nodeName[1], "Entry");

	if(getAttrValue(top, nodeName, "Active", active) == TCAPI_PROCESS_OK &&
		getAttrValue(top, nodeName, "UpstreamIF", upstreamif) == TCAPI_PROCESS_OK) {
		if(!strcmp(active, ACTIVE)){
			/*Check wan interface whether get IP*/
			if((wan_if_index = get_wanindex_by_name(upstreamif)) < 0){
				tcdbg_printf("%s:get wan index failed\n", __func__);
				return FAIL;
			}

			if(get_waninfo_by_index(wan_if_index, "ISP", isp) != SUCCESS){
				tcdbg_printf("%s:get wan ISP failed\n", __func__);
				return FAIL;
			}

			if(!strcmp(isp, BRIDGE_MODE)){
				tcdbg_printf("%s:upstream interface %s is bridge mode, mldproxy will not be start\n", __func__, upstreamif);
				return FAIL;
			}
			
#ifdef TCSUPPORT_CUC 
            // only internet and IPTV wan can be used to be igmp/mld proxy
			if(get_waninfo_by_index(wan_if_index, "ServiceList", ServiceList) != SUCCESS){
				tcdbg_printf("%s:get wan ServiceList failed\n", __func__);
				return FAIL;
			}

			if(NULL == strstr(ServiceList, "IPTV") && NULL == strstr(ServiceList, "INTERNET")){
				tcdbg_printf("%s:upstream interface %s is not for internet nor for IPTV, mldproxy will not be start\n", __func__, upstreamif);
				return FAIL;
			}
#endif
			
			memset(nodeName, 0, sizeof(nodeName));
		  	strcpy(nodeName[0],"WanInfo");
		  	sprintf(nodeName[1], "Entry%d", wan_if_index);
			getAttrValue(top, nodeName, "Status6", status6);

			if(!strcmp(status6, "up")){
				sprintf(buf,"/userfs/bin/ecmh -i %s",upstreamif); 
				system(buf);
				tcdbg_printf("MLD proxy process on!\n");
			}
			else{
				//tcdbg_printf("%s:%s is down\n", __func__, upstreamif);
			}
		}
		
	}
	
	return SUCCESS;
}
#endif

#ifdef TCSUPPORT_IPV6

#if defined(TCSUPPORT_CT_PORT_BIND)
void checkLanBind(mxml_node_t *top){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char lanbindCheck[8] = {0};

	memset(nodeName, 0, sizeof(nodeName));
  	strcpy(nodeName[0],"Sys");
  	strcpy(nodeName[1], "Entry");
	if(getAttrValue(top, nodeName, "lanbindCheck", lanbindCheck) == TCAPI_PROCESS_OK && strcmp(lanbindCheck,"1") == 0){
		system("/usr/bin/portbindcmd checkLanBind 1");
	}else{		//dsiable
		system("/usr/bin/portbindcmd checkLanBind 0");
	}
}
#endif

/*______________________________________________________________________________
**	Sys_init
**
**	descriptions:
**		the init function of Sys node.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
Sys_init(void){
	int ret=0;
	cfg_node_t node={
			.name=SYS_NODE,
			.type=SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=Sys_write,
			.cfg_verify=Sys_verify,
			.cfg_execute=Sys_execute,
			.cfg_boot=Sys_boot,
			.cfg_pre_unset=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}
/*______________________________________________________________________________
**	Sys_write
**
**	descriptions:
**	
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 		Specify the parant node of  igmpproxy cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
Sys_write(mxml_node_t *top, mxml_node_t *parant){
	return SUCCESS;
}/* end Sys_write */

/*______________________________________________________________________________
**	Sys_verify
**
**	descriptions:
**		To check the system parameter of system element  is validly or not.
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
Sys_verify(mxml_node_t *node){
	return SUCCESS;
}/* end dproxy_verify */

/*_____________________________________________________________________________
**      function name: Sys_execute
**      descriptions:
**            To execute Sys node service.
**
**      parameters:
**            node:   Put the Sys of cfg node tree that is used to execute ipv6 settings.
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
**____________________________________________________________________________
*/
int
Sys_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
#if defined(TCSUPPORT_CT_DSL_EX)
	int dsl_state = 0;		
	dsl_state = get_dsl_state();
	if(dsl_state != 1 && dsl_state != 2 ){
		copy_wan_node(0);			//copy only when dsl is down
	}else{
		tcdbg_printf("Sys_execute do not copy_wan_node when dsl is up\n");
	}
#endif
	setIpv6Radio(top);
#if defined(TCSUPPORT_CT_PORT_BIND)
	checkLanBind(top);
#endif

	return SUCCESS;
}
/*______________________________________________________________________________
**	Sys_boot
**
**	descriptions:
**		the boot function of Sys node.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
Sys_boot(mxml_node_t *top){
	setIpv6Radio(top);
#if defined(TCSUPPORT_CT_PORT_BIND)
	checkLanBind(top);
#endif
	
	return SUCCESS;
}
#endif

#if !defined(TCSUPPORT_CUC) && !defined(TCSUPPORT_CMCC)
int LogicID_init(void)
{
	int ret=0;
	cfg_node_t node={
			.name=LOGICID_NODE,
			.type=SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=LogicID_execute,
			.cfg_boot=LogicID_boot,
			.cfg_pre_unset=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}
int LogicID_execute(mxml_node_t *top, char name[][MAX_NODE_NAME])
{
	return LogicID_check(top);
}

int LogicID_boot(mxml_node_t *top)
{
	return LogicID_check(top);
}

int LogicID_check(mxml_node_t *top)
{
	char v_value[128] = {0};
	char isTR69Chg[12] = {0};
	int intIsTR69Chg = 0;
	int intisUserNamechg = 0;
	char username[128] = {0}, password[128] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char webCurNode[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
#if defined(CT_COM_DEVICEREG) || defined(TCSUPPORT_CT_PON_BIND2_WEB)
	char dev_nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char cwmp_node[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char tmpval[10] = {0};
	int isNOBootRegister = 0;
#ifdef CWMP
	cwmp_msg_t message;
	long type = 1;//tr69 must be 1
	int msgFlag = IPC_NOWAIT;//0;
	int mq = 2; //value change type 
#endif
#endif
#if defined(TCSUPPORT_CT_PON)
	char epon_nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char gpon_nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
#endif

	memset(nodeName, 0, sizeof(nodeName));
	memset(webCurNode, 0, sizeof(webCurNode));
#if defined(CT_COM_DEVICEREG) || defined(TCSUPPORT_CT_PON_BIND2_WEB)
	memset(dev_nodeName, 0, sizeof(dev_nodeName));
	memset(cwmp_node, 0, sizeof(cwmp_node));
#endif
#if defined(TCSUPPORT_CT_PON)
	memset(epon_nodeName, 0, sizeof(epon_nodeName));
	memset(gpon_nodeName, 0, sizeof(gpon_nodeName));
#endif

	strcpy(nodeName[0], LOGICID_NODE);
	strcpy(nodeName[1], LOGICID_NODE_ENTRY);
	strcpy(webCurNode[0], "WebCurSet");
	strcpy(webCurNode[1], "Entry");
#if defined(CT_COM_DEVICEREG) || defined(TCSUPPORT_CT_PON_BIND2_WEB)
	strcpy(dev_nodeName[0], "deviceAccount");
	strcpy(dev_nodeName[1], "Entry");
	strcpy(cwmp_node[0], "Cwmp");
	strcpy(cwmp_node[1], "Entry");
#endif
#if defined(TCSUPPORT_CT_PON)
	strcpy(epon_nodeName[0], "EPON");
	strcpy(epon_nodeName[1], "LOIDAuth");
	strcpy(gpon_nodeName[0], "GPON");
	strcpy(gpon_nodeName[1], "LOIDAuth");
#endif

	memset(isTR69Chg, 0, sizeof(isTR69Chg));
	if( TCAPI_PROCESS_OK == getAttrValue(top, webCurNode, "isTR69Chg", isTR69Chg)
		&& 0 == strcmp(isTR69Chg, "1") )
	{
		setAttrValue(top, webCurNode, "isTR69Chg", "0");
		intIsTR69Chg = 1;
	}
	else
		intIsTR69Chg= 0;


	 // restore the correct informations
#ifdef CT_COM_DEVICEREG
#if !defined(TCSUPPORT_CT_PON) && !defined(TCSUPPORT_CT_E8B_ADSL)
	 memset(v_value, 0, sizeof(v_value) );
	 if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, LOGICID_REG_STATUS, v_value) )
		strcpy(v_value, "99");
	 setAttrValue(top, dev_nodeName, DEVACCOUNT_STATUS, v_value);

	memset(v_value, 0, sizeof(v_value) );
	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, LOGICID_REG_RESULT, v_value) )
		strcpy(v_value, "99");
	setAttrValue(top, dev_nodeName, DEVACCOUNT_RESULT, v_value);
#endif
#endif

	memset(v_value, 0, sizeof(v_value) );
	if( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, LOGICID_USERNAME, v_value)
		&& (('\0' != v_value[0]) || ('\0' == v_value[0] && (1 == intIsTR69Chg
#if defined(TCSUPPORT_CT_PON_C7)
		|| (1) /* alway use the loid from <LogicID> node. */
#endif
		))) )
	{
#if defined(CT_COM_DEVICEREG) || defined(TCSUPPORT_CT_PON_BIND2_WEB)
		setAttrValue(top, dev_nodeName, DEVACCOUNT_USERNAME, v_value);
#endif
#if defined(TCSUPPORT_CT_PON)
		setAttrValue(top, epon_nodeName, EPON_LOID_USR, v_value);
		setAttrValue(top, gpon_nodeName, GPON_LOID_USR, v_value);
#endif
		intisUserNamechg = 1;
	}

	memset(v_value, 0, sizeof(v_value) );
	if( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, LOGICID_PSW, v_value)
		&& (('\0' != v_value[0]) || ('\0' == v_value[0] && (1 == intIsTR69Chg
		|| 1 == intisUserNamechg
		))) )
	{
#if defined(CT_COM_DEVICEREG) || defined(TCSUPPORT_CT_PON_BIND2_WEB)
		setAttrValue(top, dev_nodeName, DEVACCOUNT_USERPASSWD, v_value);
#endif
#if defined(TCSUPPORT_CT_PON)
		setAttrValue(top, epon_nodeName, EPON_LOID_PSW, v_value);
		setAttrValue(top, gpon_nodeName, GPON_LOID_PSW, v_value);
#endif
	}

#if defined(TCSUPPORT_CT_PON)
	tcapi_commit_internal(top, "EPON_LOIDAuth");
	tcapi_commit_internal(top, "GPON_LOIDAuth");
#endif
	
#ifdef CT_COM_DEVICEREG
	memset(tmpval, 0 , sizeof(tmpval));
	if ( TCAPI_PROCESS_OK != getAttrValue(top, dev_nodeName, "isNOBootRegister", tmpval) )
		strcpy(tmpval, "0");
	isNOBootRegister = atoi(tmpval);
#if defined(TCSUPPORT_CT_ADSL_HN)
	setAttrValue(top, cwmp_node, "devregInform", "0");
#endif

	tcapi_commit_internal(top, "deviceAccount_Entry");

	if ( TCAPI_PROCESS_OK != getAttrValue(top, dev_nodeName, DEVACCOUNT_USERNAME, username) )
		username[0] = '\0';
	if ( TCAPI_PROCESS_OK != getAttrValue(top, dev_nodeName, DEVACCOUNT_USERPASSWD, password) )
		password[0] = '\0';
	// register ITMS+ again if device state is locked.
	memset(v_value, 0, sizeof(v_value) );
	if( 0 == isNOBootRegister
		&& TCAPI_PROCESS_OK == getAttrValue(top, dev_nodeName, DEVACCOUNT_WEBPAGELOCK, v_value)
		&& 0 != strcmp(v_value, "0")
		&& '\0' != username[0] && '\0' != password[0] )
	{
#if !defined(TCSUPPORT_CT_FJ)
		setAttrValue(top, cwmp_node, "devregInform", "1");
#endif
		setAttrValue(top, dev_nodeName, "newStatus", "0");
		setAttrValue(top, dev_nodeName, "newResult", "0");
#ifdef CWMP
		memset(&message, 0, sizeof(cwmp_msg_t));
		message.cwmptype = mq;
		if ( sendmegq(type,&message,msgFlag) < 0 )
			tcdbg_printf("\nLogicID_boot==> send message error!\n");
#endif
	}

#endif

	return SUCCESS;
}

#if defined(TCSUPPORT_CT_PON)
extern int g_wan_boot_finish;
static int ponState = 0;
static int olt_reg_counter = 0;
static int isOLT_downup = 0;
static int isromfile_chg = 0;
#define	OLT_REG_SUCC_TIME_1	30
#define	OLT_REG_SUCC_TIME_2	60
void resetOLT_TimerBySave()
{
	isromfile_chg = 1;
	olt_reg_counter = 0;
}
int setPonState(int state)
{
	ponState = state;
	if ( 0 == ponState )
	{
		isOLT_downup = 1;
		olt_reg_counter = 0;
	}

	return 0;
}
int getPonState()
{
	return ponState;
}
int saveLoid()
{
	tcapi_msg_t msg;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char epon_nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char v_username_old[128] = {0};
	char v_password_old[128] = {0};
	char v_username_new[128] = {0};
	char v_password_new[128] = {0};
	mxml_node_t *top = NULL;

	top = get_node_top();

	memset(nodeName, 0, sizeof(nodeName));
	memset(epon_nodeName, 0, sizeof(epon_nodeName));

	strcpy(nodeName[0], LOGICID_NODE);
	strcpy(nodeName[1], LOGICID_NODE_ENTRY);
	strcpy(epon_nodeName[0], "EPON");
	strcpy(epon_nodeName[1], "LOIDAuth");

	memset(v_username_old, 0, sizeof(v_username_old) );
	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, LOGICID_USERNAME, v_username_old) )
		v_username_old[0] = '\0';

	memset(v_password_old, 0, sizeof(v_password_old) );
	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, LOGICID_PSW, v_password_old) )
		v_password_old[0] = '\0';

	// get loid from epon only, because epon loid will be same with gpon
	memset(v_username_new, 0, sizeof(v_username_new) );
	if( TCAPI_PROCESS_OK != getAttrValue(top, epon_nodeName, EPON_LOID_USR, v_username_new)
		|| '\0' == v_username_new[0] )
		return -1;
	memset(v_password_new, 0, sizeof(v_password_new) );
	if( TCAPI_PROCESS_OK != getAttrValue(top, epon_nodeName, EPON_LOID_PSW, v_password_new)
		)
		return -1;

	if ( 0 != strcmp(v_username_old, v_username_new)
		|| 0 != strcmp(v_password_old, v_password_new) )
	{
		setAttrValue(top, nodeName, LOGICID_USERNAME, v_username_new);
		setAttrValue(top, nodeName, LOGICID_PSW, v_password_new);
#if defined(TCSUPPORT_CT_PON_CN_JS)								
		setAttrValue(top, nodeName, "devReged", "1");
		tcapi_save();
#endif
		memset(&msg,0,sizeof(msg)) ;
		//tcapi_save_req(top, &msg);
		tcapi_save();
		tcdbg_printf("\nnew loid is saved! \n");
	}

	return 0;
}

int saveLoid_internal()
{
	tcapi_msg_t msg;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char epon_nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char v_username_old[128] = {0};
	char v_password_old[128] = {0};
	char v_username_new[128] = {0};
	char v_password_new[128] = {0};
	mxml_node_t *top = NULL;

	top = get_node_top();

	memset(nodeName, 0, sizeof(nodeName));
	memset(epon_nodeName, 0, sizeof(epon_nodeName));

	strcpy(nodeName[0], LOGICID_NODE);
	strcpy(nodeName[1], LOGICID_NODE_ENTRY);
	strcpy(epon_nodeName[0], "EPON");
	strcpy(epon_nodeName[1], "LOIDAuth");

	memset(v_username_old, 0, sizeof(v_username_old) );
	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, LOGICID_USERNAME, v_username_old) )
		v_username_old[0] = '\0';

	memset(v_password_old, 0, sizeof(v_password_old) );
	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, LOGICID_PSW, v_password_old) )
		v_password_old[0] = '\0';

	// get loid from epon only, because epon loid will be same with gpon
	memset(v_username_new, 0, sizeof(v_username_new) );
	if( TCAPI_PROCESS_OK != getAttrValue(top, epon_nodeName, EPON_LOID_USR, v_username_new)
		|| '\0' == v_username_new[0] )
		return -1;
	memset(v_password_new, 0, sizeof(v_password_new) );
	if( TCAPI_PROCESS_OK != getAttrValue(top, epon_nodeName, EPON_LOID_PSW, v_password_new)
		)
		return -1;

	if ( 0 != strcmp(v_username_old, v_username_new)
		|| 0 != strcmp(v_password_old, v_password_new) )
	{
		setAttrValue(top, nodeName, LOGICID_USERNAME, v_username_new);
		setAttrValue(top, nodeName, LOGICID_PSW, v_password_new);

		memset(&msg,0,sizeof(msg)) ;
		tcapi_save_req(top, &msg);
		tcdbg_printf("\ninternal_save(): new loid is saved!\n");
	}

	return 0;
}


int checkOLTstate()
{
	int checkTime = ( 1 == isOLT_downup ) ? OLT_REG_SUCC_TIME_1 : OLT_REG_SUCC_TIME_2;
	if ( !g_wan_boot_finish
		|| 0 == getPonState()
		|| ( 1 != isromfile_chg && 0 == isOLT_downup ))
		return -1;

	if ( olt_reg_counter <= 2 )
		tcdbg_printf("\nit will start to check loid valid=[%d]! \n", olt_reg_counter);
	if ( ++olt_reg_counter >= checkTime )
	{
		tcdbg_printf("\nloid valid check is end! \n");
		isOLT_downup = 0;
		olt_reg_counter = 0;
		isromfile_chg = 0;
		saveLoid();
	}

	return 0;
}
#endif

#endif

/*______________________________________________________________________________
**	getDefine_WLAN_PATH
**
**	descriptions:
**		used by unopen code
**	parameters:
**
**	return:
**		Success: WLAN_PATH
**		FAIL: it should never be happened
**_______________________________________________________________________________
*/
char *getDefine_WLAN_PATH(){
	return WLAN_PATH;
}

/*______________________________________________________________________________
**	getDefine_DEF_ROMFILE_PATH
**
**	descriptions:
**		used by unopen code
**	parameters:
**
**	return:
**		Success: 	DEF_ROMFILE_PATH
**		FAIL: it should never be happened
**_______________________________________________________________________________
*/
char *getDefine_DEF_ROMFILE_PATH(){
	return DEF_ROMFILE_PATH;
}

/*______________________________________________________________________________
**	getDefine_DEFAULTROMFILE_RA_SIZE
**
**	descriptions:
**		used by unopen code
**	parameters:
**
**	return:
**		Success: 	DEFAULTROMFILE_RA_SIZE
**		FAIL: it should never be happened
**_______________________________________________________________________________
*/
int getDefine_DEFAULTROMFILE_RA_SIZE(){
#if !(defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505))
	return DEFAULTROMFILE_RA_SIZE;
#else
	return 0x10000;
#endif
}

/*______________________________________________________________________________
**	getDefine_DEFAULTROMFILE_RA_OFFSET
**
**	descriptions:
**		used by unopen code
**	parameters:
**
**	return:
**		Success: DEFAULTROMFILE_RA_OFFSET
**		FAIL: it should never be happened
**_______________________________________________________________________________
*/
int getDefine_DEFAULTROMFILE_RA_OFFSET(){
#if !(defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505))
	return DEFAULTROMFILE_RA_OFFSET;
#else
	return 0x10000;
#endif
}