/*********************************************************************
* LEGAL DISCLAIMER
*
* (Header of MediaTek Software/Firmware Release or Documentation)
*
* BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY 
* ACKNOWLEDGES AND AGREES THAT THE SOFTWARE/FIRMWARE AND ITS 
* DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED FROM MEDIATEK AND/OR 
* ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS ONLY. 
* MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR 
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF 
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR 
* NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY 
* WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH 
* MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK 
* SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY
* WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO NOT BE 
* RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S 
* SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
* BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND 
* CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED 
* HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE 
* MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR 
* SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE
* AT ISSUE.
*
* THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN 
* ACCORDANCE WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING
* ITS CONFLICT OF LAWS PRINCIPLES.
**********************************************************************
*/

/*
 *	File:	spi_test.c
 *	 
 *	Author: Peter Jiang
 *	 		
 *	Last Revision Date: 2015.6.11
 *	 
 *	Description:
 *	test spi read/write via accessing /dev/spi (spi.ko)
 *
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/ioctl.h>
#include "slic_ctrl_api.h"


int spi_test_help(void)
{
	printf ("spi_test commad instructions are as follows: \n");
	printf ("spi_test init format: spi_test init slic_type_id interface_type_id \n");
	printf ("spi_test init example: spi_test init 2 1 \n");
	printf ("spi_test write and read format: \n"); 
	printf ("spi_test slic_type_id interface_type_id slic_id ctrl cmd dataLen data\n");
	printf ("spi_test write example: spi_test 2 1 0 0x7E 0x25 4 ff ee ff ee \n");
	printf ("spi_test read example: spi_test 2 1 0 0xBE 0x25 4 \n");
}

int main (int argc, char *argv[])
{
	int ret = 0;
	int i = 0;
	

	spi_cmd_struct cmd = {0};

	if (argc > 1)
	{
       if ((!strcmp (argv[1], "init")) && argc == 4){
        	cmd.slic_type_id = strtol(argv[2], NULL, 16) & 0xFF;
        	cmd.interface_type_id = strtol(argv[3], NULL, 16) & 0xFF;
        	if (cmd.slic_type_id >= 0 && cmd.slic_type_id <= 2 && cmd.interface_type_id >= 0 && cmd.interface_type_id <= 3){
        		slic_ctrl_init(&cmd);
        	}
        	else{
        		spi_test_help();
        	}
        	return 0;
       }
       
       if ((!strcmp (argv[1], "test")) && argc == 3){
         	cmd.interface_type_id = strtol(argv[2], NULL, 16) & 0xFF;
         	if (cmd.interface_type_id == 0 || cmd.interface_type_id == 3){
         		slic_ctrl_pressure_test_lantiq(&cmd);
         	}
         	else{
         		spi_test_help();
         	}
         	return 0;
       }
	}

	if (argc < 7) {
		spi_test_help();
		return -1;
	}
	
	//printf ("[test]%s %s %s %s %s %s %s %s\n", argv[0], argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7]);
	cmd.slic_type_id = strtol(argv[1], NULL, 16) & 0xFF;
	cmd.interface_type_id = strtol(argv[2], NULL, 16) & 0xFF;
	cmd.slic_id = strtol(argv[3], NULL, 16) & 0xFF;
	cmd.ctrl = strtol(argv[4], NULL, 16) & 0xFF;
	cmd.cmd = strtol(argv[5], NULL, 16) & 0xFF;
	cmd.cmdLen = strtol(argv[6], NULL, 16) & 0xFF;

	printf ("[test]slic_type_id:%d, interface_type_id:%d, slic_id: %d, ctrl:%d, cmd:%d, cmdLen:%d\n", cmd.slic_type_id, cmd.interface_type_id, cmd.slic_id, cmd.ctrl, cmd.cmd, cmd.cmdLen);
	if (cmd.cmdLen > 28){
		printf ("length invalid, the max value is 28\n");
		return -1;
	}
		
	for (i = 0; (i< cmd.cmdLen) && (7+i < argc); i++)
	{
		cmd.data[i] = strtol(argv[7+i], NULL, 16) & 0xFF;
		printf (" 0x%x ", cmd.data[i]);
	}
	printf ("\n");
	
	ret = slic_ctrl_rw (&cmd);

#if 0
	ret = pressure_test ();
#endif		

#if 0
	ret = pressure_test2 ();
#endif	
	return ret;
}
