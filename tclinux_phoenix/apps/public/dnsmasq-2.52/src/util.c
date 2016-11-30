/* dnsmasq is Copyright (c) 2000-2010 Simon Kelley

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 dated June, 1991, or
   (at your option) version 3 dated 29 June, 2007.
 
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
      
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/* The SURF random number generator was taken from djbdns-1.05, by 
   Daniel J Bernstein, which is public domain. */


#include "dnsmasq.h"

#ifdef HAVE_BROKEN_RTC
#include <sys/times.h>
#endif

#ifdef LOCALEDIR
#include <idna.h>
#endif

#ifdef HAVE_ARC4RANDOM
void rand_init(void)
{
  return;
}

unsigned short rand16(void)
{
   return (unsigned short) (arc4random() >> 15);
}

#else

/* SURF random number generator */

typedef unsigned int uint32;

static uint32 seed[32];
static uint32 in[12];
static uint32 out[8];

void rand_init()
{
  int fd = open(RANDFILE, O_RDONLY);
  
  if (fd == -1 ||
      !read_write(fd, (unsigned char *)&seed, sizeof(seed), 1) ||
      !read_write(fd, (unsigned char *)&in, sizeof(in), 1))
    die(_("failed to seed the random number generator: %s"), NULL, EC_MISC);
  
  close(fd);
}

#define ROTATE(x,b) (((x) << (b)) | ((x) >> (32 - (b))))
#define MUSH(i,b) x = t[i] += (((x ^ seed[i]) + sum) ^ ROTATE(x,b));

static void surf(void)
{
  uint32 t[12]; uint32 x; uint32 sum = 0;
  int r; int i; int loop;

  for (i = 0;i < 12;++i) t[i] = in[i] ^ seed[12 + i];
  for (i = 0;i < 8;++i) out[i] = seed[24 + i];
  x = t[11];
  for (loop = 0;loop < 2;++loop) {
    for (r = 0;r < 16;++r) {
      sum += 0x9e3779b9;
      MUSH(0,5) MUSH(1,7) MUSH(2,9) MUSH(3,13)
      MUSH(4,5) MUSH(5,7) MUSH(6,9) MUSH(7,13)
      MUSH(8,5) MUSH(9,7) MUSH(10,9) MUSH(11,13)
    }
    for (i = 0;i < 8;++i) out[i] ^= t[i + 4];
  }
}

unsigned short rand16(void)
{
  static int outleft = 0;

  if (!outleft) {
    if (!++in[0]) if (!++in[1]) if (!++in[2]) ++in[3];
    surf();
    outleft = 8;
  }

  return (unsigned short) out[--outleft];
}

#endif

static int check_name(char *in)
{
  /* remove trailing . 
     also fail empty string and label > 63 chars */
  size_t dotgap = 0, l = strlen(in);
  char c;
  int nowhite = 0;
  
  if (l == 0 || l > MAXDNAME) return 0;
  
  if (in[l-1] == '.')
    {
      if (l == 1) return 0;
      in[l-1] = 0;
    }
  
  for (; (c = *in); in++)
    {
      if (c == '.')
	dotgap = 0;
      else if (++dotgap > MAXLABEL)
	return 0;
      else if (isascii(c) && iscntrl(c)) 
	/* iscntrl only gives expected results for ascii */
	return 0;
#ifndef LOCALEDIR
      else if (!isascii(c))
	return 0;
#endif
      else if (c != ' ')
	nowhite = 1;
    }

  if (!nowhite)
    return 0;

  return 1;
}

/* Hostnames have a more limited valid charset than domain names
   so check for legal char a-z A-Z 0-9 - _ 
   Note that this may receive a FQDN, so only check the first label 
   for the tighter criteria. */
int legal_hostname(char *name)
{
  char c;

  if (!check_name(name))
    return 0;

  for (; (c = *name); name++)
    /* check for legal char a-z A-Z 0-9 - _ . */
    {
      if ((c >= 'A' && c <= 'Z') ||
	  (c >= 'a' && c <= 'z') ||
	  (c >= '0' && c <= '9') ||
	  c == '-' || c == '_')
	continue;
      
      /* end of hostname part */
      if (c == '.')
	return 1;
      
      return 0;
    }
  
  return 1;
}
  
