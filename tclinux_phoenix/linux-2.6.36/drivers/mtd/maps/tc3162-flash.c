#include <linux/init.h>
#include <linux/types.h>
#include <linux/root_dev.h>
#include <linux/kernel.h>
#include <linux/mtd/map.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/vmalloc.h>
#include <asm/io.h>
#include <asm/tc3162/tc3162.h>
#ifdef TCSUPPORT_DUAL_IMAGE_ENHANCE
#include "../chips/spiflash_tc3162.h"
#endif

#if defined(TCSUPPORT_INIC_CLIENT) || defined(TCSUPPORT_INIC_HOST)
#include <linux/mtd/fttdp_inic.h>
#endif

#if defined (TCSUPPORT_GPON_DUAL_IMAGE) || defined (TCSUPPORT_EPON_DUAL_IMAGE)
#include "flash_layout/tc_partition.h"
#endif

#if defined(TCSUPPORT_FWC_ENV)
#include <modules/personality_parm_global_def.h>
#endif

#ifdef TCSUPPORT_MTD_PARTITIONS_CMDLINE
#include <linux/slab.h>

#define STR_LEN	150
/* special size referring to all the remaining space in a partition */
#define SIZE_REMAINING UINT_MAX
#define SIZE_TO_GET (UINT_MAX-1)
#define OFFSET_CONTINUOUS UINT_MAX
#define OFFSET_BACK_FORWARD	(UINT_MAX-1)

/* error message prefix */
#define ERRP "mtd: "
#define TCLINUX "tclinux"
#define TCLINUX_SLAVE "tclinux_slave"
#define TCLINUX_REAL_SIZE "tclinux_real_size"
#define TCLINUX_SLAVE_REAL_SIZE "tclinux_slave_real_size"
#define RESERVEAREA "reservearea"
#define KERNEL_PART "kernel"
#define ROOTFS_PART "rootfs"
#define KERNEL_SLAVE_PART "kernel_slave"
#define ROOTFS_SLAVE_PART "rootfs_slave"
#define BOOTLOADER_PART "bootloader"
#define ROMFILE_PART "romfile"
#define BOOTLOADER_PART_STR "0[bootloader],"
#define ROMFILE_PART_STR "0[romfile],"

extern unsigned long flash_base;
#if defined(TCSUPPORT_CT_PON)
extern int nand_flash_avalable_size;
#endif

/* mtdpart_setup() parses into here */
static struct mtd_partition *ecnt_parts;
static int num_parts = 0;
static int has_remaining_part_flag = 0;
static uint64_t tclinux_part_size = 0;			
static uint64_t tclinux_part_offset = OFFSET_CONTINUOUS;	
static int kernel_part_index = -1;
static int kernel_slave_part_index = -1;
#endif

#define WINDOW_ADDR 0x1fc00000
#define WINDOW_SIZE 0x400000
#define BUSWIDTH 	2

#define TRX_LEN	256
#define KERNEL_PARTITION(a)	(a + TRX_LEN)  //Include trx header
#define ROOTFS_PARTITION(a)	((a + 0x10000) & ~(0x10000-1))
//#define ROOTFS_PARTITION(a)	(a)

#ifdef CONFIG_DUAL_IMAGE
#ifdef TCSUPPORT_FREE_BOOTBASE
#define FLAG_ADDR		(START_ADDR - 1)
#else
#define	FLAG_ADDR		0x8001ffff
#endif
#if defined(TCSUPPORT_CT_DUAL_IMAGE)
#if defined(TCSUPPORT_CT_PON)
#if defined(TCSUPPORT_RESERVEAREA_EXTEND)
#ifdef TCSUPPORT_NAND_FLASH//tony add
#define	MAIN_IMAGE_SIZE		(0x1000000)//16M for tclinux.bin
#else
#if defined(TCSUPPORT_CT_PON_CAU)
#define	MAIN_IMAGE_SIZE		(0x3A0000)
#else
#define	MAIN_IMAGE_SIZE		(0x7B0000)
#endif
#endif
#else
#define	MAIN_IMAGE_SIZE		(0x780000)
#endif

#ifdef TCSUPPORT_NAND_FLASH
#define	SLAVE_IMAGE_OFFSET	(0x1080000)
#else
#if defined(TCSUPPORT_BOOTROM_LARGE_SIZE)
#if defined(TCSUPPORT_RESERVEAREA_EXTEND)
#if defined(TCSUPPORT_CT_PON_CAU)
#define	SLAVE_IMAGE_OFFSET	(0x3D0000)
#else
#define	SLAVE_IMAGE_OFFSET	(0x7E0000)
#endif
#else
#define	SLAVE_IMAGE_OFFSET	(0x7b0000)
#endif
#endif
/*NAND_FLASH*/
#endif

#ifdef TCSUPPORT_NAND_FLASH
#define	SLAVE_IMAGE_SIZE	(0x1000000)
#else
#if defined(TCSUPPORT_RESERVEAREA_EXTEND)
#if defined(TCSUPPORT_CT_PON_CAU)
#define	SLAVE_IMAGE_SIZE	(0x3A0000)
#else
#define	SLAVE_IMAGE_SIZE	(0x3B0000)
#endif
#else
#define	SLAVE_IMAGE_SIZE	(0x400000)
#endif
/*NAND_FLASH*/
#endif

#else
#define	MAIN_IMAGE_SIZE		(0x680000)
#if defined(TCSUPPORT_BOOTROM_LARGE_SIZE)
#define	SLAVE_IMAGE_OFFSET	(0x6b0000)
#endif
#define	SLAVE_IMAGE_SIZE	(0x500000)
#endif
#else
#if defined(TCSUPPORT_CUC_DUAL_IMAGE)
#define	MAIN_IMAGE_SIZE		(0x700000)
#if defined(TCSUPPORT_BOOTROM_LARGE_SIZE)
#define	SLAVE_IMAGE_OFFSET	(0x730000)
#endif
#define	SLAVE_IMAGE_SIZE	(0x700000)
#else
#ifdef TCSUPPORT_NAND_BMT
#define	MAIN_IMAGE_SIZE		(0x1000000)//16M for tclinux.bin
#define	SLAVE_IMAGE_OFFSET	(0x7E0000)
#define	SLAVE_IMAGE_SIZE	(0x3B0000)
#else
#define	MAIN_IMAGE_SIZE		(0x500000)
#define	SLAVE_IMAGE_OFFSET	(0x520000)
#define	SLAVE_IMAGE_SIZE	(0x400000)
#endif
#endif
#endif
#endif

#if defined(TCSUPPORT_CT_OSGI)
#define JOYME_ADD_JFFS2		(0x2000000)
#define JOYME_ADD_YAFS2		(0x2400000)
#else
#define JOYME_ADD_JFFS2		(0)
#define JOYME_ADD_YAFS2		(0)
#endif

#ifdef TCSUPPORT_JFFS2_BLOCK
#define BLOCK_FOR_JFFS2		(0x400000)
#if defined(TCSUPPORT_RESERVEAREA_EXTEND)
#define	JFFS2_OFFSET		(0xb90000)
#else
#define	JFFS2_OFFSET		(0xbc0000)
#endif
#endif


/* frankliao added 20101215 */
extern int nand_flash_avalable_size;
extern unsigned long flash_base;
extern unsigned int (*ranand_read_byte)(unsigned long long);
extern unsigned int (*ranand_read_dword)(unsigned long long);
extern int __devinit ra_nand_init(void);
extern void __devinit ra_nand_remove(void);

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

#ifdef TCSUPPORT_MT7510_E1
#define READ_FLASH_BYTE(i)      ( (IS_NANDFLASH) ? \
                                                                ((ranand_read_byte != NULL) ? ranand_read_byte((i)) : -1) \
                                                                : (ReadSPIByte(i)) )


/* for read flash, frankliao added 20101216 */
#define READ_FLASH_DWORD(i)  ( (IS_NANDFLASH) ? \
                                                                ((ranand_read_dword != NULL) ? ranand_read_dword((i)) : -1) \
                                                                : (ReadSPIDWord(i)) )
#else
/* for read flash, frankliao added 20101216 */
#define READ_FLASH_DWORD(i)  ( (IS_NANDFLASH) ? \
								((ranand_read_dword != NULL) ? ranand_read_dword((i)) : -1) \
								: (ReadSPIDWord(i)) )
#endif
 #ifdef TCSUPPORT_MTD_ENCHANCEMENT
 #if 0
 #if defined ( TCSUPPORT_RESERVEAREA_1_BLOCK)
 #define BLOCK_NUM_FOR_RESERVEAREA 1
 #elif defined(TCSUPPORT_RESERVEAREA_2_BLOCK)
  #define BLOCK_NUM_FOR_RESERVEAREA 2
 #elif defined(TCSUPPORT_RESERVEAREA_3_BLOCK)
 #define BLOCK_NUM_FOR_RESERVEAREA 3
#else //TCSUPPORT_RESERVEAREA_4_BLOCK
 #define BLOCK_NUM_FOR_RESERVEAREA 4
#endif
#endif
	#if ((TCSUPPORT_RESERVEAREA_BLOCK != 1)&& (TCSUPPORT_RESERVEAREA_BLOCK != 2)&& (TCSUPPORT_RESERVEAREA_BLOCK !=3)&& (TCSUPPORT_RESERVEAREA_BLOCK !=4) && (TCSUPPORT_RESERVEAREA_BLOCK !=5)&& (TCSUPPORT_RESERVEAREA_BLOCK !=7))
#define BLOCK_NUM_FOR_RESERVEAREA 4
#else
 #define BLOCK_NUM_FOR_RESERVEAREA TCSUPPORT_RESERVEAREA_BLOCK
#endif
#endif

static struct mtd_info *tc3162_mtd_info;

