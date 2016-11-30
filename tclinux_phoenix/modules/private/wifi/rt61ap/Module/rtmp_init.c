/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2005, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attempt
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

	Module Name:
	rtmp_init.c

	Abstract:
	Miniport generic portion header file

	Revision History:
	Who         When          What
	--------    ----------    ----------------------------------------------
	Paul Lin    2002-08-01    created
    John Chang  2004-08-20    RT2561/2661 use scatter-gather scheme
*/
#include    "rt_config.h"

UCHAR    BIT8[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
ULONG    BIT32[] = {0x00000001, 0x00000002, 0x00000004, 0x00000008,
                    0x00000010, 0x00000020, 0x00000040, 0x00000080,
                    0x00000100, 0x00000200, 0x00000400, 0x00000800,
                    0x00001000, 0x00002000, 0x00004000, 0x00008000,
                    0x00010000, 0x00020000, 0x00040000, 0x00080000,
                    0x00100000, 0x00200000, 0x00400000, 0x00800000,
                    0x01000000, 0x02000000, 0x04000000, 0x08000000,
                    0x10000000, 0x20000000, 0x40000000, 0x80000000};

char*   CipherName[] = {"none","wep64","wep128","TKIP","AES","CKIP64","CKIP128"};

const unsigned short ccitt_16Table[] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
    0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
    0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
    0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
    0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
    0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
    0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
    0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
    0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
    0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
    0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
    0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
    0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
    0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
    0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
    0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
    0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
    0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
    0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
    0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
    0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
    0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
    0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};
#define ByteCRC16(v, crc) \
    (unsigned short)((crc << 8) ^  ccitt_16Table[((crc >> 8) ^ (v)) & 255])

//
// BBP register initialization set
//
BBP_REG_PAIR   BBPRegTable[] = {
	{3,     0x00},
	{15,    0x30},
	{17,    0x40},
	{21,    0xc8},
	{22,    0x38},
	{23,    0x06},
	{24,    0xfe},
	{25,    0x0a},
	{26,    0x0d},
	{34,    0x12},
	{37,    0x07},
	{39,    0xf8}, // 2005-09-02 by Gary, Atheros 11b issue 
	{41,    0x60},
	{53,    0x10},
	{54,    0x18},
	{60,    0x10},
	{61,    0x04},
	{62,    0x04},
	{75,    0xfe},
	{86,    0xfe},
	{88,    0xfe},
	{90,    0x0f},
	{99,    0x00},
	{102,   0x16},
	{107,   0x04},
};
#define	NUM_BBP_REG_PARMS	(sizeof(BBPRegTable) / sizeof(BBP_REG_PAIR))

//
// ASIC register initialization sets
//
RTMP_REG_PAIR	MACRegTable[] =	{
//	{MAC_CSR11,     0x0000000a}, // 0x302c, power state transition time
//	{TXRX_CSR5,     0x0000000f}, // 0x3054, Basic rate set bitmap
//	{TXRX_CSR0,     0x0277b032}, // 0x3040, RX control
	{TXRX_CSR1,     0x9eb39eb3}, // 0x3044, BBP 51:RSSI, R42:OFDM rate, R47:CCK SIGNAL
    {TXRX_CSR2,     0x8a8b8c8d}, // 0x3048, CCK TXD BBP registers
    {TXRX_CSR3,     0x00858687}, // 0x304c, OFDM TXD BBP registers
    {TXRX_CSR4,     0xf44a0732}, // 0x3050, Auto Responder / TX retransmission register
    {TXRX_CSR7,     0x2E31353B}, // 0x305c, ACK/CTS payload consume time for 18/12/9/6 mbps
    {TXRX_CSR8,     0x2a2a2a2c}, // 0x3060, ACK/CTS payload consume time for 54/48/36/24 mbps
    {TXRX_CSR10,    0x00001000}, // 0x3068, default value = 0x1010, but we need to disable
                                 //         IBSS mode TBTT phase adaptive adjustment step
    {TXRX_CSR15,    0x0000000f}, // 0x307c, TKIP MIC priority byte "AND" mask
    {MAC_CSR6,      0x00000fff}, // 0x3018, MAX frame length
	{MAC_CSR10,     0x00000718}, // 0x3028, ASIC PIN control in various power states
	{MAC_CSR12,     0x00000004}, // 0x3030, power state control, set to AWAKE state
    {MAC_CSR13,     0x0000fe00}, // 0x3034, GPIO pin#0 as bHwRadio
//	{INT_SOURCE_CSR,0xffffffff}, // 0x3068, Clear all pending interrupt source
//  {MAC_CSR14,     0x00001e6}, // 0x3038, default both LEDs off
//  {PHY_CSR2,      0x82188200}, // 0x308c, pre-TX BBP control
//	{TXRX_CSR11,    0x0000e78f}, // 0x306c, AES, mask off more data bit for MIC calculation
//	{TX_DMA_DST_CSR,0x000000aa}, // 0x342c, ASIC TX FIFO to host shared memory mapping
    {SEC_CSR0,      0x00000000}, // 0x30a0, invalidate all shared key entries
    {SEC_CSR1,      0x00000000}, // 0x30a4, reset all shared key algorithm to "none"
    {SEC_CSR5,      0x00000000}, // 0x30b4, reset all shared key algorithm to "none"
    {PHY_CSR5,		0x060a100c}, // 0x00040a06
	{PHY_CSR6,		0x00080606},
	{PHY_CSR7,		0x00000a08},
	{PCI_CFG_CSR,   0x28ca4404}, // 0x3460, PCI configuration (old value will cause DMA issue, modified by Mark Liu)
    {AIFSN_CSR,     0x00002273},
    {CWMIN_CSR,     0x00002344},
    {CWMAX_CSR,     0x000034aa},
    {MAC_CSR8,      0x016c030a}, // 0x3020, SIFS time for compensation
    {PHY_CSR1,      0x000023b0}, // 0x3084, BBP Register R/W mode set to "Parallel mode"
    //{TEST_MODE_CSR, 0x00000200}, // 0x3484, Count TXOP anyway. by Mark 2005/07/27 for WMM S3T07 issue
    {M2H_CMD_DONE_CSR, 0xffffffff}, // 0x2104, clear M2H_CMD_DONE mailbox
};
#define	NUM_MAC_REG_PARMS	(sizeof(MACRegTable) / sizeof(RTMP_REG_PAIR))

/*
	========================================================================
	
	Routine Description:
		Allocate RTMP_ADAPTER data block and do some initialization

	Arguments:
		Adapter		Pointer to our adapter

	Return Value:
		NDIS_STATUS_SUCCESS
		NDIS_STATUS_FAILURE

	Note:
	
	========================================================================
*/
/*KH:Move two functions, "RTMPusecDealy and RTMPBuildDesireRate, to the head of this file for resolving
the limitation of cross-compilers' jump*/
// Unify all delay routine by using udelay
VOID	RTMPusecDelay(
	IN	ULONG	usec)
{
	ULONG	i;

	for (i = 0; i < (usec / 50); i++)
		udelay(50);

	if (usec % 50)
		udelay(usec % 50);
}
VOID	RTMPBuildDesireRate(
	IN PRTMP_ADAPTER 	pAd,
	IN UCHAR			apidx,
	IN UCHAR			rate)
{
	NdisZeroMemory(pAd->PortCfg.MBSSID[apidx].DesiredRates, MAX_LEN_OF_SUPPORTED_RATES);
	switch (pAd->PortCfg.PhyMode)
	{
		case PHY_11A: // A only
			switch (rate)
			{
				case 0:
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0] = 0x6c; // 54Mbps
					pAd->PortCfg.MBSSID[apidx].DesiredRates[1] = 0x60; // 48Mbps
					pAd->PortCfg.MBSSID[apidx].DesiredRates[2] = 0x48; // 36Mbps
					pAd->PortCfg.MBSSID[apidx].DesiredRates[3] = 0x30; // 24Mbps
					pAd->PortCfg.MBSSID[apidx].DesiredRates[4] = 0x24; // 18M
					pAd->PortCfg.MBSSID[apidx].DesiredRates[5] = 0x18; // 12M
					pAd->PortCfg.MBSSID[apidx].DesiredRates[6] = 0x12; // 9M
					pAd->PortCfg.MBSSID[apidx].DesiredRates[7] = 0x0c; // 6M
					break;
				case 1:
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0] = 0x0c; // 6M
					break;
				case 2:
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0] = 0x12; // 9M
					break;
				case 3:
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0] = 0x18; // 12M
					break;
				case 4:
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0] = 0x24; // 18M
					break;
				case 5:
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0] = 0x30; // 24M
					break;
				case 6:
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0] = 0x48; // 36M
					break;
				case 7:
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0] = 0x60; // 48M
					break;
				case 8:
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0] = 0x6c; // 54M
					break;
				//Tuning to max desired rate
				case 9: // 6M
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0] = 0x0c;
					break;
				case 10: // 9M
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0] = 0x0c;
					pAd->PortCfg.MBSSID[apidx].DesiredRates[1] = 0x12;
					break;
				case 11: // 12M
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0] = 0x0c;
					pAd->PortCfg.MBSSID[apidx].DesiredRates[1] = 0x12;
					pAd->PortCfg.MBSSID[apidx].DesiredRates[2] = 0x18;
					break;
				case 12: // 18M
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0] = 0x0c;
					pAd->PortCfg.MBSSID[apidx].DesiredRates[1] = 0x12;
					pAd->PortCfg.MBSSID[apidx].DesiredRates[2] = 0x18;
					pAd->PortCfg.MBSSID[apidx].DesiredRates[3] = 0x24;
					break;
				case 13: // 24M
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0] = 0x0c;
					pAd->PortCfg.MBSSID[apidx].DesiredRates[1] = 0x12;
					pAd->PortCfg.MBSSID[apidx].DesiredRates[2] = 0x18;
					pAd->PortCfg.MBSSID[apidx].DesiredRates[3] = 0x24;
					pAd->PortCfg.MBSSID[apidx].DesiredRates[4] = 0x30;
					break;
				case 14: // 36M
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0] = 0x0c;
					pAd->PortCfg.MBSSID[apidx].DesiredRates[1] = 0x12;
					pAd->PortCfg.MBSSID[apidx].DesiredRates[2] = 0x18;
					pAd->PortCfg.MBSSID[apidx].DesiredRates[3] = 0x24;
					pAd->PortCfg.MBSSID[apidx].DesiredRates[4] = 0x30;
					pAd->PortCfg.MBSSID[apidx].DesiredRates[5] = 0x48;
					break;
				case 15: // 48M
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0] = 0x0c;
					pAd->PortCfg.MBSSID[apidx].DesiredRates[1] = 0x12;
					pAd->PortCfg.MBSSID[apidx].DesiredRates[2] = 0x18;
					pAd->PortCfg.MBSSID[apidx].DesiredRates[3] = 0x24;
					pAd->PortCfg.MBSSID[apidx].DesiredRates[4] = 0x30;
					pAd->PortCfg.MBSSID[apidx].DesiredRates[5] = 0x48;
					pAd->PortCfg.MBSSID[apidx].DesiredRates[6] = 0x60;
					break;
				case 16: // 54M
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0] = 0x0c;
					pAd->PortCfg.MBSSID[apidx].DesiredRates[1] = 0x12;
					pAd->PortCfg.MBSSID[apidx].DesiredRates[2] = 0x18;
					pAd->PortCfg.MBSSID[apidx].DesiredRates[3] = 0x24;
					pAd->PortCfg.MBSSID[apidx].DesiredRates[4] = 0x30;
					pAd->PortCfg.MBSSID[apidx].DesiredRates[5] = 0x48;
					pAd->PortCfg.MBSSID[apidx].DesiredRates[6] = 0x60;
					pAd->PortCfg.MBSSID[apidx].DesiredRates[7] = 0x6c;
					break;
			}
			break;
		case PHY_11BG_MIXED: // B/G Mixed
		case PHY_11B: // B only
		case PHY_11G: // G Only
		default:
			switch (rate)
			{
				case 0:
					switch (pAd->PortCfg.PhyMode)
					{
						case PHY_11B: // B Only
							pAd->PortCfg.MBSSID[apidx].DesiredRates[0] = 0x16; // 11Mbps
							pAd->PortCfg.MBSSID[apidx].DesiredRates[1] = 0x0b; // 5.5Mbps
							pAd->PortCfg.MBSSID[apidx].DesiredRates[2] = 0x04; // 2Mbps
							pAd->PortCfg.MBSSID[apidx].DesiredRates[3] = 0x02; // 1Mbps
							break;
						case PHY_11G: // G Only
						case PHY_11BG_MIXED: // B/G Mixed
						default:
							pAd->PortCfg.MBSSID[apidx].DesiredRates[0] = 0x6c; // 54Mbps
							pAd->PortCfg.MBSSID[apidx].DesiredRates[1] = 0x60; // 48Mbps
							pAd->PortCfg.MBSSID[apidx].DesiredRates[2] = 0x48; // 36Mbps
							pAd->PortCfg.MBSSID[apidx].DesiredRates[3] = 0x30; // 24Mbps
							pAd->PortCfg.MBSSID[apidx].DesiredRates[4] = 0x16; // 11Mbps
							pAd->PortCfg.MBSSID[apidx].DesiredRates[5] = 0x0b; // 5.5Mbps
							pAd->PortCfg.MBSSID[apidx].DesiredRates[6] = 0x04; // 2Mbps
							pAd->PortCfg.MBSSID[apidx].DesiredRates[7] = 0x02; // 1Mbps
							break;
					}
					break;
				case 1:
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0] = 0x02; // 1M
					break;
				case 2:
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0] = 0x04; // 2M
					break;
				case 3:
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0] = 0x0b; // 5.5M
					break;
				case 4:
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0] = 0x16; // 11M
					break;
				case 5:
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0] = 0x0c; // 6M
					break;
				case 6:
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0] = 0x12; // 9M
					break;
				case 7:
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0] = 0x18; // 12M
					break;
				case 8:
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0] = 0x24; // 18M
					break;
				case 9:
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0] = 0x30; // 24M
					break;
				case 10:
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0] = 0x48; // 36M
					break;
				case 11:
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0] = 0x60; // 48M
					break;
				case 12:
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0] = 0x6c; // 54M
					break;
				// Dynamic tune tx rate to Max desiredRates
				case 13: // 1M
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0]  = 2;     // 1 mbps, in units of 0.5 Mbps
					break;
				case 14: // 2M
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0]  = 2;     // 1 mbps, in units of 0.5 Mbps
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[1]  = 4;     // 2 mbps, in units of 0.5 Mbps
					break;
				case 15://5.5M
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0]  = 2; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[1]  = 4; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[2]  = 11;
					break;
				case 16://11M
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0]  = 2; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[1]  = 4; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[2]  = 11;
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[3]  = 22; 
					break;
				case 17://6M
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0]  = 2; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[1]  = 4; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[2]  = 11;
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[3]  = 22; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[4]  = 12; 
					break;
				case 18://9M
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0]  = 2; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[1]  = 4; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[2]  = 11;
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[3]  = 22; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[4]  = 12; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[5]  = 18; 
					break;
				case 19://12M
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0]  = 2; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[1]  = 4; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[2]  = 11;
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[3]  = 22; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[4]  = 12; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[5]  = 18; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[6]  = 24; 
					break;
				case 20://18M
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0]  = 2; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[1]  = 4; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[2]  = 11;
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[3]  = 22; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[4]  = 12; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[5]  = 18; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[6]  = 24; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[7]  = 36; 
					break;
				case 21://24M
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0]  = 2; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[1]  = 4; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[2]  = 11;
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[3]  = 22; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[4]  = 12; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[5]  = 18; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[6]  = 24; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[7]  = 36; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[8]  = 48; 
					break;
				case 22://36M
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0]  = 2; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[1]  = 4; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[2]  = 11;
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[3]  = 22; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[4]  = 12; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[5]  = 18; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[6]  = 24; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[7]  = 36; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[8]  = 48; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[9]  = 72; 
					break;
				case 23://48M
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0]  = 2; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[1]  = 4; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[2]  = 11;
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[3]  = 22; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[4]  = 12; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[5]  = 18; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[6]  = 24; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[7]  = 36; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[8]  = 48; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[9]  = 72; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[10] = 96; 
					break;
				case 24://54M
					pAd->PortCfg.MBSSID[apidx].DesiredRates[0]  = 2; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[1]  = 4; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[2]  = 11;
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[3]  = 22; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[4]  = 12; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[5]  = 18; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[6]  = 24; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[7]  = 36; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[8]  = 48; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[9]  = 72; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[10] = 96; 
		            pAd->PortCfg.MBSSID[apidx].DesiredRates[11] = 108;
					break;
			}
			break;
	}

	DBGPRINT(RT_DEBUG_TRACE, "I/F(ra%d) TxRate=(%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x)\n", apidx,
			pAd->PortCfg.MBSSID[apidx].DesiredRates[0],pAd->PortCfg.MBSSID[apidx].DesiredRates[1],
			pAd->PortCfg.MBSSID[apidx].DesiredRates[2],pAd->PortCfg.MBSSID[apidx].DesiredRates[3],
			pAd->PortCfg.MBSSID[apidx].DesiredRates[4],pAd->PortCfg.MBSSID[apidx].DesiredRates[5],
			pAd->PortCfg.MBSSID[apidx].DesiredRates[6],pAd->PortCfg.MBSSID[apidx].DesiredRates[7],
			pAd->PortCfg.MBSSID[apidx].DesiredRates[8],pAd->PortCfg.MBSSID[apidx].DesiredRates[9],
			pAd->PortCfg.MBSSID[apidx].DesiredRates[10],pAd->PortCfg.MBSSID[apidx].DesiredRates[11]);
}
NDIS_STATUS	RTMPAllocAdapterBlock(
	IN	PRTMP_ADAPTER	pAdapter)
{
	DBGPRINT(RT_DEBUG_TRACE, "--> RTMPAllocAdapterBlock\n");

	do
	{
		// Init spin locks
		NdisAllocateSpinLock(&pAdapter->TxRingLock);
		NdisAllocateSpinLock(&pAdapter->MgmtRingLock);
		NdisAllocateSpinLock(&pAdapter->RxRingLock);
        NdisAllocateSpinLock(&pAdapter->TxSwQueueLock);
	} while (FALSE);

	DBGPRINT(RT_DEBUG_TRACE, "<-- RTMPAllocAdapterBlock\n");
	
	return (NDIS_STATUS_SUCCESS);
}

/*
    ========================================================================

    Routine Description:
        Allocate all DMA releated resources

    Arguments:
        Adapter         Pointer to our adapter

    Return Value:
        None

    Note:

    ========================================================================
*/
NDIS_STATUS RTMPAllocDMAMemory(
    IN  PRTMP_ADAPTER   pAdapter)
{
	dma_addr_t		RingBasePa; /* UMAXLONG */
	PVOID			RingBaseVa;
	INT             index, num;
	PTXD_STRUC		pTxD;
	PRXD_STRUC		pRxD;
	PRTMP_TX_RING   pTxRing;
    PRTMP_DMABUF    pDmaBuf;
    struct sk_buff	*pSkb;

	DBGPRINT(RT_DEBUG_TRACE, "--> RTMPAllocDMAMemory\n");
	do
	{
		//
		// Allocate all ring descriptors, include TxD, RxD, MgmtD.
		// Although each size is different, to prevent cacheline and alignment
		// issue, I intentional set them all to 64 bytes.
		//
		for (num=0; num<NUM_OF_TX_RING; num++)
		{
		    dma_addr_t  BufBasePaLow;
		    PVOID  BufBaseVa;
		    
    		// 
    		// Allocate Tx ring descriptor's memory (5 TX rings = 4 ACs + 1 HCCA)
    		//
            pAdapter->TxDescRing[num].AllocSize = TX_RING_SIZE * TXD_SIZE;
    		pAdapter->TxDescRing[num].AllocVa
            	= pci_alloc_consistent(pAdapter->pPci_Dev, pAdapter->TxDescRing[num].AllocSize, &pAdapter->TxDescRing[num].AllocPa);

    		if (pAdapter->TxDescRing[num].AllocVa == NULL)
    		{
		    	DBGPRINT_ERR("Failed to allocate a big buffer\n");
    			break;
	    	}

    		// Zero init this memory block
	    	NdisZeroMemory(pAdapter->TxDescRing[num].AllocVa, pAdapter->TxDescRing[num].AllocSize);

    		// Save PA & VA for further operation
		    RingBasePa     = pAdapter->TxDescRing[num].AllocPa;
    		RingBaseVa     = pAdapter->TxDescRing[num].AllocVa;

    		// 
    		// Allocate all 1st TXBuf's memory for this TxRing
    		//
            pAdapter->TxBufSpace[num].AllocSize = TX_RING_SIZE * TX_DMA_1ST_BUFFER_SIZE;
    		pAdapter->TxBufSpace[num].AllocVa
            	= pci_alloc_consistent(pAdapter->pPci_Dev, pAdapter->TxBufSpace[num].AllocSize, &pAdapter->TxBufSpace[num].AllocPa);

    		if (pAdapter->TxBufSpace[num].AllocVa == NULL)
    		{
		    	DBGPRINT_ERR("Failed to allocate a big buffer\n");
    			break;
	    	}

    		// Zero init this memory block
	    	NdisZeroMemory(pAdapter->TxBufSpace[num].AllocVa, pAdapter->TxBufSpace[num].AllocSize);

    		// Save PA & VA for further operation
		    BufBasePaLow  = pAdapter->TxBufSpace[num].AllocPa;
    		BufBaseVa     = pAdapter->TxBufSpace[num].AllocVa;

    		//
	    	// Initialize Tx Ring Descriptor and associated buffer memory
		    //
		    pTxRing = &pAdapter->TxRing[num];
		    for (index = 0; index < TX_RING_SIZE; index++)
		    {
			    // Init Tx Ring Size, Va, Pa variables
			    pTxRing->Cell[index].AllocSize = TXD_SIZE;
			    pTxRing->Cell[index].AllocVa = RingBaseVa;
			    pTxRing->Cell[index].AllocPa = RingBasePa;

			    // Setup Tx Buffer size & address. only 802.11 header will store in this space
			    pDmaBuf = &pTxRing->Cell[index].DmaBuf;
                pDmaBuf->AllocSize = TX_DMA_1ST_BUFFER_SIZE;
                pDmaBuf->AllocVa = BufBaseVa;
                pDmaBuf->AllocPa = BufBasePaLow;
                
			    // link the pre-allocated TxBuf to TXD
			    pTxD = (PTXD_STRUC) pTxRing->Cell[index].AllocVa;
			    pTxD->BufCount = 1;
			    pTxD->BufPhyAddr0 = BufBasePaLow;

#ifdef BIG_ENDIAN
				RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
#endif
                // advance to next ring descriptor address
                RingBasePa += TXD_SIZE;
                RingBaseVa = (PUCHAR) RingBaseVa + TXD_SIZE;
            
                // advance to next TxBuf address
                BufBasePaLow += TX_DMA_1ST_BUFFER_SIZE;
                BufBaseVa = (PUCHAR) BufBaseVa + TX_DMA_1ST_BUFFER_SIZE;
		    }
            DBGPRINT(RT_DEBUG_TRACE, "TxRing[%d]: total %d entry allocated\n", num, index);
	    }

		//
		// Allocate MGMT ring descriptor's memory except Tx ring which allocated eariler
		//
		pAdapter->MgmtDescRing.AllocSize = MGMT_RING_SIZE * TXD_SIZE;
		pAdapter->MgmtDescRing.AllocVa
           	= pci_alloc_consistent(pAdapter->pPci_Dev, pAdapter->MgmtDescRing.AllocSize, &pAdapter->MgmtDescRing.AllocPa);

		if (pAdapter->MgmtDescRing.AllocVa == NULL)
		{
			DBGPRINT_ERR("Failed to allocate a big buffer\n");
			break;
		}

		// Zero init this memory block
		NdisZeroMemory(pAdapter->MgmtDescRing.AllocVa, pAdapter->MgmtDescRing.AllocSize);

		// Save PA & VA for further operation
		RingBasePa     = pAdapter->MgmtDescRing.AllocPa;
		RingBaseVa     = pAdapter->MgmtDescRing.AllocVa;

		//
		// Initialize MGMT Ring and associated buffer memory
		//
		for (index = 0; index < MGMT_RING_SIZE; index++)
		{
			// Init MGMT Ring Size, Va, Pa variables
			pAdapter->MgmtRing.Cell[index].AllocSize = TXD_SIZE;
			pAdapter->MgmtRing.Cell[index].AllocVa = RingBaseVa;
			pAdapter->MgmtRing.Cell[index].AllocPa = RingBasePa;

			pTxD = (PTXD_STRUC) pAdapter->MgmtRing.Cell[index].AllocVa;
#ifdef BIG_ENDIAN
			RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
#endif
            // Offset to next ring descriptor address
            RingBasePa += TXD_SIZE;
            RingBaseVa = (PUCHAR) RingBaseVa + TXD_SIZE;

            // no pre-allocated buffer required in MgmtRing for scatter-gather case
		}
        DBGPRINT(RT_DEBUG_TRACE, "MGMT Ring: total %d entry allocated\n", index);

		//
		// Allocate RX ring descriptor's memory except Tx ring which allocated eariler
		//
		pAdapter->RxDescRing.AllocSize = RX_RING_SIZE * RXD_SIZE;
		pAdapter->RxDescRing.AllocVa
           	= pci_alloc_consistent(pAdapter->pPci_Dev, pAdapter->RxDescRing.AllocSize, &pAdapter->RxDescRing.AllocPa);

		if (pAdapter->RxDescRing.AllocVa == NULL)
		{
			DBGPRINT_ERR("Failed to allocate a big buffer\n");
			break;
		}

		// Zero init this memory block
		NdisZeroMemory(pAdapter->RxDescRing.AllocVa, pAdapter->RxDescRing.AllocSize);

		// Save PA & VA for further operation
		RingBasePa     = pAdapter->RxDescRing.AllocPa;
		RingBaseVa     = pAdapter->RxDescRing.AllocVa;

		//
		// Initialize Rx Ring and associated buffer memory
		//
		for (index = 0; index < RX_RING_SIZE; index++)
		{
			// Init RX Ring Size, Va, Pa variables
			pAdapter->RxRing.Cell[index].AllocSize = RXD_SIZE;
			pAdapter->RxRing.Cell[index].AllocVa = RingBaseVa;
			pAdapter->RxRing.Cell[index].AllocPa = RingBasePa;

            // Offset to next ring descriptor address
            RingBasePa += RXD_SIZE;
            RingBaseVa = (PUCHAR) RingBaseVa + RXD_SIZE;

			// Setup Rx associated Buffer size & allocate share memory
			pDmaBuf = &pAdapter->RxRing.Cell[index].DmaBuf;
			pDmaBuf->AllocSize = RX_DMA_BUFFER_SIZE;
			
			pSkb = __dev_alloc_skb(pDmaBuf->AllocSize, MEM_ALLOC_FLAG);
			RTMP_SET_PACKET_SOURCE(pSkb, PKTSRC_DRIVER);
			pDmaBuf->pSkb    = pSkb;
			pDmaBuf->AllocVa = pSkb->data;
			pDmaBuf->AllocPa = pci_map_single(pAdapter->pPci_Dev, (PVOID)pSkb->data, pDmaBuf->AllocSize, PCI_DMA_FROMDEVICE);

			// Error handling
			if (pDmaBuf->AllocVa == NULL)
			{
				DBGPRINT_ERR("Failed to allocate RxRing's 1st buffer\n");
				break;
			}

			// Zero init this memory block
			NdisZeroMemory(pDmaBuf->AllocVa, pDmaBuf->AllocSize);
            
			// Write RxD buffer address & allocated buffer length
			pRxD = (PRXD_STRUC) pAdapter->RxRing.Cell[index].AllocVa;
			pRxD->BufPhyAddr = pDmaBuf->AllocPa;

			// Rx owner bit assign to NIC immediately
			pRxD->Owner = DESC_OWN_NIC;

#ifdef BIG_ENDIAN
			RTMPDescriptorEndianChange((PUCHAR)pRxD, TYPE_RXD);
#endif			
		}
        DBGPRINT(RT_DEBUG_TRACE, "Rx Ring: total %d entry allocated\n", index);

	}	while (FALSE);

	DBGPRINT(RT_DEBUG_TRACE, "<-- RTMPAllocDMAMemory\n");

    return 0;
}

