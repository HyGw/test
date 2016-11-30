/*
** $Id: //BBN_Linux/DEV/main/tclinux_phoenix/apps/private/phone_app/parameter/inc/Get_PPP_DialStatus_func.h#1 $
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

#ifndef _GET_PPP_DIALSTATUS_FUNC_C_
#define _GET_PPP_DIALSTATUS_FUNC_C_

#include "parameter_comm_func.h"

#define PARA_CONNECTIONSTATUS	"ConnectionStatus"
#define PARA_WANSTATUS	   "WANStatus"
#define PARA_DIALREASON	   "DialReason"
#define PARA_CONNECTIONSTATUS1	"ConnectionStatus1"
#define PARA_WANSTATUS1	"WANStatus1"
#define PARA_DIALREASON1	"DialReason1"

typedef enum {
    Connecting =0,               
    Connected,
    PendingDisconnect,
    Disconnecting,
    Disconnected,
    Authenticating,       /*pppconnstatus  reserved*/
    Unconfigured               
} wanConnStatus;

json_t* GetPPP_DialStatus(json_t* req_root, json_t* req_para);
int getWANCnt1_IPv4InfoValue(char IPv4ConStatus[8],char IPv4Status[32],char IPv4ConError[128],uint32 wanNodeIndex,uint32 pvcNodeEntry);
int getWANCnt1_IPv6InfoValue(char IPv6ConStatus[8],char IPv6Status[32],char IPv6ConError[128],uint32 wanNodeIndex,uint32 pvcNodeEntry);
int getWanPPPCntGetStatusInfo(int type, char WanStatus[32],uint32 wanNodeIndex,uint32 pvcNodeEntry);
int getISPIndex(int *defRouterIfv4, int *defRouterIfv6);
int TcGetWANConnStatus(int type, wanConnStatus *connstatus, int pvcindex);

#endif

