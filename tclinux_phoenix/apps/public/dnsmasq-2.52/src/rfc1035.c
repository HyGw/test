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

#include "dnsmasq.h"

static int add_resource_record(HEADER *header, char *limit, int *truncp, 
			       unsigned int nameoffset, unsigned char **pp, 
			       unsigned long ttl, unsigned int *offset, unsigned short type, 
			       unsigned short class, char *format, ...);

#define CHECK_LEN(header, pp, plen, len) \
    ((size_t)((pp) - (unsigned char *)(header) + (len)) <= (plen))

#define ADD_RDLEN(header, pp, plen, len) \
    (!CHECK_LEN(header, pp, plen, len) ? 0 : (long)((pp) += (len)), 1)

static int extract_name(HEADER *header, size_t plen, unsigned char **pp, 
			char *name, int isExtract, int extrabytes)
{
  unsigned char *cp = (unsigned char *)name, *p = *pp, *p1 = NULL;
  unsigned int j, l, hops = 0;
  int retvalue = 1;
  
  if (isExtract)
    *cp = 0;

  while (1)
    { 
      unsigned int label_type;

      if (!CHECK_LEN(header, p, plen, 1))
	return 0;
      
      if ((l = *p++) == 0) 
	/* end marker */
	{
	  /* check that there are the correct no of bytes after the name */
	  if (!CHECK_LEN(header, p, plen, extrabytes))
	    return 0;
	  
	  if (isExtract)
	    {
	      if (cp != (unsigned char *)name)
		cp--;
	      *cp = 0; /* terminate: lose final period */
	    }
	  else if (*cp != 0)
	    retvalue = 2;
	  
	  if (p1) /* we jumped via compression */
	    *pp = p1;
	  else
	    *pp = p;
	  
	  return retvalue;
	}

      label_type = l & 0xc0;
      
      if (label_type == 0xc0) /* pointer */
	{ 
	  if (!CHECK_LEN(header, p, plen, 1))
	    return 0;
	      
	  /* get offset */
	  l = (l&0x3f) << 8;
	  l |= *p++;
	  
	  if (!p1) /* first jump, save location to go back to */
	    p1 = p;
	      
	  hops++; /* break malicious infinite loops */
	  if (hops > 255)
	    return 0;
	  
	  p = l + (unsigned char *)header;
	}
      else if (label_type == 0x80)
	return 0; /* reserved */
      else if (label_type == 0x40)
	{ /* ELT */
	  unsigned int count, digs;
	  
	  if ((l & 0x3f) != 1)
	    return 0; /* we only understand bitstrings */

	  if (!isExtract)
	    return 0; /* Cannot compare bitsrings */
	  
	  count = *p++;
	  if (count == 0)
	    count = 256;
	  digs = ((count-1)>>2)+1;
	  
	  /* output is \[x<hex>/siz]. which is digs+9 chars */
	  if (cp - (unsigned char *)name + digs + 9 >= MAXDNAME)
	    return 0;
	  if (!CHECK_LEN(header, p, plen, (count-1)>>3))
	    return 0;

	  *cp++ = '\\';
	  *cp++ = '[';
	  *cp++ = 'x';
	  for (j=0; j<digs; j++)
	    {
	      unsigned int dig;
	      if (j%2 == 0)
		dig = *p >> 4;
	      else
		dig = *p++ & 0x0f;
	      
	      *cp++ = dig < 10 ? dig + '0' : dig + 'A' - 10;
	    } 
	  cp += sprintf((char *)cp, "/%d]", count);
	  /* do this here to overwrite the zero char from sprintf */
	  *cp++ = '.';
	}
      else 
	{ /* label_type = 0 -> label. */
	  if (cp - (unsigned char *)name + l + 1 >= MAXDNAME)
	    return 0;
	  if (!CHECK_LEN(header, p, plen, l))
	    return 0;
	  
	  for(j=0; j<l; j++, p++)
	    if (isExtract)
	      {
		unsigned char c = *p;
		if (isascii(c) && !iscntrl(c) && c != '.')
		  *cp++ = *p;
		else
		  return 0;
	      }
	    else 
	      {
		unsigned char c1 = *cp, c2 = *p;
		
		if (c1 == 0)
		  retvalue = 2;
		else 
		  {
		    cp++;
		    if (c1 >= 'A' && c1 <= 'Z')
		      c1 += 'a' - 'A';
		    if (c2 >= 'A' && c2 <= 'Z')
		      c2 += 'a' - 'A';
		    
		    if (c1 != c2)
		      retvalue =  2;
		  }
	      }
	  
	  if (isExtract)
	    *cp++ = '.';
	  else if (*cp != 0 && *cp++ != '.')
	    retvalue = 2;
	}
    }
}
 
/* Max size of input string (for IPv6) is 75 chars.) */
#define MAXARPANAME 75
static int in_arpa_name_2_addr(char *namein, struct all_addr *addrp)
{
  int j;
  char name[MAXARPANAME+1], *cp1;
  unsigned char *addr = (unsigned char *)addrp;
  char *lastchunk = NULL, *penchunk = NULL;
  
  if (strlen(namein) > MAXARPANAME)
    return 0;

  memset(addrp, 0, sizeof(struct all_addr));

  /* turn name into a series of asciiz strings */
  /* j counts no of labels */
  for(j = 1,cp1 = name; *namein; cp1++, namein++)
    if (*namein == '.')
      {
	penchunk = lastchunk;
        lastchunk = cp1 + 1;
	*cp1 = 0;
	j++;
      }
    else
      *cp1 = *namein;
  
  *cp1 = 0;

  if (j<3)
    return 0;

  if (hostname_isequal(lastchunk, "arpa") && hostname_isequal(penchunk, "in-addr"))
    {
      /* IP v4 */
      /* address arives as a name of the form
	 www.xxx.yyy.zzz.in-addr.arpa
	 some of the low order address octets might be missing
	 and should be set to zero. */
      for (cp1 = name; cp1 != penchunk; cp1 += strlen(cp1)+1)
	{
	  /* check for digits only (weeds out things like
	     50.0/24.67.28.64.in-addr.arpa which are used 
	     as CNAME targets according to RFC 2317 */
	  char *cp;
	  for (cp = cp1; *cp; cp++)
	    if (!isdigit((int)*cp))
	      return 0;
	  
	  addr[3] = addr[2];
	  addr[2] = addr[1];
	  addr[1] = addr[0];
	  addr[0] = atoi(cp1);
	}

      return F_IPV4;
    }
#ifdef HAVE_IPV6
  else if (hostname_isequal(penchunk, "ip6") && 
	   (hostname_isequal(lastchunk, "int") || hostname_isequal(lastchunk, "arpa")))
    {
      /* IP v6:
         Address arrives as 0.1.2.3.4.5.6.7.8.9.a.b.c.d.e.f.ip6.[int|arpa]
    	 or \[xfedcba9876543210fedcba9876543210/128].ip6.[int|arpa]
      
	 Note that most of these the various reprentations are obsolete and 
	 left-over from the many DNS-for-IPv6 wars. We support all the formats
	 that we can since there is no reason not to.
      */

      if (*name == '\\' && *(name+1) == '[' && 
	  (*(name+2) == 'x' || *(name+2) == 'X'))
	{	  
	  for (j = 0, cp1 = name+3; *cp1 && isxdigit((int) *cp1) && j < 32; cp1++, j++)
	    {
	      char xdig[2];
	      xdig[0] = *cp1;
	      xdig[1] = 0;
	      if (j%2)
		addr[j/2] |= strtol(xdig, NULL, 16);
	      else
		addr[j/2] = strtol(xdig, NULL, 16) << 4;
	    }
	  
	  if (*cp1 == '/' && j == 32)
	    return F_IPV6;
	}
      else
	{
	  for (cp1 = name; cp1 != penchunk; cp1 += strlen(cp1)+1)
	    {
	      if (*(cp1+1) || !isxdigit((int)*cp1))
		return 0;
	      
	      for (j = sizeof(struct all_addr)-1; j>0; j--)
		addr[j] = (addr[j] >> 4) | (addr[j-1] << 4);
	      addr[0] = (addr[0] >> 4) | (strtol(cp1, NULL, 16) << 4);
	    }
	  
	  return F_IPV6;
	}
    }
#endif
  
  return 0;
}

static unsigned char *skip_name(unsigned char *ansp, HEADER *header, size_t plen, int extrabytes)
{
  while(1)
    {
      unsigned int label_type;
      
      if (!CHECK_LEN(header, ansp, plen, 1))
	return NULL;
      
      label_type = (*ansp) & 0xc0;

      if (label_type == 0xc0)
	{
	  /* pointer for compression. */
	  ansp += 2;	
	  break;
	}
      else if (label_type == 0x80)
	return NULL; /* reserved */
      else if (label_type == 0x40)
	{
	  /* Extended label type */
	  unsigned int count;
	  
	  if (!CHECK_LEN(header, ansp, plen, 2))
	    return NULL;
	  
	  if (((*ansp++) & 0x3f) != 1)
	    return NULL; /* we only understand bitstrings */
	  
	  count = *(ansp++); /* Bits in bitstring */
	  
	  if (count == 0) /* count == 0 means 256 bits */
	    ansp += 32;
	  else
	    ansp += ((count-1)>>3)+1;
	}
      else
	{ /* label type == 0 Bottom six bits is length */
	  unsigned int len = (*ansp++) & 0x3f;
	  
	  if (!ADD_RDLEN(header, ansp, plen, len))
	    return NULL;

	  if (len == 0)
	    break; /* zero length label marks the end. */
	}
    }

  if (!CHECK_LEN(header, ansp, plen, extrabytes))
    return NULL;
  
  return ansp;
}

