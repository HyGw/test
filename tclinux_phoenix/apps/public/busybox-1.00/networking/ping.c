/* vi: set sw=4 ts=4: */
/*
 * $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/apps/public/busybox-1.00/networking/ping.c#2 $
 * Mini ping implementation for busybox
 *
 * Copyright (C) 1999 by Randolph Chung <tausq@debian.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * This version of ping is adapted from the ping in netkit-base 0.10,
 * which is:
 *
 * Copyright (c) 1989 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Mike Muuss.
 *
 * Original copyright notice is retained at the end of this file.
 */

#include <sys/param.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/signal.h>

#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "busybox.h"

#if defined __GLIBC__
#include <time.h>
#endif
static const int DEFDATALEN = 56;
static const int MAXIPLEN = 60;
static const int MAXICMPLEN = 76;
static const int MAXPACKET = 65468;
#define	MAX_DUP_CHK	(8 * 128)
#if 0
static const int MAXWAIT = 10;
#else
#define MAXWAIT	10
#endif
#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
int PINGINTERVAL = 1;		/* second */
int PINGINTERVAL_DIAG =1;
uint8_t PINGDSCP = 0;
#else
static const int PINGINTERVAL = 1;		/* second */
#endif
int timeout=MAXWAIT;

#ifdef CWMP
int cwmpflag = 0;
#define CWMP_PING_PID_PATH "/tmp/cwmp/tr69diag_ping.pid"
#endif
#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
int pidentry = 0;
int ctcomflag = 0;
char DevStr[32] = "None",GatewayStr[32] = "None";
#define CTCOM_PING_PID_PATH "/tmp/cwmp/ct_ping%d.pid"
#define IPPINGDIAGNOSTIC_ENTRY "CtDiagnostic_Entry%d"
#define CTCOM_PING_ROUTE_PATH "/tmp/cwmp/ct_ping_route"
#define IPPINGDIAGNOSTIC_ADDROUTE "route add -host %s dev %s"
#define IPPINGDIAGNOSTIC_DELROUTE "route del %s"
#endif
#if defined(TCSUPPORT_EPON_OAM_CUC)
int cuc_ping_flag = 0;
#define CUC_PING_TEST_PID_PATH "/var/run/cuc_voip_ping.pid"
#define CUC_PING_TEST_RESULT_END	"0"
#define CUC_PING_TEST_RESULT_UNKNOWN	"3"
#define CUC_PING_TEST_RESULT_UNKNOWN_IP	"4"
#define CUC_PING_TEST_RESULT_SENT_TO_ERR	"6"
#define CUC_PING_TEST_ACTION_COMPLETE	"2"
#endif
#ifdef TCSUPPORT_EPON_DUMMY
int epon_dummy_flag = 0;
#endif

#define O_QUIET         (1 << 0)
#define O_TERM          (1 << 3)
#if defined(TCSUPPORT_CT_IPOE_EMULATOR)
int ipoe_flag = 0;
#define IPOE_EMULATOR_PING_PID_PATH "/var/run/ipoe_ping.pid"
#define IPOE_EMULATOR_PING_RESULT "/var/run/ipoe_emu/PingResult"

static void pingError()
{
	FILE *fp = NULL;

	if ( ipoe_flag )
	{
		fp = fopen(IPOE_EMULATOR_PING_RESULT, "wb");
		if ( fp )
		{
			fprintf(fp, "%ld,%ld,%lu,%lu,%lu\n", 0, 0, 0, 0, 0);
			fclose(fp);
		}
		tcapi_commit("IpoeEmulator_Entry");
	}
}
#endif

#define	A(bit)		rcvd_tbl[(bit)>>3]	/* identify byte in array */
#define	B(bit)		(1 << ((bit) & 0x07))	/* identify bit in byte */
#define	SET(bit)	(A(bit) |= B(bit))
#define	CLR(bit)	(A(bit) &= (~B(bit)))
#define	TST(bit)	(A(bit) & B(bit))

static void ping(const char *host);