char *canonicalise(char *in, int *nomem)
{
  char *ret = NULL;
#ifdef LOCALEDIR
  int rc;
#endif

  if (nomem)
    *nomem = 0;
  
  if (!check_name(in))
    return NULL;
  
#ifdef LOCALEDIR
  if ((rc = idna_to_ascii_lz(in, &ret, 0)) != IDNA_SUCCESS)
    {
      if (ret)
	free(ret);

      if (nomem && (rc == IDNA_MALLOC_ERROR || rc == IDNA_DLOPEN_ERROR))
	{
	  my_syslog(LOG_ERR, _("failed to allocate memory"));
	  *nomem = 1;
	}
    
      return NULL;
    }
#else
  if ((ret = whine_malloc(strlen(in)+1)))
    strcpy(ret, in);
  else if (nomem)
    *nomem = 1;    
#endif

  return ret;
}

unsigned char *do_rfc1035_name(unsigned char *p, char *sval)
{
  int j;
  
  while (sval && *sval)
    {
      unsigned char *cp = p++;
      for (j = 0; *sval && (*sval != '.'); sval++, j++)
	*p++ = *sval;
      *cp  = j;
      if (*sval)
	sval++;
    }
  return p;
}

/* for use during startup */
void *safe_malloc(size_t size)
{
  void *ret = malloc(size);
  
  if (!ret)
    die(_("could not get memory"), NULL, EC_NOMEM);
     
  return ret;
}    

void safe_pipe(int *fd, int read_noblock)
{
  if (pipe(fd) == -1 || 
      !fix_fd(fd[1]) ||
      (read_noblock && !fix_fd(fd[0])))
    die(_("cannot create pipe: %s"), NULL, EC_MISC);
}

void *whine_malloc(size_t size)
{
  void *ret = malloc(size);

  if (!ret)
    my_syslog(LOG_ERR, _("failed to allocate %d bytes"), (int) size);

  return ret;
}

int sockaddr_isequal(union mysockaddr *s1, union mysockaddr *s2)
{
  if (s1->sa.sa_family == s2->sa.sa_family)
    { 
      if (s1->sa.sa_family == AF_INET &&
	  s1->in.sin_port == s2->in.sin_port &&
	  s1->in.sin_addr.s_addr == s2->in.sin_addr.s_addr)
	return 1;
#ifdef HAVE_IPV6      
      if (s1->sa.sa_family == AF_INET6 &&
	  s1->in6.sin6_port == s2->in6.sin6_port &&
	  IN6_ARE_ADDR_EQUAL(&s1->in6.sin6_addr, &s2->in6.sin6_addr))
	return 1;
#endif
    }
  return 0;
}

int sa_len(union mysockaddr *addr)
{
#ifdef HAVE_SOCKADDR_SA_LEN
  return addr->sa.sa_len;
#else
#ifdef HAVE_IPV6
  if (addr->sa.sa_family == AF_INET6)
    return sizeof(addr->in6);
  else
#endif
    return sizeof(addr->in); 
#endif
}

/* don't use strcasecmp and friends here - they may be messed up by LOCALE */
int hostname_isequal(char *a, char *b)
{
  unsigned int c1, c2;
  
  do {
    c1 = (unsigned char) *a++;
    c2 = (unsigned char) *b++;
    
    if (c1 >= 'A' && c1 <= 'Z')
      c1 += 'a' - 'A';
    if (c2 >= 'A' && c2 <= 'Z')
      c2 += 'a' - 'A';
    
    if (c1 != c2)
      return 0;
  } while (c1);
  
  return 1;
}
    
time_t dnsmasq_time(void)
{
#ifdef HAVE_BROKEN_RTC
  struct tms dummy;
  static long tps = 0;

  if (tps == 0)
    tps = sysconf(_SC_CLK_TCK);

  return (time_t)(times(&dummy)/tps);
#else
  return time(NULL);
#endif
}

int is_same_net(struct in_addr a, struct in_addr b, struct in_addr mask)
{
  return (a.s_addr & mask.s_addr) == (b.s_addr & mask.s_addr);
} 