/*
    ========================================================================

    Routine Description:
        Free all DMA memory.

    Arguments:
        Adapter         Pointer to our adapter

    Return Value:
        None

    Note:

    ========================================================================
*/
VOID    RTMPFreeDMAMemory(
    IN  PRTMP_ADAPTER   pAdapter)
{
	INT		    index, num;

	DBGPRINT(RT_DEBUG_TRACE, "--> RTMPFreeDMAMemory\n");

	//
	// Free RX Ring related space
	//
	for (index = RX_RING_SIZE - 1 ; index >= 0; index--)
	{
		if ((pAdapter->RxRing.Cell[index].DmaBuf.AllocVa) && (pAdapter->RxRing.Cell[index].DmaBuf.pSkb))
		{
			pci_unmap_single(pAdapter->pPci_Dev, pAdapter->RxRing.Cell[index].DmaBuf.AllocPa, pAdapter->RxRing.Cell[index].DmaBuf.AllocSize, PCI_DMA_FROMDEVICE);
			RELEASE_NDIS_PACKET(pAdapter, pAdapter->RxRing.Cell[index].DmaBuf.pSkb);
		}
	}
	NdisZeroMemory(pAdapter->RxRing.Cell, RX_RING_SIZE * sizeof(RTMP_DMACB));

    if (pAdapter->RxDescRing.AllocVa)
    {
    	pci_free_consistent(pAdapter->pPci_Dev, pAdapter->RxDescRing.AllocSize,
			pAdapter->RxDescRing.AllocVa, pAdapter->RxDescRing.AllocPa);
    }
    NdisZeroMemory(&pAdapter->RxDescRing, sizeof(RTMP_DMABUF));

	//
	// Free MGMT Ring related space
	//
	if (pAdapter->MgmtDescRing.AllocVa)
	{
		pci_free_consistent(pAdapter->pPci_Dev, pAdapter->MgmtDescRing.AllocSize,
			pAdapter->MgmtDescRing.AllocVa, pAdapter->MgmtDescRing.AllocPa);
	}
	NdisZeroMemory(&pAdapter->MgmtDescRing, sizeof(RTMP_DMABUF));

	//
	// Free TX Ring buffer
	//
	for (num = 0; num < NUM_OF_TX_RING; num++)
	{
    	if (pAdapter->TxBufSpace[num].AllocVa)
	    {
	    	pci_free_consistent(pAdapter->pPci_Dev, pAdapter->TxBufSpace[num].AllocSize,
				pAdapter->TxBufSpace[num].AllocVa, pAdapter->TxBufSpace[num].AllocPa);
	    }
	    NdisZeroMemory(&pAdapter->TxBufSpace[num], sizeof(RTMP_DMABUF));
	    
    	if (pAdapter->TxDescRing[num].AllocVa)
	    {
	    	pci_free_consistent(pAdapter->pPci_Dev, pAdapter->TxDescRing[num].AllocSize,
				pAdapter->TxDescRing[num].AllocVa, pAdapter->TxDescRing[num].AllocPa);
	    }
	    NdisZeroMemory(&pAdapter->TxDescRing[num], sizeof(RTMP_DMABUF));
	}

	//
	// Free preallocated shared memory
	//
    NdisFreeSpinLock(&pAdapter->TxSwQueueLock);
	NdisFreeSpinLock(&pAdapter->RxRingLock);
	NdisFreeSpinLock(&pAdapter->MgmtRingLock);
	NdisFreeSpinLock(&pAdapter->TxRingLock);

	DBGPRINT(RT_DEBUG_TRACE, "<-- RTMPFreeDMAMemory\n");
}

inline  VOID    NICDisableInterrupt(
    IN  PRTMP_ADAPTER   pAdapter)
{
    RTMP_IO_WRITE32(pAdapter, INT_MASK_CSR, 0xffffff7f);     // 0xffffff7f
    RTMP_IO_WRITE32(pAdapter, MCU_INT_MASK_CSR, 0xffffffff);
    RTMP_CLEAR_FLAG(pAdapter, fRTMP_ADAPTER_INTERRUPT_ACTIVE);
}

inline  VOID    NICEnableInterrupt(
    IN  PRTMP_ADAPTER   pAdapter,
    IN	UINT32 			int_mask)
{
#ifdef THREAD_ISR
	/* 
	 * mask out pendding interrupt bit 
     * interrupt aggregation : 0x0000fe90
	 */
    RTMP_IO_WRITE32(pAdapter, INT_MASK_CSR, 0x0000fe90 | int_mask);
#else 
    RTMP_IO_WRITE32(pAdapter, INT_MASK_CSR, 0x0000ff10 | int_mask);     // 0x0000fe90(interrupt aggregation, can be test in embedded platform while bad throughput)
#endif
    RTMP_IO_WRITE32(pAdapter, MCU_INT_MASK_CSR, 0x00000200); // 0x000001ff
    RTMP_SET_FLAG(pAdapter, fRTMP_ADAPTER_INTERRUPT_ACTIVE);
}

/*
	========================================================================
	
	Routine Description:
		Initialize transmit data structures

	Arguments:
		Adapter						Pointer to our adapter

	Return Value:
		None

	Note:
		Initialize all transmit releated private buffer, include those define
		in RTMP_ADAPTER structure and all private data structures.
		
	========================================================================
*/
VOID	NICInitTxRxRingAndBacklogQueue(
	IN	PRTMP_ADAPTER	pAdapter)
{
	DBGPRINT(RT_DEBUG_TRACE, "<--> NICInitTxRxRingAndBacklogQueue\n");

    // Initialize all transmit related software queues
    InitializeQueueHeader(&pAdapter->TxSwQueue[QID_AC_BE]);
    InitializeQueueHeader(&pAdapter->TxSwQueue[QID_AC_BK]);
    InitializeQueueHeader(&pAdapter->TxSwQueue[QID_AC_VI]);
    InitializeQueueHeader(&pAdapter->TxSwQueue[QID_AC_VO]);
    InitializeQueueHeader(&pAdapter->TxSwQueue[QID_HCCA]);
    
    // Initialize the NULL-Frame transmit related software queues
    InitializeQueueHeader(&pAdapter->NullFrameTxSwQueue);

	// Init RX Ring index pointer
	pAdapter->RxRing.CurRxIndex           = 0;
	
    // Init TX rings index pointer
    {
        INT i;
        for (i=0; i<NUM_OF_TX_RING; i++)
        {
	        pAdapter->TxRing[i].CurTxIndex         = 0;
            pAdapter->TxRing[i].NextTxDmaDoneIndex = 0;
        }
    }

    // init MGMT ring index pointer
    pAdapter->MgmtRing.CurTxIndex              = 0;
    pAdapter->MgmtRing.NextTxDmaDoneIndex      = 0;
        
	pAdapter->PrivateInfo.TxRingFullCnt       = 0;
}

/*
    ========================================================================

    Routine Description:
        Read initial parameters from EEPROM

    Arguments:
        Adapter                     Pointer to our adapter

    Return Value:
        None

    Note:
        
    ========================================================================
*/
VOID    NICReadEEPROMParameters(
    IN  PRTMP_ADAPTER   pAd)
{
	ULONG 			data;
	USHORT			i, value, value2;
	EEPROM_TX_PWR_STRUC	    Power;
    EEPROM_VERSION_STRUC    Version;
	EEPROM_ANTENNA_STRUC	Antenna;
	EEPROM_LED_STRUC		LedSetting;
	UCHAR			AModeGainValue = 0;

	DBGPRINT(RT_DEBUG_TRACE, "--> NICReadEEPROMParameters\n");

	// Init EEPROM Address Number, before access EEPROM; if 93c46, EEPROMAddressNum=6, else if 93c66, EEPROMAddressNum=8
	RTMP_IO_READ32(pAd, E2PROM_CSR, &data);

	if(data & 0x20)
		pAd->EEPROMAddressNum = 6;		// 93C46
	else
		pAd->EEPROMAddressNum = 8;     // 93C66

    // RT2660 MAC no longer auto load MAC address from E2PROM. Driver has to intialize
    // MAC address registers according to E2PROM setting
    {
    	/*
            krammer change for bug 957,make rt61ap get the mac addr from flash
        */
        
        MAC_CSR2_STRUC csr2;
        MAC_CSR3_STRUC csr3;
	#ifdef TC_SOC
   	/* frankliao modify 20101216 */
	//  uint8 *flash_mac_addr = (uint8 *)0xbfc0ff48;
	char flash_mac_addr[6];

	for (i=0; i<6; i++) {
		flash_mac_addr[i] = READ_FLASH_BYTE(flash_base + 0xff48 + i);
	}


	if( (flash_mac_addr[0] == 0) && (flash_mac_addr[1] == 0) && (flash_mac_addr[2] == 0) &&
	    (flash_mac_addr[3] == 0) && (flash_mac_addr[4] == 0) && (flash_mac_addr[5] == 0) )
		printk(KERN_INFO "The MAC address in flash is null!\n");	    
	else    
  		memcpy(pAd->CurrentAddress, flash_mac_addr, 6);

	#else
	 USHORT  Addr01,Addr23,Addr45 ;

        Addr01=RTMP_EEPROM_READ16(pAd, 0x04);
        Addr23=RTMP_EEPROM_READ16(pAd, 0x06);
        Addr45=RTMP_EEPROM_READ16(pAd, 0x08);

        pAd->CurrentAddress[0] = (UCHAR)(Addr01 & 0xff);
        pAd->CurrentAddress[1] = (UCHAR)(Addr01 >> 8);
        pAd->CurrentAddress[2] = (UCHAR)(Addr23 & 0xff);
        pAd->CurrentAddress[3] = (UCHAR)(Addr23 >> 8);
        pAd->CurrentAddress[4] = (UCHAR)(Addr45 & 0xff);
        pAd->CurrentAddress[5] = (UCHAR)(Addr45 >> 8);
	#endif

        csr2.field.Byte0 = pAd->CurrentAddress[0];
        csr2.field.Byte1 = pAd->CurrentAddress[1];
        csr2.field.Byte2 = pAd->CurrentAddress[2];
        csr2.field.Byte3 = pAd->CurrentAddress[3];
        RTMP_IO_WRITE32(pAd, MAC_CSR2, csr2.word);
        csr3.word = 0;
        csr3.field.Byte4 = pAd->CurrentAddress[4];
        csr3.field.Byte5 = pAd->CurrentAddress[5];
        csr3.field.U2MeMask = 0xff;
        RTMP_IO_WRITE32(pAd, MAC_CSR3, csr3.word);

        // Prepare MBSSID's mac addr, the mac addr will be the E2PROM's mac addr plus 0,1,2,3 as MBSSID[0],MBSSID[1],MBSSID[2],MBSSID[3]'s mac addr
        for(i = 0; i < MAX_MBSSID_NUM; i++)
        {
        	BOOLEAN bCarry = FALSE;
        	INT		j;
        	
        	for (j = (MAC_ADDR_LEN-1); j >= 0; j--)
        	{
        		if (j == (MAC_ADDR_LEN-1))
        		{
	        		if (((pAd->CurrentAddress[j]+i) & 0xFF) < pAd->CurrentAddress[j])
	        			bCarry = TRUE;
	        		else
	        			bCarry = FALSE;
	        		pAd->PortCfg.MBSSID[i].Bssid[j] = (pAd->CurrentAddress[j]+i) & 0xFF;
        		}
        		else
        		{
        			if (bCarry == TRUE)
        			{
        				if (((pAd->CurrentAddress[j]+1) & 0xFF) < pAd->CurrentAddress[j])
		        			bCarry = TRUE;
		        		else
		        			bCarry = FALSE;
		        		pAd->PortCfg.MBSSID[i].Bssid[j] = (pAd->CurrentAddress[j]+1) & 0xFF;
        			}
        			else
        				pAd->PortCfg.MBSSID[i].Bssid[j] = pAd->CurrentAddress[j];
        		}
        	}

        	DBGPRINT(RT_DEBUG_TRACE,"MBSSID[%d] MAC=%02x:%02x:%02x:%02x:%02x:%02x\n", i,
	            pAd->PortCfg.MBSSID[i].Bssid[0], pAd->PortCfg.MBSSID[i].Bssid[1],
	            pAd->PortCfg.MBSSID[i].Bssid[2], pAd->PortCfg.MBSSID[i].Bssid[3],
	            pAd->PortCfg.MBSSID[i].Bssid[4], pAd->PortCfg.MBSSID[i].Bssid[5]);
        }
    }   

    // Init the channel number for TX channel power
    NdisZeroMemory(pAd->TxPower, sizeof(pAd->TxPower));
    // 0. 11b/g
    for (i = 0; i < 14; i++)
        pAd->TxPower[i].Channel = i + 1;
    // 1. UNI 36 - 64
    for (i = 0; i < 8; i++)
        pAd->TxPower[i + 14].Channel = 36 + i * 4;
    // 2. HipperLAN 2 100 - 140
    for (i = 0; i < 11; i++)
        pAd->TxPower[i + 22].Channel = 100 + i * 4;
    // 3. UNI 140 - 165
    for (i = 0; i < 5; i++)
        pAd->TxPower[i + 33].Channel = 149 + i * 4;        
    for (i = 0; i < 4; i++)	// EYL: add 20070801, to support channel 34, 38, 42, 46
        pAd->TxPower[i + 38].Channel = 34 + i * 4;

    // if E2PROM version mismatch with driver's expectation, then skip
    // all subsequent E2RPOM retieval and set a system error bit to notify GUI
    Version.word = RTMP_EEPROM_READ16(pAd, EEPROM_VERSION_OFFSET);
    pAd->EepromVersion = Version.field.Version + Version.field.FaeReleaseNumber * 256;
    DBGPRINT(RT_DEBUG_TRACE, "E2PROM: Version = %d, FAE release #%d\n", Version.field.Version, Version.field.FaeReleaseNumber);
    
    if (Version.field.Version > VALID_EEPROM_VERSION)
    {
        DBGPRINT_ERR("E2PROM: WRONG VERSION %d, should be %d\n",Version.field.Version, VALID_EEPROM_VERSION);

        // hard-code default value when no proper E2PROM installed
        pAd->bAutoTxAgcA = FALSE;
        pAd->bAutoTxAgcG = FALSE;
        
        // Default the channel power
        for (i = 0; i < MAX_NUM_OF_CHANNELS; i++)
            pAd->TxPower[i].Power = DEFAULT_RF_TX_POWER;
    
        pAd->RfIcType = RFIC_5225;
        for(i = 0; i < NUM_EEPROM_BBP_PARMS; i++)
            pAd->EEPROMDefaultValue[i] = 0xffff;
        return;
    }

	// Read BBP default value from EEPROM and store to array(EEPROMDefaultValue) in pAd
	for(i = 0; i < NUM_EEPROM_BBP_PARMS; i++)
	{
		value = RTMP_EEPROM_READ16(pAd, EEPROM_BBP_BASE_OFFSET + i*2);
		
		pAd->EEPROMDefaultValue[i] = value;
	}

	// We have to parse NIC configuration 0 at here.
	// If TSSI did not have preloaded value, it should reset the TxAutoAgc to false
	// Therefore, we have to read TxAutoAgc control beforehand.
	// Read Tx AGC control bit
	Antenna.word = pAd->EEPROMDefaultValue[0];
	if (Antenna.field.DynamicTxAgcControl == 1)
		pAd->bAutoTxAgcA = pAd->bAutoTxAgcG = TRUE;
	else
		pAd->bAutoTxAgcA = pAd->bAutoTxAgcG = FALSE;		
	
	//
	// Reset PhyMode if we don't support 802.11a
	//
	if ((pAd->PortCfg.PhyMode == PHY_11ABG_MIXED) || (pAd->PortCfg.PhyMode == PHY_11A))
	{
		//
		// Only RFIC_5225 & RFIC_5325 support 802.11a
		//
		if ((Antenna.field.RfIcType != RFIC_5225) && (Antenna.field.RfIcType != RFIC_5325))
			pAd->PortCfg.PhyMode = PHY_11BG_MIXED;
	}
	
	// Read Tx power value for all channels
	// Value from 1 - 0x7f. Default value is 24.
	// 0. 11b/g
	// Power value 0xFA (-6) ~ 0x24 (36)
	for (i = 0; i < 7; i++)
	{
		Power.word = RTMP_EEPROM_READ16(pAd, EEPROM_G_TX_PWR_OFFSET + i * 2);
		
		if ((Power.field.Byte0 > 36) || (Power.field.Byte0 < -6))
			pAd->TxPower[i * 2].Power = DEFAULT_RF_TX_POWER;
		else
			pAd->TxPower[i * 2].Power = Power.field.Byte0;

		if ((Power.field.Byte1 > 36) || (Power.field.Byte1 < -6))
			pAd->TxPower[i * 2 + 1].Power = DEFAULT_RF_TX_POWER;
		else
			pAd->TxPower[i * 2 + 1].Power = Power.field.Byte1;
	}
	// 1. UNI 36 - 64, HipperLAN 2 100 - 140, UNI 140 - 165
	for (i = 0; i < 12; i++)
	{
		Power.word = RTMP_EEPROM_READ16(pAd, EEPROM_A_TX_PWR_OFFSET + i * 2);
		
		if ((Power.field.Byte0 > 36) || (Power.field.Byte0 < -6))
			pAd->TxPower[i * 2 + 14].Power = DEFAULT_RF_TX_POWER;
		else
			pAd->TxPower[i * 2 + 14].Power = Power.field.Byte0;

		if ((Power.field.Byte1 > 36) || (Power.field.Byte1 < -6))
			pAd->TxPower[i * 2 + 15].Power = DEFAULT_RF_TX_POWER;
		else
			pAd->TxPower[i * 2 + 15].Power = Power.field.Byte1;
	}
	
	// EYL: add 20070801, to support channel 34, 38, 42, 46
	// Channel 34
	Power.word = RTMP_EEPROM_READ16(pAd, 0x7C);
	if ((Power.field.Byte1 > 36) || (Power.field.Byte1 < -6))
		pAd->TxPower[38].Power = DEFAULT_RF_TX_POWER;
	else
		pAd->TxPower[38].Power = Power.field.Byte1;

	// Channel 38, 42
	Power.word = RTMP_EEPROM_READ16(pAd, 0x7E);
	if ((Power.field.Byte0 > 36) || (Power.field.Byte0 < -6))
		pAd->TxPower[39].Power = DEFAULT_RF_TX_POWER;
	else
		pAd->TxPower[39].Power = Power.field.Byte0;
	if ((Power.field.Byte1 > 36) || (Power.field.Byte1 < -6))
		pAd->TxPower[40].Power = DEFAULT_RF_TX_POWER;
	else
		pAd->TxPower[40].Power = Power.field.Byte1;

	// Channel 46
	Power.word = RTMP_EEPROM_READ16(pAd, 0x80);
	if ((Power.field.Byte0 > 36) || (Power.field.Byte0 < -6))
		pAd->TxPower[41].Power = DEFAULT_RF_TX_POWER;
	else
		pAd->TxPower[41].Power = Power.field.Byte0;
	// EYL: add end

	// Read TSSI reference and TSSI boundary for temperature compensation. This is ugly
	// 0. 11b/g
	{
		Power.word = RTMP_EEPROM_READ16(pAd, 0x54);
		pAd->TssiMinusBoundaryG[4] = Power.field.Byte0;
		pAd->TssiMinusBoundaryG[3] = Power.field.Byte1;
		Power.word = RTMP_EEPROM_READ16(pAd, 0x56);
		pAd->TssiMinusBoundaryG[2] = Power.field.Byte0;
		pAd->TssiMinusBoundaryG[1] = Power.field.Byte1;
		Power.word = RTMP_EEPROM_READ16(pAd, 0x58);
		pAd->TssiPlusBoundaryG[1] = Power.field.Byte0;
		pAd->TssiPlusBoundaryG[2] = Power.field.Byte1;
		Power.word = RTMP_EEPROM_READ16(pAd, 0x5a);
		pAd->TssiPlusBoundaryG[3] = Power.field.Byte0;
		pAd->TssiPlusBoundaryG[4] = Power.field.Byte1;
        Power.word = RTMP_EEPROM_READ16(pAd, 0x5c);
        pAd->TssiRefG   = Power.field.Byte0;
        pAd->TxAgcStepG = Power.field.Byte1;    
        pAd->TxAgcCompensateG = 0;
        pAd->TssiMinusBoundaryG[0] = pAd->TssiRefG;
        pAd->TssiPlusBoundaryG[0]  = pAd->TssiRefG;

		// Disable TxAgc if the based value is not right
		if (pAd->TssiRefG == 0xff)
			pAd->bAutoTxAgcG = FALSE;

        DBGPRINT(RT_DEBUG_TRACE,"E2PROM: G Tssi[-4 .. +4] = %d %d %d %d - %d -%d %d %d %d, step=%d, tuning=%d\n",
            pAd->TssiMinusBoundaryG[4], pAd->TssiMinusBoundaryG[3], pAd->TssiMinusBoundaryG[2], pAd->TssiMinusBoundaryG[1],
            pAd->TssiRefG,
            pAd->TssiPlusBoundaryG[1], pAd->TssiPlusBoundaryG[2], pAd->TssiPlusBoundaryG[3], pAd->TssiPlusBoundaryG[4],
            pAd->TxAgcStepG, pAd->bAutoTxAgcG);
	}	
	// 1. 11a
	{
		Power.word = RTMP_EEPROM_READ16(pAd, 0x90);
		pAd->TssiMinusBoundaryA[4] = Power.field.Byte0;
		pAd->TssiMinusBoundaryA[3] = Power.field.Byte1;
		Power.word = RTMP_EEPROM_READ16(pAd, 0x92);
		pAd->TssiMinusBoundaryA[2] = Power.field.Byte0;
		pAd->TssiMinusBoundaryA[1] = Power.field.Byte1;
		Power.word = RTMP_EEPROM_READ16(pAd, 0x94);
		pAd->TssiPlusBoundaryA[1] = Power.field.Byte0;
		pAd->TssiPlusBoundaryA[2] = Power.field.Byte1;
		Power.word = RTMP_EEPROM_READ16(pAd, 0x96);
		pAd->TssiPlusBoundaryA[3] = Power.field.Byte0;
		pAd->TssiPlusBoundaryA[4] = Power.field.Byte1;
        Power.word = RTMP_EEPROM_READ16(pAd, 0x98);
        pAd->TssiRefA   = Power.field.Byte0;
        pAd->TxAgcStepA = Power.field.Byte1;    
        pAd->TxAgcCompensateA = 0;
        pAd->TssiMinusBoundaryA[0] = pAd->TssiRefA;
        pAd->TssiPlusBoundaryA[0]  = pAd->TssiRefA;

		// Disable TxAgc if the based value is not right
		if (pAd->TssiRefA == 0xff)
			pAd->bAutoTxAgcA = FALSE;

        DBGPRINT(RT_DEBUG_TRACE,"E2PROM: A Tssi[-4 .. +4] = %d %d %d %d - %d -%d %d %d %d, step=%d, tuning=%d\n",
            pAd->TssiMinusBoundaryA[4], pAd->TssiMinusBoundaryA[3], pAd->TssiMinusBoundaryA[2], pAd->TssiMinusBoundaryA[1],
            pAd->TssiRefA,
            pAd->TssiPlusBoundaryA[1], pAd->TssiPlusBoundaryA[2], pAd->TssiPlusBoundaryA[3], pAd->TssiPlusBoundaryA[4],
            pAd->TxAgcStepA, pAd->bAutoTxAgcA);
	}	
    pAd->BbpRssiToDbmDelta = 0x79;
	
	// Read frequency offset and RF programming sequence setting for RT5225
    value = RTMP_EEPROM_READ16(pAd, EEPROM_FREQ_OFFSET);
	if ((value & 0xFF00) == 0xFF00)
	{
		pAd->RFProgSeq = 0;
	}
	else
	{
		pAd->RFProgSeq = (value & 0x0300) >> 8;	// bit 8,9
	}
	
	value &= 0x00FF;
	if (value != 0x00FF)
		pAd->RfFreqOffset = value;
	else
		pAd->RfFreqOffset = 0;
	DBGPRINT(RT_DEBUG_TRACE, "E2PROM: RF freq offset=0x%x, RF programming seq=%d\n", pAd->RfFreqOffset,pAd->RFProgSeq);

	//CountryRegion byte offset = 0x25
	value = pAd->EEPROMDefaultValue[2] >> 8;
	value2 = pAd->EEPROMDefaultValue[2] & 0x00FF;
	if ((value <= 7) && (value2 <= 10))
	{
		pAd->PortCfg.CountryRegion = ((UCHAR) value) | 0x80;
		pAd->PortCfg.CountryRegionForABand = ((UCHAR) value2) | 0x80;
	}

		//
	// Get RSSI Offset on EEPROM 0x9Ah & 0x9Ch.
	// The valid value are (-10 ~ 10) 
	// 
	value = RTMP_EEPROM_READ16(pAd, EEPROM_RSSI_BG_OFFSET);
	pAd->BGRssiOffset1 = value & 0x00ff;
	pAd->BGRssiOffset2 = (value >> 8);

	// Validate 11b/g RSSI_1 offset.
	if ((pAd->BGRssiOffset1 < -10) || (pAd->BGRssiOffset1 > 10))
		pAd->BGRssiOffset1 = 0;

	// Validate 11b/g RSSI_2 offset.
	if ((pAd->BGRssiOffset2 < -10) || (pAd->BGRssiOffset2 > 10))
		pAd->BGRssiOffset2 = 0;
		
	value = RTMP_EEPROM_READ16(pAd, EEPROM_RSSI_A_OFFSET);
	pAd->ARssiOffset1 = value & 0x00ff;
	pAd->ARssiOffset2 = (value >> 8);

	// Validate 11a RSSI_1 offset.
	if ((pAd->ARssiOffset1 < -10) || (pAd->ARssiOffset1 > 10))
		pAd->ARssiOffset1 = 0;

	//Validate 11a RSSI_2 offset.
	if ((pAd->ARssiOffset2 < -10) || (pAd->ARssiOffset2 > 10))
		pAd->ARssiOffset2 = 0;

	//
	// Get LED Setting.
	//
	LedSetting.word = RTMP_EEPROM_READ16(pAd, EEPROM_LED_OFFSET);
	if (LedSetting.word == 0xFFFF)
	{
		//
		// Set it to Default.
		//
		LedSetting.field.PolarityRDY_G = 1;   // Active High.
		LedSetting.field.PolarityRDY_A = 1;   // Active High.
		LedSetting.field.PolarityACT = 1;    // Active High.
		LedSetting.field.PolarityGPIO_0 = 1; // Active High.
		LedSetting.field.PolarityGPIO_1 = 1; // Active High.
		LedSetting.field.PolarityGPIO_2 = 1; // Active High.
		LedSetting.field.PolarityGPIO_3 = 1; // Active High.
		LedSetting.field.PolarityGPIO_4 = 1; // Active High.
		LedSetting.field.STALedMode = LED_MODE_DEFAULT;
		LedSetting.field.APLedMode = LED_MODE_DEFAULT;
	}
	pAd->LedCntl.word = 0;
	pAd->LedCntl.field.LedMode = LedSetting.field.APLedMode | 0x18;	// Offset 0x18 to let firmware know AP or STA
	pAd->LedCntl.field.PolarityRDY_G = LedSetting.field.PolarityRDY_G;
	pAd->LedCntl.field.PolarityRDY_A = LedSetting.field.PolarityRDY_A;
	pAd->LedCntl.field.PolarityACT = LedSetting.field.PolarityACT;
	pAd->LedCntl.field.PolarityGPIO_0 = LedSetting.field.PolarityGPIO_0;
	pAd->LedCntl.field.PolarityGPIO_1 = LedSetting.field.PolarityGPIO_1;
	pAd->LedCntl.field.PolarityGPIO_2 = LedSetting.field.PolarityGPIO_2;
	pAd->LedCntl.field.PolarityGPIO_3 = LedSetting.field.PolarityGPIO_3;
	pAd->LedCntl.field.PolarityGPIO_4 = LedSetting.field.PolarityGPIO_4;

	// 
	value = RTMP_EEPROM_READ16(pAd, EEPROM_TX_POWER_DELTA);
	if ((value & 0x00FF) == 0xFF)
	{
		pAd->TxPowerDelta.word = 0;
//		RTMP_IO_WRITE32(pAd, TXRX_CSR3, 0x00858687);
	}
	else
	{
		pAd->TxPowerDelta.word = value;
//		RTMP_IO_WRITE32(pAd, TXRX_CSR3, 0xde858687);// Per-Packet per txpower
	}

	AModeGainValue = (value >> 8);
	if (AModeGainValue == 0xff)
	{
		pAd->AModeGainValue = 0x08;  //Default value set to 0x08
	}
	else
	{
		pAd->AModeGainValue = AModeGainValue;
	}

	DBGPRINT(RT_DEBUG_TRACE, "TxPowerDelta Config (Delta=%d, Sign=%d, Enable=%d)\n",
		pAd->TxPowerDelta.field.Delta, pAd->TxPowerDelta.field.Sign, pAd->TxPowerDelta.field.Enable);
	
	DBGPRINT(RT_DEBUG_TRACE, "<-- NICReadEEPROMParameters\n");
}

