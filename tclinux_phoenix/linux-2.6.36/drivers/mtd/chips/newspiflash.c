/*
 * SPIFLASH support for TC3162
 */

/*
 * MTD driver for the SPI Flash Memory support.
 *
 * Copyright (c) 2005-2006 Atheros Communications Inc.
 * Copyright (C) 2006-2007 FON Technology, SL.
 * Copyright (C) 2006-2007 Imre Kaloz <kaloz@openwrt.org>
 * Copyright (C) 2006-2007 Felix Fietkau <nbd@openwrt.org>
 *
 * This code is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
*/
#include <linux/proc_fs.h> 

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/version.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/semaphore.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/gen_probe.h>
#include <asm/delay.h>
#include <asm/io.h>
#include <asm/tc3162/tc3162.h>
#include "newspiflash.h"
#include <linux/spinlock.h>

/* debugging */
/* #define SPIFLASH_DEBUG */
//#define TC_SOC2

#define MIN(a,b)        ((a) < (b) ? (a) : (b))
#define FALSE 	0
#define TRUE 	1

#define ID_MASK         		0xffff

#define MANUFACTURER_ID(id) 	((id >> 16) & ID_MASK)
#define DEVICE_ID(id)       	(id & ID_MASK)

#define SIZE_64KiB      		0x10000
#define SIZE_64MiB              0x4000000
#define SIZE_32MiB              0x2000000
#define SIZE_16MiB              0x1000000
#define SIZE_8MiB       		0x800000
#define SIZE_4MiB       		0x400000
#define SIZE_2MiB       		0x200000

/* Manufacturers */
#define MANUFACTURER_ST 		0x0020
#define MANUFACTURER_WINBOND	0x00ef
#define MANUFACTURER_SST		0x00bf
#define MANUFACTURER_MXIC		0x00c2
#define MANUFACTURER_SPANSION	0x0001
#define MANUFACTURER_EON		0x001c
#define MANUFACTURER_NUMONYX	0x0020
#define MANUFACTURER_GIGADEVICE	0x00c8
#define MANUFACTURER_ESMT		0x008c

/* ST */
#define M25P16	          		0x2015
#define M25P32	          		0x2016
#define M25P64	          		0x2017

/* Winbond */
#define W25X16					0x3015
#define W25X32					0x3016
#define W25X64					0x3017
#define W25X128					0x3018
#define W25X256					0x3019

#define W25Q16					0x4015
#define W25Q32					0x4016
#define W25Q64					0x4017
#define W25Q128					0x4018
#define W25Q256					0x4019

/* SST */
#define SST25VF032B				0x254a

/* MXIC */
#define MX25L16					0x2015
#define MX25L3205D				0x2016
#define MX25L6405D              0x2017
#define MX25L12805D             0x2018
#define MX25L25635E  			0x2019

/* SPANSION */
#define S25FL016A          		0x0214
#define S25FL032A          		0x0215
#define S25FL064A          		0x0216
#define S25FL128P          		0x2018
#define S25FL256S				0x0219

#define S25FL208K          		0x4014
#define S25FL116K          		0x4015
#define S25FL216K          		0x4015
#define S25FL127S         		0x2018

/* MICRON(NUMONYX) */
#define N25Q064					0xba17
#define N25Q128					0xba18
#define N25Q256					0xba19

/* EON */
#define EN25Q64          		0x3017

/* GIGA */
#define GD25Q16				0x4015
#define GD25Q32				0x4016
#define GD25Q64				0x4017
#define GD25Q128B			0x4018

/* ESMT */
#define F25L16PA			0x2115
#define F25L32QA			0x4116
#define F25L64QA			0x4117

struct mtd_info *spi_nor_mtd;
static DEFINE_SPINLOCK(spinorLock);
static DEFINE_SPINLOCK(spiLock);

//#if defined(TC_SOC2) 
DECLARE_MUTEX(SPI_SEM);//Make sure all related SPI operations are atomic
//DECLARE_MUTEX_LOCKED(SPI_SEM);//Make sure all related SPI operations are atomic
//#endif

struct spi_flash_info {
	const u16 mfr_id;
	const u16 dev_id;
	const u16 extra_id;
	const char *name;
	const int DeviceSize;
	const int EraseSize;
};

struct spi_chip_info {
	struct spi_flash_info *flash;
	void (*destroy)(struct spi_chip_info *chip_info);

	u32 (*read)(struct map_info *map, u32 from, u32 to, u32 size);
	u32 (*read_manual)(struct mtd_info *mtd, unsigned long from, unsigned char *buf, unsigned long len);
	u32 (*write)(struct mtd_info *mtd, u32 from, u32 to, u32 size);
	u32 (*erase)(struct mtd_info *mtd, u32 addr);
};
static int
spiflash_manual_read_internal (struct mtd_info *mtd, unsigned long from, unsigned char *buf, unsigned long len, unsigned char read_type);

static wait_queue_head_t spiflash_wq;
static spinlock_t spiflash_mutex;
static int spiflash_state;

static __u8 byte_program_mode = 0;
static __u8 four_byte_mode = 0;
static __u8 is_W25Q256 = 0;
static __u8 is_MX25L256 = 0;
static __u8 is_S25FL256S = 0;
static __u8 is_N25Q256 = 0;
static __u8 is_SPI_32MB = 0;
static __u8 epcheck_flag = 0;

void send_single_opcode_cmd(uint8 op_code)
{
	SEND_CSL_CMD;
	SEND_OPFIFO_WRITE_CMD(OP_OUTS, OP_REPEAT_ONE);
	SEND_DFIFO_WRITE_CMD(op_code);
	SEND_CSH_CMD;
}

void sf_manual_begin(void)
{	
	/* disable interrupt */
	/*local_irq_disable(); */
	WARN_ON(in_interrupt());
	spin_lock(&spiLock);

	/* disable READ_IDLE_EN */
	SEND_READ_IDLE_DIS_CMD;
	/* wait until auto read status is IDLE */
	while(ReadReg(SF_RDCTL_FSM)) ;
	/* auto mode -> manual mode */
	SEND_AUTO2MANUAL_CMD;
}

void sf_manual_end(void) 
{
	/* manual mode -> auto mode */
	SEND_MANUAL2AUTO_CMD;
	/* enable READ_IDLE_EN */
	SEND_READ_IDLE_EN_CMD;
	
	/* enable interrupt */
	/*local_irq_enable();*/
	spin_unlock(&spiLock);
}

void sf_manual_wren_begin(void)
{	
	sf_manual_begin();
	SEND_WREN_CMD;
}

void sf_manual_wren_end(void) 
{
	uint8 reg_sr1 = 0;
	/* wait until WIP bit is idle */
	SEND_WAIT_WIP_IDLE_INTERNAL_CMD(reg_sr1);
	
	sf_manual_end();
}

#if defined(TCSUPPORT_NEW_SPIFLASH_DEBUG)
#define PP_MAX	(256)
#define SE_MAX	(4096)
#define SE_CNT	(16)
#define SEED_MAX (0x100)

#define AUTOREAD_TEST	(0)
#define MANUALREAD_TEST	(1)
#define MUXREAD_TEST	(2)

#define SIZE_32MiB	0x2000000
#define AUTOREAD_TEST_MAX	(4)
#define MANUALREAD_TEST_MAX	(4)
#define MUXREAD_TEST_MAX	(8)

#define SFC_CLOCK_MAX 150

#define TP_MAX (10)
uint8 TestPattern[TP_MAX] =
{
    {0xA5},
    {0x5A},
    {0xFF},
    {0x00},
    {0x55},
    {0xAA},
    {0x96},
    {0x69},
    {0xCC},
    {0x33}
};

uint8 TestMode_TEXT[3][12] =
{
	{"AutoMode"},
	{"ManualMode"},
	{"MuxMode"},
};

uint8 ReadMode_TEXT[4][20] =
{
	{"ReadData"},
	{"FastRead"},
	{"FastRead_DualOut"},
	{"FastRead_DualIO"},
};

uint8 MuxMode_TEXT[8][25] =
{
	{"Auto_ReadData"},
	{"Auto_FastRead"},
	{"Auto_FastRead_DualOut"},
	{"Auto_FastRead_DualIO"},
	{"Manual_ReadData"},
	{"Manual_FastRead"},
	{"Manual_FastRead_DualOut"},
	{"Manual_FastRead_DualIO"},
};

uint8 clk_rate=0;
uint8 CLKRate_TEXT[3][12] =
{
	{"25MHZ"},
	{"50MHZ"},
	{"Changing"},
};

uint8 Address3B4B_TEXT[3][40] =
{
	{"4Bytes access all 32MB flash"},
	{"3Bytes access front 16MB flash"},
	{"3Bytes access, and use 13/0C/3C/BC"},
};

uint32 Round_Value = 0;
uint16 AutoRead_Value = 0;
uint16 ManualRead_Value = 0;
uint16 MuxRead_Value = 0;
uint16 isProgram_Value = 0;
uint16 CLKRate_Value = 0;
uint16 Exit_Value = 0;
uint16 Address3B4B_Value = 0;
uint32 TestPattern_Value = 0;
uint32 StartAddress_Value = 0;
uint16 ReadIdle_Value = 0;
uint16 Tn_Value = 0;
uint16 Tn_cnt = 0;
uint16 TnMappingTab[6];

uint16 AutoRead_cnt = 0;
uint16 ManualRead_cnt = 0;
uint16 MuxRead_cnt = 0;
uint16 AutoMappingTab[4];
uint16 ManualMappingTab[4];
uint16 MuxMappingTab[8];
uint32 spi_task2_en=0;
uint16 Addr3B4B_Flag=0;

uint32 getTime(void)
{
	return (jiffies * 10);
}

void exit_4Byte_mode(void)
{
	if(is_S25FL256S){
		SEND_WRITE_REGISTER_CMD(SF_OP_WR_BANKREG, SF_BANKREG_3B);
		SEND_WRDIS_CMD; // spansion bank register does not require the WREN command to precede it, which means WREN bit need to disable by manually.
	} else if(is_N25Q256){
		SEND_EXIT_4BYTES_MODE_WREN_CMD; //for N25Q256, to enter/exit 4Byte address mode, the WREN bit must be set to 1 first.
	} else {
		SEND_EXIT_4BYTES_MODE_CMD;
	}
	Addr3B4B_Flag = AUTO_3BMODE;
}
#endif

void enter_4Byte_mode(void)
{
	if(is_S25FL256S){
		SEND_WRITE_REGISTER_CMD(SF_OP_WR_BANKREG, SF_BANKREG_4B);
		SEND_WRDIS_CMD; // spansion bank register does not require the WREN command to precede it, which means WREN bit need to disable by manually.
	} else if(is_N25Q256){
		SEND_ENTER_4BYTES_MODE_WREN_CMD; //for N25Q256, to enter/exit 4Byte address mode, the WREN bit must be set to 1 first.
	}else{
		SEND_ENTER_4BYTES_MODE_CMD;
	}
#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
	Addr3B4B_Flag = AUTO_4BMODE;
#endif
}

