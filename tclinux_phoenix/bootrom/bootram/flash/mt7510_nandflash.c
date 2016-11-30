//#define CONFIG_BADBLOCK_CHECK 

#include <linux/string.h>
#include <linux/stddef.h>
#include <linux/compiler.h>
#include <asm/io.h>
#include <asm/system.h>
//#include <linux/dma-mapping.h>
//#include <linux/miscdevice.h>

//#include <asm/scatterlist.h>

#include <mt7510_nandflash.h>
#include <asm/tc3162.h>
#if	defined(TCSUPPORT_NAND_BMT) && !defined(LZMA_IMG)
#include "bmt.h"
#endif

#if defined (TCSUPPORT_GPON_DUAL_IMAGE) || defined (TCSUPPORT_EPON_DUAL_IMAGE)
#include "flash_layout/tc_partition.h"
#endif

#ifdef LZMA_IMG

#define printf(args...) 		do{}while(0)
#else
#define printf prom_printf
#endif

//#define CONFIG_BADBLOCK_CHECK
//#define PIO_MODE_SUPPORT
//#define NAND_DEBUG
//#define NAND_BMT
#ifdef LZMA_IMG
#undef TCSUPPORT_NAND_BMT
#endif

#if	defined(TCSUPPORT_NAND_BMT)
#define CONFIG_BADBLOCK_CHECK
#define NAND_BMT

#define BMT_BAD_BLOCK_INDEX_OFFSET (1)
#define POOL_GOOD_BLOCK_PERCENT 8/100
#define SLAVE_IMAGE_OFFSET 0xf00000
static int bmt_pool_size = 0;
static bmt_struct *g_bmt = NULL;
static init_bbt_struct *g_bbt = NULL;
extern int nand_logic_size;
extern int nand_flash_avalable_size;
#endif

u32 reservearea_size = 0;
struct ra_nand_chip ra;
int en_oob_write = 0;
int en_oob_erase = 0;

#define IMAGE_OOB_SIZE 64	/* fix 64 oob buffer size padding after page buffer, no hw ecc info */
#define PAGE_OOB_SIZE 64	/* 64 bytes for 2K page, 128 bytes for 4k page */

static u8 *local_buffer_16_align;       // 16 byte aligned buffer, for HW issue
static u8 local_buffer[4096+16];
static u8 fdm_buf[PAGE_OOB_SIZE];

uint8 g_bHwEcc=1;
uint8 g_bUseAHBMode=1;
uint8 g_bAutoFMT=1;
bool g_bInitDone;
static bool g_bcmdstatus;
static u32 g_value = 0;

flashdev_info devinfo;

//static bmt_struct *g_bmt;


/* MT6573 NAND Driver */
struct mt6573_nand_host_hw mt6573_nand_hw = {
	.nfi_bus_width          = 8,
	.nfi_access_timing		= NFI_DEFAULT_ACCESS_TIMING,
	.nfi_cs_num				= NFI_CS_NUM,
	.nand_sec_size			= 512,
	.nand_sec_shift			= 9,
	.nand_ecc_size			= 2048,
	.nand_ecc_bytes			= 32,
	.nand_ecc_mode			= NAND_ECC_HW,
};

#define NFI_SET_REG32(reg, value) \
do {	\
	g_value = (DRV_Reg32(reg) | (value));\
	DRV_WriteReg32(reg, g_value); \
} while(0)

#define NFI_SET_REG16(reg, value) \
do {	\
	g_value = (DRV_Reg16(reg) | (value));\
	DRV_WriteReg16(reg, g_value); \
} while(0)

#define NFI_CLN_REG32(reg, value) \
do {	\
	g_value = (DRV_Reg32(reg) & (~(value)));\
	DRV_WriteReg32(reg, g_value); \
} while(0)

#define NFI_CLN_REG16(reg, value) \
do {	\
	g_value = (DRV_Reg16(reg) & (~(value)));\
	DRV_WriteReg16(reg, g_value); \
} while(0)

#define PIO_BIG_ENDIAN (DRV_Reg16(NFI_CNFG_REG16) & CNFG_PIO_BIG_ENDIAN)
#define NAND_SECTOR_SIZE (512)
#define OOB_PER_SECTOR      (16)
#define OOB_AVAI_PER_SECTOR (8)

#define TIMEOUT_1   0x1fff
#define TIMEOUT_2   0x8ff
#define TIMEOUT_3   0xffff
#define TIMEOUT_4   5000   //PIO

#define NFI_ISSUE_COMMAND(cmd, col_addr, row_addr, col_num, row_num) \
   do { \
      DRV_WriteReg16(NFI_CMD_REG16,cmd);\
      while (DRV_Reg32(NFI_STA_REG32) & STA_CMD_STATE);\
      DRV_WriteReg32(NFI_COLADDR_REG32, col_addr);\
      DRV_WriteReg32(NFI_ROWADDR_REG32, row_addr);\
      DRV_WriteReg16(NFI_ADDRNOB_REG16, col_num | (row_num<<ADDR_ROW_NOB_SHIFT));\
      while (DRV_Reg32(NFI_STA_REG32) & STA_ADDR_STATE);\
   }while(0);



static int mt6573_nand_wait();
static int mt6573_nand_dev_ready();

extern void flush_dcache_range(unsigned long start, unsigned long end);
#if defined(TCSUPPORT_DUAL_IMAGE_ENHANCE) && !defined(LZMA_IMG)
extern int offset;
#endif

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

#ifdef NAND_DEBUG
void dump_nfi(void)
{

    printf( "NFI_ACCCON: 0x%x\n",  DRV_Reg32(NFI_ACCCON_REG32));
    printf( "NFI_PAGEFMT: 0x%x\n", DRV_Reg16(NFI_PAGEFMT_REG16));
    printf( "NFI_CNFG: 0x%x\n", DRV_Reg16(NFI_CNFG_REG16));
    printf( "NFI_CON: 0x%x\n", DRV_Reg16(NFI_CON_REG16));
    printf( "NFI_STRDATA: 0x%x\n", DRV_Reg16(NFI_STRDATA_REG16));
    printf( "NFI_ADDRCNTR: 0x%x\n", DRV_Reg16(NFI_ADDRCNTR_REG16));
    printf( "NFI_FIFOSTA: 0x%x\n", DRV_Reg16(NFI_FIFOSTA_REG16));
    printf( "NFI_ADDRNOB: 0x%x\n", DRV_Reg16(NFI_ADDRNOB_REG16));
    printf( "NFI_FDM_0L: 0x%x\n", DRV_Reg32(NFI_FDM0L_REG32));
    printf( "NFI_STA: 0x%x\n", DRV_Reg32(NFI_STA_REG32));
    printf( "NFI_FDM_0M: 0x%x\n", DRV_Reg32(NFI_FDM0M_REG32));
    printf( "NFI_IOCON: 0x%x\n", DRV_Reg16(NFI_IOCON_REG16));
    printf( "NFI_BYTELEN: 0x%x\n", DRV_Reg16(NFI_BYTELEN_REG16));
    printf( "NFI_COLADDR: 0x%x\n", DRV_Reg32(NFI_COLADDR_REG32));
    printf( "NFI_ROWADDR: 0x%x\n", DRV_Reg32(NFI_ROWADDR_REG32));
    printf( "ECC_ENCCNFG: 0x%x\n", DRV_Reg32(ECC_ENCCNFG_REG32));
    printf( "ECC_ENCCON: 0x%x\n", DRV_Reg16(ECC_ENCCON_REG16));
    printf( "ECC_DECCNFG: 0x%x\n", DRV_Reg32(ECC_DECCNFG_REG32));
    printf( "ECC_DECCON: 0x%x\n", DRV_Reg16(ECC_DECCON_REG16));
    printf( "NFI_CSEL: 0x%x\n", DRV_Reg16(NFI_CSEL_REG16));
	//ECC

	printf( "NFI_STRADDR: 0x%x\n", DRV_Reg32(NFI_STRADDR_REG32));
#if 0
	printf( "ECC_DECDIADDR: 0x%x\n", DRV_Reg32(ECC_DECDIADDR_REG32));
	printf( "ECC_FDMADDR_REG32: 0x%x\n", DRV_Reg32(ECC_FDMADDR_REG32));
	printf( "ECC_DECFSM_REG32: 0x%x\n", DRV_Reg32(ECC_DECFSM_REG32));
	printf( "ECC_SYNSTA_REG32: 0x%x\n", DRV_Reg32(ECC_SYNSTA_REG32));
	printf( "ECC_DECNFIDI_REG32: 0x%x\n", DRV_Reg32(ECC_DECNFIDI_REG32));
	printf( "ECC_SYN0_REG32: 0x%x\n", DRV_Reg32(ECC_SYN0_REG32));
    // printf( "NFI clock register: 0x%x: %s\n", DRV_Reg32((volatile u32 *)0x00000000),
    //         (DRV_Reg32((volatile u32 *)0xF0039300) & (1 << 17)) ? "miss" : "OK");
#endif
}
#endif

bool get_device_info(u16 id, u32 ext_id, flashdev_info *pdevinfo)
{
	u32 index;
	for(index=0;gen_FlashTable[index].id!=0;index++)
	{
	    //if(id==gen_FlashTable[index].id && ext_id == gen_FlashTable[index].ext_id)
	    if (id == gen_FlashTable[index].id) 
        {
        	ext_id = ((gen_FlashTable[index].ext_id&0xFF)==0xFF)?(ext_id|0xFF) : ext_id;
					if(ext_id == gen_FlashTable[index].ext_id){
	    	pdevinfo->id = gen_FlashTable[index].id;
	    	pdevinfo->ext_id = gen_FlashTable[index].ext_id;
			pdevinfo->blocksize = gen_FlashTable[index].blocksize;
			pdevinfo->addr_cycle = gen_FlashTable[index].addr_cycle;
			pdevinfo->iowidth = gen_FlashTable[index].iowidth;
			pdevinfo->timmingsetting = gen_FlashTable[index].timmingsetting;
			pdevinfo->advancedmode = gen_FlashTable[index].advancedmode;
			pdevinfo->pagesize = gen_FlashTable[index].pagesize;
	        pdevinfo->totalsize = gen_FlashTable[index].totalsize;
			memcpy(pdevinfo->devciename,gen_FlashTable[index].devciename,sizeof(pdevinfo->devciename));
            //printf( "Device found in MTK table, ID: %x\n", id);
	
	        goto find;		
		}
	}
	}

find:
	if(0==pdevinfo->id)
	{
    	printf( "Device not found, ID: %x\n", id);
    	#ifdef TCSUPPORT_XPON_HAL_API_EXT
			printf( "And Use fault 128M nandflash !\n");
			pdevinfo->blocksize = 128;
			pdevinfo->addr_cycle = 4 ;
			pdevinfo->iowidth = 8 ;
			pdevinfo->timmingsetting = 0x44333;
			pdevinfo->advancedmode = 0 ;
			pdevinfo->pagesize = 2048 ;
			pdevinfo->totalsize = 128 ;
			memcpy(pdevinfo->devciename,"unkonwn flash chip ",sizeof(pdevinfo->devciename));
			return true;
		#endif
		
	  return false;
	}
	else
	{
		return true;
	}
}


/******************************************************************************
 * ECC_Config
 * 
 * DESCRIPTION:
 *   Configure HW ECC!
 * 
 * PARAMETERS: 
 *   struct mt6573_nand_host_hw *hw
 * 
 * RETURNS: 
 *   None  
 * 
 * NOTES: 
 *   None
 * 
 ******************************************************************************/
static void ECC_Config(struct mt6573_nand_host_hw *hw)
{
	u32 u4ENCODESize;
	u32 u4DECODESize;

	DRV_WriteReg16(ECC_DECCON_REG16, DEC_DE);
	do{;}while (!DRV_Reg16(ECC_DECIDLE_REG16));

	DRV_WriteReg16(ECC_ENCCON_REG16, ENC_DE);
	do{;}while (!DRV_Reg32(ECC_ENCIDLE_REG32));

	/* setup FDM register base */
	DRV_WriteReg32(ECC_FDMADDR_REG32, NFI_FDM0L_REG32);

	/* Sector + FDM */
	u4ENCODESize = (hw->nand_sec_size + 8) << 3;
	/* Sector + FDM + YAFFS2 meta data bits */
	u4DECODESize = ((hw->nand_sec_size + 8) << 3) + 4 * 13; 

	/* configure ECC decoder && encoder*/
	DRV_WriteReg32(ECC_DECCNFG_REG32,
		ECC_CNFG_ECC4|DEC_CNFG_NFI|DEC_CNFG_EMPTY_EN|DEC_CNFG_BURST_EN |
		(u4DECODESize << DEC_CNFG_CODE_SHIFT));

	DRV_WriteReg32(ECC_ENCCNFG_REG32, 
		ECC_CNFG_ECC4|ENC_CNFG_NFI|
		(u4ENCODESize << ENC_CNFG_MSG_SHIFT));


	if(g_bUseAHBMode)
		NFI_SET_REG32(ECC_DECCNFG_REG32, DEC_CNFG_CORRECT);
	else
		NFI_SET_REG32(ECC_DECCNFG_REG32, DEC_CNFG_EL);

}

