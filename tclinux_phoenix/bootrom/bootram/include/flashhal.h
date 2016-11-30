#ifndef __FLASHHAL_H__
#define __FLASHHAL_H__

#ifndef VPint
#define VPint			*(volatile unsigned int *)
#endif

#ifndef CR_AHB_HWCONF
#define CR_AHB_HWCONF       (0xbfb0008c)
#endif

#if defined(TCSUPPORT_CPU_EN7512)||defined(TCSUPPORT_CPU_EN7521)
#include "controller/spi_controller/spi_controller.h"
extern unsigned char SPI_NAND_Flash_Read_Byte(unsigned long addr);
extern unsigned long SPI_NAND_Flash_Read_DWord(unsigned long addr);
#endif

extern int flash_init(unsigned long rom_base);
extern int flash_read(unsigned long from,
	unsigned long len, unsigned long *retlen, unsigned char *buf);
extern int flash_write(unsigned long to,
	unsigned long len, unsigned long *retlen, const unsigned char *buf);
extern int flash_erase(unsigned long addr, unsigned long size);

extern unsigned long flash_base;
extern unsigned long flash_tclinux_start;

extern unsigned char ReadNandCache(unsigned long index);
extern unsigned char ReadNandByte(unsigned long index);
extern unsigned long ReadNandDWord(unsigned long index);

#ifdef TCSUPPORT_NEW_SPIFLASH
extern unsigned char ReadSPICache(unsigned long index);
extern unsigned char ReadSPIByte(unsigned long index);
extern unsigned long ReadSPIDWord(unsigned long index);
#else
#define ReadSPICache(i) (*((unsigned char*)i))
#define ReadSPIByte(i) (*((unsigned char*)i))
#define ReadSPIDWord(i) (*((unsigned int*)i))
#endif



#if defined(TCSUPPORT_CPU_EN7512)||defined(TCSUPPORT_CPU_EN7521)
#define IS_SPIFLASH				((~(VPint(SPI_CONTROLLER_REGS_STRAP))) & 0x2)
#define NANDFLASH_HWTRAP		 (VPint(SPI_CONTROLLER_REGS_STRAP) & 0x2)
#elif TCSUPPORT_CPU_MT7505
#define IS_SPIFLASH			1
#define NANDFLASH_HWTRAP 	0
#else
#define IS_SPIFLASH				( (isRT63165 || isRT63365 || isMT751020  || isEN751221) ? ((VPint(CR_AHB_SSR) & (1<<20)) || !(NANDFLASH_HWTRAP)) :((VPint(CR_AHB_SSR) & (1<<20)) || (VPint(CR_AHB_HWCONF) & (1<<4))) )
#define NANDFLASH_HWTRAP 	(VPint(CR_AHB_HWCONF) & 0x1)
#endif
#define IS_NANDFLASH				( (isEN751221 || isRT63165 || isRT63365 || isMT751020) ? ((NANDFLASH_HWTRAP)) : (0) )

#if defined(TCSUPPORT_CPU_EN7512)||defined(TCSUPPORT_CPU_EN7521)
#define READ_FLASH_CACHE(i)			( IS_NANDFLASH ? SPI_NAND_Flash_Read_Byte(i): ReadSPICache(i)  )
#define READ_FLASH_BYTE(i)			( IS_NANDFLASH ? SPI_NAND_Flash_Read_Byte(i): ReadSPIByte(i)   )
#define READ_FLASH_DWORD(i)			( IS_NANDFLASH ? SPI_NAND_Flash_Read_DWord(i): ReadSPIDWord(i) )
#else
#define READ_FLASH_CACHE(i)			( NANDFLASH_HWTRAP ? ReadNandCache(i) : ReadSPICache(i) )
#define READ_FLASH_BYTE(i)			( NANDFLASH_HWTRAP ? ReadNandByte(i) : ReadSPIByte(i) )
#define READ_FLASH_DWORD(i)			( NANDFLASH_HWTRAP ? ReadNandDWord(i) : ReadSPIDWord(i) )
#endif


//#ifdef CONFIG_DUAL_IMAGE
#if 0
extern int flash_write_anyaddr(unsigned long to,
	unsigned long len, unsigned long *retlen, const unsigned char *buf);
#endif
#endif /* __FLASHHAL_H__ */