/*
    ========================================================================

    Routine Description:
        Set default value from EEPROM

    Arguments:
        Adapter                     Pointer to our adapter

    Return Value:
        None

    Note:

    ========================================================================
*/
VOID    NICInitAsicFromEEPROM(
    IN  PRTMP_ADAPTER   pAd)
{
	USHORT					i;
	EEPROM_ANTENNA_STRUC	Antenna;
	EEPROM_NIC_CONFIG2_STRUC    NicConfig2;

	DBGPRINT(RT_DEBUG_TRACE, "--> NICInitAsicFromEEPROM\n");

   	for(i = 3; i < NUM_EEPROM_BBP_PARMS; i++)
    {
        UCHAR BbpRegIdx, BbpValue;
	
   		if ((pAd->EEPROMDefaultValue[i] != 0xFFFF) && (pAd->EEPROMDefaultValue[i] != 0))
    	{
    	    BbpRegIdx = (UCHAR)(pAd->EEPROMDefaultValue[i] >> 8);
    	    BbpValue  = (UCHAR)(pAd->EEPROMDefaultValue[i] & 0xff);
    	    RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BbpRegIdx, BbpValue);
   		}
    }
    
	Antenna.word = pAd->EEPROMDefaultValue[0];

	if (Antenna.word == 0xFFFF)
	{
	    Antenna.word = 0;
        Antenna.field.RfIcType = RFIC_5225;
        Antenna.field.HardwareRadioControl = 0;     // no hardware control
        Antenna.field.DynamicTxAgcControl = 0;
        Antenna.field.RxDefaultAntenna = 2;         // Ant-B
        Antenna.field.TxDefaultAntenna = 2;         // Ant-B
        Antenna.field.NumOfAntenna = 2;
        DBGPRINT(RT_DEBUG_TRACE, "E2PROM, Antenna parameter error, hard code as 0x%04x\n", Antenna.word);
	}

    pAd->RfIcType = (UCHAR) Antenna.field.RfIcType;

	// recovery invalid settings in EEPROM
	if (pAd->DefaultTxAntenna != (char)-1)
	{
		Antenna.field.RxDefaultAntenna = pAd->DefaultTxAntenna;
        Antenna.field.TxDefaultAntenna = pAd->DefaultTxAntenna;
	}
	
	// Save the antenna for future use
	pAd->Antenna.word = Antenna.word;
	
	NicConfig2.word = pAd->EEPROMDefaultValue[1];
	if (NicConfig2.word == 0xffff)
	{
		NicConfig2.word = 0;
	}
	// Save the antenna for future use
	pAd->NicConfig2.word = NicConfig2.word;

	// external LNA For 5G has different R17 base
	if (pAd->NicConfig2.field.ExternalLNAForA)
	{
		pAd->BbpTuning.R17LowerBoundA += pAd->AModeGainValue;
		pAd->BbpTuning.R17UpperBoundA += pAd->AModeGainValue;
	}
	// external LNA For 2.4G has different R17 base
	if (pAd->NicConfig2.field.ExternalLNAForG)
	{
	    pAd->BbpTuning.R17LowerBoundG += 0x10;
	    pAd->BbpTuning.R17UpperBoundG += 0x10;
	}

   	DBGPRINT(RT_DEBUG_TRACE, "RFIC=%d, LED mode=%d\n", pAd->RfIcType, pAd->LedCntl.field.LedMode);

	DBGPRINT(RT_DEBUG_TRACE, "<-- NICInitAsicFromEEPROM\n");
}

NDIS_STATUS NICInitializeAdapter(
	IN PRTMP_ADAPTER   pAdapter)
{
	TX_RING_CSR0_STRUC TxCsr0;
	TX_RING_CSR1_STRUC TxCsr1;
	RX_RING_CSR_STRUC  RxCsr;
	UMAXLONG		Value;
    NDIS_STATUS     Status = NDIS_STATUS_SUCCESS;

	DBGPRINT(RT_DEBUG_TRACE, "--> NICInitializeAdapter\n");

    //
    // write all shared Ring's base address into ASIC
    //
    
    // Write AC_BK base address register
    Value = pAdapter->TxRing[QID_AC_BK].Cell[0].AllocPa;
    RTMP_IO_WRITE32(pAdapter, AC1_BASE_CSR, Value);
            
    // Write AC_BE base address register
    Value = pAdapter->TxRing[QID_AC_BE].Cell[0].AllocPa;
    RTMP_IO_WRITE32(pAdapter, AC0_BASE_CSR, Value);
            
    // Write AC_VI base address register
    Value = pAdapter->TxRing[QID_AC_VI].Cell[0].AllocPa;
    RTMP_IO_WRITE32(pAdapter, AC2_BASE_CSR, Value);
            
    // Write AC_VO base address register
    Value = pAdapter->TxRing[QID_AC_VO].Cell[0].AllocPa;
    RTMP_IO_WRITE32(pAdapter, AC3_BASE_CSR, Value);
            
    // Write HCCA base address register
    //  Value = NdisGetPhysicalAddressLow(pAdapter->TxRing[QID_HCCA].Cell[0].AllocPa);
    //  RTMP_IO_WRITE32(pAdapter, HCCA_BASE_CSR, Value);
            
    // Write MGMT_BASE_CSR register
    Value = pAdapter->MgmtRing.Cell[0].AllocPa;
    RTMP_IO_WRITE32(pAdapter, MGMT_BASE_CSR, Value);
            
    // Write RX_BASE_CSR register
    Value = pAdapter->RxRing.Cell[0].AllocPa;
    RTMP_IO_WRITE32(pAdapter, RX_BASE_CSR, Value);

    //
    // set each Ring's SIZE and DESCRIPTOR size into ASIC
    //
    
	// Write TX_RING_CSR0 register
	TxCsr0.word = 0;
	TxCsr0.field.Ac0Total = TX_RING_SIZE;
	TxCsr0.field.Ac1Total = TX_RING_SIZE;
	TxCsr0.field.Ac2Total = TX_RING_SIZE;
	TxCsr0.field.Ac3Total = TX_RING_SIZE;
	RTMP_IO_WRITE32(pAdapter, TX_RING_CSR0, TxCsr0.word);

	// Write TX_RING_CSR1 register
	TxCsr1.word = 0;
    TxCsr1.field.TxdSize = TXD_SIZE/4;
    TxCsr1.field.HccaTotal = TX_RING_SIZE;
    TxCsr1.field.MgmtTotal = MGMT_RING_SIZE;
	RTMP_IO_WRITE32(pAdapter, TX_RING_CSR1, TxCsr1.word);

	// Write RX_RING_CSR register
	RxCsr.word = 0;
	RxCsr.field.RxdSize = RXD_SIZE/4;
	RxCsr.field.RxRingTotal = RX_RING_SIZE;
	RxCsr.field.RxdWritebackSize = 4;
	RTMP_IO_WRITE32(pAdapter, RX_RING_CSR, RxCsr.word);

    //
    // Load shared memeory configuration to ASIC DMA block
    //

    // 0x342c, ASIC TX FIFO to host shared memory mapping
    RTMP_IO_WRITE32(pAdapter, TX_DMA_DST_CSR, 0x000000aa); 

    // ASIC load all TX ring's base address
    RTMP_IO_WRITE32(pAdapter, LOAD_TX_RING_CSR, 0x1f);

    // ASIC load RX ring's base address. still disable RX
    RTMP_IO_WRITE32(pAdapter, RX_CNTL_CSR, 0x00000002);
    
	// Initialze ASIC for TX & Rx operation
	NICInitializeAsic(pAdapter);

    // Load firmware
//  Status = NICLoadFirmware(pAdapter);
    
	DBGPRINT(RT_DEBUG_TRACE, "<-- NICInitializeAdapter\n");
    return Status;
}

void    NICInitializeAsic(
	IN    PRTMP_ADAPTER   pAdapter)
{
	ULONG 			Index, Counter;
	UCHAR			Value = 0xff;
	ULONG 			MacCsr12;

	DBGPRINT(RT_DEBUG_TRACE, "--> NICInitializeAsic\n");

	// Initialize MAC register to default value
	for (Index = 0; Index < NUM_MAC_REG_PARMS; Index++)
	{
		RTMP_IO_WRITE32(pAdapter, MACRegTable[Index].Register, MACRegTable[Index].Value);
	}
	
	// Set Host ready before kicking Rx
//	RTMP_IO_WRITE32(pAdapter, MAC_CSR1, 0x1); // reset MAC state machine, requested by Kevin 2003-2-11
	RTMP_IO_WRITE32(pAdapter, MAC_CSR1, 0x3);
    RTMP_IO_WRITE32(pAdapter, MAC_CSR1, 0x0);

    //
	// Before program BBP, we need to wait BBP/RF get wake up.
	//
	Index = 0;
	do
	{
		RTMP_IO_READ32(pAdapter, MAC_CSR12, &MacCsr12);

		if (MacCsr12 & 0x08)
			break;
		
		RTMPusecDelay(1000);
	} while (Index++ < 1000);

	// Read BBP register, make sure BBP is up and running before write new data
	Index = 0;
	do 
	{
		RTMP_BBP_IO_READ8_BY_REG_ID(pAdapter, BBP_R0, &Value);
		DBGPRINT(RT_DEBUG_TRACE, "BBP version = %d\n", Value);
	} while ((++Index < 100) && ((Value == 0xff) || (Value == 0x00)));
    ASSERT(Index < 100);
    
	// Initialize BBP register to default value
	for (Index = 0; Index < NUM_BBP_REG_PARMS; Index++)
	{
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAdapter, BBPRegTable[Index].Register, BBPRegTable[Index].Value);
	}

	// Kick Rx
    RTMP_IO_WRITE32(pAdapter, RX_CNTL_CSR, 0x00000001);     // enable RX of DMA block

    // This delay is needed when ATE(RXFRAME) turn on
    RTMPusecDelay(10);

	// Clear raw counters
	RTMP_IO_READ32(pAdapter, STA_CSR0, &Counter);
	RTMP_IO_READ32(pAdapter, STA_CSR1, &Counter);
	RTMP_IO_READ32(pAdapter, STA_CSR2, &Counter);
	
    // assert HOST ready bit
//  RTMP_IO_WRITE32(pAdapter, MAC_CSR1, 0x0); // 2004-09-14 asked by Mark
//  RTMP_IO_WRITE32(pAdapter, MAC_CSR1, 0x4);

	RTMP_IO_WRITE32(pAdapter, MAC_CSR1, 0x4);

	DBGPRINT(RT_DEBUG_TRACE, "<-- NICInitializeAsic\n");
}

/*
    ========================================================================

    Routine Description:
        Reset NIC Asics

    Arguments:
        Adapter                     Pointer to our adapter

    Return Value:
        None

    Note:
        Reset NIC to initial state AS IS system boot up time.

    ========================================================================
*/
VOID    NICIssueReset(
    IN  PRTMP_ADAPTER   pAdapter)
{
    ULONG  DmaRxIdle=0;
    ULONG  DmaTxIdle=0;
    int   retryCount = 0;

	DBGPRINT(RT_DEBUG_TRACE, "--> NICIssueReset\n");
	
	// Abort Tx, prevent ASIC from writing to Host memory
	RTMP_IO_WRITE32(pAdapter, TX_CNTL_CSR, 0x001f0000);
	
RetryTxDMA:
	RTMP_IO_READ32(pAdapter,DMA_STATUS_CSR,&DmaTxIdle);
	if((DmaTxIdle & (0x1<<8))) {
        DBGPRINT(RT_DEBUG_TRACE, "DmaTx already idle\n");
        DBGPRINT(RT_DEBUG_TRACE, "DmaTx value (%x) \n", DmaTxIdle);
	}
	else {
	  udelay(1000);// delay 1 ms
	  retryCount++;
	  if (retryCount <= 100)
          goto RetryTxDMA;
	}

	// Disable Rx, register value supposed will remain after reset
	RTMP_IO_WRITE32(pAdapter, TXRX_CSR0, 0x0257b032);//0x0256b032
	// Need Delay
    udelay(1000);// delay 1 ms
	
	// Disbable DMA RX
    RTMP_IO_WRITE32(pAdapter, RX_CNTL_CSR,0x0);
    // 
    // Need Delay
    udelay(1000);// delay 1 ms
    // Check 3480 register. bit 9
    RTMP_IO_READ32(pAdapter,DMA_STATUS_CSR,&DmaRxIdle);

    retryCount = 0;
RetryRxDMA:
    RTMP_IO_READ32(pAdapter,DMA_STATUS_CSR,&DmaRxIdle);  
    if((DmaRxIdle & (0x1<<9))) {
        DBGPRINT(RT_DEBUG_TRACE, "DmaRx already idle\n");
        DBGPRINT(RT_DEBUG_TRACE, "DmaRx value (%x) \n", DmaRxIdle);
    }
    else {
        udelay(1000);// delay 1 ms
	    retryCount++;
	    if (retryCount <= 100)
          goto RetryRxDMA;
    }
	// Issue reset and clear from reset state
	RTMP_IO_WRITE32(pAdapter, MAC_CSR1, 0x03); // 2004-09-17 change from 0x01
	//RTMP_IO_WRITE32(pAdapter, MAC_CSR1, 0x00); // 2005-07-22 Mark suggest

	DBGPRINT(RT_DEBUG_TRACE, "<-- NICIssueReset\n");
}

/*
    ========================================================================

    Routine Description:
        Check ASIC registers and find any reason the system might hang

    Arguments:
        Adapter                     Pointer to our adapter

    Return Value:
        None

    Note:


    ========================================================================
*/
BOOLEAN NICCheckForHang(
    IN  PRTMP_ADAPTER   pAd)
{
    return (FALSE);
}