#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
static void pingErrorHandleFunc(void);
#endif
/* common routines */
static int in_cksum(unsigned short *buf, int sz)
{
	int nleft = sz;
	int sum = 0;
	unsigned short *w = buf;
	unsigned short ans = 0;

	while (nleft > 1) {
		sum += *w++;
		nleft -= 2;
	}

	if (nleft == 1) {
		*(unsigned char *) (&ans) = *(unsigned char *) w;
		sum += ans;
	}

	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	ans = ~sum;
	return (ans);
}

/* simple version */
#ifndef CONFIG_FEATURE_FANCY_PING
static char *hostname = NULL;
static void noresp(int ign)
{
	printf("No response from %s\n", hostname);
	exit(EXIT_FAILURE);
}

static void ping(const char *host)
{
	struct hostent *h;
	struct sockaddr_in pingaddr;
	struct icmp *pkt;
	int pingsock, c;
	char packet[DEFDATALEN + MAXIPLEN + MAXICMPLEN];

	pingsock = create_icmp_socket();

	memset(&pingaddr, 0, sizeof(struct sockaddr_in));

	pingaddr.sin_family = AF_INET;
	h = xgethostbyname(host);
	memcpy(&pingaddr.sin_addr, h->h_addr, sizeof(pingaddr.sin_addr));
	hostname = h->h_name;

	pkt = (struct icmp *) packet;
	memset(pkt, 0, sizeof(packet));
	pkt->icmp_type = ICMP_ECHO;
	pkt->icmp_cksum = in_cksum((unsigned short *) pkt, sizeof(packet));

	c = sendto(pingsock, packet, sizeof(packet), 0,
			   (struct sockaddr *) &pingaddr, sizeof(struct sockaddr_in));

	if (c < 0 || c != sizeof(packet))
		bb_perror_msg_and_die("sendto");

	signal(SIGALRM, noresp);
	alarm(5);					/* give the host 5000ms to respond */
	/* listen for replies */
	while (1) {
		struct sockaddr_in from;
		size_t fromlen = sizeof(from);

		if ((c = recvfrom(pingsock, packet, sizeof(packet), 0,
						  (struct sockaddr *) &from, &fromlen)) < 0) {
			if (errno == EINTR)
				continue;
			bb_perror_msg("recvfrom");
			continue;
		}
		if (c >= 76) {			/* ip + icmp */
			struct iphdr *iphdr = (struct iphdr *) packet;

			pkt = (struct icmp *) (packet + (iphdr->ihl << 2));	/* skip ip hdr */
			if (pkt->icmp_type == ICMP_ECHOREPLY)
				break;
		}
	}
	printf("%s is alive!\n", hostname);
	return;
}

extern int ping_main(int argc, char **argv)
{
	argc--;
	argv++;
	if (argc < 1)
		bb_show_usage();
	ping(*argv);
	return EXIT_SUCCESS;
}

#else /* ! CONFIG_FEATURE_FANCY_PING */
/* full(er) version */
static struct sockaddr_in pingaddr;
static int pingsock = -1;
static int datalen; /* intentionally uninitialized to work around gcc bug */

static long ntransmitted, nreceived, nrepeats, pingcount;
static int myid, options;
static unsigned long tmin = ULONG_MAX, tmax, tsum;
static char rcvd_tbl[MAX_DUP_CHK / 8];

struct hostent *hostent;

static void sendping(int);
static void pingstats(int);
static void unpack(char *, int, struct sockaddr_in *);

/**************************************************************************/

