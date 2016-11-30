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
#ifndef _CT_JOYME_API_H_
#define _CT_JOYME_API_H_

#include "libtcapi.h"
#include "json.h"
#include "json_helper.h"

#define EVENT_INFORM_PATH		"/tmp/eventInform"

#define PARA_PARAMETER "Parameter"

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


#define DEV_MAC_RULE_NUM	30
#define TRTCM_MATCH_MAC_RULE_NUM  		10
#define BANDWIDTH_COMMON_NODE		"BandWidth_Common"

#define CT_IP_ADDR_STR_MAX_LEN				16
#define CT_IPV6_ADDR_STR_MAX_LEN			64
/*define in libtcapi.h*/
//#define CT_USB_DEV_NAME_MAX_LEN				128
//#define CT_USB_DEV_MOUNT_PATH_MAX_LEN		256
#define CT_WLAN_SSID_MAX_LEN				32
#define CT_WLAN_PASSWORD_MAX_LEN			32
#define CT_LAN_DEV_NAME_MAX_LEN				64
#define CT_LAN_DEV_BRAND_NAME_MAX_LEN		64
#define CT_LAN_DEV_OS_NAME_MAX_LEN			64

#define CT_BUNLE_SYMBOLIC_NAME_MAX_LEN		64
#define MAX_MAC_NUM							40
#define MAX_SAR_RULE 64
#define MAX_LDEV_NUM						64
#define ATTACH_LANINFO_PATH						"/usr/osgi/config/laninfo.conf"

#define VPN_INSTANCE_NUM					16
#define VPN_ATTACH_NUM						16

typedef enum
{
	CT_EVENT_USB_DEV_ACTION,
	CT_EVENT_WAN_IP_CHANGE,
	CT_EVENT_WLAN_DEV_ONLINE
}CtEvnetType;
#if 0
typedef enum
{
	CT_USB_DEV_ACTION_INSERT,
	CT_USB_DEV_ACTION_PULL
}CtUsbDevActionType;

typedef enum
{
	CT_USB_DEV_SERIAL = 0x1,
	CT_USB_DEV_CDC_ACM = 0x2,	
	CT_USB_DEV_HID = 0x4,
	CT_USB_DEV_STORAGE = 0x8
}CtUsbDevType;
#endif
typedef enum
{
	CT_LAN_DEV_CONN_UNKNOWN,
	CT_LAN_DEV_CONN_WIRED,	
	CT_LAN_DEV_CONN_WLAN,
}CtLanDevConnType;

typedef enum
{
	CT_LAN_DEV_UNKNOWN,
	CT_LAN_DEV_PHONE,	
	CT_LAN_DEV_PAD,
	CT_LAN_DEV_PC,
	CT_LAN_DEV_STB,	
	CT_LAN_DEV_OTHER
}CtLanDevType;

typedef enum
{
	CT_WL_AUTH_OPEN,
	CT_WL_AUTH_SHARED,
	CT_WL_AUTH_WPA_PSK,
	CT_WL_AUTH_WPA2_PSK,
	CT_WL_AUTH_WPA2_PSK_MIX
}CtWlanAuthMode;

typedef enum
{
	CT_PARITY_NONE,
	CT_PARITY_EVEN,
	CT_PARITY_ODD,
	CT_PARITY_MARK,
	CT_PARITY_SPACE,
}CtParityType;

typedef struct
{
	CtUsbDevActionType actionType;
	CtUsbDevType devType;
	int devId;
	char devName[CT_USB_DEV_NAME_MAX_LEN];
	char mountPath[CT_USB_DEV_MOUNT_PATH_MAX_LEN];
}CtUsbDevEvent;

typedef struct
{
	char ipAddr[CT_IP_ADDR_STR_MAX_LEN];
	char subnetMask[CT_IP_ADDR_STR_MAX_LEN];
	char ipv6Addr[CT_IPV6_ADDR_STR_MAX_LEN];
	int prefixLen;
}CtWanIpChangeEvent;

typedef struct
{
	unsigned char macAddr[6];
}CtWlanDevOnlineEvnet;

