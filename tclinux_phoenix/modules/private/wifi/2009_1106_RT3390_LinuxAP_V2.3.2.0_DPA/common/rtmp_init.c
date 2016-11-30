/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2004, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
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
*/
#include	"rt_config.h"

#define RT3090A_DEFAULT_INTERNAL_LNA_GAIN	0x0A
UCHAR    BIT8[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
char*   CipherName[] = {"none","wep64","wep128","TKIP","AES","CKIP64","CKIP128","CKIP152","SMS4"};

//
// BBP register initialization set
//
REG_PAIR   BBPRegTable[] = {
	{BBP_R65,		0x2C},		// fix rssi issue
	{BBP_R66,		0x38},	// Also set this default value to pAd->BbpTuning.R66CurrentValue at initial
	{BBP_R69,		0x12},
	{BBP_R70,		0xa},	// BBP_R70 will change to 0x8 in ApStartUp and LinkUp for rt2860C, otherwise value is 0xa
	{BBP_R73,		0x10},
	{BBP_R81,		0x37},
	{BBP_R82,		0x62},
	{BBP_R83,		0x6A},
	{BBP_R84,		0x99},	// 0x19 is for rt2860E and after. This is for extension channel overlapping IOT. 0x99 is for rt2860D and before
	{BBP_R86,		0x00},	// middle range issue, Rory @2008-01-28 	
	{BBP_R91,		0x04},	// middle range issue, Rory @2008-01-28
	{BBP_R92,		0x00},	// middle range issue, Rory @2008-01-28
	{BBP_R103,		0x00}, 	// near range high-power issue, requested from Gary @2008-0528
	{BBP_R105,		0x05},	// 0x05 is for rt2860E to turn on FEQ control. It is safe for rt2860D and before, because Bit 7:2 are reserved in rt2860D and before.
	{BBP_R106,		0x35},	// Optimizing the Short GI sampling request from Gray @2009-0409
};
#define	NUM_BBP_REG_PARMS	(sizeof(BBPRegTable) / sizeof(REG_PAIR))


//
// ASIC register initialization sets
//

RTMP_REG_PAIR	MACRegTable[] =	{
#if defined(HW_BEACON_OFFSET) && (HW_BEACON_OFFSET == 0x200)
	{BCN_OFFSET0,			0xf8f0e8e0}, /* 0x3800(e0), 0x3A00(e8), 0x3C00(f0), 0x3E00(f8), 512B for each beacon */
	{BCN_OFFSET1,			0x6f77d0c8}, /* 0x3200(c8), 0x3400(d0), 0x1DC0(77), 0x1BC0(6f), 512B for each beacon */
#elif defined(HW_BEACON_OFFSET) && (HW_BEACON_OFFSET == 0x100)
	{BCN_OFFSET0,			0xece8e4e0}, /* 0x3800, 0x3A00, 0x3C00, 0x3E00, 512B for each beacon */
	{BCN_OFFSET1,			0xfcf8f4f0}, /* 0x3800, 0x3A00, 0x3C00, 0x3E00, 512B for each beacon */
#else
    #error You must re-calculate new value for BCN_OFFSET0 & BCN_OFFSET1 in MACRegTable[]!!!
#endif // HW_BEACON_OFFSET //

	{LEGACY_BASIC_RATE,		0x0000013f}, //  Basic rate set bitmap
	{HT_BASIC_RATE,		0x00008003}, // Basic HT rate set , 20M, MCS=3, MM. Format is the same as in TXWI.
	{MAC_SYS_CTRL,		0x00}, // 0x1004, , default Disable RX
	{RX_FILTR_CFG,		0x17f97}, //0x1400  , RX filter control,  
	{BKOFF_SLOT_CFG,	0x209}, // default set short slot time, CC_DELAY_TIME should be 2	 
	//{TX_SW_CFG0,		0x40a06}, // Gary,2006-08-23 
	{TX_SW_CFG0,		0x0}, 		// Gary,2008-05-21 for CWC test 
	{TX_SW_CFG1,		0x80606}, // Gary,2006-08-23 
	{TX_LINK_CFG,		0x1020},		// Gary,2006-08-23 
	//{TX_TIMEOUT_CFG,	0x00182090},	// CCK has some problem. So increase timieout value. 2006-10-09// MArvek RT
	{TX_TIMEOUT_CFG,	0x000a2090},	// CCK has some problem. So increase timieout value. 2006-10-09// MArvek RT , Modify for 2860E ,2007-08-01
#if 0 //ndef TCSUPPORT_CZ_OTE
	{MAX_LEN_CFG,		MAX_AGGREGATION_SIZE | 0x00001000},	// 0x3018, MAX frame length. Max PSDU = 16kbytes.
#else // default turn on this patch
	{MAX_LEN_CFG,		0x2FFF},
#endif
	{LED_CFG,		0x7f031e46}, // Gary, 2006-08-23

//#ifdef CONFIG_AP_SUPPORT
//	{WMM_AIFSN_CFG,		0x00001173},
//	{WMM_CWMIN_CFG,		0x00002344},
//	{WMM_CWMAX_CFG,		0x000034a6},
//	{WMM_TXOP0_CFG,		0x00100020},
//	{WMM_TXOP1_CFG,		0x002F0038},
//#endif // CONFIG_AP_SUPPORT //

//#ifdef CONFIG_STA_SUPPORT
//	{WMM_AIFSN_CFG,		0x00002273},
//	{WMM_CWMIN_CFG,		0x00002344},
//	{WMM_CWMAX_CFG,		0x000034aa},
//#endif // CONFIG_STA_SUPPORT //
#ifdef INF_AMAZON_SE
	{PBF_MAX_PCNT,			0x1F3F6F6F}, 	//iverson modify for usb issue, 2008/09/19
											// 6F + 6F < total page count FE
											// so that RX doesn't occupy TX's buffer space when WMM congestion.
#else
	{PBF_MAX_PCNT,			0x1F3FBF9F}, 	//0x1F3f7f9f},		//Jan, 2006/04/20
#endif // INF_AMAZON_SE //
	//{TX_RTY_CFG,			0x6bb80408},	// Jan, 2006/11/16
// WMM_ACM_SUPPORT
//	{TX_RTY_CFG,			0x6bb80101},	// sample
	{TX_RTY_CFG,			0x47d01f0f},	// Jan, 2006/11/16, Set TxWI->ACK =0 in Probe Rsp Modify for 2860E ,2007-08-03
	
	{AUTO_RSP_CFG,			0x00000013},	// Initial Auto_Responder, because QA will turn off Auto-Responder
	{CCK_PROT_CFG,			0x05740003 /*0x01740003*/},	// Initial Auto_Responder, because QA will turn off Auto-Responder. And RTS threshold is enabled. 
	{OFDM_PROT_CFG,			0x05740003 /*0x01740003*/},	// Initial Auto_Responder, because QA will turn off Auto-Responder. And RTS threshold is enabled. 
	{GF20_PROT_CFG,			0x01744004},    // set 19:18 --> Short NAV for MIMO PS
	{GF40_PROT_CFG,			0x03F44084},    
	{MM20_PROT_CFG,			0x01744004},    
#ifdef RTMP_MAC_PCI
	{MM40_PROT_CFG,			0x03F54084},	
#endif // RTMP_MAC_PCI //
	{TXOP_CTRL_CFG,			0x0000583f, /*0x0000243f*/ /*0x000024bf*/},	//Extension channel backoff.
	{TX_RTS_CFG,			0x00092b20},	
//#ifdef WIFI_TEST
	{EXP_ACK_TIME,			0x002400ca},	// default value
//#else
//	{EXP_ACK_TIME,			0x005400ca},	// suggested by Gray @ 20070323 for 11n intel-sta throughput
//#endif // end - WIFI_TEST //
//#ifdef CONFIG_AP_SUPPORT
//	{TBTT_SYNC_CFG,			0x00422000},	// TBTT_ADJUST(7:0) == 0	
//	{TBTT_SYNC_CFG,			0x00012000},	// TBTT_ADJUST(7:0) == 0	
//#endif // CONFIG_AP_SUPPORT //
	{TXOP_HLDR_ET, 			0x00000002},

	/* Jerry comments 2008/01/16: we use SIFS = 10us in CCK defaultly, but it seems that 10us
		is too small for INTEL 2200bg card, so in MBSS mode, the delta time between beacon0
		and beacon1 is SIFS (10us), so if INTEL 2200bg card connects to BSS0, the ping
		will always lost. So we change the SIFS of CCK from 10us to 16us. */
	{XIFS_TIME_CFG,			0x33a41010},
	{PWR_PIN_CFG,			0x00000003},	// patch for 2880-E
};

#ifdef CONFIG_AP_SUPPORT
RTMP_REG_PAIR	APMACRegTable[] =	{
	{WMM_AIFSN_CFG,		0x00001173},
	{WMM_CWMIN_CFG,	0x00002344},
	{WMM_CWMAX_CFG,	0x000034a6},
	{WMM_TXOP0_CFG,		0x00100020},
	{WMM_TXOP1_CFG,		0x002F0038},
	{TBTT_SYNC_CFG,		0x00012000},
};
#endif // CONFIG_AP_SUPPORT //


#define	NUM_MAC_REG_PARMS		(sizeof(MACRegTable) / sizeof(RTMP_REG_PAIR))
#ifdef CONFIG_AP_SUPPORT
#define	NUM_AP_MAC_REG_PARMS		(sizeof(APMACRegTable) / sizeof(RTMP_REG_PAIR))
#endif // CONFIG_AP_SUPPORT //

#ifdef TC_SOC
void tcSetMacAddr(unsigned char* mac_addr){
	int i;
	uint8_t def_mac_addr[] = {0x00, 0x00, 0xaa, 0xbb, 0xcc, 0xff};

//	  uint8 *flash_mac_addr = (uint8 *)0xbfc0ff48;
	char flash_mac_addr[6];

	for (i=0; i<6; i++) {
		flash_mac_addr[i] = READ_FLASH_BYTE(flash_base + 0xff48 + i);
	}


	if( (flash_mac_addr[0] == 0) && (flash_mac_addr[1] == 0) && (flash_mac_addr[2] == 0) &&
	    (flash_mac_addr[3] == 0) && (flash_mac_addr[4] == 0) && (flash_mac_addr[5] == 0) )
		printk(KERN_INFO "The MAC address in flash is null!\n");	    
	else    
  		memcpy(def_mac_addr, flash_mac_addr, 6);  	
	
	for (i = 0; i < 6; i++) {
		mac_addr[i] = def_mac_addr[i];
	}
}
#endif

/*
	========================================================================
	
	Routine Description:
		Allocate RTMP_ADAPTER data block and do some initialization

	Arguments:
		Adapter		Pointer to our adapter

	Return Value:
		NDIS_STATUS_SUCCESS
		NDIS_STATUS_FAILURE

	IRQL = PASSIVE_LEVEL

	Note:
	
	========================================================================
*/
NDIS_STATUS	RTMPAllocAdapterBlock(
	IN  PVOID	handle,
	OUT	PRTMP_ADAPTER	*ppAdapter)
{
	PRTMP_ADAPTER	pAd;
	NDIS_STATUS		Status;
	INT 			index;
	UCHAR			*pBeaconBuf = NULL;

	DBGPRINT(RT_DEBUG_TRACE, ("--> RTMPAllocAdapterBlock\n"));

	*ppAdapter = NULL;

	do
	{
		// Allocate RTMP_ADAPTER memory block
		Status = AdapterBlockAllocateMemory(handle, (PVOID *)&pAd);
		if (Status != NDIS_STATUS_SUCCESS)
		{
			DBGPRINT_ERR(("Failed to allocate memory - ADAPTER\n"));
			break;
		}
//#ifndef VENDOR_FEATURE3_SUPPORT	//STA & MESH	
		DBGPRINT(RT_DEBUG_OFF, ("\n\n=== pAd = %p, size = %d ===\n\n", pAd, (UINT32)sizeof(RTMP_ADAPTER)));


		// Init spin locks
		NdisAllocateSpinLock(&pAd->MgmtRingLock);
#ifdef RTMP_MAC_PCI
		NdisAllocateSpinLock(&pAd->RxRingLock);
#ifdef RT3090
#endif // RT3090 //
#endif // RTMP_MAC_PCI //

		for (index =0 ; index < NUM_OF_TX_RING; index++)
		{
			NdisAllocateSpinLock(&pAd->TxSwQueueLock[index]);
			NdisAllocateSpinLock(&pAd->DeQueueLock[index]);
			pAd->DeQueueRunning[index] = FALSE;
		}

		NdisAllocateSpinLock(&pAd->irq_lock);

#ifdef WMM_ACM_SUPPORT
		NdisAllocateSpinLock(&pAd->AcmTspecSemLock);
		NdisAllocateSpinLock(&pAd->AcmTspecIrqLock);
#endif // WMM_ACM_SUPPORT //

#ifdef CONFIG_AP_SUPPORT
#ifdef UAPSD_AP_SUPPORT
        UAPSD_Init(pAd);
#endif // UAPSD_AP_SUPPORT //
#endif // CONFIG_AP_SUPPORT //
	} while (FALSE);

	
	*ppAdapter = pAd;

	DBGPRINT_S(Status, ("<-- RTMPAllocAdapterBlock, Status=%x\n", Status));
	return Status;
}


/*
	========================================================================
	
	Routine Description:
		Read initial Tx power per MCS and BW from EEPROM
		
	Arguments:
		Adapter						Pointer to our adapter

	Return Value:
		None

	IRQL = PASSIVE_LEVEL

	Note:
		
	========================================================================
*/
VOID	RTMPReadTxPwrPerRate(
	IN	PRTMP_ADAPTER	pAd)
{
	ULONG		data, Adata, Gdata;
	USHORT		i, value, value2;
	INT			Apwrdelta, Gpwrdelta;
	UCHAR		t1,t2,t3,t4;
	BOOLEAN		bApwrdeltaMinus = TRUE, bGpwrdeltaMinus = TRUE;
	
	//
	// Get power delta for 20MHz and 40MHz.
	//
	DBGPRINT(RT_DEBUG_TRACE, ("Txpower per Rate\n"));
	RT28xx_EEPROM_READ16(pAd, EEPROM_TXPOWER_DELTA, value2);
	Apwrdelta = 0;
	Gpwrdelta = 0;

	if ((value2 & 0xff) != 0xff)
	{
		if ((value2 & 0x80))
			Gpwrdelta = (value2&0xf);
		
		if ((value2 & 0x40))
			bGpwrdeltaMinus = FALSE;
		else
			bGpwrdeltaMinus = TRUE;
	}
	if ((value2 & 0xff00) != 0xff00)
	{
		if ((value2 & 0x8000))
			Apwrdelta = ((value2&0xf00)>>8);

		if ((value2 & 0x4000))
			bApwrdeltaMinus = FALSE;
		else
			bApwrdeltaMinus = TRUE;
	}	
	DBGPRINT(RT_DEBUG_TRACE, ("Gpwrdelta = %x, Apwrdelta = %x .\n", Gpwrdelta, Apwrdelta));

	//
	// Get Txpower per MCS for 20MHz in 2.4G.
	//
	for (i=0; i<5; i++)
	{
		RT28xx_EEPROM_READ16(pAd, EEPROM_TXPOWER_BYRATE_20MHZ_2_4G + i*4, value);
		data = value;
		if (bApwrdeltaMinus == FALSE)
		{
			t1 = (value&0xf)+(Apwrdelta);
			if (t1 > 0xf)
				t1 = 0xf;
			t2 = ((value&0xf0)>>4)+(Apwrdelta);
			if (t2 > 0xf)
				t2 = 0xf;
			t3 = ((value&0xf00)>>8)+(Apwrdelta);
			if (t3 > 0xf)
				t3 = 0xf;
			t4 = ((value&0xf000)>>12)+(Apwrdelta);
			if (t4 > 0xf)
				t4 = 0xf;
		}
		else
		{
			if ((value&0xf) > Apwrdelta)
				t1 = (value&0xf)-(Apwrdelta);
			else
				t1 = 0;
			if (((value&0xf0)>>4) > Apwrdelta)
				t2 = ((value&0xf0)>>4)-(Apwrdelta);
			else
				t2 = 0;
			if (((value&0xf00)>>8) > Apwrdelta)
				t3 = ((value&0xf00)>>8)-(Apwrdelta);
			else
				t3 = 0;
			if (((value&0xf000)>>12) > Apwrdelta)
				t4 = ((value&0xf000)>>12)-(Apwrdelta);
			else
				t4 = 0;
		}				
		Adata = t1 + (t2<<4) + (t3<<8) + (t4<<12);
		if (bGpwrdeltaMinus == FALSE)
		{
			t1 = (value&0xf)+(Gpwrdelta);
			if (t1 > 0xf)
				t1 = 0xf;
			t2 = ((value&0xf0)>>4)+(Gpwrdelta);
			if (t2 > 0xf)
				t2 = 0xf;
			t3 = ((value&0xf00)>>8)+(Gpwrdelta);
			if (t3 > 0xf)
				t3 = 0xf;
			t4 = ((value&0xf000)>>12)+(Gpwrdelta);
			if (t4 > 0xf)
				t4 = 0xf;
		}
		else
		{
			if ((value&0xf) > Gpwrdelta)
				t1 = (value&0xf)-(Gpwrdelta);
			else
				t1 = 0;
			if (((value&0xf0)>>4) > Gpwrdelta)
				t2 = ((value&0xf0)>>4)-(Gpwrdelta);
			else
				t2 = 0;
			if (((value&0xf00)>>8) > Gpwrdelta)
				t3 = ((value&0xf00)>>8)-(Gpwrdelta);
			else
				t3 = 0;
			if (((value&0xf000)>>12) > Gpwrdelta)
				t4 = ((value&0xf000)>>12)-(Gpwrdelta);
			else
				t4 = 0;
		}				
		Gdata = t1 + (t2<<4) + (t3<<8) + (t4<<12);
		
		RT28xx_EEPROM_READ16(pAd, EEPROM_TXPOWER_BYRATE_20MHZ_2_4G + i*4 + 2, value);
		if (bApwrdeltaMinus == FALSE)
		{
			t1 = (value&0xf)+(Apwrdelta);
			if (t1 > 0xf)
				t1 = 0xf;
			t2 = ((value&0xf0)>>4)+(Apwrdelta);
			if (t2 > 0xf)
				t2 = 0xf;
			t3 = ((value&0xf00)>>8)+(Apwrdelta);
			if (t3 > 0xf)
				t3 = 0xf;
			t4 = ((value&0xf000)>>12)+(Apwrdelta);
			if (t4 > 0xf)
				t4 = 0xf;
		}
		else
		{
			if ((value&0xf) > Apwrdelta)
				t1 = (value&0xf)-(Apwrdelta);
			else
				t1 = 0;
			if (((value&0xf0)>>4) > Apwrdelta)
				t2 = ((value&0xf0)>>4)-(Apwrdelta);
			else
				t2 = 0;
			if (((value&0xf00)>>8) > Apwrdelta)
				t3 = ((value&0xf00)>>8)-(Apwrdelta);
			else
				t3 = 0;
			if (((value&0xf000)>>12) > Apwrdelta)
				t4 = ((value&0xf000)>>12)-(Apwrdelta);
			else
				t4 = 0;
		}				
		Adata |= ((t1<<16) + (t2<<20) + (t3<<24) + (t4<<28));
		if (bGpwrdeltaMinus == FALSE)
		{
			t1 = (value&0xf)+(Gpwrdelta);
			if (t1 > 0xf)
				t1 = 0xf;
			t2 = ((value&0xf0)>>4)+(Gpwrdelta);
			if (t2 > 0xf)
				t2 = 0xf;
			t3 = ((value&0xf00)>>8)+(Gpwrdelta);
			if (t3 > 0xf)
				t3 = 0xf;
			t4 = ((value&0xf000)>>12)+(Gpwrdelta);
			if (t4 > 0xf)
				t4 = 0xf;
		}
		else
		{
			if ((value&0xf) > Gpwrdelta)
				t1 = (value&0xf)-(Gpwrdelta);
			else
				t1 = 0;
			if (((value&0xf0)>>4) > Gpwrdelta)
				t2 = ((value&0xf0)>>4)-(Gpwrdelta);
			else
				t2 = 0;
			if (((value&0xf00)>>8) > Gpwrdelta)
				t3 = ((value&0xf00)>>8)-(Gpwrdelta);
			else
				t3 = 0;
			if (((value&0xf000)>>12) > Gpwrdelta)
				t4 = ((value&0xf000)>>12)-(Gpwrdelta);
			else
				t4 = 0;
		}				
		Gdata |= ((t1<<16) + (t2<<20) + (t3<<24) + (t4<<28));
		data |= (value<<16);

		/* For 20M/40M Power Delta issue */		
#ifdef A_BAND_SUPPORT		
		pAd->Tx20MPwrCfgABand[i] = data;
		pAd->Tx40MPwrCfgABand[i] = Adata;
#endif		
		pAd->Tx20MPwrCfgGBand[i] = data;
		pAd->Tx40MPwrCfgGBand[i] = Gdata;
		
		if (data != 0xffffffff)
			RTMP_IO_WRITE32(pAd, TX_PWR_CFG_0 + i*4, data);
		DBGPRINT_RAW(RT_DEBUG_TRACE, ("20MHz BW, 2.4G band-%lx,  Adata = %lx,  Gdata = %lx \n", data, Adata, Gdata));
	}
}


/*
	========================================================================
	
	Routine Description:
		Read initial channel power parameters from EEPROM
		
	Arguments:
		Adapter						Pointer to our adapter

	Return Value:
		None

	IRQL = PASSIVE_LEVEL

	Note:
		
	========================================================================
*/
VOID	RTMPReadChannelPwr(
	IN	PRTMP_ADAPTER	pAd)
{
	UCHAR				i, choffset;
	EEPROM_TX_PWR_STRUC	    Power;
	EEPROM_TX_PWR_STRUC	    Power2;
#ifdef RT33xx
	UCHAR Tx0ALC = 0, Tx1ALC = 0, Tx0FinePowerCtrl = 0, Tx1FinePowerCtrl = 0;
#endif // RT33xx //
	
	// Read Tx power value for all channels
	// Value from 1 - 0x7f. Default value is 24.
	// Power value : 2.4G 0x00 (0) ~ 0x1F (31)
	//             : 5.5G 0xF9 (-7) ~ 0x0F (15)
#ifdef RT3390	
	// 0. 11b/g, ch1 - ch 14
	for (i = 0; i < 7; i++)
	{
		// Tx power control over RF R12, RF R13 and BBP R109
		if ((IS_RT3090A(pAd) || IS_RT3390(pAd)) && pAd->infType==RTMP_DEV_INF_PCIE)
		{
			RT28xx_EEPROM_READ16(pAd, EEPROM_G_TX_PWR_OFFSET + i * 2, Power.word);
			RT28xx_EEPROM_READ16(pAd, EEPROM_G_TX2_PWR_OFFSET + i * 2, Power2.word);
			pAd->TxPower[i * 2].Channel = i * 2 + 1;
			pAd->TxPower[i * 2 + 1].Channel = i * 2 + 2;

			//
			// Tx0 power control
			//
			Tx0ALC = GET_TX_ALC_BIT_MASK & Power.field.Byte0;
			Tx0FinePowerCtrl = ((GET_TX_FINE_POWER_CTRL_BIT_MASK & Power.field.Byte0) >> NUMBER_OF_BITS_FOR_TX_ALC);

			if (IS_RT3390(pAd))
			{
				pAd->TxPower[i * 2].Power = Power.field.Byte0;
			}
			else
			{
				if ((Tx0ALC > 31) || (Tx0ALC < 0))
				{
					pAd->TxPower[i * 2].Power = DEFAULT_RF_TX_POWER;
				}
				else
				{
					pAd->TxPower[i * 2].Power = Tx0ALC;
				}
			}	

			if ((Tx0FinePowerCtrl > 4) || (Tx0FinePowerCtrl < 0))
			{
				pAd->TxPower[i * 2].Tx0FinePowerCtrl = DEFAULT_BBP_TX_FINE_POWER_CTRL;
			}
			else
			{
				pAd->TxPower[i * 2].Tx0FinePowerCtrl = Tx0FinePowerCtrl;
			}

			Tx0ALC = GET_TX_ALC_BIT_MASK & Power.field.Byte1;
			Tx0FinePowerCtrl = ((GET_TX_FINE_POWER_CTRL_BIT_MASK & Power.field.Byte1) >> NUMBER_OF_BITS_FOR_TX_ALC);

			if (IS_RT3390(pAd))
			{
				pAd->TxPower[i * 2 + 1].Power = Power.field.Byte1;
			}
			else
			{
				if ((Tx0ALC > 31) || (Tx0ALC < 0))
				{
					pAd->TxPower[i * 2 + 1].Power = DEFAULT_RF_TX_POWER;
				}
				else
				{
					pAd->TxPower[i * 2 + 1].Power = Tx0ALC;
				}
			}

			if ((Tx0FinePowerCtrl > 4) || (Tx0FinePowerCtrl < 0))
			{
				pAd->TxPower[i * 2 + 1].Tx0FinePowerCtrl = DEFAULT_BBP_TX_FINE_POWER_CTRL;
			}
			else
			{
				pAd->TxPower[i * 2 + 1].Tx0FinePowerCtrl = Tx0FinePowerCtrl;
			}

			//
			// Tx1 power control
			//
			if (pAd->Antenna.field.TxPath >= 2)
			{
				Tx1ALC = GET_TX_ALC_BIT_MASK & Power2.field.Byte0;
				Tx1FinePowerCtrl = ((GET_TX_FINE_POWER_CTRL_BIT_MASK & Power2.field.Byte0) >> NUMBER_OF_BITS_FOR_TX_ALC);

				if ((Tx1ALC > 31) || (Tx1ALC < 0))
				{
					pAd->TxPower[i * 2].Power2 = DEFAULT_RF_TX_POWER;
				}
				else
				{
					pAd->TxPower[i * 2].Power2 = Tx1ALC;
				}

				if ((Tx1FinePowerCtrl > 4) || (Tx1FinePowerCtrl < 0))
				{
					pAd->TxPower[i * 2].Tx1FinePowerCtrl = DEFAULT_BBP_TX_FINE_POWER_CTRL;
				}
				else
				{
					pAd->TxPower[i * 2].Tx1FinePowerCtrl = Tx1FinePowerCtrl;
				}

				Tx1ALC = GET_TX_ALC_BIT_MASK & Power2.field.Byte1;
				Tx1FinePowerCtrl = ((GET_TX_FINE_POWER_CTRL_BIT_MASK & Power2.field.Byte1) >> NUMBER_OF_BITS_FOR_TX_ALC);

				if ((Tx1ALC > 31) || (Tx1ALC < 0))
				{
					pAd->TxPower[i * 2 + 1].Power2 = DEFAULT_RF_TX_POWER;
				}
				else
				{
					pAd->TxPower[i * 2 + 1].Power2 = Tx1ALC;
				}

				if ((Tx1FinePowerCtrl > 4) || (Tx1FinePowerCtrl < 0))
				{
					pAd->TxPower[i * 2 + 1].Tx1FinePowerCtrl = DEFAULT_BBP_TX_FINE_POWER_CTRL;
				}
				else
				{
					pAd->TxPower[i * 2 + 1].Tx1FinePowerCtrl = Tx1FinePowerCtrl;
				}
			}

			DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPower[%d].Power = 0x%02X, pAd->TxPower[%d].Tx0FinePowerCtrl = 0x%02X, pAd->TxPower[%d].Power = 0x%02X, pAd->TxPower[%d].Tx0FinePowerCtrl = 0x%02X, pAd->TxPower[%d].Power2 = 0x%02X, pAd->TxPower[%d].Tx1FinePowerCtrl = 0x%02X, pAd->TxPower[%d].Power2 = 0x%02X, pAd->TxPower[%d].Tx1FinePowerCtrl = 0x%02X, \n", 
				__FUNCTION__, 
				i * 2, 
				pAd->TxPower[i * 2].Power, 
				i * 2, 
				pAd->TxPower[i * 2].Tx0FinePowerCtrl, 
				i * 2 + 1, 
				pAd->TxPower[i * 2 + 1].Power, 
				i * 2 + 1, 
				pAd->TxPower[i * 2 + 1].Tx0FinePowerCtrl, 
				i * 2, 
				pAd->TxPower[i * 2].Power2, 
				i * 2, 
				pAd->TxPower[i * 2].Tx1FinePowerCtrl, 
				i * 2 + 1, 
				pAd->TxPower[i * 2 + 1].Power2, 
				i * 2 + 1, 
				pAd->TxPower[i * 2 + 1].Tx1FinePowerCtrl));
		}
		else // Tx power control over RF R12 and RF R13
		{
		RT28xx_EEPROM_READ16(pAd, EEPROM_G_TX_PWR_OFFSET + i * 2, Power.word);
		RT28xx_EEPROM_READ16(pAd, EEPROM_G_TX2_PWR_OFFSET + i * 2, Power2.word);
		pAd->TxPower[i * 2].Channel = i * 2 + 1;
		pAd->TxPower[i * 2 + 1].Channel = i * 2 + 2;

			pAd->TxPower[i * 2].Power = Power.field.Byte0;
			if(!IS_RT3390(pAd))  // 3370 has different Tx power range
			{
		if ((Power.field.Byte0 > 31) || (Power.field.Byte0 < 0))
			pAd->TxPower[i * 2].Power = DEFAULT_RF_TX_POWER;
			}				

			pAd->TxPower[i * 2 + 1].Power = Power.field.Byte1;
			if(!IS_RT3390(pAd)) // 3370 has different Tx power range
			{
		if ((Power.field.Byte1 > 31) || (Power.field.Byte1 < 0))
			pAd->TxPower[i * 2 + 1].Power = DEFAULT_RF_TX_POWER;
			}				

		if ((Power2.field.Byte0 > 31) || (Power2.field.Byte0 < 0))
			pAd->TxPower[i * 2].Power2 = DEFAULT_RF_TX_POWER;
		else
			pAd->TxPower[i * 2].Power2 = Power2.field.Byte0;

		if ((Power2.field.Byte1 > 31) || (Power2.field.Byte1 < 0))
			pAd->TxPower[i * 2 + 1].Power2 = DEFAULT_RF_TX_POWER;
		else
			pAd->TxPower[i * 2 + 1].Power2 = Power2.field.Byte1;
	}
	}
