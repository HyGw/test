/***************************************************************
Copyright Statement:

This software/firmware and related documentation (¡°EcoNet Software¡±) 
are protected under relevant copyright laws. The information contained herein 
is confidential and proprietary to EcoNet (HK) Limited (¡°EcoNet¡±) and/or 
its licensors. Without the prior written permission of EcoNet and/or its licensors, 
any reproduction, modification, use or disclosure of EcoNet Software, and 
information contained herein, in whole or in part, shall be strictly prohibited.

EcoNet (HK) Limited  EcoNet. ALL RIGHTS RESERVED.

BY OPENING OR USING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY 
ACKNOWLEDGES AND AGREES THAT THE SOFTWARE/FIRMWARE AND ITS 
DOCUMENTATIONS (¡°ECONET SOFTWARE¡±) RECEIVED FROM ECONET 
AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON AN ¡°AS IS¡± 
BASIS ONLY. ECONET EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, 
WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, 
OR NON-INFRINGEMENT. NOR DOES ECONET PROVIDE ANY WARRANTY 
WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTIES WHICH 
MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE ECONET SOFTWARE. 
RECEIVER AGREES TO LOOK ONLY TO SUCH THIRD PARTIES FOR ANY AND ALL 
WARRANTY CLAIMS RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES 
THAT IT IS RECEIVER¡¯S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD 
PARTY ALL PROPER LICENSES CONTAINED IN ECONET SOFTWARE.

ECONET SHALL NOT BE RESPONSIBLE FOR ANY ECONET SOFTWARE RELEASES 
MADE TO RECEIVER¡¯S SPECIFICATION OR CONFORMING TO A PARTICULAR 
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
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/mtd/rt_flash.h>
#include <linux/random.h>

#include <asm/tc3162/cmdparse.h>
#include <asm/tc3162/tc3162.h>

#ifdef TCSUPPORT_RA_HWNAT
#include <linux/foe_hook.h>
#endif
#include "fe.h"
#include "fe_api.h"
#include "fe_test.h"
#include "../../qdma/EN7512/qdma_api.h"


typedef union {
		 struct {
#ifdef __BIG_ENDIAN
			 uint32 resv1		 : 1;
			 uint32 tsid		 : 5;
			 uint32 tse 		 : 1;
			 uint32 dei 		 : 1;
			 uint32 resv2		 : 12;
			 uint32 oam 		 : 1;
			 uint32 channel 	 : 8;
			 uint32 queue		 : 3;
#else
			 uint32 queue		 : 3;
			 uint32 channel 	 : 8;
			 uint32 oam 		 : 1;
			 uint32 resv2		 : 12;
			 uint32 dei 		 : 1;
			 uint32 tse 		 : 1;
			 uint32 tsid		 : 5;
			 uint32 resv1		 : 1;
#endif 
		 
#ifdef __BIG_ENDIAN
			 uint32 ico 		 : 1;
			 uint32 uco 		 : 1;
			 uint32 tco 		 : 1;
			 uint32 tso 		 : 1;
			 uint32 resv3		 : 6;
			 uint32 fPort		 : 3;
			 uint32 vlanEn		 : 1;
			 uint32 vlanTpID	 : 2;
			 uint32 vlanTag 	 : 16;
#else
			 uint32 vlanTag 	 : 16;
			 uint32 vlanTpID	 : 2;
			 uint32 vlanEn		 : 1;
			 uint32 fPort		 : 3;
			 uint32 resv3		 : 6;
			 uint32 tso 		 : 1;
			 uint32 tco 		 : 1;
			 uint32 uco 		 : 1;
			 uint32 ico 		 : 1;
#endif /* __BIG_ENDIAN */
		 } raw ;
		 uint msg[2] ;
	 } ethTxMsg_t ;
 
 typedef struct
 {
		 
	 uint32 word;
 
 } rxMsgWord0_t;
 
 typedef struct
 {
#ifdef __BIG_ENDIAN
		uint32 resv	  	: 9;
		uint32 sport 	: 4;
		uint32 rsn	  	: 5;
		uint32 entry	: 14;
#else
		uint32 entry	: 14;
		uint32 rsn	  	: 5;
		uint32 sport   : 4;
		uint32 resv		: 9;
#endif	 
 } rxMsgWord1_t;
 
 typedef struct
 {
		 
#ifdef __BIG_ENDIAN
		uint32 resv	  		: 13;
		uint32 vlanEn 	  	: 1;
		uint32 vlanTpID	: 2;
		uint32 vlanTag	  	: 16;
#else
		uint32 vlanTag	  	: 16;
		uint32 vlanTpID	: 2;
		uint32 vlanEn 	  	: 1;
		uint32 resv		  	: 13;
#endif
 
 } rxMsgWord2_t;
 
 
 typedef struct ethRxMsg_s
 {
		 rxMsgWord0_t rxMsgW0;
		 rxMsgWord1_t rxMsgW1;
		 rxMsgWord2_t rxMsgW2;
		 uint32  resv;
 
 } ethRxMsg_t;

typedef struct fe_test_data_s{
	int type;
//	int len;
//	int datalen;
//	char data[128];
}fe_test_data;



#define FE_MAC_MASK  0x0f
#define FE_OC		1
#define FE_BC		2
#define FE_MC		4
///my mac address
#define FE_UC		8   

#define FE_VLAN		4
#define FE_VLAN_MASK	(0x0f << FE_VLAN)

#define FE_0VLAN		1 << FE_VLAN
#define FE_1CVLAN		2 << FE_VLAN
#define FE_1SVLAN		4 << FE_VLAN
#define FE_2VLAN		8 << FE_VLAN

#define FE_LAY3		8
#define FE_LAY3_MASK		(0x3f << FE_LAY3)

#define FE_ARP				1 << FE_LAY3
#define FE_IPV4			2 << FE_LAY3
#define FE_IPV6 			4 << FE_LAY3
#define FE_PPPOE_IPV4 	8 << FE_LAY3
#define FE_PPPOE_IPV6 	16 << FE_LAY3
#define FE_PPPOED 		32 << FE_LAY3


#define FE_LAY4		16
#define FE_LAY4_MASK		(0x0f << FE_LAY4)

#define FE_ICMP		1 << FE_LAY4
#define FE_IGMP		2 << FE_LAY4
#define FE_TCP 		4 << FE_LAY4
#define FE_UDP 		8 << FE_LAY4


////packet type format
/*****************************************************
| Resv(8)| Layer4(8) | Layer3(8)  | VLAN(4) | MAC(4) |
*****************************************************/
////////ARP 
#define ARP_0VLAN_OC	(FE_ARP ) | (FE_0VLAN) | (FE_OC)
#define ARP_1CVLAN_OC	(FE_ARP ) | (FE_1CVLAN)  | (FE_OC)
#define ARP_1SVLAN_OC	(FE_ARP ) | (FE_1SVLAN)  | (FE_OC)
#define ARP_2VLAN_OC	(FE_ARP ) | (FE_2VLAN)  | (FE_OC)

#define ARP_0VLAN_BC	(FE_ARP ) | (FE_0VLAN ) | (FE_BC)
#define ARP_1CVLAN_BC	(FE_ARP ) | (FE_1CVLAN ) | (FE_BC)
#define ARP_1SVLAN_BC	(FE_ARP ) | (FE_1SVLAN ) | (FE_BC)
#define ARP_2VLAN_BC	(FE_ARP ) | (FE_2VLAN ) | (FE_BC)

#define ARP_0VLAN_UC	(FE_ARP ) | (FE_0VLAN ) | (FE_UC)
#define ARP_1CVLAN_UC	(FE_ARP ) | (FE_1CVLAN ) | (FE_UC)
#define ARP_1SVLAN_UC	(FE_ARP ) | (FE_1SVLAN ) | (FE_UC)
#define ARP_2VLAN_UC	(FE_ARP ) | (FE_2VLAN ) | (FE_UC)

///////ICMPV4
#define ICMP_IPV4_0VLAN_OC	(FE_ICMP ) | (FE_IPV4 ) | (FE_0VLAN ) | (FE_OC)
#define ICMP_IPV4_1CVLAN_OC	(FE_ICMP ) | (FE_IPV4 ) | (FE_1CVLAN) | (FE_OC)
#define ICMP_IPV4_1SVLAN_OC	(FE_ICMP ) | (FE_IPV4 ) | (FE_1SVLAN ) | (FE_OC)
#define ICMP_IPV4_2VLAN_OC	(FE_ICMP ) | (FE_IPV4 ) | (FE_2VLAN ) | (FE_OC)

#define ICMP_IPV4_0VLAN_BC	(FE_ICMP ) | (FE_IPV4) | (FE_0VLAN ) | (FE_BC)
#define ICMP_IPV4_1CVLAN_BC	(FE_ICMP ) | (FE_IPV4) | (FE_1CVLAN) | (FE_BC)
#define ICMP_IPV4_1SVLAN_BC	(FE_ICMP ) | (FE_IPV4) | (FE_1SVLAN ) | (FE_BC)
#define ICMP_IPV4_2VLAN_BC	(FE_ICMP ) | (FE_IPV4) | (FE_2VLAN) | (FE_BC)

#define ICMP_IPV4_0VLAN_UC	(FE_ICMP ) | (FE_IPV4) | (FE_0VLAN ) | (FE_UC)
#define ICMP_IPV4_1CVLAN_UC	(FE_ICMP ) | (FE_IPV4) | (FE_1CVLAN ) | (FE_UC)
#define ICMP_IPV4_1SVLAN_UC	(FE_ICMP ) | (FE_IPV4) | (FE_1SVLAN) | (FE_UC)
#define ICMP_IPV4_2VLAN_UC	(FE_ICMP ) | (FE_IPV4) | (FE_2VLAN ) | (FE_UC)

////////IGMPV4
#define IGMP_IPV4_0VLAN_MC	(FE_IGMP ) | (FE_IPV4 ) | (FE_0VLAN) | (FE_MC)
#define IGMP_IPV4_1CVLAN_MC	(FE_IGMP ) | (FE_IPV4 ) | (FE_1CVLAN) | (FE_MC)
#define IGMP_IPV4_1SVLAN_MC	(FE_IGMP ) | (FE_IPV4 ) | (FE_1SVLAN) | (FE_MC)
#define IGMP_IPV4_2VLAN_MC	(FE_IGMP ) | (FE_IPV4 ) | (FE_2VLAN) | (FE_MC)

///////TCPV4
#define TCP_IPV4_0VLAN_OC		(FE_TCP ) | (FE_IPV4 ) | (FE_0VLAN ) | (FE_OC)
#define TCP_IPV4_1CVLAN_OC	(FE_TCP ) | (FE_IPV4 ) | (FE_1CVLAN) | (FE_OC)
#define TCP_IPV4_1SVLAN_OC	(FE_TCP ) | (FE_IPV4 ) | (FE_1SVLAN) | (FE_OC)
#define TCP_IPV4_2VLAN_OC		(FE_TCP ) | (FE_IPV4 ) | (FE_2VLAN ) | (FE_OC)
 
#define TCP_IPV4_0VLAN_BC		(FE_TCP ) | (FE_IPV4) | (FE_0VLAN ) | (FE_BC)
#define TCP_IPV4_1CVLAN_BC	(FE_TCP ) | (FE_IPV4) | (FE_1CVLAN) | (FE_BC)
#define TCP_IPV4_1SVLAN_BC	(FE_TCP ) | (FE_IPV4) | (FE_1SVLAN ) | (FE_BC)
#define TCP_IPV4_2VLAN_BC		(FE_TCP ) | (FE_IPV4) | (FE_2VLAN ) | (FE_BC)

#define TCP_IPV4_0VLAN_MC		(FE_TCP ) | (FE_IPV4 ) | (FE_0VLAN ) | (FE_MC)
#define TCP_IPV4_1CVLAN_MC	(FE_TCP ) | (FE_IPV4 ) | (FE_1CVLAN ) | (FE_MC)
#define TCP_IPV4_1SVLAN_MC	(FE_TCP ) | (FE_IPV4 ) | (FE_1SVLAN ) | (FE_MC)
#define TCP_IPV4_2VLAN_MC		(FE_TCP ) | (FE_IPV4 ) | (FE_2VLAN ) | (FE_MC)

#define TCP_IPV4_0VLAN_UC		(FE_TCP ) | (FE_IPV4 ) | (FE_0VLAN ) | (FE_UC)
#define TCP_IPV4_1CVLAN_UC	(FE_TCP ) | (FE_IPV4 ) | (FE_1CVLAN ) | (FE_UC)
#define TCP_IPV4_1SVLAN_UC	(FE_TCP ) | (FE_IPV4 ) | (FE_1SVLAN ) | (FE_UC)
#define TCP_IPV4_2VLAN_UC		(FE_TCP ) | (FE_IPV4 ) | (FE_2VLAN ) | (FE_UC)

//////UDPV4
#define UDP_IPV4_0VLAN_OC		(FE_UDP ) | (FE_IPV4 ) | (FE_0VLAN ) | (FE_OC)
#define UDP_IPV4_1CVLAN_OC	(FE_UDP ) | (FE_IPV4 ) | (FE_1CVLAN ) | (FE_OC)
#define UDP_IPV4_1SVLAN_OC	(FE_UDP ) | (FE_IPV4 ) | (FE_1SVLAN ) | (FE_OC)
#define UDP_IPV4_2VLAN_OC		(FE_UDP ) | (FE_IPV4 ) | (FE_2VLAN ) | (FE_OC)
  
#define UDP_IPV4_0VLAN_BC		(FE_UDP ) | (FE_IPV4 ) | (FE_0VLAN ) | (FE_BC)
#define UDP_IPV4_1CVLAN_BC	(FE_UDP ) | (FE_IPV4 ) | (FE_1CVLAN ) | (FE_BC)
#define UDP_IPV4_1SVLAN_BC	(FE_UDP ) | (FE_IPV4 ) | (FE_1SVLAN ) | (FE_BC)
#define UDP_IPV4_2VLAN_BC		(FE_UDP ) | (FE_IPV4 ) | (FE_2VLAN ) | (FE_BC)
 
#define UDP_IPV4_0VLAN_MC		(FE_UDP ) | (FE_IPV4) | (FE_0VLAN ) | (FE_MC)
#define UDP_IPV4_1CVLAN_MC	(FE_UDP ) | (FE_IPV4) | (FE_1CVLAN ) | (FE_MC)
#define UDP_IPV4_1SVLAN_MC	(FE_UDP ) | (FE_IPV4) | (FE_1SVLAN ) | (FE_MC)
#define UDP_IPV4_2VLAN_MC		(FE_UDP ) | (FE_IPV4) | (FE_2VLAN ) | (FE_MC)
 
