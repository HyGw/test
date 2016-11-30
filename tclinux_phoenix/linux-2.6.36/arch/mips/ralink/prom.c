#include <linux/init.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/bootmem.h>
#include <linux/blkdev.h>

#include <asm/mipsmtregs.h>
#include <asm/addrspace.h>
#include <asm/bootinfo.h>
#include <asm/cpu.h>
#include <asm/time.h>
#include <asm/tc3162/tc3162.h>
#include <asm/traps.h>

#ifdef TCSUPPORT_INIC_CLIENT
#include <linux/mtd/fttdp_inic.h>
#endif

extern int __imem, __dmem;

/* frankliao added 20101215 */
unsigned long flash_base;
EXPORT_SYMBOL(flash_base);
unsigned int (*ranand_read_byte)(unsigned long long) = NULL;
EXPORT_SYMBOL(ranand_read_byte);
unsigned int (*ranand_read_dword)(unsigned long long) = NULL;
EXPORT_SYMBOL(ranand_read_dword);

#ifdef CONFIG_MIPS_TC3262
unsigned char io_swap_noneed=0;
EXPORT_SYMBOL(io_swap_noneed);
#endif

static void tc3162_component_setup(void)
{
	unsigned int controlReg;
	unsigned long flags;

	/* setup bus timeout value */
	VPint(CR_AHB_AACS) = 0xffff;

	/* reset hwnat */
	if (isRT65168) {
		/* table reset */
		VPint(0xbfbe0024) = 0x0;
		VPint(0xbfbe0024) = 0xffff;
		
		/* hwnat swreset */
		VPint(0xbfbe0000) = (1<<1);
	}

#ifdef CONFIG_CPU_TC3162
#ifdef CONFIG_TC3162_IMEM
	/* setup imem start address */
	VPint(CR_IMEM) = CPHYSADDR(&__imem);

	/* clear internal imem */
	local_irq_save(flags);
	controlReg = read_c0_cctl();
	write_c0_cctl(controlReg & ~CCTL_IMEMOFF);
	write_c0_cctl(controlReg | CCTL_IMEMOFF);
	write_c0_cctl(controlReg);
	local_irq_restore(flags);

	/* refill internal imem */
	local_irq_save(flags);
	controlReg = read_c0_cctl();
	write_c0_cctl(controlReg & ~CCTL_IMEMFILL4);
	write_c0_cctl(controlReg | CCTL_IMEMFILL4);
	write_c0_cctl(controlReg);
	local_irq_restore(flags);
	
	printk("Enable IMEM addr=%x\n", CPHYSADDR(&__imem));
#endif

#ifdef CONFIG_TC3162_DMEM
	/* setup dmem start address */
	VPint(CR_DMEM) = CPHYSADDR(&__dmem);

	memcpy((void *) 0xa0001000, (void *) KSEG1ADDR(&__dmem), 0x800);

	/* clear internal dmem */
	local_irq_save(flags);
	controlReg = read_c0_cctl();
	write_c0_cctl(controlReg & ~CCTL_DMEMOFF);
	write_c0_cctl(controlReg | CCTL_DMEMOFF);
	write_c0_cctl(controlReg);
	local_irq_restore(flags);

	/* internal dmem on */
	local_irq_save(flags);
	controlReg = read_c0_cctl();
	write_c0_cctl(controlReg & ~CCTL_DMEMON);
	write_c0_cctl(controlReg | CCTL_DMEMON);
	write_c0_cctl(controlReg);
	local_irq_restore(flags);

	printk("Enable DMEM addr=%x\n", CPHYSADDR(&__dmem));

	memcpy((void *) KSEG1ADDR(&__dmem), (void *) 0xa0001000, 0x800);
#endif
#endif
}

