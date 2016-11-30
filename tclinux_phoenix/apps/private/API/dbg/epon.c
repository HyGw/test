
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
#include <libapi_lib_eponmgr.h>
#include <asm/tc3162/tc3162.h>

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
int DoEponCmd(int argc, char *argv[], void *p);
static int doEponSet(int argc, char *argv[], void *p);
static int doEponGet(int argc, char *argv[], void *p);
static int doEponSetDriverDbgLevel(int argc, char *argv[], void *p);
#ifdef TCSUPPORT_CPU_EN7521
static int doEponSetPhyBurstEn(int argc, char *argv[], void *p);
#endif
static int doEponSetDyingGasp(int argc, char *argv[], void *p);
static int doEponSetDyingGaspMode(int argc, char *argv[], void *p);
static int doEponSetDyingGaspNum(int argc, char *argv[], void *p);
static int doEponGetDyingGasp(int argc, char *argv[], void *p);
#ifdef TCSUPPORT_EPON_POWERSAVING
static int doEponSetEarlyWakeUp(int argc, char *argv[], void *p);
#endif
static int doEponSetSnifferMode(int argc, char *argv[], void *p);




/************************************************************************
*                  P U B L I C   D A T A
*************************************************************************
*/



/************************************************************************
*                  P R I V A T E   D A T A
*************************************************************************
*/
static cmds_t eponDbgCmds[] = {
	{"set",		doEponSet,		0x10,	0,		NULL},
	{"get",		doEponGet,		0x10,	0,		NULL},
	{NULL,		NULL,		0x10,	0,		NULL},
};

static cmds_t eponSetCmds[] = {
	{"debuglevel",		doEponSetDriverDbgLevel,	0x10,	1,	"<debuglevel(0~4)>"},
#ifdef TCSUPPORT_CPU_EN7521
	{"bursten",		doEponSetPhyBurstEn,	0x10,	1,	"<bursten(0 | 1)>"},
#endif
	{"dyinggasp",		doEponSetDyingGasp,	0x10,	0,	"Set the EPON Dying Gasp parameters"},
#ifdef TCSUPPORT_EPON_POWERSAVING
	{"earlywakeup",		doEponSetEarlyWakeUp,	0x10,	0,	"<enable(0 | 1)> <timer> <count>"},
#endif
	{"sniffermode",		doEponSetSnifferMode,	0x10,	1,	"<enable(0 | 1 | 2)> <mask>"},
	{NULL,		NULL,		0x10,	0,		NULL},
};

static cmds_t eponSetDyingGaspCmds[] = {
	{"mode",		doEponSetDyingGaspMode,	0x10,	1,	"<mode(0 | 1)>"},
	{"num",		doEponSetDyingGaspNum,	0x10,	1,	"<num(0~3>"},
	{NULL,		NULL,		0x10,	0,		NULL},
};


static cmds_t eponGetCmds[] = {
	{"dyinggasp",		doEponGetDyingGasp,	0x10,	0,	"Get the EPON Dying Gasp parameters"},
	{NULL,		NULL,		0x10,	0,	NULL},
};

