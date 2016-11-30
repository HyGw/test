#ifndef __ECNT_LIB_DBG_H__
#define __ECNT_LIB_DBG_H__



enum ECNT_DBG_LIB_TYPE 
{
	ECNT_DBG_TYPE_PCIE = 0,
	ECNT_DBG_TYPE_ACL,
	ECNT_DBG_TYPE_VOIP,
	ECNT_DBG_TYPE_GPON,
	ECNT_DBG_TYPE_BRTABELE,
	ECNT_DBG_TYPE_MAX	
};	


typedef enum _ecnt_itf_debug_level_
{
	ECNT_DBG_ERROR  = 1 << 0,
	ECNT_DBG_WARN   = 1 << 1,
	ECNT_DBG_NOTICE = 1 << 2,
	ECNT_DBG_DEBUG  = 1 << 3,
}e_ecnt_itf_debug_level_t;

void ecnt_itf_printf(char *fmt,...);								
int get_itf_dbg_switch(unsigned int level,unsigned int type);

#define ECNT_LIB_DBG
#ifdef ECNT_LIB_DBG
#define ECNT_LIB_DBG_PRINT(level,type,fmt, ...) do{\
                                            if(get_itf_dbg_switch(level,type))\
                                                ecnt_itf_printf("[%s]:" fmt,__FUNCTION__,##__VA_ARGS__);\
                                         }while(0)

#else
#define ECNT_LIB_DBG_PRINT(level,type,fmt, ...)
#endif

#if 0
#define ECNT_LIB_PCIE_PRINT_ERROR(fmt, ...)  ECNT_LIB_DBG_PRINT(ECNT_DBG_ERROR,  ECNT_DBG_TYPE_PCIE, fmt, ##__VA_ARGS__)
#define ECNT_LIB_PCIE_PRINT_WARN(fmt, ...)   ECNT_LIB_DBG_PRINT(ECNT_DBG_WARN,   ECNT_DBG_TYPE_PCIE, fmt, ##__VA_ARGS__)
#define ECNT_LIB_PCIE_PRINT_NOTICE(fmt, ...) ECNT_LIB_DBG_PRINT(ECNT_DBG_NOTICE, ECNT_DBG_TYPE_PCIE, fmt, ##__VA_ARGS__)
#define ECNT_LIB_PCIE_PRINT_DEBUG(fmt, ...)  ECNT_LIB_DBG_PRINT(ECNT_DBG_DEBUG,  ECNT_DBG_TYPE_PCIE, fmt, ##__VA_ARGS__)
#endif
#endif
