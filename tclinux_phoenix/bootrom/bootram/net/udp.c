#include "skbuff.h"
#include "eth.h"
#include "ip.h"
#include "udp.h"
#include "tftp.h"

#include "osdep.h"
#if defined(TCSUPPORT_CT_BOOTLOADER_UPGRADE)
extern int authed;
#endif

int udp_init(void)
{
	return 0;
}

int udp_rcv_packet(sk_buff *skb)
{
	struct udphdr *udp_hdr = (struct udphdr *)(skb->data);

	/* record udp header pointer */
	skb->udp_hdr = udp_hdr;

	skb->len = ntohs(udp_hdr->len);
	skb_pull(skb, sizeof(struct udphdr));

	if (ntohs(udp_hdr->dest) == TFTP)
	{
#if defined(TCSUPPORT_CT_BOOTLOADER_UPGRADE)
	 	if (authed)
#endif
			tftp_rcv_packet(skb);
	}

	return 0;
}

int udp_send(sk_buff *skb, unsigned long ip,
	unsigned short source, unsigned short dest)
{
	struct udphdr *udp_hdr;

	udp_hdr = (struct udphdr *)skb_push(skb, sizeof(struct udphdr));
	udp_hdr->source = htons(source);
	udp_hdr->dest = htons(dest);
	udp_hdr->len = htons(12);
	udp_hdr->check = 0;

	ip_send(skb, ip, UDP);

	return 0;
}

void udp_skb_reserve(sk_buff *skb)
{
	ip_skb_reserve(skb);
	skb_reserve(skb, sizeof(struct udphdr));
}

unsigned short udp_get_source_port(sk_buff *skb)
{
	struct udphdr *udp_hdr;
	
	udp_hdr = (struct udphdr *)(skb->udp_hdr);
	return ntohs(udp_hdr->source);
}
