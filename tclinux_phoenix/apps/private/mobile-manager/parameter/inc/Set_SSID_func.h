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

#ifndef _SET_SSID_FUNC_C_
#define _SET_SSID_FUNC_C_

#include "parameter_comm_func.h"

json_t* Set_SSID_func(json_t* req_root, json_t* req_para, int* ssidIndex);
int delaysetwifiinfo(int done, void *arg, void *arg1);

#endif


