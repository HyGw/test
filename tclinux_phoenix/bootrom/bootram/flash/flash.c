#include <asm/io.h>
#include <asm/tc3162.h>

//#define SUPPORT_SST_FLASH	1

#define printf	prom_printf

#define MAX_CHIPS             6

#define DEVICE_TYPE_X8	(8 / 8)
#define DEVICE_TYPE_X16	(16 / 8)
#define DEVICE_TYPE_X32	(32 / 8)

/* Addresses */
#define ADDR_MANUFACTURER		0x0000
#define ADDR_DEVICE_ID			0x0001
#define ADDR_SECTOR_LOCK		0x0002
#define ADDR_HANDSHAKE			0x0003

#ifdef SUPPORT_SST_FLASH	
#define ADDR_UNLOCK_1			0x5555
#define ADDR_UNLOCK_2			0x2AAA
#else
#define ADDR_UNLOCK_1			0x0555
#define ADDR_UNLOCK_2			0x02AA
#endif


/* Commands */
#define CMD_UNLOCK_DATA_1				0x00AA
#define CMD_UNLOCK_DATA_2				0x0055
#define CMD_MANUFACTURER_UNLOCK_DATA	0x0090
#define CMD_UNLOCK_BYPASS_MODE			0x0020
#define CMD_PROGRAM_UNLOCK_DATA			0x00A0
#define CMD_RESET_DATA					0x00F0
#define CMD_SECTOR_ERASE_UNLOCK_DATA	0x0080
#define CMD_SECTOR_ERASE_UNLOCK_DATA_2	0x0030

#define CMD_UNLOCK_SECTOR		0x0060


/* Manufacturers */
#define MANUFACTURER_AMD		0x0001
#define MANUFACTURER_FUJITSU	0x0004
#define MANUFACTURER_ATMEL		0x001f
#define MANUFACTURER_ST			0x0020
#define MANUFACTURER_TOSHIBA	0x0098
#define MANUFACTURER_HYNIX		0x00ad
#define MANUFACTURER_SST		0x00bf
#define MANUFACTURER_MXIC		0x00C2
#define MANUFACTURER_WINBOND	0xddda

/* AMD */
#define AM29F040B			0x00a4
#define AM29F800BB			0x2258
#define AM29F800BT			0x22d6
#define AM29LV040B			0x004f
#define AM29LV800BB			0x225b
#define AM29LV800BT			0x22da
#define AM29LV160BT_DT		0x22c4
#define AM29LV160BB_DB		0x2249
#define AM29LV320BB_DB		0x22f9

/* MXIC */
#define MX29LV800B			0x225B
#define MX29LV160AB			0x2249
#define MX29LV320AB			0x22A8

/* Winbond */
#define W19B320ATB			0x227e

/* Fujitsu */
#define MBM29LV160TE		0x22C4
#define MBM29LV160BE		0x2249

/* HYNIX */
#define HY29F040A			0x00a4
#define HY29LV160T			0x22c4
#define HY29LV160B			0x2249

/* SST */
#define SST39VF040         	0x00D7
#define SST39VF080          0x00D8
#define SST39VF016          0x00D9
#define SST39VF3201         0x235B

/* ATMEL */
#define ATMEL_AT49BV163A 	0x00c0 

#define D6_MASK	            0x40

struct erase_region {
	unsigned long offset;
	unsigned long erasesize;
	unsigned long numblocks;
};

struct flchip {
	unsigned long start;	/* Offset within the map */
	unsigned long size;
	int buswidth;		/* in octets */
	struct mtd_info *mtd;
	int numeraseregions;
	struct erase_region regions[4];
};

struct mtd_info {
	unsigned long offset; /* Offset within the memory */
	unsigned long size;   /* Total size of the MTD */
	int numchips;	
	struct flchip chips[MAX_CHIPS];
};

struct flash_info {
	const unsigned short mfr_id;
	const unsigned short dev_id;
	const char *name;
	const unsigned long size;
	const int numeraseregions;
	const struct erase_region regions[4];
};

