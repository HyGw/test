/*
 * arch/mips/kernel/head.S
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1994, 1995 Waldorf Electronics
 * Written by Ralf Baechle and Andreas Busse
 * Copyright (C) 1995 - 1999 Ralf Baechle
 * Copyright (C) 1996 Paul M. Antoine
 * Modified for DECStation and hence R3000 support by Paul M. Antoine
 * Further modifications by David S. Miller and Harald Koerfgen
 * Copyright (C) 1999 Silicon Graphics, Inc.
 *
 * Head.S contains the MIPS exception handler and startup code.
 *
 **************************************************************************
 *  9 Nov, 2000.
 *  Added Cache Error exception handler and SBDDP EJTAG debug exception.
 *
 *  Kevin Kissell, kevink@mips.com and Carsten Langgaard, carstenl@mips.com
 *  Copyright (C) 2000 MIPS Technologies, Inc.  All rights reserved.
 **************************************************************************
 */
#include <linux/config.h>
#include <linux/threads.h>
#include <linux/init.h>

#include <asm/asm.h>
#include <asm/cacheops.h>
#include <asm/current.h>
#include <asm/offset.h>
#include <asm/processor.h>
#include <asm/regdef.h>
#include <asm/cachectl.h>
#include <asm/mipsregs.h>
#include <asm/stackframe.h>  
#include <asm/bootinfo.h>

#if defined(DDR_CALI_NAND)
#define	CR_ARB_BASE	0xBFB00000
#define HW_CONFIG		0x8c
#define HW_CONFIG2	0xf8
#define	SCREG_WR0	0x280
#endif

__INIT
	.text
	/*
	 * Reserved space for exception handlers.
	 * Necessary for machines which link their kernels at KSEG0.
	 * FIXME: Use the initcode feature to get rid of unused handler
	 * variants.
	 */
	.fill	0x280




	NESTED(start, 16, sp)
	.set	noreorder
	/* The following two symbols are used for kernel profiling. */
	EXPORT(stext)
	EXPORT(_stext)


	/*
	 * Stack for kernel and init, current variable
	 */

	/*mfc0 k0, $6
	nop
	nop*/
	la	k0, head_start
 	jr	k0
	nop

	#if defined(DDR_CALI_NAND) && defined(LZMA_IMG)
	nop		//reserve for store calibration bin start address
	nop		//reserve for store calibration bin end address
	nop
	nop	
	#endif
	
head_start:
	#if defined(DDR_CALI_NAND) && defined(SPRAM_IMG)
	la	t0, CR_ARB_BASE
	lw	t1, HW_CONFIG(t0)
	nop

	/* check HWCONF BIT [0]=0x0 to define NAND/SPI boot */
	/* do DDR freq change here,which is only for NAND boot mode */
	andi	t1, t1, 0x1
	beqz	t1, no_change_freq
	nop
	//Save return pc for calibration bin
	move	k1, ra
	la	k0, CR_ARB_BASE		
	sw	k1, SCREG_WR0(k0)
	nop

	//Change freq to 800M or 1G
	jal 	sys_mtk_dmc_change_freq
	nop

no_change_freq:
	#endif
	#if defined(DDR_CALI_NAND) && defined(LZMA_IMG)
	/*jump to ddr calibration bin from lzma.img */
	/*run ddr calibration in IMEM/DMEM		*/
#define	DSPRAM_PA	0x00017000
#define ISPRAM_PA	0x00018000
#define	DSPRAM_VA	0x80017000
#define ISPRAM_VA	0x80018000
#define ISPRAM_SIZE 	0x8000	//32k
#define DSPRAM_SIZE	0x1000 	/*DMEM is 4KB*/

