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
#include <asm/fpu.h>
#include <asm/mipsregs.h>
#include <asm/system.h>
#include <asm/r4kcache.h>
#include <asm/hazards.h>
#ifdef CONFIG_MIPS_TC3262
#include <asm/tc3162/tc3162.h>

extern int __imem, __dmem;
#endif
static char *sram_allocp = NULL;
static int sram_size = 0;
static int sram_free = 0;

static char *dspram_p = NULL;
static int dspram_used_size = 0;
static int dspram_max_size = 0x1000;	//4K
int is_sram_addr(void *p)
{
	if ((CKSEG1ADDR(p) & 0xffffc000) == (CKSEG1ADDR(DSPRAM_BASE) & 0xffffc000))
		return 1;
	else 
		return 0;
}
EXPORT_SYMBOL(is_sram_addr);

void *alloc_sram(int n)
{
	if (sram_allocp == NULL)
		return NULL;

	if (sram_free >= n) {
		sram_free -= n;
		sram_allocp += n;
		printk("alloc_sram p=%p free=%04x\n", sram_allocp, sram_free);
		return sram_allocp - n;
	} else 
		return NULL;
}
EXPORT_SYMBOL(alloc_sram);

void free_sram(void *p, int n)
{
	if (sram_allocp == (p+n)) {
		sram_free += n;
		sram_allocp -= n;
	}
	printk("free_sram p=%p free=%04x\n", sram_allocp, sram_free);
}
EXPORT_SYMBOL(free_sram);

void write_to_dspram(long  data)
{
	if(dspram_p == NULL || dspram_max_size == 0)
		return;

	
	*(long *)dspram_p = data;
	dspram_p += sizeof(long);

	dspram_used_size += sizeof(long);

	if(dspram_used_size >= dspram_max_size){
		dspram_p = (char *)(DSPRAM_BASE);
		dspram_used_size = 0;
	}
}

unsigned int dspram_base_addr()
{
	return DSPRAM_BASE;
}


#define MIPS34K_Index_Store_Data_I	0x0c

/*
 * These definitions are correct for the 24K/34K/74K SPRAM sample
 * implementation. The 4KS interpreted the tags differently...
 */
#define SPRAM_TAG0_ENABLE	0x00000080
#define SPRAM_TAG0_PA_MASK	0xfffff000
#define SPRAM_TAG1_SIZE_MASK	0xfffff000

#define SPRAM_TAG_STRIDE	8

#define ERRCTL_SPRAM		(1 << 28)

/* errctl access */
#define read_c0_errctl(x) read_c0_ecc(x)
#define write_c0_errctl(x) write_c0_ecc(x)

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

static void ispram_store_data(unsigned int offset, unsigned int datalo, unsigned int datahi)
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

static void ispram_store_tag(unsigned int offset, unsigned int data)
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


static unsigned int ispram_load_tag(unsigned int offset)
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

static __cpuinit void dspram_store_tag(unsigned int offset, unsigned int data)
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


static __cpuinit unsigned int dspram_load_tag(unsigned int offset)
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

static void probe_spram(char *type,
	    unsigned int base,
	    unsigned int (*read)(unsigned int),
	    void (*write)(unsigned int, unsigned int))
{
	unsigned int firstsize = 0, lastsize = 0;
	unsigned int firstpa = 0, lastpa = 0, pa = 0;
	unsigned int offset = 0;
	unsigned int size, tag0, tag1;
	unsigned int enabled;
	int i;

	/*
	 * The limit is arbitrary but avoids the loop running away if
	 * the SPRAM tags are implemented differently
	 */

	for (i = 0; i < 8; i++) {
		tag0 = read(offset);
		tag1 = read(offset+SPRAM_TAG_STRIDE);
		pr_debug("DBG %s%d: tag0=%08x tag1=%08x\n",
			 type, i, tag0, tag1);

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
		tag0 = (base & SPRAM_TAG0_PA_MASK) | SPRAM_TAG0_ENABLE;
		write(offset, tag0);

		base += size;

		/* reread the tag */
		tag0 = read(offset);
		pa = tag0 & SPRAM_TAG0_PA_MASK;
		enabled = tag0 & SPRAM_TAG0_ENABLE;

		if (i == 0) {
			firstpa = pa;
			firstsize = size;
		}

		lastpa = pa;
		lastsize = size;

		if (strcmp(type, "DSPRAM") == 0) {
			unsigned int *vp = (unsigned int *)(CKSEG1 | pa);
			unsigned int v;
			if(!isMT751020 && !isMT7505 && !isEN751221){
#define TDAT	0x5a5aa5a5
			vp[0] = TDAT;
			vp[1] = ~TDAT;

			mb();

			v = vp[0];
			if (v != TDAT)
				printk(KERN_ERR "vp=%p wrote=%08x got=%08x\n",
				       vp, TDAT, v);
			v = vp[1];
			if (v != ~TDAT)
				printk(KERN_ERR "vp=%p wrote=%08x got=%08x\n",
				       vp+1, ~TDAT, v);
			}
#ifdef CONFIG_MIPS_TC3262
			if (enabled) {
				if(isMT751020 || isMT7505 || isEN751221){
					dspram_max_size = size;
				}
				else{
				sram_allocp = (char *) vp;
				sram_size = sram_free = size;
				}
			}
#endif
		}

		pr_info("%s%d: PA=%08x,Size=%08x%s\n",
			type, i, pa, size, enabled ? ",enabled" : "");
		offset += 2 * SPRAM_TAG_STRIDE;
	}
}

