/* $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/modules/private/wifi/2009_1106_RT3390_LinuxAP_V2.3.2.0_DPA/miniupnpd/daemonize.h#1 $ */
/* MiniUPnP project
 * http://miniupnp.free.fr/ or http://miniupnp.tuxfamily.org/
 * (c) 2006 Thomas Bernard 
 * This software is subject to the conditions detailed
 * in the LICENCE file provided within the distribution */

#ifndef __DAEMONIZE_H__
#define __DAEMONIZE_H__

#include "config.h"

#ifndef USE_DAEMON
/* daemonize()
 * "fork" to background, detach from terminal, etc... 
 * returns: pid of the daemon, exits upon failure */
int
daemonize(void);
#endif

/* writepidfile()
 * write the pid to a file */
int
writepidfile(const char * fname, int pid);

/* checkforrunning()
 * check for another instance running
 * returns: 0 only instance
 *          -1 invalid filename
 *          -2 another instance running  */
int
checkforrunning(const char * fname);

#endif

