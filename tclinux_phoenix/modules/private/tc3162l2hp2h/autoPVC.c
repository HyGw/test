/*
 * AutoPVC.c
 *
 *  Created on: Mar 4, 2009
 *      Author: krammer
 */

#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/atmdev.h>
#include <linux/mm.h>
#include <linux/proc_fs.h>
#include <asm/tc3162/tc3162.h>
#include <asm/tc3162/cmdparse.h>
#include "tsarm.h"
#include "autoPVC.h"
#include "aal5.h"

#define ALIGN32BYTESBASE	32
#define AAL5_CELL_BUFFER_SIZE 1024
#define PROBE_PKT_SIZE 1024


/* Pseudo-header for TCP and UDP checksumming */
typedef struct {
	int32 source;		/* IP source */
	int32 dest;			/* IP destination */
	uint8 protocol;		/* Protocol */
	uint16 length;		/* Data field length */
} pseudohdr_t;

/*extern function*/
extern int subcmd(const cmds_t tab[], int argc, char *argv[], void *p);
extern int autopvc_cmd_register_to_sys(cmds_t *cmds_p);
extern int autopvc_cmd_unregister_to_sys(void);
extern uint8 atmCcDataReq(uint8 *data_p);
extern void delay1ms(int ms);
extern uint32 align32Byte(uint32 addr);
extern void dumpCell(uint8* src, int len);
/*extern function*/

/*function declare*/
void vcPoolControlCC(uint8 flag);
#if	!defined(TCSUPPORT_CPU_MT7510)
void clearTxCcDescrPool(void);
#endif

/*global data*/
uint32 g_manageFlag=0;
uint32 g_vcPoolProbeService=0;
vc_pool_t g_pvc_pool[MAX_PVC_NUM];
vc_pool_t g_vc_pool[MAX_VC_POOL_NUM];
uint8 g_pcmac_addr[6], fin = 0;
int g_vcHuntSet=0;
static spinlock_t autopvcLock;
cmds_t	vcPoolCmd;
#if	!defined(TCSUPPORT_CPU_MT7510)
static int tx_raw_flag;
extern atmTxCcDescrPool_t *atmTxCcDescrPool;
#else
int	tx_raw_flag;
#endif

uint8 *AAL5_CELL;
unsigned int vpi_detected, vci_detected, encap_detected, service_detected;

static uint8 RFC1483_LLC_HDR[] = {
	0xaa, 0xaa, 0x03, 0x00, 0x80, 0xc2, 0x00, 0x07, 0x00, 0x00
};
static uint8 RFC1483_LLC_FCS_HDR[] = {
	0xaa, 0xaa, 0x03, 0x00, 0x80, 0xc2, 0x00, 0x01, 0x00, 0x00
};
static uint8 PPPOE_PADO[] = {
	0x88, 0x63,	0x11, 0x07,	0x00, 0x00
};
static uint8 RFC1483_VC_HDR[] = {
	0x00, 0x00
};
//static uint8 PPPOA_LLC_HDR[] = {
//	0xfe, 0xfe, 0x03, 0xcf, 0xc0, 0x21
//};
//static uint8 PPPOA_VC_HDR[] = {
//	0xc0, 0x21
//};

static uint8 PADI_PPPOE[] = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xaa, 0xbb, 0x01, 0x23, 0x45, 0x88, 0x63,	0x11, 0x09,
	0x00, 0x00, 0x00, 0x0c, 0x01, 0x01, 0x00, 0x00, 0x01, 0x03, 0x00, 0x04, 0x31, 0x30,	0x30, 0x30,
	0x08, 0x00, 0x45, 0x00, 0x02, 0x28, 0x13, 0xf5, 0x40, 0x00, 0x80, 0x06, 0x61, 0x68,	0xc0, 0xa8,
	0x01, 0x21, 0xc0, 0xa8, 0x01, 0x01, 0x05, 0xcc, 0x00, 0x14, 0x2a, 0x31, 0x0a, 0x30,	0xff, 0x78
};
/*global data*/
/*crc_L2autopvc*/

#include "crc.h"

/******************************************************************************
* Macros (This is used to speedup code in the calc_crc first while loop)
******************************************************************************/

#define DO1(c, crc) ((crc) = crc32tab[((unsigned int)((crc)>>24) ^ (*c++)) & 0xff] ^ ((crc) << 8))
#define DO2(c, crc)  DO1(c, crc); DO1(c, crc);
#define DO4(c, crc)  DO2(c, crc); DO2(c, crc);
#define DO8(c, crc)  DO4(c, crc); DO4(c, crc);


/******************************************************************************
* Precomputed AAL5 CRC32 lookup table
******************************************************************************/

static unsigned long crc32tab[256] = {

	0x00000000L, 0x04C11DB7L, 0x09823B6EL, 0x0D4326D9L,
	0x130476DCL, 0x17C56B6BL, 0x1A864DB2L, 0x1E475005L,
	0x2608EDB8L, 0x22C9F00FL, 0x2F8AD6D6L, 0x2B4BCB61L,
	0x350C9B64L, 0x31CD86D3L, 0x3C8EA00AL, 0x384FBDBDL,
	0x4C11DB70L, 0x48D0C6C7L, 0x4593E01EL, 0x4152FDA9L,
	0x5F15ADACL, 0x5BD4B01BL, 0x569796C2L, 0x52568B75L,
	0x6A1936C8L, 0x6ED82B7FL, 0x639B0DA6L, 0x675A1011L,
	0x791D4014L, 0x7DDC5DA3L, 0x709F7B7AL, 0x745E66CDL,
	0x9823B6E0L, 0x9CE2AB57L, 0x91A18D8EL, 0x95609039L,
	0x8B27C03CL, 0x8FE6DD8BL, 0x82A5FB52L, 0x8664E6E5L,
	0xBE2B5B58L, 0xBAEA46EFL, 0xB7A96036L, 0xB3687D81L,
	0xAD2F2D84L, 0xA9EE3033L, 0xA4AD16EAL, 0xA06C0B5DL,
	0xD4326D90L, 0xD0F37027L, 0xDDB056FEL, 0xD9714B49L,
	0xC7361B4CL, 0xC3F706FBL, 0xCEB42022L, 0xCA753D95L,
	0xF23A8028L, 0xF6FB9D9FL, 0xFBB8BB46L, 0xFF79A6F1L,
	0xE13EF6F4L, 0xE5FFEB43L, 0xE8BCCD9AL, 0xEC7DD02DL,
	0x34867077L, 0x30476DC0L, 0x3D044B19L, 0x39C556AEL,
	0x278206ABL, 0x23431B1CL, 0x2E003DC5L, 0x2AC12072L,
	0x128E9DCFL, 0x164F8078L, 0x1B0CA6A1L, 0x1FCDBB16L,
	0x018AEB13L, 0x054BF6A4L, 0x0808D07DL, 0x0CC9CDCAL,
	0x7897AB07L, 0x7C56B6B0L, 0x71159069L, 0x75D48DDEL,
	0x6B93DDDBL, 0x6F52C06CL, 0x6211E6B5L, 0x66D0FB02L,
	0x5E9F46BFL, 0x5A5E5B08L, 0x571D7DD1L, 0x53DC6066L,
	0x4D9B3063L, 0x495A2DD4L, 0x44190B0DL, 0x40D816BAL,
	0xACA5C697L, 0xA864DB20L, 0xA527FDF9L, 0xA1E6E04EL,
	0xBFA1B04BL, 0xBB60ADFCL, 0xB6238B25L, 0xB2E29692L,
	0x8AAD2B2FL, 0x8E6C3698L, 0x832F1041L, 0x87EE0DF6L,
	0x99A95DF3L, 0x9D684044L, 0x902B669DL, 0x94EA7B2AL,
	0xE0B41DE7L, 0xE4750050L, 0xE9362689L, 0xEDF73B3EL,
	0xF3B06B3BL, 0xF771768CL, 0xFA325055L, 0xFEF34DE2L,
	0xC6BCF05FL, 0xC27DEDE8L, 0xCF3ECB31L, 0xCBFFD686L,
	0xD5B88683L, 0xD1799B34L, 0xDC3ABDEDL, 0xD8FBA05AL,
	0x690CE0EEL, 0x6DCDFD59L, 0x608EDB80L, 0x644FC637L,
	0x7A089632L, 0x7EC98B85L, 0x738AAD5CL, 0x774BB0EBL,
	0x4F040D56L, 0x4BC510E1L, 0x46863638L, 0x42472B8FL,
	0x5C007B8AL, 0x58C1663DL, 0x558240E4L, 0x51435D53L,
	0x251D3B9EL, 0x21DC2629L, 0x2C9F00F0L, 0x285E1D47L,
	0x36194D42L, 0x32D850F5L, 0x3F9B762CL, 0x3B5A6B9BL,
	0x0315D626L, 0x07D4CB91L, 0x0A97ED48L, 0x0E56F0FFL,
	0x1011A0FAL, 0x14D0BD4DL, 0x19939B94L, 0x1D528623L,
	0xF12F560EL, 0xF5EE4BB9L, 0xF8AD6D60L, 0xFC6C70D7L,
	0xE22B20D2L, 0xE6EA3D65L, 0xEBA91BBCL, 0xEF68060BL,
	0xD727BBB6L, 0xD3E6A601L, 0xDEA580D8L, 0xDA649D6FL,
	0xC423CD6AL, 0xC0E2D0DDL, 0xCDA1F604L, 0xC960EBB3L,
	0xBD3E8D7EL, 0xB9FF90C9L, 0xB4BCB610L, 0xB07DABA7L,
	0xAE3AFBA2L, 0xAAFBE615L, 0xA7B8C0CCL, 0xA379DD7BL,
	0x9B3660C6L, 0x9FF77D71L, 0x92B45BA8L, 0x9675461FL,
	0x8832161AL, 0x8CF30BADL, 0x81B02D74L, 0x857130C3L,
	0x5D8A9099L, 0x594B8D2EL, 0x5408ABF7L, 0x50C9B640L,
	0x4E8EE645L, 0x4A4FFBF2L, 0x470CDD2BL, 0x43CDC09CL,
	0x7B827D21L, 0x7F436096L, 0x7200464FL, 0x76C15BF8L,
	0x68860BFDL, 0x6C47164AL, 0x61043093L, 0x65C52D24L,
	0x119B4BE9L, 0x155A565EL, 0x18197087L, 0x1CD86D30L,
	0x029F3D35L, 0x065E2082L, 0x0B1D065BL, 0x0FDC1BECL,
	0x3793A651L, 0x3352BBE6L, 0x3E119D3FL, 0x3AD08088L,
	0x2497D08DL, 0x2056CD3AL, 0x2D15EBE3L, 0x29D4F654L,
	0xC5A92679L, 0xC1683BCEL, 0xCC2B1D17L, 0xC8EA00A0L,
	0xD6AD50A5L, 0xD26C4D12L, 0xDF2F6BCBL, 0xDBEE767CL,
	0xE3A1CBC1L, 0xE760D676L, 0xEA23F0AFL, 0xEEE2ED18L,
	0xF0A5BD1DL, 0xF464A0AAL, 0xF9278673L, 0xFDE69BC4L,
	0x89B8FD09L, 0x8D79E0BEL, 0x803AC667L, 0x84FBDBD0L,
	0x9ABC8BD5L, 0x9E7D9662L, 0x933EB0BBL, 0x97FFAD0CL,
	0xAFB010B1L, 0xAB710D06L, 0xA6322BDFL, 0xA2F33668L,
	0xBCB4666DL, 0xB8757BDAL, 0xB5365D03L, 0xB1F740B4L

};

