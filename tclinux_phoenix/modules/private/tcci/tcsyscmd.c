/************************************************************************
 *
 *	Copyright (C) 2007 Trendchip Technologies, Corp.
 *	All Rights Reserved.
 *
 * Trendchip Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of Trendchip Technologies, Co. and shall
 * not be reproduced, copied, disclosed, or used in whole or in part
 * for any reason without the prior express written permission of
 * Trendchip Technologeis, Co.
 *
 *************************************************************************/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/jiffies.h>
#include <linux/timer.h>
#include <linux/wait.h>
#include <linux/proc_fs.h>
#include <asm/addrspace.h>
#include <asm/io.h>
#include <asm/tc3162/cmdparse.h>
#include <asm/tc3162/tc3162.h>
#include <asm/tc3162/ledcetrl.h>
#include <asm/mipsregs.h>
#include <linux/version.h>
extern int subcmd(const cmds_t tab[], int argc, char *argv[], void *p);
extern int cmd_register(cmds_t *cmds_p);

static int doSys(int argc, char *argv[], void *p);
static int doSysMemrl(int argc, char *argv[], void *p);
static int doSysMemwl(int argc, char *argv[], void *p);
static int doSysModifyBit(int argc, char *argv[], void *p);
static int doSysMemwl2(int argc, char *argv[], void *p);
static int doSysMemrw(int argc, char *argv[], void *p);
static int doSysMemww(int argc, char *argv[], void *p);
static int doSysMemory(int argc, char *argv[], void *p);
static int doSysMemcpy(int argc, char *argv[], void *p);
static int doSysFillMem(int argc, char *argv[], void *p);
static int doSysMac(int argc, char *argv[], void *p);
static int doSysOnuType(int argc, char *argv[], void *p);
static int doSysExMdio(int argc, char *argv[], void *p);

static int doExMdioConf(int argc, char *argv[], void *p);
static int doExMdioWrite(int argc, char *argv[], void *p);
static int doExMdioRead(int argc, char *argv[], void *p);


#if defined(TCSUPPORT_WAN2LAN_EXT)
static int doLan2lan(int argc, char *argv[], void *p);
#define GSW_BASE        0xBFB58000
#define GSW_PORT_BASE     0x2000
#define GSW_MAC_FC    0x10
#define GSW_PCL(n) GSW_PORT_BASE + (n)*0x100 + 0x04
#define read_reg_word(reg) 		regRead32(reg)
#define write_reg_word(reg, wdata) 	regWrite32(reg, wdata)
extern uint32 (*gswPbusRead_hook)(uint32 pbus_addr);
extern int (*gswPbusWrite_hook)(uint32 pbus_addr, uint32 pbus_data);
extern int (*lanPortmap_hook)(int lanPort);
extern int (*lanPortmap_rev_hook)(int switchPort);
#endif
#ifdef WAN2LAN
static int doWan2lan(int argc, char *argv[], void *p);
int my_atoi(const char *str);
int masko_on_off = 0;
#if defined(TCSUPPORT_WAN2LAN_EXT)
int masko_ext = 0;
#endif

#if defined(TCSUPPORT_WAN2LAN_EXT)
int masko = 0;
#else
int masko = 0xF;
#endif
EXPORT_SYMBOL(masko_on_off);
#if defined(TCSUPPORT_WAN2LAN_EXT)
EXPORT_SYMBOL(masko_ext);
#endif
EXPORT_SYMBOL(masko);

#if defined(TCSUPPORT_CMDPROMPT)
int print_usage=0;
int print_state=0;
int print_portmask=0;
int print_errormsg=0;
#define WAN2LAN_INFO_USAGE "Usage: wan2lan [on||off] <number> \r\n"
#define WAN2LAN_INFO_ON "Current wan2lan feature status: on \r\n"
#define WAN2LAN_INFO_OFF "Current wan2lan feature status: off \r\n"
#define WAN2LAN_INFO_PORTMASK "Current portmask number is[%d] \r\n"
#define WAN2LAN_INFO_ERRORMSG "input portmask number is out of range(available value is 0~15) \r\n"
#endif

#endif

/*#define TCSUPPORT_GDMA_TEST 1*/
/*#define TCSUPPORT_TIMER_TEST 1*/

#if defined(TCSUPPORT_GDMA_TEST)
#define TC_GDMA_BASE     	0xBFB30000
#define	TC_GDMA_SA0    	    0xBFB30000
#define	TC_GDMA_DA0    	    0xBFB30004
#define	TC_GDMA_CT00    	0xBFB30008
#define	TC_GDMA_CT10    	0xBFB3000c
#define TC_GDMA_DONEINT  	(TC_GDMA_BASE + 0x204)
#define GDMA_CH_NUM         8
#define GDMA_MAX_BURST      4
#ifndef VPint
#define VPint			*(volatile unsigned long int *)
#endif
int doGdmaTest(int argc, char *argv[], void *p);
#endif


#if defined(TCSUPPORT_TIMER_TEST)
#define TIMER0_IRQ_SRC              (4)
#define TIMER1_IRQ_SRC              (5)
#define TIMER2_IRQ_SRC              (6)
#define WDOG_IRQ_SRC                (9)
#define MIPS_IRQ_SRC_OFFSET         (1)

#define CONFIG_TIMER0_IRQ			(TIMER0_IRQ_SRC + MIPS_IRQ_SRC_OFFSET)
#define CONFIG_TIMER1_IRQ			(TIMER1_IRQ_SRC + MIPS_IRQ_SRC_OFFSET)
#define CONFIG_TIMER2_IRQ			(TIMER2_IRQ_SRC + MIPS_IRQ_SRC_OFFSET)
#define CONFIG_WDOG_IRQ			    (WDOG_IRQ_SRC + MIPS_IRQ_SRC_OFFSET)

int doTimerTest(int argc, char *argv[], void *p);
void timer_Configure(uint8  timer_no, uint8 timer_enable, uint8 timer_mode, uint8 timer_halt);

#define timerLdvSet(timer_no,val) *(volatile uint32 *)(CR_TIMER0_LDV+timer_no*0x08) = (val)
#define timerCtlSet(timer_no, timer_enable, timer_mode,timer_halt)	timer_Configure(timer_no, timer_enable, timer_mode, timer_halt)

uint8 timer_sel = 0;
uint32 globe_cnt = 0;
#endif

#if defined(TCSUPPORT_AUTOBENCH_AFE)||defined(TCSUPPORT_AUTOBENCH_MT7510)
extern void tc3162wdog_kick(void);
static int waitDmtPowerOn(int argc, char *argv[], void *p);
static int checkReboot(int argc, char *argv[], void *p);
#endif
#if defined(TCSUPPORT_LED_BTN_CHECK) || defined(TCSUPPORT_TEST_LED_ALL) 
#if defined(TCSUPPORT_BTN_CHECK)
extern int gButtonCheck; 
#endif
#if defined(TCSUPPORT_LED_CHECK) || defined(TCSUPPORT_TEST_LED_ALL) 
extern int doLedOn(void);
extern int doLedOff(void);
extern int doLedRecover(void);
static int doLedCheck(int argc, char *argv[], void *p);
#endif
#if defined(TCSUPPORT_BTN_CHECK)
static int doButtonCheck(int argc, char *argv[], void *p);
#endif
#endif

#if defined(TCSUPPORT_USBHOST)
int doUSBEYE(int argc, char *argv[], void *p);
#endif
int doSysWatch(int argc, char *argv[], void *p);
#if defined(TCSUPPORT_NEW_SPIFLASH_DEBUG)
int do_sf_basic_test(int argc, char *argv[]);
int do_sf_task2_test(int argc, char *argv[]);
int do_sf_handler_test(int argc, char *argv[]);
#endif
#if defined(TCSUPPORT_NEW_SPIFLASH)
int do_sf_EPCheck(int argc, char *argv[]);
#endif

#if defined(TCSUPPORT_CPU_MT7505) || defined(TCSUPPORT_CPU_EN7512) || defined(TCSUPPORT_CPU_EN7521)
static int gdmpdebug = 0;
#define GDMPDEBUGP(f, a...)	if (gdmpdebug) printk(f, ## a )

int doSysGdmpDbg(int argc, char *argv[], void *p);
int doGDmpCfg(int argc, char *argv[], void *p);
int doSysGdmpMem(int argc, char *argv[], void *p);
#endif
#ifdef TCSUPPORT_PON_TEST
int doLaserTest(int argc, char *argv[], void *p);
int doPonTempTest(int argc, char *argv[], void *p);
int doSysAtbp(int argc, char *argv[], void *p);
int doSysPonLinkStatus(int argc, char *argv[], void *p);
#endif

extern void printVersionStr(void);
int doImageInfo(int argc,char *argv[],void *p);

static int doVoipTest(int argc, char *argv[], void *p);
static int doBindShowTask(int argc, char *argv[], void *p);

#ifdef TCSUPPORT_CDS
int doDead(int argc,char *argv[],void *p);
#endif

int doSysBobCarlibration(int argc, char *argv[], void *p);

#if defined(TCSUPPORT_CT_UPG_PINGPONG)
int doSysBootFlag(int argc, char *argv[], void *p);
#endif

