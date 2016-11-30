
/*********************************************************************
* LEGAL DISCLAIMER
*
* (Header of MediaTek Software/Firmware Release or Documentation)
*
* BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY 
* ACKNOWLEDGES AND AGREES THAT THE SOFTWARE/FIRMWARE AND ITS 
* DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED FROM MEDIATEK AND/OR 
* ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS ONLY. 
* MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR 
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF 
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR 
* NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY 
* WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH 
* MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK 
* SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY
* WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO NOT BE 
* RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S 
* SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
* BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND 
* CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED 
* HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE 
* MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR 
* SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE
* AT ISSUE.
*
* THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN 
* ACCORDANCE WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING
* ITS CONFLICT OF LAWS PRINCIPLES.
**********************************************************************
*/
/**
 *  @file           eva_log.h
 *  @author         mtk06404
 *  @brief          eva_log  header file used to handle with dsp log.
 *  @date           2016.05.31
 */
#ifndef __EVA_LOG_H__
#define __EVA_LOG_H__


/* CONSTANTS AND MACROS
*/

/* module index defined for log, added by mtk07059_20151021. */
/* EVA Log Modules */
typedef enum{
	EVA_LOG_MODULE_ADAM=0,
	EVA_LOG_MODULE_GDI,
	EVA_LOG_MODULE_FXS,
	EVA_LOG_MODULE_VDSP,
	EVA_LOG_MODULE_ORTP,
	EVA_LOG_MODULE_LEC,
	EVA_LOG_MODULE_SLIC,
	EVA_LOG_MODULE_PCM,
	EVA_LOG_MODULE_MAX
}evaLogModuleIndex;


/* EVA Log Modules Mask */
#define EVA_MODULE_MASK_ADAM	(0x1 << EVA_LOG_MODULE_ADAM)
#define EVA_MODULE_MASK_GDI 	(0x1 << EVA_LOG_MODULE_GDI)
#define EVA_MODULE_MASK_FXS 	(0x1 << EVA_LOG_MODULE_FXS)
#define EVA_MODULE_MASK_VDSP	(0x1 << EVA_LOG_MODULE_VDSP)
#define EVA_MODULE_MASK_ORTP	(0x1 << EVA_LOG_MODULE_ORTP)
#define EVA_MODULE_MASK_LEC 	(0x1 << EVA_LOG_MODULE_LEC)
#define EVA_MODULE_MASK_SLIC	(0x1 << EVA_LOG_MODULE_SLIC)
#define EVA_MODULE_MASK_PCM   	(0x1 << EVA_LOG_MODULE_PCM)


#define EVA_MODULE_MASK_ALL	(EVA_MODULE_MASK_ADAM|EVA_MODULE_MASK_GDI \
	|EVA_MODULE_MASK_FXS|EVA_MODULE_MASK_VDSP|EVA_MODULE_MASK_ORTP \
	|EVA_MODULE_MASK_LEC|EVA_MODULE_MASK_SLIC|EVA_MODULE_MASK_PCM)


/* EVA Log Levels */
typedef enum {
	EVA_LOG_NONE = 0,	/* Log level NONE */
	EVA_LOG_ERR = 1,	/* Log level ERROR, for the error that effect the main function. */
	EVA_LOG_WARN = 2,	/* Log level WARNING, for warning of the innormal cases.  */
	EVA_LOG_INFO = 3,	/* Log level INFO, for information of function entry and exit. */
	EVA_LOG_DBG = 4,	/* Log level DEBUG, for other log. */
	EVA_LOG_MAX = 5,	/* Log level MAX */
} evaLogLevel_e;


/* EVA Log Levels Mask */
#define EVA_LOG_MASK_ERR	(0x1 << (EVA_LOG_ERR-1))
#define EVA_LOG_MASK_WARN	(0x1 << (EVA_LOG_WARN-1))
#define EVA_LOG_MASK_INFO	(0x1 << (EVA_LOG_INFO-1))
#define EVA_LOG_MASK_DBG	(0x1 << (EVA_LOG_DBG-1))

#define _SLIC_DEBUG_ENABLE	1
#define VDSP_DEBUG 1
#define _PCM_DEBUG_ENABLE 	1


