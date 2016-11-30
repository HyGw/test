#include <asm/io.h>
#include <asm/tc3162.h>
#include <flash.h>
#include <flashhal.h>
#include <spiflash.h>
#include <nandflash.h>

#include <linux/init.h>
#include <linux/string.h>

#ifndef TCSUPPORT_BB_SPI
#define spiflash_init(a)				-1
#define spiflash_erase(a, b)			-1
#define spiflash_read(a, b, c, d)		-1
#define spiflash_write(a, b, c, d)		-1
#endif

#ifndef TCSUPPORT_BB_NAND
#define nandflash_init(a)				-1
#define nandflash_erase(a, b)			-1
#define nandflash_read(a, b, c, d)		-1
#define nandflash_write(a, b, c , d)	-1
#endif


unsigned long flash_base;

int flash_init(unsigned long rom_base)
{
	if (IS_SPIFLASH) {
		if(isMT751020 || isMT7505){
			VPint(0xbfb00038) &= 0xffe0e0e0;
			VPint(0xbfb00038) |= 0x80070F00;
			flash_base = 0xbc000000;
		
		} else {
#if defined(TCSUPPORT_CT_BOOTLOADER_UPGRADE)
		flash_base = 0xb0000000;
#else
		flash_base = 0xbfc00000;
#endif
		}
		prom_printf("flash base: %x\n", flash_base);
		return spiflash_init(flash_base);
	} else {
	}	
}

int flash_erase(unsigned long addr, unsigned long size)
{
	if (IS_NANDFLASH) 
		return nandflash_erase(addr, size);
	else if (IS_SPIFLASH) {
		return spiflash_erase(addr, size);
	}
}

int flash_read(unsigned long from,
	unsigned long len, unsigned long *retlen, unsigned char *buf)
{
	if (IS_NANDFLASH) 
		return nandflash_read(from, len, retlen, buf);
	else if (IS_SPIFLASH) 
		return spiflash_read(from, len, retlen, buf);
}

int flash_write(unsigned long to,
	unsigned long len, unsigned long *retlen, const unsigned char *buf)
{
	if (IS_NANDFLASH) 
		return nandflash_write(to, len, retlen, buf);
	else if (IS_SPIFLASH) {
		return spiflash_write(to, len, retlen, buf);
	}
}
//#ifdef CONFIG_DUAL_IMAGE
#if 0
int flash_write_anyaddr(unsigned long to,
	unsigned long len, unsigned long *retlen, const unsigned char *buf)
{
	unsigned long buf_data = 0x80030000;
	struct spi_flash_info flash = getFlashTableEntry(getFlashIndex());
	unsigned long dstaddr = to;
	unsigned long eraselen = len;
	if (dstaddr & (flash.EraseSize - 1))
	  	dstaddr = dstaddr - (dstaddr & (flash.EraseSize - 1));
	if (eraselen & (flash.EraseSize - 1))
		eraselen = eraselen - (eraselen & (flash.EraseSize - 1)) + flash.EraseSize;

	if(len + to > flash.DeviceSize)
		return -1;
	//copy erase block data from flash to sdram
	prom_printf("\r\ndstaddr is %x",dstaddr);
	prom_printf("\r\neraselen is %lu",eraselen);
	prom_printf("\r\nmodifybeginaddr is %x",buf_data);
	prom_printf("\r\nmodifyaddr is %x",buf_data+(to & (flash.EraseSize - 1)));
	memcpy((char*)buf_data,(char*)(0xb0000000+dstaddr),eraselen);
	memcpy((char*)(buf_data+(to & (flash.EraseSize - 1))),buf,len);
	//do flash erase and write 
	if (flash_erase(dstaddr, eraselen))
	{
		prom_printf("\r\nspiflash erase error!");
		goto error;
	}
	
	if (flash_write(dstaddr, eraselen, &retlen, (const unsigned char *)buf_data))
	{
		prom_printf("\r\nspiflash write error!");
		goto error;
	}
	return 0;

error:
	prom_printf("\r\nerror[flash_write_anyaddr] ");
	memset((char*)buf_data,0,eraselen);
	return -2;
}
#endif
