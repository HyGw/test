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
#include "tc_vlan_trans.h"
#include "tc_l2lu.h"

#include <linux/capability.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/times.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/if_vlan.h>
#include <linux/if_ether.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/wait.h>
#include <linux/init.h>

/************************************************************************
*                  D E F I N E S   &   C O N S T A N T S
*************************************************************************
*/
#define FH_VLAN_MSG( F, B...) { \
		if(debugLevel) { \
			printk(F , ##B) ; \
		} \
	}

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
static int debugLevel = 0;
FHVLAN_GlbPriv_T *gpWanPriv = NULL ;
static struct proc_dir_entry *gfh_vlan_proc_dir = NULL;
static int vlan_trans_major = COSNT_FH_VLAN_MAJOR_NUM;

/************************************************************************
*                  F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/
static int fh_vlan_debug_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
	int index=0 ;

	index = sprintf(buf, "Debug Level: %x\n", debugLevel);

	index -= off;
	*start = buf + off;

	if (index > count)
		index = count;
	else
		*eof = 1;

	if (index < 0)
		index = 0;

	return index;
}

static int fh_vlan_debug_write_proc
(struct file *file, const char *buffer, unsigned long count, void *data)
{
	char val_string[64], cmd[32], subcmd[32] ;
	uint action ;
	int ret;
	
	if (count > sizeof(val_string) - 1)
		return -EINVAL ;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT ;

	sscanf(val_string, "%s %s %x", cmd, subcmd, &action) ;

	if ( !strcmp(cmd, "vlan_trans") )
	{
		uint vlan1,vlan2,vlanCnt;

		if ( !strcmp(subcmd, "show") )
		{
			showTransVlanInfo();
			return count;
		}

		if ( !strcmp(subcmd, "clear") )
		{
			memset(&gpWanPriv->fhVlan,0,sizeof(gpWanPriv->fhVlan));
			printk("clear all vlan list\n");
			showTransVlanInfo();
			return count;
		}

		sscanf(val_string, "%s %d %d\n", cmd, &vlan1, &vlan2) ;
		vlanCnt = gpWanPriv->fhVlan.vlanCount;
		if ( vlanCnt == 8 )
			return count;
		gpWanPriv->fhVlan.vlan[vlanCnt].srcVlan = vlan1;
		gpWanPriv->fhVlan.vlan[vlanCnt].dstVlan = vlan2;
		gpWanPriv->fhVlan.vlanCount++;
		showTransVlanInfo();
	}
	else if( !strcmp(cmd, "msg") )
	{
		if ( !strcmp(subcmd, "debug") )
			debugLevel = action;
		printk("Debug Level: %x\n", debugLevel) ; 
	}

	return count ;
}

static int getVlanType(unsigned short TPID)
{
	switch(TPID)
	{
		case ETH_P_8021Q:
			return ETH_P_8021Q;
		case 0x88a8:
			return ETH_P_8021Q;
		case 0x9100:
			return ETH_P_8021Q;
		case 0xAABB:      /*only for test*/
			return ETH_P_8021Q;
		default:
			return -1;
	}
}

static void __dump_skb(struct sk_buff *skb, uint pktLen) 
{
	unchar n, *p = (unchar *)skb->data ;
	int i ;

	for(i=0 ; i<pktLen ; i++) {
		n = i & 0x0f ;
		
		if(n == 0x00) 		printk(" \n%.4x: ", i) ;
		else if(n == 0x08) 	printk(" ") ;

		printk("%.2x ", *p++) ; 
	}
	printk("\n") ;
}


