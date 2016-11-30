/* crypto/des/ecb3_enc.c */
/* Copyright (C) 1995-1998 Eric Young (eay@cryptsoft.com)
 * All rights reserved.
 *
 * This package is an SSL implementation written
 * by Eric Young (eay@cryptsoft.com).
 * The implementation was written so as to conform with Netscapes SSL.
 * 
 * This library is free for commercial and non-commercial use as long as
 * the following conditions are aheared to.  The following conditions
 * apply to all code found in this distribution, be it the RC4, RSA,
 * lhash, DES, etc., code; not just the SSL code.  The SSL documentation
 * included with this distribution is covered by the same copyright terms
 * except that the holder is Tim Hudson (tjh@cryptsoft.com).
 * 
 * Copyright remains Eric Young's, and as such any Copyright notices in
 * the code are not to be removed.
 * If this package is used in a product, Eric Young should be given attribution
 * as the author of the parts of the library used.
 * This can be in the form of a textual message at program startup or
 * in documentation (online or textual) provided with the package.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    "This product includes cryptographic software written by
 *     Eric Young (eay@cryptsoft.com)"
 *    The word 'cryptographic' can be left out if the rouines from the library
 *    being used are not cryptographic related :-).
 * 4. If you include any Windows specific code (or a derivative thereof) from 
 *    the apps directory (application code) you must include an acknowledgement:
 *    "This product includes software written by Tim Hudson (tjh@cryptsoft.com)"
 * 
 * THIS SOFTWARE IS PROVIDED BY ERIC YOUNG ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * 
 * The licence and distribution terms for any publically available version or
 * derivative of this code cannot be changed.  i.e. this code cannot simply be
 * copied and put under another distribution licence
 * [including the GNU Public Licence.]
 */

#include "ecb3_enc.h"
#include "md5.h"

void DES_encrypt2(DES_LONG *data, DES_key_schedule *ks, int enc)
	{
	register DES_LONG l,r,t,u;

	register const unsigned char *des_SP=(const unsigned char *)DES_SPtrans;

	register DES_LONG *s;

	r=data[0];
	l=data[1];

	/* Things have been modified so that the initial rotate is
	 * done outside the loop.  This required the
	 * DES_SPtrans values in sp.h to be rotated 1 bit to the right.
	 * One perl script later and things have a 5% speed up on a sparc2.
	 * Thanks to Richard Outerbridge <71755.204@CompuServe.COM>
	 * for pointing this out. */
	/* clear the top bits on machines with 8byte longs */
	r=ROTATE(r,29)&0xffffffffL;
	l=ROTATE(l,29)&0xffffffffL;

	s=ks->ks->deslong;
	/* I don't know if it is worth the effort of loop unrolling the
	 * inner loop */
	if (enc)
		{
		D_ENCRYPT(l,r, 0); /*  1 */
		D_ENCRYPT(r,l, 2); /*  2 */
		D_ENCRYPT(l,r, 4); /*  3 */
		D_ENCRYPT(r,l, 6); /*  4 */
		D_ENCRYPT(l,r, 8); /*  5 */
		D_ENCRYPT(r,l,10); /*  6 */
		D_ENCRYPT(l,r,12); /*  7 */
		D_ENCRYPT(r,l,14); /*  8 */
		D_ENCRYPT(l,r,16); /*  9 */
		D_ENCRYPT(r,l,18); /*  10 */
		D_ENCRYPT(l,r,20); /*  11 */
		D_ENCRYPT(r,l,22); /*  12 */
		D_ENCRYPT(l,r,24); /*  13 */
		D_ENCRYPT(r,l,26); /*  14 */
		D_ENCRYPT(l,r,28); /*  15 */
		D_ENCRYPT(r,l,30); /*  16 */
		}
	else
		{
		D_ENCRYPT(l,r,30); /* 16 */
		D_ENCRYPT(r,l,28); /* 15 */
		D_ENCRYPT(l,r,26); /* 14 */
		D_ENCRYPT(r,l,24); /* 13 */
		D_ENCRYPT(l,r,22); /* 12 */
		D_ENCRYPT(r,l,20); /* 11 */
		D_ENCRYPT(l,r,18); /* 10 */
		D_ENCRYPT(r,l,16); /*  9 */
		D_ENCRYPT(l,r,14); /*  8 */
		D_ENCRYPT(r,l,12); /*  7 */
		D_ENCRYPT(l,r,10); /*  6 */
		D_ENCRYPT(r,l, 8); /*  5 */
		D_ENCRYPT(l,r, 6); /*  4 */
		D_ENCRYPT(r,l, 4); /*  3 */
		D_ENCRYPT(l,r, 2); /*  2 */
		D_ENCRYPT(r,l, 0); /*  1 */
		}
	/* rotate and clear the top bits on machines with 8byte longs */
	data[0]=ROTATE(l,3)&0xffffffffL;
	data[1]=ROTATE(r,3)&0xffffffffL;
	l=r=t=u=0;
	}


