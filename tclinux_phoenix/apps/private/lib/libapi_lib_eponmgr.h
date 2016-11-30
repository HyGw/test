
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
#ifndef _API_LIB_EPONMGR_H
#define _API_LIB_EPONMGR_H


/************************************************************************
*                  I N C L U D E S
*************************************************************************
*/


/************************************************************************
*                  D E F I N E S   &   C O N S T A N T S
*************************************************************************
*/
#define EPONMGR_SUCCESS	(0)
#define EPONMGR_FAIL	(-1)

#define EPON_MAC_DEV "/dev/epon_mac"
#define EPON_MAC_IOC_MAGIC 'j'
#define EPON_MAC_MAJOR 221

#define EPON_IOCTL_SET_DEBUG_LEVEL     	_IOW(EPON_MAC_IOC_MAGIC, 27,  unsigned char)
#ifdef TCSUPPORT_CPU_EN7521
#define EPON_IOCTL_SET_PHY_BURST_EN     	_IOW(EPON_MAC_IOC_MAGIC, 28,  unsigned int)
#endif
#define EPON_IOCTL_SET_DYING_GASP_MODE     	_IOW(EPON_MAC_IOC_MAGIC, 29,  unsigned char)
#define EPON_IOCTL_GET_DYING_GASP_MODE     	_IOW(EPON_MAC_IOC_MAGIC, 30,  unsigned char)
#define EPON_IOCTL_SET_DYING_GASP_NUM     	_IOW(EPON_MAC_IOC_MAGIC, 31,  unsigned int)
#define EPON_IOCTL_GET_DYING_GASP_NUM     	_IOW(EPON_MAC_IOC_MAGIC, 32,  unsigned int)
#ifdef TCSUPPORT_EPON_POWERSAVING
#define EPON_IOCTL_SET_EARLY_WAKEUP     	_IOW(EPON_MAC_IOC_MAGIC, 33,  unsigned int)
#endif
#define EPON_IOCTL_SET_SNIFFER_MODE     	_IOW(EPON_MAC_IOC_MAGIC, 34,  unsigned int)



/************************************************************************
*                  M A C R O S
*************************************************************************
*/

/************************************************************************
*                  D A T A	 T Y P E S
*************************************************************************
*/
typedef unsigned char		u_char;
typedef unsigned short		u_short;
typedef unsigned int		u_int;
typedef unsigned long		u_long;

typedef struct {
	u_char llidIdx;
	u_char param0;
	u_short param1;
	u_int param2;
	u_char info[128];
} eponMacIoctl_t ;


/************************************************************************
*                  D A T A   D E C L A R A T I O N S
*************************************************************************
*/

/************************************************************************
*                  F U N C T I O N   D E F I N I T I O N S
************************************************************************/
/* set */
int eponmgr_lib_set_dbg_level(u_char dbg_lvl);
#ifdef TCSUPPORT_CPU_EN7521
int eponmgr_lib_set_phy_burst_en(u_int mode);
#endif
int eponmgr_lib_set_dying_gasp_mode(u_char mode);
int eponmgr_lib_set_dying_gasp_num(u_int num);
#ifdef TCSUPPORT_EPON_POWERSAVING
int eponmgr_lib_set_early_wakeup(u_char enable,u_int timer,u_short count);
#endif
int eponmgr_lib_set_sniffer_mode(u_char enable,u_short mask);


/* get */
int eponmgr_lib_get_dying_gasp_mode(u_char *mode);
int eponmgr_lib_get_dying_gasp_num(u_int *num);


#endif
