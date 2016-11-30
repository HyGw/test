#include "skbuff.h"
#include "eth.h"
#include "ip.h"
#include "udp.h"
#include "arp.h"
#include "icmp.h"

#include "ipv4/lwip/ip_addr.h"
#include "lwip/err.h"
#include "lwip/pbuf.h"
#include "lwip/tcp.h"
#include "lwip/opt.h"
#include "linux/stddef.h"

#include "osdep.h"
#include "common.h"

#ifdef TCSUPPORT_NAND_RT63368
extern int nand_logic_size;
#endif

#if defined(TCSUPPORT_MULTI_UPGRADE) || defined(TCSUPPORT_MULTI_BOOT)
#include "../../../tools/trx/trx.h"
#include <asm/tc3162.h>
#include <spiflash.h>
#include <nandflash.h>
#include "../include/flashhal.h"
#include "tftp.h"
#include "mac.h"

#if !defined(TCSUPPORT_CT_WAN_PTM)
#define DBG_PRINT
#endif

#define MULTI_BUF_BASE					0x80020000
#define MICONF_OFFSET				0xff00
#define MICONF_SIZE					0x100
#define DSL_GPIO_DEFAULT 6
#define INTERNET_GPIO_DEFAULT 7
struct mtd_info {
	unsigned long offset; /* Offset within the memory */
	unsigned long size;   /* Total size of the MTD */
	unsigned long erasesize;
};

extern struct mtd_info mtd;
#ifdef TCSUPPORT_BB_NAND
extern struct ra_nand_chip ra;
#endif
#endif

#if defined(TCSUPPORT_MULTI_UPGRADE)
#if !defined(TCSUPPORT_C1_TRUE)
#define BACKUPRF_TO_BOTTOM_2         0x20000
#define BACKUPRF_TO_BOTTOM_3         0x30000
#define BACKUPRF_TO_BOTTOM_4		0x40000
#define BACKUPRF_TO_BOTTOM_7		0x70000
#endif
#endif

#define RESERVEAREA_BLOCK_BASE 0
/*backupromfile*/
#define BACKUPROMFILE_RA_SIZE 0x10000
#define BACKUPROMFILE_RA_OFFSET RESERVEAREA_BLOCK_BASE
/*defaultromfile*/
#define DEFAULTROMFILE_RA_SIZE 0x10000
#define DEFAULTROMFILE_RA_OFFSET (BACKUPROMFILE_RA_OFFSET+BACKUPROMFILE_RA_SIZE)
/*syslog*/
#define SYSLOG_RA_SIZE 0x10000
#define SYSLOG_RA_OFFSET (DEFAULTROMFILE_RA_OFFSET+DEFAULTROMFILE_RA_SIZE)
/*product para*/
#define PROLINE_CWMPPARA_RA_SIZE 0x10000
#define PROLINE_CWMPPARA_RA_OFFSET (SYSLOG_RA_OFFSET+SYSLOG_RA_SIZE)
/*temp data*/
#define TEMP_RA_SIZE 0x10000
#define TEMP_RA_OFFSET (PROLINE_CWMPPARA_RA_OFFSET+PROLINE_CWMPPARA_RA_SIZE)

#if defined(TCSUPPORT_MULTI_BOOT)
#define MULTIBOOT_SIG_OFFSET 			8
#define MULTIBOOT_SIG					"zyx"
#define MULTIBOOT_PACKET_SIZE 			1024

#define TRX_LEN	256
#define ROMD_TO_BOTTOM			0x50000

#define CLEAR_ROMFILE				0x2
#define CLEAR_ROMD					0x4
#define CUR_FILE_MASK				0xFF00
#define CURR_FILE_FLAG_BOOT	 	0x100
#define CURR_FILE_FLAG_ROMFILE		0x200
#define CURR_FILE_FLAG_TCLINUX		0x400
#define CURR_FILE_FLAG_ROMD		0x800
#define ROMFILE_SIZE				0x10000


struct MULTI_BOOT_T{
	unsigned char		signature[4];
	unsigned short	checkSum;
	unsigned int		ID;
	unsigned int		dataLen;
	unsigned int		fileLen;
	unsigned int		fileFlag;
	unsigned short	countryCode;
	unsigned char		debugFlag;
	unsigned char		reserve1;
	unsigned int		reserve2;
	unsigned char		data[MULTIBOOT_PACKET_SIZE];
}  __attribute__ ((packed));

unsigned int rcv_len = 0;
volatile char finishMultiBoot = 0;
unsigned int cur_file_type = 0;
volatile char startMultiBoot= 0;
#endif












#if defined(TCSUPPORT_MULTI_UPGRADE)
/*
	pkt format is shown as below:		
	byte 0~1 	pkt type		
	byte 2~3 	pkt sequence
	byte 4~11	pkt tag
	byte 12~15	total length
	byte 16~19	CRC of firmware fragment
	byte 20~23	CRC of whole firmware 
	1024bytes	data
*/
#define MULTI_UPGRADE_PKT_TAG		"TCBulkFW"
#define MULTIPKT_TAG_OFFSET 			12
#define LAST_MULTIPKT					0x40
#define NOT_LAST_MULTIPKT				0x20
#define IMG_LEN_OFFSET					20
#define LT_OFFSET						8
#define CRC_OFFSET						28
#define MULTI_UPGRADE_PKT_HEADLEN 	32
#define MULTI_UPGRADE_DATA_LEN 		1024

unsigned long receive_len = 0;
char startMultiUpgrade = 0;
#define FLASH_ERASE_SIZE			0x10000
extern int nand_logic_size;
extern int nand_flash_avalable_size;
extern u32 reservearea_size;
#ifdef TCSUPPORT_BB_NAND
#ifdef TCSUPPORT_MTD_PARTITIONS_CMDLINE
#define ROMFILE_SIZE				ecnt_get_romfile_size()
#else
#ifdef TCSUPPORT_NAND_BMT
#define ROMFILE_SIZE				0x40000
#else
#if defined(TCSUPPORT_CT_PON)
#define ROMFILE_SIZE				0x40000
#else
#define ROMFILE_SIZE				0x10000
#endif
#endif
#endif
#if (TCSUPPORT_RESERVEAREA_BLOCK==7)
#if defined(TCSUPPORT_CT_PON)
#define RESERVEAREA_BLOCK_BASE 0
/*backupromfile*/
#define BACKUPROMFILE_RA_SIZE 0x40000
#define BACKUPROMFILE_RA_OFFSET RESERVEAREA_BLOCK_BASE
/*defaultromfile*/
#define DEFAULTROMFILE_RA_SIZE 0x40000
#define DEFAULTROMFILE_RA_OFFSET (BACKUPROMFILE_RA_OFFSET+BACKUPROMFILE_RA_SIZE)
/*syslog*/
#define SYSLOG_RA_SIZE 0x40000
#define SYSLOG_RA_OFFSET (DEFAULTROMFILE_RA_OFFSET+DEFAULTROMFILE_RA_SIZE)
/*product para*/
#define PROLINE_CWMPPARA_RA_SIZE 0x40000
#define PROLINE_CWMPPARA_RA_OFFSET (SYSLOG_RA_OFFSET+SYSLOG_RA_SIZE)
/*temp data*/
#define TEMP_RA_SIZE 0x40000
#define TEMP_RA_OFFSET (PROLINE_CWMPPARA_RA_OFFSET+PROLINE_CWMPPARA_RA_SIZE)
#endif
#endif
#else
#ifdef TCSUPPORT_MTD_PARTITIONS_CMDLINE
#define ROMFILE_SIZE				ecnt_get_romfile_size()
#else
#define ROMFILE_SIZE				0x10000
#endif
#if (TCSUPPORT_RESERVEAREA_BLOCK==7)
#if defined(TCSUPPORT_CT_PON)
#define RESERVEAREA_BLOCK_BASE 0
/*backupromfile*/
#define BACKUPROMFILE_RA_SIZE 0x10000
#define BACKUPROMFILE_RA_OFFSET RESERVEAREA_BLOCK_BASE
/*defaultromfile*/
#define DEFAULTROMFILE_RA_SIZE 0x10000
#define DEFAULTROMFILE_RA_OFFSET (BACKUPROMFILE_RA_OFFSET+BACKUPROMFILE_RA_SIZE)
/*syslog*/
#define SYSLOG_RA_SIZE 0x10000
#define SYSLOG_RA_OFFSET (DEFAULTROMFILE_RA_OFFSET+DEFAULTROMFILE_RA_SIZE)
/*product para*/
#define PROLINE_CWMPPARA_RA_SIZE 0x10000
#define PROLINE_CWMPPARA_RA_OFFSET (SYSLOG_RA_OFFSET+SYSLOG_RA_SIZE)
/*temp data*/
#define TEMP_RA_SIZE 0x10000
#define TEMP_RA_OFFSET (PROLINE_CWMPPARA_RA_OFFSET+PROLINE_CWMPPARA_RA_SIZE)
#endif
#endif
#endif
#if !defined(TCSUPPORT_C1_TRUE)
volatile char finishMultiUpgrade = 0;
int maxMultiLedNum = 16;
#endif