int amdflash_init(unsigned long rom_base);
int amdflash_read(unsigned long from,
	unsigned long len, unsigned long *retlen, unsigned char *buf);
int amdflash_write(unsigned long to,
	unsigned long len, unsigned long *retlen, const unsigned char *buf);
int amdflash_erase(unsigned long addr, unsigned long size);


const struct flash_info table[] = {
	{
		mfr_id: MANUFACTURER_AMD,
		dev_id: AM29LV160BB_DB,
		name: "AM29LV160BB/DB",
		size: 0x00200000,
		numeraseregions: 4,
		regions: {
			{ offset: 0x000000, erasesize: 0x04000, numblocks:  1 },
			{ offset: 0x004000, erasesize: 0x02000, numblocks:  2 },
			{ offset: 0x008000, erasesize: 0x08000, numblocks:  1 },
			{ offset: 0x010000, erasesize: 0x10000, numblocks: 31 }
		}
	},
	{
		mfr_id: MANUFACTURER_AMD,
		dev_id: AM29LV320BB_DB,
		name: "AM29LV320BB/DB",
		size: 0x00400000,
		numeraseregions: 2,
		regions: {
			{ offset: 0x000000, erasesize: 0x02000, numblocks:  8 },
			{ offset: 0x010000, erasesize: 0x10000, numblocks: 63 }
		}
	},
	{
		mfr_id: MANUFACTURER_MXIC,
		dev_id: MX29LV160AB,
		name: "MX29LV160AB",
		size: 0x00200000,
		numeraseregions: 4,
		regions: {
			{ offset: 0x000000, erasesize: 0x04000, numblocks:	1 },
			{ offset: 0x004000, erasesize: 0x02000, numblocks:	2 },
			{ offset: 0x008000, erasesize: 0x08000, numblocks:	1 },
			{ offset: 0x010000, erasesize: 0x10000, numblocks: 31 }
		}
	},
	{
		mfr_id: MANUFACTURER_MXIC,
		dev_id: MX29LV320AB,
		name: "MX29LV320AB",
		size: 0x00400000,
		numeraseregions: 2,
		regions: {
       		{ offset: 0x000000, erasesize: 0x02000, numblocks:  8 }, 
           	{ offset: 0x010000, erasesize: 0x10000, numblocks: 63 }
		}
	},
	{
		mfr_id: MANUFACTURER_WINBOND,
		dev_id: W19B320ATB,
		name: "W19B320ATB",
		size: 0x00400000,
		numeraseregions: 2,
		regions: {
       		{ offset: 0x000000, erasesize: 0x02000, numblocks:  8 }, 
           	{ offset: 0x010000, erasesize: 0x10000, numblocks: 63 }
		}
	},
	{
		mfr_id: MANUFACTURER_FUJITSU,
		dev_id: MBM29LV160BE,
		name: "Fujitsu MBM29LV160BE",
		size: 0x00200000,
		numeraseregions: 4,
		regions: {
			{ offset: 0x000000, erasesize: 0x04000, numblocks:  1 },
			{ offset: 0x004000, erasesize: 0x02000, numblocks:  2 },
			{ offset: 0x008000, erasesize: 0x08000, numblocks:  1 },
			{ offset: 0x010000, erasesize: 0x10000, numblocks: 31 }
		}
	},
	{ 
   		mfr_id: MANUFACTURER_ATMEL, 
       	dev_id: ATMEL_AT49BV163A, 
       	name: "AT49BV163A", 
       	size: 0x00200000, 
       	numeraseregions: 3, 
        regions: { 
       		{ offset: 0x000000, erasesize: 0x02000, numblocks:  8 }, 
           	{ offset: 0x010000, erasesize: 0x08000, numblocks:  2 }, 
           	{ offset: 0x020000, erasesize: 0x10000, numblocks: 30 }, 
        } 
    },         
	{
		mfr_id: MANUFACTURER_HYNIX,
		dev_id: HY29LV160B,
		name: "HY29LV160B",
		size: 0x00200000,
		numeraseregions: 4,
		regions: {
			{ offset: 0x000000, erasesize: 0x04000, numblocks:	1 },
			{ offset: 0x004000, erasesize: 0x02000, numblocks:	2 },
			{ offset: 0x008000, erasesize: 0x08000, numblocks:	1 },
			{ offset: 0x010000, erasesize: 0x10000, numblocks: 31 }
		}
	},
	{
		mfr_id: MANUFACTURER_SST,
		dev_id: SST39VF016,
		name: "SST39VF016",
		size: 0x00200000,
		numeraseregions: 1,
		regions: {
			{ offset: 0x000000, erasesize: 0x01000, numblocks: 512 }
		}
	},
	{
		mfr_id: MANUFACTURER_SST,
		dev_id: SST39VF3201,
		name: "SST39VF3201",
		size: 0x00400000,
		numeraseregions: 1,
		regions: {
			/*
			{ offset: 0x000000, erasesize: 0x10000, numblocks: 64 } 
			*/
			{ offset: 0x000000, erasesize: 0x01000, numblocks: 1024 }
		}
	}
};

