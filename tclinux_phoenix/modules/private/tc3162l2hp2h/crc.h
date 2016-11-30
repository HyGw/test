

#ifndef _CRC_H_
#define _CRC_H_

/******************************************************************************
* Constants
******************************************************************************/

#define AAL5_CRC32_REMAINDER 0xCBF43926
#define AAL5_CRC32_INITIAL 0xffffffff

#define ATM_HEADER_REMAINDER 0x107
#define ATM_HEADER_COSET_LEADER 0x055

/******************************************************************************
* Prototype
******************************************************************************/

extern unsigned int  aal5_calc_crc(unsigned char *mem, int len, unsigned int initial);
extern unsigned char atm_calc_hec(unsigned char *header);

#endif
