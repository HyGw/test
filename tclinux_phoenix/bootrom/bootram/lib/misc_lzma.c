/*
 * misc-lzma.c 
 * LZMA decompression module 
 *
 * Copyright (C) 2004 Streetdata Pty Ltd.
 * Copyright (C) 2005 Marek Michalkiewicz
 */
#ifdef CONFIG_DUAL_IMAGE
#include <asm/tc3162.h>
#endif
#include <flashhal.h>
#include "common.h"

/* frankliao added 20100806 */
#ifndef LZMA_IMG
#define puts prom_printf
#else
#define puts(x) {}
#endif

#define memzero(d, count) (memset((d), 0, (count)))

#define __ptr_t void *
#define NULL ((void *) 0)

typedef unsigned char  uch;
typedef unsigned short ush;
typedef unsigned long  ulg;

#if 0
extern char input_data[];
extern char input_data_end[];
#endif
/* frankliao modify 20100806 */
#ifdef TCSUPPORT_BB_NAND
#ifdef TCSUPPORT_MTD_PARTITIONS_CMDLINE
static char *input_data = (char *) KERNEL_FLASH_START;
#else
static char *input_data = (char *) LZMA_FLASH_START;
#endif
static char *input_data_end = (char *) LZMA_FLASH_END;
#else
static char *input_data = (char *) KERNEL_FLASH_START;
static char *input_data_end = (char *) KERNEL_FLASH_END;
unsigned char ReadNandCache(unsigned long index) {} 
unsigned char ReadNandByte(unsigned long index) {} 
unsigned long ReadNandDWord(unsigned long index) {} 
#endif

static uch *output_data;
static ulg output_ptr;
static ulg bytes_out;

#define _LZMA_PROB32
#include "LzmaDecode.c"

#ifdef LZMA_IMG
#define error(x) {}
#else
static void error(char *x)
{
	int ptr;

	puts("\n\n");
	puts(x);
	puts("\n\n -- System halted");

	while(1);	/* Halt */
}
#endif

#if defined(TCSUPPORT_CPU_EN7512)||defined(TCSUPPORT_CPU_EN7521)	
#define INPUT_DATA(i) ( (NANDFLASH_HWTRAP) ? SPI_NAND_Flash_Read_Byte((unsigned long)i) : (*((unsigned char*)(i))) )
#else
#define INPUT_DATA(i) ( (NANDFLASH_HWTRAP) ? ReadNandByte((unsigned long)i) : (*((unsigned char*)(i))) )

#endif
/*______________________________________________________________________________
**  set_lzma_addr
**
**  descriptions: set lzma file input address
**				  
**  execution sequence:
**  commands process:
**  parameters: lzma_data_start: start address of lzma data
**				lzma_data_end:   end address of lzma data	 					 
**					
**  local:
**  global:
**  return:
**  called by:
**  call:
**____________________________________________________________________________*/
int
set_lzma_addr(char* lzma_data_start, char* lzma_data_end)
{
	input_data = lzma_data_start;
	input_data_end = lzma_data_end;
}

#define MAX_LC_PLUS_LP 3
#define LZMA_BUF_SIZE (LZMA_BASE_SIZE + ((LZMA_LIT_SIZE) << (MAX_LC_PLUS_LP)))
//static CProb lzma_buf[LZMA_BUF_SIZE];
/* frankliao modify 20100806 */
ulg
decompress_kernel(ulg output_start, ulg free_mem_ptr_p, ulg free_mem_ptr_end_p)
{
	int lc, lp, pb, result;
	unsigned int input_size, output_size;
	unsigned char *p;
	CLzmaDecoderState state;

	output_data = (uch *)output_start;	/* Points to kernel start */
	puts("Uncompressing [LZMA] ... ");
#if 0
#ifdef CONFIG_DUAL_IMAGE
	//flag = 1 mean to use slave kernel
	if (isTC3162U || isTC3182)
	{
		input_data = (char *) 0xb0020100;
		input_data_end = (char *) 0xb0120000;
	}
	
	if(flag)
	{
		puts("from slave");
		#if 0
		input_data = (unsigned char*)(0xb0020100+0x500000);
		input_data_end = (unsigned char*)(0xb0120000+0x500000);
		#endif
		input_data = (unsigned char*)(input_data+0x500000);
		input_data_end = (unsigned char*)(input_data_end+0x500000);
	}
	else
	{
		puts("from main");
		#if 0
		input_data = (unsigned char*)(0xb0020100);
		input_data_end = (unsigned char*)(0xb0120000);
		#endif
	}
#endif	
#endif
	p = (unsigned char *) (input_data);

	if (LzmaDecodeProperties(&state.Properties, p, LZMA_PROPERTIES_SIZE)
	    != LZMA_RESULT_OK)
	{
#if defined(TCSUPPORT_FWC_ENV)
        puts("\n bad properties.\n");
		return E_FH_NW_PRODUCT_DECOMPRESS_WRONG;
#else
		error("bad properties");
#endif
	}
	
	if (LzmaGetNumProbs(&state.Properties) > LZMA_BUF_SIZE)
	{
#if defined(TCSUPPORT_FWC_ENV)
        puts("\n not enough memory.\n");
		return E_FH_NW_PRODUCT_DECOMPRESS_WRONG;
#else
		error("not enough memory");
#endif
	}

	//state.Probs = &lzma_buf;
	state.Probs = free_mem_ptr_p;

	input_size = input_data_end - input_data;

	output_size = (INPUT_DATA( (p+8) ));
//	output_size = p[8];
	
	output_size = ((output_size << 8) + INPUT_DATA( (p+7) ));
//	output_size = ((output_size << 8) + p[7] );
	
	output_size = ((output_size << 8) + INPUT_DATA( (p+6) ));
//	output_size = ((output_size << 8) + p[6] );
	
	output_size = ((output_size << 8) + INPUT_DATA( (p+5) ));
//	output_size = ((output_size << 8) + p[5] );


//	if (LOAD_ADDR < UNCOMPRESS_OUT + output_size)
//		error("image too big");
	if(output_data + output_size > free_mem_ptr_p){
		error("linux.bin image too big");
	}
#define LZMA_HEADER_SIZE 13
	p += LZMA_HEADER_SIZE;
	input_size -= LZMA_HEADER_SIZE;

	result = LzmaDecode(&state, p, input_size, &input_size,
			    output_data, output_size, &output_size);

	if (result != LZMA_RESULT_OK)
	{
#if defined(TCSUPPORT_FWC_ENV)
        puts("\n decompression error.\n");
		return E_FH_NW_PRODUCT_DECOMPRESS_WRONG;
#else
		error("decompression error");
#endif
	}
	
	puts(" done.\n");

#if defined(TCSUPPORT_FWC_ENV)
	return E_FH_NW_PRODUCT_DECOMPRESS_RIGHT;
#else
	return output_ptr;
#endif
}