static void pingstats(int junk)
{
	int status;
#if defined(TCSUPPORT_CT_IPOE_EMULATOR)
	FILE *fp = NULL;
	int temp = 0, aver = 0;
#endif

	signal(SIGINT, SIG_IGN);

#ifdef CWMP
	if(cwmpflag != 0)
		printf("%ld,%ld,%lu,%lu,%lu\n",ntransmitted,nreceived, tmin / 10,(tsum / (nreceived + nrepeats)) / 10, tmax / 10);
#endif
#if defined(TCSUPPORT_EPON_OAM_CUC)
	char *tmp[32] = {0};
	if(cuc_ping_flag){
		tcapi_set("CucPing_Entry","Result",CUC_PING_TEST_RESULT_END);
		sprintf(tmp, "%ld", ntransmitted);
		tcapi_set("CucPing_Entry","TXPkt", tmp);
		
		memset(tmp,0, sizeof(tmp));
		sprintf(tmp, "%ld", nreceived);
		tcapi_set("CucPing_Entry","RxPkt",tmp);
		
		memset(tmp,0, sizeof(tmp));
		sprintf(tmp, "%ld", (ntransmitted-nreceived));
		tcapi_set("CucPing_Entry","LostPkt",tmp);

		memset(tmp,0, sizeof(tmp));
		sprintf(tmp, "%ld", ((ntransmitted-nreceived)*100)/ntransmitted);
		tcapi_set("CucPing_Entry","LostPktRatio",tmp);

		if (nreceived){
			memset(tmp,0, sizeof(tmp));
			sprintf(tmp, "%lu.%lu", tmin / 10, tmin % 10);
			tcapi_set("CucPing_Entry","MinDelay",tmp);

			memset(tmp,0, sizeof(tmp));
			sprintf(tmp, "%lu.%lu", tmax / 10, tmax % 10);
			tcapi_set("CucPing_Entry","MaxDelay",tmp);

			memset(tmp,0, sizeof(tmp));
			sprintf(tmp, "%lu.%lu", (tsum / (nreceived + nrepeats)) / 10, (tsum / (nreceived + nrepeats)) % 10);
			tcapi_set("CucPing_Entry","AvgDelay",tmp);
		}else{
			tcapi_set("CucPing_Entry","MinDelay","0");
			tcapi_set("CucPing_Entry","MaxDelay","0");
			tcapi_set("CucPing_Entry","AvgDelay","0");
		}

		tcapi_set("CucPing_Entry", "PingAction", CUC_PING_TEST_ACTION_COMPLETE);
		tcapi_commit("CucPing");
	}
#endif
#ifdef TCSUPPORT_EPON_DUMMY
	if(epon_dummy_flag){
		char *lostPkt[32] = {0};
		sprintf(lostPkt, "%ld", ((ntransmitted-nreceived)*100)/ntransmitted);
		tcapi_set("EPON_Dummy","LostPkt",lostPkt);
	}
#endif
#if defined(TCSUPPORT_CT_IPOE_EMULATOR)
	if ( ipoe_flag  )
	{
		fp = fopen(IPOE_EMULATOR_PING_RESULT, "wb");
		if ( fp )
		{
			if ( 0 == nreceived )
			{
				tmin = 0;
				tmax = 0;
				tsum = 0;
			}

			if ( 0 != (nreceived + nrepeats) )
				aver = (tsum / (nreceived + nrepeats));
			fprintf(fp, "%ld,%ld,%lu,%lu,%lu\n", ntransmitted, nreceived, tmin / 10, aver / 10, tmax / 10);
			fclose(fp);
		}
		tcapi_commit("IpoeEmulator_Entry");
	}
#endif

	printf("\n--- %s ping statistics ---\n", hostent->h_name);
	printf("%ld packets transmitted, ", ntransmitted);
	printf("%ld packets received, ", nreceived);
	if (nrepeats)
		printf("%ld duplicates, ", nrepeats);
	if (ntransmitted)
		printf("%ld%% packet loss\n",
			   (ntransmitted - nreceived) * 100 / ntransmitted);
	if (nreceived)
		printf("round-trip min/avg/max = %lu.%lu/%lu.%lu/%lu.%lu ms\n",
			   tmin / 10, tmin % 10,
			   (tsum / (nreceived + nrepeats)) / 10,
			   (tsum / (nreceived + nrepeats)) % 10, tmax / 10, tmax % 10);
	if (nreceived != 0)
		status = EXIT_SUCCESS;
	else
		status = EXIT_FAILURE;
	exit(status);
}

