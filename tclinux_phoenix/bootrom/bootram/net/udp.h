#ifndef __UDP_H
#define __UDP_H

#define TFTP 0x45

/*
 *	This structure defines an udp header.
 */

struct udphdr {
	unsigned short	source;
	unsigned short	dest;
	unsigned short	len;
	unsigned short	check;
}__attribute__ ((packed));

int udp_init(void);
int udp_rcv_packet(sk_buff *skb);
int udp_send(sk_buff *skb, unsigned long ip,
	unsigned short source, unsigned short dest);
void udp_skb_reserve(sk_buff *skb);
unsigned short udp_get_source_port(sk_buff *skb);

#endif /* __UDP_H */
