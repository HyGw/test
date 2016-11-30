#include <asm/tc3162.h>
#include <asm/system.h>
#include "dramtest.h"

//#define DRAM_PROTECT_POLLING

#if defined(DRAM_PROTECT_TEST)
extern int dram_access_detect(void);
#endif

int dram_pat_set(void *startAddr, unsigned long size, unsigned long pattern, int patType,unsigned char wByte)
{
	unsigned char *addr1;
	unsigned char pat1;
	unsigned short *addr2;
	unsigned short pat2;
	unsigned long *addr4;
	unsigned long pat4;
	unsigned char antiPat1;
	unsigned char antiPat2;
	unsigned char antiPat4;

	if(startAddr == 0)
	{
		prom_printf("dram_pat_set: input data ERROR!\n");
		return -1;
	}

#if defined(DRAM_PROTECT_TEST)
//	prom_printf("dram_pat_set: invalid dram access detect for read startAddr\n");
#endif

	switch(wByte)
	{
		case 4:
			pat4 = (unsigned long)(pattern & 0xffffffff);
			addr4 = (unsigned long*)startAddr;
			size = size >> 2;
			MP_DBG(DBG_L1, "dram_pat_set: pat=0x%x wByte=%d patType=%d \n",pat4, wByte, patType);
		#if defined(DRAM_PROTECT_TEST)
			#ifdef DRAM_PROTECT_POLLING
			if (dram_access_detect()){
				prom_printf("read invalid virual address %X value %X\n", addr4, *addr4);
			}
			#endif
		#endif
			break;
		case 2:
			pat2 = (unsigned short)(pattern & 0xffff);
			addr2 = (unsigned short*)startAddr;
			size = size >> 1;
			MP_DBG(DBG_L1, "dram_pat_set: pat=0x%x wByte=%d patType=%d \n",pat2, wByte, patType);
		#if defined(DRAM_PROTECT_TEST)
			#ifdef DRAM_PROTECT_POLLING
			if (dram_access_detect()){
				prom_printf("read invalid virual address %X value %X\n", addr2, *addr2);
			}
			#endif
		#endif
			break;
		case 1:
			pat1 = (unsigned char)(pattern & 0xff);
			addr1 = (unsigned char*)startAddr;
			MP_DBG(DBG_L1, "dram_pat_set: pat=0x%x wByte=%d patType=%d \n",pat1, wByte, patType);
		#if defined(DRAM_PROTECT_TEST)
			#ifdef DRAM_PROTECT_POLLING
			if (dram_access_detect()){
				prom_printf("read invalid virual address %X value %X\n", addr1, *addr1);
			}
			#endif
		#endif
			break;
		default:
			prom_printf("dram_pat_set: ERROR! wByte=%d \n", wByte);
			return -1;
	}

#if defined(DRAM_PROTECT_TEST)
//	prom_printf("dram_pat_set: invalid dram access detect for set dram address\n");
#endif

	while(size)
	{
		switch(wByte)
		{
			case 4:
				if(patType == ANTI_INCR_PAT)
				{
					antiPat4 = ~pat4;
					*addr4 = antiPat4;
				}
				else
				{
					*addr4 = pat4;
				}

			#if defined(DRAM_PROTECT_TEST)
				#ifdef DRAM_PROTECT_POLLING
				if (dram_access_detect()){
					prom_printf("write invalid virual address %X\n", addr4);
					prom_printf("write pattern %X\n", pat4);
				}
				#endif
			#endif

				addr4++;
				if((patType == INCR_PAT) || (patType == ANTI_INCR_PAT))
				{
					pat4++;
				}
				break;
			case 2:
				if(patType == ANTI_INCR_PAT)
				{
					antiPat2 = ~pat2;
					*addr2 = antiPat2;
				}
				else
				{
					*addr2 = pat2;
				}

			#if defined(DRAM_PROTECT_TEST)
				#ifdef DRAM_PROTECT_POLLING
				if (dram_access_detect()){
					prom_printf("write invalid virual address %X\n", addr2);
					prom_printf("write pattern %X\n", pat2);
				}
				#endif
			#endif

				addr2++;
				if((patType == INCR_PAT) || (patType == ANTI_INCR_PAT))
				{
					pat2++;
				}
				break;
			case 1:
				if(patType == ANTI_INCR_PAT)
				{
					antiPat1 = ~pat1;
					*addr1 = antiPat1;
				}
				else
				{
					*addr1 = pat1;
				}

			#if defined(DRAM_PROTECT_TEST)
				#ifdef DRAM_PROTECT_POLLING
				if (dram_access_detect()){
					prom_printf("write invalid virual address 0x%X\n", addr1);
					prom_printf("write pattern 0x%X\n", pat1);
				}
				#endif
			#endif

				addr1++;
				if((patType == INCR_PAT) || (patType == ANTI_INCR_PAT))
				{
					pat1++;
				}
				break;
			default:
				prom_printf("dram_pat_set: ERROR! wByte=%d \n", wByte);
				return -1;
		}

		size--;
	}

	return 0;
}

