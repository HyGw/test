/*
 * define path names
 *
 * $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/apps/public/ppp-2.4.1.pppoe4.orig/pppd/pathnames.h#1 $
 */

#ifdef HAVE_PATHS_H
#include <paths.h>

#else /* HAVE_PATHS_H */
#ifndef _PATH_VARRUN
#define _PATH_VARRUN 	"/etc/ppp/"
#endif
#define _PATH_DEVNULL	"/dev/null"
#endif /* HAVE_PATHS_H */

#ifndef _ROOT_PATH
#define _ROOT_PATH
#endif

#define _PATH_UPAPFILE 	 _ROOT_PATH "/etc/ppp/pap-secrets"
#define _PATH_CHAPFILE 	 _ROOT_PATH "/etc/ppp/chap-secrets"
#define _PATH_SYSOPTIONS _ROOT_PATH "/etc/ppp/options"
#define _PATH_IPUP	 _ROOT_PATH "/etc/ppp/ip-up"
#define _PATH_IPDOWN	 _ROOT_PATH "/etc/ppp/ip-down"
#define _PATH_AUTHUP	 _ROOT_PATH "/etc/ppp/auth-up"
#define _PATH_AUTHDOWN	 _ROOT_PATH "/etc/ppp/auth-down"
#define _PATH_TTYOPT	 _ROOT_PATH "/etc/ppp/options."
#define _PATH_CONNERRS	 _ROOT_PATH "/etc/ppp/connect-errors"
#define _PATH_PEERFILES	 _ROOT_PATH "/etc/ppp/peers/"
#ifndef TCSUPPORT_DNSEACHPVC
#define _PATH_RESOLV	 _ROOT_PATH "/etc/ppp/resolv.conf"
#else
#define _PATH_RESOLV	 _ROOT_PATH "/etc/ppp/resolv_%s.conf"
#endif

#define _PATH_USEROPT	 ".ppprc"

#ifdef INET6
#define _PATH_IPV6UP     _ROOT_PATH "/etc/ppp/ipv6-up"
#define _PATH_IPV6DOWN   _ROOT_PATH "/etc/ppp/ipv6-down"
#if defined(TCSUPPORT_CT)
#define _PATH_IPV6UP_PRE     _ROOT_PATH "/etc/ppp/ipv6-up_pre"
#if defined(TCSUPPORT_CT_PPPOE_IPV6_EMULATOR)
#define _PATH_EMU_IPV6UP _ROOT_PATH "/etc/ppp/emu_ipv6-up"
#define _PATH_EMU_IPV6UP_PRE _ROOT_PATH "/etc/ppp/emu_ipv6-up_pre"
#if defined(TCSUPPORT_CT_PPPERRCODE_ENHANCE)
#define ERROR_OUT_OF_RESOURCES "ERROR_SERVER_OUT_OF_RESOURCES_IPv6"
#else
#define ERROR_OUT_OF_RESOURCES "ERROR_SERVER_OUT_OF_RESOURCES"
#endif
#endif
#endif
#endif

#ifdef IPX_CHANGE
#define _PATH_IPXUP	 _ROOT_PATH "/etc/ppp/ipx-up"
#define _PATH_IPXDOWN	 _ROOT_PATH "/etc/ppp/ipx-down"
#endif /* IPX_CHANGE */

#ifdef __STDC__
#define _PATH_PPPDB	_ROOT_PATH _PATH_VARRUN "pppd.tdb"
#else /* __STDC__ */
#ifdef HAVE_PATHS_H
#define _PATH_PPPDB	"/var/run/pppd.tdb"
#else
#define _PATH_PPPDB	"/etc/ppp/pppd.tdb"
#endif
#endif /* __STDC__ */

#ifdef PLUGIN
#define _PATH_PLUGIN	"/usr/lib/pppd/" VERSION
#endif /* PLUGIN */
