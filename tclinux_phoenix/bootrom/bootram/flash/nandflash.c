//#define CONFIG_BADBLOCK_CHECK 

#include <linux/string.h>
#include <linux/stddef.h>
#include <linux/compiler.h>
#include <asm/io.h>
#include <nandflash.h>

#include <asm/tc3162.h>

#ifdef LZMA_IMG
#define printf(args...) 		do{}while(0)
#else
#define printf prom_printf
#endif

#define NULL_DEFINED( ... )     do{}while(0)
#define NULL_DEF_RET_1( ... )   (1)
#define NULL_DEF_RET_0( ... )   (0)

#define HZ 1

#define READ_STATUS_RETRY	1000000
#define MAX_BLOCK_NUM		(1<<15)
#define NFC_BIG_ENDIAN		0x02
#define NFC_LITTLE_ENDIAN	0x0

struct ra_nand_chip ra;
static char	buffers[ SIZE_2KiB_BYTES + SIZE_64iB_BYTES ];
#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
static char readback_buffers[SIZE_2KiB_BYTES + SIZE_64iB_BYTES];
static int block_table[1024];
static unsigned long next_erase_addr;
#endif

static int skipbbt = 0;
static int ranfc_debug = 0;
static int ranfc_bbt = 1;
static int ranfc_verify = 1;
static int ranfc_flags = 0;
static int ranfc_page = 0;
static int column_addr_cycle = 0;
static int row_addr_cycle = 0;
static int addr_cycle = 0;


#if defined(TCSUPPORT_DUAL_IMAGE_ENHANCE) && !defined(LZMA_IMG)
extern int offset;
#endif

#if 0
#define ra_dbg(args...) do { if (ranfc_debug) printf(args); } while(0)
#else
#define ra_dbg(args...)
#endif

#define CLEAR_INT_STATUS() 	ra_outl(NFC_INT_ST, ra_inl(NFC_INT_ST))
#define NFC_TRANS_DONE() 	(ra_inl(NFC_INT_ST) & INT_ST_ND_DONE)

static struct nand_opcode opcode_tables[] = {
	{
		type: STANDARD_SMALL_FLASH,
		read1: 0x00,
		read2: NONE,
		readB: 0x01,
		readoob: 0x50,
		pageprog1: 0x8000,
		pageprog2: 0x10,
		writeoob: 0x8050,
		erase1: 0x60,
		erase2: 0xd0,
		status: 0x70,
		reset: 0xff,
	},
	{
		type: STANDARD_LARGE_FLASH,
		read1: 0x00,
		read2: 0x30,
		readB: NONE,
		readoob: NONE,
		pageprog1: 0x80,
		pageprog2: 0x10,
		writeoob: 0x80,
		erase1: 0x60,
		erase2: 0xd0,
		status: 0x70,
		reset: 0xff,
	},
};

static struct nand_info flash_tables[] = {
	{
		mfr_id: MANUFACTURER_ST3A,
		dev_id: ST128W3A,
		name: "ST NAND128W3A",
		numchips: (1),
		chip_shift: SIZE_16MiB_BIT,
		page_shift: SIZE_512iB_BIT,
		erase_shift: SIZE_16KiB_BIT,
		oob_shift: SIZE_16iB_BIT,
		badblockpos: (4),		//512 pagesize bad blk offset --> 4
		opcode_type: STANDARD_SMALL_FLASH,
	},
	{
		mfr_id: MANUFACTURER_ST3A,
		dev_id: ST512W3A,
		name: "ST NAND512W3A",
		numchips: (1),
		chip_shift: SIZE_64MiB_BIT,
		page_shift: SIZE_512iB_BIT,
		erase_shift: SIZE_16KiB_BIT,
		oob_shift: SIZE_16iB_BIT,
		badblockpos: (4),		//512 pagesize bad blk offset --> 4
		opcode_type: STANDARD_SMALL_FLASH,
	},
	{
		mfr_id: MANUFACTURER_ZENTEL,
		dev_id: A5U1GA31ATS,
		name:   "ZENTEL NAND1GA31ATS",
		numchips: (1),
		chip_shift: SIZE_128MiB_BIT,
		page_shift: SIZE_2KiB_BIT,
		erase_shift: SIZE_128KiB_BIT,
		oob_shift: SIZE_64iB_BIT,
		badblockpos: (51),
		opcode_type: STANDARD_LARGE_FLASH,

	},
	{
		mfr_id: MANUFACTURER_SAMSUNG,
		dev_id: K9F1G08U0D,
		name:   "SAMSUNG K9F1G08U0D",
		numchips: (1),
		chip_shift: SIZE_128MiB_BIT,
		page_shift: SIZE_2KiB_BIT,
		erase_shift: SIZE_128KiB_BIT,
		oob_shift: SIZE_64iB_BIT,
		badblockpos: (51),
		opcode_type: STANDARD_LARGE_FLASH,

	},
	{
		mfr_id: MANUFACTURER_MIRCON,
		dev_id: MT29F2G08AAD,
		name:	"MICRON NAND2G08AAD",
		numchips: (1),
		chip_shift: SIZE_256MiB_BIT,
		page_shift: SIZE_2KiB_BIT,
		erase_shift: SIZE_128KiB_BIT,
		oob_shift: SIZE_64iB_BIT,
		badblockpos: (51),
		opcode_type: STANDARD_LARGE_FLASH,
	},
	{
		mfr_id: MANUFACTURER_MIRCON,
		dev_id: MT29F4G08AAC,
		name:   "MICRON NAND4G08AAC",
		numchips: (1),
		chip_shift: SIZE_512MiB_BIT,
		page_shift: SIZE_2KiB_BIT,
		erase_shift: SIZE_128KiB_BIT,
		oob_shift: SIZE_64iB_BIT,
		badblockpos: (51),
		opcode_type: STANDARD_LARGE_FLASH,
	},
   {/* SPANSION support */
       mfr_id: MANUFACTURER_SPANSION,
       dev_id: S34ML01G1,
       name:   "SPANSION S34ML01G1",
       numchips: (1),
       chip_shift:  SIZE_128MiB_BIT,
       page_shift: SIZE_2KiB_BIT,
       erase_shift: SIZE_128KiB_BIT,
       oob_shift: SIZE_64iB_BIT,
       badblockpos: (0),
       opcode_type: STANDARD_LARGE_FLASH
	}
	
};


static struct nand_ecclayout oob_layout_tables[] = {
	/* 512iB page size flash */
	{
		.eccbytes = SMALL_FLASH_ECC_BYTES,
		.eccpos = {SMALL_FLASH_ECC_OFFSET, SMALL_FLASH_ECC_OFFSET+1, SMALL_FLASH_ECC_OFFSET+2},
		.oobfree = {
			 {.offset = 0, .length = 4},
			 {.offset = 8, .length = 8},
			 {.offset = 0, .length = 0}
		 },
		.oobavail = 12,
		// 4th byte is bad-block flag.
	},

	/* 2K page size flash */
	{
		.eccbytes = LARGE_FLASH_ECC_BYTES,
		#ifdef RT63365		
		.eccpos = { 
					LARGE_FLASH_ECC_OFFSET,   LARGE_FLASH_ECC_OFFSET+1,  LARGE_FLASH_ECC_OFFSET+2,
					LARGE_FLASH_ECC_OFFSET+16, LARGE_FLASH_ECC_OFFSET+17,  LARGE_FLASH_ECC_OFFSET+18,
					LARGE_FLASH_ECC_OFFSET+32, LARGE_FLASH_ECC_OFFSET+33,  LARGE_FLASH_ECC_OFFSET+34,
					LARGE_FLASH_ECC_OFFSET+48, LARGE_FLASH_ECC_OFFSET+49, LARGE_FLASH_ECC_OFFSET+50
		},
		.oobfree = {
			 {.offset = 0, .length = 5},
			 {.offset = 8, .length = 13},
			 {.offset = 24, .length = 13},
			 {.offset = 40, .length = 11},
			 {.offset = 52, .length = 1},
			 {.offset = 56, .length = 8}
		},
		#else	
		.eccpos = { 
					LARGE_FLASH_ECC_OFFSET,   LARGE_FLASH_ECC_OFFSET+1,  LARGE_FLASH_ECC_OFFSET+2,
					LARGE_FLASH_ECC_OFFSET+3, LARGE_FLASH_ECC_OFFSET+4,  LARGE_FLASH_ECC_OFFSET+5,
					LARGE_FLASH_ECC_OFFSET+6, LARGE_FLASH_ECC_OFFSET+7,  LARGE_FLASH_ECC_OFFSET+8,
					LARGE_FLASH_ECC_OFFSET+9, LARGE_FLASH_ECC_OFFSET+10, LARGE_FLASH_ECC_OFFSET+11
		},
		.oobfree = {
			 {.offset = 0, .length = 51},
			 {.offset = 0, .length = 0}
		},
		#endif
		.oobavail = 51,
		// 2009th byte is bad-block flag.
	}
};

