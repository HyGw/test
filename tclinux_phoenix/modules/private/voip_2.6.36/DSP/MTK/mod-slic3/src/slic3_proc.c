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


#include "slic3_proc.h"
#include <../../install/include/voip_version.h>

static  slic_proc_cmds_t proc_slic_cmds[] =
{
	//{"csOffset", slic_chipSelect_offset_read, slic_chipSelect_offset_write},
	{"ver_slic3", voip_version_read_proc, voip_version_write_proc},

	{"",0, 0},
};

static slic_proc_profile_t  slic_proc_profile;
static struct proc_dir_entry   *slic_proc_dir=NULL;


int 
slic_chipSelect_offset_read(char *buffer, char **start, off_t offset, 
int length, int *eof, void *data){

    int len = 0;
    len += sprintf(buffer + len, "%d", slic_proc_profile.csOffset);
    return  len;
}

int 
slic_chipSelect_offset_write(struct file *file, const char *buffer, 
unsigned long count, void *data){

	char cmdStr[MAX_CMD_STR_LEN + 1];
	int strLen;

	strLen = (count > MAX_CMD_STR_LEN) ? MAX_CMD_STR_LEN : (count - 1);

	if(copy_from_user(cmdStr, buffer, strLen))
                return -EFAULT;

	cmdStr[strLen] = 0;
	sscanf(cmdStr, "%d", &(slic_proc_profile.csOffset));
        return count;
}

static int voip_version_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	
	int len = 0;
	
	len = sprintf(page, "%s\n", RELEASE_VOIP_VERSION);

	len -= off;
	*start = page + off;
	
	if (len > count)
		len = count;
	else
		*eof = 1;
	
	if (len < 0)
		len = 0;
	
	return len;
}
static int voip_version_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	return count;
}



int 
slic_get_chipSelect_offset(void){
	return slic_proc_profile.csOffset;
}

void
slic_proc_profile_init(void){
	slic_proc_profile_t * pProfile = &slic_proc_profile;
		
	memset(pProfile, 0, sizeof(slic_proc_profile));
		
//#if defined(SLIC_SILICON) &&  !defined(SI3217X)
#if 0
	pProfile->csOffset = 0;
#else
	pProfile->csOffset = 1;
#endif

}

int 
slic_proc_init(void){
	int i;
	struct proc_dir_entry   *temp_dir=0;
	slic_proc_profile_init();

	slic_proc_dir = proc_mkdir(MODULE_NAME, NULL);
	if(slic_proc_dir == NULL)
	{
		/* failed when creating file */
		printk("Error while creating %s directory\n", MODULE_NAME);
		return -1;
	}

	i=0;
	while(strlen(proc_slic_cmds[i].cmd) != 0)
	{
		temp_dir = create_proc_read_entry(proc_slic_cmds[i].cmd, S_IWUSR|S_IRUSR|
			S_IRGRP|S_IROTH, slic_proc_dir, proc_slic_cmds[i].read_proc, NULL);
		temp_dir->write_proc = proc_slic_cmds[i].write_proc;
		i++;
	}
	printk("slic3[%s]\n",RELEASE_VOIP_VERSION);
	return 0;
}

int 
slic_proc_dest(void){
	int i;
	i=0;
	while(strlen(proc_slic_cmds[i].cmd) != 0)
	{
		remove_proc_entry(proc_slic_cmds[i].cmd, slic_proc_dir);
		i++;
	}
	remove_proc_entry(MODULE_NAME, NULL);
	return 0;
}