#endif // RT3390 //
	// 1. U-NII lower/middle band: 36, 38, 40; 44, 46, 48; 52, 54, 56; 60, 62, 64 (including central frequency in BW 40MHz)
	// 1.1 Fill up channel
	choffset = 14;
	for (i = 0; i < 4; i++)
	{
		pAd->TxPower[3 * i + choffset + 0].Channel	= 36 + i * 8 + 0;
		pAd->TxPower[3 * i + choffset + 0].Power	= DEFAULT_RF_TX_POWER;
		pAd->TxPower[3 * i + choffset + 0].Power2	= DEFAULT_RF_TX_POWER;

		pAd->TxPower[3 * i + choffset + 1].Channel	= 36 + i * 8 + 2;
		pAd->TxPower[3 * i + choffset + 1].Power	= DEFAULT_RF_TX_POWER;
		pAd->TxPower[3 * i + choffset + 1].Power2	= DEFAULT_RF_TX_POWER;

		pAd->TxPower[3 * i + choffset + 2].Channel	= 36 + i * 8 + 4;
		pAd->TxPower[3 * i + choffset + 2].Power	= DEFAULT_RF_TX_POWER;
		pAd->TxPower[3 * i + choffset + 2].Power2	= DEFAULT_RF_TX_POWER;
	}

	// 1.2 Fill up power
	for (i = 0; i < 6; i++)
	{
		RT28xx_EEPROM_READ16(pAd, EEPROM_A_TX_PWR_OFFSET + i * 2, Power.word);
		RT28xx_EEPROM_READ16(pAd, EEPROM_A_TX2_PWR_OFFSET + i * 2, Power2.word);

		if ((Power.field.Byte0 < 16) && (Power.field.Byte0 >= -7))
			pAd->TxPower[i * 2 + choffset + 0].Power = Power.field.Byte0;

		if ((Power.field.Byte1 < 16) && (Power.field.Byte1 >= -7))
			pAd->TxPower[i * 2 + choffset + 1].Power = Power.field.Byte1;			

		if ((Power2.field.Byte0 < 16) && (Power2.field.Byte0 >= -7))
			pAd->TxPower[i * 2 + choffset + 0].Power2 = Power2.field.Byte0;

		if ((Power2.field.Byte1 < 16) && (Power2.field.Byte1 >= -7))
			pAd->TxPower[i * 2 + choffset + 1].Power2 = Power2.field.Byte1;			
	}
	
	// 2. HipperLAN 2 100, 102 ,104; 108, 110, 112; 116, 118, 120; 124, 126, 128; 132, 134, 136; 140 (including central frequency in BW 40MHz)
	// 2.1 Fill up channel
	choffset = 14 + 12;
	for (i = 0; i < 5; i++)
	{
		pAd->TxPower[3 * i + choffset + 0].Channel	= 100 + i * 8 + 0;
		pAd->TxPower[3 * i + choffset + 0].Power	= DEFAULT_RF_TX_POWER;
		pAd->TxPower[3 * i + choffset + 0].Power2	= DEFAULT_RF_TX_POWER;

		pAd->TxPower[3 * i + choffset + 1].Channel	= 100 + i * 8 + 2;
		pAd->TxPower[3 * i + choffset + 1].Power	= DEFAULT_RF_TX_POWER;
		pAd->TxPower[3 * i + choffset + 1].Power2	= DEFAULT_RF_TX_POWER;

		pAd->TxPower[3 * i + choffset + 2].Channel	= 100 + i * 8 + 4;
		pAd->TxPower[3 * i + choffset + 2].Power	= DEFAULT_RF_TX_POWER;
		pAd->TxPower[3 * i + choffset + 2].Power2	= DEFAULT_RF_TX_POWER;
	}
	pAd->TxPower[3 * 5 + choffset + 0].Channel		= 140;
	pAd->TxPower[3 * 5 + choffset + 0].Power		= DEFAULT_RF_TX_POWER;
	pAd->TxPower[3 * 5 + choffset + 0].Power2		= DEFAULT_RF_TX_POWER;

	// 2.2 Fill up power
	for (i = 0; i < 8; i++)
	{
		RT28xx_EEPROM_READ16(pAd, EEPROM_A_TX_PWR_OFFSET + (choffset - 14) + i * 2, Power.word);
		RT28xx_EEPROM_READ16(pAd, EEPROM_A_TX2_PWR_OFFSET + (choffset - 14) + i * 2, Power2.word);

		if ((Power.field.Byte0 < 16) && (Power.field.Byte0 >= -7))
			pAd->TxPower[i * 2 + choffset + 0].Power = Power.field.Byte0;

		if ((Power.field.Byte1 < 16) && (Power.field.Byte1 >= -7))
			pAd->TxPower[i * 2 + choffset + 1].Power = Power.field.Byte1;			

		if ((Power2.field.Byte0 < 16) && (Power2.field.Byte0 >= -7))
			pAd->TxPower[i * 2 + choffset + 0].Power2 = Power2.field.Byte0;

		if ((Power2.field.Byte1 < 16) && (Power2.field.Byte1 >= -7))
			pAd->TxPower[i * 2 + choffset + 1].Power2 = Power2.field.Byte1;			
	}

	// 3. U-NII upper band: 149, 151, 153; 157, 159, 161; 165, 167, 169; 171, 173 (including central frequency in BW 40MHz)
	// 3.1 Fill up channel
	choffset = 14 + 12 + 16;
	/*for (i = 0; i < 2; i++)*/
	for (i = 0; i < 3; i++)
	{
		pAd->TxPower[3 * i + choffset + 0].Channel	= 149 + i * 8 + 0;
		pAd->TxPower[3 * i + choffset + 0].Power	= DEFAULT_RF_TX_POWER;
		pAd->TxPower[3 * i + choffset + 0].Power2	= DEFAULT_RF_TX_POWER;

		pAd->TxPower[3 * i + choffset + 1].Channel	= 149 + i * 8 + 2;
		pAd->TxPower[3 * i + choffset + 1].Power	= DEFAULT_RF_TX_POWER;
		pAd->TxPower[3 * i + choffset + 1].Power2	= DEFAULT_RF_TX_POWER;

		pAd->TxPower[3 * i + choffset + 2].Channel	= 149 + i * 8 + 4;
		pAd->TxPower[3 * i + choffset + 2].Power	= DEFAULT_RF_TX_POWER;
		pAd->TxPower[3 * i + choffset + 2].Power2	= DEFAULT_RF_TX_POWER;
	}
	pAd->TxPower[3 * 3 + choffset + 0].Channel		= 171;
	pAd->TxPower[3 * 3 + choffset + 0].Power		= DEFAULT_RF_TX_POWER;
	pAd->TxPower[3 * 3 + choffset + 0].Power2		= DEFAULT_RF_TX_POWER;

	pAd->TxPower[3 * 3 + choffset + 1].Channel		= 173;
	pAd->TxPower[3 * 3 + choffset + 1].Power		= DEFAULT_RF_TX_POWER;
	pAd->TxPower[3 * 3 + choffset + 1].Power2		= DEFAULT_RF_TX_POWER;

	// 3.2 Fill up power
	/*for (i = 0; i < 4; i++)*/
	for (i = 0; i < 6; i++)
	{
		RT28xx_EEPROM_READ16(pAd, EEPROM_A_TX_PWR_OFFSET + (choffset - 14) + i * 2, Power.word);
		RT28xx_EEPROM_READ16(pAd, EEPROM_A_TX2_PWR_OFFSET + (choffset - 14) + i * 2, Power2.word);

		if ((Power.field.Byte0 < 16) && (Power.field.Byte0 >= -7))
			pAd->TxPower[i * 2 + choffset + 0].Power = Power.field.Byte0;

		if ((Power.field.Byte1 < 16) && (Power.field.Byte1 >= -7))
			pAd->TxPower[i * 2 + choffset + 1].Power = Power.field.Byte1;			

		if ((Power2.field.Byte0 < 16) && (Power2.field.Byte0 >= -7))
			pAd->TxPower[i * 2 + choffset + 0].Power2 = Power2.field.Byte0;

		if ((Power2.field.Byte1 < 16) && (Power2.field.Byte1 >= -7))
			pAd->TxPower[i * 2 + choffset + 1].Power2 = Power2.field.Byte1;			
	}

	// 4. Print and Debug
	/*choffset = 14 + 12 + 16 + 7;*/
	choffset = 14 + 12 + 16 + 11;
	

}


