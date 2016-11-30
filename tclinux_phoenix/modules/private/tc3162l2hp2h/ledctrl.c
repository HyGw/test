/*
** $Id: //BBN_Linux/DEV/main/tclinux_phoenix/modules/private/tc3162l2hp2h/tcledctrl.c#37 $
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
** $Log: tcledctrl.c,v $
** Revision 1.22  2011/10/21 08:58:29  leidong
** Bug #11663:
** WPS led keep blinking though WLan was disabled by WLan button.
**
** Revision 1.21  2011/10/12 07:17:47  shnwind
** RT63365 wifi led
**
** Revision 1.20  2011/10/12 05:29:55  shnwind
** RT63365 wifi led
**
** Revision 1.19  2011/07/29 05:43:00  johnma
** #10911 Add a CI-command to let all of leds on or off.
**
** Revision 1.18  2011/07/24 12:00:14  xyzhu_nj
** #11006: Merge China telecom branch to Main trunk.
**
** Revision 1.17  2011/07/08 10:39:41  xyyou
**  BBU code into WBU trunk
**
** Revision 1.16  2011/07/07 10:28:49  lyxue_sz
** #10324:sys led on/off for BILLION TRUE
**
** Revision 1.15  2011/06/17 06:33:59  lyxue_sz
** #8413, #7047
**
** Revision 1.14  2011/06/15 07:16:36  lino
** add RT65168 support: 32 GPIOs support
**
** Revision 1.13  2011/05/26 08:57:29  lyxue_sz
** #10039
**
** Revision 1.12  2011/05/13 02:07:42  lyxue_sz
** #10039: when customers modify configurations and save flash, power led green flash
**
** Revision 1.11  2011/03/25 08:09:30  gcyin_nj
** #merge SZ branch Dlink to Main trunk
**
** Revision 1.10  2011/03/17 14:05:55  here
** [Ehancement]Accroding the CHT spec to operate the DSL LED. The dsl link down that  the DSL led is off. The dsl link is up that DSL led is on. The DSL TX/RX traffic that the DSL LED is blinking.
**
** Revision 1.9  2011/02/22 12:47:31  xmdai_nj
** #8996:Hardware WPS button have no effect.
**
** Revision 1.8  2011/02/18 05:49:36  xhshi
** #9035
** bicolor mechanism
**
** Revision 1.7  2011/01/13 08:29:39  xyyou
** 5392 driver keep the old wps led to fix branch issue
**
** Revision 1.6  2011/01/11 13:15:46  xyyou
** #8289
**
** Revision 1.5  2011/01/06 15:02:40  pork
** commit TDI layer with Zarlink 2S1O
**
** Revision 1.4  2011/01/06 02:20:36  ybdu_sz
** merge from branch_sz to main bugfree#7026,7028
**
** Revision 1.3  2010/12/31 08:09:12  xflu_sz
** merge from branch sz: bugfree:#7047
**
** Revision 1.2  2010/10/15 05:05:29  xmdai_nj
** #7017:add function for wlan button(main trunk)
**
** Revision 1.1.1.1  2010/09/30 21:14:55  josephxu
** modules/public, private
**
** Revision 1.6  2010/09/03 13:36:01  xyyou
** add new version control

** Revision 1.5.4.3  2010/09/26 07:57:52  tzzhang_nj
** [Enhancement]#7028:WLAN/WPS dual color led.
**
** Revision 1.5.4.2  2010/09/26 07:19:06  tzzhang_nj
** [Enhancement]#7026:WLAN/WPS button for Zyxel request.
**          0s<press time<5s,WLAN active/deactive
**   5s<=press time<10s,WPS enable
**   press time>=15s,Reset to OOB
**
** Revision 1.5  2010/07/22 03:07:18  yzwang_nj
** Support Version control mechanism for tcfullecone.ko, tcsmux.ko, and tcledctrl.ko
**
** Revision 1.4  2010/06/11 00:31:41  xhshi
** #5964 support dare's new led and key requests
**
** Revision 1.3  2010/04/29 11:46:01  xyyou
** bug fix :when the reset btn gpio is 0, and the wps led is not configured, then start wps, the CPE will restart.
**
** Revision 1.2  2010/04/16 08:30:07  yzwang_nj
** Remove the debug message "No Internet LED USED "
**
** Revision 1.1.1.1  2010/04/09 09:35:29  feiyan
** New TC Linux Make Flow Trunk
**
** Revision 1.6  2010/03/24 11:03:52  xyyou
** remove debug msg
**
** Revision 1.5  2010/03/22 14:14:52  yzwang_nj
** [Bug#5128] Implement TR068 LED
**
** Revision 1.4  2010/03/19 13:21:54  xyyou
** # 5213 Add  RT3390 11n Webpage
** Support RT3390 AutoChannel feature
** Support RT3390 WMM feature
** # 5214 Support RT3390 wps feature
**      Add wps led spec&not spec
**
** Revision 1.3  2010/03/19 12:31:49  xyyou
** # 5213 Add  RT3390 11n Webpage
** Support RT3390 AutoChannel feature
** Support RT3390 WMM feature
** # 5214 Support RT3390 wps feature
**      Add wps led spec&not spec
**
** Revision 1.2  2010/03/19 07:06:44  xyyou
** # 5213 Add  RT3390 11n Webpage
** Support RT3390 AutoChannel feature
** Support RT3390 WMM feature
** # 5214 Support RT3390 wps feature
**      Add wps led spec&not spec
**
** Revision 1.1.1.1  2009/12/17 01:48:05  josephxu
** 20091217, from Hinchu ,with VoIP
**
** Revision 1.1.1.1  2007/10/12 08:25:04  ian
** Fix Kernel for CVS crash
**
** Revision 1.3  2007/05/30 15:05:46  ian
** Remove the hard-coded LAN reset in initialization
**
** Revision 1.2  2007/05/11 03:24:17  here
** Support the  led.conf file to confiugre  Switch IC Model.
**
** Revision 1.1.1.1  2007/04/12 09:42:02  ian
** TCLinuxTurnkey2007
**
** Revision 1.3  2006/07/06 07:24:23  lino
** update copyright year
**
** Revision 1.2  2006/07/06 05:54:00  lino
** add kernel module support
**
** Revision 1.1.1.1  2005/11/02 05:45:16  lino
** no message
**
** Revision 1.5  2005/03/04 09:40:23  lino
** add high active gpio support
**
** Revision 1.4  2005/02/14 06:35:35  lino
** add led turn on/off in timer
**
** Revision 1.3  2004/08/09 12:26:22  lino
** add tc3162 support
**
*/

#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/proc_fs.h>
#include <linux/version.h>
#include <asm/addrspace.h>
#include <asm/tc3162/tc3162.h>
#include <asm/tc3162/ledcetrl.h>
#include <linux/slab.h>
#include "../../../version/tcversion.h"

#define KERNEL_2_6_36 		(LINUX_VERSION_CODE > KERNEL_VERSION(2,6,31))
#define KERNEL_3_18_21 		(LINUX_VERSION_CODE >= KERNEL_VERSION(3,18,19))

#define printf	printk
//#define HW_LEN 32
#define HW_LEN 64
/*__________________________________________________________________________
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%      constant definition
%%________________________________________________________________________*/
//#define USE_LED_TASK			1

#define GPIO_MAX_NO				32
#define GPIO_OUTPUT_MAX_NO		(81)
#define GPIO_OUTPUT_ACTIVED		(1)
#define GPIO_OUTPUT_DEACTIVED	(0)
#define GPIO_INPUT_MODE			(0x00)
#define GPIO_OUTPUT_MODE		(0x01)

struct gpio_output_status_s {
	uint8 actived	: 1;
	uint8 ctrl		: 2;
};

struct gpio_output_status_s gpio_output_status[GPIO_OUTPUT_MAX_NO];

/*20090811 pork modified: LED ctrl table expand*/
#if defined(TCSUPPORT_XPON_LED_UPGRADE)
#define LED_MAX_NO				104
#else
#define LED_MAX_NO				103
#endif

#ifdef TCSUPPORT_WLAN_LED_BY_SW
#define LED_SPEED_FASTEST_BNO	1
#endif
#define LED_SPEED_FAST_BNO		2
#define LED_SPEED_MED_BNO		5
#define LED_SPEED_SLOW_BNO		10
#define LED_SPEED_VERY_SLOW_BNO	20
#define LED_SPEED_SLOWLY_BNO	30
#define TC3162_LED_TIMMER_INTERVAL 5
#define TC3262_LED_TIMMER_INTERVAL 10
#ifdef LED_WPSSPEC_COMPLY//please modify the same defined in wsc.c
#define 	WPS_INPROGRESS	1
#define	WPS_ERROR			2
#define	WPS_OVERLAP		3
#define	WPS_SUCCESS		4
#define	WPS_INPUT			0
#ifdef CONFIG_MIPS_TC3262
#define	WPS_TIMEOUT	1200*TC3162_LED_TIMMER_INTERVAL/TC3262_LED_TIMMER_INTERVAL
#else
#define	WPS_TIMEOUT	1200
#endif
extern void SetWPSLedMode(unsigned char LedMode);
#endif
#ifdef TCSUPPORT_USB_HOST_LED
#if defined(TCSUPPORT_CPU_MT7510) || defined(TCSUPPORT_CPU_MT7520) || defined(TCSUPPORT_CPU_MT7505) || defined(TCSUPPORT_CPU_EN7521)
extern void (*Usb_Led_Flash_Op_hook)(unsigned int opmode ,unsigned int phyport);
#if KERNEL_3_18_21
extern int pre_usb_state[2];
#endif
#endif
#endif

#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
extern void (*Simcard_Led_hook)(unsigned int type ,unsigned int status);
#endif

#if defined(TCSUPPORT_CT_BUTTONDETECT)
#define BUTTON_QUEUE_LEN 10
#define WLAN_BUTTON	1
#define	WPS_BUTTON	2
#define CLICK	1
#define DOUBLE_CLICK	2
#define LONG_CLICK	3

#define TIMES_IN_ONE_SECOUND 	10

static struct timer_list button_detect_timer;
static unsigned long lastWlanJiffies = 0;
static unsigned long lastWpsJiffies = 0;
#ifdef TCSUPPORT_WLAN
static int wlan_counters = 0;
#endif
#ifdef WSC_AP_SUPPORT
static int wps_counters = 0;
#endif

static int wlanDelay = 0;
static int wpsDelay = 0;

struct button_event
{
	int type[BUTTON_QUEUE_LEN];///1 :wlan button;  2: wps button
	int data[BUTTON_QUEUE_LEN];///1:click; 2:double click; 3: long click.
	int head, tail;
	spinlock_t lock;
}button_eq;
static void event_enqueue(struct button_event * q,int type,int data);
#endif

#if defined(TCSUPPORT_TEST_LED_ALL)
#if defined(TCSUPPORT_CY) || defined(TCSUPPORT_C7)
#define CR_GPIO_SSR			0xBFB00860
#endif

#if defined(TCSUPPORT_CT_PON) && defined(TCSUPPORT_CPU_EN7521)
#define RG_GPIO_CTRL		0xbfbf0200
#define RG_PON_I2C_MODE		0xbfa20104
#define RG_FORCE_GPIO2_EN	0xbfa20108
#endif

#endif
static int led_wifi(int action, int num, int b);
static int led_wifi_set_mode(int action, int num);
static int get_led_data(int num);
#if defined(TCSUPPORT_WLAN_GPIO)
#if defined(RT5392) || defined(RT5390) || defined(RT5390HL)|| defined(MT7592)
int chkWlanGpioMode(uint8 led_gpio);
EXPORT_SYMBOL(chkWlanGpioMode);
uint8 getDslLedGpio(void);
EXPORT_SYMBOL(getDslLedGpio);
int get_led_mode_data(int led_gpio);
#define WLAN_GPIO_MODE_DATE (1<<30)
#endif
#endif
#define WLAN_GPIO_MAP_OFFSET 64
#define WLAN_LED_OFF 0
#define WLAN_LED_ON  1
#define WLAN_LED_OEN  2
#define WLAN_LED_IEN  3
#define WLAN_LED_RECOVER 4

#define WLAN_GPIO_START 0
#if defined(MT7592)
#define WLAN_GPIO_END	15
#else
#define WLAN_GPIO_END	14
#endif

#define isDEFAULT         0x0
typedef struct led_path
{
    uint8 chipid ;
    const char *path;
}led_path_m;

#if defined(TCSUPPORT_WLAN_GPIO) || (defined(TCSUPPORT_LED_SWITCH_BUTTON)&&defined(TCSUPPORT_WLAN))
#define GPIOCTRL_WIFI 0x228
#ifdef TCSUPPORT_WLAN_LED_BY_SW
void (*WLan_Led_Flash_Op_hook)(unsigned int opmode);
EXPORT_SYMBOL(WLan_Led_Flash_Op_hook);
#endif
int (*hook_wlan_led_action)(int action, int gpio);
EXPORT_SYMBOL(hook_wlan_led_action);

#if defined(TCSUPPORT_LED_SWITCH_BUTTON) && defined(TCSUPPORT_WLAN)
#if defined(TCSUPPORT_WLAN_AC)
int (*hook_wlan_led_action_5g)(int action, int gpio);
EXPORT_SYMBOL(hook_wlan_led_action_5g);
#endif
#endif

unsigned int (*hook_wlan_led_get_data)(int gpio);
EXPORT_SYMBOL(hook_wlan_led_get_data);
void set_wlan_gpio_mode(void);
EXPORT_SYMBOL(set_wlan_gpio_mode);
#define MAX_GPIO_WIFI 17
int gpio_current[MAX_GPIO_WIFI]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
EXPORT_SYMBOL(gpio_current);

#endif

#if defined(TCSUPPORT_FWC_ENV)
int ledConfigMux(int num);
int ledSetModeEnable(int num, uint8 mode);
int ledGetModeEnable(int num,uint8* data);
int ledWriteDate(int num,uint8 data);
int ledReadDate(int num,uint8* data);
EXPORT_SYMBOL(ledConfigMux);
EXPORT_SYMBOL(ledSetModeEnable);
EXPORT_SYMBOL(ledGetModeEnable);
EXPORT_SYMBOL(ledWriteDate);
EXPORT_SYMBOL(ledReadDate);
typedef enum{
LED_CMD_ERRO=-1,
LED_CMD_NOGPIO=0,
LED_CMD_SUCCESS=1	
}LED_CMD_RESULT;
typedef enum{
GPIO_NO_ENABLE=-1,
GPIO_IN_MODE=0,
GPIO_OUT_MODE=1
}GPIO_MODE;
typedef enum{
GPIO_DATA_VL=0,
GPIO_DATA_VH=1
}GPIO_DATA;
typedef struct {
	uint8 gpio;
	uint32 enable;
	uint8 onOff;
} GPIO_IOMUX_CTL_t;
static GPIO_IOMUX_CTL_t EN7512LED[7]={
{3, 1 << 7,  0},
{7,	1 << 6,  0},
{8, 1 << 5,  0},
{9, 1 << 4,  0},
{10,1 << 3,  0},
{31,1 << 21, 1},
{0,0,0},
};
#endif
/*__________________________________________________________________________
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%      macro definition
%%________________________________________________________________________*/
#define LED_MODE(x)		((x) & LED_MODE_MASK) 
#define LED_BICOLOR(x)	((x) & LED_MODE_BICOLOR) 
#define LED_GPIO_MODE(x) (((x) & 0xff00) >> 8)
#define LED_GPIO1(x)	(((x) & 0xf0) >> 4)
#define LED_GPIO2(x)	((x) & 0x0f)
#define LED_GPIO1_NEW(x)	(((x) & 0xff00) >> 8)
#define LED_GPIO2_NEW(x)	((x) & 0xff)
#define LED_GET_GPIO_CTRL(x) ((x>31)?(regRead32(CR_GPIO_CTRL1)&(1<<(x-32))):(regRead32(CR_GPIO_CTRL)&(1<<x)))
#define LED_GET_GPIO_DATA(x) ((x>31)?(regRead32(CR_GPIO_DATA1)&(1<<(x-32))):(regRead32(CR_GPIO_DATA)&(1<<x)))

#define LED_OFF(x, b) 	do {								\
							if(gpio_output_status[x].actived == GPIO_OUTPUT_DEACTIVED) {	\
								DO_LED_OFF(x, b);	\
							}	\
						} while (0)

#define LED_ON(x, b) 	do {								\
							if(gpio_output_status[x].actived == GPIO_OUTPUT_DEACTIVED) {	\
								DO_LED_ON(x, b);	\
							}	\
						} while (0)
#ifdef TCSUPPORT_GPIO_ECM
#define DO_LED_OFF(x, b) do {	if(!(led_wifi(WLAN_LED_OFF, x, b))){							\
							if (isSerialGPIO[LED_GPIO2(x)]||isSerialGPIO[LED_GPIO1(x)]){	\
								uint32 value = regRead32(CR_SGPIO_DATA);			\						
								int count = 0;						\
								if (b){							\
									value |= (1<<(LED_GPIO1(x)));			\
									value |= (1<<(LED_GPIO2(x)));			\
								}else{							\
									value |= (1<<(x));				\
								}							\
								while(regRead32(CR_SGPIO_DATA)&SLED_STATUS_REDAY){		\
									udelay(10);					\
									if(++count >= 0xffff){				\
										break;					\
									}						\
								}							\
								regWrite32(CR_SGPIO_DATA,value);				\
							}else{								\
								if (b) {						\
									if(LED_GPIO_MODE(x)){	\
										uint32 value1 = regRead32(CR_GPIO_DATA);	\
										uint32 value2 = regRead32(CR_GPIO_DATA1);	\
										int flag = 0;\
										if(LED_GPIO1_NEW(x) > 31){\
											value2 |= (1<<(LED_GPIO1_NEW(x)-32));	\
										}else{	\
											value1 |= (1<<(LED_GPIO1_NEW(x)));	\
										}	\
										if(LED_GPIO2_NEW(x) > 31){\
											value2 |= (1<<(LED_GPIO2_NEW(x)-32));	\
											flag = 1;\
										}else{	\
											value1 |= (1<<(LED_GPIO2_NEW(x)));	\
										}	\
										if(flag){	\
											regWrite32(CR_GPIO_DATA1,value2);				\
											regWrite32(CR_GPIO_DATA,value1);				\
										}else{	\
											regWrite32(CR_GPIO_DATA,value1);				\
											regWrite32(CR_GPIO_DATA1,value2);				\
										}	\
									}else{						\	
										uint32 value = regRead32(CR_GPIO_DATA);			\
										value |= (1<<(LED_GPIO1(x)));	\
										value |= (1<<(LED_GPIO2(x)));	\
										regWrite32(CR_GPIO_DATA,value);				\
									}	\
								} else {						\
									uint32 value = 0;			\
									if(x > 31){					\
										value = regRead32(CR_GPIO_DATA1);			\
										value |= (1<<(x-32));		\
										regWrite32(CR_GPIO_DATA1,value);				\
									}else{	\
										value = regRead32(CR_GPIO_DATA);			\
										value |= (1<<(x));		\
										regWrite32(CR_GPIO_DATA,value);				\
									}	\		
								}\
							}								\
							}		\
						} while (0)

#define DO_LED_ON(x, b) do {	if(!(led_wifi(WLAN_LED_ON, x, b))){							\
							if (isSerialGPIO[LED_GPIO2(x)]||isSerialGPIO[LED_GPIO1(x)]){	\
								uint32 value = regRead32(CR_SGPIO_DATA);			\
								int count = 0;						\
								if (b){							\
									value &= ~(1<<(LED_GPIO1(x)));			\
									value |= (1<<(LED_GPIO2(x)));			\
								}else{							\
									value &= ~(1<<(x));				\
								}							\
								while(regRead32(CR_SGPIO_DATA)&SLED_STATUS_REDAY){		\
									udelay(10);					\
									if(++count >= 0xffff){				\
										break;					\
									}						\
								}							\
								regWrite32(CR_SGPIO_DATA,value);				\
							}else{								\
								if (b) {						\
									if(LED_GPIO_MODE(x)){	\
										uint32 value1 = regRead32(CR_GPIO_DATA);	\
										uint32 value2 = regRead32(CR_GPIO_DATA1);	\
										int flag = 0;\
										if(LED_GPIO1_NEW(x) > 31){\
											value2 &= ~ (1<<(LED_GPIO1_NEW(x)-32));	\
										}else{	\
											value1 &= ~ (1<<(LED_GPIO1_NEW(x)));	\
										}	\
										if(LED_GPIO2_NEW(x) > 31){\
											value2 |= (1<<(LED_GPIO2_NEW(x)-32));	\
											flag = 1;\
										}else{	\
											value1 |= (1<<(LED_GPIO2_NEW(x)));	\
										}	\
										if(flag){	\
											regWrite32(CR_GPIO_DATA1,value2);				\
											regWrite32(CR_GPIO_DATA,value1);				\		
										}else{\
											regWrite32(CR_GPIO_DATA,value1);				\
											regWrite32(CR_GPIO_DATA1,value2);				\	
										}\				
									}else{						\	
										uint32 value = regRead32(CR_GPIO_DATA); 		\
										value |= (1<<(LED_GPIO1(x)));	\
										value &= ~ (1<<(LED_GPIO2(x)));	\
										regWrite32(CR_GPIO_DATA,value); 			\
									}	\
								} else {						\
									uint32 value = 0;			\
									if(x > 31){ 				\
										value = regRead32(CR_GPIO_DATA1);			\
										value  &= ~  (1<<(x-32));		\
										regWrite32(CR_GPIO_DATA1,value);				\
									}else{	\
										value = regRead32(CR_GPIO_DATA);			\
										value  &= ~  (1<<(x));		\
										regWrite32(CR_GPIO_DATA,value); 			\
									}	\		
								}\		
							}								\
							}								\
						} while (0)	