/******************************************************************************
 * ECC_Decode_Start
 * 
 * DESCRIPTION:
 *   HW ECC Decode Start !
 * 
 * PARAMETERS: 
 *   None
 * 
 * RETURNS: 
 *   None  
 * 
 * NOTES: 
 *   None
 * 
 ******************************************************************************/
static void ECC_Decode_Start(void)
{
   	/* wait for device returning idle */
	while(!(DRV_Reg16(ECC_DECIDLE_REG16) & DEC_IDLE));
	DRV_WriteReg16(ECC_DECCON_REG16, DEC_EN);
}

/******************************************************************************
 * ECC_Decode_End
 * 
 * DESCRIPTION:
 *   HW ECC Decode End !
 * 
 * PARAMETERS: 
 *   None
 * 
 * RETURNS: 
 *   None  
 * 
 * NOTES: 
 *   None
 * 
 ******************************************************************************/
static void ECC_Decode_End(void)
{
   /* wait for device returning idle */
	while(!(DRV_Reg16(ECC_DECIDLE_REG16) & DEC_IDLE));
	DRV_WriteReg16(ECC_DECCON_REG16, DEC_DE);
}
#ifndef LZMA_IMG
/******************************************************************************
 * ECC_Encode_Start
 * 
 * DESCRIPTION:
 *   HW ECC Encode Start !
 * 
 * PARAMETERS: 
 *   None
 * 
 * RETURNS: 
 *   None  
 * 
 * NOTES: 
 *   None
 * 
 ******************************************************************************/
static void ECC_Encode_Start(void)
{
   /* wait for device returning idle */
	while(!(DRV_Reg32(ECC_ENCIDLE_REG32) & ENC_IDLE));
	//mb();
	DRV_WriteReg16(ECC_ENCCON_REG16, ENC_EN);
}

/******************************************************************************
 * ECC_Encode_End
 * 
 * DESCRIPTION:
 *   HW ECC Encode End !
 * 
 * PARAMETERS: 
 *   None
 * 
 * RETURNS: 
 *   None  
 * 
 * NOTES: 
 *   None
 * 
 ******************************************************************************/
static void ECC_Encode_End(void)
{
   /* wait for device returning idle */
	while(!(DRV_Reg32(ECC_ENCIDLE_REG32) & ENC_IDLE));
	//mb();
	DRV_WriteReg16(ECC_ENCCON_REG16, ENC_DE);
}
#endif
/******************************************************************************
 * mt6573_nand_check_bch_error
 * 
 * DESCRIPTION:
 *   Check BCH error or not !
 * 
 * PARAMETERS: 
 *   struct mtd_info *mtd
 *	 u8* pDataBuf
 *	 u32 u4SecIndex
 *	 u32 u4PageAddr
 * 
 * RETURNS: 
 *   None  
 * 
 * NOTES: 
 *   None
 * 
 ******************************************************************************/
static bool mt6573_nand_check_bch_error(
	 u8* pDataBuf, u32 u4SecIndex, u32 u4PageAddr)
{
	bool bRet = true;
	u16 u2SectorDoneMask = 1 << u4SecIndex;
	u32 u4ErrorNumDebug, i, u4ErrNum, u4ErrorNumDebug1;
	u32 timeout = 0xFFFF;
    u32 correct_count = 0;
	// int el;
//#if !USE_AHB_MODE
	#if 0
	u32 au4ErrBitLoc[6];
	u32 u4ErrByteLoc, u4BitOffset;
	u32 u4ErrBitLoc1th, u4ErrBitLoc2nd;
	#endif
//#endif

	//4 // Wait for Decode Done
	while (0 == (u2SectorDoneMask & DRV_Reg16(ECC_DECDONE_REG16)))
	{       
		timeout--;
		
		if (0 == timeout)
		{
			printf("bch_error return timeout \n");
			//dump_nfi();
			return false;
		}
	}
//#if (USE_AHB_MODE)
	if(g_bUseAHBMode){
		u4ErrorNumDebug = DRV_Reg32(ECC_DECENUM0_REG32);
		u4ErrorNumDebug1 = DRV_Reg32(ECC_DECENUM1_REG32);
		if ((0 != (u4ErrorNumDebug & 0xFFFFF)) || (0 != (u4ErrorNumDebug1 & 0xFFFFF)))
		{
			for (i = 0; i <= u4SecIndex; ++i)
			{
	        	if(i<4){
					u4ErrNum = DRV_Reg32(ECC_DECENUM0_REG32) >> (i*5);
	        	}
				else{
					u4ErrNum = DRV_Reg32(ECC_DECENUM1_REG32) >> ((i-4)*5);
				}
				u4ErrNum &= 0x1F;
				correct_count += u4ErrNum;
			    
				if (0x1F == u4ErrNum)
				{
					
					bRet = false;
					printf("UnCorrectable at PageAddr=%d, Sector=%d\n", u4PageAddr, i);
				} 
				#ifdef NAND_DEBUG
				else 
				{
				    if (u4ErrNum)
				    {
					    printf("Correct %d at PageAddr=%d, Sector=%d\n", u4ErrNum, u4PageAddr, i);
				    }
				}
				#endif
			}
			
			//printf("bit error is %u\n", correct_count);
			
		}
	}
	

	//if(bRet == false)
	//	printf("mt6573_nand_check_bch_error return false \n");
	
	return bRet;
}

/******************************************************************************
 * mt6573_nand_RFIFOValidSize
 * 
 * DESCRIPTION:
 *   Check the Read FIFO data bytes !
 * 
 * PARAMETERS: 
 *   u16 u2Size
 * 
 * RETURNS: 
 *   None   
 * 
 * NOTES: 
 *   None
 *
 ******************************************************************************/
static bool mt6573_nand_RFIFOValidSize(u16 u2Size)
{
	u32 timeout = 0xFFFF;
	while (FIFO_RD_REMAIN(DRV_Reg16(NFI_FIFOSTA_REG16)) < u2Size)
	{
		timeout--;
		if (0 == timeout){
			return false;
		}
	}
	return true;
}

/******************************************************************************
 * mt6573_nand_WFIFOValidSize
 * 
 * DESCRIPTION:
 *   Check the Write FIFO data bytes !
 * 
 * PARAMETERS: 
 *   u16 u2Size
 * 
 * RETURNS: 
 *   None   
 * 
 * NOTES: 
 *   None
 *
 ******************************************************************************/
static bool mt6573_nand_WFIFOValidSize(u16 u2Size)
{
	u32 timeout = 0xFFFF;
	while (FIFO_WR_REMAIN(DRV_Reg16(NFI_FIFOSTA_REG16)) > u2Size)
    {
		timeout--;
		if (0 == timeout)
        {
			return false;
		}
	}
	return true;
}

/******************************************************************************
 * mt6573_nand_status_ready
 * 
 * DESCRIPTION:
 *   Indicate the NAND device is ready or not ! 
 * 
 * PARAMETERS: 
 *   u32 u4Status
 * 
 * RETURNS: 
 *   None   
 * 
 * NOTES: 
 *   None
 *
 ******************************************************************************/
static bool mt6573_nand_status_ready(u32 u4Status)
{
	u32 timeout = 0xFFFF;
	while ((DRV_Reg32(NFI_STA_REG32) & u4Status) != 0)
    {
		timeout--;
		if (0 == timeout)
        {
			return false;
		}
	}
	return true;
}

/******************************************************************************
 * mt6573_nand_reset
 * 
 * DESCRIPTION:
 *   Reset the NAND device hardware component ! 
 * 
 * PARAMETERS: 
 *   struct mt6573_nand_host *host (Initial setting data)
 * 
 * RETURNS: 
 *   None   
 * 
 * NOTES: 
 *   None
 *
 ******************************************************************************/
static bool mt6573_nand_reset(void)
{
	// HW recommended reset flow
	int timeout = 0xFFFF;
	if (DRV_Reg16(NFI_MASTERSTA_REG16)) // master is busy
	{
		//	mb();
	    DRV_WriteReg16(NFI_CON_REG16, CON_FIFO_FLUSH | CON_NFI_RST);
	    while (DRV_Reg16(NFI_MASTERSTA_REG16))
	    {
	        timeout--;
	        if (!timeout)
	        {
	            printf( "Wait for NFI_MASTERSTA timeout\n");
	        }
	    }
	}
	/* issue reset operation */
	//mb();
	DRV_WriteReg16(NFI_CON_REG16, CON_FIFO_FLUSH | CON_NFI_RST);

	return mt6573_nand_status_ready(STA_NFI_FSM_MASK|STA_NAND_BUSY) &&
		   mt6573_nand_RFIFOValidSize(0) &&
		   mt6573_nand_WFIFOValidSize(0);
}

/******************************************************************************
 * mt6573_nand_set_mode
 * 
 * DESCRIPTION:
 *    Set the oepration mode ! 
 * 
 * PARAMETERS: 
 *   u16 u2OpMode (read/write) 
 * 
 * RETURNS: 
 *   None   
 * 
 * NOTES: 
 *   None
 *
 ******************************************************************************/
static void mt6573_nand_set_mode(u16 u2OpMode)
{
	u16 u2Mode = DRV_Reg16(NFI_CNFG_REG16);
	u2Mode &= ~CNFG_OP_MODE_MASK;
	u2Mode |= u2OpMode;
	DRV_WriteReg16(NFI_CNFG_REG16, u2Mode);
}


/******************************************************************************
 * mt6573_nand_set_autoformat
 * 
 * DESCRIPTION:
 *    Enable/Disable hardware autoformat ! 
 * 
 * PARAMETERS: 
 *   bool bEnable (Enable/Disable)
 * 
 * RETURNS: 
 *   None   
 * 
 * NOTES: 
 *   None
 *
 ******************************************************************************/
static void mt6573_nand_set_autoformat(bool bEnable)
{
	if (g_bAutoFMT && bEnable)
	{
		NFI_SET_REG16(NFI_CNFG_REG16, CNFG_AUTO_FMT_EN);
	}
	else
	{
		NFI_CLN_REG16(NFI_CNFG_REG16, CNFG_AUTO_FMT_EN);
	}
}

/******************************************************************************
 * mt6573_nand_configure_fdm
 * 
 * DESCRIPTION:
 *   Configure the FDM data size ! 
 * 
 * PARAMETERS: 
 *   u16 u2FDMSize
 * 
 * RETURNS: 
 *   None   
 * 
 * NOTES: 
 *   None
 *
 ******************************************************************************/
static void mt6573_nand_configure_fdm(u16 u2FDMSize)
{
	NFI_CLN_REG16(NFI_PAGEFMT_REG16, PAGEFMT_FDM_MASK | PAGEFMT_FDM_ECC_MASK);
	NFI_SET_REG16(NFI_PAGEFMT_REG16, u2FDMSize << PAGEFMT_FDM_SHIFT);
	NFI_SET_REG16(NFI_PAGEFMT_REG16, u2FDMSize << PAGEFMT_FDM_ECC_SHIFT);
}

/******************************************************************************
 * mt6573_nand_configure_lock
 * 
 * DESCRIPTION:
 *   Configure the NAND lock ! 
 * 
 * PARAMETERS: 
 *   u16 u2FDMSize
 * 
 * RETURNS: 
 *   None   
 * 
 * NOTES: 
 *   None
 *
 ******************************************************************************/
static void mt6573_nand_configure_lock(void)
{
	u32 u4WriteColNOB = 2;
	u32 u4WriteRowNOB = 3;
	u32 u4EraseColNOB = 0;
	u32 u4EraseRowNOB = 3;
	DRV_WriteReg16(NFI_LOCKANOB_REG16, 
		(u4WriteColNOB << PROG_CADD_NOB_SHIFT)  |
		(u4WriteRowNOB << PROG_RADD_NOB_SHIFT)  |
		(u4EraseColNOB << ERASE_CADD_NOB_SHIFT) |
		(u4EraseRowNOB << ERASE_RADD_NOB_SHIFT));

	#if 0
	if (CHIPVER_ECO_1 == g_u4ChipVer)
    {
		int i;
		for (i = 0; i < 16; ++i)
        {
			DRV_WriteReg32(NFI_LOCK00ADD_REG32 + (i << 1), 0xFFFFFFFF);
			DRV_WriteReg32(NFI_LOCK00FMT_REG32 + (i << 1), 0xFFFFFFFF);
		}
		//DRV_WriteReg16(NFI_LOCKANOB_REG16, 0x0);
		DRV_WriteReg32(NFI_LOCKCON_REG32, 0xFFFFFFFF);
		DRV_WriteReg16(NFI_LOCK_REG16, NFI_LOCK_ON);
	}
	#endif
}

