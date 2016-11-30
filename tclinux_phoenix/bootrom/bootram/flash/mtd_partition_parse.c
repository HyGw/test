/***************************************************************
Copyright Statement:

This software/firmware and related documentation (¡°EcoNet Software¡±) 
are protected under relevant copyright laws. The information contained herein 
is confidential and proprietary to EcoNet (HK) Limited (¡°EcoNet¡±) and/or 
its licensors. Without the prior written permission of EcoNet and/or its licensors, 
any reproduction, modification, use or disclosure of EcoNet Software, and 
information contained herein, in whole or in part, shall be strictly prohibited.

EcoNet (HK) Limited  EcoNet. ALL RIGHTS RESERVED.

BY OPENING OR USING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY 
ACKNOWLEDGES AND AGREES THAT THE SOFTWARE/FIRMWARE AND ITS 
DOCUMENTATIONS (¡°ECONET SOFTWARE¡±) RECEIVED FROM ECONET 
AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON AN ¡°AS IS¡± 
BASIS ONLY. ECONET EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, 
WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, 
OR NON-INFRINGEMENT. NOR DOES ECONET PROVIDE ANY WARRANTY 
WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTIES WHICH 
MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE ECONET SOFTWARE. 
RECEIVER AGREES TO LOOK ONLY TO SUCH THIRD PARTIES FOR ANY AND ALL 
WARRANTY CLAIMS RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES 
THAT IT IS RECEIVER¡¯S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD 
PARTY ALL PROPER LICENSES CONTAINED IN ECONET SOFTWARE.

ECONET SHALL NOT BE RESPONSIBLE FOR ANY ECONET SOFTWARE RELEASES 
MADE TO RECEIVER¡¯S SPECIFICATION OR CONFORMING TO A PARTICULAR 
STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND 
ECONET'S ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE ECONET 
SOFTWARE RELEASED HEREUNDER SHALL BE, AT ECONET'S SOLE OPTION, TO 
REVISE OR REPLACE THE ECONET SOFTWARE AT ISSUE OR REFUND ANY SOFTWARE 
LICENSE FEES OR SERVICE CHARGES PAID BY RECEIVER TO ECONET FOR SUCH 
ECONET SOFTWARE.
***************************************************************/

/************************************************************************
*                  I N C L U D E S
*************************************************************************
*/
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/mtd/partitions.h>
#include <linux/vmalloc.h>
#include <asm/io.h>
#include <asm/tc3162.h>
#include <flashhal.h>
#if defined (TCSUPPORT_BB_NAND)
#include <mt7510_nandflash.h>
#endif
#include "common.h"

/************************************************************************
*                  D E F I N E S   &   C O N S T A N T S
*************************************************************************
*/
#define MTD_DBG 0

#if MTD_DBG
#define printf prom_printf
#else
#define printf(args...) 		do{}while(0)
#endif

#define __ALIGN_KERNEL(x, a)		__ALIGN_KERNEL_MASK(x, (typeof(x))(a) - 1)
#define __ALIGN_KERNEL_MASK(x, mask)	(((x) + (mask)) & ~(mask))
#define ALIGN(x, a)		__ALIGN_KERNEL((x), (a))

#define PARTS_SIZE 512
#define STR_LEN	150

/* special size referring to all the remaining space in a partition */
#define SIZE_REMAINING UINT_MAX
#define SIZE_TO_GET (UINT_MAX-1)
#define OFFSET_CONTINUOUS UINT_MAX
#define OFFSET_BACK_FORWARD	(UINT_MAX-1)
#define OFFSET_NOT_FOUND UINT_MAX 

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


#define WINDOW_ADDR 0x1fc00000
#define WINDOW_SIZE 0x400000
#define BUSWIDTH 	2

#define TRX_LEN	256
#define KERNEL_PARTITION(a)	(a + TRX_LEN)  //Include trx header
#define ROOTFS_PARTITION(a)	((a + 0x10000) & ~(0x10000-1))

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