int fb_ewan_rx_vlan_proc(struct sk_buff *skb)
{
	u16 vid = 0;
	u16 pbit = 0;
	unsigned char *dp = NULL;
	__be16 h_proto;
	int vlan_tci = 0;
	struct vlan_ethhdr* veth;
 	 int i =0;

	if(NULL == skb || NULL == skb->data)
	{
		printk("skb or data is null\n");
		return -1;
	}
	
	skb_push(skb,14);
	
	dp = skb->data;
	dp +=  ETH_ALEN+ETH_ALEN;
	h_proto = *(__be16*)dp;
	
	if ( debugLevel )
	{
		FH_VLAN_MSG("fb RX vlan proc before vlan proc \n") ;
		__dump_skb(skb, skb->len) ;
	}
	
	/* vlan proc */
	if ( getVlanType(h_proto) == -1 )
	{
		/* untag socket proc, add vlan 4093 */
		FH_VLAN_MSG("fb_ewan_rx_vlan_proc untag message, add vlan 4093\n");
		vlan_tci = setVID(FB_SPECIAL_VLAN);
		__vlan_put_tag(skb,vlan_tci);
	}
	else
	{	
		if(0 == gpWanPriv->fhVlan.vlanCount)
		{
			FH_VLAN_MSG("there is no vlan list config return\n");
			skb_pull(skb,14);
			return 0;
		}
		/* tag socket proc, replace vlan1 to vlan2 */
		vid = getVID(*(__be16*)(dp+2));
		pbit = (*(__be16*)(dp+2)) & 0xE000;
		FH_VLAN_MSG("fb_ewan_rx_vlan_proc tag message, vlan is %d\n"
			,vid);
		for(i = 0;i < gpWanPriv->fhVlan.vlanCount;i++)
		{
			if(vid == gpWanPriv->fhVlan.vlan[i].srcVlan)
			{
				FH_VLAN_MSG("fb_ewan_rx_vlan_proc tag message, vlan is %d"
					" should trans to %d\n"
					,vid,gpWanPriv->fhVlan.vlan[i].dstVlan);
				vlan_tci = setVID(gpWanPriv->fhVlan.vlan[i].dstVlan)  | pbit;
				veth = (struct vlan_ethhdr *)skb->data;
				veth->h_vlan_TCI = htons(vlan_tci);	
				break;
			}
		}
	}

	if ( debugLevel )
	{
		FH_VLAN_MSG("fb RX vlan proc after vlan proc \n") ;
		__dump_skb(skb, skb->len) ;
	}
	skb_pull(skb,14);

	return 0;
}


int fb_ewan_tx_vlan_proc(struct sk_buff *skb)
{
	u16 vid = 0;
	unsigned char *dp = NULL;
	__be16 h_proto;
	int vlan_tci = 0;

	if(NULL == skb || NULL == skb->data)
	{
		printk("skb or skb->data is null\n");
		return -1;
	}

	dp = skb->data;
	dp +=  ETH_ALEN+ETH_ALEN;
	h_proto = *(__be16*)dp;

	skb_reset_mac_header(skb);

	if ( debugLevel )
	{
		FH_VLAN_MSG("fb_ewan_tx_vlan_proc before vlan proc \n") ;
		__dump_skb(skb, skb->len) ;
	}

	/* vlan proc */
	if(getVlanType(h_proto) == -1)
	{
		FH_VLAN_MSG("fb_ewan_tx_vlan_proc untag message \n");
		return 0;
	}
	else
	{
		vid = getVID(*(__be16*)(dp+2));
		FH_VLAN_MSG("fb_ewan_tx_vlan_proc tag message, vid is %d\n"
			, vid);
		if(FB_SPECIAL_VLAN == vid)
		{
			/*vlan is 4093, remove the tag */
			FH_VLAN_MSG("fb_ewan_tx_vlan_proc tag message,"
			" need to remove tag:%d\n"
			, vid);
			memmove(skb->mac_header + VLAN_HEAD_LEN, skb->mac_header, 12);
			skb->mac_header = skb->mac_header + VLAN_HEAD_LEN;
			skb_pull(skb, VLAN_HEAD_LEN);
		}
	}

	if( debugLevel )
	{
		FH_VLAN_MSG("fb_ewan_tx_vlan_proc after vlan proc \n") ;
		__dump_skb(skb, skb->len) ;
	}

	return 0;
}

