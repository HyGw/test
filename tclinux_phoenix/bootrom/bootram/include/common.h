#ifndef _COMMON_H
#define _COMMON_H

#if defined(TCSUPPORT_FWC_ENV)
#include <personality_parm_global_def.h>
#endif

#ifdef TCSUPPORT_FREE_BOOTBASE
#define KERNEL_START_ADDR	0x80002000  
#else
#define KERNEL_START_ADDR	0x80020000
#endif

#define KERNEL_RAM_START 	0x80750000
#define KERNEL_RAM_END   	0x807b0000

#define KERNEL_FLASH_START 	0xbfc20100
#define KERNEL_FLASH_END   	0xbfda0000

#ifdef CONFIG_DUAL_IMAGE
#ifdef TCSUPPORT_FREE_BOOTBASE
#define FLAG_ADDR		0x80001fff;
#else
#define	FLAG_ADDR		0x8001ffff;
#endif
#endif

#ifdef TCSUPPORT_MTD_PARTITIONS_CMDLINE
#define TCLINUX_SLAVE_FLASH_START	(ecnt_get_tclinux_slave_mtd_offset()-ecnt_get_tclinux_mtd_offset())
#else
#if defined(TCSUPPORT_CT_DUAL_IMAGE)
#if defined(TCSUPPORT_CT_PON)
#ifdef TCSUPPORT_RESERVEAREA_EXTEND
#ifdef TCSUPPORT_BB_NAND//tony add
#define TCLINUX_SLAVE_FLASH_START (0x1000000)             // Attention: should take tclinux start address into consideration, which means should +0x30000
#else
#if defined(TCSUPPORT_CT_PON_CAU)
#define TCLINUX_SLAVE_FLASH_START (0x3a0000)             // Attention: should take tclinux start address into consideration, which means should +0x3000
#else
#define TCLINUX_SLAVE_FLASH_START (0x7b0000)             // Attention: should take tclinux start address into consideration, which means should +0x30000
#endif
#endif
#else
#define TCLINUX_SLAVE_FLASH_START (0x780000)
#endif
#else
#define TCLINUX_SLAVE_FLASH_START (0x680000)
#endif
#else
#if defined(TCSUPPORT_CUC_DUAL_IMAGE)
#define TCLINUX_SLAVE_FLASH_START (0x1000000)
#else
#define TCLINUX_SLAVE_FLASH_START (0x500000)
#endif
#endif
#endif

#ifdef BOOTROM_LARGE_SIZE
#ifdef TCSUPPORT_MTD_PARTITIONS_CMDLINE
#define TCLINUX_FLASH_START		ecnt_get_tclinux_mtd_offset()
#else
#ifdef TCSUPPORT_BB_NAND//tony add
#if defined(TCSUPPORT_CT_PON) || defined(TCSUPPORT_NAND_BMT)
#define TCLINUX_FLASH_START	(0x80000)
#else
#define TCLINUX_FLASH_START	(0x30000)
#endif
#else
#define TCLINUX_FLASH_START	(0x30000)
#endif
#endif
#define LZMA_FLASH_START	(TCLINUX_FLASH_START + 0x100) //0x100 is trx header length
#define LZMA_FLASH_END	(0x130000)
#define LZMA_FLASH_LARGE_END	(0x1b0000)
#else
#ifdef TCSUPPORT_MTD_PARTITIONS_CMDLINE
#define TCLINUX_FLASH_START		ecnt_get_tclinux_mtd_offset()
#else
#define TCLINUX_FLASH_START	(0x20000)
#endif
#define LZMA_FLASH_START	(TCLINUX_FLASH_START + 0x100) //0x100 is trx header length
#define LZMA_FLASH_END	(0x120000)
#define LZMA_FLASH_LARGE_END	(0x1a0000)

#endif

