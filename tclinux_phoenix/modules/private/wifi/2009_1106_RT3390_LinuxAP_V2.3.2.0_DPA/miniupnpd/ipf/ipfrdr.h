/* $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/modules/private/wifi/2009_1106_RT3390_LinuxAP_V2.3.2.0_DPA/miniupnpd/ipf/ipfrdr.h#1 $ */
/* MiniUPnP project
 * http://miniupnp.free.fr/ or http://miniupnp.tuxfamily.org/
 * (c) 2006-2007 Thomas Bernard 
 * This software is subject to the conditions detailed
 * in the LICENCE file provided within the distribution */
#ifndef __IPFRDR_H__
#define __IPFRDR_H__

#include "../commonrdr.h"

int
add_redirect_rule2(const char * ifname, unsigned short eport,
                   const char * iaddr, unsigned short iport, int proto,
				   const char * desc);

int
add_filter_rule2(const char * ifname, const char * iaddr,
                 unsigned short eport, unsigned short iport,
				 int proto, const char * desc);
 

/* get_redirect_rule() gets internal IP and port from
 * interface, external port and protocl
 */
#if 0
int
get_redirect_rule(const char * ifname, unsigned short eport, int proto,
                  char * iaddr, int iaddrlen, unsigned short * iport,
                  char * desc, int desclen,
                  u_int64_t * packets, u_int64_t * bytes);

int
get_redirect_rule_by_index(int index,
                           char * ifname, unsigned short * eport,
                           char * iaddr, int iaddrlen, unsigned short * iport,
                           int * proto, char * desc, int desclen,
                           u_int64_t * packets, u_int64_t * bytes);
#endif

/* delete_redirect_rule()
 */
int
delete_redirect_rule(const char * ifname, unsigned short eport, int proto);

/* delete_filter_rule()
 */
int
delete_filter_rule(const char * ifname, unsigned short eport, int proto);

int
clear_redirect_rules(void);

#endif

