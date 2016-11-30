#ifndef __LIBAPI_LIB_FLOWMGR_H__
#define __LIBAPI_LIB_FLOWMGR_H__

#define ecnt_flowmgr_lib_add  				flowmgr_lib_add
#define ecnt_flowmgr_lib_del  				flowmgr_lib_del
#define ecnt_flowmgr_lib_clean  			flowmgr_lib_clean
#define ecnt_flowmgr_lib_ioctl 				flowmgr_lib_ioctl


#define ETH_ALEN  6  

/* API 返回值*/
typedef enum
{
    ECNT_E_FLOW_SUCCESS = 0,
	ECNT_E_FLOW_NEXIST,
	ECNT_E_FLOW_EXIST,
	ECNT_E_FLOW_OVER,
	ECNT_E_FLOW_ODEV,
	ECNT_E_FLOW_FAIL
}ECNT_LIB_FLOW_RET;

enum
{
	ECN_FLD_SRC_PORT			= (1 << 0),	    /* 基于源PORT 匹配*/
	ECN_FLD_SA_MAC 				= (1 << 1),  	/* 基于SA MAC分类(SA MAC)*/
	ECN_FLD_DA_MAC 				= (1 << 2),  	/* 基于DA MAC分类(DA MAC)*/
	ECN_FLD_SA_IP  				= (1 << 3),  	/* 基于源IP地址分类(SA IP)*/
	ECN_FLD_DA_IP  			 	= (1 << 4),  	/* 基于目的IP地址分类(DA IP)*/
	ECN_FLD_VLAN_ID     	 	= (1 << 5),  	/* 基于VLAN ID分类(VLAN ID)*/
	ECN_FLD_L2_ETH_TYPE 	 	= (1 << 6),  	/* 基于以太网类型(Ethernet type)*/
	ECN_FLD_IP_PROTOCOL_TYPE	= (1 << 7), 	/* 基于IP协议类型(IP protocol type)*/
	ECN_FLD_VLAN_PRIO           = (1 << 8),  	/* 基于VLAN PRIO分类(pbit)*/
	ECN_FLD_IPV4_TRAFFIC_CLASS  = (1 << 9),  	/* 基于IP TOS/DSCP（IP v4）分类(TOS/DSCP（IP v4）)*/
	ECN_FLD_L4_SA_PORT			= (1 << 10),  	/* 基于L4 源PORT分类(L4 SA PORT)*/
	ECN_FLD_L4_DA_PORT 			= (1 << 11), 	/* 基于L4 目的PORT分类(L4 DA PORT)*/
	ECN_FLD_TTL 				= (1 << 12),    /* 基于生命周期的分类(IPv4生存时间值)*/

	/*IPV6相关*/
	ECN_FLD_IPV6_DSCP			= (1 << 13),    /* 基于IP DSCP分类(IP v6)*/
	ECN_FLD_IP_VER				= (1 << 14),    /* 基于IP版本号(IPV4/IPV6)*/
	ECN_FLD_IPV6_FLOW_LABEL		= (1 << 15),    /* 基于IPv6 flow label*/  
	ECN_FLD_DA_IPV6				= (1 << 16),    /* 基于IPv6目的地址*/
	ECN_FLD_SA_IPV6				= (1 << 17),    /* 基于IPv6源地址*/  
	ECN_FLD_DA_IPV6_HEAD		= (1 << 18),    /* 基于IPV6目的地址前缀*/
	ECN_FLD_SA_IPV6_HEAD		= (1 << 19),    /* 基于IPV6源地址前缀*/
	ECN_FLD_IPV6_NEXT_HEADER	= (1 << 20),   	/* 基于IPv6 next header（如TCP、UDP、ICMPv6、IGMPv6等）分类*/
	ECN_FLD_IPV6_TRAFFIC_CLASS	= (1 << 21), 	/* 基于IPv6  traffic class*/
	ECN_FLD_IPV6_HOP_LIMIT		= (1 << 22),    /* 基于IPv6 hop limit    */
	ECN_FLD_IPV6_L4_SOURCE_PORT	= (1 << 23),	/* 基于IPv6 的L4 源PORT分类(IPv6 L4 SA PORT)    */
	ECN_FLD_IPV6_L4_DEST_PORT	= (1 << 24),  	/* 基于IPv6 的L4 目的PORT分类(IPv6 L4 DA PORT) */
	/*以下暂不支持*/
	ECN_FLD_PHY_DEST_PORT		= (1 << 25),	/* 基于物理目的端口分类(Phy Dest Port)*/
	ECN_FLD_DHCPV6_MESSAGE_TYPE = (1 << 26),	/* 基于DHCPv6消息类型*/

};