#define UDP_IPV4_0VLAN_UC		(FE_UDP ) | (FE_IPV4 ) | (FE_0VLAN ) | (FE_UC)
#define UDP_IPV4_1CVLAN_UC	(FE_UDP ) | (FE_IPV4 ) | (FE_1CVLAN ) | (FE_UC)
#define UDP_IPV4_1SVLAN_UC	(FE_UDP ) | (FE_IPV4 ) | (FE_1SVLAN ) | (FE_UC)
#define UDP_IPV4_2VLAN_UC		(FE_UDP ) | (FE_IPV4 ) | (FE_2VLAN ) | (FE_UC)

//////ICMPV6
#define ICMP_IPV6_0VLAN_OC	(FE_ICMP ) | (FE_IPV6 ) | (FE_0VLAN ) | (FE_OC)
#define ICMP_IPV6_1CVLAN_OC	(FE_ICMP ) | (FE_IPV6 ) | (FE_1CVLAN ) | (FE_OC)
#define ICMP_IPV6_1SVLAN_OC	(FE_ICMP ) | (FE_IPV6 ) | (FE_1SVLAN) | (FE_OC)
#define ICMP_IPV6_2VLAN_OC	(FE_ICMP ) | (FE_IPV6 ) | (FE_2VLAN ) | (FE_OC)
 
#define ICMP_IPV6_0VLAN_BC	(FE_ICMP ) | (FE_IPV6 ) | (FE_0VLAN ) | (FE_BC)
#define ICMP_IPV6_1CVLAN_BC	(FE_ICMP ) | (FE_IPV6 ) | (FE_1CVLAN) | (FE_BC)
#define ICMP_IPV6_1SVLAN_BC	(FE_ICMP ) | (FE_IPV6 ) | (FE_1SVLAN ) | (FE_BC)
#define ICMP_IPV6_2VLAN_BC	(FE_ICMP ) | (FE_IPV6) |  (FE_2VLAN ) | (FE_BC)

#define ICMP_IPV6_0VLAN_MC	(FE_ICMP ) | (FE_IPV6 ) | (FE_0VLAN ) | (FE_MC)
#define ICMP_IPV6_1CVLAN_MC	(FE_ICMP ) | (FE_IPV6 ) | (FE_1CVLAN ) | (FE_MC)
#define ICMP_IPV6_1SVLAN_MC	(FE_ICMP ) | (FE_IPV6 ) | (FE_1SVLAN ) | (FE_MC)
#define ICMP_IPV6_2VLAN_MC	(FE_ICMP ) | (FE_IPV6) | (FE_2VLAN ) | (FE_MC)

#define ICMP_IPV6_0VLAN_UC	(FE_ICMP ) | (FE_IPV6 ) | (FE_0VLAN ) | (FE_UC)
#define ICMP_IPV6_1CVLAN_UC	(FE_ICMP ) | (FE_IPV6 ) | (FE_1CVLAN ) | (FE_UC)
#define ICMP_IPV6_1SVLAN_UC	(FE_ICMP ) | (FE_IPV6 ) | (FE_1SVLAN ) | (FE_UC)
#define ICMP_IPV6_2VLAN_UC	(FE_ICMP ) | (FE_IPV6 ) | (FE_2VLAN ) | (FE_UC)
 
////////TCPV6
#define TCP_IPV6_0VLAN_OC		(FE_TCP ) | (FE_IPV6 ) | (FE_0VLAN ) | (FE_OC)
#define TCP_IPV6_1CVLAN_OC	(FE_TCP ) | (FE_IPV6 ) | (FE_1CVLAN ) | (FE_OC)
#define TCP_IPV6_1SVLAN_OC	(FE_TCP ) | (FE_IPV6 ) | (FE_1SVLAN ) | (FE_OC)
#define TCP_IPV6_2VLAN_OC		(FE_TCP ) | (FE_IPV6 ) | (FE_2VLAN ) | (FE_OC)
	 
#define TCP_IPV6_0VLAN_BC		(FE_TCP ) | (FE_IPV6 ) | (FE_0VLAN ) | (FE_BC)
#define TCP_IPV6_1CVLAN_BC	(FE_TCP ) | (FE_IPV6 ) | (FE_1CVLAN ) | (FE_BC)
#define TCP_IPV6_1SVLAN_BC	(FE_TCP ) | (FE_IPV6 ) | (FE_1SVLAN ) | (FE_BC)
#define TCP_IPV6_2VLAN_BC		(FE_TCP ) | (FE_IPV6 ) | (FE_2VLAN ) | (FE_BC)
	
#define TCP_IPV6_0VLAN_MC		(FE_TCP ) | (FE_IPV6 ) | (FE_0VLAN ) | (FE_MC)
#define TCP_IPV6_1CVLAN_MC	(FE_TCP) | (FE_IPV6 ) | (FE_1CVLAN ) | (FE_MC)
#define TCP_IPV6_1SVLAN_MC	(FE_TCP ) | (FE_IPV6 ) | (FE_1SVLAN) | (FE_MC)
#define TCP_IPV6_2VLAN_MC		(FE_TCP ) | (FE_IPV6 ) | (FE_2VLAN ) | (FE_MC)
	
#define TCP_IPV6_0VLAN_UC		(FE_TCP ) | (FE_IPV6 ) | (FE_0VLAN ) | (FE_UC)
#define TCP_IPV6_1CVLAN_UC	(FE_TCP ) | (FE_IPV6) | (FE_1CVLAN ) | (FE_UC)
#define TCP_IPV6_1SVLAN_UC	(FE_TCP ) | (FE_IPV6 ) | (FE_1SVLAN ) | (FE_UC)
#define TCP_IPV6_2VLAN_UC		(FE_TCP ) | (FE_IPV6 ) | (FE_2VLAN ) | (FE_UC)
	
//////UDPV6
#define UDP_IPV6_0VLAN_OC		(FE_UDP ) | (FE_IPV6 ) | (FE_0VLAN ) | (FE_OC)
#define UDP_IPV6_1CVLAN_OC	(FE_UDP ) | (FE_IPV6 ) | (FE_1CVLAN) | (FE_OC)
#define UDP_IPV6_1SVLAN_OC	(FE_UDP ) | (FE_IPV6 ) | (FE_1SVLAN ) | (FE_OC)
#define UDP_IPV6_2VLAN_OC		(FE_UDP ) | (FE_IPV6 ) | (FE_2VLAN ) | (FE_OC)
	  
#define UDP_IPV6_0VLAN_BC		(FE_UDP ) | (FE_IPV6 ) | (FE_0VLAN ) | (FE_BC)
#define UDP_IPV6_1CVLAN_BC	(FE_UDP ) | (FE_IPV6 ) | (FE_1CVLAN ) | (FE_BC)
#define UDP_IPV6_1SVLAN_BC	(FE_UDP ) | (FE_IPV6 ) | (FE_1SVLAN ) | (FE_BC)
#define UDP_IPV6_2VLAN_BC		(FE_UDP ) | (FE_IPV6 ) | (FE_2VLAN ) | (FE_BC)
	 
#define UDP_IPV6_0VLAN_MC		(FE_UDP ) | (FE_IPV6 ) | (FE_0VLAN ) | (FE_MC)
#define UDP_IPV6_1CVLAN_MC	(FE_UDP ) | (FE_IPV6 ) | (FE_1CVLAN ) | (FE_MC)
#define UDP_IPV6_1SVLAN_MC	(FE_UDP ) | (FE_IPV6 ) | (FE_1SVLAN ) | (FE_MC)
#define UDP_IPV6_2VLAN_MC		(FE_UDP ) | (FE_IPV6 ) | (FE_2VLAN ) | (FE_MC)
	 
#define UDP_IPV6_0VLAN_UC		(FE_UDP ) | (FE_IPV6 ) | (FE_0VLAN) | (FE_UC)
#define UDP_IPV6_1CVLAN_UC	(FE_UDP ) | (FE_IPV6 ) | (FE_1CVLAN ) | (FE_UC)
#define UDP_IPV6_1SVLAN_UC	(FE_UDP ) | (FE_IPV6 ) | (FE_1SVLAN ) | (FE_UC)
#define UDP_IPV6_2VLAN_UC		(FE_UDP ) | (FE_IPV6 ) | (FE_2VLAN ) | (FE_UC)

///PPPOE_IPV4
 ///////ICMPV4
#define ICMP_PPPOE_IPV4_0VLAN_OC	(FE_ICMP ) | (FE_PPPOE_IPV4) | (FE_0VLAN ) | (FE_OC)
#define ICMP_PPPOE_IPV4_1CVLAN_OC	(FE_ICMP ) | (FE_PPPOE_IPV4) | (FE_1CVLAN) | (FE_OC)
#define ICMP_PPPOE_IPV4_1SVLAN_OC	(FE_ICMP ) | (FE_PPPOE_IPV4) | (FE_1SVLAN ) | (FE_OC)
#define ICMP_PPPOE_IPV4_2VLAN_OC	(FE_ICMP ) | (FE_PPPOE_IPV4) | (FE_2VLAN ) | (FE_OC)
 
#define ICMP_PPPOE_IPV4_0VLAN_BC	(FE_ICMP ) | (FE_PPPOE_IPV4) | (FE_0VLAN ) | (FE_BC)
#define ICMP_PPPOE_IPV4_1CVLAN_BC	(FE_ICMP ) | (FE_PPPOE_IPV4) | (FE_1CVLAN) | (FE_BC)
#define ICMP_PPPOE_IPV4_1SVLAN_BC	(FE_ICMP ) | (FE_PPPOE_IPV4) | (FE_1SVLAN ) | (FE_BC)
#define ICMP_PPPOE_IPV4_2VLAN_BC	(FE_ICMP ) | (FE_PPPOE_IPV4) | (FE_2VLAN) | (FE_BC)
 
#define ICMP_PPPOE_IPV4_0VLAN_UC	(FE_ICMP ) | (FE_PPPOE_IPV4) | (FE_0VLAN ) | (FE_UC)
#define ICMP_PPPOE_IPV4_1CVLAN_UC	(FE_ICMP ) | (FE_PPPOE_IPV4) | (FE_1CVLAN ) | (FE_UC)
#define ICMP_PPPOE_IPV4_1SVLAN_UC	(FE_ICMP ) | (FE_PPPOE_IPV4) | (FE_1SVLAN) | (FE_UC)
#define ICMP_PPPOE_IPV4_2VLAN_UC	(FE_ICMP ) | (FE_PPPOE_IPV4) | (FE_2VLAN ) | (FE_UC)
 
 ////////IGMPV4
#define IGMP_PPPOE_IPV4_0VLAN_MC	(FE_IGMP ) | (FE_PPPOE_IPV4 ) | (FE_0VLAN) | (FE_MC)
#define IGMP_PPPOE_IPV4_1CVLAN_MC	(FE_IGMP ) | (FE_PPPOE_IPV4 ) | (FE_1CVLAN) | (FE_MC)
#define IGMP_PPPOE_IPV4_1SVLAN_MC	(FE_IGMP ) | (FE_PPPOE_IPV4 ) | (FE_1SVLAN) | (FE_MC)
#define IGMP_PPPOE_IPV4_2VLAN_MC	(FE_IGMP ) | (FE_PPPOE_IPV4 ) | (FE_2VLAN) | (FE_MC)
 
 ///////TCPV4
#define TCP_PPPOE_IPV4_0VLAN_OC		(FE_TCP ) | (FE_PPPOE_IPV4 ) | (FE_0VLAN ) | (FE_OC)
#define TCP_PPPOE_IPV4_1CVLAN_OC	(FE_TCP ) | (FE_PPPOE_IPV4 ) | (FE_1CVLAN) | (FE_OC)
#define TCP_PPPOE_IPV4_1SVLAN_OC	(FE_TCP ) | (FE_PPPOE_IPV4 ) | (FE_1SVLAN) | (FE_OC)
#define TCP_PPPOE_IPV4_2VLAN_OC		(FE_TCP ) | (FE_PPPOE_IPV4 ) | (FE_2VLAN ) | (FE_OC)
  
#define TCP_PPPOE_IPV4_0VLAN_BC		(FE_TCP ) | (FE_PPPOE_IPV4) | (FE_0VLAN ) | (FE_BC)
#define TCP_PPPOE_IPV4_1CVLAN_BC	(FE_TCP ) | (FE_PPPOE_IPV4) | (FE_1CVLAN) | (FE_BC)
#define TCP_PPPOE_IPV4_1SVLAN_BC	(FE_TCP ) | (FE_PPPOE_IPV4) | (FE_1SVLAN ) | (FE_BC)
#define TCP_PPPOE_IPV4_2VLAN_BC		(FE_TCP ) | (FE_PPPOE_IPV4) | (FE_2VLAN ) | (FE_BC)
 
#define TCP_PPPOE_IPV4_0VLAN_MC		(FE_TCP ) | (FE_PPPOE_IPV4 ) | (FE_0VLAN ) | (FE_MC)
#define TCP_PPPOE_IPV4_1CVLAN_MC	(FE_TCP ) | (FE_PPPOE_IPV4 ) | (FE_1CVLAN ) | (FE_MC)
#define TCP_PPPOE_IPV4_1SVLAN_MC	(FE_TCP ) | (FE_PPPOE_IPV4 ) | (FE_1SVLAN ) | (FE_MC)
#define TCP_PPPOE_IPV4_2VLAN_MC		(FE_TCP ) | (FE_PPPOE_IPV4 ) | (FE_2VLAN ) | (FE_MC)
 
#define TCP_PPPOE_IPV4_0VLAN_UC		(FE_TCP ) | (FE_PPPOE_IPV4 ) | (FE_0VLAN ) | (FE_UC)
#define TCP_PPPOE_IPV4_1CVLAN_UC	(FE_TCP ) | (FE_PPPOE_IPV4 ) | (FE_1CVLAN ) | (FE_UC)
#define TCP_PPPOE_IPV4_1SVLAN_UC	(FE_TCP ) | (FE_PPPOE_IPV4 ) | (FE_1SVLAN ) | (FE_UC)
#define TCP_PPPOE_IPV4_2VLAN_UC		(FE_TCP ) | (FE_PPPOE_IPV4 ) | (FE_2VLAN ) | (FE_UC)
 
 //////UDPV4