/*
	========================================================================
	
	Routine Description:
		Read statistical counters from hardware registers and record them
		in software variables for later on query

	Arguments:
		pAdapter					Pointer to our adapter

	Return Value:
		None

	========================================================================
*/
VOID NICUpdateRawCounters(
    IN PRTMP_ADAPTER pAdapter)
{
	STA_CSR0_STRUC StaCsr0;
    STA_CSR1_STRUC StaCsr1;
	STA_CSR2_STRUC StaCsr2;
    STA_CSR3_STRUC StaCsr3;

    RTMP_IO_READ32(pAdapter, STA_CSR0, &StaCsr0.word);

    // Update RX PLCP error counter
    pAdapter->PrivateInfo.PhyRxErrCnt += StaCsr0.field.PlcpErr;
    
    // Update FCS counters
    pAdapter->WlanCounters.FCSErrorCount += StaCsr0.field.CrcErr;
            
    // Add FCS error count to private counters
    pAdapter->RalinkCounters.OneSecRxFcsErrCnt += StaCsr0.field.CrcErr;
    pAdapter->RalinkCounters.RealFcsErrCount += StaCsr0.field.CrcErr;

	// Update False CCA counter
#ifdef CARRIER_DETECTION_SUPPORT
	if (pAdapter->PortCfg.CarrierDetect.Enable == FALSE)
#endif
	{
	RTMP_IO_READ32(pAdapter, STA_CSR1, &StaCsr1.word);
	pAdapter->RalinkCounters.OneSecFalseCCACnt = (ULONG)StaCsr1.field.FalseCca;
	DBGPRINT(RT_DEBUG_INFO, "OneSecFalseCCACnt = %ld\n", pAdapter->RalinkCounters.OneSecFalseCCACnt);
	}

	// Update RX Overflow counter
 	RTMP_IO_READ32(pAdapter, STA_CSR2, &StaCsr2.word);
	pAdapter->Counters8023.RxNoBuffer += (StaCsr2.field.RxOverflowCount + StaCsr2.field.RxFifoOverflowCount);

	// Update BEACON sent count
 	RTMP_IO_READ32(pAdapter, STA_CSR3, &StaCsr3.word);
	pAdapter->RalinkCounters.OneSecBeaconSentCnt += StaCsr3.field.TxBeaconCount;
	
#ifdef RALINK_ATE
	if (pAdapter->ate.bRxFer == 1)
	{
		printk("CrcErr= %d, FalseCca= %d\n", StaCsr0.field.CrcErr, StaCsr1.field.FalseCca);
	}
#endif
}

/*
    ========================================================================

    Routine Description:
        Reset NIC from error

    Arguments:
        Adapter                     Pointer to our adapter

    Return Value:
        None

    Note:
        Reset NIC from error state

    ========================================================================
*/
VOID    NICResetFromError(
    IN  PRTMP_ADAPTER   pAdapter)
{
	// Reset BBP (according to alex, reset ASIC will force reset BBP
	// Therefore, skip the reset BBP
	// RTMP_IO_WRITE32(pAdapter, MAC_CSR1, 0x2);
	// Release BBP reset
	// RTMP_IO_WRITE32(pAdapter, MAC_CSR1, 0x0);
		
	RTMP_IO_WRITE32(pAdapter, MAC_CSR1, 0x1);
	// Remove ASIC from reset state
	RTMP_IO_WRITE32(pAdapter, MAC_CSR1, 0x0);

	// Init send data structures and related parameters
	NICInitTxRxRingAndBacklogQueue(pAdapter);

	NICInitializeAdapter(pAdapter);	
    NICInitAsicFromEEPROM(pAdapter);

	// Switch to current channel, since during reset process, the connection should remains on.	
    AsicSwitchChannel(pAdapter, pAdapter->PortCfg.Channel);
    AsicLockChannel(pAdapter, pAdapter->PortCfg.Channel);
}

/*
    ========================================================================

    Routine Description:
        Find key section for Get key parameter.

    Arguments:
        buffer                      Pointer to the buffer to start find the key section
        section                     the key of the secion to be find

    Return Value:
        NULL                        Fail
        Others                      Success
    ========================================================================
*/
PUCHAR  RTMPFindSection(
    IN  PCHAR   buffer)
{
    CHAR temp_buf[32];
    PUCHAR  ptr;

    strcpy(temp_buf, "Default");

    if((ptr = rtstrstr(buffer, temp_buf)) != NULL)
            return (ptr+strlen("\n"));
        else
            return NULL;
}

 /**
  * strstr - Find the first substring in a %NUL terminated string
  * @s1: The string to be searched
  * @s2: The string to search for
  */
char * rtstrstr(const char * s1,const char * s2)
{
	INT l1, l2;

	l2 = strlen(s2);
	if (!l2)
		return (char *) s1;
	
	l1 = strlen(s1);
	
	while (l1 >= l2)
	{
		l1--;
		if (!memcmp(s1,s2,l2))
			return (char *) s1;
		s1++;
	}
	
	return NULL;
}
 
/**
 * rstrtok - Split a string into tokens
 * @s: The string to be searched
 * @ct: The characters to search for
 * * WARNING: strtok is deprecated, use strsep instead. However strsep is not compatible with old architecture.
 */
char * __rstrtok;
char * rstrtok(char * s,const char * ct)
{
	char *sbegin, *send;

	sbegin  = s ? s : __rstrtok;
	if (!sbegin)
	{
		return NULL;
	}

	sbegin += strspn(sbegin,ct);
	if (*sbegin == '\0')
	{
		__rstrtok = NULL;
		return( NULL );
	}

	send = strpbrk( sbegin, ct);
	if (send && *send != '\0')
		*send++ = '\0';

	__rstrtok = send;

	return (sbegin);
}

/*
    ========================================================================

    Routine Description:
        Get key parameter.

    Arguments:
        key                         Pointer to key string
        dest                        Pointer to destination      
        destsize                    The datasize of the destination
        buffer                      Pointer to the buffer to start find the key

    Return Value:
        TRUE                        Success
        FALSE                       Fail

    Note:
        This routine get the value with the matched key (case case-sensitive)
    ========================================================================
*/
INT RTMPGetKeyParameter(
    IN  PCHAR   key,
    OUT PCHAR   dest,   
    IN  INT     destsize,
    IN  PCHAR   buffer)
{
    CHAR *temp_buf1;
    CHAR *temp_buf2;
    CHAR *start_ptr;
    CHAR *end_ptr;
    CHAR *ptr;
    CHAR *offset = 0;
    INT  len;

	temp_buf1 = kmalloc(MAX_PARAM_BUFFER_SIZE, MEM_ALLOC_FLAG);
	if(temp_buf1 == NULL)
        return (FALSE);
	
	temp_buf2 = kmalloc(MAX_PARAM_BUFFER_SIZE, MEM_ALLOC_FLAG);
	if(temp_buf2 == NULL)
	{
		kfree(temp_buf1);
        return (FALSE);
	}
	
    //find section
    if((offset = RTMPFindSection(buffer)) == NULL)
    {
    	kfree(temp_buf1);
    	kfree(temp_buf2);
        return (FALSE);
    }

    strcpy(temp_buf1, "\n");
    strcat(temp_buf1, key);
    strcat(temp_buf1, "=");

    //search key
    if((start_ptr=rtstrstr(offset, temp_buf1))==NULL)
    {
    	kfree(temp_buf1);
    	kfree(temp_buf2);
        return (FALSE);
    }

    start_ptr+=strlen("\n");
    if((end_ptr=rtstrstr(start_ptr, "\n"))==NULL)
       end_ptr=start_ptr+strlen(start_ptr);

    if (end_ptr<start_ptr)
    {
    	kfree(temp_buf1);
    	kfree(temp_buf2);
        return (FALSE);
    }

    memcpy(temp_buf2, start_ptr, end_ptr-start_ptr);
    temp_buf2[end_ptr-start_ptr]='\0';
    len = strlen(temp_buf2);
    strcpy(temp_buf1, temp_buf2);
    if((start_ptr=rtstrstr(temp_buf1, "=")) == NULL)
    {
    	kfree(temp_buf1);
    	kfree(temp_buf2);
        return (FALSE);
    }

    strcpy(temp_buf2, start_ptr+1);
    ptr = temp_buf2;
    //trim space or tab
    while(*ptr != 0x00)
    {
        if( (*ptr == ' ') || (*ptr == '\t') )
            ptr++;
        else
           break;
    }

    len = strlen(ptr);    
    memset(dest, 0x00, destsize);
    strncpy(dest, ptr, len >= destsize ?  destsize: len);

	kfree(temp_buf1);
   	kfree(temp_buf2);
    return TRUE;
}

//iverson
INT RTMPGetKeyParameter_SSID(
    IN  PCHAR   key,
    OUT PCHAR   dest,   
    IN  INT     destsize,
    IN  PCHAR   buffer)
{
    CHAR *temp_buf1;
    CHAR *temp_buf2;
    CHAR *start_ptr;
    CHAR *end_ptr;
    CHAR *ptr;
    CHAR *offset = 0;
    INT  len;

	temp_buf1 = kmalloc(MAX_PARAM_BUFFER_SIZE, MEM_ALLOC_FLAG);
	if(temp_buf1 == NULL)
        return (FALSE);
	
	temp_buf2 = kmalloc(MAX_PARAM_BUFFER_SIZE, MEM_ALLOC_FLAG);
	if(temp_buf2 == NULL)
	{
		kfree(temp_buf1);
        return (FALSE);
	}
	
    //find section
    if((offset = RTMPFindSection(buffer)) == NULL)
    {
    	kfree(temp_buf1);
    	kfree(temp_buf2);
        return (FALSE);
    }

    strcpy(temp_buf1, "\n");
    strcat(temp_buf1, key);
    strcat(temp_buf1, "=");

    //search key
    if((start_ptr=rtstrstr(offset, temp_buf1))==NULL)
    {
    	kfree(temp_buf1);
    	kfree(temp_buf2);
        return (FALSE);
    }

    start_ptr+=strlen("\n");
    if((end_ptr=rtstrstr(start_ptr, "\n"))==NULL)
       end_ptr=start_ptr+strlen(start_ptr);

    if (end_ptr<start_ptr)
    {
    	kfree(temp_buf1);
    	kfree(temp_buf2);
        return (FALSE);
    }

    memcpy(temp_buf2, start_ptr, end_ptr-start_ptr);
    temp_buf2[end_ptr-start_ptr]='\0';
    len = strlen(temp_buf2);
    strcpy(temp_buf1, temp_buf2);
    if((start_ptr=rtstrstr(temp_buf1, "=")) == NULL)
    {
    	kfree(temp_buf1);
    	kfree(temp_buf2);
        return (FALSE);
    }

    strcpy(temp_buf2, start_ptr+1);
    ptr = temp_buf2;
    //trim space or tab
    //2007/09/28: KH add to permit ApCliSsid and SSID to assign space and tab
    if(strcmp(key,"ApCliSsid")&&strcmp(key,"SSID"))
    while(*ptr != 0x00)
    {
        if( (*ptr == '\t') )
            ptr++;
        else
           break;
    }

    len = strlen(ptr);    
    memset(dest, 0x00, destsize);
    strncpy(dest, ptr, len >= destsize ?  destsize: len);

	kfree(temp_buf1);
   	kfree(temp_buf2);
    return TRUE;
}

//iverson end
static void rtmp_read_key_parms_from_file(IN  PRTMP_ADAPTER pAd, char *tmpbuf, char *buffer)
{
	char		tok_str[16];
	PUCHAR		macptr;						
	INT			i=0, idx;
	ULONG		KeyType[MAX_MBSSID_NUM];
	ULONG		KeyLen;
	ULONG		KeyIdx;
	UCHAR		CipherAlg = CIPHER_WEP64;

	//DefaultKeyID
	if(RTMPGetKeyParameter("DefaultKeyID", tmpbuf, 25, buffer))
	{
	    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
	    {
			KeyIdx = simple_strtol(macptr, 0, 10);
			if((KeyIdx >= 1 ) && (KeyIdx <= 4))
				pAd->PortCfg.MBSSID[i].DefaultKeyId = (UCHAR) (KeyIdx - 1 );
			else
				pAd->PortCfg.MBSSID[i].DefaultKeyId = 0;

			DBGPRINT(RT_DEBUG_TRACE, "I/F(ra%d) DefaultKeyID(0~3)=%d\n", i, pAd->PortCfg.MBSSID[i].DefaultKeyId);
	    }
	}	   

	for (idx=0; idx<4; idx++) {

		sprintf(tok_str, "Key%dType", idx+1);
		//Key1Type
		if(RTMPGetKeyParameter(tok_str, tmpbuf, 128, buffer))
		{
		    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
		    {
			    KeyType[i] = simple_strtol(macptr, 0, 10);
		    }

			sprintf(tok_str, "Key%dStr", idx+1);
			//Key1Str
			if(RTMPGetKeyParameter(tok_str, tmpbuf, 512, buffer))
			{
			    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
		        {
					KeyLen = strlen(macptr);
                    pAd->PortCfg.MBSSID[i].SharedKey[idx].WepKeyType = KeyType[i];
					if(KeyType[i] == 0)
					{//Hex type
						if( (KeyLen == 10) || (KeyLen == 26))
						{
							pAd->PortCfg.MBSSID[i].SharedKey[idx].KeyLen = KeyLen / 2;
							AtoH(macptr, pAd->PortCfg.MBSSID[i].SharedKey[idx].Key, KeyLen / 2);
							if (KeyLen == 10)
								CipherAlg = CIPHER_WEP64;
							else
								CipherAlg = CIPHER_WEP128;
							pAd->PortCfg.MBSSID[i].SharedKey[idx].CipherAlg = CipherAlg;

							DBGPRINT(RT_DEBUG_TRACE, "I/F(ra%d) Key%dStr=%s and type=%s\n", i, idx+1, macptr, (KeyType[i]==0) ? "Hex":"Ascii");
						}
						else
						{ //Invalid key length
							DBGPRINT(RT_DEBUG_ERROR, "Key%dStr is Invalid key length!\n", idx+1);
						}								
					}
					else
					{//Ascii								
						if( (KeyLen == 5) || (KeyLen == 13))
						{
							pAd->PortCfg.MBSSID[i].SharedKey[idx].KeyLen = KeyLen;
							NdisMoveMemory(pAd->PortCfg.MBSSID[i].SharedKey[idx].Key, macptr, KeyLen);
							if (KeyLen == 5)
								CipherAlg = CIPHER_WEP64;
							else
								CipherAlg = CIPHER_WEP128;
							pAd->PortCfg.MBSSID[i].SharedKey[idx].CipherAlg = CipherAlg;
					
							DBGPRINT(RT_DEBUG_TRACE, "I/F(ra%d) Key%dStr=%s and type=%s\n", i, idx+1, macptr, (KeyType[i]==0) ? "Hex":"Ascii");		
						}
						else
						{ //Invalid key length
							DBGPRINT(RT_DEBUG_ERROR, "Key%dStr is Invalid key length!\n", idx+1);
						}
					}
			    }
			}
		}
	}
}

#ifdef APCLI_SUPPORT
//KH: The following function, "rtmp_read_ApClikey_parms_from_file", is used to read the WEP keys and Defaultkey of Ap Client. 
static void rtmp_read_ApClikey_parms_from_file(IN  PRTMP_ADAPTER pAd, char *tmpbuf, char *buffer)
{
	char		tok_str[16];						
	INT			idx;
	ULONG		KeyType;
	ULONG		KeyLen;
	ULONG		KeyIdx;
	UCHAR		CipherAlg = CIPHER_WEP64;

	//DefaultKeyID
	
	if(RTMPGetKeyParameter("ApCliDefaultKeyID", tmpbuf, 25, buffer))
	{
		KeyIdx = simple_strtol(tmpbuf, 0, 10);
		if((KeyIdx >= 1 ) && (KeyIdx <= 4))
			pAd->ApCliTab.ApCliEntry[0].DefaultKeyId = (UCHAR) (KeyIdx - 1 );
		else
			pAd->ApCliTab.ApCliEntry[0].DefaultKeyId = 0;

		//DBGPRINT(RT_DEBUG_TRACE, "I/F(ra%d) DefaultKeyID(0~3)=%d\n", i, pAd->PortCfg.MBSSID[i].DefaultKeyId);
	}	   

	for (idx=0; idx<4; idx++) 
	{
		sprintf(tok_str, "ApCliKey%dType", idx+1);
		//Key?Type
		if(RTMPGetKeyParameter(tok_str, tmpbuf, 128, buffer))
		{
   		    KeyType= simple_strtol(tmpbuf, 0, 10);
		    sprintf(tok_str, "ApCliKey%dStr", idx+1);
		    //Key?Str
		    if(RTMPGetKeyParameter(tok_str, tmpbuf, 40, buffer))
		   	{
				KeyLen = strlen(tmpbuf);
				//Which key
                pAd->ApCliTab.ApCliEntry[0].SharedKey[idx].WepKeyType = KeyType;
				if(KeyType == 0)
				{//Hex type
					if( (KeyLen == 10) || (KeyLen == 26))
					{
						pAd->ApCliTab.ApCliEntry[0].SharedKey[idx].KeyLen = KeyLen / 2;
						AtoH(tmpbuf, pAd->ApCliTab.ApCliEntry[0].SharedKey[idx].Key, KeyLen / 2);
						if (KeyLen == 10)
							CipherAlg = CIPHER_WEP64;
						else
							CipherAlg = CIPHER_WEP128;
						pAd->ApCliTab.ApCliEntry[0].SharedKey[idx].CipherAlg = CipherAlg;

						//DBGPRINT(RT_DEBUG_TRACE, " Key%dStr=%s and type=%s\n", i, idx+1, tmpbuf, (KeyType==0) ? "Hex":"Ascii");
					}
					else
					{ //Invalid key length
						DBGPRINT(RT_DEBUG_ERROR, "Key%dStr is Invalid key length!\n", idx+1);
					}								
				}
				else
				{//Ascii								
					if( (KeyLen == 5) || (KeyLen == 13))
					{
						pAd->ApCliTab.ApCliEntry[0].SharedKey[idx].KeyLen = KeyLen;
						NdisMoveMemory(pAd->ApCliTab.ApCliEntry[0].SharedKey[idx].Key, tmpbuf, KeyLen);
						if (KeyLen == 5)
							CipherAlg = CIPHER_WEP64;
						else
							CipherAlg = CIPHER_WEP128;
						pAd->ApCliTab.ApCliEntry[0].SharedKey[idx].CipherAlg = CipherAlg;
					
						DBGPRINT(RT_DEBUG_TRACE, " Key%dStr=%s and type=%s\n", idx+1, tmpbuf, (KeyType==0) ? "Hex":"Ascii");		
					}
					else
					{ //Invalid key length
						DBGPRINT(RT_DEBUG_ERROR, "Key%dStr is Invalid key length!\n", idx+1);
					}
				}		
			}
		}
   	}
}


static void rtmp_read_ap_client_from_file(IN  PRTMP_ADAPTER pAd, char *tmpbuf, char *buffer)
{
	PUCHAR		macptr;						
	INT			i=0,j=0;
	UCHAR		macAddress[MAC_ADDR_LEN];
	UCHAR		keyMaterial[40];

	//ApCliEnable
	if(RTMPGetKeyParameter("ApCliEnable", tmpbuf, 128, buffer))
	{
		for (i = 0, macptr = rstrtok(tmpbuf,";"); (macptr && i < MAX_APCLI_ENTRY); macptr = rstrtok(NULL,";"), i++)
		{
			if ((strncmp(macptr, "0", 1) == 0))
				pAd->ApCliTab.ApCliEntry[i].Enable = FALSE;
			else if ((strncmp(macptr, "1", 1) == 0))
				pAd->ApCliTab.ApCliEntry[i].Enable = TRUE;
	        else
				pAd->ApCliTab.ApCliEntry[i].Enable = FALSE;

			if (pAd->ApCliTab.ApCliEntry[i].Enable)
			{
				pAd->ApCliTab.ApCliEntry[i].WpaState = SS_NOTUSE;
				pAd->ApCliTab.ApCliEntry[i].PortSecured = WPA_802_1X_PORT_NOT_SECURED;
				NdisZeroMemory(pAd->ApCliTab.ApCliEntry[i].ReplayCounter, LEN_KEY_DESC_REPLAY); 
			}
			DBGPRINT(RT_DEBUG_TRACE,"ApCliEntry[%d].Enable=%d\n", i, pAd->ApCliTab.ApCliEntry[i].Enable);
	    }
	}

	//ApCliSsid
	if(RTMPGetKeyParameter("ApCliSsid", tmpbuf, MAX_PARAM_BUFFER_SIZE, buffer))
	{
		for (i=0, macptr = rstrtok(tmpbuf,";"); (macptr && i < MAX_APCLI_ENTRY); macptr = rstrtok(NULL,";"), i++) 
		{
			//Ssid acceptable strlen must be less than 32 and bigger than 0.
			if((strlen(macptr) < 0) || (strlen(macptr) > 32))
				continue; 

			pAd->ApCliTab.ApCliEntry[i].CfgSsidLen = strlen(macptr);
			if(pAd->ApCliTab.ApCliEntry[i].CfgSsidLen > 0)
			{
				memcpy(&pAd->ApCliTab.ApCliEntry[i].CfgSsid, macptr, pAd->ApCliTab.ApCliEntry[i].CfgSsidLen);
				pAd->ApCliTab.ApCliEntry[i].Valid = FALSE;// it should be set when successfuley association
			} else
			{
				NdisZeroMemory(&(pAd->ApCliTab.ApCliEntry[i].CfgSsid), MAX_LEN_OF_SSID);
				continue;
			}
			DBGPRINT(RT_DEBUG_TRACE,"ApCliEntry[%d].CfgSsidLen=%d, CfgSsid=%s\n", i, pAd->ApCliTab.ApCliEntry[i].CfgSsidLen, pAd->ApCliTab.ApCliEntry[i].CfgSsid);
		}
	}

	//ApCliBssid
	if(RTMPGetKeyParameter("ApCliBssid", tmpbuf, MAX_PARAM_BUFFER_SIZE, buffer))
	{
		for (i=0, macptr = rstrtok(tmpbuf,";"); (macptr && i < MAX_APCLI_ENTRY); macptr = rstrtok(NULL,";"), i++) 
		{
			if(strlen(macptr) != 17)  //Mac address acceptable format 01:02:03:04:05:06 length 17
				continue; 
			if(strcmp(macptr,"00:00:00:00:00:00") == 0)
				continue; 
			if(i >= MAX_APCLI_ENTRY)
				break; 
			for (j=0; j<ETH_LENGTH_OF_ADDRESS; j++)
			{
				AtoH(macptr, &macAddress[j], 2);
				macptr=macptr+3;
			}	
			memcpy(&pAd->ApCliTab.ApCliEntry[i].CfgApCliBssid[0], &macAddress, ETH_LENGTH_OF_ADDRESS);
			pAd->ApCliTab.ApCliEntry[i].Valid = FALSE;// it should be set when successfuley association
		}
	}
	
	// ApCliWPAPSK					
	if (RTMPGetKeyParameter("ApCliWPAPSK", tmpbuf, 255, buffer))
	{
		for (i = 0, macptr = rstrtok(tmpbuf,";"); (macptr && i < MAX_APCLI_ENTRY); macptr = rstrtok(NULL,";"), i++)
		{
			int     err = 0;
            
            if((strlen(macptr) < 8) || (strlen(macptr) > 64))
            {
				DBGPRINT(RT_DEBUG_ERROR, "APCli_WPAPSK_KEY, key string required 8 ~ 64 characters!!!\n");
				continue; 
			}
			
			NdisMoveMemory(pAd->ApCliTab.ApCliEntry[i].PSK, macptr, strlen(macptr));
			pAd->ApCliTab.ApCliEntry[i].PSKLen = strlen(macptr);
			DBGPRINT(RT_DEBUG_TRACE, "I/F(apcli%d) APCli_WPAPSK_KEY=%s, Len=%d\n", i, pAd->ApCliTab.ApCliEntry[i].PSK, pAd->ApCliTab.ApCliEntry[i].PSKLen); 			      		
			           
            if ((pAd->PortCfg.MBSSID[MAIN_MBSSID].AuthMode != Ndis802_11AuthModeWPAPSK) &&
				            (pAd->PortCfg.MBSSID[MAIN_MBSSID].AuthMode != Ndis802_11AuthModeWPA2PSK))
			{
				err = 1;
			}
			
			if ((strlen(macptr) >= 8) && (strlen(macptr) < 64))
			{// ASCII mode
				PasswordHash((char *)macptr, pAd->ApCliTab.ApCliEntry[i].CfgSsid, pAd->ApCliTab.ApCliEntry[i].CfgSsidLen, keyMaterial);
				NdisMoveMemory(pAd->ApCliTab.ApCliEntry[i].PMK, keyMaterial, 32);
			}
			else if (strlen(macptr) == 64)
			{// Hex mode
				AtoH(macptr, pAd->ApCliTab.ApCliEntry[i].PMK, 32);
			}
	
			if (err == 0)
			{  
				// Start STA supplicant WPA state machine
				DBGPRINT(RT_DEBUG_TRACE, "Start STA supplicant WPA state machine \n");
                pAd->ApCliTab.ApCliEntry[i].WpaState = SS_START;
            }    
#ifdef DBG
			DBGPRINT(RT_DEBUG_TRACE, "I/F(apcli%d) PMK Material => \n", i);
			for (j = 0; j < 32; j++)
			{
				printk("%02x:", pAd->ApCliTab.ApCliEntry[i].PMK[j]);
				if ((j%16) == 15)
					printk("\n");
			}
			printk("\n");
#endif
		}
	}
	
	//KH Adding
	rtmp_read_ApClikey_parms_from_file(pAd, tmpbuf, buffer);
}
#endif

