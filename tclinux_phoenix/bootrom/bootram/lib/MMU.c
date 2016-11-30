#include <asm/tc3162.h>
#include <asm/system.h>

#define CONFIG_PAGE_SIZE_64M

#define PM_4K		0x00000000
#define PM_16K		0x00006000
#define PM_64K		0x0001e000
#define PM_256K		0x0007e000
#define PM_1M		0x001fe000
#define PM_4M		0x007fe000
#define PM_16M		0x01ffe000
#define PM_64M		0x07ffe000
#define PM_256M		0x1fffe000

#ifdef CONFIG_PAGE_SIZE_4KB
#define PM_DEFAULT_MASK	PM_4K
#elif defined(CONFIG_PAGE_SIZE_16KB)
#define PM_DEFAULT_MASK	PM_16K
#elif defined(CONFIG_PAGE_SIZE_64KB)
#define PM_DEFAULT_MASK	PM_64K
#elif defined(CONFIG_PAGE_SIZE_4M)
#define PM_DEFAULT_MASK	PM_4M
#elif defined(CONFIG_PAGE_SIZE_64M)
#define PM_DEFAULT_MASK	PM_64M
#else
#error Bad page size configuration!
#endif

#ifdef CONFIG_PAGE_SIZE_4KB
#define PAGE_SHIFT	12
#elif defined(CONFIG_PAGE_SIZE_8KB)
#define PAGE_SHIFT	13
#elif defined(CONFIG_PAGE_SIZE_16KB)
#define PAGE_SHIFT	14
#elif defined(CONFIG_PAGE_SIZE_64KB)
#define PAGE_SHIFT	16
#elif defined(CONFIG_PAGE_SIZE_4M)
#define PAGE_SHIFT	22
#elif defined(CONFIG_PAGE_SIZE_64M)
#define PAGE_SHIFT	26
#else
#error Bad page shift configuration!
#endif

#define PAGE_SIZE	(1UL << PAGE_SHIFT)
#define PAGE_MASK       (~((1 << PAGE_SHIFT) - 1))

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


/*
 * TLB operations.
 *
 * It is responsibility of the caller to take care of any TLB hazards.
 */
static inline void tlb_probe(void)
{
	asm volatile(
		//".set noreorder\n\t"
		//".set\tmips32\n\t"
		"tlbp"
		//".set\tmips0"
		//".set reorder"
		);
}

static inline void tlb_read(void)
{
	asm volatile(
		//".set noreorder\n\t"
		//".set\tmips32\n\t"
		"tlbr"
		//".set\tmips0"
		//".set reorder"
		);
}

static inline void tlb_write_indexed(void)
{
	asm volatile(
		//".set noreorder\n\t"
		//".set\tmips32\n\t"
		"tlbwi"
		//".set\tmips0"
		//".set reorder"
		);
}

static inline void tlb_write_random(void)
{
	asm volatile(
		//".set noreorder\n\t"
		//".set\tmips32\n\t"
		"tlbwr"
		//".set\tmips0"
		//".set reorder"
		);
}


#define read_c0_entrylo0()	__read_32bit_c0_register($2, 0)
#define write_c0_entrylo0(val)	__write_32bit_c0_register($2, 0, val)
#define read_c0_entrylo1()	__read_32bit_c0_register($3, 0)
#define write_c0_entrylo1(val)	__write_32bit_c0_register($3, 0, val)
#define read_c0_index()		__read_32bit_c0_register($0, 0)
#define write_c0_index(val)	__write_32bit_c0_register($0, 0, val)
#define read_c0_wired()		__read_32bit_c0_register($6, 0)
#define write_c0_wired(val)	__write_32bit_c0_register($6, 0, val)
#define read_c0_entryhi()	__read_32bit_c0_register($10, 0)
#define write_c0_entryhi(val)	__write_32bit_c0_register($10, 0, val)
#define read_c0_pagemask()	__read_32bit_c0_register($5, 0)
#define write_c0_pagemask(val)	__write_32bit_c0_register($5, 0, val)


