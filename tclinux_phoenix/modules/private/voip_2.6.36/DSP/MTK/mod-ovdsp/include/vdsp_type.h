#ifndef __VDSP_TYPE_H__
#define __VDSP_TYPE_H__


/* CONSTANTS AND MACROS
*/

/* TYPE DEFINITION
*/

typedef enum e_jitter_scheme
{
	JITTER_DYNAMIC=1,
	JITTER_STATIC=2,
	JITTER_DISABLE=3
} jitter_scheme_e;

typedef enum e_vdsp_event
{
	VDSP_EVENT_RFC2833_DIGIT = 0x510,
	VDSP_EVENT_DISC_FAX,
	VDSP_EVENT_DVCC_DETECTED,
	VDSP_EVENT_JB,
#ifdef C7_DSP_SUPPORT
	VDSP_EVENT_PT_CHANGE,
#endif
/* added by mtk07059_20151021, for debug log. */
	VDSP_EVENT_LOG_SWITCH,
#ifdef CJ_DSP_SUPPORT
	VDSP_EVENT_FIRST_RTP,/* added by mtk07059_20150514 */
#endif
	VDSP_EVENT_MAX,
} E_VDSP_EVENT;

typedef	enum e_vad_process_scheme
{
	VAD_DISABLE = 1,
	VAD_NOCNG = 2,
	VAD_ONLY_G711ANNEXII =3,
	VAD_CODEC_SPECIFIC = 4,
	VAD_ONLYCNG = 5,
} E_VAD_PROCESS_SCHEME;

typedef enum e_vcodec_pt_type
{
	PT_CODEC_PCMU = 0,
	PT_CODEC_PCMA = 8,
	PT_CODEC_CN = 13,
	PT_CODEC_G729 = 18,
	PT_CODEC_G722 = 9,
	PT_CODEC_G7231 = 4,
	PT_CODEC_G726_16 = 78, /* R1 definition */ 
	PT_CODEC_G726_24 = 79,
	PT_CODEC_G726_32 = 80, //modified by terry for adapt to D2 g726
	PT_CODEC_G726_40 = 81, 
	PT_CODEC_iLBC = 82,
	PT_TELEPHONE_EVENT = 83,
	PT_TELEPHONE_DL_EVENT = 83,/*the same as PT_TELEPHONE_EVENT*/
	PT_CODEC_AMR = 85, //isabella_130830_AMR
	PT_REDUNDANT =86, //mtk69097_for_redundant
	PT_CODEC_PCMA_VBD =87,/*mtk08176 for vbd*/
	PT_CODEC_PCMU_VBD =88,/*mtk08176 for vbd*/
} E_VCODEC_PTTYPE;

typedef	enum e_g7231_bitrate
{
	G7231_5300 = 53,
	G7231_6300 = 63
	
} E_G7231_BITRATE;

typedef	enum e_ilbc_bitrate
{
	ILBC_20 = 1520,	/* 15.20 kbit/s for 20 ms */
	ILBC_30 = 1333,	/* 13.33 kbit/s for 30 ms */
	
} E_ILBC_BITRATE;

//<--isabella_130830_AMR
#ifdef AMR_CODEC
typedef	enum e_amr_bitrate
{
	AMR_475 = 475,
	AMR_515 = 515,			  
	AMR_59 = 590,
	AMR_67 = 670,
	AMR_74 = 740,
	AMR_795 = 795,
	AMR_102 = 1020,
	AMR_122 = 1220,			  
	AMR_SID = 2000,//defined by isabella  
	AMR_NONE = 3000,//defined by isabella
} E_AMR_BITRATE;
#endif
//<--isabella_130830_AMR


typedef enum e_vcodec_type
{
	VOIP_CODEC_NONE 	= 0x0000,
	VOIP_CODEC_G729 	= 0x0001,
	VOIP_CODEC_G7231	= 0x0002,
	VOIP_CODEC_G726_16 	= 0x0008,
	VOIP_CODEC_G726_24 	= 0x0010,
	VOIP_CODEC_G726_32 	= 0x0020,
	VOIP_CODEC_G726_40 	= 0x0040,
	VOIP_CODEC_PCMA 	= 0x0080,
	VOIP_CODEC_PCMU 	= 0x0100,
	VOIP_CODEC_G722		= 0x0200,	
	VOIP_CODEC_CN 		= 0x0400,
	VOIP_CODEC_ILBC		= 0x0800,
	VOIP_TELEPHONE_EVENT= 0x1000,
#ifdef AMR_CODEC
	VOIP_CODEC_AMR	= 0x2000, //isabella_130830_AMR
#endif
	VOIP_CODEC_REDUDANT	= 0x4000, //mtk69097_for_redundant
#if defined(TCSUPPORT_C1_MS) || defined(CJ_DSP_SUPPORT)
	VOIP_TELEPHONE_DL_EVENT= 0x8000,
#endif
#ifdef V152_SUPPORT
	VOIP_CODEC_PCMA_VBD =0x10000,/*mtk08176 for vbd*/
	VOIP_CODEC_PCMU_VBD =0x20000,/*mtk08176 for vbd*/
#endif
	VOIP_RESET_CODEC    =0x40000,

} E_VCODEC_TYPE;


/**
 *	@brief RTP socket behavior mode
 */ 
typedef	enum e_socket_mode
{
	SDMODE_NOINITIAL=0x0,
	SDMODE_NONE=0x01,
	SDMODE_SENDONLY=0x2,
	SDMODE_RECVONLY=0x4,
	SDMODE_SENDRECV=0x8,
	SDMODE_MIXED=0x10,
	SDMODE_MAX=0x80,
	
} E_SOCKET_MODE;

typedef	struct s_nw_info
{
	unsigned long ip_addr;
	int port;
	int rtcpPort;
	
} T_NWINFO;

#endif /* __VDSP_TYPE_H__ */

