 /***************************************************************************************
 *      Copyright(c) 2014 ECONET Incorporation All rights reserved.
 *
 *      This is unpublished proprietary source code of ECONET Incorporation
 *
 *      The copyright notice above does not evidence any actual or intended
 *      publication of such source code.
 ***************************************************************************************
 */

/*======================================================================================
 * MODULE NAME: spi
 * FILE NAME: spi_nand_flash.h
 * DATE: 2014/11/21
 * VERSION: 1.00
 * PURPOSE: To Provide SPI NAND Access interface.
 * NOTES:
 *
 * AUTHOR : Chuck Kuo         REVIEWED by
 *
 * FUNCTIONS  
 *      SPI_NAND_Flash_Init             To provide interface for SPI NAND init. 
 *      SPI_NAND_Flash_Get_Flash_Info   To get system current flash info. 
 *      SPI_NAND_Flash_Write_Nbyte      To provide interface for Write N Bytes into SPI NAND Flash. 
 *      SPI_NAND_Flash_Read_NByte       To provide interface for Read N Bytes from SPI NAND Flash. 
 *      SPI_NAND_Flash_Erase            To provide interface for Erase SPI NAND Flash. 
 *      SPI_NAND_Flash_Read_Byte        To provide interface for read 1 Bytes from SPI NAND Flash. 
 *      SPI_NAND_Flash_Read_DWord       To provide interface for read Double Word from SPI NAND Flash. 
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

#ifndef __SPI_NAND_FLASH_H__
    #define __SPI_NAND_FLASH_H__

/* INCLUDE FILE DECLARATIONS --------------------------------------------------------- */
#include "asm/system.h"
#include "asm/types.h"

/* MACRO DECLARATIONS ---------------------------------------------------------------- */
#define SPI_NAND_FLASH_OOB_FREE_ENTRY_MAX 32

/* TYPE DECLARATIONS ----------------------------------------------------------------- */
typedef enum{
	SPI_NAND_FLASH_READ_DUMMY_BYTE_PREPEND,
	SPI_NAND_FLASH_READ_DUMMY_BYTE_APPEND,
	
	SPI_NAND_FLASH_READ_DUMMY_BYTE_DEF_NO
	
} SPI_NAND_FLASH_READ_DUMMY_BYTE_T;

typedef enum{
	SPI_NAND_FLASH_RTN_NO_ERROR =0,
	SPI_NAND_FLASH_RTN_PROBE_ERROR,
	SPI_NAND_FLASH_RTN_ALIGNED_CHECK_FAIL,
	SPI_NAND_FLASH_RTN_DETECTED_BAD_BLOCK,
	SPI_NAND_FLASH_RTN_ERASE_FAIL,
	SPI_NAND_FLASH_RTN_PROGRAM_FAIL,


	SPI_NAND_FLASH_RTN_DEF_NO
} SPI_NAND_FLASH_RTN_T;

typedef enum{
	SPI_NAND_FLASH_READ_SPEED_MODE_SINGLE =0,
	SPI_NAND_FLASH_READ_SPEED_MODE_DUAL,
	SPI_NAND_FLASH_READ_SPEED_MODE_QUAD,
	
	SPI_NAND_FLASH_READ_SPEED_MODE_DEF_NO	
} SPI_NAND_FLASH_READ_SPEED_MODE_T;


typedef enum{
	SPI_NAND_FLASH_WRITE_SPEED_MODE_SINGLE =0,
	SPI_NAND_FLASH_WRITE_SPEED_MODE_QUAD,
	
	SPI_NAND_FLASH_WRITE_SPEED_MODE_DEF_NO	
} SPI_NAND_FLASH_WRITE_SPEED_MODE_T;


typedef enum{
	SPI_NAND_FLASH_DEBUG_LEVEL_0 =0,
	SPI_NAND_FLASH_DEBUG_LEVEL_1,
	SPI_NAND_FLASH_DEBUG_LEVEL_2,
	
	SPI_NAND_FLASH_DEBUG_LEVEL_DEF_NO	
} SPI_NAND_FLASH_DEBUG_LEVEL_T;

