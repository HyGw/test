/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology	5th	Rd.
 * Science-based Industrial	Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2006, Ralink Technology, Inc.
 *
 * All rights reserved.	Ralink's source	code is	an unpublished work	and	the
 * use of a	copyright notice does not imply	otherwise. This	source code
 * contains	confidential trade secret material of Ralink Tech. Any attemp
 * or participation	in deciphering,	decoding, reverse engineering or in	any
 * way altering	the	source code	is stricitly prohibited, unless	the	prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

	Module Name:
	wsc.c

	Abstract:

	Revision History:
	Who			When			What
	--------	----------		----------------------------------------------
	Paul Lin	06-08-08		Initial
	JuemingChen 06-10-30        Do modifications and Add APIs for AP
*/
#include "rt_config.h"
#include "wsc_tlv.h"
#ifdef CONFIG_CRYPTO_HMACXXX
#include <linux/crypto.h>
#include <linux/scatterlist.h>
#endif //CONFIG_CRYPTO_HMAC

static UCHAR	Wsc_Personal_String[] =  "Wi-Fi Easy and Secure Key Derivation";

#define WSC_TLV_ENT(TLV_PARA) (0x00ff & TLV_PARA)
#define WSC_TLV_BYTE1(TLV_PARA) (0x000f & TLV_PARA)
#define WSC_TLV_BYTE2(TLV_PARA) (0x000f & (TLV_PARA >> 4))

// Global reusable buffer
static UCHAR	HmacData[2048];			// temp buffer to store encrypt output data
static WSC_TLV_0B wsc_tlv_0b[]=
{
	{0,0},
	/*AP Channel*/						{0x1001, 2},				//WSC_ID_AP_CHANNEL
	/*Association State*/				{0x1002, 2},				//WSC_ID_ASSOC_STATE
	/*Authentication Type*/				{0x1003, 2},				//WSC_ID_AUTH_TYPE
	/*Authentication Type Flags*/		{0x1004, 2},				//WSC_ID_AUTH_TYPE_FLAGS
	/*Authenticator*/					{0x1005, 8},				//WSC_ID_AUTHENTICATOR
	{0,0},
	{0,0},
	/*Config Methods*/					{0x1008, 2},				//WSC_ID_CONFIG_METHODS
	/*Configuration Error*/				{0x1009, 2},				//WSC_ID_CONFIG_ERROR
	/*Confirmation URL4*/				{0x100A, 0x40}, // <= 64B	WSC_ID_CONF_URL4
	/*Confirmation URL6*/				{0x100B, 0x4C}, // <= 76B	WSC_ID_CONF_URL6
	/*Connection Type*/					{0x100C, 1},				//WSC_ID_CONN_TYPE
	/*Connection Type Flags*/			{0x100D, 1},				//WSC_ID_CONN_TYPE_FLAGS
	/*Credential*/						{0x100E, 0xff},				//WSC_ID_CREDENTIAL
	/*Encryption Type*/					{0x100F, 2},				//WSC_ID_ENCR_TYPE
	/*Encryption Type Flags*/			{0x1010, 2},				//WSC_ID_ENCR_TYPE_FLAGS
	/*Device Name*/						{0x1011, 0x20}, // <= 32B	WSC_ID_DEVICE_NAME
	/*Device Password ID*/				{0x1012, 2},				//WSC_ID_DEVICE_PWD_ID
	{0,0},
	/*E-Hash1*/							{0x1014, 32},				//WSC_ID_E_HASH1
	/*E-Hash2*/							{0x1015, 32},				//WSC_ID_E_HASH2
	/*E-SNonce1*/						{0x1016, 16},				//WSC_ID_E_SNONCE1
	/*E-SNonce2*/						{0x1017, 16},				//WSC_ID_E_SNONCE2
	/*Encrypted Settings*/				{0x1018, 0xff},	// *		//WSC_ID_ENCR_SETTINGS
	{0,0},
	/*Enrollee Nonce*/					{0x101A, 16},				//WSC_ID_ENROLLEE_NONCE
	/*Feature_ID*/						{0x101B, 4},				//WSC_ID_FEATURE_ID
	/*Identity*/						{0x101C, 0x50}, // <= 80B	WSC_ID_IDENTITY
	{0,0},															//WSC_ID_IDENTITY_PROOF
	/*Key Wrap Authenticator*/			{0x101E, 8},				//WSC_ID_KEY_WRAP_AUTH
	/*Key Identifier*/					{0x101F, 16},				//WSC_ID_KEY_IDENTIFIER
	/*MAC Address*/						{0x1020, 6},				//WSC_ID_MAC_ADDR
	/*Manufacturer*/					{0x1021, 0x40}, // <= 64B	WSC_ID_MANUFACTURER
	/*Message Type*/					{0x1022, 1},				//WSC_ID_MSG_TYPE
	/*Model Name*/						{0x1023, 0x20}, // <= 32B	WSC_ID_MODEL_NAME
	/*Model Number*/					{0x1024, 0x20}, // <= 32B	WSC_ID_MODEL_NUMBER
	{0,0},
	/*Network Index*/					{0x1026, 1},				//WSC_ID_NW_INDEX
	/*Network Key*/						{0x1027, 0x40}, // <= 64B	WSC_ID_NW_KEY
	/*Network Key Index*/				{0x1028, 1},				//WSC_ID_NW_KEY_INDEX
	/*New Device Name*/					{0x1029, 0x20}, // <= 32B	WSC_ID_NEW_DEVICE_NAME
	/*New Password*/					{0x102A, 0x40}, // <= 64B	WSC_ID_NEW_PWD
	{0,0},
	/*OOB Device Password*/				{0x102C, 0x3A}, // <= 58B	WSC_ID_OOB_DEV_PWD
	/*OS Version*/						{0x102D, 4},				//WSC_ID_OS_VERSION
	{0,0},
	/*Power Level*/						{0x102F, 1},				//WSC_ID_POWER_LEVEL
	/*PSK Current*/						{0x1030, 1},				//WSC_ID_PSK_CURRENT
	/*PSK Max*/							{0x1031, 1},				//WSC_ID_PSK_MAX
	/*Public Key*/						{0x1032, 192},				//WSC_ID_PUBLIC_KEY
	/*Radio Enabled*/					{0x1033, 1},				//WSC_ID_RADIO_ENABLED
	/*Reboot*/							{0x1034, 1},				//WSC_ID_REBOOT
	/*Registrar Current*/				{0x1035, 1},				//WSC_ID_REGISTRAR_CURRENT
	/*Registrar Established*/			{0x1036, 1},				//WSC_ID_REGISTRAR_ESTBLSHD
	{0,0},															//WSC_ID_REGISTRAR_LIST
	/*Registrar Max*/					{0x1038, 1},				//WSC_ID_REGISTRAR_MAX
	/*Registrar Nonce*/					{0x1039, 16},				//WSC_ID_REGISTRAR_NONCE
	/*Request Type*/					{0x103A, 1},				//WSC_ID_REQ_TYPE
	/*Response Type*/					{0x103B, 1},				//WSC_ID_RESP_TYPE
	/*RF Bands*/						{0x103C, 1},				//WSC_ID_RF_BAND
	/*R-Hash1*/							{0x103D, 32},				//WSC_ID_R_HASH1
	/*R-Hash2*/							{0x103E, 32},				//WSC_ID_R_HASH2
	/*R-SNonce1*/						{0x103F, 16},				//WSC_ID_R_SNONCE1
	/*R-SNonce2*/						{0x1040, 16},				//WSC_ID_R_SNONCE2
	/*Selected Registrar*/				{0x1041, 1},				//WSC_ID_SEL_REGISTRAR
	/*Serial Number*/					{0x1042, 0x20}, // <= 32B	WSC_ID_SERIAL_NUM
	{0,0},
	/*Simple Config State*/				{0x1044, 1},				//WSC_ID_SC_STATE
	/*SSID*/							{0x1045, 0x20}, // <= 32B	WSC_ID_SSID
	/*Total Networks*/					{0x1046, 1},				//WSC_ID_TOT_NETWORKS
	/*UUID-E*/							{0x1047, 16},				//WSC_ID_UUID_E
	/*UUID-R*/							{0x1048, 16},				//WSC_ID_UUID_R
	{0,0},															//WSC_ID_VENDOR_EXT
	/*Version*/							{0x104A, 1},				//WSC_ID_VERSION
	/*X.509 Certificate Request*/		{0x104B, 0xff},				//WSC_ID_X509_CERT_REQ
	/*X.509 Certificate*/				{0x104C, 0xff},				//WSC_ID_X509_CERT
	/*EAP Identity*/					{0x104D, 0x40}, // <= 64B	WSC_ID_EAP_IDENTITY
	/*Message Counter*/					{0x104E, 8},				//WSC_ID_MSG_COUNTER
	/*Public Key Hash*/					{0x104F, 20},				//WSC_ID_PUBKEY_HASH
	/*Rekey Key*/						{0x1050, 32},				//WSC_ID_REKEY_KEY
	/*Key Lifetime*/					{0x1051, 4},				//WSC_ID_KEY_LIFETIME
	/*Permitted Config Methods*/		{0x1052, 2},				//WSC_ID_PERM_CFG_METHODS
	/*Selected Registrar Config Method*/{0x1053, 2},				//WSC_ID_SEL_REG_CFG_METHODS
	/*Primary Device Type*/				{0x1054, 8},				//WSC_ID_PRIM_DEV_TYPE
	{0,0},															//WSC_ID_SEC_DEV_TYPE_LIST
	/*Portable Device*/					{0x1056, 1},				//WSC_ID_PORTABLE_DEVICE
	/*AP Setup Locked*/					{0x1057, 1},				//WSC_ID_AP_SETUP_LOCKED
	{0,0},															//WSC_ID_APP_LIST
	/*EAP Type*/						{0x1059, 0x08}, // <= 8B	WSC_ID_EAP_TYPE
	{0,0},
	{0,0},
	{0,0},
	{0,0},
	{0,0},
	{0,0},
	/*Initialization Vector*/			{0x1060, 32},				//WSC_ID_INIT_VECTOR
	/*Key Provided Automatically*/		{0x1061, 1},				//WSC_ID_KEY_PROVIDED_AUTO
	/*802.1X Enabled*/					{0x1062, 1},				//WSC_ID_8021X_ENABLED
	{0,0},															//
	/*WEPTransmitKey*/					{0x1064, 1},				//WSC_ID_WEPTRANSMITKEY
	/*<Reserved for WFA> 0x1065 V 0x1FFF*/
	/*<Unavailable> 0x000 V 0x0FFF,0x2000 V 0xFFFF*/
};

void InitializeWSCTLV(void)
{
	INT					loopi = 0;

	for(loopi = 0; loopi < WSC_TLV_ENT(WSC_ID_WEPTRANSMITKEY); loopi++)
	{
		wsc_tlv_0b[loopi].tag = cpu2be16(wsc_tlv_0b[loopi].tag);
		wsc_tlv_0b[loopi].len = cpu2be16(wsc_tlv_0b[loopi].len);
	}
	return;
}

int AppendWSCTLV(USHORT index, OUT UCHAR * obuf, IN UCHAR * ibuf, IN u16 varlen)
{
	int len = 0;

	if((u16)0 < varlen && (u16)255 >= varlen)
	{
		wsc_tlv_0b[WSC_TLV_ENT(index)].len = cpu2be16(varlen);
	}
	memcpy(obuf, &wsc_tlv_0b[WSC_TLV_ENT(index)], 4);
	len = cpu2be16(wsc_tlv_0b[WSC_TLV_ENT(index)].len);
	memcpy(obuf + 4, ibuf, len);
	len += 4;
	return len;
}

/*
	========================================================================
	
	Routine Description:
		Process elements encryption settings

	Arguments:
		pAdapter    - NIC Adapter pointer
		
	Return Value:
		None

	IRQL = DISPATCH_LEVEL
	
	Note:
		1. Recv M4 M5 M6 M7
		
	========================================================================
*/
static VOID	WscParseEncrSettings(
	IN	PRTMP_ADAPTER		pAdapter, 
	IN	PUCHAR				pPlainData,
	IN	INT					PlainLength,
// For AP Client support WPS Modification
	IN  PWSC_CTRL           pWscControl)
// --
{
//	UCHAR	apidx = MAIN_MBSSID;
// For AP Client support WPS Modification
//	PWSC_REG_DATA		pReg = (PWSC_REG_DATA) &pAdapter->PortCfg.MBSSID[apidx].WscControl.RegData;
	PWSC_REG_DATA		pReg = (PWSC_REG_DATA) &pWscControl->RegData;
// --
	USHORT	WscType, WscLen, HmacLen;
	PUCHAR	pData;
	UCHAR	Hmac[8], Temp[32];

	HmacLen = (USHORT)(PlainLength - 12);
	pData  = pPlainData;

	// Start to process WSC IEs
	while (PlainLength > 4)
	{
		WSC_TLV_0B	TLV_Encr;
		memcpy((u8 *)&TLV_Encr, pData, 4);
		WscType = cpu2be16(TLV_Encr.tag);
		WscLen  = cpu2be16(TLV_Encr.len);
		pData  += 4;
		PlainLength -= 4;

		// Parse M2 WSC type and store to RegData structure
		switch (WscType)
		{
			case WSC_ID_E_SNONCE1:
				// for verification with our enrollee nonce
				NdisMoveMemory(pReg->Es1, pData, WscLen);
				break;

			case WSC_ID_E_SNONCE2:
				// for verification with our enrollee nonce
				NdisMoveMemory(pReg->Es2, pData, WscLen);
				break;

			case WSC_ID_R_SNONCE1:
				// for verification with our enrollee nonce
				NdisMoveMemory(pReg->Rs1, pData, WscLen);
				break;

			case WSC_ID_R_SNONCE2:
				// for verification with our enrollee nonce
				NdisMoveMemory(pReg->Rs2, pData, WscLen);
				break;

			case WSC_ID_KEY_WRAP_AUTH:
				NdisMoveMemory(Hmac, pData, WscLen);
				break;

			default:
				DBGPRINT(RT_DEBUG_TRACE, "WscParseEncrSettings --> Unknown IE 0x%04x\n", WscType);
				break;
		}

		// Offset to net WSC Ie
		pData  += WscLen;
		PlainLength -= WscLen;
	}
	// Validate HMAC, reuse KDK buffer
	hmac_sha(pReg->AuthKey, 32, pPlainData, HmacLen, Temp, 32);
	
	if (RTMPEqualMemory(Hmac, Temp, 8) != 1)
	{
		DBGPRINT(RT_DEBUG_ERROR, "WscParseEncrSettings --> HMAC not match\n");
		DBGPRINT(RT_DEBUG_TRACE, "MD --> 0x%08x-%08x\n", (ULONG )cpu2be32(*((PULONG ) &Temp[0])), (ULONG )cpu2be32(*((PULONG ) &Temp[4])));
		DBGPRINT(RT_DEBUG_TRACE, "calculated --> 0x%08x-%08x\n", (ULONG )cpu2be32(*((PULONG ) &Hmac[0])), (ULONG )cpu2be32(*((PULONG ) &Hmac[4])));
	}
}

/*
	========================================================================
	
	Routine Description:
		Process credentials within AP encryption settings

	Arguments:
		pAdapter    - NIC Adapter pointer
		
	Return Value:
		None

	IRQL = DISPATCH_LEVEL
	
	Note:
		1. Recv M8
		
	========================================================================
*/
static VOID	WscProcessCredential(
	IN	PRTMP_ADAPTER		pAdapter, 
	IN	PUCHAR				pPlainData,
	IN	INT					PlainLength,
// For AP Client support WPS Modification
	IN  PWSC_CTRL           pWscControl)
// --
{
	USHORT			WscType, WscLen, Cnt, CurrentIdx=0, Idx;
	PUCHAR			pData, pTmp;
	PWSC_PROFILE	pProfile;
//	UCHAR			apidx = MAIN_MBSSID;

	pData  = pPlainData;

	// Cleanup Old contents
	NdisZeroMemory(&pWscControl->WscProfile, sizeof(WSC_PROFILE));
	
	pProfile = (PWSC_PROFILE) &pWscControl->WscProfile;
	
	// Init Profile number
	Cnt = 0;
	
	// Start to process WSC IEs within credential
	while (PlainLength > 4)
	{
		WSC_TLV_0B	TLV_Recv;
		memcpy((u8 *)&TLV_Recv, pData, 4);
		WscType = cpu2be16(TLV_Recv.tag);
		WscLen  = cpu2be16(TLV_Recv.len);
		pData  += 4;
		PlainLength -= 4;

		// Parse M2 WSC type and store to RegData structure
		switch (WscType)
		{
			case WSC_ID_NW_INDEX:
				// A new profile, add the cnt and save to database
				CurrentIdx = Cnt;	// since the index start from 0, we have to minus 1
				Cnt++;				
				break;

			case WSC_ID_SSID:
				// Find the exact length of SSID without null terminator
				pTmp = pData;
				for (Idx = 0; Idx < WscLen; Idx++)
				{
					if (*(pTmp++) == 0x0)
						break;
				}
				pProfile->Profile[CurrentIdx].SSID.SsidLength = Idx;
				NdisMoveMemory(pProfile->Profile[CurrentIdx].SSID.Ssid, pData, pProfile->Profile[CurrentIdx].SSID.SsidLength);
				break;
								
			case WSC_ID_AUTH_TYPE:
				pProfile->Profile[CurrentIdx].AuthType = cpu2be16(*((PUSHORT) pData));//(u8 *)&pReg->RegistrarInfo.AuthTypeFlags
				break;
								
			case WSC_ID_ENCR_TYPE:
				pProfile->Profile[CurrentIdx].EncrType = cpu2be16(*((PUSHORT) pData));//(u8 *)&pReg->RegistrarInfo.EncrTypeFlags
				break;

			case WSC_ID_NW_KEY_INDEX:
				// Our ket index start from 0 internally
				pProfile->Profile[CurrentIdx].KeyIndex = (*pData) - 1;
				break;
				
			case WSC_ID_NW_KEY:
				pProfile->Profile[CurrentIdx].KeyLength = WscLen;
				NdisMoveMemory(pProfile->Profile[CurrentIdx].Key, pData, pProfile->Profile[CurrentIdx].KeyLength);
				break;
				
			case WSC_ID_MAC_ADDR:
//				if (RTMPCompareMemory(pAdapter->PortCfg.WscControl.RegData.EnrolleeInfo.MacAddr, pData, 6) != 0)
				{
					DBGPRINT(RT_DEBUG_TRACE, "Rx M8 Compare enrollee MacAddr mismatched \n");
				}
				break;
				
			case WSC_ID_KEY_WRAP_AUTH:
				// Not used here, since we already verify it at decryption
				break;

			case WSC_ID_CREDENTIAL:
				// Credential IE, The WscLen include all length within profile, we need to modify it
				// to be able to parse all profile fields
				WscLen = 0;
				break;

			default:
				DBGPRINT(RT_DEBUG_TRACE, "WscProcessCredential --> Unknown IE 0x%04x\n", WscType);
				break;
		}

		// Offset to net WSC Ie
		pData  += WscLen;
		PlainLength -= WscLen;
	}

	// Svae the total number
	pProfile->ProfileCnt = Cnt;
	DBGPRINT(RT_DEBUG_TRACE, "WscProcessCredential --> %d profile retrieved from credential\n", Cnt);
}

