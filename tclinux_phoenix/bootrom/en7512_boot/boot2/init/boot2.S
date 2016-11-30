        
#include <asm/asm.h>
#include <asm/regdef.h>
#include <asm/mipsregs.h>        

#define	CR_ARB_BASE			0xBFB00000
#define	CR_TIMER_BASE			0xBFBF0100
#define	GPIOBASE			0xBFBF0200
#define	CR_SMC_BASE			0xBFB10000
#define	CR_DMC_BASE			0xBFB20000
#define CR_CHIP_SCU_BASE	0xBFA20000  //YMC_20131127
#define CR_AFE_BASE			0xBF900000

#define SRAM_START_ADDR_NEW	0xBFA30040
#define SRAM_START_ADDR		0xBFA30000
//#define UNCACHE_MODE
//#define DEBUG 0

//CML_mt7510_start
// delay BUS clock
#define DLY_CLK_CYCLE(cyc) \
		li	t0, cyc; \
		srl	t0, t0, 1; \
999:	subu	t0, t0, 1; \
		bgtz	t0, 999b; \
		nop

// delay for ns
#define DLY_NS(ns, CLK_CYCLE_NS) \
		move	t1, CLK_CYCLE_NS; \
		li	t0, ns; \
		div	t0, t1; \
		mflo t0; \
		srl	t0, t0, 1; \
999:	subu	t0, t0, 1; \
		bgtz	t0, 999b; \
		nop
//CML_mt7510_end


#if 1

#define DISPLAYLED(d0,d1) 	nop

#else

#define DISPLAYLED(d0,d1) \
    la v0, GPIOBASE; \
    li t5, d0; \
	li t6, d1; \
	sw t5, 0x4(v0); \
2:  addi t6, -1; \
	bnez t6, 2b;\
	nop; \
	nop

#endif
					 
boot_start:		

#ifndef DDR_CALI_NAND
#ifdef DEBUG

		jal sys_uart_init
                nop
                li t7, 'x'
                jal disp_character
                nop
#endif




#ifndef BOOTROM_IN_SRAM
#ifdef MT7510_FPGA_TEST
		jal sys_init_mtk_dmc
		nop
#endif

		/*  Initialise caches. If compiled for simulation, we
	 	 *  skip this since caches can be assumed to be invalidated.
	 	 */
		jal     sys_init_cache
		nop
#endif

#define	SCREG_WR0		0x280
#define SET_MIPS3() .##set mips3
#define SET_MIPS0() .##set mips0
#define CACHE_OP( code, type )			( ((code) << 2) | (type) )
#define DCACHE_ADDR_HIT_WRITEBACK		CACHE_OP(0x6, 1)
#define ICACHE_ADDR_HIT_INVALIDATE		CACHE_OP(0x4, 0)


#define	SHARE_FEMEM_SEL	0xBFB00958
#define	SHARE_FEMEM_PA	0xBFA32800
#define	SHARE_FEMEM_VA	0x9FA32800

		//Enable pbus access FE memory
		la      v0, SHARE_FEMEM_SEL
		lw      t6, 0x0(v0)
		nop

		ori	t6, t6, 0x1
		sw	t6, 0x0(v0)
		nop


#ifdef DDR_CALI
#ifdef DEBUG
                li t7, 'd'
                jal disp_character
                nop

#endif

		/*run ddr calibration in FEMEM, size is 48KB
		*/

                /* Store address information of spram.img */
		la  t1, SRAM_START_ADDR
		lw  s1, 0x8(t1)
		nop
		lw  s2, 0xc(t1)
		nop
		la  s3, 0xbfa32800
		
		la  s4, move_ddr_calibration


		la t1, 0x9FA30040
		nop
		
		jr	t1
		nop

LEAF(move_ddr_calibration)
		nop
END(move_ddr_calibration)
		
		
#if 0
		/* cache writeback and invalidate address */
		li		t1, 0x9fa30000
		
		li		t2, SHARE_FEMEM_VA
		sw		t2, 0x30(t1)
		nop
		li		t2, 0x9fa3c000
		sw		t2, 0x34(t1)		
		nop
#endif		
		
 	 		
		//Save return pc
		la	k1, ddr_calibration_done
		la	k0, CR_ARB_BASE
		sw	k1, SCREG_WR0(k0) 		
 		
 		la	k0, 0x9fa32800 				 		
 		
		jr	k0
		nop


LEAF(ddr_calibration_done)
		//Clear return pc register
		la	k1, 0
		la	k0, CR_ARB_BASE		
		sw	k1, SCREG_WR0(k0)
END(ddr_calibration_done)
ddr_calibration_exit:
#endif

nop
nop

#ifndef TCSUPPORT_SLM_EN
/* Bypass SLM function */
		la  k0, 0xbfa60000
		lw  t7,	0x0(k0)
		nop
		
		ori t7, t7, 0x1
		sw  t7, 0x0(k0)
		nop
#endif
		/* configure arbiter */
	    la	v1,	CR_ARB_BASE  
		/* configure SMC */
		/* SRAM address: */
		/*	1. FLASH	 16bit begin at 0x1fc00000 to 0x1fffffff  size:	  4MB */
		/*	2. SRAM    	 8bit begin at 0x1fe00000 to 0x1fe3ffff	size: 256KB   */
		li	a0,	0x80071f1e
		li	a1,	0x00071f1f
		li	a2,	0x00040000

		sw	a0,	0x20(v1)
		sw	a1,	0x24(v1)
		
#ifdef DEBUG
                li t7, '5'
                jal disp_character
                nop

#endif	 		

		DISPLAYLED(0x52, 0x1000)

// SRAM rank 3,4,5
		li	a0,	0x00040808
		li	a1,	0xc0041010	// with bridge				
		li	a2,	0x80050000

		sw	a2,	0x34(v1)		

		DISPLAYLED(0x54, 0x1000)

    	li	v1,	CR_SMC_BASE

		li	a0, 0x102d1040    		

 		li	a2,	0x10aa8100				/* 16-bit hpi8  configure  */
 		li  a3,	0x200028d0				/* 32-bit dmt   configure  */

		sw	a0,	0x0(v1)      			/* CR_SMC_BANK0     */
//		sw	a1,	0x4(v1)      			/* CR_SMC_BANK1     */
// 		sw	a2,	0x10(v1)     			/* CR_SMC_BANK4     */
 		sw	a3,	0x14(v1)     			/* CR_SMC_BANK5     */

		DISPLAYLED(0x58, 0x1000)

#ifdef DEBUG
                li t7, '6'
                jal disp_character
                nop

#endif		

		/* stop interrupt */
		li		t0, 	0xbfb40000
		nop

		sw		zero, 	0x0(t0)			/* all interrupt belong to IRQ */
		sw		zero, 	0x04(t0)		/* set all interrupt mask to 0 .. disable all interrupt */

		DISPLAYLED(0x57, 0x20000)




#ifdef DEBUG
                li t7, 'e'
                jal disp_character
                nop

#endif
#ifdef BOOTROM_IN_SRAM

#define	SHARE_FEMEM_SEL	0xBFB00958
#define	SHARE_FEMEM_PA	0x1FA30000
#define	SHARE_FEMEM_VA	0x9FA32800
#define BOOTROM_START_ADDR	0x9FA30040
#define BOOTROM_END_ADDR	0x9FA3C000

		//Enable pbus access FE memory
		la      v0, SHARE_FEMEM_SEL
		lw      t6, 0x0(v0)
		nop

		ori	t6, t6, 0x1
		sw	t6, 0x0(v0)
		nop
#else
#define BOOTROM_START_ADDR	0x80000000
#define BOOTROM_END_ADDR	0x80020000
#endif

/* bootloader copression support */
#ifdef LZMA_IMG

                /* Store address information of boot.img */
		la  t1, SRAM_START_ADDR
		lw  t0, 0x18(t1)
		la t1, 0x80010000
		sw t0, 0x0(t1)

		la  t1, SRAM_START_ADDR
		lw  t0, 0x1c(t1)
		la t1, 0x80010020
		sw t0, 0x0(t1)

		la  t1, SRAM_START_ADDR
		lw  k0, 0x10(t1)
		lw  k1, 0x14(t1)
		la	t1, 0x80000000
#else
		la	t1, SRAM_START_ADDR
		lw  k0, 0x18(t1)
		lw  k1, 0x1c(t1)
		
		la  t1, BOOTROM_START_ADDR
#endif


#ifdef DEBUG
                li t7, 'g'
                jal disp_character
                nop
#endif                

		la 	t1, SRAM_START_ADDR
	
		lw  t2, 0x18(t1)
		nop
		li 	t3, 0x80010000
		sw	t2, 0x0(t3)
		nop			

		lw  t2, 0x1c(t1)
		nop
		li 	t3, 0x80010020
		sw	t2, 0x0(t3)				
		nop
		
		lw  s1, 0x10(t1)
		nop
		lw  s2, 0x14(t1)
		nop
		la	s3, 0x80000000
		la	s4,	loading_bootcode_done
		nop
		
		//Save return pc

 		la	k0, SRAM_START_ADDR_NEW
		jr	k0
		nop			
		


LEAF(loading_bootcode_done)
                nop
END(loading_bootcode_done)		
load_exit:

#ifdef DEBUG
        li t7, 'k'
        jal disp_character
        nop
#endif                

		li	t1, 0x80000000
		li	t2, 0x80200000

1:
SET_MIPS3()
		cache   DCACHE_ADDR_HIT_WRITEBACK, 0(t1)
		sync
		cache 	ICACHE_ADDR_HIT_INVALIDATE, 0(t1)
SET_MIPS0()

		addiu 	t1, t1, 32
		bne		t2, t1, 1b
		nop

		//DISPLAYLED(0x5e, 0x20000)

		li		t0, 0xf200ff15 			// Initialize the status register
		mtc0	t0, $12
		nop
		
		la t2,	0x80000000
		jr t2
		nop
/*********************boot2 End************************************************************/

1:
		//DISPLAYLED(0x5c, 0x1000)
		lw	t0, 0(k0)
		nop
		sw	t0, 0(t1)
		//DISPLAYLED(0x50, 0x1000)
		

		//CML_test
		nop
		lw	t5, 0(t1)
		nop
		bne	t0, t5, ram_error
		nop
		//CML_test
		
		
		addiu k0, k0, 4
		addiu t1, t1, 4
		bne	k1, k0, 1b
		nop
		nop
		nop



//CML_test
		b ram_pass
		nop
ram_error:
		b ram_error
		nop
ram_pass:
//CML_test


#define S_ConfigK0		0			/* Kseg0 coherency algorithm (R/W) */
#define M_ConfigK0		(0x7 << S_ConfigK0)

#define K_CacheAttrU		2			/* Uncached */

#define SET_MIPS3() .##set mips3
#define SET_MIPS0() .##set mips0
#define SET_PUSH()  .##set push
#define SET_POP()   .##set pop

/* cache operations */

#define CACHE_OP( code, type )			( ((code) << 2) | (type) )

