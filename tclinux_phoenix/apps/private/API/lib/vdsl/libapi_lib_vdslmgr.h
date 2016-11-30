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
#ifndef _API_LIB_VDSLMGR_H
#define _API_LIB_VDSLMGR_H
#include <net/if.h>
#include <linux/types.h>
#include <7512ptm_api.h> /*MODULES*/


/************************************************************************
*                  D A T A	 T Y P E S
************************************************************************/
typedef unsigned char			unchar;
typedef unsigned short			ushort;
typedef unsigned int			uint;
typedef unsigned long			ulong;


/************************************************************************
*                  I N C L U D E S
************************************************************************/


/************************************************************************
*                  D E F I N E S   &   C O N S T A N T S
************************************************************************/
#define VDSLMGR_SUCCESS	(0)
#define VDSLMGR_FAIL	(-1)

/************************************************************************
*                  M A C R O S
************************************************************************/


/************************************************************************
*                  E X T E R N A L	 D A T A   D E C L A R A T I O N S
************************************************************************/


/************************************************************************
*                  F U N C T I O N   D E F I N I T I O N S
************************************************************************/
/* set */
int vdslmgr_lib_set_ptm_clean_counter();
int vdslmgr_lib_set_ptm_reset();
int vdslmgr_lib_set_ptm_msglevel(ushort _level);

/* get */
int vdslmgr_lib_get_ptm_counter(PTMReg_t *p_ptmReg);
int vdslmgr_lib_get_tc_counter(TCReg_t *tcReg);

#ifdef TCSUPPORT_BONDING
int vdslmgr_lib_set_flowctl(ushort _enable);
int vdslmgr_lib_set_bonding_reset();
int vdslmgr_lib_set_fragment(ushort _enable);
int vdslmgr_lib_set_line_path(ushort _mode);

int vdslmgr_lib_get_flowctl(ushort mode, ushort *_enable);
int vdslmgr_lib_get_fragment(ushort *_enable);
int vdslmgr_lib_get_linepath(ushort *_mode);
int vdslmgr_lib_get_bonding_counter(BondingReg_t *_BondingReg);


#endif

/************************************************************************
*                  P U B L I C   D A T A
************************************************************************/

/************************************************************************
*                  P R I V A T E   D A T A
************************************************************************/

/************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
************************************************************************/

#endif