#ifdef LZMA_IMG
void * memcpy(void * dest,const void *src,size_t count)
{
	char *tmp = (char *) dest, *s = (char *) src;

	while (count--)
		*tmp++ = *s++;

	return dest;
}

void * memset(void * s,int c,size_t count)
{
	char *xs = (char *) s;

	while (count--)
		*xs++ = c;

	return s;
}
#endif


/*************************************************************
 * nfc functions 
 *************************************************************/

static int nfc_wait_ready(struct ra_nand_chip *ra);

/**
 * reset nand chip
 */
static int 
nfc_chip_reset(struct ra_nand_chip *ra)
{
	int status, cmd1;

	cmd1 = 0xff;
//	cmd1 = ra->opcode->reset;
	// reset nand flash
	ra_outl(NFC_CMD1, cmd1);
//	ra_outl(NFC_CONF, 0x0101 | (addr_cycle << 16));

	if (ra_inl(CR_AHB_HWCONF) & (0x00080000))
		ra_outl(NFC_CONF, 0x0101 | (0x5 << 16));
	else	
		ra_outl(NFC_CONF, 0x0101 | (0x4 << 16));

	status = nfc_wait_ready(ra);  //erase wait 5us

	if (status & NAND_STATUS_FAIL) {	
		printf("%s: fail \n", __func__);
	}

	return (int)(status & NAND_STATUS_FAIL);
}


/** 
 * clear NFC and flash chip.
 */
static int 
nfc_all_reset(struct ra_nand_chip *ra)
{
	long retry;

	ra_dbg("%s: \n", __func__);

	// reset controller
	ra_outl(NFC_CTRL, ra_inl(NFC_CTRL) | 0x02); //clear data buffer
	ra_outl(NFC_CTRL, ra_inl(NFC_CTRL) & ~0x02); //clear data buffer

	CLEAR_INT_STATUS();

	retry = READ_STATUS_RETRY;
	while ((ra_inl(NFC_INT_ST) & 0x02) != 0x02 && retry--);
	if (retry <= 0) {

		printf("nfc_all_reset: clean buffer fail \n");

		return -1;
	}

	retry = READ_STATUS_RETRY;
	while ((ra_inl(NFC_STATUS) & 0x1) != 0x0 && retry--); //fixme, controller is busy ?

	nfc_chip_reset(ra);

	return 0;

}


/** NOTICE: only called by nfc_wait_ready().
 * @return -1, nfc can not get transction done 
 * @return 0, ok.
 */
static int 
_nfc_read_status(struct ra_nand_chip *ra, char *status)
{
	unsigned long cmd1, conf;
	int int_st, nfc_st;
	long retry;

	cmd1 = 0x70;
//	cmd1 = ra->opcode->status;
	conf = 0x000101 | (1 << 20);

	//fixme, should we check nfc status?
	CLEAR_INT_STATUS();

	ra_outl(NFC_CMD1, cmd1); 	
	ra_outl(NFC_CONF, conf); 

	/* FIXME, 
	 * 1. since we have no wired ready signal, directly 
	 * calling this function is not gurantee to read right status under ready state.
	 * 2. the other side, we can not determine how long to become ready, this timeout retry is nonsense.
	 * 3. SUGGESTION: call nfc_read_status() from nfc_wait_ready(),
	 * that is aware about caller (in sementics) and has snooze plused nfc ND_DONE.
	 */
	retry = READ_STATUS_RETRY; 
	do {
		nfc_st = ra_inl(NFC_STATUS);
		int_st = ra_inl(NFC_INT_ST);
	} while (!(int_st & INT_ST_RX_BUF_RDY) && retry--);

	if (!(int_st & INT_ST_RX_BUF_RDY)) {

		printf("nfc_read_status: NFC fail, int_st(%x), nfc:%x, reset nfc and flash. \n", 
		       int_st, nfc_st);
		nfc_all_reset(ra);
		*status = NAND_STATUS_FAIL;
		return -1;
	}
	#ifdef RT63365
	/* frank modify 20110420 for rt63365 big endian*/
	*status = (char)(((ra_inl(NFC_DATA)) & 0xff000000)>>24);
	#else	
	/* frankliao modify 20101001 for rt63165 a*/
	*status = (char)((ra_inl(NFC_DATA)) & 0x0ff);
	#endif

	return 0;
}


/**
 * @return !0, chip protect.
 * @return 0, chip not protected.
 */
static int 
nfc_check_wp(void)
{
	/* Check the WP bit */
#if !defined CONFIG_NOT_SUPPORT_WP
	return !(ra_inl(NFC_CTRL) & 0x01);
#else
	char result = 0;
	int ret;

	ret = _nfc_read_status(&ra, &result);
	//fixme, if ret < 0

	return !(result & NAND_STATUS_WP);
#endif

}


#if !defined CONFIG_NOT_SUPPORT_RB
/*
 * @return !0, chip ready.
 * @return 0, chip busy.
 */
static int 
nfc_device_ready(void)
{
	/* Check the ready  */
	return !!(ra_inl(NFC_STATUS) & 0x04);
}
#endif

/* frankliao added 20101015 */
static int
nfc_read_id(void)
{
	unsigned int ret_data, int_st;
	long retry;

	retry = READ_STATUS_RETRY;

	CLEAR_INT_STATUS();

	ra_outl(NFC_CMD1, NAND_CMD1_READID); 	
	ra_outl(NFC_CONF, NAND_CONF_READID);

	while (retry > 0) {
		int_st = ra_inl(NFC_INT_ST);
		if (int_st & INT_ST_RX_BUF_RDY) {
			ret_data = ra_inl(NFC_DATA);
			break;
		} else {
			retry--;
		}
	}

	if (retry <= 0) {
		printf("%s: read id fail \n", __func__);
		return -1;
	} else {
		return ret_data;
	}	
}


/**
 * generic function to get data from flash.
 * @return data length reading from flash.
 */
static int 
_ra_nand_pull_data(char *buf, int len)
{
	char *p = buf;
	long retry;

	// receive data by use_gdma 
/*	if (use_gdma) { 
		if (_ra_nand_dma_pull((unsigned long)p, len)) {
			printf("%s: fail \n", __func__);
			len = -1; //return error
		}

		return len;
	} */

	//fixme: retry count size?
	retry = READ_STATUS_RETRY;
	// no gdma
	
//	burst_size = 8;
	while (len > 0) {
		int int_st = ra_inl(NFC_INT_ST);
		/* frankliao added 20101004 */
		if (int_st & INT_ST_RX_BUF_RDY) {
			unsigned int ret_data;
			int ret_size;
			ret_data = ra_inl(NFC_DATA);
			/* frankliao added 20101005 */
			ra_outl(NFC_INT_ST, INT_ST_RX_BUF_RDY); 
			/* frankliao added 20101005 */
				
			ret_size = sizeof(unsigned long);
			ret_size = (ret_size<len) ? ret_size : len;
//			ret_size = min(ret_size, len);
			
			len -= ret_size;
			#ifdef RT63365
			// rt63365 modify
			*(unsigned int*)p = ret_data;
			p += ret_size;
			#else
			while (ret_size-- > 0) {
				//nfc is little endian 
				*p++ = ret_data & 0x0ff;
				ret_data >>= 8; 
			}
			#endif			
		} else if (int_st & INT_ST_ND_DONE) {
//			printk("!! done\n");
			break;
		} else {
			if (retry-- < 0) 
				break;
		}
	}

	return (p-buf);
}


