#ifndef __PARSE_TRAFFIC_H__
#define __PARSE_TRAFFIC_H__
#include <linux/if_ether.h>
#include <linux/skbuff.h>
#include <linux/if_vlan.h>

#define IPV6_LEN   16
#define IPV6_HEAD  8
#define IPV4_LEN   4

#define UDP                     0x11
#define TCP                     0x06
#define PPP_IP_PROTOCOL         0x0021  /* Internet Protocol */
#define PPP_IPV6_PROTOCOL		0x0057	/*Internet V6 Protocol*/
#define DHCPV6_CLI_PORT         546
#define DHCPV6_SER_PORT         547
#define DHCP_CLI_PORT			67
#define DHCP_SER_PORT			68


typedef  unsigned char  uint8;
typedef  unsigned short uint16; 
typedef  unsigned int   uint32; 
typedef  unsigned long long uint64;

typedef struct _vlan_info_
{
	unsigned short vlan_id;
	unsigned char  vlan_prio;
	unsigned short vlan_proto;
}ecn_vlan_info_t, *pt_ecn_vlan_info;

typedef struct {
	struct ethhdr ethaddr;
}mac_basic_t;

typedef struct {
	uint8 skip[6];
	uint16 proto;
}ppp_hdr_t;

typedef struct {
	uint32 version:4,
		   traffic:8,
		   flowlbl:20;
	uint16 payload;
	uint8  nexthr;
	uint8  hoplimit;
	uint8  saddr[IPV6_LEN];
	uint8  daddr[IPV6_LEN];	   
}ipv6hdr_t;

typedef struct {
	uint8 type;
}dhcpv6hdr_t;

#endif

