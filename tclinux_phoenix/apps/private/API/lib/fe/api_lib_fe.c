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
#include "libapi_lib_fe.h"
#include <ecnt_hook/ecnt_hook_fe_type.h>

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

static int fe_lib_ioctl(int cmd, void *p)
{
	int fd, ret;
	
	fd = open("/dev/fe", O_RDWR);
    if (fd < 0) {
		printf("Open %s pseudo device failed\n","/dev/fe");
		return -1;
    }

	ret = ioctl(fd, 0, p);
	close(fd);

	if(ret >= 0) 
		return 0;
	
	return -1;
}

static int fe_lib_rate_common(int cmd,uint rate,uint mode)
{
	int ret;
	struct ecnt_fe_data para;

	memset(&para,0,sizeof(struct ecnt_fe_data));
	para.function_id = cmd;
	para.api_data.rate_cfg.rate= rate ;
	para.api_data.rate_cfg.mode = mode;
	ret = fe_lib_ioctl(cmd,&para);
	return ret;
}

/******************************************************************************
function name:	fe_lib_set_tx_rate
 Descriptor:		It's used to set gdma2 tx ratelimit.
 Input Args:		rate:  (unit 1KB/s or 1Kpacket/s)
 				mode: 0:Byte per second
 			         	1: packet per second
 Ret Value:		0: success
 				1: fail
 other:
 
******************************************************************************/
int fe_lib_set_tx_rate(uint rate,uint mode)
{
	return fe_lib_rate_common(FE_SET_TX_RATE,rate,mode);
}

int fe_lib_set_rxuc_rate(uint rate,uint mode)
{
	return fe_lib_rate_common(FE_SET_RXUC_RATE,rate,mode);
}

int fe_lib_set_rxbc_rate(uint rate,uint mode)
{
	return fe_lib_rate_common(FE_SET_RXBC_RATE,rate,mode);
}

int fe_lib_set_rxmc_rate(uint rate,uint mode)
{
	return fe_lib_rate_common(FE_SET_RXMC_RATE,rate,mode);
}

int fe_lib_set_rxoc_rate(uint rate,uint mode)
{
	return fe_lib_rate_common(FE_SET_RXOC_RATE,rate,mode);
}

static int fe_lib_vip_common(int cmd,ushort type,ushort src,ushort dst,unsigned char mode)
{
	int ret;
	struct ecnt_fe_data para;

	memset(&para,0,sizeof(struct ecnt_fe_data));
	para.function_id = cmd;
	para.api_data.vip.type= type ;
	para.api_data.vip.src = src;
	para.api_data.vip.dst = dst;
	para.api_data.vip.mode = mode ;
	ret = fe_lib_ioctl(cmd,&para);
	return ret;
}

int fe_lib_add_vip_ether(ushort type)
{
	return fe_lib_vip_common(FE_ADD_VIP_ETHER,type,0,0,0);
}

int fe_lib_add_vip_ppp(ushort type)
{
	return fe_lib_vip_common(FE_ADD_VIP_PPP,type,0,0,0);
}

int fe_lib_add_vip_ip(ushort type)
{
	return fe_lib_vip_common(FE_ADD_VIP_IP,type,0,0,0);
}

int fe_lib_add_vip_tcp(ushort src,ushort dst,unsigned char mode)
{
	return fe_lib_vip_common(FE_ADD_VIP_TCP,0,src,dst,mode);
}

int fe_lib_add_vip_udp(ushort src,ushort dst,unsigned char mode)
{
	return fe_lib_vip_common(FE_ADD_VIP_UDP,0,src,dst,mode);
}

int fe_lib_del_vip_ether(ushort type)
{
	return fe_lib_vip_common(FE_DEL_VIP_ETHER,type,0,0,0);
}

int fe_lib_del_vip_ppp(ushort type)
{
	return fe_lib_vip_common(FE_DEL_VIP_PPP,type,0,0,0);
}

int fe_lib_del_vip_ip(ushort type)
{
	return fe_lib_vip_common(FE_DEL_VIP_IP,type,0,0,0);
}

int fe_lib_del_vip_tcp(ushort src,ushort dst,unsigned char mode)
{
	return fe_lib_vip_common(FE_DEL_VIP_TCP,0,src,dst,mode);
}

