/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1994, 1995, 1996, 1997, 2000, 2001 by Ralf Baechle
 * Copyright (C) 2000 Silicon Graphics, Inc.
 * Modified for further R[236]000 support by Paul M. Antoine, 1996.
 * Kevin D. Kissell, kevink@mips.com and Carsten Langgaard, carstenl@mips.com
 * Copyright (C) 2000 MIPS Technologies, Inc.  All rights reserved.
 */
#ifndef _ASM_MIPSREGS_H
#define _ASM_MIPSREGS_H

#include <linux/linkage.h>

/*
 * The following macros are especially useful for __asm__
 * inline assembler.
 */
#ifndef __STR
#define __STR(x) #x
#endif
#ifndef STR
#define STR(x) __STR(x)
#endif

/*
 * Coprocessor 0 register names
 */
#define CP0_INDEX $0
#define CP0_RANDOM $1
#define CP0_ENTRYLO0 $2
#define CP0_ENTRYLO1 $3
#define CP0_CONF $3
#define CP0_CONTEXT $4
#define CP0_PAGEMASK $5
#define CP0_WIRED $6
#define CP0_INFO $7
#define CP0_BADVADDR $8
#define CP0_COUNT $9
#define CP0_ENTRYHI $10
#define CP0_COMPARE $11
#define CP0_STATUS $12
#define CP0_CAUSE $13
#define CP0_EPC $14
#define CP0_PRID $15
#define CP0_CONFIG $16
#define CP0_LLADDR $17
#define CP0_WATCHLO $18
#define CP0_WATCHHI $19
#define CP0_XCONTEXT $20
#define CP0_FRAMEMASK $21
#define CP0_DIAGNOSTIC $22
#define CP0_PERFORMANCE $25
#define CP0_ECC $26
#define CP0_CACHEERR $27
#define CP0_TAGLO $28
#define CP0_TAGHI $29
#define CP0_ERROREPC $30

/*
 * R4640/R4650 cp0 register names.  These registers are listed
 * here only for completeness; without MMU these CPUs are not useable
 * by Linux.  A future ELKS port might take make Linux run on them
 * though ...
 */
#define CP0_IBASE $0
#define CP0_IBOUND $1
#define CP0_DBASE $2
#define CP0_DBOUND $3
#define CP0_CALG $17
#define CP0_IWATCH $18
#define CP0_DWATCH $19

/* 
 * Coprocessor 0 Set 1 register names
 */
#define CP0_S1_DERRADDR0  $26
#define CP0_S1_DERRADDR1  $27
#define CP0_S1_INTCONTROL $20
/*
 * Coprocessor 1 (FPU) register names
 */
#define CP1_REVISION   $0
#define CP1_STATUS     $31

/*
 * FPU Status Register Values
 */
/*
 * Status Register Values
 */

#define FPU_CSR_FLUSH   0x01000000      /* flush denormalised results to 0 */
#define FPU_CSR_COND    0x00800000      /* $fcc0 */
#define FPU_CSR_COND0   0x00800000      /* $fcc0 */
#define FPU_CSR_COND1   0x02000000      /* $fcc1 */
#define FPU_CSR_COND2   0x04000000      /* $fcc2 */
#define FPU_CSR_COND3   0x08000000      /* $fcc3 */
#define FPU_CSR_COND4   0x10000000      /* $fcc4 */
#define FPU_CSR_COND5   0x20000000      /* $fcc5 */
#define FPU_CSR_COND6   0x40000000      /* $fcc6 */
#define FPU_CSR_COND7   0x80000000      /* $fcc7 */

/*
 * X the exception cause indicator
 * E the exception enable
 * S the sticky/flag bit
*/
#define FPU_CSR_ALL_X 0x0003f000
#define FPU_CSR_UNI_X   0x00020000
#define FPU_CSR_INV_X   0x00010000
#define FPU_CSR_DIV_X   0x00008000
#define FPU_CSR_OVF_X   0x00004000
#define FPU_CSR_UDF_X   0x00002000
#define FPU_CSR_INE_X   0x00001000

#define FPU_CSR_ALL_E   0x00000f80
#define FPU_CSR_INV_E   0x00000800
#define FPU_CSR_DIV_E   0x00000400
#define FPU_CSR_OVF_E   0x00000200
#define FPU_CSR_UDF_E   0x00000100
#define FPU_CSR_INE_E   0x00000080

#define FPU_CSR_ALL_S   0x0000007c
#define FPU_CSR_INV_S   0x00000040
#define FPU_CSR_DIV_S   0x00000020
#define FPU_CSR_OVF_S   0x00000010
#define FPU_CSR_UDF_S   0x00000008
#define FPU_CSR_INE_S   0x00000004

/* rounding mode */
#define FPU_CSR_RN      0x0     /* nearest */
#define FPU_CSR_RZ      0x1     /* towards zero */
#define FPU_CSR_RU      0x2     /* towards +Infinity */
#define FPU_CSR_RD      0x3     /* towards -Infinity */


