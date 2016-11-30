/** 
 *	@file conf_pcm_user.h 
 */ 
#ifndef conf_pcm_user_h
#define conf_pcm_user_h

#ifndef PCM_BASE
#define PCM_BASE 0x800C0000
#else
#undef PCM_BASE
#define PCM_BASE 0x800C0000
#endif

#ifndef PCM_DMA_BASE
#define PCM_DMA_BASE 0x80050000
#else
#undef PCM_DMA_BASE
#define PCM_DMA_BASE 0x80050000
#endif

#ifndef IRQ_PCM
#define IRQ_PCM 25
#else
#undef IRQ_PCM
#define IRQ_PCM 25
#endif

#ifndef IRQ_PCM_DMA
#define IRQ_PCM_DMA 24
#else
#undef IRQ_PCM_DMA
#define IRQ_PCM_DMA 24
#endif

#ifndef PCM_CHAN_MAX
#define PCM_CHAN_MAX 2
#else
#undef PCM_CHAN_MAX
#define PCM_CHAN_MAX 2
#endif

#ifndef PCM_BUFSZ
#define PCM_BUFSZ 160
#else
#undef PCM_BUFSZ
#define PCM_BUFSZ 160
#endif

#ifndef PCM_HARDLIMIT
#define PCM_HARDLIMIT 0
#else
#undef PCM_HARDLIMIT
#define PCM_HARDLIMIT 0
#endif

#ifndef PCM_LOOPBACK
#define PCM_LOOPBACK 0
#else
#undef PCM_LOOPBACK
#define PCM_LOOPBACK 0
#endif

#ifndef PCM_DMA
#define PCM_DMA 1
#else
#undef PCM_DMA
#define PCM_DMA 1
#endif

#ifndef PCM_DEBUG
#define PCM_DEBUG 0
#else
#undef PCM_DEBUG
#define PCM_DEBUG 0
#endif

#ifndef PCM_PRI_CONFIG
#define PCM_PRI_CONFIG 1
#else
#undef PCM_PRI_CONFIG
#define PCM_PRI_CONFIG 1
#endif

#ifndef PCM_DATA
#define PCM_DATA 0
#else
#undef PCM_DATA
#define PCM_DATA 0
#endif

#ifndef PCM_PRI_MANGLE
#define PCM_PRI_MANGLE 0
#else
#undef PCM_PRI_MANGLE
#define PCM_PRI_MANGLE 0
#endif

#ifndef PCM_PRI_LEC
#define PCM_PRI_LEC 2
#else
#undef PCM_PRI_LEC
#define PCM_PRI_LEC 2
#endif

#ifndef PCM_PRI_DTMF
#define PCM_PRI_DTMF 4
#else
#undef PCM_PRI_DTMF
#define PCM_PRI_DTMF 4
#endif

#ifndef PCM_PRI_CID
#define PCM_PRI_CID 6
#else
#undef PCM_PRI_CID
#define PCM_PRI_CID 6
#endif

#ifndef PCM_PRI_FAX
#define PCM_PRI_FAX 8
#else
#undef PCM_PRI_FAX
#define PCM_PRI_FAX 8
#endif

#ifndef PCM_PRI_DRC
#define PCM_PRI_DRC 10
#else
#undef PCM_PRI_DRC
#define PCM_PRI_DRC 10
#endif

#ifndef PCM_PRI_DATA
#define PCM_PRI_DATA 12
#else
#undef PCM_PRI_DATA
#define PCM_PRI_DATA 12
#endif

#ifndef PCM_PRI_TIMER
#define PCM_PRI_TIMER 14
#else
#undef PCM_PRI_TIMER
#define PCM_PRI_TIMER 14
#endif

#ifndef PCM_PRI_MAX
#define PCM_PRI_MAX 16
#else
#undef PCM_PRI_MAX
#define PCM_PRI_MAX 16
#endif

#endif