#define UDP_PPPOE_IPV4_0VLAN_OC		(FE_UDP ) | (FE_PPPOE_IPV4 ) | (FE_0VLAN ) | (FE_OC)
#define UDP_PPPOE_IPV4_1CVLAN_OC	(FE_UDP ) | (FE_PPPOE_IPV4 ) | (FE_1CVLAN ) | (FE_OC)
#define UDP_PPPOE_IPV4_1SVLAN_OC	(FE_UDP ) | (FE_PPPOE_IPV4 ) | (FE_1SVLAN ) | (FE_OC)
#define UDP_PPPOE_IPV4_2VLAN_OC		(FE_UDP ) | (FE_PPPOE_IPV4 ) | (FE_2VLAN ) | (FE_OC)
   
#define UDP_PPPOE_IPV4_0VLAN_BC		(FE_UDP ) | (FE_PPPOE_IPV4 ) | (FE_0VLAN ) | (FE_BC)
#define UDP_PPPOE_IPV4_1CVLAN_BC	(FE_UDP ) | (FE_PPPOE_IPV4 ) | (FE_1CVLAN ) | (FE_BC)
#define UDP_PPPOE_IPV4_1SVLAN_BC	(FE_UDP ) | (FE_PPPOE_IPV4 ) | (FE_1SVLAN ) | (FE_BC)
#define UDP_PPPOE_IPV4_2VLAN_BC		(FE_UDP ) | (FE_PPPOE_IPV4 ) | (FE_2VLAN ) | (FE_BC)
  
#define UDP_PPPOE_IPV4_0VLAN_MC		(FE_UDP ) | (FE_PPPOE_IPV4) | (FE_0VLAN ) | (FE_MC)
#define UDP_PPPOE_IPV4_1CVLAN_MC	(FE_UDP ) | (FE_PPPOE_IPV4) | (FE_1CVLAN ) | (FE_MC)
#define UDP_PPPOE_IPV4_1SVLAN_MC	(FE_UDP ) | (FE_PPPOE_IPV4) | (FE_1SVLAN ) | (FE_MC)
#define UDP_PPPOE_IPV4_2VLAN_MC		(FE_UDP ) | (FE_PPPOE_IPV4) | (FE_2VLAN ) | (FE_MC)
  
#define UDP_PPPOE_IPV4_0VLAN_UC		(FE_UDP ) | (FE_PPPOE_IPV4 ) | (FE_0VLAN ) | (FE_UC)
#define UDP_PPPOE_IPV4_1CVLAN_UC	(FE_UDP ) | (FE_PPPOE_IPV4 ) | (FE_1CVLAN ) | (FE_UC)
#define UDP_PPPOE_IPV4_1SVLAN_UC	(FE_UDP ) | (FE_PPPOE_IPV4 ) | (FE_1SVLAN ) | (FE_UC)
#define UDP_PPPOE_IPV4_2VLAN_UC		(FE_UDP ) | (FE_PPPOE_IPV4 ) | (FE_2VLAN ) | (FE_UC)
 
 //////ICMPV6
#define ICMP_PPPOE_IPV6_0VLAN_OC	(FE_ICMP ) | (FE_PPPOE_IPV6 ) | (FE_0VLAN ) | (FE_OC)
#define ICMP_PPPOE_IPV6_1CVLAN_OC	(FE_ICMP ) | (FE_PPPOE_IPV6 ) | (FE_1CVLAN ) | (FE_OC)
#define ICMP_PPPOE_IPV6_1SVLAN_OC	(FE_ICMP ) | (FE_PPPOE_IPV6) | (FE_1SVLAN) | (FE_OC)
#define ICMP_PPPOE_IPV6_2VLAN_OC	(FE_ICMP ) | (FE_PPPOE_IPV6 ) | (FE_2VLAN ) | (FE_OC)
  
#define ICMP_PPPOE_IPV6_0VLAN_BC	(FE_ICMP ) | (FE_PPPOE_IPV6 ) | (FE_0VLAN ) | (FE_BC)
#define ICMP_PPPOE_IPV6_1CVLAN_BC	(FE_ICMP ) | (FE_PPPOE_IPV6 ) | (FE_1CVLAN) | (FE_BC)
#define ICMP_PPPOE_IPV6_1SVLAN_BC	(FE_ICMP ) | (FE_PPPOE_IPV6 ) | (FE_1SVLAN ) | (FE_BC)
#define ICMP_PPPOE_IPV6_2VLAN_BC	(FE_ICMP ) | (FE_PPPOE_IPV6) |  (FE_2VLAN ) | (FE_BC)
 
#define ICMP_PPPOE_IPV6_0VLAN_MC	(FE_ICMP ) | (FE_PPPOE_IPV6 ) | (FE_0VLAN ) | (FE_MC)
#define ICMP_PPPOE_IPV6_1CVLAN_MC	(FE_ICMP ) | (FE_PPPOE_IPV6 ) | (FE_1CVLAN ) | (FE_MC)
#define ICMP_PPPOE_IPV6_1SVLAN_MC	(FE_ICMP ) | (FE_PPPOE_IPV6 ) | (FE_1SVLAN ) | (FE_MC)
#define ICMP_PPPOE_IPV6_2VLAN_MC	(FE_ICMP ) | (FE_PPPOE_IPV6) | (FE_2VLAN ) | (FE_MC)
 
#define ICMP_PPPOE_IPV6_0VLAN_UC	(FE_ICMP ) | (FE_PPPOE_IPV6 ) | (FE_0VLAN ) | (FE_UC)
#define ICMP_PPPOE_IPV6_1CVLAN_UC	(FE_ICMP ) | (FE_PPPOE_IPV6 ) | (FE_1CVLAN ) | (FE_UC)
#define ICMP_PPPOE_IPV6_1SVLAN_UC	(FE_ICMP ) | (FE_PPPOE_IPV6 ) | (FE_1SVLAN ) | (FE_UC)
#define ICMP_PPPOE_IPV6_2VLAN_UC	(FE_ICMP ) | (FE_PPPOE_IPV6 ) | (FE_2VLAN ) | (FE_UC)
  
 ////////TCPV6
#define TCP_PPPOE_IPV6_0VLAN_OC		(FE_TCP ) | (FE_PPPOE_IPV6) | (FE_0VLAN ) | (FE_OC)
#define TCP_PPPOE_IPV6_1CVLAN_OC	(FE_TCP ) | (FE_PPPOE_IPV6 ) | (FE_1CVLAN ) | (FE_OC)
#define TCP_PPPOE_IPV6_1SVLAN_OC	(FE_TCP ) | (FE_PPPOE_IPV6 ) | (FE_1SVLAN ) | (FE_OC)
#define TCP_PPPOE_IPV6_2VLAN_OC		(FE_TCP ) | (FE_PPPOE_IPV6 ) | (FE_2VLAN ) | (FE_OC)
	  
#define TCP_PPPOE_IPV6_0VLAN_BC		(FE_TCP ) | (FE_PPPOE_IPV6 ) | (FE_0VLAN ) | (FE_BC)
#define TCP_PPPOE_IPV6_1CVLAN_BC	(FE_TCP ) | (FE_PPPOE_IPV6 ) | (FE_1CVLAN ) | (FE_BC)
#define TCP_PPPOE_IPV6_1SVLAN_BC	(FE_TCP ) | (FE_PPPOE_IPV6 ) | (FE_1SVLAN ) | (FE_BC)
#define TCP_PPPOE_IPV6_2VLAN_BC		(FE_TCP ) | (FE_PPPOE_IPV6 ) | (FE_2VLAN ) | (FE_BC)
	 
#define TCP_PPPOE_IPV6_0VLAN_MC		(FE_TCP ) | (FE_PPPOE_IPV6 ) | (FE_0VLAN ) | (FE_MC)
#define TCP_PPPOE_IPV6_1CVLAN_MC	(FE_TCP ) | (FE_PPPOE_IPV6 ) | (FE_1CVLAN ) | (FE_MC)
#define TCP_PPPOE_IPV6_1SVLAN_MC	(FE_TCP ) | (FE_PPPOE_IPV6 ) | (FE_1SVLAN) | (FE_MC)
#define TCP_PPPOE_IPV6_2VLAN_MC		(FE_TCP ) | (FE_PPPOE_IPV6 ) | (FE_2VLAN ) | (FE_MC)
	 
#define TCP_PPPOE_IPV6_0VLAN_UC		(FE_TCP) | (FE_PPPOE_IPV6 ) | (FE_0VLAN ) | (FE_UC)
#define TCP_PPPOE_IPV6_1CVLAN_UC	(FE_TCP ) | (FE_PPPOE_IPV6 ) | (FE_1CVLAN ) | (FE_UC)
#define TCP_PPPOE_IPV6_1SVLAN_UC	(FE_TCP ) | (FE_PPPOE_IPV6 ) | (FE_1SVLAN ) | (FE_UC)
#define TCP_PPPOE_IPV6_2VLAN_UC		(FE_TCP ) | (FE_PPPOE_IPV6 ) | (FE_2VLAN ) | (FE_UC)
	 
 //////UDPV6
#define UDP_PPPOE_IPV6_0VLAN_OC		(FE_UDP ) | (FE_PPPOE_IPV6) | (FE_0VLAN ) | (FE_OC)
#define UDP_PPPOE_IPV6_1CVLAN_OC	(FE_UDP ) | (FE_PPPOE_IPV6 ) | (FE_1CVLAN) | (FE_OC)
#define UDP_PPPOE_IPV6_1SVLAN_OC	(FE_UDP ) | (FE_PPPOE_IPV6 ) | (FE_1SVLAN ) | (FE_OC)
#define UDP_PPPOE_IPV6_2VLAN_OC		(FE_UDP ) | (FE_PPPOE_IPV6 ) | (FE_2VLAN ) | (FE_OC)
	   
#define UDP_PPPOE_IPV6_0VLAN_BC		(FE_UDP ) | (FE_PPPOE_IPV6 ) | (FE_0VLAN ) | (FE_BC)
#define UDP_PPPOE_IPV6_1CVLAN_BC	(FE_UDP ) | (FE_PPPOE_IPV6 ) | (FE_1CVLAN ) | (FE_BC)
#define UDP_PPPOE_IPV6_1SVLAN_BC	(FE_UDP ) | (FE_PPPOE_IPV6 ) | (FE_1SVLAN ) | (FE_BC)
#define UDP_PPPOE_IPV6_2VLAN_BC		(FE_UDP ) | (FE_PPPOE_IPV6 ) | (FE_2VLAN ) | (FE_BC)
	  
#define UDP_PPPOE_IPV6_0VLAN_MC		(FE_UDP ) | (FE_PPPOE_IPV6 ) | (FE_0VLAN ) | (FE_MC)
#define UDP_PPPOE_IPV6_1CVLAN_MC	(FE_UDP ) | (FE_PPPOE_IPV6 ) | (FE_1CVLAN ) | (FE_MC)
#define UDP_PPPOE_IPV6_1SVLAN_MC	(FE_UDP ) | (FE_PPPOE_IPV6 ) | (FE_1SVLAN ) | (FE_MC)
#define UDP_PPPOE_IPV6_2VLAN_MC		(FE_UDP ) | (FE_PPPOE_IPV6 ) | (FE_2VLAN ) | (FE_MC)
	  
