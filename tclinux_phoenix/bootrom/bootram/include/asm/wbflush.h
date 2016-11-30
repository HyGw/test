/*
 * Header file for using the wbflush routine
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (c) 1998 Harald Koerfgen
 *
 * $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/bootrom/bootram/include/asm/wbflush.h#1 $
 */
#ifndef __ASM_MIPS_WBFLUSH_H
#define __ASM_MIPS_WBFLUSH_H

#include <linux/config.h>

#if defined(CONFIG_CPU_HAS_WB)
/*
 * R2000 or R3000
 */
extern void (*__wbflush) (void);

#define wbflush() __wbflush()

#else
/*
 * we don't need no stinkin' wbflush
 */

#define wbflush()  do { } while(0)

#endif

extern void wbflush_setup(void);

#endif /* __ASM_MIPS_WBFLUSH_H */
