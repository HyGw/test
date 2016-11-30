/*
 * linux/include/asm-mips/namei.h
 *
 * Included from linux/fs/namei.c
 *
 * $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/bootrom/bootram/include/asm/namei.h#1 $
 */
#ifndef __ASM_MIPS_NAMEI_H
#define __ASM_MIPS_NAMEI_H

#include <linux/config.h>

/* Only one at this time. */
#define IRIX32_EMUL "usr/gnemul/irix/"

#ifdef CONFIG_BINFMT_IRIX

static inline char *__emul_prefix(void)
{
	if (current->personality != PER_IRIX32)
		return NULL;
	return IRIX32_EMUL;
}

#else /* !defined(CONFIG_BINFMT_IRIX) */

#define __emul_prefix() NULL

#endif /* !defined(CONFIG_BINFMT_IRIX) */

#endif /* __ASM_MIPS_NAMEI_H */