#define CKSEG0 0x80000000
/*
 * Make sure all entries differ.  If they're not different
 * MIPS32 will take revenge ...
 */
#define UNIQUE_ENTRYHI(idx) (CKSEG0 + ((idx) << (PAGE_SHIFT + 1)))

#define TLB_PAGE_GLOBAL  (1<<0)
#define TLB_PAGE_VALID  (1<<1)
#define TLB_PAGE_DIRTY  ((1<<2)|(1<<4)|(1<<10))
#define TLB_PAGE_CACHE_COHERENCY_UNCACHED  (2<<3)
#define TLB_PAGE_CACHE_COHERENCY_WRITEBACK  (3<<3)
#define TLB_PAGE_CACHE_COHERENCY_UNCACHEDACC  (7<<3)

static unsigned long ENTRYLO(unsigned long paddr)
{
#if 0
	return (((paddr & PAGE_MASK) >> 6 ) | (TLB_PAGE_GLOBAL | TLB_PAGE_VALID | TLB_PAGE_DIRTY |TLB_PAGE_CACHE_COHERENCY_UNCACHED));
#else
	return (((paddr & PAGE_MASK) >> 6 ) | (TLB_PAGE_GLOBAL | TLB_PAGE_VALID | TLB_PAGE_DIRTY));
#endif
}




#define SSNOP   	sll $0,$0,1
#define EHB     	sll $0,$0,3
#define NOPS		SSNOP; SSNOP; SSNOP; EHB

static void NOPS_C(void)
{
	asm("sll $0,$0,1");
	asm("sll $0,$0,1");
	asm("sll $0,$0,1");
	asm("sll $0,$0,3");
};

int g_TLBSize = 32;

void local_flush_tlb_all(void) 
{
	unsigned long flags;
	unsigned long old_ctx;
	int entry;

	/* Save old context and create impossible VPN2 value */
	old_ctx = read_c0_entryhi();
	write_c0_entrylo0(0);
	write_c0_entrylo1(0);

	entry = read_c0_wired();
	entry = 0;
	/* Blast 'em all away. */
	while (entry < g_TLBSize) {
		/* Make sure all entries differ. */
		write_c0_entryhi(UNIQUE_ENTRYHI(entry));
		write_c0_index(entry);
		//write_c0_entrylo0(0);
		//write_c0_entrylo1(0);
		//mtc0_tlbw_hazard();
		NOPS_C();
		tlb_write_indexed();
		entry++;
	}
	//tlbw_use_hazard();
	NOPS_C();
	write_c0_entryhi(old_ctx);
}

void update_tlb_wired(unsigned long vir_address, unsigned long phy_address, unsigned long tlb_entry)
{
	unsigned long flags;
	int idx, pid;
	unsigned long old_wired;
	unsigned long old_pagemask;
	unsigned long old_ctx;
	unsigned long old_idx;
	unsigned long old_entrylo0;
	unsigned long old_entrylo1;
	unsigned long old_entryhi;
	unsigned long old_c0status;

	/*Flush all TLB Entry*/
	local_flush_tlb_all();

	
	/*Add new tlb entry*/
	/* Save old context and create impossible VPN2 value */
	old_ctx = read_c0_entryhi(); 
	old_pagemask = read_c0_pagemask();

	write_c0_index(tlb_entry);
	NOPS_C();
	write_c0_pagemask(PM_DEFAULT_MASK); 
    pid = old_ctx & 0xff;
	vir_address &= (PAGE_MASK << 1);
	write_c0_entryhi(vir_address | pid);
    write_c0_entrylo0(ENTRYLO(phy_address)); 
	prom_printf("pagesize: 0x%x \n",PAGE_SIZE);
	write_c0_entrylo1(ENTRYLO(phy_address+PAGE_SIZE));
	tlb_write_indexed();
	NOPS_C();
	//tlbw_use_hazard();
	//write_c0_entryhi(old_ctx);
	NOPS_C();
	//tlbw_use_hazard();	/* What is the hazard here? */
	//write_c0_pagemask(old_pagemask);
}