/*
 * Values for PageMask register
 */
#include <linux/config.h>
#ifdef CONFIG_CPU_VR41XX
#define PM_1K   0x00000000
#define PM_4K   0x00001800
#define PM_16K  0x00007800
#define PM_64K  0x0001f800
#define PM_256K 0x0007f800
#else
#define PM_4K   0x00000000
#define PM_16K  0x00006000
#define PM_64K  0x0001e000
#define PM_256K 0x0007e000
#define PM_1M   0x001fe000
#define PM_4M   0x007fe000
#define PM_16M  0x01ffe000
#endif

/*
 * Values used for computation of new tlb entries
 */
#define PL_4K   12
#define PL_16K  14
#define PL_64K  16
#define PL_256K 18
#define PL_1M   20
#define PL_4M   22
#define PL_16M  24

/*
 * Macros to access the system control coprocessor
 */
#define read_32bit_cp0_register(source)                         \
({ int __res;                                                   \
        __asm__ __volatile__(                                   \
	".set\tpush\n\t"					\
	".set\treorder\n\t"					\
        "mfc0\t%0,"STR(source)"\n\t"                            \
	".set\tpop"						\
        : "=r" (__res));                                        \
        __res;})

#define read_32bit_cp0_set1_register(source)                    \
({ int __res;                                                   \
        __asm__ __volatile__(                                   \
	".set\tpush\n\t"					\
	".set\treorder\n\t"					\
        "cfc0\t%0,"STR(source)"\n\t"                            \
	".set\tpop"						\
        : "=r" (__res));                                        \
        __res;})

/*
 * For now use this only with interrupts disabled!
 */
#define read_64bit_cp0_register(source)                         \
({ int __res;                                                   \
        __asm__ __volatile__(                                   \
        ".set\tmips3\n\t"                                       \
        "dmfc0\t%0,"STR(source)"\n\t"                           \
        ".set\tmips0"                                           \
        : "=r" (__res));                                        \
        __res;})

#define write_32bit_cp0_register(register,value)                \
        __asm__ __volatile__(                                   \
        "mtc0\t%0,"STR(register)"\n\t"				\
	"nop"							\
        : : "r" (value));

#define write_32bit_cp0_set1_register(register,value)           \
        __asm__ __volatile__(                                   \
        "ctc0\t%0,"STR(register)"\n\t"				\
	"nop"							\
        : : "r" (value));

#define write_64bit_cp0_register(register,value)                \
        __asm__ __volatile__(                                   \
        ".set\tmips3\n\t"                                       \
        "dmtc0\t%0,"STR(register)"\n\t"                         \
        ".set\tmips0"                                           \
        : : "r" (value))

/* 
 * This should be changed when we get a compiler that support the MIPS32 ISA. 
 */
#define read_mips32_cp0_config1()                               \
({ int __res;                                                   \
        __asm__ __volatile__(                                   \
	".set\tnoreorder\n\t"                                   \
	".set\tnoat\n\t"                                        \
     	".word\t0x40018001\n\t"                                 \
	"move\t%0,$1\n\t"                                       \
	".set\tat\n\t"                                          \
	".set\treorder"                                         \
	:"=r" (__res));                                         \
        __res;})

/*
 * R4x00 interrupt enable / cause bits
 */
#define IE_SW0          (1<< 8)
#define IE_SW1          (1<< 9)
#define IE_IRQ0         (1<<10)
#define IE_IRQ1         (1<<11)
#define IE_IRQ2         (1<<12)
#define IE_IRQ3         (1<<13)
#define IE_IRQ4         (1<<14)
#define IE_IRQ5         (1<<15)

/*
 * R4x00 interrupt cause bits
 */
#define C_SW0           (1<< 8)
#define C_SW1           (1<< 9)
#define C_IRQ0          (1<<10)
#define C_IRQ1          (1<<11)
#define C_IRQ2          (1<<12)
#define C_IRQ3          (1<<13)
#define C_IRQ4          (1<<14)
#define C_IRQ5          (1<<15)

#ifndef _LANGUAGE_ASSEMBLY
/*
 * Manipulate the status register.
 * Mostly used to access the interrupt bits.
 */
#define __BUILD_SET_CP0(name,register)                          \
extern __inline__ unsigned int                                  \
set_cp0_##name(unsigned int set)				\
{                                                               \
	unsigned int res;                                       \
                                                                \
	res = read_32bit_cp0_register(register);                \
	res |= set;						\
	write_32bit_cp0_register(register, res);        	\
                                                                \
	return res;                                             \
}								\
								\
extern __inline__ unsigned int                                  \
clear_cp0_##name(unsigned int clear)				\
{                                                               \
	unsigned int res;                                       \
                                                                \
	res = read_32bit_cp0_register(register);                \
	res &= ~clear;						\
	write_32bit_cp0_register(register, res);		\
                                                                \
	return res;                                             \
}								\
								\