#define ICACHE_INDEX_INVALIDATE			CACHE_OP(0x0, 0)
#define ICACHE_INDEX_LOAD_TAG			CACHE_OP(0x1, 0)
#define ICACHE_INDEX_STORE_TAG			CACHE_OP(0x2, 0)
#define DCACHE_INDEX_WRITEBACK_INVALIDATE	CACHE_OP(0x0, 1)
#define DCACHE_INDEX_LOAD_TAG			CACHE_OP(0x1, 1)
#define DCACHE_INDEX_STORE_TAG			CACHE_OP(0x2, 1)
#define SCACHE_INDEX_WRITEBACK_INVALIDATE	CACHE_OP(0x0, 3)
#define SCACHE_INDEX_STORE_TAG			CACHE_OP(0x2, 3)

#define ICACHE_ADDR_HIT_INVALIDATE		CACHE_OP(0x4, 0)
#define ICACHE_ADDR_FILL			CACHE_OP(0x5, 0)
#define ICACHE_ADDR_FETCH_LOCK			CACHE_OP(0x7, 0)
#define DCACHE_ADDR_HIT_INVALIDATE		CACHE_OP(0x4, 1)
#define DCACHE_ADDR_HIT_WRITEBACK_INVALIDATE	CACHE_OP(0x5, 1)
#define DCACHE_ADDR_HIT_WRITEBACK		CACHE_OP(0x6, 1)
#define DCACHE_ADDR_FETCH_LOCK			CACHE_OP(0x7, 1)

#define SCACHE_ADDR_HIT_WRITEBACK_INVALIDATE	CACHE_OP(0x5, 3)

#define ICACHE_INVALIDATE_WORKAROUND(reg) \
SET_PUSH();				  \
SET_MIPS0();				  \
	la     reg, 999f;		  \
SET_POP();				  \
	cache  ICACHE_ADDR_FILL, 0(reg);  \
	sync;				  \
	nop; nop; nop; nop;		  \
999:

#define ICACHE_ADDR_INVALIDATE_OP(addr,scratch)			  \
	    ICACHE_INVALIDATE_WORKAROUND(scratch);		  \
	    cache ICACHE_ADDR_HIT_INVALIDATE, 0(addr)

#ifdef MT7510_DCACHE_TEST
#define CACHE_TEST_FAIL_GPIO		9
#define CACHE_TEST_PASS_GPIO		8
#define CACHE_TEST_DONE_GPIO		10

		la		t1, CR_ARB_BASE		//cache test decided by hwtrap		
		lw		t2, 0x8c(t1)
		nop
		
		srl		t2, t2, 22
		andi		t2, t2, 0x3
		beq		t2, 0, normal_init		
		nop

		/*Init GPIO Configuration*/
		/*GPIO 8,9,10*/
		li       a0, CACHE_TEST_FAIL_GPIO
		li	a1, 1
		jal     turnon_gpio
		nop
		
		li       a0, CACHE_TEST_PASS_GPIO
		li	a1, 1
		jal     turnon_gpio
		nop
		
		li       a0, CACHE_TEST_DONE_GPIO
		li	a1, 1
		jal     turnon_gpio
		nop

		/*Test start*/
		jal DCacheTag_test
		nop
		bnez		v0, ct_fail
		nop

		jal 	DCacheData_test
		nop
		bnez		v0, ct_fail
		nop

		/*Show Test Pass, Turn on GPIO 8*/

		li       a0, CACHE_TEST_PASS_GPIO
		li	a1, 0
		jal     turnon_gpio
		nop
		
		j ct_done
		nop

ct_fail:
		/*Show Test Fail, Turn on GPIO 9*/

		li       a0, CACHE_TEST_FAIL_GPIO
		li	a1, 0
		jal     turnon_gpio
		nop

ct_done:
		/*Show Test Done, Turn on GPIO 10*/
		
		li       a0, CACHE_TEST_DONE_GPIO
		li	a1, 0
		jal     turnon_gpio
		nop
		


normal_init:
#endif
#ifndef UNCACHE_MODE
		la		t1, BOOTROM_START_ADDR
		la		t2, BOOTROM_END_ADDR
1:
SET_MIPS3()
		cache   DCACHE_ADDR_HIT_WRITEBACK, 0(t1)
		sync
		cache 	ICACHE_ADDR_HIT_INVALIDATE, 0(t1)
SET_MIPS0()

		addiu 	t1, t1, 32
		bne		t2, t1, 1b
		nop
#endif

		DISPLAYLED(0x5e, 0x20000)

#ifdef DEBUG
		li t7, 'f'
          	jal disp_character
          	nop

#endif

		/* Disable pbus access FE memory */
		la	v0, SHARE_FEMEM_SEL
		lw	t6, 0x0(v0)
		nop
 		li 	t2, ~0x1
		and	t6, t6, t2
		sw	t6, 0x0(v0)
		nop


  	la	k0, BOOTROM_START_ADDR
  	//la	k0, 0xa0000000 //CML_uncache
		jr	k0
		nop
error:
	/*  Error handling. Display error code (if device for this is available) and
	 *  enter an infinite loop (from init.h)
	 */
		b error
		nop

#ifdef TC3262

		.set noreorder

/*
 * Cache attribute values in the C field of EntryLo and the
 * K0 field of Config
 */
#define K_CacheAttrCWTnWA	0			/* Cacheable, write-thru, no write allocate */
#define K_CacheAttrCWTWA	1			/* Cacheable, write-thru, write allocate */
#define K_CacheAttrU		2			/* Uncached */
#define K_CacheAttrC		3			/* Cacheable */
#define K_CacheAttrCN		3			/* Cacheable, non-coherent */
#define K_CacheAttrCCE		4			/* Cacheable, coherent, exclusive */
#define K_CacheAttrCCS		5			/* Cacheable, coherent, shared */
#define K_CacheAttrCCU		6			/* Cacheable, coherent, update */
#define K_CacheAttrUA		7			/* Uncached accelerated */

#define S_ConfigMM              18     /* 24K specific, merging enable/disable */
#define M_ConfigMM              (0x1 << S_ConfigMM)

/*  Generic MIPS32/MIPS64 fields of CONFIG0 register (ie the ones not
 *  reserved for implementations)
 */
#define CONFIG0_MIPS32_64_MSK  0x8000ffff

/* MIPS 34K specifics */
#define STATUS_MIPS34K	    0
#define CONFIG0_MIPS34K  ((K_CacheAttrCN << S_ConfigK23) |\
			  (K_CacheAttrCN << S_ConfigKU)  |\
			  (M_ConfigMM))

/*  Generic MIPS32/MIPS64 fields of STATUS register (ie the ones not 
 *  reserved for implementations)
 */
#define STATUS_MIPS32_64_MSK   0xfffcffff

/*  Setup of STATUS register used for MIPS32/MIPS64 processors
 *  FR field only relevant for MIPS64 (Read only for MIPS32)
 */
#define STATUS_MIPS32_64   (M_StatusBEV | M_StatusFR)

#ifdef UNCACHE_MODE
#define CONFIG0_MIPS32_64    (K_CacheAttrU << S_ConfigK0)
#else
#define CONFIG0_MIPS32_64    (K_CacheAttrCN << S_ConfigK0)
//#define CONFIG0_MIPS32_64    (K_CacheAttrCWTnWA << S_ConfigK0)
#endif

/*
 *************************************************************************
 *                S O F T W A R E   G P R   I N D I C E S                *
 *************************************************************************
 *
 * These definitions provide the index (number) of the GPR, as opposed
 * to the assembler register name ($n).
 */

#define R_zero			 0
#define R_AT			 1
#define R_v0			 2
#define R_v1			 3
#define R_a0			 4
#define R_a1			 5
#define R_a2			 6
#define R_a3			 7
#define R_t0			 8
#define R_t1			 9
#define R_t2			10
#define R_t3			11
#define R_t4			12
#define R_t5			13
#define R_t6			14
#define R_t7			15
#define R_s0			16
#define R_s1			17
#define R_s2			18
#define R_s3			19
#define R_s4			20
#define R_s5			21
#define R_s6			22
#define R_s7			23
#define R_t8			24
#define R_t9			25
#define R_k0			26
#define R_k1			27
#define R_gp			28
#define R_sp			29
#define R_fp			30
#define R_s8			30
#define R_ra			31


#define KSEG0BASE		  0x80000000

/*  We use ssnop instead of nop operations in order to handle 
 *  superscalar CPUs.
 *  The "sll zero,zero,1" notation is compiler backwards compatible.
 */
#define SSNOP   sll zero,zero,1
#define EHB     sll zero,zero,3
#define NOPS	SSNOP; SSNOP; SSNOP; EHB

#define MFC0_SEL_OPCODE(dst, src, sel)\
	  	.##word (0x40000000 | ((dst)<<16) | ((src)<<11) | (sel))

#define MTC0_SEL_OPCODE(src, dst, sel)\
	  	.##word (0x40800000 | ((src)<<16) | ((dst)<<11) | (sel));\
		NOPS

/************************************************************************
 *  sys_init_processor
 ************************************************************************/
LEAF(sys_init_processor)
#if 0
		MFC0_SEL_OPCODE(R_t0, 16, 7)
		li  	t1, (1<<8)
		or		t0, t1
		MTC0_SEL_OPCODE(R_t0, 16, 7)
#endif
		/* Setup MIPS34K specifics (implementation dependent fields) */
		mfc0 	t0, C0_Config 
		li		t1, CONFIG0_MIPS32_64_MSK
		and		t0, t1
		li		t1, CONFIG0_MIPS34K
		or		t0, t1
		mtc0	t0, C0_Config 

		mfc0   	t0, C0_Status 
		li		t1, STATUS_MIPS32_64_MSK
		and		t0, t1
		li		t1, STATUS_MIPS34K
		or		t0, t1
		mtc0	t0, C0_Status 

		/* Setup generic MIPS32/MIPS64 fields of STATUS register */
		mfc0   	t0, C0_Status 
		li		t1, ~STATUS_MIPS32_64_MSK
		and		t0, t1
		li		t1, STATUS_MIPS32_64
		or		t0, t1
		mtc0	t0, C0_Status 

		/* Setup generic MIPS32 fields of CONFIG0 register */
		mfc0   	t0, C0_Config 
		li		t1, ~CONFIG0_MIPS32_64_MSK
		and		t0, t1
		li		t1, CONFIG0_MIPS32_64
		or		t0, t1
		mtc0	t0, C0_Config 

		jr		ra
		move	v0, zero
END(sys_init_processor)

/************************************************************************
 *  sys_init_cache
 ************************************************************************/
LEAF(sys_init_cache)

#define RA					t4
#define icache_size			t3
#define icache_linesize		t2
#define dcache_size			t1
#define dcache_linesize		t0

		move	RA, ra

		/* set icache, dcache size to 32KB only for FPGA board */
#ifdef TC3262_FPGA
		mfc0    s5, C0_Config 
		MFC0_SEL_OPCODE( R_s6, R_C0_Config1, R_C0_SelConfig1 )

		/* Enable write access to config1 */
		li		t0, 1<<19
		or		t0, s5, t0
		mtc0	t0, C0_Config 

		li  	s0, 2

		/* Set Icache settings to requested values */
		li		t0, ~(M_Config1IS | M_Config1DS)
		and		t0, s6, t0

		li		t1, S_Config1IS
		sllv	t1, s0, t1
		or		t0, t1

		/* Set Dcache settings to requested values */
		li		t1, S_Config1DS
		sllv	t1, s0, t1
		or		t0, t1

		/* Write CONFIG1 register */
		MTC0_SEL_OPCODE( R_t0, R_C0_Config1, R_C0_SelConfig1 )

		/* Disable write access to config1 */
		mtc0	s5, C0_Config 

		/* Empty pipeline */
		nop; nop; nop; nop; nop; nop