extern unsigned long Jiffies;

#if defined(TCSUPPORT_MULTI_UPGRADE_LIONS)
/*according to the structure of map defined in mic.c*/
typedef struct _mrdInfoLinux_s{
	unsigned char SDRAM_CONF_array[4];
	unsigned char VENDOR_NAME[32];
	unsigned char PRODUCT_NAME[32];
	unsigned char FUNC_BYTE[4];
	unsigned char ETHER_ADDR[6];
	unsigned char SNMP_SYSOBJID_array[22];
	unsigned char COUNTRY_array;
	unsigned char ether_gpio;
	unsigned char power_gpio[2];
}mrdInfoLinux_t;

#define LIONS_ETHER_ADDR_OFFSET			0x5ff8
#define TC_FLASH_READ_CMD		"/userfs/bin/mtd readflash %s %lu %lu %s"
#define LINOSALLINONE_SIG_OFFSET	38
#define LINOSALLINONE_LEN_OFFSET	42
#define LINOSALLINONE_CHKSUM_OFFSET	52
#define LINOSALLINONE_HEADER	51
#define SIG_LINOSALLINONE	"LOA"
#define SIG_LEN	3
#define LINOSALLINONE_FIRST_PKT_HEADLEN		83
#define LINOSALLINONE_FIRST_DATA_LEN	973

static uint8  odd_flag = 0;
static uint8 isLinosAllinone = 0;
static unsigned short mutAllinoneChksum = 0;
static unsigned long mutAllinoneSize = 0;
static uint32 ChkSum = 0;

void linosAllinoneChkSumCalc(uint8 *start_p, uint32 len)
{
	uint32 i;
	uint16 word;
	
	for( i=0; i<len; i++ )
	{
		word = start_p[i];
		if( (i&0x01) == odd_flag )
		    ChkSum += (word << 8)&0xff00 ;
		else
		    ChkSum += word&0x00ff;
	    if (ChkSum > 0xFFFF)
	    	ChkSum = (ChkSum + 1) & 0xFFFF;	
	}	
	if( (receive_len&0x01) == 1 )
		odd_flag = 1;
	else
		odd_flag = 0;
	
}

void restore_linux_info(char *buf)
{
	mrdInfoLinux_t mrd_u;
	unsigned char mic_buf[256] = {0};
	unsigned long retlen;
	
	memset(&mrd_u,0,sizeof(mrd_u));
	flash_read(TCBOOT_BASE+MICONF_OFFSET, MICONF_SIZE, &retlen, mic_buf);
	mrd_u = *(mrdInfoLinux_t *)mic_buf;
	
	/*only restore Linux mac addr. Other parameters in Linux mrd can be update as below. */
	memcpy(buf+LIONS_ETHER_ADDR_OFFSET, mrd_u.ETHER_ADDR, 6);
	
	return;
}
#endif