static unsigned char *skip_questions(HEADER *header, size_t plen)
{
  int q;
  unsigned char *ansp = (unsigned char *)(header+1);

  for (q = ntohs(header->qdcount); q != 0; q--)
    {
      if (!(ansp = skip_name(ansp, header, plen, 4)))
	return NULL;
      ansp += 4; /* class and type */
    }
  
  return ansp;
}

static unsigned char *skip_section(unsigned char *ansp, int count, HEADER *header, size_t plen)
{
  int i, rdlen;
  
  for (i = 0; i < count; i++)
    {
      if (!(ansp = skip_name(ansp, header, plen, 10)))
	return NULL; 
      ansp += 8; /* type, class, TTL */
      GETSHORT(rdlen, ansp);
      if (!ADD_RDLEN(header, ansp, plen, rdlen))
	return NULL;
    }

  return ansp;
}

/* CRC the question section. This is used to safely detect query 
   retransmision and to detect answers to questions we didn't ask, which 
   might be poisoning attacks. Note that we decode the name rather 
   than CRC the raw bytes, since replies might be compressed differently. 
   We ignore case in the names for the same reason. Return all-ones
   if there is not question section. */
unsigned int questions_crc(HEADER *header, size_t plen, char *name)
{
  int q;
  unsigned int crc = 0xffffffff;
  unsigned char *p1, *p = (unsigned char *)(header+1);

  for (q = ntohs(header->qdcount); q != 0; q--) 
    {
      if (!extract_name(header, plen, &p, name, 1, 4))
	return crc; /* bad packet */
      
      for (p1 = (unsigned char *)name; *p1; p1++)
	{
	  int i = 8;
	  char c = *p1;

	  if (c >= 'A' && c <= 'Z')
	    c += 'a' - 'A';

	  crc ^= c << 24;
	  while (i--)
	    crc = crc & 0x80000000 ? (crc << 1) ^ 0x04c11db7 : crc << 1;
	}
      
      /* CRC the class and type as well */
      for (p1 = p; p1 < p+4; p1++)
	{
	  int i = 8;
	  crc ^= *p1 << 24;
	  while (i--)
	    crc = crc & 0x80000000 ? (crc << 1) ^ 0x04c11db7 : crc << 1;
	}

      p += 4;
      if (!CHECK_LEN(header, p, plen, 0))
	return crc; /* bad packet */
    }

  return crc;
}


size_t resize_packet(HEADER *header, size_t plen, unsigned char *pheader, size_t hlen)
{
  unsigned char *ansp = skip_questions(header, plen);
    
  /* if packet is malformed, just return as-is. */
  if (!ansp)
    return plen;
  
  if (!(ansp = skip_section(ansp, ntohs(header->ancount) + ntohs(header->nscount) + ntohs(header->arcount),
			    header, plen)))
    return plen;
    
  /* restore pseudoheader */
  if (pheader && ntohs(header->arcount) == 0)
    {
      /* must use memmove, may overlap */
      memmove(ansp, pheader, hlen);
      header->arcount = htons(1);
      ansp += hlen;
    }

  return ansp - (unsigned char *)header;
}

unsigned char *find_pseudoheader(HEADER *header, size_t plen, size_t  *len, unsigned char **p, int *is_sign)
{
  /* See if packet has an RFC2671 pseudoheader, and if so return a pointer to it. 
     also return length of pseudoheader in *len and pointer to the UDP size in *p
     Finally, check to see if a packet is signed. If it is we cannot change a single bit before
     forwarding. We look for SIG and TSIG in the addition section, and TKEY queries (for GSS-TSIG) */
  
  int i, arcount = ntohs(header->arcount);
  unsigned char *ansp = (unsigned char *)(header+1);
  unsigned short rdlen, type, class;
  unsigned char *ret = NULL;

  if (is_sign)
    {
      *is_sign = 0;

      if (header->opcode == QUERY)
	{
	  for (i = ntohs(header->qdcount); i != 0; i--)
	    {
	      if (!(ansp = skip_name(ansp, header, plen, 4)))
		return NULL;
	      
	      GETSHORT(type, ansp); 
	      GETSHORT(class, ansp);
	      
	      if (class == C_IN && type == T_TKEY)
		*is_sign = 1;
	    }
	}
    }
  else
    {
      if (!(ansp = skip_questions(header, plen)))
	return NULL;
    }
    
  if (arcount == 0)
    return NULL;
  
  if (!(ansp = skip_section(ansp, ntohs(header->ancount) + ntohs(header->nscount), header, plen)))
    return NULL; 
  
  for (i = 0; i < arcount; i++)
    {
      unsigned char *save, *start = ansp;
      if (!(ansp = skip_name(ansp, header, plen, 10)))
	return NULL; 

      GETSHORT(type, ansp);
      save = ansp;
      GETSHORT(class, ansp);
      ansp += 4; /* TTL */
      GETSHORT(rdlen, ansp);
      if (!ADD_RDLEN(header, ansp, plen, rdlen))
	return NULL;
      if (type == T_OPT)
	{
	  if (len)
	    *len = ansp - start;
	  if (p)
	    *p = save;
	  ret = start;
	}
      else if (is_sign && 
	       i == arcount - 1 && 
	       class == C_ANY && 
	       (type == T_SIG || type == T_TSIG))
	*is_sign = 1;
    }
  
  return ret;
}
      
  
/* is addr in the non-globally-routed IP space? */ 
static int private_net(struct in_addr addr) 
{
  in_addr_t ip_addr = ntohl(addr.s_addr);

  return
    ((ip_addr & 0xFF000000) == 0x7F000000)  /* 127.0.0.0/8    (loopback) */ || 
    ((ip_addr & 0xFFFF0000) == 0xC0A80000)  /* 192.168.0.0/16 (private)  */ ||
    ((ip_addr & 0xFF000000) == 0x0A000000)  /* 10.0.0.0/8     (private)  */ ||
    ((ip_addr & 0xFFF00000) == 0xAC100000)  /* 172.16.0.0/12  (private)  */ ||
    ((ip_addr & 0xFFFF0000) == 0xA9FE0000)  /* 169.254.0.0/16 (zeroconf) */ ;
}

static unsigned char *do_doctor(unsigned char *p, int count, HEADER *header, size_t qlen)
{
  int i, qtype, qclass, rdlen;
  unsigned long ttl;

  for (i = count; i != 0; i--)
    {
      if (!(p = skip_name(p, header, qlen, 10)))
	return 0; /* bad packet */
      
      GETSHORT(qtype, p); 
      GETSHORT(qclass, p);
      GETLONG(ttl, p);
      GETSHORT(rdlen, p);
      
      if ((qclass == C_IN) && (qtype == T_A))
	{
	  struct doctor *doctor;
	  struct in_addr addr;
	  
	  if (!CHECK_LEN(header, p, qlen, INADDRSZ))
	    return 0;
	   
	   /* alignment */
	  memcpy(&addr, p, INADDRSZ);
	  
	  for (doctor = daemon->doctors; doctor; doctor = doctor->next)
	    {
	      if (doctor->end.s_addr == 0)
		{
		  if (!is_same_net(doctor->in, addr, doctor->mask))
		    continue;
		}
	      else if (ntohl(doctor->in.s_addr) > ntohl(addr.s_addr) || 
		       ntohl(doctor->end.s_addr) < ntohl(addr.s_addr))
		continue;
	     
	      addr.s_addr &= ~doctor->mask.s_addr;
	      addr.s_addr |= (doctor->out.s_addr & doctor->mask.s_addr);
	      /* Since we munged the data, the server it came from is no longer authoritative */
	      header->aa = 0;
	      memcpy(p, &addr, INADDRSZ);
	      break;
	    }
	}
      
      if (!ADD_RDLEN(header, p, qlen, rdlen))
	 return 0; /* bad packet */
    }
  
  return p; 
}

static int find_soa(HEADER *header, size_t qlen)
{
  unsigned char *p;
  int qtype, qclass, rdlen;
  unsigned long ttl, minttl = ULONG_MAX;
  int i, found_soa = 0;
  
  /* first move to NS section and find TTL from any SOA section */
  if (!(p = skip_questions(header, qlen)) ||
      !(p = do_doctor(p, ntohs(header->ancount), header, qlen)))
    return 0;  /* bad packet */
  
  for (i = ntohs(header->nscount); i != 0; i--)
    {
      if (!(p = skip_name(p, header, qlen, 10)))
	return 0; /* bad packet */
      
      GETSHORT(qtype, p); 
      GETSHORT(qclass, p);
      GETLONG(ttl, p);
      GETSHORT(rdlen, p);
      
      if ((qclass == C_IN) && (qtype == T_SOA))
	{
	  found_soa = 1;
	  if (ttl < minttl)
	    minttl = ttl;

	  /* MNAME */
	  if (!(p = skip_name(p, header, qlen, 0)))
	    return 0;
	  /* RNAME */
	  if (!(p = skip_name(p, header, qlen, 20)))
	    return 0;
	  p += 16; /* SERIAL REFRESH RETRY EXPIRE */
	  
	  GETLONG(ttl, p); /* minTTL */
	  if (ttl < minttl)
	    minttl = ttl;
	}
      else if (!ADD_RDLEN(header, p, qlen, rdlen))
	return 0; /* bad packet */
    }
  
  /* rewrite addresses in additioal section too */
  if (!do_doctor(p, ntohs(header->arcount), header, qlen))
    return 0;
  
  if (!found_soa)
    minttl = daemon->neg_ttl;

  return minttl;
}

