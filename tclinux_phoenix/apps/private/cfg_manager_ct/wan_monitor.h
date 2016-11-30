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
#ifndef _WAN_MONITOR_H
#define _WAN_MONITOR_H

#define WAN_IF_INFO_NUM 64

struct if_info{
	int	status_br;
	int status;
	int status6;
	int dhcp_renew_flag;  /*need to send renew signal for dhcp mode when adsl is up*/
#if defined(TCSUPPORT_CT_PPPINFORM)
	char username[65];
#endif
};

#if defined(TCSUPPORT_CT_JOYME)
typedef struct
{
	char wanIpAddr[16];
	char wanSubnetMask[16];
	char ipv6WanAddr[64];
	int wanPrefixLen;
}WanIpEvent;
#endif

enum if_state{
	IF_STATE_DOWN = 0,
	IF_STATE_UP,
	IF_STATE_GETIP,
	IF_STATE_GETGATEWAY,
	IF_STATE_GETDNS,             /*DNS server address*/
	IF_STATE_DOWN6,
	IF_STATE_UP6,
	IF_STATE_GETIP6,
	IF_STATE_GETGATEWAY6,
	IF_STATE_GETDNS6,             /*DNS server ipv6 address*/
	IF_STATE_GETPD,               /*Get prefix delegation for IPv6 DHCPv6-PD mode*/
	IF_STATE_MODIFY,
	BRIDGE_IF_STATE_DOWN,
	BRIDGE_IF_STATE_UP,
};

enum adsl_state{
	ADSL_STATE_DOWN=0,
	ADSL_STATE_UP,
};

enum dhcp_signal_type{
	DHCP_SIGNAL_RELEASE=0,
	DHCP_SIGNAL_RENEW,
};

#if defined(TCSUPPORT_CT)
#define NTPFLAGWITHNOEXECUTE 0
#define NTPFLAGWITHEXECUTE 1
#define NTPFLAGWITHSYNC 2
#endif
void wan_monitor_init();

#ifdef CWMP
void sendMsgtoCwmp(int cwmpMsgType
#if defined(TCSUPPORT_CT_MIDWARE)
, int if_state, int if_index 
#endif
);
#if defined(TCSUPPORT_CT_MIDWARE)
void restart_ctadmin(mxml_node_t *top, int if_index);
#endif

#endif

#if defined(TCSUPPORT_CT_JOYME)
void wanIpChangeInform(mxml_node_t *top, int if_index, int if_state);
#endif
int interface_state_change(mxml_node_t *top, int if_state, char *if_name);
int adsl_state_change(int adsl_state);

void restart_dnsmasq();
#if defined(TCSUPPORT_CT)
void initNTPTimerStruct();
#endif
#if defined(TCSUPPORT_CT_PON_CY_JX)
unsigned long secclock();
char * getUpDateTime(void);
#endif
#endif