extern __inline__ unsigned int                                  \
change_cp0_##name(unsigned int change, unsigned int new)	\
{                                                               \
	unsigned int res;                                       \
                                                                \
	res = read_32bit_cp0_register(register);                \
	res &= ~change;                                         \
	res |= (new & change);                                  \
	if(change)                                              \
		write_32bit_cp0_register(register, res);        \
                                                                \
	return res;                                             \
}

__BUILD_SET_CP0(status,CP0_STATUS)
__BUILD_SET_CP0(cause,CP0_CAUSE)
__BUILD_SET_CP0(config,CP0_CONFIG)

#endif /* defined (_LANGUAGE_ASSEMBLY) */

/*
 * Bitfields in the R4xx0 cp0 status register
 */
#define ST0_IE			0x00000001
#define ST0_EXL			0x00000002
#define ST0_ERL			0x00000004
#define ST0_KSU			0x00000018
#  define KSU_USER		0x00000010
#  define KSU_SUPERVISOR	0x00000008
#  define KSU_KERNEL		0x00000000
#define ST0_UX			0x00000020
#define ST0_SX			0x00000040
#define ST0_KX 			0x00000080
#define ST0_DE			0x00010000
#define ST0_CE			0x00020000

/*
 * Bitfields in the R[23]000 cp0 status register.
 */
#define ST0_IEC                 0x00000001
#define ST0_KUC			0x00000002
#define ST0_IEP			0x00000004
#define ST0_KUP			0x00000008
#define ST0_IEO			0x00000010
#define ST0_KUO			0x00000020
/* bits 6 & 7 are reserved on R[23]000 */
#define ST0_ISC			0x00010000
#define ST0_SWC			0x00020000
#define ST0_CM			0x00080000

/*
 * Bits specific to the R4640/R4650
 */
#define ST0_UM                 (1   <<  4)
#define ST0_IL                 (1   << 23)
#define ST0_DL                 (1   << 24)

/*
 * Bitfields in the TX39 family CP0 Configuration Register 3
 */
#define TX39_CONF_ICS_SHIFT	19
#define TX39_CONF_ICS_MASK	0x00380000
#define TX39_CONF_ICS_1KB 	0x00000000
#define TX39_CONF_ICS_2KB 	0x00080000
#define TX39_CONF_ICS_4KB 	0x00100000
#define TX39_CONF_ICS_8KB 	0x00180000
#define TX39_CONF_ICS_16KB 	0x00200000

#define TX39_CONF_DCS_SHIFT	16
#define TX39_CONF_DCS_MASK	0x00070000
#define TX39_CONF_DCS_1KB 	0x00000000
#define TX39_CONF_DCS_2KB 	0x00010000
#define TX39_CONF_DCS_4KB 	0x00020000
#define TX39_CONF_DCS_8KB 	0x00030000
#define TX39_CONF_DCS_16KB 	0x00040000

#define TX39_CONF_CWFON 	0x00004000
#define TX39_CONF_WBON  	0x00002000
#define TX39_CONF_RF_SHIFT	10
#define TX39_CONF_RF_MASK	0x00000c00
#define TX39_CONF_DOZE		0x00000200
#define TX39_CONF_HALT		0x00000100
#define TX39_CONF_LOCK		0x00000080
#define TX39_CONF_ICE		0x00000020
#define TX39_CONF_DCE		0x00000010
#define TX39_CONF_IRSIZE_SHIFT	2
#define TX39_CONF_IRSIZE_MASK	0x0000000c
#define TX39_CONF_DRSIZE_SHIFT	0
#define TX39_CONF_DRSIZE_MASK	0x00000003

/*
 * Status register bits available in all MIPS CPUs.
 */
#define ST0_IM			0x0000ff00
#define  STATUSB_IP0		8
#define  STATUSF_IP0		(1   <<  8)
#define  STATUSB_IP1		9
#define  STATUSF_IP1		(1   <<  9)
#define  STATUSB_IP2		10
#define  STATUSF_IP2		(1   << 10)
#define  STATUSB_IP3		11
#define  STATUSF_IP3		(1   << 11)
#define  STATUSB_IP4		12
#define  STATUSF_IP4		(1   << 12)
#define  STATUSB_IP5		13
#define  STATUSF_IP5		(1   << 13)
#define  STATUSB_IP6		14
#define  STATUSF_IP6		(1   << 14)
#define  STATUSB_IP7		15
#define  STATUSF_IP7		(1   << 15)
#define  STATUSB_IP8		0
#define  STATUSF_IP8		(1   << 0)
#define  STATUSB_IP9		1
#define  STATUSF_IP9		(1   << 1)
#define  STATUSB_IP10		2
#define  STATUSF_IP10		(1   << 2)
#define  STATUSB_IP11		3
#define  STATUSF_IP11		(1   << 3)
#define  STATUSB_IP12		4
#define  STATUSF_IP12		(1   << 4)
#define  STATUSB_IP13		5
#define  STATUSF_IP13		(1   << 5)
#define  STATUSB_IP14		6
#define  STATUSF_IP14		(1   << 6)
#define  STATUSB_IP15		7
#define  STATUSF_IP15		(1   << 7)
#define ST0_CH			0x00040000
#define ST0_SR			0x00100000
#define ST0_BEV			0x00400000
#define ST0_RE			0x02000000
#define ST0_FR			0x04000000
#define ST0_CU			0xf0000000
#define ST0_CU0			0x10000000
#define ST0_CU1			0x20000000
#define ST0_CU2			0x40000000
#define ST0_CU3			0x80000000
#define ST0_XX			0x80000000	/* MIPS IV naming */