#define SPRAM_MAGIC_NUM 	0x32514356
#define SPRAM_START_OFFSET	0x80000290
	//TODO: hwtrap check
	la	t0, CR_ARB_BASE
	lw	t1, HW_CONFIG(t0)
	nop

	/* check HWCONF BIT [0]=0x0 to define NAND/SPI boot */
	/* do DDR calibration here only for NAND boot mode */
	andi	t1, t1, 0x1
	beqz	t1, ddr_calibration_exit
	nop
	la	t0, CR_ARB_BASE
	lw	t1, HW_CONFIG2(t0)
	nop

	la	t0, SPRAM_START_OFFSET
	lw	t1, 0(t0)		//store spram start address to t1
	nop

	//Check spram.bin magic number
	add	t0, t1, 0x80000000
	lw	t2, 0(t0)
	nop

	bne	t2, SPRAM_MAGIC_NUM, ddr_calibration_exit
	nop

	//Enable DMEM
	li	a0, DSPRAM_PA
	li	a1, 1
	jal	probe_spram
	nop

	//Enable IMEM
	li	a0, ISPRAM_PA
	li	a1, 3
	jal	probe_spram
	nop

	//Copy instruction to IMEM
	la	t0, SPRAM_START_OFFSET
	lw	t2, 0(t0)
	nop
	add	t2, t2, 0x80000000		//spram start address to t2
	lw	t1, 4(t2)		//Get data section length
	nop

	//la	k0, __spram_start
	add	k0, t2, t1		//Skip data section length
	add 	k0, k0, 0x10		//Skip spram.bin header

	
	lw	a0, 8(t2)		//Get code section length
	nop
	
	subu	a1, k0, 0x80000000	//start address of instruction
	li	a2, 0
	jal	ispram_fill
	nop

	//Init DMEM
	la	k1, DSPRAM_VA	//Dst address
	li	t1, 0
1:
	sw	zero, 0(k1)	//Init as zero
	nop
	add	k1, k1, 4
	add	t1, t1, 4
	blt	t1, DSPRAM_SIZE, 1b	
	nop
	
	//Copy data to DMEM
	la	t0, SPRAM_START_OFFSET
	lw	t3, 0(t0)
	nop
	add	t3, t3, 0x80000000		//spram start address to t2
	
	lw	t2, 4(t3)		//Get data section length
	nop

	add	k0, t3, 0x10	//Source address, skip spram.bin header
	la	k1, DSPRAM_VA	//Dst address
	li	t1, 0
1:
	lw	t0, 0(k0)	//Read
	nop
	sw	t0, 0(k1)	//Write
	nop
	add	k1, k1, 4
	add	k0, k0, 4
	add	t1, t1, 4
	blt	t1, t2, 1b	
	nop

	#if 0
	//Save return pc
	move	k1, ra
	add	k1, k1, 0x20	//store ddr_calibration_done address
	la	k0, CR_ARB_BASE		
	sw	k1, SCREG_WR0(k0)
	nop
	#endif
	
	la	k0, ISPRAM_VA
	jal	k0
	nop

ddr_calibration_done:
	//Clear return pc register
	la	k1, 0
	la	k0, CR_ARB_BASE		
	sw	k1, SCREG_WR0(k0)

	//disable ispram
	li	a0, ISPRAM_PA
	li	a1, 2
	jal	probe_spram
	nop

	//diable dspram
	li	a0, DSPRAM_PA
	li	a1, 0
	jal	probe_spram
	nop
ddr_calibration_exit:
	#endif
	la	$28, init_task_union
	addiu	t0, $28, KERNEL_STACK_SIZE-32
	//sw	t0, kernelsp
	subu	sp, t0, 4*SZREG

	/* The firmware/bootloader passes argc/argp/envp
	 * to us as arguments.  But clear bss first because
	 * the romvec and other important info is stored there
	 * by prom_init().
	 */
	la	t0, _fbss
	sw	zero, (t0)
	la	t1, (_end - 4)
	la	k0, _end
1:
	addiu	t0, 4
	sw	zero, (t0)
	bne	t0, t1, 1b
	nop

	jal	init_arch	
	nop
	END(start)


	/* General exception vector. */
		/* TLB refill, EXL == 0, R[23]00 version */

	.set	at
	.set	reorder	   