#ifdef L2_AUTOPVC
static cmds_t sysCmds[] = {
#else
static const cmds_t sysCmds[] = {
#endif
	{"memrl",		doSysMemrl,		0x02,  	1,  NULL},
	{"memwl",		doSysMemwl,		0x02,  	2,  NULL},
	{"modifybit",		doSysModifyBit,		0x02,  	3,  NULL},
	{"memwl2",		doSysMemwl2,		0x02,  	3,  "sys memwl2 <0|1 (and|or)> <reg> <hex val>"},
	{"memrw",		doSysMemrw,		0x02,  	1,  NULL},
	{"memww",		doSysMemww,		0x02,  	2,  NULL},
	{"memory",		doSysMemory,	0x02,  	2,  NULL},
	{"memcpy",		doSysMemcpy,	0x02,  	3,  NULL},
	{"fillmem",		doSysFillMem,	0x02,  	3,  NULL},
	{"mac",	    	doSysMac,	    0x02,  	1,  NULL},
	{"onutype",	    doSysOnuType,	0x02,  	1,  NULL},
	{"exmdio",	    doSysExMdio,	0x02,  	0,  "sys exmdio <config|write|read>"},
#ifdef L2_AUTOPVC
	{"autopvc",		NULL,			0x12,	1,	NULL},
#endif
#ifdef WAN2LAN
	{"wan2lan",		doWan2lan,		0x02,	0,	NULL},
#if defined(TCSUPPORT_WAN2LAN_EXT)
	{"lan2lan",		doLan2lan,		0x02,	0,	NULL},
#endif
#endif
#if defined(TCSUPPORT_AUTOBENCH_AFE) || defined(TCSUPPORT_AUTOBENCH_MT7510)
    {"waitdmt",     waitDmtPowerOn,      0x02,   0,  NULL},
    {"checkreboot",     checkReboot,      0x02,   0,  NULL},
#endif
#if defined(TCSUPPORT_LED_BTN_CHECK) || defined(TCSUPPORT_TEST_LED_ALL) 
#if defined(TCSUPPORT_LED_CHECK) || defined(TCSUPPORT_TEST_LED_ALL) 
	{"led",		doLedCheck,		0x02,	0,	NULL},
#endif
#if defined(TCSUPPORT_BTN_CHECK)
	{"button",	doButtonCheck,	0x02,	0,	NULL},	
#endif
#endif

#if defined(TCSUPPORT_USBHOST)
	{"usbeye",  doUSBEYE,  0x02,   0,  NULL},
#endif	
	{"watch",		doSysWatch,			0x02,	0,	NULL},
#if defined(TCSUPPORT_NEW_SPIFLASH_DEBUG)
	{"sftest",	 do_sf_basic_test,		0x02,	0,	NULL},
	{"sftest2",	 do_sf_task2_test,		0x02,	0,	NULL},
	{"sfhandlertest",	 do_sf_handler_test,		0x02,	0,	NULL},
#endif
#if defined(TCSUPPORT_NEW_SPIFLASH)
	{"sfepcheck",	 do_sf_EPCheck,		0x02,	0,	NULL},
#endif
#if defined(TCSUPPORT_CPU_MT7505) || defined(TCSUPPORT_CPU_EN7512) || defined(TCSUPPORT_CPU_EN7521)
	{"gdmpDebug",		doSysGdmpDbg,	0x02,  	1,  NULL},
	{"gdmpReg",		doGDmpCfg,			0x02,	0,	NULL},
	{"gdmpMemory",		doSysGdmpMem,	0x02,  	2,  NULL},	
#endif	

#ifdef TCSUPPORT_PON_TEST
	{"lasertest",        doLaserTest, 0x02, 1, "sys lasertest <on|off>"},
	{"pontest",        doPonTempTest, 0x02, 0, NULL},
	{"ponlink",        doSysPonLinkStatus, 0x02, 0, NULL},
	{"atbp",        doSysAtbp, 0x02, 0, NULL},
#endif
	{"ver",		printVersionStr,			0x02,	0,	NULL},
	{"imginfo",		doImageInfo,			0x02,	0,	NULL},
	{"voiptest",	doVoipTest,			0x02,	0,	NULL},
	{"bindshowtask",	doBindShowTask,			0x02,	0,	NULL},
#ifdef TCSUPPORT_CDS
	{"dead",		doDead,			0x02,	0,	NULL},
#endif
	{"bobcarlibration", 	doSysBobCarlibration,			0x01,	0,	NULL},
#if defined(TCSUPPORT_CT_UPG_PINGPONG)
	{"bootflag", 	doSysBootFlag,			0x01,	0,	NULL},
#endif
#if defined(TCSUPPORT_GDMA_TEST)
    {"gdmatest",      doGdmaTest,   0x02,   1,  "sys gdmatest <cnt>"}, 
#endif
#if defined(TCSUPPORT_TIMER_TEST)    
    {"timer",      doTimerTest,   0x02,   1,  "sys timer <timer 0~3>"}, 
#endif         
	{NULL,			NULL,			0x10,	0,	NULL},
};

static const cmds_t subExMdioCmds[] = {
	{"config",		doExMdioConf,		0x02,  	2,  "sys exmdio config <mdc gpio> <mdio gpio>"},
	{"write",		doExMdioWrite,		0x02,  	2,  "sys exmdio write <reg> <value>"},
	{"read",		doExMdioRead,		0x02,  	1,  "sys exmdio read <reg>"},
	{NULL,			NULL,			0x10,	0,	NULL},
};


//#define BOOTLOADER_SIZE 64*1024
extern void setUserSpaceFlag(int num);

int doSys(int argc, char *argv[], void *p)
{
#if defined(TCSUPPORT_CMDPROMPT)
	if(argc==1){
		setUserSpaceFlag(12);
		return -1;
	}else{
		return subcmd(sysCmds, argc, argv, p);
	}
#else
	return subcmd(sysCmds, argc, argv, p);
#endif
	
}

#if defined(TCSUPPORT_NEW_SPIFLASH_DEBUG)
int do_sf_basic_test(int argc, char *argv[])
{
	sf_basic_test_entry(argc, argv);
	return 0;
}
int do_sf_task2_test(int argc, char *argv[])
{
	sf_task2_test_entry(argc, argv);
	return 0;
}
int do_sf_handler_test(int argc, char *argv[])
{
	sf_handler_test_entry(argc, argv);
	return 0;
}
#endif
#if defined(TCSUPPORT_NEW_SPIFLASH)
int do_sf_EPCheck(int argc, char *argv[])
{
	sf_EPCheck_entry(argc, argv);
	return 0;
}
#endif

/* check memory address is valid or not */
static inline int address_valid_check(unsigned long addr, int byteNum)
{
	int addrMask=0;
	addrMask = (byteNum==4)?(0x3):((byteNum==2)?(0x1):(0));
	if(((addr>>28)<8) || ((addr&addrMask)!=0)) {
		printk("address error\n");
		return -1;
	}
	return 0;
}

int doSysMemrl(int argc, char *argv[], void *p)
{
	unsigned long *ptr=NULL;

	ptr = (unsigned long *)simple_strtoul(argv[1], NULL, 16);
	if(address_valid_check((unsigned long)ptr, 4) != 0) {
		return -1;
	}
	printk("\r\n<Address>\t<Value>\r\n");
	printk("0x%08lx\t0x%08lx\r\n", (unsigned long)ptr, (unsigned long)*ptr);

	return 0;
}

int doSysMemwl(int argc, char *argv[], void *p)
{
	unsigned long *ptr=NULL;
	unsigned long value=0;

	ptr = (unsigned long *)simple_strtoul(argv[1], NULL, 16);
	value = (unsigned long)simple_strtoul(argv[2], NULL, 16);
	if(address_valid_check((unsigned long)ptr, 4) != 0) {
		return -1;
	}
	*ptr = value;

	return 0;
}
int doSysModifyBit(int argc, char *argv[], void *p)
{
	unsigned long *ptr=NULL;
         unsigned long   value=0;
         unsigned long   pos=0;

	ptr = (unsigned long *) simple_strtoul(argv[1], NULL, 16);
         pos = (unsigned long)  simple_strtoul(argv[2], NULL, 10);
         value = (unsigned long) simple_strtoul(argv[3], NULL, 10);
        
	if(address_valid_check((unsigned long)ptr, 4) != 0) {
		return -1;
	}
        if(value == 1)
                regWrite32(ptr, (regRead32(ptr) | (0x1 << pos)) );
        else
                regWrite32(ptr, (regRead32(ptr) & ~(0x1 << pos)) );   

        return 0;
}
int doSysMemwl2(int argc, char *argv[], void *p)
{
	unsigned long orFlag;
	unsigned long ptr;
	unsigned long value;
	orFlag = (unsigned long *)simple_strtoul(argv[1], NULL, 16);
	ptr = (unsigned long)simple_strtoul(argv[2], NULL, 16);
	value = (unsigned long)simple_strtoul(argv[3], NULL, 16);
	if(address_valid_check((unsigned long)ptr, 4) != 0) {
		return -1;
	}
	if(orFlag){
		regWrite32(ptr,regRead32(ptr)|value);
	}
	else{
		regWrite32(ptr,regRead32(ptr)&value);
	}
	//printk("addr:0x%x val: 0x%x (%s 0x%x)\n",ptr,regRead32(ptr),orFlag?"or":"and",value);
	return 0;
}

int doSysMemrw(int argc, char *argv[], void *p)
{
	unsigned short *ptr;

	ptr = (unsigned short *)simple_strtoul(argv[1], NULL, 16);
	if(address_valid_check((unsigned long)ptr, 2) != 0) {
		return -1;
	}
	printk("\r\n<Address>\t<Value>\r\n");
	printk("0x%08lx\t0x%04x\r\n", (unsigned long) ptr, (unsigned int) (*ptr));

	return 0;
}

int doSysMemww(int argc, char *argv[], void *p)
{
	unsigned short *ptr;
	unsigned short value;

	ptr = (unsigned short *)simple_strtoul(argv[1], NULL, 16);
	value = (unsigned short)simple_strtoul(argv[2], NULL, 16);
	if(address_valid_check((unsigned long)ptr, 2) != 0) {
		return -1;
	}
	*ptr = value;

	return 0;
}

static int dump(unsigned long addr, unsigned long len)
{
	register int n, m, c, r;
	unsigned char temp[16];

	for( n = len; n > 0; ){
			printk("%.8lx ", addr);
			r = n < 16? n: 16;
			memcpy((void *) temp, (void *) addr, r);
			addr += r;
			for( m = 0; m < r; ++m ){
					printk("%c", (m & 3) == 0 && m > 0? '.': ' ');
					printk("%.2x", temp[m]);
			}
			for(; m < 16; ++m )
					printk("   ");
			printk("  |");
			for( m = 0; m < r; ++m ){
				c = temp[m];
				printk("%c", ' ' <= c && c <= '~'? c: '.');
			}
			n -= r;
			for(; m < 16; ++m )
					printk(" ");
			printk("|\n");
	}

	return 0;
}

int doSysMemory(int argc, char *argv[], void *p)
{
	unsigned long addr, len;

	addr = simple_strtoul(argv[1], NULL, 16);
	len = simple_strtoul(argv[2], NULL, 16);
	dump(addr, len);

	return 0;
}

int doSysMemcpy(int argc, char *argv[], void *p)
{
	unsigned long src, dst, len;

	src = simple_strtoul(argv[1], NULL, 16);
	dst = simple_strtoul(argv[2], NULL, 16);
	len = simple_strtoul(argv[3], NULL, 16);
	memcpy((void *)dst, (void *)src, len);

	return 0;
}

int doSysFillMem(int argc, char *argv[], void *p)
{
	unsigned long addr, len, pattern;

	addr = simple_strtoul(argv[1], NULL, 16);
	len = simple_strtoul(argv[2], NULL, 16);
	pattern = simple_strtoul(argv[3], NULL, 16);
	memset((void *)addr, pattern, len);

	return 0;
}


int doSysMac(int argc, char *argv[], void *p)
{
    setUserSpaceFlag(1);
	return 0;
}
int doSysOnuType(int argc, char *argv[], void *p)
{
    setUserSpaceFlag(16);
	return 0;
}

#if defined(TCSUPPORT_CPU_MT7505) || defined(TCSUPPORT_CPU_EN7512) || defined(TCSUPPORT_CPU_EN7521)
int doSysGdmpDbg(int argc, char *argv[], void *p)
{
	if (argc == 2)
	{
		if ((strcmp(argv[1], "on")) == 0)
		{
			gdmpdebug = 1;
			printk("Now the gdmpDebug is on\r\n");
		}	
		else if	((strcmp(argv[1], "off")) == 0)
		{	
			printk("Now the gdmpDebug is off\r\n");
			gdmpdebug = 0;
		}	
	}
	else
	{	
		printk("usage: sys gdmpDebug <on | off>\r\n");
		printk("Now the gdmpDebug is %s\r\n",gdmpdebug ? "on":"off");
	}
	
	return 0;
}

int doSysGdmpMem(int argc, char *argv[], void *p)
{
	unsigned long length;
	unsigned char buf[18];
	int i,j = 0;
	struct file *fp;

	unsigned long *ptr;
	mm_segment_t fs;
		
	ptr = (unsigned long *)simple_strtoul(argv[1], NULL, 16);
	length = (unsigned long)simple_strtoul(argv[2], NULL, 16);
	if ((length % 8) != 0)
	{
		printk("the length must be n times 8!!!\n");
		return 0;
	}	
	
	memset(buf,0,sizeof(buf));
	fp=filp_open("/tmp/gdmp.raw",O_WRONLY | O_CREAT |O_TRUNC,0);
	if(IS_ERR(fp)) {
	    printk("file open error!\n");
	    return PTR_ERR(fp);
	} 
	
	fs=get_fs();
	set_fs(KERNEL_DS);
	for( i = 0; i < length/4; i = i + 2)
	{
		GDMPDEBUGP("%.8x", (unsigned long) (*(ptr + i + 1)));
		GDMPDEBUGP("%.8x", (unsigned long) (*(ptr + i)));
		GDMPDEBUGP( "\r\n");
	
		sprintf(buf,"%.8x%.8x\r\n",(unsigned long) (*(ptr + i + 1)),(unsigned long) (*(ptr + i)));
	    fp->f_op->write(fp,buf,sizeof(buf),&fp->f_pos);
		memset(buf,0,sizeof(buf));
	}    
	filp_close(fp,NULL);	
	set_fs(fs);
	printk("GDMP Memory dump Finished\n");
	return 0;
}

int doGDmpCfg(int argc, char *argv[], void *p)
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


	unsigned char buf[1024];
	struct file *fp;
	mm_segment_t fs;
	int len = 0;

	fp=filp_open("/tmp/gdmp.csr",O_WRONLY | O_CREAT |O_TRUNC,0);
	if(IS_ERR(fp)) {
	    printk("file open error!\n");
	    return PTR_ERR(fp);
	} 
	fs=get_fs();
	set_fs(KERNEL_DS);
	memset(buf,0,sizeof(buf));
	GDMPDEBUGP("GDMP_GLO_CFG\t%08lx\r\n",regRead32(GDMP_GLO_CFG));	
	len += sprintf(buf + len,"GDMP_GLO_CFG\t%08lx\r\n",regRead32(GDMP_GLO_CFG));
	
	GDMPDEBUGP("GDMP_CAP_RLT\t%08lx\r\n",regRead32(GDMP_CAP_RLT));
	len += sprintf(buf + len,"GDMP_CAP_RLT\t%08lx\r\n",regRead32(GDMP_CAP_RLT));
	
	GDMPDEBUGP("GDMP_TRG_RLT\t%08lx\r\n",regRead32(GDMP_TRG_RLT));	
	len += sprintf(buf + len,"GDMP_TRG_RLT\t%08lx\r\n",regRead32(GDMP_TRG_RLT));
	
	GDMPDEBUGP("GDMP_INT_STS\t%08lx\r\n",regRead32(GDMP_INT_STS));
	len += sprintf(buf + len,"GDMP_INT_STS\t%08lx\r\n",regRead32(GDMP_INT_STS));
	
	GDMPDEBUGP("GDMP_INT_MSK\t%08lx\r\n",regRead32(GDMP_INT_MSK));	
	len += sprintf(buf + len,"GDMP_INT_MSK\t%08lx\r\n",regRead32(GDMP_INT_MSK));
	
	GDMPDEBUGP("GDMP_PRB_SEL\t%08lx\r\n",regRead32(GDMP_PRB_SEL));
	len += sprintf(buf + len,"GDMP_PRB_SEL\t%08lx\r\n",regRead32(GDMP_PRB_SEL));

	GDMPDEBUGP("GDMP_TRG_PATN0_L\t%08lx\r\n",regRead32(GDMP_TRG_PATN0_L));	
	len += sprintf(buf + len,"GDMP_TRG_PATN0_L\t%08lx\r\n",regRead32(GDMP_TRG_PATN0_L));
	
	GDMPDEBUGP("GDMP_TRG_PATN0_MSK_L\t%08lx\r\n",regRead32(GDMP_TRG_PATN0_MSK_L));
	len +=sprintf(buf + len,"GDMP_TRG_PATN0_MSK_L\t%08lx\r\n",regRead32(GDMP_TRG_PATN0_MSK_L));

	GDMPDEBUGP("GDMP_TRG_PATN0_H\t%08lx\r\n",regRead32(GDMP_TRG_PATN0_H));	
	len +=sprintf(buf + len,"GDMP_TRG_PATN0_H\t%08lx\r\n",regRead32(GDMP_TRG_PATN0_H));
	
	GDMPDEBUGP("GDMP_TRG_PATN0_MSK_H\t%08lx\r\n",regRead32(GDMP_TRG_PATN0_MSK_H));
	len +=sprintf(buf + len,"GDMP_TRG_PATN0_MSK_H\t%08lx\r\n",regRead32(GDMP_TRG_PATN0_MSK_H));
	
	GDMPDEBUGP("GDMP_TRG_PATN1_L\t%08lx\r\n",regRead32(GDMP_TRG_PATN1_L));	
	len +=sprintf(buf + len,"GDMP_TRG_PATN1_L\t%08lx\r\n",regRead32(GDMP_TRG_PATN1_L));
	
	GDMPDEBUGP("GDMP_TRG_PATN1_MSK_L\t%08lx\r\n",regRead32(GDMP_TRG_PATN1_MSK_L));
	len +=sprintf(buf + len,"GDMP_TRG_PATN1_MSK_L\t%08lx\r\n",regRead32(GDMP_TRG_PATN1_MSK_L));
	
	GDMPDEBUGP("GDMP_TRG_PATN1_H\t%08lx\r\n",regRead32(GDMP_TRG_PATN1_H));	
	len +=sprintf(buf + len,"GDMP_TRG_PATN1_H\t%08lx\r\n",regRead32(GDMP_TRG_PATN1_H));
	
	GDMPDEBUGP("GDMP_TRG_PATN1_MSK_H\t%08lx\r\n",regRead32(GDMP_TRG_PATN1_MSK_H));
	len +=sprintf(buf + len,"GDMP_TRG_PATN1_MSK_H\t%08lx\r\n",regRead32(GDMP_TRG_PATN1_MSK_H));
	
	GDMPDEBUGP("GDMP_TRG_MODE\t%08lx\r\n",regRead32(GDMP_TRG_MODE));	
	len +=sprintf(buf + len,"GDMP_TRG_MODE\t%08lx\r\n",regRead32(GDMP_TRG_MODE));
	
	GDMPDEBUGP("GDMP_CAP_CFG\t%08lx\r\n",regRead32(GDMP_CAP_CFG));
	len +=sprintf(buf + len,"GDMP_CAP_CFG\t%08lx\r\n",regRead32(GDMP_CAP_CFG));

	GDMPDEBUGP("GDMP_CLK_CFG\t%08lx\r\n",regRead32(GDMP_CLK_CFG));
	len +=sprintf(buf + len,"GDMP_CLK_CFG\t%08lx\r\n",regRead32(GDMP_CLK_CFG));	
	fp->f_op->write(fp,buf,strlen(buf),&fp->f_pos);	
		
	filp_close(fp,NULL);
	set_fs(fs);
	printk("GDMP Register dump Finished\n");
	return 0;
}
#endif

