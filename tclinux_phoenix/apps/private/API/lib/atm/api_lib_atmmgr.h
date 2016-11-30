
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
#ifndef _ATM_LIB_H_
#define _ATM_LIB_H_


/************************************************************************
*                  I N C L U D E S
*************************************************************************
*/


/************************************************************************
*                  D E F I N E S   &   C O N S T A N T S
*************************************************************************
*/
#define ATMCMD_IOC_MAGIC 'p'
#define ATMCMD_IOC_SET_CMD_OPT    _IOW(ATMCMD_IOC_MAGIC,0,unsigned char)
#define SET_MSG_LVL  0
#define RESET_MAC    1
#define CLR_CNT      2
#define DUMP_REG     3
#define GET_STATE    4
#define GET_CNT      5

   
/************************************************************************
*                  M A C R O S
*************************************************************************
*/

/************************************************************************
*                  D A T A	 T Y P E S
*************************************************************************
*/
#ifndef UINT8
#define UINT8
    typedef unsigned char uint8;            /* 8-bit unsigned integer       */
#endif


typedef struct atm_cmd_ioctl_s
{
    /*which cmd,msglvl or reset....*/
    int function_id;
    /*lvl1:rx error msg; lvl2:tx msg and data lvl3:rx msg and data*/
    uint8 dbg_lvl;
    /*dump link state of current vc_no*/
    uint8 vc_no;
}atmcmd_ioctl;



/************************************************************************
*                  D A T A   D E C L A R A T I O N S
*************************************************************************
*/


/************************************************************************
*                  F U N C T I O N    D E C L A R A T I O N S
#                  I N L I N E    F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/
int adslmgr_lib_atm_set_msglvl(unsigned char dbg_lvl);
int adslmgr_lib_atm_get_linkstate(unsigned char channel_no); 
int adslmgr_lib_atm_set_reset(void);   
int adslmgr_lib_atm_set_clrcnt(void);  
int adslmgr_lib_atm_get_reg(void);   
int adslmgr_lib_atm_get_counter(void);

#endif