/* frankliao added 20100803 */	
#if	(!defined LZMA_IMG) && (!defined SPRAM_IMG)
 /* General exception vector.*/

	/* General exception vector. */
	NESTED(except_vec3_generic, 0, sp)
	.set	noat
	.set	mips0
	mfc0	k1, CP0_CAUSE
	la	k0, exception_handlers
	andi	k1, k1, 0x7c
	addu	k0, k0, k1
	lw	k0, (k0)
	nop
	jr	k0
	 nop
	END(except_vec3_generic)
	.set	at
#endif

#if defined(DDR_CALI_NAND) && defined(SPRAM_IMG)
/************************************************************************
 *
 *                          sys_mtk_dmc_change_freq
 *  Description :
 *  -------------
 *  Perform MTK DMC change frequency procedure
 *
 *  Parameters :
 *  ------------
 *
 *  Return values :
 *  ---------------
 *  0 If OK, error code != 1 if error
  ************************************************************************/
#define	CR_DMC_BASE			0xBFB20000

#define DLY_CLK_CYCLE(cyc) \
		li	t0, cyc; \
		srl	t0, t0, 1; \
999:	subu	t0, t0, 1; \
		bgtz	t0, 999b; \
		nop
  
#ifndef MT75XX_FPGA
#define DDR2	0
#define DDR3	1
#else
#define SDRAM	0
#define DDR3	1
#define DDR1	2
#define DDR2	3
#endif
#define DRAM_SIZE_2MB 0
#define DRAM_SIZE_8MB 1
#define DRAM_SIZE_16MB 2
#define DRAM_SIZE_32MB 3
#define DRAM_SIZE_64MB 4
#define DRAM_SIZE_128MB 5
#define DRAM_SIZE_256MB 6
#define DRAM_SIZE_512MB 7
#define XTAL_INPUT_FREQ_25MHZ	0
#define XTAL_INPUT_FREQ_20MHZ	1
#define XTAL_INPUT_FREQ_40MHZ	2
#define DRAMC_WAIT_CMD_RESPONSE_MAX_CNT 50000
#define REG_RW_DELAY	10 // delay for register read/write
#define CLK_CYCLE_NS	s7 // store the time per clock cycle in s7, unit: nano second
#define DLY_NS s6 // store the nano second to be delayed
#define DLY_RA	s5
#define XTAL_SRC_SEL	s4 // Xtal input frequency select

#define MEMPLL1_CONFIG_FOR_SSC
  
LEAF(sys_mtk_dmc_change_freq)
		move DLY_RA, ra
		
MTK_DMC_CHANGE_FREQ_START:
		// judge the platform, DRAM type & size
		la		k0, CR_ARB_BASE
		lw		t0, 0x8c(k0)
		nop
		srl		s0, t0, 31 // FPGA indication

		// judge FPGA or ASIC platform
		bnez	s0, 1f // FPGA
		nop
		//ASIC
		srl		s1, t0, 18
		andi	s1, s1, 0x1	// DRAM type
		j 88f
		nop	
1:  //FPGA
		srl		s1, t0, 24
		andi	s1, s1, 0x3	// DRAM type