#if defined(TCSUPPORT_TIMER_TEST)
uint32 getTimerCnt(void)
{
    return globe_cnt;
}

void pauseTimerCnt(uint32 second)
{
    volatile uint32 timebase;
    uint32 timepassed;

    timebase = getTimerCnt();
    printk("timebase %d\r\n", timebase);
    do
    {
        timepassed = getTimerCnt();       
        printk("timepassed %d\r\n", timepassed);      
        if(timepassed < timebase)
        {
           timepassed += (0xffffffff - timebase + 1); 
        }
        else
        {
            timepassed -= timebase;
        }
    }
    while(timepassed < second);
}

static irqreturn_t timer_isr(int irq, void *dev_id)
{
    uint32 word;
    /*1.close interrupt*/
    VPint(CR_INTC_IMR) &=~(1<<(TIMER0_IRQ_SRC + timer_sel));
    
    /*2.get interrupt status bit,and write 1 to clear*/
    word = VPint(CR_TIMER_CTL);
	word &= 0xffc0ffff;
	switch(timer_sel){
		case 0:
			word |= 0x00010000;
			break;
        case 1:
			word |= 0x00020000;
			break;
		case 2:
			word |= 0x00040000;
			break;
		case 3:
			word |= 0x00200000;
			break;
		default:
			word |= 0x00250000;
			break;
	}
	VPint(CR_TIMER_CTL) = word;
    
    /*3.interrupt process*/
	globe_cnt++;
    
    /*4.open interrupt*/
	VPint(CR_INTC_IMR) |=(1<<(TIMER0_IRQ_SRC + timer_sel));
  
	return IRQ_HANDLED ;
}