#else
#if 0// defined(TCSUPPORT_CT) && !defined(TCSUPPORT_AUTOBENCH)
#define DO_LED_OFF(x, b) 	do {	if(!(led_wifi(WLAN_LED_OFF, x, b))){				\
					if (b) {						\
						VPint(CR_GPIO_DATA) |= (1<<(LED_GPIO1(x)));	\
						VPint(CR_GPIO_DATA) |= (1<<(LED_GPIO2(x)));	\
					} else {						\
						VPint(CR_GPIO_DATA) |= (1<<(x));		\
					}							\
				}								\
			} while (0)
#define DO_LED_ON(x, b) 	do {	if(!(led_wifi(WLAN_LED_ON, x, b))){	\
					if (b) {						\
						VPint(CR_GPIO_DATA) &= ~(1<<(LED_GPIO1(x)));	\
						VPint(CR_GPIO_DATA) |= (1<<(LED_GPIO2(x)));	\
					} else {						\
						VPint(CR_GPIO_DATA) &= ~(1<<(x));		\
					} 							\
				}								\
			} while (0)	
#else
#define DO_LED_OFF(x, b) do {								\
							if(!(led_wifi(WLAN_LED_OFF, x, b))){				\
								if (b) {						\
									if(LED_GPIO_MODE(x)){	\
										uint32 value1 = regRead32(CR_GPIO_DATA);	\
										uint32 value2 = regRead32(CR_GPIO_DATA1);	\
										uint32 flag = 0;	\
										if(LED_GPIO1_NEW(x) > 31){\
											value2 |= (1<<(LED_GPIO1_NEW(x)-32));	\
										}else{	\
											value1 |= (1<<(LED_GPIO1_NEW(x)));	\
										}	\
										if(LED_GPIO2_NEW(x) > 31){\
											value2 |= (1<<(LED_GPIO2_NEW(x)-32));	\
											flag = 1;\
										}else{	\
											value1 |= (1<<(LED_GPIO2_NEW(x)));	\
										}	\
										if(flag){	\
											regWrite32(CR_GPIO_DATA1,value2);				\
											regWrite32(CR_GPIO_DATA,value1);				\
										}else{	\
											regWrite32(CR_GPIO_DATA,value1);				\
											regWrite32(CR_GPIO_DATA1,value2);				\
										}	\
									}else{	\
										regWrite32(CR_GPIO_DATA,regRead32(CR_GPIO_DATA)|(1<<(LED_GPIO1(x))));	\
										regWrite32(CR_GPIO_DATA,regRead32(CR_GPIO_DATA)|(1<<(LED_GPIO2(x))));	\
									}	\
								} else {						\
									if(x > 31){					\
										regWrite32(CR_GPIO_DATA1,regRead32(CR_GPIO_DATA1)|(1<<(x-32)));	\
									}else{						\
										regWrite32(CR_GPIO_DATA,regRead32(CR_GPIO_DATA)|(1<<x));	\
									}						\
								}							\
							}								\
						} while (0)
#define DO_LED_ON(x, b) do {								\
							if(!(led_wifi(WLAN_LED_ON, x, b))){	\
								if (b) {						\
									if(LED_GPIO_MODE(x)){	\
										uint32 value1 = regRead32(CR_GPIO_DATA);	\
										uint32 value2 = regRead32(CR_GPIO_DATA1);	\
										int flag = 0;\
										if(LED_GPIO1_NEW(x) > 31){\
											value2 &= ~ (1<<(LED_GPIO1_NEW(x)-32));	\
										}else{	\
											value1 &= ~ (1<<(LED_GPIO1_NEW(x)));	\
										}	\
										if(LED_GPIO2_NEW(x) > 31){\
											value2 |= (1<<(LED_GPIO2_NEW(x)-32));	\
											flag = 1;\
										}else{	\
											value1 |= (1<<(LED_GPIO2_NEW(x)));	\
										}	\
										if(flag){	\
											regWrite32(CR_GPIO_DATA1,value2);				\
											regWrite32(CR_GPIO_DATA,value1);				\		
										}else{\
											regWrite32(CR_GPIO_DATA,value1);				\
											regWrite32(CR_GPIO_DATA1,value2);				\	
										}\				
									}else{						\	
										regWrite32(CR_GPIO_DATA,regRead32(CR_GPIO_DATA)& ~(1<<(LED_GPIO1(x))));	\
										regWrite32(CR_GPIO_DATA,regRead32(CR_GPIO_DATA)|(1<<(LED_GPIO2(x))));	\
									}	\
								} else {						\
									if(x > 31){					\
										regWrite32(CR_GPIO_DATA1,regRead32(CR_GPIO_DATA1)& ~(1<<(x-32)));	\
									}else{						\
										regWrite32(CR_GPIO_DATA,regRead32(CR_GPIO_DATA)& ~(1<<x));	\
									}						\
								} 							\
							}								\
						} while (0)
#endif
#endif

#if 0// defined(TCSUPPORT_CT) && !defined(TCSUPPORT_AUTOBENCH)
#define LED_OEN(x)		do { 	if(!(led_wifi(WLAN_LED_OEN, x, 0))){	\
						(x > 15) ? (VPint(CR_GPIO_CTRL1) |= (1<<(x-16)*2)) : \
							(VPint(CR_GPIO_CTRL) |= (1<<((x)*2))); VPint(CR_GPIO_ODRAIN) |= (1<<(x));\
					}\
				} while(0)
/* input enable */
#define LED_IEN(x)		do { 	if(!(led_wifi(WLAN_LED_IEN, x, 0))){	\
						(x > 15) ? (VPint(CR_GPIO_CTRL1) &= ~(0x00000003 << ((x-16)* 2)) ): \
							(VPint(CR_GPIO_CTRL) &= ~(0x00000003 << ((x)* 2))); VPint(CR_GPIO_ODRAIN) &= ~(0x00000001 << (x)); \
					}\
				} while(0)
#else
#define LED_OEN(x)		do { 	if(!(led_wifi(WLAN_LED_OEN, x, 0))){	\
									if(x > 31){							\
										if(x > 47){						\
											regWrite32(CR_GPIO_CTRL3,regRead32(CR_GPIO_CTRL3)|(1<<((x-48)*2)));	\
										}else{							\
											regWrite32(CR_GPIO_CTRL2,regRead32(CR_GPIO_CTRL2)|(1<<((x-32)*2)));	\
										}								\
										regWrite32(CR_GPIO_ODRAIN1,regRead32(CR_GPIO_ODRAIN1)|(1<<(x-32)));	\
									}									\
									else{								\
										if(x > 15){						\
											regWrite32(CR_GPIO_CTRL1,regRead32(CR_GPIO_CTRL1)|(1<<((x-16)*2)));	\
										}else{							\
											regWrite32(CR_GPIO_CTRL,regRead32(CR_GPIO_CTRL)|(1<<(x*2)));	\
										}								\
										regWrite32(CR_GPIO_ODRAIN,regRead32(CR_GPIO_ODRAIN)|(1<<(x)));	\
									}									\
								}\
				} while(0)
/* input enable */
#define LED_IEN(x)		do { 	if(!(led_wifi(WLAN_LED_IEN, x, 0))){	\
									if(x > 31){							\
										if(x > 47)						\
											regWrite32(CR_GPIO_CTRL3,regRead32(CR_GPIO_CTRL3)&~(0x00000003 << ((x-48)* 2)));	\
										else							\
											regWrite32(CR_GPIO_CTRL2,regRead32(CR_GPIO_CTRL2)&~(0x00000003 << ((x-32)* 2)));	\
										regWrite32(CR_GPIO_ODRAIN1,regRead32(CR_GPIO_ODRAIN1)&~(0x00000001 << (x-32)));	\
									}									\
									else{								\
										if(x > 15)						\
											regWrite32(CR_GPIO_CTRL1,regRead32(CR_GPIO_CTRL1)&~(0x00000003 << ((x-16)* 2)));	\
										else							\
											regWrite32(CR_GPIO_CTRL,regRead32(CR_GPIO_CTRL)&~(0x00000003 << (x* 2)));	\
										regWrite32(CR_GPIO_ODRAIN,regRead32(CR_GPIO_ODRAIN)&~(0x00000001 << x));	\
									}									\
								}\
				} while(0)

#endif
// debug macro
#if 1
#define LED_DBG(x) 		do { } while (0)
#define LED_DBG_W(x, y)	do { } while (0)
#define LED_DBG_B(x, y)	do { } while (0)
#define LED_DBG_L(x, y)	do { } while (0)
#else
static int led_dbg = 0x01;
#define dbg_pline_1(x)		printk("%s\n", x)
#define dbg_plineb_1(x, y)	printk("%s0x%02x\n", x, (uint8) y)
#define dbg_plinew_1(x, y)	printk("%s0x%04x\n", x, (uint16) y)
#define dbg_plinel_1(x, y)	printk("%s0x%08lx\n", x, (uint32) y)
#define LED_DBG 		if(led_dbg&0x01) dbg_pline_1
#define LED_DBG_W 		if(led_dbg&0x01) dbg_plinew_1
#define LED_DBG_B 		if(led_dbg&0x01) dbg_plineb_1
#define LED_DBG_L 		if(led_dbg&0x01) dbg_plinel_1
#endif
/*______________________________________________________________________________
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%      Type Declaration
%%____________________________________________________________________________*/

typedef struct {
	uint8 gpio;
	uint8 mode;
	uint8 speed;
	uint8 onoff;
} ledctrl_t;

typedef struct {
	uint8 data;
	uint8 onoff;
	uint8 blink_no;
	uint8 def_blink_no;
} ledvar_t;

/*______________________________________________________________________________
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%	data declaration
%%____________________________________________________________________________*/
/*----- global variables -----------------------------------------------*/

/*----- local variables -----------------------------------------------*/

static ledctrl_t ledCtrl[LED_MAX_NO];
static ledvar_t ledVar[LED_MAX_NO];
#ifdef TCSUPPORT_WLAN_LED_BY_SW
static uint8 ledDefBNO[5] = {
	LED_SPEED_FAST_BNO,
	LED_SPEED_MED_BNO,
	LED_SPEED_SLOW_BNO,
	LED_SPEED_VERY_SLOW_BNO,
	LED_SPEED_FASTEST_BNO
};
#else
static uint8 ledDefBNO[4] = {
	LED_SPEED_FAST_BNO,
	LED_SPEED_MED_BNO,
	LED_SPEED_SLOW_BNO,
	LED_SPEED_VERY_SLOW_BNO
};
#endif
static uint8 gpioOnOff[GPIO_MAX_NO];
static uint8 gpioNoAct[GPIO_MAX_NO];
static uint8 led_stop = 0;
static uint8	toggleflag = 1; // 0 = LED On; 1 = LED off
static uint8	CI_blink_flag = 0;

#ifdef TCSUPPORT_GPIO_ECM
	uint8	isSerialGPIO[LED_TYPE_MAX_NO] = {
	LED_PARALLEL, 
	LED_PARALLEL,
	LED_PARALLEL,
	LED_PARALLEL,
	LED_PARALLEL,
	LED_PARALLEL,
	LED_PARALLEL,
	LED_PARALLEL,
	LED_PARALLEL,
	LED_PARALLEL,
	LED_PARALLEL,
	LED_PARALLEL,
	LED_PARALLEL,
	LED_PARALLEL,
	LED_PARALLEL,
	LED_PARALLEL
	}; /* serial=1; parallel=0 */
#endif

static ledctrl_t ledDefCtrl[LED_MAX_NO] = {
/* empty configuration */
{0,       0,       0,       0},//0
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       1,       0,       0},//31
{0,       0,       0,       0},//32
{0,       0,       0,       0},//33
{0,       0,       0,       0},//34
{0,       0,       0,       0},//35
{0,       0,       0,       0},//36
{0,       0,       0,       0},//37
{0,       0,       0,       0},//38
{0,       0,       0,       0},//39
{0,       0,       0,       0},//40
{0,       0,       0,       0},//41
{25,	  LED_MODE_ONOFF,		0,		 1},//42 Tx_disable
{0,       0,       0,       0},//43
{0,       0,       0,       0},//44
{0,       0,       0,       0},//45
{0,       0,       0,       0},//46
{0,       0,       0,       0},//47
{0,       0,       0,       0},//48
{0,       0,       0,       0},//49
{0,       0,       0,       0},//50
{0,       0,       0,       0},//51
{0,       0,       0,       0},//52
{0,       0,       0,       0},//53
{0,       0,       0,       0},//54
{0,       0,       0,       0},//55
{0,       0,       0,       0},//56 SYS Reset
{0,       0,       0,       0},//57
{0,       0,       0,       0},//58
{0,       0,       0,       0},//59
{0,       0,       0,       0},//60
{0,       0,       0,       0},//61
{0,       0,       0,       0},//62
{0,       0,       0,       0},//63

{0,       0,       0,       0},//64
{0, 	  0,	   0,       0},//65
{0,       0,       0,       0},//66
{0,       0,       0,       0},//67
{0,       0,       0,       0},//68
{0,       0,       0,       0},//69
{0,       0,       0,       0},//70
{0, 	  0,	   0,       0},//71
{0,       0,       0,       0},//72
{0, 	  0,	   0,       0},//73
{0, 	  0,	   0,       0},//74
{0, 	  0,	   0,       0},//75
{0,       0,       0,       0},//76
{0, 	  0,	   0,       0},//77
{0,       0,       0,       0},//78
{0,       0,       0,       0},//79
{0,       0,       0,       0},//80
{0,       0,       0,       0},//81
{0,       0,       0,       0},//82
{0, 	  0,	   0,       0},//83
{0,       0,       0,       0},//84
{0, 	  0,	   0,       0},//85
{0, 	  0,	   0,       0},//86
{0, 	  0,	   0,       0},//87
{0,       0,       0,       0},//88
{0, 	  0,	   0,       0},//89
{0,       0,       0,       0},//90
{0,       0,       0,       0},//91
{0,       0,       0,       0},//92
{0,       0,       0,       0},//93
{0,       0,       0,       0},//94
{0, 	  0,	   0,       0},//95
{0,       0,       0,       0},//96
{0, 	  0,	   0,       0},//97
{0, 	  0,	   0,       0},//98
{0, 	  0,	   0,       0},//99
{0, 	  0,	   0,       0},//100
{0, 	  0,	   0,       0},//101
{0, 	  0,	   0,       0},//102
#if defined(TCSUPPORT_XPON_LED_UPGRADE)
{0, 	  0,	   0,		0},//103
#endif
};

static struct timer_list led_timer;
static int ppp_led = 0;
/* detect reset button */
#if 0//modify by xyyou to add input btn timer
static struct timer_list reset_button_timer;
#else
static struct timer_list input_button_timer;
#endif
static int reset_button_stats = 0;
#ifdef TCSUPPORT_WLAN
static int wlan_button_stats = 0;
#endif

#ifdef TCSUPPORT_LED_SWITCH_BUTTON
static int led_switch_button_stats = 0;
static int led_switch_button_pressed = 0;    // indicates if the led switch is pressed(""pressed" means press and release)
#endif


int xpon_los_status = 0;              // indicate if there is optical signal
EXPORT_SYMBOL(xpon_los_status);

#if defined(TCSUPPORT_XPON_LED)
static int led_xpon_status = 0;
EXPORT_SYMBOL(led_xpon_status);
uint8 led_ether_up_status[4] = {0};
EXPORT_SYMBOL(led_ether_up_status);
#endif

#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
static int simcard_interface_status = 0;
static int simcard_config_status = 0;

#endif

#ifdef TCSUPPORT_LEDKEY
static int power_behavior = 0;
#define POWER_LED_OFF 0
#define POWER_LIGHT_RED 1
#define POWER_LIGHT_GREEN 2
#define POWER_RED_FLASH 3
#endif
#if defined(TCSUPPORT_FW_INTERNET_LED)
static int firmware_GR_flag = 0;
static int firmware_flash_times = 0;
#if defined(TCSUPPORT_C9_ROST_LED)
#define FIRMWARE_TIMERS        10 /* 1HZ */
#else
#define FIRMWARE_TIMERS        5
#endif
#define FIRMWARE_BEHAVIOR_YES  8
#define FIRMWARE_BEHAVIOR_NO   9
#endif 
#ifdef WSC_AP_SUPPORT//add by xyyou
static int wps_button_stats = 0;
int wps_button_type=0;
#if defined(RT5392) || defined(MT7601E)|| defined(MT7592)
unsigned short wsc_done = 0;
#ifdef BBUTOWBU
int wscStatus=0;
#define WPSSTART 100
#define WPSSTOP 200
#define WLANSTOP 300
#define	STATUS_WSC_IDLE							1
#define STATUS_WSC_FAIL        			        2		// WSC Process Fail
#define STATUS_WSC_CONFIGURED					34
#define	STATUS_WSC_PBC_SESSION_OVERLAP			0x0109		// AP PBC session overlap
#endif
#else
void LED_IEN_WLAN_WPS(void){
	LED_IEN(ledCtrl[GPIO_WLAN_WPS].gpio);
}
EXPORT_SYMBOL(LED_IEN_WLAN_WPS);
#endif
#endif
#if defined(TCSUPPORT_LED_BTN_CHECK) || defined(TCSUPPORT_TEST_LED_ALL)|| defined(GPIO_VERIFY)  
#if defined(TCSUPPORT_BTN_CHECK)
int gButtonCheck = 0;             //Add this flag to check buttons are disabled or not,1:disable,0:enable.
static int gButtonType = 0;     
#endif
#if defined(TCSUPPORT_LED_CHECK) || defined(TCSUPPORT_TEST_LED_ALL) || defined(GPIO_VERIFY) 
int doLedOn(void);
int doLedOff(void);
int doLedRecover(void);
#define LED_MODE_MASK				0xf
#define LED_MODE(x)		((x) & LED_MODE_MASK) 
#endif
#if defined(TCSUPPORT_BTN_CHECK)
EXPORT_SYMBOL(gButtonCheck);
#endif
#if defined(TCSUPPORT_LED_CHECK) || defined(TCSUPPORT_TEST_LED_ALL) 
EXPORT_SYMBOL(doLedOn);
EXPORT_SYMBOL(doLedOff);
EXPORT_SYMBOL(doLedRecover);
#endif

#endif
#if defined(TCSUPPORT_CT_LONG_RESETBTN) || defined(TCSUPPORT_CY) || defined(TCSUPPORT_C1_ZY)
#ifdef CONFIG_MIPS_TC3262
#define LEDTIMER_ONE_SECOUND TC3262_LED_TIMMER_INTERVAL
#else
#define LEDTIMER_ONE_SECOUND TC3162_LED_TIMMER_INTERVAL
#endif
#ifdef TCSUPPORT_C1_ZY
#define INPUTBTN_TIMER_ONE_SECOUND 4
#else
#define INPUTBTN_TIMER_ONE_SECOUND 2
#endif
#define LEDTIMER_FLASH (10*INPUTBTN_TIMER_ONE_SECOUND)
static int reset_btn_long_stats = 0;
static int reset_btn_long_led_counters = 0;
static int reset_btn_long_led_onff = 0;
void showLedOnOff(void);
#endif




#if defined(TCSUPPORT_WLAN_GPIO) || (defined(TCSUPPORT_LED_SWITCH_BUTTON)&&defined(TCSUPPORT_WLAN))
static int iswifigpio(int num){
	int ret;

	if((num >= (WLAN_GPIO_START + WLAN_GPIO_MAP_OFFSET)) && (num <= (WLAN_GPIO_END + WLAN_GPIO_MAP_OFFSET)))
	{	
		ret = (num - WLAN_GPIO_MAP_OFFSET);
	}	
	else
	{
		ret = -1;		
	}
		
	return ret;
}
void set_wlan_gpio_mode(void){
	int i;

	for (i = 0; i < LED_MAX_NO; i++) {
		
		if ((LED_MODE(ledCtrl[i].mode) != LED_MODE_NOT_USED) 
			&&  (ledCtrl[i].gpio >= WLAN_GPIO_MAP_OFFSET)){
			if (LED_MODE(ledCtrl[i].mode) != LED_MODE_INPUT)
			{
				LED_OEN(ledCtrl[i].gpio);
			}
			else
			{
				LED_IEN(ledCtrl[i].gpio);
			}
		}
	}
}
#endif
static int led_wifi(int action, int num, int b){
#if defined(TCSUPPORT_WLAN_GPIO)

	int wifi_gpio = 0;
	/*if bicolor led, just return */
	if (b) {
		return 0;
	}

	wifi_gpio = iswifigpio(num);
// 	printk("num %d wifi gpio %d\n",num,wifi_gpio);
	if(wifi_gpio != -1){
		if(hook_wlan_led_action != NULL){
			hook_wlan_led_action(action, wifi_gpio);
		}
		return 1;
	}
#endif	
	return 0;
}
#if defined(TCSUPPORT_WLAN_GPIO)
#if defined(RT5392) || defined(RT5390) || defined(RT5390HL)|| defined(MT7592)

/*_____________________________________________________________________________
**      function name: chkWlanGpioMode
**      descriptions:
**			check whether wlan gpio set correct or not
**
**      parameters:
**            None
**
**      global:
**            None
**
**      return:
**             0: set wrong
**             1: set correct
**
**      call:
**   	 None
**
**      revision:
**      1. shelven.lu 2012/05/08
**____________________________________________________________________________
*/
int chkWlanGpioMode(uint8 led_gpio)
{
	if(get_led_mode_data(led_gpio) || get_led_data(led_gpio)){
		return 0;
	}

	return 1;
}
/*_____________________________________________________________________________
**      function name: getDslLedGpio
**      descriptions:
**			get DSL led GPIO value
**
**      parameters:
**            None
**
**      global:
**             ledCtrl
**
**      return:
**             gpio value
**      call:
**   	 None
**
**      revision:
**      1. shelven.lu 2012/05/08
**____________________________________________________________________________
*/
uint8 getDslLedGpio(void)
{
	uint8 ret=0;

	ret = ledCtrl[LED_DSL_STATUS].gpio;

	return ret;
}
/*_____________________________________________________________________________
**      function name: get_led_mode_data
**      descriptions:
**			get led mode value
**
**      parameters:
**            None
**
**      global:
**            None
**
**      return:
**             mode value
**      call:
**   	 None
**
**      revision:
**      1. shelven.lu 2012/05/08
**____________________________________________________________________________
*/

