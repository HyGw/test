/* $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/modules/private/wifi/2009_1106_RT3390_LinuxAP_V2.3.2.0_DPA/miniupnpd/miniupnpdtypes.h#1 $ */
/* MiniUPnP project
 * http://miniupnp.free.fr/ or http://miniupnp.tuxfamily.org/
 * (c) 2006-2007 Thomas Bernard
 * This software is subject to the conditions detailed
 * in the LICENCE file provided within the distribution */
#ifndef __MINIUPNPDTYPES_H__
#define __MINIUPNPDTYPES_H__

#include "config.h"
#include <netinet/in.h>

/* structure for storing lan addresses
 * with ascii representation and mask */
struct lan_addr_s {
	char str[16];	/* example: 192.168.0.1 */
	struct in_addr addr, mask;	/* ip/mask */
#ifdef MULTIPLE_EXTERNAL_IP
	char ext_ip_str[16];
	struct in_addr ext_ip_addr;
#endif
};

#endif
