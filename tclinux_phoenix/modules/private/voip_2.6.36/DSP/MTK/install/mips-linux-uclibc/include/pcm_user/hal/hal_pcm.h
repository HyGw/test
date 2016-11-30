/**
 *	@author Howard Chen
 */
#ifndef HAL_PCM_0318
#define HAL_PCM_0318

#define PCM_L2ISAR   	(CIRQ_BASE+0x1320)  // PCM Error Indication Status / ACK
#define PCM_L2IMKR   	(CIRQ_BASE+0x1324)	// Int Mask
#define PCM_L2IMCR   	(CIRQ_BASE+0x1328)  // Int Clear
#define PCM_L2IMSR   	(CIRQ_BASE+0x132C)  // Int Set 

void hal_pcm_init(unsigned base, int with_dsp);

void hal_pcm_enable(unsigned cid, unsigned en);

void hal_pcm_dest(void);

#endif
