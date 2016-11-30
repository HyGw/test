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
************************************************************************/
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include "libapi_lib_pcie.h"
#include <ecnt_hook/ecnt_hook_pcie_type.h>
/************************************************************************
*                  D E F I N E S   &   C O N S T A N T S
************************************************************************/

/************************************************************************
*                  M A C R O S
************************************************************************/

/************************************************************************
*                  D A T A	 T Y P E S
************************************************************************/

/************************************************************************
*                  E X T E R N A L	 D A T A   D E C L A R A T I O N S
************************************************************************/

/************************************************************************
*                  F U N C T I O N   D E F I N I T I O N S
************************************************************************/

/************************************************************************
*                  P U B L I C   D A T A
************************************************************************/

/************************************************************************
*                  P R I V A T E   D A T A
************************************************************************/

/************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
************************************************************************/

static int pcie_lib_ioctl(int cmd, void *p)
{
	int fd, ret;
	struct ecnt_pcie_data *para = (struct ecnt_pcie_data*) p;;
	
	fd = open("/dev/pcie", O_RDWR);
    if (fd < 0) {
		printf("Open %s pseudo device failed\n","/dev/pcie");
		return -1;
    }
	
	para->function_id = cmd;
	
	ret = ioctl(fd, 0, p);
	
	close(fd);

	if(ret >= 0) 
		return 0;
	
	return -1;
}

int pcie_lib_get_confreg(int idx,uint offset,uint* value)
{
	int ret;
	struct ecnt_pcie_data para;

	memset(&para,0,sizeof(struct ecnt_pcie_data));
	para.idx = idx;
	para.conf.off = offset;
	ret = pcie_lib_ioctl(PCIE_GET_CONFREG,&para);
	*value = para.retValue;
	return ret >= 0 ? para.retValue : ret;

}

int pcie_lib_set_confreg(int idx,uint offset,uint value)
{
	int ret;
	struct ecnt_pcie_data para;

	memset(&para,0,sizeof(struct ecnt_pcie_data));
	para.idx = idx;
	para.conf.off = offset;
	para.conf.val = value;
	ret = pcie_lib_ioctl(PCIE_SET_CONFREG,&para);
	return ret >= 0 ? para.retValue : ret;
}

int pcie_lib_get_aspm(int idx,int* value)
{
	int ret;
	struct ecnt_pcie_data para;

	memset(&para,0,sizeof(struct ecnt_pcie_data));
	para.idx = idx;
	ret = pcie_lib_ioctl(PCIE_GET_ASPM,&para);
	*value = para.retValue;
	return ret >= 0 ? para.retValue : ret;

}

int pcie_lib_set_aspm(int idx,uint value)
{
	int ret;
	struct ecnt_pcie_data para;

	memset(&para,0,sizeof(struct ecnt_pcie_data));
	para.idx = idx;
	para.conf.val = value;
	ret = pcie_lib_ioctl(PCIE_SET_ASPM,&para);
	return ret >= 0 ? para.retValue : ret;

}

int pcie_lib_get_speed(int idx,int* value)
{
	int ret;
	struct ecnt_pcie_data para;

	memset(&para,0,sizeof(struct ecnt_pcie_data));
	para.idx = idx;
	ret = pcie_lib_ioctl(PCIE_GET_SPEED,&para);
	*value = para.retValue;
	return ret >= 0 ? para.retValue : ret;

}

int pcie_lib_set_speed(int idx,uint value)
{
	int ret;
	struct ecnt_pcie_data para;

	memset(&para,0,sizeof(struct ecnt_pcie_data));
	para.idx = idx;
	para.conf.val = value;
	ret = pcie_lib_ioctl(PCIE_SET_SPEED,&para);
	return ret >= 0 ? para.retValue : ret;

}

int pcie_lib_get_count(int idx,struct ecnt_pcie_count_data* pcnt)
{
	int ret;
	struct ecnt_pcie_data para;

	memset(&para,0,sizeof(struct ecnt_pcie_data));
	para.idx = idx;
	ret = pcie_lib_ioctl(PCIE_GET_COUNT,&para);
	memcpy(pcnt,&para.cnt,sizeof(struct ecnt_pcie_count_data));
	return ret >= 0 ? para.retValue : ret;
}


int pcie_lib_get_linkstate(int idx,int* value)
{
	int ret;
	struct ecnt_pcie_data para;

	memset(&para,0,sizeof(struct ecnt_pcie_data));
	para.idx = idx;
	ret = pcie_lib_ioctl(PCIE_GET_LINKSTATE,&para);
	*value = para.retValue;
	return ret >= 0 ? para.retValue : ret;

}

