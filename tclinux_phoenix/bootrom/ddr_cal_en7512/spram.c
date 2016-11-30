/*
 * MIPS SPRAM support
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 *
 * Copyright (C) 2007, 2008 MIPS Technologies, Inc.
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/ptrace.h>
#include <linux/stddef.h>
#include <linux/module.h>

#include <asm/cpu.h>
//#include <asm/fpu.h>
#include <asm/mipsregs.h>
#include <asm/system.h>
//#include <asm/r4kcache.h>
//#include <asm/hazards.h>
#include <asm/tc3162.h>

#define MIPS34K_Index_Store_Data_I	0x0c

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


#define CKSEG0 0x80000000


/*
 * These definitions are correct for the 24K/34K/74K SPRAM sample
 * implementation. The 4KS interpreted the tags differently...
 */
#define SPRAM_TAG0_ENABLE	0x00000080
#define SPRAM_TAG0_PA_MASK	0xfffff000
#define SPRAM_TAG1_SIZE_MASK	0xfffff000

#define SPRAM_TAG_STRIDE	8

#define ERRCTL_SPRAM		(1 << 28)

/*
 * Macros to access the system control coprocessor
 */

#define __read_32bit_c0_register(source, sel)				\
({ int __res;								\
	if (sel == 0)							\
		__asm__ __volatile__(					\
			"mfc0\t%0, " #source "\n\t"			\
			: "=r" (__res));				\
	else								\
		__asm__ __volatile__(					\
			".set\tmips32\n\t"				\
			"mfc0\t%0, " #source ", " #sel "\n\t"		\
			".set\tmips0\n\t"				\
			: "=r" (__res));				\
	__res;								\
})

#define __write_32bit_c0_register(register, sel, value)			\
do {									\
	if (sel == 0)							\
		__asm__ __volatile__(					\
			"mtc0\t%z0, " #register "\n\t"			\
			: : "Jr" ((unsigned int)(value)));		\
	else								\
		__asm__ __volatile__(					\
			".set\tmips32\n\t"				\
			"mtc0\t%z0, " #register ", " #sel "\n\t"	\
			".set\tmips0"					\
			: : "Jr" ((unsigned int)(value)));		\
} while (0)



#define read_c0_ecc()		__read_32bit_c0_register($26, 0)
#define write_c0_ecc(val)	__write_32bit_c0_register($26, 0, val)

#define read_c0_taglo()		__read_32bit_c0_register($28, 0)
#define write_c0_taglo(val)	__write_32bit_c0_register($28, 0, val)

#define read_c0_idatalo()		__read_32bit_c0_register($28, 1)
#define write_c0_idatalo(val)	__write_32bit_c0_register($28, 1, val)

#define read_c0_dtaglo()	__read_32bit_c0_register($28, 2)
#define write_c0_dtaglo(val)	__write_32bit_c0_register($28, 2, val)

#define read_c0_idatahi()		__read_32bit_c0_register($29, 1)
#define write_c0_idatahi(val)	__write_32bit_c0_register($29, 1, val)



#define read_c0_config()	__read_32bit_c0_register($16, 0)

#define PHYS_TO_K0(x)			(((uint32)x) | 0x80000000)
#define K0_TO_PHYS(x)			(((uint32)x) & 0x7fffffff)



/* errctl access */
#define read_c0_errctl(x) read_c0_ecc(x)
#define write_c0_errctl(x) write_c0_ecc(x)

static inline void ehb(void)
{
	__asm__ __volatile__(
	"	.set	mips32r2				\n"
	"	ehb						\n"
	"	.set	mips0					\n");
}


/*
 * Different semantics to the set_c0_* function built by __BUILD_SET_C0
 */
static unsigned int bis_c0_errctl(unsigned int set)
{
	unsigned int res;
	res = read_c0_errctl();
	write_c0_errctl(res | set);
	return res;
}
static   void ispram_store_data(unsigned int offset, unsigned int datalo, unsigned int datahi)
{
	unsigned int errctl;

	/* enable SPRAM tag access */
	errctl = bis_c0_errctl(ERRCTL_SPRAM);
	ehb();

#ifdef CONFIG_CPU_BIG_ENDIAN
	write_c0_idatalo(datahi);
	ehb();

	write_c0_idatahi(datalo);
	ehb();
#else
	write_c0_idatalo(datalo);
	ehb();

	write_c0_idatahi(datahi);
	ehb();
#endif

	cache_op(MIPS34K_Index_Store_Data_I, CKSEG0|offset);
	ehb();

	write_c0_errctl(errctl);
	ehb();
}

static   void ispram_store_tag(unsigned int offset, unsigned int data)
{
	unsigned int errctl;

	/* enable SPRAM tag access */
	errctl = bis_c0_errctl(ERRCTL_SPRAM);
	ehb();

	write_c0_taglo(data);
	ehb();

	cache_op(Index_Store_Tag_I, CKSEG0|offset);
	ehb();

	write_c0_errctl(errctl);
	ehb();
}


static   unsigned int ispram_load_tag(unsigned int offset)
{
	unsigned int data;
	unsigned int errctl;

	/* enable SPRAM tag access */
	errctl = bis_c0_errctl(ERRCTL_SPRAM);
	ehb();
	cache_op(Index_Load_Tag_I, CKSEG0 | offset);
	ehb();
	data = read_c0_taglo();
	ehb();
	write_c0_errctl(errctl);
	ehb();

	return data;
}

