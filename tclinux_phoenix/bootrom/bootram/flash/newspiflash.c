#include <asm/types.h>
#include <asm/io.h>
#include <asm/tc3162.h>
#include "newspiflash.h"
#ifdef SPI_DRAM_TEST_CMD
#include "dramtest.h"
#endif
#include "linux/spinlock.h"

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
#define MX25L6405D				0x2017
#define MX25L12805D				0x2018
#define MX25L25635E  			0x2019

/* SPANSION */
#define S25FL016A          		0x0214
#define S25FL032A          		0x0215
#define S25FL064A          		0x0216
#define S25FL128P          		0x2018
#define S25FL256S				0x0219
#define S25FL016K          		0x4015

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

int spiflash_init (unsigned long rom_base);
void spiflash_exit (void);
#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
uint16 Addr3B4B_Flag=AUTO_3BMODE;
extern uint16 Address3B4B_Value;
extern uint16 ReadIdle_Value;
extern uint32 rd_time;
extern uint32 pp_time;
extern uint32 se_time;
extern uint32 be_time;
int spiflash_probe (void);
#else
static int spiflash_probe (void);
#endif
static int spiflash_erase_internal (struct mtd_info *mtd,unsigned long addr, unsigned long len);
static int spiflash_read_internal (struct mtd_info *mtd, unsigned long from,unsigned long len,unsigned long *retlen,unsigned char *buf);
static int spiflash_write_internal (struct mtd_info *mtd,unsigned long to,unsigned long len,unsigned long *retlen,const unsigned char *buf);

/* Mapping of generic opcodes to STM serial flash opcodes */

struct mtd_info mtd;

unsigned long spidbg = 0;
static __u8 byte_program_mode = 0;
static __u8 is_W25Q256 = 0;
static __u8 is_MX25L256 = 0;
static __u8 is_S25FL256S = 0;
static __u8 is_N25Q256 = 0;
spinlock_t 	global_sf_lock;
//#ifdef CONFIG_DUAL_IMAGE

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
	local_irq_disable();
	
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
	local_irq_enable();
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

#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
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

#if 0
void enter_dualIO_enable_mode(void)
{
	u8 reg_value = 0;
	u8 reg[2];
	if(is_N25Q256) {
		SEND_READ_REGISTER_CMD(SF_OP_RD_ENHANCED_CFGREG, reg_value);
		printf("\nenter_dualIO_enable_mode-1::ENHANCED_CFGREG=0x%04x\n", reg_value);
		//SEND_WORD_READ_REGISTER_CMD(SF_OP_RD_NONVOLATILE_CFGREG, reg);
		printf("\nenter_dualIO_enable_mode-1::Non-VOLATILE_CFGREG=0x%04x\n", ((reg[0]<<8) | reg[1]));
		SEND_READ_REGISTER_CMD(SF_OP_RD_VOLATILE_CFGREG, reg_value);
		printf("\nenter_dualIO_enable_mode-1::VOLATILE_CFGREG=0x%04x\n", reg_value);
		reg_value &= 0x0F;
		reg_value |= 0x10;
		printf("\nenter_dualIO_enable_mode-2::VOLATILE_CFGREG=0x%04x\n", reg_value);
		SEND_WRITE_REGISTER_CMD(SF_OP_RD_VOLATILE_CFGREG, reg_value);
		printf("\nenter_dualIO_enable_mode-3::VOLATILE_CFGREG=0x%04x\n", reg_value);
	}
}
#endif

static u32
spiflash_read_id (void)
{
	u32 flash_id=0;
	u8 id[3]={0};
	u8 cur_mode=0;
	
	SEND_RD_ID_CMD(id);
	
	flash_id = id[0] << 16 | id[1] << 8 | id[2];

	return flash_id;
}

