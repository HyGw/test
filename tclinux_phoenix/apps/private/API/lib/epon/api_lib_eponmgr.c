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
#include <sys/ioctl.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <asm/tc3162/tc3162.h>

#include "libapi_lib_eponmgr.h"

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
int eponmgr_lib_ioctl(int cmd, void *data);


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
int eponmgr_lib_ioctl(int cmd, void *data)
{
	int fp = -1;
	int ret = -1;

	fp = open(EPON_MAC_DEV, O_RDONLY);
	if (fp == -1){
		perror("eponmgr  IOCTL file open failed!\n");
		return EPONMGR_FAIL;
	}

	if(data == NULL){
		close(fp);
		perror("eponmgr  data == NULL!\n");
		return EPONMGR_FAIL;
	}
	
	ret = ioctl(fp, cmd, data);
	if (ret < 0){
		perror("eponmgr  IOCTL error!\n");
		close(fp);
		return EPONMGR_FAIL;
	}
	close(fp);
	
	return EPONMGR_SUCCESS;
}

int eponmgr_lib_set_dbg_level(u_char dbg_lvl)
{
	eponMacIoctl_t data;
	memset(&data , 0 , sizeof(data));

	data.param0 = dbg_lvl;
	return eponmgr_lib_ioctl(EPON_IOCTL_SET_DEBUG_LEVEL, &data);
}

#ifdef TCSUPPORT_CPU_EN7521
int eponmgr_lib_set_phy_burst_en(u_int mode)
{
	eponMacIoctl_t data;
	memset(&data , 0 , sizeof(data));

	data.param2 = mode;
	return eponmgr_lib_ioctl(EPON_IOCTL_SET_PHY_BURST_EN, &data);
}
#endif

int eponmgr_lib_set_dying_gasp_mode(u_char mode)
{
	eponMacIoctl_t data;
	memset(&data , 0 , sizeof(data));

	data.param0 = mode;
	return eponmgr_lib_ioctl(EPON_IOCTL_SET_DYING_GASP_MODE, &data);
}

int eponmgr_lib_set_dying_gasp_num(u_int num)
{
	eponMacIoctl_t data;
	memset(&data , 0 , sizeof(data));

	data.param2 = num;
	return eponmgr_lib_ioctl(EPON_IOCTL_SET_DYING_GASP_NUM, &data);
}

int eponmgr_lib_get_dying_gasp_mode(u_char *mode)
{
	eponMacIoctl_t data;
	memset(&data , 0 , sizeof(data));

	eponmgr_lib_ioctl(EPON_IOCTL_GET_DYING_GASP_MODE, &data);
	*mode = data.param0;

	return EPONMGR_SUCCESS;
}

int eponmgr_lib_get_dying_gasp_num(u_int *num)
{
	eponMacIoctl_t data;
	memset(&data , 0 , sizeof(data));

	eponmgr_lib_ioctl(EPON_IOCTL_GET_DYING_GASP_NUM, &data);
	*num = data.param2;

	return EPONMGR_SUCCESS;
}

#ifdef TCSUPPORT_EPON_POWERSAVING
int eponmgr_lib_set_early_wakeup(u_char enable,u_int timer,u_short count)
{
	eponMacIoctl_t data;
	memset(&data , 0 , sizeof(data));

	data.param0 = enable;
	data.param2 = timer;
	data.param1 = count;

	return eponmgr_lib_ioctl(EPON_IOCTL_SET_EARLY_WAKEUP, &data);
}
#endif

int eponmgr_lib_set_sniffer_mode(u_char enable,u_short mask)
{
	eponMacIoctl_t data;
	
	memset(&data , 0 , sizeof(data));

	data.param0 = enable;
	data.param1 = mask;

	return eponmgr_lib_ioctl(EPON_IOCTL_SET_SNIFFER_MODE, &data);
}


