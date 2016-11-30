#include <asm/types.h>
#include <asm/io.h>
#include <asm/tc3162.h>
#include "spiflash.h"
#ifdef SPI_DRAM_TEST_CMD
#include "dramtest.h"
#endif

/* debugging */
/* #define SPIFLASH_DEBUG */
#define TC_SOC

static char module_name[] = "spiflash";

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
#define MANUFACTURER_GIGADEVICE		0x00c8
/* ST */
#define M25P16	          		0x2015
#define M25P32	          		0x2016
#define M25P64	          		0x2017

/* Winbond */
#define W25X16					0x3015
#define W25X32					0x3016
#define W25X64					0x3017
#define W25X128					0x3018

#define W25Q16					0x4015
#define W25Q32					0x4016
#define W25Q64					0x4017
#define W25Q128					0x4018

/* SST */
#define SST25VF032B				0x254a

/* MXIC */
#define MX25L3205D				0x2016
#define MX25L6405D				0x2017
#define MX25L12805D				0x2018
#define MX25L25635E  			0x2019

/* SPANSION */
#define S25FL016A          		0x0214
#define S25FL032A          		0x0215
#define S25FL064A          		0x0216
#define S25FL128P          		0x2018

#define N25Q064					0xba17
#define S25FL016K          		0x4015
											
/* EON */
#define EN25Q64          		0x3017

/* GIGA */
#define GD25Q64				0x4017
#define GD25Q32				0x4016
#define GD25Q128			0x4018

/* define SPI mode */
#define SPI_STD_RD				0x0
#define SPI_STD_FAST_RD			0x1
#define SPI_FAST_RD_DUAL_O		0x2
#define SPI_FAST_RD_DUAL_IO		0x3
#define SPI_FAST_RD_QUAD_O		0x4
#define SPI_FAST_RD_QUAD_IO		0x5
#define SPI_BURST_RD_QUAD_IO	0x6

#ifdef TC_SOC
#define SPI_REG_BASE		0xbfbc0000
#define SPI_REG_MASTER		0xbfbc0028
#define SPI_REG_MOREBUF		0xbfbc002c
#define SPI_REG_SPACE_CR	0xbfbc003c
#define SPI_FLASH_DATA2          0x0C
#define SPI_FLASH_DATA3          0x10
#define SPI_FLASH_DATA4          0x14
#define SPI_FLASH_DATA5          0x18
#define SPI_FLASH_DATA6          0x1C
#define SPI_FLASH_DATA7          0x20
#define SPI_FLASH_DATA8          0x24
#endif

#ifndef CONFIG_DUAL_IMAGE
struct spi_flash_info {
	const u16 mfr_id;
	const u16 dev_id;
	const u16 extra_id;
	const char *name;
	const int DeviceSize;
	const int EraseSize;
	int mode;
};
#endif

#define printf	prom_printf

struct mtd_info {
	unsigned long offset; /* Offset within the memory */
	unsigned long size;   /* Total size of the MTD */
	unsigned long erasesize;
};

static __u32 spiflash_regread32(int reg);
static void spiflash_regwrite32(int reg, __u32 data);
static __u32 spiflash_sendcmd (int op);

int spiflash_init (unsigned long rom_base);
void spiflash_exit (void);
static int spiflash_probe (void);
static int spiflash_erase_internal (struct mtd_info *mtd,unsigned long addr, unsigned long len);
static int spiflash_read_internal (struct mtd_info *mtd, unsigned long from,unsigned long len,unsigned long *retlen,unsigned char *buf);
static int spiflash_write_internal (struct mtd_info *mtd,unsigned long to,unsigned long len,unsigned long *retlen,const unsigned char *buf);

/* Mapping of generic opcodes to STM serial flash opcodes */
struct opcodes {
    __u16 code;
    __s8 tx_cnt;
    __s8 rx_cnt;
} stm_opcodes[] = {
        {STM_OP_WR_ENABLE, 1, 0},
        {STM_OP_WR_DISABLE, 1, 0},
        {STM_OP_RD_STATUS, 1, 1},
        {STM_OP_WR_STATUS, 2, 0},
        {STM_OP_RD_DATA, 4, 4},
        {STM_OP_FAST_RD_DATA, 1, 0},
        {STM_OP_PAGE_PGRM, 8, 0},
        {STM_OP_SECTOR_ERASE, 4, 0},
        {STM_OP_BULK_ERASE, 1, 0},
        {STM_OP_DEEP_PWRDOWN, 1, 0},
        {STM_OP_RD_SIG, 4, 1},
        {STM_OP_RD_ID, 1, 3},
        {STM_OP_RD_STATUS2, 1, 1},
        {STM_OP_HPM, 1, 0},
		{STM_OP_EN4B, 1, 0}
};

struct mtd_info mtd;

unsigned long spidbg = 0;
static __u8 byte_program_mode = 0;
//#ifdef CONFIG_DUAL_IMAGE
#if 0
static struct spi_flash_info flash_tables[];
static unsigned char flash_index;
struct spi_flash_info
getFlashTableEntry(unsigned char index)
{
	return flash_tables[index];
}

unsigned char 
getFlashIndex(void)
{
	return flash_index;
}
#endif

static __u32
spiflash_regread32(int reg)
{
	volatile __u32 *addr = (__u32 *)(CR_SPI_BASE + reg);
	__u32 data = *addr;

	if (spidbg)
   		printf ("REGREAD addr = 0x%08x data= %08x\n", addr, data);
#ifdef SPIFLASH_DEBUG
   	printf ("REGREAD addr = 0x%08x data= %08x\n", addr, *addr);
#endif
	return (data);
}

static void
spiflash_regwrite32(int reg, __u32 data)
{
	volatile __u32 *addr = (__u32 *)(CR_SPI_BASE + reg);

	if (spidbg)
   		printf ("REGWRITE addr = 0x%08x data= %08x\n", addr, data);
#ifdef SPIFLASH_DEBUG
   	printf ("REGWRITE addr = 0x%08x data= %08x\n", addr, data);
#endif
	*addr = data;
	return;
}

