/* dhcpd.c
 *
 * udhcp Server
 * Copyright (C) 1999 Matthew Ramsay <matthewr@moreton.com.au>
 *			Chris Trew <ctrew@moreton.com.au>
 *
 * Rewrite by Russ Dill <Russ.Dill@asu.edu> July 2001
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <time.h>
#include <sys/time.h>

#include "dhcpd.h"
#include "arpping.h"
#include "socket.h"
#include "options.h"
#include "files.h"
#include "serverpacket.h"
#include "common.h"
#include "signalpipe.h"
#include "static_leases.h"

#ifdef BBU_SOC
#include "../../../lib/libtcapi.h"

extern int dhcp_arp_timeout_ms;
#endif

/* define */
/* globals */
struct dhcpOfferedAddr *leases;
struct server_config_t server_config;//current dhcp server
struct server_config_t dhcp_config;//main pool dhcp sever
#ifdef TCSUPPORT_FON_V2
int IsFonDhcpServer = 0;
#endif

extern int leaseNumber;

#ifdef DHCP_PROFILE
struct server_config_t option60_config;//option 60 pool dhcp server
struct Option60_Param_s gOption60_Param;
uint8_t gOption60IsMatched = 0;
struct Option240_Param_s gOption240_Param;
#endif
unsigned long gTotalMaxLeases = 0;//total max leases of all pools
#if defined (CWMP) && defined(TR111)
Tr111_Param_t gTR111_Parm;
#endif

#if defined(TCSUPPORT_CT_IPTV_NODE)
STB_Param_t gSTB_Parm;
#endif