void DES_encrypt3(DES_LONG *data, DES_key_schedule *ks1,
		  DES_key_schedule *ks2, DES_key_schedule *ks3)
	{
	register DES_LONG l,r;

	l=data[0];
	r=data[1];
	IP(l,r);
	data[0]=l;
	data[1]=r;
	DES_encrypt2((DES_LONG *)data,ks1,DES_ENCRYPT);
	DES_encrypt2((DES_LONG *)data,ks2,DES_DECRYPT);
	DES_encrypt2((DES_LONG *)data,ks3,DES_ENCRYPT);
	l=data[0];
	r=data[1];
	FP(r,l);
	data[0]=l;
	data[1]=r;
	}

void DES_decrypt3(DES_LONG *data, DES_key_schedule *ks1,
		  DES_key_schedule *ks2, DES_key_schedule *ks3)
	{
	register DES_LONG l,r;

	l=data[0];
	r=data[1];
	IP(l,r);
	data[0]=l;
	data[1]=r;
	DES_encrypt2((DES_LONG *)data,ks3,DES_DECRYPT);
	DES_encrypt2((DES_LONG *)data,ks2,DES_ENCRYPT);
	DES_encrypt2((DES_LONG *)data,ks1,DES_DECRYPT);
	l=data[0];
	r=data[1];
	FP(r,l);
	data[0]=l;
	data[1]=r;
	}

void DES_ecb3_encrypt(const_DES_cblock *input, DES_cblock *output,
		      DES_key_schedule *ks1, DES_key_schedule *ks2,
		      DES_key_schedule *ks3,
	     int enc)
	{
	register DES_LONG l0,l1;
	DES_LONG ll[2];
	const unsigned char *in = &(*input)[0];
	unsigned char *out = &(*output)[0];

	c2l(in,l0);
	c2l(in,l1);
	ll[0]=l0;
	ll[1]=l1;
	if (enc)
		DES_encrypt3(ll,ks1,ks2,ks3);
	else
		DES_decrypt3(ll,ks1,ks2,ks3);
	l0=ll[0];
	l1=ll[1];
	l2c(l0,out);
	l2c(l1,out);
	}

#define HPERM_OP(a,t,n,m) ((t)=((((a)<<(16-(n)))^(a))&(m)),\
	(a)=(a)^(t)^(t>>(16-(n))))

