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
#ifndef _CFG_MANAGER_H
#define _CFG_MANAGER_H

#ifdef WITHVOIP
#define VOIP        1 /*use WITHVOIP define to decide using voip function or not. shnwind 20100203*/
#else
#define VOIP        0
#endif

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <pwd.h>
#include <unistd.h>
#include <string.h>
#include <regex.h>

#include "config.h"
#include "mxml.h"
#include "common.h"
#include "tcapi.h"
#include "interface.h"
#include "advanced.h"
#include "access.h"
#include "maintenance.h"
#include "web.h"
#include "tcapi_proc.h"
#include "wan.h"
#include<linux/ioctl.h>

#include "wan_monitor.h"
#if VOIP
#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)
#include "voip_mtk.h"
#else
#include "voip.h"
#endif
#else
#define VOIP_LINE_NUM 2
#endif
#if defined(TCSUPPORT_CT_JOYME_BANDWIDTH)
#include "bandwidth.h"
#endif



#include "../lib/libtcapi.h"

#include "../../public/mtd/tc_partition.h"

#include <pthread.h>

#define MAX_TCAPI_CLIENT	5

#define ROMFILE_TAG	"ROMFILE"
#define ROMFILE_PATH 	"/tmp/var/romfile.cfg"

#define FRAG_ROMFILE_PATH 	"/tmp/var/frag_romfile.cfg"
#define USING_ROMFILE_PATH    "/var/romfile.cfg"

#define LOAD_ROMFILE_FILE_NAME "/tmp/tc_load_romfile.conf"
#define STORE_ROMFILE_FILE_NAME "/tmp/tc_store_romfile.conf"
#define CWMP_UPGRADE_ROMFILE_FILE_NAME "/tmp/tc_cwmp_upgrade_romfile.conf"
#define TMP_CONFIG_FILE_PATH "/tmp/ctromfile.cfg"
#define GZ_TMP_PATH "/tmp/"
#define GZ_ROMFILE_FILE_NAME "romfile.gz"
#define CT_ROMFILE_FILE_NAME "ctromfile.cfg"
#define CT_CWMP_DOWNLOAD_CHKROMFILE_FILE_NAME "/tmp/cwmp_ctcheckromfile.cfg"
#define DEF_ROMFILE_PATH_EPON 	"/userfs/ctromfile_epon.cfg"
#if defined(TCSUPPORT_CT_BACKUPROMFILEENCHANCEMENT)
#define DEF_ROMFILE_PATH 	"/userfs/ctromfile.cfg"
#else
#define DEF_ROMFILE_PATH 	"/userfs/romfile.cfg"
#endif
#define UPDATE_ROMFILE_CMD	"/userfs/bin/mtd write %s romfile"
#define BR_FILE_NAME "/tmp/tc_backupromfile"
#define DR_FILE_NAME "/tmp/tc_defaultromfile"
#define ITMSR_FILE_NAME "/tmp/tc_itmsromfile"
#define UPLOAD_ROMFILE_PATH	"/var/tmp/up_romfile.cfg"
#define RUNNING_ROMFILE_PATH 	"/dev/mtdblock1"
#define BK_WEBINFO_PATH	"/tmp/web.xml"
#define TMP_CONF	"/tmp/tmp.conf"
#define TMP_PPP_PATH "/tmp/checkppp"

#define CFG_MANAGER_STATE_FILE_PATH "var/tmp/cfg_manager_up"

#ifndef TCSUPPORT_QOS
#define QOS_REMARKING  1  /*Rodney_20090724*/
#endif

#define	RUN_ROMFILE_FLAG	0
#define	BK_ROMFILE_FLAG	1
#define	DEF_ROMFILE_FLAG	2
#define	DEFFILESYSTEM_ROMFILE_FLAG	3
#define ITMS_ROMFILE_FLAG		4
#define ROMFILE_MAX_PENDING   65536
#define ROMFILE_MAX_LEN   65536