static void dumpCell_buffer(u8* buf, u32 len, u32 from){
	u32 i=0;
	
	from &= 0x03FFFFFF;
	from |= 0xBC000000;
	printk("\r\n\t\t00  01  02  03  04  05  06  07  08  09  0A  0B  0C  0D  0E  0F");
	for(i=0; i<len; i++){
		if((i&15)==0){
			printk("\r\n0x%08X:\t", from+i);
		}
		printk("%02x  ", *(buf+i));
	}
	printk("\r\n");
}/*end dumpCell_buffer*/

#if 0
void enter_dualIO_enable_mode(void)
{
	u8 reg_value = 0;
	if(is_N25Q256) {
		SEND_READ_REGISTER_CMD(SF_OP_RD_ENHANCED_CFGREG, reg_value);
		reg_value &= 0x3F;
		reg_value |= 0x80;
		//SEND_WRITE_REGISTER_CMD(SF_OP_WR_ENHANCED_CFGREG, reg_value);
	}
}
#endif

/* Probe SPI flash device
 * Function returns 0 for failure.
 * and flashconfig_tbl array index for success.
 */
static u32
spiflash_read_id (void)
{
	u32 flash_id=0;
	u8 id[3]={0};
	u8 cur_mode=0;

	WARN_ON(in_interrupt());
#if defined(TC_SOC2) && defined(CONFIG_MIPS_TC3262)
	if(down_Manual_interruptible(&SPI_SEM))
		return -ERESTARTSYS;
#else
	down_Normal_interruptible(&SPI_SEM); /*This is empty fuction*/
	spin_lock(&spinorLock);
#endif

	SEND_RD_ID_CMD(id);

	flash_id = id[0] << 16 | id[1] << 8 | id[2];
	
#if defined(TC_SOC2) && defined(CONFIG_MIPS_TC3262)
	up_Manual(&SPI_SEM);
#else
	up_Normal(&SPI_SEM); /*This is empty fuction*/
	spin_unlock(&spinorLock);
#endif

	return flash_id;
}

static u32
spiflash_erase (struct mtd_info *mtd, u32 addr)
{
	u32 i = 0;
	u8 cur_mode=0;
	u8 mode=0;
	unsigned char *rd_buf=NULL;
	u32 destAddr = addr;
#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
	u32 begtime=0, endtime=0, passtime=0;
	u32 begtick=0, endtick=0;
	unsigned long dest_addr = addr;
	uint16 cur3B4B_Flag = Addr3B4B_Flag;
#endif

	WARN_ON(in_interrupt());
	/* sanity checks */
	if (addr + mtd->erasesize > mtd->size) return (-1);
	
#ifndef TCSUPPORT_NEW_SPIFLASH_DEBUG
	printk("\nerase addr=%x size=%x", addr, mtd->erasesize);
#endif
	
	if(mtd->erasesize == SIZE_64KiB){
		
#if defined(TC_SOC2) && defined(CONFIG_MIPS_TC3262)
		if(down_Manual_interruptible(&SPI_SEM))
			return -ERESTARTSYS;
#else
	down_Normal_interruptible(&SPI_SEM); /*This is empty fuction*/
		spin_lock(&spinorLock);
#endif

#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
		if(mtd->size >= SIZE_32MiB) {
			if((Address3B4B_Value != 0) && ((dest_addr>>24) != 0)) {
				if(Addr3B4B_Flag == AUTO_3BMODE) {
					enter_4Byte_mode();
				}
			}
		}
		begtime = getTime();
		begtick = VPint(CR_TIMER1_VLR);
		
		if(mtd->size >= SIZE_32MiB) {
			if((Address3B4B_Value == 0) || ((Address3B4B_Value != 0) && ((dest_addr>>24) != 0)))
			{
				//printk("\nErase::mode=1, addr is 0x%08x", addr);
				mode = 1;
			} else {
				if(Addr3B4B_Flag == AUTO_4BMODE) {
					exit_4Byte_mode();
				}
				if(is_W25Q256 == 1) {
					SEND_WRITE_REGISTER_CMD(SF_OP_WR_STATUSEX, 0);
				}
				mode = 0;
			}
		} else {
			mode = 0;
		}
#else
		if(mtd->size >= SIZE_32MiB){
			//printk("\nErase::mode=1, mtd->size is %X\n", mtd->size);
			mode = 1;
		} else {
			mode = 0;
		}
#endif
		SEND_SECTOR_ERASE_CMD(addr, mode);

#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
		endtick = VPint(CR_TIMER1_VLR);
		endtime = getTime();

		if(endtime < begtime)
			passtime = 0xffffffff - begtime + endtime;
		else
			passtime = endtime - begtime;
		passtime = (passtime/10) * VPint(CR_TIMER1_LDV)+ begtick - endtick;
		printk("\nerase addr=%x size=%x passtime is %lu", addr, mtd->erasesize, passtime);
		
		if(mtd->size >= SIZE_32MiB) {
			if((cur3B4B_Flag == AUTO_4BMODE) && (Addr3B4B_Flag == AUTO_3BMODE)) {
				enter_4Byte_mode();
			} else if((cur3B4B_Flag == AUTO_3BMODE) && (Addr3B4B_Flag == AUTO_4BMODE)) {
				exit_4Byte_mode();
			}
		}
#endif

#if defined(TC_SOC2) && defined(CONFIG_MIPS_TC3262)
		up_Manual(&SPI_SEM);
#else
	up_Normal(&SPI_SEM); /*This is empty fuction*/
		spin_unlock(&spinorLock);
#endif

		if(epcheck_flag == 1) {
			printk("\nerase check start");
			rd_buf = kmalloc(sizeof(char) * SIZE_64KiB, GFP_KERNEL);
			spiflash_manual_read_internal(mtd, destAddr, rd_buf, SIZE_64KiB, READ_DATA);
			for(i=0; i<SIZE_64KiB; i++) {
				if( rd_buf[i] !=  0xFF) {
					printk("\nErase fail at Address %X::index is %d, value is %X, should be %X, reg 0xBFA10000 is 0x%08x, clk_rate is %s\n", destAddr+i, i, rd_buf[i], 0xFF, SF_READ_MODE_VALUE, ((SF_CLK_CHANGE_VALUE==0)?("25MHZ"):("50MHZ")));
					dumpCell_buffer((u8 *)(rd_buf), SIZE_64KiB, destAddr);
					break;
				} 
			}
			kfree(rd_buf);
		}	
	} else {
		return (-1);
	}

   	return (0);
}

/* wait until the flash chip is ready and grab a lock */
static int spiflash_wait_ready(int state)
{
	DECLARE_WAITQUEUE(wait, current);

retry:
	spin_lock_bh(&spiflash_mutex);
	if (spiflash_state != FL_READY) {
		set_current_state(TASK_UNINTERRUPTIBLE);
		add_wait_queue(&spiflash_wq, &wait);
		spin_unlock_bh(&spiflash_mutex);
		schedule();
		remove_wait_queue(&spiflash_wq, &wait);

		if(signal_pending(current))
			return 0;

		goto retry;
	}
	spiflash_state = state;

	return 1;
}

static inline void spiflash_done(void)
{
	spiflash_state = FL_READY;
	spin_unlock_bh(&spiflash_mutex);
	wake_up(&spiflash_wq);
}

static int
spiflash_manual_read_internal (struct mtd_info *mtd, unsigned long from, unsigned char *buf, unsigned long len, unsigned char read_type)
{
	int mode=0;
	u8 cur_mode=0;
	unsigned long dest_addr = from;
#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
	uint16 cur3B4B_Flag = Addr3B4B_Flag;
#endif

	WARN_ON(in_interrupt());
#ifdef SPIFLASH_DEBUG
   	printk ("spiflash_read (from = 0x%08x, len = %d)\n", from, len);
#endif

   	/* sanity checks */
   	if (!len) return (0);
   	if (from + len > mtd->size) return (-1);

	
#if defined(TC_SOC2) && defined(CONFIG_MIPS_TC3262)
	if(down_Manual_interruptible(&SPI_SEM))
		return -ERESTARTSYS;
#else
	down_Normal_interruptible(&SPI_SEM); /*This is empty fuction*/
	spin_lock(&spinorLock);
#endif
	
#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
	if((Address3B4B_Value == 2) && (mtd->size >= SIZE_32MiB) && (is_MX25L256 == 0) && ((dest_addr>>24) != 0))	{
		if(Addr3B4B_Flag == AUTO_3BMODE) {
			enter_4Byte_mode();
		}
		switch(read_type){
		case READ_DATA:
			SEND_4BYTES_READ_CMD(from, buf, len);
			break;
			
		case FAST_READ:
			SEND_4BYTES_FASTREAD_CMD(from, buf, len);
			break;
			
		case FAST_READ_DUALOUT:
			SEND_4BYTES_FASTREAD_DUALOUT_CMD(from, buf, len);
			break;
			
		case FAST_READ_DUALIO:
			SEND_4BYTES_FASTREAD_DUALIO_CMD(from, buf, len);
			break;

		default:
			printk("spiflash_read: not support this mode\n");
			break;		
		}
		
		if(cur3B4B_Flag == AUTO_3BMODE) {
			exit_4Byte_mode();
		}
	} else {
		if(mtd->size >= SIZE_32MiB) {
			if((Address3B4B_Value != 0) && ((dest_addr>>24) != 0)) {
				if(Addr3B4B_Flag == AUTO_3BMODE) {
					enter_4Byte_mode();
				}
			}

			if((Address3B4B_Value == 0) || ((Address3B4B_Value != 0) && ((dest_addr>>24) != 0))) {
				mode = 1;
			} else {
				if(Addr3B4B_Flag == AUTO_4BMODE) {
					exit_4Byte_mode();
				}
				if(is_W25Q256 == 1) {
					SEND_WRITE_REGISTER_CMD(SF_OP_WR_STATUSEX, 0);
				}
				mode = 0;
			}
		} else {
			mode = 0;
		}
#else
		if(mtd->size >= SIZE_32MiB) {
			mode = 1;
			//printk("\nRead::mode=1, mtd->size is %X\n", mtd->size);
		} else {
			mode = 0;
		}
#endif

	switch(read_type){
	case READ_DATA:
		SEND_READ_CMD(from, buf, len, mode);
		break;
		
	case FAST_READ:
		SEND_FASTREAD_CMD(from, buf, len, mode);
		break;
		
	case FAST_READ_DUALOUT:
		SEND_FASTREAD_DUALOUT_CMD(from, buf, len, mode);
		break;
		
	case FAST_READ_DUALIO:
		SEND_FASTREAD_DUALIO_CMD(from, buf, len, mode);
		break;

	default:
		printk("spiflash_read: not support this mode\n");
		break;		
	}
#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
		if(mtd->size >= SIZE_32MiB) {
			if((cur3B4B_Flag == AUTO_4BMODE) && (Addr3B4B_Flag == AUTO_3BMODE)) {
				enter_4Byte_mode();
			} else if((cur3B4B_Flag == AUTO_3BMODE) && (Addr3B4B_Flag == AUTO_4BMODE)) {
				exit_4Byte_mode();
			}
		}
	}
#endif
	
#if defined(TC_SOC2) && defined(CONFIG_MIPS_TC3262)
	up_Manual(&SPI_SEM);
#else
	up_Normal(&SPI_SEM); /*This is empty fuction*/
        spin_unlock(&spinorLock);
#endif

   	return (0);
}