static const DES_LONG des_skb[8][64]={
	{
	/* for C bits (numbered as per FIPS 46) 1 2 3 4 5 6 */
	0x00000000L,0x00000010L,0x20000000L,0x20000010L,
	0x00010000L,0x00010010L,0x20010000L,0x20010010L,
	0x00000800L,0x00000810L,0x20000800L,0x20000810L,
	0x00010800L,0x00010810L,0x20010800L,0x20010810L,
	0x00000020L,0x00000030L,0x20000020L,0x20000030L,
	0x00010020L,0x00010030L,0x20010020L,0x20010030L,
	0x00000820L,0x00000830L,0x20000820L,0x20000830L,
	0x00010820L,0x00010830L,0x20010820L,0x20010830L,
	0x00080000L,0x00080010L,0x20080000L,0x20080010L,
	0x00090000L,0x00090010L,0x20090000L,0x20090010L,
	0x00080800L,0x00080810L,0x20080800L,0x20080810L,
	0x00090800L,0x00090810L,0x20090800L,0x20090810L,
	0x00080020L,0x00080030L,0x20080020L,0x20080030L,
	0x00090020L,0x00090030L,0x20090020L,0x20090030L,
	0x00080820L,0x00080830L,0x20080820L,0x20080830L,
	0x00090820L,0x00090830L,0x20090820L,0x20090830L,
	},{
	/* for C bits (numbered as per FIPS 46) 7 8 10 11 12 13 */
	0x00000000L,0x02000000L,0x00002000L,0x02002000L,
	0x00200000L,0x02200000L,0x00202000L,0x02202000L,
	0x00000004L,0x02000004L,0x00002004L,0x02002004L,
	0x00200004L,0x02200004L,0x00202004L,0x02202004L,
	0x00000400L,0x02000400L,0x00002400L,0x02002400L,
	0x00200400L,0x02200400L,0x00202400L,0x02202400L,
	0x00000404L,0x02000404L,0x00002404L,0x02002404L,
	0x00200404L,0x02200404L,0x00202404L,0x02202404L,
	0x10000000L,0x12000000L,0x10002000L,0x12002000L,
	0x10200000L,0x12200000L,0x10202000L,0x12202000L,
	0x10000004L,0x12000004L,0x10002004L,0x12002004L,
	0x10200004L,0x12200004L,0x10202004L,0x12202004L,
	0x10000400L,0x12000400L,0x10002400L,0x12002400L,
	0x10200400L,0x12200400L,0x10202400L,0x12202400L,
	0x10000404L,0x12000404L,0x10002404L,0x12002404L,
	0x10200404L,0x12200404L,0x10202404L,0x12202404L,
	},{
	/* for C bits (numbered as per FIPS 46) 14 15 16 17 19 20 */
	0x00000000L,0x00000001L,0x00040000L,0x00040001L,
	0x01000000L,0x01000001L,0x01040000L,0x01040001L,
	0x00000002L,0x00000003L,0x00040002L,0x00040003L,
	0x01000002L,0x01000003L,0x01040002L,0x01040003L,
	0x00000200L,0x00000201L,0x00040200L,0x00040201L,
	0x01000200L,0x01000201L,0x01040200L,0x01040201L,
	0x00000202L,0x00000203L,0x00040202L,0x00040203L,
	0x01000202L,0x01000203L,0x01040202L,0x01040203L,
	0x08000000L,0x08000001L,0x08040000L,0x08040001L,
	0x09000000L,0x09000001L,0x09040000L,0x09040001L,
	0x08000002L,0x08000003L,0x08040002L,0x08040003L,
	0x09000002L,0x09000003L,0x09040002L,0x09040003L,
	0x08000200L,0x08000201L,0x08040200L,0x08040201L,
	0x09000200L,0x09000201L,0x09040200L,0x09040201L,
	0x08000202L,0x08000203L,0x08040202L,0x08040203L,
	0x09000202L,0x09000203L,0x09040202L,0x09040203L,
	},{
	/* for C bits (numbered as per FIPS 46) 21 23 24 26 27 28 */
	0x00000000L,0x00100000L,0x00000100L,0x00100100L,
	0x00000008L,0x00100008L,0x00000108L,0x00100108L,
	0x00001000L,0x00101000L,0x00001100L,0x00101100L,
	0x00001008L,0x00101008L,0x00001108L,0x00101108L,
	0x04000000L,0x04100000L,0x04000100L,0x04100100L,
	0x04000008L,0x04100008L,0x04000108L,0x04100108L,
	0x04001000L,0x04101000L,0x04001100L,0x04101100L,
	0x04001008L,0x04101008L,0x04001108L,0x04101108L,
	0x00020000L,0x00120000L,0x00020100L,0x00120100L,
	0x00020008L,0x00120008L,0x00020108L,0x00120108L,
	0x00021000L,0x00121000L,0x00021100L,0x00121100L,
	0x00021008L,0x00121008L,0x00021108L,0x00121108L,
	0x04020000L,0x04120000L,0x04020100L,0x04120100L,
	0x04020008L,0x04120008L,0x04020108L,0x04120108L,
	0x04021000L,0x04121000L,0x04021100L,0x04121100L,
	0x04021008L,0x04121008L,0x04021108L,0x04121108L,
	},{
	/* for D bits (numbered as per FIPS 46) 1 2 3 4 5 6 */
	0x00000000L,0x10000000L,0x00010000L,0x10010000L,
	0x00000004L,0x10000004L,0x00010004L,0x10010004L,
	0x20000000L,0x30000000L,0x20010000L,0x30010000L,
	0x20000004L,0x30000004L,0x20010004L,0x30010004L,
	0x00100000L,0x10100000L,0x00110000L,0x10110000L,
	0x00100004L,0x10100004L,0x00110004L,0x10110004L,
	0x20100000L,0x30100000L,0x20110000L,0x30110000L,
	0x20100004L,0x30100004L,0x20110004L,0x30110004L,
	0x00001000L,0x10001000L,0x00011000L,0x10011000L,
	0x00001004L,0x10001004L,0x00011004L,0x10011004L,
	0x20001000L,0x30001000L,0x20011000L,0x30011000L,
	0x20001004L,0x30001004L,0x20011004L,0x30011004L,
	0x00101000L,0x10101000L,0x00111000L,0x10111000L,
	0x00101004L,0x10101004L,0x00111004L,0x10111004L,
	0x20101000L,0x30101000L,0x20111000L,0x30111000L,
	0x20101004L,0x30101004L,0x20111004L,0x30111004L,
	},{
	/* for D bits (numbered as per FIPS 46) 8 9 11 12 13 14 */
	0x00000000L,0x08000000L,0x00000008L,0x08000008L,
	0x00000400L,0x08000400L,0x00000408L,0x08000408L,
	0x00020000L,0x08020000L,0x00020008L,0x08020008L,
	0x00020400L,0x08020400L,0x00020408L,0x08020408L,
	0x00000001L,0x08000001L,0x00000009L,0x08000009L,
	0x00000401L,0x08000401L,0x00000409L,0x08000409L,
	0x00020001L,0x08020001L,0x00020009L,0x08020009L,
	0x00020401L,0x08020401L,0x00020409L,0x08020409L,
	0x02000000L,0x0A000000L,0x02000008L,0x0A000008L,
	0x02000400L,0x0A000400L,0x02000408L,0x0A000408L,
	0x02020000L,0x0A020000L,0x02020008L,0x0A020008L,
	0x02020400L,0x0A020400L,0x02020408L,0x0A020408L,
	0x02000001L,0x0A000001L,0x02000009L,0x0A000009L,
	0x02000401L,0x0A000401L,0x02000409L,0x0A000409L,
	0x02020001L,0x0A020001L,0x02020009L,0x0A020009L,
	0x02020401L,0x0A020401L,0x02020409L,0x0A020409L,
	},{
	/* for D bits (numbered as per FIPS 46) 16 17 18 19 20 21 */
	0x00000000L,0x00000100L,0x00080000L,0x00080100L,
	0x01000000L,0x01000100L,0x01080000L,0x01080100L,
	0x00000010L,0x00000110L,0x00080010L,0x00080110L,
	0x01000010L,0x01000110L,0x01080010L,0x01080110L,
	0x00200000L,0x00200100L,0x00280000L,0x00280100L,
	0x01200000L,0x01200100L,0x01280000L,0x01280100L,
	0x00200010L,0x00200110L,0x00280010L,0x00280110L,
	0x01200010L,0x01200110L,0x01280010L,0x01280110L,
	0x00000200L,0x00000300L,0x00080200L,0x00080300L,
	0x01000200L,0x01000300L,0x01080200L,0x01080300L,
	0x00000210L,0x00000310L,0x00080210L,0x00080310L,
	0x01000210L,0x01000310L,0x01080210L,0x01080310L,
	0x00200200L,0x00200300L,0x00280200L,0x00280300L,
	0x01200200L,0x01200300L,0x01280200L,0x01280300L,
	0x00200210L,0x00200310L,0x00280210L,0x00280310L,
	0x01200210L,0x01200310L,0x01280210L,0x01280310L,
	},{
	/* for D bits (numbered as per FIPS 46) 22 23 24 25 27 28 */
	0x00000000L,0x04000000L,0x00040000L,0x04040000L,
	0x00000002L,0x04000002L,0x00040002L,0x04040002L,
	0x00002000L,0x04002000L,0x00042000L,0x04042000L,
	0x00002002L,0x04002002L,0x00042002L,0x04042002L,
	0x00000020L,0x04000020L,0x00040020L,0x04040020L,
	0x00000022L,0x04000022L,0x00040022L,0x04040022L,
	0x00002020L,0x04002020L,0x00042020L,0x04042020L,
	0x00002022L,0x04002022L,0x00042022L,0x04042022L,
	0x00000800L,0x04000800L,0x00040800L,0x04040800L,
	0x00000802L,0x04000802L,0x00040802L,0x04040802L,
	0x00002800L,0x04002800L,0x00042800L,0x04042800L,
	0x00002802L,0x04002802L,0x00042802L,0x04042802L,
	0x00000820L,0x04000820L,0x00040820L,0x04040820L,
	0x00000822L,0x04000822L,0x00040822L,0x04040822L,
	0x00002820L,0x04002820L,0x00042820L,0x04042820L,
	0x00002822L,0x04002822L,0x00042822L,0x04042822L,
	}};