88:
		srl		s2, t0, 13
		andi	s2, s2, 0x7	// DRAM size
		
		srl		XTAL_SRC_SEL, t0, 22
		andi	XTAL_SRC_SEL, XTAL_SRC_SEL, 0x3	// Xtal Freq.

		li		CLK_CYCLE_NS, 1 // store the time per clock cycle in s7, unit: nano second, for ASIC 750MHz clock

		// FPGA platform
		bnez	s0, mtk_dmc_change_freq_done // FPGA doesn't need to change frequency
		nop
		
		/* *** disable DRAM controller *** */
		la		k0,	CR_ARB_BASE
		lw		t0, 0x74(k0)
		nop
		li		t1, 0xfffffffb
		and		t0, t0, t1 // 0x74[2]: DRAMC enable
		sw		t0, 0x74(k0)
		nop
		DLY_CLK_CYCLE(150)
				
				
		la		k0, CR_DMC_BASE // k0 stores DRAMC base address
		nop
		
		lw		t0, 0x1c0(k0)
		nop
		li		t1, 0x7fffffff
		and		t0,	t0, t1 // disable HW gw calibration
		sw		t0, 0x1c0(k0)
		nop
		
		lw		t0, 0x1ec(k0)
		nop
		li		t1, 0xfffff8ef
		and		t0,	t0, t1 // disable EMI/arbiter setting
		sw		t0, 0x1ec(k0)
		nop
		
		lw		t0, 0x4(k0)
		nop
		li		t1, 0xffff7fff
		and		t0,	t0, t1 // disable power down; CKE keep high
		sw		t0, 0x4(k0)
		nop
		
		/* *** start changing frequency *** */

		lw		t0, 0xe4(k0)
		nop
		li		t1, 0xfffffffb
		and		t0,	t0, t1 // disable CKEFIXON
		sw		t0, 0xe4(k0)
		nop
		
		lw		t0, 0x4(k0)
		nop
		li		t1, 0x04000000
		or		t0,	t0, t1 // enter self-refresh
		sw		t0, 0x4(k0)
		nop
		
		li		t1, DRAMC_WAIT_CMD_RESPONSE_MAX_CNT
