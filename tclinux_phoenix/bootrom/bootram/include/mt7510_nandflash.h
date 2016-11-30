#ifndef MT7510_NAND_H
#define MT7510_NAND_H

typedef volatile unsigned short *P_U16;

typedef volatile unsigned int   *P_U32;
#define bool uint8 
#define true 1
#define false 0

#define READ_REGISTER_UINT32(reg) \
    (*(volatile unsigned int  * const)(reg))

#define WRITE_REGISTER_UINT32(reg, val) \
    (*(volatile unsigned int  * const)(reg)) = (val)

#define INREG32(x)          READ_REGISTER_UINT32((unsigned int *)((void*)(x)))
#define OUTREG32(x, y)      WRITE_REGISTER_UINT32((unsigned int *)((void*)(x)), (unsigned int )(y))
#define SETREG32(x, y)      OUTREG32(x, INREG32(x)|(y))
#define CLRREG32(x, y)      OUTREG32(x, INREG32(x)&~(y))
#define MASKREG32(x, y, z)  OUTREG32(x, (INREG32(x)&~(y))|(z))

#define DRV_Reg8(addr)              INREG32(addr)
#define DRV_WriteReg8(addr, data)   OUTREG32(addr, data)
#define DRV_SetReg8(addr, data)     SETREG32(addr, data)
#define DRV_ClrReg8(addr, data)     CLRREG32(addr, data)

#define DRV_Reg16(addr)             INREG32(addr)
#define DRV_WriteReg16(addr, data)  OUTREG32(addr, data)
#define DRV_SetReg16(addr, data)    SETREG32(addr, data)
#define DRV_ClrReg16(addr, data)    CLRREG32(addr, data)

#define DRV_Reg32(addr)             INREG32(addr)
#define DRV_WriteReg32(addr, data)  OUTREG32(addr, data)
#define DRV_SetReg32(addr, data)    SETREG32(addr, data)
#define DRV_ClrReg32(addr, data)    CLRREG32(addr, data)

#define NFI_DEFAULT_ACCESS_TIMING 0xF3FFFFFF
#define NFI_CS_NUM 1
#define NAND_ECC_HW	1


#define NFI_BASE 0xBFBE0000
#define NFIECC_BASE 0xBFBE1000
/*******************************************************************************
 * NFI Register Definition 
 *******************************************************************************/

#define NFI_CNFG_REG16  	((volatile P_U16)(NFI_BASE+0x0000))
#define NFI_PAGEFMT_REG16   ((volatile P_U16)(NFI_BASE+0x0004))
#define NFI_CON_REG16      	((volatile P_U16)(NFI_BASE+0x0008))
#define NFI_ACCCON_REG32   	((volatile P_U32)(NFI_BASE+0x000C))
#define NFI_INTR_EN_REG16   ((volatile P_U16)(NFI_BASE+0x0010))
#define NFI_INTR_REG16      ((volatile P_U16)(NFI_BASE+0x0014))

#define NFI_CMD_REG16   	((volatile P_U16)(NFI_BASE+0x0020))

#define NFI_ADDRNOB_REG16   ((volatile P_U16)(NFI_BASE+0x0030))
#define NFI_COLADDR_REG32  	((volatile P_U32)(NFI_BASE+0x0034))
#define NFI_ROWADDR_REG32  	((volatile P_U32)(NFI_BASE+0x0038))

#define NFI_STRDATA_REG16   ((volatile P_U16)(NFI_BASE+0x0040))

#define NFI_DATAW_REG32    	((volatile P_U32)(NFI_BASE+0x0050))
#define NFI_DATAR_REG32    	((volatile P_U32)(NFI_BASE+0x0054))
#define NFI_PIO_DIRDY_REG16 ((volatile P_U16)(NFI_BASE+0x0058))

#define NFI_STA_REG32      	((volatile P_U32)(NFI_BASE+0x0060))
#define NFI_FIFOSTA_REG16   ((volatile P_U16)(NFI_BASE+0x0064))
#define NFI_LOCKSTA_REG16   ((volatile P_U16)(NFI_BASE+0x0068))