void ispram_fill(void)
{
	unsigned int pa, size, tag0, tag1;
	unsigned int offset;
	unsigned int datalo, datahi;

	tag0 = ispram_load_tag(0);
	tag1 = ispram_load_tag(0+SPRAM_TAG_STRIDE);

	pa = tag0 & SPRAM_TAG0_PA_MASK;
	size = tag1 & SPRAM_TAG1_SIZE_MASK;

	if (size == 0)
		return;

	for (offset = 0; offset < size; offset += 8) {
		datalo = *(unsigned int *) (PHYS_TO_K0(pa + offset));
		datahi = *(unsigned int *) (PHYS_TO_K0(pa + offset + 4));
		ispram_store_data(offset, datalo, datahi);
	}
}

void ispram_refill(void)
{
	//probe_spram("ISPRAM", CPHYSADDR(&__imem),
	//		&ispram_load_tag, &ispram_store_tag);
	ispram_fill();
}

void __cpuinit spram_config(void)
{
	struct cpuinfo_mips *c = &current_cpu_data;
	unsigned int config0;

	switch (c->cputype) {
	case CPU_24K:
	case CPU_34K:
	case CPU_74K:
	case CPU_1004K:
		config0 = read_c0_config();
		/* FIXME: addresses are Malta specific */
#ifdef CONFIG_MIPS_TC3262
#ifdef CONFIG_TC3162_IMEM
		if (config0 & (1<<24)) {
			probe_spram("ISPRAM", CPHYSADDR(&__imem),
				    &ispram_load_tag, &ispram_store_tag);
			ispram_fill();
			if (!isRT63165 && !isRT63365 && !isMT751020 && !isMT7505 && !isEN751221)
				VPint(CR_DMC_ISPCFGR) = (CPHYSADDR(&__imem) & 0xfffff000) | (1<<8) | (0x7);
		}
#endif
#ifdef CONFIG_TC3162_DMEM
		if (isRT63165 || isRT63365) {
			VPint(CR_SRAM) = (CPHYSADDR(DSPRAM_BASE) & 0xffffc000) | (1<<0);
			printk("Enable SRAM=%08lx\n", VPint(CR_SRAM));

			sram_allocp = (char *) CKSEG1ADDR(DSPRAM_BASE);
			sram_size = sram_free = 0x8000;
		} else {
			if (!isTC3182 && !isRT65168) {
				if (config0 & (1<<23)) {
					if(isMT751020){
						probe_spram("DSPRAM", CPHYSADDR(DSPRAM_BASE),
							&dspram_load_tag, &dspram_store_tag);
						dspram_p = (char *)(DSPRAM_BASE);
					}
					else{
						probe_spram("DSPRAM", CPHYSADDR(DSPRAM_BASE),
							&dspram_load_tag, &dspram_store_tag);
						VPint(CR_DMC_DSPCFGR) = (CPHYSADDR(DSPRAM_BASE) & 0xfffff000) | (1<<8) | (0x7);
					}
				}
			}
		}
#endif
#else
		if (config0 & (1<<24)) {
			probe_spram("ISPRAM", 0x1c000000,
				    &ispram_load_tag, &ispram_store_tag);
		}
		if (config0 & (1<<23))
			probe_spram("DSPRAM", 0x1c100000,
				    &dspram_load_tag, &dspram_store_tag);
#endif
	}
}