/* frankliao added 20101215 */
void flash_init(void)
{

	if ((IS_NANDFLASH) && (isRT63165 || isRT63365 || isMT751020 || isEN751221)) {
		flash_base = 0x0;
	} else {
#ifdef TCSUPPORT_ADDR_MAPPING
		if(isMT751020 || isMT7505 || isEN751221){
			#ifdef TCSUPPORT_INIC_CLIENT
			/* To use last INIC_CLIENT_RAM_SIMU_OFFSET size of RAM as flash */
			flash_base = 0xA0000000 + (0x800000 * (1 << (((VPint(0xbfb0008c) >> 13) & 0x7) - 1))- INIC_CLIENT_RAM_SIMU_MAX_SIZE);
			#else
			flash_base = 0xbc000000;
			#endif
			printk("%s: flash_base:%x \n",__func__,flash_base);
		}
		else if (isTC3162U || isRT63260 || isRT65168 || isTC3182 || isRT63165 || isRT63365)
#else
		if (isTC3162U || isRT63260 || isRT65168 || isTC3182 || isRT63165 || isRT63365 || isMT751020 || isMT7505 || isEN751221)
#endif		
			flash_base = 0xb0000000;
		else
			flash_base = 0xbfc00000;
		printk("%s: flash_base:%x \n",__func__,flash_base);
	}
}

const char *get_system_type(void)
{
#ifdef CONFIG_MIPS_TC3262
	if(isEN751221){
		io_swap_noneed = 1;
		return "EcoNet EN751221 SOC";	
	}else if (isTC3182)
		return "TrendChip TC3182 SOC";
	else if (isRT65168)
		return "Ralink RT65168 SOC";
	else if (isRT63165){
		io_swap_noneed = 1;
		return "Ralink RT63165 SOC";
	} else if (isRT63365) {
		io_swap_noneed = 1;
#ifdef TCSUPPORT_DYING_GASP		
		if(!isRT63368){
			//gpio 4 is share pin for rt63365.
			VPint(0xbfb00860) &= ~(1<<13);//disable port 4 led when use rt63365.
		}
#endif		
		return "Ralink RT63365 SOC";
	}else if (isMT751020){
		io_swap_noneed = 1;
		return "Ralink MT751020 SOC";
	}else if (isMT7505){
		io_swap_noneed = 1;
		return "Ralink MT7505 SOC";
	}
	else
		return "TrendChip TC3169 SOC";
#else
	if (isRT63260)	
		return "Ralink RT63260 SOC";
	else if (isTC3162U)
		return "TrendChip TC3162U SOC";
	else if (isTC3162L5P5)
		return "TrendChip TC3162L5/P5 SOC";
	else if (isTC3162L4P4)
		return "TrendChip TC3162L4/P4 SOC";	
	else if (isTC3162L3P3)
		return "TrendChip TC3162L2F/P2F";
	else if (isTC3162L2P2)
		return "TrendChip TC3162L2/P2";
	else 
		return "TrendChip TC3162";
#endif
}

extern struct plat_smp_ops msmtc_smp_ops;
#define VECTORSPACING 0x100	/* for EI/VI mode */


void __init mips_nmi_setup (void)
{
	void *base;
	extern char except_vec_nmi;
	#if 0
	base = cpu_has_veic ?
		(void *)(CAC_BASE + 0xa80) :
		(void *)(CAC_BASE + 0x380);
	#endif

	base = cpu_has_veic ?
		(void *)(ebase + 0x200 + VECTORSPACING*64) :
		(void *)(ebase + 0x380);
		
	printk("nmi base is %x\n",base);

	//Fill the NMI_Handler address in a register, which is a R/W register
	//start.S will read it, then jump to NMI_Handler address
	VPint(0xbfb00244) = base;
	
	memcpy(base, &except_vec_nmi, 0x80);
	flush_icache_range((unsigned long)base, (unsigned long)base + 0x80);
}

void cpu_dma_round_robin(uint8 mode)
{
    uint32 reg_value=0;
    reg_value = VPint(ARB_CFG);
    if(mode == ENABLE){
        reg_value |= ROUND_ROBIN_ENABLE;
    } else {
        reg_value &= ROUND_ROBIN_DISBALE;
    }
    VPint(ARB_CFG) = reg_value;
}


