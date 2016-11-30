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
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <fcntl.h>

#include "api_lib_atmmgr.h"

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
int atm_cmd_ioctl(atmcmd_ioctl *data, int datalen);

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
int atm_cmd_ioctl(atmcmd_ioctl *data, int datalen)
{
   	int fd = -1;
	int ret = -1;	
	fd = open("/dev/atm", O_RDWR);
	if(fd == -1)
	{
		printf("\r\natmdbgcmd open ioctl file fail\r\n");
        return -1;
    }
	ret = ioctl(fd, ATMCMD_IOC_SET_CMD_OPT, data);
	if(ret < 0)
	{
		printf("\r\natmdbgcmd ioctl error\r\n");        
        close(fd);
        return -1;
	}
	close(fd);
	return 0; 
}

int adslmgr_lib_atm_set_msglvl(unsigned char dbg_lvl)
{
	atmcmd_ioctl data;

	memset(&data, 0, sizeof(data));
    data.function_id = SET_MSG_LVL;
	data.dbg_lvl = dbg_lvl;
	
	return atm_cmd_ioctl(&data,sizeof(data));
}

int adslmgr_lib_atm_set_reset(void)
{		
    atmcmd_ioctl data;
    data.function_id = RESET_MAC;
	return atm_cmd_ioctl(&data,sizeof(data));
}

int adslmgr_lib_atm_set_clrcnt(void)
{
    atmcmd_ioctl data;
    data.function_id = CLR_CNT;
	return atm_cmd_ioctl(&data,sizeof(data));
}

int adslmgr_lib_atm_get_reg(void)
{	
    atmcmd_ioctl data;
    data.function_id = DUMP_REG;
	return atm_cmd_ioctl(&data,sizeof(data));
}

int adslmgr_lib_atm_get_linkstate(unsigned char channel_no)
{
	atmcmd_ioctl data;

	memset(&data, 0, sizeof(data));
    data.function_id = GET_STATE;
	data.vc_no= channel_no;
   
	return atm_cmd_ioctl(&data,sizeof(data));
}

int adslmgr_lib_atm_get_counter(void)
{
	atmcmd_ioctl data;
    data.function_id = GET_CNT; 
	return atm_cmd_ioctl(&data,sizeof(data));
}




