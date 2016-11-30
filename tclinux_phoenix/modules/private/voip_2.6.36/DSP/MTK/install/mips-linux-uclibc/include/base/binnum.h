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
 * 	@file binnum.h
 *	@brief binary number utilities 
 *	@author Howard Chen
 * 
 */
#ifndef BINNUM_DEF
#define BINNUM_DEF
#include "link_base.h"

#define BU_LOG2(_v)				(((_v)==64)? 6:					\
									((_v)==32)? 5: 				\
										((_v)==16)? 4: 			\
											((_v)==8)? 3: 		\
												((_v)==4)? 2: 	\
													((_v)==2)? 1:	\
														((_v)==0)? 0: -1)

#define BU_T_MSK(_n)				((1<<(_n))-1)
#define BU_F_MSK(_ms,_ls)			(BU_T_MSK((_ms)-(_ls)+1)<<(_ls))
#define BU_B_MSK(_n)				(1 << (_n))
#define BU_WID(_ms,_ls)				((_ms)-(_ls)+1)

#define BU_ALN(_i, _n)				((_i) & ~BU_T_MSK(_n))

#define BU_RUP(_i, _n)				BU_ALN((_i)+BU_T_MSK(_n), _n)

/**
 *	@brief trucate <_i> to <_n> bits
 */
#define BU_TRU(_i, _n)				((_i) & BU_T_MSK(_n))

/**
 *	@brief get (<_ms>:<_ls>) field
 */
#define BU_G_FLD(_i,_ms,_ls)	    (((_i)<<(31-(_ms))) >> (31- (_ms) + (_ls)))

/**
 *	@brief or (<_ms>:<_ls>) field
 */
#define BU_O_FLD(_i,_ms,_ls,_v)   	((_i)|((BU_TRU(_v,BU_WID(_ms,_ls))<<(_ls))))

/**
 *	@brief clean (<_ms>:<_ls>) field
 */
#define BU_C_FLD(_i,_ms,_ls)		((_i) & ~BU_F_MSK(_ms,_ls))

/**
 *	@brief replace (<_ms>:<_ls>) field with <_v>
 */
#define BU_R_FLD(_i,_ms,_ls,_v)		((((_i)&~BU_F_MSK(_ms,_ls)))|((BU_TRU(_v,BU_WID(_ms,_ls))<<(_ls))))
/**
 *	@brief get <_n> bit
 */
#define BU_G_BIT(_i,_n)				BU_G_FLD(_i, _n, _n)

/**
 *	@brief or <_n> bit with 0x1
 */
#define BU_O_BIT(_i,_n)				((_i) |(1 << (_n)))

/**
 *	@brief clean <_n> bit
 */
#define BU_C_BIT(_i,_n)				((_i) & ~BU_B_MSK(_n))

/**
 *	@brief replace <_n> bit with <_v>
 */
#define BU_R_BIT(_i,_n,_v)			(((_i)&~BU_B_MSK(_n))|(((_v)&0x1)<<(_n)))

enum BU_MEM_UNIT { BU_BYTE = 1, BU_HWORD = 2 , BU_WORD = 4 };

/**
 *	@brief if running on a big endian target
 */
LINK_BASE int BU_IsHostBig(void);

LINK_BASE void BU_Load(const unsigned char *mem,
		       unsigned int *value, enum BU_MEM_UNIT unit,
		       int change_endian);

#define BU_Store(_mem, _value, _unit,_change_endian) \
		BU_CpyMem((unsigned char*)&(_value),_mem, _unit,_change_endian)

LINK_BASE void BU_CpyMem(const unsigned char *from,
			 unsigned char *to, enum BU_MEM_UNIT unit,
			 int change_endian);

/**
 *	@brief convert <n> as a binary number ascii string
 *	@param buf must be longer than 33 byte
 */
LINK_BASE const char *BU_ToBinStr(unsigned int n, char *buf);

#endif