#ifdef IGMP_SNOOP_SUPPORT
static void rtmp_read_igmp_snoop_from_file(IN  PRTMP_ADAPTER pAd, char *tmpbuf, char *buffer)
{
	PUCHAR		macptr;						
	INT			i=0;

	//IgmpSnEnable
	if(RTMPGetKeyParameter("IgmpSnEnable", tmpbuf, 128, buffer))
	{
		for (i = 0, macptr = rstrtok(tmpbuf,";"); (macptr && i < pAd->PortCfg.BssidNum); macptr = rstrtok(NULL,";"), i++)
		{
			if ((strncmp(macptr, "0", 1) == 0))
				pAd->PortCfg.MBSSID[i].IgmpSnoopEnable = FALSE;
			else if ((strncmp(macptr, "1", 1) == 0))
				pAd->PortCfg.MBSSID[i].IgmpSnoopEnable = TRUE;
	        else
				pAd->PortCfg.MBSSID[i].IgmpSnoopEnable = FALSE;

			DBGPRINT(RT_DEBUG_TRACE,"MBSSID[%d].Enable=%d\n", i, pAd->PortCfg.MBSSID[i].IgmpSnoopEnable);
	    }
	}
}
#endif // IGMP_SNOOP_SUPPORT //

static void rtmp_read_acl_parms_from_file(IN  PRTMP_ADAPTER pAd, char *tmpbuf, char *buffer)
{
	char		tok_str[32];
	PUCHAR		macptr;						
	INT			i=0, j=0, idx;
	UCHAR		macAddress[MAC_ADDR_LEN];
											  
	for (idx=0; idx<MAX_MBSSID_NUM; idx++) {
		//AccessPolicy0
		sprintf(tok_str, "AccessPolicy%d", idx);
		if(RTMPGetKeyParameter(tok_str, tmpbuf, 10, buffer))
		{
			switch (simple_strtol(tmpbuf, 0, 10))
			{
				case 1: //Allow All
					pAd->PortCfg.MBSSID[idx].AccessControlList.Policy = 1;
					break;
				case 2: //Reject All
					pAd->PortCfg.MBSSID[idx].AccessControlList.Policy = 2;
					break;
				case 0: //Disable
				default:
					pAd->PortCfg.MBSSID[idx].AccessControlList.Policy = 0;
					break;
			}
			DBGPRINT(RT_DEBUG_TRACE, "%s=%d\n", tok_str, pAd->PortCfg.MBSSID[idx].AccessControlList.Policy);
		}
		//AccessControlList0
		sprintf(tok_str, "AccessControlList%d", idx);
		if(RTMPGetKeyParameter(tok_str, tmpbuf, MAX_PARAM_BUFFER_SIZE, buffer))
		{
			for (i=0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++) 
			{
				if (strlen(macptr) != 17)  //Mac address acceptable format 01:02:03:04:05:06 length 17
					continue;
				if (pAd->PortCfg.MBSSID[idx].AccessControlList.Num >= MAX_NUM_OF_ACL_LIST)
				    break;
			
				for (j=0; j<ETH_LENGTH_OF_ADDRESS; j++)
				{
					AtoH(macptr, &macAddress[j], 2);
					macptr=macptr+3;
				}
				
				pAd->PortCfg.MBSSID[idx].AccessControlList.Num++;
				NdisMoveMemory(pAd->PortCfg.MBSSID[idx].AccessControlList.Entry[i].Addr, macAddress, ETH_LENGTH_OF_ADDRESS);				
}
			DBGPRINT(RT_DEBUG_TRACE, "%s=Get %d Mac Address\n", tok_str, pAd->PortCfg.MBSSID[idx].AccessControlList.Num);
 		}
	}
}

#ifdef WMM_SUPPORT
/*
    ========================================================================

    Routine Description:
        In kernel mode read parameters from file

    Arguments:
        src                     the location of the file.
        dest                        put the parameters to the destination.
        Length                  size to read.

    Return Value:
        None

    Note:

    ========================================================================
*/
static void rtmp_read_wmm_parms_from_file(IN  PRTMP_ADAPTER pAd, char *tmpbuf, char *buffer)
{
	PUCHAR					macptr;						
	INT						i=0;

	//WmmCapable
	if(RTMPGetKeyParameter("WmmCapable", tmpbuf, 32, buffer))
	{
	    BOOLEAN bEnableWmm = FALSE;
	    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
	    {
			if(simple_strtol(macptr, 0, 10) != 0)  //Enable
			{
				pAd->PortCfg.MBSSID[i].bWmmCapable = TRUE;
				bEnableWmm = TRUE;
			}
			else //Disable
			{
				pAd->PortCfg.MBSSID[i].bWmmCapable = FALSE;
			}
			if (bEnableWmm)
			{
				pAd->PortCfg.APEdcaParm.bValid = TRUE;
				pAd->PortCfg.BssEdcaParm.bValid = TRUE;
			}
			else
				{
				pAd->PortCfg.APEdcaParm.bValid = FALSE;
				pAd->PortCfg.BssEdcaParm.bValid = FALSE;
			}

			DBGPRINT(RT_DEBUG_TRACE, "I/F(ra%d) WmmCapable=%d\n", i, pAd->PortCfg.MBSSID[i].bWmmCapable);
	    }
	}
	//DLSCapable
	if(RTMPGetKeyParameter("DLSCapable", tmpbuf, 32, buffer))
	{
	    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
	    {
			if(simple_strtol(macptr, 0, 10) != 0)  //Enable
			{
				pAd->PortCfg.MBSSID[i].bDLSCapable = TRUE;
			}
			else //Disable
			{
				pAd->PortCfg.MBSSID[i].bDLSCapable = FALSE;
			}

			DBGPRINT(RT_DEBUG_TRACE, "I/F(ra%d) DLSCapable=%d\n", i, pAd->PortCfg.MBSSID[i].bDLSCapable);
	    }
	}
	//APAifsn
	if(RTMPGetKeyParameter("APAifsn", tmpbuf, 32, buffer))
	{
	    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
	    {
			pAd->PortCfg.APEdcaParm.Aifsn[i] = simple_strtol(macptr, 0, 10);;

			DBGPRINT(RT_DEBUG_TRACE, "APAifsn[%d]=%d\n", i, pAd->PortCfg.APEdcaParm.Aifsn[i]);
	    }
	}
	//APCwmin
	if(RTMPGetKeyParameter("APCwmin", tmpbuf, 32, buffer))
	{
	    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
	    {
			pAd->PortCfg.APEdcaParm.Cwmin[i] = simple_strtol(macptr, 0, 10);;

			DBGPRINT(RT_DEBUG_TRACE, "APCwmin[%d]=%d\n", i, pAd->PortCfg.APEdcaParm.Cwmin[i]);
	    }
	}
	//APCwmax
	if(RTMPGetKeyParameter("APCwmax", tmpbuf, 32, buffer))
	{
	    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
	    {
			pAd->PortCfg.APEdcaParm.Cwmax[i] = simple_strtol(macptr, 0, 10);;

			DBGPRINT(RT_DEBUG_TRACE, "APCwmax[%d]=%d\n", i, pAd->PortCfg.APEdcaParm.Cwmax[i]);
	    }
	}
	//APTxop
	if(RTMPGetKeyParameter("APTxop", tmpbuf, 32, buffer))
	{
	    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
	    {
			pAd->PortCfg.APEdcaParm.Txop[i] = simple_strtol(macptr, 0, 10);;

			DBGPRINT(RT_DEBUG_TRACE, "APTxop[%d]=%d\n", i, pAd->PortCfg.APEdcaParm.Txop[i]);
	    }
	}
	//APACM
	if(RTMPGetKeyParameter("APACM", tmpbuf, 32, buffer))
	{
	    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
	    {
			pAd->PortCfg.APEdcaParm.bACM[i] = simple_strtol(macptr, 0, 10);;

			DBGPRINT(RT_DEBUG_TRACE, "APACM[%d]=%d\n", i, pAd->PortCfg.APEdcaParm.bACM[i]);
	    }
	}
	//BSSAifsn
	if(RTMPGetKeyParameter("BSSAifsn", tmpbuf, 32, buffer))
	{
	    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
	    {
			pAd->PortCfg.BssEdcaParm.Aifsn[i] = simple_strtol(macptr, 0, 10);;

			DBGPRINT(RT_DEBUG_TRACE, "BSSAifsn[%d]=%d\n", i, pAd->PortCfg.BssEdcaParm.Aifsn[i]);
	    }
	}
	//BSSCwmin
	if(RTMPGetKeyParameter("BSSCwmin", tmpbuf, 32, buffer))
	{
	    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
	    {
			pAd->PortCfg.BssEdcaParm.Cwmin[i] = simple_strtol(macptr, 0, 10);;

			DBGPRINT(RT_DEBUG_TRACE, "BSSCwmin[%d]=%d\n", i, pAd->PortCfg.BssEdcaParm.Cwmin[i]);
	    }
	}
	//BSSCwmax
	if(RTMPGetKeyParameter("BSSCwmax", tmpbuf, 32, buffer))
	{
	    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
	    {
			pAd->PortCfg.BssEdcaParm.Cwmax[i] = simple_strtol(macptr, 0, 10);;

			DBGPRINT(RT_DEBUG_TRACE, "BSSCwmax[%d]=%d\n", i, pAd->PortCfg.BssEdcaParm.Cwmax[i]);
	    }
	}
	//BSSTxop
	if(RTMPGetKeyParameter("BSSTxop", tmpbuf, 32, buffer))
	{
	    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
	    {
			pAd->PortCfg.BssEdcaParm.Txop[i] = simple_strtol(macptr, 0, 10);;

			DBGPRINT(RT_DEBUG_TRACE, "BSSTxop[%d]=%d\n", i, pAd->PortCfg.BssEdcaParm.Txop[i]);
	    }
	}
	//BSSACM
	if(RTMPGetKeyParameter("BSSACM", tmpbuf, 32, buffer))
	{
	    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
	    {
			pAd->PortCfg.BssEdcaParm.bACM[i] = simple_strtol(macptr, 0, 10);;

			DBGPRINT(RT_DEBUG_TRACE, "BSSACM[%d]=%d\n", i, pAd->PortCfg.BssEdcaParm.bACM[i]);
	    }
	}
	//AckPolicy
	if(RTMPGetKeyParameter("AckPolicy", tmpbuf, 32, buffer))
	{
	    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
	    {
			pAd->PortCfg.AckPolicy[i] = simple_strtol(macptr, 0, 10);;

			DBGPRINT(RT_DEBUG_TRACE, "AckPolicy[%d]=%d\n", i, pAd->PortCfg.AckPolicy[i]);
	    }
	}
	//APSDCapable
	if(RTMPGetKeyParameter("APSDCapable", tmpbuf, 10, buffer))
	{
		if(simple_strtol(tmpbuf, 0, 10) != 0)  //Enable
			pAd->PortCfg.bAPSDCapable = TRUE;
		else
			pAd->PortCfg.bAPSDCapable = FALSE;

		DBGPRINT(RT_DEBUG_TRACE, "APSDCapable=%d\n", pAd->PortCfg.bAPSDCapable);
	}
#ifdef RTL865X_SOC
	//EthWithVLANTag
	if(RTMPGetKeyParameter("EthWithVLANTag", tmpbuf, 10, buffer))
	{
		if(simple_strtol(tmpbuf, 0, 10) != 0)  //Enable
			pAd->PortCfg.bEthWithVLANTag = TRUE;
		else
			pAd->PortCfg.bEthWithVLANTag = FALSE;

		DBGPRINT(RT_DEBUG_TRACE, "bEthWithVLANTag=%d\n", pAd->PortCfg.bEthWithVLANTag);
	}
#endif
}
#endif /* WMM_SUPPORT */

