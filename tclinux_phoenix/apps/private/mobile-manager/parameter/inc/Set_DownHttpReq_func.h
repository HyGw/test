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

#ifndef _SET_DOWNHTTPREQ_FUNC_C_
#define _SET_DOWNHTTPREQ_FUNC_C_

#include "parameter_comm_func.h"

#define	WGET_WAITTIME	5
#define THREAD_DEF_COUNT	8
#define THREAD_MAX_COUNT	20
#define	PROC_DEF_COUNT		1
#define PROC_MAX_COUNT		10

extern int httpcon_time;

json_t* SetDownHttpReq(json_t* req_root, json_t* req_para);
#endif
