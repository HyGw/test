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

#ifndef _PPP_DIALCONTROL_FUNC_C_
#define _PPP_DIALCONTROL_FUNC_C_

#include "parameter_comm_func.h"
#include "threadpool.h"

#define PARA_CONSTATUS	"ConnectionStatus"
#define PARA_WANSTA	"WANStatus"
#define PARA_PPPDIALREASON	"DialReason"
#define	PPPDIALMAXTIME		20
#define	RESTOREDELAYTIME	60

json_t* PPP_DialControl(json_t* req_root, json_t* req_para);
void * pppdial_process (worker_data_t *argv);

#endif
