/***************************************************************************************
 *      Copyright(c) 2014 ECONET Incorporation All rights reserved.
 *
 *      This is unpublished proprietary source code of ECONET Networks Incorporation
 *
 *      The copyright notice above does not evidence any actual or intended
 *      publication of such source code.
 ***************************************************************************************
 */

/*======================================================================================
 * MODULE NAME: spi
 * FILE NAME: spi_nand_flash.c
 * DATE: 2014/11/21
 * VERSION: 1.00
 * PURPOSE: To Provide SPI NAND Access interface.
 * NOTES:
 *
 * AUTHOR : Chuck Kuo         REVIEWED by
 *
 * FUNCTIONS
 *
 *      SPI_NAND_Flash_Init             To provide interface for SPI NAND init. 
 *      SPI_NAND_Flash_Write_Nbyte      To provide interface for Write N Bytes into SPI NAND Flash. 
 *      SPI_NAND_Flash_Read_Byte        To provide interface for read 1 Bytes from SPI NAND Flash. 
 *      SPI_NAND_Flash_Read_DWord       To provide interface for read Double Word from SPI NAND Flash.  
 *      SPI_NAND_Flash_Read_NByte       To provide interface for Read N Bytes from SPI NAND Flash. 
 *      SPI_NAND_Flash_Erase            To provide interface for Erase SPI NAND Flash. 
 * 
 * DEPENDENCIES
 *
 * * $History: $
 * MODIFICTION HISTORY:
 * Version 1.00 - Date 2014/11/21 By Chuck Kuo
 * ** This is the first versoin for creating to support the functions of
 *    current module.
 *
 *======================================================================================
 */

/* INCLUDE FILE DECLARATIONS --------------------------------------------------------- */
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

#include "spi_nand_flash.h"
#include "spi_controller.h"
#include <asm/io.h>
#include <asm/tc3162/tc3162.h>
#include <stdarg.h>


#include <linux/slab.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/wait.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>
#include <linux/mtd/nand_ecc.h>
#include <linux/dma-mapping.h>
#include <linux/jiffies.h>
#include <linux/platform_device.h>
#include <linux/proc_fs.h>
#include <linux/time.h>

#include <asm/io.h>
#include <asm/cacheflush.h>
#include <asm/uaccess.h>

#include <linux/miscdevice.h>
#include <linux/scatterlist.h>


#include "../ralink/bmt.h"


/* Added for TCM used */
#include <asm/system.h>

#include <linux/mtd/map.h>
#include <asm/tc3162/tc3162.h>

#if defined (TCSUPPORT_GPON_DUAL_IMAGE) || defined (TCSUPPORT_EPON_DUAL_IMAGE)
#include "flash_layout/tc_partition.h"
#endif

#ifdef TCSUPPORT_NAND_BMT
#define POOL_GOOD_BLOCK_PERCENT 8/100
#define SLAVE_IMAGE_OFFSET 0xf00000
extern int nand_logic_size;
#endif


DECLARE_MUTEX(SPI_NAND_SEM);  /* Make sure all related SPI NAND operations are atomic */

struct spi_chip_info {
	struct spi_flash_info *flash;
	void (*destroy)(struct spi_chip_info *chip_info);

	u32 (*read)(struct map_info *map, u32 from, u32 to, u32 size);
	u32 (*read_manual)(struct mtd_info *mtd, unsigned long from, unsigned char *buf, unsigned long len);
	u32 (*write)(struct mtd_info *mtd, u32 from, u32 to, u32 size);
	u32 (*erase)(struct mtd_info *mtd, u32 addr);
};

extern unsigned int (*ranand_read_byte)(unsigned long long);
extern unsigned int (*ranand_read_dword)(unsigned long long);


/* NAMING CONSTANT DECLARATIONS ------------------------------------------------------ */

/* SPI NAND Command Set */
#define _SPI_NAND_OP_GET_FEATURE			0x0F	/* Get Feature */
#define _SPI_NAND_OP_SET_FEATURE			0x1F	/* Set Feature */
#define _SPI_NAND_OP_PAGE_READ				0x13	/* Load page data into cache of SPI NAND chip */
#define _SPI_NAND_OP_READ_FROM_CACHE_SINGLE	0x03	/* Read data from cache of SPI NAND chip, single speed*/
#define _SPI_NAND_OP_READ_FROM_CACHE_DUAL	0x3B	/* Read data from cache of SPI NAND chip, dual speed*/
#define _SPI_NAND_OP_READ_FROM_CACHE_QUAD	0x6B	/* Read data from cache of SPI NAND chip, quad speed*/
#define _SPI_NAND_OP_WRITE_ENABLE			0x06	/* Enable write data to  SPI NAND chip */
#define _SPI_NAND_OP_WRITE_DISABLE			0x04	/* Reseting the Write Enable Latch (WEL) */
#define _SPI_NAND_OP_PROGRAM_LOAD_SINGLE			0x02	/* Write data into cache of SPI NAND chip with cache reset, single speed */
#define _SPI_NAND_OP_PROGRAM_LOAD_QUAD				0x32	/* Write data into cache of SPI NAND chip with cache reset, quad speed */
#define _SPI_NAND_OP_PROGRAM_LOAD_RAMDOM_SINGLE		0x84	/* Write data into cache of SPI NAND chip, single speed */
#define _SPI_NAND_OP_PROGRAM_LOAD_RAMDON_QUAD		0x34	/* Write data into cache of SPI NAND chip, quad speed */

#define _SPI_NAND_OP_PROGRAM_EXECUTE		0x10	/* Write data from cache into SPI NAND chip */
#define _SPI_NAND_OP_READ_ID				0x9F	/* Read Manufacture ID and Device ID */
#define _SPI_NAND_OP_BLOCK_ERASE			0xD8	/* Erase Block */
#define _SPI_NAND_OP_RESET					0xFF	/* Reset */
#define _SPI_NAND_OP_DIE_SELECT				0xC2	/* Die Select */

/* SPI NAND register address of command set */
#define _SPI_NAND_ADDR_PROTECTION			0xA0	/* Address of protection */
#define _SPI_NAND_ADDR_FEATURE				0xB0	/* Address of feature */
#define _SPI_NAND_ADDR_STATUS				0xC0	/* Address of status */
#define _SPI_NAND_ADDR_MANUFACTURE_ID		0x00	/* Address of Manufacture ID */
#define _SPI_NAND_ADDR_DEVICE_ID			0x01	/* Address of Device ID */

/* SPI NAND value of register address of command set */
#define _SPI_NAND_VAL_DISABLE_PROTECTION	0x0		/* Value for disable write protection */
#define _SPI_NAND_VAL_ENABLE_PROTECTION		0x38	/* Value for enable write protection */
#define _SPI_NAND_VAL_OIP					0x1		/* OIP = Operaton In Progress */
#define _SPI_NAND_VAL_ERASE_FAIL			0x4		/* E_FAIL = Erase Fail */
#define _SPI_NAND_VAL_PROGRAM_FAIL			0x8		/* P_FAIL = Program Fail */



/* SPI NAND Size Define */
#define _SPI_NAND_PAGE_SIZE_2KBYTE			0x0800	
#define _SPI_NAND_OOB_SIZE_64BYTE			0x40
#define _SPI_NAND_OOB_SIZE_120BYTE			0x78
#define _SPI_NAND_OOB_SIZE_128BYTE			0x80
#define _SPI_NAND_BLOCK_SIZE_128KBYTE		0x20000
#define _SPI_NAND_CHIP_SIZE_512MBIT			0x04000000
#define _SPI_NAND_CHIP_SIZE_1GBIT			0x08000000
#define _SPI_NAND_CHIP_SIZE_2GBIT			0x10000000
#define _SPI_NAND_CHIP_SIZE_4GBIT			0x20000000

/* SPI NAND Manufacturers ID */
#define _SPI_NAND_MANUFACTURER_ID_GIGADEVICE	0xC8
#define _SPI_NAND_MANUFACTURER_ID_WINBOND		0xEF
#define _SPI_NAND_MANUFACTURER_ID_ESMT			0xC8
#define _SPI_NAND_MANUFACTURER_ID_MXIC		0xC2
#define _SPI_NAND_MANUFACTURER_ID_ZENTEL		0xC8
#define _SPI_NAND_MANUFACTURER_ID_ETRON			0xD5
#define _SPI_NAND_MANUFACTURER_ID_TOSHIBA		0x98
#define _SPI_NAND_MANUFACTURER_ID_MICRON		0x2C




/* SPI NAND Device ID */
#define _SPI_NAND_DEVICE_ID_GD5F1GQ4UAYIG	0xF1
#define _SPI_NAND_DEVICE_ID_GD5F1GQ4UBYIG	0xD1
#define _SPI_NAND_DEVICE_ID_GD5F1GQ4UCYIG	0xB1
#define _SPI_NAND_DEVICE_ID_GD5F2GQ4UBYIG	0xD2
#define _SPI_NAND_DEVICE_ID_GD5F2GQ4UCYIG	0xB2
#define _SPI_NAND_DEVICE_ID_F50L512M41A		0x20
#define _SPI_NAND_DEVICE_ID_F50L1G41A0		0x21
#define _SPI_NAND_DEVICE_ID_W25N01GV		0xAA
#define _SPI_NAND_DEVICE_ID_W25M02GV		0xAB
#define _SPI_NAND_DEVICE_ID_MXIC35LF1GE4AB	0x12
#define _SPI_NAND_DEVICE_ID_MXIC35LF2GE4AB	0x22
#define _SPI_NAND_DEVICE_ID_MXIC35LF4GE4AB	0x32
#define _SPI_NAND_DEVICE_ID_A5U12A21ASC		0x20
#define _SPI_NAND_DEVICE_ID_A5U1GA21BWS		0x21
#define _SPI_NAND_DEVICE_ID_EM73C044SNB		0x11
#define _SPI_NAND_DEVICE_ID_EM73D044SNA		0x12
#define _SPI_NAND_DEVICE_ID_TC58CVG0S3H		0xC2
#define _SPI_NAND_DEVICE_ID_TC58CVG1S3H		0xCB
#define _SPI_NAND_DEVICE_ID_MT29F1G01		0x14





/* Others Define */
#define _SPI_NAND_LEN_ONE_BYTE				(1)
#define _SPI_NAND_LEN_TWO_BYTE				(2)
#define _SPI_NAND_LEN_THREE_BYTE			(3)
#define _SPI_NAND_BLOCK_ROW_ADDRESS_OFFSET	(6)

#define _SPI_NAND_PAGE_SIZE  				2048
#define _SPI_NAND_OOB_SIZE  				128
#define _SPI_NAND_CACHE_SIZE 				(_SPI_NAND_PAGE_SIZE+_SPI_NAND_OOB_SIZE)

#define _SPI_FREQUENCY_ADJUST_REG			0xBFA200CC

#define LINUX_USE_OOB_START_OFFSET		4
#define MAX_LINUX_USE_OOB_SIZE			26
#define MAX_USE_OOB_SIZE				(LINUX_USE_OOB_START_OFFSET + MAX_LINUX_USE_OOB_SIZE + 2)


#define MIN(a,b)        ((a) < (b) ? (a) : (b))


/* MACRO DECLARATIONS ---------------------------------------------------------------- */
#define _SPI_NAND_BLOCK_ALIGNED_CHECK( __addr__,__block_size__) ((__addr__) & ( __block_size__ - 1))
#define _SPI_NAND_GET_DEVICE_INFO_PTR		&(_current_flash_info_t)


unsigned long spinand_spinlock_flags;
static DEFINE_SPINLOCK(spinandLock);



/* Porting Replacement */
#define _SPI_NAND_SEMAPHORE_LOCK()			spin_lock_irqsave(&spinandLock, spinand_spinlock_flags)	/* Disable interrupt */
#define _SPI_NAND_SEMAPHORE_UNLOCK()		spin_unlock_irqrestore(&spinandLock, spinand_spinlock_flags)	/* Enable interrupt  */
#define _SPI_NAND_PRINTF					printk
#define _SPI_NAND_DEBUG_PRINTF				spi_nand_flash_debug_printf
#define _SPI_NAND_DEBUG_PRINTF_ARRAY		spi_nand_flash_debug_printf_array
#define _SPI_NAND_ENABLE_MANUAL_MODE		SPI_CONTROLLER_Enable_Manual_Mode
#define _SPI_NAND_WRITE_ONE_BYTE			SPI_CONTROLLER_Write_One_Byte
#define _SPI_NAND_WRITE_NBYTE				SPI_CONTROLLER_Write_NByte
#define _SPI_NAND_READ_NBYTE				SPI_CONTROLLER_Read_NByte
#define _SPI_NAND_READ_CHIP_SELECT_HIGH		SPI_CONTROLLER_Chip_Select_High
#define _SPI_NAND_READ_CHIP_SELECT_LOW		SPI_CONTROLLER_Chip_Select_Low


struct ra_nand_chip ra;
struct nand_info flashInfo;
unsigned char _plane_select_bit=0;
unsigned char _die_id = 0;

struct mtd_info 	*spi_nand_mtd;


/* TYPE DECLARATIONS ----------------------------------------------------------------- */
struct _SPI_NAND_FLASH_ID_T {
	u8 mfr_id;
	u8 dev_id;	
};

/* STATIC VARIABLE DECLARATIONS ------------------------------------------------------ */
#if	defined(TCSUPPORT_NAND_BMT) && !defined(LZMA_IMG)

static int bmt_pool_size = 0;
static bmt_struct *g_bmt = NULL;
static init_bbt_struct *g_bbt = NULL;
#endif


SPI_NAND_FLASH_DEBUG_LEVEL_T  _SPI_NAND_DEBUG_LEVEL = SPI_NAND_FLASH_DEBUG_LEVEL_0;
int							  _SPI_NAND_TEST_FLAG = 0;


u8								_SPI_NAND_DEBUG_FLAG= 0;	/* For control printf debug message or not */
u32 							_current_page_num	= 0xFFFFFFFF;
u32 							_spare_area_factor 	= 32;
u8		 						_current_cache_page[_SPI_NAND_CACHE_SIZE];
u8		 						_current_cache_page_data[_SPI_NAND_PAGE_SIZE];
u8		 						_current_cache_page_oob[_SPI_NAND_OOB_SIZE];
u8		 						_current_cache_page_oob_mapping[_SPI_NAND_OOB_SIZE];


struct SPI_NAND_FLASH_INFO_T	_current_flash_info_t;			/* Store the current flash information */


struct spi_nand_flash_ooblayout ooblayout_esmt = {
	.oobsize = 36, 
	.oobfree = {{0,1}, {8,8}, {16,1}, {24,8}, {32,1}, {40,8}, {48,1}, {56,8} }
};


struct spi_nand_flash_ooblayout ooblayout_mxic = {
	.oobsize = 64, 
	.oobfree = {{0,64}}
};

struct spi_nand_flash_ooblayout ooblayout_winbond = {
	.oobsize = 32, 
	.oobfree = {{0,8}, {16,8}, {32,8}, {48,8} }
};

struct spi_nand_flash_ooblayout ooblayout_gigadevice_a = {
	.oobsize = 48, 
	.oobfree = {{0,12}, {16,12}, {32,12}, {48,12} }
};

struct spi_nand_flash_ooblayout ooblayout_gigadevice_b = {
	.oobsize = 64, 
	.oobfree = {{0,64}}
};

struct spi_nand_flash_ooblayout ooblayout_gigadevice_c = {
	.oobsize = 64, 
	.oobfree = {{0,64}}
};

struct spi_nand_flash_ooblayout ooblayout_zentel = {
	.oobsize = 36, 
	.oobfree = {{0,1}, {8,8}, {16,1}, {24,8}, {32,1}, {40,8}, {48,1}, {56,8} }
};

struct spi_nand_flash_ooblayout ooblayout_etron = {
	.oobsize = 60, 
	.oobfree = {{0,16}, {30,16}, {60,16}, {90,16}}
};

struct spi_nand_flash_ooblayout ooblayout_toshiba = {
	.oobsize = 64, 
	.oobfree = {{0,64}}
};

struct spi_nand_flash_ooblayout ooblayout_micron = {
	.oobsize = 64, 
	.oobfree = {{0,64}}
};



/*****************************[ Notice]******************************/
/* If new spi nand chip have page size more than 2KB,  or oob size more than 128 bytes,  than*/
/* it will need to adjust the #define of _SPI_NAND_PAGE_SIZE and _SPI_NAND_OOB_SIZE */
/*****************************[ Notice]******************************/