#define NFI_ADDRCNTR_REG16  ((volatile P_U16)(NFI_BASE+0x0070))

#define NFI_STRADDR_REG32  	((volatile P_U32)(NFI_BASE+0x0080))
#define NFI_BYTELEN_REG16   ((volatile P_U16)(NFI_BASE+0x0084))

#define NFI_CSEL_REG16      ((volatile P_U16)(NFI_BASE+0x0090))
#define NFI_IOCON_REG16     ((volatile P_U16)(NFI_BASE+0x0094))

#define NFI_FDM0L_REG32    	((volatile P_U32)(NFI_BASE+0x00A0))
#define NFI_FDM0M_REG32    	((volatile P_U32)(NFI_BASE+0x00A4))

#define NFI_LOCK_REG16	  	((volatile P_U16)(NFI_BASE+0x0100))
#define NFI_LOCKCON_REG32  	((volatile P_U32)(NFI_BASE+0x0104))
#define NFI_LOCKANOB_REG16  ((volatile P_U16)(NFI_BASE+0x0108))
#define NFI_LOCK00ADD_REG32 ((volatile P_U32)(NFI_BASE+0x0110))
#define NFI_LOCK00FMT_REG32 ((volatile P_U32)(NFI_BASE+0x0114))
#define NFI_LOCK01ADD_REG32 ((volatile P_U32)(NFI_BASE+0x0118))
#define NFI_LOCK01FMT_REG32 ((volatile P_U32)(NFI_BASE+0x011C))
#define NFI_LOCK02ADD_REG32 ((volatile P_U32)(NFI_BASE+0x0120))
#define NFI_LOCK02FMT_REG32 ((volatile P_U32)(NFI_BASE+0x0124))
#define NFI_LOCK03ADD_REG32 ((volatile P_U32)(NFI_BASE+0x0128))
#define NFI_LOCK03FMT_REG32 ((volatile P_U32)(NFI_BASE+0x012C))
#define NFI_LOCK04ADD_REG32 ((volatile P_U32)(NFI_BASE+0x0130))
#define NFI_LOCK04FMT_REG32 ((volatile P_U32)(NFI_BASE+0x0134))
#define NFI_LOCK05ADD_REG32 ((volatile P_U32)(NFI_BASE+0x0138))
#define NFI_LOCK05FMT_REG32 ((volatile P_U32)(NFI_BASE+0x013C))
#define NFI_LOCK06ADD_REG32 ((volatile P_U32)(NFI_BASE+0x0140))
#define NFI_LOCK06FMT_REG32 ((volatile P_U32)(NFI_BASE+0x0144))
#define NFI_LOCK07ADD_REG32 ((volatile P_U32)(NFI_BASE+0x0148))
#define NFI_LOCK07FMT_REG32 ((volatile P_U32)(NFI_BASE+0x014C))
#define NFI_LOCK08ADD_REG32 ((volatile P_U32)(NFI_BASE+0x0150))
#define NFI_LOCK08FMT_REG32 ((volatile P_U32)(NFI_BASE+0x0154))
#define NFI_LOCK09ADD_REG32 ((volatile P_U32)(NFI_BASE+0x0158))
#define NFI_LOCK09FMT_REG32 ((volatile P_U32)(NFI_BASE+0x015C))
#define NFI_LOCK10ADD_REG32 ((volatile P_U32)(NFI_BASE+0x0160))
#define NFI_LOCK10FMT_REG32 ((volatile P_U32)(NFI_BASE+0x0164))
#define NFI_LOCK11ADD_REG32 ((volatile P_U32)(NFI_BASE+0x0168))
#define NFI_LOCK11FMT_REG32 ((volatile P_U32)(NFI_BASE+0x016C))
#define NFI_LOCK12ADD_REG32 ((volatile P_U32)(NFI_BASE+0x0170))
#define NFI_LOCK12FMT_REG32 ((volatile P_U32)(NFI_BASE+0x0174))
#define NFI_LOCK13ADD_REG32 ((volatile P_U32)(NFI_BASE+0x0178))
#define NFI_LOCK13FMT_REG32 ((volatile P_U32)(NFI_BASE+0x017C))
#define NFI_LOCK14ADD_REG32 ((volatile P_U32)(NFI_BASE+0x0180))
#define NFI_LOCK14FMT_REG32 ((volatile P_U32)(NFI_BASE+0x0184))
#define NFI_LOCK15ADD_REG32 ((volatile P_U32)(NFI_BASE+0x0188))
#define NFI_LOCK15FMT_REG32 ((volatile P_U32)(NFI_BASE+0x018C))