int dram_pat_cmp(void *startAddr, unsigned long size, unsigned long pattern, unsigned char wByte, unsigned char rByte)
{
	unsigned char *addr1;
	unsigned char pat1;
	unsigned short *addr2;
	unsigned short pat2;
	unsigned long *addr4;
	unsigned long pat4;
	unsigned long wPat;
	unsigned char pNum;
	unsigned long rPat[4] = {0};
	int i, n;
    unsigned int dramSize = VPint(0xbfa00034);

	if(startAddr == 0)
	{
		prom_printf("dram_pat_cmp: input data ERROR!\n");
		return -1;
	}

	switch(wByte)
	{
		case 4:
			wPat = pattern & 0xffffffff;
			break;
		case 2:
			wPat = pattern & 0xffff;
			break;
		case 1:
			wPat = pattern & 0xff;
			break;
		default:
			prom_printf("dram_pat_cmp: ERROR! wByte=%d \n", wByte);
			return -1;
	}
	MP_DBG(DBG_L1, "dram_pat_cmp: wPat=0x%x, wByte=%d \n",wPat, wByte);

	switch(rByte)
	{
		case 4:
			addr4 = (unsigned long*)startAddr;
			size = size >> 2;
		#if defined(DRAM_PROTECT_TEST)
			#ifdef DRAM_PROTECT_POLLING
			if (dram_access_detect()){
				prom_printf("read invalid virual address %X value %X\n", addr4, *addr4);
			}
			#endif
		#endif
			break;
		case 2:
			addr2 = (unsigned short*)startAddr;
			size = size >> 1;
		#if defined(DRAM_PROTECT_TEST)
			#ifdef DRAM_PROTECT_POLLING
			if (dram_access_detect()){
				prom_printf("read invalid virual address %X value %X\n", addr2, *addr2);
			}
			#endif
		#endif
			break;
		case 1:
			addr1 = (unsigned char*)startAddr;
		#if defined(DRAM_PROTECT_TEST)
			#ifdef DRAM_PROTECT_POLLING
			if (dram_access_detect()){
				prom_printf("read invalid virual address %X value %X\n", addr1, *addr1);
			}
			#endif
		#endif
			break;
		default:
			prom_printf("dram_pat_cmp: ERROR! read byte=%d \n", rByte);
			return -1;
	}

	if(wByte > rByte)
	{
		pNum = wByte / rByte;
		for(i=pNum-1; i>=0 ;i--)
		{
			switch(rByte)
			{
				case 2:
					rPat[i] = wPat & 0xffff;
					wPat = wPat >> 16;
					break;
				case 1:
					rPat[i] = wPat & 0xff;
					wPat = wPat >> 8;
					break;
				default:
					prom_printf("dram_pat_cmp: ERROR! read byte=%d \n", rByte);
					return -1;
			}
		}
	}
	else if(wByte < rByte)
	{
		pNum = 1;
		n = rByte / wByte;
		for(i=0; i<(n-1); i++)
		{
			switch(wByte)
			{
				case 2:
					wPat = (wPat << 16) | wPat;
					break;
				case 1:
					wPat = (wPat << 8) | wPat;
					break;
				default:
					prom_printf("dram_pat_cmp: ERROR! wByte=%d \n", wByte);
					return -1;
			}
		}
		rPat[0] = wPat;
	}
	else
	{
		pNum = wByte / rByte;
		rPat[0] = wPat;
	}
	for(i=0; i<pNum; i++)
	{
		MP_DBG(DBG_L1, "dram_pat_cmp: rPat[%d]=0x%x, rByte=%d \n", i, rPat[i], rByte);
	}

#if defined(DRAM_PROTECT_TEST)
//	prom_printf("dram_pat_cmp: invalid dram access detect for read dram address\n");
#endif

	i = 0;
	while(size)
	{
		switch(rByte)
		{
			case 4:
                if (addr4 >= dramSize){
                    addr4++; break;
                }
				pat4 = (unsigned long) rPat[i] & 0xffffffff;
				if(*addr4 != pat4)
				{
					prom_printf("dram_pat_cmp ERROR! addr=0x%x result=0x%X pat=0x%X read byte=%d\n", addr4, *addr4, pat4, rByte);
					#if defined(DRAM_PROTECT_TEST)
					#ifdef DRAM_PROTECT_POLLING
					if (dram_access_detect()){
						prom_printf("read invalid virual address %X value %X\n", addr4, *addr4);
					}
					#endif
					#else
					return -1;
					#endif
				}
				addr4++;
				break;
			case 2:
                if (addr2 >= dramSize){
                    addr2++; break;
                }
				pat2 = (unsigned short) rPat[i] & 0xffff;
				if(*addr2 != pat2)
				{
					prom_printf("dram_pat_cmp ERROR! addr=0x%x result=0x%X pat=0x%X read byte=%d\n", addr2, *addr2, pat2, rByte);
					#if defined(DRAM_PROTECT_TEST)
					#ifdef DRAM_PROTECT_POLLING
					if (dram_access_detect()){
						prom_printf("read invalid virual address %X value %X\n", addr2, *addr2);
					}
					#endif
					#else
					return -1;
					#endif
				}
				addr2++;
				break;
			case 1:
                if (addr1 >= dramSize){
                    addr1++; break;
                }
				pat1 = (unsigned char) rPat[i] & 0xff;
				if(*addr1 != pat1)
				{
					prom_printf("dram_pat_cmp ERROR! addr=0x%x result=0x%X pat=0x%X read byte=%d\n", addr1, *addr1, pat1, rByte);
					#if defined(DRAM_PROTECT_TEST)
					#ifdef DRAM_PROTECT_POLLING
					if (dram_access_detect()){
						prom_printf("read invalid virual address %X value %X\n", addr1, *addr1);
					}
					#endif
					#else
					return -1;
					#endif
				}
				addr1++;
				break;
			default:
				prom_printf("dram_pat_cmp: ERROR! read byte=%d \n", rByte);
				return -1;
		}

		#if defined(DRAM_PROTECT_TEST)
		#ifdef DRAM_PROTECT_POLLING
		if (dram_access_detect()){	
			if (rByte == 4 )
				prom_printf("read invalid virual address=%X value=%X pat=%X\n", addr4, *addr4, pat4);
			else if (rByte == 2)
				prom_printf("read invalid virual address=%X value=%X pat=%X\n", addr2, *addr2, pat2);
			else if (rByte == 1)
				prom_printf("read invalid virual address=%X value=%X pat=%X\n", addr1, *addr1, pat1);
		}
		#endif
		#endif

		i++;
		if(i >= pNum)
		{
			i = 0;
		}
		size--;
	}

	return 0;
}