#define OP_NUM		3
#define OP_READ		0
#define OP_WRITE	1
#define OP_VERIFY	2

#define NO_QMARKS 0
#define QMARKS 1
#define NO_ATTRIBUTE 2


#ifndef PURE_BRIDGE
	#define PVC_NUM 8
#else
	#define PVC_NUM 4
#endif
/*Wan cfg node*/
#define MAX_SMUX_NUM 8
#define MAX_WAN_IF_INDEX  (PVC_NUM*MAX_SMUX_NUM)
#define USE_DEFAULT_IFNAME_FORMAT 1
//#define WAN_DBUG 1

#define MAX_VIRSERV_RULE 10
#define MAX_IPMAP_RULE 16
/*krammer : change the value from 16 to 64*/
#define MAX_ROUTE_NUM 64
/*krammer : change the value from 16 to 64*/
#define MAX_BSSID_NUM 4

#define LAN_NUM             1
#define MAX_ARG_NUM	3
#define DEF_IP	"0.0.0.0"
/**********20080825 racing add************/
#ifdef TCSUPPORT_QOS
#define MAX_QOS_RULE 30
#else
#define MAX_QOS_RULE 16
#endif
/**********20080825 racing add************/
#define MAX_ACL_RULE 16
/**********20080926 racing add************/
//#define MAX_IPMACFILTER_RULE 16
#define MAX_IPMACFILTER_RULE 	120
#define MAX_MAC_FILTER_RULE		40
#define MAX_IPUP_FILTER_RULE	40
#define MAX_IPDOWN_FILTER_RULE	40

/**********20081021 krammer add************/
#define kAscii_0            0x30
#define kAscii_9            0x39
#define kAscii_A            0x41
#define kAscii_Z            0x5A
#define kAscii_a            0x61
#define kAscii_z            0x7A
#define kAscii_Plus         0x2B
#define kAscii_Slash        0x2F
#define kAscii_Equal        0x3D

#if defined(TCSUPPORT_CT_E8DDNS)
#define MAX_DDNS_NUM 64
#define PVC_ENTRY_NUM 8
#define END_OF_ENTRY "-------end of entry-------\r\n"
#endif
#define MAX_URL_FILTER_RULE 16

#define SUB_NODE_NAME	"Entry"
#define SUB_PVC_NAME	"PVC%d"
#define NAS_IF	"nas%d"
#define PPP_IF	"ppp%d"
#define Inet_Addr  "inet addr:"
#define HWADDR   "HWaddr"
#define NAS_GATEWAY "gateway="
#define NAS_GATEWAYFILE "/etc/nas%d_gateway.conf"

#define PVC_INDEX 		"index"
#define ENTRY_INDEX 	"index"

#define ATTR_SIZE 32

#define MAXSIZE	160
#define QM_LEN	2

#define FAIL -1
#define SUCCESS 0
#define ROMFILE_ERR_MSG "The Romfile format is wrong!!!"
#define USE_DEF_ROMFILE_MSG "Romfile format is wrong, we use default romfile to replace current setting romfile!!\n"
#define ROMFILE_CHECKSUM_ERR_MSG "Romfile checksum is wrong!!!\n"
#define ROMFILE_CHECK_ERR_MSG "Romfile check is wrong!!!\n"
#define DEF_ROMFILE_ERR_MSG "Default romfile format is wrong.Please to check again!!!\n"
#define DEF_ROMFILE_ERR_MSG_2 "Running romfile format is wrong.Please to check again!!!\n"
#define OPEN_FAIL_MSG "Open fail: %s %s\n"
#define RENAME_FAIL_MSG "Rename fail: %s\n"
#define VERIFY_ERR_MSG "verify_romfile func: None %s node, will get it from default romfile\n"
#define VERIFY_ERR_MSG_2 "verify_romfile func: None %s node, will get it from running romfile\n"
#define CREATE_ROMFILE_ERR_MSG "Fail to create the romfile!!!\n"
#define VERIFY_ERR_MSG_001 "verify_romfile func: None %s node from default romfile\n"
#define VERIFY_ERR_MSG_002 "verify_romfile func: None %s node from running romfile\n"
#define VERIFY_INFO_MSG_001 "verify_romfile func: add %s node to current from default romfile\n"
#define VERIFY_INFO_MSG_002 "verify_romfile func: add %s node to current from running romfile\n"
#define UPGRADE_ROMFILE_CMD	"/userfs/bin/mtd %s write /tmp/var/romfile.cfg romfile"
#define SHIFT_INDEX 32
#define ATTR_INDEX	0
#define VALUE_INDEX	1

