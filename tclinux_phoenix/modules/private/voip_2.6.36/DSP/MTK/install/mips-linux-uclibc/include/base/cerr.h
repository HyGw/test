/******************************************************************************
 *	LEGAL DISCLAIMER
 *
 *	(HEADER OF MEDIATEK SOFTWARE/FIRMWARE RELEASE OR DOCUMENTATION)
 *
 *	(c) 2008 MediaTek Inc.  ALL RIGHTS RESERVED.
 *
 *	BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND 
 *	AGREES THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS 
 *	(MEDIATEK SOFTWARE) RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE 
 *	PROVIDED TO BUYER ON AN AS IS BASIS ONLY.  MEDIATEK EXPRESSLY DISCLAIMS 
 *	ANY AND ALL WARRANTIES, WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT 
 *	LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
 *	PARTICULAR PURPOSE, OR NON-INFRINGEMENT.  NOR DOES MEDIATEK PROVIDE ANY 
 *	WARRANTY WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PART(IES) 
 *	WHICH MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK 
 *	SOFTWARE.  BUYER AGREES TO LOOK ONLY TO SUCH THIRD PART(IES) FOR ANY AND ALL 
 *	WARRANTY CLAIMS RELATING THERETO.  NEITHER SHALL MEDIATEK BE RESPONSIBLE FOR 
 *	ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION OR CONFORMING 
 *	TO A PARTICULAR STANDARD OR OPEN FORUM.
 *
 *	BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE 
 *	LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER SHALL BE, 
 *	AT MEDIATEK'S SOLE OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT 
 *	ISSUE OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGES PAID BY BUYER 
 *	TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE. 
 *
 *	THE MEDIATEK SOFTWARE IS PROVIDED FOR AND ONLY FOR USE WITH MEDIATEK CHIPS 
 *	OR PRODUCTS.  EXCEPT AS EXPRESSLY PROVIDED, NO LICENSE IS GRANTED BY 
 *	IMPLICATION OR OTHERWISE UNDER ANY INTELLECTUAL PROPERTY RIGHTS, INCLUDING 
 *	PATENT OR COPYRIGHTS, OF MEDIATEK.  UNAUTHORIZED USE, REPRODUCTION, OR 
 *	DISCLOSURE OF THE MEDIATEK SOFTWARE IN WHOLE OR IN PART IS STRICTLY 
 *	PROHIBITED.
 *
 *	THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH 
 *	THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF 
 *	LAWS PRINCIPLES.  
 */
/**
 *	@file  cerr.h
 *	@brief c-style exception handling, 
 *		condition: -D _NODEBUG_ for debugging message elimination
 *
 *	@author Howard Chen
 *	@date   2006 , 6
 *
 **/

#ifndef CERR_0629
#define CERR_0629
#include "link_base.h"

/// max error message
#define CERR_DEF_MSG					32

/// max trace message
#define CERR_DEF_TRACE					128

#ifdef PORK_MODIFY
#define exc_t int
#else
typedef const struct cerr *exc_t;
#endif

/**
 *	@brief c-style exception handling object
 */
typedef struct cerr {
	int errnum;
	int line;
	const char *file;
	char *msg;
	int nmsg;
	char *trace;
	int trace_sz;
	char *(*get_msg) (exc_t o);
} cerr_t;

/**
 *	@brief the cerr_t constructor using the default(shared) message buffer
 *	@see _CERR_Init
 */
#define CERR_Init(_err)   			_CERR_Init(_err, 0, 0, 0, 0, 0)

/** 
 *	@brief the cerr_t constructor which uses a dynamic err msg model 
 */
#define CERR_InitD(_err, _f) 		_CERR_Init(_err, 0, 0, 0, 0, _f)

/**
 *	@brief check if object has been initialized
 */
#define CERR_IsInit(_err)			((_err)->msg!=0)

/**
 *	@brief the cerr_t constructor 
 *	@param msg buffer for error message
 *	@param nmsg sizeof <msg>
 *	@param trace buffer for the trace message
 *	@param trace_sz sizeof <trace>
 *	@param function which convert errnum to string
 *	@return err itself
 */
LINK_BASE exc_t _CERR_Init(cerr_t * err,
			   char *msg, unsigned int nmsg,
			   char *trace, unsigned int trace_sz,
			   char *(*get_msg) (exc_t err));

/**
 *	@brief the cerr_t constructor 
 *  @param ec <err> set its initial values according to <ec>
 *	@return err itself
 */
LINK_BASE exc_t CERR_Clone(cerr_t * err, exc_t ec);

/**
 *	@brief report <ec> as a fatal error and halt system
 */
