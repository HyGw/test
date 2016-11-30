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
#ifndef _BANDWIDTH_H
#define _BANDWIDTH_H

int devMaxBandWidth_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int devMaxBandWidth_boot(mxml_node_t *top);
int devMaxBandwidth_init(void);


int devBandWidth_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int devBandWidth_boot(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int devBandwidth_init(void);

#define DEV_MAC_RULE_NUM	30
#define	FEMAC_BANDWIDTH_PATH 	"/proc/tc3162/devBandwidth"
#define HWNAT_BANDWIDTH_PATH 	"/proc/tc3162/hwnat_bandwidth"

#endif