struct mtd_info mtd;

static void
delay1ms(
	int ms
)
{
	volatile uint32 timer_now, timer_last;
	volatile uint32 tick_acc;
	uint32 one_tick_unit = 1 * get_SYS_HCLK() * 1000 / 2;
	volatile uint32 tick_wait = ms * one_tick_unit; 
	volatile uint32 timer1_ldv = VPint(CR_TIMER1_LDV);

	tick_acc = 0;
	timer_last = VPint(CR_TIMER1_VLR);
	do {
		timer_now = VPint(CR_TIMER1_VLR);
	  	if (timer_last >= timer_now) 
	  		tick_acc += timer_last - timer_now;
		else
			tick_acc += timer1_ldv - timer_now + timer_last;
		timer_last = timer_now;
	} while (tick_acc < tick_wait);
}

static unsigned long wide_read(struct flchip *chip, unsigned long addr)
{
	unsigned long mem_addr;

	mem_addr = chip->mtd->offset + chip->start + addr;
	//mem_addr |= NOCACHE_BIT;
	//mem_addr |= NOCACHE_BIT;

	switch (chip->buswidth) {
	case 1:
		return *((volatile unsigned char *)mem_addr);
	case 2:
		return *((volatile unsigned short *)mem_addr);
	case 4:
		return *((volatile unsigned long *)mem_addr);
	default:
		printf("wide_read(): unsupported buswidth\r\n");
	}

	return 0;
}

static void wide_write(struct flchip *chip, unsigned long val, unsigned long addr)
{
	unsigned long mem_addr;

	mem_addr = chip->mtd->offset + chip->start + addr;
	//mem_addr |= NOCACHE_BIT;
	//mem_addr |= NOCACHE_BIT;

	switch (chip->buswidth) {
	case 1:
		*((volatile unsigned char *)mem_addr) = val;
		break;
	case 2:
		*((volatile unsigned short *)mem_addr) = val;
		break;
	case 4:
		*((volatile unsigned long *)mem_addr) = val;
		break;
	default:
		printf("wide_write(): unsupported buswidth\r\n");
		break;
	}
}

static unsigned long make_cmd(struct flchip *chip, unsigned long cmd)
{
	return cmd;
}

static void send_unlock(struct flchip *chip)
{
	wide_write(chip, make_cmd(chip, CMD_UNLOCK_DATA_1), chip->buswidth * ADDR_UNLOCK_1);
	wide_write(chip, make_cmd(chip, CMD_UNLOCK_DATA_2), chip->buswidth * ADDR_UNLOCK_2);
}

static void send_cmd(struct flchip *chip, unsigned long cmd)
{
	send_unlock(chip);
	wide_write(chip, make_cmd(chip, cmd), chip->buswidth * ADDR_UNLOCK_1);
}

