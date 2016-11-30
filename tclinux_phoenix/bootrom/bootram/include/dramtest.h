#ifndef DRAMTEST_H_
#define DRAMTEST_H_


typedef struct dramTest_info_s {
	unsigned long startAddr;
	unsigned long size;
	unsigned long pattern;
	unsigned char wByte;
} dramTest_info_t;

typedef enum {
	ORI_PAT,
	INCR_PAT,
	ANTI_INCR_PAT,
} patType;

typedef struct gdmaTest_info_s {
	unsigned long sa; /*phy addr*/
	unsigned long da; /*phy addr*/
	unsigned long len;
	unsigned char burst_size;
	unsigned char wswap;
} gdmaTest_info_t;


extern int dram_pat_set(void *startAddr, unsigned long size, unsigned long pattern, int patType,unsigned char wByte);
extern int dramTest(dramTest_info_t *info, int isInputPat);
extern int gdmaTestDram(gdmaTest_info_t *info);

#define DBG_L1  0x1
#define DBG_L2  0x2
#define DBG_L3  0x4
#define DBG_L4  0x8
#if defined(TCSUPPORT_CPU_EN7521)
extern int mp_dbg_level;
#define DRAMTESTOFFSET0 0xa4040000
#define DRAMTESTOFFSET1 0xa4060000
#define MP_DBG(level, F, B...) { \
                                    if(mp_dbg_level & level) { \
                                        prom_printf(F, ##B) ; \
                                    } \
                                }
#else
#define DRAMTESTOFFSET0 0xa0040000
#define DRAMTESTOFFSET1 0xa0060000
#define MP_DBG(level, F, B...) { prom_printf(F, ##B) ; }
#endif

#endif /* DRAMTEST_H_ */
