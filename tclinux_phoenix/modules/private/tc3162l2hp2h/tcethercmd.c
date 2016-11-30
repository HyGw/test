/************************************************************************
 *
 *	Copyright (C) 2006 Trendchip Technologies, Corp.
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

#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/proc_fs.h>
#include <asm/addrspace.h>
#include <asm/tc3162/tc3162.h>	
#include <asm/tc3162/ledcetrl.h>	
#include <asm/tc3162/cmdparse.h>
#include "tcethercmd.h"
#include "../tcphy/tcetherphy.h"

#ifdef TC2031_DEBUG
extern int subcmd(const cmds_t tab[], int argc, char *argv[], void *p);
extern int cmd_register(cmds_t *cmds_p);
extern void macDispCnt(void);
extern void macClearCnt(void);
extern void setUserSpaceFlag(int num);


 const cmds_t etherCmds[] = {
	{"ip",		doIp,	0x12,  	0,  NULL},
	{"set",		doSet,		0x12,  	0,  NULL},
	{"driver",		doDriver,	0x12,  	0,  NULL},
	{NULL,			NULL,			0x10,	0,	NULL},	
};

 const cmds_t ipCmds[] = {
	{"addr",		doAddr,	0x02,  	0,  NULL},
	{NULL,			NULL,			0x10,	0,	NULL},	
};

 const cmds_t setCmds[] = {
	{"lan",		doLan,	0x12,  	0,  NULL},
	{NULL,			NULL,			0x10,	0,	NULL},	
};

 const cmds_t lanCmds[] = {
	{"ipaddr",		doIpaddr,	0x02,  	1,  "set lan ipaddr <ipaddr> <subnet mask>"},
	{NULL,			NULL,			0x10,	0,	NULL},	
};

 const cmds_t driverCmds[] = {
	{"cnt",		doCnt,	0x12,  	0,  NULL},
	{NULL,			NULL,			0x10,	0,	NULL},	
};

 const cmds_t cntCmds[] = {
	{"disp",		doDisp,	0x02,  	1,  "disp <name>"},
	{"clear",		doClear,	0x02,  	1,  NULL},	
	{NULL,			NULL,			0x10,	0,	NULL}	
};


/*_____________________________________________________________________________
**      function name: doIp
**      descriptions:
**           register subcommand of "ether ip". 
**             
**      parameters:
**            argc : argument number
**            argv : argument point
**		  p     :  no use
**      global:
**            None
**             
**      return:
**            Success:        0
**            Otherwise:     -1
**	     
**      call:
**      	
**      revision:
**      
**____________________________________________________________________________
*/

 int doIp(int argc, char *argv[], void *p)
{
	return subcmd(ipCmds, argc, argv, p);
}

/*_____________________________________________________________________________
**      function name: doSet
**      descriptions:
**           register subcommand of "ether set". 
**             
**      parameters:
**            argc : argument number
**            argv : argument point 
**		 p     :  no use
**      global:
**            None
**             
**      return:
**            Success:        0
**            Otherwise:     -1
**	     
**      call:
**      	
**      revision:
**      
**____________________________________________________________________________
*/

 int doSet(int argc, char *argv[], void *p)
{
	return subcmd(setCmds, argc, argv, p);
}

/*_____________________________________________________________________________
**      function name: doDriver
**      descriptions:
**           register subcommand of "ether driver". 
**             
**      parameters:
**            argc : argument number
**            argv : argument point 
**		 p     :  no use
**      global:
**            None
**             
**      return:
**            Success:        0
**            Otherwise:     -1
**	     
**      call:
**      	
**      revision:
**      
**____________________________________________________________________________
*/

int doDriver(int argc, char *argv[], void *p)
{
	return subcmd(driverCmds, argc, argv, p);
}

/*_____________________________________________________________________________
**      function name: doAddr
**      descriptions:
**           ci command "ether ip addr". 
**           Show ip address. 
**      parameters:
**            argc : argument number
**            argv : argument point 
**		 p     :  no use
**      global:
**            None
**             
**      return:
**            Success:        0
**            Otherwise:     -1
**	     
**      call:
**      	
**      revision:
**      
**____________________________________________________________________________
*/

 int doAddr(int argc, char *argv[], void *p)
{
	setUserSpaceFlag(2);
	return 0;
}

