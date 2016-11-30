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
 *	@file  utest.h 
 *	@brief the assertion-based unit test framework. A unit testsuite
 *         is composites of unit test cases. Each test case is 
 *         a user defined function. To build a unit test module, 
 *         The test module simply add all its test functions with the 
 *         UTST_Add() and then invoke UTST_Run()
 *
 *	@author Howard Chen
 *	@date   2006 , 7
 *
 **/
#ifndef UTEST_0630
#define UTEST_0630

#include "cerr.h"
#include "llist.h"

/**
 *	@brief unit test assertion
 *  @param _exp the _exp must be true, or the test fails  
 */
#define UTST(_exp)	if(!(_exp)) CERR_Msg(&UTST_err, "unit test fail");

/**
 *	@brief unit test assertion with handler
 *  @param _exp the _exp must be true, or the test fails 
 *  @param _stat if the statment fails, do statment 
 */
#define UTST_DO(_exp, _sta)	\
	do{						\
		if(!(_exp)){ 		\
			(_sta);			\
			CERR_Msg(&UTST_err, "unit test fail");	\
		}					\
	}while(0)

/**
 *	@brief <_exp> is a expression whose type is exc_t, and should be no error
 */
#define UTST_EXC(_exp)			\
	do{							\
		exc_t ec;				\
		if((ec = (_exp)))		\
			CERR_Fatal(ec);		\
	}while(0)
	
extern cerr_t UTST_err;

/** 
 *	@brief the test case type
 */
typedef exc_t(*utest_cast_t) (void *);

/**
 *	@brief default maximal number of test cast
 */
#define	UTST_DEF_NTST		1024

/**
 *	@brief function which is used to allocate memory
 */
typedef void *(*utest_alloc) (unsigned int sz);

/** 
 *	@brief set up a function which is used to allocate memory
 *		   on default, a fixed size () circular buffer is used 
 */
LINK_BASE void UTST_Init(utest_alloc _malloc);

#define UTST_Add(_tc)	_UTST_Add(_tc, #_tc)

/** 
 *	@brief add a new test_case 
 */
LINK_BASE void _UTST_Add(utest_cast_t test_case, char *name);

LINK_BASE void UTST_Fatal(exc_t ec);

#define UTST_Run(_p)	_UTST_Run(_p, __FILE__, 0, UTST_Fatal)

#define UTST_RunNoStop(_p)	_UTST_Run(_p, __FILE__, 1, UTST_Fatal)

/**
 *	@brief run all tests
 *  @param p user defined 
 *  @param no_stop do not halt even if a case failed
 */
LINK_BASE void _UTST_Run(void *p, const char* file, int no_stop,void (*fatal)(exc_t ec) );

/**
 *	@brief halt the system, whatever on
 *		PC: win32/ linux
 *		ARM:RTL/ T32 Simulator/ GDB Simulator / T32 ICE
 *
 */
LINK_BASE void UTST_Halt(int);

typedef struct utest {
    utest_cast_t test_case;
    llist_t link;
    char *name;
} utest_t;


LINK_BASE llist_t *UTST_GetList(void);
	
#endif