/**
 * generic function to put data into flash.
 * @return data length writing into flash.
 */
static int 
_ra_nand_push_data(char *buf, int len)
{
	char *p = buf;
	unsigned long retry;

	// receive data by use_gdma 
/*	if (use_gdma) { 
		if (_ra_nand_dma_push((unsigned long)p, len))
			len = 0;		
		return len;
	} */

	// no gdma frankliao modify old value : 528
	retry = 100000000;

	while(len > 0) {
		int int_st = ra_inl(NFC_INT_ST);
		if (int_st & INT_ST_TX_BUF_RDY) {
			unsigned int tx_data = 0;
			int tx_size, iter = 0;

#if 1 //fixme, need optimaize in words	
//			tx_size = min(len, (int)sizeof(unsigned long));
//			tx_size = min(len, (int)sizeof(unsigned int));

			tx_size = (int)sizeof(unsigned int);
			tx_size = (len < tx_size) ? len : tx_size;
			
			#ifdef RT63365			
			// frank added 20110420 for rt63365
			tx_data = *(unsigned int*)p;
			p += tx_size;

			#else
			for (iter = 0; iter < tx_size; iter++) {
				tx_data |= ((*p++ & 0xff) << (8*iter));
			}
			#endif			
//			printf("tx_data : %x\n", tx_data);
			/* frankliao mark */
			ra_outl(NFC_INT_ST, INT_ST_TX_BUF_RDY);
			ra_outl(NFC_DATA, tx_data);

			len -= tx_size;
#else
			//optimaize
#endif
		}
		
		if (int_st & INT_ST_ND_DONE) {	// code review tag 
			break;
		} else {
			/* frankliao modify 20101006 */
			if (retry-- < 0) {
				ra_dbg("%s p:%p buf:%p \n", __func__, p, buf);
				break;
			}
		}
	}
	
	return (int)(p-buf);

}


static int 
nfc_select_chip(struct ra_nand_chip *ra, int chipnr)
{
#if (CONFIG_NUMCHIPS == 1)
	if (!(chipnr < CONFIG_NUMCHIPS))
		return -1;
	return 0;
#else
//	BUG();
	printf("not support mutiple chips\n");
	return -1;
#endif
}


/** wait nand chip becomeing ready and return queried status.
 * @param snooze: sleep time in ms unit before polling device ready.
 * @return status of nand chip
 * @return NAN_STATUS_FAIL if something unexpected.
 */
static int 
nfc_wait_ready(struct ra_nand_chip *ra)
{
	long retry;
	char status;

	retry = READ_STATUS_RETRY ;  // ndelay(100)

	while (!NFC_TRANS_DONE() && retry--) ;
	
	if (!NFC_TRANS_DONE()) {
		printf("nfc_wait_ready: no transaction done \n");
		return NAND_STATUS_FAIL;
	}

#if !defined (CONFIG_NOT_SUPPORT_RB)
	//fixme
	while(!(status = nfc_device_ready()) && retry--);

	if (status == 0) {
		printf("nfc_wait_ready: no device ready. \n");	
		return NAND_STATUS_FAIL;
	}

	// frank modify 20110421
	while (retry--) {
		_nfc_read_status(ra, &status);
		if (status & NAND_STATUS_READY)
			break;
	}

	if (retry<0) {
		printf("nfc_wait_ready 2: no device ready, status(%x). \n", status);	
	}	


//	_nfc_read_status(ra, &status);
	return status;

#else

	while (retry--) {
		_nfc_read_status(ra, &status);
		if (status & NAND_STATUS_READY)
			break;
	}

	if (retry<0) {
		printf("nfc_wait_ready 2: no device ready, status(%x). \n", status);	
	}	

	return status;

#endif
}


/**
 * return 0: erase OK
 * return -EIO: fail 
 */
static int 
nfc_erase_block(struct ra_nand_chip *ra, unsigned long row_addr)
{
	unsigned long cmd1, cmd2, conf;
	char status;

//	cmd1 = 0x60;
//	cmd2 = 0xd0;
	cmd1 = ra->opcode->erase1;
	cmd2 = ra->opcode->erase2;

	if (ra->flash->page_shift == SIZE_512iB_BIT) {
		conf = 0x00513 | ((row_addr_cycle)<<16);
	} else {
		conf = 0x00503 | ((row_addr_cycle)<<16);
	}

	ra_dbg("Erase CMD1:%2lx\n", cmd1);
	ra_dbg("Erase CMD2:%2lx\n", cmd2);
	ra_dbg("Erase BLOCK:%2lx\n", row_addr);
	ra_dbg("CONFIG:%5lx\n", conf);

	// set NFC
	ra_dbg("%s: cmd1: %lx, cmd2:%lx row_addr: %lx, conf: %lx \n", 
	       __func__, cmd1, cmd2, row_addr, conf);

	//fixme, should we check nfc status?
	CLEAR_INT_STATUS();

	ra_outl(NFC_CMD1, cmd1); 	
	ra_outl(NFC_CMD2, cmd2);
	ra_outl(NFC_ADDR, row_addr);
	ra_outl(NFC_CONF, conf); 

	status = nfc_wait_ready(ra);  //erase wait 3ms 
	if (status & NAND_STATUS_FAIL) {
		printf("%s: fail \n", __func__);
		return -EIO;
	}
	
	return 0;

}


static inline int 
_nfc_write_raw_data(struct ra_nand_chip *ra, int cmd1, int cmd3, unsigned long row_addr, 
					unsigned long column_addr, int conf, char *buf, int len)
{
	int ret, i;

	CLEAR_INT_STATUS();

	ra_outl(NFC_CMD1, cmd1); 	
	ra_outl(NFC_CMD3, cmd3); 	
	if (ra->flash->page_shift == SIZE_2KiB_BIT) {
		ra_outl(NFC_ADDR, (row_addr << (column_addr_cycle << 3)) | column_addr );
		ra_outl(NFC_ADDRII, ((row_addr>>(32 - (column_addr_cycle<<3)))));
	} else {
		ra_outl(NFC_ADDR, (row_addr << (column_addr_cycle<<3)) | column_addr );
	}

	ra_outl(NFC_CONF, conf); 

	ret = _ra_nand_push_data(buf, len);
	if (ret != len) {
		printf("%s: ret:%x (%x) \n", __func__, ret, len);
		return NAND_STATUS_FAIL;
	}

	/* frankliao modify 20101004 */
	ret = nfc_wait_ready(ra); //write wait 1ms
	if (ret & NAND_STATUS_FAIL) {
		printf("%s: fail \n", __func__);
		return NAND_STATUS_FAIL;
	}

	return 0;
}


static inline int 
_nfc_read_raw_data(struct ra_nand_chip *ra, int cmd1, int cmd2, unsigned long row_addr, 
				   unsigned long column_addr, int conf, char *buf, int len)
{
	int ret;

	CLEAR_INT_STATUS();

	ra_outl(NFC_CMD1, cmd1); 	
	
	if (ra->flash->page_shift == SIZE_2KiB_BIT) {
		ra_outl(NFC_CMD2, cmd2); 	
		ra_outl(NFC_ADDR, (row_addr << (column_addr_cycle << 3)) | column_addr );
		ra_outl(NFC_ADDRII, ((row_addr>>(32 - (column_addr_cycle << 3)))));
	} else {
		ra_outl(NFC_ADDR, (row_addr << (column_addr_cycle << 3)) | column_addr );
	}		

	ra_outl(NFC_CONF, conf); 

	ret = _ra_nand_pull_data(buf, len);

	if (ret != len) {
		printf("%s: ret:%x (%x) \n", __func__, ret, len);
		return NAND_STATUS_FAIL;
	}

	//FIXME, this section is not necessary
	// frankliao modify
	ret = nfc_wait_ready(ra); //wait ready 
	if (ret & NAND_STATUS_FAIL) {
		printf("%s: fail \n", __func__);
		return NAND_STATUS_FAIL;
	} 

	return 0;
}