#endif

		/* L1 cache */
		bal		sys_determine_icache_linesize
		nop
		move	icache_linesize, v0

		bal		sys_determine_icache_lines
		nop
		multu	icache_linesize, v0
		mflo	icache_size

		bal		sys_determine_dcache_linesize
		nop
		move	dcache_linesize, v0

		bal		sys_determine_dcache_lines
		nop
		multu	dcache_linesize, v0
		mflo	dcache_size

		/* Initialise instruction cache */
		move	a0, icache_size
		move	a1, icache_linesize
		bal		sys_init_icache
		nop
	
		/* Initialise data cache */
		move	a0, dcache_size
		move	a1, dcache_linesize
		bal		sys_init_dcache
		nop

		/* Done */
		jr		RA
		move	v0, zero
END(sys_init_cache)


#define R_C0_DTagLo	      	28
#define R_C0_SelDTagLo	    2

#define R_C0_ITagLo			28
#define R_C0_SelITagLo		0

LEAF( sys_init_icache )
		/* 24K/24KE/34K : Clear ITagLo */
		MTC0_SEL_OPCODE( R_zero, R_C0_ITagLo, R_C0_SelITagLo )

		/* Calc an address that will correspond to the first cache line */
		li		a2, KSEG0BASE

		/* Calc an address that will correspond to the last cache line  */
		addu	a3, a2, a0
		subu    a3, a1

		/* Loop through all lines, invalidating each of them */
1:	
SET_MIPS3()
		cache	ICACHE_INDEX_STORE_TAG, 0(a2)	/* clear tag */
SET_MIPS0()
		bne		a2, a3, 1b
		addu	a2, a1

		jr		ra
		nop
END(sys_init_icache )

LEAF(sys_init_dcache )
		/* 24K/24KE/34K : Clear DTagLo */
		MTC0_SEL_OPCODE( R_zero, R_C0_DTagLo, R_C0_SelDTagLo )

		/* Calc an address that will correspond to the first cache line */
		li		a2, KSEG0BASE
	
		/* Calc an address that will correspond to the last cache line  */
		addu	a3, a2, a0
		subu    a3, a1

		/* Loop through all lines, invalidating each of them */
1:	
SET_MIPS3()
		cache	DCACHE_INDEX_STORE_TAG, 0(a2)	/* clear tag */
SET_MIPS0()
		bne		a2, a3, 1b
		addu	a2, a1

		jr		ra
		nop
END(sys_init_dcache )

LEAF(sys_determine_icache_linesize)	

		/* Read CONFIG1 register, which holds implementation data */
		MFC0_SEL_OPCODE( R_t9, R_C0_Config1, R_C0_SelConfig1 )

#define config1	t9

		/* I-cache line size */
		li		t8, M_Config1IL
		and		t8, config1
		li		t7, S_Config1IL
		srl		t8, t7
		li		t7, 0x2
		sll		v0, t7, t8

		jr		ra
		nop
END(sys_determine_icache_linesize)	
	
LEAF(sys_determine_icache_lines)	

		/* Read CONFIG1 register, which holds implementation data */
		MFC0_SEL_OPCODE( R_t9, R_C0_Config1, R_C0_SelConfig1 )

#define config1	t9

		/* I-cache lines
		 * Calculated as associativity * sets per way
		 */
		li		t8, M_Config1IA
		and		t8, config1
		li		t7, S_Config1IA
		srl		t8, t7
		addiu	t8,1				/* t8 = associativity	*/

		li		t7, M_Config1IS
		and		t7, config1
		li		t9, S_Config1IS
		srl		t7, t9
		li		t9, 0x40
		sll		t7, t9, t7			/* t7 = sets per way	*/

		multu	t8, t7
		mflo    v0

		jr		ra
		nop
END(sys_determine_icache_lines)

LEAF(sys_determine_dcache_linesize)	

		/* Read CONFIG1 register, which holds implementation data */
		MFC0_SEL_OPCODE( R_t9, R_C0_Config1, R_C0_SelConfig1 )

#define config1	t9

		/* D-cache line size */
		li		t8, M_Config1DL
		and		t8, config1
		li		t7, S_Config1DL
		srl		t8, t7
		li		t7, 0x2
		sll		v0, t7, t8

		jr		ra
		nop
END(sys_determine_dcache_linesize)	

LEAF(sys_determine_dcache_lines)	
		/* Read CONFIG1 register, which holds implementation data */
		MFC0_SEL_OPCODE( R_t9, R_C0_Config1, R_C0_SelConfig1 )

#define config1	t9

		/* D-cache lines
		 * Calculated as associativity * sets per way
		 */
		li		t8, M_Config1DA
		and		t8, config1
		li		t7, S_Config1DA
		srl		t8, t7
		addiu	t8,1				/* t8 = associativity	*/

		li		t7, M_Config1DS
		and		t7, config1
		li		t9, S_Config1DS
		srl		t7, t9
		li		t9, 0x40
		sll		t7, t9, t7			/* t7 = sets per way	*/

		multu	t8, t7
		mflo    v0

		jr		ra
		nop
END(sys_determine_dcache_lines)



#ifndef BOOTROM_IN_SRAM
#ifdef MT7510_FPGA_TEST
/************************************************************************
 *
 *                          sys_init_mtk_dmc
 *  Description :
 *  -------------
 *  Perform MTK DMC initialization
 *
 *  Parameters :
 *  ------------
 *
 *  Return values :
 *  ---------------
 *  0 If OK, error code != 1 if error
  ************************************************************************/
//CML_mt7510_start
/*
#define	TRCD	15 // 4 bits
#define	TRP		15 // 4 bits
#define	TFAW	15 // 4 bits
#define	TWR		15 // 4 bits
#define	TWTR	15 // 4 bits
#define	TRC		31 // 5 bits
#define	TRAS	15 // 4 bits
#define	TRRD	 7 // 3 bits
#define	REFCNT 255 // 8 bits
#define	TRFC   255 // 8 bits
#define	TRTP	 7 // 3 bits
*/

#define SDRAM	0
#define DDR3	1
#define DDR1	2
#define DDR2	3
#define REG_RW_DELAY	10 // delay for register read/write
#define CLK_CYCLE_NS	s7 // store the number of clock cycle for one nano second

//#define CLK32M_DELAY_32NS nop; // 1 instruction about 32 ns for 32M clock


LEAF(sys_init_mtk_dmc)

		// judge the platform, DRAM type & size
		la		k0, CR_ARB_BASE
		lw		t0, 0x8c(k0)
		nop
		srl		s0, t0, 31 // FPGA indication
		srl		s1, t0, 24
		andi	s1, s1, 0x3	// DRAM type
		srl		s2, t0, 13
		andi	s2, s2, 0x7	// DRAM size
		
		// judge FPGA or ASIC platform
		bnez	s0, 1f // FPGA
		nop
		li		CLK_CYCLE_NS, 16 // store the number of clock cycle for one nano second in s7, ASIC
		j 88f
		nop	
1:
		li		CLK_CYCLE_NS, 16 // store the number of clock cycle for one nano second in s7, FPGA
88:

		/* de-assert DRAM controller reset */
	  la		k0,	CR_ARB_BASE
		li  	t0, 0x00000000      
		sw		t0, 0x40(k0)
		nop
		
		// total delay 200us
		DLY_NS(50000, CLK_CYCLE_NS) // wait 50us
		DLY_NS(50000, CLK_CYCLE_NS) // wait 50us
		DLY_NS(50000, CLK_CYCLE_NS) // wait 50us
		DLY_NS(50000, CLK_CYCLE_NS) // wait 50us		
		
		la		k0, CR_DMC_BASE // k0 stores DRAMC base address

		// FPGA platform
		bnez	s0, PLL_CLK_CFG_DONE // FPGA doesn't need to config PLL & clock source setting
		nop
			
		/* *** PLL & clock source setting *** */
		// judge DDR2 or DDR3
		li		t0, DDR3
		beq		s1, t0, 3f	// DDR3 type
		nop
		
		// DDR2
		//li		t0, 0xc2064357 // set FREQDIV2, WTLAT, DATLAT, AC timing, ODT/OCD control, DDR2EN for DDR2
		li		t0, 0xc2061237 // set FREQDIV2, WTLAT, DATLAT, AC timing, ODT/OCD control, DDR2EN for DDR2 cml_dbg
		sw		t0, 0x7c(k0)
		nop
		
		j 99f
		nop
3:
		// DDR3
		li		t0, 0xb18732dd // set FREQDIV2, WTLAT, DATLAT, AC timing, ODT/OCD control for DDR3
		sw		t0, 0x7c(k0)
		nop
99:
		li		t0, 0x00000003 // set to 1-PLL sync mode
		sw		t0, 0x640(k0)
		nop
		li		t0, 0x02005a00 // set RG_MEMPLL_BIAS_PWD
		sw		t0, 0x634(k0)
		nop
		DLY_NS(2000, CLK_CYCLE_NS) // wait 2us
		li		t0, 0x02005800 // set RG_MEMPLL_BIAS_RST
		sw		t0, 0x634(k0)
		nop
		
		// wait 200us
		DLY_NS(50000, CLK_CYCLE_NS) // wait 50us
		DLY_NS(50000, CLK_CYCLE_NS) // wait 50us
		DLY_NS(50000, CLK_CYCLE_NS) // wait 50us
		DLY_NS(50000, CLK_CYCLE_NS) // wait 50us
		
		li		t0, 0xd000000e // set RG_MEMPLL_PWD
		sw		t0, 0x600(k0)
		nop
		li		t0, 0xd000200e // set RG_MEMPLL_MONCKEN
		sw		t0, 0x600(k0)
		nop
		li		t0, 0xd200200e // set RG_MEMPLL_FMEN
		sw		t0, 0x600(k0)
		nop
		li		t0, 0x4ac6021c // set RG_DMSS_PWDB
		sw		t0, 0x608(k0)
		nop
		DLY_NS(20000, CLK_CYCLE_NS) // wait 20us
		li		t0, 0x4ac6061c // set RG_DMSS_RSTB
		sw		t0, 0x608(k0)
		nop
		DLY_NS(40, CLK_CYCLE_NS) // wait 40ns
		li		t0, 0xc0000101 // set RG_DMSS_PCW_NCPO_CHG
		sw		t0, 0x604(k0)
		nop
		DLY_NS(40, CLK_CYCLE_NS) // wait 40ns
		li		t0, 0x4ac61e1c // set RG_DMSS_NCPO_EN
		sw		t0, 0x608(k0)
		nop
		li		t0, 0x000d0901 // set RG_DMSS_LPF_EN
		sw		t0, 0x610(k0)
		nop
		DLY_NS(2000, CLK_CYCLE_NS) // wait 2us
		li		t0, 0x4ac61f1c // set RG_MEMPLL_DDSEN
		sw		t0, 0x608(k0)
		nop
		DLY_NS(20000, CLK_CYCLE_NS) // wait 20us
		li		t0, 0x008d0901 // set RG_MEMPLL_DIV_EN
		sw		t0, 0x610(k0)
		nop
		DLY_NS(20000, CLK_CYCLE_NS) // wait 20us

		// judge DDR2 or DDR3
		li		t0, DDR3
		beq		s1, t0, 3f	// DDR3 type
		nop
		
		// DDR2
		li		t0, 0xd0008013 // set RG_MEMPLL2_FBDIV2 & RG_MEMPLL2_FBDIV
		sw		t0, 0x614(k0)
		nop
		li		t0, 0x00000001 // set RG_MEMPLL2_FB_MCK_SEL
		sw		t0, 0x618(k0)
		nop
		li		t0, 0x13000000 // set RG_MEMPLL3_FBDIV2
		sw		t0, 0x61c(k0)
		nop
		li		t0, 0x01d00080 // set RG_MEMPLL3_FBDIV
		sw		t0, 0x620(k0)
		nop
		li		t0, 0x00000000 // set RG_MEMPLL3_FB_MCK_SEL
		sw		t0, 0x624(k0)
		nop
		li		t0, 0x80130000 // set RG_MEMPLL4_FBDIV2
		sw		t0, 0x628(k0)
		nop
		li		t0, 0x0001d000 // set RG_MEMPLL4_FBDIV & RG_MEMPLL4_FB_MCK_SEL
		sw		t0, 0x62c(k0)
		nop
		li		t0, 0xd0008012 // set RG_MEMPLL2_PWD & RG_MEMPLL2_POSDIV
		sw		t0, 0x614(k0)
		nop
		li		t0, 0x12000000 // set RG_MEMPLL3_PWD
		sw		t0, 0x61c(k0)
		nop
		li		t0, 0x01d00080 // set RG_MEMPLL3_POSDIV
		sw		t0, 0x620(k0)
		nop
		li		t0, 0x80120000 // set RG_MEMPLL4_PWD & RG_MEMPLL4_POSDIV
		sw		t0, 0x628(k0)
		nop
		
		j 99f
		nop
