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
 *  @file ctask.h
 *	@author Howard Chen, LT Chang
 *	@brief	task object
 */
#ifndef TASK0901_DEF
#define TASK0901_DEF

#include <base/cerr.h>
#include "link_syss.h"

typedef struct ctask ctask_t;

/**
 *	@brief get current task object
 */
LINK_SYSS ctask_t* TASK_CurTask(void);

/**
 *	@brief maxium size of task
 */
#define TASK_MAX_SIZE		64

/**
 *	@brief get the size of a task object
 */
LINK_SYSS unsigned TASK_Size(void);

/**
 *	@brief init a task
 * @param o task object
 * @param name the name of the task
 * @param entry_function the entry point of the task
 * @param arg the argument which will be passed to entry function
 * @param stack_start the start address of stack 
 * @param stack_sz the stack size
 * @param priority the task priority
 */
LINK_SYSS exc_t TASK_Init(ctask_t* o, 
				char* name,
				void (*entry_function)(void* arg),
                const void* arg, 
				void* stack_start,
				unsigned stack_sz,
				unsigned priority);

/**
 *	@brief destroy a task object
 * @param o a task object 
 */
LINK_SYSS exc_t TASK_Dest(ctask_t* o);

/**
 *	@brief set task priority
 *	@param o a task object
 * 	@param priority priority of the task
 */
LINK_SYSS exc_t TASK_SetPriority(ctask_t* o, unsigned priority);

/**
 *	@brief get task priority
 *	@param o a task object
 * 	@param priority priority of the task
 */
LINK_SYSS unsigned TASK_GetPriority(ctask_t* o);

typedef enum{
	TASK_CTRL_INFO_STACK,
}ctask_ioctl_t;

/**
 *	@brief destroy a task object
 * @param o a task object 
 * @param req requested information type
 */
LINK_SYSS exc_t TASK_Ctrl(ctask_t* o, ctask_ioctl_t req, ...);

/**
 *	@brief cause a task to sleep
 * @param tick sleep period
 */
LINK_SYSS exc_t TASK_Sleep(unsigned tick);

/**
 *	@brief suspend a task
 * @param o a task object 
 */
LINK_SYSS exc_t TASK_Suspend(ctask_t* o);

/**
 *	@brief resume a task
 * @param o a task object 
 */
LINK_SYSS exc_t TASK_Resume(ctask_t* o);

/**
 *	@brief terminate a task
 * @param o a task object 
 */	
LINK_SYSS exc_t TASK_Terminate(ctask_t* o);

/**
 *	@brief relinquish CPU
 */
LINK_SYSS exc_t TASK_Relinquish(void);

/**
 *	@brief check if a task is suspended
 * @param o a task object 
 */	
LINK_SYSS unsigned TASK_IsSuspending(ctask_t* o);

/**
 *	@brief check stack
 * @param o a task object 
 */	
LINK_SYSS void  TASK_CheckStack(ctask_t* o);

/**
 *	@brief get task name
 * @param o a task object 
 */	
LINK_SYSS const char* TASK_GetName(ctask_t* o);

/** 
 *	@param req == TASK_CTRL_INFO_STACK
 */
exc_t task_ctrl_info_stack(ctask_t* o, ctask_ioctl_t req, 
		unsigned* stack_addr, unsigned* stack_sz, unsigned* stack_usage);

#if defined(LINUX)

	#define TASK_Ctrl			#error "do not support Task_Ctrl"
	#define TASK_CheckStack		#error "do not support Task_CheckStack"
	#define TASK_IsSuspending 	#error "do not support Task_CheckStack"
	
#endif

#endif