static __u32
spiflash_sendcmd (int op)
{
	 __u32 reg;
	 __u32 mask;
	struct opcodes *ptr_opcode;

	ptr_opcode = &stm_opcodes[op];

	do {
		reg = spiflash_regread32(SPI_FLASH_CTL);
	} while (reg & SPI_CTL_BUSY);

	spiflash_regwrite32(SPI_FLASH_OPCODE, ptr_opcode->code);

	reg = (reg & ~SPI_CTL_TX_RX_CNT_MASK) | ptr_opcode->tx_cnt |
        	(ptr_opcode->rx_cnt << 4) | SPI_CTL_START;

	spiflash_regwrite32(SPI_FLASH_CTL, reg);

	if (ptr_opcode->rx_cnt > 0) {
        	do {
          		reg = spiflash_regread32(SPI_FLASH_CTL);
        	} while (reg & SPI_CTL_BUSY);

        	reg = (__u32) spiflash_regread32(SPI_FLASH_DATA);

        	switch (ptr_opcode->rx_cnt) {
        	case 1:
            		mask = 0x000000ff;
            		break;
        	case 2:
            		mask = 0x0000ffff;
            		break;
        	case 3:
            		mask = 0x00ffffff;
            		break;
        	default:
            		mask = 0xffffffff;
            		break;
        	}

        	reg &= mask;
   	} else {
       		reg = 0;
	}

	return reg;
}

/* Probe SPI flash device
 * Function returns 0 for failure.
 * and flashconfig_tbl array index for success.
 */
static u32
spiflash_read_id (void)
{
	u32 flash_id;

	flash_id = spiflash_sendcmd(SPI_RD_ID);
	flash_id = (flash_id & 0xff) << 16 | (flash_id & 0xff00) | (flash_id >> 16) & 0xff;
	return flash_id;
}

static int
spiflash_erase_oneblock (struct mtd_info *mtd, unsigned long addr, unsigned long len)
{
	struct opcodes *ptr_opcode;
	__u32 temp, reg;
	int finished = FALSE;

   	/* sanity checks */
   	if (addr + len > mtd->size) return (-1);

#ifdef SPIFLASH_DEBUG
   	printf ("spiflash_eraseone_block (addr = 0x%08x, len = %d)\n", addr, len);
#endif
	printf("erase addr=%x size=%x\n", addr, mtd->erasesize);

	ptr_opcode = &stm_opcodes[SPI_SECTOR_ERASE];

	temp = ((__u32)addr << 8) | (__u32)(ptr_opcode->code);
	spiflash_sendcmd(SPI_WRITE_ENABLE);
	do {
		reg = spiflash_regread32(SPI_FLASH_CTL);
	} while (reg & SPI_CTL_BUSY);

	spiflash_regwrite32(SPI_FLASH_OPCODE, temp);

	if(mtd->size >= SIZE_32MiB){
		reg = ((__u32)addr & 0xff000000) |(reg & 0x00ffff00) | (ptr_opcode->tx_cnt+1) | SPI_CTL_START;
	}else{
	reg = (reg & ~SPI_CTL_TX_RX_CNT_MASK) | ptr_opcode->tx_cnt | SPI_CTL_START;
	}
	spiflash_regwrite32(SPI_FLASH_CTL, reg);

	do {
		reg = spiflash_sendcmd(SPI_RD_STATUS);
		if (!(reg & SPI_STATUS_WIP)) {
			finished = TRUE;
		}
	} while (!finished);

   	return (0);
}

static int
spiflash_erase_internal (struct mtd_info *mtd, unsigned long addr, unsigned long len)
{
	int ret;

#if 0
	if(addr & (mtd->erasesize - 1))
		return -1;
	if(len & (mtd->erasesize - 1))
		return -1;
#endif
#ifdef SPIFLASH_DEBUG
   	printf ("spiflash_erase (addr = 0x%08x, len = %d)\n", addr, len);
#endif

	if (addr & (mtd->erasesize - 1))
	  	addr = addr - (addr & (mtd->erasesize - 1));
	if (len & (mtd->erasesize - 1))
		len = len - (len & (mtd->erasesize - 1)) + mtd->erasesize;

	if(len + addr > mtd->size)
		return -1;

	while (len) {
#ifdef SPIFLASH_DEBUG
   		printf ("spiflash_erase (addr = 0x%08x, len = %d)\n", addr, len);
#endif
		ret = spiflash_erase_oneblock(mtd, addr, len);
		if (ret < 0) return ret;

		addr += mtd->erasesize;
		len -= mtd->erasesize;
	}

	return 0;
}

static int
spiflash_read_internal (struct mtd_info *mtd, unsigned long from,unsigned long len,unsigned long *retlen,unsigned char *buf)
{
	unsigned char	*read_addr;

#ifdef SPIFLASH_DEBUG
   	printf ("spiflash_read (from = 0x%08x, len = %d)\n", from, len);
#endif

   	/* sanity checks */
   	if (!len) return (0);
   	if (from + len > mtd->size) return (-1);


   	/* we always read len bytes */
   	*retlen = len;

	memcpy(buf, (char *)((mtd->offset + from)), len);

   	return (0);
}