static const struct SPI_NAND_FLASH_INFO_T spi_nand_flash_tables[] = {
	{
		mfr_id:						_SPI_NAND_MANUFACTURER_ID_GIGADEVICE,
		dev_id:						_SPI_NAND_DEVICE_ID_GD5F1GQ4UAYIG,
		ptr_name:					"_SPI_NAND_DEVICE_ID_GD5F1GQ4UA",
		device_size:				_SPI_NAND_CHIP_SIZE_1GBIT,
		page_size:					_SPI_NAND_PAGE_SIZE_2KBYTE,
		oob_size:					_SPI_NAND_OOB_SIZE_64BYTE,
		erase_size:					_SPI_NAND_BLOCK_SIZE_128KBYTE,
		dummy_mode:					SPI_NAND_FLASH_READ_DUMMY_BYTE_APPEND,
		read_mode:					SPI_NAND_FLASH_READ_SPEED_MODE_SINGLE,
		oob_free_layout : 			&ooblayout_gigadevice_a,
		feature:					SPI_NAND_FLASH_FEATURE_NONE,
	},

	{
		mfr_id:						_SPI_NAND_MANUFACTURER_ID_GIGADEVICE,
		dev_id:						_SPI_NAND_DEVICE_ID_GD5F1GQ4UBYIG,
		ptr_name:					"_SPI_NAND_DEVICE_ID_GD5F1GQ4UB",
		device_size:				_SPI_NAND_CHIP_SIZE_1GBIT,
		page_size:					_SPI_NAND_PAGE_SIZE_2KBYTE,
		oob_size:					_SPI_NAND_OOB_SIZE_128BYTE,
		erase_size:					_SPI_NAND_BLOCK_SIZE_128KBYTE,
		dummy_mode:					SPI_NAND_FLASH_READ_DUMMY_BYTE_APPEND,
		read_mode:					SPI_NAND_FLASH_READ_SPEED_MODE_SINGLE,
		oob_free_layout : 			&ooblayout_gigadevice_b,
		feature:					SPI_NAND_FLASH_FEATURE_NONE,
	},

	{
		mfr_id: 					_SPI_NAND_MANUFACTURER_ID_GIGADEVICE,
		dev_id: 					_SPI_NAND_DEVICE_ID_GD5F1GQ4UCYIG,
		ptr_name:					"_SPI_NAND_DEVICE_ID_GD5F1GQ4UC",
		device_size:				_SPI_NAND_CHIP_SIZE_1GBIT,
		page_size:					_SPI_NAND_PAGE_SIZE_2KBYTE,
		oob_size:					_SPI_NAND_OOB_SIZE_128BYTE,
		erase_size: 				_SPI_NAND_BLOCK_SIZE_128KBYTE,
		dummy_mode: 				SPI_NAND_FLASH_READ_DUMMY_BYTE_PREPEND,
		read_mode:					SPI_NAND_FLASH_READ_SPEED_MODE_SINGLE,
		oob_free_layout : 			&ooblayout_gigadevice_c,
		feature:					SPI_NAND_FLASH_FEATURE_NONE,
	},

	{
		mfr_id:						_SPI_NAND_MANUFACTURER_ID_GIGADEVICE,
		dev_id:						_SPI_NAND_DEVICE_ID_GD5F2GQ4UBYIG,
		ptr_name:					"_SPI_NAND_DEVICE_ID_GD5F2GQ4UB",
		device_size:				_SPI_NAND_CHIP_SIZE_2GBIT,
		page_size:					_SPI_NAND_PAGE_SIZE_2KBYTE,
		oob_size:					_SPI_NAND_OOB_SIZE_128BYTE,
		erase_size:					_SPI_NAND_BLOCK_SIZE_128KBYTE,
		dummy_mode:					SPI_NAND_FLASH_READ_DUMMY_BYTE_APPEND,
		read_mode:					SPI_NAND_FLASH_READ_SPEED_MODE_SINGLE,
		oob_free_layout : 			&ooblayout_gigadevice_b,
		feature:					SPI_NAND_FLASH_FEATURE_NONE,
	},		
	
	{
		mfr_id: 					_SPI_NAND_MANUFACTURER_ID_GIGADEVICE,
		dev_id: 					_SPI_NAND_DEVICE_ID_GD5F2GQ4UCYIG,
		ptr_name:					"_SPI_NAND_DEVICE_ID_GD5F2GQ4UC",
		device_size:				_SPI_NAND_CHIP_SIZE_2GBIT,
		page_size:					_SPI_NAND_PAGE_SIZE_2KBYTE,
		oob_size:					_SPI_NAND_OOB_SIZE_128BYTE,
		erase_size: 				_SPI_NAND_BLOCK_SIZE_128KBYTE,
		dummy_mode: 				SPI_NAND_FLASH_READ_DUMMY_BYTE_PREPEND,
		read_mode:					SPI_NAND_FLASH_READ_SPEED_MODE_SINGLE,
		oob_free_layout : 			&ooblayout_gigadevice_c,
		feature:					SPI_NAND_FLASH_FEATURE_NONE,
	},		
				
	{
		mfr_id:						_SPI_NAND_MANUFACTURER_ID_ESMT,
		dev_id:						_SPI_NAND_DEVICE_ID_F50L512M41A,
		ptr_name:					"_SPI_NAND_DEVICE_ID_F50L512",
		device_size:				_SPI_NAND_CHIP_SIZE_512MBIT,
		page_size:					_SPI_NAND_PAGE_SIZE_2KBYTE,
		oob_size:					_SPI_NAND_OOB_SIZE_64BYTE,
		erase_size:					_SPI_NAND_BLOCK_SIZE_128KBYTE,
		dummy_mode:					SPI_NAND_FLASH_READ_DUMMY_BYTE_APPEND,
		read_mode:					SPI_NAND_FLASH_READ_SPEED_MODE_SINGLE,
		oob_free_layout : 			&ooblayout_esmt,
		feature:					SPI_NAND_FLASH_FEATURE_NONE,
	},
	
	{
		mfr_id: 					_SPI_NAND_MANUFACTURER_ID_ESMT,
		dev_id: 					_SPI_NAND_DEVICE_ID_F50L1G41A0,
		ptr_name:					"_SPI_NAND_DEVICE_ID_F50L1G",		
		device_size:				_SPI_NAND_CHIP_SIZE_1GBIT,
		page_size:					_SPI_NAND_PAGE_SIZE_2KBYTE,
		oob_size:					_SPI_NAND_OOB_SIZE_64BYTE,
		erase_size:					_SPI_NAND_BLOCK_SIZE_128KBYTE,
		dummy_mode:					SPI_NAND_FLASH_READ_DUMMY_BYTE_APPEND,
		read_mode:					SPI_NAND_FLASH_READ_SPEED_MODE_SINGLE,
		oob_free_layout : 			&ooblayout_esmt,
		feature:					SPI_NAND_FLASH_FEATURE_NONE,
	},
	
	{
		mfr_id:						_SPI_NAND_MANUFACTURER_ID_WINBOND,
		dev_id:						_SPI_NAND_DEVICE_ID_W25N01GV,
		ptr_name:					"_SPI_NAND_DEVICE_ID_W25N01G",	
		device_size:				_SPI_NAND_CHIP_SIZE_1GBIT,
		page_size:					_SPI_NAND_PAGE_SIZE_2KBYTE,
		oob_size:					_SPI_NAND_OOB_SIZE_64BYTE,
		erase_size:					_SPI_NAND_BLOCK_SIZE_128KBYTE,
		dummy_mode:					SPI_NAND_FLASH_READ_DUMMY_BYTE_APPEND,
		read_mode:					SPI_NAND_FLASH_READ_SPEED_MODE_SINGLE,
		oob_free_layout : 			&ooblayout_winbond,
		feature:					SPI_NAND_FLASH_FEATURE_NONE,
	},

	{
		mfr_id: 					_SPI_NAND_MANUFACTURER_ID_WINBOND,
		dev_id: 					_SPI_NAND_DEVICE_ID_W25M02GV,
		ptr_name:					"_SPI_NAND_DEVICE_ID_W25M02G",
		device_size:				_SPI_NAND_CHIP_SIZE_2GBIT,
		page_size:					_SPI_NAND_PAGE_SIZE_2KBYTE,
		oob_size:					_SPI_NAND_OOB_SIZE_64BYTE,
		erase_size: 				_SPI_NAND_BLOCK_SIZE_128KBYTE,
		dummy_mode: 				SPI_NAND_FLASH_READ_DUMMY_BYTE_APPEND,
		read_mode:					SPI_NAND_FLASH_READ_SPEED_MODE_SINGLE,
		oob_free_layout : 			&ooblayout_winbond,
		feature:					SPI_NAND_FLASH_DIE_SELECT_HAVE,
	},		
	
	{
		mfr_id:						_SPI_NAND_MANUFACTURER_ID_MXIC,
		dev_id:						_SPI_NAND_DEVICE_ID_MXIC35LF1GE4AB,
		ptr_name:					"_SPI_NAND_DEVICE_ID_MXIC35LF1G",		
		device_size:				_SPI_NAND_CHIP_SIZE_1GBIT,
		page_size:					_SPI_NAND_PAGE_SIZE_2KBYTE,
		oob_size:					_SPI_NAND_OOB_SIZE_64BYTE,
		erase_size:					_SPI_NAND_BLOCK_SIZE_128KBYTE,
		dummy_mode:					SPI_NAND_FLASH_READ_DUMMY_BYTE_APPEND,
		read_mode:					SPI_NAND_FLASH_READ_SPEED_MODE_SINGLE,
		oob_free_layout : 			&ooblayout_mxic,
		feature:					SPI_NAND_FLASH_FEATURE_NONE,
	},

	{
		mfr_id:						_SPI_NAND_MANUFACTURER_ID_MXIC,
		dev_id:						_SPI_NAND_DEVICE_ID_MXIC35LF2GE4AB,
		ptr_name:					"_SPI_NAND_DEVICE_ID_MXIC35LF2G",
		device_size:				_SPI_NAND_CHIP_SIZE_2GBIT,
		page_size:					_SPI_NAND_PAGE_SIZE_2KBYTE,
		oob_size:					_SPI_NAND_OOB_SIZE_64BYTE,
		erase_size:					_SPI_NAND_BLOCK_SIZE_128KBYTE,
		dummy_mode:					SPI_NAND_FLASH_READ_DUMMY_BYTE_APPEND,
		read_mode:					SPI_NAND_FLASH_READ_SPEED_MODE_SINGLE,
		oob_free_layout : 			&ooblayout_mxic,
		feature:					SPI_NAND_FLASH_PLANE_SELECT_HAVE,
	},

	{
		mfr_id: 					_SPI_NAND_MANUFACTURER_ID_ZENTEL,
		dev_id: 					_SPI_NAND_DEVICE_ID_A5U12A21ASC,
		ptr_name:					"_SPI_NAND_DEVICE_ID_A5U12A21ASC",
		device_size:				_SPI_NAND_CHIP_SIZE_512MBIT,
		page_size:					_SPI_NAND_PAGE_SIZE_2KBYTE,
		oob_size:					_SPI_NAND_OOB_SIZE_64BYTE,
		erase_size: 				_SPI_NAND_BLOCK_SIZE_128KBYTE,
		dummy_mode: 				SPI_NAND_FLASH_READ_DUMMY_BYTE_APPEND,
		read_mode:					SPI_NAND_FLASH_READ_SPEED_MODE_SINGLE,
		oob_free_layout : 			&ooblayout_zentel,		
		feature:					SPI_NAND_FLASH_FEATURE_NONE,
	},
	{
		mfr_id: 					_SPI_NAND_MANUFACTURER_ID_ZENTEL,
		dev_id: 					_SPI_NAND_DEVICE_ID_A5U1GA21BWS,
		ptr_name:					"_SPI_NAND_DEVICE_ID_A5U1GA21BWS",
		device_size:				_SPI_NAND_CHIP_SIZE_1GBIT,
		page_size:					_SPI_NAND_PAGE_SIZE_2KBYTE,
		oob_size:					_SPI_NAND_OOB_SIZE_64BYTE,
		erase_size: 				_SPI_NAND_BLOCK_SIZE_128KBYTE,
		dummy_mode: 				SPI_NAND_FLASH_READ_DUMMY_BYTE_APPEND,
		read_mode:					SPI_NAND_FLASH_READ_SPEED_MODE_SINGLE,
		oob_free_layout : 			&ooblayout_zentel,		
		feature:					SPI_NAND_FLASH_FEATURE_NONE,
	},	

	{
		mfr_id: 					_SPI_NAND_MANUFACTURER_ID_ETRON,
		dev_id: 					_SPI_NAND_DEVICE_ID_EM73C044SNB,
		ptr_name:					"_SPI_NAND_DEVICE_ID_EM73C044SNB",
		device_size:				_SPI_NAND_CHIP_SIZE_1GBIT,
		page_size:					_SPI_NAND_PAGE_SIZE_2KBYTE,
		oob_size:					_SPI_NAND_OOB_SIZE_128BYTE,
		erase_size:					_SPI_NAND_BLOCK_SIZE_128KBYTE,
		dummy_mode:					SPI_NAND_FLASH_READ_DUMMY_BYTE_APPEND,
		read_mode:					SPI_NAND_FLASH_READ_SPEED_MODE_SINGLE,
		oob_free_layout : 			&ooblayout_etron,			
		feature:					SPI_NAND_FLASH_FEATURE_NONE,
	},
	
	{
		mfr_id: 					_SPI_NAND_MANUFACTURER_ID_ETRON,
		dev_id: 					_SPI_NAND_DEVICE_ID_EM73D044SNA,
		ptr_name:					"_SPI_NAND_DEVICE_ID_EM73D044SNA",
		device_size:				_SPI_NAND_CHIP_SIZE_2GBIT,
		page_size:					_SPI_NAND_PAGE_SIZE_2KBYTE,
		oob_size:					_SPI_NAND_OOB_SIZE_128BYTE,
		erase_size: 				_SPI_NAND_BLOCK_SIZE_128KBYTE,
		dummy_mode: 				SPI_NAND_FLASH_READ_DUMMY_BYTE_APPEND,
		read_mode:					SPI_NAND_FLASH_READ_SPEED_MODE_SINGLE,		
		oob_free_layout :			&ooblayout_etron,		
		feature:					SPI_NAND_FLASH_FEATURE_NONE,
	},
	
	{
		mfr_id: 					_SPI_NAND_MANUFACTURER_ID_TOSHIBA,
		dev_id: 					_SPI_NAND_DEVICE_ID_TC58CVG0S3H,
		ptr_name:					"_SPI_NAND_DEVICE_ID_TC58CVG0S3H",
		device_size:				_SPI_NAND_CHIP_SIZE_1GBIT,
		page_size:					_SPI_NAND_PAGE_SIZE_2KBYTE,
		oob_size:					_SPI_NAND_OOB_SIZE_64BYTE,
		erase_size: 				_SPI_NAND_BLOCK_SIZE_128KBYTE,
		dummy_mode: 				SPI_NAND_FLASH_READ_DUMMY_BYTE_APPEND,
		read_mode:					SPI_NAND_FLASH_READ_SPEED_MODE_SINGLE,		
		oob_free_layout : 			&ooblayout_toshiba,		
		feature:					SPI_NAND_FLASH_FEATURE_NONE,
	},		

	{
		mfr_id: 					_SPI_NAND_MANUFACTURER_ID_TOSHIBA,
		dev_id: 					_SPI_NAND_DEVICE_ID_TC58CVG1S3H,
		ptr_name:					"_SPI_NAND_DEVICE_ID_TC58CVG1S3H",
		device_size:				_SPI_NAND_CHIP_SIZE_2GBIT,
		page_size:					_SPI_NAND_PAGE_SIZE_2KBYTE,
		oob_size:					_SPI_NAND_OOB_SIZE_64BYTE,
		erase_size: 				_SPI_NAND_BLOCK_SIZE_128KBYTE,
		dummy_mode: 				SPI_NAND_FLASH_READ_DUMMY_BYTE_APPEND,
		read_mode:					SPI_NAND_FLASH_READ_SPEED_MODE_SINGLE,		
		oob_free_layout : 			&ooblayout_toshiba,		
		feature:					SPI_NAND_FLASH_FEATURE_NONE,
	}, 

	{
		mfr_id: 					_SPI_NAND_MANUFACTURER_ID_MICRON,
		dev_id: 					_SPI_NAND_DEVICE_ID_MT29F1G01,
		ptr_name:					"_SPI_NAND_DEVICE_ID_MT29F1G01",
		device_size:				_SPI_NAND_CHIP_SIZE_1GBIT,
		page_size:					_SPI_NAND_PAGE_SIZE_2KBYTE,
		oob_size:					_SPI_NAND_OOB_SIZE_128BYTE,
		erase_size: 				_SPI_NAND_BLOCK_SIZE_128KBYTE,
		dummy_mode: 				SPI_NAND_FLASH_READ_DUMMY_BYTE_APPEND,
		read_mode:					SPI_NAND_FLASH_READ_SPEED_MODE_SINGLE,		
		oob_free_layout :			&ooblayout_micron, 	
		feature:					SPI_NAND_FLASH_FEATURE_NONE,
	}	
	
};

/* LOCAL SUBPROGRAM BODIES------------------------------------------------------------ */
static int generic_ffs(int x)
{
	int r = 1;

	if (!x)
		return 0;
	if (!(x & 0xffff)) {
		x >>= 16;
		r += 16;
	}
	if (!(x & 0xff)) {
		x >>= 8;
		r += 8;
	}
	if (!(x & 0xf)) {
		x >>= 4;
		r += 4;
	}
	if (!(x & 3)) {
		x >>= 2;
		r += 2;
	}
	if (!(x & 1)) {
		x >>= 1;
		r += 1;
	}
	return r;
}

static void spi_nand_flash_debug_printf( SPI_NAND_FLASH_DEBUG_LEVEL_T DEBUG_LEVEL, char *fmt, ... )
{
	if( _SPI_NAND_DEBUG_LEVEL >= DEBUG_LEVEL )
	{
		
		unsigned char 		str_buf[100];	
		va_list 			argptr;
		int 				cnt;		
	
		va_start(argptr, fmt);
		cnt = vsprintf(str_buf, fmt, argptr);
		va_end(argptr);
				
		_SPI_NAND_PRINTF("%s", str_buf);	
	}
}

static void spi_nand_flash_debug_printf_array( SPI_NAND_FLASH_DEBUG_LEVEL_T DEBUG_LEVEL, char *buf, u32 len )
{

	u32	idx_for_debug;

	if( _SPI_NAND_DEBUG_LEVEL >= DEBUG_LEVEL )
	{

		for(idx_for_debug=0; idx_for_debug< len; idx_for_debug++)
		{				
			if( ((idx_for_debug) %8 == 0) )
			{
				_SPI_NAND_PRINTF("\n%04x: ", (idx_for_debug));
			}
			_SPI_NAND_PRINTF("%02x ", buf[idx_for_debug]);
		}			
		_SPI_NAND_PRINTF("\n");		
	}	

}

static SPI_NAND_FLASH_RTN_T spi_nand_protocol_die_select( u8 die_id)
{
	SPI_NAND_FLASH_RTN_T	rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;

	/* 1. Chip Select low */
	_SPI_NAND_READ_CHIP_SELECT_LOW();

	/* 2. Send C2h opcode (Die Select) */
	_SPI_NAND_WRITE_ONE_BYTE( _SPI_NAND_OP_DIE_SELECT );

	/* 3. Send Die ID */
	_SPI_NAND_WRITE_ONE_BYTE( die_id );

	/* 5. Chip Select High */
	_SPI_NAND_READ_CHIP_SELECT_HIGH();

	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "spi_nand_protocol_die_select\n");

	return (rtn_status);
}

static SPI_NAND_FLASH_RTN_T spi_nand_protocol_reset( void )
{
	SPI_NAND_FLASH_RTN_T	rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;
	
	/* 1. Chip Select low */
	_SPI_NAND_READ_CHIP_SELECT_LOW();

	/* 2. Send FFh opcode (Reset) */
	_SPI_NAND_WRITE_ONE_BYTE( _SPI_NAND_OP_SET_FEATURE );

	/* 5. Chip Select High */
	_SPI_NAND_READ_CHIP_SELECT_HIGH();	

	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "spi_nand_protocol_reset\n");
	
	return (rtn_status);	
}


/*------------------------------------------------------------------------------------
 * FUNCTION: static SPI_NAND_FLASH_RTN_T spi_nand_protocol_set_status_reg_1( u8    protection )
 * PURPOSE : To implement the SPI nand protocol for set status register 1.
 * AUTHOR  : Chuck Kuo
 * CALLED BY
 *   -
 * CALLS
 *   -
 * PARAMs  :
 *   INPUT : protection - The protection variable of this function.
 *   OUTPUT: None
 * RETURN  : SPI_RTN_NO_ERROR - Successful.   Otherwise - Failed.
 * NOTES   :
 * MODIFICTION HISTORY:
 * Date 2014/12/17 by Chuck Kuo - The first revision for this function.
 *
 *------------------------------------------------------------------------------------
 */
static SPI_NAND_FLASH_RTN_T spi_nand_protocol_set_status_reg_1( u8 protection )
{
	SPI_NAND_FLASH_RTN_T	rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;
	
	/* 1. Chip Select low */
	_SPI_NAND_READ_CHIP_SELECT_LOW();

	/* 2. Send 0Fh opcode (Set Feature) */
	_SPI_NAND_WRITE_ONE_BYTE( _SPI_NAND_OP_SET_FEATURE );
	
	/* 3. Offset addr of protection (0xA0) */
	_SPI_NAND_WRITE_ONE_BYTE( _SPI_NAND_ADDR_PROTECTION );
	
	/* 4. Write new proteciton setting */
	_SPI_NAND_WRITE_ONE_BYTE( protection );

	/* 5. Chip Select High */
	_SPI_NAND_READ_CHIP_SELECT_HIGH();	

	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "spi_nand_protocol_set_status_reg_1: protection=0x%x\n", protection);
	
	return (rtn_status);
}

/*------------------------------------------------------------------------------------
 * FUNCTION: static SPI_NAND_FLASH_RTN_T spi_nand_protocol_get_status_reg_1( u8    *ptr_rtn_protection )
 * PURPOSE : To implement the SPI nand protocol for get status register 1.
 * AUTHOR  : Chuck Kuo
 * CALLED BY
 *   -
 * CALLS
 *   -
 * PARAMs  :
 *   INPUT : None
 *   OUTPUT: ptr_rtn_protection  - A pointer to the ptr_rtn_protection variable.
 * RETURN  : SPI_RTN_NO_ERROR - Successful.   Otherwise - Failed.
 * NOTES   :
 * MODIFICTION HISTORY:
 * Date 2014/12/17 by Chuck Kuo - The first revision for this function.
 *
 *------------------------------------------------------------------------------------
 */
static SPI_NAND_FLASH_RTN_T spi_nand_protocol_get_status_reg_1( u8 *ptr_rtn_protection )
{
	SPI_NAND_FLASH_RTN_T	rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;
	
	/* 1. Chip Select low */
	_SPI_NAND_READ_CHIP_SELECT_LOW();

	/* 2. Send 0Fh opcode (Get Feature) */
	_SPI_NAND_WRITE_ONE_BYTE( _SPI_NAND_OP_GET_FEATURE );
	
	/* 3. Offset addr of protection (0xA0) */
	_SPI_NAND_WRITE_ONE_BYTE( _SPI_NAND_ADDR_PROTECTION );
	
	/* 4. Read 1 byte data */
	_SPI_NAND_READ_NBYTE( ptr_rtn_protection, _SPI_NAND_LEN_ONE_BYTE, SPI_CONTROLLER_SPEED_SINGLE);

	/* 5. Chip Select High */
	_SPI_NAND_READ_CHIP_SELECT_HIGH();	

	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "spi_nand_protocol_get_status_reg_1: protection=0x%x\n", *ptr_rtn_protection);
	
	return (rtn_status);
}

/*------------------------------------------------------------------------------------
 * FUNCTION: static SPI_NAND_FLASH_RTN_T spi_nand_protocol_set_status_reg_2( u8   feature )
 * PURPOSE : To implement the SPI nand protocol for set status register 2.
 * AUTHOR  : Chuck Kuo
 * CALLED BY
 *   -
 * CALLS
 *   -
 * PARAMs  :
 *   INPUT : feature - The feature variable of this function.
 *   OUTPUT: None
 * RETURN  : SPI_RTN_NO_ERROR - Successful.   Otherwise - Failed.
 * NOTES   :
 * MODIFICTION HISTORY:
 * Date 2015/01/21 by Chuck Kuo - The first revision for this function.
 *
 *------------------------------------------------------------------------------------
 */
static SPI_NAND_FLASH_RTN_T spi_nand_protocol_set_status_reg_2( u8 feature )
{
	SPI_NAND_FLASH_RTN_T	rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;
	
	/* 1. Chip Select low */
	_SPI_NAND_READ_CHIP_SELECT_LOW();

	/* 2. Send 0Fh opcode (Set Feature) */
	_SPI_NAND_WRITE_ONE_BYTE( _SPI_NAND_OP_SET_FEATURE );
	
	/* 3. Offset addr of feature (0xB0) */
	_SPI_NAND_WRITE_ONE_BYTE( _SPI_NAND_ADDR_FEATURE );
	
	/* 4. Write new feature setting */
	_SPI_NAND_WRITE_ONE_BYTE( feature );

	/* 5. Chip Select High */
	_SPI_NAND_READ_CHIP_SELECT_HIGH();	

	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "spi_nand_protocol_set_status_reg_2: feature=0x%x\n", feature);
	
	return (rtn_status);
}

/*------------------------------------------------------------------------------------
 * FUNCTION: static SPI_NAND_FLASH_RTN_T spi_nand_protocol_get_status_reg_2( u8   *ptr_rtn_feature )
 * PURPOSE : To implement the SPI nand protocol for get status register 2.
 * AUTHOR  : Chuck Kuo
 * CALLED BY
 *   -
 * CALLS
 *   -
 * PARAMs  :
 *   INPUT : None
 *   OUTPUT: ptr_rtn_feature  - A pointer to the ptr_rtn_feature variable.
 * RETURN  : SPI_RTN_NO_ERROR - Successful.   Otherwise - Failed.
 * NOTES   :
 * MODIFICTION HISTORY:
 * Date 2015/01/21 by Chuck Kuo - The first revision for this function.
 *
 *------------------------------------------------------------------------------------
 */
static SPI_NAND_FLASH_RTN_T spi_nand_protocol_get_status_reg_2( u8 *ptr_rtn_feature )
{
	SPI_NAND_FLASH_RTN_T	rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;
	
	/* 1. Chip Select low */
	_SPI_NAND_READ_CHIP_SELECT_LOW();

	/* 2. Send 0Fh opcode (Get Feature) */
	_SPI_NAND_WRITE_ONE_BYTE( _SPI_NAND_OP_GET_FEATURE );
	
	/* 3. Offset addr of protection (0xB0) */
	_SPI_NAND_WRITE_ONE_BYTE( _SPI_NAND_ADDR_FEATURE );
	
	/* 4. Read 1 byte data */
	_SPI_NAND_READ_NBYTE( ptr_rtn_feature, _SPI_NAND_LEN_ONE_BYTE, SPI_CONTROLLER_SPEED_SINGLE);

	/* 5. Chip Select High */
	_SPI_NAND_READ_CHIP_SELECT_HIGH();	

	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "spi_nand_protocol_get_status_reg_2: feature=0x%x\n", *ptr_rtn_feature);
	
	return (rtn_status);
}

/*------------------------------------------------------------------------------------
 * FUNCTION: SPI_NAND_FLASH_RTN_T spi_nand_protocol_get_status_reg_3( u8 *ptr_rtn_status )
 * PURPOSE : To implement the SPI nand protocol for get status register 3.
 * AUTHOR  : Chuck Kuo
 * CALLED BY
 *   -
 * CALLS
 *   -
 * PARAMs  :
 *   INPUT : stats_reg - The stats_reg variable of this function.
 *   OUTPUT: None
 * RETURN  : SPI_RTN_NO_ERROR - Successful.   Otherwise - Failed.
 * NOTES   :
 * MODIFICTION HISTORY:
 * Date 2014/12/16 by Chuck Kuo - The first revision for this function.
 *
 *------------------------------------------------------------------------------------
 */
static SPI_NAND_FLASH_RTN_T spi_nand_protocol_get_status_reg_3( u8 *ptr_rtn_status )
{
	SPI_NAND_FLASH_RTN_T	rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;
	
	/* 1. Chip Select low */
	_SPI_NAND_READ_CHIP_SELECT_LOW();

	/* 2. Send 0Fh opcode (Get Feature) */
	_SPI_NAND_WRITE_ONE_BYTE( _SPI_NAND_OP_GET_FEATURE );
	
	/* 3. Offset addr of status (0xC0) */
	_SPI_NAND_WRITE_ONE_BYTE( _SPI_NAND_ADDR_STATUS );
	
	/* 4. Read 1 byte data */
	_SPI_NAND_READ_NBYTE( ptr_rtn_status, _SPI_NAND_LEN_ONE_BYTE, SPI_CONTROLLER_SPEED_SINGLE);

	/* 5. Chip Select High */
	_SPI_NAND_READ_CHIP_SELECT_HIGH();	

	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "spi_nand_protocol_get_status_reg_3: status=0x%x\n", *ptr_rtn_status);
	
	return (rtn_status);
}

/*------------------------------------------------------------------------------------
 * FUNCTION: static SPI_NAND_FLASH_RTN_T spi_nand_protocol_write_enable( void )
 * PURPOSE : To implement the SPI nand protocol for write enable.
 * AUTHOR  : Chuck Kuo
 * CALLED BY
 *   -
 * CALLS
 *   -
 * PARAMs  :
 *   INPUT : None
 *   OUTPUT: None
 * RETURN  : SPI_RTN_NO_ERROR - Successful.   Otherwise - Failed.
 * NOTES   :
 * MODIFICTION HISTORY:
 * Date 2014/12/17 by Chuck Kuo - The first revision for this function.
 *
 *------------------------------------------------------------------------------------
 */
static SPI_NAND_FLASH_RTN_T spi_nand_protocol_write_enable( void )
{
	SPI_NAND_FLASH_RTN_T	rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;
	
	/* 1. Chip Select Low */
	_SPI_NAND_READ_CHIP_SELECT_LOW();
	
	/* 2. Write op_cmd 0x06 (Write Enable (WREN)*/
	_SPI_NAND_WRITE_ONE_BYTE( _SPI_NAND_OP_WRITE_ENABLE );
	
	/* 3. Chip Select High */
	_SPI_NAND_READ_CHIP_SELECT_HIGH();
	
	return (rtn_status);
}

/*------------------------------------------------------------------------------------
 * FUNCTION: static SPI_NAND_FLASH_RTN_T spi_nand_protocol_write_disable( void )
 * PURPOSE : To implement the SPI nand protocol for write disable.
 * AUTHOR  : Chuck Kuo
 * CALLED BY
 *   -
 * CALLS
 *   -
 * PARAMs  :
 *   INPUT : None
 *   OUTPUT: None
 * RETURN  : SPI_RTN_NO_ERROR - Successful.   Otherwise - Failed.
 * NOTES   :
 * MODIFICTION HISTORY:
 * Date 2014/12/17 by Chuck Kuo - The first revision for this function.
 *
 *------------------------------------------------------------------------------------
 */
static SPI_NAND_FLASH_RTN_T spi_nand_protocol_write_disable( void )
{
	SPI_NAND_FLASH_RTN_T	rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;	
	
	/* 1. Chip Select Low */
	_SPI_NAND_READ_CHIP_SELECT_LOW();
	
	/* 2. Write op_cmd 0x04 (Write Disable (WRDI)*/
	_SPI_NAND_WRITE_ONE_BYTE( _SPI_NAND_OP_WRITE_DISABLE );
	
	/* 3. Chip Select High */
	_SPI_NAND_READ_CHIP_SELECT_HIGH();
	
	return (rtn_status);
}

/*------------------------------------------------------------------------------------
 * FUNCTION: static SPI_NAND_FLASH_RTN_T spi_nand_protocol_block_erase( u32   block_idx )
 * PURPOSE : To implement the SPI nand protocol for block erase.
 * AUTHOR  : Chuck Kuo
 * CALLED BY
 *   -
 * CALLS
 *   -
 * PARAMs  :
 *   INPUT : block_idx - The block_idx variable of this function.
 *   OUTPUT: None
 * RETURN  : SPI_RTN_NO_ERROR - Successful.   Otherwise - Failed.
 * NOTES   :
 * MODIFICTION HISTORY:
 * Date 2014/12/17 by Chuck Kuo - The first revision for this function.
 *
 *------------------------------------------------------------------------------------
 */
