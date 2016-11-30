
/*
** $Id: //BBN_Linux/Branch/Branch_for_MT7505_IC_20131122/tclinux_phoenix/apps/private/TR69_64/cwmp/cwmpParameter/Includes/cwmpParameter_table.h#1 $
*/
/************************************************************************
 *
 *	Copyright (C) 2003 Trendchip Technologies, Corp.
 *	All Rights Reserved.
 *
 * Trendchip Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of Trendchip Technologies, Co. and shall
 * not be reproduced, copied, disclosed, or used in whole or in part
 * for any reason without the prior express written permission of
 * Trendchip Technologeis, Co.
 *
 *************************************************************************/

#ifndef _PARAMETER_C_
#define _PARAMETER_C_

#include "json.h"
#include "debug.h"
#include "jsonpkt.h"
#include "libcompileoption.h"

enum request_from_type{
		LOCAL_REQ,
		REMOTE_REQ,			
};

#define PERMIT_LOCAL	(0x01<<0)
#define PERMIT_REMOTE	(0x01<<1)
#define PERMIT_BOTH		0x03
#define PERMIT_NONE		0x00

typedef enum reply_json_type{
	CMD_RT_OK = 0,
	CMD_RT_ERR,
	CMD_UNRECOGNIZE,
	CMD_PERMIT_ERR,
	CMD_FUNC_BUSY,
	CMD_AUTH_ERR,
	UNRECOGNIZE,
	PARA_FUNC_BUSY,
}cmd_result_t;

typedef int (delayfunc)(int done, void *arg, void *arg1);
typedef json_t* (*cmdfuncNoDelay)(json_t* req_root, json_t* req_para);
typedef json_t* (*cmdfuncNeedDelay)(json_t* req_root, json_t* req_para, void *arg);

typedef struct phone_para_s
{
	char *cmdtype;
	void *cmdfunc;
	int permit;
	int need_auth;
	int need_delay;
	delayfunc *cmdfuncdelay;
	pthread_mutex_t mutex_lock;
}phone_para_t, *phone_para_ptr;


typedef struct PluginPkt
{
	delayfunc *exec_func;
	trans_pkt_t *packet;
	void* arg;
	int isLargeBuf;
} PluginPkt_t;

typedef struct PluginResource
{
	int cpu;
	int ram;
	int flash;
	int socket;
	char port[32];
	int thread;	
}PluginResource_t, *PluginResource_ptr;

#define CMD_RES_ERR			-1
#define CMD_RES_SUCCESS	0
#define CMD_RES_BUSY 		-5

#define PARA_RESULT_SUCCESS 0
#define PARA_RESULT_PLUG_IN_NOT_EXIST -12
#define PARA_RESULT_PLUG_IN_BUSY -16
#define PARA_RESULT_PLUG_IN_TIMEOUT -100

#define PARA_CMD_TYE "CmdType"
#define PARA_RESULT "Result"
#define PARA_ID "ID"
#define PARA_SEQUENCE_ID "SequenceId"
#define PARA_STATUS "Status"
#define PARA_FAIL_REASON "FailReason"
#define PARA_PARAMETER "Parameter"
#define PARA_PARAMETER_RT "return_Parameter"

#define BANDWIDTH_COMMON		"BandWidth_Common"
#define BANDWIDTH_ENTRY			"BandWidth_Entry%d"
#define BANDWIDTH_DEVICE_SPEED		"BandWidth_DeviceSpeed"

#define MOBILE_ENTRY 			"mobile_Entry"
#define MOBILE_SLEEPENTRY 		"mobile_Sleep"
#define MOBILE_WIFIEXENTRY 		"mobile_WifiEx"
#define MOBILE_PLUGINRIGHTENTRY 	"mobile_PluginRight"
#define MOBILE_OSRIGHTENTRY 	"mobile_OSRight"
#define MOBILE_RIGHT1PATH 		"/usr/osgi"
#define MAX_ARRAY_NUM 100
extern int LedStatus;
extern int SleepStatus;
extern int NFCStatus;

#define VPN_ENTRY 			"VPN_Entry"
#define VPN_COMMON 		"VPN_Common"

#define ParameterInsertJSONChild		json_insert_child
#define ParameterNewJSONObject		json_new_object
#define ParameterNewJSONString		json_new_string
#define ParameterSearchValue			json_find_first_value //json_find_value
#define ParameterSearchFirstLabel		json_find_first_label
#define ParameterSearchFirstValue	json_find_first_value
#define ParameterSearchLabel			json_find_label
#define ParameterFreeJSON			json_free_value
#define ParameterCopyJSONLabel		json_copy_label
#define ParameterNewJSONArray		json_new_array
#define ParameterInsertJSONPair		json_insert_pair_into_object

void InterfaceRightinit();
int GetParaTableLen(void);
int ParameterInit(void);
json_t* ParameterHandle
(json_t *json_root, int request_from, PluginPkt_t *plugpkt, int *isLargeBuf);
json_t* ParameterBuildJSONReplyHead(json_t* req_json, int reply_result);
json_t* ParameterBuildJSONReplyPara(json_t* req_json, int reply_status);
int ParameterBuildJSONAddPara2Head(json_t* reply_root, json_t* reply_para);
void ParameterDumpJSON(const json_t * object);
json_t* ParameterBuildJSONReplyParaString
(json_t* req_para, char* reply_status);
int GetRight1(char *plgName,char *listName,char *typeName,char *nodeName);
int GetOSResource(char *plgName,PluginResource_ptr pResource);
int ReleaseRight1Resource(char *plgName,int pid);

#endif