/*
 * Bitfields and bit numbers in the coprocessor 0 cause register.
 *
 * Refer to your MIPS R4xx0 manual, chapter 5 for explanation.
 */
#define  CAUSEB_EXCCODE		2
#define  CAUSEF_EXCCODE		(31  <<  2)
#define  CAUSEB_IP		8
#define  CAUSEF_IP		(255 <<  8)
#define  CAUSEB_IP0		8
#define  CAUSEF_IP0		(1   <<  8)
#define  CAUSEB_IP1		9
#define  CAUSEF_IP1		(1   <<  9)
#define  CAUSEB_IP2		10
#define  CAUSEF_IP2		(1   << 10)
#define  CAUSEB_IP3		11
#define  CAUSEF_IP3		(1   << 11)
#define  CAUSEB_IP4		12
#define  CAUSEF_IP4		(1   << 12)
#define  CAUSEB_IP5		13
#define  CAUSEF_IP5		(1   << 13)
#define  CAUSEB_IP6		14
#define  CAUSEF_IP6		(1   << 14)
#define  CAUSEB_IP7		15
#define  CAUSEF_IP7		(1   << 15)
#define  CAUSEB_IV		23
#define  CAUSEF_IV		(1   << 23)
#define  CAUSEB_CE		28
#define  CAUSEF_CE		(3   << 28)
#define  CAUSEB_BD		31
#define  CAUSEF_BD		(1   << 31)

/*
 * Bits in the coprozessor 0 config register.
 */
#define CONF_CM_CACHABLE_NO_WA		0
#define CONF_CM_CACHABLE_WA		1
#define CONF_CM_UNCACHED		2
#define CONF_CM_CACHABLE_NONCOHERENT	3
#define CONF_CM_CACHABLE_CE		4
#define CONF_CM_CACHABLE_COW		5
#define CONF_CM_CACHABLE_CUW		6
#define CONF_CM_CACHABLE_ACCELERATED	7
#define CONF_CM_CMASK			7
#define CONF_DB				(1 <<  4)
#define CONF_IB				(1 <<  5)
#define CONF_SC				(1 << 17)
#define CONF_AC                         (1 << 23)
#define CONF_HALT                       (1 << 25)

/*
 * R10000 performance counter definitions.
 *
 * FIXME: The R10000 performance counter opens a nice way to implement CPU
 *        time accounting with a precission of one cycle.  I don't have
 *        R10000 silicon but just a manual, so ...
 */

/*
 * Events counted by counter #0
 */
#define CE0_CYCLES			0
#define CE0_INSN_ISSUED			1
#define CE0_LPSC_ISSUED			2
#define CE0_S_ISSUED			3
#define CE0_SC_ISSUED			4
#define CE0_SC_FAILED			5
#define CE0_BRANCH_DECODED		6
#define CE0_QW_WB_SECONDARY		7
#define CE0_CORRECTED_ECC_ERRORS	8
#define CE0_ICACHE_MISSES		9
#define CE0_SCACHE_I_MISSES		10
#define CE0_SCACHE_I_WAY_MISSPREDICTED	11
#define CE0_EXT_INTERVENTIONS_REQ	12
#define CE0_EXT_INVALIDATE_REQ		13
#define CE0_VIRTUAL_COHERENCY_COND	14
#define CE0_INSN_GRADUATED		15

/*
 * Events counted by counter #1
 */
#define CE1_CYCLES			0
#define CE1_INSN_GRADUATED		1
#define CE1_LPSC_GRADUATED		2
#define CE1_S_GRADUATED			3
#define CE1_SC_GRADUATED		4
#define CE1_FP_INSN_GRADUATED		5
#define CE1_QW_WB_PRIMARY		6
#define CE1_TLB_REFILL			7
#define CE1_BRANCH_MISSPREDICTED	8
#define CE1_DCACHE_MISS			9
#define CE1_SCACHE_D_MISSES		10
#define CE1_SCACHE_D_WAY_MISSPREDICTED	11
#define CE1_EXT_INTERVENTION_HITS	12
#define CE1_EXT_INVALIDATE_REQ		13
#define CE1_SP_HINT_TO_CEXCL_SC_BLOCKS	14
#define CE1_SP_HINT_TO_SHARED_SC_BLOCKS	15

/*
 * These flags define in which priviledge mode the counters count events
 */