static SPI_NAND_FLASH_RTN_T spi_nand_protocol_block_erase( u32 block_idx )
{
	SPI_NAND_FLASH_RTN_T	rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;
	
	/* 1. Chip Select Low */
	_SPI_NAND_READ_CHIP_SELECT_LOW();
	
	/* 2. Write op_cmd 0xD8 (Block Erase) */
	_SPI_NAND_WRITE_ONE_BYTE( _SPI_NAND_OP_BLOCK_ERASE );
	
	/* 3. Write block number */
	block_idx = block_idx << _SPI_NAND_BLOCK_ROW_ADDRESS_OFFSET; 	/*Row Address format in SPI NAND chip */
	
	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "spi_nand_protocol_block_erase : block idx =0x%x\n", block_idx);
	
	_SPI_NAND_WRITE_ONE_BYTE( (block_idx >> 16) & 0xff );		/* dummy byte */
	_SPI_NAND_WRITE_ONE_BYTE( (block_idx >> 8)  & 0xff );
	_SPI_NAND_WRITE_ONE_BYTE(  block_idx & 0xff );
	
	/* 4. Chip Select High */
	_SPI_NAND_READ_CHIP_SELECT_HIGH();
			
	return (rtn_status);
}

/*------------------------------------------------------------------------------------
 * FUNCTION: static SPI_NAND_FLASH_RTN_T spi_nand_protocol_read_id( struct _SPI_NAND_FLASH_ID_T *ptr_rtn_flash_id )
 * PURPOSE : To implement the SPI nand protocol for read id.
 * AUTHOR  : Chuck Kuo
 * CALLED BY
 *   -
 * CALLS
 *   -
 * PARAMs  :
 *   INPUT : None
 *   OUTPUT: None
 * RETURN  : SPI_RTN_NO_ERROR - Successful.   Otherwise - Failed.
 * NOTES   :
 * MODIFICTION HISTORY:
 * Date 2014/12/12 by Chuck Kuo - The first revision for this function.
 *
 *------------------------------------------------------------------------------------
 */
static SPI_NAND_FLASH_RTN_T spi_nand_protocol_read_id ( struct _SPI_NAND_FLASH_ID_T * ptr_rtn_flash_id )
{
	SPI_NAND_FLASH_RTN_T	rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;	
	
	/* 1. Chip Select Low */
	_SPI_NAND_READ_CHIP_SELECT_LOW();
	
	/* 2. Write op_cmd 0x9F (Read ID) */
	_SPI_NAND_WRITE_ONE_BYTE( _SPI_NAND_OP_READ_ID );	
	
	/* 3. Write Address Byte (0x00) */
	_SPI_NAND_WRITE_ONE_BYTE( _SPI_NAND_ADDR_MANUFACTURE_ID );	
	
	/* 4. Read data (Manufacture ID and Device ID) */	
	_SPI_NAND_READ_NBYTE( &(ptr_rtn_flash_id->mfr_id), _SPI_NAND_LEN_ONE_BYTE, SPI_CONTROLLER_SPEED_SINGLE);	
	_SPI_NAND_READ_NBYTE( &(ptr_rtn_flash_id->dev_id), _SPI_NAND_LEN_ONE_BYTE, SPI_CONTROLLER_SPEED_SINGLE);
	
	/* 5. Chip Select High */
	_SPI_NAND_READ_CHIP_SELECT_HIGH();

	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "spi_nand_protocol_read_id : mfr_id=0x%x, dev_id=0x%x\n", ptr_rtn_flash_id->mfr_id, ptr_rtn_flash_id->dev_id);
	
	return (rtn_status);
}

/*------------------------------------------------------------------------------------
 * FUNCTION: static SPI_NAND_FLASH_RTN_T spi_nand_protocol_read_id_2( struct _SPI_NAND_FLASH_ID_T *ptr_rtn_flash_id )
 * PURPOSE : To implement the SPI nand protocol for read id.
 * AUTHOR  : Chuck Kuo
 * CALLED BY
 *   -
 * CALLS
 *   -
 * PARAMs  :
 *   INPUT : None
 *   OUTPUT: None
 * RETURN  : SPI_RTN_NO_ERROR - Successful.   Otherwise - Failed.
 * NOTES   :
 * MODIFICTION HISTORY:
 * Date 2014/12/12 by Chuck Kuo - The first revision for this function.
 *
 *------------------------------------------------------------------------------------
 */
static SPI_NAND_FLASH_RTN_T spi_nand_protocol_read_id_2 ( struct _SPI_NAND_FLASH_ID_T *ptr_rtn_flash_id )
{
	SPI_NAND_FLASH_RTN_T	rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;	
	
	/* 1. Chip Select Low */
	_SPI_NAND_READ_CHIP_SELECT_LOW();
	
	/* 2. Write op_cmd 0x9F (Read ID) */
	_SPI_NAND_WRITE_ONE_BYTE( _SPI_NAND_OP_READ_ID );	
	
	/* 3. Read data (Manufacture ID and Device ID) */	
	_SPI_NAND_READ_NBYTE( &(ptr_rtn_flash_id->mfr_id), _SPI_NAND_LEN_ONE_BYTE, SPI_CONTROLLER_SPEED_SINGLE);	
	_SPI_NAND_READ_NBYTE( &(ptr_rtn_flash_id->dev_id), _SPI_NAND_LEN_ONE_BYTE, SPI_CONTROLLER_SPEED_SINGLE);
	
	/* 4. Chip Select High */
	_SPI_NAND_READ_CHIP_SELECT_HIGH();

	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "spi_nand_protocol_read_id_2 : mfr_id=0x%x, dev_id=0x%x\n", ptr_rtn_flash_id->mfr_id, ptr_rtn_flash_id->dev_id);
	
	return (rtn_status);
}


/*------------------------------------------------------------------------------------
 * FUNCTION: static SPI_NAND_FLASH_RTN_T spi_nand_protocol_page_read( u32    page_number )
 * PURPOSE : To implement the SPI nand protocol for page read.
 * AUTHOR  : Chuck Kuo
 * CALLED BY
 *   -
 * CALLS
 *   -
 * PARAMs  :
 *   INPUT : page_number - The page_number variable of this function.
 *   OUTPUT: None
 * RETURN  : SPI_RTN_NO_ERROR - Successful.   Otherwise - Failed.
 * NOTES   :
 * MODIFICTION HISTORY:
 * Date 2014/12/17 by Chuck Kuo - The first revision for this function.
 *
 *------------------------------------------------------------------------------------
 */
static SPI_NAND_FLASH_RTN_T spi_nand_protocol_page_read ( u32 page_number )
{
	SPI_NAND_FLASH_RTN_T	rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;
	
	
	/* 1. Chip Select low */
	_SPI_NAND_READ_CHIP_SELECT_LOW();
	
	/* 2. Send 13h opcode */
	_SPI_NAND_WRITE_ONE_BYTE( _SPI_NAND_OP_PAGE_READ );
	
	/* 3. Send page number */
	_SPI_NAND_WRITE_ONE_BYTE( ((page_number >> 16 ) & 0xff) );
	_SPI_NAND_WRITE_ONE_BYTE( ((page_number >> 8  ) & 0xff) );
	_SPI_NAND_WRITE_ONE_BYTE( ((page_number)        & 0xff) );
	
	/* 4. Chip Select High */
	_SPI_NAND_READ_CHIP_SELECT_HIGH();	

	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "spi_nand_load_page_into_cache: value=0x%x\n", ((page_number ) & 0xff) );	
	
	return (rtn_status);
	
}

/*------------------------------------------------------------------------------------
 * FUNCTION: static SPI_NAND_FLASH_RTN_T spi_nand_protocol_read_from_cache_single_speed( u32  data_offset,
 *                                                                          u32  len,
 *                                                                          u8   *ptr_rtn_buf )
 * PURPOSE : To implement the SPI nand protocol for read from cache with single speed.
 * AUTHOR  : Chuck Kuo
 * CALLED BY
 *   -
 * CALLS
 *   -
 * PARAMs  :
 *   INPUT : data_offset  - The data_offset variable of this function.
 *           len          - The len variable of this function.
 *   OUTPUT: ptr_rtn_buf  - A pointer to the ptr_rtn_buf variable.
 * RETURN  : SPI_RTN_NO_ERROR - Successful.   Otherwise - Failed.
 * NOTES   :
 * MODIFICTION HISTORY:
 * Date 2014/12/19 by Chuck Kuo - The first revision for this function.
 *
 *------------------------------------------------------------------------------------
 */
static SPI_NAND_FLASH_RTN_T spi_nand_protocol_read_from_cache_single_speed( u32 data_offset, 
																			u32 len, 
																			u8 *ptr_rtn_buf, 
																			SPI_NAND_FLASH_READ_DUMMY_BYTE_T dummy_mode )
{			

	struct SPI_NAND_FLASH_INFO_T	*ptr_dev_info_t;
	SPI_NAND_FLASH_RTN_T	rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;
	
	ptr_dev_info_t	= _SPI_NAND_GET_DEVICE_INFO_PTR;	
	
	/* 1. Chip Select low */
	_SPI_NAND_READ_CHIP_SELECT_LOW();
	
	/* 2. Send 03h opcode */
	_SPI_NAND_WRITE_ONE_BYTE( _SPI_NAND_OP_READ_FROM_CACHE_SINGLE );
	
	/* 3. Send data_offset addr */
	if( dummy_mode == SPI_NAND_FLASH_READ_DUMMY_BYTE_PREPEND )
	{
		_SPI_NAND_WRITE_ONE_BYTE( (0xff) );						/* dummy byte */
	}
	
	
	if( ((ptr_dev_info_t->feature) & SPI_NAND_FLASH_PLANE_SELECT_HAVE) )
	{
		if( _plane_select_bit == 0)
		{
			_SPI_NAND_WRITE_ONE_BYTE( ((data_offset >> 8 ) &(0xef)) );
		}
		if( _plane_select_bit == 1)
		{
			_SPI_NAND_WRITE_ONE_BYTE( ((data_offset >> 8 ) | (0x10)) );
		}				
	}	
	else
	{
		_SPI_NAND_WRITE_ONE_BYTE( ((data_offset >> 8 ) &(0xff)) );
	}
	
	_SPI_NAND_WRITE_ONE_BYTE( ((data_offset      ) &(0xff)) );	
	
	if( dummy_mode == SPI_NAND_FLASH_READ_DUMMY_BYTE_APPEND )
	{
		_SPI_NAND_WRITE_ONE_BYTE( (0xff) );						/* dummy byte */
	}	
	
	/* 4. Read n byte (len) data */
	_SPI_NAND_READ_NBYTE( ptr_rtn_buf, len, SPI_CONTROLLER_SPEED_SINGLE);
		
	/* 5. Chip Select High */
	_SPI_NAND_READ_CHIP_SELECT_HIGH();
		
	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "spi_nand_protocol_read_from_cache_single_speed : data_offset=0x%x, buf=0x%x\n", data_offset, ptr_rtn_buf);	
	
	return (rtn_status);
}

/*------------------------------------------------------------------------------------
 * FUNCTION: static SPI_NAND_FLASH_RTN_T spi_nand_protocol_read_from_cache_dual_speed( u32   data_offset,
 *                                                                                     u32   len,
 *                                                                                     u8    *ptr_rtn_buf )
 * PURPOSE : To implement the SPI nand protocol for read from cache with dual speed.
 * AUTHOR  : Chuck Kuo
 * CALLED BY
 *   -
 * CALLS
 *   -
 * PARAMs  :
 *   INPUT : data_offset  - The data_offset variable of this function.
 *           len          - The len variable of this function.
 *   OUTPUT: ptr_rtn_buf  - A pointer to the ptr_rtn_buf variable.
 * RETURN  : SPI_RTN_NO_ERROR - Successful.   Otherwise - Failed.
 * NOTES   :
 * MODIFICTION HISTORY:
 * Date 2015/01/20 by Chuck Kuo - The first revision for this function.
 *
 *------------------------------------------------------------------------------------
 */
static SPI_NAND_FLASH_RTN_T spi_nand_protocol_read_from_cache_dual_speed( u32 data_offset, 
																		  u32 len, 
																		  u8 *ptr_rtn_buf,
																		  SPI_NAND_FLASH_READ_DUMMY_BYTE_T dummy_mode )
{			
	
	struct SPI_NAND_FLASH_INFO_T	*ptr_dev_info_t;
	SPI_NAND_FLASH_RTN_T	rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;
	
	ptr_dev_info_t	= _SPI_NAND_GET_DEVICE_INFO_PTR;
	
	/* 1. Chip Select low */
	_SPI_NAND_READ_CHIP_SELECT_LOW();
	
	/* 2. Send 3Bh opcode */
	_SPI_NAND_WRITE_ONE_BYTE( _SPI_NAND_OP_READ_FROM_CACHE_DUAL );
	
	/* 3. Send data_offset addr */
	if( dummy_mode == SPI_NAND_FLASH_READ_DUMMY_BYTE_PREPEND )
	{
		_SPI_NAND_WRITE_ONE_BYTE( (0xff) );						/* dummy byte */
	}	
	
	if( ((ptr_dev_info_t->feature) & SPI_NAND_FLASH_PLANE_SELECT_HAVE) )
	{
		if( _plane_select_bit == 0)
		{
			_SPI_NAND_WRITE_ONE_BYTE( ((data_offset >> 8 ) &(0xef)) );
		}
		if( _plane_select_bit == 1)
		{
			_SPI_NAND_WRITE_ONE_BYTE( ((data_offset >> 8 ) | (0x10)) );
		}				
	}	
	else
	{
		_SPI_NAND_WRITE_ONE_BYTE( ((data_offset >> 8 ) &(0xff)) );
	}

	_SPI_NAND_WRITE_ONE_BYTE( ((data_offset      ) &(0xff)) );
		_SPI_NAND_WRITE_ONE_BYTE( (0xff) );						/* dummy byte */
	
	/* 4. Read n byte (len) data */
	_SPI_NAND_READ_NBYTE( ptr_rtn_buf, len, SPI_CONTROLLER_SPEED_DUAL);
		
	/* 5. Chip Select High */
	_SPI_NAND_READ_CHIP_SELECT_HIGH();
	
	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "spi_nand_protocol_read_from_cache_dual_speed : data_offset=0x%x, buf=0x%x\n", data_offset, ptr_rtn_buf);
	
	return (rtn_status);
}

/*------------------------------------------------------------------------------------
 * FUNCTION: static SPI_NAND_FLASH_RTN_T spi_nand_protocol_read_from_cache_quad_speed( u32   data_offset,
 *                                                                                     u32   len,
 *                                                                                     u8    *ptr_rtn_buf )
 * PURPOSE : To implement the SPI nand protocol for read from cache with quad speed.
 * AUTHOR  : Chuck Kuo
 * CALLED BY
 *   -
 * CALLS
 *   -
 * PARAMs  :
 *   INPUT : data_offset  - The data_offset variable of this function.
 *           len          - The len variable of this function.
 *   OUTPUT: ptr_rtn_buf  - A pointer to the ptr_rtn_buf variable.
 * RETURN  : SPI_RTN_NO_ERROR - Successful.   Otherwise - Failed.
 * NOTES   :
 * MODIFICTION HISTORY:
 * Date 2015/01/20 by Chuck Kuo - The first revision for this function.
 *
 *------------------------------------------------------------------------------------
 */
static SPI_NAND_FLASH_RTN_T spi_nand_protocol_read_from_cache_quad_speed( u32 data_offset, 
																		  u32 len, 
																		  u8 *ptr_rtn_buf,
																		  SPI_NAND_FLASH_READ_DUMMY_BYTE_T dummy_mode )
{			
	
	struct SPI_NAND_FLASH_INFO_T	*ptr_dev_info_t;
	SPI_NAND_FLASH_RTN_T	rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;
	
	ptr_dev_info_t	= _SPI_NAND_GET_DEVICE_INFO_PTR;
	
	/* 1. Chip Select low */
	_SPI_NAND_READ_CHIP_SELECT_LOW();
	
	/* 2. Send 6Bh opcode */
	_SPI_NAND_WRITE_ONE_BYTE( _SPI_NAND_OP_READ_FROM_CACHE_QUAD );
	
	/* 3. Send data_offset addr */
	if( dummy_mode == SPI_NAND_FLASH_READ_DUMMY_BYTE_PREPEND )
	{
		_SPI_NAND_WRITE_ONE_BYTE( (0xff) );						/* dummy byte */
	}		

	if( ((ptr_dev_info_t->feature) & SPI_NAND_FLASH_PLANE_SELECT_HAVE) )
	{
		if( _plane_select_bit == 0)
		{
			_SPI_NAND_WRITE_ONE_BYTE( ((data_offset >> 8 ) &(0xef)) );
		}
		if( _plane_select_bit == 1)
		{
			_SPI_NAND_WRITE_ONE_BYTE( ((data_offset >> 8 ) | (0x10)) );
		}				
	}	
	else
	{
		_SPI_NAND_WRITE_ONE_BYTE( ((data_offset >> 8 ) &(0xff)) );
	}
	_SPI_NAND_WRITE_ONE_BYTE( ((data_offset      ) &(0xff)) );
		_SPI_NAND_WRITE_ONE_BYTE( (0xff) );						/* dummy byte */
	
	/* 4. Read n byte (len) data */
	_SPI_NAND_READ_NBYTE( ptr_rtn_buf, len, SPI_CONTROLLER_SPEED_QUAD);
		
	/* 5. Chip Select High */
	_SPI_NAND_READ_CHIP_SELECT_HIGH();
	
	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "spi_nand_protocol_read_from_cache_quad_speed : data_offset=0x%x, buf=0x%x\n", data_offset, ptr_rtn_buf);
	
	return (rtn_status);
}

/*------------------------------------------------------------------------------------
 * FUNCTION: static SPI_NAND_FLASH_RTN_T spi_nand_protocol_program_load_single_speed( u32     addr,
 *                                                                       u8      *ptr_data,
 *                                                                       u32     len       )
 * PURPOSE : To implement the SPI nand protocol for program load, with single speed.
 * AUTHOR  : Chuck Kuo
 * CALLED BY
 *   -
 * CALLS
 *   -
 * PARAMs  :
 *   INPUT : addr      - The addr variable of this function.
 *           ptr_data  - A pointer to the ptr_data variable.
 *           len       - The len variable of this function.
 *   OUTPUT: None
 * RETURN  : SPI_RTN_NO_ERROR - Successful.   Otherwise - Failed.
 * NOTES   :
 * MODIFICTION HISTORY:
 * Date 2014/12/17 by Chuck Kuo - The first revision for this function.
 *
 *------------------------------------------------------------------------------------
 */
static SPI_NAND_FLASH_RTN_T spi_nand_protocol_program_load_single_speed ( u32 addr, 
																		  u8 *ptr_data, 
																		  u32 len )
{

	struct SPI_NAND_FLASH_INFO_T	*ptr_dev_info_t;
	SPI_NAND_FLASH_RTN_T	rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;
	
	ptr_dev_info_t	= _SPI_NAND_GET_DEVICE_INFO_PTR;
	
	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "spi_nand_protocol_program_load_single_speed: addr=0x%x, len=0x%x\n", addr, len );
	
	/* 1. Chip Select low */
	_SPI_NAND_READ_CHIP_SELECT_LOW();
	
	/* 2. Send 02h opcode */
	_SPI_NAND_WRITE_ONE_BYTE( _SPI_NAND_OP_PROGRAM_LOAD_SINGLE );
	
	/* 3. Send address offset */
	if( ((ptr_dev_info_t->feature) & SPI_NAND_FLASH_PLANE_SELECT_HAVE) )
	{
		if( _plane_select_bit == 0)
		{
			_SPI_NAND_WRITE_ONE_BYTE( ((addr >> 8 ) & (0xef)) );
		}
		if( _plane_select_bit == 1)
		{
			_SPI_NAND_WRITE_ONE_BYTE( ((addr >> 8 ) | (0x10)) );
		}				
	}	
	else
	{
		_SPI_NAND_WRITE_ONE_BYTE( ((addr >> 8 ) & (0xff)) );
	}

	_SPI_NAND_WRITE_ONE_BYTE( ((addr)        & (0xff)) );
	
	/* 4. Send data */
	_SPI_NAND_WRITE_NBYTE( ptr_data, len, SPI_CONTROLLER_SPEED_SINGLE);
	
	/* 5. Chip Select High */
	_SPI_NAND_READ_CHIP_SELECT_HIGH();	
	
	return (rtn_status);
}

/*------------------------------------------------------------------------------------
 * FUNCTION: static SPI_NAND_FLASH_RTN_T spi_nand_protocol_program_load_quad_speed( u32  addr,
 *                                                                                  u8   *ptr_data,
 *                                                                                  u32  len       )
 * PURPOSE : To implement the SPI nand protocol for program load, with quad speed.
 * AUTHOR  : Chuck Kuo
 * CALLED BY
 *   -
 * CALLS
 *   -
 * PARAMs  :
 *   INPUT : addr      - The addr variable of this function.
 *           ptr_data  - A pointer to the ptr_data variable.
 *           len       - The len variable of this function.
 *   OUTPUT: None
 * RETURN  : SPI_RTN_NO_ERROR - Successful.   Otherwise - Failed.
 * NOTES   :
 * MODIFICTION HISTORY:
 * Date 2015/01/21 by Chuck Kuo - The first revision for this function.
 *
 *------------------------------------------------------------------------------------
 */
static SPI_NAND_FLASH_RTN_T spi_nand_protocol_program_load_quad_speed ( u32 addr, 
																		u8 *ptr_data, 
																		u32 len )
{
	struct SPI_NAND_FLASH_INFO_T	*ptr_dev_info_t;
	SPI_NAND_FLASH_RTN_T	rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;
	
	ptr_dev_info_t	= _SPI_NAND_GET_DEVICE_INFO_PTR;
	
	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "spi_nand_protocol_program_load_quad_speed: addr=0x%x, len=0x%x\n", addr, len );
	
	/* 1. Chip Select low */
	_SPI_NAND_READ_CHIP_SELECT_LOW();
	
	/* 2. Send 32h opcode */
	_SPI_NAND_WRITE_ONE_BYTE( _SPI_NAND_OP_PROGRAM_LOAD_QUAD );
	
	/* 3. Send address offset */
	if( ((ptr_dev_info_t->feature) & SPI_NAND_FLASH_PLANE_SELECT_HAVE)  )
	{
		if( _plane_select_bit == 0)
		{
			_SPI_NAND_WRITE_ONE_BYTE( ((addr >> 8 ) & (0xef)) );
		}
		if( _plane_select_bit == 1)
		{
			_SPI_NAND_WRITE_ONE_BYTE( ((addr >> 8 ) | (0x10)) );
		}				
	}	
	else
	{
		_SPI_NAND_WRITE_ONE_BYTE( ((addr >> 8 ) & (0xff)) );
	}

	_SPI_NAND_WRITE_ONE_BYTE( ((addr)        & (0xff)) );
	
	/* 4. Send data */
	_SPI_NAND_WRITE_NBYTE( ptr_data, len, SPI_CONTROLLER_SPEED_QUAD);
	
	/* 5. Chip Select High */
	_SPI_NAND_READ_CHIP_SELECT_HIGH();	
	
	return (rtn_status);
}


/*------------------------------------------------------------------------------------
 * FUNCTION: static SPI_NAND_FLASH_RTN_T spi_nand_protocol_program_load_random_single_speed( u32     addr,
 *                                                                       u8      *ptr_data,
 *                                                                       u32     len       )
 * PURPOSE : To implement the SPI nand protocol for program load, with single speed.
 * AUTHOR  : Chuck Kuo
 * CALLED BY
 *   -
 * CALLS
 *   -
 * PARAMs  :
 *   INPUT : addr      - The addr variable of this function.
 *           ptr_data  - A pointer to the ptr_data variable.
 *           len       - The len variable of this function.
 *   OUTPUT: None
 * RETURN  : SPI_RTN_NO_ERROR - Successful.   Otherwise - Failed.
 * NOTES   :
 * MODIFICTION HISTORY:
 * Date 2014/12/17 by Chuck Kuo - The first revision for this function.
 *
 *------------------------------------------------------------------------------------
 */