/**
 * @return !0: fail
 * @return 0: OK
 */
int 
nfc_read_oob(struct ra_nand_chip *ra, int page, unsigned long offs, char *buf, int len, int flags)
{
	unsigned int cmd1 = 0;
	unsigned int conf = 0;
	unsigned int cmd2 = 0;
	unsigned int ecc_en;
	unsigned long row_addr = 0;
	unsigned long column_addr = 0;
	int use_gdma;
	int status;

	int pages_perblock = 1<<(ra->flash->erase_shift - ra->flash->page_shift);
	// constrain of nfc read function 

#if defined (WORKAROUND_RX_BUF_OV)
	if (len > 60) {
		return -1;
	}//problem of rx-buffer overrun 
#endif

	if (offs >> ra->flash->oob_shift) {
		printf("Page Boundary Overflow\n");
		return -1;
	} //	BUG_ON (offs >> ra->flash->oob_shift); //page boundry

	if ((unsigned int)(((offs + len) >> ra->flash->oob_shift) + page) >
		((page + pages_perblock) & ~(pages_perblock-1))) {
		printf("Block Boundary Overflow \n");
		return -1;
	} //block boundry

	use_gdma = flags & FLAG_USE_GDMA;
	ecc_en = flags & FLAG_ECC_EN;
	row_addr = page;
	column_addr = offs & (1 << (column_addr_cycle << 3)) -1;

//	cmd1 = 0x50;
	if (ra->flash->page_shift == SIZE_2KiB_BIT) {
		cmd1 = ra->opcode->read1;
		cmd2 = ra->opcode->read2;
		conf = 0x000511| ((addr_cycle)<<16) | ((len) << 20); 
		column_addr |= (1<<11);
	} else {
		cmd1 = ra->opcode->readoob;
		conf = 0x000141| ((addr_cycle)<<16) | ((len) << 20); 
	}

/*	if (ecc_en) 
		conf |= (1<<3); 
	if (use_gdma)
		conf |= (1<<2);
*/

	ra_dbg("%s: cmd1: %x, row_addr: %x, column_addr: %x, conf: %x, len:%x, flag:%x\n", 
	       __func__, cmd1, row_addr, column_addr, conf, len, flags);

	status = _nfc_read_raw_data(ra, cmd1, cmd2, row_addr, column_addr, conf, buf, len);
	
	if (status & NAND_STATUS_FAIL) {
		printf("%s: fail\n", __func__);
		return -EIO;
	}

	return 0; 
}


/**
 * @return -EIO, writing size is less than a page 
 * @return 0, OK
 */
int 
nfc_read_page(struct ra_nand_chip *ra, char *buf, int page
#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
    , int flags
#endif
)
{
	unsigned int cmd1 = 0, cmd2 = 0, conf = 0;
	unsigned long row_addr = 0;
	unsigned long column_addr = 0;
	int pagesize, size, offs;
	int status = 0;
#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
    int ecc_en = flags & FLAG_ECC_EN;
#endif


	page = page & ((1 << ra->flash->chip_shift)-1); // chip boundary

	pagesize = (1 << ra->flash->page_shift);

	/* frankliao added 20101029, read size make status reg show ecc error*/
#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
	if (ecc_en){
	    size = pagesize + (1 <<ra->flash->oob_shift);   //add oobsize
	}
	else{
#endif	
		size = pagesize;
#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
    }
#endif		

	offs = 0;
	while (size > 0) {
		int len;
#if defined (WORKAROUND_RX_BUF_OV)
		len = min(60, size);
#else
		len = size;
#endif		
		row_addr = page;
		column_addr = offs & ((1<< (column_addr_cycle << 3))-1);

		if (ra->flash->page_shift == SIZE_512iB_BIT) {
			/* frankliao added 512 bytes ~ 528 bytes 20101012*/
			if (unlikely((offs & ~((1 << ra->flash->page_shift)-1))))
				cmd1 = ra->opcode->readoob;
			/* frankliao added 256 bytes ~ 512 bytes 20101012*/
			else if (offs & ~((1<< (column_addr_cycle << 3))-1))
				cmd1 = ra->opcode->readB;
			else
				cmd1 = ra->opcode->read1;

			conf = 0x000141 | ((addr_cycle)<<16) | (len << 20); 

		} else if (ra->flash->page_shift == SIZE_2KiB_BIT) {
			cmd1 = ra->opcode->read1;
			cmd2 = ra->opcode->read2;
			conf = 0x000511 | ((addr_cycle)<<16) | (len << 20); 
		}
			
		// frankliao
#if !defined (WORKAROUND_RX_BUF_OV)
//		if (ecc_en) 
//			conf |= (1<<3); 
#endif
//		if (use_gdma)
//			conf |= (1<<2);

#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
		if (ecc_en) 
			conf |= (1<<3); 
#endif

		status = _nfc_read_raw_data(ra, cmd1, cmd2, row_addr, column_addr, conf, buf+offs, len);
		if (status & NAND_STATUS_FAIL) {
			printf("%s: fail \n", __func__);
			return -EIO;
		}

		offs += len;
		size -= len;
	}

	// verify and correct ecc frankliao mark
	
#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
    if ((flags & (FLAG_VERIFY | FLAG_ECC_EN)) == (FLAG_VERIFY | FLAG_ECC_EN)) {
		status = nfc_ecc_verify(ra, buf, page, FL_READING);	
		if (status != 0) {
			printf("%s: fail, buf:%x, page:%x, flag:%x\n", 
			       __func__, (unsigned int)buf, page, flags);
			return -EBADMSG;
		}
	}
#endif
//	else {
		// fix,e not yet support
//	} 

	return 0;
}


/** 
 * @return -EIO, fail to write
 * @return 0, OK
 */
int 
nfc_write_page(struct ra_nand_chip *ra, char *buf, int page
#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
    , int flags
#endif
)
{
	unsigned int cmd1 = 0, cmd3, conf = 0;
	unsigned long row_addr = 0;
//	unsigned int ecc_en;
//	int use_gdma;
	int pagesize;
	char status;

//	uint8_t *oob = buf + (1 << ra->flash->page_shift);

//	use_gdma = flags & FLAG_USE_GDMA;
//	ecc_en = flags & FLAG_ECC_EN;
//	oob[ra->flash->badblockpos] = 0xff;	//tag as good block.

	page = page & ((1<<ra->flash->chip_shift)-1); // chip boundary

//	pagesize = (1 << ra->flash->page_shift);
	pagesize = (1 << ra->flash->page_shift) + (1 << ra->flash->oob_shift);
//	ra_dbg("PAGESIZE = %d\n", pagesize);
//	ra_dbg("FLAGS = %d\n", flags);

	/* frankliao added 20101029 */
//	if (ecc_en) {
//		pagesize = pagesize + (1 << ra->flash->oob_shift);
//	}	
	
	row_addr = page;

	/* frankliao modify 20101004 */
	cmd1 = ra->opcode->pageprog1;
	cmd3 = ra->opcode->pageprog2;

	/* frankliao modify 20100105 */
	if (ra->flash->page_shift == SIZE_512iB_BIT) {
		conf = 0x001243 | ((addr_cycle)<<16) | ((pagesize) << 20);
	} else if (ra->flash->page_shift == SIZE_2KiB_BIT) {
		conf = 0x001103 | ((addr_cycle)<<16) | ((pagesize) << 20);
	} else {
		printf("Undefined Write Page Command\n");
		return -EIO;
	}

//	if (ecc_en) 
	conf |= (1<<3); 
//	if (use_gdma)
//		conf |= (1<<2);

	// set NFC
	ra_dbg("nfc_write_page: cmd1: %x, cmd3: %x row_addr: %x, conf: %x, len:%x\n", 
	       cmd1, cmd3, row_addr, conf, pagesize);
	status = _nfc_write_raw_data(ra, cmd1, cmd3, row_addr, 0, conf, buf, pagesize);

	if (status & NAND_STATUS_FAIL) {
		printf("%s: fail \n", __func__);
		return -1;
	}
#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
	if (flags & FLAG_VERIFY) { // verify and correct ecc
		status = nfc_ecc_verify(ra, buf, page, FL_WRITING);
	
		if (status != 0) {
			printf("%s: ecc_verify fail: ret:%x \n", __func__, status);
			return -EBADMSG;
		}
	}
#endif

	return 0;
}

