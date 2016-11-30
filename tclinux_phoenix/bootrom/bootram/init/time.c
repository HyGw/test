#include <linux/errno.h>
#include <linux/init.h>
#include <linux/kernel_stat.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/random.h>
#include <asm/bitops.h>
#include <asm/bootinfo.h>
#include <asm/irq.h>
#include <asm/mipsregs.h>
#include <asm/system.h>
#include <linux/circ_buf.h>
#include <asm/tc3162.h>
#include <asm/io.h>
#if defined(TCSUPPORT_FWC_ENV)
#include <commands.h>
#endif
#if defined(TCSUPPORT_MULTI_BOOT)
#include "net/mac.h"
#endif
#if defined(TCSUPPORT_MULTI_UPGRADE)
#if !defined(TCSUPPORT_C1_TRUE)
#include <flashhal.h>
#include "net/mac.h"
#endif
#endif

#ifndef SPRAM_IMG
volatile unsigned long Jiffies;
#endif

#if defined(TCSUPPORT_MULTI_BOOT)
#define SYS_WRITE_FINISH_BLK_TIME 		0x4
#define SYS_BLK_TIME				0x10
extern volatile char finishMultiBoot;
extern volatile char startMultiBoot;
#endif

#if defined(TCSUPPORT_MULTI_UPGRADE)
#if !defined(TCSUPPORT_C1_TRUE)
#define SYS_BLK_TIME				0x10
extern volatile char finishMultiUpgrade;
extern int maxMultiLedNum;
#endif
#endif

void 
timer_Configure(
	uint8  timer_no, 
	uint8 timer_enable, 
	uint8 timer_mode, 
	uint8 timer_halt
)
{
    uint32 word,word1;
    word = tc_inl(CR_TIMER_CTL);
    word1 = (timer_enable << timer_no)|(timer_mode << (timer_no + 8))|(timer_halt << (timer_no + 26));
    word |= word1;
    tc_outl(CR_TIMER_CTL, word);    
}

void 
timerSet(
	uint32 timer_no,
	uint32 timerTime, 
	uint32 enable,
	uint32 mode, 
	uint32 halt
)
{   
    uint32 word;
	
    word = 1 * get_SYS_HCLK() * 1000 /2;
	word = word * timerTime;
    timerLdvSet(timer_no,word);
    timerCtlSet(timer_no,enable,mode,halt);
}

#ifdef SPRAM_IMG

void time_polling_init(void)
{
	timerSet(1, TIMERTICKS_10MS, ENABLE, TIMER_TOGGLEMODE, TIMER_HALTDISABLE);
}