/*
	========================================================================
	
	Routine Description:
		Read initial parameters from EEPROM
		
	Arguments:
		Adapter						Pointer to our adapter

	Return Value:
		None

	IRQL = PASSIVE_LEVEL

	Note:
		
	========================================================================
*/
VOID	NICReadEEPROMParameters(
	IN	PRTMP_ADAPTER	pAd,
	IN	PUCHAR			mac_addr)
{
	UINT32			data = 0;
	USHORT			i, value, value2;
	UCHAR			TmpPhy;
	EEPROM_TX_PWR_STRUC	    Power;
	EEPROM_VERSION_STRUC    Version;
	EEPROM_ANTENNA_STRUC	Antenna;
	EEPROM_NIC_CONFIG2_STRUC    NicConfig2;
	USHORT  Addr01,Addr23,Addr45 ;
	MAC_DW0_STRUC csr2;
	MAC_DW1_STRUC csr3;


	DBGPRINT(RT_DEBUG_TRACE, ("--> NICReadEEPROMParameters\n"));	

	if (pAd->chipOps.eeinit)
	{
		pAd->chipOps.eeinit(pAd);
#ifdef RTMP_EFUSE_SUPPORT
#ifdef RT30xx
#ifdef RALINK_ATE
	if(!pAd->bFroceEEPROMBuffer && pAd->bEEPROMFile)
	{
		DBGPRINT(RT_DEBUG_TRACE, ("--> NICReadEEPROMParameters::(Efuse)Load to EEPROM Buffer Mode\n"));	
		eFuseLoadEEPROM(pAd);
	}
#endif // RALINK_ATE //
#endif // RT30xx //
#endif // RTMP_EFUSE_SUPPORT //
	}
	// Init EEPROM Address Number, before access EEPROM; if 93c46, EEPROMAddressNum=6, else if 93c66, EEPROMAddressNum=8
	RTMP_IO_READ32(pAd, E2PROM_CSR, &data);
	DBGPRINT(RT_DEBUG_TRACE, ("--> E2PROM_CSR = 0x%x\n", data));

	if((data & 0x30) == 0)
		pAd->EEPROMAddressNum = 6;		// 93C46
	else if((data & 0x30) == 0x10)
		pAd->EEPROMAddressNum = 8;     // 93C66
	else
		pAd->EEPROMAddressNum = 8;     // 93C86
	DBGPRINT(RT_DEBUG_TRACE, ("--> EEPROMAddressNum = %d\n", pAd->EEPROMAddressNum ));

	/* Read MAC setting from EEPROM and record as permanent MAC address */
	DBGPRINT(RT_DEBUG_TRACE, ("Initialize MAC Address from E2PROM \n"));

#ifdef TC_SOC
	tcSetMacAddr(pAd->PermanentAddress);
#else

	RT28xx_EEPROM_READ16(pAd, 0x04, Addr01);
	RT28xx_EEPROM_READ16(pAd, 0x06, Addr23);
	RT28xx_EEPROM_READ16(pAd, 0x08, Addr45);

	pAd->PermanentAddress[0] = (UCHAR)(Addr01 & 0xff);
	pAd->PermanentAddress[1] = (UCHAR)(Addr01 >> 8);
	pAd->PermanentAddress[2] = (UCHAR)(Addr23 & 0xff);
	pAd->PermanentAddress[3] = (UCHAR)(Addr23 >> 8);
	pAd->PermanentAddress[4] = (UCHAR)(Addr45 & 0xff);
	pAd->PermanentAddress[5] = (UCHAR)(Addr45 >> 8);
#endif

	//more conveninet to test mbssid, so ap's bssid &0xf1
	if (pAd->PermanentAddress[0] == 0xff)
		pAd->PermanentAddress[0] = RandomByte(pAd)&0xf8;
			
	DBGPRINT(RT_DEBUG_TRACE, ("E2PROM MAC: =%02x:%02x:%02x:%02x:%02x:%02x\n",
								PRINT_MAC(pAd->PermanentAddress)));

	/* Assign the actually working MAC Address */
	if (pAd->bLocalAdminMAC)
	{		
		DBGPRINT(RT_DEBUG_TRACE, ("Use the MAC address what is assigned from Configuration file(.dat). \n"));
	}
	else if (mac_addr && 
			 strlen((PSTRING)mac_addr) == 17 &&
			 (strcmp(mac_addr, "00:00:00:00:00:00") != 0))
	{
		INT		j;
		PSTRING	macptr;

		macptr = (PSTRING) mac_addr;

		for (j=0; j<MAC_ADDR_LEN; j++)
		{
			AtoH(macptr, &pAd->CurrentAddress[j], 1);
			macptr=macptr+3;
		}	
		
		DBGPRINT(RT_DEBUG_TRACE, ("Use the MAC address what is assigned from Moudle Parameter. \n"));
	}
	else
	{
		COPY_MAC_ADDR(pAd->CurrentAddress, pAd->PermanentAddress);
		DBGPRINT(RT_DEBUG_TRACE, ("Use the MAC address what is assigned from EEPROM. \n"));
	}

	/* Set the current MAC to ASIC */	
	csr2.field.Byte0 = pAd->CurrentAddress[0];
	csr2.field.Byte1 = pAd->CurrentAddress[1];
	csr2.field.Byte2 = pAd->CurrentAddress[2];
	csr2.field.Byte3 = pAd->CurrentAddress[3];
	RTMP_IO_WRITE32(pAd, MAC_ADDR_DW0, csr2.word);
	csr3.word = 0;
	csr3.field.Byte4 = pAd->CurrentAddress[4];
	csr3.field.Byte5 = pAd->CurrentAddress[5];
	csr3.field.U2MeMask = 0xff;
	RTMP_IO_WRITE32(pAd, MAC_ADDR_DW1, csr3.word);
	DBGPRINT_RAW(RT_DEBUG_TRACE,("Current MAC: =%02x:%02x:%02x:%02x:%02x:%02x\n",
					PRINT_MAC(pAd->CurrentAddress)));

	// if not return early. cause fail at emulation.
	// Init the channel number for TX channel power	
	RTMPReadChannelPwr(pAd);

	// if E2PROM version mismatch with driver's expectation, then skip
	// all subsequent E2RPOM retieval and set a system error bit to notify GUI
	RT28xx_EEPROM_READ16(pAd, EEPROM_VERSION_OFFSET, Version.word);
	pAd->EepromVersion = Version.field.Version + Version.field.FaeReleaseNumber * 256;
	DBGPRINT(RT_DEBUG_TRACE, ("E2PROM: Version = %d, FAE release #%d\n", Version.field.Version, Version.field.FaeReleaseNumber));

	if (Version.field.Version > VALID_EEPROM_VERSION)
	{
		DBGPRINT_ERR(("E2PROM: WRONG VERSION 0x%x, should be %d\n",Version.field.Version, VALID_EEPROM_VERSION));
		/*pAd->SystemErrorBitmap |= 0x00000001;

		// hard-code default value when no proper E2PROM installed
		pAd->bAutoTxAgcA = FALSE;
		pAd->bAutoTxAgcG = FALSE;

		// Default the channel power
		for (i = 0; i < MAX_NUM_OF_CHANNELS; i++)
			pAd->TxPower[i].Power = DEFAULT_RF_TX_POWER;

		// Default the channel power
		for (i = 0; i < MAX_NUM_OF_11JCHANNELS; i++)
			pAd->TxPower11J[i].Power = DEFAULT_RF_TX_POWER;
		
		for(i = 0; i < NUM_EEPROM_BBP_PARMS; i++)
			pAd->EEPROMDefaultValue[i] = 0xffff;
		return;  */
	}

	// Read BBP default value from EEPROM and store to array(EEPROMDefaultValue) in pAd
	RT28xx_EEPROM_READ16(pAd, EEPROM_NIC1_OFFSET, value);
	pAd->EEPROMDefaultValue[0] = value;

	RT28xx_EEPROM_READ16(pAd, EEPROM_NIC2_OFFSET, value);
	pAd->EEPROMDefaultValue[1] = value;

	RT28xx_EEPROM_READ16(pAd, EEPROM_COUNTRY_REGION, value);	// Country Region
	pAd->EEPROMDefaultValue[2] = value;

	for(i = 0; i < 8; i++)
	{
		RT28xx_EEPROM_READ16(pAd, EEPROM_BBP_BASE_OFFSET + i*2, value);
		pAd->EEPROMDefaultValue[i+3] = value;
	}

	// We have to parse NIC configuration 0 at here.
	// If TSSI did not have preloaded value, it should reset the TxAutoAgc to false
	// Therefore, we have to read TxAutoAgc control beforehand.
	// Read Tx AGC control bit
	Antenna.word = pAd->EEPROMDefaultValue[0];
	if (Antenna.word == 0xFFFF)
	{
#ifdef RT30xx
		if(IS_RT3090(pAd))
		{
			Antenna.word = 0;
			Antenna.field.RfIcType = RFIC_3020;
			Antenna.field.TxPath = 1;
			Antenna.field.RxPath = 1;		
		}
		else
#endif // RT30xx //
#ifdef RT33xx
		if(IS_RT3390(pAd))
		{
		    printk("NICReadEEPROMParameters : RFIC 3320\n");
			Antenna.word = 0;
			Antenna.field.RfIcType = RFIC_3320;
			Antenna.field.TxPath = 1;
			Antenna.field.RxPath = 1;		
		}
		else
#endif // RT33xx //
		{

			Antenna.word = 0;
			Antenna.field.RfIcType = RFIC_2820;
			Antenna.field.TxPath = 1;
			Antenna.field.RxPath = 2;
			DBGPRINT(RT_DEBUG_WARN, ("E2PROM error, hard code as 0x%04x\n", Antenna.word));
		}
	}

	// Choose the desired Tx&Rx stream.
	if ((pAd->CommonCfg.TxStream == 0) || (pAd->CommonCfg.TxStream > Antenna.field.TxPath))
		pAd->CommonCfg.TxStream = Antenna.field.TxPath;

	if ((pAd->CommonCfg.RxStream == 0) || (pAd->CommonCfg.RxStream > Antenna.field.RxPath))
	{
		pAd->CommonCfg.RxStream = Antenna.field.RxPath;
	
		if ((pAd->MACVersion != RALINK_2883_VERSION) &&
			(pAd->CommonCfg.RxStream > 2))
		{
			// only 2 Rx streams for RT2860 series
			pAd->CommonCfg.RxStream = 2;
		}
	}

	// 3*3
	// read value from EEPROM and set them to CSR174 ~ 177 in chain0 ~ chain2
	// yet implement
	for(i=0; i<3; i++)
	{
	}

	NicConfig2.word = pAd->EEPROMDefaultValue[1];

#ifdef WSC_INCLUDED
	/* WSC hardware push button function 0811 */
// TODO: Shiang, Need to check about the gpio value, it seems has issue in RT2880/RT3052 board.
	if ((pAd->MACVersion == 0x28600100) || (pAd->MACVersion == 0x28700100))
	WSC_HDR_BTN_MR_HDR_SUPPORT_SET(pAd, NicConfig2.field.EnableWPSPBC);
#endif // WSC_INCLUDED //

#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
	{
		if (NicConfig2.word == 0xffff)
		{
			NicConfig2.word = 0;
		}
	}
#endif // CONFIG_AP_SUPPORT //	


#ifdef A_BAND_SUPPORT
	if (NicConfig2.field.DynamicTxAgcControl == 1)
		pAd->bAutoTxAgcA = pAd->bAutoTxAgcG = TRUE;
	else
		pAd->bAutoTxAgcA = pAd->bAutoTxAgcG = FALSE;
#else
	if (NicConfig2.field.DynamicTxAgcControl == 1)
		pAd->bAutoTxAgcG = TRUE;
	else
		pAd->bAutoTxAgcG = FALSE;
#endif	
	
	DBGPRINT_RAW(RT_DEBUG_TRACE, ("NICReadEEPROMParameters: RxPath = %d, TxPath = %d\n", Antenna.field.RxPath, Antenna.field.TxPath));

	// Save the antenna for future use
	pAd->Antenna.word = Antenna.word;

	// Set the RfICType here, then we can initialize RFIC related operation callbacks
	pAd->Mlme.RealRxPath = (UCHAR) Antenna.field.RxPath;
	pAd->RfIcType = (UCHAR) Antenna.field.RfIcType;


#ifdef RTMP_RF_RW_SUPPORT
	RtmpChipOpsRFHook(pAd);
#endif // RTMP_RF_RW_SUPPORT //

	//
	// Reset PhyMode if we don't support 802.11a
	// Only RFIC_2850 & RFIC_2750 support 802.11a
	//
	if ((Antenna.field.RfIcType != RFIC_2850)
		&& (Antenna.field.RfIcType != RFIC_2750)
		&& (Antenna.field.RfIcType != RFIC_3052)
		)
	{
		if ((pAd->CommonCfg.PhyMode == PHY_11ABG_MIXED) || 
			(pAd->CommonCfg.PhyMode == PHY_11A))
			pAd->CommonCfg.PhyMode = PHY_11BG_MIXED;
#ifdef DOT11_N_SUPPORT
		else if ((pAd->CommonCfg.PhyMode == PHY_11ABGN_MIXED)	|| 
				 (pAd->CommonCfg.PhyMode == PHY_11AN_MIXED) 	|| 
				 (pAd->CommonCfg.PhyMode == PHY_11AGN_MIXED) 	||
				 (pAd->CommonCfg.PhyMode == PHY_11N_5G))
			pAd->CommonCfg.PhyMode = PHY_11BGN_MIXED;
#endif // DOT11_N_SUPPORT //
	}
	
	// Read TSSI reference and TSSI boundary for temperature compensation. This is ugly
	// 0. 11b/g
	{
		/* these are tempature reference value (0x00 ~ 0xFE)
		   ex: 0x00 0x15 0x25 0x45 0x88 0xA0 0xB5 0xD0 0xF0
		   TssiPlusBoundaryG [4] [3] [2] [1] [0] (smaller) +
		   TssiMinusBoundaryG[0] [1] [2] [3] [4] (larger) */
		RT28xx_EEPROM_READ16(pAd, EEPROM_G_TSSI_BOUND1, Power.word);
		pAd->TssiMinusBoundaryG[4] = Power.field.Byte0;
		pAd->TssiMinusBoundaryG[3] = Power.field.Byte1;
		RT28xx_EEPROM_READ16(pAd, EEPROM_G_TSSI_BOUND2, Power.word);
		pAd->TssiMinusBoundaryG[2] = Power.field.Byte0;
		pAd->TssiMinusBoundaryG[1] = Power.field.Byte1;
		RT28xx_EEPROM_READ16(pAd, EEPROM_G_TSSI_BOUND3, Power.word);
		pAd->TssiRefG   = Power.field.Byte0; /* reference value [0] */
		pAd->TssiPlusBoundaryG[1] = Power.field.Byte1;
		RT28xx_EEPROM_READ16(pAd, EEPROM_G_TSSI_BOUND4, Power.word);
		pAd->TssiPlusBoundaryG[2] = Power.field.Byte0;
		pAd->TssiPlusBoundaryG[3] = Power.field.Byte1;
		RT28xx_EEPROM_READ16(pAd, EEPROM_G_TSSI_BOUND5, Power.word);
		pAd->TssiPlusBoundaryG[4] = Power.field.Byte0;
		pAd->TxAgcStepG = Power.field.Byte1;    
		pAd->TxAgcCompensateG = 0;
		pAd->TssiMinusBoundaryG[0] = pAd->TssiRefG;
		pAd->TssiPlusBoundaryG[0]  = pAd->TssiRefG;

		// Disable TxAgc if the based value is not right
		if (pAd->TssiRefG == 0xff)
			pAd->bAutoTxAgcG = FALSE;

		DBGPRINT(RT_DEBUG_TRACE,("E2PROM: G Tssi[-4 .. +4] = %d %d %d %d - %d -%d %d %d %d, step=%d, tuning=%d\n",
			pAd->TssiMinusBoundaryG[4], pAd->TssiMinusBoundaryG[3], pAd->TssiMinusBoundaryG[2], pAd->TssiMinusBoundaryG[1],
			pAd->TssiRefG,
			pAd->TssiPlusBoundaryG[1], pAd->TssiPlusBoundaryG[2], pAd->TssiPlusBoundaryG[3], pAd->TssiPlusBoundaryG[4],
			pAd->TxAgcStepG, pAd->bAutoTxAgcG));
	}	
#ifdef A_BAND_SUPPORT	
	// 1. 11a
	{
		RT28xx_EEPROM_READ16(pAd, EEPROM_A_TSSI_BOUND1, Power.word);
		pAd->TssiMinusBoundaryA[4] = Power.field.Byte0;
		pAd->TssiMinusBoundaryA[3] = Power.field.Byte1;
		RT28xx_EEPROM_READ16(pAd, EEPROM_A_TSSI_BOUND2, Power.word);
		pAd->TssiMinusBoundaryA[2] = Power.field.Byte0;
		pAd->TssiMinusBoundaryA[1] = Power.field.Byte1;
		RT28xx_EEPROM_READ16(pAd, EEPROM_A_TSSI_BOUND3, Power.word);
		pAd->TssiRefA   = Power.field.Byte0;
		pAd->TssiPlusBoundaryA[1] = Power.field.Byte1;
		RT28xx_EEPROM_READ16(pAd, EEPROM_A_TSSI_BOUND4, Power.word);
		pAd->TssiPlusBoundaryA[2] = Power.field.Byte0;
		pAd->TssiPlusBoundaryA[3] = Power.field.Byte1;
		RT28xx_EEPROM_READ16(pAd, EEPROM_A_TSSI_BOUND5, Power.word);
		pAd->TssiPlusBoundaryA[4] = Power.field.Byte0;
		pAd->TxAgcStepA = Power.field.Byte1;    
		pAd->TxAgcCompensateA = 0;
		pAd->TssiMinusBoundaryA[0] = pAd->TssiRefA;
		pAd->TssiPlusBoundaryA[0]  = pAd->TssiRefA;

		// Disable TxAgc if the based value is not right
		if (pAd->TssiRefA == 0xff)
			pAd->bAutoTxAgcA = FALSE;

		DBGPRINT(RT_DEBUG_TRACE,("E2PROM: A Tssi[-4 .. +4] = %d %d %d %d - %d -%d %d %d %d, step=%d, tuning=%d\n",
			pAd->TssiMinusBoundaryA[4], pAd->TssiMinusBoundaryA[3], pAd->TssiMinusBoundaryA[2], pAd->TssiMinusBoundaryA[1],
			pAd->TssiRefA,
			pAd->TssiPlusBoundaryA[1], pAd->TssiPlusBoundaryA[2], pAd->TssiPlusBoundaryA[3], pAd->TssiPlusBoundaryA[4],
			pAd->TxAgcStepA, pAd->bAutoTxAgcA));
	}	
#endif // A_BAND_SUPPORT //
	pAd->BbpRssiToDbmDelta = 0x0;
	
	// Read frequency offset setting for RF
	RT28xx_EEPROM_READ16(pAd, EEPROM_FREQ_OFFSET, value);
	if ((value & 0x00FF) != 0x00FF)
		pAd->RfFreqOffset = (ULONG) (value & 0x00FF);
	else
		pAd->RfFreqOffset = 0;
	DBGPRINT(RT_DEBUG_TRACE, ("E2PROM: RF FreqOffset=0x%lx \n", pAd->RfFreqOffset));

	//CountryRegion byte offset (38h)
	value = pAd->EEPROMDefaultValue[2] >> 8;		// 2.4G band
	value2 = pAd->EEPROMDefaultValue[2] & 0x00FF;	// 5G band
	
	if ((value <= REGION_MAXIMUM_BG_BAND) && (value2 <= REGION_MAXIMUM_A_BAND))
	{
		pAd->CommonCfg.CountryRegion = ((UCHAR) value) | 0x80;
		pAd->CommonCfg.CountryRegionForABand = ((UCHAR) value2) | 0x80;
		TmpPhy = pAd->CommonCfg.PhyMode;
		pAd->CommonCfg.PhyMode = 0xff;
		RTMPSetPhyMode(pAd, TmpPhy);
#ifdef DOT11_N_SUPPORT
		SetCommonHT(pAd);
#endif // DOT11_N_SUPPORT //
	}

	//
	// Get RSSI Offset on EEPROM 0x9Ah & 0x9Ch.
	// The valid value are (-10 ~ 10) 
	// 
	RT28xx_EEPROM_READ16(pAd, EEPROM_RSSI_BG_OFFSET, value);
	pAd->BGRssiOffset0 = value & 0x00ff;
	pAd->BGRssiOffset1 = (value >> 8);
	RT28xx_EEPROM_READ16(pAd, EEPROM_RSSI_BG_OFFSET+2, value);
	pAd->BGRssiOffset2 = value & 0x00ff;
#ifdef A_BAND_SUPPORT	
	pAd->ALNAGain1 = (value >> 8);
#endif
	RT28xx_EEPROM_READ16(pAd, EEPROM_LNA_OFFSET, value);
	pAd->BLNAGain = value & 0x00ff;
#ifdef A_BAND_SUPPORT	
	pAd->ALNAGain0 = (value >> 8);
#endif
#ifdef RT3090
	// RT3090A's internal LNA gain is 0x0A// RT3090A's internal LNA gain is 0x0A
	if (IS_RT3090A(pAd))
	{
		pAd->BLNAGain = RT3090A_DEFAULT_INTERNAL_LNA_GAIN;
	}
#endif // RT3090 //
	// Validate 11b/g RSSI_0 offset.
	if ((pAd->BGRssiOffset0 < -10) || (pAd->BGRssiOffset0 > 10))
		pAd->BGRssiOffset0 = 0;

	// Validate 11b/g RSSI_1 offset.
	if ((pAd->BGRssiOffset1 < -10) || (pAd->BGRssiOffset1 > 10))
		pAd->BGRssiOffset1 = 0;

	// Validate 11b/g RSSI_2 offset.
	if ((pAd->BGRssiOffset2 < -10) || (pAd->BGRssiOffset2 > 10))
		pAd->BGRssiOffset2 = 0;
		
#ifdef A_BAND_SUPPORT		
	RT28xx_EEPROM_READ16(pAd, EEPROM_RSSI_A_OFFSET, value);
	pAd->ARssiOffset0 = value & 0x00ff;
	pAd->ARssiOffset1 = (value >> 8);
	RT28xx_EEPROM_READ16(pAd, (EEPROM_RSSI_A_OFFSET+2), value);
	pAd->ARssiOffset2 = value & 0x00ff;
	pAd->ALNAGain2 = (value >> 8);


	if (((UCHAR)pAd->ALNAGain1 == 0xFF) || (pAd->ALNAGain1 == 0x00))
		pAd->ALNAGain1 = pAd->ALNAGain0;
	if (((UCHAR)pAd->ALNAGain2 == 0xFF) || (pAd->ALNAGain2 == 0x00))
		pAd->ALNAGain2 = pAd->ALNAGain0;

	// Validate 11a RSSI_0 offset.
	if ((pAd->ARssiOffset0 < -10) || (pAd->ARssiOffset0 > 10))
		pAd->ARssiOffset0 = 0;

	// Validate 11a RSSI_1 offset.
	if ((pAd->ARssiOffset1 < -10) || (pAd->ARssiOffset1 > 10))
		pAd->ARssiOffset1 = 0;

	//Validate 11a RSSI_2 offset.
	if ((pAd->ARssiOffset2 < -10) || (pAd->ARssiOffset2 > 10))
		pAd->ARssiOffset2 = 0;
#endif // A_BAND_SUPPORT //

#ifdef RT30xx
	//
	// Get TX mixer gain setting
	// 0xff are invalid value
	// Note: RT30xX default value is 0x00 and will program to RF_R17 only when this value is not zero.
	//       RT359X / RT357x default value is 0x02
	//
	if (IS_RT30xx(pAd) || IS_RT3572(pAd) || IS_RT3390(pAd))
	{
		RT28xx_EEPROM_READ16(pAd, EEPROM_TXMIXER_GAIN_2_4G, value);
		pAd->TxMixerGain24G = 0;
		value &= 0x00ff;
		if (value != 0xff)
		{
			value &= 0x07;
			pAd->TxMixerGain24G = (UCHAR)value;
		}
	}
#endif // RT30xx //
	
#ifdef LED_CONTROL_SUPPORT	
	//
	// Get LED Setting.
	//
	RT28xx_EEPROM_READ16(pAd, EEPROM_FREQ_OFFSET, value);
	pAd->LedCntl.word = (value>>8);
	RT28xx_EEPROM_READ16(pAd, EEPROM_LED1_OFFSET, value);
	pAd->Led1 = value;
	RT28xx_EEPROM_READ16(pAd, EEPROM_LED2_OFFSET, value);
	pAd->Led2 = value;
	RT28xx_EEPROM_READ16(pAd, EEPROM_LED3_OFFSET, value);
	pAd->Led3 = value;
#endif // LED_CONTROL_SUPPORT //	
		
	RTMPReadTxPwrPerRate(pAd);

#ifdef SINGLE_SKU
	RT28xx_EEPROM_READ16(pAd, EEPROM_DEFINE_MAX_TXPWR, pAd->CommonCfg.DefineMaxTxPwr);
	if ((pAd->CommonCfg.DefineMaxTxPwr & 0xFF) <= 0x50 && pAd->CommonCfg.AntGain > 0 && pAd->CommonCfg.BandedgeDelta >= 0)
	{
		DBGPRINT(RT_DEBUG_TRACE, ("Single SKU Mode is enabled\n"));
		pAd->CommonCfg.bSKUMode = TRUE;
	}
	else
	{
		DBGPRINT(RT_DEBUG_TRACE, ("Single SKU Mode is disabled\n"));
		pAd->CommonCfg.bSKUMode = FALSE;
	}
#endif // SINGLE_SKU //

#ifdef RT30xx
#ifdef RTMP_EFUSE_SUPPORT
	RtmpEfuseSupportCheck(pAd);
#endif // RTMP_EFUSE_SUPPORT //
#endif // RT30xx //

	DBGPRINT(RT_DEBUG_TRACE, ("<-- NICReadEEPROMParameters\n"));
}


/*
	========================================================================
	
	Routine Description:
		Set default value from EEPROM
		
	Arguments:
		Adapter						Pointer to our adapter

	Return Value:
		None

	IRQL = PASSIVE_LEVEL
	
	Note:
		
	========================================================================
*/
VOID	NICInitAsicFromEEPROM(
	IN	PRTMP_ADAPTER	pAd)
{
	USHORT					i;
//	EEPROM_ANTENNA_STRUC	Antenna;
	EEPROM_NIC_CONFIG2_STRUC    NicConfig2;
	UCHAR	BBPR3 = 0;
	
	DBGPRINT(RT_DEBUG_TRACE, ("--> NICInitAsicFromEEPROM\n"));
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


	NicConfig2.word = pAd->EEPROMDefaultValue[1];

#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
	{
		if (NicConfig2.word == 0xffff)
		{
			NicConfig2.word = 0;
		}
	}
#endif // CONFIG_AP_SUPPORT //


	// Save the antenna for future use
	pAd->NicConfig2.word = NicConfig2.word;

#ifdef RT30xx
	// set default antenna as main
	if ((pAd->RfIcType == RFIC_2020) || (pAd->RfIcType == RFIC_3020) || (pAd->RfIcType == RFIC_3320))
		AsicSetRxAnt(pAd, pAd->RxAnt.Pair1PrimaryRxAnt);
#endif // RT30xx //

#ifdef LED_CONTROL_SUPPORT
	//
	// Send LED Setting to MCU.
	//
	if (pAd->LedCntl.word == 0xFF)
	{
		pAd->LedCntl.word = 0x01;
		pAd->Led1 = 0x5555;
		pAd->Led2 = 0x2221;

#ifdef RTMP_MAC_PCI
		pAd->Led3 = 0xA9F8;
#endif // RTMP_MAC_PCI //
	}

	AsicSendCommandToMcu(pAd, 0x52, 0xff, (UCHAR)pAd->Led1, (UCHAR)(pAd->Led1 >> 8));
	AsicSendCommandToMcu(pAd, 0x53, 0xff, (UCHAR)pAd->Led2, (UCHAR)(pAd->Led2 >> 8));
	AsicSendCommandToMcu(pAd, 0x54, 0xff, (UCHAR)pAd->Led3, (UCHAR)(pAd->Led3 >> 8));
	AsicSendCommandToMcu(pAd, 0x51, 0xff, 0, pAd->LedCntl.field.Polarity);
	
	pAd->LedIndicatorStrength = 0xFF;
	RTMPSetSignalLED(pAd, -100);	// Force signal strength Led to be turned off, before link up
#endif // LED_CONTROL_SUPPORT //

	// Turn off patching for cardbus controller
	if (NicConfig2.field.CardbusAcceleration == 1)
	{
//		pAd->bTest1 = TRUE;
	}
#ifdef A_BAND_SUPPORT
	if (NicConfig2.field.DynamicTxAgcControl == 1)
		pAd->bAutoTxAgcA = pAd->bAutoTxAgcG = TRUE;
	else
		pAd->bAutoTxAgcA = pAd->bAutoTxAgcG = FALSE;
#else
	if (NicConfig2.field.DynamicTxAgcControl == 1)
		pAd->bAutoTxAgcG = TRUE;
	else
		pAd->bAutoTxAgcG = FALSE;
#endif	

#ifdef RTMP_INTERNAL_TX_ALC
	// Internal Tx ALC
	if (((NicConfig2.field.DynamicTxAgcControl == 1) && 
            (NicConfig2.field.bInternalTxALC == 1)) || (!IS_RT3390(pAd)))
	{
		pAd->TxPowerCtrl.bInternalTxALC = FALSE;
	}
	else
	{
		if (NicConfig2.field.bInternalTxALC == 1)
		{
			pAd->TxPowerCtrl.bInternalTxALC = TRUE;
		}
		else
		{
			pAd->TxPowerCtrl.bInternalTxALC = FALSE;
		}
	}

	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPowerCtrl.bInternalTxALC = %d\n", 
		__FUNCTION__, 
		pAd->TxPowerCtrl.bInternalTxALC));
#endif // RTMP_INTERNAL_TX_ALC //

	//
	// Since BBP has been progamed, to make sure BBP setting will be 
	// upate inside of AsicAntennaSelect, so reset to UNKNOWN_BAND!!
	//
	pAd->CommonCfg.BandState = UNKNOWN_BAND;
	
	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &BBPR3);
	BBPR3 &= (~0x18);
	if(pAd->Antenna.field.RxPath == 3)
	{
		BBPR3 |= (0x10);
	}
	else if(pAd->Antenna.field.RxPath == 2)
	{
		BBPR3 |= (0x8);
	}
	else if(pAd->Antenna.field.RxPath == 1)
	{
		BBPR3 |= (0x0);
	}
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, BBPR3);


#ifdef RT30xx
	// update registers from EEPROM for RT3071 or later(3572/3562/3592).
	if (IS_RT3090(pAd) || IS_RT3572(pAd) || IS_RT3390(pAd))
	{
		UCHAR RegIdx, RegValue;
		USHORT value;

		// after RT3071, write BBP from EEPROM 0xF0 to 0x102
		for (i = 0xF0; i <= 0x102; i = i+2)
		{
			value = 0xFFFF;
			RT28xx_EEPROM_READ16(pAd, i, value);
			if ((value != 0xFFFF) && (value != 0))
			{
				RegIdx = (UCHAR)(value >> 8);
				RegValue  = (UCHAR)(value & 0xff);
				RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, RegIdx, RegValue);
				DBGPRINT(RT_DEBUG_TRACE, ("Update BBP Registers from EEPROM(0x%0x), BBP(0x%x) = 0x%x\n", i, RegIdx, RegValue));
			}
		}

		// after RT3071, write RF from EEPROM 0x104 to 0x116
		for (i = 0x104; i <= 0x116; i = i+2)
		{
			value = 0xFFFF;
			RT28xx_EEPROM_READ16(pAd, i, value);
			if ((value != 0xFFFF) && (value != 0))
			{
				RegIdx = (UCHAR)(value >> 8);
				RegValue  = (UCHAR)(value & 0xff);
				RT30xxWriteRFRegister(pAd, RegIdx, RegValue);
				DBGPRINT(RT_DEBUG_TRACE, ("Update RF Registers from EEPROM0x%x), BBP(0x%x) = 0x%x\n", i, RegIdx, RegValue));
			}
		}
	}
#endif // RT30xx //
#ifdef LED_CONTROL_SUPPORT
	DBGPRINT(RT_DEBUG_TRACE, ("TxPath = %d, RxPath = %d, RFIC=%d, Polar+LED mode=%x\n", 
				pAd->Antenna.field.TxPath, pAd->Antenna.field.RxPath, 
				pAd->RfIcType, pAd->LedCntl.word));
#else
	DBGPRINT(RT_DEBUG_TRACE, ("TxPath = %d, RxPath = %d, RFIC=%d, \n", 
				pAd->Antenna.field.TxPath, pAd->Antenna.field.RxPath, 
				pAd->RfIcType));
#endif // LED_CONTROL_SUPPORT //
	DBGPRINT(RT_DEBUG_TRACE, ("<-- NICInitAsicFromEEPROM\n"));
}

/*
	========================================================================
	
	Routine Description:
		Post-process the BBP registers based on the chip model

	Arguments:
		Adapter						Pointer to our adapter

	Return Value:
		None

	IRQL = PASSIVE_LEVEL

	Note:
		
	========================================================================
*/
VOID PostBBPInitialization(
	IN PRTMP_ADAPTER pAd)
{
	BBP_R105_STRUC BBPR105 = {0};
	BBP_R106_STRUC BBPR106 = {0};
	
	DBGPRINT(RT_DEBUG_TRACE, ("--> %s\n", __FUNCTION__));

//	RTUSBReadBBPRegister(pAd, BBP_R105, &BBPR105.byte);
	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R105, &BBPR105.byte);

	if (IS_RT3090A(pAd) || IS_RT3390(pAd))
	{
		// Disable re-modulation until further verification (poor Rx throughput with Atheros APs)
		/*
		BBPR105.field.EnableSIGRemodulation = 1; // The channel estimation updates based on remodulation of L-SIG and HT-SIG symbols.
		*/
		BBPR105.field.ADAPTIVE_FEQ_ENABLE = 1; // Enable the adaptive FEQ
		BBPR105.field.ADAPTIVE_FEQ_STEP = 0x01; // The step size for the adaptive FEQ
	}
	else
	{
		BBPR105.field.EnableSIGRemodulation = 0;
		BBPR105.field.ADAPTIVE_FEQ_ENABLE = 0;
		BBPR105.field.ADAPTIVE_FEQ_STEP = 0;
	}

	//
	// Apply Maximum Likelihood Detection (MLD) for 2 stream case (reserved field if single RX)
	//
	if (pAd->Antenna.field.RxPath == 1) // Single RX
	{
		BBPR105.field.MLDFor2Stream = 0;
	}
	else
	{
		BBPR105.field.MLDFor2Stream = 1;
	}

//	RTUSBWriteBBPRegister(pAd, BBP_R105, BBPR105.byte);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R105, BBPR105.byte);

	DBGPRINT(RT_DEBUG_TRACE, ("%s: BBP_R105: BBPR105.field.EnableSIGRemodulation = %d, BBPR105.field.ADAPTIVE_FEQ_ENABLE = %d, BBPR105.field.ADAPTIVE_FEQ_STEP = %d\n", 
		__FUNCTION__, 
		BBPR105.field.EnableSIGRemodulation, 
		BBPR105.field.ADAPTIVE_FEQ_ENABLE, 
		BBPR105.field.ADAPTIVE_FEQ_STEP));

	if (IS_RT3390(pAd) || IS_RT3572(pAd))
	{
		// Increase the possibility of using medium power to receive packets
//		RTUSBWriteBBPRegister(pAd, BBP_R83, 0x4A);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R83, 0x4A);
	}

	if (IS_RT3090A(pAd) || IS_RT3390(pAd))
	{
//		RTUSBReadBBPRegister(pAd, BBP_R106, &BBPR106.byte);
		RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R106, &BBPR106.byte);

		BBPR106.field.ShortGI_Offset20 = 0x06; // Delay GI remover when the short GI is detected in 20MHz band (20M sampling rate)
		BBPR106.field.ShortGI_Offset40 = 0x08; // Delay GI remover when the short GI is detected in 40MHz band (40M sampling rate)
		
//		RTUSBWriteBBPRegister(pAd, BBP_R106, BBPR106.byte);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R106, BBPR106.byte);

		DBGPRINT(RT_DEBUG_TRACE, ("%s: BBPR106: BBPR106.field.ShortGI_Offset20 = %d, BBPR106.field.ShortGI_Offset40 = %d\n", 
			__FUNCTION__, 
			BBPR106.field.ShortGI_Offset20, 
			BBPR106.field.ShortGI_Offset40));
	}

	DBGPRINT(RT_DEBUG_TRACE, ("<-- %s\n", __FUNCTION__));
}