static SPI_NAND_FLASH_RTN_T spi_nand_protocol_program_load_random_single_speed ( u32 addr, 
																		  u8 *ptr_data, 
																		  u32 len )
{
	SPI_NAND_FLASH_RTN_T	rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;
	
	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "spi_nand_protocol_program_load_single_speed: addr=0x%x, len=0x%x\n", addr, len );
	
	/* 1. Chip Select low */
	_SPI_NAND_READ_CHIP_SELECT_LOW();
	
	/* 2. Send 84h opcode */
	_SPI_NAND_WRITE_ONE_BYTE( _SPI_NAND_OP_PROGRAM_LOAD_RAMDOM_SINGLE );
	
	/* 3. Send address offset */
	_SPI_NAND_WRITE_ONE_BYTE( ((addr >> 8  ) & 0xff) );
	_SPI_NAND_WRITE_ONE_BYTE( ((addr)        & 0xff) );
	
	/* 4. Send data */
	_SPI_NAND_WRITE_NBYTE( ptr_data, len, SPI_CONTROLLER_SPEED_SINGLE);
	
	/* 5. Chip Select High */
	_SPI_NAND_READ_CHIP_SELECT_HIGH();	
	
	return (rtn_status);
}

/*------------------------------------------------------------------------------------
 * FUNCTION: static SPI_NAND_FLASH_RTN_T spi_nand_protocol_program_load_random_quad_speed( u32  addr,
 *                                                                                  u8   *ptr_data,
 *                                                                                  u32  len       )
 * PURPOSE : To implement the SPI nand protocol for program load, with quad speed.
 * AUTHOR  : Chuck Kuo
 * CALLED BY
 *   -
 * CALLS
 *   -
 * PARAMs  :
 *   INPUT : addr      - The addr variable of this function.
 *           ptr_data  - A pointer to the ptr_data variable.
 *           len       - The len variable of this function.
 *   OUTPUT: None
 * RETURN  : SPI_RTN_NO_ERROR - Successful.   Otherwise - Failed.
 * NOTES   :
 * MODIFICTION HISTORY:
 * Date 2015/01/21 by Chuck Kuo - The first revision for this function.
 *
 *------------------------------------------------------------------------------------
 */
static SPI_NAND_FLASH_RTN_T spi_nand_protocol_program_load_random_quad_speed ( u32 addr, 
																		u8 *ptr_data, 
																		u32 len )
{
	SPI_NAND_FLASH_RTN_T	rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;
	
	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "spi_nand_protocol_program_load_quad_speed: addr=0x%x, len=0x%x\n", addr, len );
	
	/* 1. Chip Select low */
	_SPI_NAND_READ_CHIP_SELECT_LOW();
	
	/* 2. Send 34h opcode */
	_SPI_NAND_WRITE_ONE_BYTE( _SPI_NAND_OP_PROGRAM_LOAD_RAMDON_QUAD );
	
	/* 3. Send address offset */
	_SPI_NAND_WRITE_ONE_BYTE( ((addr >> 8  ) & 0xff) );
	_SPI_NAND_WRITE_ONE_BYTE( ((addr)        & 0xff) );
	
	/* 4. Send data */
	_SPI_NAND_WRITE_NBYTE( ptr_data, len, SPI_CONTROLLER_SPEED_QUAD);
	
	/* 5. Chip Select High */
	_SPI_NAND_READ_CHIP_SELECT_HIGH();	
	
	return (rtn_status);
}


/*------------------------------------------------------------------------------------
 * FUNCTION: static SPI_NAND_FLASH_RTN_T spi_nand_protocol_program_execute( u32  addr )
 * PURPOSE : To implement the SPI nand protocol for program execute.
 * AUTHOR  : Chuck Kuo
 * CALLED BY
 *   -
 * CALLS
 *   -
 * PARAMs  :
 *   INPUT : addr - The addr variable of this function.
 *   OUTPUT: None
 * RETURN  : SPI_RTN_NO_ERROR - Successful.   Otherwise - Failed.
 * NOTES   :
 * MODIFICTION HISTORY:
 * Date 2014/12/17 by Chuck Kuo - The first revision for this function.
 *
 *------------------------------------------------------------------------------------
 */
static SPI_NAND_FLASH_RTN_T spi_nand_protocol_program_execute ( u32 addr )
{
	SPI_NAND_FLASH_RTN_T	rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;

	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "spi_nand_protocol_program_execute: addr=0x%x\n", addr);	
	
	/* 1. Chip Select low */
	_SPI_NAND_READ_CHIP_SELECT_LOW();
	
	/* 2. Send 10h opcode */
	_SPI_NAND_WRITE_ONE_BYTE( _SPI_NAND_OP_PROGRAM_EXECUTE );
	
	/* 3. Send address offset */
	_SPI_NAND_WRITE_ONE_BYTE( ((addr >> 16  ) & 0xff) );
	_SPI_NAND_WRITE_ONE_BYTE( ((addr >> 8	 ) & 0xff) );
	_SPI_NAND_WRITE_ONE_BYTE( ((addr)         & 0xff) );
	
	/* 4. Chip Select High */
	_SPI_NAND_READ_CHIP_SELECT_HIGH();	
	
	return (rtn_status);
}

static void spi_nand_select_die ( u32 page_number )
{
	struct SPI_NAND_FLASH_INFO_T *ptr_dev_info_t;
	u8 die_id = 0;

	ptr_dev_info_t = _SPI_NAND_GET_DEVICE_INFO_PTR;

	if(((ptr_dev_info_t->feature) & SPI_NAND_FLASH_DIE_SELECT_HAVE))
	{
		die_id = ((page_number >> 16) & 0xff);

		if (_die_id != die_id)
		{
			_die_id = die_id;
			spi_nand_protocol_die_select(die_id);

			_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_2, "spi_nand_protocol_die_select: die_id=0x%x\n", die_id);
		}
	}
}

static SPI_NAND_FLASH_RTN_T ecc_fail_check( u32 page_number )
{
	u8								status;
	struct SPI_NAND_FLASH_INFO_T	*ptr_dev_info_t;
	SPI_NAND_FLASH_RTN_T			rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;
	
	ptr_dev_info_t	= _SPI_NAND_GET_DEVICE_INFO_PTR;

	spi_nand_protocol_get_status_reg_3( &status); 	


	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "ecc_fail_check: status=0x%x\n", status);

	if( ptr_dev_info_t->mfr_id == _SPI_NAND_MANUFACTURER_ID_GIGADEVICE )
	{


		if( ptr_dev_info_t->dev_id == _SPI_NAND_DEVICE_ID_GD5F1GQ4UAYIG)
		{
			if( ((status & 0x30) >> 4) == 0x2 )
			{
				rtn_status = SPI_NAND_FLASH_RTN_DETECTED_BAD_BLOCK;
			}
		}

		if( ptr_dev_info_t->dev_id == _SPI_NAND_DEVICE_ID_GD5F1GQ4UBYIG)
		{
			if( ((status & 0x30) >> 4) == 0x2 )
			{
				rtn_status = SPI_NAND_FLASH_RTN_DETECTED_BAD_BLOCK;
			}
		}

		if( ptr_dev_info_t->dev_id == _SPI_NAND_DEVICE_ID_GD5F2GQ4UBYIG)
		{
			if( ((status & 0x30) >> 4) == 0x2 )
			{
				rtn_status = SPI_NAND_FLASH_RTN_DETECTED_BAD_BLOCK;
			}
		}


		if( ptr_dev_info_t->dev_id == _SPI_NAND_DEVICE_ID_GD5F1GQ4UCYIG)
		{
			if( ((status & 0x70) >> 4) == 0x7 )
			{
				rtn_status = SPI_NAND_FLASH_RTN_DETECTED_BAD_BLOCK;
			}
		}

		if( ptr_dev_info_t->dev_id == _SPI_NAND_DEVICE_ID_GD5F2GQ4UCYIG)
		{
			if( ((status & 0x70) >> 4) == 0x7 )
			{
				rtn_status = SPI_NAND_FLASH_RTN_DETECTED_BAD_BLOCK;
			}
		}		
	
	}


	if( ptr_dev_info_t->mfr_id == _SPI_NAND_MANUFACTURER_ID_MXIC )
	{

		if( ((status & 0x30) >> 4) == 0x2 )
		{
			rtn_status = SPI_NAND_FLASH_RTN_DETECTED_BAD_BLOCK;
		}

	}
	
	if( ptr_dev_info_t->mfr_id == _SPI_NAND_MANUFACTURER_ID_WINBOND )
	{

		if( ((status & 0x30) >> 4) == 0x2 )
		{
			rtn_status = SPI_NAND_FLASH_RTN_DETECTED_BAD_BLOCK;
		}

	}	

	if( ptr_dev_info_t->mfr_id == _SPI_NAND_MANUFACTURER_ID_ESMT )
	{

		if( ((status & 0x30) >> 4) == 0x2 )
		{
			rtn_status = SPI_NAND_FLASH_RTN_DETECTED_BAD_BLOCK;
		}

	}		
	if( ptr_dev_info_t->mfr_id == _SPI_NAND_MANUFACTURER_ID_ZENTEL )
	{

		if( ((status & 0x30) >> 4) == 0x2 )
		{
			rtn_status = SPI_NAND_FLASH_RTN_DETECTED_BAD_BLOCK;
		}

	}
	if( ptr_dev_info_t->mfr_id == _SPI_NAND_MANUFACTURER_ID_ETRON )
	{

		if( ((status & 0x30) >> 4) == 0x2 )
		{
			rtn_status = SPI_NAND_FLASH_RTN_DETECTED_BAD_BLOCK;
		}

	}
	if( ptr_dev_info_t->mfr_id == _SPI_NAND_MANUFACTURER_ID_TOSHIBA )
	{

		if( ((status & 0x30) >> 4) == 0x2 )
		{
			rtn_status = SPI_NAND_FLASH_RTN_DETECTED_BAD_BLOCK;
		}

	}	
	if( ptr_dev_info_t->mfr_id == _SPI_NAND_MANUFACTURER_ID_MICRON )
	{

		if( ((status & 0x70) >> 4) == 0x2 )
		{
			rtn_status = SPI_NAND_FLASH_RTN_DETECTED_BAD_BLOCK;
		}

	}

	

	if( rtn_status == SPI_NAND_FLASH_RTN_DETECTED_BAD_BLOCK)
	{
		_SPI_NAND_PRINTF("[spinand_ecc_fail_check] : ECC cannot recover detected !, page=0x%x\n", page_number);
	}

	return (rtn_status);
}


/*------------------------------------------------------------------------------------
 * FUNCTION: static SPI_NAND_FLASH_RTN_T spi_nand_load_page_into_cache( long  page_number )
 * PURPOSE : To load page into SPI NAND chip.
 * AUTHOR  : Chuck Kuo
 * CALLED BY
 *   -
 * CALLS
 *   -
 * PARAMs  :
 *   INPUT : page_number - The page_number variable of this function.
 *   OUTPUT: None
 * RETURN  : SPI_RTN_NO_ERROR - Successful.   Otherwise - Failed.
 * NOTES   :
 * MODIFICTION HISTORY:
 * Date 2014/12/16 by Chuck Kuo - The first revision for this function.
 *
 *------------------------------------------------------------------------------------
 */
static SPI_NAND_FLASH_RTN_T spi_nand_load_page_into_cache( u32 page_number)
{
	u8						status;
	SPI_NAND_FLASH_RTN_T	rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;
	
	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "spi_nand_load_page_into_cache: page number =0x%x\n", page_number);
		
	if( _current_page_num == page_number )
	{
		_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "spi_nand_load_page_into_cache: page number == _current_page_num\n");
	}
	else
	{
		spi_nand_select_die ( page_number );

		spi_nand_protocol_page_read ( page_number );
		
		/*  Checking status for load page/erase/program complete */
		do{
			spi_nand_protocol_get_status_reg_3( &status);
		}while( status & _SPI_NAND_VAL_OIP) ;
		
		_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "spi_nand_load_page_into_cache : status =0x%x\n", status);

		rtn_status = ecc_fail_check(page_number);
	}
	
	return (rtn_status);
	
}

/*------------------------------------------------------------------------------------
 * FUNCTION: static SPI_NAND_FLASH_RTN_T spi_nand_block_aligned_check( u32   addr,
 *                                                                     u32   len  )
 * PURPOSE : To check block align.
 * AUTHOR  : Chuck Kuo
 * CALLED BY
 *   -
 * CALLS
 *   -
 * PARAMs  :
 *   INPUT : addr - The addr variable of this function.
 *           len  - The len variable of this function.
 *   OUTPUT: None
 * RETURN  : SPI_RTN_NO_ERROR - Successful.   Otherwise - Failed.
 * NOTES   :
 * MODIFICTION HISTORY:
 * Date 2014/12/15 by Chuck Kuo - The first revision for this function.
 *
 *------------------------------------------------------------------------------------
 */
static SPI_NAND_FLASH_RTN_T spi_nand_block_aligned_check( u32 addr, 
														  u32 len )
{
	struct SPI_NAND_FLASH_INFO_T		*ptr_dev_info_t;
	SPI_NAND_FLASH_RTN_T				rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;	
	
	
	ptr_dev_info_t = _SPI_NAND_GET_DEVICE_INFO_PTR ;
	
	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "SPI_NAND_BLOCK_ALIGNED_CHECK_check: addr=0x%x, len=0x%x, block size =0x%x \n", addr, len, (ptr_dev_info_t->erase_size));	

	if (_SPI_NAND_BLOCK_ALIGNED_CHECK(len, (ptr_dev_info_t->erase_size))) 
	{
		len = ( (len/ptr_dev_info_t->erase_size) + 1) * (ptr_dev_info_t->erase_size);
		_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "spi_nand__SPI_NAND_BLOCK_ALIGNED_CHECK_check: erase block aligned first check OK, addr:%x len:%x\n", addr, len, (ptr_dev_info_t->erase_size));
	}

	if (_SPI_NAND_BLOCK_ALIGNED_CHECK(addr, (ptr_dev_info_t->erase_size)) || _SPI_NAND_BLOCK_ALIGNED_CHECK(len, (ptr_dev_info_t->erase_size)) ) 
	{
		_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "spi_nand__SPI_NAND_BLOCK_ALIGNED_CHECK_check: erase block not aligned, addr:0x%x len:0x%x, blocksize:0x%x\n", addr, len, (ptr_dev_info_t->erase_size));		
		rtn_status = SPI_NAND_FLASH_RTN_ALIGNED_CHECK_FAIL;
	}	
	
	return (rtn_status);
}


static SPI_NAND_FLASH_RTN_T spi_nand_erase_block ( u32 block_index)
{

	u8						status;
	SPI_NAND_FLASH_RTN_T	rtn_status	= SPI_NAND_FLASH_RTN_NO_ERROR;

	spi_nand_select_die ( (block_index << _SPI_NAND_BLOCK_ROW_ADDRESS_OFFSET) );

	/* 2.2 Enable write_to flash */
	spi_nand_protocol_write_enable();		

	/* 2.3 Erasing one block */
	spi_nand_protocol_block_erase( block_index );

	/* 2.4 Checking status for erase complete */
	do{
		spi_nand_protocol_get_status_reg_3( &status);
	}while( status & _SPI_NAND_VAL_OIP) ;
				
	/* 2.5 Disable write_flash */
	spi_nand_protocol_write_disable();

	/* 2.6 Check Erase Fail Bit */
	if( status & _SPI_NAND_VAL_ERASE_FAIL )
	{
		_SPI_NAND_PRINTF("spi_nand_erase_block : erase block fail, block=0x%x, status=0x%x\n", block_index, status);
		rtn_status = SPI_NAND_FLASH_RTN_ERASE_FAIL;
	}

	return rtn_status;
}


/*------------------------------------------------------------------------------------
 * FUNCTION: static SPI_NAND_FLASH_RTN_T spi_nand_erase_internal( u32     addr,
 *                                               				  u32     len )
 * PURPOSE : To erase flash internally.
 * AUTHOR  : Chuck Kuo
 * CALLED BY
 *   -
 * CALLS
 *   -
 * PARAMs  :
 *   INPUT : addr - The addr variable of this function.
 *           len - The size variable of this function.
 *   OUTPUT: None
 * RETURN  : SPI_RTN_NO_ERROR - Successful.   Otherwise - Failed.
 * NOTES   :
 * MODIFICTION HISTORY:
 * Date 2014/12/15 by Chuck Kuo - The first revision for this function.
 *
 *------------------------------------------------------------------------------------
 */
static SPI_NAND_FLASH_RTN_T spi_nand_erase_internal( u32 addr, 
													 u32 len )
{		
	u32						block_index = 0;
	u32						erase_len	= 0;
	SPI_NAND_FLASH_RTN_T	rtn_status  = SPI_NAND_FLASH_RTN_NO_ERROR;
	
#if	defined(TCSUPPORT_NAND_BMT) && !defined(LZMA_IMG)

	struct SPI_NAND_FLASH_INFO_T	*ptr_dev_info_t;
    unsigned short phy_block_bbt;
	u32				logical_block, physical_block;
	ptr_dev_info_t	= _SPI_NAND_GET_DEVICE_INFO_PTR;
#endif	

	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "\nspi_nand_erase_internal (in): addr=0x%x, len=0x%x\n", addr, len );		
		
	_SPI_NAND_SEMAPHORE_LOCK();	
	
	SPI_NAND_Flash_Clear_Read_Cache_Data();
	
	/* 1. Check the address and len must aligned to NAND Flash block size */
	if( spi_nand_block_aligned_check( addr, len) == SPI_NAND_FLASH_RTN_NO_ERROR)
	{
		/* 2. Erase block one by one */
		while( erase_len < len )
		{
			/* 2.1 Caculate Block index */
			block_index = (addr/(_current_flash_info_t.erase_size));
#if	defined(TCSUPPORT_NAND_BMT) && !defined(LZMA_IMG)
			logical_block = block_index;
			physical_block = get_mapping_block_index(logical_block, &phy_block_bbt);		
			if( physical_block != logical_block)
			{			
				_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "Bad Block Mapping, from %d block to %d block\n", logical_block, physical_block);
			}
			block_index = physical_block;
#endif							
			_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "spi_nand_erase_internal: addr=0x%x, len=0x%x, block_idx=0x%x\n", addr, len, block_index );
						
						
			rtn_status = spi_nand_erase_block(block_index);
						
			/* 2.6 Check Erase Fail Bit */
			if( rtn_status != SPI_NAND_FLASH_RTN_NO_ERROR )
			{
#if	defined(TCSUPPORT_NAND_BMT) && !defined(LZMA_IMG)
					if (update_bmt(addr,UPDATE_ERASE_FAIL, NULL, NULL))
					{
						_SPI_NAND_PRINTF("Erase fail at block: %d, update BMT success\n", addr/(_current_flash_info_t.erase_size));
						rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;
					}
					else
					{
						_SPI_NAND_PRINTF("Erase fail at block: %d, update BMT fail\n", addr/(_current_flash_info_t.erase_size));
						rtn_status = SPI_NAND_FLASH_RTN_ERASE_FAIL;
						break;
					}					
#else
				_SPI_NAND_PRINTF("spi_nand_erase_internal : Erase Fail at addr=0x%x, len=0x%x, block_idx=0x%x\n", addr, len, block_index);
				rtn_status = SPI_NAND_FLASH_RTN_ERASE_FAIL;
				break;

#endif
			}			
			
			/* 2.7 Erase next block if needed */
			addr		+= _current_flash_info_t.erase_size;
			erase_len	+= _current_flash_info_t.erase_size;
			
		}		
	}
	else
	{
		rtn_status = SPI_NAND_FLASH_RTN_ALIGNED_CHECK_FAIL;
	}	
	
	_SPI_NAND_SEMAPHORE_UNLOCK();
		
	return 	(rtn_status);
}




static SPI_NAND_FLASH_RTN_T spi_nand_read_page ( u32 page_number, 
  													   SPI_NAND_FLASH_READ_SPEED_MODE_T speed_mode
  													 )
{

	u32			 					idx=0;
	u32								i, j;
	struct SPI_NAND_FLASH_INFO_T	*ptr_dev_info_t;
	SPI_NAND_FLASH_RTN_T			rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;
	struct spi_nand_flash_oobfree 	*ptr_oob_entry_idx;	

	ptr_dev_info_t	= _SPI_NAND_GET_DEVICE_INFO_PTR;			

	/* 1. Load Page into cache of NAND Flash Chip */
	if( spi_nand_load_page_into_cache(page_number) == SPI_NAND_FLASH_RTN_DETECTED_BAD_BLOCK )
	{
		_SPI_NAND_PRINTF("spi_nand_read_page: Bad Block, ECC cannot recovery detecte, page=0x%x\n", page_number);		
		rtn_status = SPI_NAND_FLASH_RTN_DETECTED_BAD_BLOCK;
	}

	/* 2. Read whole data from cache of NAND Flash Chip */
	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "spi_nand_read_page: curren_page_num=0x%x, page_number=0x%x\n", _current_page_num, page_number);

	/* No matter what status, we must read the cache data to dram */
	if( (_current_page_num != page_number) )
	{
		memset(_current_cache_page, 0x0, sizeof(_current_cache_page)); 		
		
		_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_2, "spi_nand_read_page: before read, _current_cache_page:\n");
		_SPI_NAND_DEBUG_PRINTF_ARRAY(SPI_NAND_FLASH_DEBUG_LEVEL_2, &_current_cache_page[0], _SPI_NAND_CACHE_SIZE);
		
		if( ((ptr_dev_info_t->feature) & SPI_NAND_FLASH_PLANE_SELECT_HAVE) )
		{
			_plane_select_bit = ((page_number >> 6)& (0x1));

			_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1,"spi_nand_read_page: plane select = 0x%x\n",  _plane_select_bit);			
		}
		
		switch (speed_mode)
		{
			case SPI_NAND_FLASH_READ_SPEED_MODE_SINGLE:
				spi_nand_protocol_read_from_cache_single_speed( 0, ((ptr_dev_info_t->page_size)+(ptr_dev_info_t->oob_size)), &_current_cache_page[0], ptr_dev_info_t->dummy_mode );
				break;
				
			case SPI_NAND_FLASH_READ_SPEED_MODE_DUAL:
				spi_nand_protocol_read_from_cache_dual_speed( 0, ((ptr_dev_info_t->page_size)+(ptr_dev_info_t->oob_size)), &_current_cache_page[0], ptr_dev_info_t->dummy_mode );
				break;										
				
			case SPI_NAND_FLASH_READ_SPEED_MODE_QUAD:
				spi_nand_protocol_read_from_cache_quad_speed( 0, ((ptr_dev_info_t->page_size)+(ptr_dev_info_t->oob_size)), &_current_cache_page[0], ptr_dev_info_t->dummy_mode );
				break;

			default:
				break;
		}
		
		_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_2, "spi_nand_read_page: after read, _current_cache_page:\n");
		_SPI_NAND_DEBUG_PRINTF_ARRAY(SPI_NAND_FLASH_DEBUG_LEVEL_2, &_current_cache_page[0], _SPI_NAND_CACHE_SIZE);

		/* Divide read page into data segment and oob segment  */
		memcpy( &_current_cache_page_data[0], &_current_cache_page[0], (ptr_dev_info_t->page_size) );
		memcpy( &_current_cache_page_oob[0],  &_current_cache_page[(ptr_dev_info_t->page_size)], (ptr_dev_info_t->oob_size) );

		idx = 0;
		ptr_oob_entry_idx = (struct spi_nand_flash_oobfree*) &( (ptr_dev_info_t->oob_free_layout)->oobfree );

		/* Transter oob area from physical offset into logical offset */
		for( i=0; (i<SPI_NAND_FLASH_OOB_FREE_ENTRY_MAX) && (ptr_oob_entry_idx[i].len) && (idx< ((ptr_dev_info_t->oob_free_layout)->oobsize)) ; i++)
		{
			for(j=0; (j< (ptr_oob_entry_idx[i].len)) && (idx<(ptr_dev_info_t->oob_free_layout->oobsize)) ; j++)
			{
				/* _SPI_NAND_PRINTF("i=%d , j=%d, len=%d, idx=%d, size=%d\n", i, j,(ptr_oob_entry_idx[i].len), idx, (ptr_dev_info_t->oob_free_layout->oobsize) ); */
				_current_cache_page_oob_mapping[idx] = _current_cache_page_oob[(ptr_oob_entry_idx[i].offset)+j];
				idx++;
			}
		}	

		_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_2, "spi_nand_read_page: _current_cache_page:\n");
		_SPI_NAND_DEBUG_PRINTF_ARRAY(SPI_NAND_FLASH_DEBUG_LEVEL_2, &_current_cache_page[0], ((ptr_dev_info_t->page_size)+(ptr_dev_info_t->oob_size)));
		_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_2, "spi_nand_read_page: _current_cache_page_oob:\n");
		_SPI_NAND_DEBUG_PRINTF_ARRAY(SPI_NAND_FLASH_DEBUG_LEVEL_2, &_current_cache_page_oob[0], (ptr_dev_info_t->oob_size));		
		_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_2, "spi_nand_read_page: _current_cache_page_oob_mapping:\n");
		_SPI_NAND_DEBUG_PRINTF_ARRAY(SPI_NAND_FLASH_DEBUG_LEVEL_2, &_current_cache_page_oob_mapping[0], (ptr_dev_info_t->oob_size));

		_current_page_num = page_number; 
	}	


	return rtn_status;


}
						