void DES_set_key_unchecked(const_DES_cblock *key, DES_key_schedule *schedule)
{
	static const int shifts2[16]={0,0,1,1,1,1,1,1,0,1,1,1,1,1,1,0};
	register DES_LONG c,d,t,s,t2;
	register const unsigned char *in;
	register DES_LONG *k;
	register int i;

	k = &schedule->ks->deslong[0];
	in = &(*key)[0];

	c2l(in,c);
	c2l(in,d);

	/* do PC1 in 47 simple operations :-)
	 * Thanks to John Fletcher (john_fletcher@lccmail.ocf.llnl.gov)
	 * for the inspiration. :-) */
	PERM_OP (d,c,t,4,0x0f0f0f0fL);
	HPERM_OP(c,t,-2,0xcccc0000L);
	HPERM_OP(d,t,-2,0xcccc0000L);
	PERM_OP (d,c,t,1,0x55555555L);
	PERM_OP (c,d,t,8,0x00ff00ffL);
	PERM_OP (d,c,t,1,0x55555555L);
	d=	(((d&0x000000ffL)<<16L)| (d&0x0000ff00L)	 |
		 ((d&0x00ff0000L)>>16L)|((c&0xf0000000L)>>4L));
	c&=0x0fffffffL;

	for (i=0; i<ITERATIONS; i++)
		{
		if (shifts2[i])
			{ c=((c>>2L)|(c<<26L)); d=((d>>2L)|(d<<26L)); }
		else
			{ c=((c>>1L)|(c<<27L)); d=((d>>1L)|(d<<27L)); }
		c&=0x0fffffffL;
		d&=0x0fffffffL;
		/* could be a few less shifts but I am to lazy at this
		 * point in time to investigate */
		s=	des_skb[0][ (c	  )&0x3f				]|
			des_skb[1][((c>> 6L)&0x03)|((c>> 7L)&0x3c)]|
			des_skb[2][((c>>13L)&0x0f)|((c>>14L)&0x30)]|
			des_skb[3][((c>>20L)&0x01)|((c>>21L)&0x06) |
						  ((c>>22L)&0x38)];
		t=	des_skb[4][ (d	  )&0x3f				]|
			des_skb[5][((d>> 7L)&0x03)|((d>> 8L)&0x3c)]|
			des_skb[6][ (d>>15L)&0x3f				 ]|
			des_skb[7][((d>>21L)&0x0f)|((d>>22L)&0x30)];

		/* table contained 0213 4657 */
		t2=((t<<16L)|(s&0x0000ffffL))&0xffffffffL;
		*(k++)=ROTATE(t2,30)&0xffffffffL;

		t2=((s>>16L)|(t&0xffff0000L));
		*(k++)=ROTATE(t2,26)&0xffffffffL;
		}
	}