/************************************************************************
*                  M A C R O S
*************************************************************************
*/

/************************************************************************
*                  D A T A   T Y P E S
*************************************************************************
*/
struct mtd_info {
	unsigned long offset; /* Offset within the memory */
	unsigned long size;   /* Total size of the MTD */
	unsigned long erasesize;
};


/************************************************************************
*                  E X T E R N A L   D A T A   D E C L A R A T I O N S
*************************************************************************
*/
extern unsigned long flash_base;
#ifdef TCSUPPORT_DUAL_IMAGE_ENHANCE
extern int offset;
#endif
extern struct mtd_info mtd;
#if defined (TCSUPPORT_BB_NAND)
extern flashdev_info devinfo;
#if defined(TCSUPPORT_CT_PON)
extern int nand_flash_avalable_size;
#endif
#endif


/************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
*************************************************************************
*/
extern unsigned int (*ranand_read_byte)(unsigned long long);
extern unsigned int (*ranand_read_dword)(unsigned long long);


/************************************************************************
*                  P U B L I C   D A T A
*************************************************************************
*/


/************************************************************************
*                  P R I V A T E   D A T A
*************************************************************************
*/
static struct mtd_partition *ecnt_parts;
static int num_parts = 0;
static int has_remaining_part_flag = 0;
static uint64_t tclinux_part_size = 0;			
static uint64_t tclinux_part_offset = OFFSET_CONTINUOUS;	
static int kernel_part_index = -1;
static int kernel_slave_part_index = -1;
static char parts_size[PARTS_SIZE];

/************************************************************************
*                  F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/
unsigned long long ecnt_memparse(const char *ptr, char **retptr, unsigned int blocksize)
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

static void ecnt_newpart_set_other_parts(struct mtd_partition *part, char *name, int index, unsigned int blocksize){
	if(!part || !name){
		printf("ecnt_newpart_set_other_parts fail, input NULL\n");
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
#ifdef TCSUPPORT_BB_NAND
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

static void ecnt_check_mtdpart_str(char *dst, char *src){
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
			prom_printf("cmdline, bootloader partition error!\n");
		memcpy(dst, src, bootloader_end_p - src+1);
		strcat(dst, ROMFILE_PART_STR);
		strcat(dst, bootloader_end_p+1);
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
static struct mtd_partition * ecnt_newpart(char *s, char **retptr,int *num_parts,
                                      int this_part,unsigned char **extra_mem_ptr,
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
			printf(KERN_ERR ERRP "no fill-up partitions allowed after already having a fill-up partition\n");
			return NULL;
		}
	}
	
	else{
		size = ecnt_memparse(s, &s, blocksize);
		if(size == 0)
			size = SIZE_TO_GET;
		if (size < PAGE_SIZE){
			printf(KERN_ERR ERRP "partition size too small (%lx)\n", size);
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
		if (!p){
			printf(KERN_ERR ERRP "no closing %c found in partition name\n", delim);
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
	if (*s == ','){
		if(!strncmp(name, TCLINUX, name_len)){
			this_part += 2;
			extra_mem_size +=strlen(KERNEL_PART)+strlen(ROOTFS_PART)+2;
		}
		else if(!strncmp(name, TCLINUX_SLAVE, name_len)){
			this_part += 2;
			extra_mem_size +=strlen(KERNEL_SLAVE_PART)+strlen(ROOTFS_SLAVE_PART)+2;
		}
		
		/* more partitions follow, parse them */
		parts = ecnt_newpart(s + 1, &s, num_parts, this_part + 1,
				&extra_mem, extra_mem_size, blocksize);
		if (!parts)
			return NULL;
	}
	else{	/* this is the last partition: allocate space for all */
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
		parts = parts_size;
	
		if(alloc_size > PARTS_SIZE){
			prom_printf(KERN_ERR ERRP "out of memory\n");
			return NULL;
		}
		extra_mem = (unsigned char *)(parts + *num_parts);
	}
	/* enter this partition (offset will be calculated later if it is zero at this point) */
	parts[this_part].size = size;
	parts[this_part].offset = offset;
	parts[this_part].mask_flags = mask_flags;
	if (name){
		strncpy(extra_mem, name, name_len);
	}
	else{
		sprintf(extra_mem, "Partition_%03d", this_part);
	}
	parts[this_part].name = extra_mem;
	extra_mem += name_len + 1;
	if(!strcmp(parts[this_part].name, TCLINUX)){
		ecnt_newpart_set_other_parts(&parts[this_part-2],TCLINUX, this_part-2, blocksize);
	}
	else if(!strcmp(parts[this_part].name, TCLINUX_SLAVE)){
		ecnt_newpart_set_other_parts(&parts[this_part-2],TCLINUX_SLAVE, this_part-2, blocksize);
	}
	if(this_part == (*num_parts -2)){
		ecnt_newpart_set_other_parts(&parts[*num_parts -1], RESERVEAREA, *num_parts -1, blocksize);
	}
	printf("partition %d: name <%s>, offset %x, size %x, mask flags %x\n",this_part, parts[this_part].name,
	     parts[this_part].offset, parts[this_part].size, parts[this_part].mask_flags);

	/* return (updated) pointer to extra_mem memory */
	if (extra_mem_ptr)
	  *extra_mem_ptr = extra_mem;

	/* return (updated) pointer command line string */
	*retptr = s;

	/* return partition table */
	return parts;
}

