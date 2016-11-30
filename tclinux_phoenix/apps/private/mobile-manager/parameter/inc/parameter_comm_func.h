
/*
** $Id: //BBN_Linux/Branch/Branch_for_MT7505_IC_20131122/tclinux_phoenix/apps/private/TR69_64/cwmp/cwmpParameter/Includes/cwmpParameter_table.h#1 $
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

#ifndef _PARAMETER_FUNC_C_
#define _PARAMETER_FUNC_C_


#include <pthread.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <assert.h>
#include <sys/time.h>
#include <fcntl.h> //for message queue 

#include "parameter_table.h"
#include "debug.h"
#include "../../../lib/libtcapi.h"
#include "libcompileoption.h"
#if defined(TCSUPPORT_CT_JOYME)
#include "joyme.h"
#endif

//for replay paket print debug
#define PARAMETER_REPLAY_DBG 0

#define MAX_PVC_NUM	 	8
#define MAX_SMUX_NUM 	8
#define MAX_WAN_IF_INDEX  (MAX_PVC_NUM * MAX_SMUX_NUM)
#define MAX_ARG_NUM	3
#define MAX_NODE_NAME 16
#define WAN_IF_NODE   "WanInfo"

typedef signed char int8;
typedef unsigned char uint8;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed long int int32;
typedef unsigned long uint32;
typedef unsigned char boolean;
typedef unsigned long ip4a;
typedef unsigned char bool;

#define ADMINENTRY		"Account_Entry0"
#define USERENTRY 		"Account_Entry1"
#define ADMINUSER		"telecomadmin"
#define ACCOUNTUSER	"username"
#define ACCOUNTPASS	"web_passwd"

void fileRead(char *path, char *buf, int size);
int fileWrite(char *path, char *buf, int size);
char* itoa(register int i);
uint8 checkName(char *Name);
unsigned long resolveName(char *name);
int isInRouteTable(char *ip, char *dev);
int resolve(char *ipstr, unsigned long *addr, unsigned short *netmask);
void LedStatusChange(int onoff);
void setSleepAct(int type);
#endif

