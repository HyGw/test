/*
** $Id: tcwdog.c,v 1.2 2010/12/09 13:18:35 xmdai_nj Exp $
*/
/************************************************************************
 *
 *	Copyright (C) 2006 Trendchip Technologies, Corp.
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
/*
** $Log: tcwdog.c,v $
** Revision 1.5  2011/09/23 02:04:50  shnwind
** Add rt63365 support
**
** Revision 1.4  2011/07/07 07:55:51  shnwind
** RT63260 & RT63260 auto_bench support
**
** Revision 1.3  2011/06/03 02:04:23  lino
** add RT65168 support
**
** Revision 1.2  2010/12/09 13:18:35  xmdai_nj
** #7955:When doing upgrade firmware in web page, it can not reboot.
**
** Revision 1.1.1.1  2010/04/09 09:39:13  feiyan
** New TC Linux Make Flow Trunk
**
** Revision 1.1.1.1  2009/12/17 01:43:39  josephxu
** 20091217, from Hinchu ,with VoIP
**
** Revision 1.1.1.1  2007/04/12 09:42:02  ian
** TCLinuxTurnkey2007
**
** Revision 1.2  2006/07/06 07:24:23  lino
** update copyright year
**
** Revision 1.1.1.1  2005/11/02 05:45:19  lino
** no message
**
*/
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/miscdevice.h>
#include <linux/smp_lock.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <asm/tc3162/tc3162.h>

#include <asm/tc3162/TCIfSetQuery_os.h>

#ifdef CONFIG_WATCHDOG_NOWAYOUT
static int nowayout = 1;
#else
static int nowayout = 0;
#endif

#if 0
MODULE_PARM(nowayout,"i");
MODULE_PARM_DESC(nowayout, "Watchdog cannot be stopped once started (default=CONFIG_WATCHDOG_NOWAYOUT)");
#endif

extern unsigned int cpu_timer_loss[4];

static int watchdog_enabled = 0;

extern void timer_Configure(uint8  timer_no, uint8 timer_enable, uint8 timer_mode, uint8 timer_halt);
extern void timerSet(uint32 timer_no, uint32 timerTime, uint32 enable, uint32 mode, uint32 halt);

extern void timer_WatchDogConfigure(uint8 tick_enable, uint8 watchdog_enable);

static spinlock_t  protect_lock;
#define PPE_DFP_CPORT	  0xbfb50e48
#define PPE_TB_CFG	      0xbfb50e1c

void 
attack_protect_set(int active, int mode){
	unsigned int value=0;
	unsigned int val_1=0;
	unsigned int flags=0;

	spin_lock_irqsave(&protect_lock, flags);

	/*active : o , off, 1 on. mode: 1 wan protect, 2, lan protect, 0, not set*/
	if(active == 1){
		if(mode == 1){
			/*disable keep alive*/
			value = regRead32(PPE_TB_CFG);
			value &= ~(0x3000);
			regWrite32(PPE_TB_CFG, value);
			/*default port 2 to drop*/
			value = regRead32(PPE_DFP_CPORT);
			value &= ~(0x700);
			value |= 0x700;
			regWrite32(PPE_DFP_CPORT, value);
		}
		else if(mode == 2){
			/*disable keep alive*/
			value = regRead32(PPE_TB_CFG);
			value &= ~(0x3000);
			regWrite32(PPE_TB_CFG, value);
			
			/*default port 1 to drop*/
			value = regRead32(PPE_DFP_CPORT);
			value &= ~(0x070);
			value |= 0x070;
			regWrite32(PPE_DFP_CPORT, value);
		}
		else{
			;/*not settings*/
		}
	}
	else{
		/*restore normal settings*/
		if(mode == 1){
			val_1 = regRead32(PPE_DFP_CPORT);
			if((val_1 & 0x70)== 0x0){ /*make sure the keep alive is setting by qdma driver*/
				/*restore the keep alive*/
				value = regRead32(PPE_TB_CFG);
				value &= ~(0x3000);
				value |= 0x3000;
				regWrite32(PPE_TB_CFG, value);
			}
			/*restore p2 to qdma port*/
			value = val_1;
			value &= ~(0x700);
			value |= 0x500;
			regWrite32(PPE_DFP_CPORT, value);
		}
		else if(mode == 2){ 
			val_1 = regRead32(PPE_DFP_CPORT);
			if((val_1 & 0x700) == 0x500){ /*make sure the keep alive is setting by femac driver*/
				/*restore the keep alive*/
				value = regRead32(PPE_TB_CFG);
				value &= ~(0x3000);
				value |= 0x3000;
				regWrite32(PPE_TB_CFG, value);
			}
			/*restore p1 to cpu port*/
			value = val_1;
			value &= ~(0x70);
			regWrite32(PPE_DFP_CPORT, value);
		}
		else{
			;/*not settings*/
		}
	}
	spin_unlock_irqrestore(&protect_lock, flags);

}
EXPORT_SYMBOL(attack_protect_set);