#define UDP_PPPOE_IPV6_0VLAN_UC		(FE_UDP ) | (FE_PPPOE_IPV6 ) | (FE_0VLAN) | (FE_UC)
#define UDP_PPPOE_IPV6_1CVLAN_UC	(FE_UDP ) | (FE_PPPOE_IPV6 ) | (FE_1CVLAN ) | (FE_UC)
#define UDP_PPPOE_IPV6_1SVLAN_UC	(FE_UDP ) | (FE_PPPOE_IPV6 ) | (FE_1SVLAN ) | (FE_UC)
#define UDP_PPPOE_IPV6_2VLAN_UC		(FE_UDP ) | (FE_PPPOE_IPV6 ) | (FE_2VLAN ) | (FE_UC)


 static fe_test_data g_fe_pkt[] = 
 {
 	{ARP_0VLAN_OC},
 	{ARP_1CVLAN_OC},		
	{ARP_1SVLAN_OC},
	{ARP_2VLAN_OC},
 	{ARP_0VLAN_BC},
 	{ARP_1CVLAN_BC},		
	{ARP_1SVLAN_BC},
	{ARP_2VLAN_BC},	
	{ARP_0VLAN_UC},
	{ARP_1CVLAN_UC},		
	{ARP_1SVLAN_UC},
	{ARP_2VLAN_UC}, 
	
	{ICMP_IPV4_0VLAN_OC},
	{ICMP_IPV4_1CVLAN_OC},
	{ICMP_IPV4_1SVLAN_OC},
	{ICMP_IPV4_2VLAN_OC},
	{ICMP_IPV4_0VLAN_BC},
	{ICMP_IPV4_1CVLAN_BC},
	{ICMP_IPV4_1SVLAN_BC},
	{ICMP_IPV4_2VLAN_BC},
	{ICMP_IPV4_0VLAN_UC},
	{ICMP_IPV4_1CVLAN_UC},
	{ICMP_IPV4_1SVLAN_UC},
	{ICMP_IPV4_2VLAN_UC},

	{IGMP_IPV4_0VLAN_MC},
	{IGMP_IPV4_1CVLAN_MC},
	{IGMP_IPV4_1SVLAN_MC},
	{IGMP_IPV4_2VLAN_MC},
	
	{TCP_IPV4_0VLAN_OC},
	{TCP_IPV4_1CVLAN_OC},
	{TCP_IPV4_1SVLAN_OC},
	{TCP_IPV4_2VLAN_OC},
	{TCP_IPV4_0VLAN_BC},
	{TCP_IPV4_1CVLAN_BC},
	{TCP_IPV4_1SVLAN_BC},
	{TCP_IPV4_2VLAN_BC},
	{TCP_IPV4_0VLAN_MC},
	{TCP_IPV4_1CVLAN_MC},
	{TCP_IPV4_1SVLAN_MC},
	{TCP_IPV4_2VLAN_MC},	
	{TCP_IPV4_0VLAN_UC},
	{TCP_IPV4_1CVLAN_UC},
	{TCP_IPV4_1SVLAN_UC},
	{TCP_IPV4_2VLAN_UC},	
	
	{UDP_IPV4_0VLAN_OC},
	{UDP_IPV4_1CVLAN_OC},
	{UDP_IPV4_1SVLAN_OC},
	{UDP_IPV4_2VLAN_OC},
	{UDP_IPV4_0VLAN_BC},
	{UDP_IPV4_1CVLAN_BC},
	{UDP_IPV4_1SVLAN_BC},
	{UDP_IPV4_2VLAN_BC},
	{UDP_IPV4_0VLAN_MC},
	{UDP_IPV4_1CVLAN_MC},
	{UDP_IPV4_1SVLAN_MC},
	{UDP_IPV4_2VLAN_MC},	
	{UDP_IPV4_0VLAN_UC},
	{UDP_IPV4_1CVLAN_UC},
	{UDP_IPV4_1SVLAN_UC},
	{UDP_IPV4_2VLAN_UC},

	{ICMP_IPV6_0VLAN_OC},
	{ICMP_IPV6_1CVLAN_OC},
	{ICMP_IPV6_1SVLAN_OC},
	{ICMP_IPV6_2VLAN_OC},
	{ICMP_IPV6_0VLAN_BC},
	{ICMP_IPV6_1CVLAN_BC},
	{ICMP_IPV6_1SVLAN_BC},
	{ICMP_IPV6_2VLAN_BC},
	{ICMP_IPV6_0VLAN_MC},
	{ICMP_IPV6_1CVLAN_MC},
	{ICMP_IPV6_1SVLAN_MC},
	{ICMP_IPV6_2VLAN_MC},
	{ICMP_IPV6_0VLAN_UC},
	{ICMP_IPV6_1CVLAN_UC},
	{ICMP_IPV6_1SVLAN_UC},
	{ICMP_IPV6_2VLAN_UC},
	
	{TCP_IPV6_0VLAN_OC},
	{TCP_IPV6_1CVLAN_OC},
	{TCP_IPV6_1SVLAN_OC},
	{TCP_IPV6_2VLAN_OC},
	{TCP_IPV6_0VLAN_BC},
	{TCP_IPV6_1CVLAN_BC},
	{TCP_IPV6_1SVLAN_BC},
	{TCP_IPV6_2VLAN_BC},
	{TCP_IPV6_0VLAN_MC},
	{TCP_IPV6_1CVLAN_MC},
	{TCP_IPV6_1SVLAN_MC},
	{TCP_IPV6_2VLAN_MC},	
	{TCP_IPV6_0VLAN_UC},
	{TCP_IPV6_1CVLAN_UC},
	{TCP_IPV6_1SVLAN_UC},
	{TCP_IPV6_2VLAN_UC},	
	
	{UDP_IPV6_0VLAN_OC},
	{UDP_IPV6_1CVLAN_OC},
	{UDP_IPV6_1SVLAN_OC},
	{UDP_IPV6_2VLAN_OC},
	{UDP_IPV6_0VLAN_BC},
	{UDP_IPV6_1CVLAN_BC},
	{UDP_IPV6_1SVLAN_BC},
	{UDP_IPV6_2VLAN_BC},
	{UDP_IPV6_0VLAN_MC},
	{UDP_IPV6_1CVLAN_MC},
	{UDP_IPV6_1SVLAN_MC},
	{UDP_IPV6_2VLAN_MC},	
	{UDP_IPV6_0VLAN_UC},
	{UDP_IPV6_1CVLAN_UC},
	{UDP_IPV6_1SVLAN_UC},
	{UDP_IPV6_2VLAN_UC},	

	
	{ICMP_PPPOE_IPV4_0VLAN_OC},
	{ICMP_PPPOE_IPV4_1CVLAN_OC},
	{ICMP_PPPOE_IPV4_1SVLAN_OC},
	{ICMP_PPPOE_IPV4_2VLAN_OC},
	{ICMP_PPPOE_IPV4_0VLAN_BC},
	{ICMP_PPPOE_IPV4_1CVLAN_BC},
	{ICMP_PPPOE_IPV4_1SVLAN_BC},
	{ICMP_PPPOE_IPV4_2VLAN_BC},
	{ICMP_PPPOE_IPV4_0VLAN_UC},
	{ICMP_PPPOE_IPV4_1CVLAN_UC},
	{ICMP_PPPOE_IPV4_1SVLAN_UC},
	{ICMP_PPPOE_IPV4_2VLAN_UC},

	{IGMP_PPPOE_IPV4_0VLAN_MC},
	{IGMP_PPPOE_IPV4_1CVLAN_MC},
	{IGMP_PPPOE_IPV4_1SVLAN_MC},
	{IGMP_PPPOE_IPV4_2VLAN_MC},
	
	{TCP_PPPOE_IPV4_0VLAN_OC},
	{TCP_PPPOE_IPV4_1CVLAN_OC},
	{TCP_PPPOE_IPV4_1SVLAN_OC},
	{TCP_PPPOE_IPV4_2VLAN_OC},
	{TCP_PPPOE_IPV4_0VLAN_BC},
	{TCP_PPPOE_IPV4_1CVLAN_BC},
	{TCP_PPPOE_IPV4_1SVLAN_BC},
	{TCP_PPPOE_IPV4_2VLAN_BC},
	{TCP_PPPOE_IPV4_0VLAN_MC},
	{TCP_PPPOE_IPV4_1CVLAN_MC},
	{TCP_PPPOE_IPV4_1SVLAN_MC},
	{TCP_PPPOE_IPV4_2VLAN_MC},	
	{TCP_PPPOE_IPV4_0VLAN_UC},
	{TCP_PPPOE_IPV4_1CVLAN_UC},
	{TCP_PPPOE_IPV4_1SVLAN_UC},
	{TCP_PPPOE_IPV4_2VLAN_UC},	
	
	{UDP_PPPOE_IPV4_0VLAN_OC},
	{UDP_PPPOE_IPV4_1CVLAN_OC},
	{UDP_PPPOE_IPV4_1SVLAN_OC},
	{UDP_PPPOE_IPV4_2VLAN_OC},
	{UDP_PPPOE_IPV4_0VLAN_BC},
	{UDP_PPPOE_IPV4_1CVLAN_BC},
	{UDP_PPPOE_IPV4_1SVLAN_BC},
	{UDP_PPPOE_IPV4_2VLAN_BC},
	{UDP_PPPOE_IPV4_0VLAN_MC},
	{UDP_PPPOE_IPV4_1CVLAN_MC},
	{UDP_PPPOE_IPV4_1SVLAN_MC},
	{UDP_PPPOE_IPV4_2VLAN_MC},	
	{UDP_PPPOE_IPV4_0VLAN_UC},
	{UDP_PPPOE_IPV4_1CVLAN_UC},
	{UDP_PPPOE_IPV4_1SVLAN_UC},
	{UDP_PPPOE_IPV4_2VLAN_UC},

	{ICMP_PPPOE_IPV6_0VLAN_OC},
	{ICMP_PPPOE_IPV6_1CVLAN_OC},
	{ICMP_PPPOE_IPV6_1SVLAN_OC},
	{ICMP_PPPOE_IPV6_2VLAN_OC},
	{ICMP_PPPOE_IPV6_0VLAN_BC},
	{ICMP_PPPOE_IPV6_1CVLAN_BC},
	{ICMP_PPPOE_IPV6_1SVLAN_BC},
	{ICMP_PPPOE_IPV6_2VLAN_BC},
	{ICMP_PPPOE_IPV6_0VLAN_MC},
	{ICMP_PPPOE_IPV6_1CVLAN_MC},
	{ICMP_PPPOE_IPV6_1SVLAN_MC},
	{ICMP_PPPOE_IPV6_2VLAN_MC},
	{ICMP_PPPOE_IPV6_0VLAN_UC},
	{ICMP_PPPOE_IPV6_1CVLAN_UC},
	{ICMP_PPPOE_IPV6_1SVLAN_UC},
	{ICMP_PPPOE_IPV6_2VLAN_UC},
	
	{TCP_PPPOE_IPV6_0VLAN_OC},
	{TCP_PPPOE_IPV6_1CVLAN_OC},
	{TCP_PPPOE_IPV6_1SVLAN_OC},
	{TCP_PPPOE_IPV6_2VLAN_OC},
	{TCP_PPPOE_IPV6_0VLAN_BC},
	{TCP_PPPOE_IPV6_1CVLAN_BC},
	{TCP_PPPOE_IPV6_1SVLAN_BC},
	{TCP_PPPOE_IPV6_2VLAN_BC},
	{TCP_PPPOE_IPV6_0VLAN_MC},
	{TCP_PPPOE_IPV6_1CVLAN_MC},
	{TCP_PPPOE_IPV6_1SVLAN_MC},
	{TCP_PPPOE_IPV6_2VLAN_MC},	
	{TCP_PPPOE_IPV6_0VLAN_UC},
	{TCP_PPPOE_IPV6_1CVLAN_UC},
	{TCP_PPPOE_IPV6_1SVLAN_UC},
	{TCP_PPPOE_IPV6_2VLAN_UC},	
	
	{UDP_PPPOE_IPV6_0VLAN_OC},
	{UDP_PPPOE_IPV6_1CVLAN_OC},
	{UDP_PPPOE_IPV6_1SVLAN_OC},
	{UDP_PPPOE_IPV6_2VLAN_OC},
	{UDP_PPPOE_IPV6_0VLAN_BC},
	{UDP_PPPOE_IPV6_1CVLAN_BC},
	{UDP_PPPOE_IPV6_1SVLAN_BC},
	{UDP_PPPOE_IPV6_2VLAN_BC},
	{UDP_PPPOE_IPV6_0VLAN_MC},
	{UDP_PPPOE_IPV6_1CVLAN_MC},
	{UDP_PPPOE_IPV6_1SVLAN_MC},
	{UDP_PPPOE_IPV6_2VLAN_MC},	
	{UDP_PPPOE_IPV6_0VLAN_UC},
	{UDP_PPPOE_IPV6_1CVLAN_UC},
	{UDP_PPPOE_IPV6_1SVLAN_UC},
	{UDP_PPPOE_IPV6_2VLAN_UC},		
 };

typedef struct fe_packet_ctl_s{
	char smac[6];
	unsigned short tpid;
	unsigned short vid1;
	unsigned short vid2;	
	unsigned short pppoeid;	
	union{
		unsigned char sip6[16];
		unsigned int sip4;
	};
	union{
		unsigned char dip6[16];
		unsigned int dip4;
	};
	unsigned short prior;
	unsigned short sport;
	unsigned short dport;
	unsigned short check;
	unsigned short opt6;
	
} fe_packet_ctl;

static fe_packet_ctl g_pkt_ctl;
static unsigned char sip[] = {0xc0,0xa8,0x01,0xb,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01};
static unsigned char dip[] = {0xc0,0xa8,0x01,0x16,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02};

#define FE_PACKET_IS_OC(x) ((x&FE_MAC_MASK) == FE_OC)
#define FE_PACKET_IS_BC(x) ((x&FE_MAC_MASK) == FE_BC)
#define FE_PACKET_IS_MC(x) ((x&FE_MAC_MASK) == FE_MC)
#define FE_PACKET_IS_UC(x) ((x&FE_MAC_MASK) == FE_UC)

#define FE_PACKET_IS_0VLAN(x)  ((x&FE_VLAN_MASK) == (FE_0VLAN ))
#define FE_PACKET_IS_1CVLAN(x) ((x&FE_VLAN_MASK) == (FE_1CVLAN ))
#define FE_PACKET_IS_1SVLAN(x) ((x&FE_VLAN_MASK) == (FE_1SVLAN ))
#define FE_PACKET_IS_2VLAN(x)  ((x&FE_VLAN_MASK) == (FE_2VLAN))

#define FE_PACKET_IS_ARP(x)  			((x&FE_LAY3_MASK) == (FE_ARP ))
#define FE_PACKET_IS_PPPOE_IPV4(x) 	((x&FE_LAY3_MASK) == (FE_PPPOE_IPV4))
#define FE_PACKET_IS_PPPOE_IPV6(x) 	((x&FE_LAY3_MASK) == (FE_PPPOE_IPV6))
#define FE_PACKET_IS_ONLY_IPV4(x) 		((x&FE_LAY3_MASK) == (FE_IPV4))
#define FE_PACKET_IS_ONLY_IPV6(x)		((x&FE_LAY3_MASK) == (FE_IPV6))


#define FE_PACKET_IS_ICMP(x)  		((x&FE_LAY4_MASK) == (FE_ICMP ))
#define FE_PACKET_IS_IGMP(x)			((x&FE_LAY4_MASK) == (FE_IGMP ))
#define FE_PACKET_IS_TCP(x) 			((x&FE_LAY4_MASK) == (FE_TCP ))
#define FE_PACKET_IS_UDP(x) 			((x&FE_LAY4_MASK) == (FE_UDP ))


#define fe_reg_read read_reg_word
#define fe_reg_write write_reg_word
#define fe_reg_modify_bits fe_reg_modify

static unsigned int g_fe_rx_q0 = 0;
static unsigned int g_fe_rx_q1 = 0;
static unsigned int g_fe_tx_q0 = 0;
static int fe_test_gdm = 0;

unsigned int  FE_PACKET_IS_IPV4(unsigned int type)
{
	return FE_PACKET_IS_PPPOE_IPV4(type) || FE_PACKET_IS_ONLY_IPV4(type);
}

unsigned int  FE_PACKET_IS_IPV6(unsigned int type)
{
	return FE_PACKET_IS_PPPOE_IPV6(type) ||  FE_PACKET_IS_ONLY_IPV6(type);
}

unsigned int  FE_PACKET_IS_PPPOE(unsigned int type)
{
	return FE_PACKET_IS_PPPOE_IPV4(type) || FE_PACKET_IS_PPPOE_IPV6(type);
}

typedef struct {
	unsigned char dmac[6];
	unsigned char smac[6];
}fe_mac_header_t;
static fe_mac_header_t g_mac_header;
int fe_test_set_mac_header(unsigned int type)
{
	memset(&g_mac_header,0,12);
	
	memcpy(g_mac_header.smac,g_pkt_ctl.smac,6);
	
	if(FE_PACKET_IS_OC(type)){
		g_mac_header.dmac[5] = 0x22;
	}else if (FE_PACKET_IS_BC(type)){
		memset(g_mac_header.dmac,0xff,6);
	}else if (FE_PACKET_IS_MC(type)){
		if (FE_PACKET_IS_IPV4(type)){
			g_mac_header.dmac[0] = 0x01;
			g_mac_header.dmac[1] = 0x00;	
			g_mac_header.dmac[2] = 0x5e;
			g_mac_header.dmac[5] = 0x11;	
		}else if(FE_PACKET_IS_IPV6(type)) {
			g_mac_header.dmac[0] = 0x33;
			g_mac_header.dmac[1] = 0x33;	
			g_mac_header.dmac[5] = 0x11;
		}else{
			g_mac_header.dmac[0] = 0x11;
			g_mac_header.dmac[5] = 0x11;
		}
	}else if (FE_PACKET_IS_UC(type)){
		g_mac_header.dmac[0] = 0x00;
		g_mac_header.dmac[1] = 0xaa;	
		g_mac_header.dmac[2] = 0xbb;
		g_mac_header.dmac[3] = 0x01;	
		g_mac_header.dmac[4] = 0x23;
		g_mac_header.dmac[5] = 0x45;			
	}else
		return -1;
	
	return 0;
}