#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
/**
 * nand_block_isbad - [MTD Interface] Check if block at offset is bad
 * @mtd:	MTD device structure
 * @offs:	offset relative to mtd start
 */
int 
nand_block_isbad(struct ra_nand_chip* ra, unsigned long offs)
{
	int page;
	int ret = 4;
	unsigned int tag;
	int pages_shift_perblock = ra->flash->erase_shift - ra->flash->page_shift;

	// align with chip
	offs = offs & ((1<<ra->flash->chip_shift) - 1);

	page = offs >> ra->flash->page_shift;
	// frankliao added 20101227
	page = page >> pages_shift_perblock;
	page = page << pages_shift_perblock;

	ret = nfc_read_oob(ra, page, ra->flash->badblockpos, (char*)&tag, 1, FLAG_NONE);

	tag = tag >> 24;
	if (ret == 0 && ((tag & 0xff) == 0xff)) 
		return 0;
	else
		return 1;
}

/**
 * nand_block_markbad -
 */
 
int nand_block_markbad(struct ra_nand_chip *ra, unsigned long offs)
{
	int page, block;
	int ret;
	unsigned int tag = 0;
	char *ecc;

	// align with chip
	offs = offs & ((1<<ra->flash->chip_shift) - 1);
	
	block = offs >> ra->flash->erase_shift;
	printf("mark bad block:%d \n", block);
	page = block * (1<<(ra->flash->erase_shift - ra->flash->page_shift));

    ret = nfc_read_page(ra, ra->buffers, page, FLAG_ECC_EN);

	if (ret != 0) {
    	printf("%s: fail to read bad block tag \n", __func__);
    }

    ecc = &ra->buffers[(1<<ra->flash->page_shift) + ra->flash->badblockpos];

    if (*ecc == (char)0xff) {
    //tag into flash
        *ecc = (char)tag;
        //		ret = nfc_write_page(ra, ra->buffers, page, FLAG_USE_GDMA);
        ret = nfc_write_page(ra, ra->buffers, page, FLAG_ECC_EN);
        if (ret) {
        	printf("%s: fail to write bad block tag \n", __func__);	
        }
    }	

}

/**
 * nfc_ecc_verify 
 */
int nfc_ecc_verify(struct ra_nand_chip *ra, unsigned char *buf, int page, int mode)
{
	int ret, i, j, ecc_num;
	unsigned char *p, *e;

	/* 512 bytes data has a ecc value */
	int ecc_bytes, ecc_offset, ecc[4];
	
//	printf("%s, page:%x mode:%d\n", __func__, page, mode);

	if (mode == FL_WRITING) {
		int len = (1 << ra->flash->page_shift) + (1 << ra->flash->oob_shift);
		int conf; 

		if (ra->flash->page_shift == SIZE_512iB_BIT)
			conf = 0x000141; 
		else
			conf = 0x000511;

		conf |= ((addr_cycle) << 16) | (len << 20);
		conf |= (1<<3); //(ecc_en) 
//		conf |= (1<<2); // (use_gdma)

		p = ra->readback_buffers;

//		ret = nfc_read_page(ra, ra->readback_buffers, page, FLAG_USE_GDMA | FLAG_ECC_EN); 
		ret = nfc_read_page(ra, ra->readback_buffers, page, FLAG_ECC_EN); 
//		ret = nfc_read_page(ra, ra->readback_buffers, page, FLAG_NONE); 
		if (ret == 0) 
			goto ecc_check;
		
		//FIXME, double comfirm
		printf("%s: read back fail, try again \n",__func__);
//		ret = nfc_read_page(ra, ra->readback_buffers, page, FLAG_USE_GDMA | FLAG_ECC_EN); 
		ret = nfc_read_page(ra, ra->readback_buffers, page, FLAG_ECC_EN); 
//		ret = nfc_read_page(ra, ra->readback_buffers, page, FLAG_NONE); 


		if (ret != 0) {
			printf("\t%s: read back fail agian \n",__func__);
			goto bad_block;
		}
	} else if (mode == FL_READING) {
		p = buf;
	} else {
		return -2;
	}

ecc_check:

	/* frankliao modify 20101018 */
	p += (1<<ra->flash->page_shift);
	
	ecc[0] = ra_inl(NFC_ECC); 
	
	if (ecc[0] == 0) {
		//printf("clean page.\n");
		return 0;
	}	

	ecc_bytes = ra->oob->eccbytes;

	ecc_offset = ra->oob->eccpos[0];
	
	/* each ecc register store 3 bytes ecc value */
	if (ecc_bytes == 12) {
		ecc[1] = ra_inl(NFC_ECCII); 
		ecc[2] = ra_inl(NFC_ECCIII); 
		ecc[3] = ra_inl(NFC_ECCIV); 
	}

	ecc_num = ecc_bytes/3;
	for (i=0; i<ecc_num; i++) {
		e = (unsigned char*)&ecc[i];
		ecc_offset = ra->oob->eccpos[i * 3];
		/* each ecc register store 3 bytes ecc value */
		for (j=0; j<3; j++) {

			#ifdef __LITTLE_ENDIAN
			int eccpos = ecc_offset + j + i*3;
			#else
			int eccpos = ecc_offset - j + 2;
			#endif

			if (*(p + eccpos) != *(e + j + 1)) {

			#ifdef __LITTLE_ENDIAN
				printf("%s mode:%s, invalid ecc, page: %x read:%x %x %x, ecc:%x \n",
				       __func__, (mode == FL_READING)?"read":"write", page,	
				       *(p+ecc_offset), *(p+ecc_offset+1), *(p+ecc_offset+2), ecc[i]);
			#else
				printf("%s mode:%s, invalid ecc, page: %x read:%x %x %x, ecc:%x \n",
				       __func__, (mode == FL_READING)?"read":"write", page,	
				       *(p+ecc_offset+2), *(p+ecc_offset+1), *(p+ecc_offset), ecc[i]);
			#endif
				return -1;
			}
		}
	}

	return 0;

bad_block:
	return -1;
}

int nandflash_scan_badblock(void)
{
    int i, j, badblock;
    int addr = 0;
    int blocksize = 1 << ra.flash->erase_shift;
    int totalblock = 1024;
   
    for(i = 0; i < totalblock; i++)
    {
        block_table[i] = i;
    }

    for(j = 0; j < totalblock; j++)
    {

        if(nand_block_isbad(&ra, addr))
        {
            badblock = addr/blocksize;

            //printf("badblock:%d \n", badblock);
            if(badblock >= TCROMFILE_START && badblock < TCROMFILE_END)
            {

                for(i = TCROMFILE_START; i < TCROMFILE_END; i++)
                {
                    if(block_table[i] == badblock)
                    {
                        break;
                    }

                }

                for(; i < TCROMFILE_END; i++)
                {
                     block_table[i]++;
                }
            
            }else if(badblock >= TCLINUX_BLOCK_START && badblock < TCLINUX_BLOCK_END){

                for(i = TCLINUX_BLOCK_START; i < TCLINUX_BLOCK_END; i++)
                {
                    if(block_table[i] == badblock)
                    {
                        break;
                    }

                }

                for(; i < TCLINUX_BLOCK_END; i++)
                {
                     block_table[i]++;
                }
            
            }
            else if(badblock >= TCLINUX_SLAVE_BLOCK_START && badblock < TCLINUX_SLAVE_BLOCK_END){

                for(i = TCLINUX_SLAVE_BLOCK_START; i < TCLINUX_SLAVE_BLOCK_END; i++)
                {
                    if(block_table[i] == badblock)
                    {
                        break;
                    }

                }

                for(; i < TCLINUX_SLAVE_BLOCK_END; i++)
                {
                     block_table[i]++;
                }


            }
            else if(badblock >= TCBKROMFILE_START && badblock < TCBKROMFILE_END){

                for(i = TCBKROMFILE_START; i < TCBKROMFILE_END; i++)
                {
                    if(block_table[i] == badblock)
                    {
                        break;
                    }

                }

                for(; i < TCBKROMFILE_END; i++)
                {
                     block_table[i]++;
                }

            }
            

        }
        
        addr += blocksize;

    }

    return 0;

}