void intMaskSet(uint32 int_src, uint8 enable)
{
    if(enable)
    {   
		VPint(CR_INTC_IMR)|= (0x01 << int_src);
    }
    else
    {
    	VPint(CR_INTC_IMR)&= ~((0x01) << int_src);
    }
}

void timer_Configure(uint8  timer_no, uint8 timer_enable, uint8 timer_mode, uint8 timer_halt)
{
    uint32 word,word1;
    word = VPint(CR_TIMER_CTL);
    word1 = (timer_enable << timer_no)|(timer_mode << (timer_no + 8))|(timer_halt << (timer_no + 26));
    word |= word1;
    VPint (CR_TIMER_CTL)=word;    
} 

void timerSet(uint32 timer_no,uint32 timerTime, uint32 enable,uint32 mode, uint32 halt)
{   
    uint32 word;
	
    word = 1 * SYS_HCLK * 500;	/* 1000 /2; */
	word = word * timerTime;
    timerLdvSet(timer_no,word);
    timerCtlSet(timer_no,enable,mode,halt);
}

int doTimerTest(int argc, char *argv[], void *p)
{   
    uint8 src_no=0;
    uint8 wdog_test_flag = 0;
    timer_sel = simple_strtoul(argv[1], NULL, 10); 
    printk("timer %d test\r\n",timer_sel);

    if((timer_sel < 0) || (timer_sel > 3))
    {
        printk("Error timer number input ,should be <0~3>");
    }

    /*set irq src number,or wdog test flag*/
    switch(timer_sel)
    {
        case 0:
           src_no = CONFIG_TIMER0_IRQ; 
           break;
        case 1:
           src_no = CONFIG_TIMER1_IRQ; 
           break;
        case 2:
           src_no = CONFIG_TIMER2_IRQ; 
           break;
        case 3: 
           wdog_test_flag = 1; 
           break;
        default:
            break;
    }
    globe_cnt = 0;
    if(wdog_test_flag)
    {
        /*system reboot,use "watchdog_reset" in tcwdog.c,using timer3*/
        while(1);
    }
    else
    {
        /*enable timer irq and set irq processor function*/
        timerSet(timer_sel, TIMERTICKS_1S, ENABLE, TIMER_TOGGLEMODE, TIMER_HALTDISABLE);
        intMaskSet(TIMER0_IRQ_SRC + timer_sel, ENABLE);
        if(request_irq(src_no, timer_isr, 0, NULL, NULL) != 0) 
        {
            printk("Request the interrupt service routine fail, irq:%d.\n", src_no) ;
            return -ENODEV ;
        }
        
        /*test,pause 10s using our enabled timer*/
        printk("Timer %d test start,counter 10 second\r\n",timer_sel);
        pauseTimerCnt(10);
        printk("Timer %d test finish\r\n",timer_sel);
        
        /*disable timer irq src*/ 
        free_irq(src_no, NULL);
        intMaskSet(TIMER0_IRQ_SRC + timer_sel, DISABLE);
    }  
    return 0;
}
#endif

#if defined(TCSUPPORT_GDMA_TEST)
uint32 reverse32(uint32 ori_value)
{
    return (((ori_value&0xff000000) >> 24) | ((ori_value&0x00ff0000) >> 8) | ((ori_value&0x0000ff00) << 8)| ((ori_value&0x000000ff) << 24));
}