/*
	========================================================================
	
	Routine Description:
		Build WSC M1 Message

	Arguments:
		pAdapter    - NIC Adapter pointer
		pbuf		- rewrite buffer
		
	Return Value:
		None

	IRQL = DISPATCH_LEVEL
	
	Note:
		Actions after Rx EAP-Req(ID)
		1. Change the correct parameters
		2. Build M1
		
	========================================================================
*/
int BuildMessageM1(
	IN	PRTMP_ADAPTER		pAdapter,
// For AP Client support WPS Modification
	IN  PWSC_CTRL           pWscControl,
// --
	OUT	VOID *pbuf)
{
	UCHAR				TB[1];
	INT					Len = 0, templen = 0, DH_Len;
	PUCHAR				pData = (PUCHAR)pbuf;
//	UCHAR				apidx = MAIN_MBSSID;
// For AP Client support WPS Modification
//	PWSC_REG_DATA		pReg = (PWSC_REG_DATA) &pAdapter->PortCfg.MBSSID[apidx].WscControl.RegData;
	PWSC_REG_DATA		pReg = (PWSC_REG_DATA) &pWscControl->RegData;
// --

	// <<Atheros JumpStart>> ++, add by johnli
//	GenerateDHPublicKey(pAdapter->PortCfg.MBSSID[apidx].WscControl.pPubKeyMem, pReg->EnrolleeRandom, 192, pReg->Pke, &DH_Len);
// For AP Client support WPS Modification
	//GenerateDHPublicKey(NULL, NULL, 0, pReg->Pke, &DH_Len);
	if (pWscControl->RegData.ReComputePke == 1)
	{
		GenerateDHPublicKey(NULL, NULL, 0, pReg->Pke, &DH_Len);
		pWscControl->RegData.ReComputePke = 0;
	}
// --
	//RTMPusecDelay(6000000); // testing by johnli, delay 6 sec to simulate low power cpu
	// <<Atheros JumpStart>> --
	// 1. Version
	templen = AppendWSCTLV(WSC_ID_VERSION, pData, &pReg->EnrolleeInfo.Version, 0);
	pData += templen;
	Len   += templen;

	// 2. Message Type, M1
	TB[0] = WSC_ID_MESSAGE_M1;
	templen = AppendWSCTLV(WSC_ID_MSG_TYPE, pData, TB, 0);
	pData += templen;
	Len   += templen;

	// 3. UUID_E, last 6 bytes use MAC
	templen = AppendWSCTLV(WSC_ID_UUID_E, pData, pReg->EnrolleeInfo.Uuid, 0);
	pData += templen;
	Len   += templen;

	// 4. MAC address
	templen = AppendWSCTLV(WSC_ID_MAC_ADDR, pData, pReg->EnrolleeInfo.MacAddr, 0);
	pData += templen;
	Len   += templen;

	// 5. Enrollee Nonce, first generate and save to Wsc Control Block
	templen = AppendWSCTLV(WSC_ID_ENROLLEE_NONCE, pData, pReg->EnrolleeNonce, 0);
	pData += templen;
	Len   += templen;

	// 6. Public Key, 192 bytes
	templen = AppendWSCTLV(WSC_ID_PUBLIC_KEY, pData, pReg->Pke, 0);
	pData += templen;
	Len   += templen;

	// 7. Authentication Type Flags
	templen = AppendWSCTLV(WSC_ID_AUTH_TYPE_FLAGS, pData, (u8 *)&pReg->EnrolleeInfo.AuthTypeFlags, 0);
	pData += templen;
	Len   += templen;
	
	// 8. Encryption Type Flags
	templen = AppendWSCTLV(WSC_ID_ENCR_TYPE_FLAGS, pData, (u8 *)&pReg->EnrolleeInfo.EncrTypeFlags, 0);
	pData += templen;
	Len   += templen;
	
	// 9. Connection Type Flag ESS
	templen = AppendWSCTLV(WSC_ID_CONN_TYPE_FLAGS, pData, (u8 *)&pReg->EnrolleeInfo.ConnTypeFlags, 0);
	pData += templen;
	Len   += templen;

	//10. Config Method
	// TODO: Might need to differ between PBC and PIN mode (Label, Display, PBC)
	templen = AppendWSCTLV(WSC_ID_CONFIG_METHODS, pData, (u8 *)&pReg->EnrolleeInfo.ConfigMethods, 0);
	pData += templen;
	Len   += templen;
	
	//11. Simple Config State (Not Configured)
	templen = AppendWSCTLV(WSC_ID_SC_STATE, pData, (u8 *)&pReg->EnrolleeInfo.ScState, 0);
	pData += templen;
	Len   += templen;

	//12. Manufacture
	templen = AppendWSCTLV(WSC_ID_MANUFACTURER, pData, pReg->EnrolleeInfo.Manufacturer, strlen(pReg->EnrolleeInfo.Manufacturer));
	pData += templen;
	Len   += templen;

	//13. Model Name
	templen = AppendWSCTLV(WSC_ID_MODEL_NAME, pData, pReg->EnrolleeInfo.ModelName, strlen(pReg->EnrolleeInfo.ModelName));
	pData += templen;
	Len   += templen;
	
	//14. Model Number
	templen = AppendWSCTLV(WSC_ID_MODEL_NUMBER, pData, pReg->EnrolleeInfo.ModelNumber, strlen(pReg->EnrolleeInfo.ModelNumber));
	pData += templen;
	Len   += templen;
	
	//15. Serial Number
	templen = AppendWSCTLV(WSC_ID_SERIAL_NUM, pData, pReg->EnrolleeInfo.SerialNumber, strlen(pReg->EnrolleeInfo.SerialNumber));
	pData += templen;
	Len   += templen;
	
	//16. Primary Device Type
	templen = AppendWSCTLV(WSC_ID_PRIM_DEV_TYPE, pData, pReg->EnrolleeInfo.PriDeviceType, 0);
	pData += templen;
	Len   += templen;
	
	//17. Device Name
	templen = AppendWSCTLV(WSC_ID_DEVICE_NAME, pData, pReg->EnrolleeInfo.DeviceName, strlen(pReg->EnrolleeInfo.DeviceName));
	pData += templen;
	Len   += templen;
	
	//18. RF Band
	templen = AppendWSCTLV(WSC_ID_RF_BAND, pData, (u8 *)&pReg->EnrolleeInfo.RfBand, 0);
	pData += templen;
	Len   += templen;

	//19. Associate state (Not associated)
	templen = AppendWSCTLV(WSC_ID_ASSOC_STATE, pData, (u8 *)&pReg->EnrolleeInfo.AssocState, 0);
	pData += templen;
	Len   += templen;

	//20. Device Password ID
	templen = AppendWSCTLV(WSC_ID_DEVICE_PWD_ID, pData, (u8 *)&pReg->EnrolleeInfo.DevPwdId, 0);
	pData += templen;
	Len   += templen;

	//21. Configure Error
	templen = AppendWSCTLV(WSC_ID_CONFIG_ERROR, pData, (u8 *)&pReg->EnrolleeInfo.ConfigError, 0);
	pData += templen;
	Len   += templen;

	//22. OS Version Not associated)
	templen = AppendWSCTLV(WSC_ID_OS_VERSION, pData, (u8 *)&pReg->EnrolleeInfo.OsVersion, 0);
	pData += templen;
	Len   += templen;

	// <<WCN vista logo>> ++, add by johnli
	// Fixed WCN vista logo 2 registrar test item issue.
	// Also prevent that WCN GetDeviceInfo disturbs EAP processing.
	if (pWscControl->WscUPnPNodeInfo.bUPnPMsgTimerRunning ||
        (pWscControl->WscUseUPnP && pWscControl->EapMsgRunning))
		;
	else
	{
	// <<WCN vista logo>> --
    		// Copy the content to Regdata for lasttx information
    		pReg->LastTx.Length = Len;
    		NdisMoveMemory(pReg->LastTx.Data, pbuf, Len);
	// <<WCN vista logo>> ++, add by johnli
	}
	// <<WCN vista logo>> --
	
	DBGPRINT(RT_DEBUG_TRACE, "BuildMessageM1 : \n");
	return Len;
}

/*
	========================================================================
	
	Routine Description:
		Build WSC M2 Message

	Arguments:
		pAdapter    - NIC Adapter pointer
		pbuf		- rewrite buffer
		
	Return Value:
		None

	IRQL = DISPATCH_LEVEL
	
	Note:
		Actions after M1
		1. Change the correct parameters
		2. Build M2
		
	========================================================================
*/
int BuildMessageM2(
	IN	PRTMP_ADAPTER		pAdapter,
// For AP Client support WPS Modification
	IN  PWSC_CTRL           pWscControl,
// --
	OUT	VOID *pbuf)
{
	UCHAR				TB[1];
	INT					Len = 0, templen = 0;
	PUCHAR				pData = (PUCHAR)pbuf, pAuth;
//	UCHAR				apidx = MAIN_MBSSID;
// For AP Client support WPS Modification
//	PWSC_REG_DATA		pReg = (PWSC_REG_DATA) &pAdapter->PortCfg.MBSSID[apidx].WscControl.RegData;
	PWSC_REG_DATA		pReg = (PWSC_REG_DATA) &pWscControl->RegData;
// --
	UCHAR				DHKey[32], KDK[32], KdkInput[38], KdfKey[80];
	INT					DH_Len;
	ULONG				HmacLen;

//	GenerateDHSecreteKey(pAdapter->PortCfg.MBSSID[apidx].WscControl.pSecKeyMem, &pReg->EnrolleeRandom[0], 192, pReg->Pke, 192, &pReg->SecretKey[0], &DH_Len);
	GenerateDHSecreteKey(NULL, NULL, 0, pReg->Pke, 192, &pReg->SecretKey[0], &DH_Len);  // edit by johnli
	sha256(&DHKey[0], &pReg->SecretKey[0], 192);

	// 1. Version
	templen = AppendWSCTLV(WSC_ID_VERSION, pData, &pReg->RegistrarInfo.Version, 0);
	pData += templen;
	Len   += templen;

	// 2. Message Type, M2
	TB[0] = WSC_ID_MESSAGE_M2;
	templen = AppendWSCTLV(WSC_ID_MSG_TYPE, pData, TB, 0);
	pData += templen;
	Len   += templen;

	// 3. Enrollee Nonce, 16 bytes
	templen = AppendWSCTLV(WSC_ID_ENROLLEE_NONCE, pData, pReg->EnrolleeNonce, 0);
	pData += templen;
	Len   += templen;

	// 4. Registrar Nonce, 16 bytes
	templen = AppendWSCTLV(WSC_ID_REGISTRAR_NONCE, pData, pReg->RegistrarNonce, 0);
	pData += templen;
	Len   += templen;

	// 5.
	templen = AppendWSCTLV(WSC_ID_UUID_R, pData, pReg->RegistrarInfo.Uuid, 0);
	pData += templen;
	Len   += templen;

	// 6.
	templen = AppendWSCTLV(WSC_ID_PUBLIC_KEY, pData, pReg->Pkr, 0);
	pData += templen;
	Len   += templen;

	// 7. Authentication Type Flags
	templen = AppendWSCTLV(WSC_ID_AUTH_TYPE_FLAGS, pData, (u8 *)&pReg->RegistrarInfo.AuthTypeFlags, 0);
	pData += templen;
	Len   += templen;

	// 8.
	templen = AppendWSCTLV(WSC_ID_ENCR_TYPE_FLAGS, pData, (u8 *)&pReg->RegistrarInfo.EncrTypeFlags, 0);
	pData += templen;
	Len   += templen;

	// 9.
	templen = AppendWSCTLV(WSC_ID_CONN_TYPE_FLAGS, pData, (u8 *)&pReg->RegistrarInfo.ConnTypeFlags, 0);
	pData += templen;
	Len   += templen;

	// 10.
	templen = AppendWSCTLV(WSC_ID_CONFIG_METHODS, pData, (u8 *)&pReg->RegistrarInfo.ConfigMethods, 0);
	pData += templen;
	Len   += templen;

	// 11.
	templen = AppendWSCTLV(WSC_ID_MANUFACTURER, pData, pReg->EnrolleeInfo.Manufacturer, strlen(pReg->EnrolleeInfo.Manufacturer));
	pData += templen;
	Len   += templen;

	// 12.
	templen = AppendWSCTLV(WSC_ID_MODEL_NAME, pData, pReg->EnrolleeInfo.ModelName, strlen(pReg->EnrolleeInfo.ModelName));
	pData += templen;
	Len   += templen;

	// 13.
	templen = AppendWSCTLV(WSC_ID_MODEL_NUMBER, pData, pReg->EnrolleeInfo.ModelNumber, strlen(pReg->EnrolleeInfo.ModelNumber));
	pData += templen;
	Len   += templen;

	// 14.
	templen = AppendWSCTLV(WSC_ID_SERIAL_NUM, pData, pReg->EnrolleeInfo.SerialNumber, strlen(pReg->EnrolleeInfo.SerialNumber));
	pData += templen;
	Len   += templen;

	// 15.
	templen = AppendWSCTLV(WSC_ID_PRIM_DEV_TYPE, pData, pReg->EnrolleeInfo.PriDeviceType, 0);
	pData += templen;
	Len   += templen;

	// 16.
	templen = AppendWSCTLV(WSC_ID_DEVICE_NAME, pData, pReg->EnrolleeInfo.DeviceName, strlen(pReg->EnrolleeInfo.DeviceName));
	pData += templen;
	Len   += templen;

	// 17. RF Band
	templen = AppendWSCTLV(WSC_ID_RF_BAND, pData, (u8 *)&pReg->EnrolleeInfo.RfBand, 0);
	pData += templen;
	Len   += templen;

	// 18.
	templen = AppendWSCTLV(WSC_ID_ASSOC_STATE, pData, (u8 *)&pReg->EnrolleeInfo.AssocState, 0);
	pData += templen;
	Len   += templen;

	// 19.
	templen = AppendWSCTLV(WSC_ID_CONFIG_ERROR, pData, (u8 *)&pReg->EnrolleeInfo.ConfigError, 0);
	pData += templen;
	Len   += templen;

	// 20. Device Password ID
	templen = AppendWSCTLV(WSC_ID_DEVICE_PWD_ID, pData, (u8 *)&pReg->EnrolleeInfo.DevPwdId, 0);
	pData += templen;
	Len   += templen;

	// 21.
	templen = AppendWSCTLV(WSC_ID_OS_VERSION, pData, (u8 *)&pReg->EnrolleeInfo.OsVersion, 0);
	pData += templen;
	Len   += templen;

	// Create KDK input data
	NdisMoveMemory(&KdkInput[0], &pReg->EnrolleeNonce[0], 16);
	NdisMoveMemory(&KdkInput[16], &pReg->EnrolleeInfo.MacAddr[0], 6);
	NdisMoveMemory(&KdkInput[22], pReg->RegistrarNonce, 16);
	
	// Generate the KDK
	hmac_sha(DHKey, 32,  KdkInput, 38, KDK, 32);				
	
	// KDF
	WscDeriveKey(KDK, 32, Wsc_Personal_String, (sizeof(Wsc_Personal_String) - 1), KdfKey, 640);

	// Assign Key from KDF
	NdisMoveMemory(pReg->AuthKey, &KdfKey[0], 32);
	NdisMoveMemory(pReg->KeyWrapKey, &KdfKey[32], 16);
	NdisMoveMemory(pReg->Emsk, &KdfKey[48], 32);
	
	// Combine last TX & RX message contents and validate the HMAC
	// We have to exclude last 12 bytes from last receive since it's authenticator value
	HmacLen = Len + pReg->LastRx.Length;
	pAuth = (PUCHAR) &HmacData[0];
	NdisMoveMemory(pAuth, pReg->LastRx.Data, pReg->LastRx.Length);
	pAuth += pReg->LastRx.Length;
	NdisMoveMemory(pAuth, pbuf, Len);

	// Validate HMAC, reuse KDK buffer
	hmac_sha(pReg->AuthKey, 32, HmacData, HmacLen, KDK, 32);
	
	// 22. Hmac
	templen = AppendWSCTLV(WSC_ID_AUTHENTICATOR, pData, KDK, 0);
	pData += templen;
	Len   += templen;

	// Copy the content to Regdata for lasttx information
	pReg->LastTx.Length = Len;
	NdisMoveMemory(pReg->LastTx.Data, pbuf, Len);

	DBGPRINT(RT_DEBUG_TRACE, "BuildMessageM2 : \n");
	return Len;
}