static int
spiflash_manual_read_data(struct mtd_info *mtd, unsigned long from, unsigned char *buf, unsigned long len)
{
	spiflash_manual_read_internal(mtd, from, buf, len, READ_DATA);
   	return (0);
}

static int
spiflash_manual_fast_read(struct mtd_info *mtd, unsigned long from, unsigned char *buf, unsigned long len)
{
	spiflash_manual_read_internal(mtd, from, buf, len, FAST_READ);
   	return (0);
}

static int
spiflash_manual_fast_read_dualOut(struct mtd_info *mtd, unsigned long from, unsigned char *buf, unsigned long len)
{
	spiflash_manual_read_internal(mtd, from, buf, len, FAST_READ_DUALOUT);
   	return (0);
}

static int
spiflash_manual_fast_read_dualIO(struct mtd_info *mtd, unsigned long from, unsigned char *buf, unsigned long len)
{
	spiflash_manual_read_internal(mtd, from, buf, len, FAST_READ_DUALIO);
   	return (0);
}

static u32
spiflash_read (struct map_info *map, u32 from, u32 to, u32 size)
{
	u8 cur_mode=0;
#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
	uint16 cur3B4B_Flag = Addr3B4B_Flag;
#endif

	WARN_ON(in_interrupt());
#if defined(TC_SOC2) && defined(CONFIG_MIPS_TC3262)
	if(down_Auto_interruptible(&SPI_SEM))
		return -ERESTARTSYS;
#else
	down_Normal_interruptible(&SPI_SEM); /*This is empty fuction*/
        spin_lock(&spinorLock);
#endif
	
#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
	if(is_SPI_32MB) {		
		if((Address3B4B_Value == 0) || ((Address3B4B_Value != 0) && ((from>>24) != 0)))
		{
			if(Addr3B4B_Flag == AUTO_3BMODE) {
				enter_4Byte_mode();
			}
			SEND_AUTO_4B_CMD;
		} else {
			if(Addr3B4B_Flag == AUTO_4BMODE) {
				exit_4Byte_mode();
			}
			SEND_AUTO_3B_CMD;
			if(is_W25Q256 == 1) {
				SEND_WRITE_REGISTER_CMD(SF_OP_WR_STATUSEX, 0);
			}
		}
	}
#endif

	memcpy4((unsigned char*)to, (unsigned char*)(map->virt + from), size);

#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
	if(is_SPI_32MB) {
		if((cur3B4B_Flag == AUTO_4BMODE) && (Addr3B4B_Flag == AUTO_3BMODE)) {
			enter_4Byte_mode();
			SEND_AUTO_4B_CMD;
		} else if((cur3B4B_Flag == AUTO_3BMODE) && (Addr3B4B_Flag == AUTO_4BMODE)) {
			exit_4Byte_mode();
			SEND_AUTO_3B_CMD;
		}
	}
#endif
#if defined(TC_SOC2) && defined(CONFIG_MIPS_TC3262)
	up_Auto(&SPI_SEM);
#else
	up_Normal(&SPI_SEM); /*This is empty fuction*/
        spin_unlock(&spinorLock);
#endif
   	return (0);
}

static u32
spiflash_write (struct mtd_info *mtd, u32 from, u32 to, u32 len)
{
	u8 cur_mode=0;
	u8 mode=0;
	static int cnt=0;
	u32 i=0;
	u32 pp_len = 0;
	u32 pp_cur_len = 0;
	unsigned long tmp_len = 0;
	u32 destAddr = to;
#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
	u32 last_addr = 0;
	u32 begtime=0, endtime=0, passtime=0;
	u32 begtick=0, endtick=0;
	uint16 cur3B4B_Flag = Addr3B4B_Flag;
#endif
	unsigned char *buf=NULL;
	unsigned char *rd_buf=NULL;

	WARN_ON(in_interrupt());
	buf = (unsigned char *) from;

   	/* sanity checks */
   	if (!len) return (0);
   	if (to + len > mtd->size) return (-1);

	//printk("program from %x to %x\n", to, to+len);
	
#if defined(TC_SOC2) && defined(CONFIG_MIPS_TC3262)
	if(down_Manual_interruptible(&SPI_SEM))
		return -ERESTARTSYS;
#else
	down_Normal_interruptible(&SPI_SEM); /*This is empty fuction*/
        spin_lock(&spinorLock);
#endif

	pp_len = len;
	
	
#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
	//last_addr = to + len;
	last_addr = to + SF_PP_MAX*8;
	if(mtd->size >= SIZE_32MiB) {
		if((Address3B4B_Value != 0) && ((last_addr>>24) != 0))
		{
			if(Addr3B4B_Flag == AUTO_3BMODE) {
				enter_4Byte_mode();
			}
		}
	}
#endif

	if(mtd->size >= SIZE_32MiB) {
		mode = 1;
	} else {
		mode = 0;
	}

	while(pp_len > 0) {
		cnt++;
		if ((cnt & 0x3f) == 0)
			printk(".");
		
		if((to & (SF_PP_MAX-1)) != 0) {
			pp_cur_len = MIN((SF_PP_MAX - (to % SF_PP_MAX)), pp_len);
		}
		else {
			pp_cur_len = MIN(SF_PP_MAX, pp_len);
		}
		pp_len -= pp_cur_len;
		
		//SEND_WREN_CMD;
#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
		begtime = getTime();
		begtick = VPint(CR_TIMER1_VLR);
		if(mtd->size >= SIZE_32MiB) {
			if((Address3B4B_Value == 0) || ((Address3B4B_Value != 0) && ((last_addr>>24) != 0)))
			{
				//printk("\nProgram::mode=1, mtd->size is %X\n", mtd->size);
				mode = 1;
			} else {
				if(Addr3B4B_Flag == AUTO_4BMODE) {
					exit_4Byte_mode();
				}
				if(is_W25Q256 == 1) {
					SEND_WRITE_REGISTER_CMD(SF_OP_WR_STATUSEX, 0);
				}
				mode = 0;
			}
		} else {
			mode = 0;
		}
#endif
		tmp_len = pp_cur_len;
		SEND_PAGE_PROGRAM_CMD(to, buf, pp_cur_len, mode);

#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
		endtick = VPint(CR_TIMER1_VLR);
		endtime = getTime();
		if((to & 0xFFFFFF)>>12 == 0xFFF) {
			if(endtime < begtime)
				passtime = 0xffffffff - begtime + endtime;
			else
				passtime = endtime - begtime;
			passtime = (passtime/10) * VPint(CR_TIMER1_LDV)+ begtick - endtick;
			printk("\nto is 0x%08x, last_addr is 0x%08x, pp_cur_len is 0x%08x", to, last_addr, pp_cur_len);
			printk("\nprogram addr=%x size=%x passtime is %lu ", to, pp_cur_len, passtime);
		}
#endif
		to += tmp_len;
	}
#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
	if(mtd->size >= SIZE_32MiB) {
		if((cur3B4B_Flag == AUTO_4BMODE) && (Addr3B4B_Flag == AUTO_3BMODE)) {
			enter_4Byte_mode();
		} else if((cur3B4B_Flag == AUTO_3BMODE) && (Addr3B4B_Flag == AUTO_4BMODE)) {
			exit_4Byte_mode();
		}
	}
#endif
	
#if defined(TC_SOC2) && defined(CONFIG_MIPS_TC3262)
		up_Manual(&SPI_SEM);
#else
	up_Normal(&SPI_SEM); /*This is empty fuction*/
        spin_unlock(&spinorLock);
#endif

	if(epcheck_flag == 1) {
		buf = (unsigned char *) from;
		rd_buf = kmalloc(sizeof(char) * len, GFP_KERNEL);
		spiflash_manual_read_internal(mtd, destAddr, rd_buf, len, READ_DATA);
		for(i=0; i<len; i++) {
			if( rd_buf[i] !=  buf[i]) {
				printk("\nProgram fail at Address %X::index is %d, value is %X, should be %X, reg 0xBFA10000 is 0x%08x, clk_rate is %s\n", destAddr+i, i, rd_buf[i], buf[i], SF_READ_MODE_VALUE, ((SF_CLK_CHANGE_VALUE==0)?("25MHZ"):("50MHZ")));
				dumpCell_buffer((u8 *)(rd_buf), len, destAddr);
				break;
			} 
		}
		kfree(rd_buf);
	}

   	return (0);
}

static struct spi_flash_info flash_tables[] = {
	{
		mfr_id: MANUFACTURER_ST,
		dev_id: M25P64,
		name: "ST M25P64",
		DeviceSize: SIZE_8MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_ST,
		dev_id: M25P32,
		name: "ST M25P32",
		DeviceSize: SIZE_4MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_ST,
		dev_id: M25P16,
		name: "ST M25P16",
		DeviceSize: SIZE_2MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_WINBOND,
		dev_id: W25X256,
		name: "Winbond W25X256",
		DeviceSize: SIZE_32MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_WINBOND,
		dev_id: W25X128,
		name: "Winbond W25X128",
		DeviceSize: SIZE_16MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_WINBOND,
		dev_id: W25X64,
		name: "Winbond W25X64",
		DeviceSize: SIZE_8MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_WINBOND,
		dev_id: W25X32,
		name: "Winbond W25X32",
		DeviceSize: SIZE_4MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_WINBOND,
		dev_id: W25X16,
		name: "Winbond W25X16",
		DeviceSize: SIZE_2MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_WINBOND,
		dev_id: W25Q256,
		name: "Winbond W25Q256",
		DeviceSize: SIZE_32MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_WINBOND,
		dev_id: W25Q128,
		name: "Winbond W25Q128",
		DeviceSize: SIZE_16MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_WINBOND,
		dev_id: W25Q64,
		name: "Winbond W25Q64",
		DeviceSize: SIZE_8MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_WINBOND,
		dev_id: W25Q32,
		name: "Winbond W25Q32",
		DeviceSize: SIZE_4MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_WINBOND,
		dev_id: W25Q16,
		name: "Winbond W25Q16",
		DeviceSize: SIZE_2MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_SST,
		dev_id: SST25VF032B,
		name: "SST 25VF032B",
		DeviceSize: SIZE_4MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_MXIC,
		dev_id: MX25L16,
		name: "MX25L16",
		DeviceSize: SIZE_2MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_MXIC,
		dev_id: MX25L3205D,
		name: "MX25L3205D",
		DeviceSize: SIZE_4MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_MXIC,
		dev_id: MX25L6405D,
		name: "MX25L6405D",
		DeviceSize: SIZE_8MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_MXIC,
		dev_id: MX25L12805D,
		name: "MX25L12805D",
		DeviceSize: SIZE_16MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_MXIC,
		dev_id: MX25L25635E,
		name: "MX25L25635E",
		DeviceSize: SIZE_32MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_SPANSION,
		dev_id: S25FL064A,
		name: "S25FL064A",
		DeviceSize: SIZE_8MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_SPANSION,
		dev_id: S25FL032A,
		name: "S25FL032A",
		DeviceSize: SIZE_4MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_SPANSION,
		dev_id: S25FL016A,
		name: "S25FL016A",
		DeviceSize: SIZE_2MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_SPANSION,
		dev_id: S25FL128P,
		name: "S25FL128P",
		DeviceSize: SIZE_16MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_SPANSION,
		dev_id: S25FL256S,
		name: "Spasion S25FL256S",
		DeviceSize: SIZE_32MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_EON,
		dev_id: EN25Q64,
		name: "EN25Q64",
		DeviceSize: SIZE_8MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_NUMONYX,
		dev_id: N25Q064,
		name: "N25Q064",
		DeviceSize: SIZE_8MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_NUMONYX,
		dev_id: N25Q128,
		name: "N25Q128",
		DeviceSize: SIZE_16MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_NUMONYX,
		dev_id: N25Q256,
		name: "N25Q256",
		DeviceSize: SIZE_32MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_GIGADEVICE,
		dev_id: GD25Q16,
		name: "GD25Q16",
		DeviceSize: SIZE_2MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_GIGADEVICE,
		dev_id: GD25Q32,
		name: "GD25Q32",
		DeviceSize: SIZE_4MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_GIGADEVICE,
		dev_id: GD25Q64,
		name: "GD25Q64",
		DeviceSize: SIZE_8MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_GIGADEVICE,
		dev_id: GD25Q128B,
		name: "GD25Q128B",
		DeviceSize: SIZE_16MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_ESMT,
		dev_id: F25L16PA,
		name: "F25L16PA",
		DeviceSize: SIZE_2MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_ESMT,
		dev_id: F25L32QA,
		name: "F25L32QA",
		DeviceSize: SIZE_4MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_ESMT,
		dev_id: F25L64QA,
		name: "F25L64QA",
		DeviceSize: SIZE_8MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_SPANSION,
		dev_id: S25FL208K,
		name: "S25FL208K",
		DeviceSize: SIZE_2MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_SPANSION,
		dev_id: S25FL116K,
		name: "S25FL116K",
		DeviceSize: SIZE_2MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_SPANSION,
		dev_id: S25FL216K,
		name: "S25FL216K",
		DeviceSize: SIZE_2MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_SPANSION,
		dev_id: S25FL127S,
		name: "S25FL127S",
		DeviceSize: SIZE_16MiB,
		EraseSize: SIZE_64KiB,
	},
};