static int
spiflash_write_internal (struct mtd_info *mtd,unsigned long to,unsigned long len,unsigned long *retlen,const unsigned char *buf)
{
	int done = FALSE, page_offset, bytes_left, finished;
	int cnt = 0;
#ifdef TC_SOC
	__u32 xact_len, spi_data[8], opcode, reg;
	__u32 reg_value;
	unsigned char words, bytes, finalrun, i, j;
#else
	__u32 xact_len, spi_data = 0, opcode, reg;
#endif

#ifdef SPIFLASH_DEBUG
   	printf ("spiflash_write (to = 0x%08x, len = %d)\n", to, len);
#endif

   	*retlen = 0;

   	/* sanity checks */
   	if (!len) return (0);
   	if (to + len > mtd->size) return (-1);

	printf("program from %x to %x\n", to, to+len);

	opcode = stm_opcodes[SPI_PAGE_PROGRAM].code;
	bytes_left = len;

	while (done == FALSE) {
		cnt++;
		if ((cnt & 0x3ff) == 0)
			printf(".");

		if (byte_program_mode)
			xact_len = MIN(bytes_left, sizeof(__u8));
		else
#ifdef TC_SOC
			xact_len = MIN(bytes_left, sizeof(__u32)<<3);
#else
			xact_len = MIN(bytes_left, sizeof(__u32));
#endif
		/* 32-bit writes cannot span across a page boundary
		 * (256 bytes). This types of writes require two page
		 * program operations to handle it correctly. The STM part
		 * will write the overflow data to the beginning of the
		 * current page as opposed to the subsequent page.
		 */
		page_offset = (to & (STM_PAGE_SIZE - 1)) + xact_len;

		if (page_offset > STM_PAGE_SIZE) {
			xact_len -= (page_offset - STM_PAGE_SIZE);
		}

		spiflash_sendcmd(SPI_WRITE_ENABLE);

		do {
			reg = spiflash_regread32(SPI_FLASH_CTL);
		} while (reg & SPI_CTL_BUSY);

#ifdef TC_SOC
		words = xact_len >> 2;
//		if(!(xact_len % 4))
		if(!(xact_len & 0x00000003))
			words--;
		bytes = 3;
//		finalrun = xact_len % 4 - 1;
		finalrun = (xact_len & 0x00000003) - 1;
		if(finalrun == 0xFF)
			finalrun = 3;
		for(i = 0; i <= words; i++){
			spi_data[i] = 0;//Make sure the initial value of spi_data[i] is 0
			if(i == words)
				bytes = finalrun;
			for(j = 0; j <= bytes; j++){
				spi_data[i] |= (buf[j + (i<<2)] << (j<<3));
			}
		}
#else
		switch (xact_len) {
			case 1:
			 	spi_data = (__u8) *buf;
				break;
			case 2:
				spi_data = (buf[1] << 8) | buf[0];
				break;
			case 3:
				spi_data = (buf[2] << 16) | (buf[1] << 8) | buf[0];
				break;
			case 4:
				spi_data = (buf[3] << 24) | (buf[2] << 16) |
							(buf[1] << 8) | buf[0];
				break;
			default:
				printf("spiflash_write: default case\n");
				break;
		}
#endif

#ifdef TC_SOC
        if (!byte_program_mode){
        	reg_value = VPint(SPI_REG_MASTER);
//			VPint(SPI_REG_MASTER) = 0x38804;//Set bit [2] to 1 to enter more buffer mode
			VPint(SPI_REG_MASTER) = reg_value | (1 << 2);
 //       	VPint(SPI_REG_MOREBUF) = 0x20000100;//Set bits [8:0] to 128 (data bit counts) and bits[29:24] to 32(comman bit counts)
        	/* write exact data size into flash */
			if(mtd->size >= SIZE_32MiB){
				VPint(SPI_REG_MOREBUF) = 0x28000000|(xact_len<<3);//Set bits [8:0] to data bit counts and bits[29:24] to 40(command bit counts)
				//printf("command more %x\n",VPint(SPI_REG_MOREBUF));
			}else{
           	VPint(SPI_REG_MOREBUF) = 0x20000000|(xact_len<<3);//Set bits [8:0] to data bit counts and bits[29:24] to 32(command bit counts)
			}
        }
#endif

#ifdef TC_SOC
		spiflash_regwrite32(SPI_FLASH_DATA, spi_data[0]);
		if (!byte_program_mode){
			spiflash_regwrite32(SPI_FLASH_DATA2, spi_data[1]);
			spiflash_regwrite32(SPI_FLASH_DATA3, spi_data[2]);
			spiflash_regwrite32(SPI_FLASH_DATA4, spi_data[3]);
			spiflash_regwrite32(SPI_FLASH_DATA5, spi_data[4]);
			spiflash_regwrite32(SPI_FLASH_DATA6, spi_data[5]);
			spiflash_regwrite32(SPI_FLASH_DATA7, spi_data[6]);
			spiflash_regwrite32(SPI_FLASH_DATA8, spi_data[7]);
			if(mtd->size >= SIZE_32MiB){
				opcode = ((__u32)to);
			}else{	
			opcode = (0x02 << 24) | ((__u32)to);
			}
		}
		else
#else
		spiflash_regwrite32(SPI_FLASH_DATA, spi_data);
#endif
		opcode = (opcode & SPI_OPCODE_MASK) | ((__u32)to << 8);

		spiflash_regwrite32(SPI_FLASH_OPCODE, opcode);

		//size is big than 32M. use four address. shnwind.
		if(mtd->size >= SIZE_32MiB){
#ifdef TC_SOC
			reg = ((0x02 << 24) | (reg & 0x00ffff00)) | (xact_len + 5) | SPI_CTL_START;
#else
			reg = ((__u32)to & 0xff000000 ) | (reg & 0x00ffff00) | (xact_len + 5) | SPI_CTL_START;
#endif	
		}else{
		reg = (reg & ~SPI_CTL_TX_RX_CNT_MASK) | (xact_len + 4) | SPI_CTL_START;
		}

		spiflash_regwrite32(SPI_FLASH_CTL, reg);
		finished = FALSE;

#ifdef TC_SOC
		if (!byte_program_mode){
			while(VPint(SPI_REG_BASE) & 0x10000);//Make sure the bit spi_master_busy is 0 and then continue
			VPint(SPI_REG_MOREBUF) = 0x00000000;//Set the default value back
//			VPint(SPI_REG_MASTER) = 0x38800;//Set the default value back
			VPint(SPI_REG_MASTER) = reg_value;
		}
#endif

		do {
			//delay1ms(1);
			reg = spiflash_sendcmd(SPI_RD_STATUS);
			if (!(reg & SPI_STATUS_WIP)) {
				finished = TRUE;
			}
		} while (!finished);

		bytes_left -= xact_len;
		to += xact_len;
		buf += xact_len;

   		*retlen += xact_len;

		if (bytes_left == 0) {
			done = TRUE;
		}
	}
	printf("\n");

	VPint(CR_SPI_CR)=1;

   	return (0);
}