typedef enum _classify_action_enum_
{
	E_ACL_ACTION			 = (1 << 0),
	E_QUEUE_MAPPING_ACTION   = (1 << 1),
	E_PRIORITY_REMARK_ACTION = (1 << 2),
	E_LIMIT_ACTION           = (1 << 3),
	E_TOS_REMARK_ACTION		 = (1 << 4),
}classify_action_enum;

typedef enum _port_list_
{
	E_ETH_PORT_0  = 0,
	E_ETH_PORT_1  = 1,
	E_ETH_PORT_2  = 2,
	E_ETH_PORT_3  = 3,
	
	E_WIFI_PORT_0 = 4,
	E_WIFI_PORT_1 = 5,
	E_WIFI_PORT_2 = 6,
	E_WIFI_PORT_3 = 7,
	
	E_WAN_PORT    = 15,
	E_MAX_PORT    = 16,
}e_port_list;



typedef struct _ecn_fld_element_
{
	unsigned int						 fld_bit_map;
	unsigned char				   dst_mac[ETH_ALEN];
	unsigned char				   src_mac[ETH_ALEN];
	struct in_addr						 ipv4_src_ip;
	struct in_addr						 ipv4_dst_ip;
	unsigned short								 vid;
	unsigned short						  ether_type;
	unsigned char								pbit;
	unsigned char								 tos;
	unsigned int					l4_ipv4_src_port;
	unsigned int					l4_ipv4_dst_port;
	unsigned char							ipv4_ttl;	
	/*ipv6_dscp : IPv6	traffic class  & 0xFC*/
	unsigned char						   ipv6_dscp; 
	unsigned char						  ip_version;
	unsigned char						 flow_lbl[3];
	struct in6_addr 					  ipv6_saddr;
	struct in6_addr 					  ipv6_daddr;
	unsigned char							 nexthdr;
	unsigned char				  ipv6_traffic_class;
	unsigned char						   hop_limit;
	unsigned int					l4_ipv6_src_port;
	unsigned int					l4_ipv6_dst_port;
	struct in6_addr 				  ipv6_saddr_hdr;
	struct in6_addr 				  ipv6_daddr_hdr;
	unsigned char					 dhcpv6_msg_type;
	unsigned char							 ip_type; 
}ecn_fld_element_t, *pt_ecn_fld_element;

typedef enum
{
	E_ACL_FORWARD = 0,
	E_ACL_DROP 	  = 1,
}e_acl_mode;

typedef struct _classify_action_info_
{
	unsigned char 			action_bit_map;
	unsigned char 	 		remark_pbit;
	unsigned char  	 		remark_queue;
	unsigned int     		limit_group_id;
	unsigned int     		remark_tos;
	e_acl_mode				acl_mode;
}ecn_classify_action_info_t, *pt_ecn_classify_action_info;

typedef struct _ecn_traffic_classify_info_
{
	e_port_list 					   port;
	ecn_fld_element_t			fld_element;
	ecn_classify_action_info_t	act_element;

}ecn_traffic_classify_info_t, *pt_ecn_traffic_classify_info;

ECNT_LIB_FLOW_RET flowmgr_lib_add(ecn_traffic_classify_info_t *port_rule);

ECNT_LIB_FLOW_RET flowmgr_lib_del(ecn_traffic_classify_info_t *port_rule);
ECNT_LIB_FLOW_RET flowmgr_lib_clean(ecn_traffic_classify_info_t *port_rule);

#endif 