/*
	========================================================================
	
	Routine Description:
		Initialize NIC hardware

	Arguments:
		Adapter						Pointer to our adapter

	Return Value:
		None

	IRQL = PASSIVE_LEVEL

	Note:
		
	========================================================================
*/
NDIS_STATUS	NICInitializeAdapter(
	IN	PRTMP_ADAPTER	pAd,
	IN   BOOLEAN    bHardReset)
{
	NDIS_STATUS     Status = NDIS_STATUS_SUCCESS;
	WPDMA_GLO_CFG_STRUC	GloCfg;
#ifdef RTMP_MAC_PCI
	UINT32			Value;
	DELAY_INT_CFG_STRUC	IntCfg;
#endif // RTMP_MAC_PCI //
//	INT_MASK_CSR_STRUC		IntMask;
	ULONG	i =0, j=0;
	AC_TXOP_CSR0_STRUC	csr0;

	DBGPRINT(RT_DEBUG_TRACE, ("--> NICInitializeAdapter\n"));
	
	// 3. Set DMA global configuration except TX_DMA_EN and RX_DMA_EN bits:
retry:
	i = 0;
	do
	{
		RTMP_IO_READ32(pAd, WPDMA_GLO_CFG, &GloCfg.word);
		if ((GloCfg.field.TxDMABusy == 0)  && (GloCfg.field.RxDMABusy == 0))
			break;
		
		RTMPusecDelay(1000);
		i++;
	}while ( i<100);
	DBGPRINT(RT_DEBUG_TRACE, ("<== DMA offset 0x208 = 0x%x\n", GloCfg.word));	
	GloCfg.word &= 0xff0;
	GloCfg.field.EnTXWriteBackDDONE =1;
	RTMP_IO_WRITE32(pAd, WPDMA_GLO_CFG, GloCfg.word);
	
	// Record HW Beacon offset
	pAd->BeaconOffset[0] = HW_BEACON_BASE0;
	pAd->BeaconOffset[1] = HW_BEACON_BASE1;
	pAd->BeaconOffset[2] = HW_BEACON_BASE2;
	pAd->BeaconOffset[3] = HW_BEACON_BASE3;
	pAd->BeaconOffset[4] = HW_BEACON_BASE4;
	pAd->BeaconOffset[5] = HW_BEACON_BASE5;
	pAd->BeaconOffset[6] = HW_BEACON_BASE6;
	pAd->BeaconOffset[7] = HW_BEACON_BASE7;
	
	//
	// write all shared Ring's base address into ASIC
	//

	// asic simulation sequence put this ahead before loading firmware.
	// pbf hardware reset
#ifdef RTMP_MAC_PCI
	RTMP_IO_WRITE32(pAd, WPDMA_RST_IDX, 0x1003f);	// 0x10000 for reset rx, 0x3f resets all 6 tx rings.
	RTMP_IO_WRITE32(pAd, PBF_SYS_CTRL, 0xe1f);
	RTMP_IO_WRITE32(pAd, PBF_SYS_CTRL, 0xe00);
#endif // RTMP_MAC_PCI //

	// Initialze ASIC for TX & Rx operation
	if (NICInitializeAsic(pAd , bHardReset) != NDIS_STATUS_SUCCESS)
	{
		if (j++ == 0)
		{
			NICLoadFirmware(pAd);
			goto retry;
		}
		return NDIS_STATUS_FAILURE;
	}


#ifdef RTMP_MAC_PCI
	// Write AC_BK base address register
	Value = RTMP_GetPhysicalAddressLow(pAd->TxRing[QID_AC_BK].Cell[0].AllocPa);
	RTMP_IO_WRITE32(pAd, TX_BASE_PTR1, Value);
	DBGPRINT(RT_DEBUG_TRACE, ("--> TX_BASE_PTR1 : 0x%x\n", Value));

	// Write AC_BE base address register
	Value = RTMP_GetPhysicalAddressLow(pAd->TxRing[QID_AC_BE].Cell[0].AllocPa);
	RTMP_IO_WRITE32(pAd, TX_BASE_PTR0, Value);
	DBGPRINT(RT_DEBUG_TRACE, ("--> TX_BASE_PTR0 : 0x%x\n", Value));

	// Write AC_VI base address register
	Value = RTMP_GetPhysicalAddressLow(pAd->TxRing[QID_AC_VI].Cell[0].AllocPa);
	RTMP_IO_WRITE32(pAd, TX_BASE_PTR2, Value);
	DBGPRINT(RT_DEBUG_TRACE, ("--> TX_BASE_PTR2 : 0x%x\n", Value));

	// Write AC_VO base address register
	Value = RTMP_GetPhysicalAddressLow(pAd->TxRing[QID_AC_VO].Cell[0].AllocPa);	
	RTMP_IO_WRITE32(pAd, TX_BASE_PTR3, Value);
	DBGPRINT(RT_DEBUG_TRACE, ("--> TX_BASE_PTR3 : 0x%x\n", Value));

	// Write HCCA base address register
	  Value = RTMP_GetPhysicalAddressLow(pAd->TxRing[QID_HCCA].Cell[0].AllocPa);
	  RTMP_IO_WRITE32(pAd, TX_BASE_PTR4, Value);
	DBGPRINT(RT_DEBUG_TRACE, ("--> TX_BASE_PTR4 : 0x%x\n", Value));

	// Write MGMT_BASE_CSR register
	Value = RTMP_GetPhysicalAddressLow(pAd->MgmtRing.Cell[0].AllocPa);
	RTMP_IO_WRITE32(pAd, TX_BASE_PTR5, Value);
	DBGPRINT(RT_DEBUG_TRACE, ("--> TX_BASE_PTR5 : 0x%x\n", Value));

	// Write RX_BASE_CSR register
	Value = RTMP_GetPhysicalAddressLow(pAd->RxRing.Cell[0].AllocPa);
	RTMP_IO_WRITE32(pAd, RX_BASE_PTR, Value);
	DBGPRINT(RT_DEBUG_TRACE, ("--> RX_BASE_PTR : 0x%x\n", Value));

	// Init RX Ring index pointer
	pAd->RxRing.RxSwReadIdx = 0;
	pAd->RxRing.RxCpuIdx = RX_RING_SIZE-1;
	RTMP_IO_WRITE32(pAd, RX_CRX_IDX, pAd->RxRing.RxCpuIdx);
	
	// Init TX rings index pointer
	{
		for (i=0; i<NUM_OF_TX_RING; i++)
		{
			pAd->TxRing[i].TxSwFreeIdx = 0;
			pAd->TxRing[i].TxCpuIdx = 0;
			RTMP_IO_WRITE32(pAd, (TX_CTX_IDX0 + i * 0x10) ,  pAd->TxRing[i].TxCpuIdx);
		}
	}

	// init MGMT ring index pointer
	pAd->MgmtRing.TxSwFreeIdx = 0;
	pAd->MgmtRing.TxCpuIdx = 0;
	RTMP_IO_WRITE32(pAd, TX_MGMTCTX_IDX,  pAd->MgmtRing.TxCpuIdx);

	//
	// set each Ring's SIZE  into ASIC. Descriptor Size is fixed by design.
	//

	// Write TX_RING_CSR0 register
	Value = TX_RING_SIZE;
	RTMP_IO_WRITE32(pAd, TX_MAX_CNT0, Value);
	RTMP_IO_WRITE32(pAd, TX_MAX_CNT1, Value);
	RTMP_IO_WRITE32(pAd, TX_MAX_CNT2, Value);
	RTMP_IO_WRITE32(pAd, TX_MAX_CNT3, Value);
	RTMP_IO_WRITE32(pAd, TX_MAX_CNT4, Value);
	Value = MGMT_RING_SIZE;
	RTMP_IO_WRITE32(pAd, TX_MGMTMAX_CNT, Value);

	// Write RX_RING_CSR register
	Value = RX_RING_SIZE;
	RTMP_IO_WRITE32(pAd, RX_MAX_CNT, Value);
#endif // RTMP_MAC_PCI //


	// WMM parameter
	csr0.word = 0;
	RTMP_IO_WRITE32(pAd, WMM_TXOP0_CFG, csr0.word);
	if (pAd->CommonCfg.PhyMode == PHY_11B)
	{
		csr0.field.Ac0Txop = 192;	// AC_VI: 192*32us ~= 6ms
		csr0.field.Ac1Txop = 96;	// AC_VO: 96*32us  ~= 3ms
	}
	else
	{
		csr0.field.Ac0Txop = 96;	// AC_VI: 96*32us ~= 3ms
		csr0.field.Ac1Txop = 48;	// AC_VO: 48*32us ~= 1.5ms
	}
	RTMP_IO_WRITE32(pAd, WMM_TXOP1_CFG, csr0.word);


#ifdef RTMP_MAC_PCI
	// 3. Set DMA global configuration except TX_DMA_EN and RX_DMA_EN bits:
	i = 0;
	do
	{
		RTMP_IO_READ32(pAd, WPDMA_GLO_CFG, &GloCfg.word);
		if ((GloCfg.field.TxDMABusy == 0)  && (GloCfg.field.RxDMABusy == 0))
			break;
		
		RTMPusecDelay(1000);
		i++;
	}while ( i < 100);

	GloCfg.word &= 0xff0;
	GloCfg.field.EnTXWriteBackDDONE =1;
	RTMP_IO_WRITE32(pAd, WPDMA_GLO_CFG, GloCfg.word);
	
	IntCfg.word = 0;
	RTMP_IO_WRITE32(pAd, DELAY_INT_CFG, IntCfg.word);
#endif // RTMP_MAC_PCI //


	// reset action
	// Load firmware
	//  Status = NICLoadFirmware(pAd);

	DBGPRINT(RT_DEBUG_TRACE, ("<-- NICInitializeAdapter\n"));
	return Status;
}

/*
	========================================================================
	
	Routine Description:
		Initialize ASIC

	Arguments:
		Adapter						Pointer to our adapter

	Return Value:
		None

	IRQL = PASSIVE_LEVEL

	Note:
		
	========================================================================
*/
NDIS_STATUS	NICInitializeAsic(
	IN	PRTMP_ADAPTER	pAd,
	IN  BOOLEAN		bHardReset)
{
	ULONG			Index = 0;
	UCHAR			R0 = 0xff;
	UINT32			MacCsr12 = 0, Counter = 0;
#ifdef RT30xx
	UCHAR			bbpreg=0;
	UCHAR			RFValue=0;
#endif // RT30xx //
	USHORT			KeyIdx;
	INT				i,apidx;

	DBGPRINT(RT_DEBUG_TRACE, ("--> NICInitializeAsic\n"));

#ifdef RTMP_MAC_PCI
	RTMP_IO_WRITE32(pAd, PWR_PIN_CFG, 0x3);	// To fix driver disable/enable hang issue when radio off
	if (bHardReset == TRUE)
	{
		RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, 0x3);
	}
	else
		RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, 0x1);

	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, 0x0);
	// Initialize MAC register to default value
	for (Index = 0; Index < NUM_MAC_REG_PARMS; Index++)
	{
#ifdef RT30xx
		if ((MACRegTable[Index].Register == TX_SW_CFG0) && ( IS_RT3090(pAd) ||  IS_RT3390(pAd)))
		{
			MACRegTable[Index].Value = 0x00000400;
		}
#endif // RT30xx //
		RTMP_IO_WRITE32(pAd, MACRegTable[Index].Register, MACRegTable[Index].Value);
	}

#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
	{
		for (Index = 0; Index < NUM_AP_MAC_REG_PARMS; Index++)
		{
			RTMP_IO_WRITE32(pAd, APMACRegTable[Index].Register, APMACRegTable[Index].Value);
		}
	}
#endif // CONFIG_AP_SUPPORT //

#endif // RTMP_MAC_PCI //


#ifdef RT30xx
	// Initialize RT3070 serial MAC registers which is different from RT2870 serial
	if (IS_RT3090(pAd) || IS_RT3572(pAd) ||IS_RT3390(pAd))
	{
		RTMP_IO_WRITE32(pAd, TX_SW_CFG1, 0);
		
		// RT3071 version E has fixed this issue
		if ((pAd->MACVersion & 0xffff) < 0x0211)
		{
			if (pAd->NicConfig2.field.DACTestBit == 1)
			{
				RTMP_IO_WRITE32(pAd, TX_SW_CFG2, 0x2C);	// To fix throughput drop drastically
			}
			else
			{
				RTMP_IO_WRITE32(pAd, TX_SW_CFG2, 0x0F);	// To fix throughput drop drastically
			}
		}
		else
		{
			RTMP_IO_WRITE32(pAd, TX_SW_CFG2, 0x0);
		}
	}
	else if (IS_RT3070(pAd))
	{
		if (((pAd->MACVersion & 0xffff) < 0x0201))
		{
			RTMP_IO_WRITE32(pAd, TX_SW_CFG1, 0);
			RTMP_IO_WRITE32(pAd, TX_SW_CFG2, 0x2C);	// To fix throughput drop drastically
		}
		else
		{
			RTMP_IO_WRITE32(pAd, TX_SW_CFG2, 0);
		}
	}
#endif // RT30xx //

	//
	// Before program BBP, we need to wait BBP/RF get wake up.
	//
	Index = 0;
	do
	{
		RTMP_IO_READ32(pAd, MAC_STATUS_CFG, &MacCsr12);

		if ((MacCsr12 & 0x03) == 0)	// if BB.RF is stable
			break;
		
		DBGPRINT(RT_DEBUG_TRACE, ("Check MAC_STATUS_CFG  = Busy = %x\n", MacCsr12));
		RTMPusecDelay(1000);
	} while (Index++ < 100);

#ifdef RTMP_MAC_PCI
	// The commands to firmware should be after these commands, these commands will init firmware
	// PCI and USB are not the same because PCI driver needs to wait for PCI bus ready
	RTMP_IO_WRITE32(pAd, H2M_BBP_AGENT, 0);	// initialize BBP R/W access agent
	RTMP_IO_WRITE32(pAd, H2M_MAILBOX_CSR, 0);
	RTMP_IO_WRITE32(pAd, H2M_INT_SRC, 0);
	AsicSendCommandToMcu(pAd, 0x72, 0, 0, 0);
#endif // RTMP_MAC_PCI //

	RTMPusecDelay(1000);

	// Read BBP register, make sure BBP is up and running before write new data
	Index = 0;
	do 
	{
		RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R0, &R0);
		DBGPRINT(RT_DEBUG_TRACE, ("BBP version = %x\n", R0));
	} while ((++Index < 20) && ((R0 == 0xff) || (R0 == 0x00)));
	//ASSERT(Index < 20); //this will cause BSOD on Check-build driver

	if ((R0 == 0xff) || (R0 == 0x00))
		return NDIS_STATUS_FAILURE;

	// Initialize BBP register to default value
	for (Index = 0; Index < NUM_BBP_REG_PARMS; Index++)
	{
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBPRegTable[Index].Register, BBPRegTable[Index].Value);
	}


#ifdef RTMP_MAC_PCI
	// TODO: shiang, check MACVersion, currently, rbus-based chip use this.
	if (pAd->MACVersion == 0x28720200)
	{
		//UCHAR value;
		ULONG value2;

		//disable MLD by Bruce 20080704
		//BBP_IO_READ8_BY_REG_ID(pAd, BBP_R105, &value);
		//BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R105, value | 4);
	
		//Maximum PSDU length from 16K to 32K bytes	
		RTMP_IO_READ32(pAd, MAX_LEN_CFG, &value2);
		value2 &= ~(0x3<<12);
		value2 |= (0x2<<12);
		RTMP_IO_WRITE32(pAd, MAX_LEN_CFG, value2);
	}
#endif // RTMP_MAC_PCI //

	// for rt2860E and after, init BBP_R84 with 0x19. This is for extension channel overlapping IOT.
	// RT3090 should not program BBP R84 to 0x19, otherwise TX will block.
	//3070/71/72,3090,3090A( are included in RT30xx),3572,3390
	if (((pAd->MACVersion & 0xffff) != 0x0101) && !(IS_RT30xx(pAd)|| IS_RT3572(pAd) || IS_RT3390(pAd)))
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R84, 0x19);

#ifdef RT30xx
	// patch RSSI inaccurate issue, due to design change
	if (IS_RT30xx(pAd) || IS_RT3572(pAd) || IS_RT3390(pAd))
	{	//update for RT3070/71/72/90/91/92,3572,3390.
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R79, 0x13);		
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R80, 0x05);	
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R81, 0x33);	
	}

	if (IS_RT3090(pAd) ||IS_RT3390(pAd))	// RT309x, RT3071/72
	{
		// enable DC filter
		if ((pAd->MACVersion & 0xffff) >= 0x0211)
		{
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R103, 0xc0);
		}

		// improve power consumption in RT3071 Ver.E 
		if ((pAd->MACVersion & 0xffff) >= 0x0211)
		{
			RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R31, &bbpreg);
			bbpreg &= (~0x3);
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R31, bbpreg);
		}
	}
	else if (IS_RT3070(pAd))
	{	
		if ((pAd->MACVersion & 0xffff) >= 0x0201)
		{
			// enable DC filter
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R103, 0xc0);
			
			// improve power consumption in RT3070 Ver.F
			RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R31, &bbpreg);
			bbpreg &= (~0x3);
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R31, bbpreg);
		}

	}
#endif // RT30xx //

	if (pAd->MACVersion == 0x28600100)
	{
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R69, 0x16);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R73, 0x12);
	}
	
	if (pAd->MACVersion >= RALINK_2880E_VERSION && pAd->MACVersion < RALINK_3070_VERSION) // 3*3
	{
		// enlarge MAX_LEN_CFG
		UINT32 csr;
		RTMP_IO_READ32(pAd, MAX_LEN_CFG, &csr);
		{
		csr &= 0xFFF;
		csr |= 0x2000;
		}
		RTMP_IO_WRITE32(pAd, MAX_LEN_CFG, csr);
	}



	// Clear raw counters
	RTMP_IO_READ32(pAd, RX_STA_CNT0, &Counter);
	RTMP_IO_READ32(pAd, RX_STA_CNT1, &Counter);
	RTMP_IO_READ32(pAd, RX_STA_CNT2, &Counter);
	RTMP_IO_READ32(pAd, TX_STA_CNT0, &Counter);
	RTMP_IO_READ32(pAd, TX_STA_CNT1, &Counter);
	RTMP_IO_READ32(pAd, TX_STA_CNT2, &Counter);
	
	// ASIC will keep garbage value after boot
	// Clear all shared key table when initial
	// This routine can be ignored in radio-ON/OFF operation. 
	if (bHardReset)
	{
		for (KeyIdx = 0; KeyIdx < 4; KeyIdx++)
		{
			RTMP_IO_WRITE32(pAd, SHARED_KEY_MODE_BASE + 4*KeyIdx, 0);
		}

		// Clear all pairwise key table when initial
		for (KeyIdx = 0; KeyIdx < 256; KeyIdx++)
		{
			RTMP_IO_WRITE32(pAd, MAC_WCID_ATTRIBUTE_BASE + (KeyIdx * HW_WCID_ATTRI_SIZE), 1);
		}
	}
	
	// assert HOST ready bit
//  RTMP_IO_WRITE32(pAd, MAC_CSR1, 0x0); // 2004-09-14 asked by Mark
//  RTMP_IO_WRITE32(pAd, MAC_CSR1, 0x4);

	// It isn't necessary to clear this space when not hard reset. 	
	if (bHardReset == TRUE)
	{
		// clear all on-chip BEACON frame space			
		for (apidx = 0; apidx < HW_BEACON_MAX_COUNT; apidx++)
		{
			for (i = 0; i < HW_BEACON_OFFSET>>2; i+=4)
				RTMP_IO_WRITE32(pAd, pAd->BeaconOffset[apidx] + i, 0x00); 
#ifdef CONFIG_AP_SUPPORT
#endif // CONFIG_AP_SUPPORT //
		}
	}
	


#ifdef RT30xx
	if (IS_RT3070(pAd) ||  IS_RT3572(pAd)||IS_RT3390(pAd) ||IS_RT3090(pAd))
	{
		// send 0x36 mcu command after 0x72 for RT3xxx to fix Radio-Off current leakage issue
		RTMPusecDelay(200);
		if (pAd->bUseEfuse)
			AsicSendCommandToMcu(pAd, 0x36, 0xff, 0xff, 0);
		else
			AsicSendCommandToMcu(pAd, 0x36, 0xff, 0xff, 0x04);
		RTMPusecDelay(10);
	}
#endif // RT30xx //

	DBGPRINT(RT_DEBUG_TRACE, ("<-- NICInitializeAsic\n"));
	return NDIS_STATUS_SUCCESS;
}


#ifdef CONFIG_AP_SUPPORT
#ifdef RTMP_MAC_PCI
/*
	========================================================================
	
	Routine Description:
		In the case, Client may be silent left without sending DeAuth or DeAssoc.
		AP'll continue retry packets for the client since AP doesn't know the STA
		is gone. To Minimum affection of exist traffic is disable retransmition for
		all those packet relative to the STA.
		So decide to change ack required setting of all packet in TX ring
		to "no ACK" requirement for specific Client.

	Arguments:
		Adapter						Pointer to our adapter

	Return Value:
		None

	IRQL = DISPATCH_LEVEL
	
	========================================================================
*/
static VOID ClearTxRingClientAck(
	IN PRTMP_ADAPTER pAd,
	IN MAC_TABLE_ENTRY *pEntry)

{
	INT index;
	USHORT TxIdx;
	PRTMP_TX_RING pTxRing;
	PTXD_STRUC pTxD;
	PTXWI_STRUC pTxWI;

	if (!pAd || !pEntry)
		return;

	for (index = 3; index >= 0; index--)
	{
		pTxRing = &pAd->TxRing[index];
		for (TxIdx = 0; TxIdx < TX_RING_SIZE; TxIdx++)
		{
			pTxD = (PTXD_STRUC) pTxRing->Cell[TxIdx].AllocVa;
			if (!pTxD->DMADONE)
			{
				 pTxWI = (PTXWI_STRUC) pTxRing->Cell[TxIdx].DmaBuf.AllocVa;
				 if (pTxWI->WirelessCliID == pEntry->Aid)
					pTxWI->ACK = FALSE;
			}
		}
	}
}
#endif // RTMP_MAC_PCI //
#endif // CONFIG_AP_SUPPORT //

VOID NICUpdateFifoStaCounters(
	IN PRTMP_ADAPTER pAd)
{
	TX_STA_FIFO_STRUC	StaFifo;
	MAC_TABLE_ENTRY		*pEntry;
	UCHAR				i = 0;
	UCHAR			pid = 0, wcid = 0;
	CHAR				reTry;
	UCHAR				succMCS;
#ifdef CONFIG_AP_SUPPORT
	PMULTISSID_STRUCT	pMbss ;
#endif//CONFIG_AP_SUPPORT//

#ifdef RALINK_ATE		
	/* Nothing to do in ATE mode */
	if (ATE_ON(pAd))
		return;
#endif // RALINK_ATE //

		do
		{
			RTMP_IO_READ32(pAd, TX_STA_FIFO, &StaFifo.word);

			if (StaFifo.field.bValid == 0)
				break;
		
			wcid = (UCHAR)StaFifo.field.wcid;

#ifdef CONFIG_AP_SUPPORT
			IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
			{
#ifdef DFS_SUPPORT
				/* the CTS frame for DFS and Carrier-Sense. */
				if ((pAd->CommonCfg.Channel > 14)
					&& (pAd->CommonCfg.bIEEE80211H == TRUE)
					&& (pAd->CommonCfg.RadarDetect.RDMode == RD_NORMAL_MODE)
					&& (wcid == DFS_CTS_WCID))
				{
					i++;
					DFSStartTrigger(pAd);
					continue;
				}
#endif // DFS_SUPPORT //

#ifdef CARRIER_DETECTION_SUPPORT
				if ((pAd->CommonCfg.CarrierDetect.Enable == TRUE)
					&& (wcid == CS_CTS_WCID))
				{
					i++;
					CarrierDetectStartTrigger(pAd);
					continue;
				}
#endif // CARRIER_DETECTION_SUPPORT //
			}
#endif // CONFIG_AP_SUPPORT //

		/* ignore NoACK and MGMT frame use 0xFF as WCID */
			if ((StaFifo.field.TxAckRequired == 0) || (wcid >= MAX_LEN_OF_MAC_TABLE))
			{
				i++;
				continue;
			}

			/* PID store Tx MCS Rate */
			pid = (UCHAR)StaFifo.field.PidType;

			pEntry = &pAd->MacTab.Content[wcid];

			pEntry->DebugFIFOCount++;

#ifdef DOT11_N_SUPPORT
			if (StaFifo.field.TxBF) // 3*3
				pEntry->TxBFCount++;
#endif // DOT11_N_SUPPORT //

#ifdef UAPSD_AP_SUPPORT
			UAPSD_SP_AUE_Handle(pAd, pEntry, StaFifo.field.TxSuccess);
#endif // UAPSD_AP_SUPPORT //


			if (!StaFifo.field.TxSuccess)
			{
#ifdef CONFIG_AP_SUPPORT
#ifdef WDS_SUPPORT
				if(IS_ENTRY_WDS(pEntry))
					pAd->WdsTab.WdsEntry[pEntry->MatchWDSTabIdx].WdsCounter.TxErrors++;
#endif


				pMbss = &pAd->ApCfg.MBSSID[pEntry->apidx];
				pMbss->TxErrorCount++;
#endif//CONFIG_AP_SUPPORT//
				pEntry->FIFOCount++;
				pEntry->OneSecTxFailCount++;
									
				if (pEntry->FIFOCount >= 1)
				{			
					DBGPRINT(RT_DEBUG_TRACE, ("#"));
#ifdef DOT11_N_SUPPORT
					pEntry->NoBADataCountDown = 64;
#endif // DOT11_N_SUPPORT //

//#ifdef CONFIG_STA_SUPPORT
//#ifdef DOT11Z_TDLS_SUPPORT
//					IF_DEV_CONFIG_OPMODE_ON_STA(pAd)
//					{
//						if(IS_ENTRY_TDLS(pEntry))
//							TDLS_LinkTearDown(pAd);
//					}
//#endif // DOT11Z_TDLS_SUPPORT //
//#endif // CONFIG_STA_SUPPORT //

					if(pEntry->PsMode == PWR_ACTIVE)
					{
#ifdef DOT11_N_SUPPORT					
						int tid;
						for (tid=0; tid<NUM_OF_TID; tid++)
						{
							BAOriSessionTearDown(pAd, pEntry->Aid,  tid, FALSE, FALSE);
						}
#endif // DOT11_N_SUPPORT //

						// Update the continuous transmission counter except PS mode
						pEntry->ContinueTxFailCnt++;

#ifdef WDS_SUPPORT
						// fix WDS Jam issue
						if(IS_ENTRY_WDS(pEntry)
							&& (pEntry->LockEntryTx == FALSE)
							&& (pEntry->ContinueTxFailCnt >= pAd->ApCfg.EntryLifeCheck))
						{ 
							DBGPRINT(RT_DEBUG_TRACE, ("Entry %02x:%02x:%02x:%02x:%02x:%02x Blocked!! (Fail Cnt = %d)\n",
								pEntry->Addr[0],pEntry->Addr[1],pEntry->Addr[2],pEntry->Addr[3],
								pEntry->Addr[4],pEntry->Addr[5],pEntry->ContinueTxFailCnt ));

							pEntry->LockEntryTx = TRUE;
						}
#endif // WDS_SUPPORT //
#ifdef CONFIG_AP_SUPPORT
#ifdef RTMP_MAC_PCI
						// if Tx fail >= 20, then clear TXWI ack in Tx Ring
						if (pEntry->ContinueTxFailCnt >= pAd->ApCfg.EntryLifeCheck)
							ClearTxRingClientAck(pAd, pEntry);
#endif // RTMP_MAC_PCI //
#endif // CONFIG_AP_SUPPORT //
					}
					else
					{    
						// Clear the FIFOCount when sta in Power Save mode. Basically we assume 
						//     this tx error happened due to sta just go to sleep.
						pEntry->FIFOCount = 0;
						pEntry->ContinueTxFailCnt = 0;
					}
					//pEntry->FIFOCount = 0;
				}
				//pEntry->bSendBAR = TRUE;
			}
			else
			{
#ifdef DOT11_N_SUPPORT
				if ((pEntry->PsMode != PWR_SAVE) && (pEntry->NoBADataCountDown > 0))
				{
					pEntry->NoBADataCountDown--;
					if (pEntry->NoBADataCountDown==0)
					{
						DBGPRINT(RT_DEBUG_TRACE, ("@\n"));
					}
				}
#endif // DOT11_N_SUPPORT //
				pEntry->FIFOCount = 0;
				pEntry->OneSecTxNoRetryOkCount++;
				// update NoDataIdleCount when sucessful send packet to STA.
				pEntry->NoDataIdleCount = 0;
				pEntry->ContinueTxFailCnt = 0;
#ifdef WDS_SUPPORT
				pEntry->LockEntryTx = FALSE;
#endif // WDS_SUPPORT //
			}

			succMCS = StaFifo.field.SuccessRate & 0x7F;


			reTry = pid - succMCS;

			if (StaFifo.field.TxSuccess)
			{
				pEntry->TXMCSExpected[pid]++;
				if (pid == succMCS)
				{
					pEntry->TXMCSSuccessful[pid]++;
				}
				else 
				{
					pEntry->TXMCSAutoFallBack[pid][succMCS]++;
				}
			}
			else
			{
				pEntry->TXMCSFailed[pid]++;
			}

			if (reTry > 0)
			{
				if ((pid >= 12) && succMCS <=7)
				{
					reTry -= 4;
				} 
				pEntry->OneSecTxRetryOkCount += reTry;
			}

			i++;
			// ASIC store 16 stack
		} while ( i < (TX_RING_SIZE<<1) );

}

