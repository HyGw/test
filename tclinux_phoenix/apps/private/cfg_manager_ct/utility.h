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
#ifndef _UTILITY_H
#define _UTILITY_H

#include "cfg_manager.h"


#define PATH_MAX_DNSMASQ 128

#define LOAD_DEFAULT_CHAIN "/usr/script/filter_forward_start.sh"
#define FLUSH_DEFAULT_CHAIN "/usr/script/filter_forward_stop.sh"

/*krammer add 2008 11 12*/
#define MAXGET_PROFILE_SIZE 128
#if defined(TCSUPPORT_CT_PORTAL_MANAGEMENT)
typedef struct {
    unsigned char  tmac[6];
    unsigned char  smac[6];
    unsigned short type;
} __attribute__((packed)) ether_hdr_t ;

typedef struct {
    unsigned short hwtype;
    unsigned short prot;
    unsigned char  hwlen;
    unsigned char  protlen;
    unsigned short opcode;
    unsigned char  smac[6];
    unsigned int   sip;
    unsigned char  tmac[6];
    unsigned int   tip;
} __attribute__((packed)) arp_hdr_t ;

typedef struct {
    ether_hdr_t ethhdr;
    arp_hdr_t  arphdr;
} __attribute__((packed)) arp_packet_t ;

#define HW_TYPE_ETHERNET    0x0001
#define ARP_OPCODE_REQUEST 0x0001
#define ARP_OPCODE_REPLY   0x0002

void
check_lanip_exist(void);

int 
conntrack_delete(u_int8_t l3protonum, u_int8_t protonum, unsigned int src_ip, u_int16_t l4dst);

int 
inet_open_rawsock(char *iface, int protocol);

int 
forge_arp_request(arp_packet_t *pkt, unsigned char *smac, unsigned int sip, unsigned int tip);

int 
inet_get_ifaddr(char *iface, unsigned char* mac, unsigned int *ip);

static int
get_arp_reply(int sock, arp_packet_t *pkt, int timeout);

int
getArpReply(char *iface, unsigned int tip, int timeout);
#endif

void 
get_lan_status(int lan_status[]);
#ifdef TCSUPPORT_CPU_MT7510
void get3262LanStatus(unsigned char* buf);
#endif
#if defined(TCSUPPORT_CT_PON)
int getPonLanStatus(char* buf);
int getPonLanStatus2(int portStatus[]);
int find_pid_by_name( char *ProcName, int *foundpid, int *foundnum);
#endif
void flush_arp();

void 
splitName(char *name, char buf[][MAX_NODE_NAME], const char* delim);

void
mergeNodeName(mxml_node_t *top, char name[][MAX_NODE_NAME]);

int
getAttrValue(mxml_node_t *top, char name[][MAX_NODE_NAME], char* attr, char* buf);

int
setAttrValue(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr, char *value);

void
fileRead(char *path, char *buf, int size);

void
kill_process(char *pid_path);

int
get_profile_str(char *keyname,char *str_return, int size, int type, char *path);

int
check_nat_enable(mxml_node_t *top, int state);
int 
tcapi_commit_internal(mxml_node_t *top, char *node_name);

int 
tcapi_set_internal(mxml_node_t *top, char *node_name, char *attribute, char *value);

#if 0
int
check_filter_state(mxml_node_t *top, int filter_state);
#endif
char
check_and_set_filter(unsigned int new_filter_state);
/*rodney_20090420 add*/
int
check_ip_exist(char *interface, int pvc, int isp, int check_flag);

/**********20080826 racing add************/
int
acl_get_active_interface(mxml_node_t *top, char interfaces[][8], const char* intf_type);

char*
string_tolower(char *string);

int
check_ip_format(char *ip_arg);

int
acl_check_ip_format(char *ip_arg);

int
acl_check_ip_range(char *ip_begin_arg, char *ip_end_arg);
/**********20080826 racing add************/

/**********20081006 racing add************/
int
check_mask_format(char *mask_arg, char *mask_decimal);

int
check_mac_format(const char *mac_arg);

int
get_interface_by_PVC(mxml_node_t *top, const char* intf_tpye, char *interface);

int
ipfilter_check_active(mxml_node_t *top);
#if defined(TCSUPPORT_CT_E8GUI)
int
ipfilter_check_switch(mxml_node_t *top);
int
parsedStr(char *str_input, char *str_ret);
#endif
/**********20081006 racing add************/

/*20081022 krammer add*/
#if 0
int
check_l7filter_state(mxml_node_t *top, int l7filter_state);
#endif
void
check_and_set_l7filter(unsigned int new_filter_state);
/*20081022 krammer add*/
#if 0
/*20081228 pork add*/
void trim(char *oriStr,char *retStr);
/*20081228 pork add*/
#endif
/*20081228 pork add*/
void quotHandle(char *oriStr);
/*20080123 pork add*/
//void
//setNewFilterState(mxml_node_t *top,int (*conditionFunction)(mxml_node_t *top),char vector,char *newFilterState);
int
get_entry_number(char *buffer, char *keyword, int *number);
/*for check_crc32. shnwind add.*/

unsigned long calculate_crc32(int imagefd, long len);
int doValPut(char *path, char *value);

#define CHECKSUM_TEMP_BUF_SIZE 4096

int is_adsl_link_up();
#if defined(TCSUPPORT_CT_PON)
int is_pon_link_up();
#endif

int get_wanindex_by_name(char *wan_if_name);
int get_waninfo_by_index(int if_index, char *attribute, char *reval);
int get_waninfo_by_name(char *wan_if_name, char *attribute, char *reval);
int get_all_wan_index(int indexbuf[], int buf_size);
mxml_node_t *get_node_top();
mxml_node_t *search_node(char nodeName[][MAX_NODE_NAME]);
int copy_node(char dst_node[][MAX_NODE_NAME], char src_node[][MAX_NODE_NAME]);

#if defined(TCSUPPORT_CT_BACKUPROMFILEENCHANCEMENT)
unsigned long compute_crc32buf(char *buf, size_t len);
#endif
#ifdef TCSUPPORT_IPV6
void setIpv6Radio(mxml_node_t *top);
#endif
int get_profile_str_new(char *keyname,char *str_return, int size,char *path);

int str2Hex(char *str);

int signalCwmpInfo(int mq, int reserved, int reserved2);
int create_simThread(pthread_t *pth, pthread_attr_t *pattr);
mxml_node_t* ct_simCardInit(mxml_node_t *top);
#endif

#if defined(TCSUPPORT_CT_PON)
int setPonState2(int state);
int getPonState2();
#endif