/************************************************************************
*                  F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/
int DoEponCmd(int argc, char *argv[], void *p)
{
	return subcmd(eponDbgCmds, argc, argv, p);   
}

static int doEponSet(int argc, char *argv[], void *p)
{
	return subcmd(eponSetCmds, argc, argv, p);
}

static int doEponGet(int argc, char *argv[], void *p)
{
	return subcmd(eponGetCmds, argc, argv, p);
}

static int doEponSetDyingGasp(int argc, char *argv[], void *p)
{
	return subcmd(eponSetDyingGaspCmds, argc, argv, p);
}

static int doEponSetDriverDbgLevel(int argc, char *argv[], void *p)
{
	unsigned char dbglevel;

	dbglevel = atoi(argv[1]);
	if (eponmgr_lib_set_dbg_level(dbglevel) == EPONMGR_FAIL)
	{
		printf("Exec. Failed: set driver debug level failed\n");
		return EPONMGR_FAIL;
	}

	printf("Exec. Successful\n") ;

	return EPONMGR_SUCCESS;
}
#ifdef TCSUPPORT_CPU_EN7521
static int doEponSetPhyBurstEn(int argc, char *argv[], void *p)
{
	unsigned int mode;

	mode = atoi(argv[1]);
	if (eponmgr_lib_set_phy_burst_en(mode) == EPONMGR_FAIL)
	{
		printf("Exec. Failed: set phy burst_en failed\n");
		return EPONMGR_FAIL;
	}

	printf("Exec. Successful\n") ;

	return EPONMGR_SUCCESS;
}
#endif

static int doEponSetDyingGaspMode(int argc, char *argv[], void *p)
{
	unsigned char mode;

	mode = atoi(argv[1]);
	if (eponmgr_lib_set_dying_gasp_mode(mode) == EPONMGR_FAIL)
	{
		printf("Exec. Failed: set dying gasp mode failed\n");
		return EPONMGR_FAIL;
	}

	printf("Exec. Successful\n") ;

	return EPONMGR_SUCCESS;
}

static int doEponSetDyingGaspNum(int argc, char *argv[], void *p)
{
	unsigned int num;

	num = atoi(argv[1]);
	if (num > 3)
	{
		printf("Input Error: num must be 0~3!\n");
		return EPONMGR_FAIL;
	}

	if (eponmgr_lib_set_dying_gasp_num(num) == EPONMGR_FAIL)
	{
		printf("Exec. Failed: set dying gasp number failed\n");
		return EPONMGR_FAIL;
	}

	printf("Exec. Successful\n") ;

	return EPONMGR_SUCCESS;	
}

static int doEponGetDyingGasp(int argc, char *argv[], void *p)
{
	unsigned char mode;
	unsigned int num;

	if (eponmgr_lib_get_dying_gasp_mode(&mode) == EPONMGR_FAIL)
	{
		printf("Exec. Failed: get dying gasp mode failed\n");
		return EPONMGR_FAIL;
	}

	if (eponmgr_lib_get_dying_gasp_num(&num) == EPONMGR_FAIL)
	{
		printf("Exec. Failed: get dying gasp number failed\n");
		return EPONMGR_FAIL;
	}

	printf("Dying Gasp\n") ;
	printf("  Mode:%s\n", (mode == 0) ? "software" : "hardware") ;
	printf("  Number:%u\n", num) ;

	return EPONMGR_SUCCESS;
}

#ifdef TCSUPPORT_EPON_POWERSAVING
static int doEponSetEarlyWakeUp(int argc, char *argv[], void *p)
{
	unsigned char flag = 0;
	unsigned int timer = 0;
	unsigned short count = 0;

	if (argc != 4)
	{
		printf("Usage: earlywakeup <enable> <timer> <count>\n");
		return EPONMGR_FAIL;
	}

	flag = atoi(argv[1]);
	timer = atoi(argv[2]);
	count = atoi(argv[3]);

	if (eponmgr_lib_set_early_wakeup(flag, timer,count) == EPONMGR_FAIL)
	{
		printf("Exec. Failed: set early wakeup failed\n");
		return EPONMGR_FAIL;
	}

	printf("Exec. Successful\n") ;
	return EPONMGR_SUCCESS;
}
#endif

static int doEponSetSnifferMode(int argc, char *argv[], void *p)
{
	unsigned char flag = 0;
	unsigned short mask = 0;

	if (argc != 3)
	{
		printf("Usage: sniffermode <enable> <mask>\n");
		return EPONMGR_FAIL;
	}

	flag = atoi(argv[1]);
	mask = atoi(argv[2]);

	if ((flag !=0) && (flag !=1) && (flag != 2))
	{
		printf("Error. Please set enable flag to 0:sniffer disable or 1:sniffer normal or 2:sniffer promisc !\n");
		return EPONMGR_FAIL;
	}
	
	if (eponmgr_lib_set_sniffer_mode(flag, mask) == EPONMGR_FAIL)
	{
		printf("Exec. Failed: set sniffer mode failed\n");
		return EPONMGR_FAIL;
	}

	printf("Exec. Successful\n") ;
	return EPONMGR_SUCCESS;
}




