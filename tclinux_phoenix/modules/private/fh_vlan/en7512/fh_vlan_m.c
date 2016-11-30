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
#include <linux/module.h>
#include <linux/kernel.h>
#include "tc_vlanfilter.h"
#include "tc_vlan_trans.h"


/************************************************************************
*                  D E F I N E S   &   C O N S T A N T S
*************************************************************************
*/
#define FHEWAN_RX_HOOK 0
#define FHEWAN_TX_HOOK 1

/************************************************************************
*                  M A C R O S
*************************************************************************
*/

/************************************************************************
*                  D A T A   T Y P E S
*************************************************************************
*/

/************************************************************************
*                  E X T E R N A L   D A T A   D E C L A R A T I O N S
*************************************************************************
*/
extern int (*fh_ewan_vlan_hook)(struct sk_buff* skb, int type);
extern int (*mtk_multicast_data_handle_hook)(struct sk_buff* skb);

/************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
************************************************************************/


/************************************************************************
*                  P U B L I C   D A T A
*************************************************************************/

/************************************************************************
*                  P R I V A T E   D A T A
*************************************************************************
*/


/************************************************************************
*                  F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/
int fh_ewan_vlan_handle(struct sk_buff* skb, int type)
{
	if ( FHEWAN_RX_HOOK == type )
	{
		if ( 0 != vlan_filter_handle(skb) )
		{
			return 1;
		}

		/* rx data packet do vlan proc after Add to HW_NAT */
		if ( -1 == fb_ewan_rx_vlan_proc(skb) )
		{
			return 1;
		}

		if ( mtk_multicast_data_handle_hook )
		{
			mtk_multicast_data_handle_hook(skb);
		}
	}
	else if ( FHEWAN_TX_HOOK == type )
	{
		fb_ewan_tx_vlan_proc(skb);
	}

	return 0;
}

static void fh_vlan_exit(void)
{
	vlan_filter_exit();
	vlan_transfer_exit();

	rcu_assign_pointer(fh_ewan_vlan_hook, NULL);
}

static int fh_vlan_init(void)
{
	int err = -1;

  	err = vlan_filter_init();
	if( 0 != err )
	{
		printk("FHT vlan filter initialization failed\n") ;
		goto fh_vlan_init_err;
	}

  	err = vlan_transfer_init();
	if( 0 != err )
	{
		printk("FHT vlan transfer initialization failed\n") ;
		goto fh_vlan_init_err;
	}

	rcu_assign_pointer(fh_ewan_vlan_hook, fh_ewan_vlan_handle);

	return 0;

fh_vlan_init_err:
	fh_vlan_exit() ;
	return -EFAULT ;
}



/* Register startup/shutdown routines */
module_init(fh_vlan_init);
module_exit(fh_vlan_exit);

MODULE_LICENSE("Proprietary");

