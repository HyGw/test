#ifndef __FLASH_H__
#define __FLASH_H__

extern int amdflash_init(unsigned long rom_base);
extern int amdflash_read(unsigned long from,
	unsigned long len, unsigned long *retlen, unsigned char *buf);
extern int amdflash_write(unsigned long to,
	unsigned long len, unsigned long *retlen, const unsigned char *buf);
extern int amdflash_erase(unsigned long addr, unsigned long size);

#endif /* __FLASH_H__ */