static void send_cmd_to_addr(struct flchip *chip, unsigned long cmd, unsigned long addr)
{
	send_unlock(chip);
	wide_write(chip, make_cmd(chip, cmd), addr);
}

static int probe_chip(struct mtd_info *mtd, unsigned long base, struct flchip *chip)
{
	unsigned long mfr_id;
	unsigned long dev_id;
	int buswidth;
	int table_size = sizeof(table) / sizeof(table[0]);
	int i;

	chip->mtd = mtd;
	chip->start = base;

	
	for (buswidth = 1; buswidth <= 4; buswidth <<= 1) {
	
		chip->buswidth = buswidth;

		/* Enter autoselect mode. */
		send_cmd(chip, CMD_RESET_DATA);
		send_cmd(chip, CMD_MANUFACTURER_UNLOCK_DATA);

		mfr_id = wide_read(chip, chip->buswidth * ADDR_MANUFACTURER);
		dev_id = wide_read(chip, chip->buswidth * ADDR_DEVICE_ID);

		//printf("Probe flash with mfr_id=%X dev_id=%X buswidth=%x\n", 
		//			mfr_id, dev_id, buswidth);

		/* Exit autoselect mode. */
		send_cmd(chip, CMD_RESET_DATA);
		
		for (i = 0; i < table_size; i++) {
			if ((mfr_id == table[i].mfr_id) &&
			    (dev_id == table[i].dev_id)) {
				chip->size = table[i].size;
				chip->numeraseregions = table[i].numeraseregions;
				memcpy(chip->regions, table[i].regions, sizeof(chip->regions));
				printf("Found %dMiB %s at 0x%x\r\n", 
					(table[i].size)/(1024*1024), table[i].name, base);
				return 0;
			}
		}
	}

	return -1;
}

static int chip_is_busy(struct flchip *chip, unsigned long addr)
{
	return ((wide_read(chip, addr) & D6_MASK) != (wide_read(chip, addr) & D6_MASK));
}

static int erase_one_block(struct flchip *chip, unsigned long addr, unsigned long size)
{
	int times_left;
	int retry = 5;
	uint32 cnt = 0;
	unsigned long value;

	printf("erase addr=%x size=%x\n", addr, size);
	do {
		send_cmd(chip, CMD_SECTOR_ERASE_UNLOCK_DATA);
		send_cmd_to_addr(chip, CMD_SECTOR_ERASE_UNLOCK_DATA_2, addr);

		delay1ms(800);

		if (retry > 0) {
			for (cnt = 0; cnt < size; cnt += chip->buswidth) {
				value = wide_read(chip, addr + cnt);
				if (value != 0xffff) {
					retry--;
					break;
				}
			}
			if (cnt >= size)
				break;
		}
	} while (retry);

	return 0;
}

static int flash_erase_internal(struct mtd_info *mtd, unsigned long addr, unsigned long size)
{
	struct flchip *chip;
	struct erase_region *region;
	int chip_start, chip_end;
	int region_start, region_end;
	int i, j, n;
	int x;

	for (i = 0; i < mtd->numchips; i++) {
		chip = &mtd->chips[i];
		chip_start = chip->start;
		chip_end = chip->start + chip->size;
		if ((addr >= chip_end) || (addr + size <= chip_start))
			continue;
		if (chip_start < addr)
			chip_start = addr;
		if (chip_end > addr + size)
			chip_end = addr + size;
		for (j = 0; j < chip->numeraseregions; j++) {
			region = &chip->regions[j];
			region_start = chip->start + region->offset;
			region_end = region_start + (region->erasesize * region->numblocks);
			if ((chip_start >= region_end) || (chip_end <= region_start))
				continue;
			if (region_start < chip_start)
				region_start = chip_start;
			if (region_end > chip_end)
				region_end = chip_end;
			for (n = region_start; n < region_end; n += region->erasesize) {
				x = erase_one_block(chip, n - chip->start, region->erasesize);
			}
		}
	}

	return 0;
}