void pause_polling(int us)
{
	volatile uint32 timer_now, timer_last;
	volatile uint32 tick_acc;
	uint32 one_tick_unit = 1 * get_SYS_HCLK() * 500;	//1000 / 2;
	volatile uint32 tick_wait = us * (one_tick_unit / 1000); 
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

#else//#ifdef SPRAM_IMG



void timer_interrupt(int irq, void *dev_id, struct pt_regs *regs)
{             
	uint32 word;
#if defined(TCSUPPORT_MULTI_BOOT)
	static int tag = 0;
#endif
#if defined(TCSUPPORT_MULTI_UPGRADE)
#if !defined(TCSUPPORT_C1_TRUE)
	int i = 0;
	static int MultiUpgradeTag = 0;
	/*Get the Multi upgrade*/
	uint8 *multi_upgrade_gpio = (uint8 *)(MULTI_UPGRADE_GPIO);
	uint8 multi_led;
#endif
#endif

	word = tc_inl(CR_TIMER_CTL);
	word &= 0xffc0ffff;
	word |= 0x00020000;
	tc_outl(CR_TIMER_CTL, word);
	Jiffies++;	
	
#if defined(TCSUPPORT_FWC_ENV)
	if(1 == restart_time_flags) {
	     restart_time_count--;
		 if(restart_time_count <= 0) {
			tc_outl(0xbfb00040, 0x80000000);
		 }
	}
#endif

#if defined(TCSUPPORT_MULTI_BOOT)
	/*
		during multi-boot, system led will blink slowly,
		after multi-boot, system led will blink fast
	*/
	if (finishMultiBoot){
		if (Jiffies & SYS_WRITE_FINISH_BLK_TIME){
			if(tag == 0){
				LED_ON(SYSTEM_GPIO);
				tag = 1;
			}
		}
		else{
			if(tag == 1){
				LED_OFF(SYSTEM_GPIO);
				tag = 0;
			}
		}
	}
	else if (startMultiBoot){
		if (Jiffies & SYS_BLK_TIME){
			if(tag == 0){
				LED_ON(SYSTEM_GPIO);
				tag = 1;
			}
		}
		else{
			if(tag == 1){
				LED_OFF(SYSTEM_GPIO);
				tag = 0;
			}
		}
	}
#endif

#if defined(TCSUPPORT_MULTI_UPGRADE)
#if !defined(TCSUPPORT_C1_TRUE)
	/*
		after multi-boot, system led will blink slowly
	*/
	if (finishMultiUpgrade){
		if (Jiffies & SYS_BLK_TIME){
			if(MultiUpgradeTag == 0){
				for(i=0; i<maxMultiLedNum; i++){
//					if(multi_upgrade_gpio[i] != 0)
//						LED_ON(multi_upgrade_gpio[i]);
					multi_led = READ_FLASH_BYTE(multi_upgrade_gpio + i);
					if(multi_led != 0)
					#if defined(TCSUPPORT_CT_WAN_PTM)
                                        {
						if(multi_led > 31)
							regWrite32(CR_GPIO_DATA1,regRead32(CR_GPIO_DATA1)& ~(1<<(multi_led-32)));
						else
							regWrite32(CR_GPIO_DATA,regRead32(CR_GPIO_DATA)& ~(1<<multi_led));
					}
                                        #else
						LED_ON(multi_led);
                                        #endif
				}
				MultiUpgradeTag = 1;
			}
		}
		else{
			if(MultiUpgradeTag == 1){
				for(i=0; i<maxMultiLedNum; i++){
//					if(multi_upgrade_gpio[i] != 0)
//						LED_OFF(multi_upgrade_gpio[i]);
					multi_led = READ_FLASH_BYTE(multi_upgrade_gpio + i);
					if(multi_led != 0)
					#if defined(TCSUPPORT_CT_WAN_PTM)
                                        {
						if(multi_led > 31)
							regWrite32(CR_GPIO_DATA1,regRead32(CR_GPIO_DATA1)|(1<<(multi_led-32)));	
						else
							regWrite32(CR_GPIO_DATA,regRead32(CR_GPIO_DATA)|(1<<multi_led));
                                        }
                                        #else
						LED_OFF(multi_led);
                                        #endif
				}
				MultiUpgradeTag = 0;
			}
		}
	}
#endif
#endif

}

uint32 getTime(void)
{
	return (Jiffies * 10);
}

void pause(int ms)
{
	volatile uint32 TimeBase;
	uint32  TimePassed;

	TimeBase = getTime();
	do {
		TimePassed = getTime();
		if ( TimePassed < TimeBase ) {
			TimePassed += 0xffffffff - TimeBase;
		}
		else {
			TimePassed -= TimeBase;
		}
	}
	while ( TimePassed < ms );
}

struct irqaction time_irq = {timer_interrupt, NULL, 0, "timer", NULL, NULL};
   
void time_init(void)
{
	timerSet(1, TIMERTICKS_10MS, ENABLE, TIMER_TOGGLEMODE, TIMER_HALTDISABLE);
	request_IRQ(TIMER1_INT, &time_irq, NULL);	
}             
#endif//#ifdef SPRAM_IMG