3:
		// DDR3
		li		t0, 0xd0008015 // set RG_MEMPLL2_FBDIV2 & RG_MEMPLL2_FBDIV
		sw		t0, 0x614(k0)
		nop
		li		t0, 0x00000001 // set RG_MEMPLL2_FB_MCK_SEL
		sw		t0, 0x618(k0)
		nop
		li		t0, 0x15000000 // set RG_MEMPLL3_FBDIV2
		sw		t0, 0x61c(k0)
		nop
		li		t0, 0x01d00080 // set RG_MEMPLL3_FBDIV
		sw		t0, 0x620(k0)
		nop
		li		t0, 0x00000000 // set RG_MEMPLL3_FB_MCK_SEL
		sw		t0, 0x624(k0)
		nop
		li		t0, 0x80150000 // set RG_MEMPLL4_FBDIV2
		sw		t0, 0x628(k0)
		nop
		li		t0, 0x0001d000 // set RG_MEMPLL4_FBDIV & RG_MEMPLL4_FB_MCK_SEL
		sw		t0, 0x62c(k0)
		nop
		li		t0, 0xd0008014 // set RG_MEMPLL2_PWD & RG_MEMPLL2_POSDIV
		sw		t0, 0x614(k0)
		nop
		li		t0, 0x14000000 // set RG_MEMPLL3_PWD
		sw		t0, 0x61c(k0)
		nop
		li		t0, 0x01d00080 // set RG_MEMPLL3_POSDIV
		sw		t0, 0x620(k0)
		nop
		li		t0, 0x80140000 // set RG_MEMPLL4_PWD & RG_MEMPLL4_POSDIV
		sw		t0, 0x628(k0)
		nop
		
99:
		DLY_NS(20000, CLK_CYCLE_NS) // wait MEMPLL stable 20us
		li		t0, 0x00000013 // enable ALLCLK_EN
		sw		t0, 0x640(k0)
		nop
		li		t0, 0x00000033 // enable PLL2CLK_EN
		sw		t0, 0x640(k0)
		nop


PLL_CLK_CFG_DONE:

		DLY_CLK_CYCLE(REG_RW_DELAY)
		
		li		t0, 0x0000110d // set test agent 2 parameter
		sw		t0, 0x48(k0)
		nop		

		// judge DDR2 or DDR3
		li		t0, DDR3
		beq		s1, t0, 3f	// DDR3 type
		nop
		
		// DDR2
		// judge FPGA or ASIC platform
		bnez	s0, 1f // FPGA
		nop
		li		t0, 0x00500900 // set pinmux for DDR2
		j 88f
		nop	
1:
		//li		t0, 0xc0500900 // set pinmux[31:30] - 0x3 for DDR2 FPGA
		li		t0, 0x00500900 // set pinmux[31:30] - 0x3 for 7512 DDR2 FPGA
88:
		sw		t0, 0xd8(k0)
		nop
	
		li		t0, 0x00000001 // reserved for DDR2
		sw		t0, 0xe4(k0)
		nop
	
		li		t0, 0x00000001 // set FIFO length
		sw		t0, 0x8c(k0)
		nop

		// judge FPGA or ASIC platform
		bnez	s0, 1f // FPGA
		nop
		li		t0, 0x00000000 // reserved
		j 88f
		nop	
1:
		li		t0, 0x80000000 // FPGA DQS single-end output
88:
		sw		t0, 0x90(k0)
		nop

		li		t0, 0x80000000 // set DQS gating window fine-tuning for DDR2
		sw		t0, 0x94(k0)
		nop
	
		//li		t0, 0x83080080 // set DQS gating window coarse-tuning
		li		t0, 0x83008008 // set DQS gating window coarse-tuning //CML_20130104, for DDR2 performance
		sw		t0, 0xdc(k0)
		nop

		// judge FPGA or ASIC platform
		bnez	s0, 1f // FPGA
		nop
		li		t0, 0x1a080080 // set DQS gating window coarse-tuning for DDR2
		j 88f
		nop	
1:
		//li		t0, 0x00080080 // FPGA no DQSN, set DQS gating window coarse-tuning for DDR2 cml_dbg 1121
		li		t0, 0x10008008 // FPGA no DQSN, set DQS gating window coarse-tuning for DDR2 //CML_20130104, for DDR2 performance
88:
		sw		t0, 0xe0(k0)
		nop
	
		j			99f
		nop
3:
		// DDR3
		//li		t0, 0x00100900 // set pinmux for DDR3 //cml_dbg 1206
		//sw		t0, 0xd8(k0)
		//nop
		// judge FPGA or ASIC platform //cml_dbg 1206
		bnez	s0, 1f // FPGA
		nop
		li		t0, 0x00100900 // set pinmux for DDR3
		j 88f
		nop	
1:
		//li		t0, 0xc0100900 // set pinmux for 7512 DDR3 FPGA
		li		t0, 0x00100900 // set pinmux for 7512 DDR3 FPGA
88:
		sw		t0, 0xd8(k0)
		nop

		#if 0 //cml_dbg 1206
		li		t0, 0x000000a3 // set DDR3EN, BC4 for DDR3
		sw		t0, 0xe4(k0)
		nop
		#else
		li		t0, 0x00000002 // set DDR3EN, BC4 for DDR3
		sw		t0, 0xe4(k0)
		nop
		DLY_NS(400, CLK_CYCLE_NS) // 400ns
		li		t0, 0x000000a3 // set DDR3EN, BC4 for DDR3
		sw		t0, 0xe4(k0)
		nop
		#endif
		
		li		t0, 0x00000001 // set FIFO length
		sw		t0, 0x8c(k0)
		nop

		li		t0, 0x00000000 // reserved
		sw		t0, 0x90(k0)
		nop
		
		//li		t0, 0x00000000 // set DQS gating window fine-tuning for DDR3
		li		t0, 0x80000000 // set DQS gating window fine-tuning for DDR3 cml_dbg 1206
		sw		t0, 0x94(k0)
		nop
		
		// judge FPGA or ASIC platform
		bnez	s0, 1f // FPGA
		nop
		li		t0, 0x83080080 // set DQS gating window coarse-tuning
		sw		t0, 0xdc(k0)
		nop
		
		li		t0, 0x13080080 // set DQS gating window coarse-tuning for DDR3
		sw		t0, 0xe0(k0)
		nop
		
		j 88f
		nop	
1:
		li		t0, 0x83002002 // set DQS gating window coarse-tuning
		sw		t0, 0xdc(k0)
		nop
		
		li		t0, 0x11002002 // set DQS gating window coarse-tuning for DDR3
		sw		t0, 0xe0(k0)
		nop
		
88:

99:
		li		t0, 0x00000000 // reserved
		sw		t0, 0xf0(k0)
		nop
		
		//li		t0, 0x01000000 // set 8BKEN
		//sw		t0, 0xf4(k0)
		//nop
		// judge DRAM type & size to decide bank number
		li		t0, 0x1
		beq		s1, t0, 8f	// DDR3 type, all DDR3 is 8 bank
		nop
		li		t0, 0x3
		bne		s1, t0, 4f	// not DDR2 & DDR3 type, set as 4 bank
		nop
		li		t0, 0x5
		bltu	s2, t0, 4f	// DDR2 type, size < 128 MB, 4 bank; else 8 bank
		nop
8:
		li		t0, 0x01000000 // 8-bank
		j			77f
		nop
4:
		li		t0, 0x00000000 // 4-bank
77:
		sw		t0, 0xf4(k0)
		nop
		
		li		t0, 0x00000080 // set MAXPENDCNT
		sw		t0, 0x168(k0)
		nop
		li		t0, 0x30000000 // enable DRAM clock
		sw		t0, 0x130(k0)
		nop

		// judge DDR2 or DDR3
		li		t0, DDR3
		beq		s1, t0, 3f	// DDR3 type
		nop
		
		// DDR2
		// judge FPGA or ASIC platform
		bnez	s0, 1f // FPGA
		nop
		li		t0, 0x00700900 // set DRAMEN for DDR2
		j 88f
		nop	
1:
		//li		t0, 0xc0700900 // set DRAMEN for DDR2 FPGA
		li		t0, 0x00700900 // set DRAMEN for 7512 DDR2 FPGA
88:
		sw		t0, 0xd8(k0)
		nop
		
		DLY_CLK_CYCLE(REG_RW_DELAY)
		
		//li		t0, 0xf00485e2 // set CMDHLD, 16Bit DQ for DDR2
		//sw		t0, 0x4(k0)
		//nop
		// judge FPGA or ASIC platform
		bnez	s0, 1f // FPGA
		nop
		li		t0, 0xf00485e2 // set CMDHLD, 16Bit DQ, MATYPE[9:8], TRRD[7:6] for DDR2
		j 88f
		nop	
1:
		li		t0, 0xf00485a2 // set CMDHLD, 16Bit DQ, MATYPE[9:8], TRRD[7:6]  for DDR2 FPGA
88:
		
		// judge DRAM type & size to decide column address bit number
		li		t1, 0xfffffcff
		and		t0,	t0, t1 // clear column address width
		
		li		t1, 0x1
		beq		s1, t1, 10f	//	DDR3 type, all DDR3 column address width is 10 bit
		nop
		li		t1, 0x3
		bne		s1, t1, 9f	//	not DDR2 & DDR3 type, set column address width as 9 bit
		nop
		li		t1, 0x4
		bltu	s2, t1, 9f	// DDR2 type, size < 64 MB, set column address width as 9 bit; else 10 bit
		nop
10:
		ori		t0, 0x00000200 // 10-bit
		j			66f
		nop
9:
		ori		t0, 0x00000100 // 9-bit