#if defined(TCSUPPORT_URL_ROUTEPOLICY)
#define ROUTEPOLICY_MAX_ENTRY_NUM 8
int doRegularMatch(char *dnsURL, char *routeURL)
{
	int result=0; //0:not match,  1:match
	char key_str[128]={0};
	char *pKeyStart = NULL;
	char *pKeyEnd = NULL;
	char keyLen = 0;
	char routeUrlLen = 0;
	
	if(strncmp(routeURL, dnsURL, strlen(routeURL)) == 0)
	{
		
	//	tcdbg_printf("doRegularMatch:---11---findMatchURL\n");		
		return 1;
	}
	else
	{	
		routeUrlLen = strlen(routeURL);
		if(routeUrlLen <= 2 )
		{
		//	tcdbg_printf("doRegularMatch:---22---not findMatchURL\n");	
			return 0;	
		}

		if(routeURL[0] == '*')
		{
			pKeyStart = &(routeURL[1]);
		}

		if(routeURL[routeUrlLen-1] == '*')
		{
			pKeyEnd = &(routeURL[routeUrlLen-2]);
		}
		
		if(pKeyStart == NULL)
		{		
			if(pKeyEnd != NULL)
			{
				strncpy(key_str, routeURL, (pKeyEnd-routeURL));

			}
			else
			{
			//	tcdbg_printf("doRegularMatch:---33---not findMatchURL\n");	
			
				return 0;
			}
		}
		else
		{
			
			if(pKeyEnd == NULL)
			{			
				strncpy(key_str, pKeyStart, strlen(pKeyStart));
			}
			else
			{
				strncpy(key_str, pKeyStart, (pKeyEnd-pKeyStart+1));
			}	
		}		
		
		if(strlen(key_str) > 0)
		{
		
			if(strstr(dnsURL, key_str) != NULL)
			{
			//	tcdbg_printf("doRegularMatch:--44-findmatch\n"); 	
			
				return 1;
			}
		}
	}

//	tcdbg_printf("doRegularMatch:---55---not findMatchURL\n");	

	return 0;
}

