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
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/fcntl.h>
#include <linux/sockios.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/types.h>



#include <7512ptm_api.h> /*MODULES*/
#include "libapi_lib_vdslmgr.h"





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

/******************************************************************************
 Descriptor:	It's used to ioctl to vdsl device.
 Input Args:	cmd: ioctl command.
                     p: pointer to vdsl driver structure, it is used to 
 Ret Value:	success: VDSLMGR_SUCCESS
                       fail: VDSLMGR_FAIL
******************************************************************************/
static int vdslmgr_lib_ioctl(int cmd, ptm_cmd_ioctl_t *data)
{
        int fd, ret;
        
        fd = open("/dev/ptm", O_RDWR);
        if (fd < 0) {
                printf("Open %s pseudo device failed\n","/dev/ptm");
                return VDSLMGR_FAIL;
        }

        ret = ioctl(fd, cmd, data);
        close(fd);

        if(ret >= 0) {
                return VDSLMGR_SUCCESS;
        } else {
                return VDSLMGR_FAIL;
        }
}



/******************************************************************************
 Descriptor:	It's used to get VDSL detail information.
 Input Args:	tcReg: pointer to struct PTMReg_t *p_ptmReg.
 Ret Value:	success: VDSLMGR_SUCCESS
                       fail:  VDSLMGR_FAIL
******************************************************************************/
int vdslmgr_lib_get_ptm_counter(PTMReg_t *p_ptmReg)
{
#if API_LIB_VDSLMGR_DBG
	printf("enter %s.\n", __func__);
#endif
        int ret;
        ptm_cmd_ioctl_t mydata;
        memset(&mydata, 0, sizeof(ptm_cmd_ioctl_t));
        mydata.function_id = PTM_IOS_GET_COUNTER;
        ret = vdslmgr_lib_ioctl(PTM_IOC_SET_CMD_OPT, &mydata);
        printf("BONDING_TXFRAG11  = 0x%08lx\n", mydata.udata._PTMReg._CTRL_REG);
        memcpy(p_ptmReg, &mydata.udata._PTMReg, sizeof(PTMReg_t));

        return ret;
}
/******************************************************************************
 Descriptor:	It's used to get VDSL-TC detail information.
 Input Args:	tcReg: pointer to struct TCReg_t *tcReg
 Ret Value:	success: VDSLMGR_SUCCESS
                       fail:  VDSLMGR_FAIL
******************************************************************************/
int vdslmgr_lib_get_tc_counter(TCReg_t *p_tcReg)
{
#if API_LIB_VDSLMGR_DBG
        printf("enter %s.\n", __func__);
#endif
        int ret;
        ptm_cmd_ioctl_t mydata;
        memset(&mydata, 0, sizeof(ptm_cmd_ioctl_t));
        mydata.function_id = PTM_TC_IOS_GET_COUNTER;
        ret = vdslmgr_lib_ioctl(PTM_IOC_SET_CMD_OPT, &mydata);
        memcpy(p_tcReg, &mydata.udata._TCReg, sizeof(TCReg_t));
        return ret;
}
/******************************************************************************
 Descriptor:	It's used to clean PTM  detail counter.
 Input Args:	
 Ret Value:	success: VDSLMGR_SUCCESS
                       fail:  VDSLMGR_FAIL
******************************************************************************/
int vdslmgr_lib_set_ptm_clean_counter()
{
#if API_LIB_VDSLMGR_DBG
	printf("enter %s.\n", __func__);
#endif
        int ret;
        ptm_cmd_ioctl_t mydata;
        memset(&mydata, 0, sizeof(ptm_cmd_ioctl_t));
        mydata.function_id = PTM_IOS_CLEAN_COUNTER;
        ret = vdslmgr_lib_ioctl(PTM_IOC_SET_CMD_OPT, &mydata);
        return ret;
}
/******************************************************************************
 Descriptor:	It's used to reset ptm mac.
 Input Args:	
 Ret Value:	success: VDSLMGR_SUCCESS
                       fail:  VDSLMGR_FAIL
******************************************************************************/
int vdslmgr_lib_set_ptm_reset()
{
#if API_LIB_VDSLMGR_DBG
        printf("enter %s.\n", __func__);
#endif
        int ret;
        ptm_cmd_ioctl_t mydata;
        memset(&mydata, 0, sizeof(ptm_cmd_ioctl_t));
        mydata.function_id = PTM_IOS_SET_RESET;
        ret = vdslmgr_lib_ioctl(PTM_IOC_SET_CMD_OPT, &mydata);
        return ret;
}
/******************************************************************************
 Descriptor:    It's used to set ptm debug msg level.
 Input Args:    
 Ret Value:     success: VDSLMGR_SUCCESS
                fail:  VDSLMGR_FAIL
******************************************************************************/
int vdslmgr_lib_set_ptm_msglevel(ushort _leve)
{
#if API_LIB_VDSLMGR_DBG
	printf("enter %s.\n", __func__);
#endif
        int ret;
        ptm_cmd_ioctl_t mydata;
        memset(&mydata, 0, sizeof(ptm_cmd_ioctl_t));
        mydata.function_id = PTM_IOS_SET_MSGLEVEL;
        mydata.udata._input = _leve;
        ret = vdslmgr_lib_ioctl(PTM_IOC_SET_CMD_OPT, &mydata);
        return ret;
}