#if defined(TCSUPPORT_CT_E8GUI)
int udhcpd_parser_option60(uint8_t *vendorID){
	
#if defined(TCSUPPORT_CUC)
	char enterprise_num[2] = {0x00, 0x01};	//unicom enterprise number
#else
	char enterprise_num[2] = {0x00, 0x00};	//ctc enterprise number
#endif

	gOption60IsMatched = 0;//not match
	memcpy(&server_config, &option60_config, sizeof(struct server_config_t));
	memset(&option60_config, 0, sizeof(struct server_config_t));
	gOption60_Param.dhcp_opt60_vid = NULL;

	if((vendorID[0] == enterprise_num[0]) && (vendorID[1] == enterprise_num[1])){
	tcdbg_printf("\nudhcpd_parser_option60, enterprisenum matched");
	read_config(DHCPD_OPTION_STB_CONF_FILE);
	if (strstr((char *)& vendorID[2], gOption60_Param.dhcp_opt60_vid) != NULL)
	{
		gOption60IsMatched = 2;
			tcdbg_printf("\nget device is STB");
	}
	if(!gOption60IsMatched){
		memset(&option60_config, 0, sizeof(struct server_config_t));
		gOption60_Param.dhcp_opt60_vid = NULL;
		read_config(DHCPD_OPTION_PHONE_CONF_FILE);
		if (strstr((char *)& vendorID[2], gOption60_Param.dhcp_opt60_vid) != NULL)
		{
			gOption60IsMatched = 3;
				tcdbg_printf("\nget device is phone");
		}
	}
	if(!gOption60IsMatched){
		memset(&option60_config, 0, sizeof(struct server_config_t));
		gOption60_Param.dhcp_opt60_vid = NULL;
		read_config(DHCPD_OPTION_CAMERA_CONF_FILE);
		if (strstr((char *)& vendorID[2], gOption60_Param.dhcp_opt60_vid) != NULL)
		{
			gOption60IsMatched = 4;
				tcdbg_printf("\nget device is camera");
		}
	}
	if(!gOption60IsMatched){
		memset(&option60_config, 0, sizeof(struct server_config_t));
		gOption60_Param.dhcp_opt60_vid = NULL;
		read_config(DHCPD_OPTION_HGW_CONF_FILE);
		if (strstr((char *)& vendorID[2], gOption60_Param.dhcp_opt60_vid) != NULL)
		{
			gOption60IsMatched = 5;
				tcdbg_printf("\nget device is HGW");
		}
	}
	}
	//default is computer
	if(!gOption60IsMatched){
		memset(&option60_config, 0, sizeof(struct server_config_t));
		read_config(DHCPD_OPTION_CONF_FILE);
		tcdbg_printf("\nget device is PC");
		gOption60IsMatched = 1;
	}
	
	option60_config.lease = server_config.lease;
	option60_config.max_leases = server_config.max_leases;
	option60_config.ifindex = server_config.ifindex;
	option60_config.server = server_config.server;
	memcpy(option60_config.arp, server_config.arp, sizeof(server_config.arp));
	memset(&server_config, 0, sizeof(struct server_config_t));
}
#endif
#if defined(TCSUPPORT_CT_ACCESSLIMIT)
void udhcpd_set_dev_info(unsigned int active, unsigned char *addr)
{
	char cmdbuf[128] = {0};
	char dev_type[16] = {0};
	
	//tcdbg_printf("\nudhcpd_set_dev_info addr= %02X:%02X:%02X:%02X:%02X:%02X\n",addr[0],addr[1],addr[2],addr[3],addr[4],addr[5]);
	switch(gOption60IsMatched){
		case 1:
			strcpy(dev_type, "computer");
			break;
		case 2:
			strcpy(dev_type, "stb");
			break;
		case 3:
			strcpy(dev_type, "phone");
			break;
		case 4:
			strcpy(dev_type, "camera");
			break;
		case 5:
			strcpy(dev_type, "hgw");
			break;
		default:
			strcpy(dev_type, "computer");
			break;
	}
	if(active == 1){
		sprintf(cmdbuf,"/usr/bin/accesslimitcmd devinfo 1 %s %02X:%02X:%02X:%02X:%02X:%02X",dev_type, 
			addr[0],addr[1],addr[2],addr[3],addr[4],addr[5]);
		system(cmdbuf);
		//tcdbg_printf("\n%s\n",cmdbuf);
	}
	else{
		sprintf(cmdbuf,"/usr/bin/accesslimitcmd devinfo 0 %s %02X:%02X:%02X:%02X:%02X:%02X",dev_type, 
			addr[0],addr[1],addr[2],addr[3],addr[4],addr[5]);
		system(cmdbuf);
		//tcdbg_printf("%s",cmdbuf);
	}
}
#endif

#ifdef COMBINED_BINARY
int udhcpd_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	fd_set rfds;
	struct timeval tv;
	int server_socket = -1;
	int bytes, retval;
	struct dhcpMessage packet;
	uint8_t *state;
	uint8_t *server_id, *requested;
	uint32_t server_id_align, requested_align;
	unsigned long timeout_end;
	struct option_set *option;
	struct dhcpOfferedAddr *lease;
	struct dhcpOfferedAddr static_lease;
	int max_sock;
	unsigned long num_ips;
	unsigned int i;

	uint32_t static_lease_ip;
#ifdef BBU_SOC	
	FILE *fp;
	char buf[32];
#endif
#if defined(TCSUPPORT_CT_FJ)
	#define IP_SKB_MARK 50
	unsigned int mark=0;
	int marklen=0;
#endif

#ifdef DHCP_PROFILE
	uint8_t *vendorID;
	uint8_t config_file_exist = 0;
	FILE *pconfig_file = NULL;
#if defined(TCSUPPORT_CT_IPTV_NODE)
	char totalNum[8];
	gSTB_Parm.header = (STBDev_t*)malloc(sizeof(STBDev_t));
	if(!gSTB_Parm.header)
	{
		LOG(LOG_ERR, "malloc STB device node(head) failed.");
		return;
	}
	memset(gSTB_Parm.header, 0,  sizeof(STBDev_t));
	gSTB_Parm.tail = gSTB_Parm.header;
	gSTB_Parm.header->next = NULL;
	gSTB_Parm.iTotalNumber = 0;
	memset(totalNum, 0, sizeof(totalNum));
	sprintf(totalNum,"%d",gSTB_Parm.iTotalNumber);
	tcapi_set("Dhcpd_Option60", "STBNum", totalNum);