/*
	========================================================================
	
	Routine Description:
		Build WSC M2D Message

	Arguments:
		pAdapter    - NIC Adapter pointer
		pbuf		- rewrite buffer
		
	Return Value:
		None

	IRQL = DISPATCH_LEVEL
	
	Note:
		Actions after M1 process Error
		1. Change the correct parameters
		2. Build M2D
		
	========================================================================
*/
int BuildMessageM2D(
	IN	PRTMP_ADAPTER		pAdapter,
// For AP Client support WPS Modification
	IN  PWSC_CTRL           pWscControl,
// --
	OUT	VOID *pbuf)
{
	UCHAR				TB[1];
	INT					Len = 0, templen = 0;
	PUCHAR				pData = (PUCHAR)pbuf;
//	UCHAR				apidx = MAIN_MBSSID;
// For AP Client support WPS Modification
//	PWSC_REG_DATA		pReg = (PWSC_REG_DATA) &pAdapter->PortCfg.MBSSID[apidx].WscControl.RegData;
	PWSC_REG_DATA		pReg = (PWSC_REG_DATA) &pWscControl->RegData;
// --

	// 1. Version
	templen = AppendWSCTLV(WSC_ID_VERSION, pData, &pReg->RegistrarInfo.Version, 0);
	pData += templen;
	Len   += templen;

	// 2. Message Type, M2D
	TB[0] = WSC_ID_MESSAGE_M2D;
	templen = AppendWSCTLV(WSC_ID_MSG_TYPE, pData, TB, 0);
	pData += templen;
	Len   += templen;

	// 3. Enrollee Nonce, 16 bytes
	templen = AppendWSCTLV(WSC_ID_ENROLLEE_NONCE, pData, pReg->EnrolleeNonce, 0);
	pData += templen;
	Len   += templen;

	// 4. Registrar Nonce, 16 bytes
	templen = AppendWSCTLV(WSC_ID_REGISTRAR_NONCE, pData, pReg->RegistrarNonce, 0);
	pData += templen;
	Len   += templen;

	// 5.
	templen = AppendWSCTLV(WSC_ID_UUID_R, pData, pReg->RegistrarInfo.Uuid, 0);
	pData += templen;
	Len   += templen;

	// 6.WSC_ID_PUBLIC_KEY

	// 7. Authentication Type Flags
	templen = AppendWSCTLV(WSC_ID_AUTH_TYPE_FLAGS, pData, (u8 *)&pReg->RegistrarInfo.AuthTypeFlags, 0);
	pData += templen;
	Len   += templen;

	// 8.
	templen = AppendWSCTLV(WSC_ID_ENCR_TYPE_FLAGS, pData, (u8 *)&pReg->RegistrarInfo.EncrTypeFlags, 0);
	pData += templen;
	Len   += templen;

	// 9.
	templen = AppendWSCTLV(WSC_ID_CONN_TYPE_FLAGS, pData, (u8 *)&pReg->RegistrarInfo.ConnTypeFlags, 0);
	pData += templen;
	Len   += templen;

	// 10.
	templen = AppendWSCTLV(WSC_ID_CONFIG_METHODS, pData, (u8 *)&pReg->RegistrarInfo.ConfigMethods, 0);
	pData += templen;
	Len   += templen;

	// 11.
	templen = AppendWSCTLV(WSC_ID_MANUFACTURER, pData, pReg->EnrolleeInfo.Manufacturer, strlen(pReg->EnrolleeInfo.Manufacturer));
	pData += templen;
	Len   += templen;

	// 12.
	templen = AppendWSCTLV(WSC_ID_MODEL_NAME, pData, pReg->EnrolleeInfo.ModelName, strlen(pReg->EnrolleeInfo.ModelName));
	pData += templen;
	Len   += templen;

	// 13.
	templen = AppendWSCTLV(WSC_ID_MODEL_NUMBER, pData, pReg->EnrolleeInfo.ModelNumber, strlen(pReg->EnrolleeInfo.ModelNumber));
	pData += templen;
	Len   += templen;

	// 14.
	templen = AppendWSCTLV(WSC_ID_SERIAL_NUM, pData, pReg->EnrolleeInfo.SerialNumber, strlen(pReg->EnrolleeInfo.SerialNumber));
	pData += templen;
	Len   += templen;

	// 15.
	templen = AppendWSCTLV(WSC_ID_PRIM_DEV_TYPE, pData, pReg->EnrolleeInfo.PriDeviceType, 0);
	pData += templen;
	Len   += templen;

	// 16.
	templen = AppendWSCTLV(WSC_ID_DEVICE_NAME, pData, pReg->EnrolleeInfo.DeviceName, strlen(pReg->EnrolleeInfo.DeviceName));
	pData += templen;
	Len   += templen;

	// 17. RF Band
	templen = AppendWSCTLV(WSC_ID_RF_BAND, pData, (u8 *)&pReg->EnrolleeInfo.RfBand, 0);
	pData += templen;
	Len   += templen;

	// 18.
	templen = AppendWSCTLV(WSC_ID_ASSOC_STATE, pData, (u8 *)&pReg->EnrolleeInfo.AssocState, 0);
	pData += templen;
	Len   += templen;

	// 19.
	templen = AppendWSCTLV(WSC_ID_CONFIG_ERROR, pData, (u8 *)&pReg->EnrolleeInfo.ConfigError, 0);
	pData += templen;
	Len   += templen;

	// 20.WSC_ID_DEVICE_PWD_ID

	// 21.
	templen = AppendWSCTLV(WSC_ID_OS_VERSION, pData, (u8 *)&pReg->EnrolleeInfo.OsVersion, 0);
	pData += templen;
	Len   += templen;

    DBGPRINT(RT_DEBUG_TRACE, "BuildMessageM2D : \n");
	return Len;
}

/*
	========================================================================
	
	Routine Description:
		Build WSC M3 Message

	Arguments:
		pAdapter    - NIC Adapter pointer
		pbuf		- rewrite buffer
		
	Return Value:
		None

	IRQL = DISPATCH_LEVEL
	
	Note:
		Actions after M2
		1. Change the correct parameters
		2. Build M3
		
	========================================================================
*/
int BuildMessageM3(
	IN	PRTMP_ADAPTER		pAdapter,
// For AP Client support WPS Modification
	IN  PWSC_CTRL           pWscControl,
// --
	OUT	VOID *pbuf)
{
	UCHAR				TB[32];
	INT					Len = 0, templen = 0;
	PUCHAR				pData = (PUCHAR)pbuf, pAuth;
//	UCHAR				apidx = MAIN_MBSSID;
// For AP Client support WPS Modification
//	PWSC_REG_DATA		pReg = (PWSC_REG_DATA) &pAdapter->PortCfg.MBSSID[apidx].WscControl.RegData;
	PWSC_REG_DATA		pReg = (PWSC_REG_DATA) &pWscControl->RegData;
// --
	ULONG				HmacLen;
	UCHAR				*pHash=NULL;
	
	pHash = kmalloc(512, MEM_ALLOC_FLAG);
	if(NULL == pHash)
		return Len;

	// 1. Version
	templen = AppendWSCTLV(WSC_ID_VERSION, pData, &pReg->EnrolleeInfo.Version, 0);
	pData += templen;
	Len   += templen;

	// 2. Message Type, M3
	TB[0] = WSC_ID_MESSAGE_M3;
	templen = AppendWSCTLV(WSC_ID_MSG_TYPE, pData, TB, 0);
	pData += templen;
	Len   += templen;

	// 3. Registrar Nonce, 16 bytes
	templen = AppendWSCTLV(WSC_ID_REGISTRAR_NONCE, pData, pReg->RegistrarNonce, 0);
	pData += templen;
	Len   += templen;

	// 4. E-Hash1
	//
	// Generate PSK1
	hmac_sha(pReg->AuthKey, 32, pReg->PIN, 4, TB, 32);

	// Copy first 16 bytes to PSK1
	NdisMoveMemory(pReg->Psk1, TB, 16);

	// Create input for E-Hash1
	NdisMoveMemory(pHash, pReg->Es1, 16);
	NdisMoveMemory(pHash + 16, pReg->Psk1, 16);
	NdisMoveMemory(pHash + 32, pReg->Pke, 192);
	NdisMoveMemory(pHash + 224, pReg->Pkr, 192);
	
	// Generate E-Hash1
	hmac_sha(pReg->AuthKey, 32, pHash, 416, pReg->EHash1, 32);
	
	templen = AppendWSCTLV(WSC_ID_E_HASH1, pData, pReg->EHash1, 0);
	pData += templen;
	Len   += templen;

	// 5. E-Hash2
	//
	// Generate PSK2
	hmac_sha(pReg->AuthKey, 32, &pReg->PIN[4], 4, TB, 32);

	// Copy first 16 bytes to PSK2
	NdisMoveMemory(pReg->Psk2, TB, 16);
		
	// Create input for E-Hash2
	NdisMoveMemory(pHash, pReg->Es2, 16);
	NdisMoveMemory(pHash + 16, pReg->Psk2, 16);
	NdisMoveMemory(pHash + 32, pReg->Pke, 192);
	NdisMoveMemory(pHash + 224, pReg->Pkr, 192);
	
	// Generate E-Hash2
	hmac_sha(pReg->AuthKey, 32, pHash, 416, pReg->EHash2, 32);
	
	templen = AppendWSCTLV(WSC_ID_E_HASH2, pData, pReg->EHash2, 0);
	pData += templen;
	Len   += templen;

	// 6. Generate authenticator
	// Combine last TX & RX message contents and validate the HMAC
	HmacLen = Len + pReg->LastRx.Length;
	pAuth = (PUCHAR) HmacData;
	NdisMoveMemory(pAuth, pReg->LastRx.Data, pReg->LastRx.Length);
	pAuth += pReg->LastRx.Length;
	NdisMoveMemory(pAuth, pbuf, Len);

	// Validate HMAC, reuse KDK buffer
	hmac_sha(pReg->AuthKey, 32, HmacData, HmacLen, TB, 32);

	templen = AppendWSCTLV(WSC_ID_AUTHENTICATOR, pData, TB, 0);
	pData += templen;
	Len   += templen;

	// Copy the content to Regdata for lasttx information
	pReg->LastTx.Length = Len;
	NdisMoveMemory(pReg->LastTx.Data, pbuf, Len);

	if(NULL != pHash)
		kfree(pHash);
	DBGPRINT(RT_DEBUG_TRACE, "BuildMessageM3 : \n");
	return Len;
}

/*
	========================================================================
	
	Routine Description:
		Build WSC M4 Message

	Arguments:
		pAdapter    - NIC Adapter pointer
		pbuf		- rewrite buffer
		
	Return Value:
		None

	IRQL = DISPATCH_LEVEL
	
	Note:
		Actions after M3
		1. Change the correct parameters
		2. Build M4
		
	========================================================================
*/
int BuildMessageM4(
	IN	PRTMP_ADAPTER		pAdapter,
// For AP Client support WPS Modification
	IN  PWSC_CTRL           pWscControl,
// --
	OUT	VOID *pbuf)
{
	UCHAR				TB[32];
	INT					Len = 0, templen = 0;
	PUCHAR				pData = (PUCHAR)pbuf, pAuth;
//	UCHAR				apidx = MAIN_MBSSID;
// For AP Client support WPS Modification
//	PWSC_REG_DATA		pReg = (PWSC_REG_DATA) &pAdapter->PortCfg.MBSSID[apidx].WscControl.RegData;
	PWSC_REG_DATA		pReg = (PWSC_REG_DATA) &pWscControl->RegData;
// --
	ULONG				HmacLen;
	UCHAR				KDK[32];
	UCHAR				Plain[128], IV_EncrData[144];//IV len 16,EncrData len 128
	INT					PlainLen = 0, EncrLen;
	UCHAR				*pHash=NULL;
	
	pHash = kmalloc(512, MEM_ALLOC_FLAG);
	if(NULL == pHash)
		return Len;
	// 1. Version
	templen = AppendWSCTLV(WSC_ID_VERSION, pData, &pReg->RegistrarInfo.Version, 0);
	pData += templen;
	Len   += templen;

	// 2. Message Type, M4
	TB[0] = WSC_ID_MESSAGE_M4;
	templen = AppendWSCTLV(WSC_ID_MSG_TYPE, pData, TB, 0);
	pData += templen;
	Len   += templen;

	// 3. Enrollee Nonce, 16 bytes
	templen = AppendWSCTLV(WSC_ID_ENROLLEE_NONCE, pData, pReg->EnrolleeNonce, 0);
	pData += templen;
	Len   += templen;

	// 4. R-Hash1
	//
	// Generate PSK1
	hmac_sha(pReg->AuthKey, 32, pReg->PIN, 4, TB, 32);

	// Copy first 16 bytes to PSK1
	NdisMoveMemory(pReg->Psk1, TB, 16);

	// Create input for R-Hash1
	NdisMoveMemory(pHash, pReg->Es1, 16);
	NdisMoveMemory(pHash + 16, pReg->Psk1, 16);
	NdisMoveMemory(pHash + 32, pReg->Pke, 192);
	NdisMoveMemory(pHash + 224, pReg->Pkr, 192);
	
	// Generate R-Hash1
	hmac_sha(pReg->AuthKey, 32, pHash, 416, pReg->RHash1, 32);
	
	templen = AppendWSCTLV(WSC_ID_R_HASH1, pData, pReg->RHash1, 0);
	pData += templen;
	Len   += templen;

	// 5. R-Hash2
	//
	// Generate PSK2
	hmac_sha(pReg->AuthKey, 32, &pReg->PIN[4], 4, TB, 32);

	// Copy first 16 bytes to PSK2
	NdisMoveMemory(pReg->Psk2, TB, 16);
		
	// Create input for R-Hash2
	NdisMoveMemory(pHash, pReg->Es2, 16);
	NdisMoveMemory(pHash + 16, pReg->Psk2, 16);
	NdisMoveMemory(pHash + 32, pReg->Pke, 192);
	NdisMoveMemory(pHash + 224, pReg->Pkr, 192);
	
	// Generate R-Hash2
	hmac_sha(pReg->AuthKey, 32, pHash, 416, pReg->RHash2, 32);
	
	templen = AppendWSCTLV(WSC_ID_R_HASH2, pData, pReg->RHash2, 0);
	pData += templen;
	Len   += templen;

	// 6a. Encrypted R-S1
	//    Prepare plain text
	PlainLen += AppendWSCTLV(WSC_ID_R_SNONCE1, &Plain[0], pReg->Es1, 0);

	// Generate HMAC
	hmac_sha(pReg->AuthKey, 32, &Plain[0], PlainLen, TB, 32);
	PlainLen += AppendWSCTLV(WSC_ID_KEY_WRAP_AUTH, &Plain[PlainLen], TB, 0);

	// 6b. Encrypted Settings
	// Encrypt data
	WscEncryptData(Plain, PlainLen, pReg->KeyWrapKey, &IV_EncrData[0], &IV_EncrData[16], &EncrLen);
	templen = AppendWSCTLV(WSC_ID_ENCR_SETTINGS, pData, &IV_EncrData[0], 16 + EncrLen);//IVLen + EncrLen
	pData += templen;
	Len   += templen;

	// 7.
	// Combine last TX & RX message contents and validate the HMAC
	// We have to exclude last 12 bytes from last receive since it's authenticator value
	HmacLen = Len + pReg->LastRx.Length;
	pAuth = (PUCHAR) HmacData;
	NdisMoveMemory(pAuth, pReg->LastRx.Data, pReg->LastRx.Length);
	pAuth += pReg->LastRx.Length;
	NdisMoveMemory(pAuth, pbuf, Len);

	// Validate HMAC, reuse KDK buffer
	hmac_sha(pReg->AuthKey, 32, HmacData, HmacLen, KDK, 32);

	templen = AppendWSCTLV(WSC_ID_AUTHENTICATOR, pData, KDK, 0);
	pData += templen;
	Len   += templen;

	// Copy the content to Regdata for lasttx information
	pReg->LastTx.Length = Len;
	NdisMoveMemory(pReg->LastTx.Data, pbuf, Len);

	if(NULL != pHash)
		kfree(pHash);
	DBGPRINT(RT_DEBUG_TRACE, "BuildMessageM4 : \n");
	return Len;
}

/*
	========================================================================
	
	Routine Description:
		Build WSC M5 Message

	Arguments:
		pAdapter    - NIC Adapter pointer
		pbuf		- rewrite buffer
		
	Return Value:
		None

	IRQL = DISPATCH_LEVEL
	
	Note:
		Actions after M4
		1. Change the correct parameters
		2. Build M5
		
	========================================================================
*/
int BuildMessageM5(
	IN	PRTMP_ADAPTER		pAdapter,
// For AP Client support WPS Modification
	IN  PWSC_CTRL           pWscControl,
// --
	OUT	VOID *pbuf)
{
	UCHAR				TB[32];
	INT					Len = 0, templen = 0;
	PUCHAR				pData = (PUCHAR)pbuf;
//	UCHAR				apidx = MAIN_MBSSID;
// For AP Client support WPS Modification
//	PWSC_REG_DATA		pReg = (PWSC_REG_DATA) &pAdapter->PortCfg.MBSSID[apidx].WscControl.RegData;
	PWSC_REG_DATA		pReg = (PWSC_REG_DATA) &pWscControl->RegData;
// --
	PUCHAR				pAuth;
	ULONG				HmacLen;
	UCHAR				Plain[128], IV_EncrData[144];//IV len 16,EncrData len 128
	INT					PlainLen=0, EncrLen;

	// 1. Version
	templen = AppendWSCTLV(WSC_ID_VERSION, pData, &pReg->EnrolleeInfo.Version, 0);
	pData += templen;
	Len   += templen;

	// 2. Message Type, M5
	TB[0] = WSC_ID_MESSAGE_M5;
	templen = AppendWSCTLV(WSC_ID_MSG_TYPE, pData, TB, 0);
	pData += templen;
	Len   += templen;

	// 3. Registrar Nonce, 16 bytes
	templen = AppendWSCTLV(WSC_ID_REGISTRAR_NONCE, pData, pReg->RegistrarNonce, 0);
	pData += templen;
	Len   += templen;

	// 4a. Encrypted E-S1
	//    Prepare plain text
	PlainLen += AppendWSCTLV(WSC_ID_E_SNONCE1, &Plain[0], pReg->Es1, 0);

	// Generate HMAC
	hmac_sha(pReg->AuthKey, 32, &Plain[0], PlainLen, TB, 32);
	PlainLen += AppendWSCTLV(WSC_ID_KEY_WRAP_AUTH, &Plain[PlainLen], TB, 0);

	// 4b. Encrypted Settings
	// Encrypt data
	WscEncryptData(Plain, PlainLen, pReg->KeyWrapKey, &IV_EncrData[0], &IV_EncrData[16], &EncrLen);
	templen = AppendWSCTLV(WSC_ID_ENCR_SETTINGS, pData, &IV_EncrData[0], 16 + EncrLen);//IVLen + EncrLen
	pData += templen;
	Len   += templen;

	// 5. Generate authenticator
	// Combine last TX & RX message contents and validate the HMAC
	HmacLen = Len + pReg->LastRx.Length;
	pAuth = (PUCHAR) HmacData;
	NdisMoveMemory(pAuth, pReg->LastRx.Data, pReg->LastRx.Length);
	pAuth += pReg->LastRx.Length;
	NdisMoveMemory(pAuth, pbuf, Len);

	// Validate HMAC, reuse KDK buffer
	hmac_sha(pReg->AuthKey, 32, HmacData, HmacLen, TB, 32);

	templen = AppendWSCTLV(WSC_ID_AUTHENTICATOR, pData, TB, 0);
	pData += templen;
	Len   += templen;

	// Copy the content to Regdata for lasttx information
	pReg->LastTx.Length = Len;
	NdisMoveMemory(pReg->LastTx.Data, pbuf, Len);

	DBGPRINT(RT_DEBUG_TRACE, "BuildMessageM5 : \n");
	return Len;
}

