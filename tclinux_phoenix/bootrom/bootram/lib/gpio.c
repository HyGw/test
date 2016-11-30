#include <linux/delay.h>
#include <asm/io.h>
#include <asm/tc3162.h>

typedef unsigned char			UCHAR;
typedef UCHAR * 				PUCHAR;

#define printf	prom_printf

#define LED_OEN(x)		do { 	if(x > 31){							\
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
				} while(0)

#define LED_OFF(x) 	do {								\
								if(x > 31){					\
									regWrite32(CR_GPIO_DATA1,regRead32(CR_GPIO_DATA1)|(1<<(x-32)));	\
								}else{						\
									regWrite32(CR_GPIO_DATA,regRead32(CR_GPIO_DATA)|(1<<x));	\
								}						\		
						} while (0)
#define LED_ON(x) 	do {								\
								if(x > 31){					\
									regWrite32(CR_GPIO_DATA1,regRead32(CR_GPIO_DATA1)& ~(1<<(x-32)));	\
								}else{						\
									regWrite32(CR_GPIO_DATA,regRead32(CR_GPIO_DATA)& ~(1<<x));	\
								}						\
						} while (0)

#define WLAN_LED_OFF 0
#define WLAN_LED_ON  1
#define WLAN_LED_OEN  2
#define WLAN_LED_IEN  3


#define MAC_CSR0            0x1000
#define Base_Addr 	      0xBF70000
#define GPIO_CTRL_CFG    0x0228	//MAC_CSR13
#define GPIO_SWITCH				0x05dc

#define LED_WIFI_OFF(x)    wifiGpioLedAct(WLAN_LED_OFF, x)
#define LED_WIFI_ON(x)      wifiGpioLedAct(WLAN_LED_ON, x)
#define LED_WIFI_OEN(x)     wifiGpioLedAct(WLAN_LED_OEN, x)																			
#define LED_WIFI_IEN(x)      wifiGpioLedAct(WLAN_LED_IEN, x)

#define RTMP_IO_READ32(_A, _R, _pV)	\
{		\
	(*_pV = readl((void *)((_A) + MAC_CSR0)));		\
	(*_pV = readl((void *)((_A) + (_R))));			\
}
#define RTMP_IO_WRITE32(_A, _R, _V)		\
{		\
	writel((_V), (void *)((_A) + (_R)));\
}

uint32 pcie_config_addr;
uint32 pcie_config_data;
sint7	pcieOKFlag = -1;


#define NIC3090_PCIe_DEVICE_ID  0x3090		// 1T/1R miniCard
#define NIC3091_PCIe_DEVICE_ID  0x3091		// 1T/2R miniCard
#define NIC3092_PCIe_DEVICE_ID  0x3092		// 2T/2R miniCard
#define NIC3390_PCIe_DEVICE_ID  0x3390		// 1T/1R miniCard
#define NIC5390_PCIe_DEVICE_ID	0x5390
#define NIC539F_PCIe_DEVICE_ID  0x539F
#define NIC5392_PCIe_DEVICE_ID  0x5392

#define NIC_PCI_VENDOR_ID	0x1814
#define PCI_DEVICE_MEM1		0xbf700000

#define PCIE_TYPE_RC		0x0
#define PCIE_TYPE_DEV		0x1
#define PCIE_BUS_RC			0x0
#define PCIE_BUS_DEV		0x1
#define PCIE_DEVNUM_0		0x0
#define PCIE_DEVNUM_1		0x1


int pcieReset(void);
int pcieRegInitConfig(void);
void ledInit(void);
static int wifiGpioLedAct(int action ,int gpio);

extern unsigned long Jiffies;
static void tc_mdelay(int delaytime){
	unsigned long start, end;

	start  = end = Jiffies;
	while ((end - start) <= delaytime)
	{
		end = Jiffies;
		printf("\r");
	}
}