VOID RTMPReadParametersFromFile(
    IN  PRTMP_ADAPTER pAd)
{
	PUCHAR					src;
	struct file				*srcf;
	INT 					retval, orgfsuid, orgfsgid;
   	mm_segment_t			orgfs;
	CHAR					*buffer;
	CHAR					*tmpbuf;
	UCHAR					keyMaterial[40];
	ULONG					RtsThresh;
	ULONG					FragThresh;
#ifdef WDS_SUPPORT
	UCHAR					macAddress[MAC_ADDR_LEN];
#endif
	PUCHAR					macptr;							
	INT						i=0, j=0;

	buffer = kmalloc(MAX_INI_BUFFER_SIZE, MEM_ALLOC_FLAG);
	if(buffer == NULL)
        return;

	tmpbuf = kmalloc(MAX_PARAM_BUFFER_SIZE, MEM_ALLOC_FLAG);
	if(tmpbuf == NULL)
	{
		kfree(buffer);
        return;
	}

   	src = PROFILE_PATH;
   	
	// Save uid and gid used for filesystem access.
	// Set user and group to 0 (root)	
	orgfsuid = current->fsuid;
	orgfsgid = current->fsgid;
	current->fsuid=current->fsgid = 0;
    orgfs = get_fs();
    set_fs(KERNEL_DS);

	if (src && *src) 
	{
		srcf = filp_open(src, O_RDONLY, 0);
		if (IS_ERR(srcf)) 
		{
			DBGPRINT(RT_DEBUG_TRACE, "--> Error %ld opening %s\n", -PTR_ERR(srcf),src);    
		}
		else 
		{
			// The object must have a read method
			if (srcf->f_op && srcf->f_op->read) 
			{
				memset(buffer, 0x00, MAX_INI_BUFFER_SIZE);
				retval=srcf->f_op->read(srcf, buffer, MAX_INI_BUFFER_SIZE, &srcf->f_pos);
				if (retval < 0)
				{
					DBGPRINT(RT_DEBUG_TRACE, "--> Read %s error %d\n", src, -retval);
				}
				else
				{
					// set file parameter to portcfg
					//CountryRegion
					if(RTMPGetKeyParameter("CountryRegion", tmpbuf, 25, buffer))
					{
						pAd->PortCfg.CountryRegion = (UCHAR) simple_strtol(tmpbuf, 0, 10);
						DBGPRINT(RT_DEBUG_TRACE, "CountryRegion=%d\n", pAd->PortCfg.CountryRegion);
					}
					//CountryRegionABand
					if(RTMPGetKeyParameter("CountryRegionABand", tmpbuf, 25, buffer))
					{
						pAd->PortCfg.CountryRegionForABand= (UCHAR) simple_strtol(tmpbuf, 0, 10);
						DBGPRINT(RT_DEBUG_TRACE, "CountryRegionABand=%d\n", pAd->PortCfg.CountryRegionForABand);
					}
					//CountryCode
					if(RTMPGetKeyParameter("CountryCode", tmpbuf, 25, buffer))
					{
						memcpy(pAd->PortCfg.CountryCode, tmpbuf , 2);
						if (strlen(pAd->PortCfg.CountryCode) != 0)
						{
							pAd->PortCfg.CountryCode[2] = ' ';
							pAd->PortCfg.bCountryFlag = TRUE;
						}
						DBGPRINT(RT_DEBUG_TRACE, "CountryCode=%s\n", pAd->PortCfg.CountryCode);
					}
#ifdef MBSS_SUPPORT
					//BSSIDNum; This must read first of other multiSSID field, so list this field first in rt2500ap.dat
					if(RTMPGetKeyParameter("BssidNum", tmpbuf, 25, buffer))
					{
						pAd->PortCfg.BssidNum = (UCHAR) simple_strtol(tmpbuf, 0, 10);
						if(pAd->PortCfg.BssidNum > MAX_MBSSID_NUM)
						{
							pAd->PortCfg.BssidNum = MAX_MBSSID_NUM;
							DBGPRINT(RT_DEBUG_TRACE, "BssidNum=%d(MAX_MBSSID_NUM is %d)\n", pAd->PortCfg.BssidNum,MAX_MBSSID_NUM);
						}
						else
						DBGPRINT(RT_DEBUG_TRACE, "BssidNum=%d\n", pAd->PortCfg.BssidNum);
					}
#else
					pAd->PortCfg.BssidNum = 1;
#endif 												
					//SSID
					if(RTMPGetKeyParameter_SSID("SSID", tmpbuf, 256, buffer))
					{
					    for (i=0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
						{
						    memcpy(pAd->PortCfg.MBSSID[i].Ssid, macptr , strlen(macptr));
						    pAd->PortCfg.MBSSID[i].Ssid[strlen(macptr)] = '\0';
						    pAd->PortCfg.MBSSID[i].SsidLen = strlen(pAd->PortCfg.MBSSID[i].Ssid);
						    
						    DBGPRINT(RT_DEBUG_TRACE, "SSID[%d]=%s\n", i, pAd->PortCfg.MBSSID[i].Ssid);
					    }
					}
					//WirelessMode
					if(RTMPGetKeyParameter("WirelessMode", tmpbuf, 10, buffer))
					{
						switch (simple_strtol(tmpbuf, 0, 10))
						{
							case PHY_11A:
								pAd->PortCfg.PhyMode = PHY_11A;
								break;
							case PHY_11B:
								pAd->PortCfg.PhyMode = PHY_11B;
								break;
							case PHY_11G:
								pAd->PortCfg.PhyMode = PHY_11G;
								break;
							case PHY_11BG_MIXED:
							default:
								pAd->PortCfg.PhyMode = PHY_11BG_MIXED;
								break;
						}
						DBGPRINT(RT_DEBUG_TRACE, "PhyMode=%d\n", pAd->PortCfg.PhyMode);
					}
					//TxRate
					if(RTMPGetKeyParameter("TxRate", tmpbuf, 20, buffer))
					{
						for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
						{
							pAd->PortCfg.MBSSID[i].DesiredRatesIndex = simple_strtol(macptr, 0, 10);
							RTMPBuildDesireRate(pAd, i,pAd->PortCfg.MBSSID[i].DesiredRatesIndex);
						}
					}
					//Channel
					if(RTMPGetKeyParameter("Channel", tmpbuf, 10, buffer))
					{
						pAd->PortCfg.Channel = (UCHAR) simple_strtol(tmpbuf, 0, 10);
						DBGPRINT(RT_DEBUG_TRACE, "Channel=%d\n", pAd->PortCfg.Channel);				
					}
                    //BasicRate
					if(RTMPGetKeyParameter("BasicRate", tmpbuf, 10, buffer))
					{
						pAd->PortCfg.BasicRateBitMap = simple_strtol(tmpbuf, 0, 10);
						DBGPRINT(RT_DEBUG_TRACE, "BasicRate=%d\n", pAd->PortCfg.BasicRateBitMap);				
					}
					//BeaconPeriod
					if(RTMPGetKeyParameter("BeaconPeriod", tmpbuf, 10, buffer))
					{
						pAd->PortCfg.BeaconPeriod = (USHORT) simple_strtol(tmpbuf, 0, 10);
						DBGPRINT(RT_DEBUG_TRACE, "BeaconPeriod=%d\n", pAd->PortCfg.BeaconPeriod);
					}
					//DtimPeriod
					if(RTMPGetKeyParameter("DtimPeriod", tmpbuf, 10, buffer))
					{
						pAd->Mlme.DtimPeriod = (UCHAR) simple_strtol(tmpbuf, 0, 10);
						DBGPRINT(RT_DEBUG_TRACE, "DtimPeriod=%d\n", pAd->Mlme.DtimPeriod);
					}
                    //TxPower
					if(RTMPGetKeyParameter("TxPower", tmpbuf, 10, buffer))
					{
						pAd->PortCfg.TxPowerPercentage = (ULONG ) simple_strtol(tmpbuf, 0, 10);
						DBGPRINT(RT_DEBUG_TRACE, "TxPower=%d\n", pAd->PortCfg.TxPowerPercentage);
					}
					//BGProtection
					if(RTMPGetKeyParameter("BGProtection", tmpbuf, 10, buffer))
					{
#ifndef WIFI_TEST
						pAd->PortCfg.UseBGProtection = 2;// disable b/g protection for throughput test
#else
						switch (simple_strtol(tmpbuf, 0, 10))
						{
							case 1: //Always On
								pAd->PortCfg.UseBGProtection = 1;
								break;
							case 2: //Always OFF
								pAd->PortCfg.UseBGProtection = 2;
								break;
							case 0: //AUTO
							default:
								pAd->PortCfg.UseBGProtection = 0;
								break;
						}
#endif
						DBGPRINT(RT_DEBUG_TRACE, "BGProtection=%d\n", pAd->PortCfg.UseBGProtection);
					}
					//OLBCDetection
					if(RTMPGetKeyParameter("DisableOLBC", tmpbuf, 10, buffer))
					{
						switch (simple_strtol(tmpbuf, 0, 10))
						{
							case 1: //disable OLBC Detection
								pAd->PortCfg.DisableOLBCDetect = 1;
								break;
							case 0: //enable OLBC Detection
								pAd->PortCfg.DisableOLBCDetect = 0;
								break;
							default:
								pAd->PortCfg.DisableOLBCDetect= 0;
								break;
						}
						DBGPRINT(RT_DEBUG_TRACE, "OLBCDetection=%d\n", pAd->PortCfg.DisableOLBCDetect);
					}

					//TxAntenna
					if(RTMPGetKeyParameter("TxAntenna", tmpbuf, 10, buffer))
					{
						if (strlen(tmpbuf) != 0)
							pAd->DefaultTxAntenna = simple_strtol(tmpbuf, 0, 10);
						DBGPRINT(RT_DEBUG_TRACE, "TxAntenna=%d\n", pAd->DefaultTxAntenna);
					}
					//RxAntenna
					if(RTMPGetKeyParameter("RxAntenna", tmpbuf, 10, buffer))
					{
						DBGPRINT(RT_DEBUG_TRACE, "RxAntenna=\n");
					}
					//TxPreamble
					if(RTMPGetKeyParameter("TxPreamble", tmpbuf, 10, buffer))
					{
						switch (simple_strtol(tmpbuf, 0, 10))
						{
							case Rt802_11PreambleShort:
								pAd->PortCfg.TxPreamble = Rt802_11PreambleShort;
								break;
							case Rt802_11PreambleLong:
							default:
								pAd->PortCfg.TxPreamble = Rt802_11PreambleLong;
								break;
						}
						DBGPRINT(RT_DEBUG_TRACE, "TxPreamble=%d\n", pAd->PortCfg.TxPreamble);
					}
					//RTSThreshold
					if(RTMPGetKeyParameter("RTSThreshold", tmpbuf, 10, buffer))
					{
						RtsThresh = simple_strtol(tmpbuf, 0, 10);
						if( (RtsThresh >= 0) && (RtsThresh <= MAX_RTS_THRESHOLD) )
							pAd->PortCfg.RtsThreshold  = (USHORT)RtsThresh;
						else
							pAd->PortCfg.RtsThreshold = MAX_RTS_THRESHOLD;
						
						DBGPRINT(RT_DEBUG_TRACE, "RTSThreshold=%d\n", pAd->PortCfg.RtsThreshold);
					}
					//FragThreshold
					if(RTMPGetKeyParameter("FragThreshold", tmpbuf, 10, buffer))
					{		
						FragThresh = simple_strtol(tmpbuf, 0, 10);
						pAd->PortCfg.bFragmentZeroDisable = FALSE;			

						if (FragThresh > MAX_FRAG_THRESHOLD || FragThresh < MIN_FRAG_THRESHOLD)
						{ //illegal FragThresh so we set it to default
							pAd->PortCfg.FragmentThreshold = MAX_FRAG_THRESHOLD;
							pAd->PortCfg.bFragmentZeroDisable = TRUE;
						}
						else
							pAd->PortCfg.FragmentThreshold = (USHORT)FragThresh;
						pAd->PortCfg.AllowFragSize = (pAd->PortCfg.FragmentThreshold) - LENGTH_802_11 - LENGTH_CRC;

						DBGPRINT(RT_DEBUG_TRACE, "FragThreshold=%d\n", pAd->PortCfg.FragmentThreshold);
					}
					//TxBurst
					if(RTMPGetKeyParameter("TxBurst", tmpbuf, 10, buffer))
					{
#ifdef WIFI_TEST
						pAd->PortCfg.bEnableTxBurst = FALSE;
#else
						if(simple_strtol(tmpbuf, 0, 10) != 0)  //Enable
							pAd->PortCfg.bEnableTxBurst = TRUE;
						else //Disable
							pAd->PortCfg.bEnableTxBurst = FALSE;
#endif
						DBGPRINT(RT_DEBUG_TRACE, "TxBurst=%d\n", pAd->PortCfg.bEnableTxBurst);
					}
#ifdef AGGREGATION_SUPPORT
					//PktAggregate
					if(RTMPGetKeyParameter("PktAggregate", tmpbuf, 10, buffer))
					{
						if(simple_strtol(tmpbuf, 0, 10) != 0)  //Enable
							pAd->PortCfg.bAggregationCapable = TRUE;
						else //Disable
							pAd->PortCfg.bAggregationCapable = FALSE;

						DBGPRINT(RT_DEBUG_TRACE, "PktAggregate=%d\n", pAd->PortCfg.bAggregationCapable);
					}
#else
					pAd->PortCfg.bAggregationCapable = FALSE;
#endif /* !AGGREGATION_SUPPORT */

					//TurboRate
					if(RTMPGetKeyParameter("TurboRate", tmpbuf, 10, buffer))
					{
						if(simple_strtol(tmpbuf, 0, 10) != 0)  //Enable
							pAd->PortCfg.EnableTurboRate = TRUE;
						else //Disable
							pAd->PortCfg.EnableTurboRate = FALSE;
						
						DBGPRINT(RT_DEBUG_TRACE, "TurboRate=%d\n", pAd->PortCfg.EnableTurboRate);
					}

#ifdef WMM_SUPPORT
					rtmp_read_wmm_parms_from_file(pAd, tmpbuf, buffer);
#else
				    for (i = 0; i <MAX_MBSSID_NUM; i++)
				    {
						pAd->PortCfg.MBSSID[i].bWmmCapable = FALSE;
				    }
#endif /* !WMM_SUPPORT */

					//NoForwarding
					if(RTMPGetKeyParameter("NoForwarding", tmpbuf, 32, buffer))
					{
					    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
					    {
    						if(simple_strtol(macptr, 0, 10) != 0)  //Enable
    							pAd->PortCfg.MBSSID[i].IsolateInterStaTraffic = TRUE;
    						else //Disable
    							pAd->PortCfg.MBSSID[i].IsolateInterStaTraffic = FALSE;

    						DBGPRINT(RT_DEBUG_TRACE, "I/F(ra%d) NoForwarding=%d\n", i, pAd->PortCfg.MBSSID[i].IsolateInterStaTraffic);
					    }
					}
					//NoForwardingBTNBSSID
					if(RTMPGetKeyParameter("NoForwardingBTNBSSID", tmpbuf, 10, buffer))
					{
						if(simple_strtol(tmpbuf, 0, 10) != 0)  //Enable
							pAd->PortCfg.IsolateInterStaTrafficBTNBSSID = TRUE;
						else //Disable
							pAd->PortCfg.IsolateInterStaTrafficBTNBSSID = FALSE;

						DBGPRINT(RT_DEBUG_TRACE, "NoForwardingBTNBSSID=%d\n", pAd->PortCfg.IsolateInterStaTrafficBTNBSSID);
					}
					//HideSSID
					if(RTMPGetKeyParameter("HideSSID", tmpbuf, 32, buffer))
					{
						for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
					    {
							if(simple_strtol(macptr, 0, 10) != 0)  //Enable
								pAd->PortCfg.MBSSID[i].bHideSsid = TRUE;
							else //Disable
								pAd->PortCfg.MBSSID[i].bHideSsid = FALSE;

							DBGPRINT(RT_DEBUG_TRACE, "I/F(ra%d) HideSSID=%d\n", i, pAd->PortCfg.MBSSID[i].bHideSsid);
						}
					}
					//ShortSlot
					if(RTMPGetKeyParameter("ShortSlot", tmpbuf, 10, buffer))
					{
						if(simple_strtol(tmpbuf, 0, 10) != 0)  //Enable
							pAd->PortCfg.UseShortSlotTime = TRUE;
						else //Disable
							pAd->PortCfg.UseShortSlotTime = FALSE;

						DBGPRINT(RT_DEBUG_TRACE, "ShortSlot=%d\n", pAd->PortCfg.UseShortSlotTime);
					}
					//AutoChannelSelect
					if(RTMPGetKeyParameter("AutoChannelSelect", tmpbuf, 10, buffer))
					{
						if(simple_strtol(tmpbuf, 0, 10) != 0)  //Enable
							pAd->PortCfg.bAutoChannelAtBootup = TRUE;
						else
							pAd->PortCfg.bAutoChannelAtBootup = FALSE;

						DBGPRINT(RT_DEBUG_TRACE, "AutoChannelAtBootup=%d\n", pAd->PortCfg.bAutoChannelAtBootup);
					}
					//MaxTxPowerLevel
					if (RTMPGetKeyParameter("MaxTxPowerLevel", tmpbuf, 10, buffer))
					{
						if (strlen(tmpbuf) != 0)
							 pAd->PortCfg.MaxTxPowerLevel= simple_strtol(tmpbuf, 0, 10);
								
						DBGPRINT(RT_DEBUG_TRACE, "MaxTxPowerLevel=%d\n", pAd->PortCfg.MaxTxPowerLevel);	
					
					}					
					//IEEE8021X
					if(RTMPGetKeyParameter("IEEE8021X", tmpbuf, 10, buffer))
					{
					    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
					    {
    						if(simple_strtol(macptr, 0, 10) != 0)  //Enable
    							pAd->PortCfg.MBSSID[i].IEEE8021X = TRUE;
    						else //Disable
    							pAd->PortCfg.MBSSID[i].IEEE8021X = FALSE;

    						DBGPRINT(RT_DEBUG_TRACE, "IEEE8021X=%d\n", pAd->PortCfg.MBSSID[i].IEEE8021X);
					    }
					}
					//IEEE80211H
					if(RTMPGetKeyParameter("IEEE80211H", tmpbuf, 10, buffer))
					{
					    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
					    {
    						if(simple_strtol(macptr, 0, 10) != 0)  //Enable
    							pAd->PortCfg.RadarDetect.IEEE80211H = TRUE;
    						else //Disable
    							pAd->PortCfg.RadarDetect.IEEE80211H = FALSE;

    						DBGPRINT(RT_DEBUG_TRACE, "IEEE80211H=%d\n", pAd->PortCfg.RadarDetect.IEEE80211H);
					    }
					}
					//CSPeriod
					if(RTMPGetKeyParameter("CSPeriod", tmpbuf, 10, buffer))
					{
					    if(simple_strtol(tmpbuf, 0, 10) != 0)
							pAd->PortCfg.RadarDetect.CSPeriod = simple_strtol(tmpbuf, 0, 10);
						else
							pAd->PortCfg.RadarDetect.CSPeriod = 0;

   						DBGPRINT(RT_DEBUG_TRACE, "CSPeriod=%d\n", pAd->PortCfg.RadarDetect.CSPeriod);
					}
					//PreAuth
					if(RTMPGetKeyParameter("PreAuth", tmpbuf, 10, buffer))
					{
					    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
					    {
    						if(simple_strtol(macptr, 0, 10) != 0)  //Enable
    							pAd->PortCfg.MBSSID[i].PreAuth = TRUE;
    						else //Disable
    							pAd->PortCfg.MBSSID[i].PreAuth = FALSE;

    						DBGPRINT(RT_DEBUG_TRACE, "PreAuth=%d\n", pAd->PortCfg.MBSSID[i].PreAuth);
					    }
					}
					//AuthMode
					if(RTMPGetKeyParameter("AuthMode", tmpbuf, 128, buffer))
					{
					    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
					    {
					    	if ((strcmp(macptr, "WEPAUTO") == 0) || (strcmp(macptr, "wepauto") == 0))
					    		pAd->PortCfg.MBSSID[i].AuthMode = Ndis802_11AuthModeAutoSwitch;
    						else if ((strcmp(macptr, "OPEN") == 0) || (strcmp(macptr, "open") == 0))
    							pAd->PortCfg.MBSSID[i].AuthMode = Ndis802_11AuthModeOpen;
    						else if ((strcmp(macptr, "SHARED") == 0) || (strcmp(macptr, "shared") == 0))
    							pAd->PortCfg.MBSSID[i].AuthMode = Ndis802_11AuthModeShared;
    						else if ((strcmp(macptr, "WPAPSK") == 0) || (strcmp(macptr, "wpapsk") == 0))
    							pAd->PortCfg.MBSSID[i].AuthMode = Ndis802_11AuthModeWPAPSK;
    						else if ((strcmp(macptr, "WPA2PSK") == 0) || (strcmp(macptr, "wpa2psk") == 0))
    							pAd->PortCfg.MBSSID[i].AuthMode = Ndis802_11AuthModeWPA2PSK;
    						else if ((strcmp(macptr, "WPAPSKWPA2PSK") == 0) || (strcmp(macptr, "wpapskwpa2psk") == 0))
    							pAd->PortCfg.MBSSID[i].AuthMode = Ndis802_11AuthModeWPAPSKWPA2PSK;
    						else if ((strcmp(macptr, "WPA") == 0) || (strcmp(macptr, "wpa") == 0))
    							pAd->PortCfg.MBSSID[i].AuthMode = Ndis802_11AuthModeWPA;
    						else if ((strcmp(macptr, "WPA2") == 0) || (strcmp(macptr, "wpa2") == 0))
    							pAd->PortCfg.MBSSID[i].AuthMode = Ndis802_11AuthModeWPA2;
    						else if ((strcmp(macptr, "WPA1WPA2") == 0) || (strcmp(macptr, "wpa1wpa2") == 0))
    							pAd->PortCfg.MBSSID[i].AuthMode = Ndis802_11AuthModeWPA1WPA2;
    						else //Default
    							pAd->PortCfg.MBSSID[i].AuthMode = Ndis802_11AuthModeOpen;

    						RTMPMakeRSNIE(pAd, pAd->PortCfg.MBSSID[i].AuthMode, pAd->PortCfg.MBSSID[i].WepStatus, i);
    						DBGPRINT(RT_DEBUG_TRACE, "I/F(ra%d) AuthMode=%d\n", i, pAd->PortCfg.MBSSID[i].AuthMode);
					    }
					}
					//EncrypType
					if(RTMPGetKeyParameter("EncrypType", tmpbuf, 128, buffer))
					{
					    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
					    {
					        if ((strcmp(macptr, "NONE") == 0) || (strcmp(macptr, "none") == 0))
					            pAd->PortCfg.MBSSID[i].WepStatus = Ndis802_11WEPDisabled;
					        else if ((strcmp(macptr, "WEP") == 0) || (strcmp(macptr, "wep") == 0))
					            pAd->PortCfg.MBSSID[i].WepStatus = Ndis802_11WEPEnabled;
					        else if ((strcmp(macptr, "TKIP") == 0) || (strcmp(macptr, "tkip") == 0))
					            pAd->PortCfg.MBSSID[i].WepStatus = Ndis802_11Encryption2Enabled;
					        else if ((strcmp(macptr, "AES") == 0) || (strcmp(macptr, "aes") == 0))
					            pAd->PortCfg.MBSSID[i].WepStatus = Ndis802_11Encryption3Enabled;
					        else if ((strcmp(macptr, "TKIPAES") == 0) || (strcmp(macptr, "tkipaes") == 0))
					            pAd->PortCfg.MBSSID[i].WepStatus = Ndis802_11Encryption4Enabled;
					        else
					            pAd->PortCfg.MBSSID[i].WepStatus = Ndis802_11WEPDisabled;

					        RTMPMakeRSNIE(pAd, pAd->PortCfg.MBSSID[i].AuthMode, pAd->PortCfg.MBSSID[i].WepStatus, i);
					        DBGPRINT(RT_DEBUG_TRACE, "I/F(ra%d) EncrypType=%d\n", i, pAd->PortCfg.MBSSID[i].WepStatus);
					    }
					}
#ifdef APCLI_SUPPORT
					//KH: AuthMode for AP Client
					if(RTMPGetKeyParameter("ApCliAuthMode", tmpbuf, 128, buffer))
					{
						//macptr =simple_strtol(tmpbuf, 0, 10);
						DBGPRINT(RT_DEBUG_TRACE, "In ApCliAuthMode=\n");
						if ((strcmp(tmpbuf, "WEPAUTO") == 0) || (strcmp(tmpbuf, "wepauto") == 0))
							pAd->ApCliTab.ApCliEntry[0].AuthMode = Ndis802_11AuthModeAutoSwitch;
    					else if ((strcmp(tmpbuf, "OPEN") == 0) || (strcmp(tmpbuf, "open") == 0))
    						pAd->ApCliTab.ApCliEntry[0].AuthMode = Ndis802_11AuthModeOpen;
    					else if ((strcmp(tmpbuf, "SHARED") == 0) || (strcmp(tmpbuf, "shared") == 0))
    						pAd->ApCliTab.ApCliEntry[0].AuthMode = Ndis802_11AuthModeShared;
    					else if ((strcmp(tmpbuf, "WPAPSK") == 0) || (strcmp(tmpbuf, "wpapsk") == 0))
    						pAd->ApCliTab.ApCliEntry[0].AuthMode = Ndis802_11AuthModeWPAPSK;
    					else if ((strcmp(tmpbuf, "WPA2PSK") == 0) || (strcmp(tmpbuf, "wpa2psk") == 0))
    						pAd->ApCliTab.ApCliEntry[0].AuthMode = Ndis802_11AuthModeWPA2PSK;						
    					else //Default
    						pAd->ApCliTab.ApCliEntry[0].AuthMode = Ndis802_11AuthModeOpen;
						RTMPApCliMakeRSNIE(pAd, pAd->ApCliTab.ApCliEntry[0].AuthMode, pAd->ApCliTab.ApCliEntry[0].PairCipher, 0);
						DBGPRINT(RT_DEBUG_TRACE, "KH:ApCliAuthMode=%d and RSNIE=%x\n",pAd->ApCliTab.ApCliEntry[0].AuthMode,*pAd->PortCfg.MBSSID[MAIN_MBSSID].RSN_IE[0]);
					}
					//EncrypType for AP Client
					if(RTMPGetKeyParameter("ApCliEncrypType", tmpbuf, 128, buffer))
					{
					    if ((strcmp(tmpbuf, "NONE") == 0) || (strcmp(tmpbuf, "none") == 0))
					        pAd->ApCliTab.ApCliEntry[0].PairCipher = Ndis802_11WEPDisabled;
					    else if ((strcmp(tmpbuf, "WEP") == 0) || (strcmp(tmpbuf, "wep") == 0))
					        pAd->ApCliTab.ApCliEntry[0].PairCipher = Ndis802_11WEPEnabled;
					    else if ((strcmp(tmpbuf, "TKIP") == 0) || (strcmp(tmpbuf, "tkip") == 0))
					        pAd->ApCliTab.ApCliEntry[0].PairCipher  = Ndis802_11Encryption2Enabled;
					    else if ((strcmp(tmpbuf, "AES") == 0) || (strcmp(tmpbuf, "aes") == 0))
					        pAd->ApCliTab.ApCliEntry[0].PairCipher  = Ndis802_11Encryption3Enabled;			        
					    else if ((strcmp(tmpbuf, "TKIPAES") == 0) || (strcmp(tmpbuf, "tkipaes") == 0))
					        pAd->ApCliTab.ApCliEntry[0].PairCipher  = Ndis802_11Encryption4Enabled;		        
					    else
					        pAd->ApCliTab.ApCliEntry[0].PairCipher  = Ndis802_11WEPDisabled;
						
						pAd->ApCliTab.ApCliEntry[0].GroupCipher=pAd->ApCliTab.ApCliEntry[0].PairCipher; 						  
					    RTMPApCliMakeRSNIE(pAd, pAd->ApCliTab.ApCliEntry[0].AuthMode, pAd->ApCliTab.ApCliEntry[0].PairCipher, 0);					        
						DBGPRINT(RT_DEBUG_TRACE, "I/F(ra%d) EncrypType=%d\n", i, pAd->ApCliTab.ApCliEntry[0].AuthMode);    
					}
#endif
					//RekeyMethod
					if(RTMPGetKeyParameter("RekeyMethod", tmpbuf, 128, buffer))
					{
						if ((strcmp(tmpbuf, "TIME") == 0) || (strcmp(tmpbuf, "time") == 0))
							pAd->PortCfg.WPAREKEY.ReKeyMethod = TIME_REKEY;
						else if ((strcmp(tmpbuf, "PKT") == 0) || (strcmp(tmpbuf, "pkt") == 0))
							pAd->PortCfg.WPAREKEY.ReKeyMethod = PKT_REKEY;
						else if ((strcmp(tmpbuf, "DISABLE") == 0) || (strcmp(tmpbuf, "disable") == 0))
							pAd->PortCfg.WPAREKEY.ReKeyMethod = DISABLE_REKEY;
						else
							pAd->PortCfg.WPAREKEY.ReKeyMethod = DISABLE_REKEY;

						DBGPRINT(RT_DEBUG_TRACE, "ReKeyMethod=%d\n", pAd->PortCfg.WPAREKEY.ReKeyMethod);
					}
					//RekeyInterval
					if(RTMPGetKeyParameter("RekeyInterval", tmpbuf, 255, buffer))
					{
						if((simple_strtol(tmpbuf, 0, 10) >= 0) && (simple_strtol(tmpbuf, 0, 10) < MAX_REKEY_INTER))
							pAd->PortCfg.WPAREKEY.ReKeyInterval = simple_strtol(tmpbuf, 0, 10);
						else //Default
							pAd->PortCfg.WPAREKEY.ReKeyInterval = 10;

						DBGPRINT(RT_DEBUG_TRACE, "ReKeyInterval=%d\n", pAd->PortCfg.WPAREKEY.ReKeyInterval);
					}
					//PMKCachePeriod
					if(RTMPGetKeyParameter("PMKCachePeriod", tmpbuf, 255, buffer))
					{
						pAd->PortCfg.PMKCachePeriod = simple_strtol(tmpbuf, 0, 10) * 60 * HZ;

						DBGPRINT(RT_DEBUG_TRACE, "PMKCachePeriod=%d\n", pAd->PortCfg.PMKCachePeriod);
					}
					//WPAPSK_KEY
					if(RTMPGetKeyParameter("WPAPSK", tmpbuf, 512, buffer))
					{
					    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
					    {
					        DBGPRINT(RT_DEBUG_TRACE, "I/F(ra%d) WPAPSK_KEY=%s\n", i, macptr);
                            if (strlen(macptr) == 64)
						    {// Hex mode
						        AtoH(macptr, pAd->PortCfg.MBSSID[i].PMK, 32);
						    }
						    else if ((strlen(macptr) >= 8) && (strlen(macptr) < 64))
						    {// ASCII mode
							    PasswordHash((char *)macptr, pAd->PortCfg.MBSSID[i].Ssid, pAd->PortCfg.MBSSID[i].SsidLen, keyMaterial);
							    NdisMoveMemory(pAd->PortCfg.MBSSID[i].PMK, keyMaterial, 32);
						    }
						    else
						    	DBGPRINT(RT_DEBUG_ERROR, "WPAPSK_KEY, key len incorrect!!!\n");

#ifdef DBG
                            DBGPRINT(RT_DEBUG_TRACE, "I/F(ra%d) WPAPSK Key => \n", i);
                            for (j = 0; j < 32; j++)
                            {
                                printk("%02x:", pAd->PortCfg.MBSSID[i].PMK[j]);
                                if ((j%16) == 15)
                                    printk("\n");
                            }
                            printk("\n");
#endif

#ifdef WSC_SUPPORT
                            NdisZeroMemory(pAd->PortCfg.MBSSID[i].WscControl.WpaPsk, 64);
                            pAd->PortCfg.MBSSID[i].WscControl.WpaPskLen = 0;
                            if ((strlen(macptr) >= 8) && (strlen(macptr) <= 64))
                            {                                    
                                NdisMoveMemory(pAd->PortCfg.MBSSID[i].WscControl.WpaPsk, macptr, strlen(macptr));
                                pAd->PortCfg.MBSSID[i].WscControl.WpaPskLen = strlen(macptr);
                            }
#endif // WSC_SUPPORT //
					    }
					}
    							
					//DefaultKeyID, KeyType, KeyStr
					rtmp_read_key_parms_from_file(pAd, tmpbuf, buffer);
    							
					//HSCounter
					if(RTMPGetKeyParameter("HSCounter", tmpbuf, 10, buffer))
					{
						switch (simple_strtol(tmpbuf, 0, 10))
						{
							case 1: //Enable
								pAd->PortCfg.bEnableHSCounter = TRUE;
								break;
							case 0: //Disable
							default:
								pAd->PortCfg.bEnableHSCounter = FALSE;
								break;
						}
						DBGPRINT(RT_DEBUG_TRACE, "HSCounter=%d\n", pAd->PortCfg.bEnableHSCounter);
					}

					//Access Control List
					rtmp_read_acl_parms_from_file(pAd, tmpbuf, buffer);
#ifdef APCLI_SUPPORT					
					// Ap Client information
					rtmp_read_ap_client_from_file(pAd, tmpbuf, buffer);
#endif 												
#ifdef IGMP_SNOOP_SUPPORT
					// Igmp Snooping information
					rtmp_read_igmp_snoop_from_file(pAd, tmpbuf, buffer);
#endif // IGMP_SNOOP_SUPPORT //
#ifdef WDS_SUPPORT								
					//WdsList
					if(RTMPGetKeyParameter("WdsList", tmpbuf, MAX_PARAM_BUFFER_SIZE, buffer))
					{
						for (i=0, macptr = rstrtok(tmpbuf,";"); (macptr && i < MAX_WDS_ENTRY); macptr = rstrtok(NULL,";"), i++) 
						{
							if(strlen(macptr) != 17)  //Mac address acceptable format 01:02:03:04:05:06 length 17
								continue; 
							if(strcmp(macptr,"00:00:00:00:00:00") == 0)
								continue; 
							if(i >= MAX_WDS_ENTRY)
								break; 

							for (j=0; j<ETH_LENGTH_OF_ADDRESS; j++)
							{
								AtoH(macptr, &macAddress[j], 2);
								macptr=macptr+3;
							}	
							memcpy(&pAd->WdsTab.WdsEntry[i].WdsAddr[0], &macAddress, ETH_LENGTH_OF_ADDRESS);
							pAd->WdsTab.WdsEntry[i].Valid = TRUE;
							pAd->WdsTab.WdsEntry[i].RestrictEntry = TRUE;
						}
					}
					//WdsEncrypType
					if(RTMPGetKeyParameter("WdsEncrypType", tmpbuf, 128, buffer))
					{
						if ((strncmp(tmpbuf, "NONE", 4) == 0) || (strncmp(tmpbuf, "none", 4) == 0))
					        pAd->WdsTab.WepStatus = Ndis802_11WEPDisabled;
						else if ((strncmp(tmpbuf, "WEP", 3) == 0) || (strncmp(tmpbuf, "wep", 3) == 0))
					        pAd->WdsTab.WepStatus = Ndis802_11WEPEnabled;
					    else if ((strncmp(tmpbuf, "TKIP", 4) == 0) || (strncmp(tmpbuf, "tkip", 4) == 0))
					        pAd->WdsTab.WepStatus = Ndis802_11Encryption2Enabled;
					    else if ((strncmp(tmpbuf, "AES", 3) == 0) || (strncmp(tmpbuf, "aes", 3) == 0))
					        pAd->WdsTab.WepStatus = Ndis802_11Encryption3Enabled;
					    else
					        pAd->WdsTab.WepStatus = Ndis802_11WEPDisabled;

					    DBGPRINT(RT_DEBUG_TRACE, "WdsEncrypType=%d\n", pAd->WdsTab.WepStatus);
					}
					//WdsKey					
					if(RTMPGetKeyParameter("WdsKey", tmpbuf, 255, buffer))
					{
						NdisZeroMemory(&pAd->WdsTab.Wpa_key, sizeof(CIPHER_KEY));

						if (((pAd->WdsTab.WepStatus == Ndis802_11Encryption2Enabled) || (pAd->WdsTab.WepStatus == Ndis802_11Encryption3Enabled))
							&& (strlen(tmpbuf) >= 8) && (strlen(tmpbuf) <= 64))
						{
							if(strlen(tmpbuf)==64)//hex
								AtoH(tmpbuf, keyMaterial, 32);
							else//ascii
								PasswordHash((char *)tmpbuf, RALINK_PASSPHRASE, sizeof(RALINK_PASSPHRASE), keyMaterial);

							if (pAd->WdsTab.WepStatus == Ndis802_11Encryption3Enabled)
								pAd->WdsTab.Wpa_key.CipherAlg = CIPHER_AES;
							else
								pAd->WdsTab.Wpa_key.CipherAlg = CIPHER_TKIP;
							
							NdisMoveMemory(&pAd->WdsTab.Wpa_key.Key, keyMaterial, 16);
							pAd->WdsTab.Wpa_key.KeyLen = 16;
							NdisMoveMemory(&pAd->WdsTab.Wpa_key.RxMic, keyMaterial+16, 8);
							NdisMoveMemory(&pAd->WdsTab.Wpa_key.TxMic, keyMaterial+16, 8);

#ifdef DBG
							DBGPRINT(RT_DEBUG_TRACE, "WdsTab.Wpa_key=>\n");
							for (i=0; i<16; i++)
								printk("%02x:", pAd->WdsTab.Wpa_key.Key[i]);
							printk("\nRxMic Key=>\n");
							for (i=0; i<8; i++)
								printk("%02x:", pAd->WdsTab.Wpa_key.RxMic[i]);
							printk("\nTxMic Key=>\n");
							for (i=0; i<8; i++)
								printk("%02x:", pAd->WdsTab.Wpa_key.TxMic[i]);
							printk("\n");
#endif
						}
					}
					//WdsEnable
					if(RTMPGetKeyParameter("WdsEnable", tmpbuf, 10, buffer))
					{						
                        RT_802_11_WDS_ENTRY *pEntry;	
						switch(simple_strtol(tmpbuf, 0, 10))
						{
						    case 1: // Restricted mode
								pAd->WdsTab.Mode = WDS_RESTRICT_MODE;
								break;
						    case 2: // Bridge mode, DisAllow association(stop Beacon generation and Probe Req.)
								pAd->WdsTab.Mode = WDS_BRIDGE_MODE;
								break;
						    case 3: // Repeater mode
								pAd->WdsTab.Mode = WDS_REPEATER_MODE;
								break;
							case 4: // Lazy mode, Auto learn wds entry by same SSID, channel, security policy
							    for(i = 0; i < MAX_WDS_ENTRY; i++)
						            {
									pEntry = &pAd->WdsTab.WdsEntry[i];
						           	pEntry->Valid = FALSE;
							    }
								pAd->WdsTab.Mode = WDS_LAZY_MODE;
								break;
						    case 0: // Disable mode
						    default:
							    RT61_Init_WDSTAB(pAd);
							    pAd->WdsTab.Mode = WDS_DISABLE_MODE;
							   	break;
						}

						DBGPRINT(RT_DEBUG_TRACE,"WDS-Enable mode=%d\n", pAd->WdsTab.Mode);

#ifdef DBG
						for(i = 0; i < MAX_WDS_ENTRY; i++)
						{
							pEntry = &pAd->WdsTab.WdsEntry[i];
							DBGPRINT(RT_DEBUG_TRACE,"WDS-AP(%02d) (%d)-%02x:%02x:%02x:%02x:%02x:%02x\n", i, pEntry->Valid,
								pEntry->WdsAddr[0], pEntry->WdsAddr[1], pEntry->WdsAddr[2], 
								pEntry->WdsAddr[3], pEntry->WdsAddr[4], pEntry->WdsAddr[5]);
						}
#endif /* DBG */
					}
#else
					pAd->WdsTab.Mode = WDS_DISABLE_MODE;
#endif					 

#ifdef CARRIER_DETECTION_SUPPORT
					//CarrierDetect
					if(RTMPGetKeyParameter("CarrierDetect", tmpbuf, 128, buffer))
					{
						if ((strncmp(tmpbuf, "0", 1) == 0))
							pAd->PortCfg.CarrierDetect.Enable = FALSE;
						else if ((strncmp(tmpbuf, "1", 1) == 0))
							pAd->PortCfg.CarrierDetect.Enable = TRUE;
						else
							pAd->PortCfg.CarrierDetect.Enable = FALSE;

						DBGPRINT(RT_DEBUG_TRACE,"CarrierDetect.Enable=%d\n", pAd->PortCfg.CarrierDetect.Enable);
					}
#endif

#ifdef WSC_SUPPORT
					//WscConfMode
					if(RTMPGetKeyParameter("WscConfMode", tmpbuf, 10, buffer))
					{
						INT WscConfMode = simple_strtol(tmpbuf, 0, 10);

						if (WscConfMode >= 0 && WscConfMode < 8)
						{
							pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.WscConfMode = WscConfMode;
						}
						else
						{
								pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.WscConfMode = WSC_DISABLE;
						}

						DBGPRINT(RT_DEBUG_TRACE, "WscConfMode=%d\n", pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.WscConfMode);
					}

					//WscConfStatus
					if(RTMPGetKeyParameter("WscConfStatus", tmpbuf, 10, buffer))
					{
						pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.WscConfStatus = (INT) simple_strtol(tmpbuf, 0, 10);

						DBGPRINT(RT_DEBUG_TRACE, "WscConfStatus=%d\n", pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.WscConfStatus);
					}

					//WscUseUPnP
					if(RTMPGetKeyParameter("WscUseUpnp", tmpbuf, 10, buffer))
					{
						pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.WscUseUPnP = (INT) simple_strtol(tmpbuf, 0, 10);

						DBGPRINT(RT_DEBUG_TRACE, "WscUseUpnp=%d\n", pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.WscUseUPnP);
					}
					// For Re-Generate Pin-Code Modification
					//WscPinCode
					if(RTMPGetKeyParameter("WscPinCode", tmpbuf, 10, buffer))
					{
						pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.WscEnrolleePinCode = (INT) simple_strtol(tmpbuf, 0, 10);
						if (ValidateChecksum(pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.WscEnrolleePinCode) == FALSE)
						{
							pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.WscEnrolleePinCode = 0;
							DBGPRINT(RT_DEBUG_TRACE, "ApCliWscPinCode InValid!!\n");
						}
						else
						    DBGPRINT(RT_DEBUG_TRACE, "WscPinCode=%d\n", pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.WscEnrolleePinCode);
					}
					// --
#endif // WSC_SUPPORT //
				}
			}
			else
			{
				DBGPRINT(RT_DEBUG_TRACE, "--> %s does not have a write method\n", src);
			}
			
			retval=filp_close(srcf,NULL);
			
			if (retval)
			{
				DBGPRINT(RT_DEBUG_TRACE, "--> Error %d closing %s\n", -retval, src);
			}
		}
	}

	set_fs(orgfs);
	current->fsuid = orgfsuid;
	current->fsgid = orgfsgid;

	kfree(buffer);
	kfree(tmpbuf);
}

/*
	========================================================================
	
	Routine Description:
		Load 8051 firmware RT2561.BIN file into MAC ASIC

	Arguments:
		Adapter						Pointer to our adapter

	Return Value:
		NDIS_STATUS_SUCCESS         firmware image load ok
		NDIS_STATUS_FAILURE         image not found

	========================================================================
*/
NDIS_STATUS NICLoadFirmware(
    IN PRTMP_ADAPTER pAd)
{
	NDIS_STATUS				Status = NDIS_STATUS_SUCCESS;
	PUCHAR					src;
	struct file				*srcf;
	INT 					retval, orgfsuid, orgfsgid, i;
   	mm_segment_t			orgfs;
	PUCHAR					pFirmwareImage;
	UINT					FileLength;
	ULONG 					MacReg;
	ULONG 					Index;
	
	DBGPRINT(RT_DEBUG_TRACE,"===> NICLoadFirmware\n");

	if (pAd->pPci_Dev->device == NIC2561_PCI_DEVICE_ID)
        src = RT2561_IMAGE_FILE_NAME;
	else if (pAd->pPci_Dev->device == NIC2561S_PCI_DEVICE_ID)
        src = RT2561S_IMAGE_FILE_NAME;
    else if (pAd->pPci_Dev->device == NIC2661_PCI_DEVICE_ID)
        src = RT2661_IMAGE_FILE_NAME;
    else
    {
        DBGPRINT_ERR("NICLoadFirmware: wrong DeviceID = 0x%04x, can't find firmware\n", pAd->pPci_Dev->device);
        return NDIS_STATUS_FAILURE;
    }
   	
	// Save uid and gid used for filesystem access.
	// Set user and group to 0 (root)	
	orgfsuid = current->fsuid;
	orgfsgid = current->fsgid;
	current->fsuid=current->fsgid = 0;
    orgfs = get_fs();
    set_fs(KERNEL_DS);

    pFirmwareImage = kmalloc(MAX_FIRMWARE_IMAGE_SIZE, MEM_ALLOC_FLAG);
    if (pFirmwareImage == NULL) 
    {
    	set_fs(orgfs);
		current->fsuid = orgfsuid;
		current->fsgid = orgfsgid;
		DBGPRINT(RT_DEBUG_ERROR, "NICLoadFirmware-Memory allocate fail\n");
    	return NDIS_STATUS_FAILURE;
    }

	if (src && *src) 
	{
		srcf = filp_open(src, O_RDONLY, 0);
		if (IS_ERR(srcf)) 
		{
			Status = NDIS_STATUS_FAILURE;
			DBGPRINT(RT_DEBUG_ERROR, "--> Error %ld opening %s\n", -PTR_ERR(srcf),src);    
		}
		else 
		{
			// The object must have a read method
			if (srcf->f_op && srcf->f_op->read) 
			{
				memset(pFirmwareImage, 0x00, MAX_FIRMWARE_IMAGE_SIZE);

				FileLength = srcf->f_op->read(srcf, pFirmwareImage, MAX_FIRMWARE_IMAGE_SIZE, &srcf->f_pos);
				if (FileLength != MAX_FIRMWARE_IMAGE_SIZE)
				{
					DBGPRINT_ERR("NICLoadFirmware: error file length (=%d) in RT2561.BIN\n",FileLength);
				}
				else
				{
					// hold 8051 in reset state
					RTMP_IO_WRITE32(pAd, MCU_CNTL_CSR, 0x02);
					RTMP_IO_WRITE32(pAd, M2H_CMD_DONE_CSR, 0xffffffff);    // clear all CmdToken
					RTMP_IO_WRITE32(pAd, H2M_MAILBOX_CSR, 0x00000000);     // MBOX owned by HOST
					RTMP_IO_WRITE32(pAd, HOST_CMD_CSR, 0x00000000);        // 

					if (FileLength == MAX_FIRMWARE_IMAGE_SIZE)
					{
						PUCHAR ptr = pFirmwareImage;
						USHORT crc = 0;
						
						for (i=0; i<(MAX_FIRMWARE_IMAGE_SIZE-2); i++, ptr++)
							crc = ByteCRC16(*ptr, crc);
						crc = ByteCRC16(0x00, crc);
						crc = ByteCRC16(0x00, crc);
						
						if ((pFirmwareImage[MAX_FIRMWARE_IMAGE_SIZE-2] != (UCHAR)(crc>>8)) ||
							(pFirmwareImage[MAX_FIRMWARE_IMAGE_SIZE-1] != (UCHAR)(crc)))
						{
							DBGPRINT_ERR("NICLoadFirmware: CRC = 0x%02x 0x%02x error, should be 0x%02x 0x%02x\n",
								pFirmwareImage[MAX_FIRMWARE_IMAGE_SIZE-2], pFirmwareImage[MAX_FIRMWARE_IMAGE_SIZE-1],
								(UCHAR)(crc>>8), (UCHAR)(crc) );

							retval = filp_close(srcf, NULL);
							if (retval)
							{
								DBGPRINT(RT_DEBUG_ERROR, "--> Error %d closing %s\n", -retval, src);
							}

							kfree(pFirmwareImage);

							set_fs(orgfs);
							current->fsuid = orgfsuid;
							current->fsgid = orgfsgid;
							
							return NDIS_STATUS_FAILURE;
						}
						DBGPRINT(RT_DEBUG_TRACE,"NICLoadFirmware: CRC ok, ver=%d.%d\n",
							pFirmwareImage[MAX_FIRMWARE_IMAGE_SIZE-4], pFirmwareImage[MAX_FIRMWARE_IMAGE_SIZE-3]);
					}

					// select 8051 program bank; write entire firmware image
					RTMP_IO_WRITE32(pAd, MCU_CNTL_CSR, 0x03); 
					for (i=0; i<FileLength; i++)
					{
						RTMP_IO_WRITE8(pAd, FIRMWARE_IMAGE_BASE + i, pFirmwareImage[i]);
					}

					// de-select 8051 program bank
					RTMP_IO_WRITE32(pAd, MCU_CNTL_CSR, 0x02);

					// 8051 get out of RESET state
					RTMP_IO_WRITE32(pAd, MCU_CNTL_CSR, 0x00);
				}
			}
			else
			{
				Status = NDIS_STATUS_FAILURE;
				DBGPRINT(RT_DEBUG_ERROR, "--> %s does not have a write method\n", src);
			}
			
			retval = filp_close(srcf, NULL);			
			if (retval)
			{
				Status = NDIS_STATUS_FAILURE;
				DBGPRINT(RT_DEBUG_ERROR, "--> Error %d closing %s\n", -retval, src);
			}
		}
	}
	else
	{
		Status = NDIS_STATUS_FAILURE;
		DBGPRINT(RT_DEBUG_ERROR, "Error src not available\n");
	}
	
	kfree(pFirmwareImage);

	set_fs(orgfs);
	current->fsuid = orgfsuid;
	current->fsgid = orgfsgid;

	Index = 0;
	do
	{
		RTMP_IO_READ32(pAd, MCU_CNTL_CSR, &MacReg);

		if (MacReg & 0x04)
			break;
		
		RTMPusecDelay(1000);
	} while (Index++ < 1000);

    if (Index >= 1000)
	{
		Status = NDIS_STATUS_FAILURE;
		DBGPRINT(RT_DEBUG_ERROR,"NICLoadFirmware: MCU is not ready\n\n\n");
	}

    DBGPRINT(RT_DEBUG_TRACE,"<=== NICLoadFirmware (src=%s, status=%d)\n", src, Status);
    
    return Status;
}

/*
    ========================================================================

    Routine Description:
        Reset NIC Asics

    Arguments:
        Adapter                     Pointer to our adapter

    Return Value:
        None

    Note:
        Reset NIC to initial state AS IS system boot up time.

    ========================================================================
*/
VOID    RTMPRingCleanUp(
    IN  PRTMP_ADAPTER   pAdapter,
    IN  UCHAR           RingType)
{
	PTXD_STRUC		pTxD;
	PRXD_STRUC		pRxD;
#ifdef	BIG_ENDIAN
    PTXD_STRUC      pDestTxD;
    TXD_STRUC       TxD;
    PRXD_STRUC      pDestRxD;
    RXD_STRUC       RxD;
#endif

	struct sk_buff  *pSkb;
	INT             i;
	PRTMP_TX_RING   pTxRing;
	UMAXLONG		IrqFlags;

    DBGPRINT(RT_DEBUG_TRACE,"RTMPRingCleanUp(RingIdx=%d, p-NDIS=%ld)\n", RingType, pAdapter->RalinkCounters.PendingNdisPacketCount);

	switch (RingType)
	{
		case QID_AC_BK:
		case QID_AC_BE:
		case QID_AC_VI:
		case QID_AC_VO:
		case QID_HCCA:

#ifndef THREAD_ISR
		    RTMP_SEM_LOCK(&pAdapter->TxRingLock, IrqFlags);
#else
			RTMP_IRQ_LOCK(IrqFlags);
#endif	/* THREAD_ISR */

		    pTxRing = &pAdapter->TxRing[RingType];
			// We have to clean all descriptors in case some error happened with reset
			for (i=0; i<TX_RING_SIZE; i++) // We have to scan all TX ring
			{
#ifndef BIG_ENDIAN
				pTxD  = (PTXD_STRUC) pTxRing->Cell[i].AllocVa;
#else
		        pDestTxD = (PTXD_STRUC) pTxRing->Cell[i].AllocVa;
		        TxD = *pDestTxD;
		        pTxD = &TxD;
		        RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
#endif

				// release scatter-and-gather NDIS_PACKET
#ifdef CONFIG_64BIT
                if (pTxRing->Cell[i].pSkb)
				{
                    RELEASE_NDIS_PACKET(pAdapter, pTxRing->Cell[i].pSkb);
                    pTxRing->Cell[i].pSkb = 0;
                    DBGPRINT(RT_DEBUG_TRACE,"Release 1 skb buffer at TxRing%d[%d]=%d.%d\n", RingType, i, pTxD->Owner, pTxD->bWaitingDmaDoneInt);
				}
                if (pTxRing->Cell[i].pNextSkb)
                {
                    RELEASE_NDIS_PACKET(pAdapter, pTxRing->Cell[i].pNextSkb);
                    pTxRing->Cell[i].pNextSkb = 0;
                    DBGPRINT(RT_DEBUG_TRACE,"Release 1 skb buffer at TxRing%d[%d]\n", RingType, i);
                }
#else
				if (pTxD->pSkb)
				{
                    RELEASE_NDIS_PACKET(pAdapter, pTxD->pSkb);
                    pTxD->pSkb = 0;
                    DBGPRINT(RT_DEBUG_TRACE,"Release 1 skb buffer at TxRing%d[%d]=%d.%d\n", RingType, i, pTxD->Owner, pTxD->bWaitingDmaDoneInt);
				}

                // release scatter-and-gather NDIS_PACKET
                if (pTxD->pNextSkb)
                {
                    RELEASE_NDIS_PACKET(pAdapter, pTxD->pNextSkb);
                    pTxD->pNextSkb = 0;
                    DBGPRINT(RT_DEBUG_TRACE,"Release 1 skb buffer at TxRing%d[%d]\n", RingType, i);
                }
#endif // CONFIG_64BIT

				pTxD->Owner = DESC_OWN_HOST;
                pTxD->bWaitingDmaDoneInt = 0;
#ifdef BIG_ENDIAN
		        RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
        		WriteBackToDescriptor((PUCHAR)pDestTxD, (PUCHAR)pTxD, FALSE, TYPE_TXD);
#endif
			}
            pTxRing->CurTxIndex = 0;
            pTxRing->NextTxDmaDoneIndex = 0;
#ifndef THREAD_ISR
		    RTMP_SEM_UNLOCK(&pAdapter->TxRingLock, IrqFlags);
#else
			RTMP_IRQ_UNLOCK(IrqFlags);
#endif	/* THREAD_ISR */

#ifndef THREAD_ISR
            RTMP_SEM_LOCK(&pAdapter->TxSwQueueLock, IrqFlags);
#else
			RTMP_IRQ_LOCK(IrqFlags);
#endif	/* THREAD_ISR */

            while (pAdapter->TxSwQueue[RingType].Head != NULL)
            {
                pSkb = (struct sk_buff *)RemoveHeadQueue(&pAdapter->TxSwQueue[RingType]);
                RELEASE_NDIS_PACKET(pAdapter, pSkb);
                DBGPRINT(RT_DEBUG_TRACE,"Release 1 skb buffer from s/w backlog queue\n");
            }

            while (pAdapter->NullFrameTxSwQueue.Head != NULL)
            {
                pSkb = (struct sk_buff *)RemoveHeadQueue(&pAdapter->NullFrameTxSwQueue);
                RELEASE_NDIS_PACKET(pAdapter, pSkb);
                DBGPRINT(RT_DEBUG_TRACE,"Release 1 skb buffer from s/w backlog queue\n");
            }

#ifndef THREAD_ISR
            RTMP_SEM_UNLOCK(&pAdapter->TxSwQueueLock, IrqFlags);
#else
			RTMP_IRQ_UNLOCK(IrqFlags);
#endif	/* THREAD_ISR */

			break;

		case QID_MGMT:
			// We have to clean all descriptors in case some error happened with reset
		    RTMP_SEM_LOCK(&pAdapter->MgmtRingLock, IrqFlags);
			for (i=0; i<MGMT_RING_SIZE; i++)
			{
#ifndef BIG_ENDIAN
				pTxD  = (PTXD_STRUC) pAdapter->MgmtRing.Cell[i].AllocVa;
#else
		        pDestTxD = (PTXD_STRUC) pAdapter->MgmtRing.Cell[i].AllocVa;
		        TxD = *pDestTxD;
		        pTxD = &TxD;
		        RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
#endif

				pTxD->Owner = DESC_OWN_HOST;
                pTxD->bWaitingDmaDoneInt = 0;

                pci_unmap_single(pAdapter->pPci_Dev, pTxD->BufPhyAddr0, pTxD->BufLen0, PCI_DMA_TODEVICE);

				// rlease scatter-and-gather NDIS_PACKET
#ifdef CONFIG_64BIT
				if (pAdapter->MgmtRing.Cell[i].pSkb)
				{
                    RELEASE_NDIS_PACKET(pAdapter, pAdapter->MgmtRing.Cell[i].pSkb);
                    pAdapter->MgmtRing.Cell[i].pSkb = 0;
				}
#else
				if (pTxD->pSkb)
				{
                    RELEASE_NDIS_PACKET(pAdapter, pTxD->pSkb);
                    pTxD->pSkb = 0;
				}
#endif // CONFIG_64BIT //
#ifdef BIG_ENDIAN
		        RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
        		WriteBackToDescriptor((PUCHAR)pDestTxD, (PUCHAR)pTxD, FALSE, TYPE_TXD);
#endif
			}
            pAdapter->MgmtRing.CurTxIndex = 0;
            pAdapter->MgmtRing.NextTxDmaDoneIndex = 0;
		    RTMP_SEM_UNLOCK(&pAdapter->MgmtRingLock, IrqFlags);
			pAdapter->RalinkCounters.MgmtRingFullCount = 0;
			break;
			
		case QID_RX:
			// We have to clean all descriptors in case some error happened with reset
		    RTMP_SEM_LOCK(&pAdapter->RxRingLock, IrqFlags);
			for (i=0; i<RX_RING_SIZE; i++)
			{
#ifndef BIG_ENDIAN
				pRxD  = (PRXD_STRUC) pAdapter->RxRing.Cell[i].AllocVa;
#else
		        pDestRxD = (PRXD_STRUC) pAdapter->RxRing.Cell[i].AllocVa;
		        RxD = *pDestRxD;
		        pRxD = &RxD;
		        RTMPDescriptorEndianChange((PUCHAR)pRxD, TYPE_RXD);
#endif
				pRxD->Owner = DESC_OWN_NIC;
#ifdef BIG_ENDIAN
		        RTMPDescriptorEndianChange((PUCHAR)pRxD, TYPE_RXD);
        		WriteBackToDescriptor((PUCHAR)pDestRxD, (PUCHAR)pRxD, FALSE, TYPE_RXD);
#endif				
			}
            pAdapter->RxRing.CurRxIndex = 0;
		    RTMP_SEM_UNLOCK(&pAdapter->RxRingLock, IrqFlags);
			break;
			
		default:
			break;
			
	}
}

/*
    ========================================================================

    Routine Description:
        Compare two memory block

    Arguments:
        Adapter                     Pointer to our adapter

    Return Value:
        1:          memory are equal
        0:          memory are not equal

    Note:

    ========================================================================
*/
ULONG   RTMPEqualMemory(
    IN  PVOID   pSrc1,
    IN  PVOID   pSrc2,
    IN  ULONG   Length)
{
    PUCHAR  pMem1;
    PUCHAR  pMem2;
    ULONG   Index = 0;

    pMem1 = (PUCHAR) pSrc1;
    pMem2 = (PUCHAR) pSrc2;

    for (Index = 0; Index < Length; Index++)
    {
        if (pMem1[Index] != pMem2[Index])
        {
            break;
        }
    }

    if (Index == Length)
    {
        return (1);
    }
    else
    {
        return (0);
	}
}

/*
    ========================================================================

    Routine Description:
        Compare two memory block

    Arguments:
		pSrc1		Pointer to first memory address
		pSrc2		Pointer to second memory addres

    Return Value:
        0:          memory is equal
        1:          pSrc1 memory is larger
        2:          pSrc2 memory is larger

    Note:

    ========================================================================
*/
ULONG   RTMPCompareMemory(
    IN  PVOID   pSrc1,
    IN  PVOID   pSrc2,
    IN  ULONG   Length)
{
    PUCHAR  pMem1;
    PUCHAR  pMem2;
    ULONG   Index = 0;

    pMem1 = (PUCHAR) pSrc1;
    pMem2 = (PUCHAR) pSrc2;

    for (Index = 0; Index < Length; Index++)
    {
        if (pMem1[Index] > pMem2[Index])
            return (1);
        else if (pMem1[Index] < pMem2[Index])
            return (2);
    }

    // Equal
    return (0);
}

/*
	========================================================================
	
	Routine Description:
		Zero out memory block

	Arguments:
		pSrc1		Pointer to memory address
		Length		Size

	Return Value:
		None
		
	Note:
		
	========================================================================
*/
VOID    NdisZeroMemory(
    IN  PVOID   pSrc,
    IN  ULONG   Length)
{
    memset(pSrc, 0, Length);
}

VOID    NdisFillMemory(
    IN  PVOID   pSrc,
    IN  ULONG   Length,
    IN  UCHAR   Fill)
{
    memset(pSrc, Fill, Length);
}

/*
	========================================================================
	
	Routine Description:
		Copy data from memory block 1 to memory block 2

	Arguments:
		pDest		Pointer to destination memory address
		pSrc		Pointer to source memory address
		Length		Copy size
		
	Return Value:
		None

	Note:
		
	========================================================================
*/
VOID    NdisMoveMemory(
    OUT PVOID   pDest,
    IN  PVOID   pSrc,
    IN  ULONG   Length)
{
	memcpy(pDest, pSrc, Length);
}

/*
	========================================================================
	
	Routine Description:
		Initialize port configuration structure

	Arguments:
		Adapter						Pointer to our adapter

	Return Value:
		None

	Note:
		
	========================================================================
*/
VOID	PortCfgInit(
	IN	PRTMP_ADAPTER pAd)
{
    UINT i, j;
    UCHAR	CipherSuiteWPATKIP[] = {
		0x00, 0x50, 0xf2, 0x01,	// oui
		0x01, 0x00,				// Version
		0x00, 0x50, 0xf2, 0x02,	// Multicast
		0x01, 0x00,				// Number of unicast
		0x00, 0x50, 0xf2, 0x02,	// unicast
		0x01, 0x00,				// number of authentication method
		0x00, 0x50, 0xf2, 0x01	// authentication
		};
    UCHAR	CipherSuiteWPATKIPLen = (sizeof(CipherSuiteWPATKIP) / sizeof(UCHAR));

    DBGPRINT(RT_DEBUG_TRACE, "--> PortCfgInit\n");    

    pAd->PortCfg.BssidNum = 1;
    for(j = 0; j < pAd->PortCfg.BssidNum; j++)
    {
        pAd->PortCfg.MBSSID[j].CapabilityInfo = 0x0000;
        
        pAd->PortCfg.MBSSID[j].AuthMode = Ndis802_11AuthModeOpen;
        pAd->PortCfg.MBSSID[j].WepStatus = Ndis802_11EncryptionDisabled;
        
        pAd->PortCfg.MBSSID[j].DefaultKeyId = 0;
        pAd->PortCfg.MBSSID[j].IndividualWepKeyIdx = 3;
        for(i = 0; i < SHARE_KEY_NUM; i++)
        {
            pAd->PortCfg.MBSSID[j].SharedKey[i].KeyLen = 0;
        }

        NdisZeroMemory(pAd->PortCfg.MBSSID[j].PMK, 32);

        pAd->PortCfg.MBSSID[j].PrivacyFilter = Ndis802_11PrivFilterAcceptAll;
        pAd->PortCfg.MBSSID[j].BANClass3Data = FALSE;
        pAd->PortCfg.MBSSID[j].IEEE8021X = FALSE;
        pAd->PortCfg.MBSSID[j].PreAuth = FALSE;
        pAd->PortCfg.MBSSID[j].IsolateInterStaTraffic = 0;  // 0- no isolation
        
        pAd->PortCfg.MBSSID[j].VLAN_VID = 0;
        pAd->PortCfg.MBSSID[j].bHideSsid = 0;                // 0- do not hide SSID

        pAd->PortCfg.MBSSID[j].TimBitmap  = 0;
        pAd->PortCfg.MBSSID[j].TimBitmap2 = 0;
        
		pAd->PortCfg.MBSSID[j].TrafficInd = 0;
		
        pAd->PortCfg.MBSSID[j].RSNIE_Len[0] = 0x16;
        NdisZeroMemory(pAd->PortCfg.MBSSID[j].RSN_IE[0], MAX_LEN_OF_RSNIE);
        NdisMoveMemory(pAd->PortCfg.MBSSID[j].RSN_IE[0], CipherSuiteWPATKIP, CipherSuiteWPATKIPLen);

        NdisZeroMemory(&pAd->PortCfg.MBSSID[j].PMKIDCache, sizeof(NDIS_802_11_PMKID));

        NdisMoveMemory(pAd->PortCfg.MBSSID[j].Ssid, "My AP", 5);
		pAd->PortCfg.MBSSID[j].SsidLen = 5;

#ifdef WSC_SUPPORT
		pAd->PortCfg.MBSSID[j].WscControl.WscConfMode = 0;
		pAd->PortCfg.MBSSID[j].WscControl.WscMode = 1;
		pAd->PortCfg.MBSSID[j].WscControl.WscConfStatus = 1;
		pAd->PortCfg.MBSSID[j].WscControl.WscStatus = 0;
		pAd->PortCfg.MBSSID[j].WscControl.WscState = 0;
		pAd->PortCfg.MBSSID[j].WscControl.WscPinCode = 0;
		// <<Reject Same PinCode>> ++, add by johnli
		pAd->PortCfg.MBSSID[j].WscControl.WscLastPinCode = 0;
		pAd->PortCfg.MBSSID[j].WscControl.WscLastStatus = WSC_ERROR_NO_ERROR;
		// <<Reject Same PinCode>> --
		// For Re-Generate Pin-Code Modification
		pAd->PortCfg.MBSSID[j].WscControl.WscEnrolleePinCode = 0;
		// --
		pAd->PortCfg.MBSSID[j].WscControl.WscSelReg = 0;
		NdisZeroMemory(&pAd->PortCfg.MBSSID[j].WscControl.RegData, sizeof(WSC_REG_DATA));
// For AP Client support WPS Modification
    	pAd->PortCfg.MBSSID[j].WscControl.RegData.ReComputePke = 1;
// --
    	pAd->PortCfg.MBSSID[j].WscControl.lastId = 1;
    	pAd->PortCfg.MBSSID[j].WscControl.WscUseUPnP = 0;
    	pAd->PortCfg.MBSSID[j].WscControl.EntryApIdx = WSC_INIT_ENTRY_APIDX;    
    	pAd->WriteWscCfgToDatFile = FALSE;
// For AP Client support WPS Modification
    	pAd->PortCfg.MBSSID[j].WscControl.pAd = pAd;    
// --
#endif // WSC_SUPPORT //
    }

    pAd->PortCfg.Channel = 6;
    pAd->PortCfg.BasicRateBitMap = 0x0F;
    pAd->PortCfg.BeaconPeriod = 100;
    pAd->PortCfg.RtsThreshold = 2347;
    pAd->PortCfg.FragmentThreshold = 2346;

    NdisFillMemory(&pAd->PortCfg.Broadcast, MAC_ADDR_LEN, 0xff);

    pAd->PortCfg.Dsifs = 10;      // in units of usec 

    // Nitro mode control
    pAd->PortCfg.bEnableTxBurst = 0;
    pAd->PortCfg.RadarDetect.IEEE80211H = FALSE;
    pAd->PortCfg.RadarDetect.CSPeriod = 6;
    pAd->PortCfg.RadarDetect.CSCount = 0;
    pAd->PortCfg.RadarDetect.RDMode = RD_NORMAL_MODE;
	pAd->PortCfg.RadarDetect.RDDurRegion = CE;

    // PHY specification
    pAd->RfIcType = RFIC_5225;
    pAd->PortCfg.BandState = UNKNOWN_BAND;
    pAd->PortCfg.PhyMode = PHY_11BG_MIXED;		// default PHY mode

    pAd->PortCfg.TxPreambleInUsed = Rt802_11PreambleLong;
    pAd->PortCfg.TxPreamble = Rt802_11PreambleLong;

	pAd->Mlme.ErpIeContent = 0;

	pAd->LatchBBPR94 = BBPR94_DEFAULT;

    pAd->RfIcType = RFIC_5225;
    pAd->LedCntl.field.LedMode = LED_MODE_DEFAULT;
	pAd->LedIndicatorStregth = 0;

    pAd->PortCfg.TxPowerPercentage = 0xffffffff;  // AUTO

	pAd->bAutoTxAgc = FALSE;			// Default is OFF
	pAd->bAutoTxAgcA = FALSE;			// Default is OFF
	pAd->bAutoTxAgcG = FALSE;			// Default is OFF

	pAd->DefaultTxAntenna = -1;

	pAd->RTSignal.Sig = SIG_NONE;
	pAd->ApdPid = 0;
	pAd->IappPid = 0;

	pAd->NeedSwapToLittleEndian = TRUE;

	pAd->PortCfg.PMKCachePeriod = (10 * 60 * HZ); // unit : tick(default: 10 minute)

	pAd->PortCfg.AvgRssi = 0;
	pAd->PortCfg.AvgRssiX8 = 0;
	pAd->PortCfg.LastRssi = 0;
	pAd->PortCfg.AvgRssi2 = 0;
	pAd->PortCfg.AvgRssi2X8 = 0;
	pAd->PortCfg.LastRssi2 = 0;

	pAd->PortCfg.bCountryFlag = FALSE;

	pAd->PortCfg.bAPSDCapable = FALSE;

	pAd->PortCfg.MaxTxPowerLevel = 16;
#ifdef RTL865X_SOC
	pAd->PortCfg.bEthWithVLANTag = FALSE;
#endif

	// dynamic BBP R17:sensibity tuning to overcome background noise
  	pAd->BbpTuning.bEnable                = TRUE;  
	pAd->BbpTuning.R17LowerBoundG         = 0x20; // for best RX sensibility
	pAd->BbpTuning.R17UpperBoundG         = 0x40; // for best RX noise isolation to prevent false CCA
	pAd->BbpTuning.R17LowerBoundA         = 0x28; // for best RX sensibility
	pAd->BbpTuning.R17UpperBoundA         = 0x48; // for best RX noise isolation to prevent false CCA
	pAd->BbpTuning.FalseCcaLowerThreshold = 100;
	pAd->BbpTuning.FalseCcaUpperThreshold = 512;   
	pAd->BbpTuning.R17Delta               = 1;

#ifdef RALINK_ATE
	memset(&pAd->ate, 0, sizeof(ATE_INFO));
	pAd->ate.Mode = ATE_APSTART;
	pAd->ate.TxCount = TX_RING_SIZE;
	pAd->ate.TxLength = 1024;
	pAd->ate.TxRate = RATE_11;
    pAd->ate.Channel = 1;
    pAd->ate.Addr1[0] = 0x00;
    pAd->ate.Addr1[1] = 0x11;
    pAd->ate.Addr1[2] = 0x22;
    pAd->ate.Addr1[3] = 0xAA;
    pAd->ate.Addr1[4] = 0xBB;
    pAd->ate.Addr1[5] = 0xCC;
	memcpy(pAd->ate.Addr2, pAd->ate.Addr1, ETH_LENGTH_OF_ADDRESS);
	memcpy(pAd->ate.Addr3, pAd->ate.Addr1, ETH_LENGTH_OF_ADDRESS);
	pAd->ate.bRxFer = 0;
#endif	//RALINK_ATE

	pAd->PortCfg.bRetryLimit = SHORT_RETRY;
	pAd->PortCfg.TxQueueSize = MAX_PACKETS_IN_QUEUE;

    DBGPRINT(RT_DEBUG_TRACE, "<-- PortCfgInit\n");
}

UCHAR BtoH(
	IN CHAR		ch)
{
    if (ch >= '0' && ch <= '9') return (ch - '0');        // Handle numerals
    if (ch >= 'A' && ch <= 'F') return (ch - 'A' + 0xA);  // Handle capitol hex digits
    if (ch >= 'a' && ch <= 'f') return (ch - 'a' + 0xA);  // Handle small hex digits
    return(255);
}

CHAR HtoB(
	IN CHAR		ch)
{
	if ((ch >= 0x0) && (ch <= 0x9)) return (ch + 0x30);
	if ((ch >= 0xA) && (ch <= 0xF)) return (ch + 0x57);
    return(255);
}

VOID HtoA(
	IN UCHAR	*src,
	OUT CHAR	*dest,
	IN INT		destlen)
{
    UCHAR *srcptr;
    PCHAR destTemp;

    srcptr = src;   
    destTemp = dest; 

    while(destlen--)
    {
    	*destTemp = *srcptr >> 4;
    	*destTemp = HtoB(*destTemp);
    	destTemp++;
    	*destTemp = *srcptr & 0xf;
    	*destTemp = HtoB(*destTemp);
    	destTemp++;
    	srcptr++;
    }
}

//
//  PURPOSE:  Converts ascii string to network order hex
//
//  PARAMETERS:
//    src    - pointer to input ascii string
//    dest   - pointer to output hex
//    destlen - size of dest
//
//  COMMENTS:
//
//    2 ascii bytes make a hex byte so must put 1st ascii byte of pair
//    into upper nibble and 2nd ascii byte of pair into lower nibble.
//
VOID AtoH(
	IN CHAR		*src,
	OUT UCHAR	*dest,
	IN INT		destlen)
{
    CHAR *srcptr;
    PUCHAR destTemp;

    srcptr = src;   
    destTemp = (PUCHAR) dest; 

    while(destlen--)
    {
        *destTemp = BtoH(*srcptr++) << 4;    // Put 1st ascii byte in upper nibble.
        *destTemp += BtoH(*srcptr++);      // Add 2nd ascii byte to above.
        destTemp++;
    }
}



// For AP Client support WPS Modification
// Init WSC timer.
#ifdef WSC_SUPPORT
VOID RTMPWscInitTimer(
	IN PWSC_CTRL pWscControl,
	IN struct timer_list *timer,
	IN PVOID timerFunc)
{
	timer->data = (UMAXLONG)pWscControl;
	timer->function = timerFunc;
	init_timer(timer);

	return;
}
#endif // WSC_SUPPORT //
// --

// Init timer.
VOID RTMPInitTimer(
	IN PRTMP_ADAPTER pAd,
	IN struct timer_list *timer,
	IN PVOID timerFunc)
{
	timer->data = (UMAXLONG)pAd;
	timer->function = timerFunc;
	init_timer(timer);

	return;
}

// Add timer object.
VOID RTMPAddTimer(
	IN struct timer_list *timer,
	IN ULONG Timeout)
{
	if (timer_pending(timer))
		del_timer_sync(timer);
	
	timer->expires = jiffies + Timeout;
	add_timer(timer);
}

// Del timer object.
VOID RTMPCancelTimer(
	IN struct timer_list *timer)
{
	if (timer_pending(timer))
		del_timer_sync(timer);

	return;
}

/*
	========================================================================
	
	Routine Description:
		Set LED Status

	Arguments:
		pAd						Pointer to our adapter
		Status					LED Status

	Return Value:
		None

	Note:
		
	========================================================================
*/
VOID RTMPSetLED(
	IN PRTMP_ADAPTER 	pAd, 
	IN UCHAR			Status)
{
	UCHAR			HighByte=0;
	UCHAR			LowByte=0;
       
	switch (Status)
	{
		case LED_LINK_DOWN:
			pAd->LedCntl.field.LinkGStatus = 0;
			pAd->LedCntl.field.LinkAStatus = 0;
			pAd->LedCntl.field.LedMode = 0x18;//reset led mode	
			HighByte = pAd->LedCntl.word >> 8;
			LowByte = pAd->LedCntl.word & 0xFF;			
			AsicSendCommandToMcu(pAd, 0x50, 0xff, LowByte, HighByte);
			pAd->LedIndicatorStregth = 0; 
			break;
		case LED_LINK_UP:
			if (pAd->PortCfg.Channel <= 14)
			{
				// 11 G mode
				pAd->LedCntl.field.LinkGStatus = 1;
				pAd->LedCntl.field.LinkAStatus = 0;
			}
			else
			{ 
				//11 A mode
				pAd->LedCntl.field.LinkGStatus = 0;
				pAd->LedCntl.field.LinkAStatus = 1;
			}
			
			HighByte = pAd->LedCntl.word >> 8;
			LowByte = pAd->LedCntl.word & 0xFF;			
			AsicSendCommandToMcu(pAd, 0x50, 0xff, LowByte, HighByte);
			break;
		case LED_RADIO_ON:
			pAd->LedCntl.field.RadioStatus = 1;
			HighByte = pAd->LedCntl.word >> 8;
			LowByte = pAd->LedCntl.word & 0xFF;
			AsicSendCommandToMcu(pAd, 0x50, 0xff, LowByte, HighByte);
			break;
		case LED_HALT: 
			//Same as Radio Off.
		case LED_RADIO_OFF:
			pAd->LedCntl.field.RadioStatus = 0;
			pAd->LedCntl.field.LinkGStatus = 0;
			pAd->LedCntl.field.LinkAStatus = 0;
			HighByte = pAd->LedCntl.word >> 8;
			LowByte = pAd->LedCntl.word & 0xFF;
			AsicSendCommandToMcu(pAd, 0x50, 0xff, LowByte, HighByte);
			break;
		default:
			DBGPRINT(RT_DEBUG_WARN, "RTMPSetLED::Unknown Status %d\n", Status);
			break;
	}
       DBGPRINT(RT_DEBUG_INFO, "RTMPSetLED::Mode=%d,HighByte=0x%02x,LowByte=0x%02x\n", pAd->LedCntl.field.LedMode, HighByte, LowByte);
}



VOID RTMPPrepareRadarDetectParams(
	IN PRTMP_ADAPTER	pAd)
{
	// Initialize parameters for firmware-base radar detection
	// Fill CTS frame
	// change 1st CTS's duration from 15100 to 25100 for Carrier-Sense feature.
	RTMPPrepareRDCTSFrame(pAd, pAd->CurrentAddress,	25100, pAd->PortCfg.RtsRate, 0, IFS_SIFS);
	RTMPPrepareRDCTSFrame(pAd, pAd->CurrentAddress,	15100, pAd->PortCfg.RtsRate, 1, IFS_SIFS);
	RTMPPrepareRDCTSFrame(pAd, pAd->CurrentAddress,	15100, pAd->PortCfg.RtsRate, 2, IFS_SIFS);
	RTMPPrepareRDCTSFrame(pAd, pAd->CurrentAddress,	15100, pAd->PortCfg.RtsRate, 3, IFS_SIFS);
//iverson
	RTMP_IO_WRITE32(pAd, 0x2980, 20);
	RTMP_IO_WRITE32(pAd, 0x2984, 10);
	RTMP_IO_WRITE32(pAd, 0x2988, 13);
	RTMP_IO_WRITE32(pAd, 0x298c, 9);
	RTMP_IO_WRITE32(pAd, 0x2990, 13);
	RTMP_IO_WRITE32(pAd, 0x2994, 20);
	RTMP_IO_WRITE32(pAd, 0x2998, 13);
	RTMP_IO_WRITE32(pAd, 0x299c, 240);
	// clear result 
	RTMP_IO_WRITE32(pAd, 0x29a0, 0);

	// Used by firmware to restore BBP value from radar detection mode
	RTMP_IO_WRITE32(pAd, 0x29b0, 0xc0535052);	// 0x[Value][ID][Value][ID]
	RTMP_IO_WRITE32(pAd, 0x29b4, 0x06121054);	// 0x[Value][ID][Value][ID]
	RTMP_IO_WRITE32(pAd, 0x29b8, 0x3816c815);	// 0x[Value][ID][Value][ID]
	RTMP_IO_WRITE32(pAd, 0x29bc, 0x28112c10);	// 0x[Value][ID][Value][ID]
	RTMP_IO_WRITE32(pAd, 0x29c0, 0xFFFF0140);	// 0xFF means firmware can ignore it

	// Change BBP setting during siwtch from a->g, g->a
    if (pAd->PortCfg.Channel <= 14)
    {
    	if (pAd->NicConfig2.field.ExternalLNAForG)
    	{
    		pAd->PortCfg.RadarDetect.BBPR17 = 0x30;	// if external LNA enable, this value need to be offset 0x10
    	}
    	else
    	{
    		pAd->PortCfg.RadarDetect.BBPR17 = 0x20;
    	}
    }
    else
    {
    	if (pAd->NicConfig2.field.ExternalLNAForA)
    	{
    		pAd->PortCfg.RadarDetect.BBPR17 = 0x28 + pAd->AModeGainValue;	// if external LNA enable, this value need to be offset 0x10
    	}
    	else
    	{
    		pAd->PortCfg.RadarDetect.BBPR17 = 0x28;
    	}
    }
    pAd->PortCfg.RadarDetect.BBPR16 = 0x2c;
    pAd->PortCfg.RadarDetect.BBPR18 = 0x06;
    pAd->PortCfg.RadarDetect.BBPR21 = 0xc8;
    pAd->PortCfg.RadarDetect.BBPR22 = 0x38;
    pAd->PortCfg.RadarDetect.BBPR64 = 0x01;
}