/* returns port number from address */
int prettyprint_addr(union mysockaddr *addr, char *buf)
{
  int port = 0;
  
#ifdef HAVE_IPV6
  if (addr->sa.sa_family == AF_INET)
    {
      inet_ntop(AF_INET, &addr->in.sin_addr, buf, ADDRSTRLEN);
      port = ntohs(addr->in.sin_port);
    }
  else if (addr->sa.sa_family == AF_INET6)
    {
      inet_ntop(AF_INET6, &addr->in6.sin6_addr, buf, ADDRSTRLEN);
      port = ntohs(addr->in6.sin6_port);
    }
#else
  strcpy(buf, inet_ntoa(addr->in.sin_addr));
  port = ntohs(addr->in.sin_port); 
#endif
  
  return port;
}

void prettyprint_time(char *buf, unsigned int t)
{
  if (t == 0xffffffff)
    sprintf(buf, _("infinite"));
  else
    {
      unsigned int x, p = 0;
       if ((x = t/86400))
	p += sprintf(&buf[p], "%dd", x);
       if ((x = (t/3600)%24))
	p += sprintf(&buf[p], "%dh", x);
      if ((x = (t/60)%60))
	p += sprintf(&buf[p], "%dm", x);
      if ((x = t%60))
	p += sprintf(&buf[p], "%ds", x);
    }
}


/* in may equal out, when maxlen may be -1 (No max len). */
int parse_hex(char *in, unsigned char *out, int maxlen, 
	      unsigned int *wildcard_mask, int *mac_type)
{
  int mask = 0, i = 0;
  char *r;
    
  if (mac_type)
    *mac_type = 0;
  
  while (maxlen == -1 || i < maxlen)
    {
      for (r = in; *r != 0 && *r != ':' && *r != '-'; r++);
      if (*r == 0)
	maxlen = i;
      
      if (r != in )
	{
	  if (*r == '-' && i == 0 && mac_type)
	   {
	      *r = 0;
	      *mac_type = strtol(in, NULL, 16);
	      mac_type = NULL;
	   }
	  else
	    {
	      *r = 0;
	      mask = mask << 1;
	      if (strcmp(in, "*") == 0)
		mask |= 1;
	      else
		out[i] = strtol(in, NULL, 16);
	      i++;
	    }
	}
      in = r+1;
    }
  
  if (wildcard_mask)
    *wildcard_mask = mask;

  return i;
}

/* return 0 for no match, or (no matched octets) + 1 */
int memcmp_masked(unsigned char *a, unsigned char *b, int len, unsigned int mask)
{
  int i, count;
  for (count = 1, i = len - 1; i >= 0; i--, mask = mask >> 1)
    if (!(mask & 1))
      {
	if (a[i] == b[i])
	  count++;
	else
	  return 0;
      }
  return count;
}

/* _note_ may copy buffer */
int expand_buf(struct iovec *iov, size_t size)
{
  void *new;

  if (size <= (size_t)iov->iov_len)
    return 1;

  if (!(new = whine_malloc(size)))
    {
      errno = ENOMEM;
      return 0;
    }

  if (iov->iov_base)
    {
      memcpy(new, iov->iov_base, iov->iov_len);
      free(iov->iov_base);
    }

  iov->iov_base = new;
  iov->iov_len = size;

  return 1;
}

char *print_mac(char *buff, unsigned char *mac, int len)
{
  char *p = buff;
  int i;
   
  if (len == 0)
    sprintf(p, "<null>");
  else
    for (i = 0; i < len; i++)
      p += sprintf(p, "%.2x%s", mac[i], (i == len - 1) ? "" : ":");
  
  return buff;
}

void bump_maxfd(int fd, int *max)
{
  if (fd > *max)
    *max = fd;
}

int retry_send(void)
{
   struct timespec waiter;
   if (errno == EAGAIN)
     {
       waiter.tv_sec = 0;
       waiter.tv_nsec = 10000;
       nanosleep(&waiter, NULL);
       return 1;
     }
   
   if (errno == EINTR)
     return 1;

   return 0;
}

int read_write(int fd, unsigned char *packet, int size, int rw)
{
  ssize_t n, done;
  
  for (done = 0; done < size; done += n)
    {
    retry:
      if (rw)
        n = read(fd, &packet[done], (size_t)(size - done));
      else
        n = write(fd, &packet[done], (size_t)(size - done));

      if (n == 0)
        return 0;
      else if (n == -1)
        {
          if (retry_send() || errno == ENOMEM || errno == ENOBUFS)
            goto retry;
          else
            return 0;
        }
    }
  return 1;
}