static bool mt6573_nand_pio_ready(void)
{
    int count = 0;
    while ( !(DRV_Reg16(NFI_PIO_DIRDY_REG16) & 1) )
    {
        count++;
        if (count > 0xffff)
        {
            printf("PIO_DIRDY timeout\n");
            return false;
        }
    }

    return true;
}

/******************************************************************************
 * mt6573_nand_set_command
 * 
 * DESCRIPTION:
 *    Send hardware commands to NAND devices ! 
 * 
 * PARAMETERS: 
 *   u16 command 
 * 
 * RETURNS: 
 *   None   
 * 
 * NOTES: 
 *   None
 *
 ******************************************************************************/
static bool mt6573_nand_set_command(u16 command)
{
	/* Write command to device */
	//mb();
	
	DRV_WriteReg16(NFI_CMD_REG16, command);
	return mt6573_nand_status_ready(STA_CMD_STATE);
}

/******************************************************************************
 * mt6573_nand_set_address
 * 
 * DESCRIPTION:
 *    Set the hardware address register ! 
 * 
 * PARAMETERS: 
 *   struct nand_chip *nand, u32 u4RowAddr 
 * 
 * RETURNS: 
 *   None   
 * 
 * NOTES: 
 *   None
 *
 ******************************************************************************/
static bool mt6573_nand_set_address(u32 u4ColAddr, u32 u4RowAddr, u16 u2ColNOB, u16 u2RowNOB)
{
	u32 coladdr = u4ColAddr, rowaddr = u4RowAddr;
	/* fill cycle addr */
	//mb();
	
	DRV_WriteReg32(NFI_COLADDR_REG32, coladdr);
	DRV_WriteReg32(NFI_ROWADDR_REG32, rowaddr);
	DRV_WriteReg16(NFI_ADDRNOB_REG16, u2ColNOB|(u2RowNOB << ADDR_ROW_NOB_SHIFT));
	return mt6573_nand_status_ready(STA_ADDR_STATE);
}

/******************************************************************************
 * mt6573_nand_check_RW_count
 * 
 * DESCRIPTION:
 *    Check the RW how many sectors ! 
 * 
 * PARAMETERS: 
 *   u16 u2WriteSize 
 * 
 * RETURNS: 
 *   None   
 * 
 * NOTES: 
 *   None
 *
 ******************************************************************************/
static bool mt6573_nand_check_RW_count(u16 u2WriteSize)
{
	u32 timeout = 0xFFFF;
	u16 u2SecNum = u2WriteSize >> 9;
    
	while (ADDRCNTR_CNTR(DRV_Reg16(NFI_ADDRCNTR_REG16)) < u2SecNum)
	{
		timeout--;
		if (0 == timeout)
	    {
	        printf("[%s] timeout\n", __FUNCTION__);
			return false;
		}
	}
	return true;
}

/******************************************************************************
 * mt6573_nand_ready_for_read
 * 
 * DESCRIPTION:
 *    Prepare hardware environment for read ! 
 * 
 * PARAMETERS: 
 *   struct nand_chip *nand, u32 u4RowAddr 
 * 
 * RETURNS: 
 *   None   
 * 
 * NOTES: 
 *   None
 *
 ******************************************************************************/
static bool mt6573_nand_ready_for_read(struct mt6573_nand_host_hw *hw, u32 u4RowAddr, u32 u4ColAddr, bool full, u8 *buf)
{
	/* Reset NFI HW internal state machine and flush NFI in/out FIFO */	
	bool bRet = false;
	u16 sec_num = hw->nand_ecc_size/hw->nand_sec_size;
	u32 col_addr = u4ColAddr;
	u32 colnob=2, rownob;

	
	if (hw->nfi_bus_width == 16)
	    col_addr /= 2;

	if (!mt6573_nand_reset())
	{
		goto cleanup;
	}
	if(g_bHwEcc){
		/* Enable HW ECC */
		NFI_SET_REG16(NFI_CNFG_REG16, CNFG_HW_ECC_EN);
	}else{
		NFI_CLN_REG16(NFI_CNFG_REG16, CNFG_HW_ECC_EN);
	}

	mt6573_nand_set_mode(CNFG_OP_READ);
	NFI_SET_REG16(NFI_CNFG_REG16, CNFG_READ_EN);
	DRV_WriteReg16(NFI_CON_REG16, sec_num << CON_NFI_SEC_SHIFT);

	if (full)
	{
		if(g_bUseAHBMode){
			NFI_SET_REG16(NFI_CNFG_REG16, CNFG_AHB);
			//DRV_WriteReg32(NFI_STRADDR_REG32, virt_to_phys(buf));
			//printf("read buffer:%x\n", buf);
			DRV_WriteReg32(NFI_STRADDR_REG32, K1_TO_PHY(buf));
		}
	else{
			NFI_CLN_REG16(NFI_CNFG_REG16, CNFG_AHB);
	}

	if(g_bHwEcc){	
		NFI_SET_REG16(NFI_CNFG_REG16, CNFG_HW_ECC_EN);
	}else{
		NFI_CLN_REG16(NFI_CNFG_REG16, CNFG_HW_ECC_EN);
	}

	}
	else
	{
	    NFI_CLN_REG16(NFI_CNFG_REG16, CNFG_HW_ECC_EN);
	    NFI_CLN_REG16(NFI_CNFG_REG16, CNFG_AHB);
	}

	mt6573_nand_set_autoformat(full);
	if (full){
		if(g_bHwEcc){
			ECC_Decode_Start();
		}
	}
	if((devinfo.pagesize == 512) && (u4ColAddr == devinfo.pagesize)){ // read oob for 512 page size
		if (!mt6573_nand_set_command(NAND_CMD_READOOB))
		{
			goto cleanup;
		}
	}
	else{

		if (!mt6573_nand_set_command(NAND_CMD_READ0))
		{
			goto cleanup;
		}
	}

	if(devinfo.pagesize == 512)
		colnob = 1;
	else
		colnob = 2;

	rownob=devinfo.addr_cycle - colnob;

	//1 FIXED ME: For Any Kind of AddrCycle
	if (!mt6573_nand_set_address(col_addr, u4RowAddr, colnob, rownob))
	{
		goto cleanup;
	}
	
	if(devinfo.pagesize != 512){
		if (!mt6573_nand_set_command(NAND_CMD_READSTART))
		{
			goto cleanup;
		}
	}

	if (!mt6573_nand_status_ready(STA_NAND_BUSY))
	{
		goto cleanup;
	}

	bRet = true;
	
cleanup:
	return bRet;
}

/**********************************************************
Description : SAL_NFI_Pointer_Operation
Input       : 0
Output      : 0
***********************************************************/
static void SAL_NFI_Pointer_Operation(u16 command)
{
#if 0
   kal_uint32  reg_val = 0;

   DRV_WriteReg(NFI_CMD, ptr_cmd);
   while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
   reg_val = DRV_Reg(NFI_CON);
   reg_val |= CON_NFI_RST;
   DRV_WriteReg(NFI_CON, reg_val);
#endif
   	//mb();
	
	DRV_WriteReg16(NFI_CMD_REG16, command);
	mt6573_nand_status_ready(STA_CMD_STATE);

	NFI_SET_REG16(NFI_CON_REG16, CON_NFI_RST);
	
}

/******************************************************************************
 * mt6573_nand_ready_for_write
 * 
 * DESCRIPTION:
 *    Prepare hardware environment for write ! 
 * 
 * PARAMETERS: 
 *   struct nand_chip *nand, u32 u4RowAddr 
 * 
 * RETURNS: 
 *   None   
 * 
 * NOTES: 
 *   None
 *
 ******************************************************************************/
static bool mt6573_nand_ready_for_write(
	struct mt6573_nand_host_hw *hw, u32 u4RowAddr, u32 col_addr, bool full, u8 *buf)
{
	bool bRet = false;
	u32 sec_num = hw->nand_ecc_size/hw->nand_sec_size;
	u32 colnob=2, rownob;
	
	if (hw->nfi_bus_width == 16)
	    col_addr /= 2;


	/* Reset NFI HW internal state machine and flush NFI in/out FIFO */	
	if (!mt6573_nand_reset())
	{
		return false;
	}

	mt6573_nand_set_mode(CNFG_OP_PRGM);
	
	NFI_CLN_REG16(NFI_CNFG_REG16, CNFG_READ_EN);
	
	DRV_WriteReg16(NFI_CON_REG16, sec_num << CON_NFI_SEC_SHIFT);

	if (full)
	{
		if(g_bUseAHBMode){
			NFI_SET_REG16(NFI_CNFG_REG16, CNFG_AHB);
			//DRV_WriteReg32(NFI_STRADDR_REG32, virt_to_phys(buf));
			//printf("write buffer:%x\n", buf);
			DRV_WriteReg32(NFI_STRADDR_REG32, K1_TO_PHY(buf));
		}
		else{
			NFI_CLN_REG16(NFI_CNFG_REG16, CNFG_AHB);
		}

		if(g_bHwEcc){
		NFI_SET_REG16(NFI_CNFG_REG16, CNFG_HW_ECC_EN);
		}else{
			NFI_CLN_REG16(NFI_CNFG_REG16, CNFG_HW_ECC_EN);
		}
	}
	else
	{
	    NFI_CLN_REG16(NFI_CNFG_REG16, CNFG_HW_ECC_EN);
	    NFI_CLN_REG16(NFI_CNFG_REG16, CNFG_AHB);
	}

	mt6573_nand_set_autoformat(full);

	if (full){
		if(g_bHwEcc){
			ECC_Encode_Start();
		}
    	}


	if(devinfo.pagesize == 512){
		if(col_addr == devinfo.pagesize){		//write oob
			SAL_NFI_Pointer_Operation(0x50);
		}
		else{
			SAL_NFI_Pointer_Operation(0);
		}
	}

	if (!mt6573_nand_set_command(NAND_CMD_SEQIN)){
		goto cleanup;
	}

	if(devinfo.pagesize == 512)
		colnob = 1;
	else
		colnob = 2;

	rownob=devinfo.addr_cycle - colnob;

	//1 FIXED ME: For Any Kind of AddrCycle
	if (!mt6573_nand_set_address(col_addr, u4RowAddr, colnob, rownob)){
		goto cleanup;
	}

	if (!mt6573_nand_status_ready(STA_NAND_BUSY)){
		goto cleanup;
	}

	bRet = true;
cleanup:

	return bRet;
}

static bool mt6573_nand_check_dececc_done(u32 u4SecNum)
{
    u32 timeout, dec_mask;
    timeout = 0xffff;
    dec_mask = (1<<u4SecNum)-1;
    while((dec_mask != (DRV_Reg16(ECC_DECDONE_REG16) & 0xFF)) && timeout>0){
		//if(DRV_Reg16(ECC_DECDONE_REG16)){
		//	printf("mt6573_nand_check_dececc_done: %x\n", DRV_Reg16(ECC_DECDONE_REG16));
		//}
        timeout--;
    }
    if(timeout == 0){
	
		//dump_nfi();
		return false;
    }
    return true;
}




/******************************************************************************
 * mt6573_nand_read_page_data
 * 
 * DESCRIPTION:
 *   Fill the page data into buffer ! 
 * 
 * PARAMETERS: 
 *   u8* pDataBuf, u32 u4Size
 * 
 * RETURNS: 
 *   None   
 * 
 * NOTES: 
 *   None
 *
 ******************************************************************************/