66:
		sw		t0, 0x4(k0)
		nop
		
		// judge FPGA or ASIC platform
		bnez	s0, 1f // FPGA
		nop
		li		t0, 0x80000033 // set to new DQS gating for DDR2
		sw		t0, 0x124(k0)
		nop
		
		li		t0, 0x40404040 // set DQS gating window fine-tuning
		sw		t0, 0x94(k0)
		nop
		li		t0, 0x8000c8b8 // enable DQS gating window auto calibration
		sw		t0, 0x1c0(k0)
		nop
		
		li		t0, 0xc2064357 // set FREQDIV2, WTLAT, DATLAT, AC timing, ODT/OCD control, DDR2EN for DDR2
		sw		t0, 0x7c(k0)
		nop
		
		li		t0, 0xf1200f01 // set DLLFRZ
		sw		t0, 0x28(k0)
		nop
		li		t0, 0xcc000000 // set ADRDEC, WATRG, FASTOE for DDR2
		sw		t0, 0x1e0(k0)
		nop
		
		j 88f
		nop	
1:
		//li		t0, 0x00000000 // FPGA no DQSN
		li		t0, 0x80000033 // FPGA no DQSN //CML_20130104, for DDR2 performance
		sw		t0, 0x124(k0)
		nop
		
		#if 0 // FPGA no DQSN, cml_dbg 1119
		li		t0, 0x40404040 // set DQS gating window fine-tuning for DDR2 FPGA
		sw		t0, 0x94(k0)
		nop
		li		t0, 0x8000c8b8 // enable DQS gating window auto calibration for DDR2 FPGA
		sw		t0, 0x1c0(k0)
		nop
		#endif // cml_dbg 1119

	
		//li		t0, 0xb206123f // cml_dbg 1119
		//li		t0, 0xb2061237 // cml_dbg 1121
		//li		t0, 0xb2061237 // cml_dbg 1127_2
		li		t0, 0x9103123f //CML_20130104, for DDR2 performance
		sw		t0, 0x7c(k0)
		nop
		
		//li		t0, 0xf1000301 // set DLLFRZ for DDR2 FPGA
		//li		t0, 0xf1200f01 // set DLLFRZ for DDR2 FPGA cml_dbg 1121
		li		t0, 0xf1000301 // set DLLFRZ for DDR2 FPGA //CML_20130104, for DDR2 performance
		sw		t0, 0x28(k0)
		nop
		li		t0, 0x80000000 // set ADRDEC, WATRG, FASTOE for DDR2 FPGA
		sw		t0, 0x1e0(k0)
		nop
88:
		li		t0, 0x00000000 // reserved
		sw		t0, 0x158(k0)
		nop
		
		//li		t0, 0x00111190 // cml_dbg 1119 new add
		//li		t0, 0x00051100 // cml_dbg 1121
		li		t0, 0x00111190 //CML_20130104, for DDR2 performance
		sw		t0, 0x110(k0)
		nop
		
		DLY_CLK_CYCLE(5) //CML_20130104, for DDR2 performance
		
		j			99f
		nop
3:
		// DDR3
		
		//li		t0, 0x00300900 // set DRAMEN for DDR3 //cml_dbg 1206
		// judge FPGA or ASIC platform //cml_dbg 1206
		bnez	s0, 1f // FPGA
		nop
		li		t0, 0x00300900 // set DRAMEN for DDR3
		j 88f
		nop	
1:
		//li		t0, 0xc0300900 // set DRAMEN for DDR3 FPGA
		li		t0, 0x00300900 // set DRAMEN for 7512 DDR3 FPGA
88:
		sw		t0, 0xd8(k0)
		nop
		
		DLY_CLK_CYCLE(REG_RW_DELAY)
		
		//li		t0, 0xf07486e2 // set CMDHLD, 16Bit DQ for DDR3
		//sw		t0, 0x4(k0)
		//nop
		// judge DRAM type & size to decide column address bit number
		li		t0, 0xf07486e2 // set CMDHLD, 16Bit DQ for DDR3
		li		t1, 0xfffffcff
		and		t0,	t0, t1 // clear column address width
		
		li		t1, 0x1
		beq		s1, t1, 10f	//	DDR3 type, all DDR3 column address width is 10 bit
		nop
		li		t1, 0x3
		bne		s1, t1, 9f	//	not DDR2 & DDR3 type, set column address width as 9 bit
		nop
		li		t1, 0x4
		bltu	s2, t1, 9f	// DDR2 type, size < 64 MB, set column address width as 9 bit; else 10 bit
		nop
10:
		ori		t0, 0x00000200 // 10-bit
		j			66f
		nop
9:
		ori		t0, 0x00000100 // 9-bit
66:
		sw		t0, 0x4(k0)
		nop
		
		li		t0, 0x80000011 // set to new DQS gating for DDR3
		sw		t0, 0x124(k0)
		nop

		li		t0, 0x40404040 // set DQS gating window fine-tuning
		sw		t0, 0x94(k0)
		nop
		li		t0, 0x8000c8b8 // enable DQS gating window auto calibration
		sw		t0, 0x1c0(k0)
		nop

		// judge FPGA or ASIC platform
		bnez	s0, 1f // FPGA
		nop
		li		t0, 0xb18732dd // set FREQDIV2, WTLAT, DATLAT, AC timing, ODT/OCD control for DDR3
		sw		t0, 0x7c(k0)
		nop
		j 88f
		nop	
1:
		li		t0, 0x918e32cd // set FREQDIV2, WTLAT, DATLAT, AC timing, ODT/OCD control for DDR3 FPGA
		sw		t0, 0x7c(k0)
		nop
88:
	
		
		bnez	s0, 1f // FPGA
		nop
		li		t0, 0xf1200f01 // set DLLFRZ for DDR3
		sw		t0, 0x28(k0)
		nop
		li		t0, 0x8c000000 // set ADRDEC, WATRG, FASTOE for DDR3
		sw		t0, 0x1e0(k0)
		nop
		j 88f
		nop	
1:
		li		t0, 0xf1000301 // set DLLFRZ for DDR3 //cml_dbg 1206
		sw		t0, 0x28(k0)
		nop
		li		t0, 0x88000000 // set ADRDEC, WATRG, FASTOE for DDR3 FPGA
		sw		t0, 0x1e0(k0)
		nop
88:

		li		t0, 0x00000000 // reserved
		sw		t0, 0x158(k0)
		nop
		
		li		t0, 0x00111190 // cml_dbg 1206, new add
		sw		t0, 0x110(k0)
		nop
99:		
		
		/* *** DRAM Initial Sequence *** */
		// judge DDR2 or DDR3  cml_dbg 1119
		li		t0, DDR3
		beq		s1, t0, 3f	// DDR3 type
		nop
		
		// DDR2
		// judge FPGA or ASIC platform
		bnez	s0, 1f // FPGA
		nop
		li		t0, 0xf07402e2 // enable CKEON
		j 88f
		nop
1:
		//li		t0, 0xf07406a2 // disable CKEON, turn off refresh when initial, test CMD to nop for DDR2 FPGA
		li		t0, 0xf00405a2 // disable CKEON, turn off refresh when initial, test CMD to nop for DDR2 FPGA //CML_20130104, for DDR2 performance
88:
		j 99f
		nop
3:
		// DDR3
		//li		t0, 0xf07402e2 // enable CKEON
		li		t0, 0xf07406e2 // enable CKEON cml_dbg 1213
99:
		// judge DRAM type & size to decide column address bit number
		li		t1, 0xfffffcff
		and		t0,	t0, t1 // clear column address width
		
		li		t1, 0x1
		beq		s1, t1, 10f	//	DDR3 type, all DDR3 column address width is 10 bit
		nop
		li		t1, 0x3
		bne		s1, t1, 9f	//	not DDR2 & DDR3 type, set column address width as 9 bit
		nop
		li		t1,	0x4
		bltu	s2, t1, 9f	// DDR2 type, size < 64 MB, set column address width as 9 bit; else 10 bit
		nop
10:
		ori		t0, 0x00000200 // 10-bit
		j			66f
		nop
9:
		ori		t0, 0x00000100 // 9-bit
66:
		sw		t0, 0x4(k0)
		nop
		
		// judge DDR2 or DDR3
		li		t0, DDR3
		beq		s1, t0, 3f	// DDR3 type
		nop
		
		// DDR2
		li		t0, 0x00000007 // enable CKEFIXON for DDR2
		sw		t0, 0xe4(k0)
		nop
		
		DLY_NS(400, CLK_CYCLE_NS) // 400ns by JEDEC spec
		
		#if 1 //CML_20130104, for DDR2 performance
		//li		t0, 0xb2061237 // cml_dbg 1119 new add
		li		t0, 0x91031237 //CML_20130104, for DDR2 performance
		sw		t0, 0x7c(k0)
		nop
		#endif // cml_dbg 1121

		li		t0, 0x00000004 // issue "Precharge All" for DDR2
		sw		t0, 0x1e4(k0)
		nop
		
		li		t0, 0x00000000
		sw		t0, 0x1e4(k0)
		nop
		
		DLY_CLK_CYCLE(4) // tRP (7nCK by JEDEC spec, clock DRAMC:DRAM=1:2)
		
		li		t0, 0x00004000 // set MR2 for DDR2
		sw		t0, 0x88(k0)
		nop
		
		li		t0, 0x00000001 // enable MRW
		sw		t0, 0x1e4(k0)
		nop
		
		DLY_CLK_CYCLE(1) // tMRD (2nCK by JEDEC spec, clock DRAMC:DRAM=1:2)
		
		j			99f
		nop
3:
		// DDR3
		#if 0 //cml_dbg 1206
		li		t0, 0x000000a7 // enable CKEFIXON for DDR3
		sw		t0, 0xe4(k0)
		nop
		#else
		li		t0, 0x00000002 // enable CKEFIXON for DDR3
		sw		t0, 0xe4(k0)
		nop
		DLY_NS(400, CLK_CYCLE_NS) // 400ns
		li		t0, 0x000000a7 // enable CKEFIXON for DDR3
		sw		t0, 0xe4(k0)
		nop
		#endif
		
		
		DLY_NS(310, CLK_CYCLE_NS) // tXPR, 310ns by JEDEC spec
		
		
		//li		t0, 0x00004208 // set MR2, RTT=ZQ/4, CWL=6 for DDR3 //cml_dbg 1206
		//sw		t0, 0x88(k0)
		//nop
		// judge FPGA or ASIC platform //cml_dbg 1206
		bnez	s0, 1f // FPGA
		nop
		li		t0, 0x00004208 // set MR2, RTT=ZQ/4, CWL=6 for DDR3 //cml_dbg 1206
		j 88f
		nop
1:
		li		t0, 0x00004000 // set MR2, RTT=ZQ/4, CWL=6 for DDR3 //cml_dbg 1206
88:
		sw		t0, 0x88(k0)
		nop
		
		li		t0, 0x00000001 // enable MRW
		sw		t0, 0x1e4(k0)
		nop
		
		DLY_CLK_CYCLE(2) // tMRD (4nCK by JEDEC spec, clock DRAMC:DRAM=1:2)