/******************************************************************************************
     ********   *******    **    **   *******    ******   **   **    ******
     **   **   **      **  ***   **   **    **     ***    ***  **   **
     *******   **      **  ** *  **   **    **     ***    ** * **   **   ****
     **   **   **      **  **  * **   **    **     ***    **  ***   **    ** 
     ********   *******    **   ***   *******    ******   **   **    *******
*******************************************************************************************/
#ifdef TCSUPPORT_BONDING
/******************************************************************************
 Descriptor:    It's used to set bonding flow control [enable|disable].
 Input Args:    master:[bfb6f000]:bit 0
 Ret Value:     success: VDSLMGR_SUCCESS
                fail:  VDSLMGR_FAIL
******************************************************************************/
int vdslmgr_lib_set_flowctl(ushort _enable)
{
#if API_LIB_VDSLMGR_DBG
                printf("enter %s.\n", __func__);
#endif
        int ret;
        ptm_cmd_ioctl_t mydata;
        memset(&mydata, 0, sizeof(ptm_cmd_ioctl_t));
        mydata.function_id = BONDING_IOS_SET_FLOWCTL;
        mydata.udata._input = _enable;
        ret = vdslmgr_lib_ioctl(BONDING_IOC_SET_CMD_OPT, &mydata);
        return ret;
}
/******************************************************************************
 Descriptor:	It's used to reset bonding mac.
 Input Args:	
 Ret Value:	success: VDSLMGR_SUCCESS
                       fail:  VDSLMGR_FAIL
******************************************************************************/
int vdslmgr_lib_set_bonding_reset()
{
#if API_LIB_VDSLMGR_DBG
	printf("enter %s.\n", __func__);
#endif
        int ret;
        ptm_cmd_ioctl_t mydata;
        memset(&mydata, 0, sizeof(ptm_cmd_ioctl_t));
        mydata.function_id = BONDING_IOS_SET_RESET;
        ret = vdslmgr_lib_ioctl(BONDING_IOC_SET_CMD_OPT, &mydata);;
        return ret;
}
/******************************************************************************
 Descriptor:	It's used to set bonding fragment.
 Input Args:	
 Ret Value:	success: VDSLMGR_SUCCESS
                       fail:  VDSLMGR_FAIL
******************************************************************************/
int vdslmgr_lib_set_fragment(ushort _enable)
{
#if API_LIB_VDSLMGR_DBG
        printf("enter %s.\n", __func__);
#endif
        int ret;
        ptm_cmd_ioctl_t mydata;
        memset(&mydata, 0, sizeof(ptm_cmd_ioctl_t));
        mydata.function_id = BONDING_IOS_SET_FRAGMENT;
        mydata.udata._input = _enable;
        ret = vdslmgr_lib_ioctl(BONDING_IOC_SET_CMD_OPT, &mydata);
        return ret;
}
/******************************************************************************
 Descriptor:	It's used to set bonding line switch.
 Input Args:	
 Ret Value:	success: VDSLMGR_SUCCESS
                       fail:  VDSLMGR_FAIL
******************************************************************************/
int vdslmgr_lib_set_line_path(ushort _mode)
{
#if API_LIB_VDSLMGR_DBG
        printf("enter %s.\n", __func__);
#endif
        int ret;
        ptm_cmd_ioctl_t mydata;
        memset(&mydata, 0, sizeof(ptm_cmd_ioctl_t));
        mydata.function_id = BONDING_IOS_SET_LINEPATH;
        mydata.udata._input = _mode;
        ret = vdslmgr_lib_ioctl(BONDING_IOC_SET_CMD_OPT, &mydata);
        return ret;
}