void watchDogReset(void)
{
#ifdef CONFIG_TC3162_ADSL
    /* stop adsl */
	if (adsl_dev_ops)
	    adsl_dev_ops->set(ADSL_SET_DMT_CLOSE, NULL, NULL); 
#endif

#if defined(CONFIG_MIPS_TC3262) && defined(TCSUPPORT_POWERSAVE_ENABLE)
	if(isRT63365)
		VPint(CR_AHB_CLK) |= 0x57e1;//restore ahb clk to default value
#endif

#if defined(TCSUPPORT_CPU_MT7505) || defined(TCSUPPORT_CPU_MT7510)
	VPint(CR_INTC_IMR) = 0x0;
	VPint(CR_TIMER_CTL) = 0x0;
#endif
#if defined(MT7592) && (defined(TCSUPPORT_CPU_MT7520)|| defined(TCSUPPORT_CPU_MT7510))
//#if defined(TCSUPPORT_WLAN_MT7592_PCIE) && defined(TCSUPPORT_CPU_MT7520)
	printk("0xbfb00834=0xeff88ce0 \n");
	VPint(CR_INTC_IMR) = 0x0;
	VPint(CR_TIMER_CTL) = 0x0;
#endif

/*watchdog reset*/
	timerSet(5, 10 * TIMERTICKS_10MS, ENABLE, TIMER_TOGGLEMODE, TIMER_HALTDISABLE);
	timer_WatchDogConfigure(ENABLE, ENABLE);
	
#if defined(MT7592) && (defined(TCSUPPORT_CPU_MT7520)|| defined(TCSUPPORT_CPU_MT7510))
//#if defined(TCSUPPORT_WLAN_MT7592_PCIE) && defined(TCSUPPORT_CPU_MT7520)
	VPint(0xbfb00834) = 0xeff88ce0;
#endif

#if defined(TCSUPPORT_CPU_MT7505) || defined(TCSUPPORT_CPU_MT7510)
	VPint(CR_DRAMC_CONF) &= ~(0x1<<2);
#endif

	while(1);
//	_machine_restart();
}

void tc3162wdog_kick(void)
{
	/* Clear Watchdog timer counter */
	uint32 word;

	if (isRT63165 || isRT63365 || isRT63260 || isMT751020 || isMT7505 || isEN751221) {
		regWrite32(CR_WDOG_RLD, 0x1);
	} else {
		word = regRead32(CR_TIMER_CTL);
		word &= 0xffc0ffff;
		word |= 0x00200000;
		regWrite32(CR_TIMER_CTL, word);
	}
}
EXPORT_SYMBOL(tc3162wdog_kick);

/* handle open device */

static int tc3162wdog_open(struct inode *inode, struct file *file)
{
	/*	Allow only one person to hold it open */
	if(watchdog_enabled)
		return -EBUSY;
	if (nowayout) {
		printk("Watchdog cannot be stopped once started. \n");
	}
  
	timerSet(5, 2000 * TIMERTICKS_10MS, ENABLE, TIMER_TOGGLEMODE, TIMER_HALTDISABLE);
	timer_WatchDogConfigure(ENABLE, ENABLE);

	watchdog_enabled=1;
	printk("TC3162 hardware watchdog initialized\n");
	return 0;
}

static int tc3162wdog_release(struct inode *inode, struct file *file)
{
	/*
	 *	Shut off the watchdog
	 * 	Lock it in if it's a module and we set nowayout
	 */
	if (nowayout) {
		printk(KERN_CRIT "Watchdog cannot be stopped once started! \n");
	} else {
		/* Stop watchdog timer */
		timer_WatchDogConfigure(DISABLE, DISABLE);

		watchdog_enabled = 0;
		printk("TC3162 hardware watchdog stopped\n");
	}
	return 0;
}