void gdmacopy(uint8 *dram_d_addr,uint8 *dram_s_addr,uint32 *sram_d_addr, uint32 *sram_s_addr, uint32 test_cnt) 
{
    uint32 i = 0;
    uint32 cnt = 0;
    uint8 addr_offset = 0;
    uint8 *s_dram = dram_s_addr;   
    uint8 *d_dram = dram_d_addr;
    uint32 *s_sram = sram_s_addr;   
    uint32 *d_sram = sram_d_addr;
    uint32 *s_dram_align = (uint32 *)dram_s_addr;    
    uint32 *d_dram_align = (uint32 *)dram_d_addr; 
    uint8 channel = 0;
    uint8 burst_size = 0;
    /*copy 1k data*/
    uint32 len = 1024;
    uint32 dword_align_len = len/4;
 
    printk("GDMA copy LEN:%d Dword\r\n",len);

    for(cnt = 0; cnt < test_cnt; cnt++)
    {
        for(channel = 0; channel < GDMA_CH_NUM; channel++)
        {
            for(burst_size = 0; burst_size <= GDMA_MAX_BURST; burst_size++)
            {
                /* printk("\r\n********************************************************************\r\n");*/
                printk("GDMA Test time:%d\r\n",cnt);
                printk("GDMA channel:%d\r\n",channel);
                printk("GDMA burst_size:%d\r\n",burst_size);   
                /* printk("\r\n********************************************************************\\r\n");  */
                
                /* DRAM TO DRAM */
                /* printk("\r\n#####################GDMA DRAM Copy to DRAM Test#####################\r\n"); */
                for(addr_offset = 0; addr_offset < 4; addr_offset++)
                {
                    s_dram = (uint8 *)(s_dram + addr_offset);
                    d_dram = (uint8 *)(d_dram + addr_offset);
                
                    /* printk("GDMA src addr:%x\r\n",s_dram); */
                    /* printk("GDMA dst addr:%x\r\n",d_dram); */
                
                    for(i = 0; i < len; i++)
                    {
                        s_dram[i] = (i+0x1)&0xff;
                    }
                
                    VPint(TC_GDMA_SA0 + 16*channel) = ((unsigned long)s_dram)&0x1fffffff; /* convert uncache to phy addr */
                    VPint(TC_GDMA_DA0 + 16*channel) = ((unsigned long)d_dram)&0x1fffffff;
                    VPint(TC_GDMA_CT10 + 16*channel) = (32<<16) | (32<8);
                    VPint(TC_GDMA_CT00 + 16*channel) = ((len&0xffff)<<16) | (burst_size<<3) | (1<<1) | (1<<0);

                    /*wait until done bit set*/
                    while(!(VPint(TC_GDMA_DONEINT) & (1<<channel)));   
                    /*write 1 to clear done bit*/
                    VPint(TC_GDMA_DONEINT) = 1<<channel;
                
                    for(i = 0; i < len; i++)
                    {
                        if(s_dram[i] != d_dram[i])
                        {
                            printk("\r\nCompare failed:dram_s_addr[%d]=%x  dram_d_addr[%d]=%x",i,s_dram[i],i,d_dram[i]);
                            printk("\r\ns_dram_addr=%x d_dram_addr=%x\r\n",&s_dram[i],&d_dram[i]);
                            return;
                        }
                    }
                    if(i == len)
                    {
                        /* printk("Compare completely success\r\n"); */
                    }    
                }
                
                /* DRAM TO SRAM(REG) */
                /* printk("\r\n#####################GDMA DRAM Copy to SRAM Test#####################\r\n"); */
                /* printk("GDMA src addr:%x\r\n",s_dram_align); */
                /* printk("GDMA dst addr:%x\r\n",d_sram); */
                
                for(i = 0; i < dword_align_len; i++)
                {
                    s_dram_align[i] = 0x12345678 + i;
                    /* printk("%x  ",s_dram_align[i]); */
                }
                printk("\r\n");
                VPint(TC_GDMA_SA0 + 16*channel) = ((unsigned long)s_dram_align)&0x1fffffff; /* convert uncache to phy addr */
                VPint(TC_GDMA_DA0 + 16*channel) = ((unsigned long)d_sram)&0x1fffffff;
                VPint(TC_GDMA_CT10 + 16*channel) = (32<<16) | (32<8);
                VPint(TC_GDMA_CT00 + 16*channel) = ((len&0xffff)<<16) | (burst_size<<3) | (1<<1) | (1<<0);
                
                while(!(VPint(TC_GDMA_DONEINT) & (1<<channel)));  
                VPint(TC_GDMA_DONEINT) = 1<<channel;/* write 1 clear */
                      
                for(i = 0; i < dword_align_len; i++)
                {   
                    d_sram[i] = reverse32(d_sram[i]);
                    if(s_dram_align[i] != d_sram[i])
                    {
                        printk("Compare failed:dram_s_addr[%d]=%x  sram_d_addr[%d]=%x",i,s_dram_align[i],i,d_sram[i]);        
                        printk("\r\ns_dram_align=%x d_sram=%x\r\n",&s_dram_align[i],&d_sram[i]);         
                        return;            
                    }
                }
                if(i == dword_align_len)
                {
                    /* printk("Compare completely success\r\n"); */
                }  
                
                /* SRAM(REG) TO DRAM */
                /* printk("\r\n#####################GDMA SRAM Copy to DRAM Test#####################\r\n");*/
                /* printk("GDMA src addr:%x\r\n",s_sram); */
                /* printk("GDMA dst addr:%x\r\n",d_dram_align); */
                for(i = 0; i < dword_align_len; i++)
                {
                    s_sram[i] = 0x456123789 + i;       
                    /* printk("%x  ",s_sram[i]); */
                }   
                printk("\r\n");
                VPint(TC_GDMA_SA0 + 16*channel) = ((unsigned long)s_sram)&0x1fffffff; /* convert uncache to phy addr */
                VPint(TC_GDMA_DA0 + 16*channel) = ((unsigned long)d_dram_align)&0x1fffffff;
                VPint(TC_GDMA_CT10 + 16*channel) = (32<<16) | (32<8);
                VPint(TC_GDMA_CT00 + 16*channel) = ((len&0xffff)<<16) | (burst_size<<3) | (1<<1) | (1<<0);
                
                while(!(VPint(TC_GDMA_DONEINT) & (1<<channel)));    
                VPint(TC_GDMA_DONEINT) = 1<<channel;
                        
                for(i = 0; i < dword_align_len; i++)
                {
                    d_dram_align[i] = reverse32(d_dram_align[i]);
                    if(s_sram[i] != d_dram_align[i])
                    {     
                        printk("Compare failed:sram_s_addr[%d]=%x  dram_d_addr[%d]=%x",i,s_sram[i],i,d_dram_align[i]);
                        printk("\r\ns_sram=%x d_dram_align=%x\r\n",&s_sram[i],&d_dram_align[i]);        
                        return;            
                    }
                }
                if(i == dword_align_len)
                {
                    /* printk("Compare completely success\r\n"); */
                }  
                
                /* SRAM(REG) TO SRAM(REG) */
                /* printk("\r\n#####################GDMA SRAM Copy to SRAM Test#####################\r\n"); */
                /* printk("GDMA src addr:%x\r\n",s_sram); */
                /* printk("GDMA dst addr:%x\r\n",d_sram); */
                
                for(i = 0; i < dword_align_len; i++)
                {
                    s_sram[i] = 0x87654321 + i;      
                    /* printk("%x  ",s_sram[i]); */
                }
                printk("\r\n");
                VPint(TC_GDMA_SA0 + 16*channel) = ((unsigned long)s_sram)&0x1fffffff; /* convert uncache to phy addr */
                VPint(TC_GDMA_DA0 + 16*channel) = ((unsigned long)d_sram)&0x1fffffff;
                VPint(TC_GDMA_CT10 + 16*channel) = (32<<16) | (32<8);
                VPint(TC_GDMA_CT00 + 16*channel) = ((len&0xffff)<<16) | (burst_size<<3) | (1<<1) | (1<<0);
                
                while(!(VPint(TC_GDMA_DONEINT) & (1<<channel)));
                VPint(TC_GDMA_DONEINT) = 1<<channel;
                
                for(i = 0; i < dword_align_len; i++)
                {
                    if(s_sram[i] != d_sram[i])
                    {
                        printk("Compare failed:sram_s_addr[%d]=%x  sram_d_addr[%d]=%x",i,s_sram[i],i,d_sram[i]);      
                        printk("\r\ns_sram=%x d_sram=%x\r\n",&s_sram[i],&d_sram[i]);
                        return; 
                    }
                }
                if(i == dword_align_len)
                {
                    /* printk("Compare completely success\r\n"); */
                } 
            }
        }
    }
}

int doGdmaTest(int argc, char *argv[], void *p)
{
#ifdef TCSUPPORT_CPU_EN7512
    uint8 *dram_s_addr = NULL;
    uint8 *dram_d_addr = NULL; 
    /* sram uncache,en7512 fe sram:0xbfa30000~bfa37fff */
    uint32 *sram_s_addr = (uint32 *)0xbfa35000;
    uint32 *sram_d_addr = (uint32 *)0xbfa32000;

    /* get param from console */
    uint32 test_cnt = simple_strtoul(argv[1], NULL, 10);   
    
    /* dram cache,1024+64(16 dword)  */
    uint8 *src_array = (uint8 *)kzalloc(2048, GFP_KERNEL);  
    uint8 *dst_array = (uint8 *)kzalloc(2048, GFP_KERNEL);
    
    /* cache covert  to uncache */
    dram_s_addr = (uint8 *)((uint32)src_array | 0x20000000);   
    dram_d_addr = (uint8 *)((uint32)dst_array | 0x20000000);

    /* set fe sram to normal memory */
    VPint(0xbfb00958) |= 1<<0;
    
    gdmacopy(dram_d_addr, dram_s_addr,sram_d_addr, sram_s_addr, test_cnt);	

    kfree(src_array);
    kfree(dst_array);

    /* restore to fe sram */
    VPint(0xbfb00958) &= ~(1<<0);
#endif
    return 0;
}
#endif

int doSysExMdio(int argc, char *argv[], void *p)
{
	return subcmd(subExMdioCmds, argc, argv, p);
}
int doExMdioConf(int argc, char *argv[], void *p)
{
	unsigned char tmpMdc;
	unsigned char tmpMdio;
	tmpMdc = (unsigned char)simple_strtoul(argv[1], NULL, 10);
	tmpMdio = (unsigned char)simple_strtoul(argv[2], NULL, 10);
	if(exModeMDIOGpioConf(tmpMdc,tmpMdio)){
		tmpMdc = tmpMdio = 0;
		exModeMDIOGpioQuery(&tmpMdc,&tmpMdio);
		printk("ex mdio config:mdc gpio:%d,mdio gpio:%d\n",tmpMdc,tmpMdio);
		return 0;
	}
	else
		return (-1);
}

int doExMdioWrite(int argc, char *argv[], void *p)
{
	unsigned int tmpReg = (unsigned int)simple_strtoul(argv[1], NULL, 16);
	unsigned int tmpVal = (unsigned int)simple_strtoul(argv[2], NULL, 16);
	exModeMDIOWrite(tmpReg,tmpVal);
	printk("write mdio reg 0x%x val:0x%x \n",tmpReg,exModeMDIORead(tmpReg));
	return 0;
}
int doExMdioRead(int argc, char *argv[], void *p)
{
	unsigned int tmpReg = (unsigned int)simple_strtoul(argv[1], NULL, 16);
	printk("mdio reg 0x%x val:0x%x \n",tmpReg,exModeMDIORead(tmpReg));
	return 0;
}