/******************************************************************************
 Descriptor:	It's used to get bonding flow control [enable|disable].
 Input Args:	
 Ret Value:	success: VDSLMGR_SUCCESS
                       fail:  VDSLMGR_FAIL
******************************************************************************/
int vdslmgr_lib_get_flowctl(ushort mode, ushort *_enable)
{
#if API_LIB_VDSLMGR_DBG
        printf("enter %s.\n", __func__);
#endif
        int ret;
        ptm_cmd_ioctl_t mydata;
        memset(&mydata, 0, sizeof(ptm_cmd_ioctl_t));
        mydata.function_id = BONDING_IOS_GET_FLOWCTL;
        mydata.udata._input = mode;
        ret = vdslmgr_lib_ioctl(BONDING_IOC_SET_CMD_OPT, &mydata);
        memcpy(_enable, &mydata.udata._output, sizeof(ushort));
        return ret;
}
/******************************************************************************
 Descriptor:	It's used to get bonding fragment.
 Input Args:	
 Ret Value:	success: VDSLMGR_SUCCESS
                       fail:  VDSLMGR_FAIL
******************************************************************************/
int vdslmgr_lib_get_fragment(ushort *_enable)
{
#if API_LIB_VDSLMGR_DBG
        printf("enter %s.\n", __func__);
#endif
        int ret;
        ptm_cmd_ioctl_t mydata;
        memset(&mydata, 0, sizeof(ptm_cmd_ioctl_t));
        mydata.function_id = BONDING_IOS_GET_FRAGMENT;
        ret = vdslmgr_lib_ioctl(BONDING_IOC_SET_CMD_OPT, &mydata);
        memcpy(_enable, &mydata.udata._output, sizeof(ushort));
        return ret;
}
/******************************************************************************
 Descriptor:	It's used to get bonding line path.
 Input Args:	
 Ret Value:	success: VDSLMGR_SUCCESS
                       fail:  VDSLMGR_FAIL
******************************************************************************/
int vdslmgr_lib_get_linepath(ushort *_mode)
{
#if API_LIB_VDSLMGR_DBG
                printf("enter %s.\n", __func__);
#endif
                int ret;
                ptm_cmd_ioctl_t mydata;
                memset(&mydata, 0, sizeof(ptm_cmd_ioctl_t));
                mydata.function_id = BONDING_IOS_GET_LINEPATH;
                mydata.udata._input = 0;
                ret = vdslmgr_lib_ioctl(BONDING_IOC_SET_CMD_OPT, &mydata);
                memcpy(_mode, &mydata.udata._output, sizeof(ushort));
                return ret;
}
/******************************************************************************
 Descriptor:	It's used to get bonding counter.
 Input Args:	
 Ret Value:	success: VDSLMGR_SUCCESS
                       fail:  VDSLMGR_FAIL
******************************************************************************/
int vdslmgr_lib_get_bonding_counter(BondingReg_t *_BondingReg)
{
#if API_LIB_VDSLMGR_DBG
                printf("enter %s.\n", __func__);
#endif
                int ret;
                ptm_cmd_ioctl_t mydata;
                memset(&mydata, 0, sizeof(ptm_cmd_ioctl_t));
                mydata.function_id = BONDING_IOS_GET_COUNTER;
                ret = vdslmgr_lib_ioctl(BONDING_IOC_SET_CMD_OPT, &mydata);
                memcpy(_BondingReg, &mydata.udata._BondingReg, sizeof(BondingReg_t));
                return ret;
}



#endif /*TCSUPPORT_BONDING*/