void pcie_init(void)
{

	pcie_config_addr = 0x1fb80020;
	pcie_config_data = 0x1fb80024;
	if(pcieReset() == 0 ){
		pcieOKFlag = pcieRegInitConfig();
		if (pcieOKFlag == 0)
			ledInit();
	}
}

int pcieReset(void){	
	int i;

	unsigned long value=0;
 	if(isRT63165){
 		return -1;		
 	}

 	if(isRT63365 || isMT751020 || isMT7505){
 		//disable interrupt
//	VPint(0xbfb8000c) &= ~(1<<20);
		tc_mdelay(5);
 		//PCI-E reset
 	#if !defined(TCSUPPORT_CT_WAN_PTM)
 		if (isFPGA) {		
 			//FPGA mode
 			if(isMT751020 || isMT7505){
 				VPint(0xbfb00834) &= ~(1<<26);
 				tc_mdelay(1);
 				VPint(0xbfb00834) |= (1<<26);
 				tc_mdelay(1);
 				VPint(0xbfb00834) &= ~(1<<26);
 				tc_mdelay(1);
 			}else{
 				VPint(0xbfb00834) &= ~(1<<26);
 				tc_mdelay(1);
 				VPint(0xbfb00834) |= (1<<26);
 				tc_mdelay(1);
 			}
 		}else
 	#endif
 		{	
 			/* disabled PCIe port 1 */
			VPint(0xbfb00088) &= ~(1<<22);
			
			tc_mdelay(1);
			VPint(0xbfb00834) |= ((1<<26) | (1<<29));
			tc_mdelay(1);
			VPint(0xbfb00834) &= ~((1<<26) | (1<<29));
			tc_mdelay(1);
 		}		

 		if(isMT751020 || isMT7505){
 			VPint(0xbfb80000) |= ((1<<1) | (1<<2));
 			tc_mdelay(1);
 			VPint(0xbfb80000) &= ~((1<<1) | (1<<2));
 			tc_mdelay(1);
 		}else{
 			VPint(0xbfb80000) |= (1<<1);
 			tc_mdelay(1);
 			VPint(0xbfb80000) &= ~(1<<1);
 			tc_mdelay(1);
 		}

		VPint(0xbfb80028) = 0x1f700000;
 		//wait device link up
 		for(i=0 ; i<1000 ; i++){
			tc_mdelay(1);
 			if((VPint(0xbfb82050) & 0x1) != 0){
 				break;
 			}   
 		}
 		if(i == 1000){
			printf("PCI-E RC can not link up\n");
			return -1;
		}
 		//config PCI-E RC
		VPint(0xbfb82010) = 0xffff0001;//disable support BAR0
 
 		//change class PCI-PCI Bridge
		VPint(0xbfb82034) = 0x06040001;
 
 		//Enable CRC count .
 		VPint(KSEG1ADDR(pcie_config_addr)) = 0x118;
		VPint(KSEG1ADDR(pcie_config_data)) |= (1<<8);

		
 
 	}else{
		VPint(0xBFB00088) &= ~(1<<29);
		tc_mdelay(5);
		VPint(0xBFB00088) &= ~(1<<30);
		tc_mdelay(5);
		VPint(0xBFB00088) |= (1<<29);
		tc_mdelay(5);
		VPint(0xBFB00088) |= (1<<30);
		tc_mdelay(5);
 		/*force link up, workaround the pcie hardware problems.*/
 		if(isTC3162U){
 			VPint(KSEG1ADDR(pcie_config_addr)) = 0x40;
 			VPint(KSEG1ADDR(pcie_config_data)) = 0x20;
 		}
	}
	return 0;
}