int nand_erase_next_goodblock(struct ra_nand_chip *ra, int block, unsigned long addr_l)
{
    unsigned int blocksize = 1 << ra->flash->erase_shift;
    unsigned int pagesize = 1 << ra->flash->page_shift;
    unsigned long offset;
    int page;
    nand_block_markbad(ra, addr_l);
    
    block++;
    offset = block * blocksize;
    offset += (block_table[block] - block) << ra->flash->erase_shift;
    
    page = (unsigned long)(offset >> ra->flash->page_shift);

    while(nfc_erase_block(ra, page))
    {

        printf("erase page(%d) fail__2 \n", page);
        
        nand_block_markbad(ra, offset);
        
        block++;
        offset = block * blocksize;
        offset += (block_table[block] - block) << ra->flash->erase_shift;
        
        page = (unsigned long)(offset >> ra->flash->page_shift);

    }

    nandflash_scan_badblock();

    return 0;

}

int nand_write_next_goodblock(struct ra_nand_chip *ra, int page_u, int page_l)
{
    int src_page, npage, nextblk_startaddr, nextblk_writeaddr, ret, block, addr;
    int readstart_page, writestart_page, last_page, readtotal_page, to_page;
    uint8_t *pbuf = NULL;
    uint32_t pagesize = (1 << ra->flash->page_shift);
    uint32_t blocksize = (1 << ra->flash->erase_shift);
    uint8_t page_data[pagesize + (1 << ra->flash->oob_shift)];

    src_page = page_l >> (ra->flash->erase_shift - ra->flash->page_shift);
    src_page = src_page << (ra->flash->erase_shift - ra->flash->page_shift);

    readstart_page = src_page;

    npage = page_l - src_page + 1;
    readtotal_page = npage;
    
    block = page_u >> (ra->flash->erase_shift - ra->flash->page_shift);
    addr = block << ra->flash->erase_shift;

    block++;
    addr += blocksize;
    
    nextblk_startaddr = addr + ((block_table[block] - block) << ra->flash->erase_shift);
    nextblk_writeaddr = nextblk_startaddr + (page_l - src_page) * pagesize;
    
    writestart_page = nextblk_startaddr >> ra->flash->page_shift;

    memcpy(page_data, ra->buffers, pagesize + (1 << ra->flash->oob_shift));

    nandflash_erase(next_erase_addr, blocksize);
    next_erase_addr += blocksize;

    while(readtotal_page > 0)
    {
        if(readtotal_page > 1){

            memset(ra->buffers, 0xff, pagesize + (1 << ra->flash->oob_shift));
    
            ret = nfc_read_page(ra, ra->buffers, readstart_page, 0); 
            if(ret)
            {
                ret = nfc_read_page(ra, ra->buffers, readstart_page, 0); 
                if(ret)
                    printf("%s: read page fail", __func__);

            }
            to_page = writestart_page;
            pbuf = ra->buffers;
        }
        else{
            to_page = nextblk_writeaddr >> ra->flash->page_shift;
            pbuf = page_data;

        }
        
        ret = nfc_write_page(ra, pbuf, to_page, FLAG_ECC_EN | FLAG_VERIFY);
        if(ret)
        {

            printf("write page(%d) fail__2 \n", to_page);

            nfc_erase_block(ra, writestart_page);
            nand_block_markbad(ra, nextblk_startaddr);    
            
            nandflash_erase(next_erase_addr, blocksize);
            next_erase_addr += blocksize;
            
            block++;
            addr += blocksize;
            nextblk_startaddr = addr + ((block_table[block] - block) << ra->flash->erase_shift);
            writestart_page = nextblk_startaddr >> ra->flash->page_shift;
            nextblk_writeaddr = nextblk_startaddr + (page_l - src_page) * pagesize;
            readstart_page = src_page;
            readtotal_page = npage;
            continue;

        }
        writestart_page++;
        readstart_page++;
        readtotal_page--;

    }

    nfc_erase_block(ra, src_page);
    nand_block_markbad(ra, src_page * pagesize);

    nandflash_scan_badblock();

    return to_page;

}

#endif

/**
 * nandflash_write_internal - [Internal] NAND write
 * NAND write with ECC
 */

//static int 
//nand_do_write_ops(struct ra_nand_chip *ra, loff_t to,
//			     struct mtd_oob_ops *ops)

static int
nandflash_write_internal(struct ra_nand_chip *ra, unsigned char *buf, 
			unsigned long to, unsigned long datalen, unsigned long *retlen)
{
	int page;
	unsigned long len;
	int pagesize = (1 << ra->flash->page_shift);
	int pagemask = (pagesize -1);
	int oobsize = 1 << ra->flash->oob_shift;
	int i, cnt;
	unsigned long addr = (unsigned long) to;

#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
    int block;
    int srcblock = addr >> ra->flash->erase_shift;
    unsigned long srcaddr = addr;
    int srcpage;
#endif	

//	block = addr >> ra->flash->erase_shift;
//	/* frankliao added 20101029 */
//	tag = nand_bbt_get(ra, block);

//	if (tag == BBT_TAG_BAD) {
//		return -EFAULT;
//	}

//	ra_dbg("%s: to:%x, ops data:%p, oob:%p datalen:%x ooblen:%x, ooboffs:%x oobmode:%x \n", 
//	       __func__, (unsigned int)to, data, oob, datalen, ooblen, ops->ooboffs, ops->mode);

#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
    addr += (block_table[srcblock] - srcblock) << ra->flash->erase_shift;

    if(addr & (~((1 << ra->flash->chip_shift) - 1))){
        printf("%s: address over chip size, write fail \n", __func__);
        return -1;
    }
#endif

	*retlen = 0;
	if (buf == 0) {
		return -1;
	}	
	

	// page write
	while (datalen) {

		int len;
		int ret;
		unsigned long offs;
		cnt++;
		if ((cnt & 0x1ff) == 0)
			printf(".");

//		int ecc_en = 0;

//		ra_dbg("%s : addr:%x, buf:%p, datalen:%x \n", 
//		       __func__, (unsigned int)addr, buf, datalen);

#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
        block = srcaddr >> ra->flash->erase_shift;
        if(srcblock != block)
		{   
            srcblock = block;
            addr = srcaddr + ((block_table[block] - block) << ra->flash->erase_shift);	
        }
#endif


		page = (int)((addr & ((1<<ra->flash->chip_shift)-1)) >> ra->flash->page_shift); //chip boundary

#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
        srcpage = (int)((srcaddr & ((1<<ra->flash->chip_shift)-1)) >> ra->flash->page_shift); 
#endif
		
		/* select chip, and check if it is write protected */
//		if (nfc_enable_chip(ra, addr, 0))
//			return -EIO;

		if (nfc_check_wp()) {

			printf("%s: nand is write protected\n", __func__);

			return -1;
		}	

#if 0
		/*
		 * if we have a bad block, we shift a blocks !
		 */
		if (nand_block_isbad(ra, addr)) {
			printf("%s : skip writing a bad block %x -->", __func__, (unsigned int)addr);
			addr += (1<<ra->flash->erase_shift);
			continue;
		}	
#endif

		// data write
		offs = addr & pagemask;
//		len = min_t(size_t, datalen, pagesize - offs);
		len = (datalen < (pagesize - offs)) ? datalen : (pagesize - offs);

		/* frankliao mark */
//		len = datalen;
		
		if (buf && len > 0) {
			memset(ra->buffers, 0xff, pagesize + oobsize);
			memcpy(ra->buffers + offs, buf, len);	// we can not sure ops->buf wether is DMA-able.
			buf += len;
			datalen -= len;
			*retlen += len;
		}

		ret = nfc_write_page(ra, ra->buffers, page
		#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
            , FLAG_ECC_EN | FLAG_VERIFY
		#endif
		);

		if (ret) {
		
        #ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
            printf("write page(%d) fail \n", page);
            page = nand_write_next_goodblock(ra, srcpage, page);
		#else
		
    //			nand_bbt_set(ra, addr >> ra->flash->erase_shift, BBT_TAG_BAD);
    			printf("Write Page Fail, Attempt to Write a Bad Block %x\n", (unsigned int)addr);
    			return ret;	
        #endif
		}

//		nand_bbt_set(ra, addr >> ra->flash->erase_shift, BBT_TAG_GOOD);

		addr = (page+1) << ra->flash->page_shift;

		#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
		srcaddr = (srcpage+1) << ra->flash->page_shift;
        #endif
		
	}

	printf("program from %x to %x\n", to, (unsigned int)addr);
	
	return 0;
}


