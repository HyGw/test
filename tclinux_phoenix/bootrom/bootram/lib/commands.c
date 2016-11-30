#include <linux/errno.h>
#include <linux/init.h>
#include <linux/kernel_stat.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/random.h>
#include <linux/string.h>

#include <asm/bitops.h>
#include <asm/bootinfo.h>
#include <asm/irq.h>
#include <asm/mipsregs.h>
#include <asm/system.h>
#include <asm/tc3162.h>
#include <asm/io.h>
#include <asm/delay.h>

#include "common.h"
#if defined(TCSUPPORT_CPU_EN7512)||defined(TCSUPPORT_CPU_EN7521)
#include "spi_nand_flash.h"
#include "controller/spi_controller/spi_controller.h"
#else
#include "nandflash.h"
#endif

#include "flashhal.h"
#include "commands.h"

#ifdef TCSUPPORT_RESERVEAREA_EXTEND
#include "flash_layout/prolinecmd.h"
#endif

#if defined(TCSUPPORT_CT_BOOTLOADER_UPGRADE)
#include "flash_layout/tc_partition.h"
#endif
#ifdef BOOT_LZMA_SUPPORT
#include "lwip/sys.h"
#include "lwip/err.h"
#include "lwip/pbuf.h"
#include "lwip/tcp.h"
#include "lwip/opt.h"
#endif

#if defined(MT7530_SLT)
#include "../net/switch_slt.h"
#include "../net/tc3262gmac.h"
#endif

#include "../../../tools/trx/trx.h"

#ifdef SPI_DRAM_TEST_CMD
#include "dramtest.h"
#endif
#if defined(CONFIG_DUAL_IMAGE) || defined(SPI_DRAM_TEST_CMD)
#if defined(TCSUPPORT_CPU_MT7505) || defined(TCSUPPORT_CPU_EN7512) || defined(TCSUPPORT_CPU_EN7521)
#include "newspiflash.h"
#else
#include "spiflash.h"
#endif
#endif
#if defined(TCSUPPORT_NEW_SPIFLASH_DEBUG)
#include "newspiflash.h"
#endif
#ifdef TCSUPPORT_MTD_PARTITIONS_CMDLINE
#include "mtd_partition_parse.h"
#endif

#if defined(TCSUPPORT_CPU_EN7521) && defined(DRAM_PROTECT_TEST)
int mp_dbg_level = 0xf;
int nmi_test_enable = 0;
#endif
#if defined(TCSUPPORT_FWC_ENV)
#define RESTART_TIME_COUNT (3*60*100)
int restart_time_flags = 0;
int restart_time_count = RESTART_TIME_COUNT;
#endif


#define KEY_BEL			0x07
#define KEY_BS			0x08
#define KEY_CR			0x0D
#define KEY_LF			0x0A
#if defined(TCSUPPORT_CT_BOOTLOADER_UPGRADE)
//#define HTTP_DBG 1

#define CMD                     0   /*flag for cmd_gets() input cmd*/
#define PWD                     1   /*flag for cmd_gets() input passwd*/
#define USERNAME_PASSWD_LEN	16  /*length of username or passwd*/
//#define FLASH_BASE 		0xb0000000 /*base address for 16M flash*/
//#define RESERVED_BASE   0xfc0000    /*base address for reserved area in flash*/
//#define FLASH_USER_NAME		((USERNAMEPASSWD_RA_OFFSET+RESERVED_BASE)|FLASH_BASE) /*begin address of username in reserved area*/
//#define FLASH_USER_NAME		((USERNAMEPASSWD_RA_OFFSET+RESERVED_BASE)|flash_base) /*begin address of username in reserved area*/
//#define FLASH_PASS_WORD		((USERNAMEPASSWD_RA_OFFSET+USERNAME_PASSWD_LEN+RESERVED_BASE)|FLASH_BASE) /*begin address of passwd in reserved area*/
//#define FLASH_PASS_WORD		((USERNAMEPASSWD_RA_OFFSET+USERNAME_PASSWD_LEN+RESERVED_BASE)|flash_base) /*begin address of passwd in reserved area*/
//#define MRD_USER_NAME			(0xff69|FLASH_BASE) /*begin address of username in MRD area*/
#define MRD_USER_NAME			(0xff69|flash_base) /*begin address of username in MRD area*/
//#define MRD_PASS_WORD			(0xff79|FLASH_BASE) /*begin address of passwd in mrd area*/
#define MRD_PASS_WORD			(0xff79|flash_base) /*begin address of passwd in mrd area*/
#define CT_USER_NAME			"telecomadmin" /*length should be less than 16*/
#endif

#ifdef TCSUPPORT_NAND_RT63368
#define NAND_KERNEL_OFFSET 0x40000
#endif


#define EOF				(-1)

#define printf			prom_printf

#define LINE_LEN		128

#ifdef BOOT_LZMA_SUPPORT
#define IP_ADDRESS_FORMAT( ip) (ip) & 0xff, ((ip) >> 8) & 0xff,\
((ip) >> 16) & 0xff, ((ip) >> 24) & 0xff
#define TCBOOT_NAME             "tcboot.bin"
#define TCLINUX_NAME            "tclinux.bin"
#define TCBOOT                  1
#define TCLINUX                 2
#define ERRFILE                -1

#define UPLOAD_BUF_SIZE 2048

#ifdef TCSUPPORT_BB_NAND
extern struct ra_nand_chip ra;
#if defined(TCSUPPORT_CPU_MT7510)||defined(TCSUPPORT_CPU_MT7520) || defined(TCSUPPORT_CPU_EN7512) || defined(TCSUPPORT_CPU_EN7521)
extern int en_oob_write;
extern int en_oob_erase;
#endif
#endif

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
  char rx_buffer[UPLOAD_BUF_SIZE];
} the_web_connection;

static char print_buffer[2048];
#endif

struct mtd_info {
	unsigned long offset; /* Offset within the memory */
	unsigned long size;   /* Total size of the MTD */
	unsigned long erasesize;
};
extern struct mtd_info mtd;
extern void pause(int ms);

static char cmd_prompt[] = "bldr";
static u8 random_byte(void);


#if defined(TCSUPPORT_CT_BOOTLOADER_UPGRADE)
static char *cmd_gets(char *buf, int len, int flag);
#else
static char *cmd_gets(char *buf, int len);
#endif

static int cmd_parse(cmds_t cmds[], char *line);

#if defined(TCSUPPORT_CT_BOOTLOADER_UPGRADE)
void	user_Auth(void);
static int trim(char *buf);
#endif

static int do_null(int argc, char *argv[]);
static int do_help(int argc, char *argv[]);
#ifndef BOOTROM_IN_SRAM
#if defined(TCSUPPORT_CT)
int do_go(int argc, char *argv[]);
#else
static int do_go(int argc, char *argv[]);
#endif
static int do_decomp(int argc, char *argv[]);
#endif
static int do_memrl(int argc, char *argv[]);
static int do_memwl(int argc, char *argv[]);
static int do_dump(int argc, char *argv[]);
static int do_jump(int argc, char *argv[]);
static int do_flash(int argc, char *argv[]);
#ifdef MT7505_GDMP
static int doGDmpCfg(int argc, char *argv[]);
static int doSysGdmpMem(int argc, char *argv[]);
#endif
static int do_imginfo(int argc, char *argv[]);
#if defined(TCSUPPORT_NEW_SPIFLASH_DEBUG)
static int do_sf_basic_test(int argc, char *argv[]);
static int do_sf_mats(int argc, char *argv[]);
static int do_sf_be(int argc, char *argv[]);
static int do_sf_readidle_test(int argc, char *argv[]);
static int do_sf_bootin4Byte(int argc, char *argv[]);
static int do_sf_nonAlign_test(int argc, char *argv[]);
#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG_EXT
static int do_sf_memcpy(int argc, char *argv[]);
static int do_sf_1ByteTest(int argc, char *argv[]);
static int do_sf_ClockSet(int argc, char *argv[]);
static int do_sf_TnSet(int argc, char *argv[]);
#endif
#endif
#if defined(TCSUPPORT_CT_BOOTLOADER_UPGRADE)
#ifndef TCSUPPORT_RESERVEAREA_EXTEND
static int do_passwd(int argc, char *argv[]);
#endif
#endif

#if defined(TCSUPPORT_NEW_SPIFLASH_DEBUG)
static int do_spinand_rw_test(int argc, char *argv[]);
static int do_spinand_read(int argc, char *argv[]);
static int do_spinand_write(int argc, char *argv[]);
static int do_spinand_erase(int argc, char *argv[]);
static int do_spinand_nonalign(int argc, char *argv[]);
static int do_spicontroller_reg_test(int argc, char *argv[]);
static int do_spicontroller_interrupt_test(int argc, char *argv[]);
static int do_spicontroller_preload_test(int argc, char *argv[]);
void spicontroller_preload_function_test( void );
static int do_spicontroller_cpol_test(int argc, char *argv[]);
static int do_spicontroller_strap_test(int argc, char *argv[]);
static int do_spi_frequency(int argc, char *argv[]);


#endif

#ifdef NAND_TEST_CMD
static int init_nand(int argc, char *argv[]);
static int read_nand(int argc, char *argv[]);
static int erase_nand(int argc, char *argv[]);
static int check_data(int argc, char *argv[]);
static int innand(int argc, char *argv[]);
#ifdef TCSUPPORT_NAND_RT63368
static int check_badblock(int argc, char *argv[]);
static int mark_badblock(int argc, char *argv[]);
static int erase_block_phy(int argc, char *argv[]);
static int read_page_phy(int argc, char *argv[]);
static int write_page_phy(int argc, char *argv[]);
#endif
#endif
#ifdef BOOTROM_BACKDOOR
static int do_bdconfig_store(int argc, char *argv[]);
static int do_bdconfig_show(int argc, char *argv[]);
#ifdef BACKDOOR_SWITCH
static int do_bdconfig_switch (int argc, char *argv[]);
static int do_ddrcal_switch(int argc, char *argv[]);
static int do_dram_bist_switch(int argc, char *argv[]);
#endif
#endif
#ifndef BOOTROM_IN_SRAM
static int do_ipaddr(int argc, char *argv[]);
#endif
#ifdef BOOT_LZMA_SUPPORT
static int do_httpd(int argc, char *argv[]);
#endif
#ifdef SPI_TEST_CMD
static int do_erase(int argc, char *argv[]);
static int do_program(int argc, char *argv[]);
#endif
static int do_xmodem_rx(int argc, char *argv[]);
#ifdef BOOTROM_IN_SRAM
static int do_xmodem_rx_flash(int argc, char *argv[]);
#endif
#ifndef BOOTROM_IN_SRAM
static int do_miir(int argc, char *argv[]);
static int do_miiw(int argc, char *argv[]);
#endif

#if defined(MT7530_SLT)
static int doPhyMMDRead(int argc, char *argv[], void *p);
static int doPhyMMDWrite(int argc, char *argv[], void *p);
static int doTCMiiBPRead(int argc, char *argv[], void *p);
static void doTCMiiBPWrite(int argc, char *argv[], void *p);
static int doPhyMiiRead (int argc, char *argv[], void *p);
static int doPhyMiiWrite (int argc, char *argv[], void *p);
static int doDumpGSW(int argc, char *argv[]);
#endif
#if defined(MT7530_SLT) || defined(MT7530_SUPPORT)
static int doGswRead(int argc, char *argv[], void *p);
static int doGswWrite(int argc, char *argv[], void *p);
#endif

#ifdef SPI_TEST_CMD
static int do_memcmp(int argc, char *argv[]);
static int do_dmacpy(int argc, char *argv[]);
static int do_memcpy(int argc, char *argv[]);
static int do_memcmp4(int argc, char *argv[]);
static int do_memcpy4(int argc, char *argv[]);
static int do_memcmp2(int argc, char *argv[]);
static int do_memcpy2(int argc, char *argv[]);
static int do_memset(int argc, char *argv[]);
static int do_memset4(int argc, char *argv[]);
static int do_memset42(int argc, char *argv[]);
static int do_memset44(int argc, char *argv[]);
static int do_incr(int argc, char *argv[]);
static int do_incrcmp(int argc, char *argv[]);
static int do_rreadb(int argc, char *argv[]);
static int do_rreadl(int argc, char *argv[]);
static int do_rwritel(int argc, char *argv[]);
static int do_step(int argc, char *argv[]);
static int do_spidbg(int argc, char *argv[]);

static int do_spiqe(int argc, char *argv[]);
static int do_spird(int argc, char *argv[]);
#ifdef ETH_TEST_CMD
static int do_ethsend(int argc, char *argv[]);
static int do_ethsend2(int argc, char *argv[]);
static int do_ethdump(int argc, char *argv[]);
#endif

extern int memcmp4(const void * cs,const void * ct,size_t count);
extern void * memcpy4(void * dest,const void *src,size_t count);
extern int memcmp2(const void * cs,const void * ct,size_t count);
extern void * memcpy2(void * dest,const void *src,size_t count);
extern void * memset4(void * s,int c,size_t count);
#endif

#ifdef SPI_DRAM_TEST_CMD
static int do_dramtest(int argc, char *argv[]);
static int do_flashtest(int argc, char *argv[]);
static int do_spiregrw(int argc, char *argv[]);
static int do_gdmatestdram(int argc, char *argv[]);
static int do_gdmatestflash(int argc, char* argv[]);
static int do_gdmatestreg(int argc, char* argv[]);
#ifdef DRAM_PROTECT_TEST
static int do_rbus_init(int argc, char *argv[]);
static int do_dram_protect_size(int argc, char *argv[]);
static int do_dram_protect_test1(int argc, char *argv[]);
static int do_dram_protect_test2(int argc, char *argv[]);
static int do_dram_protect_test3(int argc, char *argv[]);
static int do_dram_protect_test4(int argc, char *argv[]);
static int do_seg_read_protect(int argc, char *argv[]);
static int do_seg_write_protect(int argc, char *argv[]);
static int do_seg_addr_set(int argc, char *argv[]);
static int do_seg_protect_test(int argc, char *argv[]);
static int do_mem_protect_test(int argc, char *argv[]);
static int do_mem_protect_test2(int argc, char *argv[]);
#if defined(TCSUPPORT_CPU_EN7521) && defined(DRAM_PROTECT_TEST)
int do_mem_protect_dbgLevel(int argc, char *argv[]);
int do_nmi_test_enable(int argc, char *argv[]);
#endif
#endif
#endif
#if defined(RBUS_COUNTER_TEST)
static int do_rbus_counter(int argc, char *argv[]);
#endif

#ifdef MT7550_GPIO_TEST//tony add
static int do_gpio_test(int argc, char *argv[]);
#endif

#if defined(TCSUPPORT_FWC_ENV)
static int do_show_all_flash_env_info(void);
static int do_show_all_dual_image_flag_info(void);
static int do_boot_debug_level(int argc, char *argv[]);
static int do_boot_reset(int argc, char *argv[]);
static int do_boot_version(int argc, char *argv[]);
static int do_flash_erase(int argc, char *argv[]);
static int do_personality_parm_cmd(int argc, char *argv[]);
static int do_stop_restart(int argc, char *argv[]);
#endif

static int do_cpufreq(int argc, char *argv[]);
#ifdef BOOT_LZMA_SUPPORT
extern unsigned long Jiffies;
void tc_mdelay(int delaytime);
static char *upload_buf = (unsigned char *)0x80020000;
static char *p_upload_buf = (unsigned char *)0x80020000;
unsigned char file_recv = 0;
#endif
//static unsigned char file_recv = 0;
#if defined(TCSUPPORT_CT_BOOTLOADER_UPGRADE)
static unsigned char auth_recv = 0;
static unsigned char post_recv = 0;
static unsigned char pass_auth = 0;
#endif

#ifdef MT7505_SWITCH_TBL_VERIRY
void MT7505_Switch_Tbl_Test(void);
#endif

#ifdef TC3262
#ifndef BOOTROM_IN_SRAM
static int do_ddrdrv(int argc, char *argv[]);
#endif
#ifdef DRAM_PROTECT_TEST
#define RBUS_INT				1
static int record1[1000][6]={0}; /* startAddr, size, overSize,      seg1 & seg2,        resv,    resv */
static int record2[1000][6]={0}; /* valid,     resv, overSize_fail, forbidden_seg_fail, illInfo, illRegAddr */
static int record3_RegAddr[1000]={0}; /*exillRegAddr */
static int record4_illInfo[1000]={0}; /*exillInfo */


static int test_round=0;
static int ex_addr_count  = 0;
static int ex_info_count  = 0;
void RbusIsr(int irq, void *dev_id, struct pt_regs *regs);
struct irqaction rbus_irq = {RbusIsr, NULL, 0, "rbus_isr", NULL, NULL};
#endif
#endif


static const cmds_t CMDS[] = {
	{ "",		do_null,		0,	"", 							""},
	{ "?",		do_help,		0,	"?", 							"Print out help messages."},
	{ "help",	do_help,		0,	"help",							"Print out help messages."},
#if defined(TCSUPPORT_FWC_ENV)
	{ "show_flash_info", do_show_all_flash_env_info, 0,  "show_flash_info  flash all env", "display flash all env"},
	{ "show_dual_image", do_show_all_dual_image_flag_info, 0,  "show_dual_image  dual image flag", "display dual image flag env"},
	{ "debug_level",     do_boot_debug_level,     	 2,  "debug_level <0/1>",              "set debug level."},
	{ "reset",			 do_boot_reset,           	 0,  "reset",                          "board reset"},
	{ "boot_version",	 do_boot_version,            0,  "boot_version",	               "boot version"},
	{ "flash_erase",	 do_flash_erase,		     3,  "flash_erase <addr> <len>",	   "erase flash"},
	{ "dual_image", 	 do_personality_parm_cmd,    4,  "dual_image <set/get> <commit/active> <kernel/rootfs/app1/app2> <value>",   "dual_image commit/active flag"},
	{ "fwc", 	 		 do_personality_parm_cmd,    3,  "fwc <set/get> <snoui/hwcfg/ethaddr> <value>", "snoui/hwcfg/etaddr  env"},
    { "stop_restart",    do_stop_restart,            0,  "stop_restart",                   "stop bootloader restart"},
#endif
#ifndef BOOTROM_IN_SRAM
#if !defined(TCSUPPORT_FWC_ENV)
	{ "go",		do_go,			0,	"go",							"Booting the linux kernel."},
	{ "decomp",	do_decomp,		0,	"decomp",						"Decompress kernel image to ram."},
#endif
#endif
	{ "memrl", 	do_memrl,		2,	"memrl <addr>",					"Read a word from addr."},
	{ "memwl",	do_memwl,		3,	"memwl <addr> <value>", 		"Write a word to addr."},
	{ "dump",	do_dump,		3,	"dump <addr> <len>",			"Dump memory content."},
	{ "jump",	do_jump,		2,	"jump <addr>", 					"Jump to addr."},
#ifdef TCSUPPORT_BB_NAND
	{ "flash",	do_flash,		4,	"flash <dst> <src> <len> <oob>",		"Write to flash from src to dst(oob: write nand oob if 1)."},
#else
	{ "flash",	do_flash,		4,	"flash <dst> <src> <len>",		"Write to flash from src to dst."},
#endif
	{ "imginfo",	do_imginfo,		0,	"imginfo",		"Show images info."},
#ifdef MT7505_GDMP
	{"gdmpr", 	doGDmpCfg,			1,	"gdmpr",	"Dump GDMP Registers."},
	{"gdmpm",		doSysGdmpMem,	3,	"gdmpm <start> <len>",	"Dump GDMP Memory."},
#endif
#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
	{ "sftest", do_sf_basic_test,		1,	"sftest <round>",				"New SPI FLASH Test."},
	{ "sfmats", do_sf_mats,		1,	"sfmats [Round] [Scope:0~1] [isChipTest:0~1] [Exit:0~1] [ReadMode:0~7] [StartAddr]",				"New SPI FLASH MATS+ Test."},
	{ "sfbe", 	do_sf_be,		1,	"sfbe",							"New SPI FLASH Bulk Erase Test."},
	{ "sfreadidle", do_sf_readidle_test,	1,	"sfreadidle <idleEn:0~1> <blockNum:0~100> <4ByteRead:0~1>",	"New SPI FLASH Read Idle Enable Test."},
	{ "sfboot4", do_sf_bootin4Byte,		1,	"sfboot4 <AddrMode>",	"New SPI FLASH boot in 4Byte Mode for W25Q256FV."},
	{ "sfnonalign", do_sf_nonAlign_test,		1,	"sfnonalign",				"New SPI FLASH Test."},
#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG_EXT
	{ "sfmemcpy", do_sf_memcpy,		1,	"sfmemcpy <AddrMode>",	"New SPI FLASH boot in 4Byte Mode for W25Q256FV."},
	{ "sf1ByteReadTest", do_sf_1ByteTest,		1,	"sf1ByteReadTest <AddrMode>",	"1Byte Read Test via Old/New SPI Flash Controller."},
	{ "sfclockset", do_sf_ClockSet,		1,	"sfclockset <0~31>",				"New SPI FLASH Clock Set."},
	{ "sftnset", do_sf_TnSet,		1,	"sftnset <0~5>",				"New SPI FLASH Tn Set."},
#endif


#if defined(TCSUPPORT_CPU_EN7512)||defined(TCSUPPORT_CPU_EN7521)
#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG
	{ "spinand_rwtest", do_spinand_rw_test, 	1,	"spi nand rw test", 		"SPI NAND Test"},
	{ "spinand_read",   do_spinand_read, 	    1,	"spi nand read", 			"SPI NAND Read"},
	{ "spinand_write",   do_spinand_write, 	    1,	"spi nand write", 			"SPI NAND Write"},
	{ "spinand_erase",   do_spinand_erase, 	    1,	"spi nand erase", 			"SPI NAND Erase"},
	{ "spinand_nonaligntest",   do_spinand_nonalign, 	    1,	"spi nand nonalign", 			"SPI NAND NonAlign"},
	{ "spicontroller_reg_test",   do_spicontroller_reg_test, 	    1,	"spi controller regs test", 			"SPI CONTROLLER REGS TEST"},
	{ "spicontroller_interrupt_test",   do_spicontroller_interrupt_test, 	    1,	"spi controller interrupt test", 			"SPI CONTROLLER INTERRUPT TEST"},
	{ "spicontroller_preload_test",   do_spicontroller_preload_test, 	    1,	"spi controller preload test", 			"SPI CONTROLLER PRELOAD TEST"},
	{ "spicontroller_cpol_test",   do_spicontroller_cpol_test, 	    1,	"spi controller cpol test", 			"SPI CONTROLLER CPOL TEST"},
	{ "spicontroller_strap_test",   do_spicontroller_strap_test, 	    1,	"spi controller strap test", 			"SPI CONTROLLER STRAP TEST"},
	{ "spi_frequency",   do_spi_frequency, 	    1,	"spi_frequency", 			"SPI FREQUENCY TEST"},
#endif
#endif


#endif
#ifdef NAND_TEST_CMD
	{ "nandit",	init_nand,		2,	"nandit <addr>",				"Write to flash from src to dst."},
	{ "nandrd",	read_nand,		3,	"nandrd <src> <len>",			"Read flash from src."},
	{ "nander",	erase_nand,		3,	"nander <dst> <len> <oob>",		"Erase flash from dst(oob: force erase oob if 1)."},	
	{ "chkmem", check_data,		4,	"chkmem <nand> <ddr> <len>",	"Check data between nand and ddr."},
	{ "innand", innand,			4,	"innand <dst> <src> <len>",		"Move data from spi to nand."},
#ifdef TCSUPPORT_NAND_RT63368
    { "checkbb",  check_badblock,   2,	"checkbb <total block>",	"Check nandflash bad block."},
    { "markbb",   mark_badblock,    2,	"markbb <block>",		    "Mark nandflash bad block."},
    { "erasepb",  erase_block_phy,  3,	"erasepb <block> <num>",	"Erase nandflash physical block."},
    { "readpg",   read_page_phy,    3,  "readpg <page> <len>",      "Read nandflash physical page."},
    { "writepg",   write_page_phy,    5,  "writepg <page> <byte index> <value> <ecc>",  
    			"change one byte of page"},
#endif
#endif	
#ifdef BOOTROM_BACKDOOR
	{ "bdstore",	do_bdconfig_store,	3,	"bdstore <flash dst> <bin src>",		"Do backdoor config store"},
	{ "bdshow",	do_bdconfig_show,	0,	"bdshow",		"Show backdoor config "},
#ifdef BACKDOOR_SWITCH	
	{ "bdswitch",	do_bdconfig_switch,	0,	"bdswitch[1|0]",		"Enable or disable backdoor function "},
	{ "ddrcalswitch",	do_ddrcal_switch,	0,	"ddrcalswitch[1|0]",		"Enable or disable ddr calibration funciton "},
	{ "drambistswitch",	do_dram_bist_switch,	0,	"drambistswitch[0|1|2]",		"disable or enable, and quick or normal test "},
#endif
#endif
#if defined(TCSUPPORT_CT_BOOTLOADER_UPGRADE)
#ifndef TCSUPPORT_RESERVEAREA_EXTEND
	{ "passwd", do_passwd,      3,    "passwd <username> <password>",   "Change username and password"},
#endif
#endif
#ifdef SPI_TEST_CMD
	{ "erase",	do_erase,		3,	"erase <dst> <len>",			"Erase flash."},
	{ "program",do_program,		4,	"program <dst> <src> <len>",	"Program flash."},
#endif
	{ "xmdm",	do_xmodem_rx,	3,	"xmdm <addr> <len>",			"Xmodem receive to addr."},
#ifdef BOOTROM_IN_SRAM
	{ "xmdmf",	do_xmodem_rx_flash,	4,	"xmdmf <addr> <len> <flash addr>",			"Xmodem receive to addr and Write to flash."},
#endif
#ifndef BOOTROM_IN_SRAM	
	{ "miir",	do_miir,		3,	"miir <phyaddr> <reg>",			"Read ethernet phy reg."},
	{ "miiw",	do_miiw,		4,	"miiw <phyaddr> <reg> <value>", "Write ethernet phy reg."},
#if defined(MT7530_SLT) || defined(MT7530_SUPPORT)
	{ "gswr",	doGswRead,   	2,	"gswr <reg>",         			"Read gsw reg."},
	{ "gsww",	doGswWrite,  	3,	"gsww <reg> <value>", 			"Write gsw reg."},
#endif
#endif
#if defined(MT7530_SLT)
	{"emiir",   doPhyMMDRead,   4,  "emiir <phyaddr> <devaddr> <reg>"},
	{"emiiw",   doPhyMMDWrite,  4,  "emiiw <phyaddr> <devaddr> <reg> <val>"},
	{"miibpr",  doTCMiiBPRead,  3,  "miibpr <phyAddr> <reg>"},
	{"miibpw",  doTCMiiBPWrite, 4,  "miibpw <phyAddr> <reg> <value>"},
	{ "phyr",	doPhyMiiRead,   2, "phyr <phyaddr> <reg>",         "Read ethernet phy reg."},
	{ "phyw",	doPhyMiiWrite,  2, "phyw <phyaddr> <reg> <value>", "Write ethernet phy reg."},
	{ "dumpgsw",doDumpGSW,  	2, "dumpgsw <port>", 				"Dump gsw reg."},
#endif

#ifdef SPI_TEST_CMD
	{ "memcmp",	do_memcmp,		4,	"memcmp <addr1> <addr2> <len>", "Memory compare."},
	{ "dmacpy",	do_dmacpy,		4,	"dmacpy <addr1> <addr2> <len>", "DMA Memory copy."},
	{ "memcpy",	do_memcpy,		4,	"memcpy <addr1> <addr2> <len>", "Memory copy."},
	{ "memcmp4",do_memcmp4,		4,	"memcmp4 <addr1> <addr2> <len>", "Memory compare."},
	{ "memcpy4",do_memcpy4,		4,	"memcpy4 <addr1> <addr2> <len>", "Memory copy."},
	{ "memcmp2",do_memcmp2,		4,	"memcmp2 <addr1> <addr2> <len>", "Memory compare."},
	{ "memcpy2",do_memcpy2,		4,	"memcpy2 <addr1> <addr2> <len>", "Memory copy."},
	{ "memset", do_memset,		4,	"memset <addr> <pattern> <len>", "Memory set."},
	{ "memset4",do_memset4,		4,	"memset4 <addr> <pattern> <len>", "Memory set."},
	{ "memset42",do_memset42,	5,	"memset42 <addr> <pattern1> <pattern2> <len>", "Memory set."},
	{ "memset44",do_memset44,	7,	"memset44 <addr> <pattern1~4> <len>", "Memory set."},
	{ "incr",	do_incr,		4,	"incr <addr> <pattern> <len>", "Memory set."},
	{ "incrcmp",do_incrcmp,		4,	"incrcmp <addr> <pattern> <len>", "Memory compare."},
	{ "rreadb", do_rreadb,		3,	"rreadb <addr1> <adddr2>", "Random read byte."},
	{ "rreadl", do_rreadl,		4,	"rreadl <addr1> <adddr2> <cnt>", "Random read word."},
	{ "rwritel",do_rwritel,		5,	"rwritel <addr1> <adddr2> <pattern> <cnt>", "Random read word."},
	{ "step",	do_step,		6,	"step <addr> <pattern1> <pattern2> <step> <len>", "Memory set."},
	{ "spidbg",	do_spidbg,		2,	"spidgb <1|0>",					"spi flash debug."},
	{ "spiqe",	do_spiqe,		2,	"spiqe <hpm>",					""},
	{ "spird",	do_spird,		0,	"spird",					""},
#endif
#ifdef ETH_TEST_CMD
	{ "ethsend", do_ethsend,	3,	"ethsend <len> <cnt>", "Send ethernet frame."},
	{ "ethsend2",do_ethsend2,	3,	"ethsend2 <len> <cnt>", "Send ethernet frame."},
	{ "ethdump", do_ethdump,	0,	"ethdump", "Dump ethernet TX/RX ring"},
#endif
	//{ "cpufreq",do_cpufreq,		3,	"cpufreq <m> <n>", "Set CPU Freq"},
	{ "cpufreq",do_cpufreq,		2,	"cpufreq <freq num> / <m> <n>", "Set CPU Freq <156~450>(freq has to be multiple of 6)"},
#ifndef BOOTROM_IN_SRAM
	{ "ipaddr",	do_ipaddr,		2,	"ipaddr <ip addr>",		"Change modem's IP."},
#endif
#ifdef BOOT_LZMA_SUPPORT	
	{ "httpd",	do_httpd,		0,	"httpd",		"Start Web Server"},
#endif	
#ifdef TC3262
#ifndef BOOTROM_IN_SRAM
	{ "ddrdrv",	do_ddrdrv,		0,	"ddrdrv <..>",		"Change DDR driving length"},	
#endif	
#endif	
#ifdef SPI_DRAM_TEST_CMD
	{ "dramtest",do_dramtest,	0,	"dramtest <addr> <size> <pat> <w Byte> <infinite>",		""},
	{ "flashtest",do_flashtest,	0,	"flashtest <addr> <size> <pat> <infinite>",				""},
	{ "spiregrw",do_spiregrw,	0,	"spiregrw",				"spi ctrler reg rw test"},
	{ "gdmatestdram",do_gdmatestdram,	0,	"gdmatestdram <infinite>",				""},
	{ "gdmatestflash",do_gdmatestflash,	0,	"gdmatestflash <infinite>",				""},
	{ "gdmatestreg",do_gdmatestreg,	0,	"gdmatestreg",				""},
#if defined(DRAM_PROTECT_TEST)

/****************************** memory access range protection *****************************/
	{ "rbus_init",          do_rbus_init,          1, "rbus_init", "register rbus isr."}, 
	{ "dram_protect_size",  do_dram_protect_size,  2, "dram_protect_size <dram size>", "Dram protect size set."},
	{ "dram_protect_test1", do_dram_protect_test1, 4, "dram_protect_test1 <counter> <isUnCache:0|1> <cpu:0|dma:1>",
													  	  "Dram memory protection test case 1"},
	{ "dram_protect_test2", do_dram_protect_test2, 4, "dram_protect_test2 <counter> <isUnCache:0|1> <cpu:0|dma:1>", 
														  "Dram memory protection test case 2"},
	{ "dram_protect_test3", do_dram_protect_test3, 4, "dram_protect_test3 <counter> <isUnCache:0|1> <cpu:0|dma:1>", 
														  "Dram memory protection test case 3"},
	{ "dram_protect_test4", do_dram_protect_test4, 4, "dram_protect_test4 <counter> <isUnCache:0|1> <cpu:0|dma:1>", 
														  "Dram memory protection test case 4"},
	{ "seg_read_protect",   do_seg_read_protect,   2, "seg_reade_protect <disable:0|enable:1>", 
													  "Segmentation memory read protection set"},
	{ "seg_write_protect",  do_seg_write_protect,  2, "seg_write_protect <disable:0|enable:1>", 
													  "Segmentation memory write protection set"},
	{ "seg_addr_set",       do_seg_addr_set,       4, "seg_addr_set <seg1:0|seg2:1> <start_addr> <end_addr>", 
													  "Segmentation memory address set"},
	{ "seg_read_protect",   do_seg_read_protect,   2, "seg_read_protect <disable:0|enable:1>", 
													  "Segmentation memory read protection set"},
	{ "seg_protect_test",   do_seg_protect_test,   6, "seg_protect_test <counter> <seg1:0|seg2:1> <isUnCache:0|1> <cpu:0|dma:1> <testCase: 0|1|2|3|4>", "Segmentation memory protection test case"},
	{ "mem_protect_test",   do_mem_protect_test,   4, "mem_protect_test <counter> <isUnCache:0|1> <cpu:0|dma:1> ", "Memory protection test"},
	{ "mem_protect_test2",   do_mem_protect_test2,   7, "mem_protect_test <counter> <isUnCache:0|1> <cpu:0|dma:1> <Add:0|Subtraction:1><byte offset> <Test Length>", "Memory protection test"},
#if defined(TCSUPPORT_CPU_EN7521) && defined(DRAM_PROTECT_TEST)
    { "mem_protect_dbgLevel", do_mem_protect_dbgLevel, 1, "mem_protect_dbgLevel 0~0xff (bitMap)", "set debug level for memory protection tests"},
    { "nmi_test_enable",    do_nmi_test_enable,    1, "nmi_test_enable 0/1", "dis/enable nmi test in LAN driver ISR"},
#endif
/****************************** memory access range protection *****************************/
#endif
#endif
#if defined(RBUS_COUNTER_TEST)
	{ "rbus_counter",   do_rbus_counter,   0, "rbus_counter <type> <MIN~MAX> <length>"},
#endif
#ifdef MT7550_GPIO_TEST//tony add
	{ "gpio_test",   do_gpio_test,   0, "gpio_test"},
#endif
#ifdef TCSUPPORT_MTD_PARTITIONS_CMDLINE
	{"mtd", do_get_mtd_info, 0, "mtd"},
#endif
	{ NULL,		NULL,			0,	NULL,		NULL}
};

static void cmd_pad_space(char *buf, int len)
{
	int slen = len - strlen(buf);
	char *cp = buf;
	int i;

	if (slen) {
		cp += strlen(buf);
		for (i = 0; i < slen; i++)
			*cp++ = ' ';
		*cp ='\0';
	}
}

int do_null(int argc, char *argv[])
{
}

int do_help(int argc, char *argv[])
{
	cmds_t *cmdp;
	char buf[80];

	for (cmdp = CMDS; cmdp->name != NULL; cmdp++) {
		if (cmdp->usage != NULL) {
			strcpy(buf, cmdp->usage);
			cmd_pad_space(buf, 36);
			printf("%s", buf);
		}

		if (cmdp->help != NULL) {
			strcpy(buf, cmdp->help);
			cmd_pad_space(buf, 42);
			printf("%s", buf);
		}

		printf("\n");
	}

	return 0;
}

extern int set_lzma_addr(char* lzma_data_start, char* lzma_data_end);
extern unsigned long decompress_kernel(unsigned long output_start, unsigned long free_mem_ptr_p, unsigned long free_mem_ptr_end_p);
extern void macResetSwMAC(void);

#ifdef CONFIG_DUAL_IMAGE
extern int checkimage(int imageflag);
#ifdef TCSUPPORT_DUAL_IMAGE_ENHANCE
extern unsigned short CheckSum(unsigned short *szBUF,int iSize);
#endif
#endif
#ifndef BOOTROM_IN_SRAM
int do_decomp(int argc, char *argv[])
{
	unsigned long output_data;
	unsigned long free_mem_ptr;
	unsigned long free_mem_ptr_end;
    void (*jumpAddr)();
#ifdef CONFIG_DUAL_IMAGE
	int retflag = 0;
	char *bufaddr = (char*)FLAG_ADDR;
	char flag = 0;
	
#ifdef TCSUPPORT_DUAL_IMAGE_ENHANCE

	char* trx_addr = NULL;
	unsigned char slave_ver[32], main_ver[32];
	int index;
	unsigned int header_len, kernel_len, rootfs_len;

	unsigned long retlen;
	int binlen = 0;
	struct trx_header *trx_main = NULL;
	struct trx_header *trx_slave = NULL;
	unsigned short checkSum_main = 0;
	unsigned short checkSum_slave = 0;
	char checkflag = 0;
#endif
#endif	
	struct trx_header *trx_temp = NULL;
	char* temp_addr = NULL;
	unsigned long startAddr = 0x0;
	unsigned int kernel_size = 0;
	int trx_header_len = sizeof(struct trx_header);

	output_data = KERNEL_START_ADDR;
	free_mem_ptr = KERNEL_RAM_START;
	free_mem_ptr_end = KERNEL_RAM_END;

	if (tc_inl(CR_INTC_IMR) != 0x0)
		macResetSwMAC();
	tc_outl(CR_INTC_IMR, 0x0);
	tc_outl(CR_TIMER_CTL, 0x0);

#ifdef CONFIG_DUAL_IMAGE
	if(checkimage(MAIN_IMAGE) < 0)
#ifdef TCSUPPORT_DUAL_IMAGE_ENHANCE
		trx_temp = (struct trx_header*)(flash_base+flash_tclinux_start+offset);
#else
		trx_temp = (struct trx_header*)(flash_base+flash_tclinux_start+TCLINUX_SLAVE_FLASH_START);
#endif
	else
#endif
	trx_temp = (struct trx_header*)(flash_base+flash_tclinux_start);

	temp_addr = &(trx_temp->kernel_len);
	kernel_size = READ_FLASH_DWORD(temp_addr)+0x100;
	kernel_size = (kernel_size+0x10000)&~(0x10000-1);

	temp_addr = &(trx_temp->decompAddr);
	startAddr = READ_FLASH_DWORD(temp_addr);
	if(startAddr != 0)
	{
		output_data = startAddr;
#ifdef CONFIG_DUAL_IMAGE
		bufaddr = (char*)(startAddr-1);
#endif
	}
	else
	{
		output_data = 0x80020000;
#ifdef CONFIG_DUAL_IMAGE
		bufaddr = (char*)(0x8001ffff);
#endif
	}
	
#ifdef CONFIG_DUAL_IMAGE
	retflag = checkimage(MAIN_IMAGE);
	if(retflag < 0)
	{
		flag = 1;
		retflag = checkimage(SLAVE_IMAGE);
		if(retflag < 0)
			return;
	}
	//store flag information to 0x8001ffff
	*bufaddr = flag;
#endif

	printf("Decompress to %X free_mem_ptr=%X free_mem_ptr_end=%X\n", output_data, free_mem_ptr, free_mem_ptr_end);

#ifdef CONFIG_DUAL_IMAGE
	/*flag 0:main image;1:backup image*/
	/* frankliao added 20100806 */
	if (flag==1){ 
		prom_printf("from slave\n");
#ifdef TCSUPPORT_DUAL_IMAGE_ENHANCE
//		set_lzma_addr(0xb0020100+offset, 0xb0120000+offset);
//		set_lzma_addr(flash_base+0x20100+offset, flash_base+0x120000+offset);
		set_lzma_addr(flash_base+flash_tclinux_start+trx_header_len+offset, flash_base+flash_tclinux_start+kernel_size+offset);

		//copy slave to main
//		trx_slave = (struct trx_header *)(0xb0000000+0x20000+offset);
		trx_slave = (struct trx_header *)(flash_base+flash_tclinux_start+offset);

		trx_addr = &(trx_slave->header_len);
		header_len = READ_FLASH_DWORD(trx_addr);
		trx_addr = &(trx_slave->kernel_len);
		kernel_len = READ_FLASH_DWORD(trx_addr);
		trx_addr = &(trx_slave->rootfs_len);
		rootfs_len = READ_FLASH_DWORD(trx_addr);
//		binlen = trx_slave->header_len + trx_slave->kernel_len + trx_slave->rootfs_len;
		binlen = header_len + kernel_len + rootfs_len;
		flash_erase(flash_tclinux_start, binlen);
		if (IS_NANDFLASH)
			flash_read(flash_base+flash_tclinux_start+offset, binlen, &retlen, 0x80020000);
		else
			memcpy(0x80020000, flash_base+flash_tclinux_start+offset, binlen);
//		memcpy(0x80020000, 0xb0000000+0x20000+offset, binlen);
		flash_write(flash_tclinux_start, binlen, &retlen, 0x80020000);
#else
//		set_lzma_addr(0xb0020100+0x500000, 0xb0120000+0x500000);
	//	set_lzma_addr(flash_base + 0x20100 + 0x500000, flash_base + 0x120000 + 0x500000);
		set_lzma_addr(flash_base + flash_tclinux_start+trx_header_len + TCLINUX_SLAVE_FLASH_START, flash_base + flash_tclinux_start+kernel_size+ TCLINUX_SLAVE_FLASH_START);

#endif
	} else {
		prom_printf("from main\n");
//		set_lzma_addr(0xb0020100, 0xb0120000);
	//	set_lzma_addr(flash_base + 0x20100, flash_base + 0x120000);
		set_lzma_addr(flash_base + flash_tclinux_start+trx_header_len, flash_base +flash_tclinux_start+ kernel_size);//xflu

#ifdef TCSUPPORT_DUAL_IMAGE_ENHANCE
		//copy main to slave

		trx_main = (struct trx_header*)(flash_base+flash_tclinux_start);
		trx_addr = &(trx_main->header_len);
		header_len = READ_FLASH_DWORD(trx_addr);
		trx_addr = &(trx_main->kernel_len);
		kernel_len = READ_FLASH_DWORD(trx_addr);
		trx_addr = &(trx_main->rootfs_len);
		rootfs_len = READ_FLASH_DWORD(trx_addr);
		binlen = header_len + kernel_len + rootfs_len;
		trx_slave = (struct trx_header *)(flash_base+flash_tclinux_start+offset);
		trx_addr = &(trx_slave->version);
		for (index=0; index<32;  index++, trx_addr++){
			slave_ver[index] = READ_FLASH_BYTE(trx_addr);
		}
		trx_addr = &(trx_main->version);
		for (index=0; index<32;  index++, trx_addr++){
			main_ver[index] = READ_FLASH_BYTE(trx_addr);
		}
#if 0
		trx_main = (struct trx_header *)(0xb0000000+0x20000);
		binlen = trx_main->header_len + trx_main->kernel_len + trx_main->rootfs_len;
		trx_slave = (struct trx_header *)(0xb0000000+0x20000+offset);
#endif		
//		if((checkimage(SLAVE_IMAGE) < 0) || (strcmp(trx_main->version,trx_slave->version) != 0)) {
		if((checkimage(SLAVE_IMAGE) < 0) || (strcmp(main_ver, slave_ver) != 0)) {
			checkflag = 1;
		}

		if(checkflag == 0){
			if (IS_NANDFLASH)
				flash_read(flash_base+flash_tclinux_start, binlen, &retlen, 0x80020000);
			else
				memcpy(0x80020000, flash_base+flash_tclinux_start, binlen);
			checkSum_main = CheckSum(0x80020000,binlen);
			if (IS_NANDFLASH)
				flash_read(flash_base+flash_tclinux_start+offset, binlen, &retlen, 0x80020000);
			else
				memcpy(0x80020000, flash_base+flash_tclinux_start+offset, binlen);
			checkSum_slave = CheckSum(0x80020000,binlen);
			if(checkSum_main != checkSum_slave){
				checkflag = 1;
			}
		}
		if(checkflag == 1){
			flash_erase(flash_tclinux_start+offset, binlen);
			if (IS_NANDFLASH)
				flash_read(flash_base+flash_tclinux_start, binlen, &retlen, 0x80020000);
			else
				memcpy(0x80020000, flash_base+flash_tclinux_start, binlen);
			flash_write(flash_tclinux_start+offset, binlen, &retlen, 0x80020000);
		}
#if 0
		if(checkflag == 0){
			memcpy(0x80020000, 0xb0000000+0x20000, binlen);
			checkSum_main = CheckSum(0x80020000,binlen);
			memcpy(0x80020000, 0xb0000000+0x20000+offset, binlen);
			checkSum_slave = CheckSum(0x80020000,binlen);
			if(checkSum_main != checkSum_slave)
				checkflag = 1;
		}
		if(checkflag == 1){
			flash_erase(0x20000+offset, binlen);
			memcpy(0x80020000, 0xb0000000+0x20000, binlen);
			flash_write(0x20000+offset, binlen, &retlen, 0x80020000);
		}
#endif		
#endif		
	}
#else
//	set_lzma_addr(KERNEL_FLASH_START, KERNEL_FLASH_END);
	set_lzma_addr( (flash_base + flash_tclinux_start+trx_header_len), (flash_base + LZMA_FLASH_LARGE_END) );
#endif

	decompress_kernel(output_data, free_mem_ptr, free_mem_ptr_end);
#ifdef TC3262
	if (IS_NANDFLASH )
		flush_icache_range(startAddr, startAddr + kernel_size*4);
#endif
	return 0;
}

int do_go(int argc, char *argv[])
{
	unsigned long output_data;
	unsigned long free_mem_ptr;
	unsigned long free_mem_ptr_end;
    void (*jumpAddr)();

#ifdef CONFIG_DUAL_IMAGE
	int retflag = 0;
	char *bufaddr = (char*)FLAG_ADDR;
	char flag = 0;
#if defined(TCSUPPORT_DUAL_IMAGE_ENHANCE) || defined(TCSUPPORT_NAND_BADBLOCK_CHECK)
	char *trx_addr = NULL;
	unsigned char slave_ver[32], main_ver[32];
	unsigned int header_len, kernel_len, rootfs_len;
	int index;
	unsigned long retlen;
	int binlen = 0;
	struct trx_header *trx_main = NULL;
	struct trx_header *trx_slave = NULL;
	unsigned short checkSum_main = 0;
	unsigned short checkSum_slave = 0;
	char checkflag = 0;
#endif
#endif	
	struct trx_header *trx_temp = NULL;
	char* temp_addr = NULL;
	unsigned long startAddr = 0x0;
	unsigned int kernel_size = 0;
	int trx_header_len = sizeof(struct trx_header);
	
	output_data = KERNEL_START_ADDR;
	free_mem_ptr = KERNEL_RAM_START;
	free_mem_ptr_end = KERNEL_RAM_END;

	if(isRT65168){
		VPint(0xbfb00084) = 0x1; //reset DMT
		pause(20);
		VPint(0xbfb00084) = 0x0; //reset DMT
		pause(1);
		VPint(0xbf500424) = 0x6004; //write AFE reg4
	}

	if (tc_inl(CR_INTC_IMR) != 0x0)
		macResetSwMAC();
	tc_outl(CR_INTC_IMR, 0x0);
	tc_outl(CR_TIMER_CTL, 0x0);

#ifdef CONFIG_DUAL_IMAGE
	if(checkimage(MAIN_IMAGE) < 0)
#ifdef TCSUPPORT_DUAL_IMAGE_ENHANCE
		trx_temp = (struct trx_header*)(flash_base+flash_tclinux_start+offset);
#elif defined(TCSUPPORT_NAND_BADBLOCK_CHECK)
        trx_temp = (struct trx_header*)(flash_base+0x2280000);
#else
		trx_temp = (struct trx_header*)(flash_base+flash_tclinux_start+TCLINUX_SLAVE_FLASH_START);
#endif
	else
#endif

#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK    
    trx_temp = (struct trx_header*)(flash_base+0x280000);
#elif defined(TCSUPPORT_NAND_RT63368)
    trx_temp = (struct trx_header*)(flash_base + NAND_KERNEL_OFFSET);    
#else
	trx_temp = (struct trx_header*)(flash_base+flash_tclinux_start);
#endif


  #if defined(TCSUPPORT_NAND_BADBLOCK_CHECK) || defined(TCSUPPORT_NAND_RT63368) 
        temp_addr = &(trx_temp->kernel_len);    
        kernel_size = READ_FLASH_DWORD(temp_addr) + 0x100;
  #else
  		temp_addr = &(trx_temp->kernel_len);
  		kernel_size = READ_FLASH_DWORD(temp_addr) + 0x100;
		kernel_size = (kernel_size+0x10000)&~(0x10000-1);//64k aligin
  #endif


	temp_addr = &(trx_temp->decompAddr);
	startAddr = READ_FLASH_DWORD(temp_addr);
	if(startAddr != 0){
		output_data = startAddr;
#ifdef CONFIG_DUAL_IMAGE
		bufaddr = (char*)(startAddr-1);
#endif
	}
	else
	{
		output_data = 0x80020000;
#ifdef CONFIG_DUAL_IMAGE
		bufaddr = (char*)(0x8001ffff);
#endif
	}
	
#ifdef CONFIG_DUAL_IMAGE
	retflag = checkimage(MAIN_IMAGE);
	if(retflag < 0)
	{
		flag = 1;
		retflag = checkimage(SLAVE_IMAGE);
		if(retflag < 0)
			return;
	}
	//store flag information to 0x8001ffff
	*bufaddr = flag;
#endif

	printf("Decompress to %X free_mem_ptr=%X free_mem_ptr_end=%X\n", output_data, free_mem_ptr, free_mem_ptr_end);

#ifdef CONFIG_DUAL_IMAGE
	/*flag 0:main image;1:backup image*/
	/* frankliao added 20100806 */
	if (flag==1){ 
		prom_printf("from slave\n");
#ifdef TCSUPPORT_DUAL_IMAGE_ENHANCE
//		set_lzma_addr(0xb0020100+offset, 0xb0120000+offset);
	//	set_lzma_addr(flash_base+0x20100+offset, flash_base+0x120000+offset);
		set_lzma_addr(flash_base+flash_tclinux_start+trx_header_len+offset, flash_base+flash_tclinux_start+kernel_size+offset);

//		trx_slave = (struct trx_header *)(0xb0000000+0x20000+offset);
		trx_slave = (struct trx_header*)(flash_base+flash_tclinux_start+offset);
		trx_addr = &(trx_slave->header_len);
		header_len = READ_FLASH_DWORD(trx_addr);
		trx_addr = &(trx_slave->kernel_len);
		kernel_len = READ_FLASH_DWORD(trx_addr);
		trx_addr = &(trx_slave->rootfs_len);
		rootfs_len = READ_FLASH_DWORD(trx_addr);
//		binlen = trx_slave->header_len + trx_slave->kernel_len + trx_slave->rootfs_len;
		binlen = header_len + kernel_len + rootfs_len;

	#ifdef TCSUPPORT_NAND_RT63368
        flash_erase(NAND_KERNEL_OFFSET, binlen);
	#else
		flash_erase(flash_tclinux_start, binlen);
	#endif
		if (IS_NANDFLASH)
			flash_read(flash_base+flash_tclinux_start+offset, binlen, &retlen, 0x80020000);
		else
			memcpy(0x80020000, flash_base+flash_tclinux_start+offset, binlen);
	#ifdef TCSUPPORT_NAND_RT63368
        flash_write(NAND_KERNEL_OFFSET, binlen, &retlen, 0x80020000);
	#else
		flash_write(flash_tclinux_start, binlen, &retlen, 0x80020000);
	#endif	

#elif defined(TCSUPPORT_NAND_BADBLOCK_CHECK)		
        set_lzma_addr(flash_base+0x2280000+0x100, flash_base+0x2280000+kernel_size);		
        trx_slave = (struct trx_header*)(flash_base+0x2280000);		
        trx_addr = &(trx_slave->header_len);		
        header_len = READ_FLASH_DWORD(trx_addr);		
        trx_addr = &(trx_slave->kernel_len);		
        kernel_len = READ_FLASH_DWORD(trx_addr);		
        trx_addr = &(trx_slave->rootfs_len);		
        rootfs_len = READ_FLASH_DWORD(trx_addr);		
        binlen = header_len + kernel_len + rootfs_len;        
        flash_erase(0x280000, binlen);        
        flash_read(flash_base+0x2280000, binlen, &retlen, 0x80020000);        
        flash_write(0x2280000, binlen, &retlen, 0x80020000);
		
#else
//		set_lzma_addr(0xb0020100+0x500000, 0xb0120000+0x500000);
	//	set_lzma_addr(flash_base + 0x20100 + 0x500000, flash_base + 0x120000 + 0x500000);
		set_lzma_addr(flash_base + flash_tclinux_start+trx_header_len + TCLINUX_SLAVE_FLASH_START, flash_base + flash_tclinux_start+kernel_size+ TCLINUX_SLAVE_FLASH_START);
#endif
	} else {
		prom_printf("from main\n");
//		set_lzma_addr(0xb0020100, 0xb0120000);
	//	set_lzma_addr(flash_base + 0x20100, flash_base + 0x120000);
	#ifdef 	TCSUPPORT_NAND_BADBLOCK_CHECK
        set_lzma_addr( (flash_base + 0x280000 + 0x100), (flash_base + 0x280000 + kernel_size) );
    #elif defined(TCSUPPORT_NAND_RT63368)
        set_lzma_addr( (flash_base + NAND_KERNEL_OFFSET + 0x100), (flash_base + NAND_KERNEL_OFFSET + kernel_size) );
	#else
		set_lzma_addr(flash_base + flash_tclinux_start+trx_header_len, flash_base + flash_tclinux_start + kernel_size);//xflu
	#endif
#ifdef TCSUPPORT_DUAL_IMAGE_ENHANCE
		//copy main to slave
//		trx_main = (struct trx_header *)(0xb0000000+0x20000);
    #ifdef TCSUPPORT_NAND_RT63368
        trx_main = (struct trx_header*)(flash_base+NAND_KERNEL_OFFSET);
    #else
		trx_main = (struct trx_header *)(flash_base+flash_tclinux_start);
	#endif	
		trx_addr = &(trx_main->header_len);
		header_len = READ_FLASH_DWORD(trx_addr);
		trx_addr = &(trx_main->kernel_len);
		kernel_len = READ_FLASH_DWORD(trx_addr);
		trx_addr = &(trx_main->rootfs_len);
		rootfs_len = READ_FLASH_DWORD(trx_addr);
		binlen = header_len + kernel_len + rootfs_len;

		trx_slave = (struct trx_header *)(flash_base+flash_tclinux_start+offset);
		trx_addr = &(trx_slave->version);
		for (index=0; index<32;  index++, trx_addr++){
			slave_ver[index] = READ_FLASH_BYTE(trx_addr);
		}
		trx_addr = &(trx_main->version);
		for (index=0; index<32;  index++, trx_addr++){
			main_ver[index] = READ_FLASH_BYTE(trx_addr);
		}

//		trx_slave = (struct trx_header *)(0xb0000000+0x20000+offset);
//		if((checkimage(SLAVE_IMAGE) < 0) || (strcmp(trx_main->version,trx_slave->version) != 0)) {
		if((checkimage(SLAVE_IMAGE) < 0) || (strcmp(main_ver, slave_ver) != 0)) {
			checkflag = 1;
		}
		if(checkflag == 0){
			if (IS_NANDFLASH)
			  #ifdef TCSUPPORT_NAND_RT63368
                flash_read(flash_base+NAND_KERNEL_OFFSET, binlen, &retlen, 0x80020000);
			  #else
				flash_read(flash_base+flash_tclinux_start, binlen, &retlen, 0x80020000);
			  #endif	
			else
				memcpy(0x80020000, flash_base+flash_tclinux_start, binlen);
			checkSum_main = CheckSum(0x80020000,binlen);
			if (IS_NANDFLASH)
				flash_read(flash_base+flash_tclinux_start+offset, binlen, &retlen, 0x80020000);
			else
				memcpy(0x80020000, flash_base+flash_tclinux_start+offset, binlen);
			checkSum_slave = CheckSum(0x80020000,binlen);
			if(checkSum_main != checkSum_slave){
				checkflag = 1;
			}
#if 0
			memcpy(0x80020000, 0xb0000000+0x20000, binlen);
			checkSum_main = CheckSum(0x80020000,binlen);
			memcpy(0x80020000, 0xb0000000+0x20000+offset, binlen);
			checkSum_slave = CheckSum(0x80020000,binlen);
			if(checkSum_main != checkSum_slave)
				checkflag = 1;
#endif				
		}

		if(checkflag == 1){
			flash_erase(flash_tclinux_start+offset, binlen);
			if (IS_NANDFLASH)
			  #ifdef TCSUPPORT_NAND_RT63368
                flash_read(flash_base+NAND_KERNEL_OFFSET, binlen, &retlen, 0x80020000);
			  #else
				flash_read(flash_base+flash_tclinux_start, binlen, &retlen, 0x80020000);
			  #endif	
			else
				memcpy(0x80020000, flash_base+flash_tclinux_start, binlen);
			flash_write(flash_tclinux_start+offset, binlen, &retlen, 0x80020000);
		}
#if 0
		if(checkflag == 1){
			flash_erase(0x20000+offset, binlen);
			memcpy(0x80020000, 0xb0000000+0x20000, binlen);
			flash_write(0x20000+offset, binlen, &retlen, 0x80020000);
		}
#endif

#elif defined(TCSUPPORT_NAND_BADBLOCK_CHECK)
        trx_main = (struct trx_header*)(flash_base+0x280000);        
        trx_addr = &(trx_main->header_len);
        header_len = READ_FLASH_DWORD(trx_addr);	
        trx_addr = &(trx_main->kernel_len);
        kernel_len = READ_FLASH_DWORD(trx_addr);	
        trx_addr = &(trx_main->rootfs_len);
        rootfs_len = READ_FLASH_DWORD(trx_addr);	
        binlen = header_len + kernel_len + rootfs_len;
        
        trx_slave = (struct trx_header *)(flash_base+0x2280000);	
        trx_addr = &(trx_slave->version);

		for (index=0; index<32;  index++, trx_addr++){	
		    slave_ver[index] = READ_FLASH_BYTE(trx_addr);		
		}
        trx_addr = &(trx_main->version);

        for (index=0; index<32;  index++, trx_addr++){
            main_ver[index] = READ_FLASH_BYTE(trx_addr);		
        }

        if((checkimage(SLAVE_IMAGE) < 0) || (strcmp(main_ver, slave_ver) != 0)) {
            checkflag = 1;		
        }

		if(checkflag == 0){			
    		flash_read(flash_base+0x280000, binlen, &retlen, 0x80020000);			
    		checkSum_main = CheckSum(0x80020000,binlen);				
    		flash_read(flash_base+0x2280000, binlen, &retlen, 0x80020000);
    		checkSum_slave = CheckSum(0x80020000,binlen);		
            if(checkSum_main != checkSum_slave){		
                checkflag = 1;			
            }			
        }

        if(checkflag == 1){		
            flash_erase(0x2280000, binlen);		
            flash_read(flash_base+0x280000, binlen, &retlen, 0x80020000);			
            flash_write(0x2280000, binlen, &retlen, 0x80020000);	
        }
        
#endif
	}
#else
//	set_lzma_addr(KERNEL_FLASH_START, KERNEL_FLASH_END);
    #ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
        set_lzma_addr( (flash_base + 0x280000 + 0x100), (flash_base + 0x280000 + 0x180000) );
    #elif defined(TCSUPPORT_NAND_RT63368)
        set_lzma_addr( (flash_base + NAND_KERNEL_OFFSET + 0x100), (flash_base + NAND_KERNEL_OFFSET + 0x1a0000) );
    #else
		set_lzma_addr( (flash_base + flash_tclinux_start+trx_header_len), (flash_base + flash_tclinux_start + kernel_size) );
	#endif      
#endif

	decompress_kernel(output_data, free_mem_ptr, free_mem_ptr_end);
/*Unify the kernel address to jump for all chip, shnwind*/
//#ifdef TC3262
//	jumpAddr = (void (*)(void))0x80020000;
//#else
	/* FIXME */
//	if (isTC3162L3P3 || isTC3162L4P4 || isTC3162L5P5) {
		//jumpAddr = (void (*)(void))KERNEL_START_ADDR;
		if(startAddr != 0)
			jumpAddr = (void (*)(void))(startAddr);
		else
			jumpAddr = (void (*)(void))(0x80020000);

//	} else {
		//jumpAddr = (void (*)(void))0x80020404;
		//jumpAddr = (void (*)(void))0x80020420;
		//jumpAddr = (void (*)(void))0x800203fc;
//		jumpAddr = (void (*)(void))0x800205bc;
//	}
//#endif
#ifdef TC3262
	if (IS_NANDFLASH )
		flush_icache_range(startAddr, startAddr + kernel_size*4);
#endif
	(*jumpAddr)();

	return 0;
}
#endif

int do_memrl(int argc, char *argv[])
{
	unsigned long addr;

	addr = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	printf("%X: %X \n", addr, tc_inl(addr));

	return 0;
}

int do_memwl(int argc, char *argv[])
{
	unsigned long addr;
	unsigned long value;

	addr = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	value = strtoul((const char*)(argv[2]), (char **)NULL, 16);
	tc_outl(addr, value);
	printf("%X: %X \n", addr, tc_inl(addr));

	return 0;
}

#define putchar(x)	serial_outc(x)

int do_dump(int argc, char *argv[])
{
	unsigned long addr;
	unsigned long len;
	register int n, m, c, r;
	unsigned char temp[16];

	addr = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	len = strtoul((const char*)(argv[2]), (char **)NULL, 16);

	for( n = len; n > 0; ){
			printf("%.8x", addr);
			putchar(' ');
			r = n < 16? n: 16;
			memcpy((void *) temp, (void *) addr, r);
			addr += r;
			for( m = 0; m < r; ++m ){
					putchar((m & 3) == 0 && m > 0? '.': ' ');
					printf("%.2x", temp[m]);
			}
			for(; m < 16; ++m )
					printf("   ");
			printf("  |");
			for( m = 0; m < r; ++m ){
				c = temp[m];
				putchar(' ' <= c && c <= '~'? c: '.');
			}
			n -= r;
			for(; m < 16; ++m )
					putchar(' ');
			printf("|\n");
	}

	return 0;
}

int do_jump(int argc, char *argv[])
{
    void (*jumpAddr)();
	unsigned long addr;
	int i;

	addr = strtoul((const char*)(argv[1]), (char **)NULL, 16);

	printf("Jump to %X\n", addr);

#ifndef BOOTROM_IN_SRAM
	if (tc_inl(CR_INTC_IMR) != 0x0)
		macResetSwMAC();
#endif
	tc_outl(CR_INTC_IMR, 0x0);
	tc_outl(CR_TIMER_CTL, 0x0);

	jumpAddr = (void (*)(void))addr;
	(*jumpAddr)();

	return 0;
}

int do_flash(int argc, char *argv[])
{
	unsigned long dst;
	unsigned long src;
	unsigned long len;
	unsigned long retlen;

	dst = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	src = strtoul((const char*)(argv[2]), (char **)NULL, 16);
	len = strtoul((const char*)(argv[3]), (char **)NULL, 16);
#ifdef TCSUPPORT_BB_NAND
#if defined(TCSUPPORT_CPU_MT7510)||defined(TCSUPPORT_CPU_MT7520) || defined(TCSUPPORT_CPU_EN7512) || defined(TCSUPPORT_CPU_EN7521)
	if(argc == 5){
		en_oob_write = strtoul((const char*)(argv[4]), (char **)NULL, 16);
		/*en_oob_write =1, read oob free buf from image and write to oob*/
	}
#endif	
#endif
	printf("Write to flash from %X to %X with %X bytes\n", src, dst, len);

	flash_erase(dst, len);
	printf("\n");
	flash_write(dst, len, &retlen, (const unsigned char *) (src));

	return 0;
}

extern int flash_read(unsigned long from,
	unsigned long len, unsigned long *retlen, unsigned char *buf);
int do_imginfo(int argc, char *argv[])
{
	int os_size = 0;
	char os_version[32];
	int ret_len = 0;
	struct trx_header trx_header_buff;
	unsigned long mainStart = 0;
	unsigned long slaveStart = 0;
	
	#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
    	mainStart = 0x280000;
	#elif defined(TCSUPPORT_NAND_RT63368)
    	mainStart = NAND_KERNEL_OFFSET;
	#else
	mainStart = TCLINUX_FLASH_START;
	#endif

	memset(&trx_header_buff,0,sizeof(struct trx_header));
	flash_read(mainStart,sizeof(struct trx_header),&ret_len,(char*)(&trx_header_buff));

	memset(os_version,0,32);
	memmove(os_version,trx_header_buff.version,32);
	os_size = trx_header_buff.len;
	printf("os1:%s",os_version);

	#ifdef TCSUPPORT_DUAL_IMAGE_ENHANCE
		#ifdef TCSUPPORT_NAND_RT63368
        		slaveStart = mainStart + offset - 0x20000;
  		#else
		slaveStart = mainStart+offset;
  		#endif		
	#elif defined(TCSUPPORT_NAND_BADBLOCK_CHECK)
	slaveStart = mainStart + 0x2280000;
	#else
	slaveStart = mainStart+TCLINUX_SLAVE_FLASH_START;
	#endif

	memset(&trx_header_buff,0,sizeof(struct trx_header));
	flash_read(slaveStart,sizeof(struct trx_header),&ret_len,(char*)(&trx_header_buff));

	memset(os_version,0,32);
	memmove(os_version,trx_header_buff.version,32);
	os_size = trx_header_buff.len;
	printf("os2:%s",os_version);

	return 0;
}
#ifdef MT7505_GDMP
int doSysGdmpMem(int argc, char *argv[])
{
	unsigned long length;
	unsigned char buf[18];
	int i,j = 0;

	unsigned long *ptr;
		
	ptr = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	length = strtoul((const char*)(argv[2]), (char **)NULL, 16);
	if ((length % 8) != 0)
	{
		printf("the length must be n times 8!!!\n");
		return 0;
	}	
	
	
	for( i = 0; i < length/4; i = i + 2)
	{
		printf("%.8x", (unsigned long) (*(ptr + i + 1)));
		printf("%.8x", (unsigned long) (*(ptr + i)));
		printf( "\r\n");
	
		
	}   

	return 0;
}


int doGDmpCfg(int argc, char *argv[])
{
#define GDMP_REG_BASE	0xBFBF0000
#define GDMP_GLO_CFG	(GDMP_REG_BASE + 0x00009000)
#define GDMP_CAP_RLT	(GDMP_REG_BASE + 0x00009004)
#define GDMP_TRG_RLT	(GDMP_REG_BASE + 0x00009008)
#define GDMP_INT_STS	(GDMP_REG_BASE + 0x00009010)
#define GDMP_INT_MSK	(GDMP_REG_BASE + 0x00009014)
#define GDMP_PRB_SEL	(GDMP_REG_BASE + 0x00009020)
#define	GDMP_TRG_PATN0_L		(GDMP_REG_BASE + 0x00009030)
#define	GDMP_TRG_PATN0_MSK_L	(GDMP_REG_BASE + 0x00009034)
#define	GDMP_TRG_PATN0_H		(GDMP_REG_BASE + 0x00009038)
#define	GDMP_TRG_PATN0_MSK_H	(GDMP_REG_BASE + 0x0000903C)
#define	GDMP_TRG_PATN1_L		(GDMP_REG_BASE + 0x00009040)
#define	GDMP_TRG_PATN1_MSK_L	(GDMP_REG_BASE + 0x00009044)
#define	GDMP_TRG_PATN1_H		(GDMP_REG_BASE + 0x00009048)
#define	GDMP_TRG_PATN1_MSK_H	(GDMP_REG_BASE + 0x0000904C)
#define	GDMP_TRG_MODE			(GDMP_REG_BASE + 0x00009050)
#define	GDMP_CAP_CFG			(GDMP_REG_BASE + 0x00009060)
#define	GDMP_CLK_CFG			(GDMP_REG_BASE + 0x00009070)


	
	printf("GDMP_GLO_CFG\t%08lx\r\n",VPint(GDMP_GLO_CFG));	
	
	printf("GDMP_CAP_RLT\t%08lx\r\n",VPint(GDMP_CAP_RLT));
	
	printf("GDMP_TRG_RLT\t%08lx\r\n",VPint(GDMP_TRG_RLT));	
	
	printf("GDMP_INT_STS\t%08lx\r\n",VPint(GDMP_INT_STS));
	
	printf("GDMP_INT_MSK\t%08lx\r\n",VPint(GDMP_INT_MSK));	
	
	printf("GDMP_PRB_SEL\t%08lx\r\n",VPint(GDMP_PRB_SEL));

	printf("GDMP_TRG_PATN0_L\t%08lx\r\n",VPint(GDMP_TRG_PATN0_L));	
	
	printf("GDMP_TRG_PATN0_MSK_L\t%08lx\r\n",VPint(GDMP_TRG_PATN0_MSK_L));

	printf("GDMP_TRG_PATN0_H\t%08lx\r\n",VPint(GDMP_TRG_PATN0_H));	
	
	printf("GDMP_TRG_PATN0_MSK_H\t%08lx\r\n",VPint(GDMP_TRG_PATN0_MSK_H));
	
	printf("GDMP_TRG_PATN1_L\t%08lx\r\n",VPint(GDMP_TRG_PATN1_L));	
	
	printf("GDMP_TRG_PATN1_MSK_L\t%08lx\r\n",VPint(GDMP_TRG_PATN1_MSK_L));
	
	printf("GDMP_TRG_PATN1_H\t%08lx\r\n",VPint(GDMP_TRG_PATN1_H));	
	
	printf("GDMP_TRG_PATN1_MSK_H\t%08lx\r\n",VPint(GDMP_TRG_PATN1_MSK_H));
	
	printf("GDMP_TRG_MODE\t%08lx\r\n",VPint(GDMP_TRG_MODE));	
	
	printf("GDMP_CAP_CFG\t%08lx\r\n",VPint(GDMP_CAP_CFG));

	printf("GDMP_CLK_CFG\t%08lx\r\n",VPint(GDMP_CLK_CFG));
	return 0;
}
#endif


#if defined(TCSUPPORT_NEW_SPIFLASH_DEBUG)

#define SPI_FREQUENCY_ADJUST_REG			0xBFA200CC

static void set_spi_clock_speed( u32 clock_factor)
{
	
	unsigned long val;
	
	val  = VPint(SPI_FREQUENCY_ADJUST_REG);
	val &= 0xffff0000;	
	val |= (((clock_factor) << 8)|1);	
	pause(10);
	val &= 0xfffffffe;	
	pause(10);
	val |= 0x1;

	VPint(SPI_FREQUENCY_ADJUST_REG) = val;

	printf("Set SPI  Clock to %d Mhz\n", (250/clock_factor));
	
}


#define PP_MAX	(256)
#define SE_MAX	(4096)
#define SE_CNT	(16)
#define SEED_MAX (0x100)

#define AUTOREAD_TEST	(0)
#define MANUALREAD_TEST	(1)
#define MUXREAD_TEST	(2)

#define SIZE_32MiB	0x2000000
#define SIZE_64KiB	0x10000

#define SFC_CLOCK_MAX 150

#define TP_MAX (10)
uint8 TestPattern[TP_MAX] =
{
    {0xA5},
    {0x5A},
    {0xFF},
    {0x00},
    {0x55},
    {0xAA},
    {0x96},
    {0x69},
    {0xCC},
    {0x33}
};

uint8 TestMode_TEXT[3][12] =
{
	{"AutoMode"},
	{"ManualMode"},
	{"MuxMode"},
};

uint8 ReadMode_TEXT[6][20] =
{
	{"ReadData"},
	{"FastRead"},
	{"FastRead_DualOut"},
	{"FastRead_DualIO"},
	{"FastRead_QuadOut"},
	{"FastRead_QuadIO"},	
};

uint8 MuxMode_TEXT[8][25] =
{
	{"Auto_ReadData"},
	{"Auto_FastRead"},
	{"Auto_FastRead_DualOut"},
	{"Auto_FastRead_DualIO"},
	{"Manual_ReadData"},
	{"Manual_FastRead"},
	{"Manual_FastRead_DualOut"},
	{"Manual_FastRead_DualIO"},
};

uint8 clk_rate=0;
uint8 CLKRate_TEXT[3][12] =
{
	{"25MHZ"},
	{"50MHZ"},
	{"Changing"},
};

uint8 Address3B4B_TEXT[3][40] =
{
	{"4Bytes access all 32MB flash"},
	{"3Bytes access front 16MB flash"},
	{"3Bytes access, and use 13/0C/3C/BC"},
};

uint32 Round_Value = 0;
uint16 AutoRead_Value = 0;
uint16 ManualRead_Value = 0;
uint16 MuxRead_Value = 0;
uint16 isProgram_Value = 0;
uint16 CLKRate_Value = 0;
uint16 Exit_Value = 0;
uint16 Address3B4B_Value = 0;
uint32 TestPattern_Value = 0;
uint32 StartAddress_Value = 0;
uint16 ReadIdle_Value = 0;
uint32 Mats_Round_Value = 0;
uint32 Mats_Scope_Value = 0;
uint32 Mats_isChipTest_Value = 0;
uint32 Mats_ReadMode_Value = 0;
uint32 Mats_Address_Value = 0;
uint16 Tn_Value = 0;
uint16 Tn_cnt = 0;
uint8 TnMappingTab[6];

uint16 AutoRead_cnt = 0;
uint16 ManualRead_cnt = 0;
uint16 MuxRead_cnt = 0;
uint16 AutoMappingTab[6];
uint16 ManualMappingTab[4];
uint16 MuxMappingTab[8];
uint32 rd_time = 0;
uint32 pp_time = 0;
uint32 se_time = 0;
uint32 be_time = 0;

extern void enter_4Byte_mode(void);
extern void exit_4Byte_mode(void);

static void SF_Test_Helper(){
#if 1
	printf("\nusage: \tsftest [Round] [AutoRD:0~15] [ManualRD:0~15] [MuxRD:0~1] [isProg:0~1] [CLK Rate:0~31] [Exit:0~1] [3B4B:0~2] [Pattern] [StartAddr] [RDIdle] [Tn]\n");
#else
	printf("\nusage: \tsftest [Round] [AutoRD:0~15] [ManualRD:0~15] [MuxRD:0~1] [isProg:0~1] [CLK Rate:0~2] [Exit:0~1] [3B4B:0~2] [Pattern] [StartAddr] [RDIdle] [Tn]\n");
#endif
	printf("Round: \t\t0~65535 means test round.   DEFAULT VALUE is 10\n");
	printf("AutoRD: \t0~63 is b'00000000~00111111, bit5 means Auto QuadIO, bit4 means Auto QuadOutput, bit3 means Auto_singleRead, bit2 means Auto_FastRead, bit1 means Auto_FastDualOutput, \n\t\tbit0 means Auto_FastDualIO.   DEFAULT VALUE is 8\n");
	printf("ManualRD: \t0~15 is b'0000~1111, bit3 means Manual_singleRead, bit2 means Manual_FastRead, bit1 means Manual_FastDualOutput, \n\t\tbit0 means Manual_FastDualIO.   DEFAULT VALUE is 0\n");
	printf("MuxRD: \t\t0 means disable MuxRead, 1 means enable MuxRead.   DEFAULT VALUE is 0\n");
	printf("isProg: \t0 means disable Program, 1 means enable Program.   DEFAULT VALUE is 0\n");
#ifdef TCSUPPORT_CPU_MT7505	
#if 1
	printf("CLK Rate: \t0 means 15MHZ, 1 means (150/1)MHZ, 2 means (150/2)MHZ, 3 means (150/3)MHZ, etc...   DEFAULT VALUE is 0\n");
#else
	printf("CLK Rate: \t0 means 25MHZ, 1 means 50MHZ, 2 means CLK Rate change on the fly.   DEFAULT VALUE is 0\n");
#endif
#endif
#if defined(TCSUPPORT_CPU_EN7512)||defined(TCSUPPORT_CPU_EN7521)
#if 1  /* ASIC */
printf("CLK Rate: \t0 means auto switch clock test, 1 ~ 31 means (250/CLK Rate)MHZ, DEFAULT VALUE is 0\n");
#else  /* FPGA */
printf("CLK Rate: \taoto test 25Mhz and 50Mhz\n");
#endif
#endif

	printf("Exit: \t\t0 means continue test while error happens, 1 means exit test while error happens.   DEFAULT VALUE is 0\n");
	printf("3B4B: \t\t0 means 4Bytes access all 32MB, 1 means 3Bytes access front 16MB, 2 means uses 13/0C/3C/BC. DEFAULT VALUE is 0\n");
	printf("TestPattern: \t0:normal test, 1:A5, 2:5A, 3:FF, 4:00, 5:55, 6:AA, 7:96, 8:69, 9:CC, 10:33, 11:Random. DEFAULT VALUE is 0\n");
	printf("StartAddr: \tDEFAULT VALUE is 0xF00000\n");
	printf("RDIdle: \t0: Disable Read IDLE, 1: Enable Read IDLE.   DEFAULT VALUE is 0\n");
	printf("Tn: \t0~15 is b'000000~111111, the value of bit0~bit5 means T0~T5 enable or desable.   DEFAULT VALUE is 0\n");
}

static void SF_Mats_Helper(){
	printf("\nusage: \tsfmats [Round] [Scope:0~1] [isChipTest:0~1] [Exit:0~1] [ReadMode:0~7] [StartAddr]\n");
	printf("Round: \t\t0~65535 means test round.   DEFAULT VALUE is 10\n");
	printf("Scope: \t\t0 means 64KB, 1 means all Flash.   DEFAULT VALUE is 0\n");
	printf("isChipTest: \t0 means ChipTest disable, 1 means ChipTest enable.   DEFAULT VALUE is 0\n");
	printf("Exit: \t\t0 means continue test while error happens, 1 means exit test while error happens.   DEFAULT VALUE is 0\n");
	printf("ReadMode: \t0~3:Auto Mode[Single/Fast/DualOut/DualIO], 4~7:Manual Mode[Single/Fast/DualOut/DualIO]   DEFAULT VALUE is 0\n");
	printf("StartAddr: \tDEFAULT VALUE is 0x700000\n\n");
}

static void dumpCell_flash(u8* src, u32 len){
	u32 i=0;
	u32 src_temp=0;

	src_temp = src;
	src_temp &= 0x03FFFFFF;
	src_temp |= 0xBC000000;
	printf("\r\n src is 0x08x, src_temp is 0x%08x", src, src_temp);
	printf("\r\n\t\t00  01  02  03  04  05  06  07  08  09  0A  0B  0C  0D  0E  0F");
	for(i=0; i<len; i++){
		if((i&15)==0){
			printf("\r\n0x%08X:\t", src_temp+i);
		}
		printf("%02x  ", *(u8*)(src_temp+i));
	}
	printf("\r\n");
}/*end dumpCell_flash*/

static void dumpCell_buffer(u8* buf, u32 len, u32 from){
	u32 i=0;

	from &= 0x03FFFFFF;
	from |= 0xBC000000;
	printf("\r\n\t\t00  01  02  03  04  05  06  07  08  09  0A  0B  0C  0D  0E  0F");
	for(i=0; i<len; i++){
		if((i&15)==0){
			printf("\r\n0x%08X:\t", from+i);
		}
		printf("%02x  ", *(buf+i));
	}
	printf("\r\n");
}/*end dumpCell_buffer*/

unsigned long sf_rand_ascend()
{
	static unsigned long rand_asc=0;
	rand_asc++;
	return rand_asc;
}
unsigned long sf_rand_descend()
{
	static unsigned long rand_des=0;
	rand_des--;
	return rand_des;
}
unsigned long sf_rand()
{
	static unsigned long rand_item=0;
	rand_item = (rand_item * 123 + 59) % 65536;
	return rand_item;
}

int sf_mats_test(u32 round)
{
	int i=0, j=0;
	unsigned long retlen=0;
	unsigned long size=0;
	unsigned long erasesize=0;
	unsigned long cur_se_off=0;
	unsigned long cur_pp_off=0;
	unsigned long start_addr=0;
	unsigned char rd_buf[2]={0};
	unsigned char pp_buf[2]={0};
	
	erasesize = mtd.erasesize;
	size = mtd.size;
	start_addr = Mats_Address_Value;

	pp_buf[0] = 0x00;
	printf("\n\n\n>>>>>>> Round%d Test::current read mode is %s >>>>>>\n", round, MuxMode_TEXT[Mats_ReadMode_Value]);

	if(Mats_isChipTest_Value) {
		printf("\nChip Test-Ascend begin, Start Address is 0x%08x, End Address is 0x%08x!\n", start_addr, size);
		for(cur_se_off = start_addr; (cur_se_off + erasesize) <= size; cur_se_off += erasesize) {
			/* erase one block, the size is 64KB */
			flash_erase(cur_se_off, erasesize);
		}
		
		for(cur_se_off = start_addr; (cur_se_off + erasesize) <= size; cur_se_off += erasesize) {
			/* erase one block, the size is 64KB */
			spiflash_mats_read(cur_se_off, 1, &retlen, rd_buf);
			if(rd_buf[0] != 0xFF) {
				printf("\n####Chip_Ascend: Read fail at Address %X, value is %X, should be FF, reg 0xBFA10000 is 0x%08x\n", cur_se_off, rd_buf[0], SF_READ_MODE_VALUE);
				if(Exit_Value == 1)
					goto read_fail;
				else
					break;
			}
			flash_write(cur_se_off, 1, &retlen, pp_buf);
		}
		
		printf("\nChip Test-Descend begin, Start Address is 0x%08x, End Address is 0x%08x!\n", start_addr, size);
		for(cur_se_off = start_addr; (cur_se_off + erasesize) <= size; cur_se_off += erasesize) {
			/* erase one block, the size is 64KB */
			flash_erase(cur_se_off, erasesize);
		}
		
		for(cur_se_off = size-erasesize ; cur_se_off >= start_addr; cur_se_off -= erasesize) {
			/* erase one block, the size is 64KB */
			spiflash_mats_read(cur_se_off, 1, &retlen, rd_buf);
			if(rd_buf[0] != 0xFF) {
				printf("\n####Chip_Descend: Read fail at Address %X, value is %X, should be FF, reg 0xBFA10000 is 0x%08x\n", cur_se_off, rd_buf[0], SF_READ_MODE_VALUE);
				if(Exit_Value == 1)
					goto read_fail;
				else
					break;
			}
			flash_write(cur_se_off, 1, &retlen, pp_buf);
		}
	}

	if(Mats_Scope_Value == 0)
		size = start_addr + erasesize;
	
	for(cur_se_off = start_addr; (cur_se_off + erasesize) <= size; cur_se_off += erasesize) {
		printf("\n64KB Erase-Ascend begin, Address is 0x%08x!\n", cur_se_off);
		/* erase one block, the size is 64KB */
		flash_erase(cur_se_off, erasesize);
	
		/* check the content of the block */
		for(j=0; j<SE_CNT * SE_MAX; ++j){
			spiflash_mats_read(cur_se_off+j, 1, &retlen, rd_buf);
			if(rd_buf[0] != 0xFF) {
				printf("\n####Block_Ascend: Read fail at Address %X::index is %d, value is %X, should be FF, reg 0xBFA10000 is 0x%08x, clk_rate is %s\n", cur_se_off+j, j, rd_buf[j], SF_READ_MODE_VALUE, CLKRate_TEXT[clk_rate]);
				if(Exit_Value == 1)
					goto read_fail;
				else
					break;
			}
			flash_write(cur_se_off+j, 1, &retlen, pp_buf);
		}

		
		printf("\n64KB Erase-Descend begin, Address is 0x%08x!\n", cur_se_off);
		/* erase one block, the size is 64KB */
		flash_erase(cur_se_off, erasesize);
		
		/* check the content of the block */
		for(j=SE_CNT * SE_MAX-1; j>=0 ; --j){
			spiflash_mats_read(cur_se_off+j, 1, &retlen, rd_buf);
			if(rd_buf[0] != 0xFF) {
				printf("\n####Block_Descend: Read fail at Address %X::index is %d, value is %X, should be %X, reg 0xBFA10000 is 0x%08x, clk_rate is %s\n", cur_se_off+j, j, rd_buf[j], 0xFF, SF_READ_MODE_VALUE, CLKRate_TEXT[clk_rate]);
				if(Exit_Value == 1)
					goto read_fail;
				else
					break;
			}
			flash_write(cur_se_off+j, 1, &retlen, pp_buf);
		}
	}
	
	return 0;
	
read_fail:
	return -1;
}


static unsigned char se_buf[SE_MAX]={0};


int sf_basic_test(u32 round, u32 readtest)
{
	unsigned long retlen=0;
	unsigned long erasesize=0;
	unsigned long size=0;
	unsigned long cur_se_off=0;
	unsigned long cur_pp_off=0;
	unsigned long start_addr=0;
	unsigned char se_buf[SE_MAX]={0};
	unsigned char pp_buf[PP_MAX]={0};
	unsigned char seed=0;
	int cur_round=0;
	int i=0, j=0, k=0;
	int index=0;
	u32 read_mode=0;
	u32 begtime=0, endtime=0, passtime=0;
#if 0
	u32 rd_begtime=0, rd_endtime=0, rd_passtime=0, rd_total_time=0;
	u32 rd_begtick=0, rd_endtick=0;
#else
	u32 rd_total_time=0;
	u32 pp_total_time=0;
	u32 rd1_average_time=0;
	u32 rd2_average_time=0;
	u32 se_average_time=0;
	u32 pp_average_time=0;
	u32 count_64KB=0;
#endif

	erasesize = mtd.erasesize;
	size = mtd.size;
	start_addr = StartAddress_Value;

	count_64KB = (size - start_addr) / SIZE_64KiB;

	cur_round = round % (TP_MAX+1);
	//read_mode = round % 5;
	if(readtest == AUTOREAD_TEST)
		printf("\n\n\n>>>>>>> Round%d Test::current test is AUTOREAD_TEST >>>>>>\n", round);
	else if(readtest == MANUALREAD_TEST)
		printf("\n\n\n>>>>>>> Round%d Test::current test is MANUALREAD_TEST >>>>>>\n", round);
	else if(readtest == MUXREAD_TEST)
		printf("\n\n\n>>>>>>> Round%d Test::current test is MUXREAD_TEST >>>>>>\n", round);
	/* check flash ID */		
	index = spiflash_probe();
	if (index < 0) {
    	printf ("\n########spiflash ID read error\n");
   	}
	
	begtime = getTime();
	for(cur_se_off = start_addr; (cur_se_off + erasesize) <= size; cur_se_off += erasesize) {
		/* erase one block, the size is 64KB */
		flash_erase(cur_se_off, erasesize);
		printf("\nerase addr=%x size=%x passtime is %lu ", cur_se_off, erasesize, se_time);
		se_average_time += se_time / count_64KB;

		rd_total_time = 0;
		/* check the content of the block */

		for(j=0; j<SE_CNT; j++){
			if(readtest == AUTOREAD_TEST)
				spiflash_autoread_test(cur_se_off+j*SE_MAX, SE_MAX, &retlen, se_buf, (++read_mode));
			else if(readtest == MANUALREAD_TEST)
				spiflash_manualread_test(cur_se_off+j*SE_MAX, SE_MAX, &retlen, se_buf, (++read_mode));
			else if(readtest == MUXREAD_TEST)
				spiflash_muxread_test(cur_se_off+j*SE_MAX, SE_MAX, &retlen, se_buf, (++read_mode));
			rd_total_time += rd_time;
			
			for(i=0; i<SE_MAX; i++) {
				if(se_buf[i] != 0xFF) {
					if(readtest == MUXREAD_TEST)
						printf("\n####Erase fail at Address %X::index is %d, value is %X, should be %X, ReadMode is %s::%s, reg 0xBFA10000 is 0x%08x, clk_rate is %s\n", cur_se_off+j*SE_MAX+i, i, se_buf[i], 0xFF, TestMode_TEXT[readtest], MuxMode_TEXT[retlen & 0x7], SF_READ_MODE_VALUE, CLKRate_TEXT[clk_rate]);
					else
						printf("\n####Erase fail at Address %X::index is %d, value is %X, should be %X, ReadMode is %s::%s, reg 0xBFA10000 is 0x%08x, clk_rate is %s\n", cur_se_off+j*SE_MAX+i, i, se_buf[i], 0xFF, TestMode_TEXT[readtest], ReadMode_TEXT[retlen & 0x3], SF_READ_MODE_VALUE, CLKRate_TEXT[clk_rate]);
					dumpCell_buffer((u8 *)(se_buf), SE_MAX, cur_se_off+j*SE_MAX);
					if(Exit_Value == 1)
						goto erase_fail;
					else
						break;
				}
			}
		}
		printf("\ne-read at Address %x, passtime is %lu ", cur_se_off, rd_total_time);
		rd1_average_time += rd_total_time / count_64KB;

		if(isProgram_Value == 1) {
			rd_total_time = 0;
			pp_total_time = 0;
			for(cur_pp_off = cur_se_off; (cur_pp_off + PP_MAX) <= (cur_se_off + erasesize); cur_pp_off += PP_MAX) {
				/* program 256Bytes of current blcok */
				if(TestPattern_Value == 0) {
					if(cur_round == TP_MAX) {
						seed = sf_rand() % SEED_MAX;
						for(i=0; i<PP_MAX; i++) {
							pp_buf[i] = (seed+i) % SEED_MAX;
						}
					} else {
						for(i=0; i<PP_MAX; i++) {
							pp_buf[i] = TestPattern[cur_round];
						}
					}
				} else if(TestPattern_Value == 11) {
					seed = sf_rand() % SEED_MAX;
					for(i=0; i<PP_MAX; i++) {
						pp_buf[i] = (seed+i) % SEED_MAX;
					}
				} else {
					for(i=0; i<PP_MAX; i++) {
						pp_buf[i] = TestPattern[TestPattern_Value-1];
					}
				}
				flash_write(cur_pp_off, PP_MAX, &retlen, pp_buf);
				pp_total_time += pp_time;
				//printf("\n Program flash from %X to %X", cur_pp_off, cur_pp_off + PP_MAX);
				
				/* check the content of current 256 Bytes */
				//spiflash_read_test(cur_pp_off, PP_MAX, &retlen, pp_buf, (read_mode++)%2);
				if(readtest == AUTOREAD_TEST)
					spiflash_autoread_test(cur_pp_off, PP_MAX, &retlen, pp_buf, (read_mode++));
				else if(readtest == MANUALREAD_TEST)
					spiflash_manualread_test(cur_pp_off, PP_MAX, &retlen, pp_buf, (read_mode++));
				else if(readtest == MUXREAD_TEST)
					spiflash_muxread_test(cur_pp_off, PP_MAX, &retlen, pp_buf, (read_mode++));
				rd_total_time += rd_time;
				for(i=0; i<PP_MAX; i++) {
					if( pp_buf[i] != ((TestPattern_Value==0)?((cur_round == TP_MAX)?((seed+i) % SEED_MAX):(TestPattern[cur_round])):((TestPattern_Value==11)?((seed+i) % SEED_MAX):(TestPattern[TestPattern_Value-1]))) ) {
						if(readtest == MUXREAD_TEST)
							printf("\n####Program fail at Address %X::index is %d, value is %X, should be %X, ReadMode is %s::%s, reg 0xBFA10000 is 0x%08x, clk_rate is %s\n", cur_pp_off+i, i, pp_buf[i], ((TestPattern_Value==0)?((cur_round == TP_MAX)?((seed+i) % SEED_MAX):(TestPattern[cur_round])):((TestPattern_Value==11)?((seed+i) % SEED_MAX):(TestPattern[TestPattern_Value-1]))), TestMode_TEXT[readtest], MuxMode_TEXT[retlen & 0x7], SF_READ_MODE_VALUE, CLKRate_TEXT[clk_rate]);
						else
							printf("\n####Program fail at Address %X::index is %d, value is %X, should be %X, ReadMode is %s::%s, reg 0xBFA10000 is 0x%08x, clk_rate is %s\n", cur_pp_off+i, i, pp_buf[i], ((TestPattern_Value==0)?((cur_round == TP_MAX)?((seed+i) % SEED_MAX):(TestPattern[cur_round])):((TestPattern_Value==11)?((seed+i) % SEED_MAX):(TestPattern[TestPattern_Value-1]))), TestMode_TEXT[readtest], ReadMode_TEXT[retlen & 0x3], SF_READ_MODE_VALUE, CLKRate_TEXT[clk_rate]);
						printf("program average time is %lu, current time is %lu\n", (pp_total_time)/((cur_pp_off - cur_se_off)>>8+1), pp_time);
						dumpCell_buffer((u8 *)(pp_buf), PP_MAX, cur_pp_off);
						if(Exit_Value == 1)
							goto program_fail;
						else
							break;
					} 
				}
			}
			printf("\nprogram at Address %x, passtime is %lu ", cur_se_off, pp_total_time);
			printf("\np-read at Address %x, passtime is %lu \n", cur_se_off, rd_total_time);
			pp_average_time += pp_total_time / count_64KB;
			rd2_average_time += rd_total_time / count_64KB;	
		}
	}
	endtime = getTime();
	if(endtime < begtime)
		passtime = 0xffffffff - begtime + endtime;
	else
		passtime = endtime - begtime;
	printf("\n\nRD1: %lu \nRD2: %lu \nSE: %lu \nPP: %lu ", rd1_average_time, rd2_average_time, se_average_time, pp_average_time);
	printf("\n>>>>>>> Round%d Test Finish::passtime is %lums", round, passtime);
	printf("\nCurrent Time is %d:%d:%d\n", endtime/3600000, (endtime/60000)%60, (endtime/1000)%60);

	return 0;
	
erase_fail:
	return -1;
	
program_fail:
	return -2;
}

int sf_manual_test(u32 round)
{
	return sf_basic_test(round, MANUALREAD_TEST);
}

int sf_auto_test(u32 round)
{
	return sf_basic_test(round, AUTOREAD_TEST);
}

int sf_mux_test(u32 round)
{
	return sf_basic_test(round, MUXREAD_TEST);
}

int do_sf_basic_test(int argc, char *argv[])
{
	int i=0;
	u32 round = 0;
	u32 pattern = 0;
	int result = 0;
	
	clk_rate = 0;
	Round_Value = 10;
	AutoRead_Value = 8;
	ManualRead_Value = 0;
	MuxRead_Value = 0;
	isProgram_Value = 0;
	CLKRate_Value = 0;
	Exit_Value = 0;
	Address3B4B_Value = 0;
	TestPattern_Value = 0;
	StartAddress_Value = 0xF00000;
	ReadIdle_Value = 0;
	Tn_Value = 0;
	Tn_cnt = 0;
	TnMappingTab[0] = 0;

	AutoRead_cnt = 0;
	ManualRead_cnt = 0;
	MuxRead_cnt = 0;
	
	AutoRead_cnt = 1;
	AutoMappingTab[0] = 0;
		
	if(argc == 1){
		SF_Test_Helper();
		return -1;
	}
	
	if(argc >= 2){
		Round_Value = simple_strtoul(argv[1], NULL, 10);
		printf("Round is %lu\n", Round_Value);
	}
	if(argc >= 3){
		AutoRead_Value = simple_strtoul(argv[2], NULL, 10);
		if(AutoRead_Value > 64) {
			printf("\nError: AutoRead Value must be [0~64]!\n");
			SF_Test_Helper();
			return -3;
		}
		else {
			AutoRead_cnt = 0;
			MuxRead_cnt = 0;
			printf("AutoRead Value is %u, ", AutoRead_Value);
			if(AutoRead_Value == 0)
				printf("no Auto Read Test\n");
			else {
				printf("including ");
				if(AutoRead_Value & 0x8) {
					printf("Single Read, ");
					AutoMappingTab[AutoRead_cnt++] = 0;
					MuxMappingTab[MuxRead_cnt++] = 0;
				}
				if(AutoRead_Value & 0x4) {
					printf("Fast Read, ");
					AutoMappingTab[AutoRead_cnt++] = 1;
					MuxMappingTab[MuxRead_cnt++] = 1;
				}
				if(AutoRead_Value & 0x2) {
					printf("Fast Read Dual Output, ");
					AutoMappingTab[AutoRead_cnt++] = 2;
					MuxMappingTab[MuxRead_cnt++] = 2;
				}
				if(AutoRead_Value & 0x1) {
					printf("Fast Read Dual IO, ");
					AutoMappingTab[AutoRead_cnt++] = 3;
					MuxMappingTab[MuxRead_cnt++] = 3;
				}
				if(AutoRead_Value & 0x10) {
					printf("Fast Read Quad Output, ");
					AutoMappingTab[AutoRead_cnt++] = 4;
				}
				if(AutoRead_Value & 0x20) {
					printf("Fast Read Quad IO, ");
					AutoMappingTab[AutoRead_cnt++] = 5;
				}				
				
				printf("\n");
			}
		}
	}
	if(argc >= 4){
		ManualRead_Value = simple_strtoul(argv[3], NULL, 10);
		if(ManualRead_Value > 15) {
			printf("\nError: ManualRead Value must be [0~15]!\n");
			SF_Test_Helper();
			return -4;
		}
		else {
			ManualRead_cnt = 0;
			printf("ManualRead Value is %u, ", ManualRead_Value);
			if(ManualRead_Value == 0)
				printf("no Manual Read Test\n");
			else {
				printf("including ");
				if(ManualRead_Value & 0x8) {
					printf("Single Read, ");
					ManualMappingTab[ManualRead_cnt++] = 0;
					MuxMappingTab[MuxRead_cnt++] = 4;
				}
				if(ManualRead_Value & 0x4) {
					printf("Fast Read, ");
					ManualMappingTab[ManualRead_cnt++] = 1;
					MuxMappingTab[MuxRead_cnt++] = 5;
				}
				if(ManualRead_Value & 0x2) {
					printf("Fast Read Dual Output, ");
					ManualMappingTab[ManualRead_cnt++] = 2;
					MuxMappingTab[MuxRead_cnt++] = 6;
				}
				if(ManualRead_Value & 0x1) {
					printf("Fast Read Dual IO, ");
					ManualMappingTab[ManualRead_cnt++] = 3;
					MuxMappingTab[MuxRead_cnt++] = 7;
				}
				printf("\n");
			}
		}
	}
	if(argc >= 5){
		MuxRead_Value = simple_strtoul(argv[4], NULL, 10);
		if(MuxRead_Value > 1) {
			printf("\nError: MuxRead Value must be [0~1]!\n");
			SF_Test_Helper();
			return -5;
		}
		else
			printf("MuxRead is %s\n", ((MuxRead_Value==1)?"enable":"disable"));
	}
	if(argc >= 6){
		isProgram_Value = simple_strtoul(argv[5], NULL, 10);
		if(isProgram_Value > 1) {
			printf("\nError: isProgram Value must be [0~1]!\n");
			SF_Test_Helper();
			return -6;
		}
		else
			printf("Program is %s\n", ((isProgram_Value==1)?"enable":"disable"));
	}	
#if 1
	if(argc >= 7){
		CLKRate_Value = simple_strtoul(argv[6], NULL, 10);
		if(CLKRate_Value > 31) {
			printf("\nError: CLK Rate Value must be [0~31]!\n");
			SF_Test_Helper();
			return -7;
		}
		else {
#ifdef TCSUPPORT_CPU_MT7505			
			if(CLKRate_Value == 0)
				CLKRate_Value = 10;
			printf("CLK Rate is %dMHZ\n", (SFC_CLOCK_MAX / CLKRate_Value));
#endif			
		}
	}
#else
	if(argc >= 7){
		CLKRate_Value = simple_strtoul(argv[6], NULL, 10);
		if(CLKRate_Value > 2) {
			printf("\nError: CLK Rate Value must be [0~2]!\n");
			SF_Test_Helper();
			return -7;
		}
		else
			printf("CLK Rate is %s\n", CLKRate_TEXT[CLKRate_Value]);
	}
#endif
	if(argc >= 8){
		Exit_Value = simple_strtoul(argv[7], NULL, 10);
		if(Exit_Value > 1) {
			printf("\nError: Exit Value must be [0~1]!\n");
			SF_Test_Helper();
			return -8;
		}
		else
			printf("Exit while error is %s\n", ((Exit_Value==1)?"enable":"disable"));
	}
	if(argc >= 9){
		Address3B4B_Value = simple_strtoul(argv[8], NULL, 10);
		if(Address3B4B_Value > 2) {
			printf("\nError: 3B4B Value must be [0~2]!\n");
			SF_Test_Helper();
			return -9;
		}
		else
			printf("The way to access 32MB flash is %s\n", Address3B4B_TEXT[Address3B4B_Value]);
	}
	if(argc >= 10){
		TestPattern_Value = simple_strtoul(argv[9], NULL, 10);
		if(TestPattern_Value > 11) {
			printf("\nError: Pattern Value must be [0~11]!\n");
			SF_Test_Helper();
			return -10;
		}
		else {
			if(TestPattern_Value == 0)
				printf("TestPattern is Normal Test\n");
			else if(TestPattern_Value == 11)
				printf("TestPattern is Increment Digital\n");
			else
				printf("TestPattern is %X\n", TestPattern[TestPattern_Value-1]);
		}
	}
	if(argc >= 11){
		StartAddress_Value = simple_strtoul(argv[10], NULL, 16);
		StartAddress_Value &= 0xFFFF0000;
		if(StartAddress_Value < 0x20000) {
			StartAddress_Value = 0x20000;
		}
		printf("Test Start Address 0x%08x\n", StartAddress_Value);
	}
	if(argc >= 12){
		ReadIdle_Value = simple_strtoul(argv[11], NULL, 10);
		if(ReadIdle_Value > 1) {
			printf("\nError: RDIdle Value must be [0~1]!\n");
			SF_Test_Helper();
			return -11;
		}
		else
			printf("Read Idle is %s\n", ((ReadIdle_Value==1)?"enable":"disable"));
	}
	if(argc >= 13){
		Tn_Value = simple_strtoul(argv[12], NULL, 10);
		if(Tn_Value > 0x3F) {
			printf("\nError: Tn Value must be [0~0x3F]!\n");
			SF_Test_Helper();
			return -12;
		}
		else {
			Tn_cnt = 0;
			if(Tn_Value == 0)
				printf("Tn Switching Disable\n");
			else if((Tn_Value & (Tn_Value-1)) == 0) {
				Tn_cnt = 1;
				printf("Tn Switching unnecessary\n");
			} else {
				if(Tn_Value & 0x1) {
					printf("T0 ");
					TnMappingTab[Tn_cnt++] = 0;
				}
				if(Tn_Value & 0x2) {
					printf("T1 ");
					TnMappingTab[Tn_cnt++] = 1;
				}
				if(Tn_Value & 0x4) {
					printf("T2 ");
					TnMappingTab[Tn_cnt++] = 2;
				}
				if(Tn_Value & 0x8) {
					printf("T3 ");
					TnMappingTab[Tn_cnt++] = 3;
				}
				if(Tn_Value & 0x10) {
					printf("T4 ");
					TnMappingTab[Tn_cnt++] = 4;
				}
				if(Tn_Value & 0x20) {
					printf("T5 ");
					TnMappingTab[Tn_cnt++] = 5;
				}
				printf("is Switching now!\n");
			}
		}
	}

#ifdef TCSUPPORT_CPU_MT7505
#if 1
	spiflash_clk_change(CLKRate_Value);
	clk_rate = 0;
#else
	if(CLKRate_Value == 0) {
		clk_rate = 0;
		spiflash_clk_change_25MHZ();
	} else if(CLKRate_Value == 1) {
		clk_rate = 1;
		spiflash_clk_change_50MHZ();
	}
#endif
#endif
	
	if(mtd.size >= SIZE_32MiB){
		if(Address3B4B_Value == 0) {
			enter_4Byte_mode();
			SEND_AUTO_4B_CMD;
    		printf("\n>>>come in 4Bytes Mode\n");
		} else {
			exit_4Byte_mode();
			SEND_AUTO_3B_CMD;
    		printf("\n>>>exit 4Bytes Mode\n");
		}
	}
	
	printf("\nAutoRead_cnt is %u, ManualRead_cnt is %u, MuxRead_cnt is %u\n", AutoRead_cnt, ManualRead_cnt, MuxRead_cnt);
	for(i=0; i<AutoRead_cnt; i++)
		printf("AutoRead[%d] is %u\n", i, AutoMappingTab[i]);
	for(i=0; i<ManualRead_cnt; i++)
		printf("ManualRead[%d] is %u\n", i, ManualMappingTab[i]);
	for(i=0; i<MuxRead_cnt; i++)
		printf("MuxRead[%d] is %u\n", i, MuxMappingTab[i]);
	
	printf("\n>>>>>>>>>>>> SPI FLASH Test Begin!!!\n");
	printf("\nVPint(CR_TIMER1_LDV) is 0x%08X\n", VPint(CR_TIMER1_LDV));
	//VPint(CR_TIMER1_LDV) = 0xFFFFFFFF;
	printf("\nVPint(CR_TIMER1_LDV) is 0x%08X\n", VPint(CR_TIMER1_LDV));

	sf_regDef_test();
	for(pattern=0; pattern<16; pattern++)
		sf_regRW_test(pattern);
	sf_regRW_test(0xFFFFFFFF);
	
	for(round = 0; round < Round_Value; round++) {

		printf("\nRound %d\n\n", round);

#if defined(TCSUPPORT_CPU_EN7512) || defined(TCSUPPORT_CPU_EN7521)   
#if 1 /* ASCI */
	if( CLKRate_Value == 0 ) /* Aoto test for multiple spi clock frequency */
	{
		if( (round %4) ==0)
		{

			VPint(0xBFA10098)=0;
			printf("CPOL register =0x%x\n", VPint(0xBFA10098));
			pause(1000);

			set_spi_clock_speed(7);

			/* adjust the delay parameter */
			VPint(0xbfa1009c) = 0x9;
			printf("0xbfa1009c register =0x%x\n", VPint(0xbfa1009c));
			pause(1000);

		}
		else if( (round %4) ==1)
	{
			VPint(0xBFA10098)=1;
		printf("CPOL register =0x%x\n", VPint(0xBFA10098)); 		
			pause(1000);

			set_spi_clock_speed(7);

			printf("Clock register =0x%x\n", VPint(0xBFA200CC));
			/* adjust the delay parameter */
			
			VPint(0xbfa1009c) = 0x9;
			printf("0xbfa1009c register =0x%x\n", VPint(0xbfa1009c));
			pause(1000);

	}
		else if( (round %4) ==2)
		{
			VPint(0xBFA10098)=0;
			printf("CPOL register =0x%x\n", VPint(0xBFA10098));
			pause(1000);

			set_spi_clock_speed(0xa);

			/* adjust the delay parameter */
				VPint(0xbfa1009c) = 0x9;			
				printf("0xbfa1009c register =0x%x\n", VPint(0xbfa1009c));
				pause(1000);

		}
		else if( (round %4) ==3)
	{
		VPint(0xBFA10098)=1;
		printf("CPOL register =0x%x\n", VPint(0xBFA10098)); 		
			pause(1000);			

			set_spi_clock_speed(0xa);

			/* adjust the delay parameter */
				VPint(0xbfa1009c) = 0x9;			
				printf("0xbfa1009c register =0x%x\n", VPint(0xbfa1009c));
				pause(1000);

		}
	}
	else 	/* Setting SPI clock frequency base on user intput */
	{
		set_spi_clock_speed(CLKRate_Value);
	}
#else  /* FPGA (Note: EN7512 FPGA only have 2 type of static clock speed to switch) */
		if( (round %4) ==0)
		{
			VPint(0xBFA10098)=0;
			printf("CPOL register =0x%x\n", VPint(0xBFA10098));			

			VPint(0xBFA2FF00) &= 0xFFFFFFFE ;
			printf("Clock register =0x%x\n", VPint(0xBFA2FF00));
			/* The  50Mhz must adjust the delay parameter */
			VPint(0xbfa1009c) = 0xa;
			printf("0xbfa1009c registet =0x%x\n", VPint(0xbfa1009c));
		}
		if( (round %4) ==1)
		{
			VPint(0xBFA10098)=1;
			printf("CPOL register =0x%x\n", VPint(0xBFA10098));

			VPint(0xBFA2FF00) &= 0xFFFFFFFE ;
			printf("Clock register =0x%x\n", VPint(0xBFA2FF00));
			/* The  50Mhz must adjust the delay parameter */
			VPint(0xbfa1009c) = 0xa;			
			printf("0xbfa1009c registet =0x%x\n", VPint(0xbfa1009c));
		}
		if( (round %4) ==2)
		{
			VPint(0xBFA10098)=0;
			printf("CPOL register =0x%x\n", VPint(0xBFA10098));

			VPint(0xBFA2FF00) |= 0x1;
			printf("Clock register =0x%x\n", VPint(0xBFA2FF00));
			/* The  25Mhz must adjust the delay parameter */
			VPint(0xbfa1009c) = 0x9;			
			printf("0xbfa1009c registet =0x%x\n", VPint(0xbfa1009c));
		}
		if( (round %4) ==3)
		{
			VPint(0xBFA10098)=1;
			printf("CPOL register =0x%x\n", VPint(0xBFA10098));

			VPint(0xBFA2FF00) |= 0x1;
			printf("Clock register =0x%x\n", VPint(0xBFA2FF00));
			/* The  25Mhz must adjust the delay parameter */
			VPint(0xbfa1009c) = 0x9;				
			printf("0xbfa1009c registet =0x%x\n", VPint(0xbfa1009c));
		}
#endif		
#endif
	
		if(AutoRead_Value != 0) {
			result = sf_auto_test(round);
			if((result != 0) && (Exit_Value == 1))
				return -13;
		}
		if(ManualRead_Value != 0) {
			result = sf_manual_test(round);
			if((result != 0) && (Exit_Value == 1))
				return -14;
		}
		if(MuxRead_Value != 0) {
			result = sf_mux_test(round);
			if((result != 0) && (Exit_Value == 1))
				return -15;
		}
	}
	return 0;
}

int do_sf_mats(int argc, char *argv[])
{
	int i=0;
	u32 round = 0;
	u32 pattern = 0;
	int result = 0;
	
	Mats_Round_Value = 10;
	Mats_Scope_Value = 0;
	Mats_isChipTest_Value = 0;
	Mats_ReadMode_Value = 0;
	Mats_Address_Value = 0x700000;

	clk_rate = 0;
	Address3B4B_Value = 0;
	CLKRate_Value = 0;
	ReadIdle_Value = 0;
	
	if(argc == 1){
		SF_Mats_Helper();
		return -1;
	}
	
	if(argc >= 2){
		Mats_Round_Value = simple_strtoul(argv[1], NULL, 10);
		printf("Mats Round is %lu\n", Mats_Round_Value);
	}
	if(argc >= 3){
		Mats_Scope_Value = simple_strtoul(argv[2], NULL, 10);
		if(Mats_Scope_Value > 1) {
			printf("\nError: Scope Value must be [0~1]!\n");
			SF_Mats_Helper();
			return -2;
		}
		else
			printf("Mats Test Scope is %s\n", ((Mats_Scope_Value==0)?"64KB":"All Flash"));
	}
	if(argc >= 4){
		Mats_isChipTest_Value = simple_strtoul(argv[3], NULL, 10);
		if(Mats_isChipTest_Value > 1) {
			printf("\nError: isChipTest Value must be [0~1]!\n");
			SF_Mats_Helper();
			return -3;
		}
		else
			printf("Mats Chip Test is %s\n", ((Mats_isChipTest_Value==1)?"enable":"disable"));
	}
	if(argc >= 5){
		Exit_Value = simple_strtoul(argv[4], NULL, 10);
		if(Exit_Value > 1) {
			printf("\nError: Exit Value must be [0~1]!\n");
			SF_Mats_Helper();
			return -4;
		}
		else
			printf("Exit while error is %s\n", ((Exit_Value==1)?"enable":"disable"));
	}
	if(argc >= 6){
		Mats_ReadMode_Value = simple_strtoul(argv[5], NULL, 10);
		if(Mats_ReadMode_Value > 7) {
			printf("\nError: Read Mode must be [0~7]!\n");
			SF_Mats_Helper();
			return -5;
		}
		else
			printf("Mats Read Mode is %s\n", MuxMode_TEXT[Mats_ReadMode_Value]);
	}
	if(argc >= 7){
		Mats_Address_Value = simple_strtoul(argv[6], NULL, 16);
		Mats_Address_Value &= 0xFFFF0000;
		if(Mats_Address_Value < 0x20000) {
			Mats_Address_Value = 0x20000;
		}
		printf("Mats Test Start Address 0x%08x\n", Mats_Address_Value);
	}
	
	spiflash_clk_change_25MHZ();
	SEND_AUTO_READ_CMD;
	
	if(mtd.size >= SIZE_32MiB){
		enter_4Byte_mode();
	}
	
	for(round = 0; round < Mats_Round_Value; round++) {
		result = sf_mats_test(round);
		if((result != 0) && (Exit_Value == 1))
			return -6;
	}
}


int do_sf_be(int argc, char *argv[])
{
	spiflash_bulk_erase();
	printf("\nbulk erase time is %lums\n", be_time);
}

int do_sf_readidle_test(int argc, char *argv[])
{
	register unsigned char tmp=0;
	register unsigned int tmp4=0;
	u32 count = 0x10000;
	u32 blockNum = 0;
	unsigned char *s = 0xbfc00000;
	unsigned int *s4 = 0xbfc00000;
	u8 read_idle_en = 0;
	u8 readMode = 0;
	u32 begtime=0, endtime=0, passtime=0;
	u32 begtick=0, endtick=0;

	if(argc == 1){
		printf("sfreadidle <idleEn:0~1> <blockNum:0~100> <4ByteRead:0~1>\n");
		return -1;
	}
	
	if(argc >= 2){
		read_idle_en = simple_strtoul(argv[1], NULL, 10);
		if(read_idle_en <=1)
			printf("read_idle_en is %s\n", ((read_idle_en==1)?("enable"):("disable")));
		else {
			printf("read_idle_en should be in [0~1]\n");
			return -2;
		}
	}
	if(argc >= 3){
		blockNum = simple_strtoul(argv[2], NULL, 10);
		printf("block Num is %d\n", blockNum);
		if(blockNum == 0) {
			printf("block Num should not be 0, change to 1!\n");
			blockNum = 1;
		}
		count *= blockNum; 
	}
	if(argc >= 4){
		readMode = simple_strtoul(argv[3], NULL, 10);
		printf("Read Mode is %d\n", readMode);
		if(readMode <=1)
			printf("read in 4Byte Mode is %s\n", ((readMode==1)?("enable"):("disable")));
		else {
			printf("read mode should be in [0~1]\n");
			return -3;
		}
	}

	printf("1::read_idle_en status is %d\n", VPint(SF_READ_IDLE_EN));
	
	begtime = getTime();
	begtick = VPint(CR_TIMER1_VLR);

	if((readMode == 0) && (read_idle_en == 1)) {
		while (count--) {
			WriteReg(SF_READ_IDLE_EN, RD_IDLE_EN);
			tmp = *s++;
		}
	} else if((readMode == 0) && (read_idle_en == 0)) {
		while (count--) {
			WriteReg(SF_READ_IDLE_EN, RD_IDLE_DIS);
			tmp = *s++;
		}
	} else if((readMode == 1) && (read_idle_en == 1)) {
		while (count--) {
			WriteReg(SF_READ_IDLE_EN, RD_IDLE_EN);
			tmp4 = *s4++;
		}
	} else if((readMode == 1) && (read_idle_en == 0)) {
		while (count--) {
			WriteReg(SF_READ_IDLE_EN, RD_IDLE_DIS);
			tmp4 = *s4++;
		}
	}

	endtick = VPint(CR_TIMER1_VLR);
	endtime = getTime();

	printf("2::read_idle_en status is %d\n", VPint(SF_READ_IDLE_EN));
	
	if(endtime < begtime)
		passtime = 0xffffffff - begtime + endtime;
	else
		passtime = endtime - begtime;
	passtime = (passtime/10) * VPint(CR_TIMER1_LDV)+ begtick - endtick;
	printf("read_idle_en %s::passtime is %lu\n", ((read_idle_en==1)?("enable"):("disable")), passtime);
	printf("s is 0x%08x, s4 is 0x%08x\n\n", s, s4);

	return tmp+tmp4;
}

int do_sf_bootin4Byte(int argc, char *argv[])
{
	return sf_bootin4Byte_Entry(argc, argv);	
}

static int do_sf_nonAlign_test(int argc, char *argv[])
{
	int i=0, j=0;
	unsigned long curAddr=0, curAddr2=0, curLen=0;
	unsigned long retlen=0;
	unsigned long erasesize=0;
	unsigned char seed=0, seedTmp=0;
	unsigned char pp_buf[PP_MAX]={0};
	unsigned char rd_buf[4100]={0};
	unsigned long startAddr=0x80000;
	unsigned long endAddr=0x80801;
	unsigned long seEndAddr=0x81000;
	unsigned long ppStartAddr=0x80100, ppEndAddr=0x80200;
	unsigned long endPPLen=0x101, endRDLen=0x801;
	unsigned long curtime=0;
	unsigned long nonAlignFlag=0;
	int rd_en=1, se_en=1, pp_en=1;
		
	erasesize = mtd.erasesize;
	
	if(argc >= 2){
		nonAlignFlag = simple_strtoul(argv[1], NULL, 10);
		if((nonAlignFlag&0x4)==0) {
			rd_en=0;
		} else {
			rd_en=1;
		}		
		if((nonAlignFlag&0x2)==0) {
			se_en=0;
		} else {
			se_en=1;
		}		
		if((nonAlignFlag&0x1)==0) {
			pp_en = 0;
		} else {
			pp_en = 1;
		}
		if((nonAlignFlag&0x8)!=0) {
			endAddr=0x80081;
			seEndAddr=0x80081;
			ppEndAddr=0x80110;
			printf("\nrdEndAddr is 0x%08x, seEndAddr is 0x%08x, ppEndAddr is 0x%08x", endAddr, seEndAddr, ppEndAddr);
		}
		printf("\nnonAlignFlag is %lu, rd_en is %d, se_en is %d, pp_en is %d\n", nonAlignFlag, rd_en, se_en, pp_en);
	}

	/***************************************/
	/********** non-aligned Read Test **********/
	/***************************************/
	if(rd_en == 1) {
		printf("\n>>>non-aligned Read Test begin");
		//1: Erase 64KB Sector
		flash_erase(startAddr, erasesize);

		//2: Program 64KB Sector with random ascend digitals
		seed = sf_rand() % SEED_MAX;
		for(i=0; i<PP_MAX; i++) {
			pp_buf[i] = (seed+i) % SEED_MAX;
		}
		for(curAddr=startAddr; curAddr<startAddr+erasesize; curAddr+=PP_MAX)
			flash_write(curAddr, PP_MAX, &retlen, pp_buf);

		//3: Loop Read with non-aligned Address and Variable length
		for(curAddr=startAddr; curAddr<=endAddr; ++curAddr) {
			curtime = getTime();
			printf("\nnon-aligned Read: curAddr is 0x%08x, curTime is %02d:%02d:%02d", curAddr, curtime/3600000, (curtime/60000)%60, (curtime/1000)%60);
			for(curLen=0; curLen<=endRDLen; ++curLen) {
				seedTmp = (seed+curAddr)%SEED_MAX;
				
				//3.1: Auto Read Test with designated Address and Length
				Mats_ReadMode_Value=0;
				spiflash_mats_read(curAddr, curLen, &retlen, rd_buf);
				for(i=0; i<curLen; i++) {
					if(rd_buf[i] != (seedTmp+i)%SEED_MAX) {
						printf("\n####Auto Read fail at Address %X::index is %d, value is %X, should be %X, curLen is %d, reg 0xBFA10000 is 0x%08x\n", curAddr+i, i, rd_buf[i], (seedTmp+i)%SEED_MAX, curLen, SF_READ_MODE_VALUE);
						dumpCell_buffer((u8 *)(rd_buf), curLen, curAddr);
						goto read_fail_exit;
					}
				}
					
				//3.2: Manual Read Test with designated Address and Length
				Mats_ReadMode_Value=4;
				spiflash_mats_read(curAddr, curLen, &retlen, rd_buf);
				for(i=0; i<curLen; i++) {
					if(rd_buf[i] != (seedTmp+i)%SEED_MAX) {
						printf("\n####Manual Read fail at Address %X::index is %d, value is %X, should be %X, curLen is %d, reg 0xBFA10000 is 0x%08x\n", curAddr+i, i, rd_buf[i], (seedTmp+i)%SEED_MAX, curLen, SF_READ_MODE_VALUE);
						dumpCell_buffer((u8 *)(rd_buf), curLen, curAddr);
						goto read_fail_exit;
					}
				}
			}
		}
		printf("\n<<<non-aligned Read Test passed!\n");
	}


	
	/****************************************/
	/********** non-aligned Erase Test **********/
	/****************************************/
	if(se_en == 1) {
		printf("\n>>>non-aligned Erase Test begin");	
		for(curAddr=startAddr; curAddr<=seEndAddr; ++curAddr) {
			curtime = getTime();
			printf("\nnon-aligned Erase: curAddr is 0x%08x, curTime is %02d:%02d:%02d", curAddr, curtime/3600000, (curtime/60000)%60, (curtime/1000)%60);		
			//1: Erase 3*64KB Sector			
			flash_erase(startAddr-erasesize, erasesize);
			flash_erase(startAddr, erasesize);
			flash_erase(startAddr+erasesize, erasesize);

			//2: Program 3*64KB Sector with 0
			for(i=0; i<PP_MAX; i++) {
				pp_buf[i] = 0;
			}
			for(curAddr2=startAddr-erasesize; curAddr2<startAddr+erasesize*2; curAddr2+=PP_MAX)
				flash_write(curAddr2, PP_MAX, &retlen, pp_buf);

			//3: Erase the second Sector
			flash_erase(curAddr, erasesize);
			
			//4: Read Data with designated Address and Length to verify
			Mats_ReadMode_Value=0;
			for(curAddr2=startAddr-erasesize; curAddr2<startAddr+erasesize*2; curAddr2+=SE_MAX) {
				spiflash_mats_read(curAddr2, SE_MAX, &retlen, rd_buf);
				for(i=0; i<SE_MAX; i++) {
					if( ((rd_buf[i] != 0xFF) && (curAddr2>=startAddr) && (curAddr2<startAddr+erasesize)) || ((rd_buf[i] != 0) && ((curAddr2<startAddr) || (curAddr2>=startAddr+erasesize))) ) {
						printf("\n####Erase fail at Address %X::index is %d, value is %X, should be %X, curLen is %d, reg 0xBFA10000 is 0x%08x\n", curAddr2+i, i, rd_buf[i], (((curAddr2<startAddr) || (curAddr2>=startAddr+erasesize))?(0):(0xFF)), SE_MAX, SF_READ_MODE_VALUE);
						dumpCell_buffer((u8 *)(rd_buf), SE_MAX, curAddr2);
						goto erase_fail_exit;
					}
				}
			}
		}	
		printf("\n<<<non-aligned Erase Test passed!\n");
	}


	/******************************************/
	/********** non-aligned Program Test **********/
	/******************************************/
	if(pp_en == 1) {
		printf("\n>>>non-aligned Program Test begin");	
		for(curAddr=ppStartAddr; curAddr<=ppEndAddr; ++curAddr) {
			curtime = getTime();
			printf("\nnon-aligned Program: curAddr is 0x%08x, curTime is %02d:%02d:%02d", curAddr, curtime/3600000, (curtime/60000)%60, (curtime/1000)%60);	
			for(curLen=0; curLen<=endPPLen; ++curLen) {
				//1: Erase 64KB Sector
				flash_erase(startAddr, erasesize);
				
				//2: Program with non-aligned Address and Variable length
				seed = sf_rand() % SEED_MAX;
				for(i=0; i<curLen; i++) {
					rd_buf[i] = (seed+i) % SEED_MAX;
				}
				flash_write(curAddr, curLen, &retlen, rd_buf);

				//3: Read Data with designated Address and Length to verify
				Mats_ReadMode_Value=0;
				spiflash_mats_read(curAddr, curLen, &retlen, rd_buf);
				for(i=0; i<curLen; i++) {
					if(rd_buf[i] != (seed+i)%SEED_MAX) {
						printf("\n####Program fail at Address %X::index is %d, value is %X, should be %X, curLen is %d, reg 0xBFA10000 is 0x%08x\n", curAddr+i, i, rd_buf[i], (seed+i)%SEED_MAX, curLen, SF_READ_MODE_VALUE);
						dumpCell_buffer((u8 *)(rd_buf), curLen, curAddr);
						goto program_fail_exit;
					}
				}
			}
		}
		printf("\n<<<non-aligned Program Test passed!\n");
	}
	
	printf("\nnon-aligned Test Finished\n");
	return 0;
	
erase_fail_exit:
	return -1;
	
program_fail_exit:
	return -2;

read_fail_exit:
	return -3;
}

#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG_EXT
int do_sf_memcpy(int argc, char *argv[])
{ 
	u32 srcAddr=0;
	u32 destAddr=0;
	u32 len=0;

	if(argc <= 3){
		printf("sfmemcpy <destAddr> <srcAddr> <len>\n");
		return -1;
	}
	
	destAddr = simple_strtoul(argv[1], NULL, 16);
	printf("destAddr is 0x%08x\n", destAddr);
	srcAddr = simple_strtoul(argv[2], NULL, 16);
	printf("srcAddr is 0x%08x\n", srcAddr);
	len = simple_strtoul(argv[3], NULL, 16);
	printf("len is 0x%08x\n", len);

	srcAddr &= 0x03FFFFFF;
	srcAddr |= 0xBC000000;

	memcpy4((unsigned int*)destAddr, (unsigned int*)srcAddr, len);

	dumpCell_buffer((u8 *)destAddr, len, srcAddr);

	return 0;
}

static int do_sf_1ByteTest(int argc, char *argv[])
{
	u32 i=0;
	u32 startAddr=0x20000;
	u32 steplen=0x100;
	u32 cnt=1000;
	u32 rd_begtime=0, rd_endtime=0, passtime=0;
	u32 rd_begtick=0, rd_endtick=0;
	unsigned char buf[2];

	if(argc == 1){
		printf("sf1ByteTest <startAddr> <steplen> <cnt>\n");
		return -1;
	}
	
	if(argc >= 2){
		startAddr = simple_strtoul(argv[1], NULL, 16);
		startAddr &= 0xFFFF0000;
		if(startAddr < 0x20000) {
			startAddr = 0x20000;
		}
		printf("Start Address 0x%08x\n", startAddr);
	}
	if(argc >= 3){
		steplen = simple_strtoul(argv[2], NULL, 16);
		if(steplen == 0) {
			steplen = 1;
		}
		printf("steplen is 0x%04x\n", steplen);
	}
	if(argc >= 4){
		cnt = simple_strtoul(argv[3], NULL, 10);
		if(cnt == 0) {
			cnt = 1;
		}
		printf("cnt is %lux\n", cnt);
	}
	
	SEND_READ_IDLE_EN_CMD;
	
	rd_begtime = getTime();
	rd_begtick = VPint(CR_TIMER1_VLR);
	
	for(i=0; i<cnt; i++) {
		memcpy(buf, (unsigned char*)((mtd.offset + startAddr)), 1);
		startAddr += steplen;
	}
	
	rd_endtick = VPint(CR_TIMER1_VLR);
	rd_endtime = getTime();
	if(rd_endtime < rd_begtime)
		passtime = 0xffffffff - rd_begtime + rd_endtime;
	else
		passtime = rd_endtime - rd_begtime;
	passtime = (passtime/10) * VPint(CR_TIMER1_LDV) + rd_begtick - rd_endtick;
	
	SEND_READ_IDLE_DIS_CMD;

	printf("\npasstime is %lu\n\n", passtime);

	return 0;

}

int do_sf_ClockSet(int argc, char *argv[])
{
	u8 sfClkRateValue=0;

	if(argc == 1) {
		printf("sfclockset <Clock:0~31>\n");
		return -1;
	}
	
	if(argc >= 2) {
		sfClkRateValue = simple_strtoul(argv[1], NULL, 10);
		if(sfClkRateValue == 0)
			sfClkRateValue = 10;
		if(sfClkRateValue <= 31) {
			printf("sfClkRateValue is %d, SFC Clock Rate is %dMHZ\n", sfClkRateValue, (SFC_CLOCK_MAX/sfClkRateValue));
		}
		else {
			printf("sfClkRateValue should be in [0-31]\n");
			return -2;
		}
	}

	WriteReg(SF_CLK_CHANGE, sfClkRateValue);
	return 0;
}

int do_sf_TnSet(int argc, char *argv[])
{
	u8 sfTnValue=0;

	if(argc == 1) {
		printf("sftnset <Tn:0~5>\n");
		return -1;
	}
	
	if(argc >= 2) {
		sfTnValue = simple_strtoul(argv[1], NULL, 10);
		if(sfTnValue <= 5) {
			printf("sfTnValue is %d\n", sfTnValue);
		}
		else {
			printf("sfTnValue should be in [0-5]\n");
			return -2;
		}
	}

	SEND_TN_CMD(sfTnValue);
	return 0;
}
#endif

void sf_regDef_test()
{
	int i=0;
	int j=1;

	printf("\n>>>>>>>SF Register Default Test Begin !!!\n");

	//	SF_READ_MODE
	if(SF_READ_MODE_VALUE == 0x00000000){
		//printf("\r\n%d. SF_READ_MODE default value correct.", j++);
	}else{
		printf("\r\n%d. SF_READ_MODE (default/real): (0x00000000/0x%08x).", j++, (uint32)SF_READ_MODE_VALUE);
	}

	//	SF_READ_IDLE_EN
	if(SF_READ_IDLE_EN_VALUE == 0x00000000){
		//printf("\r\n%d. SF_READ_IDLE_EN default value correct.", j++);
	}else{
		printf("\r\n%d. SF_READ_IDLE_EN (default/real): (0x00000000/0x%08x).", j++, (uint32)SF_READ_IDLE_EN_VALUE);
	}

	//	SF_SIDLY
	if(SF_SIDLY_VALUE == 0x00000000){
		//printf("\r\n%d. SF_SIDLY default value correct.", j++);
	}else{
		printf("\r\n%d. SF_SIDLY (default/real): (0x00000000/0x%08x).", j++, (uint32)SF_SIDLY_VALUE);
	}

	//	SF_CSHEXT
	if(SF_CSHEXT_VALUE == 0x00000005){
		//printf("\r\n%d. SF_CSHEXT default value correct.", j++);
	}else{
		printf("\r\n%d. SF_CSHEXT (default/real): (0x00000005/0x%08x).", j++, (uint32)SF_CSHEXT_VALUE);
	}

	//	SF_CSLEXT
	if(SF_CSLEXT_VALUE == 0x00000001){
		//printf("\r\n%d. SF_CSLEXT default value correct.", j++);
	}else{
		printf("\r\n%d. SF_CSLEXT (default/real): (0x00000001/0x%08x).", j++, (uint32)SF_CSLEXT_VALUE);
	}

	//	SF_MTX_MODE_TOG
	if(SF_MTX_MODE_TOG_VALUE == 0x00000000){
		//printf("\r\n%d. SF_MTX_MODE_TOG default value correct.", j++);
	}else{
		printf("\r\n%d. SF_MTX_MODE_TOG (default/real): (0x00000000/0x%08x).", j++, (uint32)SF_MTX_MODE_TOG_VALUE);
	}

	//	SF_RDCTL_FSM
	if(SF_RDCTL_FSM_VALUE == 0x00000000){
		//printf("\r\n%d. SF_RDCTL_FSM default value correct.", j++);
	}else{
		printf("\r\n%d. SF_RDCTL_FSM (default/real): (0x00000000/0x%08x).", j++, (uint32)SF_RDCTL_FSM_VALUE);
	}

	//	SF_MACMUX_SEL
	if(SF_MACMUX_SEL_VALUE == 0x00000001){
		//printf("\r\n%d. SF_MACMUX_SEL default value correct.", j++);
	}else{
		printf("\r\n%d. SF_MACMUX_SEL (default/real): (0x00000001/0x%08x).", j++, (uint32)SF_MACMUX_SEL_VALUE);
	}

	//	SF_MANUAL_EN
	if(SF_MANUAL_EN_VALUE == 0x00000000){
		//printf("\r\n%d. SF_MANUAL_EN default value correct.", j++);
	}else{
		printf("\r\n%d. SF_MANUAL_EN (default/real): (0x00000000/0x%08x).", j++, (uint32)SF_MANUAL_EN_VALUE);
	}

	//	SF_MANUAL_OPFIFO_EMPTY
	if(SF_MANUAL_OPFIFO_EMPTY_VALUE == 0x00000001){
		//printf("\r\n%d. SF_MANUAL_OPFIFO_EMPTY default value correct.", j++);
	}else{
		printf("\r\n%d. SF_MANUAL_OPFIFO_EMPTY (default/real): (0x00000001/0x%08x).", j++, (uint32)SF_MANUAL_OPFIFO_EMPTY_VALUE);
	}

	//	SF_MANUAL_OPFIFO_WDATA
	if(SF_MANUAL_OPFIFO_WDATA_VALUE == 0x00000000){
		//printf("\r\n%d. SF_MANUAL_OPFIFO_WDATA default value correct.", j++);
	}else{
		printf("\r\n%d. SF_MANUAL_OPFIFO_WDATA (default/real): (0x00000000/0x%08x).", j++, (uint32)SF_MANUAL_OPFIFO_WDATA_VALUE);
	}

	//	SF_MANUAL_OPFIFO_FULL
	if(SF_MANUAL_OPFIFO_FULL_VALUE == 0x00000000){
		//printf("\r\n%d. SF_MANUAL_OPFIFO_FULL default value correct.", j++);
	}else{
		printf("\r\n%d. SF_MANUAL_OPFIFO_FULL (default/real): (0x00000000/0x%08x).", j++, (uint32)SF_MANUAL_OPFIFO_FULL_VALUE);
	}

	//	SF_MANUAL_OPFIFO_WR
	if(SF_MANUAL_OPFIFO_WR_VALUE == 0x00000000){
		//printf("\r\n%d. SF_MANUAL_OPFIFO_WR default value correct.", j++);
	}else{
		printf("\r\n%d. SF_MANUAL_OPFIFO_WR (default/real): (0x00000000/0x%08x).", j++, (uint32)SF_MANUAL_OPFIFO_WR_VALUE);
	}

	//	SF_MANUAL_DFIFO_FULL
	if(SF_MANUAL_DFIFO_FULL_VALUE == 0x00000000){
		//printf("\r\n%d. SF_MANUAL_DFIFO_FULL default value correct.", j++);
	}else{
		printf("\r\n%d. SF_MANUAL_DFIFO_FULL (default/real): (0x00000000/0x%08x).", j++, (uint32)SF_MANUAL_DFIFO_FULL_VALUE);
	}

	//	SF_MANUAL_DFIFO_WDATA
	if(SF_MANUAL_DFIFO_WDATA_VALUE == 0x00000000){
		//printf("\r\n%d. SF_MANUAL_DFIFO_WDATA default value correct.", j++);
	}else{
		printf("\r\n%d. SF_MANUAL_DFIFO_WDATA (default/real): (0x00000000/0x%08x).", j++, (uint32)SF_MANUAL_DFIFO_WDATA_VALUE);
	}

	//	SF_MANUAL_DFIFO_EMPTY
	if(SF_MANUAL_DFIFO_EMPTY_VALUE == 0x00000000){
		//printf("\r\n%d. SF_MANUAL_DFIFO_EMPTY default value correct.", j++);
	}else{
		printf("\r\n%d. SF_MANUAL_DFIFO_EMPTY (default/real): (0x00000000/0x%08x).", j++, (uint32)SF_MANUAL_DFIFO_EMPTY_VALUE);
	}

	//	SF_MANUAL_DFIFO_RD
	if(SF_MANUAL_DFIFO_RD_VALUE == 0x00000000){
		//printf("\r\n%d. SF_MANUAL_DFIFO_RD default value correct.", j++);
	}else{
		printf("\r\n%d. SF_MANUAL_DFIFO_RD (default/real): (0x00000000/0x%08x).", j++, (uint32)SF_MANUAL_DFIFO_RD_VALUE);
	}

	//	SF_MANUAL_DFIFO_RDATA
	if(SF_MANUAL_DFIFO_RDATA_VALUE == 0x00000000){
		//printf("\r\n%d. SF_MANUAL_DFIFO_RDATA default value correct.", j++);
	}else{
		printf("\r\n%d. SF_MANUAL_DFIFO_RDATA (default/real): (0x00000000/0x%08x).", j++, (uint32)SF_MANUAL_DFIFO_RDATA_VALUE);
	}

	//	SF_DUMMY
	if(SF_DUMMY_VALUE == 0x00000001){
		//printf("\r\n%d. SF_DUMMY default value correct.", j++);
	}else{
		printf("\r\n%d. SF_DUMMY (default/real): (0x00000001/0x%08x).", j++, (uint32)SF_DUMMY_VALUE);
	}

	//	SF_ADDR_3B4B
	if(SF_ADDR_3B4B_VALUE == 0x00000000){
		//printf("\r\n%d. SF_ADDR_3B4B default value correct.", j++);
	}else{
		printf("\r\n%d. SF_ADDR_3B4B (default/real): (0x00000000/0x%08x).", j++, (uint32)SF_ADDR_3B4B_VALUE);
	}

	//	SF_CFG3B4B_EN
	if(SF_CFG3B4B_EN_VALUE == 0x00000000){
		//printf("\r\n%d. SF_CFG3B4B_EN default value correct.", j++);
	}else{
		printf("\r\n%d. SF_CFG3B4B_EN (default/real): (0x00000000/0x%08x).", j++, (uint32)SF_CFG3B4B_EN_VALUE);
	}

	//	SF_INTERRUPT
	if(SF_INTERRUPT_VALUE == 0x00000000){
		//printf("\r\n%d. SF_INTERRUPT default value correct.", j++);
	}else{
		printf("\r\n%d. SF_INTERRUPT (default/real): (0x00000000/0x%08x).", j++, (uint32)SF_INTERRUPT_VALUE);
	}

	//	SF_INTERRUPT_EN
	if(SF_INTERRUPT_EN_VALUE == 0x00000000){
		//printf("\r\n%d. SF_INTERRUPT default value correct.", j++);
	}else{
		printf("\r\n%d. SF_INTERRUPT_EN (default/real): (0x00000000/0x%08x).", j++, (uint32)SF_INTERRUPT_EN_VALUE);
	}

	//	SF_SI_CK_SEL
	if(SF_SI_CK_SEL_VALUE == 0x00000009){
		//printf("\r\n%d. SF_CLK_CHANGE default value correct.", j++);
	}else{
		printf("\r\n%d. SF_SI_CK_SEL (default/real): (0x00000009/0x%08x).", j++, (uint32)SF_SI_CK_SEL_VALUE);
	}

	//	SF_CLK_CHANGE
	if(SF_CLK_CHANGE_VALUE == 0x00000000){
		//printf("\r\n%d. SF_CLK_CHANGE default value correct.", j++);
	}else{
		printf("\r\n%d. SF_CLK_CHANGE (default/real): (0x00000000/0x%08x).\n", j++, (uint32)SF_CLK_CHANGE_VALUE);
	}
}

void sf_regRW_test(u32 pattern)
{
	int i=0;
	int j=1;
	u32 rval=0;

	printf("\n>>>>>>>SF Register RW Test Begin, pattern is 0x%08x!!!\n", pattern);

	//	SF_READ_MODE
	rval = SF_READ_MODE_VALUE;
	SF_READ_MODE_VALUE = pattern;
	if(SF_READ_MODE_VALUE == (pattern & 0x00000007)){
		//printf("\r\n%d. SF_READ_MODE write 0x%08x correct.", j++, pattern);
	}else{	
		printf("\r\n%d. SF_READ_MODE (default/real): (0x%08x/0x%08x).", j++,(pattern & 0x00000007),  (uint32)SF_READ_MODE_VALUE);
	}
	SF_READ_MODE_VALUE = rval;

	//	SF_READ_IDLE_EN
	rval = SF_READ_IDLE_EN_VALUE;
	SF_READ_IDLE_EN_VALUE = pattern;
	if(SF_READ_IDLE_EN_VALUE == (pattern & 0x00000001)){
		//printf("\r\n%d. SF_READ_IDLE_EN write 0x%08x correct.", j++, pattern);
	}else{	
		printf("\r\n%d. SF_READ_IDLE_EN (default/real): (0x%08x/0x%08x).", j++,(pattern & 0x00000001),  (uint32)SF_READ_IDLE_EN_VALUE);
	}
	SF_READ_IDLE_EN_VALUE = rval;

	//	SF_SIDLY
	rval = SF_SIDLY_VALUE;
	SF_SIDLY_VALUE = pattern;
	if(SF_SIDLY_VALUE == (pattern & 0x00000003)){
		//printf("\r\n%d. SF_SIDLY write 0x%08x correct.", j++, pattern);
	}else{	
		printf("\r\n%d. SF_SIDLY (default/real): (0x%08x/0x%08x).", j++,(pattern & 0x00000003),  (uint32)SF_SIDLY_VALUE);
	}
	SF_SIDLY_VALUE = rval;

	//	SF_CSHEXT
	rval = SF_CSHEXT_VALUE;
	SF_CSHEXT_VALUE = pattern;
	if(SF_CSHEXT_VALUE == (pattern & 0x00000007)){
		//printf("\r\n%d. SF_CSHEXT write 0x%08x correct.", j++, pattern);
	}else{	
		printf("\r\n%d. SF_CSHEXT (default/real): (0x%08x/0x%08x).", j++,(pattern & 0x00000007),  (uint32)SF_CSHEXT_VALUE);
	}
	SF_CSHEXT_VALUE = rval;

	//	SF_CSLEXT
	rval = SF_CSLEXT_VALUE;
	SF_CSLEXT_VALUE = pattern;
	if(SF_CSLEXT_VALUE == (pattern & 0x00000007)){
		//printf("\r\n%d. SF_CSLEXT write 0x%08x correct.", j++, pattern);
	}else{	
		printf("\r\n%d. SF_CSLEXT (default/real): (0x%08x/0x%08x).", j++, (pattern & 0x00000007), (uint32)SF_CSLEXT_VALUE);
	}
	SF_CSLEXT_VALUE = rval;

	//	SF_MTX_MODE_TOG
	rval = SF_MTX_MODE_TOG_VALUE;
	SF_MTX_MODE_TOG_VALUE = pattern;
	if(SF_MTX_MODE_TOG_VALUE == (pattern & 0x0000000F)){
		//printf("\r\n%d. SF_MTX_MODE_TOG write 0x%08x correct.", j++, pattern);
	}else{	
		printf("\r\n%d. SF_MTX_MODE_TOG (default/real): (0x%08x/0x%08x).", j++, (pattern & 0x0000000F), (uint32)SF_MTX_MODE_TOG_VALUE);
	}
	SF_MTX_MODE_TOG_VALUE = rval;

	//	skip SF_RDCTL_FSM  (RO)

	//	SF_MACMUX_SEL
	rval = SF_MACMUX_SEL_VALUE;
	SF_MACMUX_SEL_VALUE = pattern;
	if(SF_MACMUX_SEL_VALUE == (pattern & 0x00000001)){
		//printf("\r\n%d. SF_MACMUX_SEL write 0x%08x correct.", j++, pattern);
	}else{	
		printf("\r\n%d. SF_MACMUX_SEL (default/real): (0x%08x/0x%08x).", j++, (pattern & 0x00000001), (uint32)SF_MACMUX_SEL_VALUE);
	}
	SF_MACMUX_SEL_VALUE = rval;

	//	SF_MANUAL_EN
	rval = SF_MANUAL_EN_VALUE;
	SF_MANUAL_EN_VALUE = pattern;
	if(SF_MANUAL_EN_VALUE == (pattern & 0x00000001)){
		//printf("\r\n%d. SF_MANUAL_EN write 0x%08x correct.", j++, pattern);
	}else{	
		printf("\r\n%d. SF_MANUAL_EN (default/real): (0x%08x/0x%08x).", j++, (pattern & 0x00000001), (uint32)SF_MANUAL_EN_VALUE);
	}
	SF_MANUAL_EN_VALUE = rval;

	//	skip SF_MANUAL_OPFIFO_EMPTY  (RO)

	//	SF_MANUAL_OPFIFO_WDATA
	rval = SF_MANUAL_OPFIFO_WDATA_VALUE;
	SF_MANUAL_OPFIFO_WDATA_VALUE = pattern;
	if(SF_MANUAL_OPFIFO_WDATA_VALUE == (pattern & 0x00003FFF)){
		//printf("\r\n%d. SF_MANUAL_OPFIFO_WDATA write 0x%08x correct.", j++, pattern);
	}else{	
		printf("\r\n%d. SF_MANUAL_OPFIFO_WDATA (default/real): (0x%08x/0x%08x).", j++,(pattern & 0x00003FFF),  (uint32)SF_MANUAL_OPFIFO_WDATA_VALUE);
	}
	SF_MANUAL_OPFIFO_WDATA_VALUE = rval;

	//	skip SF_MANUAL_OPFIFO_FULL (RO)
	
	//	skip SF_MANUAL_OPFIFO_WR (WO)
	
	//	skip SF_MANUAL_DPFIFO_FULL (RO)
	
	//	skip SF_MANUAL_DPFIFO_WDATA (WO)
	
	//	skip SF_MANUAL_DPFIFO_EMPTY (RO)
	
	//	skip SF_MANUAL_DPFIFO_RD (WO)
	
	//	skip SF_MANUAL_DPFIFO_RDATA(RO)

	//	SF_DUMMY
	rval = SF_DUMMY_VALUE;
	SF_DUMMY_VALUE = pattern;
	if(SF_DUMMY_VALUE == (pattern & 0x0000000F)){
		//printf("\r\n%d. SF_DUMMY write 0x%08x correct.", j++, pattern);
	}else{	
		printf("\r\n%d. SF_DUMMY (default/real): (0x%08x/0x%08x).", j++,(pattern & 0x0000000F),  (uint32)SF_DUMMY_VALUE);
	}
	SF_DUMMY_VALUE = rval;

	//	SF_ADDR_3B4B
	rval = SF_ADDR_3B4B_VALUE;
	SF_ADDR_3B4B_VALUE = pattern;
	if(SF_ADDR_3B4B_VALUE == (pattern & 0x00000001)){
		//printf("\r\n%d. SF_ADDR_3B4B write 0x%08x correct.", j++, pattern);
	}else{	
		printf("\r\n%d. SF_ADDR_3B4B (default/real): (0x%08x/0x%08x).", j++,(pattern & 0x00000001),  (uint32)SF_ADDR_3B4B_VALUE);
	}
	SF_ADDR_3B4B_VALUE = rval;

	//	SF_PROBE_SEL
	rval = SF_PROBE_SEL_VALUE;
	SF_PROBE_SEL_VALUE = pattern;
	if(SF_PROBE_SEL_VALUE == (pattern & 0x8000001F)){
		//printf("\r\n%d. SF_PROBE_SEL write 0x%08x correct.", j++, pattern);
	}else{	
		printf("\r\n%d. SF_PROBE_SEL (default/real): (0x%08x/0x%08x).", j++,(pattern & 0x8000001F),  (uint32)SF_PROBE_SEL_VALUE);
	}
	SF_PROBE_SEL_VALUE = rval;

	//	SF_CFG3B4B_EN
	rval = SF_CFG3B4B_EN_VALUE;
	SF_CFG3B4B_EN_VALUE = pattern;
	if(SF_CFG3B4B_EN_VALUE == (pattern & 0x00000001)){
		//printf("\r\n%d. SF_CFG3B4B_EN write 0x%08x correct.", j++, pattern);
	}else{	
		printf("\r\n%d. SF_CFG3B4B_EN (default/real): (0x%08x/0x%08x).", j++,(pattern & 0x00000001),  (uint32)SF_CFG3B4B_EN_VALUE);
	}
	SF_CFG3B4B_EN_VALUE = rval;
	
	//	skip SF_INTERRUPT (WO)

	//	SF_INTERRUPT_EN
	rval = SF_INTERRUPT_EN_VALUE;
	SF_INTERRUPT_EN_VALUE = pattern;
	if(SF_INTERRUPT_EN_VALUE == (pattern & 0x000001FF)){
		//printf("\r\n%d. SF_INTERRUPT_EN write 0x%08x correct.", j++, pattern);
	}else{	
		printf("\r\n%d. SF_INTERRUPT_EN (default/real): (0x%08x/0x%08x).", j++,(pattern & 0x000001FF),  (uint32)SF_INTERRUPT_EN_VALUE);
	}
	SF_INTERRUPT_EN_VALUE = rval;

	//	SF_SI_CK_SEL
	rval = SF_SI_CK_SEL_VALUE;
	SF_SI_CK_SEL_VALUE = pattern;
	if(SF_SI_CK_SEL_VALUE == (pattern & 0x0000000F)){
		//printf("\r\n%d. SF_SI_CK_SEL write 0x%08x correct.", j++, pattern);
	}else{	
		printf("\r\n%d. SF_SI_CK_SEL (default/real): (0x%08x/0x%08x).", j++,(pattern & 0x0000000F),  (uint32)SF_SI_CK_SEL_VALUE);
	}
	SF_SI_CK_SEL_VALUE = rval;
	
	//	SF_CLK_CHANGE
	rval = SF_CLK_CHANGE_VALUE;
	SF_CLK_CHANGE_VALUE = pattern;
	if(SF_CLK_CHANGE_VALUE == (pattern & 0x0000001F)){
		//printf("\r\n%d. SF_CLK_CHANGE write 0x%08x correct.", j++, pattern);
	}else{	
		printf("\r\n%d. SF_CLK_CHANGE (default/real): (0x%08x/0x%08x).", j++,(pattern & 0x0000001F),  (uint32)SF_CLK_CHANGE_VALUE);
	}
	SF_CLK_CHANGE_VALUE = rval;
}
#endif


#if defined(TCSUPPORT_CPU_EN7512)||defined(TCSUPPORT_CPU_EN7521)
#ifdef TCSUPPORT_NEW_SPIFLASH_DEBUG			/*Chuck Kuo, For SPI NAND Test */


#define SPI_NAND_CHECK_PATTERN_SIZE		2048

typedef enum{
	_SPI_NAND_TEST_RTN_NO_ERROR,
	_SPI_NAND_TEST_RTN_ERROR_WITH_SOME_REASON,
	_SPI_NAND_TEST_RTN_PARSER_ERROR,	
	_SPI_NAND_TEST_RTN_WRITE_MODE_CHECK_ERROR,
	_SPI_NAND_TEST_RTN_WRITE_MODE_NOT_SET,
	_SPI_NAND_TEST_RTN_READ_MODE_CHECK_ERROR,
	_SPI_NAND_TEST_RTN_READ_MODE_NOT_SET,
	_SPI_NAND_TEST_RTN_PATTER_TEST_ERROR,

	_SPI_NAND_TEST_RTN_DEF_NO

} _SPI_NAND_TEST_RTN_T;

unsigned char SPI_NAND_TEST_PATTERN[]=
{
    {0xA5},
    {0x5A},
    {0xFF},
    {0x00},
    {0x55},
    {0xAA}
};



typedef struct{
	char			*ptr_parameter_name;
	unsigned long 	default_value;
	unsigned long	current_value;
	
} _spi_nand_rwtest_parameter_value_t;

typedef struct{
  unsigned int  Round;
  unsigned int  ManualRead_Mode;
  unsigned int  ManualWrite_Mode;
  unsigned int  CLK_Rate;
  unsigned int  Pattern;
  unsigned long StarAddr;	
} _spi_nand_rwtest_input_parameter_t;

static _spi_nand_rwtest_input_parameter_t input_parameter_t;

static _spi_nand_rwtest_parameter_value_t _spi_nand_rwtest_info_t[]={
		{ "Round",				10,			10},
		{ "ManualRead_Mode",	0x1,		0x1},
		{ "ManualWrite_Mode",	0x1,		0x1},
		{ "CLK_Rate",			0,			0},
		{ "Pattern",			0,			0},
		{ "StarAddr",			0x7F00000,	0x7F00000},
};

static void SPI_NAND_RWTest_Helper()
{
	printf("\nusage: spinand_rwtest[Round] [ManualRead_Mode] [ManualWrite_Mode] [CLK_Rate]\n");
	printf("                     [Pattern] [StartAddr]\n");
	printf("[Round]:            0~65535 means test round.   DEFAULT VALUE is 10\n");
	printf("[ManualRead_Mode]:  To setup the ManualRead_Mode Test Enable or not\n");
	printf("                    0~7 is b'000~111, from bit0 to bit2\n");
	printf("                    set bit0 to enable Manual_singleRead\n");
	printf("                    set bit1 to enable Manual_DualRead\n");
	printf("                    set bit2 to enable Manual_QualRead\n");
	printf("                    Default is 0x1\n");
	printf("[ManualWrite_Mode]: To setup the ManualWrite_Mode Test Enable or not\n");
	printf("                    0~3 is b'00~11, from bit0 to bit1\n");
	printf("                    set bit0 to enable Manual_singleWrite\n");
	printf("                    set bit1 to enable Manual_QualWrite\n");	
	printf("                    Default is 0x1\n");
#if 1	/* ASIC */	
	printf("[CLK Rate]:         0 means auto switch clock test\n");	
	printf("                    1 ~ 31 means (250/CLK Rate)MHZ\n");
	printf("                    DEFAULT VALUE is 0\n");
#else	/* FPGA */
printf("[CLK Rate]: aoto test 25Mhz and 50Mhz\n");
#endif
	printf("[TestPattern]:      To setup test pattern\n");
	printf("                    0: A5, 1: 5A, 2: FF, 3: 00, 4: 55, 5: AA\n");
	printf("                    6: Random, 7: All\n");
	printf("                    DEFAULT VALUE is 0\n");
	printf("[StartAddr]:        To setup the test start address\n");
	printf("                    (Test Till to the end of Flash)\n");
	printf("                    DEFAULT VALUE is (chip size - 1MB)\n");
}


int spinand_rw_test_input_parser(int argc, char *argv[])
{
	unsigned int  					idx;
	struct SPI_NAND_FLASH_INFO_T	flash_info_t;

	if(argc < 2 ){
		return -1;
	}	
	

	SPI_NAND_Flash_Get_Flash_Info(&flash_info_t);
	input_parameter_t.StarAddr			= (flash_info_t.device_size) - 0x100000;
	

	for( idx=2 ; idx<=argc ; idx++)
	{
		if(idx == 7)			
		{
			/* The input "address" is HEX format */
			_spi_nand_rwtest_info_t[idx-2].current_value = simple_strtoul(argv[idx-1], NULL, 16);
		}
		else
		{
			_spi_nand_rwtest_info_t[idx-2].current_value = simple_strtoul(argv[idx-1], NULL, 10);
		}
	}

	printf("The Test Parameter Setting\n");
	printf("Parameter_Name     Default_Value     Current_Value\n");
	printf("---------------------------------------------\n");
	for( idx=0 ; idx< (sizeof(_spi_nand_rwtest_info_t)/sizeof(_spi_nand_rwtest_parameter_value_t)) ; idx++)
	{
		printf("%-18s, 0x%-18x, 0x%-18x\n", _spi_nand_rwtest_info_t[idx].ptr_parameter_name, _spi_nand_rwtest_info_t[idx].default_value, _spi_nand_rwtest_info_t[idx].current_value);
	}
	input_parameter_t.Round 			= _spi_nand_rwtest_info_t[0].current_value;
	input_parameter_t.ManualRead_Mode	= _spi_nand_rwtest_info_t[1].current_value;
	input_parameter_t.ManualWrite_Mode	= _spi_nand_rwtest_info_t[2].current_value;
	input_parameter_t.CLK_Rate			= _spi_nand_rwtest_info_t[3].current_value;
	input_parameter_t.Pattern			= _spi_nand_rwtest_info_t[4].current_value;
	input_parameter_t.StarAddr			= _spi_nand_rwtest_info_t[5].current_value;

	return 0;
	
}

static int spinand_write_speed_check( SPI_NAND_FLASH_WRITE_SPEED_MODE_T speed_mode )
{
	_SPI_NAND_TEST_RTN_T	rtn_status = _SPI_NAND_TEST_RTN_NO_ERROR;

	if( ((input_parameter_t.ManualWrite_Mode) &  (1 << speed_mode)) )
	{
		if( speed_mode == SPI_NAND_FLASH_WRITE_SPEED_MODE_SINGLE ) 
		{
			printf("\nSPI_NAND_RW_TEST: WRITE SPEED is SINGLE MODE\n");
		}
		else if( speed_mode == SPI_NAND_FLASH_WRITE_SPEED_MODE_QUAD ) 
		{
			printf("\nSPI_NAND_RW_TEST: WRITE SPEED is QUAD MODE\n");
		}
		else
		{
			printf("\nSPI_NAND_RW_TEST: WRITE SPEED UNKNOWN\n");
			rtn_status = _SPI_NAND_TEST_RTN_WRITE_MODE_CHECK_ERROR;
		}	
	}
	else
	{
		printf("\nSPI_NAND_RW_TEST: NO SUCH WRITE SPEED\n");
		rtn_status = _SPI_NAND_TEST_RTN_WRITE_MODE_NOT_SET;
	}

	return (rtn_status);	
}

static int spinand_read_speed_check( SPI_NAND_FLASH_READ_SPEED_MODE_T speed_mode )
{
	_SPI_NAND_TEST_RTN_T	rtn_status = _SPI_NAND_TEST_RTN_NO_ERROR;

	if( ((input_parameter_t.ManualRead_Mode) &  (1 << speed_mode)) )
	{
		if( speed_mode == SPI_NAND_FLASH_READ_SPEED_MODE_SINGLE ) 
		{
			printf("SPI_NAND_RW_TEST: READ SPEED is SINGLE MODE\n");
		}
		else if( speed_mode == SPI_NAND_FLASH_READ_SPEED_MODE_DUAL ) 
		{
			printf("SPI_NAND_RW_TEST: READ SPEED is DUAL MODE\n");
		}		
		else if( speed_mode == SPI_NAND_FLASH_READ_SPEED_MODE_QUAD ) 
		{
			printf("SPI_NAND_RW_TEST: READ SPEED is QUAD MODE\n");
		}
		else
		{
			printf("SPI_NAND_RW_TEST: READ SPEED UNKNOWN\n");
			rtn_status = _SPI_NAND_TEST_RTN_READ_MODE_CHECK_ERROR;
		}	
	}
	else
	{
		rtn_status = _SPI_NAND_TEST_RTN_READ_MODE_NOT_SET;
	}

	return (rtn_status);	
}




static int spinand_rw_test_with_pattern( unsigned char test_pattern )
{

	SPI_NAND_FLASH_WRITE_SPEED_MODE_T	write_speed;
	SPI_NAND_FLASH_READ_SPEED_MODE_T	read_speed;
	struct SPI_NAND_FLASH_INFO_T		flash_info_t;
	unsigned long						test_addr;
	unsigned long						write_return_len=0;
	unsigned long						read_return_len=0;
	unsigned char						test_pattern_write_buf[SPI_NAND_CHECK_PATTERN_SIZE];
	unsigned char						test_pattern_read_buf[SPI_NAND_CHECK_PATTERN_SIZE];
	_SPI_NAND_TEST_RTN_T				rtn_status = _SPI_NAND_TEST_RTN_PATTER_TEST_ERROR;
	SPI_NAND_FLASH_RTN_T				flash_rtn_status;


	printf("SPI_NAND_RW_TEST: pattern=0x%x\n", test_pattern);


	/* 1. Get Flash Information */
	SPI_NAND_Flash_Get_Flash_Info(&flash_info_t);

	//SPI_NAND_DEBUG_ENABLE();

	/* 2. Preparre Test pattern buffer */
	memset( test_pattern_write_buf, test_pattern, SPI_NAND_CHECK_PATTERN_SIZE);	
						
	for( write_speed= SPI_NAND_FLASH_WRITE_SPEED_MODE_SINGLE ; write_speed<SPI_NAND_FLASH_WRITE_SPEED_MODE_DEF_NO ; write_speed++ )
	{		
		if( spinand_write_speed_check(write_speed)!=  _SPI_NAND_TEST_RTN_NO_ERROR)			
		{
			continue;		
		}
		else
		{				
			for( test_addr = input_parameter_t.StarAddr; (test_addr + (flash_info_t.erase_size)) < flash_info_t.device_size ; test_addr += (flash_info_t.erase_size))
			{		
				printf("SPI_NAND_RW_TEST: test_addr=0x%x\n", test_addr);
				/* 1. Ersa current test block */	
				flash_rtn_status = SPI_NAND_Flash_Erase( test_addr, (flash_info_t.erase_size));
				if(  flash_rtn_status == SPI_NAND_FLASH_RTN_NO_ERROR)
				{
					/* 2. Write Test Pattern */
					flash_rtn_status = SPI_NAND_Flash_Write_Nbyte( test_addr, SPI_NAND_CHECK_PATTERN_SIZE, &write_return_len, &test_pattern_write_buf, write_speed);
					if(  flash_rtn_status == SPI_NAND_FLASH_RTN_NO_ERROR )
					{					
						/* 3. Read Check (For the read mode if user enable) */
						for( read_speed =SPI_NAND_FLASH_READ_SPEED_MODE_SINGLE ; read_speed<SPI_NAND_FLASH_READ_SPEED_MODE_DEF_NO ; read_speed++ )
						{
							SPI_NAND_Flash_Clear_Read_Cache_Data();
							memset( test_pattern_read_buf, 0x0, SPI_NAND_CHECK_PATTERN_SIZE);	
							
							if( spinand_read_speed_check(read_speed) !=  _SPI_NAND_TEST_RTN_NO_ERROR)
							{
								continue;
							}
							else
							{
								//pause(1000);
								//SPI_NAND_DEBUG_ENABLE();
								flash_rtn_status = SPI_NAND_Flash_Read_NByte(test_addr, SPI_NAND_CHECK_PATTERN_SIZE, &read_return_len, test_pattern_read_buf, read_speed );
								if( flash_rtn_status == SPI_NAND_FLASH_RTN_NO_ERROR )
								{																	
									if( memcmp(test_pattern_write_buf, test_pattern_read_buf , (SPI_NAND_CHECK_PATTERN_SIZE) ) !=0 )
									{
										printf("spinand_rw_test_with_pattern: test pattern compare error at addr =0x%x\n\n", test_addr );
										goto spinand_rw_pattern_test_fail;
									}									
								}
								else if (flash_rtn_status == SPI_NAND_FLASH_RTN_DETECTED_BAD_BLOCK)
								{															
										goto next_round;									
								}								
								else
								{
									printf("spinand_rw_test_with_pattern: read fail at addr =0x%x\n\n", test_addr);
									goto spinand_rw_pattern_test_fail;										
								}
								//SPI_NAND_DEBUG_DISABLE();								
							}
							
						}								
					}
					else
					{
						printf("spinand_rw_test_with_pattern: write pattern fail at addr =0x%x, with test pattern0x%x\n\n", test_addr, test_pattern );
						goto next_round;						
					}
					
				}
				else
				{
					printf("spinand_rw_test_with_pattern: erase fail at addr =0x%x, SKIP\n\n", test_addr );
					goto next_round;
					
				}		

next_round: 
				if (flash_rtn_status == SPI_NAND_FLASH_RTN_DETECTED_BAD_BLOCK)
				{
					printf("spinand_rw_test_with_pattern : BAD Block Detected , SKIP\n");
				}
				if (flash_rtn_status == SPI_NAND_FLASH_RTN_ERASE_FAIL)
				{
					printf("spinand_rw_test_with_pattern : Erase Fail Detected , SKIP\n");
				}
				if (flash_rtn_status == SPI_NAND_FLASH_RTN_PROGRAM_FAIL)
				{
					printf("spinand_rw_test_with_pattern : Program Fail Detected , SKIP\n");
				}				
			
				
			}
			
		}
	}

	rtn_status = _SPI_NAND_TEST_RTN_NO_ERROR;


	//SPI_NAND_DEBUG_DISABLE();

spinand_rw_pattern_test_fail:
	
	return (rtn_status);	
	
}

int do_spinand_rw_test(int argc, char *argv[])
{    
	unsigned long 					test_round;
	unsigned long 					idx;
	unsigned char					test_pattern;
	unsigned char					incemental_test_pattern=0x0;
	u32 							begtime=0, endtime=0, passtime=0, totaltime=0;
	struct SPI_NAND_FLASH_INFO_T	flash_info_t;
	SPI_NAND_FLASH_RTN_T			rtn_status;


	//SPI_NAND_DEBUG_ENABLE();

	/* 1. Parser User Input and setup Parameter */
	if( spinand_rw_test_input_parser(argc, argv) != 0 )
	{
		printf("Input Error, the input format is: \n");
		SPI_NAND_RWTest_Helper();
	}
	else
	{			
		/* 2.1 Get Current Flash Information */
		SPI_NAND_Flash_Get_Flash_Info(&flash_info_t);
		printf("Flash Info: \nChip Name=%s \nchip size=0x%x \nerase size=0x%x\n\n", flash_info_t.ptr_name, flash_info_t.device_size, flash_info_t.erase_size);
	

		/* 2.3 Test each round */
		for( test_round=1 ; test_round<= (input_parameter_t.Round) ; test_round++)
		{	
			begtime = getTime();			/* Caculate time start */
			printf("================================================================\n");
			printf("SPI_NAND_RW_TEST: ROUND %d start\n", test_round);


#if 1  /*ASIC */
		if( (input_parameter_t.CLK_Rate) == 0 ) /* Aoto test for multiple spi clock frequency */
		{

			if( (test_round %6) ==0)
			{
				VPint(0xBFA10098)=0;
				printf("CPOL register =0x%x\n", VPint(0xBFA10098)); 		
			
				set_spi_clock_speed(0xa);

				/* adjust the delay parameter */
				
				VPint(0xbfa1009c) = 0x9;
				printf("0xbfa1009c register =0x%x\n", VPint(0xbfa1009c));
			}
			if( (test_round %6) ==1)
			{
				VPint(0xBFA10098)=1;
				printf("CPOL register =0x%x\n", VPint(0xBFA10098));
			
				set_spi_clock_speed(0xa);

				/*  adjust the delay parameter */
				VPint(0xbfa1009c) = 0x9;			
				printf("0xbfa1009c register =0x%x\n", VPint(0xbfa1009c));
			}

			
			if( (test_round %6) ==2)
			{
				VPint(0xBFA10098)=0;
				printf("CPOL register =0x%x\n", VPint(0xBFA10098));
			
				set_spi_clock_speed(0x5);

				/* adjust the delay parameter */
				VPint(0xbfa1009c) = 0x9;			
				printf("0xbfa1009c register =0x%x\n", VPint(0xbfa1009c));
			}
			if( (test_round %6) ==3)
			{
				VPint(0xBFA10098)=1;
				printf("CPOL register =0x%x\n", VPint(0xBFA10098));
			
				set_spi_clock_speed(0x5);	

				/*adjust the delay parameter */
				VPint(0xbfa1009c) = 0x9;						
				printf("0xbfa1009c register =0x%x\n", VPint(0xbfa1009c));
			}	

			if( (test_round %6) ==4)
			{
				VPint(0xBFA10098)=0;
				printf("CPOL register =0x%x\n", VPint(0xBFA10098)); 		

				set_spi_clock_speed(0x3);

				/* adjust the delay parameter */
				
				VPint(0xbfa1009c) = 0xa;
				printf("0xbfa1009c register =0x%x\n", VPint(0xbfa1009c));
			}
			if( (test_round %6) ==5)
			{
				VPint(0xBFA10098)=1;
				printf("CPOL register =0x%x\n", VPint(0xBFA10098));

				set_spi_clock_speed(0x3);

				
				/* adjust the delay parameter */
				VPint(0xbfa1009c) = 0xa;			
				printf("0xbfa1009c register =0x%x\n", VPint(0xbfa1009c));
			}
		}
		else 	/* Setting SPI clock frequency base on user intput */
		{
			set_spi_clock_speed((input_parameter_t.CLK_Rate));
		}			

#else   /*FPGA */
			if( (test_round %4) ==0)
			{
				VPint(0xBFA10098)=0;
				printf("CPOL register =0x%x\n", VPint(0xBFA10098)); 		
			
				VPint(0xBFA2FF00) &= 0xFFFFFFFE ;
				printf("Clock register =0x%x\n", VPint(0xBFA2FF00));
				/* The	50Mhz must adjust the delay parameter */
				
				VPint(0xbfa1009c) = 0xa;
				printf("0xbfa1009c register =0x%x\n", VPint(0xbfa1009c));
			}
			if( (test_round %4) ==1)
			{
				VPint(0xBFA10098)=1;
				printf("CPOL register =0x%x\n", VPint(0xBFA10098));
			
				VPint(0xBFA2FF00) &= 0xFFFFFFFE ;
				printf("Clock register =0x%x\n", VPint(0xBFA2FF00));
				/* The	50Mhz must adjust the delay parameter */
				VPint(0xbfa1009c) = 0xa;			
				printf("0xbfa1009c register =0x%x\n", VPint(0xbfa1009c));
			}
			if( (test_round %4) ==2)
			{
				VPint(0xBFA10098)=0;
				printf("CPOL register =0x%x\n", VPint(0xBFA10098));
			
				VPint(0xBFA2FF00) |= 0x1;
				printf("Clock register =0x%x\n", VPint(0xBFA2FF00));
				/* The	25Mhz must adjust the delay parameter */
				VPint(0xbfa1009c) = 0x9;			
				printf("0xbfa1009c register =0x%x\n", VPint(0xbfa1009c));
			}
			if( (test_round %4) ==3)
			{
				VPint(0xBFA10098)=1;
				printf("CPOL register =0x%x\n", VPint(0xBFA10098));
			
				VPint(0xBFA2FF00) |= 0x1;
				printf("Clock register =0x%x\n", VPint(0xBFA2FF00));
				/* The	25Mhz must adjust the delay parameter */
				VPint(0xbfa1009c) = 0x9;						
				printf("0xbfa1009c register =0x%x\n", VPint(0xbfa1009c));
			}	
#endif			

			printf("\n\n");



			
			for( idx=0 ; idx< 7 ; idx++)
			{
				if( (idx < 6) && ((input_parameter_t.Pattern == idx) || input_parameter_t.Pattern== 7) ) 		/* Normal Test Pattern */
				{
					test_pattern = SPI_NAND_TEST_PATTERN[idx];
					
					if( spinand_rw_test_with_pattern( test_pattern ) != _SPI_NAND_TEST_RTN_NO_ERROR)
					{
						printf("do_spinand_rw_test: fail at round %d, with pattern=0x%x\n\n", test_round, test_pattern);
						return 0;
					}									
				}
				if( (idx == 6) && ( input_parameter_t.Pattern == 6 || input_parameter_t.Pattern== 7) )			/* Random Test Pattern */
				{
					test_pattern = random_byte() & 0xff;
					
					if( spinand_rw_test_with_pattern( test_pattern ) != _SPI_NAND_TEST_RTN_NO_ERROR)
					{
						printf("do_spinand_rw_test: fail at round %d, with random pattern=0x%x\n\n", test_round, test_pattern);
						return 0;
					}				
					
				}
			}
			printf("SPI NAND RW_TEST: ROUND %d finish\n", test_round);
			printf("================================================================\n");				
			endtime = getTime();
			if(endtime < begtime)
				passtime = 0xffffffff - begtime + endtime;
			else
				passtime = endtime - begtime;

			totaltime+=passtime;
			printf("\n>>>>>>> Round %d Test Finish::passtime is %lums", test_round, passtime);
			printf("\nCurrent Time is %d:%d:%d\n", endtime/3600000, (endtime/60000)%60, (endtime/1000)%60);			

		}

		printf("All Round test time is %lums\n", totaltime);
			
	}		

	//SPI_NAND_DEBUG_DISABLE();
	
	return 0;
			
}


static int do_spinand_read(int argc, char *argv[])
{

	unsigned long						idx;
	unsigned long						input_addr;
	unsigned long						input_len;
	unsigned long						input_speed;
	unsigned long						read_return_len;
	unsigned char						buf[4096];
	u32 								begtime=0, endtime=0, passtime=0;
	SPI_NAND_FLASH_READ_SPEED_MODE_T	read_mode = SPI_NAND_FLASH_READ_SPEED_MODE_SINGLE;

	if(argc < 2 ){
		return -1;
	}

	input_addr 	= simple_strtoul(argv[1], NULL, 16);	
	input_len  	= simple_strtoul(argv[2], NULL, 16);
	read_mode	= simple_strtoul(argv[3], NULL, 10);

	printf("\n Read SPI NAND From 0x%x, len 0x%x, read_mode=0x%x\n", input_addr, input_len, read_mode);
	memset(buf, 0x0 ,4096);	

	SPI_NAND_DEBUG_ENABLE();

	begtime = getTime();			/* Caculate time start */

	SPI_NAND_Flash_Clear_Read_Cache_Data();

	SPI_NAND_Flash_Read_NByte( input_addr, input_len, &read_return_len, buf, read_mode);
	
	endtime = getTime();

	
	if(endtime < begtime)
		passtime = 0xffffffff - begtime + endtime;
	else
		passtime = endtime - begtime;
	
	printf("\n>>>>>>> Read Test Finish::passtime is %lums\n", passtime);
	

	SPI_NAND_DEBUG_DISABLE();

#if 1
	for(idx=0; idx< input_len; idx++)
	{
		if( ((idx) %8 == 0) )
		{
			printf("\n%d: ", (idx));
		}
		printf("0x%x ", buf[idx]);		
	}
#endif	

    printf("\n\n");

	return 0;
	
}


static int do_spinand_write(int argc, char *argv[])
{
	unsigned long						idx;
	unsigned long						write_addr;
	unsigned long						write_len;
	unsigned long						write_retrun_len;
	unsigned char						buf[4096];
	int									rtn_status;
	SPI_NAND_FLASH_WRITE_SPEED_MODE_T	write_mode;

	if(argc < 2 ){
		return -1;
	}

	write_addr = simple_strtoul(argv[1], NULL, 16);	
	write_len  = simple_strtoul(argv[2], NULL, 16);
	write_mode = simple_strtoul(argv[3], NULL, 10);
	
	for(idx=0; idx< 4096; idx++)
	{
		buf[idx] = 0x55;
	}


	printf("\nSPI NAND Write to 0x%x, len 0x%x, speed=0x%x\n", write_addr, write_len, write_mode);

	SPI_NAND_Flash_Write_Nbyte( write_addr, write_len, &write_retrun_len, &buf, write_mode);


	return 0;

}


static int do_spinand_erase(int argc, char *argv[])
{
	unsigned long	erase_addr;
	unsigned long	erase_len;

	erase_addr = simple_strtoul(argv[1], NULL, 16);	
	erase_len  = simple_strtoul(argv[2], NULL, 16);

	printf("erase addr =0x%lx, erase_len=0x%lx\n", erase_addr, erase_len);

	SPI_NAND_Flash_Erase(erase_addr, erase_len);

	return 0;
	
}

#define SPI_NAND_ALIGN_TEST_START_ADDR		0x2000000
#define SPI_NAND_ALIGN_TEST_SIZE			0x801

static int do_spinand_nonalign(int argc, char *argv[])
{


	unsigned long 					addr, idx, write_return_len, read_return_len;
	unsigned char					write_buf[SPI_NAND_ALIGN_TEST_SIZE];
	unsigned char					read_buf[SPI_NAND_ALIGN_TEST_SIZE];
	struct SPI_NAND_FLASH_INFO_T	flash_info_t;


	/* 1. Get Flash Information */
	SPI_NAND_Flash_Get_Flash_Info(&flash_info_t);

	/* 2. Erase block */
	for( addr=SPI_NAND_ALIGN_TEST_START_ADDR ; addr < (SPI_NAND_ALIGN_TEST_START_ADDR+SPI_NAND_ALIGN_TEST_SIZE) ; addr+=flash_info_t.erase_size )
	{
		SPI_NAND_Flash_Erase(addr, (flash_info_t.erase_size) );
	}

	/* 3. Prepare write buffer */
	for( idx=0 ; idx<SPI_NAND_ALIGN_TEST_SIZE ; idx++)
	{
		write_buf[idx] = (unsigned char)idx;
	}

	/* 4. Write pattern */
	SPI_NAND_Flash_Write_Nbyte( SPI_NAND_ALIGN_TEST_START_ADDR, SPI_NAND_ALIGN_TEST_SIZE, &write_return_len, &write_buf, SPI_NAND_FLASH_WRITE_SPEED_MODE_SINGLE);

	pause(1000);

	/* 5. Read Pattern Check */
	SPI_NAND_Flash_Read_NByte(SPI_NAND_ALIGN_TEST_START_ADDR, SPI_NAND_ALIGN_TEST_SIZE, &read_return_len, read_buf, SPI_NAND_FLASH_READ_SPEED_MODE_SINGLE);
	

	/* 6. Check */
	for( idx=0 ; idx<SPI_NAND_ALIGN_TEST_SIZE; idx++ )
	{
		if( read_buf[idx] != write_buf[idx])	
		{
			printf("[Fail]SPI NAND ALIGN Test Fail\n");
			return 1;
		}
	}

	printf("[Success]SPI NAND ALIGN Test OK\n");
	return 0;
	
}


static int do_spicontroller_reg_test(int argc, char *argv[])
{
	u32		regs_test_pattern[]={ 0x55555555, 0xAAAAAAAA, 0xFFFFFFFF, 0x00000000};
	u32		idx;

	printf("================================================================\n");				
	printf("SPI CONTROLLER regsiter default test start\n");
	sf_regDef_test();
	printf("\nSPI CONTROLLER regsiter default test finish\n");
	printf("================================================================\n");				

	printf("\n\n================================================================\n");
	printf("SPI CONTROLLER regsiter r/w test start\n");
	for( idx=0 ; idx< (sizeof(regs_test_pattern)/sizeof(u32)) ; idx++)
	{
		sf_regRW_test( regs_test_pattern[idx] );
	}	

	printf("\nSPI CONTROLLER regsiter r/w test finish\n");
	printf("================================================================\n");
		
}


#define IMR1	0xBFB40050 

static int do_spicontroller_interrupt_test(int argc, char *argv[])
{

	unsigned long			value;
	int						result = 0;
	

	/* 1. Enable Manual Mode */
	SPI_CONTROLLER_Enable_Manual_Mode();

	
	printf("Check: The Interrupt EVENT register(0xBFA10090) is 0x%x\n", SF_INTERRUPT_VALUE );
	printf("Check: The Interrupt MASK  register(0xBFA10094) is 0x%x\n", SF_INTERRUPT_EN_VALUE );

	if( !(SF_INTERRUPT_VALUE &0x1) )		/* Interrupt doesn't triggered yet */
	{
		/* 2. Using auto read to get the first byte  */
		value =  *(volatile unsigned int *)(0xBFC00000);

		/* 3. Check the interuupt bit  */
		if( SF_INTERRUPT_VALUE & 0x1 )
		{
			printf("[Sucess]The Auto Mode/Manual Mode Conflict interrupt has been triggered !\n");
			SF_INTERRUPT_VALUE		|= 0x1;			/* Write 1 to clear the interrupt bit */
			SF_INTERRUPT_EN_VALUE	|= 0x1;			/* Enable the conflict interupt bit to ISR Sources*/
			VPint(IMR1) |=0x10;
			/* 4. Using auto read to get the first byte  */
			value =  *(volatile unsigned int *)(0xBFC00000);
		}
		else
		{
			printf("[Fail]The Auto Mode/Manual Mode Conflict interrupt doesn't be triggered !\n");
		}		
	}
	
	
}

#define SPI_CONTROLLER_REGS_MTX_MODE_TOG		0xBFA10014
#define SPI_CONTROLLER_REGS_MTX_MAX_EN			0xBFA10020
#define SPI_CONTROLLER_REGS_CPOL				0xBFA10098
#define SPI_CONTROLLER_REGS_PLDCTL_RSTTIMER 	0xBFA100E8
#define SPI_CONTROLLER_REGS_PLDCTL_CFG0 		0xBFA100EC
#define SPI_CONTROLLER_REGS_PLDCTL_CFG1 		0xBFA100F0
#define SPI_CONTROLLER_REGS_PLDCTL_SWEN 		0xBFA100F4
#define SPI_CONTROLLER_REGS_PLDCTL_STS			0xBFA100FC
#define SPI_CONTROLLER_REGS_SW_CFGSPITYPE_VAL	0xBFA10104
#define SPI_CONTROLLER_REGS_SW_CFGSPITYPE_EN	0xBFA10108
#define SPI_CONTROLLER_REGS_SW_CFGNANDADDR_VAL	0xBFA1010C
#define SPI_CONTROLLER_REGS_SW_CFGNANDADDR_EN	0xBFA10110
#define SPI_CONTROLLER_REGS_SFC_STRAP			0xBFA10114



#define TIMERCTLR_REGS						0xBFBF0100
#define TIMER3LVR_REGS						0xBFBF012C


void spicontroller_preload_function_test( void )
{
	unsigned char	buf[100];
	unsigned long 	rtn_len;
	unsigned long 	addr;
	unsigned long 	idx;
	int				result=0;

	/* Read information */	
	SPI_NAND_Flash_Read_NByte( 0x1800, 16, &rtn_len, buf, SPI_NAND_FLASH_READ_SPEED_MODE_SINGLE );

	/* Write 1 to clear the preload counter */
	VPint(SPI_CONTROLLER_REGS_PLDCTL_STS) |= 0x1;

	/* Indicate the #3 page (count from 0) */
	VPint(SPI_CONTROLLER_REGS_PLDCTL_CFG1) = 0x00000313;

	/* Software to trigger preload executing once */
	VPint(SPI_CONTROLLER_REGS_PLDCTL_SWEN) |= 0x1;

	pause(1000);

	/* Check counter */
	if( VPint(SPI_CONTROLLER_REGS_PLDCTL_STS) & 0x1 )
	{
		printf("The preload has been triggered\n");

		/* Switch to Auto Mode */
		VPint(SPI_CONTROLLER_REGS_MTX_MODE_TOG) =0;
		VPint(SPI_CONTROLLER_REGS_MTX_MAX_EN) =0;
		pause(1000);
		
		/* Compare data */
		for( idx=0 ; idx<16 ; idx+=4 )
		{						
			if( (buf[idx] 	!=  ((VPint(0xBFC00000+idx)>>24) & 0xff)) ||
				(buf[idx+1] !=  ((VPint(0xBFC00000+idx)>>16) & 0xff)) || 
				(buf[idx+2] !=  ((VPint(0xBFC00000+idx)>> 8) & 0xff)) ||
				(buf[idx+3] !=  ((VPint(0xBFC00000+idx))  & 0xff)) )
			{
				printf("[Fail]: Preload content not match\n");
				break;
			}
		}
		if( idx == 16)
		{
			printf("[Success]: Preload content match\n");
			result =1;
		}
	}
	else
	{
		printf("[Fail]: Preload counter not be triggered\n");
	}

	/* Switch to Manual Mode */
	VPint(SPI_CONTROLLER_REGS_MTX_MODE_TOG) =9;
	VPint(SPI_CONTROLLER_REGS_MTX_MAX_EN) =1;
	pause(1000);
	

	/*  Show the Test Result */	
	if(result == 1)
	{
		printf("[Success] SPI CONTROLLER preload function test\n");
	}
	else
	{
		printf("[Fail] SPI CONTROLLER preload function test\n");
	}
	
	
}

void spicontroller_preload_reset_test( unsigned int reset_mode )
{

	/******** Load first page into the cache of SPI NAND chip ********/
	/* Indicate the #0 page */
	VPint(SPI_CONTROLLER_REGS_PLDCTL_CFG1) = 0x00000013;

	pause(1000);

	/* Software to trigger preload executing once */
	VPint(SPI_CONTROLLER_REGS_PLDCTL_SWEN) |= 0x1;

	pause(1000);
	
	/********  Set Test Value ********/
	VPint( SPI_CONTROLLER_REGS_PLDCTL_RSTTIMER )	= 0x123456; 
	VPint( SPI_CONTROLLER_REGS_PLDCTL_CFG0 )		= 0x1234;
	VPint( SPI_CONTROLLER_REGS_PLDCTL_CFG1 )		= 0x12345678;

	pause(1000);

	if( reset_mode == 0 )		/* Software Reset */
	{
		/********  Triggered SW Reset  ********/
		VPint(0xBFB00040) = 0x80000000;	
	}
	else if( reset_mode == 1 )	/* Watchdog Reset */
	{
		VPint(TIMER3LVR_REGS) = 0x1000;
		VPint(TIMERCTLR_REGS) = 0x2000020;
	}
	else
	{
	}

	
}

void spicontroller_preload_reset_check( void )
{

	if( (VPint(SPI_CONTROLLER_REGS_PLDCTL_RSTTIMER) == 0x123456) &&
		(VPint(SPI_CONTROLLER_REGS_PLDCTL_CFG0) 	== 0x1234) &&
		(VPint(SPI_CONTROLLER_REGS_PLDCTL_CFG1) 	== 0x12345678)  ) 
	{
		printf("[Success] Preload parameter check\n");
	}
	else
	{
		printf("[Fail] Preload parameter check\n");
	}
}

static int do_spicontroller_preload_test(int argc, char *argv[])
{
	//printf("input=%s\n", argv[1]);

	if( strcmp( argv[1], "function") == 0 )
	{
		printf("SPI CONTROLLER , preload function test\n");
		spicontroller_preload_function_test();		
	}
	else if( strcmp( argv[1], "sw_reset") == 0 )
	{
		printf("SPI CONTROLLER , software reset test\n");
		spicontroller_preload_reset_test(0);		
	}
	else if( strcmp( argv[1], "watchdog_reset") == 0  )
	{
		printf("SPI CONTROLLER , watchdog reset test\n");
		spicontroller_preload_reset_test(1);		
	}
	else if( strcmp( argv[1], "reset_check") == 0 )
	{
		printf("SPI CONTROLLER , reset parameter check test\n");
		spicontroller_preload_reset_check();		
	}
	else
	{
		printf("spicontroller_preload_test [ function | sw_reset | watchdog_reset | reset_check]\n");
	}

	return 0;
}


static int do_spicontroller_cpol_test(int argc, char *argv[])
{

	unsigned long 	mode, idx, rtn_len;
	unsigned char	buf[16], buf2[16];

	
	mode  = simple_strtoul(argv[1], NULL, 10);	
	printf("The input CPOL mode is 0x%x\n", mode);
	
	/* Read information */	
	SPI_NAND_Flash_Read_NByte( 0x1800, 16, &rtn_len, buf, SPI_NAND_FLASH_READ_SPEED_MODE_SINGLE );

	if(mode == 0)
	{
		VPint(SPI_CONTROLLER_REGS_CPOL) = 0;
	}
	else if( mode == 1)
	{
		VPint(SPI_CONTROLLER_REGS_CPOL) = 1;
	}
	else
	{
		printf("CPOL value is 0 or 1, please input correctly\n");
		return 0;
	}

	SPI_NAND_Flash_Read_NByte( 0x1800, 16, &rtn_len, buf2, SPI_NAND_FLASH_READ_SPEED_MODE_SINGLE );

	for( idx=0 ; idx<16 ; idx++ )
	{
		if(buf[idx] != buf2[idx])
		{
			printf("[Fail]: CPOL Teset\n");
			return 1;
		}
	}

	printf("[Success]: CPOL Test\n");
	

	return 0;
	
}


static int do_spicontroller_strap_test(int argc, char *argv[])
{

	if( VPint(SPI_CONTROLLER_REGS_SFC_STRAP) == 0x6)
	{
		printf("[Success]Check Hardware STRAP Setting, OK\n");

		/* Setup to Software control STRAP */
		VPint(SPI_CONTROLLER_REGS_SW_CFGSPITYPE_VAL)	= 0;
		VPint(SPI_CONTROLLER_REGS_SW_CFGSPITYPE_EN)	= 1;
		VPint(SPI_CONTROLLER_REGS_SW_CFGNANDADDR_VAL)	= 0;
		VPint(SPI_CONTROLLER_REGS_SW_CFGNANDADDR_EN)	= 1;

		pause(1000);
		printf("SPI_CONTROLLER_REGS_SFC_STRAP, value =0x%x\n", VPint(SPI_CONTROLLER_REGS_SFC_STRAP));

		if( VPint(SPI_CONTROLLER_REGS_SFC_STRAP) == 0x0 )
		{
			printf("[Success]Software Configure STRAP value, OK\n");

			pause(1000);
			VPint(0xBFB00040) = 0x80000000;  /* Triggered Software Reset */			
		}
		else
		{
			printf("[Fail]Software Configure STRAP value, Fail\n");
		}		
	}
	else
	{
		printf("[Fail]Check Hardware STRAP Setting, Fail\n");
	}


	return 0;	
	
}


static int do_spi_frequency(int argc, char *argv[])
{
	unsigned long addr=0xbfc00000;
	unsigned char sidly_idx, edge_idx;
	unsigned char buf[4]={0x0};
	unsigned long ret_len;
	unsigned long val=0;

	int idx;

	//SPI_NAND_DEBUG_ENABLE();
#if 0	

	flash_read(0x0, 4, &ret_len, buf);	

	for( idx=0; idx<4 ;idx++)
	{
		printf("buf[%d]=0x%x ", idx, buf[idx]);
	}
	printf("\n");	
#endif	
	
#if 1

	for( edge_idx=8 ; edge_idx<=0xd ; edge_idx++)
	{
		VPint(0xbfa1009c) = edge_idx;
		
		for( sidly_idx=0 ; sidly_idx<=3 ; sidly_idx++)
		{
				VPint(0xbfa10008) = sidly_idx;


				for(idx = SPI_NAND_FLASH_READ_SPEED_MODE_SINGLE ; idx< SPI_NAND_FLASH_READ_SPEED_MODE_DEF_NO ; idx++)
				{
				buf[0]=buf[1]=buf[2]=buf[3]=0;

				SPI_NAND_Flash_Clear_Read_Cache_Data();

					SPI_NAND_Flash_Read_NByte(0x0, 4, &ret_len, buf, SPI_NAND_FLASH_READ_SPEED_MODE_SINGLE);
					/* flash_read(0x0, 4, &ret_len, buf);	 */
				val = 0;
				val |= buf[3];
				val |= (buf[2]<<8);
				val |= (buf[1]<<16);
				val |= (buf[0]<<24);

					printf("%x Mode: \n", idx);
				
				printf("0xbfa1009c=0x%x, 0xbfa10008=0x%x\n", VPint(0xbfa1009c), VPint(0xbfa10008));
					printf("%X: %X \n", addr, val);
				}
		}
	}
#endif	
	
}


#endif
#endif

#if defined(TCSUPPORT_CT_BOOTLOADER_UPGRADE)
#ifndef TCSUPPORT_RESERVEAREA_EXTEND
int do_passwd(int argc, char *argv[])
{
	unsigned long retlen;
//	uint8* flash_addr = ((EEPROM_RA_OFFSET+RESERVED_BASE)|FLASH_BASE);
	uint8* flash_addr;
	unsigned long reserved_base = 0;
	int i ;
	int username_len=0;
	int passwd_len=0;

	if (IS_SPIFLASH){
		reserved_base = (mtd.size - 4 * mtd.erasesize);
	} else {
	#ifdef TCSUPPORT_BB_NAND
		reserved_base = (1<<ra.flash->chip_shift) - 4 * (1<<ra.flash->erase_shift);
	#endif	
	}
#if defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505)
	flash_addr = ((TEMP_RA_OFFSET+reserved_base)|flash_base);
#else
	flash_addr = ((EEPROM_RA_OFFSET+reserved_base)|flash_base);
#endif
	username_len = strlen((const char*)(argv[1]));
	passwd_len = strlen((const char*)(argv[2]));
	
	if((username_len>=USERNAME_PASSWD_LEN)||(passwd_len>=USERNAME_PASSWD_LEN))
	{
		prom_printf("Length of username and password should be less than 16!\n");
		return -1;
	}
	if(argc !=3)
	{
		prom_printf("command passwd only with 2 parameters!\n");
		return -1;
	}

//	prom_printf("do_passwd:username = %s\n", argv[1]);
//	prom_printf("do_passwd:passwd = %s\n", argv[2]);
	
	for(i=0; i<RESERVEAREA_ERASE_SIZE; i++)
		upload_buf[i] = READ_FLASH_BYTE(flash_addr + i);
#if defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505)
	for(i=0; i<username_len; i++)
	{
		upload_buf[CERM1_RA_SIZE+CERM2_RA_SIZE+CERM3_RA_SIZE+CERM4_RA_SIZE+i] 
			= ((const char*)(argv[1]))[i];
	}
	upload_buf[CERM1_RA_SIZE+CERM2_RA_SIZE+CERM3_RA_SIZE+CERM4_RA_SIZE+username_len] = '\0';

	for(i=0; i<passwd_len; i++)
	{	
		upload_buf[CERM1_RA_SIZE+CERM2_RA_SIZE+CERM3_RA_SIZE+CERM4_RA_SIZE+0x10+i] 
			= ((const char*)(argv[2]))[i];
	}	
	upload_buf[CERM1_RA_SIZE+CERM2_RA_SIZE+CERM3_RA_SIZE+CERM4_RA_SIZE+0x10+passwd_len] = '\0';


	flash_erase(TEMP_RA_OFFSET+reserved_base, RESERVEAREA_ERASE_SIZE);
	flash_write(TEMP_RA_OFFSET+reserved_base, RESERVEAREA_ERASE_SIZE, &retlen, (const unsigned char *) upload_buf);	
#else
	for(i=0; i<username_len; i++)
	{
		upload_buf[EEPROM_RA_SIZE+CERM1_RA_SIZE+CERM2_RA_SIZE+CERM3_RA_SIZE+CERM4_RA_SIZE+i] 
			= ((const char*)(argv[1]))[i];
	}
	upload_buf[EEPROM_RA_SIZE+CERM1_RA_SIZE+CERM2_RA_SIZE+CERM3_RA_SIZE+CERM4_RA_SIZE+username_len] = '\0';
	
	for(i=0; i<passwd_len; i++)
	{	
		upload_buf[EEPROM_RA_SIZE+CERM1_RA_SIZE+CERM2_RA_SIZE+CERM3_RA_SIZE+CERM4_RA_SIZE+0x10+i] 
			= ((const char*)(argv[2]))[i];
	}	
	upload_buf[EEPROM_RA_SIZE+CERM1_RA_SIZE+CERM2_RA_SIZE+CERM3_RA_SIZE+CERM4_RA_SIZE+0x10+passwd_len] = '\0';

	
	flash_erase(EEPROM_RA_OFFSET+reserved_base, RESERVEAREA_ERASE_SIZE);
	flash_write(EEPROM_RA_OFFSET+reserved_base, RESERVEAREA_ERASE_SIZE, &retlen, (const unsigned char *) upload_buf);		
#endif
	printf("Change username and password successfully!\n");

	return 0;
}
#endif
#endif

#ifdef NAND_TEST_CMD
int init_nand(int argc, char *argv[])
{
	printf("init_nand\n");
	nandflash_init(0);
	return 0;
}

int erase_nand(int argc, char *argv[])
{
	unsigned long dst;
	unsigned long len;

	printf("erase_nand\n");
	dst = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	len = strtoul((const char*)(argv[2]), (char **)NULL, 16);
	if(argc == 4){
		en_oob_erase = strtoul((const char*)(argv[3]), (char **)NULL, 16);
		/*en_oob_erase =1, donot find redirect block and force erase badblock flag and oob*/
	}
	nandflash_erase(dst, len);
	printf("\n");
}

int read_nand(int argc, char *argv[])
{
	int i;
	unsigned long src;
	unsigned long len;
	unsigned long retlen;
	char buf[4224];
	unsigned long dst= 0;

	printf("read_nand\n");
	src = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	len = strtoul((const char*)(argv[2]), (char **)NULL, 16);

	if(argv[3]){
		dst = strtoul((const char*)(argv[3]), (char **)NULL, 16);
	}

	if(dst){
		printf("read_nand to dst:%x\n", dst);
		nandflash_read(src, len, &retlen, dst);
	}
	else{
		nandflash_read(src, len, &retlen, buf);

		printf("read len : %lu\n", len);
		for (i=1; i<=len; i++) {
			if ((i%16 == 0) && (i > 0)) {
				printf("\n");
			}
			printf("%02x ", buf[i-1] & 0xff);
			
		}
	}

	printf("\nretlen : %d\n\n", retlen);
}

int innand(int argc, char *argv[])
{
	unsigned long dst;
	unsigned long src;
	unsigned long retlen;
	unsigned long len;
	char buffer[2112];
	char ddr_data;

	dst = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	src = strtoul((const char*)(argv[2]), (char **)NULL, 16);
	len = strtoul((const char*)(argv[3]), (char **)NULL, 16);

	printf("nand dst addr : %lx\n", dst);
	printf("src addr : %lx\n", src);
	printf("move %ld bytes\n", len);


	nandflash_write(dst, len, &retlen, (const unsigned char*)src);
}

int check_data(int argc, char *argv[])
{
	unsigned long nand_addr;
	unsigned long ddr_addr;
	unsigned long retlen;
	unsigned long len;
	unsigned long counter;
	char buffer[2112];
	char ddr_data;
	int check_result = 0;

	nand_addr = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	ddr_addr = strtoul((const char*)(argv[2]), (char **)NULL, 16);
	len = strtoul((const char*)(argv[3]), (char **)NULL, 16);

	printf("nand addr : %lx\n", nand_addr);
	printf("ddr addr : %lx\n", ddr_addr);
	printf("check %ld bytes\n", len);

	counter = 0;
	while (len > 0) {

		nandflash_read(nand_addr, 1, &retlen, buffer);
		ddr_data =*((char*)ddr_addr);

		if (buffer[0] != ddr_data) {
			printf("found error\n");
			printf("offset %d\n", counter);
			printf("nand : %2x\n", buffer[0] & 0xff);
			printf("ddr : %2x\n", ddr_data & 0xff);
			printf("\n\n");
			check_result++;
		}
		printf(".");
		nand_addr++;
		ddr_addr++;
		counter++;
		len --;

	}
	if(check_result){
		printf("check data fail,fail time:%d\n",check_result);
	}
	else{
		printf("check data pass\n");
	}
}
#ifdef TCSUPPORT_NAND_RT63368
int check_badblock(int argc, char *argv[])
{
    int i, totalblock;
    int addr = 0;
    int block_size = 1 << ra.flash->erase_shift;

    totalblock = strtoul((const char*)(argv[1]), (char **)NULL, 0);

    for(i = 0; i < totalblock; i++)
    {
        if(mt6573_nand_block_bad_hw(&ra, addr, 0))
        {
            printf("badblock:%d \n", addr/block_size);
        }

        addr += block_size;
    }

    return 0;
    
}

int mark_badblock(int argc, char *argv[])
{
    unsigned long block, addr;

    block = strtoul((const char*)(argv[1]), (char **)NULL, 0);
    addr = block << ra.flash->erase_shift;

    mt6573_nand_block_markbad_hw(&ra, addr, 0);

    return 0;
}

int erase_block_phy(int argc, char *argv[])
{
    unsigned long erase_num, erase_start_block, i, page;

    erase_start_block = strtoul((const char*)(argv[1]), (char **)NULL, 0);
    erase_num = strtoul((const char*)(argv[2]), (char **)NULL, 0);

    for(i = 0; i < erase_num; i++)
    {
        page = (erase_start_block << ra.flash->erase_shift) >> ra.flash->page_shift;
        mt6573_nand_erase_hw(&ra, page);
        erase_start_block++;
    }

    return 0;

}

int read_page_phy(int argc, char *argv[])
{
    unsigned long page, len, i;
    char buf[2112];

    page = strtoul((const char*)(argv[1]), (char **)NULL, 0);
    len = strtoul((const char*)(argv[2]), (char **)NULL, 0);

    mt6573_nand_exec_read_page(&ra, page, 2048, buf, buf + 2048);

    printf("read len: %u \n", len);

    for(i = 0; i < len; i++)
    {
        if(i % 16 == 0)
            printf("\n");

        printf("%02x ", buf[i] & 0xff);

    }

    printf("\n");

    return 0;

}



static int write_page_phy(int argc, char *argv[])
{
    unsigned long page, byteIndex, value, ecc_flag;
    char buf[2112];
	char temp = 0;

    page = strtoul((const char*)(argv[1]), (char **)NULL, 0);
    byteIndex = strtoul((const char*)(argv[2]), (char **)NULL, 0);
	value = strtoul((const char*)(argv[3]), (char **)NULL, 0);
	ecc_flag = strtoul((const char*)(argv[4]), (char **)NULL, 0);

    mt6573_nand_exec_read_page(&ra, page, 2048, buf, buf + 2048);

	temp = value;

	printf("\r\n page=%d, byteIndex=%d, value=%d, ecc_flag=%d\r\n", page, byteIndex, value, ecc_flag);

	buf[byteIndex] = temp;
	
	nfc_write_page(&ra, buf, page, (ecc_flag? FLAG_ECC_EN:0));	
    return 0;

}



#endif
#endif

#ifdef BOOTROM_BACKDOOR
#define BD_MAGIC_NUM	0x54321253
#define READ_BUF_BASE	0x80020000
#ifndef TCSUPPORT_CPU_MT7505
#define CONFIG_INFO_START	0xfef0
#else
#define CONFIG_INFO_START	0x30
#endif

typedef struct {
	int	magic_num;
	int ddr_config_length;
	int eth_config_length;
	int reserved[5];
} header_t;


int do_bdconfig_store(int argc,char * argv [ ])
{
	unsigned long dst;
	unsigned long src;
	unsigned long len;
	unsigned long retlen;
	int config_info[4];

	header_t *p_bd_header;

	dst = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	src = strtoul((const char*)(argv[2]), (char **)NULL, 16);
	//len = strtoul((const char*)(argv[3]), (char **)NULL, 16);

	if (IS_NANDFLASH) {
		printf("back door config is not support NAND Flash\n");
		return 0;
	}


	/*check magic num*/
	p_bd_header = (header_t *)src;

	if(p_bd_header->magic_num !=  BD_MAGIC_NUM){
		printf("Magic num is wrong: %x\n", p_bd_header->magic_num);
		return 0;
	}

	config_info[0] = dst; //ddr config start address
	config_info[1] = p_bd_header->ddr_config_length; //ddr config length
	config_info[2] = dst+p_bd_header->ddr_config_length; //ddr config start address
	config_info[3] = p_bd_header->eth_config_length; //ddr config start address

	printf("ddr start:%x, ddr len:%x, eth start:%x, eth len:%x\n", config_info[0], config_info[1], config_info[2], config_info[3]);


	src += sizeof(header_t);
	len = config_info[1] + config_info[3];

	printf("Write to flash from %X to %X with %X bytes\n", src, dst, len);
	flash_erase(dst, len);
	printf("\n");
	flash_write(dst, len, &retlen, (const unsigned char *) (src));

	/*write config into to flash*/
	flash_read(TCBOOT_BASE, (unsigned long)TCBOOT_SIZE, &retlen,
								(const unsigned char*)READ_BUF_BASE);

	memcpy(READ_BUF_BASE+CONFIG_INFO_START, (char *) (config_info), sizeof(config_info));

	flash_erase(TCBOOT_BASE, TCBOOT_SIZE);
	prom_printf("\n");
	flash_write(TCBOOT_BASE, (unsigned long)TCBOOT_SIZE, &retlen,
								(const unsigned char*)READ_BUF_BASE);

	return 0;

}

int do_bdconfig_show(int argc,char * argv [ ])
{
	unsigned long dst;
	unsigned long src;
	unsigned long len;
	unsigned long retlen;
	int config_info[4], i;
	char buf[16];

	header_t *p_bd_header;

	if (IS_NANDFLASH) {
		printf("back door config is not support NAND Flash\n");
		return 0;
	}

	#if 0
	dst = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	len = strtoul((const char*)(argv[2]), (char **)NULL, 16);

	if(dst){
		printf("\n");
		while(len){
			flash_read(dst, 4, &retlen, (const unsigned char *) (buf));
			printf(" %x \n", *(int*)buf);
			len -= 4;
			dst += 4;
		}
		printf("\n");
		return 0;
	}
	#endif

	/*read config from flash*/
	flash_read(CONFIG_INFO_START, sizeof(config_info), &retlen, (const unsigned char *) (config_info));

	printf("ddr start:%x, ddr len:%x, eth start:%x, eth len:%x\n", config_info[0], config_info[1], config_info[2], config_info[3]);

	if(config_info[0] == 0x0 || config_info[1] == 0xffffffff){
		printf("ddr back door config is none\n");
	}
	else{

		for(i=0; i<config_info[1]; i+=8){
			flash_read(config_info[0]+i, 8, &retlen, (const unsigned char *) (buf));
			printf("ddr %x=%x\n", *(int*)buf, *(int*)(buf+4));
			
		}
	}
	if(config_info[2] == 0x0 || config_info[3] == 0xffffffff){
		printf("ether back door config is none\n");
	}
	else{
		for(i=0; i<config_info[3]; i+=12){
			flash_read(config_info[2]+i, 12, &retlen, (const unsigned char *) (buf));
			printf("ether phy %x: %x=%x\n", *(int*)buf, *(int*)(buf+4), *(int*)(buf+8));
			
		}
	}

	return 0;

}

#ifdef BACKDOOR_SWITCH
#define BD_CFG_FLAG			0x2c
#define BD_SWITCH_NUM		0x5a3978bc
static int do_bdconfig_switch (int argc, char *argv[])
{
	unsigned long bdSwitchNew = 0;
	unsigned long bdVaule;
	unsigned long bdSwitchOld;
	unsigned long retlen;

#if !defined(TCSUPPORT_CPU_EN7512) && !defined(TCSUPPORT_CPU_EN7521)
	if (IS_NANDFLASH) {
		printf("back door config is not support NAND Flash\n");
		return 0;
	}
#endif	

	flash_read(BD_CFG_FLAG, 4, &retlen, (const unsigned char *) (&bdVaule));
	
	if(bdVaule == BD_SWITCH_NUM){
		bdSwitchOld = 1;//now
		bdVaule = 0;
	}
	else{
		bdSwitchOld = 0;//now
		bdVaule = BD_SWITCH_NUM;
	}
	printf("Current backdoor function is  %s\n", ((bdSwitchOld==1)? "enable":"disable"));
	
	if (argc >= 2)	{
		bdSwitchNew = strtoul((const char*)(argv[1]), (char **)NULL, 16);	
	
		if(bdSwitchNew!=bdSwitchOld){
		/*write config into to flash*/
			flash_read(TCBOOT_BASE, (unsigned long)TCBOOT_SIZE, &retlen,(const unsigned char*)READ_BUF_BASE);
			
			memcpy(READ_BUF_BASE+BD_CFG_FLAG, (char *) (&bdVaule), sizeof(&bdVaule));

			flash_erase(TCBOOT_BASE, TCBOOT_SIZE);
			prom_printf("\n");
			flash_write(TCBOOT_BASE, (unsigned long)TCBOOT_SIZE, &retlen,(const unsigned char*)READ_BUF_BASE);
			printf("Current backdoor function change to  %x %s\n", bdVaule, ((bdSwitchNew==1)? "enable":"disable"));
		}
		else
			printf("No Change\r\n");
	}
	
	
	return 0;

}

#define DDR_CAL_FLAG		0x28
#define DC_SWITCH_NUM		0xa59387cb
static int do_ddrcal_switch(int argc, char *argv[])
{
	unsigned long dcSwitchNew = 0;
	unsigned long dcValue;
	unsigned long dcSwitchOld;
	unsigned long retlen;

	if (IS_NANDFLASH) {
		printf("DDR caliration switch is not support NAND Flash\n");
		return 0;
	}

	flash_read(DDR_CAL_FLAG, 4, &retlen, (const unsigned char *) (&dcValue));
	
	if(dcValue == DC_SWITCH_NUM){
		dcSwitchOld = 0;//disable
		dcValue = 0;
	}
	else{
		dcSwitchOld = 1;//enable
		dcValue = DC_SWITCH_NUM;
	}
	printf("DDR caliration function is  %s\n", ((dcSwitchOld==1)? "enable":"disable"));
	
	if (argc >= 2)	{
		dcSwitchNew = strtoul((const char*)(argv[1]), (char **)NULL, 16);	
	
		if(dcSwitchNew!=dcSwitchOld){
		/*write config into to flash*/
			flash_read(TCBOOT_BASE, (unsigned long)TCBOOT_SIZE, &retlen,(const unsigned char*)READ_BUF_BASE);
			
			memcpy(READ_BUF_BASE+DDR_CAL_FLAG, (char *) (&dcValue), sizeof(&dcValue));

			flash_erase(TCBOOT_BASE, TCBOOT_SIZE);
			prom_printf("\n");
			flash_write(TCBOOT_BASE, (unsigned long)TCBOOT_SIZE, &retlen,(const unsigned char*)READ_BUF_BASE);
			printf("DDR caliration function change to  %x %s\n", dcValue, ((dcSwitchNew==1)? "enable":"disable"));
		}
		else
			printf("No Change\r\n");
	}
	
	return 0;

}

#define DB_CFG_FLAG		0x40
#define DB_QUICK_TEST	0xABCDEF01
#define BD_NORMAL_TEST	0xABCDEF02
static int do_dram_bist_switch(int argc, char *argv[])
{
	unsigned long dbSwitchNew = 0;
	unsigned long dbValue;
	unsigned long dbSwitchOld;
	unsigned long retlen;

	if (IS_NANDFLASH) {
		printf("DRAM BIST is not support NAND Flash\n");
		return 0;
	}

	flash_read(DB_CFG_FLAG, 4, &retlen, (const unsigned char *) (&dbValue));
	
	if(dbValue == DB_QUICK_TEST){
		dbSwitchOld = 1;//now
	}
	else if(dbValue == BD_NORMAL_TEST){
		dbSwitchOld = 2;//now
	}
	else{
		dbSwitchOld = 0;//now
	}
	printf("Current DRAM BIST function is  %s", ((dbSwitchOld == 0)? "disable":"enable"));
	if(dbSwitchOld)
		printf(", and Test mode is %s", ((dbSwitchOld == 1)? "quick":"normal"));
	printf("\r\n");
	
	if (argc >= 2)	{
		dbSwitchNew = strtoul((const char*)(argv[1]), (char **)NULL, 16);	
	
		if(dbSwitchNew != dbSwitchOld){
			if(dbSwitchNew == 1)
				dbSwitchNew = DB_QUICK_TEST;
			else if(dbSwitchNew == 2)
				dbSwitchNew = BD_NORMAL_TEST;
			else
				dbSwitchNew = 0;
		/*write config into to flash*/
			flash_read(TCBOOT_BASE, (unsigned long)TCBOOT_SIZE, &retlen,(const unsigned char*)READ_BUF_BASE);
			
			memcpy(READ_BUF_BASE+DB_CFG_FLAG, (char *) (&dbSwitchNew), sizeof(&dbSwitchNew));

			flash_erase(TCBOOT_BASE, TCBOOT_SIZE);
			prom_printf("\n");
			flash_write(TCBOOT_BASE, (unsigned long)TCBOOT_SIZE, &retlen,(const unsigned char*)READ_BUF_BASE);
			printf("DRAM BIST function change to  %s", ((dbSwitchNew == 0)? "disable":"enable"));
			if(dbSwitchNew)
				printf(", and Test mode is %s", ((dbSwitchNew == DB_QUICK_TEST)? "quick":"normal"));
			printf("\r\n");
		}
		else
			printf("No Change\r\n");
	}
	
	
	return 0;

}
#endif
#endif
#define isdigit(x)	((x)>='0'&&(x)<='9')
uint32
aton(
	char	*s
)
{
	uint32 n;
	int	i;
	n = 0;
	if(s == NULL)
		return 0;
	for(i=24;i>=0;i -= 8)
	{
		/* Skip any leading stuff (e.g., spaces, '[') */
		while(*s != '\0' && !isdigit(*s))
			s++;
		if(*s == '\0')
			break;
		n |= (strtoul((const char*)s, (char **)NULL, 10)) << i;
		if((s = strchr(s,'.')) == NULL)
			break;
		s++;
	}
	return n;
} /* aton */
#ifndef BOOTROM_IN_SRAM
extern void IP_change(uint32 ipaddr);
#if defined(TCSUPPORT_CT_BOOTLOADER_UPGRADE)
extern int authed;
#endif

int do_ipaddr(int argc, char *argv[]){
	uint32 ipaddr;
	ipaddr = aton(argv[1]);
	printf("Change IP address to %s\r\n", argv[1]);
	IP_change(ipaddr);
}
#endif

#ifdef BOOT_LZMA_SUPPORT
/*
 *
 */
static void my_err_tcp(void *arg,
		       err_t err) {
  char what_error[30];
  struct web_connection *web = (struct web_connection *)arg;


  sprintf( what_error, "my tcp error %d", err);
#ifdef HTTP_DBG  
  prom_printf( what_error);
#endif  

  if ( web->current_connection != NULL) {
    web->current_connection = NULL;
  }
}

/*
 * The TCP polling
 */
static err_t
my_poll_tcp(void *arg, struct tcp_pcb *tpcb) {
  if ( tpcb != NULL) {
    struct web_connection *web = (struct web_connection *)arg;
    if ( tpcb == web->current_connection) {
      /* see if there has been any traffic */
      if ( tpcb->rcv_nxt == web->last_rx_seqno) {
#ifdef HTTP_DBG		  
	prom_printf( "Closing idle connection");
#endif	
	/* close the connection */
	tcp_close( tpcb);
      }
      else {
	web->last_rx_seqno = tpcb->rcv_nxt;
      }
    }
    else {
#ifdef HTTP_DBG		
      prom_printf( "Closing unserved connection");
#endif	  
      /* close the connection */
      tcp_close( tpcb);
    }
  }
  return ERR_OK;
}

/*
 * The TCP sent callback.
 * The stack has received acknowledgement for some data it has sent.
 */
static err_t
my_sent_tcp( void *arg, struct tcp_pcb *tpcb,
	      u16_t len) {
#ifdef HTTP_DBG	
  prom_printf("my TCP sent\r\n");
#endif  
  return ERR_OK;
}

/*
 * The TCP receive callback.
 * This is where data is read off on the TCP stream.
 */
#if defined(TCSUPPORT_CT_BOOTLOADER_UPGRADE)
static err_t
my_recv_tcp( void *arg, struct tcp_pcb *tpcb,
	     struct pbuf *p, err_t err) {
  struct web_connection *web = (struct web_connection *)arg;
  unsigned long reserved_base = 0;
  char *start, *end;

  enum auth_index{ username=0, password,login,Upload, ending };
  char* auth_Attr[5]={"username", "passwd",  "login", "Upload\r\n","\r\n------"};
#ifdef TCSUPPORT_RESERVEAREA_EXTEND
  proline_Para proline;
  char user_name[CFEUSRNAMELEN] = {0};
  char pass_word[CFEPWDLEN] = {0};
#else
  char user_name[USERNAME_PASSWD_LEN] = {0};
  char pass_word[USERNAME_PASSWD_LEN] = {0};
#endif

//  char *flash_user_name_addr = (char *)FLASH_USER_NAME;
//  char *flash_pass_word_addr = (char *)FLASH_PASS_WORD;

  char *flash_user_name_addr;
  char *flash_pass_word_addr;
  
  int i=0;
  short filetype = 0;
  /* frankliao modify 20100810 */
  extern int checkfile(short, int, char *);

  if (IS_SPIFLASH){
#ifdef TCSUPPORT_RESERVEAREA_EXTEND
	reserved_base = (mtd.size - 7 * mtd.erasesize);
#else
  	reserved_base = (mtd.size - 4 * mtd.erasesize);
#endif
  } else {
  #ifdef TCSUPPORT_BB_NAND
  	reserved_base = (1<<ra.flash->chip_shift) - 4 * (1<<ra.flash->erase_shift);
  #endif	
  }
#ifdef TCSUPPORT_RESERVEAREA_EXTEND
  flash_user_name_addr = (char *)(((PROLINE_CWMPPARA_RA_OFFSET + reserved_base)|flash_base) + ((char *)&proline.cfeusrname[0] - (char *)&proline.flag));
  flash_pass_word_addr = flash_user_name_addr + CFEUSRNAMELEN;
#else
  flash_user_name_addr = (char *)((USERNAMEPASSWD_RA_OFFSET+reserved_base)|flash_base);
  flash_pass_word_addr = (char *)((USERNAMEPASSWD_RA_OFFSET+USERNAME_PASSWD_LEN+reserved_base)|flash_base);
#endif
#ifdef HTTP_DBG  
  prom_printf("my_recv_tcp........\r\n");
#endif  

  if ( p != NULL) 
  {
	if ( tpcb == web->current_connection) 
	{
		int copy_size = p->len;
		
#ifdef HTTP_DBG	  
		prom_printf("tpcb == web->current_connection\r\n");
#endif	  

		memcpy( web->rx_buffer, p->payload, copy_size);
		web->rx_buffer[copy_size] = '\0';
#ifdef HTTP_DBG	  
		prom_printf("web->rx_buffer!!!\r\n");				
#endif	

		if (strstr(web->rx_buffer, "POST")||(post_recv))
		{
			struct pbuf *ptemp=NULL;		
			post_recv = 1;
			prom_printf(".");

			for(ptemp=p; ptemp!=NULL; ptemp=ptemp->next)
			{
				copy_size = ptemp->len;
				
				memcpy( web->rx_buffer, ptemp->payload, copy_size);
				web->rx_buffer[copy_size] = '\0';

				memcpy(p_upload_buf, web->rx_buffer, copy_size);
				p_upload_buf += copy_size;
				i = 0;
			
				for (end = web->rx_buffer; memcmp(end, auth_Attr[login], strlen(auth_Attr[login])); ++end)
					if (++i >= copy_size)
						break;
									
				if (i < copy_size)
				{
					auth_recv = 1;
					post_recv = 0;
				}
				i = 0;
				for (end = web->rx_buffer; memcmp(end, auth_Attr[Upload], strlen(auth_Attr[Upload])); ++end)
					if(++i >= copy_size)
						break;
				if (i < copy_size)
				{	
					if(pass_auth)
					{
						file_recv = 1;
						post_recv = 0;
					}
					else
					{
						afterRefresh( p, tpcb);
						p_upload_buf = (unsigned char *)0x80020000;
					}
				}
			}
			 /* tell the stack we have received the data so it can advertise increase in the * receive window. */
			tcp_recved( tpcb, p->tot_len);
	 	}	 
		else
		{	
			/* process what we got */
	      	process_http( p, tpcb);
		}
		if (auth_recv)
		{
#ifdef TCSUPPORT_RESERVEAREA_EXTEND
			char UserName[CFEUSRNAMELEN] = {0};
			char PassWord[CFEPWDLEN] = {0};
			for(i=0; i<CFEUSRNAMELEN-1; i++)
			{	
				user_name[i] = READ_FLASH_BYTE(flash_user_name_addr+i);
			}
			user_name[i] = '\0';
			for(i=0; i<CFEPWDLEN-1; i++)
			{	
				pass_word[i] = READ_FLASH_BYTE(flash_pass_word_addr+i);
			}
			pass_word[i] = '\0';

			if((user_name[0] == 0 && user_name[1] == 0 && user_name[2] == 0)
				|| (user_name[0] == 0xff && user_name[1] == 0xff && user_name[2] == 0xff))
			{
				memset(user_name, 0, CFEUSRNAMELEN);
				snprintf(user_name,sizeof(user_name),"%s",DEFAULT_CFE_USERNAME);
			}
			if((pass_word[0] == 0 && pass_word[1] == 0 && pass_word[2] == 0)
				|| (pass_word[0] == 0xff && pass_word[1] == 0xff && pass_word[2] == 0xff))
			{
				memset(pass_word, 0, CFEPWDLEN);
				snprintf(pass_word,sizeof(pass_word),"%s",DEFAULT_CFE_PWD);
			}
			
#else
			char UserName[USERNAME_PASSWD_LEN] = {0};
			char PassWord[USERNAME_PASSWD_LEN] = {0};
			
			for(i=0; i<USERNAME_PASSWD_LEN-1; i++)
			{	
				user_name[i] = READ_FLASH_BYTE(flash_user_name_addr+i);
				pass_word[i] = READ_FLASH_BYTE(flash_pass_word_addr+i);
			}
			user_name[i] = '\0';	
			pass_word[i] = '\0';
#endif

#ifdef HTTP_DBG	  
			prom_printf("my_recv_tcp:---flash---user_name = %s\n", user_name);
			prom_printf("my_recv_tcp:---flash---pass_word = %s\n", pass_word);
#endif			

#if 1//ndef TCSUPPORT_RESERVEAREA_EXTEND
			if(strncmp(CT_USER_NAME, user_name, strlen(CT_USER_NAME)) != 0)
			{
				flash_user_name_addr = (char *)MRD_USER_NAME;	
				flash_pass_word_addr = (char *)MRD_PASS_WORD;	
				for(i=0; i<USERNAME_PASSWD_LEN-1; i++)
				{
					user_name[i] = READ_FLASH_BYTE(flash_user_name_addr+i);
					pass_word[i] = READ_FLASH_BYTE(flash_pass_word_addr+i);
				}
				user_name[i] = '\0';	
				pass_word[i] = '\0';				
			}
#endif
			
			start = strstr(upload_buf, "\"username\"\r\n\r\n");
			start += strlen("\"username\"\r\n\r\n");
			end = strstr(start,auth_Attr[ending]);
			strncpy(UserName, start, end-start);
			start = strstr(upload_buf, "\"passwd\"\r\n\r\n");
			start += strlen("\"passwd\"\r\n\r\n");
			end = strstr(start,auth_Attr[ending]);
			strncpy(PassWord, start, end-start);
			
#ifdef TCSUPPORT_RESERVEAREA_EXTEND
			if ((!strncmp(UserName, user_name, CFEUSRNAMELEN))&&(!strncmp(PassWord, pass_word, CFEPWDLEN)))
#else
			if ((!strncmp(UserName, user_name, USERNAME_PASSWD_LEN))&&(!strncmp(PassWord, pass_word, USERNAME_PASSWD_LEN)))
#endif
			{
				pass_auth = 1;
				afterAuthed(p, tpcb);
			}
			p_upload_buf = (unsigned char *)0x80020000;
			auth_recv = 0;
		}
#ifdef HTTP_DBG	
	  	for(i = copy_size - 20; i < copy_size; i++)
      			prom_printf("%c ", web->rx_buffer[i]);
	  		prom_printf("\r\n");			
#endif  
	 	 if(file_recv)
		 {
				unsigned long retlen;
#ifdef HTTP_DBG			
				prom_printf("\r\n=================================\r\n%s\r\n==============================\r\n", upload_buf);
#endif			
				start = strstr(upload_buf, "filename=");
				start += 10;

				if(strstr(start, TCBOOT_NAME))
        				filetype = TCBOOT;
				else if(strstr(start, TCLINUX_NAME))
        				filetype = TCLINUX;
    				else
        				filetype = ERRFILE;

				//start = strstr(upload_buf, "octet-stream");
 				start = strstr(start, "\r\n\r\n"); 
				start += 4;
				for (end = start; memcmp(end, auth_Attr[ending], strlen(auth_Attr[ending])); ++end)
				/* nothing */;
#ifdef HTTP_DBG			
				if(!end)
					prom_printf("2. end is NULL\r\n");
				prom_printf("start - upload_buf = %d\r\n", start - upload_buf);			
				for(i = 0; i < 10; i++)
					prom_printf("start[%d] = 0x%2X ", i, start[i]);
				for(i = end-start-10; i < end-start; i++)
					prom_printf("start[%d] = 0x%2X ", i, start[i]);
				prom_printf("\r\nsize = %d\r\n", end-start);
#endif		
			/* frankliao modify 20100810 */
			if(!checkfile(filetype, end-start, 0x80020000+start-upload_buf))
			{
				afterUpload(p, tpcb);
				if(filetype == TCBOOT)
				{
					flash_erase(0, end-start);
					flash_write(0, end-start, &retlen, (const unsigned char *) (0x80020000+start - upload_buf));
				}
				else if(filetype == TCLINUX)
				{
					flash_erase(flash_tclinux_start, end-start);
					flash_write(flash_tclinux_start, end-start, &retlen, (const unsigned char *) (0x80020000+start - upload_buf));
				}
				prom_printf("Firmware is uploaded successfully!\r\n");
				do_go(0,NULL);
			}
			else
				prom_printf("Fail to upload firmware!\r\n");

			p_upload_buf = (unsigned char *)0x80020000;
			file_recv = 0;
			pass_auth = 0;
	  }
    }
    else 
    {
      /* close the strange connection */
		prom_printf("tcp_close()\r\n");
	  	tcp_close( tpcb);
    }
    pbuf_free( p);
  }
  else if ( tpcb == web->current_connection) 
  {
    /* connection is closed */
	sprintf( print_buffer, "Connection from %ld.%ld.%ld.%ld is closed",
	IP_ADDRESS_FORMAT( tpcb->remote_ip.addr));
#ifdef HTTP_DBG	
	prom_printf( print_buffer);
#endif	
	web->current_connection = NULL;
	web->last_rx_seqno = 0;
  }

  return ERR_OK;
}
#else
static err_t
my_recv_tcp( void *arg, struct tcp_pcb *tpcb,
	     struct pbuf *p, err_t err) {
  struct web_connection *web = (struct web_connection *)arg;
  char *start, *end;
  char upload[] = {'U', 'p', 'l', 'o', 'a', 'd'};
  int i;
  short filetype = 0;
  int copy_size = 0;
  struct pbuf *q;
  static char count=0;

  /* frankliao modify 20100810 */
  extern int checkfile(short, int, char *);

#ifdef HTTP_DBG  
  prom_printf("my_recv_tcp........\r\n");
#endif  

  if ( p != NULL) {
    if ( tpcb == web->current_connection) {
#ifdef HTTP_DBG	  
	  prom_printf("tpcb == web->current_connection\r\n");
#endif	  

	if(p->len > UPLOAD_BUF_SIZE){
		prom_printf("packet length is bigger than 20000????\n");
	}
	q = p;
	for(copy_size=0; q!=NULL; q=q->next){
		memcpy( (web->rx_buffer + copy_size), q->payload, q->len);
        	copy_size += q->len;
        }

      //if ( copy_size >= 200)
	//copy_size = 199;

      //memcpy( web->rx_buffer, p->payload, copy_size);
      web->rx_buffer[copy_size] = '\0';
#ifdef HTTP_DBG	  
	  prom_printf("web->rx_buffer!!!\r\n");
#endif	  
	  if(strstr(web->rx_buffer, "POST")){
		  prom_printf("START TO RECEIVE the FILE\r\n");
//		  memset(upload_buf, 0, 4096);
		  file_recv = 1;
	  }
#ifdef HTTP_DBG	  
	  for(i = copy_size - 20; i < copy_size; i++)
      	prom_printf("%c ", web->rx_buffer[i]);
	  prom_printf("\r\n");
#endif	  
	  if(file_recv){
		  if((++count & 63) == 63)
		    prom_printf(".");
		  memcpy(p_upload_buf, web->rx_buffer, copy_size);
		  p_upload_buf += copy_size;
		  i = 0;
		  for (end = web->rx_buffer; memcmp(end, upload, 6); ++end)
			if(++i >= copy_size)
				break;
		  if(i < copy_size){
//		  if(strstr(web->rx_buffer, "Upload")){
//		    unsigned char *start, *end;
			unsigned char upload_end[] = {0x0d, 0x0a, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d};
			int i;
			unsigned long retlen;

		  	prom_printf("\r\nSTART TO CLOSE the FILE\r\n");
		  	file_recv = 0;
#ifdef HTTP_DBG			
			prom_printf("\r\n=================================\r\n%s\r\n==============================\r\n", upload_buf);
#endif			
			start = strstr(upload_buf, "filename=");
			start += 10;
			end = strchr(start, '"');

//			if(!memcmp(start, TCBOOT_NAME, end-start))
			if(strstr(start, TCBOOT_NAME))
        		filetype = TCBOOT;
//    		else if(!memcmp(start, TCLINUX_NAME, end-start))
			else if(strstr(start, TCLINUX_NAME))
        		filetype = TCLINUX;
    		else
        		filetype = ERRFILE;

			//solve firefox upgrade crash. shnwind
			//start = strstr(upload_buf, "octet-stream");
			start = strstr(start, "\r\n\r\n");
			start += 4;
			for (end = start; memcmp(end, upload_end, 8); ++end)
				/* nothing */;
#ifdef HTTP_DBG			
			if(!end)
				prom_printf("2. end is NULL\r\n");
			prom_printf("start - upload_buf = %d\r\n", start - upload_buf);			
			for(i = 0; i < 10; i++)
				prom_printf("start[%d] = 0x%2X ", i, start[i]);
			for(i = end-start-10; i < end-start; i++)
				prom_printf("start[%d] = 0x%2X ", i, start[i]);
			prom_printf("\r\nsize = %d\r\n", end-start);
#endif		
			/* frankliao modify 20100810 */
			if(!checkfile(filetype, end-start, 0x80020000+start-upload_buf)){
				if(filetype == TCBOOT){
					flash_erase(0, end-start);
					flash_write(0, end-start, &retlen, (const unsigned char *) (0x80020000+start - upload_buf));
				}
				else if(filetype == TCLINUX){
					flash_erase(flash_tclinux_start, end-start);
					flash_write(flash_tclinux_start, end-start, &retlen, (const unsigned char *) (0x80020000+start - upload_buf));
				}
				prom_printf("Firmware is uploaded successfully!\r\n");
			}
			else
				prom_printf("Fail to upload firmware!\r\n");
			afterUpload(p, tpcb);
			p_upload_buf = (unsigned char *)0x80020000;
	  	  }
	  }

      /* tell the stack we have received the data so it can advertise increase in the
       * receive window. */
      tcp_recved( tpcb, p->tot_len);

      /* process what we got */
      process_http( p, tpcb);
    }
    else {
      /* close the strange connection */
		prom_printf("tcp_close()\r\n");
      tcp_close( tpcb);
    }
    pbuf_free( p);
  }
  else if ( tpcb == web->current_connection) {
    /* connection is closed */
    sprintf( print_buffer, "Connection from %ld.%ld.%ld.%ld is closed",
	     IP_ADDRESS_FORMAT( tpcb->remote_ip.addr));
#ifdef HTTP_DBG	
    prom_printf( print_buffer);
#endif	
    web->current_connection = NULL;
    web->last_rx_seqno = 0;
  }

  return ERR_OK;
}

#endif

static err_t web_accept(void *arg, struct tcp_pcb *newpcb,
			err_t err) {
  struct web_connection *web = (struct web_connection *)arg;
  err_t accept_err = ERR_ABRT;

#ifdef HTTP_DBG  
  prom_printf("web_accept\r\n");
#endif  

  if ( err == ERR_OK) {
//    if ( web->current_connection == NULL) {
      web->current_connection = newpcb;

      /* Setup the connection */
      tcp_arg( newpcb, web);
      tcp_poll( newpcb, my_poll_tcp, 10);
      tcp_recv( newpcb, my_recv_tcp);
      tcp_sent( newpcb, my_sent_tcp);
      tcp_err( newpcb, my_err_tcp);

      accept_err = ERR_OK;
//    }
#ifdef HTTP_DBG	
    } else {
      prom_printf("One at a time please\r\n");
    }
#else	
  }
#endif  

  return accept_err;
}

int do_httpd(int argc, char *argv[]){

	struct tcp_pcb *web_pcb;
	struct ip_addr ipaddr;
	extern unsigned long local_ip;

	stats_init();
  	sys_init();
  	mem_init();
  	memp_init();
  	pbuf_init();
  	etharp_init();
	tcp_init();

#ifdef HTTP_DBG	
	prom_printf("start the web server\r\n");
#endif	

	the_web_connection.current_connection = NULL;
	the_web_connection.last_rx_seqno = 0;

	web_pcb = tcp_new();
  	if ( web_pcb == NULL) {
#ifdef HTTP_DBG		
   		prom_printf("Failed to create Web server PCB\r\n");
#endif		
   		return;
  	}

  	web_pcb = tcp_listen(web_pcb);

  	if ( web_pcb == NULL) {
#ifdef HTTP_DBG		
   		prom_printf( "Failed to listen on web pcb\r\n");
#endif		
   		return;
  	}
	 /* Pass the web connection data around the callbacks */
  	tcp_arg( web_pcb, (void *)&the_web_connection);

  	/* wait for connections */
  	tcp_accept( web_pcb, web_accept);

	ipaddr.addr = local_ip;
  	if ( tcp_bind( web_pcb, &ipaddr, 80) != ERR_OK) {
#ifdef HTTP_DBG		
   		prom_printf("Failed to bind to HTTP port (80)\r\n");
#endif		
   		return;
  	}

}
#endif


#ifdef SPI_TEST_CMD
int do_erase(int argc, char *argv[])
{
	unsigned long dst;
	unsigned long len;
	unsigned long retlen;

	dst = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	len = strtoul((const char*)(argv[2]), (char **)NULL, 16);

	printf("Erase flash from %X with %X bytes\n", dst, len);

	flash_erase(dst, len);
	printf("\n");
	//flash_write(dst, len, &retlen, (const unsigned char *) (src));

	return 0;
}

int do_program(int argc, char *argv[])
{
	unsigned long dst;
	unsigned long src;
	unsigned long len;
	unsigned long retlen;

	dst = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	src = strtoul((const char*)(argv[2]), (char **)NULL, 16);
	len = strtoul((const char*)(argv[3]), (char **)NULL, 16);

	printf("Write to flash from %X to %X with %X bytes\n", src, dst, len);

	//flash_erase(dst, len);
	printf("\n");
	flash_write(dst, len, &retlen, (const unsigned char *) (src));

	return 0;
}
#endif


// MT7530 Switch SLT Program Function
#if defined(MT7530_SLT) || defined(MT7530_SUPPORT)
extern uint32 gswPbusRead(uint32 pbus_addr);
extern int gswPbusWrite(uint32 pbus_addr, uint32 pbus_data);
#endif
#if defined(MT7530_SLT)
extern void tcMiiBpw(uint32 enetPhyAddr, uint32 phyReg, uint32 miiData);
extern int tcMiiBpr(uint32 enetPhyAddr, uint32 phyReg);
extern int tcMiiExtStationFillAddr(uint32 portAddr, uint32 devAddr, uint32 regAddr);
extern int tcMiiExtStationFillAddr_ext(uint32 portAddr, uint32 devAddr, uint32 regAddr);
extern void tcMiiExtStationWrite_ext(uint32 portAddr, uint32 devAddr, uint32 regAddr, uint32 miiData);
extern uint32 tcMiiExtStationRead_ext(uint32 portAddr, uint32 devAddr, uint32 regAddr, uint8 op);
extern void mtEMiiRegWrite(uint32 port_num, uint32 dev_num, uint32 reg_num, uint32 reg_data);
extern uint32 mtEMiiRegRead(uint32 port_num, uint32 dev_num, uint32 reg_num);
extern uint32 gswPmiRead(uint32 phy_addr, uint32 phy_reg);
extern uint32 gswPmiWrite(uint32 phy_addr, uint32 phy_reg, uint32 phy_data);
extern void mtMiiRegWrite(uint32 port_num, uint32 reg_num, uint32 reg_data);
extern uint32 mtMiiRegRead(uint32 port_num,uint8 reg_num);



static int doDumpGSW(int argc, char *argv[])
{
	int index = 0;
	int port;
	char buf[2000];

	uint32 mib_offset;

	port = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	{
		mib_offset = MIB_PORT_OFFSET*port;
		index += sprintf(buf+index, "[ Port %d ]\n", port);
		index += sprintf(buf+index, "Tx drop pkts          = 0x%08lx, ", gswPbusRead(MIB_TX_DROP_REG + mib_offset));
		index += sprintf(buf+index, "Tx ucast pkts         = 0x%08lx\n", gswPbusRead(MIB_TX_UCAST_REG + mib_offset));
		index += sprintf(buf+index, "Tx mcast pkts         = 0x%08lx, ", gswPbusRead(MIB_TX_MCAST_REG + mib_offset));
		index += sprintf(buf+index, "Tx bcast pkts         = 0x%08lx\n", gswPbusRead(MIB_TX_BCAST_REG + mib_offset));
		index += sprintf(buf+index, "Tx col	              = 0x%08lx, ", gswPbusRead(MIB_TX_COL_REG + mib_offset));
		index += sprintf(buf+index, "Tx scol               = 0x%08lx\n", gswPbusRead(MIB_TX_SCOL_REG + mib_offset));
		index += sprintf(buf+index, "Tx mcol               = 0x%08lx, ", gswPbusRead(MIB_TX_MCOL_REG + mib_offset));
		index += sprintf(buf+index, "Tx defer              = 0x%08lx\n", gswPbusRead(MIB_TX_DEFER_REG + mib_offset));
		index += sprintf(buf+index, "Tx lcol               = 0x%08lx, ", gswPbusRead(MIB_TX_LCOL_REG + mib_offset));
		index += sprintf(buf+index, "Tx xcol               = 0x%08lx\n", gswPbusRead(MIB_TX_XCOL_REG + mib_offset));
		index += sprintf(buf+index, "Tx pause              = 0x%08lx\n", gswPbusRead(MIB_TX_PAUSE_REG + mib_offset));
		index += sprintf(buf+index, "Tx 64 pkts            = 0x%08lx, ", gswPbusRead(MIB_TX_PKT_64_CNT + mib_offset));
		index += sprintf(buf+index, "Tx 65-127 pkts        = 0x%08lx\n", gswPbusRead(MIB_TX_PKT_65TO127_CNT + mib_offset));
		index += sprintf(buf+index, "Tx 128-255 pkts       = 0x%08lx, ", gswPbusRead(MIB_TX_PKT_128TO255_CNT + mib_offset));
		index += sprintf(buf+index, "Tx 256-511 pkts       = 0x%08lx\n", gswPbusRead(MIB_TX_PKT_256TO511_CNT + mib_offset));
		index += sprintf(buf+index, "Tx 512-1023 pkts      = 0x%08lx, ", gswPbusRead(MIB_TX_PKT_512TO1023_CNT + mib_offset));
		index += sprintf(buf+index, "Tx 1024-MAX pkts      = 0x%08lx\n", gswPbusRead(MIB_TX_PKT_1024TOMAX_CNT + mib_offset));
		index += sprintf(buf+index, "Rx drop pkts          = 0x%08lx, ", gswPbusRead(MIB_RX_DROP_REG + mib_offset));
		index += sprintf(buf+index, "Rx filter pkts        = 0x%08lx\n", gswPbusRead(MIB_RX_FILTER_REG + mib_offset));
		index += sprintf(buf+index, "Rx ucast pkts         = 0x%08lx, ", gswPbusRead(MIB_RX_UCAST_REG + mib_offset));
		index += sprintf(buf+index, "Rx mcast pkts         = 0x%08lx\n", gswPbusRead(MIB_RX_MCAST_REG + mib_offset));
		index += sprintf(buf+index, "Rx bcast pkts         = 0x%08lx, ", gswPbusRead(MIB_RX_BCAST_REG + mib_offset));
		index += sprintf(buf+index, "Rx align err pkts     = 0x%08lx\n", gswPbusRead(MIB_RX_ALIGN_ERR_REG + mib_offset));
		index += sprintf(buf+index, "Rx fcs pkts           = 0x%08lx, ", gswPbusRead(MIB_RX_FCS_ERR_REG + mib_offset));
		index += sprintf(buf+index, "Rx undersize pkts     = 0x%08lx\n", gswPbusRead(MIB_RX_UNDERSIZE_REG + mib_offset));
		index += sprintf(buf+index, "Rx frag err pkts      = 0x%08lx, ", gswPbusRead(MIB_RX_FRAG_ERR_REG + mib_offset));
		index += sprintf(buf+index, "Rx oversize pkts      = 0x%08lx\n", gswPbusRead(MIB_RX_OVERSIZE_REG + mib_offset));
		index += sprintf(buf+index, "Rx jabb err pkts      = 0x%08lx, ", gswPbusRead(MIB_RX_JABB_ERR_REG + mib_offset));
		index += sprintf(buf+index, "Rx pase pkts          = 0x%08lx\n", gswPbusRead(MIB_RX_PAUSE_REG + mib_offset));

		index += sprintf(buf+index, "Rx 64 pkts            = 0x%08lx, ", gswPbusRead(MIB_RX_PKT_64_CNT + mib_offset));
		index += sprintf(buf+index, "Rx 65-127 pkts        = 0x%08lx\n", gswPbusRead(MIB_RX_PKT_65TO127_CNT + mib_offset));
		index += sprintf(buf+index, "Rx 128-255 pkts       = 0x%08lx, ", gswPbusRead(MIB_RX_PKT_128TO255_CNT + mib_offset));
		index += sprintf(buf+index, "Rx 256-511 pkts       = 0x%08lx\n", gswPbusRead(MIB_RX_PKT_256TO511_CNT + mib_offset));
		index += sprintf(buf+index, "Rx 512-1023 pkts      = 0x%08lx, ", gswPbusRead(MIB_RX_PKT_512TO1023_CNT + mib_offset));
		index += sprintf(buf+index, "Rx 1024-MAX pkts      = 0x%08lx\n", gswPbusRead(MIB_RX_PKT_1024TOMAX_CNT + mib_offset));
		index += sprintf(buf+index, "Tx oct                = 0x%08lx, ", gswPbusRead(MIB_TX_OCT_CNT_L + mib_offset));
		index += sprintf(buf+index, "Rx oct                = 0x%08lx\n", gswPbusRead(MIB_RX_OCT_CNT_L + mib_offset));

		index += sprintf(buf+index, "------\n");

	}
	
	prom_printf("%s\n", buf);

	return 0;
}


static void doTCMiiBPWrite(int argc, char *argv[], void *p)
{
	uint32 enetPhyAddr, phyReg, miiData;
	uint32 cnt=1000000;

	
	enetPhyAddr=strtoul((const char*)(argv[1]), (char **)NULL, 16);

	phyReg=strtoul((const char*)(argv[2]), (char **)NULL, 16);

	miiData=strtoul((const char*)(argv[3]), (char **)NULL, 16);
	
	tcMiiBpw(enetPhyAddr, phyReg, miiData);

    prom_printf("PhyAddr=%x Reg=%x value:%x\r\n", enetPhyAddr, phyReg, miiData);
}


static int doTCMiiBPRead(int argc, char *argv[], void *p)
{
	uint32 reg, enetPhyAddr, phyReg;
	uint32 cnt=1000000;

	enetPhyAddr=strtoul((const char*)(argv[1]), (char **)NULL, 16);
	phyReg=strtoul((const char*)(argv[2]), (char **)NULL, 16);

	reg = tcMiiBpr(enetPhyAddr, phyReg);

	prom_printf("PhyAddr=%x Reg=%x value:%x\r\n", enetPhyAddr, phyReg, reg);

    return reg;
}

int doPhyMMDRead(int argc, char *argv[], void *p)
{
	
	uint32 dev_addr=0;
	uint32 phyaddr=0;
	uint32 reg_addr=0;
	uint32 start_addr=0;
	uint32 end_addr=0;
	uint32 value=0;
	int i=0;

	if (argc!=4 && argc!=6){
		prom_printf("Usage: emiir all <phyaddr> <devaddr> <s_addr> <e_addr>\r\n");
		prom_printf("       emiir <phyaddr> <devaddr> <reg>\r\n");
		return 0;			
	}

	if (strcmp(argv[1], "all") == 0){ /*Post Read*/
		phyaddr = simple_strtoul(argv[2],NULL,16);
		dev_addr = simple_strtoul(argv[3],NULL,16);
		reg_addr = simple_strtoul(argv[4],NULL,16);
		value = simple_strtoul(argv[5],NULL,16);

		if((start_addr>65535)  ||(end_addr>65535)){
			printk("s_addr or e_addr must be less than 65536\r\n");
			return 0;
		}
		for(i=start_addr; i<=end_addr; i++)
		{
			value=mtEMiiRegRead(phyaddr,dev_addr,i);
			prom_printf("* doPhyMMDRead =>phyaddr=%d,  dev_addr=%d, data_addr=0x%04lX , value=0x%04lX\r\n", phyaddr, dev_addr,  i, value);
		}
		prom_printf("\r\n");
	}
    else
	{
		// tce emiir <DevAddr> <PhyAddr> <regAddr>
		phyaddr = simple_strtoul(argv[1],NULL,16);
		dev_addr = simple_strtoul(argv[2],NULL,16);
		reg_addr = simple_strtoul(argv[3],NULL,16);
		value=mtEMiiRegRead(phyaddr,dev_addr,reg_addr);
		prom_printf("* doPhyMMDRead =>phyaddr=%d,  dev_addr=%d, data_addr=0x%04lX , value=0x%04lX\r\n", phyaddr, dev_addr, reg_addr, value);
		return 0;
    }
    return 0;           
}

int doPhyMMDWrite(int argc, char *argv[], void *p)
{
	uint32 dev_addr=0;
	uint32 phyaddr=0;
	uint32 reg_addr=0;
	uint32 value=0;
	uint16 STBit=0;
	uint16 BFlen=0;
	uint16 BF,BFMsk=0;
	uint32 RValue =0;
	uint8 i=0;


	if (argc==5)
	{
		// tce emiiw <DevAddr> <PhyAddr> <regAddr> <data>
		phyaddr = simple_strtoul(argv[1],NULL,16);
		dev_addr = simple_strtoul(argv[2],NULL,16);
		reg_addr = simple_strtoul(argv[3],NULL,16);
		value = simple_strtoul(argv[4],NULL,16);
    }
 	 if (((argc == 5) || (argc == 7)) && (phyaddr<=0x1f)){
 		     if(argc == 5){
				mtEMiiRegWrite(phyaddr, dev_addr,reg_addr,value);
				prom_printf("* doPhyMMDWrite => phyaddr=%d, dev_addr=%d, data_addr=0x%04lX , value=0x%04lX\r\n", phyaddr, dev_addr, reg_addr, value);
 		     }
 		     else if(((argc == 7) && (strcmp(argv[0], "emiiwb")) == 0)){
				RValue = mtEMiiRegRead(phyaddr,dev_addr, reg_addr);
				for(i=0;i<BFlen;i++){
					BF=1;
					BF=BF<<(STBit+i);
					BFMsk = BFMsk | BF;
				}
				BFMsk = ~BFMsk;
				value = (RValue & BFMsk) | (value<<STBit);
			  	prom_printf("* Phyaddr=%d, DevAddr=0x%02lX,RegAddr=0x%02lX, STBit=%0d, BFlen=%0d, value=0x%04lX\r\n", phyaddr,dev_addr, reg_addr, STBit, BFlen, value);		
				prom_printf("* OrgValue=0x%04lX,",RValue);
				mtEMiiRegWrite(phyaddr, dev_addr,reg_addr, value);
				RValue = mtEMiiRegRead(phyaddr,dev_addr, reg_addr);
				prom_printf(" ModValue=0x%04lX\r\n", RValue);
	 	   	}     	
			return 0;
	}
	else {
        prom_printf("       CMD Error : emiiw <PhyAddr> <DevAddr> <regAddr> <data>\r\n");
		prom_printf("                         emiiwb <PhyAddr> <DevAddr> <RegAddr> <STBit> <BFLen> <BFVal> \r\n");	
        return 0;      
	}
}



int
doPhyMiiRead (int argc, char *argv[], void *p)
{
    uint16 phyaddr=0;
    const uint16 page_reg=31;
    uint16 page_val=0;
    uint32 reg=0;
    uint32 value;

    int i;

    //  argc:3
    //  tce miir all <PhyAddr> 
    //  tce miir <PhyAddr> <RegAddr>
    //  argc:4
    //  tce miir all <PhyAddr> <PageNo>
    //  tce miir <PhyAddr> <PageNo> <RegAddr>

    // get parameters
    if (argc==3){
        if (strcmp(argv[1], "all") == 0){ // tce miir all <PhyAddr> 
			phyaddr = strtoul((const char*)(argv[2]), (char **)NULL, 16);
        }
        else { // tce miir <PhyAddr> <RegAddr>
			phyaddr = strtoul((const char*)(argv[1]), (char **)NULL, 16);
			reg = strtoul((const char*)(argv[2]), (char **)NULL, 16);
        }
    }
    else if (argc==4){
        if (strcmp(argv[1], "all") == 0){ // tce miir all <PhyAddr> <PageNo>
			phyaddr = strtoul((const char*)(argv[2]), (char **)NULL, 16);
            page_val = getMiiPage(argv[3]);
        }
        else{ // tce miir <PhyAddr> <PageNo> <RegAddr>
			phyaddr = strtoul((const char*)(argv[1]), (char **)NULL, 16);
            page_val = getMiiPage(argv[2]);
			reg = strtoul((const char*)(argv[3]), (char **)NULL, 16);
        }
    }

    if(reg != 31)
      mtMiiRegWrite(phyaddr, page_reg, 0);

    // check parameters
    if ((argc==3 || argc==4) 
        && phyaddr<=31 && reg<=31 && page_val!=0xffff){
        // set page
        if (argc==4){           
            if (strcmp(argv[1], "all") == 0){ // multiple read
                prom_printf("* PageNo=%s ",argv[3]);
                prom_printf("\r\n");             
            }
            else{
                prom_printf("* PageNo=%s ",argv[2]);
            }
            mtMiiRegWrite(phyaddr, page_reg, page_val);
        }
        // read data
        if (strcmp(argv[1], "all") == 0){ // multiple read
            for( i=0; i<32; i++ ){
                value = mtMiiRegRead(phyaddr, i);
                prom_printf("[reg=0x%02lX val=%04lX]", i, value);
                if( (i+1) % 4 == 0 )
                    prom_printf("\r\n");             
            }
        }
        else{
//			Frank
            value = mtMiiRegRead(phyaddr, reg);
            prom_printf("* PhyAddr=%d RegAddr=0x%02lX value=0x%04lX\r\n", phyaddr, reg, value);
        }       
        return 0;           
    }   
    else { // error message
        prom_printf("Usage: miir all <PhyAddr> [PageNo]\r\n");
        prom_printf("       miir <PhyAddr> <RegAddr>\r\n");
        prom_printf("       miir <PhyAddr> <PageNo> <RegAddr>\r\n");
        return 0;           
    }
}

int
doPhyMiiWrite (int argc, char *argv[], void *p)
{
	uint16 phyaddr=0;
	const uint16 page_reg=31;
	uint16 page_val=0;
	uint32 reg=0;
	uint32 value=0;
	uint16 STBit=0;
	uint16 BFlen=0;
	uint16 BF,BFMsk=0;
	uint32 RValue =0;
	int i;


    //  tce miir <PhyAddr> <RegAddr> <Value>
    //  tce miir <PhyAddr> <PageNo> <RegAddr> <Value>

    if (argc==4){
		phyaddr = strtoul((const char*)(argv[1]), (char **)NULL, 16);
		reg = strtoul((const char*)(argv[2]), (char **)NULL, 16);
		value = strtoul((const char*)(argv[3]), (char **)NULL, 16);
		prom_printf("*1: argv[2]=%s, argv[3]=%s\r\n", argv[2], argv[3]);		
		prom_printf("*2: phyaddr=%d, reg=%x, value=%x\r\n", phyaddr, reg, value);
    }
    else if (argc==5){
		phyaddr = strtoul((const char*)(argv[1]), (char **)NULL, 16);
        page_val = getMiiPage(argv[2]);
		reg = strtoul((const char*)(argv[3]), (char **)NULL, 16);
		value = strtoul((const char*)(argv[4]), (char **)NULL, 16);
		prom_printf("*1: argv[1]=%s, argv[2]=%s, argv[3]=%s, argv[4]=%s\n", argv[1], argv[2], argv[3], argv[4]);	
		prom_printf("*2: phyaddr=%d, page =%x, reg=%x, value=%x\r\n", phyaddr, page_val, reg, value);
    }
	else if (argc==6){
		if(strcmp(argv[0], "miiwb") == 0){
			phyaddr = strtoul((const char*)(argv[1]), (char **)NULL, 16);
			reg = strtoul((const char*)(argv[2]), (char **)NULL, 16);
			STBit = strtoul((const char*)(argv[3]), (char **)NULL, 16);
			BFlen = strtoul((const char*)(argv[4]), (char **)NULL, 16);
			value = strtoul((const char*)(argv[5]), (char **)NULL, 16);
			prom_printf("* Phyaddr=%d, RegAddr=0x%02lX, STBit=%0d, BFlen=%0d, value=0x%04lX\r\n", phyaddr, reg, STBit, BFlen, value);		
		}
	}
	else if (argc==7){
		if(strcmp(argv[0], "miiwb") == 0){
			phyaddr = strtoul((const char*)(argv[1]), (char **)NULL, 16);
			page_val = getMiiPage(argv[2]);
			reg = strtoul((const char*)(argv[3]), (char **)NULL, 16);
			STBit = strtoul((const char*)(argv[4]), (char **)NULL, 16);
			BFlen = strtoul((const char*)(argv[5]), (char **)NULL, 16);
			value = strtoul((const char*)(argv[6]), (char **)NULL, 16);
			prom_printf("* Phyaddr=%d, pageNo=%d, RegAddr=0x%02lX, STBit=%0d, BFlen=%0d, value=0x%04lX\r\n", phyaddr, page_val, reg, STBit, BFlen, value);
		}
	}

    // check parameters and write
    
    if(reg != 31)
      mtMiiRegWrite(phyaddr, page_reg, 0);
    
    if ((argc==4 || argc==5) 
        && (phyaddr<=31) && (reg<=31) && (page_val!=0xffff)){
        // set page
        if (argc==5){
            prom_printf("* PageNo=%s ",argv[2]);
            mtMiiRegWrite(phyaddr, page_reg, page_val);
        }
        // write data
        prom_printf("* Phyaddr=%d RegAddr=0x%02lX value=0x%04lX\r\n", phyaddr, reg, value);
        mtMiiRegWrite(phyaddr, reg, value);
    }
 	else if (((argc == 6) || (argc == 7))
		&& (phyaddr<=31) && (reg<=31) && (page_val != 0xffff)){
        if (argc==7){
            prom_printf("* PageNo=%s ",argv[2]);
            mtMiiRegWrite(phyaddr, page_reg, page_val);
        }
		if(strcmp(argv[0], "miiwb") == 0){
			RValue = mtMiiRegRead(phyaddr, reg);
			for(i=0;i<BFlen;i++){
				BF=1;
				BF=BF<<(STBit+i);
				BFMsk = BFMsk | BF;
			}
			BFMsk = ~BFMsk;
			value = (RValue & BFMsk) | (value<<STBit);
			prom_printf("* OrgValue=0x%04lX,",RValue);
			mtMiiRegWrite(phyaddr, reg, value);
			RValue = mtMiiRegRead(phyaddr, reg);
			prom_printf(" ModValue=0x%04lX\r\n", RValue);
		}
    }
    else { // error message
		prom_printf("Usage: miiw <PhyAddr> <RegAddr> <RegVal>\r\n");
		prom_printf("       miiw <PhyAddr> <PageNo> <RegAddr> <RegVal>\r\n");
		prom_printf("       miiwb <PhyAddr> <RegAddr> <STBit> <BFLen> <BFVal> \r\n");	
		prom_printf("       miiwb <PhyAddr> <PageNo> <RegAddr> <STBit> <BFLen> <BFVal> \r\n");	
		return 0;
    }
    return 0;
}
#endif
#ifndef BOOTROM_IN_SRAM

#if defined(MT7530_SLT) || defined(MT7530_SUPPORT)

int 
doGswRead(int argc, char *argv[], void *p)
{
    uint16 pbus_addr=0;
    uint32 value;

	pbus_addr = strtoul((const char*)(argv[1]), (char **)NULL, 16);

	prom_printf("Read gsw register: %x\n", pbus_addr);

	value = gswPbusRead(pbus_addr);

	prom_printf("value: %x\n", value);
	
	return 0;
}

int 
doGswWrite(int argc, char *argv[], void *p)
{
    uint16 pbus_addr=0;
    uint32 pbus_data=0;

	pbus_addr = strtoul((const char*)(argv[1]), (char **)NULL, 16);

	pbus_data = strtoul((const char*)(argv[2]), (char **)NULL, 16);

	prom_printf("Write gsw register %x into %x value\n", pbus_addr, pbus_data);

	gswPbusWrite(pbus_addr, pbus_data);
	
	return 0;
}
#endif
int do_miir(int argc, char *argv[])
{
	unsigned long phyaddr;
	unsigned long reg;

	phyaddr = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	reg = strtoul((const char*)(argv[2]), (char **)NULL, 16);

	printf("Read phy reg %x from addr %x=%04x\n", reg, phyaddr, miiStationRead(phyaddr, reg));

	return 0;
}

int do_miiw(int argc, char *argv[])
{
	unsigned long phyaddr;
	unsigned long reg;
	unsigned long value;

	phyaddr = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	reg = strtoul((const char*)(argv[2]), (char **)NULL, 16);
	value = strtoul((const char*)(argv[3]), (char **)NULL, 16);

	miiStationWrite(phyaddr, reg, value);
	printf("Write phy reg %x from addr %x=%04x\n", reg, phyaddr, miiStationRead(phyaddr, reg));

	return 0;
}
#endif


static u32 zd_rand = 0;
static u32 rand_word = 0;

static u8 random_byte(void)
{
	zd_rand = zd_rand * 1664525 + 1013904223;
	return (u8) (zd_rand >> 24);
}
#ifdef SPI_TEST_CMD
static u32 random_word(void)
{
	rand_word += (VPint(CR_TIMER1_VLR)) * (random_byte());
	return rand_word;
}

int do_memcmp(int argc, char *argv[])
{
	unsigned long addr1;
	unsigned long addr2;
	unsigned long len;
	int result;

        addr1 = strtoul((const char*)(argv[1]), (char **)NULL, 16);
        addr2 = strtoul((const char*)(argv[2]), (char **)NULL, 16);
        len   = strtoul((const char*)(argv[3]), (char **)NULL, 16);
	result = memcmp(addr1, addr2, len);
	printf("memcmp addr1=%X addr2=%X len=%X result=%d %X\n", addr1, addr2, len, result, result);

	return 0;
}

int do_dmacpy(int argc, char *argv[])
{
	unsigned long addr1;
	unsigned long addr2;
	unsigned long len;
	unsigned char ch, burst_size;

	addr1 = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	addr2 = strtoul((const char*)(argv[2]), (char **)NULL, 16);
	len   = strtoul((const char*)(argv[3]), (char **)NULL, 16);

	if (isRT63365 || isMT751020 || isMT7505) {
		ch = random_byte() & 0xf;
		burst_size = random_byte() & 0x7;
		if (burst_size > 4)
			burst_size = 4;
		if (len == 0)
			len = 1;
		// GDMA
		VPint(CR_GDMA_SA(ch)) = (addr2 & 0x1fffffff);
		VPint(CR_GDMA_DA(ch)) = (addr1 & 0x1fffffff);
		VPint(CR_GDMA_CT1(ch)) = (32<<16) | (32<<8);
		VPint(CR_GDMA_CT0(ch)) = ((len&0xffff)<<16) | (burst_size<<3) | (1<<1) | (1<<0);
		while (!(VPint(CR_GDMA_DONEINT) & (1<<ch))) ;
		VPint(CR_GDMA_DONEINT) = (1<<ch);
	} else {
		// GDMA
		VPint(CR_GDMA_DCSA) = (addr2 & 0x1fffffff);
		VPint(CR_GDMA_DCDA) = (addr1 & 0x1fffffff);
		VPint(CR_GDMA_DCBT) = (len & 0xffff);
		VPint(CR_GDMA_DCC) = 0x101;
		while(!((*(volatile unsigned char *)(0xbfb3000d) & 0x1)));
		*(volatile unsigned char *)(0xbfb3000d) = 0x3;
	}

	if (isRT63365) 
		printf("ch=%d burst_size=%d ", ch, burst_size);
	printf("dmacpy addr1=%X addr2=%X len=%X\n", addr1, addr2, len);

	return 0;
}

int do_memcpy(int argc, char *argv[])
{
	unsigned long addr1;
	unsigned long addr2;
	unsigned long len;

	addr1 = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	addr2 = strtoul((const char*)(argv[2]), (char **)NULL, 16);
	len   = strtoul((const char*)(argv[3]), (char **)NULL, 16);
	memcpy(addr1, addr2, len);
	printf("memcpy addr1=%X addr2=%X len=%X\n", addr1, addr2, len);

	return 0;
}

int do_memcmp4(int argc, char *argv[])
{
	unsigned long addr1;
	unsigned long addr2;
	unsigned long len;
	int result;

	addr1 = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	addr2 = strtoul((const char*)(argv[2]), (char **)NULL, 16);
	len   = strtoul((const char*)(argv[3]), (char **)NULL, 16);
	result = memcmp4(addr1, addr2, len);
	printf("memcmp4 addr1=%X addr2=%X len=%X result=%d %X\n", addr1, addr2, len, result, result);

	return 0;
}

int do_memcpy4(int argc, char *argv[])
{
	unsigned long addr1;
	unsigned long addr2;
	unsigned long len;

	addr1 = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	addr2 = strtoul((const char*)(argv[2]), (char **)NULL, 16);
	len   = strtoul((const char*)(argv[3]), (char **)NULL, 16);
	memcpy4(addr1, addr2, len);
	printf("memcpy4 addr1=%X addr2=%X len=%X\n", addr1, addr2, len);

	return 0;
}

int do_memcmp2(int argc, char *argv[])
{
	unsigned long addr1;
	unsigned long addr2;
	unsigned long len;
	int result;

	addr1 = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	addr2 = strtoul((const char*)(argv[2]), (char **)NULL, 16);
	len   = strtoul((const char*)(argv[3]), (char **)NULL, 16);
	result = memcmp2(addr1, addr2, len);
	printf("memcmp2 addr1=%X addr2=%X len=%X result=%d %X\n", addr1, addr2, len, result, result);

	return 0;
}

int do_memcpy2(int argc, char *argv[])
{
	unsigned long addr1;
	unsigned long addr2;
	unsigned long len;

	addr1 = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	addr2 = strtoul((const char*)(argv[2]), (char **)NULL, 16);
	len   = strtoul((const char*)(argv[3]), (char **)NULL, 16);
	memcpy2(addr1, addr2, len);
	printf("memcpy2 addr1=%X addr2=%X len=%X\n", addr1, addr2, len);

	return 0;
}


int do_memset(int argc, char *argv[])
{
	unsigned long addr;
	unsigned char pattern;
	unsigned long len;

	addr = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	pattern = strtoul((const char*)(argv[2]), (char **)NULL, 16);
	len   = strtoul((const char*)(argv[3]), (char **)NULL, 16);
	memset(addr, pattern, len);
	printf("memset addr=%X pattern=%X len=%X\n", addr, pattern, len);

	return 0;
}

int do_memset4(int argc, char *argv[])
{
	unsigned long addr;
	unsigned long pattern;
	unsigned long len;

	addr = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	pattern = strtoul((const char*)(argv[2]), (char **)NULL, 16);
	len   = strtoul((const char*)(argv[3]), (char **)NULL, 16);
	memset4(addr, pattern, len);
	printf("memset4 addr=%X pattern=%X len=%X\n", addr, pattern, len);

	return 0;
}

int do_memset42(int argc, char *argv[])
{
	unsigned long addr;
	unsigned long pattern1;
	unsigned long pattern2;
	unsigned long len;
	unsigned int *xs;
	int toggle = 1;

	addr = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	pattern1 = strtoul((const char*)(argv[2]), (char **)NULL, 16);
	pattern2 = strtoul((const char*)(argv[3]), (char **)NULL, 16);
	len   = strtoul((const char*)(argv[4]), (char **)NULL, 16);
	printf("memset4 addr=%X pattern1=%X pattern2=%X len=%X\n", addr, pattern1, pattern2, len);
	xs = (unsigned int *) addr;
	len = len >> 2;
	while (len--) {
		if (toggle)
			*xs++ = pattern1;
		else
			*xs++ = pattern2;
		toggle = toggle ^ 1;
	}

	return 0;
}

int do_memset44(int argc, char *argv[])
{
	unsigned long addr;
	unsigned long pattern[4];
	unsigned long len;
	unsigned int *xs;
	int i;

	addr = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	for (i = 0 ; i < 4; i++)
		pattern[i] = strtoul((const char*)(argv[2+i]), (char **)NULL, 16);
	len   = strtoul((const char*)(argv[6]), (char **)NULL, 16);
	printf("memset4 addr=%X p1~4=%X %X %X %X len=%X\n", addr, pattern[0], pattern[1], pattern[2], pattern[3], len);
	xs = (unsigned int *) addr;
	len = len >> 2;
	i=0;
	while (len--) {
		*xs++ = pattern[i];
		i++;
		if (i >= 4)
			i = 0;
	}

	return 0;
}

int do_incr(int argc, char *argv[])
{
	unsigned long addr;
	unsigned long pattern;
	unsigned long len;
	unsigned char *xs;
	unsigned char pat;
	unsigned char step = 1;
	int i;

	addr = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	pattern = strtoul((const char*)(argv[2]), (char **)NULL, 16);
	len   = strtoul((const char*)(argv[3]), (char **)NULL, 16);
    if (argc == 5)
		step   = strtoul((const char*)(argv[4]), (char **)NULL, 16);

	pat = (unsigned char) pattern;

	printf("incr addr=%X p=%X len=%X step=%X\n", addr, pat, len, step);
	xs = (unsigned char *) addr;

	while (len--) {
		*xs++ = pat;
		pat += step;
	}

	return 0;
}

int do_incrcmp(int argc, char *argv[])
{
	unsigned long addr;
	unsigned long pattern;
	unsigned long len;
	unsigned char *xs;
	unsigned char pat;
	unsigned char step = 1;
	int i;

	addr = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	pattern = strtoul((const char*)(argv[2]), (char **)NULL, 16);
	len   = strtoul((const char*)(argv[3]), (char **)NULL, 16);
    if (argc == 5)
		step   = strtoul((const char*)(argv[4]), (char **)NULL, 16);

	pat = (unsigned char) pattern;

	printf("incrcmp addr=%X p=%X len=%X step=%X\n", addr, pat, len, step);
	xs = (unsigned char *) addr;
	while (len--) {
		if (*xs != pat)
			printf("incrcmp addr1=%X len=%X result=%X %X\n", xs, len, *xs, pat);
		xs++;
		pat += step;
	}

	return 0;
}

int do_rreadb(int argc, char* argv[])
{
	unsigned long addr1;
	unsigned long addr2;
	int i;
	const unsigned char *su1, *su2, *su3 = 0x80020000;
	unsigned char c, d;
	int offset;

	addr1 = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	addr2 = strtoul((const char*)(argv[2]), (char **)NULL, 16);

	memcpy(0x80020000, addr1, addr2-addr1+1);

	su1 = (const unsigned char *) addr1;
	su2 = (const unsigned char *) addr2;

	while (su1 < su2) {
		c = *su1;
		d = *su3;

		if (c != d)
			printf("error su1=%08x su3=%08x c=%02x d=%02x\n", su1, su3, c, d);

		offset = (random_byte() & 0x7f);
		su1 += offset;
		su3 += offset;
	}

	return 0;
}

int do_rreadl(int argc, char* argv[])
{
	unsigned long addr1;
	unsigned long addr2;
	volatile unsigned long *su1, *su2;
	volatile unsigned long val;
	unsigned long i, cnt;
	int offset;

	addr1 = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	addr2 = strtoul((const char*)(argv[2]), (char **)NULL, 16);
	cnt = strtoul((const char*)(argv[3]), (char **)NULL, 16);

	for (i = 0; i < cnt; i++) {
		su1 = (unsigned long *) addr1;
		su2 = (unsigned long *) addr2;

		while (su1 < su2) {
			val = *su1;
			
			offset = 1;
			su1 += offset;
		}
	}

	return 0;
}

int do_rwritel(int argc, char* argv[])
{
	unsigned long addr1;
	unsigned long addr2;
	unsigned long *su1, *su2;
	unsigned long c, d;
	unsigned long pattern;
	unsigned long i, cnt;
	unsigned char random = 0;
	int offset;

	addr1 = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	addr2 = strtoul((const char*)(argv[2]), (char **)NULL, 16);
	pattern = strtoul((const char*)(argv[3]), (char **)NULL, 16);
	cnt = strtoul((const char*)(argv[4]), (char **)NULL, 16);

	if (pattern == 0x00000000)
		random = 1;

	for (i = 0; i < cnt; i++) {
		su1 = (unsigned long *) addr1;
		su2 = (unsigned long *) addr2;

		while (su1 < su2) {
			*su1 = pattern;

			offset = 1;
			su1 += offset;

			if (random)
				pattern += VPint(CR_TIMER1_VLR) * random_byte();
		}
	}

	return 0;
}

int do_step(int argc, char *argv[])
{
	unsigned long addr;
	unsigned long value;
	unsigned long step;
	unsigned long len;
	unsigned char *xs;
	unsigned char pat1;
	unsigned char pat2;
	unsigned char toggle = 1;
	int i;

	addr = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	value = strtoul((const char*)(argv[2]), (char **)NULL, 16);
	pat1 = (unsigned char) value;
	value = strtoul((const char*)(argv[3]), (char **)NULL, 16);
	pat2 = (unsigned char) value;
	step = strtoul((const char*)(argv[4]), (char **)NULL, 16);
	len = strtoul((const char*)(argv[5]), (char **)NULL, 16);

	printf("step addr=%X p1=%X p2=%x step=%X len=%X\n", addr, pat1, pat2, step, len);
	xs = (unsigned char *) addr;
	i = 0;
	while (len--) {
		if (toggle)
			*xs++ = pat1;
		else
			*xs++ = pat2;
		i++;
		if (i >= step) {
			toggle ^= 1;
			i = 0;
		}
	}

	return 0;
}


int do_spidbg(int argc, char* argv[])
{
	extern unsigned long spidbg;

	spidbg = strtoul((const char*)(argv[1]), (char **)NULL, 16);

	prom_printf("spi dbg=%X\n", spidbg);

	return 0;
}
#ifndef TCSUPPORT_NEW_SPIFLASH
extern void spiflash_qe(int hpm);
extern void spiflash_rdst(void);
#endif

int do_spiqe(int argc, char* argv[])
{
#ifndef TCSUPPORT_NEW_SPIFLASH

	int hpm;

    hpm	= strtoul((const char*)(argv[1]), (char **)NULL, 10);
	prom_printf("hpm=%X\n", hpm);
	spiflash_qe(hpm);
#endif
	return 0;
}

int do_spird(int argc, char* argv[])
{
#ifndef TCSUPPORT_NEW_SPIFLASH

	spiflash_rdst();
#endif

	return 0;
}
#endif

#ifdef MT7505_SWITCH_TBL_VERIRY
#define REG_ATC_ADDR			(0xbfb58080)
#define REG_ATC_AC_CMD_OFFT		(0)
#define REG_ATC_AC_CMD_LENG		(3)
#define REG_ATC_AC_CMD_RELMASK		(0x00000007)
#define REG_ATC_AC_CMD_MASK		(REG_ATC_AC_CMD_RELMASK << REG_ATC_AC_CMD_OFFT)
#define REG_ATC_AC_SAT_OFFT		(4)
#define REG_ATC_AC_SAT_LENG		(2)
#define REG_ATC_AC_SAT_RELMASK		(0x00000003)
#define REG_ATC_AC_SAT_MASK		(REG_ATC_AC_SAT_RELMASK << REG_ATC_AC_SAT_OFFT)
#define REG_ATC_AC_MAT_OFFT		(8)
#define REG_ATC_AC_MAT_LENG		(4)
#define REG_ATC_AC_MAT_RELMASK		(0x0000000F)
#define REG_ATC_AC_MAT_MASK		(REG_ATC_AC_MAT_RELMASK << REG_ATC_AC_MAT_OFFT)
#define REG_AT_SRCH_HIT_OFFT		(13)
#define REG_AT_SRCH_HIT_RELMASK		(0x00000001)
#define REG_AT_SRCH_HIT_MASK		(REG_AT_SRCH_HIT_RELMASK << REG_AT_SRCH_HIT_OFFT)
#define REG_AT_SRCH_END_OFFT		(14)
#define REG_AT_SRCH_END_RELMASK		(0x00000001)
#define REG_AT_SRCH_END_MASK		(REG_AT_SRCH_END_RELMASK << REG_AT_SRCH_END_OFFT)
#define REG_ATC_BUSY_OFFT		(15)
#define REG_ATC_BUSY_LENG		(1)
#define REG_ATC_BUSY_RELMASK		(0x00000001)
#define REG_ATC_BUSY_MASK		(REG_ATC_BUSY_RELMASK << REG_ATC_BUSY_OFFT)
#define REG_AT_ADDR_OFFT		(16)
#define REG_AT_ADDR_LENG		(12)
#define REG_AT_ADDR_RELMASK		(0x00000FFF)
#define REG_AT_ADDR_MASK		(REG_AT_ADDR_RELMASK << REG_AT_ADDR_OFFT)
#define REG_TSRA1_ADDR			(0xbfb58084)
#define REG_TSRA2_ADDR			(0xbfb58088)
#define REG_ATRD_ADDR			(0xbfb5808C)

#define REG_VTCR_ADDR			(0xbfb58090)
#define REG_VTCR_VID_OFFT		(0)
#define REG_VTCR_VID_LENG		(12)
#define REG_VTCR_VID_RELMASK		(0x00000FFF)
#define REG_VTCR_VID_MASK		(REG_VTCR_VID_RELMASK << REG_VTCR_VID_OFFT)
#define REG_VTCR_FUNC_OFFT		(12)
#define REG_VTCR_FUNC_LENG		(4)
#define REG_VTCR_FUNC_RELMASK		(0x0000000F)
#define REG_VTCR_FUNC_MASK		(REG_VTCR_FUNC_RELMASK << REG_VTCR_FUNC_OFFT)
#define REG_VTCR_IDX_INVLD_OFFT		(16)
#define REG_VTCR_IDX_INVLD_RELMASK	(0x00000001)
#define REG_VTCR_IDX_INVLD_MASK		(REG_VTCR_IDX_INVLD_RELMASK << REG_VTCR_IDX_INVLD_OFFT)
#define REG_VTCR_BUSY_OFFT		(31)
#define REG_VTCR_BUSY_RELMASK		(0x00000001)
#define REG_VTCR_BUSY_MASK		(REG_VTCR_BUSY_RELMASK << REG_VTCR_BUSY_OFFT)

#define REG_VAWD1_ADDR			(0xbfb58094)
#define REG_VAWD2_ADDR			(0xbfb58098)

void MT7505_Switch_Tbl_Test(void)
{
	int i;
	unsigned long vawd1, vawd2, rule_vawd1, rule_vawd2, rate_vawd1, rate_vawd2;
	unsigned long value, reg;

	prom_printf("VLAN TABLE------------------->\n");
	for (i=0; i<16; i++)
	{		
		prom_printf("Entry[%d]--------->\n",i);	
		reg = REG_VTCR_ADDR; // VTCR
		value = (0x80000000 + i);
		//gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
		VPint(reg) = value;
		reg = REG_VTCR_ADDR; // VTCR
		while (1) {
			//value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
			value = VPint(reg);
			if ((value & 0x80000000) == 0) { //table busy
				break;
			}
		}
		reg = REG_VAWD1_ADDR; // VAWD1
		//vawd1 = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
		vawd1 = VPint(reg);		
		prom_printf("bfb58094=%x\n", vawd1);
	}

	prom_printf("MAC TABLE------------------->\n");
	for (i=0; i<512; i=i+4)	
	{
		prom_printf("Entry[%d]--------->\n",i);	
		VPint(0xbfb58078) = i << 2;
		VPint(REG_ATC_ADDR) = 0x8030;
		prom_printf("bfb58084=%x,bfb58088=%x,bfb5808c=%x\n", VPint(REG_TSRA1_ADDR),VPint(REG_TSRA2_ADDR),VPint(REG_ATRD_ADDR));
	}	
	
	prom_printf("ACL TABLE------------------->\n");
	{
		while (1){	// wait until not busy
			//value = gsw_reg_read(swcfg->skfd, swcfg->ifr, REG_VTCR_ADDR);
			value = VPint(REG_VTCR_ADDR); 
			if ((value & REG_VTCR_BUSY_MASK) == 0){
				break;
			}
		}

		// Display ACL table entry 0~63
		prom_printf("Found following ACL table entries :\n");
		for (i = 0; i < 64; i ++){
			value = REG_VTCR_BUSY_MASK | (0x04 << REG_VTCR_FUNC_OFFT) | i;	// read ACL table (0x04)
			//gsw_reg_write(swcfg->skfd, swcfg->ifr, REG_VTCR_ADDR, value);
			VPint(REG_VTCR_ADDR) = value;
			while (1){	// wait until not busy
				//value = gsw_reg_read(swcfg->skfd, swcfg->ifr, REG_VTCR_ADDR);
				value = VPint(REG_VTCR_ADDR);
				if ((value & REG_VTCR_BUSY_MASK) == 0){
					break;
				}
			}
			//vawd1 = gsw_reg_read(swcfg->skfd, swcfg->ifr, REG_VAWD1_ADDR);
			vawd1 = VPint(REG_VAWD1_ADDR);
			//vawd2 = gsw_reg_read(swcfg->skfd, swcfg->ifr, REG_VAWD2_ADDR);
			vawd2 = VPint(REG_VAWD2_ADDR);

			if (vawd2 & (1 << 19)){ // EN==1
				prom_printf("[#%2d] CMP_SEL:%d, SP_map:%02X, OFST_TP:%d, WORD_OFST:%3d, BIT_MASK:%04X, CMP_PAT:%04X\n",
					i, (vawd2 & 1), ((vawd2>>8) & 0xFF), ((vawd2>>16) & 0x07), ((vawd2>>1) & 0x3F), ((vawd1>>16) & 0xFFFF), (vawd1 & 0xFFFF));
			}
		}

		// Display Mask, Rule Ctrl, and Rate Ctrl table entry 0~31
		prom_printf("Found following Mask, Rule ctrl, Rate ctrl table entries : (in vawd1,vawd2)\n");
		for (i = 0; i < 32; i ++){
			value = REG_VTCR_BUSY_MASK | (0x08 << REG_VTCR_FUNC_OFFT) | i;	// read Mask Table (0x08)
			//gsw_reg_write(swcfg->skfd, swcfg->ifr, REG_VTCR_ADDR, value);
			VPint(REG_VTCR_ADDR) = value;
			while (1){	// wait until not busy
				//value = gsw_reg_read(swcfg->skfd, swcfg->ifr, REG_VTCR_ADDR);
				value = VPint(REG_VTCR_ADDR);
				if ((value & REG_VTCR_BUSY_MASK) == 0){
					break;
				}
			}
			//vawd1 = gsw_reg_read(swcfg->skfd, swcfg->ifr, REG_VAWD1_ADDR);
			vawd1 = VPint(REG_VAWD1_ADDR);
			//vawd2 = gsw_reg_read(swcfg->skfd, swcfg->ifr, REG_VAWD2_ADDR);
			vawd2 = VPint(REG_VAWD2_ADDR);

			if (vawd1 || vawd2){	// ACL_MASK[63:0] any bit != 0
				value = REG_VTCR_BUSY_MASK | (0x0A << REG_VTCR_FUNC_OFFT) | i;	// read Rule Ctrl Table (0x0A)
				//gsw_reg_write(swcfg->skfd, swcfg->ifr, REG_VTCR_ADDR, value);
				VPint(REG_VTCR_ADDR)=value;
				while (1){	// wait until not busy
					//value = gsw_reg_read(swcfg->skfd, swcfg->ifr, REG_VTCR_ADDR);
					value = VPint(REG_VTCR_ADDR);
					if ((value & REG_VTCR_BUSY_MASK) == 0){
						break;
					}
				}
				//rule_vawd1 = gsw_reg_read(swcfg->skfd, swcfg->ifr, REG_VAWD1_ADDR);
				rule_vawd1 = VPint(REG_VAWD1_ADDR);
				//rule_vawd2 = gsw_reg_read(swcfg->skfd, swcfg->ifr, REG_VAWD2_ADDR);
				rule_vawd2 = VPint(REG_VAWD2_ADDR);

				value = REG_VTCR_BUSY_MASK | (0x0C << REG_VTCR_FUNC_OFFT) | i;	// read Rate Ctrl Table (0x0C)
				//gsw_reg_write(swcfg->skfd, swcfg->ifr, REG_VTCR_ADDR, value);
				VPint(REG_VTCR_ADDR)=value;
				while (1){	// wait until not busy
					//value = gsw_reg_read(swcfg->skfd, swcfg->ifr, REG_VTCR_ADDR);
					value = VPint(REG_VTCR_ADDR);					
					if ((value & REG_VTCR_BUSY_MASK) == 0){
						break;
					}
				}
				//rate_vawd1 = gsw_reg_read(swcfg->skfd, swcfg->ifr, REG_VAWD1_ADDR);
				rate_vawd1 = VPint(REG_VAWD1_ADDR);
				//rate_vawd2 = gsw_reg_read(swcfg->skfd, swcfg->ifr, REG_VAWD2_ADDR);
				rate_vawd2= VPint(REG_VAWD2_ADDR);
				
				prom_printf("[#%2d] Mask:%08X,%08X; Rule:%08X,%08X; Rate:%08X,%08X\n",
					i, vawd1, vawd2, rule_vawd1, rule_vawd2, rate_vawd1, rate_vawd2);
			}
		}
	}	
}
#endif

#ifdef SPI_DRAM_TEST_CMD
#define MAX_CMD_LEN 			(128)
static void getAttrVal(const char *inStr, const char *pAttr, const char *delim, char *pVal, int valBufLen) {
	char tmp[MAX_CMD_LEN] = {0};
	char *t;
	int len;

	if((inStr == 0) || (pAttr == 0) || (delim == 0) || (pVal == 0))
	{
		return;
	}

	len = strlen(inStr);
	if(len >= MAX_CMD_LEN)
	{
		len = MAX_CMD_LEN -1;
	}
	strncpy(tmp, inStr, len);
	tmp[len] = '\0';
	strcpy(pVal, "\0");
	t = (char*)strtok(tmp, delim);

	while (t) {
		if (!strcmp(pAttr, t)){
			t = (char*)strtok((void*)0, delim);
			if (t) {
				len = strlen(t);
				if(len >= valBufLen)
				{
					len = valBufLen -1;
				}
				strncpy(pVal, t, len);
				pVal[len] = '\0';
			}
			break;
		}

		t = (char*)strtok((void*)0, delim);
	}
}

#define KSEG0_BASE_ADDR	 0x80000000
#define KSEG0_START_ADDR 0x80020000
#define KSEG0_END_ADDR	 0x8fffffff

#define KSEG1_BASE_ADDR	 0xa0000000
#define KSEG1_START_ADDR 0xa0020000
#define KSEG1_END_ADDR	 0xafffffff

#define MAX_DRAM_SIZE	 0x10000000

#ifdef TCSUPPORT_ADDR_MAPPING
extern int highmemConfig(void);
#endif
int do_dramtest(int argc, char* argv[])
{
	int i;
	char pVal[MAX_CMD_LEN] = {0};
	unsigned long addr = 0;
	unsigned long size = 0;
	unsigned long pattern = 0;
	unsigned char wByte = 0;
	unsigned char infinite = 0;
	int isGetAddr = 0, isGetSize = 0, isGetPat = 0, isGetWByte = 0, isInfinite = 0;
	unsigned long totalSize = 0;
	unsigned long kseg0EndAddr = 0, kseg1EndAddr = 0;
	unsigned long endAddr;
#ifdef TCSUPPORT_ADDR_MAPPING
	int highmemFlag = 0;
	int highmemSize = 0;
#endif
	dramTest_info_t info;


	totalSize = calculate_dram_size();
	printf("totalSize=%d MB ",totalSize);
#ifdef TCSUPPORT_ADDR_MAPPING
	if(totalSize >= 448){
		highmemSize = (totalSize - 448)<<20;
		totalSize = 448; // 20130130 pork added: for address mapping
		highmemFlag = 1;
	}
#endif
	totalSize = totalSize << 20;
	printf("0x%x  \n",totalSize);
	kseg0EndAddr = 0x80000000 + totalSize - 1;
	kseg1EndAddr = 0xa0000000 + totalSize - 1;

	printf("argc=%d \n",argc);
	if(argc > 1)
	{
		for(i=1; i<argc; i++)
		{
			if(!isGetAddr)
			{
				getAttrVal(argv[i], "a", "=", pVal, MAX_CMD_LEN);
				if(strlen(pVal))
				{
					addr = strtoul((const char*)pVal, (char **)NULL, 16);
					addr = addr & 0xfffffffc;
					if((addr >= KSEG0_START_ADDR && addr <= kseg0EndAddr) || \
							(addr >= KSEG1_START_ADDR && addr <= kseg1EndAddr))
					{
						isGetAddr = 1;
						printf("addr=0x%x \n", addr);
					}
					continue;
				}
			}

			if(!isGetSize)
			{
				getAttrVal(argv[i], "s", "=", pVal, MAX_CMD_LEN);
				if(strlen(pVal))
				{
					size = strtoul((const char*)pVal, (char **)NULL, 16);
					if(isGetAddr)
					{
						endAddr = addr + size;
					}
					else{
						endAddr = KSEG0_START_ADDR + size;
					}
					if((endAddr >= KSEG0_START_ADDR && endAddr <= kseg0EndAddr) || \
							(endAddr >= KSEG1_START_ADDR && endAddr <= kseg1EndAddr))
					{
						isGetSize = 1;
						printf("size=0x%x \n", size);
					}
					continue;
				}
			}

			if(!isGetPat)
			{
				getAttrVal(argv[i], "p", "=", pVal, MAX_CMD_LEN);
				if(strlen(pVal))
				{
					pattern = strtoul((const char*)pVal, (char **)NULL, 16);
					isGetPat = 1;
					printf("pat=0x%x \n", pattern);
					continue;
				}
			}

			if(!isGetWByte)
			{
				getAttrVal(argv[i], "w", "=", pVal, MAX_CMD_LEN);
				if(strlen(pVal))
				{
					wByte = strtoul((const char*)pVal, (char **)NULL, 10);
					if((wByte == 1) || (wByte == 2) || (wByte == 4))
					{
						isGetWByte = 1;
						printf("wByte=%d \n", wByte);
					}
					continue;
				}
			}

			if(!isInfinite)
			{
				getAttrVal(argv[i], "i", "=", pVal, MAX_CMD_LEN);
				if(strlen(pVal))
				{
					infinite = strtoul((const char*)pVal, (char **)NULL, 10);
					isInfinite = 1;
					printf("infinite=%d \n", infinite);
					continue;
				}
			}
		}
	}

	if(isGetAddr)
	{
		if(!isGetSize)
		{
			if(addr >= KSEG0_START_ADDR && addr <= kseg0EndAddr)
			{
				size = kseg0EndAddr - addr + 1;
			}
			else if(addr >= KSEG1_START_ADDR && addr <= kseg1EndAddr)
			{
				size = kseg1EndAddr - addr + 1;
			}
		}


		info.startAddr = addr;
		info.size = size;
		info.pattern = pattern;
		info.wByte = wByte;
		if(infinite == 0)
		{
			printf("1TEST <addr=0x%x> <size=0x%x> <wByte=%d> \n", addr, size, wByte);
			dramTest(&info, isGetPat);
		}
		else
		{
			while(1)
			{
				printf("1TEST <addr=0x%x> <size=0x%x> <wByte=%d> \n", addr, size, wByte);
				if(dramTest(&info, isGetPat) != 0)
				{
					return 0;
				}
			}
		}
	}
	else
	{
		if(!isGetSize)
		{
			size = totalSize - 0x20000;
		}

		info.size = size;
		info.pattern = pattern;
		info.wByte = wByte;

		if(infinite == 0)
		{
			printf("2TEST <addr=0x%x> <size=0x%x> <wByte=%d> \n", KSEG0_START_ADDR, size, wByte);
			info.startAddr = KSEG0_START_ADDR;
			dramTest(&info, isGetPat);
			printf("2TEST <addr=0x%x> <size=0x%x> <wByte=%d> \n", KSEG1_START_ADDR, size, wByte);
			info.startAddr = KSEG1_START_ADDR;
			dramTest(&info, isGetPat);
		}
		else
		{
			while(1)
			{
#ifdef TCSUPPORT_ADDR_MAPPING
				if(highmemFlag){
					info.size = highmemSize;
					info.startAddr = 0xd0000000;
					printf("2TEST <Highmem size:0x%x> <wByte=%d> \n",info.size,wByte);
					highmemConfig();
					if(dramTest(&info, isGetPat) != 0)
						return 0;
				}
				info.startAddr = KSEG0_START_ADDR;
				info.size = size;
				printf("2TEST <addr=0x%x> <size=0x%x> <wByte=%d> \n", KSEG0_START_ADDR, info.size, wByte);
#else
				printf("2TEST <addr=0x%x> <size=0x%x> <wByte=%d> \n", KSEG0_START_ADDR, size, wByte);
				info.startAddr = KSEG0_START_ADDR;
#endif
				if(dramTest(&info, isGetPat) != 0)
				{
					return 0;
				}
				printf("2TEST <addr=0x%x> <size=0x%x> <wByte=%d> \n", KSEG1_START_ADDR, info.size, wByte);
				info.startAddr = KSEG1_START_ADDR;
				if(dramTest(&info, isGetPat) != 0)
				{
					return 0;
				}
			}
		}
	}

	return 0;
}

int do_flashtest(int argc, char* argv[])
{
	int i;
	char pVal[MAX_CMD_LEN] = {0};
	unsigned long addr = 0;
	unsigned long size = 0;
	unsigned long pattern = 0;
	unsigned char infinite = 0;
	int isGetAddr = 0, isGetSize = 0, isGetPat = 0, isInfinite = 0;
	unsigned long totalSize = 0;
	unsigned long flashEndAddr = 0;
	unsigned long endAddr;
	spiFlashTest_info_t info;

	totalSize = spiflash_sizeGet();
	if(totalSize == -1)
	{
		return 0;
	}
	printf("totalSize=0x%x %dMB \n", totalSize, totalSize>> 20);
	flashEndAddr = totalSize - 1;

	printf("argc=%d \n",argc);
	if(argc > 1)
	{
		for(i=1; i<argc; i++)
		{
			if(!isGetAddr)
			{
				getAttrVal(argv[i], "a", "=", pVal, MAX_CMD_LEN);
				if(strlen(pVal))
				{
					addr = strtoul((const char*)pVal, (char **)NULL, 16);
					addr = addr & 0xffff0000;
					if((addr >= FLASH_START_TEST && addr <= flashEndAddr))
					{
						isGetAddr = 1;
						printf("addr=0x%x \n", addr);
					}
					continue;
				}
			}

			if(!isGetSize)
			{
				getAttrVal(argv[i], "s", "=", pVal, MAX_CMD_LEN);
				if(strlen(pVal))
				{
					size = strtoul((const char*)pVal, (char **)NULL, 16);
					size = size & 0xffff0000;
					if(isGetAddr)
					{
						endAddr = addr + size;
					}
					else{
						endAddr = FLASH_START_TEST + size;
					}
					if((endAddr >= FLASH_START_TEST && endAddr <= flashEndAddr))
					{
						isGetSize = 1;
						printf("size=0x%x \n", size);
					}
					continue;
				}
			}

			if(!isGetPat)
			{
				getAttrVal(argv[i], "p", "=", pVal, MAX_CMD_LEN);
				if(strlen(pVal))
				{
					pattern = strtoul((const char*)pVal, (char **)NULL, 16);
					isGetPat = 1;
					printf("pat=0x%x \n", pattern);
					continue;
				}
			}

			if(!isInfinite)
			{
				getAttrVal(argv[i], "i", "=", pVal, MAX_CMD_LEN);
				if(strlen(pVal))
				{
					infinite = strtoul((const char*)pVal, (char **)NULL, 10);
					isInfinite = 1;
					printf("infinite=%d \n", infinite);
					continue;
				}
			}
		}
	}

	if(isGetAddr)
	{
		if(!isGetSize)
		{
			size = flashEndAddr - addr + 1;
		}
	}
	else
	{
		addr = FLASH_START_TEST;
		if(!isGetSize)
		{
			size = totalSize - 0x20000;
		}
	}

	info.startAddr = addr;
	info.size = size;
	info.pattern = pattern;
	if(infinite == 0)
	{
		printf("SPI FLASH TEST <addr=0x%x> <size=0x%x> \n", addr, size);
		spiFlashTest(&info, isGetPat);
	}
	else
	{
		while(1)
		{
			printf("SPI FLASH TEST <addr=0x%x> <size=0x%x> \n", addr, size);
			if(spiFlashTest(&info, isGetPat) != 0)
			{
				return 0;
			}
		}
	}

	return 0;
}

int do_spiregrw(int argc, char* argv[])
{
#define SPI_REG_BASE		0xbfbc0000
	unsigned long regAddr[10] = {0x04, 0x08 , 0x0c , 0x10, 0x14, 0x18, 0x1c, 0x20, 0x24, 0x28};
	unsigned long val[10] = {0x10f, 0x1ff0ef, 0x9f, 0x1ee0ff, 0xa52, 0x2fe034, 0x1ff0e, 0xfef, 0x2fe03, 0x26ff88};
	unsigned long addr;
	int i;
	unsigned long writeVal, readVal;

	for(i=0; i<10; i++)
	{
		addr = SPI_REG_BASE | regAddr[i];
		writeVal = val[i];

		/*write*/
		tc_outl(addr, writeVal);
		printf("addr=%x val=%x \n",addr, writeVal);
		/*read*/
		readVal = tc_inl(addr);
		printf("readVal=%x\n", readVal);

		if(readVal != writeVal)
		{
			printf("ERROR! addr=0x%x, write val=0x%x, read val=0x%x \n", addr, writeVal, readVal);
		}
	}

	printf("register test finished \n");
	return 0;
}

#define DEF_LENGTH 		0xffff
#define TEST_STAR_ADDR 	0x20000

int gdmaTestCpy(unsigned long sa, unsigned long da, unsigned long len, unsigned char burst_size, unsigned char wswap)
{
	unsigned char ch;
	int counter = 0;

	if (!isMT751020 && !isMT7505) {
		printf("No Support! \n");
		return 0;
	}

	ch = random_byte() & 0xf;
    if (isEN751221)
        ch &= 0x7;
	if (len == 0)
		len = 1;

	// GDMA
	VPint(CR_GDMA_SA(ch)) = sa;
	VPint(CR_GDMA_DA(ch)) = da;
	VPint(CR_GDMA_CT1(ch)) = (wswap<<28) | (1<<2);
//	printf("%X: %X \n", CR_GDMA_CT1(ch), tc_inl(CR_GDMA_CT1(ch)));
	VPint(CR_GDMA_CT0(ch)) = ((len&0xffff)<<16) | (burst_size<<3) | (1<<1) | (1<<0);
//	printf("%X: %X \n", CR_GDMA_CT0(ch), tc_inl(CR_GDMA_CT0(ch)));

	while(counter < 500){
		if(!(VPint(CR_GDMA_DONEINT) & (1<<ch))){
			VPint(CR_GDMA_DONEINT) = (1<<ch);
			break;
		}
		counter++;
	}

	printf("ch=%d burst_size=%d wswap=%d ", ch, burst_size, wswap);
	printf("gdmaTestCpy sa=0x%x da=0x%x len=0x%x\n", sa, da, (len&0xffff));

	return 0;
}

int do_gdmatestdram(int argc, char* argv[])
{

	char pVal[MAX_CMD_LEN] = {0};
	unsigned char infinite = 0;
	gdmaTest_info_t info;
	unsigned long dramSize = 0;
	unsigned char burst_size;


	if (!isMT751020 && !isMT7505) {
		printf("No Support! \n");
		return 0;
	}

	dramSize = calculate_dram_size();
	printf("dramSize=%d MB ",dramSize);
	dramSize = dramSize << 20;
	printf("0x%x  \n",dramSize);

	getAttrVal(argv[1], "i", "=", pVal, MAX_CMD_LEN);
	if(strlen(pVal))
	{
		infinite = strtoul((const char*)pVal, (char **)NULL, 10);
		printf("infinite=%d \n\n", infinite);
	}

	do
	{
		info.sa = (random_word() % (dramSize - TEST_STAR_ADDR)) + TEST_STAR_ADDR;
		info.da = info.sa + DEF_LENGTH + 1;
		if(info.da >= dramSize){
			info.sa = TEST_STAR_ADDR;
			info.da = info.sa + DEF_LENGTH + 1;
		}
		info.len = DEF_LENGTH;
		info.wswap = 0;

		for(burst_size=0; burst_size<=4; burst_size++)
		{
			info.burst_size = burst_size;
			if(gdmaTestDram(&info) != 0)
			{
				return 0;
			}
		}
		printf("*********\n");
	}while(infinite);

	return 0;
}


int do_gdmatestflash(int argc, char* argv[])
{
	char pVal[MAX_CMD_LEN] = {0};
	unsigned char infinite = 0;
	gdmaTest_info_t info;
	unsigned long flashSize = 0;
	unsigned long len = 0;
	unsigned char burst_size;
	unsigned long addr = 0;
	unsigned long flashPhyBaseAddrBlock1 = 0x1c000000;

	if (!isMT751020 && !isMT7505) {
		printf("No Support! \n");
		return 0;
	}

	flashSize = spiflash_sizeGet();
	if(flashSize == -1)
	{
		return 0;
	}
	printf("flashSize=0x%x %dMB \n", flashSize, flashSize>> 20);

	getAttrVal(argv[1], "i", "=", pVal, MAX_CMD_LEN);
	if(strlen(pVal))
	{
		infinite = strtoul((const char*)pVal, (char **)NULL, 10);
		printf("infinite=%d \n\n", infinite);
	}

	do
	{
		addr = (random_word() % (flashSize - FLASH_START_TEST)) + FLASH_START_TEST;
		addr &= ~(0xffff);
		if((addr + DEF_LENGTH) >= flashSize)
		{
			addr = FLASH_START_TEST;
		}
		info.sa = addr + flashPhyBaseAddrBlock1; //flash phy addr
		info.da = addr; 					//dram phy addr
		info.len = DEF_LENGTH & ~(0x3);
		info.wswap = 1;

		for(burst_size=0; burst_size<=4; burst_size++)
		{
			info.burst_size = burst_size;
			if(gdmaTestFlash(&info) != 0)
			{
				return 0;
			}
		}
		printf("#########\n");
	}while(infinite);

	return 0;
}


int do_gdmatestreg(int argc, char* argv[])
{
#ifndef TCSUPPORT_CPU_MT7505
#define TEST_NUMS 4
#define KSEG1_START_ADDR 0xa0020000
										/*ptm,      atm,         usb,           pcie,        fe,         pcm,            spi,       */
	unsigned long virAddr[TEST_NUMS] = {0xbfb62004, 0xbfb60004, /*0xbfba0054,*/ 0xbfb80000, 0xbfb50830, 0xbfbd0000, 0xbfbc0028};
	unsigned long val[TEST_NUMS] = {0x2010101,      0x00000003,  /*0x00001001,*/ 0x001000b0, 0xa52,     0xf5071306, 0x78880};
#else
#define TEST_NUMS 4
#define KSEG1_START_ADDR 0xa0020000
										/*ptm,      atm,         usb,           pcie,        fe,         pcm,            spi,       */
	unsigned long virAddr[TEST_NUMS] = {/*0xbfb62004, */0xbfb60004, 0xbfbb0054, 0xbfb00088, 0xbfb50830/*, 0xbfbd0000, 0xbfbc0028*/ };
	unsigned long val[TEST_NUMS] = {/*0x2010101,      */0x00000003,  0x00002000, 0x13c08000, 0xa52/*,     0xf5071306, 0x78880*/};
#endif
	unsigned long *dramAddr;
	int i;
	int ret = 0;
	int len = 0;
	unsigned char burst_size;


	if (!isMT751020 && !isMT7505) {
		printf("No Support! \n");
		return 0;
	}

	dramAddr = KSEG1_START_ADDR;
	len = 4;

	for(burst_size=0; burst_size<=4; burst_size++)
	{
		for(i=0; i<TEST_NUMS; i++)
		{
			printf("index i=%d\n",i);
			/*write*/
			*dramAddr = val[i];
			gdmaTestCpy(TEST_STAR_ADDR, PHYSADDR(virAddr[i]), len, burst_size, 1);
			ret += memcmp(KSEG1_START_ADDR, virAddr[i], len);

			/*read*/
			*dramAddr = 0;
			gdmaTestCpy(PHYSADDR(virAddr[i]), TEST_STAR_ADDR, len, burst_size, 1);
			ret += memcmp(KSEG1_START_ADDR, virAddr[i], len);
			printf("\n\n");
		}
	}

	if(ret != 0)
	{
		printf("gdmatestreg FAIL!\n\n");
	}
	else
	{
		printf("gdmatestreg SUCCESS!\n\n");
	}

	return 0;
}

/**************** memory access range protection *****************/
#if defined(DRAM_PROTECT_TEST)
static unsigned long segAddr[2][2] = { {KSEG0_BASE_ADDR, KSEG0_START_ADDR}, 
  									   {KSEG1_BASE_ADDR, KSEG1_START_ADDR} }; 

int illinfo_record(unsigned long ill_info_reg,unsigned long ill_addr_reg)
{
	int i=0;
	int new_data = 0;

	//different address access
	if(record2[test_round][5] != ill_addr_reg)
	{
		if(ex_addr_count < 1000)
		{
			if(ex_addr_count)
			{
				for(i=0;i<ex_addr_count;i++)
				{
					if(ill_addr_reg != record3_RegAddr[i])
					{
						new_data = 1;
					}
					else
					{
						new_data = 0;
						break;
					}
				}
				if(new_data)
				{
					record3_RegAddr[ex_addr_count] = ill_addr_reg;
					ex_addr_count++;
				}
			}
			else
			{
				record3_RegAddr[ex_addr_count] = ill_addr_reg;
				ex_addr_count++;
			}
		}
	}
	
	//different ill_info
	if(record2[test_round][4] != ill_info_reg)
	{
		if(ex_info_count< 1000)
		{
			if(ex_info_count)
			{
				for(i=0;i<ex_info_count;i++)
				{
					if(ill_info_reg !=  record4_illInfo[i])
					{
						new_data = 1;
					}
					else
					{
						new_data = 0;
						break;
					}
				}
				if(new_data)
				{
					record4_illInfo[ex_info_count] = ill_info_reg;
					ex_info_count++;
				}
			}
			else
			{
				record4_illInfo[ex_info_count] = ill_info_reg;
				ex_info_count++;
			}
		}
	}
	return 1;
}

int dram_access_detect(void)
{
	unsigned long ill_info_reg;
	unsigned long ill_addr_reg;

	
	ill_info_reg = VPint(0xbfa00030);
	if (ill_info_reg & (1<<31)){
		ill_addr_reg = VPint(0xbfa0002c);
		if (ill_info_reg & (1<<30)){
			MP_DBG(DBG_L4, "Invalid dram write interrupt!!\n");
		} else {
			MP_DBG(DBG_L4, "Invalid dram read interrupt!!\n");
		}
		MP_DBG(DBG_L4, "Access invalid dram address %X\n", ill_addr_reg);
		MP_DBG(DBG_L4, "ill info register value: %X\n", ill_info_reg);

		if (record2[test_round][0]==0)
		{
			record2[test_round][0] = 1;
			record2[test_round][5] = ill_addr_reg;
            		record2[test_round][4] = ill_info_reg;
		}
		else
		{
			illinfo_record(ill_info_reg,ill_addr_reg);
		}
		
		if (ill_info_reg & (1<<24)){
			record2[test_round][2] = 1;
		} else {
			record2[test_round][3] = 1;
		}
		VPint(0xbfa00030) = (1<<31);
		return 1;
	} else {
		return 0;
	}
}


void RbusIsr(int irq, void *dev_id, struct pt_regs *regs)
{
	MP_DBG(DBG_L1, "Rbus Isr\n");
	dram_access_detect();
}

#if defined(TCSUPPORT_CPU_EN7521)
int do_mem_protect_dbgLevel(int argc, char *argv[])
{
    mp_dbg_level = strtoul((const char*)(argv[1]), (char **)NULL, 16);
        
    prom_printf("memory protection debug level: 0x%x\n", mp_dbg_level);

    return 0;
}
int do_nmi_test_enable(int argc, char *argv[])
{
    nmi_test_enable = strtoul((const char*)(argv[1]), (char **)NULL, 16);
        
    prom_printf("nmi_test_enable: %d\n", nmi_test_enable);

    return 0;
}
#endif

static int do_rbus_init(int argc, char *argv[])
{
	prom_printf("rbus init\n");
	request_IRQ(RBUS_INT, &rbus_irq, NULL);
	VPint(CR_INTC_IMR) |= (0x01 << RBUS_INT);

	return 0;
}


int do_dram_protect_size(int argc, char* argv[])
{
	unsigned long size;
	size = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	prom_printf("Dram physical size : 0x0 ~ 0x%x\n", (size-1));
	VPint(0xbfa00034) = (size - 1);
	return 0;
}


int do_dram_protect_test1(int argc, char* argv[])
{
	dramTest_info_t info;
	unsigned long startAddr, phy_endAddr, vir_endAddr, randAddr, randSize, addrMask, maxSize;
	unsigned long dramSize, counter, isUnCache, dmaEn, i;
	
	counter = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	isUnCache = strtoul((const char*)(argv[2]), (char **)NULL, 16);
	dmaEn = strtoul((const char*)(argv[3]), (char **)NULL, 16);

	if (isUnCache)
		isUnCache = 1;

	if (dmaEn)
		dmaEn = 1;

	/* dram end addresss - dram start address + 1 */
	phy_endAddr = (VPint(0xbfa00034));
	dramSize = phy_endAddr - 0 + 1;
	
	MP_DBG(DBG_L1, "Dram protect test case 1\n");
	MP_DBG(DBG_L1, "Test physical address between 0x20000 ~ 0x%x\n", (dramSize - 1));
	MP_DBG(DBG_L1, "Test virtual address between 0x%x ~ 0x%x\n", segAddr[isUnCache][1], 
															 (segAddr[isUnCache][1] + dramSize - 0x20001));
	MP_DBG(DBG_L1, "Memory Read/Write Test by %s\n", (dmaEn == 1) ? "GDMA" : "CPU");
	MP_DBG(DBG_L1, "Test total round: %x\n", counter);
	MP_DBG(DBG_L1, "========================================================================\n");

	addrMask = (dramSize - 1);
	MP_DBG(DBG_L1, "addrMask: %x\n", addrMask);

	for (i=0; i<counter; i++){
		
		randSize = ( (random_word()) & addrMask );

		if (randSize > (dramSize - 4)){
			randSize = dramSize - 4;
		}

		randSize -= (randSize % 4);

    #if defined(TCSUPPORT_CPU_EN7521) 
	if (isEN7526c)
	{
	        /* FPFA's DRAM is 128M and only its 64M is initialized as Memory use(by HW TRAP). In order not
	         * to be affected during tests, test code operates on 64M ~ 128M */
	        if (dmaEn) /* GDMA will use DRAMTESTOFFSET0 & 1 to move data, so don't operate on these areas */
	            info.startAddr = 0x4070000 + ((random_word()) % (dramSize-0x4070000));
	        else
	            info.startAddr = 0x4000000 + ((random_word()) % (dramSize-0x4000000));

	        info.startAddr += segAddr[isUnCache][0];
	        info.startAddr -= (info.startAddr % 4);
	        maxSize = 0xffff;
	}
    #else
		/*Since 0x0~0x20000 is for bootloader, so startAddr starts from 0x20000*/
		if (randSize > 0x20000){
			info.startAddr = segAddr[isUnCache][0] + randSize;
		} else {
			info.startAddr = segAddr[isUnCache][1];
		}

        maxSize = 0x1000;
    #endif

		// random select test size
		info.size = ((random_word()) % maxSize);
		if (info.size == 0){
			info.size = maxSize;
		} 
	
		if ((info.size + info.startAddr) > (segAddr[isUnCache][0] + dramSize)){
			info.size = (segAddr[isUnCache][0] + dramSize - info.startAddr);
		}

		/* 4 byte alignment */
		info.size -= (info.size % 4);

		MP_DBG(DBG_L1, "Test round: %x\n", i);
		MP_DBG(DBG_L1, "Dram start address: %x\n", info.startAddr);
		MP_DBG(DBG_L1, "Dram test size: %x\n", info.size);

		info.wByte = 0;
	
		if (dmaEn){
			dramTest2(&info, 0);
		} else {
			if (dramTest(&info, 0))
               prom_printf("\nRound %d FAIL !\n", i); 
		}
	}

    prom_printf("\n%d Test Rounds finished !\n", counter);

	return 0;
}

int do_dram_protect_test2(int argc, char* argv[])
{
	dramTest_info_t info;
	unsigned long startAddr, phy_endAddr, vir_endAddr, randAddr, randSize, addrMask, minSize, randSize_2;
	unsigned long dramSize, counter, isUnCache, dmaEn, i;

	counter = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	isUnCache = strtoul((const char*)(argv[2]), (char **)NULL, 16);
	dmaEn = strtoul((const char*)(argv[3]), (char **)NULL, 16);

	if (isUnCache)
		isUnCache = 1;

	if (dmaEn)
		dmaEn = 1;

	/* dram end addresss - dram start address + 1 */
	phy_endAddr = (VPint(0xbfa00034));
	dramSize = phy_endAddr - 0 + 1;
	
	MP_DBG(DBG_L1, "Dram protect test case 2\n");

	MP_DBG(DBG_L1, "Test physical start address between 0x20000 ~ 0x%x\n", (dramSize - 1));
	MP_DBG(DBG_L1, "Test physical end address over 0x%x\n", (dramSize - 1));

	MP_DBG(DBG_L1, "Test virtual start address between 0x%x ~ 0x%x\n", segAddr[isUnCache][1], 
									   						       (segAddr[isUnCache][1] + dramSize - 0x20001));

	MP_DBG(DBG_L1, "Test virtual end address over 0x%x\n", (segAddr[isUnCache][1] + dramSize - 0x20001));

	MP_DBG(DBG_L1, "Test total round: %x\n", counter);
	MP_DBG(DBG_L1, "========================================================================\n");

	addrMask = (dramSize - 1);
	MP_DBG(DBG_L1, "addrMask: %x\n", addrMask);

	for (i=0; i<counter; i++){
    #if defined(TCSUPPORT_CPU_EN7521)
	if(isEN7526c)
	{
	        info.startAddr = (segAddr[isUnCache][0] + dramSize - 0xf000);
	        /* GDMA lenth has only 16 bites, so  randSize or randSize_2 can't exceed 0xffff */
	        randSize = ((random_word()) & 0xf ) +1 +0xf000; /* 1~16 Byte for cpu uncached access*/
	        randSize_2 = ((random_word()) & 0xff ) +1 +0xf000; /* 1~256 Byte for gdma access*/
	}
    #else
		startAddr = (segAddr[isUnCache][0] + dramSize - 0x10000);
		randSize = ( (random_word()) & 0xfff ) + 4;
		randSize -= (randSize % 4);
		info.startAddr = startAddr + randSize;
		minSize = 0x10000 - randSize;
		randSize = minSize + ((random_word()) % 0x1000) + 4;
		/* 4 byte alignment */
		randSize -= (randSize % 4);
    #endif

		info.size = randSize;
		info.wByte = 0;
	
		MP_DBG(DBG_L1, "Test round: %x\n", i);
		MP_DBG(DBG_L1, "Dram start address: %x\n", info.startAddr);

		if (dmaEn){
			if (info.size >= 0x10000){
				info.size = 0xfffc;
			}

        #if defined(TCSUPPORT_CPU_EN7521)
		if(isEN7526c)
		{
            		info.size = randSize_2;
		}
        #endif
			MP_DBG(DBG_L1, "Dram test size: %x\n", info.size);

			dramTest2(&info, 0);
		} else {
        #if defined(TCSUPPORT_CPU_EN7521)
		if(isEN7526c)
		{
            		info.size = randSize;
		}
        #endif
			MP_DBG(DBG_L1, "Dram test size: %x\n", info.size);

			dramTest(&info, 0);
		}
	}
	return 0;
}

int do_dram_protect_test3(int argc, char* argv[])
{
	dramTest_info_t info;
	unsigned long startAddr, phy_endAddr, vir_endAddr, randAddr, randSize, addrMask, maxSize;
	unsigned long dramSize, counter, isUnCache, i;

	counter = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	isUnCache = strtoul((const char*)(argv[2]), (char **)NULL, 16);

	if (isUnCache)
		isUnCache = 1;

	/* dram end addresss - dram start address + 1 */
	phy_endAddr = (VPint(0xbfa00034));
	dramSize = phy_endAddr - 0 + 1;
	
	prom_printf("Dram protect test case 3\n");

	prom_printf("Test invalid physical start address\n");
	prom_printf("Test physical end address between 0x0 ~ 0x%x\n", (dramSize - 1 - 0x40000));

	prom_printf("Test invalid virtual start address between 0x%x ~ 0x%x\n", segAddr[isUnCache][0] - 0x20000,
																			segAddr[isUnCache][0] );

	prom_printf("Test virtual address between 0x%x ~ 0x%x\n", segAddr[isUnCache][0] - 0x20000, 
															 (segAddr[isUnCache][0] + dramSize - 0x40001));

	prom_printf("Test total round: %x\n", counter);
	prom_printf("========================================================================\n");

	addrMask = (dramSize - 1);
	prom_printf("addrMask: %x\n", addrMask);

	for (i=0; i<counter; i++){
		randSize = ( (random_word()) & 0x1ffff );
		randSize -= (randSize % 4);
		prom_printf("randSize1 %x\n", randSize);

		startAddr =  segAddr[isUnCache][0] - 0x20000 + randSize;

		prom_printf("startAddr1 %x\n", startAddr);
#if 1
		randSize = (0x20000 - randSize) + ((random_word()) & 0xfff);
		/* 4 byte alignment */
		randSize -= (randSize % 4);
#endif
		prom_printf("randSize1 %x\n", randSize);
		prom_printf("Test round: %x\n", i);
		prom_printf("Dram start address: %x\n", startAddr);
		prom_printf("Dram test size: %x\n", randSize);

		info.startAddr = startAddr;
		info.size = randSize;
		info.wByte = 0;
	
		dramTest(&info, 0);
	}

	return 0;
}

int do_dram_protect_test4(int argc, char* argv[])
{
	dramTest_info_t info;
	unsigned long startAddr, phy_endAddr, vir_endAddr, randAddr, randSize, addrMask, maxSize;
	unsigned long dramSize, counter, isUnCache, dmaEn, i;

	counter = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	isUnCache = strtoul((const char*)(argv[2]), (char **)NULL, 16);
	dmaEn = strtoul((const char*)(argv[3]), (char **)NULL, 16);

	if (isUnCache)
		isUnCache = 1;

	if (dmaEn)
		dmaEn = 1;

	/* dram end addresss - dram start address + 1 */
	phy_endAddr = (VPint(0xbfa00034));
	dramSize = phy_endAddr - 0 + 1;
	
	prom_printf("Dram protect test case 4\n");

	prom_printf("Test physical start address between 0x20000 ~ 0x%x\n", (dramSize - 1));
	prom_printf("Test physical end address over 0x%x\n", (dramSize - 1));

	prom_printf("Test virtual start address between 0x%x ~ 0x%x\n", segAddr[isUnCache][1], 
									   						       (segAddr[isUnCache][1] + dramSize - 0x20001));

	prom_printf("Test virtual end address over 0x%x\n", (segAddr[isUnCache][1] + dramSize - 0x20001));

	prom_printf("Test total round: %x\n", counter);
	prom_printf("========================================================================\n");

	addrMask = (dramSize - 1);
	prom_printf("addrMask: %x\n", addrMask);

	randSize = ( (random_word()) & addrMask );
	randSize -= (randSize % 4);

	if (randSize > (dramSize - 4)){
		randSize = dramSize - 4;
	}

	for (i=0; i<counter; i++){
        #if defined(TCSUPPORT_CPU_EN7521)
		if(isEN7526c)
		{
	            info.startAddr = segAddr[isUnCache][0] + dramSize + ((random_word()) & addrMask );
	            info.startAddr -= (info.startAddr % 4); 
	            if (dmaEn)
	                info.size =  ((random_word()) & 0xff) + 1;
	            else
	    			info.size =  ((random_word()) & 0xf) + 1;
	            info.size -= (info.size % 4);
		}
        #else
			info.startAddr = segAddr[isUnCache][0] + dramSize + randSize;
			info.size =  ( (random_word()) & 0xff ) + 4;
        #endif
        
            info.size -= (info.size % 4);
            if (info.size == 0)
                info.size = 4;
			info.wByte = 0;

			prom_printf("Test round: %x\n", i);
			prom_printf("Dram start address: %x\n", info.startAddr);
			prom_printf("Dram test size: %x\n", info.size);
	
		if (dmaEn){
			dramTest2(&info, 0);
		} else {
			dramTest(&info, 0);
		}
	}
	return 0;
}

static int do_seg_write_protect(int argc, char *argv[])
{
	unsigned long enable;

	enable = strtoul((const char*)(argv[1]), (char **)NULL, 16);

	if (enable)
		enable = 1;

	prom_printf("Segmentation write protect: %s\n", (enable==1) ? "enable" : "disable");

	if (enable){
		VPint(0xbfa00018) |= ((1<<1) | (1<<3));
	} else {
		VPint(0xbfa00018) &= (~((1<<1) | (1<<3)));
	}

	return 0;
}

static int do_seg_read_protect(int argc, char *argv[])
{
	unsigned long enable;

	enable = strtoul((const char*)(argv[1]), (char **)NULL, 16);

	if (enable)
		enable = 1;

	prom_printf("Segmentation read protect: %s\n", (enable==1) ? "enable" : "disable");

	if (enable){
		VPint(0xbfa00018) |= ((1<<2) | (1<<4));
	} else {
		VPint(0xbfa00018) &= (~((1<<2) | (1<<4)));
	}

	return 0;
}

static int do_seg_addr_set(int argc, char *argv[])
{
	unsigned long seg, startAddr, endAddr;

	seg = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	startAddr = strtoul((const char*)(argv[2]), (char **)NULL, 16);
	endAddr = strtoul((const char*)(argv[3]), (char **)NULL, 16);

	if (seg) 
		seg = 1;

	prom_printf("Segmentation%s: \n", (seg==1) ? "2" : "1");
	prom_printf("Start address:%x\n", startAddr);
	prom_printf("End address:%x\n",   endAddr);

	if (seg){
		VPint(0xbfa00024) = startAddr;
		VPint(0xbfa00028) = endAddr;
	} else {
		VPint(0xbfa0001c) = startAddr;
		VPint(0xbfa00020) = endAddr;
	}
}

static int do_seg_protect_test(int argc, char *argv[])
{
	dramTest_info_t info;
	unsigned long phy_endAddr, randAddr, randSize, addrMask, segStart, segEnd;
	unsigned long dramSize, counter, isUnCache, dmaEn, seg, i;
	unsigned long startAddr, testCase;
    unsigned long segStartMin;

	counter = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	seg = strtoul((const char*)(argv[2]), (char **)NULL, 16);
	isUnCache = strtoul((const char*)(argv[3]), (char **)NULL, 16);
	dmaEn = strtoul((const char*)(argv[4]), (char **)NULL, 16);
	testCase = strtoul((const char*)(argv[5]), (char **)NULL, 16);

	if (seg)
		seg = 1;

	if (isUnCache)
		isUnCache = 1;

	if (dmaEn)
		dmaEn = 1;

	phy_endAddr = (VPint(0xbfa00034));
	dramSize = phy_endAddr - 0 + 1;
	
	MP_DBG(DBG_L1, "Segmentation protect test case 1\n");
	MP_DBG(DBG_L1, "Valid physical address: 0x0 ~ 0x%x\n", (dramSize - 1));
	MP_DBG(DBG_L1, "Test Segmentation%s\n", (seg==1) ? "2" : "1");

	if (seg){
		segStart = VPint(0xbfa00024);
		segEnd = VPint(0xbfa00028);
	} else {
		segStart = VPint(0xbfa0001c);
		segEnd = VPint(0xbfa00020);
	}

#if defined(TCSUPPORT_CPU_EN7521)
	if(isEN7526c)
	{
	    if (VPint(0xbfa00024) > VPint(0xbfa0001c))
	        segStartMin = VPint(0xbfa0001c);
	    else
	        segStartMin = VPint(0xbfa00024);
	}
#endif

	if (segStart < 4){
		prom_printf("segStart address is too small to test\n");
		return -1;
	}

	if (segEnd > (dramSize - 4)){
		prom_printf("segEnd address is too large to test\n");
		return -1;
	}

	MP_DBG(DBG_L1, "Segmentation Start Addr: %x, End Addr: %x\n", segStart, segEnd);

	MP_DBG(DBG_L1, "Test total round: %x\n", counter);
	MP_DBG(DBG_L1, "========================================================================\n");

	addrMask = (dramSize - 1);
	MP_DBG(DBG_L1, "addrMask: %x\n", addrMask);

	for (i=0; i<counter; i++){
		switch(testCase)
		{
			case 0:
            #if defined(TCSUPPORT_CPU_EN7521)
			if(isEN7526c)
			{
		                /* FPFA's DRAM is 128M and only its 64M is initialized as Memory use(by HW TRAP). 
		                 * In order not to be affected during tests, test code operates above 64M */
		                if (dmaEn) /* DRAMTESTOFFSET0 & 1 areas will be used to move data during GDMA tests, so don't operate on these areas */ 
		                    startAddr = (random_word() % (segStartMin-((DRAMTESTOFFSET1+0x1000)&0xfffffff))) + (DRAMTESTOFFSET1+0x1000);
		                else
		                    startAddr = (random_word() % (segStartMin-0x4000000)) + 0x4000000;
		                if (startAddr >= (segStartMin-8))
		                    startAddr = (segStartMin-8);
		                /* CPU word access needs to be word alignment */
		                startAddr -= (startAddr%4);

		                info.size = ((random_word()) & 0xfff);
		                if (info.size < 4)
		                    info.size = 4;
		                if (startAddr + info.size >= segStartMin)
		                   info.size = segStartMin - startAddr - 4;
			}
            #else
				startAddr = segStart - (random_word() & 0xfff);
				startAddr -= (startAddr%4);
				startAddr = (startAddr<0) ? 0 : startAddr;
				info.size = ((random_word() % (segStart - startAddr)) & 0xfff);
            #endif
				break;

			case 1:
            #if defined(TCSUPPORT_CPU_EN7521)
			if(isEN7526c)
			{
	                startAddr = (segStart - 0xf000);
	                if (dmaEn)
	                    info.size = ((random_word()) & 0xff ) +1 +0xf000; /* 1~256 Byte for gdma access*/   
	                else
	                    info.size = ((random_word()) & 0xf ) +1 +0xf000; /* 1~16 Byte for cpu uncached access*/

	                info.size -= (info.size % 4);
	                if (startAddr + info.size <= segStart)
	                    info.size += segStart - (startAddr + info.size) + 4;
			}
            #else
				startAddr = segStart - ((random_word()) & 0xfff);
				startAddr -= (startAddr%4);
				startAddr = (startAddr<0) ? 0 : startAddr;
				info.size = (segStart - startAddr) + (random_word() & 0xfff);
            #endif
				break;

			case 2:
            #if defined(TCSUPPORT_CPU_EN7521)
		if(isEN7526c)
		{
	                startAddr = (random_word() % (segEnd-segStart)) + segStart;
	                startAddr -= (startAddr%4);
	                if (dmaEn)
	                    info.size = ((random_word()) & 0xff ) +1; /* 1~256 Byte for cpu uncached access*/
	                else
	                    info.size = ((random_word()) & 0xf ) +1; /* 1~16 Byte for cpu uncached access*/ 
	                
	                info.size -= (info.size%4);
	                if (startAddr + info.size >= segEnd)
	                   info.size -= (startAddr + info.size) - segEnd + 4; 
	                if (info.size == 0)
	                    info.size = 4;
		}
            #else
				startAddr = segStart - ((random_word()) & 0xfff);
				startAddr -= (startAddr%4);
				startAddr = (startAddr<0) ? 0 : startAddr;
				info.size = (segEnd - startAddr) + (random_word() & 0xfff);
            #endif
				break;

			case 3:
            #if defined(TCSUPPORT_CPU_EN7521)
			if(isEN7526c)
			{
		                if (dmaEn)
		                    startAddr = segEnd - (((random_word()) & 0xff ) +1);
		                else
		                    startAddr = segEnd - (((random_word()) & 0xf ) +1); 

		                startAddr -= (startAddr%4);
		                info.size = (segEnd - startAddr) + (random_word() & 0xfff);
		                if (startAddr+info.size <= segEnd)
		                    info.size += segEnd - (startAddr+info.size)+4;
			}
            #else
				startAddr = segStart + ((random_word()) % (segEnd - segStart));
				startAddr -= (startAddr%4);
				startAddr = (startAddr<0) ? 0 : startAddr;
				info.size = (segEnd - startAddr) + (random_word() & 0xfff);
            #endif
				break;
			case 4:
            #if defined(TCSUPPORT_CPU_EN7521)
			if(isEN7526c)
			{
		                if (dmaEn) {
		                    startAddr = segStart - (segStart % 0x10);
		                    info.size = (segEnd-startAddr) + ((random_word())& 0xff) + 4;
		                }
		                else {
		                    startAddr = segStart - (segStart%4);
		                    info.size = (segEnd-startAddr) + ((random_word())& 0xf) + 4;
		                    info.size += (info.size%4);
		                }
			}
            #else
				startAddr = segStart + (random_word() % (segEnd - segStart));
				startAddr -= (startAddr%4);
				startAddr = (startAddr<0) ? 0 : startAddr;
				info.size = ((random_word()%(segEnd - startAddr)) & 0xfff);
            #endif
				break;
			default:
				break;
		}

		info.startAddr = segAddr[isUnCache][0] + startAddr;
		info.size -= (info.size%4);

		info.wByte = 0;
		MP_DBG(DBG_L1, "Test round: %x\n", i);
		MP_DBG(DBG_L1, "Dram start address: %x\n", info.startAddr);
		MP_DBG(DBG_L1, "Dram test size: %x\n", info.size);

		if (dmaEn){
			dramTest2(&info, 0);
		} else {
			dramTest(&info, 0);
		}
	}

    prom_printf("\n%d Test Rounds finished !\n", counter);

	return 0;
}

static int do_mem_protect_test(int argc, char *argv[])
{
	dramTest_info_t info;
	char flag = 0;
	unsigned long phy_endAddr, randAddr, randSize, addrMask, segStart, segEnd;
	unsigned long dramSize, counter, isUnCache, dmaEn, seg, i;
	unsigned long startAddr, testCase;
	unsigned long seg1Start, seg1End, seg2Start, seg2End, segStartMin;

	counter = strtoul((const char*)(argv[1]), (char **)NULL, 10);
	isUnCache = strtoul((const char*)(argv[2]), (char **)NULL, 16);
	dmaEn = strtoul((const char*)(argv[3]), (char **)NULL, 16);

	if (isUnCache)
		isUnCache = 1;

	if (dmaEn)
		dmaEn = 1;

	phy_endAddr = (VPint(0xbfa00034));
	dramSize = phy_endAddr - 0 + 1;
	
	MP_DBG(DBG_L1, "Memory Protect Test\n");
	MP_DBG(DBG_L1, "Valid physical address: 0x0 ~ 0x%x\n", (dramSize - 1));

	addrMask = (dramSize - 1);
	MP_DBG(DBG_L1, "addrMask: %x\n", addrMask);

	seg1Start = (VPint(0xbfa0001c));
	seg1End = (VPint(0xbfa00020));
	seg2Start = (VPint(0xbfa00024));
	seg2End = (VPint(0xbfa00028));

#if defined(TCSUPPORT_CPU_EN7521)
	if(isEN7526c)
	{
	    if (seg1Start > seg2Start)
	        segStartMin = seg2Start;
	    else
	        segStartMin = seg1Start;
	}
#endif

	for (test_round=0; test_round<counter; test_round++){
		for (i=0; i<6; i++){
			record1[test_round][i] = 0;
			record2[test_round][i] = 0;
		}
	}

	for (test_round=0; test_round<counter; test_round++){
    #if defined(TCSUPPORT_CPU_EN7521)
	if(isEN7526c)
	{
		startAddr = 0x60000 + (random_word() % (dramSize-0x60000+0x800000));
		startAddr -= (startAddr%4);
	       info.startAddr = segAddr[isUnCache][0] + startAddr;
	       if (dmaEn){
	            info.size = (random_word() & 0xff)+1;
	       }
	       else{
	            info.size = (random_word() & 0xf)+1;
    		}
	}
    #else
		startAddr = (random_word() & addrMask);
		startAddr -= (startAddr%4);
		if (startAddr > (dramSize - 0xff))
			startAddr -= 0x100;
		if (startAddr < 0x40000)
			startAddr += 0x40000;
		info.startAddr = segAddr[isUnCache][1] + startAddr;
		info.size = (random_word() & 0xff);
        	startAddr += 0x20000
    #endif
		info.size -= (info.size % 4);
	
		if(!info.size)
			info.size = 0x4;
		info.wByte = 0;

		MP_DBG(DBG_L1, "Test round: %d\n", test_round);
		MP_DBG(DBG_L1, "Dram start address: %x\n", info.startAddr);
		MP_DBG(DBG_L1, "Dram test size: %x\n", info.size);

		record1[test_round][0] = startAddr;
		record1[test_round][1] = info.size;
		
		if ((startAddr + info.size) > phy_endAddr)
			record1[test_round][2] = 1;

		if ((startAddr>=seg1Start) && (startAddr<=seg1End))
			record1[test_round][3] = 1;

		if (((startAddr+info.size)>=seg1Start) && ((startAddr+info.size)<=seg1End))
			record1[test_round][3] = 1;

		if ((startAddr>=seg2Start) && (startAddr<=seg2End))
			record1[test_round][3] = 1;

		if (((startAddr+info.size)>=seg2Start) && ((startAddr+info.size)<=seg2End))
			record1[test_round][3] = 1;

		if (dmaEn){
			dramTest2(&info, 0);
		} else {
			dramTest(&info, 0);
		}
	}

	MP_DBG(DBG_L3, "seg1: %x ~ %x\n", seg1Start, seg1End);
	MP_DBG(DBG_L3, "seg2: %x ~ %x\n", seg2Start, seg2End);
    MP_DBG(DBG_L3, "Dram Size: %x\n\n", phy_endAddr);

	for (i=0; i<counter; i++){
		MP_DBG(DBG_L3, "test round: %d\n", i);
		MP_DBG(DBG_L3, "test startAddr : %x\n",      record1[i][0]);
        MP_DBG(DBG_L3, "test size      : %x\n",      record1[i][1]);
		MP_DBG(DBG_L3, "expected result: overdram-> %d seg_err-> %d\n",  record1[i][2], record1[i][3]);	
		MP_DBG(DBG_L3, "testing result:  overdram-> %d seg_err-> %d\n",  record2[i][2], record2[i][3]);	
		if (record2[i][2] || record2[i][3]){
			MP_DBG(DBG_L3, "invalid address : %x\n", record2[i][5]);
            MP_DBG(DBG_L3, "invalid info    : %x\n", record2[i][4]);
		}

		if ((record1[i][2] != record2[i][2]) || (record1[i][3] != record2[i][3])) {
			flag = 1;
            MP_DBG(DBG_L3, "test round:%d Fail\n\n", i);
        }
        else
            MP_DBG(DBG_L3, "test round:%d Pass\n\n", i);
	}

	if (flag){
		prom_printf("Memory Test Fail\n");
	} else {
		prom_printf("Memory Test Success\n");
	}
 
	return 0;
}

#if defined(TCSUPPORT_CPU_EN7521)

static int do_mem_protect_test2(int argc, char *argv[])
{
	dramTest_info_t info;
	char flag = 0;
	unsigned long phy_endAddr, randAddr, randSize, addrMask, segStart, segEnd;
	unsigned long dramSize, counter, isUnCache, dmaEn, seg, i, operation;
	unsigned long startAddr, testCase;
	unsigned long seg1Start, seg1End, seg2Start, seg2End, segStartMin;
	long point_offset, testLen;
	
	counter = strtoul((const char*)(argv[1]), (char **)NULL, 10);
	isUnCache = strtoul((const char*)(argv[2]), (char **)NULL, 16);
	dmaEn = strtoul((const char*)(argv[3]), (char **)NULL, 16);
	operation= strtoul((const char*)(argv[4]), (char **)NULL, 10);
	point_offset = strtoul((const char*)(argv[5]), (char **)NULL, 10);
	testLen= strtoul((const char*)(argv[6]), (char **)NULL, 10);

	if (isUnCache)
		isUnCache = 1;

	if (dmaEn)
		dmaEn = 1;

	dmaEn = 0; /*only test CPU access DRAM*/

	/*record1 array maximum number is 1000.*/
	if(counter > 1000)
	{
		counter = 1000;
	}
	phy_endAddr = (VPint(0xbfa00034));
	dramSize = phy_endAddr - 0 + 1;
	
	MP_DBG(DBG_L1, "Memory Protect Test\n");
	MP_DBG(DBG_L1, "Valid physical address: 0x0 ~ 0x%x\n", (dramSize - 1));

	addrMask = (dramSize - 1);
	MP_DBG(DBG_L1, "addrMask: %x\n", addrMask);

	seg1Start = (VPint(0xbfa0001c));
	seg1End = (VPint(0xbfa00020));
	seg2Start = (VPint(0xbfa00024));
	seg2End = (VPint(0xbfa00028));

	if(isEN7526c)
	{
	    if (seg1Start > seg2Start)
	        segStartMin = seg2Start;
	    else
	        segStartMin = seg1Start;
	}

	for (test_round=0; test_round<counter; test_round++){
		for (i=0; i<6; i++){
			record1[test_round][i] = 0;
			record2[test_round][i] = 0;
		}
	}
	memset(record3_RegAddr,0,1000);
	memset(record4_illInfo,0,1000);
	ex_addr_count = 0;
	ex_info_count = 0 ;
	
	for (test_round=0; test_round<counter; test_round++)
	{
		if(isEN7526c)
		{
			if(operation)
			{
				startAddr =  segStartMin - point_offset;
			}
			else
			{
				startAddr =  segStartMin + point_offset;
			}
			/* assigned start address to non-alignment */
			if(startAddr % 8 == 0)
			{
				startAddr = startAddr - 1;
			}
		
		        info.startAddr = startAddr + segAddr[isUnCache][0];
		        if (dmaEn){
		            info.size = (random_word() & 0xff)+1;
		        }
		        else{
		            info.size = testLen;
		        }
		}
		
		if(!info.size)
			info.size = 0x4;
		info.wByte = 0;

		MP_DBG(DBG_L1, "Test round: %d\n", test_round);
		MP_DBG(DBG_L1, "Dram start address: %x\n", info.startAddr);
		MP_DBG(DBG_L1, "Dram test size: %x\n", info.size);

		record1[test_round][0] = startAddr;
		record1[test_round][1] = info.size;
		
		if ((startAddr + info.size) > phy_endAddr)
			record1[test_round][2] = 1;

		if ((startAddr>=seg1Start) && (startAddr<=seg1End))
			record1[test_round][3] = 1;

		if (((startAddr+info.size)>=seg1Start) && ((startAddr+info.size)<=seg1End))
			record1[test_round][3] = 1;

		/*cross the segment memory*/
		if ((startAddr<seg1Start) && ((startAddr+info.size)>=seg1Start))
			record1[test_round][3] = 1;
		
		if ((startAddr>=seg2Start) && (startAddr<=seg2End))
			record1[test_round][3] = 1;

		if (((startAddr+info.size)>=seg2Start) && ((startAddr+info.size)<=seg2End))
			record1[test_round][3] = 1;

		/*cross the segment memory*/
		if ((startAddr<seg2Start) && ((startAddr+info.size)>=seg2Start))
			record1[test_round][3] = 1;

		if (dmaEn){
			dramTest2(&info, 0);
		} else {
			dram_Nonalignment_Test(&info, 0);
		}
	}

	MP_DBG(DBG_L3, "seg1: %x ~ %x\n", seg1Start, seg1End);
	MP_DBG(DBG_L3, "seg2: %x ~ %x\n", seg2Start, seg2End);
    	MP_DBG(DBG_L3, "Dram Size: %x\n\n", phy_endAddr);

	for (i=0; i<counter; i++)
	{
		if((record1[i][2] != record2[i][2] )
		||(record1[i][3] != record2[i][3])
			)
		{
			MP_DBG(DBG_L3, "test round: %d\n", i);
			MP_DBG(DBG_L3, "test startAddr/endAddr : 0x%x/0x%x\n",      record1[i][0],(record1[i][0] + record1[i][1] - 1));
	        	MP_DBG(DBG_L3, "test size      : 0x%x\n",      record1[i][1]);
			MP_DBG(DBG_L3, "expected result: overdram-> %d seg_err-> %d\n",  record1[i][2], record1[i][3]);	
			MP_DBG(DBG_L3, "testing result:  overdram-> %d seg_err-> %d\n",  record2[i][2], record2[i][3]);	
			if (record2[i][2] || record2[i][3])
			{
				MP_DBG(DBG_L3, "invalid address : 0x%x\n", record2[i][5]);
	            		MP_DBG(DBG_L3, "invalid info    : 0x%x\n", record2[i][4]);
			}

			flag = 1;
	            	MP_DBG(DBG_L3, "test round:%d Fail\n\n", i);
	        	
		}
	}

	if (flag)
	{
		prom_printf("Memory Test Fail\n");
	} else {
		prom_printf("Memory Test Success\n");
	}
	/*Print more infomation*/
	MP_DBG(DBG_L3, "Count : %d\n",      counter);

	for (i=0; i<counter; i++)
	{
		if((record1[i][2]== record2[i][2] )
		||(record1[i][3]== record2[i][3])
			)
		{
			MP_DBG(DBG_L3, "test startAddr/endAddr : 0x%x/0x%x\n",      record1[i][0],(record1[i][0] + record1[i][1] - 1));
		       MP_DBG(DBG_L3, "test size      : 0x%x\n",      record1[i][1]);
			MP_DBG(DBG_L3, "expected result: overdram-> %d seg_err-> %d\n",  record1[i][2], record1[i][3]);	
			MP_DBG(DBG_L3, "testing result:  overdram-> %d seg_err-> %d\n",  record2[i][2], record2[i][3]);	
			if (record2[i][2] || record2[i][3])
			{
				MP_DBG(DBG_L3, "invalid address : 0x%x\n", record2[i][5]);
	            		MP_DBG(DBG_L3, "invalid info    : 0x%x\n", record2[i][4]);
			}
			break;
		}
	}

	/*illegal address different*/
	if(ex_addr_count)
	{
		MP_DBG(DBG_L3, "more invalid address : ");
		for (i=0; i<ex_addr_count; i++)
		{
			MP_DBG(DBG_L3, "0x%x, ", record3_RegAddr[i]);
		}
		MP_DBG(DBG_L3, "\n");
	}

	//illegal info different
	if(ex_info_count)
	{
		MP_DBG(DBG_L3, "more invalid info : ");
		for (i=0; i<ex_info_count; i++)
		{
			MP_DBG(DBG_L3, "0x%x, ", record4_illInfo[i]);
		}
		MP_DBG(DBG_L3, "\n");
	}
	
	return 0;
}
#endif

#if defined(TCSUPPORT_CPU_EN7521)
int nmi_mem_protect_test(void)
{
	dramTest_info_t info;
	char flag = 0;
	unsigned long phy_endAddr, randAddr, randSize, addrMask, segStart, segEnd;
	unsigned long dramSize, counter, isUnCache, dmaEn, seg, i;
	unsigned long startAddr, testCase;
	unsigned long seg1Start, seg1End, seg2Start, seg2End;

    if (nmi_test_enable == 0)
        return 0;

	phy_endAddr = (VPint(0xbfa00034));
	dramSize = phy_endAddr - 0 + 1;
	
	MP_DBG(DBG_L1, "Memory Protect Test\n");
	MP_DBG(DBG_L1, "Valid physical address: 0x0 ~ 0x%x\n", (dramSize - 1));

	addrMask = (dramSize - 1);
	MP_DBG(DBG_L1, "addrMask: %x\n", addrMask);

	seg1Start = (VPint(0xbfa0001c));
	seg1End = (VPint(0xbfa00020));
	seg2Start = (VPint(0xbfa00024));
	seg2End = (VPint(0xbfa00028));

	MP_DBG(DBG_L1, "seg1: %x ~ %x\n", seg1Start, seg1End);
	MP_DBG(DBG_L1, "seg2: %x ~ %x\n", seg2Start, seg2End);
    MP_DBG(DBG_L1, "Dram Size: %x\n\n", phy_endAddr);

    /* case0: 0x60000~seg1Start, case1: seg1Start~seg1End, case2: seg1End~seg2Start,
     * case3: seg2Start~seg2End, case4: seg2End~dramSize,  case5: dramSize~+0xff */
	for (testCase=0; testCase<6; testCase++){
        switch (testCase) {
            case 0:
                startAddr = 0x60000 + (random_word() % (seg1Start-0x60000));
                break;
            case 1:
                startAddr = seg1Start + (random_word() % (seg1End-seg1Start-0xf));
                break;
            case 2:
                startAddr = seg1End + (random_word() % (seg2Start-seg1End-0xf));
                break;
            case 3:
                startAddr = seg2Start + (random_word() % (seg2End-seg2Start-0xf));
                break;
            case 4:
                startAddr = seg2End + (random_word() % (dramSize-seg2End-0xf));
                break;
            case 5:
                startAddr += dramSize+(random_word() % 0x800000)+4;
                break;
            default:

                break;
        }

        startAddr -= (startAddr%4);
		info.startAddr = segAddr[1][0] + startAddr;
		info.size = (random_word() & 0xf);
		info.size -= (info.size % 4);
		
		if(!info.size)
			info.size = 0x4;
		info.wByte = 0;

		MP_DBG(DBG_L1, "Test case: %d\n", testCase);
		MP_DBG(DBG_L1, "Dram start address: %x\n", info.startAddr);
		MP_DBG(DBG_L1, "Dram test size: %x\n", info.size);

		dramTest(&info, 0);
	}

    nmi_test_enable = 0;
 
	return 0;
}
#endif

/**************** memory access range protection *****************/
#endif
/****************** spi dram range protection ********************/

#endif
#if defined(RBUS_COUNTER_TEST)
rcnt_para rcnt_info[]=
{
	{CPU2PBUS_WRCNT,CPU2PBUS_WRCNT_MIN,CPU2PBUS_WRCNT_MAX},
	{CPU2PBUS_RDCNT,CPU2PBUS_RDCNT_MIN,CPU2PBUS_RDCNT_MAX},
	
	{CPU2DRAM_WRCNT,CPU2DRAM_WRCNT_MIN,CPU2DRAM_WRCNT_MAX},
	{CPU2DRAM_RDCNT,CPU2DRAM_RDCNT_MIN,CPU2DRAM_RDCNT_MAX},
	
	{DMA2DRAM_WRCNT,DMA2DRAM_WRCNT_MIN,DMA2DRAM_WRCNT_MAX},
	{DMA2DRAM_RDCNT,DMA2DRAM_RDCNT_MIN,DMA2DRAM_RDCNT_MAX},
	{0, 0, 0}
};

auto_para auto_test_table[]=
{
#if 0//PCIe 
	{1, 0x1fb80000, 0x1fb81000, 0x500, 1025},
	{1, 0x1fb80000, 0x1fb81004, 0x500, 1026},
	{1, 0x1fb80000, 0x1fb81008, 0x500, 1027},
	{1, 0x1fb80000, 0x1fb82000, 0x500, 1280},
	{1, 0x1fb80000, 0x1fb84000, 0x500, 1280},

	{2, 0x1fb80000, 0x1fb81000, 0x500, 1025},
	{2, 0x1fb80000, 0x1fb81004, 0x500, 1026},
	{2, 0x1fb80000, 0x1fb81008, 0x500, 1027},
	{2, 0x1fb80000, 0x1fb82000, 0x500, 1280},
	{2, 0x1fb80000, 0x1fb84000, 0x500, 1280},
#endif
#if 1//USB
	{1, 0x1fba0000, 0x1fba1000, 0x500, 1025},
	{1, 0x1fba0000, 0x1fba1004, 0x500, 1026},
	{1, 0x1fba0000, 0x1fba1008, 0x500, 1027},
	{1, 0x1fba0000, 0x1fba2000, 0x500, 1280},
	{1, 0x1fba0000, 0x1fba4000, 0x500, 1280},
	
	{2, 0x1fba0000, 0x1fba1000, 0x500, 1025},
	{2, 0x1fba0000, 0x1fba1004, 0x500, 1026},
	{2, 0x1fba0000, 0x1fba1008, 0x500, 1027},
	{2, 0x1fba0000, 0x1fba2000, 0x500, 1280},
	{2, 0x1fba0000, 0x1fba4000, 0x500, 1280},
#endif
#if 1//QDMA
	{1, 0x1fb50000, 0x1fb51000, 0x500, 1025},
	{1, 0x1fb50000, 0x1fb51004, 0x500, 1026},
	{1, 0x1fb50000, 0x1fb51008, 0x500, 1027},
	{1, 0x1fb50000, 0x1fb52000, 0x500, 1280},
	{1, 0x1fb50000, 0x1fb54000, 0x500, 1280},

	{2, 0x1fb50000, 0x1fb51000, 0x500, 1025},
	{2, 0x1fb50000, 0x1fb51004, 0x500, 1026},
	{2, 0x1fb50000, 0x1fb51008, 0x500, 1027},
	{2, 0x1fb50000, 0x1fb52000, 0x500, 1280},
	{2, 0x1fb50000, 0x1fb54000, 0x500, 1280},
#endif	
////////////////////////////////////////////////////////////
	{3, 0x20000, 0x21000, 0x1000, 1026},
	{3, 0x20000, 0x22000, 0x1000, 2050},
	{3, 0x20000, 0x22004, 0x1000, 2050},
	{3, 0x20000, 0x22008, 0x1000, 2052},
	{3, 0x20000, 0x24000, 0x1000, 4096},
	{3, 0x20000, 0x24008, 0x1000, 4096},

	{4, 0x20000, 0x21000, 0x1000, 1026},
	{4, 0x20000, 0x22000, 0x1000, 2050},
	{4, 0x20000, 0x22004, 0x1000, 2050},
	{4, 0x20000, 0x22008, 0x1000, 2052},
	{4, 0x20000, 0x24000, 0x1000, 4096},
	{4, 0x20000, 0x24008, 0x1000, 4096},
////////////////////////////////////////////////////////////
	{5, 0x20000, 0x21000, 0x64, 65},
	{5, 0x20000, 0x21100, 0x64, 69},
	{5, 0x20000, 0x21140, 0x64, 70},
	{5, 0x20000, 0x2117f, 0x64, 70},
	{5, 0x20000, 0x21180, 0x64, 71},
	{5, 0x20000, 0x25000, 0x64, 100},

	{6, 0x20000, 0x21000, 0x64, 65},
	{6, 0x20000, 0x21100, 0x64, 69},
	{6, 0x20000, 0x21140, 0x64, 70},
	{6, 0x20000, 0x2117f, 0x64, 70},
	{6, 0x20000, 0x21180, 0x64, 71},
	{6, 0x20000, 0x25000, 0x64, 100},
	{ 0, 0x0 , 0x0, 0x0, 0},
};

int gdmacopy(unsigned long addr1, unsigned long addr2, unsigned long len)
{
	unsigned char ch, burst_size;
//	ch = random_byte() & 0xf;
	ch = 0xf;
    if (isEN751221)
        ch &= 0x7;

//	burst_size = random_byte() & 0x7;
	burst_size = 0x4;
	if (len == 0)
		len = 1;
	// GDMA
	VPint(CR_GDMA_SA(ch)) = (addr2 & 0x1fffffff);
	VPint(CR_GDMA_DA(ch)) = (addr1 & 0x1fffffff);
	VPint(CR_GDMA_CT1(ch)) = (32<<16) | (32<<8);
	VPint(CR_GDMA_CT0(ch)) = ((len&0xffff)<<16) | (burst_size<<3) | (1<<1) | (1<<0);
	while (!(VPint(CR_GDMA_DONEINT) & (1<<ch))) ;
	VPint(CR_GDMA_DONEINT) = (1<<ch);
}

int Cfg_cnt_ctr(unsigned int type, unsigned int startAddr, unsigned int endAddr)
{
	unsigned long tempvalue = 0;

	tempvalue |= 1<<(type-1);
	
	VPint(BUS_CNT_EN) = 0;
	VPint(BUS_CNT_EN) = tempvalue;
	VPint(rcnt_info[type-1].startAddr) = startAddr;
	VPint(rcnt_info[type-1].endAddr) = endAddr;

	prom_printf("\r\n(1)EN\t:0x%08x => 0x%08x", BUS_CNT_EN, VPint(BUS_CNT_EN));
	prom_printf("\r\n(2)MIN\t:0x%08x => 0x%08x", rcnt_info[type-1].startAddr, VPint(rcnt_info[type-1].startAddr));
	prom_printf("\r\n(3)MAX\t:0x%08x => 0x%08x", rcnt_info[type-1].endAddr, VPint(rcnt_info[type-1].endAddr));

	return 0;
}

int Read_cnt_reg(unsigned int type)
{
	prom_printf("\r\n(5)CNT\t:0x%08x => 0x%08x", rcnt_info[type-1].count, VPint(rcnt_info[type-1].count));
	return VPint(rcnt_info[type-1].count);
}

int RCnt_func(unsigned int type, unsigned int startAddr, unsigned int endAddr, unsigned int len, int result)
{
	unsigned char *ptrrw = NULL;
	int i;
	int count = 0;
	unsigned long tempvalue;
	prom_printf("\r\nStart Test!");
	Cfg_cnt_ctr(type, startAddr, endAddr);
	startAddr = startAddr | 0xa0000000;
	ptrrw = startAddr;
	prom_printf("\r\n(4)RED\t:0x%08x -> 0x%08x  %x", startAddr, endAddr, len);
	
	switch(type)
	{		
		case 1://cpu2pbus write
			for(i=0; i< len; i++)
			{
				*ptrrw = 0x005aa5ff;
				ptrrw+=4;
			}
			count = Read_cnt_reg(type);
			break;
		case 2://cpu2pbus read
			for(i=0; i< len; i++)
			{
				tempvalue = *(volatile unsigned long *)ptrrw;
				ptrrw+=4;
			}
			count = Read_cnt_reg(type);
			break;
		case 3://cpu2dram write
			for(i=0; i< len; i++)
			{
				*ptrrw = 0x005aa5ff;
				ptrrw+=4;
			}
			count = Read_cnt_reg(type);
			break;
		case 4://cpu2dram read
			for(i=0; i< len; i++)
			{
				tempvalue = *(volatile unsigned long *)ptrrw;
				ptrrw+=4;
			}
			count = Read_cnt_reg(type);
			break;
		case 5://dma2dram write
			for(i=0; i< len; i++)
			{
				gdmacopy(startAddr, 0xa0020000, 0x40);
				startAddr+=0x40;
			}
			count = Read_cnt_reg(type);
			break;
		case 6://dma2dram read
			for(i=0; i< len; i++)
			{
				gdmacopy(0xa0040000, startAddr, 0x40);
				startAddr+=0x40;
			}
			count = Read_cnt_reg(type);
			break;
		default:
			prom_printf("\r\nNot support this type!");
			break;
	}
	if(count == result){
		prom_printf("\r\nOK!\r\n");
		return 0;
	}
	else{
		prom_printf("\r\nXX %d  VS  %d!\r\n", count, result);
		return 1;
	}
}

static int do_rbus_counter(int argc, char *argv[])
{
	unsigned int type, length;
	unsigned long seg, startAddr, endAddr;
	
	int i, auto_num, count, result;

	auto_num = 0;
	result = 0;
	count = 0;

	if(argc <2){
		prom_printf("\r\nrbus_counter [all | <type> <MIN~MAX> <length> <count>]\r\n");
		return 0;
	}
	if(strcmp(argv[1], "all") == 0){
		auto_num = sizeof(auto_test_table)/sizeof(auto_para);
		for(i = 0; i < auto_num-1; i++)
		{
			result = RCnt_func(auto_test_table[i].type, auto_test_table[i].startAddr, 
				auto_test_table[i].endAddr, auto_test_table[i].length, auto_test_table[i].count);
			if(result != 0){
				prom_printf("\r\nError %d, Try cmd: rbus_counter %d %x %x %x %d\r\n", i, 
					auto_test_table[i].type, auto_test_table[i].startAddr, 
					auto_test_table[i].endAddr, auto_test_table[i].length, auto_test_table[i].count);
				return;
			}
		}
		prom_printf("===========>Test All PASS\r\n");
	}
	else if(argc >= 6)
	{
		type = simple_strtoul(argv[1],NULL,16);
		startAddr = strtoul((const char*)(argv[2]), (char **)NULL, 16);
		endAddr = strtoul((const char*)(argv[3]), (char **)NULL, 16);
		length = simple_strtoul(argv[4],NULL,16);
		count = simple_strtoul(argv[5],NULL,16);
		RCnt_func(type, startAddr, endAddr, length, count);
	}
	else
		prom_printf("\r\nrbus_counter <type> <MIN~MAX> <length> <count>");
	return 0;
}
#endif

#ifdef MT7550_GPIO_TEST//tony add
static int do_gpio_test(int argc, char *argv[])
{
	uint8 regs = 0;
	uint8 data1, data, num = 1;
	uint8 valid, i;
	
	if (argc>=3 && (strcmp(argv[1], "r") == 0))
	{
		regs = simple_strtoul(argv[2],NULL,16);
		if (argc == 4)
			num = simple_strtoul(argv[3],NULL,16);

		for(i=0; i<num; i++)
		{
			valid = AfeMT7550ReadReg(regs+i, &data);
			if(valid == 0)
			{
				prom_printf("\r\n7550 reg read not access!!");
			}
			if(((i % 8) == 0))
			{
				prom_printf("\r\nregs 0x%x: ", regs+i);
			}
			prom_printf("0x%x ", data);
		}
		prom_printf("\r\n");
	}
	else if (argc==4 && (strcmp(argv[1], "w") == 0))
	{
		regs = simple_strtoul(argv[2],NULL,16);
		data = simple_strtoul(argv[3],NULL,16);
				
		valid = AfeMT7550WriteReg(regs, data);
		if(valid == 0)
		{
			prom_printf("\r\n7550 reg write not access!!");
		}
		AfeMT7550ReadReg(regs, &data1);
		if(data1!= data)
		{
			prom_printf("\r\nWrite error: %x VS. %x\r\n", data1, data);
		}
	}
	else
		prom_printf("\r\ngpio_test [r/w] 0xregs [0xvalue]\r\n");
}
#endif

#ifdef TC3262
#ifdef MT75XX_REDUCE_SIZE
static void
delay1us(
        int us
)
{
        volatile uint32 timer_now, timer_last;
        volatile uint32 tick_acc;
        uint32 one_tick_unit = get_SYS_HCLK() << 1;
        volatile uint32 tick_wait = us * one_tick_unit;
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
#endif
int do_cpufreq(int argc, char* argv[])
{
#ifdef MT75XX_REDUCE_SIZE
#ifndef BOOTROM_IN_SRAM
	unsigned int tmp, reg_temp, maxfreq, max_cpu_value, cur_cpu_value, target_cpu;
	int step_num, cpu_step;
	char cpu_trap, crys_trap;
	
	cpu_trap = ((VPint(CR_AHB_HWCONF)>>9)&0x3);
	crys_trap =((VPint(CR_AHB_HWCONF)>>22)&0x3);
	if((crys_trap == 0)){
		cpu_step = 2500;
	}else if ((crys_trap == 1) || (crys_trap == 2)){
		cpu_step = 2000;
	}else{
		prom_printf("error crystal hw trap\n");
		return -1;
	}

	switch(cpu_trap)
	{
		case 0 :
			maxfreq = 750;
			cur_cpu_value = (VPint(0xbfb00364));
			if(cpu_step == 2500)
				max_cpu_value = 0x1e000000;
			else
				max_cpu_value = 0x25800000;
			break;
		case 1 :
			maxfreq = 650;
			if(cpu_step == 2500){
				reg_temp = 0xbfb003b0;
				max_cpu_value = 0x1a000000;
			}else{
				reg_temp = 0xbfb003b8;
				max_cpu_value = 0x20800000;
			}
			cur_cpu_value = (((VPint(reg_temp)>>8)&0xff)<<24) | (((VPint(reg_temp)>>3)&0x1)<<23) | (VPint(0xbfb00364)&0x7fffff);
			break;
		case 2 :
			maxfreq = 500; 
			if(cpu_step == 2500){
				reg_temp = 0xbfb003b0;
				max_cpu_value = 0x14000000;
			}else{
				reg_temp = 0xbfb003b8;
				max_cpu_value = 0x19000000;
			}
			cur_cpu_value = (((VPint(reg_temp)>>24)&0xff)<<24) | (((VPint(reg_temp)>>19)&0x1)<<23) | (VPint(0xbfb00364)&0x7fffff);
			break;
		case 3 :
			maxfreq = 250; 
			if(cpu_step == 2500){
				reg_temp = 0xbfb003b4;
				max_cpu_value = 0x14000000;
			}else{
				reg_temp = 0xbfb003bc;
				max_cpu_value = 0x19000000;
			}
			cur_cpu_value = (((VPint(reg_temp)>>8)&0xff)<<24) | (((VPint(reg_temp)>>3)&0x1)<<23) | (VPint(0xbfb00364)&0x7fffff);
			break;
		default:
			prom_printf("error cpu hw trap\n");
			return -1;
	}
	target_cpu = strtoul((const char*)(argv[1]), (char **)NULL, 10);

	prom_printf("Target CPU %dMHz MAX CPU %dMHz cpu step %dKHz\n",target_cpu,maxfreq,cpu_step);
	if(target_cpu > maxfreq)
		return -1;
	
	step_num = ((max_cpu_value - cur_cpu_value)/0x199800) - ((maxfreq - target_cpu)*1000/cpu_step);

	prom_printf("max_cpu_value %x,cur_cpu_value %x step_num %d\n",max_cpu_value,cur_cpu_value,step_num);

	VPint(0xbfb0039c) = 0x00750025;
	while(step_num != 0){
		if(step_num > 0){
			step_num--;
			cur_cpu_value += 0x199800;
		}else{
			step_num++;
			cur_cpu_value -= 0x199800;
		}
		//prom_printf("cur_cpu_value %x , CPU %x\n",cur_cpu_value,VPint(0xbfb00364));
		switch(cpu_trap)
        	{
                	case 0 :
				VPint(0xbfb00364) = cur_cpu_value;
				break;
			case 1:
				if(cpu_step == 2500){
					reg_temp = 0xbfb003b0;
				}else{
					reg_temp = 0xbfb003b8;
				}
				tmp = VPint(reg_temp);
				tmp &= ((0xff<<8) | (1<3));
				tmp |= (((cur_cpu_value>>24) &0xff)<<8) | (((cur_cpu_value>>23) &0x1)<<3);
				VPint(reg_temp) = tmp;
				break;
			case 2:
				if(cpu_step == 2500){
					reg_temp = 0xbfb003b0;	
				}else{
					reg_temp = 0xbfb003b8;
				}
				tmp = VPint(reg_temp);
				tmp &= ((0xff<<24) | (1<19));
				tmp |= (((cur_cpu_value>>24) &0xff)<<24) | (((cur_cpu_value>>23) &0x1)<<19);
				VPint(reg_temp) = tmp;
				break;
			case 3:
				if(cpu_step == 2500){
					reg_temp = 0xbfb003b4;
				}else{
					reg_temp = 0xbfb003bc;
				}
				tmp = VPint(reg_temp);
				tmp &= ((0xff<<8) | (1<3));
				tmp |= (((cur_cpu_value>>24) &0xff)<<8) | (((cur_cpu_value>>3) &0x1)<<3);
				VPint(reg_temp) = tmp;
				break;
			default:
				 prom_printf("error\n");
				break;
		}
		if(cpu_trap != 0){
			tmp = VPint(0xbfb00364);
			tmp &= ~(0x7fffff);
			tmp |= (cur_cpu_value) & 0x7fffff;
			VPint(0xbfb00364) = tmp;
		}
		tmp = VPint(0xbfb00368);
		if(tmp &(1<<12)){
			tmp &= ~(1<<12);
		}else{
			tmp |= (1<<12);
		}
		VPint(0xbfb00368) = tmp;
		delay1us(1);	
	}	
	VPint(0xbfb0039c) = 0;
#endif
#else
if (isTC3182 || isRT65168) {
	unsigned int n;
	unsigned int targetfreq;
	unsigned int current_m = VPint(CR_AHB_CPUF)&0x1f;
	unsigned int current_n = (VPint(CR_AHB_CPUF)&0x1ff0000)>>16;
	unsigned int currentfreq = 12*(current_n+1)/(current_m+1);

	prom_printf("usage: cpufreq <clk>\n");

    targetfreq = strtoul((const char*)(argv[1]), (char **)NULL, 10);
	n = (targetfreq*(current_m+1)/12)-1;
    targetfreq = 12*(n+1)/(current_m+1);
	
	prom_printf("m=%d n=%d change CPU freq to %dMhz\n", current_m, n, targetfreq);

    if (currentfreq > targetfreq) {
		while (current_n > n) {
			current_n -= 5;
			currentfreq = 12*(current_n+1)/(current_m+1);
			VPint(CR_AHB_CPUF) = current_n << 16 | current_m;
			pause(1);
		}
    } else {
		while (current_n < n) {
			current_n += 5;
			currentfreq = 12*(current_n+1)/(current_m+1);
			VPint(CR_AHB_CPUF) = current_n << 16 | current_m;
			pause(1);
		}
	}

	if (current_n != n) {
		current_n = n;
		currentfreq = 12*(current_n+1)/(current_m+1);
		VPint(CR_AHB_CPUF) = current_n << 16 | current_m;
		pause(1);
	}
    prom_printf("\n");
} else if (isRT63365) {
	int divq_array[6] = {1, 2, 4, 8, 16, 32};
	unsigned int pllreg;
	unsigned int fvco;
	unsigned int targetfreq;
	unsigned int current_divf, current_divq, current_divr, currentfreq;

	pllreg = (((VPint(0xbfb0008c)&((1<<9)|(1<<8)))>>8) == 0x1) ? 0xbfb001d0 : (VPint(0xbfb0008c)&(1<<25) ? 0xbfb001c8 : 0xbfb001cc);

	current_divf = (VPint(pllreg)>>16)&0xff;
	current_divq = (VPint(pllreg)>>8)&0x7;
	current_divr = VPint(pllreg)&0x1f;
	currentfreq = 20/(current_divr+1)*(current_divf+1)/divq_array[current_divq];

	prom_printf("usage: cpufreq <clk>\n");

    targetfreq = strtoul((const char*)(argv[1]), (char **)NULL, 10);
	
	//prom_printf("divr=%d divf=%d divq=%d\n", current_divr, current_divf, current_divq);

	prom_printf("current CPU freq=%dMhz change CPU freq to %dMhz\n", currentfreq, targetfreq);

    if (currentfreq > targetfreq) {
		while (currentfreq > targetfreq) {
			current_divf--;
			fvco = 20/(current_divr+1)*(current_divf+1);
			if ((fvco < 1200) || (fvco > 2400)) {
				current_divf = current_divf * 2;
				current_divq++;
			}
			currentfreq = 20/(current_divr+1)*(current_divf+1)/divq_array[current_divq];
			VPint(pllreg) = (VPint(pllreg) & 0xff00f8ff) | (current_divf<<16) | (current_divq<<8);
			pause(1);
		}
    } else {
		while (currentfreq < targetfreq) {
			current_divf++;
			fvco = 20/(current_divr+1)*(current_divf+1);
			if ((fvco < 1200) || (fvco > 2400)) {
				current_divf = current_divf / 2;
				current_divq--;
			}
			currentfreq = 20/(current_divr+1)*(current_divf+1)/divq_array[current_divq];
			VPint(pllreg) = (VPint(pllreg) & 0xff00f8ff) | (current_divf<<16) | (current_divq<<8);
			pause(1);
		}
	}

	currentfreq = 20/(current_divr+1)*(current_divf+1)/divq_array[current_divq];
	prom_printf("change CPU freq to %dMhz\n", currentfreq);

    prom_printf("\n");
} else {
	unsigned int m, n;
	unsigned int targetfreq;
	unsigned int tmp_m = (VPint(CR_AHB_CPUF)&0x1ff0000)>>16;
	unsigned int tmp_n = VPint(CR_AHB_CPUF)&0x1f;
	unsigned int currentfreq = 6*(tmp_m+1)/((tmp_n+1));
	/*cpufreq <freq>*/
    if(argc==2){
    	targetfreq = strtoul((const char*)(argv[1]), (char **)NULL, 10);
    	targetfreq = targetfreq - (targetfreq%6);
    	m = targetfreq/6 - 1;
    	n = 0;

    }
    else{/*cpufreq <m> <n>*/
    	m = strtoul((const char*)(argv[1]), (char **)NULL, 10);
    	n = strtoul((const char*)(argv[2]), (char **)NULL, 10);
        targetfreq = 6*(m+1)/((n+1));
        tmp_m = (currentfreq/6)*(n+1) - 1;
    }
	if(targetfreq >450 || targetfreq < 156){
		prom_printf("CPU freq has to be between 156~450 Mhz\n");
		return 0;
	}
	if(argc==2){
		prom_printf("change CPU freq to %d Mhz (the freq has to be multiple of 6)\n",targetfreq);
	}
	else{
		prom_printf("m=%d n=%d change CPU freq to %dMhz\n", m, n, targetfreq);
	}

    if(currentfreq > targetfreq){
		while(tmp_m > m){
			tmp_m-=(n+1);
			VPint(CR_AHB_CPUF) = tmp_m << 16 |n;
			uart_init();
			pause(1);
		}
    }
    else{
		while(tmp_m < m){
			tmp_m+=(n+1);
			VPint(CR_AHB_CPUF) = tmp_m << 16 |n;
			uart_init();
			pause(1);
		}
    }
    prom_printf("\n");
}
#endif
	return 0;
}
#else
/* TC3162U PLL configuration */

int do_cpufreq(int argc, char* argv[])
{
	unsigned int n, pll_gain;
	unsigned int targetfreq;
	unsigned int current_m = (VPint(CR_AHB_PLL)>>25)&0x1f;
	unsigned int current_n = (VPint(CR_AHB_PLL)>>16)&0x1ff;
	unsigned int currentfreq = 6*(current_n+1)/((current_m+1));

	if (!isTC3162U)
		return;

	prom_printf("usage: cpufreq <n> m=8\n");

    n = strtoul((const char*)(argv[1]), (char **)NULL, 10);
    targetfreq = 6*(n+1)/((current_m+1));
	
	prom_printf("m=%d n=%d change CPU freq to %dMhz\n", current_m, n, targetfreq);

    if (currentfreq > targetfreq) {
		while (current_n > n) {
			current_n -= 5;
			currentfreq = 6*(current_n+1)/((current_m+1));
			if (currentfreq > 300)
				pll_gain = 1;
			else 
				pll_gain = 0;
			VPint(CR_AHB_PLL) = ((VPint(CR_AHB_PLL) & 0xffff)<<16) | (pll_gain<<15) | ((current_m & 0x1f) << 9) | (current_n & 0x1ff);
			uart_init();
			pause(1);
		}
    } else {
		while (current_n < n) {
			current_n += 5;
			currentfreq = 6*(current_n+1)/((current_m+1));
			if (currentfreq > 300)
				pll_gain = 1;
			else 
				pll_gain = 0;
			currentfreq = 6*(current_n+1)/((current_m+1));
			VPint(CR_AHB_PLL) = ((VPint(CR_AHB_PLL) & 0xffff)<<16) | (pll_gain<<15) | ((current_m & 0x1f) << 9) | (current_n & 0x1ff);
			uart_init();
			pause(1);
		}
	}

	if (current_n != n) {
		current_n = n;
		currentfreq = 6*(current_n+1)/((current_m+1));
		if (currentfreq > 300)
			pll_gain = 1;
		else 
			pll_gain = 0;
		VPint(CR_AHB_PLL) = ((VPint(CR_AHB_PLL) & 0xffff)<<16) | (pll_gain<<15) | ((current_m & 0x1f) << 9) | (current_n & 0x1ff);
		uart_init();
		pause(1);
	}

    prom_printf("\n");
	return 0;
}

#endif

#ifdef TC3262
#ifndef BOOTROM_IN_SRAM

int do_ddrdrv(int argc, char* argv[])
{
	uint32 ma_drvp, ma_drvn;
	uint32 odt_drvp, odt_drvn;
	uint32 dq_drvp, dq_drvn;
	uint32 dqs_drvp, dqs_drvn;
	
	uint32 ck_drvp, ck_drvn;
	uint32 csn_drvp, csn_drvn;
	uint32 cke_drvp, cke_drvn;
	
	if (argc != 15) {
		ma_drvp = (VPint(0xbfb20100) >> 25) & 0x1f;
		ma_drvn = (VPint(0xbfb20100) >> 20) & 0x1f;
		odt_drvp = (VPint(0xbfb20100) >> 9) & 0x1f;
		odt_drvn = (VPint(0xbfb20100) >> 4) & 0x1f;
		dq_drvp = (VPint(0xbfb2010c) >> 12) & 0x1f;
		dq_drvn = (VPint(0xbfb2010c) >> 4) & 0x1f;
		dqs_drvp = (VPint(0xbfb20114) >> 12) & 0x1f;
		dqs_drvn = (VPint(0xbfb20114) >> 4) & 0x1f;
		ck_drvp = (VPint(0xbfb20104) >> 17) & 0x1f;
		ck_drvn = (VPint(0xbfb20104) >> 12) & 0x1f;
		csn_drvp = (VPint(0xbfb20104) >> 27) & 0x1f;
		csn_drvn = (VPint(0xbfb20104) >> 22) & 0x1f;
		cke_drvp = (VPint(0xbfb20108) >> 20) & 0x1f;
		cke_drvn = (VPint(0xbfb20108) >> 12) & 0x1f;
		
		prom_printf("MA  DRVP=%04x DRVN=%04x\n", ma_drvp, ma_drvn);
		prom_printf("ODT DRVP=%04x DRVN=%04x\n", odt_drvp, odt_drvn);
		prom_printf("DQ  DRVP=%04x DRVN=%04x\n", dq_drvp, dq_drvn);
		prom_printf("DQS DRVP=%04x DRVN=%04x\n", dqs_drvp, dqs_drvn);
		prom_printf("CK  DRVP=%04x DRVN=%04x\n", ck_drvp, ck_drvn);
		prom_printf("CSN DRVP=%04x DRVN=%04x\n", csn_drvp, csn_drvn);
		prom_printf("CKE DRVP=%04x DRVN=%04x\n", cke_drvp, cke_drvn);
	} else {
		ma_drvp = strtoul((const char*)(argv[1]), (char **)NULL, 16) & 0x1f;		
		ma_drvn = strtoul((const char*)(argv[2]), (char **)NULL, 16) & 0x1f;		
		odt_drvp = strtoul((const char*)(argv[3]), (char **)NULL, 16) & 0x1f;		
		odt_drvn = strtoul((const char*)(argv[4]), (char **)NULL, 16) & 0x1f;		
		dq_drvp = strtoul((const char*)(argv[5]), (char **)NULL, 16) & 0x1f;		
		dq_drvn = strtoul((const char*)(argv[6]), (char **)NULL, 16) & 0x1f;		
		dqs_drvp = strtoul((const char*)(argv[7]), (char **)NULL, 16) & 0x1f;		
		dqs_drvn = strtoul((const char*)(argv[8]), (char **)NULL, 16) & 0x1f;		
		ck_drvp = strtoul((const char*)(argv[9]), (char **)NULL, 16) & 0x1f;		
		ck_drvn = strtoul((const char*)(argv[10]), (char **)NULL, 16) & 0x1f;		
		csn_drvp = strtoul((const char*)(argv[11]), (char **)NULL, 16) & 0x1f;		
		csn_drvn = strtoul((const char*)(argv[12]), (char **)NULL, 16) & 0x1f;		
		cke_drvp = strtoul((const char*)(argv[13]), (char **)NULL, 16) & 0x1f;		
		cke_drvn = strtoul((const char*)(argv[14]), (char **)NULL, 16) & 0x1f;		

		VPint(0xbfb20100) = VPint(0xbfb20100) & (~((0x1f<<25) | (0x1f<<20))) | ((ma_drvp<<25) | (ma_drvn<<20));
		VPint(0xbfb20100) = VPint(0xbfb20100) & (~((0x1f<<9) | (0x1f<<4))) | ((odt_drvp<<9) | (odt_drvn<<4));
		VPint(0xbfb2010c) = VPint(0xbfb2010c) & (~((0x1f<<12) | (0x1f<<4))) | ((dq_drvp<<12) | (dq_drvn<<4));
		VPint(0xbfb20114) = VPint(0xbfb20114) & (~((0x1f<<12) | (0x1f<<4))) | ((dqs_drvp<<12) | (dqs_drvn<<4));
		VPint(0xbfb20104) = VPint(0xbfb20104) & (~((0x1f<<17) | (0x1f<<12))) | ((ck_drvp<<17) | (ck_drvn<<12));
		VPint(0xbfb20104) = VPint(0xbfb20104) & (~((0x1f<<27) | (0x1f<<22))) | ((csn_drvp<<27) | (csn_drvn<<22));
		VPint(0xbfb20108) = VPint(0xbfb20108) & (~((0x1f<<20) | (0x1f<<12))) | ((cke_drvp<<20) | (cke_drvn<<12));
	}

	return 0;
}
#endif
#endif

#ifdef ETH_TEST_CMD
extern int tc3162_eth_testsend(unsigned long len, unsigned long cnt);
extern int tc3162_eth_testsend2(unsigned long len, unsigned long cnt);
extern int tc3162_eth_dump(void);

int do_ethsend(int argc, char *argv[])
{
	unsigned long len;
	unsigned long cnt;
	unsigned long i;
	unsigned char k;

	len = strtoul((const char*)(argv[1]), (char **)NULL, 10);
	cnt = strtoul((const char*)(argv[2]), (char **)NULL, 10);

	prom_printf("Send %d ethernet frame with len=%d\n", cnt, len);

	tc3162_eth_testsend(len, cnt);

	return 0;
}

int do_ethsend2(int argc, char *argv[])
{
	unsigned long len;
	unsigned long cnt;
	unsigned long i;
	unsigned char k;

	len = strtoul((const char*)(argv[1]), (char **)NULL, 10);
	cnt = strtoul((const char*)(argv[2]), (char **)NULL, 10);

	prom_printf("Send %d ethernet frame with len=%d\n", cnt, len);

	tc3162_eth_testsend2(len, cnt);

	return 0;
}

int do_ethdump(int argc, char *argv[])
{
	tc3162_eth_dump();

	return 0;
}

#endif

extern int XModemReceive(char *bufBase, int bufLen);

int do_xmodem_rx(int argc, char* argv[])
{
	unsigned long addr;
	unsigned int len;
	unsigned int rcv_len;

	addr = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	len = strtoul((const char*)(argv[2]), (char **)NULL, 16);

	rcv_len = XModemReceive((char *) addr, len);
	if (rcv_len)
		prom_printf("received len=%x\n", rcv_len);
	else
		prom_printf("received error\n");

	return 0;
}
#ifdef BOOTROM_IN_SRAM
extern int	is_fw_upgrade;
extern int	flash_base_addr;

int do_xmodem_rx_flash(int argc, char* argv[])
{
	unsigned long addr;
	unsigned int len;
	unsigned int flash_addr;
	unsigned int rcv_len;

	addr = strtoul((const char*)(argv[1]), (char **)NULL, 16);
	len = strtoul((const char*)(argv[2]), (char **)NULL, 16);
	flash_addr = strtoul((const char*)(argv[3]), (char **)NULL, 16);

	is_fw_upgrade = 1;
	flash_base_addr = flash_addr;
	prom_printf("received from UART and write to flash\n");
	rcv_len = XModemReceive((char *) addr, len);
	if (rcv_len)
		prom_printf("received len=%x\n", rcv_len);
	else
		prom_printf("received error\n");

	return 0;
}
#endif

#if defined(TCSUPPORT_CT_BOOTLOADER_UPGRADE)
char *cmd_gets(char *buf,	int len, int flag)
{
	int c,i=0;
	char *cp;

	cp = buf;
	while ((c = serial_inc()) != KEY_CR) 
	{
		if ( c == KEY_BS ) 
		{
			if ( cp != buf ) 
			{
				printf("\b \b");
				cp--;
				i--;
			}
		} 
		else
		{
			if ( buf != NULL ) 
			{
				if ( i <= len-1 )			
				{
					if ( flag == CMD )
						serial_outc(c); /*cmd mode*/
					else 
						serial_outc('*'); /*passord mode*/
					*cp++ = c;
					i++;
				}
			}
		}
	}
	if (buf != NULL)
		*cp = '\0';
	return buf;
}
#else
char *cmd_gets(char *buf,	int len)
{
	int c;
	char *cp;

	cp = buf;
	while ((c = serial_inc()) != KEY_CR) {
		if (c == KEY_BS) {
			if ( cp != buf ) {
				printf("\b \b");
				cp--;
			}
		} else {
			if(buf != NULL) {
				serial_outc(c);
				*cp++ = c;
			}
		}
	}
	if(buf != NULL)
		*cp = '\0';
	return buf;
}
#endif

int cmd_parse(cmds_t cmds[], char *line)
{
	cmds_t *cmdp;
	char *argv[NARGS],*cp;
	char **pargv;
	int argc,i;

	/* Remove cr/lf */
	if((cp = strchr(line,'\n')) != NULL)
		*cp = '\0';
	if((cp = strchr(line,'\r')) != NULL)
		*cp = '\0';

	for(argc = 0;argc < NARGS;argc++)
		argv[argc] = NULL;

	for(argc = 0;argc < NARGS;){
		/* Skip leading white space */
		while(*line == ' ' || *line == '\t')
			line++;
		if(*line == '\0')
			break;
		argv[argc++] = line;	/* Beginning of token */

		/* Find space or tab. If not present,
		 * then we've already found the last
		 * token.
		 */
		if((cp = strchr(line,' ')) == NULL && (cp = strchr(line,'\t')) == NULL) {
			break;
		}
		*cp++ = '\0';
		line = cp;
	}
	if (argc < 1) {		/* empty command line */
		argc = 1;
		argv[0] = "";
	}
	/* Look up command in table; prefix matches are OK */
	for(cmdp = cmds;cmdp->name != NULL;cmdp++){
		if(strncmp(argv[0],cmdp->name,strlen(argv[0])) == 0) {
			break;
		}
	}
	if(cmdp->name == NULL) {
		if(cmdp->help != NULL)
			printf("%s\n",cmdp->help);
		return -1;
	}
	if(argc < cmdp->nargs) {
		/* Insufficient arguments */
		printf("Usage: %s\n",cmdp->usage);
		return -1;
	}
	if(cmdp->func == NULL)
		return 0;
	return (*cmdp->func)(argc,argv);
}

int	cmd_proc(void)
{
	static uint8 line[LINE_LEN];
#if defined(TCSUPPORT_FWC_ENV)
	restart_time_flags = 1;
	restart_time_count = RESTART_TIME_COUNT;
#endif

	while (1) {
		printf("%s> ", cmd_prompt);
#if defined(TCSUPPORT_CT_BOOTLOADER_UPGRADE)
		cmd_gets(line, LINE_LEN,CMD);
#else
		cmd_gets(line, LINE_LEN);
#endif
		printf("\n");

#if defined(TCSUPPORT_FWC_ENV)
		if(0 == strlen(line)) {/*filter enter key*/
			continue;
		}
		if(0 == cmd_parse(CMDS, line)) {/*parse command success*/
			restart_time_count = RESTART_TIME_COUNT;
		}
#else
		cmd_parse(CMDS, line);
#endif
	}
}
#if defined(TCSUPPORT_CT_BOOTLOADER_UPGRADE)
void	user_Auth(void)
{
	int i;
	uint8 UserName[LINE_LEN];
	uint8 Pwd[LINE_LEN];
#ifdef TCSUPPORT_RESERVEAREA_EXTEND
	proline_Para proline;
	char user_name[CFEUSRNAMELEN] = {0};
	char pass_word[CFEPWDLEN] = {0};
#else
	char user_name[USERNAME_PASSWD_LEN] = {0};
	char pass_word[USERNAME_PASSWD_LEN] = {0};
#endif
	unsigned long reserved_base = 0;
//	char *flash_user_name_addr = (char *)FLASH_USER_NAME;
//	char *flash_pass_word_addr = (char *)FLASH_PASS_WORD;
	char *flash_user_name_addr;
	char *flash_pass_word_addr;
	
	if (IS_SPIFLASH){
#ifdef TCSUPPORT_RESERVEAREA_EXTEND
			reserved_base = (mtd.size - 7 * mtd.erasesize);
#else
		reserved_base = (mtd.size - 4 * mtd.erasesize);
#endif
	} else {
	#ifdef TCSUPPORT_BB_NAND
		reserved_base = (1<<ra.flash->chip_shift) - 4 * (1<<ra.flash->erase_shift);
	#endif	
	}

#ifdef TCSUPPORT_RESERVEAREA_EXTEND
	flash_user_name_addr = (char *)(((PROLINE_CWMPPARA_RA_OFFSET + reserved_base)|flash_base) + ((char *)&proline.cfeusrname[0] - (char *)&proline.flag));
	flash_pass_word_addr = flash_user_name_addr + CFEUSRNAMELEN;

	for(i=0; i<CFEUSRNAMELEN-1; i++)
	{	
		user_name[i] = READ_FLASH_BYTE(flash_user_name_addr+i);
	}
	user_name[i] = '\0';
	for(i=0; i<CFEPWDLEN-1; i++)
	{	
		pass_word[i] = READ_FLASH_BYTE(flash_pass_word_addr+i);
	}
	pass_word[i] = '\0';

	if((user_name[0] == 0 && user_name[1] == 0 && user_name[2] == 0)
		|| (user_name[0] == 0xff && user_name[1] == 0xff && user_name[2] == 0xff))
	{
		memset(user_name, 0, CFEUSRNAMELEN);
		snprintf(user_name,sizeof(user_name),"%s",DEFAULT_CFE_USERNAME);
	}
	if((pass_word[0] == 0 && pass_word[1] == 0 && pass_word[2] == 0)
		|| (pass_word[0] == 0xff && pass_word[1] == 0xff && pass_word[2] == 0xff))
	{
		memset(pass_word, 0, CFEPWDLEN);
		snprintf(pass_word,sizeof(pass_word),"%s",DEFAULT_CFE_PWD);
	}
#else
	flash_user_name_addr = (char *)((USERNAMEPASSWD_RA_OFFSET+reserved_base)|flash_base);
	flash_pass_word_addr = (char *)((USERNAMEPASSWD_RA_OFFSET+USERNAME_PASSWD_LEN+reserved_base)|flash_base);

	for(i=0; i<USERNAME_PASSWD_LEN-1; i++)
	{	
//		user_name[i] = flash_user_name_addr[i];
//		pass_word[i] = flash_pass_word_addr[i];
		user_name[i] = READ_FLASH_BYTE(flash_user_name_addr+i);
		pass_word[i] = READ_FLASH_BYTE(flash_pass_word_addr+i);
	}
	user_name[i] = '\0';	
	pass_word[i] = '\0';

//	printf("user_Auth: flash---user_name = %s\n\n", user_name);
//	printf("user_Auth: flash---pass_word = %s\n\n", pass_word);
	
#endif

	if(strncmp(CT_USER_NAME, user_name, strlen(CT_USER_NAME)) != 0)
	{
		flash_user_name_addr = (char *)MRD_USER_NAME;	
		flash_pass_word_addr = (char *)MRD_PASS_WORD;	
		for(i=0; i<USERNAME_PASSWD_LEN-1; i++)
		{
			user_name[i] = READ_FLASH_BYTE(flash_user_name_addr+i);
			pass_word[i] = READ_FLASH_BYTE(flash_pass_word_addr+i);
		}
		user_name[i] = '\0';	
		pass_word[i] = '\0';
	}


	while (1) {
		memset(UserName, 0, LINE_LEN);
		memset(Pwd, 0, LINE_LEN);
		printf("UserName: ");
		cmd_gets(UserName, LINE_LEN, CMD);
		printf("\n");
		printf("Password: ");
		cmd_gets(Pwd, LINE_LEN, PWD);
		i = trim(UserName);
		printf("\n\n");
		
#ifdef TCSUPPORT_RESERVEAREA_EXTEND
		if((!strncmp(&UserName[i], user_name, CFEUSRNAMELEN))&&(!strncmp(Pwd, pass_word, CFEPWDLEN)))
#else
		if((!strncmp(&UserName[i], user_name, USERNAME_PASSWD_LEN))&&(!strncmp(Pwd, pass_word, USERNAME_PASSWD_LEN)))
#endif
		{
			authed = 1;
			break;
		}
	}
}

int trim(char *buf)
{
	int i,j;
	int flag_i=1, flag_j=1;
	for (i=0,j=strlen(buf)-1; i<=j; i++,j--)
	{
		if (flag_i)
		{	
			if((buf[i] != ' ') && (buf[i] != '\t'))
			flag_i = 0;
		}
		if (flag_j)
		{
			if((buf[j] == ' ') || (buf[j] == '\t'))
				buf[j] = 0;
			else
				flag_j = 0;
		}
		if ((flag_i == 0) && (flag_j == 0))
			break;
	}
	return i;
}
#endif

#if defined(TCSUPPORT_FWC_ENV)

int g_boot_debug_level = E_BOOT_NO_INFO_LEVLE;

static int do_boot_debug_level(int argc, char *argv[])
{
    g_boot_debug_level = strtoul((const char*)(argv[1]), (char **)NULL, 16);
    
	printf("g_boot_debug_level = 0x%X.\n", g_boot_debug_level);
    return 0;
}

static int do_boot_reset(int argc, char *argv[])
{
    unsigned long             addr_value = 0;
    unsigned long reset_control_register = RESET_CONTROL_REGISTER;

    addr_value = tc_inl(reset_control_register);
    addr_value |= (1 << SYSTEM_SOFTWARE_RESET_OFFSET);

    tc_outl(reset_control_register, addr_value);

    return 0;
}

static int do_boot_version(int argc, char *argv[])
{
    unsigned int       i = 0;
	unsigned char buffer[FH_NW_PRODUCT_ENV_SIZE];
    memset(buffer, 0, sizeof(buffer));

    for(i=0; i <(FH_NW_PRODUCT_ENV_SIZE -1) ; i++)
    {
        buffer[i] = READ_FLASH_BYTE(flash_base+0xff04 + i);
    }

    buffer[i] = '\0';

    printf("bootversion = %s.\n",buffer);	

    return 0;
}

static int do_flash_erase(int argc, char *argv[])
{
    unsigned long        len = 0;
    unsigned long start_addr = 0;

    start_addr = strtoul((const char*)(argv[1]), (char **)NULL, 16);
    len        = strtoul((const char*)(argv[2]), (char **)NULL, 16);

    printf("start_addr = 0x%x, len = 0x%x.\n",start_addr, len);

    flash_erase(start_addr, len);
}

/************************************************************************************************/
static int printf_flash_env_info(flash_env_personality_parm_partition_info_t *pt_info, unsigned int flag, unsigned int line)
{
    if((g_boot_debug_level & E_BOOT_DEBUG_INFO_LEVLE) || (E_BOOT_DEBUG_INFO_LEVLE == flag))
    {
		int i = 0;
		int j = 0;
		printf("*****************line = %d**************************************.\n", line);
		printf("snoui = %s\t", 			pt_info->env.sn_oui);
		printf("hw_cfg = %s\t",			pt_info->env.hw_cfg);
		printf("ethaddr = %s\t", 	 pt_info->env.eth_addr);
		printf("f_reserve0 = %s\t", pt_info->env.f_reserve0);

		printf("\n");
		printf("f_reserve1 = %s\t", pt_info->env.f_reserve1);
		printf("f_reserve2 = %s\t", pt_info->env.f_reserve2);
		printf("f_reserve3 = %s\t", pt_info->env.f_reserve3);
		printf("f_reserve4 = %s\t", pt_info->env.f_reserve4);

		printf("\n");
		printf("f_reserve5 = %s\t", pt_info->env.f_reserve5);
		printf("f_reserve6 = %s\t", pt_info->env.f_reserve6);
		printf("f_reserve7 = %s\t", pt_info->env.f_reserve7);
		printf("f_reserve8 = %s\t", pt_info->env.f_reserve8);

		printf("\n");
		printf("f_reserve9 = %s\t", pt_info->env.f_reserve9);
		printf("f_reserve10 = %s\t", pt_info->env.f_reserve10);
		printf("f_reserve11 = %s\t", pt_info->env.f_reserve11);
		printf("f_reserve12 = %s\t", pt_info->env.f_reserve12);
		
		printf("\n");
		printf("f_reserve13 = %s\t", pt_info->env.f_reserve13);
		printf("f_reserve14 = %s\t", pt_info->env.f_reserve14);
		printf("f_reserve15 = %s\t", pt_info->env.f_reserve15);
		printf("f_reserve16 = %s\t", pt_info->env.f_reserve16);

		printf("\n");
		printf("f_reserve17 = %s\t", pt_info->env.f_reserve17);
		printf("f_reserve18 = %s\t", pt_info->env.f_reserve18);
		printf("f_reserve19 = %s\t", pt_info->env.f_reserve19);	
		printf("\n");
		for(i=0; i<FH_NW_PRODUCT_CHECK_VENDOR_SIZE; i++)
		{
			j = i;
	        if(0 == j%4)
	        {
	            printf("\n");
	        }
			printf("vendor[%d] = 0x%X.\t", i, pt_info->check_info.vendor[i]);
		}
		printf("\n");
		printf("magic = 0x%X, crc_check = 0x%X.\n",pt_info->check_info.magic, pt_info->check_info.crc_check);
	}
	
	return 0;
}

static int printf_dual_image_info(dual_image_personality_parm_partition_info_t *pt_info, unsigned int flag, unsigned int line)
{
    if((g_boot_debug_level & E_BOOT_DEBUG_INFO_LEVLE) || (E_BOOT_DEBUG_INFO_LEVLE == flag))
    {
		int i = 0;
		int j = 0;
		printf("*****************line = %d**************************************.\n", line);
		printf("kernel_commit = %d\t", pt_info->dual_image_element.kernel_commit);
		printf("kernel_active = %d\t", pt_info->dual_image_element.kernel_active);
		printf("rootfs_commit = %d\t", pt_info->dual_image_element.rootfs_commit);
		printf("rootfs_active = %d\t", pt_info->dual_image_element.rootfs_active);
		printf("\n");
		printf("app1_commit = %d\t",   pt_info->dual_image_element.app1_commit);
		printf("app1_active = %d\t",   pt_info->dual_image_element.app1_active);
		printf("app2_commit = %d\t",   pt_info->dual_image_element.app2_commit);
		printf("app2_active = %d\t",   pt_info->dual_image_element.app2_active);
		printf("\n");
		for(i=0; i<FH_NW_PRODUCT_DUAL_IMAGE_FLAG_RESERVE_SIZE; i++)
		{
			j = i;
	        if(0 == j%4)
	        {
	            printf("\n");
	        }
			printf("dual_reserve[%d] = 0x%X.\t", i, pt_info->dual_image_element.reserve[i]);
		}
		printf("\n");
		for(i=0; i<FH_NW_PRODUCT_CHECK_RESERVE_SIZE; i++)
		{
			j = i;
	        if(0 == j%4)
	        {
	            printf("\n");
	        }
			printf("check_reserve[%d] = 0x%X.\t", i, pt_info->check_info.reserve[i]);
		}
		printf("\n");
		for(i=0; i<FH_NW_PRODUCT_CHECK_VENDOR_SIZE; i++)
		{
			j = i;
	        if(0 == j%4)
	        {
	            printf("\n");
	        }
			printf("vendor[%d] = 0x%X.\t", i, pt_info->check_info.vendor[i]);
		}
		printf("\n");
		printf("magic = 0x%X, crc_check = 0x%X.\n",pt_info->check_info.magic, pt_info->check_info.crc_check);
	}

	return 0;
}

static int dual_image_offset_convert_value(unsigned long offset, unsigned long vaule, unsigned char *flag)
{
    switch(offset)
    {
        case KERNEL_COMMIT_OFFSET:
        case ROOTFS_COMMIT_OFFSET:
        case APP1_COMMIT_OFFSET:
        case APP2_COMMIT_OFFSET:
        {
            if(E_FH_NW_PRODUCT_COMMIT_B != vaule)
            {
                vaule = E_FH_NW_PRODUCT_COMMIT_A;
            }
            break;
        }
        case KERNEL_ACTIVE_OFFSET:
        case ROOTFS_ACTIVE_OFFSET:
        case APP2_ACTIVE_OFFSET:
        case APP1_ACTIVE_OFFSET:
        {
            if((E_FH_NW_PRODUCT_ACTIVE_A != vaule) && (E_FH_NW_PRODUCT_ACTIVE_B != vaule))
            {
                vaule = E_FH_NW_PRODUCT_ACTIVE_DEFAULT;
            }
            break;
        }
        default:
        {
            return E_PERSONALITY_PARM_INVALID_ARGUMENT;
        }
    }

    *flag = (vaule & FH_NW_PRODUCT_DUAL_IMAGE_MASK);
    return E_PERSONALITY_PARM_RIGHT_EXECUTE;
}

static int personality_parm_check_vendor_define(personality_parm_cmd_base_addr_t *pt_info)
{
	unsigned long 	A_base_addr = 0;
	unsigned long 	B_base_addr = 0;
    unsigned long 	  	ret_len = 0;
	unsigned char         flag  = 0;
	unsigned char  A_vaild_flag = INVLID;
	unsigned char  B_vaild_flag = INVLID;
	check_personality_parm_partition_info_t 	A_policy;
	check_personality_parm_partition_info_t 	B_policy;
	memset(&A_policy, 0, sizeof(A_policy));
	memset(&B_policy, 0, sizeof(B_policy));
	switch(pt_info->read_base_addr)
	{
		case FH_NW_PRODUCT_DUAL_IMAGE_READ_BASE_ADDR:
		case FH_NW_PRODUCT_DUAL_IMAGE_WRITE_BASE_ADDR:
		{ 
			A_base_addr = (pt_info->read_base_addr  + FH_NW_PRODUCT_DUAL_IMAGE_FLAG_CHECK_INFO_OFFSET);
			B_base_addr = (pt_info->write_base_addr + FH_NW_PRODUCT_DUAL_IMAGE_FLAG_CHECK_INFO_OFFSET);
			break;
		}
		case FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR:
		case FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR:
		{
			A_base_addr = (pt_info->read_base_addr  + FH_NW_PRODUCT_FLASH_ENV_INFO_OFFSET);
			B_base_addr = (pt_info->write_base_addr + FH_NW_PRODUCT_FLASH_ENV_INFO_OFFSET);
			break;
		}
		default:
		{
            pt_info->both_valid_flag  = INVLID;
			return FH_NW_PRODUCT_BREAK;
		}
	}
    BOOT_DEBUG_INFO("A_base_addr = 0x%X, B_base_addr = 0x%X.\n", A_base_addr, B_base_addr);
	flash_read(A_base_addr, FH_NW_PRODUCT_CHECK_SIZE, &ret_len,  (unsigned char *)&A_policy);
	flash_read(B_base_addr, FH_NW_PRODUCT_CHECK_SIZE, &ret_len,  (unsigned char *)&B_policy);

	/*vendor define  compare*/
	if(0 == memcmp(A_policy.vendor, FH_NW_PRODUCT_VENDOR_DEFINE, FH_NW_PRODUCT_CHECK_VENDOR_SIZE))
	{
		A_vaild_flag  = VALID;
	}
	if(0 == memcmp(B_policy.vendor, FH_NW_PRODUCT_VENDOR_DEFINE, FH_NW_PRODUCT_CHECK_VENDOR_SIZE))
	{
		B_vaild_flag  = VALID;
	}
	
    BOOT_DEBUG_INFO("A_vaild_flag = %d, B_vaild_flag = %d.\n", A_vaild_flag, B_vaild_flag);
	if((VALID == A_vaild_flag) && (INVLID == B_vaild_flag))
	{
		/******wait check A crc*********/
		pt_info->A_valid_flag = FH_NW_PRODUCT_POSSIBLE_VALID;
		return FH_NW_PRODUCT_CONTINUE;
	}
	else if((INVLID == A_vaild_flag) && (VALID == B_vaild_flag))
	{
		/******wait check B crc*********/
		pt_info->B_valid_flag	= FH_NW_PRODUCT_POSSIBLE_VALID;
		return FH_NW_PRODUCT_CONTINUE;
	}
	else if((INVLID == A_vaild_flag) && (INVLID == B_vaild_flag))
	{
		/******read A, write B*********/
		pt_info->valid_read_addr  = pt_info->read_base_addr;
		pt_info->valid_write_addr = pt_info->write_base_addr;
		pt_info->both_valid_flag  = INVLID;
		return FH_NW_PRODUCT_BREAK;
	}	

	/******wait check A&&B crc*********/
	pt_info->A_valid_flag    = FH_NW_PRODUCT_POSSIBLE_VALID;
	pt_info->B_valid_flag    = FH_NW_PRODUCT_POSSIBLE_VALID;
	
	return FH_NW_PRODUCT_CONTINUE;
}

static int personality_parm_check_crc(personality_parm_cmd_base_addr_t *pt_info)
{

	unsigned long   A_base_addr = 0;
	unsigned long   B_base_addr = 0;
	unsigned long       ret_len = 0;
	unsigned int    	  A_crc = 0;
	unsigned int    	  B_crc = 0;
	unsigned char         flag  = 0;
	unsigned long check_data_size = 0;
	unsigned char  A_vaild_flag = INVLID;
	unsigned char  B_vaild_flag = INVLID;
    dual_image_personality_parm_partition_info_t A_dual_image;
    dual_image_personality_parm_partition_info_t B_dual_image;
    flash_env_personality_parm_partition_info_t  A_env_info;
    flash_env_personality_parm_partition_info_t  B_env_info;
	memset(&A_dual_image, 0, sizeof(A_dual_image));
	memset(&B_dual_image, 0, sizeof(B_dual_image));
	memset(&A_env_info, 0, sizeof(A_env_info));
	memset(&B_env_info, 0, sizeof(B_env_info));

    BOOT_DEBUG_INFO("both_valid_flag = %d.\n", pt_info->both_valid_flag);

	if(INVLID == pt_info->both_valid_flag)
	{
		return FH_NW_PRODUCT_BREAK;
	}
	
	switch(pt_info->read_base_addr)
	{
	    case FH_NW_PRODUCT_DUAL_IMAGE_READ_BASE_ADDR:
	    case FH_NW_PRODUCT_DUAL_IMAGE_WRITE_BASE_ADDR:
	    { 
	    	flag = FH_NW_PRODUCT_DUAL_IMAGE_PARTITION;
	        A_base_addr = pt_info->read_base_addr;
	        B_base_addr = pt_info->write_base_addr;
	        check_data_size = (sizeof(dual_image_personality_parm_partition_info_t) - sizeof(unsigned int));
	        break;
	    }
		case FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR:
		case FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR:
		{
			flag = FH_NW_PRODUCT_FLASH_ENV_PARTITION;
			A_base_addr = pt_info->read_base_addr;
			B_base_addr = pt_info->write_base_addr;
			check_data_size = (sizeof(flash_env_personality_parm_partition_info_t) - sizeof(unsigned int));
			break;
		}
	    default:
	    {
	    	pt_info->both_valid_flag = INVLID;
	        return FH_NW_PRODUCT_BREAK;
	    }
	}
	
    BOOT_DEBUG_INFO("A_base_addr = 0x%X, B_base_addr = 0x%X.\n", A_base_addr, B_base_addr);

    /*calc crc*/
	if(FH_NW_PRODUCT_POSSIBLE_VALID == pt_info->A_valid_flag)
	{
        if(FH_NW_PRODUCT_DUAL_IMAGE_PARTITION == flag)
        {
            flash_read(A_base_addr, sizeof(A_dual_image), &ret_len,  (unsigned char *)&A_dual_image);
            A_crc = crc32buf((char *)&A_dual_image, check_data_size, 1);
			BOOT_DEBUG_INFO("A_crc = 0x%X, A_dual_image.check_info.crc_check = 0x%X.\n",A_crc, A_dual_image.check_info.crc_check);
            if(A_crc == A_dual_image.check_info.crc_check)
            {
                A_vaild_flag = VALID;
            }
        }
        else if(FH_NW_PRODUCT_FLASH_ENV_PARTITION == flag)
        {
			flash_read(A_base_addr, sizeof(A_env_info), &ret_len,  (unsigned char *)&A_env_info);
			A_crc = crc32buf((char *)&A_env_info, check_data_size, 1);
			BOOT_DEBUG_INFO("A_crc = 0x%X, A_env_info.check_info.crc_check = 0x%X.\n",A_crc, A_env_info.check_info.crc_check);
			if(A_crc == A_env_info.check_info.crc_check)
			{
				A_vaild_flag = VALID;
			}
        }
	}

	if(FH_NW_PRODUCT_POSSIBLE_VALID == pt_info->B_valid_flag)
	{
        if(FH_NW_PRODUCT_DUAL_IMAGE_PARTITION == flag)
        {
			flash_read(B_base_addr, sizeof(B_dual_image), &ret_len,  (unsigned char *)&B_dual_image);
			B_crc = crc32buf((char *)&B_dual_image, check_data_size, 1);
			BOOT_DEBUG_INFO("B_crc = 0x%X, B_dual_image.check_info.crc_check = 0x%X.\n",B_crc, B_dual_image.check_info.crc_check);
			if(B_crc == B_dual_image.check_info.crc_check)
			{
				B_vaild_flag = VALID;
			}
        }
		else if(FH_NW_PRODUCT_FLASH_ENV_PARTITION == flag)
		{
			flash_read(B_base_addr, sizeof(B_env_info), &ret_len,  (unsigned char *)&B_env_info);
			B_crc = crc32buf((char *)&B_env_info, check_data_size, 1);
			BOOT_DEBUG_INFO("B_crc = 0x%X, B_env_info.check_info.crc_check = 0x%X.\n",B_crc, B_env_info.check_info.crc_check);
			if(B_crc == B_env_info.check_info.crc_check)
			{
				B_vaild_flag = VALID;
			}
		}
	}
	
    BOOT_DEBUG_INFO("A_vaild_flag = %d, B_vaild_flag = %d.\n", A_vaild_flag, B_vaild_flag);

	/*crc && vendor synthetic judgmen*/
	if((VALID == A_vaild_flag) && (INVLID == B_vaild_flag))
	{
		/******read A, write B*********/
		pt_info->valid_read_addr  = pt_info->read_base_addr;
		pt_info->valid_write_addr = pt_info->write_base_addr;
		return FH_NW_PRODUCT_BREAK;
	}
	else if((INVLID == A_vaild_flag) && (VALID == B_vaild_flag))
	{
    	/******read B, write A*********/
		pt_info->valid_read_addr  = pt_info->write_base_addr;
    	pt_info->valid_write_addr = pt_info->read_base_addr;		
		return FH_NW_PRODUCT_BREAK;
	}
	else if((INVLID == A_vaild_flag) && (INVLID == B_vaild_flag))
	{
    	/******read A, write B*********/
		pt_info->valid_read_addr  = pt_info->read_base_addr;
		pt_info->valid_write_addr = pt_info->write_base_addr;
		pt_info->both_valid_flag  = INVLID;
		return FH_NW_PRODUCT_BREAK;
	}
	
	/**********wait check magic********/	
    pt_info->both_valid_flag  = VALID;
    
	return FH_NW_PRODUCT_CONTINUE;
}

static int personality_parm_check_magic(personality_parm_cmd_base_addr_t *pt_info)
{
	unsigned long 	A_base_addr = 0;
	unsigned long 	B_base_addr = 0;
    unsigned long 	  	ret_len = 0;
	unsigned char  A_vaild_flag = INVLID;
	unsigned char  B_vaild_flag = INVLID;
	check_personality_parm_partition_info_t 	A_policy;
	check_personality_parm_partition_info_t 	B_policy;

	memset(&A_policy, 0, sizeof(A_policy));
	memset(&B_policy, 0, sizeof(B_policy));

	BOOT_DEBUG_INFO("both_valid_flag = %d.\n", pt_info->both_valid_flag);
	if(VALID != pt_info->both_valid_flag)
	{
		return 0;
	}
	
	switch(pt_info->read_base_addr)
	{
		case FH_NW_PRODUCT_DUAL_IMAGE_READ_BASE_ADDR:
		case FH_NW_PRODUCT_DUAL_IMAGE_WRITE_BASE_ADDR:
		{ 
			A_base_addr = (pt_info->read_base_addr + FH_NW_PRODUCT_DUAL_IMAGE_FLAG_CHECK_INFO_OFFSET);
			B_base_addr = (pt_info->write_base_addr + FH_NW_PRODUCT_DUAL_IMAGE_FLAG_CHECK_INFO_OFFSET);
			break;
		}
		case FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR:
		case FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR:
		{
			A_base_addr = (pt_info->read_base_addr  + FH_NW_PRODUCT_FLASH_ENV_INFO_OFFSET);
			B_base_addr = (pt_info->write_base_addr + FH_NW_PRODUCT_FLASH_ENV_INFO_OFFSET);
			break;
		}
		default:
		{
			return -1;
		}
	}
	
    BOOT_DEBUG_INFO("A_base_addr = 0x%X, B_base_addr = 0x%X.\n", A_base_addr, B_base_addr);

	flash_read(A_base_addr, FH_NW_PRODUCT_CHECK_SIZE, &ret_len,  (unsigned char *)&A_policy);
	flash_read(B_base_addr, FH_NW_PRODUCT_CHECK_SIZE, &ret_len,  (unsigned char *)&B_policy);
	
	BOOT_DEBUG_INFO("A_policy.magic = 0x%X, B_policy.magic = 0x%X.\n", A_policy.magic, B_policy.magic);
	/*crc && vendor &&magic  synthetic judgmen*/
	if((A_policy.magic == 0) && (B_policy.magic == MAX_MAGIC_VALUE))
	{
		/******read A, write B*********/
		pt_info->valid_read_addr  = pt_info->read_base_addr;
		pt_info->valid_write_addr = pt_info->write_base_addr;
	}
	else if((A_policy.magic == MAX_MAGIC_VALUE) && (B_policy.magic == 0))
	{
		/******read B, write A*********/
		pt_info->valid_read_addr  = pt_info->write_base_addr;
		pt_info->valid_write_addr = pt_info->read_base_addr;
	}
	else if(A_policy.magic >= B_policy.magic)
	{
		/******read A, write B*********/
		pt_info->valid_read_addr  = pt_info->read_base_addr;
		pt_info->valid_write_addr = pt_info->write_base_addr;
	}
	else
	{
		/******read B, write A*********/
		pt_info->valid_read_addr  = pt_info->write_base_addr;
		pt_info->valid_write_addr = pt_info->read_base_addr;
	}

	return 	0;
}

int personality_parm_get_partition_valid(personality_parm_cmd_base_addr_t *pt_info)
{
	int iret = 0;

    iret = personality_parm_check_vendor_define(pt_info);
    if(FH_NW_PRODUCT_CONTINUE != iret)
    {
    	return 0;
    }
    BOOT_DEBUG_INFO("need check_crc.\n");
    
    iret = personality_parm_check_crc(pt_info);
    if(FH_NW_PRODUCT_CONTINUE != iret)
    {
    	return 0;
    }

    BOOT_DEBUG_INFO("need check_magic.\n");
	iret = personality_parm_check_magic(pt_info);
	if(0 != iret)
	{
		printf("check magic fail.\n");
	}
	
	BOOT_DEBUG_INFO("check magic ok.\n");
	
	return iret;
}

static int get_dual_image_entry(personality_parm_t* pt_info)
{
    int            iret = E_PERSONALITY_PARM_RIGHT_EXECUTE;
    unsigned char  flag = 0;
    unsigned long value = 0;
    personality_parm_cmd_base_addr_t info;
    
	memset(&info, 0, sizeof(info));
	info.read_base_addr  = pt_info->read_base_addr;
	info.write_base_addr = pt_info->write_base_addr;
	iret = personality_parm_get_partition_valid(&info);
	if(0 != iret)
	{
    	pt_info->element.dual_image_flag = 0;
		return E_PERSONALITY_PARM_INVALID_ARGUMENT;
	}
	
	BOOT_DEBUG_INFO("both_valid_flag = %d. v_offset = 0x%X.\n", info.both_valid_flag, pt_info->v_offset);

	if(INVLID == info.both_valid_flag)
	{
		switch(pt_info->v_offset)
		{
			case KERNEL_COMMIT_OFFSET:
			case ROOTFS_COMMIT_OFFSET:
			case APP1_COMMIT_OFFSET:
			case APP2_COMMIT_OFFSET:
			{
				pt_info->element.dual_image_flag = E_FH_NW_PRODUCT_COMMIT_A;
                break;
			}
			case KERNEL_ACTIVE_OFFSET:
			case ROOTFS_ACTIVE_OFFSET:
			case APP2_ACTIVE_OFFSET:
			case APP1_ACTIVE_OFFSET:
			{
            	pt_info->element.dual_image_flag = E_FH_NW_PRODUCT_ACTIVE_DEFAULT;
            	break;
			}
			default:
			{
				pt_info->element.dual_image_flag = 0;
				break;;
			}
		}
	}
	else
	{
        pt_info->read_base_addr  = info.valid_read_addr;
        pt_info->write_base_addr = info.valid_write_addr;

		BOOT_DEBUG_INFO("read = 0x%X, write = 0x%X.\n", pt_info->read_base_addr, pt_info->write_base_addr);
		BOOT_DEBUG_INFO("v_offset = 0x%X.\n",  pt_info->v_offset);
		
		value = READ_FLASH_BYTE(pt_info->read_base_addr + pt_info->v_offset);
		iret  = dual_image_offset_convert_value(pt_info->v_offset, value, &flag);  
		if(E_PERSONALITY_PARM_RIGHT_EXECUTE != iret)
		{
			return E_PERSONALITY_PARM_INVALID_ARGUMENT;
		}
		pt_info->element.dual_image_flag = (flag & FH_NW_PRODUCT_DUAL_IMAGE_MASK);
		BOOT_DEBUG_INFO("value = %d, flag = %d.\n", value, flag);
	}
	
	BOOT_DEBUG_INFO("dual_image_flag = %d.\n",pt_info->element.dual_image_flag);
	return E_PERSONALITY_PARM_RIGHT_EXECUTE;
}

static int offset_to_dual_image_info(personality_parm_t *offset_info, dual_image_personality_parm_partition_info_t *pt_info)
{
	switch(offset_info->v_offset)
	{
        case KERNEL_COMMIT_OFFSET:
        {
        	pt_info->dual_image_element.kernel_commit = offset_info->element.dual_image_flag;
            break;
        }
        case ROOTFS_COMMIT_OFFSET:
        {
			pt_info->dual_image_element.rootfs_commit = offset_info->element.dual_image_flag;
            break;
        }
        case APP1_COMMIT_OFFSET:
        {
			pt_info->dual_image_element.app1_commit = offset_info->element.dual_image_flag;
        	break;
        }
        case APP2_COMMIT_OFFSET:
        {
			pt_info->dual_image_element.app2_commit = offset_info->element.dual_image_flag;
			break;
        }
        case KERNEL_ACTIVE_OFFSET:
        {
			pt_info->dual_image_element.kernel_active = offset_info->element.dual_image_flag;
			break;
        }
        case ROOTFS_ACTIVE_OFFSET:
        {
			pt_info->dual_image_element.rootfs_active = offset_info->element.dual_image_flag;
			break;
        }
        case APP2_ACTIVE_OFFSET:
        {
			pt_info->dual_image_element.app2_active = offset_info->element.dual_image_flag;
			break;
        }
        case APP1_ACTIVE_OFFSET:
        {
			pt_info->dual_image_element.app1_active = offset_info->element.dual_image_flag;
			break;
        }
        default:
        {
        	break;
        }
	}

	return 0;
	
}


static int set_dual_image_entry(personality_parm_t* pt_info)
{
	int            iret = E_PERSONALITY_PARM_RIGHT_EXECUTE;
	unsigned char  flag = 0;
	unsigned long value = 0;
	unsigned long ret_len = 0;
	unsigned long check_data_size = 0;
	unsigned int crc_check_sum = 0;
	personality_parm_cmd_base_addr_t info;
	dual_image_personality_parm_partition_info_t dual_info;
    unsigned long dual_info_size = sizeof(dual_image_personality_parm_partition_info_t);
    
    memset(&dual_info, 0, sizeof(dual_info));
    memset(&info, 0, sizeof(info));
	info.read_base_addr  = pt_info->read_base_addr;
	info.write_base_addr = pt_info->write_base_addr;
	iret = personality_parm_get_partition_valid(&info);
	if(0 != iret)
	{
		return E_PERSONALITY_PARM_INVALID_ARGUMENT;
	}
	
    pt_info->read_base_addr  = info.valid_read_addr;
    pt_info->write_base_addr = info.valid_write_addr;
    
    BOOT_DEBUG_INFO("read = 0x%X, write = 0x%X.\n", pt_info->read_base_addr, pt_info->write_base_addr);
    BOOT_DEBUG_INFO("info.both_valid_flag = %d.\n", info.both_valid_flag);

	if(INVLID != info.both_valid_flag)
	{
		flash_read(pt_info->read_base_addr, dual_info_size, &ret_len, (unsigned char *)&dual_info);
	}
	
    printf_dual_image_info(&dual_info, E_BOOT_NO_INFO_LEVLE, __LINE__);
    
	offset_to_dual_image_info(pt_info, &dual_info);
    memcpy(dual_info.check_info.vendor, FH_NW_PRODUCT_VENDOR_DEFINE, FH_NW_PRODUCT_CHECK_VENDOR_SIZE);
    if(dual_info.check_info.magic != 0xFFFF)
    {
        dual_info.check_info.magic += 1;
    }
    else
    {
    	dual_info.check_info.magic = 0;
    }
    
    printf_dual_image_info(&dual_info, E_BOOT_NO_INFO_LEVLE, __LINE__);
    
    check_data_size = (dual_info_size - sizeof(unsigned int));
    crc_check_sum = crc32buf((char *)&dual_info, check_data_size, 1);
    
    dual_info.check_info.crc_check = crc_check_sum;

    printf_dual_image_info(&dual_info, E_BOOT_NO_INFO_LEVLE, __LINE__);
    
    flash_erase(pt_info->write_base_addr, FH_NW_PRODUCT_BLOCK_SIZE);
    flash_write(pt_info->write_base_addr, dual_info_size, &ret_len, (const unsigned char *)&dual_info);  

	return E_PERSONALITY_PARM_RIGHT_EXECUTE;
}
/*********************************************************************************/
static int set_flash_env_entry(personality_parm_t* pt_info)
{
	int            iret = E_PERSONALITY_PARM_RIGHT_EXECUTE;
	unsigned char  flag = 0;
	unsigned long value = 0;
	unsigned long ret_len = 0;
	unsigned long check_data_size = 0;
	unsigned int crc_check_sum = 0;
	personality_parm_cmd_base_addr_t info;
	flash_env_personality_parm_partition_info_t env_info;
    unsigned long env_info_size = sizeof(flash_env_personality_parm_partition_info_t);
    
	memset(&info, 0, sizeof(info));
    memset(&env_info, 0, sizeof(env_info));
	info.read_base_addr  = pt_info->read_base_addr;
	info.write_base_addr = pt_info->write_base_addr;
	iret = personality_parm_get_partition_valid(&info);
	if(0 != iret)
	{
		return E_PERSONALITY_PARM_INVALID_ARGUMENT;
	}
	
    pt_info->read_base_addr  = info.valid_read_addr;
    pt_info->write_base_addr = info.valid_write_addr;
    BOOT_DEBUG_INFO("read = 0x%X, write = 0x%X\n", pt_info->read_base_addr, pt_info->write_base_addr);
    BOOT_DEBUG_INFO("info.both_valid_flag = %d.\n", info.both_valid_flag);
	if(INVLID != info.both_valid_flag)
	{
		flash_read(pt_info->read_base_addr, env_info_size, &ret_len, (unsigned char *)&env_info);
	}
	else
	{
		strcpy(env_info.env.sn_oui,   "00");
		strcpy(env_info.env.eth_addr, "00:0A:C2:12:34:56");
		strcpy(env_info.env.hw_cfg,   "0x77");
	}
	/*****************************************************************************************/
    printf_flash_env_info(&env_info, E_BOOT_NO_INFO_LEVLE, __LINE__);
	memset((unsigned char *)&env_info.env + pt_info->v_offset, 0, FH_NW_PRODUCT_ENV_SIZE);
    strcpy((unsigned char *)&env_info.env + pt_info->v_offset, pt_info->element.env);
    
    printf_flash_env_info(&env_info, E_BOOT_NO_INFO_LEVLE, __LINE__);
    memcpy(env_info.check_info.vendor, FH_NW_PRODUCT_VENDOR_DEFINE, FH_NW_PRODUCT_CHECK_VENDOR_SIZE);

    BOOT_DEBUG_INFO("check_info.magic = 0x%X.\n", env_info.check_info.magic);
    
    if(env_info.check_info.magic != 0xFFFF)
    {
        env_info.check_info.magic += 1;
    }
    else
    {
    	env_info.check_info.magic = 0;
    }
    
    printf_flash_env_info(&env_info, E_BOOT_NO_INFO_LEVLE, __LINE__);
    check_data_size = (env_info_size - sizeof(unsigned int));
    crc_check_sum = crc32buf((char *)&env_info, check_data_size, 1);
    env_info.check_info.crc_check = crc_check_sum;

    printf_flash_env_info(&env_info, E_BOOT_NO_INFO_LEVLE, __LINE__);
    
    flash_erase(pt_info->write_base_addr, FH_NW_PRODUCT_BLOCK_SIZE);
    flash_write(pt_info->write_base_addr, env_info_size, &ret_len, (const unsigned char *)&env_info);  

	return E_PERSONALITY_PARM_RIGHT_EXECUTE;
}

static int get_flash_env_entry(personality_parm_t* pt_info)
{
	int            iret = E_PERSONALITY_PARM_RIGHT_EXECUTE;
	unsigned char  flag = 0;
	unsigned long value = 0;
	unsigned long ret_len = 0;
	personality_parm_cmd_base_addr_t info;
	flash_env_personality_parm_partition_info_t env_info;
    unsigned long env_info_size = sizeof(env_info);

	memset(&info, 0, sizeof(info));
	memset(&env_info, 0, sizeof(env_info));
	
	info.read_base_addr  = pt_info->read_base_addr;
	info.write_base_addr = pt_info->write_base_addr;
	iret = personality_parm_get_partition_valid(&info);
	if(0 != iret)
	{
		return E_PERSONALITY_PARM_INVALID_ARGUMENT;
	}
	
	/*default value*/
	if(INVLID == info.both_valid_flag)
	{
		switch(pt_info->v_offset)
		{
			case FH_FLASH_SNOUI_OFFSET:
			{
				strcpy(pt_info->element.env, "00");
				break;
			}
			case FH_FLASH_HWCFG_OFFSET:
			{
				strcpy(pt_info->element.env, "0x77");
				break;
			}
			case FH_FLASH_ETHADDR_OFFSET:
			{
				strcpy(pt_info->element.env, "00:0A:C2:12:34:56");
				break;
			}
			default:
			{
				break;
			}
		}
	}
	else
	{
		pt_info->read_base_addr  = info.valid_read_addr;
		pt_info->write_base_addr = info.valid_write_addr;
		BOOT_DEBUG_INFO("read = 0x%X, write = 0x%X.\n", pt_info->read_base_addr, pt_info->write_base_addr);
    	flash_read(pt_info->read_base_addr, env_info_size, &ret_len,  (unsigned char *)&env_info);
		memcpy(pt_info->element.env, (unsigned char *)&env_info.env + pt_info->v_offset, FH_NW_PRODUCT_ENV_SIZE);
	}

	return E_PERSONALITY_PARM_RIGHT_EXECUTE;
}

/*********************************************************************************/
int read_fh_nw_product_dual_image_flag_env(dual_image_flag_t *pt_info)
{
	int              iret = 0;
	unsigned char    flag = 0;
	unsigned long   value = 0;
	unsigned long ret_len = 0;
	personality_parm_cmd_base_addr_t info;
	if(NULL == pt_info)
	{
		return -1;
	}

	memset(&info, 0, sizeof(info));
	info.read_base_addr  = FH_NW_PRODUCT_DUAL_IMAGE_READ_BASE_ADDR;
	info.write_base_addr = FH_NW_PRODUCT_DUAL_IMAGE_WRITE_BASE_ADDR;
	iret = personality_parm_get_partition_valid(&info);
	if(0 != iret)
	{
		return -1;
	}

	/*default value*/
	if(INVLID != info.both_valid_flag)
	{
		BOOT_DEBUG_INFO("read = 0x%X, write = 0x%X\n", info.valid_read_addr, info.valid_write_addr, __LINE__);
		flash_read(info.valid_read_addr, sizeof(dual_image_flag_t), &ret_len,  (unsigned char *)pt_info);
	}

	return 0;
}

int read_fh_nw_product_flash_env(personality_parm_flash_env_t *pt_info)
{
	int              iret = 0;
	unsigned long ret_len = 0;
	personality_parm_cmd_base_addr_t info;
	if(NULL == pt_info)
	{
		return -1;
	}

	memset(&info, 0, sizeof(info));
	info.read_base_addr  = FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR;
	info.write_base_addr = FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR;
	iret = personality_parm_get_partition_valid(&info);
	if(0 != iret)
	{
		return -1;
	}

	/*default value*/
	if(INVLID != info.both_valid_flag)
	{
		BOOT_DEBUG_INFO("read = 0x%X, write = 0x%X\n", info.valid_read_addr, info.valid_write_addr, __LINE__);
		flash_read(info.valid_read_addr, sizeof(personality_parm_flash_env_t), &ret_len,  (unsigned char *)pt_info);
	}
	else
	{
		strcpy(pt_info->eth_addr, "00:0A:C2:12:34:56");
		strcpy(pt_info->hw_cfg,   "0x77");
		strcpy(pt_info->sn_oui,   "00");
	}
	
	return 0;
}

int read_fh_nw_product_memory_env(personality_parm_memory_env_t *pt_info)
{
	int             iret = 0;
	unsigned int       i = 0;
	unsigned char buffer[FH_NW_PRODUCT_ENV_SIZE];
	personality_parm_cmd_base_addr_t info;
	
	memset(buffer, 0, sizeof(buffer));
	memset(&info, 0, sizeof(info));
	if(NULL == pt_info)
	{
		return -1;
	}
	/*bootversion*/
	for(i=0; i <(FH_NW_PRODUCT_ENV_SIZE -1); i++)
	{
		buffer[i] = READ_FLASH_BYTE(flash_base+0xff04 + i);
	}
	buffer[i] = '\0';
	memcpy(&pt_info->boot_version, buffer, FH_NW_PRODUCT_ENV_SIZE);

	info.read_base_addr  = FH_NW_PRODUCT_DUAL_IMAGE_READ_BASE_ADDR;
	info.write_base_addr = FH_NW_PRODUCT_DUAL_IMAGE_WRITE_BASE_ADDR;
	iret = personality_parm_get_partition_valid(&info);
	if(0 != iret)
	{
		return -1;
	}
	
	if(FH_NW_PRODUCT_DUAL_IMAGE_READ_BASE_ADDR == info.valid_read_addr)
	{
		strcpy(&pt_info->m_reserve0, PERSONALITY_PARM_DUAL_IMAGE_READ_PARTITION);
	}
	else
	{
		strcpy(&pt_info->m_reserve0, PERSONALITY_PARM_DUAL_IMAGE_WRITE_PARTITION);
	}

	return 0;
}

static const personality_parm_table_t g_personality_parm_tab_b[]=
{
    {"set", "commit", 	"kernel",     E_FH_NW_PRODUCT_SET_COMMIT_KERNEL, 			FH_NW_PRODUCT_DUAL_IMAGE_READ_BASE_ADDR, FH_NW_PRODUCT_DUAL_IMAGE_WRITE_BASE_ADDR, KERNEL_COMMIT_OFFSET,  			FH_NW_PRODUCT_DUAL_IMAGE_FLAG_SIZE, NULL,            		set_dual_image_entry},
    {"set", "commit", 	"rootfs",     E_FH_NW_PRODUCT_SET_COMMIT_ROOTFS, 			FH_NW_PRODUCT_DUAL_IMAGE_READ_BASE_ADDR, FH_NW_PRODUCT_DUAL_IMAGE_WRITE_BASE_ADDR, ROOTFS_COMMIT_OFFSET,  			FH_NW_PRODUCT_DUAL_IMAGE_FLAG_SIZE, NULL,            		set_dual_image_entry},
    {"set", "commit", 	"app1",   	  E_FH_NW_PRODUCT_SET_COMMIT_APP1,   			FH_NW_PRODUCT_DUAL_IMAGE_READ_BASE_ADDR, FH_NW_PRODUCT_DUAL_IMAGE_WRITE_BASE_ADDR, APP1_COMMIT_OFFSET,    			FH_NW_PRODUCT_DUAL_IMAGE_FLAG_SIZE, NULL,            		set_dual_image_entry},
    {"set", "commit", 	"app2",       E_FH_NW_PRODUCT_SET_COMMIT_APP2,   			FH_NW_PRODUCT_DUAL_IMAGE_READ_BASE_ADDR, FH_NW_PRODUCT_DUAL_IMAGE_WRITE_BASE_ADDR, APP2_COMMIT_OFFSET,    			FH_NW_PRODUCT_DUAL_IMAGE_FLAG_SIZE, NULL,            		set_dual_image_entry},
    {"get", "commit", 	"kernel", 	  E_FH_NW_PRODUCT_GET_COMMIT_KERNEL, 			FH_NW_PRODUCT_DUAL_IMAGE_READ_BASE_ADDR, FH_NW_PRODUCT_DUAL_IMAGE_WRITE_BASE_ADDR, KERNEL_COMMIT_OFFSET,  			FH_NW_PRODUCT_DUAL_IMAGE_FLAG_SIZE, get_dual_image_entry,  					NULL},
    {"get", "commit", 	"rootfs", 	  E_FH_NW_PRODUCT_GET_COMMIT_ROOTFS, 			FH_NW_PRODUCT_DUAL_IMAGE_READ_BASE_ADDR, FH_NW_PRODUCT_DUAL_IMAGE_WRITE_BASE_ADDR, ROOTFS_COMMIT_OFFSET,  			FH_NW_PRODUCT_DUAL_IMAGE_FLAG_SIZE, get_dual_image_entry,  					NULL},
    {"get", "commit", 	"app1",   	  E_FH_NW_PRODUCT_GET_COMMIT_APP1,   			FH_NW_PRODUCT_DUAL_IMAGE_READ_BASE_ADDR, FH_NW_PRODUCT_DUAL_IMAGE_WRITE_BASE_ADDR, APP1_COMMIT_OFFSET,    			FH_NW_PRODUCT_DUAL_IMAGE_FLAG_SIZE, get_dual_image_entry,  					NULL},
    {"get", "commit", 	"app2",       E_FH_NW_PRODUCT_GET_COMMIT_APP2,  	 		FH_NW_PRODUCT_DUAL_IMAGE_READ_BASE_ADDR, FH_NW_PRODUCT_DUAL_IMAGE_WRITE_BASE_ADDR, APP2_COMMIT_OFFSET,    			FH_NW_PRODUCT_DUAL_IMAGE_FLAG_SIZE, get_dual_image_entry,  					NULL},
    {"set", "active", 	"kernel", 	  E_FH_NW_PRODUCT_SET_ACTIVE_KERNEL, 			FH_NW_PRODUCT_DUAL_IMAGE_READ_BASE_ADDR, FH_NW_PRODUCT_DUAL_IMAGE_WRITE_BASE_ADDR, KERNEL_ACTIVE_OFFSET,  			FH_NW_PRODUCT_DUAL_IMAGE_FLAG_SIZE, NULL,            		set_dual_image_entry},
    {"set", "active", 	"rootfs",     E_FH_NW_PRODUCT_SET_ACTIVE_ROOTFS, 			FH_NW_PRODUCT_DUAL_IMAGE_READ_BASE_ADDR, FH_NW_PRODUCT_DUAL_IMAGE_WRITE_BASE_ADDR, ROOTFS_ACTIVE_OFFSET,  			FH_NW_PRODUCT_DUAL_IMAGE_FLAG_SIZE, NULL,            		set_dual_image_entry},
    {"set", "active", 	"app1",       E_FH_NW_PRODUCT_SET_ACTIVE_APP1,   			FH_NW_PRODUCT_DUAL_IMAGE_READ_BASE_ADDR, FH_NW_PRODUCT_DUAL_IMAGE_WRITE_BASE_ADDR, APP1_ACTIVE_OFFSET,    			FH_NW_PRODUCT_DUAL_IMAGE_FLAG_SIZE, NULL,            		set_dual_image_entry},
    {"set", "active", 	"app2",       E_FH_NW_PRODUCT_SET_ACTIVE_APP2,   			FH_NW_PRODUCT_DUAL_IMAGE_READ_BASE_ADDR, FH_NW_PRODUCT_DUAL_IMAGE_WRITE_BASE_ADDR, APP2_ACTIVE_OFFSET,    			FH_NW_PRODUCT_DUAL_IMAGE_FLAG_SIZE, NULL,            		set_dual_image_entry},
    {"get", "active", 	"kernel",     E_FH_NW_PRODUCT_GET_ACTIVE_KERNEL, 			FH_NW_PRODUCT_DUAL_IMAGE_READ_BASE_ADDR, FH_NW_PRODUCT_DUAL_IMAGE_WRITE_BASE_ADDR, KERNEL_ACTIVE_OFFSET,  			FH_NW_PRODUCT_DUAL_IMAGE_FLAG_SIZE, get_dual_image_entry,					NULL},
    {"get", "active", 	"rootfs",     E_FH_NW_PRODUCT_GET_ACTIVE_ROOTFS, 			FH_NW_PRODUCT_DUAL_IMAGE_READ_BASE_ADDR, FH_NW_PRODUCT_DUAL_IMAGE_WRITE_BASE_ADDR, ROOTFS_ACTIVE_OFFSET,  			FH_NW_PRODUCT_DUAL_IMAGE_FLAG_SIZE, get_dual_image_entry,					NULL},
    {"get", "active", 	"app1",       E_FH_NW_PRODUCT_GET_ACTIVE_APP1,   			FH_NW_PRODUCT_DUAL_IMAGE_READ_BASE_ADDR, FH_NW_PRODUCT_DUAL_IMAGE_WRITE_BASE_ADDR, APP1_ACTIVE_OFFSET,    			FH_NW_PRODUCT_DUAL_IMAGE_FLAG_SIZE, get_dual_image_entry,					NULL},
    {"get", "active", 	"app2",   	  E_FH_NW_PRODUCT_GET_ACTIVE_APP2,   			FH_NW_PRODUCT_DUAL_IMAGE_READ_BASE_ADDR, FH_NW_PRODUCT_DUAL_IMAGE_WRITE_BASE_ADDR, APP2_ACTIVE_OFFSET,    			FH_NW_PRODUCT_DUAL_IMAGE_FLAG_SIZE, get_dual_image_entry,					NULL},

    {"set", "snoui",  		NULL,     E_FH_NW_PRODUCT_SET_SNOUI, 		   			FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_SNOUI_OFFSET,   		FH_NW_PRODUCT_ENV_SIZE, 			NULL,            set_flash_env_entry},
    {"set", "hwcfg",   		NULL,     E_FH_NW_PRODUCT_SET_HWCFG, 		   			FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_HWCFG_OFFSET,   		FH_NW_PRODUCT_ENV_SIZE, 			NULL,            set_flash_env_entry},
    {"set", "ethaddr", 		NULL,     E_FH_NW_PRODUCT_SET_ETHERADDR, 	   			FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_ETHADDR_OFFSET, 		FH_NW_PRODUCT_ENV_SIZE, 			NULL,            set_flash_env_entry},
	{"set", "reserve0",		NULL,     E_FH_NW_PRODUCT_SET_FLASH_ENV_RESERVE0,		FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE0_OFFSET, 		FH_NW_PRODUCT_ENV_SIZE, 			NULL,            set_flash_env_entry},
	{"set", "reserve1",		NULL,     E_FH_NW_PRODUCT_SET_FLASH_ENV_RESERVE1,		FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE1_OFFSET, 		FH_NW_PRODUCT_ENV_SIZE, 			NULL,            set_flash_env_entry},
	{"set", "reserve2",		NULL,     E_FH_NW_PRODUCT_SET_FLASH_ENV_RESERVE2,		FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE2_OFFSET, 		FH_NW_PRODUCT_ENV_SIZE, 			NULL,            set_flash_env_entry},
	{"set", "reserve3",		NULL,     E_FH_NW_PRODUCT_SET_FLASH_ENV_RESERVE3,		FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE3_OFFSET, 		FH_NW_PRODUCT_ENV_SIZE, 			NULL,            set_flash_env_entry},
	{"set", "reserve4",		NULL,     E_FH_NW_PRODUCT_SET_FLASH_ENV_RESERVE4,		FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE4_OFFSET, 		FH_NW_PRODUCT_ENV_SIZE, 			NULL,            set_flash_env_entry},
	{"set", "reserve5",		NULL,     E_FH_NW_PRODUCT_SET_FLASH_ENV_RESERVE5,		FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE5_OFFSET, 		FH_NW_PRODUCT_ENV_SIZE, 			NULL,            set_flash_env_entry},
	{"set", "reserve6",		NULL,     E_FH_NW_PRODUCT_SET_FLASH_ENV_RESERVE6,		FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE6_OFFSET, 		FH_NW_PRODUCT_ENV_SIZE, 			NULL,            set_flash_env_entry},
	{"set", "reserve7",		NULL,     E_FH_NW_PRODUCT_SET_FLASH_ENV_RESERVE7,		FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE7_OFFSET, 		FH_NW_PRODUCT_ENV_SIZE, 			NULL,            set_flash_env_entry},
	{"set", "reserve8", 	NULL,     E_FH_NW_PRODUCT_SET_FLASH_ENV_RESERVE8,       FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE8_OFFSET,        FH_NW_PRODUCT_ENV_SIZE,     		NULL,            set_flash_env_entry},
	{"set", "reserve9", 	NULL,     E_FH_NW_PRODUCT_SET_FLASH_ENV_RESERVE9,       FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE9_OFFSET,        FH_NW_PRODUCT_ENV_SIZE,     		NULL,            set_flash_env_entry},
	{"set", "reserve10",	NULL,     E_FH_NW_PRODUCT_SET_FLASH_ENV_RESERVE10,		FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE10_OFFSET, 		FH_NW_PRODUCT_ENV_SIZE, 			NULL,            set_flash_env_entry},
	{"set", "reserve11",    NULL,     E_FH_NW_PRODUCT_SET_FLASH_ENV_RESERVE11,      FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE11_OFFSET,       FH_NW_PRODUCT_ENV_SIZE,     		NULL,            set_flash_env_entry},
	{"set", "reserve12",    NULL,     E_FH_NW_PRODUCT_SET_FLASH_ENV_RESERVE12,      FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE12_OFFSET,       FH_NW_PRODUCT_ENV_SIZE,     		NULL,            set_flash_env_entry},
	{"set", "reserve13",    NULL,     E_FH_NW_PRODUCT_SET_FLASH_ENV_RESERVE13,      FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE13_OFFSET,       FH_NW_PRODUCT_ENV_SIZE,     		NULL,            set_flash_env_entry},
	{"set", "reserve14",    NULL,     E_FH_NW_PRODUCT_SET_FLASH_ENV_RESERVE14,      FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE14_OFFSET,       FH_NW_PRODUCT_ENV_SIZE,     		NULL,            set_flash_env_entry},
	{"set", "reserve15",    NULL,     E_FH_NW_PRODUCT_SET_FLASH_ENV_RESERVE15,      FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE15_OFFSET,       FH_NW_PRODUCT_ENV_SIZE,     		NULL,            set_flash_env_entry},
	{"set", "reserve16",    NULL,     E_FH_NW_PRODUCT_SET_FLASH_ENV_RESERVE16,      FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE16_OFFSET,       FH_NW_PRODUCT_ENV_SIZE,     		NULL,            set_flash_env_entry},
	{"set", "reserve17",    NULL,     E_FH_NW_PRODUCT_SET_FLASH_ENV_RESERVE17,      FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE17_OFFSET,       FH_NW_PRODUCT_ENV_SIZE,     		NULL,            set_flash_env_entry},
	{"set", "reserve18",    NULL,     E_FH_NW_PRODUCT_SET_FLASH_ENV_RESERVE18,      FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE18_OFFSET,       FH_NW_PRODUCT_ENV_SIZE,     		NULL,            set_flash_env_entry},
	{"set", "reserve19",    NULL,     E_FH_NW_PRODUCT_SET_FLASH_ENV_RESERVE19,      FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE19_OFFSET,       FH_NW_PRODUCT_ENV_SIZE,     		NULL,            set_flash_env_entry},

    {"get", "snoui",        NULL,     E_FH_NW_PRODUCT_GET_SNOUI,            		FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_SNOUI_OFFSET,           FH_NW_PRODUCT_ENV_SIZE,     		get_flash_env_entry, 			NULL},
    {"get", "hwcfg",        NULL,     E_FH_NW_PRODUCT_GET_HWCFG,            		FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_HWCFG_OFFSET,           FH_NW_PRODUCT_ENV_SIZE,     		get_flash_env_entry, 			NULL},
    {"get", "ethaddr",      NULL,     E_FH_NW_PRODUCT_GET_ETHERADDR,        		FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_ETHADDR_OFFSET,         FH_NW_PRODUCT_ENV_SIZE,     		get_flash_env_entry, 			NULL},
    {"get", "reserve0",     NULL,     E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE0,     	FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE0_OFFSET,        FH_NW_PRODUCT_ENV_SIZE,    	 		get_flash_env_entry, 			NULL},
    {"get", "reserve1",     NULL,     E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE1,       FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE1_OFFSET,        FH_NW_PRODUCT_ENV_SIZE,     		get_flash_env_entry, 			NULL},
    {"get", "reserve2",     NULL,     E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE2,       FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE2_OFFSET,        FH_NW_PRODUCT_ENV_SIZE,     		get_flash_env_entry,            NULL},
    {"get", "reserve3",     NULL,     E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE3,       FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE3_OFFSET,        FH_NW_PRODUCT_ENV_SIZE,     		get_flash_env_entry,            NULL},
    {"get", "reserve4",     NULL,     E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE4,       FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE4_OFFSET,        FH_NW_PRODUCT_ENV_SIZE,     		get_flash_env_entry,            NULL},
    {"get", "reserve5",     NULL,     E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE5,       FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE5_OFFSET,        FH_NW_PRODUCT_ENV_SIZE,     		get_flash_env_entry,            NULL},
    {"get", "reserve6",     NULL,     E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE6,       FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE6_OFFSET,        FH_NW_PRODUCT_ENV_SIZE,     		get_flash_env_entry,            NULL},
    {"get", "reserve7",     NULL,     E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE7,       FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE7_OFFSET,        FH_NW_PRODUCT_ENV_SIZE,     		get_flash_env_entry,            NULL},
    {"get", "reserve8",     NULL,     E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE8,       FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE8_OFFSET,        FH_NW_PRODUCT_ENV_SIZE,     		get_flash_env_entry,            NULL},
    {"get", "reserve9",     NULL,     E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE9,       FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE9_OFFSET,        FH_NW_PRODUCT_ENV_SIZE,     		get_flash_env_entry,            NULL},
    {"get", "reserve10",    NULL,     E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE10,      FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE10_OFFSET,       FH_NW_PRODUCT_ENV_SIZE,     		get_flash_env_entry,            NULL},
    {"get", "reserve11",    NULL,     E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE11,      FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE11_OFFSET,       FH_NW_PRODUCT_ENV_SIZE,     		get_flash_env_entry,            NULL},
    {"get", "reserve12",    NULL,     E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE12,      FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE12_OFFSET,       FH_NW_PRODUCT_ENV_SIZE,     		get_flash_env_entry,            NULL},
    {"get", "reserve13",    NULL,     E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE13,      FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE13_OFFSET,       FH_NW_PRODUCT_ENV_SIZE,     		get_flash_env_entry,            NULL},
    {"get", "reserve14",    NULL,     E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE14,      FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE14_OFFSET,       FH_NW_PRODUCT_ENV_SIZE,     		get_flash_env_entry,            NULL},
    {"get", "reserve15",    NULL,     E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE15,      FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE15_OFFSET,       FH_NW_PRODUCT_ENV_SIZE,     		get_flash_env_entry,            NULL},
    {"get", "reserve16",    NULL,     E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE16,      FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE16_OFFSET,       FH_NW_PRODUCT_ENV_SIZE,     		get_flash_env_entry,            NULL},
    {"get", "reserve17",    NULL,     E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE17,      FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE17_OFFSET,       FH_NW_PRODUCT_ENV_SIZE,     		get_flash_env_entry,            NULL},
    {"get", "reserve18",    NULL,     E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE18,      FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE18_OFFSET,       FH_NW_PRODUCT_ENV_SIZE,     		get_flash_env_entry,            NULL},
    {"get", "reserve19",    NULL,     E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE19,      FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR,  FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR,  FH_FLASH_RESERVE19_OFFSET,       FH_NW_PRODUCT_ENV_SIZE,     		get_flash_env_entry,            NULL},
};
#define MAX_PERSONALITY_PARM_TABLE_b_SIZE ((sizeof(g_personality_parm_tab_b)) / (sizeof(personality_parm_table_t)))

static int do_stop_restart(int argc, char *argv[])
{
		restart_time_flags = 0;
		printf("stop bootloader success\n");
		return 0;
}
static int do_personality_parm_cmd(int argc, char *argv[])
{
    int                 iret = E_PERSONALITY_PARM_RIGHT_EXECUTE;
    unsigned int           i = 0;
    unsigned int	 cmd_len = 0;
    unsigned char       flag = 0;
    unsigned long  cmd_value = 0;
    personality_parm_t info;
    
    printf("\n enter do_personality_parm_cmd.\n");
    for(i=0; ((i<MAX_PERSONALITY_PARM_TABLE_b_SIZE) && (NULL != g_personality_parm_tab_b[i].action_cmd)); i++)
    {
        if((0 == strcmp(g_personality_parm_tab_b[i].action_cmd, argv[1])) && (0 == strcmp(g_personality_parm_tab_b[i].node_name, argv[2])))
        {
            if(0 == strcmp(g_personality_parm_tab_b[i].action_cmd, "set") && (NULL != g_personality_parm_tab_b[i].set_func))
            {
                /*dual image flag*/
                if(((0 == strcmp(g_personality_parm_tab_b[i].node_name, "commit")) || (0 == strcmp(g_personality_parm_tab_b[i].node_name, "active"))) \
                    && (0 == strcmp(g_personality_parm_tab_b[i].sub_node_name, argv[3])))
                {
					if(argc < 5)
					{
						printf("please check parm.\n");
						return 0;
					}
                    cmd_value = strtoul((const char*)(argv[4]), (char **)NULL, 16);
                    iret = dual_image_offset_convert_value(g_personality_parm_tab_b[i].offset, cmd_value, &flag);
                    if(E_PERSONALITY_PARM_RIGHT_EXECUTE != iret)
                    {
                        printf("invaild argument error.line = %d.\n", __LINE__);
                        return E_PERSONALITY_PARM_INVALID_ARGUMENT;
                    }
                    memset(&info, 0, sizeof(info));
                    info.read_base_addr  = g_personality_parm_tab_b[i].read_base_addr;
                    info.write_base_addr = g_personality_parm_tab_b[i].write_base_addr;
                    info.v_offset        = g_personality_parm_tab_b[i].offset;
                    info.v_size          = g_personality_parm_tab_b[i].size;
                    info.element.dual_image_flag = (flag & FH_NW_PRODUCT_DUAL_IMAGE_MASK);
                    iret = g_personality_parm_tab_b[i].set_func(&info);
                    if(E_PERSONALITY_PARM_RIGHT_EXECUTE != iret)
                    {
                        printf("set %s %s execute fail.\n", g_personality_parm_tab_b[i].node_name, g_personality_parm_tab_b[i].sub_node_name);
                    }
                    else
                    {
                        printf("set %s %s execute success.\n", g_personality_parm_tab_b[i].node_name, g_personality_parm_tab_b[i].sub_node_name);
                    }
                    return iret;
                }
				else if((0 == strcmp(g_personality_parm_tab_b[i].node_name, argv[2])) && ((0 != strcmp(g_personality_parm_tab_b[i].node_name, "commit")) && (0 != strcmp(g_personality_parm_tab_b[i].node_name, "active"))))
				{
					memset(&info, 0, sizeof(info));
					cmd_len = 0;
					info.read_base_addr  = g_personality_parm_tab_b[i].read_base_addr;
					info.write_base_addr = g_personality_parm_tab_b[i].write_base_addr;
					info.v_offset 		 = g_personality_parm_tab_b[i].offset;
					info.v_size			 = g_personality_parm_tab_b[i].size;
					if(argc < 4)
					{
						printf("please check parm.\n");
						return 0;
					}
					cmd_len = strlen(argv[3]);
					if(cmd_len >= (FH_NW_PRODUCT_ENV_SIZE - 1))
					{
						memcpy(info.element.env, argv[3], (FH_NW_PRODUCT_ENV_SIZE-1));
						info.element.env[FH_NW_PRODUCT_ENV_SIZE-1] = '\0';
					}
					else
					{
						strcpy(info.element.env, argv[3]);
					}
					
					iret = g_personality_parm_tab_b[i].set_func(&info);
					if(E_PERSONALITY_PARM_RIGHT_EXECUTE != iret)
					{
						printf("set %s %s execute fail.\n", g_personality_parm_tab_b[i].node_name, info.element.env);
					}
					else
					{
						printf("set %s %s execute success.\n", g_personality_parm_tab_b[i].node_name, info.element.env);
					}
					return iret;
				}
                else
                {
                    continue;
                }
            }
            else if(0 == strcmp(g_personality_parm_tab_b[i].action_cmd, "get") && (NULL != g_personality_parm_tab_b[i].get_func))
            {
                /*dual image get*/
                if(((0 == strcmp(g_personality_parm_tab_b[i].node_name, "commit")) || (0 == strcmp(g_personality_parm_tab_b[i].node_name, "active"))) \
                    && (0 == strcmp(g_personality_parm_tab_b[i].sub_node_name, argv[3])))
                {
                    memset(&info, 0, sizeof(info));
                    info.read_base_addr  = g_personality_parm_tab_b[i].read_base_addr;
                    info.write_base_addr = g_personality_parm_tab_b[i].write_base_addr;
                    info.v_offset 		 = g_personality_parm_tab_b[i].offset;
                    info.v_size   		 = g_personality_parm_tab_b[i].size;
                    iret = g_personality_parm_tab_b[i].get_func(&info);
                    if(E_PERSONALITY_PARM_RIGHT_EXECUTE != iret)
                    {
                        printf("get %s %s execute fail.\n", g_personality_parm_tab_b[i].node_name, g_personality_parm_tab_b[i].sub_node_name);
                    }
                    else
                    {
                        printf("get %s %s = %d execute success.\n", g_personality_parm_tab_b[i].node_name, g_personality_parm_tab_b[i].sub_node_name, info.element.dual_image_flag);
                    }
                    return iret;
                }
				else if((0 == strcmp(g_personality_parm_tab_b[i].node_name, argv[2])) && ((0 != strcmp(g_personality_parm_tab_b[i].node_name, "commit")) && (0 != strcmp(g_personality_parm_tab_b[i].node_name, "active"))))
				{
					memset(&info, 0, sizeof(info));
					info.read_base_addr  = g_personality_parm_tab_b[i].read_base_addr;
					info.write_base_addr = g_personality_parm_tab_b[i].write_base_addr;
					info.v_offset        = g_personality_parm_tab_b[i].offset;
					info.v_size          = g_personality_parm_tab_b[i].size;
					iret = g_personality_parm_tab_b[i].get_func(&info);
					if(E_PERSONALITY_PARM_RIGHT_EXECUTE != iret)
					{
						printf("get %s execute fail.\n", g_personality_parm_tab_b[i].node_name);
					}
					else
					{
						printf("get %s = %s execute success.\n", g_personality_parm_tab_b[i].node_name, info.element.env);
					}
					return iret;
				}
                else
                {
                    continue;
                }
            }
        }
    }
    
    return E_PERSONALITY_PARM_RIGHT_EXECUTE;	
}

static int do_show_all_dual_image_flag_info(void)
{
	int            iret = E_PERSONALITY_PARM_RIGHT_EXECUTE;
	unsigned char  flag = 0;
	unsigned long ret_len = 0;
	personality_parm_cmd_base_addr_t info;
	dual_image_personality_parm_partition_info_t dual_info;
	unsigned long dual_info_size = sizeof(dual_image_personality_parm_partition_info_t);

	memset(&dual_info, 0, sizeof(dual_info));
	memset(&info, 0, sizeof(info));
	info.read_base_addr  = FH_NW_PRODUCT_DUAL_IMAGE_READ_BASE_ADDR;
	info.write_base_addr = FH_NW_PRODUCT_DUAL_IMAGE_WRITE_BASE_ADDR;
	iret = personality_parm_get_partition_valid(&info);
	if(0 != iret)
	{
		return E_PERSONALITY_PARM_INVALID_ARGUMENT;
	}
	/*default value*/
	if(INVLID == info.both_valid_flag)
	{
        memset(&dual_info, 0, sizeof(dual_info));
	}
	else
	{
		flash_read(info.valid_read_addr, dual_info_size, &ret_len,  (unsigned char *)&dual_info);
	}
	
	printf_dual_image_info(&dual_info, E_BOOT_DEBUG_INFO_LEVLE, __LINE__);

	return E_PERSONALITY_PARM_RIGHT_EXECUTE;
}

static int do_show_all_flash_env_info(void)
{
	int            iret = E_PERSONALITY_PARM_RIGHT_EXECUTE;
	unsigned long ret_len = 0;
	personality_parm_cmd_base_addr_t info;
	flash_env_personality_parm_partition_info_t env_info;
    unsigned long env_info_size = sizeof(env_info);

	memset(&info, 0, sizeof(info));
	memset(&env_info, 0, sizeof(env_info));
	
	info.read_base_addr  = FH_NW_PRODUCT_FLASH_ENV_READ_BASE_ADDR;
	info.write_base_addr = FH_NW_PRODUCT_FLASH_ENV_WRITE_BASE_ADDR;
	iret = personality_parm_get_partition_valid(&info);
	if(0 != iret)
	{
		return E_PERSONALITY_PARM_INVALID_ARGUMENT;
	}
	/*default value*/
	if(INVLID == info.both_valid_flag)
	{
		printf("default value.\n");
		printf("snoui = 00.\n");
		printf("hwcfg = 0x77.\n");
		printf("ethaddr = 00:0A:C2:12:34:56.\n");
	}
	else
	{
    	flash_read(info.valid_read_addr, env_info_size, &ret_len,  (unsigned char *)&env_info);
		printf_flash_env_info(&env_info, E_BOOT_DEBUG_INFO_LEVLE, __LINE__);
	}

	return E_PERSONALITY_PARM_RIGHT_EXECUTE;

}

#endif