static ssize_t tc3162wdog_write(struct file *file, const char *data, size_t len, loff_t *ppos)
{
	if (len) {
		tc3162wdog_kick();
	}
	return len;
}

static struct file_operations tc3162wdog_fops = {
	owner:		THIS_MODULE,
	write:		tc3162wdog_write,
	open:		tc3162wdog_open,
	release:	tc3162wdog_release,
};

static struct miscdevice tc3162wdog_miscdev = {
	minor:		WATCHDOG_MINOR,
	name:		"watchdog",
	fops:		&tc3162wdog_fops,
};

static int watchdog_reset_read_proc(char *page, char **start, off_t off,
		int count, int *eof, void *data){

	return 0;
}
static int watchdog_reset_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data){
	watchDogReset();	
	return 0;
}

extern int is_nmi_enable(void);
extern void set_nmi_enable(uint8 nmi_enable);
extern void show_stack_nmi();


static int nmi_enable_read_proc(char *page, char **start, off_t off,
		int count, int *eof, void *data){

	int len;

	len = sprintf(page, is_nmi_enable() ? "1\n" : "0\n");
	len -= off;
	*start = page + off;

	if (len > count)
		len = count;
	else
		*eof = 1;

	if (len < 0)
		len = 0;

	return len;
}
static int nmi_enable_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data){
	char val_string[3];
	uint8 enable;

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';

	if(val_string[0] == 'd'){
		show_stack_nmi();
	}
	else{

		enable = simple_strtol(val_string, NULL, 10);
		set_nmi_enable(enable);
	}

	return count;
}

static int timer_interrupt_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	int len, i;

	len = 0;
	for(i = 0; i < 4; i++) {
		len += sprintf(page + len, "CPU%d: timer lose d'%08u\n", i, cpu_timer_loss[i]);
	}

	len -= off;
	*start = page + off;

	if (len > count)
		len = count;
	else
		*eof = 1;

	if (len < 0)
		len = 0;

	return len;
}

static int timer_interrupt_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{	
	char val_string[64], cmd[64] ,subcmd[64];
	uint action;
	int i;

	memset(val_string, 0, (sizeof(val_string)));
	memset(cmd, 0, (sizeof(cmd)));
	memset(subcmd, 0, (sizeof(subcmd)));
	
	if (count > sizeof(val_string) - 1)
		return -EINVAL ;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT ;

	sscanf(val_string, "%s %s %x", cmd, subcmd, &action) ;
	
	if(!strcmp(cmd, "reset")) {
		for(i = 0; i < 4; i++) {
			cpu_timer_loss[i] = 0;
		}
	} else if(!strcmp(cmd, "Usage")) {
		printk("reset\n");
    }

	return count ;
}

static int __init tc3162_watchdog_init(void)
{
	struct proc_dir_entry *watchdog_proc=NULL;
	struct proc_dir_entry *timer_procdir=NULL;
	struct proc_dir_entry *timer_proc=NULL;
	
	watchdog_proc = create_proc_entry("watchdog_reset", 0, NULL);
	watchdog_proc->read_proc = watchdog_reset_read_proc;
	watchdog_proc->write_proc = watchdog_reset_write_proc;
	if (isMT751020 || isMT7505 || isEN751221) {
		watchdog_proc = create_proc_entry("nmi_enable", 0, NULL);
		watchdog_proc->read_proc = nmi_enable_read_proc;
		watchdog_proc->write_proc = nmi_enable_write_proc;

	}

	spin_lock_init(&protect_lock);

	
	misc_register(&tc3162wdog_miscdev);
	if(timer_procdir == NULL) {
		timer_procdir = proc_mkdir("timer", NULL);
		if(timer_procdir != NULL) {
			timer_proc = create_proc_entry("interrupt", 0, timer_procdir);
			if(timer_proc != NULL) {
				timer_proc->read_proc = timer_interrupt_read_proc;
				timer_proc->write_proc = timer_interrupt_write_proc;
			} else {
				printk("\ncreate timer_interrupt proc fail.\n");
			}
		} else {
			printk("\ncreate timer_interrupt proc fail.\n");
		}
	}
	
	printk("TC3162 hardware watchdog module loaded.\n");
	return 0;
}

static void __exit tc3162_watchdog_exit(void)
{
	misc_deregister(&tc3162wdog_miscdev);
}

module_init(tc3162_watchdog_init);
module_exit(tc3162_watchdog_exit);