int dram_incrPat_cmp(void *startAddr, unsigned long size, unsigned long pattern, unsigned char wByte, unsigned char rByte)
{
	unsigned char *addr1;
	unsigned char pat1;
	unsigned short *addr2;
	unsigned short pat2;
	unsigned long *addr4;
	unsigned long pat4;
	int i, n;
	unsigned long rPat[4] = {0};
	unsigned char pNum;
	int patIdx = -1;
	unsigned long tmp;
	unsigned char shiftByte;
    unsigned int dramSize = VPint(0xbfa00034);


	if(startAddr == 0)
	{
		prom_printf("dram_incrPat_cmp: input data ERROR!\n");
		return -1;
	}

#if defined(DRAM_PROTECT_TEST)
//	prom_printf("dram_incrPat_cmp: invalid dram access detect for read dram address\n");
#endif

	switch(wByte)
	{
		case 4:
			pat4 = (unsigned long)(pattern & 0xffffffff);
			break;
		case 2:
			pat2 = (unsigned short)(pattern & 0xffff);
			break;
		case 1:
			pat1 = (unsigned char)(pattern & 0xff);
			break;
		default:
			prom_printf("dram_incrPat_cmp: ERROR! wByte=%d \n", wByte);
			return -1;
	}
	switch(rByte)
	{
		case 4:
			addr4 = (unsigned long*)startAddr;
			size = size >> 2;
			#if defined(DRAM_PROTECT_TEST)
			#ifdef DRAM_PROTECT_POLLING
			if (dram_access_detect()){
				prom_printf("read invalid virual address %X value %X\n", addr4, *addr4);
			}
			#endif
			#endif
			break;
		case 2:
			addr2 = (unsigned short*)startAddr;
			size = size >> 1;
			#if defined(DRAM_PROTECT_TEST)
			#ifdef DRAM_PROTECT_POLLING
			if (dram_access_detect()){
				prom_printf("read invalid virual address %X value %X\n", addr2, *addr2);
			}
			#endif
			#endif
			break;
		case 1:
			addr1 = (unsigned char*)startAddr;
			#if defined(DRAM_PROTECT_TEST)
			#ifdef DRAM_PROTECT_POLLING
			if (dram_access_detect()){
				prom_printf("read invalid virual address %X value %X\n", addr1, *addr1);
			}
			#endif
			#endif
			break;
		default:
			prom_printf("dram_incrPat_cmp: ERROR! read byte=%d \n", rByte);
			return -1;
	}

	while(size)
	{
		if(wByte > rByte)
		{
			pNum = wByte / rByte;
			if((patIdx >= pNum-1) || (patIdx == -1))
			{
				switch(wByte)
				{
					case 4:
						tmp = pat4;
						pat4++;
						break;
					case 2:
						tmp = pat2;
						pat2++;
						break;
					case 1:
						tmp = pat1;
						pat1++;
						break;
					default:
						prom_printf("dram_incrPat_cmp: ERROR! wByte=%d \n", wByte);
						return -1;
				}
				for(i=pNum-1; i>=0 ;i--)
				{
					switch(rByte)
					{
						case 2:
							rPat[i] = tmp & 0xffff;
							tmp = tmp >> 16;
							break;
						case 1:
							rPat[i] = tmp & 0xff;
							tmp = tmp >> 8;
							break;
						default:
							prom_printf("dram_incrPat_cmp: ERROR! read byte=%d \n", rByte);
							return -1;
					}
				}
				patIdx = 0;
			}
			else
			{
				patIdx++;
			}
		}
		else if(wByte < rByte)
		{
			/* take wByte=1 rByte=4 pat1=0x01 for example,
			 * it will get rPat[0]= 0 | ((0x01 <<8) | 0x02) in the first for while,
			 * and then get rPat[0]= (0x0102 << 16 )| ((0x03 <<8) | 0x04) in the second for while.
			 * In this case, n=2 , so the result is rPat[0]=0x01020304 */
			patIdx = 0;
			n = rByte / (wByte * 2);
			rPat[patIdx] = 0;
			shiftByte = 1;
			for(i=0; i<n; i++)
			{
				switch(wByte)
				{
					case 2:
						rPat[patIdx] = (rPat[patIdx] << 16) | ((pat2 << 16) | (++pat2));
						pat2++;
						break;
					case 1:
						rPat[patIdx] = (rPat[patIdx] << (8 * shiftByte)) | ((pat1 << 8) | (++pat1));
						pat1++;
						shiftByte = shiftByte << 1;
						break;
					default:
						prom_printf("dram_incrPat_cmp: ERROR! wByte=%d \n", wByte);
						return -1;
				}
			}
		}
		else
		{
			patIdx = 0;
			switch(wByte)
			{
				case 4:
					rPat[patIdx] = pat4;
					pat4++;
					break;
				case 2:
					rPat[patIdx] = pat2;
					pat2++;
					break;
				case 1:
					rPat[patIdx] = pat1;
					pat1++;
					break;
				default:
					prom_printf("dram_incrPat_cmp: ERROR! wByte=%d \n", wByte);
					return -1;
			}
		}

		switch(rByte)
		{
			case 4:
                if (((unsigned long)addr4 & 0x1fffffff) >= dramSize){
                    addr4++; break;
                }
				if(*addr4 != (unsigned long)(rPat[patIdx] & 0xffffffff))
				{
					prom_printf("dram_incrPat_cmp ERROR! addr=0x%x result=0x%X pat=0x%X read byte=%d\n", addr4, *addr4, (unsigned long)(rPat[patIdx] & 0xffffffff), rByte);

					#if defined(DRAM_PROTECT_TEST)
					#ifdef DRAM_PROTECT_POLLING
					if (dram_access_detect()){
						prom_printf("read invalid virual address %X value %X\n", addr4, *addr4);
					}
					#endif
					#else
					return -1;
					#endif
				}
				addr4++;
				break;
			case 2:
                if (((unsigned short)addr2 & 0x1fffffff) >= dramSize){
                    addr2++; break;
                }
				if(*addr2 != (unsigned short)(rPat[patIdx] & 0xffff))
				{
					prom_printf("dram_incrPat_cmp ERROR! addr=0x%x result=0x%X pat=0x%X read byte=%d\n", addr2, *addr2, (unsigned short)(rPat[patIdx] & 0xffff), rByte);

					#if defined(DRAM_PROTECT_TEST)
					#ifdef DRAM_PROTECT_POLLING
					if (dram_access_detect()){
						prom_printf("read invalid virual address %X value %X\n", addr2, *addr2);
					}
					#endif
					#else
					return -1;
					#endif
				}
				addr2++;
				break;
			case 1:
                if (((unsigned char)addr1 & 0x1fffffff) >= dramSize){
                    addr1++; break;
                }
				if(*addr1 != (unsigned char)(rPat[patIdx] & 0xff))
				{
					prom_printf("dram_incrPat_cmp ERROR! addr=0x%x result=0x%X pat=0x%X read byte=%d\n", addr1, *addr1, (unsigned char)(rPat[patIdx] & 0xff), rByte);

					#if defined(DRAM_PROTECT_TEST)
					#ifdef DRAM_PROTECT_POLLING
					if (dram_access_detect()){
						prom_printf("read invalid virual address=%X value=%X\n", addr1, *addr1);
					}
					#endif
					#else
					return -1;
					#endif
				}
				addr1++;
				break;
			default:
				prom_printf("dram_incrPat_cmp: ERROR! read byte=%d \n", rByte);
				return -1;
		}

		#if defined(DRAM_PROTECT_TEST)
		#ifdef DRAM_PROTECT_POLLING
		if (dram_access_detect()){
			if (rByte == 4 )
				prom_printf("read invalid virual address=%X value=%X pat=%X\n", addr4, *addr4, pat4);
			else if (rByte == 2)
				prom_printf("read invalid virual address=%X value=%X pat=%X\n", addr2, *addr2, pat2);
			else if (rByte == 1)
				prom_printf("read invalid virual address=%X value=%X pat=%X\n", addr1, *addr1, pat1);
		}
		#endif
		#endif

		size--;
	}

	return 0;
}