static int fhvlan_add_vlan_to_list(unsigned short vlan1, unsigned short vlan2)
{
	int i = 0;
	struct FH_VLAN_CFG_INFO_S *vlanList = &gpWanPriv->fhVlan;
	unsigned char vlanCnt = vlanList->vlanCount;

	if(vlanCnt >= FH_MAX_VLAN_NUMBER)
	{
		printk("vlan has reached the max number, please delete first\n");
		return FH_RET_VLAN_REACH_MAX;
	}

	for(i = 0; i < vlanCnt; i++)
	{
		if(vlan1 == vlanList->vlan[i].srcVlan)
		{
			printk("source vlan has exist, replace destation %d to vlan %d\n",
				vlanList->vlan[i].dstVlan,vlan2);
			vlanList->vlan[i].dstVlan = vlan2;
			return FH_RET_SUCCESS;
		}
	}

	vlanList->vlan[vlanCnt].srcVlan = vlan1;
	vlanList->vlan[vlanCnt].dstVlan = vlan2;
	vlanList->vlanCount++;

	return FH_RET_SUCCESS;
}

static int fhvlan_del_vlan_from_list(unsigned short vlan1, unsigned short vlan2)
{
	int i = 0;
	struct FH_VLAN_CFG_INFO_S *vlanList = &gpWanPriv->fhVlan;
	unsigned char vlanCnt = vlanList->vlanCount;

	if(vlanCnt == 0)
	{
		printk("There is no vlan in list\n");
		return FH_RET_VLAN_CNT_ZERO;
	}

	for(i = 0; i < vlanCnt; i++)
	{
		if((vlan1 == vlanList->vlan[i].srcVlan) && (vlan2 == vlanList->vlan[i].dstVlan))
		{
			printk("vlan found success, delete\n");
			/* move last vlan to current vlan, delete last vlan, counter-- */
			vlanList->vlan[i].srcVlan = vlanList->vlan[vlanCnt - 1].srcVlan;
			vlanList->vlan[i].dstVlan = vlanList->vlan[vlanCnt - 1].dstVlan;
			
			vlanList->vlan[vlanCnt - 1].srcVlan = 0;
			vlanList->vlan[vlanCnt - 1].dstVlan = 0;
			vlanList->vlanCount--;
			return FH_RET_SUCCESS;
		}
	}
	printk("target vlan need to delete dost not exist\n");
	return FH_RET_VLAN_NOT_EXIST;
}

void showTransVlanInfo(void)
{
	int i = 0;
	printk("Kernel Total vlan list Number is %d\n", gpWanPriv->fhVlan.vlanCount);
	printk("NO    source Vlan           target vlan\n");
	printk("----------------------------------------\n");
	for(i = 0;i < gpWanPriv->fhVlan.vlanCount;i++)
	{
		printk("%d       %4d                  %4d\n",i+1,
			gpWanPriv->fhVlan.vlan[i].srcVlan,gpWanPriv->fhVlan.vlan[i].dstVlan);
	}
	printk("\n\n");
}

int fhvlan_get_vlan_info(struct FH_VLAN_CFG_INFO_S *pVlaninfo)
{
	if(NULL == pVlaninfo)
	{
		return -1;
	}

	if ( copy_to_user(pVlaninfo, &gpWanPriv->fhVlan,
		sizeof(struct FH_VLAN_CFG_INFO_S)) )
	{
		printk("xmcs_get_vlan_info copy_to_user fail\n");
		return -1;
	}

	return 0;
}

int fhvlan_set_vlan_info(struct FH_VLAN_CFG_OPERATE_S *pVlaninfoApp)
{
	int ret = 0;
	struct FH_VLAN_CFG_OPERATE_S pVlaninfo = {0};
	
	if(NULL == pVlaninfoApp)
	{
		return -1;
	}

	if(copy_from_user(&pVlaninfo, pVlaninfoApp, sizeof(struct FH_VLAN_CFG_OPERATE_S)))
	{
		printk("xmcs_set_vlan_info copy_from_user fail\n");
		return -1;
	}


	switch (pVlaninfo.vlanOperate)
	{
		case FH_MAX_VLAN_OPT_ADD:
			printk("kernel rcv app add vlan trans cfg \n");
			ret = fhvlan_add_vlan_to_list(pVlaninfo.vlan.srcVlan,pVlaninfo.vlan.dstVlan);
			break;
		case FH_MAX_VLAN_OPT_DEL:
			printk("kernel rcv app delete vlan trans cfg \n");
			ret = fhvlan_del_vlan_from_list(pVlaninfo.vlan.srcVlan,pVlaninfo.vlan.dstVlan);
			break;
		default:
			printk("kernel rcv app vlan trans cfg operate unknow \n");
			break;
	}
	if(FH_RET_SUCCESS != ret)
	{
		printk("xmcs_set_vlan_info fail \n");
		return ret;
	}
	showTransVlanInfo();
	return 0;
}

