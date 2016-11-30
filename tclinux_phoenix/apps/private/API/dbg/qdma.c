/***************************************************************
Copyright Statement:

This software/firmware and related documentation (EcoNet Software) 
are protected under relevant copyright laws. The information contained herein 
is confidential and proprietary to EcoNet (HK) Limited (EcoNet) and/or 
its licensors. Without the prior written permission of EcoNet and/or its licensors, 
any reproduction, modification, use or disclosure of EcoNet Software, and 
information contained herein, in whole or in part, shall be strictly prohibited.

EcoNet (HK) Limited  EcoNet. ALL RIGHTS RESERVED.

BY OPENING OR USING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY 
ACKNOWLEDGES AND AGREES THAT THE SOFTWARE/FIRMWARE AND ITS 
DOCUMENTATIONS (ECONET SOFTWARE) RECEIVED FROM ECONET 
AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON AN AS IS 
BASIS ONLY. ECONET EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, 
WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, 
OR NON-INFRINGEMENT. NOR DOES ECONET PROVIDE ANY WARRANTY 
WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTIES WHICH 
MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE ECONET SOFTWARE. 
RECEIVER AGREES TO LOOK ONLY TO SUCH THIRD PARTIES FOR ANY AND ALL 
WARRANTY CLAIMS RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES 
THAT IT IS RECEIVERS SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD 
PARTY ALL PROPER LICENSES CONTAINED IN ECONET SOFTWARE.

ECONET SHALL NOT BE RESPONSIBLE FOR ANY ECONET SOFTWARE RELEASES 
MADE TO RECEIVERS SPECIFICATION OR CONFORMING TO A PARTICULAR 
STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND 
ECONET'S ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE ECONET 
SOFTWARE RELEASED HEREUNDER SHALL BE, AT ECONET'S SOLE OPTION, TO 
REVISE OR REPLACE THE ECONET SOFTWARE AT ISSUE OR REFUND ANY SOFTWARE 
LICENSE FEES OR SERVICE CHARGES PAID BY RECEIVER TO ECONET FOR SUCH 
ECONET SOFTWARE.
***************************************************************/

/************************************************************************
*                  I N C L U D E S
************************************************************************/
#include <stdio.h>
#include <string.h>

#include <libapi_lib_utility.h>
#include "libapi_lib_qdmamgr.h"

/************************************************************************
*                  D E F I N E S   &   C O N S T A N T S
************************************************************************/

/************************************************************************
*                  M A C R O S
************************************************************************/

/************************************************************************
*                  D A T A	 T Y P E S
************************************************************************/

/************************************************************************
*                  E X T E R N A L	 D A T A   D E C L A R A T I O N S
************************************************************************/
extern const cmds_t QdmaGetCmds[];
extern const cmds_t QdmaSetCmds[];

/************************************************************************
*                  F U N C T I O N   D E F I N I T I O N S
************************************************************************/

/*********************** qdmamgr_root_command **************************/
int DoQdmaLanCmd (int argc, char** argv, void *p);
int DoQdmaWanCmd (int argc, char** argv, void *p);
static int do_set(int argc, char** argv, void *p);
static int do_get(int argc, char** argv, void *p);

/************************************************************************
*                  P U B L I C   D A T A
************************************************************************/

/************************************************************************
*                  P R I V A T E   D A T A
************************************************************************/
static const cmds_t QdmaDbgCmds[] = {
/*	name,		func, 			flags,	argcmin,		argc_errmsg */
	{"set",		do_set,			0,		0,			"Set qdma driver status"},
	{"get",		do_get,			0,		0,			"Get qdma driver status"},
	/* Cmd should be added before this line!  */
	{"help",	api_lib_help,	0,		0,			"Show qdmamgr usage."},
	{NULL, NULL, 0, 0, NULL}
};

/************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
************************************************************************/

/******************************************************************************
 Descriptor:	It's used to set qdma driver status.
 Input Args:	see QdmaDbgCmds.
 Ret Value:	none.
******************************************************************************/
static int do_set(int argc, char* argv[], void *p)
{
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	return subcmd(QdmaSetCmds, argc, argv, p);
}

/******************************************************************************
 Descriptor:	It's used to get qdma driver status.
 Input Args:	see QdmaDbgCmds.
 Ret Value:	none.
******************************************************************************/
static int do_get(int argc, char* argv[], void *p)
{
#if API_CMD_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	return subcmd(QdmaGetCmds, argc, argv, p);
}

/******************************************************************************
 Descriptor:	It's used to config qdma driver.
 Input Args:	see QdmaDbgCmds
 Ret Value: Success: QDMAMGR_SUCCESS
                      Fail: QDMAMGR_FAIL
******************************************************************************/
int DoQdmaLanCmd (int argc, char* argv[], void *p)
{
	char qdmaType[16];
	strcpy(qdmaType, QDMA_TYPE_LAN);
	return subcmd(QdmaDbgCmds, argc, argv, (void *)qdmaType);
}

int DoQdmaWanCmd (int argc, char* argv[], void *p)
{
	char qdmaType[16];
	strcpy(qdmaType, QDMA_TYPE_WAN);
	return subcmd(QdmaDbgCmds, argc, argv, (void *)qdmaType);
}