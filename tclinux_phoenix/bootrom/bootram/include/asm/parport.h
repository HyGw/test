/* $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/bootrom/bootram/include/asm/parport.h#1 $
 *
 * Copyright (C) 1999, 2000  Tim Waugh <tim@cyberelk.demon.co.uk>
 *
 * This file should only be included by drivers/parport/parport_pc.c.
 */
#ifndef _ASM_PARPORT_H
#define _ASM_PARPORT_H

static int __devinit parport_pc_find_isa_ports (int autoirq, int autodma);
static int __devinit parport_pc_find_nonpci_ports (int autoirq, int autodma)
{
	return parport_pc_find_isa_ports (autoirq, autodma);
}

#endif /* _ASM_PARPORT_H */
