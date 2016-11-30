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

#ifndef _DEV_LOCALRECOVERY_FUNC_C_
#define _DEV_LOCALRECOVERY_FUNC_C_

#include "parameter_comm_func.h"

json_t* DevLocalRecovery(json_t* req_root, json_t* req_para, void *arg);
int DevBoot2Defset(int done, void *arg,  void *arg1);

#endif