#define TCBOOT_BASE             0x0
#ifdef TCSUPPORT_MTD_PARTITIONS_CMDLINE
#define TCBOOT_SIZE		ecnt_get_boot_size()
#else
#ifdef BOOTROM_LARGE_SIZE
#ifdef TCSUPPORT_BB_NAND//tony add
#if defined(TCSUPPORT_CT_PON) || defined(TCSUPPORT_NAND_BMT)
#define TCBOOT_SIZE 		0x40000 	//256K
#else
#define TCBOOT_SIZE 		0x20000 	//128K
#endif
#else
#define TCBOOT_SIZE 		0x20000 	//128K
#endif
#else
#define TCBOOT_SIZE 		0x10000
#endif
#endif
extern char readBootFlagFromFlash(void);
extern void writeBootFlagtoFlash(char flag);
extern unsigned char readCommitFlagFromFlash(void);
extern void writeCommitFlagToFlash(char flag);
extern void exchange_fw(char boot_flag, char commit_flag);

#if defined(TCSUPPORT_FWC_ENV)

extern int g_boot_debug_level;

#define E_BOOT_NO_INFO_LEVLE    (0)
#define E_BOOT_DEBUG_INFO_LEVLE (1 << 0)

#define BOOT_DEBUG_INFO(fmt, ...)  do{ \
										if(g_boot_debug_level & E_BOOT_DEBUG_INFO_LEVLE)\
											printf("func: %s, line:%d " fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
									}while(0)

#define FH_NW_PRODUCT_BOOT_NAME          			"boot"
#define FH_NW_PRODUCT_ROOTFS_NAME        			"rootfs"
#define FH_NW_PRODUCT_APP1_NAME          			"aImage0"
#define FH_NW_PRODUCT_APP2_NAME          			"aImage1"
#define FH_NW_PRODUCT_KERNEL_NAME        			"uImage"
#define FH_NW_PRODUCT_CONF_NAME          			"fhcfg"

#define FH_NW_PRODUCT_BOOT_FLAG     				(11)
#define FH_NW_PRODUCT_KERNEL_FLAG   				(12)
#define FH_NW_PRODUCT_ROOTFS_FLAG   				(13)
#define FH_NW_PRODUCT_APP1_FLAG     				(14)
#define FH_NW_PRODUCT_APP2_FLAG     	   			(15)
#define FH_NW_PRODUCT_CONF_FLAG     	   			(16)

#define RESET_CONTROL_REGISTER 						(0xbfb00040)
#define SYSTEM_SOFTWARE_RESET_OFFSET				(31)

typedef enum _FH_NW_PRODUCT_CRC_CHECK_
{
	E_FH_NW_PRODUCT_CHECK_RIGHT = 0, 
	E_FH_NW_PRODUCT_CHECK_WRONG = 1, 
	
}E_FH_NW_PRODUCT_CRC_CHECK_T;

typedef enum _FH_NW_PRODUCT_DECOMPRESS_
{
	E_FH_NW_PRODUCT_DECOMPRESS_RIGHT = 0, 
	E_FH_NW_PRODUCT_DECOMPRESS_WRONG = 1, 

}E_FH_NW_PRODUCT_DECOMPRESS_T;

typedef enum _FH_NW_PRODUCT_PARTITION_
{
	E_FH_NW_PRODUCT_KERNEL_A = 0, 
	E_FH_NW_PRODUCT_KERNEL_B = 1, 
	E_FH_NW_PRODUCT_ROOTFS_A = 2, 
	E_FH_NW_PRODUCT_ROOTFS_B = 3, 
	E_FH_NW_PRODUCT_APP1_A   = 4, 
	E_FH_NW_PRODUCT_APP1_B   = 5, 
	E_FH_NW_PRODUCT_APP2_A   = 6, 
	E_FH_NW_PRODUCT_APP2_B   = 7, 
	
}E_FH_NW_PRODUCT_PARTITION_T;

int read_fh_nw_product_dual_image_flag_env(dual_image_flag_t *pt_info);
int read_fh_nw_product_flash_env(personality_parm_flash_env_t *pt_info);
int read_fh_nw_product_memory_env(personality_parm_memory_env_t *pt_info);
#endif

#endif /* _COMMON_H */