99:		
		li		t0, 0x00000000 // disable MRW
		sw		t0, 0x1e4(k0)
		nop
		li		t0, 0x00006000 // set MR3
		sw		t0, 0x88(k0)
		nop
		li		t0, 0x00000001 // enable MRW
		sw		t0, 0x1e4(k0)
		nop

		// judge DDR2 or DDR3
		li		t0, DDR3
		beq		s1, t0, 3f	// DDR3 type
		nop
		
		// DDR2
		DLY_CLK_CYCLE(1) // tMRD (2nCK by JEDEC spec, clock DRAMC:DRAM=1:2)
		
		li		t0, 0x00000000 // disable MRW
		sw		t0, 0x1e4(k0)
		nop
		
		// judge FPGA or ASIC platform
		bnez	s0, 1f // FPGA
		nop
		li		t0, 0x00002040 // set MR1 for DDR2
		j 88f
		nop	
1:
		li		t0, 0x00002441 // FPGA disable #DQS & DLL, set MR1 for DDR2
		//li		t0, 0x00002440 // FPGA disable #DQS & DLL, set MR1 for DDR2 //CML_20130104, for DDR2 performance
88:
		sw		t0, 0x88(k0)
		nop

		li		t0, 0x00000001 // enable MRW
		sw		t0, 0x1e4(k0)
		nop
		DLY_CLK_CYCLE(1) // tMRD (2nCK by JEDEC spec, clock DRAMC:DRAM=1:2)
		
		li		t0, 0x00000000 // disable MRW
		sw		t0, 0x1e4(k0)
		nop
		//li		t0, 0x00000b63 // set MR0, BL8, CL=6, DLL reset for DDR2
		li		t0, 0x00000b43 // set MR0, BL8, CL=4, DLL reset for DDR2 //CML_20130104, for DDR2 performance
		sw		t0, 0x88(k0)
		nop
		li		t0, 0x00000001 // enable MRW
		sw		t0, 0x1e4(k0)
		nop
		DLY_CLK_CYCLE(1) // tMRD (2nCK by JEDEC spec, clock DRAMC:DRAM=1:2)
		
		li		t0, 0x00000000 // disable MRW
		sw		t0, 0x1e4(k0)
		nop

		li		t0, 0x00000004 // issue "Precharge All" for DDR2
		sw		t0, 0x1e4(k0)
		nop
		li		t0, 0x00000000 //
		sw		t0, 0x1e4(k0)
		nop
		DLY_CLK_CYCLE(4) // tRP (7nCK by JEDEC spec, clock DRAMC:DRAM=1:2)

		li		t0, 0x00000a43 //CML_20130104, for DDR2 performance
		sw		t0, 0x88(k0)
		nop
		
		li		t0, 0x00000008 // issue "Auto Refresh" for DDR2
		sw		t0, 0x1e4(k0)
		nop
		li		t0, 0x00000000 //
		sw		t0, 0x1e4(k0)
		nop
		DLY_NS(328, CLK_CYCLE_NS) // tRFC (327.5ns(4Gb) by JEDEC spec)

		li		t0, 0x00000008 // issue "Auto Refresh" for DDR2
		sw		t0, 0x1e4(k0)
		nop
		li		t0, 0x00000000 //
		sw		t0, 0x1e4(k0)
		nop
		DLY_NS(328, CLK_CYCLE_NS) // tRFC (327.5ns(4Gb) by JEDEC spec)
		
		#if 0 //CML_20130104, for DDR2 performance
		li		t0, 0x00000a63 // set MR0, DLL not reset for DDR2
		sw		t0, 0x88(k0)
		nop
		#endif
		
		li		t0, 0x00000001 // enable MRW
		sw		t0, 0x1e4(k0)
		nop
		DLY_CLK_CYCLE(1) // tMRD (2nCK by JEDEC spec, clock DRAMC:DRAM=1:2)
		
		li		t0, 0x00000000 // disable MRW
		sw		t0, 0x1e4(k0)
		nop
		
		// judge FPGA or ASIC platform
		bnez	s0, 1f // FPGA
		nop
		li		t0, 0x000023c0 // set MR1, enter OCD calibration for DDR2
		j 88f
		nop	
1:
		li		t0, 0x000027c1 // FPGA disable #DQS & DLL, set MR1, enter OCD calibration for DDR2
		//li		t0, 0x000023c0 // FPGA disable #DQS & DLL, set MR1, enter OCD calibration for DDR2 //CML_20130104, for DDR2 performance
88:
		sw		t0, 0x88(k0)
		nop

		li		t0, 0x00000001 // enable MRW
		sw		t0, 0x1e4(k0)
		nop
		DLY_CLK_CYCLE(1) // tMRD (2nCK by JEDEC spec, clock DRAMC:DRAM=1:2)
		
		li		t0, 0x00000000 // disable MRW
		sw		t0, 0x1e4(k0)
		nop
		
		// judge FPGA or ASIC platform
		bnez	s0, 1f // FPGA
		nop
		li		t0, 0x00002040 // set MR1, exit OCD calibration for DDR2
		j 88f
		nop	
1:
		li		t0, 0x00002441 // FPGA disable #DQS & DLL, set MR1, exit OCD calibration for DDR2
		//li		t0, 0x00002440 // FPGA disable #DQS & DLL, set MR1, exit OCD calibration for DDR2 //CML_20130104, for DDR2 performance
88:
		sw		t0, 0x88(k0)
		nop

		li		t0, 0x00000001 // enable MRW
		sw		t0, 0x1e4(k0)
		nop
		DLY_CLK_CYCLE(1) // tMRD (2nCK by JEDEC spec, clock DRAMC:DRAM=1:2)
		
		li		t0, 0x00000000 // disable MRW
		sw		t0, 0x1e4(k0)
		nop
		
		// judge FPGA or ASIC platform
		bnez	s0, 1f // FPGA
		nop
		li		t0, 0xc206435f // enable WODTEN for DDR2
		sw		t0, 0x7c(k0)
		nop
		li		t0, 0x00000703 // disable CKEFIXON for DDR2
		sw		t0, 0xe4(k0)
		nop
				
		j 88f
		nop	
1:
		//li		t0, 0xb206123f // enable WODTEN for DDR2 FPGA
		//li		t0, 0xb206123f // cml_dbg 1127_2
		li		t0, 0x9103122f //CML_20130104, for DDR2 performance
		sw		t0, 0x7c(k0)
		nop
		//li		t0, 0x00000703 // disable CKEFIXON for DDR2
		li		t0, 0x00000707 // disable CKEFIXON for DDR2 //CML_20130104, for DDR2 performance
		sw		t0, 0xe4(k0)
		nop
88:
		li		t0, 0x80000000 // enable DRAM address decode by DRAMC //CML_20130104, for DDR2 performance
		sw		t0, 0x1e0(k0)
		nop
		
		j			99f
		nop
3:
		// DDR3
		DLY_CLK_CYCLE(2) // tMRD (4nCK by JEDEC spec, clock DRAMC:DRAM=1:2)
		
		li		t0, 0x00000000 // disable MRW
		sw		t0, 0x1e4(k0)
		nop
		
		// judge FPGA or ASIC platform
		bnez	s0, 1f // FPGA
		nop
		li		t0, 0x00002000 // set MR1 for DDR3
		j 88f
		nop	
1:
		li		t0, 0x00002001 // FPGA disable DLL, set MR1 for DDR3
88:
		sw		t0, 0x88(k0)
		nop

		li		t0, 0x00000001 // enable MRW
		sw		t0, 0x1e4(k0)
		nop
		DLY_CLK_CYCLE(2) // tMRD (4nCK by JEDEC spec, clock DRAMC:DRAM=1:2)
		
		li		t0, 0x00000000 // disable MRW
		sw		t0, 0x1e4(k0)
		nop
		
		//li		t0, 0x00000941 // set MR0, BC4 on the fly, CL=8, DLL reset, WR=8 for DDR3 //cml_dbg 1206
		//sw		t0, 0x88(k0)
		//nop
		// judge FPGA or ASIC platform //cml_dbg 1206
		bnez	s0, 1f // FPGA
		nop
		li		t0, 0x00000941 // set MR0, BC4 on the fly, CL=8, DLL reset, WR=8 for DDR3 //cml_dbg 1206
		j 88f
		nop	
1:
		li		t0, 0x00000911 // set MR0, BC4 on the fly, CL=8, DLL reset, WR=8 for DDR3 //cml_dbg 1206
88:
		sw		t0, 0x88(k0)
		nop
		
		li		t0, 0x00000001 // enable MRW
		sw		t0, 0x1e4(k0)
		nop
		DLY_CLK_CYCLE(6) // tMRD (12nCK by JEDEC spec, clock DRAMC:DRAM=1:2)
		
		li		t0, 0x00000000 // disable MRW
		sw		t0, 0x1e4(k0)
		nop
		li		t0, 0x00000400 // set ZQCL for DDR3
		sw		t0, 0x88(k0)
		nop
		li		t0, 0x00000010 // issue ZQCL for DDR3
		sw		t0, 0x1e4(k0)
		nop
		DLY_CLK_CYCLE(256) // tZQinit (512nCK by JEDEC spec, clock DRAMC:DRAM=1:2)
		
		li		t0, 0x00000000 // clear ZQCL
		sw		t0, 0x1e4(k0)
		nop
		li		t0, 0x00001100 // ??
		sw		t0, 0x1e4(k0)
		nop
		DLY_CLK_CYCLE(2) // tMRD (4nCK by JEDEC spec, clock DRAMC:DRAM=1:2)

		#if 0 //cml_dbg 1206
		li		t0, 0x000007a3 // disable CKEFIXON for DDR3
		sw		t0, 0xe4(k0)
		nop
		#else
		li		t0, 0x00000002 // disable CKEFIXON for DDR3
		sw		t0, 0xe4(k0)
		nop
		DLY_NS(400, CLK_CYCLE_NS) // 400ns
		li		t0, 0x000007a3 // disable CKEFIXON for DDR3
		sw		t0, 0xe4(k0)
		nop
		#endif
	
		// judge FPGA or ASIC platform
		bnez	s0, 1f // FPGA
		nop
		li		t0, 0x8c000000 // enable R_DMWDATREG0 & R_DMFASTOE(WLAT0=0) for DDR3
		j 88f
		nop	
1:
		li		t0, 0x88000000 // enable R_DMWDATREG0 & R_DMFASTOE(WLAT0=0) for DDR3 FPGA
88:
		sw		t0, 0x1e0(k0)
		nop	
		
99:

		li		t0, 0x0000ffff // set all DRAM addresses to 1
		sw		t0, 0x88(k0)
		nop
		
		li		t0, 0x00000020 // issue NOP to test command
		sw		t0, 0x1e4(k0)
		nop
		
		// judge DDR2 or DDR3
		li		t0, DDR3
		beq		s1, t0, 3f	// DDR3 type
		nop
		
		// DDR2
		DLY_CLK_CYCLE(1) // tMRD (2nCK by JEDEC spec, clock DRAMC:DRAM=1:2)
		
		j 99f
		nop
3:
		// DDR3
		DLY_CLK_CYCLE(2) // tMRD (4nCK by JEDEC spec, clock DRAMC:DRAM=1:2)
		
99:
		li		t0, 0x00000000 // clear NOP
		sw		t0, 0x1e4(k0)
		nop
		
		li		t0, 0x10622842 // disable R_DMMIOCKCTRLOFF for non-stop DRAM clock
		sw		t0, 0x1dc(k0)
		nop

		//li		t0, 0xf07486e2 // enable R_DMCKEON
		//sw		t0, 0x4(k0)
		//nop
		// judge DDR2 or DDR3
		li		t0, DDR3
		beq		s1, t0, 3f	// DDR3 type
		nop
		
		// DDR2		
		// judge FPGA or ASIC platform
		bnez	s0, 1f // FPGA
		nop
		li		t0, 0xf07486e2 // enable R_DMCKEON for DDR2
		j 88f
		nop	