#if defined(TCSUPPORT_CT_L2TP_VPN)

/* vpn dns group header */
struct vpn_dns_group *gVPNDNSGrp = NULL;
/* delete vpn dns group temp header */
struct vpn_dns_group *gDelTEMP_VPNDNSGrp = NULL;

/*
check delete domain, return 0 if domain match
*/
int checkDelDomain(char *domain, int vpn_entry)
{
	struct vpn_dns_group *cur_vpndns_group = NULL;

	if ( !domain )
		return -1;

	for ( cur_vpndns_group = gDelTEMP_VPNDNSGrp; cur_vpndns_group;
		cur_vpndns_group = cur_vpndns_group->next )
	{
		if ( vpn_entry == cur_vpndns_group->vpn_tunnel_idx
			&& 0 == strcmp(domain, cur_vpndns_group->domain) )
			return 0;
	}

	return -2;
}
/*
free delete domain temp list
*/
int freeDelDomain()
{
	struct vpn_dns_group *start = NULL;
	struct vpn_dns_group *temp = NULL;

	if ( !gDelTEMP_VPNDNSGrp )
		return -1;

	for ( start = gDelTEMP_VPNDNSGrp; start;  )
	{
		temp = start->next;
		if ( start->domain )
			free(start->domain);
		free(start);
		start = temp;
	}

	return 0;
}


