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

#ifndef _SET_WIRELESS_SCHEDULE_FUNC_C_
#define _SET_WIRELESS_SCHEDULE_FUNC_C_

#include "parameter_comm_func.h"

#define SCHEDULE_ON  2
#define SCHEDULE_OFF  3
#define MOBILE_ENTRY 	"mobile_Entry"

uint8 getScheduleParse(uint16 weekDay, uint32 min, uint32 sec);
json_t* Set_wireless_schedule(json_t* req_root, json_t* req_para);

#endif