static struct spi_chip_info *spiflash_tc3162_setup(struct map_info *map)
{
	struct spi_chip_info *chip_info=NULL;

	chip_info = kmalloc(sizeof(*chip_info), GFP_KERNEL);
	if (!chip_info) {
		printk(KERN_WARNING "Failed to allocate memory for chip_info\n");
		return NULL;
	}

	memset(chip_info, 0, sizeof(struct spi_chip_info));

	return chip_info;
}

static void spiflash_tc3162_destroy(struct spi_chip_info *chip_info)
{
	if (chip_info) {
		kfree(chip_info);
	}
}

struct spi_chip_info *spiflash_probe_tc3162(struct map_info *map)
{
	int i=0;
	struct spi_chip_info *chip_info = NULL;
	unsigned long flash_id=0;
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
	if(down_Manual_interruptible(&SPI_SEM))
		return -ERESTARTSYS;
	
#if 0
	*((__u32 *)(CR_SPI_BASE | SPI_FLASH_MM)) = reg0x28;
#endif
#endif
#if 0
	spin_lock_init(&spiflash_mutex);
	init_waitqueue_head(&spiflash_wq);
	spiflash_state = FL_READY;
#endif

	flash_id = spiflash_read_id();

	for (i=0; i < ARRAY_SIZE(flash_tables); i++) {
		if ((MANUFACTURER_ID(flash_id) == flash_tables[i].mfr_id) &&
		    (DEVICE_ID(flash_id) == flash_tables[i].dev_id)) {
			if ( (MANUFACTURER_ID(flash_id) == MANUFACTURER_WINBOND) && (DEVICE_ID(flash_id) == W25Q256) ) {
				is_W25Q256 = 1;
			}
			if ( (MANUFACTURER_ID(flash_id) == MANUFACTURER_MXIC) && (DEVICE_ID(flash_id) == MX25L25635E) ) {
				is_MX25L256 = 1;
			}
			if ( (MANUFACTURER_ID(flash_id) == MANUFACTURER_SPANSION) && (DEVICE_ID(flash_id) == S25FL256S) ) {
				is_S25FL256S = 1;
			}
			if ( (MANUFACTURER_ID(flash_id) == MANUFACTURER_NUMONYX) && (DEVICE_ID(flash_id) == N25Q256) ) {
				is_N25Q256 = 1;
			}
			if ( flash_tables[i].DeviceSize == SIZE_32MiB) {
				is_SPI_32MB = 1;
			}
			chip_info = spiflash_tc3162_setup(map);
			if (chip_info) {
				chip_info->flash      = &flash_tables[i];
				chip_info->destroy    = spiflash_tc3162_destroy;

				chip_info->read       = spiflash_read;
				chip_info->read_manual   = spiflash_manual_fast_read;
				chip_info->write      = spiflash_write;
				chip_info->erase      = spiflash_erase;
			}
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
			up_Manual(&SPI_SEM);
#endif
			return chip_info;
		}
	}
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
	up_Manual(&SPI_SEM);
#endif
	return NULL;
}

int mtd_spiflash_erase(struct mtd_info *mtd, struct erase_info *instr)
{
	struct map_info *map = mtd->priv;
	struct spi_chip_info *chip_info = (struct spi_chip_info *)map->fldrv_priv;
	unsigned long adr=0;
	unsigned long len=0;
	int ret = 0;
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
	if(down_Manual_interruptible(&SPI_SEM))
		return -ERESTARTSYS;
#if 0
	*((__u32 *)(CR_SPI_BASE | SPI_FLASH_MM)) = reg0x28;
#endif
#endif
	if (!chip_info->erase){
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
		up_Manual(&SPI_SEM);
#endif
		return -EOPNOTSUPP;
	}

	if (instr->addr & (mtd->erasesize - 1)){
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
		up_Manual(&SPI_SEM);
#endif
		return -EINVAL;
	}

	if (instr->len & (mtd->erasesize -1)){
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
		up_Manual(&SPI_SEM);
#endif
		return -EINVAL;
	}		

	if ((instr->len + instr->addr) > mtd->size){
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
		up_Manual(&SPI_SEM);
#endif
		return -EINVAL;
	}

	adr = instr->addr;
	len = instr->len;

	while (len) {
#if 0
		if (!spiflash_wait_ready(FL_ERASING)){
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
			up_Manual(&SPI_SEM);
#endif
			return -EINTR;
		}
#endif
		ret = chip_info->erase(mtd, adr);
	//	spiflash_done();
		if (ret){
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
			up_Manual(&SPI_SEM);
#endif
			return ret;
		}

		adr += mtd->erasesize;
		len -= mtd->erasesize;
	}

	instr->state = MTD_ERASE_DONE;
	if (instr->callback)
		instr->callback(instr);

#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
	up_Manual(&SPI_SEM);
#endif
	return 0;
}

int mtd_spiflash_read(struct mtd_info *mtd, loff_t from, size_t len, size_t *retlen, u_char *buf)
{
	struct map_info *map = mtd->priv;
	struct spi_chip_info *chip_info = (struct spi_chip_info *)map->fldrv_priv;
	int ret = 0;
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
	if(down_Manual_interruptible(&SPI_SEM))
		return -ERESTARTSYS;
#if 0
	*((__u32 *)(CR_SPI_BASE | SPI_FLASH_MM)) = reg0x28;
#endif
#endif
	//if (!chip_info->read){
	if (!chip_info->read_manual){
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
		up_Manual(&SPI_SEM);
#endif
		return -EOPNOTSUPP;
	}

	//ret = chip_info->read(map, from, (u32)buf, len);
	ret = chip_info->read_manual(mtd, from, buf, len);
	if (ret){
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
		up_Manual(&SPI_SEM);
#endif
		return ret;
	}

	if(retlen)
		(*retlen) = len;
	
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
	up_Manual(&SPI_SEM);
#endif
	return 0;
}

int mtd_spiflash_write(struct mtd_info *mtd, loff_t to, size_t len, size_t *retlen, const u_char *buf)
{
	struct map_info *map = mtd->priv;
	struct spi_chip_info *chip_info = (struct spi_chip_info *)map->fldrv_priv;
	int ret = 0;
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
	if(down_Manual_interruptible(&SPI_SEM))
		return -ERESTARTSYS;

#if 0
	*((__u32 *)(CR_SPI_BASE | SPI_FLASH_MM)) = reg0x28;
#endif	
#endif
	if (!chip_info->write){
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
		up_Manual(&SPI_SEM);
#endif
		return -EOPNOTSUPP;
	}
#if 0
	if (!spiflash_wait_ready(FL_WRITING)){
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
		up_Manual(&SPI_SEM);
#endif
		return -EINTR;
	}
#endif
	ret = chip_info->write(mtd, (u32)buf, to, len);
	//spiflash_done();
	if (ret){
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
		up_Manual(&SPI_SEM);
#endif
		return ret;
	}

	if (retlen)
		(*retlen) = len;

#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
	up_Manual(&SPI_SEM);
#endif
	return 0;
}

unsigned char ReadSPIByte(unsigned long index)
{
	unsigned char buf[2] = {0};
	size_t retlen;

	index &= 0x03FFFFFF;

	mtd_spiflash_read(spi_nor_mtd, (loff_t)index, 1, &retlen, buf);

	return buf[0];
}

unsigned long ReadSPIDWord(unsigned long index)
{
	unsigned char buf[5] = {0};
	unsigned long dword = 0;
	size_t retlen;

	index &= 0x03FFFFFF;

	mtd_spiflash_read(spi_nor_mtd, (loff_t)index, 4, &retlen, buf);

	dword = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];

	return dword;
}
EXPORT_SYMBOL(ReadSPIByte);
EXPORT_SYMBOL(ReadSPIDWord);

void sfc_AutoManualInterrupt_handler()
{
	if(ReadReg(SF_INTERRUPT) == 1) {
		printk("\n>>> %d: Auto Mode and Manual Mode access in the same time, Reg(0xbfb40054) is 0x%08x\n", AUTO_MANUAL_INT, VPint(0xbfb40054));
		WriteReg(SF_INTERRUPT, 1);
	} else {
		printk("\n>>> %d: Strange!! should not come here\n", AUTO_MANUAL_INT);
	}
}

#ifdef TCSUPPORT_DUAL_IMAGE_ENHANCE
int offset = 0;
#endif

static struct mtd_info *spiflash_probe(struct map_info *map)
{
	struct spi_chip_info *chip_info = NULL;
	struct mtd_info *mtd=NULL;

	chip_info = spiflash_probe_tc3162(map);
	if (!chip_info)
		return NULL;

	mtd = kzalloc(sizeof(*mtd), GFP_KERNEL);
	if (!mtd) {
		printk(KERN_WARNING "Failed to allocate memory for MTD device\n");
		return NULL;
	}

	mtd->priv = map;
	mtd->type = MTD_NORFLASH;
	mtd->erase   = mtd_spiflash_erase;
	mtd->write   = mtd_spiflash_write;
	mtd->read    = mtd_spiflash_read;
	mtd->flags   = MTD_CAP_NORFLASH;
	mtd->name    = map->name;
	mtd->writesize = 1;
	mtd->owner   = THIS_MODULE;

