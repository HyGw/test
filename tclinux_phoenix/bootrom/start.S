        
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

//#define UNCACHE_MODE
//#define DEBUG 1

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
					 
        .text
        .set noreorder
		.globl __start              
__start:
		j	boot_start
		nop

header_start:
		nop
		nop
		nop
		nop
		nop						// for boot_start address
		nop						// for boot_end	address
		nop						// for lzma_start address
		nop						// for lzma_end address
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
#ifdef BACKDOOR_SWITCH  /* EN7512, for Internal Bootrom Bypass function */
		nop	/*If this byte value != ( 0x0 or 0xff ), then bypass function enable  */
		nop						
		nop						
		nop						
		nop						
		nop						
		nop						
		nop						
#endif
boot_start:
#ifdef DEBUG

		jal sys_uart_init
                nop


                li t7, 'x'
                jal disp_character
                nop
#endif
#ifndef DDR_CALI_NAND

		/* Clear watch registers */
		mtc0	zero, CP0_WATCHLO
		mtc0	zero, CP0_WATCHHI

		/* Check whether NMI happen */
		mfc0   	k0, CP0_STATUS
		srl		k1, k0, (S_StatusNMI)   
		andi		k1, 0x1
		beqz		k1, no_nmi
		nop

		la		k0,	0xbfb00244	/*NMI happen, read NMI handler address, then jump to it*/
		lw		k1, 0x0(k0)
		nop
		jr		k1
		nop
		
no_nmi:
		/* Disable interrupts and KSU field (power up issue) */
		mfc0   	k0, CP0_STATUS
		li		k1, ~(M_StatusIE | M_StatusKSU)
		and		k0, k1
		mtc0	k0, CP0_STATUS

		/* Setup cause register (clear cause code)  */
		li		k0, M_CauseIV
		mtc0	k0, CP0_CAUSE
#ifdef DEBUG
		li t7, 'a'
                jal disp_character
                nop

#endif


		/* Perform CPU specific initialisation */
		jal     sys_init_processor
		nop
#ifdef DEBUG
                li t7, 'c'
                jal disp_character
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

#define	SHARE_FEMEM_SEL	0xBFB00958
#define	SHARE_FEMEM_PA	0x1FA30000
#define	SHARE_FEMEM_VA	0x9FA30000
#define BOOTROM_START_ADDR	0xBFA30040
#define BOOTROM_END_ADDR	0xBFA3C020
#define SRAM_START_ADDR	  0xBFA30000

		//Enable pbus access FE memory
		la      v0, SHARE_FEMEM_SEL
		lw      t6, 0x0(v0)
		nop

		ori	t6, t6, 0x1
		sw	t6, 0x0(v0)
		nop


		la  k0, __move_data_start
		la  k1, (__move_data_end + 4)
		la  t1, BOOTROM_START_ADDR

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


#ifdef DEBUG
		    li t7, 'l'
          	jal disp_character          	
          	nop
#endif


		DISPLAYLED(0x5e, 0x20000)

#ifdef DEBUG
		li t7, 'f'
          	jal disp_character
          	nop

#endif

		la      t1, SRAM_START_ADDR
		
		la		t2, __boot2_start
		sw		t2, 0x0(t1)
		la		t2, __boot2_end
		sw		t2, 0x4(t1)
		la      t2, __spram_start
		sw		t2, 0x8(t1)
		la      t2, __spram_end
		sw		t2, 0xc(t1)

		la      t2, __lzma_start
		sw		t2, 0x10(t1)
		la      t2, __lzma_end
		sw		t2, 0x14(t1)
		la      t2, __boot_start
		sw		t2, 0x18(t1)
		la      t2, __boot_end
		sw		t2, 0x1c(t1)
		
		la		s1, __boot2_start
		la		s2, __boot2_end
		la		s3, 0xbfa30800
		la		s4, 0xbfa30800

  	la	k0, BOOTROM_START_ADDR
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
#if 0
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
#endif