#define NFI_FIFODATA0_REG32 ((volatile P_U32)(NFI_BASE+0x0190))
#define NFI_FIFODATA1_REG32 ((volatile P_U32)(NFI_BASE+0x0194))
#define NFI_FIFODATA2_REG32 ((volatile P_U32)(NFI_BASE+0x0198))
#define NFI_FIFODATA3_REG32 ((volatile P_U32)(NFI_BASE+0x019C))
#define NFI_MASTERSTA_REG16 ((volatile P_U16)(NFI_BASE+0x0210))


/*******************************************************************************
 * NFI Register Field Definition 
 *******************************************************************************/

/* NFI_CNFG */
#define CNFG_AHB             (0x0001)
#define CNFG_READ_EN         (0x0002)
#define CNFG_DMA_BURST_EN    (0x0004)
#define CNFG_PIO_BIG_ENDIAN  (0x0008)
#define CNFG_BYTE_RW         (0x0040)
#define CNFG_HW_ECC_EN       (0x0100)
#define CNFG_AUTO_FMT_EN     (0x0200)
#define CNFG_OP_IDLE         (0x0000)
#define CNFG_OP_READ         (0x1000)
#define CNFG_OP_SRD          (0x2000)
#define CNFG_OP_PRGM         (0x3000)
#define CNFG_OP_ERASE        (0x4000)
#define CNFG_OP_RESET        (0x5000)
#define CNFG_OP_CUST         (0x6000)
#define CNFG_OP_MODE_MASK    (0x7000)
#define CNFG_OP_MODE_SHIFT   (12)

/* NFI_PAGEFMT */
#define PAGEFMT_512          (0x0000)
#define PAGEFMT_2K           (0x0001)
#define PAGEFMT_4K           (0x0002)

#define PAGEFMT_PAGE_MASK    (0x0003)

#define PAGEFMT_DBYTE_EN     (0x0008)

#define PAGEFMT_SPARE_16     (0x0000)
#define PAGEFMT_SPARE_26     (0x0001)
#define PAGEFMT_SPARE_27     (0x0002)
#define PAGEFMT_SPARE_28     (0x0003)
#define PAGEFMT_SPARE_MASK   (0x0030)
#define PAGEFMT_SPARE_SHIFT  (4)

#define PAGEFMT_FDM_MASK     (0x0F00)
#define PAGEFMT_FDM_SHIFT    (8)

#define PAGEFMT_FDM_ECC_MASK  (0xF000)
#define PAGEFMT_FDM_ECC_SHIFT (12)

/* NFI_CON */
#define CON_FIFO_FLUSH       (0x0001)
#define CON_NFI_RST          (0x0002)
#define CON_NFI_SRD          (0x0010)

#define CON_NFI_NOB_MASK     (0x0060)
#define CON_NFI_NOB_SHIFT    (5)

#define CON_NFI_BRD          (0x0100)
#define CON_NFI_BWR          (0x0200)

#define CON_NFI_SEC_MASK     (0xF000)
#define CON_NFI_SEC_SHIFT    (12)

/* NFI_ACCCON */
#define ACCCON_SETTING       ()

