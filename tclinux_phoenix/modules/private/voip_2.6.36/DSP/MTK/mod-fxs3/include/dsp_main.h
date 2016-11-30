/**
 *	@file dsp_main.h
 *	@author JJ Wang, Howard Chen
 */
#ifndef DSP_MAIN_H
#define	BASE_VDSP_M2D					0x0000
#define DSP_MAIN_H
/*
#define VDSP_PCCR						BASE_VDSP_M2D+0
#define VDSP_PENDS					BASE_VDSP_M2D+1
#define VDSP_PMR						BASE_VDSP_M2D+2
#define VDSP_PCLCR					BASE_VDSP_M2D+3
#define VDSP_PTSCR0					BASE_VDSP_M2D+4
#define VDSP_PTSCR1					BASE_VDSP_M2D+5
#define VDSP_PTSCR2					BASE_VDSP_M2D+6
#define VDSP_PTSCR3					BASE_VDSP_M2D+7
#define VDSP_LBTEST					BASE_VDSP_M2D+8
#define VDSP_CH_SET					BASE_VDSP_M2D+9
*/
/*	
#define VDSP_FUN_EN0				(BASE_VDSP_M2D+0xa)
#define VDSP_LEC_PAR0					(BASE_VDSP_M2D+0x10)
#define VDSP_LEC_PAR1					(BASE_VDSP_M2D+0x11)
#define VDSP_LEC_PAR2					(BASE_VDSP_M2D+0x12)
#define VDSP_LEC_PAR3					(BASE_VDSP_M2D+0x13)
#define VDSP_LEC_PAR4					(BASE_VDSP_M2D+0x14)
#define VDSP_LEC_PAR5					(BASE_VDSP_M2D+0x15)
*/
#define DSP_SUCCESS 0
#define DSP_FAIL -1


typedef struct {
    int  t1;    /* Frequency of tone1 to generate */
    int  t1_power;     /* Power in 0.5db units for t1 */
    int  t2;    /* Frequency of tone2 to generate */
    int  t2_power;     /* Power in 0.5db units for t2 */
    int  t3;    /* Frequency of tone3 to generate */
    int  t3_power;     /* Power in 0.5db units for t3 */
    int  t4;    /* Frequency of tone4 to generate */
    int  t4_power;     /* Power in 0.5db units for t4 */
    int  dummy;
    int  num_par;  /* Number of on/off pairs */
    int  tg_make1;    /* 1st on  time in 1 ms units */
    int  tg_break1;   /* 1st off time in 1 ms units */
    int  tg_repeat1;  /* 1st cadence cycles */ 
    int  tg_make2;    /* 2nd on  time in 1 ms units */
    int  tg_break2;   /* 2nd off time in 1 ms units */
    int  tg_repeat2;  /* 2nd cadence cycles */ 
    int  tg_make3;    /* 3rd on  time in 1 ms units */
    int  tg_break3;   /* 3rd off time in 1 ms units */
    int  tg_repeat3;  /* 3rd cadence cycles */ 
    int  tg_repeat;   /* Number of cycles of tones */
} vdsp_tonegen_para;

typedef struct {
	short VAD;
	union{
	/* -------- G.729 -------- */
	struct
	{
		int _conf;
	} _g729;
	#define G729_CONF		_u._g729._conf
	/* -------- ILBC -------- */
	struct
	{
		unsigned short rate;
	} _ilbc;
	#define ILBC_RATE	_u._ilbc.rate
	} _u;
}vdsp_codec_conf_t;

typedef struct {
    short tx_codec;
    short rx_codec;
    vdsp_codec_conf_t *tx_conf;
    vdsp_codec_conf_t *rx_conf;
} vdsp_codec_t;



enum VDSP_cid_spec_e
{
	VDSP_CID_NONE,
	VDSP_CID_MIN,
	/* ----------------------------------- */
	VDSP_CID_BELLCORE,		// USA 
	VDSP_CID_NTT,			// JAPAN 
	VDSP_CID_ETSI,			// EUROPE 
	VDSP_CID_CHT,			// TAIWAN 
	VDSP_CID_BT,				// BRITISH 
	/* ----------------------------------- */
	VDSP_CID_MAX,
};