static int flash_read_internal(struct mtd_info *mtd, unsigned long from,
	unsigned long len, unsigned long *retlen, unsigned char *buf)
{
	if ((from + len) > mtd->size)
		return -1;

	//memcpy(buf, (char *)((mtd->offset + from) | NOCACHE_BIT), len);
	memcpy(buf, (char *)((mtd->offset + from)), len);
	*retlen = len;

	return 0;
}

static int write_one_word(struct mtd_info *mtd, struct flchip *chip,
			  unsigned long addr, unsigned long datum)
{
	int times_left;

	send_cmd(chip, CMD_PROGRAM_UNLOCK_DATA);
	wide_write(chip, datum, addr);

	times_left = 500000;
	while (times_left-- && chip_is_busy(chip, addr));

	for (times_left = 0; times_left < 2000; times_left++) {
		if (datum == wide_read(chip, addr))
			break;
	}
	
	if (!times_left)
		return -1;

	return 0;
}

static int flash_write_internal(struct mtd_info *mtd, unsigned long to,
	unsigned long len, unsigned long *retlen, const unsigned char *buf)
{
	struct flchip *chip;
	int chip_start, chip_end;
	int i, n;

	*retlen = 0;
	for (i = 0; i < mtd->numchips; i++) {
		chip = &mtd->chips[i];
		chip_start = chip->start;
		chip_end = chip->start + chip->size;
		if ((to >= chip_end) || (to + len <= chip_start))
			continue;
		if (chip_start < to)
			chip_start = to;
		if (chip_end > to + len)
			chip_end = to + len;

		printf("program from %x to %x\n", chip_start, chip_end);
		if (chip->buswidth == 1)
			for (n = chip_start; n < chip_end; n++) {
				write_one_word(mtd, chip, n - chip->start, buf[n - to]);
				if (((n - chip_start) & 0x3ff) == 0)
					printf(".");
			}
		else if (chip->buswidth == 2)
			for (n = chip_start; n < chip_end; n += 2) {
				write_one_word(mtd, chip, n - chip->start, 
						*((unsigned short *)(&buf[n - to])));
				if (((n - chip_start) & 0x3ff) == 0)
					printf(".");
			}
		else if (chip->buswidth == 4)
			for (n = chip_start; n < chip_end; n += 4) {
				write_one_word(mtd, chip, n - chip->start,
						*((unsigned long *)(&buf[n - to])));
				if (((n - chip_start) & 0x3ff) == 0)
					printf(".");
			}
		else {
			printf("flash_write_internal(): unsupported buswidth\r\n");
			return -1;
		}
		printf("\n");

		*retlen += chip_end - chip_start;
	}

	return 0;
}

int amdflash_init(unsigned long rom_base)
{
	unsigned long flash_base;
	struct flchip *chip;
	int i;

	/* turn off flash write protect */
	tc_outl(CR_GPIO_DATA, tc_inl(CR_GPIO_DATA) | (1 << 5));

	mtd.offset = rom_base;
	mtd.size = 0;
	mtd.numchips = 0;

	flash_base = 0;
	for (i = 0; i < MAX_CHIPS; i++) {
		chip = &mtd.chips[i];
		if (probe_chip(&mtd, flash_base, chip) == 0) {
			mtd.size += chip->size;
			mtd.numchips++;
			flash_base+= chip->size;
			/* lino: support one chip now */
			break;
		} else
			break;
	}

	return 0;
}

int amdflash_erase(unsigned long addr, unsigned long size)
{
	return flash_erase_internal(&mtd, addr, size);
}

int amdflash_read(unsigned long from,
	unsigned long len, unsigned long *retlen, unsigned char *buf)
{
	return flash_read_internal(&mtd, from, len, retlen, buf);
}

int amdflash_write(unsigned long to,
	unsigned long len, unsigned long *retlen, const unsigned char *buf)
{
	return flash_write_internal(&mtd, to, len, retlen, buf);
}

