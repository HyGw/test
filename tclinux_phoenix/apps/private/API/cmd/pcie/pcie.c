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
#include "libapi_lib_pcie.h"
//#include "linux/libcompileoption.h"

static int do_get(int argc, char *argv[], void *p);
static int do_set(int argc, char *argv[], void *p);
static int do_get_conf(int argc, char *argv[], void *p);
static int do_get_aspm(int argc, char *argv[], void *p);
static int do_get_speed(int argc, char *argv[], void *p);
static int do_get_linkstate(int argc, char *argv[], void *p);
static int do_get_count(int argc, char *argv[], void *p);
static int do_set_conf(int argc, char *argv[], void *p);
static int do_set_aspm(int argc, char *argv[], void *p);
static int do_set_speed(int argc, char *argv[], void *p);
static int do_set_mac_reset(int argc, char *argv[], void *p);
static int do_set_hot_reset(int argc, char *argv[], void *p);

static cmds_t getCmds[] = {
    {"conf",    do_get_conf,   		0x0, 0, NULL},
    {"aspm",   	do_get_aspm,   		0x0, 0, NULL},
	{"speed",	do_get_speed,		0x0, 0, NULL},
	{"link",	do_get_linkstate,   0x0, 0, NULL},
	{"count",	do_get_count,	 	0x0, 0, NULL},
    {NULL,      NULL,       		0x0, 0, NULL}
};

static cmds_t setCmds[] = {
    {"conf",    do_set_conf,     	0x0, 0, NULL},
    {"aspm",   	do_set_aspm,   		0x0, 0, NULL},
    {"speed",   do_set_speed,     	0x0, 0, NULL},
    {NULL,      NULL,      	 		0x0, 0, NULL}
};


static cmds_t pcieCmds[] = {
    {"get",     do_get,     0x10, 0, NULL},
    {"set",   	do_set,   	0x10, 0, NULL},
    {NULL,      NULL,       0x10, 0, NULL}
};



static int do_get_conf(int argc, char *argv[], void *p)
{
	int idx,offset,ret,value;
	
	if (argc < 3)
	{
		printf("pcie get conf <idx> <offset>  idx = 0(rc0)|1(rc1)|2(ep0)|3(ep1) \n");
		return 0;
	}
	idx = strtoul(argv[1],NULL,10);
	offset = strtoul(argv[2],NULL,16);
	ret = pcie_lib_get_confreg(idx,offset,&value);
	if (ret >= 0){
		printf("Conf Read(%d): offset(%x) value(%08x) \n",idx,offset,value);
	}else{
		printf("Conf Read(%d):Fail \n",idx);
	}
	
	return 0;
}


static int do_get_aspm(int argc, char *argv[], void *p)
{
	int idx,ret,value;
	
	char* str[4] = {"Disable","L0s Only","L1 Only","L0s + L1"};
	
	if (argc < 2)
	{
		printf("pcie get conf <idx>  idx = 0(rc0)|1(rc1)|2(ep0)|3(ep1) \n");
		return 0;
	}
	
	idx = strtoul(argv[1],NULL,10);
	ret = pcie_lib_get_aspm(idx,&value);
	if (ret >= 0){
		printf("Get ASPMd(%d): %s \n",idx,str[value&0x3]);
	}else{
		printf("Get ASPM(%d):Fail \n",idx);
	}

	return 0;
}

static int do_get_speed(int argc, char *argv[], void *p)
{
	int idx,ret,value;
	char* str[4] = {"Error","2.5GT","5GT","Error"};
	
	if (argc < 2)
	{
		printf("pcie get conf <idx>  idx = 2(ep0)|3(ep1) \n");
		return 0;
	}
	
	idx = strtoul(argv[1],NULL,10);
	ret = pcie_lib_get_speed(idx,&value);
	if (ret >= 0){
		printf("Get Speed(%d): %s \n",idx,str[value&0x3]);
	}else{
		printf("Get Speed(%d):Fail \n",idx);
	}

	return 0;
}