	mtd->size      = chip_info->flash->DeviceSize;
	mtd->erasesize = chip_info->flash->EraseSize;

	map->fldrv_priv = chip_info;

	printk(KERN_INFO "%s: Found SPIFLASH %dMiB %s\n",
		map->name, chip_info->flash->DeviceSize/(1024*1024), chip_info->flash->name);
#ifdef TCSUPPORT_DUAL_IMAGE_ENHANCE
		offset = chip_info->flash->DeviceSize/2;
#endif

	spi_nor_mtd = mtd;

	return mtd;
}

static void spiflash_destroy(struct mtd_info *mtd)
{
	struct map_info *map = (struct map_info *)mtd->priv;
	struct spi_chip_info *chip_info = (struct spi_chip_info *)map->fldrv_priv;

	if (chip_info->destroy) {
		chip_info->destroy(chip_info);
	}
}

static struct mtd_chip_driver spiflash_chipdrv = {
	.probe	 = spiflash_probe,
	.destroy = spiflash_destroy,
	.name	 = "spiflash_probe",
	.module	 = THIS_MODULE
};

#if defined(TCSUPPORT_SUPPORT_FLASH)
static int read_proc_support_flash(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	int len;
	unsigned int flash_id;
	int  index_flash;
	char *buf_proc = NULL;
	char buf_line[200];
	char buf_name[100];
	char *buf_replace = NULL;
	int total_len=0;

	
	buf_proc = kmalloc(4*1024, GFP_KERNEL);
	if (!buf_proc) 
	{
		printk(KERN_WARNING "Failed to allocate memory for /proc/tc3162/support_flash\n");
		return 0;
	}

	memset(buf_proc,0,4*1024);

	for(index_flash=0; index_flash < sizeof(flash_tables)/sizeof(struct spi_flash_info); index_flash++)
	{
		strcpy(buf_name,flash_tables[index_flash].name);//replace whitespace with '_'
		while( (buf_replace=strstr(buf_name, " "))!=NULL)
			*buf_replace='#';

		if(flash_tables[index_flash].DeviceSize/0x100000 <4)
			continue;

		flash_id = (flash_tables[index_flash].mfr_id <<16) | ( flash_tables[index_flash].dev_id & 0xffff);

		sprintf(buf_line,"%s %#x %d \n",buf_name , flash_id, 
			flash_tables[index_flash].DeviceSize/0x100000);

		total_len += strlen(buf_line);
		if(total_len>4*1024)
			break;
		strcat(buf_proc,buf_line);
	}

	len = sprintf(page, "%s", buf_proc);

	len -= off;
	*start = page + off;

	if (len > count)
		len = count;
	else
		*eof = 1;

	if (len < 0)
		len = 0;
	
	kfree(buf_proc);
	return len;
}
#endif

static int __init spiflash_probe_init(void)
{
   	int index=0;
	unsigned long flash_id=0;
#if defined(TCSUPPORT_SUPPORT_FLASH)
	create_proc_read_entry("tc3162/support_flash", 0, NULL, read_proc_support_flash, NULL);
#endif

	printk("\nSPI Flash Init Start\n");
	/* enable SF_MAXMUX_SEL */
	WriteReg(SF_MACMUX_SEL, MANUAL_MACMUXSEL);

	flash_id = spiflash_read_id();

	for (index=0; index < ARRAY_SIZE(flash_tables); index++) {
		if ((MANUFACTURER_ID(flash_id) == flash_tables[index].mfr_id) &&
		    (DEVICE_ID(flash_id) == flash_tables[index].dev_id)) {
			if ( (MANUFACTURER_ID(flash_id) == MANUFACTURER_WINBOND) && (DEVICE_ID(flash_id) == W25Q256) ) {
				is_W25Q256 = 1;
			}
			if ( (MANUFACTURER_ID(flash_id) == MANUFACTURER_MXIC) && (DEVICE_ID(flash_id) == MX25L25635E) ) {
				is_MX25L256 = 1;
			}
			if ( (MANUFACTURER_ID(flash_id) == MANUFACTURER_SPANSION) && (DEVICE_ID(flash_id) == S25FL256S) ) {
				is_S25FL256S = 1;
			}
			if ( (MANUFACTURER_ID(flash_id) == MANUFACTURER_NUMONYX) && (DEVICE_ID(flash_id) == N25Q256) ) {
				is_N25Q256 = 1;
			}
			if ( flash_tables[index].DeviceSize == SIZE_32MiB) {
				is_SPI_32MB = 1;
			}
			break;
		}
	}
	
	if (index == ARRAY_SIZE(flash_tables)) {
    	printk ("Found no SPI flash device\n");
    	return -1;
   	}
	
	if(flash_tables[index].DeviceSize >= SIZE_32MiB){
		//enter_dualIO_enable_mode();
		enter_4Byte_mode();
		SEND_AUTO_4B_CMD;
	} else {
		SEND_AUTO_3B_CMD;
	}
	SEND_AUTO_READ_CMD;
	
	SEND_WRITE_REGISTER_CMD(SF_OP_WR_STATUS1, SF_SR1_DEFAULT);
	
	// set SFC Clock to 25MHZ
	WriteReg(SF_CLK_CHANGE, 6);
	// set Tn to 1
	WriteReg(SF_SI_CK_SEL, 9);

	printk("\nSPI Flash Init Finish\n");

	VPint(SF_INTERRUPT_EN) |= AUTO_MANUAL_INTR_EN;
	if (request_irq(AUTO_MANUAL_INT, sfc_AutoManualInterrupt_handler, 0, "SFC_AutoManual", NULL) != 0) {
		printk("can not allocate IRQ for auto_manual_interrupt.\n");
		return -1;
	}
	VPint(CR_INTC_IMR_1) |= (0x01 << (AUTO_MANUAL_INT-33));

	register_mtd_chip_driver(&spiflash_chipdrv);
	return 0;
}

static void __exit spiflash_probe_exit(void)
{
#if defined(TCSUPPORT_SUPPORT_FLASH)
	remove_proc_entry("tc3162/support_flash",  NULL);
#endif

	unregister_mtd_chip_driver(&spiflash_chipdrv);
}

#if defined(TCSUPPORT_NEW_SPIFLASH_DEBUG)
static void SF_Test_Helper(){
#if 1
	printk("\nusage: \tsftest [Round] [AutoRD:0~15] [ManualRD:0~15] [MuxRD:0~1] [isProg:0~1] [CLK Rate:0~31] [Exit:0~1] [3B4B:0~2] [Pattern] [StartAddr] [RDIdle] [Tn]\n");
#else
	printk("\nusage: \tsftest [Round] [AutoRD:0~15] [ManualRD:0~15] [MuxRD:0~1] [isProg:0~1] [CLK Rate:0~2] [Exit:0~1] [3B4B:0~2] [Pattern] [StartAddr] [RDIdle] [Tn]\n");
#endif
	printk("Round: \t\t0~65535 means test round.   DEFAULT VALUE is 10\n");
	printk("AutoRD: \t0~15 is b'0000~1111, bit3 means Auto_singleRead, bit2 means Auto_FastRead, bit1 means Auto_FastDualOutput, \n\t\tbit0 means Auto_FastDualIO.   DEFAULT VALUE is 8\n");
	printk("ManualRD: \t0~15 is b'0000~1111, bit3 means Manual_singleRead, bit2 means Manual_FastRead, bit1 means Manual_FastDualOutput, \n\t\tbit0 means Manual_FastDualIO.   DEFAULT VALUE is 0\n");
	printk("MuxRD: \t\t0 means disable MuxRead, 1 means enable MuxRead.   DEFAULT VALUE is 0\n");
	printk("isProg: \t0 means disable Program, 1 means enable Program.   DEFAULT VALUE is 0\n");
#if 1
	printk("CLK Rate: \t0 means 15MHZ, 1 means (150/1)MHZ, 2 means (150/2)MHZ, 3 means (150/3)MHZ, etc...   DEFAULT VALUE is 0\n");
#else
	printk("CLK Rate: \t0 means 25MHZ, 1 means 50MHZ, 2 means CLK Rate change on the fly.   DEFAULT VALUE is 0\n");
#endif
	printk("Exit: \t\t0 means continue test while error happens, 1 means exit test while error happens.   DEFAULT VALUE is 0\n");
	printk("3B4B: \t\t0 means 4Bytes access all 32MB, 1 means 3Bytes access front 16MB, 2 means uses 13/0C/3C/BC. DEFAULT VALUE is 0\n");
	printk("TestPattern: \t0:normal test, 1:A5, 2:5A, 3:FF, 4:00, 5:55, 6:AA, 7:96, 8:69, 9:CC, 10:33, 11:Random. DEFAULT VALUE is 0\n");
	printk("StartAddr: \tDEFAULT VALUE is 0xF00000\n\n");
	printk("RDIdle: \t0: Disable Read IDLE, 1: Enable Read IDLE.   DEFAULT VALUE is 0\n");
	printk("Tn: \t0~15 is b'000000~111111, the value of bit0~bit5 means T0~T5 enable or desable.   DEFAULT VALUE is 0\n");
}

unsigned long sf_rand_ascend()
{
	static unsigned long rand_asc=0;
	rand_asc++;
	return rand_asc;
}
unsigned long sf_rand_descend()
{
	static unsigned long rand_des=0;
	rand_des--;
	return rand_des;
}
unsigned long sf_rand()
{
	static unsigned long rand_item=0;
	rand_item = (rand_item * 123 + 59) % 65536;
	return rand_item;
}

void spiflash_Tn_change(void)
{
	static int Tn_cur_index = 0;
	SEND_TN_CMD(TnMappingTab[(Tn_cur_index++)%Tn_cnt]);
}

#if 1
void spiflash_clk_change(int mode)
{
	WriteReg(SF_CLK_CHANGE, mode);
}

void spiflash_clk_change_25MHZ()
{
	spiflash_clk_change(6);
}

void spiflash_clk_change_50MHZ()
{
	spiflash_clk_change(3);
}
#else
void spiflash_clk_change(int mode)
{
	if(mode & 0x1) {
		SEND_CLK_25MHZ_CMD;
		clk_rate = 0;
	}
	else {
		SEND_CLK_50MHZ_CMD;
		clk_rate = 1;
	}
}

void spiflash_clk_change_25MHZ()
{
	SEND_CLK_25MHZ_CMD;
}

void spiflash_clk_change_50MHZ()
{
	SEND_CLK_50MHZ_CMD;
}
#endif

int spiflash_autoread_test(struct mtd_info *mtd,
	unsigned long from, unsigned char *buf, unsigned long len, unsigned long *retlen, u32 mode)
{
	mode %= AutoRead_cnt;
	*retlen = AutoMappingTab[mode];
	if(Tn_cnt >= 2)
		spiflash_Tn_change();
#if 0
	if(CLKRate_Value == 2)
		spiflash_clk_change(mode);
#endif
	switch(AutoMappingTab[mode]) {
		case 0:
			SEND_AUTO_READ_CMD;
			break;
		case 1:
			SEND_AUTO_FASTREAD_CMD;
			break;
		case 2:
			SEND_AUTO_FASTREAD_DUALOUT_CMD;
			break;
		case 3:
			SEND_AUTO_FASTREAD_DUALIO_CMD;
			break;
	}
	return spiflash_read(mtd->priv, from, (u32)buf, len);
}