int pcie_write_config_word(unsigned char type, unsigned char bus, unsigned char devnum, unsigned char regnum, unsigned long int value)
{
	if(isRT63165 || isRT63365 || isMT751020 || isMT7505){
		VPint(KSEG1ADDR(pcie_config_addr)) = (bus<<24 |devnum<<19|regnum);
	}else{
		VPint(KSEG1ADDR(pcie_config_addr))=(type<<31|bus<<20 |devnum<<15|regnum);
	}	
		VPint(KSEG1ADDR(pcie_config_data))=value;
	return 0;
}
int pcie_write_config_byte(unsigned char type, unsigned char bus, unsigned char devnum, unsigned char regnum, unsigned char value)
{
	if(isRT63165 || isRT63365 || isMT751020 || isMT7505){
		VPint(KSEG1ADDR(pcie_config_addr)) = (bus<<24 |devnum<<19|regnum);
	}else{	
		VPint(KSEG1ADDR(pcie_config_addr))=(type<<31|bus<<20 |devnum<<15|regnum);
	}	
		VPint(KSEG1ADDR(pcie_config_data))=value;
	return 0;
}
unsigned long int pcie_read_config_word(unsigned char type, unsigned char bus, unsigned char devnum, unsigned char regnum)
{
	if(isRT63165 || isRT63365 || isMT751020 || isMT7505){
		VPint(KSEG1ADDR(pcie_config_addr))=(bus<<24 |devnum<<19|regnum);		
	}else{	
		VPint(KSEG1ADDR(pcie_config_addr))=(type<<31|bus<<20|devnum<<15|regnum);
	}
	return VPint(KSEG1ADDR(pcie_config_data));
}