static bool mt6573_nand_dma_read_data(u8 *buf, u32 length)
{
	//int interrupt_en = g_i4Interrupt;
	int timeout = 0xffff;
	//struct scatterlist sg;
	//enum dma_data_direction dir = DMA_FROM_DEVICE;

	//  sg_init_one(&sg, buf, length);
	//dma_map_sg(&(mtd->dev), &sg, 1, dir);
	//dma_map_sg(NULL, &sg, 1, dir);


	NFI_CLN_REG16(NFI_CNFG_REG16, CNFG_BYTE_RW);
	// DRV_WriteReg32(NFI_STRADDR_REG32, __virt_to_phys(pDataBuf));
 
	if ((unsigned int)buf % 16)		// TODO: can not use AHB mode here
	{
	    //printf( "Un-16-aligned address\n");
	    NFI_CLN_REG16(NFI_CNFG_REG16, CNFG_DMA_BURST_EN);
	}
	else
	{
	    NFI_SET_REG16(NFI_CNFG_REG16, CNFG_DMA_BURST_EN);
	}

	DRV_Reg16(NFI_INTR_REG16);
	DRV_WriteReg16(NFI_INTR_EN_REG16, INTR_AHB_DONE_EN);

	//dump_nfi();
	//printk("NFI_STRADDR_REG32=%x\n", DRV_Reg32(NFI_STRADDR_REG32));


	NFI_SET_REG16(NFI_CON_REG16, CON_NFI_BRD);
   


        while (!DRV_Reg16(NFI_INTR_REG16))
        {
		timeout--;

		if (0 == timeout)
		{
		    printf( "[%s] poll nfi_intr error\n", __FUNCTION__);
			#ifdef NAND_DEBUG
		    dump_nfi();
			#endif
		    return false; //4  // AHB Mode Time Out!
		}
        }
		timeout = 0xffff;
        while ( (length >> 9) > ((DRV_Reg16(NFI_BYTELEN_REG16) & 0xf000) >> 12) )
        {
		timeout--;

		if (0 == timeout)
		{
		    printf( "[%s] poll BYTELEN error\n", __FUNCTION__);
			#ifdef NAND_DEBUG
		    dump_nfi();
			#endif
		    return false; //4  // AHB Mode Time Out!
		}
	}
	//dma_unmap_sg(NULL, &sg, 1, dir);

	return true;
}


#if defined(CONFIG_BADBLOCK_CHECK) || defined(NAND_BMT) || defined(PIO_MODE_SUPPORT)

static bool mt6573_nand_mcu_read_data(u8 *buf, u32 length)
{
    int timeout = 0xffff;
	u32 i, sec_num, sec_idx, temp;
	u32* buf32 = (u32 *)buf;
	
    if ((u32)buf % 4 || length % 4)
	    NFI_SET_REG16(NFI_CNFG_REG16, CNFG_BYTE_RW);
    else
        NFI_CLN_REG16(NFI_CNFG_REG16, CNFG_BYTE_RW);

    //DRV_WriteReg32(NFI_STRADDR_REG32, 0);
    //mb();
	NFI_SET_REG16(NFI_CON_REG16, CON_NFI_BRD);

    if ((u32)buf % 4 || length % 4)
    {
        for (i = 0; (i < (length))&&(timeout > 0);)
        {
    		//if (FIFO_RD_REMAIN(DRV_Reg16(NFI_FIFOSTA_REG16)) >= 4)
    		if (DRV_Reg16(NFI_PIO_DIRDY_REG16) & 1)
            {
    			*buf++ = (u8)DRV_Reg32(NFI_DATAR_REG32);
    			i++;
    		} 
            else 
            {
    			timeout--;
    		}
    		if (0 == timeout)
            {
                printf( "[%s] timeout\n", __FUNCTION__);
				#ifdef NAND_DEBUG
                dump_nfi();
				#endif
    			return false;
    		}
    	}
    }
    else
    {
    	if(g_bAutoFMT || (length < NAND_SECTOR_SIZE)){
	        for (i = 0; (i < (length >> 2))&&(timeout > 0);)
	        {
	    		//if (FIFO_RD_REMAIN(DRV_Reg16(NFI_FIFOSTA_REG16)) >= 4)
	    		if (DRV_Reg16(NFI_PIO_DIRDY_REG16) & 1)
	            {
	    			*buf32++ = DRV_Reg32(NFI_DATAR_REG32);
	    			i++;
					
	    		} 
	            else 
	            {
	    			timeout--;
	    		}
	    		if (0 == timeout)
	            {
	                printf( "[%s] auto fmt mode timeout\n", __FUNCTION__);
					#ifdef NAND_DEBUG
	                dump_nfi();
					#endif
	    			return false;
	    		}
	    	}
    	}
	else{
		sec_num = length / NAND_SECTOR_SIZE;
		for(sec_idx = 0 ; sec_idx < sec_num ; sec_idx++)
		{
			timeout = 0xFFFF;
			for (i = 0; (i < (NAND_SECTOR_SIZE >> 2))&&(timeout > 0);)
		        {
		    		//if (FIFO_RD_REMAIN(DRV_Reg16(NFI_FIFOSTA_REG16)) >= 4)
		    		if (DRV_Reg16(NFI_PIO_DIRDY_REG16) & 1)
		            {
		    			*buf32++ = DRV_Reg32(NFI_DATAR_REG32);
		    			i++;
		    		} 
		            else 
		            {
		    			timeout--;
		    		}
		    		if (0 == timeout)
		            {
		                printf( "[%s] timeout\n", __FUNCTION__);
						#ifdef NAND_DEBUG
		                dump_nfi();
						#endif
		    			return false;
		    		}
		    	}
			for (i = 0; (i < (OOB_PER_SECTOR >> 2))&&(timeout > 0);)
		        {
		    		//if (FIFO_RD_REMAIN(DRV_Reg16(NFI_FIFOSTA_REG16)) >= 4)
		    		if (DRV_Reg16(NFI_PIO_DIRDY_REG16) & 1)
		            {
		    			temp = DRV_Reg32(NFI_DATAR_REG32);
		    			i++;
		    		} 
		            else 
		            {
		    			timeout--;
		    		}
		    		if (0 == timeout)
		            {
		                printf( "[%s] timeout\n", __FUNCTION__);
						#ifdef NAND_DEBUG
		                dump_nfi();
						#endif
		    			return false;
		    		}
		    	}
			
    	 	}
	}
	
     }
	
    return true;
}
#endif

static bool mt6573_nand_read_page_data(u8* pDataBuf, u32 u4Size)
{

	if(g_bUseAHBMode)
		return mt6573_nand_dma_read_data(pDataBuf, u4Size);
	#ifdef PIO_MODE_SUPPORT
	else
		return mt6573_nand_mcu_read_data(pDataBuf, u4Size);
	#endif

}	

#ifndef LZMA_IMG
/******************************************************************************
 * mt6573_nand_write_page_data
 * 
 * DESCRIPTION:
 *   Fill the page data into buffer ! 
 * 
 * PARAMETERS: 
 *   u8* pDataBuf, u32 u4Size
 * 
 * RETURNS: 
 *   None   
 * 
 * NOTES: 
 *   None
 *
 ******************************************************************************/
static bool mt6573_nand_dma_write_data(u8 *pDataBuf, u32 u4Size)
{
	//int i4Interrupt = g_i4Interrupt;	//g_i4Interrupt;
	u32 timeout = 0xFFFF;
	//struct scatterlist sg;
	//enum dma_data_direction dir = DMA_TO_DEVICE;
	u16 reg_status = 0;


	//flush_dcache_range((unsigned long)pDataBuf, (unsigned long)(pDataBuf+u4Size));
	
	NFI_CLN_REG16(NFI_CNFG_REG16, CNFG_BYTE_RW);
	DRV_Reg16(NFI_INTR_REG16);
	DRV_WriteReg16(NFI_INTR_EN_REG16, 0);
	// DRV_WriteReg32(NFI_STRADDR_REG32, (u32*)virt_to_phys(pDataBuf));
    
	if ((unsigned int)pDataBuf % 16)		// TODO: can not use AHB mode here
	{
	    //printf( "Un-16-aligned address\n");
	    NFI_CLN_REG16(NFI_CNFG_REG16, CNFG_DMA_BURST_EN);
	}
	else
	{
	    NFI_SET_REG16(NFI_CNFG_REG16, CNFG_DMA_BURST_EN);
	}
    

	//dmac_clean_range(pDataBuf, pDataBuf + u4Size);
	//mb();
	NFI_SET_REG16(NFI_CON_REG16, CON_NFI_BWR);
	

	//	dump_nfi();
	//printk("NFI_STRADDR_REG32=%x\n", DRV_Reg32(NFI_STRADDR_REG32));
	//printk("NFI_INTR_EN_REG16=%x\n", DRV_Reg32(NFI_INTR_EN_REG16));
	//dump_buf(pDataBuf, 16);
	 //printk("***********************: %x\n", pDataBuf);

	while ( (u4Size >> 9) > ((DRV_Reg16(NFI_BYTELEN_REG16) & 0xf000) >> 12) )
	{
		timeout--;
		if (0 == timeout)
		{
		 	printf( "[%s] poll BYTELEN error\n", __FUNCTION__);
			return false; //4  // AHB Mode Time Out!
		}
	}



    return true;
}
#endif

#if	defined(PIO_MODE_SUPPORT) || defined(NAND_BMT)
static bool mt6573_nand_mcu_write_data(const u8 *buf, u32 length)
{
	u32 timeout = 0xFFFF;
	u32 i, sec_idx, sec_num;	
	u32* pBuf32, *pOOBBuf32 = NULL;
	NFI_CLN_REG16(NFI_CNFG_REG16, CNFG_BYTE_RW);
	//mb();
	NFI_SET_REG16(NFI_CON_REG16, CON_NFI_BWR);
	pBuf32 = (u32*)buf;
    
    if ((u32)buf % 4 || length % 4)
	    NFI_SET_REG16(NFI_CNFG_REG16, CNFG_BYTE_RW);
    else
        NFI_CLN_REG16(NFI_CNFG_REG16, CNFG_BYTE_RW);

    if ((u32)buf % 4 || length % 4)
    {
        for (i = 0; (i < (length))&&(timeout > 0);)
        {
            if (DRV_Reg16(NFI_PIO_DIRDY_REG16) & 1)
            {
			    DRV_WriteReg32(NFI_DATAW_REG32, *buf++);
    			i++;
    		} 
            else 
            {
    			timeout--;
    		}
    		if (0 == timeout)
            {
                printf( "[%s] timeout\n", __FUNCTION__);
				#ifdef NAND_DEBUG
                dump_nfi();
				#endif
    			return false;
    		}
    	}
    }
    else
    {
    	if(g_bAutoFMT || (length < NAND_SECTOR_SIZE)){
		 for (i = 0; (i < (length >> 2)) && (timeout > 0); )
	        {
			    // if (FIFO_WR_REMAIN(DRV_Reg16(NFI_FIFOSTA_REG16)) <= 12)
	            if (DRV_Reg16(NFI_PIO_DIRDY_REG16) & 1)
	            {
				    DRV_WriteReg32(NFI_DATAW_REG32, *pBuf32++);
				    i++;
			    } 
	            else 
	            {
				    timeout--;
			    }
			    if (0 == timeout)
	            {
	                    printf( "[%s] timeout\n", __FUNCTION__);
						#ifdef NAND_DEBUG
	                    dump_nfi();
						#endif
	        			return false;
			    }
		    }
    	}
	else{
		
    		sec_num = length / NAND_SECTOR_SIZE;
		for(sec_idx = 0 ; sec_idx < sec_num ; sec_idx++)
		{
			timeout = 0xFFFF;
			for (i = 0; (i < (NAND_SECTOR_SIZE >> 2)) && (timeout > 0); )
			{
				if (DRV_Reg16(NFI_PIO_DIRDY_REG16) & 1)
				{
					    DRV_WriteReg32(NFI_DATAW_REG32, *pBuf32++);
					    i++;
				} 
				else 
				{
				    timeout--;
				}
				if (0 == timeout)
				{
		                    printf( "[%s] timeout\n", __FUNCTION__);
							#ifdef NAND_DEBUG
		                    dump_nfi();
							#endif
		        			return false;
				}
			}
			for (i = 0; (i < (OOB_PER_SECTOR >> 2)) && (timeout > 0); )
			{
				if (DRV_Reg16(NFI_PIO_DIRDY_REG16) & 1)
				{
					
					DRV_WriteReg32(NFI_DATAW_REG32, 0xffffffff);	   
					 i++;
				} 
				else 
				{
				    timeout--;
				}
				if (0 == timeout)
				{
		                    printf( "[%s] timeout\n", __FUNCTION__);
							#ifdef NAND_DEBUG
		                    dump_nfi();
							#endif
		        			return false;
				}
			}
    	 	}
	}
	
		
	        
    }

	return true;
}
#endif
#ifndef LZMA_IMG
static bool mt6573_nand_write_page_data(u8* buf, u32 size)
{
//printk("mt6573_nand_write_page_data enter\n");
	if(g_bUseAHBMode)
		return mt6573_nand_dma_write_data(buf, size);
	#ifdef PIO_MODE_SUPPORT
	else		
		return mt6573_nand_mcu_write_data(buf, size);
	#endif
}
#endif
/******************************************************************************
 * mt6573_nand_read_fdm_data
 * 
 * DESCRIPTION:
 *   Read a fdm data ! 
 * 
 * PARAMETERS: 
 *   u8* pDataBuf, u32 u4SecNum
 * 
 * RETURNS: 
 *   None   
 * 
 * NOTES: 
 *   None
 *
 ******************************************************************************/
