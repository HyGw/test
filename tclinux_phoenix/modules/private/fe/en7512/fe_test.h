


#ifndef _FE_TEST_H
#define _FE_TEST_H

/* used for FE index */ 
#define MT7512_FE_GDM1 	0
#define MT7512_FE_GDM2 	1
#define MT7512_FE_CDM1 	2
#define MT7512_FE_CDM2 	3
    
     
/* used for WAN Rx rate limiit */
#define FE_WAN_RX_UC_RATE 	0
#define FE_WAN_RX_BC_RATE 	1
#define FE_WAN_RX_MC_RATE 	2
#define FE_WAN_RX_OC_RATE 	3

/************************************************************************
*               M A C R O S
*************************************************************************
*/

/************************************************************************
*               D A T A   T Y P E S
*************************************************************************
*/


#define FE_VIP_TYPE_ETHER		 0
#define FE_VIP_TYPE_PPPOE 	1
#define FE_VIP_TYPE_IP 		2
#define FE_VIP_TYPE_TCP 		3
#define FE_VIP_TYPE_UDP 		4

#define FE_PCP_8P0D	1
#define FE_PCP_7P1D	2
#define FE_PCP_6P2D	3
#define FE_PCP_5P3D	4

#define PSE_IRQ_NUMBER		22

#define FE_VIP_PACKET_NUM  	32

#define FE_L2LU_KEY_NUM 	15

typedef struct fe_vip_packet_s{
	unsigned short int en: 1;
	unsigned short int type: 3;
	unsigned short int sp_en: 1;
	unsigned short int dp_en: 1;
	unsigned short int sw_en: 1;
	unsigned short int res: 9;
	union{	
		unsigned short sport;
		unsigned short etype;
		unsigned short ppp;
		unsigned short iptype;
	};
	unsigned short dport;	
} fe_vip_packet;




 typedef struct fe_l2lu_key_s{
	unsigned int dscp_mask: 8;
	unsigned int cvid_en: 1;
	unsigned int cpcp_en: 1;
	unsigned int res0: 2;
	unsigned int svid_en: 1;
	unsigned int spcp_en: 1;
	unsigned int res1: 2;	 
	unsigned int dscp: 16;
	 
	unsigned int cvid: 12;
	unsigned int res2: 1;	
	unsigned int cpcp: 3;
	unsigned int svid: 12;
	unsigned int res3: 1;	 
	unsigned int spcp: 3;	
 } fe_l2lu_key;


typedef struct fe_buff_s{
	unsigned int p0 : 8;
	unsigned int p1 : 8;
	unsigned int p2 : 8;
	unsigned int p4 : 8;
	unsigned int p5 : 8;
	unsigned int vp1 : 8;
	unsigned int vp2 : 8;
}fe_buff;

typedef struct fe_vlan_ctrl_s{
	unsigned int sp_en: 1;
	unsigned int rx_untag: 1;
	unsigned int res: 14;
	unsigned int tpid: 16;	
}fe_vlan_ctrl;

#endif