//filter bit map
#define WAN_VECTOR (1<<0)
#define FIREWALL_VECTOR (1<<1)
#define ACL_VECTOR (1<<2)
#define IPFILTER_VECTOR (1<<3)
#define APP_VECTOR (1<<4)
#define URL_VECTOR (1<<5)
#define ALGSW_VECTOR (1<<6)
#define QOS_VECTOR	(1<<7)

#define L7_BIT_MASK (APP_VECTOR|URL_VECTOR)
//added by xyzhu_nj_20091204
#define TCAPI_READALL_CMD 7

/* mtk06404 add 20140102 */
enum simulate_test_type{
	SIMULATE_NONE,
	SIMULATE_CALLER,
	SIMULATE_CALLED,
	SIMULATE_MAX,
};

enum simulate_state_machine{
	SIMULATE_INIT_STATE = 0,
	SIMULATE_WAIT_CONN_STATE,
	SIMULATE_DTMF_TONE_STATE,
	SIMULATE_RECVING_DTMF_STATE,
	SIMULATE_TIMEOUT_STATE,
	SIMULATE_DISCONNECT_STATE,
	SIMULATE_BLOCK_STATE,
	SIMULATE_SIP_NO_OK_STATE,
	SIMULATE_MAX_STATE,
};

#define SIMULATE_LINE_IDLE "Idle"
#define SIMULATE_LINE_DIALING "Dialing"
#define SIMULATE_LINE_RINGBACK "Ringback"
#define SIMULATE_LINE_RINGING "Ringing"
#define SIMULATE_LINE_CONNECT "Connect"
#define SIMULATE_LINE_DISCONNECT "Disconnect"
#define SIMULATE_START "simulateStart"
#define SIMULATE_CALLED_NUM "CalledNumber"

#define	SIMULATE_STOP "0"
#define SIMULATE_CFG_DONE "1"
#define SIMULATE_SEND_DTMF "2"


typedef enum simulate_cur_status{
	SIMULATE_STATUS_IDLE       = 0,
	SIMULATE_STATUS_OFFHOOK,
	SIMULATE_STATUS_DIAL_TONE,
	SIMULATE_STATUS_RECEIVING,
	SIMULATE_STATUS_RECEIVE_END,
	SIMULATE_STATUS_RINGING_BACK,
	SIMULATE_STATUS_CONNECTED,
	SIMULATE_STATUS_TEST_END,
	SIMULATE_STATUS_TEST_MAX,
}simulate_cur_status_t;

typedef enum simulate_fail_reason{
	SIMULATE_NO_ERROR   = 0,
	SIMULATE_CALLER_NO_DIAL_TONE,
	SIMULATE_CALLER_OFFHOOK_RELEASE,
	SIMULATE_CALLER_DIALING_REALEASE,
	SIMULATE_CALLER_AFTER_DIAL_REALEASE,
	SIMULATE_CALLER_NO_ANSWER,
	SIMULATE_CALLER_KEY_ERROR,

	SIMULATE_CALLED_NO_INCOMING_CALL  = 20,
	SIMULATE_CALLED_OFFHOOK_RELEASE,
	SIMULATE_CALLED_NO_ANSWER,

	SIMULATE_BLOCK_REASON_RESULT,
	SIMULATE_ERROR_MAX,
}simulate_caller_fail_reason_t;