/******************************************************************************
* Precomputed ATM CRC8 lookup table
******************************************************************************/

unsigned char hec_table[256]=
{

	0x00, 0x07, 0x0e, 0x09, 0x1c, 0x1b, 0x12, 0x15,
	0x38, 0x3f, 0x36, 0x31, 0x24, 0x23, 0x2a, 0x2d,
	0x70, 0x77, 0x7e, 0x79, 0x6c, 0x6b, 0x62, 0x65,
	0x48, 0x4f, 0x46, 0x41, 0x54, 0x53, 0x5a, 0x5d,
	0xe0, 0xe7, 0xee, 0xe9, 0xfc, 0xfb, 0xf2, 0xf5,
	0xd8, 0xdf, 0xd6, 0xd1, 0xc4, 0xc3, 0xca, 0xcd,
	0x90, 0x97, 0x9e, 0x99, 0x8c, 0x8b, 0x82, 0x85,
	0xa8, 0xaf, 0xa6, 0xa1, 0xb4, 0xb3, 0xba, 0xbd,
	0xc7, 0xc0, 0xc9, 0xce, 0xdb, 0xdc, 0xd5, 0xd2,
	0xff, 0xf8, 0xf1, 0xf6, 0xe3, 0xe4, 0xed, 0xea,
	0xb7, 0xb0, 0xb9, 0xbe, 0xab, 0xac, 0xa5, 0xa2,
	0x8f, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9d, 0x9a,
	0x27, 0x20, 0x29, 0x2e, 0x3b, 0x3c, 0x35, 0x32,
	0x1f, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0d, 0x0a,
	0x57, 0x50, 0x59, 0x5e, 0x4b, 0x4c, 0x45, 0x42,
	0x6f, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7d, 0x7a,
	0x89, 0x8e, 0x87, 0x80, 0x95, 0x92, 0x9b, 0x9c,
	0xb1, 0xb6, 0xbf, 0xb8, 0xad, 0xaa, 0xa3, 0xa4,
	0xf9, 0xfe, 0xf7, 0xf0, 0xe5, 0xe2, 0xeb, 0xec,
	0xc1, 0xc6, 0xcf, 0xc8, 0xdd, 0xda, 0xd3, 0xd4,
	0x69, 0x6e, 0x67, 0x60, 0x75, 0x72, 0x7b, 0x7c,
	0x51, 0x56, 0x5f, 0x58, 0x4d, 0x4a, 0x43, 0x44,
	0x19, 0x1e, 0x17, 0x10, 0x05, 0x02, 0x0b, 0x0c,
	0x21, 0x26, 0x2f, 0x28, 0x3d, 0x3a, 0x33, 0x34,
	0x4e, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5c, 0x5b,
	0x76, 0x71, 0x78, 0x7f, 0x6a, 0x6d, 0x64, 0x63,
	0x3e, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2c, 0x2b,
	0x06, 0x01, 0x08, 0x0f, 0x1a, 0x1d, 0x14, 0x13,
	0xae, 0xa9, 0xa0, 0xa7, 0xb2, 0xb5, 0xbc, 0xbb,
	0x96, 0x91, 0x98, 0x9f, 0x8a, 0x8d, 0x84, 0x83,
	0xde, 0xd9, 0xd0, 0xd7, 0xc2, 0xc5, 0xcc, 0xcb,
	0xe6, 0xe1, 0xe8, 0xef, 0xfa, 0xfd, 0xf4, 0xf3

};

static uint8 DHCP_DISCOVER[] = {	
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x1c, 0xda, 0x29, 0x4b, 0x08, 0x00, 0x45, 0x00,
	0x01, 0x48, 0x00, 0x0a, 0x00, 0x00, 0x80, 0x11, 0x39, 0x9c, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
	0xff, 0xff, 0x00, 0x44, 0x00, 0x43, 0x01, 0x34, 0xe4, 0x81, 0x01, 0x01, 0x06, 0x00, 0x5b, 0xec,
	0xd7, 0xca, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0xda, 0x29, 0x4b, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x82, 0x53, 0x63, 0x35, 0x01, 0x01, 0x74, 0x01, 0x01,
	0x3d, 0x07, 0x01, 0x00, 0x00, 0x1c, 0xda, 0x29, 0x4b, 0x0c, 0x0f, 0x74, 0x63, 0x2d, 0x65, 0x34,
	0x66, 0x36, 0x65, 0x64, 0x32, 0x66, 0x35, 0x62, 0x38, 0x37, 0x3c, 0x08, 0x4d, 0x53, 0x46, 0x54,
	0x20, 0x35, 0x2e, 0x30, 0x37, 0x0b, 0x01, 0x0f, 0x03, 0x06, 0x2c, 0x2e, 0x2f, 0x1f, 0x21, 0xf9,
	0x2b, 0xff, 0x00, 0x00, 0x00, 0x00
};

static uint16
get16(
	uint8	*cp
)
{
	register uint16 x;

	x = *cp++;
	x <<= 8;
	x |= *cp;
	return x;
} /* get16 */

