#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/ioport.h>
#include <linux/types.h>
#include <linux/init.h>
#include <asm/tc3162/tc3162.h>

/* ADSL RTS dump function */
void macSend(uint32 chanId, struct sk_buff *skb)
{
}
EXPORT_SYMBOL(macSend);

void TCConsole(uint8 mode)
{
}
EXPORT_SYMBOL(TCConsole);

void uartMacPutchar(int ch)
{
}
EXPORT_SYMBOL(uartMacPutchar);

uint32 GetIpAddr(void)
{
	return 0;
}
EXPORT_SYMBOL(GetIpAddr);

uint8 *GetMacAddr(void)
{
	uint8 *flash_mac_addr = (uint8 *)0xbfc0ff48;
	static uint8 macAddr[7];
	
	if( (flash_mac_addr[0] == 0) && (flash_mac_addr[1] == 0) && (flash_mac_addr[2] == 0) &&
		(flash_mac_addr[3] == 0) && (flash_mac_addr[4] == 0) && (flash_mac_addr[5] == 0) ){
	    	macAddr[0] = 0x00;
	        macAddr[1] = 0xaa;
	        macAddr[2] = 0xbb;
	        macAddr[3] = 0x01;
	        macAddr[4] = 0x23;
	        macAddr[5] = 0x45;
	}else{
	        memcpy(macAddr, flash_mac_addr, 6);
	}

    macAddr[6] = 0x0;
    return macAddr;
}
EXPORT_SYMBOL(GetMacAddr);

static int __init tc3162_mac_init(void)
{
	return 0;
}
 
static void __exit tc3162_mac_exit(void){
}
module_init(tc3162_mac_init);
module_exit(tc3162_mac_exit);