static struct map_info tc3162_map = {
       .name = "tc3162",
       .size = WINDOW_SIZE,
       .bankwidth = BUSWIDTH,
       .phys = WINDOW_ADDR,
};
#ifdef TCSUPPORT_SQUASHFS_ADD_YAFFS
#define	SQUASHFS_ADD_YAFFS_SIZE (0x500000 + JOYME_ADD_YAFS2)	//5M
#endif
#ifndef TCSUPPORT_IS_FH_PON
#if !defined(TCSUPPORT_FWC_ENV)
static struct mtd_partition tc3162_parts[] = {
	{									 	/* First partition */
		  name 	     : "bootloader",	 	/* Bootloader section */
#ifdef TCSUPPORT_BOOTROM_LARGE_SIZE
		  size	  	 : 0x00020000, 		 	/* Size =128k */		
#else
		  size	  	 : 0x00010000, 		 	/* Size  */
#endif
		  offset     : 0				 	/* Offset from start of flash- location 0x0*/
//		  mask_flags : MTD_WRITEABLE	 	/* This partition is not writable */
	},
	{									 	/* Second partition */
		  name       : "romfile",			/* config filesystem section */
		  size	     :  0x00010000,		 	/* Size */
		  offset     : MTDPART_OFS_APPEND	/* Append after bootloader section */
	},
	{									 	/* Third partition */
		  name       : "kernel",		 	/* Kernel section */
#ifdef CONFIG_MTD_PURE_BRIDGE
		  size	     :  0x000a0000,		 	/* Size */
#else
		  size		 :	0x000d0000,
#endif
		/* 
		 * frank modify for nand flash support
		 * for nand flash, romfile partition is put in the last block,
		 * so the kernel partition cannot append after romfile section
		 */
		  offset     : 0x20000	/* Append after bootloader section */
//		  offset     : MTDPART_OFS_APPEND	/* Append after bootloader section */
	},
	{									 	/* Fourth partition */
		  name       : "rootfs", 		 	/* Root filesystem section */
		  size	     : MTDPART_SIZ_FULL, 	/* Occupy rest of flash */
		  offset     : MTDPART_OFS_APPEND 	/* Append after kernel section */
	},
	{
		  name       : "tclinux",            /* tclinux partition */	
		  size       : MTDPART_SIZ_FULL,    /* Occupy rest of flash */
		  offset     : 0x00020000   
	}
#ifdef CONFIG_DUAL_IMAGE
	,
	{
		  name       : "kernel_slave",            /* tclinux slave partition */	
#ifdef CONFIG_MTD_PURE_BRIDGE
		  size	     :  0x000a0000,		 	/* Size */
#else
		  size		 :	0x000d0000,
#endif	  
		   offset	 :  SLAVE_IMAGE_OFFSET   
	},
	{
		  name       : "rootfs_slave",            /* tclinux slave partition */	
		  size       : MTDPART_SIZ_FULL,    /* Occupy rest of flash */
		  offset     : MTDPART_OFS_APPEND   
	},
	{
		  name       : "tclinux_slave",            /* tclinux slave partition */	
		  size       : MTDPART_SIZ_FULL,    /* Occupy rest of flash */
		  offset 	 : SLAVE_IMAGE_OFFSET	
	}
#endif 
#ifdef TCSUPPORT_INIC_HOST
	,		
	{
		  name       : INIC_CLIENT_ROMFILE_NAME,
		  size       : INIC_CLIENT_ROMFILE_SIZE,
		  offset     : MTDPART_OFS_APPEND   
	}
#endif
#ifdef TCSUPPORT_JFFS2_BLOCK
   ,
	{
		 name		: "jffs2",			/*test partition */    
		 size		: BLOCK_FOR_JFFS2, //MTDPART_SIZ_FULL,    /* Occupy rest of flash */
		 offset 	: JFFS2_OFFSET   
	}
#endif
#if defined(TCSUPPORT_CT_OSGI)
#if defined(TCSUPPORT_SQUASHFS_ADD_YAFFS)
	 ,
	  {
		   name 	  : "yaffs",			/*nand yaffs partition */	 
		   size 	  : SQUASHFS_ADD_YAFFS_SIZE,   /* occupy 5M*/
		   offset	  : MTDPART_OFS_APPEND	 
	 }
#endif
#endif
 #ifdef TCSUPPORT_MTD_ENCHANCEMENT
 	,
	 {
		  name       : "reservearea",            /*test partition */	
		  size       : 0x00040000,   /* occupy the last 4 blocks */
		  offset     : MTDPART_OFS_APPEND   
	}
#endif
#if defined(TCSUPPORT_MULTI_BOOT) && !defined(TCSUPPORT_C1_ZY_SFU)
 	,
	 {
		  name       : "romd",            /*test partition */	
		  size       : 0x00010000,   /* occupy one block*/
		  offset     : MTDPART_OFS_APPEND   
	}
#endif
#if !defined(TCSUPPORT_CT_OSGI)
#if defined(TCSUPPORT_SQUASHFS_ADD_YAFFS)
	,
	 {
		  name       : "yaffs",            /*nand yaffs partition */	
		  size       : SQUASHFS_ADD_YAFFS_SIZE,   /* occupy 5M*/
		  offset     : MTDPART_OFS_APPEND   
	}
#endif
#endif

};
#else
static struct mtd_partition tc3162_parts[] = {
    {                                                   /* First partition */
        name    :   FH_NW_PRODUCT_BOOT_PARTITION,  /* Bootloader section */
        size    :   FH_NW_PRODUCT_BOOT_SIZE,            /* Size =128k */		
        offset  :   0                                   /* Offset from start of flash- location 0x0*/
    },
    {									 
        name    :   FH_NW_PRODUCT_KERNELA_PARTITION,	
        size    :   FH_NW_PRODUCT_KERNEL_SIZE ,		 
        offset  :   0x00100000	
    },
    {									 	
        name    :   FH_NW_PRODUCT_APP1A_PARTITION,		 	
        size    :   FH_NW_PRODUCT_APP1_SIZE,		 
        offset  :   MTDPART_OFS_APPEND	    
    },
    {									 	
        name    :   FH_NW_PRODUCT_APP2A_PARTITION,		 	
        size    :   FH_NW_PRODUCT_APP2_SIZE ,		 
        offset  :   MTDPART_OFS_APPEND	    
    },
    {									 
        name    :   FH_NW_PRODUCT_ROOTFSA_PARTITION, 		 
        size    :   FH_NW_PRODUCT_ROOTFS_SIZE , 
        offset  :   MTDPART_OFS_APPEND 
    },
    {
        name    :   FH_NW_PRODUCT_KERNELB_PARTITION,           	
        size    :   FH_NW_PRODUCT_KERNEL_SIZE ,    
        offset  :   MTDPART_OFS_APPEND   
    },
    {                                       
        name    :   FH_NW_PRODUCT_APP1B_PARTITION,            
        size    :   FH_NW_PRODUCT_APP1_SIZE,         
        offset  :   MTDPART_OFS_APPEND      
    },
    {                                       
        name    :   FH_NW_PRODUCT_APP2B_PARTITION,            
        size    :   FH_NW_PRODUCT_APP2_SIZE ,        
        offset  :   MTDPART_OFS_APPEND      
    },
    {                                    
        name    :   FH_NW_PRODUCT_ROOTFSB_PARTITION,       
        size    :   FH_NW_PRODUCT_ROOTFS_SIZE , 
        offset  :   MTDPART_OFS_APPEND 
    },
    {                                    
        name    :   FH_NW_PRODUCT_CONFIGURATION_PARTITION,       
        size    :   FH_NW_PRODUCT_CONFIGURATION_SIZE , 
        offset  :   MTDPART_OFS_APPEND 
    },
    {
        name    :   FH_NW_PRODUCT_PERSONALITY_PARM1_PARTITION,		
        size    :   FH_NW_PRODUCT_PERSONALITY_PARM1_SIZE ,
        offset  :   MTDPART_OFS_APPEND	
    },
    {
        name    :   FH_NW_PRODUCT_PERSONALITY_PARM2_PARTITION,        
        size    :   FH_NW_PRODUCT_PERSONALITY_PARM2_SIZE ,
        offset  :   MTDPART_OFS_APPEND  
    },
    {
        name    :   FH_NW_PRODUCT_PERSONALITY_PARM3_PARTITION,		
        size    :   FH_NW_PRODUCT_PERSONALITY_PARM3_SIZE ,
        offset  :   MTDPART_OFS_APPEND	
    },
    {
        name    :   FH_NW_PRODUCT_PERSONALITY_PARM4_PARTITION,		
        size    :   FH_NW_PRODUCT_PERSONALITY_PARM4_SIZE ,
        offset  :   MTDPART_OFS_APPEND	
    },
    {
        name    :   FH_NW_PRODUCT_PERSONALITY_PARM5_PARTITION,		
        size    :   FH_NW_PRODUCT_PERSONALITY_PARM5_SIZE ,
        offset  :   MTDPART_OFS_APPEND	
    },
    {
        name    :   FH_NW_PRODUCT_PERSONALITY_PARM6_PARTITION,		
        size    :   FH_NW_PRODUCT_PERSONALITY_PARM6_SIZE ,
        offset  :   MTDPART_OFS_APPEND	
    },
    {
        name    :   FH_NW_PRODUCT_PERSONALITY_PARM7_PARTITION,		
        size    :   FH_NW_PRODUCT_PERSONALITY_PARM7_SIZE ,
        offset  :   MTDPART_OFS_APPEND	
    },
    {
        name    :   FH_NW_PRODUCT_PERSONALITY_PARM8_PARTITION,		
        size    :   FH_NW_PRODUCT_PERSONALITY_PARM8_SIZE ,
        offset  :   MTDPART_OFS_APPEND	
    },
};
#endif
#else
static struct mtd_partition tc3162_parts[] = {
	{									 	/* First partition */
		  name 	     : "boot",	 	/* Bootloader section */
		  size	  	 : 0x200000, 		 	/* Size =128k */		
		  offset     : 0				 	/* Offset from start of flash- location 0x0*/
	},
	{									 
		  name       : "KernelA",		
		  size	     :  0x00400000 ,		 
		  offset     :  0x00200000	
	},
	