void MultiWriteImage(char *ptr, unsigned long datalen)
{
	struct trx_header *trx = (struct trx_header *)ptr;
	unsigned long retlen;
	int cal_check_sum = 0;
	unsigned int romfile_offset = 0;
	unsigned int tclinux_size = 0;
#if !defined(TCSUPPORT_C1_TRUE)
	unsigned int backuprf_offset = 0;
#endif
	/*Get the Internet GPIO Number*/
//	uint8 internet_gpio = *(uint8 *)(INTERNET_GPIO);
	uint8 internet_gpio = READ_FLASH_BYTE(INTERNET_GPIO);
	/*Get the DSL GPIO Number*/
//	uint8 dsl_gpio = *(uint8 *)(ADSL_GPIO);
	uint8 dsl_gpio = READ_FLASH_BYTE(ADSL_GPIO);
	if(dsl_gpio == 0)
		dsl_gpio = DSL_GPIO_DEFAULT;
	if(internet_gpio == 0){
		if(isEN7526D || isEN7526G || isEN7513 || isEN7513G || isEN7521G || isEN7586)
			internet_gpio = 28;
		else if(isEN7526F || isEN7521F || isEN7521S || isEN7512)
			internet_gpio = 4;
		else
			internet_gpio = INTERNET_GPIO_DEFAULT;
	}
#if !defined(TCSUPPORT_C1_TRUE)
	if (IS_SPIFLASH) {
		#if (TCSUPPORT_RESERVEAREA_BLOCK==2)
		backuprf_offset = mtd.size - BACKUPRF_TO_BOTTOM_2;
		#elif (TCSUPPORT_RESERVEAREA_BLOCK==3)
		backuprf_offset = mtd.size - BACKUPRF_TO_BOTTOM_3;
		#elif (TCSUPPORT_RESERVEAREA_BLOCK==4)
		backuprf_offset = mtd.size - BACKUPRF_TO_BOTTOM_4;
		#elif (TCSUPPORT_RESERVEAREA_BLOCK==7)
		backuprf_offset = mtd.size - BACKUPRF_TO_BOTTOM_7;
		#else
		#endif
	} else if (IS_NANDFLASH) {
#ifdef TCSUPPORT_BB_NAND	
		#if (TCSUPPORT_RESERVEAREA_BLOCK==2)
		    #ifdef TCSUPPORT_NAND_RT63368
            backuprf_offset = nand_logic_size - 2 * (1<<ra.flash->erase_shift);
		    #else
		backuprf_offset = (1<<ra.flash->chip_shift) - 2 * (1<<ra.flash->erase_shift);
		    #endif
		#elif (TCSUPPORT_RESERVEAREA_BLOCK==3)
		    #ifdef TCSUPPORT_NAND_RT63368
            backuprf_offset = nand_logic_size - 3 * (1<<ra.flash->erase_shift);
		    #else
		backuprf_offset = (1<<ra.flash->chip_shift) - 3 * (1<<ra.flash->erase_shift);
		    #endif
		#elif (TCSUPPORT_RESERVEAREA_BLOCK==4)
		    #ifdef TCSUPPORT_NAND_RT63368
            backuprf_offset = nand_logic_size - 4 * (1<<ra.flash->erase_shift);
		    #else
			backuprf_offset = (1<<ra.flash->chip_shift) - 4 * (1<<ra.flash->erase_shift);
			//backuprf_offset = nand_logic_size - 4 * reservearea_size;		
		    #endif
		#elif (TCSUPPORT_RESERVEAREA_BLOCK==5)
		    #ifdef TCSUPPORT_NAND_RT63368
            backuprf_offset = nand_logic_size - 5 * (1<<ra.flash->erase_shift);
		    #else
			backuprf_offset = (1<<ra.flash->chip_shift) - 5 * (1<<ra.flash->erase_shift);
			//backuprf_offset = nand_logic_size - 5 * reservearea_size;
		    #endif
		#elif (TCSUPPORT_RESERVEAREA_BLOCK==7)
		    #ifdef TCSUPPORT_NAND_RT63368
            backuprf_offset = nand_logic_size - 7 * (1<<ra.flash->erase_shift);
		    #else
			//backuprf_offset = nand_logic_size - 7 * reservearea_size;
			backuprf_offset = nand_flash_avalable_size - 7 * reservearea_size;
		    #endif
		#else
		#endif

		#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
        backuprf_offset = TCBKROMFILE_START << ra.flash->erase_shift;
		#endif
#endif		
	}
#endif
	/*
		turn off ADSL LED and turn on Internet LED
	*/
	LED_ON(dsl_gpio);
	LED_ON(internet_gpio);
#if defined(TCSUPPORT_MULTI_UPGRADE_LIONS)
	if(isLinosAllinone == 1){
		prom_printf("Linos allinone file\n");
		prom_printf("Write to flash from %X to %X with %X bytes\n", MULTI_BUF_BASE, TCBOOT_BASE, datalen);
		prom_printf(" write bootbase \n");
		flash_erase(TCBOOT_BASE, TCBOOT_SIZE);
		prom_printf("\n");
		flash_write(TCBOOT_BASE, TCBOOT_SIZE, &retlen, (const unsigned char *) (MULTI_BUF_BASE));
		prom_printf(" write ras \n");
		flash_erase(0x10000,datalen-TCBOOT_SIZE);
		prom_printf("\n");
		flash_write(0x10000, datalen-TCBOOT_SIZE, &retlen, (const unsigned char *) (MULTI_BUF_BASE+TCBOOT_SIZE));
	}
	else{
#endif
	if(trx->magic == TRX_MAGIC2){
		prom_printf("bin file\n");
		#ifdef DBG_PRINT
		prom_printf("Write to flash from %X to %X with %X bytes\n", MULTI_BUF_BASE, flash_tclinux_start, datalen);
		#endif
#if !defined(TCSUPPORT_C1_TRUE)
		if (IS_SPIFLASH){
			flash_erase((TCBOOT_BASE+TCBOOT_SIZE), ROMFILE_SIZE);//erase current romfile for reset with default setting
		} else if (IS_NANDFLASH){
#ifdef TCSUPPORT_BB_NAND		
			if (ra_inl(CR_AHB_HWCONF) & (0x00080000)) {
			#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
			    flash_erase(TCBOOT_BASE + 7 * (1<<ra.flash->erase_shift), ROMFILE_SIZE);
			#elif defined(TCSUPPORT_NAND_RT63368)    
			    flash_erase(TCBOOT_BASE + (1<<ra.flash->erase_shift), ROMFILE_SIZE);
			#else
			#ifdef TCSUPPORT_BB_NAND
			#ifdef TCSUPPORT_NAND_BMT
				flash_erase((TCBOOT_BASE+TCBOOT_SIZE), ROMFILE_SIZE);//erase current romfile for reset with default setting
			#else
#if defined(TCSUPPORT_CT_PON)
				flash_erase((TCBOOT_BASE+TCBOOT_SIZE), ROMFILE_SIZE);//erase current romfile for reset with default setting
#else
				flash_erase((1<<ra.flash->chip_shift) - 6 * (1<<ra.flash->erase_shift), ROMFILE_SIZE);
#endif
			#endif
			#else
				flash_erase((1<<ra.flash->chip_shift) - 6 * (1<<ra.flash->erase_shift), ROMFILE_SIZE);
			#endif
			#endif
			} else {
				flash_erase((TCBOOT_BASE+TCBOOT_SIZE), ROMFILE_SIZE);
			}
#endif			
		}
#if !defined(TCSUPPORT_CT_PON_CN)
		#if (TCSUPPORT_RESERVEAREA_BLOCK==2 || TCSUPPORT_RESERVEAREA_BLOCK==3 || TCSUPPORT_RESERVEAREA_BLOCK==4 || TCSUPPORT_RESERVEAREA_BLOCK==5 || TCSUPPORT_RESERVEAREA_BLOCK==7)
		flash_erase(backuprf_offset, ROMFILE_SIZE);//erase backup romfile for reset with default setting
#if defined(TCSUPPORT_C7)
		flash_erase(backuprf_offset+ROMFILE_SIZE, ROMFILE_SIZE);//erase default romfile for reset with default setting
#endif
		#endif
		#if (TCSUPPORT_RESERVEAREA_BLOCK==7)
#if defined(TCSUPPORT_CT_PON)
		flash_erase(backuprf_offset+TEMP_RA_OFFSET, TEMP_RA_SIZE);//erase temp area data.
#endif
		#endif
#endif
#endif

      #ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
        flash_erase(TCBOOT_BASE + 20 * (1<<ra.flash->erase_shift), datalen);
        prom_printf("\n");
        flash_write(TCBOOT_BASE + 20 * (1<<ra.flash->erase_shift), datalen, &retlen, (const unsigned char *) (MULTI_BUF_BASE));
      #elif defined(TCSUPPORT_NAND_RT63368)
        flash_erase(TCBOOT_BASE + 2 * (1<<ra.flash->erase_shift), datalen);
        prom_printf("\n");
        flash_write(TCBOOT_BASE + 2 * (1<<ra.flash->erase_shift), datalen, &retlen, (const unsigned char *) (MULTI_BUF_BASE));
      #else
		flash_erase(flash_tclinux_start, datalen);
		prom_printf("\n");
		flash_write(flash_tclinux_start, datalen, &retlen, (const unsigned char *) (MULTI_BUF_BASE));
      #endif
		//flash_write();
	}else{
		prom_printf("all_in_one file\n");
		#ifdef DBG_PRINT
		prom_printf("bin file\n");
		prom_printf("Write to flash from 0x%X to 0x%X with 0x%X bytes\n", MULTI_BUF_BASE, TCBOOT_BASE, datalen);
		//flash_erase(TCBOOT_BASE, datalen);
		//flash_write(TCBOOT_BASE, datalen, &retlen, (const unsigned char *) (MULTI_BUF_BASE));
		prom_printf(" write tcboot.bin \n");
		#endif
		flash_erase(TCBOOT_BASE, TCBOOT_SIZE);
		prom_printf("\n");
		flash_write(TCBOOT_BASE, TCBOOT_SIZE, &retlen, (const unsigned char *) (MULTI_BUF_BASE));
		#ifdef DBG_PRINT
		prom_printf(" write romfile.cfg \n");
		#endif
		if (IS_SPIFLASH){
			romfile_offset = TCBOOT_BASE+TCBOOT_SIZE;
		} else if (IS_NANDFLASH){
#ifdef TCSUPPORT_BB_NAND		
			if (ra_inl(CR_AHB_HWCONF) & (0x00080000)) {
			#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
			    romfile_offset = TCBOOT_BASE + 7 * (1<<ra.flash->erase_shift);
			#elif defined(TCSUPPORT_NAND_RT63368)
			    romfile_offset = TCBOOT_BASE + (1<<ra.flash->erase_shift);
			#else
			#ifdef TCSUPPORT_BB_NAND
			#ifdef TCSUPPORT_NAND_BMT
				romfile_offset = TCBOOT_BASE+TCBOOT_SIZE;
			#else
#if defined(TCSUPPORT_CT_PON)
				romfile_offset = TCBOOT_BASE+TCBOOT_SIZE;
#else
				romfile_offset = (1<<ra.flash->chip_shift) - 6 * (1<<ra.flash->erase_shift);
#endif
			#endif
			#else
				romfile_offset = (1<<ra.flash->chip_shift) - 6 * (1<<ra.flash->erase_shift);
			#endif
			#endif
			} else {
				romfile_offset = TCBOOT_BASE+TCBOOT_SIZE;
			}
#endif			
		}
		
		flash_erase(romfile_offset, ROMFILE_SIZE);
		prom_printf("\n");		
#if defined(TCSUPPORT_CT)
#if defined(TCSUPPORT_CY)
		#if (TCSUPPORT_RESERVEAREA_BLOCK==2 || TCSUPPORT_RESERVEAREA_BLOCK==3 || TCSUPPORT_RESERVEAREA_BLOCK==4 || TCSUPPORT_RESERVEAREA_BLOCK==7)
		flash_erase(backuprf_offset, ROMFILE_SIZE);//erase backup romfile for reset with default setting
		prom_printf("\n");
                #endif
#endif
#if defined(TCSUPPORT_C7)
		flash_write(romfile_offset, ROMFILE_SIZE, &retlen, (const unsigned char *) (MULTI_BUF_BASE+TCBOOT_SIZE));
#endif
		#if (TCSUPPORT_RESERVEAREA_BLOCK==7)
#if defined(TCSUPPORT_CT_PON)
		flash_erase(backuprf_offset+BACKUPROMFILE_RA_OFFSET, BACKUPROMFILE_RA_SIZE);//erase backup romfile.
		flash_erase(backuprf_offset+TEMP_RA_OFFSET, TEMP_RA_SIZE);//erase temp area data.
#endif
		#endif
#else
		flash_write(romfile_offset, ROMFILE_SIZE, &retlen, (const unsigned char *) (MULTI_BUF_BASE+TCBOOT_SIZE));
#endif
		#ifdef DBG_PRINT		
		prom_printf(" write tclinux.bin \n");
		#endif
	#ifdef 	TCSUPPORT_NAND_BADBLOCK_CHECK
        flash_erase(TCBOOT_BASE + 20 * (1<<ra.flash->erase_shift), datalen-0x20000);
        prom_printf("\n");
        flash_write(TCBOOT_BASE + 20 * (1<<ra.flash->erase_shift), datalen-0x20000, &retlen, (const unsigned char *) (MULTI_BUF_BASE+flash_tclinux_start));
    #elif defined(TCSUPPORT_NAND_RT63368)
        flash_erase(TCBOOT_BASE + 2 * (1<<ra.flash->erase_shift), datalen-0x20000);
        prom_printf("\n");
        flash_write(TCBOOT_BASE + 2 * (1<<ra.flash->erase_shift), datalen-0x20000, &retlen, (const unsigned char *) (MULTI_BUF_BASE+flash_tclinux_start));
	#else
/* should modify for general sun.zhu_20140503 */
		flash_erase(TCBOOT_BASE+TCBOOT_SIZE+ROMFILE_SIZE, datalen-TCBOOT_SIZE-ROMFILE_SIZE);
		prom_printf("\n");
		flash_write(TCBOOT_BASE+TCBOOT_SIZE+ROMFILE_SIZE, datalen-TCBOOT_SIZE-ROMFILE_SIZE, &retlen, (const unsigned char *) (MULTI_BUF_BASE+flash_tclinux_start));
	#endif
		}
#if defined(TCSUPPORT_MULTI_UPGRADE_LIONS)
		}
#endif	
	/*
		turn on ADSL and Internet LED
	*/	
	
	#ifdef DBG_PRINT
	LED_ON(dsl_gpio);
	LED_OFF(internet_gpio);	
	#endif
}