static void mt6573_nand_read_fdm_data(u8* pDataBuf, u32 u4SecNum)
{
	u32 i;
	u32* pBuf32 = (u32*)pDataBuf;

	if (pBuf32)
	{
		for (i = 0; i < u4SecNum; ++i)
		{
			*pBuf32++ = DRV_Reg32(NFI_FDM0L_REG32 + (i<<1));
			*pBuf32++ = DRV_Reg32(NFI_FDM0M_REG32 + (i<<1));
			//*pBuf32++ = DRV_Reg32((u32)NFI_FDM0L_REG32 + (i<<3));
			//*pBuf32++ = DRV_Reg32((u32)NFI_FDM0M_REG32 + (i<<3));
		}
	}
}

#ifndef LZMA_IMG
/******************************************************************************
 * mt6573_nand_write_fdm_data
 * 
 * DESCRIPTION:
 *   Write a fdm data ! 
 * 
 * PARAMETERS: 
 *   u8* pDataBuf, u32 u4SecNum
 * 
 * RETURNS: 
 *   None   
 * 
 * NOTES: 
 *   None
 *
 ******************************************************************************/

static void mt6573_nand_write_fdm_data(u8* pDataBuf, u32 u4SecNum)
{
	u32 i, j;
	u8 checksum = 0;
	bool empty = true;
	struct nand_oobfree *free_entry;
	u32* pBuf32;

	memcpy(fdm_buf, pDataBuf, u4SecNum * 8);


    	pBuf32 = (u32*)fdm_buf;
	for (i = 0; i < u4SecNum; ++i)
	{
		DRV_WriteReg32(NFI_FDM0L_REG32 + (i<<1), *pBuf32++);
		DRV_WriteReg32(NFI_FDM0M_REG32 + (i<<1), *pBuf32++);		
		//DRV_WriteReg32((u32)NFI_FDM0L_REG32 + (i<<3), *pBuf32++);
		//DRV_WriteReg32((u32)NFI_FDM0M_REG32 + (i<<3), *pBuf32++);			
	}
}
#endif

/******************************************************************************
 * mt6573_nand_stop_read
 * 
 * DESCRIPTION:
 *   Stop read operation ! 
 * 
 * PARAMETERS: 
 *   None
 * 
 * RETURNS: 
 *   None   
 * 
 * NOTES: 
 *   None
 *
 ******************************************************************************/
static void mt6573_nand_stop_read(void)
{
	NFI_CLN_REG16(NFI_CON_REG16, CON_NFI_BRD);
	mt6573_nand_reset();
	if(g_bHwEcc){
		ECC_Decode_End();
	}
	DRV_WriteReg16(NFI_INTR_EN_REG16, 0);
}
#ifndef LZMA_IMG
/******************************************************************************
 * mt6573_nand_stop_write
 * 
 * DESCRIPTION:
 *   Stop write operation ! 
 * 
 * PARAMETERS: 
 *   None
 * 
 * RETURNS: 
 *   None   
 * 
 * NOTES: 
 *   None
 *
 ******************************************************************************/
static void mt6573_nand_stop_write(void)
{
	NFI_CLN_REG16(NFI_CON_REG16, CON_NFI_BWR);
	if(g_bHwEcc){
		ECC_Encode_End();
	}
    DRV_WriteReg16(NFI_INTR_EN_REG16, 0);
}
#endif
/******************************************************************************
 * mt6573_nand_exec_read_page
 * 
 * DESCRIPTION:
 *   Read a page data ! 
 * 
 * PARAMETERS: 
 *   struct mtd_info *mtd, u32 u4RowAddr, u32 u4PageSize, 
 *   u8* pPageBuf, u8* pFDMBuf
 * 
 * RETURNS: 
 *   None   
 * 
 * NOTES: 
 *   None
 *
 ******************************************************************************/
int mt6573_nand_exec_read_page(
	struct mt6573_nand_host_hw *hw, u32 u4RowAddr, u32 u4PageSize, u8* pPageBuf, u8* pFDMBuf)
{
	u8 *buf;
	int bRet = 0;
	//struct nand_chip *nand = mtd->priv;
	u32 u4SecNum = u4PageSize >> 9;
	
	//printf( "mt6573_nand_exec_read_page:u4RowAddr:%u, u4PageSize:%u\n", u4RowAddr, u4PageSize);

	if (((u32)pPageBuf % 16) && local_buffer_16_align)
	{
        //	printf( "Data buffer not 16 bytes aligned: %p\n", pPageBuf);
		buf = local_buffer_16_align;
	}
	else
		buf = pPageBuf;


	if (mt6573_nand_ready_for_read(hw, u4RowAddr, 0, true, buf))
	{
		if (!mt6573_nand_read_page_data(buf, u4PageSize))
		{
			//printf("mt6573_nand_read_page_data return false\n");
			bRet = -1;
		}


        
		if (!mt6573_nand_status_ready(STA_NAND_BUSY))
		{
			//printf("mt6573_nand_status_ready return false\n");
			bRet = -1;
		}
	
		//dump_buf(local_buffer_16_align, u4PageSize);
		if(g_bHwEcc){

			if(!mt6573_nand_check_dececc_done(u4SecNum))
			{
				//printf("mt6573_nand_check_dececc_done return false\n");
				bRet = -1;
				
			}
		}       
        
		mt6573_nand_read_fdm_data(pFDMBuf, u4SecNum);

		if(g_bHwEcc){

			if (!mt6573_nand_check_bch_error(buf, u4SecNum - 1, u4RowAddr))
			{
				//printf("mt6573_nand_check_bch_error return false\n");
				bRet = -1;
			}
		}
		mt6573_nand_stop_read();
	}
	

    if (buf == local_buffer_16_align)
        memcpy(pPageBuf, buf, u4PageSize);

	
	return bRet;
}

#ifndef LZMA_IMG
/******************************************************************************
 * mt6573_nand_exec_write_page
 * 
 * DESCRIPTION:
 *   Write a page data ! 
 * 
 * PARAMETERS: 
 *   struct mtd_info *mtd, u32 u4RowAddr, u32 u4PageSize, 
 *   u8* pPageBuf, u8* pFDMBuf
 * 
 * RETURNS: 
 *   None   
 * 
 * NOTES: 
 *   None
 *
 ******************************************************************************/
int mt6573_nand_exec_write_page(
	struct mt6573_nand_host_hw *hw, u32 u4RowAddr, u32 u4PageSize, u8* pPageBuf, u8* pFDMBuf)
{
	//struct nand_chip *chip = mtd->priv;
	u32 u4SecNum = u4PageSize >> 9;
	u8 *buf;
	u8 status;    


	if (((u32)pPageBuf % 16) && local_buffer_16_align)
	{
	    //printf( "Data buffer not 16 bytes aligned: %p\n", pPageBuf);
	    memcpy(local_buffer_16_align, pPageBuf, hw->nand_ecc_size);
	    buf = local_buffer_16_align;
	}
	else
	    buf = pPageBuf;



	if (mt6573_nand_ready_for_write(hw, u4RowAddr, 0, true, buf))
	{

		mt6573_nand_write_fdm_data(pFDMBuf, u4SecNum);

			
		(void)mt6573_nand_write_page_data(buf, u4PageSize);
		(void)mt6573_nand_check_RW_count(u4PageSize);
		mt6573_nand_stop_write();
		(void)mt6573_nand_set_command(NAND_CMD_PAGEPROG);
		while(DRV_Reg32(NFI_STA_REG32) & STA_NAND_BUSY);		
	}
	

	status = mt6573_nand_wait();
	if (status & NAND_STATUS_FAIL){
		printf("write_page:fail,status=%d", status);
	    return -1;
	}
	else
	    return 0;
}
#endif

/******************************************************************************
 * mt6573_nand_command_bp
 * 
 * DESCRIPTION:
 *   Handle the commands from MTD ! 
 * 
 * PARAMETERS: 
 *   struct mtd_info *mtd, unsigned int command, int column, int page_addr
 * 
 * RETURNS: 
 *   None   
 * 
 * NOTES: 
 *   None
 *
 ******************************************************************************/
static void mt6573_nand_command_bp(unsigned int command,
			 int column, int page_addr)
{
//	struct nand_chip* nand = mtd->priv;
	int       timeout;
	int	page_address = page_addr;


	//printk("mt6573_nand_command_bp:0x%x, 0x%x, 0x%x\n", command, column, page_addr);
    switch (command) 
    {
    #if 0
        case NAND_CMD_SEQIN:
		    /* Reset g_kCMD */
		//if (g_kCMD.u4RowAddr != page_addr) {
			memset(g_kCMD.au1OOB, 0xFF, sizeof(g_kCMD.au1OOB));
			g_kCMD.pDataBuf = NULL;
        //}
		    g_kCMD.u4RowAddr = page_addr;
		    g_kCMD.u4ColAddr = column;
            break;

        case NAND_CMD_PAGEPROG:
           	if (g_kCMD.pDataBuf || (0xFF != g_kCMD.au1OOB[0])) 
    		{
           		u8* pDataBuf = g_kCMD.pDataBuf ? g_kCMD.pDataBuf : nand->buffers->databuf;
    			mt6573_nand_exec_write_page(mtd, g_kCMD.u4RowAddr, mtd->writesize, pDataBuf, g_kCMD.au1OOB);
    			g_kCMD.u4RowAddr = (u32)-1;
    			g_kCMD.u4OOBRowAddr = (u32)-1;
            }
            break;

        case NAND_CMD_READOOB:
    		g_kCMD.u4RowAddr = page_addr;        	
    		g_kCMD.u4ColAddr = column + mtd->writesize;
    		#ifdef NAND_PFM
    		g_kCMD.pureReadOOB = 1;
    		g_kCMD.pureReadOOBNum += 1;
    		#endif
			break;
			
        case NAND_CMD_READ0:
    		g_kCMD.u4RowAddr = page_addr;        	
    		g_kCMD.u4ColAddr = column;
    		#ifdef NAND_PFM
    		g_kCMD.pureReadOOB = 0;
    		#endif		
			break;
        #endif
		#ifndef LZMA_IMG
        case NAND_CMD_ERASE1:
    		
    		(void)mt6573_nand_reset();
            mt6573_nand_set_mode(CNFG_OP_ERASE);
    		(void)mt6573_nand_set_command(NAND_CMD_ERASE1);

		if(devinfo.pagesize == 512){
			(void)mt6573_nand_set_address(0,page_addr,0,devinfo.addr_cycle-1);
		}
		else{
			(void)mt6573_nand_set_address(0,page_addr,0,devinfo.addr_cycle-2);
		}    		
			
            break;
            
        case NAND_CMD_ERASE2:
       	    (void)mt6573_nand_set_command(NAND_CMD_ERASE2);
			while(DRV_Reg32(NFI_STA_REG32) & STA_NAND_BUSY);
		    
            break;
    	#endif
        case NAND_CMD_STATUS:
            (void)mt6573_nand_reset(); 
            NFI_CLN_REG16(NFI_CNFG_REG16, CNFG_BYTE_RW);		           	
			mt6573_nand_set_mode(CNFG_OP_SRD);
            mt6573_nand_set_mode(CNFG_READ_EN);
            NFI_CLN_REG16(NFI_CNFG_REG16, CNFG_AHB);
            NFI_CLN_REG16(NFI_CNFG_REG16, CNFG_HW_ECC_EN);
		    (void)mt6573_nand_set_command(NAND_CMD_STATUS);
        	NFI_CLN_REG16(NFI_CON_REG16, CON_NFI_NOB_MASK);
        	//mb();
			DRV_WriteReg16(NFI_CON_REG16, CON_NFI_SRD|(1 << CON_NFI_NOB_SHIFT));
            g_bcmdstatus = true;            
            break;
            
        case NAND_CMD_RESET:
       	    (void)mt6573_nand_reset();
            break;

	case NAND_CMD_READID: 
		mt6573_nand_reset();
		/* Disable HW ECC */
		NFI_CLN_REG16(NFI_CNFG_REG16, CNFG_HW_ECC_EN);
		NFI_CLN_REG16(NFI_CNFG_REG16, CNFG_AHB);

		#if 1
		/* Reset NFI state machine */
		mt6573_nand_reset();

		/* Issue NAND chip reset command for Micron's MCP */
		NFI_ISSUE_COMMAND(NAND_CMD_RESET, 0, 0, 0, 0);

		timeout = TIMEOUT_4;

		while(timeout)
	        timeout--;
		#endif

    
        /* Disable 16-bit I/O */
        //NFI_CLN_REG16(NFI_PAGEFMT_REG16, PAGEFMT_DBYTE_EN);
		
		NFI_SET_REG16(NFI_CNFG_REG16, CNFG_READ_EN|CNFG_BYTE_RW);
		(void)mt6573_nand_reset();
		mt6573_nand_set_mode(CNFG_OP_SRD);
		(void)mt6573_nand_set_command(NAND_CMD_READID);
		(void)mt6573_nand_set_address(0,0,1,0);
		//mb();
		DRV_WriteReg16(NFI_CON_REG16, CON_NFI_SRD);
		while(DRV_Reg32(NFI_STA_REG32) & STA_DATAR_STATE);
		break;
            
        default:
            //BUG();        
            break;
    }
 }


