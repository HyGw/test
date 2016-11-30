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

#ifndef _ATTACH_WAN_DNS_TUNNEL_FUNC_H_
#define _ATTACH_WAN_DNS_TUNNEL_FUNC_H_


#include "parameter_comm_func.h"

#define AWN_IP_NUM 8
#define AWN_DOMAIN_NUM 8

/*
Attach_Wan_DNS_Tunnel_func process
*/
json_t* Attach_Wan_DNS_Tunnel_func
(json_t* req_root, json_t* req_para);
/*
Detach_Wan_DNS_Tunnel_func process
*/
json_t* Detach_Wan_DNS_Tunnel_func
(json_t* req_root, json_t* req_para);

#endif