/*
	========================================================================
	
	Routine Description:
		Read statistical counters from hardware registers and record them
		in software variables for later on query

	Arguments:
		pAd					Pointer to our adapter

	Return Value:
		None

	IRQL = DISPATCH_LEVEL
	
	========================================================================
*/
VOID NICUpdateRawCounters(
	IN PRTMP_ADAPTER pAd)
{
	UINT32	OldValue;//, Value2;
	//ULONG	PageSum, OneSecTransmitCount;
	//ULONG	TxErrorRatio, Retry, Fail;
	RX_STA_CNT0_STRUC	 RxStaCnt0;
	RX_STA_CNT1_STRUC   RxStaCnt1;
	RX_STA_CNT2_STRUC   RxStaCnt2;
	TX_STA_CNT0_STRUC 	 TxStaCnt0;
	TX_STA_CNT1_STRUC	 StaTx1;
	TX_STA_CNT2_STRUC	 StaTx2;
#ifdef STATS_COUNT_SUPPORT
	TX_AGG_CNT_STRUC	TxAggCnt;
	TX_AGG_CNT0_STRUC	TxAggCnt0;
	TX_AGG_CNT1_STRUC	TxAggCnt1;
	TX_AGG_CNT2_STRUC	TxAggCnt2;
	TX_AGG_CNT3_STRUC	TxAggCnt3;
	TX_AGG_CNT4_STRUC	TxAggCnt4;
	TX_AGG_CNT5_STRUC	TxAggCnt5;
	TX_AGG_CNT6_STRUC	TxAggCnt6;
	TX_AGG_CNT7_STRUC	TxAggCnt7;
#endif // STATS_COUNT_SUPPORT //
	COUNTER_RALINK		*pRalinkCounters;


	pRalinkCounters = &pAd->RalinkCounters;

	RTMP_IO_READ32(pAd, RX_STA_CNT0, &RxStaCnt0.word);
	RTMP_IO_READ32(pAd, RX_STA_CNT2, &RxStaCnt2.word);

#ifdef CONFIG_AP_SUPPORT
#ifdef CARRIER_DETECTION_SUPPORT
	if ((pAd->CommonCfg.CarrierDetect.Enable == FALSE) || (pAd->OpMode == OPMODE_STA))
#endif // CARRIER_DETECTION_SUPPORT //
#endif // CONFIG_AP_SUPPORT //
	{
		RTMP_IO_READ32(pAd, RX_STA_CNT1, &RxStaCnt1.word);
	    // Update RX PLCP error counter
	    pAd->PrivateInfo.PhyRxErrCnt += RxStaCnt1.field.PlcpErr;
		// Update False CCA counter
		pAd->RalinkCounters.OneSecFalseCCACnt += RxStaCnt1.field.FalseCca;
	}

#ifdef STATS_COUNT_SUPPORT
	// Update FCS counters
	OldValue= pAd->WlanCounters.FCSErrorCount.u.LowPart;
	pAd->WlanCounters.FCSErrorCount.u.LowPart += (RxStaCnt0.field.CrcErr); // >> 7);
	if (pAd->WlanCounters.FCSErrorCount.u.LowPart < OldValue)
		pAd->WlanCounters.FCSErrorCount.u.HighPart++;
#endif // STATS_COUNT_SUPPORT //

	// Add FCS error count to private counters
	pRalinkCounters->OneSecRxFcsErrCnt += RxStaCnt0.field.CrcErr;
	OldValue = pRalinkCounters->RealFcsErrCount.u.LowPart;
	pRalinkCounters->RealFcsErrCount.u.LowPart += RxStaCnt0.field.CrcErr;
	if (pRalinkCounters->RealFcsErrCount.u.LowPart < OldValue)
		pRalinkCounters->RealFcsErrCount.u.HighPart++;

	// Update Duplicate Rcv check
	pRalinkCounters->DuplicateRcv += RxStaCnt2.field.RxDupliCount;
#ifdef STATS_COUNT_SUPPORT
	pAd->WlanCounters.FrameDuplicateCount.u.LowPart += RxStaCnt2.field.RxDupliCount;
#endif // STATS_COUNT_SUPPORT //
	// Update RX Overflow counter
	pAd->Counters8023.RxNoBuffer += (RxStaCnt2.field.RxFifoOverflowCount);
	
	//pAd->RalinkCounters.RxCount = 0;

	
	//if (!OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_TX_RATE_SWITCH_ENABLED) || 
	//	(OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_TX_RATE_SWITCH_ENABLED) && (pAd->MacTab.Size != 1)))
	if (!pAd->bUpdateBcnCntDone)
	{
	// Update BEACON sent count
	RTMP_IO_READ32(pAd, TX_STA_CNT0, &TxStaCnt0.word);
	RTMP_IO_READ32(pAd, TX_STA_CNT1, &StaTx1.word);
	RTMP_IO_READ32(pAd, TX_STA_CNT2, &StaTx2.word);
	pRalinkCounters->OneSecBeaconSentCnt += TxStaCnt0.field.TxBeaconCount;
	pRalinkCounters->OneSecTxRetryOkCount += StaTx1.field.TxRetransmit;
	pRalinkCounters->OneSecTxNoRetryOkCount += StaTx1.field.TxSuccess;
	pRalinkCounters->OneSecTxFailCount += TxStaCnt0.field.TxFailCount;

#ifdef STATS_COUNT_SUPPORT
	pAd->WlanCounters.TransmittedFragmentCount.u.LowPart += StaTx1.field.TxSuccess;
	pAd->WlanCounters.RetryCount.u.LowPart += StaTx1.field.TxRetransmit;
	pAd->WlanCounters.FailedCount.u.LowPart += TxStaCnt0.field.TxFailCount;
#endif // STATS_COUNT_SUPPORT //
	}


	//if (pAd->bStaFifoTest == TRUE)
#ifdef STATS_COUNT_SUPPORT
	{
		RTMP_IO_READ32(pAd, TX_AGG_CNT, &TxAggCnt.word);
	RTMP_IO_READ32(pAd, TX_AGG_CNT0, &TxAggCnt0.word);
	RTMP_IO_READ32(pAd, TX_AGG_CNT1, &TxAggCnt1.word);
	RTMP_IO_READ32(pAd, TX_AGG_CNT2, &TxAggCnt2.word);
	RTMP_IO_READ32(pAd, TX_AGG_CNT3, &TxAggCnt3.word);
		RTMP_IO_READ32(pAd, TX_AGG_CNT4, &TxAggCnt4.word);
		RTMP_IO_READ32(pAd, TX_AGG_CNT5, &TxAggCnt5.word);
		RTMP_IO_READ32(pAd, TX_AGG_CNT6, &TxAggCnt6.word);
		RTMP_IO_READ32(pAd, TX_AGG_CNT7, &TxAggCnt7.word);
		pRalinkCounters->TxAggCount += TxAggCnt.field.AggTxCount;
		pRalinkCounters->TxNonAggCount += TxAggCnt.field.NonAggTxCount;
		pRalinkCounters->TxAgg1MPDUCount += TxAggCnt0.field.AggSize1Count;
		pRalinkCounters->TxAgg2MPDUCount += TxAggCnt0.field.AggSize2Count;
		
		pRalinkCounters->TxAgg3MPDUCount += TxAggCnt1.field.AggSize3Count;
		pRalinkCounters->TxAgg4MPDUCount += TxAggCnt1.field.AggSize4Count;
		pRalinkCounters->TxAgg5MPDUCount += TxAggCnt2.field.AggSize5Count;
		pRalinkCounters->TxAgg6MPDUCount += TxAggCnt2.field.AggSize6Count;
	
		pRalinkCounters->TxAgg7MPDUCount += TxAggCnt3.field.AggSize7Count;
		pRalinkCounters->TxAgg8MPDUCount += TxAggCnt3.field.AggSize8Count;
		pRalinkCounters->TxAgg9MPDUCount += TxAggCnt4.field.AggSize9Count;
		pRalinkCounters->TxAgg10MPDUCount += TxAggCnt4.field.AggSize10Count;

		pRalinkCounters->TxAgg11MPDUCount += TxAggCnt5.field.AggSize11Count;
		pRalinkCounters->TxAgg12MPDUCount += TxAggCnt5.field.AggSize12Count;
		pRalinkCounters->TxAgg13MPDUCount += TxAggCnt6.field.AggSize13Count;
		pRalinkCounters->TxAgg14MPDUCount += TxAggCnt6.field.AggSize14Count;

		pRalinkCounters->TxAgg15MPDUCount += TxAggCnt7.field.AggSize15Count;
		pRalinkCounters->TxAgg16MPDUCount += TxAggCnt7.field.AggSize16Count;

		// Calculate the transmitted A-MPDU count
		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += TxAggCnt0.field.AggSize1Count;
		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt0.field.AggSize2Count >> 1);

		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt1.field.AggSize3Count / 3);
		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt1.field.AggSize4Count >> 2);

		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt2.field.AggSize5Count / 5);
		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt2.field.AggSize6Count / 6);

		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt3.field.AggSize7Count / 7);
		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt3.field.AggSize8Count >> 3);

		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt4.field.AggSize9Count / 9);
		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt4.field.AggSize10Count / 10);

		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt5.field.AggSize11Count / 11);
		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt5.field.AggSize12Count / 12);

		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt6.field.AggSize13Count / 13);
		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt6.field.AggSize14Count / 14);

		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt7.field.AggSize15Count / 15);
		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt7.field.AggSize16Count >> 4);	
	}
#endif // STATS_COUNT_SUPPORT //			

#ifdef DBG_DIAGNOSE
	{
		RtmpDiagStruct	*pDiag;
		UCHAR			ArrayCurIdx, i;
		
		pDiag = &pAd->DiagStruct;
		ArrayCurIdx = pDiag->ArrayCurIdx;
		
		if (pDiag->inited == 0)
		{
			NdisZeroMemory(pDiag, sizeof(struct _RtmpDiagStrcut_));
			pDiag->ArrayStartIdx = pDiag->ArrayCurIdx = 0;
			pDiag->inited = 1;
		}
		else
		{
			// Tx
			pDiag->TxFailCnt[ArrayCurIdx] = TxStaCnt0.field.TxFailCount;
			pDiag->TxAggCnt[ArrayCurIdx] = TxAggCnt.field.AggTxCount;
			pDiag->TxNonAggCnt[ArrayCurIdx] = TxAggCnt.field.NonAggTxCount;
			pDiag->TxAMPDUCnt[ArrayCurIdx][0] = TxAggCnt0.field.AggSize1Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][1] = TxAggCnt0.field.AggSize2Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][2] = TxAggCnt1.field.AggSize3Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][3] = TxAggCnt1.field.AggSize4Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][4] = TxAggCnt2.field.AggSize5Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][5] = TxAggCnt2.field.AggSize6Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][6] = TxAggCnt3.field.AggSize7Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][7] = TxAggCnt3.field.AggSize8Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][8] = TxAggCnt4.field.AggSize9Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][9] = TxAggCnt4.field.AggSize10Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][10] = TxAggCnt5.field.AggSize11Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][11] = TxAggCnt5.field.AggSize12Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][12] = TxAggCnt6.field.AggSize13Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][13] = TxAggCnt6.field.AggSize14Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][14] = TxAggCnt7.field.AggSize15Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][15] = TxAggCnt7.field.AggSize16Count;

			pDiag->RxCrcErrCnt[ArrayCurIdx] = RxStaCnt0.field.CrcErr;
			
			INC_RING_INDEX(pDiag->ArrayCurIdx,  DIAGNOSE_TIME);
			ArrayCurIdx = pDiag->ArrayCurIdx;
			for (i =0; i < 9; i++)
			{
				pDiag->TxDescCnt[ArrayCurIdx][i]= 0;
				pDiag->TxSWQueCnt[ArrayCurIdx][i] =0;
				pDiag->TxMcsCnt[ArrayCurIdx][i] = 0;
				pDiag->RxMcsCnt[ArrayCurIdx][i] = 0;
			}
			pDiag->TxDataCnt[ArrayCurIdx] = 0;
			pDiag->TxFailCnt[ArrayCurIdx] = 0;
			pDiag->RxDataCnt[ArrayCurIdx] = 0;
			pDiag->RxCrcErrCnt[ArrayCurIdx]  = 0;
//			for (i = 9; i < 16; i++)
			for (i = 9; i < 24; i++) // 3*3
			{
				pDiag->TxDescCnt[ArrayCurIdx][i] = 0;
				pDiag->TxMcsCnt[ArrayCurIdx][i] = 0;
				pDiag->RxMcsCnt[ArrayCurIdx][i] = 0;
}

			if (pDiag->ArrayCurIdx == pDiag->ArrayStartIdx)
				INC_RING_INDEX(pDiag->ArrayStartIdx,  DIAGNOSE_TIME);
		}
		
	}
#endif // DBG_DIAGNOSE //


}


NDIS_STATUS NICLoadFirmware(
	IN PRTMP_ADAPTER pAd)
{
	NDIS_STATUS	 status = NDIS_STATUS_SUCCESS;
	if (pAd->chipOps.loadFirmware)
		status = pAd->chipOps.loadFirmware(pAd);

	return status;
}


/*
	========================================================================
	
	Routine Description:
		erase 8051 firmware image in MAC ASIC

	Arguments:
		Adapter						Pointer to our adapter

	IRQL = PASSIVE_LEVEL
		
	========================================================================
*/
VOID NICEraseFirmware(
	IN PRTMP_ADAPTER pAd)
{
	if (pAd->chipOps.eraseFirmware)
		pAd->chipOps.eraseFirmware(pAd);
	
}/* End of NICEraseFirmware */


/*
	========================================================================
	
	Routine Description:
		Load Tx rate switching parameters

	Arguments:
		Adapter						Pointer to our adapter

	Return Value:
		NDIS_STATUS_SUCCESS         firmware image load ok
		NDIS_STATUS_FAILURE         image not found

	IRQL = PASSIVE_LEVEL

	Rate Table Format:
		1. (B0: Valid Item number) (B1:Initial item from zero)
		2. Item Number(Dec)      Mode(Hex)     Current MCS(Dec)    TrainUp(Dec)    TrainDown(Dec)
		
	========================================================================
*/
NDIS_STATUS NICLoadRateSwitchingParams(
	IN PRTMP_ADAPTER pAd)
{
	return NDIS_STATUS_SUCCESS;
}