int dram_antiIncrPat_cmp(void *startAddr, unsigned long size, unsigned long pattern, unsigned char wByte)
{
	unsigned char *addr1;
	unsigned char pat1;
	unsigned char antiPat1;
	unsigned short *addr2;
	unsigned short pat2;
	unsigned char antiPat2;
	unsigned long *addr4;
	unsigned long pat4;
	unsigned char antiPat4;
    unsigned int dramSize = VPint(0xbfa00034);


	if(startAddr == 0)
	{
		prom_printf("dram_antiIncrPat_cmp: input data ERROR!\n");
		return -1;
	}

	switch(wByte)
	{
		case 4:
			pat4 = (unsigned long)(pattern & 0xffffffff);
			addr4 = (unsigned long*)startAddr;
			size = size >> 2;
		#if defined(DRAM_PROTECT_TEST)
			#ifdef DRAM_PROTECT_POLLING
			if (dram_access_detect()){
				prom_printf("read invalid virual address %X value %X\n", addr4, *addr4);
			}
			#endif
		#endif
			break;
		case 2:
			pat2 = (unsigned short)(pattern & 0xffff);
			addr2 = (unsigned short*)startAddr;
			size = size >> 1;
		#if defined(DRAM_PROTECT_TEST)
			#ifdef DRAM_PROTECT_POLLING
			if (dram_access_detect()){
				prom_printf("read invalid virual address %X value %X\n", addr2, *addr2);
			}
			#endif
		#endif
			break;
		case 1:
			pat1 = (unsigned char)(pattern & 0xff);
			addr1 = (unsigned char*)startAddr;
		#if defined(DRAM_PROTECT_TEST)
			#ifdef DRAM_PROTECT_POLLING
			if (dram_access_detect()){
				prom_printf("read invalid virual address %X value %X\n", addr1, *addr1);
			}
			#endif
		#endif
			break;
		default:
			prom_printf("dram_antiIncrPat_cmp: ERROR! wByte=%d \n", wByte);
			return -1;
	}

#if defined(DRAM_PROTECT_TEST)
//	prom_printf("dram_antiIncrPat_cmp: invalid dram access detect for read dram address\n");
#endif

	while(size)
	{
		switch(wByte)
		{
			case 4:
                if (addr4 >= dramSize){
                    addr4++; break;
                }
				antiPat4 = ~pat4;
				if(*addr4 != antiPat4)
				{
					prom_printf("dram_antiIncrPat_cmp ERROR! addr=0x%x result=0x%X pat=0x%X wByte=%d\n", addr4, *addr4, pat4, wByte);
					#if defined(DRAM_PROTECT_TEST)
					#ifdef DRAM_PROTECT_POLLING
					if (dram_access_detect()){
						prom_printf("read invalid virual address %X value %X\n", addr4, *addr4);
					}
					#endif
					#else
					return -1;
					#endif
				}
				addr4++;
				pat4++;
				break;
			case 2:
                if (addr2 >= dramSize){
                    addr2++; break;
                }
				antiPat2 = ~pat2;
				if(*addr2 != antiPat2)
				{
					prom_printf("dram_antiIncrPat_cmp ERROR! addr=0x%x result=0x%X pat=0x%X wByte=%d\n", addr2, *addr2, pat2, wByte);
					#if defined(DRAM_PROTECT_TEST)
					#ifdef DRAM_PROTECT_POLLING
					if (dram_access_detect()){
						prom_printf("read invalid virual address %X value %X\n", addr2, *addr2);
					}
					#endif
					#else
					return -1;
					#endif
				}
				addr2++;
				pat2++;
				break;
			case 1:
                if (addr1 >= dramSize){
                    addr1++; break;
                }
				antiPat1 = ~pat1;
				if(*addr1 != antiPat1)
				{
					prom_printf("dram_antiIncrPat_cmp ERROR! addr=0x%x result=0x%X pat=0x%X wByte=%d\n", addr1, *addr1, pat1, wByte);
					#if defined(DRAM_PROTECT_TEST)
					#ifdef DRAM_PROTECT_POLLING
					if (dram_access_detect()){
						prom_printf("read invalid virual address %X value %X\n", addr1, *addr1);
					}
					#endif
					#else
					return -1;
					#endif
				}
				addr1++;
				pat1++;
				break;
			default:
				prom_printf("dram_antiIncrPat_cmp: ERROR! wByte=%d \n", wByte);
				return -1;
		}

		#if defined(DRAM_PROTECT_TEST)
		#ifdef DRAM_PROTECT_POLLING
		if (dram_access_detect()){	
			if (wByte == 4 )
				prom_printf("read invalid virual address=%X value=%X pat=%X\n", addr4, *addr4, pat4);
			else if (wByte == 2)
				prom_printf("read invalid virual address=%X value=%X pat=%X\n", addr2, *addr2, pat2);
			else if (wByte == 1)
				prom_printf("read invalid virual address=%X value=%X pat=%X\n", addr1, *addr1, pat1);
		}
		#endif
		#endif

		size--;
	}

	return 0;
}

