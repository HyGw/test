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
#ifndef _SIM_H
#define _SIM_H
#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include "cfg_manager.h"
#include "utility.h"
#include "../lib/simCardUtil.h"
#include "../lib/simcardlib.h"
#include <fcntl.h>
#include "global.h"

#define SIMCARD_LOCK_FILE "/tmp/simCardlockfd"

#define DEVICE_ACCOUNT_NODE_NAME "deviceAccount"
#define LOGIC_ID_NODE_NAME "LogicID"
#define CWMP_NODE_NAME "Cwmp"


#define SIM_CARD_NODE_NAME "SimCard"
#define ENTRY_NODE_NAME "Entry"

#define SIM_CARD_EXIST_CHECK_TIME	(1)

void simThread(void* unused);

#endif