/*****************************************
**3DEC-ECB encrypt for Sim-card separation
**key: input, 16 bytes
**data: input, 8 bytes align, padding 0x20
**c: output, encrypt data(length of c >= length of data)
**data_len: length of data
*****************************************/
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
int enCry_3DES_ecb_for_simCard(unsigned char *data, unsigned char *key, unsigned char *c, int data_len)
{
	int docontinue = 1;
	int data_rest;
	unsigned char *src = NULL;	//data afer padding
	#if 0
	unsigned char *dst = NULL;	//data after decrypt
	#endif
	int len;
	unsigned char tmp[8];
	unsigned char in[8];
	#if 0
	unsigned char out[8];
	#endif
	unsigned char block_key[9];
	DES_key_schedule ks, ks2;
	int i;
	int count;

	//data padding
	data_rest = data_len % 8;
	if(data_rest)
		len = data_len + (8 -data_rest);
	else
		len = data_len;
	src = (unsigned char *)malloc(len);
	#if 0
	dst = (unsigned char *)malloc(len);
	#endif
	if((src == NULL)){
		tcdbg_printf("\nenCry_3DES_ecb_for_simCard: malloc error!!");
		docontinue = 0;
	}
	if(docontinue){
		//data padding
		memset(src, 0, len);
		memcpy(src, data, data_len);
		if(data_rest)
			memset(src+data_len, 0x20, 8-data_rest);
		//set key
		memset(block_key, 0, sizeof(block_key));
		memcpy(block_key, key, 8);
		DES_set_key_unchecked((const_DES_cblock *)block_key, &ks);
		memcpy(block_key, key+8,8);
		DES_set_key_unchecked((const_DES_cblock *)block_key, &ks2);
		#if 0
		tcdbg_printf("\nbefore encrypt:\n");
		for(i = 0; i < len; i++){
			tcdbg_printf("0x%.2X ", *(src+i));
		}
		tcdbg_printf("\n");
		#endif
		//encrypt every 8 bytes
		count = len / 8;
		for(i = 0; i < count; i++){
			memset(tmp, 0, 8);
			memset(in, 0, 8);
			#if 0
			memset(out, 0, 8);
			#endif
			memcpy(tmp, src+8*i, 8);
			//encrypt
			DES_ecb3_encrypt((const_DES_cblock *)tmp, (DES_cblock *)in, &ks, &ks2, &ks, DES_ENCRYPT);
			memcpy(c+8*i, in, 8);
			//decrypt
			#if 0
			DES_ecb3_encrypt((const_DES_cblock *)in, (DES_cblock *)out, &ks, &ks2, &ks, DES_DECRYPT);
			memcpy(dst+8*i, out, 8);
			#endif
		}
		#if 0
		tcdbg_printf("\nafter encrypt:\n");
		for(i = 0; i < len; i++){
			tcdbg_printf("0x%.2X ", *(c+i));
		}
		tcdbg_printf("\n");
		tcdbg_printf("\nafter decrypt:\n");
		for(i = 0; i < len; i++){
			tcdbg_printf("0x%.2X ", *(dst+i));
		}
		tcdbg_printf("\n");
		#endif
	}
	if(src != NULL){
		free(src);
		src = NULL;
	}
	#if 0
	if(dst != NULL){
		free(dst);
		dst = NULL;
	}
	#endif
	return len;
}
#endif