int dramTest(dramTest_info_t *info, int isInputPat)
{
	int ret = 0;
	unsigned char defWByte[] = {1, 2 , 4 , 0};
	unsigned char defRByte[] = {1, 2 , 4 , 0};
	int i,j;
	unsigned char byte;
	unsigned char defPat[4] = {0x5a, 0xa5, 0x00, 0xff};
	unsigned char pat;


	if(info == 0)
	{
		prom_printf("dramTest: ERROR! no test info \n");
		return -1;
	}

	/* use input write byte instead of default write byte */
	switch(info->wByte)
	{
		case 1:
		case 2:
		case 4:
			defWByte[0] = info->wByte;
			defWByte[1] = 0;
			break;
		default:
			break;
	}

	for(i=0; defWByte[i]!=0; i++)
	{
		byte = defWByte[i];
		if(isInputPat) /* use the input pattern */
		{
			dram_pat_set(info->startAddr, info->size, info->pattern, ORI_PAT, byte);
			for(j=0; defRByte[j]!=0; j++)
			{
				ret += dram_pat_cmp(info->startAddr, info->size, info->pattern, byte, defRByte[j]);
			}

		}
		else /* use the default patterns, incremental pattern */
		{
			dram_pat_set(info->startAddr, info->size, 0, INCR_PAT, byte);
			for(j=0; defRByte[j]!=0; j++)
			{
				ret += dram_incrPat_cmp(info->startAddr, info->size, 0, byte, defRByte[j]);
			}
		}
	}

	/* use the default patterns, "anti-incremental patten, 0x5a, 0xa5, 0x0, 0xff" */
	if(isInputPat == 0)
	{
		dram_pat_set(info->startAddr, info->size, 0, ANTI_INCR_PAT, 1);
		dram_antiIncrPat_cmp(info->startAddr, info->size, 0, 1);	
		for(i=0; i<4; i++)
		{
			pat = defPat[i];
			dram_pat_set(info->startAddr, info->size, pat, ORI_PAT, 1);
			ret += dram_pat_cmp(info->startAddr, info->size, pat, 1, 1);
		}
	}

	if(ret != 0)
	{
		MP_DBG(DBG_L1, "dramTest FAIL!\n\n");
		return -1;
	}
	else
	{
		MP_DBG(DBG_L1, "dramTest SUCCESS!\n\n");
		return 0;
	}
}