#define CEB_USER	8	/* Count events in user mode, EXL = ERL = 0 */
#define CEB_SUPERVISOR	4	/* Count events in supvervisor mode EXL = ERL = 0 */
#define CEB_KERNEL	2	/* Count events in kernel mode EXL = ERL = 0 */
#define CEB_EXL		1	/* Count events with EXL = 1, ERL = 0 */

#ifdef TC3262
/*
 ************************************************************************
 *               S T A T U S   R E G I S T E R   ( 1 2 )                *
 ************************************************************************
 * 	
 *  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
 *  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |C|C|C|C|R|F|R|M|P|B|T|S|M| | R |I|I|I|I|I|I|I|I|K|S|U|U|R|E|E|I|
 * |U|U|U|U|P|R|E|X|X|E|S|R|M| | s |M|M|M|M|M|M|M|M|X|X|X|M|s|R|X|E| Status
 * |3|2|1|0| | | | | |V| | |I| | v |7|6|5|4|3|2|1|0| | | | |v|L|L| |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

#define C0_Status		$12
#define R_C0_Status		12
#define R_C0_SelStatus		0
#define C0_SR			C0_Status		/* OBSOLETE - DO NOT USE IN NEW CODE */

#define S_StatusCU		28			/* Coprocessor enable (R/W) */
#define M_StatusCU		(0xf << S_StatusCU)
#define S_StatusCU3		31 			/* No longer used in Release 2 */
#define M_StatusCU3     	(0x1 << S_StatusCU3)
#define S_StatusCU2		30
#define M_StatusCU2		(0x1 << S_StatusCU2)
#define S_StatusCU1		29
#define M_StatusCU1		(0x1 << S_StatusCU1)
#define S_StatusCU0		28
#define M_StatusCU0		(0x1 << S_StatusCU0)
#define S_StatusRP		27			/* Enable reduced power mode (R/W) */
#define M_StatusRP		(0x1 << S_StatusRP)
#define S_StatusFR		26			/* Enable 64-bit FPRs (R/W) */
#define M_StatusFR		(0x1 << S_StatusFR)
#define S_StatusRE		25			/* Enable reverse endian (R/W) */
#define M_StatusRE		(0x1 << S_StatusRE)
#define S_StatusMX		24			/* Enable access to MDMX and DSP ASE's (R/W) */
#define M_StatusMX		(0x1 << S_StatusMX)
#define S_StatusPX		23			/* Enable access to 64-bit instructions/data (MIPS64 only) (R/W) */
#define M_StatusPX		(0x1 << S_StatusPX)
#define S_StatusBEV		22			/* Enable Boot Exception Vectors (R/W) */
#define M_StatusBEV		(0x1 << S_StatusBEV)
#define S_StatusTS		21			/* Denote TLB shutdown (R/W) */
#define M_StatusTS		(0x1 << S_StatusTS)
#define S_StatusSR		20			/* Denote soft reset (R/W) */
#define M_StatusSR		(0x1 << S_StatusSR)
#define S_StatusNMI		19
#define M_StatusNMI		(0x1 << S_StatusNMI)	/* Denote NMI (R/W) */
#define S_StatusIM		8			/* Interrupt mask (R/W) */
#define M_StatusIM		(0xff << S_StatusIM)
#define S_StatusIM7		15
#define M_StatusIM7		(0x1 << S_StatusIM7)
#define S_StatusIM6		14
#define M_StatusIM6		(0x1 << S_StatusIM6)
#define S_StatusIM5		13
#define M_StatusIM5		(0x1 << S_StatusIM5)
#define S_StatusIM4		12
#define M_StatusIM4		(0x1 << S_StatusIM4)
#define S_StatusIM3		11
#define M_StatusIM3		(0x1 << S_StatusIM3)
#define S_StatusIM2		10
#define M_StatusIM2		(0x1 << S_StatusIM2)
#define S_StatusIM1		9
#define M_StatusIM1		(0x1 << S_StatusIM1)
#define S_StatusIM0		8
#define M_StatusIM0		(0x1 << S_StatusIM0)
#define S_StatusIPL     10
#define M_StatusIPL     (0x3f << S_StatusIPL)
#define S_StatusKX		7			/* Enable access to extended kernel addresses (MIPS64 only) (R/W) */
#define M_StatusKX		(0x1 << S_StatusKX)
#define S_StatusSX		6			/* Enable access to extended supervisor addresses (MIPS64 only) (R/W) */
#define M_StatusSX		(0x1 << S_StatusSX)
#define S_StatusUX		5			/* Enable access to extended user addresses (MIPS64 only) (R/W) */
#define M_StatusUX		(0x1 << S_StatusUX)
#define S_StatusKSU		3			/* Two-bit current mode (R/W) */
#define M_StatusKSU		(0x3 << S_StatusKSU)
#define S_StatusUM		4			/* User mode if supervisor mode not implemented (R/W) */
#define M_StatusUM		(0x1 << S_StatusUM)
#define S_StatusSM		3			/* Supervisor mode (R/W) */
#define M_StatusSM		(0x1 << S_StatusSM)
#define S_StatusERL		2			/* Denotes error level (R/W) */
#define M_StatusERL		(0x1 << S_StatusERL)
#define S_StatusEXL		1			/* Denotes exception level (R/W) */
#define M_StatusEXL		(0x1 << S_StatusEXL)
#define S_StatusIE		0			/* Enables interrupts (R/W) */
#define M_StatusIE		(0x1 << S_StatusIE)