888:
		subu	t1, t1, 0x1
		beqz	t1, MTK_DMC_CHANGE_FREQ_START // timeout
		nop
		lw		t0, 0x3b8(k0)
		nop
		srl		t0, t0, 16 // SREF_STATE
		andi	t0, t0, 0x1 // wait for entering self refresh
		beqz	t0, 888b
		nop
		
		
		#ifdef MEMPLL1_CONFIG_FOR_SSC
		lw		t0, 0x60c(k0)
		li		t1, 0x00000001
		or		t0,	t0, t1
		sw		t0, 0x60c(k0)
		
		lw		t0, 0x600(k0)
		li		t1, 0xfffffffc
		and		t0,	t0, t1
		sw		t0, 0x600(k0)

		li		t0, 0x1e000000
		sw		t0, 0x608(k0)
		nop
		
		lw		t0, 0x60c(k0)
		li		t1, 0xffffefff
		and		t0,	t0, t1
		sw		t0, 0x60c(k0)
		
		lw		t0, 0x60c(k0)
		li		t1, 0x00001000
		or		t0,	t0, t1
		sw		t0, 0x60c(k0)

		lw		t0, 0x60c(k0)
		li		t1, 0xffffffef
		and		t0,	t0, t1
		sw		t0, 0x60c(k0)
		
		lw		t0, 0x600(k0)
		li		t1, 0xfffffff3
		and		t0,	t0, t1
		sw		t0, 0x600(k0)

		lw		t0, 0x600(k0)
		li		t1, 0x08000000
		or		t0,	t0, t1
		sw		t0, 0x600(k0)

		lw		t0, 0x61c(k0)
		li		t1, 0xffffffe0
		and		t0,	t0, t1
		li		t1, 0x0000000f
		or		t0, t0, t1
		sw		t0, 0x61c(k0)

		lw		t0, 0x618(k0)
		li		t1, 0x3fffffff
		and		t0,	t0, t1
		sw		t0, 0x618(k0)

		lw		t0, 0x604(k0)
		li		t1, 0xfffffff1
		and		t0,	t0, t1
		sw		t0, 0x604(k0)

		lw		t0, 0x604(k0)
		li		t1, 0xfffffffe
		and		t0,	t0, t1
		li		t1, 0x00000001
		or		t0, t0, t1
		sw		t0, 0x604(k0)

		lw		t0, 0x60c(k0)
		li		t1, 0xff7fffff
		and		t0,	t0, t1
		li		t1, 0x00800000
		or		t0, t0, t1
		sw		t0, 0x60c(k0)

		lw		t0, 0x610(k0)
		li		t1, 0xffff0000
		and		t0,	t0, t1
		li		t1, 0x000000fa
		or		t0, t0, t1
		sw		t0, 0x610(k0)

		lw		t0, 0x614(k0)
		li		t1, 0xfffffff0
		and		t0,	t0, t1
		sw		t0, 0x614(k0)

		lw		t0, 0x610(k0)
		li		t1, 0x000fffff
		and		t0,	t0, t1
		li		t1, 0x4e800000  //t1, 0x13a00000:1%
		or		t0, t0, t1
		sw		t0, 0x610(k0)

		lw		t0, 0x614(k0)
		li		t1, 0xff0000ff
		and		t0,	t0, t1
		li		t1, 0x0004e800  //t1, 0x13a00000:1%
		or		t0, t0, t1
		sw		t0, 0x614(k0)
		#endif //end of MEMPLL1_CONFIG_FOR_SSC
		
		li		t0, 0x02210a00 // reset RG_MEMPLL2_EN
		sw		t0, 0x620(k0)
		nop
		li		t0, 0x02210a00 // reset RG_MEMPLL3_EN
		sw		t0, 0x62c(k0)
		nop
		li		t0, 0x02210a00 // reset RG_MEMPLL4_EN
		sw		t0, 0x644(k0)
		nop
		
		la		DLY_NS,	1000  // wait 1us
		bal		dly_ns_mt75xx
		nop
		
		li		t0, 0x0000000f // reset RG_MEMPLL_DIV_EN
		sw		t0, 0x61c(k0)
		nop
		
		la		DLY_NS,	20000  // wait 20us
		bal		dly_ns_mt75xx
		nop

		#ifdef MEMPLL1_CONFIG_FOR_SSC
		li		t0, 0x3a213c00 // reset RG_MEMPLL_EN
		sw		t0, 0x600(k0)
		nop
		
		la		DLY_NS,	1000000  // wait 1ms
		bal		dly_ns_mt75xx
		nop
		
		li		t0, 0x1a003c00 // reset RG_MEMPLL_BIAS_LPF_EN
		sw		t0, 0x600(k0)
		nop
		
		la		DLY_NS,	2000  // wait 2us
		bal		dly_ns_mt75xx
		nop
		
		li		t0, 0x0a003c00 // set RG_MEMPLL_BIAS_EN
		sw		t0, 0x600(k0)
		nop

		la		DLY_NS,	2000  // wait 2us
		bal		dly_ns_mt75xx
		nop
				
		li		t0, 0x1a003c00 // set RG_MEMPLL_BIAS_EN
		sw		t0, 0x600(k0)
		nop
		
		la		DLY_NS,	2000  // wait 2us
		bal		dly_ns_mt75xx
		nop
		
		li		t0, 0x3a003c00 // set RG_MEMPLL_BIAS_LPF_EN
		sw		t0, 0x600(k0)
		nop

		la		DLY_NS,	1000000  // wait 1ms
		bal		dly_ns_mt75xx
		nop

		li		t0, 0x3a213c10 // set RG_MEMPLL_EN
		sw		t0, 0x600(k0)
		nop
		
		#else
		
		li		t0, 0x32213c00 // reset RG_MEMPLL_EN
		sw		t0, 0x600(k0)
		nop
		
		la		DLY_NS,	1000000  // wait 1ms
		bal		dly_ns_mt75xx
		nop
		
		li		t0, 0x12003c00 // reset RG_MEMPLL_BIAS_LPF_EN
		sw		t0, 0x600(k0)
		nop
		
		la		DLY_NS,	2000  // wait 2us
		bal		dly_ns_mt75xx
		nop
		
		li		t0, 0x02003c00 // set RG_MEMPLL_BIAS_EN
		sw		t0, 0x600(k0)
		nop
		
		la		DLY_NS,	2000  // wait 2us
		bal		dly_ns_mt75xx
		nop
				
		li		t0, 0x12003c00 // set RG_MEMPLL_BIAS_EN
		sw		t0, 0x600(k0)
		nop		
		
		la		DLY_NS,	2000  // wait 2us
		bal		dly_ns_mt75xx
		nop
		
		li		t0, 0x32003c00 // set RG_MEMPLL_BIAS_LPF_EN
		sw		t0, 0x600(k0)
		nop

		la		DLY_NS,	1000000  // wait 1ms
		bal		dly_ns_mt75xx
		nop

		li		t0, 0x32213c10 // set RG_MEMPLL_EN
		sw		t0, 0x600(k0)
		nop
		#endif //end of MEMPLL1_CONFIG_FOR_SSC

	
		la		DLY_NS,	20000  // wait 20us
		bal		dly_ns_mt75xx
		nop

		li		t0, 0x0000002f // set RG_MEMPLL_DIV_EN
		sw		t0, 0x61c(k0)
		nop
		
		la		DLY_NS,	1000  // wait 1us
		bal		dly_ns_mt75xx
		nop
	
		// judge Xtal input frequence
		li		t0, XTAL_INPUT_FREQ_25MHZ
		beq		XTAL_SRC_SEL, t0, 25f	// Xtal input frequency 25 MHz
		nop

		// Xtal input frequency other than 25 MHz
		// DDR-800
		li		t0, 0x02211400 // set RG_MEMPLL2_FBKDIV
		sw		t0, 0x620(k0)
		nop
		li		t0, 0x02211400 // set RG_MEMPLL3_FBKDIV
		sw		t0, 0x62c(k0)
		nop
		li		t0, 0x02211400 // set RG_MEMPLL4_FBKDIV
		sw		t0, 0x644(k0)
		nop
				
		li		t0, 0x00020000 // set RG_MEMPLL2_M4PDIV
		sw		t0, 0x624(k0)
		nop
		li		t0, 0x02211410 // set RG_MEMPLL2_EN
		sw		t0, 0x620(k0)
		nop
		li		t0, 0x00020000 // set RG_MEMPLL3_M4PDIV
		sw		t0, 0x630(k0)
		nop
		li		t0, 0x02211410 // set RG_MEMPLL3_EN
		sw		t0, 0x62c(k0)
		nop
		li		t0, 0x00020000 // set RG_MEMPLL4_M4PDIV
		sw		t0, 0x648(k0)
		nop
		li		t0, 0x02211410 // set RG_MEMPLL4_EN
		sw		t0, 0x644(k0)
		nop
	
		j 333f
		nop