/*
 * nand_setup - setup nand flash info and opcode
 * frankliao added 20101014
 */
static int
nandflash_setup(void)
{
	int flash_id, i, subpage_bit = 0;
	unsigned int id_mask, mfr_id, dev_id, nbits, endian = 0;
	int alloc_size, bbt_size, buffers_size;

	struct nand_info *flash = NULL;
	struct nand_opcode *opcode = NULL;

	flash_id = nfc_read_id();

	if (flash_id == -1) {
		printf("read flash id fail\n");
		return -1;
	}
	
	id_mask = 0xffff;
	nbits = 16;
	if ((flash_id >> nbits) != 0) {
		nbits = 8;
		id_mask = 0xff;
	}

	#ifdef RT63365
	endian = ra_inl(NFC_CTRLII) & 0x02;

	if (endian == NFC_BIG_ENDIAN) {
		mfr_id = ((flash_id >> 24) & id_mask);
		dev_id = ((flash_id >> (24-nbits)) & id_mask);
	} else {
		mfr_id = (flash_id & id_mask);
		dev_id = ((flash_id >> nbits) & id_mask);
	}
	#else
	mfr_id = (flash_id & id_mask);
	dev_id = ((flash_id >> nbits) & id_mask);
	#endif
//	mfr_id = (flash_id & id_mask);
//	dev_id = ((flash_id >> nbits) & id_mask);
#if 0
    printf("mfr_id: 0x%x\n", mfr_id);
    printf("dev_id: 0x%x\n", dev_id);
    printf("3rd_id: 0x%x\n", (flash_id >> 8) & 0xff);
    printf("4th_id: 0x%x\n", (flash_id) & 0xff);
#endif
	ra_dbg("endian : %2x\n", endian);
	ra_dbg("mfr_id : %2x\n", mfr_id);
	ra_dbg("dev_id : %2x\n", dev_id);

	for (i=0; i < sizeof(flash_tables) / sizeof(struct nand_info); i++) {
		if ((mfr_id == flash_tables[i].mfr_id) && (dev_id == flash_tables[i].dev_id)) {
			flash = &flash_tables[i];
			break;
		}
	}
	
	if (flash == NULL) {
		printf("Undefined Manufactor ID and Devcie ID\n");
		return -1;
	}
	
	if (flash->page_shift == SIZE_512iB_BIT) {
		subpage_bit = 1;
	}

	column_addr_cycle = (flash->page_shift - subpage_bit + 7) / 8;
	row_addr_cycle = (flash->chip_shift - flash->page_shift + 7) / 8;
	addr_cycle = column_addr_cycle + row_addr_cycle;

	for (i=0; i < sizeof(opcode_tables) / sizeof(struct nand_opcode); i++) {
		if (flash->opcode_type == opcode_tables[i].type) {
			opcode = &opcode_tables[i];
			break;
		}
	}

	if (opcode == NULL) {
		printf("Undefined Opcode\n");
		return -1;
	}

	memset((void*) &ra, 0, sizeof(struct ra_nand_chip));
	memset((void*) buffers, 0, sizeof(buffers));

#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
    memset((void*) readback_buffers, 0, sizeof(readback_buffers));
    ra.readback_buffers = readback_buffers;
#endif

	ra.buffers = buffers;

	if (flash->page_shift == SIZE_512iB_BIT) {
		ra.oob				= &oob_layout_tables[ STANDARD_SMALL_FLASH ];
	} else if (flash->page_shift == SIZE_2KiB_BIT) {
		ra.oob				= &oob_layout_tables[ STANDARD_LARGE_FLASH ];
	} else {
		printf("Undefined NAND OOB LAYOUT\n");
		return -1;
	}

	ra.flash = flash;
	ra.opcode = opcode;

	return 0;
}


/************************************************************
 * the init/exit section.
 */

int 
nandflash_init(int rom_base) 
{
	int ret;
	int int_st;
	int retry;

	/* frank */
	if ( (ra_inl(CR_AHB_BASE+0x64) >> 16) == 0x4){      
		ra_outl(0xbfb00860, ra_inl(0xbfb00860) | (0x00000100));   
	}	

	/* frankliao added 20101028 */
#ifdef RT63365		
	/* frank */
//	if (ra.flash->page_shift == SIZE_2KiB_BIT) {
	if (ra_inl(CR_AHB_HWCONF) & (0x00080000)) {      
		ra_outl(NFC_CTRLII, 0x76501);    
//	} else  if (ra.flash->page_shift == SIZE_512iB_BIT) {
	} else {
		ra_outl(NFC_CTRLII, 0x76500);
	}
	ra_outl(NFC_CTRLII, ra_inl(NFC_CTRLII) | 0x02); //big endian for rt63365  
#else
//	if (ra.flash->page_shift == SIZE_2KiB_BIT) {
	if (ra_inl(CR_AHB_HWCONF) & (0x00080000)) {
		ra_outl(NFC_CTRLII, ra_inl(NFC_CTRLII) | 0x0); 
//	} else  if (ra.flash->page_shift == SIZE_512iB_BIT) {
	} else {
		ra_outl(NFC_CTRLII, ra_inl(NFC_CTRLII) | 0x01); 
	}

//	if (ra.flash->page_shift == SIZE_512iB_BIT) {		// RT63165
//		ra_outl(NFC_CTRLII, ra_inl(NFC_CTRLII) | 0x01); 
//	}	
#endif	

//	ra_outl(NFC_CTRL, ra_inl(NFC_CTRL) | 0x01); //set wp to high
	ra_outl(NFC_CTRL, 0x2408881);  

	if ( nfc_all_reset(&ra) !=0 ) {
		printf("NAND Flash Reset Fail\n");
		return -1;
	}

	ret = nandflash_setup();

	if (ret != 0) {
		printf("NAND Flash Setup Fail\n");
		return -1;
	}

#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
    nandflash_scan_badblock();
#endif

#if defined(TCSUPPORT_DUAL_IMAGE_ENHANCE) && !defined(LZMA_IMG)
	offset = (1 << ra.flash->chip_shift) / 2;
	printf("nand flash offset: %x\n", offset);
#endif

	return 0;
}


/**
 * nandflash_read_internal - [Internal] Read data
 *
 * Internal function. Called with chip held.
 */

