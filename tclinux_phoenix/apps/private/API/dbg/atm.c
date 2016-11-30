
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
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <libapi_lib_utility.h>
#include <api_lib_atmmgr.h>

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



/************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
*************************************************************************
*/
int DoAtmCmd(int argc, char **argv, void *p);  
static int DoAtmSet(int argc, char **argv, void *p);
static int DoAtmGet(int argc, char **argv, void *p);
static int DoSetLvl(int argc, char **argv, void *p);
static int DoSetMacReset(int argc, char **argv, void *p);  
static int DoSetCntClr(int argc, char **argv, void *p);
static int DoGetReg(int argc, char **argv, void *p); 
static int DoGetCnt(int argc, char **argv, void *p);
static int DoGetLinkStats(int argc, char **argv, void *p);



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
static const cmds_t AtmDbgCmds[] = {
    {"set",           DoAtmSet,        0x10,   0,  "Set atm status"},
    {"get",           DoAtmGet,        0x10,   0,  "Get atm status"},
    {"help",	      api_lib_help,	   0x10,   0,  "Show atm usage."},
    {NULL,            NULL,            0x10,   0,  NULL},
};

static const cmds_t AtmSetCmds[] = {
    {"msglvl",        DoSetLvl,             0x10,   1,  "msglvl:0~3,0:none 1:rx error msg 2:tx msg/data 3:tx/rx msg/data"},
    {"reset",         DoSetMacReset,        0x10,   0,  "atm reset"},
    {"clrcnt",        DoSetCntClr,          0x10,   0,  "clear atm sw counter"},    
    {"help",	      api_lib_help,	        0x10,   0,  "Show atm set usage."},
    {NULL,            NULL,                 0x10,   0,  NULL},
};

static const cmds_t AtmGetCmds[] = {
    {"dumpreg",       DoGetReg,          0x10,   0,  "dump atm register"},
    {"count",         DoGetCnt,          0x10,   0,  "get atm sw counter"},      
    {"state",         DoGetLinkStats,    0x10,   1,  "state:vc_no(0~10)"},   
    {"help",	      api_lib_help,	     0x10,   0,  "Show atm get usage."},
    {NULL,            NULL,              0x10,   0,  NULL},
};


int DoAtmCmd(int argc, char **argv, void *p)  
{
    return subcmd(AtmDbgCmds, argc, argv, (void *)AtmDbgCmds);   
}


static int DoAtmSet(int argc, char **argv, void *p)
{
    return subcmd(AtmSetCmds, argc, argv, (void *)AtmSetCmds);
}

static int DoAtmGet(int argc, char **argv, void *p)
{
    return subcmd(AtmGetCmds, argc, argv, (void *)AtmGetCmds);
}

static int DoSetLvl(int argc, char **argv, void *p)
{
    return(adslmgr_lib_atm_set_msglvl( (atoi(argv[1]) & 0xf)));
}

static int DoSetMacReset(int argc, char **argv, void *p)
{
    return(adslmgr_lib_atm_set_reset());
}

static int DoSetCntClr(int argc, char **argv, void *p)
{
    return(adslmgr_lib_atm_set_clrcnt());
}
static int DoGetReg(int argc, char **argv, void *p)
{
    return(adslmgr_lib_atm_get_reg());
}

static int DoGetCnt(int argc, char **argv, void *p)
{
    return(adslmgr_lib_atm_get_counter());
}

static int DoGetLinkStats(int argc, char **argv, void *p)
{
    return(adslmgr_lib_atm_get_linkstate( (atoi(argv[1]) & 0xf)));
}


