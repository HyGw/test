/*********************************************************************************
 * decleration and function prototype for igmp module
 *
 * Copyright (C) 2015 Econet Technologies, Corp.
 * All Rights Reserved.
 *
 *********************************************************************************/
#ifndef ECNT_IGMP_H_
#define	ECNT_IGMP_H_

#ifndef uint8
#define uint8	unsigned char
#endif
	
#ifndef uint16
#define uint16	unsigned short
#endif
	
#ifndef uint32
#define uint32	unsigned long
#endif

#define PROTOCOL_UDP 0x11
#define PROTOCOL_UDP6 0x11

#define  PACKET_IPV4 0x0800
#define  PACKET_IPV6 0x86dd

/*not include wifi ports*/
#define MAX_PORT_NUM 32
#define LAN_PORT_NUM	4
#define WAN_MAP_PORT_ID	15

#define PROTOCOL_IGMP	0x02
#define PROTOCOL_UDP	0x11
#define PROTOCOL_TCP	0x06

#define PROTOCOL_ICMPV6	0x3a
#define PROTOCOL_UDP6	0x11
#define PROTOCOL_TCP6	0x02

#define ECNT_IGMP_NOT_QUERY	 0
#define ECNT_IGMP_GENERAL_QUERY	 1
#define ECNT_IGMP_SPECIFIC_QUERY 2
#define IGMP_HOST_MEMBER_QUERY	 0x11

#define XPON_MASK_MLDV1  16
#define XPON_MASK_IGMPV1 1
#define XPON_MASK_IGMPV2 2
#define XPON_MASK_IGMPV3 4
#define XPON_MASK_MLDV1  16
#define XPON_MASK_MLDV2  32

#define  XPON_HWNAT_AGE_TIME 3000

#define ETH_P_8021Q	0x8100 
#define ETH_P_QinQ_88a8	0x88a8          
#define ETH_P_QinQ_9100	0x9100

typedef struct
{    
	unsigned char  mac_address_byte [6];
}MTK_MAC_ADDRESS ;

typedef struct
{    
	unsigned char ip_address_array [16] ;
} MTK_IPV6_ADDRESS ;

typedef struct
{ 
	union{        
		/* IPv4*/         
		uint32 ipv4_address ;          
		/* IPv6*/         
		MTK_IPV6_ADDRESS ipv6_address ;     
		}IP ;    
		
		/*True = ipv6, False = ipv4*/     
		bool is_ipv6 ;
}MTK_IP_ADDRESS ;

typedef enum
{
	MCST_VLAN_TRANSPARENT = 0,
	MCST_VLAN_REPLACE,
	MCST_VLAN_REPLACE_AND_REMARK,
	MCST_VLAN_STRIP,
	MCST_VLAN_ADD,
	MCST_VLAN_ADD_AND_REMARK
}MTK_VLAN_MODE;

typedef struct
{
	uint16 vlan_mode;
	int (*mtk_vlan_handle)(struct sk_buff* skb,uint32 outerVlan,uint32 innerVlan,uint32 outerTci,uint32 innerTci);
}MTK_VLAN_HANDLE_t;

typedef IGMP_HWNATEntry_t MTK_IGMP_HWNATEntry_t;

typedef enum
{
	MULTI_LOG_OFF = 0,
    MULTI_LOG_ERROR,
    MULTI_LOG_WARNING , 
    MULTI_LOG_DBG,
    MULTI_LOG_DUMP_DATA, 
    MULTI_LOG_MAX_LEVER, 
} MULTI_LOG_LEVER;


/* IOCTL CMD */
#define IGMP_ADD_ENTRY			1
#define IGMP_DEL_ENTRY			2
#define IGMP_GET_ENTRY			3
#define IGMP_CLEAR_ENTRY		4
#define IGMP_UPDATE_ENTRY		5
#define IGMP_SET_FILTER_MODE	6
#define IGMP_GET_FILTER_MODE	7
#define IGMP_SET_IGMP_SNOOPING_ON_OFF	8
#define IGMP_GET_IGMP_SNOOPING_ON_OFF	9


/* filter mode */
typedef enum
{
	FWD_MODE_IP_AND_VID = 0,
	FWD_MODE_MAC,
	FWD_MODE_MAC_AND_VID,
	FWD_MODE_MAC_AND_IP,
	FWD_MODE_MAC_AND_IP_AND_VID,
}mc_fwd_mode;

enum _igmp_snooping_on_off_
{
	E_IGMP_SNOOPING_OFF  = 0,
	E_IGMP_SNOOPING_ON   = 1,
};

/* information provided by App */
struct vlan_info
{
	uint16 vlan_mode;
	uint16 outer_tpid;
	uint16 outer_tci;
	uint16 inner_tpid;
	uint16 inner_tci;
};

typedef struct
{
	uint8 port_id;
	uint16 vlan_id;
	MTK_IP_ADDRESS group_addr;
	MTK_MAC_ADDRESS group_mac;
	struct vlan_info vlan_action;
}mc_fwd_tbl_entry;

typedef struct
{
	uint8 port_id;
	int entry_num;
	mc_fwd_tbl_entry *pEntry;
}mc_fwd_entrys;

typedef struct
{
	struct list_head list;
	int flow_idx;
	uint8 port_id;
	uint16 vlan_id;
	MTK_IP_ADDRESS group_addr;
	MTK_MAC_ADDRESS group_mac;
	struct vlan_info vlan_action;
}MC_FwdEntry;

typedef struct
{
	mc_fwd_mode fwd_mode;
	struct list_head fwd_list;
}MC_FwdTable;


extern int (*tc3262_gmac_passthrough_hook)(struct sk_buff *skb,struct net_device* dev);

extern int (*xpon_igmp_learn_flow_hook)(struct sk_buff* skb);
extern int (*multicast_speed_find_entry_hook)(int index);
extern int (*ra_sw_nat_hook_drop_packet) (struct sk_buff * skb);
extern int (*hwnat_is_alive_pkt_hook)(struct sk_buff* skb);
extern int (*hwnat_is_alive_index_hook)(int index);
extern int (*hwnat_skb_to_foe_hook)(struct sk_buff* skb);
extern int (*hwnat_set_special_tag_hook)(int index, int tag);
extern int (*hwnat_multicast_set_hwnat_info_hook)(int index, int mask);
extern int (*hwnat_delete_foe_entry_hook)(int index); 
extern int (*hwnat_is_multicast_entry_hook)(int index ,unsigned char* grp_addr,unsigned char* src_addr,int type);
extern int (*hwnat_is_drop_entry_hook)(int index ,unsigned char* grp_addr,unsigned char* src_addr,int type);
extern int (*hwnat_set_rule_according_to_state_hook)(int index, int state,int mask);
extern int (*hwnat_set_recover_info_hook)(struct sk_buff* skb,struct sock *sk,int flag);

extern int (*mtk_multicast_data_handle_hook)(struct sk_buff *skb);

static int mtk_xpon_get_vlan_tag_num(struct sk_buff* skb);
int mtk_add_or_update_iptv_table_entry(short int group_vid ,MTK_IP_ADDRESS group_ip);
int mtk_add_iptv_table_entry(void);
int mtk_remove_iptv_table_entry(void);
int mtk_update_iptv_table_entry(void);
int mtk_get_fwd_port_mask_iptv_table(short int vid,MTK_MAC_ADDRESS mac_addr, MTK_IP_ADDRESS src_ip,MTK_IP_ADDRESS group_ip,uint32 *fwd_port_mask,unsigned char* bridge_flag);

#endif

