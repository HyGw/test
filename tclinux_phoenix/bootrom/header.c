#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#if defined(__APPLE__)
#include <machine/endian.h>
#include <machine/byte_order.h>
#define __BYTE_ORDER BYTE_ORDER
#define __BIG_ENDIAN BIG_ENDIAN
#define bswap_16(x) NXSwapShort(x)
#define bswap_32(x) NXSwapInt(x)
#define bswap_64(x) NXSwapLongLong(x)
#else
#include <endian.h>
#include <byteswap.h>
#endif
#include <sys/stat.h>
#ifndef TRENDCHIP
#define __BYTE_ORDER __BIG_ENDIAN
#if __BYTE_ORDER == __BIG_ENDIAN
#define STORE32_LE(X)       bswap_32(X)
#elif __BYTE_ORDER == __LITTLE_ENDIAN
#define STORE32_LE(X)       (X)
#else
#error unkown endianness!
#endif
#else
#define STORE32_LE(X)       bswap_32(X)
#endif
								 

#ifdef BOOTROM_LARGE_SIZE
#define TCBOOT_SIZE     0x20000 //128k tcboot
#else
#define TCBOOT_SIZE     0x10000
#endif
#define BOOT_START 		0
#define BOOT_END   		1
#define LZMA_START		2
#define LZMA_END		3
#define SPRAM_START		4
#define SPRAM_END		5


#define TCBOOT_MAGIC_NUM 0x38373536
void usage()
{
	fprintf(stderr, "Usage: header system.map tcboot.bin\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv) 
{
	char buffer[100];
	char name[30];
	char *buf;
	uint32_t startS[6], addr;
	int errno, c, i;
	size_t n;
	FILE *in;
	FILE *out;
	int	add_cali_address = 0;

	errno = 0;

	#ifdef DDR_CALI_NAND
	add_cali_address = 1;
	#endif

	if (!(buf = malloc(TCBOOT_SIZE + 4))) {
		fprintf(stderr, "malloc failed\n");
		return EXIT_FAILURE;
	}

	if (!(in = fopen(argv[1], "rb"))) {
		fprintf(stderr, "can not open \"%s\" for reading\n", optarg);
		usage();
	}

	while (fgets(buffer, 100, in) != NULL) {
		if(buffer == NULL) 
			break;
		sscanf(buffer, "%x %c %s", &addr, &c, name);
		if (strcmp(name, "__lzma_start") == 0) {
			startS[ LZMA_START ] = addr - 0xbfc00000;
		} else if (strcmp(name, "__lzma_end") == 0) {
			startS[ LZMA_END ] = addr - 0xbfc00000;
		} else if (strcmp(name, "__boot_start") == 0) {
			startS[ BOOT_START ] = addr - 0xbfc00000;
		} else if (strcmp(name, "__boot_end") == 0) {
			startS[ BOOT_END ] = addr - 0xbfc00000;
		} else if (strcmp(name, "__spram_start") == 0) {
			startS[ SPRAM_START ] = addr - 0xbfc00000;
		}else if (strcmp(name, "__spram_end") == 0) {
			startS[ SPRAM_END ] = addr - 0xbfc00000;
		}
	}

	fclose(in);
	
	if (!(in = fopen(argv[2], "rb"))) {
		fprintf(stderr, "can not open \"%s\" for reading\n", optarg);
		usage();
	}

	if (!(out = fopen("tcboot.bin", "wb"))) {
		fprintf(stderr, "can not open file tcboot.bin for writing\n");
		usage();
	}

	n = fread(buf, 1, TCBOOT_SIZE + 1, in);
	if (!feof(in)) {
		fprintf(stderr, "fread failure or file \"%s\" too large\n",optarg);
		fclose(in);
		return EXIT_FAILURE;
	}
	fclose(in);

	if(add_cali_address){
		startS[ SPRAM_START ] -= startS[ LZMA_START ];
		startS[ SPRAM_END ] -= startS[ LZMA_START ];
		*(uint32_t*)(buf + startS[ LZMA_START ] + 0x290) = STORE32_LE(startS[ SPRAM_START ]);
		*(uint32_t*)(buf + startS[ LZMA_START ] + 0x294) = STORE32_LE(startS[ SPRAM_END ]);
	}

	for (i=0; i<6; i++) {
		startS[i] = STORE32_LE(startS[i]);
	}

	*(uint32_t*)(buf+0xc) = TCBOOT_MAGIC_NUM;
	*(uint32_t*)(buf+0x10) = startS[ LZMA_START ];
	if(add_cali_address){
		*(uint32_t*)(buf+0x14) = startS[ BOOT_START ];
	}
	else{
		*(uint32_t*)(buf+0x14) = startS[ LZMA_END ];
	}
	*(uint32_t*)(buf+0x18) = startS[ BOOT_START ];
	*(uint32_t*)(buf+0x1c) = startS[ BOOT_END ];
	fwrite(buf, TCBOOT_SIZE, 1, out);

	return 0;
				
}