/*
init vpn dns data.
*/
int initVPNDnsGroup()
{
	char nodename[64] = {0}, attrname[64] = {0};
	char ipdomain[256] = {0}, svripAddr[64] = {0};
	char vpnentrybuf[16] = {0};
	int idx = 0, domain_idx = 0, dnsbufLen = 0, vpn_entryidx = 0;
	char cmdbuf[256] = {0};
	struct vpn_dns_group **new_dns_vpn_group = NULL;
	struct vpn_dns_group **new_del_dns_vpn_group = NULL;

	new_dns_vpn_group = &gVPNDNSGrp;
	new_del_dns_vpn_group = &gDelTEMP_VPNDNSGrp;

	/* clear vpn chains for LAN */
	for ( idx = VPN_INSTANCE_NUM - 1; idx >=0; idx -- )
	{
		/* flush and zero it */
		bzero(cmdbuf, sizeof(cmdbuf));
		snprintf(cmdbuf, sizeof(cmdbuf) - 1
			, "ebtables -t broute -F vpn_lan_entry%d", idx);
		system(cmdbuf);

		bzero(cmdbuf, sizeof(cmdbuf));
		snprintf(cmdbuf, sizeof(cmdbuf) - 1
			, "ebtables -t broute -Z vpn_lan_entry%d", idx);
		system(cmdbuf);
	}

	/*
	  init delete vpn dns list
	*/
	bzero(nodename, sizeof(nodename));
	snprintf(nodename, sizeof(nodename) - 1
		, "%s", VPN_DELENTRY);
	for ( idx = 0; idx < VPN_ATTACH_NUM; idx ++)
	{
		snprintf(attrname, sizeof(attrname) - 1
			, "ipDomainNameAddr%d", idx);
		bzero(ipdomain, sizeof(ipdomain));
		if ( 0 != 
			tcapi_get(nodename, attrname, ipdomain)
			|| 0 == ipdomain[0] )
			continue;
		snprintf(attrname, sizeof(attrname) - 1
			, "ipDomainNameIdx%d", idx);
		bzero(vpnentrybuf, sizeof(vpnentrybuf));
		if ( 0 != 
			tcapi_get(nodename, attrname, vpnentrybuf)
			|| 0 == vpnentrybuf[0] )
			continue;
		vpn_entryidx = atoi(vpnentrybuf);
		if ( vpn_entryidx < 0 || vpn_entryidx >= VPN_INSTANCE_NUM )
			continue;

		if ( !(*new_del_dns_vpn_group) )
			*new_del_dns_vpn_group =
			safe_malloc(sizeof(struct vpn_dns_group));

		/* malloc group failed. */
		if ( !(*new_del_dns_vpn_group) )
			return -1;

		(*new_del_dns_vpn_group)->domain = NULL;
		dnsbufLen = strlen(ipdomain) + 1;
		(*new_del_dns_vpn_group)->domain = safe_malloc(dnsbufLen);

		/* copy domain from node. */
		bzero((*new_del_dns_vpn_group)->domain, dnsbufLen);
		strncpy((*new_del_dns_vpn_group)->domain
			, ipdomain, dnsbufLen);
		(*new_del_dns_vpn_group)->vpn_tunnel_idx = vpn_entryidx;

		(*new_del_dns_vpn_group)->next = NULL;
		new_del_dns_vpn_group = &((*new_del_dns_vpn_group)->next);
	}

	/*
	  init vpn dns group.
	*/
	for ( idx = 0; idx < VPN_INSTANCE_NUM; idx ++)
	{
		snprintf(nodename, sizeof(nodename) - 1
			, VPN_ENTRY"%d", idx);
		bzero(svripAddr, sizeof(svripAddr));
		if ( 0 != 
			tcapi_get(nodename, "serverIpAddr", svripAddr)
			|| 0 == svripAddr[0] )
			continue;

		/* get domain list */
		for ( domain_idx = 0; domain_idx < VPN_ATTACH_NUM; domain_idx ++)
		{
			bzero(ipdomain, sizeof(ipdomain));
			snprintf(attrname, sizeof(attrname) - 1
				, "ipDomainNameAddr%d", domain_idx);
			if ( 0 == 
				tcapi_get(nodename, attrname, ipdomain)
				&& 0 != ipdomain[0] )
			{
				/* delete domains which need to be deleted */
				if ( 0 == checkDelDomain(ipdomain, idx) )
				{
					tcapi_set(nodename, attrname, "");
					continue;
				}

				/* check domain type, continue if IP type address */
				if ( 0 == checkDomainType(ipdomain, idx) )
					continue;

				if ( !(*new_dns_vpn_group) )
					*new_dns_vpn_group =
					safe_malloc(sizeof(struct vpn_dns_group));

				/* malloc group failed. */
				if ( !(*new_dns_vpn_group) )
					return -1;

				(*new_dns_vpn_group)->domain = NULL;
				dnsbufLen = strlen(ipdomain) + 1;
				(*new_dns_vpn_group)->domain = safe_malloc(dnsbufLen);

				/* copy domain from node. */
				bzero((*new_dns_vpn_group)->domain, dnsbufLen);
				strncpy((*new_dns_vpn_group)->domain
					, ipdomain, dnsbufLen);
				(*new_dns_vpn_group)->vpn_tunnel_idx = idx;

				(*new_dns_vpn_group)->next = NULL;
				new_dns_vpn_group = &((*new_dns_vpn_group)->next);
			}
		}
	}

	bzero(nodename, sizeof(nodename));
	snprintf(nodename, sizeof(nodename) - 1
		, "%s", VPN_DELENTRY);
	tcapi_unset(nodename);
	tcapi_save();
	freeDelDomain();

	return 0;
}

/*
init vpn dns data.
*/
int vpnDNSTunnelHandle
(char *domain, struct all_addr *addr, int addrlen, int flags)
{
	int cflags = REG_EXTENDED, status = 0, match_domain = 0;
	int vpn_entry_idx = 0, chidx = 0;
	const size_t nmatch = 1;
	regex_t reg;
	regmatch_t pmatch[1];
	char *pdomain = NULL;
	char pattern[MAXDNAME] = {0};
	struct vpn_dns_group *cur_vpndns_group = NULL;

	if ( !domain || !addr )
		return -1;

	if ( !(flags & F_IPV4) )
	{
		return -2;
	}

	for ( cur_vpndns_group = gVPNDNSGrp; cur_vpndns_group;
		cur_vpndns_group = cur_vpndns_group->next )
	{
		status = -1;
		if ( !(cur_vpndns_group->domain) )
			continue;
		pdomain = cur_vpndns_group->domain;
		if ( pdomain )
		{
			bzero(pattern, sizeof(pattern));
			chidx = 0;
			/* remove regx character "\" as C not support it */
			while ( *pdomain )
			{
				if ( '\\' != *pdomain )
					pattern[chidx++] = *pdomain;
				pdomain ++;
			}

			if ( 0 == pattern[0] )
				continue;
					
			/* 1. compile regx */
			if ( regcomp(&reg, pattern, cflags) < 0 )
			{
				tcdbg_printf("\n[%s]==>regcomp err occur\n"
					,__FUNCTION__);
				continue;
			}
			/* 2. match it */
			status = regexec(&reg, domain, nmatch, pmatch, 0);
			/* 3.  free it */
			regfree(&reg);

			/* match */
			if ( 0 == status )
			{
				match_domain = 1;
				vpn_entry_idx = cur_vpndns_group->vpn_tunnel_idx;
				break;
			}
		}
	}

	if ( match_domain )
	{
		if ( flags & F_IPV4 )
			return addIPv4Host2VPNEntry(addr, 32, vpn_entry_idx);

		return 0;
	}

	return -20;
}

