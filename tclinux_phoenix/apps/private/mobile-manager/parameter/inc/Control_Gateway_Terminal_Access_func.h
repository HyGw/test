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

#ifndef _CONTROL_GATEWAY_TERMINAL_ACCESS_FUNC_C_
#define _CONTROL_GATEWAY_TERMINAL_ACCESS_FUNC_C_

#include "parameter_comm_func.h"

int findMacaddr(char *adrr);

void addMacaddr(char *macadrr);

void DELMacaddr(int num);

json_t* Control_Gateway_Terminal_Access_func(json_t* req_root, json_t* req_para);

#if defined(TCSUPPORT_CT_JOYME)
#define MAX_SAR_RULE 64

json_t* Get_Attach_Device_Storage_Access_func
(json_t* req_root, json_t* req_para);
#endif

#endif
