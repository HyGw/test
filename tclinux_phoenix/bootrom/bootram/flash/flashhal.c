#include <asm/io.h>
#include <asm/tc3162.h>
#include <flash.h>
#include <flashhal.h>
#include <spiflash.h>
#include <nandflash.h>

#include <linux/init.h>
#include <linux/string.h>

#include "common.h"

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
#ifdef TCSUPPORT_MTD_PARTITIONS_CMDLINE
unsigned long flash_tclinux_start;
#else
unsigned long flash_tclinux_start = TCLINUX_FLASH_START;
#endif

int flash_init(unsigned long rom_base)
{
	if (IS_NANDFLASH) {
		if (isEN751221)					
		{
			VPint(0xbfb00038) &= 0xffe0e0e0;
			VPint(0xbfb00038) |= 0x80070F00;
			flash_base = 0xbc000000;
		}
		if (isRT63165 || isRT63365 || isMT751020){
			/*enable addr bigger than 4M support.*/
			VPint(0xbfb00038) |= 0x80000000;		
		}
		flash_base = 0x0;
		return nandflash_init(flash_base);
	} else if (IS_SPIFLASH) {
#ifdef TC3262
#ifdef TCSUPPORT_ADDR_MAPPING
		if(isMT751020 || isMT7505 || isEN751221){
			VPint(0xbfb00038) &= 0xffe0e0e0;
			VPint(0xbfb00038) |= 0x80070F00;
			flash_base = 0xbc000000;
		}
		else if (isTC3182 || isRT65168 || isRT63165 || isRT63365){
#else
		if (isTC3182 || isRT65168 || isRT63165 || isRT63365 || isMT751020 || isMT7505 ||isEN751221){ // 7505 FPGA
#endif
			/*enable addr bigger than 4M support.*/
			VPint(0xbfb00038) |= 0x80000000;		
			flash_base = 0xb0000000;
		} else {
#else	
		if (isTC3162U) {
			/*SDRAM Driving Strength Setting*/
			/*update new setting. shnwind 20100225*/
			VPint(CR_SDRAM_DRI_STRENG_1)=0xfffe0000;
			VPint(CR_SDRAM_DRI_STRENG_2)=0x000007cf;
			VPint(CR_SDRAM_DRI_STRENG_3)=0;
			/*enable addr bigger than 4M support.*/
			VPint(0xbfb00038) |= 0x80000000;		
			flash_base = 0xb0000000;
		} else if(isRT63260){

			#if !defined(TCSUPPORT_AUTOBENCH)
			if (isRT63260B){
				VPint(0xbfb000b4) = 0x154;
			} else if (isRT63260C){
				VPint(0xbfb000b4) = 0x154;
				VPint(0xbfb000bc) = (VPint(0xbfb000bc) & (~(0x1<<9))) | (0x1<<16);
			} 
			#endif
			
	 		//fix usb device bug. merge from linos.	
			VPint(0xbfb000bc) &= ~((1<<0)|(1<<1)|(1<<2)|(1<<9));
			VPint(0xbfb000bc) = (VPint(0xbfb000bc) & (~(1<<9))) | (1<<2);

			/*enable addr bigger than 4M support.*/
			VPint(0xbfb00038) |= 0x80000000;
			flash_base = 0xb0000000;
		}
		else {
#endif
#if defined(TCSUPPORT_CT_BOOTLOADER_UPGRADE)
		flash_base = 0xb0000000;
#else
		flash_base = 0xbfc00000;
#endif
		}
		prom_printf("flash base: %x\n", flash_base);
		return spiflash_init(flash_base);
	} else {
		return amdflash_init(rom_base);
	}	
}

int flash_erase(unsigned long addr, unsigned long size)
{
	if (IS_NANDFLASH) 
		return nandflash_erase(addr, size);
	else if (IS_SPIFLASH) {
		return spiflash_erase(addr, size);
	} else
		return amdflash_erase(addr, size);
}

int flash_read(unsigned long from,
	unsigned long len, unsigned long *retlen, unsigned char *buf)
{
	if (IS_NANDFLASH) 
		return nandflash_read(from, len, retlen, buf);
	else if (IS_SPIFLASH) 
		return spiflash_read(from, len, retlen, buf);
	else
		return amdflash_read(from, len, retlen, buf);
}

int flash_write(unsigned long to,
	unsigned long len, unsigned long *retlen, const unsigned char *buf)
{
	if (IS_NANDFLASH) 
		return nandflash_write(to, len, retlen, buf);
	else if (IS_SPIFLASH) {
		return spiflash_write(to, len, retlen, buf);
	}	
	else
		return amdflash_write(to, len, retlen, buf);
}

#ifdef BOOTROM_LARGE_SIZE
#ifdef TCSUPPORT_BB_NAND
extern struct ra_nand_chip ra;
#endif

int flash_firmware_base_init(void)
{
#ifdef TCSUPPORT_BB_NAND
	if(IS_NANDFLASH){
		/*Modify NAND Flash Layout for Block size is 128k or 256k*/
		if((1<<ra.flash->erase_shift) >= 0x20000)
			/*1st block is bootloader, 2nd is romfile*/
			//flash_tclinux_start = (1<<ra.flash->erase_shift)*2;
			/*tclinux offset is 0x80000 for 128k&256k block size*/
			flash_tclinux_start = 0x40000*2;
	}
	else
#endif
	{
		flash_tclinux_start = TCLINUX_FLASH_START;
	}
}
#endif


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
