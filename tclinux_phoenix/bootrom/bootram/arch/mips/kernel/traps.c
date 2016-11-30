/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1994 - 1999 by Ralf Baechle
 * Modified for R3000 by Paul M. Antoine, 1995, 1996
 * Complete output from die() by Ulf Carlsson, 1998
 * Copyright (C) 1999 Silicon Graphics, Inc.
 */

#include <linux/linkage.h>
#include <asm/ptrace.h>
#include <asm/mipsregs.h>
#include <asm/addrspace.h>
#include <asm/system.h>
#include <asm/tc3162.h>

#ifdef TC3262

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

#define PTR		.dword

/*
 * Cache Operations available on all MIPS processors with R4000-style caches
 */
#define Index_Invalidate_I      0x00
#define Index_Writeback_Inv_D   0x01
#define Index_Load_Tag_I	0x04
#define Index_Load_Tag_D	0x05
#define Index_Store_Tag_I	0x08
#define Index_Store_Tag_D	0x09
#define Hit_Invalidate_I	0x10
#define Hit_Invalidate_D	0x11
#define Hit_Writeback_Inv_D	0x15

#define cache_op(op,addr)						\
	__asm__ __volatile__(						\
	"	.set	push					\n"	\
	"	.set	noreorder				\n"	\
	"	.set	mips3\n\t				\n"	\
	"	cache	%0, %1					\n"	\
	"	.set	pop					\n"	\
	:								\
	: "i" (op), "R" (*(unsigned char *)(addr)))


#define protected_cache_op(op,addr)				\
	__asm__ __volatile__(					\
	"	.set	push			\n"		\
	"	.set	noreorder		\n"		\
	"	.set	mips3			\n"		\
	"1:	cache	%0, (%1)		\n"		\
	"2:	.set	pop			\n"		\
	"	.section __ex_table,\"a\"	\n"		\
	"	"STR(PTR)" 1b, 2b		\n"		\
	"	.previous"					\
	:							\
	: "i" (op), "r" (addr))

#define instruction_hazard()						\
do {									\
	unsigned long tmp;						\
									\
	__asm__ __volatile__(						\
	"	.set	mips64r2				\n"	\
	"	dla	%0, 1f					\n"	\
	"	jr.hb	%0					\n"	\
	"	.set	mips0					\n"	\
	"1:							\n"	\
	: "=r" (tmp));							\
} while (0)

static void protected_writeback_dcache_line(unsigned long addr)
{
	protected_cache_op(Hit_Writeback_Inv_D, addr);
}

void flush_icache_range(unsigned long start, unsigned long end)	
{									
	unsigned long lsize = 32;
	unsigned long addr = start & ~(lsize - 1);			
	unsigned long aend = (end - 1) & ~(lsize - 1);			

	while (1) {							
		protected_cache_op(Hit_Writeback_Inv_D, addr);
		__asm__ __volatile__ ("sync");
		protected_cache_op(Hit_Invalidate_I, addr);				
		if (addr == aend)					
			break;						
		addr += lsize;						
	}								

	instruction_hazard();
}

void flush_dcache_range(unsigned long start, unsigned long end)	
{									
	unsigned long lsize = 32;
	unsigned long addr = start & ~(lsize - 1);			
	unsigned long aend = (end - 1) & ~(lsize - 1);			

	while (1) {							
		protected_cache_op(Hit_Invalidate_D, addr);				
		if (addr == aend)					
			break;						
		addr += lsize;						
	}								

	instruction_hazard();
}

#else

#define CP0_CCTL $20		/* Lexra Cache Control Register */

/*
 * Lexra Cache Control Register fields
 */
#define CCTL_DINVAL	        0x00000001
#define CCTL_IINVAL			0x00000002
#define CCTL_ILOCK			0x0000000c
#define CCTL_IRAMFILL4      0x00000010
#define CCTL_IRAMOFF		0x00000020

#define PROM_DEBUG

#ifdef PROM_DEBUG
extern int prom_printf(char *, ...);
#endif

void flush_cache_all(void);
void flush_icache(void);
void flush_dcache(void);

void flush_cache_all(void)
{
	flush_icache();
	flush_dcache();
}

void flush_icache()
{
	unsigned int controlReg;

	controlReg = read_32bit_cp0_register(CP0_CCTL);

	write_32bit_cp0_register(CP0_CCTL, (controlReg & ~CCTL_IINVAL));
	write_32bit_cp0_register(CP0_CCTL, (controlReg | CCTL_IINVAL));

	/* delay to allow cache to be flushed */
	__asm__ __volatile__(".set\tnoreorder\n\t"
			     "nop\n\t"
			     "nop\n\t"
			     "nop\n\t"
			     "nop\n\t"
			     "nop\n\t"
			     "nop\n\t"
			     "nop\n\t"
			     "nop\n\t" "nop\n\t" ".set\treorder\n\t");

	write_32bit_cp0_register(CP0_CCTL, controlReg);
}

void flush_dcache()
{
	unsigned int controlReg;

	controlReg = read_32bit_cp0_register(CP0_CCTL);

	write_32bit_cp0_register(CP0_CCTL, (controlReg & ~CCTL_DINVAL));
	write_32bit_cp0_register(CP0_CCTL, (controlReg | CCTL_DINVAL));

	/* delay to allow cache to be flushed */
	__asm__ __volatile__(".set\tnoreorder\n\t"
			     "nop\n\t"
			     "nop\n\t"
			     "nop\n\t"
			     "nop\n\t"
			     "nop\n\t"
			     "nop\n\t"
			     "nop\n\t"
			     "nop\n\t" "nop\n\t" ".set\treorder\n\t");

	write_32bit_cp0_register(CP0_CCTL, controlReg);
}
#endif

void trap_init(void)
{
#ifdef TC3262
	/* clear interrupt counter */
	VPint(CR_INTC_ITR) |= (1<<18) | (1<<10);
#else
	flush_cache_all();
#endif
}
