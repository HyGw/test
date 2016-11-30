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
 *  @file ctimer.h
 *	@author Howard Chen, HC Chu
 *	@brief timer oect
 */
#ifndef STIMER_DEF
#define STIMER_DEF

#include <base/cerr.h>
#include "link_syss.h"

#define TIME_MAX_SIZE	0x30

typedef struct ctimer	ctimer_t;

/**
 *	@brief returns the ctimer_t struct size
 */
LINK_SYSS unsigned TIME_Size(void);

/**
 *	@brief timer initialization function
 *  @param o : ctimer_t object
 *	@param name : name of the ctimer_t object
 *	@param expiration_func : function that will be executed when timer expire
 *	@param argumennt : argument for the expiration function
 *	@param initial_time (tick): the first expiration period 
 *	@param reschedule_time (tick): after first exipration, new timer expiration period(periodic)
 *	                         0 for one shot timer, which could also be started again by
 *							 TIME_Activate
 */
LINK_SYSS exc_t TIME_Init(ctimer_t* o,
				 char* name, 
                 void (*expiration_func)(void* argument),
                 void* argument,
                 unsigned initial_time,
                 unsigned reschedule_time);

/**
 *	@brief timer destroy funciton
 */
LINK_SYSS exc_t TIME_Dest(ctimer_t* o);

/**
 *	@brief timer activation funciton
 */
LINK_SYSS exc_t TIME_Activate(ctimer_t* o);

/**
 *	@brief stop current event, and restart timer 
 */
LINK_SYSS exc_t TIME_Reset(ctimer_t* o, 
				unsigned initial_time,
				unsigned reschedule_time);

/**
 *	@brief timer deactivation funciton
 */
LINK_SYSS exc_t TIME_Deactivate(ctimer_t* o);

/**
 *	@brief timer control funciton
 *	@brief req = 0, set delay-able timer, means the timer function could be delayed 
 *         req = 1, query timer status
 *         req = 2, query timer pid
 *					
 *
 */
LINK_SYSS exc_t TIME_Ctrl(ctimer_t*o, int req, ...);

#ifdef LINUX
#include <linux/param.h>

#define TICK_TO_MS(x)	(1000*x/HZ)
#define MS_TO_TICK(x)	(x*HZ/1000)
#else
//ecos TICKS_PER_SEC
#endif

#endif



