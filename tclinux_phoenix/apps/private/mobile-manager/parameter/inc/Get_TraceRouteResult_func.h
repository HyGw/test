/*
** $Id: //BBN_Linux/DEV/main/tclinux_phoenix/apps/private/phone_app/parameter/inc/Get_TraceRouteResult_func.h#1 $
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

#ifndef _GET_TRACEROUTERESULT_FUNC_C_
#define _GET_TRACEROUTERESULT_FUNC_C_

#include "parameter_comm_func.h"

#define PARA_TRACESTATUS	"TraceStatus"
#define PARA_TRACERESULT	"TraceResult"
#define TRACEROUTEDIAGNOSTICFILE "/tmp/cwmp/traceroutediagnostic_stat"
#define NO_QMARKS 0
#define QMARKS 1
#define MAX_BUF_SIZE	2048
#define MAXGET_PROFILE_SIZE 128

json_t* Get_TraceRouteResult(json_t* req_root, json_t* req_para);
int get_profile_str(char *keyname,char *str_return, int size, int type, char *path);

#endif