typedef struct simulate_result_entry{
	int  errorCode;
	char *errorStr;
	char *conclusion;
}simulate_result_entry_t;

typedef struct simulate_status_entry{
	int  statusCode;
	char *statusStr;
}simulate_status_entry_t;


#if 0
typedef struct
cfg_op_s{
	int (*cfg_read)(mxml_node_t *top, char name[][MAX_NODE_NAME], char* attr);
	int (*cfg_write)(mxml_node_t *top,mxml_node_t *node);
	int (*cfg_verify)(mxml_node_t *node);
	int (*cfg_execute)(mxml_node_t *top, char name[][MAX_NODE_NAME]);
}cfg_op_t;
#endif

typedef struct
cfg_node_s{
	char *name;
	/*
	char name[MAX_NODE_NAME];
	*/
/*
    15   14  13 12  11  10   9   8   7    6   5    4   3   2    1   0       (bit order)
+----+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
| ADv |                Type          |              MAX			|
+----+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
*/
#define SINGLE_TYPE	(1 << 8)
#define PVC_TYPE	(1 << 9)
#define ENTRY_TYPE	(1 << 10)
#define WEB_TYPE	(1 << 11)
#define UPDATE_TYPE	(1 << 12)
#define ADV_FLAG	(1 << 15)
	unsigned short int   type;
	char **adv_table;
	char **merge_table;
	int (*cfg_read)(mxml_node_t *top, char name[][MAX_NODE_NAME], char* attr);
	int (*cfg_write)(mxml_node_t *top,mxml_node_t *node);
	int (*cfg_verify)(mxml_node_t *node);
	int (*cfg_execute)(mxml_node_t *top, char name[][MAX_NODE_NAME]);
	int (*cfg_boot)(mxml_node_t *top);
	int (*cfg_pre_unset)(mxml_node_t *top, char name[][MAX_NODE_NAME]);
	pthread_mutex_t *nodeLock;
}cfg_node_t;

/*used by unopen code,do not add compile option in this struct!*/
typedef struct _Backup_Parameters_Info
{
	int flag;					  /*flag to judge if node operation or attribute operation*/	
	char *node;		/*node name-----for example:Cwmp_Entry*/
	char *attr;		/*attribute name----for example:acsUrl*/	
	char value[20];
	int	(*funp)(struct _Key_Parameters_Info* parainfo,mxml_node_t* oldtree,mxml_node_t* newtree);     /* handle function poiter*/
}Backup_Parameters_Info,*Backup_Parameters_Info_Ptr;

typedef int (*node_init)(void);

int
register_cfg_node(cfg_node_t *newNode);

mxml_node_t*
parser_romfile(const char* pathname);

mxml_node_t *
load_def_romfile(char *file);

int
create_romfile(mxml_node_t *xml);

int
isNode(const char *name);

int
verify_romfile( mxml_node_t *tree);


const char*
whitespace_cb(mxml_node_t *node, int where);

void
print_mxml_tree( mxml_node_t *tree);

void
tcapi_readAll_req(tcapi_msg_t *msg);

#if 0  /*Orginal ci-command mode */
int
node_read(mxml_node_t * top, char * node_name, char * file, char * node_attr, int type);
#endif
int
node_write(mxml_node_t *node, char *file, int type);

int
updateString(char* file, const char* src, char *dst);

int
write2file(char *buf, char *pathname);

cfg_node_t *
getAllCfgTable(void);

char *getPVCGateway(int pvcindex);

#ifdef TCSUPPORT_DNSEACHPVC
char* getDNSInfo(char* type, char* isp, char* dev, char* buf);
#endif