#define M_Status0Fields		0x00040000
#define M_StatusRFields		0x058000e0		/* FR, MX, PX, KX, SX, UX unused in MIPS32 */
#define M_Status0Fields64	0x00040000
#define M_StatusRFields64	0x00000000

/*
 * Values in the KSU field
 */
#define K_StatusKSU_U		2			/* User mode in KSU field */
#define K_StatusKSU_S		1			/* Supervisor mode in KSU field */
#define K_StatusKSU_K		0			/* Kernel mode in KSU field */

/*
 ************************************************************************
 *                C A U S E   R E G I S T E R   ( 1 3 )                 *
 ************************************************************************
 * 	
 *  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
 *  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * | | |   | | |   |I|W| | | | | | |I|I|I|I|I|I|I|I| |         |   |
 * |B|0| C |0|0| 0 |V|P| | | | | | |P|P|P|P|P|P|P|P|0| ExcCode | 0 | Cause
 * |D| | E | | |   | | | | | | | | |7|6|5|4|3|2|1|0| |         |   |
 * | | |   | | |   | | | | | | | | | | | | | | | | | |         |   |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *    T     D P                    \---RIPL----/
 *    I     C C                      Release 2
 *            I
 */

#define C0_Cause		$13
#define R_C0_Cause		13
#define R_C0_SelCause	0
#define C0_CAUSE		C0_Cause		/* OBSOLETE - DO NOT USE IN NEW CODE */

#define S_CauseBD		31
#define M_CauseBD		(0x1 << S_CauseBD)
#define S_CauseCE		28
#define M_CauseCE		(0x3<< S_CauseCE)
#define S_CauseIV		23
#define M_CauseIV		(0x1 << S_CauseIV)
#define S_CauseWP		22
#define M_CauseWP		(0x1 << S_CauseWP)


#define S_CauseIP		8
#define M_CauseIP		(0xff << S_CauseIP)
#define S_CauseIPEXT		10
#define M_CauseIPEXT		(0x3f << S_CauseIPEXT)

#define S_CausePCI      26
#define M_CausePCI      (0x1 << S_CausePCI)
#define S_CauseDC       27
#define M_CauseDC       (0x1 << S_CauseDC)
#define S_CauseTI       30
#define M_CauseTI       (0x1 << S_CauseTI)
#define S_CauseRIPL     10
#define M_CauseRIPL     (0x3f << S_CauseRIPL)

#define S_CauseIP13		21
#define M_CauseIP13		(0x1 << S_CauseIP13)
#define S_CauseIP12		20
#define M_CauseIP12		(0x1 << S_CauseIP12)
#define S_CauseIP11		19
#define M_CauseIP11		(0x1 << S_CauseIP10)
#define S_CauseIP10		18
#define M_CauseIP10		(0x1 << S_CauseIP9)
#define S_CauseIP9		17
#define M_CauseIP9		(0x1 << S_CauseIP8)
#define S_CauseIP8		16
#define M_CauseIP8		(0x1 << S_CauseIP7)

#define S_CauseIP7		15
#define M_CauseIP7		(0x1 << S_CauseIP7)
#define S_CauseIP6		14
#define M_CauseIP6		(0x1 << S_CauseIP6)
#define S_CauseIP5		13
#define M_CauseIP5		(0x1 << S_CauseIP5)
#define S_CauseIP4		12
#define M_CauseIP4		(0x1 << S_CauseIP4)
#define S_CauseIP3		11
#define M_CauseIP3		(0x1 << S_CauseIP3)
#define S_CauseIP2		10
#define M_CauseIP2		(0x1 << S_CauseIP2)
#define S_CauseIP1		9
#define M_CauseIP1		(0x1 << S_CauseIP1)
#define S_CauseIP0		8
#define M_CauseIP0		(0x1 << S_CauseIP0)
#define S_CauseExcCode		2
#define M_CauseExcCode		(0x1f << S_CauseExcCode)

#ifdef MIPS_Release2
#define M_Cause0FieldsR2	0x033f0083
#define M_CauseRFieldsR2	0xf400fc7c
#endif
#define M_Cause0Fields		0x4f3f0083
#define M_CauseRFields		0xb000fc7c

/*
 * Values in the CE field
 */
#define K_CauseCE0		0			/* Coprocessor 0 in the CE field */
#define K_CauseCE1		1			/* Coprocessor 1 in the CE field */
#define K_CauseCE2		2			/* Coprocessor 2 in the CE field */
#define K_CauseCE3		3			/* Coprocessor 3 in the CE field */

/*
 * Values in the ExcCode field
 */