enum VDSP_codec_e
{
	VDSP_CODEC_NONE,//16 bit linier mode
	VDSP_CODEC_ALAW,
	VDSP_CODEC_MULAW,
	VDSP_CODEC_G726,
	VDSP_CODEC_G729,
	VDSP_CODEC_G723_1,
	VDSP_CODEC_ILBC,
#ifdef AMR_CODEC
	VDSP_CODEC_AMR, //isabella_130830_AMR
#endif
	VDSP_CODEC_MAX,
};

enum VDSP_codec_configuration_e
{
	VDSP_CODEC_CONF_VAD,
	VDSP_CODEC_CONF_MAX,
};

enum VDSP_codec_direction_e
{
	VDSP_CODEC_UPLINK,
	VDSP_CODEC_DOWNLINK,
};


typedef struct fdsp{
	
	const char* ver;
	
	void* priv[10];
	/**
	 *	@brief down dsp firm
	 *	@param rev	firm reversion 
	 */
	int (*lec_enable)(struct fdsp* o,unsigned cid,unsigned en);

/* added by mtk07059_20140825,  for V21 damaged by EC.*/	
	int (*lec_aes_enable)(struct fdsp* o, unsigned cid, unsigned enable);
	int (*lec_bypass_set)(struct fdsp* o, unsigned cid, unsigned status);
	int (*lec_nlp_enable)(struct fdsp* o,unsigned cid,unsigned en);
	int (*lecModeSet)(struct fdsp* o,unsigned cid,unsigned mode);

	int (*lec_config)(struct fdsp* o,unsigned id,unsigned value);

	int (*lec_enableGet)(struct fdsp* o,unsigned cid,unsigned *en);

	int (*nr_enable)(struct fdsp* o,unsigned cid,unsigned en);

	int (*nr_config)(struct fdsp* o,unsigned id,unsigned value);

	int (*nr_enableGet)(struct fdsp* o,unsigned cid,unsigned *en);

	int (*nr_rx_enable)(struct fdsp* o,unsigned cid,unsigned en);

	int (*dl_nr_enable)(struct fdsp* o,unsigned cid,unsigned en);

	int (*tone_det_enable)(struct fdsp* o,int cid,int en,int remove_en);

	int (*fax_tone_det_enable)(struct fdsp* o,unsigned cid,unsigned en);

	int (*log_filter_config)(struct fdsp* o,unsigned id,unsigned value);

//	int (*log_flush)(struct fdsp* o);

	int (*pcm_reset)(struct fdsp* o,unsigned ch_num);

	int (*dsp_isr_init)(struct fdsp* o);
	
	int (*dsp_fast_isr)(struct fdsp* o);

	int (*dsp_isr)(struct fdsp* o);

	int (*dsp_eventnotify_register)(struct fdsp* o,void *data);

	int (*tone_gen)(struct fdsp* o,unsigned cid,unsigned en,vdsp_tonegen_para *para);

	int (*DTMFtone_gen)(struct fdsp* o, int cid, char num, int ms);
	
	//caller ID related function
	int (*callerid_setup)(struct fdsp* o, int cid, char *callid, int tx_pw, int spec,int strlen);

	int (*callerid_re_setup)(struct fdsp* o, int cid);
	
	int (*callerid_start_gen)(struct fdsp* o, int cid);

	int (*callerid_stop_gen)(struct fdsp* o, int cid);

	int (*callerid_gen_state)(struct fdsp* o, int cid);

	int (*drc_enable)(struct fdsp* o,unsigned cid,unsigned en);

	int (*drc_enableGet)(struct fdsp* o,unsigned cid,unsigned *en);
	
	int (*codec_reset)(struct fdsp* o, unsigned cid,  vdsp_codec_t *codec);

	int (*codec_config)(struct fdsp* o, unsigned short cid,short direction,short id,short value);
}fvdsp_t;

void dsp_auto_gain_control(int cid,short * buf, int buf_sz);
fvdsp_t*	FVDSP_GetInstance(void);
int vdsp_mutex(int lock);
int vdsp_irq_off(int off,int *flag);

#endif