static struct spi_flash_info flash_tables[] = {
	{
		mfr_id: MANUFACTURER_ST,
		dev_id: M25P64,
		name: "ST M25P64",
		DeviceSize: SIZE_8MiB,
		EraseSize: SIZE_64KiB,
		mode: SPI_STD_FAST_RD,
	},
	{
		mfr_id: MANUFACTURER_ST,
		dev_id: M25P32,
		name: "ST M25P32",
		DeviceSize: SIZE_4MiB,
		EraseSize: SIZE_64KiB,
		mode: SPI_STD_FAST_RD,
	},
	{
		mfr_id: MANUFACTURER_ST,
		dev_id: M25P16,
		name: "ST M25P16",
		DeviceSize: SIZE_2MiB,
		EraseSize: SIZE_64KiB,
		mode: SPI_STD_FAST_RD,
	},
	{
		mfr_id: MANUFACTURER_WINBOND,
		dev_id: W25X128,
		name: "Winbond W25X128",
		DeviceSize: SIZE_16MiB,
		EraseSize: SIZE_64KiB,
		mode: SPI_FAST_RD_DUAL_O,
	},
	{
		mfr_id: MANUFACTURER_WINBOND,
		dev_id: W25X64,
		name: "Winbond W25X64",
		DeviceSize: SIZE_8MiB,
		EraseSize: SIZE_64KiB,
		mode: SPI_FAST_RD_DUAL_O,
	},
	{
		mfr_id: MANUFACTURER_WINBOND,
		dev_id: W25X32,
		name: "Winbond W25X32",
		DeviceSize: SIZE_4MiB,
		EraseSize: SIZE_64KiB,
		mode: SPI_FAST_RD_DUAL_O,
	},
	{
		mfr_id: MANUFACTURER_WINBOND,
		dev_id: W25X16,
		name: "Winbond W25X16",
		DeviceSize: SIZE_2MiB,
		EraseSize: SIZE_64KiB,
		mode: SPI_FAST_RD_DUAL_O,
	},
	{
		mfr_id: MANUFACTURER_WINBOND,
		dev_id: W25Q128,
		name: "Winbond W25Q128",
		DeviceSize: SIZE_16MiB,
		EraseSize: SIZE_64KiB,
		mode: SPI_FAST_RD_QUAD_IO,
	},
	{
		mfr_id: MANUFACTURER_WINBOND,
		dev_id: W25Q64,
		name: "Winbond W25Q64",
		DeviceSize: SIZE_8MiB,
		EraseSize: SIZE_64KiB,
		mode: SPI_FAST_RD_QUAD_IO,
	},
	{
		mfr_id: MANUFACTURER_WINBOND,
		dev_id: W25Q32,
		name: "Winbond W25Q32",
		DeviceSize: SIZE_4MiB,
		EraseSize: SIZE_64KiB,
		mode: SPI_FAST_RD_QUAD_IO,
	},
	{
		mfr_id: MANUFACTURER_WINBOND,
		dev_id: W25Q16,
		name: "Winbond W25Q16",
		DeviceSize: SIZE_2MiB,
		EraseSize: SIZE_64KiB,
		mode: SPI_FAST_RD_QUAD_IO,
	},

	{
		mfr_id: MANUFACTURER_SST,
		dev_id: SST25VF032B,
		name: "SST 25VF032B",
		DeviceSize: SIZE_4MiB,
		EraseSize: SIZE_64KiB,
		mode: SPI_STD_FAST_RD,
	},
	{
		mfr_id: MANUFACTURER_MXIC,
		dev_id: MX25L3205D,
		name: "MX25L3205D",
		DeviceSize: SIZE_4MiB,
		EraseSize: SIZE_64KiB,
		mode: SPI_STD_FAST_RD,
	},
	{
		mfr_id: MANUFACTURER_MXIC,
		dev_id: MX25L6405D,
		name: "MX25L6405D",
		DeviceSize: SIZE_8MiB,
		EraseSize: SIZE_64KiB,
		mode: SPI_STD_FAST_RD,
	},
	{
		mfr_id: MANUFACTURER_MXIC,
		dev_id: MX25L12805D,
		name: "MX25L12805D",
		DeviceSize: SIZE_16MiB,
		EraseSize: SIZE_64KiB,
		mode: SPI_STD_FAST_RD,
	},
	{
		mfr_id: MANUFACTURER_MXIC,
		dev_id: MX25L25635E,
		name: "MX25L25635DE",
		DeviceSize: SIZE_32MiB,
		EraseSize: SIZE_64KiB,
		mode: SPI_FAST_RD_QUAD_IO,
	},
	{
		mfr_id: MANUFACTURER_SPANSION,
		dev_id: S25FL064A,
		name: "S25FL064A",
		DeviceSize: SIZE_8MiB,
		EraseSize: SIZE_64KiB,
		mode: SPI_STD_FAST_RD,
	},
	{
		mfr_id: MANUFACTURER_SPANSION,
		dev_id: S25FL032A,
		name: "S25FL032A",
		DeviceSize: SIZE_4MiB,
		EraseSize: SIZE_64KiB,
		mode: SPI_STD_FAST_RD,
	},
	{
		mfr_id: MANUFACTURER_SPANSION,
		dev_id: S25FL016A,
		name: "S25FL016A",
		DeviceSize: SIZE_2MiB,
		EraseSize: SIZE_64KiB,
		mode: SPI_STD_FAST_RD,
	},
	{
		mfr_id: MANUFACTURER_SPANSION,
		dev_id: S25FL128P,
		name: "S25FL128P",
		DeviceSize: SIZE_16MiB,
		EraseSize: SIZE_64KiB,
		mode: SPI_STD_FAST_RD,
	},
	{
		mfr_id: MANUFACTURER_EON,
		dev_id: EN25Q64,
		name: "EN25Q64",
		DeviceSize: SIZE_8MiB,
		EraseSize: SIZE_64KiB,
		mode: SPI_FAST_RD_QUAD_IO,
	},
	{
		mfr_id: MANUFACTURER_NUMONYX,
		dev_id: N25Q064,
		name: "N25Q064",
		DeviceSize: SIZE_8MiB,
		EraseSize: SIZE_64KiB,
		mode: SPI_STD_RD,

	},
	{
		mfr_id: MANUFACTURER_SPANSION,
		dev_id: S25FL016K,
		name: "S25FL016K",
		DeviceSize: SIZE_2MiB,
		EraseSize: SIZE_64KiB,
		mode: SPI_FAST_RD_DUAL_IO,
	},
	{
		mfr_id: MANUFACTURER_GIGADEVICE,
		dev_id: GD25Q64,
		name: "GD25Q64",
		DeviceSize: SIZE_8MiB,
		EraseSize: SIZE_64KiB,
		mode: SPI_FAST_RD_DUAL_IO,
	},
	{
		mfr_id: MANUFACTURER_GIGADEVICE,
		dev_id: GD25Q32,
		name: "GD25Q32",
		DeviceSize: SIZE_4MiB,
		EraseSize: SIZE_64KiB,
		mode: SPI_FAST_RD_DUAL_IO,
	},
	{
		mfr_id: MANUFACTURER_GIGADEVICE,
		dev_id: GD25Q128,
		name: "GD25Q128",
		DeviceSize: SIZE_16MiB,
		EraseSize: SIZE_64KiB,
		mode: SPI_FAST_RD_QUAD_IO,
	},
};