int updateUrlRoutePolicyEntry(char *dnsURL, struct all_addr addr, int index)
{
	FILE *fp = NULL;
	FILE *fp1 = NULL;
	char fileName[64]={0};
	char tmp_fileName[64]={0};
	char ruleEntry[256]={0};
	char newRuleEntry[256]={0};
	char *url_key = NULL;
	char *ip_key = NULL;
	char routeURL[128] = {0};
	char oldEntryIp[32]={0};
	int urlLen = 0;
	int matchItem = 0;
	int entryNum = 0;
	int ret_code = 0;//0 : no update,  1: add a new entry, 2:update an old entry
	
#define  FIRST_STR_CMD_URL 		"/usr/bin/iptables -t mangle -I routep_chain"
#define  STR_DEL_CMD_URL 		"/usr/bin/iptables -t mangle -D routep_chain"
#define MARK_BASE 0x1000000
	char urlIpMask[32] = {0};
	char pvcIndex[32] = {0};
	char nodeName[32]={0};
	char rpcmd[512];
	char rpcmd_last[128];
	char ipDst_cmd[64];
	int pvcId = 0;	
	
	sprintf(fileName, "/tmp/url_routePolicy%d", index);
	fp = fopen(fileName, "a+");
	if(fp == NULL)
	{
	//	tcdbg_printf("updateUrlRoutePolicyEntry:---open %s fail!\n",fileName);			
		return ret_code;//no entry update
	}
	
	fseek(fp, 0, 0);

	sprintf(tmp_fileName, "/tmp/tmp_url_routePolicy");
	fp1 = fopen(tmp_fileName, "w+");
	if(fp1 == NULL)
	{
	//	tcdbg_printf("updateUrlRoutePolicyEntry:---open %s fail!\n",tmp_fileName);	
		fclose(fp);
		return ret_code;//no entry update
	}
	
	fseek(fp1, 0, 0);
	
	while(1)
	{
		memset(ruleEntry, 0, sizeof(ruleEntry));
		if(fgets(ruleEntry, sizeof(ruleEntry), fp) == NULL)
		{
	//		tcdbg_printf("updateUrlRoutePolicyEntry:---fgets fail!\n");		
			break;
		}
		
	//	tcdbg_printf("updateUrlRoutePolicyEntry:---ruleEntry---%s\n",ruleEntry);		
		entryNum++;
		
		url_key = strstr(ruleEntry,"dnsURL=");
		if(url_key == NULL){
	//		tcdbg_printf("updateUrlRoutePolicyEntry:---not find dnsURL\n");					
			continue;
		}	
		url_key += strlen("dnsURL=");

		ip_key = strstr(ruleEntry,"dnsIP=");
		if(ip_key == NULL){
	//		tcdbg_printf("updateUrlRoutePolicyEntry:---not find dnsIP\n");						
			continue;
		}	
		urlLen = ip_key - url_key - 1;
		ip_key += strlen("dnsIP=");
	//	tcdbg_printf("updateUrlRoutePolicyEntry:---ip_key = %s \n",ip_key);	
		
		if(inet_addr(ip_key) == addr.addr.addr4.s_addr)
		{
	//		tcdbg_printf("updateUrlRoutePolicyEntry:---%s has been configured\n",ip_key);	
			fclose(fp);			
			fclose(fp1);			
			return ret_code;//no entry update
		}
		
	//	tcdbg_printf("updateUrlRoutePolicyEntry:---dnsURL=%s\n",dnsURL);		
	//	tcdbg_printf("updateUrlRoutePolicyEntry:---url_key=%s\n",url_key);		
	//	tcdbg_printf("updateUrlRoutePolicyEntry:---urlLen=%d\n",urlLen);		
#if 0		
		if((strlen(dnsURL) == urlLen) && (strncmp(dnsURL, url_key, urlLen) == 0))
		{
		
	//		tcdbg_printf("updateUrlRoutePolicyEntry:---findMatchEntry %d\n",entryNum);		
			matchItem = entryNum;
			sprintf(newRuleEntry, "dnsURL=%s dnsIP=%s\n", dnsURL, inet_ntoa(addr.addr.addr4));
			fputs(newRuleEntry,fp1);
			memset(oldEntryIp, 0, sizeof(oldEntryIp));
			strncpy(oldEntryIp, ip_key, strlen(ip_key)-1);	
		}
		else
#endif			
		{
			fputs(ruleEntry,fp1);	
		}
	}

	if(matchItem != 0)
	{
//		tcdbg_printf("updateUrlRoutePolicyEntry:---update an old entry %d!\n", matchItem);	
		fclose(fp);		

		fp = fopen(fileName, "w");
		if(fp == NULL)
		{
		//	tcdbg_printf("updateUrlRoutePolicyEntry:---open %s fail!\n",fileName);
			fclose(fp1);			
			return ret_code;//no entry update
		}		
		fseek(fp, 0, 0);
		fseek(fp1, 0, 0);
		memset(ruleEntry, 0, sizeof(ruleEntry));
		while((fgets(ruleEntry, sizeof(ruleEntry), fp1) != NULL))
		{
			fputs(ruleEntry, fp);
			memset(ruleEntry, 0, sizeof(ruleEntry));
		}
		ret_code = 2;//update a old entry
	}
	else
	{
	//	tcdbg_printf("updateUrlRoutePolicyEntry:---add a new entry!\n");		
	
		sprintf(newRuleEntry, "dnsURL=%s dnsIP=%s\n", dnsURL, inet_ntoa(addr.addr.addr4));
		fseek(fp, 0, 2);
		fputs(newRuleEntry, fp);
		entryNum++;
		ret_code = 1;//add a new entry
	}
	fclose(fp); 		
	fclose(fp1);


	//tcdbg_printf("updateUrlRoutePolicyEntry:---entryNum=%d\n",entryNum);	

#if 1
	memset(nodeName, 0, sizeof(nodeName));
	sprintf(nodeName, "RoutePolicy_Entry%d", index);
	memset(pvcIndex,0,32);
	if(tcapi_get(nodeName,"PolicyPVC",pvcIndex) != 0)
	{
	//	tcdbg_printf("updateUrlRoutePolicyEntry:--get RoutePolicy_Entry type error!\n");		
		return 0;
	}

	pvcId = atoi(pvcIndex); 				

	if(ret_code == 2)
	{
		//delete old iptable rule
		memset(urlIpMask,0,32);
		snprintf(urlIpMask, sizeof(urlIpMask), "255.255.255.255");
		memset(ipDst_cmd,0,64);
		sprintf(ipDst_cmd,"-d %s/%s", oldEntryIp, urlIpMask);


		memset(rpcmd_last,0,128);
		memset(rpcmd,0,512);
		sprintf(rpcmd_last,  "-j RETURN");

		sprintf(rpcmd, "%s %s %s",STR_DEL_CMD_URL, ipDst_cmd, rpcmd_last); 
		system(rpcmd);


	//	tcdbg_printf("delet rule: %s\n",rpcmd);		
		

		memset(rpcmd_last,0,128);
		memset(rpcmd,0,512);
		sprintf(rpcmd_last,  "-j MARK --or-mark 0x%x",MARK_BASE+(pvcId<<21));
		sprintf(rpcmd, "%s %s %s",STR_DEL_CMD_URL, ipDst_cmd, rpcmd_last); 
		system(rpcmd);
 	
	//	tcdbg_printf("delet rule: %s\n",rpcmd);				
	}

	//add a new iptable rule
	memset(urlIpMask,0,32);
	snprintf(urlIpMask, sizeof(urlIpMask), "255.255.255.255");
	memset(ipDst_cmd,0,64);
	sprintf(ipDst_cmd,"-d %s/%s", inet_ntoa(addr.addr.addr4),urlIpMask);


	memset(rpcmd_last,0,128);
	memset(rpcmd,0,512);
	sprintf(rpcmd_last,  "-j RETURN");

	sprintf(rpcmd, "%s %s %s",FIRST_STR_CMD_URL, ipDst_cmd, rpcmd_last); 
	system(rpcmd); 


	memset(rpcmd_last,0,128);
	memset(rpcmd,0,512);
	sprintf(rpcmd_last,  "-j MARK --or-mark 0x%x",MARK_BASE+(pvcId<<21));
	sprintf(rpcmd, "%s %s %s",FIRST_STR_CMD_URL, ipDst_cmd, rpcmd_last); 
	system(rpcmd); 	
#endif
	return ret_code;//entry update

}
#if 0
int clearOldPolicyRouteByURL(char *dnsURL, int index)
{
	FILE *fp = NULL;
	FILE *fp1 = NULL;
	char fileName[64]={0};
	char tmp_fileName[64]={0};
	char ruleEntry[256]={0};
	char newRuleEntry[256]={0};
	char *url_key = NULL;
	char *ip_key = NULL;
	char routeURL[128] = {0};
	char oldEntryIp[32]={0};
	int urlLen = 0;
	int matchItem = 0;
	int entryNum = 0;
	int ret_code = 0;//0 : no update,  1: delete all old relate entry
	
#define  FIRST_STR_CMD_URL 		"/usr/bin/iptables -t mangle -I routep_chain"
#define  STR_DEL_CMD_URL 		"/usr/bin/iptables -t mangle -D routep_chain"
#define MARK_BASE 0x1000000
	char urlIpMask[32] = {0};
	char pvcIndex[32] = {0};
	char nodeName[32]={0};
	char rpcmd[512];
	char rpcmd_last[128];
	char ipDst_cmd[64];
	int pvcId = 0;	

	memset(nodeName, 0, sizeof(nodeName));
	sprintf(nodeName, "RoutePolicy_Entry%d", index);
	memset(pvcIndex,0,32);
	if(tcapi_get(nodeName,"PolicyPVC",pvcIndex) != 0)
	{
	//	tcdbg_printf("clearOldPolicyRouteByURL:--get RoutePolicy_Entry type error!\n");		
		return ret_code;
	}
	
	sprintf(fileName, "/tmp/url_routePolicy%d", index);
	fp = fopen(fileName, "a+");
	if(fp == NULL)
	{
	//	tcdbg_printf("clearOldPolicyRouteByURL:---open %s fail!\n",fileName);			
		return ret_code;//no entry update
	}
	
	fseek(fp, 0, 0);

	sprintf(tmp_fileName, "/tmp/tmp_url_routePolicy");
	fp1 = fopen(tmp_fileName, "w+");
	if(fp1 == NULL)
	{
	//	tcdbg_printf("clearOldPolicyRouteByURL:---open %s fail!\n",tmp_fileName);	
		fclose(fp);
		return ret_code;//no entry update
	}
	
	fseek(fp1, 0, 0);

	pvcId = atoi(pvcIndex); 	
	
	while(1)
	{
		memset(ruleEntry, 0, sizeof(ruleEntry));
		if(fgets(ruleEntry, sizeof(ruleEntry), fp) == NULL)
		{
	//		tcdbg_printf("clearOldPolicyRouteByURL:---fgets fail!\n"); 	
			break;
		}
		
	//	tcdbg_printf("clearOldPolicyRouteByURL:---ruleEntry---%s\n",ruleEntry);		
		entryNum++;
		
		url_key = strstr(ruleEntry,"dnsURL=");
		if(url_key == NULL){
	//		tcdbg_printf("clearOldPolicyRouteByURL:---not find dnsURL\n"); 				
			continue;
		}	
		url_key += strlen("dnsURL=");

		ip_key = strstr(ruleEntry,"dnsIP=");
		if(ip_key == NULL){
	//		tcdbg_printf("clearOldPolicyRouteByURL:---not find dnsIP\n");						
			continue;
		}	
		urlLen = ip_key - url_key - 1;
		ip_key += strlen("dnsIP=");
	//	tcdbg_printf("clearOldPolicyRouteByURL:---ip_key = %s \n",ip_key); 
		
	//	tcdbg_printf("clearOldPolicyRouteByURL:---dnsURL=%s\n",dnsURL);		
	//	tcdbg_printf("clearOldPolicyRouteByURL:---url_key=%s\n",url_key);		
	//	tcdbg_printf("clearOldPolicyRouteByURL:---urlLen=%d\n",urlLen);		
		if((strlen(dnsURL) == urlLen) && (strncmp(dnsURL, url_key, urlLen) == 0))
		{
			matchItem = entryNum;
			memset(oldEntryIp, 0, sizeof(oldEntryIp));
			strncpy(oldEntryIp, ip_key, strlen(ip_key)-1);	
			tcdbg_printf("clearOldPolicyRouteByURL:---delete iptable rule URL = %s, ip = %s\n", dnsURL, oldEntryIp);		
			{
			//delete old iptable rule
			memset(urlIpMask,0,32);
			snprintf(urlIpMask, sizeof(urlIpMask), "255.255.255.255");
			memset(ipDst_cmd,0,64);
			sprintf(ipDst_cmd,"-d %s/%s", oldEntryIp, urlIpMask);


			memset(rpcmd_last,0,128);
			memset(rpcmd,0,512);
			sprintf(rpcmd_last,  "-j RETURN");

			sprintf(rpcmd, "%s %s %s",STR_DEL_CMD_URL, ipDst_cmd, rpcmd_last); 
			system(rpcmd);


		//	tcdbg_printf("delet rule: %s\n",rpcmd); 	
			

			memset(rpcmd_last,0,128);
			memset(rpcmd,0,512);
			sprintf(rpcmd_last,  "-j MARK --or-mark 0x%x",MARK_BASE+(pvcId<<21));
			sprintf(rpcmd, "%s %s %s",STR_DEL_CMD_URL, ipDst_cmd, rpcmd_last); 
			system(rpcmd);
		
		//	tcdbg_printf("delet rule: %s\n",rpcmd); 			
			}			
		}
		else
		{
			fputs(ruleEntry,fp1);	
		}
	}

	if(matchItem != 0)
	{
		fclose(fp); 	

		fp = fopen(fileName, "w");
		if(fp == NULL)
		{
		//	tcdbg_printf("clearOldPolicyRouteByURL:---open %s fail!\n",fileName);
			fclose(fp1);			
			return ret_code;//no entry delete
		}		
		fseek(fp, 0, 0);
		fseek(fp1, 0, 0);
		memset(ruleEntry, 0, sizeof(ruleEntry));
		while((fgets(ruleEntry, sizeof(ruleEntry), fp1) != NULL))
		{
			fputs(ruleEntry, fp);
			memset(ruleEntry, 0, sizeof(ruleEntry));
		}
		ret_code = 2;//clear old related entry
	}

	fclose(fp); 		
	fclose(fp1);

	return ret_code;//entry update

}
#endif