/*****************************************
**3DEC-ECB encrypt
**key: input, 192bits
**data: input, 8 bytes align, PKCS7 padding, <= 16bytes
**c: output, encrypt data
*****************************************/
int enCry(unsigned char *data, unsigned char *key, unsigned char *c)
{
	int docontinue = 1;
	int data_len, data_rest;
	unsigned char *src = NULL;	//data afer padding
	//unsigned char *dst = NULL;	//data after decrypt
	int len;
	unsigned char ch;
	unsigned char tmp[8];
	unsigned char in[8];
	//unsigned char out[8];
	unsigned char block_key[9];
	DES_key_schedule ks, ks2, ks3;
	int i;
	int count;

	//data padding
	data_len = strlen(data);
	data_rest = data_len % 8;
	len = data_len + (8 -data_rest);
	ch = 8 - data_rest;
	src = (unsigned char *)malloc(len);
	//dst = (unsigned char *)malloc(len);
	if((src == NULL) /*|| (dst == NULL)*/){
		tcdbg_printf("\nenCry: malloc error!!");
		docontinue = 0;
	}
	if(docontinue){
		//data padding
		memset(src, 0, len);
		memcpy(src, data, data_len);
		memset(src+data_len, ch, 8-data_rest);
		//set key
		memset(block_key, 0, sizeof(block_key));
		memcpy(block_key, key, 8);
		DES_set_key_unchecked((const_DES_cblock *)block_key, &ks);
		memcpy(block_key, key+8,8);
		DES_set_key_unchecked((const_DES_cblock *)block_key, &ks2);
		memcpy(block_key, key+16, 8);
		DES_set_key_unchecked((const_DES_cblock *)block_key, &ks3);
		#if 0
		tcdbg_printf("\nbefore encrypt:\n");
		for(i = 0; i < len; i++){
			tcdbg_printf("0x%.2X ", *(src+i));
		}
		tcdbg_printf("\n");
		#endif
		//encrypt every 8 bytes
		count = len / 8;
		for(i = 0; i < count; i++){
			memset(tmp, 0, 8);
			memset(in, 0, 8);
			//memset(out, 0, 8);
			memcpy(tmp, src+8*i, 8);
			//encrypt
			DES_ecb3_encrypt((const_DES_cblock *)tmp, (DES_cblock *)in, &ks, &ks2, &ks3, DES_ENCRYPT);
			memcpy(c+8*i, in, 8);
			//decrypt
			//DES_ecb3_encrypt((const_DES_cblock *)in, (DES_cblock *)out, &ks, &ks2, &ks3, DES_DECRYPT);
			//memcpy(dst+8*i, out, 8);
		}
		#if 0
		tcdbg_printf("\nafter encrypt:\n");
		for(i = 0; i < len; i++){
			tcdbg_printf("0x%.2X ", *(c+i));
		}
		tcdbg_printf("\n");
		tcdbg_printf("\nafter decrypt:\n");
		for(i = 0; i < len; i++){
			tcdbg_printf("0x%.2X ", *(dst+i));
		}
		tcdbg_printf("\n");
		#endif
	}
	if(src != NULL){
		free(src);
		src = NULL;
	}
	/*
	if(dst != NULL){
		free(dst);
		dst = NULL;
	}*/
	return len;
}