#define	EX_INT			0			/* Interrupt */
#define	EXC_INT			(EX_INT << S_CauseExcCode)
#define	EX_MOD			1			/* TLB modified */
#define	EXC_MOD			(EX_MOD << S_CauseExcCode)
#define	EX_TLBL		        2			/* TLB exception (load or ifetch) */
#define	EXC_TLBL		(EX_TLBL << S_CauseExcCode)
#define	EX_TLBS		        3			/* TLB exception (store) */
#define	EXC_TLBS		(EX_TLBS << S_CauseExcCode)
#define	EX_ADEL		        4			/* Address error (load or ifetch) */
#define	EXC_ADEL		(EX_ADEL << S_CauseExcCode)
#define	EX_ADES		        5			/* Address error (store) */
#define	EXC_ADES		(EX_ADES << S_CauseExcCode)
#define	EX_IBE			6			/* Instruction Bus Error */
#define	EXC_IBE			(EX_IBE << S_CauseExcCode)
#define	EX_DBE			7			/* Data Bus Error */
#define	EXC_DBE			(EX_DBE << S_CauseExcCode)
#define	EX_SYS			8			/* Syscall */
#define	EXC_SYS			(EX_SYS << S_CauseExcCode)
#define	EX_SYSCALL		EX_SYS
#define	EXC_SYSCALL		EXC_SYS
#define	EX_BP			9			/* Breakpoint */
#define	EXC_BP			(EX_BP << S_CauseExcCode)
#define	EX_BREAK		EX_BP
#define	EXC_BREAK		EXC_BP
#define	EX_RI			10			/* Reserved instruction */
#define	EXC_RI			(EX_RI << S_CauseExcCode)
#define	EX_CPU			11			/* CoProcessor Unusable */
#define	EXC_CPU			(EX_CPU << S_CauseExcCode)
#define	EX_OV			12			/* OVerflow */
#define	EXC_OV			(EX_OV << S_CauseExcCode)
#define	EX_TR		    	13			/* Trap instruction */
#define	EXC_TR			(EX_TR << S_CauseExcCode)
#define	EX_TRAP			EX_TR
#define	EXC_TRAP		EXC_TR
#define EX_FPE			15			/* floating point exception */
#define EXC_FPE			(EX_FPE << S_CauseExcCode)
#define EX_CEU			17			/* CorExtend exception */
#define EXC_CEU			(EX_CEU << S_CauseExcCode)
#define EX_C2E			18			/* COP2 exception */
#define EXC_C2E			(EX_C2E << S_CauseExcCode)
#define EX_MDMX			22			/* MDMX exception */
#define EXC_MDMX		(EX_MDMX << S_CauseExcCode)
#define EX_WATCH		23			/* Watch exception */
#define EXC_WATCH		(EX_WATCH << S_CauseExcCode)
#define EX_MCHECK	    24 		/* Machine check exception */
#define EXC_MCHECK  	(EX_MCHECK << S_CauseExcCode)
#define EX_THREAD	        25			/* MT Thread exception */
#define EXC_THREAD 		(EX_THREAD << S_CauseExcCode)
#define EX_DSPDIS	        26			/* DSP Disabled exception */
#define EXC_DSPDIS 		(EX_DSPDIS << S_CauseExcCode)
#define EX_CacheErr	        30			/* Cache error caused re-entry to Debug Mode */
#define EXC_CacheErr 	(EX_CacheErr << S_CauseExcCode)

/*
 ************************************************************************
 *               C O N F I G   R E G I S T E R   ( 1 6 )                *
 ************************************************************************
 * 	
 *  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
 *  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |M|                             |B| A |  A  |  M  |RSVD |V|  K  | Config
 * | | Reserved for Implementations|E| T |  R  |  T  |     |I|  0  |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

#define C0_Config		$16
#define R_C0_Config		16
#define R_C0_SelConfig		0
#define C0_CONFIG		C0_Config		/* OBSOLETE - DO NOT USE IN NEW CODE */


#define S_ConfigMore		31			/* Additional config registers present (R) */
#define M_ConfigMore		(0x1 << S_ConfigMore)
#define S_ConfigImpl		16			/* Implementation-specific fields */
#define M_ConfigImpl		(0x7fff << S_ConfigImpl)
#define S_ConfigBE		15			/* Denotes big-endian operation (R) */
#define M_ConfigBE		(0x1 << S_ConfigBE)
#define S_ConfigAT		13			/* Architecture type (R) */
#define M_ConfigAT		(0x3 << S_ConfigAT)
#define W_ConfigAT		2
#define S_ConfigAR		10			/* Architecture revision (R) */
#define M_ConfigAR		(0x7 << S_ConfigAR)
#define S_ConfigMT		7			/* MMU Type (R) */
#define M_ConfigMT		(0x7 << S_ConfigMT)
#define W_ConfigMT		3
#define S_ConfigVI		3			/* Icache is virtual (R) */
#define M_ConfigVI		(0x1 << S_ConfigVI)
#define S_ConfigK0		0			/* Kseg0 coherency algorithm (R/W) */
#define M_ConfigK0		(0x7 << S_ConfigK0)
#define W_ConfigK0		3