#endif

#endif
	struct  timespec  curtime;

#if defined (CWMP) && defined(TR111)
	gTR111_Parm.VIHeader= (mDev_t*)malloc(sizeof(mDev_t));
	if(!gTR111_Parm.VIHeader)
	{		
		LOG(LOG_ERR, "malloc device node(head) failed.");
		return;
	}
	memset(gTR111_Parm.VIHeader, 0, sizeof(mDev_t));
	gTR111_Parm.VITail= gTR111_Parm.VIHeader;
	gTR111_Parm.VIHeader->next = NULL;
	gTR111_Parm.iNotifyFlag = 1;
	gTR111_Parm.iTotalNumber = 0;
	gTR111_Parm.iNotifyTimer = 0;
#endif
	leaseNumber = 0;

	memset(&server_config, 0, sizeof(struct server_config_t));
	/*read dhcp main pool config file*/
	memset(&dhcp_config, 0, sizeof(struct server_config_t));

	//read_config(argc < 2 ? DHCPD_CONF_FILE : argv[1]);
	if(argc < 2)
		read_config(DHCPD_CONF_FILE);
	else{
#ifdef TCSUPPORT_FON_V2
		if(!strcmp(argv[1], FON_DHCPD_CONF_FILE))
			IsFonDhcpServer = 1;
#endif
		read_config(argv[1]);
	}

	/* Start the log, sanitize fd's, and write a pid file */
#if defined(TCSUPPORT_FON_V2)
	if(IsFonDhcpServer)	
		start_log_and_pid("fon udhcpd", dhcp_config.pidfile);
	else
#endif
	start_log_and_pid("udhcpd", dhcp_config.pidfile);

	/*get main pool lease time*/
	if ((option = find_option(dhcp_config.options, DHCP_LEASE_TIME))) {
		memcpy(&dhcp_config.lease, option->data + 2, 4);
		dhcp_config.lease = ntohl(dhcp_config.lease);
	}
	else dhcp_config.lease = LEASE_TIME;
#ifdef BBU_SOC
	fp = fopen("/etc/dhcpdarpwaittime","r");
	if(fp != NULL){
		fread(buf, sizeof(buf), 1, fp);
		dhcp_arp_timeout_ms = atoi(buf);
		tcdbg_printf("User set Arp Wait time %dms",dhcp_arp_timeout_ms);
		fclose(fp);
	}else{
		dhcp_arp_timeout_ms = 50;	
	}
#endif	

	/* Sanity check */
	num_ips = ntohl(dhcp_config.end) - ntohl(dhcp_config.start) + 1; 
	if (dhcp_config.max_leases > num_ips) {
		LOG(LOG_ERR, "max_leases value (%lu) not sane, "
			"setting to %lu instead",
			dhcp_config.max_leases, num_ips);
		dhcp_config.max_leases = num_ips;
	}

	if (read_interface(dhcp_config.interface, &dhcp_config.ifindex,
			   &dhcp_config.server, dhcp_config.arp) < 0)
		return 1;
	
#ifdef DHCP_PROFILE
	/*init gOption60_Param*/
	gOption60_Param.enable = 0;
	gOption60_Param.dhcp_opt60_vid = NULL;

	/*check conditional pool config file is exist or not*/
	if ((pconfig_file = fopen(DHCPD_OPTION_CONF_FILE, "r")) != NULL)
		config_file_exist = 1;
	
	if(config_file_exist) {
		/*read dhcp option60 pool config file*/
		memset(&option60_config, 0, sizeof(struct server_config_t));
		read_config(DHCPD_OPTION_CONF_FILE);
		
		/*get option60 pool lease time*/
		if ((option = find_option(option60_config.options, DHCP_LEASE_TIME))) {
			memcpy(&option60_config.lease, option->data + 2, 4);
			option60_config.lease = ntohl(option60_config.lease);
		}
		else option60_config.lease = LEASE_TIME;

		/* Sanity check */
		num_ips = ntohl(option60_config.end) - ntohl(option60_config.start) + 1;
		if (option60_config.max_leases > num_ips) {
			LOG(LOG_ERR, "max_leases value (%lu) not sane, "
				"setting to %lu instead",
				option60_config.max_leases, num_ips);
			option60_config.max_leases = num_ips;
		}

		if (read_interface(option60_config.interface, &option60_config.ifindex,
				   &option60_config.server, option60_config.arp) < 0)
			return 1;
	}