typedef struct {
	unsigned short tpid1;
	unsigned short vid1;	
	unsigned short tpid2;
	unsigned short vid2;
}fe_vlan_header_t;
static fe_vlan_header_t g_vlan_header = {0x8100,0,0x8100,0};

int fe_test_set_vlan_header(unsigned int type)
{
	if (FE_PACKET_IS_0VLAN(type)){
		return 0;
	}else if (FE_PACKET_IS_1CVLAN(type)){
		g_vlan_header.tpid1 = g_pkt_ctl.tpid;
		g_vlan_header.vid1 = g_pkt_ctl.vid1;
	}else if(FE_PACKET_IS_1SVLAN(type)){
		g_vlan_header.tpid1 = 0x88a8;
		g_vlan_header.vid1 = g_pkt_ctl.vid1;
	}else if(FE_PACKET_IS_2VLAN(type)){
		g_vlan_header.tpid1 = 0x88a8;
		g_vlan_header.tpid2 = 0x8100;
		g_vlan_header.vid1 = g_pkt_ctl.vid1;
		g_vlan_header.vid2 = g_pkt_ctl.vid2;

	}else
		return -1;

	return 0;
}

typedef struct {
	unsigned char ver;
	unsigned char code;
	unsigned short sid;
	unsigned short len;
	unsigned short proto;	
}fe_pppoe_header_t;
static fe_pppoe_header_t g_pppoe_header = {0x11,0x00,0x0000,0x0000,0x0000};

int fe_test_set_pppoe_header(unsigned int type)
{
	if (FE_PACKET_IS_PPPOE(type) == 0)
		return 0;

	g_pppoe_header.sid = g_pkt_ctl.pppoeid;
	
	if (FE_PACKET_IS_PPPOE_IPV4(type))
		g_pppoe_header.proto = 0x0021;
	else if (FE_PACKET_IS_PPPOE_IPV6(type))
		g_pppoe_header.proto = 0x0057;
	else 
		return -1;
	
	return 0;
}

typedef struct {
	unsigned char ver;
	unsigned char	tos;
	unsigned short	 len;
	unsigned short	 id;
	unsigned short	 frag;
	unsigned char	ttl;
	unsigned char	prot;
	unsigned short check;
	unsigned int	saddr;
	unsigned int	daddr;
}fe_ipv4_header_t;

static fe_ipv4_header_t g_ipv4_header = {0x45,0x00,0x0000,0x0000,0x0000,0xff,0x00,0x0000};

int fe_test_set_ipv4_header(unsigned int type)
{
	if (FE_PACKET_IS_IPV4(type) == 0)
		return 0;

	g_ipv4_header.saddr = g_pkt_ctl.sip4;
	g_ipv4_header.daddr = g_pkt_ctl.dip4;

	g_ipv4_header.tos = g_pkt_ctl.prior & 0xff;

	if(FE_PACKET_IS_ICMP(type)){
		g_ipv4_header.prot = 0x01;
	}else if (FE_PACKET_IS_IGMP(type)){
		g_ipv4_header.prot = 0x02;
	}else if (FE_PACKET_IS_TCP(type)){
		g_ipv4_header.prot = 0x06;
	}else if (FE_PACKET_IS_UDP(type)){
		g_ipv4_header.prot = 0x11;
	}else 
		return -1;
		
	return 0;
}
typedef struct {
	unsigned char	ver;
	unsigned char flow[3];
	unsigned short len;
	unsigned char	next;
	unsigned char	hop;
	unsigned char 	saddr[16];
	unsigned char  daddr[16];
}fe_ipv6_header_t;

typedef struct {
	unsigned char type;
	unsigned char len;
	unsigned char data[7];
}fe_ipv6_opt_header_t;
static fe_ipv6_opt_header_t g_ipv6_opt_header = {0,0,{0,0,0,0,0,0}};
static fe_ipv6_header_t g_ipv6_header = {0x60,{0x00,0x00,0x00},0x0000,0x00,0xff};

int fe_test_set_ipv6_header(unsigned int type)
{
	if (FE_PACKET_IS_IPV6(type) == 0)
		return 0;

	memcpy(g_ipv6_header.saddr,g_pkt_ctl.sip6,16);
	memcpy(g_ipv6_header.daddr,g_pkt_ctl.dip6,16);

	g_ipv6_header.ver |= (g_pkt_ctl.prior >> 4) & 0xf;
	g_ipv6_header.flow[0] = (g_pkt_ctl.prior & 0xf) << 4;
	
	if(FE_PACKET_IS_ICMP(type)){
		g_ipv6_header.next = 0x01;
	}else if (FE_PACKET_IS_IGMP(type)){
		g_ipv6_header.next = 0x02;
	}else if (FE_PACKET_IS_TCP(type)){
		g_ipv6_header.next = 0x06;
	}else if (FE_PACKET_IS_UDP(type)){
		g_ipv6_header.next = 0x11;
	}else 
		return -1;

	if (g_pkt_ctl.opt6 & 1)
	{
		g_ipv6_opt_header.type = g_ipv6_header.next;
		g_ipv6_header.next = (g_pkt_ctl.opt6 >> 8);
	}
	
	return 0;
}


typedef  struct {
	unsigned short	src;
	unsigned short	dest;
	unsigned short	len;
	unsigned short	check;
}fe_udp_header_t;
static fe_udp_header_t g_udp_header = {0x0000,0x0000,0x00000,0xbeaf};

int fe_test_set_udp_header(unsigned int type)
{

	if (FE_PACKET_IS_UDP(type) == 0)
		return 0;
	
	g_udp_header.src = g_pkt_ctl.sport;
	g_udp_header.dest = g_pkt_ctl.dport;
	
	return 0;
}


typedef  struct {
	unsigned short	src;
	unsigned short	dest;
	unsigned int	seq;
	unsigned int	ack;
	unsigned short res1;
	unsigned short	window;
	unsigned short	check;
	unsigned short	urg;
}fe_tcp_header_t;
static fe_tcp_header_t g_tcp_header = {0,0,1,1,0,0x100,0,0};

int fe_test_set_tcp_header(unsigned int type)
{
	if (FE_PACKET_IS_TCP(type) == 0)
		return 0;
	
	g_tcp_header.src = g_pkt_ctl.sport;
	g_tcp_header.dest = g_pkt_ctl.dport;
	g_tcp_header.res1 = 0x5000;
	return 0;
}

static int  fe_test_dump_skb(struct sk_buff *skb)
{
	char tmp[80];
	char *p = skb->data;
	char *t = tmp;
	int i, n = 0;

	printk("skb=%08lx data=%08lx len=%d\n", (uint32) skb, (uint32) skb->data, skb->len);
	for (i = 0; i < skb->len; i++) {
		t += sprintf(t, "%02x ", *p++ & 0xff);
		if ((i & 0x0f) == 0x0f) {
			printk("%04x: %s\n", n, tmp);
			n += 16;
			t = tmp;
		}
	}
	if (i & 0x0f)
		printk("%04x: %s\n", n, tmp);


	return 0;
}



static int fe_test_set_pkt_header(unsigned int type)
{
	if (fe_test_set_mac_header(type) < 0)
		return -1;

	if (fe_test_set_vlan_header(type) < 0)
		return -1;

	if (fe_test_set_pppoe_header(type) < 0)
		return -1;

	if (fe_test_set_ipv4_header(type) < 0)
		return -1;	

	if (fe_test_set_ipv6_header(type) < 0)
		return -1;	
	
	if (fe_test_set_tcp_header(type) < 0)
		return -1;	

	if (fe_test_set_udp_header(type) < 0)
		return -1;	
	
	return 0;
}




int fe_test_create_pkt(unsigned int type,int pktlen,struct sk_buff *skb)
{
	int len = 0,vlan_len = 0;

	if(pktlen < 0)
		pktlen = 0;

	 if(fe_test_set_pkt_header(type) < 0)
	 	goto dump;

	memset(skb->data,0,64);
	
	memcpy(skb->data,&g_mac_header,12);
	
	len += 12;

	 if(FE_PACKET_IS_1CVLAN(type) || FE_PACKET_IS_1SVLAN(type))
	 	vlan_len = 4;
	 else if(FE_PACKET_IS_2VLAN(type))
	 	vlan_len = 8;
	else
		vlan_len = 0;
	
	 if(vlan_len > 0)
	 	memcpy(skb->data + len,&g_vlan_header,vlan_len);

	len += vlan_len;

	if (FE_PACKET_IS_ARP(type)){
		skb->data[len] = 0x08;
		skb->data[len+1] = 0x06;
		len += pktlen + 2;	
		
		skb_put(skb,len);
		return 0;
	}
		
	if(FE_PACKET_IS_PPPOE(type)){
		skb->data[len] = 0x88;
		skb->data[len+1] = 0x64;
		len += 2;	
		g_pppoe_header.len = pktlen + (FE_PACKET_IS_PPPOE_IPV4(type) ? 20:40);
		if (FE_PACKET_IS_TCP(type))		
			g_pppoe_header.len += 20;
		else if (FE_PACKET_IS_UDP(type))
			g_pppoe_header.len += 8;
		
		memcpy(skb->data + len,&g_pppoe_header,8);
		len += 8;	
	}else if (FE_PACKET_IS_ONLY_IPV4(type)){
		skb->data[len] = 0x08;
		skb->data[len+1] = 0x00;
		len += 2;	
	}else if(FE_PACKET_IS_ONLY_IPV6(type)){
		skb->data[len] = 0x86;
		skb->data[len+1] = 0xdd;
		len += 2;	
	}else
		goto dump;


	if(FE_PACKET_IS_IPV4(type)){
		if (FE_PACKET_IS_TCP(type))
			g_ipv4_header.len = pktlen + 40;
		else if (FE_PACKET_IS_UDP(type))
			g_ipv4_header.len = pktlen + 28;
		else
			g_ipv4_header.len = pktlen + 20;
		memcpy(skb->data + len,&g_ipv4_header,20);
		len += 20;
	}else if (FE_PACKET_IS_IPV6(type)){
		if (FE_PACKET_IS_TCP(type))
			g_ipv6_header.len = pktlen + 20;
		else if (FE_PACKET_IS_UDP(type))
			g_ipv6_header.len = pktlen + 8;
		else
			g_ipv6_header.len = pktlen;

		if (g_pkt_ctl.opt6 & 1){
			memcpy(skb->data + len,&g_ipv6_opt_header,8);
			g_ipv6_header.len += 8; 
			len += 8;
		}
			
		memcpy(skb->data + len,&g_ipv6_header,40);
		len += 40;
	}else 
		goto dump;
	
	if (FE_PACKET_IS_TCP(type)){
		memcpy(skb->data + len,&g_tcp_header,20);
		len += 20;
	}else if (FE_PACKET_IS_UDP(type)){
		g_udp_header.len = pktlen;
		memcpy(skb->data + len,&g_udp_header,8);
		len += 8;
	}
	
	skb_put(skb,len + pktlen);

 	return 0;
dump:
	printk("fe_test_create_pkt: error type(%08x) len(%d) \n",type,pktlen);
	fe_test_dump_skb(skb);	
	return -1;
}


static int (*fe_test_rx_hook)(void *msg_p,struct sk_buff *skb) = NULL;
static int g_dump_flag = 0;

static int fe_test_dump_rxmsg(void *msg_p)
{
	unsigned int *  p = (unsigned int * )msg_p;
	printk("Recv Rx MSG: (%08x) (%08x) (%08x) \n",*p,*(p+1),*(p+2));
	return 0;
}

static int fe_test_dump_rx(void *msg_p,struct sk_buff *skb)
{
	fe_test_dump_rxmsg(msg_p);
	fe_test_dump_skb(skb);
	return 0;
}


static  int fe_test_rx(void *msg_p, uint msg_len, struct sk_buff *skb, uint rx_len)
{

	skb_put(skb,rx_len);
	
	if (fe_test_rx_hook)
		fe_test_rx_hook(msg_p,skb);
	
	if(g_dump_flag&0x2)
		fe_test_dump_rx(msg_p,skb);
	
	dev_kfree_skb_any(skb);
	return 0;
}


//"flag" used for tx msg vlan control
static int fe_test_tx(struct sk_buff *skb,unsigned int type)
{
	ethTxMsg_t ethTxMsg; 

	unsigned int tpid = g_pkt_ctl.tpid ;
	
	ethTxMsg.msg[0] = 0;
	ethTxMsg.msg[1] = 0;
	
	if (g_pkt_ctl.check)
		ethTxMsg.msg[1] = 0xe0000000;
	
	
	
	if((type >> 31) & 0x01)
	{
		ethTxMsg.raw.vlanTag =  g_pkt_ctl.vid1;
		ethTxMsg.raw.vlanTpID = (tpid == 0x8100 ? 0 : (tpid== 0x88a8 ? 1 : 2) );	
		ethTxMsg.raw.vlanEn = 1;
	}
	
	if(fe_test_gdm)
	{
		ethTxMsg.raw.channel = 0;
		ethTxMsg.raw.queue =  0 ;//random32() % 8;
		ethTxMsg.raw.fPort = DPORT_GDMA2;
	}
	else
	{
		ethTxMsg.raw.channel = 0 ;// random32() % 4;
		ethTxMsg.raw.queue = 0 ;//random32() % 8;
		ethTxMsg.raw.fPort = DPORT_GDMA1;
	}
	
	g_fe_tx_q0++;

	if(fe_test_gdm)
		qdma_wan_transmit_packet(skb,ethTxMsg.msg[0],ethTxMsg.msg[1],0);
	else
		qdma_lan_transmit_packet(skb,ethTxMsg.msg[0],ethTxMsg.msg[1],0);
	
	return 0;
}



struct sk_buff * fe_test_allocate_skb(void)
{
	int offset = 0 ;
	struct sk_buff * skb = NULL ;
	
	
#if defined(TCSUPPORT_MAX_PACKET_2000)
	skb = skbmgr_alloc_skb4k();
#else
	skb = skbmgr_alloc_skb2k();
#endif
	
	
	offset = ((uint)(skb->tail) & 3) ;
	if(offset) {
		skb_reserve(skb, (3 - offset)) ;
	}