static SPI_NAND_FLASH_RTN_T spi_nand_write_page(	u32 page_number, 
														u32 data_offset,
													  	u8  *ptr_data, 
													  	u32 data_len, 
													  	u32 oob_offset,
														u8  *ptr_oob , 
														u32 oob_len,
													    SPI_NAND_FLASH_WRITE_SPEED_MODE_T speed_mode )
{

		u8		status, status_2;
		u32		i=0,j=0,idx=0;
		struct SPI_NAND_FLASH_INFO_T	*ptr_dev_info_t;
		struct spi_nand_flash_oobfree 	*ptr_oob_entry_idx;	
		SPI_NAND_FLASH_RTN_T			rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;

		ptr_dev_info_t	= _SPI_NAND_GET_DEVICE_INFO_PTR;	

		/* Read Current page data to software cache buffer */
		spi_nand_read_page(page_number, SPI_NAND_FLASH_READ_SPEED_MODE_SINGLE);

		/* Rewirte the software cahe buffer */
		if(data_len > 0)
		{		
			memcpy( &_current_cache_page_data[data_offset], &ptr_data[0], data_len );
		}			

		if( oob_len > 0 )	/* Write OOB */
		{
			ptr_oob_entry_idx = (struct spi_nand_flash_oobfree*) &( ptr_dev_info_t->oob_free_layout->oobfree );
			
			for( i=0; (i<SPI_NAND_FLASH_OOB_FREE_ENTRY_MAX) && (ptr_oob_entry_idx[i].len) && ((idx< (ptr_dev_info_t->oob_free_layout->oobsize)) && (idx < oob_len))  ; i++)
			{
				for(j=0; (j< (ptr_oob_entry_idx[i].len)) && (idx<(ptr_dev_info_t->oob_free_layout->oobsize)) && ((idx< (ptr_dev_info_t->oob_free_layout->oobsize)) && (idx < oob_len)) ; j++)
				{

					_current_cache_page_oob[(ptr_oob_entry_idx[i].offset)+j] &= ptr_oob[idx];
					idx++;
				}
			}			
		}

		memcpy( &_current_cache_page[0], &_current_cache_page_data[0], _SPI_NAND_PAGE_SIZE );
		memcpy( &_current_cache_page[_SPI_NAND_PAGE_SIZE],  &_current_cache_page_oob[0], _SPI_NAND_OOB_SIZE );


		_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_2, "spi_nand_write_page: data_offset=0x%x, date_len=0x%x, oob_offset=0x%x, oob_len=0x%x\n", data_offset, data_len, oob_offset, oob_len);
		_SPI_NAND_DEBUG_PRINTF_ARRAY(SPI_NAND_FLASH_DEBUG_LEVEL_2, &_current_cache_page[0], ((ptr_dev_info_t->page_size) + (ptr_dev_info_t->oob_size)));		
	
		if( ((ptr_dev_info_t->feature) & SPI_NAND_FLASH_PLANE_SELECT_HAVE)  )
		{
			_plane_select_bit = ((page_number >> 6) & (0x1));

			_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_2, "spi_nand_write_page: _plane_select_bit=0x%x\n", _plane_select_bit );

		}

		spi_nand_select_die ( page_number );

		/* Different Manafacture have different prgoram flow and setting */

		if( ((ptr_dev_info_t->mfr_id) == _SPI_NAND_MANUFACTURER_ID_MXIC ) || \
			((ptr_dev_info_t->mfr_id) == _SPI_NAND_MANUFACTURER_ID_WINBOND ) || \
			((ptr_dev_info_t->mfr_id) == _SPI_NAND_MANUFACTURER_ID_ESMT)  || \
			((ptr_dev_info_t->mfr_id) == _SPI_NAND_MANUFACTURER_ID_ETRON) )
		{							
			/*	Enable write_to flash */
			spi_nand_protocol_write_enable();		
			
			/* Proram data into buffer of SPI NAND chip */
			switch( speed_mode )
			{						
				case SPI_NAND_FLASH_WRITE_SPEED_MODE_SINGLE:
					spi_nand_protocol_program_load_single_speed( 0, &_current_cache_page[0], ((ptr_dev_info_t->page_size) + (ptr_dev_info_t->oob_size)) );
					break;
					
				case SPI_NAND_FLASH_WRITE_SPEED_MODE_QUAD:
					spi_nand_protocol_program_load_quad_speed( 0, &_current_cache_page[0], ((ptr_dev_info_t->page_size) + (ptr_dev_info_t->oob_size)));
					break;

				default:
					break;
			}				
		}	
		else if( ((ptr_dev_info_t->mfr_id) == _SPI_NAND_MANUFACTURER_ID_GIGADEVICE) )
		{			

			switch( speed_mode )
			{						
				case SPI_NAND_FLASH_WRITE_SPEED_MODE_SINGLE:
					spi_nand_protocol_program_load_single_speed( 0, &_current_cache_page[0], ((ptr_dev_info_t->page_size) + (ptr_dev_info_t->oob_size)) );
					break;
					
				case SPI_NAND_FLASH_WRITE_SPEED_MODE_QUAD:
					spi_nand_protocol_program_load_quad_speed( 0, &_current_cache_page[0], ((ptr_dev_info_t->page_size) + (ptr_dev_info_t->oob_size)) );
					break;
					
				default:
					break;
			}
						
			/*	Enable write_to flash */
			spi_nand_protocol_write_enable();					
		}					
		else
		{
			/*	Enable write_to flash */
			spi_nand_protocol_write_enable();		
			
			/* Proram data into buffer of SPI NAND chip */
			switch( speed_mode )
			{						
				case SPI_NAND_FLASH_WRITE_SPEED_MODE_SINGLE:
					spi_nand_protocol_program_load_single_speed(  0, &_current_cache_page[0], ((ptr_dev_info_t->page_size) + (ptr_dev_info_t->oob_size)) );
					break;
					
				case SPI_NAND_FLASH_WRITE_SPEED_MODE_QUAD:
					spi_nand_protocol_program_load_quad_speed(  0, &_current_cache_page[0], ((ptr_dev_info_t->page_size) + (ptr_dev_info_t->oob_size)) );
					break;	

				default:
					break;
			}
		}		

		/*	Execute program data into SPI NAND chip  */
		spi_nand_protocol_program_execute ( page_number );

		/*	Checking status for erase complete */
		do{
			spi_nand_protocol_get_status_reg_3( &status);
		}while( status & _SPI_NAND_VAL_OIP) ;
					
		/*. Disable write_flash */
		spi_nand_protocol_write_disable();

		spi_nand_protocol_get_status_reg_1( &status_2);

		_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "[spi_nand_write_page]: status 1 = 0x%x, status 3 =0x%x\n", status_2, status);

		/*	Check Program Fail Bit */
		if( status & _SPI_NAND_VAL_PROGRAM_FAIL )
		{
			_SPI_NAND_PRINTF("spi_nand_write_page : Program Fail at addr_offset =0x%x, page_number=0x%x, status=0x%x\n", data_offset, page_number, status);
			rtn_status = SPI_NAND_FLASH_RTN_PROGRAM_FAIL;
		}		


		return (rtn_status);
	
}




static SPI_NAND_FLASH_RTN_T spi_nand_write_page_internal(	u32 page_number, 
																	u32 data_offset,
													  				u8  *ptr_data, 
																  	u32 data_len, 
																  	u32 oob_offset,
																	u8  *ptr_oob , 
																	u32 oob_len,
																    SPI_NAND_FLASH_WRITE_SPEED_MODE_T speed_mode )
{

	u32					 			addr_offset;
	struct SPI_NAND_FLASH_INFO_T	*ptr_dev_info_t;
	SPI_NAND_FLASH_RTN_T			rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;
		
#if	defined(TCSUPPORT_NAND_BMT) && !defined(LZMA_IMG)
    unsigned short phy_block_bbt;
	u32			   logical_block, physical_block;
	u32			   page_offset_in_block;
#endif


	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "[spi_nand_write_page_internal]: enter\n");
	
	ptr_dev_info_t	= _SPI_NAND_GET_DEVICE_INFO_PTR;	
	
	_SPI_NAND_SEMAPHORE_LOCK();	
	
	SPI_NAND_Flash_Clear_Read_Cache_Data();
	
#if	defined(TCSUPPORT_NAND_BMT) && !defined(LZMA_IMG)
			page_offset_in_block = ((page_number * (ptr_dev_info_t->page_size))%(ptr_dev_info_t->erase_size))/(ptr_dev_info_t->page_size);
			logical_block = ((page_number * (ptr_dev_info_t->page_size))/(ptr_dev_info_t->erase_size)) ;
			physical_block = get_mapping_block_index(logical_block, &phy_block_bbt);
	
			if( physical_block != logical_block)
			{			
				_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "Bad Block Mapping, from %d block to %d block\n", logical_block, physical_block);
			}

			page_number = (page_offset_in_block)+((physical_block*(ptr_dev_info_t->erase_size))/(ptr_dev_info_t->page_size));
#endif
		
		_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "[spi_nand_write_page_internal]: page_number = 0x%x\n", page_number);
		
#if	defined(TCSUPPORT_NAND_BMT) && !defined(LZMA_IMG)
		if(block_is_in_bmt_region(physical_block))
		{						
			memcpy(ptr_oob + OOB_INDEX_OFFSET, &phy_block_bbt, OOB_INDEX_SIZE);
		}
		
       if(_SPI_NAND_TEST_FLAG == 0)		
       {


		   _SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_2, "spi_nand_write_page_internal: data_offset=0x%x, date_len=0x%x, oob_offset=0x%x, oob_len=0x%x\n", data_offset, data_len, oob_offset, oob_len);
			_SPI_NAND_DEBUG_PRINTF_ARRAY(SPI_NAND_FLASH_DEBUG_LEVEL_2, &ptr_oob[0], oob_len);
	   
			rtn_status = spi_nand_write_page(page_number, addr_offset, ptr_data, data_len, 0, ptr_oob, MAX_USE_OOB_SIZE , speed_mode);
       }
	   else
	   {
	   		rtn_status = SPI_NAND_FLASH_RTN_PROGRAM_FAIL;
	   }
					
		if(rtn_status != SPI_NAND_FLASH_RTN_NO_ERROR)
		{
		    _SPI_NAND_PRINTF("write fail at page: 0x%x \n", page_number);
            if (update_bmt(page_number * (ptr_dev_info_t->page_size), UPDATE_WRITE_FAIL, ptr_data, ptr_oob))
            {
                _SPI_NAND_PRINTF("Update BMT success\n");
				rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;

            }
            else
            {
                _SPI_NAND_PRINTF("Update BMT fail\n");
            }		
		}
#else
		rtn_status = spi_nand_write_page(page_number, addr_offset, ptr_data, data_len, 0, ptr_oob, MAX_USE_OOB_SIZE , speed_mode);
#endif


	SPI_NAND_Flash_Clear_Read_Cache_Data();
	
	_SPI_NAND_SEMAPHORE_UNLOCK();
	
	return (rtn_status);
}



/*------------------------------------------------------------------------------------
 * FUNCTION: static SPI_NAND_FLASH_RTN_T spi_nand_write_internal( u32    dst_addr,
 *                                                                u32    len,
 *                                                                u32    *ptr_rtn_len,
 *                                                                u8*    ptr_buf      )
 * PURPOSE : To write flash internally.
 * AUTHOR  : Chuck Kuo
 * CALLED BY
 *   -
 * CALLS
 *   -
 * PARAMs  :
 *   INPUT : dst_addr     - The dst_addr variable of this function.
 *           len          - The len variable of this function.
 *           ptr_buf      - A pointer to the ptr_buf variable.
 *   OUTPUT: ptr_rtn_len  - A pointer to the ptr_rtn_len variable.
 * RETURN  : SPI_RTN_NO_ERROR - Successful.   Otherwise - Failed.
 * NOTES   :
 * MODIFICTION HISTORY:
 * Date 2014/12/19 by Chuck Kuo - The first revision for this function.
 *
 *------------------------------------------------------------------------------------
 */
 static SPI_NAND_FLASH_RTN_T spi_nand_write_internal( u32 dst_addr, 
													 u32 len, 
													 u32 *ptr_rtn_len, 
													 u8* ptr_buf, 
													 SPI_NAND_FLASH_WRITE_SPEED_MODE_T speed_mode )
{
	u32					 			remain_len, write_addr, data_len, page_number, physical_dst_addr;
	u32					 			addr_offset;
	struct SPI_NAND_FLASH_INFO_T	*ptr_dev_info_t;
	SPI_NAND_FLASH_RTN_T			rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;
		
#if	defined(TCSUPPORT_NAND_BMT) && !defined(LZMA_IMG)
    unsigned short phy_block_bbt;
	unsigned long  addr_offset_in_block;
	u32			   logical_block, physical_block;
	u8			   oob_buf[_SPI_NAND_OOB_SIZE]={0xff};
#endif

		
	*ptr_rtn_len 	= 0;
	ptr_dev_info_t	= _SPI_NAND_GET_DEVICE_INFO_PTR;	
	remain_len 		= len;
	write_addr 		= dst_addr;
	
	_SPI_NAND_SEMAPHORE_LOCK();	
	
	SPI_NAND_Flash_Clear_Read_Cache_Data();

	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "spi_nand_write_internal: remain_len =0x%x\n", remain_len);
	
	while( remain_len > 0 )	
	{
		physical_dst_addr = write_addr;
	
#if	defined(TCSUPPORT_NAND_BMT) && !defined(LZMA_IMG)
		memset(oob_buf, 0xff, _SPI_NAND_OOB_SIZE);	
			addr_offset_in_block = (write_addr %(ptr_dev_info_t->erase_size) );
			logical_block = (write_addr / (ptr_dev_info_t->erase_size));
			physical_block = get_mapping_block_index(logical_block, &phy_block_bbt);
			physical_dst_addr = (physical_block * (ptr_dev_info_t->erase_size))+ addr_offset_in_block;
	
			if( physical_block != logical_block)
			{			
				_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "Bad Block Mapping, from %d block to %d block\n", logical_block, physical_block);
			}
#endif
	
		/* Caculate page number */
		addr_offset = (physical_dst_addr % (ptr_dev_info_t->page_size));
		page_number = (physical_dst_addr / (ptr_dev_info_t->page_size));		
		
		_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "\nspi_nand_write_internal: addr_offset =0x%x, page_number=0x%x, remain_len=0x%x, page_size=0x%x\n", addr_offset, page_number, remain_len,(ptr_dev_info_t->page_size) );		
		if( ((addr_offset + remain_len ) > (ptr_dev_info_t->page_size))  )  /* data cross over than 1 page range */
		{
			data_len = ((ptr_dev_info_t->page_size) - addr_offset);			
		}
		else
		{
			data_len = remain_len;
		}		
		
#if	defined(TCSUPPORT_NAND_BMT) && !defined(LZMA_IMG)
		if(block_is_in_bmt_region(physical_block))
		{						
			memcpy(oob_buf + OOB_INDEX_OFFSET, &phy_block_bbt, OOB_INDEX_SIZE);
		}
		
       if(_SPI_NAND_TEST_FLAG == 0)		
       {
		rtn_status = spi_nand_write_page(page_number, addr_offset, &(ptr_buf[len - remain_len]), data_len, 0, &oob_buf[0], _SPI_NAND_OOB_SIZE , speed_mode);
       }
	   else
	   {
	   	rtn_status = SPI_NAND_FLASH_RTN_PROGRAM_FAIL;
	   }
					
		if(rtn_status != SPI_NAND_FLASH_RTN_NO_ERROR)
		{
		    _SPI_NAND_PRINTF("write fail at page: %d \n", page_number);
            if (update_bmt(page_number * (ptr_dev_info_t->page_size), UPDATE_WRITE_FAIL, &(ptr_buf[len - remain_len]), oob_buf))
            {            	
                _SPI_NAND_PRINTF("Update BMT success\n");
				rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;

            }
            else
            {
                _SPI_NAND_PRINTF("Update BMT fail\n");
				_SPI_NAND_SEMAPHORE_UNLOCK();	
                return -1;
            }		
		}
#else
		rtn_status = spi_nand_write_page(page_number, addr_offset, &(ptr_buf[len - remain_len]), data_len, 0, NULL, 0 , speed_mode);
#endif
		
		/* 8. Write remain data if neccessary */
		write_addr	+= data_len;
		remain_len  -= data_len;				
		ptr_rtn_len += data_len;			
		
	}
	
	_SPI_NAND_SEMAPHORE_UNLOCK();
	
	return (rtn_status);
}

static SPI_NAND_FLASH_RTN_T spi_nand_read_page_internal ( u32 page_number, 
																 SPI_NAND_FLASH_READ_SPEED_MODE_T speed_mode )
{
		u32 							logical_block, physical_block;
		struct SPI_NAND_FLASH_INFO_T	*ptr_dev_info_t;
		SPI_NAND_FLASH_RTN_T			rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;	
	
#if	defined(TCSUPPORT_NAND_BMT) && !defined(LZMA_IMG)
		unsigned short phy_block_bbt;
		u32 		   page_offset_in_block;
#endif					
		ptr_dev_info_t	= _SPI_NAND_GET_DEVICE_INFO_PTR;			
	
			
		_SPI_NAND_SEMAPHORE_LOCK(); 	
		
#if	defined(TCSUPPORT_NAND_BMT) && !defined(LZMA_IMG)
			page_offset_in_block = (((page_number * (ptr_dev_info_t->page_size))%(ptr_dev_info_t->erase_size))/ (ptr_dev_info_t->page_size));
			logical_block = ((page_number * (ptr_dev_info_t->page_size))/(ptr_dev_info_t->erase_size)) ;
			physical_block = get_mapping_block_index(logical_block, &phy_block_bbt);
	
			if( physical_block != logical_block)
			{			
				_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "Bad Block Mapping, from %d block to %d block\n", logical_block, physical_block);
			}

			page_number = (page_offset_in_block)+ ((physical_block*(ptr_dev_info_t->erase_size))/(ptr_dev_info_t->page_size));
#endif				
			
	
			spi_nand_read_page(page_number, speed_mode);
			
		_SPI_NAND_SEMAPHORE_UNLOCK();
						
		return (rtn_status);
	
}




/*------------------------------------------------------------------------------------
 * FUNCTION: static SPI_NAND_FLASH_RTN_T spi_nand_read_internal( u32     addr,
 *                                                               u32     len,
 *                                                               u8      *ptr_rtn_buf )
 * PURPOSE : To read flash internally.
 * AUTHOR  : Chuck Kuo
 * CALLED BY
 *   -
 * CALLS
 *   -
 * PARAMs  :
 *   INPUT : addr         - The addr variable of this function.
 *           len          - The len variable of this function.
 *   OUTPUT: ptr_rtn_buf  - A pointer to the ptr_rtn_buf variable.
 * RETURN  : SPI_RTN_NO_ERROR - Successful.   Otherwise - Failed.
 * NOTES   :
 * MODIFICTION HISTORY:
 * Date 2014/12/19 by Chuck Kuo - The first revision for this function.
 *
 *------------------------------------------------------------------------------------
 */
static SPI_NAND_FLASH_RTN_T spi_nand_read_internal ( unsigned long long addr, 
													 u32 len, 
													 u8 *ptr_rtn_buf, 
													 SPI_NAND_FLASH_READ_SPEED_MODE_T speed_mode )
{
	u32			 					page_number, data_offset;
	u32			 					read_addr, physical_read_addr, remain_len, logical_block, physical_block;
	struct SPI_NAND_FLASH_INFO_T	*ptr_dev_info_t;
	SPI_NAND_FLASH_RTN_T			rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;	

#if	defined(TCSUPPORT_NAND_BMT) && !defined(LZMA_IMG)
    unsigned short phy_block_bbt;
	unsigned long  addr_offset_in_block;
#endif
	
	if(  (0xbc000000 <= addr) && (addr<= 0xbfffffff) )		/* Reserver address area for system */
	{
		if( (addr & 0xbfc00000) == 0xbfc00000)
		{
			addr &= 0x003fffff;
		}
		else
		{
			addr &= 0x03ffffff;
		}
	}
		
	ptr_dev_info_t	= _SPI_NAND_GET_DEVICE_INFO_PTR;			
	read_addr  		= addr;
	remain_len 		= len;

	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "\nspi_nand_read_internal : addr=0x%lx, len=0x%x\n", addr, len );
		
	_SPI_NAND_SEMAPHORE_LOCK();	
	
	while(remain_len > 0)
	{
		physical_read_addr = read_addr;
	
#if	defined(TCSUPPORT_NAND_BMT) && !defined(LZMA_IMG)
		addr_offset_in_block = (read_addr %(ptr_dev_info_t->erase_size) );
		logical_block = (read_addr / (ptr_dev_info_t->erase_size));
		_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1,"read_addr=0x%x, erase size =0x%x, logical_block =0x%x\n", read_addr, (ptr_dev_info_t->erase_size), logical_block);
		physical_block = get_mapping_block_index(logical_block, &phy_block_bbt);
		physical_read_addr = (physical_block * (ptr_dev_info_t->erase_size))+ addr_offset_in_block;
		
		if( physical_block != logical_block)
		{			
			_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "Bad Block Mapping, from %d block to %d block\n", logical_block, physical_block);
		}
#endif		

		/* Caculate page number */
		data_offset = (physical_read_addr % (ptr_dev_info_t->page_size));
		page_number = (physical_read_addr / (ptr_dev_info_t->page_size));		

		_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "spi_nand_read_internal: read_addr=0x%x, page_number=0x%x, data_offset=0x%x\n", physical_read_addr, page_number, data_offset);

		spi_nand_read_page(page_number, speed_mode);
		
		/* 3. Retrieve the request data */
		if( (data_offset + remain_len) < _SPI_NAND_PAGE_SIZE ) 
			{				
			memcpy( &ptr_rtn_buf[len - remain_len], &_current_cache_page_data[data_offset], (sizeof(unsigned char)*remain_len) );
			remain_len =0;			
			
		}
		else			
		{
			memcpy( &ptr_rtn_buf[len - remain_len], &_current_cache_page_data[data_offset], (sizeof(unsigned char)*(_SPI_NAND_PAGE_SIZE- data_offset)));
			remain_len -= (_SPI_NAND_PAGE_SIZE- data_offset);
			read_addr += (_SPI_NAND_PAGE_SIZE- data_offset);
		}
	}			
	_SPI_NAND_SEMAPHORE_UNLOCK();
					
	return (rtn_status);
}
			

/*------------------------------------------------------------------------------------
 * FUNCTION: static void spi_nand_manufacute_init( struct SPI_NAND_FLASH_INFO_T *ptr_device_t )
 * PURPOSE : To init SPI NAND Flash chip
 * AUTHOR  : Chuck Kuo
 * CALLED BY
 *   -
 * CALLS
 *   -
 * PARAMs  :
 *   INPUT : None
 *   OUTPUT: None.
 * RETURN  : None.
 * NOTES   :
 * MODIFICTION HISTORY:
 * Date 2015/05/15 by Chuck Kuo - The first revision for this function.
 *
 *------------------------------------------------------------------------------------
 */