int get_led_mode_data(int led_gpio)
{
	int ret;
	int wifi_gpio = 0;
	int modeflag=WLAN_GPIO_MODE_DATE;
	wifi_gpio = iswifigpio(led_gpio);
	
	if(wifi_gpio != -1){
		if(hook_wlan_led_get_data != NULL){
			ret=hook_wlan_led_get_data(wifi_gpio | modeflag);
		}
		else
		{
			ret = ((1<<wifi_gpio)+8); 
		}
	}
	else
	{
		//ret = regRead32(CR_GPIO_CTRL) & (1<<led_gpio);
		ret = LED_GET_GPIO_CTRL(led_gpio);
	}
	return ret;
}
#endif
#endif
static int get_led_data(int num){
	int ret;
	
	
#if defined(TCSUPPORT_WLAN_GPIO)
	int wifi_gpio = 0;
	
	wifi_gpio = iswifigpio(num);
// 	printk("wifi_gpio %d %d\n",wifi_gpio,num);
	if(wifi_gpio != -1){
		if(hook_wlan_led_get_data != NULL){
			ret = hook_wlan_led_get_data(wifi_gpio);
		}else
		{
			ret = (1<<wifi_gpio); //0 means press. 
		}
	}else
#endif
	{
		//ret = regRead32(CR_GPIO_DATA) & (1<<num);
		ret = LED_GET_GPIO_DATA(num);

	}
	return ret;
}

#if defined(TCSUPPORT_FWC_ENV)
int ledConfigMux(int num)
{
	int i=0;
	uint32 value =0;
	uint32 tmp=0;
	if((num>64) || (num<0))
		return LED_CMD_ERRO;
	for(i=0;EN7512LED[i].gpio!=0;i++)
	{
		if(EN7512LED[i].gpio==num){	
			value = regRead32(0xbfa20104);
			if (EN7512LED[i].onOff)
			{
				value |= EN7512LED[i].enable;
			}
			else
			{
				value &= (~EN7512LED[i].enable);
			}
			regWrite32(0xbfa20104,value);	
		}
	}
	return LED_CMD_SUCCESS;
	//return LED_CMD_NOGPIO;
}
int ledSetModeEnable(int num, uint8 mode)
{
	if((num>64) || (num<0) || (mode < 0) || (mode > 1))
		return LED_CMD_ERRO;
	if( mode ==GPIO_IN_MODE){
		LED_IEN(num);
	}
	else{
		LED_OEN(num);
	}
	return LED_CMD_SUCCESS;
}
int ledGetModeEnable(int num,uint8* data)
{
	uint32 ctlValue=0;
	uint32 modeValue=0;
	if((num>64) || (num<0))
		return LED_CMD_ERRO;
	if(!(led_wifi(WLAN_LED_OEN, num, 0)))
	{	
		if(num > 31){	
			if(num > 47){
				ctlValue=regRead32(CR_GPIO_CTRL3)&(1<<((num-48)*2));	
				}else{							
				ctlValue=regRead32(CR_GPIO_CTRL2)&(1<<((num-32)*2));
				}								
				modeValue=regRead32(CR_GPIO_ODRAIN1)&(1<<(num-32));	
			}
			else{	
			if(num > 15){
				ctlValue=regRead32(CR_GPIO_CTRL1)&(1<<((num-16)*2));	
				}else{							
				ctlValue=regRead32(CR_GPIO_CTRL)&(1<<((num)*2));
				}								
				modeValue=regRead32(CR_GPIO_ODRAIN)&(1<<(num-16));	
			}
	}
	if( modeValue == 0)
		return GPIO_NO_ENABLE;
	if( ctlValue )
		*data = 1;
	else
		*data = 0;
	return LED_CMD_SUCCESS;
}
int ledWriteDate(int num,uint8 data)
{
		if((num>64) || (num<0) || (data < 0) || (data > 1))
			return LED_CMD_ERRO;
		if(data == 1)
		{
			if(num > 31){
				regWrite32(CR_GPIO_DATA1,regRead32(CR_GPIO_DATA1)|(1<<(num-32)));
			}else{
				regWrite32(CR_GPIO_DATA,regRead32(CR_GPIO_DATA)|(1<<num));
			}
		}	
		else
		{
			if(num > 31){
				regWrite32(CR_GPIO_DATA1,regRead32(CR_GPIO_DATA1)& ~(1<<(num-32)));
			}else{
				regWrite32(CR_GPIO_DATA,regRead32(CR_GPIO_DATA)& ~(1<<num));
			}
		}
		return LED_CMD_SUCCESS;
}
int ledReadDate(int num,uint8* data)
{
	uint32 temp=0;
	if((num>64) || (num<0))
		return LED_CMD_ERRO;
	if(num > 31){
		temp = regRead32(CR_GPIO_DATA1)&(1<<(num-32));
	}else{
		temp = regRead32(CR_GPIO_DATA)&(1<<num);
	}
	if(temp)
		*data=1;
	else
		*data=0;
	return LED_CMD_SUCCESS;
}
#endif
#ifdef TR068_LED
/* internet_led_on =0, internet_trying_led_on=0 : ADSL not up or No default router or All pvcs are bridged mode. Internet LED off  
*   internet_led_on =1, internet_trying_led_on=0 : IP Connected. Internet LED Solid Green
*   internet_led_on =0, internet_trying_led_on=1 : ADSL UP and trying to get WAN IP. Internet LED Solid Red
*/
int internet_led_on = 0;	/* 1:IP Connected; 0:No IP Connected*/ 
int internet_trying_led_on = 0;  /* 1:adsl up and trying to get WAN IP; 0:adsl not up or can't found defaule router*/
#if defined(TCSUPPORT_CPU_MT7510) ||  defined(TCSUPPORT_CPU_MT7520) || defined(TCSUPPORT_CPU_MT7505)
int internet_hwnat_pktnum = 8;
int internet_hwnat_timer_switch = 1;
#endif
EXPORT_SYMBOL(internet_led_on);
EXPORT_SYMBOL(internet_trying_led_on);
#if defined(TCSUPPORT_CPU_MT7510) ||  defined(TCSUPPORT_CPU_MT7520) || defined(TCSUPPORT_CPU_MT7505)
EXPORT_SYMBOL(internet_hwnat_pktnum);
EXPORT_SYMBOL(internet_hwnat_timer_switch);
#endif
#endif
#if defined(TCSUPPORT_ETHER_ALL_LED)
uint8 ether_lan_speed = 0;
EXPORT_SYMBOL(ether_lan_speed);
#endif
/*__________________________________________________________________________
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%      function declaration
%%________________________________________________________________________*/

//static void ledTimer(void);
static void ledChk(void);
static void ledDump(void);
static void LED_Blink(void);
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
void simcardLedChk(void);
#endif
#ifdef GPIO_VERIFY
extern void ledVerifyInit();
extern void ledVerifyUnInit();
uint8 verify_get_led_stop();
void verify_set_led_stop(uint8 flag);
void verify_set_led_blink(uint8 flag);
void verify_set_led_paral_or_serial(uint16 gpio_num,uint8 value);
void verify_led_dump(void);


uint8 verify_get_led_stop()
{
	return led_stop;
}

void verify_set_led_stop(uint8 flag)
{
	led_stop = flag;
	return;
}

void verify_set_led_blink(uint8 flag)
{
	CI_blink_flag = flag;
	return;
}

void verify_set_led_paral_or_serial(uint16 gpio_num,uint8 value)
{
	if(gpio_num >= LED_TYPE_MAX_NO)
		return;
	isSerialGPIO[gpio_num] = value;
	return;
}

void verify_led_dump(void)
{
	ledDump();
}
#endif

#ifdef LED_WPSSPEC_COMPLY
uint8 WPSLedMode = 0;
uint16 WPSLEDTimeCnt = 0;


void SetWPSLedMode(unsigned	char LedMode)
{
	static uint8 lastLedMode = 0;
	WPSLedMode = LedMode;
	if((LedMode != lastLedMode) || (LedMode !=  WPS_ERROR)){
		WPSLEDTimeCnt = 0;
	}
		
	if(LedMode != WPS_SUCCESS){
#if !defined(TCSUPPORT_CT)
		ledSetMode(GPIO_WLAN_WPS, LED_MODE_BLINK);
		if(ledGetMode(GPIO_WLAN_WPS)!=LED_MODE_NOT_USED)
			LED_OEN(ledCtrl[GPIO_WLAN_WPS].gpio);
#endif
		ledTurnOn(GPIO_WLAN_WPS);
//#ifdef WLAN_RT2561_SUPPORT//ALPHA
		ledTurnOn(LED_WLAN_WPS_ACT_STATUS);
//#endif
	}else{
#if !defined(TCSUPPORT_CT)			
		ledSetMode(GPIO_WLAN_WPS, LED_MODE_ONOFF);
		if(ledGetMode(GPIO_WLAN_WPS)!=LED_MODE_NOT_USED)
			LED_OEN(ledCtrl[GPIO_WLAN_WPS].gpio);
#endif
		
		ledTurnOn(GPIO_WLAN_WPS);
#if 0//def RT2880_SUPPORT
		ClearwpsLedOn();
		//ALPHA
		ledTurnOff(LED_WLAN_WPS_ACT_STATUS);
		ledTurnOn(LED_WLAN_WPS_STATUS);
		//ALPHA
#endif
		//do turn to onoff mode
	}
	 lastLedMode = LedMode;
}

void WPSLedBlink(uint8 i)
{
	uint8 OnOffTemp = 0;
	static uint8 WPSInProgressLedCtl = 1;
	static uint8 WPS5timesCnt = 0;
	static uint8 WPS5timesOK = 0;
	static uint8 WPS5timesCnt2 = 0;

	switch(WPSLedMode){
		
		case WPS_INPROGRESS:
			
			OnOffTemp = gpioOnOff[LED_GPIO2(ledCtrl[i].gpio)] ;
			gpioOnOff[LED_GPIO2(ledCtrl[i].gpio)] ^= 1;
			if(gpioOnOff[LED_GPIO2(ledCtrl[i].gpio)]){//xyyou_0902
				LED_ON(ledCtrl[i].gpio, LED_BICOLOR(ledCtrl[i].mode));
				LED_DBG(" on");
			} else {				
				if(WPSInProgressLedCtl != 0){
					WPSInProgressLedCtl--;
					gpioOnOff[LED_GPIO2(ledCtrl[i].gpio)] = OnOffTemp;
				}else{
					LED_OFF(ledCtrl[i].gpio, LED_BICOLOR(ledCtrl[i].mode));
					LED_DBG(" off");
					WPSInProgressLedCtl = 1;
				}				
			} 
			gpioNoAct[LED_GPIO2(ledCtrl[i].gpio)] = 0;
				
			break;
			
		case WPS_ERROR:
			
			if(WPSLEDTimeCnt < WPS_TIMEOUT){
				WPSLEDTimeCnt ++;
			}
			else{
				WPSLEDTimeCnt = 0;
#ifdef MT7601E
				ledTurnOff(LED_WLAN_WPS_STATUS);
				ledTurnOff(LED_WLAN_WPS_ACT_STATUS);
				ledTurnOff(GPIO_WLAN_WPS); //turn off wps led when error timeout, then set to input mode.
#endif
				if(ledGetMode(GPIO_WLAN_WPS)!=LED_MODE_NOT_USED)
					LED_IEN(ledCtrl[GPIO_WLAN_WPS].gpio);
				ledSetMode(GPIO_WLAN_WPS, LED_MODE_INPUT);	
			#if 0//def RT2880_SUPPORT
				rt2880ClearWPSPending();
			#endif//ALPHA
#ifndef MT7601E
				ledTurnOff(LED_WLAN_WPS_STATUS);
				ledTurnOff(LED_WLAN_WPS_ACT_STATUS);
#endif
				break;	
			//#endif//ALPHA
				//do turn off led!!	
			}
			gpioOnOff[LED_GPIO2(ledCtrl[i].gpio)] ^= 1;
			if(gpioOnOff[LED_GPIO2(ledCtrl[i].gpio)]){//xyyou_0902
				LED_ON(ledCtrl[i].gpio, LED_BICOLOR(ledCtrl[i].mode));
				LED_DBG(" on");
			} else {
				LED_OFF(ledCtrl[i].gpio, LED_BICOLOR(ledCtrl[i].mode));
				LED_DBG(" off");
			} 
			gpioNoAct[LED_GPIO2(ledCtrl[i].gpio)] = 0;
			break;
			
		case WPS_OVERLAP:
			
			if(WPSLEDTimeCnt < WPS_TIMEOUT){
				WPSLEDTimeCnt ++;
			}
			else{
				WPSLEDTimeCnt = 0;
#ifdef MT7601E
				ledTurnOff(LED_WLAN_WPS_STATUS);
				ledTurnOff(LED_WLAN_WPS_ACT_STATUS);
				ledTurnOff(GPIO_WLAN_WPS);
#endif
				if(ledGetMode(GPIO_WLAN_WPS)!=LED_MODE_NOT_USED)
					LED_IEN(ledCtrl[GPIO_WLAN_WPS].gpio);
				ledSetMode(GPIO_WLAN_WPS, LED_MODE_INPUT);			
				//do turn off led!!	
			//#ifdef WLAN_RT2561_SUPPORT
			#if 1//ALPHA
#ifndef MT7601E
				ledTurnOff(LED_WLAN_WPS_STATUS);
				ledTurnOff(LED_WLAN_WPS_ACT_STATUS);
#endif
				break;
			#endif
			}
			if(WPS5timesOK == 0){
				WPS5timesCnt++;
			}
			if(WPS5timesCnt > 10){
				WPS5timesCnt = 0;
				WPS5timesOK = 1;
				WPS5timesCnt2 = 5;
			}

			OnOffTemp = gpioOnOff[LED_GPIO2(ledCtrl[i].gpio)] ;
			gpioOnOff[LED_GPIO2(ledCtrl[i].gpio)] ^= 1;
			if(gpioOnOff[LED_GPIO2(ledCtrl[i].gpio)]){//xyyou_0902
				if(WPS5timesOK){
					if(WPS5timesCnt2){
						WPS5timesCnt2--; 
						gpioOnOff[LED_GPIO2(ledCtrl[i].gpio)] = OnOffTemp;
					}else{
						LED_ON(ledCtrl[i].gpio, LED_BICOLOR(ledCtrl[i].mode));
						LED_DBG(" on");
						WPS5timesOK = 0;
						WPS5timesCnt = 0;
					}
				}else{
					LED_ON(ledCtrl[i].gpio, LED_BICOLOR(ledCtrl[i].mode));
					LED_DBG(" on");
				}
			} else {
				LED_OFF(ledCtrl[i].gpio, LED_BICOLOR(ledCtrl[i].mode));				
				LED_DBG(" off");
			} 
			gpioNoAct[LED_GPIO2(ledCtrl[i].gpio)] = 0;
			
			break;
			
		case WPS_SUCCESS:
			// we do led mode switch in setWPSLedMode();
			break;
		default:
			break;
	}
}
#endif
#ifdef TCSUPPORT_LEDKEY
#endif
#if defined(TCSUPPORT_FW_INTERNET_LED)
void FirmwareLedBlink(void)
{
	static int flag = 0;//0:Firmware led on,1:Firmware led off
	if (flag) {
		if ((LED_MODE(ledCtrl[LED_INTERNET_ACT_STATUS].mode) != LED_MODE_NOT_USED)){
			LED_ON(ledCtrl[LED_INTERNET_ACT_STATUS].gpio,LED_BICOLOR(ledCtrl[LED_INTERNET_ACT_STATUS].mode));
		}
		flag=0;
	}
	else {
		if (LED_MODE(ledCtrl[LED_INTERNET_ACT_STATUS].mode) != LED_MODE_NOT_USED){
#if defined(TCSUPPORT_C9_ROST_LED)
			LED_OFF(ledCtrl[LED_INTERNET_ACT_STATUS].gpio,LED_BICOLOR(ledCtrl[LED_INTERNET_ACT_STATUS].mode));
#else
			LED_ON(ledCtrl[LED_INTERNET_TRYING_STATUS].gpio,LED_BICOLOR(ledCtrl[LED_INTERNET_TRYING_STATUS].mode));
#endif
		}
		flag=1;
	}
}
#endif
/*______________________________________________________________________________
**	ledInit
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
void ledInit(void)
{
	uint8  i;
	uint32 word;

#ifndef CONFIG_MIPS_TC3262
	/* LED USB, XDSLLINK, XDSLACT, SYS, ROM_WRPT: output pin */
	regWrite32(CR_GPIO_CTRL,(1<<(5*2))|(1<<(3*2))|(1<<(2*2))|(1<<(1*2))|(1<<(0*2)));
	regWrite32(CR_GPIO_ODRAIN, (1<<5)|(1<<3)|(1<<2)|(1<<1)|(1<<0));

	// set gpio6 as input pin; gpio6 is reset button
	word = regRead32(CR_GPIO_CTRL);
	word &= 0xffffcfff;
	regWrite32(CR_GPIO_CTRL,word);

#endif
#ifdef TCSUPPORT_CPU_MT7520
	// set bfb00860 bit14=1, single led mode.
	word = regRead32(CR_GPIO_SHR);
	word |= (1<<14);
	
#ifdef TCSUPPORT_XPON_LED	
	// enable software control ether led.
	if (isMT7525 || isMT7520 || isMT7520S
	) 
		word &= ~(0x1F<<9);
#endif


#if defined(TCSUPPORT_CT_PON_CY)
	word &= ~(0x01<<28);
#endif

	regWrite32(CR_GPIO_SHR,word);
#endif
	for (i = 0; i < GPIO_MAX_NO; i++) {
		gpioOnOff[i] = 0;
		gpioNoAct[i] = 0;
	}

	for (i = 0; i < LED_MAX_NO; i++) {
		ledCtrl[i].mode = LED_MODE_NOT_USED;

		ledVar[i].data = 0;
		ledVar[i].onoff = 0;
		ledVar[i].blink_no = 0;
		ledVar[i].def_blink_no = 0;
	}

	memcpy(ledCtrl, ledDefCtrl, sizeof(ledctrl_t) * LED_MAX_NO);

	for (i = 0; i < LED_MAX_NO; i++) {
#if 0 //krammer change for input type
		if (ledCtrl[i].mode != LED_MODE_NOT_USED)
			LED_OEN(ledCtrl[i].gpio);
#else
		if (LED_MODE(ledCtrl[i].mode) != LED_MODE_NOT_USED) {
			if (LED_MODE(ledCtrl[i].mode) != LED_MODE_INPUT)
			{		
				if (LED_BICOLOR(ledCtrl[i].mode)){
					LED_OEN(LED_GPIO1(ledCtrl[i].gpio));
					LED_OEN(LED_GPIO2(ledCtrl[i].gpio));
				}else{
					LED_OEN(ledCtrl[i].gpio);
				}
			}
			else
			{
				LED_IEN(ledCtrl[i].gpio);
			}
		}
#endif
	}

	//regWrite32(CR_GPIO_DATA,regRead32(CR_GPIO_DATA)|((1<<3)|(1<<2)|(1<<1)|(1<<0)));
	for (i = 0; i < LED_MAX_NO; i++) {
#if KERNEL_2_6_36	
		if ((LED_MODE(ledCtrl[i].mode) == LED_MODE_ONOFF) && ((i != LED_LAN_RESET) || (i != LED_PWR_USB)))
#else
		if ((LED_MODE(ledCtrl[i].mode) == LED_MODE_ONOFF) && (i != LED_LAN_RESET))
#endif		
			ledTurnOff(i);
	}
#ifdef TR068_LED
	ledTurnOn(LED_TR68_PWR_BOOTED);
#endif
#if defined(TCSUPPORT_C9_ROST_LED)
	ledTurnOff(LED_INTERNET_NOACT_STATUS);
	ledTurnOff(LED_INTERNET_STATUS);
	ledTurnOff(LED_INTERNET_ACT_STATUS);	
	ledTurnOn(LED_INTERNET_TRYING_STATUS);

	ledTurnOff(LED_XPON_STATUS);
	ledTurnOff(LED_XPON_TRYING_STATUS);
	ledTurnOn(LED_XPON_UNREG_STATUS);
#endif

}