static void sendping(int junk)
{
	struct icmp *pkt;
	int i;
	char packet[datalen + 8];

#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
	char pingbuf[10] = {0};
	char ctcombuf[100] = {0};
	struct  timespec  curtime, lasttime, difftime; 	
	int tempinterval = 0;
	int pingnum = 0;
#endif
#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
startsendping:
#endif
	pkt = (struct icmp *) packet;

	pkt->icmp_type = ICMP_ECHO;
	pkt->icmp_code = 0;
	pkt->icmp_cksum = 0;
	pkt->icmp_seq = ntransmitted++;
	pkt->icmp_id = myid;
	CLR(pkt->icmp_seq % MAX_DUP_CHK);

	gettimeofday((struct timeval *) &packet[8], NULL);
	pkt->icmp_cksum = in_cksum((unsigned short *) pkt, sizeof(packet));
	
	i = sendto(pingsock, packet, sizeof(packet), 0,
			   (struct sockaddr *) &pingaddr, sizeof(struct sockaddr_in));

	if (i < 0)
	{
#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
		pingErrorHandleFunc();
#endif
#if defined(TCSUPPORT_EPON_OAM_CUC)
		if(cuc_ping_flag){
			tcapi_set("CucPing_Entry","Result",CUC_PING_TEST_RESULT_SENT_TO_ERR);
			tcapi_set("CucPing_Entry", "PingAction", CUC_PING_TEST_ACTION_COMPLETE);
			tcapi_commit("CucPing");
		}
#endif
#if defined(TCSUPPORT_CT_IPOE_EMULATOR)
		pingError();
#endif
		bb_perror_msg_and_die("sendto");
	
	}
	else if ((size_t)i != sizeof(packet))
	{
#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
		pingErrorHandleFunc();
#endif
		
#if defined(TCSUPPORT_EPON_OAM_CUC)
		if(cuc_ping_flag){
			tcapi_set("CucPing_Entry","Result",CUC_PING_TEST_RESULT_UNKNOWN);
			tcapi_set("CucPing_Entry", "PingAction", CUC_PING_TEST_ACTION_COMPLETE);
			tcapi_commit("CucPing");
		}
#endif
#if defined(TCSUPPORT_CT_IPOE_EMULATOR)
		pingError();
#endif
		bb_error_msg_and_die("ping wrote %d chars; %d expected", i,
			(int)sizeof(packet));
	}

	signal(SIGALRM, sendping);
	
#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
	memset(&lasttime,0,sizeof(struct  timespec));
	memset(&curtime,0,sizeof(struct  timespec));
	memset(&difftime,0,sizeof(struct  timespec));
	tempinterval = PINGINTERVAL_DIAG;
	clock_gettime(CLOCK_MONOTONIC, &lasttime);
	if(ctcomflag && (pingcount > 0))
	{
		//printf("\r\nsend ping!!");
		sprintf(ctcombuf,IPPINGDIAGNOSTIC_ENTRY,pidentry);
		tcapi_get(ctcombuf,"PingNum",pingbuf);
		pingnum = atoi(pingbuf);
		//sprintf(pingbuf,"%lu",ntransmitted);
		sprintf(pingbuf,"%lu",pingnum+1);
		tcapi_set(ctcombuf,"PingNum",pingbuf);
		if(ntransmitted >= pingcount)
		{	
			if(0 == nreceived)
				tcapi_set(ctcombuf,"DiagnosticsState","Error_Other");
			else
				tcapi_set(ctcombuf,"DiagnosticsState","Complete");      	
			tcapi_set(ctcombuf,"StopPingFlag","2");
			tcapi_commit(ctcombuf);
			if(strcmp(GatewayStr,"None")&&strcmp(DevStr,"None")){
				memset(ctcombuf,0,sizeof(ctcombuf));
				//sprintf(ctcombuf,"/sbin/route delete -net %s netmask 255.255.255.255 gw %s dev %s",inet_ntoa(*((struct in_addr  *)(hostent->h_addr))),GatewayStr,DevStr);
				sprintf(ctcombuf, IPPINGDIAGNOSTIC_DELROUTE, inet_ntoa(*(struct in_addr  *)(hostent->h_addr)), DevStr);
				system(ctcombuf);
			}
		
			//tcapi_save();
		}
		tcapi_save();
	}
	clock_gettime(CLOCK_MONOTONIC, &curtime);
	difftime.tv_sec = curtime.tv_sec - lasttime.tv_sec;
	   difftime.tv_nsec += curtime.tv_nsec - lasttime.tv_nsec;
	   while (difftime.tv_nsec > 1000000000) 
	{
		difftime.tv_sec++;
		difftime.tv_nsec -= 1000000000;
		}
		if (difftime.tv_nsec < 0) 
	{
		difftime.tv_sec--;
		difftime.tv_nsec += 1000000000;
		}
	
	tempinterval -= difftime.tv_sec;
	if(tempinterval <= 0)
		//tempinterval = 1;
		goto startsendping;
	else
		PINGINTERVAL = tempinterval;
	
#endif
	
	if (pingcount == 0 || ntransmitted < pingcount) {	/* schedule next in 1s */
#if defined(TCSUPPORT_CT_PON_GD)
		if (options & O_TERM)
		{
			if(nreceived > 0)				// once get a reply, just terminate
			{
				signal(SIGALRM, pingstats);
				alarm(timeout);
			}
			else {
				alarm(PINGINTERVAL);
			}
		}
		else
#endif

		alarm(PINGINTERVAL);
	} else {					/* done, wait for the last ping to come back */
		/* todo, don't necessarily need to wait so long... */
		signal(SIGALRM, pingstats);
		/*alarm(MAXWAIT);*/
		/*Support a new option for set timeout value*/
		alarm(timeout);
	}
}