int setPolicyRouteByURL(char *dnsURL, struct all_addr addr, int flag)
{
	char routeURL[64] = {0};
	int i=0;
	char nodeName[32]={0};
	char urlIP[32] = {0};
	char active[4]={0};
	char type[4]={0};
	int findMatchURL = 0;
	int ret = 0;
	
//	tcdbg_printf("setPolicyRoute:---urlIP = %s\n", inet_ntoa(addr.addr.addr4));
//	tcdbg_printf("setPolicyRoute:---dnsURL = %s\n", dnsURL);

	if((strlen(dnsURL) == 0) || (addr.addr.addr4.s_addr == 0))
	{
		return 1;
	}

	for(i=0; i<ROUTEPOLICY_MAX_ENTRY_NUM ; i++)
	{
		memset(nodeName, 0, sizeof(nodeName));
		sprintf(nodeName, "RoutePolicy_Entry%d", i);

		memset(active, 0, sizeof(active));
		if(tcapi_get(nodeName,"Active",active) != 0)
		{
		//	tcdbg_printf("setPolicyRoute:--get RoutePolicy_Entry type error!\n");		
			continue;
		}	
		if(atoi(active) == 0)
		{
			//0: deactive,  1: active
			continue;
		}		
		
		memset(type, 0, sizeof(type));
		if(tcapi_get(nodeName,"Type",type) != 0)
		{
		//	tcdbg_printf("setPolicyRoute:--get RoutePolicy_Entry type error!\n");		
			continue;
		}
//		tcdbg_printf("setPolicyRoute:---type=%d\n", atoi(type));
		
		if(atoi(type) == 1)
		{
			//1: IP mode,  0: URL mode
			continue;
		}

		memset(routeURL, 0, sizeof(routeURL));
		tcapi_get(nodeName,"URL",routeURL);
	//	tcdbg_printf("setPolicyRoute:---routeURL = %s\n", routeURL);
		if(strlen(routeURL) != 0)
		{
			if(doRegularMatch(dnsURL, routeURL))
			{
	//			tcdbg_printf("setPolicyRoute:---findMatchURL\n");		
				findMatchURL = 1;
				break;
			}
		}
		else
		{
			continue;
		}
	}

	if(findMatchURL == 1)
	{
	//	tcdbg_printf("setPolicyRoute:---findMatchURL, updateUrlRoutePolicyEntry\n");
#if 0	
		if(flag == 0)
		{
		//	tcdbg_printf("setPolicyRoute:---first time, clear old relate rule\n");	
			clearOldPolicyRouteByURL(dnsURL, i);
		}
#endif		
		if((ret = updateUrlRoutePolicyEntry(dnsURL, addr, i)) != 0)
		{
//			tcapi_commit(nodeName);
		}

		return 0;		
	}
	
	return 1;

}

#endif


/* Note that the following code can create CNAME chains that don't point to a real record,
   either because of lack of memory, or lack of SOA records.  These are treated by the cache code as 
   expired and cleaned out that way. 
   Return 1 if we reject an address because it look like parct of dns-rebinding attack. */
int extract_addresses(HEADER *header, size_t qlen, char *name, time_t now)
{
  unsigned char *p, *p1, *endrr, *namep;
  int i, j, qtype, qclass, aqtype, aqclass, ardlen, res, searched_soa = 0;
  unsigned long ttl = 0;
  struct all_addr addr;
#if defined(TCSUPPORT_CT_L2TP_VPN)
  char query_domain[MAXDNAME] = {0};
#endif
#if defined(TCSUPPORT_URL_ROUTEPOLICY)
	unsigned char queryURL[64]={0};
	int reval = 0;
	strncpy(queryURL, name, sizeof(queryURL));
#endif

#if defined(TCSUPPORT_CT_L2TP_VPN)
  bzero(query_domain, sizeof(query_domain));
  snprintf(query_domain, sizeof(query_domain) - 1
  	, "%s", name);
#endif
  cache_start_insert();

  /* find_soa is needed for dns_doctor side-effects, so don't call it lazily if there are any. */
  if (daemon->doctors)
    {
      searched_soa = 1;
      ttl = find_soa(header, qlen);
    }
  
  /* go through the questions. */
  p = (unsigned char *)(header+1);
  
  for (i = ntohs(header->qdcount); i != 0; i--)
    {
      int found = 0, cname_count = 5;
      struct crec *cpp = NULL;
      int flags = header->rcode == NXDOMAIN ? F_NXDOMAIN : 0;
      unsigned long cttl = ULONG_MAX, attl;
      
      namep = p;
      if (!extract_name(header, qlen, &p, name, 1, 4))
	return 0; /* bad packet */
           
      GETSHORT(qtype, p); 
      GETSHORT(qclass, p);
      
      if (qclass != C_IN)
	continue;

      /* PTRs: we chase CNAMEs here, since we have no way to 
	 represent them in the cache. */
      if (qtype == T_PTR)
	{ 
	  int name_encoding = in_arpa_name_2_addr(name, &addr);
	  
	  if (!name_encoding)
	    continue;

	  if (!(flags & F_NXDOMAIN))
	    {
	    cname_loop:
	      if (!(p1 = skip_questions(header, qlen)))
		return 0;
	      
	      for (j = ntohs(header->ancount); j != 0; j--) 
		{
		  unsigned char *tmp = namep;
		  /* the loop body overwrites the original name, so get it back here. */
		  if (!extract_name(header, qlen, &tmp, name, 1, 0) ||
		      !(res = extract_name(header, qlen, &p1, name, 0, 10)))
		    return 0; /* bad packet */
		  
		  GETSHORT(aqtype, p1); 
		  GETSHORT(aqclass, p1);
		  GETLONG(attl, p1);
		  GETSHORT(ardlen, p1);
		  endrr = p1+ardlen;
		  
		  /* TTL of record is minimum of CNAMES and PTR */
		  if (attl < cttl)
		    cttl = attl;

		  if (aqclass == C_IN && res != 2 && (aqtype == T_CNAME || aqtype == T_PTR))
		    {
		      if (!extract_name(header, qlen, &p1, name, 1, 0))
			return 0;
		      
		      if (aqtype == T_CNAME)
			{
			  if (!cname_count--)
			    return 0; /* looped CNAMES */
			  goto cname_loop;
			}
		      
		      cache_insert(name, &addr, now, cttl, name_encoding | F_REVERSE);
		      found = 1; 
		    }
		  
		  p1 = endrr;
		  if (!CHECK_LEN(header, p1, qlen, 0))
		    return 0; /* bad packet */
		}
	    }
	  
	   if (!found && !(daemon->options & OPT_NO_NEG))
	    {
	      if (!searched_soa)
		{
		  searched_soa = 1;
		  ttl = find_soa(header, qlen);
		}
	      if (ttl)
		cache_insert(NULL, &addr, now, ttl, name_encoding | F_REVERSE | F_NEG | flags);	
	    }
	}
      else
	{
	  /* everything other than PTR */
	  struct crec *newc;
	  int addrlen;

	  if (qtype == T_A)
	    {
	      addrlen = INADDRSZ;
	      flags |= F_IPV4;
	    }
#ifdef HAVE_IPV6
	  else if (qtype == T_AAAA)
	    {
	      addrlen = IN6ADDRSZ;
	      flags |= F_IPV6;
	    }
#endif
	  else 
	    continue;
	    
	  if (!(flags & F_NXDOMAIN))
	    {
	    cname_loop1:
	      if (!(p1 = skip_questions(header, qlen)))
		return 0;
	      
	      for (j = ntohs(header->ancount); j != 0; j--) 
		{
		  if (!(res = extract_name(header, qlen, &p1, name, 0, 10)))
		    return 0; /* bad packet */
		  
		  GETSHORT(aqtype, p1); 
		  GETSHORT(aqclass, p1);
		  GETLONG(attl, p1);
		  GETSHORT(ardlen, p1);
		  endrr = p1+ardlen;
		  
		  if (aqclass == C_IN && res != 2 && (aqtype == T_CNAME || aqtype == qtype))
		    {
		      if (aqtype == T_CNAME)
			{
			  if (!cname_count--)
			    return 0; /* looped CNAMES */
			  newc = cache_insert(name, NULL, now, attl, F_CNAME | F_FORWARD);
			  if (newc && cpp)
			    {
			      cpp->addr.cname.cache = newc;
			      cpp->addr.cname.uid = newc->uid;
			    }

			  cpp = newc;
			  if (attl < cttl)
			    cttl = attl;
			  
			  if (!extract_name(header, qlen, &p1, name, 1, 0))
			    return 0;
			  goto cname_loop1;
			}
		      else
			{
			  found = 1;
			  
			  /* copy address into aligned storage */
			  if (!CHECK_LEN(header, p1, qlen, addrlen))
			    return 0; /* bad packet */
#if defined(TCSUPPORT_CT_PHONEAPP)
		  if ( aqtype == qtype && 
			  (T_A == aqtype
#ifdef HAVE_IPV6
			  || T_AAAA == aqtype
#endif
			  ))
			  redirectHOST((struct all_addr *)p1, addrlen, flags);
#endif
			  memcpy(&addr, p1, addrlen);
#if defined(TCSUPPORT_CT_L2TP_VPN)
			  vpnDNSTunnelHandle(query_domain, &addr, addrlen, flags);
#endif
			  /* check for returned address in private space */
			  if ((daemon->options & OPT_NO_REBIND) &&
			      (flags & F_IPV4) &&
			      private_net(addr.addr.addr4))
			    return 1;
			  
			  newc = cache_insert(name, &addr, now, attl, flags | F_FORWARD);
			  if (newc && cpp)
			    {
			      cpp->addr.cname.cache = newc;
			      cpp->addr.cname.uid = newc->uid;
			    }
			  cpp = NULL;
#if defined(TCSUPPORT_URL_ROUTEPOLICY)
			  if(found &&  (flags&F_IPV4) && (aqclass == C_IN))
			  {
				  reval = setPolicyRouteByURL(queryURL, addr, 1);

			  }
#endif 
			  
			}
		    }
		  
		  p1 = endrr;
		  if (!CHECK_LEN(header, p1, qlen, 0))
		    return 0; /* bad packet */
		}
	    }
	  
	  if (!found && !(daemon->options & OPT_NO_NEG))
	    {
	      if (!searched_soa)
		{
		  searched_soa = 1;
		  ttl = find_soa(header, qlen);
		}
	      /* If there's no SOA to get the TTL from, but there is a CNAME 
		 pointing at this, inherit its TTL */
	      if (ttl || cpp)
		{
		  newc = cache_insert(name, NULL, now, ttl ? ttl : cttl, F_FORWARD | F_NEG | flags);	
		  if (newc && cpp)
		    {
		      cpp->addr.cname.cache = newc;
		      cpp->addr.cname.uid = newc->uid;
		    }
		}
	    }
	}
    }
  
  /* Don't put stuff from a truncated packet into the cache, but do everything else */
  if (!header->tc)
    cache_end_insert();

  return 0;
}