/* NFI_INTR_EN */
#define INTR_RD_DONE_EN      (0x0001)
#define INTR_WR_DONE_EN      (0x0002)
#define INTR_RST_DONE_EN     (0x0004)
#define INTR_ERASE_DONE_EN   (0x0008)
#define INTR_BSY_RTN_EN      (0x0010)
#define INTR_ACC_LOCK_EN     (0x0020)
#define INTR_AHB_DONE_EN     (0x0040)
#define INTR_ALL_INTR_DE     (0x0000)
#define INTR_ALL_INTR_EN     (0x007F)

/* NFI_INTR */
#define INTR_RD_DONE         (0x0001)
#define INTR_WR_DONE         (0x0002)
#define INTR_RST_DONE        (0x0004)
#define INTR_ERASE_DONE      (0x0008)
#define INTR_BSY_RTN         (0x0010)
#define INTR_ACC_LOCK        (0x0020)
#define INTR_AHB_DONE        (0x0040)

/* NFI_ADDRNOB */
#define ADDR_COL_NOB_MASK    (0x0003)
#define ADDR_COL_NOB_SHIFT   (0)
#define ADDR_ROW_NOB_MASK    (0x0030)
#define ADDR_ROW_NOB_SHIFT   (4)

/* NFI_STA */
#define STA_READ_EMPTY       (0x00001000)
#define STA_ACC_LOCK         (0x00000010)
#define STA_CMD_STATE        (0x00000001)
#define STA_ADDR_STATE       (0x00000002)
#define STA_DATAR_STATE      (0x00000004)
#define STA_DATAW_STATE      (0x00000008)

#define STA_NAND_FSM_MASK    (0x1F000000)
#define STA_NAND_BUSY        (0x00000100)
#define STA_NAND_BUSY_RETURN (0x00000200)
#define STA_NFI_FSM_MASK     (0x000F0000)
#define STA_NFI_OP_MASK      (0x0000000F)

/* NFI_FIFOSTA */
#define FIFO_RD_EMPTY        (0x0040)
#define FIFO_RD_FULL         (0x0080)
#define FIFO_WR_FULL         (0x8000)
#define FIFO_WR_EMPTY        (0x4000)
#define FIFO_RD_REMAIN(x)    (0x1F&(x))
#define FIFO_WR_REMAIN(x)    ((0x1F00&(x))>>8)

/* NFI_ADDRCNTR */
#define ADDRCNTR_CNTR(x)     ((0xF000&(x))>>12)
#define ADDRCNTR_OFFSET(x)   (0x03FF&(x))

/* NFI_LOCK */
#define NFI_LOCK_ON          (0x0001)

/* NFI_LOCKANOB */
#define PROG_RADD_NOB_MASK   (0x7000)
#define PROG_RADD_NOB_SHIFT  (12)
#define PROG_CADD_NOB_MASK   (0x0300)
#define PROG_CADD_NOB_SHIFT  (8)
#define ERASE_RADD_NOB_MASK   (0x0070)
#define ERASE_RADD_NOB_SHIFT  (4)
#define ERASE_CADD_NOB_MASK   (0x0007)
#define ERASE_CADD_NOB_SHIFT  (0)

/*******************************************************************************
 * ECC Register Definition 
 *******************************************************************************/

#define ECC_ENCCON_REG16	((volatile P_U16)(NFIECC_BASE+0x0000))
#define ECC_ENCCNFG_REG32  	((volatile P_U32)(NFIECC_BASE+0x0004))
#define ECC_ENCDIADDR_REG32	((volatile P_U32)(NFIECC_BASE+0x0008))
#define ECC_ENCIDLE_REG32  	((volatile P_U32)(NFIECC_BASE+0x000C))
#define ECC_ENCPAR0_REG32   ((volatile P_U32)(NFIECC_BASE+0x0010))
#define ECC_ENCPAR1_REG32   ((volatile P_U32)(NFIECC_BASE+0x0014))
#define ECC_ENCPAR2_REG32   ((volatile P_U32)(NFIECC_BASE+0x0018))
#define ECC_ENCPAR3_REG32   ((volatile P_U32)(NFIECC_BASE+0x001C))
#define ECC_ENCPAR4_REG32   ((volatile P_U32)(NFIECC_BASE+0x0020))
#define ECC_ENCSTA_REG32    ((volatile P_U32)(NFIECC_BASE+0x0024))
#define ECC_ENCIRQEN_REG16  ((volatile P_U16)(NFIECC_BASE+0x0028))
#define ECC_ENCIRQSTA_REG16 ((volatile P_U16)(NFIECC_BASE+0x002C))