typedef struct
{
	char wanIpAddr[CT_IP_ADDR_STR_MAX_LEN];
	char wanSubnetMask[CT_IP_ADDR_STR_MAX_LEN];
	char ipv6WanAddr[CT_IPV6_ADDR_STR_MAX_LEN];
	int wanPrefixLen;
	char lanIpAddr[CT_IP_ADDR_STR_MAX_LEN];
	char lanSubnetMask[CT_IP_ADDR_STR_MAX_LEN];
	char ipv6LanAddr[CT_IPV6_ADDR_STR_MAX_LEN];
	int lanPrefixLen;
}CtSysIpAddrCfg;


typedef struct
{
	unsigned char macAddr[6];
}CtMacAddr;

typedef struct
{
	char ipAddr[CT_IP_ADDR_STR_MAX_LEN];
}CtIpAddr;

typedef struct
{
	char devName[CT_LAN_DEV_NAME_MAX_LEN];
	CtLanDevType devType;
	unsigned char macAddr[6];	
	char ipAddr[CT_IP_ADDR_STR_MAX_LEN];
	CtLanDevConnType connType;
	int port;		
	/*0:unknown;1:Lan1;2:Lan2;3:Lan3;4:Lan4;5:SSID-1;6:SSID-2;7:SSID-3;8:SSID-4;9:SSID-5;10:SSID-6;11:SSID-7;12:SSID-8*/
	char brandName[CT_LAN_DEV_BRAND_NAME_MAX_LEN];
	char osName[CT_LAN_DEV_OS_NAME_MAX_LEN];
	unsigned int onlineTime;
}CtLanDevInfo;

typedef struct
{
	int enable;
	CtWlanAuthMode authMode;
	char ssid[CT_WLAN_SSID_MAX_LEN];
	char password[CT_WLAN_PASSWORD_MAX_LEN];
}CtWlanSecurityCfg;

typedef struct
{
	int bandrate;
	CtParityType parity;
	int dataBits;
	int stopBits;
	int hwFlowCtrl;
	int swFlowCtrl;
}CtUsbSerialCfg;

/*DPI struct*/
typedef struct CtSgwTupleInfo_
{
	unsigned char direct;
	unsigned char proto;
	unsigned int sipv4;
	unsigned int dipv4;
	unsigned int sipv6[4];
	unsigned int dipv6[4];
	unsigned short sport;
	unsigned short dport;
	unsigned long in_iif;	/*index of in interface*/
	unsigned long out_iif;  /*index of out interface*/
}CtSgwTupleInfo;

struct stat_t {
    unsigned int  user;
    unsigned int  nice;
    unsigned int  system;
    unsigned int  idle;
    unsigned int  iowait;
    unsigned int  irq;
    unsigned int  softirq;
    unsigned int  total;
};

typedef int (*ctSgw_appCtxCreate)(void** p_app_ctx, unsigned char flag);
typedef void (*ctSgw_appCtxDestroy)(void** p_app_ctx);
typedef int (*ctSgw_appProcAppId)(unsigned char *layer2date, CtSgwTupleInfo *tupleinfo, void *p_app_ctx, unsigned  int *layer7_id);

typedef struct ctSgw_dpiFuncs
{
	ctSgw_appCtxCreate ctSgw_appCtxCreateHook;
	ctSgw_appCtxDestroy ctSgw_appCtsDestoryHook;
	ctSgw_appProcAppId ctSgw_appProcAppIdHook;
}ctSgw_dpiFuncs;



#define MAX_SSID_NUM 				(4)
#define MAX_PVC_NUM	 				(8)
#define MAX_SMUX_NUM 				(8)

#define WAN_PVC_ENTRY_NODE	 		"Wan_PVC%d_Entry%d"
#define WAN_SERVICELIST_ATTR 		"ServiceList"
#define WAN_SERVICELIST_INTERNET	"INTERNET"
#define MOBILE_ENTRY "mobile_Entry"

#define VPN_ENTRY 			"VPN_Entry"
#define VPN_COMMON 		"VPN_Common"