static int
spiflash_erase_oneblock (struct mtd_info *mtd, unsigned long addr, unsigned long len)
{
	u8 cur_mode=0;
	u8 mode=0;
#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
	u32 begtime=0, endtime=0, passtime=0;
	u32 begtick=0, endtick=0;
	unsigned long dest_addr = addr;
	uint16 cur3B4B_Flag = Addr3B4B_Flag;
#endif	

   	/* sanity checks */
   	if (addr + len > mtd->size) return (-1);

#ifdef SPIFLASH_DEBUG
   	printf ("spiflash_eraseone_block (addr = 0x%08x, len = %d)\n", addr, len);
#endif

	if(mtd->erasesize == SIZE_64KiB) {
#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
		begtime = getTime();
		begtick = VPint(CR_TIMER1_VLR);
		
		if(mtd->size >= SIZE_32MiB) {
			if(Address3B4B_Value == 0) {
				mode = 1;
			} else if((Address3B4B_Value != 0) && ((dest_addr>>24) != 0)) {
				if(Addr3B4B_Flag == AUTO_3BMODE) {
					enter_4Byte_mode();
				}
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
		if(mtd->size >= SIZE_32MiB)
		{
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
		se_time = passtime;
		
		if(mtd->size >= SIZE_32MiB) {
			if((cur3B4B_Flag == AUTO_4BMODE) && (Addr3B4B_Flag == AUTO_3BMODE)) {
				enter_4Byte_mode();
			} else if((cur3B4B_Flag == AUTO_3BMODE) && (Addr3B4B_Flag == AUTO_4BMODE)) {
				exit_4Byte_mode();
			}
		}
#endif
		printf("\nerase addr=0x%08x", addr);
#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
		printf(" passtime is %lu", se_time);
#endif
	} else {
		return (-1);
	}
	
   	return (0);
}

static int
spiflash_erase_internal (struct mtd_info *mtd, unsigned long addr, unsigned long len)
{
	int ret=0;

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
spiflash_manual_read_internal (struct mtd_info *mtd, unsigned long from,unsigned long len,unsigned long *retlen,unsigned char *buf, unsigned char read_type)
{
	int mode=0;
	u8 cur_mode=0;
	unsigned long dest_addr = from;
#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
	u32 rd_begtime=0, rd_endtime=0;
	u32 rd_begtick=0, rd_endtick=0;
	uint16 cur3B4B_Flag = Addr3B4B_Flag;
#endif
#ifdef SPIFLASH_DEBUG
   	printf ("spiflash_read (from = 0x%08x, len = %d)\n", from, len);
#endif

   	/* sanity checks */
   	if (!len) return (0);
   	if (from + len > mtd->size) return (-1);

#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
	if((Address3B4B_Value == 2) && (mtd->size >= SIZE_32MiB) && (is_MX25L256 == 0) && ((dest_addr>>24) != 0)) {
		if(Addr3B4B_Flag == AUTO_3BMODE) {
			enter_4Byte_mode();
		}
		rd_begtime = getTime();
		rd_begtick = VPint(CR_TIMER1_VLR);
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
			printf("spiflash_read: not support this mode\n");
			break;		
		}
		rd_endtick = VPint(CR_TIMER1_VLR);
		rd_endtime = getTime();
		if(rd_endtime < rd_begtime)
			rd_time = 0xffffffff - rd_begtime + rd_endtime;
		else
			rd_time = rd_endtime - rd_begtime;
		rd_time = (rd_time/10) * VPint(CR_TIMER1_LDV) + rd_begtick - rd_endtick;

		if(cur3B4B_Flag == AUTO_3BMODE) {
			exit_4Byte_mode();
		}
	} else {
		if(mtd->size >= SIZE_32MiB) {
			if(Address3B4B_Value == 0) {
				mode = 1;
			} else if((Address3B4B_Value != 0) && ((dest_addr>>24) != 0)) {
				if(Addr3B4B_Flag == AUTO_3BMODE) {
					enter_4Byte_mode();
				}
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
		} else {
			mode = 0;
		}
#endif
		
#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
		rd_begtime = getTime();
		rd_begtick = VPint(CR_TIMER1_VLR);
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
			printf("spiflash_read: not support this mode\n");
			break;		
		}
#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
		rd_endtick = VPint(CR_TIMER1_VLR);
		rd_endtime = getTime();
		if(rd_endtime < rd_begtime)
			rd_time = 0xffffffff - rd_begtime + rd_endtime;
		else
			rd_time = rd_endtime - rd_begtime;
		rd_time = (rd_time/10) * VPint(CR_TIMER1_LDV) + rd_begtick - rd_endtick;
		if(mtd->size >= SIZE_32MiB) {
			if((cur3B4B_Flag == AUTO_4BMODE) && (Addr3B4B_Flag == AUTO_3BMODE)) {
				enter_4Byte_mode();
			} else if((cur3B4B_Flag == AUTO_3BMODE) && (Addr3B4B_Flag == AUTO_4BMODE)) {
				exit_4Byte_mode();
			}
		}
	}
#endif

#ifndef TCSUPPORT_NEW_SPIFLASH_DEBUG
   	/* we always read len bytes */
   	*retlen = len;
#endif

   	return (0);
}

static int
spiflash_manual_read_data(struct mtd_info *mtd, unsigned long from,unsigned long len,unsigned long *retlen,unsigned char *buf)
{
	spiflash_manual_read_internal(mtd, from, len, retlen, buf, READ_DATA);
   	return (0);
}

static int
spiflash_manual_fast_read(struct mtd_info *mtd, unsigned long from,unsigned long len,unsigned long *retlen,unsigned char *buf)
{
	spiflash_manual_read_internal(mtd, from, len, retlen, buf, FAST_READ);
   	return (0);
}

static int
spiflash_manual_fast_read_dualOut(struct mtd_info *mtd, unsigned long from,unsigned long len,unsigned long *retlen,unsigned char *buf)
{
	spiflash_manual_read_internal(mtd, from, len, retlen, buf, FAST_READ_DUALOUT);
   	return (0);
}

static int
spiflash_manual_fast_read_dualIO(struct mtd_info *mtd, unsigned long from,unsigned long len,unsigned long *retlen,unsigned char *buf)
{
	spiflash_manual_read_internal(mtd, from, len, retlen, buf, FAST_READ_DUALIO);
   	return (0);
}

static int
spiflash_read_internal (struct mtd_info *mtd, unsigned long from,unsigned long len,unsigned long *retlen,unsigned char *buf)
{
	u8 cur_mode=0;
#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
	u32 rd_begtime=0, rd_endtime=0;
	u32 rd_begtick=0, rd_endtick=0;
	uint16 cur3B4B_Flag = Addr3B4B_Flag;
#endif

#ifdef SPIFLASH_DEBUG
   	printf ("spiflash_read (from = 0x%08x, len = %d)\n", from, len);
#endif
	
   	/* sanity checks */
   	if (!len) return (0);
   	if (from + len > mtd->size)	return (-1);
	
#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
	if(mtd->size >= SIZE_32MiB) {
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
	
#ifndef TCSUPPORT_NEW_SPIFLASH_DEBUG
   	/* we always read len bytes */
   	*retlen = len;
#endif

#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
	rd_begtime = getTime();
	rd_begtick = VPint(CR_TIMER1_VLR);
	if(ReadIdle_Value) {
		SEND_READ_IDLE_EN_CMD;
	}
#else
	SEND_READ_IDLE_EN_CMD;
#endif

	memcpy4(buf, (unsigned char*)((mtd->offset + from)), len);

#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
	if(ReadIdle_Value) {
		SEND_READ_IDLE_DIS_CMD;
	}
	rd_endtick = VPint(CR_TIMER1_VLR);
	rd_endtime = getTime();
	if(rd_endtime < rd_begtime)
		rd_time = 0xffffffff - rd_begtime + rd_endtime;
	else
		rd_time = rd_endtime - rd_begtime;
	rd_time = (rd_time/10) * VPint(CR_TIMER1_LDV) + rd_begtick - rd_endtick;
	
	if(mtd->size >= SIZE_32MiB) {
		if((cur3B4B_Flag == AUTO_4BMODE) && (Addr3B4B_Flag == AUTO_3BMODE)) {
			enter_4Byte_mode();
			SEND_AUTO_4B_CMD;
		} else if((cur3B4B_Flag == AUTO_3BMODE) && (Addr3B4B_Flag == AUTO_4BMODE)) {
			exit_4Byte_mode();
			SEND_AUTO_3B_CMD;
		}
	}
#endif

   	return (0);
}

static int
spiflash_write_internal (struct mtd_info *mtd,unsigned long to,unsigned long len,unsigned long *retlen,const unsigned char *buf)
{
	u8 cur_mode=0;
	u8 mode=0;
	static int cnt=0;
	u8 reg_sr3 = 0;
	unsigned long pp_len = 0;
	unsigned long pp_cur_len = 0;
	unsigned long tmp_len = 0;
#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
	unsigned long last_addr = 0;
	u32 begtime=0, endtime=0, passtime=0;
	u32 begtick=0, endtick=0;
	uint16 cur3B4B_Flag = Addr3B4B_Flag;
#endif
	u32 flags;

#ifdef SPIFLASH_DEBUG
   	printf ("spiflash_write (to = 0x%08x, len = %d)\n", to, len);
#endif

   	*retlen = 0;

   	/* sanity checks */
   	if (!len) return (0);
   	if (to + len > mtd->size) return (-1);

	//printf("program from %x to %x\n", to, to+len);

	pp_len = len;

#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
	//last_addr = to + len;
	last_addr = to + SF_PP_MAX*8;
#endif

	if(mtd->size >= SIZE_32MiB) {
		mode = 1;
	}
	else {
		mode = 0;
	}

	while(pp_len > 0) {
		cnt++;
		if ((cnt & 0x3f) == 0)
			printf(".");
		
		if((to & (SF_PP_MAX-1)) != 0) {
			pp_cur_len = MIN((SF_PP_MAX - (to % SF_PP_MAX)), pp_len);
		}
		else {
			pp_cur_len = MIN(SF_PP_MAX, pp_len);
		}
		pp_len -= pp_cur_len;
		
#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
		begtime = getTime();
		begtick = VPint(CR_TIMER1_VLR);
		if(mtd->size >= SIZE_32MiB) {
			if(Address3B4B_Value == 0) {
				mode = 1;
			} else if((Address3B4B_Value != 0) && ((last_addr>>24) != 0)) {
				if(Addr3B4B_Flag == AUTO_3BMODE) {
					enter_4Byte_mode();
				}
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
		if(endtime < begtime)
			passtime = 0xffffffff - begtime + endtime;
		else
			passtime = endtime - begtime;
		passtime = (passtime/10) * VPint(CR_TIMER1_LDV)+ begtick - endtick;
		pp_time = passtime;
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
		dev_id: W25X256,
		name: "Winbond W25X256",
		DeviceSize: SIZE_32MiB,
		EraseSize: SIZE_64KiB,
		mode: SPI_FAST_RD_DUAL_O,
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
		dev_id: W25Q256,
		name: "Winbond W25Q256",
		DeviceSize: SIZE_32MiB,
		EraseSize: SIZE_64KiB,
		mode: SPI_FAST_RD_QUAD_IO,
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
		dev_id: MX25L16,
		name: "MX25L16",
		DeviceSize: SIZE_2MiB,
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
		name: "MX25L25635E",
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
		mfr_id: MANUFACTURER_SPANSION,
		dev_id: S25FL256S,
		name: "Spasion S25FL256S",
		DeviceSize: SIZE_32MiB,
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
		mfr_id: MANUFACTURER_NUMONYX,
		dev_id: N25Q128,
		name: "N25Q128",
		DeviceSize: SIZE_16MiB,
		EraseSize: SIZE_64KiB,
		mode: SPI_STD_RD,
	},
	{
		mfr_id: MANUFACTURER_NUMONYX,
		dev_id: N25Q256,
		name: "N25Q256",
		DeviceSize: SIZE_32MiB,
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
		dev_id: GD25Q16,
		name: "GD25Q16",
		DeviceSize: SIZE_2MiB,
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
		dev_id: GD25Q64,
		name: "GD25Q64",
		DeviceSize: SIZE_8MiB,
		EraseSize: SIZE_64KiB,
		mode: SPI_FAST_RD_DUAL_IO,
	},
	{
		mfr_id: MANUFACTURER_GIGADEVICE,
		dev_id: GD25Q128B,
		name: "GD25Q128B",
		DeviceSize: SIZE_16MiB,
		EraseSize: SIZE_64KiB,
		mode: SPI_FAST_RD_DUAL_IO,
	},
	{
		mfr_id: MANUFACTURER_ESMT,
		dev_id: F25L16PA,
		name: "F25L16PA",
		DeviceSize: SIZE_2MiB,
		EraseSize: SIZE_64KiB,
		mode: SPI_FAST_RD_DUAL_IO,
	},
	{
		mfr_id: MANUFACTURER_ESMT,
		dev_id: F25L32QA,
		name: "F25L32QA",
		DeviceSize: SIZE_4MiB,
		EraseSize: SIZE_64KiB,
		mode: SPI_FAST_RD_DUAL_IO,
	},
	{
		mfr_id: MANUFACTURER_ESMT,
		dev_id: F25L64QA,
		name: "F25L64QA",
		DeviceSize: SIZE_8MiB,
		EraseSize: SIZE_64KiB,
		mode: SPI_FAST_RD_DUAL_IO,
	},
	{
		mfr_id: MANUFACTURER_SPANSION,
		dev_id: S25FL208K,
		name: "S25FL208K",
		DeviceSize: SIZE_2MiB,
		EraseSize: SIZE_64KiB,
		mode: SPI_STD_FAST_RD,
	},
	{
		mfr_id: MANUFACTURER_SPANSION,
		dev_id: S25FL116K,
		name: "S25FL116K",
		DeviceSize: SIZE_2MiB,
		EraseSize: SIZE_64KiB,
		mode: SPI_STD_FAST_RD,
	},
	{
		mfr_id: MANUFACTURER_SPANSION,
		dev_id: S25FL216K,
		name: "S25FL216K",
		DeviceSize: SIZE_2MiB,
		EraseSize: SIZE_64KiB,
		mode: SPI_STD_FAST_RD,
	},
	{
		mfr_id: MANUFACTURER_SPANSION,
		dev_id: S25FL127S,
		name: "S25FL127S",
		DeviceSize: SIZE_16MiB,
		EraseSize: SIZE_64KiB,
		mode: SPI_STD_FAST_RD,
	},
};


#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
int spiflash_probe(void)
#else
static int spiflash_probe(void)
#endif
{
	int i=0;
	unsigned long flash_id=0;

	flash_id = spiflash_read_id();

	for (i=0; i < (sizeof(flash_tables)/sizeof(struct spi_flash_info)); i++) {
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
			return i;
		}
	}
	return -1;
}

#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
static int
spiflash_bulk_erase_internal(struct mtd_info *mtd)
{
	u8 cur_mode=0;
#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
	u32 begtime=0, endtime=0, passtime=0;
#endif
		
#ifdef SPIFLASH_DEBUG
	printf ("spiflash_erase whole_chip\n");
#endif
	
#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
	begtime = getTime();
#endif

	SEND_BULK_ERASE_CMD;
	
#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
	endtime = getTime();

	if(endtime < begtime)
		passtime = 0xffffffff - begtime + endtime;
	else
		passtime = endtime - begtime;
	be_time = passtime;
#endif

	return (0);
}

int spiflash_bulk_erase(void)
{
	return spiflash_bulk_erase_internal(&mtd);
}
#endif

int spiflash_erase(unsigned long addr, unsigned long size)
{
	return spiflash_erase_internal(&mtd, addr, size);
}

int spiflash_read(unsigned long from,
	unsigned long len, unsigned long *retlen, unsigned char *buf)
{
	return spiflash_manual_fast_read(&mtd, from, len, retlen, buf);
}

int spiflash_write(unsigned long to,
	unsigned long len, unsigned long *retlen, const unsigned char *buf)
{
	return spiflash_write_internal(&mtd, to, len, retlen, buf);
}

unsigned char ReadSPICache(unsigned long index)
{
	unsigned long retlen = 0;
	unsigned long len = 1;
	unsigned char buf[2]={0};
	index &= 0x03FFFFFF;
	spiflash_read_internal(&mtd, index, len, &retlen, buf);
	return buf[0];
}

unsigned char ReadSPIByte(unsigned long index)
{
	unsigned long retlen = 0;
	unsigned long len = 1;
	unsigned char buf[2]={0};
	index &= 0x03FFFFFF;
	spiflash_read_internal(&mtd, index, len, &retlen, buf);
	return buf[0];
}

unsigned long ReadSPIDWord(unsigned long index)
{
	unsigned long retlen = 0;
	unsigned long len = 4;
	unsigned char buf[5]={0};
	unsigned long dword=0;
	index &= 0x03FFFFFF;
	spiflash_read_internal(&mtd, index, len, &retlen, buf);
	dword = (buf[0] << 24) | (buf[1] << 16) | (buf[2] <<8) | buf[3];
	return dword;
}

static void Quad_Mode_Enable(int index)
{
	u8 val=0;
	u8 check=0;

	if( (flash_tables[index].mfr_id == MANUFACTURER_MXIC) )
	{
		printf("\nMXIC Quad Mode Enable\n");
		SEND_READ_REGISTER_CMD(SF_OP_RD_STATUS1, val );
		val |= 0x40;
		SEND_WRITE_REGISTER_CMD(SF_OP_WR_STATUS1, val);

		SEND_READ_REGISTER_CMD(SF_OP_RD_STATUS1, check );
		printf("Status Register 1 = 0x%x\n", check);

		
	}

	if( (flash_tables[index].mfr_id == MANUFACTURER_WINBOND) )
	{

		if( (flash_tables[index].dev_id == W25Q64) )
		{
			printf("\nWinbond Quad Mode Enable\n");
			SEND_READ_REGISTER_CMD(SF_OP_RD_STATUS2, val );
			val |= 0x2;
			
			SEND_WRITE_REGISTER_TWO_BYTE_CMD(SF_OP_WR_STATUS1, 0x0, val);

			SEND_READ_REGISTER_CMD(SF_OP_RD_STATUS2, check );
			printf("Status Register 2 = 0x%x\n", check);
		}
		else
		{
			printf("\nWinbond Quad Mode Enable\n");
			SEND_READ_REGISTER_CMD(SF_OP_RD_STATUS2, val );
			val |= 0x2;
			SEND_WRITE_REGISTER_CMD(SF_OP_WR_STATUS2, val);

			SEND_READ_REGISTER_CMD(SF_OP_RD_STATUS2, check );
			printf("Status Register 2 = 0x%x\n", check);		
		}
	}


	if( (flash_tables[index].mfr_id == MANUFACTURER_GIGADEVICE) )
	{
		printf("\nGigaDevice Quad Mode Enable\n");
		SEND_READ_REGISTER_CMD(SF_OP_RD_STATUS2, val );
		val |= 0x2;
		SEND_WRITE_REGISTER_TWO_BYTE_CMD(SF_OP_WR_STATUS1, 0x0, val);

		SEND_READ_REGISTER_CMD(SF_OP_RD_STATUS2, check );
		printf("Status Register 2 = 0x%x\n", check);
	}	

	
}



#ifdef TCSUPPORT_DUAL_IMAGE_ENHANCE
int offset = 0;
#endif
int
spiflash_init (unsigned long rom_base)
{
   	int index=0;
	int spi_read_bytes=0;
	unsigned int val;

	/* turn off flash write protect */
	//tc_outl(CR_GPIO_DATA, tc_inl(CR_GPIO_DATA) | (1 << 5));

	mtd.offset = rom_base;
	mtd.size = 0;
	
#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
	sf_regDef_test();
	for(index=0; index<16; index++)
		sf_regRW_test(index);
	sf_regRW_test(0xFFFFFFFF);
#endif
	/* enable SF_MAXMUX_SEL */
	WriteReg(SF_MACMUX_SEL, MANUAL_MACMUXSEL);
	
   	index = spiflash_probe();
	if (index < 0) {
    	printf ("%s: Found no SPI flash device\n", module_name);
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

	/* Quad_Mode_Enable(index); */

	// set SFC Clock to 25MHZ
#if defined(TCSUPPORT_CPU_EN7512)||defined(TCSUPPORT_CPU_EN7521)
	val = VPint(SF_CLK_CHANGE);
	val |= 0xa01;
	VPint(SF_CLK_CHANGE) = val;	
#else
	WriteReg(SF_CLK_CHANGE, 6);
#endif

	// set Tn to 1
	WriteReg(SF_SI_CK_SEL, 9);
	
//#ifdef CONFIG_DUAL_IMAGE
#if 0
	flash_index = index;
#endif
#ifdef TC3262
	/* TC3182LDV doesn't support quad mode */
	if (isTC3182 || isRT65168 || isRT63165 || isRT63365 || isMT751020 || isMT7505 || isEN751221)
#else
	/* Note: TC3162U doesn't support quad mode */ 
	if (isTC3162U || isRT63260 || isTC3162L5P5)           //TC3162L2M does not support Quad Mode neither, added by fredli, bug#10664          
#endif
	{
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

	printf("Found SPI Flash %dMiB %s at 0x%x\r\n", 
		(flash_tables[index].DeviceSize)/(1024*1024), flash_tables[index].name, rom_base);

	spin_lock_init(global_sf_lock);

   	mtd.size = flash_tables[index].DeviceSize;
   	mtd.erasesize = flash_tables[index].EraseSize;
#ifdef TCSUPPORT_DUAL_IMAGE_ENHANCE
	offset = flash_tables[index].DeviceSize/2;
#endif
   	return 0;
}

#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
#define AUTOREAD_TEST_MAX	(4)
#define MANUALREAD_TEST_MAX	(4)
#define MUXREAD_TEST_MAX	(8)

extern uint8 clk_rate;
extern uint16 CLKRate_Value;
extern uint16 AutoRead_cnt;
extern uint16 ManualRead_cnt;
extern uint16 MuxRead_cnt;
extern uint16 AutoMappingTab[4];
extern uint16 ManualMappingTab[4];
extern uint16 MuxMappingTab[8];
extern uint16 Mats_ReadMode_Value;
extern uint16 Tn_cnt;
extern uint8 TnMappingTab[6];
extern unsigned long sf_rand();

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

int spiflash_mats_read(unsigned long from,
	unsigned long len, unsigned long *retlen, unsigned char *buf)
{
	switch(Mats_ReadMode_Value) {
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
			return spiflash_manual_read_data(&mtd, from, len, retlen, buf);
		case 5:
			return spiflash_manual_fast_read(&mtd, from, len, retlen, buf);
		case 6:
			return spiflash_manual_fast_read_dualOut(&mtd, from, len, retlen, buf);
		case 7:
			return spiflash_manual_fast_read_dualIO(&mtd, from, len, retlen, buf);
	}
	return spiflash_read_internal(&mtd, from, len, retlen, buf);
}

int spiflash_autoread_test(unsigned long from,
	unsigned long len, unsigned long *retlen, unsigned char *buf, u32 mode)
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
		case 4:
			SEND_AUTO_FASTREAD_QUADOUT_CMD;
			break;
		case 5:
			SEND_AUTO_FASTREAD_QUADIO_CMD;			
			break;
	}
	return spiflash_read_internal(&mtd, from, len, retlen, buf);
}

int spiflash_manualread_test(unsigned long from,
	unsigned long len, unsigned long *retlen, unsigned char *buf, u32 mode)
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
		return spiflash_manual_read_data(&mtd, from, len, retlen, buf);
	case 1:
		return spiflash_manual_fast_read(&mtd, from, len, retlen, buf);
	case 2:
		return spiflash_manual_fast_read_dualOut(&mtd, from, len, retlen, buf);
	case 3:
		return spiflash_manual_fast_read_dualIO(&mtd, from, len, retlen, buf);
	}
}

int spiflash_muxread_test(unsigned long from,
	unsigned long len, unsigned long *retlen, unsigned char *buf, u32 mode)
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
			return spiflash_manual_read_data(&mtd, from, len, retlen, buf);
		case 5:
			return spiflash_manual_fast_read(&mtd, from, len, retlen, buf);
		case 6:
			return spiflash_manual_fast_read_dualOut(&mtd, from, len, retlen, buf);
		case 7:
			return spiflash_manual_fast_read_dualIO(&mtd, from, len, retlen, buf);
	}
	return spiflash_read_internal(&mtd, from, len, retlen, buf);
}

int sf_bootin4Byte_Entry(int argc, char *argv[])
{
	u8 bootAddrMode=0;
	u8 hwTrapEn=0;
	u8 sw3B4B=0;
	u8 reg=0;
	
	if(!is_W25Q256) {
		printf("\nthis flash is not W25Q256, can't boot in 4Byte Mode!\n");
		return -1;
	}
		
	if(argc == 1){
		printf("sfoot4 <bootAddrMode:0~1> <hwTrapEn:0~1> <sw3B4B:0~1>\n");
		SEND_READ_REGISTER_CMD(SF_OP_RD_STATUS3, reg);
		printf("Status Register3 is %x, boot up Address is %s\n", reg, (((reg&0x2)==0x2)?("4Byte Mode"):("3Byte Mode")));
		printf("0xbfa1008C is %x, hwTrap is %s\n", VPint(SF_CFG3B4B_EN), ((VPint(SF_CFG3B4B_EN)==0)?("Enable"):("Disable")));
		printf("0xbfa10084 is %x, sw Address Mode %s\n\n", VPint(SF_ADDR_3B4B), ((VPint(SF_ADDR_3B4B)==1)?("4Byte Mode"):("3Byte Mode")));
		return -2;
	}
	
	if(argc >= 2){
		bootAddrMode = simple_strtoul(argv[1], NULL, 10);
		if(bootAddrMode <= 1)
			printf("bootAddrMode is %s\n", ((bootAddrMode==1)?("4Byte Mode"):("3Byte Mode")));
		else {
			printf("bootAddrMode should be in [0~1]\n");
			return -3;
		}
	}
	if(argc >= 3){
		hwTrapEn = simple_strtoul(argv[2], NULL, 10);
		if(hwTrapEn <= 1)
			printf("hwTrap is %s\n", ((hwTrapEn==0)?("Enable"):("Disable")));
			if(hwTrapEn == 1) {
				WriteReg(SF_CFG3B4B_EN, CFG3B4B_EN);
			} else if (hwTrapEn == 0) {
				WriteReg(SF_CFG3B4B_EN, CFG3B4B_DIS);
			}
		else {
			printf("hwTrapEn should be in [0~1]\n");
			return -4;
		}
	}
	if(argc >= 4){
		sw3B4B = simple_strtoul(argv[3], NULL, 10);
		if(sw3B4B <= 1)
			printf("sw3B4B is %s\n", ((sw3B4B==1)?("4Byte Mode"):("3Byte Mode")));
			if(sw3B4B == 1) {
				WriteReg(SF_ADDR_3B4B, AUTO_4BMODE);
			} else if (sw3B4B == 0) {
				WriteReg(SF_ADDR_3B4B, AUTO_3BMODE);
			}
		else {
			printf("sw3B4B should be in [0~1]\n");
			return -4;
		}
	}
	
	if(bootAddrMode == 1){
		SEND_READ_REGISTER_CMD(SF_OP_RD_STATUS3, reg);
		printf("1>>>Status Register3 is %x\n", reg);
		reg |= 0x2;
		printf("2>>>Status Register3 is %x\n", reg);
		SEND_WRITE_REGISTER_CMD(SF_OP_WR_STATUS3, 0x2);
	} else if(bootAddrMode == 0) {
		SEND_READ_REGISTER_CMD(SF_OP_RD_STATUS3, reg);
		reg &= 0xFD;
		SEND_WRITE_REGISTER_CMD(SF_OP_WR_STATUS3, 0x0);
	}
	SEND_READ_REGISTER_CMD(SF_OP_RD_STATUS3, reg);
	
	printf("Status Register3 is %x, boot up Address is %s\n", reg, (((reg&0x2)==0x2)?("4Byte Mode"):("3Byte Mode")));
	printf("0xbfa1008C is %x, hwTrap is %s\n", VPint(SF_CFG3B4B_EN), ((VPint(SF_CFG3B4B_EN)==0)?("Enable"):("Disable")));
	printf("0xbfa10084 is %x, sw Address Mode %s\n\n", VPint(SF_ADDR_3B4B), ((VPint(SF_ADDR_3B4B)==1)?("4Byte Mode"):("3Byte Mode")));
	

	return 0;	
}
#endif

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

	if(isMT751020 || isMT7505 || isEN751221){
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

	if(isMT751020 || isMT7505 || isEN751221){
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