void __init prom_init(void)
{
	unsigned long memsize;
	unsigned char samt;
	unsigned long col;
	unsigned long row;

	/* frankliao added 20101222 */
	flash_init();

#ifdef CONFIG_MIPS_TC3262
	if (isRT63165 || isRT63365 || isMT751020 || isMT7505 || isEN751221) {
		/* enable external sync */
#ifdef TCSUPPORT_IS_FH_PON			
		strcat(arcs_cmdline, "rootfstype=jffs2 ro init=/etc/preinit.sh");
#endif

#if defined(TCSUPPORT_FWC_ENV)
        strcat(arcs_cmdline, "rootfstype=jffs2 ro");
#endif

		strcat(arcs_cmdline, " es=1");

#ifndef CONFIG_SMP
	{
		/* when kernel is UP, set ES=1. Otherwise, set in mips_mt_set_cpuoptions */
		unsigned int oconfig7 = read_c0_config7();
		unsigned int nconfig7 = oconfig7;

		nconfig7 |= (1 << 8);

		__asm__ __volatile("sync");
		write_c0_config7(nconfig7);
		ehb();
		printk("Config7: 0x%08x\n", read_c0_config7());
	}
#endif
	}

	if(isMT751020){
		memsize = 0x800000 * (1 << (((VPint(0xbfb0008c) >> 13) & 0x7) - 1));	
		if(memsize >= (448<<20)){
			memsize = (448<<20);
		}
		printk("memsize:%dMB\n", (memsize>>20));
	}
	else if(isMT7505){
		if(isFPGA)
			memsize = 0x800000 * (1 << (((VPint(0xbfb0008c) >> 13) & 0x7) - 1));
		else{
			if(VPint(CR_AHB_HWCONF) & (1<<10))
			{
				/* DDR1 */
				memsize = 0x4000000 / (1 << ((VPint(CR_AHB_HWCONF) >> 11) & 0x3));
			}
			else{
				/* DDR2 */
				if(!((VPint(CR_AHB_HWCONF) >> 11) & 0x3))
					memsize = 256 * 0x100000;
				if(((VPint(CR_AHB_HWCONF) >> 11) & 0x3) == 0x1)
					memsize = 32 * 0x100000;
				if(((VPint(CR_AHB_HWCONF) >> 11) & 0x3) == 0x2)
					memsize =  128 * 0x100000;
				else
					memsize = 64 * 0x100000;
			}
		}
		printk("memsize:%dMB\n", (memsize>>20));
		
	}else if(isEN751221){
		if(isFPGA){
                        memsize = 0x800000 * (1 << (((VPint(0xbfb0008c) >> 13) & 0x7) - 1));
		}
 		else{
			memsize = GET_DRAM_SIZE;
			if(memsize == 512) {
				memsize = 440; /* always reserve 8M to QDMA HW when memsize if 512MByte */
			}
#ifndef TCSUPPORT_SLM_EN
			else {
				memsize = memsize-8; /* without SLM, we should reserve 8M to QDMA HW */
			}
#endif
			memsize = memsize << 20;
		}
		printk("memsize:%dMB\n", (memsize>>20));
	}else if (isRT63165 || isRT63365) {
		/* DDR */
		if (VPint(CR_AHB_HWCONF) & (1<<25)) {
			memsize = 0x800000 * (1 << (((VPint(CR_DMC_DDR_CFG1) >> 18) & 0x7) - 1));

		/* SDRAM */
		} else {
			unsigned long sdram_cfg1;
			
			/* calculate SDRAM size */
			sdram_cfg1 = VPint(0xbfb20004);
			row = 11 + ((sdram_cfg1>>16) & 0x3);
			col = 8 + ((sdram_cfg1>>20) & 0x3);
			/* 4 bands and 16 bit width */
			memsize = (1 << row) * (1 << col) * 4 * 2;
		}
	} else {
		memsize = 0x800000 * (1 << (((VPint(CR_DMC_CTL1) >> 18) & 0x7) - 1));
	}
#else
	/* calculate SDRAM size */
	samt = VPchar(CR_DMC_SAMT);
	row = 8 + (samt & 0x3);
	col = 11 + ((samt>>2) & 0x3);
	/* 4 bands and 16 bit width */
	memsize = (1 << row) * (1 << col) * 4 * 2;
#endif

	printk("%s prom init\n", get_system_type());

	tc3162_component_setup();

	#ifdef TCSUPPORT_INIC_CLIENT
	add_memory_region(0 + 0x20000, memsize - 0x20000 - INIC_CLIENT_RAM_SIMU_MAX_SIZE, BOOT_MEM_RAM);
	#else
	add_memory_region(0 + 0x20000, memsize - 0x20000, BOOT_MEM_RAM);
	#endif
	if (isMT751020 || isMT7505 || isEN751221) {
		board_nmi_handler_setup = mips_nmi_setup;
	}

	//mips_machgroup = MACH_GROUP_TRENDCHIP;
	//mips_machtype = MACH_TRENDCHIP_TC3162;

#ifdef CONFIG_MIPS_MT_SMP
	register_smp_ops(&vsmp_smp_ops);
#endif
#ifdef CONFIG_MIPS_MT_SMTC
	register_smp_ops(&msmtc_smp_ops);
#endif

#if !defined(TCSUPPORT_XPON_HAL_API_EXT) 
	if(isMT751020 && IS_NANDFLASH){
		cpu_dma_round_robin(ENABLE);
	}
#endif

    if(isEN751221)
		cpu_dma_round_robin(ENABLE);
}