static uint32
get32 (
	uint8	*cp
)
{
	uint32 rval;

	rval = *cp++;
	rval <<= 8;
	rval |= *cp++;
	rval <<= 8;
	rval |= *cp++;
	rval <<= 8;
	rval |= *cp;

	return rval;
} /* get32 */

static uint8 *
put32(
	uint8	*cp,
	uint32	x
)
{
	*cp++ = x >> 24;
	*cp++ = x >> 16;
	*cp++ = x >> 8;
	*cp++ = x;
	return cp;
} /* put32 */

static uint8 *
put16(
	uint8	*cp,
	uint16	x
)
{
	*cp++ = x >> 8;
	*cp++ = x;

	return cp;
} /* put16 */

/*
* Function     : aal5_calc_crc
* Return Value : crc calculated for the given buffer ( needs to be xor'ed)
* Description  : none
*/

unsigned int aal5_calc_crc(unsigned char *mem, int len, unsigned int initial)
{

	register unsigned int crc;

	crc = initial;

	while( len >= 8) {
		DO8(mem, crc);
		len -= 8;
	}

	while( len ) {
		DO1(mem, crc);
		len--;
	}

	return(crc);

}

unsigned char atm_calc_hec(unsigned char *header)
{

	register unsigned char hec,i;

	hec = 0;

	for(i=0; i<4; i++)
		hec = hec_table[hec^header[i]];

	return(hec^ATM_HEADER_COSET_LEADER);

}

/*crc_L2autopvc*/

/*aal5*/
/*
* Function     : atm_header_create
* Return value : none
* Description  : Creates an atm cell header to (vpi, vci, pti, clp)
*/

void atm_header_create(unsigned char *header, int vpi, int vci, int pti, int clp)
{

	/*
	* ATM UNI cell header
	*
	*  8     7     6     5     4     3     2     1
	* *************************************************
	* *          GFC        *          VPI            *
	* *************************************************
	* *         (VPI)       *                         *
	* ***********************                         *
	* *                      VCI                      *
	* *                     ***************************
	* *                     *         PTI     *  CLP  *
	* *************************************************
	* *                Header CRC                     *
	* *************************************************
	*
	*/

	vpi &= 0x000000ff;
	vci &= 0x0000ffff;
	pti &= 0x00000007;
	clp &= 0x00000001;

	header[0] =  (vpi >> 4);
	header[1] =  (vpi << 4) | (vci >> 12);
	header[2] =  (vci & 0x00000ff0) >> 4;
	header[3] = ((vci & 0x0000000f) << 4) | (pti << 1) | clp;
#if 0
#ifdef DEBUG
	header[4] =  atm_calc_hec(header);
#else
	header[4] = 0xec; /* Arbitrary constant */
#endif
#endif

}

void atm_cell_create_with_header(unsigned char *cell, unsigned char *data, unsigned char *header)
{

	memcpy(cell + ATM_CELL_HEADER_SIZE, data, ATM_CELL_DATA_SIZE);
	memcpy(cell, header, ATM_CELL_HEADER_SIZE);

}

/*
* Function     : aal5_frame_to_atm_cells
* Return value : -1 if the aal5 frame is not recognized (its length is not %48)
*                or atm cell buffer size
* Description  : given an aal5 frame, build an atm cell queue
*/

int aal5_frame_to_atm_cells(unsigned char *atm_cells, unsigned char *aal5_frame, int length, int vpi, int vci)
{

	unsigned char *src, *dst;
	unsigned char header[5];
	unsigned int cells;

	if(length%ATM_CELL_DATA_SIZE)
			return(-1);

	cells = length/ATM_CELL_DATA_SIZE - 1;


	/*
	* We will write atm cells from last to first
	* This allow us to use the same buffer for src and dst
	* This cell must have pti = 1 to mark end af the aal5 frame
	*/

	src = aal5_frame + ATM_CELL_DATA_SIZE *cells;
	dst = atm_cells  + ATM_CELL_TOTAL_SIZE*cells;
	atm_header_create(header, vpi, vci, 1, 0);

	atm_cell_create_with_header(dst, src, header);

	/* (Re)Create the header */
	atm_header_create(header, vpi, vci, 0, 0);

	/* Build all the other cells */
	while(cells--) {

		src -= ATM_CELL_DATA_SIZE;
		dst -= ATM_CELL_TOTAL_SIZE;
		atm_cell_create_with_header(dst, src, header);

	}

	return((length/ATM_CELL_DATA_SIZE) * ATM_CELL_TOTAL_SIZE);

}

/*aal5*/


//int
//vcPoolProbeVcOamRx(
//	uint8 vpi_in,
//	uint16 vci_in
//)
//{
//	uint8 vpi;
//	uint16 vci;
//	int i;
//
////	if (!(vcPoolProbeService & PROBE_OAM))
////		return 0;
//
//	/* check if it is remote node's vpi/vci */
//	vpi = spSysParam.remoteNode[0].atmPf.vpiBrdg;
//	vci = spSysParam.remoteNode[0].atmPf.vciBrdg;
//	if ((vpi_in == vpi) && (vci_in == vci)) {
//		VCPOOL_DBG_B("\r\n probe VC RX remote node vpi=", vpi);
//		VCPOOL_DBG_W(" vci=", vci);
//		vcPoolManageFlag |= VC_HUNT_FLAG_MANG_STOP;
//		return 0;
//	}
//
//	for (i = 0; i < spSysParam.nVCHuntSet; i++) {
//		vpi = vcPoolSet[i].vpi;
//		vci = vcPoolSet[i].vci;
//		if (vci == 0)
//			continue;
//		if ((vpi_in == vpi) && (vci_in == vci)) {
//			vcPoolSet[i].flags |= VC_HUNT_FLAG_HUNT_SUCC;
//			return 1;
//		}
//	}
//	return 0;
//}


/*______________________________________________________________________________
**  RawCcCellRecv
**
**  descriptions:check header
**  parameters:
**  local:
**  global:
**  return:
**  	1	yes
**  	0	no
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
void RawCcCellRecv(uint8 * cellp)
{
	atmOamCell_t *oamCellp;
//	uint32 aal5Len;
//	int i;
//	uint8 * data_p;
	uint8 *cp;
#if 1
	int ihl;
	uint8 protocol, ETHER_TYPE_IP[2] = {0x08, 0x00};
	uint16 src_port, dst_port;
#endif	
	uint8 chk;
	uint16 flags;
	uint8 /*vpi,*/ vpi_in;
	uint16 /*vci,*/ vci_in;
	int encap=0,i=0;
	int vcHuntSetNumber=0;
	vc_pool_t *pTmpVcPool=NULL;

//	if ( !(g_manageFlag & VC_HUNT_FLAG_MANG_START)) {
//		return;
//	}

	if(g_vcHuntSet == HUNT_PVC){
		vcHuntSetNumber=MAX_PVC_NUM;
		pTmpVcPool=g_pvc_pool;
	}
	else if(g_vcHuntSet == HUNT_VCPOOL){
		vcHuntSetNumber=MAX_VC_POOL_NUM;
		pTmpVcPool=g_vc_pool;
	}

	oamCellp = (atmOamCell_t *) cellp;
	if ((oamCellp->vci == 3) || (oamCellp->vci == 4)){
		printk("discard oam cell...\n");
		return;
	}

	vpi_in = oamCellp->vpi;
	vci_in = oamCellp->vci;

//	if(g_vcHuntSet == HUNT_PVC){
//		for(i=0;i<MAX_PVC_NUM;i++){
//			if((vpi_in == g_pvc_pool[i].vpi)&&(vci_in == g_pvc_pool[i].vci)){
//				break;
//			}
//		}
//		encap=g_pvc_pool[i].encap;
//	}
//	else{
//		encap=g_encap;
//	}
	for(i=0;i<vcHuntSetNumber;i++){
		if((vpi_in == pTmpVcPool[i].vpi)&&(vci_in == pTmpVcPool[i].vci)){
			break;
		}
	}
	encap=pTmpVcPool[i].encap;

	flags = 0;
