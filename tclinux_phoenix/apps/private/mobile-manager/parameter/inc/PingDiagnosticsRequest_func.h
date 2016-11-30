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

#ifndef _PINGDIAGNOSTICSREQUEST_FUNC_C_
#define _PINGDIAGNOSTICSREQUEST_FUNC_C_

#include "parameter_comm_func.h"

typedef struct ping_s
{
	ip4a target;	/*Starting target Ip address*/
	uint32 interval;/*Inter-ping interval(s)*/
	uint32 sent;	/*Total number of sent*/
	uint32 responses;/*Total number of responses*/
	uint16 len;		/*Length of data portion of ping*/
	uint32 maxrtt;	/*Maximun rtt*/
	uint32 minrtt;	/*Minimun rtt*/
	uint32 avrrtt;	/*Average rtt*/	
	uint32 dscp;
}ping_t;

uint8  thePingFlag;// 2 TERMINATED;1 PING_INPROGRESS;0 NOT_ACTIVE
int resolveHostError;
int DomainNameError;

void *cwmpPingTask(void);
int doCwmpPing(void);
json_t* PingDiagnosticsRequest(json_t* req_root, json_t* req_para);

#endif

