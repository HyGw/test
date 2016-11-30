/* $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/modules/private/wifi/2009_1106_RT3390_LinuxAP_V2.3.2.0_DPA/miniupnpd/testgetifaddr.c#1 $ */
/* MiniUPnP project
 * http://miniupnp.free.fr/ or http://miniupnp.tuxfamily.org/
 * (c) 2006-2008 Thomas Bernard 
 * This software is subject to the conditions detailed
 * in the LICENCE file provided within the distribution */
#include <stdio.h>
#include "getifaddr.h"

int main(int argc, char * * argv) {
	char addr[16];
	if(argc < 2) {
		fprintf(stderr, "Usage:\t%s interface_name\n", argv[0]);
		return 1;
	}
	if(getifaddr(argv[1], addr, sizeof(addr)) < 0) {
		fprintf(stderr, "Cannot get address for interface %s.\n", argv[1]);
		return 1;
	}
	printf("Interface %s has IP address %s.\n", argv[1], addr);
	return 0;
}