#if 1	
	if (g_vcPoolProbeService & PROBE_DHCP) {
		cp = &cellp[4];
		chk = 0;
		/* check if RFC1483 LLC header */
		if(!encap){
			if (memcmp(cp, RFC1483_LLC_HDR, sizeof(RFC1483_LLC_HDR)) == 0 ||
				memcmp(cp, RFC1483_LLC_FCS_HDR, sizeof(RFC1483_LLC_FCS_HDR)) == 0) {
				cp += sizeof(RFC1483_LLC_HDR);
				chk = 1;
			}
		}
		else{
			if (memcmp(cp, RFC1483_VC_HDR, sizeof(RFC1483_VC_HDR)) == 0){
				cp += sizeof(RFC1483_VC_HDR);
				chk = 1;
			}
		}
		if (chk) {
			/* skip ethernet mac address */
			cp += 12;
			/* check if IP packet */
			if (memcmp(cp, ETHER_TYPE_IP, sizeof(ETHER_TYPE_IP)) == 0) {
				printk("ETHER_TYPE_IP\r\n");
				cp += sizeof(ETHER_TYPE_IP);
				protocol = *(cp + 9);
				/* check if UDP protocol */
				if (protocol == 0x11) {
					printk("protocol == 0x11\r\n");
					/* skip ip header */
					ihl = (*cp & 0xf) << 2;
					cp += ihl;
					src_port = get16(cp);
					dst_port = get16(cp + 2);
					printk("src_port = 0x%X, dst_port = 0x%X\r\n", src_port, dst_port);
					/* check if DHCP packet */
					if ((src_port == 0x43) && (dst_port == 0x44)){
//						VCPOOL_DBG("\r\n *****CC dhcp back *****");
						flags |= PROBE_DHCP;
						goto find;
					}
				}
			}
		}

	}
#endif
	if (g_vcPoolProbeService & PROBE_PPPOE) {
		cp = &cellp[4];
		chk = 0;
		/* check if RFC1483 LLC header */
		if(!encap){
			if (memcmp(cp, RFC1483_LLC_HDR, sizeof(RFC1483_LLC_HDR)) == 0 ||
				memcmp(cp, RFC1483_LLC_FCS_HDR, sizeof(RFC1483_LLC_FCS_HDR)) == 0) {
				cp += sizeof(RFC1483_LLC_HDR);
				chk = 1;
			}
		}
		else{
			if (memcmp(cp, RFC1483_VC_HDR, sizeof(RFC1483_VC_HDR)) == 0) {
				cp += sizeof(RFC1483_VC_HDR);
				chk = 1;
			}
		}
		if (chk) {
			/* skip ethernet mac address */
			cp += 12;
			/* check PPPoE PADO packet */
			if (memcmp(cp, PPPOE_PADO, sizeof(PPPOE_PADO)) == 0) {
				flags |= PROBE_PPPOE;
				goto find;
			}
		}
	}

//	if( vcPoolProbeService & PROBE_ARP){
//		cp = &cellp[4];
//		chk = 0;
//		/* check if RFC1483 LLC header */
//		if (memcmp(cp, RFC1483_LLC_HDR, sizeof(RFC1483_LLC_HDR)) == 0 ||
//			memcmp(cp, RFC1483_LLC_FCS_HDR, sizeof(RFC1483_LLC_FCS_HDR)) == 0) {
//			cp += sizeof(RFC1483_LLC_HDR);
//			chk = 1;
//		}
//		/* check dst ethernet mac address */
//		if( (chk == 1) &&  (memcmp(cp, pcmac_addr, sizeof(pcmac_addr)) == 0) ){
//			//printf("is my mac\n");
//			chk =2;
//		}
//		if( chk == 2){
//			cp += 12;
//			/* check if ARP packet */
//			if (memcmp(cp, ETHER_TYPE_ARP, sizeof(ETHER_TYPE_ARP)) == 0) {
//				VCPOOL_DBG("\r\n ******* CC arp back! *******");
//				flags |= PROBE_ARP;
//				goto find;
//			}
//		}
//	}
find:
	if(flags){
		vcPoolProbeVcPktRx(vpi_in,vci_in);
	}

}

/*______________________________________________________________________________
**  vcPoolProbeVcPktRx
**
**  descriptions:check if the vci and vci is in our vcpool!
**  parameters:
**  local:
**  global:
**  return:
**  	1	yes
**  	0	no
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int
vcPoolProbeVcPktRx(
	uint8 vpi_in,
	uint16 vci_in
//	struct sk_buff *bp
)
{
	uint8 vpi=0;
	uint16 vci=0;
	int i;
	int vcHuntSetNumber=0;
	vc_pool_t *pTmpVcPool=NULL;
	vpi_detected = vpi_in;
	vci_detected = vci_in;
	service_detected = g_vcPoolProbeService;
	encap_detected = g_vc_pool[0].encap;

	//g_manageFlag &= ~VC_HUNT_FLAG_MANG_START;
	//vcPoolControlCC(0);
	//clearTxCcDescrPool();
	//fin = 1;

	if(g_vcHuntSet == HUNT_PVC){
		vcHuntSetNumber=MAX_PVC_NUM;
		pTmpVcPool=g_pvc_pool;
	}
	else if(g_vcHuntSet == HUNT_VCPOOL){
		vcHuntSetNumber=MAX_VC_POOL_NUM;
		pTmpVcPool=g_vc_pool;
	}


	for (i = 0; i < vcHuntSetNumber; i++) {
		vpi=pTmpVcPool[i].vpi;
		vci=pTmpVcPool[i].vci;
		if (vci == 0){
			continue;
		}
		if ((vpi_in == vpi) && (vci_in == vci) ) {
			pTmpVcPool[i].management |= VC_HUNT_FLAG_HUNT_SUCC;
			return 1;
		}

	}
	return 0;
}

#if 0
/*______________________________________________________________________________
**  sarAAL5RxComplete
**
**  descriptions:check the vci and vci , if is in our vcpool, free it!
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
void
sarAAL5RxComplete(
	struct sk_buff *bp,
	uint8	vpi,
	uint16	vci
)
{
	if (vcPoolProbeVcPktRx(vpi, vci)) {
		dev_kfree_skb_any(bp);
		return;
	}
}
#endif

/*______________________________________________________________________________
**  setTxRaw
**
**  descriptions:set tx_raw enable
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
void
setTxRaw(atmTxCcDescr_t * atmTxCcDescrp){
	if(tx_raw_flag){
		atmTxCcDescrp->tdes0 |= TSARM_TX_RAW_EN;
	}
}

/*______________________________________________________________________________
**  vcPoolControlCC
**
**  descriptions:set cc channel control register
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
void vcPoolControlCC(uint8 flag)
{
	if(flag){
		TSARM_GFR |= GFR_ACTIVE_MIS;
		TSARM_GFR &= ~(GFR_RX_INACT_VC_M);
	}
	else{
		TSARM_GFR &= ~(GFR_ACTIVE_MIS);
		TSARM_GFR |= GFR_RX_INACT_VC_M;
	}
}

/*______________________________________________________________________________
**  vcPoolSendPkt
**
**  descriptions:transform packet to atmcell and send to cc channel
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int
vcPoolSendPkt(
	uint8 *pProbePkt,
	int size,
	uint8 vpi,
	uint16 vci
)
{
//	unsigned char *pCell;
	unsigned int crc;
	unsigned int total_length;
	int cell_totallen;
	int cellnum;
	int /*i,*/ j;
//	int i_state;
	atmOamCell_t *oamCellp;
	uint32 flags;

	total_length = ATM_CELL_DATA_SIZE * ((size + 8 + ATM_CELL_DATA_SIZE - 1) / ATM_CELL_DATA_SIZE);
	memset(pProbePkt + size, 0, total_length - size);

	pProbePkt[total_length - 6] = (size & 0x0000ff00)>>8;
	pProbePkt[total_length - 5] = (size & 0x000000ff);

	crc = ~ aal5_calc_crc(pProbePkt, total_length - 4, ~0);

	pProbePkt[total_length - 4] = (crc & 0xff000000)>>24;
	pProbePkt[total_length - 3] = (crc & 0x00ff0000)>>16;
	pProbePkt[total_length - 2] = (crc & 0x0000ff00)>> 8;
	pProbePkt[total_length - 1] = (crc & 0x000000ff);

	/* aal5 frame to atm cell */
	cell_totallen = aal5_frame_to_atm_cells(AAL5_CELL, pProbePkt, total_length, vpi, vci);
	cellnum = cell_totallen / ATM_CELL_TOTAL_SIZE;

	/* send raw data out */
