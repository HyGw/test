#include <linux/init.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/errno.h>
#include <asm/io.h>
#include <asm/mipsregs.h>
#include <asm/tc3162.h>

#ifdef TC3262

extern void flush_icache_range(unsigned long start, unsigned long end);

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

#define read_c0_cause()		__read_32bit_c0_register($13, 0)
#define write_c0_cause(val)	__write_32bit_c0_register($13, 0, val)

#define read_c0_intctl()	__read_32bit_c0_register($12, 1)
#define write_c0_intctl(val)	__write_32bit_c0_register($12, 1, val)

#define read_c0_ebase()		__read_32bit_c0_register($15,1)
#define write_c0_ebase(val)	__write_32bit_c0_register($15, 1, val)

#define read_c0_config1()		__read_32bit_c0_register($16, 1)
#define write_c0_config1(val)	__write_32bit_c0_register($16, 1, val)

static unsigned int					
change_c0_cause(unsigned int change, unsigned int new)		
{								
	unsigned int res;					
								
	res = read_c0_cause();					
	res &= ~change;						
	res |= (new & change);					
	write_c0_cause(res);					
								
	return res;						
}

static unsigned int					
change_c0_intctl(unsigned int change, unsigned int new)		
{								
	unsigned int res;					
								
	res = read_c0_intctl();					
	res &= ~change;						
	res |= (new & change);					
	write_c0_intctl(res);					
								
	return res;						
}

#endif

void *set_except_vector(int n, void *addr);
asmlinkage void do_reserved(struct pt_regs *regs);
void __init exception_init(void);

unsigned long ebase;

static struct irqaction *irq_action[NR_IRQS] =
{
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};


#define ALLINTS (IE_SW0 | IE_SW1 | IE_IRQ0 | IE_IRQ1 | IE_IRQ2 | IE_IRQ3 | IE_IRQ4 | IE_IRQ5)


void intPrioritySet(uint8 source, uint8 level)
{
    uint32 IPRn,IPLn;
    uint32 word;
    
    IPRn=level/4;
    IPLn=level%4;
    word = tc_inl(CR_INTC_IPR0+IPRn*4);
    word |= source << ((3-IPLn)*8);
    tc_outl(CR_INTC_IPR0+IPRn*4, word);
}

void set_irq_priority(void)
{
#ifdef TC3262
	unsigned int IntPriority[32]={
		// UART_INT, RTC_ALARM_INT, RTC_TICK_INT, RESERVED0, TIMER0_INT
		IPL9,	IPL7,	IPL22,	IPL21,	IPL30,		
		// TIMER1_INT, TIMER2_INT, TIMER3_INT, TIMER4_INT, TIMER5_INT
		IPL29,	IPL17,	IPL19,	IPL10,	IPL1,
		// GPIO_INT, RESERVED1, RESERVED2, RESERVED3, APB_DMA0_INT
		IPL11,	IPL12,	IPL13,	IPL14,	IPL18,
		// APB_DMA1_INT, RESERVED4, RESERVED5, DYINGGASP_INT, DMT_INT
		//IPL15,	IPL16,	IPL17,	IPL0,	IPL3,
		IPL15,	IPL16,	IPL8,	IPL20,	IPL4,
		// ARBITER_ERR_INT, MAC_INT, SAR_INT, USB_INT, RESERVED8
		IPL31,	IPL6,	IPL5,	IPL23,	IPL24, 
		// RESERVED9, XSLV0_INT, XSLV1_INT, XSLV2_INT, XAPB0_INT
		IPL25,	IPL26,	IPL27,	IPL28,	IPL3,
		// XAPB1_INT, SWR_INT
		//IPL1,	IPL20	
		IPL2,	IPL0	
	};
#else
	unsigned int IntPriority[32]={
		// UART_INT, RTC_ALARM_INT, RTC_TICK_INT, RESERVED0, TIMER0_INT
		IPL5,	IPL19,	IPL22,	IPL21,	IPL7,		
		// TIMER1_INT, TIMER2_INT, TIMER3_INT, TIMER4_INT, TIMER5_INT
		IPL1,	IPL6,	IPL8,	IPL9,	IPL10,
		// GPIO_INT, RESERVED1, RESERVED2, RESERVED3, APB_DMA0_INT
		IPL11,	IPL12,	IPL13,	IPL14,	IPL18,
		// APB_DMA1_INT, RESERVED4, RESERVED5, DYINGGASP_INT, DMT_INT
		IPL15,	IPL16,	IPL17,	IPL0,	IPL2,
		// ARBITER_ERR_INT, MAC_INT, SAR_INT, USB_INT, RESERVED8
		IPL31,	IPL4,	IPL3,	IPL23,	IPL24, 
		// RESERVED9, XSLV0_INT, XSLV1_INT, XSLV2_INT, XAPB0_INT
		IPL25,	IPL26,	IPL27,	IPL28,	IPL29,
		// XAPB1_INT, SWR_INT
		IPL30,	IPL20	
	};
#endif
	int i;	

	tc_outl(CR_INTC_IPR0, 0);
	tc_outl(CR_INTC_IPR1, 0);
	tc_outl(CR_INTC_IPR2, 0);
	tc_outl(CR_INTC_IPR3, 0);
	tc_outl(CR_INTC_IPR4, 0);
	tc_outl(CR_INTC_IPR5, 0);
	tc_outl(CR_INTC_IPR6, 0);
	tc_outl(CR_INTC_IPR7, 0);

	for (i = 0; i < 32; i++)
		intPrioritySet(i, IntPriority[i]);
}