static ledctrl_t sysBootLedCtrl;
#if 0//not used
/*______________________________________________________________________________
**	ledSysInitOn
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
void ledSysInitOn(void)
{
	uint8  i;

	if (ledCtrl[LED_SYS_INIT_STATUS].mode == LED_MODE_NOT_USED)
		return;

	sysBootLedCtrl = ledCtrl[LED_SYS_BOOT_STATUS];
	ledCtrl[LED_SYS_BOOT_STATUS].mode = LED_MODE_NOT_USED;

	for (i = 0; i < LED_MAX_NO; i++) {
		if ((((ledCtrl[i].mode != LED_MODE_NOT_USED) && (ledCtrl[i].mode != LED_MODE_NOACT))
				|| (i == LED_SYS_BOOT_STATUS)) && (i != LED_LAN_RESET)) {
			if (ledCtrl[i].onoff)
				LED_OFF(ledCtrl[i].gpio);
			else
				LED_ON(ledCtrl[i].gpio);
			//LED_ON(ledCtrl[i].gpio);
		}
	}
	mdelay(500);
}


/*______________________________________________________________________________
**	ledSysInitOff
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
void ledSysInitOff(void)
{
	uint8  i;

	if (ledCtrl[LED_SYS_INIT_STATUS].mode == LED_MODE_NOT_USED)
		return;

	for (i = 0; i < LED_MAX_NO; i++) {
		if (((ledCtrl[i].mode != LED_MODE_NOT_USED) && (ledCtrl[i].mode != LED_MODE_NOACT))
				&& (i != LED_LAN_RESET)) {
			if (ledCtrl[i].onoff)
				LED_ON(ledCtrl[i].gpio);
			else
				LED_OFF(ledCtrl[i].gpio);
			//LED_OFF(ledCtrl[i].gpio);
		}
	}
	ledCtrl[LED_SYS_BOOT_STATUS] = sysBootLedCtrl;
	//mdelay(500);
}
#endif

/*______________________________________________________________________________
**	ledTurnOn
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
__IMEM void ledTurnOn(uint8 led_no)
{
	if (led_stop) {
		if (led_no != LED_PHY_VCC_DISABLE && led_no != LED_PHY_TX_POWER_DISABLE)
		return;
	}	

	switch(LED_MODE(ledCtrl[led_no].mode)) {
		case LED_MODE_NOT_USED:
			return;
		case LED_MODE_ONOFF:
			LED_DBG_B("\r\n on led_no=", led_no);
			LED_DBG_B("\r\n on led onoff bit=", ledCtrl[led_no].onoff);
			LED_DBG_B(" gpio=", ledCtrl[led_no].gpio);
			if (led_no == LED_SYS_STATUS) {					/* special case for sys led */
				if (ledCtrl[led_no].onoff) {
					LED_ON(ledCtrl[led_no].gpio, LED_BICOLOR(ledCtrl[led_no].mode));
					gpioOnOff[LED_GPIO2(ledCtrl[led_no].gpio)] = 1;
				} else {
					LED_OFF(ledCtrl[led_no].gpio,LED_BICOLOR(ledCtrl[led_no].mode));
					gpioOnOff[LED_GPIO2(ledCtrl[led_no].gpio)] = 0;
				}
			} else {
				if (ledCtrl[led_no].onoff) {
					LED_OFF(ledCtrl[led_no].gpio,LED_BICOLOR(ledCtrl[led_no].mode));
					gpioOnOff[LED_GPIO2(ledCtrl[led_no].gpio)] = 0;
				} else {
					LED_ON(ledCtrl[led_no].gpio, LED_BICOLOR(ledCtrl[led_no].mode));
					gpioOnOff[LED_GPIO2(ledCtrl[led_no].gpio)] = 1;
				}
			}
			break;
		case LED_MODE_BLINK:
			ledVar[led_no].data = 1;
#if defined(TCSUPPORT_CT) && defined(LED_WPSSPEC_COMPLY)
			if(led_no == LED_WLAN_WPS_ACT_STATUS)
				ledVar[led_no].def_blink_no = 1; //set WPS blink speed for CT
			else
#endif
				ledVar[led_no].def_blink_no = ledDefBNO[ledCtrl[led_no].speed];
			break;
		case LED_MODE_NOACT:
			ledVar[led_no].data = 1;
			//edVar[led_no].def_blink_no = ledDefBNO[LED_SPEED_VERY_SLOW];
			ledVar[led_no].def_blink_no = LED_SPEED_SLOWLY_BNO;
			break;
		case LED_MODE_PULSE:
			ledVar[led_no].data = 1;
			ledVar[led_no].onoff = 1;
			ledVar[led_no].def_blink_no = ledDefBNO[LED_SPEED_SLOW];
			break;
	}
}

/*______________________________________________________________________________
**	ledTurnOff
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
void ledTurnOff(uint8 led_no)
{
	if (led_stop) {
		if (led_no != LED_PHY_VCC_DISABLE && led_no != LED_PHY_TX_POWER_DISABLE)
		return;
	}

	switch(LED_MODE(ledCtrl[led_no].mode)) {
		case LED_MODE_NOT_USED:
			return;
		case LED_MODE_ONOFF:
			LED_DBG_B("\r\n off led_no=", led_no);
			LED_DBG_B("\r\n off led onoff bit=", ledCtrl[led_no].onoff);
			LED_DBG_B(" gpio=", ledCtrl[led_no].gpio);
			if (ledCtrl[led_no].onoff) {
				LED_ON(ledCtrl[led_no].gpio, LED_BICOLOR(ledCtrl[led_no].mode));
				gpioOnOff[LED_GPIO2(ledCtrl[led_no].gpio)] = 1;
			} else {
				LED_OFF(ledCtrl[led_no].gpio,LED_BICOLOR(ledCtrl[led_no].mode));
				gpioOnOff[LED_GPIO2(ledCtrl[led_no].gpio)] = 0;
			}
			break;
		case LED_MODE_BLINK:
			LED_DBG_B("\r\n blink off led_no=", led_no);
			LED_DBG_B(" gpio=", ledCtrl[led_no].gpio);
			ledVar[led_no].data = 0;
			if (ledCtrl[led_no].onoff) {
				LED_ON(ledCtrl[led_no].gpio, LED_BICOLOR(ledCtrl[led_no].mode));
				gpioOnOff[LED_GPIO2(ledCtrl[led_no].gpio)] = 1;
			} else {
				LED_OFF(ledCtrl[led_no].gpio,LED_BICOLOR(ledCtrl[led_no].mode));
				gpioOnOff[LED_GPIO2(ledCtrl[led_no].gpio)] = 0;
			}
			break;
		case LED_MODE_NOACT:
			ledVar[led_no].data = 0;
 			break;
   		case LED_MODE_PULSE:
			ledVar[led_no].data = 0;
			LED_OFF(ledCtrl[led_no].gpio, LED_BICOLOR(ledCtrl[led_no].mode));
			ledVar[led_no].onoff = 0;
			break;
	}
}

/*______________________________________________________________________________
**	ledGetMode
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
uint8 ledGetMode(uint8 led_no)
{
	return LED_MODE(ledCtrl[led_no].mode);
}

void ledSetMode(uint8 led_no, uint8 mode)
{
	if(ledGetMode(led_no) != LED_MODE_NOT_USED){
		ledCtrl[led_no].mode = mode;
		ledDefCtrl[led_no].mode = mode;
	}
}
/*______________________________________________________________________________
**	getSysResetGpio
**
**	descriptions: pork
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
uint8 getSysResetGpio(void)
{
	//if (ledGetMode(ledCtrl[GPIO_SYS_RESET].mode) == LED_MODE_INPUT)
	if (LED_MODE(ledCtrl[GPIO_SYS_RESET].mode) == LED_MODE_INPUT)
		return ledCtrl[GPIO_SYS_RESET].gpio;
	else
		return 6;
}

uint8 ledGetGpio(uint8 led_no)
{
	return ledCtrl[led_no].gpio;
}

void led_oen(uint8 led_no){
	LED_OEN(led_no);
}
void led_ien(uint8 led_no){
	LED_IEN(led_no);
}

#if defined(RT5392) || defined(MT7601E)|| defined(MT7592)
#ifdef WSC_AP_SUPPORT//add by xyyou
int wscTimerRunning=0;
int wpsLedChk(void){
	if(wscTimerRunning){
		#ifndef LED_WPSSPEC_COMPLY
		//#ifdef ZYXEL_LEH//xyyou_0903
		#if 0//defined (ZYXEL_LEH) && defined(WLAN_RT2561_SUPPORT)
			wps_led = 1;
			//ledTurnOff(LED_WLAN_STATUS);//please do not open it here!!!xyyou_0911
			//ledTurnOff(LED_WLAN_ACT_STATUS);//please do not open it here!!!xyyou_0911
			ledTurnOff(LED_WLAN_NOACT_STATUS);//to avoid Wifi led turned on by NOACT,xyyou_0903
		#endif
			ledTurnOn(LED_WLAN_WPS_ACT_STATUS);
			ledTurnOn(GPIO_WLAN_WPS);
		//	printk("xyyou: in tcledctrl.c turnon 44+58\n");
		#endif

	}else if(wsc_done){
    			wsc_done--;	
#ifdef BBUTOWBU
			ledTurnOff(LED_WLAN_WPS_ACT_STATUS);//xyyou???
			ledTurnOn(LED_WLAN_WPS_STATUS);
#endif
		#ifndef LED_WPSSPEC_COMPLY
#if !defined(TCSUPPORT_CT)		
			ledSetMode(GPIO_WLAN_WPS, LED_MODE_ONOFF);
#endif
			ledTurnOn(GPIO_WLAN_WPS);
		#endif
	}else{
		#ifdef LED_WPSSPEC_COMPLY
#if !defined(TCSUPPORT_CT)
			if(ledGetMode(GPIO_WLAN_WPS) == LED_MODE_ONOFF)
#endif
		#endif
			{
			//#ifdef ZYXEL_LEH
			#if 0//defined (ZYXEL_LEH) && defined(WLAN_RT2561_SUPPORT)
				if(wps_led){//xyyou_0903
			#endif
				ledTurnOff(LED_WLAN_WPS_STATUS);
				ledTurnOff(LED_WLAN_WPS_ACT_STATUS);
					//ledTurnOff(LED_WLAN_WPS_NOACT_STATUS);
				ledTurnOff(GPIO_WLAN_WPS);//Support rt2561 wps led blink--duoduo 2008/11/11
#if defined(TCSUPPORT_CT)
				if(ledGetMode(GPIO_WLAN_WPS)!=LED_MODE_NOT_USED)
					LED_IEN(ledCtrl[GPIO_WLAN_WPS].gpio);
#else
				LED_IEN(ledCtrl[GPIO_WLAN_WPS].gpio);
#endif
				ledSetMode(GPIO_WLAN_WPS, LED_MODE_INPUT);
			//#ifdef ZYXEL_LEH
			#if 0//defined (ZYXEL_LEH) && defined(WLAN_RT2561_SUPPORT)		
					wps_led = 0;
				}
			#endif
				
			}
		}
			
	}
#ifdef BBUTOWBU
#ifdef  LED_WPSSPEC_COMPLY 
//int wscStatus=0;
int statusprobe = 0;
void wpsLedChkSpec(void){		
	static int laststatus=0;
	switch(wscStatus){
			/*	case STATUS_WSC_NOTUSED:
				case STATUS_WSC_IDLE:
					dbg_pline_1("\r\ninput mode in ioctl\r\n");
					RaCfgObj.WPSPending = 0;
					LED_IN(ledGetGpio(GPIO_WLAN_WPS));
					ledSetMode(GPIO_WLAN_WPS, LED_MODE_INPUT);
					break;
			*/
				case STATUS_WSC_PBC_SESSION_OVERLAP:

					SetWPSLedMode(WPS_OVERLAP);
					statusprobe = 0;
					break;
				//case STATUS_WSC_NOTUSED:
				case STATUS_WSC_IDLE:
					if((wscStatus==laststatus)||(laststatus==0))
						break;
				case STATUS_WSC_FAIL:
					SetWPSLedMode(WPS_ERROR);
					statusprobe = 0;
					break;
			/*
				case STATUS_WSC_PBC_TOO_MANY_AP:
				case STATUS_WSC_PBC_NO_AP:
				case STATUS_WSC_EAP_FAIL_RECEIVED:
				case STATUS_WSC_EAP_NONCE_MISMATCH:
				case STATUS_WSC_EAP_INVALID_DATA:
				case STATUS_WSC_PASSWORD_MISMATCH:
				case STATUS_WSC_EAP_REQ_WRONG_SMI:
				case STATUS_WSC_EAP_REQ_WRONG_VENDOR_TYPE:
					//SetWPSLedMode(WPS_ERROR);
					break;
			*/
				case STATUS_WSC_CONFIGURED:
				/* add by xyzhu, when restart wps, the wsc status may configured, so the wps led may not blink. */
#if defined(TCSUPPORT_CT)
					if (laststatus == wscStatus) 
						break;
#endif

					SetWPSLedMode(WPS_SUCCESS);
					statusprobe = 0;
					break;
				default:

					SetWPSLedMode(WPS_INPROGRESS);
					break;
		}
		laststatus=wscStatus;
}
#endif
#endif
#endif

#endif

/*______________________________________________________________________________
**	ledTimer
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
#ifdef TCSUPPORT_USB_HOST_LED
#if defined(TCSUPPORT_CPU_MT7510) || defined(TCSUPPORT_CPU_MT7520) || defined(TCSUPPORT_CPU_MT7505) || defined(TCSUPPORT_CPU_EN7521)
int usb_dev_status = 0 ;
unsigned int usb_phyport_status = 0 ;
#else
extern unsigned int usb_dev_connected; //usb led support
extern unsigned int usb_led_blink;
#endif
#endif
void ledTimer(unsigned long data)
{
#ifdef TCSUPPORT_USB_HOST_LED
#if defined(TCSUPPORT_CPU_MT7510) || defined(TCSUPPORT_CPU_MT7520) || defined(TCSUPPORT_CPU_MT7505) || defined(TCSUPPORT_CPU_EN7521)
	if(Usb_Led_Flash_Op_hook)
			Usb_Led_Flash_Op_hook(USB_DEFAULT,0);//not care phy port,so set to 0
#else
	if(usb_dev_connected){
		if(usb_led_blink){
			ledTurnOn(LED_USB_ACT_STATUS);
		}
		else{
			ledTurnOn(LED_USB_STATUS);
		}
	}
	else{
		ledTurnOff(LED_USB_STATUS);
		ledTurnOff(LED_USB_ACT_STATUS);
	}//usb led support
	usb_led_blink = 0;
#endif
#endif

#ifdef TCSUPPORT_WLAN_LED_BY_SW
	if(WLan_Led_Flash_Op_hook)
			WLan_Led_Flash_Op_hook(WLAN_DEFAULT);
#endif
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
	simcardLedChk();
#endif
	ledChk();
#ifdef CONFIG_MIPS_TC3262
	mod_timer(&led_timer, jiffies + TC3262_LED_TIMMER_INTERVAL);
#else
	mod_timer(&led_timer, jiffies + TC3162_LED_TIMMER_INTERVAL);
#endif
#if defined(RT5392) || defined(MT7601E)|| defined(MT7592)
#ifdef WSC_AP_SUPPORT//add by xyyou
	{
		static int wpscnt=0;
		
		if(wpscnt==10){
			wpsLedChk();
			wpscnt=0;
		}
		wpscnt++;
#ifdef BBUTOWBU
	#ifdef  LED_WPSSPEC_COMPLY //add by xyyou to support wps led
	if(statusprobe)
		wpsLedChkSpec();
	#endif	
#endif
	}
#endif
#endif

#ifdef TCSUPPORT_LEDKEY
#endif	
#if defined(TCSUPPORT_FW_INTERNET_LED)
	if(firmware_GR_flag){ //blink for 2HZ
		if(++firmware_flash_times >= FIRMWARE_TIMERS){
			FirmwareLedBlink();
			firmware_flash_times = 0;
		}
	}
#endif
#if defined(TCSUPPORT_CT_LONG_RESETBTN) || defined(TCSUPPORT_CY) || defined(TCSUPPORT_C1_ZY)
	if ( 1 == reset_btn_long_stats )
	{
#if defined(TCSUPPORT_CT_PON_GD) || defined(TCSUPPORT_CT_PON_CZ_CQ)
		if(0==(reset_btn_long_led_counters % LEDTIMER_ONE_SECOUND))
		{
			showLedOnOff();
		}

		reset_btn_long_led_counters++;
#else
		if ( ++reset_btn_long_led_counters >= LEDTIMER_ONE_SECOUND )
		{
			showLedOnOff();
			reset_btn_long_led_counters = 0;
		}
#endif
	}
#endif
}

/*______________________________________________________________________________
**	ledChk
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
void ledChk(void)
{
	uint8 i;

		if (led_stop)
			return;

		if (CI_blink_flag) {
			LED_Blink();
		}

#if defined(TCSUPPORT_ETHER_ALL_LED)
	if(ether_lan_speed)
		ledVar[LED_ETHER_ACT_STATUS].def_blink_no = ether_lan_speed;
#endif	
		for (i = 0; i < LED_MAX_NO; i++) {
			if ((LED_MODE(ledCtrl[i].mode) == LED_MODE_NOT_USED) ||
				(LED_MODE(ledCtrl[i].mode) == LED_MODE_ONOFF) ||
				#ifdef TCSUPPORT_WLAN_LED_BY_SW
				 i == LED_WLAN_ACT_STATUS ||
				 #endif
				((LED_MODE(ledCtrl[i].mode) == LED_MODE_PULSE) && (ledVar[i].data == 0)))
				continue;

			switch (LED_MODE(ledCtrl[i].mode)) {
				case LED_MODE_BLINK:
					if (++ledVar[i].blink_no >= ledVar[i].def_blink_no) {
						if (ledVar[i].data) {
							LED_DBG_B("\r\n data=1 i=", i);
							LED_DBG_B(" blink_no=", ledVar[i].blink_no);
#if !defined(TCSUPPORT_C1_NEW_GUI) 
				#if defined (WSC_AP_SUPPORT) && defined(LED_WPSSPEC_COMPLY) || defined(TCSUPPORT_ETHER_ALL_LED)
						if ((i != GPIO_WLAN_WPS) && (i != LED_WLAN_WPS_ACT_STATUS) && (i != LED_ETHER_ACT_STATUS))
				#endif
#endif
			#if defined(TCSUPPORT_XPON_LED)
#if !defined(TCSUPPORT_C5_XPON_AUTH_LED) 
							if(i != LED_XPON_TRYING_STATUS && i != LED_XPON_LOS_STATUS 
			#if defined(TCSUPPORT_XPON_LED_UPGRADE)
								&& i != LED_XPON_UPGRADE
			#endif
								)								
#endif
								if(((isMT7525 || isMT7520 || isMT7520S) && i != LED_VOIP_HOOK1_STATUS && i != LED_VOIP_HOOK2_STATUS && i!=LED_ETHER_PORT1_ACT_STATUS && i!=LED_ETHER_PORT2_ACT_STATUS && i!=LED_ETHER_PORT3_ACT_STATUS && i!=LED_ETHER_PORT4_ACT_STATUS && i!=LED_VOIP_REG_STATUS
) || ((isEN751221) && (i == LED_USB_ACT_STATUS))
#ifdef TCSUPPORT_USB_HOST_LED
#if defined(TCSUPPORT_CPU_MT7510) || defined(TCSUPPORT_CPU_MT7520) || defined(TCSUPPORT_CPU_MT7505) || defined(TCSUPPORT_CPU_EN7521)
|| ((isEN751221) && (i == LED_USB2_ACT_STATUS))
#endif
#endif
)
            #else
                            if(isEN751221 && i != LED_VOIP_HOOK1_STATUS && i != LED_VOIP_HOOK2_STATUS)   
			#endif
#if defined(TCSUPPORT_C9_ROST_LED)
							if((i != LED_INTERNET_TRYING_STATUS) && (i != LED_XPON_UNREG_STATUS))
#endif
							ledVar[i].data = 0;
				#ifdef TCSUPPORT_LEDKEY
				//when set once led  light all the time
						if(i == LED_PWR_FLASH)
							ledVar[i].data = 1;
				#endif		
				#ifdef LED_WPSSPEC_COMPLY
							if(((i == GPIO_WLAN_WPS) && (!ledGetGpio(LED_WLAN_WPS_ACT_STATUS))&&(!LED_MODE(ledCtrl[LED_WLAN_WPS_ACT_STATUS].mode)))||(i == LED_WLAN_WPS_ACT_STATUS)){//ALPHA
									WPSLedBlink(i);
							}else
				#endif
							{
								gpioOnOff[LED_GPIO2(ledCtrl[i].gpio)] ^= 1;
								if (gpioOnOff[LED_GPIO2(ledCtrl[i].gpio)]) {
									LED_ON(ledCtrl[i].gpio, LED_BICOLOR(ledCtrl[i].mode));
									LED_DBG(" on");
								} else {
									LED_OFF(ledCtrl[i].gpio, LED_BICOLOR(ledCtrl[i].mode));
									LED_DBG(" off");
								}
								gpioNoAct[LED_GPIO2(ledCtrl[i].gpio)] = 0;
							}
						} else {
							gpioNoAct[LED_GPIO2(ledCtrl[i].gpio)]++;
						}
						ledVar[i].blink_no = 0;
					}
					break;
				case LED_MODE_NOACT:
					if (ledVar[i].data) {
						if ((ledCtrl[i].onoff && !gpioOnOff[LED_GPIO2(ledCtrl[i].gpio)]) ||
							(!ledCtrl[i].onoff && gpioOnOff[LED_GPIO2(ledCtrl[i].gpio)])) {
							if (gpioNoAct[LED_GPIO2(ledCtrl[i].gpio)] >= 1) {
								LED_DBG_B("\r\n noact data=1 i=", i);
								LED_DBG_B(" blink_no=", ledVar[i].blink_no);
								if (ledCtrl[i].onoff) {
									LED_ON(ledCtrl[i].gpio,LED_BICOLOR(ledCtrl[i].mode));
									gpioOnOff[ledCtrl[i].gpio] = 1;
									LED_DBG(" on");
								} else {
									LED_OFF(ledCtrl[i].gpio,LED_BICOLOR(ledCtrl[i].mode));
									gpioOnOff[LED_GPIO2(ledCtrl[i].gpio)] = 0;
									LED_DBG(" off");
								}
								LED_DBG_B(" gpioNoAct=", gpioNoAct[LED_GPIO2(ledCtrl[i].gpio)]);
							}
						}
					}
					break;
				case LED_MODE_PULSE:
					if (ledVar[i].onoff) {
						LED_ON(ledCtrl[i].gpio,LED_BICOLOR(ledCtrl[i].mode));
						ledVar[i].onoff = 0;
						ledVar[i].blink_no = 0;
					} else {
						if (++ledVar[i].blink_no >= ledVar[i].def_blink_no) {
							LED_OFF(ledCtrl[i].gpio,LED_BICOLOR(ledCtrl[i].mode));
							ledVar[i].data = 0;
						}
					}
					break;
			}
		}
}

/*______________________________________________________________________________
**	ledDump
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
void ledDump(void)
{
	uint8 i;

	printf("led_no\tgpio\tmode\tspeed\tonoff\r\n");
	for (i = 0; i < LED_MAX_NO; i++) {
		if(ledCtrl[i].mode > 10)
			printf("%d\t%x\t%x\t%d\t%d\r\n",
				i,
				ledCtrl[i].gpio,
				ledCtrl[i].mode,
				ledCtrl[i].speed,
				ledCtrl[i].onoff);
		else
			printf("%d\t%d\t%d\t%d\t%d\r\n",
				i,
				ledCtrl[i].gpio,
				ledCtrl[i].mode,
				ledCtrl[i].speed,
				ledCtrl[i].onoff);
	}
}

int
getLEDDef(char *stateLED)
{
	uint16	index=0;
	uint8 i;

	index += sprintf( stateLED+index, "led_no\tgpio\tmode\tspeed\tonoff\n");
	for (i = 0; i < LED_MAX_NO; i++) {
		if(ledCtrl[i].mode > 10)
		index += sprintf(stateLED+index, "%d\t%x\t%x\t%d\t%d\n",
			i,
			ledCtrl[i].gpio,
			ledCtrl[i].mode,
			ledCtrl[i].speed,
			ledCtrl[i].onoff);	
		else
		index += sprintf(stateLED+index, "%d\t%d\t%d\t%d\t%d\n",
			i,
			ledCtrl[i].gpio,
			ledCtrl[i].mode,
			ledCtrl[i].speed,
			ledCtrl[i].onoff);
	}
	return index;
}

void LED_Blink(void)
{
	uint8 i;
	if (toggleflag) {
		for (i = 0; i < LED_MAX_NO; i++) {
			if ((LED_MODE(ledCtrl[i].mode) != LED_MODE_NOT_USED) || (i == LED_SYS_BOOT_STATUS))
				LED_ON(ledCtrl[i].gpio,LED_BICOLOR(ledCtrl[i].mode));
		}
		toggleflag=0;
	}
	else {
		for (i = 0; i < LED_MAX_NO; i++) {
			if (LED_MODE(ledCtrl[i].mode) != LED_MODE_NOT_USED)
				LED_OFF(ledCtrl[i].gpio,LED_BICOLOR(ledCtrl[i].mode));
		}
		toggleflag=1;
	}
}

#if 0
void resetButtonTimer(unsigned long data)
{
	if ((regRead32(CR_GPIO_DATA) & (1<<getSysResetGpio())) == 0) {		/* reset button is pressed */
		reset_button_stats++;
	} else {
		reset_button_stats = 0;
	}
	mod_timer(&reset_button_timer, jiffies + (HZ/2));
}

