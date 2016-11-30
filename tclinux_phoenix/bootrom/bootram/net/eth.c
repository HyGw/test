#include <flashhal.h>
#include "skbuff.h"
#include "eth.h"
#include "mac.h"
 
#include "osdep.h"

extern __inline__ void
__sti(void)
{
	__asm__ __volatile__(
		".set\tpush\n\t"
		".set\treorder\n\t"
		".set\tnoat\n\t"
		"mfc0\t$1,$12\n\t"
		"ori\t$1,0x1f\n\t"
		"xori\t$1,0x1e\n\t"
		"mtc0\t$1,$12\n\t"
		".set\tpop\n\t"
		: /* no outputs */
		: /* no inputs */
		: "$1", "memory");
}

/*
 * For cli() we have to insert nops to make shure that the new value
 * has actually arrived in the status register before the end of this
 * macro.
 * R4000/R4400 need three nops, the R4600 two nops and the R10000 needs
 * no nops at all.
 */
extern __inline__ void
__cli(void)
{
	__asm__ __volatile__(
		".set\tpush\n\t"
		".set\treorder\n\t"
		".set\tnoat\n\t"
		"mfc0\t$1,$12\n\t"
		"ori\t$1,1\n\t"
		"xori\t$1,1\n\t"
		".set\tnoreorder\n\t"
		"mtc0\t$1,$12\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		".set\tpop\n\t"
		: /* no outputs */
		: /* no inputs */
		: "$1", "memory");
}

#  define sti() __sti()
#  define cli() __cli()

unsigned char *local_eth_addr;
extern void pause(int ms);
/*_____________________________________________________________________________
**      function name: eth_gpioReset
**      descriptions:
**       Accroding the mi.conf gpio value to reset lan .
**	   
**      parameters:
**         none
**             
**      global:
**         None
**             
**      return:
**           None
**	     
**      call:
**   	     None
**      		
**      revision:
**      1. Here 2010/01/14
**____________________________________________________________________________
*/
void
eth_gpioReset(void)
{
	/*Get the Ethernet GPIO Number*/	/* frankliao modify 20101208 */
	uint8 ether_gpio= READ_FLASH_BYTE(FLASH_ETHER_GPIO);

	LED_OEN(ether_gpio);
	LED_ON(ether_gpio);
	pause(100);
	LED_OFF(ether_gpio);
	/*Must waiting for 0.6 sec after reset*/
	pause(600);
}/*end lanGpioReset*/
/*_____________________________________________________________________________
**      function name: eth_latsch_up
**      descriptions:
**        To avoid large current and use the ethernet gpio to reset TC2206 switch
**		  ic.
**	   
**      parameters:
**         none
**             
**      global:
**         None
**             
**      return:
**           None
**	     
**      call:
**   	     None
**      		
**      revision:
**      1. Here 2010/01/14
**____________________________________________________________________________
*/
void
eth_latsch_up(void){
	if (isTC3162U) {
		VPint(CR_AHB_SSR) |= (1<<11)|(1<<10)|(1<<9)|(1<<8);
		eth_gpioReset();
	}
}/*end eth_latsch_up*/

int eth_init(unsigned char *mac_addr)
{
	local_eth_addr = mac_addr;
	return tc3162_eth_init(local_eth_addr);
}

int eth_send(sk_buff *skb, unsigned char *dest_addr, unsigned short proto)
{
	struct ethhdr *eth_hdr;
	int ret;

	eth_hdr = (struct ethhdr *)skb_push(skb, ETH_HLEN);

	memcpy(eth_hdr->h_dest, dest_addr, ETH_ALEN);
	memcpy(eth_hdr->h_source, local_eth_addr, ETH_ALEN);
	eth_hdr->h_proto = htons(proto);

	if (skb->len < ETH_ZLEN) {
		/* Pad the packet out to the minimum */
		memset(skb->data + skb->len, 0, ETH_ZLEN - skb->len);
		skb->len = ETH_ZLEN;
	}		

	cli();
	ret = tc3162_eth_send(skb);
	sti();
	return ret;
}

int eth_rcv(sk_buff *skb)
{	
	struct ethhdr *eth_hdr = (struct ethhdr *)(skb->data);
	int i;

	skb_pull(skb, sizeof(struct ethhdr));

#if 0
	prom_printf("data=%x\n", skb->data);
	prom_printf("sizeof(ethhdr)=%d \n", sizeof(struct ethhdr));
	prom_printf("h_dest= \n");
	for (i = 0; i < ETH_ALEN; i++)
		prom_printf("%x ", eth_hdr->h_dest[i]);
	prom_printf("\n");
	prom_printf("h_source= \n");
	for (i = 0; i < ETH_ALEN; i++)
		prom_printf("%x ", eth_hdr->h_source[i]);
	prom_printf("\n");
	prom_printf("proto= %X\n", eth_hdr->h_proto);
#endif

	switch(eth_hdr->h_proto){
	case ETH_P_ARP:
		//prom_printf("ETH_P_ARP proto= %X\n", eth_hdr->h_proto);
		arp_rcv_packet(skb);
		break;
	case ETH_P_IP:
		//prom_printf("ETH_P_IP proto= %X\n", eth_hdr->h_proto);
		ip_rcv_packet(skb);
		break;
	default:
		break;
	}
	free_skb(skb);
	
	return 0;	
}

void eth_skb_reserve(sk_buff *skb)
{
	skb_reserve(skb, ETH_HLEN);
}

void eth_exit(void)
{
	tc3162_eth_exit();
}