void __init prom_free_prom_memory(void)
{
	/* We do not have any memory to free */
}

int prom_putchar(char data)
{
	while (!(LSR_INDICATOR & LSR_THRE))
		;
	VPchar(CR_UART_THR) = data; 
	return 1;
}
EXPORT_SYMBOL(prom_putchar);

char prom_getchar(void)
{
	while (!(LSR_INDICATOR & LSR_RECEIVED_DATA_READY))
		;
	return VPchar(CR_UART_RBR);
}

static char ppbuf[1024];

void
prom_write(const char *buf, unsigned int n)
{
	char ch;

	while (n != 0) {
		--n;
		if ((ch = *buf++) == '\n')
			prom_putchar('\r');
		prom_putchar(ch);
	}
}
EXPORT_SYMBOL(prom_write);

void
prom_printf(const char *fmt, ...)
{
	va_list args;
	int i;

	va_start(args, fmt);
	i = vscnprintf(ppbuf, sizeof(ppbuf), fmt, args);
	va_end(args);

	prom_write(ppbuf, i);
}
EXPORT_SYMBOL(prom_printf);

#ifdef CONFIG_KGDB
static unsigned long  uclk_65000[13]={
	357500, 	// uclk 5.5     Baud Rate 115200
	175500, 	// uclk 2.7     Baud Rate 57600
	119808, 	// uclk 1.8432  Baud Rate 38400
	89856,	// uclk	1.3824	Baud Rate 28800
	59904,	// uclk 0.9216	Baud Rate 19200
	44928,	// uclk 0.6912	Baud Rate 14400
	29952,	// uclk 0.4608	Baud Rate 9600
	14976,	// uclk 0.2304	Baud Rate 4800
	7488,	// uclk 0.1152	Baud Rate 2400 
	3744,	// uclk 0.0576	Baud Rate 1200
	1872,	// uclk 0.0288	Baud Rate 600
	936,		// uclk 0.0144	Baud Rate 300
	343		// uclk 0.00528	Baud Rate 110
};