#else
#ifdef TCSUPPORT_WLAN
uint8 getWlanRadioGpio(void)
{
	if (LED_MODE(ledCtrl[LED_WLAN_RADIO].mode) == LED_MODE_INPUT) // krammer change for right behavior
		return ledCtrl[LED_WLAN_RADIO].gpio;
}
#endif

#ifdef TCSUPPORT_LED_SWITCH_BUTTON
uint8 getLedSwitchGpio(void)
{
	if (LED_MODE(ledCtrl[LED_LED_SWITCH].mode) == LED_MODE_INPUT)
	{
		return ledCtrl[LED_LED_SWITCH].gpio;
	}
}
#endif

void inputButtonTimer(unsigned long data)
{
	if (get_led_data(getSysResetGpio()) == 0) {		/* reset button is pressed */
		reset_button_stats++;
#if defined(TCSUPPORT_LED_BTN_CHECK)
#if defined(TCSUPPORT_BTN_CHECK)
		if(gButtonCheck){
			reset_button_stats = 0;
			gButtonType = 1;
			printk("Reset button is pressed! \r\n");
		}
#endif
#endif
#if defined(TCSUPPORT_CT_LONG_RESETBTN) || defined(TCSUPPORT_CY) || defined(TCSUPPORT_C1_ZY)
#if defined(TCSUPPORT_CT_PON_GD) || defined(TCSUPPORT_CY) || defined(TCSUPPORT_CT_PON_CZ_CQ)
		if (reset_button_stats >= LEDTIMER_FLASH)
		{
			reset_btn_long_stats = 1;
		}
#else
		if ( reset_button_stats >= (2*INPUTBTN_TIMER_ONE_SECOUND) )
			reset_btn_long_stats = 1;
#endif
#endif
	} else {
		reset_button_stats = 0;
	}
#ifdef TCSUPPORT_WLAN
	if (LED_MODE(ledCtrl[LED_WLAN_RADIO].mode) == LED_MODE_INPUT){
		if (get_led_data(getWlanRadioGpio()) == 0) {		/* reset button is pressed */
			wlan_button_stats ++;
#if defined(TCSUPPORT_LED_BTN_CHECK)
#if defined(TCSUPPORT_BTN_CHECK)
			if(gButtonCheck){
				wlan_button_stats = 0;
				gButtonType = 2;
				printk("Wifi button is pressed! \r\n");
			}
#endif
#endif
		} else {
			wlan_button_stats = 0;
		}
	}
#endif
#ifdef WSC_AP_SUPPORT//add by xyyou
	if (LED_MODE(ledCtrl[GPIO_WLAN_WPS].mode) == LED_MODE_INPUT){
		if (get_led_data(ledGetGpio(GPIO_WLAN_WPS)) == 0) {  /* wps button is pressed */
			wps_button_stats++;
#if defined(TCSUPPORT_LED_BTN_CHECK)
#if defined(TCSUPPORT_BTN_CHECK)
		if(gButtonCheck){
			wps_button_stats = 0;
			gButtonType = 3;
			printk("WPS button is pressed! \r\n");
		}
#endif
#endif
		}else{
#if !defined(TCSUPPORT_WIFI_WPS_BTN_CO_USE)		
			if(wps_button_stats>=10){//0.5*10==5s
				wps_button_type=2;
			}else if(wps_button_stats!=0){
				wps_button_type=1;
			}
#endif
			wps_button_stats=0;
		}
	}else{
		//wait to do....
	}
#if defined(TCSUPPORT_WPS_BTN_NEWSPEC)
	if (LED_MODE(ledCtrl[GPIO_WLAN_WPS_2].mode) == LED_MODE_INPUT) {
		if (get_led_data(ledGetGpio(GPIO_WLAN_WPS_2)) == 0) {  /* wps button is pressed */
			wps_button_stats++;		
		}
		else {
			if (wps_button_stats != 0) {
				wps_button_type = 4;
				wps_button_stats=0;
			}
		}
	}
	else {
		//wait to do....
	}
#endif
#endif	

#ifdef TCSUPPORT_LED_SWITCH_BUTTON
	if (get_led_data(getLedSwitchGpio()) == 0) {		/* led switch button is pressed */
		led_switch_button_stats++;
#if defined(TCSUPPORT_LED_BTN_CHECK)
#if defined(TCSUPPORT_BTN_CHECK)
			if(gButtonCheck){
				led_switch_button_stats = 0;
				gButtonType = 4;
				printk("LED switch button is pressed! \r\n");
			}
#endif
#endif
	} else {
		if (0 != led_switch_button_stats)
		{
			led_switch_button_pressed = 1;
		}
		
		led_switch_button_stats = 0;
	}
#endif

	mod_timer(&input_button_timer, jiffies + (HZ/2));
}
#endif
static int led_def_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int i;
	int len = getLEDDef(buf);
	if (len <= off+count)
		*eof = 1;
	*start = buf + off;
	len -= off;
	if (len>count)
		len = count;
	if (len<0)
		len = 0;

	memcpy(ledCtrl, ledDefCtrl, sizeof(ledctrl_t) * LED_MAX_NO);

	for (i = 0; i < LED_MAX_NO; i++) {
#if 0 //krammer change for input type
		if (ledCtrl[i].mode != LED_MODE_NOT_USED)
			LED_OEN(ledCtrl[i].gpio);
#endif
		if (LED_MODE(ledCtrl[i].mode) != LED_MODE_INPUT)
		{
			if (LED_BICOLOR(ledCtrl[i].mode))
			{
				LED_OEN(LED_GPIO1(ledCtrl[i].gpio));
				LED_OEN(LED_GPIO2(ledCtrl[i].gpio));
			}
			else
			{
				LED_OEN(ledCtrl[i].gpio);
			}
		}
		else
		{
			LED_IEN(ledCtrl[i].gpio);
		}
	}

	return len;
}

static int led_def_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	int len;
	int set_info[5];
	char get_buf[HW_LEN];

/* do a range checking, don't overflow buffers in kernel modules */
  if(count > HW_LEN)
    len = HW_LEN;
  else
    len = count;

/* use the copy_from_user function to copy buffer data to
 * to our get_buf */
  if(copy_from_user(get_buf, buffer, len)) {
    return -EFAULT;
  }

/* zero terminate get_buf */
 get_buf[len]='\0';

 if (sscanf(get_buf, "%d %d %d %d %d", set_info, (set_info+1), (set_info+2), (set_info+3), (set_info+4)) != 5) {
		printf("usage: <led_no> <gpio> <mode> <speed> <onoff>\n");
	return count;
}
 
 if(set_info[2] > 10)
{
	sscanf(get_buf, "%d %x %x %d %d", set_info, (set_info+1), (set_info+2), (set_info+3), (set_info+4));
}

	//set ledDefCtrl information
	ledCtrl[set_info[0]].gpio  = set_info[1];
	ledCtrl[set_info[0]].mode  = set_info[2];
	ledCtrl[set_info[0]].speed = set_info[3];
	ledCtrl[set_info[0]].onoff = set_info[4];

	//reset LAN side LDE
#if 0 //krammer change for input type
	LED_OEN(ledCtrl[set_info[0]].gpio);
#endif
	if (LED_MODE(ledCtrl[set_info[0]].mode) != LED_MODE_INPUT)
	{
		if (LED_BICOLOR(ledCtrl[set_info[0]].mode))
		{
			LED_OEN(LED_GPIO1(ledCtrl[set_info[0]].gpio));
			LED_OEN(LED_GPIO2(ledCtrl[set_info[0]].gpio));
		}
		else
		{
			LED_OEN(ledCtrl[set_info[0]].gpio);
		}
	}
	else
	{
		LED_IEN(ledCtrl[set_info[0]].gpio);
	}
	LED_ON(ledCtrl[set_info[0]].gpio,LED_BICOLOR(ledCtrl[set_info[0]].mode));
	LED_OFF(ledCtrl[set_info[0]].gpio,LED_BICOLOR(ledCtrl[set_info[0]].mode));
	gpioOnOff[LED_GPIO2(ledCtrl[set_info[0]].gpio)] = 0;
	return len;
}


#if defined(TCSUPPORT_CPU_MT7520)
static int gpio_dbg_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	int gpio_num,gpio_val;
	char val_string[64], cmd[32], subcmd[32] ;
	uint action ;
	
	if (count > sizeof(val_string) - 1)
		return -EINVAL ;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT ;
	
	val_string[count] = '\0';

	sscanf(val_string, "%s %s %d %d", cmd, subcmd, &gpio_num,&gpio_val) ;

	if(!strcmp(cmd, "gpio")) {
		if(!strcmp(subcmd, "input")) {
			LED_IEN(gpio_num);
			gpio_val = LED_GET_GPIO_DATA(gpio_num);
			printk("get gpio%d input: 0x%x\n",gpio_num,gpio_val);
		} else if(!strcmp(subcmd, "output")){
				LED_OEN(gpio_num);	
				if(gpio_val == 0){
					LED_OFF(gpio_num,0);
				} else {
					LED_ON(gpio_num,0);
					gpio_val = 1;
				}
				printk("set gpio%d output: %d\n",gpio_num,gpio_val);
		} else if(!strcmp(subcmd, "get")) {
			gpio_val = LED_GET_GPIO_DATA(gpio_num);
			printk("get gpio%d val: 0x%x\n",gpio_num,gpio_val);
		} else if(!strcmp(subcmd, "set")){
			if(gpio_val == 0){
				LED_OFF(gpio_num,0);
			} else {
				LED_ON(gpio_num,0);
				gpio_val = 1;
			}
			printk("set gpio%d val: %d\n",gpio_num,gpio_val);
		} else{
			printk("gpio output/set <gpio_num> <gpio_val> \n");
			printk("gpio input/get <gpio_num> \n");
		}
		
		
	}
	return count ;
}

#endif

static int led_ppp_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	int len;

	len = sprintf(page, "%d\n", ppp_led);

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

static int led_ppp_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[8];
	unsigned long val;

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';

	val = simple_strtoul(val_string, NULL, 10);
	if (val != 0)
		ppp_led = 1;
	else
		ppp_led = 0;

	if (ppp_led) {
		ledTurnOn(LED_PPP_STATUS);
		ledTurnOn(LED_PPP_NOACT_STATUS);
	} else {
		ledTurnOff(LED_PPP_STATUS);
		ledTurnOff(LED_PPP_ACT_STATUS);
		ledTurnOff(LED_PPP_NOACT_STATUS);
	}
	return count;
}
#ifdef WSC_AP_SUPPORT//add by xyyou
static int wps_button_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	int len;

	len = sprintf(page, "%d\n", wps_button_type);

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

static int wps_button_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[8];
	unsigned long val;

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';

	val = simple_strtoul(val_string, NULL, 10);
	if (val != 0)
		wps_button_type = val;
	else
		wps_button_type = 0;
#ifdef BBUTOWBU
	if(wps_button_type==WPSSTART){
#if !defined(TCSUPPORT_CT)			
		ledSetMode(GPIO_WLAN_WPS, LED_MODE_BLINK);
		if(ledGetMode(GPIO_WLAN_WPS)!=LED_MODE_NOT_USED)
		led_oen(ledGetGpio(GPIO_WLAN_WPS));
#endif

#ifdef LED_WPSSPEC_COMPLY
		//ledTurnOn(GPIO_WLAN_WPS);//xyyou???
		//ledTurnOn(LED_WLAN_WPS_ACT_STATUS);
	//	msleep(2000);//xyyou???
		statusprobe=1;
#endif
		wps_button_type=0;
	}else if(wps_button_type==WPSSTOP){
	    	ledTurnOff(LED_WLAN_WPS_STATUS);
    		ledTurnOff(LED_WLAN_WPS_ACT_STATUS);
    //ledTurnOff(LED_WLAN_WPS_NOACT_STATUS);
//when wps stopped,The GPIO_WLAN_WPS should be return to INPUT mode.
    		ledTurnOff(GPIO_WLAN_WPS);
		if(ledGetMode(GPIO_WLAN_WPS)!=LED_MODE_NOT_USED)
    			led_ien(ledGetGpio(GPIO_WLAN_WPS));
    		ledSetMode(GPIO_WLAN_WPS, LED_MODE_INPUT);
	
	#ifdef  LED_WPSSPEC_COMPLY 
		statusprobe=0;
	#endif
		wps_button_type=0;
	}
	else if (wps_button_type==WLANSTOP){
		ledTurnOff(LED_WLAN_WPS_STATUS);
    	ledTurnOff(LED_WLAN_WPS_ACT_STATUS);
		ledTurnOff(GPIO_WLAN_WPS);
	#if defined(RT5392) || defined(MT7601E)|| defined(MT7592)
		wscTimerRunning = 0;
		wsc_done = 0;
	#endif
	#ifdef  LED_WPSSPEC_COMPLY 
		statusprobe=0;
	#endif
		wps_button_type=0;
	}
#endif	
	return count;
}
#endif
#if defined(TCSUPPORT_LED_BTN_CHECK)
#if defined(TCSUPPORT_BTN_CHECK)
static int button_type_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	int len;

	len = sprintf(page, "%d\n", gButtonType);

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

static int button_type_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[8];
	unsigned long val;

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';

	val = simple_strtoul(val_string, NULL, 10);
	if (val != 0)
		gButtonType = val;
	else
		gButtonType = 0;
	
	return count;
}
#endif
#endif
void save_gpio_info(int x)
{
	gpio_output_status[x].actived = GPIO_OUTPUT_ACTIVED;
#if defined(TCSUPPORT_CT)
	if(x > 15) {
		gpio_output_status[x].ctrl = ((VPint(CR_GPIO_CTRL1) >> (((x) - 16) * 2)) & 0x03);
	} else {
		gpio_output_status[x].ctrl = ((VPint(CR_GPIO_CTRL) >> ((x) * 2)) & 0x03);
	}
#else
	if(x > 31) {
		if(x > 47) {
			gpio_output_status[x].ctrl = ((regRead32(CR_GPIO_CTRL3) >> (((x) - 48) * 2)) & 0x03);
		} else {
			gpio_output_status[x].ctrl = ((regRead32(CR_GPIO_CTRL2) >> (((x) - 32) * 2)) & 0x03);
		}
	} else {
		if(x > 15) {
			gpio_output_status[x].ctrl = ((regRead32(CR_GPIO_CTRL1) >> (((x) - 16) * 2)) & 0x03);
		} else {
			gpio_output_status[x].ctrl = ((regRead32(CR_GPIO_CTRL) >> ((x) * 2)) & 0x03);
		}
	}		
#endif
}

void restore_gpio_info(int x)
{
	gpio_output_status[x].actived = GPIO_OUTPUT_DEACTIVED;

	if(gpio_output_status[x].ctrl == GPIO_OUTPUT_MODE) {
		LED_OEN(x);
	} else if (gpio_output_status[x].ctrl == GPIO_INPUT_MODE) {
		LED_IEN(x);
	} 
}
/*_____________________________________________________________________________
**      function name: gpio_output_write_proc
**      descriptions:
**            When echo "active gpio_no 1" > /proc/tc3162/gpio_output, this function will
**            turn high the gpio_no. ex:
**            When echo "active 20 1" > /proc/tc3162/gpio_output, turn high the gpio 20.
**            When echo "active gpio_no 0" > /proc/tc3162/gpio_output, this function will
**            turn low the gpio_no.
**            When echo "deactive gpio_no" > /proc/tc3162/gpio_output, this function will
**            restore status before active.
**
**____________________________________________________________________________
*/
static int gpio_output_write_proc(struct file *file, const char *buffer, unsigned long count, void *data)
{
	char val_string[32];
	char active[32];
	int gpio_no;
	int gpio_status;
	
	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';

	sscanf(val_string, "%s %d %d", active, &gpio_no, &gpio_status);

	if(gpio_no >= GPIO_OUTPUT_MAX_NO) {
		printk("gpio_no exceeds %d.\n", GPIO_OUTPUT_MAX_NO - 1);
		return -EINVAL;
	}	

	if(strcasecmp("active", active) == 0) {	
		/* save gpio status */
		save_gpio_info(gpio_no);
		/* enable gpio output */
		LED_OEN(gpio_no);

		if(gpio_status == 0) {	// turn low the gpio
			/* LED_ON will turn low the gpio */
			DO_LED_ON(gpio_no, LED_BICOLOR(LED_MODE_ONOFF));
		} else if(gpio_status == 1) { // turn high the gpio
			/* LED_ON will turn high the gpio */
			DO_LED_OFF(gpio_no, LED_BICOLOR(LED_MODE_ONOFF));
		} else {
			printk("gpio_status error.\n");
			return -EINVAL;
		}		
	} else if(strcasecmp("deactive", active) == 0) {
		/* restore gpio status */
		restore_gpio_info(gpio_no);
	}
			
	return count;
}

static int gpio_output_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	int len = 0;
	int i;

	for(i = 0; i < GPIO_OUTPUT_MAX_NO; i++) {
		len += sprintf(page + len, "%02d: actived:%02x ctrl:%02x\n", 
								   i, 
								   gpio_output_status[i].actived,
								   gpio_output_status[i].ctrl);
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

static int reset_button_proc_stats(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	int len;

	len = sprintf(page, "%d\n", reset_button_stats);

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

#ifdef TCSUPPORT_WLAN
static int wlan_button_proc_stats(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	int len;

	len = sprintf(page, "%d\n", wlan_button_stats);

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

#endif

#ifdef TCSUPPORT_LED_SWITCH_BUTTON
static int led_switch_button_proc_stats(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	int len;

	len = sprintf(page, "%d\n", led_switch_button_pressed);

	len -= off;
	*start = page + off;

	if (len > count)
		len = count;
	else
		*eof = 1;

	if (len < 0)
		len = 0;

	if (0 != led_switch_button_pressed)     // is pressed, clear the flag
	{
		led_switch_button_pressed = 0;
	}
	return len;
}
#if defined(TCSUPPORT_CT_JOYME)
static int led_switch_button_proc_wrstats(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[8];
	int val1 = 0;

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';
	
	if (sscanf(val_string, "%d", &val1) != 1) {

		printk("usage: <value1>\n");
		return count;
	}

	if(val1 && (0 == led_switch_button_pressed)){
		led_switch_button_pressed = val1;		
	}
	
	return count;
}
#endif
#endif

#ifdef TCSUPPORT_GPIO_ECM
static int led_mode_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	int len;

	len = sprintf(page, "%d\n", isSerialGPIO[0]);

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


static int led_mode_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[8];
	int val = 0;
	int i;

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';
	
	if (sscanf(val_string, "%d", &val) != 1) {

		printf("usage: <led mode(0:parallel; 1:serial)>\n");
		return count;
	}
	if((val == LED_PARALLEL) || (val == LED_SERIAL))
	{
		for(i = 0; i < LED_TYPE_MAX_NO; i++)
		{
			if(val)
			{
				//serial
				isSerialGPIO[i] = LED_SERIAL;
			}
			else
			{
				//parallel
				isSerialGPIO[i] = LED_PARALLEL;
			}
		}
	}
	else
	{
		printk("\r\nnot support such mode!");
	}

	return count;
}

#endif

#ifdef TCSUPPORT_LEDKEY
static int led_power_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	int len;

	len = sprintf(page, "%d %d\n", power_behavior);

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


static int led_power_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[8];
	int val = 0;

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';
	
	if (sscanf(val_string, "%d", &val) != 1) {

		printf("usage: <power behavior(0:power off;1:light red;2:light green;3:red flash)>\n");
		return count;
	}

	power_behavior = val;

	if ((ledGetMode(LED_TR68_PWR_BOOTING) == LED_MODE_NOT_USED) &&
		(ledGetMode(LED_PWR_FLASH) == LED_MODE_NOT_USED) &&
		(ledGetMode(LED_TR68_PWR_BOOTED) == LED_MODE_NOT_USED)){

		//printk("\nNo POWER LED USED\n");
		return count;
	}
	
	if (POWER_LED_OFF == power_behavior) 
	{
		
		ledTurnOff(LED_TR68_PWR_BOOTING);
		ledTurnOff(LED_TR68_PWR_BOOTED);
		ledTurnOff(LED_PWR_FLASH);
	} 
	else if(POWER_LIGHT_RED == power_behavior)
	{
		ledTurnOff(LED_TR68_PWR_BOOTED);
		ledTurnOff(LED_PWR_FLASH);
		ledTurnOn(LED_TR68_PWR_BOOTING);
	}
	else if(POWER_LIGHT_GREEN == power_behavior)
	{
		ledTurnOff(LED_PWR_FLASH);
		ledTurnOff(LED_TR68_PWR_BOOTING);
		ledTurnOn(LED_TR68_PWR_BOOTED);
	}
	else if(POWER_RED_FLASH == power_behavior)
	{
		ledTurnOff(LED_TR68_PWR_BOOTED);
		ledTurnOff(LED_TR68_PWR_BOOTING);
		ledTurnOn(LED_PWR_FLASH);
	}
	else
	{
		printk("\r\npower behavior is not support!");
	}
	
	return count;
}

#endif

#ifdef TCSUPPORT_USB_HOST_LED
#if defined(TCSUPPORT_CPU_MT7510) ||  defined(TCSUPPORT_CPU_MT7520) || defined(TCSUPPORT_CPU_MT7505) || defined(TCSUPPORT_CPU_EN7521)
static int led_usb_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	int len;

	len = sprintf(page, "%d %d\n", usb_dev_status, usb_phyport_status);
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


static int led_usb_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[8];
	int val1 = 0,val2 = 0;

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';
	
	if (sscanf(val_string, "%d %d", &val1,&val2) != 2) {

		printf("usage: <value1> <value2>\n");
		return count;
	}

	usb_dev_status = val1;
	usb_phyport_status = val2;
	return count;
}
#endif
#endif