//	oamCellp = (atmOamCell_t *)(CACHE_TO_NONCACHE(AAL5_CELL));
	oamCellp = (atmOamCell_t *)(AAL5_CELL);

//	for (i = 0; i < 1; i++) { // send pkt for 2 times
	for( j=0; j<cellnum; j++){
//			i_state = dirps();
		spin_lock_irqsave(&autopvcLock, flags);
		tx_raw_flag=1;
		atmCcDataReq((uint8 *)oamCellp/*, 1*/);
		tx_raw_flag=0;
		spin_unlock_irqrestore(&autopvcLock, flags);
//			restore(i_state);
		oamCellp++;
	}
	delay1ms(10);
//	}

	return 0;
}

/*______________________________________________________________________________
**  vcPoolProbeVcWithPADI
**
**  descriptions:send probe packet with padi
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
void
vcPoolProbeVcWithPADI(uint8 vpi, uint16 vci, int encap)
{
	uint8 *pProbePkt;
	uint8 *my_mac = NULL;
	int i;

	pProbePkt=kmalloc(PROBE_PKT_SIZE, GFP_KERNEL);
	if(pProbePkt == NULL){
		printk("Error:in %s ,can't allocate a buffer for probe packet, so stop this probe\n",__func__);
		return;
	}
	my_mac = g_pcmac_addr;

	if (g_vcPoolProbeService & PROBE_PPPOE) {
		/* send pppoe llc probe packet */
		memset(pProbePkt, 0, PROBE_PKT_SIZE);
		if(!encap){//llc
			memcpy(pProbePkt, RFC1483_LLC_HDR, sizeof(RFC1483_LLC_HDR));
			memcpy(pProbePkt + sizeof(RFC1483_LLC_HDR), PADI_PPPOE, sizeof(PADI_PPPOE));
			memcpy(pProbePkt + sizeof(RFC1483_LLC_HDR) + 6, my_mac, 6);
			vcPoolSendPkt(pProbePkt, sizeof(RFC1483_LLC_HDR) + sizeof(PADI_PPPOE), vpi, vci);
			vcPoolSendPkt(pProbePkt, sizeof(RFC1483_LLC_HDR) + sizeof(PADI_PPPOE), vpi, vci);
		}
		else{
			memcpy(pProbePkt, RFC1483_VC_HDR, sizeof(RFC1483_VC_HDR));
			memcpy(pProbePkt + sizeof(RFC1483_VC_HDR), PADI_PPPOE, sizeof(PADI_PPPOE));
			memcpy(pProbePkt + sizeof(RFC1483_VC_HDR) + 6, my_mac, 6);
			vcPoolSendPkt(pProbePkt, sizeof(RFC1483_VC_HDR) + sizeof(PADI_PPPOE), vpi, vci);
			vcPoolSendPkt(pProbePkt, sizeof(RFC1483_VC_HDR) + sizeof(PADI_PPPOE), vpi, vci);
		}
	}
	kfree(pProbePkt);

	return;

}

/* Extract a short from a long */
#define hiword(x)       ((uint16)((x) >> 16))
#define loword(x)       ((uint16)(x))

static uint16 in_cksum(pseudohdr_t *pseudohdr, uint16 *ptr, int nbytes)
{
	register int32		sum;		/* assumes long == 32 bits */
	uint16 				oddbyte;
	register uint16		answer; 	/* assumes u_short == 16 bits */

	/*
	 * Our algorithm is simple, using a 32-bit accumulator (sum),
	 * we add sequential 16-bit words to it, and at the end, fold back
	 * all the carry bits from the top 16 bits into the lower 16 bits.
	 */

	sum = 0;

	/* Sum pseudo-header, if present */
	if(pseudohdr != NULL){
		sum = hiword(pseudohdr->source);
		sum += loword(pseudohdr->source);
		sum += hiword(pseudohdr->dest);
		sum += loword(pseudohdr->dest);
		sum += pseudohdr->protocol;
		sum += pseudohdr->length;
	}

	while (nbytes > 1)	{
		sum += *ptr++;
		nbytes -= 2;
	}

				/* mop up an odd byte, if necessary */
	if (nbytes == 1) {
		oddbyte = 0;		/* make sure top half is zero */
		*((u_char *) &oddbyte) = *(u_char *)ptr;   /* one byte only */
		sum += oddbyte;
	}

	/*
	 * Add back carry outs from top 16 bits to low 16 bits.
	 */

	sum  = (sum >> 16) + (sum & 0xffff);	/* add high-16 to low-16 */
	sum += (sum >> 16); 		/* add carry */
	answer = ~sum;		/* ones-complement, then truncate to 16 bits */
	return(answer);
}

int
vcPoolSendOnePkt(
	uint8 *pProbePkt, 
	int size, 
	uint8 vpi, 
	uint16 vci
)
{

	unsigned char *pCell;
	unsigned int crc;
	unsigned int total_length;
	int cell_totallen;
	int cellnum;
	int i, j, n;
	int i_state;
	atmOamCell_t *oamCellp;
	uint32 flags;

	total_length = ATM_CELL_DATA_SIZE * ((size + 8 + ATM_CELL_DATA_SIZE - 1) / ATM_CELL_DATA_SIZE);
	//printf("total_length of aal5 frame with tail  is %d \n", total_length);

	memset(pProbePkt + size, 0, total_length - size);

	pProbePkt[total_length - 6] = (size & 0x0000ff00)>>8;
	pProbePkt[total_length - 5] = (size & 0x000000ff);

	crc = ~ aal5_calc_crc(pProbePkt, total_length - 4, ~0);

	pProbePkt[total_length - 4] = (crc & 0xff000000)>>24;
	pProbePkt[total_length - 3] = (crc & 0x00ff0000)>>16;
	pProbePkt[total_length - 2] = (crc & 0x0000ff00)>> 8;
	pProbePkt[total_length - 1] = (crc & 0x000000ff);

	/* aal5 frame to atm cell */
	cell_totallen = aal5_frame_to_atm_cells(AAL5_CELL, pProbePkt, total_length, vpi, vci);
	cellnum = cell_totallen / ATM_CELL_TOTAL_SIZE;
	//printf("total cell length is %d, cell number is %d \n", cell_totallen, cellnum);

	/* send raw data out */
	oamCellp = (atmOamCell_t *)(CACHE_TO_NONCACHE(AAL5_CELL));

	for (i = 0; i < 1; i++) { // send pkt for 2 times
		for( j=0; j<cellnum; j++){
	#if 0 //test by zhj
		if(vpi == 0 && vci == 35){
			dbg_pline_1("\n ---- dhcp ---\n");
			for(n=0; n<52; n++)
				dbg_plineb_1(" ", *(((uint8 *)oamCellp)+n));
			dbg_pline_1("\n -------\n");

		}

	#endif
//			i_state = dirps();
			spin_lock_irqsave(&autopvcLock, flags);
			tx_raw_flag=1;
			atmCcDataReq((uint8 *)oamCellp/*, 1*/);
			tx_raw_flag=0;
			spin_unlock_irqrestore(&autopvcLock, flags);
//			restore(i_state);
			oamCellp++;		
			
		}		
//	 	pause(10);
		delay1ms(10);
	}

	return 0;
}

