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
#ifndef __SLIC_PROC_H__
#define __SLIC_PROC_H__

#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define MODULE_NAME		"slic"
#define MAX_CMD_STR_LEN	32
/*********************************
 * Related SLIC Chip Select Offset File Path *
 *********************************/
#define SLIC_CS_OFFSET_FILEPATH "/proc/slic/csOffset"

typedef struct  slic_proc_cmds_s
{
	char cmd[20];
	read_proc_t *read_proc;
	write_proc_t *write_proc;    
} slic_proc_cmds_t;

typedef struct  slic_proc_profile_s
{
    int csOffset;
} slic_proc_profile_t;


/*********************************
 * Related Function Declaration *
 *********************************/


int slic_chipSelect_offset_read(char *buffer, char **start, off_t offset, int length, int *eof, void *data);
int slic_chipSelect_offset_write(struct file *file, const char *buffer, unsigned long count, void *data);

static int voip_version_read_proc(char *page, char **start, off_t off,	int count, int *eof, void *data);
static int voip_version_write_proc(struct file *file, const char *buffer, unsigned long count, void *data);

int slic_get_chipSelect_offset(void);
void slic_proc_profile_init(void);
int slic_proc_init(void);
int slic_proc_dest(void);

#endif