int spiflash_manualread_test(struct mtd_info *mtd,
	unsigned long from, unsigned char *buf, unsigned long len, unsigned long *retlen, u32 mode)
{
	mode %= ManualRead_cnt;
	*retlen = ManualMappingTab[mode];
	if(Tn_cnt >= 2)
		spiflash_Tn_change();
#if 0
	if(CLKRate_Value == 2)
		spiflash_clk_change(mode);
#endif
	switch(ManualMappingTab[mode]) {
	case 0:
		return spiflash_manual_read_data(mtd, from, buf, len);
	case 1:
		return spiflash_manual_fast_read(mtd, from, buf, len);
	case 2:
		return spiflash_manual_fast_read_dualOut(mtd, from, buf, len);
	case 3:
		return spiflash_manual_fast_read_dualIO(mtd, from, buf, len);
	}
}

int spiflash_muxread_test(struct mtd_info *mtd,
	unsigned long from, unsigned char *buf, unsigned long len, unsigned long *retlen, u32 mode)
{
	mode = sf_rand() % MuxRead_cnt;
	*retlen = MuxMappingTab[mode];
	if(Tn_cnt >= 2)
		spiflash_Tn_change();
#if 0
	if(CLKRate_Value == 2)
		spiflash_clk_change(mode);
#endif
	switch(MuxMappingTab[mode]) {
		case 0:
			SEND_AUTO_READ_CMD;
			break;
		case 1:
			SEND_AUTO_FASTREAD_CMD;
			break;
		case 2:
			SEND_AUTO_FASTREAD_DUALOUT_CMD;
			break;
		case 3:
			SEND_AUTO_FASTREAD_DUALIO_CMD;
			break;
		case 4:
			return spiflash_manual_read_data(mtd, from, buf, len);
		case 5:
			return spiflash_manual_fast_read(mtd, from, buf, len);
		case 6:
			return spiflash_manual_fast_read_dualOut(mtd, from, buf, len);
		case 7:
			return spiflash_manual_fast_read_dualIO(mtd, from, buf, len);
	}
	return spiflash_read(mtd->priv, from, (u32)buf, len);
}


int sf_basic_test(u32 round, u32 readtest)
{
	unsigned long retlen=0;
	unsigned long erasesize=0;
	unsigned long size=0;
	unsigned long cur_se_off=0;
	unsigned long cur_pp_off=0;
	unsigned long start_addr=0;
	unsigned char se_buf[SE_MAX];
	unsigned char pp_buf[PP_MAX];
	unsigned char seed=0;
	int cur_round=0;
	int i=0, j=0;
	int index=0;
	u32 read_mode=0;
	u32 begtime=0, endtime=0, passtime=0;
	struct erase_info instr;
	struct map_info *map=NULL;
	struct mtd_info *mtd=NULL;

	map = kzalloc(sizeof(struct map_info), GFP_KERNEL);
	if (!map) {
		printk(KERN_WARNING "Failed to allocate memory for MAP\n");
		return NULL;
	}
	map->virt = 0xBC000000;
	
	/* check flash ID */		
	mtd = spiflash_probe(map);
	if (!mtd) {
		printk ("\n########spiflash ID read error\n");
	}
	
	erasesize = mtd->erasesize;
	size = mtd->size;
	start_addr = StartAddress_Value;

	cur_round = round % (TP_MAX+1);
	//read_mode = round % 5;
	if(readtest == AUTOREAD_TEST)
		printk("\n\n\n>>>>>>> Round%d Test::current test is AUTOREAD_TEST >>>>>>\n", round);
	else if(readtest == MANUALREAD_TEST)
		printk("\n\n\n>>>>>>> Round%d Test::current test is MANUALREAD_TEST >>>>>>\n", round);
	else if(readtest == MUXREAD_TEST)
		printk("\n\n\n>>>>>>> Round%d Test::current test is MUXREAD_TEST >>>>>>\n", round);
		
#if 1
	begtime = getTime();
#endif
	for(cur_se_off = start_addr; (cur_se_off + erasesize) <= size; cur_se_off += erasesize) {
		/* erase one block, the size is 64KB */
		instr.addr = cur_se_off;
		instr.len = erasesize;
		tc3162wdog_kick();
		mtd_spiflash_erase(mtd, &instr);
		//printk("\n\n Erase flash from %X to %X\n", cur_se_off, cur_se_off + erasesize);
		
		/* check the content of the block */
		for(j=0; j<SE_CNT; j++){
			//mtd_spiflash_read(mtd, cur_se_off+j*SE_MAX, SE_MAX, &retlen, se_buf);
			if(readtest == AUTOREAD_TEST)
				spiflash_autoread_test(mtd, cur_se_off+j*SE_MAX, se_buf, SE_MAX, &retlen, (++read_mode));
			else if(readtest == MANUALREAD_TEST)
				spiflash_manualread_test(mtd, cur_se_off+j*SE_MAX, se_buf, SE_MAX, &retlen, (++read_mode));
			else if(readtest == MUXREAD_TEST)
				spiflash_muxread_test(mtd, cur_se_off+j*SE_MAX, se_buf, SE_MAX, &retlen, (++read_mode));
			for(i=0; i<SE_MAX; i++) {
				if(se_buf[i] != 0xFF) {
					if(readtest == MUXREAD_TEST)
						printk("\n####Erase fail at Address %X::index is %d, value is %X, should be %X, ReadMode is %s::%s, reg 0xBFA10000 is 0x%08x, clk_rate is %s\n", cur_se_off+j*SE_MAX+i, i, se_buf[i], 0xFF, TestMode_TEXT[readtest], MuxMode_TEXT[retlen & 0x7], SF_READ_MODE_VALUE, CLKRate_TEXT[clk_rate]);
					else
						printk("\n####Erase fail at Address %X::index is %d, value is %X, should be %X, ReadMode is %s::%s, reg 0xBFA10000 is 0x%08x, clk_rate is %s\n", cur_se_off+j*SE_MAX+i, i, se_buf[i], 0xFF, TestMode_TEXT[readtest], ReadMode_TEXT[retlen & 0x3], SF_READ_MODE_VALUE, CLKRate_TEXT[clk_rate]);
					dumpCell_buffer((u8 *)(se_buf), SE_MAX, cur_se_off+j*SE_MAX);
					if(Exit_Value == 1)
						goto erase_fail;
					else
						break;
				}
			}
		}

		if(isProgram_Value == 1) {
			for(cur_pp_off = cur_se_off; (cur_pp_off + PP_MAX) <= (cur_se_off + erasesize); cur_pp_off += PP_MAX) {
				/* program 256Bytes of current blcok */
				if(TestPattern_Value == 0) {
					if(cur_round == TP_MAX) {
						seed = sf_rand() % SEED_MAX;
						for(i=0; i<PP_MAX; i++) {
							pp_buf[i] = (seed+i) % SEED_MAX;
						}
					} else {
						for(i=0; i<PP_MAX; i++) {
							pp_buf[i] = TestPattern[cur_round];
						}
					}
				} else if(TestPattern_Value == 11) {
					seed = sf_rand() % SEED_MAX;
					for(i=0; i<PP_MAX; i++) {
						pp_buf[i] = (seed+i) % SEED_MAX;
					}
				} else {
					for(i=0; i<PP_MAX; i++) {
						pp_buf[i] = TestPattern[TestPattern_Value-1];
					}
				}
				mtd_spiflash_write(mtd, cur_pp_off, PP_MAX, &retlen, pp_buf);
				//printk("\n Program flash from %X to %X", cur_pp_off, cur_pp_off + PP_MAX);
				
				/* check the content of current 256 Bytes */
				//mtd_spiflash_read(mtd, cur_pp_off, PP_MAX, &retlen, pp_buf);
				if(readtest == AUTOREAD_TEST)
					spiflash_autoread_test(mtd, cur_pp_off, pp_buf, PP_MAX, &retlen, (++read_mode));
				else if(readtest == MANUALREAD_TEST)
					spiflash_manualread_test(mtd, cur_pp_off, pp_buf, PP_MAX, &retlen, (++read_mode));
				else if(readtest == MUXREAD_TEST)
					spiflash_muxread_test(mtd, cur_pp_off, pp_buf, PP_MAX, &retlen, (++read_mode));
				for(i=0; i<PP_MAX; i++) {
					if( pp_buf[i] != ((TestPattern_Value==0)?((cur_round == TP_MAX)?((seed+i) % SEED_MAX):(TestPattern[cur_round])):((TestPattern_Value==11)?((seed+i) % SEED_MAX):(TestPattern[TestPattern_Value-1]))) ) {
						if(readtest == MUXREAD_TEST)
							printk("\n####Program fail at Address %X::index is %d, value is %X, should be %X, ReadMode is %s::%s, reg 0xBFA10000 is 0x%08x, clk_rate is %s\n", cur_pp_off+i, i, pp_buf[i], ((TestPattern_Value==0)?((cur_round == TP_MAX)?((seed+i) % SEED_MAX):(TestPattern[cur_round])):((TestPattern_Value==11)?((seed+i) % SEED_MAX):(TestPattern[TestPattern_Value-1]))), TestMode_TEXT[readtest], MuxMode_TEXT[retlen & 0x7], SF_READ_MODE_VALUE, CLKRate_TEXT[clk_rate]);
						else
							printk("\n####Program fail at Address %X::index is %d, value is %X, should be %X, ReadMode is %s::%s, reg 0xBFA10000 is 0x%08x, clk_rate is %s\n", cur_pp_off+i, i, pp_buf[i], ((TestPattern_Value==0)?((cur_round == TP_MAX)?((seed+i) % SEED_MAX):(TestPattern[cur_round])):((TestPattern_Value==11)?((seed+i) % SEED_MAX):(TestPattern[TestPattern_Value-1]))), TestMode_TEXT[readtest], ReadMode_TEXT[retlen & 0x3], SF_READ_MODE_VALUE, CLKRate_TEXT[clk_rate]);
						dumpCell_buffer((u8 *)(pp_buf), PP_MAX, cur_pp_off);
						if(Exit_Value == 1)
							goto program_fail;
						else
							break;
					} 
				}
			}
		}
	}
#if 1
	endtime = getTime();
	if(endtime < begtime)
		passtime = 0xffffffff - begtime + endtime;
	else
		passtime = endtime - begtime;
	printk("\n>>>>>>> Round%d Test Finish::passtime is %lums\n", round, passtime);
	printk("\nCurrent Time is %d:%d:%d\n", endtime/3600000, (endtime/60000)%60, (endtime/1000)%60);
#endif

	kzfree(map->fldrv_priv);
	kzfree(map);
	kzfree(mtd);

	return 0;

erase_fail:
	return -1;
	
program_fail:
	return -2;
}

int sf_manual_test(u32 round)
{
	return sf_basic_test(round, MANUALREAD_TEST);
}

int sf_auto_test(u32 round)
{
	return sf_basic_test(round, AUTOREAD_TEST);
}