static void  unmask_irq(unsigned int irq)
{
    tc_outl(CR_INTC_IMR, (tc_inl(CR_INTC_IMR) | (1 << irq)));
}
static void  mask_irq(unsigned int irq)
{
    tc_outl(CR_INTC_IMR, tc_inl(CR_INTC_IMR) & (~(1 << irq)));
}

extern asmlinkage void do_IRQ(int irq, struct pt_regs *regs);

void enable_irq(unsigned int irq)
{
}

static unsigned int startup_irq(unsigned int irq)
{
   unmask_irq(irq);
   return 0;		/* Never anything pending  */
}

void disable_irq(unsigned int irq)
{
}

#define shutdown_irq		disable_irq

static void mask_and_ack_irq(unsigned int irq)
{
}
static void end_irq(unsigned int irq)
{
}

void show_regs(struct pt_regs * regs)
{
	int i;
	
	prom_printf("\n");

	prom_printf(" lo=%x hi=%x\n", regs->lo, regs->hi);
	prom_printf(" cp0_epc=%x cp0_badvaddr=%x\n", regs->cp0_epc, regs->cp0_badvaddr);
	prom_printf(" cp0_status=%x cp0_cause=%x\n", regs->cp0_status, regs->cp0_cause);

	prom_printf("pad0\n");
	for (i = 0; i < 6; i++) {
		if ((i % 4) == 0)
			prom_printf("\n");
		prom_printf(" 0x%x", regs->pad0[i]);
	}
	prom_printf("\nregs\n");
	for (i = 0; i < 32; i++) {
		if ((i % 4) == 0)
			prom_printf("\n");
		prom_printf(" %x", regs->regs[i]);
	}
	prom_printf("\n");
}

void irq_dispatch(int irq_nr, struct pt_regs *regs)
{
	do_IRQ(irq_nr, regs);
}

inline unsigned int					
clear_cp0_status(unsigned int clear)				
{								
	unsigned int res;					
								
	res = read_32bit_cp0_register(CP0_STATUS);		
	res &= ~clear;						
	write_32bit_cp0_register(CP0_STATUS, res);									
}								

inline unsigned int					
change_cp0_status(unsigned int change, unsigned int newvalue)	
{								
	unsigned int res;					
								
	res = read_32bit_cp0_register(CP0_STATUS);		
	res &= ~change;						
	res |= (newvalue & change);					
	write_32bit_cp0_register(CP0_STATUS, res);		
							
	return res;						
}


