/*
 * pcm_listener.h
 *
 *  Created on: 2011/10/27
 *      Author: pork
 */

#ifndef PCM_LISTENER_H_
#define PCM_LISTENER_H_

#include  "pcm_ext_conf.h"
#include  "eva_log.h"

#define PCM_CHAN_SUP_MAX 4
#define PCM_BUf_SIZE 160
#define PCM_SAM_SZ 80
#define MAX_RX_QUEUE_SIZE 8
#define PCM_CHANNEL_MAX 8
#define DSP_PROC_KTHREAD_PRIO 100
#define MODULE_NAME_DSPCore		"DSPCore"
#define PCM_NUM 2

#ifndef PCM_BUFSZ
#define PCM_BUFSZ 160
#else
#undef PCM_BUFSZ
#define PCM_BUFSZ 160
#endif

#ifndef PCM_PRI_MANGLE
#define PCM_PRI_MANGLE 0
#else
#undef PCM_PRI_MANGLE
#define PCM_PRI_MANGLE 0
#endif

#ifndef PCM_PRI_GAIN_DL
#define PCM_PRI_GAIN_DL 2
#else
#undef PCM_PRI_GAIN_DL
#define PCM_PRI_GAIN_DL 2
#endif

#ifndef PCM_PRI_FAX
#define PCM_PRI_FAX 4
#else
#undef PCM_PRI_FAX
#define PCM_PRI_FAX 4
#endif

#ifndef PCM_PRI_GAIN_UL
#define PCM_PRI_GAIN_UL 6
#else
#undef PCM_PRI_GAIN_UL
#define PCM_PRI_GAIN_UL 6
#endif


#ifndef PCM_PRI_LEC
#define PCM_PRI_LEC 8
#else
#undef PCM_PRI_LEC
#define PCM_PRI_LEC 8
#endif

#ifndef PCM_PRI_DTMF_NEAR 
#define PCM_PRI_DTMF_NEAR 10
#else
#undef PCM_PRI_DTMF_NEAR
#define PCM_PRI_DTMF_NEAR 10
#endif

#ifndef PCM_PRI_CID
#define PCM_PRI_CID 12
#else
#undef PCM_PRI_CID
#define PCM_PRI_CID 12
#endif

#ifndef PCM_PRI_DTMF_FAR 
#define PCM_PRI_DTMF_FAR 14
#else
#undef PCM_PRI_DTMF_FAR
#define PCM_PRI_DTMF_FAR 14
#endif

#ifndef PCM_PRI_DATA
#define PCM_PRI_DATA 16
#else
#undef PCM_PRI_DATA
#define PCM_PRI_DATA 16
#endif

#ifndef PCM_PRI_TIMER
#define PCM_PRI_TIMER 18
#else
#undef PCM_PRI_TIMER
#define PCM_PRI_TIMER 18
#endif

#ifndef PCM_PRI_MAX
#define PCM_PRI_MAX 20
#else
#undef PCM_PRI_MAX
#define PCM_PRI_MAX 20
#endif

#define PCM_PRI_MAX_INTER		(PCM_PRI_MAX+1)

typedef struct{
	
	int (*pcmRecvFuncRegister)(void*);	
	void (*pcmRecv)(void);
	#ifdef PCM_LOOPBACK_TEST
	void* (*pcmSendBufAlloc)(int);	
	#else
	void* (*pcmSendBufAlloc)(void);
	#endif	
	int (*pcmSend)(uint8**,int,int*);;
	void (*pcmRecvChBufFree)(uint8*);	
	void (*pcmTxBufKfree)(void*);
	void (*reInitPcm)(unsigned long);
	int (*pcmRecvSampleSizeGet)(unsigned char*);	
    void (*timeSlotCfgReinit)(int);
	int (*pcmBitDelaySet)(int);	
	int (*flushDlRtpQueueRegister)(void*);
	void (*pcmDMAStop)(int);
	int (*pcmConfig)(configNode_t*,int*);
	void (*pcmRestart)(int);	
}PCM_FUNC_TBL;

typedef	enum pcm_func_id
{
	
	PCM_FUNC_RecvFuncRegister = 1,
	PCM_FUNC_Recv = 2,
	PCM_FUNC_SendBufAlloc = 3,
	PCM_FUNC_Send = 4,
	PCM_FUNC_RecvChBufFree = 5,
	PCM_FUNC_TxBufKfree = 6, 
	PCM_FUNC_reInit = 7, 
	PCM_FUNC_RecvSampleSizeGet = 8,	
	PCM_FUNC_timeSlotCfgReinit = 9,
	PCM_FUNC_BitDelaySet = 10,
	PCM_FUNC_flushDlRtpQ = 11,
	PCM_FUNC_CONFIG = 12,
	PCM_FUNC_Restart = 13,
	PCM_FUNC_DMAStop = 14,
} PCM_FUNC_ID;

typedef struct pcm_listener {

	/**
	 *	@param notify
	 *	@param buf, "pointer of" the pointer to rx buffer, listener could modify the content
	 *         the the rx buffer or assign a new buffer addres to *buf
	 */
	void (*tx_notify)(struct pcm_listener* o, unsigned** buf, unsigned  buf_sz);

	/**
	 *	@param notify
	 *	@param buf, "pointer of" the pointer to rx buffer, listener could modify the content
	 *         the the rx buffer or assign a new buffer addres to *buf
	 *  @param buf_sz, "pointer of" the rx buffer size which the *buf points to
	 *         a listener could modify the *buf_sz
	 */
	void (*rx_notify)(struct pcm_listener* o, unsigned** buf, unsigned* buf_sz);

	int mod_consist;
	void* priv;

} pcm_listener_t;



typedef struct pcm_chan{
	unsigned* chanTxBuf;
	unsigned* chanRxBuf;
	pcm_listener_t*	listeners[PCM_PRI_MAX_INTER];
#if defined(PCM_DATA_REPLACE)
	int ch_num;
#endif
}pcm_chan_t;

int pcm_listener_init(void);
void pcm_listener_stop(void);
void pcm_listener_start(void);
int pcm_listener_handle(int);
#if defined(PCM_DATA_REPLACE)
void pcm_data_cpoy(uint16 *dst, uint16 *src, int len);
struct file * pcm_replace_file_open(void);
void pcm_replace_file_close(void);
int pcm_replace_data_from_file(unsigned short *inBuf);
#endif

/**
 *	@param priority 0~3,
 *		for rx , listener[0] -> listener[1] -> listener[2] -> listener[3]
 * 		for tx , listener[3] -> listener[2] -> listener[1] -> listener[0]
 */
void pcm_listener_add(unsigned cid, unsigned priority, pcm_listener_t* listener);
void pcm_listener_del(unsigned cid, unsigned priority);
void pcmRxqueueClean(void) ;
void pcm_channel_num_set(unsigned pcm_id,int ch_num);
int pcm_channel_num_get(unsigned pcm_id);
#endif /* PCM_LISTENER_H_ */