int sf_mux_test(u32 round)
{
	return sf_basic_test(round, MUXREAD_TEST);
}

int sf_basic_test_entry(int argc, char *argv[])
{
	int i=0;
	u32 round = 0;
	u32 pattern = 0;
	int result = 0;	
	struct map_info *map=NULL;
	struct mtd_info *mtd=NULL;
	
	clk_rate = 0;
	Round_Value = 10;
	AutoRead_Value = 8;
	ManualRead_Value = 0;
	MuxRead_Value = 0;
	isProgram_Value = 0;
	CLKRate_Value = 0;
	Exit_Value = 0;
	Address3B4B_Value = 0;
	TestPattern_Value = 0;
	StartAddress_Value = 0xF00000;
	ReadIdle_Value = 0;
	Tn_Value = 0;
	Tn_cnt = 0;
	TnMappingTab[0] = 0;

	AutoRead_cnt = 0;
	ManualRead_cnt = 0;
	MuxRead_cnt = 0;
	
	AutoRead_cnt = 1;
	AutoMappingTab[0] = 0;
	
	if(argc == 1){
		SF_Test_Helper();
		return -1;
	}
	
	if(argc >= 2){
		Round_Value = simple_strtoul(argv[1], NULL, 10);
		printk("Round is %lu\n", Round_Value);
	}
	if(argc >= 3){
		AutoRead_Value = simple_strtoul(argv[2], NULL, 10);
		if(AutoRead_Value > 15) {
			printk("\nError: AutoRead Value must be [0~15]!\n");
			SF_Test_Helper();
			return -3;
		}
		else {
			AutoRead_cnt = 0;
			MuxRead_cnt = 0;
			printk("AutoRead Value is %u, ", AutoRead_Value);
			if(AutoRead_Value == 0)
				printk("no Auto Read Test\n");
			else {
				printk("including ");
				if(AutoRead_Value & 0x8) {
					printk("Single Read, ");
					AutoMappingTab[AutoRead_cnt++] = 0;
					MuxMappingTab[MuxRead_cnt++] = 0;
				}
				if(AutoRead_Value & 0x4) {
					printk("Fast Read, ");
					AutoMappingTab[AutoRead_cnt++] = 1;
					MuxMappingTab[MuxRead_cnt++] = 1;
				}
				if(AutoRead_Value & 0x2) {
					printk("Fast Read Dual Output, ");
					AutoMappingTab[AutoRead_cnt++] = 2;
					MuxMappingTab[MuxRead_cnt++] = 2;
				}
				if(AutoRead_Value & 0x1) {
					printk("Fast Read Dual IO, ");
					AutoMappingTab[AutoRead_cnt++] = 3;
					MuxMappingTab[MuxRead_cnt++] = 3;
				}
				printk("\n");
			}
		}
	}
	if(argc >= 4){
		ManualRead_Value = simple_strtoul(argv[3], NULL, 10);
		if(ManualRead_Value > 15) {
			printk("\nError: ManualRead Value must be [0~15]!\n");
			SF_Test_Helper();
			return -4;
		}
		else {
			ManualRead_cnt = 0;
			printk("ManualRead Value is %u, ", ManualRead_Value);
			if(ManualRead_Value == 0)
				printk("no Manual Read Test\n");
			else {
				printk("including ");
				if(ManualRead_Value & 0x8) {
					printk("Single Read, ");
					ManualMappingTab[ManualRead_cnt++] = 0;
					MuxMappingTab[MuxRead_cnt++] = 4;
				}
				if(ManualRead_Value & 0x4) {
					printk("Fast Read, ");
					ManualMappingTab[ManualRead_cnt++] = 1;
					MuxMappingTab[MuxRead_cnt++] = 5;
				}
				if(ManualRead_Value & 0x2) {
					printk("Fast Read Dual Output, ");
					ManualMappingTab[ManualRead_cnt++] = 2;
					MuxMappingTab[MuxRead_cnt++] = 6;
				}
				if(ManualRead_Value & 0x1) {
					printk("Fast Read Dual IO, ");
					ManualMappingTab[ManualRead_cnt++] = 3;
					MuxMappingTab[MuxRead_cnt++] = 7;
				}
				printk("\n");
			}
		}
	}
	if(argc >= 5){
		MuxRead_Value = simple_strtoul(argv[4], NULL, 10);
		if(MuxRead_Value > 1) {
			printk("\nError: MuxRead Value must be [0~1]!\n");
			SF_Test_Helper();
			return -5;
		}
		else
			printk("MuxRead is %s\n", ((MuxRead_Value==1)?"enable":"disable"));
	}
	if(argc >= 6){
		isProgram_Value = simple_strtoul(argv[5], NULL, 10);
		if(isProgram_Value > 1) {
			printk("\nError: isProgram Value must be [0~1]!\n");
			SF_Test_Helper();
			return -6;
		}
		else
			printk("Program is %s\n", ((isProgram_Value==1)?"enable":"disable"));
	}
#if 1
	if(argc >= 7){
		CLKRate_Value = simple_strtoul(argv[6], NULL, 10);
		if(CLKRate_Value > 31) {
			printk("\nError: CLK Rate Value must be [0~31]!\n");
			SF_Test_Helper();
			return -7;
		}
		else {
			if(CLKRate_Value == 0)
				CLKRate_Value = 10;
			printk("CLK Rate is %dMHZ\n", (SFC_CLOCK_MAX / CLKRate_Value));
		}
	}
#else
	if(argc >= 7){
		CLKRate_Value = simple_strtoul(argv[6], NULL, 10);
		if(CLKRate_Value > 2) {
			printk("\nError: CLK Rate Value must be [0~2]!\n");
			SF_Test_Helper();
			return -7;
		}
		else
			printk("CLK Rate is %s\n", CLKRate_TEXT[CLKRate_Value]);
	}
#endif
	if(argc >= 8){
		Exit_Value = simple_strtoul(argv[7], NULL, 10);
		if(Exit_Value > 1) {
			printk("\nError: Exit Value must be [0~1]!\n");
			SF_Test_Helper();
			return -8;
		}
		else
			printk("Exit while error is %s\n", ((Exit_Value==1)?"enable":"disable"));
	}
	if(argc >= 9){
		Address3B4B_Value = simple_strtoul(argv[8], NULL, 10);
		if(Address3B4B_Value > 2) {
			printk("\nError: 3B4B Value must be [0~2]!\n");
			SF_Test_Helper();
			return -9;
		}
		else
			printk("The way to access 32MB flash is %s\n", Address3B4B_TEXT[Address3B4B_Value]);
	}
	if(argc >= 10){
		TestPattern_Value = simple_strtoul(argv[9], NULL, 10);
		if(TestPattern_Value > 11) {
			printk("\nError: Pattern Value must be [0~11]!\n");
			SF_Test_Helper();
			return -10;
		}
		else {
			if(TestPattern_Value == 0)
				printk("TestPattern is Normal Test\n");
			else if(TestPattern_Value == 11)
				printk("TestPattern is Increment Digital\n");
			else
				printk("TestPattern is %X\n", TestPattern[TestPattern_Value-1]);
		}
	}
	if(argc >= 11){
		StartAddress_Value = simple_strtoul(argv[10], NULL, 16);
		StartAddress_Value &= 0xFFFF0000;
		if(StartAddress_Value < 0x20000) {
			StartAddress_Value = 0x20000;
		}
		printk("Test Start Address 0x%08x\n", StartAddress_Value);
	}

	if(argc >= 12){
		ReadIdle_Value = simple_strtoul(argv[11], NULL, 10);
		if(ReadIdle_Value > 1) {
			printk("\nError: RDIdle Value must be [0~1]!\n");
			SF_Test_Helper();
			return -11;
		}
		else
			printk("Read Idle is %s\n", ((ReadIdle_Value==1)?"enable":"disable"));
	}
	if(argc >= 13){
		Tn_Value = simple_strtoul(argv[12], NULL, 10);
		if(Tn_Value > 0x3F) {
			printk("\nError: Tn Value must be [0~0x3F]!\n");
			SF_Test_Helper();
			return -12;
		}
		else {
			Tn_cnt = 0;
			if(Tn_Value == 0)
				printk("Tn Switching Disable\n");
			else if((Tn_Value & (Tn_Value-1)) == 0) {
				Tn_cnt = 1;
				printk("Tn Switching unnecessary\n");
			} else {
				if(Tn_Value & 0x1) {
					printk("T0 ");
					TnMappingTab[Tn_cnt++] = 0;
				}
				if(Tn_Value & 0x2) {
					printk("T1 ");
					TnMappingTab[Tn_cnt++] = 1;
				}
				if(Tn_Value & 0x4) {
					printk("T2 ");
					TnMappingTab[Tn_cnt++] = 2;
				}
				if(Tn_Value & 0x8) {
					printk("T3 ");
					TnMappingTab[Tn_cnt++] = 3;
				}
				if(Tn_Value & 0x10) {
					printk("T4 ");
					TnMappingTab[Tn_cnt++] = 4;
				}
				if(Tn_Value & 0x20) {
					printk("T5 ");
					TnMappingTab[Tn_cnt++] = 5;
				}
				printk("is Switching now!\n");
			}
		}
	}
	
#if 1
	spiflash_clk_change(CLKRate_Value);
	clk_rate = 0;
#else
	if(CLKRate_Value == 0) {
		clk_rate = 0;
		spiflash_clk_change_25MHZ();
	} else if(CLKRate_Value == 1) {
		clk_rate = 1;
		spiflash_clk_change_50MHZ();
	}
#endif

	map = kzalloc(sizeof(struct map_info), GFP_KERNEL);
	if (!map) {
		printk(KERN_WARNING "Failed to allocate memory for MAP\n");
		return NULL;
	}
	map->virt = 0xBC000000;
	
	/* check flash ID */		
	mtd = spiflash_probe(map);
	if (!mtd) {
		printk ("\n########spiflash ID read error\n");
	}
	
	if(mtd->size >= SIZE_32MiB){
		if(Address3B4B_Value == 0) {
			enter_4Byte_mode();
			SEND_AUTO_4B_CMD;
    		printk("\n>>>come in 4Bytes Mode\n");
		} else {
			exit_4Byte_mode();
			SEND_AUTO_3B_CMD;
    		printk("\n>>>exit 4Bytes Mode\n");
		}
	}

	printk("\nAutoRead_cnt is %u, ManualRead_cnt is %u, MuxRead_cnt is %u\n", AutoRead_cnt, ManualRead_cnt, MuxRead_cnt);
	for(i=0; i<AutoRead_cnt; i++)
		printk("AutoRead[%d] is %u\n", i, AutoMappingTab[i]);
	for(i=0; i<ManualRead_cnt; i++)
		printk("ManualRead[%d] is %u\n", i, ManualMappingTab[i]);
	for(i=0; i<MuxRead_cnt; i++)
		printk("MuxRead[%d] is %u\n", i, MuxMappingTab[i]);
	
	printk("\n>>>>>>>>>>>> SPI FLASH Test Begin!!!\n");
	printk("\nVPint(CR_TIMER1_LDV) is 0x%08X\n", VPint(CR_TIMER1_LDV));
	//VPint(CR_TIMER1_LDV) = 0xFFFFFFFF;
	printk("\nVPint(CR_TIMER1_LDV) is 0x%08X\n", VPint(CR_TIMER1_LDV));

#if 0
	sf_regDef_test();
	for(pattern=0; pattern<16; pattern++)
		sf_regRW_test(pattern);
	sf_regRW_test(0xFFFFFFFF);
#endif
	
	for(round = 0; round < Round_Value; round++) {
		if(AutoRead_Value != 0) {
			result = sf_auto_test(round);
			if((result != 0) && (Exit_Value == 1))
				return -13;
		}
		if(ManualRead_Value != 0) {
			result = sf_manual_test(round);
			if((result != 0) && (Exit_Value == 1))
				return -14;
		}
		if(MuxRead_Value != 0) {
			result = sf_mux_test(round);
			if((result != 0) && (Exit_Value == 1))
				return -15;
		}
	}
	
	kzfree(map->fldrv_priv);
	kzfree(map);
	kzfree(mtd);
	return 0;
}