/*
add resolve IPv4 address to VPN LAN Entry.
*/
int addIPv4Host2VPNEntry
(struct all_addr *addr, int mask, int tunnel)
{
	char ipaddr[64] = {0};
	char cmdbuf[256] = {0};
	const char *ebtfrmt =
		"ebtables -t broute -%s vpn_lan_entry%d -p IPv4 "
		"--ip-dst %s/%d "
		"-j mark --mark-or 0x%x --mark-target CONTINUE";
#define VPN_PPP_FWMAKR_START 0x6e0000

	if ( !addr || tunnel < 0 || tunnel >= VPN_INSTANCE_NUM )
		return -1;

	bzero(ipaddr, sizeof(ipaddr));
	inet_ntop(AF_INET, addr
			, ipaddr, sizeof(ipaddr));

	if ( 0 == ipaddr[0] )
		return -2;

	/* delete old rule and add it */
	bzero(cmdbuf, sizeof(cmdbuf));
	snprintf(cmdbuf, sizeof(cmdbuf) - 1
		, ebtfrmt
		, "D"
		, tunnel
		, ipaddr, mask
		, VPN_PPP_FWMAKR_START + tunnel);
	system(cmdbuf);

	bzero(cmdbuf, sizeof(cmdbuf));
	snprintf(cmdbuf, sizeof(cmdbuf) - 1
		, ebtfrmt
		, "A"
		, tunnel
		, ipaddr, mask
		, VPN_PPP_FWMAKR_START + tunnel);
	system(cmdbuf);

	return 0;
}

/*
check domain type, return 0 if domain is IP type
*/
int checkDomainType(char *domain, int tunnel)
{
	char partA[MAXDNAME] = {0}, partB[MAXDNAME] = {0};
	int mask = 0;
	unsigned int addr_idx = 0, laddr_l = 0, laddr_r = 0;

	if ( !domain )
		return -1;

	bzero(partA, sizeof(partA));
	bzero(partB, sizeof(partB));

	/* chek type 1: xxxx.xxxx.xxxx.xxxx */
	if ( 1 == inet_pton(AF_INET, domain, &laddr_l) )
	{
		addIPv4Host2VPNEntry(&laddr_l, 32, tunnel);
		return 0;
	}

	/* chek type 2: xxxx.xxxx.xxxx.xxxx/xx */
	sscanf(domain, "%[^/]/%d", partA, &mask);
	if ( 1 == inet_pton(AF_INET, partA, &laddr_l)
		&& mask > 1 && mask <= 32 )
	{
		addIPv4Host2VPNEntry(&laddr_l, mask, tunnel);
		return 0;
	}

	/* chek type 3: xxxx.xxxx.xxxx.xxxx-xxxx.xxxx.xxxx.xxxx */
	sscanf(domain, "%[^-]-%s", partA, partB);
	if ( 1 == inet_pton(AF_INET, partA, &laddr_l)
		&& 1 == inet_pton(AF_INET, partB, &laddr_r)
		&& 0 == memcmp(&laddr_l, &laddr_r, 3)
		)
	{
		for ( addr_idx = laddr_l; addr_idx <= laddr_r; addr_idx ++ )
		{
			addIPv4Host2VPNEntry(&addr_idx, 32, tunnel);
		}

		return 0;
	}

	return -2;
}


#endif