static void spi_nand_manufacute_init( struct SPI_NAND_FLASH_INFO_T *ptr_device_t )
{
	unsigned char feature;

	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1,"SPI NAND Chip Init : Unlock all block and Enable Quad Mode\n"); 

	if( ((ptr_device_t->mfr_id) == _SPI_NAND_MANUFACTURER_ID_GIGADEVICE) || \
		((ptr_device_t->mfr_id) == _SPI_NAND_MANUFACTURER_ID_MXIC) )
	{
		/* 1. Unlock All block */
		spi_nand_protocol_get_status_reg_1(&feature);
		feature &= 0xC1;
		spi_nand_protocol_set_status_reg_1(feature);

		spi_nand_protocol_get_status_reg_1(&feature);
		_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "After Unlock all block setup, the status register1 =0x%x\n", feature);					


		/* 2. Enable Qual mode */
		spi_nand_protocol_get_status_reg_2(&feature);
		feature |= 0x1;
		spi_nand_protocol_set_status_reg_2(feature);

		spi_nand_protocol_get_status_reg_2(&feature);
		_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "After enable qual mode setup, the status register2 =0x%x\n", feature);				
	}
	else if( (ptr_device_t->mfr_id) == _SPI_NAND_MANUFACTURER_ID_WINBOND)
	{			
		/* Enable to modify the status regsiter 1 */
		feature = 0x58;
		spi_nand_protocol_set_status_reg_2(feature);

		/* Unlock all block and Enable Qual mode */
		feature = 0x81;
		spi_nand_protocol_set_status_reg_1(feature);

		/* Disable to modify the status regsiter 1 */
		feature = 0x18;
		spi_nand_protocol_set_status_reg_2(feature);	

		spi_nand_protocol_get_status_reg_1(&feature);

		/* Unlock all block for Die_1 */
		if(((ptr_device_t->feature) & SPI_NAND_FLASH_DIE_SELECT_HAVE))
		{
			_die_id = 0x01;
			spi_nand_protocol_die_select(_die_id);

			/* Enable to modify the status regsiter 1 */
			feature = 0x58;
			spi_nand_protocol_set_status_reg_2(feature);

			/* Unlock all block and Enable Qual mode */
			feature = 0x81;
			spi_nand_protocol_set_status_reg_1(feature);

			/* Disable to modify the status regsiter 1 */
			feature = 0x18;
			spi_nand_protocol_set_status_reg_2(feature);

			spi_nand_protocol_get_status_reg_1(&feature);
		}

		_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "After Unlock all block setup, the status register1 =0x%x\n", feature);					
	}
	else if( (ptr_device_t->mfr_id) == _SPI_NAND_MANUFACTURER_ID_ESMT)
	{
		/* 1. Unlock All block */
		spi_nand_protocol_get_status_reg_1(&feature);
		feature &= 0xC1;
		spi_nand_protocol_set_status_reg_1(feature);

		spi_nand_protocol_get_status_reg_1(&feature);
		_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "After Unlock all block setup, the status register1 =0x%x\n", feature);						
	}
	else if( (ptr_device_t->mfr_id) == _SPI_NAND_MANUFACTURER_ID_ZENTEL) 
	{
		/* 1. Unlock All block */
		spi_nand_protocol_get_status_reg_1(&feature);
		feature &= 0xC7;
		spi_nand_protocol_set_status_reg_1(feature);

		spi_nand_protocol_get_status_reg_1(&feature);
		_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "After Unlock all block setup, the status register1 =0x%x\n", feature);						
	}
	else if( (ptr_device_t->mfr_id) == _SPI_NAND_MANUFACTURER_ID_ETRON)
	{
		/* 1. Unlock All block */
		spi_nand_protocol_get_status_reg_1(&feature);
		feature &= 0xC7;
		spi_nand_protocol_set_status_reg_1(feature);

		spi_nand_protocol_get_status_reg_1(&feature);
		_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "After Unlock all block setup, the status register1 =0x%x\n", feature);						

		/* 2. Enable Qual mode */
		spi_nand_protocol_get_status_reg_2(&feature);
		feature |= 0x1;
		spi_nand_protocol_set_status_reg_2(feature);

		spi_nand_protocol_get_status_reg_2(&feature);
		_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "After enable qual mode setup, the status register2 =0x%x\n", feature);						
	}	
	else if( (ptr_device_t->mfr_id) == _SPI_NAND_MANUFACTURER_ID_TOSHIBA)
	{
		/* 1. Unlock All block */
		spi_nand_protocol_get_status_reg_1(&feature);
		feature &= 0xC7;
		spi_nand_protocol_set_status_reg_1(feature);

		spi_nand_protocol_get_status_reg_1(&feature);
		_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1,"After Unlock all block setup, the status register1 =0x%x\n", feature);												
	}

	else if( (ptr_device_t->mfr_id) == _SPI_NAND_MANUFACTURER_ID_MICRON)
	{
		/* 1. Unlock All block */
		spi_nand_protocol_get_status_reg_1(&feature);
		feature &= 0x83;
		spi_nand_protocol_set_status_reg_1(feature);

		spi_nand_protocol_get_status_reg_1(&feature);
		_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1,"After Unlock all block setup, the status register1 =0x%x\n", feature);												
	}


	
	else
	{
		/* 1. Unlock All block */
		spi_nand_protocol_get_status_reg_1(&feature);
		feature &= 0xC1;
		spi_nand_protocol_set_status_reg_1(feature);

		spi_nand_protocol_get_status_reg_1(&feature);
		_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "After Unlock all block setup, the status register1 =0x%x\n", feature);					


		/* 2. Enable Qual mode */
		spi_nand_protocol_get_status_reg_2(&feature);
		feature |= 0x1;
		spi_nand_protocol_set_status_reg_2(feature);

		spi_nand_protocol_get_status_reg_2(&feature);
		_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "After enable qual mode setup, the status register2 =0x%x\n", feature);		
	}
	
}


/*------------------------------------------------------------------------------------
 * FUNCTION: static SPI_NAND_FLASH_RTN_T spi_nand_probe( struct SPI_NAND_FLASH_INFO_T  *ptr_rtn_device_t )
 * PURPOSE : To probe SPI NAND flash id.
 * AUTHOR  : Chuck Kuo
 * CALLED BY
 *   -
 * CALLS
 *   -
 * PARAMs  :
 *   INPUT : None
 *   OUTPUT: rtn_index  - The rtn_index variable of this function.
 * RETURN  : SPI_RTN_NO_ERROR - Successful.   Otherwise - Failed.
 * NOTES   :
 * MODIFICTION HISTORY:
 * Date 2014/12/12 by Chuck Kuo - The first revision for this function.
 *
 *------------------------------------------------------------------------------------
 */
static SPI_NAND_FLASH_RTN_T spi_nand_probe( struct SPI_NAND_FLASH_INFO_T * ptr_rtn_device_t )
{
	u32 					i=0;
	SPI_NAND_FLASH_RTN_T	rtn_status = SPI_NAND_FLASH_RTN_PROBE_ERROR;

	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "spi_nand_probe: start \n");
	

	/* Protocol for read id */
	_SPI_NAND_SEMAPHORE_LOCK();	
	spi_nand_protocol_read_id( (struct SPI_NAND_FLASH_INFO_T *)ptr_rtn_device_t );
	_SPI_NAND_SEMAPHORE_UNLOCK();	
	
	_SPI_NAND_PRINTF("spi_nand_probe: mfr_id=0x%x, dev_id=0x%x\n", ptr_rtn_device_t->mfr_id, ptr_rtn_device_t->dev_id);

	for ( i=0 ; i < (sizeof(spi_nand_flash_tables)/sizeof(struct SPI_NAND_FLASH_INFO_T)) ; i++) 
	{
		_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1,"spi_nand_probe: table[%d]: mfr_id=0x%x, dev_id=0x%x\n", i, spi_nand_flash_tables[i].mfr_id, spi_nand_flash_tables[i].dev_id );
		
		if ( ( (ptr_rtn_device_t->mfr_id) == spi_nand_flash_tables[i].mfr_id) &&
		     ( (ptr_rtn_device_t->dev_id) == spi_nand_flash_tables[i].dev_id)  ) 
		{
			ptr_rtn_device_t->device_size = spi_nand_flash_tables[i].device_size;
			ptr_rtn_device_t->erase_size  = spi_nand_flash_tables[i].erase_size;
			ptr_rtn_device_t->page_size	  = spi_nand_flash_tables[i].page_size;
			ptr_rtn_device_t->oob_size	  = spi_nand_flash_tables[i].oob_size;
			ptr_rtn_device_t->dummy_mode  = spi_nand_flash_tables[i].dummy_mode;
			ptr_rtn_device_t->read_mode	  = spi_nand_flash_tables[i].read_mode;
			memcpy( &(ptr_rtn_device_t->ptr_name) , &(spi_nand_flash_tables[i].ptr_name), sizeof(ptr_rtn_device_t->ptr_name));
			memcpy( &(ptr_rtn_device_t->oob_free_layout) , &(spi_nand_flash_tables[i].oob_free_layout), sizeof(ptr_rtn_device_t->oob_free_layout));
			ptr_rtn_device_t->feature = spi_nand_flash_tables[i].feature;
			
			rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;
			break;
		}
	}
	
	if ( rtn_status != SPI_NAND_FLASH_RTN_NO_ERROR )
	{
		/* Another protocol for read id  (For example, the GigaDevice SPI NADN chip for Type C */
		_SPI_NAND_SEMAPHORE_LOCK();	
		spi_nand_protocol_read_id_2( (struct SPI_NAND_FLASH_INFO_T *)ptr_rtn_device_t );
		_SPI_NAND_SEMAPHORE_UNLOCK();	
		
		_SPI_NAND_PRINTF("spi_nand_probe: mfr_id=0x%x, dev_id=0x%x\n", ptr_rtn_device_t->mfr_id, ptr_rtn_device_t->dev_id);

		for ( i=0 ; i < (sizeof(spi_nand_flash_tables)/sizeof(struct SPI_NAND_FLASH_INFO_T)) ; i++) 
		{
			_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1,"spi_nand_probe: table[%d]: mfr_id=0x%x, dev_id=0x%x\n", i, spi_nand_flash_tables[i].mfr_id, spi_nand_flash_tables[i].dev_id );
			
			if ( ( (ptr_rtn_device_t->mfr_id) == spi_nand_flash_tables[i].mfr_id) &&
			     ( (ptr_rtn_device_t->dev_id) == spi_nand_flash_tables[i].dev_id)  ) 
			{
				ptr_rtn_device_t->device_size = spi_nand_flash_tables[i].device_size;
				ptr_rtn_device_t->erase_size  = spi_nand_flash_tables[i].erase_size;
				ptr_rtn_device_t->page_size	  = spi_nand_flash_tables[i].page_size;
				ptr_rtn_device_t->oob_size	  = spi_nand_flash_tables[i].oob_size;
				ptr_rtn_device_t->dummy_mode  = spi_nand_flash_tables[i].dummy_mode;
				ptr_rtn_device_t->read_mode	  = spi_nand_flash_tables[i].read_mode;
				memcpy( &(ptr_rtn_device_t->ptr_name) , &(spi_nand_flash_tables[i].ptr_name), sizeof(ptr_rtn_device_t->ptr_name));
				memcpy( &(ptr_rtn_device_t->oob_free_layout) , &(spi_nand_flash_tables[i].oob_free_layout), sizeof(ptr_rtn_device_t->oob_free_layout));
				ptr_rtn_device_t->feature = spi_nand_flash_tables[i].feature;
				
				rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;
				break;
			}
		}
	}
	

	if(rtn_status == SPI_NAND_FLASH_RTN_NO_ERROR)
	{
		_SPI_NAND_SEMAPHORE_LOCK();
		spi_nand_manufacute_init(ptr_rtn_device_t);		
		_SPI_NAND_SEMAPHORE_UNLOCK();	
	}	
	
	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "spi_nand_probe: end \n");
	
	return (rtn_status);
}

/* EXPORTED SUBPROGRAM BODIES -------------------------------------------------------- */


u32 reservearea_size = 0;

#if	defined(TCSUPPORT_NAND_BMT) && !defined(LZMA_IMG)
#define BMT_BAD_BLOCK_INDEX_OFFSET (1)
#define POOL_GOOD_BLOCK_PERCENT 8/100
#define MAX_BMT_SIZE_PERCENTAGE 1/10

extern int nand_flash_avalable_size;


int en7512_nand_exec_read_page(u32 page, u8* date, u8* oob)
{ 

	if( spi_nand_read_page(page, SPI_NAND_FLASH_READ_SPEED_MODE_SINGLE) == SPI_NAND_FLASH_RTN_NO_ERROR)
	{
		/* Get data segment and oob segment  */
		memcpy( date, &_current_cache_page_data[0], _SPI_NAND_PAGE_SIZE );
		memcpy( oob,  &_current_cache_page_oob_mapping[0], _SPI_NAND_OOB_SIZE );		

		return 0;
	}
	else
	{
		 _SPI_NAND_PRINTF( "en7512_nand_exec_read_page: Read Error\n");
		return -1;
	}
	
}
int en7512_nand_check_block_bad(u32 offset, u32 bmt_block)
{

	 u32							 page_number;
	 struct SPI_NAND_FLASH_INFO_T	 *ptr_dev_info_t;
	 
	 ptr_dev_info_t  = _SPI_NAND_GET_DEVICE_INFO_PTR;	

	if(  (0xbc000000 <= offset) && (offset<= 0xbfffffff) )		/* Reserver address area for system */
	{
		if( (offset & 0xbfc00000) == 0xbfc00000)
		{
			offset &= 0x003fffff;
		}
		else
		{
			offset &= 0x03ffffff;
		}
	}	 

	 /* Caculate page number */
	 page_number = (offset / (ptr_dev_info_t->page_size));		 
			 
	 _SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "en7512_nand_check_block_bad: read_addr=0x%x, page_number=0x%x\n", offset, page_number);
	 
	 spi_nand_read_page(page_number, SPI_NAND_FLASH_READ_SPEED_MODE_SINGLE);
	 
	
	 if(bmt_block){
		 if(_current_cache_page_oob_mapping[BMT_BAD_BLOCK_INDEX_OFFSET] != 0xff){
			 _SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "Bad block detected at page_addr 0x%x, oob_buf[%d] is 0x%x\n", page_number, BMT_BAD_BLOCK_INDEX_OFFSET,_current_cache_page_oob_mapping[BMT_BAD_BLOCK_INDEX_OFFSET]);
			 return 1;
		 }
	 }
	 else
	 {
		 if(_current_cache_page_oob_mapping[0] != 0xff)
		 {
			 _SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "Bad block detected at page_addr 0x%x, oob_buf[0] is 0x%x\n", page_number, _current_cache_page_oob_mapping[0]);
			 return 1;
		 }
	 }


	return 0;  /* Good Block*/

}
int en7512_nand_erase(u32 offset)
{

	struct SPI_NAND_FLASH_INFO_T	*ptr_dev_info_t;
	
	ptr_dev_info_t	= _SPI_NAND_GET_DEVICE_INFO_PTR;   

	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "en7512_nand_erase: offset =0x%x, erase_size=0x%x\n", offset, (ptr_dev_info_t->erase_size));

	if(spi_nand_erase_block( (offset / (ptr_dev_info_t->erase_size)) )== SPI_NAND_FLASH_RTN_NO_ERROR)
	{
		return 0;		
	}
	else
	{
		_SPI_NAND_PRINTF("en7512_nand_erase : Fail \n");
		return -1;
	}
}
int en7512_nand_mark_badblock(u32 offset, u32 bmt_block)
{

	u32 							page_number;
	u8 buf[8];
	struct SPI_NAND_FLASH_INFO_T	*ptr_dev_info_t;
	SPI_NAND_FLASH_RTN_T			rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;

	 ptr_dev_info_t  = _SPI_NAND_GET_DEVICE_INFO_PTR;	

	 /* Caculate page number */
	 page_number = (offset / (ptr_dev_info_t->page_size));		 


	memset(buf, 0xFF, 8);
	if(bmt_block)
	{
		buf[BMT_BAD_BLOCK_INDEX_OFFSET] = 0;
	}
	else
	{
		buf[0] = 0;
	}

	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_2, "en7512_nand_mark_badblock: buf info:\n");
	_SPI_NAND_DEBUG_PRINTF_ARRAY(SPI_NAND_FLASH_DEBUG_LEVEL_2, &buf[0], 8);

	_SPI_NAND_PRINTF("en7512_nand_mark_badblock: page_num=0x%x\n", page_number);

	rtn_status = spi_nand_write_page(page_number, 0, NULL, 0, 0, &buf[0], 8 , SPI_NAND_FLASH_WRITE_SPEED_MODE_SINGLE);
	

	if( rtn_status == SPI_NAND_FLASH_RTN_NO_ERROR)
	{
		return 0;
	}
	else
	{
		return -1;
	}


}
int en7512_nand_exec_write_page(u32 page, u8 *dat, u8 *oob)
{

	SPI_NAND_FLASH_RTN_T			rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;

	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "en7512_nand_exec_write_page: page=0x%x\n", page);
	
	rtn_status = spi_nand_write_page(page, 0, dat, _SPI_NAND_PAGE_SIZE, 0, oob, _SPI_NAND_OOB_SIZE , SPI_NAND_FLASH_WRITE_SPEED_MODE_SINGLE);

	if( rtn_status == SPI_NAND_FLASH_RTN_NO_ERROR)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}


u32 maximum_bmt_block_count = 0;

int calc_bmt_pool_size(struct mtd_info *mtd)
{
	struct nand_chip *nand = mtd->priv;
    int chip_size = nand->chipsize;
    int block_size = 1 << nand->phys_erase_shift;
    int total_block = chip_size / block_size;
    int last_block = total_block - 1;
    
    u16 valid_block_num = 0;
    u16 need_valid_block_num = total_block * POOL_GOOD_BLOCK_PERCENT;


	_SPI_NAND_PRINTF("calc_bmt_pool_size : need_valid_block_num=0x%x, total_block=0x%x\n",need_valid_block_num, total_block);

	maximum_bmt_block_count = total_block * MAX_BMT_SIZE_PERCENTAGE;

#if defined(TCSUPPORT_CT_PON)
	maximum_bmt_block_count = BMT_SIZE_FOR_RESERVE_AREA;

	if( need_valid_block_num > BMT_SIZE_FOR_RESERVE_AREA)
	{
		need_valid_block_num = BMT_SIZE_FOR_RESERVE_AREA;	
	}
#endif

	
    for(;last_block > 0; --last_block)
    {
        if(en7512_nand_check_block_bad(last_block * block_size, BAD_BLOCK_RAW))
        {
            continue;
    
        }
        else
        {
            valid_block_num++;
            if(valid_block_num == need_valid_block_num)
            {
                break;
            }

        }

    }

    return (total_block - last_block);
    
}



#endif



/*------------------------------------------------------------------------------------
 * FUNCTION: SPI_NAND_FLASH_RTN_T SPI_NAND_Flash_Init( long  rom_base )
 * PURPOSE : To provide interface for SPI NAND init.
 * AUTHOR  : Chuck Kuo
 * CALLED BY
 *   -
 * CALLS
 *   -
 * PARAMs  :
 *   INPUT : rom_base - The rom_base variable of this function.
 *   OUTPUT: None
 * RETURN  : SPI_RTN_NO_ERROR - Successful.   Otherwise - Failed.
 * NOTES   :
 * MODIFICTION HISTORY:
 * Date 2014/12/12 by Chuck Kuo - The first revision for this function.
 *
 *------------------------------------------------------------------------------------
 */
SPI_NAND_FLASH_RTN_T SPI_NAND_Flash_Init(u32 rom_base)
{
	unsigned long val;
	
	SPI_NAND_FLASH_RTN_T	rtn_status = SPI_NAND_FLASH_RTN_PROBE_ERROR;	
	
	/* 1. set SFC Clock to 25MHZ */
	val = VPint(_SPI_FREQUENCY_ADJUST_REG);
	val |= 0xa01;
	VPint(_SPI_FREQUENCY_ADJUST_REG) = val;
	
	/* 2. Enable Manual Mode */
	_SPI_NAND_ENABLE_MANUAL_MODE();	
	
	_SPI_NAND_PRINTF("SPI_NAND_Flash_init: before init, mtx_mode_tog=0xl%x, manaul_en=0x%lx\n", VPint(0xbfa10014), VPint(0xbfa10020) );
	
 	/* 3. Probe flash information */
	if ( spi_nand_probe(  &_current_flash_info_t) != SPI_NAND_FLASH_RTN_NO_ERROR )
	{
		_SPI_NAND_PRINTF("SPI NAND Flash Detected Error !\n");
	}
	else
	{		
		_SPI_NAND_PRINTF("Dected SPI NAND Flash : %s, Flash Size=0x%x\n", _current_flash_info_t.ptr_name,  _current_flash_info_t.device_size);
		rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;
	}

   return (rtn_status);
}


/*------------------------------------------------------------------------------------
 * FUNCTION: SPI_NAND_FLASH_RTN_T SPI_NAND_Flash_Get_Flash_Info( struct SPI_NAND_FLASH_INFO_T    *ptr_rtn_into_t )
 * PURPOSE : To get system current flash info.
 * AUTHOR  : Chuck Kuo
 * CALLED BY
 *   -
 * CALLS
 *   -
 * PARAMs  :
 *   INPUT : None
 *   OUTPUT: ptr_rtn_into_t  - A pointer to the structure of the ptr_rtn_into_t variable.
 * RETURN  : SPI_RTN_NO_ERROR - Successful.   Otherwise - Failed.
 * NOTES   :
 * MODIFICTION HISTORY:
 * Date 2015/01/14 by Chuck Kuo - The first revision for this function.
 *
 *------------------------------------------------------------------------------------
 */
SPI_NAND_FLASH_RTN_T SPI_NAND_Flash_Get_Flash_Info( struct SPI_NAND_FLASH_INFO_T *ptr_rtn_into_t)
{
	struct SPI_NAND_FLASH_INFO_T	*ptr_dev_info_t;
	SPI_NAND_FLASH_RTN_T			rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;
		
		
	ptr_dev_info_t	= _SPI_NAND_GET_DEVICE_INFO_PTR;
	
	memcpy( ptr_rtn_into_t, ptr_dev_info_t, sizeof(struct SPI_NAND_FLASH_INFO_T) );
	
	return (rtn_status);	
}

/*------------------------------------------------------------------------------------
 * FUNCTION: SPI_NAND_FLASH_RTN_T SPI_NAND_Flash_Write_Nbyte( u32    dst_addr,
 *                                                            u32    len,
 *                                                            u32    *ptr_rtn_len,
 *                                                            u8*    ptr_buf      )
 * PURPOSE : To provide interface for Write N Bytes into SPI NAND Flash.
 * AUTHOR  : Chuck Kuo
 * CALLED BY
 *   -
 * CALLS
 *   -
 * PARAMs  :
 *   INPUT : dst_addr - The dst_addr variable of this function.
 *           len      - The len variable of this function.
 *           buf      - The buf variable of this function.
 *   OUTPUT: rtn_len  - The rtn_len variable of this function.
 * RETURN  : SPI_RTN_NO_ERROR - Successful.   Otherwise - Failed.
 * NOTES   :
 * MODIFICTION HISTORY:
 * Date 2014/12/15 by Chuck Kuo - The first revision for this function.
 *
 *------------------------------------------------------------------------------------
 */
SPI_NAND_FLASH_RTN_T SPI_NAND_Flash_Write_Nbyte( u32								dst_addr, 
												 u32								len, 
												 u32								*ptr_rtn_len, 
												 u8									*ptr_buf, 
												 SPI_NAND_FLASH_WRITE_SPEED_MODE_T	speed_node )
{		
	SPI_NAND_FLASH_RTN_T	rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;	
	
	rtn_status =  spi_nand_write_internal(dst_addr, len, ptr_rtn_len, ptr_buf, speed_node);
	
	*ptr_rtn_len = len ;  /* Chuck Kuo, tmp modify */
	
	return (rtn_status);
}

/*------------------------------------------------------------------------------------
 * FUNCTION: int SPI_NAND_Flash_Read_NByte( long     addr,
 *                                          long     len,
 *                                          long     *retlen,
 *                                          char     *buf    )
 * PURPOSE : To provide interface for Read N Bytes from SPI NAND Flash.
 * AUTHOR  : Chuck Kuo
 * CALLED BY
 *   -
 * CALLS
 *   -
 * PARAMs  :
 *   INPUT : addr    - The addr variable of this function.
 *           len     - The len variable of this function.
 *           retlen  - The retlen variable of this function.
 *           buf     - The buf variable of this function.
 *   OUTPUT: None
 * RETURN  : SPI_RTN_NO_ERROR - Successful.   Otherwise - Failed.
 * NOTES   :
 * MODIFICTION HISTORY:
 * Date 2014/12/12 by Chuck Kuo - The first revision for this function.
 *
 *------------------------------------------------------------------------------------
 */
u32 SPI_NAND_Flash_Read_NByte(	unsigned long long				 addr, 
								u32								 len, 
								u32								 *retlen, 
								u8								 *buf, 
								SPI_NAND_FLASH_READ_SPEED_MODE_T speed_mode)
{
	
	return spi_nand_read_internal(addr, len, buf, speed_mode);	
}