int dram_Nonalignment_Test(dramTest_info_t *info, int isInputPat)
{
	int ret = 0;
	unsigned char defWByte[] = {1, 0 };
	unsigned char defRByte[] = {1, 0};
	int i,j;
	unsigned char byte;
	unsigned char defPat[4] = {0x5a, 0xa5, 0x00, 0xff};
	unsigned char pat;


	if(info == 0)
	{
		prom_printf("dramTest: ERROR! no test info \n");
		return -1;
	}

	/* use input write byte instead of default write byte */
	switch(info->wByte)
	{
		case 1:
		case 2:
		case 4:
			defWByte[0] = info->wByte;
			defWByte[1] = 0;
			break;
		default:
			break;
	}

	for(i=0; defWByte[i]!=0; i++)
	{
		byte = defWByte[i];
		if(isInputPat) /* use the input pattern */
		{
			dram_pat_set(info->startAddr, info->size, info->pattern, ORI_PAT, byte);
			for(j=0; defRByte[j]!=0; j++)
			{
				ret += dram_pat_cmp(info->startAddr, info->size, info->pattern, byte, defRByte[j]);
			}

		}
		else /* use the default patterns, incremental pattern */
		{
			dram_pat_set(info->startAddr, info->size, 0, INCR_PAT, byte);
			for(j=0; defRByte[j]!=0; j++)
			{
				ret += dram_incrPat_cmp(info->startAddr, info->size, 0, byte, defRByte[j]);
			}
		}
	}

	/* use the default patterns, "anti-incremental patten, 0x5a, 0xa5, 0x0, 0xff" */
	if(isInputPat == 0)
	{
		dram_pat_set(info->startAddr, info->size, 0, ANTI_INCR_PAT, 1);
		dram_antiIncrPat_cmp(info->startAddr, info->size, 0, 1);	
		for(i=0; i<4; i++)
		{
			pat = defPat[i];
			dram_pat_set(info->startAddr, info->size, pat, ORI_PAT, 1);
			ret += dram_pat_cmp(info->startAddr, info->size, pat, 1, 1);
		}
	}

	if(ret != 0)
	{
		MP_DBG(DBG_L1, "dramTest FAIL!\n\n");
		return -1;
	}
	else
	{
		MP_DBG(DBG_L1, "dramTest SUCCESS!\n\n");
		return 0;
	}
}