1:
		//li		t0, 0xf07486a2 // enable R_DMCKEON for DDR2 FPGA
		li		t0, 0xf00406a2 // enable R_DMCKEON for DDR2 FPGA //CML_20130104, for DDR2 performance
88:
		j 99f
		nop
3:
		// judge FPGA or ASIC platform
		bnez	s0, 1f // FPGA
		nop
		li		t0, 0xf07486e2 // enable R_DMCKEON for DDR3
		j 88f
		nop
1:				
		//li		t0, 0xf07482e2 // enable R_DMCKEON for DDR3 FPGA
		li		t0, 0xf07486e2 // enable R_DMCKEON for DDR3 FPGA cml_dbg 1213
88:
99:

		// judge DRAM type & size to decide column address bit number
		li		t1, 0xfffffcff
		and		t0,	t0, t1 // clear column address width
		
		li		t1, 0x1
		beq		s1, t1, 10f	//	DDR3 type, all DDR3 column address width is 10 bit
		nop
		li		t1, 0x3
		bne		s1, t1, 9f	//	not DDR2 & DDR3 type, set column address width as 9 bit
		nop
		li		t1,	0x4
		bltu	s2, t1, 9f	// DDR2 type, size < 64 MB, set column address width as 9 bit; else 10 bit
		nop
10:
		ori		t0, 0x00000200 // 10-bit
		j			66f
		nop
9:
		ori		t0, 0x00000100 // 9-bit
66:
		sw		t0, 0x4(k0)
		nop
		
		/* *** set others *** */
		DLY_CLK_CYCLE(REG_RW_DELAY)
		
		li		t0, 0x00000000 // set CS delay
		sw		t0, 0xc(k0)
		nop
		
		// judge DDR2 or DDR3
		li		t0, DDR3
		beq		s1, t0, 3f	// DDR3 type
		nop
		
		// DDR2
		// judge FPGA or ASIC platform
		bnez	s0, 1f // FPGA
		nop
		li		t0, 0x55d84508 // set AC timing for DDR2
		sw		t0, 0x0(k0)
		nop
		
		DLY_CLK_CYCLE(REG_RW_DELAY)
		
		li		t0, 0xa8000401 // set tRFC for DDR2
		sw		t0, 0x44(k0)
		nop
		
		li		t0, 0x00000650 // set AC timing for DDR2
		sw		t0, 0x1e8(k0)
		nop
		
		li		t0, 0x00407361 // set REFCNT for DDR2
		sw		t0, 0x8(k0)
		nop
		
		j 88f
		nop	
1:
		//li		t0, 0x22884486 // set AC timing for DDR2 FPGA
		li		t0, 0x00074400 // set AC timing for DDR2 FPGA //CML_20130104, for DDR2 performance
		sw		t0, 0x0(k0)
		nop
		
		DLY_CLK_CYCLE(REG_RW_DELAY)
		
		//li		t0, 0xa8070401 // set tRFC for DDR2 FPGA
		li		t0, 0xa8000401 // set tRFC for DDR2 FPGA //CML_20130121, for improving performance
		sw		t0, 0x44(k0)
		nop
		
		//li		t0, 0x00000630 // set AC timing for DDR2 FPGA
		li		t0, 0x00000600 // set AC timing for DDR2 FPGA //CML_20130121, change tRFC for improving performance
		sw		t0, 0x1e8(k0)
		nop
		
		//li		t0, 0x0040730F // set REFCNT for DDR2 FPGA
		li		t0, 0x0040430f // set REFCNT for DDR2 FPGA //CML_20130104, for DDR2 performance
		sw		t0, 0x8(k0)
		nop
88:
		
		j 99f
		nop
3:
		// DDR3
		// judge FPGA or ASIC platform
		bnez	s0, 1f // FPGA
		nop
		
		li		t0, 0x55fb46eb // set AC timing for DDR3
		sw		t0, 0x0(k0)
		nop
		DLY_CLK_CYCLE(REG_RW_DELAY)
		li		t0, 0xa88d0401 // set tRFC for DDR3
		sw		t0, 0x44(k0)
		nop
		li		t0, 0x00000660 // set AC timing for DDR3
		sw		t0, 0x1e8(k0)
		nop
		li		t0, 0x00047930 // set REFCNT for DDR3
		sw		t0, 0x8(k0)
		nop
		
		j 88f
		nop
1:
		li		t0, 0x44ea45b9 // set AC timing for DDR3
		sw		t0, 0x0(k0)
		nop
		DLY_CLK_CYCLE(REG_RW_DELAY)
		//li		t0, 0xa8830401 // set tRFC for DDR3
		li		t0, 0xa8800401 // set tRFC for DDR3 //CML_20130121, for improving performance
		sw		t0, 0x44(k0)
		nop
		//li		t0, 0x00000610 // set AC timing for DDR3
		li		t0, 0x00000600 // set AC timing for DDR3 //CML_20130121, change tRFC for improving performance
		sw		t0, 0x1e8(k0)
		nop
		li		t0, 0x00046907 // set REFCNT for DDR3
		sw		t0, 0x8(k0)
		nop
88:
99:
		li		t0, 0x00000000 // set DQM delay
		sw		t0, 0x10(k0)
		nop
		li		t0, 0xedcb000f // set DRAM pin output enable
		sw		t0, 0xf8(k0)
		nop
		li		t0, 0x27010000 // set tXP
		sw		t0, 0xfc(k0)
		nop
		
		// judge DDR2 or DDR3
		li		t0, DDR3
		beq		s1, t0, 3f	// DDR3 type
		nop
		
		// DDR2
		li		t0, 0x00c80008
		sw		t0, 0x1d8(k0)
		nop

		// judge FPGA or ASIC platform
		bnez	s0, 1f // FPGA
		nop
		
		//li		t0, 0x00c80008 // set bus monitor for DDR2
		//sw		t0, 0x1d8(k0)
		//nop
		
		j 88f
		nop	
1:
		li		t0, 0x00c8000c // cml_dbg 1121
		sw		t0, 0x1d8(k0)
		nop
88:

		j 99f
		nop
3:
		li		t0, 0x00c80008 // set bus monitor for DDR3
		sw		t0, 0x1d8(k0)
		nop
99:

		#if 0
		li		t0, 0x00051100 // set rank size as 0, cml_dbg
		sw		t0, 0x110(k0)
		nop
		#endif
		
		



		/* *** set DRAM page size and enable DRAM controller *** */
		la		k0,	CR_ARB_BASE
		lw		t0, 0x74(k0)
		nop
		li		t1, 0xfffffef8
		and		t0, t0, t1 // 0x74[9:8]: DRAMC sel cmd pipeline; 0x74[2]: DRAMC enable; 0x74[1:0] : page size
		
		li		t1, DDR3
		beq		s1, t1, 2f	//	DDR3 type, all DDR3 page size is 2KB
		nop
		li		t1, DDR2
		bne		s1, t1, 1f	//	not DDR2 & DDR3 type, set page size as 1KB
		nop
		li		t1, 0x3
		bgtu	s2, t1, 2f	// DDR2 type, size > 32 MB,  set page size as 2KB; else 1KB
		nop
1:
		//ori		t0, 0x00000004 // page size 1KB and enable DRAMC //CML_20130125
		ori		t0, 0x00000104 // page size 1KB and enable DRAMC(0x74[2]=1), use read cmd pipeline (0x74[9:8]=2'h1)
		j			55f
		nop
2:
		//ori		t0, 0x00000005 // page size 2KB and enable DRAMC //CML_20130125
		ori		t0, 0x00000105 // page size 2KB and enable DRAMC(0x74[2]=1), use read cmd pipeline (0x74[9:8]=2'h1)
55:
		sw		t0, 0x74(k0)
		nop
		DLY_CLK_CYCLE(150)
		
		
#if 0
		/* *** enable DRAM controller *** */
	  la		k0,	CR_ARB_BASE
		lw		t0, 0x74(k0)
		nop
		ori		t0, t0, 0x04
		sw		t0, 0x74(k0)
		nop
		DLY_CLK_CYCLE(150)
#endif

mtk_dmc_done:
		jr		ra
		nop

END(sys_init_mtk_dmc)

//CML_mt7510_done
#endif
#endif


#ifdef MT7510_DCACHE_TEST
#define R_C0_DataLo	      	28
#define R_C0_SelDataLo	    1

#define R_C0_TagLo			28
#define R_C0_SelTagLo		0


#define DCT_STARTADDR	0x80020000 	/* dcache test start addr */
#define DCT_SIZE			0x10000		/* dcache test size: 64KB */
#define DCT_LINESIZE		0x20		/* dcache test line size: 32B */

#if 1
//==============================================================							 			
// DCacheTag_test
//==============================================================
/* 	test dcache tag array. 
	1. round-1:	
	  - write all tag array to 0xFFFFFCE0
	  - read value from dcache tag array and compare with 0xFFFFFCE0 (skip R, P bits)
	  - if any not match, then fail; 
	  - else continue round-2
	2. round-2: 
	  - write all tag array to 0x0
	  - read value from dcache tag array and compare with 0x0
	  - if any not match, then fail; 
	  - else pass
*/
LEAF(DCacheTag_test)
			.set  noreorder

			li	t0, 	0x2 //round
			li 	t1, 	0xFFFFFCE0	/*value write to tags, skip R, P bits */
			li 	t2,	0x0			/* values read from taglo, use it to compare with t1 */
			li  	v0, 1			/* init return value fail */
			
			/* write patterns to tag array */
dct_test_start:
			MTC0_SEL_OPCODE( R_t1, R_C0_TagLo, R_C0_SelTagLo )
			/* Calc an address that will correspond to the first cache line */
			li    a2, DCT_STARTADDR
								
			/* Calc an address that will correspond to the last cache line */
			addu  a3, a2, DCT_SIZE
			//subu  a3, a3, DCT_LINESIZE
															
			/* Loop through all lines, set each tag to 0 */
1:
			cache DCACHE_INDEX_STORE_TAG, 0(a2)
			addu  a2, a2, DCT_LINESIZE
			bne   a2, a3, 1b
			nop			

			/* check the whole d-cache tag array*/
			/* Calc an address that will correspond to the first cache line */
			li    a2, DCT_STARTADDR
								
			/* Calc an address that will correspond to the last cache line */
			addu  a3, a2, DCT_SIZE
			//subu  a3, a3, DCT_LINESIZE
			
1:			/* Loop through all lines, invalidating each of them */
			cache DCACHE_INDEX_LOAD_TAG, 0(a2)
			/* check tag array */
			//mfc0 t0, 	R_C0_TagLo, R_C0_SelTagLo
			li	t2, 0x0
			MFC0_SEL_OPCODE( R_t2, R_C0_TagLo, R_C0_SelTagLo )
			bne	  t2, t1, dct_test_fail /* if (t1!=t2) goto fail */
			nop
			addu  a2, a2, DCT_LINESIZE	
			bne   a2, a3, 1b
			nop

			li 	t1, 0x0 /* set round-2 t1 to 0 */
			subu		t0,	t0, 	1 /* round--*/
			bnez		t0, 	dct_test_start	 	
			nop
		
