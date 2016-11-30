/*-
 * Copyright (c) 1990, 1993
 *      The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Van Jacobson.
 *
 * Special for busybox ported by Vladimir Oleynik <dzo@simtreas.ru> 2001
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
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

/*
 * traceroute host  - trace the route ip packets follow going to "host".
 * Notes
 * -----
 * This program must be run by root or be setuid.  (I suggest that
 * you *don't* make it setuid -- casual use could result in a lot
 * of unnecessary traffic on our poor, congested nets.)
 *
 * I stole the idea for this program from Steve Deering.  Since
 * the first release, I've learned that had I attended the right
 * IETF working group meetings, I also could have stolen it from Guy
 * Almes or Matt Mathis.  I don't know (or care) who came up with
 * the idea first.  I envy the originators' perspicacity and I'm
 * glad they didn't keep the idea a secret.
 *
 * Tim Seaver, Ken Adelman and C. Philip Wood provided bug fixes and/or
 * enhancements to the original distribution.
 *
 * I've hacked up a round-trip-route version of this that works by
 * sending a loose-source-routed udp datagram through the destination
 * back to yourself.  Unfortunately, SO many gateways botch source
 * routing, the thing is almost worthless.  Maybe one day...
 *
 *  -- Van Jacobson (van@helios.ee.lbl.gov)
 *     Tue Dec 20 03:50:13 PST 1988
 */

#undef CONFIG_FEATURE_TRACEROUTE_VERBOSE
//#define CONFIG_FEATURE_TRACEROUTE_VERBOSE
#undef CONFIG_FEATURE_TRACEROUTE_SO_DEBUG   /* not in documentation man */

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include "inet_common.h"
#include <netdb.h>
#include <endian.h>
#include <netinet/udp.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>


#define MAXPACKET       65535   /* max ip packet size */
#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN  64
#endif

#define SIZEOF_ICMP_HDR  8

//#define TCSUPPORT_TRACEROUTE   //xyyou==
#ifdef TCSUPPORT_TRACEROUTE
static int cwmpflag = 0;
#define CWMP_TRACEROUTE_PID_PATH "/tmp/cwmp/tr69diag_traceroute.pid"
#endif


/*
 * format of a (udp) probe packet.
 */
struct opacket {
	struct ip ip;
	struct udphdr udp;
	u_char seq;             /* sequence number of this packet */
	u_char ttl;             /* ttl packet left with */
	struct timeval tv;      /* time packet left */
};

/*
 * format of a (icmp) probe packet.
 */
struct opacket_icmp {
	struct ip ip;
	struct icmp icmp;
	u_char seq;             /* sequence number of this packet */
	u_char ttl;             /* ttl packet left with */
	struct timeval tv;      /* time packet left */
};


/*
 * Definitions for internet protocol version 4.
 * Per RFC 791, September 1981.
 */
#define IPVERSION       4


#include "busybox.h"

static u_char  packet[512];            /* last inbound (icmp) packet */
static struct opacket  *outpacket;     /* last output (udp) packet */
static struct opacket_icmp  *outpacket_icmp;     /* last output (icmp) packet */


static int s;                          /* receive (icmp) socket file descriptor */
static int sndsock;                    /* send (udp) socket file descriptor */

static struct sockaddr whereto;        /* Who to try to reach */
static int datalen;                    /* How much data */

static char *hostname;

static int max_ttl = 30;
static u_short ident;
static u_short port = 32768+666;       /* start udp dest port # for probe packets */

#ifdef CONFIG_FEATURE_TRACEROUTE_VERBOSE
static int verbose;
#endif
static int waittime = 5;               /* time to wait for response (in seconds) */
static int nflag;                      /* print addresses numerically */

enum { 
	PACKET_ICMP,
	PACKET_UDP,
	PACKET_UNDEFINED
};

static int packet_type = PACKET_UDP;