static char *icmp_type_name (int id)
{
	switch (id) {
	case ICMP_ECHOREPLY: 		return "Echo Reply";
	case ICMP_DEST_UNREACH: 	return "Destination Unreachable";
	case ICMP_SOURCE_QUENCH: 	return "Source Quench";
	case ICMP_REDIRECT: 		return "Redirect (change route)";
	case ICMP_ECHO: 			return "Echo Request";
	case ICMP_TIME_EXCEEDED: 	return "Time Exceeded";
	case ICMP_PARAMETERPROB: 	return "Parameter Problem";
	case ICMP_TIMESTAMP: 		return "Timestamp Request";
	case ICMP_TIMESTAMPREPLY: 	return "Timestamp Reply";
	case ICMP_INFO_REQUEST: 	return "Information Request";
	case ICMP_INFO_REPLY: 		return "Information Reply";
	case ICMP_ADDRESS: 			return "Address Mask Request";
	case ICMP_ADDRESSREPLY: 	return "Address Mask Reply";
	default: 					return "unknown ICMP type";
	}
}

static void unpack(char *buf, int sz, struct sockaddr_in *from)
{
	struct icmp *icmppkt;
	struct iphdr *iphdr;
	struct timeval tv, *tp;
	int hlen, dupflag;
	unsigned long triptime;

	gettimeofday(&tv, NULL);

	/* check IP header */
	iphdr = (struct iphdr *) buf;
	hlen = iphdr->ihl << 2;
	/* discard if too short */
	if (sz < (datalen + ICMP_MINLEN))
		return;

	sz -= hlen;
	icmppkt = (struct icmp *) (buf + hlen);

	if (icmppkt->icmp_id != myid)
	    return;				/* not our ping */

	if (icmppkt->icmp_type == ICMP_ECHOREPLY) {
	    ++nreceived;
		tp = (struct timeval *) icmppkt->icmp_data;

		if ((tv.tv_usec -= tp->tv_usec) < 0) {
			--tv.tv_sec;
			tv.tv_usec += 1000000;
		}
		tv.tv_sec -= tp->tv_sec;

		triptime = tv.tv_sec * 10000 + (tv.tv_usec / 100);
		tsum += triptime;
		if (triptime < tmin)
			tmin = triptime;
		if (triptime > tmax)
			tmax = triptime;

		if (TST(icmppkt->icmp_seq % MAX_DUP_CHK)) {
			++nrepeats;
			--nreceived;
			dupflag = 1;
		} else {
			SET(icmppkt->icmp_seq % MAX_DUP_CHK);
			dupflag = 0;
		}

		if (options & O_QUIET)
			return;

		printf("%d bytes from %s: icmp_seq=%u", sz,
			   inet_ntoa(*(struct in_addr *) &from->sin_addr.s_addr),
			   icmppkt->icmp_seq);
		printf(" ttl=%d", iphdr->ttl);
		printf(" time=%lu.%lu ms", triptime / 10, triptime % 10);
		if (dupflag)
			printf(" (DUP!)");
		printf("\n");
	} else
		if (icmppkt->icmp_type != ICMP_ECHO)
			bb_error_msg("Warning: Got ICMP %d (%s)",
					icmppkt->icmp_type, icmp_type_name (icmppkt->icmp_type));
}