#ifdef L2_AUTOPVC
/*_____________________________________________________________________________
**      function name: autopvc_cmd_register_to_sys
**      descriptions:
**         Register autopvc ci-cmd into sys ci-cmd tables.
**
**      parameters:
**             cmds_p: Specify you want to register ci-cmd
**      global:
**             Cmds
**      return:
**              Success:        0
**              Otherwise:     -1
**      call:
**   	None
**      revision:
**      1. krammer 2008/8/13
**____________________________________________________________________________
*/
int
autopvc_cmd_register_to_sys(cmds_t *cmds_p)
{
	printk("register autopvc cmd to sys\n");
	int i=0;
	for(i=0; sysCmds[i].name; i++){
		if(memcmp(sysCmds[i].name,"autopvc",7) == 0){
			sysCmds[i].func = (*cmds_p).func;
			return 0;
		}
	}

	printk("Can't find autopvc-cmd\n");
	return -1;
}
int
autopvc_cmd_unregister_to_sys(void)
{
    printk("unregister autopvc cmd from sys\n");
	int i=0;
	for(i=0; sysCmds[i].name; i++){      
		if(memcmp(sysCmds[i].name,"autopvc",7) == 0){
            sysCmds[i].func = NULL;
			return 0;
		}
	}

	printk("Can't find autopvc-cmd\n");
	return -1;
}
EXPORT_SYMBOL(autopvc_cmd_register_to_sys);
EXPORT_SYMBOL(autopvc_cmd_unregister_to_sys);
#endif
#if defined(TCSUPPORT_WAN2LAN_EXT)
 u32 gsw_read(uint32 reg)
 {
 	if(isMT7520G || isMT7525G){
		if(gswPbusRead_hook != NULL)
			return gswPbusRead_hook(reg);
		else
			return 0;
 	}
	else{
		return read_reg_word(GSW_BASE + reg);
	}
 }
 void gsw_write(uint32 reg, u32 regVal)
 {
 	if(isMT7520G || isMT7525G){
		if(gswPbusWrite_hook != NULL)
			gswPbusWrite_hook(reg, regVal);
 	}
	else{
		write_reg_word(GSW_BASE + reg, regVal);
	}
	return;
 }
 int getPort(int lanPort)
 {
 	int portId = -1;
 	if(lanPortmap_hook)
		portId = lanPortmap_hook(lanPort);
	return portId;
 }
/*get lan port ,ex: LAN1 ,LAN2 ,LAN3 */
 int getLanPort(int switchPort)
 {
 	int portId = -1;
 	if(lanPortmap_rev_hook)
		portId = lanPortmap_rev_hook(switchPort);
	return portId;
 }
 
 static int doLan2lan(int argc, char *argv[], void *p)
 {
 	int srcLan = 0;	
	int portId = -1;
	int dstLan = 0;
	u32 regVal = 0;

	if(isMT7520G || isMT7525G){
		if(gswPbusWrite_hook == NULL){
			printk("Ext_switch, but not insmod raeth, error\n");
			return -1;
		}
	}
		
 	if(argc != 5 && argc != 2){
 		printk("Usage: lan2lan <src LanPort> [rx||tx||rxtx] [on] <dst LanPort>  or\r\n lan2lan off\r\n");
		return -1;
	}
	if(5 == argc){
		if((my_atoi(argv[4]) > 4) || (my_atoi(argv[4]) < 1)){
			printk("dst Lan port out of range(available value is 1~4) \r\n");
			return -1;
		}
		if (strcmp(argv[3], "on") == 0){
			srcLan = my_atoi(argv[1]);
			if(srcLan < 1 || srcLan >4){
				printk("Src Lan port must in 1~4.");
					return -1;
			}
			portId = getPort(srcLan);
			if(-1 == portId){
				printk("Invalid src LanPort!\n");
				return -1;
			}
			if(strcmp(argv[2], "rx") == 0){
				regVal = gsw_read(GSW_PCL(portId));
				regVal |= 1<<8;		 /* Enable TX */
				gsw_write(GSW_PCL(portId), regVal);
				printk("lan2lan src Lan port %d rx on\r\n", srcLan);
			}
			else if(strcmp(argv[2], "tx") == 0){
				regVal = gsw_read(GSW_PCL(portId));
				regVal |= 1<<9;		 /* Enable RX */
				gsw_write(GSW_PCL(portId), regVal);
				printk("lan2lan src Lan port %d tx on\r\n", srcLan);
			}
			else if(strcmp(argv[2], "rxtx") == 0){
				regVal = gsw_read(GSW_PCL(portId));
				regVal |= 1<<8;
				regVal |= 1<<9;
				gsw_write(GSW_PCL(portId), regVal);
				printk("lan2lan src Lan port %d rx and tx on\r\n", srcLan);
			}
			else{
				printk("Usage: lan2lan <src LanPort> [rx||tx||rxtx] [on] <dst LanPort>  or\r\n      lan2lan off\r\n");
				return -1;
			}
			dstLan = my_atoi(argv[4]);
			portId = getPort(dstLan);
			if(-1 == portId){
				printk("Invalid dst LanPort!\n");
				return -1;
			}
			if(dstLan == srcLan){
				printk("Src port is the same with the dst port, error!\n");
				return -1;
			}
			regVal = gsw_read(GSW_MAC_FC);
			regVal |= 1<<3;   /* Enable mirror function */
			regVal &= 0xFFFFFFF8;
			regVal |= portId;
			gsw_write(GSW_MAC_FC, regVal);
			printk("lan2lan dst Lan port %d\r\n", dstLan);
			
			return 0;
		}
		else{
			printk("Usage: lan2lan <src LanPort> [rx||tx||rxtx] [on] <dst LanPort>  or\r\n      lan2lan off\r\n");
			return -1;
		}

	}	
	else if(2 == argc){
		if(strcmp(argv[1], "off") == 0){
			regVal = gsw_read(GSW_MAC_FC);
			regVal &= ~(1<<3);
			regVal &= 0xFFFFFFF8;
			gsw_write(GSW_MAC_FC, regVal);
			for(srcLan = 1; srcLan <= 4; srcLan++){
				portId = getPort(srcLan);
				if(-1 == portId){
					continue;
				}
				regVal = gsw_read(GSW_PCL(portId));
				regVal &= ~(1<<8);
				regVal &= ~(1<<9);
				gsw_write(GSW_PCL(portId), regVal);
			}
			printk("lan2lan now is off\r\n");
			return 0;
		}
		else{
			printk("Usage: lan2lan <src LanPort> [rx||tx||rxtx] [on] <dst LanPort>  or\r\n      lan2lan off\r\n");
			return -1;
		}
	}
	else{
		printk("Usage: lan2lan <src LanPort> [rx||tx||rxtx] [on] <dst LanPort>  or\r\n      lan2lan off\r\n");
		return -1;
	}
	return -1;

}
#endif
#ifdef WAN2LAN
int my_atoi(const char *str)
{
	int result = 0;
	int signal = 1;
	if( (*str >= '0' && *str <= '9') || *str == '-' || *str == '+' ){
		if( *str == '-' || *str == '+' ){
			if( *str == '-' )
				signal = -1;
			str++;
		}
	}
	else 
		return 0;
	while( *str >= '0' && *str <= '9' )
		result = result*10 + (*str++ - '0');
	return signal*result;
}

