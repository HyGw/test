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

#ifndef _Set_Wanl2tp_C_
#define _Set_Wanl2tp_C_

#include "parameter_comm_func.h"

#define PARA_TUNNELNAME "tunnelName"



json_t* Create_WANL2TP_Tunnel(json_t* req_root, json_t* req_para);
json_t* Remove_WANL2TP_Tunnel(json_t * req_root,json_t * req_para);
json_t* Attach_WANL2TP_Tunnel(json_t* req_root, json_t* req_para);
json_t* Get_WANL2TP_Tunnel_Status(json_t* req_root, json_t* req_para);
json_t* Detach_WANL2TP_Tunnel(json_t* req_root, json_t* req_para);





#endif

