#ifndef __MTD_RT_FLASH_H__
#define __MTD_RT_FLASH_H__

#include <asm/tc3162/tc3162.h>
#ifndef VPint
#define VPint			*(volatile unsigned long int *)
#endif

#ifdef TCSUPPORT_NEW_SPIFLASH
extern unsigned char ReadSPIByte(unsigned long index);
extern unsigned long ReadSPIDWord(unsigned long index);
#else
#ifdef TCSUPPORT_MT7510_E1
#define ReadSPIByte(i) (((*((unsigned char*)i))==0) ? (*((unsigned char*)i)): (*((unsigned char*)i)))
#define ReadSPIDWord(i) (((*((unsigned int*)i))==0) ? (*((unsigned int*)i)): (*((unsigned int*)i)))
#else
#define ReadSPIByte(i) (*((unsigned char*)i))
#define ReadSPIDWord(i) (*((unsigned int*)i))
#endif
#endif
#if TCSUPPORT_MT7510_E1
#define READ_FLASH_BYTE(i)  	( (IS_NANDFLASH) ? \
								((ranand_read_byte != NULL) ? ranand_read_byte((i)) : -1) \
								: (ReadSPIByte(i)) )


/* for read flash, frankliao added 20101216 */
#define READ_FLASH_DWORD(i)  ( (IS_NANDFLASH) ? \
								((ranand_read_dword != NULL) ? ranand_read_dword((i)) : -1) \
								: (ReadSPIDWord(i)) )
#else
#define READ_FLASH_BYTE(i)  	( (IS_NANDFLASH) ? \
								((ranand_read_byte != NULL) ? ranand_read_byte((i)) : -1) \
								: (ReadSPIByte(i)) )


/* for read flash, frankliao added 20101216 */
#define READ_FLASH_DWORD(i)  ( (IS_NANDFLASH) ? \
								((ranand_read_dword != NULL) ? ranand_read_dword((i)) : -1) \
								: (ReadSPIDWord(i)) )

#endif
/* frankliao added 20101215 */
extern unsigned long flash_base;
extern unsigned int (*ranand_read_byte)(unsigned long long);
extern unsigned int (*ranand_read_dword)(unsigned long long);

#endif /* __MTD_MTD_RT_FLASH_H__ */
