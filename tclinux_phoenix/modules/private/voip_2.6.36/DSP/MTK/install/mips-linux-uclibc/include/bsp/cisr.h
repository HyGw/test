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
 *  @file cisr.h 
 *	@author Howard Chen, Terry Liu
 *	@brief interrupt service routine 
 */
#ifndef ISR0901_DEF
#define ISR0901_DEF

#include <base/cerr.h>
#include "link_bsp.h"
#include "syss.h"

/**
 *	@brief return by an isr
 */
typedef enum  
{
    CISR_DONE	  = 1,    ///< Interrupt was handled
    CISR_CALL_DSR = 2     ///< Schedule DSR
}cisr_status_t;

/**
 *	@brief the isr handler, which is execute in an ISR context
 *			with interrupt disabled 
 *	@return CISR_CALL_DSR if dsr is required for subsequent handling
 *			or CISR_DONE
 *	@see cisr_isr_ret_t
 */
typedef cisr_status_t (*cisr_isr_t)(unsigned irq, void* data);

typedef void (*cisr_dsr_t)(unsigned irq, unsigned count, void* data);

struct cisr{

	exc_t (*init)(struct cisr* o, cisr_isr_t isr, int is_level_trigger, void* data);

	exc_t (*setDSR)(struct cisr* o, cisr_dsr_t dsr);

	exc_t (*attach)(struct cisr* o, unsigned irq);

	exc_t (*detach)(struct cisr* o, unsigned irq);
	
	exc_t (*dest)(struct cisr* o);

	unsigned reserved[4];///< reserved for more method without changing offset
	
	cisr_isr_t isr;
	
	cisr_dsr_t dsr;
	
	void*   data;			///< argument to isr & dsr
	
	int 	level_triger; 

};

#define CISR_MAX_SIZE				(sizeof(cisr_t) + (sizeof(unsigned)<<4))

#define CISR_Size()					({			\
			syss_t* syss = SYSS_GetInstance();	\
			syss->cisrSize(syss);				\
		})

#define CISR_Init(_o, _isr, _l, _d)			\
	do{										\
		syss_t* syss = SYSS_GetInstance();	\
		syss->cisrGen(syss, _o);			\
		(_o)->init(_o,_isr,_l,_d);			\
	}while(0)

#define CISR_SetDSR(_o, _dsr)		(_o)->setDSR(_o, _dsr)

#define CISR_Attach(_o, _irq)		(_o)->attach(_o, _irq)

#define CISR_Detach(_o, _irq)		(_o)->detach(_o, _irq)
	
#define CISR_Dest(_o)				(_o)->dest(_o)

#define CISR_MAX_CONTEXT_SIZE		(sizeof(unsigned int) * 4)		///< Maximum size of CISR context to process CISR context switching (SAVE/RESTORE) */

/**
 *	@brief	store the original context of CISR for IRQ (irq) into context. 
 *			And clear the related context for context update
 *	@param	irq	
 *	@param  context unsigned array, should larger than CISR_MAX_CONTEXT_SIZE
 */
LINK_BSP void CISR_Save(unsigned irq, unsigned* context);

/**
 *	@brief  re-store the original contextl back for IRQ (irq) into context which gotten in CISR_Save()
 */
LINK_BSP void CISR_Restore(unsigned irq, unsigned* context);

#define CISR_PARAM_LEVEL_TRIGGER	(1)					///< Parameter passed into CISR module (in CISR_Init()) : Level trigger 

#define CISR_PARAM_PULSE_TRIGGER	(0)					///< Parameter passed into CISR module (in CISR_Init()) : Pulse trigger

#ifdef NEWLIB

LINK_BSP void CISR_HandleIRQ(void);

#endif

#endif