void
vcPoolProbeVcWithDHCP(uint8 vpi, uint16 vci, int encap)
{
	static uint16 trans_id = 0;
	uint8 probePkt[670];   //should be larger than 656 jianli 060727
	uint8 *pProbePkt;
	uint8 *my_mac = NULL;
	pseudohdr_t pseudohdr;
	int i;

	pProbePkt = probePkt;
	my_mac = g_pcmac_addr;

	if (g_vcPoolProbeService & PROBE_DHCP) {
		/* send dhcp req llc probe packet */ 
		memset(pProbePkt, 0, sizeof(probePkt));
		if(!encap){
			memcpy(pProbePkt, RFC1483_LLC_HDR, sizeof(RFC1483_LLC_HDR));
//			if (vcPoolProbeDHCPDiscover) {
			if(1){
				memcpy(pProbePkt + sizeof(RFC1483_LLC_HDR), DHCP_DISCOVER, sizeof(DHCP_DISCOVER));
				/* SA */
				memcpy(pProbePkt + 10 + 6, my_mac, 6);	
				/* client hardware address */
				memcpy(pProbePkt + 10 + 70, my_mac, 6);  
				/* client identifier */
				memcpy(pProbePkt + 10 + 291, my_mac, 6);  
			
				memset(pProbePkt + 10 + 24, 0, 2);
				put16(pProbePkt + 10 + 18, get16(pProbePkt + 10 + 18) + trans_id++);
				put16(pProbePkt + 10 + 24, in_cksum(NULL, (uint16 *)(pProbePkt + 10 + 14), 20));			
				put32(pProbePkt + 10 + 46, get32(pProbePkt + 10 + 46) + trans_id++);			
			
				/* Clear checksum for later recalculation */
				pseudohdr.source= 0x0000;
				pseudohdr.dest = 0xffff;
				pseudohdr.protocol = 17;
				pseudohdr.length = 308;
				memset(pProbePkt + 10 + 40, 0, 2);
				/* If requested, recompute checksum and insert into header */
				put16(pProbePkt + 10 + 40,in_cksum(&pseudohdr, (uint16 *)(pProbePkt + 10 + 34), 308));
				vcPoolSendOnePkt(pProbePkt, sizeof(RFC1483_LLC_HDR) + sizeof(DHCP_DISCOVER), vpi, vci);
				vcPoolSendOnePkt(pProbePkt, sizeof(RFC1483_LLC_HDR) + sizeof(DHCP_DISCOVER), vpi, vci);
			} 
#if 0			
			else {
				if (!vcPoolProbeNewDHCP) {
					memcpy(pProbePkt + sizeof(RFC1483_LLC_HDR), DHCP_REQ, sizeof(DHCP_REQ));
					vcPoolSendOnePkt(pProbePkt, sizeof(RFC1483_LLC_HDR) + sizeof(DHCP_REQ), vpi, vci);
					vcPoolSendOnePkt(pProbePkt, sizeof(RFC1483_LLC_HDR) + sizeof(DHCP_REQ), vpi, vci);
				} else {
					memcpy(pProbePkt + sizeof(RFC1483_LLC_HDR), DHCP_REQ_NEW, sizeof(DHCP_REQ_NEW));
					/* SA */
					memcpy(pProbePkt + 10 + 6, my_mac, 6);  
					/* client hardware address */
					memcpy(pProbePkt + 10 + 70, my_mac, 6);  
					/* client identifier */
					memcpy(pProbePkt + 10 + 288, my_mac, 6);  

					memset(pProbePkt + 10 + 24, 0, 2);
					put16(pProbePkt + 10 + 18, get16(pProbePkt + 10 + 18) + trans_id++);
					put16(pProbePkt + 10 + 24, in_cksum(NULL, (uint16 *)(pProbePkt + 10 + 14), 20));

					put32(pProbePkt + 10 + 46, get32(pProbePkt + 10 + 46) + trans_id++);
				

					/* Clear checksum for later recalculation */
					pseudohdr.source= 0x0000;
					pseudohdr.dest = 0xffff;
					pseudohdr.protocol = 17;
					pseudohdr.length = 556;
					memset(pProbePkt + 10 + 40, 0, 2);
					/* If requested, recompute checksum and insert into header */
					put16(pProbePkt + 10 + 40,in_cksum(&pseudohdr, (uint16 *)(pProbePkt + 10 + 34), 556));
					vcPoolSendOnePkt(pProbePkt, sizeof(RFC1483_LLC_HDR) + sizeof(DHCP_REQ_NEW), vpi, vci);
					vcPoolSendOnePkt(pProbePkt, sizeof(RFC1483_LLC_HDR) + sizeof(DHCP_REQ_NEW), vpi, vci);
				}
			}
#endif			
		}
		else{
			memcpy(pProbePkt, RFC1483_VC_HDR, sizeof(RFC1483_VC_HDR));
//			if (vcPoolProbeDHCPDiscover) {
			if(1){
				memcpy(pProbePkt + sizeof(RFC1483_VC_HDR), DHCP_DISCOVER, sizeof(DHCP_DISCOVER));
				/* SA */
				memcpy(pProbePkt + 10 + 6, my_mac, 6);	
				/* client hardware address */
				memcpy(pProbePkt + 10 + 70, my_mac, 6);  
				/* client identifier */
				memcpy(pProbePkt + 10 + 291, my_mac, 6);  
			
				memset(pProbePkt + 10 + 24, 0, 2);
				put16(pProbePkt + 10 + 18, get16(pProbePkt + 10 + 18) + trans_id++);
				put16(pProbePkt + 10 + 24, in_cksum(NULL, (uint16 *)(pProbePkt + 10 + 14), 20));			
				put32(pProbePkt + 10 + 46, get32(pProbePkt + 10 + 46) + trans_id++);			
			
				/* Clear checksum for later recalculation */
				pseudohdr.source= 0x0000;
				pseudohdr.dest = 0xffff;
				pseudohdr.protocol = 17;
				pseudohdr.length = 308;
				memset(pProbePkt + 10 + 40, 0, 2);
				/* If requested, recompute checksum and insert into header */
				put16(pProbePkt + 10 + 40,in_cksum(&pseudohdr, (uint16 *)(pProbePkt + 10 + 34), 308));
				vcPoolSendOnePkt(pProbePkt, sizeof(RFC1483_VC_HDR) + sizeof(DHCP_DISCOVER), vpi, vci);
				vcPoolSendOnePkt(pProbePkt, sizeof(RFC1483_VC_HDR) + sizeof(DHCP_DISCOVER), vpi, vci);
			} 
#if 0			
			else {
				if (!vcPoolProbeNewDHCP) {
					memcpy(pProbePkt + sizeof(RFC1483_VC_HDR), DHCP_REQ, sizeof(DHCP_REQ));
					vcPoolSendOnePkt(pProbePkt, sizeof(RFC1483_VC_HDR) + sizeof(DHCP_REQ), vpi, vci);
					vcPoolSendOnePkt(pProbePkt, sizeof(RFC1483_VC_HDR) + sizeof(DHCP_REQ), vpi, vci);
				} else {
					memcpy(pProbePkt + sizeof(RFC1483_VC_HDR), DHCP_REQ_NEW, sizeof(DHCP_REQ_NEW));
					/* SA */
					memcpy(pProbePkt + 10 + 6, my_mac, 6);  
					/* client hardware address */
					memcpy(pProbePkt + 10 + 70, my_mac, 6);  
					/* client identifier */
					memcpy(pProbePkt + 10 + 288, my_mac, 6);  

					memset(pProbePkt + 10 + 24, 0, 2);
					put16(pProbePkt + 10 + 18, get16(pProbePkt + 10 + 18) + trans_id++);
					put16(pProbePkt + 10 + 24, in_cksum(NULL, (uint16 *)(pProbePkt + 10 + 14), 20));

					put32(pProbePkt + 10 + 46, get32(pProbePkt + 10 + 46) + trans_id++);
				

					/* Clear checksum for later recalculation */
					pseudohdr.source= 0x0000;
					pseudohdr.dest = 0xffff;
					pseudohdr.protocol = 17;
					pseudohdr.length = 556;
					memset(pProbePkt + 10 + 40, 0, 2);
					/* If requested, recompute checksum and insert into header */
					put16(pProbePkt + 10 + 40,in_cksum(&pseudohdr, (uint16 *)(pProbePkt + 10 + 34), 556));
					vcPoolSendOnePkt(pProbePkt, sizeof(RFC1483_VC_HDR) + sizeof(DHCP_REQ_NEW), vpi, vci);
					vcPoolSendOnePkt(pProbePkt, sizeof(RFC1483_VC_HDR) + sizeof(DHCP_REQ_NEW), vpi, vci);
				}
			}
#endif			
		}
	}
	return;

}