uint16_t inet_cksum(uint16_t *addr, int nleft)
{
	/*
	 * Our algorithm is simple, using a 32 bit accumulator,
	 * we add sequential 16 bit words to it, and at the end, fold
	 * back all the carry bits from the top 16 bits into the lower
	 * 16 bits.
	 */
	unsigned sum = 0;
	while (nleft > 1) {
		sum += *addr++;
		nleft -= 2;
	}

	/* Mop up an odd byte, if necessary */
	if (nleft == 1) {
//		if (BB_LITTLE_ENDIAN)
//			sum += *(uint8_t*)addr;
//		else
			sum += *(uint8_t*)addr << 8;
	}

	/* Add back carry outs from top 16 bits to low 16 bits */
	sum = (sum >> 16) + (sum & 0xffff);     /* add hi 16 to low 16 */
	sum += (sum >> 16);                     /* add carry */

	return (uint16_t)~sum;
}


/*
 * Construct an Internet address representation.
 * If the nflag has been supplied, give
 * numeric value, otherwise try for symbolic name.
 */
static inline void
inetname(struct sockaddr_in *from)
{
	char *cp;
	static char domain[MAXHOSTNAMELEN + 1];
	char name[MAXHOSTNAMELEN + 1];
	static int first = 1;
	const char *ina;

	if (first && !nflag) {
		first = 0;
		if (getdomainname(domain, MAXHOSTNAMELEN) != 0)
			domain[0] = 0;
	}
	cp = 0;
	if (!nflag && from->sin_addr.s_addr != INADDR_ANY) {
		if(INET_rresolve(name, sizeof(name), from, 0x4000, 0xffffffff) >= 0) {
			if ((cp = strchr(name, '.')) &&
			    !strcmp(cp + 1, domain))
				*cp = 0;
			cp = (char *)name;
		}
	}
	ina = inet_ntoa(from->sin_addr);
	if (nflag)
		printf(" %s", ina);
	else
		printf(" %s (%s)", (cp ? cp : ina), ina);
}

static inline void
print(u_char *buf, int cc, struct sockaddr_in *from)
{
	struct ip *ip;
	int hlen;

	ip = (struct ip *) buf;
	hlen = ip->ip_hl << 2;
	cc -= hlen;

	inetname(from);
#ifdef CONFIG_FEATURE_TRACEROUTE_VERBOSE
	if (verbose)
		printf (" %d bytes to %s", cc, inet_ntoa (ip->ip_dst));
#endif
}

static inline double
deltaT(struct timeval *t1p, struct timeval *t2p)
{
	double dt;

	dt = (double)(t2p->tv_sec - t1p->tv_sec) * 1000.0 +
	     (double)(t2p->tv_usec - t1p->tv_usec) / 1000.0;
	return (dt);
}

static inline int
wait_for_reply(int sock, struct sockaddr_in *from, int reset_timer)
{
	fd_set fds;
	static struct timeval wait;
	int cc = 0;
	int fromlen = sizeof (*from);

	FD_ZERO(&fds);
	FD_SET(sock, &fds);
	if (reset_timer) {
		/*
		 * traceroute could hang if someone else has a ping
		 * running and our ICMP reply gets dropped but we don't
		 * realize it because we keep waking up to handle those
		 * other ICMP packets that keep coming in.  To fix this,
		 * "reset_timer" will only be true if the last packet that
		 * came in was for us or if this is the first time we're
		 * waiting for a reply since sending out a probe.  Note
		 * that this takes advantage of the select() feature on
		 * Linux where the remaining timeout is written to the
		 * struct timeval area.
		 */
		wait.tv_sec = waittime;
		wait.tv_usec = 0;
	}

	if (select(sock+1, &fds, (fd_set *)0, (fd_set *)0, &wait) > 0)
		cc=recvfrom(s, (char *)packet, sizeof(packet), 0,
			    (struct sockaddr *)from, &fromlen);

	return(cc);
}

#ifdef CONFIG_FEATURE_TRACEROUTE_VERBOSE
/*
 * Convert an ICMP "type" field to a printable string.
 */
static inline const char *
pr_type(u_char t)
{
	static const char * const ttab[] = {
	"Echo Reply",   "ICMP 1",       "ICMP 2",       "Dest Unreachable",
	"Source Quench", "Redirect",    "ICMP 6",       "ICMP 7",
	"Echo",         "ICMP 9",       "ICMP 10",      "Time Exceeded",
	"Param Problem", "Timestamp",   "Timestamp Reply", "Info Request",
	"Info Reply"
	};

	if(t > 16)
		return("OUT-OF-RANGE");

	return(ttab[t]);
}
#endif