static void hsuartInit(void)
{
	unsigned long	div_x,div_y;
	unsigned long	word;
	unsigned long   tmp;

	tmp = VPint(CR_GPIO_CTRL);
	tmp &= ~0x0fa30000;
	tmp |= 0x0fa30000;
	VPint(CR_GPIO_CTRL) = tmp; // set GPIO pin 13 & pin 12 are alternative outputs, GPIO pin 11 & pin 10 are alternative inputs
	tmp = VPint(CR_GPIO_ODRAIN);
	tmp &= ~0x00003000;
	tmp |= 0x00003000;
	VPint (CR_GPIO_ODRAIN) = tmp; // set GPIO output enable

// Set FIFO controo enable, reset RFIFO, TFIFO, 16550 mode, watermark=0x00 (1 byte)
	VPchar(CR_HSUART_FCR) = UART_FCR|UART_WATERMARK;

// Set modem control to 0
	VPchar(CR_HSUART_MCR) = UART_MCR;

// Disable IRDA, Disable Power Saving Mode, RTS , CTS flow control
	VPchar(CR_HSUART_MISCC) = UART_MISCC;

	/* access the bardrate divider */
	VPchar(CR_HSUART_LCR) = UART_BRD_ACCESS;

	div_y = UART_XYD_Y;
	div_x = (unsigned int)(uclk_65000[0]/SYS_HCLK)*2;
	word = (div_x<<16)|div_y;
	VPint(CR_HSUART_XYD) = word;

/* Set Baud Rate Divisor to 3*16		*/
	VPchar(CR_HSUART_BRDL) = UART_BRDL;
	VPchar(CR_HSUART_BRDH) = UART_BRDH;

/* Set DLAB = 0, clength = 8, stop =1, no parity check 	*/
	VPchar(CR_HSUART_LCR) = UART_LCR;

// Set interrupt Enable to, enable Tx, Rx and Line status
	VPchar(CR_HSUART_IER) = UART_IER;	
}

static int hsuartInitialized = 0;

int putDebugChar(char c)
{
	if (!hsuartInitialized) {
		hsuartInit();
		hsuartInitialized = 1;
	}

	while (!(VPchar(CR_HSUART_LSR) & LSR_THRE))
		;
	VPchar(CR_HSUART_THR) = c; 

	return 1;
}

char getDebugChar(void)
{
	if (!hsuartInitialized) {
		hsuartInit();
		hsuartInitialized = 1;
	}

	while (!(VPchar(CR_HSUART_LSR) & LSR_RECEIVED_DATA_READY))
		;
	return VPchar(CR_HSUART_RBR);
}
#endif
#if defined(TCSUPPORT_DYING_GASP) && (defined(CONFIG_MIPS_RT65168) || defined(CONFIG_MIPS_RT63365))
__IMEM
void dying_gasp_setup_mem_cpu(void){
#ifdef CONFIG_MIPS_RT65168	
		VPint(0xbfb20000) |= (1<<12); //set ddr to self refresh mode. 
		VPint(0xbfb000c0) &= ~((1<<5)|(1<<6)|(1<<7));//CPU divide to 32 and ram divide to 3
		VPint(0xbfb000c0) |= (1<<3)|(1<<4)|(1<<5)|(1<<7);
#endif
#ifdef CONFIG_MIPS_RT63365	
#if defined(TCSUPPORT_CPU_MT7510)|| defined(TCSUPPORT_CPU_MT7520) || defined(TCSUPPORT_CPU_MT7505) || defined(TCSUPPORT_CPU_EN7512)
#ifdef TCSUPPORT_CPU_EN7512               
                  /*FDIV down freq*/
                   VPint(0xbfa200d0) = 0x2000; /*FNDIV, clock /2*/
                   VPint(0xbfa20148) = 0xffff800c; 
                   VPint(0xbfa20148) = 0xffff000c; 
#endif
		VPint(0xbfb00044) = 1; //Enable DDR Self Refresh Mode
		VPint(0xbfb20004) &= ~(1<<15);
		VPint(0xbfb200e4) &= ~(1<<2);
		VPint(0xbfb00074) |= (1<<4);
		VPint(0xbfb00040) |= (1<<0); // reset ddr device
#else 	
		VPint(0xbfb00040) |= (1<<0); // reset ddr device
		//do not kill CPU because we need do watchdog interrupt
		//kill CPU
		//VPint(0xbfb001c8) |= (1<<24); // bypass pll 2 700M 	
		//VPint(0xbfb001cc) |= (1<<24); // bypass pll 2 665M	
		//VPint(0xbfb001d0) |= (1<<24); // bypass pll 2 500
#endif
#endif
	if (cpu_wait)
		(*cpu_wait)();
}
EXPORT_SYMBOL(dying_gasp_setup_mem_cpu);
#endif