/*______________________________________________________________________________
**  reset_all_data
**
**  descriptions:reset all global data
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
void reset_all_data(void){
	g_manageFlag=0;
	g_vcPoolProbeService=0;
	memset(g_pvc_pool, 0, sizeof(g_pvc_pool));
	memset(g_vc_pool, 0, sizeof(g_vc_pool));
	memset(g_pcmac_addr, 0, sizeof(g_pcmac_addr));
}

#if	!defined(TCSUPPORT_CPU_MT7510)
/*______________________________________________________________________________
**  clearTxCcDescrPool
**
**  descriptions:clear tx cc descriptor pool
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
void clearTxCcDescrPool(void){
	uint8 descrId;
	atmTxCcDescr_t *atmTxCcDescrp;
	atmTxCcDescrPool_t *atmTxCcDescrVcPoolp;
	atmTxCcDescrVcPoolp = (atmTxCcDescrPool_t *)CACHE_TO_NONCACHE(atmTxCcDescrPool);
	if(atmTxCcDescrPool){
		for ( descrId = 0; descrId < ATM_TX_CC_DESCR_NUMMAX; descrId++ )
		{
			atmTxCcDescrp = atmTxCcDescrVcPoolp->txCcDescrPool+ descrId;
			if ( descrId == ATM_TX_CC_DESCR_NUMMAX - 1 ) {
				//BD_GAP, EOR = 1, others=0;
				atmTxCcDescrp->tdes0 =TSARM_TX_DESCR_BD_GAP(TSARM_TX_CC_BD_GAP)
							| TSARM_TX_DESCR_EOR;
			}else{
				//BD_GAP, others=0;
				atmTxCcDescrp->tdes0 =TSARM_TX_DESCR_BD_GAP(TSARM_TX_CC_BD_GAP);
			}
			// TDES1 =0 ; Buffer length =0;
			atmTxCcDescrp->tdes1 = 0x0;
			// TDES2 = 0x0; buffer address = null
			atmTxCcDescrp->tdes2 = 0x0;
			atmTxCcDescrp->tdes3 = 0x0;
		}
	}
}
#endif

/*______________________________________________________________________________
**  doVcPoolActive
**
**  descriptions:use to control the sar side autopvc on/off
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int
doVcPoolActive(
	int     argc,
	char    *argv[],
	void    *p
)
{

	int active = simple_strtoul(argv[1],NULL,10);

	if ((active!=0)&&(active!=1) ) {
		printk("Usage: active <1|0>\n");
		return -1;
	}

	reset_all_data();

	if (active) {
		fin = 0;
		g_manageFlag |= VC_HUNT_FLAG_MANG_START;
		vcPoolControlCC(0);
		vcPoolControlCC(1);
	} else {
		fin = 1;
		g_manageFlag &= ~VC_HUNT_FLAG_MANG_START;
		vcPoolControlCC(0);
#if !defined(TCSUPPORT_CPU_MT7510)
		clearTxCcDescrPool();
#endif
	}

	return 0;
}

/*______________________________________________________________________________
**  doVcPoolSet
**
**  descriptions:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int
doVcPoolSet(
	int     argc,
	char    *argv[],
	void    *p
)
{

	int rnIndex;
	uint8 vpi;
	uint16 vci;
	int encap;
	int i;
	vc_pool_t tmpVC;

	if((argc == 8) && (memcmp(argv[1], "mac", 3) == 0)){//set mac byte0 byte1 byte2 byte3 byte4 byte5 byte6
		for(i=0;i<6;i++){
			g_pcmac_addr[i]=simple_strtoul(argv[i+2],NULL,16);
		}
		printk("\r\n");
#if 0
		printk("0: vpi = %d, vci = %d\r\n", g_vc_pool[0].vpi, g_vc_pool[0].vci);
		printk("1: vpi = %d, vci = %d\r\n", g_vc_pool[1].vpi, g_vc_pool[1].vci);
#endif		
	}
	/*
	 * set pvc 		index vpi vci encap
	 * set vcpool 	index vpi vci
	 */
	else if(argc >= 5){ //set pvc or vcpool
		rnIndex = simple_strtoul(argv[2],NULL,10);
		vpi = simple_strtoul(argv[3],NULL,10);
		vci = simple_strtoul(argv[4],NULL,10);
		if  (vci < 1)  {
			printk("Error: vpi:%d or vci:%d is in the wrong range\r\n",vpi,vci);
			return -1;
		}

		if((argc == 6) && (memcmp(argv[1], "pvc", 3) == 0)){//autopvc set pvc index vpi vci encap
			/* Check the remoteNode conflict or not */
#if !defined(TCSUPPORT_AUTO_PVC_TRIGGER) 
			for (i = 0; i < MAX_PVC_NUM; i++) {
#endif
				tmpVC = g_pvc_pool[i];
				if ((tmpVC.vpi == vpi) &&
					(tmpVC.vci == vci)) {
					printk("Error: duplicate vpi %d and vci %d in current PVC setting\n",vpi,vci);
					return -1;
				}
#if !defined(TCSUPPORT_AUTO_PVC_TRIGGER)
			}
#endif
			encap=simple_strtoul(argv[5],NULL,10);
			if((encap != 0) && (encap != 1)){
				printk("Error: wrong encapsulation when setting pvc pool\n");
				return -1;
			}
#if !defined(TCSUPPORT_AUTO_PVC_TRIGGER) 
			if ((rnIndex < 0) || (rnIndex > MAX_PVC_NUM -1)) {
#endif
				printk("Error: wrong index when set pvc pool, %d\r\n", rnIndex);
				return -1;
			}
			g_pvc_pool[rnIndex].vpi=vpi;
			g_pvc_pool[rnIndex].vci=vci;
			g_pvc_pool[rnIndex].encap=encap;
		}
		else if((argc == 5) && (memcmp(argv[1], "vcpool", 6) == 0)){//autopvc set vcpool index vpi vci
			if  (vci < 1)  {
				printk("Error: vpi:%d or vci:%d is in the wrong range\r\n",vpi,vci);
				return -1;
			}
#if 0
			for (i = 0; i < MAX_VC_POOL_NUM; i++) {
				tmpVC = g_vc_pool[i];
				if ((tmpVC.vpi == vpi) &&
					(tmpVC.vci == vci)) {
					printk("Error: duplicate vpi and vci in VC pool\n");
					return -1;
				}
			}

#endif
			if ((rnIndex < 0) || (rnIndex > MAX_VC_POOL_NUM -1)) {
				printk("Error: wrong index when set vcpool, %d\r\n", rnIndex);
				return -1;
			}
			g_vc_pool[rnIndex].vpi=vpi;
			g_vc_pool[rnIndex].vci=vci;
		}
		else{
			printk("Error: autopvc set <pvc|vcpool|mac>\r\n");
		}
	}
	else{
		printk("Error: too less arguments when autopvc set\r\n");
	}

	return 0;
}