int gdmacpy(unsigned long addr1, unsigned long addr2, unsigned long len)
{
	unsigned char ch, burst_size;
//	ch = random_byte() & 0xf;
	ch = 0xf;
//	burst_size = random_byte() & 0x7;
	burst_size = 0x4;
    if (isEN751221)
        ch = 0;
	if (len == 0)
		len = 1;
	// GDMA
	VPint(CR_GDMA_SA(ch)) = (addr2 & 0x1fffffff);
	VPint(CR_GDMA_DA(ch)) = (addr1 & 0x1fffffff);
	VPint(CR_GDMA_CT1(ch)) = (32<<16) | (32<<8);
	VPint(CR_GDMA_CT0(ch)) = ((len&0xffff)<<16) | (burst_size<<3) | (1<<1) | (1<<0);
    #if 0
    prom_printf("\nGDMA_SA0(0x%x) : 0x%x\n", CR_GDMA_SA(ch), VPint(CR_GDMA_SA(ch)));
    prom_printf("\nGDMA_DA0(0x%x) : 0x%x\n", CR_GDMA_DA(ch), VPint(CR_GDMA_DA(ch)));
    prom_printf("\nGDMA_CT00(0x%x) : 0x%x\n", CR_GDMA_CT0(ch), VPint(CR_GDMA_CT0(ch)));
    prom_printf("\nGDMA_CT01(0x%x) : 0x%x\n", CR_GDMA_CT1(ch), VPint(CR_GDMA_CT1(ch)));
    prom_printf("\nGDMA_DONE(0x%x) : 0x%x\n", CR_GDMA_DONEINT, VPint(CR_GDMA_DONEINT));
    #endif
	while (!(VPint(CR_GDMA_DONEINT) & (1<<ch))) ;
	VPint(CR_GDMA_DONEINT) = (1<<ch);
}


#if 1 //TreyDbg
void dump_data(char *p, int len)
{
	char tmp[80];
	char *t = tmp;
	int i, n = 0;

	prom_printf("data=%08lx len=%d\n", (uint32) p, len);
	for (i = 0; i < len; i++) {
		t += sprintf(t, "%02x ", *p++ & 0xff);
		if ((i & 0x0f) == 0x0f) {
			prom_printf("%04x: %s\n", n, tmp);
			n += 16;
			t = tmp;
		}
	}
	if (i & 0x0f)
		prom_printf("%04x: %s\n", n, tmp);

    prom_printf("\n\n");
}
#endif

