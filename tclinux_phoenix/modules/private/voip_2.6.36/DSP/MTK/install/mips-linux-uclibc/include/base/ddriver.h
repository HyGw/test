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
 *	@file 	ddriver.h
 *	@brief  the generic device driver interface 
 *	@author Howard Chen
 *
 */
#ifndef DDDRIVER_DEF
#define DDDRIVER_DEF

#include "cerr.h"

typedef enum    e_ddriver_status
{
    DDR_STATUS_INIT   =   0x10,
} E_DDR_STATUS;

#define DDR_PRIV_SIZE		(8*sizeof(void*))
/**
 *	@brief the generic device driver object.
 *		   which defines the operation hooks
 *		   Not all device support all the operations.
 *		   Operation hooks which is not supported are set to null
 *			
 */
typedef struct ddriver {

    /**
     *  @brief driver name
     */
    char *name;

    /**
     *  @brief driver status
     */
    int status;

    /**
     *  @brief device open function, 
     *         use variable arguments to initialize 
     */
     exc_t(*open) (struct ddriver * o, ...);

    /**
     *  @brief device close function
     */
     exc_t(*close) (struct ddriver * o);

    /**
     *  @brief device read function
     *  @param buff  buffer for reading
     *  @param count sizeof <buff>
     *  @param offp  the offset for read. Only meaningful when device
     *               support random access
     */
     exc_t(*read) (struct ddriver * o, char *buff,
               unsigned int count, unsigned int *offp);

    /**
     *  @brief device write function
     *  @param buff  buffer to write
     *  @param count sizeof <buff>
     *  @param offp  the offset for write. Only meaningful when device
     *               support random access
     */
     exc_t(*write) (struct ddriver * o, const char *buff,
            unsigned int count, unsigned int *offp);

    /**
     *  @brief device ioctl function. It is used to model all 
     *          operations which can not be modeled by read & write
     *  @param request the request type
     */
     exc_t(*ioctl) (struct ddriver * o, int request, ...);

    /**
     *  @brief the cerr object used internal for excetpion handling
     */
    cerr_t err;

    /**
     *  @brief the extension data pointer
     */
    void *priv[DDR_PRIV_SIZE / sizeof(void *)];

} ddriver_t;


#endif
