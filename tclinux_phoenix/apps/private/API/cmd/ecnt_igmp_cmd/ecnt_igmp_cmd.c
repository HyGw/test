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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "libapi_lib_utility.h"
#include "libapi_lib_igmpmgr.h"

static int do_set_igmp_snooping_on_off(int argc, char **argv)
{
    unsigned char enable = E_IGMP_SNOOPING_ON;

	if (argc < 2)
	{
		printf("Usage: ecnt_igmp_cmd set_snooping [0:disable 1:enable]\n");
		return -1;
	}
	
    enable = (strtoul(argv[1],NULL,16) & 0xf);
    
	ecnt_igmp_snooping_on_off_set(enable);
    
    printf("success.\n");
    
	return 0;
}

static int do_get_igmp_snooping_on_off(int argc, char **argv)
{
    unsigned char enable = E_IGMP_SNOOPING_ON;
	
	ecnt_igmp_snooping_on_off_get(&enable);
    
    printf("snooping enable = %d.\n", enable);
    
	return 0;
}

static cmds_t Ecnt_igmp_Cmds[] = 
{
    {"get_snooping",			do_get_igmp_snooping_on_off,	0x0, 0, NULL},
    {"set_snooping",      	    do_set_igmp_snooping_on_off,    0x0, 0, NULL},
    {NULL,                      NULL,                           0x0, 0, NULL},
};

int main(int argc, char **argv) 
{
    return subcmd(Ecnt_igmp_Cmds, argc, argv, NULL);
}