void loadRomfile(int types);
void loadBackupRomfile(void);
void loadDefaultRomfile(void);
mxml_node_t*check_romfile(char* pathname);
mxml_node_t*tc_parse_romfile(void);
int check_checksum(char *inputfile,char *outputfile);
int compute_checksum(char *inputfile,char *outputfile);
mxml_node_t *load_checksum_def_romfile(char *file);
int getMTDInfo(const char *partition);
int update_UploadedRomfile(int imagefd, int offset, unsigned  int length, char *outputfile);
void get_backup_paras(mxml_node_t* defaulttree);
void set_backup_paras(mxml_node_t* defaulttree);

/*********shnwind add*/
#define UPNPD_PATH 	"/etc/igd/igd.conf"

/*used by unopen code,do not add compile option in this struct!*/
struct PRE_SYS_STATE{
#ifndef PURE_BRIDGE
    	char nat_raw_state;/*system use raw table NOTRACK module */
//    	char filter_m_state;	/*system load ipfilter module */
    	unsigned int filter_m_state;	/*system load ipfilter module */
#endif
    char wlan_Bssid_num;/*WLan bssid_num */
	char wlan_chann;//add by xyyou to support AutoChann
	char voip_service;	
	char cc_is_run;
//	char l7filter_state;/*system load l7filter module*/
   	char wlan11ac_Bssid_num;/*WLan11ac bssid_num */
	unsigned char wlan11ac_chann;// to support AutoChann
	int WdsEnable;
	char WdsEncrypType[5];
	int PerWdsON[MAX_WDS_ENTRY];
	char PerWdsKey[MAX_WDS_ENTRY][65];
	char WdsKey[65];
	char Wds_MAC[MAX_WDS_ENTRY][18];
	char WepKeyStr[4][27];
	int WdsEnable_11ac;
	char WdsEncrypType_11ac[5];
	int PerWdsON_11ac[MAX_WDS_ENTRY];
	char PerWdsKey_11ac[MAX_WDS_ENTRY][65];
	char WdsKey_11ac[65];
	char Wds_MAC_11ac[MAX_WDS_ENTRY][18];
	char WepKeyStr_11ac[4][27];
}pre_sys_state;


typedef struct voip_simulate_state{
	int  timeCnt;
	char testType;		/* reference simulate_test_type*/
	char state;  	/* state machine  simulate_state_machine*/
	simulate_cur_status_t line_state;  /* record current line state */
	char confirmNum[33];
	char calledNum[33];
	char confirmEnable[8];
	unsigned calledWaitTime;
	unsigned callHoldTime;
	char callConfirmDigit[33];
}voip_simulate_state_t;

/*******shnwind add end****/

#define WIFI_DOWN_CMD_RAI   	"ifconfig rai%d down"
#define WIFI_UP_CMD_RAI     	"sleep 1;ifconfig rai%d up;"
#define WIFI_DOWN_CMD_RA  		"ifconfig ra%d down"
#define WIFI_UP_CMD_RA  		"sleep 1;ifconfig ra%d up;"
#define WIFI_DOWN_CMD_WDSI  	"ifconfig wdsi%d down;"
#define WIFI_UP_CMD_WDSI     	"sleep 1;ifconfig wdsi%d up;"
#define WIFI_DOWN_CMD_WDS  		"ifconfig wds%d down"
#define WIFI_UP_CMD_WDS  		"sleep 1;ifconfig wds%d up;"

int setLandingPageEnableFlag(mxml_node_t *top);
int isLandingPageEnable();

//Unsigned16 RpDecodeBase64Data(char *theInputPtr, Unsigned16 theInputCount, char *theOutputPtr);
//Unsigned16 RpDecodeBase64DataNoNull(char *theInputPtr, Unsigned16 theInputCount, char *theOutputPtr);

void tcapi_enhance_lock(void);
void tcapi_enhance_unlock(void);
void tcapi_enhance_commit_node_queue_process(void);
void tcapi_test_lock(void);
void tcapi_test_unlock(void);
void tcapi_set_test_lock_switch(int val);

int CheckPid(int pid);
mxml_node_t *load_checksum_def_romfile_fs(char *file);
mxml_node_t *load_def_romfile_fs(char *file);
void call_notify_dslite_state();
#endif
