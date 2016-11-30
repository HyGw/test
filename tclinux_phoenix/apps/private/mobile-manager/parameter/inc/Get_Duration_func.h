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

#ifndef _Get_DURATION_FUNC_C_
#define _Get_DURATION_FUNC_C_

#include "parameter_comm_func.h"

#define PARA_SYSDuration "SYSDuration"
#define PARA_PPPoEDuration "PPPoEDuration"
#define PARA_PONDuration "PONDuration"

json_t* Get_Duration_func(json_t* req_root, json_t* req_para);
void get_sysup_duration(char* temp);
void get_ppp_duration(char* temp);
void get_PON_duration(char* temp);

#endif