static inline int
packet_ok(u_char *buf, int cc, struct sockaddr_in *from, int seq)
{
	struct icmp *icp;
	u_char type, code;
	int hlen;
	struct ip *ip;

	ip = (struct ip *) buf;
	hlen = ip->ip_hl << 2;
	if (cc < hlen + ICMP_MINLEN) {
#ifdef CONFIG_FEATURE_TRACEROUTE_VERBOSE
		if (verbose)
			printf("packet too short (%d bytes) from %s\n", cc,
				inet_ntoa(from->sin_addr));
#endif
		return (0);
	}
	cc -= hlen;
	icp = (struct icmp *)(buf + hlen);
	type = icp->icmp_type;
	code = icp->icmp_code;
	
	if ((type == ICMP_TIMXCEED && code == ICMP_TIMXCEED_INTRANS) 
		|| type == ICMP_UNREACH
		|| type == ICMP_ECHOREPLY)
	{
		struct ip *hip;
		struct udphdr *up;

		hip = &icp->icmp_ip;
		hlen = hip->ip_hl << 2;

		if (packet_type == PACKET_ICMP)
		{
			struct icmp *hicmp;

			/* XXX */
			if (type == ICMP_ECHOREPLY
			 && icp->icmp_id == htons(ident)
			 && icp->icmp_seq == htons(seq)
			) {
				return ICMP_UNREACH_PORT+1;
			}

			hicmp = (struct icmp *)((unsigned char *)hip + hlen);
			if (hlen + SIZEOF_ICMP_HDR <= cc
			 && hip->ip_p == IPPROTO_ICMP
			 && hicmp->icmp_id == htons(ident)
			 && hicmp->icmp_seq == htons(seq)
			)
			{
				return (type == ICMP_TIMXCEED ? -1 : code + 1);
			}
		}
		else
		{
			up = (struct udphdr *)((u_char *)hip + hlen);
			if (hlen + 12 <= cc && hip->ip_p == IPPROTO_UDP &&
			    up->source == htons(ident) &&
			    up->dest == htons(port+seq))
				return (type == ICMP_TIMXCEED? -1 : code+1);
		}
	}
#ifdef CONFIG_FEATURE_TRACEROUTE_VERBOSE
	if (verbose) {
		int i;
		u_long *lp = (u_long *)&icp->icmp_ip;

		printf("\n%d bytes from %s to %s: icmp type %d (%s) code %d\n",
			cc, inet_ntoa(from->sin_addr), inet_ntoa(ip->ip_dst),
			type, pr_type(type), icp->icmp_code);
		for (i = 4; i < cc ; i += sizeof(long))
			printf("%2d: x%8.8lx\n", i, *lp++);
	}
#endif
	return(0);
}

static void             /* not inline */
send_probe(int seq, int ttl)
{	
	int i;
	struct timezone tz;
	struct icmp *ic = NULL;
	struct opacket_icmp *op_icmp = NULL;
	struct opacket *op = NULL;
	struct udphdr *up = NULL;
	struct ip *ip = NULL;
	
	if (packet_type == PACKET_ICMP)
	{
		op_icmp = outpacket_icmp;
		ic = &op_icmp->icmp;
		ip = &op_icmp->ip;
	}
	else
	{
		op = outpacket;
		up = &op->udp;
		ip = &op->ip;
	}

	ip->ip_off = 0;
	ip->ip_hl = sizeof(*ip) >> 2;
	if (packet_type == PACKET_ICMP)
	{
		ip->ip_p = IPPROTO_ICMP;
	}
	else
	{
		ip->ip_p = IPPROTO_UDP;
	}
	
	ip->ip_len = datalen;
	ip->ip_ttl = ttl;
	ip->ip_v = IPVERSION;
	ip->ip_id = htons(ident+seq);

	if (packet_type == PACKET_ICMP)
	{
		ic->icmp_seq = htons(seq);
		//		(void) gettimeofday(&op_icmp->tv, &tz);

		/* Always calculate checksum for icmp packets */
		ic->icmp_cksum = 0;
		ic->icmp_cksum = inet_cksum((uint16_t *)ic,
					datalen - sizeof(struct ip) );
		if (ic->icmp_cksum == 0)
			ic->icmp_cksum = 0xffff;

		i = sendto(sndsock, (char *)outpacket_icmp, datalen, 0, &whereto,
		   sizeof(struct sockaddr));
	}
	else
	{
		up->source = htons(ident);
		up->dest = htons(port+seq);
		up->len = htons((u_short)(datalen - sizeof(struct ip)));
		up->check = 0;
		op->seq = seq;
		op->ttl = ttl;
		(void) gettimeofday(&op->tv, &tz);

		i = sendto(sndsock, (char *)outpacket, datalen, 0, &whereto,
		   sizeof(struct sockaddr));
	}

	
	if (i < 0 || i != datalen)  {
		if (i<0)
			perror("sendto");
		printf("traceroute: wrote %s %d chars, ret=%d\n", hostname,
			datalen, i);
		(void) fflush(stdout);
	}
}