/******************************************************************************
 * mt6573_nand_select_chip
 * 
 * DESCRIPTION:
 *   Select a chip ! 
 * 
 * PARAMETERS: 
 *   struct mtd_info *mtd, int chip
 * 
 * RETURNS: 
 *   None   
 * 
 * NOTES: 
 *   None
 *
 ******************************************************************************/
//static void mt6573_nand_select_chip(struct mtd_info *mtd, int chip)
static void mt6573_nand_select_chip(int chip)
{
    switch(chip)
    {
	case -1:
		break;
	case 0: 
	case 1:
		DRV_WriteReg16(NFI_CSEL_REG16, chip);
		break;
    }
}

/******************************************************************************
 * mt6573_nand_read_byte
 * 
 * DESCRIPTION:
 *   Read a byte of data ! 
 * 
 * PARAMETERS: 
 *   struct mtd_info *mtd
 * 
 * RETURNS: 
 *   None   
 * 
 * NOTES: 
 *   None
 *
 ******************************************************************************/
//static uint8_t mt6573_nand_read_byte(struct mtd_info *mtd)
static uint8_t mt6573_nand_read_byte()
{
    uint8_t retval = 0;
	int value = 0;
    
    if (!mt6573_nand_pio_ready())
    {
        printf("pio ready timeout\n");
        retval = false;
    }

    if(g_bcmdstatus)
    {
     	if(PIO_BIG_ENDIAN){
	        value = DRV_Reg8(NFI_DATAR_REG32);
			retval = (uint8_t)(value >> 24);
     	}
		else
			retval = DRV_Reg8(NFI_DATAR_REG32);
		
        NFI_CLN_REG16(NFI_CON_REG16, CON_NFI_NOB_MASK); 
        mt6573_nand_reset();
	if(g_bUseAHBMode){        
		NFI_SET_REG16(NFI_CNFG_REG16, CNFG_AHB);
	}

	if(g_bHwEcc){
		NFI_SET_REG16(NFI_CNFG_REG16, CNFG_HW_ECC_EN);
	}else{
		NFI_CLN_REG16(NFI_CNFG_REG16, CNFG_HW_ECC_EN);
	}
        g_bcmdstatus = false;
    }
    else{
        if(PIO_BIG_ENDIAN){
	        value = DRV_Reg8(NFI_DATAR_REG32);
			retval = (uint8_t)(value >> 24);
     	}
		else
			retval = DRV_Reg8(NFI_DATAR_REG32);
    }
	
    return retval;
}


static int mt6573_nand_wait()
{
	u32 timeout = 0xFFFF;
	int status;
	
	mt6573_nand_command_bp(NAND_CMD_STATUS, -1, -1);

	while(timeout){
		timeout--;
		
		if(mt6573_nand_dev_ready())
			break;
	}
	status = (int)mt6573_nand_read_byte();
	return status;
}

#if defined(CONFIG_BADBLOCK_CHECK)||defined(NAND_BMT) 

/******************************************************************************
 * mt6573_nand_read_oob_raw
 *
 * DESCRIPTION:
 *   Read oob data
 *
 * PARAMETERS:
 *   struct mtd_info *mtd, const uint8_t *buf, int addr, int len 
 *
 * RETURNS:
 *   None
 *
 * NOTES:
 *   this function read raw oob data out of flash, so need to re-organise 
 *   data format before using.
 *   len should be times of 8, call this after nand_get_device.
 *   Should notice, this function read data without ECC protection.
 *
 *****************************************************************************/
static int mt6573_nand_read_oob_raw(struct mt6573_nand_host_hw *hw, uint8_t *buf, int page_addr, int len)
{
    //struct nand_chip *chip = (struct nand_chip *)mtd->priv;
    u32 col_addr = 0;
    u32 sector = 0;
    int res = 0;
    u32 colnob=2, rawnob;
	int randomread =0;
    int read_len = 0;

    if (len > 128 || len % OOB_AVAI_PER_SECTOR || !buf)
    {
        //printf( "[%s] invalid parameter, len: %d, buf: %p\n",
        //        __FUNCTION__, len, buf);
        return -1;
    }
   
	
    while (len > 0)
    {
        //read_len = min(len, OOB_PER_SECTOR);
        read_len = len < OOB_PER_SECTOR ? len : OOB_PER_SECTOR;
        col_addr = NAND_SECTOR_SIZE + sector * (NAND_SECTOR_SIZE + OOB_PER_SECTOR); // TODO: Fix this hard-code 16
	
		if (!mt6573_nand_ready_for_read(hw, page_addr, col_addr, false, NULL))
        {
            printf("ready_for_read return failed\n");
            res = -1;
            goto error;
        }
        if (!mt6573_nand_mcu_read_data(buf + OOB_PER_SECTOR * sector, read_len))    // TODO: and this 8
        {
            printf("mcu_read_data return failed\n");
            res = -1;
            goto error;
        }
        mt6573_nand_stop_read();
		//dump_data(buf + 16 * sector,16);
        sector++;
        len -= read_len;
		
    }
	
    
	
error:
    NFI_CLN_REG16(NFI_CON_REG16, CON_NFI_BRD);
    return res;
}

#if defined(NAND_BMT)

static int mt6573_nand_write_oob_raw(struct mt6573_nand_host_hw *hw, const uint8_t *buf, int page_addr, int len)
{
    //struct nand_chip *chip = mtd->priv;
    // int i;
    u32 col_addr = 0;
    u32 sector = 0;
    // int res = 0;
    // u32 colnob=2, rawnob=devinfo.addr_cycle-2;
	// int randomread =0;
    int write_len = 0;
    int status;

    if (len > 64 || len % OOB_AVAI_PER_SECTOR || !buf)
    {
        //printf( "[%s] invalid parameter, len: %d, buf: %p\n",
        //        __FUNCTION__, len, buf);
        return -1;
    }

    while (len > 0)
    {
        //write_len = min(len, OOB_PER_SECTOR);
		write_len = len < OOB_PER_SECTOR ? len : OOB_PER_SECTOR;
        col_addr = sector * (NAND_SECTOR_SIZE + OOB_PER_SECTOR) + NAND_SECTOR_SIZE;
        if (!mt6573_nand_ready_for_write(hw, page_addr, col_addr, false, NULL))
        {
            return -1;
        }
        
        if (!mt6573_nand_mcu_write_data(buf + sector * OOB_PER_SECTOR, write_len))
        {
            return -1;
        }
        
		(void)mt6573_nand_check_RW_count(write_len);
	    NFI_CLN_REG16(NFI_CON_REG16, CON_NFI_BWR);
        (void)mt6573_nand_set_command(NAND_CMD_PAGEPROG);
		
        while(DRV_Reg32(NFI_STA_REG32) & STA_NAND_BUSY);		
	
        status = mt6573_nand_wait();
        if (status & NAND_STATUS_FAIL)
        {
            printf( "status: %d\n", status);
            return -1;
        }

        len -= write_len;
        sector++;
    }

    return 0;
}


int mt6573_nand_block_markbad_hw(flashdev_info* flashdev, loff_t offset, unsigned long bmt_block)
{
    //struct nand_chip *chip = mtd->priv;
    int block = (int)offset / (flashdev->blocksize << KB_SHIFT);
    int page = block * ((flashdev->blocksize << KB_SHIFT) / flashdev->pagesize);
    int ret;

    u8 buf[8];
    memset(buf, 0xFF, 8);
	if(bmt_block){
		buf[BMT_BAD_BLOCK_INDEX_OFFSET] = 0;
	}
	else{
		if(flashdev->pagesize == 512){
			buf[6] = 0;
		}
		else{
	    	buf[0] = 0;
		}
	}

    ret = mt6573_nand_write_oob_raw(&mt6573_nand_hw, buf, page, 8);
    return ret;
}
#endif

int mt6573_nand_block_bad_hw(flashdev_info* flashdev, loff_t ofs, unsigned long bmt_block)
{
    //struct nand_chip *chip = (struct nand_chip *)mtd->priv;
    int block_size = flashdev->blocksize << KB_SHIFT;
    int block_addr = ((int)(ofs) / block_size ) * block_size;
	int page_addr = block_addr / flashdev->pagesize;
    unsigned int page_per_block = (block_size / flashdev->pagesize);


    unsigned char oob_buf[8];
    page_addr &= ~(page_per_block - 1);

    if (mt6573_nand_read_oob_raw(&mt6573_nand_hw, oob_buf, page_addr, sizeof(oob_buf)))
    {
        printf( "read_oob_raw return error\n");
        return 1;
    }

	//printf( "mt6573_nand_block_bad_hw:page_addr%x\n", page_addr);
	//printf( "%x, %x ,%x, %x ,%x, %x ,%x, %x \n", oob_buf[0],oob_buf[1],
	//	oob_buf[2],oob_buf[3],oob_buf[4], oob_buf[5],oob_buf[6],oob_buf[7]);

	if(bmt_block){
		if(oob_buf[BMT_BAD_BLOCK_INDEX_OFFSET] != 0xff){
			printf( "Bad block detected at page_addr 0x%x, oob_buf[%d] is 0x%x\n", page_addr, BMT_BAD_BLOCK_INDEX_OFFSET,oob_buf[BMT_BAD_BLOCK_INDEX_OFFSET]);
	        return 1;
 		}
	}
	else{
	 	if(flashdev->pagesize == 512){  //512B page size
	 		if( oob_buf[6] != 0xff){
		 		printf( "Bad block detected at page_addr 0x%x, oob_buf[6] is 0x%x\n", page_addr, oob_buf[6]);
		        return 1;
			}
	 	}
		else{
			if(oob_buf[0] != 0xff)
		    {
		        printf( "Bad block detected at 0x%x, oob_buf[0] is 0x%x\n", page_addr, oob_buf[0]);
		        return 1;
		    }
		}
	}


    return 0;        // everything is OK, good block
}

#endif
#if 0
static int mt6573_nand_block_bad(struct mtd_info *mtd, loff_t ofs)
{
    int chipnr = 0;
    
    struct nand_chip *chip = (struct nand_chip *)mtd->priv;
    int block = (int)ofs >> chip->phys_erase_shift;
    int mapped_block;

    int ret;



    mapped_block = get_mapping_block_index(block);

    ret = mt6573_nand_block_bad_hw(mtd, mapped_block << chip->phys_erase_shift);

    if (ret)
    {
        printf( "Unmapped bad block: 0x%x\n", mapped_block);
        if (update_bmt(mapped_block << chip->phys_erase_shift, UPDATE_UNMAPPED_BLOCK, NULL, NULL))
        {
            printf( "Update BMT success\n");
            ret = 0;
        }
        else
        {
            printf( "Update BMT fail\n");
            ret = 1;
        }
    }



    return ret;
}
#endif




/******************************************************************************
 * mt6573_nand_init_hw
 * 
 * DESCRIPTION:
 *   Initial NAND device hardware component ! 
 * 
 * PARAMETERS: 
 *   struct mt6573_nand_host *host (Initial setting data)
 * 
 * RETURNS: 
 *   None   
 * 
 * NOTES: 
 *   None
 *
 ******************************************************************************/