/*
	========================================================================
	
	Routine Description:
		Build WSC M6 Message

	Arguments:
		pAdapter    - NIC Adapter pointer
		pbuf		- rewrite buffer
		
	Return Value:
		None

	IRQL = DISPATCH_LEVEL
	
	Note:
		Actions after M5
		1. Change the correct parameters
		2. Build M6
		
	========================================================================
*/
int BuildMessageM6(
	IN	PRTMP_ADAPTER		pAdapter,
// For AP Client support WPS Modification
	IN  PWSC_CTRL           pWscControl,
// --
	OUT	VOID *pbuf)
{
	UCHAR				TB[32];
	INT					Len = 0, templen = 0;
	PUCHAR				pData = (PUCHAR)pbuf, pAuth;
//	UCHAR				apidx = MAIN_MBSSID;
// For AP Client support WPS Modification
//	PWSC_REG_DATA		pReg = (PWSC_REG_DATA) &pAdapter->PortCfg.MBSSID[apidx].WscControl.RegData;
	PWSC_REG_DATA		pReg = (PWSC_REG_DATA) &pWscControl->RegData;
// --
	ULONG				HmacLen;
	UCHAR				KDK[32];
	UCHAR				Plain[128], IV_EncrData[144];//IV len 16,EncrData len 128
	INT					PlainLen = 0, EncrLen;

	// 1. Version
	templen = AppendWSCTLV(WSC_ID_VERSION, pData, &pReg->RegistrarInfo.Version, 0);
	pData += templen;
	Len   += templen;

	// 2. Message Type, M6
	TB[0] = WSC_ID_MESSAGE_M6;
	templen = AppendWSCTLV(WSC_ID_MSG_TYPE, pData, TB, 0);
	pData += templen;
	Len   += templen;

	// 3. Enrollee Nonce, 16 bytes
	templen = AppendWSCTLV(WSC_ID_ENROLLEE_NONCE, pData, pReg->EnrolleeNonce, 0);
	pData += templen;
	Len   += templen;

	// 4a. Encrypted R-S2
	//    Prepare plain text
	PlainLen += AppendWSCTLV(WSC_ID_R_SNONCE2, &Plain[0], pReg->Es2, 0);

	// Generate HMAC
	hmac_sha(pReg->AuthKey, 32, &Plain[0], PlainLen, TB, 32);
	PlainLen += AppendWSCTLV(WSC_ID_KEY_WRAP_AUTH, &Plain[PlainLen], TB, 0);

	// 4b. Encrypted Settings
	// Encrypt data
	WscEncryptData(Plain, PlainLen, pReg->KeyWrapKey, &IV_EncrData[0], &IV_EncrData[16], &EncrLen);
	templen = AppendWSCTLV(WSC_ID_ENCR_SETTINGS, pData, &IV_EncrData[0], 16 + EncrLen);//IVLen + EncrLen
	pData += templen;
	Len   += templen;

	// 5.
	// Combine last TX & RX message contents and validate the HMAC
	// We have to exclude last 12 bytes from last receive since it's authenticator value
	HmacLen = Len + pReg->LastRx.Length;
	pAuth = (PUCHAR) HmacData;
	NdisMoveMemory(pAuth, pReg->LastRx.Data, pReg->LastRx.Length);
	pAuth += pReg->LastRx.Length;
	NdisMoveMemory(pAuth, pbuf, Len);

	// Validate HMAC, reuse KDK buffer
	hmac_sha(pReg->AuthKey, 32, HmacData, HmacLen, KDK, 32);

	templen = AppendWSCTLV(WSC_ID_AUTHENTICATOR, pData, KDK, 0);
	pData += templen;
	Len   += templen;

	// Copy the content to Regdata for lasttx information
	pReg->LastTx.Length = Len;
	NdisMoveMemory(pReg->LastTx.Data, pbuf, Len);

	// Copy the content to Regdata for lasttx information
	pReg->LastTx.Length = Len;
	NdisMoveMemory(pReg->LastTx.Data, pbuf, Len);

	DBGPRINT(RT_DEBUG_TRACE, "BuildMessageM6 : \n");
	return Len;
}

/*
	========================================================================
	
	Routine Description:
		Build WSC M7 Message

	Arguments:
		pAdapter    - NIC Adapter pointer
		pbuf		- rewrite buffer
		
	Return Value:
		None

	IRQL = DISPATCH_LEVEL
	
	Note:
		Actions after M6
		1. Change the correct parameters
		2. Build M7
		
	========================================================================
*/
int BuildMessageM7(
	IN	PRTMP_ADAPTER		pAdapter,
// For AP Client support WPS Modification
	IN  PWSC_CTRL           pWscControl,
// --
	OUT	VOID *pbuf)
{
	UCHAR				TB[32];
	INT					Len = 0, templen = 0;
	PUCHAR				pData = (PUCHAR)pbuf, pAuth;
	UCHAR				apidx = MAIN_MBSSID;
// For AP Client support WPS Modification
//	PWSC_REG_DATA		pReg = (PWSC_REG_DATA) &pAdapter->PortCfg.MBSSID[apidx].WscControl.RegData;
	PWSC_REG_DATA		pReg = (PWSC_REG_DATA) &pWscControl->RegData;
// --
	ULONG				HmacLen;
	UCHAR				Plain[256], *IV_EncrData=NULL;//IV len 16 ,EncrData len 
	INT					PlainLen=0, EncrLen;

	IV_EncrData = kmalloc(512, MEM_ALLOC_FLAG);
	if(NULL == IV_EncrData)
		return 0;

	// 1. Version
	templen = AppendWSCTLV(WSC_ID_VERSION, pData, &pReg->EnrolleeInfo.Version, 0);
	pData += templen;
	Len   += templen;

	// 2. Message Type, M7
	TB[0] = WSC_ID_MESSAGE_M7;
	templen = AppendWSCTLV(WSC_ID_MSG_TYPE, pData, TB, 0);
	pData += templen;
	Len   += templen;

	// 3. Registrar Nonce, 16 bytes
	templen = AppendWSCTLV(WSC_ID_REGISTRAR_NONCE, pData, pReg->RegistrarNonce, 0);
	pData += templen;
	Len   += templen;

	// 4a. Encrypted E-S2
	//    Prepare plain text
	PlainLen += AppendWSCTLV(WSC_ID_E_SNONCE2, &Plain[0], pReg->Es2, 0);
    //if(pAdapter->PortCfg.MBSSID[apidx].WscControl.WscConfStatus == WSC_SCSTATE_CONFIGURED)
    {
// For AP Client support WPS Modification
	USHORT  authType;
	USHORT  encyType;

#ifdef APCLI_SUPPORT
	if (pWscControl->EntryApIdx == MIN_NET_DEVICE_FOR_APCLI)
	{
		authType = htons(WscGetAuthType(pAdapter->ApCliTab.ApCliEntry[apidx].AuthMode));
		encyType = htons(WscGetEncryType(pAdapter->ApCliTab.ApCliEntry[apidx].PairCipher));
		PlainLen += AppendWSCTLV(WSC_ID_SSID, &Plain[PlainLen], pAdapter->ApCliTab.ApCliEntry[apidx].Ssid, pAdapter->ApCliTab.ApCliEntry[apidx].SsidLen);
	}
	else
#endif // APCLI_SUPPORT //
// --
	{
		authType = htons(WscGetAuthType(pAdapter->PortCfg.MBSSID[apidx].AuthMode));
		encyType = htons(WscGetEncryType(pAdapter->PortCfg.MBSSID[apidx].WepStatus));
		PlainLen += AppendWSCTLV(WSC_ID_SSID, &Plain[PlainLen], pAdapter->PortCfg.MBSSID[apidx].Ssid, pAdapter->PortCfg.MBSSID[apidx].SsidLen);
	}
    	PlainLen += AppendWSCTLV(WSC_ID_MAC_ADDR, &Plain[PlainLen], pReg->EnrolleeInfo.MacAddr, 0);
    	PlainLen += AppendWSCTLV(WSC_ID_AUTH_TYPE, &Plain[PlainLen], (u8 *)&authType, 0);
    	PlainLen += AppendWSCTLV(WSC_ID_ENCR_TYPE, &Plain[PlainLen], (u8 *)&encyType, 0);
        printk("\nauthType = %02x\n", authType);
        printk("\nencyType = %02x\n", encyType);
    	PlainLen += AppendWSCTLV(WSC_ID_NW_KEY_INDEX, &Plain[PlainLen], &pReg->RegistrarInfo.NewKeyIndex, 0);
    	PlainLen += AppendWSCTLV(WSC_ID_NW_KEY, &Plain[PlainLen], pReg->RegistrarInfo.NewKey, pReg->RegistrarInfo.NewKeyLen);
    }

	// Generate HMAC
	hmac_sha(pReg->AuthKey, 32, &Plain[0], PlainLen, TB, 32);
	PlainLen += AppendWSCTLV(WSC_ID_KEY_WRAP_AUTH, &Plain[PlainLen], TB, 0);

	// 4b. Encrypted Settings
	// Encrypt data
	WscEncryptData(Plain, PlainLen, pReg->KeyWrapKey, IV_EncrData, IV_EncrData + 16, &EncrLen);
	templen = AppendWSCTLV(WSC_ID_ENCR_SETTINGS, pData, IV_EncrData, 16 + EncrLen);//IVLen + EncrLen
	pData += templen;
	Len   += templen;

	// 5. Generate authenticator
	// Combine last TX & RX message contents and validate the HMAC
	HmacLen = Len + pReg->LastRx.Length;
	pAuth = (PUCHAR) HmacData;
	NdisMoveMemory(pAuth, pReg->LastRx.Data, pReg->LastRx.Length);
	pAuth += pReg->LastRx.Length;
	NdisMoveMemory(pAuth, pbuf, Len);

	// Validate HMAC, reuse KDK buffer
	hmac_sha(pReg->AuthKey, 32, HmacData, HmacLen, TB, 32);

	templen = AppendWSCTLV(WSC_ID_AUTHENTICATOR, pData, TB, 0);
	pData += templen;
	Len   += templen;

	// Copy the content to Regdata for lasttx information
	pReg->LastTx.Length = Len;
	NdisMoveMemory(pReg->LastTx.Data, pbuf, Len);
	
	if(NULL != IV_EncrData)
		kfree(IV_EncrData);
	DBGPRINT(RT_DEBUG_TRACE, "BuildMessageM7 : \n");
	return Len;
}

/*
	========================================================================
	
	Routine Description:
		Build WSC M8 Message

	Arguments:
		pAdapter    - NIC Adapter pointer
		pbuf		- rewrite buffer
		
	Return Value:
		None

	IRQL = DISPATCH_LEVEL
	
	Note:
		Actions after M7
		1. Change the correct parameters
		2. Build M8
		
	========================================================================
*/
int BuildMessageM8(
	IN	PRTMP_ADAPTER		pAdapter,
// For AP Client support WPS Modification
	IN  PWSC_CTRL           pWscControl,
// --
	OUT	VOID *pbuf)
{
	UCHAR				TB[256];
	INT					Len = 0, templen = 0;
	PUCHAR				pData = (PUCHAR)pbuf, pAuth;
//	UCHAR				apidx = MAIN_MBSSID;
// For AP Client support WPS Modification
//	PWSC_REG_DATA		pReg = (PWSC_REG_DATA) &pAdapter->PortCfg.MBSSID[apidx].WscControl.RegData;
	PWSC_REG_DATA		pReg = (PWSC_REG_DATA) &pWscControl->RegData;
// --
	ULONG				HmacLen;
	UCHAR				KDK[32];
	UCHAR				Plain[300], *IV_EncrData=NULL;//IV len 16 ,EncrData len 
	INT					CerLen = 0, PlainLen = 0, EncrLen;

	IV_EncrData = kmalloc(512, MEM_ALLOC_FLAG);
	if(NULL == IV_EncrData)
		return 0;

	// 1. Version
	templen = AppendWSCTLV(WSC_ID_VERSION, pData, &pReg->RegistrarInfo.Version, 0);
	pData += templen;
	Len   += templen;

	// 2. Message Type, M8
	TB[0] = WSC_ID_MESSAGE_M8;
	templen = AppendWSCTLV(WSC_ID_MSG_TYPE, pData, TB, 0);
	pData += templen;
	Len   += templen;

	// 3. Enrollee Nonce, 16 bytes
	templen = AppendWSCTLV(WSC_ID_ENROLLEE_NONCE, pData, pReg->EnrolleeNonce, 0);
	pData += templen;
	Len   += templen;

	// 4a. Encrypted R-S1
	CerLen += AppendWSCTLV(WSC_ID_NW_INDEX, &TB[0], "1", 0);
// For AP Client support WPS Modification
#ifdef APCLI_SUPPORT
	if (pWscControl->EntryApIdx == MIN_NET_DEVICE_FOR_APCLI)
		CerLen += AppendWSCTLV(WSC_ID_SSID, &TB[CerLen], pAdapter->ApCliTab.ApCliEntry[0].Ssid, pAdapter->ApCliTab.ApCliEntry[0].SsidLen);
	else
#endif // APCLI_SUPPORT //
// --
	CerLen += AppendWSCTLV(WSC_ID_SSID, &TB[CerLen], pAdapter->PortCfg.MBSSID[0].Ssid, pAdapter->PortCfg.MBSSID[0].SsidLen);
	// <<WCN vista logo>> ++, edit by johnli
/*
	CerLen += AppendWSCTLV(WSC_ID_AUTH_TYPE, &TB[CerLen], (u8 *)&pReg->RegistrarInfo.AuthTypeFlags, 0);
	CerLen += AppendWSCTLV(WSC_ID_ENCR_TYPE, &TB[CerLen], (u8 *)&pReg->RegistrarInfo.EncrTypeFlags, 0);
*/
    	CerLen += AppendWSCTLV(WSC_ID_AUTH_TYPE, &TB[CerLen], (u8 *)&pReg->RegistrarInfo.AuthType, 0);
    	CerLen += AppendWSCTLV(WSC_ID_ENCR_TYPE, &TB[CerLen], (u8 *)&pReg->RegistrarInfo.EncrType, 0);
	// <<WCN vista logo>> --
	CerLen += AppendWSCTLV(WSC_ID_NW_KEY_INDEX, &TB[CerLen], &pReg->RegistrarInfo.NewKeyIndex, 0);
	CerLen += AppendWSCTLV(WSC_ID_NW_KEY, &TB[CerLen], pReg->RegistrarInfo.NewKey, pReg->RegistrarInfo.NewKeyLen);
	CerLen += AppendWSCTLV(WSC_ID_MAC_ADDR, &TB[CerLen], pReg->EnrolleeInfo.MacAddr, 0);
	//    Prepare plain text
	PlainLen += AppendWSCTLV(WSC_ID_CREDENTIAL, &Plain[0], TB, CerLen);

	// Generate HMAC
	hmac_sha(pReg->AuthKey, 32, &Plain[0], PlainLen, TB, 32);
	PlainLen += AppendWSCTLV(WSC_ID_KEY_WRAP_AUTH, &Plain[PlainLen], TB, 0);

	// 4b. Encrypted Settings
	// Encrypt data
	WscEncryptData(Plain, PlainLen, pReg->KeyWrapKey, IV_EncrData, IV_EncrData + 16, &EncrLen);
	templen = AppendWSCTLV(WSC_ID_ENCR_SETTINGS, pData, IV_EncrData, 16 + EncrLen);//IVLen + EncrLen
	pData += templen;
	Len   += templen;

	// 5.
	// Combine last TX & RX message contents and validate the HMAC
	// We have to exclude last 12 bytes from last receive since it's authenticator value
	HmacLen = Len + pReg->LastRx.Length;
	pAuth = (PUCHAR) HmacData;
	NdisMoveMemory(pAuth, pReg->LastRx.Data, pReg->LastRx.Length);
	pAuth += pReg->LastRx.Length;
	NdisMoveMemory(pAuth, pbuf, Len);

	// Validate HMAC, reuse KDK buffer
	hmac_sha(pReg->AuthKey, 32, HmacData, HmacLen, KDK, 32);

	templen = AppendWSCTLV(WSC_ID_AUTHENTICATOR, pData, KDK, 0);
	pData += templen;
	Len   += templen;

	if(NULL != IV_EncrData)
		kfree(IV_EncrData);
	DBGPRINT(RT_DEBUG_TRACE, "BuildMessageM8 : \n");
	return Len;
}

int BuildMessageDONE(
	IN	PRTMP_ADAPTER		pAdapter,
// For AP Client support WPS Modification
	IN  PWSC_CTRL           pWscControl,
// --
	OUT	VOID *pbuf)
{
	UCHAR				TB[1];
	INT					Len = 0, templen = 0;
	PUCHAR				pData = (PUCHAR)pbuf;
//	UCHAR				apidx = MAIN_MBSSID;
// For AP Client support WPS Modification
//	PWSC_REG_DATA		pReg = (PWSC_REG_DATA) &pAdapter->PortCfg.MBSSID[apidx].WscControl.RegData;
	PWSC_REG_DATA		pReg = (PWSC_REG_DATA) &pWscControl->RegData;
// --

	// 1. Version
	templen = AppendWSCTLV(WSC_ID_VERSION, pData, &pReg->RegistrarInfo.Version, 0);
	pData += templen;
	Len   += templen;

	// 2. Message Type, WSC DONE
	TB[0] = WSC_MSG_WSC_DONE;
	templen = AppendWSCTLV(WSC_ID_MSG_TYPE, pData, TB, 0);
	pData += templen;
	Len   += templen;

	// 3. Enrollee Nonce, 16 bytes
	templen = AppendWSCTLV(WSC_ID_ENROLLEE_NONCE, pData, pReg->EnrolleeNonce, 0);
	pData += templen;
	Len   += templen;

	// 4. Registrar Nonce, 16 bytes
	templen = AppendWSCTLV(WSC_ID_REGISTRAR_NONCE, pData, pReg->RegistrarNonce, 0);
	pData += templen;
	Len   += templen;

	DBGPRINT(RT_DEBUG_TRACE, "BuildMessageDONE : \n");
	return Len;
}