#endif

/*account the total max leases of all pools*/
#ifdef DHCP_PROFILE
	if(config_file_exist) 
		gTotalMaxLeases = dhcp_config.max_leases + option60_config.max_leases ;
	else
		gTotalMaxLeases = dhcp_config.max_leases;
#else
	gTotalMaxLeases = dhcp_config.max_leases;
#endif

	leases = xcalloc(gTotalMaxLeases, sizeof(struct dhcpOfferedAddr));
	/* need initialization.
	 * You see, dhcpd maybe restarted
	 * */
	for(i=0; i<gTotalMaxLeases;i++){
		memset(&(leases[i]), 0, sizeof(struct dhcpOfferedAddr));
	}
	read_leases(dhcp_config.lease_file);

#ifndef UDHCP_DEBUG
	background(dhcp_config.pidfile); /* hold lock during fork. */
#endif

	/* Setup the signal pipe */
	udhcp_sp_setup();

	clock_gettime(CLOCK_MONOTONIC, &curtime);
	timeout_end = curtime.tv_sec+ dhcp_config.auto_time;
	//timeout_end = time(0) + dhcp_config.auto_time;
	while(1) { /* loop until universe collapses */

		if (server_socket < 0)
			if ((server_socket = listen_socket(INADDR_ANY, SERVER_PORT, dhcp_config.interface)) < 0) {
				LOG(LOG_ERR, "FATAL: couldn't create server socket, %m");
				return 2;
			}

		max_sock = udhcp_sp_fd_set(&rfds, server_socket);
		if (dhcp_config.auto_time) {
			//tv.tv_sec = timeout_end - time(0);
			clock_gettime(CLOCK_MONOTONIC, &curtime);
			tv.tv_sec = timeout_end - curtime.tv_sec;
			tv.tv_usec = 0;
		}
#if defined(TR111) || defined(TCSUPPORT_CT_IPTV_NODE)
//#if defined (CWMP) && defined(TR111)
		/* refresh each second */
		tv.tv_sec = 1;
#endif
		
		if (!dhcp_config.auto_time || tv.tv_sec > 0) {
//#if defined (CWMP) && defined(TR111)
#if defined(TR111) || defined(TCSUPPORT_CT_IPTV_NODE)
			retval = select(max_sock + 1, &rfds, NULL, NULL, &tv);
#else
			retval = select(max_sock + 1, &rfds, NULL, NULL, dhcp_config.auto_time ? &tv : NULL);
#endif
		} else retval = 0; /* If we already timed out, fall through */

		if (retval == 0) {
#if defined(TR111) || defined(TCSUPPORT_CT_IPTV_NODE)
//#if defined (CWMP) && defined(TR111)
			clock_gettime(CLOCK_MONOTONIC, &curtime);
			//if(timeout_end < time(0))
			if(timeout_end <curtime.tv_sec)
			{
#endif
			write_leases();
			clock_gettime(CLOCK_MONOTONIC, &curtime);
			timeout_end = curtime.tv_sec + dhcp_config.auto_time;
			//timeout_end = time(0) + dhcp_config.auto_time;
//#if defined (CWMP) && defined(TR111)
#if  defined(TR111) || defined(TCSUPPORT_CT_IPTV_NODE)
			}
			else
			{
#if defined(TCSUPPORT_CT_IPTV_NODE)
				dhcpSTBRefresh();
#endif
#ifdef TR111
				dhcpRefresh();
#endif	
		}
#endif
			continue;
		} else if (retval < 0 && errno != EINTR) {
			DEBUG(LOG_INFO, "error on select");
			continue;
		}

		switch (udhcp_sp_read(&rfds)) {
		case SIGUSR1:
			LOG(LOG_INFO, "Received a SIGUSR1");
			write_leases();
			/* why not just reset the timeout, eh */
			clock_gettime(CLOCK_MONOTONIC, &curtime);
			timeout_end = curtime.tv_sec+ dhcp_config.auto_time;
			//timeout_end = time(0) + dhcp_config.auto_time;
			continue;
		case SIGTERM:
			LOG(LOG_INFO, "Received a SIGTERM");
			return 0;
		case 0: break;		/* no signal */
		default: continue;	/* signal or error (probably EINTR) */
		}


		if ((bytes = get_packet(&packet, server_socket)) < 0) { /* this waits for a packet - idle */
			if (bytes == -1 && errno != EINTR) {
				DEBUG(LOG_INFO, "error on read, %m, reopening socket");
				close(server_socket);
				server_socket = -1;
			}
			continue;
		}
		
#if defined(TCSUPPORT_CT_FJ)
		if (0 == getsockopt(server_socket, SOL_IP, IP_SKB_MARK, &mark, &marklen)){
			//get mark
		}	
#endif
	
#ifdef DHCP_PROFILE
#if defined(TCSUPPORT_CT_IPTV_NODE)
		gOption60IsMatched = 0;
#endif
		if ((vendorID = get_option(&packet, DHCP_VENDOR)) != NULL) {
				/* DHCP option 60:
				 * enable option 60, if match the vendorclass, the DHCP client will get the IP from conditional pool
				 */		
				if (config_file_exist && gOption60_Param.enable) {
#if defined(TCSUPPORT_CT_E8GUI)
					udhcpd_parser_option60(vendorID);	
#else
					if ((vendorID[OPT_LEN - 2] >= VENDOR_CLASS_ID_LEN_MAX)
					|| (vendorID[OPT_LEN - 2] != strlen(gOption60_Param.dhcp_opt60_vid))
					|| (strncmp(vendorID, gOption60_Param.dhcp_opt60_vid, vendorID[OPT_LEN - 2]) != 0)) {		
#if 0
						gOption60IsMatched = 0;//not match
						memcpy(&server_config, &option60_config, sizeof(struct server_config_t));
						memset(&option60_config, 0, sizeof(struct server_config_t));
						gOption60_Param.dhcp_opt60_vid = NULL;
						read_config(DHCPD_OPTION_STB_CONF_FILE);
						if (strstr(vendorID, gOption60_Param.dhcp_opt60_vid) != NULL)
						{
							gOption60IsMatched = 2;
							tcdbg_printf("get device is STB\n");
						}
						if(!gOption60IsMatched){
						memset(&option60_config, 0, sizeof(struct server_config_t));
						gOption60_Param.dhcp_opt60_vid = NULL;
						read_config(DHCPD_OPTION_PHONE_CONF_FILE);
						if (strstr(vendorID, gOption60_Param.dhcp_opt60_vid) != NULL)
						{
							gOption60IsMatched = 3;
							tcdbg_printf("get device is phone\n");
						}
						}
						if(!gOption60IsMatched){
						memset(&option60_config, 0, sizeof(struct server_config_t));
						gOption60_Param.dhcp_opt60_vid = NULL;
						read_config(DHCPD_OPTION_CAMERA_CONF_FILE);
						if (strstr(vendorID, gOption60_Param.dhcp_opt60_vid) != NULL)
						{
							gOption60IsMatched = 4;
							tcdbg_printf("get device is camera\n");
						}
						}
						//default is computer
						if(!gOption60IsMatched){
							memset(&option60_config, 0, sizeof(struct server_config_t));
							read_config(DHCPD_OPTION_CONF_FILE);
							gOption60IsMatched = 1;
						}
						
						option60_config.lease = server_config.lease;
						option60_config.max_leases = server_config.max_leases;
						option60_config.ifindex = server_config.ifindex;
						option60_config.server = server_config.server;
						memcpy(option60_config.arp, server_config.arp, sizeof(server_config.arp));
						memset(&server_config, 0, sizeof(struct server_config_t));
#endif					
						gOption60IsMatched = 0;//not match

					}
					else{
						gOption60IsMatched = 1;//match

					}
#endif
				}
				else{//conditional pool disabled
					gOption60IsMatched = 0;
				}

		}

	if(gOption60IsMatched) {		
		memcpy(&server_config, &option60_config, sizeof(struct server_config_t));//using option 60 pool
#if !defined(TCSUPPORT_CT_IPTV_NODE)
#if !defined(TCSUPPORT_CT_PORTAL_MANAGEMENT)
		gOption60IsMatched = 0;
#endif
#endif
	tcdbg_printf("goptionasdfasd = %d\n", gOption60IsMatched);
	}
	else
#endif
		memcpy(&server_config, &dhcp_config, sizeof(struct server_config_t));//using main pool

		if ((state = get_option(&packet, DHCP_MESSAGE_TYPE)) == NULL) {
			DEBUG(LOG_ERR, "couldn't get option from packet, ignoring");
			continue;
		}

		/* Look for a static lease */
		static_lease_ip = getIpByMac(server_config.static_leases, &packet.chaddr);

		if(static_lease_ip)
		{
			printf("Found static lease: %x\n", static_lease_ip);

			memcpy(&static_lease.chaddr, &packet.chaddr, 16);
			static_lease.yiaddr = static_lease_ip;
			static_lease.expires = 0;

			lease = &static_lease;

		}
		else
		{
		lease = find_lease_by_chaddr(packet.chaddr);
		}

		switch (state[0]) {
		case DHCPDISCOVER:
			DEBUG(LOG_INFO,"received DISCOVER");
#if defined(TCSUPPORT_CT_IPTV_NODE)
			if(2 == gOption60IsMatched){
				insertSTBDevice(&packet);
				memset(totalNum, 0, sizeof(totalNum));
				sprintf(totalNum,"%d",gSTB_Parm.iTotalNumber);
				tcapi_set("Dhcpd_Option60", "STBNum", totalNum);
			}
#endif

#if defined(TCSUPPORT_CT_FJ)
			if (sendOffer(&packet, mark) < 0) {
#else
			if (sendOffer(&packet) < 0) {
#endif

				LOG(LOG_ERR, "send OFFER failed");
			}
			break;
 		case DHCPREQUEST:
			DEBUG(LOG_INFO, "received REQUEST");

			requested = get_option(&packet, DHCP_REQUESTED_IP);
			server_id = get_option(&packet, DHCP_SERVER_ID);

			if (requested) memcpy(&requested_align, requested, 4);
			if (server_id) memcpy(&server_id_align, server_id, 4);

			if (lease) {
				if (server_id) {
					/* SELECTING State */
					DEBUG(LOG_INFO, "server_id = %08x", ntohl(server_id_align));
					if (server_id_align == server_config.server && requested &&
					    requested_align == lease->yiaddr) {
#if defined(TCSUPPORT_CT_FJ)
						sendACK(&packet, lease->yiaddr, mark);
#else

						sendACK(&packet, lease->yiaddr);
#endif

#if defined(TCSUPPORT_CT_ACCESSLIMIT)
						udhcpd_set_dev_info(1, packet.chaddr);
#endif
#if defined(TCSUPPORT_CT_PORTAL_MANAGEMENT)
						write_ip_vendor_files(lease->yiaddr, gOption60IsMatched);
#endif
					}
				} else {
					if (requested) {
						/* INIT-REBOOT State */
						if (lease->yiaddr == requested_align)
						{
#if defined(TCSUPPORT_CT_FJ)
							sendACK(&packet, lease->yiaddr, mark);
#else
							sendACK(&packet, lease->yiaddr);
#endif

#if defined(TCSUPPORT_CT_ACCESSLIMIT)
							udhcpd_set_dev_info(1, packet.chaddr);	
#endif
#if defined(TCSUPPORT_CT_PORTAL_MANAGEMENT)
							write_ip_vendor_files(lease->yiaddr, gOption60IsMatched);
#endif
						}
						else sendNAK(&packet);
					} else {
						/* RENEWING or REBINDING State */
						if (lease->yiaddr == packet.ciaddr)
						{
#if defined(TCSUPPORT_CT_FJ)
							sendACK(&packet, lease->yiaddr, mark);
#else
							sendACK(&packet, lease->yiaddr);
#endif


#if defined(TCSUPPORT_CT_ACCESSLIMIT)
							udhcpd_set_dev_info(1, packet.chaddr);							
#endif
#if defined(TCSUPPORT_CT_PORTAL_MANAGEMENT)
							write_ip_vendor_files(lease->yiaddr, gOption60IsMatched);
#endif
						}
						else {
							/* don't know what to do!!!! */
							sendNAK(&packet);
						}
					}
				}

			/* what to do if we have no record of the client */
			} else if (server_id) {
				/* SELECTING State */

			} else if (requested) {
				/* INIT-REBOOT State */
				if ((lease = find_lease_by_yiaddr(requested_align))) {
					if (lease_expired(lease)) {
						/* probably best if we drop this lease */
						memset(lease->chaddr, 0, 16);
					/* make some contention for this address */
					} else sendNAK(&packet);
				} else {
					/*if lease == NULL, it means client send request without receving
					DHCP OFFER packet, according to RFC 2131, dhcp server should send NAK*/
					sendNAK(&packet);
				} /* else remain silent */

			} else {
				 /* RENEWING or REBINDING State */
				sendNAK(&packet);
			}
			break;
		case DHCPDECLINE:
			DEBUG(LOG_INFO,"received DECLINE");
			if (lease) {
				memset(lease->chaddr, 0, 16);
				clock_gettime(CLOCK_MONOTONIC, &curtime);
				lease->expires = curtime.tv_sec+ server_config.decline_time;
				//lease->expires = time(0) + server_config.decline_time;
			}
			break;
		case DHCPRELEASE:
			DEBUG(LOG_INFO,"received RELEASE");
#if defined (CWMP) && defined(TR111)
			if (check_vi(&packet))
				deleteDevice(&packet);
#endif
#if defined(TCSUPPORT_CT_IPTV_NODE)
			if(2 == gOption60IsMatched){
				deleteSTBDevice(&packet);
				memset(totalNum, 0, sizeof(totalNum));
				sprintf(totalNum,"%d",gSTB_Parm.iTotalNumber);
				tcapi_set("Dhcpd_Option60", "STBNum", gSTB_Parm.iTotalNumber);
			}
#endif
#if defined(TCSUPPORT_CT_ACCESSLIMIT)
			udhcpd_set_dev_info(0, packet.chaddr);
#endif

			clock_gettime(CLOCK_MONOTONIC, &curtime);
			if (lease) lease->expires = curtime.tv_sec;
			//if (lease) lease->expires = time(0);
			//write lease file and send msg to CWMP to reinit host tree
			write_leases_enhance(lease);
			break;
		case DHCPINFORM:
			DEBUG(LOG_INFO,"received INFORM");
			
#if defined (CWMP) && defined(TR111)
			if (check_vi(&packet))
				insertDevice(&packet);
#endif
#if defined(TCSUPPORT_CT_FJ)
			send_inform(&packet, mark);
#else
			send_inform(&packet);
#endif
			break;
		default:
			LOG(LOG_WARNING, "unsupported DHCP message (%02x) -- ignoring", state[0]);
		}
	}

	return 0;
}