static void mt6573_nand_init_hw(struct mt6573_nand_host_hw *hw)
{
	//MSG(INIT, "Enable NFI Clock\n");
	//nand_enable_clock();

	g_bInitDone = false;

	/* Set default NFI access timing control */
	DRV_WriteReg32(NFI_ACCCON_REG32, hw->nfi_access_timing);
	DRV_WriteReg16(NFI_CNFG_REG16, 0);
	DRV_WriteReg16(NFI_PAGEFMT_REG16, 0);
	
	/* Reset the state machine and data FIFO, because flushing FIFO */
	(void)mt6573_nand_reset();
	
	/* Set the ECC engine */
	if(hw->nand_ecc_mode == NAND_ECC_HW)
	{
		//printf( "Use HW ECC\n");
		if(g_bHwEcc){
			NFI_SET_REG32(NFI_CNFG_REG16, CNFG_HW_ECC_EN);
		}
		//NFI_SET_REG32(NFI_CNFG_REG16, CNFG_PIO_BIG_ENDIAN);
		ECC_Config(hw);
   		mt6573_nand_configure_fdm(8);
		mt6573_nand_configure_lock();
	}

	/* Initilize interrupt. Clear interrupt, read clear. */
	DRV_Reg16(NFI_INTR_REG16);
	
	/* Interrupt arise when read data or program data to/from AHB is done. */
	DRV_WriteReg16(NFI_INTR_EN_REG16, 0);
}

static int mt6573_nand_dev_ready()
{	
    return !(DRV_Reg32(NFI_STA_REG32) & STA_NAND_BUSY);
}

/******************************************************************************
 *
 * Erase a block at a logical address
 *
 *****************************************************************************/
int mt6573_nand_erase_hw( int page){
	u8 status;

    	/* Send commands to erase a block */
	mt6573_nand_command_bp(NAND_CMD_ERASE1, -1, page);
	mt6573_nand_command_bp(NAND_CMD_ERASE2, -1, -1);

	status = mt6573_nand_wait();
	if (status & NAND_STATUS_FAIL){
		printf("mt6573_nand_erase_hw:fail,status=%d", status);
	    return -1;
	}
	else
	    return 0;    
}

#ifndef LZMA_IMG
static int
nandflash_write_internal(flashdev_info* flashdev, const unsigned char *buf, 
			unsigned long to, unsigned long datalen, unsigned long *retlen)
{
	int page;
	unsigned long len;
	int pagesize = flashdev->pagesize;
	int pagemask = (pagesize -1);
	int oobfreesize = pagesize >> 6; /* only oobfree no hwecc */
	int i, cnt;
	unsigned long addr = (unsigned long) to;

	#ifdef TCSUPPORT_NAND_BMT
    int physical_block;
    int logic_page;
    unsigned long addr_offset_in_block;
    unsigned long logic_addr = addr;
    unsigned short phy_block_bbt;
	#endif

	*retlen = 0;
	if (buf == 0) {
		return -1;
	}	
	

	// page write
	while (datalen > 0) {

		int len;
		int ret;
		unsigned long offs;
		cnt++;
		if ((cnt & 0x1ff) == 0)
			printf(".");

//		int ecc_en = 0;
#ifdef TCSUPPORT_NAND_BMT
		
		addr_offset_in_block = logic_addr % (flashdev->blocksize << KB_SHIFT);
		physical_block = get_mapping_block_index(logic_addr/(flashdev->blocksize << KB_SHIFT), &phy_block_bbt);
		addr = (physical_block*(flashdev->blocksize << KB_SHIFT)) + addr_offset_in_block;
#endif
		page = (int)((addr & ((flashdev->totalsize << MB_SHIFT)-1)) / flashdev->pagesize); //chip boundary

#ifdef TCSUPPORT_NAND_BMT
        logic_page = (int)((logic_addr & ((flashdev->totalsize << MB_SHIFT)-1)) / flashdev->pagesize); 
#endif

#if 0//def CONFIG_BADBLOCK_CHECK
		/*
		 * if we have a bad block, we shift a blocks !
		 */
		if(mt6573_nand_block_bad_hw(flashdev, addr, BAD_BLOCK_RAW)){
		
			printf("%s : skip writing a bad block %x -->", __func__, (unsigned int)addr);
			addr += flashdev->blocksize << KB_SHIFT;
			continue;
		}	
#endif
		// data write
		offs = addr & pagemask;
//		len = min_t(size_t, datalen, pagesize - offs);
		len = (datalen < (pagesize - offs)) ? datalen : (pagesize - offs);
		/* frankliao mark */
//		len = datalen;

		
		memset(fdm_buf, 0xff, sizeof(fdm_buf));
		
		if (buf && len > 0) {
			//FIXME, random write is not implemented
			memset(local_buffer_16_align, 0xff, pagesize);
			memcpy(local_buffer_16_align + offs, buf, len);	// we can not sure ops->buf wether is DMA-able.
			if(en_oob_write){	
				buf += len;
				datalen -= len;
				*retlen += len;	
				if(datalen < IMAGE_OOB_SIZE){
					printf("get oob buffer size err:datalen=%d!!\n",datalen);
					break;
				}
				/* copy oob free buffer, no hw ECC  */
				memcpy(fdm_buf, buf, oobfreesize);
			}
			#ifdef TCSUPPORT_NAND_BMT
			if(block_is_in_bmt_region(physical_block))
			{
                memcpy(fdm_buf + OOB_INDEX_OFFSET, &phy_block_bbt, OOB_INDEX_SIZE);
			}
			#endif
			if(en_oob_write){		
				buf += IMAGE_OOB_SIZE;				
				datalen -= IMAGE_OOB_SIZE;
				*retlen += IMAGE_OOB_SIZE;
			}else{
				buf += len;
				datalen -= len;
				*retlen += len;
			}
		}

		ret = mt6573_nand_exec_write_page(&mt6573_nand_hw, page, pagesize, (u8 *)local_buffer_16_align, fdm_buf);

		if (ret) {
			#ifdef TCSUPPORT_NAND_BMT
		    printf("write fail at page: %d \n", page);
            if (update_bmt(page * pagesize, 
                        UPDATE_WRITE_FAIL, local_buffer_16_align, fdm_buf))
            {
                printf("Update BMT success\n");
           
            }
            else
            {
                printf("Update BMT fail\n");
                return -1;
            }

			#else
			printf("Write Page Fail, Attempt to Write a Bad Block %x\n", (unsigned int)addr);
			return ret;
			#endif
		}


		addr = (page+1) * flashdev->pagesize;
		
#ifdef TCSUPPORT_NAND_BMT
        logic_addr = (logic_page + 1) * pagesize;
#endif
		
	}

	printf("program from %x to %x\n", to, (unsigned int)addr);
	
	return 0;
}
#endif

static int
nandflash_read_internal(flashdev_info* flashdev, unsigned char *buf, unsigned long from, 
						unsigned long datalen, unsigned long* retlen)
{
	int page, ret;
	int pagesize = flashdev->pagesize;
	int pagemask = (pagesize -1);
	int no_local_buffer = 0;

	unsigned long addr = from;
	unsigned char *buffer = NULL;

	*retlen = 0;

	#ifdef TCSUPPORT_NAND_BMT
    int physical_block;
    int logic_page;
    unsigned long addr_offset_in_block;
    unsigned long logic_addr = addr;
    unsigned short phy_block_bbt;
	#endif

	//printf("%s: addr:%x, datalen:%x \n", 
	//       __func__, addr, datalen);
	/* frankliao added 20101029 */
/*	block = addr >> ra->flash->erase_shift;
	tag = nand_bbt_get(ra, block);
	if (tag == BBT_TAG_BAD) {
		return -EFAULT;
	}
*/
	//prom_printf("nandflash_read addr:%x, datalen:%x \n",addr, datalen);

	if (buf == 0)
		return 0;
	if(datalen > 4096){ //for Dual Image Copy
		no_local_buffer = 1;
	}

	while (datalen) {

		unsigned long len, offs;

		//printf("%s : addr:%x, buf:%p, datalen:%x \n", 
		//       __func__, (unsigned int)addr, buf, datalen);
#ifdef TCSUPPORT_NAND_BMT
		
		addr_offset_in_block = logic_addr % (flashdev->blocksize << KB_SHIFT);
		physical_block = get_mapping_block_index(logic_addr/(flashdev->blocksize << KB_SHIFT), &phy_block_bbt);
		addr = (physical_block *(flashdev->blocksize << KB_SHIFT)) + addr_offset_in_block;
#endif

		page = (int)((addr & ((flashdev->totalsize << MB_SHIFT)-1)) / flashdev->pagesize); 

#ifdef TCSUPPORT_NAND_BMT
        logic_page = (int)((logic_addr & ((flashdev->totalsize << MB_SHIFT)-1)) / flashdev->pagesize); 
#endif

#if 0//def CONFIG_BADBLOCK_CHECK
		/*
		 * if we have a bad block, we shift a blocks !
		 */
		if(mt6573_nand_block_bad_hw(flashdev, addr, BAD_BLOCK_RAW)){
		
			printf("%s : skip read a bad block %x -->", __func__, (unsigned int)addr);
			addr += flashdev->blocksize << KB_SHIFT;
			continue;
		}	
			
#endif
		if(no_local_buffer && ((u32)buf % 16 == 0)){
			buffer = (unsigned char*)K0_TO_K1(buf+pagesize); //Take the next page room as a uncached buffer 
			ret = mt6573_nand_exec_read_page(&mt6573_nand_hw, page, pagesize, buffer, fdm_buf);
		}
		else{
		ret = mt6573_nand_exec_read_page(&mt6573_nand_hw, page, pagesize, local_buffer_16_align, fdm_buf);
		}
		#if 0
		if (ret) {
			printf("read fail:\n");
			return -1;
		}
		#endif
		ret = 0;
		
		// data read
		offs = addr & pagemask;
		/* frankliao mark */
		len = (datalen < (pagesize-offs)) ? datalen : (pagesize-offs);
		if (buf && len > 0) {
			if(!no_local_buffer){
				memcpy(buf, local_buffer_16_align + offs, len);
			}
			else{
				memcpy(buf, buffer + offs, len);
			}
			buf += len;
			datalen -= len;
			*retlen += len;
			if (ret) {
				return -1;
			}
		}
		addr = (page+1) * flashdev->pagesize;
		#ifdef TCSUPPORT_NAND_BMT
        logic_addr = (logic_page + 1) * flashdev->pagesize;
        #endif
	}
	
	return 0;
}



#ifndef LZMA_IMG
/**
 * nand_erase_internal - [Internal] erase block(s)
 * Erase one ore more blocks
 */
static int 
nandflash_erase_internal(flashdev_info* flashdev, unsigned long offset, unsigned long len)
{
	int status, ret;
	unsigned long page;
	unsigned int blocksize = flashdev->blocksize << KB_SHIFT;

	ret = 0;
	#ifdef TCSUPPORT_NAND_BMT
	int physical_block;
    int logic_page;
    unsigned long logic_addr = offset;
    unsigned short phy_block_bbt;
	#endif

	printf("%s: start:%x, len:%x \n", __func__, 
	       offset, (unsigned int)len);

#define BLOCK_ALIGNED(a) ((a) & (blocksize - 1))

	if (BLOCK_ALIGNED(len)) {
		len = (len/blocksize + 1) * blocksize;
	}

	if (BLOCK_ALIGNED(offset) || BLOCK_ALIGNED(len)) {
		printf("%s: erase block not aligned, addr:%x len:%x\n", __func__, offset, len);
		return -1;
	}

	while (len) {

#ifdef TCSUPPORT_NAND_BMT
		if(!en_oob_erase){ 
		/* en_oob_erase =1, donot find redirect block and force erase badblock flag and oob*/
		physical_block = get_mapping_block_index(logic_addr/blocksize, &phy_block_bbt);
        offset = (physical_block * blocksize);
		}
#endif
		page = (unsigned long)(offset / flashdev->pagesize);
		
#if 0//def CONFIG_BADBLOCK_CHECK
		/*
		 * if we have a bad block, we do not erase bad blocks !
		 */
		 if(mt6573_nand_block_bad_hw(flashdev, offset, BAD_BLOCK_RAW)){
		
			printf("nand_erase: attempt to erase a "
			       "bad block at 0x%x\n", offset);
			printf("%s: skip erasing a bad block\n", __func__);
			offset += blocksize;
			ret++;
			continue;
		}
			
#endif
		status = mt6573_nand_erase_hw(page);

		/* See if block erase succeeded */
		if (status) {
		#ifdef TCSUPPORT_NAND_BMT
			if(!en_oob_erase){
			if (update_bmt(offset,UPDATE_ERASE_FAIL, NULL, NULL))
	        {
	            printf("Erase fail at block: %d, update BMT success\n", offset/blocksize);
	        }
	        else
	        {
	            printf("Erase fail at block: %d, update BMT fail\n", offset/blocksize);
	            return -1;
	        }
			}
		#else
			printf("%s: failed erase, block 0x%08x\n", __func__, page);
			return -1;
		#endif
		}

		/* Increment page address and decrement length */
		len -= blocksize;
		offset += blocksize;
		#ifdef TCSUPPORT_NAND_BMT
		if(!en_oob_erase){
		logic_addr += blocksize;
		}
		#endif

	}

	return ret;
}
#endif

