#ifndef __SKBUFF_H
#define __SKBUFF_H

typedef struct sk_buff_s {
	unsigned char buf[2000];
	unsigned int truesize;			/* Buffer size 				*/

	unsigned char *data;			/* Data head pointer		*/
	unsigned char *ip_hdr;			/* IP header pointer		*/
	unsigned char *udp_hdr;			/* UDP header pointer		*/
	unsigned int used;
	unsigned int len;				/* Length of actual data	*/
} sk_buff;

int skb_init(void);
sk_buff *alloc_skb(unsigned int size);
void free_skb(sk_buff *skb);
unsigned char *skb_put(sk_buff *skb, unsigned int len);
unsigned char *skb_push(sk_buff *skb, unsigned int len);
unsigned char *skb_pull(sk_buff *skb, unsigned int len);
void skb_reserve(sk_buff *skb, unsigned int len);

#endif	/* __SKBUFF_H */