/*_____________________________________________________________________________
**      function name: doLan
**      descriptions:
**           register subcommand of "ether set Lan". 
**             
**      parameters:
**            argc : argument number
**            argv : argument point 
**		 p     :  no use
**      global:
**            None
**             
**      return:
**            Success:        0
**            Otherwise:     -1
**	     
**      call:
**      	
**      revision:
**      
**____________________________________________________________________________
*/

 int doLan(int argc, char *argv[], void *p)
{
	return subcmd(lanCmds, argc, argv, p);
}

/*_____________________________________________________________________________
**      function name: doIpaddr
**      descriptions:
**           ci command of "ether set Lan ip addr". 
**            Set lan ip addr.
**      parameters:
**            argc : argument number
**            argv : argument point 
**		 p     :  no use
**      global:
**            None
**             
**      return:
**            Success:        0
**            Otherwise:     -1
**	     
**      call:
**      	
**      revision:
**      
**____________________________________________________________________________
*/

 int doIpaddr(int argc, char *argv[], void *p)
{
	setUserSpaceFlag(3);
	return 0;
}

/*_____________________________________________________________________________
**      function name: doCnt
**      descriptions:
**           register subcommand of "ether driver cnt". 
**             
**      parameters:
**            argc : argument number
**            argv : argument point 
**		 p     :  no use
**      global:
**            None
**             
**      return:
**            Success:        0
**            Otherwise:     -1
**	     
**      call:
**      	
**      revision:
**      
**____________________________________________________________________________
*/
 int doCnt(int argc, char *argv[], void *p)
{
	return subcmd(cntCmds, argc, argv, p);
}

/*_____________________________________________________________________________
**      function name: doDisp
**      descriptions:
**           ci command of "ether set Lan ip addr disp". 
**            Display counters of mac.
**      parameters:
**            argc : argument number
**            argv : argument point 
**		 p     :  no use
**      global:
**            None
**             
**      return:
**            Success:        0
**            Otherwise:     -1
**	     
**      call:
**      	
**      revision:
**      
**____________________________________________________________________________
*/

 int doDisp(int argc, char *argv[], void *p)
{
	macDispCnt();
	return 0;
}

/*_____________________________________________________________________________
**      function name: doClear
**      descriptions:
**           ci command of "ether set Lan ip addr clear". 
**            Clear counters of mac.
**      parameters:
**            argc : argument number
**            argv : argument point 
**		 p     :  no use
**      global:
**            None
**             
**      return:
**            Success:        0
**            Otherwise:     -1
**	     
**      call:
**      	
**      revision:
**      
**____________________________________________________________________________
*/
 int doClear(int argc, char *argv[], void *p)
{
	macClearCnt();
	return 0;
}

/*_____________________________________________________________________________
**      function name: doEther
**      descriptions:
**           register subcommand of "ether ". 
**             
**      parameters:
**            argc : argument number
**            argv : argument point 
**		 p     :  no use
**      global:
**            None
**             
**      return:
**            Success:        0
**            Otherwise:     -1
**	     
**      call:
**      	
**      revision:
**      
**____________________________________________________________________________
*/
 int doEther(int argc, char *argv[], void *p)
{
	return subcmd(etherCmds, argc, argv, p);
}

/*_____________________________________________________________________________
**      function name: tcethercmd
**      descriptions:
**           register rootcommand of "ether ". 
**             
**      parameters:
**            none
**      global:
**            None
**             
**      return:
**            none
**	     
**      call:
**      	
**      revision:
**      
**____________________________________________________________________________
*/

void tcethercmd(void)
{
	cmds_t ether_cmd;

	ether_cmd.name= "ether";
	ether_cmd.func=doEther;
	ether_cmd.flags=0x12;
	ether_cmd.argcmin=0;
	ether_cmd.argc_errmsg=NULL;
	
	cmd_register(&ether_cmd);
}

#endif