static   void dspram_store_tag(unsigned int offset, unsigned int data)
{
	unsigned int errctl;

	/* enable SPRAM tag access */
	errctl = bis_c0_errctl(ERRCTL_SPRAM);
	ehb();
	write_c0_dtaglo(data);
	ehb();
	cache_op(Index_Store_Tag_D, CKSEG0 | offset);
	ehb();
	write_c0_errctl(errctl);
	ehb();
}


static   unsigned int dspram_load_tag(unsigned int offset)
{
	unsigned int data;
	unsigned int errctl;

	errctl = bis_c0_errctl(ERRCTL_SPRAM);
	ehb();
	cache_op(Index_Load_Tag_D, CKSEG0 | offset);
	ehb();
	data = read_c0_dtaglo();
	ehb();
	write_c0_errctl(errctl);
	ehb();

	return data;
}

/*
enable:
0:disable dspram
1:enable dspram
2:disable ispram
3:enable ispram
*/
void probe_spram(unsigned int base, unsigned int enable)
{
	unsigned int offset = 0;
	unsigned int size, tag0, tag1;
	unsigned int enabled;
	int i;
	unsigned int type = (enable & 0x2) >> 0x1;

	if(type){
		tag0 = ispram_load_tag(offset);
		tag1 = ispram_load_tag(offset+SPRAM_TAG_STRIDE);
	}
	else{
		tag0 = dspram_load_tag(offset);
		tag1 = dspram_load_tag(offset+SPRAM_TAG_STRIDE);
	}
	size = tag1 & SPRAM_TAG1_SIZE_MASK;

	if (size == 0)
		return;

	/* Align base with size */
	base = (base + size - 1) & ~(size-1);

	/* reprogram the base address base address and enable */
	if(enable & 0x1)
  		tag0 = (base & SPRAM_TAG0_PA_MASK) | SPRAM_TAG0_ENABLE;
  	else
  		tag0 = (base & SPRAM_TAG0_PA_MASK);
	//write(offset, tag0);
	if(type){
		ispram_store_tag(offset, tag0);
	}
	else{
		dspram_store_tag(offset, tag0);
	}
	return;
}

#if 0
/*
enable:
0:disable dspram
1:enable dspram
2:disable ispram
3:enable ispram
*/
void probe_spram(unsigned int base, unsigned int enable)
{
	unsigned int firstsize = 0, lastsize = 0;
	unsigned int firstpa = 0, lastpa = 0, pa = 0;
	unsigned int offset = 0;
	unsigned int size, tag0, tag1;
	unsigned int enabled;
	int i;
	unsigned int type = (enable & 0x2) >> 0x1;
	/*
	 * The limit is arbitrary but avoids the loop running away if
	 * the SPRAM tags are implemented differently
	 */
	for (i = 0; i < 8; i++) {
		if(type){
			tag0 = ispram_load_tag(offset);
			tag1 = ispram_load_tag(offset+SPRAM_TAG_STRIDE);
		}
		else{
			tag0 = dspram_load_tag(offset);
			tag1 = dspram_load_tag(offset+SPRAM_TAG_STRIDE);
		}
		//pr_debug("DBG %s%d: tag0=%08x tag1=%08x\n",
		//	 type, i, tag0, tag1);

		size = tag1 & SPRAM_TAG1_SIZE_MASK;

		if (size == 0)
			break;

		if (i != 0) {
			/* tags may repeat... */
			if ((pa == firstpa && size == firstsize) ||
			    (pa == lastpa && size == lastsize))
				break;
		}

		/* Align base with size */
		base = (base + size - 1) & ~(size-1);

		/* reprogram the base address base address and enable */
		if(enable & 0x1)
	  		tag0 = (base & SPRAM_TAG0_PA_MASK) | SPRAM_TAG0_ENABLE;
	  	else
	  		tag0 = (base & SPRAM_TAG0_PA_MASK);
		//write(offset, tag0);
		if(type){
			ispram_store_tag(offset, tag0);
		}
		else{
			dspram_store_tag(offset, tag0);
		}

		base += size;

		/* reread the tag */
		//tag0 = read(offset);
		if(type){
			tag0 = ispram_load_tag(offset);
		}
		else{
			tag0 = dspram_load_tag(offset);
		}
		pa = tag0 & SPRAM_TAG0_PA_MASK;
		enabled = tag0 & SPRAM_TAG0_ENABLE;

		if (i == 0) {
			firstpa = pa;
			firstsize = size;
		}

		lastpa = pa;
		lastsize = size;

		offset += 2 * SPRAM_TAG_STRIDE;
	}
}
#endif

  void ispram_fill(int i_size, unsigned int from, unsigned int to)
{
	unsigned int pa, size, tag0, tag1;
	unsigned int offset;
	unsigned int datalo, datahi;

	tag0 = ispram_load_tag(0);
	tag1 = ispram_load_tag(0+SPRAM_TAG_STRIDE);

	pa = tag0 & SPRAM_TAG0_PA_MASK;
	if(i_size)
		size = i_size;
	else
		size = tag1 & SPRAM_TAG1_SIZE_MASK;

	if(from)
		pa = from;

	if (size == 0)
		return;

	for (offset = 0; offset < size; offset += 8) {
		//datalo = *(unsigned int *) (PHYS_TO_K0(pa + offset));
		//datahi = *(unsigned int *) (PHYS_TO_K0(pa + offset + 4));
		datahi = *(unsigned int *) (PHYS_TO_K0(pa + offset));
		datalo = *(unsigned int *) (PHYS_TO_K0(pa + offset + 4));
		ispram_store_data(to + offset, datalo, datahi);
	}
}