static int if_fh_cmd_proc(uint cmd, ulong arg) 
{
	int ret = -EINVAL ;
	
	if(_IOC_DIR(cmd) == _IOC_WRITE)
	{
		switch ( cmd )
		{
			case FH_IF_IOS_VLAN_CONFIG_SET :
				ret = fhvlan_set_vlan_info((struct FH_VLAN_CFG_OPERATE_S *)arg) ;
				break;
			case FH_IF_IOG_VLAN_INFO_GET:
				ret = fhvlan_get_vlan_info((struct FH_VLAN_CFG_INFO_S *)arg) ;
				break;
			default:
				FH_VLAN_MSG("No such I/O command, cmd: %x\n", cmd) ;
				break ;
		}
	} 
	else
	{
		FH_VLAN_MSG("No such I/O command, cmd: %x\n", cmd) ;
	}

	return ret ;
}

long fh_vlan_iface_ioctl(struct file *filp, uint cmd, ulong arg) 
{
	int ret = -EINVAL ;

	if ( _IOC_TYPE(cmd) == INTERFACE_MAGIC )
	{
		ret = if_fh_cmd_proc(cmd, arg) ;
	}

	return ret ;
}

/***************************************************************
***************************************************************/
struct file_operations fh_vlan_fops = {
	.owner 			= THIS_MODULE,
	.unlocked_ioctl	= fh_vlan_iface_ioctl,
};



/***************************************************************
***************************************************************/
void fhvlan_dev_destroy(void) 
{   
	unregister_chrdev(vlan_trans_major, CONST_FH_VLAN_DEV_NAME);
	/*printk("FHVLAN interface destroy successful\n") ;*/
}

/***************************************************************
***************************************************************/
int fhvlan_dev_init(void)
{
	int result = 0 ;

    result = register_chrdev(vlan_trans_major
    , CONST_FH_VLAN_DEV_NAME, &fh_vlan_fops);
    if ( result < 0 )
	{
		printk("vlan_trans: can't get major %d\n",vlan_trans_major);
        return result;
    }

	/*printk("FH VLAN interface initialization successful\n") ;*/
	return 0 ; 
out:
	fhvlan_dev_destroy() ;
	return result ;
}



static void vlan_transfer_init_global_data(void)
{
	debugLevel = 0;
}

int vlan_transfer_exit(void)
{
    if ( gfh_vlan_proc_dir )
	{
        remove_proc_entry("debug", gfh_vlan_proc_dir);
    }
    remove_proc_entry("fhvlan", NULL);

	fhvlan_dev_destroy();

	return 0;
}

int vlan_transfer_init(void)
{
	struct proc_dir_entry *temp_proc=NULL;

	/* initial the global data memory */
	gpWanPriv = (FHVLAN_GlbPriv_T *)kmalloc(sizeof(FHVLAN_GlbPriv_T), GFP_KERNEL);
	if ( gpWanPriv == NULL )
	{
		printk("VLAN transfer Alloc data struct memory failed\n") ;
		goto vlan_transfer_error;
	} 
	else
	{
		memset(gpWanPriv, 0, sizeof(FHVLAN_GlbPriv_T));
		/*printk("VLAN transfer Alloc data struct memory successful, %d\n"
			, sizeof(FHVLAN_GlbPriv_T));*/
		vlan_transfer_init_global_data();
	}

	gfh_vlan_proc_dir = proc_mkdir("fhvlan", NULL);
	if ( NULL == gfh_vlan_proc_dir )
	   goto vlan_transfer_error;

	temp_proc = create_proc_entry("debug", 0, gfh_vlan_proc_dir) ;
    if ( NULL == temp_proc )
		goto vlan_transfer_error;
	temp_proc->read_proc = fh_vlan_debug_read_proc ;
	temp_proc->write_proc = fh_vlan_debug_write_proc;

	if ( 0 != fhvlan_dev_init() )
		goto vlan_transfer_error;

	return 0;

vlan_transfer_error:
	vlan_transfer_exit() ;
	return -EFAULT;
}


