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

#ifndef _CWMP_RPC_FUNC_H_
#define _CWMP_RPC_FUNC_H_


#include "parameter_comm_func.h"

#define PARA_NAME	"Name"
#define PARA_VALUE	"Value"
#define PARA_SETCOMMIT_NAME "SETPARAMETERVALUES_COMMIT"

enum OP_CMD
{
	RPC_CMD_REQ = 1,
	RPC_CMD_REPLY,
	RPC_CMD_REPLY_END,
	RPC_CMD_REPLY_ERROR
};

typedef struct _tcphone_rpc_msg_
{
#define RPC_MSG_MAGIC 0x11221122
#define RPC_MAX_NODE_BUFLEN 512
#define ZERO_RPC_MSG(x) { bzero(&x, sizeof(x)); x.magic = RPC_MSG_MAGIC; }
	uint32_t magic;
	uint8_t opcmd;
	uint8_t reserve[3];
	uint32_t valuelen;
	char rpcmethod[64];
	char nodename[RPC_MAX_NODE_BUFLEN];
} tcphone_rpc_msg;

#define FREE_X(x) { if (x) free(x); x = NULL; }
#define CLOSE_X(x) { if (x) close(x); x = NULL; }

json_t* RPC_AddObject_func
(json_t* req_root, json_t* req_para);
json_t* RPC_DeleteObject_func
(json_t* req_root, json_t* req_para);
json_t* RPC_GetParameterValues_func
(json_t* req_root, json_t* req_para);
json_t* RPC_SetParameterValues_func
(json_t* req_root, json_t* req_para, void *arg);
int HandleCommit(int done, void *arg, void *arg1);

#endif

