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
 *  @file cmemv.h
 *	@author	Howard Chen
 *	
 *	@brief  memory pool object, which is used for variable size memory 
 *			management
 *
 */
#ifndef CMEMV_DEF
#define CMEMV_DEF

#include <base/cerr.h>
#include "cwait.h"
#include "link_syss.h"

#define CMEMV_MAX_SIZE		90


/** 
 *	@brief get the size of cmemv_t 
 */
LINK_SYSS unsigned CMEMV_Size(void);

/**
 *	@brief a fixe-sized memory pool
 */

typedef struct cmemv cmemv_t;

#define CMEMV_SIGNATURE				0x89abcdef
#define CMEMV_Alloc(_sz)	({              \
	unsigned *ptr = 0;			\
	syss_t* o = SYSS_GetInstance();		\
	ptr = o->memAlloc(o, _sz);		\
	*ptr = CMEMV_SIGNATURE;			\
	ptr;					\
        })

/**
 *	@brief initial the memory pool object
 *	@param start the start address which must be got from CMEMV_Alloc macro
 *	@param the memory size which is pointed by <start>
 */
LINK_SYSS exc_t CMEMV_Init(cmemv_t* o, char *name, void* start, unsigned sz);

LINK_SYSS exc_t CMEMV_Dest(cmemv_t* o);

/**
 *	@brief alloca memory from memory pool object
 *	@param sz 	memory size
 *	@param wait 
 */
LINK_SYSS void* CMEMV_Malloc(cmemv_t* o, unsigned sz, cwait_t wait);

/**
 *  @brief get the original memory pool object
 *	@param memory_ptr
 *	@return the memory pool object which the memory_ptr belongs to 
 *			0 if can not find 
 */
LINK_SYSS cmemv_t* CMEMV_GetOwner(void* memory_ptr);


LINK_SYSS exc_t CMEMV_Free(void* mem); 

typedef enum{
	CMEMV_GET_USAGE,
}cmemv_ctrl_t;

LINK_SYSS exc_t CMEMV_Ctrl(cmemv_t* o, cmemv_ctrl_t req, ...);

/**
 *  @brief the the current memory pool utilization
 *	@param req == CMEMV_GET_USAGE
 */
exc_t cmemv_ctrl_get_usage(cmemv_t* o, cmemv_ctrl_t req, 
	unsigned* total_bytes, unsigned* free_bytes);

#endif

