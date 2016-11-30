/*
 *  ebt_ip
 *
 *	Authors:
 *	Bart De Schuymer <bart.de.schuymer@pandora.be>
 *
 *  April, 2002
 *
 *  Changes:
 *    added ip-sport and ip-dport
 *    Innominate Security Technologies AG <mhopf@innominate.com>
 *    September, 2002
 */

#ifndef __LINUX_BRIDGE_EBT_IP_H
#define __LINUX_BRIDGE_EBT_IP_H

#define EBT_IP_SOURCE 0x01
#define EBT_IP_DEST 0x02
#define EBT_IP_TOS 0x04
#define EBT_IP_PROTO 0x08
#define EBT_IP_SPORT 0x10
#define EBT_IP_DPORT 0x20
#if 1  /*Rodney_20090724*/
#define EBT_IP_IPP 0x40
#define EBT_IP_DSCP 0x80
#define EBT_IP_MASK (EBT_IP_SOURCE | EBT_IP_DEST | EBT_IP_TOS | EBT_IP_PROTO |\
 EBT_IP_SPORT | EBT_IP_DPORT | EBT_IP_IPP | EBT_IP_DSCP)
#else
#define EBT_IP_MASK (EBT_IP_SOURCE | EBT_IP_DEST | EBT_IP_TOS | EBT_IP_PROTO |\
 EBT_IP_SPORT | EBT_IP_DPORT )
#endif
#define EBT_IP_MATCH "ip"

/* the same values are used for the invflags */
struct ebt_ip_info
{
/* xyzhu_nj_20100413*/
#if 1
	__be32 saddr[2];
	__be32 daddr[2];
	__be32 smsk[2];
	__be32 dmsk[2];
#else
	__be32 saddr;
	__be32 daddr;
	__be32 smsk;
	__be32 dmsk;
#endif

	uint8_t  tos;
#if 1 /*Rodney_20090724*/
	uint8_t  protocol[2];
#else
	uint8_t  protocol;
#endif
	uint8_t  bitmask;
	uint8_t  invflags;
	uint16_t sport[2];
	uint16_t dport[2];
#if 1  /*Rodney_20090724*/
	uint8_t  ipp[2];
	uint8_t  dscp[2];
#endif
};

#endif