	return skb;
}



unsigned int fe_test_send_packet(unsigned int type,int len)
{
	struct sk_buff *skb;
	unsigned int i,cnt = 0;
	
	int fe_pkt_num = sizeof(g_fe_pkt) / sizeof(fe_test_data);
	
	for(i= 0; i< fe_pkt_num ;i++){

		if ((g_fe_pkt[i].type & type) != g_fe_pkt[i].type)
			continue;
		
		skb = fe_test_allocate_skb();
		if(skb==NULL)
		{
			printk("fe_test_send_packet: alloc skb err! \n");
			return cnt;
		}
		
		 if (fe_test_create_pkt(g_fe_pkt[i].type,len,skb) < 0)
		 {
			 dev_kfree_skb_any(skb); 
			 return cnt;

		 }

		if(g_dump_flag&0x1)
			fe_test_dump_skb(skb);

		cnt++;
		 
		fe_test_tx(skb,type);
	
		msleep(100);
	}
	return cnt;

}


int fe_test_dump(int flag)
{
	g_dump_flag = flag;

	return 0;
}


int fe_test_init(int idx){   //idx = 0 for GDM1 ,1 for GDM2 

	QDMA_InitCfg_t qdmaInitCfg;

	char smc[6] = {0x00,0x00,0x00,0x00,0x00,0x11};
	
	memset (&g_pkt_ctl,0,sizeof(fe_packet_ctl));

	memcpy(g_pkt_ctl.smac,smc,6);
	
	g_pkt_ctl.tpid = 0x8100;
	
	g_pkt_ctl.vid1 = 0x0011;
	
	g_pkt_ctl.vid2 = 0x0022;

	g_pkt_ctl.pppoeid = 0x3344;

	g_pkt_ctl.sport = 0x11;
	
	g_pkt_ctl.dport = 0x22;

	g_pkt_ctl.check = 1;

	memcpy(g_pkt_ctl.sip6,sip,16);
	memcpy(g_pkt_ctl.dip6,dip,16);

	
	//set GDM1 or GDM2 Loopback Mode 
	printk("FE Test Init: GDM%d \n",idx + 1);
	if (idx)
	{
		qdma_wan_loopback_mode(QDMA_LOOPBACK_UMAC);
		fe_reg_write(GDMA2_FWD_CFG,0x4444);
		qdma_wan_dma_mode(QDMA_DISABLE, QDMA_DISABLE, QDMA_DISABLE);
	}
	else
	{
		qdma_lan_loopback_mode(QDMA_LOOPBACK_UMAC);
		qdma_lan_dma_mode(QDMA_DISABLE, QDMA_DISABLE, QDMA_DISABLE);
		fe_reg_write(GDMA1_FWD_CFG,0x4444);
		fe_reg_write(0xbfb5b600,0x5e33b);
		fe_reg_write(GDMA1_LPBK_CFG,0x100fff03);	
		fe_reg_write(GDMA1_VLAN_CHECK,0);	
		fe_reg_write(CDMA1_VLAN_CTRL,0x81000000);
		
	}

    /* register QDMA callback functions */
    memset(&qdmaInitCfg, 0, sizeof(QDMA_InitCfg_t));
//	qdmaInitCfg.cbXmitFinish = NULL;
	qdmaInitCfg.cbRecvPkts = fe_test_rx;
//	qdmaInitCfg.cbEventHandler = NULL;
	if(idx)
	{
		qdma_wan_init(&qdmaInitCfg);
//		qdma_wan_receive_packet_mode(QDMA_RX_INTERRUPT);
		qdma_wan_dma_mode(QDMA_ENABLE, QDMA_ENABLE, QDMA_ENABLE); 
	}
    else
	{
		qdma_lan_init(&qdmaInitCfg);
//		qdma_lan_receive_packet_mode(QDMA_RX_INTERRUPT);
		qdma_lan_dma_mode(QDMA_ENABLE, QDMA_ENABLE, QDMA_ENABLE); 
	}

	fe_test_gdm = idx;
	
	return 0;
}

/////////////////////////////////////////////////////////////////////

int fe_test_clear_cnt(void)
{
	g_fe_rx_q0 = g_fe_rx_q1 = g_fe_tx_q0 = 0;
	return 0;
}

int fe_test_get_rx1(void)
{
	return g_fe_rx_q1;
}
int fe_test_get_tx0(void)
{
	return g_fe_tx_q0;

}

int fe_test_vip_rx(void *msg_p,struct sk_buff *skb)
{
	ethRxMsg_t* rxmsg = (ethRxMsg_t*)msg_p;
	
	if (rxmsg->resv == 0xbeafbeaf)
		g_fe_rx_q1 ++;
	else
		g_fe_rx_q0 ++;

	return  0;
}

/*
static fe_vip_packet g_vip[] = {
	{1,0,0,0,0,0x0806,0x0000},
	{1,1,0,0,0,0x0021,0x0000},
	{1,2,0,0,0,0x0001,0x0000},
	{1,3,1,0,0,0x0010,0x0020},
	{1,3,0,1,0,0x0010,0x0020},
	{1,3,1,1,0,0x0010,0x0020},
	{1,3,1,1,1,0x0010,0x0020},
	{1,4,1,0,0,0x0010,0x0020},
	{1,4,0,1,0,0x0010,0x0020},
	{1,4,1,1,0,0x0010,0x0020},
	{1,4,1,1,1,0x0010,0x0020},
};

*/


int fe_test_vip_t012(int idx,unsigned int type){

	fe_vip_packet fe_vip;

	memset(&fe_vip,0,sizeof(fe_vip_packet));

	if(FE_PACKET_IS_ARP(type)){
		fe_vip.type = 0;
		fe_vip.sport = 0x0806;
	}else if(FE_PACKET_IS_PPPOE_IPV4(type)){
		fe_vip.type = 1;
		fe_vip.sport = 0x0021;
	}else if(FE_PACKET_IS_PPPOE_IPV6(type)){
		fe_vip.type = 1;
		fe_vip.sport = 0x0057;
	}else if(FE_PACKET_IS_ICMP(type)){
		fe_vip.type = 2;
		fe_vip.sport = 0x01;
	}else if(FE_PACKET_IS_IGMP(type)){
		fe_vip.type = 2;
		fe_vip.sport = 0x02;
	}else
		goto dump;
	
	fe_set_vip_packet(idx,&fe_vip);
		
	fe_test_send_packet(type,10);
	
	return 0;;
	
dump:
	printk("fe_test_vip_t012: type = %08x",type);
	return 0;
}

int fe_test_vip_t34(int idx,unsigned int type){

	fe_vip_packet fe_vip;

	memset(&fe_vip,0,sizeof(fe_vip_packet));

	if(FE_PACKET_IS_TCP(type)){
		fe_vip.type = 3;
		fe_vip.sport = g_pkt_ctl.sport;
		fe_vip.dport = g_pkt_ctl.dport;	
	}else if(FE_PACKET_IS_UDP(type)){
		fe_vip.type = 4;
		fe_vip.sport = g_pkt_ctl.sport;
		fe_vip.dport = g_pkt_ctl.dport;		
	}else
		goto dump;
	
	fe_vip.sp_en = 1;
	
	fe_set_vip_packet(idx,&fe_vip);
		
	fe_test_send_packet(type,10);
		
//	fe_del_vip_packet(idx);

	fe_vip.sp_en = 0;
	fe_vip.dp_en = 1;
	fe_set_vip_packet(idx,&fe_vip);
		
	fe_test_send_packet(type,10);
		
//	fe_del_vip_packet(idx);
	
	fe_vip.sp_en = 1;
	fe_vip.dp_en = 1;
	fe_set_vip_packet(idx,&fe_vip);
		
	fe_test_send_packet(type,10);
		
//	fe_del_vip_packet(idx);

	fe_vip.sp_en = 1;
	fe_vip.dp_en = 1;
	fe_vip.sp_en = 1;
	fe_set_vip_packet(idx,&fe_vip);
		
	fe_test_send_packet(type,10);
		
//	fe_del_vip_packet(idx);
	
	return 0;;
dump:
	return 0;
}


int fe_test_vip(unsigned int num){

	int i = 0,j = 0,flag = 0;
	int fe_pkt_num = sizeof(g_fe_pkt) / sizeof(fe_test_data);
	unsigned int type = 0;
	
	fe_test_rx_hook = fe_test_vip_rx;

		i = num;
		for(j = 0; j < fe_pkt_num ; j++){
			
			fe_test_clear_cnt();

			type = g_fe_pkt[j].type;
				
			if(FE_PACKET_IS_ARP(type)){
				fe_test_vip_t012(i,type);
			}else if(FE_PACKET_IS_PPPOE_IPV4(type)){
				fe_test_vip_t012(i,type);
			}else if(FE_PACKET_IS_PPPOE_IPV6(type)){
				fe_test_vip_t012(i,type);
			}else if(FE_PACKET_IS_ICMP(type)){
				fe_test_vip_t012(i,type);
			}else if(FE_PACKET_IS_IGMP(type)){
				fe_test_vip_t012(i,type);
			}else if(FE_PACKET_IS_TCP(type)) {
				fe_test_vip_t34(i,type);
			}else if(FE_PACKET_IS_TCP(type)) {
				fe_test_vip_t34(i,type);
			}
			if (fe_test_get_tx0() != fe_test_get_rx1())
			{
				printk("Test VIP Packet: Entry(%d) Packet(%x) Send(%d) Recv(%d) \n",
				i,type,fe_test_get_tx0(),fe_test_get_rx1());
				flag = 1;
				goto exit;
			}
			printk("Test VIP Packet: Entry(%d) Type(%08x) OK \n",i,type);
		}
		fe_del_vip_packet(i);

exit:	
	fe_test_rx_hook = NULL;

	
	if(flag == 1)
		printk("FE VIP Test(%d): Fail \n",num );
	else
		printk("FE VIP Test(%d: Successful \n",num);
	
	return 0;
}

///////////////////////////////////////////////////////////////

static unsigned int g_offload_rx = 0; 
int fe_test_offload_pppoe_rx(void *msg_p,struct sk_buff *skb)
{
	unsigned int pppoeid;
	
	if(skb->data[12] != 0x88 ||  skb->data[13] != 0x64)
		goto dump;

	pppoeid = (skb->data[16] << 8) |  skb->data[17];
	
	if(pppoeid != g_pkt_ctl.pppoeid)
		goto dump;
	
	g_offload_rx++;
	
	return 0;

dump:
	printk("fe_test_offload_pppoe_rx: \n");
	fe_test_dump_skb(skb);	
	return 0;	
}



int fe_test_offload_pppoe(void)
{
	unsigned int val,reg;
	unsigned int mask = 0x00ff061f;
	unsigned int cnt = 0;
	unsigned int pppoeid = 0x1234;
	
	if (fe_test_gdm)
		reg = 0xbfb51404;
	else
		reg = 0xbfb50404;	

	g_offload_rx= 0;
	
	fe_test_rx_hook = fe_test_offload_pppoe_rx;

	g_pkt_ctl.pppoeid = pppoeid;
		
	val =  fe_reg_read(reg);
	
	fe_reg_write(reg,(0x10000 | pppoeid));

	cnt += fe_test_send_packet(mask,0);
	
	fe_reg_write(reg,val);	
	
	fe_test_rx_hook = NULL;
	
	if(cnt == g_offload_rx)
		printk("PPPOE Insert Test: OK \n");
	else
		printk("PPPOE Insert Test: Err \n");
	
	return 0;
}

int fe_test_offload_padding_rx(void *msg_p,struct sk_buff *skb)
{
	if(skb->len < 56)
		goto dump;

	g_offload_rx++;
	
	return 0;
dump:
	fe_test_dump_skb(skb);	
	return 0;	
}

int fe_test_offload_padding(void)
{
	unsigned int val,reg,cnt = 0;

	if (fe_test_gdm)
		reg = 0xbfb51408;
	else
		reg = 0xbfb50408;	
	
	g_offload_rx = 0;

	fe_test_rx_hook = fe_test_offload_padding_rx;
	
	val =  fe_reg_read(reg);
	
	fe_reg_modify_bits(reg,1,0,1);

	cnt += fe_test_send_packet(0x00ffffff,0);
	
	fe_reg_write(reg,val);
	
	fe_test_rx_hook = NULL;
	
	if(g_offload_rx == cnt )
		printk("TX Padding Test: OK ! \n");
	else
		printk("TX Padding Test: Err! \n");
	
	return 0;

	
}


int fe_test_offload_vtag_rx(void *msg_p,struct sk_buff *skb)
{

	unsigned int tpid,vid;

	tpid = (skb->data[12] << 8) | skb->data[13];
	vid = (skb->data[14] << 8) | skb->data[15];

	if(tpid != g_pkt_ctl.tpid)
		goto dump;

	if(vid !=  g_pkt_ctl.vid1)
		goto dump;
	
	g_offload_rx++;
	
	return 0;
dump:
	fe_test_dump_skb(skb);	
	return 0;
}

int fe_test_offload_vtag(void)
{
	unsigned int val0,reg0,val1,reg1,flag = 0;
	unsigned int cnt = 0;
	if (fe_test_gdm)
	{
		reg0 = 0xbfb51400;
		reg1 = 0xbfb51510;
	}
	else
	{
		reg0 = 0xbfb50400;	
		reg1 = 0xbfb50510;
	}

	fe_test_rx_hook = fe_test_offload_vtag_rx;
	
	val0 =  fe_reg_read(reg0);
	val1 =  fe_reg_read(reg1);

	fe_reg_modify_bits(reg0,0x9100,16,16);

	g_pkt_ctl.tpid = 0x8100;
	g_offload_rx = 0;
	cnt = fe_test_send_packet(0x80fffe1f,0);   //insert 0x8100
	if (g_offload_rx != cnt)
		flag = 1;

	g_pkt_ctl.tpid = 0x88a8;
	g_offload_rx = 0;
	cnt = fe_test_send_packet(0x80fffe1f,0);  //insert 0x88a8
	if (g_offload_rx != cnt)
		flag = 1;
	
	g_pkt_ctl.tpid = 0x9100;
	g_offload_rx = 0;
	cnt = fe_test_send_packet(0x80fffe1f,0); //insert 0x9100
	if (g_offload_rx != cnt)
		flag = 1;

	
	fe_reg_write(reg0,val0);
	fe_reg_write(reg1,val1);
	g_pkt_ctl.tpid = 0x8100;		
	fe_test_rx_hook = NULL;
	
	if(flag == 0 )
		printk("VLAN TPID Test: OK ! \n");
	else
		printk("VLAN TPID Test: Err! \n");
	
	return 0;
}