/* If the packet holds exactly one query
   return F_IPV4 or F_IPV6  and leave the name from the query in name. 
   Abuse F_BIGNAME to indicate an NS query - yuck. */

unsigned short extract_request(HEADER *header, size_t qlen, char *name, unsigned short *typep)
{
  unsigned char *p = (unsigned char *)(header+1);
  int qtype, qclass;

  if (typep)
    *typep = 0;

  if (ntohs(header->qdcount) != 1 || header->opcode != QUERY)
    return 0; /* must be exactly one query. */
  
  if (!extract_name(header, qlen, &p, name, 1, 4))
    return 0; /* bad packet */
   
  GETSHORT(qtype, p); 
  GETSHORT(qclass, p);

  if (typep)
    *typep = qtype;

  if (qclass == C_IN)
    {
      if (qtype == T_A)
	return F_IPV4;
      if (qtype == T_AAAA)
	return F_IPV6;
      if (qtype == T_ANY)
	return  F_IPV4 | F_IPV6;
      if (qtype == T_NS || qtype == T_SOA)
	return F_QUERY | F_BIGNAME;
    }
  
  return F_QUERY;
}


size_t setup_reply(HEADER *header, size_t qlen,
		struct all_addr *addrp, unsigned short flags, unsigned long ttl)
{
  unsigned char *p = skip_questions(header, qlen);
  
  header->qr = 1; /* response */
  header->aa = 0; /* authoritive */
  header->ra = 1; /* recursion if available */
  header->tc = 0; /* not truncated */
  header->nscount = htons(0);
  header->arcount = htons(0);
  header->ancount = htons(0); /* no answers unless changed below */
  if (flags == F_NEG)
    header->rcode = SERVFAIL; /* couldn't get memory */
  else if (flags == F_NOERR)
    header->rcode = NOERROR; /* empty domain */
  else if (flags == F_NXDOMAIN)
    header->rcode = NXDOMAIN;
  else if (p && flags == F_IPV4)
    { /* we know the address */
      header->rcode = NOERROR;
      header->ancount = htons(1);
      header->aa = 1;
      add_resource_record(header, NULL, NULL, sizeof(HEADER), &p, ttl, NULL, T_A, C_IN, "4", addrp);
    }
#ifdef HAVE_IPV6
  else if (p && flags == F_IPV6)
    {
      header->rcode = NOERROR;
      header->ancount = htons(1);
      header->aa = 1;
      add_resource_record(header, NULL, NULL, sizeof(HEADER), &p, ttl, NULL, T_AAAA, C_IN, "6", addrp);
    }
#endif
  else /* nowhere to forward to */
    header->rcode = REFUSED;
 
  return p - (unsigned char *)header;
}

/* check if name matches local names ie from /etc/hosts or DHCP or local mx names. */
int check_for_local_domain(char *name, time_t now)
{
  struct crec *crecp;
  struct mx_srv_record *mx;
  struct txt_record *txt;
  struct interface_name *intr;
  struct ptr_record *ptr;
  
  if ((crecp = cache_find_by_name(NULL, name, now, F_IPV4 | F_IPV6)) &&
      (crecp->flags & (F_HOSTS | F_DHCP)))
    return 1;
  
  for (mx = daemon->mxnames; mx; mx = mx->next)
    if (hostname_isequal(name, mx->name))
      return 1;

  for (txt = daemon->txt; txt; txt = txt->next)
    if (hostname_isequal(name, txt->name))
      return 1;

  for (intr = daemon->int_names; intr; intr = intr->next)
    if (hostname_isequal(name, intr->name))
      return 1;

  for (ptr = daemon->ptr; ptr; ptr = ptr->next)
    if (hostname_isequal(name, ptr->name))
      return 1;
 
  return 0;
}

/* Is the packet a reply with the answer address equal to addr?
   If so mung is into an NXDOMAIN reply and also put that information
   in the cache. */
int check_for_bogus_wildcard(HEADER *header, size_t qlen, char *name, 
			     struct bogus_addr *baddr, time_t now)
{
  unsigned char *p;
  int i, qtype, qclass, rdlen;
  unsigned long ttl;
  struct bogus_addr *baddrp;

  /* skip over questions */
  if (!(p = skip_questions(header, qlen)))
    return 0; /* bad packet */

  for (i = ntohs(header->ancount); i != 0; i--)
    {
      if (!extract_name(header, qlen, &p, name, 1, 10))
	return 0; /* bad packet */
  
      GETSHORT(qtype, p); 
      GETSHORT(qclass, p);
      GETLONG(ttl, p);
      GETSHORT(rdlen, p);
      
      if (qclass == C_IN && qtype == T_A)
	{
	  if (!CHECK_LEN(header, p, qlen, INADDRSZ))
	    return 0;
	  
	  for (baddrp = baddr; baddrp; baddrp = baddrp->next)
	    if (memcmp(&baddrp->addr, p, INADDRSZ) == 0)
	      {
		/* Found a bogus address. Insert that info here, since there no SOA record
		   to get the ttl from in the normal processing */
		cache_start_insert();
		cache_insert(name, NULL, now, ttl, F_IPV4 | F_FORWARD | F_NEG | F_NXDOMAIN | F_CONFIG);
		cache_end_insert();
		
		return 1;
	      }
	}
      
      if (!ADD_RDLEN(header, p, qlen, rdlen))
	return 0;
    }
  
  return 0;
}

static int add_resource_record(HEADER *header, char *limit, int *truncp, unsigned int nameoffset, unsigned char **pp, 
			       unsigned long ttl, unsigned int *offset, unsigned short type, unsigned short class, char *format, ...)
{
  va_list ap;
  unsigned char *sav, *p = *pp;
  int j;
  unsigned short usval;
  long lval;
  char *sval;

  if (truncp && *truncp)
    return 0;

  PUTSHORT(nameoffset | 0xc000, p);
  PUTSHORT(type, p);
  PUTSHORT(class, p);
  PUTLONG(ttl, p);      /* TTL */

  sav = p;              /* Save pointer to RDLength field */
  PUTSHORT(0, p);       /* Placeholder RDLength */

  va_start(ap, format);   /* make ap point to 1st unamed argument */
  
  for (; *format; format++)
    switch (*format)
      {
#ifdef HAVE_IPV6
      case '6':
	sval = va_arg(ap, char *); 
	memcpy(p, sval, IN6ADDRSZ);
	p += IN6ADDRSZ;
	break;
#endif
	
      case '4':
	sval = va_arg(ap, char *); 
	memcpy(p, sval, INADDRSZ);
	p += INADDRSZ;
	break;
	
      case 's':
	usval = va_arg(ap, int);
	PUTSHORT(usval, p);
	break;
	
      case 'l':
	lval = va_arg(ap, long);
	PUTLONG(lval, p);
	break;
	
      case 'd':
	/* get domain-name answer arg and store it in RDATA field */
	if (offset)
	  *offset = p - (unsigned char *)header;
	p = do_rfc1035_name(p, va_arg(ap, char *));
	*p++ = 0;
	break;
	
      case 't':
	usval = va_arg(ap, int);
	sval = va_arg(ap, char *);
	memcpy(p, sval, usval);
	p += usval;
	break;

      case 'z':
	sval = va_arg(ap, char *);
	usval = sval ? strlen(sval) : 0;
	if (usval > 255)
	  usval = 255;
	*p++ = (unsigned char)usval;
	memcpy(p, sval, usval);
	p += usval;
	break;
      }

  va_end(ap);	/* clean up variable argument pointer */
  
  j = p - sav - 2;
  PUTSHORT(j, sav);     /* Now, store real RDLength */
  
  /* check for overflow of buffer */
  if (limit && ((unsigned char *)limit - p) < 0)
    {
      if (truncp)
	*truncp = 1;
      return 0;
    }
  
  *pp = p;
  return 1;
}

static unsigned long crec_ttl(struct crec *crecp, time_t now)
{
  /* Return 0 ttl for DHCP entries, which might change
     before the lease expires. */

  if  (crecp->flags & (F_IMMORTAL | F_DHCP))
    return daemon->local_ttl;
  
  return crecp->ttd - now;
}
  

