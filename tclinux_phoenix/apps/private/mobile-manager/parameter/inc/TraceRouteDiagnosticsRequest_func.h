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

#ifndef _GET_TRACEROUTEDIAGNOSTICSREQUEST_FUNC_C_
#define _GET_TRACEROUTEDIAGNOSTICSREQUEST_FUNC_C_

#include "parameter_comm_func.h"

#define TMP_PHONEAPP_TRACE_PATH "/tmp/cwmp/trace_result"

typedef struct traceroute_s{
	ip4a   	target;
	uint32  NumberofTries;
	uint32 Timeout;
	uint32 DataBlockSize;
	uint32 MaxHopCount;
	uint32 ResponseTime;
	uint16 RouteHopsNumberOfEntries;
} traceroute_t;

typedef struct tracerouteHophost_s{
	char HopHost[257];
	char HopHostAddress[16];
	uint32 HopErrorCode;
	char HopRTTime[33];
	
} tracerouteHophost_t;

uint8  theTraceRouteFlag;// 1 TRACEROUTE_INPROGRESS;0 NOT_ACTIVE

json_t* TraceRouteDiagnosticsRequest(json_t* req_root, json_t* req_para);

#endif