int BuildMessageACK(
	IN	PRTMP_ADAPTER		pAdapter,
// For AP Client support WPS Modification
	IN  PWSC_CTRL           pWscControl,
// --
	OUT	VOID *pbuf)
{
	UCHAR				TB[1];
	INT					Len = 0, templen = 0;
	PUCHAR				pData = (PUCHAR)pbuf;
//	UCHAR				apidx = MAIN_MBSSID;
// For AP Client support WPS Modification
//	PWSC_REG_DATA		pReg = (PWSC_REG_DATA) &pAdapter->PortCfg.MBSSID[apidx].WscControl.RegData;
	PWSC_REG_DATA		pReg = (PWSC_REG_DATA) &pWscControl->RegData;
// --

	// 1. Version
	templen = AppendWSCTLV(WSC_ID_VERSION, pData, &pReg->RegistrarInfo.Version, 0);
	pData += templen;
	Len   += templen;

	// 2. Message Type, WSC ACK
	TB[0] = WSC_MSG_WSC_ACK;
	templen = AppendWSCTLV(WSC_ID_MSG_TYPE, pData, TB, 0);
	pData += templen;
	Len   += templen;

	// 3. Enrollee Nonce, 16 bytes
	templen = AppendWSCTLV(WSC_ID_ENROLLEE_NONCE, pData, pReg->EnrolleeNonce, 0);
	pData += templen;
	Len   += templen;

	// 4. Registrar Nonce, 16 bytes
	templen = AppendWSCTLV(WSC_ID_REGISTRAR_NONCE, pData, pReg->RegistrarNonce, 0);
	pData += templen;
	Len   += templen;

	DBGPRINT(RT_DEBUG_TRACE, "BuildMessageACK : \n");
	return Len;
}

int BuildMessageNACK(
	IN	PRTMP_ADAPTER		pAdapter,
// For AP Client support WPS Modification
	IN  PWSC_CTRL           pWscControl,
// --
	OUT	VOID *pbuf)
{
	UCHAR				TB[2];
	INT					Len = 0, templen = 0;
	PUCHAR				pData = (PUCHAR)pbuf;
// For AP Client support WPS Modification
/*
    UCHAR				apidx = MAIN_MBSSID;
	PWSC_REG_DATA		pReg = (PWSC_REG_DATA) &pAdapter->PortCfg.MBSSID[apidx].WscControl.RegData;
*/
	PWSC_REG_DATA		pReg = (PWSC_REG_DATA) &pWscControl->RegData;
// --
    USHORT              ConfigError = htons(pReg->EnrolleeInfo.ConfigError);

	// 1. Version
	templen = AppendWSCTLV(WSC_ID_VERSION, pData, &pReg->RegistrarInfo.Version, 0);
	pData += templen;
	Len   += templen;

	// 2. Message Type, WSC NACK
	TB[0] = WSC_ID_MESSAGE_NACK;
	templen = AppendWSCTLV(WSC_ID_MSG_TYPE, pData, TB, 0);
	pData += templen;
	Len   += templen;

	// 3. Enrollee Nonce, 16 bytes
	templen = AppendWSCTLV(WSC_ID_ENROLLEE_NONCE, pData, pReg->EnrolleeNonce, 0);
	pData += templen;
	Len   += templen;

	// 4. Registrar Nonce, 16 bytes
	templen = AppendWSCTLV(WSC_ID_REGISTRAR_NONCE, pData, pReg->RegistrarNonce, 0);
	pData += templen;
	Len   += templen;

	// 5. Error
	templen = AppendWSCTLV(WSC_ID_CONFIG_ERROR, pData, (u8 *)&ConfigError, 0);
	pData += templen;
	Len   += templen;

	DBGPRINT(RT_DEBUG_TRACE, "BuildMessageNACK : \n");
	return Len;
}


/*
	========================================================================
	
	Routine Description:
		Process WSC M1 Message

	Arguments:
		pAdapter    - NIC Adapter pointer
		pbuf		- recv buffer
		Length		- recv Length
		
	Return Value:
		None

	IRQL = DISPATCH_LEVEL
	
	Note:
		Actions after Rx M1
		1. Change the correct parameters
		2. Process M1
		
	========================================================================
*/
int ProcessMessageM1(
	IN	PRTMP_ADAPTER		pAdapter,
	IN	VOID *precv,
	IN	INT Length,
	OUT	PWSC_REG_DATA pReg)
{
	int					ret = WSC_ERROR_NO_ERROR;
	PUCHAR				pData = NULL;
	USHORT				WscType, WscLen, FieldCheck[7]={0,0,0,0,0,0,0};
//	INT					DH_Len;

	// <<Low Power CPU>> ++, remove by johnli
//	GenerateDHPublicKey(pAdapter->PortCfg.MBSSID[0].WscControl.pPubKeyMem, pAdapter->PortCfg.MBSSID[0].WscControl.RegData.EnrolleeRandom, 192, pAdapter->PortCfg.MBSSID[0].WscControl.RegData.Pkr, &DH_Len);
	// <<Low Power CPU>> --
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_VERSION))] |= (1 << WSC_TLV_BYTE1(WSC_ID_VERSION));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_MSG_TYPE))] |= (1 << WSC_TLV_BYTE1(WSC_ID_MSG_TYPE));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_UUID_E))] |= (1 << WSC_TLV_BYTE1(WSC_ID_UUID_E));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_MAC_ADDR))] |= (1 << WSC_TLV_BYTE1(WSC_ID_MAC_ADDR));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_ENROLLEE_NONCE))] |= (1 << WSC_TLV_BYTE1(WSC_ID_ENROLLEE_NONCE));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_PUBLIC_KEY))] |= (1 << WSC_TLV_BYTE1(WSC_ID_PUBLIC_KEY));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_AUTH_TYPE_FLAGS))] |= (1 << WSC_TLV_BYTE1(WSC_ID_AUTH_TYPE_FLAGS));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_ENCR_TYPE_FLAGS))] |= (1 << WSC_TLV_BYTE1(WSC_ID_ENCR_TYPE_FLAGS));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_CONN_TYPE_FLAGS))] |= (1 << WSC_TLV_BYTE1(WSC_ID_CONN_TYPE_FLAGS));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_CONFIG_METHODS))] |= (1 << WSC_TLV_BYTE1(WSC_ID_CONFIG_METHODS));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_SC_STATE))] |= (1 << WSC_TLV_BYTE1(WSC_ID_SC_STATE));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_MANUFACTURER))] |= (1 << WSC_TLV_BYTE1(WSC_ID_MANUFACTURER));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_MODEL_NAME))] |= (1 << WSC_TLV_BYTE1(WSC_ID_MODEL_NAME));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_MODEL_NUMBER))] |= (1 << WSC_TLV_BYTE1(WSC_ID_MODEL_NUMBER));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_SERIAL_NUM))] |= (1 << WSC_TLV_BYTE1(WSC_ID_SERIAL_NUM));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_PRIM_DEV_TYPE))] |= (1 << WSC_TLV_BYTE1(WSC_ID_PRIM_DEV_TYPE));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_DEVICE_NAME))] |= (1 << WSC_TLV_BYTE1(WSC_ID_DEVICE_NAME));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_RF_BAND))] |= (1 << WSC_TLV_BYTE1(WSC_ID_RF_BAND));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_ASSOC_STATE))] |= (1 << WSC_TLV_BYTE1(WSC_ID_ASSOC_STATE));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_CONFIG_ERROR))] |= (1 << WSC_TLV_BYTE1(WSC_ID_CONFIG_ERROR));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_DEVICE_PWD_ID))] |= (1 << WSC_TLV_BYTE1(WSC_ID_DEVICE_PWD_ID));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_OS_VERSION))] |= (1 << WSC_TLV_BYTE1(WSC_ID_OS_VERSION));
	// Copy the content to Regdata for lastRx information
	// Length must include authenticator IE size
	pReg->LastRx.Length = Length;		
	NdisMoveMemory(pReg->LastRx.Data, precv, Length);
	pData = pReg->LastRx.Data;
		
	// Start to process WSC IEs
	while (Length > 4)
	{
		WSC_TLV_0B	TLV_Recv;
		memcpy((u8 *)&TLV_Recv, pData, 4);
		WscType = cpu2be16(TLV_Recv.tag);
		WscLen  = cpu2be16(TLV_Recv.len);
		pData  += 4;
		Length -= 4;

		// Parse M1 WSC type and store to RegData structure
		switch (WscType)
		{
			case WSC_ID_VERSION:
				if(pReg->RegistrarInfo.Version != *pData)
					DBGPRINT(RT_DEBUG_ERROR, "Rx WPS Message Version mismatched %02x\n",*pData);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_VERSION))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_VERSION));
				break;
				
			case WSC_ID_MSG_TYPE:
				if(WSC_ID_MESSAGE_M1 != *pData)
					DBGPRINT(RT_DEBUG_ERROR, "Rx WPS Message Type mismatched %02x\n",*pData);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_MSG_TYPE))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_MSG_TYPE));
				break;
				
			case WSC_ID_UUID_E:
				NdisMoveMemory(pReg->EnrolleeInfo.Uuid, pData, WscLen);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_UUID_E))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_UUID_E));
				break;
				
			case WSC_ID_MAC_ADDR:
				NdisMoveMemory(pReg->EnrolleeInfo.MacAddr, pData, WscLen);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_MAC_ADDR))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_MAC_ADDR));
				break;
				
			case WSC_ID_ENROLLEE_NONCE:
				NdisMoveMemory(pReg->EnrolleeNonce, pData, WscLen);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_ENROLLEE_NONCE))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_ENROLLEE_NONCE));
				break;
				
			case WSC_ID_PUBLIC_KEY:
				// Get Enrollee Public Key
				NdisMoveMemory(pReg->Pke, pData, WscLen);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_PUBLIC_KEY))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_PUBLIC_KEY));
				break;
				
			case WSC_ID_AUTH_TYPE_FLAGS:
				pReg->EnrolleeInfo.AuthTypeFlags = *((PUSHORT) pData);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_AUTH_TYPE_FLAGS))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_AUTH_TYPE_FLAGS));
				break;
				
			case WSC_ID_ENCR_TYPE_FLAGS:
				pReg->EnrolleeInfo.EncrTypeFlags = *((PUSHORT) pData);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_ENCR_TYPE_FLAGS))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_ENCR_TYPE_FLAGS));
				break;
				
			case WSC_ID_CONN_TYPE_FLAGS:
				pReg->EnrolleeInfo.ConnTypeFlags = *pData;
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_CONN_TYPE_FLAGS))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_CONN_TYPE_FLAGS));
				break;
				
			case WSC_ID_CONFIG_METHODS:
				pReg->EnrolleeInfo.ConfigMethods = *((PUSHORT) pData);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_CONFIG_METHODS))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_CONFIG_METHODS));
				break;
				
			case WSC_ID_SC_STATE:
				pReg->EnrolleeInfo.ScState = *((PUSHORT) pData);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_SC_STATE))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_SC_STATE));
				break;
				
			case WSC_ID_MANUFACTURER:
				NdisMoveMemory(&pReg->EnrolleeInfo.Manufacturer, pData, WscLen);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_MANUFACTURER))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_MANUFACTURER));
				break;
				
			case WSC_ID_MODEL_NAME:
				NdisMoveMemory(&pReg->EnrolleeInfo.ModelName, pData, WscLen);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_MODEL_NAME))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_MODEL_NAME));
				break;
				
			case WSC_ID_MODEL_NUMBER:
				NdisMoveMemory(&pReg->EnrolleeInfo.ModelNumber, pData, WscLen);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_MODEL_NUMBER))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_MODEL_NUMBER));
				break;
				
			case WSC_ID_SERIAL_NUM:
				NdisMoveMemory(&pReg->EnrolleeInfo.SerialNumber, pData, WscLen);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_SERIAL_NUM))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_SERIAL_NUM));
				break;
				
			case WSC_ID_PRIM_DEV_TYPE:
				NdisMoveMemory(&pReg->EnrolleeInfo.PriDeviceType, pData, WscLen);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_PRIM_DEV_TYPE))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_PRIM_DEV_TYPE));
				break;
				
			case WSC_ID_DEVICE_NAME:
				NdisMoveMemory(&pReg->EnrolleeInfo.DeviceName, pData, WscLen);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_DEVICE_NAME))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_DEVICE_NAME));
				break;
				
			case WSC_ID_RF_BAND:
				pReg->EnrolleeInfo.RfBand = *pData;
				//if() ret = WSC_ERROR_CHAN24_NOT_SUPP;
				//if() ret = WSC_ERROR_CHAN50_NOT_SUPP;
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_RF_BAND))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_RF_BAND));
				break;
				
			case WSC_ID_ASSOC_STATE:
				pReg->EnrolleeInfo.AssocState = *((PUSHORT) pData);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_ASSOC_STATE))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_ASSOC_STATE));
				break;
				
			case WSC_ID_CONFIG_ERROR:
				pReg->EnrolleeInfo.ConfigError = *((PUSHORT) pData);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_CONFIG_ERROR))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_CONFIG_ERROR));
				break;
				
			case WSC_ID_DEVICE_PWD_ID:
				DBGPRINT(RT_DEBUG_TRACE, "   WPS Registrar DPID %04x\n",pReg->EnrolleeInfo.DevPwdId);
				if(WSC_DEVICEPWDID_DEFAULT == *(PUSHORT) pData)
					{
						DBGPRINT(RT_DEBUG_TRACE, "Rx WPS           DPID PIN\n");
					}
				else if(WSC_DEVICEPWDID_PUSH_BTN == *(PUSHORT) pData)
					{
						DBGPRINT(RT_DEBUG_TRACE, "Rx WPS           DPID PBC\n");
					}
				else
					{
						DBGPRINT(RT_DEBUG_TRACE, "Rx WPS           DPID unsupport\n");
					}
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_DEVICE_PWD_ID))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_DEVICE_PWD_ID));
				break;
				
			case WSC_ID_OS_VERSION:
				pReg->EnrolleeInfo.OsVersion = *((PULONG) pData);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_OS_VERSION))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_OS_VERSION));
				break;

			default:
				DBGPRINT(RT_DEBUG_TRACE, "ProcessMessageM1 --> Unknown IE 0x%04x\n", WscType);
				break;				
		}

		// Offset to net WSC Ie
		pData  += WscLen;
		Length -= WscLen;
	}					

	if( FieldCheck[0] || FieldCheck[1] || FieldCheck[2] || FieldCheck[3] || FieldCheck[4] || FieldCheck[5] || FieldCheck[6] )
		ret = WSC_ERROR_WANTING_FIELD;
	DBGPRINT(RT_DEBUG_TRACE, "ProcessMessageM1 : \n");
	return ret;
}

