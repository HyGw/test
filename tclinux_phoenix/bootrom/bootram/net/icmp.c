#include "skbuff.h"
#include "eth.h"
#include "ip.h"
#include "icmp.h"

#include "osdep.h"

static unsigned long client_ip;

static int icmp_send_echo(sk_buff *skb)
{
	struct icmphdr *icmp_hdr, *icmp_echo;   
   	unsigned int tmp, sum, i, cnt;          
	sk_buff *echo_skb;
                        
   	cnt = skb->len;         
	icmp_hdr = (struct icmphdr *)(skb->data);
	if (in_csum(skb->data, skb->len) != 0)
		prom_printf("rcv icmp checksum err packet\n");

   	echo_skb = alloc_skb(ETH_FRAME_LEN);
   	ip_skb_reserve(echo_skb);            //reserve for eth and ip stack  
   	icmp_echo = (struct icmphdr *)skb_put(echo_skb, cnt);        
        
	memcpy(echo_skb->data, icmp_hdr, skb->len);
	icmp_echo->type  = 0;
   	icmp_echo->code  = 0;   
   	icmp_echo->check = 0;   
   	icmp_echo->check = in_csum(echo_skb->data, echo_skb->len);

   	ip_send(echo_skb, client_ip, ICMP);

   	return 0;
}

static int icmp_rcv_echo(sk_buff *skb)
{
   	client_ip = ip_get_source_ip(skb);
                
   	icmp_send_echo(skb);
        
   	return 0;
}

int icmp_rcv_packet(sk_buff *skb)
{
   	struct icmphdr *icmp_hdr = (struct icmphdr *)(skb->data);
        
	switch(icmp_hdr->type) {
   		case 8:
       		icmp_rcv_echo(skb);
			break;
	}
        
   	return 0;
}