#ifdef TR068_LED
#if defined(TCSUPPORT_CPU_MT7510) ||  defined(TCSUPPORT_CPU_MT7520) || defined(TCSUPPORT_CPU_MT7505)
static int led_internet_para_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	int len;

	len = sprintf(page, "%d %d\n", internet_hwnat_pktnum,internet_hwnat_timer_switch);

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


static int led_internet_para_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[8];
	int val1 = 0,val2 = 0;

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';
	
	if (sscanf(val_string, "%d %d", &val1,&val2) != 2) {

		printf("usage: <judgement pkts> <need timer or not>\n");
		return count;
	}

	internet_hwnat_pktnum = val1 > 0? val1:8;;
	internet_hwnat_timer_switch = val2>0?1:0;
	
	return count;
}
#endif
static int led_internet_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	int len;

	len = sprintf(page, "%d %d\n", internet_led_on, internet_trying_led_on);

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

static int led_internet_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[8];
	int val1, val2;

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';
	
	if (sscanf(val_string, "%d %d", &val1, &val2) != 2) {

		printf("usage: <is_internet_led> <is_internet_trying_led>\n");
		return count;
	}

#if defined(TCSUPPORT_FW_INTERNET_LED)
	if(firmware_GR_flag){
		if (val2 == FIRMWARE_BEHAVIOR_NO){
			firmware_GR_flag = 0;
			ledTurnOn(LED_INTERNET_STATUS);
			ledTurnOn(LED_INTERNET_NOACT_STATUS);
		}
		return count;
	}
	else{
		if (val2 == FIRMWARE_BEHAVIOR_YES){
			firmware_GR_flag = 1;
			ledTurnOff(LED_INTERNET_STATUS);
			ledTurnOff(LED_INTERNET_ACT_STATUS);
			ledTurnOff(LED_INTERNET_NOACT_STATUS);
			ledTurnOff(LED_INTERNET_TRYING_STATUS);
			internet_led_on = 0;
			internet_trying_led_on = 1;
			return count;
		}
	}
#endif	
	if (val1 != 0){
#ifdef TCSUPPORT_WAN_ETHER_LED
		internet_led_on = val1;
#else
		internet_led_on = 1;
#endif
	}
	else
		internet_led_on = 0;

	if (val2 != 0)
		internet_trying_led_on = 1;
	else
		internet_trying_led_on = 0;

	if ((ledGetMode(LED_INTERNET_STATUS) == LED_MODE_NOT_USED) &&
		(ledGetMode(LED_INTERNET_ACT_STATUS) == LED_MODE_NOT_USED) &&
		(ledGetMode(LED_INTERNET_TRYING_STATUS) == LED_MODE_NOT_USED) &&
		(ledGetMode(LED_INTERNET_NOACT_STATUS) == LED_MODE_NOT_USED)){

		return count;
	}
#if defined(TCSUPPORT_C9_ROST_LED)
	if (internet_led_on) {
		/*IP Connected*/
		ledTurnOff(LED_INTERNET_TRYING_STATUS);
		ledTurnOn(LED_INTERNET_STATUS);
		ledTurnOn(LED_INTERNET_NOACT_STATUS);
	} 
	else {
		/*Not Connected or connecting*/
		ledTurnOff(LED_INTERNET_NOACT_STATUS);
		ledTurnOff(LED_INTERNET_STATUS);
		ledTurnOn(LED_INTERNET_TRYING_STATUS);
	}
	
#else	
#if defined(TCSUPPORT_CT)
	if (internet_led_on) {//IP Connected and no traffic
		ledTurnOff(LED_INTERNET_TRYING_STATUS); // add by xyzhu
		ledTurnOn(LED_INTERNET_STATUS);
		ledTurnOn(LED_INTERNET_NOACT_STATUS);
	} 
	else {
		ledTurnOff(LED_INTERNET_NOACT_STATUS); // add by xyzhu
		if(internet_trying_led_on) {//CPE is trying to get WAN IP
			ledTurnOff(LED_INTERNET_STATUS);
			ledTurnOn(LED_INTERNET_TRYING_STATUS);
		}
		else {//modem in bridge mode or ADSL connection not present
			ledTurnOff(LED_INTERNET_STATUS);
			ledTurnOff(LED_INTERNET_TRYING_STATUS);
			
		}
	}
#else
#ifdef TCSUPPORT_WAN_ETHER_LED
	if(internet_led_on == 1){//has DSL IP Connected and no traffic
			ledTurnOff(LED_INTERNET_TRYING_STATUS);
			ledTurnOn(LED_INTERNET_STATUS);
			ledTurnOn(LED_INTERNET_NOACT_STATUS);
	}
	else{
		//no dsl has ip, turn off dsl led
		ledTurnOff(LED_INTERNET_STATUS);
		ledTurnOff(LED_INTERNET_NOACT_STATUS);
		if(internet_trying_led_on) {//CPE is trying to get WAN IP
			//ledTurnOff(LED_INTERNET_STATUS);
			ledTurnOn(LED_INTERNET_TRYING_STATUS);
		}
		else {//modem in bridge mode or ADSL connection not present
			//ledTurnOff(LED_INTERNET_STATUS);
			ledTurnOff(LED_INTERNET_TRYING_STATUS);
			
		}
	}
	
	if(internet_led_on == 2){
		//only has ether wan ip
		ledTurnOn(LED_ETHER_WAN_STATUS);
	}else{
		ledTurnOff(LED_ETHER_WAN_STATUS);
	}
	
#else
	if (internet_led_on) {//IP Connected and no traffic
		ledTurnOff(LED_INTERNET_TRYING_STATUS);
		ledTurnOn(LED_INTERNET_STATUS);
		ledTurnOn(LED_INTERNET_NOACT_STATUS);
	} 
	else {
		ledTurnOff(LED_INTERNET_STATUS);
		ledTurnOff(LED_INTERNET_NOACT_STATUS);
		if(internet_trying_led_on) {//CPE is trying to get WAN IP
			//ledTurnOff(LED_INTERNET_STATUS);
			ledTurnOn(LED_INTERNET_TRYING_STATUS);
		}
		else {//modem in bridge mode or ADSL connection not present
			//ledTurnOff(LED_INTERNET_STATUS);
			ledTurnOff(LED_INTERNET_TRYING_STATUS);
			
		}
	}
#endif
#endif
#endif
	
	return count;
}
#endif

#if defined(TCSUPPORT_XPON_LED)
static int led_xpon_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	int len;

	len = sprintf(page, "%d\n", led_xpon_status);

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

static int led_xpon_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[8];
	unsigned long val;

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';

	val = simple_strtoul(val_string, NULL, 10);
#if defined(TCSUPPORT_XPON_LED_UPGRADE)	
	if(val < 0 || val > 3)
#else
	if(val < 0 || val > 2)
#endif		
		led_xpon_status = 0;
	else
		led_xpon_status = val;
	
#if defined(TCSUPPORT_C9_ROST_LED)
	if (led_xpon_status == 2 && xpon_los_status != 0) {
		ledTurnOff(LED_XPON_UNREG_STATUS); 
		ledTurnOff(LED_XPON_TRYING_STATUS); 	
		ledTurnOn(LED_XPON_STATUS);
	} else if(led_xpon_status == 1 && xpon_los_status != 0){
		ledTurnOff(LED_XPON_UNREG_STATUS); 
		ledTurnOff(LED_XPON_STATUS); 	
		ledTurnOn(LED_XPON_TRYING_STATUS);

	}else{
		ledTurnOff(LED_XPON_TRYING_STATUS); 
		ledTurnOff(LED_XPON_STATUS); 	
		ledTurnOn(LED_XPON_UNREG_STATUS);
	}
#else	
	if (led_xpon_status == 2 && xpon_los_status != 0) {
#if defined(TCSUPPORT_XPON_LED_UPGRADE)
		ledTurnOff(LED_XPON_UPGRADE);
#endif	
		ledTurnOff(LED_XPON_TRYING_STATUS);		
		ledTurnOn(LED_XPON_STATUS);
	} else if(led_xpon_status == 1 && xpon_los_status != 0){
#if defined(TCSUPPORT_XPON_LED_UPGRADE)
		ledTurnOff(LED_XPON_UPGRADE);
#endif	
		ledTurnOff(LED_XPON_STATUS);
		ledTurnOn(LED_XPON_TRYING_STATUS);
#if defined(TCSUPPORT_XPON_LED_UPGRADE)
	} else if(led_xpon_status == 3 && xpon_los_status != 0){
		ledTurnOff(LED_XPON_STATUS);
		ledTurnOff(LED_XPON_TRYING_STATUS);
		ledTurnOn(LED_XPON_UPGRADE);
#endif	
	}else{
		ledTurnOff(LED_XPON_STATUS);
		ledTurnOff(LED_XPON_TRYING_STATUS);
#if defined(TCSUPPORT_XPON_LED_UPGRADE)
		ledTurnOff(LED_XPON_UPGRADE);
#endif	

	}
#endif	

	return count;
}
static int ether_led_752x_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	int len;
	
#if defined(TCSUPPORT_CUC_C5_2P)
	len = sprintf(page, "%d  %d  %d  %d\n", led_ether_up_status[0], led_ether_up_status[3], led_ether_up_status[2], led_ether_up_status[1]);
#else
	len = sprintf(page, "%d  %d  %d  %d\n", led_ether_up_status[0], led_ether_up_status[1], led_ether_up_status[2], led_ether_up_status[3]);
#endif

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

#if defined(TCSUPPORT_CUC_C5_SFU)
int isPort4Up100M;
EXPORT_SYMBOL(isPort4Up100M);
#endif


static int ether_led_752x_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[10];
	int index=-1, val=-1;

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';
	
	sscanf(val_string, "%d %d", &index, &val);
#if !defined(TCSUPPORT_XPON_HAL_API_EXT) 
	if(index < 0 || index > 3 || val < 0 || val > 2)
		return -EFAULT;
#endif

	else{		
#if defined(TCSUPPORT_CUC_C5_2P)
	if(index == 1) index = 3;
	else if(index == 2) index = 2;
	else if(index == 3) index = 1;
#else
#if defined(TCSUPPORT_CUC_C5_4P)
	index = 3 - index;  
#else
	if (isMT7520S && index == 0)
		index = 3; // translate to port 4
#endif
#endif

		led_ether_up_status[index] = val;
	}
	
	if (val==2) { // ON
		{
		ledTurnOff(LED_ETHER_PORT1_ACT_STATUS + 2*index);
		ledTurnOn(LED_ETHER_PORT1_STATUS+ 2*index);
#if defined(TCSUPPORT_CUC_C5_SFU)
		if(isPort4Up100M)
			ledTurnOn(LED_ETHER_100M_STATUS);
#endif
}
	}
	else if (val==1) { // BLINK
		{
		ledTurnOff(LED_ETHER_PORT1_STATUS+ 2*index);
		ledTurnOn(LED_ETHER_PORT1_ACT_STATUS + 2*index);
}
	} 
	else { // OFF
		{
		ledTurnOff(LED_ETHER_PORT1_STATUS + 2*index);
		ledTurnOff(LED_ETHER_PORT1_ACT_STATUS + 2*index);
#if defined(TCSUPPORT_CUC_C5_SFU)
		ledTurnOff(LED_ETHER_100M_STATUS);
#endif
}		
	}

	return count;
}


#endif

#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
static int led_simcard_interface_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	int len;

	len = sprintf(page, "%d\n", simcard_interface_status);
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

static int led_simcard_interface_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[8];
	unsigned long val;

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';

	val = simple_strtoul(val_string, NULL, 10);
	if(val < 0 || val > 3)
		simcard_interface_status = 0;
	else
		simcard_interface_status = val;
	if(Simcard_Led_hook)
		Simcard_Led_hook(SIMCARD_LED_INTERFACE, simcard_interface_status);
	return count;
}

static int led_simcard_config_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	int len;

	len = sprintf(page, "%d\n", simcard_config_status);
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

static int led_simcard_config_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[8];
	unsigned long val;

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';

	val = simple_strtoul(val_string, NULL, 10);
	if(val < 0 || val > 2)
		simcard_config_status = 0;
	else
		simcard_config_status = val;
	if(Simcard_Led_hook)
		Simcard_Led_hook(SIMCARD_LED_CONFIG, simcard_config_status);
	return count;
}

void Simcard_Led(unsigned int type ,unsigned int status)
{

	if(type == SIMCARD_LED_INTERFACE){
		switch(status){
			case SIMCARD_INTERFACE_CONNECT:
				simcard_interface_status = SIMCARD_INTERFACE_CONNECT;
				ledTurnOn(LED_SIM_STATUS);
				break;
			case SIMCARD_INTERFACE_ERROR:		//0.5s on, 0.5s off
				simcard_interface_status = SIMCARD_INTERFACE_ERROR;
				break;
			case SIMCARD_INTERFACE_AUTH_FAIL:	//0.5s on, 1.5 off
				simcard_interface_status = SIMCARD_INTERFACE_AUTH_FAIL;
				break;
			default:
				simcard_interface_status = SIMCARD_INTERFACE_UNUSED;
				ledTurnOff(LED_SIM_STATUS);
				break;
		}
	}
	else if(type == SIMCARD_LED_CONFIG){
		switch(status){
			case SIMCARD_CONFIG_SUCCESS:
				simcard_config_status = SIMCARD_CONFIG_SUCCESS;
				ledTurnOn(LED_SIM_CFG_STATUS);
				break;
			case SIMCARD_CONFIG_RUNNING:		//0.5s on, 0.5s off
				simcard_config_status = SIMCARD_CONFIG_RUNNING;
				break;
			default:
				simcard_config_status = SIMCARD_CONFIG_FAIL;
				ledTurnOff(LED_SIM_CFG_STATUS);
				break;
		}
	}
	
	return;
}

void simcardLedChk(void){
	static int interface_error_cnt=0;
	static int interface_auth_fail_cnt = 0;
	static int config_running_cnt = 0;
	static int interface_connect_cnt = 0;
	static int interface_unused_cnt = 0;
	static int config_success_cnt = 0;
	static int config_fail_cnt = 0;
	
	//SIMCARD_INTERFACE_ERROR, 0.5s on, 0.5s off
	if(simcard_interface_status == SIMCARD_INTERFACE_ERROR){
		interface_auth_fail_cnt = 0;
		interface_connect_cnt = 0;
		interface_unused_cnt = 0;
		if(interface_error_cnt == 5){
			ledTurnOn(LED_SIM_STATUS);
		}
		if(interface_error_cnt == 10){
			ledTurnOff(LED_SIM_STATUS);
			interface_error_cnt = 0;
		}
		interface_error_cnt++;
	}else if(simcard_interface_status == SIMCARD_INTERFACE_AUTH_FAIL){
	//SIMCARD_INTERFACE_AUTH_FAIL, 0.5s on, 1.5 off
		interface_error_cnt = 0;
		interface_connect_cnt = 0;
		interface_unused_cnt = 0;
		if(interface_auth_fail_cnt == 5){
			ledTurnOff(LED_SIM_STATUS);
		}
		if(interface_auth_fail_cnt == 20){
			ledTurnOn(LED_SIM_STATUS);
			interface_auth_fail_cnt = 0;
		}
		interface_auth_fail_cnt++;
	}else{
		interface_error_cnt = 0;
		interface_auth_fail_cnt = 0;
		if(simcard_interface_status == SIMCARD_INTERFACE_CONNECT){
			interface_unused_cnt = 0;
			if(interface_connect_cnt < 3){
				ledTurnOn(LED_SIM_STATUS);
				interface_connect_cnt++;
			}
		}else{
			interface_connect_cnt = 0;
			if(interface_unused_cnt < 3){
				ledTurnOff(LED_SIM_STATUS);
				interface_unused_cnt++;
			}
		}
	}

	//SIMCARD_CONFIG_RUNNING, 0.5s on, 0.5s off
	if(simcard_config_status == SIMCARD_CONFIG_RUNNING){
		if(config_running_cnt == 5){
			ledTurnOn(LED_SIM_CFG_STATUS);
		}
		if(config_running_cnt == 10){
			ledTurnOff(LED_SIM_CFG_STATUS);
			config_running_cnt = 0;
		}
		config_running_cnt++;
	}else if(simcard_config_status == SIMCARD_CONFIG_SUCCESS){
		config_running_cnt = 0;
		config_fail_cnt = 0;
		if(config_success_cnt < 3){
			ledTurnOn(LED_SIM_CFG_STATUS);
			config_success_cnt++;
		}
	}else{
		config_running_cnt = 0;
		config_success_cnt = 0;
		if(config_fail_cnt < 3){
			ledTurnOff(LED_SIM_CFG_STATUS);
			config_fail_cnt++;
		}
	}
}

#endif


#if defined(TCSUPPORT_CT_LONG_RESETBTN) || defined(TCSUPPORT_CY) || defined(TCSUPPORT_C1_ZY)
static uint8 wholeLed[]=
{
#if !defined(TCSUPPORT_CT_PON)
	LED_DSL_STATUS,
#endif
	LED_USB_STATUS,
#ifdef TCSUPPORT_USB_HOST_LED
#if defined(TCSUPPORT_CPU_MT7510) || defined(TCSUPPORT_CPU_MT7520) || defined(TCSUPPORT_CPU_MT7505) || defined(TCSUPPORT_CPU_EN7521)
	LED_USB2_STATUS,
#endif
#endif
	LED_WLAN_WPS_STATUS,
	LED_INTERNET_STATUS,
	LED_VOIP_HOOK1_STATUS,
	LED_VOIP_HOOK2_STATUS,
#ifdef TCSUPPORT_XPON_LED
	LED_XPON_STATUS,
	LED_XPON_LOS_ON_STATUS,
#endif
#ifdef TCSUPPORT_C1_ZY
	LED_TR68_PWR_BOOTED,
#endif
};

uint8 isOnOffLed(uint8 led_no)
{
	int i = 0;

	for( i=0; i< sizeof(wholeLed)/sizeof(uint8); i++ )
	{
		if(led_no == wholeLed[i])
			return 1;
	}
	return 0;
}

void led_onoff_sw(int led_no, int led_on)
{
	if( led_on )	//turn on
	{
		if(ledCtrl[led_no].onoff)
			LED_OFF(ledCtrl[led_no].gpio, LED_BICOLOR(ledCtrl[led_no].mode));
		else
		{
			if(LED_BICOLOR(ledCtrl[led_no].mode))
			{
				LED_ON(  (ledCtrl[led_no].gpio & 0x0f) , 0);
				LED_ON(  ((ledCtrl[led_no].gpio>>4)&0x0f), 0);
			}
			else
				LED_ON(ledCtrl[led_no].gpio, LED_BICOLOR(ledCtrl[led_no].mode));
		}	
	}
	else	//turn off
	{
		if(ledCtrl[led_no].onoff)
		{					
			if(LED_BICOLOR(ledCtrl[led_no].mode))
			{
				LED_ON(  (ledCtrl[led_no].gpio & 0x0f) , 0);
				LED_ON(  ((ledCtrl[led_no].gpio>>4)&0x0f), 0);
			}
			else
				LED_ON(ledCtrl[led_no].gpio, LED_BICOLOR(ledCtrl[led_no].mode));
		}
		else
			LED_OFF(ledCtrl[led_no].gpio, LED_BICOLOR(ledCtrl[led_no].mode));
	}
}

void showLedOnOff(void)
{
	int i = 0;
	uint8 ledmode = 0;

	led_stop = 1;

	for ( i = 0; i < LED_MAX_NO; i++ )
	{
		ledmode = LED_MODE(ledCtrl[i].mode);
		if ( LED_MODE_NOT_USED == ledmode
			|| LED_MODE_INPUT == ledmode
			|| LED_MODE_NOACT == ledmode
			|| 0 == isOnOffLed(i) )
			continue;

		if ( reset_btn_long_led_onff )
			led_onoff_sw(i, 0);
		else
			led_onoff_sw(i, 1);
	}

	reset_btn_long_led_onff = ( 1 == reset_btn_long_led_onff ? 0 : 1 );
}
#endif