/*
	========================================================================
	
	Routine Description:
		Process WSC M2 Message

	Arguments:
		pAdapter    - NIC Adapter pointer
		pbuf		- rewrite buffer
		
	Return Value:
		None

	IRQL = DISPATCH_LEVEL
	
	Note:
		Actions after Rx M2
		1. Change the correct parameters
		2. Process M2
		
	========================================================================
*/
int ProcessMessageM2(
	IN	PRTMP_ADAPTER		pAdapter,
	IN	VOID *precv,
	IN	INT Length,
	OUT	PWSC_REG_DATA pReg)
{
	int					ret = WSC_ERROR_NO_ERROR;
	ULONG				HmacLen;
	UCHAR				Hmac[8], KDK[32];
	UCHAR				DHKey[32], KdkInput[38], KdfKey[80];
	INT					DH_Len;
	PUCHAR				pData = NULL;
	USHORT				WscType, WscLen, FieldCheck[7]={0,0,0,0,0,0,0};
	
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_VERSION))] |= (1 << WSC_TLV_BYTE1(WSC_ID_VERSION));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_MSG_TYPE))] |= (1 << WSC_TLV_BYTE1(WSC_ID_MSG_TYPE));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_ENROLLEE_NONCE))] |= (1 << WSC_TLV_BYTE1(WSC_ID_ENROLLEE_NONCE));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_REGISTRAR_NONCE))] |= (1 << WSC_TLV_BYTE1(WSC_ID_REGISTRAR_NONCE));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_UUID_R))] |= (1 << WSC_TLV_BYTE1(WSC_ID_UUID_R));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_PUBLIC_KEY))] |= (1 << WSC_TLV_BYTE1(WSC_ID_PUBLIC_KEY));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_AUTH_TYPE_FLAGS))] |= (1 << WSC_TLV_BYTE1(WSC_ID_AUTH_TYPE_FLAGS));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_ENCR_TYPE_FLAGS))] |= (1 << WSC_TLV_BYTE1(WSC_ID_ENCR_TYPE_FLAGS));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_CONN_TYPE_FLAGS))] |= (1 << WSC_TLV_BYTE1(WSC_ID_CONN_TYPE_FLAGS));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_CONFIG_METHODS))] |= (1 << WSC_TLV_BYTE1(WSC_ID_CONFIG_METHODS));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_MANUFACTURER))] |= (1 << WSC_TLV_BYTE1(WSC_ID_MANUFACTURER));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_MODEL_NAME))] |= (1 << WSC_TLV_BYTE1(WSC_ID_MODEL_NAME));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_MODEL_NUMBER))] |= (1 << WSC_TLV_BYTE1(WSC_ID_MODEL_NUMBER));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_SERIAL_NUM))] |= (1 << WSC_TLV_BYTE1(WSC_ID_SERIAL_NUM));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_PRIM_DEV_TYPE))] |= (1 << WSC_TLV_BYTE1(WSC_ID_PRIM_DEV_TYPE));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_DEVICE_NAME))] |= (1 << WSC_TLV_BYTE1(WSC_ID_DEVICE_NAME));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_RF_BAND))] |= (1 << WSC_TLV_BYTE1(WSC_ID_RF_BAND));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_ASSOC_STATE))] |= (1 << WSC_TLV_BYTE1(WSC_ID_ASSOC_STATE));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_CONFIG_ERROR))] |= (1 << WSC_TLV_BYTE1(WSC_ID_CONFIG_ERROR));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_DEVICE_PWD_ID))] |= (1 << WSC_TLV_BYTE1(WSC_ID_DEVICE_PWD_ID));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_OS_VERSION))] |= (1 << WSC_TLV_BYTE1(WSC_ID_OS_VERSION));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_AUTHENTICATOR))] |= (1 << WSC_TLV_BYTE1(WSC_ID_AUTHENTICATOR));
	// Copy the content to Regdata for lastRx information
	// Length must include authenticator IE size
	pReg->LastRx.Length = Length;		
	NdisMoveMemory(pReg->LastRx.Data, precv, Length);
	pData = pReg->LastRx.Data;
		
	// Start to process WSC IEs
	while (Length > 4)
	{
		WSC_TLV_0B	TLV_Recv;
		memcpy((u8 *)&TLV_Recv, pData, 4);
		WscType = cpu2be16(TLV_Recv.tag);
		WscLen  = cpu2be16(TLV_Recv.len);
		pData  += 4;
		Length -= 4;

		// Parse M2 WSC type and store to RegData structure
		switch (WscType)
		{
			case WSC_ID_VERSION:
				if(pReg->EnrolleeInfo.Version != *pData)
					DBGPRINT(RT_DEBUG_ERROR, "Rx WPS Message Version mismatched %02x\n",*pData);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_VERSION))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_VERSION));
				break;
				
			case WSC_ID_MSG_TYPE:
				if(WSC_ID_MESSAGE_M2 != *pData)
					DBGPRINT(RT_DEBUG_ERROR, "Rx WPS Message Type mismatched %02x\n",*pData);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_MSG_TYPE))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_MSG_TYPE));
				break;
				
			case WSC_ID_ENROLLEE_NONCE:
				// for verification with our enrollee nonce
				if (RTMPCompareMemory(pReg->EnrolleeNonce, pData, WscLen) != 0)
				{
					DBGPRINT(RT_DEBUG_TRACE, "Rx M2 Compare enrollee nonce mismatched \n");
				}
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_ENROLLEE_NONCE))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_ENROLLEE_NONCE));
				break;
				
			case WSC_ID_REGISTRAR_NONCE:
				NdisMoveMemory(pReg->RegistrarNonce, pData, WscLen);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_REGISTRAR_NONCE))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_REGISTRAR_NONCE));
				break;
				
			case WSC_ID_UUID_R:
				NdisMoveMemory(pReg->RegistrarInfo.Uuid, pData, WscLen);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_UUID_R))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_UUID_R));
				break;
				
			case WSC_ID_PUBLIC_KEY:
				// Get Registrar Public Key
				NdisMoveMemory(&pReg->Pkr, pData, WscLen);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_PUBLIC_KEY))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_PUBLIC_KEY));
				break;
				
			case WSC_ID_AUTH_TYPE_FLAGS:
				pReg->RegistrarInfo.AuthTypeFlags = *((PUSHORT) pData);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_AUTH_TYPE_FLAGS))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_AUTH_TYPE_FLAGS));
				break;
				
			case WSC_ID_ENCR_TYPE_FLAGS:
				pReg->RegistrarInfo.EncrTypeFlags = *((PUSHORT) pData);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_ENCR_TYPE_FLAGS))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_ENCR_TYPE_FLAGS));
				break;
				
			case WSC_ID_CONN_TYPE_FLAGS:
				pReg->RegistrarInfo.ConnTypeFlags = *pData;
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_CONN_TYPE_FLAGS))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_CONN_TYPE_FLAGS));
				break;
				
			case WSC_ID_CONFIG_METHODS:
				pReg->RegistrarInfo.ConfigMethods = *((PUSHORT) pData);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_CONFIG_METHODS))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_CONFIG_METHODS));
				break;
				
			case WSC_ID_MANUFACTURER:
				NdisMoveMemory(&pReg->RegistrarInfo.Manufacturer, pData, WscLen);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_MANUFACTURER))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_MANUFACTURER));
				break;
				
			case WSC_ID_MODEL_NAME:
				NdisMoveMemory(&pReg->RegistrarInfo.ModelName, pData, WscLen);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_MODEL_NAME))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_MODEL_NAME));
				break;
				
			case WSC_ID_MODEL_NUMBER:
				NdisMoveMemory(&pReg->RegistrarInfo.ModelNumber, pData, WscLen);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_MODEL_NUMBER))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_MODEL_NUMBER));
				break;
				
			case WSC_ID_SERIAL_NUM:
				NdisMoveMemory(&pReg->RegistrarInfo.SerialNumber, pData, WscLen);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_SERIAL_NUM))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_SERIAL_NUM));
				break;
				
			case WSC_ID_PRIM_DEV_TYPE:
				NdisMoveMemory(&pReg->RegistrarInfo.PriDeviceType, pData, WscLen);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_PRIM_DEV_TYPE))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_PRIM_DEV_TYPE));
				break;
				
			case WSC_ID_DEVICE_NAME:
				NdisMoveMemory(&pReg->RegistrarInfo.DeviceName, pData, WscLen);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_DEVICE_NAME))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_DEVICE_NAME));
				break;
				
			case WSC_ID_RF_BAND:
				pReg->RegistrarInfo.RfBand = *pData;
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_RF_BAND))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_RF_BAND));
				break;
				
			case WSC_ID_ASSOC_STATE:
				pReg->RegistrarInfo.AssocState = *((PUSHORT) pData);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_ASSOC_STATE))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_ASSOC_STATE));
				break;
				
			case WSC_ID_CONFIG_ERROR:
				pReg->RegistrarInfo.ConfigError = *((PUSHORT) pData);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_CONFIG_ERROR))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_CONFIG_ERROR));
				break;
				
			case WSC_ID_DEVICE_PWD_ID:
				if(pReg->EnrolleeInfo.DevPwdId != *(PUSHORT) pData)
				{
					DBGPRINT(RT_DEBUG_ERROR, "ProcessMessageM2 DPID R%d E%d\n",*(PUSHORT) pData ,pReg->EnrolleeInfo.DevPwdId);
					ret = WSC_ERROR_DEV_PWD_AUTH_FAIL;
					Length = 0;
				}
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_DEVICE_PWD_ID))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_DEVICE_PWD_ID));
				break;
				
			case WSC_ID_OS_VERSION:
				pReg->RegistrarInfo.OsVersion = *((PULONG) pData);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_OS_VERSION))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_OS_VERSION));
				break;
				
			case WSC_ID_AUTHENTICATOR:
				NdisMoveMemory(Hmac, pData, WscLen);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_AUTHENTICATOR))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_AUTHENTICATOR));
				break;

			default:
				DBGPRINT(RT_DEBUG_TRACE, "ProcessMessageM2 --> Unknown IE 0x%04x\n", WscType);
				break;				
		}

		// Offset to net WSC Ie
		pData  += WscLen;
		Length -= WscLen;
	}					

	if(WSC_ERROR_NO_ERROR != ret) 
		goto out;

	// Let's start to generate keys and verify the data payload
	// Generate Secret Key
//	GenerateDHSecreteKey(pAdapter->PortCfg.MBSSID[MAIN_MBSSID].WscControl.pSecKeyMem, &pReg->EnrolleeRandom[0], 192, pReg->Pkr, 192, &pReg->SecretKey[0], &DH_Len);
	GenerateDHSecreteKey(NULL, NULL, 0, pReg->Pkr, 192, &pReg->SecretKey[0], &DH_Len);  // edit by johnli

#ifndef CONFIG_CRYPTO_HMACXXX
	// Compute the DHKey based on the DH secret
	sha256(&DHKey[0], &pReg->SecretKey[0], 192);
#else
{
	struct crypto_tfm *tfm;
	unsigned int klen = 192;
	struct scatterlist sg[1];
	char result[64];

	sg_set_buf(&sg[0], "sha256", 6);
	tfm = crypto_alloc_tfm("sha256", 0);
	if (tfm == NULL) {
		printk("failed to load transform for sha256\n");
		return;
	}
	//crypto_tfm_alg_digestsize(tfm);
	crypto_hmac(tfm, &pReg->SecretKey[0], &klen, sg, 1, result);
}
#endif //CONFIG_CRYPTO_HMAC

	// Create KDK input data
	NdisMoveMemory(&KdkInput[0], pReg->EnrolleeNonce, 16);
		
	NdisMoveMemory(&KdkInput[16], pReg->EnrolleeInfo.MacAddr, 6);
		
	NdisMoveMemory(&KdkInput[22], pReg->RegistrarNonce, 16);
	
	// Generate the KDK
	hmac_sha(DHKey, 32,  KdkInput, 38, KDK, 32);				
	
	// KDF
	WscDeriveKey(KDK, 32, Wsc_Personal_String, (sizeof(Wsc_Personal_String) - 1), KdfKey, 640);

	// Assign Key from KDF
	NdisMoveMemory(pReg->AuthKey, &KdfKey[0], 32);
	NdisMoveMemory(pReg->KeyWrapKey, &KdfKey[32], 16);
	NdisMoveMemory(pReg->Emsk, &KdfKey[48], 32);
	
	// Combine last TX & RX message contents and validate the HMAC
	// We have to exclude last 12 bytes from last receive since it's authenticator value
	HmacLen = pReg->LastTx.Length + pReg->LastRx.Length - 12;
	NdisMoveMemory(&HmacData[0], pReg->LastTx.Data, pReg->LastTx.Length);
	NdisMoveMemory(&HmacData[pReg->LastTx.Length], pReg->LastRx.Data, pReg->LastRx.Length - 12);

	// Validate HMAC, reuse KDK buffer
	hmac_sha(pReg->AuthKey, 32, HmacData, HmacLen, KDK, 32);
	
	if (RTMPEqualMemory(Hmac, KDK, 8) != 1)
	{
		DBGPRINT(RT_DEBUG_ERROR, "ProcessMessageM2 --> HMAC not match\n");
		DBGPRINT(RT_DEBUG_TRACE, "MD --> 0x%08x-%08x\n", (ULONG )cpu2be32(*((PULONG ) KDK)), (ULONG )cpu2be32(*((PULONG )(KDK + 4))));
		DBGPRINT(RT_DEBUG_TRACE, "calculated --> 0x%08x-%08x\n", (ULONG )cpu2be32(*((PULONG ) &Hmac[0])), (ULONG )cpu2be32(*((PULONG ) &Hmac[4])));
		ret = WSC_ERROR_HMAC_FAIL;
	}
	// Update WSC status
	// pAdapter->PortCfg.WscControl.WscStatus = STATUS_WSC_EAP_M2_RECEIVED;	

	if( FieldCheck[0] || FieldCheck[1] || FieldCheck[2] || FieldCheck[3] || FieldCheck[4] || FieldCheck[5] || FieldCheck[6] )
		ret = WSC_ERROR_WANTING_FIELD;
out :
	DBGPRINT(RT_DEBUG_TRACE, "ProcessMessageM2 : \n");
	return ret;
}

/*
	========================================================================
	
	Routine Description:
		Process WSC M2D Message

	Arguments:
		pAdapter    - NIC Adapter pointer
		pbuf		- rewrite buffer
		
	Return Value:
		None

	IRQL = DISPATCH_LEVEL
	
	Note:
		Actions after Rx M2D
		1. Change the correct parameters
		2. Process M2D
		
	========================================================================
*/
int ProcessMessageM2D(
	IN	PRTMP_ADAPTER		pAdapter,
	IN	VOID *precv,
	IN	INT Length,
	OUT	PWSC_REG_DATA pReg)
{
	int					ret = WSC_ERROR_NO_ERROR;
	PUCHAR				pData = NULL;
	USHORT				WscType, WscLen;
	
	// Copy the content to Regdata for lastRx information
	// Length must include authenticator IE size
	pReg->LastRx.Length = Length;		
	NdisMoveMemory(pReg->LastRx.Data, precv, Length);
	pData = pReg->LastRx.Data;

	// Start to process WSC IEs
	while (Length > 4)
	{
		WSC_TLV_0B	TLV_Recv;
		memcpy((u8 *)&TLV_Recv, pData, 4);
		WscType = cpu2be16(TLV_Recv.tag);
		WscLen  = cpu2be16(TLV_Recv.len);
		pData  += 4;
		Length -= 4;

		// Parse M2 WSC type and store to RegData structure
		switch (WscType)
		{
			case WSC_ID_VERSION:
				if(pReg->EnrolleeInfo.Version != *pData)
					DBGPRINT(RT_DEBUG_ERROR, "Rx WPS Message Version mismatched %02x\n",*pData);
				break;
				
			case WSC_ID_MSG_TYPE:
				if(WSC_ID_MESSAGE_M2D != *pData)
					DBGPRINT(RT_DEBUG_ERROR, "Rx WPS Message Type mismatched %02x\n",*pData);
				break;
				
			case WSC_ID_ENROLLEE_NONCE:
				// for verification with our enrollee nonce
				if (RTMPCompareMemory(pReg->EnrolleeNonce, pData, WscLen) != 0)
				{
					DBGPRINT(RT_DEBUG_TRACE, "Rx M2 Compare enrollee nonce mismatched \n");
				}
				break;
				
			case WSC_ID_REGISTRAR_NONCE:
				NdisMoveMemory(pReg->RegistrarNonce, pData, WscLen);
				break;
				
			case WSC_ID_UUID_R:
				NdisMoveMemory(pReg->RegistrarInfo.Uuid, pData, WscLen);
				break;
				
			case WSC_ID_PUBLIC_KEY:
				// There shall be no Public transmitted in M2D
				DBGPRINT(RT_DEBUG_TRACE, "ProcessMessageM2D --> Receive WSC_ID_PUBLIC_KEY!! werid!\n");
				break;
				
			case WSC_ID_AUTH_TYPE_FLAGS:
				pReg->RegistrarInfo.AuthTypeFlags = *((PUSHORT) pData);
				break;
				
			case WSC_ID_ENCR_TYPE_FLAGS:
				pReg->RegistrarInfo.EncrTypeFlags = *((PUSHORT) pData);
				break;
				
			case WSC_ID_CONN_TYPE_FLAGS:
				pReg->RegistrarInfo.ConnTypeFlags = *pData;
				break;
				
			case WSC_ID_CONFIG_METHODS:
				pReg->RegistrarInfo.ConfigMethods = *((PUSHORT) pData);
				break;
				
			case WSC_ID_MANUFACTURER:
				NdisMoveMemory(&pReg->RegistrarInfo.Manufacturer, pData, WscLen);
				break;
				
			case WSC_ID_MODEL_NAME:
				NdisMoveMemory(&pReg->RegistrarInfo.ModelName, pData, WscLen);
				break;
				
			case WSC_ID_MODEL_NUMBER:
				NdisMoveMemory(&pReg->RegistrarInfo.ModelNumber, pData, WscLen);
				break;
				
			case WSC_ID_SERIAL_NUM:
				NdisMoveMemory(&pReg->RegistrarInfo.SerialNumber, pData, WscLen);
				break;
				
			case WSC_ID_PRIM_DEV_TYPE:
				NdisMoveMemory(&pReg->RegistrarInfo.PriDeviceType, pData, WscLen);
				break;
				
			case WSC_ID_DEVICE_NAME:
				NdisMoveMemory(&pReg->RegistrarInfo.DeviceName, pData, WscLen);
				break;
				
			case WSC_ID_RF_BAND:
				pReg->RegistrarInfo.RfBand = *pData;
				break;
				
			case WSC_ID_ASSOC_STATE:
				pReg->RegistrarInfo.AssocState = *((PUSHORT) pData);
				break;
				
			case WSC_ID_CONFIG_ERROR:
				pReg->RegistrarInfo.ConfigError = *((PUSHORT) pData);
				break;
				
			case WSC_ID_DEVICE_PWD_ID:
				break;
				
			case WSC_ID_OS_VERSION:
				pReg->RegistrarInfo.OsVersion = *((PULONG) pData);
				break;
				
			case WSC_ID_AUTHENTICATOR:
				// No authenticator in M2D
				break;

			default:
				DBGPRINT(RT_DEBUG_TRACE, "ProcessMessageM2D --> Unknown IE 0x%04x\n", WscType);
				break;
				
		}

		// Offset to net WSC Ie
		pData  += WscLen;
		Length -= WscLen;
	}					
	
	// Update WSC status
	// pAdapter->PortCfg.WscControl.WscStatus = STATUS_WSC_EAP_M2D_RECEIVED;	

	DBGPRINT(RT_DEBUG_TRACE, "ProcessMessageM2D : \n");
	return ret;
}

/*
	========================================================================
	
	Routine Description:
		Process WSC M3 Message

	Arguments:
		pAdapter    - NIC Adapter pointer
		pbuf		- recv buffer
		Length		- recv Length
		
	Return Value:
		None

	IRQL = DISPATCH_LEVEL
	
	Note:
		Actions after Rx M3
		1. Change the correct parameters
		2. Process M3
		
	========================================================================
*/
int ProcessMessageM3(
	IN	PRTMP_ADAPTER		pAdapter,
	IN	VOID *precv,
	IN	INT Length,
	OUT	PWSC_REG_DATA pReg)
{
	int					ret = WSC_ERROR_NO_ERROR;
	ULONG				HmacLen;
	UCHAR				Hmac[8], KDK[32];
	PUCHAR				pData = NULL;
	USHORT				WscType, WscLen, FieldCheck[7]={0,0,0,0,0,0,0};
	
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_VERSION))] |= (1 << WSC_TLV_BYTE1(WSC_ID_VERSION));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_MSG_TYPE))] |= (1 << WSC_TLV_BYTE1(WSC_ID_MSG_TYPE));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_REGISTRAR_NONCE))] |= (1 << WSC_TLV_BYTE1(WSC_ID_REGISTRAR_NONCE));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_E_HASH1))] |= (1 << WSC_TLV_BYTE1(WSC_ID_E_HASH1));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_E_HASH2))] |= (1 << WSC_TLV_BYTE1(WSC_ID_E_HASH2));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_AUTHENTICATOR))] |= (1 << WSC_TLV_BYTE1(WSC_ID_AUTHENTICATOR));

	// Copy the content to Regdata for lastRx information
	// Length must include authenticator IE size
	pReg->LastRx.Length = Length;		
	NdisMoveMemory(pReg->LastRx.Data, precv, Length);
	pData = pReg->LastRx.Data;

	// Start to process WSC IEs
	while (Length > 4)
	{
		WSC_TLV_0B	TLV_Recv;
		memcpy((u8 *)&TLV_Recv, pData, 4);
		WscType = cpu2be16(TLV_Recv.tag);
		WscLen  = cpu2be16(TLV_Recv.len);
		pData  += 4;
		Length -= 4;

		// Parse M3 WSC type and store to RegData structure
		switch (WscType)
		{
			case WSC_ID_VERSION:
				if(pReg->RegistrarInfo.Version != *pData)
					DBGPRINT(RT_DEBUG_ERROR, "Rx WPS Message Version mismatched %02x\n",*pData);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_VERSION))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_VERSION));
				break;
				
			case WSC_ID_MSG_TYPE:
				if(WSC_ID_MESSAGE_M3 != *pData)
					DBGPRINT(RT_DEBUG_ERROR, "Rx WPS Message Type mismatched %02x\n",*pData);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_MSG_TYPE))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_MSG_TYPE));
				break;
				
			case WSC_ID_REGISTRAR_NONCE:
				// for verification with our Registrar nonce
				if (RTMPCompareMemory(pReg->RegistrarNonce, pData, WscLen) != 0)
					DBGPRINT(RT_DEBUG_TRACE, "Rx M3 Compare Registrar nonce mismatched \n");
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_REGISTRAR_NONCE))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_REGISTRAR_NONCE));
				break;
				
			case WSC_ID_E_HASH1:
				NdisMoveMemory(&pReg->EHash1[0], pData, WscLen);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_E_HASH1))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_E_HASH1));
				break;
				
			case WSC_ID_E_HASH2:
				NdisMoveMemory(&pReg->EHash2[0], pData, WscLen);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_E_HASH2))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_E_HASH2));
				break;
				
			case WSC_ID_AUTHENTICATOR:
				NdisMoveMemory(Hmac, pData, WscLen);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_AUTHENTICATOR))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_AUTHENTICATOR));
				break;
				
			default:
				DBGPRINT(RT_DEBUG_TRACE, "ProcessMessageM3 --> Unknown IE 0x%04x\n", WscType);
				break;				
		}

		// Offset to net WSC Ie
		pData  += WscLen;
		Length -= WscLen;
	}

	// Combine last TX & RX message contents and validate the HMAC
	// We have to exclude last 12 bytes from last receive since it's authenticator value
	HmacLen = pReg->LastTx.Length + pReg->LastRx.Length - 12;
	NdisMoveMemory(&HmacData[0], pReg->LastTx.Data, pReg->LastTx.Length);
	NdisMoveMemory(&HmacData[pReg->LastTx.Length], pReg->LastRx.Data, pReg->LastRx.Length - 12);

	// Validate HMAC, reuse KDK buffer
	hmac_sha(pReg->AuthKey, 32, HmacData, HmacLen, KDK, 32);
	
	if (RTMPEqualMemory(Hmac, KDK, 8) != 1)
	{
		DBGPRINT(RT_DEBUG_ERROR, "ProcessMessageM3 --> HMAC not match\n");
		DBGPRINT(RT_DEBUG_TRACE, "MD --> 0x%08x-%08x\n", (ULONG )cpu2be32(*((PULONG ) KDK)), (ULONG )cpu2be32(*((PULONG )(KDK + 4))));
		DBGPRINT(RT_DEBUG_TRACE, "calculated --> 0x%08x-%08x\n", (ULONG )cpu2be32(*((PULONG ) &Hmac[0])), (ULONG )cpu2be32(*((PULONG ) &Hmac[4])));
		ret = WSC_ERROR_HMAC_FAIL;
	}

	if( FieldCheck[0] || FieldCheck[1] || FieldCheck[2] || FieldCheck[3] || FieldCheck[4] || FieldCheck[5] || FieldCheck[6] )
		ret = WSC_ERROR_WANTING_FIELD;
	DBGPRINT(RT_DEBUG_TRACE, "ProcessMessageM3 : \n");
	return ret;
}

