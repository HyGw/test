/* $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/modules/private/wifi/2009_1106_RT3390_LinuxAP_V2.3.2.0_DPA/miniupnpd/natpmp.h#1 $ */
/* MiniUPnP project
 * author : Thomas Bernard
 * website : http://miniupnp.free.fr/ or http://miniupnp.tuxfamily.org/
 */
#ifndef __NATPMP_H__
#define __NATPMP_H__

#define NATPMP_PORT (5351)

int OpenAndConfNATPMPSocket();

void ProcessIncomingNATPMPPacket(int s);

int ScanNATPMPforExpiration();

int CleanExpiredNATPMP();

#endif