dct_test_pass:
			li 	v0, 0
			j	dct_test_done
			nop
dct_test_fail:
			li v0, 1
dct_test_done:
			jr    ra
			nop

			.set  reorder
END(DCacheTag_test)
#endif

#if 1
//==============================================================							 			
// DCacheData_test
//==============================================================
/* 	test dcache data array. 
	1. write 16KB uncached SDRAM to 0
	2. valide all cache line mapping to the whole 16KB SDRAM 
	3. write all 16KB cached SDRAM to 0xFFFFFFFF (NOTE: new data should only keep in cache)
	4. read all data from uncached SDRAM and compared with 0xFFFFFFFF, if mach, then fail, else pass
*/
LEAF(DCacheData_test)
			.set  noreorder
#define RA              		t4
			move RA, ra

			li  	v0, 1			/* init return value fail */

			/* init all 16KB SDRAM to 0*/
			li	a2, 	DCT_STARTADDR
			li 	t0, 	0xA0000000
			or 	a2, 	a2, 	t0		/*change to uncached address */
			addu  a3, a2, DCT_SIZE
1:
			sw zero, (a2)
			addu a2, a2, 4
			bne   a2, a3, 1b
			nop				

			li    	a2, 	DCT_STARTADDR		
			addu  a3, a2, DCT_SIZE
			li 	t0, 	0xFFFFFC00
1:			/* valide all cache line mapping to the SDRAM */
			and 	t1, 	a2, 	t0 /* move PA to t1 */
			ori 	t1, 	t1,	0x80		/* valide PA */
			MTC0_SEL_OPCODE( R_t1, R_C0_TagLo, R_C0_SelTagLo )
			cache DCACHE_INDEX_STORE_TAG, 0(a2)
			addu  a2, a2, DCT_LINESIZE
			bne   a2, a3, 1b
			nop		

			li    	a2, 	DCT_STARTADDR
			addu  a3, a2, DCT_SIZE
			li 	t1, 	0xFFFFFFFF			
1:			/* write all 16KB cache to 0xFFFFFFFF */
			sw 	t1, 	(a2)
			addu a2, 	a2, 	4
			bne   a2, 	a3, 	1b
			nop

			/* read all data from uncached sdram and compared with 0xFFFFFFFF, if mach, then fail*/
			li	a2, 	DCT_STARTADDR
			li 	t0, 	0xA0000000
			or 	a2, 	a2, 	t0		/*change to uncached address */
			addu  a3, a2, DCT_SIZE
			li 	t1, 	0xFFFFFFFF
1:
			lw	t2, 	(a2)
			nop
			beq	t2, 	t1, 	dcd_test_fail /* if (t1==t2) goto fail */
			nop				
			addu a2, a2, 4
			bne   a2, a3, 1b
			nop

			/*write back the cache data*/
			/*DCACHE_ADDR_HIT_WRITEBACK*/
			li    	a2, 	DCT_STARTADDR		
			addu  a3, a2, DCT_SIZE			
1:			/* write back all cache line to the SDRAM */
			cache DCACHE_ADDR_HIT_WRITEBACK, 0(a2)
			addu  a2, a2, DCT_LINESIZE
			bne   a2, a3, 1b
			nop

			/* read all data from uncached sdram and compared with 0xFFFFFFFF, if not mach, then fail*/
			li	a2, 	DCT_STARTADDR
			li 	t0, 	0xA0000000
			or 	a2, 	a2, 	t0		/*change to uncached address */
			addu  a3, a2, DCT_SIZE
			li 	t1, 	0xFFFFFFFF
1:
			lw	t2, 	(a2)
			nop
			bne	t2, 	t1, 	dcd_test_fail /* if (t1==t2) goto fail */
			nop				
			addu a2, a2, 4
			bne   a2, a3, 1b
			nop

			/* init all 64KB uncache SDRAM to 0 again*/
			li	a2, 	DCT_STARTADDR
			li 	t0, 	0xA0000000
			or 	a2, 	a2, 	t0		/*change to uncached address */
			addu  a3, a2, DCT_SIZE
1:
			sw zero, (a2)
			addu a2, a2, 4
			bne   a2, a3, 1b
			nop

			/*invalide the cache data*/
			/*DCACHE_ADDR_HIT_INVALIDATE*/
			li    	a2, 	DCT_STARTADDR		
			addu  a3, a2, DCT_SIZE			
1:			/* invalide the cache data*/
			cache DCACHE_ADDR_HIT_INVALIDATE, 0(a2)
			addu  a2, a2, DCT_LINESIZE
			bne   a2, a3, 1b
			nop

			/* read all data from uncached sdram and compared with 0x0, if not mach, then fail*/
			li	a2, 	DCT_STARTADDR
			li 	t0, 	0xA0000000
			or 	a2, 	a2, 	t0		/*change to uncached address */
			addu  a3, a2, DCT_SIZE
			li 	t1, 	0x0
1:
			lw	t2, 	(a2)
			nop
			bne	t2, 	t1, 	dcd_test_fail /* if (t1==t2) goto fail */
			nop				
			addu a2, a2, 4
			bne   a2, a3, 1b
			nop
			

dcd_test_pass:
			li 	v0, 0
			j	dcd_test_done
			nop
dcd_test_fail:
			li v0, 1
dcd_test_done:
			//jr    ra
			jr RA
			nop

			.set  reorder
END(DCacheData_test)
#endif

/*************************
 * GPIO Module Registers *
 *************************/
#define CR_GPIO_BASE       	0xBFBF0200
#define CR_GPIO_CTRL	    (CR_GPIO_BASE + 0x00)
#define CR_GPIO_DATA	    (CR_GPIO_BASE + 0x04)
#define CR_GPIO_INTS      	(CR_GPIO_BASE + 0x08)
#define CR_GPIO_EDET	    (CR_GPIO_BASE + 0x0C)
#define CR_GPIO_LDET       	(CR_GPIO_BASE + 0x10)
#define CR_GPIO_ODRAIN      (CR_GPIO_BASE + 0x14)
#define CR_GPIO_CTRL1	    (CR_GPIO_BASE + 0x20)

LEAF(turnon_gpio)
		/*configure gpio as ouput*/
		bgt	a0, 15, 2f
		nop
		la      t0, CR_GPIO_CTRL
		b	3f
		nop

2:
		la      t0, CR_GPIO_CTRL1
3:
		lw      t1, 0x0(t0)
		nop
		sll	t2, a0, 1
		li	t3, 1
		sll	t2, t3, t2
		or	t1, t1, t2
		sw	t1, 0x0(t0)
		nop

		/*set it as open drain*/
		la      t0, CR_GPIO_ODRAIN
		lw      t1, 0x0(t0)
		nop
		li  t2, 1
		sll	t2, t2, a0
		or	t1, t1, t2
		sw	t1, 0x0(t0)
		nop

		/*set output high/low*/
		la      t0, CR_GPIO_DATA
		lw      t1, 0x0(t0)
		nop

		li 	t2, 1
		sll	t2, t2, a0

		beq		a1, 1,  set_high
		nop
		
		/*output is low*/
		not	t2, t2		
		and	t1, t1, t2
		b	1f
		nop

set_high:
		/*output is high*/
		or	t1, t1, t2

1:
		sw	t1, 0x0(t0)
		nop

		jr	ra
		nop


END(turnon_gpio)


#endif /* MT7510_CACHE_TEST */

#endif
#endif /*DDR_CALI_NAND*/

#ifdef DEBUG
LEAF(sys_uart_init)
            la          k0,     0xbfbf0000

//              DISPLAYLED(0xf7, 0x10000)

                /*
                        VPchar(CR_UART_LCR)=UART_BRD_ACCESS;
                */
                li              k1, 0x80
                sb              k1, 0x0f(k0)


//              DISPLAYLED(0xdf, 0x10000)

                /*
                        VPint(CR_UART_XYD)=word;
                */
                la              k1, 0xea00fde8
                sw              k1, 0x2c(k0)
                nop


//              DISPLAYLED(0xfd, 0x10000)

                /*
                        VPchar(CR_UART_BRDL)=UART_BRDL;
                */
                li              k1, 0x01
                sb              k1, 0x03(k0)


//              DISPLAYLED(0xbf, 0x10000)

                /*
                        VPchar(CR_UART_BRDH)=UART_BRDH;
                */
                li              k1, 0x00
                sb              k1, 0x07(k0)

//              DISPLAYLED(0x7f, 0x10000)

                /*
                        VPchar(CR_UART_LCR)=UART_LCR;
                */
                li              k1, 0x03
                sb              k1, 0x0f(k0)


//              DISPLAYLED(0xff, 0x10000)

                /*
                        VPchar(CR_UART_FCR)=UART_FCR|UART_WATERMARK;
                */
                li              k1, 0x0f
                sb              k1, 0x0b(k0)

//              DISPLAYLED(0x00, 0x10000)

                /*
                        VPchar(CR_UART_MCR)=UART_MCR;
                */                                                              
                li              k1, 0x00
                sb              k1, 0x13(k0)

//              DISPLAYLED(0xf7, 0x10000)

                /*
                        VPchar(CR_UART_MISCC)=UART_MISCC;
                */                                                              
                li              k1, 0x00
                sb              k1, 0x27(k0)

//              DISPLAYLED(0xdf, 0x10000)

                /*
                        VPchar(CR_UART_IER)=UART_IER;
                */                                                              
                li              k1, 0x01
                sb              k1, 0x07(k0)

//              DISPLAYLED(0xfd, 0x10000)

                /*
                        VPint(CR_INTC_IMR)|= (0x01 << intSource);
                        UART_INT
                */
                la              t0, 0xbfb40000
                li              k1, 0x01
                sb              k1, 0x04(t0)
#if 1
                /*
                        test code
                */
                li              k1, 'h'
                sb              k1, 0x03(k0)
1:
                lb      t5, 0x17(k0)
                nop
                andi    t5, 0x20
                beqz     t5, 1b
                nop


                li              k1, 'e'
                sb              k1, 0x03(k0)
                //jal           check_uart_status
                //nop

2:
                lb    t5, 0x17(k0)
                nop
                andi    t5, 0x20
                beqz    t5, 2b
                nop


                li              k1, 'l'
                sb              k1, 0x03(k0)
                //jal           check_uart_status
                //nop
3:
                lb    t5, 0x17(k0)
                nop
                andi    t5, 0x20
                beqz    t5, 3b
                nop



                li              k1, 'l'
                sb              k1, 0x03(k0)
                //jal           check_uart_status
                //nop
4:
                lb      t5, 0x17(k0)
                nop
                andi    t5, 0x20
                beqz    t5, 4b
                nop

                li              k1, 'o'
                sb              k1, 0x03(k0)
                //jal           check_uart_status
                //nop
5:
                lb    t5, 0x17(k0)
                nop
                andi    t5, 0x20
                beqz    t5, 5b
                nop
#endif
//              DISPLAYLED(0xbf, 0x10000)

                jr    ra
                nop

END(sys_uart_init)

LEAF(disp_character)
                la              k0,     0xbfbf0000
wait:
                lb              t5, 0x17(k0)
                nop
                andi            t5, 0x20
                beqz    t5, wait
                nop

                sb              t7, 0x03(k0)

                jr    ra
                nop
END(disp_character)
#endif
