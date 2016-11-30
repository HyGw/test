/*
** $Id: //BBN_Linux/DEV/main/tclinux_phoenix/apps/private/phone_app/parameter/inc/Get_AssignedWanConnectInfo_func.h#1 $
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

#ifndef _GET_ASSIGNEDWANCONNECTINFO_FUNC_C_
#define _GET_ASSIGNEDWANCONNECTINFO_FUNC_C_

#include "parameter_comm_func.h"

#define PARA_INDEX	"INDEX"
#define PARA_IFNAME	"IFNAME"
#define PARA_SERVICELIST	"SERVICELIST"
#define PARA_CONNECTIONTYPE	"CONNECTIONTYPE"
#define PARA_VLANID	"VLANID"
#define PARA_8021P	"8021P"
#define PARA_PROTOCOL	"PROTOCOL"
#define PARA_CONNECTIONSTATUS	"CONNECTIONSTATUS"
#define PARA_IPADDRESS	"IPADDRESS"
#define PARA_SUBNETMASK	"SUBNETMASK"
#define PARA_GATEWAY	"GATEWAY"
#define PARA_DNS1	"DNS1"
#define PARA_DNS2	"DNS2"
#define PARA_IPV6_CONNECTIONSTATUS	"IPV6_CONNECTIONSTATUS"
#define PARA_IPV6_IPADDRESS	"IPV6_IPADDRESS"
#define PARA_IPV6_PREFIXLENGTH	"IPV6_PREFIXLENGTH"
#define PARA_IPV6_GATEWAY       "IPV6_GATEWAY"
#define PARA_IPV6_DNS1	"IPV6_DNS1"
#define PARA_IPV6_DNS2	"IPV6_DNS2"
#define PARA_IPV6_PRIFIX	"IPV6_PRIFIX"

json_t* Get_AssignedWanConnectInfo(json_t* req_root, json_t* req_para);

#endif

