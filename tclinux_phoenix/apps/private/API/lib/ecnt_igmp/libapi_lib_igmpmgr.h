/*********************************************************************************
 * decleration and function prototype for igmp module
 *
 * Copyright (C) 2015 Econet Technologies, Corp.
 * All Rights Reserved.
 *
 *********************************************************************************/
#ifndef _LIBAPI_LIB_IGMPMGR_H_
#define _LIBAPI_LIB_IGMPMGR_H_

/*********************************************************************************
 * data structure definition section
 *
 *********************************************************************************/
#ifndef uint8
#define uint8	unsigned char
#endif
	
#ifndef uint16
#define uint16	unsigned short
#endif
	
#ifndef uint32
#define uint32	unsigned long
#endif

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

#define bool	uint8

typedef struct
{    
	unsigned char  mac_address_byte[6];
}MTK_MAC_ADDRESS;

typedef struct
{    
	unsigned char ip_address_array[16];
} MTK_IPV6_ADDRESS;

typedef struct
{ 
	union{        
		/* IPv4*/         
		uint32 ipv4_address;          
		/* IPv6*/         
		MTK_IPV6_ADDRESS ipv6_address;     
		}IP;
		
		/*True = ipv6, False = ipv4*/     
		bool is_ipv6;
}MTK_IP_ADDRESS;

typedef enum
{
	FWD_MODE_IP_AND_VID = 0,
	FWD_MODE_MAC,
	FWD_MODE_MAC_AND_VID,
	FWD_MODE_MAC_AND_IP,
	FWD_MODE_MAC_AND_IP_AND_VID,
}mc_fwd_mode;

typedef enum
{
	MCST_VLAN_TRANSPARENT = 0,
	MCST_VLAN_REPLACE,
	MCST_VLAN_REPLACE_AND_REMARK,
	MCST_VLAN_STRIP,
	MCST_VLAN_ADD,
	MCST_VLAN_ADD_AND_REMARK
}MTK_VLAN_MODE;

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

enum _igmp_snooping_on_off_
{
	E_IGMP_SNOOPING_OFF  = 0,
	E_IGMP_SNOOPING_ON   = 1,
};

/*********************************************************************************
 * API function prototype declaration section
 *
 *********************************************************************************/
int ecnt_igmp_mode_get(mc_fwd_mode *mode);

int ecnt_igmp_mode_set(mc_fwd_mode mode);

int ecnt_igmp_entry_clear(void);

int ecnt_igmp_entry_add(mc_fwd_tbl_entry *entry_data);

int ecnt_igmp_entry_del(mc_fwd_tbl_entry *entry_data);

int ecnt_igmp_entry_update(mc_fwd_tbl_entry *entry_data);

int ecnt_igmp_entry_get(uint8 port_id,mc_fwd_tbl_entry entry[],int *array_size);

int ecnt_igmp_snooping_on_off_get(unsigned char *enable);

int ecnt_igmp_snooping_on_off_set(unsigned char enable);

#endif