/* return zero if we can't answer from cache, or packet size if we can */
size_t answer_request(HEADER *header, char *limit, size_t qlen,  
		      struct in_addr local_addr, struct in_addr local_netmask, time_t now) 
{
  char *name = daemon->namebuff;
  unsigned char *p, *ansp, *pheader;
  int qtype, qclass;
  struct all_addr addr;
  unsigned int nameoffset;
  unsigned short flag;
  int q, ans, anscount = 0, addncount = 0;
  int dryrun = 0, sec_reqd = 0;
  int is_sign;
  struct crec *crecp;
  int nxdomain = 0, auth = 1, trunc = 0;
  struct mx_srv_record *rec;
 
#if defined(TCSUPPORT_URL_ROUTEPOLICY)
  unsigned char queryURL[64]={0};
  int reval = 0;
  strncpy(queryURL, name, sizeof(queryURL));
#endif
  
 
  /* If there is an RFC2671 pseudoheader then it will be overwritten by
     partial replies, so we have to do a dry run to see if we can answer
     the query. We check to see if the do bit is set, if so we always
     forward rather than answering from the cache, which doesn't include
     security information. */

  if (find_pseudoheader(header, qlen, NULL, &pheader, &is_sign))
    { 
      unsigned short udpsz, ext_rcode, flags;
      unsigned char *psave = pheader;

      GETSHORT(udpsz, pheader);
      GETSHORT(ext_rcode, pheader);
      GETSHORT(flags, pheader);
      
      sec_reqd = flags & 0x8000; /* do bit */ 

      /* If our client is advertising a larger UDP packet size
	 than we allow, trim it so that we don't get an overlarge
	 response from upstream */

      if (!is_sign && (udpsz > daemon->edns_pktsz))
	PUTSHORT(daemon->edns_pktsz, psave); 

      dryrun = 1;
    }

  if (ntohs(header->qdcount) == 0 || header->opcode != QUERY )
    return 0;
  
  for (rec = daemon->mxnames; rec; rec = rec->next)
    rec->offset = 0;
  
 rerun:
  /* determine end of question section (we put answers there) */
  if (!(ansp = skip_questions(header, qlen)))
    return 0; /* bad packet */
   
  /* now process each question, answers go in RRs after the question */
  p = (unsigned char *)(header+1);