int ledNumberSpecial(int led_no)
{
	
	return 0;
}

static char* led_conf_path_find(void)
{
    uint8 i;
    char *p_path = NULL;
    uint8 arr_len = 0;
	struct file	*srcf;
    led_path_m path_table[]=
    {
        {isEN7526F, "/userfs/7526fled.conf"}, 
        {isEN7526D, "/userfs/7526dled.conf"},
        {isEN7526G, "/userfs/7526gled.conf"},
        {isEN7512, "/userfs/7512led.conf"},
        {isEN7513, "/userfs/7513led.conf"},
        {isEN7513G, "/userfs/7513gled.conf"},
        {isEN7586, "/userfs/7586led.conf"},
        {isEN7521F, "/userfs/7521fled.conf"},
        {isEN7521G, "/userfs/7521gled.conf"},      
        {isEN7521S, "/userfs/7521sled.conf"},
        {isDEFAULT,"/userfs/led.conf"}/*must last element*/
    };
    
    arr_len = sizeof(path_table)/sizeof(path_table[0]);
    
    /*find led.conf*/
    for(i = 0; i < arr_len; i++)
    {
        if(path_table[i].chipid)
        {
            p_path = path_table[i].path;
            break;
        }
    }
    
    /*check whether led.conf file exist,otherwise use default*/
    if(p_path && *p_path)
    { 
        srcf = filp_open(p_path, O_RDONLY, 0);
        if (IS_ERR(srcf))
        {
            p_path = path_table[arr_len-1].path;
        }
        else
        {
            filp_close(srcf,NULL);
        }
    }
    else
    {
        printk("find led.conf error\r\n");
    }
        
    /*use default led.conf*/
    if(i == arr_len)
        p_path = path_table[arr_len-1].path;

    /*if TCSUPPORT_XPON_HAL_API_EXT,rewrite path*/
    #if defined(TCSUPPORT_XPON_HAL_API_EXT)
        p_path = "/etc/led.conf";
    #endif
    
        return p_path;
}

/*______________________________________________________________________________
**	ledReadConf
**
**	descriptions:
**		Read led.conf into table
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**		2009.8.12	pork modified
**____________________________________________________________________________*/
static void ledReadConf(void)
{
	struct file				*srcf;
	uint32 tmp_fpos = 0;
	char *src;
	
#if !KERNEL_2_6_36
	int 					orgfsuid, orgfsgid;
#endif

	mm_segment_t			orgfs;
	char					*buffer;
	char					*tmpbuf;
	int set_info[5];
	const int bufSize = 128;
	buffer = kmalloc(bufSize, GFP_ATOMIC);
	int ret_value;
		
	if(buffer == NULL)
	       return;
	
    src = led_conf_path_find();
   
    printk("led path:%s\r\n",src);
    
	// Save uid and gid used for filesystem access.
	// Set user and group to 0 (root)
	#if !KERNEL_2_6_36
	orgfsuid = current->fsuid;
	orgfsgid = current->fsgid;
	current->fsuid=current->fsgid = 0;
	#endif
	
	orgfs = get_fs();
	set_fs(KERNEL_DS);

	if (src && *src)
	{
		srcf = filp_open(src, O_RDONLY, 0);
		if (IS_ERR(srcf)){
			printk("--> Error opening \n");
		}
		else{
			memset(buffer,0,bufSize);
			
			while(srcf->f_op->read(srcf, buffer, bufSize, &srcf->f_pos) >0)
			{
				tmpbuf = buffer;
				
				/*filter comment line*/
				if((*tmpbuf) != ';')
				{
					sscanf(tmpbuf, "%d %d %d %d %d", set_info, (set_info+1), (set_info+2), (set_info+3), (set_info+4));
					
					if(set_info[2] > 10){
						sscanf(tmpbuf, "%d %x %x %d %d", set_info, (set_info+1), (set_info+2), (set_info+3), (set_info+4));
					}
					
					if(set_info[0] < LED_MAX_NO)
					{
						ret_value = ledNumberSpecial(set_info[0]);

						if (0 == ret_value)
						{
						
							ledDefCtrl[set_info[0]].gpio  = set_info[1];

#if defined(TCSUPPORT_CT_PON)
							if (set_info[0] == LED_INTERNET_ACT_STATUS)
								ledDefCtrl[set_info[0]].mode  = LED_MODE_ONOFF; /* turn on */
#endif

							ledDefCtrl[set_info[0]].mode  = set_info[2];
							ledDefCtrl[set_info[0]].speed = set_info[3];
							ledDefCtrl[set_info[0]].onoff = set_info[4];
						}
					}
				}
				
				/*If read the end of line, read the next*/
				while((*tmpbuf) != '\r' && (*tmpbuf) != '\n')
				{
					tmp_fpos++;
					tmpbuf++;
				}
				
				/*indicate the next head of line*/
				tmp_fpos++;
				srcf->f_pos = tmp_fpos;
				memset(buffer, 0, bufSize);
			}
			filp_close(srcf,NULL);
		}
	}
	set_fs(orgfs);
	
#if !KERNEL_2_6_36
	current->fsuid = orgfsuid;
	current->fsgid = orgfsgid;
#endif
	kfree(buffer);
}

#if defined(TC_SUPPORT_3G) && defined(TR068_LED)
extern void (*Dongle_InternetLed_hook)(void);

void Dongle_InternetLed_Flash(){
	if(internet_led_on) {//IP connected and IP traffic is passing
		ledTurnOn(LED_INTERNET_STATUS);
		ledTurnOn(LED_INTERNET_ACT_STATUS);
	} 
	else {
		if(!internet_trying_led_on) {
			ledTurnOff(LED_INTERNET_STATUS);
			ledTurnOff(LED_INTERNET_TRYING_STATUS);
		}
	}	
}
#endif
#ifdef TCSUPPORT_USB_HOST_LED
#if defined(TCSUPPORT_CPU_MT7510) || defined(TCSUPPORT_CPU_MT7520) || defined(TCSUPPORT_CPU_MT7505) || defined(TCSUPPORT_CPU_EN7521)
void Usb_Led_Flash_Op(unsigned int opmode ,unsigned int phyport)
{
	int temp = 0;

#ifdef TCSUPPORT_LED_SWITCH_BUTTON
	if (led_stop)
			return;
#endif

	switch (opmode)
	{
		case USB_DISCONNECT:
		if(USBPHYPORT1 == phyport)
		{
			temp = usb_dev_status & 0x0000ffff;
			temp = --temp < 0? 0:temp;
			usb_dev_status &= 0xffff0000;
			usb_dev_status |= temp & 0xffff;
			if(!temp)
			{
				usb_phyport_status &= ~(1<<phyport);
				//ledTurnOff(LED_USB_ACT_STATUS);
				ledTurnOff(LED_USB_STATUS);
			}
		}
		else if(USBPHYPORT2 == phyport)
		{
			temp = (usb_dev_status & 0xffff0000) >> 16;
			temp = --temp < 0? 0:temp;
			usb_dev_status &= 0x0000ffff;
			usb_dev_status |= (temp << 16) & 0xffff0000;
			if(!temp)
			{
				usb_phyport_status &= ~(1<<phyport);
				//ledTurnOff(LED_USB2_ACT_STATUS);
				ledTurnOff(LED_USB2_STATUS);	
			}
		}

		break;

		case USB_CONNECT:
		if(USBPHYPORT1 == phyport)
		{
			temp = usb_dev_status & 0xffff;
			//temp++;
			usb_dev_status &= 0xffff0000;
			usb_dev_status |= ++temp & 0xffff;
			//ledTurnOff(LED_USB_ACT_STATUS);
			ledTurnOn(LED_USB_STATUS);
		}
		else if(USBPHYPORT2 == phyport)
		{
			temp = (usb_dev_status & 0xffff0000) >> 16;
			//temp++;
			usb_dev_status &= 0xffff;
			usb_dev_status |= (++temp << 16) & 0xffff0000;
			//ledTurnOff(LED_USB2_ACT_STATUS);
			ledTurnOn(LED_USB2_STATUS);
		}
		usb_phyport_status |= 1<<phyport;

		break;


		case USB_BLINK:
		if(USBPHYPORT1 == phyport)
			ledTurnOn(LED_USB_ACT_STATUS);
		
		if(USBPHYPORT2 == phyport)
			ledTurnOn(LED_USB2_ACT_STATUS);	

		break;

		case USB_DEFAULT://Fall through	
		default:
		//for timer use
		if(usb_dev_status){
			if(usb_phyport_status & (1<<USBPHYPORT1))
			{
				ledTurnOn(LED_USB_STATUS);
			}
			else
			{
				ledTurnOff(LED_USB_STATUS);
			}
			
			if(usb_phyport_status & (1<<USBPHYPORT2))
			{
				ledTurnOn(LED_USB2_STATUS);
			}
			else
			{
				ledTurnOff(LED_USB2_STATUS);
			}
		}
		else{
			ledTurnOff(LED_USB_STATUS);
			ledTurnOff(LED_USB_ACT_STATUS);
			ledTurnOff(LED_USB2_STATUS);
			ledTurnOff(LED_USB2_ACT_STATUS);
		}
		break;
	}
	
	return;
}
#endif
#endif

#ifdef TCSUPPORT_WLAN_LED_BY_SW
unsigned int wifi_ap_switch = 0;
unsigned int wifi_ap_blink = 0;

static int led_wlan_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	int len;

	len = sprintf(page, "%d\n", wifi_ap_switch);

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

static int led_wlan_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[8];
	unsigned long opmode;

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';

	opmode = simple_strtoul(val_string, NULL, 10);
	if(opmode < 0 || opmode > 2)
		opmode = 0;

	wifi_ap_switch = (opmode == WLAN_DISCONNECT)?0:1;
	WLan_Led_Flash_Op(opmode);
	return count;
}

void WLanLedBlink(void)
{
	static int flag = 0;//0:wlan led on,1:wlan led off
	if(wifi_ap_switch)
	{
		if(wifi_ap_blink)
		{
			if (++ledVar[LED_WLAN_ACT_STATUS].blink_no >= ledVar[LED_WLAN_ACT_STATUS].def_blink_no) {	
				if (flag) {
					LED_OFF(ledCtrl[LED_WLAN_ACT_STATUS].gpio,LED_BICOLOR(ledCtrl[LED_WLAN_ACT_STATUS].mode));
					flag = 0;	
					wifi_ap_blink = 0;
				}
				else {
					LED_ON(ledCtrl[LED_WLAN_ACT_STATUS].gpio,LED_BICOLOR(ledCtrl[LED_WLAN_ACT_STATUS].mode));
					flag = 1;
				}
				ledVar[LED_WLAN_ACT_STATUS].blink_no = 0;
			}
		}
		else
		{
			ledTurnOff(LED_WLAN_ACT_STATUS);
			ledTurnOn(LED_WLAN_STATUS);
		}
	}
	else
	{
		ledTurnOff(LED_WLAN_STATUS);
		ledTurnOff(LED_WLAN_ACT_STATUS);
	}
}

void WLan_Led_Flash_Op(unsigned int opmode)
{
	int temp = 0;
	if(LED_MODE(ledCtrl[LED_WLAN_ACT_STATUS].mode) == LED_MODE_NOT_USED)
		return;
	
	switch (opmode)
	{
		case WLAN_DISCONNECT:
			ledTurnOff(LED_WLAN_STATUS);
		break;

		case WLAN_CONNECT:
			ledTurnOn(LED_WLAN_STATUS);
		break;

		case WLAN_BLINK:	
			wifi_ap_blink = 1;
		break;

		case WLAN_DEFAULT://Fall through	
		default:
		//for timer use
		WLanLedBlink();
		break;
	}
	
	return;
}
EXPORT_SYMBOL(WLan_Led_Flash_Op);
#endif

#if defined(TCSUPPORT_CT_BUTTONDETECT)
static int event_full(struct button_event *q)
{
	if(((q->tail - q->head) == 1) || ((q->tail - q->head) == -(BUTTON_QUEUE_LEN-1)))
		return 1;
	else 
		return 0;
}

static int event_empty(struct button_event* o)
{
	return (o->head == o->tail);
}

static void event_enqueue(struct button_event *q, int type, int data){
	int flags;
	spin_lock_irqsave(&q->lock, flags);
	if(event_full(q)) {	// drop
		q->tail++;
		if(q->tail == BUTTON_QUEUE_LEN){
			q->tail = 0;
		}	
	}
	q->type[q->head] = type;
	q->data[q->head] = data;
	
	q->head ++ ;
	if(q->head == BUTTON_QUEUE_LEN){
		q->head = 0;
	}
	spin_unlock_irqrestore(&q->lock, flags);
}

static unsigned event_dequeue(struct button_event * o, int *data)
{
	int t,flags;
	spin_lock_irqsave(&o->lock, flags);
	if(event_empty(o)){
		spin_unlock_irqrestore(&o->lock, flags);
		return -1;
	}
	t=o->tail++;
	data[0]=o->type[t];
	data[1]=o->data[t];
	
	if(o->tail == BUTTON_QUEUE_LEN){
		o->tail = 0;
	}
	spin_unlock_irqrestore(&o->lock, flags);
	return 1;
}


static int ledbutton_open(struct inode *inode, struct file *filp)
{	
	return 0;	
}
	
static int ledbutton_release(struct inode *inode, struct file *filp)
{
	return 0;
}

static int cdev_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int ret = 0;
	int t[2] = {0};
	if (_IOC_TYPE(cmd) !=LEDBUTTON_IOCTL_MAGIC)
	{
		return -ENOTTY;
	}

	if (_IOC_DIR(cmd) & _IOC_READ)
	{
		ret = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
	}
	else if (_IOC_DIR(cmd) & _IOC_WRITE)
	{
		ret = !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));

	}

	if (ret)
	{
		return -EFAULT;
	}
	
	switch(cmd)
	{
		case LEDBUTTON_IOCTL_SELECT:
		{
			int i;
			i = event_dequeue(&button_eq, &t[0]);
			if (i > 0)
			{
				//printk("sofree::%s:%d i = %d\r\n", __FUNCTION__, __LINE__, i);
				ret = copy_to_user((void __user *) arg, t, sizeof(int[2]));			
				if(ret)
					printk("copy data error\n");
			}
			break;
		}	
		default:
			ret = -1;
	}
	return ret;
}

void buttonDetectTimer(unsigned long data)
{
	
#ifdef TCSUPPORT_WLAN
		if (LED_MODE(ledCtrl[LED_WLAN_RADIO].mode) == LED_MODE_INPUT){
			if (get_led_data(getWlanRadioGpio()) == 0) {		/* wlan button is pressed */
				wlan_counters ++;
			} else {
				if(wlan_counters  > TIMES_IN_ONE_SECOUND*3){   //long click >3s
					//printk("Wifi button is pressed!   >3s \r\n");
					event_enqueue(&button_eq, WLAN_BUTTON, LONG_CLICK);
				}else if(wlan_counters >0){
					if(jiffies - lastWlanJiffies < 1*HZ){
						event_enqueue(&button_eq, WLAN_BUTTON, DOUBLE_CLICK);
						wlanDelay = 0;//double click then clean the first "click " flag.
					}
					else{
						wlanDelay = TIMES_IN_ONE_SECOUND-1;//remember this time's "click"
					}
					lastWlanJiffies = jiffies;
				}else{
					if((wlanDelay>0) &&(--wlanDelay == 1)){//enqueue the last click info
						event_enqueue(&button_eq, WLAN_BUTTON, CLICK);
						wlanDelay = 0;
					}
				}		
				wlan_counters = 0;
			}
		}
#endif

#ifdef WSC_AP_SUPPORT
	if (LED_MODE(ledCtrl[GPIO_WLAN_WPS].mode) == LED_MODE_INPUT){
		if (get_led_data(ledGetGpio(GPIO_WLAN_WPS)) == 0) {  /* wps button is pressed */
			wps_counters++;
		}else{
			if(wps_counters  > TIMES_IN_ONE_SECOUND*3){   //long click   >3s
				//printk("wps button is pressed!   > 3s \r\n");
				event_enqueue(&button_eq, WPS_BUTTON, LONG_CLICK);
			}else if(wps_counters >0){
				if(jiffies - lastWpsJiffies < 1*HZ){
					event_enqueue(&button_eq, WPS_BUTTON, DOUBLE_CLICK);
					wpsDelay = 0;//double click then clean the first "click " flag.
				}
				else{
					wpsDelay = TIMES_IN_ONE_SECOUND-1;//remember this time's "click"
				}
				lastWpsJiffies = jiffies;
			}else{
				if((wpsDelay>0) &&(--wpsDelay == 1)){//enqueue the last click info
					event_enqueue(&button_eq, WPS_BUTTON, CLICK);
					wpsDelay = 0;
				}
			}		
			wps_counters=0;
		}
	}
#endif	
	mod_timer(&button_detect_timer, jiffies + (HZ/TIMES_IN_ONE_SECOUND));

}

static void event_init(struct button_event* o)
{
	memset(o, 0, sizeof(struct button_event));
	o->head = o->tail = 0;
	spin_lock_init(&(o->lock));
	return;
}

static struct file_operations led_button_fops = 
{
	.owner = THIS_MODULE,
	.unlocked_ioctl = cdev_ioctl,
	.open = ledbutton_open,
	.release= ledbutton_release,
};


int led_button_cdev_init(void)
{
	int err = 0;
	
	err = register_chrdev(LEDBUTTON_MAJOR_NUM , MODULE_NAME, &led_button_fops);
	event_init(&button_eq);
	lastWlanJiffies = jiffies;
	lastWpsJiffies = jiffies;
	
	init_timer(&button_detect_timer);
	button_detect_timer.expires = jiffies + (HZ * 2);
	button_detect_timer.function = buttonDetectTimer;
	button_detect_timer.data = 0x0;
	add_timer(&button_detect_timer);
	return err;	
}

void led_button_cdev_exit(void)
{
	del_timer_sync(&button_detect_timer);
	unregister_chrdev(LEDBUTTON_MAJOR_NUM, MODULE_NAME);
}

#endif