static void spiflash_unlock(void)
{
	struct opcodes *ptr_opcode;
	__u32 reg;
	int finished = FALSE;
	__u32 status_reg;

	status_reg = spiflash_sendcmd(SPI_RD_STATUS);
	ptr_opcode = &stm_opcodes[SPI_WR_STATUS];

	spiflash_sendcmd(SPI_WRITE_ENABLE);
	do {
		reg = spiflash_regread32(SPI_FLASH_CTL);
	} while (reg & SPI_CTL_BUSY);

	status_reg &= ~0x1c;
	spiflash_regwrite32(SPI_FLASH_DATA, status_reg);
	spiflash_regwrite32(SPI_FLASH_OPCODE, ptr_opcode->code);

	reg = (reg & ~SPI_CTL_TX_RX_CNT_MASK) | ptr_opcode->tx_cnt | SPI_CTL_START;
	spiflash_regwrite32(SPI_FLASH_CTL, reg);
}

static void spiflash_quadenable(void)
{
	struct opcodes *ptr_opcode;
	__u32 reg;
	__u32 reg2;
	int finished = FALSE;
	__u32 status_reg;
	__u32 status_reg2;

	reg = spiflash_regread32(SPI_FLASH_MM);
	reg |= 0x2;
	spiflash_regwrite32(SPI_FLASH_MM, reg);

	status_reg = spiflash_sendcmd(SPI_RD_STATUS);
	status_reg2 = spiflash_sendcmd(SPI_RD_STATUS2);
	ptr_opcode = &stm_opcodes[SPI_WR_STATUS];

	spiflash_sendcmd(SPI_WRITE_ENABLE);
	do {
		reg = spiflash_regread32(SPI_FLASH_CTL);
	} while (reg & SPI_CTL_BUSY);

	status_reg2 |= STM_STATUS_QE;
	spiflash_regwrite32(SPI_FLASH_DATA, status_reg);
	reg2 = ((status_reg & 0xff)<<24) | ((status_reg2 & 0xff)<<16) | ptr_opcode->code;
	spiflash_regwrite32(SPI_FLASH_OPCODE, reg2);

	reg = (reg & ~SPI_CTL_TX_RX_CNT_MASK) | (ptr_opcode->tx_cnt+1) | SPI_CTL_START;
	spiflash_regwrite32(SPI_FLASH_CTL, reg);
}

void spiflash_qe(int hpm)
{
	if (hpm)
		spiflash_sendcmd(SPI_HPM_ENABLE);
	else
		spiflash_sendcmd(SPI_RD_SIG);

	spiflash_quadenable();
}

void spiflash_rdst(void)
{
	__u32 status_reg;
	__u32 status_reg2;

	spidbg = 1;
	status_reg = spiflash_sendcmd(SPI_RD_STATUS);
   	printf ("status_reg =%08x\n", status_reg);
	status_reg2 = spiflash_sendcmd(SPI_RD_STATUS2);
   	printf ("status_reg2=%08x\n", status_reg2);
	spidbg = 0;
}

static int spiflash_probe(void)
{
	int i;
	unsigned long flash_id;

	flash_id = spiflash_read_id();

	for (i=0; i < (sizeof(flash_tables)/sizeof(struct spi_flash_info)); i++) {
		if ((MANUFACTURER_ID(flash_id) == flash_tables[i].mfr_id) &&
		    (DEVICE_ID(flash_id) == flash_tables[i].dev_id)) {
			if (MANUFACTURER_ID(flash_id) == MANUFACTURER_SST) {
				spiflash_unlock();
				byte_program_mode = 1;
			}
			return i;
		}
	}
	return -1;
}

int spiflash_erase(unsigned long addr, unsigned long size)
{
	return spiflash_erase_internal(&mtd, addr, size);
}

int spiflash_read(unsigned long from,
	unsigned long len, unsigned long *retlen, unsigned char *buf)
{
	return spiflash_read_internal(&mtd, from, len, retlen, buf);
}