	{									 	
		  name       : "AppA",		 	
		  size	     :  0x1400000 ,		 
		  offset     :  MTDPART_OFS_APPEND	    
	},
	
	{									 
		  name       : "RootfsA", 		 
		  size	     :  0x1000000 , 
		  offset     :  MTDPART_OFS_APPEND 
	}

	,
	{
		  name       : "KernelB",           	
		  size       :  0x00400000 ,    
		  offset     :  MTDPART_OFS_APPEND   
	}	,
	{
		  name       : "AppB",          
		  size	     :  0x1400000 ,		 	 
		  offset	 :  MTDPART_OFS_APPEND   
	},
	{
		  name       : "RootfsB",            
		  size       : 0x1000000 ,   
		  offset     : MTDPART_OFS_APPEND   
	},
	{
		  name       : "ConfigurationA",            	
		  size       :  0x200000 ,   
		  offset 	 :  MTDPART_OFS_APPEND	
	},
	{
		 name		: "ConfigurationB",			
		 size		:  0xa00000 , 
		 offset 	:  MTDPART_OFS_APPEND   
	}	,
	 {
		  name       : "UserLocalCT",            	
		  size       : 0x400000 ,   
		  offset     : MTDPART_OFS_APPEND   
	},
	 {
		  name       : "Userdata",            	
		  size       : 0xe00000 ,  
		  offset     : MTDPART_OFS_APPEND   
	}

};
#endif
static int tc3162_parts_size = sizeof(tc3162_parts) / sizeof(tc3162_parts[0]);

static struct mtd_info *get_mtd_named(char *name)
{
	int i;
	struct mtd_info *mtd;

	for (i = 0; i < 32; i++) {
		mtd = get_mtd_device(NULL, i);
		if (mtd) {
			if (strcmp(mtd->name, name) == 0)
				return(mtd);
			put_mtd_device(mtd);
		}
	}
	return(NULL);
}

#if defined(TCSUPPORT_CT_DUAL_IMAGE) || defined(TCSUPPORT_CUC_DUAL_IMAGE) || defined(TCSUPPORT_NAND_BMT)
struct trx_header {
	unsigned long int magic;			/* "HDR0" */
	unsigned long int kernel_len;	/* Length of kernel image */
	unsigned long int crc32;			/* 32-bit CRC from flag_version to end of file */
	unsigned long int flag_version;	/* 0:15 flags, 16:31 version */
	unsigned long int reserved[12];	/* Reserved field of header */
};
#define TRX_MAGIC2	0x32524448	/* "for tclinux" */
#endif