//static int 
//nand_do_read_ops(struct ra_nand_chip *ra, loff_t from,
//			    struct mtd_oob_ops *ops)
static int
nandflash_read_internal(struct ra_nand_chip* ra, unsigned char *buf, unsigned long from, 
						unsigned long datalen, unsigned long* retlen)
{
	int page, ret;
	int pagesize = (1 << ra->flash->page_shift);
	int pagemask = (pagesize -1);
    
	unsigned long addr = from;

#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
    int block;
    int srcblock = addr >> ra->flash->erase_shift;
    unsigned long srcaddr = addr;
    int srcpage;
#endif

	*retlen = 0;

	ra_dbg("%s: addr:%x, datalen:%x \n", 
	       __func__, addr, datalen);

#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
    addr += (block_table[srcblock] - srcblock) << ra->flash->erase_shift;

    if(addr & (~((1 << ra->flash->chip_shift) - 1))){
        printf("%s: address over chip size, read fail \n", __func__);
        return -1;
    }
#endif

	/* frankliao added 20101029 */
/*	block = addr >> ra->flash->erase_shift;
	tag = nand_bbt_get(ra, block);
	if (tag == BBT_TAG_BAD) {
		return -EFAULT;
	}
*/

	if (buf == 0)
		return 0;

	while (datalen) {

		unsigned long len, offs;

		ra_dbg("%s : addr:%x, buf:%p, datalen:%x \n", 
		       __func__, (unsigned int)addr, buf, datalen);

#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
        
		block = srcaddr >> ra->flash->erase_shift;
	
		if(srcblock != block)
		{   
            srcblock = block;
            addr = srcaddr + ((block_table[block] - block) << ra->flash->erase_shift);	
		}
#endif        

		page = (int)((addr & ((1<<ra->flash->chip_shift)-1)) >> ra->flash->page_shift); 

#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
        srcpage = (int)((srcaddr & ((1<<ra->flash->chip_shift)-1)) >> ra->flash->page_shift); 
#endif
		

#if 0
		/*
		 * if we have a bad block, we shift a blocks !
		 */		
		if (nand_block_isbad(ra, addr)) {
			printf("%s : skip reading a bad block %x -->", __func__, (unsigned int)addr);

			addr += (1<<ra->flash->erase_shift);

			printf(" %x\n", (unsigned int)addr);

			continue;
		}			
#endif

		ret = nfc_read_page(ra, ra->buffers, page
		#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
            , FLAG_ECC_EN // | FLAG_VERIFY  
		#endif
		);

		//FIXME, something strange here, some page needs 2 more tries to guarantee read success.
		if (ret) {
			printf("read again:\n");
			/* frankliao modify 20101001 */
			ret = nfc_read_page(ra, ra->buffers, page
		#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
            , FLAG_ECC_EN //| FLAG_VERIFY  
		#endif
		    ); 
			if (ret) {
				printf("read again fail \n");				
			} else {
				printf(" read again susccess \n");
			}
		}
		// data read
		offs = addr & pagemask;
		/* frankliao mark */
		len = (datalen < (pagesize-offs)) ? datalen : (pagesize-offs);
		if (buf && len > 0) {
			memcpy(buf, ra->buffers + offs, len);	// we can not sure ops->buf wether is DMA-able.
			buf += len;
			datalen -= len;
			*retlen += len;
			if (ret) {
				return -1;
			}
		}
		addr = (page+1) << ra->flash->page_shift;
		
	    #ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
		srcaddr = (srcpage+1) << ra->flash->page_shift;
        #endif	
	}
	
	return 0;
}




/**
 * nand_erase_internal - [Internal] erase block(s)
 * Erase one ore more blocks
 */
static int 
nandflash_erase_internal(struct ra_nand_chip *ra, unsigned long offset, unsigned long len)
{
	int status, ret;
	unsigned long page;
	unsigned int blocksize = 1<<ra->flash->erase_shift;

#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
    int block;
    int srcblock = offset >> ra->flash->erase_shift;
    unsigned long srcaddr = offset;
#endif

	ret = 0;

	ra_dbg("%s: start:%x, len:%x \n", __func__, 
	       offset, (unsigned int)len);

#define BLOCK_ALIGNED(a) ((a) & (blocksize - 1))

	if (BLOCK_ALIGNED(len)) {
		len = (len/blocksize + 1) * blocksize;
	}

	if (BLOCK_ALIGNED(offset) || BLOCK_ALIGNED(len)) {
		ra_dbg("%s: erase block not aligned, addr:%x len:%x\n", __func__, offset, len);
		return -EINVAL;
	}

#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
    offset += (block_table[srcblock] - srcblock) << ra->flash->erase_shift;

    if(offset & (~((1 << ra->flash->chip_shift) - 1))){
        printf("%s: address over chip size, erase fail \n", __func__);
        return -1;
    }    
#endif

	while (len) {


#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
        block = srcaddr >> ra->flash->erase_shift;
        if(srcblock != block)
		{   
            srcblock = block;
            offset = srcaddr + ((block_table[block] - block) << ra->flash->erase_shift);	
        }
#endif

		page = (unsigned long)(offset >> ra->flash->page_shift);

		/* select device and check wp */
		if (nfc_check_wp()) {
			printf("%s: nand is write protected \n", __func__);
			return -1;
		}
#if 0
		/*
		 * if we have a bad block, we do not erase bad blocks !
		 */
		if (nand_block_isbad(ra, offset)) {

			printf("nand_erase: attempt to erase a "
			       "bad block at 0x%x\n", offset);
			printf("%s: skip erasing a bad block\n", __func__);
			offset += blocksize;
			ret ++;
			continue;
		}	
#endif
		status = nfc_erase_block(ra, page);
		        
		/* See if block erase succeeded */
		if (status) {
		
#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK

                printf("erase page(%d) fail \n", page);
                nand_erase_next_goodblock(ra, block, offset);

#else
		    
			    printf("%s: failed erase, block 0x%08x\n", __func__, page);
			    return -1;
#endif


		}

//		printf("erase addr: 0x%x \n", offset);

		/* Increment page address and decrement length */
		len -= blocksize;
		offset += blocksize;

		#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
		srcaddr += blocksize;
		next_erase_addr = srcaddr;
        #endif

	}

	return ret;
}


int 
nandflash_erase(unsigned long offset, unsigned long len)
{
	return nandflash_erase_internal(&ra, offset, len);
}


int nandflash_read(unsigned long from,
	unsigned long len, unsigned long *retlen, unsigned char *buf)
{
	return nandflash_read_internal(&ra, buf, from, len, retlen);
}


int nandflash_write(unsigned long to,
	unsigned long len, unsigned long *retlen, const unsigned char *buf)
{
	return nandflash_write_internal(&ra, buf, to, len, retlen);
}


#define NAND_BUFFER_SIZE     	2048
static unsigned long buffer_init = 0;
static unsigned long nand_offset = 0;
static unsigned char buf[ NAND_BUFFER_SIZE ];

unsigned char ReadNandCache(unsigned long index)
{
	int ret;
	unsigned long retlen;

	if (!buffer_init) {
		ret = nandflash_read(index, NAND_BUFFER_SIZE, &retlen, buf);
		if (ret != 0)
			return -1;
		nand_offset = index;
		buffer_init = 1;
	}

	if ((index >= (nand_offset + NAND_BUFFER_SIZE)) || (index < nand_offset)){
		ret = nandflash_read(index, NAND_BUFFER_SIZE, &retlen, buf);
		if (ret != 0)
			return -1;
		nand_offset = index;
	}
		
	return buf[ (int)(index - nand_offset) ];

}


unsigned char ReadNandByte(unsigned long index)
{
	unsigned char data;
	int ret;
	unsigned long retlen;
	ret = nandflash_read(index, 1, &retlen, &data);
	if (ret != 0)
		 return -1;

	return data;
}

unsigned long ReadNandDWord(unsigned long index)
{
	unsigned char data[4];
	unsigned long retlen, dword;
	int ret, i;

	ret = nandflash_read(index, 4, &retlen, data);
	if (ret != 0)
		return -1;

	dword = 0;
	for (i=0; i<4; i++) {
		dword += (unsigned long)data[i];
		if (i<3)
			dword <<= 8;
	}

	return dword;
}