int fe_lib_del_vip_udp(ushort src,ushort dst,unsigned char mode)
{
	return fe_lib_vip_common(FE_DEL_VIP_UDP,0,src,dst,mode);
}

static int fe_lib_l2lu_common(int cmd,ushort svlan,ushort cvlan,uchar prio,uchar mask)
{
	int ret;
	struct ecnt_fe_data para;

	memset(&para,0,sizeof(struct ecnt_fe_data));
	para.function_id = cmd;
	para.api_data.l2lu.svlan= svlan ;
	para.api_data.l2lu.cvlan = cvlan;
	para.api_data.l2lu.dscp = prio;
	para.api_data.l2lu.mask = mask;
	ret = fe_lib_ioctl(cmd,&para);
	return ret;

}

int fe_lib_add_l2lu_vlan_dscp(ushort svlan,ushort cvlan,uchar dscp,uchar mask)
{
	return fe_lib_l2lu_common(FE_ADD_L2LU_VLAN_DSCP,svlan,cvlan,dscp,mask);
}

int fe_lib_add_l2lu_vlan_trfc(ushort svlan,ushort cvlan,uchar trfc,uchar mask)
{
	return fe_lib_l2lu_common(FE_ADD_L2LU_VLAN_TRFC,svlan,cvlan,trfc,mask);
}

int fe_lib_del_l2lu_vlan_dscp(ushort svlan,ushort cvlan,uchar dscp,uchar mask)
{
	return fe_lib_l2lu_common(FE_DEL_L2LU_VLAN_DSCP,svlan,cvlan,dscp,mask);
}

int fe_lib_del_l2lu_vlan_trfc(ushort svlan,ushort cvlan,uchar trfc,uchar mask)
{
	return fe_lib_l2lu_common(FE_DEL_L2LU_VLAN_TRFC,svlan,cvlan,trfc,mask);
}

/******************************************************************************
function name:	fe_lib_set_vip_go_ring1_by_port
 Descriptor:		It's used to set Very Important Packet go ring1 by port, for tr069&voip
 Input Args:		
 				sport: source port
 				dport: dest port
 				mode: 	1 only compare source port
 						2 only compare dest port
 						3 compare source port & dest port
 						7 compare (sport == set sport && dport == set dport) || (sport == set dport && dport == set sport)
 				ip_proto: 0: TCP, 1: UDP
 Ret Value:		0: success
 				1: fail
 other:

******************************************************************************/
int fe_lib_set_vip_go_ring1_by_port(ushort sport,ushort dport,unchar mode,unchar ip_proto)
{
	/*for customers, set SW->QDMA sport and dport.*/
	/*here, should swap the sport and dport*/
	if( 1 == mode)
		mode = 2 ;
	else if( 2 == mode )
		mode = 1 ;
	/*set vip go ring1*/
	if( 0 == ip_proto )
		fe_lib_add_vip_tcp( dport , sport , mode );
	else
		fe_lib_add_vip_udp( dport , sport , mode );
		
	return 0 ;
}

/******************************************************************************
function name:	fe_lib_del_vip_go_ring1_by_port
 Descriptor:		It's used to set Very Important Packet go ring1 by port, for tr069&voip
 Input Args:		
 				sport: source port
 				dport: dest port
 				mode: 	1 only compare source port
 						2 only compare dest port
 						3 compare source port & dest port
 						7 compare (sport == set sport && dport == set dport) || (sport == set dport && dport == set sport)
 				ip_proto: 0: TCP, 1: UDP
 Ret Value:		0: success
 				1: fail
 other:

******************************************************************************/
int fe_lib_del_vip_go_ring1_by_port(ushort sport,ushort dport,unchar mode,unchar ip_proto)
{
	/*for customers, set SW->QDMA sport and dport.*/
	/*here, should swap the sport and dport*/
	if( 1 == mode)
		mode = 2 ;
	else if( 2 == mode )
		mode = 1 ;

	/*del vip go ring1*/
	if( 0 == ip_proto )
		fe_lib_del_vip_tcp( dport , sport , mode );
	else
		fe_lib_del_vip_udp( dport , sport , mode );
		
	return 0 ;
}