25:
		// Xtal input frequency 25 MHz
		// DDR-800
		li		t0, 0x02210800 // set RG_MEMPLL2_FBKDIV
		sw		t0, 0x620(k0)
		nop
		li		t0, 0x02210800 // set RG_MEMPLL3_FBKDIV
		sw		t0, 0x62c(k0)
		nop
		li		t0, 0x02210800 // set RG_MEMPLL4_FBKDIV
		sw		t0, 0x644(k0)
		nop
		
		li		t0, 0x00020000 // set RG_MEMPLL2_M4PDIV
		sw		t0, 0x624(k0)
		nop
		li		t0, 0x02210810 // set RG_MEMPLL2_EN
		sw		t0, 0x620(k0)
		nop
		li		t0, 0x00020000 // set RG_MEMPLL3_M4PDIV
		sw		t0, 0x630(k0)
		nop
		li		t0, 0x02210810 // set RG_MEMPLL3_EN
		sw		t0, 0x62c(k0)
		nop
		li		t0, 0x00020000 // set RG_MEMPLL4_M4PDIV
		sw		t0, 0x648(k0)
		nop
		li		t0, 0x02210810 // set RG_MEMPLL4_EN
		sw		t0, 0x644(k0)
		nop

333:
	
		la		DLY_NS, 20000  // wait 20us
		bal		dly_ns_mt75xx
		nop

		lw		t0, 0x4(k0)
		nop
		li		t1, 0xfbffffff
		and		t0,	t0, t1 // eixt self-refresh
		sw		t0, 0x4(k0)
		nop

		li		t1, DRAMC_WAIT_CMD_RESPONSE_MAX_CNT