/*______________________________________________________________________________
**  doVcPoolProbe
**
**  descriptions:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int
doVcPoolProbe(
	int     argc,
	char    *argv[],
	void    *p
)
{
	int vcHuntSetNumber=0;
	int n=0,i=0;
	uint8 vpi=0;
	uint16 vci=0;
	int encap=0;
	vc_pool_t *pTmpVcPool=NULL;

	if(memcmp(argv[1],"pvc",3) == 0){
#if !defined(TCSUPPORT_AUTO_PVC_TRIGGER) 
		vcHuntSetNumber=MAX_PVC_NUM;
#endif
		g_vcHuntSet=HUNT_PVC;
		pTmpVcPool=g_pvc_pool;
	}
	else if(memcmp(argv[1],"vcpool",6) == 0){
		vcHuntSetNumber=MAX_VC_POOL_NUM;
		g_vcHuntSet=HUNT_VCPOOL;
		pTmpVcPool=g_vc_pool;
#if 0		
		printk("0: vpi = %d, vci = %d\r\n", g_vc_pool[0].vpi, g_vc_pool[0].vci);
		printk("1: vpi = %d, vci = %d\r\n", g_vc_pool[1].vpi, g_vc_pool[1].vci);
#endif		
	}

	for( n=0; n<SERVICE_NUMBER; n++){ // three types of pkt
		for (i = 0; i < vcHuntSetNumber; i++) {
			vpi=pTmpVcPool[i].vpi;
			vci=pTmpVcPool[i].vci;
			encap=pTmpVcPool[i].encap;
			if (vci == 0)
				continue;
//			if ((vpi == spSysParam.remoteNode[0].atmPf.vpiBrdg) &&
//				(vci == spSysParam.remoteNode[0].atmPf.vciBrdg))
//				continue;
//			if (L2ProbePktReady & PADI_FIRST && n==0)
//				continue;
//			if( n==0 && (vcPoolProbeService & PROBE_ARP))
//				vcPoolProbeVcWithARP(vpi, vci);
			if( n==1 && (g_vcPoolProbeService & PROBE_PPPOE)){
				vcPoolProbeVcWithPADI(vpi, vci, encap);
			}
#if 1			
			else if( n==2 && (g_vcPoolProbeService & PROBE_DHCP)){
				vcPoolProbeVcWithDHCP(vpi, vci, encap);
			}
#endif			
		}
	}
	return 0;
}

/*______________________________________________________________________________
**  doVcPoolService
**
**  descriptions:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int
doVcPoolService(
	int 	argc,
	char	*argv[],
	void	*p
)
{
	if (argc == 2) {
		g_vcPoolProbeService=simple_strtoul(argv[1],NULL,16);
//		return 0;
	}
#if 0	
	printk("Probe Service Type: ");

	if (g_vcPoolProbeService & PROBE_OAM)
		printk("OAM ");
	if (g_vcPoolProbeService & PROBE_PPPOE)
		printk("PPPoE ");

	if (g_vcPoolProbeService & PROBE_ARP)
		printk("ARP ");

	if (g_vcPoolProbeService & PROBE_DHCP)
		printk("DHCP ");
#endif	

#if 0	
	printk("0: vpi = %d, vci = %d\r\n", g_vc_pool[0].vpi, g_vc_pool[0].vci);
	printk("1: vpi = %d, vci = %d\r\n", g_vc_pool[1].vpi, g_vc_pool[1].vci);
#endif

	printk("\n");
	return 0;
}

/*______________________________________________________________________________
**  doVcPoolEncap
**
**  descriptions:set encap
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int
doVcPoolEncap(
	int     argc,
	char    *argv[],
	void    *p
)
{
	int i=0;
	uint8 encap=0;
	encap=simple_strtoul(argv[1],NULL,10);
	for(i=0;i<MAX_VC_POOL_NUM;i++){
		g_vc_pool[i].encap=encap;
	}
#if 0	
	printk("0: vpi = %d, vci = %d\r\n", g_vc_pool[0].vpi, g_vc_pool[0].vci);
	printk("1: vpi = %d, vci = %d\r\n", g_vc_pool[1].vpi, g_vc_pool[1].vci);
	printk("encap = %d\r\n", g_vc_pool[0].encap);
#endif	
	return 0;
}

/*______________________________________________________________________________
**  doVcPoolResult
**
**  descriptions:add the autopvc result into buffer, this buffer will send to proc
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
static int doVcPoolResult(char *buf){
	int i=0;
	uint8 pvc_bit_map=0;
	uint16 vc_pool_bit_map=0;
	uint16 index = 0;

	index += sprintf(buf+index, "pvc ");
#if !defined(TCSUPPORT_AUTO_PVC_TRIGGER) 
	for(i=0;i<MAX_PVC_NUM;i++){
#endif
		if((g_pvc_pool[i].management == VC_HUNT_FLAG_HUNT_SUCC)){
			pvc_bit_map|=(1<<i);
		}
#if !defined(TCSUPPORT_AUTO_PVC_TRIGGER)
	}
#endif

	index += sprintf(buf+index, "%u\n",pvc_bit_map);

	index += sprintf(buf+index, "vcpool ");
	for(i=0;i<MAX_VC_POOL_NUM;i++){
		if((g_vc_pool[i].management == VC_HUNT_FLAG_HUNT_SUCC)){
			vc_pool_bit_map|=(1<<i);
		}
	}
	index += sprintf(buf+index, "%u\n",vc_pool_bit_map);
	return index;
}


/*autopvc cmd table*/
static const cmds_t vcPoolCmds[] = {
	{"active",		doVcPoolActive, 		0x02,	1,	"active <1|0>"},//0x02
	{"set",			doVcPoolSet, 			0x02,	2,	NULL},
	{"encap",		doVcPoolEncap, 			0x02,	1,	"<0(LLC)|1(VC)>"},
	{"probe", 		doVcPoolProbe,			0x02,	1, 	"<pvc|vcpool>"},
	{"service",		doVcPoolService, 		0x02,	0,	"service [probe service]"},
};
/*autopvc cmd table*/

/*______________________________________________________________________________
**  doVcPoolCmd
**
**  descriptions:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int
doVcPoolCmd(
	int     argc,
	char    *argv[],
	void    *p
)
{
	return subcmd(vcPoolCmds, argc, argv, p);

}

/*______________________________________________________________________________
**  tsarm_autopvc_read_proc
**
**  descriptions:return the result of autopvc from proc
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
static int tsarm_autopvc_read_proc(char *buf, char **start, off_t off, int count,
        int *eof, void *data)
{
	int len = doVcPoolResult(buf);
	if (len <= off+count)
		*eof = 1;
	*start = buf + off;
	len -= off;
	if (len>count)
		len = count;
	if (len<0)
		len = 0;
	return len;
}

static int tsarm_autopvc_fin_read_proc(char *buf, char **start, off_t off, int count,
        int *eof, void *data)
{
	int len = sprintf(buf, "%d\r\n", fin);
	return len;
}

static int tsarm_AUTOPVC_read_proc(char *buf, char **start, off_t off, int count,
        int *eof, void *data)
{
	int len;
	char encap[4], protocol[6];
	
//	printk("g_vc_pool[0].encap = %d\r\n", g_vc_pool[0].encap);
	switch(encap_detected){
		case 0:
			sprintf(encap, "%s", "llc");
			break;
		case 1:
			sprintf(encap, "%s", "vc");
			break;
		default:
			sprintf(encap, "%s", "llc");
			break;
	}
//	printk("g_vcPoolProbeService = %d\r\n", g_vcPoolProbeService);
	if (service_detected & PROBE_PPPOE)
		sprintf(protocol, "pppoe");
	else if (service_detected & PROBE_DHCP)
		sprintf(protocol, "dhcp");
	else
		sprintf(protocol, "null");

	if(!vpi_detected && !vci_detected)
		len = 0;
	else
	len = sprintf(buf, "(%d,%d,%s,%s)\r\n", vpi_detected, vci_detected, encap, protocol);

	return len;
}

static int tsarm_AUTOPVC_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[32];

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';

	if(val_string[0] == '1'){
		vpi_detected = 0;
		vci_detected = 0;
		service_detected = 0;
		encap_detected = 0;
	}

	return count;
}

/*_____________________________________________________________________________
**      function name: autopvcInit
**      descriptions:
**        	register autopvc cmd and allocate buffer.
**      parameters:
**           None
**      global:
**           None
**      return:
**         	None
**      call:
**           autopvc_cmd_register_to_sys
**      revision:
**      1. Krammer 2009/03/11
**____________________________________________________________________________
*/
void
autopvcInit(void)
{
	uint32 mem_size=0;
	/*Register autopvc ci-cmd*/
	vcPoolCmd.func=doVcPoolCmd;
	struct proc_dir_entry *tsarm_proc;
	autopvc_cmd_register_to_sys(&vcPoolCmd);

	create_proc_read_entry("tc3162/autopvc", 0, NULL, tsarm_autopvc_read_proc, NULL);
	create_proc_read_entry("tc3162/autopvc_fin", 0, NULL, tsarm_autopvc_fin_read_proc, NULL);
	tsarm_proc = create_proc_entry("AUTO_PVC_SEARCH", 0, NULL);
	tsarm_proc->read_proc = tsarm_AUTOPVC_read_proc;
	tsarm_proc->write_proc = tsarm_AUTOPVC_write_proc;

	mem_size=sizeof(uint8)*AAL5_CELL_BUFFER_SIZE+ALIGN32BYTESBASE;
	if ((AAL5_CELL = kmalloc(mem_size, GFP_KERNEL)) == NULL) {
		printk("unable to kmalloc atmCellCirBuf structure.\n");
	}
	dma_cache_inv((unsigned long)AAL5_CELL, mem_size);
	AAL5_CELL=(uint8 *) CACHE_TO_NONCACHE((uint32)AAL5_CELL);
	memset((uint8*)AAL5_CELL, 0, mem_size);
	AAL5_CELL=(uint8 *)align32Byte((uint32)AAL5_CELL);

}/*end autopvcInit*/

void
autopvcExit(void){
    autopvc_cmd_unregister_to_sys();
	remove_proc_entry("tc3162/autopvc",NULL);
	remove_proc_entry("tc3162/autopvc_fin",NULL);
	remove_proc_entry("AUTO_PVC_SEARCH",NULL);
	if (AAL5_CELL){
		AAL5_CELL=(uint8 *)K1_TO_K0((uint32)AAL5_CELL);
		kfree(AAL5_CELL);
		AAL5_CELL=NULL;
	}
}