  for (q = ntohs(header->qdcount); q != 0; q--)
    {
      /* save pointer to name for copying into answers */
      nameoffset = p - (unsigned char *)header;

      /* now extract name as .-concatenated string into name */
      if (!extract_name(header, qlen, &p, name, 1, 4))
	return 0; /* bad packet */
            
      GETSHORT(qtype, p); 
      GETSHORT(qclass, p);

      ans = 0; /* have we answered this question */
      
      if (qtype == T_TXT || qtype == T_ANY)
	{
	  struct txt_record *t;
	  for(t = daemon->txt; t ; t = t->next)
	    {
	      if (t->class == qclass && hostname_isequal(name, t->name))
		{
		  ans = 1;
		  if (!dryrun)
		    {
		      log_query(F_CNAME | F_FORWARD | F_CONFIG | F_NXDOMAIN, name, NULL, "<TXT>");
		      if (add_resource_record(header, limit, &trunc, nameoffset, &ansp, 
					      daemon->local_ttl, NULL,
					      T_TXT, t->class, "t", t->len, t->txt))
			anscount++;

		    }
		}
	    }
	}

      if (qclass == C_IN)
	{
	  if (qtype == T_PTR || qtype == T_ANY)
	    {
	      /* see if it's w.z.y.z.in-addr.arpa format */
	      int is_arpa = in_arpa_name_2_addr(name, &addr);
	      struct ptr_record *ptr;
	      struct interface_name* intr = NULL;

	      for (ptr = daemon->ptr; ptr; ptr = ptr->next)
		if (hostname_isequal(name, ptr->name))
		  break;

	      if (is_arpa == F_IPV4)
		for (intr = daemon->int_names; intr; intr = intr->next)
		  {
		    if (addr.addr.addr4.s_addr == get_ifaddr(intr->intr).s_addr)
		      break;
		    else
		      while (intr->next && strcmp(intr->intr, intr->next->intr) == 0)
			intr = intr->next;
		  }
	      
	      if (intr)
		{
		  ans = 1;
		  if (!dryrun)
		    {
		      log_query(F_IPV4 | F_REVERSE | F_CONFIG, intr->name, &addr, NULL);
		      if (add_resource_record(header, limit, &trunc, nameoffset, &ansp, 
					      daemon->local_ttl, NULL,
					      T_PTR, C_IN, "d", intr->name))
			anscount++;
		    }
		}
	      else if (ptr)
		{
		  ans = 1;
		  if (!dryrun)
		    {
		      log_query(F_CNAME | F_FORWARD | F_CONFIG | F_NXDOMAIN, name, NULL, "<PTR>");
		      for (ptr = daemon->ptr; ptr; ptr = ptr->next)
			if (hostname_isequal(name, ptr->name) &&
			    add_resource_record(header, limit, &trunc, nameoffset, &ansp, 
						daemon->local_ttl, NULL,
						T_PTR, C_IN, "d", ptr->ptr))
			  anscount++;
			 
		    }
		}
	      else if ((crecp = cache_find_by_addr(NULL, &addr, now, is_arpa)))
		do 
		  { 
		    /* don't answer wildcard queries with data not from /etc/hosts or dhcp leases */
		    if (qtype == T_ANY && !(crecp->flags & (F_HOSTS | F_DHCP)))
		      continue;
		    
		    if (crecp->flags & F_NEG)
		      {
			ans = 1;
			auth = 0;
			if (crecp->flags & F_NXDOMAIN)
			  nxdomain = 1;
			if (!dryrun)
			  log_query(crecp->flags & ~F_FORWARD, name, &addr, NULL);
		      }
		    else if ((crecp->flags & (F_HOSTS | F_DHCP)) || !sec_reqd)
		      {
			ans = 1;
			if (!(crecp->flags & (F_HOSTS | F_DHCP)))
			  auth = 0;
			if (!dryrun)
			  {
			    log_query(crecp->flags & ~F_FORWARD, cache_get_name(crecp), &addr, 
				      record_source(crecp->uid));
			    
			    if (add_resource_record(header, limit, &trunc, nameoffset, &ansp, 
						    crec_ttl(crecp, now), NULL,
						    T_PTR, C_IN, "d", cache_get_name(crecp)))
			      anscount++;
			  }
		      }
		  } while ((crecp = cache_find_by_addr(crecp, &addr, now, is_arpa)));
	      else if (is_arpa == F_IPV4 && 
		       (daemon->options & OPT_BOGUSPRIV) && 
		       private_net(addr.addr.addr4))
		{
		  /* if not in cache, enabled and private IPV4 address, return NXDOMAIN */
		  ans = 1;
		  nxdomain = 1;
		  if (!dryrun)
		    log_query(F_CONFIG | F_REVERSE | F_IPV4 | F_NEG | F_NXDOMAIN, 
			      name, &addr, NULL);
		}
	    }
	    
	  for (flag = F_IPV4; flag; flag = (flag == F_IPV4) ? F_IPV6 : 0)
	    {
	      unsigned short type = T_A;
	      
	      if (flag == F_IPV6)
#ifdef HAVE_IPV6
		type = T_AAAA;
#else
	        break;
#endif
	      
	      if (qtype != type && qtype != T_ANY)
		continue;
	      
	      /* Check for "A for A"  queries; be rather conservative 
		 about what looks like dotted-quad.  */
	      if (qtype == T_A)
		{
		  char *cp;
		  unsigned int i, a;
		  int x;

		  for (cp = name, i = 0, a = 0; *cp; i++)
		    {
		      if (!isdigit(*cp) || (x = strtol(cp, &cp, 10)) > 255) 
			{
			  i = 5;
			  break;
			}
		      
		      a = (a << 8) + x;
		      
		      if (*cp == '.') 
			cp++;
		    }
		  
		  if (i == 4)
		    {
		      ans = 1;
		      if (!dryrun)
			{
			  addr.addr.addr4.s_addr = htonl(a);
			  log_query(F_FORWARD | F_CONFIG | F_IPV4, name, &addr, NULL);
			  if (add_resource_record(header, limit, &trunc, nameoffset, &ansp, 
						  daemon->local_ttl, NULL, type, C_IN, "4", &addr))
			  {
			    anscount++;
#if defined(TCSUPPORT_URL_ROUTEPOLICY)
					if((type==T_A))
					{
						reval = setPolicyRouteByURL(queryURL, crecp->addr.addr, 1);
					}					
#endif
			  }
			}
		      continue;
		    }
		}

	      /* interface name stuff */
	      if (qtype == T_A)
		{
		  struct interface_name *intr;

		  for (intr = daemon->int_names; intr; intr = intr->next)
		    if (hostname_isequal(name, intr->name))
		      break;
		  
		  if (intr)
		    {
		      ans = 1;
		      if (!dryrun)
			{
			  if ((addr.addr.addr4 = get_ifaddr(intr->intr)).s_addr == (in_addr_t) -1)
			    log_query(F_FORWARD | F_CONFIG | F_IPV4 | F_NEG, name, NULL, NULL);
			  else
			    {
			      log_query(F_FORWARD | F_CONFIG | F_IPV4, name, &addr, NULL);
			      if (add_resource_record(header, limit, &trunc, nameoffset, &ansp, 
						      daemon->local_ttl, NULL, type, C_IN, "4", &addr))
			      {
				anscount++;
#if defined(TCSUPPORT_URL_ROUTEPOLICY)
						if((type==T_A))
						{
							reval = setPolicyRouteByURL(queryURL, crecp->addr.addr, 1);
						}
#endif
						
			      }
			    }
			}
		      continue;
		    }
		}

	    cname_restart:
	      if ((crecp = cache_find_by_name(NULL, name, now, flag | F_CNAME)))
		{
		  int localise = 0;
		  
		  /* See if a putative address is on the network from which we recieved
		     the query, is so we'll filter other answers. */
		  if (local_addr.s_addr != 0 && (daemon->options & OPT_LOCALISE) && flag == F_IPV4)
		    {
		      struct crec *save = crecp;
		      do {
			if ((crecp->flags & F_HOSTS) &&
			    is_same_net(*((struct in_addr *)&crecp->addr), local_addr, local_netmask))
			  {
			    localise = 1;
			    break;
			  } 
			} while ((crecp = cache_find_by_name(crecp, name, now, flag | F_CNAME)));
		      crecp = save;
		    }
			  
		  do
		    { 
		      /* don't answer wildcard queries with data not from /etc/hosts
			 or DHCP leases */
		      if (qtype == T_ANY && !(crecp->flags & (F_HOSTS | F_DHCP)))
			break;
		      
		      if (crecp->flags & F_CNAME)
			{
			  if (!dryrun)
			    {
			      log_query(crecp->flags, name, NULL, record_source(crecp->uid));
			      if (add_resource_record(header, limit, &trunc, nameoffset, &ansp, 
						      crec_ttl(crecp, now), &nameoffset,
						      T_CNAME, C_IN, "d", cache_get_name(crecp->addr.cname.cache)))
				anscount++;
			    }
			  
			  strcpy(name, cache_get_name(crecp->addr.cname.cache));
			  goto cname_restart;
			}
		      
		      if (crecp->flags & F_NEG)
			{
			  ans = 1;
			  auth = 0;
			  if (crecp->flags & F_NXDOMAIN)
			    nxdomain = 1;
			  if (!dryrun)
			    log_query(crecp->flags, name, NULL, NULL);
			}
		      else if ((crecp->flags & (F_HOSTS | F_DHCP)) || !sec_reqd)
			{
			  /* If we are returning local answers depending on network,
			     filter here. */
			  if (localise && 
			      (crecp->flags & F_HOSTS) &&
			      !is_same_net(*((struct in_addr *)&crecp->addr), local_addr, local_netmask))
			    continue;
       
			  if (!(crecp->flags & (F_HOSTS | F_DHCP)))
			    auth = 0;
			  
			  ans = 1;
			  if (!dryrun)
			    {
			      log_query(crecp->flags & ~F_REVERSE, name, &crecp->addr.addr,
					record_source(crecp->uid));
			      
			      if (add_resource_record(header, limit, &trunc, nameoffset, &ansp, 
						      crec_ttl(crecp, now), NULL, type, C_IN, 
						      type == T_A ? "4" : "6", &crecp->addr))
			      {
				anscount++;
#if defined(TCSUPPORT_URL_ROUTEPOLICY)
						if((type==T_A))
						{
							reval = setPolicyRouteByURL(queryURL, crecp->addr.addr, 1);
						}
#endif

			      }
			    }
			}
		    } while ((crecp = cache_find_by_name(crecp, name, now, flag | F_CNAME)));
		}
	    }
	  
	  if (qtype == T_MX || qtype == T_ANY)
	    {
	      int found = 0;
	      for (rec = daemon->mxnames; rec; rec = rec->next)
		if (!rec->issrv && hostname_isequal(name, rec->name))
		  {
		  ans = found = 1;
		  if (!dryrun)
		    {
		      unsigned int offset;
		      log_query(F_CNAME | F_FORWARD | F_CONFIG | F_NXDOMAIN, name, NULL, "<MX>");
		      if (add_resource_record(header, limit, &trunc, nameoffset, &ansp, daemon->local_ttl,
					      &offset, T_MX, C_IN, "sd", rec->weight, rec->target))
			{
			  anscount++;
			  if (rec->target)
			    rec->offset = offset;
			}
		    }
		  }
	      
	      if (!found && (daemon->options & (OPT_SELFMX | OPT_LOCALMX)) && 
		  cache_find_by_name(NULL, name, now, F_HOSTS | F_DHCP))
		{ 
		  ans = 1;
		  if (!dryrun)
		    {
		      log_query(F_CNAME | F_FORWARD | F_CONFIG | F_NXDOMAIN, name, NULL, "<MX>");
		      if (add_resource_record(header, limit, &trunc, nameoffset, &ansp, daemon->local_ttl, NULL, 
					      T_MX, C_IN, "sd", 1, 
					      (daemon->options & OPT_SELFMX) ? name : daemon->mxtarget))
			anscount++;
		    }
		}
	    }
	  	  
	  if (qtype == T_SRV || qtype == T_ANY)
	    {
	      int found = 0;
	      
	      for (rec = daemon->mxnames; rec; rec = rec->next)
		if (rec->issrv && hostname_isequal(name, rec->name))
		  {
		    found = ans = 1;
		    if (!dryrun)
		      {
			unsigned int offset;
			log_query(F_CNAME | F_FORWARD | F_CONFIG | F_NXDOMAIN, name, NULL, "<SRV>");
			if (add_resource_record(header, limit, &trunc, nameoffset, &ansp, daemon->local_ttl, 
						&offset, T_SRV, C_IN, "sssd", 
						rec->priority, rec->weight, rec->srvport, rec->target))
			  {
			    anscount++;
			    if (rec->target)
			      rec->offset = offset;
			  }
		      }
		  }
	      
	      if (!found && (daemon->options & OPT_FILTER) &&  (qtype == T_SRV || (qtype == T_ANY && strchr(name, '_'))))
		{
		  ans = 1;
		  if (!dryrun)
		    log_query(F_CONFIG | F_NEG, name, NULL, NULL);
		}
	    }

	  if (qtype == T_NAPTR || qtype == T_ANY)
	    {
	      struct naptr *na;
	      for (na = daemon->naptr; na; na = na->next)
		if (hostname_isequal(name, na->name))
		  {
		    ans = 1;
		    if (!dryrun)
		      {
			log_query(F_CNAME | F_FORWARD | F_CONFIG | F_NXDOMAIN, name, NULL, "<NAPTR>");
			if (add_resource_record(header, limit, &trunc, nameoffset, &ansp, daemon->local_ttl, 
						NULL, T_NAPTR, C_IN, "sszzzd", 
						na->order, na->pref, na->flags, na->services, na->regexp, na->replace))
			  anscount++;
		      }
		  }
	    }
	  
	  if (qtype == T_MAILB)
	    ans = 1, nxdomain = 1;

	  if (qtype == T_SOA && (daemon->options & OPT_FILTER))
	    {
	      ans = 1; 
	      if (!dryrun)
		log_query(F_CONFIG | F_NEG, name, &addr, NULL);
	    }
	}

      if (!ans)
	return 0; /* failed to answer a question */
    }
  
  if (dryrun)
    {
      dryrun = 0;
      goto rerun;
    }
  
  /* create an additional data section, for stuff in SRV and MX record replies. */
  for (rec = daemon->mxnames; rec; rec = rec->next)
    if (rec->offset != 0)
      {
	/* squash dupes */
	struct mx_srv_record *tmp;
	for (tmp = rec->next; tmp; tmp = tmp->next)
	  if (tmp->offset != 0 && hostname_isequal(rec->target, tmp->target))
	    tmp->offset = 0;
	
	crecp = NULL;
	while ((crecp = cache_find_by_name(crecp, rec->target, now, F_IPV4 | F_IPV6)))
	  {
#ifdef HAVE_IPV6
	    int type =  crecp->flags & F_IPV4 ? T_A : T_AAAA;
#else
	    int type = T_A;
#endif
	    if (crecp->flags & F_NEG)
	      continue;

	    if (add_resource_record(header, limit, NULL, rec->offset, &ansp, 
				    crec_ttl(crecp, now), NULL, type, C_IN, 
				    crecp->flags & F_IPV4 ? "4" : "6", &crecp->addr))
	    {
	      addncount++;
#if defined(TCSUPPORT_URL_ROUTEPOLICY)
			  if((type==T_A))
			  {
				  reval = setPolicyRouteByURL(queryURL, crecp->addr.addr, 1);
			  }
#endif 
		  
	    }
	  }
      }
  
  /* done all questions, set up header and return length of result */
  header->qr = 1; /* response */
  header->aa = auth; /* authoritive - only hosts and DHCP derived names. */
  header->ra = 1; /* recursion if available */
  header->tc = trunc; /* truncation */
  if (anscount == 0 && nxdomain)
    header->rcode = NXDOMAIN;
  else
    header->rcode = NOERROR; /* no error */
  header->ancount = htons(anscount);
  header->nscount = htons(0);
  header->arcount = htons(addncount);
  return ansp - (unsigned char *)header;
}