888:
		subu	t1, t1, 0x1
		beqz	t1, MTK_DMC_CHANGE_FREQ_START // timeout
		nop
		lw		t0, 0x3b8(k0)
		nop
		srl		t0, t0, 16 // SREF_STATE
		andi	t0, t0, 0x1
		bnez	t0, 888b // wait for exiting self refresh
		nop
		
		lw		t0, 0xe4(k0)
		nop
		li		t1, 0x00000004
		or		t0,	t0, t1 // enable CKEFIXON
		sw		t0, 0xe4(k0)
		nop
		

		// judge DDR2 or DDR3
		li		t0, DDR3
		beq		s1, t0, 3f	// DDR3 type
		nop
		
		// DDR2
		li		t0, 0x000f5f2f // set REFCNT for DDR2-800
		sw		t0, 0x8(k0)
		nop
		
		//for DDR2-800
		lw		t0, 0xe0(k0)
		li		t1, 0xf8ffffff
		and		t0,	t0, t1
		li		t1, 0x02000000
		or		t0, t0, t1
		sw		t0, 0xe0(k0)
		
		lw		t0, 0x124(k0)
		li		t1, 0xffffffcc
		and		t0,	t0, t1
		li		t1, 0x00000000
		or		t0, t0, t1
		sw		t0, 0x124(k0)
		
		li		t0, 0x30303030
		sw		t0, 0x94(k0)
		
		li		t0, 0x00001919
		sw		t0, 0x18(k0)
		li		t0, 0x05050505
		sw		t0, 0x210(k0)
		li		t0, 0x00000000
		sw		t0, 0x214(k0)
		li		t0, 0x06060606
		sw		t0, 0x218(k0)
		li		t0, 0x03030303
		sw		t0, 0x21c(k0)
		
		lw		t0, 0x7c(k0)
		li		t1, 0xffffff8f
		and		t0,	t0, t1
		li		t1, 0x00000040
		or		t0, t0, t1
		sw		t0, 0x7c(k0)
		
		lw		t0, 0xe4(k0)
		li		t1, 0xffffffef
		and		t0,	t0, t1
		sw		t0, 0xe4(k0)
		
		li		t0, 0x00000011
		sw		t0, 0x10(k0)
		li		t0, 0x11113333
		sw		t0, 0x200(k0)
		li		t0, 0x11112222
		sw		t0, 0x204(k0)
		li		t0, 0x00000023
		sw		t0, 0x14(k0)
		
		lw		t0, 0x7c(k0)
		li		t1, 0xffff0fff
		and		t0,	t0, t1
		li		t1, 0x00001000
		or		t0, t0, t1
		sw		t0, 0x7c(k0)
		
		j			99f
		nop
3:
		lw		t0, 0x7c(k0)
		nop
		li		t1, 0xfffffff7
		and		t0,	t0, t1 // disable ODT when MR setting
		sw		t0, 0x7c(k0)
		nop
		
		li		t0, 0x00002004 // set MR1, enable DLL, RTT = RZQ/4 for DDR3
		sw		t0, 0x88(k0)
		nop
		li		t0, 0x00000001 // enable MRW
		sw		t0, 0x1e4(k0)
		nop
		
		li		t1, DRAMC_WAIT_CMD_RESPONSE_MAX_CNT
888:
		subu	t1, t1, 0x1
		beqz	t1, MTK_DMC_CHANGE_FREQ_START // timeout
		nop
		lw		t0, 0x3b8(k0)
		nop
		andi	t0, t0, 0x1 // wait for MRW command response, bit[0]
		beqz	t0, 888b
		nop
		
		DLY_CLK_CYCLE(2) // tMRD (4nCK by JEDEC spec, clock DRAMC:DRAM=1:2)

		li		t0, 0x00000000 // disable MRW
		sw		t0, 0x1e4(k0)
		nop
		
		li		t0, 0x00004200 // set MR2, RTT=ZQ/4, CWL=5 for DDR3 DDR3-800
		sw		t0, 0x88(k0)
		nop
		li		t0, 0x00000001 // enable MRW
		sw		t0, 0x1e4(k0)
		nop
		
		li		t1, DRAMC_WAIT_CMD_RESPONSE_MAX_CNT