/*------------------------------------------------------------------------------------
 * FUNCTION: SPI_NAND_FLASH_RTN_T SPI_NAND_Flash_Erase( u32  dst_addr,
 *                                                      u32  len      )
 * PURPOSE : To provide interface for Erase SPI NAND Flash.
 * AUTHOR  : Chuck Kuo
 * CALLED BY
 *   -
 * CALLS
 *   -
 * PARAMs  :
 *   INPUT : dst_addr - The dst_addr variable of this function.
 *           len      - The len variable of this function.
 *   OUTPUT: None
 * RETURN  : SPI_RTN_NO_ERROR - Successful.   Otherwise - Failed.
 * NOTES   :
 * MODIFICTION HISTORY:
 * Date 2014/12/17 by Chuck Kuo - The first revision for this function.
 *
 *------------------------------------------------------------------------------------
 */
SPI_NAND_FLASH_RTN_T SPI_NAND_Flash_Erase(	u32 dst_addr, 
											u32 len )
{
	SPI_NAND_FLASH_RTN_T	rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;
	
	rtn_status =  spi_nand_erase_internal(dst_addr, len);
	
	return (rtn_status);
}

/*------------------------------------------------------------------------------------
 * FUNCTION: char SPI_NAND_Flash_Read_Byte( long     addr )
 * PURPOSE : To provide interface for read 1 Bytes from SPI NAND Flash.
 * AUTHOR  : Chuck Kuo
 * CALLED BY
 *   -
 * CALLS
 *   -
 * PARAMs  :
 *   INPUT : addr - The addr variable of this function.
 *   OUTPUT: None
 * RETURN  : SPI_RTN_NO_ERROR - Successful.   Otherwise - Failed.
 * NOTES   :
 * MODIFICTION HISTORY:
 * Date 2014/12/12 by Chuck Kuo - The first revision for this function.
 *
 *------------------------------------------------------------------------------------
 */
unsigned char SPI_NAND_Flash_Read_Byte(unsigned long long addr)
{
	unsigned char ch;
	size_t retlen;
	int ret=0;
	
	ret = spi_nand_mtd->read(spi_nand_mtd, (loff_t)addr, 1, &retlen, &ch);
	if(ret != 0)
	{
		_SPI_NAND_PRINTF("Error : SPI_NAND_Flash_Read_Byte , read 0x%llx error\n", addr);
		return -1;
	}
	
	return ch;
	
}

/*------------------------------------------------------------------------------------
 * FUNCTION: long SPI_NAND_Flash_Read_DWord( long    addr )
 * PURPOSE : To provide interface for read Double Word from SPI NAND Flash.
 * AUTHOR  : Chuck Kuo
 * CALLED BY
 *   -
 * CALLS
 *   -
 * PARAMs  :
 *   INPUT : addr - The addr variable of this function.
 *   OUTPUT: None
 * RETURN  : SPI_RTN_NO_ERROR - Successful.   Otherwise - Failed.
 * NOTES   :
 * MODIFICTION HISTORY:
 * Date 2014/12/12 by Chuck Kuo - The first revision for this function.
 *
 *------------------------------------------------------------------------------------
 */
unsigned long SPI_NAND_Flash_Read_DWord(unsigned long long addr)
{

	unsigned char data[5];
	unsigned long dword;
	size_t retlen;
	int ret = 0, i;
	
	ret = spi_nand_mtd->read( spi_nand_mtd, (loff_t)addr, 4, &retlen, data);

	if (ret != 0)
	{
		_SPI_NAND_PRINTF("Error : SPI_NAND_Flash_Read_DWord , read 0x%llx error\n", addr);
		return -1;
	}

	dword = 0;	
	for (i=0; i<4; i++) {
		dword += (unsigned long)data[i];
		if (i<3)
			dword <<= 8;
	}
	return dword;	
		

}

/*------------------------------------------------------------------------------------
 * FUNCTION: void SPI_NAND_DEBUG_ENABLE( void )
 * PURPOSE : To enable to printf debug message of SPI NAND driver.
 * AUTHOR  : Chuck Kuo
 * CALLED BY
 *   -
 * CALLS
 *   -
 * PARAMs  :
 *   INPUT : None
 *   OUTPUT: None
 * RETURN  : SPI_RTN_NO_ERROR - Successful.   Otherwise - Failed.
 * NOTES   :
 * MODIFICTION HISTORY:
 * Date 2015/01/20 by Chuck Kuo - The first revision for this function.
 *
 *------------------------------------------------------------------------------------
 */
void SPI_NAND_DEBUG_ENABLE( void )
{	
	_SPI_NAND_DEBUG_FLAG = 1;	
}

/*------------------------------------------------------------------------------------
 * FUNCTION: void SPI_NAND_DEBUG_DISABLE( void )
 * PURPOSE : To disable to printf debug message of SPI NAND driver.
 * AUTHOR  : Chuck Kuo
 * CALLED BY
 *   -
 * CALLS
 *   -
 * PARAMs  :
 *   INPUT : None
 *   OUTPUT: None
 * RETURN  : SPI_RTN_NO_ERROR - Successful.   Otherwise - Failed.
 * NOTES   :
 * MODIFICTION HISTORY:
 * Date 2015/01/20 by Chuck Kuo - The first revision for this function.
 *
 *------------------------------------------------------------------------------------
 */
void SPI_NAND_DEBUG_DISABLE( void )
{	
	_SPI_NAND_DEBUG_FLAG = 0;	
}

/*------------------------------------------------------------------------------------
 * FUNCTION: void SPI_NAND_Flash_Clear_Read_Cache_Data( void )
 * PURPOSE : To clear the cache data for read. 
 *           (The next time to read data will get data from flash chip certainly.)
 * AUTHOR  : Chuck Kuo
 * CALLED BY
 *   -
 * CALLS
 *   -
 * PARAMs  :
 *   INPUT : None
 *   OUTPUT: None
 * RETURN  : SPI_RTN_NO_ERROR - Successful.   Otherwise - Failed.
 * NOTES   :
 * MODIFICTION HISTORY:
 * Date 2015/01/21 by Chuck Kuo - The first revision for this function.
 *
 *------------------------------------------------------------------------------------
 */
void SPI_NAND_Flash_Clear_Read_Cache_Data( void )
{
	_current_page_num	= 0xFFFFFFFF;
}


/***********************************************************************************/
/***********************************************************************************/
/***** Modify for SPI NAND linux kernel driver below *******************************/
/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/


/* NAND driver */
int __devinit ra_nand_init(void)
{
	return 0;
}

void __devinit ra_nand_remove(void)
{

}


/*****************************************************************************************/
/*****************************************************************************************/
/* Porting */
/*****************************************************************************************/
/*****************************************************************************************/

#define LINUX_DEBUG_FLAG				0

/* feature/ status reg */
#define REG_BLOCK_LOCK                  0xa0
#define REG_OTP                         0xb0
#define REG_STATUS                      0xc0/* timing */

/* status */
#define STATUS_OIP_MASK                 0x01
#define STATUS_READY                    (0 << 0)
#define STATUS_BUSY                     (1 << 0)

#define STATUS_E_FAIL_MASK              0x04
#define STATUS_E_FAIL                   (1 << 2)

#define STATUS_P_FAIL_MASK              0x08
#define STATUS_P_FAIL                   (1 << 3)

#define STATUS_ECC_MASK                 0x30
#define STATUS_ECC_1BIT_CORRECTED       (1 << 4)
#define STATUS_ECC_ERROR                (2 << 4)
#define STATUS_ECC_RESERVED             (3 << 4)

/*ECC enable defines*/
#define OTP_ECC_MASK                    0x10
#define OTP_ECC_OFF                     0
#define OTP_ECC_ON                      1

#define ECC_DISABLED
#define ECC_IN_NAND
#define ECC_SOFT

#define SPI_NAND_PROCNAME				"driver/spi_nand_debug"




#define BUFSIZE (2 * 2048)
#define CACHE_BUF 2112


#define CONFIG_MTD_SPINAND_ONDIEECC		1


#ifdef CONFIG_MTD_SPINAND_ONDIEECC
static int enable_hw_ecc;
static int enable_read_hw_ecc;

#if 0
static struct nand_ecclayout spinand_oob_64 = {
        .eccbytes = 16,
        .eccpos = {
                12,13,14,15,
                28,29,30,31,
                44,45,46,47,
                60,61,62,63 },
        .oobavail = 48,
        .oobfree = {
                {.offset = 2,
                        .length = 10},
                {.offset = 16,
                        .length = 12},
                {.offset = 32,
                        .length = 12},
                {.offset = 48,
                        .length = 12},
        }
};
#endif
static struct nand_ecclayout spinand_oob_64 = {
        .eccbytes = 0,
        .eccpos = {},
        .oobavail = MAX_LINUX_USE_OOB_SIZE,
        .oobfree = {
                {.offset = 0, 
				 .length = MAX_LINUX_USE_OOB_SIZE}
        }
};


#endif

static inline struct spinand_state *mtd_to_state(struct mtd_info *mtd)
{
        struct nand_chip *chip = (struct nand_chip *)mtd->priv;
        struct spinand_info *info = (struct spinand_info *)chip->priv;
        struct spinand_state *state = (struct spinand_state *)info->priv;

        return state;
}


/*
 * spinand_read_id- Read SPI Nand ID
 * Description:
 *    Read ID: read two ID bytes from the SPI Nand device
 */
static int spinand_read_id(struct spi_device *spi_nand, u8 *id)
{
	struct _SPI_NAND_FLASH_ID_T flash_id;
	struct SPI_NAND_FLASH_INFO_T	*ptr_dev_info_t;
	ptr_dev_info_t		= 	_SPI_NAND_GET_DEVICE_INFO_PTR;	


	(id[0])= ptr_dev_info_t->mfr_id;
	(id[1])= ptr_dev_info_t->dev_id;

#if 0
	_SPI_NAND_SEMAPHORE_LOCK();
	spi_nand_protocol_read_id(&flash_id);
	_SPI_NAND_SEMAPHORE_UNLOCK();
	
	(id[0])= flash_id.mfr_id;
	(id[1])= flash_id.dev_id;
#endif	
	
	return 0;
}

/*
 * spinand_read_status- send command 0xf to the SPI Nand status register
 * Description:
 *    After read, write, or erase, the Nand device is expected to set the
 *    busy status.
 *    This function is to allow reading the status of the command: read,
 *    write, and erase.
 *    Once the status turns to be ready, the other status bits also are
 *    valid status bits.
 */
static int spinand_read_status(struct spi_device *spi_nand, uint8_t *status)
{
		return spi_nand_protocol_get_status_reg_3(status);
}

#define MAX_WAIT_JIFFIES  (40 * HZ)
static int wait_till_ready(struct spi_device *spi_nand)
{
        unsigned long deadline;
        int retval;
        u8 stat = 0;

        deadline = jiffies + MAX_WAIT_JIFFIES;
        do {
                retval = spinand_read_status(spi_nand, &stat);
                if (retval < 0)
                        return -1;
                else if (!(stat & 0x1))
                        break;

                cond_resched();
        } while (!time_after_eq(jiffies, deadline));

        if ((stat & 0x1) == 0)
                return 0;

        return -1;
}
/**
 * spinand_get_otp- send command 0xf to read the SPI Nand OTP register
 * Description:
 *   There is one bit( bit 0x10 ) to set or to clear the internal ECC.
 *   Enable chip internal ECC, set the bit to 1
 *   Disable chip internal ECC, clear the bit to 0
 */
static int spinand_get_otp(struct spi_device *spi_nand, u8 *otp)
{
	return spi_nand_protocol_get_status_reg_2(otp);
}

/**
 * spinand_set_otp- send command 0x1f to write the SPI Nand OTP register
 * Description:
 *   There is one bit( bit 0x10 ) to set or to clear the internal ECC.
 *   Enable chip internal ECC, set the bit to 1
 *   Disable chip internal ECC, clear the bit to 0
 */
static int spinand_set_otp(struct spi_device *spi_nand, u8 *otp)
{
	return spi_nand_protocol_set_status_reg_2(*otp);
}

#ifdef CONFIG_MTD_SPINAND_ONDIEECC
/**
 * spinand_enable_ecc- send command 0x1f to write the SPI Nand OTP register
 * Description:
 *   There is one bit( bit 0x10 ) to set or to clear the internal ECC.
 *   Enable chip internal ECC, set the bit to 1
 *   Disable chip internal ECC, clear the bit to 0
 */
static int spinand_enable_ecc(struct spi_device *spi_nand)
{
        int retval;
        u8 otp = 0;

        retval = spinand_get_otp(spi_nand, &otp);
        if (retval < 0)
                return retval;

        if ((otp & OTP_ECC_MASK) == OTP_ECC_MASK)
                return 0;
        otp |= OTP_ECC_MASK;
        retval = spinand_set_otp(spi_nand, &otp);
        if (retval < 0)
                return retval;
        return spinand_get_otp(spi_nand, &otp);
}
#endif

static int spinand_disable_ecc(struct spi_device *spi_nand)
{
        int retval;
        u8 otp = 0;

        retval = spinand_get_otp(spi_nand, &otp);
        if (retval < 0)
                return retval;

        if ((otp & OTP_ECC_MASK) == OTP_ECC_MASK) {
                otp &= ~OTP_ECC_MASK;
                retval = spinand_set_otp(spi_nand, &otp);
                if (retval < 0)
                        return retval;
                return spinand_get_otp(spi_nand, &otp);
        }
        return 0;
}

/**
 * spinand_write_enable- send command 0x06 to enable write or erase the
 * Nand cells
 * Description:
 *   Before write and erase the Nand cells, the write enable has to be set.
 *   After the write or erase, the write enable bit is automatically
 *   cleared (status register bit 2)
 *   Set the bit 2 of the status register has the same effect
 */
static int spinand_write_enable(struct spi_device *spi_nand)
{
        return spi_nand_protocol_write_enable();
}

static int spinand_read_page_to_cache(struct spi_device *spi_nand, u32 page_id)
{
	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "spinand_read_page_to_cache: page_idx=0x%x\n", page_id);
	return spi_nand_protocol_page_read( (u32)page_id);
}

/*
 * spinand_read_from_cache- send command 0x03 to read out the data from the
 * cache register(2112 bytes max)
 * Description:
 *   The read can specify 1 to 2112 bytes of data read at the corresponding
 *   locations.
 *   No tRd delay.
 */
static int spinand_read_from_cache(struct spi_device *spi_nand, u32 page_id,
                u32 byte_id, u32 len, u8 *rbuf)
{
	
	 unsigned int ret;
	 u8			status;
		
      spi_nand_protocol_read_from_cache_single_speed(byte_id, len, rbuf, SPI_NAND_FLASH_READ_DUMMY_BYTE_APPEND);
      
        while (1) {
                ret = spinand_read_status(spi_nand, &status);
                if (ret < 0) {
                              _SPI_NAND_PRINTF("err %d read status register\n", ret);
                        return ret;
                }

                if ((status & STATUS_OIP_MASK) == STATUS_READY) {
                        break;
                }
        }            

	  return 0;  /* Chuck , tmp return success any way */
}

/*
 * spinand_read_page-to read a page with:
 * @page_id: the physical page number
 * @offset:  the location from 0 to 2111
 * @len:     number of bytes to read
 * @rbuf:    read buffer to hold @len bytes
 *
 * Description:
 *   The read includes two commands to the Nand: 0x13 and 0x03 commands
 *   Poll to read status to wait for tRD time.
 */
static int spinand_read_page(struct spi_device *spi_nand, u32 page_id,
                u32 offset, u32 len, u8 *rbuf)
{

	struct SPI_NAND_FLASH_INFO_T	*ptr_dev_info_t;
	SPI_NAND_FLASH_RTN_T			rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;

	ptr_dev_info_t	= _SPI_NAND_GET_DEVICE_INFO_PTR;

	rtn_status = spi_nand_read_internal( (((ptr_dev_info_t->page_size)*page_id)+offset), len, rbuf, SPI_NAND_FLASH_READ_SPEED_MODE_SINGLE);

	if(rtn_status == SPI_NAND_FLASH_RTN_NO_ERROR)
	{
		return 0;
	}
	else
	{
		_SPI_NAND_PRINTF("spinand_read_page, error\n");
		return -1;
	}

}

/*
 * spinand_program_data_to_cache--to write a page to cache with:
 * @byte_id: the location to write to the cache
 * @len:     number of bytes to write
 * @rbuf:    read buffer to hold @len bytes
 *
 * Description:
 *   The write command used here is 0x84--indicating that the cache is
 *   not cleared first.
 *   Since it is writing the data to cache, there is no tPROG time.
 */
static int spinand_program_data_to_cache(struct spi_device *spi_nand,
                u32 page_id, u32 byte_id, u32 len, u8 *wbuf)
{
        return spi_nand_protocol_program_load_single_speed(byte_id, wbuf, len);
}

/**
 * spinand_program_execute--to write a page from cache to the Nand array with
 * @page_id: the physical page location to write the page.
 *
 * Description:
 *   The write command used here is 0x10--indicating the cache is writing to
 *   the Nand array.
 *   Need to wait for tPROG time to finish the transaction.
 */
static int spinand_program_execute(struct spi_device *spi_nand, u32 page_id)
{
	return spi_nand_protocol_program_execute(page_id);
}

/**
 * spinand_program_page--to write a page with:
 * @page_id: the physical page location to write the page.
 * @offset:  the location from the cache starting from 0 to 2111
 * @len:     the number of bytes to write
 * @wbuf:    the buffer to hold the number of bytes
 *
 * Description:
 *   The commands used here are 0x06, 0x84, and 0x10--indicating that
 *   the write enable is first sent, the write cache command, and the
 *   write execute command.
 *   Poll to wait for the tPROG time to finish the transaction.
 */
static int spinand_program_page(struct mtd_info *mtd,
                u32 page_id, u32 offset, u32 len, u8 *buf)
{

	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "[spinand_program_page]: enter, page=0x%x\n", page_id);

	struct SPI_NAND_FLASH_INFO_T	*ptr_dev_info_t;
	struct spinand_state *state = mtd_to_state(mtd);			
	SPI_NAND_FLASH_RTN_T			rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;

	ptr_dev_info_t	= _SPI_NAND_GET_DEVICE_INFO_PTR;



	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_2, "spinand_program_page: _current_cache_page_oob_mapping: state->oob_buf_len=0x%x, state->oob_buf=\n", (state->oob_buf_len));
	_SPI_NAND_DEBUG_PRINTF_ARRAY(SPI_NAND_FLASH_DEBUG_LEVEL_2, &(state->oob_buf[0]), (state->oob_buf_len));
	
	rtn_status = spi_nand_write_page_internal(page_id, (state->buf_idx), &state->buf[(state->buf_idx)], (state->buf_len),  0, (&state->oob_buf[0]), (state->oob_buf_len), SPI_NAND_FLASH_WRITE_SPEED_MODE_SINGLE);

	if( rtn_status == SPI_NAND_FLASH_RTN_NO_ERROR)
	{
		return 0;
	}
	else
	{
		_SPI_NAND_PRINTF("spinand_program_page, error\n");
		return -1;
	}
	
}

/**
 * spinand_erase_block_erase--to erase a page with:
 * @block_id: the physical block location to erase.
 *
 * Description:
 *   The command used here is 0xd8--indicating an erase command to erase
 *   one block--64 pages
 *   Need to wait for tERS.
 */
static int spinand_erase_block_erase(struct spi_device *spi_nand, u32 block_id)
{

	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1,"[spinand_erase_block_erase]: enter, block id=0x%x \n", block_id);
	return spi_nand_protocol_block_erase(block_id);
}

/**
 * spinand_erase_block--to erase a page with:
 * @block_id: the physical block location to erase.
 *
 * Description:
 *   The commands used here are 0x06 and 0xd8--indicating an erase
 *   command to erase one block--64 pages
 *   It will first to enable the write enable bit (0x06 command),
 *   and then send the 0xd8 erase command
 *   Poll to wait for the tERS time to complete the tranaction.
 */
static int spinand_erase_block(struct spi_device *spi_nand, u32 block_id)
{

	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1,"[spinand_erase_block]: enter, block id=0x%x \n", block_id);

	struct SPI_NAND_FLASH_INFO_T	*ptr_dev_info_t;
	SPI_NAND_FLASH_RTN_T			rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;
	

	ptr_dev_info_t	= _SPI_NAND_GET_DEVICE_INFO_PTR;	
	
	rtn_status = spi_nand_erase_internal( ((block_id)*(ptr_dev_info_t->erase_size)) , (ptr_dev_info_t->erase_size) );

	if(rtn_status == SPI_NAND_FLASH_RTN_NO_ERROR)
	{
		return 0;
	}
	else
	{
		_SPI_NAND_PRINTF("spinand_erase_block, error\n");
		return -1;
	}
		
}


void spinand_write_buf(struct mtd_info *mtd, const uint8_t *buf, int len)
{

	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "[spinand_write_buf]: enter \n");
	int		min_oob_size;
    struct spinand_state *state = mtd_to_state(mtd);

	struct SPI_NAND_FLASH_INFO_T	*ptr_dev_info_t;
	ptr_dev_info_t	= _SPI_NAND_GET_DEVICE_INFO_PTR;

	if( (state->col) >= (mtd->writesize))	/* Write OOB area */
	{
		min_oob_size = MIN(len, (MAX_LINUX_USE_OOB_SIZE - ((state->col) - (mtd->writesize))));
		memcpy( &(state->oob_buf)[((state->col)-(mtd->writesize))+LINUX_USE_OOB_START_OFFSET], buf, min_oob_size);
		state->col += min_oob_size;
		state->oob_buf_len = min_oob_size;
	}
	else 										/* Write Data area */
	{
		memcpy( &(state->buf)[state->buf_idx], buf, len);
	    state->col += len;
		state->buf_len += len;
	}

}

void spinand_read_buf(struct mtd_info *mtd, uint8_t *buf, int len)
{
    struct spinand_state *state = mtd_to_state(mtd);
	struct SPI_NAND_FLASH_INFO_T	*ptr_dev_info_t;
	ptr_dev_info_t	= _SPI_NAND_GET_DEVICE_INFO_PTR;

	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "[spinand_read_buf]: enter, len=0x%x, offset=0x%x\n", len, state->buf_idx);	

	if( ((state->command) == NAND_CMD_READID) ||
		((state->command) == NAND_CMD_STATUS)    )
	{
		memcpy(buf, &state->buf, len);
	}
	else
	{
		if( (state->buf_idx) < ( ptr_dev_info_t->page_size ))		/* Read data area */
		{
        	memcpy(buf, &_current_cache_page_data[state->buf_idx], len);
		}
		else														/* Read oob area */
		{
			/* dump_stack(); */
			memcpy(buf, &_current_cache_page_oob_mapping[ ((state->buf_idx)-(ptr_dev_info_t->page_size))+ LINUX_USE_OOB_START_OFFSET], len);			
		}	        
	}	

	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_2, "spinand_read_buf : idx=0x%x, len=0x%x\n", (state->buf_idx), len);
	_SPI_NAND_DEBUG_PRINTF_ARRAY(SPI_NAND_FLASH_DEBUG_LEVEL_2, &buf[0], len);
	
	state->buf_idx += len;
		
}


#ifdef CONFIG_MTD_SPINAND_ONDIEECC
static void spinand_write_page_hwecc(struct mtd_info *mtd,
                struct nand_chip *chip, const uint8_t *buf, int oob_required)
{
	
	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "[spinand_write_page_hwecc]: enter \n");

	spinand_write_buf(mtd, buf, mtd->writesize);
	spinand_write_buf(mtd, chip->oob_poi, mtd->oobsize);


	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_2, "spinand_write_page_hwecc: data=\n");
	_SPI_NAND_DEBUG_PRINTF_ARRAY(SPI_NAND_FLASH_DEBUG_LEVEL_2, &buf[0], mtd->writesize);
	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_2, "spinand_write_page_hwecc: oob=\n");
	_SPI_NAND_DEBUG_PRINTF_ARRAY(SPI_NAND_FLASH_DEBUG_LEVEL_2, &chip->oob_poi[0], mtd->oobsize);	
	
}