struct map_info *map2=NULL;
struct mtd_info *mtd2=NULL;
unsigned long cur_se_off2=0x00600000;
unsigned long max_size=0x00700000;
int sf_task2_test()
{
	unsigned long retlen=0;
	unsigned char se_buf[SE_MAX];
	int i=0, j=0;
	uint32 read_mode=0;
	struct erase_info instr;
	static unsigned long erasesize=0;
	static unsigned long size=0;
	static int first_flag = 0;
	static unsigned char pp_data = 1;
	unsigned char reg_sr1=0;
	unsigned long cur_pp_off=0;
	unsigned char pp_buf[PP_MAX];

	if(first_flag == 0) {
		printk("\nTASK2::Init map2 & mtd2");
		map2 = kzalloc(sizeof(struct map_info), GFP_KERNEL);
		if (!map2) {
			printk(KERN_WARNING "\nTASK2::Failed to allocate memory for MAP\n");
			return NULL;
		}
		map2->virt = 0xBC000000;

		/* check flash ID */		
		mtd2 = spiflash_probe(map2);
		if (!mtd2) {
			printk ("\n########TASK::spiflash ID read error\n");
		}
		first_flag = 1;
	
		erasesize = mtd2->erasesize;
		size = mtd2->size;
	}

	instr.addr = cur_se_off2;
	instr.len = erasesize;
	instr.callback = NULL;
	mtd_spiflash_erase(mtd2, &instr);

	for(j=0; j<SE_CNT; j++){
		spiflash_autoread_test(mtd2, cur_se_off2+j*SE_MAX, se_buf, SE_MAX, &retlen, (++read_mode));
		for(i=0; i<SE_MAX; i++) {
			if(se_buf[i] != 0xFF) {
				SEND_READ_REGISTER_CMD(SF_OP_RD_STATUS1, reg_sr1);
				printk("\n##########TASK2::Erase fail at Address %X::index is %d, value is %X, should be %X, read_mode is %d, reg_sr1 is 0x%04x\n", cur_se_off2+j*SE_MAX+i, i, se_buf[i], 0xFF, AutoMappingTab[read_mode%AutoRead_cnt], reg_sr1);
				dumpCell_buffer((u8 *)(se_buf), SE_MAX, cur_se_off2+j*SE_MAX);
				goto e_fail;
			}
		}
	}

	for(i=0; i<PP_MAX; i++) {
		pp_buf[i] = pp_data;
	}
	for(cur_pp_off = cur_se_off2; (cur_pp_off + PP_MAX) <= (cur_se_off2 + erasesize); cur_pp_off += PP_MAX) {
		mtd_spiflash_write(mtd2, cur_pp_off, PP_MAX, &retlen, pp_buf);			
	}
	
e_fail:
	cur_se_off2 += erasesize;
	if(cur_se_off2 >= max_size) {
		cur_se_off2 = 0x00600000;
		pp_data++;
	}
	printk ("\nTASK2::finished___cur_se_off2 is 0x%08x\n", cur_se_off2);

#if 0
	spiflash_destroy(mtd2);
	kzfree(map2);
	kzfree(mtd2);
#endif
	return 0;
}

int sf_handler_test_entry(int argc, char *argv[])
{
	u32 i=0;
	u32 count=10;
	u32 autoMode=0;
	u8 data[3]={0};
	u8 to[257]={0};
	u32 from = 0;
	
	if(argc >= 2){
		count = simple_strtoul(argv[1], NULL, 10);
		printk("count is %lu\n", count);
	}
	if(argc >= 3){
		autoMode = simple_strtoul(argv[2], NULL, 10);
		printk("autoMode is %lu\n", autoMode);
	}
	
	for(i=0; i<count; i++) {
		  SEND_AUTO2MANUAL_CMD;
          SEND_CSL_CMD;
          SEND_OPFIFO_WRITE_CMD(OP_OS2IS, OP_REPEAT_ONE);
          SEND_DFIFO_WRITE_CMD(SF_OP_RD_ID);
          SEND_OPFIFO_WRITE_CMD(OP_INS, OP_REPEAT_THREE);
		  if(autoMode != 0) {
			memcpy((char *)to, (char *)(0xbc000000 + from), 0x100);
		  }
          SEND_DFIFO_READ_CMD(*(data));
          SEND_DFIFO_READ_CMD(*((data)+1));
          SEND_DFIFO_READ_CMD(*((data)+2));
          SEND_CSH_CMD;
		  SEND_MANUAL2AUTO_CMD;
		  printk("Flash ID is %x:%x:%x\n", data[0], data[1], data[2]);
	}

	return 0;
}

int sf_task2_test_entry(int argc, char *argv[])
{
	if(argc>=2){
		spi_task2_en = simple_strtoul(argv[1], NULL, 10);
	}
	printk("test2_flag is %lu\n", spi_task2_en);

	return 0;
}

EXPORT_SYMBOL(sf_task2_test_entry);
EXPORT_SYMBOL(spi_task2_en);
EXPORT_SYMBOL(sf_task2_test);
EXPORT_SYMBOL(sf_basic_test_entry);
EXPORT_SYMBOL(sf_handler_test_entry);
EXPORT_SYMBOL(spiflash_clk_change);

#endif
int sf_EPCheck_entry(int argc, char *argv[])
{	
	if(argc >= 2){
		epcheck_flag = simple_strtoul(argv[1], NULL, 10);
		if(epcheck_flag > 1) {
			printk("\nError: EPCheck Flag must be [0~1]!\n");
			return -1;
		}
	}
	printk("epcheck is %s\n", ((epcheck_flag==1)?"enable":"disable"));
	return 0;
}
int sf_EPCheck_enable(void)
{	
	epcheck_flag = 1;
	return 0;
}
int sf_EPCheck_disable(void)
{	
	epcheck_flag = 0;
	return 0;
}
#ifdef TCSUPPORT_AUTOBENCH
int sf_autobench_check(){//7505???
	int i=0;
	unsigned long flash_id=0;
	unsigned long retlen=0;
	unsigned long erasesize=0;
	unsigned long start_addr=0;
	unsigned char se_buf[SE_MAX]={0};
	unsigned char pp_buf[PP_MAX]={0};
	unsigned char rd_buf[PP_MAX]={0};
	unsigned char seed=0;
	struct erase_info instr;

	
	struct map_info *map=NULL;
	struct mtd_info *mtd=NULL;

	map = kzalloc(sizeof(struct map_info), GFP_KERNEL);
	if (!map) {
		printk(KERN_WARNING "Failed to allocate memory for MAP\n");
		return NULL;
	}
	map->virt = 0xBC000000;
	
	/* check flash ID */		
	mtd = spiflash_probe(map);
	if (!mtd) {
		printk ("\n########spiflash ID read error\n");
	}

	erasesize = mtd->erasesize;
	start_addr = 0x700000;

	/* Check Flash ID */
	flash_id = spiflash_read_id();
	for (i=0; i < ARRAY_SIZE(flash_tables); i++) {
			if ((MANUFACTURER_ID(flash_id) == flash_tables[i].mfr_id) &&
				(DEVICE_ID(flash_id) == flash_tables[i].dev_id)) {				
				printk("mfr id: %x \n", MANUFACTURER_ID(flash_id));
				printk("dev id: %x\n", DEVICE_ID(flash_id));

				break;
			}
	}

	if(i == ARRAY_SIZE(flash_tables)) {
		goto read_id_fail;
	}

	if(isEN751221){
		start_addr=0x30000; /* FOR SPI NOR TRX header */
		spiflash_read(mtd->priv, start_addr, rd_buf, 4);

		/* read TRX */
		if( (rd_buf[0]!=0x32) || (rd_buf[1]!=0x52) || (rd_buf[2]!=0x44) || (rd_buf[3]!=0x48) ){
			printk("\n SPI read fail\n");
			goto program_fail;
		}
	}else{	
		/* Erase and Check */
		//flash_erase(start_addr, erasesize);
		spiflash_erase(mtd, start_addr);
		//instr.addr = start_addr;
		//instr.len = erasesize;
			//tc3162wdog_kick();
		//mtd_spiflash_erase(mtd, &instr);
			
		//SEND_AUTO_READ_CMD;

		//spiflash_autoread_test(mtd, start_addr, se_buf, SE_MAX, &retlen, (++read_mode));


		spiflash_read(mtd->priv, start_addr, se_buf, SE_MAX);
		//spiflash_read_internal(&mtd, start_addr, SE_MAX, &retlen, se_buf);
		for(i=0; i<SE_MAX; i++) {
			if(se_buf[i] != 0xFF) {
				printk("\nAuto Read fail at Address %X::index is %d, value is %X, should be %X, curLen is %d\n", start_addr+i, i, se_buf[i], 0xFF, SE_MAX);
				goto erase_fail;
			}
		}
		
		/* Program and Check */
		seed = sf_rand() % SEED_MAX;
		for(i=0; i<SEED_MAX; i++) {
			pp_buf[i] = (seed+i) % SEED_MAX;
		}
		//flash_write(start_addr, PP_MAX, &retlen, pp_buf);
		//spiflash_write(start_addr, PP_MAX, &retlen, pp_buf);
		spiflash_write(mtd, pp_buf, start_addr, PP_MAX);
		//SEND_AUTO_READ_CMD;
		spiflash_read(mtd->priv, start_addr, rd_buf, PP_MAX);
		//spiflash_man_read_data(&mtd, start_addr, PP_MAX, &retlen, rd_buf);
		for(i=0; i<PP_MAX; i++) {
			if(rd_buf[i] != pp_buf[i]) {
				printk("\nRead fail at Address %X::index is %d, value is %X, should be %X, curLen is %d\n", start_addr+i, i, rd_buf[i], pp_buf[i], PP_MAX);
				goto program_fail;
			}
		}
	}
	return 0;
	
read_id_fail:
erase_fail:
program_fail:
	
	return -1;

}
EXPORT_SYMBOL(sf_autobench_check);
#endif
EXPORT_SYMBOL(sf_EPCheck_entry);
EXPORT_SYMBOL(sf_EPCheck_enable);
EXPORT_SYMBOL(sf_EPCheck_disable);

module_init(spiflash_probe_init);
module_exit(spiflash_probe_exit);
EXPORT_SYMBOL(SPI_SEM);