/*
 * The following definitions are technically part of the "reserved for
 * implementations" field, but are the semi-standard definition used in
 * fixed-mapping MMUs to control the cacheability of kuseg and kseg2/3
 * references.  For that reason, they are included here, but may be
 * overridden by true implementation-specific definitions
 */
#define S_ConfigK23		28			/* Kseg2/3 coherency algorithm (FM MMU only) (R/W) */
#define M_ConfigK23		(0x7 << S_ConfigK23)
#define W_ConfigK23		3
#define S_ConfigKU		25			/* Kuseg coherency algorithm (FM MMU only) (R/W) */
#define M_ConfigKU		(0x7 << S_ConfigKU)
#define W_ConfigKU		3

#define M_Config0Fields		0x00000070
#define M_ConfigRFields		0x8000ff88

/*
 * Values in the AT field
 */
#define K_ConfigAT_MIPS32	0			/* MIPS32 */
#define K_ConfigAT_MIPS64S	1			/* MIPS64 with 32-bit addresses */
#define K_ConfigAT_MIPS64	2			/* MIPS64 with 32/64-bit addresses */
#define K_ConfigAT_MAX		2			/* Max value */

/*
 * Values in the AR field
 */

#define K_ConfigAR_Rel1		0			/* Release 1 of the architecture */
#define K_ConfigAR_Rel2		1			/* Release 2 of the architecture */

/*
 * Values in the MT field
 */
#define K_ConfigMT_NoMMU	0			/* No MMU */
#define K_ConfigMT_TLBMMU	1			/* Standard TLB MMU */
#define K_ConfigMT_BATMMU	2			/* Standard BAT MMU */
#define K_ConfigMT_FMTMMU	3			/* Standard FMT MMU */
#define K_ConfigMT_FMMMU	K_ConfigMT_FMTMMU       /* alias for compatibility */

/*
 ************************************************************************
 *         C O N F I G 1   R E G I S T E R   ( 1 6, SELECT 1 )          *
 ************************************************************************
 * 	
 *  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
 *  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |M|  MMU Size |  IS |  IL |  IA |  DS |  DL |  DA |C|M|P|W|C|E|F| Config1
 * | |           |     |     |     |     |     |     |2|D|C|R|A|P|P|
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

#define C0_Config1		$16,1
#define R_C0_Config1		16
#define R_C0_SelConfig1		1

#define S_Config1M		31			/* Additional Config registers present (R) */
#define M_Config1M		(0x1 << S_Config1More)
#define S_Config1More		S_Config1M		/* OBSOLETE */
#define M_Config1More		(0x1 << S_Config1M)
#define S_Config1MMUSize 	25			/* Number of MMU entries - 1 (R) */
#define M_Config1MMUSize 	(0x3f << S_Config1MMUSize)
#define W_Config1MMUSize 	6
#define S_Config1IS		22			/* Icache sets per way (R) */
#define M_Config1IS		(0x7 << S_Config1IS)
#define W_Config1IS		3
#define S_Config1IL		19			/* Icache line size (R) */
#define M_Config1IL		(0x7 << S_Config1IL)
#define W_Config1IL		3
#define S_Config1IA		16			/* Icache associativity - 1 (R) */
#define M_Config1IA		(0x7 << S_Config1IA)
#define W_Config1IA		3
#define S_Config1DS		13			/* Dcache sets per way (R) */
#define M_Config1DS		(0x7 << S_Config1DS)
#define W_Config1DS		3
#define S_Config1DL		10			/* Dcache line size (R) */
#define M_Config1DL		(0x7 << S_Config1DL)
#define W_Config1DL		3
#define S_Config1DA		7			/* Dcache associativity (R) */
#define M_Config1DA		(0x7 << S_Config1DA)
#define S_Config1C2		6			/* Coprocessor 2 present (R) */
#define W_Config1DA		3
#define M_Config1C2		(0x1 << S_Config1C2)
#define S_Config1MD		5			/* Denotes MDMX present (R) */
#define M_Config1MD		(0x1 << S_Config1MD)
#define S_Config1PC		4			/* Denotes performance counters present (R) */
#define M_Config1PC		(0x1 << S_Config1PC)
#define S_Config1WR		3			/* Denotes watch registers present (R) */
#define M_Config1WR		(0x1 << S_Config1WR)
#define S_Config1CA		2			/* Denotes MIPS-16 present (R) */
#define M_Config1CA		(0x1 << S_Config1CA)
#define S_Config1EP		1			/* Denotes EJTAG present (R) */
#define M_Config1EP		(0x1 << S_Config1EP)
#define S_Config1FP		0			/* Denotes floating point present (R) */
#define M_Config1FP		(0x1 << S_Config1FP)
#define W_Config1FP		1

#endif


#endif /* _ASM_MIPSREGS_H */
