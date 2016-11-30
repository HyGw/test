/**
 *	@author Howard Chen
 */
#ifndef GDMA0318_DEF
#define GDMA0318_DEF

#include <asm/types.h>
#include <asm/io.h>
#include <asm/hardware.h>

extern unsigned hal_hdma_base;

#define HDMA_CMD(_chan, _i)		(hal_hdma_base + 0x114 + (((_i)&1)<<2) + (_chan) * 0x20)
#define HDMA_HDSR()				(hal_hdma_base + 0x100)
#define HDMA_HDCPR()			(hal_hdma_base + 0x104)
#define HDMA_GISAR      		(CIRQ_BASE+0x1300)
#define HDMA_GIMCR				(CIRQ_BASE+0x1308)
#define HDMA_GIMSR				(CIRQ_BASE+0x130C)

#define HDMA_STA_DONE			(0x1ff<<2)
#define HDMA_STA_VFF_UGT		(0xf<<24)

void hal_hdma_init(unsigned hal_hdma_base, int pcm_with_dsp);

void hal_hdma_start(unsigned chan, unsigned phy0, unsigned phy1, unsigned buf_sz);

void hal_hdma_stop(unsigned chan);

void hal_hdma_recover(int cid);

/**
 *	@brief resume the no <bi> buffer
 */
static inline void hal_hdma_resume(unsigned chan, unsigned bi)
{
	unsigned v = readl(HDMA_CMD(chan, bi)) | 1;
    writel(v, HDMA_CMD(chan, bi)); 
}

/**
 *	@return 1 if both buffer are not active 
 */
static inline int hal_hdma_is_idle(unsigned chan)
{
	return ((readl(HDMA_HDSR()) & ( 0x10001 << chan)) == 0);
}

/** 
 *	@return current busy buffer, either 0/1, dont care if hal_hdma_is_idle() 
 */
static inline int hal_hdma_get_busybuf(unsigned chan)
{
	return ((readl(HDMA_HDCPR()) >> chan) & 0x1);
}

#endif