#define ECC_DECCON_REG16    ((volatile P_U16)(NFIECC_BASE+0x0100))
#define ECC_DECCNFG_REG32   ((volatile P_U32)(NFIECC_BASE+0x0104))
#define ECC_DECDIADDR_REG32 ((volatile P_U32)(NFIECC_BASE+0x0108))
#define ECC_DECIDLE_REG16   ((volatile P_U16)(NFIECC_BASE+0x010C))
#define ECC_DECFER_REG16    ((volatile P_U16)(NFIECC_BASE+0x0110))
#define ECC_DECENUM0_REG32   ((volatile P_U32)(NFIECC_BASE+0x0114))
#define ECC_DECENUM1_REG32   ((volatile P_U32)(NFIECC_BASE+0x0118))
#define ECC_DECDONE_REG16   ((volatile P_U16)(NFIECC_BASE+0x011C))
#define ECC_DECEL0_REG32    ((volatile P_U32)(NFIECC_BASE+0x0120))
#define ECC_DECEL1_REG32    ((volatile P_U32)(NFIECC_BASE+0x0124))
#define ECC_DECEL2_REG32    ((volatile P_U32)(NFIECC_BASE+0x0128))
#define ECC_DECEL3_REG32    ((volatile P_U32)(NFIECC_BASE+0x012C))
#define ECC_DECEL4_REG32    ((volatile P_U32)(NFIECC_BASE+0x0130))
#define ECC_DECEL5_REG32    ((volatile P_U32)(NFIECC_BASE+0x0134))
#define ECC_DECEL6_REG32    ((volatile P_U32)(NFIECC_BASE+0x0138))
#define ECC_DECEL7_REG32    ((volatile P_U32)(NFIECC_BASE+0x013C))
#define ECC_DECIRQEN_REG16  ((volatile P_U16)(NFIECC_BASE+0x0140))
#define ECC_DECIRQSTA_REG16 ((volatile P_U16)(NFIECC_BASE+0x0144))
#define ECC_FDMADDR_REG32   ((volatile P_U32)(NFIECC_BASE+0x0148)) 
#define ECC_DECFSM_REG32    ((volatile P_U32)(NFIECC_BASE+0x014C))
#define ECC_SYNSTA_REG32    ((volatile P_U32)(NFIECC_BASE+0x0150))
#define ECC_DECNFIDI_REG32  ((volatile P_U32)(NFIECC_BASE+0x0154))
#define ECC_SYN0_REG32      ((volatile P_U32)(NFIECC_BASE+0x0158))

/*******************************************************************************
 * ECC register definition
 *******************************************************************************/
/* ECC_ENCON */
#define ENC_EN             		(0x0001)
#define ENC_DE                 	(0x0000)

/* ECC_ENCCNFG */
#define ECC_CNFG_ECC4          	(0x0000)
#define ECC_CNFG_ECC6          	(0x0001)
#define ECC_CNFG_ECC8          	(0x0002)
#define ECC_CNFG_ECC10         	(0x0003)
#define ECC_CNFG_ECC12         	(0x0004)
#define ECC_CNFG_ECC_MASK      	(0x00000007)

#define ENC_CNFG_NFI           	(0x0010)
#define ENC_CNFG_MODE_MASK     	(0x0010)

#define ENC_CNFG_META6         	(0x10300000)
#define ENC_CNFG_META8         	(0x10400000)

