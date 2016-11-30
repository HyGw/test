#ifndef _COMMANDS_H
#define _COMMANDS_H

#define NARGS		20
#if defined(CONFIG_DUAL_IMAGE) || defined(TR068_LED)
#define MAIN_IMAGE 0
#define SLAVE_IMAGE 1
#endif

#if defined(RBUS_COUNTER_TEST)
#define BUS_CNT_BASE 				0xbfa00000
#define BUS_CNT_EN       				(BUS_CNT_BASE + 0x3c)
//CPU2PBUS Conuter
#define CPU2PBUS_WRCNT       		(BUS_CNT_BASE + 0x40)
#define CPU2PBUS_WRCNT_MIN       	(BUS_CNT_BASE + 0x44)
#define CPU2PBUS_WRCNT_MAX       	(BUS_CNT_BASE + 0x48)
#define CPU2PBUS_RDCNT       			(BUS_CNT_BASE + 0x50)
#define CPU2PBUS_RDCNT_MIN       	(BUS_CNT_BASE + 0x54)
#define CPU2PBUS_RDCNT_MAX       	(BUS_CNT_BASE + 0x58)
//CPU2DRAM Conuter
#define CPU2DRAM_WRCNT       		(BUS_CNT_BASE + 0x60)
#define CPU2DRAM_WRCNT_MIN       	(BUS_CNT_BASE + 0x64)
#define CPU2DRAM_WRCNT_MAX       	(BUS_CNT_BASE + 0x68)
#define CPU2DRAM_RDCNT       		(BUS_CNT_BASE + 0x70)
#define CPU2DRAM_RDCNT_MIN       	(BUS_CNT_BASE + 0x74)
#define CPU2DRAM_RDCNT_MAX      	(BUS_CNT_BASE + 0x78)
//DMA2DRAM Conuter
#define DMA2DRAM_WRCNT       		(BUS_CNT_BASE + 0x80)
#define DMA2DRAM_WRCNT_MIN       	(BUS_CNT_BASE + 0x84)
#define DMA2DRAM_WRCNT_MAX       	(BUS_CNT_BASE + 0x88)
#define DMA2DRAM_RDCNT       		(BUS_CNT_BASE + 0x90)
#define DMA2DRAM_RDCNT_MIN       	(BUS_CNT_BASE + 0x94)
#define DMA2DRAM_RDCNT_MAX       	(BUS_CNT_BASE + 0x98)

typedef struct _rcnt_para
{
	unsigned int count;
	unsigned int startAddr;
	unsigned int endAddr;
}rcnt_para;

typedef struct _auto_para
{
	int type;
	unsigned int startAddr;
	unsigned int endAddr;
	unsigned int length;
	int count;
}auto_para;


#endif

typedef struct cmds_s {
	const char *name;		
	int (*func)(int argc,char *argv[]);
	int  nargs;	
	const char *usage;	
	const char *help;	
} cmds_t;

extern int cmd_proc(void);
#if defined(TCSUPPORT_FWC_ENV)

extern int restart_time_flags;
extern int restart_time_count;
#endif

#endif /* _COMMANDS_H */