void
MultiUpgradeHandle(sk_buff *skb)
{
	unsigned long LenAndType = swap32(*(unsigned long *)(skb->data + LT_OFFSET));
	unsigned long pktseq = ((LenAndType & 0xffff0000)>>16);
	unsigned long imagelen = swap32(*(unsigned long *)(skb->data + IMG_LEN_OFFSET));
	unsigned char ret, pkt_type, last_packet = 0;
	static unsigned long prevImgLen = 0;
	static unsigned long Jiffiestmp;
	static char tag = 0;
	static int oldSeq=-1;
	static int startSeq = -1;
	static int maxSeq = 0;
	int cal_check_sum = 0;
	int raw_check_sum = 0;
	unsigned char *crc_ptr = NULL;
	struct trx_header *trx = (struct trx_header *)MULTI_BUF_BASE;
	struct trx_header *aio_trx = NULL;
	unsigned long retlen = 0;
	static char badimg = 0;

	/*Get the Internet GPIO Number*/
//	uint8 internet_gpio = *(uint8 *)(INTERNET_GPIO);
	uint8 internet_gpio = READ_FLASH_BYTE(INTERNET_GPIO);
	/*Get the DSL GPIO Number*/
//	uint8 dsl_gpio = *(uint8 *)(ADSL_GPIO);
	uint8 dsl_gpio = READ_FLASH_BYTE(ADSL_GPIO);

	if(dsl_gpio == 0)
			dsl_gpio = DSL_GPIO_DEFAULT;
	if(internet_gpio == 0){
		if(isEN7526D || isEN7526G || isEN7513 || isEN7513G || isEN7521G || isEN7586)
			internet_gpio = 28;
		else if(isEN7526F || isEN7521F || isEN7521S || isEN7512)
			internet_gpio = 4;
		else
			internet_gpio = INTERNET_GPIO_DEFAULT;
	}
	
	if(finishMultiUpgrade == 1){
		return; //upgrade success
	}
/* may modify for general sun.zhu_20140503 */	
	if (startSeq == -1)
	{
		startSeq = pktseq;
		oldSeq = pktseq - 1; 		
		receive_len = 0;
		maxSeq = imagelen / MULTI_UPGRADE_DATA_LEN;
		if (imagelen % MULTI_UPGRADE_DATA_LEN)
			maxSeq++;
	}

	/*
		check seq
	*/
	if (pktseq!=(oldSeq+1))
	{
		if ((pktseq != 1) || (oldSeq != maxSeq))  // on only one condition it would be fine: pktseq == 1 && oldSeq == macSeq 
		{
			goto error;
		}
	}

	oldSeq = pktseq;        // always assign current packet seq to old seq, or the (oldSeq != maxSeq) would be false after the loop meets the end
	
	if ((startSeq != -1) && ((startSeq != 1) && (pktseq == (startSeq - 1)) || (startSeq == 1 && pktseq == maxSeq)))
	{
		last_packet = 1;
	}
	else
	{
		last_packet = 0;
	}

#if defined(TCSUPPORT_MULTI_UPGRADE_LIONS)
	if(isLinosAllinone && (pktseq > 1)){
		memcpy(MULTI_BUF_BASE + ((pktseq-1)*MULTI_UPGRADE_DATA_LEN)-LINOSALLINONE_HEADER, skb->data + MULTI_UPGRADE_PKT_HEADLEN, MULTI_UPGRADE_DATA_LEN);
		receive_len += MULTI_UPGRADE_DATA_LEN;
		linosAllinoneChkSumCalc(skb->data + MULTI_UPGRADE_PKT_HEADLEN,MULTI_UPGRADE_DATA_LEN);
	}
	else if(!isLinosAllinone){
#endif
	memcpy(MULTI_BUF_BASE + ((pktseq-1)*MULTI_UPGRADE_DATA_LEN), skb->data + MULTI_UPGRADE_PKT_HEADLEN, MULTI_UPGRADE_DATA_LEN);

	receive_len += MULTI_UPGRADE_DATA_LEN;
#if defined(TCSUPPORT_MULTI_UPGRADE_LIONS)
	}
#endif

	if((Jiffies - Jiffiestmp) > 20){
		Jiffiestmp = Jiffies;
		if(tag == 0){
			
#ifdef DBG_PRINT
			LED_OFF(internet_gpio);
#endif
			tag = 1;
		}else{
#ifdef DBG_PRINT
			LED_ON(internet_gpio);
#endif
			tag = 0;
		}
	}

	if(last_packet == 1){
#if defined(TCSUPPORT_MULTI_UPGRADE_LIONS)
		if(isLinosAllinone){
			if(receive_len < mutAllinoneSize){
				#ifdef DBG_PRINT
				prom_printf("linos allinone receive_len < imagelen\n");
				#endif
				receive_len = 0;
				isLinosAllinone = 0;
				goto error;
			}
			/*Check the image size, Linos has two block reserved area*/
			if(IS_SPIFLASH){
				if(mutAllinoneSize>(mtd.size-FLASH_ERASE_SIZE*2)){
					#ifdef DBG_PRINT
					prom_printf("Image too big for partition: %s\n", mtd);
					LED_OFF(dsl_gpio);
					LED_ON(internet_gpio);
					#endif
					receive_len = 0;
					isLinosAllinone = 0;
					goto error;
				}
			}
			//prom_printf("calculate checksum = %x\n",(uint16)ChkSum);
			/*crc check*/
			if(mutAllinoneChksum != (uint16)ChkSum){
				#ifdef DBG_PRINT
				prom_printf("Linos Allinone checksum error!!calculate checksum = %x\n",(uint16)ChkSum);
				LED_OFF(dsl_gpio);
				LED_ON(internet_gpio);
				#endif
				receive_len = 0;
				isLinosAllinone = 0;
				goto error;
			}
			/*backup Linux mac address to Linos allinone*/
			restore_linux_info((char *)MULTI_BUF_BASE);
			#ifdef DBG_PRINT
			prom_printf("Linos allinone receive ok, goto erase and write image to flash\n");
			#endif
			MultiWriteImage(MULTI_BUF_BASE, mutAllinoneSize);
			finishMultiUpgrade = 1;
			return;
		}		
#endif
		if(receive_len >= imagelen){
			/*
				decide the type of image
			*/			
			if(trx->magic == TRX_MAGIC2){
				cal_check_sum = crc32buf((MULTI_BUF_BASE+sizeof(struct trx_header)), (imagelen-sizeof(struct trx_header)), 1);

				if (cal_check_sum != trx->crc32){
					#ifdef DBG_PRINT
					prom_printf("checksum error!!! \r\n");

					/*
						turn off ADSL and Internet LED
					*/
					LED_OFF(dsl_gpio);
					LED_ON(internet_gpio);
					#endif
					goto error;
				}
				
				if (IS_SPIFLASH) {
					/*Check the image size*/
					#if (TCSUPPORT_RESERVEAREA_BLOCK==1)
					if(imagelen > (mtd.size-TCBOOT_SIZE-ROMFILE_SIZE-FLASH_ERASE_SIZE*1)) {
					#elif (TCSUPPORT_RESERVEAREA_BLOCK==2)
					if(imagelen > (mtd.size-TCBOOT_SIZE-ROMFILE_SIZE-FLASH_ERASE_SIZE*2)) {
					#elif (TCSUPPORT_RESERVEAREA_BLOCK==3)
					if(imagelen > (mtd.size-TCBOOT_SIZE-ROMFILE_SIZE-FLASH_ERASE_SIZE*3)) {
					#elif (TCSUPPORT_RESERVEAREA_BLOCK==4)
					if(imagelen > (mtd.size-TCBOOT_SIZE-ROMFILE_SIZE-FLASH_ERASE_SIZE*4)) {
					#else
					if(imagelen > (mtd.size-TCBOOT_SIZE-ROMFILE_SIZE-FLASH_ERASE_SIZE*7)) {
					#endif
						badimg = 1;
					}
				} else if (IS_NANDFLASH) {
#ifdef TCSUPPORT_BB_NAND				
					if (ra_inl(CR_AHB_HWCONF) & (0x00080000)) {
						if(imagelen > ((1<<ra.flash->chip_shift) - 7*(1<<ra.flash->erase_shift))) {
							badimg = 1;
						}
					} else {
						/*Check the image size*/
						#if (TCSUPPORT_RESERVEAREA_BLOCK==1)
						if(imagelen > ((1<<ra.flash->chip_shift)-TCBOOT_SIZE-ROMFILE_SIZE-FLASH_ERASE_SIZE*1)) {
						#elif (TCSUPPORT_RESERVEAREA_BLOCK==2)
						if(imagelen > ((1<<ra.flash->chip_shift)-TCBOOT_SIZE-ROMFILE_SIZE-FLASH_ERASE_SIZE*2)) {
						#elif (TCSUPPORT_RESERVEAREA_BLOCK==3)
						if(imagelen > ((1<<ra.flash->chip_shift)-TCBOOT_SIZE-ROMFILE_SIZE-FLASH_ERASE_SIZE*3)) {
						#elif (TCSUPPORT_RESERVEAREA_BLOCK==4)
						if(imagelen > ((1<<ra.flash->chip_shift)-TCBOOT_SIZE-ROMFILE_SIZE-FLASH_ERASE_SIZE*4)) {
						#elif (TCSUPPORT_RESERVEAREA_BLOCK==5)
						if(imagelen > ((1<<ra.flash->chip_shift)-TCBOOT_SIZE-ROMFILE_SIZE-FLASH_ERASE_SIZE*5)) {
						#else
						if(imagelen > ((1<<ra.flash->chip_shift)-TCBOOT_SIZE-ROMFILE_SIZE-FLASH_ERASE_SIZE*7)) {
						#endif
							badimg = 1;
						}	
					}
#endif					
				}
				
				if (badimg){
					#ifdef DBG_PRINT
					prom_printf("Image too big for partition: %s\n", mtd);
					LED_OFF(dsl_gpio);
					LED_ON(internet_gpio);
					#endif
					goto error;
				}
			}
			else{
				
				/*
					check if image is tclinux_allinone
				*/
/* may modify for general sun.zhu_20140503 */
				aio_trx = (struct trx_header *)(MULTI_BUF_BASE+TCBOOT_SIZE+ROMFILE_SIZE);
				if(aio_trx->magic == TRX_MAGIC2){
					#ifdef DBG_PRINT
					prom_printf(" it is tclinux_allineone!!! \r\n");	
					#endif
				}
				else{
					#ifdef DBG_PRINT
					prom_printf(" it is not tclinux_allineone!!! \r\n");	
					#endif
					goto error;
				}
				cal_check_sum = crc32buf((char*)MULTI_BUF_BASE, (imagelen-4), 1);
				
				crc_ptr = (unsigned char *)(MULTI_BUF_BASE+imagelen-4);
				raw_check_sum |= *crc_ptr;
				raw_check_sum |= *(crc_ptr+1)<<8;
				raw_check_sum |= *(crc_ptr+2)<<16;
				raw_check_sum |= *(crc_ptr+3)<<24;
				
				if (cal_check_sum != raw_check_sum){
					#ifdef DBG_PRINT
					prom_printf("tclinux_allinone checksum error!!! \r\n");

					/*
						turn off ADSL and Internet LED
					*/
					
					LED_OFF(dsl_gpio);
					LED_ON(internet_gpio);
					#endif
					
					goto error;
				}
				
				/*
					backup mi.conf
				*/
				flash_read(TCBOOT_BASE+MICONF_OFFSET, MICONF_SIZE, &retlen, MULTI_BUF_BASE+MICONF_OFFSET);
			}
			#ifdef DBG_PRINT
			prom_printf("receive ok\n");
			#endif
			MultiWriteImage(MULTI_BUF_BASE, imagelen);
			finishMultiUpgrade = 1;
		}else{
			#ifdef DBG_PRINT
			prom_printf("receive_len < imagelen\n");
			#endif
			receive_len = 0;
			prevImgLen = 0;
			goto error;
		}
	}
/* may modify for general sun.zhu_20140503 */
	return;
error:
	startSeq = -1;
	maxSeq = 0;
	return;
}
#endif


#if defined(TCSUPPORT_MULTI_BOOT)
unsigned short calculate_check_sum(unsigned char *base, int len)
{
	int check_sum = 0;
	unsigned char *start = NULL;
	unsigned char *end = NULL;
	unsigned short result;

	if (base == NULL)
		return -1;

	start = base;
	end = start+len;

	while(start < end){
	        check_sum += *start;
	        start++;
	}

	result = (check_sum & 0xffff) + (check_sum >>16 );
	return result;
}


void MultiBootHandle(sk_buff *skb)
{
	static int oldSeq=-1;
	int cal_check_sum = 0;
	int raw_check_sum = 0;
	unsigned short pkt_check_sum = 0;
	unsigned char *crc_ptr = NULL;
//	unsigned int addr;
	struct trx_header *trx = (struct trx_header *)MULTI_BUF_BASE;

	struct MULTI_BOOT_T *rcv_ptr = (struct MULTI_BOOT_T *)(skb->data + MULTIBOOT_SIG_OFFSET);
	unsigned long pktseq = rcv_ptr->ID+1;
	unsigned long imagelen = rcv_ptr->fileLen;

	#ifdef CONFIG_DUAL_IMAGE
	#ifdef TCSUPPORT_DUAL_IMAGE_ENHANCE
//	unsigned int *header_slave = (unsigned int *)(0xb0020000+offset);
	unsigned int *header_slave = (unsigned int *)(flash_base+flash_tclinux_start+offset);
	unsigned int slave_tclinux_offset = offset+flash_tclinux_start;
	#else
//	unsigned int *header_slave = (unsigned int *)(0xb0020000+0x500000);		
	unsigned int *header_slave = (unsigned int *)(flash_base+flash_tclinux_start+TCLINUX_SLAVE_FLASH_START);		
	unsigned int slave_tclinux_offset = flash_tclinux_start+TCLINUX_SLAVE_FLASH_START;
	#endif	
//	unsigned int slave_kernel_size = header_slave[20]+TRX_LEN;
//	unsigned int slave_rootfs_size = header_slave[21];

	char boot_flag = 0;

	unsigned int slave_kernel_size = READ_FLASH_DWORD((unsigned long long)((unsigned int)header_slave + 20 * sizeof(unsigned int)))+TRX_LEN;
	unsigned int slave_rootfs_size = READ_FLASH_DWORD((unsigned long long)((unsigned int)header_slave + 21 * sizeof(unsigned int)));;
	unsigned int slave_tclinux_size  = slave_kernel_size + slave_rootfs_size;
	#endif

	unsigned int romd_offset;
	unsigned int nand2K_rom_offset;
//	unsigned int *header = (unsigned int *)(0xb0020000);
	unsigned int *header = (unsigned int *)(flash_base+flash_tclinux_start);
//	unsigned int kernel_size = header[20]+TRX_LEN;
//	unsigned int rootfs_size = header[21];
	unsigned int kernel_size = READ_FLASH_DWORD((unsigned long long)((unsigned int)header + 20 * sizeof(unsigned int)))+TRX_LEN;
	unsigned int rootfs_size = READ_FLASH_DWORD((unsigned long long)((unsigned int)header + 21 * sizeof(unsigned int)));;
	unsigned int tclinux_size  = kernel_size + rootfs_size;
	unsigned long retlen = 0;
	unsigned long romfile_offset;
	
	if (IS_SPIFLASH) {
		romd_offset = mtd.size - ROMD_TO_BOTTOM;
	} else if (IS_NANDFLASH) {	
#ifdef TCSUPPORT_BB_NAND	
		/* 128 K block NAND Flash */
		if (ra_inl(CR_AHB_HWCONF) & (0x00080000)) {
			romd_offset = (1<<ra.flash->chip_shift) - 5 * (1<<ra.flash->erase_shift);
			nand2K_rom_offset = (1<<ra.flash->chip_shift) - 6 * (1<<ra.flash->erase_shift);
		} else {
			romd_offset = (1<<ra.flash->chip_shift) - ROMD_TO_BOTTOM;
		}
#endif		
	}	

	if(finishMultiBoot == 1){
		return; //upgrade success
	}
	
	if (pktseq == 1){
		/*
			check if need to clear romfile or romd
		*/
		if (rcv_ptr->debugFlag == CLEAR_ROMFILE){
			#ifdef DBG_PRINT
			prom_printf("erase running romfile...  \n");
			#endif
			if (IS_SPIFLASH) {
				flash_erase((TCBOOT_BASE+TCBOOT_SIZE), ROMFILE_SIZE);
			} else if (IS_NANDFLASH) {
#ifdef TCSUPPORT_BB_NAND			
				if (ra_inl(CR_AHB_HWCONF) & (0x00080000)) {
					flash_erase(nand2K_rom_offset,  (1<<ra.flash->erase_shift));
				} else {			
					flash_erase((TCBOOT_BASE+TCBOOT_SIZE), ROMFILE_SIZE);
				}	
#endif				
			}	
			finishMultiBoot = 1;
			return;
		}
		else if (rcv_ptr->debugFlag == CLEAR_ROMD){
			#ifdef DBG_PRINT
			prom_printf("erase rom-d...  \n");
			#endif
			if (IS_SPIFLASH) {
				flash_erase(romd_offset, ROMFILE_SIZE);
			} else {	
#ifdef TCSUPPORT_BB_NAND			
				if (ra_inl(CR_AHB_HWCONF) & (0x00080000)) {
					flash_erase(romd_offset, (1<<ra.flash->erase_shift));
				} else {
					flash_erase(romd_offset, ROMFILE_SIZE);
				}	
#endif				
			}	
			finishMultiBoot = 1;
			return;
		}

		oldSeq = 0; 		
		rcv_len = 0;
	}


	/*
		check seq
	*/
	if (pktseq!=(oldSeq+1)){
		return;
	}
	else
		oldSeq = pktseq;

	/*
		store data in buf and update rcv_len
	*/
	memcpy(MULTI_BUF_BASE+(rcv_ptr->ID)*MULTIBOOT_PACKET_SIZE, rcv_ptr->data, rcv_ptr->dataLen);
	rcv_len += rcv_ptr->dataLen;


	cur_file_type = rcv_ptr->fileFlag&CUR_FILE_MASK;


	/*
		check crc for romfile and rom-d
	*/
	if (cur_file_type == CURR_FILE_FLAG_ROMFILE || cur_file_type==CURR_FILE_FLAG_ROMD){
		pkt_check_sum = calculate_check_sum(rcv_ptr->data, rcv_ptr->dataLen);

		if (rcv_ptr->checkSum != pkt_check_sum){
			#ifdef DBG_PRINT
			prom_printf(" check sum is not correct!!!\r\n");	
			#endif
			rcv_len = 0;
			oldSeq = -1;
			return;
		}
	}


	/*
		 received the last pkt
	*/
	if (rcv_len >= imagelen){
		/*
			check the type of image
		*/	
		switch (cur_file_type){
		case CURR_FILE_FLAG_BOOT:
			#ifdef DBG_PRINT
			prom_printf(" it is tcboot.bin!!!\r\n");
			#endif

			/*
				check CRC
			*/
			cal_check_sum = crc32buf((char*)MULTI_BUF_BASE, (imagelen-4), 1);
			crc_ptr = (unsigned char *)(MULTI_BUF_BASE+imagelen-4);

			raw_check_sum |= *crc_ptr<<24;
			raw_check_sum |= *(crc_ptr+1)<<16;
			raw_check_sum |= *(crc_ptr+2)<<8;
			raw_check_sum |= *(crc_ptr+3);

			
			if (cal_check_sum != raw_check_sum){
				#ifdef DBG_PRINT
				prom_printf("tclinux.bin checksum error!!! \r\n");		
				#endif
				goto error;
			}
			
			/*
				backup mi.conf
			*/
			flash_read(TCBOOT_BASE+MICONF_OFFSET, MICONF_SIZE, &retlen, MULTI_BUF_BASE+MICONF_OFFSET);
			
			LED_ON(SYSTEM_GPIO);
			prom_printf("Write to flash from %X to %X with %X bytes\n", MULTI_BUF_BASE, TCBOOT_BASE, imagelen);
			flash_erase(TCBOOT_BASE, TCBOOT_SIZE);
			prom_printf("\n");		
			flash_write(TCBOOT_BASE, imagelen, &retlen, (const unsigned char *) (MULTI_BUF_BASE));	
			break;

		case CURR_FILE_FLAG_TCLINUX:
		
			if(trx->magic == TRX_MAGIC2){
				#ifdef DBG_PRINT
				prom_printf(" it is tclinux.bin!!! \r\n");				
				#endif
				cal_check_sum = crc32buf((MULTI_BUF_BASE+sizeof(struct trx_header)), (imagelen-sizeof(struct trx_header)), 1);

				if (cal_check_sum != trx->crc32){
					#ifdef DBG_PRINT
					prom_printf("checksum error!!! \r\n");
					#endif
					goto error;
				}


				#ifdef CONFIG_DUAL_IMAGE
				if (flash_tclinux_start+tclinux_size > slave_tclinux_offset){
					#ifdef DBG_PRINT
					prom_printf("tclinux can not be upgraded because of dual image!!!\r\n");			
					#endif
					goto error;
				} 	
				#else
				if (flash_tclinux_start+tclinux_size > romd_offset){
					#ifdef DBG_PRINT
					prom_printf("romd can not be upgraded because tclinux.bin is too large!!!\r\n");			
					#endif
					goto error;
				} 			
				#endif
						
				LED_ON(SYSTEM_GPIO);			
				prom_printf("Write to flash from %X to %X with %X bytes\n", MULTI_BUF_BASE, flash_tclinux_start, imagelen);
				flash_erase(flash_tclinux_start, imagelen);
				prom_printf("\n");				
				flash_write(flash_tclinux_start, imagelen, &retlen, (const unsigned char *) (MULTI_BUF_BASE));
			}
			else{
				
				/*
					check if image is tclinux_allinone
				*/
				if (memcmp(MULTI_BUF_BASE+TCBOOT_SIZE, "<ROMFILE>", strlen("<ROMFILE>")) == 0){
					#ifdef DBG_PRINT
					prom_printf(" it is tclinux_allineone!!! \r\n");	
					#endif
				}
				else{
					#ifdef DBG_PRINT
					prom_printf(" it is not tclinux_allineone!!! \r\n");	
					#endif
					goto error;
				}

				cal_check_sum = crc32buf((char*)MULTI_BUF_BASE, (imagelen-4), 1);			
				crc_ptr = (unsigned char *)(MULTI_BUF_BASE+imagelen-4);
				raw_check_sum |= *crc_ptr;
				raw_check_sum |= *(crc_ptr+1)<<8;
				raw_check_sum |= *(crc_ptr+2)<<16;
				raw_check_sum |= *(crc_ptr+3)<<24;
				if (cal_check_sum != raw_check_sum){
					#ifdef DBG_PRINT
					prom_printf("tclinux_allinone checksum error!!! \r\n");					
					#endif
					goto error;
				}

				/*
					check if image is too large
				*/
				#ifdef CONFIG_DUAL_IMAGE
				if (TCBOOT_BASE+imagelen > slave_tclinux_offset){
					prom_printf("tclinux_allinone can not be upgraded because of dual image!!!\r\n");			
					goto error;
				} 	
				#else
				if (TCBOOT_BASE+imagelen > romd_offset){
					prom_printf("tclinux_allinone can not be upgraded because it is too large!!!\r\n");			
					goto error;
				} 			
				#endif

				/*
					backup mi.conf
				*/
				flash_read(TCBOOT_BASE+MICONF_OFFSET, MICONF_SIZE, &retlen, MULTI_BUF_BASE+MICONF_OFFSET);

				LED_ON(SYSTEM_GPIO);				
				prom_printf("Write to flash from %X to %X with %X bytes\n", MULTI_BUF_BASE, TCBOOT_BASE, imagelen);
				flash_erase(TCBOOT_BASE, imagelen);
				prom_printf("\n");
				flash_write(TCBOOT_BASE, imagelen, &retlen, (const unsigned char *) (MULTI_BUF_BASE));

				if (IS_NANDFLASH){
				#ifdef TCSUPPORT_BB_NAND				
					prom_printf(" write romfile.cfg \n");
					/* page size 2K bytes nand flash */
					if (ra_inl(CR_AHB_HWCONF) & (0x00080000)){
						romfile_offset = (1<<ra.flash->chip_shift) - 6 * (1<<ra.flash->erase_shift);
						flash_erase(romfile_offset, ROMFILE_SIZE);
						prom_printf("\n");		
						flash_write(romfile_offset, ROMFILE_SIZE, &retlen, (const unsigned char *) (MULTI_BUF_BASE+TCBOOT_SIZE));
					}
				#endif					
				}
			}	
			break;

		case CURR_FILE_FLAG_ROMD:
			#ifdef DBG_PRINT
			prom_printf(" it is rom-d!!!\r\n");			
			#endif

			#ifdef CONFIG_DUAL_IMAGE
			if (slave_tclinux_offset+slave_tclinux_size > romd_offset){
				#ifdef DBG_PRINT
				prom_printf("romd can not be upgraded because of dual image!!!\r\n");			
				#endif
				goto error;
			} 	
			#else
			if (flash_tclinux_start+tclinux_size > romd_offset){
				#ifdef DBG_PRINT
				prom_printf("romd can not be upgraded because tclinux.bin is too large!!!\r\n");			
				#endif
				goto error;
			} 			
			#endif

			LED_ON(SYSTEM_GPIO);
			prom_printf("Write to flash from %X to %X with %X bytes\n", MULTI_BUF_BASE, romd_offset, imagelen);
			flash_erase(romd_offset, ROMFILE_SIZE);
			prom_printf("\n");
			flash_write(romd_offset, imagelen, &retlen, (const unsigned char *) (MULTI_BUF_BASE));
			break;

		case CURR_FILE_FLAG_ROMFILE:
			#ifdef DBG_PRINT
			prom_printf(" it is romfile!!!\r\n");	
			#endif

			LED_ON(SYSTEM_GPIO);			
			prom_printf("Write to flash from %X to %X with %X bytes\n", MULTI_BUF_BASE, (TCBOOT_BASE+TCBOOT_SIZE), imagelen);
			/* page size 2K bytes nand flash */
			if (IS_NANDFLASH && (ra_inl(CR_AHB_HWCONF) & (0x00080000))){
			#ifdef TCSUPPORT_BB_NAND
				romfile_offset = (1<<ra.flash->chip_shift) - 6 * (1<<ra.flash->erase_shift);
			#endif				
			} else{
				romfile_offset = TCBOOT_BASE+TCBOOT_SIZE;
			}
			flash_erase(romfile_offset, ROMFILE_SIZE);
			prom_printf("\n");
			flash_write(romfile_offset, imagelen, &retlen, (const unsigned char *) (MULTI_BUF_BASE));
			break;

		default:
			#ifdef DBG_PRINT
			prom_printf(" unknown file type!!!\r\n");
			#endif
			goto error;
		}
		#ifdef DBG_PRINT
		prom_printf("receive ok\n");
		#endif

		finishMultiBoot = 1;
	}
	return;


error:

	LED_OFF(SYSTEM_GPIO);
	startMultiBoot = 0;
	return;
}
#endif



#ifdef BOOT_LZMA_SUPPORT
struct web_connection {
  /* the client tcp connection */
  struct tcp_pcb *current_connection;

  /* Detect idle connection using the last seq number */
  unsigned long last_rx_seqno;

  /* the rx state */
  int rx_state;

  /* save the GET http path here */
  char get_path[200];

  /* use this buffer for processing received data */
  char rx_buffer[200];
} the_web_connection;
#endif

unsigned long local_ip = 0;
/* Extract a short from a long */
#define hiword(x)       ((unsigned short)((x) >> 16))
#define loword(x)       ((unsigned short)(x))

unsigned short in_csum(/* pseudohdr_t *pseudohdr, */ unsigned short *ptr, int nbytes)
{
	register int			sum;		/* assumes long == 32 bits */
	unsigned short			oddbyte;
	register unsigned short	answer; 	/* assumes u_short == 16 bits */

	/*
	 * Our algorithm is simple, using a 32-bit accumulator (sum),
	 * we add sequential 16-bit words to it, and at the end, fold back
	 * all the carry bits from the top 16 bits into the lower 16 bits.
	 */

	sum = 0;

	/* Sum pseudo-header, if present */
#if 0
	if(pseudohdr != NULL){
		sum = hiword(pseudohdr->source);
		sum += loword(pseudohdr->source);
		sum += hiword(pseudohdr->dest);
		sum += loword(pseudohdr->dest);
		sum += pseudohdr->protocol;
		sum += pseudohdr->length;
	}
#endif

	while (nbytes > 1)	{
		sum += *ptr++;
		nbytes -= 2;
	}

				/* mop up an odd byte, if necessary */
	if (nbytes == 1) {
		oddbyte = 0;		/* make sure top half is zero */
		*((unsigned char *) &oddbyte) = *(unsigned char *)ptr;   /* one byte only */
		sum += oddbyte;
	}

	/*
	 * Add back carry outs from top 16 bits to low 16 bits.
	 */

	sum  = (sum >> 16) + (sum & 0xffff);	/* add high-16 to low-16 */
	sum += (sum >> 16); 		/* add carry */
	answer = ~sum;		/* ones-complement, then truncate to 16 bits */
	return(answer);
}

int ip_init(unsigned long ip)
{
	local_ip = ip;
	return 0;
}

int ip_rcv_packet(sk_buff *skb)
{
	struct iphdr *ip_hdr = (struct iphdr *)(skb->data);

#if defined(TCSUPPORT_MULTI_UPGRADE) || defined(TCSUPPORT_MULTI_BOOT) 
	char *mac = (char *)(ip_hdr) - sizeof(struct ethhdr);
	static int drop_multi = 0;
#endif

#if defined(TCSUPPORT_MULTI_UPGRADE)
	/*Get the Internet GPIO Number*/
//	uint8 internet_gpio = *(uint8 *)(INTERNET_GPIO);
	uint8 internet_gpio = READ_FLASH_BYTE(INTERNET_GPIO);
	/*Get the DSL GPIO Number*/
//	uint8 dsl_gpio = *(uint8 *)(ADSL_GPIO);
	uint8 dsl_gpio = READ_FLASH_BYTE(ADSL_GPIO);
#if !defined(TCSUPPORT_C1_TRUE)
	int i = 0;
	/*Get the Multi upgrade*/
	uint8 *multi_upgrade_gpio = (uint8 *)(MULTI_UPGRADE_GPIO);
	uint8 multi_led;
#endif
	if(dsl_gpio == 0)
			dsl_gpio = DSL_GPIO_DEFAULT;
	if(internet_gpio == 0){
		if(isEN7526D || isEN7526G || isEN7513 || isEN7513G || isEN7521G || isEN7586)
			internet_gpio = 28;
		else if(isEN7526F || isEN7521F || isEN7521S || isEN7512)
			internet_gpio = 4;
		else
			internet_gpio = INTERNET_GPIO_DEFAULT;
	}
#endif

	/* record ip header pointer */
	skb->ip_hdr = ip_hdr;

#if !defined(TCSUPPORT_MULTI_UPGRADE) && !defined(TCSUPPORT_MULTI_BOOT)
	if (ntohl(ip_hdr->daddr) == local_ip)
	{
#endif

		if (ip_hdr->protocol == UDP){
			skb->len = ntohs(ip_hdr->tot_len);
			skb_pull(skb, sizeof(struct iphdr));

#if defined(TCSUPPORT_MULTI_UPGRADE) || defined(TCSUPPORT_MULTI_BOOT) 
#if !defined(TCSUPPORT_C1_OBM)
			if(mac[0] & 0x1)
#endif
			{
				if (0){
					;
				}
#if defined(TCSUPPORT_MULTI_UPGRADE)
				else if(memcmp((skb->data + MULTIPKT_TAG_OFFSET), MULTI_UPGRADE_PKT_TAG, (sizeof(MULTI_UPGRADE_PKT_TAG)-1)) == 0){
					if (skb->len < 1048) {
						#ifdef DBG_PRINT
						prom_printf("skb->len < 1048 \r\n");
						#endif
						return 0;
					}
								
					if(startMultiUpgrade == 0){
						prom_printf("\nStartMultiUpgrade\n");
						startMultiUpgrade = 1;
						LED_OEN(internet_gpio); 
						LED_OEN(dsl_gpio);
#if !defined(TCSUPPORT_C1_TRUE)
						for(i=0; i<maxMultiLedNum; i++){
							multi_led = READ_FLASH_BYTE(multi_upgrade_gpio + i);
//							if(multi_upgrade_gpio[i] != 0)
//								LED_OEN(multi_upgrade_gpio[i]);
							if(multi_led != 0)
							#if defined(TCSUPPORT_CT_WAN_PTM)
{
								if(multi_led > 31){
									if(multi_led > 47){
										regWrite32(CR_GPIO_CTRL3,regRead32(CR_GPIO_CTRL3)|(1<<((multi_led-48)*2)));
									}else{
										regWrite32(CR_GPIO_CTRL2,regRead32(CR_GPIO_CTRL2)|(1<<((multi_led-32)*2)));
									}
									regWrite32(CR_GPIO_ODRAIN1,regRead32(CR_GPIO_ODRAIN1)|(1<<(multi_led-32)));	
								}else{
									if(multi_led > 15){
										regWrite32(CR_GPIO_CTRL1,regRead32(CR_GPIO_CTRL1)|(1<<((multi_led-16)*2)));
									}else{
										regWrite32(CR_GPIO_CTRL,regRead32(CR_GPIO_CTRL)|(1<<(multi_led*2)));
									}
									regWrite32(CR_GPIO_ODRAIN,regRead32(CR_GPIO_ODRAIN)|(1<<multi_led));
								}
							}
                                                        #else
								LED_OEN(multi_led);
                                                        #endif
						}		
#endif
						LED_OFF(internet_gpio);			
					}
					MultiUpgradeHandle(skb);
				}				
#endif

#if defined(TCSUPPORT_MULTI_BOOT)
				else if(memcmp((skb->data + MULTIBOOT_SIG_OFFSET), MULTIBOOT_SIG, (sizeof(MULTIBOOT_SIG)-1)) == 0){

						if (skb->len < 1054) {
						#ifdef DBG_PRINT 
							prom_printf("skb->len < 1054 \r\n");
						#endif
							return 0;
						}

								
						if(startMultiBoot == 0){
						#ifdef DBG_PRINT	
							prom_printf("\nStartMultiBoot\n");
						#endif
							startMultiBoot = 1;
							LED_OEN(SYSTEM_GPIO);
						}
						MultiBootHandle(skb);
					}
				
#endif			
			}	

#if defined(TCSUPPORT_MULTI_UPGRADE)
			if(startMultiUpgrade == 1){
				return 0;
			}
#endif

#if defined(TCSUPPORT_MULTI_BOOT)
			if(startMultiBoot == 1){
				return 0;
			}
#endif
			
#endif			
		
			udp_rcv_packet(skb);
		}
		else if(ip_hdr->protocol == ICMP){
			skb->len = ntohs(ip_hdr->tot_len);
			skb_pull(skb, sizeof(struct iphdr));
			icmp_rcv_packet(skb);
		}
#ifdef BOOT_LZMA_SUPPORT
		else if(ip_hdr->protocol == TCP){
			struct pbuf *tcp_pkt;
            		struct pbuf *q;
           		int i;
#ifdef HTTP_DBG
			prom_printf("Here comes TCP: skb->len = %d\r\n", skb->len);
#endif			
			tcp_pkt = pbuf_alloc(PBUF_TRANSPORT, skb->len, PBUF_POOL);
			if(!tcp_pkt){
#ifdef HTTP_DBG				
				prom_printf("failed to allocate pbuf\r\n");
#endif				
				return -1;
			}
			//pbuf is a link list queue, so need copy p->next. shnwind 20110721. 
	    		for(q=tcp_pkt,i=0; q!=NULL; q=q->next){
                		memcpy(q->payload, (skb->data)+i, q->len);
                		i+=q->len;
		 	}
			//memcpy(tcp_pkt->payload, skb->data, tcp_pkt->len); //treyy
			tcp_input(tcp_pkt);
		}
#endif

#if !defined(TCSUPPORT_MULTI_UPGRADE) && !defined(TCSUPPORT_MULTI_BOOT)
	}
#endif
	
	return 0;
}

#ifdef BOOT_LZMA_SUPPORT
int tc_tcpout(struct pbuf *p, struct ip_addr *src, struct ip_addr *dest, u8_t ttl, u8_t tos)
{
	sk_buff *skb;

#ifdef HTTP_DBG	
	prom_printf("tc_tcpout\r\n");
#endif	

	skb = alloc_skb(ETH_FRAME_LEN);
	ip_skb_reserve(skb);
	skb->len += p->len;
	memcpy(skb->data, p->payload, p->len);
#ifdef HTTP_DBG	
	prom_printf("tc_tcpout: p->len = %d\r\n", p->len);
#endif	
	ip_send(skb, dest->addr, TCP);
}
#endif

int ip_send(sk_buff *skb, unsigned long ip, unsigned char proto)
{
	struct iphdr *ip_hdr;
	static unsigned short ip_id = 32;
	unsigned char dest_eth_addr[ETH_ALEN];

	if (arp_get_eth_addr(ip, dest_eth_addr)) {
		free_skb(skb);
		return -1;
	}

	ip_hdr = (struct iphdr *)skb_push(skb, sizeof(struct iphdr));

	ip_hdr->ihl = 5;
	ip_hdr->version = 4;
	ip_hdr->tos = 0;
	ip_hdr->tot_len = htons(skb->len);
	ip_hdr->id = htons(ip_id++);
	ip_hdr->frag_off = 0;
	ip_hdr->ttl = 20;
	ip_hdr->protocol = proto;
	ip_hdr->check = 0;

	ip_hdr->saddr = htonl(local_ip);
	ip_hdr->daddr = htonl(ip);
	ip_hdr->check = in_csum((unsigned short *)ip_hdr, sizeof(struct iphdr));
	
	eth_send(skb, dest_eth_addr, ETH_P_IP);

	return 0;
}

void ip_skb_reserve(sk_buff *skb)
{
	eth_skb_reserve(skb);
	skb_reserve(skb, sizeof(struct iphdr));
}

unsigned long ip_get_source_ip(sk_buff *skb)
{
	struct iphdr *ip_hdr;

	ip_hdr = (struct iphdr *)(skb->ip_hdr);
	return ntohl(ip_hdr->saddr);
}
