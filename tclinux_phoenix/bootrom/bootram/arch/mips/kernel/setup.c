#include <asm/asm.h>
#include <asm/bootinfo.h>
#include <asm/cachectl.h>
#include <asm/io.h>
#include <asm/stackframe.h>
#include <asm/system.h>
#include <asm/cpu.h>
#include <asm/mipsregs.h>
#include <asm/tc3162.h>

#if 0
void hw_init(void)
{
	int i;

    /* lino: stop each module dma task */

	/* stop mac dma */
	tc_outl(CR_MAC_MACCR, 0);

	/* stop atm sar dma */	
	tc_outl(TSARM_GFR, tc_inl(TSARM_GFR) & ~((1 << 1) | (1 << 0)));

	/* reset USB */
	/* reset USB DMA */
	tc_outl(CR_USB_SYS_CTRL_REG, tc_inl(CR_USB_SYS_CTRL_REG) | (1 << 31));
	/* reset USB SIE */
	tc_outl(CR_USB_SYS_CTRL_REG, tc_inl(CR_USB_SYS_CTRL_REG) | (1 << 30));

	for (i = 0; i < 2000; i++)
		;

	/* restore USB SIE */
	tc_outl(CR_USB_SYS_CTRL_REG, tc_inl(CR_USB_SYS_CTRL_REG) & ~(1 << 30));

	for (i = 0; i < 2000; i++)
		;

	tc_outl(CR_USB_SYS_CTRL_REG, tc_inl(CR_USB_SYS_CTRL_REG) & ~(1 << 31));
}
#endif

asmlinkage void init_arch(int argc, char **argv, char **envp, int *prom_vec)
{
#ifdef LZMA_IMG
	start_lzma();
#elif defined(SPRAM_IMG)
	start_spram();
#else
	//hw_init();
	start_kernel();
#endif
}


void  setup_arch(void)
{
	return ;
}