int fe_test_offload_untag_rx(void *msg_p,struct sk_buff *skb)
{
	unsigned int tpid,vid;
	ethRxMsg_t* msg;

	tpid = (g_pkt_ctl.tpid == 0x8100) ? 0 : ((g_pkt_ctl.tpid == 0x88a8) ? 1:2);

	msg = (ethRxMsg_t*)msg_p;

	vid = g_vlan_header.vid1;


	if(msg->rxMsgW2.vlanEn == 0 || msg->rxMsgW2.vlanTpID != tpid)
		goto dump;

	if(vid != msg->rxMsgW2.vlanTag)
		goto dump;
	
	g_offload_rx++;
	
	return 0;

dump:
	printk("fe_test_offload_untag_rx: rx msg(%08x,%08x,%08x) \n",msg->rxMsgW0,msg->rxMsgW1,msg->rxMsgW2);
	printk("fe_test_offload_untag_rx: tpid(%08x) vid(%08x) \n",tpid,vid);
	printk("fe_test_offload_untag_rx: msg (%08x,%08x,%08x)\n",msg->rxMsgW2.vlanEn ,msg->rxMsgW2.vlanTpID,msg->rxMsgW2.vlanTag);
	fe_test_dump_skb(skb);	
	return 0;
}

int fe_test_offload_untag(void)
{
	unsigned int val,reg,flag = 0,cnt =0;
	
	if (fe_test_gdm)
		reg = 0xbfb51400;
	else
		reg = 0xbfb50400;	
	
	fe_test_rx_hook = fe_test_offload_untag_rx;
	
	val =  fe_reg_read(reg);
	fe_reg_modify_bits(reg,1,1,1);
	fe_reg_modify_bits(0xbfb50000,0x9100,16,16);

	g_pkt_ctl.tpid = 0x8100;
	g_offload_rx = 0;
	cnt = fe_test_send_packet(0x00f072f,0);
	if (g_offload_rx != cnt)
		flag =1;

	g_pkt_ctl.tpid = 0x88a8;
	g_offload_rx = 0;
	cnt = fe_test_send_packet(0x00f072f,0);
	if (g_offload_rx != cnt)
		flag =1;	
	
	g_pkt_ctl.tpid = 0x9100;
	g_offload_rx = 0;
	cnt = fe_test_send_packet(0x00f072f,0);
	if (g_offload_rx != cnt)
		flag =1;
	
	fe_reg_write(reg,val);
	fe_reg_modify_bits(0xbfb50000,0x8100,16,16);
	
	fe_test_rx_hook = NULL;
	g_pkt_ctl.tpid = 0x8100;
	if(flag == 0 )
		printk("RX Untag Test: OK ! \n");
	else
		printk("RX Untag Test: Err! \n");
	
	return 0;

	
}



int fe_test_offload(void)
{
	unsigned int val, reg ;

	printk("Start Offload Test: GDM%d\n",fe_test_gdm + 1);

	
//test pppoe insert
	fe_test_offload_pppoe();

//test padding
	if (fe_test_gdm == 0)
		fe_test_offload_padding();

//test vlan tpid
	fe_test_offload_vtag();

//test vlan untag
	fe_test_offload_untag();

	return 0;
}

////////////////////////////////////////////////////////////////

int fe_test_check_mib(void)
{
	unsigned int rx_reg,tx_reg,rx_num,tx_num;
	unsigned int i,value, flag = 0;;
	
	if (fe_test_gdm)
	{
		rx_reg = 0xbfb51600;
		rx_num = 17;
		tx_reg = 0xbfb51648;
		tx_num = 22;
	}
	else
	{
		rx_reg = 0xbfb50600;
		rx_num = 4;
		tx_reg = 0xbfb50640;
		tx_num = 6;
	}

	for(i = 0; i< rx_num; i++)
	{
	 	if (value = fe_reg_read(rx_reg + i*4))
		{
			printk("Reg(%08x): %08x \n",(rx_reg + i*4),value);
				flag = 1;
		}
	}
	

	for(i = 0; i< tx_num; i++)
	{
	 	if(value = fe_reg_read(tx_reg + i*4))
		{
			printk("Reg(%08x): %08x \n",(tx_reg + i*4),value);
			flag = 1;
		}
	}
	
	
	return flag;	
}


int fe_test_clear_mib(void)
{
	if (fe_test_gdm)
		fe_clear_mib_counter(MT7512_FE_GDM2);
	else
		fe_clear_mib_counter(MT7512_FE_GDM1);

	return 0;
}


int fe_test_mib(void)
{
	fe_test_clear_mib();
	if (fe_test_check_mib())
		printk("Test MIB Clear: Fail \n");	
	 else
		printk("Test MIB Clear: Successful \n");
	return 0;

}
////////////////////////////////////////////////////////////////
unsigned int fe_test_get_errcnt(void)
{
	unsigned int reg;
	
	if (fe_test_gdm)
		reg = GDMA2_RX_ERRDROPCNT;
	else
		reg = GDMA1_RX_ERROR_DROP_CNT;
	

	return fe_reg_read(reg);
}

int fe_test_error_type(unsigned int type,unsigned int mask,int len)
{
	unsigned int txcnt,ecnt;
	char* str[] = {"udp","tcp","ip","crc","short","long"};


	fe_test_clear_mib();

	if (type == 2)
		txcnt = fe_test_send_packet((mask<<16) | 0x02ff,len); //IPv4 only
	else 
		txcnt = fe_test_send_packet((mask<<16) | 0x06ff,len); //IPv4 & IPv6
	
	ecnt = fe_test_get_errcnt();
	if (txcnt == ecnt)
		printk("Test (%s) Error: Tx(%d) Rx(%d) Ok \n",str[type],txcnt,ecnt);
	else
		printk("Test (%s) Error: Tx(%d) Rx(%d) Fail \n",str[type],txcnt,ecnt);

	fe_test_clear_mib();

	if (type == 2)
		txcnt = fe_test_send_packet((mask<<16) | 0x02ff,len); //IPv4 only
	else 
		txcnt = fe_test_send_packet((mask<<16) | 0x18ff,len); //IPv4 & IPv6 over PPPOE

	ecnt = fe_test_get_errcnt();
	if (txcnt == ecnt)
		printk("Test (%s) Error (pppoe): Tx(%d) Rx(%d) Ok \n",str[type],txcnt,ecnt);
	else
		printk("Test (%s) Error (pppoe): Tx(%d) Rx(%d) Fail \n",str[type],txcnt,ecnt);
	
	
	return 0;
}

int fe_test_error_udp(void)
{
	unsigned int reg,value;
		
	if (fe_test_gdm)
		reg = GDMA2_FWD_CFG;
	else
		reg = GDMA1_FWD_CFG;	

	value = fe_reg_read(reg);
	
	fe_reg_modify_bits(reg,1,20,1);

	fe_test_error_type(0,8,0);
	
	fe_reg_write(reg,value);	
	
	return 0;
}

int fe_test_error_tcp(void)
{
	unsigned int reg,value;
		
	if (fe_test_gdm)
		reg = GDMA2_FWD_CFG;
	else
		reg = GDMA1_FWD_CFG;	

	value = fe_reg_read(reg);
	
	fe_reg_modify_bits(reg,1,21,1);

	fe_test_error_type(1,4,0);

	fe_reg_write(reg,value);
	
	return 0;
}

int fe_test_error_ip(void)
{
	unsigned int reg,value;
		
	if (fe_test_gdm)
		reg = GDMA2_FWD_CFG;
	else
		reg = GDMA1_FWD_CFG;	

	value = fe_reg_read(reg);
	
	fe_reg_modify_bits(reg,1,22,1);

	fe_test_error_type(2,3,0);

	fe_reg_write(reg,value);
	
	return 0;
}


int fe_test_error_short(void)
{
	unsigned int reg,value;
	unsigned int lreg,leng;
		
	if (fe_test_gdm)
	{
		reg = GDMA2_FWD_CFG;
		lreg = GDMA2_LEN_CFG;
	}
	else
	{
		reg = GDMA1_FWD_CFG;	
		lreg = GDMA1_LEN_CFG;
	}

	value = fe_reg_read(reg);
	leng = fe_reg_read(lreg);

	fe_reg_modify_bits(reg,1,24,1);
	fe_reg_modify_bits(lreg,100,0,14);

	fe_test_error_type(4,0x0f,0);

	fe_reg_write(reg,value);	
	fe_reg_write(lreg,leng);	
	return 0;
}

int fe_test_error_long(void)
{
	unsigned int reg,value;
	unsigned int lreg,leng;
		
	if (fe_test_gdm)
	{
		reg = GDMA2_FWD_CFG;
		lreg = GDMA2_LEN_CFG;
	}
	else
	{
		reg = GDMA1_FWD_CFG;	
		lreg = GDMA1_LEN_CFG;
	}

	value = fe_reg_read(reg);
	leng = fe_reg_read(lreg);

	fe_reg_modify_bits(reg,1,25,1);
	fe_reg_modify_bits(lreg,1000,16,14);

	fe_test_error_type(5,0x0f,1000);

	fe_reg_write(reg,value);
	fe_reg_write(lreg,leng);	

	return 0;
}

int fe_test_error(void)
{
	printk("FE Test Error Packet: GDM%d\n",fe_test_gdm + 1);

	g_pkt_ctl.check = 0;

//IP checksum error
	fe_test_error_ip();

//TCP checksum error
	fe_test_error_tcp();

//UDP checksum error
	fe_test_error_udp();

//Short length error
	fe_test_error_short();

//long length error
	fe_test_error_long();

	g_pkt_ctl.check = 1;

	return 0;
}
////////////////////////////////////////////////////////////////////
int fe_test_interrupt_0_5(unsigned int irq)
{
	unsigned int i, reg,value;

	fe_reg_write(FE_INT_STATUS,1<<irq);
	
	if (irq == 0)
		reg = GDMA1_TXCHN_EN;
	else
		reg = GDMA2_TXCHN_EN;

	value = fe_reg_read(reg);

	fe_reg_write(PSE_IQ_REV1,0);
	fe_reg_write(PSE_IQ_REV2,0);		
	
	fe_reg_write(reg,0);
	
	fe_test_send_packet(0x10211,0x40);
	
	if (fe_reg_read(FE_INT_STATUS) & (1<<irq))
		printk("FE Interrupt: IRQ(%d) OK\n",irq);
	else		
		printk("FE Interrupt: IRQ(%d) Fail\n",irq);

	fe_reg_write(reg,value);
	
	return 0;
}
int fe_test_interrupt_1_2(unsigned int irq)
{
	unsigned int reg,value;

	fe_reg_write(FE_INT_STATUS,1<<irq);
	
	if (irq == 1)
		reg = 0xbfb50804;
	else
		reg = 0xbfb51804;

	value = fe_reg_read(reg);
	
	fe_reg_write(reg,value&0xfffffffb);

	fe_reg_write(PSE_IQ_REV1,0x02);
	
	fe_reg_write(PSE_IQ_REV2,0x0200);	

	fe_test_send_packet(0x10211,0x40);

	if (fe_reg_read(FE_INT_STATUS) & (1<<irq))
		printk("FE Interrupt: IRQ(%d) OK\n",irq);
	else		
		printk("FE Interrupt: IRQ(%d) Fail\n",irq);

	fe_reg_write(reg,value);
	
	return 0;
}

int fe_test_interrupt_8(unsigned int irq)
{
	unsigned int i,val;

	fe_reg_write(FE_INT_STATUS,1<<irq);


	val = (fe_reg_read(PSE_FQFC_CFG) >> 16) & 0xff;
	
	fe_reg_write(PSE_IQ_REV1,val);
	fe_reg_write(PSE_IQ_REV2,0);

	fe_reg_write(GDMA1_TXCHN_EN,0);
	
	for(i=0;i<80;i++)
		fe_test_send_packet(0x10211,0x400);

	if (fe_reg_read(FE_INT_STATUS) & (1<<irq))
		printk("FE Interrupt: IRQ(%d) OK\n",irq);
	else		
		printk("FE Interrupt: IRQ(%d) Fail\n",irq);

	fe_reg_write(GDMA1_TXCHN_EN,0xffffffff);

	return 0;
}


int fe_test_interrupt(unsigned int irq)
{
	unsigned int val1,val2;

	val1 = fe_reg_read(PSE_IQ_REV1);
	val2 = fe_reg_read(PSE_IQ_REV2);

	if (irq == 0 || irq == 5)
		fe_test_interrupt_0_5(irq);
	else if (irq == 1 || irq == 2)
		fe_test_interrupt_1_2(irq);
	else if (irq == 8)
		fe_test_interrupt_8(irq);


	fe_reg_write(PSE_IQ_REV1,val1);
	fe_reg_write(PSE_IQ_REV2,val2);		
	return 0;
}


///////////////////////////////////////////////////////////////////


int fe_test_check_buffer(unsigned int in,unsigned int out)
{
	unsigned int input,output,max_input;

	if (in < 4)
		input = (fe_reg_read(PSE_IQ_STA1) >>  (in*8)) &0xff;
	else
		input = (fe_reg_read(PSE_IQ_STA2) >>  ((in-4)*8)) &0xff;


	if (out < 4)
		output = (fe_reg_read(PSE_OQ_STA1) >>  (out*8)) &0xff;
	else
		output = (fe_reg_read(PSE_OQ_STA2) >>  ((out-4)*8)) &0xff;

	
	if (in < 4)
		max_input = (fe_reg_read(PSE_IQ_MAX1) >>  (in*8)) &0xff;
	else
		max_input = (fe_reg_read(PSE_IQ_MAX2) >>  ((in-4)*8)) &0xff;

	if (input != output || input != max_input)
	{
		printk("FE Test Buffer(%d->%d): Input/Output/Max(%d %d %d) Fail! \n",in,out,input,output,max_input);
		return 0;
	}
	printk("FE Test Buffer(%d->%d): Input/Output/Max(%d %d %d)! \n",in,out,input,output,max_input);	
	return 1;
}



int fe_test_buffer_io(unsigned int in,unsigned int out)
{
	unsigned int res = 0;
	unsigned int i;

	if (in < 4)
		res = (fe_reg_read(PSE_IQ_REV1) >>  (in*8)) &0xff;
	else
		res =  (fe_reg_read(PSE_IQ_REV2) >> ((in-4)*8) ) &0xff;

	for(i=0;i<res;i++)
	{
		fe_test_send_packet(ICMP_IPV4_0VLAN_OC,300);
		if (fe_test_check_buffer(in,out)==0)
		{
			return 0;
		}
	}
	printk("FE test Buffer(P%d->p%d): OK! Res(%d) \n",in,out,res);
	return 0;
}