int dramTest2(dramTest_info_t *info)
{
	int ret = 0;
	unsigned char defWByte[] = {1, 2 , 4 , 0};
	unsigned char defRByte[] = {1, 2 , 4 , 0};
	int i,j;
	unsigned char defPat[4] = {0x5a, 0xa5, 0x00, 0xff};
	unsigned char pat;
    	unsigned int dramTestOffset_0 = DRAMTESTOFFSET0; 
    	unsigned int dramTestOffset_1 = DRAMTESTOFFSET1;


	if(info == 0)
	{
		prom_printf("dramTest: ERROR! no test info \n");
		return -1;
	}

	MP_DBG(DBG_L1, "Test Pattern: %x\n", INCR_PAT);
	dram_pat_set(dramTestOffset_0, info->size, 0, INCR_PAT, 1);
	MP_DBG(DBG_L1, "gdma write data from 0x%x to 0x%x with size 0x%x\n", dramTestOffset_0, info->startAddr, info->size);
	gdmacpy(info->startAddr, dramTestOffset_0, info->size); 
	MP_DBG(DBG_L1, "gdma read data from 0x%x to 0x%x with size 0x%x\n", info->startAddr, dramTestOffset_1, info->size);
	gdmacpy(dramTestOffset_1, info->startAddr, info->size);
	MP_DBG(DBG_L1, "check read/write value\n");
	ret += dram_incrPat_cmp(dramTestOffset_1, info->size, 0, 1, 1);
	MP_DBG(DBG_L1, "\n\n");

	MP_DBG(DBG_L1, "Test Pattern: %x\n", ANTI_INCR_PAT);
	dram_pat_set(dramTestOffset_0, info->size, 0, ANTI_INCR_PAT, 1);
	MP_DBG(DBG_L1, "gdma write data from 0x%x to 0x%x with size 0x%x\n", dramTestOffset_0, info->startAddr, info->size);
	gdmacpy(info->startAddr, dramTestOffset_0, info->size);
	MP_DBG(DBG_L1, "gdma read data from 0x%x to 0x%x with size 0x%x\n", info->startAddr, dramTestOffset_1, info->size);
	gdmacpy(dramTestOffset_1, info->startAddr, info->size);
	MP_DBG(DBG_L1, "check read/write value\n");
	ret += dram_antiIncrPat_cmp(dramTestOffset_1, info->size, 0, 1);	

	for(i=0; i<4; i++)
	{
		pat = defPat[i];
		MP_DBG(DBG_L1, "Test Pattern: %x\n", pat);
		dram_pat_set(dramTestOffset_0, info->size, pat, ORI_PAT, 1);
		MP_DBG(DBG_L1, "gdma write data from 0x%x to 0x%x with size 0x%x\n", dramTestOffset_0, info->startAddr, info->size);
		gdmacpy(info->startAddr, dramTestOffset_0, info->size);
		MP_DBG(DBG_L1, "gdma read data from 0x%x to 0x%x with size 0x%x\n", info->startAddr, dramTestOffset_1, info->size);
		gdmacpy(dramTestOffset_1, info->startAddr, info->size);
		MP_DBG(DBG_L1, "check read/write value\n");
		ret += dram_pat_cmp(dramTestOffset_1, info->size, pat, 1, 1);
		MP_DBG(DBG_L1, "\n\n");
	}

#if 0
	if(ret != 0)
	{
		prom_printf("dramTest FAIL!\n\n");
		return -1;
	}
	else
	{
		prom_printf("dramTest SUCCESS!\n\n");
		return 0;
	}
#endif
	return 0;
}

int gdmaTestDram(gdmaTest_info_t *info)
{
#define BASE_KSEG1_ADDR 	0xa0000000
	int ret = 0;
	unsigned long virDramSA = 0;
	unsigned long virDramDA = 0;
	int i;
	unsigned char defPat[4] = {0x5a, 0xa5, 0x00, 0xff};
	unsigned char pat;

	if(info == 0)
	{
		prom_printf("gdmaTestDram: ERROR! no test info \n");
		return -1;
	}

	virDramSA = info->sa + BASE_KSEG1_ADDR;
	virDramDA = info->da + BASE_KSEG1_ADDR;

	/* use the default patterns, "incremental patten" */
	dram_pat_set(virDramSA, info->len, 0, INCR_PAT, 1);
	gdmaTestCpy(info->sa, info->da, info->len, info->burst_size, info->wswap);
	ret += memcmp(virDramSA, virDramDA, info->len);

	/* use the default patterns, "anti-incremental patten, 0x5a, 0xa5, 0x0, 0xff" */
	dram_pat_set(virDramSA, info->len, 0, ANTI_INCR_PAT, 1);
	gdmaTestCpy(info->sa, info->da, info->len, info->burst_size, info->wswap);
	ret += memcmp(virDramSA, virDramDA, info->len);
	for(i=0; i<4; i++)
	{
		pat = defPat[i];
		dram_pat_set(virDramSA, info->len, pat, ORI_PAT, 1);
		gdmaTestCpy(info->sa, info->da, info->len, info->burst_size, info->wswap);
		ret += memcmp(virDramSA, virDramDA, info->len);
	}

	if(ret != 0)
	{
		prom_printf("gdmaTestDram FAIL!\n\n");
		return -1;
	}
	else
	{
		prom_printf("gdmaTestDram SUCCESS!\n\n");
		return 0;
	}
}

#ifdef TCSUPPORT_ADDR_MAPPING
extern void update_tlb_wired(unsigned long vir_address, unsigned long phy_address, unsigned long tlb_entry);
int highmemConfig(void){
	static initFlag;
	if(!initFlag){
		update_tlb_wired(0xd0000000,0x40000000,1);
		initFlag = 1;
		prom_printf("MMU setup ! \n");
	}
	return 0;
}
#endif