/*
	========================================================================
	
	Routine Description:
		Process WSC M4 Message

	Arguments:
		pAdapter    - NIC Adapter pointer
		pbuf		- recv buffer
		Length		- recv Length
		
	Return Value:
		None

	IRQL = DISPATCH_LEVEL
	
	Note:
		Actions after Rx M4
		1. Change the correct parameters
		2. Process M4
		
	========================================================================
*/
int ProcessMessageM4(
	IN	PRTMP_ADAPTER		pAdapter,
// For AP Client support WPS Modification
	IN  PWSC_CTRL           pWscControl,
// --
	IN	VOID *precv,
	IN	INT Length,
	OUT	PWSC_REG_DATA pReg)
{
	int					ret = WSC_ERROR_NO_ERROR;
	ULONG				HmacLen;
	UCHAR				Hmac[8], KDK[32], RHash[32];
	INT					EncrLen;
	PUCHAR				pData = NULL;
	UCHAR				*IV_DecrData=NULL;//IV len 16 ,DecrData len 
	UCHAR				*pHash=NULL;//Reuse IV_DecrData memory
	USHORT				WscType, WscLen, FieldCheck[7]={0,0,0,0,0,0,0};
	
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_VERSION))] |= (1 << WSC_TLV_BYTE1(WSC_ID_VERSION));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_MSG_TYPE))] |= (1 << WSC_TLV_BYTE1(WSC_ID_MSG_TYPE));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_ENROLLEE_NONCE))] |= (1 << WSC_TLV_BYTE1(WSC_ID_ENROLLEE_NONCE));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_R_HASH1))] |= (1 << WSC_TLV_BYTE1(WSC_ID_R_HASH1));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_R_HASH2))] |= (1 << WSC_TLV_BYTE1(WSC_ID_R_HASH2));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_ENCR_SETTINGS))] |= (1 << WSC_TLV_BYTE1(WSC_ID_ENCR_SETTINGS));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_AUTHENTICATOR))] |= (1 << WSC_TLV_BYTE1(WSC_ID_AUTHENTICATOR));

	IV_DecrData = kmalloc(512, MEM_ALLOC_FLAG);
	if(NULL == IV_DecrData)
	{
		ret = WSC_ERROR_CAN_NOT_ALLOCMEM;
		return ret;
	}
	pHash = IV_DecrData;
	// Copy the content to Regdata for lastRx information
	// Length must include authenticator IE size
	pReg->LastRx.Length = Length;		
	NdisMoveMemory(pReg->LastRx.Data, precv, Length);
	pData = pReg->LastRx.Data;
	
	// Start to process WSC IEs
	while (Length > 4)
	{
		WSC_TLV_0B	TLV_Recv;
		memcpy((u8 *)&TLV_Recv, pData, 4);
		WscType = cpu2be16(TLV_Recv.tag);
		WscLen  = cpu2be16(TLV_Recv.len);
		pData  += 4;
		Length -= 4;

		// Parse M2 WSC type and store to RegData structure
		switch (WscType)
		{
			case WSC_ID_VERSION:
				if(pReg->EnrolleeInfo.Version != *pData)
					DBGPRINT(RT_DEBUG_ERROR, "Rx WPS Message Version mismatched %02x\n",*pData);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_VERSION))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_VERSION));
				break;
				
			case WSC_ID_MSG_TYPE:
				if(WSC_ID_MESSAGE_M4 != *pData)
					DBGPRINT(RT_DEBUG_ERROR, "Rx WPS Message Type mismatched %02x\n",*pData);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_MSG_TYPE))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_MSG_TYPE));
				break;
				
			case WSC_ID_ENROLLEE_NONCE:
				// for verification with our enrollee nonce
				if (RTMPCompareMemory(pReg->EnrolleeNonce, pData, WscLen) != 0)
					DBGPRINT(RT_DEBUG_TRACE, "Rx M4 Compare enrollee nonce mismatched \n");
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_ENROLLEE_NONCE))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_ENROLLEE_NONCE));
				break;
				
			case WSC_ID_R_HASH1:
				NdisMoveMemory(&pReg->RHash1, pData, WscLen);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_R_HASH1))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_R_HASH1));
				break;
				
			case WSC_ID_R_HASH2:
				NdisMoveMemory(&pReg->RHash2, pData, WscLen);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_R_HASH2))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_R_HASH2));
				break;

			case WSC_ID_ENCR_SETTINGS:
				// There shall have smoe kind of length check
				if (WscLen <= 16)
					break;
				if (WscLen > 512)
				{
					// ApEncrSetting is not enough
					DBGPRINT(RT_DEBUG_TRACE, "ApEncrSettings array size is not enough, require %d\n", WscLen);
				}
				NdisMoveMemory(IV_DecrData, pData, WscLen);
				WscDecryptData(IV_DecrData + 16, (WscLen - 16), pReg->KeyWrapKey, IV_DecrData, pReg->ApEncrSettings, &EncrLen);
				DBGPRINT(RT_DEBUG_TRACE, "M4 ApEncrSettings len = %d\n ", EncrLen);

				// Parse encryption settings
				WscParseEncrSettings(pAdapter, pReg->ApEncrSettings, EncrLen, pWscControl);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_ENCR_SETTINGS))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_ENCR_SETTINGS));
				break;
				
			case WSC_ID_AUTHENTICATOR:
				NdisMoveMemory(Hmac, pData, WscLen);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_AUTHENTICATOR))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_AUTHENTICATOR));
				break;
				
			default:
				DBGPRINT(RT_DEBUG_TRACE, "ProcessMessageM4 --> Unknown IE 0x%04x\n", WscType);
				break;
		}

		// Offset to net WSC Ie
		pData  += WscLen;
		Length -= WscLen;
	}

	// Verify R-Hash1
	// Create input for R-Hash1
	NdisMoveMemory(pHash, pReg->Rs1, 16);
	NdisMoveMemory(pHash + 16, pReg->Psk1, 16);
	NdisMoveMemory(pHash + 32, pReg->Pke, 192);
	NdisMoveMemory(pHash + 224, pReg->Pkr, 192);
	
	// Generate R-Hash1
	hmac_sha(pReg->AuthKey, 32, pHash, 416, RHash, 32);
	
	if (RTMPCompareMemory(pReg->RHash1, RHash, 32) != 0)
	{
		DBGPRINT(RT_DEBUG_TRACE, "ProcessMessageM4 --> RHash1 not matched\n");
		ret = WSC_ERROR_HASH_FAIL;
		goto out;
	}
	
	// Combine last TX & RX message contents and validate the HMAC
	// We have to exclude last 12 bytes from last receive since it's authenticator value
	HmacLen = pReg->LastTx.Length + pReg->LastRx.Length - 12;
	NdisMoveMemory(&HmacData[0], pReg->LastTx.Data, pReg->LastTx.Length);
	NdisMoveMemory(&HmacData[pReg->LastTx.Length], pReg->LastRx.Data, pReg->LastRx.Length - 12);

	// Validate HMAC, reuse KDK buffer
	hmac_sha(pReg->AuthKey, 32, HmacData, HmacLen, KDK, 32);
	
	if (RTMPEqualMemory(Hmac, KDK, 8) != 1)
	{
		DBGPRINT(RT_DEBUG_ERROR, "ProcessMessageM4 --> HMAC not match\n");
		DBGPRINT(RT_DEBUG_TRACE, "MD --> 0x%08x-%08x\n", (ULONG )cpu2be32(*((PULONG ) KDK)), (ULONG )cpu2be32(*((PULONG )(KDK + 4))));
		DBGPRINT(RT_DEBUG_TRACE, "calculated --> 0x%08x-%08x\n", (ULONG )cpu2be32(*((PULONG ) &Hmac[0])), (ULONG )cpu2be32(*((PULONG ) &Hmac[4])));
		ret = WSC_ERROR_HMAC_FAIL;
	}
	
	// Update WSC status
	// pAdapter->PortCfg.WscControl.WscStatus = STATUS_WSC_EAP_M4_RECEIVED;	
	if( FieldCheck[0] || FieldCheck[1] || FieldCheck[2] || FieldCheck[3] || FieldCheck[4] || FieldCheck[5] || FieldCheck[6] )
		ret = WSC_ERROR_WANTING_FIELD;
out :
	if(NULL != IV_DecrData)
		kfree(IV_DecrData);
	DBGPRINT(RT_DEBUG_TRACE, "ProcessMessageM4 : \n");
	return ret;
}

/*
	========================================================================
	
	Routine Description:
		Process WSC M5 Message

	Arguments:
		pAdapter    - NIC Adapter pointer
		pbuf		- recv buffer
		Length		- recv Length
		
	Return Value:
		None

	IRQL = DISPATCH_LEVEL
	
	Note:
		Actions after Rx M5
		1. Change the correct parameters
		2. Process M5
		
	========================================================================
*/
int ProcessMessageM5(
	IN	PRTMP_ADAPTER		pAdapter,
// For AP Client support WPS Modification
	IN  PWSC_CTRL           pWscControl,
// --
	IN	VOID *precv,
	IN	INT Length,
	OUT	PWSC_REG_DATA pReg)
{
	int					ret = WSC_ERROR_NO_ERROR;
	ULONG				HmacLen;
	UCHAR				Hmac[8], KDK[32], EHash[32];
	INT					EncrLen;
	PUCHAR				pData = NULL;
	UCHAR				*IV_DecrData=NULL;//IV len 16 ,DecrData len 
	UCHAR				*pHash=NULL;//Reuse IV_DecrData memory
	USHORT				WscType, WscLen, FieldCheck[7]={0,0,0,0,0,0,0};
	
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_VERSION))] |= (1 << WSC_TLV_BYTE1(WSC_ID_VERSION));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_MSG_TYPE))] |= (1 << WSC_TLV_BYTE1(WSC_ID_MSG_TYPE));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_REGISTRAR_NONCE))] |= (1 << WSC_TLV_BYTE1(WSC_ID_REGISTRAR_NONCE));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_ENCR_SETTINGS))] |= (1 << WSC_TLV_BYTE1(WSC_ID_ENCR_SETTINGS));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_AUTHENTICATOR))] |= (1 << WSC_TLV_BYTE1(WSC_ID_AUTHENTICATOR));

	IV_DecrData = kmalloc(512, MEM_ALLOC_FLAG);
	if(NULL == IV_DecrData)
	{
		ret = WSC_ERROR_CAN_NOT_ALLOCMEM;
		return ret;
	}
	pHash = IV_DecrData;
	// Copy the content to Regdata for lastRx information
	// Length must include authenticator IE size
	pReg->LastRx.Length = Length;		
	NdisMoveMemory(pReg->LastRx.Data, precv, Length);
	pData = pReg->LastRx.Data;

	// Start to process WSC IEs
	while (Length > 4)
	{
		WSC_TLV_0B	TLV_Recv;
		memcpy((u8 *)&TLV_Recv, pData, 4);
		WscType = cpu2be16(TLV_Recv.tag);
		WscLen  = cpu2be16(TLV_Recv.len);
		pData  += 4;
		Length -= 4;

		// Parse M2 WSC type and store to RegData structure
		switch (WscType)
		{
			case WSC_ID_VERSION:
				if(pReg->RegistrarInfo.Version != *pData)
					DBGPRINT(RT_DEBUG_ERROR, "Rx WPS Message Version mismatched %02x\n",*pData);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_VERSION))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_VERSION));
				break;
				
			case WSC_ID_MSG_TYPE:
				if(WSC_ID_MESSAGE_M5 != *pData)
					DBGPRINT(RT_DEBUG_ERROR, "Rx WPS Message Type mismatched %02x\n",*pData);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_MSG_TYPE))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_MSG_TYPE));
				break;
				
			case WSC_ID_REGISTRAR_NONCE:
				// for verification with our Registrar nonce
				if (RTMPCompareMemory(pReg->RegistrarNonce, pData, WscLen) != 0)
					DBGPRINT(RT_DEBUG_TRACE, "Rx M5 Compare Registrar nonce mismatched \n");
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_REGISTRAR_NONCE))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_REGISTRAR_NONCE));
				break;

			case WSC_ID_ENCR_SETTINGS:
				// There shall have smoe kind of length check
				if (WscLen <= 16)
					break;
				if (WscLen > 1024)
				{
					// ApEncrSetting is not enough
					DBGPRINT(RT_DEBUG_TRACE, "ApEncrSettings array size is not enough, require %d\n", WscLen);
				}
				NdisMoveMemory(IV_DecrData, pData, WscLen);
				WscDecryptData(IV_DecrData + 16, (WscLen - 16), pReg->KeyWrapKey, IV_DecrData, pReg->ApEncrSettings, &EncrLen);
				DBGPRINT(RT_DEBUG_TRACE, "M5 ApEncrSettings len = %d\n ", EncrLen);

				// Parse encryption settings
				WscParseEncrSettings(pAdapter, pReg->ApEncrSettings, EncrLen, pWscControl);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_ENCR_SETTINGS))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_ENCR_SETTINGS));
				break;
				
			case WSC_ID_AUTHENTICATOR:
				NdisMoveMemory(Hmac, pData, WscLen);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_AUTHENTICATOR))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_AUTHENTICATOR));
				break;
				
			default:
				DBGPRINT(RT_DEBUG_TRACE, "ProcessMessageM4 --> Unknown IE 0x%04x\n", WscType);
				break;
		}

		// Offset to net WSC Ie
		pData  += WscLen;
		Length -= WscLen;
	}
	
	// Combine last TX & RX message contents and validate the HMAC
	// We have to exclude last 12 bytes from last receive since it's authenticator value
	HmacLen = pReg->LastTx.Length + pReg->LastRx.Length - 12;
	NdisMoveMemory(&HmacData[0], pReg->LastTx.Data, pReg->LastTx.Length);
	NdisMoveMemory(&HmacData[pReg->LastTx.Length], pReg->LastRx.Data, pReg->LastRx.Length - 12);

	// Verify E-Hash1
	// Create input for E-Hash1
	NdisMoveMemory(pHash, pReg->Es1, 16);
	NdisMoveMemory(pHash + 16, pReg->Psk1, 16);
	NdisMoveMemory(pHash + 32, pReg->Pke, 192);
	NdisMoveMemory(pHash + 224, pReg->Pkr, 192);
	
	// Generate E-Hash1
	hmac_sha(pReg->AuthKey, 32, pHash, 416, EHash, 32);
	
	if (RTMPCompareMemory(pReg->EHash1, EHash, 32) != 0)
	{
		DBGPRINT(RT_DEBUG_TRACE, "ProcessMessageM5 --> EHash1 not matched\n");
		pReg->EnrolleeInfo.ConfigError = WSC_ERROR_HASH_FAIL;
		ret = WSC_ERROR_HASH_FAIL;
		goto out;
	}
	
	// Validate HMAC, reuse KDK buffer
	hmac_sha(pReg->AuthKey, 32, HmacData, HmacLen, KDK, 32);
	
	if (RTMPEqualMemory(Hmac, KDK, 8) != 1)
	{
		DBGPRINT(RT_DEBUG_ERROR, "ProcessMessageM5 --> HMAC not match\n");
		DBGPRINT(RT_DEBUG_TRACE, "MD --> 0x%08x-%08x\n", (ULONG )cpu2be32(*((PULONG ) KDK)), (ULONG )cpu2be32(*((PULONG )(KDK + 4))));
		DBGPRINT(RT_DEBUG_TRACE, "calculated --> 0x%08x-%08x\n", (ULONG )cpu2be32(*((PULONG ) &Hmac[0])), (ULONG )cpu2be32(*((PULONG ) &Hmac[4])));
		ret = WSC_ERROR_HMAC_FAIL;
	}
	if( FieldCheck[0] || FieldCheck[1] || FieldCheck[2] || FieldCheck[3] || FieldCheck[4] || FieldCheck[5] || FieldCheck[6] )
		ret = WSC_ERROR_WANTING_FIELD;
out :
	if(NULL != IV_DecrData)
		kfree(IV_DecrData);
	DBGPRINT(RT_DEBUG_TRACE, "ProcessMessageM5 : \n");
	return ret;
}