static int spinand_read_page_hwecc(struct mtd_info *mtd, struct nand_chip *chip,
                uint8_t *buf, int page)
{
	u32    idx;
	struct SPI_NAND_FLASH_INFO_T	*ptr_dev_info_t;

	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "[spinand_read_page_hwecc]: enter, page=0x%x \n", page);	
	

	_SPI_NAND_SEMAPHORE_LOCK();

	ptr_dev_info_t	= _SPI_NAND_GET_DEVICE_INFO_PTR;	

	if(buf==NULL)
	{
		_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "[spinand_read_page_hwecc]buf is NULL\n");
	}
	
	memcpy(buf, &_current_cache_page_data[0], (ptr_dev_info_t->page_size));
	memcpy((chip->oob_poi), &_current_cache_page_oob_mapping[LINUX_USE_OOB_START_OFFSET], MAX_LINUX_USE_OOB_SIZE);


	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_2, "spinand_read_page_hwecc: data:\n");
	_SPI_NAND_DEBUG_PRINTF_ARRAY(SPI_NAND_FLASH_DEBUG_LEVEL_2, &buf[0], mtd->writesize);
	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_2, "spinand_read_page_hwecc: oob:\n");
	_SPI_NAND_DEBUG_PRINTF_ARRAY(SPI_NAND_FLASH_DEBUG_LEVEL_2, &chip->oob_poi[0], mtd->oobsize);	
	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_2, "spinand_read_page_hwecc: _current_cache_page_oob_mapping:\n");
	_SPI_NAND_DEBUG_PRINTF_ARRAY(SPI_NAND_FLASH_DEBUG_LEVEL_2, &_current_cache_page_oob_mapping[0], mtd->oobsize);			

	_SPI_NAND_SEMAPHORE_UNLOCK();

	return 0;		

}

#endif


static int spinand_write_page(struct mtd_info *mtd, struct nand_chip *chip, 
        const u8 *buf, int page, int cached, int raw)
{

	struct SPI_NAND_FLASH_INFO_T	*ptr_dev_info_t;
	struct spinand_state *state = mtd_to_state(mtd);			
	SPI_NAND_FLASH_RTN_T			rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;


	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "[spinand_write_page]: enter, page=0x%x \n", page);	

	memset((state->oob_buf), 0xff, MAX_USE_OOB_SIZE);
	memcpy(&(state->oob_buf)[LINUX_USE_OOB_START_OFFSET], chip->oob_poi, MAX_LINUX_USE_OOB_SIZE);
	
	rtn_status = spi_nand_write_page_internal(page, 0, buf, (ptr_dev_info_t->page_size),  0, (state->oob_buf), MAX_LINUX_USE_OOB_SIZE, SPI_NAND_FLASH_WRITE_SPEED_MODE_SINGLE);
	if(rtn_status == SPI_NAND_FLASH_RTN_NO_ERROR ) 
	{
		return 0;
	}
	else
	{
		return -EIO;
	}
}


static int spinand_write_oob(struct mtd_info *mtd, struct nand_chip *chip, int page)
{

	struct spinand_state *state = mtd_to_state(mtd);			
	SPI_NAND_FLASH_RTN_T			rtn_status = SPI_NAND_FLASH_RTN_NO_ERROR;


	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "[spinand_write_oob]: enter, page=0x%x \n", page);	

	memset(state->oob_buf, 0xff, MAX_USE_OOB_SIZE);
	memcpy(&(state->oob_buf)[LINUX_USE_OOB_START_OFFSET], chip->oob_poi, MAX_LINUX_USE_OOB_SIZE);

	rtn_status = spi_nand_write_page_internal(page, 0, NULL, 0,  0, (&state->oob_buf[0]), MAX_LINUX_USE_OOB_SIZE, SPI_NAND_FLASH_WRITE_SPEED_MODE_SINGLE);

	if(rtn_status == SPI_NAND_FLASH_RTN_NO_ERROR ) 
	{
		return 0;
	}
	else
	{
		return -EIO;
	}
	
}


static int spinand_read_oob(struct mtd_info *mtd,struct nand_chip *chip, int page, int sndcmd)
{
	return 0;
	
}


static int spinand_block_markbad(struct mtd_info *mtd, loff_t offset)
{

	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "[spinand_block_markbad]: enter , offset=0x%x\n", offset);

	return 0;

}


static int spinand_block_bad(struct mtd_info *mtd, loff_t ofs, int getchip)
{

	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "[spinand_block_bad]: enter \n");
	
	return 0;
}




static void spinand_select_chip(struct mtd_info *mtd, int dev)
{
	
	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "[spinand_select_chip]: enter \n");	
}

static int spinand_dev_ready(struct mtd_info *mtd)
{	
	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "[spinand_dev_ready]: enter \n");
	return 1;
}

static void spinand_enable_hwecc(struct mtd_info *mtd, int mode)
{
	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "[spinand_enable_hwecc]: enter \n");
}


static int spinand_correct_data(struct mtd_info *mtd, u_char *dat,
				     u_char *read_ecc, u_char *calc_ecc)
{
	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "[spinand_correct_data]: enter \n");
	return 0;
}




static int spinand_calculate_ecc(struct mtd_info *mtd, const u_char *dat, u_char *ecc_code)
{
	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "[spinand_calculate_ecc]: enter \n");
	return 0;
}


static uint8_t spinand_read_byte(struct mtd_info *mtd)
{

        struct spinand_state *state = mtd_to_state(mtd);
        u8 data;

		_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "[spinand_read_byte]: enter \n");		

        data = state->buf[state->buf_idx];
        state->buf_idx++;
        return data;
}


static int spinand_wait(struct mtd_info *mtd, struct nand_chip *chip)
{


        struct spinand_info *info = (struct spinand_info *)chip->priv;

        unsigned long timeo = jiffies;
        int retval, state = chip->state;
        u8 status;

		_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "[spinand_wait]: enter \n");		

        if (state == FL_ERASING)
                timeo += (HZ * 400) / 1000;
        else
                timeo += (HZ * 20) / 1000;

        while (time_before(jiffies, timeo)) {
                retval = spinand_read_status(info->spi, &status);
                if ((status & STATUS_OIP_MASK) == STATUS_READY)
                        return 0;

                cond_resched();
        }
        return 0;
}


/*
 * spinand_reset- send RESET command "0xff" to the Nand device.
 */
static void spinand_reset(struct spi_device *spi_nand)
{

	_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "[spinand_reset]: enter \n");

	spi_nand_protocol_reset();
}


static void spinand_cmdfunc(struct mtd_info *mtd, unsigned int command, int column, int page)
{

	struct SPI_NAND_FLASH_INFO_T	*ptr_dev_info_t;
	
    struct nand_chip *chip = (struct nand_chip *)mtd->priv;
    struct spinand_info *info = (struct spinand_info *)chip->priv;
    struct spinand_state *state = (struct spinand_state *)info->priv;
    u16		block_id;

	ptr_dev_info_t	= _SPI_NAND_GET_DEVICE_INFO_PTR;

	state->command = command;
	
	switch (command) {
	/*
	 * READ0 - read in first  0x800 bytes
	 */
	case NAND_CMD_READ1:
			_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "[spinand_cmdfunc]: NAND_CMD_READ1 \n");
	case NAND_CMD_READ0:
			_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "[spinand_cmdfunc]: NAND_CMD_READ0 \n");	

			state->buf_idx = column;
			spi_nand_read_page_internal(page, SPI_NAND_FLASH_READ_SPEED_MODE_SINGLE);
			
			_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1,"[spinand_cmdfunc]: NAND_CMD_READ0/1, End\n");

	        break;
	/* READOOB reads only the OOB because no ECC is performed. */
	case NAND_CMD_READOOB:
 			_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "[spinand_cmdfunc]: NAND_CMD_READOOB, page=0x%x \n", page);
	        state->buf_idx = column + (ptr_dev_info_t->page_size);
	        spi_nand_read_page_internal(page, SPI_NAND_FLASH_READ_SPEED_MODE_SINGLE);
			
	        break;
	case NAND_CMD_RNDOUT:
			_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "[spinand_cmdfunc]: NAND_CMD_RNDOUT \n");
	        state->buf_idx = column;
	        break;
	case NAND_CMD_READID:
			_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "[spinand_cmdfunc]: NAND_CMD_READID \n");
	        state->buf_idx = 0;
	        spinand_read_id(info->spi, (u8 *)state->buf);
	        break;
	/* ERASE1 stores the block and page address */
	case NAND_CMD_ERASE1:
			block_id = page /((mtd->erasesize)/(mtd->writesize));

			_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "[spinand_cmdfunc]: NAND_CMD_ERASE1 \n");			
			_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "erasesize=0x%x, writesiez=0x%x, page=0x%x, block_idx=0x%x\n", (mtd->erasesize), (mtd->writesize), page, block_id);
	        spinand_erase_block(info->spi, block_id);
	        break;
	/* ERASE2 uses the block and page address from ERASE1 */
	case NAND_CMD_ERASE2:
			_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1,"[spinand_cmdfunc]: NAND_CMD_ERASE2 \n");
	        break;
	/* SEQIN sets up the addr buffer and all registers except the length */
	case NAND_CMD_SEQIN:
			_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "[spinand_cmdfunc]: NAND_CMD_SEQIN \n");			
	        state->col = column;
	        state->row = page;
	        state->buf_idx = column;
			state->buf_len = 0;			
			state->oob_buf_len = 0 ;
			memset(state->buf, 0xff, BUFSIZE);
			memset(state->oob_buf, 0xff, MAX_USE_OOB_SIZE);
	        break;
	/* PAGEPROG reuses all of the setup from SEQIN and adds the length */
	case NAND_CMD_PAGEPROG:
			_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "[spinand_cmdfunc]: NAND_CMD_PAGEPROG \n");			
	        spinand_program_page(mtd, state->row, state->col,
	                        state->buf_idx, state->buf);
	        break;
	case NAND_CMD_STATUS:
			_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "[spinand_cmdfunc]: NAND_CMD_STATUS \n");			
	        spinand_get_otp(info->spi, state->buf);
	        if (!(state->buf[0] & 0x80))
	                state->buf[0] = 0x80;
	        state->buf_idx = 0;
	        break;
	/* RESET command */
	case NAND_CMD_RESET:
			_SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1, "[spinand_cmdfunc]: NAND_CMD_RESET \n");			
	        if (wait_till_ready(info->spi))
	                printk("WAIT timedout!!!\n");
	        /* a minimum of 250us must elapse before issuing RESET cmd*/
	        udelay(250);
	        spinand_reset(info->spi);
	        break;
	default:
	        _SPI_NAND_DEBUG_PRINTF(SPI_NAND_FLASH_DEBUG_LEVEL_1,"[spinand_cmdfunc]: Unknown CMD: 0x%x\n", command);
	}
}


struct nand_flash_dev spi_nand_flash_ids[] = {
	{NULL,	0, 0, 0, 0, 0},
	{NULL,}
};


static void free_allcate_memory(struct mtd_info *mtd)
{


	_SPI_NAND_PRINTF("SPI NAND : free_allcate_memory");
	
	if( ((struct spinand_info *)(((struct nand_chip *)(mtd->priv))->priv))->spi )
	{
		kfree( ((struct spinand_info *)(((struct nand_chip *)(mtd->priv))->priv))->spi );
	}

	if( ((struct spinand_info *)(((struct nand_chip *)(mtd->priv))->priv)) )
	{
		kfree( ((struct spinand_info *)(((struct nand_chip *)(mtd->priv))->priv)) );
	}

	if( (((struct nand_chip *)(mtd->priv))->priv) )	
	{
		kfree ( (((struct nand_chip *)(mtd->priv))->priv) ) ;
	}

	if((mtd->priv) )
	{
		kfree((mtd->priv));
	}

	if(mtd)
	{
		kfree(mtd);
	}
		
}


static int spi_nand_setup(u32 *ptr_rtn_mtd_address)
{
    struct mtd_info 		*mtd;
    struct nand_chip 		*chip;
    struct spinand_info 	*info;
    struct spinand_state	*state;    
    struct spi_device		*spi_nand;
	struct SPI_NAND_FLASH_INFO_T	*ptr_dev_info_t;
	int ret;

	ptr_dev_info_t	= _SPI_NAND_GET_DEVICE_INFO_PTR;

	_SPI_NAND_PRINTF("[spi_nand_setup] : Enter \n");
    
	/* 1. Allocate neccessary struct memory ,and assigned related pointer */
    info  = kzalloc(sizeof(struct spinand_info),GFP_KERNEL);
    if (!info)
    {
    	_SPI_NAND_PRINTF("spi_nand_setup: allocate info structure error! \n");
		return -ENOMEM;            
    }
            	            
	/* Chuck Kuo, temp assign. Other function will pass it, but we wil not use it in functions. */	            
	spi_nand = kzalloc(sizeof(struct spinand_info),GFP_KERNEL);	
    if (!spi_nand)
    {	
	    _SPI_NAND_PRINTF("spi_nand_setup: allocate spi_nand structure error! \n");
		return -ENOMEM;
    }
		
    info->spi = spi_nand;		

    state = kzalloc(sizeof(struct spinand_state),GFP_KERNEL);    
    if (!state)
    {	
	    _SPI_NAND_PRINTF("spi_nand_setup: allocate state structure error! \n");
		return -ENOMEM;
    }

    info->priv      = state;
    state->buf_idx  = 0;
    state->buf      = kzalloc( BUFSIZE, GFP_KERNEL);			/* Data buffer */
	state->oob_buf	= kzalloc( MAX_USE_OOB_SIZE, GFP_KERNEL);	/* OOB buffer */
    if (!state->buf)
    {
    	_SPI_NAND_PRINTF("spi_nand_setup: allocate data buf error! \n");
		return -ENOMEM;
    }
    if (!state->oob_buf)
    {
    	_SPI_NAND_PRINTF("spi_nand_setup: allocate oob buf error! \n");
		return -ENOMEM;
    }	

    chip = kzalloc(sizeof(struct nand_chip),GFP_KERNEL);
    if (!chip)
    {
	    _SPI_NAND_PRINTF("spi_nand_setup: allocate chip structure error! \n");
    	return -ENOMEM;
    }

	chip->priv				= info;
	chip->read_byte 		= spinand_read_byte;
	chip->read_buf			= spinand_read_buf;
	chip->write_buf 		= spinand_write_buf;
	chip->waitfunc			= spinand_wait;
	chip->options			|= NAND_CACHEPRG;
	chip->select_chip		= spinand_select_chip;
	chip->dev_ready 		= spinand_dev_ready;
	chip->cmdfunc			= spinand_cmdfunc;
#ifdef CONFIG_MTD_SPINAND_ONDIEECC
    chip->ecc.mode  		= NAND_ECC_HW;
    chip->ecc.size  		= 0x200;
    chip->ecc.bytes 		= 0x4;
    chip->ecc.steps 		= 0x4;
	chip->ecc.total 		= chip->ecc.steps * chip->ecc.bytes;
#if 0  
    chip->ecc.strength		= 1;
#endif     
    chip->ecc.layout 		= &spinand_oob_64;
    chip->ecc.read_page		= spinand_read_page_hwecc;
    chip->ecc.write_page 	= spinand_write_page_hwecc;
#else
    chip->ecc.mode  = NAND_ECC_SOFT;
    if (spinand_disable_ecc(spi_nand) < 0)
            pr_info("%s: disable ecc failed!\n", __func__);
#endif
	

	
    chip->options	|= NAND_SKIP_BBTSCAN;   	/*To skips the bbt scan during initialization.  */
    /*  For BMT, we need to revise driver architecture */
#if 0	
    //chip->write_page		= spinand_write_page;
    //chip->ecc.read_oob		= spinand_read_oob;
#endif
    chip->ecc.write_oob		= spinand_write_oob;
    chip->block_markbad		= spinand_block_markbad;   /* tmp null */
    chip->block_bad			= spinand_block_bad;		/* tmp null */
	chip->ecc.calculate		= spinand_calculate_ecc;
	chip->ecc.correct		= spinand_correct_data;
	chip->ecc.hwctl			= spinand_enable_hwecc;

    mtd = kzalloc(sizeof(struct mtd_info), GFP_KERNEL);
    if (!mtd)
    {
		    _SPI_NAND_PRINTF("spi_nand_setup: allocate mtd error! \n");
            return -ENOMEM;
    }

	spi_nand_mtd = mtd;
    mtd->priv = chip;
    mtd->name = "EN7512-SPI_NAND";
    mtd->owner = THIS_MODULE;
    mtd->oobsize = MAX_LINUX_USE_OOB_SIZE;

	spi_nand_flash_ids[0].name 		= ptr_dev_info_t->ptr_name;
	spi_nand_flash_ids[0].id   		= ptr_dev_info_t->dev_id; 
	spi_nand_flash_ids[0].pagesize  = ptr_dev_info_t->page_size; 
	spi_nand_flash_ids[0].chipsize  = ((ptr_dev_info_t->device_size)>>20); 
	spi_nand_flash_ids[0].erasesize = ptr_dev_info_t->erase_size; 
	spi_nand_flash_ids[0].options   = 0; 
	
	ret = nand_scan_ident(mtd, 1, spi_nand_flash_ids);
	if (!ret)
	{
		_SPI_NAND_PRINTF("nand_scan_ident ok\n");
		ret = nand_scan_tail(mtd);
		_SPI_NAND_PRINTF("[spi_nand_setup]: chip size =  0x%llx, erase_shift=0x%x\n", chip->chipsize, chip->phys_erase_shift);
	}
	else
	{
		_SPI_NAND_PRINTF("nand_scan_ident fail\n");
		return -ENOMEM;
	}           


#if	defined(TCSUPPORT_NAND_BMT) && !defined(LZMA_IMG)

				_SPI_NAND_SEMAPHORE_LOCK();

				bmt_pool_size = calc_bmt_pool_size(mtd);
				if(bmt_pool_size > maximum_bmt_block_count)
				{
					_SPI_NAND_PRINTF("Error : bmt pool size: %d > maximum size %d\n", bmt_pool_size, maximum_bmt_block_count);
					_SPI_NAND_PRINTF("Error: init bmt failed \n");
					return -1;
				}				
				_SPI_NAND_PRINTF("bmt pool size: %d \n", bmt_pool_size);	
				
				if (!g_bmt)
				{
					if ( !(g_bmt = init_bmt(mtd, bmt_pool_size)) )
					{
						_SPI_NAND_PRINTF("Error: init bmt failed \n");
						_SPI_NAND_SEMAPHORE_UNLOCK();
						return -1;
					}
				}
			
				if (!g_bbt)
				{
					if ( !(g_bbt = start_init_bbt()) )
					{
						_SPI_NAND_PRINTF("Error: init bbt failed \n");
							_SPI_NAND_SEMAPHORE_UNLOCK();
						return -1;
					}
				}
	
				if(write_bbt_or_bmt_to_flash() != 0)
				{				
					_SPI_NAND_PRINTF("Error: save bbt or bmt to nand failed \n");
					_SPI_NAND_SEMAPHORE_UNLOCK();
					return -1;
				}
				
				if(create_badblock_table_by_bbt())
				{
					_SPI_NAND_PRINTF("Error: create bad block table failed \n");
					_SPI_NAND_SEMAPHORE_UNLOCK();
					return -1;
				}
	
				_SPI_NAND_PRINTF("BMT & BBT Init Success \n");

				_SPI_NAND_SEMAPHORE_UNLOCK();				

#ifdef TCSUPPORT_CT_PON
			    	nand_flash_avalable_size = chip->chipsize - (BMT_SIZE_FOR_RESERVE_AREA * 0x20000);
					mtd->size = chip->chipsize - (BMT_SIZE_FOR_RESERVE_AREA * 0x20000);
#else					

	mtd->size = nand_logic_size;					
#endif							
				
					
#endif
	ranand_read_byte  = SPI_NAND_Flash_Read_Byte;
	ranand_read_dword = SPI_NAND_Flash_Read_DWord;


	*ptr_rtn_mtd_address = mtd;
	
	return 0;
	
	
}


static int spi_nand_proc_read(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{

	int len;

	if (off > 0) 
	{
		return 0;
	}

	len = sprintf(page, "SPI NAND DEBUG LEVEL=%d, _SPI_NAND_TEST_FLAG=%d\n", _SPI_NAND_DEBUG_LEVEL, _SPI_NAND_TEST_FLAG);

	return len;
	
}

static int spi_nand_proc_write(struct file* file, const char* buffer,
	unsigned long count, void *data)
{


	char buf[16];

	int len = count;

	if (copy_from_user(buf, buffer, len)) 
    {
		return -EFAULT;
	}	

	buf[len] = '\0';

	_SPI_NAND_PRINTF("len = 0x%x, buf[0]=%c, buf[1]=%c\n", len , buf[0], buf[1]);


	if (buf[0] == '0') 
	{
		_SPI_NAND_PRINTF("Set SPI NAND DEBUG LEVLE to %d\n", SPI_NAND_FLASH_DEBUG_LEVEL_0);
		_SPI_NAND_DEBUG_LEVEL = SPI_NAND_FLASH_DEBUG_LEVEL_0;
	}
	else if (buf[0] == '1') 
	{
		_SPI_NAND_PRINTF("Set SPI NAND DEBUG LEVLE to %d\n", SPI_NAND_FLASH_DEBUG_LEVEL_1);
		_SPI_NAND_DEBUG_LEVEL = SPI_NAND_FLASH_DEBUG_LEVEL_1;
	}	
	else if (buf[0] == '2') 
	{
		_SPI_NAND_PRINTF("Set SPI NAND DEBUG LEVLE to %d\n", SPI_NAND_FLASH_DEBUG_LEVEL_2);
		_SPI_NAND_DEBUG_LEVEL = SPI_NAND_FLASH_DEBUG_LEVEL_2;
	}	
	else
	{
		_SPI_NAND_PRINTF("DEBUG LEVEL only up to %d\n", (SPI_NAND_FLASH_DEBUG_LEVEL_DEF_NO -1 ));
	}

	if(buf[1] == '0')
	{
		_SPI_NAND_TEST_FLAG = 0;
		_SPI_NAND_PRINTF("Set _SPI_NAND_TEST_FLAG to %d\n", _SPI_NAND_TEST_FLAG);
	}
	if(buf[1] == '1')
	{
		_SPI_NAND_TEST_FLAG = 1;
		_SPI_NAND_PRINTF("Set _SPI_NAND_TEST_FLAG to %d\n", _SPI_NAND_TEST_FLAG);
	}	

	return len;

}



static struct mtd_info *spi_nand_probe_kernel(struct map_info *map)
{
	
	u32	mtd_address;
	int	rtn_status;

	_SPI_NAND_PRINTF("EN7512 mtd init: spi nand probe enter\n");		

	rtn_status = spi_nand_setup(&mtd_address);

	if(rtn_status == 0 )   /* Probe without error */
	{
		return ((struct mtd_info * )(mtd_address));
	}
	else
	{
		free_allcate_memory( (struct mtd_info * )(mtd_address) );			
		_SPI_NAND_PRINTF("[spi_nand_probe_kernel] probe fail !\n");
		return NULL;
	}

}

static void spi_nand_destroy_kernel(struct mtd_info *mtd)
{
	free_allcate_memory(mtd);	
}

static struct mtd_chip_driver spi_nand_chipdrv = {
	.probe	 = spi_nand_probe_kernel,
	.destroy = spi_nand_destroy_kernel,
	.name	 = "nandflash_probe",
	.module	 = THIS_MODULE
};

static int __init linux_spi_nand_flash_init(void)
{

	struct proc_dir_entry *entry;

	_SPI_NAND_PRINTF("IS_SPIFLASH=0x%x, IS_NANDFLASH=0x%x, (0xBFA10114)=0x%lx)\n", (unsigned int)IS_SPIFLASH, (unsigned int)IS_NANDFLASH, VPint(0xBFA10114));
	
	if(IS_SPIFLASH){ 	/* For boot from SPI NOR, then mount NAND as a MTD partition */
		_SPI_NAND_PRINTF("[linux_spi_nand_flash_init] spi nor flash\n");
		return -1;
	}
	else{				
		SPI_NAND_Flash_Init(0);
		
		_SPI_NAND_PRINTF("spi nand flash\n");
		register_mtd_chip_driver(&spi_nand_chipdrv);


		entry = create_proc_entry(SPI_NAND_PROCNAME, 0666, NULL);
		if (entry == NULL) 
		{
			_SPI_NAND_PRINTF("SPI NAND  unable to create /proc entry\n");
			return -ENOMEM;
		}
		entry->read_proc = spi_nand_proc_read;
		entry->write_proc = spi_nand_proc_write;

		
		return 0;
	}

}

static void __init linux_spi_nand_flash_exit(void)
{
	if(IS_SPIFLASH){
		}
	else
	{
		unregister_mtd_chip_driver(&spi_nand_chipdrv);

		remove_proc_entry(SPI_NAND_PROCNAME, NULL);
	}	
}

module_init(linux_spi_nand_flash_init);
module_exit(linux_spi_nand_flash_exit);


/* End of [spi_nand_flash.c] package */