888:
		subu	t1, t1, 0x1
		beqz	t1, MTK_DMC_CHANGE_FREQ_START // timeout
		nop
		lw		t0, 0x3b8(k0)
		nop
		andi	t0, t0, 0x1 // wait for MRW command response, bit[0]
		beqz	t0, 888b
		nop
		
		DLY_CLK_CYCLE(6) // tMOD (12nCK by JEDEC spec, clock DRAMC:DRAM=1:2)

		li		t0, 0x00000000 // disable MRW
		sw		t0, 0x1e4(k0)
		nop

		lw		t0, 0x1e0(k0)
		nop
		li		t1, 0x04000000
		or		t0,	t0, t1 // enable FASTOE for CWL < 6
		sw		t0, 0x1e0(k0)
		nop
		
		lw		t0, 0x7c(k0)
		nop
		li		t1, 0x8fffffff
		and		t0,	t0, t1 // clear WLAT setting
		li		t1, 0x20000008 // set WLAT = 2 for CWL=5 and enable ODT when MR setting finished
		or		t0, t0, t1
		sw		t0, 0x7c(k0)
		nop

		li		t0, 0x000f5f2f // set REFCNT for DDR3-800
		sw		t0, 0x8(k0)
		nop
		
		//for DDR3-800
		lw		t0, 0xe0(k0)
		li		t1, 0xf8ffffff
		and		t0,	t0, t1
		li		t1, 0x02000000
		or		t0, t0, t1
		sw		t0, 0xe0(k0)
		
		lw		t0, 0x124(k0)
		li		t1, 0xffffffcc
		and		t0,	t0, t1
		li		t1, 0x00000022
		or		t0, t0, t1
		sw		t0, 0x124(k0)
		
		li		t0, 0x30303030
		sw		t0, 0x94(k0)
		
		li		t0, 0x00001919
		sw		t0, 0x18(k0)
		li		t0, 0x06060606
		sw		t0, 0x210(k0)
		li		t0, 0x04040404
		sw		t0, 0x214(k0)
		li		t0, 0x09090909
		sw		t0, 0x218(k0)
		li		t0, 0x07070707
		sw		t0, 0x21c(k0)
		
		lw		t0, 0x7c(k0)
		li		t1, 0xffffff8f
		and		t0,	t0, t1
		li		t1, 0x00000050
		or		t0, t0, t1
		sw		t0, 0x7c(k0)
		
		lw		t0, 0xe4(k0)
		li		t1, 0xffffffef
		and		t0,	t0, t1
		sw		t0, 0xe4(k0)
		
		li		t0, 0x00000012
		sw		t0, 0x10(k0)
		li		t0, 0x11113333
		sw		t0, 0x200(k0)
		li		t0, 0x11112222
		sw		t0, 0x204(k0)
		li		t0, 0x00000023
		sw		t0, 0x14(k0)
		
99:
		
		/* *** set DRAM page size and enable DRAM controller *** */
		la		k0,	CR_ARB_BASE
		lw		t0, 0x74(k0)
		nop
		li		t1, 0x00000004
		or		t0, t0, t1 // 0x74[2]: DRAMC enable
		sw		t0, 0x74(k0)
		nop
		
		DLY_CLK_CYCLE(150)		
		
mtk_dmc_change_freq_done:
		jr		DLY_RA
		nop
END(sys_mtk_dmc_change_freq)

/************************************************************************
 *
 *                          dly_ns_mt75xx
 *  Description :
 *  -------------
 *  Perform nano second delay for MT75xx 
 *
 *  Parameters :
 *  ------------
 *
 *  Return values :
 *  ---------------
 *  
 *
 ************************************************************************/
LEAF(dly_ns_mt75xx)
		//move	t0, DLY_NS
		
		#if 0	
		div	DLY_NS, CLK_CYCLE_NS
		mflo t0
		srl	t0, t0, 1
		#else
		srl	t0, DLY_NS, 2
		#endif
		
999:	subu	t0, t0, 1
		bgtz	t0, 999b
		nop

		jr		ra
		nop
END(dly_ns_mt75xx)
#endif
/*
 * This buffer is reserved for the use of the cache error handler.
 */

__FINIT
