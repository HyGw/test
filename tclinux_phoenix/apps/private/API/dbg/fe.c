/***************************************************************
Copyright Statement:

This software/firmware and related documentation (¡°EcoNet Software¡±) 
are protected under relevant copyright laws. The information contained herein 
is confidential and proprietary to EcoNet (HK) Limited (¡°EcoNet¡±) and/or 
its licensors. Without the prior written permission of EcoNet and/or its licensors, 
any reproduction, modification, use or disclosure of EcoNet Software, and 
information contained herein, in whole or in part, shall be strictly prohibited.

EcoNet (HK) Limited  EcoNet. ALL RIGHTS RESERVED.

BY OPENING OR USING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY 
ACKNOWLEDGES AND AGREES THAT THE SOFTWARE/FIRMWARE AND ITS 
DOCUMENTATIONS (¡°ECONET SOFTWARE¡±) RECEIVED FROM ECONET 
AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON AN ¡°AS IS¡± 
BASIS ONLY. ECONET EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, 
WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, 
OR NON-INFRINGEMENT. NOR DOES ECONET PROVIDE ANY WARRANTY 
WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTIES WHICH 
MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE ECONET SOFTWARE. 
RECEIVER AGREES TO LOOK ONLY TO SUCH THIRD PARTIES FOR ANY AND ALL 
WARRANTY CLAIMS RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES 
THAT IT IS RECEIVER¡¯S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD 
PARTY ALL PROPER LICENSES CONTAINED IN ECONET SOFTWARE.

ECONET SHALL NOT BE RESPONSIBLE FOR ANY ECONET SOFTWARE RELEASES 
MADE TO RECEIVER¡¯S SPECIFICATION OR CONFORMING TO A PARTICULAR 
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
#include <asm/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <ctype.h>

#include "libapi_lib_utility.h"
#include "libapi_lib_fe.h"


static int do_set(int argc, char *argv[], void *p);
static int do_add(int argc, char *argv[], void *p);
static int do_del(int argc, char *argv[], void *p);
static int do_add_eth(int argc, char *argv[], void *p);
static int do_add_ppp(int argc, char *argv[], void *p);
static int do_add_ip(int argc, char *argv[], void *p);
static int do_add_tcp(int argc, char *argv[], void *p);
static int do_add_udp(int argc, char *argv[], void *p);
static int do_add_dscp(int argc, char *argv[], void *p);
static int do_add_trfc(int argc, char *argv[], void *p);
static int do_del_eth(int argc, char *argv[], void *p);
static int do_del_ppp(int argc, char *argv[], void *p);
static int do_del_ip(int argc, char *argv[], void *p);
static int do_del_tcp(int argc, char *argv[], void *p);
static int do_del_udp(int argc, char *argv[], void *p);
static int do_del_dscp(int argc, char *argv[], void *p);
static int do_del_trfc(int argc, char *argv[], void *p);
static int do_set_tx(int argc, char *argv[], void *p);
static int do_set_rxuc(int argc, char *argv[], void *p);
static int do_set_rxbc(int argc, char *argv[], void *p);
static int do_set_rxmc(int argc, char *argv[], void *p);
static int do_set_rxoc(int argc, char *argv[], void *p);



static cmds_t addCmds[] = {
    {"eth", do_add_eth,   		0x0, 0, NULL},
    {"ppp", do_add_ppp,   		0x0, 0, NULL},
	{"ip",	do_add_ip,		0x0, 0, NULL},
	{"tcp",	do_add_tcp,		0x0, 0, NULL},	
	{"udp",	do_add_udp,		0x0, 0, NULL},
	{"dscp",do_add_dscp,		0x0, 0, NULL},	
	{"trfc",do_add_trfc,		0x0, 0, NULL},	
    {NULL,  NULL,       		0x0, 0, NULL}
};

static cmds_t delCmds[] = {
    {"eth", do_del_eth,   		0x0, 0, NULL},
    {"ppp", do_del_ppp,   		0x0, 0, NULL},
	{"ip",	do_del_ip,		0x0, 0, NULL},
	{"tcp",	do_del_tcp,		0x0, 0, NULL},	
	{"udp",	do_del_udp,		0x0, 0, NULL},	
	{"dscp",do_del_dscp,		0x0, 0, NULL},	
	{"trfc",do_del_trfc,		0x0, 0, NULL},		
    {NULL,  NULL,       		0x0, 0, NULL}
};

static cmds_t setCmds[] = {
    {"tx",    do_set_tx,     	0x0, 0, NULL},
    {"rxuc",  do_set_rxuc,   		0x0, 0, NULL},
    {"rxbc",  do_set_rxbc,     	0x0, 0, NULL},
    {"rxmc",  do_set_rxmc,   		0x0, 0, NULL},
    {"rxoc",  do_set_rxoc,     	0x0, 0, NULL},   
    {NULL,    NULL,      	 		0x0, 0, NULL}
};


static cmds_t feCmds[] = {
    {"set",     do_set,     0x10, 0, NULL},
    {"add",   	do_add,   	0x10, 0, NULL},
    {"del",   	do_del,   	0x10, 0, NULL},    
    {NULL,      NULL,       0x10, 0, NULL}
};


static int do_add_eth(int argc, char *argv[], void *p)
{
	unsigned short type;

	if (argc < 2)
	{
		printf("fe add eth <type> \n");
		return 0;
	}
	type = strtoul(argv[1],NULL,16);
	return fe_lib_add_vip_ether(type);
}

static int do_add_ppp(int argc, char *argv[], void *p)
{
	unsigned short type;

	if (argc < 2)
	{
		printf("fe add ppp <type> \n");
		return 0;
	}
	type = strtoul(argv[1],NULL,16);
	return fe_lib_add_vip_ppp(type);

}

static int do_add_ip(int argc, char *argv[], void *p)
{
	unsigned short type;

	if (argc < 2)
	{
		printf("fe add ip <type> \n");
		return 0;
	}
	type = strtoul(argv[1],NULL,16);
	return fe_lib_add_vip_ip(type);

}

static int do_add_tcp(int argc, char *argv[], void *p)
{
	unsigned short src,dst;

	if (argc < 3)
	{
		printf("fe add tcp <src> <dst> \n");
		return 0;
	}
	src = strtoul(argv[1],NULL,16);
	dst = strtoul(argv[2],NULL,16);	
	
	return fe_lib_add_vip_tcp(src,dst);

}

static int do_add_udp(int argc, char *argv[], void *p)
{
	unsigned short src,dst;

	if (argc < 3)
	{
		printf("fe add udp <src> <dst> \n");	
		return 0;
	}
	src = strtoul(argv[1],NULL,16);
	dst = strtoul(argv[2],NULL,16);	

	return fe_lib_add_vip_udp(src,dst);

}

static int do_add_dscp(int argc, char *argv[], void *p)
{
	unsigned short svlan,cvlan,prio,mask;

	if (argc < 5)
	{
		printf("fe add dscp <svlan> <cvlan> <dscp> <mask> \n");
		return 0;
	}
	svlan = strtoul(argv[1],NULL,16);
	cvlan = strtoul(argv[2],NULL,16);	
	prio = strtoul(argv[3],NULL,16);
	mask = strtoul(argv[4],NULL,16);
	return fe_lib_add_l2lu_vlan_dscp(svlan,cvlan,prio,mask);

}

static int do_add_trfc(int argc, char *argv[], void *p)
{
	unsigned short svlan,cvlan,prio,mask;

	if (argc < 5)
	{
		printf("fe add trfc <svlan> <cvlan> <trfc> <mask> \n");
		return 0;
	}
	svlan = strtoul(argv[1],NULL,16);
	cvlan = strtoul(argv[2],NULL,16);	
	prio = strtoul(argv[3],NULL,16);
	mask = strtoul(argv[4],NULL,16);
	return fe_lib_add_l2lu_vlan_trfc(svlan,cvlan,prio,mask);

}

static int do_del_eth(int argc, char *argv[], void *p)
{
	unsigned short type;

	if (argc < 2)
	{
		printf("fe del eth <type> \n");
		return 0;
	}
	type = strtoul(argv[1],NULL,16);
	return fe_lib_del_vip_ether(type);

}

static int do_del_ppp(int argc, char *argv[], void *p)
{
	unsigned short type;

	if (argc < 2)
	{
		printf("fe del ppp <type> \n");
		return 0;
	}
	type = strtoul(argv[1],NULL,16);
	return fe_lib_del_vip_ppp(type);

}

static int do_del_ip(int argc, char *argv[], void *p)
{
	unsigned short type;

	if (argc < 2)
	{
		printf("fe del ip <type> \n");
		return 0;
	}
	type = strtoul(argv[1],NULL,16);
	return fe_lib_del_vip_ip(type);

}

static int do_del_tcp(int argc, char *argv[], void *p)
{
	unsigned short src,dst;

	if (argc < 3)
	{
		printf("fe del tcp <src> <dst> \n");	
		return 0;
	}
	src = strtoul(argv[1],NULL,16);
	dst = strtoul(argv[2],NULL,16);	
	
	return fe_lib_del_vip_tcp(src,dst);
}

static int do_del_udp(int argc, char *argv[], void *p)
{
	unsigned short src,dst;

	if (argc < 3)
	{
		printf("fe del udp <src> <dst> \n");
		return 0;
	}
	src = strtoul(argv[1],NULL,16);
	dst = strtoul(argv[2],NULL,16);	
	
	return fe_lib_del_vip_udp(src,dst);
}

static int do_del_dscp(int argc, char *argv[], void *p)
{
	unsigned short svlan,cvlan,prio,mask;

	if (argc < 5)
	{
		printf("fe del dscp <svlan> <cvlan> <dscp> <mask> \n");
		return 0;
	}
	svlan = strtoul(argv[1],NULL,16);
	cvlan = strtoul(argv[2],NULL,16);	
	prio = strtoul(argv[3],NULL,16);
	mask = strtoul(argv[4],NULL,16);
	return fe_lib_del_l2lu_vlan_dscp(svlan,cvlan,prio,mask);
}

static int do_del_trfc(int argc, char *argv[], void *p)
{
	unsigned short svlan,cvlan,prio,mask;

	if (argc < 5)
	{
		printf("fe del trfc <svlan> <cvlan> <trfc> <mask> \n");	
		return 0;
	}
	svlan = strtoul(argv[1],NULL,16);
	cvlan = strtoul(argv[2],NULL,16);	
	prio = strtoul(argv[3],NULL,16);
	mask = strtoul(argv[4],NULL,16);
	return fe_lib_del_l2lu_vlan_trfc(svlan,cvlan,prio,mask);
}

static int do_set_tx(int argc, char *argv[], void *p)
{
	unsigned int rate,mode;

	if (argc < 3)
	{
		printf("fe set tx <rate> <mode> \n");
		return 0;
	}
	rate = strtoul(argv[1],NULL,16);
	mode = strtoul(argv[2],NULL,16);
	
	return fe_lib_set_tx_rate(rate,mode);
}

static int do_set_rxuc(int argc, char *argv[], void *p)
{
	unsigned int rate,mode;

	if (argc < 3)
	{
		printf("fe set rxuc <rate> <mode> \n");
		return 0;
	}
	rate = strtoul(argv[1],NULL,16);
	mode = strtoul(argv[2],NULL,16);
	
	return fe_lib_set_rxuc_rate(rate,mode);

}

static int do_set_rxbc(int argc, char *argv[], void *p)
{
	unsigned int rate,mode;

	if (argc < 3)
	{
		printf("fe set rxbc <rate> <mode> \n");
		return 0;
	}
	rate = strtoul(argv[1],NULL,16);
	mode = strtoul(argv[2],NULL,16);
	
	return fe_lib_set_rxbc_rate(rate,mode);

}

static int do_set_rxmc(int argc, char *argv[], void *p)
{
	unsigned int rate,mode;

	if (argc < 3)
	{
		printf("fe set rxmc <rate> <mode> \n");
		return 0;
	}
	rate = strtoul(argv[1],NULL,16);
	mode = strtoul(argv[2],NULL,16);
	
	return fe_lib_set_rxmc_rate(rate,mode);

}

static int do_set_rxoc(int argc, char *argv[], void *p)
{
	unsigned short rate,mode;

	if (argc < 3)
	{
		printf("fe set rxoc <rate> <mode> \n");
		return 0;
	}
	rate = strtoul(argv[1],NULL,16);
	mode = strtoul(argv[2],NULL,16);

	return fe_lib_set_rxoc_rate(rate,mode);
}


static int do_del(int argc, char *argv[], void *p)
{
	return subcmd(delCmds, argc, argv, NULL);
}

static int do_add(int argc, char *argv[], void *p)
{
	return subcmd(addCmds, argc, argv, NULL);
}

static int do_set(int argc, char *argv[], void *p)
{
	return subcmd(setCmds, argc, argv, NULL);
}

int main(int argc, char **argv) 
{
 //   init_compileoption_val();

	return subcmd(feCmds, argc, argv, NULL);
}