#define ENC_CNFG_MSG_MASK  		(0x1FFF0000)
#define ENC_CNFG_MSG_SHIFT 		(0x10)

/* ECC_ENCIDLE */
#define ENC_IDLE           		(0x0001)

/* ECC_ENCSTA */
#define STA_FSM            		(0x001F)
#define STA_COUNT_PS       		(0xFF10)
#define STA_COUNT_MS       		(0x3FFF0000)

/* ECC_ENCIRQEN */
#define ENC_IRQEN          		(0x0001)

/* ECC_ENCIRQSTA */
#define ENC_IRQSTA         		(0x0001)

/* ECC_DECCON */
#define DEC_EN             		(0x0001)
#define DEC_DE             		(0x0000)

/* ECC_ENCCNFG */
#define DEC_CNFG_ECC4          (0x0000)
//#define DEC_CNFG_ECC6          (0x0001)
//#define DEC_CNFG_ECC12         (0x0002)
#define DEC_CNFG_NFI           (0x0010)
//#define DEC_CNFG_META6         (0x10300000)
//#define DEC_CNFG_META8         (0x10400000)

#define DEC_CNFG_BURST_EN           (0x00100)
#define DEC_CNFG_FER           (0x01000)
#define DEC_CNFG_EL            (0x02000)
#define DEC_CNFG_CORRECT       (0x03000)
#define DEC_CNFG_TYPE_MASK     (0x03000)

#define DEC_CNFG_EMPTY_EN      (0x80000000)

#define DEC_CNFG_CODE_MASK     (0x1FFF0000)
#define DEC_CNFG_CODE_SHIFT    (0x10)

/* ECC_DECIDLE */
#define DEC_IDLE           		(0x0001)


/*
 * Standard NAND flash commands
 */
#define NAND_CMD_READ0		0
#define NAND_CMD_READ1		1
#define NAND_CMD_RNDOUT		5
#define NAND_CMD_PAGEPROG	0x10
#define NAND_CMD_READOOB	0x50
#define NAND_CMD_ERASE1		0x60
#define NAND_CMD_STATUS		0x70
#define NAND_CMD_STATUS_MULTI	0x71
#define NAND_CMD_SEQIN		0x80
#define NAND_CMD_RNDIN		0x85
#define NAND_CMD_READID		0x90
#define NAND_CMD_ERASE2		0xd0
#define NAND_CMD_RESET		0xff

/* Extended commands for large page devices */
#define NAND_CMD_READSTART	0x30
#define NAND_CMD_RNDOUTSTART	0xE0
#define NAND_CMD_CACHEDPROG	0x15

/* Status bits */
#define NAND_STATUS_FAIL		0x01
#define NAND_STATUS_FAIL_N1		0x02
#define NAND_STATUS_TRUE_READY	0x20
#define NAND_STATUS_READY		0x40
#define NAND_STATUS_WP			0x80

#define KB_SHIFT 10
#define MB_SHIFT 20

#define K0_TO_K1(x)			(((uint32)x) | 0xa0000000)
#define K1_TO_PHY(x)		(((uint32)x) & 0x1fffffff)


typedef struct
{
   unsigned short id;          //deviceid+menuid
   unsigned int ext_id; 
   unsigned char  addr_cycle;
   unsigned char  iowidth;
   unsigned short totalsize;   
   unsigned short blocksize;
   unsigned short pagesize;
   unsigned int timmingsetting;
   char devciename[14];
   unsigned int advancedmode;   //
}flashdev_info,*pflashdev_info;

/* NAND driver */
struct mt6573_nand_host_hw {
    unsigned int nfi_bus_width;		    /* NFI_BUS_WIDTH */ 
	unsigned int nfi_access_timing;		/* NFI_ACCESS_TIMING */  
	unsigned int nfi_cs_num;			/* NFI_CS_NUM */
	unsigned int nand_sec_size;			/* NAND_SECTOR_SIZE */
	unsigned int nand_sec_shift;		/* NAND_SECTOR_SHIFT */
	unsigned int nand_ecc_size;
	unsigned int nand_ecc_bytes;
	unsigned int nand_ecc_mode;
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


struct ra_nand_chip {