int joyme_init(int *sock, int pid);
int joyme_cleanup(void);
int joyme_handleEvent(int type, CtEvnetType *eventType, void **eventInfo);
int joyme_wlanSetState(int ssidIndex, int enable);
int joyme_wlanGetSecurityCfg(CtWlanSecurityCfg *securityCfg);
int joyme_lanGetDevDevInfo(CtLanDevInfo **devInfoList, int *devNum);
int joyme_lanUpdateDevInfo(const CtLanDevInfo *devInfo);
int joyme_lanSetDevAccessPermission(const CtMacAddr *macAddr, int interetAccess, int storageAccess);
int joyme_lanGetDevInternetAccessBlacklist(CtMacAddr **macAddrList, int *macAddrNum);
int joyme_lanGetDevStorageAccessBlacklist(CtMacAddr * * macAddrList,int * macAddrNum);
int joyme_lanGetDevBandwidth(const CtMacAddr *macAddr, int* usBandwith, int* dsBandwith);
int joyme_lanSetDevMaxBandwidth(const CtMacAddr *macAddr, int usBandwith, int dsBandwith);
int joyme_lanGetDevMaxBandwidth(const CtMacAddr *macAddr, int* usBandwith, int* dsBandwith);
int joyme_lanGetPortStatus(int *portStatus);
int joyme_lanSetDevStatsStatus(int enable);
int joyme_wanGetIfStats(int *usStats, int *dsStats);
int joyme_wanGetPppoeAccount(char *pppoeAccount, int accountLen);
int joyme_wanCreateL2tpTunnel(const char *userId,const char *serverIpAddr,const char *username,const char *password,char *tunnelName,int nameLen);
int joyme_wanRemoveL2tpTunnel(const char *userId,const char *tunnelName);
int joyme_wanAttachL2tpTunnel(const char *tunnelName,const CtIpAddr *ipAddrList, int ipAddrNum);
int joyme_wanDetachL2tpTunnel(const char *tunnelName,const CtIpAddr *ipAddrList, int ipAddrNum);
int joyme_wanGetL2tpTunnelStatus(const char *tunnelName, int *tunnelStatus);
int joyme_sysGetCpuUsage(int *percent);
int joyme_sysGetMemUsage(int *percent);
int joyme_sysGetIpAddr(CtSysIpAddrCfg *ipAddrCfg);
int joyme_sysGetLoid(char *loid, int loidLen);
int joyme_sysGetMac(unsigned char *mac);
int joyme_sysPushWeb(int enable, int width, int height, const char *place, const char *url, int time);
int joyme_usbRegister(int type,int pid, char* name,CtUsbDevType devType);
int joyme_usbUnregister(int type,int pid, char* name);
int joyme_usbLock(int type,int pid, char* name,int devId);
int joyme_usbUnlock(int type,int pid, char* name,int devId);
int joyme_usbOpen(int type,int pid, char* name,int devId, int *handle);
int joyme_usbClose(int handle);
int joyme_usbDataAvailable(int type,int pid, char* name,int handle);
int joyme_usbRead(int type,int pid, char* name,int handle, void *buf, size_t* count, int timeout);
int joyme_usbWrite(int type,int pid, char* name,int handle, void *buf, size_t* count);
int joyme_usbSetSerial(int type,int pid, char* name,int handle, const CtUsbSerialCfg *serialCfg);
int joyme_eventInform(int type,int pid, char* name, const char *event);
int joyme_dpiCtxCreate(void ** p_dpi_ctx, unsigned char flag);
int joyme_dpiCtxDestory(void ** p_dpi_ctx);
int joyme_dpiProcAppld(unsigned char *layer2data, CtSgwTupleInfo *tupleinfo, void * dpi_ctx, unsigned int *layer7_id);
int joyme_appRegisterFunc(ctSgw_dpiFuncs *funcs);
void joyme_appUnRegisterFunc(void);

int GetCpuUsageTOT_IDLE(unsigned int *total, unsigned int *idle);
void fileRead(char *path, char *buf, int size);
int GetMacAddr(unsigned char *macstr, unsigned char *macnum);
int doFileGetValue(char *path, char **line_buf, int len);
int doFilePutValue(char *path, char *value);

void joyme_upg_test(void);
int joyme_sysSetSSN(const char *ssn);
int joyme_sysGetSSN(char *ssn, int ssnLen);

int getOsgiTestFlag();

#endif