int fe_test_buffer_mibclear(void)
{
	fe_reg_write(FE_CNT_CFG,1);

	msleep(1);
	
	return 0;
}


int fe_test_buffer(unsigned int in,unsigned int out)
{
	printk("FE Test Buffer: \n");
	fe_test_buffer_mibclear();
	fe_test_buffer_io(in,out);

	return 0;
}

/////////////////////////////////////////////////////////////////


int fe_test_retire_all(unsigned int idx)
{
	unsigned int mask = 0,chn = 0;
	
	if (idx == MT7512_FE_GDM1)
		mask = fe_reg_read(GDMA1_TX_CHN_VLD);
	else
		mask = fe_reg_read(GDMA2_TX_CHN_VLD);

	while(mask && chn < 32)
	{
		if (mask & 0x1)
			fe_set_channel_retire(idx,chn,0);
		mask = (mask >> 1);
		chn++;
	}
	return 0;
}

int fe_test_channel_retire(unsigned int idx,unsigned int chn)
{
	unsigned int reg,reg1,index, value;
	
	if (idx)
	{
		reg = GDMA2_TXCHN_EN;
		index = MT7512_FE_GDM2;
	}
	else
	{
		reg = GDMA1_TXCHN_EN;
		index = MT7512_FE_GDM1;
	}
	
	
	value = fe_reg_read(reg);
	
	if (chn < 32)
		value &= ~(1 << chn);
	else
		value = 0;
	
	fe_reg_write(reg,value);
		
	mdelay(10);

	printk("Test GDM%d Channel Retire: Tx Valid(%x) \n",idx+1,
				idx ? fe_reg_read(GDMA2_TX_CHN_VLD) : fe_reg_read(GDMA1_TX_CHN_VLD) );

	if (chn < 32)
		fe_set_channel_retire(index,chn,0);
	else
		fe_test_retire_all(index);


	if (chn < 32)
		value |=  (1 << chn);
	else
		value = 0xffffffff;
	
	fe_reg_write(reg,value);

		
	return 0;
}

///////////////////////////////////////////


static int g_l2lu_index = 0;
static int g_l2lu_entry_index = 0;

int fe_test_set_l2lu(unsigned int idx,unsigned int type)
{
	fe_l2lu_key key;
	unsigned int vid1,vid2,prior,*ptr;

	ptr = (unsigned int*) &key;
	vid1 = g_pkt_ctl.vid1;
	vid2 = g_pkt_ctl.vid2;
	prior =  g_pkt_ctl.prior;
	
	memset(&key,0,sizeof(key));

	if (FE_PACKET_IS_1CVLAN(type)){
		key.cvid = vid1 & 0xfff;
		key.cpcp = (vid1 >> 13) & 0x7;
	}else if (FE_PACKET_IS_1SVLAN(type)){
		key.svid = vid1 & 0xfff;
		key.spcp = (vid1 >> 13) & 0x7;
	}else if (FE_PACKET_IS_2VLAN(type)){
		key.svid = vid1 & 0xfff;
		key.spcp = (vid1 >> 13) & 0x7;
		key.cvid = vid2 & 0xfff;
		key.cpcp = (vid2 >> 13) & 0x7;
	}
	
	key.dscp = prior;
	if (FE_PACKET_IS_IPV4(type))
		key.dscp_mask = 0x3f;
	
	if (FE_PACKET_IS_IPV6(type))
		key.dscp_mask = 0xff;

	fe_set_l2lu_key(idx,&key);
	
	printk("KEY0(%08x) KEY1(%08x)",fe_reg_read(PSE_L2LU_KEY0(idx)),fe_reg_read(PSE_L2LU_KEY1(idx)));
	
	return 0;
}

int fe_test_l2lu_rx(void *msg_p,struct sk_buff *skb)
{
	static int cnt = 0,new_index;
	ethRxMsg_t*  ptr = (ethRxMsg_t*)msg_p;

	cnt++;
	
	new_index = ptr->rxMsgW1.entry;
	
	if (cnt & 1)
		g_l2lu_entry_index = new_index;
	else{
		if ((g_l2lu_entry_index + (g_l2lu_index<<1)) == new_index)
			printk(" OK (%d,%d) \n",g_l2lu_entry_index,new_index);
		else
			printk(" Fail (%d,%d) \n",g_l2lu_entry_index,new_index);
	}

	return 0;
}


int fe_test_l2lu(unsigned int num,unsigned int len)
{
	int i = 0,j = 0;
	int fe_pkt_num = sizeof(g_fe_pkt) / sizeof(fe_test_data);
	
	fe_test_rx_hook = fe_test_l2lu_rx;
	g_pkt_ctl.check = 1;
	g_pkt_ctl.vid1 = 0x2011;
	g_pkt_ctl.vid2 = 0x4022;
	g_pkt_ctl.prior = 0x55;	
	
//	for(i = 0;i <  FE_L2LU_KEY_NUM ; i++){
		g_l2lu_entry_index = 0;
		g_l2lu_index = num + 1;
		i = num;
		for(j = 0; j < fe_pkt_num ; j++){

			if (FE_PACKET_IS_IPV4(g_fe_pkt[j].type) ||  FE_PACKET_IS_IPV6(g_fe_pkt[j].type))
			{
				printk("GDM%d L2LU(%d): Type(%x)",fe_test_gdm +1,num,g_fe_pkt[j].type);
				fe_del_l2lu_key(i);
				fe_test_send_packet(g_fe_pkt[j].type,len);
				fe_test_set_l2lu(i,g_fe_pkt[j].type);
				fe_test_send_packet(g_fe_pkt[j].type,len);
			}		
		}
		fe_del_l2lu_key(i);
//	}
//	g_pkt_ctl.check = 0;
	fe_test_rx_hook = NULL;
	return 0;

}



///////////////////////////////////////////////////////

static int g_pcp_idx = 0; 
static int g_pcp_coding[8][16] = { 
  {0,0,2,2,4,4,6,6,8,8,10,10,12,12,14,14},    //encoding 8P0D
  {0,0,2,2,4,4,6,6,10,8,10,8,12,12,14,14},    //encoding 7P1D
  {0,0,2,2,6,4,6,4,10,8,10,8,12,12,14,14},    //encoding 6P2D
  {2,0,2,0,6,4,6,4,10,8,10,8,12,12,14,14},    //encoding 5P3D

  {0,0,2,2,4,4,6,6,8,8,10,10,12,12,14,14},    //decoding 8P0D
  {0,0,2,2,4,4,6,6,9,9,8,8,12,12,14,14},       //decoding 7P1D
  {0,0,2,2,5,5,4,4,9,9,8,8,12,12,14,14},       //decoding 6P2D
  {1,1,0,0,5,5,4,4,9,9,8,8,12,12,14,14}        //decoding 5P3D
};


int fe_test_pcp_rx(void *msg_p,struct sk_buff *skb)
{
	unsigned int pcp, old;
	fe_vlan_header_t*  pvlan;

	pvlan = (fe_vlan_header_t*)(skb->data+12);

	old = (g_pkt_ctl.vid1 >> 12) & 0xf;

	pcp = (pvlan->vid1 >> 12) & 0xf;

	if (pvlan->tpid1 != 0x8100 && pvlan->tpid1 != 0x88a8)
		goto Error;

	if (pcp !=  g_pcp_coding[g_pcp_idx][old])
		goto Error;
	
	printk("PCP OK:  Send(%x)  Expected(%x) Received(%x) \n",old,g_pcp_coding[g_pcp_idx][old],pcp);
	return 0;
Error:
	printk("PCP Error:  Send(%x)  Expected(%x) Received(%x) \n",old,g_pcp_coding[g_pcp_idx][old],pcp);
	return 0;
}


int fe_test_pcp(int idx,unsigned int mask)
{
	unsigned int reg;
	unsigned int i,j,off;
	
	if (idx == 0){
		reg = GDMA1_LAN_PCP; off = 0;
	}else if (idx == 1){
		reg = GDMA1_LAN_PCP; off = 8;
	}else if (idx == 2){
		reg = GDMA2_WAN_PCP; off = 0;
	}else if (idx == 3){
		reg = GDMA2_WAN_PCP; off = 8;
	}else 
		return 0;

	fe_test_rx_hook = fe_test_pcp_rx;
	
	for(i=0;i<8;i++)
	{
		printk("Test PCP Coding(%s%d): %s (%dP%dD) \n",idx%2 ? "GDM":"CDM",(idx>>1) + 1,i < 4 ? "Encoding":"Decoding",(8-i%4),(i%4));
		g_pcp_idx = i;
		fe_reg_write(reg,1<<(i+off));
		printk("Write Reg: %x %x \n",reg,1<<(i+off));
		for(j=0;j<16;j++)
		{
			g_pkt_ctl.vid1 = (j<<12) |  0x11;
			fe_test_send_packet(mask,10);
		}
	}
	
	fe_test_rx_hook = NULL;
	
	return 0;
}



//////////////////////////////////////////////////////////////////
static int g_misc_rx = 0;
int fe_test_misc_rx(void *msg_p,struct sk_buff *skb)
{
	ethRxMsg_t*  ptr = (ethRxMsg_t*)msg_p;

	if (ptr->rxMsgW1.rsn == 0x7)
		g_misc_rx++;
	
	return 0;
}

int fe_test_misc(unsigned int mask,unsigned int len)
{
	int i,cnt;
	unsigned char opt[5] = {0,43,50,51,60};
	g_misc_rx = 0;
	fe_test_rx_hook = fe_test_misc_rx;


	for (i=0; i< 5; i++){	
		fe_reg_write(FE_IPV6_EXT,opt[i]);
		g_pkt_ctl.opt6 = (opt[i] << 8) | 1;
		cnt = fe_test_send_packet(mask,len);
		if (g_misc_rx == cnt )
			printk("FE IPv6 OPT Test: OPT Type(%x) OK \n",fe_reg_read(FE_IPV6_EXT));
		else
			printk("FE IPv6 OPT Test: OPT Type(%x) Fail \n",fe_reg_read(FE_IPV6_EXT));
	}
	
	fe_test_rx_hook = NULL;

	fe_reg_write(FE_IPV6_EXT,0);
	
	g_pkt_ctl.opt6 = 0;
	
	return 0;
}

/////////////////////////////////////////////////////////////
static unsigned int g_cpu_rx_old_pps = 0;
static unsigned int g_cpu_rx_curr_pps = 0;
static unsigned int g_cpu_rx_old_bps = 0;
static unsigned int g_cpu_rx_curr_bps = 0;
static struct timer_list g_cpu_timer;
static unsigned int g_cpu_rx_period = 0;
static unsigned int g_cpu_rx_idx = 0;

static void fe_test_cpu_protect_timer(unsigned long data)
{
	unsigned int pps,bps;

	if (g_cpu_rx_period == 0)
	{
		fe_test_rx_hook = NULL;
		return;
	}
	
	g_cpu_rx_period--;
	
	pps = g_cpu_rx_curr_pps >= g_cpu_rx_old_pps ? 
		(g_cpu_rx_curr_pps - g_cpu_rx_old_pps) : (~g_cpu_rx_old_pps) + g_cpu_rx_curr_pps;

	bps = g_cpu_rx_curr_bps >= g_cpu_rx_old_bps ?
		(g_cpu_rx_curr_bps - g_cpu_rx_old_bps) : (~g_cpu_rx_old_bps) + g_cpu_rx_curr_bps;
	
	printk("FE Test CPU Rx Rate(%d): %dPps %dKBps \n",g_cpu_rx_period,pps,bps/1000);

	g_cpu_rx_old_pps = g_cpu_rx_curr_pps;
	
	g_cpu_rx_old_bps = g_cpu_rx_curr_bps;
	
  	g_cpu_timer.expires = jiffies + msecs_to_jiffies(1000);
	
	add_timer(&g_cpu_timer);
	
	return;
}

int fe_test_cpu_protect_rx(void *msg_p,struct sk_buff *skb)
{
	ethRxMsg_t* rxmsg = (ethRxMsg_t*)msg_p;

	if (g_cpu_rx_period == 0)
	{
		mdelay(1);
		return 0;
	}
	
	if ( (rxmsg->resv == 0xbeafbeaf && g_cpu_rx_idx == 1)
		|| (rxmsg->resv != 0xbeafbeaf && g_cpu_rx_idx == 0))
	{
		g_cpu_rx_curr_pps++;
		g_cpu_rx_curr_bps += skb->len;
	}
	
	return 0;
}

int fe_test_cpu_protect(unsigned int idx,unsigned int tm)
{
	g_cpu_rx_curr_pps = 0;
	g_cpu_rx_curr_bps = 0;
	g_cpu_rx_old_pps = 0;
	g_cpu_rx_old_bps = 0;
	g_cpu_rx_period = tm;
	g_cpu_rx_idx = idx;
	
	fe_test_rx_hook = fe_test_cpu_protect_rx;

	if (tm > 0)
	{
		init_timer(&g_cpu_timer);
		
	    g_cpu_timer.expires = jiffies + msecs_to_jiffies(1000);
		
	    g_cpu_timer.function = fe_test_cpu_protect_timer;
		
	    g_cpu_timer.data = 0;
		
	    add_timer(&g_cpu_timer);
	}

	return 0;
}

////////////////////////////////////////////////////////////
static int fe_isr_on = 0;
static irqreturn_t fe_test_isr(int irq, void *dev_id)
{
	unsigned int status ;

	status = read_reg_word(0xbfb50008);
	if (status == 0)
		return IRQ_NONE;
	printk("\n fe_isr: status = %x",status);
	
	write_reg_word(0xbfb50008,status);
	return IRQ_HANDLED;
}

 int  fe_test_register_isr(void)
{
	if (request_irq(22,fe_test_isr,IRQF_SHARED,"fe_isr",2) != 0)
	{
		printk("fe register isr error \n");
	}
	write_reg_word(0xbfb5000c,0xffffffff);
	fe_isr_on = 1;
	return 0;
}
 int fe_test_unregister_isr(void)
{
	 write_reg_word(0xbfb5000c,0);
	if (fe_isr_on)
		free_irq(22,2);
	fe_isr_on = 0;
	return 0;
}

int fe_test_send(unsigned int type,unsigned int len)
{
	fe_test_send_packet(type,len);
	return 0;
}


