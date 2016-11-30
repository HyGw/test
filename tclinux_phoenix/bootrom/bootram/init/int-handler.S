
#include <asm/asm.h>
#include <asm/regdef.h>
#include <asm/mipsregs.h>
#include <asm/stackframe.h>
#include <asm/addrspace.h>

#define	CR_INTC_IVR		0xBFB40030
#define	CR_INTC_IMR		0xBFB40004

		.text
		.set	noreorder
		.align	5
		NESTED(IRQ_finder, PT_SIZE, sp)
		.set	noat
		SAVE_ALL
		CLI				
		.set	at
		.set	noreorder

#ifdef TC3262
		mfc0	a0, CP0_CAUSE
		andi	a0, a0, 0xff00
		srl 	a0, a0, 10
		subu	a0, a0, 1
#else
		la		t0,	CR_INTC_IVR
        lw      a0, 0(t0)  	  
        nop
#endif
		jal 	irq_dispatch       
        move   	a1, sp
        
		RESTORE_ALL_AND_RET
		nop
		END(IRQ_finder)

		.set    at