static void ping(const char *host)
{
	char packet[datalen + MAXIPLEN + MAXICMPLEN];
	int sockopt;

#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)//add route
	char ctcombuf[100] = {0};
	FILE *fp = NULL;	
	uint8_t tos = 0;
#endif
	pingsock = create_icmp_socket();

	memset(&pingaddr, 0, sizeof(struct sockaddr_in));

	pingaddr.sin_family = AF_INET;
	
#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)//add route
	if(ctcomflag) {
		if ((hostent = gethostbyname(host)) == NULL)
			{
				sprintf(ctcombuf,IPPINGDIAGNOSTIC_ENTRY,pidentry);
				tcapi_set(ctcombuf,"DiagnosticsState","Error_CannotResolveHostName");
				tcapi_set(ctcombuf,"StopPingFlag","2");
				tcapi_commit(ctcombuf);	
				tcapi_save();
				bb_herror_msg_and_die("%s", host);
				
			}
		else if(strcmp(GatewayStr,"None") && strcmp(DevStr,"None"))
			{
				memset(ctcombuf,0,sizeof(ctcombuf));
				tcdbg_printf("\r\t-------------add route----\n");
				//sprintf(ctcombuf,"/sbin/route add -net %s netmask 255.255.255.255 gw %s dev %s",inet_ntoa(*(struct in_addr  *)(hostent->h_addr)),GatewayStr,DevStr);
				sprintf(ctcombuf, IPPINGDIAGNOSTIC_ADDROUTE, inet_ntoa(*(struct in_addr  *)(hostent->h_addr)), DevStr);
				system(ctcombuf);
				memset(ctcombuf,0,sizeof(ctcombuf));
				sprintf(ctcombuf,"%s%d",CTCOM_PING_ROUTE_PATH,pidentry);
				fp = fopen(ctcombuf,"wb");
				if(!fp)
				{	
					memset(ctcombuf,0,sizeof(ctcombuf));
					sprintf(ctcombuf,IPPINGDIAGNOSTIC_ENTRY,pidentry);
					tcapi_set(ctcombuf,"DiagnosticsState","Error_Internal");
					tcapi_commit(ctcombuf);	
					tcapi_save();
				}
				memset(ctcombuf,0,sizeof(ctcombuf));
				//sprintf(ctcombuf,"/sbin/route delete -net %s netmask 255.255.255.255 gw %s dev %s",inet_ntoa(*((struct in_addr  *)(hostent->h_addr))),GatewayStr,DevStr);
				sprintf(ctcombuf, IPPINGDIAGNOSTIC_DELROUTE, inet_ntoa(*(struct in_addr  *)(hostent->h_addr)), DevStr);
				fputs(ctcombuf,fp);
				fclose(fp);
			}
		}
	else{	
			hostent = xgethostbyname(host);	
		}
			
#else
		hostent = xgethostbyname(host);
#endif
	if (hostent->h_addrtype != AF_INET)
	{
#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
		pingErrorHandleFunc();
#endif
#if defined(TCSUPPORT_EPON_OAM_CUC)
		if(cuc_ping_flag){
			tcapi_set("CucPing_Entry","Result",CUC_PING_TEST_RESULT_UNKNOWN_IP);
			tcapi_set("CucPing_Entry", "PingAction", CUC_PING_TEST_ACTION_COMPLETE);
			tcapi_commit("CucPing");
		}
#endif
#if defined(TCSUPPORT_CT_IPOE_EMULATOR)
		pingError();
#endif
		bb_error_msg_and_die("unknown address type; only AF_INET is currently supported.");
	}

	memcpy(&pingaddr.sin_addr, hostent->h_addr, sizeof(pingaddr.sin_addr));

	/* enable broadcast pings */
	sockopt = 1;
	setsockopt(pingsock, SOL_SOCKET, SO_BROADCAST, (char *) &sockopt,
			   sizeof(sockopt));

	/* set recv buf for broadcast pings */
	sockopt = 48 * 1024;
	setsockopt(pingsock, SOL_SOCKET, SO_RCVBUF, (char *) &sockopt,
			   sizeof(sockopt));
	
#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
	tos = (PINGDSCP << 2);
	setsockopt(pingsock, SOL_IP, IP_TOS, (void *) &tos, sizeof(tos));
#endif

#ifdef CWMP
if(cwmpflag == 0)
{
#endif
	printf("PING %s (%s): %d data bytes\n",
	           hostent->h_name,
		   inet_ntoa(*(struct in_addr *) &pingaddr.sin_addr.s_addr),
		   datalen);

#ifdef CWMP
}
#endif
	signal(SIGINT, pingstats);

	/* start the ping's going ... */
	sendping(0);

	/* listen for replies */
	while (1) {
		struct sockaddr_in from;
		socklen_t fromlen = (socklen_t) sizeof(from);
		int c;

		if ((c = recvfrom(pingsock, packet, sizeof(packet), 0,
						  (struct sockaddr *) &from, &fromlen)) < 0) {
			if (errno == EINTR)
				continue;
			bb_perror_msg("recvfrom");
			continue;
		}
		unpack(packet, c, &from);
		if (pingcount > 0 && nreceived >= pingcount) {
			break;
                }
#if defined(TCSUPPORT_CT_PON_GD)
		else
		{
			if (options & O_TERM)
			{
				if(nreceived > 0)               // once get a reply, just terminate
				{
					break;
				}
			}
		}
#endif
	}
	pingstats(0);
}