/*=======================slic Log==============================*/
/* === Do not change the value of following macro=== */
#define _SLIC_DBG_TYPE_DEBUG      (0x1 << (EVA_LOG_DBG-1))
#define _SLIC_DBG_TYPE_INFO       (0x1 << (EVA_LOG_INFO-1))
#define _SLIC_DBG_TYPE_WARNING    (0x1 << (EVA_LOG_WARN-1))
#define _SLIC_DBG_TYPE_ERROR      (0x1 << (EVA_LOG_ERR-1))
#define _SLIC_DBG_TYPE_ALL        0xffffffff

/* Slic Sub Moudles */
typedef enum e_slic_modid
{
	SLIC_MOD_SLIC,
	SLIC_MOD_SPI,
}slic_modid_e;

/* Slic Sub Moudle Mask */
#define SLIC_LOG_MASK_SPI		(0x1 << SLIC_MOD_SPI)
#define SLIC_LOG_MASK_SLIC		(0x1 << SLIC_MOD_SLIC)


/* Debug */
#if defined (_SLIC_DEBUG_ENABLE) 

/* check log switch for slic module. */
#define LOGDBG_CHECK_SLIC_ENABLE	(GET_Log_moudle_switch() & EVA_MODULE_MASK_SLIC)
#define LOGDBG_CHECK_SUB_SLIC_ENABLE (	GET_Log_submoudle_switch(EVA_LOG_MODULE_SLIC) & SLIC_LOG_MASK_SLIC)


