/****************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 * (c) Copyright 2002, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ****************************************************************************

    Module Name:
	cmm_cfg.c

    Abstract:
    Ralink WiFi Driver configuration related subroutines

    Revision History:
    Who          When          What
    ---------    ----------    ----------------------------------------------
*/



#include "rt_config.h"


char* GetPhyMode(
	int Mode)
{
	switch(Mode)
	{
		case MODE_CCK:
			return "CCK";

		case MODE_OFDM:
			return "OFDM";
#ifdef DOT11_N_SUPPORT
		case MODE_HTMIX:
			return "HTMIX";

		case MODE_HTGREENFIELD:
			return "GREEN";
#endif // DOT11_N_SUPPORT //
		default:
			return "N/A";
	}
}


char* GetBW(
	int BW)
{
	switch(BW)
	{
		case BW_10:
			return "10M";

		case BW_20:
			return "20M";
#ifdef DOT11_N_SUPPORT
		case BW_40:
			return "40M";
#endif // DOT11_N_SUPPORT //
		default:
			return "N/A";
	}
}


/* 
    ==========================================================================
    Description:
        Set Country Region to pAd->CommonCfg.CountryRegion.
        This command will not work, if the field of CountryRegion in eeprom is programmed.
        
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT RT_CfgSetCountryRegion(
	IN PRTMP_ADAPTER	pAd, 
	IN PSTRING			arg,
	IN INT				band)
{
	LONG region;
	UCHAR *pCountryRegion;
	
	region = simple_strtol(arg, 0, 10);

	if (band == BAND_24G)
		pCountryRegion = &pAd->CommonCfg.CountryRegion;
	else
		pCountryRegion = &pAd->CommonCfg.CountryRegionForABand;
	
	// TODO: Is it neccesay for following check???
	// Country can be set only when EEPROM not programmed
	if (*pCountryRegion & 0x80)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("CfgSetCountryRegion():CountryRegion in eeprom was programmed\n"));
		return FALSE;
	}

	if((region >= 0) && 
	   (((band == BAND_24G) && ((region <= REGION_MAXIMUM_BG_BAND) || (region == REGION_31_BG_BAND))) || 
	    ((band == BAND_5G) && (region <= REGION_MAXIMUM_A_BAND) ))
	  )
	{
		*pCountryRegion= (UCHAR) region;
	}
	else
	{
		DBGPRINT(RT_DEBUG_ERROR, ("CfgSetCountryRegion():region(%ld) out of range!\n", region));
		return FALSE;
	}

	return TRUE;
	
}


/* 
    ==========================================================================
    Description:
        Set Wireless Mode
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT RT_CfgSetWirelessMode(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	INT		MaxPhyMode = PHY_11G;
	LONG	WirelessMode;
	
#ifdef DOT11_N_SUPPORT
	if (!RTMP_TEST_MORE_FLAG(pAd, fRTMP_ADAPTER_DISABLE_DOT_11N))
	MaxPhyMode = PHY_11N_5G;
#endif // DOT11_N_SUPPORT //
		
	WirelessMode = simple_strtol(arg, 0, 10);
	if (WirelessMode <= MaxPhyMode)
	{
		pAd->CommonCfg.PhyMode = WirelessMode;
		pAd->CommonCfg.DesiredPhyMode = WirelessMode;
		printk("PhyMode=%x, DesiredPhyMode=%x\n",pAd->CommonCfg.PhyMode,pAd->CommonCfg.DesiredPhyMode);
		return TRUE;
	}
	printk("PhyMode=%x, DesiredPhyMode=%x\n",pAd->CommonCfg.PhyMode,pAd->CommonCfg.DesiredPhyMode);

	return FALSE;
	
}


INT RT_CfgSetShortSlot(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	LONG ShortSlot;

	ShortSlot = simple_strtol(arg, 0, 10);

	if (ShortSlot == 1)
		pAd->CommonCfg.bUseShortSlotTime = TRUE;
	else if (ShortSlot == 0)
		pAd->CommonCfg.bUseShortSlotTime = FALSE;
	else
		return FALSE;  //Invalid argument 

	return TRUE;
}


/* 
    ==========================================================================
    Description:
        Set WEP KEY base on KeyIdx
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	RT_CfgSetWepKey(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			keyString,
	IN	CIPHER_KEY		*pSharedKey,
	IN	INT				keyIdx)
{
	INT				KeyLen;
	INT				i;
	UCHAR			CipherAlg = CIPHER_NONE;
	BOOLEAN			bKeyIsHex = FALSE;

	// TODO: Shall we do memset for the original key info??
	memset(pSharedKey, 0, sizeof(CIPHER_KEY));
	KeyLen = strlen(keyString);
	switch (KeyLen)
	{
		case 5: //wep 40 Ascii type
		case 13: //wep 104 Ascii type
			bKeyIsHex = FALSE;
			pSharedKey->KeyLen = KeyLen;
			NdisMoveMemory(pSharedKey->Key, keyString, KeyLen);
			break;
			
		case 10: //wep 40 Hex type
		case 26: //wep 104 Hex type
			for(i=0; i < KeyLen; i++)
			{
				if( !isxdigit(*(keyString+i)) )
					return FALSE;  //Not Hex value;
			}
			bKeyIsHex = TRUE;
			pSharedKey->KeyLen = KeyLen/2 ;
			AtoH(keyString, pSharedKey->Key, pSharedKey->KeyLen);
			break;
			
		default: //Invalid argument 
			DBGPRINT(RT_DEBUG_TRACE, ("RT_CfgSetWepKey(keyIdx=%d):Invalid argument (arg=%s)\n", keyIdx, keyString));
			return FALSE;
	}

	pSharedKey->CipherAlg = ((KeyLen % 5) ? CIPHER_WEP128 : CIPHER_WEP64);
	DBGPRINT(RT_DEBUG_TRACE, ("RT_CfgSetWepKey:(KeyIdx=%d,type=%s, Alg=%s)\n", 
						keyIdx, (bKeyIsHex == FALSE ? "Ascii" : "Hex"), CipherName[CipherAlg]));

	return TRUE;
}


/* 
    ==========================================================================
    Description:
        Set WPA PSK key

    Arguments:
        pAdapter	Pointer to our adapter
        keyString	WPA pre-shared key string
        pHashStr	String used for password hash function
        hashStrLen	Lenght of the hash string
        pPMKBuf		Output buffer of WPAPSK key

    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT RT_CfgSetWPAPSKKey(
	IN RTMP_ADAPTER	*pAd, 
	IN PSTRING		keyString,
	IN UCHAR		*pHashStr,
	IN INT			hashStrLen,
	OUT PUCHAR		pPMKBuf)
{
	int keyLen;
	UCHAR keyMaterial[40];

	keyLen = strlen(keyString);
	if ((keyLen < 8) || (keyLen > 64))
	{
		DBGPRINT(RT_DEBUG_TRACE, ("WPAPSK Key length(%d) error, required 8 ~ 64 characters!(keyStr=%s)\n", 
									keyLen, keyString));
		return FALSE;
	}

	memset(pPMKBuf, 0, 32);
	if (keyLen == 64)
	{
	    AtoH(keyString, pPMKBuf, 32);
	}
	else
	{
	    PasswordHash(keyString, pHashStr, hashStrLen, keyMaterial);
	    NdisMoveMemory(pPMKBuf, keyMaterial, 32);		
	}

	return TRUE;
}

INT	RT_CfgSetFixedTxPhyMode(
	IN	PSTRING			arg)
{
	INT		fix_tx_mode = FIXED_TXMODE_HT;
	UINT32	value;

	if (strcmp(arg, "OFDM") == 0 || strcmp(arg, "ofdm") == 0)
	{
		fix_tx_mode = FIXED_TXMODE_OFDM;
	}	
	else if (strcmp(arg, "CCK") == 0 || strcmp(arg, "cck") == 0)
	{
	    fix_tx_mode = FIXED_TXMODE_CCK;
	}
	else if (strcmp(arg, "HT") == 0 || strcmp(arg, "ht") == 0)
	{
	    fix_tx_mode = FIXED_TXMODE_HT;
	}
	else
	{
		value = simple_strtol(arg, 0, 10);
		// 1 : CCK
		// 2 : OFDM
		// otherwise : HT
		if (value == FIXED_TXMODE_CCK || value == FIXED_TXMODE_OFDM)
			fix_tx_mode = value;	
		else
			fix_tx_mode = FIXED_TXMODE_HT;
	}

	return fix_tx_mode;
					
}	

INT	RT_CfgSetMacAddress(
	IN 	PRTMP_ADAPTER 	pAd,
	IN	PSTRING			arg)
{
	INT	i, mac_len;
	
	/* Mac address acceptable format 01:02:03:04:05:06 length 17 */
	mac_len = strlen(arg);
	if(mac_len != 17)  
	{
		DBGPRINT(RT_DEBUG_ERROR, ("%s : invalid length (%d)\n", __FUNCTION__, mac_len));
		return FALSE;
	}

	if(strcmp(arg, "00:00:00:00:00:00") == 0)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("%s : invalid mac setting \n", __FUNCTION__));
		return FALSE;
	}

	for (i = 0; i < MAC_ADDR_LEN; i++)
	{
		AtoH(arg, &pAd->CurrentAddress[i], 1);
		arg = arg + 3;
	}	

	pAd->bLocalAdminMAC = TRUE;
	return TRUE;
}

#ifdef WSC_INCLUDED
INT	RT_CfgSetWscPinCode(
	IN RTMP_ADAPTER *pAd,
	IN PSTRING		pPinCodeStr,
	OUT PWSC_CTRL   pWscControl)
{
	UINT pinCode;

	pinCode = (UINT) simple_strtol(pPinCodeStr, 0, 10); // When PinCode is 03571361, return value is 3571361.
	if (strlen(pPinCodeStr) == 4)
	{
		pWscControl->WscEnrolleePinCode = pinCode;
		pWscControl->WscEnrolleePinCodeLen = 4;
	}
	else if ( ValidateChecksum(pinCode) )
	{
		pWscControl->WscEnrolleePinCode = pinCode;
		pWscControl->WscEnrolleePinCodeLen = 8;
	}
	else
	{
		DBGPRINT(RT_DEBUG_ERROR, ("RT_CfgSetWscPinCode(): invalid Wsc PinCode (%d)\n", pinCode));
		return FALSE;
	}
	
	DBGPRINT(RT_DEBUG_TRACE, ("RT_CfgSetWscPinCode():Wsc PinCode=%d\n", pinCode));
	
	return TRUE;
	
}
#endif // WSC_INCLUDED //