static int do_get_linkstate(int argc, char *argv[], void *p)
{
	int idx,ret, value;
	char* str[2] = {"Down","Up"};
	
	if (argc < 2)
	{
		printf("pcie get conf <idx>  idx = 0(rc0)|1(rc1) \n");
		return 0;
	}
	
	idx = strtoul(argv[1],NULL,10);
	ret = pcie_lib_get_linkstate(idx,&value);
	if (ret >= 0){
		printf("Get Link(%d): %s \n",idx,str[value&0x1]);
	}else{
		printf("Get Link(%d):Fail \n",idx);
	}

	return 0;


}

static int do_get_count(int argc, char *argv[], void *p)
{
	int idx,ret;
	int  cnt[10];

	if (argc < 2)
	{
		printf("pcie get count <idx>  idx = 0(rc0)|1(rc1) \n");
		return 0;
	}
	
	idx = strtoul(argv[1],NULL,10);
	ret = pcie_lib_get_count(idx,&cnt);
	if (ret >= 0){
		printf("Get CountS(%d): \n",idx);
		printf("ECRC Error = %08x \n",cnt[0]);
		printf("TLP CRC Error = %08x \n",cnt[1]);
		printf("DLLP CRC Error = %08x \n",cnt[2]);
		printf("Replay Timeout Error = %08x \n",cnt[3]);
		printf("Replay Rollover Error = %08x \n",cnt[4]);	
		printf("TLP Unexpectd Error = %08x \n",cnt[5]);		
	}else{
		printf("Get CountS(%d):Fail \n",idx);
	}
	
	return 0;
}

static int do_set_conf(int argc, char *argv[], void *p)
{
	int idx,offset,value;

	if (argc < 4)
	{
		printf("pcie set conf <idx> <offset> <value>  idx = 0(rc0)|1(rc1)|2(ep0)|3(ep1) \n");
		return 0;
	}
	idx = strtoul(argv[1],NULL,10);
	offset = strtoul(argv[2],NULL,16);
	value = strtoul(argv[3],NULL,16);
	if (pcie_lib_set_confreg(idx,offset,value) >= 0){
		printf("Conf Write(%d): offset(%x) value(%08x) Ok \n",idx,offset,value);
	}else{
		printf("Conf Write(%d):Fail \n",idx);
	}
	
	return 0;
}

static int do_set_aspm(int argc, char *argv[], void *p)
{
	int idx,value;
	
	if (argc < 3)
	{
		printf("pcie set aspm <idx> <val> idx = 0(rc0)|1(rc1)|2(ep0)|3(ep1) val = 0(disable)|1(Los Only)|2(L1 Only)|3(Both) \n");
		return 0;
	}
	idx = strtoul(argv[1],NULL,10);
	value = strtoul(argv[2],NULL,10);
	if (pcie_lib_set_aspm(idx,value) >= 0){
		printf("Set ASPM(%d): (%d) Ok \n",idx,value);
	}else{
		printf("Set ASPM(%d):Fail \n",idx);
	}
	
	return 0;

}

static int do_set_speed(int argc, char *argv[], void *p)
{
	int idx,value;
	
	if (argc < 3)
	{
		printf("pcie set speed <idx> <val> idx = 2(ep0)|3(ep1) val = 1(2.5GT)|2(5GT)\n");
		return 0;
	}
	idx = strtoul(argv[1],NULL,10);
	value = strtoul(argv[2],NULL,10);
	if (pcie_lib_set_speed(idx,value) >= 0){
		printf("Set Speed(%d): (%d) Ok \n",idx,value);
	}else{
		printf("Set Speed(%d):Fail \n",idx);
	}
	
	return 0;
}

static int do_get(int argc, char *argv[], void *p)
{
	return subcmd(getCmds, argc, argv, NULL);
}


static int do_set(int argc, char *argv[], void *p)
{
	return subcmd(setCmds, argc, argv, NULL);
}

int main(int argc, char **argv) 
{
 //   init_compileoption_val();

	return subcmd(pcieCmds, argc, argv, NULL);
}