#define SLIC_DBG_MSG(type, fmt, var...) \
	do \
	{ \
		if(LOGDBG_CHECK_SLIC_ENABLE && LOGDBG_CHECK_SUB_SLIC_ENABLE) {\
			if (GET_Log_level(EVA_LOG_MODULE_SLIC) & type) \
				slic_log_format_printk(SLIC_MOD_SLIC, type,__func__,__LINE__, fmt, ##var);\
		}\
	} while(0)

#define SLIC_1_DBG_MSG(type, fmt, var...) \
	do \
	{ \
		if(LOGDBG_CHECK_SLIC_ENABLE && LOGDBG_CHECK_SUB_SLIC_ENABLE) {\
			if (GET_Log_level(EVA_LOG_MODULE_SLIC) & type) \
				slic_log_format_printk(SLIC_MOD_SLIC, type,__func__,__LINE__, fmt, ##var);\
		}\
	} while(0)

#define dbg_printf(fmt, var...) \
	do \
	{ \
		if(LOGDBG_CHECK_SLIC_ENABLE && LOGDBG_CHECK_SUB_SLIC_ENABLE) {\
			if (GET_Log_level(EVA_LOG_MODULE_SLIC) & _SLIC_DBG_TYPE_ERROR) \
				slic_log_format_printk(SLIC_MOD_SLIC, _SLIC_DBG_TYPE_ERROR,__func__,__LINE__, fmt"\n", ##var);\
		}\
	} while(0)

#define event_printf(fmt, var...) \
	do \
	{ \
		if(LOGDBG_CHECK_SLIC_ENABLE && LOGDBG_CHECK_SUB_SLIC_ENABLE) {\
			if (GET_Log_level(EVA_LOG_MODULE_SLIC) & _SLIC_DBG_TYPE_DEBUG) \
				slic_log_format_printk(SLIC_MOD_SLIC, _SLIC_DBG_TYPE_DEBUG,__func__,__LINE__, fmt"\n", ##var);\
		}\
	} while(0)	
#else
#define SLIC_DBG_MSG(type, fmt, var ...) do {} while (0)
#define SLIC_1_DBG_MSG(type, fmt, var ...) do {} while (0)
#define dbg_printf(fmt, var...)   do {} while (0)
#define event_printf(fmt, var...)   do {} while (0)
#endif
/*=======================slic Log end ===========================*/



/*=======================spi Log ==============================*/

/* === Do not change the value of following macro=== */
#define SPI_LOG_LEVEL_DEBUG      (0x1 << (EVA_LOG_DBG-1))
#define SPI_LOG_LEVEL_INFO       (0x1 << (EVA_LOG_INFO-1))
#define SPI_LOG_LEVEL_WARNING    (0x1 << (EVA_LOG_WARN-1))
#define SPI_LOG_LEVEL_ERROR      (0x1 << (EVA_LOG_ERR-1))
#define SPI_LOG_LEVEL_ALL        0xffffffff

/* Debug */
#if defined (_SLIC_DEBUG_ENABLE)
#define LOGDBG_CHECK_SPI_ENABLE (GET_Log_submoudle_switch(EVA_LOG_MODULE_SLIC) & SLIC_LOG_MASK_SPI)
#define SPI_RATE_LIMIT  ((jiffies%GET_Log_interval(EVA_LOG_MODULE_SLIC)) < GET_Log_times(EVA_LOG_MODULE_SLIC))

#define SPI_DBG(lv, fmt, args...) \
	do \
	{ \
		if(GET_Log_level(EVA_LOG_MODULE_SLIC) & lv) {\
			if (LOGDBG_CHECK_SPI_ENABLE && LOGDBG_CHECK_SLIC_ENABLE) \
				spi_log_format_printk(SLIC_MOD_SPI, lv,__func__,__LINE__, fmt, ##args);\
		}\
	} while(0)	
				
#else
#define SPI_DBG(lv, fmt, args...) do {} while(0)
#endif
/*=======================spi Log end ===========================*/

/*=======================pcm Log =============================*/
/* === Do not change the value of following macro=== */
#define PCM_LOG_LEVEL_DEBUG      (0x1 << (EVA_LOG_DBG-1))
#define PCM_LOG_LEVEL_INFO       (0x1 << (EVA_LOG_INFO-1))
#define PCM_LOG_LEVEL_WARNING    (0x1 << (EVA_LOG_WARN-1))
#define PCM_LOG_LEVEL_ERROR      (0x1 << (EVA_LOG_ERR-1))
#define PCM_LOG_LEVEL_ALL        0xffffffff

/* Debug */
/* pcm Sub Moudles */
typedef enum e_pcm_modid
{
	PCM_MOD_CONFIG,
	PCM_MOD_TX,
	PCM_MOD_RX,
}pcm_modid_e;

/* pcm Sub Moudle Mask */
#define PCM_LOG_MASK_CONFIG		(0x1 << PCM_MOD_CONFIG)
#define PCM_LOG_MASK_TX			(0x1 << PCM_MOD_TX)
#define PCM_LOG_MASK_RX			(0x1 << PCM_MOD_RX)

#if defined (_PCM_DEBUG_ENABLE)

#define LOGDBG_CHECK_PCM_ENABLE (GET_Log_moudle_switch() & EVA_MODULE_MASK_PCM)
#define PCM_RATE_LIMIT  ((jiffies%GET_Log_interval(EVA_LOG_MODULE_PCM)) < GET_Log_times(EVA_LOG_MODULE_PCM))

#define PCM_DBG(mid, lv, fmt, args...) \
	do \
	{ \
		if((GET_Log_level(EVA_LOG_MODULE_PCM) & lv) &&(GET_Log_submoudle_switch(EVA_LOG_MODULE_PCM) & mid )) {\
			if (LOGDBG_CHECK_PCM_ENABLE) \
				pcm_log_format_printk(mid, lv,__func__,__LINE__, fmt, ##args);\
		}\
	} while(0)	
					
#else
#define PCM_DBG(lv, fmt, args...) do {} while(0)
#endif

/*=======================pcm Log end===========================*/


/*=======================vdsp Log =============================*/

/* Vdsp Sub Moudles */
typedef enum e_vdsp_modid
{
	OVDSP_MOD_UL,
	OVDSP_MOD_DL_PCM,
	OVDSP_MOD_DL_DECODE,
	OVDSP_MOD_DB,
	OVDSP_MOD_FR,
	OVDSP_MOD_BUF,
	OVDSP_MOD_TASK,
	OVDSP_MOD_IO_CTL,
	OVDSP_MOD_MUTEX,
	
}vdsp_modid_e;

/* Vdsp Sub Moudle Mask */
#define OVDSP_LOG_MASK_UL		(0x1 << OVDSP_MOD_UL)
#define OVDSP_LOG_MASK_DL_PCM		(0x1 << OVDSP_MOD_DL_PCM)
#define OVDSP_LOG_MASK_DL_DECODE	(0x1 << OVDSP_MOD_DL_DECODE)
#define OVDSP_LOG_MASK_DB		(0x1 << OVDSP_MOD_DB)
#define OVDSP_LOG_MASK_FR		(0x1 << OVDSP_MOD_FR)
#define OVDSP_LOG_MASK_BUF		(0x1 << OVDSP_MOD_BUF)
#define OVDSP_LOG_MASK_TASK		(0x1 << OVDSP_MOD_TASK)
#define OVDSP_LOG_MASK_IO_CTL		(0x1 << OVDSP_MOD_IO_CTL)
#define OVDSP_LOG_MASK_MUTEX		(0x1 << OVDSP_MOD_MUTEX)

/* Vdsp Log Level */
#define OVDSP_LOG_LEVEL_ERR EVA_LOG_MASK_ERR
#define OVDSP_LOG_LEVEL_WARN EVA_LOG_MASK_WARN
#define OVDSP_LOG_LEVEL_INFO EVA_LOG_MASK_INFO
#define OVDSP_LOG_LEVEL_DBG EVA_LOG_MASK_DBG

#define LOG_ENTER(modid) DBGPRINT0_LOGTOOL(modid, OVDSP_LOG_LEVEL_INFO, "Enter. \n\r")
#define LOG_EXIT(modid) DBGPRINT0_LOGTOOL(modid, OVDSP_LOG_LEVEL_INFO, "Exit. \n\r")
#define LOG_ENTER_LIMIT(modid) DBGPRINT0_LIMIT(modid, OVDSP_LOG_LEVEL_INFO, "Enter. \n\r")
#define LOG_EXIT_LIMIT(modid) DBGPRINT0_LIMIT(modid, OVDSP_LOG_LEVEL_INFO, "Exit. \n\r")

#if VDSP_DEBUG
	/* check log switch for vdsp module. */
	#define LOGDBG_CHECK_ENABLE (GET_Log_moudle_switch() & EVA_MODULE_MASK_VDSP)
	/* Limit condition check. */
	#define VDSP_RATE_LIMIT  ((jiffies%GET_Log_interval(EVA_LOG_MODULE_VDSP)) < GET_Log_times(EVA_LOG_MODULE_VDSP))

	/* Basic log printer. */
	#define DBGLOG0(modid, type, fmt) \
		do \
		{ \
			if(LOGDBG_CHECK_ENABLE) {\
					ovdsp_log_format_printk(modid, type,__func__,__LINE__,fmt);\
			}\
		} while(0)

	#define DBGLOG1(modid, type, fmt, arg1) \
		do \
		{ \
			if(LOGDBG_CHECK_ENABLE) {\
				ovdsp_log_format_printk(modid, type,__func__,__LINE__,fmt,arg1);\
			}\
		} while(0)

	#define DBGLOG2(modid, type, fmt, arg1, arg2) \
		do \
		{ \
			if(LOGDBG_CHECK_ENABLE) {\
				ovdsp_log_format_printk(modid, type,__func__,__LINE__,fmt,arg1,arg2);\
			}\
		} while(0)

	#define DBGLOG3(modid, type, fmt, arg1, arg2, arg3) \
		do \
		{ \
			if(LOGDBG_CHECK_ENABLE) {\
				ovdsp_log_format_printk(modid, type,__func__,__LINE__,fmt,arg1,arg2,arg3);\
			}\
		} while(0)
	

	/* Output debug log due to limit condition. */
	#define DBGPRINT0_LIMIT(mid, lv, args...)	\
		do \
		{ \
			if( (GET_Log_submoudle_switch(EVA_LOG_MODULE_VDSP) & mid) && (GET_Log_level(EVA_LOG_MODULE_VDSP) & lv) ) \
			{ \
				if (VDSP_RATE_LIMIT) \
					DBGLOG0(mid, lv, args); \
			} \
		} while (0)	
		
	#define DBGPRINT1_LIMIT(mid, lv, args...)	\
		do \
		{ \
			if( (GET_Log_submoudle_switch(EVA_LOG_MODULE_VDSP) & mid) && (GET_Log_level(EVA_LOG_MODULE_VDSP) & lv) ) \
			{ \
				if (VDSP_RATE_LIMIT) \
					DBGLOG1(mid, lv, args); \
			} \
		} while (0)	
		
	#define DBGPRINT2_LIMIT(mid, lv, args...)	\
		do \
		{ \
			if( (GET_Log_submoudle_switch(EVA_LOG_MODULE_VDSP) & mid) && (GET_Log_level(EVA_LOG_MODULE_VDSP) & lv) ) \
			{ \
				if (VDSP_RATE_LIMIT) \
					DBGLOG2(mid, lv, args); \
			} \
		} while (0)	
		
	#define DBGPRINT3_LIMIT(mid, lv, args...)	\
		do \
		{ \
			if( (GET_Log_submoudle_switch(EVA_LOG_MODULE_VDSP) & mid) && (GET_Log_level(EVA_LOG_MODULE_VDSP) & lv) ) \
			{ \
				if (VDSP_RATE_LIMIT) \
					DBGLOG3(mid, lv, args); \
			} \
		} while (0)	


	/* Output debug log with sub moudle id and log level. */	
	#define DBGPRINT0_LOGTOOL(mid, lv, args...)	\
		do \
		{ \
			if( (GET_Log_submoudle_switch(EVA_LOG_MODULE_VDSP) & mid) && (GET_Log_level(EVA_LOG_MODULE_VDSP) & lv) ) \
			{ \
				DBGLOG0(mid, lv, args); \
			} \
		} while (0) 
		
	#define DBGPRINT1_LOGTOOL(mid, lv, args...)	\
		do \
		{ \
			if( (GET_Log_submoudle_switch(EVA_LOG_MODULE_VDSP) & mid) && (GET_Log_level(EVA_LOG_MODULE_VDSP) & lv) ) \
			{ \
				DBGLOG1(mid, lv, args); \
			} \
		} while (0) 
		
	#define DBGPRINT2_LOGTOOL(mid, lv, args...)	\
		do \
		{ \
			if( (GET_Log_submoudle_switch(EVA_LOG_MODULE_VDSP) & mid) && (GET_Log_level(EVA_LOG_MODULE_VDSP) & lv) ) \
			{ \
				DBGLOG2(mid, lv, args); \
			} \
		} while (0) 
		
	#define DBGPRINT3_LOGTOOL(mid, lv, args...)	\
		do \
		{ \
			if( (GET_Log_submoudle_switch(EVA_LOG_MODULE_VDSP) & mid) && (GET_Log_level(EVA_LOG_MODULE_VDSP) & lv) ) \
			{ \
				DBGLOG3(mid, lv, args); \
			} \
		} while (0) 
	#define VDSP_ASSERT(exp, args...) \
	do \
	{ \
		char tmpBuf[64]; \
		sprintf(tmpBuf, args); \
		CERR_Assert(exp, tmpBuf); \
	} while (0)
	
#else
	#define DBGPRINT0(lv, args...) 
	#define DBGPRINT1(lv, args...) 
	#define DBGPRINT2(lv, args...) 
	#define DBGPRINT3(lv, args...) 
	#define DBGPRINT0_LIMIT(mid, lv, args...)
	#define DBGPRINT1_LIMIT(mid, lv, args...)
	#define DBGPRINT2_LIMIT(mid, lv, args...)
	#define DBGPRINT3_LIMIT(mid, lv, args...)
	#define DBGPRINT0_LOGTOOL(mid, lv, args...)
	#define DBGPRINT1_LOGTOOL(mid, lv, args...)
	#define DBGPRINT2_LOGTOOL(mid, lv, args...)
	#define DBGPRINT3_LOGTOOL(mid, lv, args...)
	#define VDSP_ASSERT(exp, args...) 
#endif
/*=======================vdsp Log end==========================*/


#endif /* __EVA_LOG_H__ */

/* 
 * $Log: eva_log.h,v $
 * Revision 0.0.1  2016-05-31 15:06:00  mtk06404
 */
 