#if defined(TCSUPPORT_CT_PON_CN_JS)
int make_option60_voip_msg(unsigned char *msg, int msg_size, int modeValue)
#else
int make_option60_voip_msg(unsigned char *msg, int msg_size)
#endif
{
	char serialNum[128] = {0};
	char authName[128] = {0};
	char password[128] = {0};
	unsigned char des3_key[24] = {0};
	unsigned char encrypt_data[136] = {0};
	int encrypt_data_len = 0;
	unsigned char r[8] = {0};
	unsigned char ts[8] = {0};
	MD5_CTX mdContext;
	unsigned char md5_data[144] = {0};
	unsigned char md5_digest[16] = {0};
	int len;
	int i;
	unsigned int temp;

	tcapi_get("VoIPBasic_Entry0", "SIPAuthenticationName", authName);
	tcapi_get("VoIPBasic_Entry0", "SIPPassword", password);
	if(!authName || (strcmp(authName, "")==0) || !password || (strcmp(password, "")==0)){
		tcdbg_printf("\nmake_option60_voip_msg: get AuthName or Password error!!");
		//msg = serial_num + admin/admin
		tcapi_get("DeviceInfo_devParaDynamic","SerialNum",serialNum);
		memcpy(msg, serialNum, 24);
		memcpy(msg+24, "admin/admin", 11);
		len = 35;
	}else{
		srand((unsigned int) time(NULL));
		for(i = 0; i < 2; i++){
			temp = rand();
			memcpy(r+i*4,&temp, 4);
		}
		#if 0
		tcdbg_printf("\nR = ");
		for(i = 0; i< 8; i++){
			tcdbg_printf("0x%.2X ", *(r+i));
		}
		#endif
		temp = (unsigned int)time(NULL);
		memcpy(ts+4, &temp, 4);
		#if 0
		tcdbg_printf("\nTS = ");
		for(i = 0; i< 8; i++){
			tcdbg_printf("0x%.2X ", *(ts+i));
		}
		#endif
		
		memcpy(des3_key, r, 8);
		memcpy(des3_key+8, ts, 8);
#if defined(TCSUPPORT_CT_PON_CN_JS)
		if(3 == modeValue)
		{
			if(strlen(authName) > 122)
			{
				tcdbg_printf("\nLength of authName is too long, Cut it!\n");
				memmove(authName+122, "@voip", 6);
			}
			else
			{
				strcat(authName, "@voip");
			}
			//tcdbg_printf("\n%s\n", authName);
		}
#endif
		encrypt_data_len = enCry((unsigned char *)authName, des3_key, encrypt_data);
		#if 0
		tcdbg_printf("\nC len = %d\n", encrypt_data_len);
		for(i = 0; i< encrypt_data_len; i++){
			tcdbg_printf("0x%.2X ", *(encrypt_data+i));
		}
		#endif
		memcpy(md5_data, r, 8);
		len = strlen(password);
		memcpy(md5_data+8, password, len);
		memcpy(md5_data+8+len, ts, 8);
		len += 16; 
		#if 0
		tcdbg_printf("\nMD5 data = ");
		for(i = 0; i< len; i++){
			tcdbg_printf("0x%.2X ", *(md5_data+i));
		}
		#endif
		MD5Init(&mdContext);
		MD5Update(&mdContext, md5_data, len);
		MD5Final(md5_digest, &mdContext);
		#if 0
		tcdbg_printf("\nKey = ");
		for(i = 0; i< 16; i++){
			tcdbg_printf("0x%.2X ", *(md5_digest+i));
		}
		#endif
#if defined(TCSUPPORT_CT_PON_CZ_GD)
		tcapi_get("DeviceInfo_devParaDynamic","SerialNum",serialNum);
		memcpy(msg, serialNum, 24);
		memset(msg+24, 1, 1 );
		memcpy(msg+25, r, 8);
		memcpy(msg+33, ts, 8);
		memcpy(msg+41, md5_digest, 16);
		if(57+encrypt_data_len <= msg_size){
			memcpy(msg+57, encrypt_data, encrypt_data_len);
			len = 57+encrypt_data_len;
		}else{
			tcdbg_printf("\nmake_option60_voip_msg: UserID too long!");
			len = 41;
		}
#else
		memset(msg, 1, 1 );
		memcpy(msg+1, r, 8);
		memcpy(msg+9, ts, 8);
		memcpy(msg+17, md5_digest, 16);
		if(33+encrypt_data_len <= msg_size){
			memcpy(msg+33, encrypt_data, encrypt_data_len);
			len = 33+encrypt_data_len;
		}else{
			tcdbg_printf("\nmake_option60_voip_msg: UserID too long!");
			len = 33;
		}
#endif
		#if 0
		tcdbg_printf("\nMSG = ");
		for(i = 0; i< len; i++){
			tcdbg_printf("0x%.2X ", *(msg+i));
		}
		tcdbg_printf("\n");
		#endif
	}
	return len;
}

