/******************************************************************************/
/*
 * Copyright (C) 1994-2008 TrendChip Technologies, Corp.
 * All Rights Reserved.
 *
 * TrendChip Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of TrendChip Technologies, Corp. and
 * shall not be reproduced, copied, disclosed, or used in whole or
 * in part for any reason without the prior express written permission of
 * TrendChip Technologies, Corp.
 */
/******************************************************************************/
#ifndef _TCAPI_PROC_H
#define _TCAPI_PROC_H

#if defined(TCSUPPORT_CT_PON_SC)
#include "../opensource_lib/inc/libmtdpartition.h"
#endif

#if defined(TCSUPPORT_CT_L2TP_VPN)
#define MAX_SHOW_BUF	(1024*32) /* for VPN node, about 23k. */
#else
#define MAX_SHOW_BUF	8192 
#endif
#define MAX_ATTR_BUF	640

#if defined(TCSUPPORT_CT_DEFAULTPARA_STORE)
#define DEFVALUEFLAG_NODENAME	"Account"
#define DEFVALUEFLAG_ENTRYNAME	"CTDefParaEntry"
#define DEFVALUEFLAG_ATTRNAME	"setDefValueFlag"
#define DEFVALUEFLAG_ACTIVE	"1"

#define WLAN_NODENAME			"WLan"
#define WLAN_ENTRYNAME			"Entry0"
#define WLAN_DEFCFG_ATTRNAME		"DefaultCfg"
#define WLAN_DEFCFG_DISABLE		"0"
#endif

int 
isVaildNode(char name[][MAX_NODE_NAME]);

void 
tcapi_set_req(mxml_node_t* tree,tcapi_msg_t *msg);

void
tcapi_unset_req(mxml_node_t* tree, tcapi_msg_t *msg);

void 
tcapi_get_req(mxml_node_t* tree, tcapi_msg_t *msg);

void 
tcapi_show_req(mxml_node_t* tree,tcapi_msg_t *msg);

void 
tcapi_commit_req(mxml_node_t* tree, tcapi_msg_t *msg);

void 
tcapi_save_req(mxml_node_t* tree, tcapi_msg_t *msg);

void 
tcapi_read_req(mxml_node_t* tree, tcapi_msg_t *msg);

extern void tcapi_readAll_req(tcapi_msg_t *msg);
#endif
