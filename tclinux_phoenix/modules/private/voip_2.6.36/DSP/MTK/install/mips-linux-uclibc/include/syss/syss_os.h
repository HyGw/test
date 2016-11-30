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
 *  @file syss_os.h
 *	@author Howard Chen
 *	@brief the RTOS system service interface
 */
#ifndef SSERVICE_DEF
#define SSERVICE_DEF

#include <base/asm.h>
#include <bsp/syss.h>

#include "link_syss.h"
#include "cmem.h"
#include "cmemv.h"
#include "cmqueue.h"
#include "cmutex.h"
#include "csema.h"
#include "ctask.h"
#include "ctimer.h"
#include "cwait.h"
#include "conf_syss.h"

#define SYSS_IntDisable(_s)       \
    __asm__ volatile(             \
             arm_asm_beg(r1)      \
            "MRS %0, CPSR\n"      \
            "ORR r1, %0, #0x80\n" \
            "MSR CPSR_cxsf, r1"   \
             arm_asm_end(r1)      \
            :"=r"(_s)             \
            :                     \
            :"r1");

#define SYSS_IntRestore(_s)     \
    __asm__ volatile(           \
             arm_asm_beg(r1)    \
            "MSR CPSR_cxsf, %0" \
             arm_asm_end(r1)    \
            :                   \
            :"r"(_s)            \
            :"r1");

#ifdef NEWLIB
	#include <tx/tx_api.h>
	#include <tx/tx_thread.h>

	/**
	 *	@brief lock the scheduler to prevent context switch
	 */
	#define SYSS_SchedLock()	_tx_thread_preempt_disable++

    /**
	 *	@brief unlock the scheduler
	 */
	#define SYSS_SchedUnlock()	_tx_thread_preempt_disable--
#else
#ifndef LINUX
#include <ecos/cyg/kernel/kapi.h>
#endif

	/**
	 *	@brief lock the scheduler to prevent context switch
	 */
	#define SYSS_SchedLock()	cyg_scheduler_lock()

    /**
	 *	@brief unlock the scheduler
	 */
	#define SYSS_SchedUnlock()	cyg_scheduler_unlock()

#endif

/**
 *	@brief the minimal block size of the memory pool object which can
 *			added to system is (2^SYSS_MEM_MIN_LOG)
 */
#define SYSS_MEM_MIN_LOG			3

/**
 *	@brief the maximal block size of the memory pool object which can
 *			added to system is (2^SYSS_MEM_MAX_LOG)
 */
#define SYSS_MEM_MAX_LOG			11

/**
 *	@brief add a block memory pool to system memory block manager
 */
LINK_SYSS exc_t SYSS_MemAdd(cmem_t* mem);

/**
 *	@brief the system memory block manager chooses a memory block pool
 *         whose block size is minimal but still bigger than [sz] to allocate the
 *		   memory, the system memory block manager automatically upscales
 *         until an memory block is available or return 0 for failure.
 */
LINK_SYSS void* SYSS_Malloc(unsigned sz);

/**
 *	@brief the same with SYSS_Malloc, but without automatic scale up
 */
LINK_SYSS void* SYSS_MallocNS(unsigned sz);

/**
 *	@brief free memory
 */
LINK_SYSS exc_t SYSS_Free(void* p);

#define SYSS_ERR_SPACE(_i)	(0x7700 + ((_i) << 4))

/**
 *	@brief the whole system service used a Num-Mode CERR for error handling,
 *		   all objects share the same error space which is partitioned
 *		   for each object type
 *	@see SYSS_ERR_SPACE
 */
typedef enum {

	SYSS_ERR = SYSS_ERR_SPACE(0),

	CMEM_ERR = SYSS_ERR_SPACE(1),

	CMEMV_ERR= SYSS_ERR_SPACE(2),

	MQUE_ERR = SYSS_ERR_SPACE(3),

	MUTX_ERR = SYSS_ERR_SPACE(4),

	SEMA_ERR = SYSS_ERR_SPACE(5),

	TASK_ERR = SYSS_ERR_SPACE(6),

	TIME_ERR = SYSS_ERR_SPACE(7),
}syss_os_err_t;

/**
 *	@brief the hardware timer frequency
 */
enum {
	SYSS_GPT_16K = 0,
	SYSS_GPT_8K  = 1,
	SYSS_GPT_4K	 = 2,
	SYSS_GPT_2K  = 3,
	SYSS_GPT_1K  = 4,
	SYSS_GPT_512 = 5,
	SYSS_GPT_256 = 6,
	SYSS_GPT_128 = 7,
};

/**
 *	@brief the rtos system service interface, which cooperate with
 *			<bsp/syss.h> to provide OS service
 *	@see syss_t
 */
typedef struct syss_os{

	/**
	 *	@brief a stack overflow error handler.
	 *	This method is invoked when detecting a stack overflow error
	 */
	void (*stackErrListener)(struct syss_os* o, ctask_t* t);

	/**
	 *	@brief get a os maintained fast memory pool
	 *	@return 0 if not available
	 */
	cmemv_t* (*getFastMem)(struct syss_os* o);

	/**
	 *	@brief set system hardware timer handler
	 *	@param no a virtual number of hardware timer. For example
	 *		   if there're four hardware timer, then one is dedicated to OS
	 *		   and the other three is numbered as 1~3
	 *	@param freq used enum { SYSS_GPT_XX ...}
	 *	@param cnt
	 *	@param is_repeat one shot or repeative
	 *	@param notify handler function which is execute in DSR context
	 *
	 *	@note  application with system service should never use <bsp/ddr_gpt.h>
	 *		   directly, it should used setHWTimer instead
	 */
 	exc_t (*setHWTimer)(
		struct syss_os* o,
		unsigned no,
		unsigned freq,
		int cnt,
		int is_repeat,
		void (*notify)(unsigned irq, void* data),
		void* data);

	/**
	 *	@brief stop hardware timer
	 */
	exc_t (*stopHWTimer)(struct syss_os* o, unsigned no);

	/**
	 *	@brief get current system ticks
	 */
	unsigned (*getClock)(struct syss_os* o);

	/**
	 *	@brief get error message
	 */
	const char* (*getErrMsg)(struct syss_os* o, exc_t exc);

}syss_os_t;

/**
 *	@brief a factory method get the singeleton syss_os object
 */
LINK_SYSS syss_os_t* SYSS_GetOS(void);

#define SYSS_InitAlloc(_sz)		({		\
		syss_t* o = SYSS_GetInstance();	\
		o->memAlloc(o, _sz);			\
	})

#define SYS_InitAlloc(_sz)		SYSS_InitAlloc(_sz)

/**
 *	@brief for each module to add as the last unit test case
 *			to reset system for the next test image to run
 */
LINK_SYSS exc_t SYSS_UTReset(void* p);


#if defined(__KERNEL__)	
	#define getFastMem		#error NOT_SUPPORT_getFastMem
	#define setHWTimer		#error NOT_SUPPORT_setHWTimer
	#define stopHWTimer		#error NOT_SUPPORT_stopHWTimer
#endif

#endif
