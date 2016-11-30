
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
*************************************************************************
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include "dbgcmd.h"
#include <libapi_lib_utility.h>

/************************************************************************
*                  D E F I N E S   &   C O N S T A N T S
*************************************************************************
*/

/************************************************************************
*                  M A C R O S
*************************************************************************
*/

/************************************************************************
*                  D A T A	 T Y P E S
*************************************************************************
*/

/************************************************************************
*                  E X T E R N A L	 D A T A   D E C L A R A T I O N S
*************************************************************************
*/
#ifdef TCSUPPORT_WAN_ATM
extern int DoAtmCmd(int argc, char **argv, void *p);
#endif
#ifdef TCSUPPORT_WAN_PTM
extern int DoPtmCmd(int argc, char **argv, void *p);
#endif
#ifdef TCSUPPORT_BONDING
extern int DoBondingCmd(int argc, char **argv, void *p);
#endif
#ifdef TCSUPPORT_WAN_GPON
extern int DoGponCmd(int argc, char **argv, void *p);
#endif
#ifdef TCSUPPORT_WAN_EPON
extern int DoEponCmd(int argc, char **argv, void *p);
#endif
#ifdef TCSUPPORT_QDMA_BUFMGR
extern int DoQdmaLanCmd (int argc, char** argv, void *p);
extern int DoQdmaWanCmd (int argc, char** argv, void *p);
#endif


#ifdef TCSUPPORT_UART2
extern int DoUART2Cmd (int argc, char** argv, void *p);
#endif

extern int DoInterfaceCmd (int argc, char* argv[], void *p); 


/************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
*************************************************************************
*/



/************************************************************************
*                  P U B L I C   D A T A
*************************************************************************
*/



/************************************************************************
*                  P R I V A T E   D A T A
*************************************************************************
*/


/************************************************************************
*                  F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/
static cmds_t DbgCmds[] = {
#ifdef TCSUPPORT_WAN_ATM
    {"atm",           DoAtmCmd,        0x10,   0,  NULL},
#endif
#ifdef TCSUPPORT_WAN_PTM
    {"ptm",           DoPtmCmd,        0x10,   0,  NULL},
#endif
#ifdef TCSUPPORT_BONDING
    {"bonding",       DoBondingCmd,    0x10,   0,  NULL},
#endif
#ifdef TCSUPPORT_WAN_GPON
    {"gpon",          DoGponCmd,       0x10,   0,  NULL},
#endif
#ifdef TCSUPPORT_WAN_EPON
    {"epon",		  DoEponCmd,	   0x10,   0,  NULL},
#endif
#ifdef TCSUPPORT_QDMA_BUFMGR
    {"qdma_lan",      DoQdmaLanCmd,    0x10,   0,  NULL},
    {"qdma_wan",      DoQdmaWanCmd,    0x10,   0,  NULL},
#endif
    {"system",        DoInterfaceCmd,     0x10,   0,  NULL},

#ifdef TCSUPPORT_UART2
    {"uart2",        DoUART2Cmd,     0x10,   0,  NULL},
#endif


    {NULL,            NULL,            0x10,   0,  NULL},
};

int main(int argc, char **argv) 
{
    return subcmd(DbgCmds, argc, argv, (void *)DbgCmds);
}