int spiflash_write(unsigned long to,
	unsigned long len, unsigned long *retlen, const unsigned char *buf)
{
	return spiflash_write_internal(&mtd, to, len, retlen, buf);
}

#define SPI_ADDR_SIZE_16BITS		1
#define SPI_ADDR_SIZE_24BITS		2
#ifdef TCSUPPORT_DUAL_IMAGE_ENHANCE
int offset = 0;
#endif
int
spiflash_init (unsigned long rom_base)
{
   	int index;
	int spi_read_bytes;

	/* turn off flash write protect */
	//tc_outl(CR_GPIO_DATA, tc_inl(CR_GPIO_DATA) | (1 << 5));

	mtd.offset = rom_base;
	mtd.size = 0;

   	index = spiflash_probe();
	if (index < 0) {
    	printf ("%s: Found no SPI flash device\n", module_name);
    	return -1;
   	}

        if(isRT63365 || isMT751020){
		//printf("SPI value %x\n",(VPint(0xbfb0008c) & (1<<27)));
		if(VPint(0xbfb0008c) & (1<<27)){ //4 byte mode
			VPint(SPI_REG_BASE) |= ((1<<19)|(1<<20)); //set addr support spi_addr ext.
		}else{
			if(flash_tables[index].DeviceSize >= SIZE_32MiB){ //3 to 4 byte mode
				VPint(SPI_REG_BASE) |= ((1<<19)|(1<<20));//set addr support spi_addr ext.
				VPint(SPI_REG_BASE + 0x30) &= ~((1<<8)|(1<<9)); //set support 25 bits.
				spiflash_sendcmd(SPI_EN4B); //send En4B command
			}	
		}
	}else{
		if(flash_tables[index].DeviceSize >= SIZE_32MiB){
			return -1;	
		}
	}
//#ifdef CONFIG_DUAL_IMAGE
#if 0
	flash_index = index;
#endif
#ifdef TC3262
	/* TC3182LDV doesn't support quad mode */
	if (isTC3182 || isRT65168 || isRT63165 || isRT63365 || isMT751020) {
#else
	/* Note: TC3162U doesn't support quad mode */ 
	if (isTC3162U || isRT63260 || isTC3162L5P5) {            //TC3162L2M does not support Quad Mode neither, added by fredli, bug#10664          
#endif
		switch (flash_tables[index].mode) {
			case SPI_FAST_RD_QUAD_O:
				flash_tables[index].mode = SPI_FAST_RD_DUAL_O;
				break;
			case SPI_FAST_RD_QUAD_IO:
				flash_tables[index].mode = SPI_FAST_RD_DUAL_IO;
				break;
			case SPI_BURST_RD_QUAD_IO:
				flash_tables[index].mode = SPI_FAST_RD_DUAL_IO;
				break;
		}
	}

	/* set spi serial mode */
	switch (flash_tables[index].mode) {
		case SPI_STD_RD:
			spi_read_bytes = 4;
			break;
		case SPI_STD_FAST_RD:
		case SPI_FAST_RD_DUAL_O:
		case SPI_FAST_RD_DUAL_IO:
		case SPI_FAST_RD_QUAD_O:
			spi_read_bytes = 5;
			break;
		case SPI_FAST_RD_QUAD_IO:
			spi_read_bytes = 7;
			break;
		case SPI_BURST_RD_QUAD_IO:
			spi_read_bytes = 5;
			break;
		default:
			spi_read_bytes = 4;
			break;
	}

	if(flash_tables[index].DeviceSize >= SIZE_32MiB){
		spi_read_bytes++;	
	}	
	switch (flash_tables[index].mode) {
		case SPI_FAST_RD_QUAD_O:
		case SPI_FAST_RD_QUAD_IO:
		case SPI_BURST_RD_QUAD_IO:
			spiflash_quadenable();
			printf("SPI Flash Quad Enable\r\n");
			break;
	}

	tc_outl(CR_SPI_QUECTL, (tc_inl(CR_SPI_QUECTL) & 0xffffff00) | (spi_read_bytes<<4) | (flash_tables[index].mode));

#ifdef TC3262
	/* don't adjust TC3182 or TC3262's spi clock */
	if (isRT63365){
		/*63368 Flash space clk = 233/(5+2) Mhz, 63365 Flash space clk = 166/(4+2) Mhz*/
		if (isRT63368)
			VPint(SPI_REG_SPACE_CR) = (VPint(SPI_REG_SPACE_CR) & 0xfffffff0) | (0x5);
		else	
			VPint(SPI_REG_SPACE_CR) = (VPint(SPI_REG_SPACE_CR) & 0xfffffff0) | (0x4);
	}else if (isRT65168 || isTC3182){
		/*3182 65168 Flash space clk = 102/(1+2) Mhz*/
		VPint(SPI_REG_SPACE_CR) = (VPint(SPI_REG_SPACE_CR) & 0xfffffff0) | (0x1);
	}
#else
	/* Set Register Space SPI clock frequency is hclk/5 */
	VPint(SPI_REG_MASTER) = (VPint(SPI_REG_MASTER) & 0xf000ffff) | (0x3 << 16);
	
	/* Set Flash Space SPI clock frequency is hclk/5 */
	if (isTC3162L5P5)
		VPint(SPI_REG_SPACE_CR) = (VPint(SPI_REG_SPACE_CR) & 0xfffff000) | 0x3;
#endif

	printf("Found SPI Flash %dMiB %s at 0x%x\r\n", 
		(flash_tables[index].DeviceSize)/(1024*1024), flash_tables[index].name, rom_base);

   	mtd.size = flash_tables[index].DeviceSize;
   	mtd.erasesize = flash_tables[index].EraseSize;
#ifdef TCSUPPORT_DUAL_IMAGE_ENHANCE
#ifdef TCSUPPORT_DUAL_IMAGE_8M
	offset = 0x3e0000;
#else
	offset = flash_tables[index].DeviceSize/2;
#endif
#endif
   	return 0;
}

#ifdef SPI_DRAM_TEST_CMD
unsigned long spiflash_sizeGet(void)
{
	int index;

   	index = spiflash_probe();
	if (index < 0) {
    	printf ("%s: Found no SPI flash device\n", module_name);
    	return -1;
   	}

	return flash_tables[index].DeviceSize;
}

int spiflash_cmp(void *dstAddr, void *srcAddr, unsigned long size, unsigned char rByte)
{
	unsigned char *dstAddr1;
	unsigned char *srcAddr1;
	unsigned short *dstAddr2;
	unsigned short *srcAddr2;
	unsigned long *dstAddr4;
	unsigned long *srcAddr4;

	if((dstAddr == 0) || (dstAddr == 0))
	{
		prom_printf("spiflash_cmp: input data ERROR!\n");
		return -1;
	}

	switch(rByte)
	{
		case 4:
			dstAddr4 = (unsigned long*)dstAddr;
			srcAddr4 = (unsigned long*)srcAddr;
			prom_printf("spiflash_cmp: dstAddr=0x%x srcAddr=0x%x read byte=%d size=0x%x\n", dstAddr4, srcAddr4, rByte, size);
			size = size >> 2;
			break;
		case 2:
			dstAddr2 = (unsigned short*)dstAddr;
			srcAddr2 = (unsigned short*)srcAddr;
			prom_printf("spiflash_cmp: dstAddr=0x%x srcAddr=0x%x read byte=%d size=0x%x\n", dstAddr2, srcAddr2, rByte, size);
			size = size >> 1;
			break;
		case 1:
			dstAddr1 = (unsigned char*)dstAddr;
			srcAddr1 = (unsigned char*)srcAddr;
			prom_printf("spiflash_cmp: dstAddr=0x%x srcAddr=0x%x read byte=%d size=0x%x\n", dstAddr1, srcAddr1, rByte, size);
			break;
		default:
			prom_printf("spiflash_cmp: ERROR! read byte=%d \n", rByte);
			return -1;
	}


	/* compare flash with dram content */
	while(size)
	{
		switch(rByte)
		{
			case 4:
				if(*dstAddr4 != *srcAddr4)
				{
					prom_printf("spiflash_cmp ERROR! dstAddr=0x%x result=0x%X srcAddr=0x%X result=0x%X byte=%d\n", dstAddr4, *dstAddr4, srcAddr4, *srcAddr4, rByte);
					return -1;
				}
				dstAddr4++;
				srcAddr4++;
				break;
			case 2:
				if(*dstAddr2 != *srcAddr2)
				{
					prom_printf("spiflash_cmp ERROR! dstAddr=0x%x result=0x%X srcAddr=0x%X result=0x%X byte=%d\n", dstAddr2, *dstAddr2, srcAddr2, *srcAddr2, rByte);
					return -1;
				}
				dstAddr2++;
				srcAddr2++;
				break;
			case 1:
				if(*dstAddr1 != *srcAddr1)
				{
					prom_printf("spiflash_cmp ERROR! dstAdd4=0x%x result=0x%X srcAddr=0x%X result=0x%X byte=%d\n", dstAddr1, *dstAddr1, srcAddr1, *srcAddr1, rByte);
					return -1;
				}
				dstAddr1++;
				srcAddr1++;
				break;
			default:
				prom_printf("spiflash_cmp: ERROR! read byte=%d \n", rByte);
				return -1;
		}

		size--;
	}

	return 0;
}


int spiFlashTest(spiFlashTest_info_t *info, int isInputPat)
{
#define DEF_DRAM_ADDR 0x80020000

	int ret = 0;
	unsigned char defWByte = 1;
	unsigned char defRByte[] = {1, 2 , 4 , 0};
	int i,j;
	unsigned char defPat[4] = {0x5a, 0xa5, 0x00, 0xff};
	unsigned long retlen;
	unsigned long testSize;
	unsigned long flashBaseAddrBlock1 = 0;
	unsigned long flashBaseAddrBlock2 = 0;
	unsigned long flashVirAddr[2] = {0};
	unsigned long addr;


	if(info == 0)
	{
		prom_printf("spiFlashTest: ERROR! no test info \n");
		return -1;
	}

	if(isMT751020){
		flashBaseAddrBlock1 = 0xbc000000;
	}else{
		flashBaseAddrBlock1 = 0xb0000000;
	}
	flashBaseAddrBlock2 = 0xbfc00000;
	flashVirAddr[0] = flashBaseAddrBlock1;
	flashVirAddr[1] = flashBaseAddrBlock2;

	/* the max size of block start with 0xbfc0000 is 4MB */
	testSize = info->size;
	if(info->startAddr + info->size > SIZE_4MiB)
	{
		testSize = SIZE_4MiB - info->startAddr;
	}

	if(isInputPat) /* use the input pattern and the default write byte is 1 */
	{
		dram_pat_set(DEF_DRAM_ADDR, info->size, info->pattern, ORI_PAT, defWByte);
	}
	else /* use the default patterns, incremental pattern */
	{
		dram_pat_set(DEF_DRAM_ADDR, info->size, 0, INCR_PAT, defWByte);
	}
	/* erase and write dram contents to flash and then compare them by 1-byte-read */
	prom_printf("Write to flash from %X to %X with %X bytes\n", info->startAddr, DEF_DRAM_ADDR, info->size);
	flash_erase(info->startAddr, info->size);
	flash_write(info->startAddr, info->size, &retlen, DEF_DRAM_ADDR);
	for(j=0; defRByte[j]!=0; j++)
	{
		ret += spiflash_cmp(flashBaseAddrBlock1 + info->startAddr, DEF_DRAM_ADDR, info->size, defRByte[j]);
		ret += spiflash_cmp(flashBaseAddrBlock2 + info->startAddr, DEF_DRAM_ADDR, testSize, defRByte[j]);
	}
	prom_printf("\n");


	/* use the default patterns, "anti-incremental patten, 0x5a, 0xa5, 0x0, 0xff" */
	if(isInputPat == 0)
	{
		dram_pat_set(DEF_DRAM_ADDR, info->size, 0, ANTI_INCR_PAT, defWByte);
		prom_printf("Write to flash from %X to %X with %X bytes\n", info->startAddr, DEF_DRAM_ADDR, info->size);
		flash_erase(info->startAddr, info->size);
		flash_write(info->startAddr, info->size, &retlen, DEF_DRAM_ADDR);
		ret += spiflash_cmp(flashBaseAddrBlock1 + info->startAddr, DEF_DRAM_ADDR, info->size, 1);
		ret += spiflash_cmp(flashBaseAddrBlock2 + info->startAddr, DEF_DRAM_ADDR, testSize, 1);
		prom_printf("\n");

		for(i=0; i<4; i++)
		{
			dram_pat_set(DEF_DRAM_ADDR, info->size, defPat[i], ORI_PAT, defWByte);
			prom_printf("Write to flash from %X to %X with %X bytes\n", info->startAddr, DEF_DRAM_ADDR, info->size);
			flash_erase(info->startAddr, info->size);
			flash_write(info->startAddr, info->size, &retlen, DEF_DRAM_ADDR);
			ret += spiflash_cmp(flashBaseAddrBlock1 + info->startAddr, DEF_DRAM_ADDR, info->size, 1);
			ret += spiflash_cmp(flashBaseAddrBlock2 + info->startAddr, DEF_DRAM_ADDR, testSize, 1);
			prom_printf("\n");
		}
	}

	/* copy the content in flash 0~FLASH_START_TEST to dram and compare them */
	prom_printf("TEST flash 0x0~0x%x\n\n",FLASH_START_TEST);
	for(i=0; i<2; i++)
	{
		addr =  flashVirAddr[i];
		memcpy4(DEF_DRAM_ADDR, addr, FLASH_START_TEST);
		for(j=0; defRByte[j]!=0; j++)
		{
			ret += spiflash_cmp(addr, DEF_DRAM_ADDR, FLASH_START_TEST, defRByte[j]);
		}
	}

	if(ret != 0)
	{
		prom_printf("#flashTest FAIL!\n\n");
		return -1;
	}
	else
	{
		prom_printf("#flashTest SUCCESS!\n\n");
		return 0;
	}
}

int gdmaTestFlash(gdmaTest_info_t *info)
{
#define DEF_DRAM_ADDR 0xa0020000
#define DRAM_BASE_ADDR_KSEG1 0xa0000000
#define TEST_LENGTH 0x10000

	int ret = 0;
	int i;
	unsigned char defPat[4] = {0x5a, 0xa5, 0x00, 0xff};
	unsigned long retlen;
	unsigned long flashAddr;
	unsigned long virFlashAddr;
	unsigned long virDramAddr;
	unsigned long flashBaseAddrBlock1 = 0;

	if(info == 0)
	{
		prom_printf("gdmaTestFlash: ERROR! no test info \n");
		return -1;
	}

	if(isMT751020){
		flashBaseAddrBlock1 = 0xbc000000;
	}else{
		flashBaseAddrBlock1 = 0xb0000000;
	}

	flashAddr = info->sa - PHYSADDR(flashBaseAddrBlock1);
	virFlashAddr =  flashAddr + flashBaseAddrBlock1;
	virDramAddr =  flashAddr + DRAM_BASE_ADDR_KSEG1;

	/* use the default patterns, incremental pattern */
	dram_pat_set(DEF_DRAM_ADDR, TEST_LENGTH, 0, INCR_PAT, 1);
	/* erase and write dram contents to flash */
	prom_printf("Write to flash from %X to %X with %X bytes\n", DEF_DRAM_ADDR, flashAddr, TEST_LENGTH);
	flash_erase(flashAddr, TEST_LENGTH);
	flash_write(flashAddr, TEST_LENGTH, &retlen, DEF_DRAM_ADDR);
	gdmaTestCpy(info->sa, info->da, info->len, info->burst_size, info->wswap);

	ret += memcmp((const unsigned char *) virDramAddr, virFlashAddr, info->len);

	/* use the default patterns, "anti-incremental patten, 0x5a, 0xa5, 0x0, 0xff" */
	dram_pat_set(DEF_DRAM_ADDR, TEST_LENGTH, 0, ANTI_INCR_PAT, 1);
	prom_printf("Write to flash from %X to %X with %X bytes\n", flashAddr, DEF_DRAM_ADDR, TEST_LENGTH);
	flash_erase(flashAddr, TEST_LENGTH);
	flash_write(flashAddr, TEST_LENGTH, &retlen, DEF_DRAM_ADDR);
	gdmaTestCpy(info->sa, info->da, info->len, info->burst_size, info->wswap);

	ret += memcmp(virDramAddr, virFlashAddr, info->len);

	for(i=0; i<4; i++)
	{
		dram_pat_set(DEF_DRAM_ADDR, TEST_LENGTH, defPat[i], ORI_PAT, 1);
		prom_printf("Write to flash from %X to %X with %X bytes\n", flashAddr, DEF_DRAM_ADDR, TEST_LENGTH);
		flash_erase(flashAddr, TEST_LENGTH);
		flash_write(flashAddr, TEST_LENGTH, &retlen, DEF_DRAM_ADDR);
		gdmaTestCpy(info->sa, info->da, info->len, info->burst_size, info->wswap);

		ret += memcmp(virDramAddr, virFlashAddr, info->len);
	}


	if(ret != 0)
	{
		prom_printf("gdmaTestFlash FAIL!\n\n");
		return -1;
	}
	else
	{
		prom_printf("gdmaTestFlash SUCCESS!\n\n");
		return 0;
	}
}

#endif