int pcieRegInitConfig(void)
{
 	unsigned int reg1_val, reg2_val;
 	unsigned int reg_val = 0;
 	int i = 0;
	int slot;
	int pci_device_exist;

 	if(isRT63165){
 		return -1;		
 	}

 	/* PCIe init module */
 	/* reset PCIe module */
 	/*
	 * From: TC/Li Fengbo 
	 * To: 'krammer' ; 'Marshall Yen \
	 * Cc: 'Liu, Shuenn-Ren' ; 'Graham Fan\
	 * Sent: Friday, May 22, 2009 2:49 PM
	 * Subject: new pof for software reboot
	 *
	 * Dear both,
	 * I have generated a new pof for software reboot, the pof file name is 
	 * software_reboot_20090522.pof
	 * It has been transported to Hsingchu, please help to check it
	 * Software Reset User Guide:
	 * After power on, there are two steps to active PCIe System
	 * 1 Wait for minimum 50us, Write ¡§1¡¨ to bit 29 of Register bfb0_0088, then
	 * 2 Wait for minimum 3.5us, write ¡§1¡¨ to bit 30 of Register bfb0_0088
	 * 
	 * Before do software reboot, 
	 * 1 Write ¡§0¡¨ to bit 29 and bit 30 of Register bfb0_0088
	 * Then reset for PCIE system is completed, you can reboot system
	 * Don¡¦t forget to release PCIe reset
	 * 2 Wait for minimum 50us , Write ¡§1¡¨ to bit 29 of bfb0_0088, then
	 * 3 Wait for minimum 3.5us, write ¡§1¡¨ to bit 30 of bfb0_0088
	 *
	 * Best regards
	 * Fengbo Li
	 *
	 */
 	/* pcie fixup start */
 	/* setup COMMAND register */
 	pcie_write_config_word(PCIE_TYPE_RC, PCIE_BUS_RC, PCIE_DEVNUM_0, 0x04, 0x00100007);

 	/* setup CACHE_LINE_SIZE register */
	if(isRT63365 || isMT751020 || isMT7505){
		pcie_write_config_word(PCIE_TYPE_RC, PCIE_BUS_RC, PCIE_DEVNUM_0, 0x0c, 0x00010000);
 	}else{	
 		pcie_write_config_word(PCIE_TYPE_RC, PCIE_BUS_RC, PCIE_DEVNUM_0, 0x0c/*PCI_CACHE_LINE_SIZE*/, 0x00000008);//duoduo_20090701
	}	
 	/* setup LATENCY_TIMER register */
 	/* pcie fixup end */
 	/*setup secondary bus number*/
	/*setup subordinate bus number*/
	pcie_write_config_word(PCIE_TYPE_RC, PCIE_BUS_RC, PCIE_DEVNUM_0, 0x18, 0x00010100);
 	/*setup I/O Base register*/
 	if(isRT63365 || isMT751020 || isMT7505){
 		pcie_write_config_word(PCIE_TYPE_RC, PCIE_BUS_RC, PCIE_DEVNUM_0, 0x30, 0x0);
 	}else{	
 		pcie_write_config_word(PCIE_TYPE_RC, PCIE_BUS_RC, PCIE_DEVNUM_0, 0x30, 0x0000FFFF);
	}
	pcie_write_config_word(PCIE_TYPE_RC, PCIE_BUS_RC, PCIE_DEVNUM_0, 0x1C, 0x000000F0);
 	/*setup memory base register*/
 	if(isRT63365 || isMT751020 || isMT7505){
		pcie_write_config_word(PCIE_TYPE_RC, PCIE_BUS_RC, PCIE_DEVNUM_0, 0x20, 0x1F701F70);
 	}else{	
 		pcie_write_config_word(PCIE_TYPE_RC, PCIE_BUS_RC, PCIE_DEVNUM_0, 0x20, 0x1F701F70);
	}	
 	/*setup prefetchable memory base register */
 	pcie_write_config_word(PCIE_TYPE_RC, PCIE_BUS_RC, PCIE_DEVNUM_0, 0x24, 0x0000FFF0);
 	/*setup I/O Base upper 16 bits register*/
	/*setup interrupt line register*/
	/*setup bridge control*/
	if(isRT63365 || isMT751020 || isMT7505){
 		pcie_write_config_word(PCIE_TYPE_RC, PCIE_BUS_RC, PCIE_DEVNUM_0, 0x3C, 0x00040119);
 	}else{	
 		pcie_write_config_word(PCIE_TYPE_RC, PCIE_BUS_RC, PCIE_DEVNUM_0, 0x3C, 0x0004010B);
	}	

 	/* pci register 0x10 config needed or not? Linos for L2H will configure it */
 	if(isRT63365 || isMT751020 || isMT7505){
		for(i = 0; i < 10; i++){
			reg1_val = pcie_read_config_word(PCIE_TYPE_DEV, PCIE_BUS_DEV, PCIE_DEVNUM_0, 0x0);
			
			tc_mdelay(1);
 			//reg2_val = pcie_read_config_word(PCIE_TYPE_DEV, PCIE_BUS_DEV, PCIE_DEVNUM_1, 0x0);
			//tc_mdelay(1);
		}	
		reg2_val = 0xffffffff;
		//Enable Interrupt
		VPint(0xbfb8000c) |= (1<<20);
 		//second band
 		//VPint(0xbfb8000c) |= (1<<21);
 	}else{	
 		do
 		{
			tc_mdelay(30);
 			reg_val = pcie_read_config_word(PCIE_TYPE_RC, PCIE_BUS_RC, PCIE_DEVNUM_0, 0xe0);
 			i++;
 		}
 		while((reg_val & 0x03f00000) != 0x00100000 && i <= 10);//check the if the dev has been link up
 		for(i = 0; i < 10; i++){
 			reg1_val = pcie_read_config_word(PCIE_TYPE_DEV, PCIE_BUS_DEV, PCIE_DEVNUM_0, 0x0);
			tc_mdelay(1);
 			reg2_val = pcie_read_config_word(PCIE_TYPE_DEV, PCIE_BUS_DEV, PCIE_DEVNUM_1, 0x0);
			tc_mdelay(1);
 		}
	}

 	if( (reg1_val != 0xffffffff) &&
 			( (reg1_val == ((NIC3090_PCIe_DEVICE_ID <<16) |NIC_PCI_VENDOR_ID)) //duoduo_20090702
 			  || (reg1_val == ((NIC3091_PCIe_DEVICE_ID <<16) |NIC_PCI_VENDOR_ID))
			  || (reg1_val == ((NIC3092_PCIe_DEVICE_ID <<16) |NIC_PCI_VENDOR_ID)) 
 			  || (reg1_val == ((NIC3390_PCIe_DEVICE_ID <<16) |NIC_PCI_VENDOR_ID))
 			  || (reg1_val == ((NIC5390_PCIe_DEVICE_ID <<16) |NIC_PCI_VENDOR_ID))//xyyou_20101111
 			  || (reg1_val == ((NIC539F_PCIe_DEVICE_ID <<16) |NIC_PCI_VENDOR_ID))
			  || (reg1_val == ((NIC5392_PCIe_DEVICE_ID <<16) |NIC_PCI_VENDOR_ID))) ){//xyyou wait to do
		pcie_write_config_word(PCIE_TYPE_DEV, PCIE_BUS_DEV, PCIE_DEVNUM_0, 0x04, 0x00100006);
 		if(isRT63365 || isMT751020 || isMT7505)
			pcie_write_config_word(PCIE_TYPE_DEV, PCIE_BUS_DEV, PCIE_DEVNUM_0, 0x10, 0x1F700000);
 		else	
 			pcie_write_config_word(PCIE_TYPE_DEV, PCIE_BUS_DEV, PCIE_DEVNUM_0, 0x10, PHYSADDR(PCI_DEVICE_MEM1)); 
 		pcie_write_config_word(PCIE_TYPE_DEV, PCIE_BUS_DEV, PCIE_DEVNUM_0, 0x14, 0); 
		pcie_write_config_word(PCIE_TYPE_DEV, PCIE_BUS_DEV, PCIE_DEVNUM_0, 0x18, 0); 
		pcie_write_config_word(PCIE_TYPE_DEV, PCIE_BUS_DEV, PCIE_DEVNUM_0, 0x1C, 0); 
		pcie_write_config_word(PCIE_TYPE_DEV, PCIE_BUS_DEV, PCIE_DEVNUM_0, 0x20, 0); 
		pcie_write_config_word(PCIE_TYPE_DEV, PCIE_BUS_DEV, PCIE_DEVNUM_0, 0x24, 0); 
		pcie_write_config_word(PCIE_TYPE_DEV, PCIE_BUS_DEV, PCIE_DEVNUM_0, 0x30, 0); 
		if(isRT63365 || isMT751020 || isMT7505)
			pcie_write_config_word(PCIE_TYPE_DEV, PCIE_BUS_DEV, PCIE_DEVNUM_0, 0x3C, 0x00000119);
		else	
			pcie_write_config_word(PCIE_TYPE_DEV, PCIE_BUS_DEV, PCIE_DEVNUM_0, 0x3C, 0x0000010B); 

		slot = PCIE_DEVNUM_0;		
		pci_device_exist++;
	}
	else if( (reg2_val != 0xffffffff) &&
			( (reg2_val == ((NIC3090_PCIe_DEVICE_ID <<16) |NIC_PCI_VENDOR_ID)) 
			  || (reg2_val == ((NIC3091_PCIe_DEVICE_ID <<16) |NIC_PCI_VENDOR_ID))
			  || (reg2_val == ((NIC3092_PCIe_DEVICE_ID <<16) |NIC_PCI_VENDOR_ID))
			  || (reg1_val == ((NIC3390_PCIe_DEVICE_ID <<16) |NIC_PCI_VENDOR_ID))
			  || (reg1_val == ((NIC5390_PCIe_DEVICE_ID <<16) |NIC_PCI_VENDOR_ID))
			  || (reg1_val == ((NIC539F_PCIe_DEVICE_ID <<16) |NIC_PCI_VENDOR_ID))
			  || (reg1_val == ((NIC5392_PCIe_DEVICE_ID <<16) |NIC_PCI_VENDOR_ID))) ){
 		slot=PCIE_DEVNUM_1;
 		pci_device_exist++;
 	}
	else{
		printf("no_pci_found error case\n");
 		return -1;
	}
	return slot;
}