/*
	========================================================================
	
	Routine Description:
		Compare two memory block

	Arguments:
		pSrc1		Pointer to first memory address
		pSrc2		Pointer to second memory address
		
	Return Value:
		0:			memory is equal
		1:			pSrc1 memory is larger
		2:			pSrc2 memory is larger

	IRQL = DISPATCH_LEVEL
	
	Note:
		
	========================================================================
*/
ULONG	RTMPCompareMemory(
	IN	PVOID	pSrc1,
	IN	PVOID	pSrc2,
	IN	ULONG	Length)
{
	PUCHAR	pMem1;
	PUCHAR	pMem2;
	ULONG	Index = 0;

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
		
	IRQL = PASSIVE_LEVEL
	IRQL = DISPATCH_LEVEL
	
	Note:
		
	========================================================================
*/
VOID	RTMPZeroMemory(
	IN	PVOID	pSrc,
	IN	ULONG	Length)
{
	PUCHAR	pMem;
	ULONG	Index = 0;

	pMem = (PUCHAR) pSrc;

	for (Index = 0; Index < Length; Index++)
	{
		pMem[Index] = 0x00;
	}
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

	IRQL = PASSIVE_LEVEL
	IRQL = DISPATCH_LEVEL
	
	Note:
		
	========================================================================
*/
VOID RTMPMoveMemory(
	OUT	PVOID	pDest,
	IN	PVOID	pSrc,
	IN	ULONG	Length)
{
	PUCHAR	pMem1;
	PUCHAR	pMem2;
	UINT	Index;

	ASSERT((Length==0) || (pDest && pSrc));

	pMem1 = (PUCHAR) pDest;
	pMem2 = (PUCHAR) pSrc;

	for (Index = 0; Index < Length; Index++)
	{
		pMem1[Index] = pMem2[Index];
	}
}


/*
	========================================================================
	
	Routine Description:
		Initialize port configuration structure

	Arguments:
		Adapter						Pointer to our adapter

	Return Value:
		None

	IRQL = PASSIVE_LEVEL

	Note:
		
	========================================================================
*/
VOID	UserCfgInit(
	IN	PRTMP_ADAPTER pAd)
{
#ifdef CONFIG_AP_SUPPORT
	UINT i, j;
#endif // CONFIG_AP_SUPPORT //
//	EDCA_PARM DefaultEdcaParm;
    UINT key_index, bss_index;

	DBGPRINT(RT_DEBUG_TRACE, ("--> UserCfgInit\n"));    
	 
	//
	//  part I. intialize common configuration
	//

	for(key_index=0; key_index<SHARE_KEY_NUM; key_index++)
	{
		for(bss_index = 0; bss_index < MAX_MBSSID_NUM; bss_index++)
		{
			pAd->SharedKey[bss_index][key_index].KeyLen = 0;
			pAd->SharedKey[bss_index][key_index].CipherAlg = CIPHER_NONE;
		}
	}

	pAd->bLocalAdminMAC = FALSE;
	pAd->EepromAccess = FALSE;
	
	pAd->Antenna.word = 0; 
	pAd->CommonCfg.BBPCurrentBW = BW_20;


#ifdef A_BAND_SUPPORT
	pAd->bAutoTxAgcA = FALSE;			// Default is OFF
#endif // A_BAND_SUPPORT //
	pAd->bAutoTxAgcG = FALSE;			// Default is OFF
#ifdef RTMP_INTERNAL_TX_ALC
	pAd->TxPowerCtrl.bInternalTxALC = FALSE; // Off by default
#endif // RTMP_INTERNAL_TX_ALC //
	pAd->RfIcType = RFIC_2820;

	// Init timer for reset complete event
	pAd->CommonCfg.CentralChannel = 1;
	pAd->bForcePrintTX = FALSE;
	pAd->bForcePrintRX = FALSE;
	pAd->bStaFifoTest = FALSE;
	pAd->bProtectionTest = FALSE;
	pAd->bHCCATest = FALSE;
	pAd->bGenOneHCCA = FALSE;
	pAd->CommonCfg.Dsifs = 10;      // in units of usec 
	pAd->CommonCfg.TxPower = 100; //mW
	pAd->CommonCfg.TxPowerPercentage = 0xffffffff; // AUTO
	pAd->CommonCfg.TxPowerDefault = 0xffffffff; // AUTO
	pAd->CommonCfg.TxPreamble = Rt802_11PreambleAuto; // use Long preamble on TX by defaut
	pAd->CommonCfg.bUseZeroToDisableFragment = FALSE;
	pAd->CommonCfg.RtsThreshold = 2347;
	pAd->CommonCfg.FragmentThreshold = 2346;
	pAd->CommonCfg.UseBGProtection = 0;    // 0: AUTO
	pAd->CommonCfg.bEnableTxBurst = TRUE; //0;    	
	pAd->CommonCfg.PhyMode = 0xff;     // unknown
	pAd->CommonCfg.BandState = UNKNOWN_BAND;
	pAd->CommonCfg.RadarDetect.CSPeriod = 10;
	pAd->CommonCfg.RadarDetect.CSCount = 0;
	pAd->CommonCfg.RadarDetect.RDMode = RD_NORMAL_MODE;
	
#ifdef CONFIG_AP_SUPPORT


#ifdef DFS_DEBUG
	pAd->CommonCfg.McuRadarDebug = 0;
	pAd->CommonCfg.ChRemain = 0;
#endif // DFS_DEBUG //
#endif // CONFIG_AP_SUPPORT //

#ifdef TONE_RADAR_DETECT_SUPPORT
#ifdef CARRIER_DETECTION_SUPPORT
	pAd->CommonCfg.CarrierDetect.delta = CARRIER_DETECT_DELTA;
	pAd->CommonCfg.CarrierDetect.div_flag = CARRIER_DETECT_DIV_FLAG;
	pAd->CommonCfg.CarrierDetect.criteria = CARRIER_DETECT_CRITIRIA;
	pAd->CommonCfg.CarrierDetect.threshold = CARRIER_DETECT_THRESHOLD;
#endif // CARRIER_DETECTION_SUPPORT //
#endif // TONE_RADAR_DETECT_SUPPORT //

	pAd->CommonCfg.RadarDetect.ChMovingTime = 65;
#ifdef MERGE_ARCH_TEAM
	pAd->CommonCfg.RadarDetect.LongPulseRadarTh = 2;
	pAd->CommonCfg.RadarDetect.AvgRssiReq = -75;
#else // original rt28xx source code
	pAd->CommonCfg.RadarDetect.LongPulseRadarTh = 3;
#endif // MERGE_ARCH_TEAM //
	pAd->CommonCfg.bAPSDCapable = FALSE;
	pAd->CommonCfg.bNeedSendTriggerFrame = FALSE;
	pAd->CommonCfg.TriggerTimerCount = 0;
	pAd->CommonCfg.bAPSDForcePowerSave = FALSE;
	pAd->CommonCfg.bCountryFlag = FALSE;
	pAd->CommonCfg.TxStream = 0;
	pAd->CommonCfg.RxStream = 0;

	NdisZeroMemory(&pAd->BeaconTxWI, sizeof(pAd->BeaconTxWI));

#ifdef DOT11_N_SUPPORT
	NdisZeroMemory(&pAd->CommonCfg.HtCapability, sizeof(pAd->CommonCfg.HtCapability));
	pAd->HTCEnable = FALSE;
	pAd->bBroadComHT = FALSE;
	pAd->CommonCfg.bRdg = FALSE;
	
#ifdef DOT11N_DRAFT3
	pAd->CommonCfg.Dot11OBssScanPassiveDwell = dot11OBSSScanPassiveDwell;	// Unit : TU. 5~1000
	pAd->CommonCfg.Dot11OBssScanActiveDwell = dot11OBSSScanActiveDwell;	// Unit : TU. 10~1000
	pAd->CommonCfg.Dot11BssWidthTriggerScanInt = dot11BSSWidthTriggerScanInterval;	// Unit : Second	
	pAd->CommonCfg.Dot11OBssScanPassiveTotalPerChannel = dot11OBSSScanPassiveTotalPerChannel;	// Unit : TU. 200~10000
	pAd->CommonCfg.Dot11OBssScanActiveTotalPerChannel = dot11OBSSScanActiveTotalPerChannel;	// Unit : TU. 20~10000
	pAd->CommonCfg.Dot11BssWidthChanTranDelayFactor = dot11BSSWidthChannelTransactionDelayFactor;
	pAd->CommonCfg.Dot11OBssScanActivityThre = dot11BSSScanActivityThreshold;	// Unit : percentage
	pAd->CommonCfg.Dot11BssWidthChanTranDelay = (pAd->CommonCfg.Dot11BssWidthTriggerScanInt * pAd->CommonCfg.Dot11BssWidthChanTranDelayFactor);

	//pAd->CommonCfg.bBssCoexEnable = TRUE; // by default, we enable this feature, you can disable it via the profile or ioctl command
	pAd->CommonCfg.bBssCoexEnable = FALSE; //Modified by fredli 11-12-1. 3390(2.3.2.17) driver fail to connect 11n client with COEX enable, and hence should avoid bandwith adjust. The value will be forced set with FALSE when initialize in cmm_profile.c as well.
#endif  // DOT11N_DRAFT3 //

	NdisZeroMemory(&pAd->CommonCfg.AddHTInfo, sizeof(pAd->CommonCfg.AddHTInfo));
	pAd->CommonCfg.BACapability.field.MMPSmode = MMPS_ENABLE;
	pAd->CommonCfg.BACapability.field.MpduDensity = 0;
	pAd->CommonCfg.BACapability.field.Policy = IMMED_BA;
	pAd->CommonCfg.BACapability.field.RxBAWinLimit = 64; //32;
	pAd->CommonCfg.BACapability.field.TxBAWinLimit = 64; //32;
	DBGPRINT(RT_DEBUG_TRACE, ("--> UserCfgInit. BACapability = 0x%x\n", pAd->CommonCfg.BACapability.word));    

	pAd->CommonCfg.BACapability.field.AutoBA = FALSE;	
	BATableInit(pAd, &pAd->BATable);

	pAd->CommonCfg.bExtChannelSwitchAnnouncement = 1;
	pAd->CommonCfg.bHTProtect = 1;
	pAd->CommonCfg.bMIMOPSEnable = TRUE;
	//2008/11/05:KH add to support Antenna power-saving of AP<--
	pAd->CommonCfg.bGreenAPEnable=FALSE;
	pAd->CommonCfg.bBlockAntDivforGreenAP = FALSE;
	//2008/11/05:KH add to support Antenna power-saving of AP-->
	pAd->CommonCfg.bBADecline = FALSE;
	pAd->CommonCfg.bDisableReordering = FALSE;

	if (pAd->MACVersion == 0x28720200)
	{
		pAd->CommonCfg.TxBASize = 13; //by Jerry recommend
	}else{
		pAd->CommonCfg.TxBASize = 7;
	}

	pAd->CommonCfg.REGBACapability.word = pAd->CommonCfg.BACapability.word;
#endif // DOT11_N_SUPPORT //

	//pAd->CommonCfg.HTPhyMode.field.BW = BW_20;
	//pAd->CommonCfg.HTPhyMode.field.MCS = MCS_AUTO;
	//pAd->CommonCfg.HTPhyMode.field.ShortGI = GI_800;
	//pAd->CommonCfg.HTPhyMode.field.STBC = STBC_NONE;
	pAd->CommonCfg.TxRate = RATE_6;
	
	pAd->CommonCfg.MlmeTransmit.field.MCS = MCS_RATE_6;
	pAd->CommonCfg.MlmeTransmit.field.BW = BW_20;
	pAd->CommonCfg.MlmeTransmit.field.MODE = MODE_OFDM;

	pAd->CommonCfg.BeaconPeriod = 100;     // in mSec

#ifdef WAPI_SUPPORT
	pAd->CommonCfg.wapi_usk_rekey_method = REKEY_METHOD_DISABLE;
	pAd->CommonCfg.wapi_msk_rekey_method = REKEY_METHOD_DISABLE;
	pAd->CommonCfg.wapi_msk_rekey_cnt = 0;
#endif // WAPI_SUPPORT //

	//
	// part II. intialize STA specific configuration
	//

	// global variables mXXXX used in MAC protocol state machines
	OPSTATUS_SET_FLAG(pAd, fOP_STATUS_RECEIVE_DTIM);
	OPSTATUS_CLEAR_FLAG(pAd, fOP_STATUS_ADHOC_ON);
	OPSTATUS_CLEAR_FLAG(pAd, fOP_STATUS_INFRA_ON);

	// PHY specification
	pAd->CommonCfg.PhyMode = PHY_11BG_MIXED;		// default PHY mode
	OPSTATUS_CLEAR_FLAG(pAd, fOP_STATUS_SHORT_PREAMBLE_INUSED);  // CCK use LONG preamble


	// Default for extra information is not valid
	pAd->ExtraInfo = EXTRA_INFO_CLEAR;
	
	// Default Config change flag
	pAd->bConfigChanged = FALSE;

	// 
	// part III. AP configurations
	//
#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
	{
		// Set MBSS Default Configurations
		pAd->ApCfg.BssidNum = MAX_MBSSID_NUM;
		for(j = BSS0; j < pAd->ApCfg.BssidNum; j++)
		{
			pAd->ApCfg.MBSSID[j].AuthMode = Ndis802_11AuthModeOpen;
			pAd->ApCfg.MBSSID[j].WepStatus = Ndis802_11EncryptionDisabled;
			pAd->ApCfg.MBSSID[j].GroupKeyWepStatus = Ndis802_11EncryptionDisabled;
			pAd->ApCfg.MBSSID[j].DefaultKeyId = 0;
			pAd->ApCfg.MBSSID[j].WpaMixPairCipher = MIX_CIPHER_NOTUSE;
                        pAd->ApCfg.MBSSID[j].RekeyCountDown = 0; /* it's used for WPA rekey */                        

			pAd->ApCfg.MBSSID[j].IEEE8021X = FALSE;
			pAd->ApCfg.MBSSID[j].PreAuth = FALSE;

			// PMK cache setting
			pAd->ApCfg.MBSSID[j].PMKCachePeriod = (10 * 60 * OS_HZ); // unit : tick(default: 10 minute)
			NdisZeroMemory(&pAd->ApCfg.MBSSID[j].PMKIDCache, sizeof(NDIS_AP_802_11_PMKID));
#ifdef DOT1X_SUPPORT
			/* dot1x related per BSS */
			pAd->ApCfg.MBSSID[j].radius_srv_num = 0;
			pAd->ApCfg.MBSSID[j].NasIdLen = 0;
#endif // DOT1X_SUPPORT //			

			/* VLAN related */
        		pAd->ApCfg.MBSSID[j].VLAN_VID = 0;

			// Default MCS as AUTO
			pAd->ApCfg.MBSSID[j].bAutoTxRateSwitch = TRUE;
			pAd->ApCfg.MBSSID[j].DesiredTransmitSetting.field.MCS = MCS_AUTO;

			// Default is zero. It means no limit.
			pAd->ApCfg.MBSSID[j].MaxStaNum = 0;
			pAd->ApCfg.MBSSID[j].StaCount = 0;
			
#ifdef WSC_AP_SUPPORT
			{
				PWSC_CTRL pWscControl;
				INT idx;
				/*
					WscControl cannot be zero here, because WscControl timers are initial in MLME Initialize 
					and MLME Initialize is called before UserCfgInit.

				*/
				pWscControl = &pAd->ApCfg.MBSSID[j].WscControl;
				NdisZeroMemory(&pWscControl->RegData, sizeof(WSC_REG_DATA));
				NdisZeroMemory(&pWscControl->WscStaPbcProbeInfo, sizeof(WSC_STA_PBC_PROBE_INFO));
				pWscControl->WscMode = 1;
				pWscControl->WscConfStatus = 1;
				pWscControl->WscConfigMethods= 0x0084;
				pWscControl->RegData.ReComputePke = 1;
				pWscControl->lastId = 1;
				pWscControl->EntryIfIdx = (MIN_NET_DEVICE_FOR_MBSSID | j);
				pWscControl->pAd = pAd;
				pWscControl->WscRejectSamePinFromEnrollee = FALSE;
				pWscControl->WscPBCOverlap= FALSE;
				pWscControl->WscConfMode = 0;
				pWscControl->WscStatus = 0;
				pWscControl->WscState = 0;
				pWscControl->WscPinCode = 0;
				pWscControl->WscLastPinFromEnrollee = 0;
				pWscControl->WscEnrolleePinCode = 0;
				pWscControl->WscSelReg = 0;
				pWscControl->WscUseUPnP = 0;
				pWscControl->bWCNTest = FALSE;
				pWscControl->WscKeyASCII = 0; /* default, 0 (64 Hex) */
				
				/*
					Enrollee 192 random bytes for DH key generation
				*/
				for (idx = 0; idx < 192; idx++)
					pWscControl->RegData.EnrolleeRandom[idx] = RandomByte(pAd);


				// xyyou_0501 Enrollee Nonce, first generate and save to Wsc Control Block
				for (idx = 0; idx < 16; idx++)
				{
					pWscControl->RegData.SelfNonce[idx] = RandomByte(pAd);
				}
				NdisZeroMemory(&pWscControl->WscDefaultSsid, sizeof(NDIS_802_11_SSID));
			}
#endif // WSC_AP_SUPPORT //        

			for(i = 0; i < WLAN_MAX_NUM_OF_TIM; i++)
	        		pAd->ApCfg.MBSSID[j].TimBitmaps[i] = 0;
		}
		pAd->ApCfg.DtimCount  = 0;
		pAd->ApCfg.DtimPeriod = DEFAULT_DTIM_PERIOD;

		pAd->ApCfg.ErpIeContent = 0;

		pAd->ApCfg.StaIdleTimeout = MAC_TABLE_AGEOUT_TIME;

#ifdef IDS_SUPPORT
		// Default disable IDS threshold and reset all IDS counters
		pAd->ApCfg.IdsEnable = FALSE;
		pAd->ApCfg.AuthFloodThreshold = 0;
		pAd->ApCfg.AssocReqFloodThreshold = 0;
		pAd->ApCfg.ReassocReqFloodThreshold = 0;
		pAd->ApCfg.ProbeReqFloodThreshold = 0;
		pAd->ApCfg.DisassocFloodThreshold = 0;
		pAd->ApCfg.DeauthFloodThreshold = 0;
		pAd->ApCfg.EapReqFooldThreshold = 0;
		RTMPClearAllIdsCounter(pAd);
#endif // IDS_SUPPORT //

#ifdef WDS_SUPPORT
		APWdsInitialize(pAd);
#endif // WDS_SUPPORT

#ifdef WSC_AP_SUPPORT
		pAd->WriteWscCfgToDatFile = 0xFF;
		pAd->WriteWscCfgToAr9DatFile = FALSE;
#endif // WSC_AP_SUPPORT //

#ifdef APCLI_SUPPORT
		for(j = 0; j < MAX_APCLI_NUM; j++) 
		{
			pAd->ApCfg.ApCliTab[j].bAutoTxRateSwitch = TRUE;
			pAd->ApCfg.ApCliTab[j].DesiredTransmitSetting.field.MCS = MCS_AUTO;
		}
#endif // APCLI_SUPPORT //
	}
#endif // CONFIG_AP_SUPPORT //


	//
	// part IV. others
	//
	// dynamic BBP R66:sensibity tuning to overcome background noise
	pAd->BbpTuning.bEnable                = TRUE;  
	pAd->BbpTuning.FalseCcaLowerThreshold = 100;
	pAd->BbpTuning.FalseCcaUpperThreshold = 512;
	pAd->BbpTuning.R66Delta               = 4;
	pAd->Mlme.bEnableAutoAntennaCheck = TRUE;
	
	//
	// Also initial R66CurrentValue, RTUSBResumeMsduTransmission might use this value.
	// if not initial this value, the default value will be 0.
	//
	pAd->BbpTuning.R66CurrentValue = 0x38;

	pAd->Bbp94 = BBPR94_DEFAULT;
	pAd->BbpForCCK = FALSE;
	
	// Default is FALSE for test bit 1
	//pAd->bTest1 = FALSE;
	
	// initialize MAC table and allocate spin lock
	NdisZeroMemory(&pAd->MacTab, sizeof(MAC_TABLE));
	InitializeQueueHeader(&pAd->MacTab.McastPsQueue);
	NdisAllocateSpinLock(&pAd->MacTabLock);

	//RTMPInitTimer(pAd, &pAd->RECBATimer, RECBATimerTimeout, pAd, TRUE);
	//RTMPSetTimer(&pAd->RECBATimer, REORDER_EXEC_INTV);

#ifdef RALINK_ATE
	NdisZeroMemory(&pAd->ate, sizeof(ATE_INFO));
	pAd->ate.Mode = ATE_STOP;
	pAd->ate.TxCount = 200;/* to exceed TX_RING_SIZE ... */
	pAd->ate.TxDoneCount = 0;
	pAd->ate.RFFreqOffset = 0;
	pAd->ate.TxLength = 1024;
	pAd->ate.TxWI.ShortGI = 0;// LONG GI : 800 ns
	pAd->ate.TxWI.PHYMODE = MODE_CCK;
	pAd->ate.TxWI.MCS = 3;
	pAd->ate.TxWI.BW = BW_20;
	pAd->ate.Channel = 1;
	pAd->ate.QID = QID_AC_BE;
	pAd->ate.Addr1[0] = 0x00;
	pAd->ate.Addr1[1] = 0x11;
	pAd->ate.Addr1[2] = 0x22;
	pAd->ate.Addr1[3] = 0xAA;
	pAd->ate.Addr1[4] = 0xBB;
	pAd->ate.Addr1[5] = 0xCC;
	NdisMoveMemory(pAd->ate.Addr2, pAd->ate.Addr1, ETH_LENGTH_OF_ADDRESS);
	NdisMoveMemory(pAd->ate.Addr3, pAd->ate.Addr1, ETH_LENGTH_OF_ADDRESS);
	pAd->ate.bRxFER = 0;
	pAd->ate.bQATxStart = FALSE;
	pAd->ate.bQARxStart = FALSE;

#ifdef RTMP_MAC_PCI 
	pAd->ate.bFWLoading = FALSE;
#endif // RTMP_MAC_PCI //


#ifdef RALINK_28xx_QA
	pAd->ate.TxStatus = 0;
	pAd->ate.AtePid = THREAD_PID_INIT_VALUE;
#endif // RALINK_28xx_QA //
#endif // RALINK_ATE //


	pAd->CommonCfg.bWiFiTest = FALSE;
#ifdef RTMP_MAC_PCI
    pAd->bPCIclkOff = FALSE;
#endif // RTMP_MAC_PCI //

#ifdef CONFIG_AP_SUPPORT
	pAd->ApCfg.EntryLifeCheck = MAC_ENTRY_LIFE_CHECK_CNT;

#endif // CONFIG_AP_SUPPORT //

	DBGPRINT(RT_DEBUG_TRACE, ("<-- UserCfgInit\n"));
}

// IRQL = PASSIVE_LEVEL
UCHAR BtoH(STRING ch)
{
	if (ch >= '0' && ch <= '9') return (ch - '0');        // Handle numerals
	if (ch >= 'A' && ch <= 'F') return (ch - 'A' + 0xA);  // Handle capitol hex digits
	if (ch >= 'a' && ch <= 'f') return (ch - 'a' + 0xA);  // Handle small hex digits
	return(255);
}

//
//  FUNCTION: AtoH(char *, UCHAR *, int)
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
// IRQL = PASSIVE_LEVEL

void AtoH(PSTRING src, PUCHAR dest, int destlen)
{
	PSTRING srcptr;
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


//+++Mark by shiang, not use now, need to remove after confirm
//---Mark by shiang, not use now, need to remove after confirm
#ifdef TC_SOC
VOID tcInitTimer(
	IN PRTMP_ADAPTER pAd,
	IN struct timer_list *timer,
	IN PVOID timerFunc)
{
	timer->data = (unsigned long)pAd;
	timer->function = timerFunc;
	init_timer(timer);
	return;
}
#endif
/*
	========================================================================
	
	Routine Description:
		Init timer objects

	Arguments:
		pAd			Pointer to our adapter
		pTimer				Timer structure
		pTimerFunc			Function to execute when timer expired
		Repeat				Ture for period timer

	Return Value:
		None

	Note:
		
	========================================================================
*/
VOID	RTMPInitTimer(
	IN	PRTMP_ADAPTER			pAd,
	IN	PRALINK_TIMER_STRUCT	pTimer,
	IN	PVOID					pTimerFunc,
	IN	PVOID					pData,
	IN	BOOLEAN					Repeat)
{
	//
	// Set Valid to TRUE for later used.
	// It will crash if we cancel a timer or set a timer 
	// that we haven't initialize before.
	// 
	pTimer->Valid      = TRUE;
	
	pTimer->PeriodicType = Repeat;
	pTimer->State      = FALSE;
	pTimer->cookie = (ULONG) pData;
	pTimer->pAd = pAd;

	RTMP_OS_Init_Timer(pAd, &pTimer->TimerObj,	pTimerFunc, (PVOID) pTimer);	
}


/*
	========================================================================
	
	Routine Description:
		Init timer objects

	Arguments:
		pTimer				Timer structure
		Value				Timer value in milliseconds

	Return Value:
		None

	Note:
		To use this routine, must call RTMPInitTimer before.
		
	========================================================================
*/
VOID	RTMPSetTimer(
	IN	PRALINK_TIMER_STRUCT	pTimer,
	IN	ULONG					Value)
{
	if (pTimer->Valid)
	{
		RTMP_ADAPTER *pAd;
		
		pAd = (RTMP_ADAPTER *)pTimer->pAd;
		if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS))
		{
			DBGPRINT_ERR(("RTMPSetTimer failed, Halt in Progress!\n"));
			return;
		}
		
		pTimer->TimerValue = Value;
		pTimer->State      = FALSE;
		if (pTimer->PeriodicType == TRUE)
		{
			pTimer->Repeat = TRUE;
			RTMP_SetPeriodicTimer(&pTimer->TimerObj, Value);
		}
		else
		{
			pTimer->Repeat = FALSE;
			RTMP_OS_Add_Timer(&pTimer->TimerObj, Value);
		}
	}
	else
	{
		DBGPRINT_ERR(("RTMPSetTimer failed, Timer hasn't been initialize!\n"));
	}
}


/*
	========================================================================
	
	Routine Description:
		Init timer objects

	Arguments:
		pTimer				Timer structure
		Value				Timer value in milliseconds

	Return Value:
		None

	Note:
		To use this routine, must call RTMPInitTimer before.
		
	========================================================================
*/
VOID	RTMPModTimer(
	IN	PRALINK_TIMER_STRUCT	pTimer,
	IN	ULONG					Value)
{
	BOOLEAN	Cancel;

	if (pTimer->Valid)
	{
		pTimer->TimerValue = Value;
		pTimer->State      = FALSE;
		if (pTimer->PeriodicType == TRUE)
		{
			RTMPCancelTimer(pTimer, &Cancel);
			RTMPSetTimer(pTimer, Value);
		}
		else
		{
			RTMP_OS_Mod_Timer(&pTimer->TimerObj, Value);
		}
	}
	else
	{
		DBGPRINT_ERR(("RTMPModTimer failed, Timer hasn't been initialize!\n"));
	}
}


/*
	========================================================================
	
	Routine Description:
		Cancel timer objects

	Arguments:
		Adapter						Pointer to our adapter

	Return Value:
		None

	IRQL = PASSIVE_LEVEL
	IRQL = DISPATCH_LEVEL
	
	Note:
		1.) To use this routine, must call RTMPInitTimer before.
		2.) Reset NIC to initial state AS IS system boot up time.
		
	========================================================================
*/
VOID	RTMPCancelTimer(
	IN	PRALINK_TIMER_STRUCT	pTimer,
	OUT	BOOLEAN					*pCancelled)
{
	if (pTimer->Valid)
	{
		if (pTimer->State == FALSE)
			pTimer->Repeat = FALSE;
		
		RTMP_OS_Del_Timer(&pTimer->TimerObj, pCancelled);
		
		if (*pCancelled == TRUE)
			pTimer->State = TRUE;

#ifdef RTMP_TIMER_TASK_SUPPORT
		// We need to go-through the TimerQ to findout this timer handler and remove it if 
		//		it's still waiting for execution.
		RtmpTimerQRemove(pTimer->pAd, pTimer);
#endif // RTMP_TIMER_TASK_SUPPORT //
	}
	else
	{
		DBGPRINT_ERR(("RTMPCancelTimer failed, Timer hasn't been initialize!\n"));
	}
}

#ifdef LED_CONTROL_SUPPORT
/*
	========================================================================
	
	Routine Description:
		Set LED Status

	Arguments:
		pAd						Pointer to our adapter
		Status					LED Status

	Return Value:
		None

	IRQL = PASSIVE_LEVEL
	IRQL = DISPATCH_LEVEL
	
	Note:
		
	========================================================================
*/
VOID RTMPSetLED(
	IN PRTMP_ADAPTER 	pAd, 
	IN UCHAR			Status)
{
	//ULONG			data;
	UCHAR			HighByte = 0;
	UCHAR			LowByte;
#ifdef WSC_INCLUDED
#ifdef WSC_LED_SUPPORT
	PWSC_CTRL		pWscControl = NULL;

#ifdef CONFIG_AP_SUPPORT
	pWscControl = &pAd->ApCfg.MBSSID[MAIN_MBSSID].WscControl;
#endif // CONFIG_AP_SUPPORT //
#endif // WSC_LED_SUPPORT //
#endif // WSC_INCLUDED //
	BOOLEAN 		bIgnored = FALSE;

#ifdef RALINK_ATE
	/*
		In ATE mode of RT2860 AP/STA, we have erased 8051 firmware.
		So LED mode is not supported when ATE is running.
	*/
	if (!IS_RT3572(pAd))
	{
		if (ATE_ON(pAd))
			return;
	}
#endif // RALINK_ATE //

	LowByte = pAd->LedCntl.field.LedMode&0x7f;
	switch (Status)
	{
		case LED_LINK_DOWN:
			HighByte = 0x20;
			AsicSendCommandToMcu(pAd, 0x50, 0xff, LowByte, HighByte);
			pAd->LedIndicatorStrength = 0; 
			break;
		case LED_LINK_UP:
#ifdef A_BAND_SUPPORT			
			if (pAd->CommonCfg.Channel > 14)
				HighByte = 0xa0;
			else
#endif // A_BAND_SUPPORT //				
				HighByte = 0x60;
			AsicSendCommandToMcu(pAd, 0x50, 0xff, LowByte, HighByte);
			break;
		case LED_RADIO_ON:
			HighByte = 0x20;
			AsicSendCommandToMcu(pAd, 0x50, 0xff, LowByte, HighByte);
			break;
		case LED_HALT: 
			LowByte = 0; // Driver sets MAC register and MAC controls LED
		case LED_RADIO_OFF:
			HighByte = 0;
			AsicSendCommandToMcu(pAd, 0x50, 0xff, LowByte, HighByte);
			break;
		case LED_WPS:
			HighByte = 0x10;
			AsicSendCommandToMcu(pAd, 0x50, 0xff, LowByte, HighByte);
			break;
		case LED_ON_SITE_SURVEY:
			HighByte = 0x08;
			AsicSendCommandToMcu(pAd, 0x50, 0xff, LowByte, HighByte);
			break;
		case LED_POWER_UP:
			HighByte = 0x04;
			AsicSendCommandToMcu(pAd, 0x50, 0xff, LowByte, HighByte);
			break;
#ifdef RALINK_ATE
#endif // RALINK_ATE //
#ifdef WSC_INCLUDED
#ifdef WSC_LED_SUPPORT
		case LED_WPS_IN_PROCESS:
			if (WscSupportWPSLEDMode(pAd))
			{
				HighByte = LINK_STATUS_WPS_IN_PROCESS;
				AsicSendCommandToMcu(pAd, MCU_SET_WPS_LED_MODE, 0xff, LowByte, HighByte);
				pWscControl->WscLEDMode = LED_WPS_IN_PROCESS;
				DBGPRINT(RT_DEBUG_TRACE, ("%s: LED_WPS_IN_PROCESS\n", __FUNCTION__));				
			}
			else
				bIgnored = TRUE;
			break;
		case LED_WPS_ERROR:
			if (WscSupportWPSLEDMode(pAd))
			{
				// In the case of LED mode 9, the error LED should be turned on only after WPS walk time expiration.
				if ((pWscControl->bWPSWalkTimeExpiration == FALSE) && 
					 ((pAd->LedCntl.field.LedMode & LED_MODE) == WPS_LED_MODE_9))
				{
					// do nothing.
				}
				else
				{
					HighByte = LINK_STATUS_WPS_ERROR;
					AsicSendCommandToMcu(pAd, MCU_SET_WPS_LED_MODE, 0xff, LowByte, HighByte);
				}
		
				pWscControl->WscLEDMode = LED_WPS_ERROR;
				pWscControl->WscLastWarningLEDMode = LED_WPS_ERROR;
				
				DBGPRINT(RT_DEBUG_TRACE, ("%s: LED_WPS_ERROR\n", __FUNCTION__));
			}
			else
				bIgnored = TRUE;
			break;
		case LED_WPS_SESSION_OVERLAP_DETECTED:
			if (WscSupportWPSLEDMode(pAd))
			{
				HighByte = LINK_STATUS_WPS_SESSION_OVERLAP_DETECTED;
				AsicSendCommandToMcu(pAd, MCU_SET_WPS_LED_MODE, 0xff, LowByte, HighByte);
				pWscControl->WscLEDMode = LED_WPS_SESSION_OVERLAP_DETECTED;
				pWscControl->WscLastWarningLEDMode = LED_WPS_SESSION_OVERLAP_DETECTED;
				DBGPRINT(RT_DEBUG_TRACE, ("%s: LED_WPS_SESSION_OVERLAP_DETECTED\n", __FUNCTION__));
			}
			else
				bIgnored = TRUE;
			break;
		case LED_WPS_SUCCESS:
			if (WscSupportWPSLEDMode(pAd))
			{
				if (((pAd->LedCntl.field.LedMode & LED_MODE) == WPS_LED_MODE_7) || 
					((pAd->LedCntl.field.LedMode & LED_MODE) == WPS_LED_MODE_11) || 
					((pAd->LedCntl.field.LedMode & LED_MODE) == WPS_LED_MODE_12))
				{
					// In the WPS LED mode 7, 11 and 12, the blue LED would last 300 seconds regardless of the AP's security settings.
					HighByte = LINK_STATUS_WPS_SUCCESS_WITH_SECURITY;
					AsicSendCommandToMcu(pAd, MCU_SET_WPS_LED_MODE, 0xff, LowByte, HighByte);
					pWscControl->WscLEDMode = LED_WPS_SUCCESS;
		
					// Turn off the WPS successful LED pattern after 300 seconds.
					RTMPSetTimer(&pWscControl->WscLEDTimer, WSC_SUCCESSFUL_LED_PATTERN_TIMEOUT);
					
					DBGPRINT(RT_DEBUG_TRACE, ("%s: LED_WPS_SUCCESS (LINK_STATUS_WPS_SUCCESS_WITH_SECURITY)\n", __FUNCTION__));
				}
				else if ((pAd->LedCntl.field.LedMode & LED_MODE) == WPS_LED_MODE_8) // The WPS LED mode 8
				{
					if (WscAPHasSecuritySetting(pAd, pWscControl)) // The WPS AP has the security setting.
					{
						HighByte = LINK_STATUS_WPS_SUCCESS_WITH_SECURITY;
						AsicSendCommandToMcu(pAd, MCU_SET_WPS_LED_MODE, 0xff, LowByte, HighByte);
						pWscControl->WscLEDMode = LED_WPS_SUCCESS;
		
						// Turn off the WPS successful LED pattern after 300 seconds.
						RTMPSetTimer(&pWscControl->WscLEDTimer, WSC_SUCCESSFUL_LED_PATTERN_TIMEOUT);
						
						DBGPRINT(RT_DEBUG_TRACE, ("%s: LED_WPS_SUCCESS (LINK_STATUS_WPS_SUCCESS_WITH_SECURITY)\n", __FUNCTION__));
					}
					else // The WPS AP does not have the secuirty setting.
					{
						HighByte = LINK_STATUS_WPS_SUCCESS_WITHOUT_SECURITY;
						AsicSendCommandToMcu(pAd, MCU_SET_WPS_LED_MODE, 0xff, LowByte, HighByte);
						pWscControl->WscLEDMode = LED_WPS_SUCCESS;
		
						// Turn off the WPS successful LED pattern after 300 seconds.
						RTMPSetTimer(&pWscControl->WscLEDTimer, WSC_SUCCESSFUL_LED_PATTERN_TIMEOUT);
						
						DBGPRINT(RT_DEBUG_TRACE, ("%s: LED_WPS_SUCCESS (LINK_STATUS_WPS_SUCCESS_WITHOUT_SECURITY)\n", __FUNCTION__));
					}
				}
				else if ((pAd->LedCntl.field.LedMode & LED_MODE) == WPS_LED_MODE_9) // The WPS LED mode 9.
				{
					// Always turn on the WPS blue LED for 300 seconds.
					HighByte = LINK_STATUS_WPS_BLUE_LED;
					AsicSendCommandToMcu(pAd, MCU_SET_WPS_LED_MODE, 0xff, LowByte, HighByte);
					pWscControl->WscLEDMode = LED_WPS_SUCCESS;
		
					// Turn off the WPS successful LED pattern after 300 seconds.
					RTMPSetTimer(&pWscControl->WscLEDTimer, WSC_SUCCESSFUL_LED_PATTERN_TIMEOUT);
					
					DBGPRINT(RT_DEBUG_TRACE, ("%s: LED_WPS_SUCCESS (LINK_STATUS_WPS_BLUE_LED)\n", __FUNCTION__));
				}
				else
				{
					DBGPRINT(RT_DEBUG_TRACE, ("%s: LED_WPS_SUCCESS (Incorrect LED mode = %d)\n", 
						__FUNCTION__, (pAd->LedCntl.field.LedMode & LED_MODE)));
					ASSERT(FALSE);
				}
			}
			else
				bIgnored = TRUE;
			break;
		case LED_WPS_TURN_LED_OFF:
			if (WscSupportWPSLEDMode(pAd))
			{
				HighByte = LINK_STATUS_WPS_TURN_LED_OFF;
				AsicSendCommandToMcu(pAd, MCU_SET_WPS_LED_MODE, 0xff, LowByte, HighByte);
				pWscControl->WscLEDMode = LED_WPS_TURN_LED_OFF;
				DBGPRINT(RT_DEBUG_TRACE, ("%s: LED_WPS_TURN_LED_OFF\n", __FUNCTION__));
			}
			else
				bIgnored = TRUE;
			break;
		case LED_WPS_TURN_ON_BLUE_LED:
			if (WscSupportWPSLEDMode(pAd))
			{
				HighByte = LINK_STATUS_WPS_BLUE_LED;
				AsicSendCommandToMcu(pAd, MCU_SET_WPS_LED_MODE, 0xff, LowByte, HighByte);
				pWscControl->WscLEDMode = LED_WPS_SUCCESS;
				DBGPRINT(RT_DEBUG_TRACE, ("%s: LED_WPS_TURN_ON_BLUE_LED\n", __FUNCTION__));
			}
			else
				bIgnored = TRUE;
			break;
		case LED_NORMAL_CONNECTION_WITHOUT_SECURITY:
			if (WscSupportWPSLEDMode(pAd))
			{
				HighByte = LINK_STATUS_NORMAL_CONNECTION_WITHOUT_SECURITY;
				AsicSendCommandToMcu(pAd, MCU_SET_WPS_LED_MODE, 0xff, LowByte, HighByte);
				pWscControl->WscLEDMode = LED_WPS_SUCCESS;
				DBGPRINT(RT_DEBUG_TRACE, ("%s: LINK_STATUS_NORMAL_CONNECTION_WITHOUT_SECURITY\n", __FUNCTION__));
			}
			else
				bIgnored = TRUE;
			break;
		case LED_NORMAL_CONNECTION_WITH_SECURITY:
			if (WscSupportWPSLEDMode(pAd))
			{
				HighByte = LINK_STATUS_NORMAL_CONNECTION_WITH_SECURITY;
				AsicSendCommandToMcu(pAd, MCU_SET_WPS_LED_MODE, 0xff, LowByte, HighByte);
				pWscControl->WscLEDMode = LED_WPS_SUCCESS;
				DBGPRINT(RT_DEBUG_TRACE, ("%s: LINK_STATUS_NORMAL_CONNECTION_WITH_SECURITY\n", __FUNCTION__));
			}
			else
				bIgnored = TRUE;
			break;
		//WPS LED Mode 10
		case LED_WPS_MODE10_TURN_ON:
			if(WscSupportWPSLEDMode10(pAd))
			{
				HighByte = LINK_STATUS_WPS_MODE10_TURN_ON;
				AsicSendCommandToMcu(pAd, MCU_SET_WPS_LED_MODE, 0xff, LowByte, HighByte);
				DBGPRINT(RT_DEBUG_TRACE, ("%s: LED_WPS_MODE10_TURN_ON\n", __FUNCTION__));
			}
			else
				bIgnored = TRUE;
			break;
		case LED_WPS_MODE10_FLASH:
			if(WscSupportWPSLEDMode10(pAd))
			{	
				HighByte = LINK_STATUS_WPS_MODE10_FLASH;
				AsicSendCommandToMcu(pAd, MCU_SET_WPS_LED_MODE, 0xff, LowByte, HighByte);
				DBGPRINT(RT_DEBUG_TRACE, ("%s: LED_WPS_MODE10_FLASH\n", __FUNCTION__));
			}
			else
				bIgnored = TRUE;
			break;
		case LED_WPS_MODE10_TURN_OFF:
			if(WscSupportWPSLEDMode10(pAd))
			{
				HighByte = LINK_STATUS_WPS_MODE10_TURN_OFF;
				AsicSendCommandToMcu(pAd, MCU_SET_WPS_LED_MODE, 0xff, LowByte, HighByte);
				DBGPRINT(RT_DEBUG_TRACE, ("%s: LED_WPS_MODE10_TURN_OFF\n", __FUNCTION__));
			}
			else
				bIgnored = TRUE;
			break;
#endif // WSC_LED_SUPPORT //
#endif // WSC_INCLUDED //
		default:
			DBGPRINT(RT_DEBUG_WARN, ("RTMPSetLED::Unknown Status %d\n", Status));
			break;
	}

    //
	// Keep LED status for LED SiteSurvey mode.
	// After SiteSurvey, we will set the LED mode to previous status.
	//
	if ((Status != LED_ON_SITE_SURVEY) && (Status != LED_POWER_UP) && (bIgnored == FALSE))
		pAd->LedStatus = Status;
    
	DBGPRINT(RT_DEBUG_TRACE, ("RTMPSetLED::Mode=%d,HighByte=0x%02x,LowByte=0x%02x\n", pAd->LedCntl.field.LedMode, HighByte, LowByte));
}

/*
	========================================================================
	
	Routine Description:
		Set LED Signal Stregth 

	Arguments:
		pAd						Pointer to our adapter
		Dbm						Signal Stregth

	Return Value:
		None

	IRQL = PASSIVE_LEVEL
	
	Note:
		Can be run on any IRQL level. 

		According to Microsoft Zero Config Wireless Signal Stregth definition as belows.
		<= -90  No Signal
		<= -81  Very Low
		<= -71  Low
		<= -67  Good
		<= -57  Very Good
		 > -57  Excellent		
	========================================================================
*/
VOID RTMPSetSignalLED(
	IN PRTMP_ADAPTER 	pAd, 
	IN NDIS_802_11_RSSI Dbm)
{
	UCHAR		nLed = 0;

	if (pAd->LedCntl.field.LedMode == LED_MODE_SIGNAL_STREGTH)
	{
	if (Dbm <= -90)
		nLed = 0;
	else if (Dbm <= -81)
		nLed = 1;
	else if (Dbm <= -71)
		nLed = 3;
	else if (Dbm <= -67)
		nLed = 7;
	else if (Dbm <= -57)
		nLed = 15;
	else 
		nLed = 31;

	//
	// Update Signal Stregth to firmware if changed.
	//
	if (pAd->LedIndicatorStrength != nLed)
	{
		AsicSendCommandToMcu(pAd, 0x51, 0xff, nLed, pAd->LedCntl.field.Polarity);
		pAd->LedIndicatorStrength = nLed;
	}
}
}
#endif // LED_CONTROL_SUPPORT //

#ifdef WSC_STA_SUPPORT
#ifdef WSC_LED_SUPPORT
//#ifdef LED_CONTROL_SUPPORT
//
// LED indication for normal connection start.
//
VOID
LEDConnectionStart(
	IN PRTMP_ADAPTER pAd)
{
	// LED indication.
	//if (pAd->StaCfg.WscControl.bWPSSession == FALSE)
	//if (pAd->StaCfg.WscControl.WscConfMode != WSC_DISABLE && pAd->StaCfg.WscControl.bWscTrigger)
	if (pAd->StaCfg.WscControl.WscConfMode == WSC_DISABLE)
	{
		if ((pAd->LedCntl.field.LedMode & LED_MODE) == WPS_LED_MODE_9) // LED mode 9.
		{
			UCHAR WPSLEDStatus = 0;
			
			// The AP uses OPEN-NONE.
			if ((pAd->StaCfg.AuthMode == Ndis802_11AuthModeOpen) && (pAd->StaCfg.WepStatus == Ndis802_11WEPDisabled))
			{
				WPSLEDStatus = LED_WPS_TURN_LED_OFF;
#ifdef RTMP_MAC_PCI
				RTMPSetLED(pAd, WPSLEDStatus);
#endif // RTMP_MAC_PCI //
				DBGPRINT(RT_DEBUG_TRACE, ("%s: LED_WPS_TURN_LED_OFF\n", __FUNCTION__));
			}
			else // The AP uses an encryption algorithm.
			{
				WPSLEDStatus = LED_WPS_IN_PROCESS;
#ifdef RTMP_MAC_PCI
				RTMPSetLED(pAd, WPSLEDStatus);
#endif // RTMP_MAC_PCI //
				DBGPRINT(RT_DEBUG_TRACE, ("%s: LED_WPS_IN_PROCESS\n", __FUNCTION__));
			}
		}
	}
}


//
// LED indication for normal connection completion.
//
VOID
LEDConnectionCompletion(
	IN PRTMP_ADAPTER pAd, 
	IN BOOLEAN bSuccess)
{
	// LED indication.
	//if (pAd->StaCfg.WscControl.bWPSSession == FALSE)
	if (pAd->StaCfg.WscControl.WscConfMode == WSC_DISABLE)
	{
		if ((pAd->LedCntl.field.LedMode & LED_MODE) == WPS_LED_MODE_9) // LED mode 9.
		{
			UCHAR WPSLEDStatus = 0;
			
			if (bSuccess == TRUE) // Successful connenction.
			{				
				// The AP uses OPEN-NONE.
				if ((pAd->StaCfg.AuthMode == Ndis802_11AuthModeOpen) && (pAd->StaCfg.WepStatus == Ndis802_11WEPDisabled))
				{
					WPSLEDStatus = LED_NORMAL_CONNECTION_WITHOUT_SECURITY;
#ifdef RTMP_MAC_PCI
					RTMPSetLED(pAd, WPSLEDStatus);
#endif // RTMP_MAC_PCI //
					DBGPRINT(RT_DEBUG_TRACE, ("%s: LED_NORMAL_CONNECTION_WITHOUT_SECURITY\n", __FUNCTION__));
				}
				else // The AP uses an encryption algorithm.
				{
					WPSLEDStatus = LED_NORMAL_CONNECTION_WITH_SECURITY;
#ifdef RTMP_MAC_PCI
					RTMPSetLED(pAd, WPSLEDStatus);
#endif // RTMP_MAC_PCI //
					DBGPRINT(RT_DEBUG_TRACE, ("%s: LED_NORMAL_CONNECTION_WITH_SECURITY\n", __FUNCTION__));
				}
			}
			else // Connection failure.
			{
				WPSLEDStatus = LED_WPS_TURN_LED_OFF;
#ifdef RTMP_MAC_PCI
				RTMPSetLED(pAd, WPSLEDStatus);
#endif // RTMP_MAC_PCI //
				DBGPRINT(RT_DEBUG_TRACE, ("%s: LED_WPS_TURN_LED_OFF\n", __FUNCTION__));
			}
		}
	}
}
//#endif // LED_CONTROL_SUPPORT //
#endif // WSC_LED_SUPPORT //
#endif // WSC_STA_SUPPORT //


/*
	========================================================================
	
	Routine Description:
		Enable RX 

	Arguments:
		pAd						Pointer to our adapter

	Return Value:
		None

	IRQL <= DISPATCH_LEVEL
	
	Note:
		Before Enable RX, make sure you have enabled Interrupt.
	========================================================================
*/
VOID RTMPEnableRxTx(
	IN PRTMP_ADAPTER	pAd)
{
//	WPDMA_GLO_CFG_STRUC	GloCfg;
//	ULONG	i = 0;
	UINT32 rx_filter_flag;

	DBGPRINT(RT_DEBUG_TRACE, ("==> RTMPEnableRxTx\n"));

	// Enable Rx DMA.
	RT28XXDMAEnable(pAd);

	// enable RX of MAC block
	if (pAd->OpMode == OPMODE_AP)
	{
		rx_filter_flag = APNORMAL;

#ifdef CONFIG_AP_SUPPORT
#ifdef IDS_SUPPORT
		IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
		{
			if (pAd->ApCfg.IdsEnable)
				rx_filter_flag &= (~0x4);	// Don't drop those not-U2M frames
		}
#endif // IDS_SUPPORT //			
#endif // CONFIG_AP_SUPPORT //

		RTMP_IO_WRITE32(pAd, RX_FILTR_CFG, rx_filter_flag);     // enable RX of DMA block
	}

	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, 0xc);
	DBGPRINT(RT_DEBUG_TRACE, ("<== RTMPEnableRxTx\n"));	
}


