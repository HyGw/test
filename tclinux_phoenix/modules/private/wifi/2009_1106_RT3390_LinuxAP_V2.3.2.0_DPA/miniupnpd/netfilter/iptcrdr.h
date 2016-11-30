/* $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/modules/private/wifi/2009_1106_RT3390_LinuxAP_V2.3.2.0_DPA/miniupnpd/netfilter/iptcrdr.h#1 $ */
/* MiniUPnP project
 * http://miniupnp.free.fr/ or http://miniupnp.tuxfamily.org/
 * (c) 2006 Thomas Bernard 
 * This software is subject to the conditions detailed
 * in the LICENCE file provided within the distribution */

#ifndef __IPTCRDR_H__
#define __IPTCRDR_H__

#include "../commonrdr.h"

int
add_redirect_rule2(const char * ifname, unsigned short eport,
                   const char * iaddr, unsigned short iport, int proto,
				   const char * desc);

int
add_filter_rule2(const char * ifname, const char * iaddr,
                 unsigned short eport, unsigned short iport,
                 int proto, const char * desc);

int
delete_redirect_and_filter_rules(unsigned short eport, int proto);

/* for debug */
int
list_redirect_rule(const char * ifname);

int
addnatrule(int proto, unsigned short eport,
               const char * iaddr, unsigned short iport);

int
add_filter_rule(int proto, const char * iaddr, unsigned short iport);

#endif