void ledInit(void)
{
	unsigned int data = 0;
	PUCHAR baseAddr = (void *)0;
	unsigned long				csr_addr;
	csr_addr = 0xBF700000;
	baseAddr =  (PUCHAR)csr_addr;
	int i=0;

	RTMP_IO_READ32(baseAddr, GPIO_SWITCH, &data);
	RTMP_IO_READ32(baseAddr, GPIO_SWITCH, &data);//read twice for rt63365

	data |= 0xffff;

	RTMP_IO_WRITE32(baseAddr, GPIO_SWITCH, data);
}

static int wifiGpioLedAct(int action ,int gpio){
	unsigned int   	gpioctl;
	PUCHAR baseAddr = (void *)0;
	unsigned long				csr_addr;
	
	csr_addr = 0xBF700000;
	baseAddr =  (PUCHAR)csr_addr;

	if (gpio < 0 || gpio > 14)
		return -1;
	
	RTMP_IO_READ32(baseAddr, GPIO_CTRL_CFG, &gpioctl);
	RTMP_IO_READ32(baseAddr, GPIO_CTRL_CFG, &gpioctl); //read twice for rt63365


	switch(action){
		case	WLAN_LED_OFF:
			if(gpio > 7){
				gpioctl |= ((1<<(gpio-8))<<16);
			}else{
				gpioctl |= (1<<gpio);
			}	
			break;
		case 	WLAN_LED_ON:
			if(gpio > 7){
				gpioctl &= ~((1<<(gpio-8))<<16);
			}else{	
				gpioctl &= ~(1<<gpio);
			}	
			break;	
		case 	WLAN_LED_OEN:
			if(gpio > 7){
				gpioctl &= ~((1<<(gpio-8))<<24);
			}else{	
				gpioctl &= ~((1<<gpio)<<8);
			}	
			break;
		case 	WLAN_LED_IEN:
			if(gpio > 7){
				gpioctl &=  ((1<<(gpio-8))<<24);
			}else{	
				gpioctl |=  ((1<<gpio)<<8);
			}	
			break;
		default:
			break;		
	}	

	RTMP_IO_WRITE32(baseAddr, GPIO_CTRL_CFG, gpioctl);
	
	return 0;
}