//+++Add by shiang, move from os/linux/rt_main_dev.c
void CfgInitHook(PRTMP_ADAPTER pAd)
{
	pAd->bBroadComHT = TRUE;
}


int rt28xx_init(
	IN PRTMP_ADAPTER pAd, 
	IN PSTRING pDefaultMac, 
	IN PSTRING pHostName)
{
	UINT					index;
	UCHAR					TmpPhy;
	NDIS_STATUS				Status;
	UINT32 					MacCsr0 = 0;



	// reset Adapter flags
	RTMP_CLEAR_FLAGS(pAd);

#ifndef VENDOR_FEATURE3_SUPPORT
	// Init BssTab & ChannelInfo tabbles for auto channel select.
#ifdef CONFIG_AP_SUPPORT	
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
	{
		AutoChBssTableInit(pAd);
		ChannelInfoInit(pAd);
	}
#endif // CONFIG_AP_SUPPORT //
#endif // VENDOR_FEATURE3_SUPPORT //

#ifdef DOT11_N_SUPPORT
	// Allocate BA Reordering memory
	ba_reordering_resource_init(pAd, MAX_REORDERING_MPDU_NUM);
#endif // DOT11_N_SUPPORT //

	// Make sure MAC gets ready.
	index = 0;
	do
	{
		RTMP_IO_READ32(pAd, MAC_CSR0, &MacCsr0);
		pAd->MACVersion = MacCsr0;

		if ((pAd->MACVersion != 0x00) && (pAd->MACVersion != 0xFFFFFFFF))
			break;

		RTMPusecDelay(10);
	} while (index++ < 100);
	DBGPRINT(RT_DEBUG_TRACE, ("MAC_CSR0  [ Ver:Rev=0x%08x]\n", pAd->MACVersion));

#ifdef RTMP_MAC_PCI
#if defined(RT3090) || defined(RT3572) || defined(RT3390)
	/*Iverson patch PCIE L1 issue to make sure that driver can be read,write ,BBP and RF register  at pcie L.1 level */
	if ((IS_RT3090(pAd) || IS_RT3572(pAd) || IS_RT3390(pAd)) && pAd->infType==RTMP_DEV_INF_PCIE)
	{
		RTMP_IO_READ32(pAd, AUX_CTRL, &MacCsr0);
		MacCsr0 |= 0x402;
		RTMP_IO_WRITE32(pAd, AUX_CTRL, MacCsr0);
		DBGPRINT(RT_DEBUG_TRACE, ("AUX_CTRL = 0x%x\n", MacCsr0));
	}
#endif // RT3090 //

	// To fix driver disable/enable hang issue when radio off
	RTMP_IO_WRITE32(pAd, PWR_PIN_CFG, 0x2);
#endif // RTMP_MAC_PCI //

	// Disable DMA
	RT28XXDMADisable(pAd);


	// Load 8051 firmware
	Status = NICLoadFirmware(pAd);
	if (Status != NDIS_STATUS_SUCCESS)
	{
		DBGPRINT_ERR(("NICLoadFirmware failed, Status[=0x%08x]\n", Status));
		goto err1;
	}

	NICLoadRateSwitchingParams(pAd);

	// Disable interrupts here which is as soon as possible
	// This statement should never be true. We might consider to remove it later
#ifdef RTMP_MAC_PCI
	if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_ACTIVE))
	{
		RTMP_ASIC_INTERRUPT_DISABLE(pAd);
	}
#endif // RTMP_MAC_PCI //

	Status = RTMPAllocTxRxRingMemory(pAd);
	if (Status != NDIS_STATUS_SUCCESS)
	{
		DBGPRINT_ERR(("RTMPAllocDMAMemory failed, Status[=0x%08x]\n", Status));
		goto err1;
	}

	RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_IN_USE);

	// initialize MLME
	//

	Status = RtmpMgmtTaskInit(pAd);
	if (Status != NDIS_STATUS_SUCCESS)
		goto err2;

	Status = MlmeInit(pAd);
	if (Status != NDIS_STATUS_SUCCESS)
	{
		DBGPRINT_ERR(("MlmeInit failed, Status[=0x%08x]\n", Status));
		goto err2;
	}

	// Initialize pAd->StaCfg, pAd->ApCfg, pAd->CommonCfg to manufacture default
	//
	UserCfgInit(pAd);
	Status = RtmpNetTaskInit(pAd);
	if (Status != NDIS_STATUS_SUCCESS)
		goto err3;

//	COPY_MAC_ADDR(pAd->ApCfg.MBSSID[apidx].Bssid, netif->hwaddr);
//	pAd->bForcePrintTX = TRUE;

	CfgInitHook(pAd);

#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
		APInitialize(pAd);
#endif // CONFIG_AP_SUPPORT //	

#ifdef BLOCK_NET_IF
	initblockQueueTab(pAd);
#endif // BLOCK_NET_IF //


	MeasureReqTabInit(pAd);
	TpcReqTabInit(pAd);

	//
	// Init the hardware, we need to init asic before read registry, otherwise mac register will be reset
	//
	Status = NICInitializeAdapter(pAd, TRUE);
	if (Status != NDIS_STATUS_SUCCESS)
	{
		DBGPRINT_ERR(("NICInitializeAdapter failed, Status[=0x%08x]\n", Status));
		if (Status != NDIS_STATUS_SUCCESS)
		goto err3;
	}	

	// Read parameters from Config File 
	Status = RTMPReadParametersHook(pAd);

	DBGPRINT(RT_DEBUG_OFF, ("1. Phy Mode = %d\n", pAd->CommonCfg.PhyMode));
	if (Status != NDIS_STATUS_SUCCESS)
	{
		DBGPRINT_ERR(("RTMPReadParametersHook failed, Status[=0x%08x]\n",Status));
		goto err4;
	}



#ifdef DOT11_N_SUPPORT
   	//Init Ba Capability parameters.
//	RT28XX_BA_INIT(pAd);
	pAd->CommonCfg.DesiredHtPhy.MpduDensity = (UCHAR)pAd->CommonCfg.BACapability.field.MpduDensity;
	pAd->CommonCfg.DesiredHtPhy.AmsduEnable = (USHORT)pAd->CommonCfg.BACapability.field.AmsduEnable;
	pAd->CommonCfg.DesiredHtPhy.AmsduSize = (USHORT)pAd->CommonCfg.BACapability.field.AmsduSize;
	pAd->CommonCfg.DesiredHtPhy.MimoPs = (USHORT)pAd->CommonCfg.BACapability.field.MMPSmode;
	// UPdata to HT IE
	pAd->CommonCfg.HtCapability.HtCapInfo.MimoPs = (USHORT)pAd->CommonCfg.BACapability.field.MMPSmode;
	pAd->CommonCfg.HtCapability.HtCapInfo.AMsduSize = (USHORT)pAd->CommonCfg.BACapability.field.AmsduSize;
	pAd->CommonCfg.HtCapability.HtCapParm.MpduDensity = (UCHAR)pAd->CommonCfg.BACapability.field.MpduDensity;
#endif // DOT11_N_SUPPORT //

	// after reading Registry, we now know if in AP mode or STA mode

	// Load 8051 firmware; crash when FW image not existent
	// Status = NICLoadFirmware(pAd);
	// if (Status != NDIS_STATUS_SUCCESS)
	//    break;

	DBGPRINT(RT_DEBUG_OFF, ("2. Phy Mode = %d\n", pAd->CommonCfg.PhyMode));

	// We should read EEPROM for all cases.  rt2860b
	NICReadEEPROMParameters(pAd, (PUCHAR)pDefaultMac);	

	DBGPRINT(RT_DEBUG_OFF, ("3. Phy Mode = %d\n", pAd->CommonCfg.PhyMode));

#ifdef RTMP_RF_RW_SUPPORT
	//Init RT30xx RFRegisters after read RFIC type from EEPROM
	NICInitRFRegisters(pAd);
#endif // RTMP_RF_RW_SUPPORT //

	NICInitAsicFromEEPROM(pAd); //rt2860b

#ifdef RTMP_INTERNAL_TX_ALC
	//
	// Initialize the desired TSSI table
	//
	InitDesiredTSSITable(pAd);
#endif // RTMP_INTERNAL_TX_ALC //
	
	/*
	     Post-process the BBP registers based on the chip model
	     N.B. the PostBBPInitialization function should be called after the NICInitAsicFromEEPROM function.
	     (reference the pAd->Antenna member).
	     only support for RT3090B3 / RT3390 / RT3370 currently.
	 */
	if ((IS_RT3090(pAd)) || (IS_RT3390(pAd)))
		PostBBPInitialization(pAd);

	// Set PHY to appropriate mode
	{
	TmpPhy = pAd->CommonCfg.PhyMode;
	pAd->CommonCfg.PhyMode = 0xff;
	RTMPSetPhyMode(pAd, TmpPhy);
#ifdef DOT11_N_SUPPORT
	SetCommonHT(pAd);
#endif // DOT11_N_SUPPORT //
	}

	// No valid channels.
	if (pAd->ChannelListNum == 0)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("Wrong configuration. No valid channel found. Check \"ContryCode\" and \"ChannelGeography\" setting.\n"));
		goto err4;
	}

#ifdef DOT11_N_SUPPORT
	DBGPRINT(RT_DEBUG_OFF, ("MCS Set = %02x %02x %02x %02x %02x\n", pAd->CommonCfg.HtCapability.MCSSet[0],
           pAd->CommonCfg.HtCapability.MCSSet[1], pAd->CommonCfg.HtCapability.MCSSet[2],
           pAd->CommonCfg.HtCapability.MCSSet[3], pAd->CommonCfg.HtCapability.MCSSet[4]));
#endif // DOT11_N_SUPPORT //

#ifdef RTMP_RF_RW_SUPPORT
	//Init RT30xx RFRegisters after read RFIC type from EEPROM
//	NICInitRFRegisters(pAd);
#endif // RTMP_RF_RW_SUPPORT //


#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
	{
		/* init QBSS Element */
		QBSS_LoadInit(pAd);

	}
#endif // CONFIG_AP_SUPPORT //

//		APInitialize(pAd);

#ifdef IKANOS_VX_1X0
	VR_IKANOS_FP_Init(pAd->ApCfg.BssidNum, pAd->PermanentAddress);