int
#if !defined(CONFIG_TRACEROUTE)&&!defined(TCSUPPORT_TRACEROUTE)
main(int argc, char *argv[])
#else
traceroute_main(int argc, char *argv[])
#endif
{
	extern char *optarg;
	extern int optind;
	struct hostent *hp;
	struct sockaddr_in from, *to;
	int ch, i, on, probe, seq, tos, ttl;

	int options = 0;                /* socket options */
	char *source = 0;
	int nprobes = 3;
#if defined(TCSUPPORT_CT)
	int stopFlag = 0;
	int	stopCounter = 0;
#endif

#ifdef TCSUPPORT_TRACEROUTE
	//time_t tm;
	//int starttime = 0;
	struct timeval starttime, endtime;
	struct timezone tztime;
	char timestr[33] = {0};
	int timestrindex = 0;
//	int errorcode = -1;
//	char hopaddrtmp[50] = {0};
//	char hophosttmp[300] = {0};
//	char hoperrortmp[30] = {0};
//	char hoprtttmp[32] = {0};
	char tmpstr[300] = {0};
	FILE *fptmp = fopen("/tmp/cwmp/traceroutediagnostic_stat", "w");
	if(fptmp == NULL){
		printf("open file error in traceroute!!!\n");
		return -1;
	}
#endif
	on = 1;
	seq = tos = 0;
	to = (struct sockaddr_in *)&whereto;
#ifdef TCSUPPORT_TRACEROUTE
#if defined(TCSUPPORT_CT)
while ((ch = getopt(argc, argv, "dm:np:aq:rs:t:w:ivx:")) != EOF)
#else
	while ((ch = getopt(argc, argv, "dm:np:q:rs:t:w:vx:")) != EOF)
#endif
#else
#if defined(TCSUPPORT_CT)
	while ((ch = getopt(argc, argv, "dm:np:aq:rs:t:w:iv")) != EOF)
#else
	while ((ch = getopt(argc, argv, "dm:np:q:rs:t:w:v")) != EOF)
#endif
#endif
		switch(ch) {
		case 'd':
#ifdef CONFIG_FEATURE_TRACEROUTE_SO_DEBUG
			options |= SO_DEBUG;
#endif
			break;
		case 'm':
			max_ttl = atoi(optarg);
			if (max_ttl <= 1)
				bb_error_msg_and_die("max ttl must be >1.");
			break;
		case 'n':
			nflag++;
			break;
		case 'p':
			port = atoi(optarg);
			if (port < 1)
				bb_error_msg_and_die("port must be >0.");
			break;
#if defined(TCSUPPORT_CT)
		case 'a':
			stopFlag = 1;
			break;
#endif
		case 'q':
			nprobes = atoi(optarg);
			if (nprobes < 1)
				bb_error_msg_and_die("nprobes must be >0.");
			break;
		case 'r':
			options |= SO_DONTROUTE;
			break;
		case 's':
			/*
			 * set the ip source address of the outbound
			 * probe (e.g., on a multi-homed host).
			 */
			source = optarg;
			break;
		case 't':
			tos = atoi(optarg);
			if (tos < 0 || tos > 255)
				bb_error_msg_and_die("tos must be 0 to 255.");
			break;
		case 'v':
#ifdef CONFIG_FEATURE_TRACEROUTE_VERBOSE
			verbose++;
#endif
			break;
		case 'w':
			waittime = atoi(optarg);
			if(waittime < 1)
			{
				waittime = 1;
			}
			else if(waittime > (24*60*60))
			{
				waittime = 24*60*60;
			}
//			if (waittime <= 1)
//				bb_error_msg_and_die("wait must be >1 sec.");
			break;
		case 'i':
			packet_type = PACKET_ICMP;
//			printf("send icmp packet\n");
			break;

#ifdef TCSUPPORT_TRACEROUTE
		case 'x':
			cwmpflag = atoi(optarg);
			if(cwmpflag)
			{
				FILE *cfp = NULL;
				cfp = fopen(CWMP_TRACEROUTE_PID_PATH, "wb");
				if(cfp)
				{
					fprintf(cfp, "%d", getpid());
					fclose(cfp);
				}			
			}
			break;
#endif
		default:
			bb_show_usage();
		}
	argc -= optind;
	argv += optind;

	if (argc < 1)
		bb_show_usage();

	setlinebuf (stdout);

	memset(&whereto, 0, sizeof(struct sockaddr));
	hp = xgethostbyname(*argv);
			to->sin_family = hp->h_addrtype;
	memcpy(&to->sin_addr, hp->h_addr, hp->h_length);
			hostname = (char *)hp->h_name;
	if (*++argv)
		datalen = atoi(*argv);
	if (datalen < 0 || datalen >= MAXPACKET - sizeof(struct opacket))
		bb_error_msg_and_die("packet size must be 0 <= s < %d.",
		    MAXPACKET - sizeof(struct opacket));
	
	ident = (getpid() & 0xffff) | 0x8000;

	if (packet_type == PACKET_ICMP)
	{
		datalen += sizeof(struct opacket_icmp);
		outpacket_icmp = (struct opacket_icmp *)xmalloc((unsigned)datalen);
		memset(outpacket_icmp, 0, datalen);
		outpacket_icmp->ip.ip_dst = to->sin_addr;
		outpacket_icmp->ip.ip_tos = tos;
		outpacket_icmp->ip.ip_v = IPVERSION;
		outpacket_icmp->ip.ip_id = 0;

		outpacket_icmp->icmp.icmp_type = ICMP_ECHO;
		outpacket_icmp->icmp.icmp_id = htons(ident);
	}
	else
	{
		datalen += sizeof(struct opacket);
		outpacket = (struct opacket *)xmalloc((unsigned)datalen);
		memset(outpacket, 0, datalen);
		outpacket->ip.ip_dst = to->sin_addr;
		outpacket->ip.ip_tos = tos;
		outpacket->ip.ip_v = IPVERSION;
		outpacket->ip.ip_id = 0;
	}

	if ((sndsock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0)
		bb_perror_msg_and_die(bb_msg_can_not_create_raw_socket);

	s = create_icmp_socket();

#ifdef CONFIG_FEATURE_TRACEROUTE_SO_DEBUG
	if (options & SO_DEBUG)
		(void) setsockopt(s, SOL_SOCKET, SO_DEBUG,
				  (char *)&on, sizeof(on));
#endif
	if (options & SO_DONTROUTE)
		(void) setsockopt(s, SOL_SOCKET, SO_DONTROUTE,
				  (char *)&on, sizeof(on));
#ifdef SO_SNDBUF
	if (setsockopt(sndsock, SOL_SOCKET, SO_SNDBUF, (char *)&datalen,
		       sizeof(datalen)) < 0)
		bb_perror_msg_and_die("SO_SNDBUF");
#endif
#ifdef IP_HDRINCL
	if (setsockopt(sndsock, IPPROTO_IP, IP_HDRINCL, (char *)&on,
		       sizeof(on)) < 0)
		bb_perror_msg_and_die("IP_HDRINCL");
#endif
#ifdef CONFIG_FEATURE_TRACEROUTE_SO_DEBUG
	if (options & SO_DEBUG)
		(void) setsockopt(sndsock, SOL_SOCKET, SO_DEBUG,
				  (char *)&on, sizeof(on));
#endif
	if (options & SO_DONTROUTE)
		(void) setsockopt(sndsock, SOL_SOCKET, SO_DONTROUTE,
				  (char *)&on, sizeof(on));

	if (source) {
		memset(&from, 0, sizeof(struct sockaddr));
		from.sin_family = AF_INET;
		from.sin_addr.s_addr = inet_addr(source);

		if (from.sin_addr.s_addr == -1)
			bb_error_msg_and_die("unknown host %s", source);

		if (packet_type == PACKET_ICMP)
		{
			outpacket_icmp->ip.ip_src = from.sin_addr;
		}
		else
		{
			outpacket->ip.ip_src = from.sin_addr;
		}
		
#ifndef IP_HDRINCL
		if (bind(sndsock, (struct sockaddr *)&from, sizeof(from)) < 0)
			bb_perror_msg_and_die("bind");
#endif
	}

#if !defined(TCSUPPORT_CT)
	fprintf(stderr, "traceroute to %s (%s)", hostname,
		inet_ntoa(to->sin_addr));
	if (source)
		fprintf(stderr, " from %s", source);
	fprintf(stderr, ", %d hops max, %d byte packets\n", max_ttl, datalen);
#else
	printf("traceroute to %s (%s)", hostname,
		inet_ntoa(to->sin_addr));
	if (source)
		printf(" from %s", source);
	printf(", %d hops max, %d byte packets\n", max_ttl, datalen);
#endif

#ifdef TCSUPPORT_TRACEROUTE
	if(cwmpflag == 1){
		//starttime = time(&tm);
		(void) gettimeofday(&starttime, &tztime);
	}

#endif

	u_long lastaddr = 0;
	int got_there = 0;
	int unreachable = 0;
	
	for (ttl = 1; ttl <= max_ttl; ++ttl)
	{
		lastaddr = 0;
		got_there = 0;
		unreachable = 0;

//		printf("ttl:%2d ", ttl);
		printf("%2d ", ttl);
		for (probe = 0; probe < nprobes; ++probe)
		{
			int cc, reset_timer;
			struct timeval t1, t2;
			struct timezone tz;
			struct ip *ip;

			(void) gettimeofday(&t1, &tz);
			send_probe(++seq, ttl);
			reset_timer = 1;
//			printf("probe%d \n",probe);
			while (
#if defined(TCSUPPORT_CT)
			(stopFlag == 0 || stopCounter < 6) &&
#endif
			 (cc = wait_for_reply(s, &from, reset_timer)) != 0)
			 {
				(void) gettimeofday(&t2, &tz);
				if ((i = packet_ok(packet, cc, &from, seq))) 
				{
//					printf("packet_ok return %d \n",i);
					reset_timer = 1;
#if 0//def TCSUPPORT_TRACEROUTE
					if(cwmpflag == 1){
						if(((i-1)!=ICMP_UNREACH_PORT)||((i-1)!=ICMP_UNREACH_PROTOCOL))
							printf("  HopErrorCode%d=%d \n", ttl, (i - 1));
					}

#endif			
					if (from.sin_addr.s_addr != lastaddr) {
						print(packet, cc, &from);
#ifdef TCSUPPORT_TRACEROUTE
						if(cwmpflag == 1){//xyyou==
							
							//printf("  HopHostAddress%d=%s \n", ttl, inet_ntoa(from.sin_addr));
							//printf("  HopHost%d=%s \n", ttl, inet_ntoa(from.sin_addr));
							//printf("  HopErrorCode%d=%d \n", ttl, (i - 1));
							sprintf(tmpstr, "HopHostAddress%d=%s \n", ttl, inet_ntoa(from.sin_addr));							
							fputs(tmpstr, fptmp);
							sprintf(tmpstr, "HopHost%d=%s \n", ttl, inet_ntoa(from.sin_addr));
							fputs(tmpstr, fptmp);
							if(i==-1)//xyyou add for ICMP_TIMXCEED case,buf it's normal for trace route
								sprintf(tmpstr, "HopErrorCode%d=%d \n", ttl, 0);
							else{
								if( i == (ICMP_UNREACH_PORT+1)){
									sprintf(tmpstr, "HopErrorCode%d=%d \n", ttl, 0);
								}else
								{
									sprintf(tmpstr, "HopErrorCode%d=%d \n", ttl, (i - 1));
								}
							}
							fputs(tmpstr, fptmp);
						}
 
#endif			
						lastaddr = from.sin_addr.s_addr;
					}
					printf("  %g ms", deltaT(&t1, &t2));
#ifdef TCSUPPORT_TRACEROUTE
					if(cwmpflag == 1){
						if(probe == 0)
							timestrindex+= sprintf(timestr + timestrindex, "%gms",  deltaT(&t1, &t2));
						else
							timestrindex+= sprintf(timestr + timestrindex,  ",%gms",  deltaT(&t1, &t2));
					//	printf("  HopRTTimes%d=%s \n", ttl, timestr);
					}
#endif			
					switch(i - 1) {
					case ICMP_UNREACH_PORT:
						ip = (struct ip *)packet;
						if (ip->ip_ttl <= 1)
							printf(" !");
						++got_there;
						break;
					case ICMP_UNREACH_NET:
						++unreachable;
						printf(" !N");
						break;
					case ICMP_UNREACH_HOST:
						++unreachable;
						printf(" !H");
						break;
					case ICMP_UNREACH_PROTOCOL:
						++got_there;
						printf(" !P");
						break;
					case ICMP_UNREACH_NEEDFRAG:
						++unreachable;
						printf(" !F");
						break;
					case ICMP_UNREACH_SRCFAIL:
						++unreachable;
						printf(" !S");
						break;
					}

//					printf("\ngot_there:%d unreachable:%d break \n",got_there,unreachable);
					break;
				} 
				else
				{
//					printf("packet_ok return %d \n",i);
					reset_timer = 0;
				}	
			}

			if (cc == 0)
			{
				printf(" *");
#if defined(TCSUPPORT_CT)
				stopCounter++;				
#endif
#ifdef TCSUPPORT_TRACEROUTE
					if(cwmpflag == 1){
						if(probe == 0)
							timestrindex+= sprintf(timestr + timestrindex, "*");
						else
							timestrindex+= sprintf(timestr + timestrindex, ",*");
					//	printf("  HopRTTimes%d=%s \n", ttl, timestr);
					}

#endif			
			}
#if defined(TCSUPPORT_CT)
			else
			{
				stopCounter = 0;
			}
#endif
			(void) fflush(stdout);
		}
#ifdef TCSUPPORT_TRACEROUTE
		if(cwmpflag == 1){
			//printf("  HopErrorCode%d=%d \n", ttl, (i - 1));
			sprintf(tmpstr, "HopRTTimes%d=%s \n", ttl, timestr);
			fputs(tmpstr, fptmp);
			timestrindex = 0;
		}

#endif	
		putchar('\n');
/*
		if(cwmpflag==1)
		{
			sprintf(tmpstr, "got_there=%d unreachable=%d nprobes=%d ttl=%d \n", got_there,unreachable,nprobes,ttl);
			fputs(tmpstr, fptmp);
		}*/
		if (got_there || (unreachable > 0 && unreachable >= nprobes-1))//xyyou==
		{
//		#ifdef TCSUPPORT_TRACEROUTE
#if 0
			if(cwmpflag == 1){
				//output num of hops
				//if(got_there){
					(void) gettimeofday(&endtime, &tztime);
					sprintf(tmpstr, "ResponseTime=%g ms\n", deltaT(&starttime, &endtime));
					fputs(tmpstr, fptmp);
					sprintf(tmpstr, "RouteHopsNumberOfEntries=%lu \n", ttl);
					fputs(tmpstr, fptmp);
				//}else{
					
				//}

			}
			fclose(fptmp);
		#endif
			break;		
//			return 0;
		}
	}

#ifdef TCSUPPORT_TRACEROUTE
		if(cwmpflag == 1){
			//output num of hops
			//if(got_there){
				(void) gettimeofday(&endtime, &tztime);
				sprintf(tmpstr, "ResponseTime=%g ms\n", deltaT(&starttime, &endtime));
				fputs(tmpstr, fptmp);
				if(ttl > max_ttl){
				sprintf(tmpstr, "RouteHopsNumberOfEntries=%lu \n", ttl-1);
				}else{
					sprintf(tmpstr, "RouteHopsNumberOfEntries=%lu \n", ttl);
				}
				fputs(tmpstr, fptmp);
				if(((ttl-1) == max_ttl) || (0 == got_there)){
					sprintf(tmpstr, "MaxHopCountExceeded=%s\n", "1");
					fputs(tmpstr, fptmp);
				}
			//}else{
				
			//}
			
		}
		fclose(fptmp);
#endif

	return 0;
}