static int generic_ffs(int x)
{
	int r = 1;

	if (!x)
		return 0;
	if (!(x & 0xffff)) {
		x >>= 16;
		r += 16;
	}
	if (!(x & 0xff)) {
		x >>= 8;
		r += 8;
	}
	if (!(x & 0xf)) {
		x >>= 4;
		r += 4;
	}
	if (!(x & 3)) {
		x >>= 2;
		r += 2;
	}
	if (!(x & 1)) {
		x >>= 1;
		r += 1;
	}
	return r;
}
#ifdef TCSUPPORT_NAND_BMT
int calc_bmt_pool_size(struct ra_nand_chip *ra)
{
    int chip_size = 1 << ra->flash->chip_shift;
    int block_size = 1 << ra->flash->erase_shift;
    int total_block = chip_size / block_size;
    int last_block = total_block - 1;
    
    u16 valid_block_num = 0;
    u16 need_valid_block_num = total_block * POOL_GOOD_BLOCK_PERCENT;
#if 0
    printf("need_valid_block_num:%d \n", need_valid_block_num);
    printf("total block:%d \n", total_block);
#endif

	nand_flash_avalable_size = chip_size - MAX_BMT_SIZE * block_size;

	printf("\r\navalable block = %d\n", nand_flash_avalable_size / block_size);

    for(;last_block > 0; --last_block)
    {
        if(mt6573_nand_block_bad_hw(&devinfo, last_block * block_size, BAD_BLOCK_RAW))
        {
            continue;  
        }
        else
        {
            valid_block_num++;
            if(valid_block_num == need_valid_block_num)
            {
                break;
            }

        }

    }

    return (total_block - last_block);   
}
#endif


/************************************************************
 * the init/exit section.
 */
struct nand_info flashInfo;
int 
nandflash_init(int rom_base) 
{
	struct mt6573_nand_host_hw *hw;
	int manu_id, dev_id, id;	
	u8 ext_id1, ext_id2, ext_id3;	
    u32 ext_id;

/* OSBNB00043636: to Fix NAND DualImage , tclinux related MTD partition can't be programmed issue 20130313 Byron */
#if defined(TCSUPPORT_DUAL_IMAGE_ENHANCE)&& !defined(LZMA_IMG)
	u32 erase_unit;
#endif
#ifdef TCSUPPORT_NAND_COPY_BOB
	int prev_reservearea_flash = -1;
	int current_reservearea_flash = -1;
	unsigned long check_prev_reservearea_flash_offset = 133824512 ; //1021th block(total 1024) physical address  0x7fa0000
	unsigned long check_current_reservearea_flash_offset = 123207680 ; //940th block(total 1024)  physical address  0x7580000
#endif


	//struct nand_info flash;

	/* Allocate memory for 16 byte aligned buffer */
	local_buffer_16_align = local_buffer + 16 - ((u32)local_buffer % 16);
	//printf( "Allocate 16 byte aligned buffer: %p\n", local_buffer_16_align);
	hw = &mt6573_nand_hw;

	if(g_bUseAHBMode){
		//flush_dcache_range((unsigned long)local_buffer_16_align, (unsigned long)(local_buffer_16_align+4096));
		local_buffer_16_align = (u8 *)K0_TO_K1(local_buffer_16_align);
	}

	mt6573_nand_init_hw(hw);
	/* Select the device */
	//nand_chip->select_chip(mtd, 0);
	mt6573_nand_select_chip(0);

	/*
	 * Reset the chip, required by some chips (e.g. Micron MT29FxGxxxxx)
	 * after power-up
	 */
	mt6573_nand_command_bp(NAND_CMD_RESET, -1, -1);
	

	/* Send the command for reading device ID */
	mt6573_nand_command_bp(NAND_CMD_READID, 0x00, -1);
	


	/* Read manufacturer and device IDs */
	manu_id = mt6573_nand_read_byte();
	dev_id = mt6573_nand_read_byte();
	//printf("manu_id=%x, dev_id=%x\n", manu_id , dev_id);

	ext_id1 = mt6573_nand_read_byte();
	ext_id2 = mt6573_nand_read_byte();
	ext_id3 = mt6573_nand_read_byte();
	ext_id = ext_id1 << 16 | ext_id2 << 8 | ext_id3;

	//printf("ext_id=%x,\n", ext_id );

	//Check NAND Info
	// id = (dev_id<<8)|manu_id;
	id = dev_id | (manu_id << 8);
	if(!get_device_info(id, ext_id, &devinfo))
	{
       	printf("Not Support this NAND Device!(ID=%x)\r\n",id );
		return -1;
	}

	printf("NAND Page size:%dB,Total size %dMB \r\n",devinfo.pagesize,devinfo.totalsize);


	if (devinfo.pagesize == 4096) {
		//nand_chip->ecc.layout = &nand_oob_128;
		//nand_chip->ecc.size = 4096;
		//nand_chip->ecc.bytes = 64;
		hw->nand_ecc_size = 4096;
		hw->nand_ecc_bytes = 64;
#if defined(TCSUPPORT_CT_PON)
		reservearea_size = 0x40000;
#else
/* OSBNB00043636: to Fix NAND DualImage , tclinux related MTD partition can't be programmed issue 20130313 Byron */
/* to fix compiler error , if MT7510 enable NAND & DualImage support */
#if defined (TCSUPPORT_GPON_DUAL_IMAGE) || defined (TCSUPPORT_EPON_DUAL_IMAGE)
		reservearea_size = NAND_FLASH_BLOCK_SIZE;
	#endif
#endif
		NFI_SET_REG16(NFI_PAGEFMT_REG16, (PAGEFMT_SPARE_16 << PAGEFMT_SPARE_SHIFT) | PAGEFMT_4K);
	} else if (devinfo.pagesize == 2048) {
		//nand_chip->ecc.layout = &nand_oob_64;
		//nand_chip->ecc.size = 2048;
		//nand_chip->ecc.bytes = 32;
		hw->nand_ecc_size = 2048;
		hw->nand_ecc_bytes = 32;
#if defined(TCSUPPORT_CT_PON)
		reservearea_size = 0x40000;
#else
/* OSBNB00043636: to Fix NAND DualImage , tclinux related MTD partition can't be programmed issue 20130313 Byron */
/* to fix compiler error , if MT7510 enable NAND & DualImage support */
#if defined (TCSUPPORT_GPON_DUAL_IMAGE) || defined (TCSUPPORT_EPON_DUAL_IMAGE)
		reservearea_size = NAND_FLASH_BLOCK_SIZE;
	#endif
#endif
		NFI_SET_REG16(NFI_PAGEFMT_REG16, (PAGEFMT_SPARE_16 << PAGEFMT_SPARE_SHIFT) | PAGEFMT_2K);
	} else if (devinfo.pagesize == 512) {
		//nand_chip->ecc.layout = &nand_oob_16;
		//nand_chip->ecc.size = 512;
		//nand_chip->ecc.bytes = 8;
		hw->nand_ecc_size = 512;
		hw->nand_ecc_bytes = 8;
		NFI_SET_REG16(NFI_PAGEFMT_REG16, (PAGEFMT_SPARE_16 << PAGEFMT_SPARE_SHIFT) | PAGEFMT_512);
	}
	
    hw->nfi_bus_width = devinfo.iowidth;
	DRV_WriteReg32(NFI_ACCCON_REG32, devinfo.timmingsetting);

	if (hw->nfi_bus_width == 16)
	{
		NFI_SET_REG16(NFI_PAGEFMT_REG16, PAGEFMT_DBYTE_EN);
	}
	mt6573_nand_select_chip(0);

	//Update the ra interface for other function
	memset((void*) &ra, 0, sizeof(struct ra_nand_chip));
	memset((void*) &flashInfo, 0, sizeof(struct nand_info));

	flashInfo.chip_shift = generic_ffs(devinfo.totalsize << MB_SHIFT) - 1;
	flashInfo.erase_shift = generic_ffs(devinfo.blocksize << KB_SHIFT) - 1;
	flashInfo.page_shift = generic_ffs(devinfo.pagesize) - 1;
	flashInfo.oob_shift = generic_ffs(hw->nand_ecc_bytes*2) - 1;

	//printf("chip_shift:%d, erase_shift:%d,page_shift:%d\n",flash.chip_shift, flash.erase_shift, flash.page_shift);

	ra.flash = &flashInfo;
	#if defined(TCSUPPORT_DUAL_IMAGE_ENHANCE) && !defined(LZMA_IMG)
	offset = (1 << ra.flash->chip_shift) / 2;
	//printf("nand flash offset: %x\n", offset);
	#endif

	
#ifdef TCSUPPORT_NAND_BMT
		bmt_pool_size = calc_bmt_pool_size(&ra);
		printf("bmt pool size: %d \n", bmt_pool_size);
		
		if (!g_bmt)
		{
			if ( !(g_bmt = init_bmt(&ra, bmt_pool_size)) )
			{
				printf("Error: init bmt failed \n");
				return -1;
			}
		}
	
		if (!g_bbt)
		{
			if ( !(g_bbt = start_init_bbt()) )
			{
				printf("Error: init bbt failed \n");
				return -1;
			}
		}
	
		if(write_bbt_or_bmt_to_flash() != 0)
		{
			printf("Error: save bbt or bmt to nand failed \n");
			return -1;
		}
		
		if(create_badblock_table_by_bbt())
		{
			printf("Error: create bad block table failed \n");
			return -1;
		}
	#if defined(TCSUPPORT_DUAL_IMAGE_ENHANCE) && !defined(LZMA_IMG)
		offset = nand_logic_size / 2;
/* OSBNB00043636: to Fix NAND DualImage , tclinux related MTD partition can't be programmed issue 20130313 Byron */
/* to fix compiler error , if MT7510 enable NAND & DualImage support */
#if 1
		erase_unit=devinfo.blocksize;
		erase_unit=erase_unit<<10;
		offset=(offset/(erase_unit)*(erase_unit));
		printf("nand_logic_size: 0x%x , block_size:0x%x  ,offset change as 0x%x \n",nand_logic_size,erase_unit,offset);
#else
		offset = (offset/NAND_FLASH_BLOCK_SIZE)*NAND_FLASH_BLOCK_SIZE; //make sure the offset is on the edge of a block
		printf("BMT bootloader nand flash offset: %x\n", offset);
#endif
	#endif	
		
#endif

#ifdef TCSUPPORT_NAND_COPY_BOB
	/*check the previous version: whether the reservearea is empty or not*/
	prev_reservearea_flash = check_BMT_reservearea_empty_or_not(check_prev_reservearea_flash_offset);
	/*check the current version: whether the reservearea is empty or not*/
	current_reservearea_flash = check_BMT_reservearea_empty_or_not(check_current_reservearea_flash_offset);
	if((prev_reservearea_flash == 1)&&(current_reservearea_flash == 0)){
		printf("\nstart coping flash data!\n");
		CopyFlashData();
	}
#endif

	return 0;
}

#ifndef LZMA_IMG
int 
nandflash_erase(unsigned long offset, unsigned long len)
{
	int rtn_status;
	
	local_irq_disable();	/*Chuck Kuo*/
	rtn_status = nandflash_erase_internal(&devinfo, offset, len);
	local_irq_enable();		/*Chuck Kuo*/
	return rtn_status;
}
#endif

int nandflash_read(unsigned long from,
	unsigned long len, unsigned long *retlen, unsigned char *buf)
{
	int rtn_status;

	local_irq_disable();	/*Chuck Kuo*/
	rtn_status = nandflash_read_internal(&devinfo, buf, from, len, retlen);
	local_irq_enable();		/*Chuck Kuo*/
	return rtn_status;
}

#ifndef LZMA_IMG

int nandflash_write(unsigned long to,
	unsigned long len, unsigned long *retlen, const unsigned char *buf)
{
	int rtn_status;

	local_irq_disable();	/*Chuck Kuo*/
	rtn_status = nandflash_write_internal(&devinfo, buf, to, len, retlen);
	local_irq_enable();		/*Chuck Kuo*/
	return rtn_status;
	
}
#endif

#define NAND_BUFFER_SIZE     	4096
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
	if (ret != 0){	
		 return -1;
	}	

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