#endif // IKANOS_VX_1X0 //

		//
	// Initialize RF register to default value
	//
	AsicSwitchChannel(pAd, pAd->CommonCfg.Channel, FALSE);
	AsicLockChannel(pAd, pAd->CommonCfg.Channel);		

	// 8051 firmware require the signal during booting time.
	//2008/11/28:KH marked the following codes to patch Frequency offset bug
	//AsicSendCommandToMcu(pAd, 0x72, 0xFF, 0x00, 0x00);

	if (pAd && (Status != NDIS_STATUS_SUCCESS))
	{
		//
		// Undo everything if it failed
		//
		if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_IN_USE))
		{
//			NdisMDeregisterInterrupt(&pAd->Interrupt);
			RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_IN_USE);
		}
//		RTMPFreeAdapter(pAd); // we will free it in disconnect()
	}
	else if (pAd)
	{
		// Microsoft HCT require driver send a disconnect event after driver initialization.
		OPSTATUS_CLEAR_FLAG(pAd, fOP_STATUS_MEDIA_STATE_CONNECTED);
//		pAd->IndicateMediaState = NdisMediaStateDisconnected;
		RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_MEDIA_STATE_CHANGE);

		DBGPRINT(RT_DEBUG_TRACE, ("NDIS_STATUS_MEDIA_DISCONNECT Event B!\n"));

#ifdef CONFIG_AP_SUPPORT
		IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
		{
			if (pAd->ApCfg.bAutoChannelAtBootup || (pAd->CommonCfg.Channel == 0))
			{
				UINT8 BBPValue = 0;
				
				// Enable Interrupt first due to we need to scan channel to receive beacons.
				RTMP_IRQ_ENABLE(pAd);
				// Now Enable RxTx
				RTMPEnableRxTx(pAd);
				RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_START_UP);

				// Let BBP register at 20MHz to do scan		
				RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R4, &BBPValue);
				BBPValue &= (~0x18);
				RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R4, BBPValue);
				DBGPRINT(RT_DEBUG_ERROR, ("SYNC - BBP R4 to 20MHz.l\n"));

				// Now we can receive the beacon and do the listen beacon
				pAd->CommonCfg.Channel = AP_AUTO_CH_SEL(pAd, TRUE);
			}

#ifdef DOT11_N_SUPPORT
			// If phymode > PHY_11ABGN_MIXED and BW=40 check extension channel, after select channel  
			N_ChannelCheck(pAd);

#ifdef DOT11N_DRAFT3
			/* 
				We only do this Overlapping BSS Scan when system up, for the 
				other situation of channel changing, we depends on station's 
				report to adjust ourself.
			*/
			if (pAd->CommonCfg.bForty_Mhz_Intolerant == TRUE)
			{
				DBGPRINT(RT_DEBUG_TRACE, ("Disable 20/40 BSSCoex Channel Scan(BssCoex=%d, 40MHzIntolerant=%d)\n", 
											pAd->CommonCfg.bBssCoexEnable, 
											pAd->CommonCfg.bForty_Mhz_Intolerant));
			}
			else if(pAd->CommonCfg.bBssCoexEnable == TRUE)
			{	
				DBGPRINT(RT_DEBUG_TRACE, ("Enable 20/40 BSSCoex Channel Scan(BssCoex=%d)\n", 
							pAd->CommonCfg.bBssCoexEnable));
				APOverlappingBSSScan(pAd);
			}

			RTMP_11N_D3_TimerInit(pAd);
//			RTMPInitTimer(pAd, &pAd->CommonCfg.Bss2040CoexistTimer, GET_TIMER_FUNCTION(Bss2040CoexistTimeOut), pAd, FALSE);
#endif // DOT11N_DRAFT3 //
#endif // DOT11_N_SUPPORT //

			APStartUp(pAd);
			DBGPRINT(RT_DEBUG_OFF, ("Main bssid = %02x:%02x:%02x:%02x:%02x:%02x\n", 
									PRINT_MAC(pAd->ApCfg.MBSSID[BSS0].Bssid)));
		}
#endif // CONFIG_AP_SUPPORT //

	}// end of else

#ifdef WSC_INCLUDED
#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
	{
		INT apidx;
#ifdef HOSTAPD_SUPPORT
		if (pAd->ApCfg.Hostapd == TRUE)
		{
			DBGPRINT(RT_DEBUG_TRACE, ("WPS is control by hostapd now.\n"));
		}
		else
#endif //HOSTAPD_SUPPORT//
		for (apidx = 0; apidx < pAd->ApCfg.BssidNum; apidx++)
		{
			PWSC_CTRL pWscControl;
			pWscControl = &pAd->ApCfg.MBSSID[apidx].WscControl;
			DBGPRINT(RT_DEBUG_TRACE, ("Generate UUID for apidx(%d)\n", apidx));
			WscGenerateUUID(pAd, &pWscControl->Wsc_Uuid_E[0], &pWscControl->Wsc_Uuid_Str[0], apidx, FALSE);
			WscInit(pAd, FALSE, apidx);
			if (pWscControl->WscEnrolleePinCode == 0)
				pWscControl->WscEnrolleePinCode = WscGeneratePinCode(pAd, FALSE, apidx);
		}
	}
#endif // CONFIG_AP_SUPPORT //


	/* WSC hardware push button function 0811 */
	WSC_HDR_BTN_Init(pAd);
#endif // WSC_INCLUDED //

	// Set up the Mac address
	RtmpOSNetDevAddrSet(pAd->net_dev, &pAd->CurrentAddress[0]);

	// Various AP function init
#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
	{
#ifdef MBSS_SUPPORT
		/* the function can not be moved to RT2860_probe() even register_netdev()
		   is changed as register_netdevice().
		   Or in some PC, kernel will panic (Fedora 4) */
		RT28xx_MBSS_Init(pAd, pAd->net_dev);
#endif // MBSS_SUPPORT //

#ifdef WDS_SUPPORT
		RT28xx_WDS_Init(pAd, pAd->net_dev);
#endif // WDS_SUPPORT //

#ifdef APCLI_SUPPORT
		RT28xx_ApCli_Init(pAd, pAd->net_dev);
#endif // APCLI_SUPPORT //
	}
#endif // CONFIG_AP_SUPPORT //


#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
	{
#ifdef MAT_SUPPORT
		MATEngineInit(pAd);
#endif // MAT_SUPPORT //

#ifdef CLIENT_WDS
	CliWds_ProxyTabInit(pAd);
#endif // CLIENT_WDS //
	}
#endif // CONFIG_AP_SUPPORT //


#ifdef WMM_ACM_SUPPORT
#ifdef CONFIG_AP_SUPPORT
	ACMP_Init(pAd,
			pAd->CommonCfg.APEdcaParm.bACM[0],
			pAd->CommonCfg.APEdcaParm.bACM[1],
			pAd->CommonCfg.APEdcaParm.bACM[2],
			pAd->CommonCfg.APEdcaParm.bACM[3], 0);
#endif // CONFIG_AP_SUPPORT //
#endif // WMM_ACM_SUPPORT //


#ifdef RT33xx
	if (IS_RT3390(pAd))
	{
		RTMP_TxEvmCalibration(pAd);
	}	
#endif // RT33xx //

	DBGPRINT_S(Status, ("<==== rt28xx_init, Status=%x\n", Status));

	return TRUE;


err4:
err3:
	MlmeHalt(pAd);
err2:
	RTMPFreeTxRxRingMemory(pAd);
err1:

#ifdef DOT11_N_SUPPORT
	os_free_mem(pAd, pAd->mpdu_blk_pool.mem); // free BA pool
#endif // DOT11_N_SUPPORT //

	// shall not set priv to NULL here because the priv didn't been free yet.
	//net_dev->priv = 0;
#ifdef INF_AMAZON_SE
err0:
#endif // INF_AMAZON_SE //
#ifdef ST
err0:
#endif // ST //

	DBGPRINT(RT_DEBUG_ERROR, ("!!! rt28xx Initialized fail !!!\n"));
	return FALSE;
}
//---Add by shiang, move from os/linux/rt_main_dev.c


static INT RtmpChipOpsRegister(
	IN RTMP_ADAPTER *pAd,
	IN INT			infType)
{
	RTMP_CHIP_OP	*pChipOps = &pAd->chipOps;
	int status;
	
	memset(pChipOps, 0, sizeof(RTMP_CHIP_OP));

	/* set eeprom related hook functions */
	status = RtmpChipOpsEepromHook(pAd, infType);

	/* set mcu related hook functions */
	switch(infType)
	{
#ifdef RTMP_PCI_SUPPORT
		case RTMP_DEV_INF_PCI:
		case RTMP_DEV_INF_PCIE:
			pChipOps->loadFirmware = RtmpAsicLoadFirmware;
			pChipOps->eraseFirmware = RtmpAsicEraseFirmware;
			pChipOps->sendCommandToMcu = RtmpAsicSendCommandToMcu;
			break;
#endif // RTMP_PCI_SUPPORT //


		default:
			break;
	}

	return status;
}


INT RtmpRaDevCtrlInit(
	IN RTMP_ADAPTER *pAd, 
	IN RTMP_INF_TYPE infType)
{
	//VOID	*handle;

	// Assign the interface type. We need use it when do register/EEPROM access.
	pAd->infType = infType;

	

#ifdef CONFIG_AP_SUPPORT
	pAd->OpMode = OPMODE_AP;
	DBGPRINT(RT_DEBUG_TRACE, ("AP Driver version-%s\n", AP_DRIVER_VERSION));
#endif // CONFIG_AP_SUPPORT //


	RtmpChipOpsRegister(pAd, infType);

#ifdef MULTIPLE_CARD_SUPPORT
{
	extern BOOLEAN RTMP_CardInfoRead(PRTMP_ADAPTER pAd);

	// find its profile path
	pAd->MC_RowID = -1; // use default profile path
	RTMP_CardInfoRead(pAd);

	if (pAd->MC_RowID == -1)
#ifdef CONFIG_AP_SUPPORT
		strcpy(pAd->MC_FileName, AP_PROFILE_PATH);
#endif // CONFIG_AP_SUPPORT //

	DBGPRINT(RT_DEBUG_TRACE, ("MC> ROW = %d, PATH = %s\n", pAd->MC_RowID, pAd->MC_FileName));
}
#endif // MULTIPLE_CARD_SUPPORT //

	return 0;
}


BOOLEAN RtmpRaDevCtrlExit(IN RTMP_ADAPTER *pAd)
{
#ifdef MULTIPLE_CARD_SUPPORT
extern UINT8  MC_CardUsed[MAX_NUM_OF_MULTIPLE_CARD];

	if ((pAd->MC_RowID >= 0) && (pAd->MC_RowID <= MAX_NUM_OF_MULTIPLE_CARD))
		MC_CardUsed[pAd->MC_RowID] = 0; // not clear MAC address
#endif // MULTIPLE_CARD_SUPPORT //


	RTMPFreeAdapter(pAd);

	return TRUE;
}


// not yet support MBSS
PNET_DEV get_netdev_from_bssid(
	IN	PRTMP_ADAPTER	pAd,
	IN	UCHAR			FromWhichBSSID)
{
	PNET_DEV dev_p = NULL;

	
#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
	{
		UCHAR infRealIdx;

		infRealIdx = FromWhichBSSID & (NET_DEVICE_REAL_IDX_MASK);
#ifdef APCLI_SUPPORT
		if(FromWhichBSSID >= MIN_NET_DEVICE_FOR_APCLI)
		{
			dev_p = (infRealIdx > MAX_APCLI_NUM ? NULL : pAd->ApCfg.ApCliTab[infRealIdx].dev);
		} 
		else
#endif // APCLI_SUPPORT //
#ifdef WDS_SUPPORT
		if(FromWhichBSSID >= MIN_NET_DEVICE_FOR_WDS)
		{
			dev_p = ((infRealIdx > MAX_WDS_ENTRY) ? NULL : pAd->WdsTab.WdsEntry[infRealIdx].dev);
		}
		else
#endif // WDS_SUPPORT //
		{
			if (FromWhichBSSID >= pAd->ApCfg.BssidNum)
	    		{
				DBGPRINT(RT_DEBUG_ERROR, ("%s: fatal error ssid > ssid num!\n", __FUNCTION__));
				dev_p = pAd->net_dev;
	    		}

	    		if (FromWhichBSSID == BSS0)
				dev_p = pAd->net_dev;
	    		else
	    		{
	    	    		dev_p = pAd->ApCfg.MBSSID[FromWhichBSSID].MSSIDDev;
	    		}
		}
	}
#endif // CONFIG_AP_SUPPORT //


	ASSERT(dev_p);
	return dev_p; /* return one of MBSS */
}


#ifdef CONFIG_AP_SUPPORT
#ifdef DOT11_N_SUPPORT
#ifdef DOT11N_DRAFT3
VOID RTMP_11N_D3_TimerInit(
IN PRTMP_ADAPTER pAd)
{
	RTMPInitTimer(pAd, &pAd->CommonCfg.Bss2040CoexistTimer, GET_TIMER_FUNCTION(Bss2040CoexistTimeOut), pAd, FALSE);
}
#endif // DOT11N_DRAFT3 //
#endif // DOT11_N_SUPPORT //
#endif // CONFIG_AP_SUPPORT //


#ifdef VENDOR_FEATURE3_SUPPORT
VOID RTMP_RF_IO_WRITE32(
	IN PRTMP_ADAPTER	pAd,
	IN UINT32 Value)
{
    	if (pAd->bPCIclkOff == FALSE) 	              
	{											
		PHY_CSR4_STRUC  PhyCsr;                        
		ULONG           BusyCnt = 0;                   
											
		do {                                            
			RTMP_IO_READ32(pAd, RF_CSR_CFG0, &PhyCsr.word); 
			if (PhyCsr.field.Busy == IDLE)            
				break;                                
			BusyCnt++;                                 
		}while (BusyCnt < MAX_BUSY_COUNT);			
		if(BusyCnt < MAX_BUSY_COUNT)                 
		{                                               
			RTMP_IO_WRITE32(pAd, RF_CSR_CFG0, Value);        
    		}                                              
    	}	                                              
}

VOID RTMP_IO_READ32(
	IN PRTMP_ADAPTER pAd,
	IN UINT32 Offset,
	OUT UINT32 *pValue)							
{																
    if (pAd->bPCIclkOff == FALSE)                                  
    {         
    		UINT Val;
		(Val = readl((void *)(pAd->CSRBaseAddress + MAC_CSR0)));		
		(Val = readl((void *)(pAd->CSRBaseAddress + Offset)));
#if defined(INF_TWINPASS) || defined(INF_DANUBE) || defined(INF_AR9) || defined(IKANOS_VX_1X0)
		*pValue = SWAP32(Val);
#else
		*pValue = Val;
#endif  // defined(INF_TWINPASS) || defined(INF_DANUBE) || defined(INF_AR9) || defined(IKANOS_VX_1X0) //
    }                                                               
    else															
		*pValue = 0;													
}

VOID RTMP_IO_WRITE32(
	IN PRTMP_ADAPTER pAd,
	IN UINT32 Offset,
	IN UINT32 Value)
{
    if (pAd->bPCIclkOff == FALSE)
    {
		UINT Val;

		Val = readl((void *)(pAd->CSRBaseAddress + MAC_CSR0));
#if defined(INF_TWINPASS) || defined(INF_DANUBE) || defined(INF_AR9) || defined(IKANOS_VX_1X0)
		Val = SWAP32(Value);
#else
		Val = Value;
#endif
		writel(Val, (void *)(pAd->CSRBaseAddress + Offset));
    }
}

VOID RTMP_BBP_IO_READ8_BY_REG_ID(
	IN PRTMP_ADAPTER pAd,
	IN UINT32 Offset,
	OUT UINT8 *pValue)
{
	if (pAd->bPCIclkOff == FALSE)
	{
		if (pAd->infType == RTMP_DEV_INF_RBUS)
			RTMP_BBP_IO_READ8(pAd, Offset, pValue, FALSE);
		else
			if (IS_SUPPORT_PCIE_PS_L3(pAd))
				RTMP_PCIE_PS_L3_BBP_IO_READ8(pAd, Offset, pValue, TRUE);
			else
				RTMP_BBP_IO_READ8(pAd, Offset, pValue, TRUE);
	}
}

VOID RTMP_BBP_IO_READ8(
	IN PRTMP_ADAPTER pAd,
	IN UCHAR Offset,
	OUT UINT8 *pValue,
	IN BOOLEAN FlgValidMCR)
{
	BBP_CSR_CFG_STRUC  BbpCsr;
	int   busyCnt, secCnt, regID;

	regID = ((FlgValidMCR) == TRUE ? H2M_BBP_AGENT : BBP_CSR_CFG);
	for (busyCnt=0; busyCnt<MAX_BUSY_COUNT; busyCnt++)
	{
		RTMP_IO_READ32(pAd, regID, &BbpCsr.word);
		if (BbpCsr.field.Busy == BUSY)
			continue;
		BbpCsr.word = 0;
		BbpCsr.field.fRead = 1;
		BbpCsr.field.BBP_RW_MODE = 1;
		BbpCsr.field.Busy = 1;
		BbpCsr.field.RegNum = Offset;
		RTMP_IO_WRITE32(pAd, regID, BbpCsr.word);
		if ((FlgValidMCR) == TRUE)
		{
			AsicSendCommandToMcu(pAd, 0x80, 0xff, 0x0, 0x0);
			RTMPusecDelay(1000);
		}
		for (secCnt=0; secCnt<MAX_BUSY_COUNT; secCnt++)
		{
			RTMP_IO_READ32(pAd, regID, &BbpCsr.word);
			if (BbpCsr.field.Busy == IDLE)
				break;
		}
		if ((BbpCsr.field.Busy == IDLE) &&
			(BbpCsr.field.RegNum == Offset))
		{
			*(pValue) = (UCHAR)BbpCsr.field.Value;
			break;
		}
	}
	if (BbpCsr.field.Busy == BUSY)
	{
		DBGPRINT_ERR(("BBP(viaMCU=%d) read R%d fail\n", (FlgValidMCR), Offset));
		*(pValue) = (pAd)->BbpWriteLatch[Offset];
		if ((FlgValidMCR) == TRUE)
		{
			RTMP_IO_READ32(pAd, regID, &BbpCsr.word);
			BbpCsr.field.Busy = 0;
			RTMP_IO_WRITE32(pAd, regID, BbpCsr.word);
		}
	}
}

VOID RTMP_BBP_IO_WRITE8_BY_REG_ID(
	IN PRTMP_ADAPTER pAd,
	IN UINT32 Offset,
	IN UINT8 Value)
{
	if ((pAd)->bPCIclkOff == FALSE)
	{
		if ((pAd)->infType == RTMP_DEV_INF_RBUS)
			RTMP_BBP_IO_WRITE8((pAd), Offset, Value, FALSE);
		else
			if (IS_SUPPORT_PCIE_PS_L3((pAd)))
				RTMP_PCIE_PS_L3_BBP_IO_WRITE8((pAd), Offset, Value, TRUE);
			else
				RTMP_BBP_IO_WRITE8((pAd), Offset, Value, TRUE);
	}
}

VOID RTMP_BBP_IO_WRITE8(
	IN PRTMP_ADAPTER pAd,
	IN UCHAR Offset,
	IN UINT8 Value,
	IN BOOLEAN FlgValidMCR)
{
	BBP_CSR_CFG_STRUC  BbpCsr;
	int busyCnt=0, regID;

	regID = ((FlgValidMCR) == TRUE ? H2M_BBP_AGENT : BBP_CSR_CFG);
	for (busyCnt=0; busyCnt<MAX_BUSY_COUNT; busyCnt++)
	{
		RTMP_IO_READ32((pAd), regID, &BbpCsr.word);
		if (BbpCsr.field.Busy == BUSY)
			continue;
		BbpCsr.word = 0;
		BbpCsr.field.fRead = 0;
		BbpCsr.field.BBP_RW_MODE = 1;
		BbpCsr.field.Busy = 1;
		BbpCsr.field.Value = Value;
		BbpCsr.field.RegNum = Offset;
		RTMP_IO_WRITE32((pAd), regID, BbpCsr.word);
		if ((FlgValidMCR) == TRUE)
		{
			AsicSendCommandToMcu(pAd, 0x80, 0xff, 0x0, 0x0);
			if ((pAd)->OpMode == OPMODE_AP)
				RTMPusecDelay(1000);
		}
		(pAd)->BbpWriteLatch[Offset] = Value;
		break;
	}
	if (busyCnt == MAX_BUSY_COUNT)
	{
		DBGPRINT_ERR(("BBP write R%d fail\n", Offset));
		if((FlgValidMCR) == TRUE)
		{
			RTMP_IO_READ32(pAd, H2M_BBP_AGENT, &BbpCsr.word);
			BbpCsr.field.Busy = 0;
			RTMP_IO_WRITE32(pAd, H2M_BBP_AGENT, BbpCsr.word);
		}
	}
}
#endif // VENDOR_FEATURE3_SUPPORT //

#ifdef WSC_AP_SUPPORT
VOID WPSLedPeriodicExec(
	IN PVOID SystemSpecific1, 
	IN PVOID FunctionContext, 
	IN PVOID SystemSpecific2, 
	IN PVOID SystemSpecific3) 
{
	PRTMP_ADAPTER	pAd = (RTMP_ADAPTER *)FunctionContext;
	static UCHAR wpscnt=0;
	
	if(wpscnt==5){
		wpsLedChk(pAd);
		wpscnt=0;
	}
	wpscnt ++;
}
#endif

#ifdef RT33xx
VOID RTMP_TxEvmCalibration(
	IN PRTMP_ADAPTER pAd)
{
	UCHAR RegIdx = 0, RegValue = 0;
	USHORT value = 0;

	value = 0xFFFF;
	RT28xx_EEPROM_READ16(pAd, 0x120, value); // RF R9
	if ((value != 0xFFFF) && (value != 0))
	{		
		RegIdx = (UCHAR)(value >> 8);
		RegValue  = (UCHAR)(value & 0xFF);

		if (RegIdx == RF_R09)
		{
			RT30xxWriteRFRegister(pAd, RF_R09, RegValue);

			DBGPRINT(RT_DEBUG_TRACE, ("%s: Configure the initial RF values in the per-NIC principle, if any, RF_R09 = 0x%02X\n", 
				__FUNCTION__, 
				RegValue));
		}
	}

	value = 0xFFFF;
	RT28xx_EEPROM_READ16(pAd, 0x122, value); // RF R19
	if ((value != 0xFFFF) && (value != 0))
	{
		RegIdx = (UCHAR)(value >> 8);
		RegValue  = (UCHAR)(value & 0xFF);

		if (RegIdx == RF_R19)
		{
			RT30xxWriteRFRegister(pAd, RF_R19, RegValue);

			DBGPRINT(RT_DEBUG_TRACE, ("%s: Configure the initial RF values in the per-NIC principle, if any, RF_R19 = 0x%02X\n", 
				__FUNCTION__, 
				RegValue));
		}
	}

	if(pAd->CommonCfg.PhyMode == PHY_11B)
	{
		value = 0xFFFF;
		RT28xx_EEPROM_READ16(pAd, 0x126, value); // RF R21
		if ((value != 0xFFFF) && (value != 0))
		{
			RegIdx = (UCHAR)(value >> 8);
			RegValue  = (UCHAR)(value & 0xFF);

			if (RegIdx == RF_R21)
			{
				RT30xxWriteRFRegister(pAd, RF_R21, RegValue); // NIC always configures as a HT-capable NIC or legacy NIC (not CCK only)

				DBGPRINT(RT_DEBUG_TRACE, ("%s: Configure the initial RF values in the per-NIC principle, if any, RF_R21 = 0x%02X\n", 
					__FUNCTION__, 
					RegValue));
			}
		}

		value = 0xFFFF;
		RT28xx_EEPROM_READ16(pAd, 0x12a, value); // RF R29
		if ((value != 0xFFFF) && (value != 0))
		{
			RegIdx = (UCHAR)(value >> 8);
			RegValue  = (UCHAR)(value & 0xFF);

			if (RegIdx == RF_R29)
			{
				RT30xxWriteRFRegister(pAd, RF_R29, RegValue); // NIC always configures as a HT-capable NIC or legacy NIC (not CCK only)

				DBGPRINT(RT_DEBUG_TRACE, ("%s: Configure the initial RF values in the per-NIC principle, if any, RF_R29 = 0x%02X\n", 
					__FUNCTION__, 
					RegValue));
			}
		}
	}	
	else
	{
		value = 0xFFFF;
		RT28xx_EEPROM_READ16(pAd, 0x124, value); // RF R21
		if ((value != 0xFFFF) && (value != 0))
		{			
			RegIdx = (UCHAR)(value >> 8);
			RegValue  = (UCHAR)(value & 0xFF);

			if (RegIdx == RF_R21)
			{
				RT30xxWriteRFRegister(pAd, RF_R21, RegValue); // NIC always configures as a HT-capable NIC or legacy NIC (not CCK only)

				DBGPRINT(RT_DEBUG_TRACE, ("%s: Configure the initial RF values in the per-NIC principle, if any, RF_R21 = 0x%02X\n", 
					__FUNCTION__, 
					RegValue));
			}
		}

		value = 0xFFFF;
		RT28xx_EEPROM_READ16(pAd, 0x128, value); // RF R29
		if ((value != 0xFFFF) && (value != 0))
		{			
			RegIdx = (UCHAR)(value >> 8);
			RegValue  = (UCHAR)(value & 0xFF);

			if (RegIdx == RF_R29)
			{
				RT30xxWriteRFRegister(pAd, RF_R29, RegValue); // NIC always configures as a HT-capable NIC or legacy NIC (not CCK only)

				DBGPRINT(RT_DEBUG_TRACE, ("%s: Configure the initial RF values in the per-NIC principle, if any, RF_R29 = 0x%02X\n", 
					__FUNCTION__, 
					RegValue));
			}
		}
	}
}
#endif // RT33xx //