/******************************************************************************
 Function:		tc3162_map_init
 Description:	It's used to init tc3162_map
 Input:		void
 Return:	    	0: Success,  -EIO: fail
******************************************************************************/
int tc3162_map_init(void){	
#ifdef TCSUPPORT_INIC_CLIENT
	u_int32_t ram_base;
#endif

#ifdef TCSUPPORT_ADDR_MAPPING
	/*add address mapping on 7510. Pork*/
	if(isMT751020 || isMT7505 || isEN751221){
		uint32 tmpVal;
		tmpVal = regRead32(0xbfb00038);
		tmpVal &= 0xffe0e0e0;
		tmpVal |= 0x80070f00;
		regWrite32(0xbfb00038,tmpVal);
		//VPint(0xbfb00038) |= 0x80070F00;
		#ifdef TCSUPPORT_INIC_CLIENT
			ram_base = 0x800000 * (1 << (((VPint(0xbfb0008c) >> 13) & 0x7) - 1))- INIC_CLIENT_RAM_SIMU_MAX_SIZE;
			printk("tc3162: iNIC simulated flash device 0x%08x at 0x%08x\n", INIC_CLIENT_RAM_SIMU_MAX_SIZE, ram_base);
			tc3162_map.virt = ioremap_nocache(ram_base, INIC_CLIENT_RAM_SIMU_MAX_SIZE);
			tc3162_map.phys = ram_base;
			tc3162_map.size = INIC_CLIENT_RAM_SIMU_MAX_SIZE;
		#else
			printk("tc3162: flash device 0x%08x at 0x%08x\n", 0x1000000, 0x1c000000);
			tc3162_map.virt = ioremap_nocache(0x1c000000, 0x1000000);
			tc3162_map.phys = 0x1c000000;
			tc3162_map.size = 0x1000000;
		#endif
		ioremap_nocache(WINDOW_ADDR, WINDOW_SIZE);
	}
	/*add 8M 16M flash support. shnwind*/
	else if (isTC3162U || isTC3182 || isRT65168 || isRT63165 || isRT63365 || isRT63260){
#else
	if (isTC3162U || isTC3182 || isRT65168 || isRT63165 || isRT63365 || isRT63260 || isMT751020 || isMT7505 || isEN751221){
#endif //TCSUPPORT_ADDR_MAPPING

//		header = (unsigned int *)0xb0020000;
		/*enable addr bigger than 4M support.*/
		VPint(0xbfb00038) |= 0x80000000;
		printk("tc3162: flash device 0x%08x at 0x%08x\n", 0x1000000, 0x10000000);
		tc3162_map.virt = ioremap_nocache(0x10000000, 0x1000000);
		tc3162_map.phys = 0x10000000;
		tc3162_map.size = 0x1000000;
		ioremap_nocache(WINDOW_ADDR, WINDOW_SIZE);
	}else{
	
//		header = (unsigned int *)0xbfc20000;
		printk("tc3162: flash device 0x%08x at 0x%08x\n", WINDOW_SIZE, WINDOW_ADDR);
		tc3162_map.virt = ioremap_nocache(WINDOW_ADDR, WINDOW_SIZE);

	}
	if (!tc3162_map.virt) {
   		printk("tc3162: Failed to ioremap\n");
		return -EIO;
	}

	simple_map_init(&tc3162_map);

	return 0;
}

static int tc3162_mtd_info_init(void){	
	#ifdef TCSUPPORT_INIC_CLIENT
		printk("MT75XX: INIC mode\n");
		tc3162_mtd_info = do_map_probe("map_ram", &tc3162_map);
	#else
		/* check if boot from SPI flash */
		if (IS_NANDFLASH) {
			tc3162_mtd_info = do_map_probe("nandflash_probe", &tc3162_map);
		} else if (IS_SPIFLASH) {
			tc3162_mtd_info = do_map_probe("spiflash_probe", &tc3162_map);
		} else {
			tc3162_mtd_info = do_map_probe("cfi_probe", &tc3162_map);
		}
	#endif

	if (!tc3162_mtd_info) {
		#ifdef TCSUPPORT_INIC_CLIENT
		printk("iNIC flash fail\n");
		#endif
		iounmap(tc3162_map.virt);
		return -ENXIO;
	}

  	tc3162_mtd_info->owner = THIS_MODULE;
	
	return 0;
}

static void tc3162_put_rootfs(void){
	struct mtd_info *mtd;
	
#ifdef CONFIG_DUAL_IMAGE
	char *bufaddr = (char*)FLAG_ADDR;
	const char flagvalue = 1;//not change!!because we use this flag to judge which image
	char tmp[8] = {0};
#endif	
	#ifdef CONFIG_DUAL_IMAGE
	//read the flag from last block to tell kernel use which image(main or slave)
	#if 0
	memcpy(tmp,(char*)(0xb0000000+tc3162_mtd_info->size -sizeof(char)),sizeof(char));	
	#endif
	memcpy(tmp,(char*)bufaddr,sizeof(char));
	if(flagvalue == tmp[0])
	{
		printk("\r\nrootfs_slave");
		mtd = get_mtd_named("rootfs_slave");
	}
	else
	{
	#endif
		printk("\r\nrootfs");
		mtd = get_mtd_named("rootfs");
	#ifdef CONFIG_DUAL_IMAGE
	}
	#endif	
	if (mtd) {
		ROOT_DEV = MKDEV(MTD_BLOCK_MAJOR, mtd->index);
		put_mtd_device(mtd);
	}
}

#ifndef TCSUPPORT_MTD_PARTITIONS_CMDLINE
int tc3162_add_partitions(void){
	unsigned int *header;
	unsigned int addr;
	#if defined(TCSUPPORT_BOOTROM_LARGE_SIZE)
		u_int32_t tclinux_flash_offset = 0x30000;
	#else
		u_int32_t tclinux_flash_offset = 0x20000;
	#endif
	
	#ifdef TCSUPPORT_SQUASHFS_ADD_YAFFS
		u_int32_t nand_yaffs_size = SQUASHFS_ADD_YAFFS_SIZE;
	#else
		u_int32_t nand_yaffs_size = 0;
	#endif
	
	#if defined(CONFIG_DUAL_IMAGE) || defined(TCSUPPORT_MTD_ENCHANCEMENT) || defined(TCSUPPORT_MULTI_BOOT) || defined(TCSUPPORT_NAND_BADBLOCK_CHECK)
		int i = 0;
	#endif
	#ifdef CONFIG_DUAL_IMAGE
		char *bufaddr = (char*)FLAG_ADDR;
		const char flagvalue = 1;//not change!!because we use this flag to judge which image
		char tmp[8] = {0};
	#endif
	//#if defined(CONFIG_DUAL_IMAGE) && (defined(TCSUPPORT_MTD_ENCHANCEMENT) ||defined(TCSUPPORT_MULTI_BOOT))
#if defined(CONFIG_DUAL_IMAGE)
#ifdef TCSUPPORT_DUAL_IMAGE_ENHANCE	
		u_int32_t tclinux_slave_offset = offset+tclinux_flash_offset;
#else
		u_int32_t tclinux_slave_offset = MAIN_IMAGE_SIZE+tclinux_flash_offset;	
#endif
		u_int32_t tclinux_slave_size = 0;
#endif
#if defined(TCSUPPORT_MTD_ENCHANCEMENT) || defined(TCSUPPORT_MULTI_BOOT)
		u_int32_t tclinux_size = 0;
#endif
#if defined(TCSUPPORT_CT_DUAL_IMAGE) || defined(TCSUPPORT_CUC_DUAL_IMAGE) || defined(TCSUPPORT_NAND_BMT)
		struct trx_header *trx = NULL;
		char *trx_addr;
		unsigned int magic;
#endif
	
  #ifdef TCSUPPORT_NAND_RT63368
		header = (unsigned int *)(flash_base + 0x40000);
  #elif defined(TCSUPPORT_NAND_BADBLOCK_CHECK)	
		header = (unsigned int *)(flash_base + 0x280000);
  #else
		/* frankliao added 20101223 */
  #ifdef TCSUPPORT_INIC_CLIENT
		header = (unsigned int *)(flash_base + INIC_CLIENT_BOOTLOADER_SIZE + INIC_CLIENT_ROMFILE_SIZE);
  #else
		header = (unsigned int *)(flash_base + tclinux_flash_offset);
  #endif
  #endif

	#ifdef TCSUPPORT_BOOTROM_LARGE_SIZE
	if (IS_NANDFLASH) {
		if(tc3162_mtd_info->erasesize >= 0x20000){
			/*tclinux offset is 0x80000 for 128k&256k block size*/
			//tclinux_flash_offset = tc3162_mtd_info->erasesize*2;
			tclinux_flash_offset = 0x40000*2;
			
#if defined(CONFIG_DUAL_IMAGE)
#ifdef TCSUPPORT_DUAL_IMAGE_ENHANCE	
			tclinux_slave_offset = offset+tclinux_flash_offset;
#else
			tclinux_slave_offset = MAIN_IMAGE_SIZE+tclinux_flash_offset;	
#endif
#endif
			header = (unsigned int *)(flash_base + tclinux_flash_offset);
		}
	}
	#endif

    #if defined(TCSUPPORT_NAND_BADBLOCK_CHECK) || defined(TCSUPPORT_NAND_RT63368)
    for(i= 0; i < tc3162_parts_size; i++)
    {
        if(!strcmp(tc3162_parts[i].name, "bootloader"))
        {
            tc3162_parts[i].size = 0x20000;
        }

        if(!strcmp(tc3162_parts[i].name, "romfile"))
        {
            tc3162_parts[i].size = 0x20000;
            #ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
            tc3162_parts[i].offset = 0xe0000;
            #else
            tc3162_parts[i].offset = 0x20000;
            #endif
        }

    }
    #endif

	#ifdef CONFIG_DUAL_IMAGE
	for(i= 0; i < tc3162_parts_size; i++)
	{
		if(!strcmp(tc3162_parts[i].name,"kernel"))
		{
			addr = READ_FLASH_DWORD((unsigned long long)((unsigned int)header + 20 * sizeof(unsigned int)));  
			tc3162_parts[i].size = KERNEL_PARTITION( addr );
//			tc3162_parts[i].size = KERNEL_PARTITION(header[20]);
			addr = READ_FLASH_DWORD((unsigned long long)((unsigned int)header + 21 * sizeof(unsigned int)));   
			tc3162_parts[i+1].size = ROOTFS_PARTITION( addr );
//			tc3162_parts[i+1].size = ROOTFS_PARTITION(header[21]);
			#if defined(TCSUPPORT_MTD_ENCHANCEMENT) || defined(TCSUPPORT_MULTI_BOOT)
			tclinux_size = tc3162_parts[i].size+tc3162_parts[i+1].size;
			#endif

			#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
            tc3162_parts[i].offset = 0x280000;
            tc3162_parts[i+2].offset = tc3162_parts[i].offset;
            #elif defined(TCSUPPORT_NAND_RT63368)
            tc3162_parts[i].offset = 0x40000;
            tc3162_parts[i+2].offset = tc3162_parts[i].offset;
			#endif
		}
		if(!strcmp(tc3162_parts[i].name,"kernel_slave"))
		{
#ifdef TCSUPPORT_DUAL_IMAGE_ENHANCE  
			unsigned int *header_slave = (unsigned int *)(flash_base + tclinux_flash_offset + offset);
			tc3162_parts[i].offset = offset + tclinux_flash_offset;
			tc3162_parts[i+2].offset = offset + tclinux_flash_offset;

#elif defined(TCSUPPORT_NAND_BADBLOCK_CHECK)
            unsigned int *header_slave = (unsigned int *)(flash_base + 0x2280000);
            tc3162_parts[i].offset = 0x2280000;
            tc3162_parts[i+2].offset = tc3162_parts[i].offset;
#else
			unsigned int *header_slave = (unsigned int *)(flash_base + tclinux_flash_offset + MAIN_IMAGE_SIZE);
//			unsigned int *header_slave = (unsigned int *)(0xb0020000+0x500000);
			tc3162_parts[i].offset = MAIN_IMAGE_SIZE + tclinux_flash_offset;
			tc3162_parts[i+2].offset = MAIN_IMAGE_SIZE + tclinux_flash_offset;
#endif
//			tc3162_parts[i].size = KERNEL_PARTITION(header_slave[20]);
//			tc3162_parts[i+1].size = ROOTFS_PARTITION(header_slave[21]);
			addr = READ_FLASH_DWORD((unsigned long long)((unsigned int)header_slave + 20 * sizeof(unsigned int)));
//			tc3162_parts[i].size = KERNEL_PARTITION(header_slave[20]);
			tc3162_parts[i].size = KERNEL_PARTITION( addr );

			addr = READ_FLASH_DWORD((unsigned long long)((unsigned int)header_slave + 21 * sizeof(unsigned int)));		
			tc3162_parts[i+1].size = ROOTFS_PARTITION( addr );
//			tc3162_parts[i+1].size = ROOTFS_PARTITION(header_slave[21]);

		 #if defined(TCSUPPORT_MTD_ENCHANCEMENT) || defined(TCSUPPORT_MULTI_BOOT)
			tclinux_slave_offset = tc3162_parts[i].offset;
		 	tclinux_slave_size = tc3162_parts[i].size + tc3162_parts[i+1].size;
		 #endif

#if defined(TCSUPPORT_CT_DUAL_IMAGE) || defined(TCSUPPORT_CUC_DUAL_IMAGE) || defined(TCSUPPORT_NAND_BMT)
		trx = (struct trx_header *)header_slave;
		trx_addr = &(trx->magic);
		magic = READ_FLASH_DWORD(trx_addr);
		if(magic !=TRX_MAGIC2){
			tclinux_slave_offset = SLAVE_IMAGE_OFFSET;
		 	tclinux_slave_size = SLAVE_IMAGE_SIZE;
			tc3162_parts[i].size = 0;	//no slave image
			tc3162_parts[i+1].size = 0; //no slave image
		}
#endif

		}
		if (IS_NANDFLASH) {	
			/* frankliao enhance 20110112, for nand flash romfile  */
			if(!strcmp(tc3162_parts[i].name,"tclinux_slave")) {
				/* frankliao add 20110112, for 128K block size nand flash romfile */
				if (tc3162_mtd_info->erasesize >= 0x20000) {
				#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
                    tc3162_parts[i].size = 0x2000000;
				#else
					tc3162_parts[i].size = tc3162_mtd_info->size - nand_yaffs_size - 6*tc3162_mtd_info->erasesize - tclinux_slave_offset;
				#endif
				} else {
					tc3162_parts[i].size = tc3162_mtd_info->size - tclinux_slave_offset;
				}	
			}	
	
			/* 
			 * frankliao enhance 20110112 
			 * the tclinux partition start from 0x20000, end at tclinux_slave_offset 
			 */
			if(!strcmp(tc3162_parts[i].name,"tclinux")) {
			#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
			    tc3162_parts[i].size = 0x2000000;
			#else
				tc3162_parts[i].size = tclinux_slave_offset - tclinux_flash_offset;
			#endif
			}	
		}		
	}
	#else 
//	tc3162_parts[2].size = KERNEL_PARTITION(header[20]);
//	tc3162_parts[2].size = KERNEL_PARTITION(header[21]);
	addr = READ_FLASH_DWORD((unsigned long long)((unsigned int)header + 20 * sizeof(unsigned int)));
	tc3162_parts[2].size = KERNEL_PARTITION( addr );
	addr = READ_FLASH_DWORD((unsigned long long)((unsigned int)header + 21 * sizeof(unsigned int)));
	tc3162_parts[3].size = ROOTFS_PARTITION( addr );

	#if defined(TCSUPPORT_NAND_BADBLOCK_CHECK) || defined(TCSUPPORT_NAND_RT63368)
	#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
        tc3162_parts[2].offset = 0x280000;
        #else
        tc3162_parts[2].offset = 0x40000;
        #endif
        tc3162_parts[4].offset = tc3162_parts[2].offset;
	#endif

	if (IS_NANDFLASH) {
		/* frankliao added 20110112, for 128K block size NAND Flash */
		if (tc3162_mtd_info->erasesize >= 0x20000) {
		#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
		    tc3162_parts[4].size = 0x2000000;
		#else
			/* the last block store nand flash romfile */
			tc3162_parts[4].size = tc3162_mtd_info->size - nand_yaffs_size - 6*tc3162_mtd_info->erasesize - tclinux_flash_offset;
		#endif
		}
	}

	#if defined(TCSUPPORT_MTD_ENCHANCEMENT) || defined(TCSUPPORT_MULTI_BOOT)
	tclinux_size = tc3162_parts[2].size + tc3162_parts[3].size;
	#endif
	#endif 

//use last 4 block as reserve area for storing data(for example:syslog,backupromfile,and so on)
 #ifdef TCSUPPORT_MTD_ENCHANCEMENT
 	for(i= 0; i < tc3162_parts_size; i++)
	{
		if(!strcmp(tc3162_parts[i].name,"reservearea"))
		{
			/* 
			 * frankliao modify 20110112 
			 * 64K block size SPI Flash & 128K block size NAND Flash 
			 */
			if (tc3162_mtd_info->erasesize >= 0x10000) {
			#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
                tc3162_parts[i].offset = 0x7c80000;
                tc3162_parts[i].size = 0x380000;
			#else
			#ifdef TCSUPPORT_NAND_FLASH
#if defined(TCSUPPORT_CT_PON)
			if (tc3162_mtd_info->erasesize >= 0x20000){ /* 128*0x20000 is used for bmt */
			#if 1//
				tc3162_parts[i].offset = nand_flash_avalable_size - BLOCK_NUM_FOR_RESERVEAREA * 0x40000;
			#else
				tc3162_parts[i].offset = tc3162_mtd_info->size - BLOCK_NUM_FOR_RESERVEAREA * 0x40000;
			#endif
				tc3162_parts[i].size = BLOCK_NUM_FOR_RESERVEAREA * 0x40000;
			}
#else
				tc3162_parts[i].offset = tc3162_mtd_info->size - nand_yaffs_size - BLOCK_NUM_FOR_RESERVEAREA*( tc3162_mtd_info->erasesize);
				tc3162_parts[i].size = BLOCK_NUM_FOR_RESERVEAREA*(tc3162_mtd_info->erasesize);
#endif
			#else
				tc3162_parts[i].offset = tc3162_mtd_info->size - nand_yaffs_size - BLOCK_NUM_FOR_RESERVEAREA*( tc3162_mtd_info->erasesize);
				tc3162_parts[i].size = BLOCK_NUM_FOR_RESERVEAREA*(tc3162_mtd_info->erasesize);
			#endif
			#endif
			/* 16 block size NAND Flash */
			} else {
				tc3162_parts[i].offset = tc3162_mtd_info->size - nand_yaffs_size - BLOCK_NUM_FOR_RESERVEAREA*(0x10000);
				tc3162_parts[i].size = BLOCK_NUM_FOR_RESERVEAREA*(0x10000);
			}
		}

  #ifndef TCSUPPORT_NAND_BADBLOCK_CHECK

		#ifdef CONFIG_DUAL_IMAGE
		memcpy(tmp,(char*)bufaddr,sizeof(char));
		if(flagvalue != tmp[0])//use main image
		{
		#endif
		if(!strcmp(tc3162_parts[i].name,"tclinux"))
		{
			#ifdef CONFIG_DUAL_IMAGE
			tc3162_parts[i].size = tclinux_slave_offset -tclinux_flash_offset ; //reserve the last 4 blocks
			#else
			
			/* 
			 * frankliao modify 201100112 
			 * 64K block size SPI Flash & 128K block size NAND Flash
			 */
			if (tc3162_mtd_info->erasesize == 0x10000) {
				tc3162_parts[i].size = tc3162_mtd_info->size - BLOCK_NUM_FOR_RESERVEAREA*( tc3162_mtd_info->erasesize) - tclinux_flash_offset; //reserve the last 4 blocks
			} 
			else if (tc3162_mtd_info->erasesize >= 0x20000) {
				tc3162_parts[i].size = tc3162_mtd_info->size - nand_yaffs_size - BLOCK_NUM_FOR_RESERVEAREA*( tc3162_mtd_info->erasesize) - tclinux_flash_offset; 
			/* 16K block size NAND Flash */
			} else {
				tc3162_parts[i].size = tc3162_mtd_info->size - BLOCK_NUM_FOR_RESERVEAREA*(0x10000) - tclinux_flash_offset;
			}
			#endif
			if(tclinux_size > tc3162_parts[i].size)
			{
				printk("\r\ntclinux size is beyond the limit!!");
				return -1;
			}
		}
		#ifdef CONFIG_DUAL_IMAGE
		}
		if(!strcmp(tc3162_parts[i].name,"tclinux_slave"))
		{
			/* 
			 * frankliao modify 201100112 
			 * 64K block size SPI Flash & 128K block size NAND Flash
			 */
			if (tc3162_mtd_info->erasesize == 0x10000) {
				tc3162_parts[i].size = tc3162_mtd_info->size - nand_yaffs_size - BLOCK_NUM_FOR_RESERVEAREA*( tc3162_mtd_info->erasesize) -tclinux_slave_offset; //reserve the last 4 blocks
			} else if (tc3162_mtd_info->erasesize >= 0x20000) {
			#ifdef TCSUPPORT_NAND_FLASH
#if defined(TCSUPPORT_CT_PON)
				tc3162_parts[i].size = MAIN_IMAGE_SIZE;
#else
				tc3162_parts[i].size = tc3162_mtd_info->size - nand_yaffs_size - BLOCK_NUM_FOR_RESERVEAREA*( tc3162_mtd_info->erasesize) -tclinux_slave_offset; 
#endif
			#else
				tc3162_parts[i].size = tc3162_mtd_info->size - nand_yaffs_size - 6*( tc3162_mtd_info->erasesize) -tclinux_slave_offset; 
			#endif
			/* 16K block size NAND Flash */
			} else {
				tc3162_parts[i].size = tc3162_mtd_info->size -BLOCK_NUM_FOR_RESERVEAREA*(0x10000) - tclinux_slave_offset;
			}
		#ifdef TCSUPPORT_JFFS2_BLOCK
			#ifndef TCSUPPORT_NAND_FLASH)
#if !defined(TCSUPPORT_CT_PON)
			tc3162_parts[i].size -= BLOCK_FOR_JFFS2;
#endif
			#endif
		#endif
			if(tclinux_slave_size > tc3162_parts[i].size)
			{
				printk("\r\ntclinux_slave size is beyond the limit!!");
									
#if !defined(TCSUPPORT_CY_PON)
			//	return -1;
#endif
			}
		}
		#endif
		#endif
		#ifdef TCSUPPORT_NAND_FLASH
#if defined(TCSUPPORT_CT_PON)
		#ifdef TCSUPPORT_JFFS2_BLOCK
		if(!strcmp(tc3162_parts[i].name,"jffs2"))
		{
			tc3162_parts[i].offset = tclinux_slave_offset + MAIN_IMAGE_SIZE;
			tc3162_parts[i].size = BLOCK_FOR_JFFS2;
		}
		#endif
#endif
		#endif		
		#ifdef TCSUPPORT_SQUASHFS_ADD_YAFFS
  		if(!strcmp(tc3162_parts[i].name,"yaffs"))
  		{
#if defined(TCSUPPORT_CT_OSGI)
  			tc3162_parts[i].offset = tclinux_slave_offset + MAIN_IMAGE_SIZE;
#else
  			tc3162_parts[i].offset = tc3162_mtd_info->size - nand_yaffs_size; 
#endif
  		}
  #endif
		
 	} 	
 #endif

	#if defined(TCSUPPORT_MULTI_BOOT)
 	for(i= 0; i < tc3162_parts_size; i++)
	{
		#if !defined(TCSUPPORT_C1_ZY_SFU)
		if(!strcmp(tc3162_parts[i].name,"romd"))
		{
			if (tc3162_mtd_info->erasesize >= 0x10000) {
				tc3162_parts[i].offset = tc3162_mtd_info->size -5*( tc3162_mtd_info->erasesize);
				tc3162_parts[i].size = (tc3162_mtd_info->erasesize);
			} else {
				tc3162_parts[i].offset = tc3162_mtd_info->size -5*(0x10000);
				tc3162_parts[i].size = 0x10000;
			}
		}
		#endif


		#ifdef CONFIG_DUAL_IMAGE
		memcpy(tmp,(char*)bufaddr,sizeof(char));
		if(flagvalue != tmp[0])//use main image
		{
		#endif
		
			if(!strcmp(tc3162_parts[i].name,"tclinux"))
			{
				#ifdef CONFIG_DUAL_IMAGE
				tc3162_parts[i].size = tclinux_slave_offset -tclinux_flash_offset ; 
				#else				
				if (tc3162_mtd_info->erasesize >= 0x20000) {
					tc3162_parts[i].size = tc3162_mtd_info->size -6*( tc3162_mtd_info->erasesize) -tclinux_flash_offset; 
				} else if (tc3162_mtd_info->erasesize == 0x10000) {
					tc3162_parts[i].size = tc3162_mtd_info->size -5*( tc3162_mtd_info->erasesize) -tclinux_flash_offset; 
				} else {
					tc3162_parts[i].size = tc3162_mtd_info->size -5*(0x10000) -tclinux_flash_offset; 
				}
				#endif
		
				if(tclinux_size > tc3162_parts[i].size)
				{
					printk("tclinux size is beyond the limit!!\r\n");					
					return -1;
				}
			}

		
		#ifdef CONFIG_DUAL_IMAGE
		}
		if(flagvalue == tmp[0])//use slave image
		{
			if(!strcmp(tc3162_parts[i].name,"tclinux_slave"))
			{
				if (tc3162_mtd_info->erasesize >= 0x20000) {
					tc3162_parts[i].size = tc3162_mtd_info->size -6*tc3162_mtd_info->erasesize -tclinux_slave_offset;
				} else if (tc3162_mtd_info->erasesize == 0x10000) {
					tc3162_parts[i].size = tc3162_mtd_info->size -5*tc3162_mtd_info->erasesize -tclinux_slave_offset;
				} else {
					tc3162_parts[i].size = tc3162_mtd_info->size -5*(0x10000) -tclinux_slave_offset;
				}
				if(tclinux_slave_size > tc3162_parts[i].size)
				{
					printk("tclinux_slave size is beyond the limit!!\r\n");
#if !defined(TCSUPPORT_CY_PON)
				//	return -1;
#endif
				}
			}
		}
		#endif
 	} 	
#endif

#if !defined(TCSUPPORT_NAND_BADBLOCK_CHECK) && !defined(TCSUPPORT_NAND_RT63368)
	/*Reset the kernel partition offset*/
	tc3162_parts[2].offset = tclinux_flash_offset;
	/*Reset the tclinux partition offset*/
	tc3162_parts[4].offset = tclinux_flash_offset;

	/* frank added 20110111 for 128K block size NAND Flash*/
	if (tc3162_mtd_info->erasesize >= 0x20000) {
		#ifdef TCSUPPORT_BOOTROM_LARGE_SIZE
		/* reset the bootloader partition size to 0x40000 which is a blocksize of 128K&256k NAND Flash */
		tc3162_parts[0].size = 0x40000; 
		/*romfile offset is 0x40000 for 128k&256k block size*/
		//tc3162_parts[1].offset = tc3162_mtd_info->erasesize;
		tc3162_parts[1].offset = 0x40000;

		/* reset the romfile partition size */
		tc3162_parts[1].size = 0x40000;
		#else
		/* reset the bootloader partition size to 0x20000 which is a blocksize of 128K NAND Flash */
		tc3162_parts[0].size = tc3162_mtd_info->erasesize; 
		/* 
		 * reset the romfile partition offset. 
		 * the romfile partition starts from the last block address
		 */
		tc3162_parts[1].offset = tc3162_mtd_info->size - nand_yaffs_size - 6*tc3162_mtd_info->erasesize;
		/* reset the romfile partition size */
		tc3162_parts[1].size = tc3162_mtd_info->erasesize;
		#endif
	} 
#endif	

#ifdef TCSUPPORT_INIC_HOST
	for(i = 0; i < tc3162_parts_size; i++) {	
		if(!strcmp(tc3162_parts[i].name,INIC_CLIENT_ROMFILE_NAME)) {
			tc3162_parts[i - 1].size -= INIC_CLIENT_ROMFILE_SIZE;
			tc3162_parts[i].offset = tc3162_parts[i - 1].offset + tc3162_parts[i - 1].size;
		}
	}
#endif


	add_mtd_partitions(tc3162_mtd_info, tc3162_parts, tc3162_parts_size);
	return 0;
}
#else
unsigned long long tc3162_memparse(const char *ptr, char **retptr, unsigned int blocksize)
{
	char *endptr;	/* local pointer to end of parsed string */

	unsigned long long ret = simple_strtoull(ptr, &endptr, 0);

	switch (*endptr) {
	case 'G':
	case 'g':
		ret <<= 10;
	case 'M':
	case 'm':
		ret <<= 10;
	case 'K':
	case 'k':
		ret <<= 10;
		endptr++;
		break;
	case 'B':
	case 'b':
		ret *=blocksize;
		endptr++;
		break;
	default:
		break;
	}

	if (retptr)
		*retptr = endptr;

	return ret;
}

static void tc3162_newpart_set_other_parts(struct mtd_partition *part, char *name, int index, unsigned int blocksize){
	if(!part || !name){
		printk("tc3162_newpart_set_other_parts fail, input NULL\n");
		return;
	}
	if(!strcmp(name, TCLINUX)){
		part[0].name = KERNEL_PART;
		part[0].size = SIZE_TO_GET;
		part[0].offset = OFFSET_CONTINUOUS;
		part[1].name = ROOTFS_PART;
		part[1].size = SIZE_TO_GET;
		part[1].offset = OFFSET_CONTINUOUS;
		kernel_part_index = index;
	}
	else if(!strcmp(name, TCLINUX_SLAVE)){
		part[0].name = KERNEL_SLAVE_PART;
		part[0].size = SIZE_TO_GET;
		part[0].offset = OFFSET_CONTINUOUS;
		part[1].name = ROOTFS_SLAVE_PART;
		part[1].size = SIZE_TO_GET;
		part[1].offset = OFFSET_CONTINUOUS;
		kernel_slave_part_index = index;
	}
	else if(!strcmp(name, RESERVEAREA)){
#ifdef TCSUPPORT_NAND_FLASH
#if defined(TCSUPPORT_CT_PON)
		if (IS_NANDFLASH) {
			blocksize = 0x40000;
		}
#endif
#endif
		part->name = RESERVEAREA;
		part->offset = OFFSET_BACK_FORWARD;
		part->size = (blocksize*TCSUPPORT_RESERVEAREA_BLOCK);
	}
}
/*
 * Parse one partition definition for an MTD. Since there can be many
 * comma separated partition definitions, this function calls itself
 * recursively until no more partition definitions are found. Nice side
 * effect: the memory to keep the mtd_partition structs and the names
 * is allocated upon the last definition being found. At that point the
 * syntax has been verified ok.
 */
static struct mtd_partition * tc3162_newpart(char *s, char **retptr, int *num_parts,
                                      int this_part, unsigned char **extra_mem_ptr,
                                      int extra_mem_size, unsigned int blocksize)
{
	struct mtd_partition *parts;
	unsigned long size = SIZE_TO_GET;
	unsigned long offset = OFFSET_CONTINUOUS;
	char *name;
	int name_len;
	unsigned char *extra_mem;
	char delim;
	unsigned int mask_flags;

	/* fetch the partition size */
	if (*s == '-'){	
		if(has_remaining_part_flag == 0){
			/* assign all remaining space to this partition */
			size = SIZE_REMAINING;
			has_remaining_part_flag = 1;
			s++;
		}
		else{
			printk(KERN_ERR ERRP "no fill-up partitions allowed after already having a fill-up partition\n");
			return NULL;
		}
	}
	
	else{
		size = tc3162_memparse(s, &s, blocksize);
		if(size == 0)
			size = SIZE_TO_GET;
		if (size < PAGE_SIZE)
		{
			printk(KERN_ERR ERRP "partition size too small (%lx)\n", size);
			return NULL;
		}
	}

	/* fetch partition name and flags */
	mask_flags = 0; /* this is going to be a regular partition */
	delim = 0;
	
        /* now look for name */
	if (*s == '['){
		delim = ']';
	}

	if (delim){
		char *p;

	    	name = ++s;
		p = strchr(name, delim);
		if (!p)
		{
			printk(KERN_ERR ERRP "no closing %c found in partition name\n", delim);
			return NULL;
		}
		name_len = p - name;
		s = p + 1;
	}
	else{
	    	name = NULL;
		name_len = 13; /* Partition_000 */
	}

	/* record name length for memory allocation later */
	extra_mem_size += name_len + 1;

	/* offset type is append */
        if (strncmp(s, "a", 1) == 0){
		offset = OFFSET_CONTINUOUS;
		s += 1;
        }

        /* offset type is back forward*/
        if (strncmp(s, "end", 3) == 0){
		offset = OFFSET_BACK_FORWARD;
		s += 3;
        }

	/* test if more partitions are following */
	if (*s == ',')
	{
		if(!strncmp(name, TCLINUX, name_len)){
			this_part += 2;
			extra_mem_size +=strlen(KERNEL_PART)+strlen(ROOTFS_PART)+2;
		}
		else if(!strncmp(name, TCLINUX_SLAVE, name_len)){
			this_part += 2;
			extra_mem_size +=strlen(KERNEL_SLAVE_PART)+strlen(ROOTFS_SLAVE_PART)+2;
		}
		
		/* more partitions follow, parse them */
		parts = tc3162_newpart(s + 1, &s, num_parts, this_part + 1,
				&extra_mem, extra_mem_size, blocksize);
		if (!parts)
			return NULL;
	}
	else
	{	/* this is the last partition: allocate space for all */
		int alloc_size;

		if(!strncmp(name, TCLINUX, name_len)){
			this_part += 2;
			extra_mem_size +=strlen(KERNEL_PART)+strlen(ROOTFS_PART)+2;
		}
		else if(!strncmp(name, TCLINUX_SLAVE, name_len)){
			this_part += 2;
			extra_mem_size +=strlen(KERNEL_SLAVE_PART)+strlen(ROOTFS_SLAVE_PART)+2;
		}
		
		*num_parts = this_part + 2; /*add reservearea partition*/
		extra_mem_size += strlen(RESERVEAREA)+1;
		alloc_size = *num_parts * sizeof(struct mtd_partition) +
			     extra_mem_size;
		parts = kzalloc(alloc_size, GFP_KERNEL);
		if (!parts){
			printk(KERN_ERR ERRP "out of memory\n");
			return NULL;
		}
		extra_mem = (unsigned char *)(parts + *num_parts);
	}
	/* enter this partition (offset will be calculated later if it is zero at this point) */
	parts[this_part].size = size;
	parts[this_part].offset = offset;
	parts[this_part].mask_flags = mask_flags;
	if (name){
		strlcpy(extra_mem, name, name_len + 1);
	}
	else{
		sprintf(extra_mem, "Partition_%03d", this_part);
	}
	parts[this_part].name = extra_mem;
	extra_mem += name_len + 1;
	if(!strcmp(parts[this_part].name, TCLINUX)){
		tc3162_newpart_set_other_parts(&parts[this_part-2],TCLINUX, this_part-2, blocksize);
	}
	else if(!strcmp(parts[this_part].name, TCLINUX_SLAVE)){
		tc3162_newpart_set_other_parts(&parts[this_part-2],TCLINUX_SLAVE, this_part-2, blocksize);
	}
	if(this_part == (*num_parts -2)){
		tc3162_newpart_set_other_parts(&parts[*num_parts -1], RESERVEAREA, *num_parts -1, blocksize);
	}
	printk("partition %d: name <%s>, offset %llx, size %llx, mask flags %x\n",this_part, parts[this_part].name,
	     parts[this_part].offset, parts[this_part].size,parts[this_part].mask_flags);

	/* return (updated) pointer to extra_mem memory */
	if (extra_mem_ptr)
	  *extra_mem_ptr = extra_mem;

	/* return (updated) pointer command line string */
	*retptr = s;

	/* return partition table */
	return parts;
}

/******************************************************************************
 Function:		
 Description:	Parse the command line.
 Input:		
 Return:	    	
******************************************************************************/
static int tc3162_mtdpart_setup(char *s, unsigned int blocksize)
{
	char cmdline[STR_LEN];
	unsigned char *extra_mem;
	char *p;

	if(s == NULL){
		panic("tc3162_mtdpart_setup(), mtd partition cmdline is NULL\n");
	}

	if(strlen(s) + strlen(BOOTLOADER_PART_STR)+strlen(ROMFILE_PART_STR) > STR_LEN){
		panic("tc3162_mtdpart_setup(), string length outof size\n");
	}
	
	tc3162_check_mtdpart_str(cmdline, s);
	
	p = cmdline;
	printk("\nparsing <%s>\n", p);

	/*
	 * parse one mtd. have it reserve memory for the
	 * struct cmdline_mtd_partition and the mtd-id string.
	 */
	ecnt_parts = tc3162_newpart(p,		/* cmdline */
			&s,		/* out: updated cmdline ptr */
			&num_parts,	/* out: number of parts */
			0,		/* first partition */
			&extra_mem, /* out: extra mem */
			 0, blocksize);
	if(!ecnt_parts)
	{
		/*
		 * An error occurred. We're either:
		 * a) out of memory, or
		 * b) in the middle of the partition spec
		 * Either way, this mtd is hosed and we're
		 * unlikely to succeed in parsing any more
		 */
		 return 0;
	 }

	return 1;
}

void tc3162_set_kernel_rootfs_part(void){
	unsigned int addr;
	unsigned int *header;
	unsigned int *header_slave;
	int i;	
	uint64_t tclinux_real_size = 0;
	uint64_t tclinux_slave_real_size = 0;

	if(kernel_part_index < 0)
		return;
	
	if(tclinux_part_offset != OFFSET_CONTINUOUS){
		header = (unsigned int *)(flash_base + tclinux_part_offset);
	}
	else{	
		panic("tc3162_set_kernel_rootfs_part(), tclinux partition offset error\n");
	}
	
	if((tclinux_part_size !=0) && (tclinux_part_size != SIZE_REMAINING)){
		header_slave = (unsigned int *)(flash_base + tclinux_part_offset + tclinux_part_size);
	}
	else{
		panic("tc3162_set_kernel_rootfs_part(), tclinux partition size error\n");		
	}

	
	addr = READ_FLASH_DWORD((unsigned long long)((unsigned int)header + 20 * sizeof(unsigned int)));
	ecnt_parts[kernel_part_index].size = KERNEL_PARTITION(addr);
	addr = READ_FLASH_DWORD((unsigned long long)((unsigned int)header + 21 * sizeof(unsigned int)));
	tclinux_real_size = ecnt_parts[kernel_part_index].size + addr;
	ecnt_parts[kernel_part_index+1].size = ROOTFS_PARTITION(addr);
	ecnt_parts[kernel_part_index+1].offset = ecnt_parts[kernel_part_index].offset + ecnt_parts[kernel_part_index].size;
	
	if(kernel_slave_part_index > 0){
		addr = READ_FLASH_DWORD((unsigned long long)((unsigned int)header_slave + 20 * sizeof(unsigned int)));
		ecnt_parts[kernel_slave_part_index].size = KERNEL_PARTITION(addr);
		addr = READ_FLASH_DWORD((unsigned long long)((unsigned int)header_slave + 21 * sizeof(unsigned int)));
		tclinux_slave_real_size = ecnt_parts[kernel_slave_part_index].size + addr;
		ecnt_parts[kernel_slave_part_index+1].size = ROOTFS_PARTITION(addr);
		ecnt_parts[kernel_slave_part_index+1].offset = ecnt_parts[kernel_slave_part_index].offset + ecnt_parts[kernel_slave_part_index].size;
	}
	/*check whether tclinux/tclinux_slave partition enough*/
	for(i=0; i <num_parts; i++){
		if(!strcmp(ecnt_parts[i].name,TCLINUX)){
			if(ecnt_parts[i].size < tclinux_real_size){
				printk("tclinux partition size = %llx, real size = %llx\n", ecnt_parts[i].size, tclinux_real_size);
				printk("tclinux partition size < its real size!!!\n");
			}
		}
		else if(!strcmp(ecnt_parts[i].name,TCLINUX_SLAVE)){
			if(ecnt_parts[i].size < tclinux_slave_real_size){
				printk("tclinux_slave partition size = %llx, real size = %llx\n", ecnt_parts[i].size, tclinux_slave_real_size);
				printk("tclinux_slave partition size < its real size!!!\n");
			}
		}
	}
}

void tc3162_check_mtdpart_str(char *dst, char *src){
	int have_bootloader_part = 0;
	int have_romfile_part = 0;
	char *bootloader_p = NULL;
	char *bootloader_end_p = NULL;
	
	if(bootloader_p = strstr(src, BOOTLOADER_PART)){
		have_bootloader_part = 1;
	}
	if(strstr(src, ROMFILE_PART)){
		have_romfile_part = 1;
	}

	if(have_romfile_part && have_bootloader_part){
		strcpy(dst, src);
	}
	else if(!have_romfile_part && !have_bootloader_part){
		strcpy(dst, BOOTLOADER_PART_STR);
		strcat(dst, ROMFILE_PART_STR);
		strcat(dst, src);
	}
	else if(have_romfile_part && !have_bootloader_part){
		strcpy(dst, BOOTLOADER_PART_STR);
		strcat(dst, src);
	}
	else if(!have_romfile_part && have_bootloader_part){
		bootloader_end_p = strchr(bootloader_p, ',');
		if(!bootloader_end_p)
			panic("cmdline, bootloader partition error!\n");
		memcpy(dst, src, bootloader_end_p - src+1);
		strcat(dst, ROMFILE_PART_STR);
		strcat(dst, bootloader_end_p+1);
	}
	
}

uint64_t tc3162_get_bootloader_romfile_size(char *name, unsigned int blocksize){
#ifdef TCSUPPORT_BOOTROM_LARGE_SIZE
	uint64_t bootloader_size = 0x00020000;
#else
	uint64_t bootloader_size = 0x00010000;
#endif
	uint64_t romfile_size = 0x00010000;
#ifdef TCSUPPORT_NAND_FLASH
	if (IS_NANDFLASH) {
		blocksize = 0x40000;
	}
#endif

	if(!strcmp(name, BOOTLOADER_PART)){
		if(bootloader_size < blocksize)
			bootloader_size = blocksize;
		return bootloader_size;
	}
	else if(!strcmp(name, ROMFILE_PART)){
		if(romfile_size < blocksize)
			romfile_size = blocksize;
		return romfile_size;
	}
	return SIZE_TO_GET;
	
}
/******************************************************************************
 Function:		tc3162_parse_cmdline_partitions
 Description:	It's used to init tc3162_parts by cmdline partiotion string
 Input:		
 Return:	    	
******************************************************************************/
int tc3162_parse_cmdline_partitions(struct mtd_info *master){
	unsigned long offset;
	int i;
	int size_remaining_index = -1;
	int first_back_forward_index = -1;
	unsigned long size_remaining_offset_start;
	unsigned long back_forward_total_size = 0;
	int is_tclinux_remaining_flag = 0;
	unsigned int blocksize = master->erasesize;
	unsigned int total_size = master->size;
#ifdef TCSUPPORT_NAND_FLASH
	if (IS_NANDFLASH) {
#if defined(TCSUPPORT_CT_PON)
		total_size = nand_flash_avalable_size;
#endif
	}
#endif
	/* parse command line */
	if(tc3162_mtdpart_setup(TCSUPPORT_PARTITIONS_CMDLINE_STR, blocksize) == 0){
		printk("tc3162_mtdpart_setup fail\n");
		return 0;
	}
	
	for(i = 0, offset = 0; i < num_parts; i++)
	{
		if(ecnt_parts[i].offset == OFFSET_BACK_FORWARD){
			first_back_forward_index = i;
			break;
		}
		if(ecnt_parts[i].size == SIZE_TO_GET){
			ecnt_parts[i].offset = offset;
			if(!strcmp(ecnt_parts[i].name,BOOTLOADER_PART) || !strcmp(ecnt_parts[i].name, ROMFILE_PART))
				ecnt_parts[i].size = tc3162_get_bootloader_romfile_size(ecnt_parts[i].name, blocksize);
			else	
				continue;
		}
		
		ecnt_parts[i].offset = offset;				
		
		if(ecnt_parts[i].size == SIZE_REMAINING){
			size_remaining_index = i;
			size_remaining_offset_start = ecnt_parts[i].offset;
			if((i+1) == num_parts){
				ecnt_parts[i].size = total_size - offset;
			}
			else{
				if(!strcmp(ecnt_parts[i].name, TCLINUX)){
					is_tclinux_remaining_flag = 1;
				}
				first_back_forward_index = i+1;
				break;
			}
		}
		if (offset + ecnt_parts[i].size > total_size)
		{
			panic(KERN_WARNING ERRP" part %d: partitioning exceeds flash size, truncating\n", i);
		}
		offset +=ecnt_parts[i].size;
		/*offset align*/
		offset = ALIGN(offset, master->erasesize);
		if(!strcmp(ecnt_parts[i].name, TCLINUX)){
			tclinux_part_offset = ecnt_parts[i].offset;
			tclinux_part_size = ecnt_parts[i].size;
		}
	}
	/*offset type is back forward*/
	for(i = (num_parts -1), offset = total_size; i >=first_back_forward_index; i--){
		ecnt_parts[i].size = ALIGN(ecnt_parts[i].size, master->erasesize);
		back_forward_total_size += ecnt_parts[i].size;
		ecnt_parts[i].offset = offset-ecnt_parts[i].size;
		offset -= ecnt_parts[i].size;
		
		if (offset + back_forward_total_size > total_size)
		{
			panic(KERN_WARNING ERRP"partitioning exceeds flash size, partition index = %d\n",i);
		}
	}
	/*calculate remaining size*/
	if(size_remaining_index != -1){
		ecnt_parts[size_remaining_index].size = offset - size_remaining_offset_start;
		if(ecnt_parts[size_remaining_index].size < 0)
			panic(KERN_WARNING ERRP"partition size < 0, index = %d \n", size_remaining_index);
		if(is_tclinux_remaining_flag){
			tclinux_part_offset = ecnt_parts[size_remaining_index].offset;
			tclinux_part_size = ecnt_parts[size_remaining_index].size;
		}
	}
	
	tc3162_set_kernel_rootfs_part();
	
	return num_parts;
}

#endif
#if defined(TCSUPPORT_FWC_ENV)
extern dev_t ROOT_DEV_SLAVE;
static void tc3162_put_fh_nw_product_roots(void)
{
	personality_parm_boot_to_kernel_info_t *realtime_info = (personality_parm_boot_to_kernel_info_t *)FH_NW_PRODUCT_BOOT2KRENEL_ENV_ADDR;
	struct mtd_info *mtd_A = NULL;
	struct mtd_info *mtd_B = NULL;

    mtd_B = get_mtd_named(FH_NW_PRODUCT_ROOTFSB_PARTITION);
    mtd_A = get_mtd_named(FH_NW_PRODUCT_ROOTFSA_PARTITION);

	if(E_FH_NW_PRODUCT_START_UP_B == realtime_info->realtime_env.rootfs_run)
	{
		if(NULL != mtd_A)
		{
			ROOT_DEV_SLAVE = MKDEV(MTD_BLOCK_MAJOR, mtd_A->index);
			put_mtd_device(mtd_A);
		}
		if(NULL != mtd_B)
		{
			ROOT_DEV = MKDEV(MTD_BLOCK_MAJOR, mtd_B->index);
			put_mtd_device(mtd_B);
		}
	}
	else
	{
	    if(NULL != mtd_A)
	    {
	        ROOT_DEV = MKDEV(MTD_BLOCK_MAJOR, mtd_A->index);
	        put_mtd_device(mtd_A);
	    }
	    if(NULL != mtd_B)
	    {
	        ROOT_DEV_SLAVE = MKDEV(MTD_BLOCK_MAJOR, mtd_B->index);
	        put_mtd_device(mtd_B);
	    }
	}	
	
	return ;
}
#endif

#ifndef TCSUPPORT_IS_FH_PON
static int __init tc3162_mtd_init(void)
{
	int ret = 0;

	struct mtd_partition *mtd_parts = 0;
	int mtd_parts_nb = 0;

	if(ret = tc3162_map_init()){
		printk("tc3162_map_init() fail\n");
		return ret;
	}
	if(ret = tc3162_mtd_info_init()){
		printk("tc3162_mtd_info_init() fail\n");
		return ret;
	}
#ifdef TCSUPPORT_MTD_PARTITIONS_CMDLINE
	if((mtd_parts_nb = tc3162_parse_cmdline_partitions(tc3162_mtd_info))<= 0){
		printk("tc3162_parse_cmdline_partitions() fail\n");
		return -1;
	}
	add_mtd_partitions(tc3162_mtd_info, ecnt_parts, mtd_parts_nb);
#else
#if defined(TCSUPPORT_FWC_ENV)
	add_mtd_partitions(tc3162_mtd_info, tc3162_parts, tc3162_parts_size);
#else
	if(ret = tc3162_add_partitions()){
		printk("tc3162_add_partitions() fail\n");
		return ret;
	}
#endif
#endif
	
#ifdef TCSUPPORT_NAND_FLASH
	if (IS_SPIFLASH) {
		ra_nand_init();
	}	
#endif

#if defined(TCSUPPORT_FWC_ENV)
	tc3162_put_fh_nw_product_roots();
#else
	tc3162_put_rootfs();
#endif

	return 0;
}
#else
static int __init tc3162_mtd_init(void)
{
	struct mtd_info *mtd;
	unsigned int *header;
	unsigned int addr;



	#if defined(CONFIG_DUAL_IMAGE) || defined(TCSUPPORT_MTD_ENCHANCEMENT) || defined(TCSUPPORT_MULTI_BOOT) || defined(TCSUPPORT_NAND_BADBLOCK_CHECK)
	int i = 0;
	#endif
	#ifdef CONFIG_DUAL_IMAGE
	char *bufaddr = (char*)FLAG_ADDR;
	const char flagvalue = 1;//not change!!because we use this flag to judge which image
	char tmp[8] = {0};
	#endif




#ifdef TCSUPPORT_ADDR_MAPPING
	/*add address mapping on 7510. Pork*/
	if(isMT751020 || isMT7505 || isEN751221){
		uint32 tmpVal;
		tmpVal = regRead32(0xbfb00038);
		tmpVal &= 0xffe0e0e0;
		tmpVal |= 0x80070f00;
		regWrite32(0xbfb00038,tmpVal);
		//VPint(0xbfb00038) |= 0x80070F00;
		#ifdef TCSUPPORT_INIC_CLIENT
			ram_base = 0x800000 * (1 << (((VPint(0xbfb0008c) >> 13) & 0x7) - 1))- INIC_CLIENT_RAM_SIMU_MAX_SIZE;
			printk("tc3162: iNIC simulated flash device 0x%08x at 0x%08x\n", INIC_CLIENT_RAM_SIMU_MAX_SIZE, ram_base);
			tc3162_map.virt = ioremap_nocache(ram_base, INIC_CLIENT_RAM_SIMU_MAX_SIZE);
			tc3162_map.phys = ram_base;
			tc3162_map.size = INIC_CLIENT_RAM_SIMU_MAX_SIZE;
		#else
			printk("tc3162: flash device 0x%08x at 0x%08x\n", 0x1000000, 0x1c000000);
			tc3162_map.virt = ioremap_nocache(0x1c000000, 0x1000000);
			tc3162_map.phys = 0x1c000000;
			tc3162_map.size = 0x1000000;
		#endif
		ioremap_nocache(WINDOW_ADDR, WINDOW_SIZE);
	}
	/*add 8M 16M flash support. shnwind*/
	else if (isTC3162U || isTC3182 || isRT65168 || isRT63165 || isRT63365 || isRT63260){
#else
	if (isTC3162U || isTC3182 || isRT65168 || isRT63165 || isRT63365 || isRT63260 || isMT751020 || isMT7505 || isEN751221){
#endif //TCSUPPORT_ADDR_MAPPING

//		header = (unsigned int *)0xb0020000;
		/*enable addr bigger than 4M support.*/
		VPint(0xbfb00038) |= 0x80000000;
		printk("tc3162: flash device 0x%08x at 0x%08x\n", 0x1000000, 0x10000000);
		tc3162_map.virt = ioremap_nocache(0x10000000, 0x1000000);
		tc3162_map.phys = 0x10000000;
		tc3162_map.size = 0x1000000;
		ioremap_nocache(WINDOW_ADDR, WINDOW_SIZE);
	}else{
	
//		header = (unsigned int *)0xbfc20000;
		printk("tc3162: flash device 0x%08x at 0x%08x\n", WINDOW_SIZE, WINDOW_ADDR);
		tc3162_map.virt = ioremap_nocache(WINDOW_ADDR, WINDOW_SIZE);

	}
	if (!tc3162_map.virt) {
   		printk("tc3162: Failed to ioremap\n");
		return -EIO;
	}

	simple_map_init(&tc3162_map);

	#ifdef TCSUPPORT_INIC_CLIENT
		printk("MT75XX: INIC mode\n");
		tc3162_mtd_info = do_map_probe("map_ram", &tc3162_map);
	#else
		/* check if boot from SPI flash */
		if (IS_NANDFLASH) {
			tc3162_mtd_info = do_map_probe("nandflash_probe", &tc3162_map);
		} else if (IS_SPIFLASH) {
			tc3162_mtd_info = do_map_probe("spiflash_probe", &tc3162_map);
		} else {
			tc3162_mtd_info = do_map_probe("cfi_probe", &tc3162_map);
		}
	#endif

	if (!tc3162_mtd_info) {
		#ifdef TCSUPPORT_INIC_CLIENT
		printk("iNIC flash fail\n");
		#endif
		iounmap(tc3162_map.virt);
		return -ENXIO;
	}

  	tc3162_mtd_info->owner = THIS_MODULE;

	add_mtd_partitions(tc3162_mtd_info, tc3162_parts, tc3162_parts_size);
	
	#if 1
	#ifdef TCSUPPORT_NAND_FLASH
	if (IS_SPIFLASH) {
		ra_nand_init();
	}	
	#endif
	#endif
	#ifdef CONFIG_DUAL_IMAGE
	//read the flag from last block to tell kernel use which image(main or slave)
	#if 0
	memcpy(tmp,(char*)(0xb0000000+tc3162_mtd_info->size -sizeof(char)),sizeof(char));	
	#endif
	memcpy(tmp,(char*)bufaddr,sizeof(char));
	if(flagvalue == tmp[0])
	{
		printk("\r\n RootfsB");
		mtd = get_mtd_named("RootfsB");
	}
	else
	{
		printk("\r\  RootfsA");
	#endif
		mtd = get_mtd_named("RootfsA");
	#ifdef CONFIG_DUAL_IMAGE
	}
	#endif	
	if (mtd) {
		ROOT_DEV = MKDEV(MTD_BLOCK_MAJOR, mtd->index);
		put_mtd_device(mtd);
	}


	return 0;
}
#endif
static void __exit tc3162_mtd_cleanup(void)
{
	if (tc3162_mtd_info) {
    	del_mtd_partitions(tc3162_mtd_info);
       	map_destroy(tc3162_mtd_info);
	}

#if 1
	#ifdef TCSUPPORT_NAND_FLASH
	if (IS_SPIFLASH) {
		ra_nand_remove();
	}	
	#endif
#endif	
   	if (tc3162_map.virt) {
   		iounmap(tc3162_map.virt);
       	tc3162_map.virt = 0;
	}
}

module_init(tc3162_mtd_init);
module_exit(tc3162_mtd_cleanup);

