#ifndef __LIBAPI_LIB_BRTABLE_H__
#define __LIBAPI_LIB_BRTABLE_H__

#define BRIDGE_LAN 		"br0"

#define ecnt_brmgr_lib_sync_brtable_start 		brmgr_lib_sync_brtable_start
#define ecnt_brmgr_lib_get_brtable 				brmgr_lib_get_brtable
#define ecnt_brmgr_lib_set_brtable_age    		brmgr_lib_set_brtable_age
#define ecnt_brmgr_lib_clean_brtable			brmgr_lib_clean_brtable

#ifndef u32
#define u32 unsigned int
#endif

#ifndef u16
#define u16 unsigned short
#endif

#ifndef u8
#define u8 unsigned char
#endif

typedef struct
{
	u8 port;
	u8 timer;
	u8 mac[6];
	u16 vid;
}macinfo_for_switch;

typedef struct
{
	u32 count;
	macinfo_for_switch macinfo[2048];
}brtable_for_switch;

struct fdb_entry {
	unsigned char mac_addr[6];
	unsigned short port_no;
	unsigned char is_local;
	struct timeval ageing_timer_value;
	unsigned short vlan_id;
	unsigned char vlan_layer;
};

typedef enum {
	ECNT_E_BRTABLE_SUCCESS = 0,
	ECNT_E_BRTABLE_PARA_FAIL,
	ECNT_E_BRTABLE_SOCK_FAIL,
	ECNT_E_BRTABLE_FAIL
}ECNT_LIB_BRTABLE_RET;


#define ECNT_LIB_BRTABLE_PRINT_ERROR(fmt, ...)  ECNT_LIB_DBG_PRINT(ECNT_DBG_ERROR,  ECNT_DBG_TYPE_BRTABELE, fmt, ##__VA_ARGS__)
#define ECNT_LIB_BRTABLE_PRINT_WARN(fmt, ...)   ECNT_LIB_DBG_PRINT(ECNT_DBG_WARN,   ECNT_DBG_TYPE_BRTABELE, fmt, ##__VA_ARGS__)
#define ECNT_LIB_BRTABLE_PRINT_NOTICE(fmt, ...) ECNT_LIB_DBG_PRINT(ECNT_DBG_NOTICE, ECNT_DBG_TYPE_BRTABELE, fmt, ##__VA_ARGS__)
#define ECNT_LIB_BRTABLE_PRINT_DEBUG(fmt, ...)  ECNT_LIB_DBG_PRINT(ECNT_DBG_DEBUG,  ECNT_DBG_TYPE_BRTABELE, fmt, ##__VA_ARGS__)

extern ECNT_LIB_BRTABLE_RET brmgr_lib_sync_brtable_start(void);
extern ECNT_LIB_BRTABLE_RET brmgr_lib_get_brtable(int index, struct fdb_entry *entry, int *num);
extern ECNT_LIB_BRTABLE_RET brmgr_lib_set_brtable_age(double time) ;
extern ECNT_LIB_BRTABLE_RET brmgr_lib_clean_brtable(void);
#endif