#if 0
typedef enum{
	SPI_NAND_FLASH_PLANE_SELECT_NONE =0,
	SPI_NAND_FLASH_PLANE_SELECT_HAVE,

	SPI_NAND_FLASH_PLANE_SELECT_DEF_NO
	
} SPI_NAND_FLASH_PLANE_SELECT_T;
#endif

/* Bitwise */
#define SPI_NAND_FLASH_FEATURE_NONE			( 0x00 )
#define SPI_NAND_FLASH_PLANE_SELECT_HAVE	( 0x01 )
#define SPI_NAND_FLASH_DIE_SELECT_HAVE		( 0x01 << 1 )

struct spi_nand_flash_oobfree{
	unsigned long	offset;
	unsigned long	len;
};

struct spi_nand_flash_ooblayout
{	unsigned long	oobsize;
	struct spi_nand_flash_oobfree oobfree[SPI_NAND_FLASH_OOB_FREE_ENTRY_MAX];
};	


struct SPI_NAND_FLASH_INFO_T {
	const u8							mfr_id;
	const u8							dev_id;
	const u8							*ptr_name;
	u32									device_size;	/* Flash total Size */
	u32									page_size;		/* Page Size 		*/
	u32									erase_size;		/* Block Size 		*/
	u32									oob_size;		/* Spare Area (OOB) Size */
	SPI_NAND_FLASH_READ_DUMMY_BYTE_T	dummy_mode;
	u32									read_mode;
	struct spi_nand_flash_ooblayout		*oob_free_layout;
	u32									feature;
#if 0
	SPI_NAND_FLASH_PLANE_SELECT_T		plane_select;
#endif
};

struct nand_info {
	int mfr_id;
	int dev_id;
	char *name;
	int numchips;
	int chip_shift;
	int page_shift;
	int erase_shift;
	int oob_shift;
	int badblockpos;
	int opcode_type;
};

#if 1
struct ra_nand_chip {
	struct nand_info *flash;
};
#endif


typedef struct
{
   unsigned short id;          
   unsigned int ext_id; 
   unsigned char  addr_cycle;
   unsigned char  iowidth;
   unsigned short totalsize;   
   unsigned short blocksize;
   unsigned short pagesize;
   unsigned int timmingsetting;
   char devciename[14];
   unsigned int advancedmode;   
}flashdev_info;




/* EXPORTED SUBPROGRAM SPECIFICATION ------------------------------------------------- */

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
SPI_NAND_FLASH_RTN_T SPI_NAND_Flash_Init( u32   rom_base );

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
SPI_NAND_FLASH_RTN_T SPI_NAND_Flash_Get_Flash_Info( struct SPI_NAND_FLASH_INFO_T *ptr_rtn_into_t);

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
                                                 u32    							len,
                                                 u32    							*ptr_rtn_len,
                                                 u8									*ptr_buf,      
                                                 SPI_NAND_FLASH_WRITE_SPEED_MODE_T 	speed_mode	);

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
u32 SPI_NAND_Flash_Read_NByte( u32  								addr,
                               u32  								len,
                               u32  								*retlen,
                               u8   								*buf,
                               SPI_NAND_FLASH_READ_SPEED_MODE_T 	speed_mode   );

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
SPI_NAND_FLASH_RTN_T SPI_NAND_Flash_Erase( u32  dst_addr,
                                           u32  len      );

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
unsigned char SPI_NAND_Flash_Read_Byte( unsigned long    addr );

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
unsigned long SPI_NAND_Flash_Read_DWord( unsigned long  addr );

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
void SPI_NAND_DEBUG_ENABLE( void );

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
void SPI_NAND_DEBUG_DISABLE( void );

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
void SPI_NAND_Flash_Clear_Read_Cache_Data( void );

#endif /* ifndef __SPI_NAND_FLASH_H__ */
/* End of [spi_nand_flash.h] package */