#define WLAN_GPIO_MAP_OFFSET 	64

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
void ledTurnOn(uint8 led_no)
{
	if((led_no >= WLAN_GPIO_MAP_OFFSET) && (led_no <= 14 + WLAN_GPIO_MAP_OFFSET) )
	{
		if(pcieOKFlag == 0)
			LED_WIFI_ON(led_no - WLAN_GPIO_MAP_OFFSET);
	}
	else
	{
		LED_ON(led_no);
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
	if((led_no >= WLAN_GPIO_MAP_OFFSET) && (led_no <= 14 + WLAN_GPIO_MAP_OFFSET) )
	{
		if(pcieOKFlag == 0)
			LED_WIFI_OFF(led_no - WLAN_GPIO_MAP_OFFSET);
	}
	else
	{
		LED_OFF(led_no);
	}
}

/*______________________________________________________________________________
**	ledTurnOen
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
void ledTurnOen(uint8 led_no)
{
	if((led_no >= WLAN_GPIO_MAP_OFFSET) && (led_no <= 14 + WLAN_GPIO_MAP_OFFSET) )
	{
		if(pcieOKFlag == 0)
			LED_WIFI_OEN(led_no - WLAN_GPIO_MAP_OFFSET);
	}
	else
	{
		LED_OEN(led_no);
	}
}


