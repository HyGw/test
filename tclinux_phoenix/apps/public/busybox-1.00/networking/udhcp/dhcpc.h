/* dhcpc.h */
#ifndef _DHCPC_H
#define _DHCPC_H

#define DEFAULT_SCRIPT  "/usr/share/udhcpc/default.script"

/* allow libbb_udhcp.h to redefine DEFAULT_SCRIPT */
#include "libbb_udhcp.h"

#define INIT_SELECTING	0
#define REQUESTING	1
#define BOUND		2
#define RENEWING	3
#define REBINDING	4
#define INIT_REBOOT	5
#define RENEW_REQUESTED 6
#define RELEASED	7
#ifdef DHCP_OPT121
#define OPT_CLASSLESS_STATIC_ROUTE 0x79
#endif

#if defined(TCSUPPORT_CT_DHCPC_OPTION125)
struct option125_itms_t{
	uint8_t enable;
	uint8_t type;
	uint8_t subopt_code;
	char subopt_data[32];
	char value[256];
};
#endif
struct client_config_t {
	char foreground;		/* Do not fork */
	char quit_after_lease;		/* Quit after obtaining lease */
	char abort_if_no_lease;		/* Abort if no lease */
	char background_if_no_lease;	/* Fork to background if no lease */
	char *interface;		/* The name of the interface to use */
	char *pidfile;			/* Optionally store the process ID */
	char *script;			/* User script to run at dhcp events */
	uint8_t *clientid;		/* Optional client id to use */
	uint8_t *hostname;		/* Optional hostname to use */
	int ifindex;			/* Index number of the interface to use */
#if defined(TCSUPPORT_CT_DHCPC_OPTION60)
	uint8_t dhcp60;			/* DHCP option60 enable */
	char *dhcp60_option;		/* DHCP option60 packet content */
#endif

#if defined(TCSUPPORT_CT_DHCPC_OPTION125)
#if 0
	uint8_t dhcp125c;		/* DHCP option125 enterprise code checking enable*/
	uint8_t dhcp125s;		/* DHCP option125 string checking enable*/
	uint32_t dhcp125Code;		/* DHCP option125 enterprise code */
	char *dhcp125String;		/* DHCP option125 string*/
#else
	struct option125_itms_t option125_info[4];
#endif
#endif
#if defined(TCSUPPORT_CT_IPOE_EMULATOR)
	char ipoe_if; 	/* IPoE Emulator */
#endif
	uint8_t arp[6];			/* Our arp address */
};

extern struct client_config_t client_config;


#endif