//merge from linos,but it dosen't support SW3052
static int doWan2lan(int argc, char *argv[], void *p)
{
#if defined(TCSUPPORT_CMDPROMPT)
    	setUserSpaceFlag(13);
#endif

#if defined(TCSUPPORT_WAN2LAN_EXT)
		if(argc < 2 || argc > 5){
#else
		if(argc < 2 || argc > 3){
#endif
#if !defined(TCSUPPORT_CMDPROMPT)
#if defined(TCSUPPORT_WAN2LAN_EXT)
			printk("Usage: wan2lan [on||off] <number> , or\r\n wan2lan on [up||down] [out||in||raeth] <number>\r\n");
#else
			printk("Usage: wan2lan [on||off] <number> \r\n");
#endif
#else
		print_usage=1;
#endif
		if(masko_on_off == 1){
#if !defined(TCSUPPORT_CMDPROMPT)
			printk("Current wan2lan feature status: on \r\n");
			printk("Current portmask number is[%d] \r\n",masko);
#else
			print_state=1;
			print_portmask=1;
#endif
		}
		else if(masko_on_off == 0){
#if !defined(TCSUPPORT_CMDPROMPT)
			printk("Current wan2lan feature status: off \r\n");
#else
			print_state=1;
#endif
		}
		return -1;
	}
	if(argc == 3){
			if((my_atoi(argv[2]) > 15) || (my_atoi(argv[2]) < 0)){
#if !defined(TCSUPPORT_CMDPROMPT)
			printk("input portmask number is out of range(available value is 0~15) \r\n");
#else
			print_errormsg=1;
#endif
			return -1;
		}
	}
#if defined(TCSUPPORT_WAN2LAN_EXT)
		else if(argc == 5){
				if((my_atoi(argv[4]) > 15) || (my_atoi(argv[4]) < 0)){
#if !defined(TCSUPPORT_CMDPROMPT)
				printk("input portmask number is out of range(available value is 0~15) \r\n");
#else
				print_errormsg=1;
#endif
				return -1;
			}
		}
#endif
#if !defined(TCSUPPORT_WAN2LAN_EXT)
	if (strcmp(argv[1], "on") == 0){
		masko_on_off = 1;
		masko = 0xF;
#if !defined(TCSUPPORT_CMDPROMPT)
		printk("Current wan2lan feature status: on \r\n");
#else
		print_state=1;
#endif
	}	
#else
	if (strcmp(argv[1], "on") == 0){
		if(argc == 3 || argc == 2)
			masko_on_off = 1;
		masko = 0xF;
#if !defined(TCSUPPORT_CMDPROMPT)
		printk("Current wan2lan feature status: on \r\n");
#else
		print_state=1;
#endif
	}	

#endif
	else if(strcmp(argv[1], "off") == 0){
		masko_on_off = 0;
#if defined(TCSUPPORT_WAN2LAN_EXT)
		masko_ext = 0;
		masko = 0x0;
#endif
#if !defined(TCSUPPORT_CMDPROMPT)
		printk("Current wan2lan feature status: off \r\n");
#else
		print_state=1;
#endif
		return 0;
	}
#if defined(TCSUPPORT_WAN2LAN_EXT)
	else{
		printk("Usage: wan2lan [on||off] <number> , or\r\n wan2lan on [up||down] [out||in||raeth] <number>\r\n");
		return -1;
	}
	if(argc >= 4){
		if (strcmp(argv[2], "up") == 0){
			if(strcmp(argv[3], "out") == 0)
				masko_ext |= 0x01;
			else if(strcmp(argv[3], "in") == 0)
				masko_ext |= 0x04;
			else if(strcmp(argv[3], "raeth") == 0)
				masko_ext |= 0x10;
			else{
				printk("Usage: wan2lan [on||off] <number> , or\r\n wan2lan on [up||down] [out||in||raeth] <number>\r\n");
				return -1;
			}
		}	
		else if(strcmp(argv[2], "down") == 0){
			if(strcmp(argv[3], "out") == 0)
				masko_ext |= 0x02;
			else if(strcmp(argv[3], "in") == 0)
				masko_ext |= 0x08;
			else if(strcmp(argv[3], "raeth") == 0)
				masko_ext |= 0x20;
			else{
				printk("Usage: wan2lan [on||off] <number> , or\r\n wan2lan on [up||down] [out||in||raeth] <number>\r\n");
				return -1;
			}
		}
		else if(strcmp(argv[2], "ext") == 0){
			if(my_atoi(argv[3]) > 0x3f || my_atoi(argv[3]) < 1){
				printk("mask error, must in 0x01~0x3f\n");
				return -1;
			}
			else{
				masko_ext |= my_atoi(argv[3]);
			}
		}
		
#if 0
		else if(strcmp(argv[2], "updown") == 0){
			if(strcmp(argv[3], "out") == 0)
				masko_ext |= 0x03;
			else if(strcmp(argv[3], "in") == 0)
				masko_ext |= 0x0c;
			else{
				printk("Usage: wan2lan [on||off] <number> , or\r\n wan2lan on [up||down] [out||in||raeth] <number>\r\n");
				return -1;
			}
		}
#endif
		else{
			printk("Usage: wan2lan [on||off] <number> , or\r\n wan2lan on [up||down] [out||in||raeth] <number>\r\n");
			return -1;
		}
	}
	printk("Current masko_ext number is[%d] \r\n",masko_ext);
#endif
	if(argc == 3){
		masko = simple_strtoul(argv[2],NULL, 10);
#if !defined(TCSUPPORT_CMDPROMPT)
		printk("Current portmask number is[%d] \r\n",masko);
#else
		print_portmask=1;
#endif
		return 0;
	}
#if defined(TCSUPPORT_WAN2LAN_EXT)
	else if(argc == 5){
		masko = simple_strtoul(argv[4],NULL, 10);
		printk("Current portmask number is[%d] \r\n",masko);
		return 0;
	}
	else{
		printk("Current portmask number is[%d] \r\n",masko);
	}
#endif
	return -1;
}
		
#if defined(TCSUPPORT_WAN2LAN_EXT)
int sys_proc_wan2lan_ext_point_mask(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
		u32 index = 0;
		off_t pos = 0;
		off_t begin = 0;
	
		index += sprintf(buf+index, "%x\n", masko_ext );
	
		*eof = 1;
	
	done:
		*start = buf + (off - begin);
		index -= (off - begin);
		if (index<0) 
			index = 0;
		if (index>count) 	
			index = count;
		return index;

}

int sys_proc_wan2lan_ext_port_mask(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
		u32 index = 0;
		off_t pos = 0;
		off_t begin = 0;

		index += sprintf(buf+index, "%x\n", masko);
	
		*eof = 1;
	
	done:
		*start = buf + (off - begin);
		index -= (off - begin);
		if (index<0) 
			index = 0;
		if (index>count) 
			index = count;
		return index;

}

int sys_proc_lan2lan_src_ports(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
		u32 index = 0;
		u32 regVal = 0;
		u32 Vlaue = 0;
		int portId = -1;
		u32 LAN_port = 0;  /*LAN_port from 1 ~4 .LAN1 LAN2 LAN3 LAN4*/
		u32 LAN_port_rx_tx_Value = 0;
		off_t pos = 0;
		off_t begin = 0;
		
		for(portId = 0;portId < 6;portId++ )
		{
			regVal = gsw_read(GSW_PCL(portId));

			/*bit8 ,bit9 rx tx enbale */
			Vlaue = ((regVal & 0x300)>>8);
			if(Vlaue)
			{
				LAN_port = getLanPort(portId);
				LAN_port_rx_tx_Value |= (Vlaue<<(2*(LAN_port-1)));
			}
		}
		/*LAN_port_rx_tx_Value. bit0,1 :LAN1 rx,tx enable ; bit2,3 :LAN2 rx,tx enable ;*/
		index += sprintf(buf+index, "%x\n", LAN_port_rx_tx_Value);
	
		*eof = 1;
	
	done:
		*start = buf + (off - begin);
		index -= (off - begin);
		if (index<0) 
			index = 0;
		if (index>count) 
			index = count;
		return index;

}

int sys_proc_lan2lan_dst_port(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
		u32 index = 0;
		u32 regVal= 0;
		int switch_port = -1 ;
		int LAN_port = 0;  /*LAN_port_port from 1 ~4 .LAN1 LAN2 LAN3 LAN4*/
		off_t pos = 0;
		off_t begin = 0;
		
		/* 0xbfb58010,  . bit 3: Enable mirror function ,bit 2~0: port num ,only support one port */
		regVal = gsw_read(GSW_MAC_FC);
		if(regVal & 0x8)
		{
			switch_port = (regVal & 0x7) ;
			LAN_port = getLanPort(switch_port);
		}
		index += sprintf(buf+index, "%d\n", LAN_port);

		*eof = 1;
	
	done:
		*start = buf + (off - begin);
		index -= (off - begin);
		if (index<0) 
			index = 0;
		if (index>count) 
			index = count;
		return index;

}
				 
#endif

#if defined(TCSUPPORT_CMDPROMPT)
int
getWan2lanInfo(char *wan2lan_print)
{
	uint16	index=0;

	if(print_usage)
		index += sprintf( wan2lan_print+index, WAN2LAN_INFO_USAGE);
	if(print_errormsg)
		index += sprintf( wan2lan_print+index, WAN2LAN_INFO_ERRORMSG);
	if(print_state){
		if(masko_on_off == 1)
			index += sprintf( wan2lan_print+index, WAN2LAN_INFO_ON);
		else if(masko_on_off == 0)
			index += sprintf( wan2lan_print+index, WAN2LAN_INFO_OFF);
	}
	if(print_portmask)
			index += sprintf( wan2lan_print+index, WAN2LAN_INFO_PORTMASK,masko);

	print_usage=0;
	print_errormsg=0;
	print_state=0;
	print_portmask=0;
	
	return index;
}
int sys_proc_wan2lan(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getWan2lanInfo(buf);
	if (len <= off+count)
		*eof = 1;
	*start = buf + off;
	len -= off;
	if (len>count)
		len = count;
	if (len<0)
		len = 0;
	return len;
}

#endif
#endif

#if defined(TCSUPPORT_CMDPROMPT)
int
getSysSubcmdsInfo(char *sys_subcmds)
{
	uint16	index=0;
	register const cmds_t *cmdp=NULL;
	char buf[66]={0};
	int i=0;

	index += sprintf( sys_subcmds+index, "valid subcommands:\n");

	memset(buf,' ',sizeof(buf));
	buf[64] = '\n';
	buf[65] = '\0';

	for(i=0,cmdp = sysCmds;cmdp->name != NULL;cmdp++){
			strncpy(&buf[i*16],cmdp->name,strlen(cmdp->name));
			if(i == 3){
				index += sprintf( sys_subcmds+index, buf);
				memset(buf,' ',sizeof(buf));
				buf[64] = '\n';
				buf[65] = '\0';
			}
			i = (i+1)%4;
	}
	if(i != 0)
		index += sprintf( sys_subcmds+index, buf);

	return index;
}
int sys_proc_subcmds(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getSysSubcmdsInfo(buf);
	if (len <= off+count)
		*eof = 1;
	*start = buf + off;
	len -= off;
	if (len>count)
		len = count;
	if (len<0)
		len = 0;
	return len;
}
#endif


#if defined(TCSUPPORT_AUTOBENCH_AFE) || defined(TCSUPPORT_AUTOBENCH_MT7510)
static int waitDmtPowerOn(int argc, char *argv[], void *p)
{
	unsigned long int value;
	/* wait until dmt3095 is power on */       
        while(1){
		if(isRT63365){
			//printk("GPIO %x\n",(VPint(CR_GPIO_DATA)));
			value = (VPint(CR_GPIO_DATA) & (1<<0));
		}else{
			value = (VPint(CR_GPIO_DATA) & 0x2000000);
		}	
		if(value){
			return 0;
		}else{
			msleep(1000);
		}
	
		tc3162wdog_kick();
        }
}

static int checkReboot(int argc, char *argv[], void *p)
{
    if(VPint(0x8001fffc)==0x1234){
        VPint(0x8001fffc)=0;
        return 0; /* return back to get a console for setting */
    }
    else{
        printk("wait for system to reboot\n");
        while(1);
    }
}
#endif

#if defined(TCSUPPORT_LED_BTN_CHECK) || defined(TCSUPPORT_TEST_LED_ALL) 
#if defined(TCSUPPORT_LED_CHECK) || defined(TCSUPPORT_TEST_LED_ALL) 
static int doLedCheck(int argc, char *argv[], void *p)
{
	setUserSpaceFlag(9);
	if((argc == 2) && ((strcmp(argv[1], "on") == 0) || (strcmp(argv[1], "off") == 0) || (strcmp(argv[1], "recover") == 0))){
		if(strcmp(argv[1], "off") == 0){
			doLedOff();
			printk("All led is turned off! \r\n");	
		 }	
		else if(strcmp(argv[1], "on") == 0){
			doLedOn();
			printk("All led is turned on! \r\n");
		}
		else
		{
			doLedRecover();
			printk("Recover led status! \r\n");
		}
	}
	else{
		printk("Usage: sys led [on||off|recover] \r\n");
		return -1;
	}

	return 0;
}	
#endif
#if defined(TCSUPPORT_BTN_CHECK)
static int doButtonCheck(int argc, char *argv[], void *p)
{
	setUserSpaceFlag(10);
	if(argc != 2 || (strcmp(argv[1], "enable") && strcmp(argv[1], "disable"))){
		printk("Usage: sys button [enable||disable]\r\n");
		return -1;
	}
	if (strcmp(argv[1], "disable") == 0){
		gButtonCheck = 1;
		printk("All buttons are disabled! \r\n");
	}	
	else{
		gButtonCheck = 0;
		printk("All buttons are enabled! \r\n");	
	 }

	return 0;
}	
#endif
#endif

#if defined(TCSUPPORT_USBHOST)
int doUSBEYE(int argc, char *argv[], void *p)
{
	printk("USB EYE TEST\n");
	if(isRT63365 || isMT751020 || isMT7505){
		VPint(0xbfbb0054) = 0x00041000;
		VPint(0xbfbb0058) = 0x00041000;
	}
	else if(isEN751221){
		VPint(0xbfb90434) = 0x40000000;
		VPint(0xbfb90444) = 0x40000000;
	}
	else{
		VPint(0xbfb80cf8) = 0x80001b40;
		VPint(0xbfb80cfc) = 0x800000e0;
		VPint(0xbfba1060) = 0x1000000;
		VPint(0xbfba1064) = 0x4000500;
		VPint(0xbfba1068) = 0x4000500;
		VPint(0xbfba1064) = 0x4000400;
		VPint(0xbfba1068) = 0x4000400;
	}
	return 0;
}
#endif


#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,31))
extern int watchFlag;
#endif
int doSysWatch(int argc, char *argv[], void *p)
{
	uint32 mask =0;
	uint32 val_Lo = 0x0;
	uint32 val_Hi = 0x40000003;
	#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,31))
	int watchFlag=0;
	#endif
	if(argc<=3){
		printk("usgae: sys watch <flag0~3> <addr> <mask> \n");
		printk("flag 1: write only\t 2: read only\t 3:read/write\n");
		printk("addr and mask MUST be Hex, mask value:<0~0x1ff>\n");
		printk("addr value must be double word\n");
		return -1;
	}
	watchFlag = simple_strtoul(argv[1], NULL, 10);
	if(argc==4){
		val_Lo = simple_strtoul(argv[2], NULL, 16);
		val_Lo &=(~0x7); //bit0:2 used for w/r/i
		mask = simple_strtoul(argv[3], NULL, 16);
	}

	if(mask>0x1ff){
		printk("mask value error, mask value:<0~0x1ff> return\n");
		watchFlag=0;
		return -1;
	}
	printk("watch addr= %08lx, mask=%08lx\n",val_Lo,mask);
	switch(watchFlag){
		case 1://write only
			val_Lo |= (0x1<<0);
			break;
		case 2://read only
			val_Lo |= (0x1<<1);
			break;
		case 3://r/w
			val_Lo |= (0x3<<0);
			break;
		default:
			printk("do not monitor any addr,return\n");
			watchFlag=0;
			break;
	}
	if(watchFlag==0)
		return -1;
		
	val_Hi |= (mask<<3);
	
	printk("val_Lo=%08lx,val_Hi=%08lx\n",val_Lo,val_Hi);
	write_c0_watchlo2(val_Lo);
	write_c0_watchhi2(val_Hi);

	return 0;
}
#ifdef TCSUPPORT_PON_TEST
int doLaserTest(int argc, char *argv[], void *p)
{
	unsigned int write_data = 0;
	unsigned int read_data = 0;
	if(argc != 2 || (strcmp(argv[1], "on") && strcmp(argv[1], "off"))){
		printk("Usage: sys lasertest [on||off]\r\n");
		return -1;
	}

	if (strcmp(argv[1], "on") == 0){
		read_data = VPint(0xbfaf0108);
		write_data = (read_data & 0xffffff5f) | (1 * 0xa0);
		VPint(0xbfaf0108) = write_data;
		printk("Enable laser force up! \r\n");
	}	
	else{
		read_data = VPint(0xbfaf0108);
		write_data = (read_data & 0xffffff5f) | (0 * 0xa0);
		VPint(0xbfaf0108) = write_data;
		printk("Disable laser force up! \r\n");	
	 }

	return 0;
}

int doPonTempTest(int argc, char *argv[], void *p){
	setUserSpaceFlag(17);
	return 0;
}

int doSysAtbp(int argc, char *argv[], void *p){
	setUserSpaceFlag(18);
	return 0;
}
int doSysPonLinkStatus(int argc, char *argv[], void *p){
	setUserSpaceFlag(19);
	return 0;
}
#endif

int doImageInfo(int argc,char *argv[],void *p)
{
	setUserSpaceFlag(21);
	return 0;
}
int doVoipTest(int argc, char *argv[], void *p)
{
	setUserSpaceFlag(22);
	return 0;
}

int doSysBobCarlibration(int argc, char *argv[], void *p)
{
	setUserSpaceFlag(23);
	return 0;
}


#if defined(TCSUPPORT_CT_UPG_PINGPONG)
int doSysBootFlag(int argc, char *argv[], void *p)
{
	setUserSpaceFlag(24);
	return 0;
}
#endif

int doBindShowTask(int argc, char *argv[], void *p)
{
	setUserSpaceFlag(25);
	return 0;
}


#ifdef TCSUPPORT_CDS
int doDead(int argc,char *argv[],void *p)
{	
	while(1)	
	{
	}	
	return 0;
}

#endif

int tcsyscmd_init(void)
{
	cmds_t sys_cmd;
#if defined(TCSUPPORT_POWERSAVE_ENABLE) 
	uint32 value=0;
#endif
#if defined(TCSUPPORT_CMDPROMPT)
	/* sys subcommands  info */
	create_proc_read_entry("tc3162/sys_subcmds", 0, NULL, sys_proc_subcmds, NULL);
#ifdef WAN2LAN
	/* sys wan2lan related info */
	create_proc_read_entry("tc3162/sys_wan2lan", 0, NULL, sys_proc_wan2lan, NULL);
#endif
#endif

#if defined(TCSUPPORT_WAN2LAN_EXT)
			/* sys wan2lan lan2lan  related info */
	create_proc_read_entry("tc3162/sys_wan2lan_ext_src_point_mask", 0, NULL, sys_proc_wan2lan_ext_point_mask, NULL);
	create_proc_read_entry("tc3162/sys_wan2lan_ext_dst_port_mask", 0, NULL, sys_proc_wan2lan_ext_port_mask, NULL);
	create_proc_read_entry("tc3162/sys_lan2lan_src_ports", 0, NULL, sys_proc_lan2lan_src_ports, NULL);
	create_proc_read_entry("tc3162/sys_lan2lan_dst_port", 0, NULL, sys_proc_lan2lan_dst_port, NULL);

#endif


	/*Init sys root ci-cmd*/
	sys_cmd.name= "sys";
	sys_cmd.func=doSys;
	sys_cmd.flags=0x12;
	sys_cmd.argcmin=0;
	sys_cmd.argc_errmsg=NULL;

#if defined(TCSUPPORT_POWERSAVE_ENABLE)
	if(!isMT751020){
#if !defined(TCSUPPORT_USBHOST)
	VPint(0xbfb000a8)=0;
#endif
	}
#endif
	/*Register sys ci-cmd*/
	cmd_register(&sys_cmd);

	return 0;
}

//static void __exit tcsyscmd_exit(void)
void tcsyscmd_exit(void)
{
#if defined(TCSUPPORT_CMDPROMPT)
	remove_proc_entry("tc3162/sys_subcmds", NULL);
#ifdef WAN2LAN
	remove_proc_entry("tc3162/sys_wan2lan", NULL);
#endif
#endif
#if defined(TCSUPPORT_WAN2LAN_EXT)
			/* sys wan2lan lan2lan  related info */
	remove_proc_entry("tc3162/sys_wan2lan_ext_src_point_mask", NULL);
	remove_proc_entry("tc3162/sys_wan2lan_ext_dst_port_mask",  NULL);
	remove_proc_entry("tc3162/sys_lan2lan_src_ports", NULL);
	remove_proc_entry("tc3162/sys_lan2lan_dst_port",  NULL);

#endif
}
//module_init (tcsyscmd_init);
//module_exit (tcsyscmd_exit);