static int __init tc3162_led_init(void)
{
	struct proc_dir_entry *led_proc;
	int i;
    unsigned int read_data = 0;
	printk("TC3162 LED Manager 0.1 init\n");
	printk("\r\ntcledctrl version: %s.\n", MODULE_VERSION_TCLEDCTRL);
	ledReadConf();
	ledInit();
#ifdef GPIO_VERIFY
	ledVerifyInit();
#endif

#if defined(TCSUPPORT_XPON_HAL_API_EXT)
    /*wait slic start, config GPIO 33 high*/
    /*config GPIO PIN 33 output_mode*/
    read_data = regRead32(CR_GPIO_CTRL2);
    read_data = (read_data & 0xfffffff3) | (1<<2);
    regWrite32(CR_GPIO_CTRL2, read_data);
    
    read_data = regRead32(CR_GPIO_ODRAIN1);
    read_data = read_data | (1<<1);
    regWrite32(CR_GPIO_ODRAIN1, read_data);

    /*config GPIO PIN 33 LOW*/
    read_data = regRead32(CR_GPIO_DATA1);
    read_data = (read_data & (~(1<<1)));
    //printk("read_data = %d, line = %d.\n", read_data, __LINE__);
    regWrite32(CR_GPIO_DATA1, read_data);
    
    /*set gpio 0 ~~5  input mode*/
    read_data = regRead32(CR_GPIO_CTRL);
    read_data &= 0xfffff000;
    regWrite32(CR_GPIO_CTRL,read_data);
    
    read_data = regRead32(CR_GPIO_ODRAIN);
    read_data &= 0xffffffc0;
    regWrite32(CR_GPIO_ODRAIN,read_data);	

#endif

#if defined(TCSUPPORT_CT_BUTTONDETECT)
	led_button_cdev_init();
#endif

	init_timer(&led_timer);
	led_timer.expires = jiffies + (HZ * 2);
	led_timer.function = ledTimer;
	led_timer.data = 0x0;
	add_timer(&led_timer);

	/* led definition */
	led_proc = create_proc_entry("tc3162/led_def", 0, NULL);
	led_proc->read_proc = led_def_read_proc;
	led_proc->write_proc = led_def_write_proc;

#if defined(TCSUPPORT_CPU_MT7520)
	/* gpio debug */
	led_proc = create_proc_entry("tc3162/gpio_dbg", 0, NULL);
	led_proc->read_proc = NULL;
	led_proc->write_proc = gpio_dbg_write_proc;
#endif

	/* pppoe/pppoa led */
	led_proc = create_proc_entry("tc3162/led_ppp", 0, NULL);
	led_proc->read_proc = led_ppp_read_proc;
	led_proc->write_proc = led_ppp_write_proc;

	/* wps input button */
	#ifdef WSC_AP_SUPPORT
	led_proc = create_proc_entry("tc3162/wps_button", 0, NULL);
	led_proc->read_proc = wps_button_read_proc;
	led_proc->write_proc = wps_button_write_proc;
	#endif

#if defined(TCSUPPORT_LED_BTN_CHECK)
#if defined(TCSUPPORT_BTN_CHECK)
	led_proc = create_proc_entry("tc3162/button_type", 0, NULL);
	led_proc->read_proc = button_type_read_proc;
	led_proc->write_proc = button_type_write_proc;
#endif
#endif
	
	/* For control led and gpio output control */
	for(i = 0; i < GPIO_OUTPUT_MAX_NO; i++) {
		gpio_output_status[i].actived = GPIO_OUTPUT_DEACTIVED;
	}
	led_proc = create_proc_entry("tc3162/gpio_output", 0, NULL);
	led_proc->write_proc = gpio_output_write_proc;
	led_proc->read_proc = gpio_output_read_proc;

	/* reset button status */
#if 0//modify by xyyou to add input btn timer
	init_timer(&reset_button_timer);
	reset_button_timer.expires = jiffies + (HZ * 2);
	reset_button_timer.function = resetButtonTimer;
	reset_button_timer.data = 0x0;
	add_timer(&reset_button_timer);
#else
	init_timer(&input_button_timer);
	input_button_timer.expires = jiffies + (HZ * 2);
	input_button_timer.function = inputButtonTimer;
	input_button_timer.data = 0x0;
	add_timer(&input_button_timer);
#endif

	create_proc_read_entry("tc3162/reset_button", 0, NULL, reset_button_proc_stats, NULL);

#ifdef TCSUPPORT_WLAN
	create_proc_read_entry("tc3162/wlan_button", 0, NULL, wlan_button_proc_stats, NULL);
#endif

#ifdef TCSUPPORT_LED_SWITCH_BUTTON
#if defined(TCSUPPORT_CT_JOYME)
	led_proc = create_proc_entry("tc3162/led_switch_button", 0, NULL);
	led_proc->read_proc = led_switch_button_proc_stats;
	led_proc->write_proc = led_switch_button_proc_wrstats;
#else
	create_proc_read_entry("tc3162/led_switch_button", 0, NULL, led_switch_button_proc_stats, NULL);
#endif
#endif

#ifdef TCSUPPORT_LEDKEY
	/* power led */
	led_proc = create_proc_entry("tc3162/led_power", 0, NULL);
	led_proc->read_proc = led_power_read_proc;
	led_proc->write_proc = led_power_write_proc;
#endif	
#ifdef TCSUPPORT_GPIO_ECM
	/* parallel or serial */
	led_proc = create_proc_entry("tc3162/ledmode", 0, NULL);
	led_proc->read_proc = led_mode_read_proc;
	led_proc->write_proc = led_mode_write_proc;
#endif

#ifdef TR068_LED
	/* internet led */
	led_proc = create_proc_entry("tc3162/led_internet", 0, NULL);
	led_proc->read_proc = led_internet_read_proc;
	led_proc->write_proc = led_internet_write_proc;
#if defined(TCSUPPORT_CPU_MT7510) ||  defined(TCSUPPORT_CPU_MT7520) || defined(TCSUPPORT_CPU_MT7505)
	led_proc = create_proc_entry("tc3162/led_internet_para", 0, NULL);
	led_proc->read_proc = led_internet_para_read_proc;
	led_proc->write_proc = led_internet_para_write_proc;
#endif
#ifdef TC_SUPPORT_3G
  	rcu_assign_pointer(Dongle_InternetLed_hook, Dongle_InternetLed_Flash); 
#endif
#endif
#ifdef TCSUPPORT_USB_HOST_LED
#if defined(TCSUPPORT_CPU_MT7510) || defined(TCSUPPORT_CPU_MT7520) || defined(TCSUPPORT_CPU_MT7505) || defined(TCSUPPORT_CPU_EN7521)
	led_proc = create_proc_entry("tc3162/led_usb", 0, NULL);
	led_proc->read_proc = led_usb_read_proc;
	led_proc->write_proc = led_usb_write_proc;

	rcu_assign_pointer(Usb_Led_Flash_Op_hook, Usb_Led_Flash_Op);
#if KERNEL_3_18_21
	if ( USB_CONNECT == pre_usb_state[0]
		|| USB_BLINK == pre_usb_state[0] )
		Usb_Led_Flash_Op(USB_CONNECT, USBPHYPORT1);
	if ( USB_CONNECT == pre_usb_state[1]
		|| USB_BLINK == pre_usb_state[1] )
		Usb_Led_Flash_Op(USB_CONNECT, USBPHYPORT2);
#endif
#endif
#endif
#ifdef TCSUPPORT_WLAN_LED_BY_SW
	led_proc = create_proc_entry("tc3162/led_wlan", 0, NULL);
	led_proc->read_proc = led_wlan_read_proc;
	led_proc->write_proc = led_wlan_write_proc;
	rcu_assign_pointer(WLan_Led_Flash_Op_hook, WLan_Led_Flash_Op);
#endif
#if defined(TCSUPPORT_XPON_LED)
	/* xpon led */
	led_proc = create_proc_entry("tc3162/led_xpon", 0, NULL);
	led_proc->read_proc = led_xpon_read_proc;
	led_proc->write_proc = led_xpon_write_proc;
	if (isMT7525 || isMT7520 || isMT7520S
	){
		led_proc = create_proc_entry("tc3162/led_752x", 0, NULL);
		led_proc->read_proc = ether_led_752x_read_proc;
		led_proc->write_proc = ether_led_752x_write_proc;
	}
#endif

#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
	led_proc = create_proc_entry("tc3162/led_simcard_interface", 0, NULL);
	if(led_proc){
		led_proc->read_proc = led_simcard_interface_read_proc;
		led_proc->write_proc = led_simcard_interface_write_proc;
	}

	led_proc = create_proc_entry("tc3162/led_simcard_config", 0, NULL);
	if(led_proc){
		led_proc->read_proc = led_simcard_config_read_proc;
		led_proc->write_proc = led_simcard_config_write_proc;
	}

	rcu_assign_pointer(Simcard_Led_hook, Simcard_Led);
#endif


	return 0;
}

static void __exit tc3162_led_exit(void)
{
	printk("TC3162 LED Manager 0.1 exit\n");

	del_timer_sync(&led_timer);
#ifdef GPIO_VERIFY
		ledVerifyUnInit();
#endif
	remove_proc_entry("tc3162/led_def", NULL);
	remove_proc_entry("tc3162/led_ppp", NULL);
	
#if defined(TCSUPPORT_CPU_MT7520)
	remove_proc_entry("tc3162/gpio_dbg", NULL);
#endif	

#if defined(TCSUPPORT_XPON_LED)
	remove_proc_entry("tc3162/led_xpon", NULL);

	if (isMT7525 || isMT7520 || isMT7520S
	){
		remove_proc_entry("tc3162/led_752x", NULL);
	}
#endif
	remove_proc_entry("tc3162/gpio_output", NULL);
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
	remove_proc_entry("tc3162/led_simcard_interface", NULL);
	remove_proc_entry("tc3162/led_simcard_config", NULL);
	rcu_assign_pointer(Simcard_Led_hook, NULL); 	
#endif

#if defined(TC_SUPPORT_3G) && defined(TR068_LED)
  	rcu_assign_pointer(Dongle_InternetLed_hook, NULL); 	
#endif
#ifdef TCSUPPORT_USB_HOST_LED
#if defined(TCSUPPORT_CPU_MT7510) || defined(TCSUPPORT_CPU_MT7520) || defined(TCSUPPORT_CPU_MT7505) || defined(TCSUPPORT_CPU_EN7521)
	rcu_assign_pointer(Usb_Led_Flash_Op_hook, NULL); 
#endif
#endif
#ifdef TCSUPPORT_WLAN_LED_BY_SW
	remove_proc_entry("tc3162/led_wlan", NULL);
	rcu_assign_pointer(WLan_Led_Flash_Op_hook, NULL);
#endif

#if defined(TCSUPPORT_CT_BUTTONDETECT)
	led_button_cdev_exit();
#endif

}

EXPORT_SYMBOL(ledTurnOn);
EXPORT_SYMBOL(ledTurnOff);
EXPORT_SYMBOL(ledGetMode);
EXPORT_SYMBOL(ledSetMode);
EXPORT_SYMBOL(ledGetGpio);
EXPORT_SYMBOL(led_oen);
EXPORT_SYMBOL(led_ien);
EXPORT_SYMBOL(ledChk);
#ifdef WSC_AP_SUPPORT//add by xyyou
#if defined(RT5392) || defined(MT7601E)|| defined(MT7592)
EXPORT_SYMBOL(wsc_done);
EXPORT_SYMBOL(wscTimerRunning);
#ifdef BBUTOWBU
EXPORT_SYMBOL(wscStatus);
#endif
#endif
#ifdef LED_WPSSPEC_COMPLY
EXPORT_SYMBOL(SetWPSLedMode);
#endif
#endif

#if defined(TCSUPPORT_TEST_LED_ALL)
static uint8 wps_gpio_mode = LED_MODE_NOT_USED;

void led_onoff(int led_no,int led_on)
{
	if(led_on)  //turn on
	{
		if(ledCtrl[led_no].onoff)
			LED_OFF(ledCtrl[led_no].gpio, LED_BICOLOR(ledCtrl[led_no].mode));
		else
		{
			if(LED_BICOLOR(ledCtrl[led_no].mode))
			{
				LED_ON(  (ledCtrl[led_no].gpio & 0x0f) , 0);
				LED_ON(  ((ledCtrl[led_no].gpio>>4)&0x0f), 0);
			}
			else
				LED_ON(ledCtrl[led_no].gpio, LED_BICOLOR(ledCtrl[led_no].mode));
		}	
	}
	else	//turn off
	{
		if(ledCtrl[led_no].onoff)
		{					
			if(LED_BICOLOR(ledCtrl[led_no].mode))
			{
				LED_ON(  (ledCtrl[led_no].gpio & 0x0f) , 0);
				LED_ON(  ((ledCtrl[led_no].gpio>>4)&0x0f), 0);
			}
			else
				LED_ON(ledCtrl[led_no].gpio, LED_BICOLOR(ledCtrl[led_no].mode));
		}
		else
			LED_OFF(ledCtrl[led_no].gpio, LED_BICOLOR(ledCtrl[led_no].mode));
	}
}


static uint8 skipLed[]={
	LED_DSL_NOACT_STATUS,
	LED_PPP_NOACT_STATUS,
	LED_USB_NOACT_STATUS,
	LED_WLAN_NOACT_STATUS,
	LED_ETHER_NOACT_STATUS,
	LED_ETHER_10M_NOACT_STATUS,
	LED_ETHER_100M_NOACT_STATUS,
	LED_INTERNET_NOACT_STATUS,
//	LED_PPP_NOACT_BICOLOR,                        //only defined in linos router
	LED_WLAN_WPS_NOACT_STATUS, 
	LED_LAN_RESET,
	LED_VOIP_SLIC1_RESET,
	LED_VOIP_SLIC2_RESET,
	LED_LAN1_RESET,
	GPIO_SYS_RESET,
	LED_SYS_BOOT_STATUS,
	LED_PHY_TX_POWER_DISABLE,
	LED_PHY_VCC_DISABLE,
                    
};

uint8 isSkipLed(uint8 led_no)
{
	int i;
	for(i=0;i< sizeof(skipLed)/sizeof(uint8) ;i++)
	{
		if(led_no == skipLed[i])
			return 1;
	}
	return 0;
}//usage:   if(isSkipLed(i)) continue;

#endif


#if defined(TCSUPPORT_LED_BTN_CHECK) || defined(TCSUPPORT_TEST_LED_ALL) || defined(GPIO_VERIFY)
#if defined(TCSUPPORT_LED_CHECK) || defined(TCSUPPORT_TEST_LED_ALL) || defined(GPIO_VERIFY)
int doLedOn(void)
{
	int i = 0;

	CI_blink_flag = 0;
	led_stop = 1;
	
#if defined(TCSUPPORT_TEST_LED_ALL)
#if defined(TCSUPPORT_CY) || defined(TCSUPPORT_C7) || defined(TCSUPPORT_CT_PON)
	uint32 word = 0; //test by xyzhu
	int lan_gpio;
#endif
#endif
	
	for (i = 0; i < LED_MAX_NO; i++) {
#if 0//steven
		if ((LED_MODE(ledCtrl[i].mode) != LED_MODE_NOT_USED) || (i == LED_SYS_BOOT_STATUS))
#else
		if (((LED_MODE(ledCtrl[i].mode) != LED_MODE_NOT_USED) && (LED_MODE(ledCtrl[i].mode) != LED_MODE_INPUT)
				)||(i == LED_SYS_BOOT_STATUS))
#endif
#if !defined(TCSUPPORT_TEST_LED_ALL)  
			//LED_ON(ledCtrl[i].gpio);//, LED_BICOLOR(ledCtrl[i].mode));
			#if defined(TCSUPPORT_XPON_LED)
			if (i == LED_XPON_STATUS || i == LED_XPON_TRYING_STATUS 
#if defined(TCSUPPORT_XPON_LED_UPGRADE)
			|| i == LED_XPON_UPGRADE 
#endif
			|| i == LED_XPON_LOS_ON_STATUS || i==LED_XPON_LOS_STATUS)
				LED_OFF(ledCtrl[i].gpio, LED_BICOLOR(ledCtrl[i].mode));
			else
			#endif
			LED_ON(ledCtrl[i].gpio,LED_BICOLOR(ledCtrl[i].mode));

	}//end for

#else
			if(LED_MODE(ledCtrl[i].mode) !=LED_MODE_NOACT )
			{
				 if(isSkipLed(i)) 
					continue;
				led_onoff(i,1);
			}
	}//end for

#if defined(TCSUPPORT_TEST_LED_ALL)
#if defined(TCSUPPORT_CY) || defined(TCSUPPORT_C7)
	word = regRead32(CR_GPIO_SSR);
	word &= ~(0x3e00);
	regWrite32(CR_GPIO_SSR, word);

	for (lan_gpio = 42; lan_gpio < 42+4; lan_gpio++) {
		LED_OEN(lan_gpio);
		LED_ON(lan_gpio, 0);
	}
#endif

#if defined(TCSUPPORT_CT_PON) && defined(TCSUPPORT_CPU_EN7521)
	/* gpio 7 ~ gpio 10 is for lan led in demo board */

	/* set gpio 7 ~ gpio 10 to gpio mode */
	word = regRead32(RG_PON_I2C_MODE);
	word &= ~(0x78);
	regWrite32(RG_PON_I2C_MODE, word);

	word = regRead32(RG_FORCE_GPIO2_EN);
	word |= (0x170);
	regWrite32(RG_FORCE_GPIO2_EN, word);

	word = regRead32(RG_GPIO_CTRL);
	word &= ~(0x3fc000);
	word |= 0x154000;
	regWrite32(RG_GPIO_CTRL, word);
	
	/* turn on lan led */
	for (lan_gpio = 7; lan_gpio < 7+4; lan_gpio++) {
		LED_OEN(lan_gpio);
		LED_ON(lan_gpio, 0);
	}
	
#endif
	
#endif

	if ((LED_MODE_NOT_USED != ledCtrl[GPIO_WLAN_WPS].mode) || (LED_MODE_NOT_USED != wps_gpio_mode))
		if( ( (ledCtrl[GPIO_WLAN_WPS].gpio & 0x0f) == (ledCtrl[ LED_WLAN_WPS_STATUS].gpio & 0x0f))
			|| (LED_MODE_NOT_USED == ledCtrl[ LED_WLAN_WPS_STATUS].mode) ) 
		{
			if(wps_gpio_mode == LED_MODE_NOT_USED)
			{
				wps_gpio_mode = ledCtrl[GPIO_WLAN_WPS].mode;
				ledCtrl[GPIO_WLAN_WPS].mode = LED_MODE_NOT_USED;	
			}
			LED_OEN(ledCtrl[GPIO_WLAN_WPS].gpio);
			led_onoff(GPIO_WLAN_WPS,1);
		}	

#if defined(TCSUPPORT_LED_SWITCH_BUTTON) && defined(TCSUPPORT_WLAN)
	// turn on wlan led
	if(hook_wlan_led_action != NULL)
	{
		hook_wlan_led_action(WLAN_LED_ON, -1);
	}
	
#if defined(TCSUPPORT_WLAN_AC)
	if(hook_wlan_led_action_5g != NULL)
	{
		hook_wlan_led_action_5g(WLAN_LED_ON, -1);
	}
#endif

#endif
#endif
	return 0;
}

int doLedOff(void)
{
	int  i;

#if defined(TCSUPPORT_TEST_LED_ALL)
#if defined(TCSUPPORT_CY) || defined(TCSUPPORT_C7) || defined(TCSUPPORT_CT_PON)
	uint32 word; // add by xyzhu
	int lan_gpio;
#endif
#endif

	CI_blink_flag = 0;

#ifdef	TCSUPPORT_LED_SWITCH_BUTTON
	led_stop = 1;          // do not return from test mode
#else
	led_stop = 0;
#endif

	for (i = 0; i < LED_MAX_NO; i++) {
#if defined(TCSUPPORT_CT_PON_SC)
		if (reset_button_stats > 60 && ledCtrl[i].gpio == LED_SYS_STATUS)
			continue;
#endif
#if 0//steven
		if (LED_MODE(ledCtrl[i].mode) != LED_MODE_NOT_USED) 
#else
		if ((LED_MODE(ledCtrl[i].mode) != LED_MODE_NOT_USED) && (LED_MODE(ledCtrl[i].mode) != LED_MODE_INPUT)
				)
#endif
#if !defined(TCSUPPORT_TEST_LED_ALL)  
			//LED_OFF(ledCtrl[i].gpio);//, LED_BICOLOR(ledCtrl[i].mode));
			#if defined(TCSUPPORT_XPON_LED)
			if (i == LED_XPON_STATUS || i == LED_XPON_TRYING_STATUS 
#if defined(TCSUPPORT_XPON_LED_UPGRADE)
			|| i == LED_XPON_UPGRADE 
#endif
			|| i == LED_XPON_LOS_ON_STATUS || i==LED_XPON_LOS_STATUS)
				LED_ON(ledCtrl[i].gpio, LED_BICOLOR(ledCtrl[i].mode));
			else
			#endif
			LED_OFF(ledCtrl[i].gpio, LED_BICOLOR(ledCtrl[i].mode));
	}//end for
#else	
			if(LED_MODE(ledCtrl[i].mode) !=LED_MODE_NOACT )  
			{
 				if(isSkipLed(i)) 
						continue;
				led_onoff(i,0);
			}

	}//end for

#if defined(TCSUPPORT_TEST_LED_ALL)
#if defined(TCSUPPORT_CY) || defined(TCSUPPORT_C7)
	word = regRead32(CR_GPIO_SSR);
	word &= ~(0x3e00);
	regWrite32(CR_GPIO_SSR, word);

	for (lan_gpio = 42; lan_gpio < 42+4; lan_gpio++) {
		LED_OEN(lan_gpio);
		LED_OFF(lan_gpio, 0);
	}
#endif

	
#if defined(TCSUPPORT_CT_PON) && defined(TCSUPPORT_CPU_EN7521)
	/* gpio 7 ~ gpio 10 is for lan led in demo board */

	/* set gpio 7 ~ gpio 10 to gpio mode */
	word = regRead32(RG_PON_I2C_MODE);
	word &= ~(0x78);
	regWrite32(RG_PON_I2C_MODE, word);

	word = regRead32(RG_FORCE_GPIO2_EN);
	word |= (0x170);
	regWrite32(RG_FORCE_GPIO2_EN, word);

	word = regRead32(RG_GPIO_CTRL);
	word &= ~(0x3fc000);
	word |= 0x154000;
	regWrite32(RG_GPIO_CTRL, word);
	
	/* turn on lan led */
	for (lan_gpio = 7; lan_gpio < 7+4; lan_gpio++) {
		LED_OEN(lan_gpio);
		LED_OFF(lan_gpio, 0);
	}
#endif

#endif
	
	if ((LED_MODE_NOT_USED != ledCtrl[GPIO_WLAN_WPS].mode) || (LED_MODE_NOT_USED != wps_gpio_mode))
	{	
		if( ( (ledCtrl[GPIO_WLAN_WPS].gpio & 0x0f) == (ledCtrl[ LED_WLAN_WPS_STATUS].gpio & 0x0f))
			|| (LED_MODE_NOT_USED == ledCtrl[ LED_WLAN_WPS_STATUS].mode) ) 
		{

			led_onoff(GPIO_WLAN_WPS,0);

			LED_IEN(ledCtrl[GPIO_WLAN_WPS].gpio);
			if(wps_gpio_mode)
			{
				ledCtrl[GPIO_WLAN_WPS].mode = wps_gpio_mode ;
				wps_gpio_mode = LED_MODE_NOT_USED;
			}
		}
	}

#if defined(TCSUPPORT_LED_SWITCH_BUTTON) && defined(TCSUPPORT_WLAN)
	// turn off wlan led
	if(hook_wlan_led_action != NULL)
	{
		hook_wlan_led_action(WLAN_LED_OFF, -1);
	}
	
#if defined(TCSUPPORT_WLAN_AC)
	if (hook_wlan_led_action_5g != NULL) {
		hook_wlan_led_action_5g(WLAN_LED_OFF, -1);
	}
#endif
	
#endif
#endif	
	return 0;
}

int doLedRecover(void)
{
#if defined(TCSUPPORT_TEST_LED_ALL)
#if defined(TCSUPPORT_CT_PON) && defined(TCSUPPORT_CPU_EN7521)
		uint32 word; // add by xyzhu
		int lan_gpio;
#endif
#endif

	led_stop = 0;
	ledTurnOn(LED_TR68_PWR_BOOTED);

#if defined(TCSUPPORT_TEST_LED_ALL)
#if defined(TCSUPPORT_CT_PON) && defined(TCSUPPORT_CPU_EN7521)
	/* set gpio 7 ~ gpio 10 to phy mode */
	word = regRead32(RG_PON_I2C_MODE);
	word |= (0x78);
	regWrite32(RG_PON_I2C_MODE, word);

	word = regRead32(RG_FORCE_GPIO2_EN);
	word &= ~(0x170);
	regWrite32(RG_FORCE_GPIO2_EN, word);
#endif
#endif

#if defined(TCSUPPORT_LED_SWITCH_BUTTON) && defined(TCSUPPORT_WLAN)
	if(hook_wlan_led_action != NULL)
	{
		hook_wlan_led_action(WLAN_LED_RECOVER, 0);
	}
	
#if defined(TCSUPPORT_WLAN_AC)
	if (hook_wlan_led_action_5g != NULL) {
		hook_wlan_led_action_5g(WLAN_LED_RECOVER, -1);
	}
#endif

#endif
	return 0;
}

#endif
#endif

module_init (tc3162_led_init);
module_exit (tc3162_led_exit);