	struct nand_info *flash;
};


static const flashdev_info gen_FlashTable[]={
	//st
	{0x2075, 0x207520 ,4,  8, 32,	16,  512,  0x44333,  "NAND256W3A", 0}, //32M,512 Page size
	//micro
	{0x2CDA,  0x909504, 5,  8,  256,	128,  2048,  0x44333,  "MT29F2G08AAB",	0}, //256M, 2K Page Size
	//{0x2CDC,  0x909504, 5,  8,  512,	128,  2048,  0x44333,  "MT29F4G08AAC",	0}, //512M, 2K Page Size
	/*new add*/
    {0xC8DA,  0x909544, 5,  8,  256,    128,  2048,  0x44333,  "F59L2G81A",     0},
	{0x2CDA,  0x909506, 5,  8, 256,	128,  2048,  0x44333,  "MT29F2G08ABAEA",	0},  //256M, 2K Page Size
	//samsung 
	{0xECDC,  0x109554, 5,  8, 512,	128,  2048,  0x44333,  "K9F4G08U0D",	0},//512M, 2K Page Size
	//toshiba	
	{0x98D1,  0x901576, 4,  8, 128,	128,  2048,  0x44333,  "TC58NVG0S3E",	0}, //128M
	{0x98D3,  0x902676, 5,  8, 1024,	256,  4096,  0x44333,  "TC58NVG3S0F",	0}, //1024M
	/*new add */
	{0x98F1,  0x8015F2, 4,  8, 128,	128,  2048,  0x44333,  "TC58BVG0S3HTA00-1G",	0}, //128M
	{0x98F1,  0x801572, 4,  8, 128,	128,  2048,  0x44333,  "TC58NVG0S3HTA00",	0}, //128M		
	{0x98DA,  0x9015F6, 5,  8, 256,	128,  2048,  0x44333,  "TC58BVG1S3HTA00-2G",	0}, //256M
	{0x98D3,  0x912676, 5,  8, 1024, 256,  4096,  0x44333, "TH58NVG3S0HTA00",	     0}, //1024M,4K page
	{0x98D3,  0x9126F6, 5,  8, 1024, 256,  4096,  0x44333, "TH58BVG3S0HTA00",	     0}, //1024M,4K page
	
	//Winbond
	{0xeff1,  0x809500, 4,  8, 128,	128,  2048,  0x44333,  "W29N01GV",	0}, //128M
	/*new add */
	{0xEFF1,  0x009500, 4,  8, 128,	128,  2048,  0x44333,  "W29N01HVSINA",	0}, //128M
	{0xEFDA,  0x909504, 5,  8, 256,	128,  2048,  0x44333,  "W29N02GVSIAA",	0}, //256M
	//MXIC
	{0xc2f1,  0x801dc2, 4,  8, 128,	128,  2048,  0x44333,  "MX30LF1G08",	0}, //128M
	{0xc2f1,  0x809502, 4,  8, 128,	128,  2048,  0x44333,  "MX30LF1G18AC",	0}, //128M
	/*new add */
	{0xC2DA,  0x909506, 5,  8, 256,	128,  2048,  0x44333,  "MX30LF2G18AC",	0}, //256M	
	//ESMT
	{0x92f1,  0x809540, 4,  8, 128,	128,  2048,  0x44333,  "F59L1G81A",	0}, //128M
		/*new add */
	{0xC8D1,  0x809542, 4,  8, 128,	128,  2048,  0x44333,  "F59L1G81LA",	0}, //128M
	{0xC8D1,  0x809540, 4,  8, 128,	128,  2048,  0x44333,  "F59L1G81MA",	0}, //128M	
	{0x0000,  0,  0,  0,	0,	  0,	0,  0,    "xxxxxxxxxxxxx", 0},
};

#endif