#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
static void pingErrorHandleFunc(void)
{
	char ctcombuf[100] = {0};
	FILE * fp;
	if(ctcomflag){ 
		sprintf(ctcombuf,IPPINGDIAGNOSTIC_ENTRY,pidentry);
		tcapi_set(ctcombuf,"DiagnosticsState","Error_Other");
		tcapi_set(ctcombuf,"StopPingFlag","2");
		tcapi_commit(ctcombuf);
		tcapi_save();
		memset(ctcombuf,0,sizeof(ctcombuf));
		sprintf(ctcombuf,"%s%d",CTCOM_PING_ROUTE_PATH,pidentry);
		fp = fopen(ctcombuf, "r");
		if(fp)
		{
			//delete the route which add at the beginning of the ping 
			memset(ctcombuf,0,sizeof(ctcombuf));
			fgets(ctcombuf, sizeof(ctcombuf), fp);
			system(ctcombuf);

			//delete the file /tmp/cwmp/ct_ping_route
			fclose(fp);
			memset(ctcombuf,0,sizeof(ctcombuf));
			sprintf(ctcombuf,"%s%d",CTCOM_PING_ROUTE_PATH,pidentry);
			unlink(ctcombuf);
		}
	}
}
#endif
extern int ping_main(int argc, char **argv)
{
	char *thisarg;
#if defined(TCSUPPORT_CT_IPOE_EMULATOR)
	FILE *fp = NULL;
#endif

	datalen = DEFDATALEN; /* initialized here rather than in global scope to work around gcc bug */

#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
	char bufstr[32] = {0};
#endif
	argc--;
	argv++;
	options = 0;
	/* Parse any options */
	while (argc >= 1 && **argv == '-') {
		thisarg = *argv;
		thisarg++;
		switch (*thisarg) {
		case 't':
			options |= O_TERM;
			break;
		case 'q':
			options |= O_QUIET;
			break;
		case 'c':
			if (--argc <= 0)
			        bb_show_usage();
			argv++;
			pingcount = atoi(*argv);
			break;
		case 's':
			if (--argc <= 0)
			        bb_show_usage();
			argv++;
			datalen = atoi(*argv);
			break;
		case 'W':/*Add a new option for wait timeout value*/
			argv++;
			timeout=atoi(*argv);		
#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
			if(timeout <= 0)
					timeout = MAXWAIT;
#endif
			break;
#ifdef CWMP
		case 'M':
			argv++;
			cwmpflag = atoi(*argv);
			if(cwmpflag)
			{
				FILE *cfp = NULL;
				cfp = fopen(CWMP_PING_PID_PATH, "wb");
				if(cfp)
				{
					fprintf(cfp, "%d", getpid());
					fclose(cfp);
				}			
			}
			break;
#endif
#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
		case 'p':
			argv++;
			pidentry = atoi(*argv);
			if(pidentry >= 0)
			{
				FILE *ctfp = NULL;
				sprintf(bufstr,CTCOM_PING_PID_PATH,pidentry);
				ctfp = fopen(bufstr, "wb");
				if(ctfp)
				{
					fprintf(ctfp, "%d", getpid());
					fclose(ctfp);
				}			
			}
			break;

		case 'i':
			argv++;
			PINGINTERVAL_DIAG = atoi(*argv);
			if(PINGINTERVAL_DIAG <= 0)
				PINGINTERVAL_DIAG = 1;
			break;

		case 'Q':
			argv++;
			PINGDSCP = atoi(*argv);
			if(PINGDSCP > 63)
				PINGDSCP = 0;
			break;
			
		case 'f':
			argv++;
			ctcomflag = atoi(*argv);
			break;		
		case 'g':
			argv++;
			strncpy(GatewayStr,*argv,sizeof(GatewayStr));
			break;
		case 'd':
			argv++;
			strncpy(DevStr,*argv,sizeof(DevStr));
			break;		
		
#endif
#if defined(TCSUPPORT_EPON_OAM_CUC)
		case 'U':
			argv++;
			cuc_ping_flag = atoi(*argv);
			if(cuc_ping_flag)
			{
				FILE *cuc_fp = NULL;
				cuc_fp = fopen(CUC_PING_TEST_PID_PATH, "wb");
				if(cuc_fp)
				{
					fprintf(cuc_fp, "%d", getpid());
					fclose(cuc_fp);
				}			
			}
			break;
#endif

#ifdef TCSUPPORT_EPON_DUMMY
		case 'E':
			argv++;
			epon_dummy_flag = atoi(*argv);
			break;

#endif

#if defined(TCSUPPORT_CT_IPOE_EMULATOR)
			case 'l':
				ipoe_flag = 1;
				fp = fopen(IPOE_EMULATOR_PING_PID_PATH, "wb");
				if ( fp )
				{
					fprintf(fp, "%d", getpid());
					fclose(fp);
				}
			break;
#endif
		default:
			bb_show_usage();
		}
		argc--;
		argv++;
	}
	if (argc < 1)
		bb_show_usage();

	myid = getpid() & 0xFFFF;
	ping(*argv);
	return EXIT_SUCCESS;
}
#endif /* ! CONFIG_FEATURE_FANCY_PING */

/*
 * Copyright (c) 1989 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Mike Muuss.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. <BSD Advertising Clause omitted per the July 22, 1999 licensing change
 *		ftp://ftp.cs.berkeley.edu/pub/4bsd/README.Impt.License.Change>
 *
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
