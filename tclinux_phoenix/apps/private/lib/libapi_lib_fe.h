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
#ifndef _API_LIB_PCIE_H
#define _API_LIB_PCIE_H

#define uint	unsigned int 
#define ushort	unsigned short 
#define uchar	unsigned char 
#define unchar	unsigned char 

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
int fe_lib_set_tx_rate(uint rate,uint mode);
int fe_lib_set_rxuc_rate(uint rate,uint mode);
int fe_lib_set_rxbc_rate(uint rate,uint mode);
int fe_lib_set_rxmc_rate(uint rate,uint mode);
int fe_lib_set_rxoc_rate(uint rate,uint mode);

int fe_lib_add_vip_ether(ushort type);
int fe_lib_add_vip_ppp(ushort type);
int fe_lib_add_vip_ip(ushort type);
int fe_lib_add_vip_tcp(ushort src,ushort dst,uchar mode);
int fe_lib_add_vip_udp(ushort src,ushort dst,uchar mode);

int fe_lib_del_vip_ether(ushort type);
int fe_lib_del_vip_ppp(ushort type);
int fe_lib_del_vip_ip(ushort type);
int fe_lib_del_vip_tcp(ushort src,ushort dst,uchar mode);
int fe_lib_del_vip_udp(ushort src,ushort dst,uchar mode);

int fe_lib_add_l2lu_vlan_dscp(ushort svlan,ushort cvlan,uchar dscp,uchar mask);
int fe_lib_add_l2lu_vlan_trfc(ushort svlan,ushort cvlan,uchar trfc,uchar mask);

int fe_lib_del_l2lu_vlan_dscp(ushort svlan,ushort cvlan,uchar dscp,uchar mask);
int fe_lib_del_l2lu_vlan_trfc(ushort svlan,ushort cvlan,uchar trfc,uchar mask);

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
int fe_lib_set_vip_go_ring1_by_port(ushort sport,ushort dport,unchar mode,unchar ip_proto);

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
int fe_lib_del_vip_go_ring1_by_port(ushort sport,ushort dport,unchar mode,unchar ip_proto);


#define ecnt_fe_lib_set_tx_rate fe_lib_set_tx_rate
#define ecnt_fe_lib_set_vip_go_ring1_by_port fe_lib_set_vip_go_ring1_by_port
#define ecnt_fe_lib_del_vip_go_ring1_by_port fe_lib_del_vip_go_ring1_by_port

#endif
