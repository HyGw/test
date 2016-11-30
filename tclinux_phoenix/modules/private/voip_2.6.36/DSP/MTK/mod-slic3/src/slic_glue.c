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
 *  @file			ddr_slic.c
 *  @author		Yi-Lun Chen
 *  @brief			To define the glue interface used by SLIC driver
 *
 *  @date			2008.10.15
 */


/*====================================================================
 *
 *      Library Inclusion Segment
 *
 *====================================================================
 */
#include <stdarg.h>

#ifdef __KERNEL__
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#else
#include <base/utest.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
//#include <gpio/ddr_gpio.h>

#include <bsp/config.h>
#endif
#include "slic_glue.h"

/*====================================================================
 *
 *	Global Variable Segment
 *
 *====================================================================
 */
/* Refer from CIRQ spec */
#ifdef __KERNEL__
#define CFG_BASE_CIRQ		(0xf8060000)
#endif
#define CIRQ_L1IMKR			((volatile unsigned int*)(CFG_BASE_CIRQ+0x0004))
#define CIRQ_FIQCONR		((volatile unsigned int*)(CFG_BASE_CIRQ+0x001C))
#define CIRQ_IRQ_MASKALL	(0xFFFFFFFF)
#define CIRQ_FIQ_MASK		(0x1)
static unsigned int slic_irq;
/*====================================================================
 *
 *	External Variable Segment
 *
 *====================================================================
 */
/*====================================================================
 *
 *	Internal Function Segment
 *
 *====================================================================
 */
 /* Mutex-protection */
 static unsigned int _mutexCnt = 0;
 static unsigned int _slicMask = 0;
/*====================================================================
 *
 *	Constant and Macro Definition Segment
 *
 *====================================================================
 */
#ifndef NULL
#define NULL		((void*)0)
#endif
#define MAX(x, y)				(((x)>(y))?(x):(y))
#define MIN(x, y)				(((x)>(y))?(y):(x))
/*====================================================================
 *
 *	Function implementation
 *
 *====================================================================
 */
 /*
  *	@brief	Initiation of SLIC driver glue-submodule
  *	@return	0: Success !=0: Failed
  *
  *	To initiate GLUE sub-module of SLIC driver.
  */
 int slicDrv_glueInit(unsigned int irq)
{
	slic_irq=irq;
	return slicDrv_glueReinit();
}

 /*
  *	@brief	Reinitiation of SLIC driver glue-submodule
  *	@return	0: Success !=0: Failed
  *
  *	To re-initiate GLUE sub-module of SLIC driver.
  */
 int slicDrv_glueReinit(void)
{
	_mutexCnt = 0;
	_slicMask = 0;
	return 0;
}

 /*
  *	@brief	Do mutex lock in SLIC drvier
  *
  *	This procedure is used to do mutex-lock of SLIC driver operation in order to prevent from access chaos.
  *	Note that this function doesn't handle the situation that "SLIC interrupt is FIQ".
  */
void slicDrv_mutexLock(void)
{
	volatile unsigned int orgIRQMask;
	volatile unsigned int orgFIQMask;

	/* Mask interrupt to make sure the atomic of "mutexCnt" increasing operation */
	orgIRQMask = *CIRQ_L1IMKR;
	orgFIQMask = *CIRQ_FIQCONR;
	*CIRQ_L1IMKR = CIRQ_IRQ_MASKALL;
	*CIRQ_FIQCONR = CIRQ_FIQ_MASK;
	/* Read it back to make sure previous update has done */
	*CIRQ_L1IMKR;
	*CIRQ_FIQCONR;

	if (_mutexCnt == 0)
	{	/* To record original SLIC IRQ status */
		_slicMask = orgIRQMask & (1 << slic_irq);
	}
	_mutexCnt ++;

	/* Always mask SLIC interrupt after return from mutexCnt increasing operation */
	orgIRQMask |= (1 << slic_irq);

	/* Restore original FIQ/IRQ configurations */
	*CIRQ_FIQCONR = orgFIQMask;
	*CIRQ_L1IMKR = orgIRQMask;

}
 
 /*
  *	@brief	Do mutex lock in SLIC drvier
  *
  *	This procedure is used to do mutex-unlock of SLIC driver operation.
  */
void slicDrv_mutexUnlock(void)
{
	volatile unsigned int orgIRQMask;
	volatile unsigned int orgFIQMask;

	/* Mask interrupt to make sure the atomic of "mutexCnt" increasing operation */
	orgIRQMask = *CIRQ_L1IMKR;
	orgFIQMask = *CIRQ_FIQCONR;
	*CIRQ_L1IMKR = CIRQ_IRQ_MASKALL;
	*CIRQ_FIQCONR = CIRQ_FIQ_MASK;
	/* Read it back to make sure previous update has done */
	*CIRQ_L1IMKR;
	*CIRQ_FIQCONR;

	if (_mutexCnt)
	{
		if ((-- _mutexCnt) == 0)
		{
			/* Restore original SLIC interrupt : Just update orgIRQMask to achieve it */
			if (_slicMask == 0)
			{
				orgIRQMask &= ~(1 << slic_irq);
			}
		}
	}

	/* Restore original FIQ/IRQ configurations */
	*CIRQ_FIQCONR = orgFIQMask;
	*CIRQ_L1IMKR = orgIRQMask;
}

#if defined (_UNIT_) || defined (_EXE_)
int main()
{    
    return  0;
}
#endif