void __init ExceptionToIrq_setup(void)
{
	extern asmlinkage void IRQ_finder(void);

	unsigned int i;

	/* Disable all hardware interrupts */
	change_cp0_status(ST0_IM, 0x00);

	/* Set up the external interrupt exception vector */
	/* First exception is Interrupt*/
	set_except_vector(0, IRQ_finder);

	/* Enable all interrupts */
	change_cp0_status(ST0_IM, ALLINTS);
#ifdef TC3262
	change_cp0_status(ST0_CU0 | ST0_IE, ST0_CU0 | ST0_IE);
#endif
}


void __init init_IRQ(void)
{
	set_irq_priority();
	ExceptionToIrq_setup();
}


// below is adopted from kernel/irq.c


int setup_IRQ(int irq, struct irqaction *new)
{
    int shared = 0;
    struct irqaction *old, **p;
    unsigned long flags;

    p = irq_action + irq;
    
    save_and_cli(flags);
    *p = new;
    
    restore_flags(flags);
    
    return 0;
}

int request_IRQ(unsigned long irq, struct irqaction *action, void* dev_id)
{

    int retval;
 
    if (irq >= NR_IRQS)
		return -EINVAL;

	action->dev_id = dev_id;
    retval = setup_IRQ(irq, action);
	unmask_irq(irq);

    if (retval)
	    return retval;
}


int	free_IRQ(unsigned long irq)
{
	mask_irq(irq);	
	return 0;
}


asmlinkage void do_IRQ(int irqnr, struct pt_regs *regs)
{
    struct irqaction *action;
	unsigned long i;

	if (irqnr > 32)
		goto bad_irq;

    action = *(irqnr + irq_action);
        	
	if (action) 
    {
	    action->handler(irqnr, action->dev_id, regs);
    }
	else
	{    
bad_irq:
		;
#if 0
		prom_printf("you got irq=%X\n", irqnr);
		prom_printf("0xbfb40050=%08x\n", VPint(0xbfb40050));
		do_reserved(regs);
#endif
	}			
}	

unsigned long exception_handlers[32];

void *set_except_vector(int n, void *addr)
{
	unsigned handler = (unsigned long) addr;
	unsigned old_handler = exception_handlers[n];
	exception_handlers[n] = handler;
	return (void *)old_handler;
}

asmlinkage void do_reserved(struct pt_regs *regs)
{
	/*fatal hard/software error*/
	int i;
	unsigned long s;

	prom_printf("Undefined Exception happen.");

	s = read_32bit_cp0_register(CP0_STATUS);
	prom_printf("CP0_STATUS=%x\n", s);
	s = read_32bit_cp0_register(CP0_CAUSE);
	prom_printf("CP0_CAUSE=%x\n", s);
	s = read_32bit_cp0_register(CP0_EPC);
	prom_printf("CP0_EPC=%x\n", s);
	s = read_32bit_cp0_register(CP0_BADVADDR);
	prom_printf("CP0_BADVADDR=%x\n", s);

	show_regs(regs);
	
	for(;;);
	/*Just hang here.*/
}

void __init exception_init(void)
{
	extern char except_vec3_generic;
	
	unsigned long i;
#ifdef TC3262
#ifdef BOOTROM_IN_SRAM
	ebase = 0x9fa30000;
#else
	ebase = KSEG0;
#endif

	write_c0_ebase (ebase);
	/* set IV=0 */
	change_c0_cause ((1<<23), 0);
	/* Setting vector spacing to zero */
	change_c0_intctl (0x3e0, 0);
#else
	ebase = KSEG0;
#endif

	/* Some firmware leaves the BEV flag set, clear it.  */
	clear_cp0_status(ST0_BEV);

#ifdef TC3262
	/* Copy the generic exception handler code to it's final destination. */
	memcpy((void *)(ebase + 0x180), &except_vec3_generic, 0x80);
#else
	/* Copy the generic exception handler code to it's final destination. */
	memcpy((void *)(ebase + 0x80), &except_vec3_generic, 0x80);
#endif
	
	/*
	 * Setup default vectors
	 */
	for (i = 0; i <= 31; i++)
		set_except_vector(i, do_reserved);

#ifdef TC3262
	flush_icache_range(ebase + 0x180, ebase + 0x180 + 0x80);
#else
	flush_icache();
#endif
}