/*
	========================================================================
	
	Routine Description:
		Process WSC M6 Message

	Arguments:
		pAdapter    - NIC Adapter pointer
		pbuf		- recv buffer
		Length		- recv Length
		
	Return Value:
		None

	IRQL = DISPATCH_LEVEL
	
	Note:
		Actions after Rx M6
		1. Change the correct parameters
		2. Process M6
		
	========================================================================
*/
int ProcessMessageM6(
	IN	PRTMP_ADAPTER		pAdapter,
// For AP Client support WPS Modification
	IN  PWSC_CTRL           pWscControl,
// --
	IN	VOID *precv,
	IN	INT Length,
	OUT	PWSC_REG_DATA pReg)
{
	int					ret = WSC_ERROR_NO_ERROR;
	ULONG				HmacLen;
	UCHAR				Hmac[8], KDK[32], RHash[32];
	INT					EncrLen;
	PUCHAR				pData = NULL;
	UCHAR				*IV_DecrData=NULL;//IV len 16 ,DecrData len 
	UCHAR				*pHash=NULL;//Reuse IV_DecrData memory
	USHORT				WscType, WscLen, FieldCheck[7]={0,0,0,0,0,0,0};
	
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_VERSION))] |= (1 << WSC_TLV_BYTE1(WSC_ID_VERSION));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_MSG_TYPE))] |= (1 << WSC_TLV_BYTE1(WSC_ID_MSG_TYPE));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_ENROLLEE_NONCE))] |= (1 << WSC_TLV_BYTE1(WSC_ID_ENROLLEE_NONCE));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_ENCR_SETTINGS))] |= (1 << WSC_TLV_BYTE1(WSC_ID_ENCR_SETTINGS));
	FieldCheck[(WSC_TLV_BYTE2(WSC_ID_AUTHENTICATOR))] |= (1 << WSC_TLV_BYTE1(WSC_ID_AUTHENTICATOR));

	IV_DecrData = kmalloc(512, MEM_ALLOC_FLAG);
	if(NULL == IV_DecrData)
	{
		ret = WSC_ERROR_CAN_NOT_ALLOCMEM;
		return ret;
	}
	pHash = IV_DecrData;
	// Copy the content to Regdata for lastRx information
	// Length must include authenticator IE size
	pReg->LastRx.Length = Length;		
	NdisMoveMemory(pReg->LastRx.Data, precv, Length);
	pData = pReg->LastRx.Data;

	// Start to process WSC IEs
	while (Length > 4)
	{
		WSC_TLV_0B	TLV_Recv;
		memcpy((u8 *)&TLV_Recv, pData, 4);
		WscType = cpu2be16(TLV_Recv.tag);
		WscLen  = cpu2be16(TLV_Recv.len);
		pData  += 4;
		Length -= 4;

		// Parse M2 WSC type and store to RegData structure
		switch (WscType)
		{
			case WSC_ID_VERSION:
				if(pReg->EnrolleeInfo.Version != *pData)
					DBGPRINT(RT_DEBUG_ERROR, "Rx WPS Message Version mismatched %02x\n",*pData);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_VERSION))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_VERSION));
				break;
				
			case WSC_ID_MSG_TYPE:
				if(WSC_ID_MESSAGE_M6 != *pData)
					DBGPRINT(RT_DEBUG_ERROR, "Rx WPS Message Type mismatched %02x\n",*pData);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_MSG_TYPE))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_MSG_TYPE));
				break;
				
			case WSC_ID_ENROLLEE_NONCE:
				// for verification with our enrollee nonce
				if (RTMPCompareMemory(pReg->EnrolleeNonce, pData, WscLen) != 0)
					DBGPRINT(RT_DEBUG_TRACE, "Rx M6 Compare enrollee nonce mismatched \n");
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_ENROLLEE_NONCE))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_ENROLLEE_NONCE));
				break;
				
			case WSC_ID_ENCR_SETTINGS:
				// There shall have smoe kind of length check
				if (WscLen <= 16)
					break;
				if (WscLen > 512)
				{
					// ApEncrSetting is not enough
					DBGPRINT(RT_DEBUG_TRACE, "ApEncrSettings array size is not enough, require %d\n", WscLen);
				}
				NdisMoveMemory(IV_DecrData, pData, WscLen);
				WscDecryptData(IV_DecrData + 16, (WscLen - 16), pReg->KeyWrapKey, IV_DecrData, pReg->ApEncrSettings, &EncrLen);
				DBGPRINT(RT_DEBUG_TRACE, "M6 ApEncrSettings len = %d\n ", EncrLen);

				// Parse encryption settings
				WscParseEncrSettings(pAdapter, pReg->ApEncrSettings, EncrLen, pWscControl);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_ENCR_SETTINGS))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_ENCR_SETTINGS));
				break;
				
			case WSC_ID_AUTHENTICATOR:
				NdisMoveMemory(Hmac, pData, WscLen);
				FieldCheck[(WSC_TLV_BYTE2(WSC_ID_AUTHENTICATOR))] ^= (1 << WSC_TLV_BYTE1(WSC_ID_AUTHENTICATOR));
				break;
				
			default:
				DBGPRINT(RT_DEBUG_TRACE, "ProcessMessageM6 --> Unknown IE 0x%04x\n", WscType);
				break;
		}

		// Offset to net WSC Ie
		pData  += WscLen;
		Length -= WscLen;
	}

	// Verify R-Hash2
	// Create input for R-Hash1
	NdisMoveMemory(pHash, pReg->Rs2, 16);
	NdisMoveMemory(pHash + 16, pReg->Psk2, 16);
	NdisMoveMemory(pHash + 32, pReg->Pke, 192);
	NdisMoveMemory(pHash + 224, pReg->Pkr, 192);
	
	// Generate R-Hash2
	hmac_sha(pReg->AuthKey, 32, pHash, 416, RHash, 32);
	
	if (RTMPCompareMemory(pReg->RHash2, RHash, 32) != 0)
	{
		DBGPRINT(RT_DEBUG_TRACE, "ProcessMessageM6 --> RHash2 not matched\n");
		// add by johnli, bug fix: 2nd half of the pin code not matched
		ret = WSC_ERROR_HASH_FAIL;
		goto out;
		// end johnli
	}
	
	// Combine last TX & RX message contents and validate the HMAC
	// We have to exclude last 12 bytes from last receive since it's authenticator value
	HmacLen = pReg->LastTx.Length + pReg->LastRx.Length - 12;
	NdisMoveMemory(&HmacData[0], pReg->LastTx.Data, pReg->LastTx.Length);
	NdisMoveMemory(&HmacData[pReg->LastTx.Length], pReg->LastRx.Data, pReg->LastRx.Length - 12);

	// Validate HMAC, reuse KDK buffer
	hmac_sha(pReg->AuthKey, 32, HmacData, HmacLen, KDK, 32);
	
	if (RTMPEqualMemory(Hmac, KDK, 8) != 1)
	{
		DBGPRINT(RT_DEBUG_ERROR, "ProcessMessageM6 --> HMAC not match\n");
		DBGPRINT(RT_DEBUG_TRACE, "MD --> 0x%08x-%08x\n", (ULONG )cpu2be32(*((PULONG ) KDK)), (ULONG )cpu2be32(*((PULONG )(KDK + 4))));
		DBGPRINT(RT_DEBUG_TRACE, "calculated --> 0x%08x-%08x\n", (ULONG )cpu2be32(*((PULONG ) &Hmac[0])), (ULONG )cpu2be32(*((PULONG ) &Hmac[4])));
		ret = WSC_ERROR_HMAC_FAIL;
	}
	
	// Update WSC status
	// pAdapter->PortCfg.WscControl.WscStatus = STATUS_WSC_EAP_M6_RECEIVED;	

	if( FieldCheck[0] || FieldCheck[1] || FieldCheck[2] || FieldCheck[3] || FieldCheck[4] || FieldCheck[5] || FieldCheck[6] )
		ret = WSC_ERROR_WANTING_FIELD;
out :
	if(NULL != IV_DecrData)
		kfree(IV_DecrData);
	DBGPRINT(RT_DEBUG_TRACE, "ProcessMessageM6 : \n");
	return ret;
}

/*
	========================================================================
	
	Routine Description:
		Process WSC M7 Message

	Arguments:
		pAdapter    - NIC Adapter pointer
		pbuf		- recv buffer
		Length		- recv Length
		
	Return Value:
		None

	IRQL = DISPATCH_LEVEL
	
	Note:
		Actions after Rx M7
		1. Change the correct parameters
		2. Process M7
		
	========================================================================
*/
int ProcessMessageM7(
	IN	PRTMP_ADAPTER		pAdapter,
// For AP Client support WPS Modification
	IN  PWSC_CTRL           pWscControl,
// --
	IN	VOID *precv,
	IN	INT Length,
	OUT	PWSC_REG_DATA pReg)
{
	int					ret = WSC_ERROR_NO_ERROR;
	ULONG				HmacLen;
	UCHAR				Hmac[8], KDK[32];
	INT					EncrLen;
	PUCHAR				pData = NULL;
	USHORT				WscType, WscLen;
	UCHAR				*IV_DecrData=NULL;//IV len 16 ,DecrData len 
	
	IV_DecrData = kmalloc(1024, MEM_ALLOC_FLAG);
	if(NULL == IV_DecrData)
	{
		ret = WSC_ERROR_CAN_NOT_ALLOCMEM;
		return ret;
	}
	// Copy the content to Regdata for lastRx information
	// Length must include authenticator IE size
	pReg->LastRx.Length = Length;		
	NdisMoveMemory(pReg->LastRx.Data, precv, Length);
	pData = pReg->LastRx.Data;

	// Start to process WSC IEs
	while (Length > 4)
	{
		WSC_TLV_0B	TLV_Recv;
		memcpy((u8 *)&TLV_Recv, pData, 4);
		WscType = cpu2be16(TLV_Recv.tag);
		WscLen  = cpu2be16(TLV_Recv.len);
		pData  += 4;
		Length -= 4;

		// Parse M2 WSC type and store to RegData structure
		switch (WscType)
		{
			case WSC_ID_VERSION:
				if(pReg->RegistrarInfo.Version != *pData)
					DBGPRINT(RT_DEBUG_ERROR, "Rx WPS Message Version mismatched %02x\n",*pData);
				break;
				
			case WSC_ID_MSG_TYPE:
				if(WSC_ID_MESSAGE_M7 != *pData)
					DBGPRINT(RT_DEBUG_ERROR, "Rx WPS Message Type mismatched %02x\n",*pData);
				break;
				
			case WSC_ID_REGISTRAR_NONCE:
				// for verification with our Registrar nonce
				if (RTMPCompareMemory(pReg->RegistrarNonce, pData, WscLen) != 0)
				{
					DBGPRINT(RT_DEBUG_TRACE, "Rx M5 Compare Registrar nonce mismatched \n");
				}
				break;

			case WSC_ID_ENCR_SETTINGS:
				// There shall have smoe kind of length check
				if (WscLen <= 16)
					break;
				if (WscLen > 1024)
				{
					// ApEncrSetting is not enough
					DBGPRINT(RT_DEBUG_TRACE, "ApEncrSettings array size is not enough, require %d\n", WscLen);
				}
				NdisMoveMemory(IV_DecrData, pData, WscLen);
				WscDecryptData(IV_DecrData + 16, (WscLen - 16), pReg->KeyWrapKey, IV_DecrData, pReg->ApEncrSettings, &EncrLen);
				DBGPRINT(RT_DEBUG_TRACE, "M7 ApEncrSettings len = %d\n ", EncrLen);

				// Parse encryption settings
				WscParseEncrSettings(pAdapter, pReg->ApEncrSettings, EncrLen, pWscControl);
				break;
				
			case WSC_ID_AUTHENTICATOR:
				NdisMoveMemory(Hmac, pData, WscLen);
				break;
				
			default:
				DBGPRINT(RT_DEBUG_TRACE, "ProcessMessageM7 --> Unknown IE 0x%04x\n", WscType);
				break;
		}

		// Offset to net WSC Ie
		pData  += WscLen;
		Length -= WscLen;
	}
	
	// Combine last TX & RX message contents and validate the HMAC
	// We have to exclude last 12 bytes from last receive since it's authenticator value
	HmacLen = pReg->LastTx.Length + pReg->LastRx.Length - 12;
	NdisMoveMemory(&HmacData[0], pReg->LastTx.Data, pReg->LastTx.Length);
	NdisMoveMemory(&HmacData[pReg->LastTx.Length], pReg->LastRx.Data, pReg->LastRx.Length - 12);

	// Validate HMAC, reuse KDK buffer
	hmac_sha(pReg->AuthKey, 32, HmacData, HmacLen, KDK, 32);
	
	if (RTMPEqualMemory(Hmac, KDK, 8) != 1)
	{
		DBGPRINT(RT_DEBUG_ERROR, "ProcessMessageM7 --> HMAC not match\n");
		DBGPRINT(RT_DEBUG_TRACE, "MD --> 0x%08x-%08x\n", (ULONG )cpu2be32(*((PULONG ) KDK)), (ULONG )cpu2be32(*((PULONG )(KDK + 4))));
		DBGPRINT(RT_DEBUG_TRACE, "calculated --> 0x%08x-%08x\n", (ULONG )cpu2be32(*((PULONG ) &Hmac[0])), (ULONG )cpu2be32(*((PULONG ) &Hmac[4])));
		ret = WSC_ERROR_HMAC_FAIL;
	}

	if(NULL != IV_DecrData)
		kfree(IV_DecrData);
	DBGPRINT(RT_DEBUG_TRACE, "ProcessMessageM7 : \n");
	return ret;
}

/*
	========================================================================
	
	Routine Description:
		Process WSC M8 Message

	Arguments:
		pAdapter    - NIC Adapter pointer
		pbuf		- recv buffer
		Length		- recv Length
		
	Return Value:
		None

	IRQL = DISPATCH_LEVEL
	
	Note:
		Actions after Rx M8
		1. Change the correct parameters
		2. Process M8
		
	========================================================================
*/
int ProcessMessageM8(
	IN	PRTMP_ADAPTER		pAdapter,
// For AP Client support WPS Modification
	IN  PWSC_CTRL           pWscControl,
// --
	IN	VOID *precv,
	IN	INT Length,
	OUT	PWSC_REG_DATA pReg)
{
	int					ret = WSC_ERROR_NO_ERROR;
	ULONG				HmacLen;
	UCHAR				Hmac[8], KDK[32];
	INT					EncrLen;
	PUCHAR				pData = NULL;
	USHORT				WscType, WscLen;
	UCHAR				*IV_DecrData=NULL;//IV len 16 ,DecrData len 
	
	IV_DecrData = kmalloc(1024, MEM_ALLOC_FLAG);
	if(NULL == IV_DecrData)
	{
		ret = WSC_ERROR_CAN_NOT_ALLOCMEM;
		return ret;
	}
	// Copy the content to Regdata for lastRx information
	// Length must include authenticator IE size
	pReg->LastRx.Length = Length;		
	NdisMoveMemory(pReg->LastRx.Data, precv, Length);
	pData = pReg->LastRx.Data;

	// Start to process WSC IEs
	while (Length > 4)
	{
		WSC_TLV_0B	TLV_Recv;
		memcpy((u8 *)&TLV_Recv, pData, 4);
		WscType = cpu2be16(TLV_Recv.tag);
		WscLen  = cpu2be16(TLV_Recv.len);
		pData  += 4;
		Length -= 4;

		// Parse M8 WSC type and store to RegData structure
		switch (WscType)
		{
			case WSC_ID_VERSION:
				if(pReg->EnrolleeInfo.Version != *pData)
					DBGPRINT(RT_DEBUG_ERROR, "Rx WPS Message Version mismatched %02x\n",*pData);
				break;
				
			case WSC_ID_MSG_TYPE:
				if(WSC_ID_MESSAGE_M8 != *pData)
					DBGPRINT(RT_DEBUG_ERROR, "Rx WPS Message Type mismatched %02x\n",*pData);
				break;
				
			case WSC_ID_ENROLLEE_NONCE:
				// for verification with our enrollee nonce
				if (RTMPCompareMemory(pReg->EnrolleeNonce, pData, WscLen) != 0)
				{
					DBGPRINT(RT_DEBUG_TRACE, "Rx M8 Compare enrollee nonce mismatched \n");
				}
				break;
				
			case WSC_ID_ENCR_SETTINGS:
				// There shall have smoe kind of length check
				if (WscLen <= 16)
					break;
				if (WscLen > 512)
				{
					// ApEncrSetting is not enough
					DBGPRINT(RT_DEBUG_TRACE, "ApEncrSettings array size is not enough, require %d\n", WscLen);
				}
				NdisMoveMemory(IV_DecrData, pData, WscLen);
				WscDecryptData(IV_DecrData + 16, (WscLen - 16), pReg->KeyWrapKey, IV_DecrData, pReg->ApEncrSettings, &EncrLen);
				DBGPRINT(RT_DEBUG_TRACE, "M8 ApEncrSettings len = %d\n ", EncrLen);

				// Parse encryption settings
				WscProcessCredential(pAdapter, pReg->ApEncrSettings, EncrLen, pWscControl);
				break;
				
			case WSC_ID_AUTHENTICATOR:
				NdisMoveMemory(Hmac, pData, WscLen);
				break;
				
			default:
				DBGPRINT(RT_DEBUG_TRACE, "ProcessMessageM8 --> Unknown IE 0x%04x\n", WscType);
				break;
		}

		// Offset to net WSC Ie
		pData  += WscLen;
		Length -= WscLen;
	}
	
	// Combine last TX & RX message contents and validate the HMAC
	// We have to exclude last 12 bytes from last receive since it's authenticator value
	HmacLen = pReg->LastTx.Length + pReg->LastRx.Length - 12;
	NdisMoveMemory(&HmacData[0], pReg->LastTx.Data, pReg->LastTx.Length);
	NdisMoveMemory(&HmacData[pReg->LastTx.Length], pReg->LastRx.Data, pReg->LastRx.Length - 12);

	// Validate HMAC, reuse KDK buffer
	hmac_sha(pReg->AuthKey, 32, HmacData, HmacLen, KDK, 32);
	
	if (RTMPEqualMemory(Hmac, KDK, 8) != 1)
	{
		DBGPRINT(RT_DEBUG_ERROR, "ProcessMessageM8 --> HMAC not match\n");
		DBGPRINT(RT_DEBUG_TRACE, "MD --> 0x%08x-%08x\n", (ULONG )cpu2be32(*((PULONG ) KDK)), (ULONG )cpu2be32(*((PULONG )(KDK + 4))));
		DBGPRINT(RT_DEBUG_TRACE, "calculated --> 0x%08x-%08x\n", (ULONG )cpu2be32(*((PULONG ) &Hmac[0])), (ULONG )cpu2be32(*((PULONG ) &Hmac[4])));
		ret = WSC_ERROR_HMAC_FAIL;
	}
	
	// Update WSC status
	// pAdapter->PortCfg.WscControl.WscStatus = STATUS_WSC_EAP_M8_RECEIVED;	

	if(NULL != IV_DecrData)
		kfree(IV_DecrData);
	DBGPRINT(RT_DEBUG_TRACE, "ProcessMessageM8 : \n");
	return ret;
}