LINK_BASE void CERR_Fatal(exc_t ec);

typedef void (*cerr_fatal_t)(exc_t ec);

/**
 *	@brief 
 *	@param fatal a call-back during CERR_Fatal is triggered
 */
LINK_BASE cerr_fatal_t CERR_SetFatalHandler(cerr_fatal_t fatal);

LINK_BASE exc_t _CERR_Set(cerr_t * err, const char *msg,
			  int _errno, const char *file, int line);

/**
 *	@brief print content of <_ec> in string 
 *	@return <buf>
 */
LINK_BASE char *_CERR_ToString(exc_t _ec, char *buf, unsigned sz);

LINK_BASE void _CERR_FatalMsg(const char *msg, const char *file,
			      unsigned line);

LINK_BASE void _CERR_FatalCode(unsigned file_code, unsigned line);

#if defined _NODEBUG_

#define CERR_Assert(_exp, _msg)	

#define CERR_Set(_err, _msg, _n) 	_CERR_Set(_err, CERR_MSG_EMPTY, _n, CERR_MSG_EMPTY, 0)

#define CERR_Forward(_err, _ec)	 	return (_ec)

LINK_BASE extern const char* CERR_MSG_EMPTY;

#else

#if defined ASSERT_FILE_ID
#define CERR_Assert(_exp, _msg)	\
		if(!(_exp))	_CERR_FatalCode(ASSERT_FILE_ID, __LINE__)
#else
#ifdef PORK_MODIFY
#define CERR_Assert(_exp, _msg)	\
		do{}while(0)
#else
#define CERR_Assert(_exp, _msg)	\
		if(!(_exp))	_CERR_FatalMsg(_msg, __FILE__, __LINE__)
#endif

#endif

#define CERR_Set(_err, _msg, _n) _CERR_Set(_err, _msg, _n, __FILE__, __LINE__)			

#define CERR_Forward(_err, _ec)								\
	do{	CERR_Assert(CERR_IsInit(_err), CERR_MSG_INIT);		\
		return _CERR_Forward(_err, _ec, __FILE__, __LINE__);}while(0)

LINK_BASE extern const char* CERR_MSG_INIT;

/**
 *	@brief forward error code
 *	@param _err
 *	@param _ec	error code
 */
LINK_BASE exc_t _CERR_Forward(cerr_t * err, exc_t ec,
			      const char *file, int line);

/**
 *	@brief printf format error message
 */
LINK_BASE const char *CERR_Fmt(cerr_t * err, const char *fmt, ...);
				
#endif	/* _NODEBUG_ */

LINK_BASE extern int cerr_dummy;

/**
 *	@brief execute _exp, assert only if !_NODEBUG_
 */
#define CERR_DoAssert(_exp,_msg)		\
	do{	cerr_dummy = (_exp);			\
		CERR_Assert(cerr_dummy, _msg);	\
	}while(0)

#ifdef PORK_MODIFY
#define CERR_ToString(_err, _buf, _sz)						\
	do{}while(0)
#else
#define CERR_ToString(_err, _buf, _sz)						\
	do{	CERR_Assert(CERR_IsInit(_err), CERR_MSG_INIT);		\
		_CERR_ToString(_err, _buf, _sz);}while(0)
#endif


/**
 *	@brief return error with message
 *	@param _cerr
 *	@param _msg	the error message
 */
#define CERR_Msg(_err,_msg)	return CERR_Set(_err, _msg, 0)

/**
 *	@brief return error with error number
 *	@param _err
 *	@param _num	the error number
 */
#define CERR_Num(_err,_num)	return CERR_Set(_err, (const char*)0, _num)

/**
 *	@brief return success status
 */
#define CERR_Ok()					return (exc_t)0

/**
 *	@brief execute <_exp> if fail, forward error code
 */
#define CERR_TryF(_err,_ec,_exp)	if(((_ec)=(_exp))) CERR_Forward(_err,_ec)

/**
 *	@brief execute <_exp> if fail, terminate process
 */
#define CERR_TryA(_ec,_exp) 		if(((_ec)=(_exp))) CERR_Fatal(_ec)

/**
 *	@brief execute <_exp>, whose value is system call return, forward if fail 
 */
#define CERR_TryS(_err, _exp)						\
	do{												\
		char buf[16];								\
		if((_exp) < 0)								\
			CERR_Msg(_err, strerror_r(errno, buf, sizeof(buf)));\
	}while(0)

#define CERR_Print(_ec, _buf_sz)					\
	do{												\
		char buf[_buf_sz];							\
		CERR_ToString(_ec, buf, sizeof(buf));		\
		printf("%s\n", buf);						\
	}while(0)

#endif