/******************************************************************************
 Function:		ecnt_mtdpart_setup
 Description:	Parse the command line.
 Input:		
 Return:	    	
******************************************************************************/
static int ecnt_mtdpart_setup(char *s, unsigned int blocksize)
{
	char cmdline[STR_LEN];
	unsigned char *extra_mem;
	char *p;

	if(s == NULL){
		prom_printf("ecnt_mtdpart_setup(), mtd partition cmdline is NULL\n");
		return 0;
	}
	
	if(strlen(s) + strlen(BOOTLOADER_PART_STR)+strlen(ROMFILE_PART_STR) > STR_LEN){
		prom_printf("ecnt_mtdpart_setup(), string length outof size\n");
		return 0;
	}
	
	ecnt_check_mtdpart_str(cmdline, s);
	
	p = cmdline;
	printf("\nparsing <%s>\n", p);

	/*
	 * parse one mtd. have it reserve memory for the
	 * struct cmdline_mtd_partition and the mtd-id string.
	 */
	ecnt_parts = ecnt_newpart(p,		/* cmdline */
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

static void ecnt_set_kernel_rootfs_part(void){
	unsigned int addr;
	unsigned int *header;
	unsigned int *header_slave;
	int i;	
	uint32_t tclinux_real_size = 0;
	uint32_t tclinux_slave_real_size = 0;

	if(kernel_part_index < 0)
		return;
	
	if(tclinux_part_offset != OFFSET_CONTINUOUS){
		header = (unsigned int *)(flash_base + tclinux_part_offset);
	}
	else{	
		prom_printf("ecnt_set_kernel_rootfs_part(), tclinux partition offset error\n");
	}
	
	if((tclinux_part_size !=0) && (tclinux_part_size != SIZE_REMAINING)){
		header_slave = (unsigned int *)(flash_base + tclinux_part_offset + tclinux_part_size);
	}
	else{
		prom_printf("ecnt_set_kernel_rootfs_part(), tclinux partition size error\n");		
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
		printf("tclinux_slave real size = %x\n", tclinux_slave_real_size);
	}
	/*check whether tclinux/tclinux_slave partition enough*/
	for(i=0; i <num_parts; i++){
		if(!strcmp(ecnt_parts[i].name,TCLINUX)){
			
			if(ecnt_parts[i].size < tclinux_real_size){
				prom_printf("tclinux partition size = 0x%x, real size = 0x%x\n", ecnt_parts[i].size, tclinux_real_size);
				prom_printf("tclinux partition size < its real size\n");
			}
		}
		else if(!strcmp(ecnt_parts[i].name,TCLINUX_SLAVE)){
			if(ecnt_parts[i].size < tclinux_slave_real_size){
				prom_printf("tclinux_slave partition size = 0x%x, real size = 0x%x\n", ecnt_parts[i].size,tclinux_slave_real_size);
				prom_printf("tclinux_slave partition size < its real size\n");
			}
		}
	}
}

static uint32_t ecnt_get_bootloader_romfile_size(char *name, unsigned int blocksize){
#ifdef TCSUPPORT_BOOTROM_LARGE_SIZE
	uint32_t bootloader_size = 0x00020000;
#else
	uint32_t bootloader_size = 0x00010000;
#endif
	uint32_t romfile_size = 0x00010000;
#ifdef TCSUPPORT_BB_NAND
	if (IS_NANDFLASH) {
		blocksize = 0x40000;
	}
#endif
	
	if(!strcmp(name, BOOTLOADER_PART)){
		if(bootloader_size < blocksize)
			bootloader_size = blocksize;
		printf("ecnt_get_bootloader_romfile_size(), bootloader = %x, blocksize = %x\n",bootloader_size, blocksize);
		return bootloader_size;
	}
	else if(!strcmp(name, ROMFILE_PART)){
		if(romfile_size < blocksize)
			romfile_size = blocksize;
		printf("ecnt_get_bootloader_romfile_size(), romfile size = %x\n",romfile_size);
		return romfile_size;
	}
	return SIZE_TO_GET;
	
}
/******************************************************************************
 Function:		__ecnt_parse_cmdline_partitions
 Description:	It's used to init tc3162_parts by cmdline partiotion string
 Input:		
 Return:	    	
******************************************************************************/
static int __ecnt_parse_cmdline_partitions(unsigned int blocksize, unsigned int size){
	unsigned long offset;
	int i;
	int size_remaining_index = -1;
	int first_back_forward_index = -1;
	unsigned long size_remaining_offset_start;
	unsigned long back_forward_total_size = 0;
	int is_tclinux_remaining_flag = 0;
	
	/* parse command line */
	if(ecnt_mtdpart_setup(TCSUPPORT_PARTITIONS_CMDLINE_STR, blocksize) == 0){
		prom_printf("ecnt_mtdpart_setup fail\n");
		return 0;
	}
	printf("__ecnt_parse_cmdline_partitions(), num_parts = %d\n", num_parts);
	for(i = 0, offset = 0; i < num_parts; i++)
	{
		if(ecnt_parts[i].offset == OFFSET_BACK_FORWARD){
			first_back_forward_index = i;
			break;
		}
		if(ecnt_parts[i].size == SIZE_TO_GET){
			ecnt_parts[i].offset = offset;
			if(!strcmp(ecnt_parts[i].name,BOOTLOADER_PART) || !strcmp(ecnt_parts[i].name, ROMFILE_PART))
				ecnt_parts[i].size = ecnt_get_bootloader_romfile_size(ecnt_parts[i].name, blocksize);
			else	
				continue;
		}
		
		ecnt_parts[i].offset = offset;
					
		if(ecnt_parts[i].size == SIZE_REMAINING){
			size_remaining_index = i;
			size_remaining_offset_start = ecnt_parts[i].offset;
			if((i+1) == num_parts){
				ecnt_parts[i].size = size - offset;
			}
			else{
				if(!strcmp(ecnt_parts[i].name, TCLINUX)){
					is_tclinux_remaining_flag = 1;
				}
				first_back_forward_index = i+1;
				break;
			}
		}
		if (offset + ecnt_parts[i].size > size)
		{
			prom_printf(ERRP" part %d: partitioning exceeds flash size, truncating\n", i);
			return 0;
		}
		offset +=ecnt_parts[i].size;
		/*offset align*/
		offset = ALIGN(offset, blocksize);
		printf("%d %s offset = %x, size = %x\n", i, ecnt_parts[i].name, ecnt_parts[i].offset, ecnt_parts[i].size);
		if(!strcmp(ecnt_parts[i].name, TCLINUX)){
			tclinux_part_offset = ecnt_parts[i].offset;
			tclinux_part_size = ecnt_parts[i].size;
		}
	}
	/*offset type is back forward*/
	for(i = (num_parts -1), offset = size; i >=first_back_forward_index; i--){
		ecnt_parts[i].size = ALIGN(ecnt_parts[i].size, blocksize);
		back_forward_total_size += ecnt_parts[i].size;
		ecnt_parts[i].offset = offset-ecnt_parts[i].size;
		printf("offset = %x, i= %d, part offset = %x, size = %x\n", offset, i, ecnt_parts[i].offset, ecnt_parts[i].size);
		offset -= ecnt_parts[i].size;
		
		if (offset + back_forward_total_size > size)
		{
			prom_printf(ERRP"partitioning exceeds flash size, partition index = %d\n",i);
			return 0;
		}
		printf("end %d %s offset = %x, size = %x\n", i, ecnt_parts[i].name, ecnt_parts[i].offset, ecnt_parts[i].size);
	}
	/*calculate remaining size*/
	if(size_remaining_index != -1){
		ecnt_parts[size_remaining_index].size = offset - size_remaining_offset_start;
		if(ecnt_parts[size_remaining_index].size < 0)
			printf(KERN_WARNING ERRP"partition size < 0, index = %d \n", size_remaining_index);
		if(is_tclinux_remaining_flag){
			tclinux_part_offset = ecnt_parts[size_remaining_index].offset;
			tclinux_part_size = ecnt_parts[size_remaining_index].size;
		}
		printf("%d %s offset = %x, size = %x\n", size_remaining_index, ecnt_parts[size_remaining_index].name, ecnt_parts[size_remaining_index].offset, ecnt_parts[size_remaining_index].size);
	}
	
	ecnt_set_kernel_rootfs_part();
	return num_parts;
}

uint32_t ecnt_get_mtd_offset(char *name){
	int i;

	if(!ecnt_parts){
		prom_printf("mtd parts is NULL\n");
		return OFFSET_NOT_FOUND;
	}
		
	for(i = 0; i < num_parts; i++){
		if(!strcmp(ecnt_parts[i].name, name))
			return ecnt_parts[i].offset;
	}
	prom_printf("not find %s partition\n", name);
	return OFFSET_NOT_FOUND;
}

uint32_t ecnt_get_mtd_size(char *name){
	int i;

	if(!ecnt_parts){
		prom_printf("mtd parts is NULL\n");
		return SIZE_TO_GET;
	}
		
	for(i = 0; i < num_parts; i++){
		if(!strcmp(ecnt_parts[i].name, name))
			return ecnt_parts[i].size;
	}
	prom_printf("not find %s partition\n", name);
	return SIZE_TO_GET;
}
uint32_t ecnt_get_tclinux_mtd_offset(void){
	uint32_t offset;

	offset = ecnt_get_mtd_offset(TCLINUX);
	if(offset == OFFSET_NOT_FOUND)
		return 0;
	else 
		return offset;
}

uint32_t ecnt_get_tclinux_slave_mtd_offset(void){
	uint32_t offset;

	offset = ecnt_get_mtd_offset(TCLINUX_SLAVE);
	if(offset == OFFSET_NOT_FOUND)
		return 0;
	else 
		return offset;
}
uint32_t ecnt_get_tclinux_flash_offset(void){
	uint32_t offset;

	offset = ecnt_get_mtd_offset(TCLINUX);
	if(offset == OFFSET_NOT_FOUND)
		return 0;
	else 
		return (offset+flash_base);
}

uint32_t ecnt_get_tclinux_slave_flash_offset(void){
	uint32_t offset;

	offset = ecnt_get_mtd_offset(TCLINUX_SLAVE);
	if(offset == OFFSET_NOT_FOUND)
		return 0;
	else 
		return (offset+flash_base);
}

uint32_t ecnt_get_reservearea_flash_offset(void){
	uint32_t offset;

	offset = ecnt_get_mtd_offset(RESERVEAREA);
	if(offset == OFFSET_NOT_FOUND)
		return 0;
	else 
		return (offset+flash_base);
}

uint32_t ecnt_get_tclinux_size(void){
	uint32_t size;

	size = ecnt_get_mtd_size(TCLINUX);
	if(size == SIZE_TO_GET)
		return 0;
	else 
		return size;
}

uint32_t ecnt_get_tclinux_slave_size(void){
	uint32_t size;

	size = ecnt_get_mtd_size(TCLINUX_SLAVE);
	if(size == SIZE_TO_GET)
		return 0;
	else 
		return size;
}

uint32_t ecnt_get_kernel_size(unsigned int erasesize){
	uint32_t size;

	size = ecnt_get_mtd_size(KERNEL_PART);
	if(size == SIZE_TO_GET)
		return 0;
	else{
		return size;
	}
}

uint32_t ecnt_get_kernel_slave_size(unsigned int erasesize){
	uint32_t size;

	size = ecnt_get_mtd_size(KERNEL_SLAVE_PART);
	if(size == SIZE_TO_GET)
		return 0;
	else{ 
		return size;
	}
}

uint32_t ecnt_get_reservearea_size(void){
	uint32_t size;

	size = ecnt_get_mtd_size(RESERVEAREA);
	if(size == SIZE_TO_GET)
		return 0;
	else 
		return size;
}

uint32_t ecnt_get_boot_size(void){
	uint32_t size;
	
	size = ecnt_get_mtd_size(BOOTLOADER_PART);
	if(size == SIZE_TO_GET)
		return 0;
	else 
		return size;
}

uint32_t ecnt_get_romfile_size(void){
	uint32_t size;
	
	size = ecnt_get_mtd_size(ROMFILE_PART);
	if(size == SIZE_TO_GET)
		return 0;
	else 
		return size;
}

int ecnt_parse_cmdline_partitions(void){
#ifdef TCSUPPORT_BB_NAND
	if (IS_NANDFLASH){
		printf("devinfo total size = %x\n",  devinfo.totalsize*1024*1024);
#if defined(TCSUPPORT_CT_PON)
		__ecnt_parse_cmdline_partitions(devinfo.blocksize*1024, nand_flash_avalable_size);
#else
		__ecnt_parse_cmdline_partitions(devinfo.blocksize*1024, devinfo.totalsize*1024*1024);
#endif
	}
	else
#endif
		__ecnt_parse_cmdline_partitions(mtd.erasesize, mtd.size);
	flash_tclinux_start = TCLINUX_FLASH_START;
#ifdef TCSUPPORT_DUAL_IMAGE_ENHANCE
	offset = ecnt_get_tclinux_size();
#endif
	
	return 1;
}

int do_get_mtd_info(int argc, char *argv[]){
	int i;
	
	if(!ecnt_parts){
		prom_printf("do_get_mtd_info(), mtd parts is NULL\n");
		return 0;
	}
	
	for(i = 0; i < num_parts; i++){
		prom_printf( "0x%08x-0x%08x : \"%s\"\n", ecnt_parts[i].offset, (ecnt_parts[i].offset + ecnt_parts[i].size), ecnt_parts[i].name);
	}
	
	return 0;
}

