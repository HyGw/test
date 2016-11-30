/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology	5th	Rd.
 * Science-based Industrial	Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2005, Ralink Technology, Inc.
 *
 * All rights reserved.	Ralink's source	code is	an unpublished work	and	the
 * use of a	copyright notice does not imply	otherwise. This	source code
 * contains	confidential trade secret material of Ralink Tech. Any attempt
 * or participation	in deciphering,	decoding, reverse engineering or in	any
 * way altering	the	source code	is stricitly prohibited, unless	the	prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

	Module Name:
	mlme.c

	Abstract:

	Revision History:
	Who			When			What
	--------	----------		----------------------------------------------
	John Chang	2004-08-25		Modify from RT2500 code base
	John Chang  2004-09-06      modified for RT2600
*/

#include "rt_config.h"
#include <stdarg.h>
#include <linux/random.h>

// e.g. RssiSafeLevelForTxRate[RATE_36]" means if the current RSSI is greater than 
//      this value, then it's quaranteed capable of operating in 36 mbps TX rate in 
//      clean environment.
//                                TxRate: 1   2   5.5   11   6    9    12   18   24   36   48   54   72  100
CHAR RssiSafeLevelForTxRate[] ={  -92, -91, -90, -87, -88, -86, -85, -83, -81, -78, -72, -71, -40, -40 };

                                  //  1      2       5.5      11  
UCHAR Phy11BNextRateDownward[] = {RATE_1, RATE_1,   RATE_2,  RATE_5_5};
UCHAR Phy11BNextRateUpward[]   = {RATE_2, RATE_5_5, RATE_11, RATE_11};

                                  //  1      2       5.5      11        6        9        12      18       24       36       48       54
UCHAR Phy11BGNextRateDownward[]= {RATE_1, RATE_1,   RATE_2,  RATE_5_5,RATE_11,  RATE_11, RATE_11, RATE_12, RATE_18, RATE_24, RATE_36, RATE_48};
UCHAR Phy11BGNextRateUpward[]  = {RATE_2, RATE_5_5, RATE_11, RATE_12, RATE_12,  RATE_12, RATE_18, RATE_24, RATE_36, RATE_48, RATE_54, RATE_54};

                                  //  1      2       5.5      11        6        9        12      18       24       36       48       54
UCHAR Phy11ANextRateDownward[] = {RATE_6, RATE_6,   RATE_6,  RATE_6,  RATE_6,   RATE_6,  RATE_9,  RATE_12, RATE_18, RATE_24, RATE_36, RATE_48};
UCHAR Phy11ANextRateUpward[]   = {RATE_9, RATE_9,   RATE_9,  RATE_9,  RATE_9,   RATE_12, RATE_18, RATE_24, RATE_36, RATE_48, RATE_54, RATE_54};

//                              RATE_1,  2, 5.5, 11,  6,  9, 12, 18, 24, 36, 48, 54
static USHORT RateUpPER[]   = {    40,  40,  35, 20, 20, 20, 20, 16, 10, 16, 10,  6 }; // in percentage
static USHORT RateDownPER[] = {    50,  50,  45, 45, 35, 35, 35, 35, 25, 25, 25, 13 }; // in percentage

UCHAR  RateIdToMbps[]    = { 1, 2, 5, 11, 6, 9, 12, 18, 24, 36, 48, 54, 72, 100};
USHORT RateIdTo500Kbps[] = { 2, 4, 11, 22, 12, 18, 24, 36, 48, 72, 96, 108, 144, 200};

UCHAR BROADCAST_ADDR[MAC_ADDR_LEN] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
UCHAR ZERO_MAC_ADDR[MAC_ADDR_LEN]  = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// Reset the RFIC setting to new series    
RTMP_RF_REGS RF5225RegTable[] = {
//      ch   R1          R2          R3(TX0~4=0) R4
		{1,  0x95002ccc, 0x95004786, 0x95068455, 0x950ffa0b},
		{2,  0x95002ccc, 0x95004786, 0x95068455, 0x950ffa1f},
		{3,  0x95002ccc, 0x9500478a, 0x95068455, 0x950ffa0b},
		{4,  0x95002ccc, 0x9500478a, 0x95068455, 0x950ffa1f},
		{5,  0x95002ccc, 0x9500478e, 0x95068455, 0x950ffa0b},
		{6,  0x95002ccc, 0x9500478e, 0x95068455, 0x950ffa1f},
		{7,  0x95002ccc, 0x95004792, 0x95068455, 0x950ffa0b},
		{8,  0x95002ccc, 0x95004792, 0x95068455, 0x950ffa1f},
		{9,  0x95002ccc, 0x95004796, 0x95068455, 0x950ffa0b},
		{10, 0x95002ccc, 0x95004796, 0x95068455, 0x950ffa1f},
		{11, 0x95002ccc, 0x9500479a, 0x95068455, 0x950ffa0b},
		{12, 0x95002ccc, 0x9500479a, 0x95068455, 0x950ffa1f},
		{13, 0x95002ccc, 0x9500479e, 0x95068455, 0x950ffa0b},
		{14, 0x95002ccc, 0x950047a2, 0x95068455, 0x950ffa13},

		// 802.11 UNI / HyperLan 2
		{36, 0x95002ccc, 0x9500499a, 0x9509be55, 0x950ffa23},
		{40, 0x95002ccc, 0x950049a2, 0x9509be55, 0x950ffa03},
		{44, 0x95002ccc, 0x950049a6, 0x9509be55, 0x950ffa0b},
		{48, 0x95002ccc, 0x950049aa, 0x9509be55, 0x950ffa13},
		{52, 0x95002ccc, 0x950049ae, 0x9509ae55, 0x950ffa1b},
		{56, 0x95002ccc, 0x950049b2, 0x9509ae55, 0x950ffa23},
		{60, 0x95002ccc, 0x950049ba, 0x9509ae55, 0x950ffa03},
		{64, 0x95002ccc, 0x950049be, 0x9509ae55, 0x950ffa0b},

		// 802.11 HyperLan 2
		{100, 0x95002ccc, 0x95004a2a, 0x950bae55, 0x950ffa03},
		{104, 0x95002ccc, 0x95004a2e, 0x950bae55, 0x950ffa0b},
		{108, 0x95002ccc, 0x95004a32, 0x950bae55, 0x950ffa13},
		{112, 0x95002ccc, 0x95004a36, 0x950bae55, 0x950ffa1b},
		{116, 0x95002ccc, 0x95004a3a, 0x950bbe55, 0x950ffa23},
		{120, 0x95002ccc, 0x95004a82, 0x950bbe55, 0x950ffa03},
		{124, 0x95002ccc, 0x95004a86, 0x950bbe55, 0x950ffa0b},
		{128, 0x95002ccc, 0x95004a8a, 0x950bbe55, 0x950ffa13},
		{132, 0x95002ccc, 0x95004a8e, 0x950bbe55, 0x950ffa1b},
		{136, 0x95002ccc, 0x95004a92, 0x950bbe55, 0x950ffa23},

		// 802.11 UNII
		{140, 0x95002ccc, 0x95004a9a, 0x950bbe55, 0x950ffa03},
		{149, 0x95002ccc, 0x95004aa2, 0x950bbe55, 0x950ffa1f},
		{153, 0x95002ccc, 0x95004aa6, 0x950bbe55, 0x950ffa27},
		{157, 0x95002ccc, 0x95004aae, 0x950bbe55, 0x950ffa07},
		{161, 0x95002ccc, 0x95004ab2, 0x950bbe55, 0x950ffa0f},
		{165, 0x95002ccc, 0x95004ab6, 0x950bbe55, 0x950ffa17},
		
		//MMAC(Japan)J52 ch 34,38,42,46
		{34, 0x95002ccc, 0x9500499a, 0x9509be55, 0x950ffa0b},
		{38, 0x95002ccc, 0x9500499e, 0x9509be55, 0x950ffa13},
		{42, 0x95002ccc, 0x950049a2, 0x9509be55, 0x950ffa1b},
		{46, 0x95002ccc, 0x950049a6, 0x9509be55, 0x950ffa23},
};
UCHAR NUM_OF_5225_CHNL = (sizeof(RF5225RegTable) / sizeof(RTMP_RF_REGS));

// Reset the RFIC setting to new series    
RTMP_RF_REGS RF5225RegTable_1[] = {
//      ch   R1          R2          R3(TX0~4=0) R4
        {1,  0x95002ccc, 0x95004786, 0x95068455, 0x950ffa0b},
        {2,  0x95002ccc, 0x95004786, 0x95068455, 0x950ffa1f},
        {3,  0x95002ccc, 0x9500478a, 0x95068455, 0x950ffa0b},
        {4,  0x95002ccc, 0x9500478a, 0x95068455, 0x950ffa1f},
        {5,  0x95002ccc, 0x9500478e, 0x95068455, 0x950ffa0b},
        {6,  0x95002ccc, 0x9500478e, 0x95068455, 0x950ffa1f},
        {7,  0x95002ccc, 0x95004792, 0x95068455, 0x950ffa0b},
        {8,  0x95002ccc, 0x95004792, 0x95068455, 0x950ffa1f},
        {9,  0x95002ccc, 0x95004796, 0x95068455, 0x950ffa0b},
        {10, 0x95002ccc, 0x95004796, 0x95068455, 0x950ffa1f},
        {11, 0x95002ccc, 0x9500479a, 0x95068455, 0x950ffa0b},
        {12, 0x95002ccc, 0x9500479a, 0x95068455, 0x950ffa1f},
        {13, 0x95002ccc, 0x9500479e, 0x95068455, 0x950ffa0b},
        {14, 0x95002ccc, 0x950047a2, 0x95068455, 0x950ffa13},
    
        // 802.11 UNI / HyperLan 2
        {36,	0x95002cd4,	0x9504481a,	0x95098455,	0x950c0a03},
		{40,	0x95002cd0,	0x95044682,	0x95098455,	0x950c0a03},
		{44,	0x95002cd0,	0x95044686,	0x95098455,	0x950c0a1b},
		{48,	0x95002cd0,	0x9504468e,	0x95098655,	0x950c0a0b},
		{52,	0x95002cd0,	0x95044692,	0x95098855,	0x950c0a23},
		{56,	0x95002cd0,	0x9504469a,	0x95098c55,	0x950c0a13},
		{60,	0x95002cd0,	0x950446a2,	0x95098e55,	0x950c0a03},
		{64,	0x95002cd0,	0x950446a6,	0x95099255,	0x950c0a1b},

        // 802.11 HyperLan 2
        {100,	0x95002cd4,	0x9504489a,	0x950b9855,	0x950c0a03},
		{104,	0x95002cd4,	0x950448a2,	0x950b9855,	0x950c0a03},
		{108,	0x95002cd4,	0x950448aa,	0x950b9855,	0x950c0a03},
		{112,	0x95002cd4,	0x950448b2,	0x950b9a55,	0x950c0a03},
		{116,	0x95002cd4,	0x950448ba,	0x950b9a55,	0x950c0a03},
		{120,	0x95002cd0,	0x95044702,	0x950b9a55,	0x950c0a03},
		{124,	0x95002cd0,	0x95044706,	0x950b9a55,	0x950c0a1b},
		{128,	0x95002cd0,	0x9504470e,	0x950b9c55,	0x950c0a0b},
		{132,	0x95002cd0,	0x95044712,	0x950b9c55,	0x950c0a23},
		{136,	0x95002cd0,	0x9504471a,	0x950b9e55,	0x950c0a13},
    
        // 802.11 UNII
        {140,	0x95002cd0,	0x95044722,	0x950b9e55,	0x950c0a03},
		{149,	0x95002cd0,	0x9504472e,	0x950ba255,	0x950c0a1b},
		{153,	0x95002cd0,	0x95044736,	0x950ba255,	0x950c0a0b},
		{157,	0x95002cd4,	0x9504490a,	0x950ba255,	0x950c0a17},
		{161,	0x95002cd4,	0x95044912,	0x950ba255,	0x950c0a17},
		{165,	0x95002cd4,	0x9504491a,	0x950ba255,	0x950c0a17},
		
        //MMAC(Japan)J52 ch 34,38,42,46
		{34, 0x95002ccc, 0x9500499a, 0x9509be55, 0x950c0a0b},
		{38, 0x95002ccc, 0x9500499e, 0x9509be55, 0x950c0a13},
		{42, 0x95002ccc, 0x950049a2, 0x9509be55, 0x950c0a1b},
		{46, 0x95002ccc, 0x950049a6, 0x9509be55, 0x950c0a23},
};
UCHAR NUM_OF_5225_CHNL_1 = (sizeof(RF5225RegTable_1) / sizeof(RTMP_RF_REGS));

#ifdef RTL865X_SOC
static inline VOID RadarSMDetectPeriodic(
	IN	unsigned long data);

static inline VOID BeaconUpdateExec(
	IN	unsigned long data);
#endif

static UCHAR DynamicBbpR17UpBoundaryTuning(
	IN UCHAR CurR17UpBoundary,
	IN UCHAR CurR17LowBoundary,
	IN int RssidBm);

/*
    ==========================================================================
    Description:
        initialize the MLME task and its data structure (queue, spinlock, 
        timer, state machines).
        
    Return:
        always return NDIS_STATUS_SUCCESS
        
    ==========================================================================
*/
NDIS_STATUS MlmeInit(
    IN PRTMP_ADAPTER pAd) 
{
    NDIS_STATUS Status = NDIS_STATUS_SUCCESS;

    DBGPRINT(RT_DEBUG_TRACE, "--> MLME Initialize\n");
    
    do 
    {
        Status = MlmeQueueInit(&pAd->Mlme.Queue);
        if(Status != NDIS_STATUS_SUCCESS) 
            break;

        pAd->Mlme.bRunning = FALSE;
        NdisAllocateSpinLock(&pAd->Mlme.TaskLock);
		NdisAllocateSpinLock(&pAd->Mlme.MemLock);
        pAd->Mlme.DtimCount = 0;
        pAd->Mlme.DtimPeriod = DEFAULT_DTIM_PERIOD;

        // init state machines
        ASSERT(ASSOC_FUNC_SIZE == MAX_ASSOC_MSG * MAX_ASSOC_STATE);
        AssocStateMachineInit(pAd, &pAd->Mlme.AssocMachine, pAd->Mlme.AssocFunc);
        
        ASSERT(AUTH_FUNC_SIZE == MAX_AUTH_MSG * MAX_AUTH_STATE);
        AuthStateMachineInit(pAd, &pAd->Mlme.AuthMachine, pAd->Mlme.AuthFunc);
        
        ASSERT(AUTH_RSP_FUNC_SIZE == MAX_AUTH_RSP_MSG * MAX_AUTH_RSP_STATE);
        AuthRspStateMachineInit(pAd, &pAd->Mlme.AuthRspMachine, pAd->Mlme.AuthRspFunc);

        ASSERT(SYNC_FUNC_SIZE == MAX_SYNC_MSG * MAX_SYNC_STATE);
        SyncStateMachineInit(pAd, &pAd->Mlme.SyncMachine, pAd->Mlme.SyncFunc);

        ASSERT(WPA_FUNC_SIZE == MAX_WPA_MSG * MAX_WPA_PTK_STATE);
        WpaStateMachineInit(pAd, &pAd->Mlme.WpaPskMachine, pAd->Mlme.WpaPskFunc);

        ASSERT(DLS_FUNC_SIZE == MAX_DLS_MSG * MAX_DLS_STATE);
        DLSStateMachineInit(pAd, &pAd->Mlme.DlsMachine, pAd->Mlme.DlsFunc);
#ifdef WSC_SUPPORT
        // Init Wsc state machine		
		ASSERT(WSC_FUNC_SIZE == MAX_WSC_MSG * MAX_WSC_STATE);
		WscStateMachineInit(pAd, &pAd->Mlme.WscMachine, pAd->Mlme.WscFunc);
#endif // WSC_SUPPORT //

#ifdef APCLI_SUPPORT
		// init apcli state machines
		ASSERT(STA_AUTH_FUNC_SIZE == STA_MAX_AUTH_MSG * STA_MAX_AUTH_STATE);
		StaAuthStateMachineInit(pAd, &pAd->Mlme.ApCliAuthMachine, pAd->Mlme.ApCliAuthFunc);

		ASSERT(STA_ASSOC_FUNC_SIZE == STA_MAX_ASSOC_MSG * STA_MAX_ASSOC_STATE);
		StaAssocStateMachineInit(pAd, &pAd->Mlme.ApCliAssocMachine, pAd->Mlme.ApCliAssocFunc);

		ASSERT(STA_SYNC_FUNC_SIZE == STA_MAX_SYNC_MSG * STA_MAX_SYNC_STATE);
		StaSyncStateMachineInit(pAd, &pAd->Mlme.ApCliSyncMachine, pAd->Mlme.ApCliSyncFunc);

		ASSERT(STA_CTRL_FUNC_SIZE == STA_MAX_CTRL_MSG * STA_MAX_CTRL_STATE);
		StaCtrlStateMachineInit(pAd, &pAd->Mlme.ApCliCtrlMachine, pAd->Mlme.ApCliCtrlFunc);

		ASSERT(STA_WPA_PSK_FUNC_SIZE == STA_MAX_WPA_PSK_MSG * STA_MAX_WPA_PSK_STATE);
		StaWpaStateMachineInit(pAd, &pAd->Mlme.ApCliWpaPskMachine, pAd->Mlme.ApCliWpaPskFunc);
#endif

        // Init mlme periodic timer
		RTMPInitTimer(pAd, &pAd->Mlme.PeriodicTimer, (PVOID)&MlmePeriodicExec);
		RTMPAddTimer(&pAd->Mlme.PeriodicTimer, MLME_TASK_EXEC_INTV);
#ifdef GEM_LEDMODE
		pAd->Mlme.bLEDblinking = FALSE;
		RTMPInitTimer(pAd, &pAd->Mlme.LEDTimer, (PVOID)&LEDPeriodicExec);
		RTMPAddTimer(&pAd->Mlme.LEDTimer, LED_TASK_EXEC_INTV);
#endif
		// Init DFS periodic timer
		RTMPInitTimer(pAd, &pAd->Mlme.DFSPeriodicTimer, (PVOID)&RadarSMDetectPeriodic);
		RTMPAddTimer(&pAd->Mlme.DFSPeriodicTimer, HZ/10);

		// Init AP Beacon Update timer
		RTMPInitTimer(pAd, &pAd->Mlme.BeaconUpdateTimer, (PVOID)&BeaconUpdateExec);
		pAd->Mlme.bBeaconUpdate = 0;
#if 0
		// Fix WiFi WPA2 4.4.2 because beacon time is not accurate
		pAd->Mlme.TbttCount = 0;
#endif
    } while (FALSE);

    DBGPRINT(RT_DEBUG_TRACE, "<-- MLME Initialize\n");

    return Status;
}

/*
    ==========================================================================
    Description:
        main loop of the MLME
    Pre:
        Mlme has to be initialized, and there are something inside the queue
    Note:
        This function is invoked from MPSetInformation and MPReceive;
        This task guarantee only one MlmeHandler will run. 

    ==========================================================================
 */
VOID MlmeHandler(
    IN PRTMP_ADAPTER pAd) 
{
    MLME_QUEUE_ELEM	*Elem = NULL;
    UMAXLONG		IrqFlags;
#ifdef APCLI_SUPPORT
	SHORT apcliIfIndex;
#endif

    // Only accept MLME and Frame from peer side, no other (control/data) frame should
    // get into this state machine

    RTMP_SEM_LOCK(&pAd->Mlme.TaskLock, IrqFlags);
    if(pAd->Mlme.bRunning) 
    {
        RTMP_SEM_UNLOCK(&pAd->Mlme.TaskLock, IrqFlags);
        return;
    } 
    else 
    {
        pAd->Mlme.bRunning = TRUE;
    }
    RTMP_SEM_UNLOCK(&pAd->Mlme.TaskLock, IrqFlags);

    while (!MlmeQueueEmpty(&pAd->Mlme.Queue)) 
    {
        //From message type, determine which state machine I should drive
        if (MlmeDequeue(&pAd->Mlme.Queue, &Elem)) 
        {
            // if dequeue success
            switch (Elem->Machine) 
            {
                case ASSOC_STATE_MACHINE:
                    StateMachinePerformAction(pAd, &pAd->Mlme.AssocMachine, Elem);
                    break;
                case AUTH_STATE_MACHINE:
                    StateMachinePerformAction(pAd, &pAd->Mlme.AuthMachine, Elem);
                    break;
                case AUTH_RSP_STATE_MACHINE:
                    StateMachinePerformAction(pAd, &pAd->Mlme.AuthRspMachine, Elem);
                    break;
                case SYNC_STATE_MACHINE:
                    StateMachinePerformAction(pAd, &pAd->Mlme.SyncMachine, Elem);
                    break;
				case WPA_STATE_MACHINE:
					StateMachinePerformAction(pAd, &pAd->Mlme.WpaPskMachine, Elem);
					break;
				case DLS_STATE_MACHINE:
					StateMachinePerformAction(pAd, &pAd->Mlme.DlsMachine, Elem);
					break;
#ifdef WSC_SUPPORT
                case WSC_AP_STATE_MACHINE:
                    {
                        int                 HeaderLen = LENGTH_802_11 + LENGTH_802_1_H + sizeof(IEEE8021X_FRAME) + sizeof(EAP_FRAME);
                        PHEADER_802_11      pHeader;
						PMAC_TABLE_ENTRY    pEntry = NULL;
						int                 apidx = MAIN_MBSSID;
						
                        pHeader = (PHEADER_802_11)Elem->Msg;
                        
                        if (Elem->MsgType == WSC_EAPOL_PACKET_MSG)
                        {
    						if ((pEntry = MacTableLookup(pAd, pHeader->Addr2)))
                                apidx = pEntry->ApIdx;
                        }
						
						if((Elem->MsgType == WSC_EAPOL_UPNP_MSG) && (Elem->MsgLen > HeaderLen))
						{	//The WSC msg from UPnP daemon
							PUCHAR		pData;
							UCHAR 		MacAddr[MAC_ADDR_LEN]= {0};
							
							DBGPRINT(RT_DEBUG_INFO, "Receive a UPnP WSC Message!\n");
                            // Skip the (802.11 + 802.1h + 802.1x + EAP) header
                        	pData = (PUCHAR) &Elem->Msg[HeaderLen];
                            Elem->MsgLen -= HeaderLen;
							// The Addr1 of UPnP-Msg used to indicate the MAC address of the AP interface. Now always be ra0.
							NdisMoveMemory(MacAddr, pHeader->Addr1, MAC_ADDR_LEN);
							memmove(Elem->Msg, MacAddr, MAC_ADDR_LEN);
							memmove(Elem->Msg+6, pData, Elem->MsgLen);
							
							StateMachinePerformAction(pAd, &pAd->Mlme.WscMachine, Elem);
						}
                        else if ((Elem->MsgType == WSC_EAPOL_PACKET_MSG) && 
// For AP Client support WPS Modification
/*
								 (MAC_ADDR_EQUAL(pHeader->Addr2, pAd->PortCfg.MBSSID[apidx].WscControl.EntryAddr)) &&
								 (Elem->MsgLen > HeaderLen))
*/
								 ((MAC_ADDR_EQUAL(pHeader->Addr2, pAd->PortCfg.MBSSID[apidx].WscControl.EntryAddr))
#ifdef APCLI_SUPPORT
								 || (MAC_ADDR_EQUAL(pHeader->Addr2, pAd->ApCliTab.ApCliEntry[apidx].WscControl.EntryAddr))
#endif // APCLI_SUPPORT //
								 ))
// --
                        {   // WSC_STATE_MACHINE can service only one station at one time
                            PUCHAR		pData;
                            PEAP_FRAME  pEapFrame;
                            // Skip the EAP LLC header
                        	pData = (PUCHAR) &Elem->Msg[LENGTH_802_11 + LENGTH_802_1_H];
                            pEapFrame = (PEAP_FRAME)(pData + sizeof(IEEE8021X_FRAME));
                        	pData += sizeof(IEEE8021X_FRAME) + sizeof(EAP_FRAME);

// For AP Client support WPS Modification
#ifdef APCLI_SUPPORT
                            pAd->ApCliTab.ApCliEntry[apidx].WscControl.lastId = pEapFrame->Id;
                            if (pEapFrame->Code == EAP_CODE_FAIL)
                            { // EAP-Fail
                                UCHAR	fail_data[] = "EAP_FAIL";

                               NdisMoveMemory(Elem->Msg, pHeader->Addr2, MAC_ADDR_LEN);
                                NdisMoveMemory(Elem->Msg+6, fail_data, strlen(fail_data));
                                Elem->MsgLen = strlen(fail_data);
                                StateMachinePerformAction(pAd, &pAd->Mlme.WscMachine, Elem);
                                break;
                            }
                            else if ((pEapFrame->Code == EAP_CODE_REQ) && (pEapFrame->Type == EAP_TYPE_ID))
                            { // EAP-Req (Identity)
                                UCHAR	id_data[] = "hello";

                                NdisMoveMemory(Elem->Msg, pHeader->Addr2, MAC_ADDR_LEN);
                                NdisMoveMemory(Elem->Msg+6, id_data, strlen(id_data));
                                Elem->MsgLen = strlen(id_data);
                                StateMachinePerformAction(pAd, &pAd->Mlme.WscMachine, Elem);
                                break;
                            }
#endif // APCLI_SUPPORT //     
                            if (Elem->MsgLen <= HeaderLen)
                            {
                                DBGPRINT(RT_DEBUG_ERROR, "Elem->MsgLen(%ld) <= HeaderLen(%d) !!\n", Elem->MsgLen, HeaderLen);
                                break;
                            }
// --
                            Elem->MsgLen -= (LENGTH_802_11 + LENGTH_802_1_H + sizeof(IEEE8021X_FRAME) + sizeof(EAP_FRAME));
                            memmove(Elem->Msg, pHeader->Addr2, MAC_ADDR_LEN);
                            if (pEapFrame->Type == 1)
                            {
                                // add by johnli, bug fix: enable WPS will cause WPA/WPA2 Enterprise authentication failed
                                if (strstr(pData, "SimpleConfig"))
                                // end johnli
                                    memmove(Elem->Msg+6, pData, Elem->MsgLen);  // EAP-Rsp (Identity)
                                // add by johnli, bug fix: enable WPS will cause WPA/WPA2 Enterprise authentication failed
                                else
                                {
                                    DBGPRINT(RT_DEBUG_TRACE, "RTMPCancelTimer EapolTimer!!\n");
                                    pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.EapolTimerRunning = FALSE;
                                    pAd->PortCfg.MBSSID[apidx].WscControl.EntryApIdx = WSC_INIT_ENTRY_APIDX;
                                    NdisZeroMemory(pAd->PortCfg.MBSSID[apidx].WscControl.EntryAddr, MAC_ADDR_LEN);
                                    RTMPCancelTimer(&pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.EapolTimer);
                                    break;
                                }
                                // end johnli
                            }
                            else
                            {
                                if (WscCheckWSCHeader(pAd, pData))
                                {
// For AP Client support WPS Modification
#ifdef APCLI_SUPPORT
                                    if ((pEapFrame->Code == EAP_CODE_REQ) && 
                                        (pEapFrame->Type == EAP_TYPE_WSC))
                                    {
                                        PWSC_FRAME			pWsc = (PWSC_FRAME) pData;
                                        if (pWsc->OpCode == WSC_OPCODE_START)
                                        {
                                            UCHAR	wsc_start[] = "WSC_START";
                                            NdisMoveMemory(Elem->Msg+6, wsc_start, strlen(wsc_start));
                                            Elem->MsgLen = strlen(wsc_start);
                                        }
                                        else
                                        {
                                            pData += sizeof(WSC_FRAME);
                                            Elem->MsgLen -= sizeof(WSC_FRAME);
                                            NdisMoveMemory(Elem->Msg+6, pData, Elem->MsgLen);
                                        }
                                    }
                                    else
#endif // APCLI_SUPPORT //
// --
                                    { // EAP-Rsp (Messages)
                                        pData += sizeof(WSC_FRAME);
                                        Elem->MsgLen -= sizeof(WSC_FRAME);
                                        memmove(Elem->Msg+6, pData, Elem->MsgLen);
                                    }
                                }
                                else
                                {
                                    DBGPRINT(RT_DEBUG_TRACE, "ERROR: WscCheckWSCHeader() return FALSE!\n");
                                    break;
                                }
                            }
                            StateMachinePerformAction(pAd, &pAd->Mlme.WscMachine, Elem);
                        }
                        else if (Elem->MsgType == WSC_EAPOL_START_MSG)
					        StateMachinePerformAction(pAd, &pAd->Mlme.WscMachine, Elem);
                    }
					break;
#endif // WSC_SUPPORT //

#ifdef APCLI_SUPPORT
				case STA_AUTH_STATE_MACHINE:
					apcliIfIndex = Elem->ifIndex;
					if(isValidApCliIf(apcliIfIndex))
						StaStateMachinePerformAction(pAd, &pAd->Mlme.ApCliAuthMachine, Elem,
							apcliIfIndex, &(pAd->ApCliTab.ApCliEntry[apcliIfIndex].AuthCurrState));
					break;

				case STA_ASSOC_STATE_MACHINE:
					apcliIfIndex = Elem->ifIndex;
					if(isValidApCliIf(apcliIfIndex))		
						StaStateMachinePerformAction(pAd, &pAd->Mlme.ApCliAssocMachine, Elem,
							apcliIfIndex, &(pAd->ApCliTab.ApCliEntry[apcliIfIndex].AssocCurrState));
					break;

				case STA_SYNC_STATE_MACHINE:
					apcliIfIndex = Elem->ifIndex;
					if(isValidApCliIf(apcliIfIndex))
						StaStateMachinePerformAction(pAd, &pAd->Mlme.ApCliSyncMachine, Elem,
							apcliIfIndex, &(pAd->ApCliTab.ApCliEntry[apcliIfIndex].SyncCurrState));
					break;

				case STA_CTRL_STATE_MACHINE:
					apcliIfIndex = Elem->ifIndex;
					if(isValidApCliIf(apcliIfIndex))
						StaStateMachinePerformAction(pAd, &pAd->Mlme.ApCliCtrlMachine, Elem,
							apcliIfIndex, &(pAd->ApCliTab.ApCliEntry[apcliIfIndex].CtrlCurrState));
					break;

				case STA_WPA_STATE_MACHINE:
					apcliIfIndex = Elem->ifIndex;
					if(isValidApCliIf(apcliIfIndex))
						StaStateMachinePerformAction(pAd, &pAd->Mlme.ApCliWpaPskMachine, Elem,
							apcliIfIndex, &(pAd->ApCliTab.ApCliEntry[apcliIfIndex].WpaPskCurrState));
					break;	
#endif
                default:
                    DBGPRINT(RT_DEBUG_TRACE, "ERROR: Illegal machine in MlmeHandler()\n");
                    break;
            } // end of switch

            // free MLME element
            Elem->Occupied = FALSE;
            Elem->MsgLen = 0;
            
        }
        else {
            DBGPRINT_ERR("MlmeHandler: MlmeQueue empty\n");
        }
    }

    RTMP_SEM_LOCK(&pAd->Mlme.TaskLock, IrqFlags);
    pAd->Mlme.bRunning = FALSE;
    RTMP_SEM_UNLOCK(&pAd->Mlme.TaskLock, IrqFlags);
}

/*
    ==========================================================================
    Description:
        Destructor of MLME (Destroy queue, state machine, spin lock and timer)
    Parameters:
        Adapter - NIC Adapter pointer
    Post:
        The MLME task will no longer work properly
        
    ==========================================================================
 */
VOID MlmeHalt(
    IN PRTMP_ADAPTER pAd) 
{
    DBGPRINT(RT_DEBUG_TRACE, "==> MlmeHalt\n");

    // disable BEACON generation and other BEACON related hardware timers
    AsicDisableSync(pAd);
    
    // Cancel pending timers
    RTMPCancelTimer(&pAd->Mlme.BeaconUpdateTimer);
    RTMPCancelTimer(&pAd->Mlme.DFSPeriodicTimer);
    RTMPCancelTimer(&pAd->Mlme.PeriodicTimer);
    RTMPCancelTimer(&pAd->Mlme.SyncAux.ScanTimer);

#ifdef APCLI_SUPPORT
	RTMPCancelTimer(&pAd->MlmeAux.ProbeTimer);
	RTMPCancelTimer(&pAd->MlmeAux.ApCliAssocTimer);
	RTMPCancelTimer(&pAd->MlmeAux.ApCliAuthTimer);
#endif

    RTMPusecDelay(500000);    // 0.5 sec to guarantee timer canceled
    
    MlmeQueueDestroy(&pAd->Mlme.Queue);
    NdisFreeSpinLock(&pAd->Mlme.TaskLock);
    NdisFreeSpinLock(&pAd->Mlme.MemLock);

	DBGPRINT(RT_DEBUG_TRACE, "<== MlmeHalt\n");
}


#ifdef GEM_LEDMODE
VOID LEDPeriodicExec(
    IN	unsigned long data)
{
    RTMP_ADAPTER	*pAd = (RTMP_ADAPTER *)data;

    if (pAd->MacTab.Size)
    {
		if (pAd->Mlme.bLEDblinking == TRUE)
		{
	    	ASIC_LED_SET_BLINK(pAd);
		}
		else
		{
	    	ASIC_LED_SET_ON(pAd);
		}
    }
    else
    {
	    ASIC_LED_SET_OFF(pAd);
    }
    pAd->Mlme.bLEDblinking = FALSE;

    RTMPAddTimer(&pAd->Mlme.LEDTimer, LED_TASK_EXEC_INTV);
}
#endif


/*
    ==========================================================================
    Description:
        This routine is executed periodically to -
        1. Decide if it's a right time to turn on PwrMgmt bit of all 
           outgoiing frames
        2. Calculate ChannelQuality based on statistics of the last
           period, so that TX rate won't toggling very frequently between a 
           successful TX and a failed TX.
        3. If the calculated ChannelQuality indicated current connection not 
           healthy, then a ROAMing attempt is tried here.
        
    ==========================================================================
 */
VOID MlmePeriodicExec(
    IN	unsigned long data)
{
    RTMP_ADAPTER	*pAd = (RTMP_ADAPTER *)data;

    pAd->Mlme.Now32 = jiffies;

	// add the most up-to-date h/w raw counters into software variable, so that
    // the dynamic tuning mechanism below are based on most up-to-date information
    // This function call is also needed when ATE turns on to reflect the statistics counter
    NICUpdateRawCounters(pAd);

#ifdef RALINK_ATE
	if (pAd->ate.bRxFer == 1)
	{
		pAd->ate.RxTotalCnt += pAd->ate.RxCntPerSec;
		printk("MlmePeriodicExec: Rx packet cnt = %ld/%ld\n\n", pAd->ate.RxCntPerSec, pAd->ate.RxTotalCnt);
		pAd->ate.RxCntPerSec = 0;
	}
#endif

	if ((RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS)) || 
		(RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_RADIO_OFF)) ||
		(RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_RESET_IN_PROGRESS)))
	{
		RTMPAddTimer(&pAd->Mlme.PeriodicTimer, MLME_TASK_EXEC_INTV);
		return;
	}
	
#ifdef RALINK_ATE
	if(pAd->ate.Mode != ATE_APSTART)
	{
		RTMPAddTimer(&pAd->Mlme.PeriodicTimer, MLME_TASK_EXEC_INTV);
	    return;
	}
#endif	// RALINK_ATE
    
#ifdef DBG
{
	extern void dbg_AggregatedRatio(PRTMP_ADAPTER	pAd);
	dbg_AggregatedRatio(pAd);
}
#endif
	
	// check every 12 second. If no U2M in the past 12 second, then AvgRSSI is no longer a 
    // valid indication of the distance between this AP and its clients. In this case, we presume a
    // mid AvgRSSI (say -60 dbm), so that no extreme TX power calibration and BBP R17 tuning
    // rules will be applied.
    if (pAd->Mlme.PeriodicRound % 12 == 2)
    {
        if (pAd->PortCfg.NumOfAvgRssiSample == 0)
        {
        	pAd->PortCfg.AvgRssi = pAd->BbpRssiToDbmDelta + RSSI_FOR_MID_SENSIBILITY;
            pAd->PortCfg.AvgRssiX8 = pAd->PortCfg.AvgRssi << 3;
            pAd->PortCfg.AvgRssi2 = pAd->BbpRssiToDbmDelta + RSSI_FOR_MID_SENSIBILITY;
            pAd->PortCfg.AvgRssi2X8 = pAd->PortCfg.AvgRssi2 << 3;
            
            DBGPRINT(RT_DEBUG_TRACE,"MlmePeriodicExec: no traffic, Reset AsicBbpTuning\n");
            AsicResetBbpTuning(pAd);
        }
        else
        {
        	pAd->PortCfg.NumOfAvgRssiSample = 0;
        }
    }
	
	// Request by David
    //AsicAdjustTxPower(pAd);

    // danamic tune BBP R17 to find a balance between sensibility and noise isolation
    AsicBbpTuning(pAd);
    
	//radar detect
	if ((pAd->PortCfg.PhyMode == PHY_11A) && (pAd->PortCfg.RadarDetect.IEEE80211H == TRUE) && RadarChannelCheck(pAd, pAd->PortCfg.Channel))
		RadarDetectPeriodic(pAd);
    
	// walk through MAC table, see if switching TX rate is required
    if (OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_TX_RATE_SWITCH_ENABLED))
    	MlmeDynamicTxRateSwitching(pAd);

    // MAC table maintenance
    MacTableMaintenance(pAd);
    ApUpdateCapabilityAndErpIe(pAd);
    RTMPMaintainPMKIDCache(pAd);


#ifdef APCLI_SUPPORT
	// check every 2 second. If # of U2M or beacon, probe_rsp less than 1 times in the past 2 second,
	// then AvgRSSI is no longer a valid indication of the distance between this AP-Client and its AP.
	if (pAd->Mlme.PeriodicRound % 2 == 0)
	{
		ApCliIfMonitor(pAd);
	}

	if (pAd->Mlme.PeriodicRound % 2 == 1)
		ApCliIfUp(pAd);
#endif

    // clear all OneSecxxx counters.
    pAd->RalinkCounters.OneSecBeaconSentCnt = 0;
    pAd->RalinkCounters.OneSecRxFcsErrCnt = 0;
    pAd->RalinkCounters.OneSecRxOkCnt = 0;
    pAd->RalinkCounters.OneSecTxFailCount = 0;
    pAd->RalinkCounters.OneSecTxNoRetryOkCount = 0;
    pAd->RalinkCounters.OneSecTxRetryOkCount = 0;

    // TODO: for debug only. to be removed
    pAd->RalinkCounters.OneSecOsTxCount[QID_AC_BE] = 0;
    pAd->RalinkCounters.OneSecOsTxCount[QID_AC_BK] = 0;
    pAd->RalinkCounters.OneSecOsTxCount[QID_AC_VI] = 0;
    pAd->RalinkCounters.OneSecOsTxCount[QID_AC_VO] = 0;
    pAd->RalinkCounters.OneSecDmaDoneCount[QID_AC_BE] = 0;
    pAd->RalinkCounters.OneSecDmaDoneCount[QID_AC_BK] = 0;
    pAd->RalinkCounters.OneSecDmaDoneCount[QID_AC_VI] = 0;
    pAd->RalinkCounters.OneSecDmaDoneCount[QID_AC_VO] = 0;
    pAd->RalinkCounters.OneSecTxDoneCount = 0;
    pAd->RalinkCounters.OneSecTxAggregationCount = 0;
    pAd->RalinkCounters.OneSecRxAggregationCount = 0;

    pAd->Mlme.PeriodicRound ++;
	MlmeHandler(pAd);

	RTMPAddTimer(&pAd->Mlme.PeriodicTimer, MLME_TASK_EXEC_INTV);
}

/*
    ==========================================================================
    Description:
    
    Output:

        
    NOTE:
        call this routine every second
    ==========================================================================
 */
VOID MlmeDynamicTxRateSwitching(
    IN PRTMP_ADAPTER pAd)
{
	INT		i;
    UCHAR	UpRate, DownRate, CurrRate;
    UCHAR	PID;
    ULONG	AllTxTotalCnt = 0;
//iverson
    static int dfs_count=0;    
    //
    // walk through MAC table, see if need to change AP's TX rate toward each entry
    //
    for (i=0; i<MAX_LEN_OF_MAC_TABLE; i++)
    {
        USHORT TxTotalCnt, TxErrorRatio = 0;
        BOOLEAN fUpgradeQuality = FALSE;
        MAC_TABLE_ENTRY *pEntry = &pAd->MacTab.Content[i];

        // only associated STA counts
        if ((pEntry->Valid == FALSE) || (pEntry->Sst != SST_ASSOC))
            continue;

        if (pEntry->PsMode == PWR_SAVE)
            continue;

        pEntry->CurrTxRateStableTime ++;
        TxTotalCnt = pEntry->OneSecTxOkCount + pEntry->OneSecTxRetryOkCount + pEntry->OneSecTxFailCount;
        AllTxTotalCnt += TxTotalCnt;

        // skip those STA that has no traffic in the past period
        if (TxTotalCnt == 0)
        {
            pEntry->TxRateUpPenalty = 0;
            continue;
        }
        
        // decide the next upgrade rate and downgrade rate, if any
        CurrRate = pEntry->CurrTxRate;
        if (pAd->PortCfg.Channel > 14)  // must be in 802.11A band
        {
            if (Phy11ANextRateUpward[CurrRate] <= pEntry->MaxSupportedRate)
				UpRate = Phy11ANextRateUpward[CurrRate];
        	else
        		UpRate = CurrRate;
            DownRate = Phy11ANextRateDownward[CurrRate];
        }
        else
        {
            if (pEntry->MaxSupportedRate < RATE_FIRST_OFDM_RATE)
	        {
	        	if (Phy11BNextRateUpward[CurrRate] <= pEntry->MaxSupportedRate)
					UpRate = Phy11BNextRateUpward[CurrRate];
	        	else
	        		UpRate = CurrRate;
	            DownRate = Phy11BNextRateDownward[CurrRate];
	        }
	        else 
	        {
	        	if (Phy11BGNextRateUpward[CurrRate] <= pEntry->MaxSupportedRate)
					UpRate = Phy11BGNextRateUpward[CurrRate];
	        	else
	        		UpRate = CurrRate;
	            DownRate = Phy11BGNextRateDownward[CurrRate];
	        }
        }

        //
        // PART 1. Decide TX Quality
        //   decide TX quality based on Tx PER when enough samples are available
        //        
        if (TxTotalCnt > 15)
        {
            TxErrorRatio = ((pEntry->OneSecTxRetryOkCount + pEntry->OneSecTxFailCount) *100) / TxTotalCnt;

            // downgrade TX quality if PER >= Rate-Down threshold
            if (TxErrorRatio >= RateDownPER[CurrRate])
            {
                pEntry->TxQuality[CurrRate] = DRS_TX_QUALITY_WORST_BOUND;
            }
            // upgrade TX quality if PER <= Rate-Up threshold
            else if (TxErrorRatio <= RateUpPER[CurrRate])
            {
                fUpgradeQuality = TRUE;
                if (pEntry->TxQuality[CurrRate])
                    pEntry->TxQuality[CurrRate] --;  // quality very good in CurrRate

                if (pEntry->TxRateUpPenalty)
                    pEntry->TxRateUpPenalty --;
                else if (pEntry->TxQuality[UpRate])
                    pEntry->TxQuality[UpRate] --;    // may improve next UP rate's quality
            }
            
        }

        // if not enough TX samples, decide by heuristic rules
        else
        {
            TxErrorRatio = 0;
            
            // Downgrade TX quality upon any TX failure in the past second
            if (pEntry->OneSecTxFailCount)
            {
                if ((pEntry->OneSecTxFailCount <= 1) && 
                    (pEntry->OneSecTxOkCount + pEntry->OneSecTxRetryOkCount))
                {
                    pEntry->TxQuality[CurrRate] += 2;  // degrade quality
                    if (pEntry->TxQuality[CurrRate] > DRS_TX_QUALITY_WORST_BOUND)
                        pEntry->TxQuality[CurrRate] = DRS_TX_QUALITY_WORST_BOUND;
                }
                else // more than 2 failure, or no TX ok cases
                {
                    pEntry->TxQuality[CurrRate] = DRS_TX_QUALITY_WORST_BOUND;
                }
            }
            // upgrade TX quality if -
            // 1. no TX failure but do have TX ok case, and
            // 2. there's more one-time-ok cases than retry-ok cases in the past second
            else if (pEntry->OneSecTxOkCount > pEntry->OneSecTxRetryOkCount)
            {
                fUpgradeQuality = TRUE;
                if (pEntry->TxQuality[CurrRate])
                    pEntry->TxQuality[CurrRate] --;  // quality very good in CurrRate

                if (pEntry->TxRateUpPenalty)
                    pEntry->TxRateUpPenalty --;
                else if (pEntry->TxQuality[UpRate])
                    pEntry->TxQuality[UpRate] --;    // may improve next UP rate's quality

            }
        }

        pEntry->PER[CurrRate] = (UCHAR)TxErrorRatio;
        if (pEntry->fNoisyEnvironment)
        {
            DBGPRINT(RT_DEBUG_TRACE,"DRS(Noisy)-[AID# %d] Qty[%d]=%d PER=%d%% %dsec,Qty[%d]=%d Pty=%d,TX/RX Aggr=%ld,%ld\n", 
                pEntry->Aid, RateIdToMbps[CurrRate], pEntry->TxQuality[CurrRate],
                TxErrorRatio, pEntry->CurrTxRateStableTime,
                RateIdToMbps[UpRate], pEntry->TxQuality[UpRate],
                pEntry->TxRateUpPenalty,
                pAd->RalinkCounters.OneSecTxAggregationCount,
                pAd->RalinkCounters.OneSecRxAggregationCount);
        }
        else
        {
        	if ((pAd->Mlme.PeriodicRound % 5) == 1)
        	{
	            DBGPRINT(RT_DEBUG_TRACE,"DRS:[AID#%d] Qty[%d]=%d PER=%d%% %dsec,Qty[%d]=%d Pty=%d,TX/RX Aggr=%ld,%ld\n", 
	                pEntry->Aid, RateIdToMbps[CurrRate], pEntry->TxQuality[CurrRate],
	                TxErrorRatio, pEntry->CurrTxRateStableTime,
	                RateIdToMbps[UpRate], pEntry->TxQuality[UpRate],
	                pEntry->TxRateUpPenalty,
                	pAd->RalinkCounters.OneSecTxAggregationCount,
                	pAd->RalinkCounters.OneSecRxAggregationCount);
        	}
        }

        //
        // PART 2. Perform TX rate switching
        //   perform rate switching
        //
        
        do
        {
            // special case 1: enter NOISY environment
            if ((UpRate != CurrRate)                     &&
                (pEntry->LastSecTxRateChangeAction == 2) &&
                ((pEntry->PER[CurrRate] > 20) || pEntry->fNoisyEnvironment) &&
                (pEntry->PER[CurrRate] < 75)             &&
                ((pEntry->PER[CurrRate]+5) > pEntry->PER[UpRate]))
            {
                // we believe this is a noisy environment. better stay at UpRate
                pEntry->fNoisyEnvironment = TRUE;

                // 2004-3-14 when claiming noisy environment, we're not only switch back
                //   to UpRate, but can be more aggressive to use one more rate up
                UpRate++;
                if (UpRate>RATE_54) UpRate=RATE_54;
                if ((UpRate==RATE_6) || (UpRate==RATE_9)) UpRate=RATE_12;
                pEntry->LastTxRate = pEntry->CurrTxRate;//record last tx rate
                pEntry->CurrTxRate = UpRate;
                break;
            }
            
            // special case 2: leave NOISY environment
            if ((pEntry->fNoisyEnvironment == TRUE) &&
                (TxTotalCnt > 15)                   && 
                (pEntry->PER[CurrRate] <= 12))
            {
                UCHAR JumpUpRate;

                pEntry->fNoisyEnvironment = FALSE;
                for (JumpUpRate = RATE_54; JumpUpRate > RATE_1; JumpUpRate--)
                {
                	if ((pAd->RfIcType == RFIC_5325) || (pAd->RfIcType == RFIC_2529))
                	{
                		if (pAd->PortCfg.AvgRssi > pAd->PortCfg.AvgRssi2)
	                	{
	                		if (pAd->PortCfg.AvgRssi > (RssiSafeLevelForTxRate[JumpUpRate] + pAd->BbpRssiToDbmDelta))
		                        break;
	                	}
	                	else
	                	{
	                		if (pAd->PortCfg.AvgRssi2 > (RssiSafeLevelForTxRate[JumpUpRate] + pAd->BbpRssiToDbmDelta))
		                        break;
	                	}
                	}
                	else
                	{
                		if (pAd->PortCfg.AvgRssi > (RssiSafeLevelForTxRate[JumpUpRate] + pAd->BbpRssiToDbmDelta))
	                        break;
                	}
                }

                if (JumpUpRate > pEntry->MaxSupportedRate)
                    JumpUpRate = pEntry->MaxSupportedRate;
            
                if (JumpUpRate > CurrRate)
                {
                    pEntry->LastTxRate = pEntry->CurrTxRate;//record last tx rate
                    pEntry->CurrTxRate = JumpUpRate;
                    break;
                }
            }
            
            // we're going to ugrade CurrRate to UpRate at next few seconds, 
            // but before that, we'd better try a NULL frame @ UpRate and 
            // see if UpRate is stable or not. If this NULL frame fails, it will
            // downgrade TxQuality[CurrRate], so that STA won't switch to
            // to UpRate in the next second
            if ((fUpgradeQuality == TRUE)          &&
                (UpRate != CurrRate)               && 
                (pEntry->TxQuality[CurrRate] <= 1) &&
                (pEntry->TxQuality[UpRate] <= 1))
            {
                DBGPRINT(RT_DEBUG_TRACE,"2 test frames at UpRate = %d Mbps\n",RateIdToMbps[UpRate]);
                // tag PID for later on per-client/WDS TX statistics caculation
                PID = PTYPE_NULL_AT_HIGH_RATE + i;
                ApEnqueueNullFrame(pAd, pEntry->Addr, UpRate, PID, pEntry->ApIdx, FALSE, FALSE, 0);
                ApEnqueueNullFrame(pAd, pEntry->Addr, UpRate, PID, pEntry->ApIdx, FALSE, FALSE, 0);
            }

            // perform DRS - consider TxRate Down frist, then rate up
            if ((pEntry->TxQuality[CurrRate] >= DRS_TX_QUALITY_WORST_BOUND) && (CurrRate != DownRate))
            {
                pEntry->LastTxRate = pEntry->CurrTxRate;//record last tx rate
                pEntry->CurrTxRate = DownRate;
            }
            else if ((pEntry->TxQuality[CurrRate] <=0) && (pEntry->TxQuality[UpRate] <=0) && (CurrRate != UpRate))
            {
                pEntry->LastTxRate = pEntry->CurrTxRate;//record last tx rate
                pEntry->CurrTxRate = UpRate;
            }
        }while (FALSE);

        // PART 3. Post-processing if TX rate switching did happen
        //     if rate-up happen, clear all bad history of all TX rates
        //     if rate-down happen, only clear DownRate's bad history
        if (pEntry->CurrTxRate > CurrRate)
        {
            DBGPRINT(RT_DEBUG_TRACE,"DRS: [AID#%d] ++TX rate = %d Mbps\n", pEntry->Aid, RateIdToMbps[UpRate]);
            pEntry->CurrTxRateStableTime = 0;
            pEntry->TxRateUpPenalty = 0;
            pEntry->LastSecTxRateChangeAction = 0;
            NdisZeroMemory(pEntry->TxQuality, MAX_LEN_OF_SUPPORTED_RATES);
            NdisZeroMemory(pEntry->PER, MAX_LEN_OF_SUPPORTED_RATES);
            if (!pAd->QuickResponeForRateUpTimerRunning)
			{
				if (pEntry->CurrTxRate <= RATE_12)
					RTMPAddTimer(&pAd->QuickResponeForRateUpTimer, (200 * HZ)/1000);
				else
					RTMPAddTimer(&pAd->QuickResponeForRateUpTimer, (100 * HZ)/1000);
				pAd->QuickResponeForRateUpTimerRunning = TRUE;
			}
        }
        else if (pEntry->CurrTxRate < CurrRate)
        {
            DBGPRINT(RT_DEBUG_TRACE,"DRS: [AID#%d] --TX rate = %d Mbps\n", pEntry->Aid, RateIdToMbps[DownRate]);

            // shorter stable time require more penalty in next rate UP criteria
//            if (pEntry->CurrTxRateStableTime < 4)      // less then 4 sec
//                pEntry->TxRateUpPenalty = DRS_PENALTY; // add 8 sec penalty
//            else if (pEntry->CurrTxRateStableTime < 8) // less then 8 sec
//                pEntry->TxRateUpPenalty = 2;           // add 2 sec penalty
//            else
                pEntry->TxRateUpPenalty = 0;           // no penalty

            pEntry->CurrTxRateStableTime = 0;
            pEntry->LastSecTxRateChangeAction = 2; // rate down
            pEntry->TxQuality[pEntry->CurrTxRate] = 0;
            pEntry->PER[pEntry->CurrTxRate] = 0;
        }
        else
            pEntry->LastSecTxRateChangeAction = 0; // rate no change
        
        // reset all OneSecxxx counters
        pEntry->OneSecTxFailCount = 0;
        pEntry->OneSecTxOkCount = 0;
        pEntry->OneSecTxRetryOkCount = 0;
    }

#ifdef WDS_SUPPORT
	// WDS tx rate tuning
	if (pAd->WdsTab.Mode != WDS_DISABLE_MODE)
    {
	    for (i=0; i<MAX_WDS_ENTRY; i++)
	    {
	        USHORT TxTotalCnt, TxErrorRatio;
	        BOOLEAN fUpgradeQuality = FALSE;
	        RT_802_11_WDS_ENTRY *pEntry = &pAd->WdsTab.WdsEntry[i];

	        if (pEntry->Valid == FALSE)
	            continue;

	        pEntry->CurrTxRateStableTime ++;
	        TxTotalCnt = pEntry->OneSecTxOkCount + pEntry->OneSecTxRetryOkCount + pEntry->OneSecTxFailCount;
	        AllTxTotalCnt += TxTotalCnt;

	        // Lazy WDS age out time is 10 sec
	        if ((pAd->WdsTab.Mode >= WDS_LAZY_MODE) && (pEntry->WdsRcvBeaconTime + (10 * HZ) < jiffies) && (!pEntry->RestrictEntry))
	        {
	            DBGPRINT(RT_DEBUG_TRACE, "Delete WDS entry(%02x:%02x:%02x:%02x:%02x:%02x), due to age out\n", 
	            	pEntry->WdsAddr[0],pEntry->WdsAddr[1],pEntry->WdsAddr[2],pEntry->WdsAddr[3],pEntry->WdsAddr[4],pEntry->WdsAddr[5]);
	            pEntry->Valid = FALSE;
	            continue;
	        }
	        
	        // skip those STA that has no traffic in the past period
	        if (TxTotalCnt == 0)
	        {
	            pEntry->TxRateUpPenalty = 0;
	            continue;
	        }

	        // decide the next upgrade rate and downgrade rate, if any
	        CurrRate = pEntry->CurrTxRate;
	        if (pAd->PortCfg.Channel > 14)  // must be in 802.11A band
	        {
	            if (Phy11ANextRateUpward[CurrRate] <= pEntry->MaxSupportedRate)
					UpRate = Phy11ANextRateUpward[CurrRate];
	        	else
	        		UpRate = CurrRate;
	            DownRate = Phy11ANextRateDownward[CurrRate];
	        }
	        else
	        {
	            if (pEntry->MaxSupportedRate < RATE_FIRST_OFDM_RATE)
		        {
		        	if (Phy11BNextRateUpward[CurrRate] <= pEntry->MaxSupportedRate)
						UpRate = Phy11BNextRateUpward[CurrRate];
		        	else
		        		UpRate = CurrRate;
		            DownRate = Phy11BNextRateDownward[CurrRate];
		        }
		        else 
		        {
		        	if (Phy11BGNextRateUpward[CurrRate] <= pEntry->MaxSupportedRate)
						UpRate = Phy11BGNextRateUpward[CurrRate];
		        	else
		        		UpRate = CurrRate;
		            DownRate = Phy11BGNextRateDownward[CurrRate];
		        }
	        }

	        // calculate Tx with retry ratio when enough samples are available
	        if (TxTotalCnt > 15)
	        {
	            TxErrorRatio = ((pEntry->OneSecTxRetryOkCount + pEntry->OneSecTxFailCount) *100) / TxTotalCnt;

	            // downgrade TX quality if retry+error ratio reached
	            if (TxErrorRatio >= RateDownPER[CurrRate])
	            {
	                pEntry->TxQuality[CurrRate] = DRS_TX_QUALITY_WORST_BOUND;
	            }
	            // upgrade TX quality if retry+error ratio reached
	            else if (TxErrorRatio <= RateUpPER[CurrRate])
	            {
	                fUpgradeQuality = TRUE;
	                if (pEntry->TxQuality[CurrRate])
	                    pEntry->TxQuality[CurrRate] --;  // quality very good in CurrRate

	                if (pEntry->TxRateUpPenalty)
	                    pEntry->TxRateUpPenalty --;
	                else if (pEntry->TxQuality[UpRate])
	                    pEntry->TxQuality[UpRate] --;    // may improve next UP rate's quality
	            }
	        }
	        else
	        {
	            TxErrorRatio = 0;  // too few samples
	        
	            // Downgrade TX quality upon any TX failure in the past second
	            if (pEntry->OneSecTxFailCount)
	            {
	                if ((pEntry->OneSecTxFailCount <= 1) && (pEntry->OneSecTxOkCount + pEntry->OneSecTxRetryOkCount))
	                {
	                    pEntry->TxQuality[CurrRate] += 2;  // degrade quality
	                    if (pEntry->TxQuality[CurrRate] > DRS_TX_QUALITY_WORST_BOUND)
	                        pEntry->TxQuality[CurrRate] = DRS_TX_QUALITY_WORST_BOUND;
	                }
	                else // more than 2 failure, or no TX ok cases
	                {
	                    pEntry->TxQuality[CurrRate] = DRS_TX_QUALITY_WORST_BOUND;
	                }
	            }
	            // upgrade TX quality if -
	            // 1. no TX failure but do have TX ok case, and
	            // 2. there's more one-time-ok cases than retry-ok cases in the past second
	            else if (pEntry->OneSecTxOkCount > pEntry->OneSecTxRetryOkCount)
	            {
	                fUpgradeQuality = TRUE;
	                if (pEntry->TxQuality[CurrRate])
	                    pEntry->TxQuality[CurrRate] --;  // quality very good in CurrRate

	                if (pEntry->TxRateUpPenalty)
	                    pEntry->TxRateUpPenalty --;
	                else if (pEntry->TxQuality[UpRate])
	                    pEntry->TxQuality[UpRate] --;    // may improve next UP rate's quality

	            }
	        }    

	        DBGPRINT(RT_DEBUG_TRACE,"WDS: Qty[%d]=%d PER=%d%% %d-sec, Qty[%d]=%d Pty=%d\n",
	            RateIdToMbps[CurrRate], pEntry->TxQuality[CurrRate],
	            TxErrorRatio, pEntry->CurrTxRateStableTime,
	            RateIdToMbps[UpRate], pEntry->TxQuality[UpRate],
	            pEntry->TxRateUpPenalty);
	        
	        // we're going to ugrade CurrRate to UpRate at next few seconds, 
	        // but before that, we'd better try a NULL frame @ UpRate and 
	        // see if UpRate is stable or not. If this NULL frame fails, it will
	        // downgrade TxQuality[CurrRate], so that STA won't switch to
	        // to UpRate in the next second
	        if ((fUpgradeQuality == TRUE)          &&
	            (UpRate != CurrRate)               && 
	            (pEntry->TxQuality[CurrRate] <= 1) &&
	            (pEntry->TxQuality[UpRate] <= 1))
	        {
	            DBGPRINT(RT_DEBUG_TRACE,"2 test frames at UpRate = %d Mbps\n",RateIdToMbps[UpRate]);
	            // tag PID for later on per-client/WDS TX statistics caculation
	            PID = PTYPE_NULL_AT_HIGH_RATE + WDS_PAIRWISE_KEY_OFFSET + i; 
	            ApEnqueueNullFrame(pAd, pEntry->WdsAddr, UpRate, PID, MAIN_MBSSID, FALSE, FALSE, 0);
	            ApEnqueueNullFrame(pAd, pEntry->WdsAddr, UpRate, PID, MAIN_MBSSID, FALSE, FALSE, 0);
	        }

	        // perform DRS - consider TxRate Down frist, then rate up
	        //     rate down, if current TX rate's quality is not good
	        //     rate up, if UPRate's quality is very good
	        if ((pEntry->TxQuality[CurrRate] >= DRS_TX_QUALITY_WORST_BOUND) && (CurrRate != DownRate))
	        {
	            pEntry->CurrTxRate = DownRate;
	        }
	        else if ((pEntry->TxQuality[CurrRate] <=0) && (pEntry->TxQuality[UpRate] <=0) && (CurrRate != UpRate))
	        {
	            pEntry->CurrTxRate = UpRate;
	        }

	        // if rate-up happen, clear all bad history of all TX rates
	        if (pEntry->CurrTxRate > CurrRate)
	        {
	            DBGPRINT(RT_DEBUG_TRACE,"WDS: ++TX rate = %d Mbps\n", RateIdToMbps[UpRate]);
	            pEntry->CurrTxRateStableTime = 0;
	            pEntry->TxRateUpPenalty = 0;
	            NdisZeroMemory(pEntry->TxQuality, MAX_LEN_OF_SUPPORTED_RATES);
	        }
	        // if rate-down happen, only clear DownRate's bad history
	        else if (pEntry->CurrTxRate < CurrRate)
	        {
	            DBGPRINT(RT_DEBUG_TRACE,"WDS: --TX rate = %d Mbps\n", RateIdToMbps[DownRate]);

	            // shorter stable time require more penalty in next rate UP criteria
	            if (pEntry->CurrTxRateStableTime < 4)      // less then 4 sec
	                pEntry->TxRateUpPenalty = DRS_PENALTY; // add 8 sec penalty
	            else if (pEntry->CurrTxRateStableTime < 8) // less then 8 sec
	                pEntry->TxRateUpPenalty = 2;           // add 2 sec penalty
	            else
	                pEntry->TxRateUpPenalty = 0;           // no penalty

	            pEntry->CurrTxRateStableTime = 0;
	            pEntry->TxQuality[pEntry->CurrTxRate] = 0;
	        }
	        
	        // reset all OneSecxxx counters
	        pEntry->OneSecTxFailCount = 0;
	        pEntry->OneSecTxOkCount = 0;
	        pEntry->OneSecTxRetryOkCount = 0;
	    }
	}
#endif /* WDS_SUPPORT */

#ifdef APCLI_SUPPORT
	// AP Client tx rate tuning
	for(i=0; i<MAX_APCLI_ENTRY; i++)
	{
		USHORT TxTotalCnt, TxErrorRatio;
		BOOLEAN fUpgradeQuality = FALSE;
		RT_802_11_APCLI_ENTRY *pEntry = &pAd->ApCliTab.ApCliEntry[i];

		if(pEntry->Valid == FALSE || pEntry->Enable == FALSE)
			continue;

		pEntry->CurrTxRateStableTime ++;
		TxTotalCnt = pEntry->OneSecTxOkCount + pEntry->OneSecTxRetryOkCount + pEntry->OneSecTxFailCount;
		AllTxTotalCnt += TxTotalCnt;

		// skip those STA that has no traffic in the past period
		if(TxTotalCnt == 0)
		{
			pEntry->TxRateUpPenalty = 0;
			continue;
		}

		// decide the next upgrade rate and downgrade rate, if any
		CurrRate = pEntry->CurrTxRate;
		if(pAd->PortCfg.Channel > 14)  // must be in 802.11A band
		{
			if(Phy11ANextRateUpward[CurrRate] <= pEntry->MaxSupportedRate)
				UpRate = Phy11ANextRateUpward[CurrRate];
			else
				UpRate = CurrRate;
			DownRate = Phy11ANextRateDownward[CurrRate];
		} else
		{
			if(pEntry->MaxSupportedRate < RATE_FIRST_OFDM_RATE)
			{
				if(Phy11BNextRateUpward[CurrRate] <= pEntry->MaxSupportedRate)
					UpRate = Phy11BNextRateUpward[CurrRate];
				else
					UpRate = CurrRate;
				DownRate = Phy11BNextRateDownward[CurrRate];
			} else
			{
				if(Phy11BGNextRateUpward[CurrRate] <= pEntry->MaxSupportedRate)
					UpRate = Phy11BGNextRateUpward[CurrRate];
				else
					UpRate = CurrRate;
				DownRate = Phy11BGNextRateDownward[CurrRate];
			}
		}

		// calculate Tx with retry ratio when enough samples are available
		if(TxTotalCnt > 15)
		{
			TxErrorRatio = ((pEntry->OneSecTxRetryOkCount + pEntry->OneSecTxFailCount) *100) / TxTotalCnt;

			// downgrade TX quality if retry+error ratio reached
			if(TxErrorRatio >= RateDownPER[CurrRate])
			{
				pEntry->TxQuality[CurrRate] = DRS_TX_QUALITY_WORST_BOUND;
			}
			// upgrade TX quality if retry+error ratio reached
			else if(TxErrorRatio <= RateUpPER[CurrRate])
			{
				fUpgradeQuality = TRUE;
				if(pEntry->TxQuality[CurrRate])
					pEntry->TxQuality[CurrRate] --;	 // quality very good in CurrRate

				if(pEntry->TxRateUpPenalty)
					pEntry->TxRateUpPenalty --;
				else if(pEntry->TxQuality[UpRate])
					pEntry->TxQuality[UpRate] --;	 // may improve next UP rate's quality
			}
		} else
		{
			TxErrorRatio = 0;  // too few samples

			// Downgrade TX quality upon any TX failure in the past second
			if(pEntry->OneSecTxFailCount)
			{
				if((pEntry->OneSecTxFailCount <= 1) && (pEntry->OneSecTxOkCount + pEntry->OneSecTxRetryOkCount))
				{
					pEntry->TxQuality[CurrRate] += 2;  // degrade quality
					if(pEntry->TxQuality[CurrRate] > DRS_TX_QUALITY_WORST_BOUND)
						pEntry->TxQuality[CurrRate] = DRS_TX_QUALITY_WORST_BOUND;
				} else // more than 2 failure, or no TX ok cases
				{
					pEntry->TxQuality[CurrRate] = DRS_TX_QUALITY_WORST_BOUND;
				}
			}
			// upgrade TX quality if -
			// 1. no TX failure but do have TX ok case, and
			// 2. there's more one-time-ok cases than retry-ok cases in the past second
			else if(pEntry->OneSecTxOkCount > pEntry->OneSecTxRetryOkCount)
			{
				fUpgradeQuality = TRUE;
				if(pEntry->TxQuality[CurrRate])
					pEntry->TxQuality[CurrRate] --;	 // quality very good in CurrRate

				if(pEntry->TxRateUpPenalty)
					pEntry->TxRateUpPenalty --;
				else if(pEntry->TxQuality[UpRate])
					pEntry->TxQuality[UpRate] --;	 // may improve next UP rate's quality

			}
		}    

		DBGPRINT(RT_DEBUG_TRACE,"APCLI: Qty[%d]=%d PER=%d%% %d-sec, Qty[%d]=%d Pty=%d\n",
			RateIdToMbps[CurrRate], pEntry->TxQuality[CurrRate],
			TxErrorRatio, pEntry->CurrTxRateStableTime,
			RateIdToMbps[UpRate], pEntry->TxQuality[UpRate],
			pEntry->TxRateUpPenalty);

		// we're going to ugrade CurrRate to UpRate at next few seconds, 
		// but before that, we'd better try a NULL frame @ UpRate and 
		// see if UpRate is stable or not. If this NULL frame fails, it will
		// downgrade TxQuality[CurrRate], so that STA won't switch to
		// to UpRate in the next second
		if((fUpgradeQuality == TRUE)          &&
			(UpRate != CurrRate)               && 
			(pEntry->TxQuality[CurrRate] <= 1) &&
			(pEntry->TxQuality[UpRate] <= 1))
		{
			DBGPRINT(RT_DEBUG_TRACE,"2 test frames at UpRate = %d Mbps\n",RateIdToMbps[UpRate]);
			// tag PID for later on per-client/APCLI TX statistics caculation
			PID = PTYPE_NULL_AT_HIGH_RATE + APCLI_PAIRWISE_KEY_OFFSET + i; 
			ApEnqueueNullFrame(pAd, pEntry->ApCliBssid, UpRate, PID, MAIN_MBSSID, FALSE, FALSE, 0);
			ApEnqueueNullFrame(pAd, pEntry->ApCliBssid, UpRate, PID, MAIN_MBSSID, FALSE, FALSE, 0);
		}

		// perform DRS - consider TxRate Down frist, then rate up
		//     rate down, if current TX rate's quality is not good
		//     rate up, if UPRate's quality is very good
		if((pEntry->TxQuality[CurrRate] >= DRS_TX_QUALITY_WORST_BOUND) && (CurrRate != DownRate))
		{
			pEntry->CurrTxRate = DownRate;
		} else if((pEntry->TxQuality[CurrRate] <=0) && (pEntry->TxQuality[UpRate] <=0) && (CurrRate != UpRate))
		{
			pEntry->CurrTxRate = UpRate;
		}

		// if rate-up happen, clear all bad history of all TX rates
		if(pEntry->CurrTxRate > CurrRate)
		{
			DBGPRINT(RT_DEBUG_TRACE,"APCLI: ++TX rate = %d Mbps\n", RateIdToMbps[UpRate]);
			pEntry->CurrTxRateStableTime = 0;
			pEntry->TxRateUpPenalty = 0;
			NdisZeroMemory(pEntry->TxQuality, MAX_LEN_OF_SUPPORTED_RATES);
		}
		// if rate-down happen, only clear DownRate's bad history
		else if(pEntry->CurrTxRate < CurrRate)
		{
			DBGPRINT(RT_DEBUG_TRACE,"APCLI: --TX rate = %d Mbps\n", RateIdToMbps[DownRate]);

			// shorter stable time require more penalty in next rate UP criteria
			if(pEntry->CurrTxRateStableTime < 4)	  // less then 4 sec
				pEntry->TxRateUpPenalty = DRS_PENALTY; // add 8 sec penalty
			else if(pEntry->CurrTxRateStableTime < 8) // less then 8 sec
				pEntry->TxRateUpPenalty = 2;		   // add 2 sec penalty
			else
				pEntry->TxRateUpPenalty	= 0;		   // no penalty

			pEntry->CurrTxRateStableTime = 0;
			pEntry->TxQuality[pEntry->CurrTxRate] = 0;
		}

		// reset all OneSecxxx counters
		pEntry->OneSecTxFailCount = 0;
		pEntry->OneSecTxOkCount = 0;
		pEntry->OneSecTxRetryOkCount = 0;
	}
#endif /* APCLI_SUPPORT */

#if 0
	//if (pAd->Mlme.PeriodicRound % 5 == 2)
	{
		if ((pAd->PortCfg.PhyMode == PHY_11A) && (pAd->PortCfg.RadarDetect.IEEE80211H == TRUE) && RadarChannelCheck(pAd, pAd->PortCfg.Channel) && (pAd->PortCfg.RadarDetect.RDMode == RD_NORMAL_MODE))
		{
			UCHAR	Pattern = 0;
			
			if (AllTxTotalCnt > 400)
			{
				Pattern = 3;
			}
			else if (AllTxTotalCnt > 200)
			{
				Pattern = 2;
			}
			else if (AllTxTotalCnt > 50)
			{
				Pattern = 1;
			}
			else
			{
				Pattern = 0;
			}

			if (Pattern != pAd->PortCfg.RadarDetect.DetectPattern)
			{
				AsicSendCommandToMcu(pAd, 0x60, 0xff, 0x01, Pattern);
			}
			pAd->PortCfg.RadarDetect.DetectPattern = Pattern;
		}
	}
#endif

	DBGPRINT(RT_DEBUG_INFO,"AllTxTotalCnt=%d, OneSecRxOkCnt=%ld, dlstest=%d\n", AllTxTotalCnt, pAd->RalinkCounters.OneSecRxOkCnt, pAd->dfstest);

	// AP should not send any packet out during Carrier-Signal exist.
	// Those commands below will let Mac start sending CTS for Radar-Detection.
	if ( (pAd->PortCfg.RadarDetect.RDMode != RD_NORMAL_MODE)
#ifdef CARRIER_DETECTION_SUPPORT
		|| (isCarrierDetectExist(pAd) ==  TRUE)
#endif // CARRIER_DETECTION_SUPPORT //
#ifdef APCLI_SUPPORT
		// ApCli will dynamic switch to new channel specifed in SWITCH-ANNOUNCEMNET IE.
		// So Our AP leave the DFS function to ApCli while ApCli interface conntected to a root-AP.
		|| (pAd->ApCliTab.ApCliEntry[0].Valid == TRUE)
#endif // APCLI_SUPPORT //
		)
	{
		return;
	}

		if ((pAd->PortCfg.PhyMode == PHY_11A) && (pAd->PortCfg.RadarDetect.IEEE80211H == TRUE) && RadarChannelCheck(pAd, pAd->PortCfg.Channel) && (pAd->PortCfg.RadarDetect.RDMode == RD_NORMAL_MODE))
		{
		if (pAd->dfstest == 0)
		{
//iverson
                	if(pAd->PortCfg.AvgRssi < 56){
 	    			 AsicSendCommandToMcu(pAd, 0x60, 0xff, 0x01, 0x03);
                	}
			else if (((AllTxTotalCnt+pAd->RalinkCounters.OneSecRxOkCnt) > 1101) || (pAd->RalinkCounters.OneSecRxOkCnt > 801))
			{
				AsicSendCommandToMcu(pAd, 0x60, 0xff, 0x01, 0x03);
			}
			else if (((AllTxTotalCnt+pAd->RalinkCounters.OneSecRxOkCnt) > 1100) || (pAd->RalinkCounters.OneSecRxOkCnt > 800))
			{
				AsicSendCommandToMcu(pAd, 0x60, 0xff, 0x01, 0x02);
			}
			else if ((AllTxTotalCnt > 300) || (pAd->RalinkCounters.OneSecRxOkCnt > 200))
			{
                                if(dfs_count < 4)
				{ 
				AsicSendCommandToMcu(pAd, 0x60, 0xff, 0x01, 0x01);
				dfs_count++;
				}
				else
				{
				 AsicSendCommandToMcu(pAd, 0x60, 0xff, 0x01, 0x02);
				 dfs_count = 0;
				}

			}
			else
			{
				// reserve the 1st CTS Frame buffer for Carrier-Sense. change command from 0x00 to 0x01.
				AsicSendCommandToMcu(pAd, 0x60, 0xff, 0x01, 0x01/*0x00*/);
		}
	}
	else
	{
		AsicSendCommandToMcu(pAd, 0x60, 0xff, 0x01, 0x01);
	}
	}
}

VOID MlmeSetTxPreamble(
    IN PRTMP_ADAPTER pAd, 
    IN USHORT TxPreamble)
{
    TXRX_CSR4_STRUC csr4;
    
    //
	// Please refer to windows driver
	//
	//TxPreamble = Rt802_11PreambleLong;
    
    RTMP_IO_READ32(pAd, TXRX_CSR4, &csr4.word);
    if (TxPreamble == Rt802_11PreambleShort)
    {
        // NOTE: 1Mbps should always use long preamble
        DBGPRINT(RT_DEBUG_TRACE, "MlmeSetTxPreamble (= SHORT PREAMBLE)\n");
        pAd->PortCfg.TxPreambleInUsed = Rt802_11PreambleShort;
        csr4.field.AutoResponderPreamble = 1;
    }
    else
    {
        DBGPRINT(RT_DEBUG_TRACE, "MlmeSetTxPreamble (= LONG PREAMBLE)\n");
        pAd->PortCfg.TxPreambleInUsed = Rt802_11PreambleLong;
        csr4.field.AutoResponderPreamble = 0;
    }
    RTMP_IO_WRITE32(pAd, TXRX_CSR4, csr4.word);
}
    
// bLinkUp is to identify the inital link speed.
// TRUE indicates the rate update at linkup, we should not try to set the rate at 54Mbps.
VOID MlmeUpdateTxRates(
    IN PRTMP_ADAPTER pAd)
{
    int i, j, num;
    ULONG  BasicRateBitmap = 0;
    UCHAR CurrBasicRate = RATE_1;
//    UCHAR AutoRespTxPower = pAd->LatchBBPR94;

    // update basic rate bitmap according to pAdapter->PortCfg.BasicRateBitMap
    UpdateBasicRateBitmap(pAd);

	for (j=0; j<pAd->PortCfg.BssidNum; j++)
	{
		UCHAR Rate, MaxDesire = RATE_1, MaxSupport = RATE_1; //, MaxBasicRate = RATE_1;
		
	    // find max desired rate
	    num = 0;
	    for (i=0; i<MAX_LEN_OF_SUPPORTED_RATES; i++)
	    {
	        switch (pAd->PortCfg.MBSSID[j].DesiredRates[i] & 0x7f)
	        {
	            case 2:  Rate = RATE_1;   num++;   break;
	            case 4:  Rate = RATE_2;   num++;   break;
	            case 11: Rate = RATE_5_5; num++;   break;
	            case 22: Rate = RATE_11;  num++;   break;
	            case 12: Rate = RATE_6;   num++;   break;
	            case 18: Rate = RATE_9;   num++;   break;
	            case 24: Rate = RATE_12;  num++;   break;
	            case 36: Rate = RATE_18;  num++;   break;
	            case 48: Rate = RATE_24;  num++;   break;
	            case 72: Rate = RATE_36;  num++;   break;
	            case 96: Rate = RATE_48;  num++;   break;
	            case 108: Rate = RATE_54; num++;   break;
	            default: Rate = RATE_1;   break;
	        }
	        if (MaxDesire < Rate)  MaxDesire = Rate;
	    }

	    // Auto rate switching is enabled only if more than one DESIRED RATES are 
	    // specified; otherwise disabled
	    if (num <= 1)
	        OPSTATUS_CLEAR_FLAG(pAd, fOP_STATUS_TX_RATE_SWITCH_ENABLED);
	    else
	        OPSTATUS_SET_FLAG(pAd, fOP_STATUS_TX_RATE_SWITCH_ENABLED);

	    // find max supported rate
	    for (i=0; i<pAd->PortCfg.SupportedRatesLen; i++)
	    {
	        switch (pAd->PortCfg.SupportedRates[i] & 0x7f)
	        {
	            case 2: Rate = RATE_1;   
	                    if (pAd->PortCfg.SupportedRates[i] & 0x80) 
	                        BasicRateBitmap |= 0x0001;  
	                    break;
	            case 4: Rate = RATE_2;   
	                    if (pAd->PortCfg.SupportedRates[i] & 0x80) 
	                        BasicRateBitmap |= 0x0002;  
	                    break;
	            case 11: 
	                    Rate = RATE_5_5; 
	                    if (pAd->PortCfg.SupportedRates[i] & 0x80) 
	                        BasicRateBitmap |= 0x0004;  
	                    break;
	            case 22: 
	                    Rate = RATE_11;  
	                    if (pAd->PortCfg.SupportedRates[i] & 0x80) 
	                        BasicRateBitmap |= 0x0008;  
	                    break;
	            case 12: 
	                    Rate = RATE_6;   
	//                    if (pAd->PortCfg.SupportedRates[i] & 0x80) 
	                        BasicRateBitmap |= 0x0010;  
	                    break;
	            case 18: 
	                    Rate = RATE_9;   
	                    if (pAd->PortCfg.SupportedRates[i] & 0x80) 
	                        BasicRateBitmap |= 0x0020;  
	                    break;
	            case 24: 
	                    Rate = RATE_12;  
	//                    if (pAd->PortCfg.SupportedRates[i] & 0x80) 
	                        BasicRateBitmap |= 0x0040;  
	                    break;
	            case 36: 
	                    Rate = RATE_18;  
	                    if (pAd->PortCfg.SupportedRates[i] & 0x80) 
	                        BasicRateBitmap |= 0x0080;  
	                    break;
	            case 48: 
	                    Rate = RATE_24;
	//                    if (pAd->PortCfg.SupportedRates[i] & 0x80) 
	                        BasicRateBitmap |= 0x0100;
	                    break;
	            case 72: 
	                    Rate = RATE_36;  
	                    if (pAd->PortCfg.SupportedRates[i] & 0x80) 
	                        BasicRateBitmap |= 0x0200;  
	                    break;
	            case 96: 
	                    Rate = RATE_48;  
	                    if (pAd->PortCfg.SupportedRates[i] & 0x80) 
	                        BasicRateBitmap |= 0x0400;  
	                    break;
	            case 108: 
	                    Rate = RATE_54; 
	                    if (pAd->PortCfg.SupportedRates[i] & 0x80) 
	                        BasicRateBitmap |= 0x0800;  
	                    break;
	            default:  
	                    Rate = RATE_1;   
	                    break;
	        }
	        if (MaxSupport < Rate)  MaxSupport = Rate;

	        // decide max basic rate, which is used to send MLME frames
	//      if ((pAd->PortCfg.SupportedRates[i] & 0x80) && (MaxBasicRate < Rate))
	//      {
	            // use only 11b CCK as max basic rate when operate in B/G mixed mode, B-only mode
	//          if (Rate < RATE_FIRST_OFDM_RATE)  MaxBasicRate = Rate;
	//      }
	    }

	    // max tx rate = min {max desire rate, max supported rate}
	    if (MaxSupport < MaxDesire)
	        pAd->PortCfg.MBSSID[j].MaxTxRate = pAd->PortCfg.MBSSID[j].TxRate = MaxSupport;
	    else
	        pAd->PortCfg.MBSSID[j].MaxTxRate = pAd->PortCfg.MBSSID[j].TxRate = MaxDesire;
#if 0
		// determine Auto Responder Tx Power
		if (pAd->TxPowerDelta.field.Enable == 1)
		{
		    if (pAd->TxPowerDelta.field.Sign == 1)
		    {
		    	if ((pAd->LatchBBPR94 + pAd->TxPowerDelta.field.Delta) < 12)
		    		AutoRespTxPower = pAd->LatchBBPR94 + pAd->TxPowerDelta.field.Delta;
		    	else
		    		AutoRespTxPower = 12;
		    }
		    else
		    {
		    	if ((pAd->LatchBBPR94 - pAd->TxPowerDelta.field.Delta) > 0)
			    	AutoRespTxPower = pAd->LatchBBPR94 - pAd->TxPowerDelta.field.Delta;
		    	else
		    		AutoRespTxPower = 0;
		    }
		}
		else
		{
			AutoRespTxPower = DEFAULT_BBP_TX_POWER;
		}
#endif
	//  pAd->PortCfg.MaxBasicRate = MaxBasicRate;
//        RTMP_IO_WRITE32(pAd, TXRX_CSR5, (BasicRateBitmap | (AutoRespTxPower << 16)));
		RTMP_IO_WRITE32(pAd, TXRX_CSR5, BasicRateBitmap);

	    // calculate the exptected ACK rate for each TX rate. This info is used to caculate
	    // the DURATION field of outgoing uniicast DATA/MGMT frame
	    for (i=0; i<MAX_LEN_OF_SUPPORTED_RATES; i++)
	    {
	        if (BasicRateBitmap & (0x01 << i))
	            CurrBasicRate = (UCHAR)i;
	        pAd->PortCfg.ExpectedACKRate[i] = CurrBasicRate;
	        DBGPRINT(RT_DEBUG_INFO,"Expected ACK rate for %d Mbps = %d Mbps\n", RateIdToMbps[i], RateIdToMbps[CurrBasicRate]);
	    }

	    switch (pAd->PortCfg.PhyMode)
	    {
	        case PHY_11BG_MIXED:
	        case PHY_11B:
	        case PHY_11G:
	            pAd->PortCfg.MlmeRate = RATE_1;	//pAd->PortCfg.MlmeRate = RATE_2;(To let beacon transmit using RATE_1)
	            pAd->PortCfg.RtsRate = RATE_11;
	            break;
	        case PHY_11A:
	            pAd->PortCfg.MlmeRate = RATE_6;
	            pAd->PortCfg.RtsRate = RATE_6;
	            break;
	        default: // error
	            pAd->PortCfg.MlmeRate = RATE_2;
	            pAd->PortCfg.RtsRate = RATE_2;
	            break;
	    }
	    
	    DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) MlmeUpdateTxRates (MaxDesire=%d Mbps, MaxSupport=%d Mbps, MaxTxRate=%d Mbps, Rate Switching =%d)\n", j,
	             RateIdToMbps[MaxDesire], RateIdToMbps[MaxSupport], RateIdToMbps[pAd->PortCfg.MBSSID[j].MaxTxRate], OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_TX_RATE_SWITCH_ENABLED));
	}

    DBGPRINT(RT_DEBUG_TRACE, " MlmeUpdateTxRates (RtsRate=%d Mbps, MlmeRate=%d Mbps, BasicRateBitmap=0x%04x)\n", 
         RateIdToMbps[pAd->PortCfg.RtsRate], RateIdToMbps[pAd->PortCfg.MlmeRate], BasicRateBitmap);
}

/*! \brief init the management mac frame header
 *  \param p_hdr mac header
 *  \param subtype subtype of the frame
 *  \param p_ds destination address, don't care if it is a broadcast address
 *  \return none
 *  \pre the station has the following information in the pAd->UserCfg
 *   - bssid
 *   - station address
 *  \post
 *  \note this function initializes the following field
 */
VOID MgtMacHeaderInit(
    IN	PRTMP_ADAPTER	pAd, 
    IN OUT PHEADER_802_11 pHdr80211, 
    IN UCHAR SubType, 
    IN UCHAR ToDs, 
    IN PUCHAR pDA, 
    IN PUCHAR pBssid) 
{
    NdisZeroMemory(pHdr80211, sizeof(HEADER_802_11));
    pHdr80211->FC.Type = BTYPE_MGMT;
    pHdr80211->FC.SubType = SubType;
    pHdr80211->FC.ToDs = ToDs;
    COPY_MAC_ADDR(pHdr80211->Addr1, pDA);
    COPY_MAC_ADDR(pHdr80211->Addr2, pBssid);
    COPY_MAC_ADDR(pHdr80211->Addr3, pBssid);
}

#ifdef APCLI_SUPPORT
/*! \brief init the management mac frame header
 *  \param p_hdr mac header
 *  \param subtype subtype of the frame
 *  \param p_ds destination address, don't care if it is a broadcast address
 *  \return none
 *  \pre the station has the following information in the pAd->UserCfg
 *   - bssid
 *   - station address
 *  \post
 *  \note this function initializes the following field
 */
VOID ApCliMgtMacHeaderInit(
    IN	PRTMP_ADAPTER	pAd, 
    IN OUT PHEADER_802_11 pHdr80211, 
    IN UCHAR SubType, 
    IN UCHAR ToDs, 
    IN PUCHAR pDA, 
    IN PUCHAR pBssid,
    IN USHORT ifIndex)
{
    NdisZeroMemory(pHdr80211, sizeof(HEADER_802_11));
    pHdr80211->FC.Type = BTYPE_MGMT;
    pHdr80211->FC.SubType = SubType;
    pHdr80211->FC.ToDs = ToDs;
    COPY_MAC_ADDR(pHdr80211->Addr1, pDA);
    COPY_MAC_ADDR(pHdr80211->Addr2, pAd->ApCliTab.ApCliEntry[ifIndex].CurrentAddress);
    COPY_MAC_ADDR(pHdr80211->Addr3, pBssid);
}
#endif

// ===========================================================================================
// mem_mgmt.c
// ===========================================================================================

/*!***************************************************************************
 * This routine build an outgoing frame, and fill all information specified 
 * in argument list to the frame body. The actual frame size is the summation 
 * of all arguments.
 * input params:
 *      Buffer - pointer to a pre-allocated memory segment
 *      args - a list of <int arg_size, arg> pairs.
 *      NOTE NOTE NOTE!!!! the last argument must be NULL, otherwise this
 *                         function will FAIL!!!
 * return:
 *      Size of the buffer
 * usage:  
 *      MakeOutgoingFrame(Buffer, output_length, 2, &fc, 2, &dur, 6, p_addr1, 6,p_addr2, END_OF_ARGS);
 ****************************************************************************/
ULONG MakeOutgoingFrame(
    OUT CHAR *Buffer, 
    OUT ULONG *FrameLen, ...) 
{
    CHAR   *p;
    int     leng;
    ULONG   TotLeng;
    va_list Args;

    // calculates the total length
    TotLeng = 0;
    va_start(Args, FrameLen);
    do 
    {
        leng = va_arg(Args, int);
        if (leng == END_OF_ARGS) 
        {
            break;
        }
        p = va_arg(Args, PVOID);
        NdisMoveMemory(&Buffer[TotLeng], p, leng);
        TotLeng = TotLeng + leng;
    } while(TRUE);

    va_end(Args); /* clean up */
    *FrameLen = TotLeng;
    return TotLeng;
}

// ===========================================================================================
// mlme_queue.c
// ===========================================================================================

/*! \brief  Initialize The MLME Queue, used by MLME Functions
 *  \param  *Queue     The MLME Queue
 *  \return Always     Return NDIS_STATE_SUCCESS in this implementation
 *  \pre
 *  \post
 *  \note   Because this is done only once (at the init stage), no need to be locked
 */
NDIS_STATUS MlmeQueueInit(
    IN MLME_QUEUE *Queue) 
{
    INT i;

    NdisAllocateSpinLock(&Queue->Lock);

    Queue->Num  = 0;
    Queue->Head = 0;
    Queue->Tail = 0;

    for (i = 0; i < MAX_LEN_OF_MLME_QUEUE; i++) 
    {
        Queue->Entry[i].Occupied = FALSE;
        Queue->Entry[i].MsgLen = 0;
        NdisZeroMemory(Queue->Entry[i].Msg, MAX_LEN_OF_MLME_BUFFER);
    }

    return NDIS_STATUS_SUCCESS;
}


/*! \brief   Enqueue a message for other threads, if they want to send messages to MLME thread
 *  \param  *Queue    The MLME Queue
 *  \param   Machine  The State Machine Id
 *  \param   MsgType  The Message Type
 *  \param   MsgLen   The Message length
 *  \param  *Msg      The message pointer
 *  \return  TRUE if enqueue is successful, FALSE if the queue is full
 *  \pre
 *  \post
 *  \note    The message has to be initialized
 */
BOOLEAN MlmeEnqueue(
	IN	PRTMP_ADAPTER	pAd,
    IN ULONG Machine, 
    IN ULONG MsgType, 
    IN ULONG MsgLen, 
    IN VOID *Msg) 
{
    INT Tail;
    UMAXLONG	IrqFlags;
	MLME_QUEUE	*Queue = (MLME_QUEUE *)&pAd->Mlme.Queue;

    // Do nothing if the driver is starting halt state.
    // This might happen when timer already been fired before cancel timer with mlmehalt
    if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS))
        return FALSE;

	// First check the size, it MUST not exceed the mlme queue size
	if (MsgLen > MAX_LEN_OF_MLME_BUFFER)
	{
        DBGPRINT_ERR("MlmeEnqueue: msg too large, size = %d \n", MsgLen);
		return FALSE;
	}
	
    if (MlmeQueueFull(Queue)) 
    {
        DBGPRINT_ERR("MlmeEnqueue: full, msg dropped and may corrupt MLME\n");
        return FALSE;
    }

    RTMP_SEM_LOCK(&(Queue->Lock), IrqFlags);
    Tail = Queue->Tail;
    Queue->Tail++;
    Queue->Num++;
    if (Queue->Tail == MAX_LEN_OF_MLME_QUEUE) 
    {
        Queue->Tail = 0;
    }
    DBGPRINT(RT_DEBUG_INFO, "MlmeEnqueue, num=%d\n",Queue->Num);
 
    Queue->Entry[Tail].Occupied = TRUE;
    Queue->Entry[Tail].Machine = Machine;
    Queue->Entry[Tail].MsgType = MsgType;
    Queue->Entry[Tail].MsgLen  = MsgLen;
    if (Msg != NULL)
        NdisMoveMemory(Queue->Entry[Tail].Msg, Msg, MsgLen);

    RTMP_SEM_UNLOCK(&(Queue->Lock), IrqFlags);

    return TRUE;
}

/*! \brief   This function is used when Recv gets a MLME message
 *  \param  *Queue           The MLME Queue
 *  \param   TimeStampHigh   The upper 32 bit of timestamp
 *  \param   TimeStampLow    The lower 32 bit of timestamp
 *  \param   Rssi            The receiving RSSI strength
 *  \param   MsgLen          The length of the message
 *  \param  *Msg             The message pointer
 *  \return  TRUE if everything ok, FALSE otherwise (like Queue Full)
 *  \pre
 *  \post
 */
BOOLEAN MlmeEnqueueForRecv(
    IN	PRTMP_ADAPTER	pAd, 
    IN ULONG TimeStampHigh, 
    IN ULONG TimeStampLow,
    IN UCHAR Rssi, 
    IN ULONG MsgLen, 
    IN VOID *Msg,
    IN UCHAR Offset) 
{
    INT          Tail, Machine;
    PFRAME_802_11 pFrame = (PFRAME_802_11)Msg;
    INT          MsgType;
	MLME_QUEUE	*Queue = (MLME_QUEUE *)&pAd->Mlme.Queue;
	UMAXLONG	IrqFlags;
	BOOLEAN  MsgSubstResult;

    // Do nothing if the driver is starting halt state.
    // This might happen when timer already been fired before cancel timer with mlmehalt
    if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS))
        return FALSE;

	// First check the size, it MUST not exceed the mlme queue size
	if (MsgLen > MAX_LEN_OF_MLME_BUFFER)
	{
        DBGPRINT_ERR("MlmeEnqueueForRecv: frame too large, size = %d \n", MsgLen);
		return FALSE;
	}

    if (MlmeQueueFull(Queue)) 
    {
//        DBGPRINT_ERR("MlmeEnqueueForRecv: full and dropped\n");
        return FALSE;
    }

#ifdef APCLI_SUPPORT
	// Beacon must be handled by ap-sync state machine.
	// Probe-rsp must be handled by apcli-sync state machine.
	// Those packets don't need to check its MAC address.
	MsgSubstResult = preCheckMsgTypeSubset(pAd, pFrame, &Machine, &MsgType, Offset);

	if(!MsgSubstResult)
    {
		if(MAC_ADDR_EQUAL(pAd->MlmeAux.Bssid, pFrame->Hdr.Addr2))
			MsgSubstResult = ApCliMsgTypeSubst(pAd, pFrame, &Machine, &MsgType, Offset);
		else
			MsgSubstResult = MsgTypeSubst(pAd, pFrame, &Machine, &MsgType, Offset);
	}
#else
	MsgSubstResult = MsgTypeSubst(pAd, pFrame, &Machine, &MsgType, Offset);
#endif

    if (!MsgSubstResult) 
    {
        DBGPRINT_ERR("MlmeEnqueueForRecv: un-recongnized mgmt->subtype=%d, Addr2: %02x-%02x-%02x-%02x-%02x-%02x\n",pFrame->Hdr.FC.SubType,
        					pFrame->Hdr.Addr2[0],pFrame->Hdr.Addr2[1],pFrame->Hdr.Addr2[2],
        					pFrame->Hdr.Addr2[3],pFrame->Hdr.Addr2[4],pFrame->Hdr.Addr2[5]);
        return FALSE;
    }
    
    // OK, we got all the informations, it is time to put things into queue
    RTMP_SEM_LOCK(&(Queue->Lock), IrqFlags);
    Tail = Queue->Tail;
    Queue->Tail++;
    Queue->Num++;
    if (Queue->Tail == MAX_LEN_OF_MLME_QUEUE) 
    {
        Queue->Tail = 0;
    }
    RTMP_SEM_UNLOCK(&(Queue->Lock), IrqFlags);
    DBGPRINT(RT_DEBUG_INFO, "MlmeEnqueueForRecv, num=%d\n",Queue->Num);
    
    Queue->Entry[Tail].Occupied = TRUE;
    Queue->Entry[Tail].Machine = Machine;
    Queue->Entry[Tail].MsgType = MsgType;
    Queue->Entry[Tail].MsgLen  = MsgLen;
    Queue->Entry[Tail].TimeStamp.vv.LowPart = TimeStampLow;
    Queue->Entry[Tail].TimeStamp.vv.HighPart = TimeStampHigh;
    Queue->Entry[Tail].Rssi = Rssi;
#ifdef APCLI_SUPPORT
	Queue->Entry[Tail].ifIndex = ApCliIfLookUp(pAd, Msg);
#endif
    NdisMoveMemory(Queue->Entry[Tail].Msg, Msg, MsgLen);
    
    MlmeHandler(pAd);

    return TRUE;
}


#ifdef WSC_SUPPORT
/*! \brief   Enqueue a message for other threads, if they want to send messages to MLME thread
 *  \param  *Queue    The MLME Queue
 *  \param   TimeStampLow    The lower 32 bit of timestamp, here we used for eventID.
 *  \param   Machine  The State Machine Id
 *  \param   MsgType  The Message Type
 *  \param   MsgLen   The Message length
 *  \param  *Msg      The message pointer
 *  \return  TRUE if enqueue is successful, FALSE if the queue is full
 *  \pre
 *  \post
 *  \note    The message has to be initialized
 */
BOOLEAN MlmeEnqueueForWsc(
	IN	PRTMP_ADAPTER	pAd,
	IN ULONG eventID,
	IN LONG senderID,
	IN ULONG Machine, 
	IN ULONG MsgType, 
	IN ULONG MsgLen, 
	IN VOID *Msg) 
{
    INT Tail;
    UMAXLONG	IrqFlags;
	MLME_QUEUE	*Queue = (MLME_QUEUE *)&pAd->Mlme.Queue;

	DBGPRINT(RT_DEBUG_TRACE, "-----> MlmeEnqueueForWsc\n");
    // Do nothing if the driver is starting halt state.
    // This might happen when timer already been fired before cancel timer with mlmehalt
    if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS))
        return FALSE;

	// First check the size, it MUST not exceed the mlme queue size
	if (MsgLen > MAX_LEN_OF_MLME_BUFFER)
	{
        DBGPRINT_ERR("MlmeEnqueueForWsc: msg too large, size = %d \n", MsgLen);
		return FALSE;
	}
	
    if (MlmeQueueFull(Queue)) 
    {
        DBGPRINT_ERR("MlmeEnqueueForWsc: full, msg dropped and may corrupt MLME\n");
        return FALSE;
    }

    RTMP_SEM_LOCK(&(Queue->Lock), IrqFlags);
    Tail = Queue->Tail;
    Queue->Tail++;
    Queue->Num++;
    if (Queue->Tail == MAX_LEN_OF_MLME_QUEUE) 
    {
        Queue->Tail = 0;
    }
    DBGPRINT(RT_DEBUG_INFO, "MlmeEnqueueForWsc, num=%d\n",Queue->Num);
 
    Queue->Entry[Tail].Occupied = TRUE;
    Queue->Entry[Tail].Machine = Machine;
    Queue->Entry[Tail].MsgType = MsgType;
    Queue->Entry[Tail].MsgLen  = MsgLen;
	Queue->Entry[Tail].TimeStamp.vv.LowPart = eventID;
	Queue->Entry[Tail].TimeStamp.vv.HighPart = senderID;
    if (Msg != NULL)
        NdisMoveMemory(Queue->Entry[Tail].Msg, Msg, MsgLen);

    RTMP_SEM_UNLOCK(&(Queue->Lock), IrqFlags);

	DBGPRINT(RT_DEBUG_TRACE, "<----- MlmeEnqueueForWsc\n");
	
    return TRUE;
}


#endif // WSC_SUPPORT //

/*! \brief   Dequeue a message from the MLME Queue
 *  \param  *Queue    The MLME Queue
 *  \param  *Elem     The message dequeued from MLME Queue
 *  \return  TRUE if the Elem contains something, FALSE otherwise
 *  \pre
 *  \post
 */
BOOLEAN MlmeDequeue(
    IN MLME_QUEUE *Queue, 
    OUT MLME_QUEUE_ELEM **Elem) 
{
	UMAXLONG	IrqFlags;
	
    RTMP_SEM_LOCK(&(Queue->Lock), IrqFlags);
    *Elem = &(Queue->Entry[Queue->Head]);
    Queue->Num--;
    Queue->Head++;
    if (Queue->Head == MAX_LEN_OF_MLME_QUEUE) 
    {
        Queue->Head = 0;
    }
    RTMP_SEM_UNLOCK(&(Queue->Lock), IrqFlags);
    DBGPRINT(RT_DEBUG_INFO, "MlmeDequeue, num=%d\n",Queue->Num);

    return TRUE;
}

/*! \brief  test if the MLME Queue is empty
 *  \param  *Queue    The MLME Queue
 *  \return TRUE if the Queue is empty, FALSE otherwise
 *  \pre
 *  \post
 */
BOOLEAN MlmeQueueEmpty(
    IN MLME_QUEUE *Queue) 
{
    BOOLEAN		Ans;
    UMAXLONG	IrqFlags;

    RTMP_SEM_LOCK(&(Queue->Lock), IrqFlags);
    Ans = (Queue->Num == 0);
    RTMP_SEM_UNLOCK(&(Queue->Lock), IrqFlags);

    return Ans;
}

/*! \brief   test if the MLME Queue is full
 *  \param   *Queue      The MLME Queue
 *  \return  TRUE if the Queue is empty, FALSE otherwise
 *  \pre
 *  \post
 */
BOOLEAN MlmeQueueFull(
    IN MLME_QUEUE *Queue) 
{
    BOOLEAN Ans;
    UMAXLONG		IrqFlags;

    RTMP_SEM_LOCK(&(Queue->Lock), IrqFlags);
    Ans = (Queue->Num == MAX_LEN_OF_MLME_QUEUE);
    RTMP_SEM_UNLOCK(&(Queue->Lock), IrqFlags);

    return Ans;
}

/*! \brief   The destructor of MLME Queue
 *  \param 
 *  \return
 *  \pre
 *  \post
 *  \note   Clear Mlme Queue, Set Queue->Num to Zero.
 */
VOID MlmeQueueDestroy(
    IN MLME_QUEUE *pQueue) 
{
	UMAXLONG		IrqFlags;
	
    RTMP_SEM_LOCK(&(pQueue->Lock), IrqFlags);
    pQueue->Num  = 0;
    pQueue->Head = 0;
    pQueue->Tail = 0;
    RTMP_SEM_UNLOCK(&(pQueue->Lock), IrqFlags);
    NdisFreeSpinLock(&(pQueue->Lock));
}

/*! \brief   To substitute the message type if the message is coming from external
 *  \param  pFrame         The frame received
 *  \param  *Machine       The state machine
 *  \param  *MsgType       the message type for the state machine
 *  \return TRUE if the substitution is successful, FALSE otherwise
 *  \pre
 *  \post
 */
BOOLEAN MsgTypeSubst(
    IN PRTMP_ADAPTER  pAd,
    IN PFRAME_802_11 pFrame, 
    OUT INT *Machine, 
    OUT INT *MsgType,
    IN UCHAR Offset) 
{
    USHORT Seq;
    UCHAR 	    EAPType, EAPCode; 
    BOOLEAN     Return = FALSE;
#ifdef WSC_SUPPORT
    PMAC_TABLE_ENTRY pEntry;
#endif // WSC_SUPPORT //

// only PROBE_REQ can be broadcast, all others must be unicast-to-me && is_mybssid; otherwise, 
// ignore this frame

    // EAPOL PACKET
    if (pFrame->Hdr.FC.Type == BTYPE_DATA) 
    {    
#ifdef WSC_SUPPORT    
        //WSC EAPOL PACKET
        
        pEntry = MacTableLookup(pAd, pFrame->Hdr.Addr2);
        if (pEntry && pAd->PortCfg.MBSSID[pEntry->ApIdx].WscControl.WscConfMode != WSC_DISABLE)
        {
            *Machine = WSC_AP_STATE_MACHINE;
            EAPType = *((UCHAR*)pFrame+LENGTH_802_11+LENGTH_802_1_H+1+Offset);
            EAPCode = *((UCHAR*)pFrame + LENGTH_802_11 + LENGTH_802_1_H + 4);
            Return = WscMsgTypeSubst(EAPType, EAPCode, (ULONG *)MsgType);
            
        }
        if (!Return)
#endif // WSC_SUPPORT //
        {

            *Machine = WPA_STATE_MACHINE;
            EAPType = *((UCHAR*)pFrame+LENGTH_802_11+LENGTH_802_1_H+1+Offset);
            Return = WPAMsgTypeSubst(EAPType, (ULONG *)MsgType);
        }
        return Return;

    }
    else if (pFrame->Hdr.FC.Type == BTYPE_MGMT) 		
    {
    	switch (pFrame->Hdr.FC.SubType) 
    	{
			case SUBTYPE_ASSOC_REQ:
				*Machine = ASSOC_STATE_MACHINE;
				*MsgType = MT2_PEER_ASSOC_REQ;
				break;
	       	case SUBTYPE_REASSOC_REQ:
				*Machine = ASSOC_STATE_MACHINE;
				*MsgType = MT2_PEER_REASSOC_REQ;
				break;
        	case SUBTYPE_PROBE_REQ:
				*Machine = SYNC_STATE_MACHINE;				
				*MsgType = MT2_PEER_PROBE_REQ;
            	break;
        	case SUBTYPE_BEACON:
				*Machine = SYNC_STATE_MACHINE;
				*MsgType = MT2_PEER_BEACON;
            	break;
        	case SUBTYPE_DISASSOC:
				*Machine = ASSOC_STATE_MACHINE;
				*MsgType = MT2_PEER_DISASSOC_REQ;
				break;
        	case SUBTYPE_AUTH:
	            // get the sequence number from payload 24 Mac Header + 2 bytes algorithm
				NdisMoveMemory(&Seq, &pFrame->Octet[2], sizeof(USHORT));
				if (Seq == 1 || Seq == 3) 
				{
					*Machine = AUTH_RSP_STATE_MACHINE;
					*MsgType = MT2_PEER_AUTH_ODD;
				} 
				else 
				{
					DBGPRINT(RT_DEBUG_TRACE,"wrong AUTH seq=%d\n",Seq);
					return FALSE;
				}
				break;
        	case SUBTYPE_DEAUTH:
				*Machine = AUTH_RSP_STATE_MACHINE;
				*MsgType = MT2_PEER_DEAUTH;
				break;
        	case SUBTYPE_ACTION:
				if (pFrame->Octet[0] == CATEGORY_DLS)
				{
					if (pFrame->Octet[1] == ACTION_DLS_REQUEST)
					{
						*Machine = DLS_STATE_MACHINE;
						*MsgType = MT2_PEER_DLS_REQ;
					}
					else if (pFrame->Octet[1] == ACTION_DLS_RESPONSE)
					{
						*Machine = DLS_STATE_MACHINE;
						*MsgType = MT2_PEER_DLS_RSP;
					}
					else if (pFrame->Octet[1] == ACTION_DLS_TEARDOWN)
					{
						*Machine = DLS_STATE_MACHINE;
						*MsgType = MT2_PEER_DLS_TEAR_DOWN;
					}
					else
					{
						DBGPRINT(RT_DEBUG_TRACE,"CATEGORY_DLS not support(action=%d)\n",pFrame->Octet[1]);
						return FALSE;
					}
					return TRUE;
				}
//				else if (pFrame->Octet[0] == CATEGORY_QOS)
//				{
//				}
				else
				{
					DBGPRINT(RT_DEBUG_TRACE,"SUBTYPE_ACTION not support(category=%d)\n",pFrame->Octet[0]);
					return FALSE;
				}
				break;
        	default:
				return FALSE;
    	}

   		return TRUE;
    }

    return FALSE;
}

#ifdef APCLI_SUPPORT
/*! \brief   To substitute the message type if the message is coming from external
 *  \param  pFrame         The frame received
 *  \param  *Machine       The state machine
 *  \param  *MsgType       the message type for the state machine
 *  \return TRUE if the substitution is successful, FALSE otherwise
 *  \pre
 *  \post
 */
BOOLEAN ApCliMsgTypeSubst(
	IN PRTMP_ADAPTER  pAd,
	IN PFRAME_802_11 pFrame, 
	OUT INT *Machine, 
	OUT INT *MsgType,
	IN UCHAR Offset)
{
	USHORT 		Seq;
    UCHAR 		EAPType; 
    BOOLEAN     Return = FALSE;
// For AP Client support WPS Modification
#ifdef WSC_SUPPORT
	UCHAR EAPCode;
#endif // WSC_SUPPORT //
// --

	// only PROBE_REQ can be broadcast, all others must be unicast-to-me && is_mybssid; otherwise, 
	// ignore this frame

    // WPA EAPOL PACKET
    if (pFrame->Hdr.FC.Type == BTYPE_DATA)
    {    
// For AP Client support WPS Modification
#ifdef WSC_SUPPORT
       //WSC EAPOL PACKET        
        if (pAd->ApCliTab.ApCliEntry[0].WscControl.WscConfMode == WSC_AP_Enrollee)
        {
            *Machine = WSC_AP_STATE_MACHINE;
            EAPType = *((UCHAR*)pFrame+LENGTH_802_11+LENGTH_802_1_H+1);
            EAPCode = *((UCHAR*)pFrame + LENGTH_802_11 + LENGTH_802_1_H + 4);
            Return = WscMsgTypeSubst(EAPType, EAPCode, MsgType);
        }
        if (!Return)
#endif // WSC_SUPPORT //
// --
	{
		*Machine = STA_WPA_STATE_MACHINE;
		EAPType = *((UCHAR*)pFrame + LENGTH_802_11 + LENGTH_802_1_H + 1);
		Return = StaWPAMsgTypeSubst(EAPType, MsgType);
	}
        return Return;
    }
	else if (pFrame->Hdr.FC.Type == BTYPE_MGMT) 		
	{
		switch (pFrame->Hdr.FC.SubType) 
		{
			case SUBTYPE_ASSOC_RSP:
				*Machine = STA_ASSOC_STATE_MACHINE;
				*MsgType = STA_MT2_PEER_ASSOC_RSP;
				break;

// For AP Client support WPS Modification
			case SUBTYPE_DISASSOC:
				*Machine = STA_ASSOC_STATE_MACHINE;
				*MsgType = STA_MT2_PEER_DISASSOC_REQ;
				break;
// --

        	case SUBTYPE_DEAUTH:
				*Machine = STA_AUTH_STATE_MACHINE;
				*MsgType = STA_MT2_PEER_DEAUTH;
				break;

			case SUBTYPE_AUTH:
				// get the sequence number from payload 24 Mac Header + 2 bytes algorithm
				NdisMoveMemory(&Seq, &pFrame->Octet[2], sizeof(USHORT));
				if (Seq == 2 || Seq == 4)
				{
					*Machine = STA_AUTH_STATE_MACHINE;
					*MsgType = STA_MT2_PEER_AUTH_EVEN;
				}
				else 
				{
					return FALSE;
				}
				break;

			default:
				return FALSE;
		}

		return TRUE;
	}

	return FALSE;
}

BOOLEAN preCheckMsgTypeSubset(
	IN PRTMP_ADAPTER  pAd,
	IN PFRAME_802_11 pFrame, 
	OUT INT *Machine, 
	OUT INT *MsgType,
	IN UCHAR Offset)
{
	if (pFrame->Hdr.FC.Type == BTYPE_MGMT) 		
	{
		switch (pFrame->Hdr.FC.SubType) 
		{
			// Beacon must be processed be AP Sync state machine.
        	case SUBTYPE_BEACON:
				*Machine = SYNC_STATE_MACHINE;
				*MsgType = MT2_PEER_BEACON;
            	break;

			// Only Sta have chance to receive Probe-Rsp.
			case SUBTYPE_PROBE_RSP:
				*Machine = STA_SYNC_STATE_MACHINE;
				*MsgType = STA_MT2_PEER_PROBE_RSP;
				break;

			default:
				return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}
#endif /* APCLI_SUPPORT */

// ===========================================================================================
// state_machine.c
// ===========================================================================================

/*! \brief Initialize the state machine.
 *  \param *S           pointer to the state machine 
 *  \param  Trans       State machine transition function
 *  \param  StNr        number of states 
 *  \param  MsgNr       number of messages 
 *  \param  DefFunc     default function, when there is invalid state/message combination 
 *  \param  InitState   initial state of the state machine 
 *  \param  Base        StateMachine base, internal use only
 *  \pre p_sm should be a legal pointer
 *  \post
 */
VOID StateMachineInit(
    IN STATE_MACHINE *S, 
    IN STATE_MACHINE_FUNC Trans[], 
    IN ULONG StNr, 
    IN ULONG MsgNr, 
    IN STATE_MACHINE_FUNC DefFunc, 
    IN ULONG InitState, 
    IN ULONG Base) 
{
    ULONG i, j;

    // set number of states and messages
    S->NrState = StNr;
    S->NrMsg   = MsgNr;
    S->Base    = Base;

    S->TransFunc  = Trans;
    
    // init all state transition to default function
    for (i = 0; i < StNr; i++) 
    {
        for (j = 0; j < MsgNr; j++) 
        {
            S->TransFunc[i * MsgNr + j] = DefFunc;
        }
    }
    
    // set the starting state
    S->CurrState = InitState;

}

/*! \brief This function fills in the function pointer into the cell in the state machine 
 *  \param *S   pointer to the state machine
 *  \param St   state
 *  \param Msg  incoming message
 *  \param f    the function to be executed when (state, message) combination occurs at the state machine
 *  \pre *S should be a legal pointer to the state machine, st, msg, should be all within the range, Base should be set in the initial state
 *  \post
 */
VOID StateMachineSetAction(
    IN STATE_MACHINE *S, 
    IN ULONG St, 
    IN ULONG Msg, 
    IN STATE_MACHINE_FUNC Func) 
{
    ULONG MsgIdx;
    
    MsgIdx = Msg - S->Base;

    if (St < S->NrState && MsgIdx < S->NrMsg) 
    {
        // boundary checking before setting the action
        S->TransFunc[St * S->NrMsg + MsgIdx] = Func;
    } 
}

/*! \brief   This function does the state transition
 *  \param   *Adapter the NIC adapter pointer
 *  \param   *S       the state machine
 *  \param   *Elem    the message to be executed
 *  \return   None
 */
VOID StateMachinePerformAction(
    IN	PRTMP_ADAPTER	pAd, 
    IN STATE_MACHINE *S, 
    IN MLME_QUEUE_ELEM *Elem) 
{
    (*(S->TransFunc[S->CurrState * S->NrMsg + Elem->MsgType - S->Base]))(pAd, Elem);
}

/*
    ==========================================================================
    Description:
        The drop function, when machine executes this, the message is simply 
        ignored. This function does nothing, the message is freed in 
        StateMachinePerformAction()
    ==========================================================================
 */
VOID Drop(
    IN PRTMP_ADAPTER pAd, 
    IN MLME_QUEUE_ELEM *Elem) 
{
}

// ===========================================================================================
// lfsr.c
// ===========================================================================================

/*
    ==========================================================================
    Description:
    ==========================================================================
 */
VOID LfsrInit(
    IN PRTMP_ADAPTER pAd, 
    IN ULONG Seed) 
{
    if (Seed == 0) 
        pAd->Mlme.ShiftReg = 1;
    else 
        pAd->Mlme.ShiftReg = Seed;

    get_random_bytes(&pAd->Mlme.ShiftReg, sizeof(pAd->Mlme.ShiftReg));
}

/*
    ==========================================================================
    Description:
    ==========================================================================
 */
UCHAR RandomByte(
    IN PRTMP_ADAPTER pAd) 
{
    ULONG i;
    UCHAR R, Result;

    R = 0;

    for (i = 0; i < 8; i++) 
    {
        if (pAd->Mlme.ShiftReg & 0x00000001) 
        {
            pAd->Mlme.ShiftReg = ((pAd->Mlme.ShiftReg ^ LFSR_MASK) >> 1) | 0x80000000;
            Result = 1;
        } 
        else 
        {
            pAd->Mlme.ShiftReg = pAd->Mlme.ShiftReg >> 1;
            Result = 0;
        }
        R = (R << 1) | Result;
    }

    return R;
}

/*
    ==========================================================================
    Description:
    ==========================================================================
 */
VOID AsicSwitchChannel(
    IN PRTMP_ADAPTER pAd, 
    IN UCHAR Channel) 
{
	ULONG  R3 = DEFAULT_RF_TX_POWER, R4;
    UCHAR index, BbpReg, Bbp94 = 0;
    CHAR TxPwer = 0;
    RTMP_RF_REGS *RFRegTable;

    // Select antenna
    AsicAntennaSelect(pAd, Channel);
        
	// Search Tx power value
	for (index = 0; index < pAd->ChannelListNum; index++)
	{
		if (Channel == pAd->ChannelList[index].Channel)
		{
			TxPwer = pAd->ChannelList[index].Power;
			break;
		}
	}
	if (index == pAd->ChannelListNum)
	{
		DBGPRINT(RT_DEBUG_TRACE,"Channel sanity fail(channel=%d), use first channel\n", pAd->PortCfg.Channel);
		pAd->PortCfg.Channel = FirstChannel(pAd);
		Channel = pAd->PortCfg.Channel;
		TxPwer = (ULONG) pAd->ChannelList[0].Power;
	}

	if (TxPwer > 31)  
	{
		//
		// R3 can't large than 36 (0x24), 31 ~ 36 used by BBP 94
		//
		R3 = 31;
		if (TxPwer <= 36)
			Bbp94 = BBPR94_DEFAULT + (UCHAR) (TxPwer - 31);		
	}
	else if (TxPwer < 0)
	{
		//
		// R3 can't less than 0, -1 ~ -6 used by BBP 94
		//	
		R3 = 0;
		if (TxPwer >= -6)
			Bbp94 = BBPR94_DEFAULT + TxPwer;
	}
	else
	{  
		// 0 ~ 31
		R3 = (ULONG) TxPwer;
		Bbp94 = BBPR94_DEFAULT;
	}
    
#if 0
    // E2PROM setting is calibrated for maximum TX power (i.e. 100%)
    // We lower TX power here according to the percentage specified from UI
    if (pAd->PortCfg.TxPowerPercentage > 90)       // 91 ~ 100%, treat as 100% in terms of mW
        ;
    else if (pAd->PortCfg.TxPowerPercentage > 60)  // 61 ~ 90%, treat as 75% in terms of mW
    {
        if (R3 > 2)
			R3 -= 2;
		else 
			R3 = 0;
    }
    else if (pAd->PortCfg.TxPowerPercentage > 30)  // 31 ~ 60%, treat as 50% in terms of mW
    {
        if (R3 > 6)
			R3 -= 6;
		else 
			R3 = 0;
    }
    else if (pAd->PortCfg.TxPowerPercentage > 15)  // 16 ~ 30%, treat as 25% in terms of mW
    {
		if (R3 > 12)
			R3 -= 12;
		else 
			R3 = 0;
	}
    else if (pAd->PortCfg.TxPowerPercentage > 9)   // 10 ~ 15%, treat as 12.5% in terms of mW
    {
		if (R3 > 18)
			R3 -= 18;
		else 
			R3 = 0;
	}
    else                                             // 0 ~ 9 %, treat as 6.25% in terms of mW
    {
		if (R3 > 24)
			R3 -= 24;
		else 
			R3 = 0;
	}

    if (R3 > 31)  R3 = 31;	// Maximum value 31
  
    R3 = R3 << 9; // shift TX power control to correct RF R3 bit position
#else
    // this modification is specially for Gemtek -star platform.
    // According to Gary's recommend change Tx-Power adjust mechanism.
    //-------------------- TxPower -----------------------------//
    //Attenuate   Percentage RF(R3)      BBP(R94) //
    //   0  90~100  Deafault  0x06  //
    //  -1dB  60~90  Deafault  0x05  //
    //  -3dB  30~60  Deafault  0x03  //
    //  -6dB  15~30  Deafault  0x00  //
    //  -9dB   9~15  Deafault - 6  0x00  //
    // -12dB   0~9  Deafault - 12  0x00  //
    //-------------------- TxPower -----------------------------//
    if (pAd->PortCfg.TxPowerPercentage > 90)       // 91 ~ 100%, treat as 100% in terms of mW
        ;
    else if (pAd->PortCfg.TxPowerPercentage > 60)  // 61 ~ 90%, treat as 75% in terms of mW
    {
		if (Bbp94 >= 1)
			Bbp94 -= 1;
		else if (R3 >= 2)
			R3 -= 2;
		else
			R3 = 0;
    }
    else if (pAd->PortCfg.TxPowerPercentage > 30)  // 31 ~ 60%, treat as 50% in terms of mW
    {
    	if (Bbp94 >= 3)
			Bbp94 -= 3;
		else
		{
			if (R3 >= (ULONG)((3-Bbp94)*2))
				R3 -= (ULONG)((3-Bbp94)*2);
			else
				R3 = 0;
			Bbp94 = 0;
		}
    }
    else if (pAd->PortCfg.TxPowerPercentage > 15)  // 16 ~ 30%, treat as 25% in terms of mW
    {
    	if (Bbp94 >= 6)
			Bbp94 -= 6;
		else
		{
			if (R3 >= (ULONG)((6-Bbp94)*2))
				R3 -= (ULONG)((6-Bbp94)*2);
			else
				R3 = 0;
			Bbp94 = 0;
		}
    }
    else if (pAd->PortCfg.TxPowerPercentage > 9)   // 10 ~ 15%, treat as 12.5% in terms of mW
    {
    	if (Bbp94 >= 6)
			Bbp94 -= 6;
		else
		{
			if (R3 >= (ULONG)((6-Bbp94)*2 + 6))
				R3 -= (ULONG)((6-Bbp94)*2 + 6);
			else
				R3 = 0;
			Bbp94 = 0;
		}
	}
    else                                             // 0 ~ 9 %, treat as 6.25% in terms of mW
    {
    	if (Bbp94 >= 6)
			Bbp94 -= 6;
		else
		{
			if (R3 >= (ULONG)((6-Bbp94)*2 + 12))
				R3 -= (ULONG)((6-Bbp94)*2 + 12);
			else
				R3 = 0;
			Bbp94 = 0;
		}
	}
    if (R3 > 31)  R3 = 31; // Maximum value 31

    R3 = R3 << 9; // shift TX power control to correct RF R3 bit position
#endif
    
	if (pAd->RFProgSeq == 0)
		RFRegTable = RF5225RegTable;
	else
		RFRegTable = RF5225RegTable_1;
    
    switch (pAd->RfIcType)
    {
        case RFIC_5225:
        case RFIC_5325:
        case RFIC_2527:
		case RFIC_2529:
            for (index = 0; index < NUM_OF_5225_CHNL; index++)
            {
                if (Channel == RFRegTable[index].Channel)
                {
                    R3 = R3 | (RFRegTable[index].R3 & 0xffffc1ff); // set TX power
                    R4 = (RFRegTable[index].R4 & (~0x0003f000)) | (pAd->RfFreqOffset << 12);

                    // Update variables
                    pAd->LatchRfRegs.Channel = Channel;
                    pAd->LatchRfRegs.R1 = RFRegTable[index].R1;
                    pAd->LatchRfRegs.R2 = RFRegTable[index].R2;
                    pAd->LatchRfRegs.R3 = R3;
                    pAd->LatchRfRegs.R4 = R4;
                    
                    // Set RF value
                    RTMP_RF_IO_WRITE32(pAd, pAd->LatchRfRegs.R1);
                    RTMP_RF_IO_WRITE32(pAd, pAd->LatchRfRegs.R2);
                    RTMP_RF_IO_WRITE32(pAd, pAd->LatchRfRegs.R3);
                    RTMP_RF_IO_WRITE32(pAd, pAd->LatchRfRegs.R4);
                    break;
                }
            }
            RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &BbpReg);
            if ((pAd->RfIcType == RFIC_5225) || (pAd->RfIcType == RFIC_2527))
                BbpReg &= 0xFE;    // b0=0 for none smart mode
            else
                BbpReg |= 0x01;    // b0=1 for smart mode
            RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, BbpReg);

            if (Bbp94 >= 0)
            {
                RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R94, Bbp94);
                pAd->LatchBBPR94 = Bbp94;
            }
            break;

        default:
            break;
    }

	//
	// On 11A, We should delay and wait RF/BBP to be stable
	// and the appropriate time should be 1000 micro seconds 
	// 2005/06/05 - On 11G, We also need this delay time. Otherwise it's difficult to pass the WHQL.
	//
    RTMPusecDelay(1000);
    
    DBGPRINT(RT_DEBUG_TRACE, "AsicSwitchChannel(RF=%d, Pwr=%d) to #%d, R1=0x%08x, R2=0x%08x, R3=0x%08x, R4=0x%08x\n",
        pAd->RfIcType, 
        (R3 & 0x00003e00) >> 9,
        Channel, 
        pAd->LatchRfRegs.R1, 
        pAd->LatchRfRegs.R2, 
        pAd->LatchRfRegs.R3, 
        pAd->LatchRfRegs.R4);
}

/*
    ==========================================================================
    Description:
        When this function is called, it's always after AsicSwitchChannel().
    ==========================================================================
 */
VOID AsicLockChannel(
    IN PRTMP_ADAPTER pAd, 
    IN UCHAR Channel) 
{
}

char *AntStr[3] = {"DIVERSITY","Ant-A","Ant-B"};

#define SOFTWARE_DIVERSITY  0
#define ANTENNA_A		    1
#define ANTENNA_B		    2
#define HARDWARE_DIVERSITY  3

/*
    ==========================================================================
    Description:
    ==========================================================================
 */
VOID	AsicAntennaSelect(
    IN	PRTMP_ADAPTER	pAd,
    IN	UCHAR			Channel) 
{
	ULONG 			csr0;	
	ABGBAND_STATE	BandState;

	DBGPRINT(RT_DEBUG_INFO,"AsicAntennaSelect(ch=%d) - Tx=%s, Rx=%s\n", 
	    Channel, AntStr[pAd->Antenna.field.TxDefaultAntenna], AntStr[pAd->Antenna.field.RxDefaultAntenna]);
	    
	if (Channel <= 14)
		BandState = BG_BAND;
	else
		BandState = A_BAND;

	//
	//  Only the first time switch from g to a or a to g
	//  and then will be reset the BBP, otherwise do nothing.
	//
	if (BandState == pAd->PortCfg.BandState)
		return;

	// Change BBP setting during siwtch from a->g, g->a
    if (Channel <= 14)
    {
		if (pAd->NicConfig2.field.ExternalLNAForG)
    	{
    		RTMP_WRITE_BBP_R17(pAd, 0x30); // if external LNA enable, this value need to be offset 0x10
    		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, 96, 0x68); // if external LNA enable, this value need to be offset 0x20
    		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, 104, 0x3C);// if external LNA enable, this value need to be offset 0x10
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, 75, 0x80);// if external LNA enable, set this to 0x80 on B/G mode, Request by David 2005/05/12
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, 86, 0x80);// if external LNA enable, set this to 0x80 on B/G mode, Request by David 2005/05/12
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, 88, 0x80);// if external LNA enable, set this to 0x80 on B/G mode, Request by David 2005/05/12 
    	}
    	else
    	{
        	RTMP_WRITE_BBP_R17(pAd, 0x20);
        	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, 96, 0x48);
        	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, 104, 0x2C);
    	}
        RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, 35, 0x50);        
        RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, 97, 0x48);
        RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, 98, 0x48);        
    }
    else
    {
		if (pAd->NicConfig2.field.ExternalLNAForA)
    	{
    		RTMP_WRITE_BBP_R17(pAd, (0x28 + pAd->AModeGainValue));	// if external LNA enable, this value need to be offset 0x10
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, 96, 0x58 + 2 * pAd->AModeGainValue); // if external LNA enable, this value need to be offset 0x20
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, 104, 0x38 + pAd->AModeGainValue);// if external LNA enable, this value need to be offset 0x10
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, 75, 0xfe);
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, 86, 0xfe);
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, 88, 0xfe);
    	}
    	else
    	{
    		RTMP_WRITE_BBP_R17(pAd, 0x28);
        	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, 96, 0x58);
        	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, 104, 0x38);
    	}
        RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, 35, 0x60);         
        RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, 97, 0x58);
        RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, 98, 0x58);
    }
    
    // Turn on PA_PE for 11A / 11G band
    RTMP_IO_READ32(pAd, PHY_CSR0, &csr0);
    csr0 = csr0 & 0xfffcffff;   // Mask off bit 16, bit 17
    if (Channel <= 14)
        csr0 = csr0 | BIT32[16]; // b16 to enable G band PA_PE
    else
        csr0 = csr0 | BIT32[17]; // b17 to enable A band PA_PE
    RTMP_IO_WRITE32(pAd, PHY_CSR0, csr0);

	pAd->PortCfg.BandState = BandState;

	AsicAntennaSetting(pAd, BandState);
}

/*
    ==========================================================================
    Description:
    ==========================================================================
 */
VOID	AsicAntennaSetting(
    IN	PRTMP_ADAPTER	pAd,
    IN	ABGBAND_STATE	BandState)
{
	UCHAR		R4, R77;
	UCHAR		FrameTypeMaskBit5 = 0;
	
	// driver must disable Rx when switching antenna, otherwise ASIC will keep default state
	// after switching, driver needs to re-enable Rx later
	RTMP_IO_WRITE32(pAd, TXRX_CSR0, 0x0257b032);        // Disable Rx
	
	// Update antenna registers
	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R4, &R4);
	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R77, &R77);

	R4	&= ~0x23;		// clear Bit 0,1,5
	R77	&= ~0x03;		// clear Bit 0,1

	FrameTypeMaskBit5 = ~(pAd->Antenna.field.FrameType << 5);

	//Select RF_Type
	switch (pAd->Antenna.field.RfIcType)
	{
		case RFIC_5225:
		case RFIC_5325:
			//Support 11B/G/A			
			if (BandState == BG_BAND)
			{
				//Check Rx Anttena
				if (pAd->Antenna.field.RxDefaultAntenna == ANTENNA_A)
				{
					R4  = R4 | 0x01;		// <Bit5:Bit1:Bit0> = <0:0:1>
					R77 = R77 | 0x03;	// <Bit1:Bit0> = <1:1>
				}
				else if (pAd->Antenna.field.RxDefaultAntenna == ANTENNA_B)
				{
					R4  = R4 | 0x01;		// <Bit5:Bit1:Bit0> = <0:0:1>
					// R77;				// <Bit1:Bit0> = <0:0>
				}
				else if (pAd->Antenna.field.RxDefaultAntenna == HARDWARE_DIVERSITY)
				{
					R4  = R4 | 0x22;		// <Bit5:Bit1:Bit0> = <1:1:0>
					/// R77;				// <Bit1:Bit0> =
				}
				else 
				{
					// AP does not support software-diversity
					// Set to ANTENNA_B as default vaule
					R4  = R4 | 0x01;		// <Bit5:Bit1:Bit0> = <0:0:1>
					// R77;				// <Bit1:Bit0> = <0:0>
				}
			}
			else //A_BAND
			{
				//Check Rx Anttena
				if (pAd->Antenna.field.RxDefaultAntenna == ANTENNA_A)
				{
					R4  = R4 | 0x01;		// <Bit5:Bit1:Bit0> = <0:0:1>
					// R77;				// <Bit1:Bit0> = <0:0>
				}
				else if (pAd->Antenna.field.RxDefaultAntenna == ANTENNA_B)
				{
					R4  = R4 | 0x01;		// <Bit5:Bit1:Bit0> = <0:0:1>
					R77	= R77 | 0x03;	// <Bit1:Bit0> = <1:1>
				}
				else if (pAd->Antenna.field.RxDefaultAntenna == HARDWARE_DIVERSITY)
				{
					R4  = R4 | 0x02;		// <Bit5:Bit1:Bit0> = <0:1:0>
					// R77;				// <Bit1:Bit0> =
				}
				else 
				{
					// AP does not support software-diversity
					// Set to ANTENNA_B as default vaule
					R4  = R4 | 0x01;		// <Bit5:Bit1:Bit0> = <0:0:1>
					R77	= R77 | 0x03;	// <Bit1:Bit0> = <1:1>
				}
			}
			break;
		case RFIC_2527:
			//Support 11B/G
			//Check Rx Anttena
			//Check Rx Anttena
			if (pAd->Antenna.field.RxDefaultAntenna == ANTENNA_A)
			{
				R4  = R4 | 0x21;		// <Bit5:Bit1:Bit0> = <1:0:1>
				R4  = R4 & FrameTypeMaskBit5;
				
				R77	= R77 | 0x03;	// <Bit1:Bit0> = <1:1>
			}
			else if (pAd->Antenna.field.RxDefaultAntenna == ANTENNA_B)
			{
				R4  = R4 | 0x21;		// <Bit5:Bit1:Bit0> = <1:0:1>
				R4  = R4 & FrameTypeMaskBit5;
				
				// R77;				// <Bit1:Bit0> = <0:0>
			}
			else if (pAd->Antenna.field.RxDefaultAntenna == HARDWARE_DIVERSITY)
			{
				R4  = R4 | 0x22;		// <Bit5:Bit1:Bit0> = <1:1:0>
				R4  = R4 & FrameTypeMaskBit5;
				
				// R77;				// <Bit1:Bit0> =
			}
			else 
			{
				// AP does not support software-diversity
				// Set to ANTENNA_B as default vaule
				R4  = R4 | 0x21;		// <Bit5:Bit1:Bit0> = <1:0:1>
				R4  = R4 & FrameTypeMaskBit5;
				
				// R77;				// <Bit1:Bit0> = <0:0>
			}
			break;			
		case RFIC_2529:			
			//Support 11B/G
			//Check Rx Anttena
			if (pAd->Antenna.field.NumOfAntenna == 0)
			{
				if ((pAd->NicConfig2.field.Enable4AntDiversity == 1)
					&& (pAd->NicConfig2.field.TxDiversity == 1))// Tx/Rx diversity
				{
					// R77;				// <Bit1:Bit0>
					R4  = R4 | 0x22;		// <Bit5:Bit1:Bit0> = <1:1:0>

					pAd->RxAnt.Pair1PrimaryRxAnt   = 1;  // assume ant-B
					pAd->RxAnt.Pair1SecondaryRxAnt = 0;  // assume ant-A
					pAd->RxAnt.Pair2PrimaryRxAnt   = 1;  // assume ant-B
					pAd->RxAnt.Pair2SecondaryRxAnt = 0;  // assume ant-A
					AsicSetRxAnt(pAd, pAd->RxAnt.Pair1PrimaryRxAnt, pAd->RxAnt.Pair2PrimaryRxAnt);
				}
				else if ((pAd->NicConfig2.field.Enable4AntDiversity == 1) &&
					(pAd->NicConfig2.field.TxDiversity == 0)	)		// Tx fix & Rx diversity
				{
					if (pAd->NicConfig2.field.TxRxFixed == 0)			// TxRxFixed <Tx:Rx> = <E1/E1:E4>
					{
						// R77;				// <Bit1:Bit0>
					}
					else if (pAd->NicConfig2.field.TxRxFixed == 1)			// TxRxFixed <Tx:Rx> = <E2/E2:E3>
					{
						// R77;				// <Bit1:Bit0>
					}
					else if (pAd->NicConfig2.field.TxRxFixed == 2)			// TxRxFixed <Tx:Rx> = <E3/E1:E3>
					{
						R77	= R77 | 0x03;	// <Bit1:Bit0> = <1:1>
					}
					else if (pAd->NicConfig2.field.TxRxFixed == 3)			// TxRxFixed <Tx:Rx> = <E4/E2:E4>
					{
						R77	= R77 | 0x03;	// <Bit1:Bit0> = <1:1>
					}
					
					R4  = R4 | 0x01;		// <Bit5:Bit1:Bit0> = <0:0:1>
					
					pAd->RxAnt.Pair1PrimaryRxAnt   = 1;  // assume ant-B
					pAd->RxAnt.Pair1SecondaryRxAnt = 0;  // assume ant-A
					pAd->RxAnt.Pair2PrimaryRxAnt   = 1;  // assume ant-B
					pAd->RxAnt.Pair2SecondaryRxAnt = 0;  // assume ant-A
					AsicSetRxAnt(pAd, pAd->RxAnt.Pair1PrimaryRxAnt, pAd->RxAnt.Pair2PrimaryRxAnt);
				}
				else if ((pAd->NicConfig2.field.Enable4AntDiversity == 0) &&
					(pAd->NicConfig2.field.TxDiversity == 1)	)		// Tx diversity & 2R
				{
					R4  = R4 | 0x02;		// <Bit5:Bit1:Bit0> = <0:1:0>
					// R77;				// <Bit1:Bit0>
					
					if (pAd->NicConfig2.field.TxRxFixed == 0)			// TxRxFixed <Tx:Rx> = <E1/E1:E4>
					{
						AsicSetRxAnt(pAd, 0, 1);
					}
					else if (pAd->NicConfig2.field.TxRxFixed == 1)			// TxRxFixed <Tx:Rx> = <E2/E2:E3>
					{
						AsicSetRxAnt(pAd, 1, 0);
					}
					else if (pAd->NicConfig2.field.TxRxFixed == 2)			// TxRxFixed <Tx:Rx> = <E3/E1:E3>
					{
						AsicSetRxAnt(pAd, 0, 0);
					}
					else if (pAd->NicConfig2.field.TxRxFixed == 3)			// TxRxFixed <Tx:Rx> = <E4/E2:E4>
					{
						AsicSetRxAnt(pAd, 1, 1);
					}
				}
				else if ((pAd->NicConfig2.field.Enable4AntDiversity == 0) &&
					(pAd->NicConfig2.field.TxDiversity == 0)	)		// Tx fix & 2R
				{
					if (pAd->NicConfig2.field.TxRxFixed == 0)			// TxRxFixed <Tx:Rx> = <E1/E1:E4>
					{
						R4  = R4 | 0x01;		// <Bit5:Bit1:Bit0> = <0:0:1>
						// R77;				// <Bit1:Bit0> = <0:0>
						AsicSetRxAnt(pAd, 0, 1);
					}
					else if (pAd->NicConfig2.field.TxRxFixed == 1)			// TxRxFixed <Tx:Rx> = <E2/E2:E3>
					{
						R4  = R4 | 0x01;		// <Bit5:Bit1:Bit0> = <0:0:1>
						// R77;				// <Bit1:Bit0> = <0:0>
						AsicSetRxAnt(pAd, 1, 0);
					}
					else if (pAd->NicConfig2.field.TxRxFixed == 2)			// TxRxFixed <Tx:Rx> = <E3/E1:E3>
					{
						R4  = R4 | 0x01;		// <Bit5:Bit1:Bit0> = <0:0:1>
						R77	= R77 | 0x03;	// <Bit1:Bit0> = <1:1>
						AsicSetRxAnt(pAd, 0, 0);
					}
					else if (pAd->NicConfig2.field.TxRxFixed == 3)			// TxRxFixed <Tx:Rx> = <E4/E2:E4>
					{
						R4  = R4 | 0x01;		// <Bit5:Bit1:Bit0> = <0:0:1>
						R77	= R77 | 0x03;	// <Bit1:Bit0> = <1:1>
						AsicSetRxAnt(pAd, 1, 1);
					}
				}
			}
			else
			{
				if (pAd->Antenna.field.RxDefaultAntenna == ANTENNA_A)
				{
					R4  = R4 | 0x01;		// <Bit5:Bit1:Bit0> = <0:0:1>
					R77	= R77 | 0x03;	// <Bit1:Bit0> = <1:1>
				}
				else if (pAd->Antenna.field.RxDefaultAntenna == ANTENNA_B)
				{
					R4  = R4 | 0x01;		// <Bit5:Bit1:Bit0> = <0:0:1>
					// R77;				// <Bit1:Bit0> = <0:0>
				}
				else if (pAd->Antenna.field.RxDefaultAntenna == HARDWARE_DIVERSITY)
				{
					R4  = R4 | 0x02;		// <Bit5:Bit1:Bit0> = <0:1:0>
					// R77;				// <Bit1:Bit0> =
				}
				else 
				{
					// AP does not support software-diversity
					// Set to ANTENNA_B as default vaule
					R4  = R4 | 0x01;		// <Bit5:Bit1:Bit0> = <0:0:1>
					// R77;				// <Bit1:Bit0> = <0:0>
				}
			}
			break;
		default:
			DBGPRINT(RT_DEBUG_TRACE, "Unkown RFIC Type\n");
			break;
	}

	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R4, R4);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R77, R77);
	
	RTMP_IO_WRITE32(pAd, TXRX_CSR0, 0x0256b032);       // enable RX of MAC block
}

/*
    ==========================================================================
    Description:
        Gives CCK TX rate 2 more dB TX power.
        This routine works only in LINK UP in INFRASTRUCTURE mode.

        calculate desired Tx power in RF R3.Tx0~5,  should consider -
        0. if current radio is a noisy environment (pAd->DrsCounters.fNoisyEnvironment)
        1. TxPowerPercentage
        2. auto calibration based on TSSI feedback
        3. extra 2 db for CCK
        4. -10 db upon very-short distance (AvgRSSI >= -40db) to AP

    NOTE: Since this routine requires the value of (pAd->DrsCounters.fNoisyEnvironment),
        it should be called AFTER MlmeDynamicTxRatSwitching()
    ==========================================================================
 */
VOID AsicAdjustTxPower(
    IN PRTMP_ADAPTER pAd) 
{
	ULONG   R3, CurrTxPwr;
    SHORT  dbm;
    UCHAR  Channel, index;
    BOOLEAN bAutoTxAgc = FALSE;
    UCHAR   TssiRef, *pTssiMinusBoundary, *pTssiPlusBoundary, TxAgcStep;
    UCHAR   BbpR1, idx;
	PCHAR	pTxAgcCompensate;
	CHAR	TxPwer = 0;

    Channel = pAd->PortCfg.Channel;

    dbm = pAd->PortCfg.AvgRssi - pAd->BbpRssiToDbmDelta;
    if ((pAd->RfIcType == RFIC_5325) || (pAd->RfIcType == RFIC_2529))
    {
    	if (pAd->PortCfg.AvgRssi2 > pAd->PortCfg.AvgRssi)
    	{
    		dbm = pAd->PortCfg.AvgRssi2 - pAd->BbpRssiToDbmDelta;
    	}
    }

    // get TX Power base from E2PROM
    R3 = DEFAULT_RF_TX_POWER;
    for (index= 0 ; index < pAd->ChannelListNum; index++)
    {
        if (pAd->ChannelList[index].Channel == pAd->PortCfg.Channel)
        {
            TxPwer = pAd->ChannelList[index].Power;
            break;
        }
    }
    
#if 0    
    if ((TxPwer > 31) || (TxPwer < 0))
		R3 = 0;
	else
		R3 = (ULONG) TxPwer;
	
    if (R3 > 31)  R3 = 31;
#else
	//
 	// Correct R3 value, R3 value should be in range 0 ~ 31.
 	//
	if (TxPwer < 0)
    	R3 = 0;
	else if (TxPwer > 31)
    	R3 = 31;
	else
    	R3 = (ULONG) TxPwer;
#endif

    // error handling just in case
    if (index >= pAd->ChannelListNum)
    {
        DBGPRINT(RT_DEBUG_ERROR, "AsicAdjustTxPower(can find Channel=%d in ChannelList[%d]\n", pAd->PortCfg.Channel, pAd->ChannelListNum);
        return;
    }

    // TX power compensation for temperature variation based on TSSI. try every 4 second
    if (pAd->Mlme.PeriodicRound % 4 == 0)
    {
        if (pAd->PortCfg.Channel <= 14)
        {
            bAutoTxAgc         = pAd->bAutoTxAgcG;
            TssiRef            = pAd->TssiRefG;
            pTssiMinusBoundary = &pAd->TssiMinusBoundaryG[0];
            pTssiPlusBoundary  = &pAd->TssiPlusBoundaryG[0];
            TxAgcStep          = pAd->TxAgcStepG;
            pTxAgcCompensate   = &pAd->TxAgcCompensateG;
        }
        else
        {
            bAutoTxAgc         = pAd->bAutoTxAgcA;
            TssiRef            = pAd->TssiRefA;
            pTssiMinusBoundary = &pAd->TssiMinusBoundaryA[0];
            pTssiPlusBoundary  = &pAd->TssiPlusBoundaryA[0];
            TxAgcStep          = pAd->TxAgcStepA;
            pTxAgcCompensate   = &pAd->TxAgcCompensateA;
        }

        if (bAutoTxAgc)
        {
            RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R1, &BbpR1);
            if (BbpR1 > pTssiMinusBoundary[1])
            {
                // Reading is larger than the reference value
                // check for how large we need to decrease the Tx power
                for (idx = 1; idx < 5; idx++)
                {
                    if (BbpR1 <= pAd->TssiMinusBoundaryG[idx])  // Found the range
                        break;
                }
                // The index is the step we should decrease, idx = 0 means there is nothing to compensate
                if (R3 > (ULONG) (TxAgcStep * (idx-1)))
                    *pTxAgcCompensate = -(TxAgcStep * (idx-1));
                else
                    *pTxAgcCompensate = -((UCHAR)R3);

                R3 += (*pTxAgcCompensate);
                DBGPRINT(RT_DEBUG_TRACE, "-- Tx Power, BBP R1=%x, TssiRef=%x, TxAgcStep=%x, step = -%d\n",
                    BbpR1, TssiRef, TxAgcStep, idx-1);
            }
            else if (BbpR1 < pTssiPlusBoundary[1])
            {
                // Reading is smaller than the reference value
                // check for how large we need to increase the Tx power
                for (idx = 1; idx < 5; idx++)
                {
                    if (BbpR1 >= pTssiPlusBoundary[idx])   // Found the range
                        break;
                }
                // The index is the step we should increase, idx = 0 means there is nothing to compensate
                *pTxAgcCompensate = TxAgcStep * (idx-1);
                R3 += (*pTxAgcCompensate);
                DBGPRINT(RT_DEBUG_TRACE, "++ Tx Power, BBP R1=%x, Tssi0=%x, TxAgcStep=%x, step = +%d\n",
                    BbpR1, TssiRef, TxAgcStep, idx-1);
            }
        }
    }
    else
	{
		if (pAd->PortCfg.Channel <= 14)
		{
            bAutoTxAgc         = pAd->bAutoTxAgcG;
			pTxAgcCompensate   = &pAd->TxAgcCompensateG;
		}
		else
		{
            bAutoTxAgc         = pAd->bAutoTxAgcA;
			pTxAgcCompensate   = &pAd->TxAgcCompensateA;
		}

		if (bAutoTxAgc)
			R3 += (*pTxAgcCompensate);
	}

    // E2PROM setting is calibrated for maximum TX power (i.e. 100%)
    // We lower TX power here according to the percentage specified from UI
    if (pAd->PortCfg.TxPowerPercentage == 0xffffffff)       // AUTO TX POWER control
	{
	}
    else if (pAd->PortCfg.TxPowerPercentage > 90)  // 91 ~ 100% & AUTO, treat as 100% in terms of mW
        ;
    else if (pAd->PortCfg.TxPowerPercentage > 60)  // 61 ~ 90%, treat as 75% in terms of mW
	{
		if (R3 > 2)
			R3 -= 2;
		else
			R3 = 0;
    }
    else if (pAd->PortCfg.TxPowerPercentage > 30)  // 31 ~ 60%, treat as 50% in terms of mW
	{
		if (R3 > 6)			
        R3 -= 6;
		else
			R3 = 0;
	}
    else if (pAd->PortCfg.TxPowerPercentage > 15)  // 16 ~ 30%, treat as 25% in terms of mW
	{
		if (R3 > 12)
			R3 -= 12;
		else
			R3 = 0;
	}
    else if (pAd->PortCfg.TxPowerPercentage > 9)   // 10 ~ 15%, treat as 12.5% in terms of mW
	{
		if (R3 > 18)
			R3 -= 18;
		else
			R3 = 0;
	}
    else                                           // 0 ~ 9 %, treat as MIN(~3%) in terms of mW
	{
		if (R3 > 24)
			R3 -= 24;
		else
			R3 = 0;
	}

	if (R3 > 31)  R3 = 31;   //Maximum value 31

    // compare the desired R3.TxPwr value with current R3, if not equal
    // set new R3.TxPwr
    CurrTxPwr = (pAd->LatchRfRegs.R3 >> 9) & 0x0000001f;
    if (CurrTxPwr != R3)
    {
        CurrTxPwr = R3;
        R3 = (pAd->LatchRfRegs.R3 & 0xffffc1ff) | (R3 << 9);
        pAd->LatchRfRegs.R3 = R3;
        
        // Set RF value
        RTMP_RF_IO_WRITE32(pAd, pAd->LatchRfRegs.R1);
        RTMP_RF_IO_WRITE32(pAd, pAd->LatchRfRegs.R2);
        RTMP_RF_IO_WRITE32(pAd, pAd->LatchRfRegs.R3);
        RTMP_RF_IO_WRITE32(pAd, pAd->LatchRfRegs.R4);
    }
    DBGPRINT(RT_DEBUG_TRACE, "AsicAdjustTxPower = %d, AvgRssi = %d\n", CurrTxPwr, dbm);
}

/*
    ==========================================================================
    Description:
    ==========================================================================
 */
VOID AsicSetBssid(
    IN PRTMP_ADAPTER pAd, 
    IN PUCHAR pBssid) 
{
    ULONG          Addr4;

	COPY_MAC_ADDR(pAd->PortCfg.Bssid, pBssid);
    Addr4 = (ULONG)(pBssid[0])       | 
            (ULONG)(pBssid[1] << 8)  | 
            (ULONG)(pBssid[2] << 16) |
            (ULONG)(pBssid[3] << 24);
    RTMP_IO_WRITE32(pAd, MAC_CSR4, Addr4);

    // number of BSSID will be set when MakeAllBssBeacon
    Addr4 = (ULONG)(pBssid[4]) | (ULONG)(pBssid[5] << 8);
    RTMP_IO_WRITE32(pAd, MAC_CSR5, Addr4);
}

/*
    ==========================================================================
    Description:
    ==========================================================================
 */
VOID AsicDisableSync(
    IN PRTMP_ADAPTER pAd) 
{
    TXRX_CSR9_STRUC csr;
    DBGPRINT(RT_DEBUG_TRACE, "--->Disable TSF synchronization\n");

    // 2003-12-20 disable TSF and TBTT while NIC in power-saving have side effect
    //            that NIC will never wakes up because TSF stops and no more 
    //            TBTT interrupts
    RTMP_IO_READ32(pAd, TXRX_CSR9, &csr.word);
    csr.field.bBeaconGen = 0;
    csr.field.TsfSyncMode = 0;
    csr.field.bTBTTEnable = 0;
    csr.field.bTsfTicking = 0;
    RTMP_IO_WRITE32(pAd, TXRX_CSR9, csr.word);
}

/*
    ==========================================================================
    Description:
    ==========================================================================
 */
VOID AsicEnableBssSync(
    IN PRTMP_ADAPTER pAd) 
{
    TXRX_CSR9_STRUC csr9;

    DBGPRINT(RT_DEBUG_TRACE, "--->AsicEnableBssSync(INFRA mode)\n");

    AsicDisableSync(pAd);
    
    // start sending BEACON
    csr9.field.BeaconInterval = pAd->PortCfg.BeaconPeriod << 4; // ASIC register in units of 1/16 TU
    csr9.field.bTsfTicking = 1;
//    csr9.field.TsfSyncMode = 2; // sync TSF in IBSS mode
    csr9.field.TsfSyncMode = 3; // edit by johnli, sync with no body (AP mode)
    csr9.field.bTBTTEnable = 1;
    csr9.field.bBeaconGen = 1;
    RTMP_IO_WRITE32(pAd, TXRX_CSR9, csr9.word);
}

/*
    ==========================================================================
    Description:
    ==========================================================================
 */
VOID AsicSetEdcaParm(
    IN PRTMP_ADAPTER pAd,
    IN PEDCA_PARM    pEdcaParm)
{
	AC_TXOP_CSR0_STRUC csr0;
    AC_TXOP_CSR1_STRUC csr1;
    AIFSN_CSR_STRUC    AifsnCsr;
    CWMIN_CSR_STRUC    CwminCsr;
    CWMAX_CSR_STRUC    CwmaxCsr;

    DBGPRINT(RT_DEBUG_TRACE,"AsicSetEdcaParm\n");
    if ((pEdcaParm == NULL) || (pEdcaParm->bValid == FALSE))
    {
        csr0.field.Ac0Txop = 0;     // QID_AC_BE
        csr0.field.Ac1Txop = 0;     // QID_AC_BK
        RTMP_IO_WRITE32(pAd, AC_TXOP_CSR0, csr0.word);
        if (pAd->PortCfg.PhyMode == PHY_11B)
        {
            csr1.field.Ac2Txop = cpu2le16(192);     // AC_VI: 192*32us ~= 6ms
            csr1.field.Ac3Txop = cpu2le16(96);      // AC_VO: 96*32us  ~= 3ms
        }
        else
        {
            csr1.field.Ac2Txop = cpu2le16(96);      // AC_VI: 96*32us ~= 3ms
            csr1.field.Ac3Txop = cpu2le16(48);      // AC_VO: 48*32us ~= 1.5ms
        }
        RTMP_IO_WRITE32(pAd, AC_TXOP_CSR1, csr1.word);

        CwminCsr.word = 0;
        CwminCsr.field.Cwmin0 = CW_MIN_IN_BITS;
        CwminCsr.field.Cwmin1 = CW_MIN_IN_BITS;
        CwminCsr.field.Cwmin2 = CW_MIN_IN_BITS;
        CwminCsr.field.Cwmin3 = CW_MIN_IN_BITS;
        RTMP_IO_WRITE32(pAd, CWMIN_CSR, CwminCsr.word);

        CwmaxCsr.word = 0;
        CwmaxCsr.field.Cwmax0 = CW_MAX_IN_BITS;
        CwmaxCsr.field.Cwmax1 = CW_MAX_IN_BITS;
        CwmaxCsr.field.Cwmax2 = CW_MAX_IN_BITS;
        CwmaxCsr.field.Cwmax3 = CW_MAX_IN_BITS;
        RTMP_IO_WRITE32(pAd, CWMAX_CSR, CwmaxCsr.word);

        RTMP_IO_WRITE32(pAd, AIFSN_CSR, 0x00002222);
        
        NdisZeroMemory(&pAd->PortCfg.APEdcaParm, sizeof(EDCA_PARM));
    }
    else
    {
        csr0.field.Ac0Txop = cpu2le16(pEdcaParm->Txop[QID_AC_BE]);
        csr0.field.Ac1Txop = cpu2le16(pEdcaParm->Txop[QID_AC_BK]);
        RTMP_IO_WRITE32(pAd, AC_TXOP_CSR0, csr0.word);

	//
	// Modify Cwmin/Cwmax/Txop on queue[QID_AC_VI], Recommend by Jerry 2005/07/27
	// To degrade our VIDO Queue's throughput for WiFi WMM S3T07 Issue.
	//
        csr1.field.Ac2Txop = cpu2le16(pEdcaParm->Txop[QID_AC_VI]/* * 7 / 10*/);
        csr1.field.Ac3Txop = cpu2le16(pEdcaParm->Txop[QID_AC_VO]);
        RTMP_IO_WRITE32(pAd, AC_TXOP_CSR1, csr1.word);

        CwminCsr.word = 0;
        CwminCsr.field.Cwmin0 = pEdcaParm->Cwmin[QID_AC_BE];
        CwminCsr.field.Cwmin1 = pEdcaParm->Cwmin[QID_AC_BK];
        CwminCsr.field.Cwmin2 = pEdcaParm->Cwmin[QID_AC_VI];
        CwminCsr.field.Cwmin3 = pEdcaParm->Cwmin[QID_AC_VO];
        RTMP_IO_WRITE32(pAd, CWMIN_CSR, CwminCsr.word);

        CwmaxCsr.word = 0;
        CwmaxCsr.field.Cwmax0 = pEdcaParm->Cwmax[QID_AC_BE];
        CwmaxCsr.field.Cwmax1 = pEdcaParm->Cwmax[QID_AC_BK];
        CwmaxCsr.field.Cwmax2 = pEdcaParm->Cwmax[QID_AC_VI];
        CwmaxCsr.field.Cwmax3 = pEdcaParm->Cwmax[QID_AC_VO];
        RTMP_IO_WRITE32(pAd, CWMAX_CSR, CwmaxCsr.word);

        AifsnCsr.word = 0;
        AifsnCsr.field.Aifsn0 = pEdcaParm->Aifsn[QID_AC_BE];
        AifsnCsr.field.Aifsn1 = pEdcaParm->Aifsn[QID_AC_BK];
        AifsnCsr.field.Aifsn2 = pEdcaParm->Aifsn[QID_AC_VI];
        AifsnCsr.field.Aifsn3 = pEdcaParm->Aifsn[QID_AC_VO];
        RTMP_IO_WRITE32(pAd, AIFSN_CSR, AifsnCsr.word);
        
        NdisMoveMemory(&pAd->PortCfg.APEdcaParm, pEdcaParm, sizeof(EDCA_PARM));

        DBGPRINT(RT_DEBUG_TRACE,"AP [Cnt#%d]: AIFSN CWmin CWmax TXOP(us)  ACM\n", pEdcaParm->EdcaUpdateCount);
        DBGPRINT(RT_DEBUG_TRACE,"    AC_BE     %d     %d     %d     %4d     %d\n",
            pEdcaParm->Aifsn[0],
            pEdcaParm->Cwmin[0],
            pEdcaParm->Cwmax[0],
            pEdcaParm->Txop[0]<<5,
            pEdcaParm->bACM[0]);
        DBGPRINT(RT_DEBUG_TRACE,"    AC_BK     %d     %d     %d     %4d     %d\n",
            pEdcaParm->Aifsn[1],
            pEdcaParm->Cwmin[1],
            pEdcaParm->Cwmax[1],
            pEdcaParm->Txop[1]<<5,
            pEdcaParm->bACM[1]);
        DBGPRINT(RT_DEBUG_TRACE,"    AC_VI     %d     %d     %d     %4d     %d\n",
            pEdcaParm->Aifsn[2],
            pEdcaParm->Cwmin[2],
            pEdcaParm->Cwmax[2],
            pEdcaParm->Txop[2]<<5,
            pEdcaParm->bACM[2]);
        DBGPRINT(RT_DEBUG_TRACE,"    AC_VO     %d     %d     %d     %4d     %d\n",
            pEdcaParm->Aifsn[3],
            pEdcaParm->Cwmin[3],
            pEdcaParm->Cwmax[3],
            pEdcaParm->Txop[3]<<5,
            pEdcaParm->bACM[3]);
    }
}

/*
    ==========================================================================
    Description:
    ==========================================================================
 */
VOID AsicSetSlotTime(
    IN PRTMP_ADAPTER pAd,
    IN BOOLEAN bUseShortSlotTime) 
{
    MAC_CSR9_STRUC Csr9;

    RTMP_IO_READ32(pAd, MAC_CSR9, &Csr9.word);

    if (pAd->PortCfg.MBSSID[MAIN_MBSSID].bWmmCapable)
    {
        if (pAd->PortCfg.PhyMode == PHY_11A)
            Csr9.field.SlotTime = 14;
        else
            Csr9.field.SlotTime = (bUseShortSlotTime)? 14 : 20;
    }
    else
    {
        if (pAd->PortCfg.PhyMode == PHY_11A)
            Csr9.field.SlotTime = 9;
        else
            Csr9.field.SlotTime = (bUseShortSlotTime)? 9 : 20;
    }

#ifndef WIFI_TEST
    // force using short SLOT time for FAE to demo performance only
    if (pAd->PortCfg.bEnableTxBurst)
        Csr9.field.SlotTime = 9;
#endif

    RTMP_IO_WRITE32(pAd, MAC_CSR9, Csr9.word);

    DBGPRINT(RT_DEBUG_INFO, "AsicSetSlotTime(=%d us)\n",Csr9.field.SlotTime);
}

VOID AsicResetBbpTuning(
	IN PRTMP_ADAPTER pAd)
{
	UCHAR R17, R62, R17UpperBound, R17LowerBound;

	// If turn on radar detection, BBP_R17 tuning needs to be turn off.
	if ((pAd->BbpTuning.bEnable == FALSE) || (pAd->PortCfg.RadarDetect.RDMode != RD_NORMAL_MODE))
        return;

	R17 = pAd->BbpWriteLatch[17];
	if (pAd->LatchRfRegs.Channel <= 14)
	{
		R17UpperBound = pAd->BbpTuning.R17UpperBoundG;
		R17LowerBound = pAd->BbpTuning.R17LowerBoundG;
	}
	else
	{
		R17UpperBound = pAd->BbpTuning.R17UpperBoundA;
		R17LowerBound = pAd->BbpTuning.R17LowerBoundA;
	}

	R17 = R17LowerBound;
	RTMP_WRITE_BBP_R17(pAd, R17);

	// BBP R62 should be set to 0x04 in A-band
	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R62, &R62);

// Disable this code block, because the tuning(R62=0x2) will cause FalseCCA large enough to block the beacon transmit
#if 0
	if (pAd->LatchRfRegs.Channel <= 14)
	{
		if (pAd->RalinkCounters.OneSecFalseCCACnt < 100)
		{
			R62 = 0x02;
		}
		else
		{
			R62 = 0x04;
		}
	}
	else
	{
		R62 = 0x04;
	}
#else
	R62 = 0x04;
#endif
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R62, R62);

	DBGPRINT(RT_DEBUG_TRACE,"Reset AsicBbpTuning, BBP_R62=%d, OneSecFalseCCACnt=%ld\n", R62, pAd->RalinkCounters.OneSecFalseCCACnt);
}

/*
    ==========================================================================
    Description:
        danamic tune BBP R17 Up Boundary to find a balance between sensibility and 
        noise isolation
        
	IRQL = DISPATCH_LEVEL

    ==========================================================================
 */
static UCHAR DynamicBbpR17UpBoundaryTuning(
	IN UCHAR CurR17UpBoundary,
	IN UCHAR CurR17LowBoundary,
	IN int RssidBm)
{
	int R17UpperBound;

	// accaording to Gary's recommend.
	// BBP R17 value upper boundary should dynamic tuning by flowing rule
	// Keep orignal Upper boundary value while Rssi great than RSSI_FOR_LOW_SENSIBILITY(-58 dBm).
	// Reduce the Upper boundary value by 10 dBm while Rssi small than RSSI_FOR_LOW_SENSIBILITY(-58) but great than RSSI_FOR_MID_SENSIBILITY(-78).
	// Reduce the Upper boundary value by dalta Rssi till it touch the low boundary while Rssi small RSSI_FOR_MID_SENSIBILITY(-78).
	//
	// R17 upper Boundary
	//  ^
	//  |
	//  |--------- (ori)
	//  |         -----------\ (ori -10)
	//  |         |          | \
	//  |         |          |   \
	//  |---------------------------> (Low boundary)
	//            -58        -78       RSSI

	do
	{
		if (RssidBm < RSSI_FOR_MID_SENSIBILITY/*-78*/)
		{
			int DaltaRssi = RSSI_FOR_MID_SENSIBILITY - RssidBm;

			R17UpperBound =
				((CurR17UpBoundary - 10 - DaltaRssi) > CurR17LowBoundary ? (CurR17UpBoundary - 10 - DaltaRssi) : CurR17LowBoundary);
			break;
		}

		if (RssidBm < RSSI_FOR_LOW_SENSIBILITY/*-58*/)
		{
			R17UpperBound = CurR17UpBoundary - 10;
			break;
		}

		R17UpperBound = CurR17UpBoundary;
	} while(FALSE);

	return R17UpperBound;
}

/*
    ==========================================================================
    Description:
        danamic tune BBP R17 to find a balance between sensibility and 
        noise isolation
        
	IRQL = DISPATCH_LEVEL

    ==========================================================================
 */
VOID AsicBbpTuning(
    IN PRTMP_ADAPTER pAd)
{
    UCHAR R17, R62, R17UpperBound, R17LowerBound;
    int dbm;
    
    if ((pAd->BbpTuning.bEnable == FALSE) || (pAd->PortCfg.RadarDetect.RDMode != RD_NORMAL_MODE))
        return;

    DBGPRINT(RT_DEBUG_INFO,"AsicBbpTuning: RSSI_1=%d, RSSI_2=%d\n", pAd->PortCfg.AvgRssi - pAd->BbpRssiToDbmDelta, pAd->PortCfg.AvgRssi2 - pAd->BbpRssiToDbmDelta);

    R17 = pAd->BbpWriteLatch[17];
    if (pAd->LatchRfRegs.Channel <= 14)
    {
        R17UpperBound = pAd->BbpTuning.R17UpperBoundG;
        R17LowerBound = pAd->BbpTuning.R17LowerBoundG;
    }
    else
    {
        R17UpperBound = pAd->BbpTuning.R17UpperBoundA;
        R17LowerBound = pAd->BbpTuning.R17LowerBoundA;
    }

    dbm = pAd->PortCfg.AvgRssi - pAd->BbpRssiToDbmDelta;
    if ((pAd->RfIcType == RFIC_5325) || (pAd->RfIcType == RFIC_2529))
    {
    	if (pAd->PortCfg.AvgRssi2 > pAd->PortCfg.AvgRssi)
    	{
    		dbm = pAd->PortCfg.AvgRssi2 - pAd->BbpRssiToDbmDelta;
    	}
    }
	
	R17UpperBound = DynamicBbpR17UpBoundaryTuning(R17UpperBound, R17LowerBound, dbm);

    // Tune BBP R62, BBP R62 should be set to 0x04 in A-band
    RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R62, &R62);
    if ((pAd->LatchRfRegs.Channel <= 14) && (pAd->MacTab.Size > 0))
    {
		if (dbm < RSSI_FOR_MID_SENSIBILITY)
		{
			R62 = 0x02;
		}
		else
		{
			R62 = 0x04;
		}
    }
    else
    {
    	R62 = 0x04;
    }
    RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R62, R62);

	// Tune BBP R17
#if 0
    if (dbm >= RSSI_FOR_VERY_LOW_SENSIBILITY)
    {
        if (R17 != 0x60) // R17UpperBound)
        {
            R17 = 0x60; // R17UpperBound;
	        RTMP_WRITE_BBP_R17(pAd, R17);
        }
        DBGPRINT(RT_DEBUG_TRACE, "strong RSSI=%d, CCA=%ld, fixed R17 at 0x%x\n", dbm, pAd->RalinkCounters.OneSecFalseCCACnt, R17);
    }
    else 
#endif
    if (dbm >= RSSI_FOR_LOW_SENSIBILITY)
    {
        if (R17 != (R17UpperBound+1))
        {
            R17 = R17UpperBound + 1;
	        RTMP_WRITE_BBP_R17(pAd, R17);
        }
        DBGPRINT(RT_DEBUG_TRACE, "strong RSSI=%d, CCA=%ld, fixed R17 at 0x%x, R62=%d \n", dbm, pAd->RalinkCounters.OneSecFalseCCACnt, R17, R62);
    }
    else
    {
        if (R17 > R17UpperBound)
		{
			// according to Gary's recommend, R17 value shoudn't over R17UpperBound.
			//R17 = R17LowerBound + 0x10;
			R17 = R17UpperBound;
		}
            
    	if ((pAd->RalinkCounters.OneSecFalseCCACnt > (ULONG)pAd->BbpTuning.FalseCcaUpperThreshold) &&
            (R17 < R17UpperBound))
        {
            R17 += pAd->BbpTuning.R17Delta;
            RTMP_WRITE_BBP_R17(pAd, R17);
            DBGPRINT(RT_DEBUG_INFO, "RSSI=%d, CCA=%ld, ++R17= 0x%x, R62=%d \n", dbm, pAd->RalinkCounters.OneSecFalseCCACnt, R17, R62);
        }
        else if ((pAd->RalinkCounters.OneSecFalseCCACnt < (ULONG)pAd->BbpTuning.FalseCcaLowerThreshold) &&
            (R17 > R17LowerBound))
        {
            R17 -= pAd->BbpTuning.R17Delta;
	        RTMP_WRITE_BBP_R17(pAd, R17);
            DBGPRINT(RT_DEBUG_INFO, "RSSI=%d, CCA=%ld, --R17= 0x%x, R62=%d \n", dbm, pAd->RalinkCounters.OneSecFalseCCACnt, R17, R62);
        }
        else
        {
            DBGPRINT(RT_DEBUG_INFO, "RSSI=%d, CCA=%ld, keep R17 at 0x%x, R62=%d \n", dbm, pAd->RalinkCounters.OneSecFalseCCACnt, R17, R62);
        }
    }
}

VOID AsicAddSharedKeyEntry(
    IN PRTMP_ADAPTER pAd,
    IN UCHAR         BssIndex,
    IN UCHAR         KeyIdx,
    IN UCHAR         CipherAlg,
    IN PUCHAR        pKey,
    IN PUCHAR        pTxMic,
    IN PUCHAR        pRxMic)
{
    INT   i;
    ULONG  offset, csr0;
    SEC_CSR1_STRUC csr1;

    DBGPRINT(RT_DEBUG_TRACE,"AsicAddSharedKeyEntry(BssIndex=%d): %s key #%d\n", BssIndex, CipherName[CipherAlg], BssIndex*4 + KeyIdx);
    DBGPRINT(RT_DEBUG_TRACE, "     Key =");
#ifdef DBG
    for (i = 0; i < 16; i++)
    {
        printk("%02x:", pKey[i]);
    }
    printk("\n");
    if (pRxMic)
    {
        DBGPRINT(RT_DEBUG_TRACE, "     Rx MIC Key = ");
        for (i = 0; i < 8; i++)
        {
            printk("%02x:", pRxMic[i]);
        }
        printk("\n");
    }
    if (pTxMic)
    {
        DBGPRINT(RT_DEBUG_TRACE, "     Tx MIC Key = ");
        for (i = 0; i < 8; i++)
        {
            printk("%02x:", pTxMic[i]);
        }
        printk("\n");
    }
#endif
    //
    // fill key material - key + TX MIC + RX MIC
    //
    offset = SHARED_KEY_TABLE_BASE + (4*BssIndex + KeyIdx)*HW_KEY_ENTRY_SIZE;
    
    for (i=0; i<MAX_LEN_OF_SHARE_KEY; i++) 
    {
        RTMP_IO_WRITE8(pAd, offset + i, pKey[i]);
    }

    offset += MAX_LEN_OF_SHARE_KEY;
    if (pTxMic)
    {
        for (i=0; i<8; i++)
        {
            RTMP_IO_WRITE8(pAd, offset + i, pTxMic[i]);
        }
    }

    offset += 8;
    if (pRxMic)
    {
        for (i=0; i<8; i++)
        {
            RTMP_IO_WRITE8(pAd, offset + i, pRxMic[i]);
        }
    }

    //
    // Update cipher algorithm.
    //
    if (BssIndex == 0)
    {
	    RTMP_IO_READ32(pAd, SEC_CSR1, &csr1.word);
	    if (KeyIdx == 0)
	        csr1.field.Bss0Key0CipherAlg = CipherAlg;
	    else if (KeyIdx == 1)
	        csr1.field.Bss0Key1CipherAlg = CipherAlg;
	    else if (KeyIdx == 2)
	        csr1.field.Bss0Key2CipherAlg = CipherAlg;
	    else
	        csr1.field.Bss0Key3CipherAlg = CipherAlg;
	    RTMP_IO_WRITE32(pAd, SEC_CSR1, csr1.word);
    }
    else if (BssIndex == 1)
    {
	    RTMP_IO_READ32(pAd, SEC_CSR1, &csr1.word);
	    if (KeyIdx == 0)
	        csr1.field.Bss1Key0CipherAlg = CipherAlg;
	    else if (KeyIdx == 1)
	        csr1.field.Bss1Key1CipherAlg = CipherAlg;
	    else if (KeyIdx == 2)
	        csr1.field.Bss1Key2CipherAlg = CipherAlg;
	    else
	        csr1.field.Bss1Key3CipherAlg = CipherAlg;
	    RTMP_IO_WRITE32(pAd, SEC_CSR1, csr1.word);
    }
    else if (BssIndex == 2)
    {
	    RTMP_IO_READ32(pAd, SEC_CSR5, &csr1.word);
	    if (KeyIdx == 0)
	        csr1.field.Bss0Key0CipherAlg = CipherAlg;
	    else if (KeyIdx == 1)
	        csr1.field.Bss0Key1CipherAlg = CipherAlg;
	    else if (KeyIdx == 2)
	        csr1.field.Bss0Key2CipherAlg = CipherAlg;
	    else
	        csr1.field.Bss0Key3CipherAlg = CipherAlg;
	    RTMP_IO_WRITE32(pAd, SEC_CSR5, csr1.word);
    }
    else if (BssIndex == 3)
    {
	    RTMP_IO_READ32(pAd, SEC_CSR5, &csr1.word);
	    if (KeyIdx == 0)
	        csr1.field.Bss1Key0CipherAlg = CipherAlg;
	    else if (KeyIdx == 1)
	        csr1.field.Bss1Key1CipherAlg = CipherAlg;
	    else if (KeyIdx == 2)
	        csr1.field.Bss1Key2CipherAlg = CipherAlg;
	    else
	        csr1.field.Bss1Key3CipherAlg = CipherAlg;
	    RTMP_IO_WRITE32(pAd, SEC_CSR5, csr1.word);
    }

    //
    // enable this key entry
    //
    RTMP_IO_READ32(pAd, SEC_CSR0, &csr0);
    csr0 |= BIT32[BssIndex*4 + KeyIdx];     // turrn on the valid bit
    RTMP_IO_WRITE32(pAd, SEC_CSR0, csr0);
    
}

VOID AsicRemoveSharedKeyEntry(
    IN PRTMP_ADAPTER pAd,
    IN UCHAR         BssIndex,
    IN UCHAR         KeyIdx)
{
    ULONG  SecCsr0;

    DBGPRINT(RT_DEBUG_TRACE,"AsicRemoveSharedKeyEntry: #%d \n", BssIndex*4 + KeyIdx);

#ifdef APCLI_SUPPORT
    ASSERT(BssIndex < (MAX_MBSSID_NUM + 1));
#else
    ASSERT(BssIndex < MAX_MBSSID_NUM);
#endif
    ASSERT(KeyIdx < SHARE_KEY_NUM);
    
    RTMP_IO_READ32(pAd, SEC_CSR0, &SecCsr0);
    SecCsr0 &= (~BIT32[BssIndex*4 + KeyIdx]); // clear the valid bit
    RTMP_IO_WRITE32(pAd, SEC_CSR0, SecCsr0);
}
    
VOID AsicAddPairwiseKeyEntry(
    IN PRTMP_ADAPTER pAd,
    IN PUCHAR        pAddr,
    IN UCHAR         KeyIdx,
    IN UCHAR         CipherAlg,
    IN PUCHAR        pKey,
    IN PUCHAR        pTxMic,
    IN PUCHAR        pRxMic)
{
    INT i;
    ULONG  offset, csr2, csr3;

    DBGPRINT(RT_DEBUG_TRACE,"AsicAddPairwiseKeyEntry: #%d Alg=%s mac=%02x:%02x:%02x:%02x:%02x:%02x key=%02x-%02x-%02x-%02x-%02x-%02x..\n",
        KeyIdx, CipherName[CipherAlg], pAddr[0], pAddr[1], pAddr[2],
        pAddr[3], pAddr[4], pAddr[5], pKey[0], pKey[1], pKey[2], pKey[3], pKey[4], pKey[5]);

    offset = PAIRWISE_KEY_TABLE_BASE + (KeyIdx * HW_KEY_ENTRY_SIZE);
    for (i=0; i<MAX_LEN_OF_PEER_KEY; i++)
    {
        RTMP_IO_WRITE8(pAd, offset + i, pKey[i]);
    }
    offset += MAX_LEN_OF_PEER_KEY;
    if (pTxMic)
    {
        for (i=0; i<8; i++)
        {
            RTMP_IO_WRITE8(pAd, offset+i, pTxMic[i]);
        }
    }
    offset += 8;
    if (pRxMic)
    {
        for (i=0; i<8; i++)
        {
            RTMP_IO_WRITE8(pAd, offset+i, pRxMic[i]);
        }
    }
    offset = PAIRWISE_TA_TABLE_BASE + (KeyIdx * HW_PAIRWISE_TA_ENTRY_SIZE);
    for (i=0; i<MAC_ADDR_LEN; i++)
    {
        RTMP_IO_WRITE8(pAd, offset+i, pAddr[i]);
    }
    RTMP_IO_WRITE8(pAd, offset+MAC_ADDR_LEN, CipherAlg);

    // enable this entry
    if (KeyIdx < 32)
    {
        RTMP_IO_READ32(pAd, SEC_CSR2, &csr2);
        csr2 |= BIT32[KeyIdx];
        RTMP_IO_WRITE32(pAd, SEC_CSR2, csr2);
    }
    else
    {
        RTMP_IO_READ32(pAd, SEC_CSR3, &csr3);
        csr3 |= BIT32[KeyIdx-32];
        RTMP_IO_WRITE32(pAd, SEC_CSR3, csr3);
    }
}

VOID AsicRemovePairwiseKeyEntry(
    IN PRTMP_ADAPTER pAd,
    IN UCHAR         KeyIdx)
{
    ULONG  csr2, csr3;

    DBGPRINT(RT_DEBUG_INFO,"AsicRemovePairwiseKeyEntry: #%d \n", KeyIdx);
    
    // invalidate this entry
    if (KeyIdx < 32)
    {
        RTMP_IO_READ32(pAd, SEC_CSR2, &csr2);
        csr2 &= (~BIT32[KeyIdx]);
        RTMP_IO_WRITE32(pAd, SEC_CSR2, csr2);
    }
    else
    {
        RTMP_IO_READ32(pAd, SEC_CSR3, &csr3);
        csr3 &= (~BIT32[KeyIdx-32]);
        RTMP_IO_WRITE32(pAd, SEC_CSR3, csr3);
    }
}

BOOLEAN AsicSendCommandToMcu(
    IN PRTMP_ADAPTER pAd,
    IN UCHAR         Command,
    IN UCHAR         Token,
    IN UCHAR         Arg0,
    IN UCHAR         Arg1)
{
    HOST_CMD_CSR_STRUC H2MCmd;
    H2M_MAILBOX_STRUC  H2MMailbox;
	int	i;

    do
	{
    	RTMP_IO_READ32(pAd, H2M_MAILBOX_CSR, &H2MMailbox.word);
    	if (H2MMailbox.field.Owner == 0)
        	break;
    	RTMPusecDelay(2);
     	DBGPRINT(RT_DEBUG_TRACE, "AsicSendCommandToMcu::Mail box is busy\n");
	} while(i++ < 100);
 
	if (i >= 100)
    	DBGPRINT_ERR("H2M_MAILBOX still hold by MCU. command fail\n");
    
    H2MMailbox.field.Owner    = 1;     // pass ownership to MCU
    H2MMailbox.field.CmdToken = Token;
    H2MMailbox.field.Arg1     = Arg1;
    H2MMailbox.field.Arg0     = Arg0;
    RTMP_IO_WRITE32(pAd, H2M_MAILBOX_CSR, H2MMailbox.word);
    
    H2MCmd.word               = 0;
    H2MCmd.field.InterruptMcu = 1;
    H2MCmd.field.HostCommand  = Command;
    RTMP_IO_WRITE32(pAd, HOST_CMD_CSR, H2MCmd.word);
    DBGPRINT(RT_DEBUG_TRACE, "SW interrupt MCU (cmd=0x%02x, token=0x%02x, arg1,arg0=0x%02x,0x%02x)\n", 
    	H2MCmd.field.HostCommand, Token, Arg1, Arg0);
    return TRUE;
}

/*
    ==========================================================================
    Description:
        Update basic rate bitmap
    ==========================================================================
 */
VOID UpdateBasicRateBitmap(
	IN	PRTMP_ADAPTER	pAdapter)
{
    INT     i;

	// if A mode, always fix BasicRateBitMap
    if (pAdapter->PortCfg.PhyMode == PHY_11A)
    {
    	pAdapter->PortCfg.BasicRateBitMap = 0x15;
    }

    if (pAdapter->PortCfg.BasicRateBitMap > 4095) // (2 ^ MAX_LEN_OF_SUPPORTED_RATES) -1
        return;

    for (i = 0; i < MAX_LEN_OF_SUPPORTED_RATES; i++)
    {
        if(pAdapter->PortCfg.BasicRateBitMap & (1 << i))
        {
            pAdapter->PortCfg.SupportedRates[i] |= 0x80;
        }
        else
        {
            pAdapter->PortCfg.SupportedRates[i] &= 0x7F;
        }
    }

    DBGPRINT(RT_DEBUG_TRACE, "UpdateBasicRateBitmap::(BasicRateBitMap=%x)(%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x)\n", pAdapter->PortCfg.BasicRateBitMap,
			pAdapter->PortCfg.SupportedRates[0],pAdapter->PortCfg.SupportedRates[1],
			pAdapter->PortCfg.SupportedRates[2],pAdapter->PortCfg.SupportedRates[3],
			pAdapter->PortCfg.SupportedRates[4],pAdapter->PortCfg.SupportedRates[5],
			pAdapter->PortCfg.SupportedRates[6],pAdapter->PortCfg.SupportedRates[7],
			pAdapter->PortCfg.SupportedRates[8],pAdapter->PortCfg.SupportedRates[9],
			pAdapter->PortCfg.SupportedRates[10],pAdapter->PortCfg.SupportedRates[11]);
}

// ===========================================================================================
// bss_table.c
// ===========================================================================================


/*! \brief initialize BSS table
 *  \param p_tab pointer to the table
 *  \return none
 *  \pre
 *  \post
 */
VOID BssTableInit(
    IN BSS_TABLE *Tab) 
{
    int i;

    Tab->BssNr = 0;
    for (i = 0; i < MAX_LEN_OF_BSS_TABLE; i++) 
    {
        NdisZeroMemory(&Tab->BssEntry[i], sizeof(BSS_ENTRY));
    }
}

/*! \brief search the BSS table by SSID
 *  \param p_tab pointer to the bss table
 *  \param ssid SSID string 
 *  \return index of the table, BSS_NOT_FOUND if not in the table
 *  \pre
 *  \post
 *  \note search by sequential search
 */
ULONG BssTableSearch(
    IN BSS_TABLE *Tab, 
    IN PUCHAR Bssid) 
{
    UCHAR i;
    
    for (i = 0; i < Tab->BssNr; i++) 
    {
        //printf("comparing %s and %s\n", p_tab->bss[i].ssid, ssid);
        if (MAC_ADDR_EQUAL(&(Tab->BssEntry[i].Bssid), Bssid)) 
        { 
            return i;
        }
    }
    return (ULONG)BSS_NOT_FOUND;
}

VOID BssTableDeleteEntry(
    IN OUT  BSS_TABLE *Tab, 
    IN      PUCHAR Bssid) 
{
    UCHAR i, j;
    
    for (i = 0; i < Tab->BssNr; i++) 
    {
        //printf("comparing %s and %s\n", p_tab->bss[i].ssid, ssid);
        if (MAC_ADDR_EQUAL(&(Tab->BssEntry[i].Bssid), Bssid)) 
        {
            for (j = i; j < Tab->BssNr - 1; j++)
            {
                NdisMoveMemory(&(Tab->BssEntry[j]), &(Tab->BssEntry[j + 1]), sizeof(BSS_ENTRY));
            }
            Tab->BssNr -= 1;
            return;
        }
    }
}

UCHAR   ZeroSsid[32] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
/*! \brief
 *  \param 
 *  \return
 *  \pre
 *  \post
 */
VOID BssEntrySet(
    IN  PRTMP_ADAPTER   pAd, 
    OUT BSS_ENTRY *pBss, 
    IN PUCHAR pBssid, 
    IN CHAR Ssid[], 
    IN UCHAR SsidLen, 
    IN UCHAR BssType, 
    IN USHORT BeaconPeriod, 
    IN PCF_PARM pCfParm, 
    IN USHORT AtimWin, 
    IN USHORT CapabilityInfo, 
    IN UCHAR SupRate[], 
    IN UCHAR SupRateLen,
    IN UCHAR ExtRate[], 
    IN UCHAR ExtRateLen,
    IN UCHAR Channel,
    IN UCHAR Rssi,
    IN LARGE_INTEGER TimeStamp,
	IN UCHAR CkipFlag,
	IN PEDCA_PARM pEdcaParm,
	IN PQOS_CAPABILITY_PARM pQosCapability,
	IN PQBSS_LOAD_PARM pQbssLoad,
    IN UCHAR LengthVIE,
    IN PNDIS_802_11_VARIABLE_IEs pVIE) 
{
    COPY_MAC_ADDR(pBss->Bssid, pBssid);
    // Default Hidden SSID to be TRUE, it will be turned to FALSE after coping SSID
    pBss->Hidden = 1;   
    if (SsidLen > 0)
    {
        // For hidden SSID AP, it might send beacon with SSID len equal to 0
        // Or send beacon /probe response with SSID len matching real SSID length,
        // but SSID is all zero. such as "00-00-00-00" with length 4.
        // We have to prevent this case overwrite correct table
        if (NdisEqualMemory(Ssid, ZeroSsid, SsidLen) == 0)
        {
            NdisMoveMemory(pBss->Ssid, Ssid, SsidLen);
            pBss->SsidLen = SsidLen;
            pBss->Hidden = 0;
        }
    }
    pBss->BssType = BssType;
    pBss->BeaconPeriod = BeaconPeriod;
    if (BssType == BSS_INFRA) 
    {
        if (pCfParm->bValid) 
        {
            pBss->CfpCount = pCfParm->CfpCount;
            pBss->CfpPeriod = pCfParm->CfpPeriod;
            pBss->CfpMaxDuration = pCfParm->CfpMaxDuration;
            pBss->CfpDurRemaining = pCfParm->CfpDurRemaining;
        }
    } 
    else 
    {
        pBss->AtimWin = AtimWin;
    }

    pBss->CapabilityInfo = CapabilityInfo;
    // The privacy bit indicate security is ON, it maight be WEP, TKIP or AES
    // Combine with AuthMode, they will decide the connection methods.
    pBss->Privacy = CAP_IS_PRIVACY_ON(pBss->CapabilityInfo);
    NdisMoveMemory(pBss->SupRate, SupRate, SupRateLen);
    pBss->SupRateLen = SupRateLen;
    NdisMoveMemory(pBss->ExtRate, ExtRate, ExtRateLen);
    pBss->ExtRateLen = ExtRateLen;
    pBss->Channel = Channel;
    pBss->Rssi = Rssi;
	// Update CkipFlag. if not exists, the value is 0x0
	pBss->CkipFlag = CkipFlag;

    // New for microsoft Fixed IEs
    NdisMoveMemory(pBss->FixIEs.Timestamp, &TimeStamp, 8);
    pBss->FixIEs.BeaconInterval = BeaconPeriod;
    pBss->FixIEs.Capabilities = CapabilityInfo;

    // New for microsoft Variable IEs
    if (LengthVIE != 0)
    {
       pBss->VarIELen = LengthVIE;
        NdisMoveMemory(pBss->VarIEs, pVIE, pBss->VarIELen);
       pBss->WepStatus = BssCipherParse(pBss);
    }
    else
    {
        pBss->VarIELen = 0;
        // No SSN ID, if security is on, this is WEP algorithm
        if  (pBss->Privacy)
            pBss->WepStatus = Ndis802_11WEPEnabled;
        // No SSN ID, security is also off.
        else
            pBss->WepStatus = Ndis802_11WEPDisabled;
    }

	// new for QOS
	if (pEdcaParm)
	    NdisMoveMemory(&pBss->EdcaParm, pEdcaParm, sizeof(EDCA_PARM));
	else
	    pBss->EdcaParm.bValid = FALSE;
	if (pQosCapability)
	    NdisMoveMemory(&pBss->QosCapability, pQosCapability, sizeof(QOS_CAPABILITY_PARM));
	else
	    pBss->QosCapability.bValid = FALSE;
	if (pQbssLoad)
	    NdisMoveMemory(&pBss->QbssLoad, pQbssLoad, sizeof(QBSS_LOAD_PARM));
	else
	    pBss->QbssLoad.bValid = FALSE;
}

/*! 
 *  \brief insert an entry into the bss table
 *  \param p_tab The BSS table
 *  \param Bssid BSSID
 *  \param ssid SSID
 *  \param ssid_len Length of SSID
 *  \param bss_type
 *  \param beacon_period
 *  \param timestamp
 *  \param p_cf
 *  \param atim_win
 *  \param cap
 *  \param rates
 *  \param rates_len
 *  \param channel_idx
 *  \return none
 *  \pre
 *  \post
 *  \note If SSID is identical, the old entry will be replaced by the new one
 */
ULONG BssTableSetEntry(
    IN  PRTMP_ADAPTER   pAd, 
    OUT BSS_TABLE *Tab, 
    IN PUCHAR pBssid, 
    IN CHAR Ssid[], 
    IN UCHAR SsidLen, 
    IN UCHAR BssType, 
    IN USHORT BeaconPeriod, 
    IN CF_PARM *CfParm, 
    IN USHORT AtimWin, 
    IN USHORT CapabilityInfo, 
    IN UCHAR SupRate[],
    IN UCHAR SupRateLen,
    IN UCHAR ExtRate[],
    IN UCHAR ExtRateLen,
    IN UCHAR ChannelNo,
    IN UCHAR Rssi,
    IN LARGE_INTEGER TimeStamp,
	IN UCHAR CkipFlag,
	IN PEDCA_PARM pEdcaParm,
	IN PQOS_CAPABILITY_PARM pQosCapability,
	IN PQBSS_LOAD_PARM pQbssLoad,
    IN UCHAR LengthVIE,
    IN PNDIS_802_11_VARIABLE_IEs pVIE)
{
    ULONG   Idx;

    Idx = BssTableSearch(Tab, pBssid);
    if (Idx == BSS_NOT_FOUND) 
    {
        if (Tab->BssNr >= MAX_LEN_OF_BSS_TABLE)
            return BSS_NOT_FOUND;
        Idx = Tab->BssNr;
        BssEntrySet(pAd, &Tab->BssEntry[Idx], pBssid, Ssid, SsidLen, BssType, BeaconPeriod,
                    CfParm, AtimWin, CapabilityInfo, SupRate, SupRateLen, ExtRate, ExtRateLen,
                    ChannelNo, Rssi, TimeStamp, CkipFlag, pEdcaParm, pQosCapability, pQbssLoad, LengthVIE, pVIE);
        Tab->BssNr++;
    } 
    else
    {
        BssEntrySet(pAd, &Tab->BssEntry[Idx], pBssid, Ssid, SsidLen, BssType, BeaconPeriod,
                    CfParm, AtimWin, CapabilityInfo, SupRate, SupRateLen, ExtRate, ExtRateLen,
                    ChannelNo, Rssi, TimeStamp, CkipFlag, pEdcaParm, pQosCapability, pQbssLoad, LengthVIE, pVIE);
    }
    
    return Idx;
}

NDIS_802_11_WEP_STATUS  BssCipherParse(
    IN 	PBSS_ENTRY	pBss)
{
    PEID_STRUCT  pEid;
    PUCHAR              pTmp;
	PRSN_IE_HEADER_STRUCT		pRsnHeader;
	PCIPHER_SUITE_STRUCT		pCipher;
	USHORT				Count;
	INT				Length;
	NDIS_802_11_ENCRYPTION_STATUS	TmpCipher;
        UCHAR	WPA_OUI[] = {0x00, 0x50, 0xf2, 0x01};
        UCHAR   RSN_OUI[] = {0x00, 0x0f, 0xac};

	//
	// WepStatus will be reset later, if AP announce TKIP or AES on the beacon frame.
	//
	if (pBss->Privacy)
	{
		pBss->WepStatus     = Ndis802_11WEPEnabled;
	}
	else
	{
		pBss->WepStatus     = Ndis802_11WEPDisabled;
	}
	// Set default to disable & open authentication before parsing variable IE
	pBss->AuthMode      = Ndis802_11AuthModeOpen;
	pBss->AuthModeAux   = Ndis802_11AuthModeOpen;

	// Init WPA setting
	pBss->WPA.PairCipher    = Ndis802_11WEPDisabled;
	pBss->WPA.PairCipherAux = Ndis802_11WEPDisabled;
	pBss->WPA.GroupCipher   = Ndis802_11WEPDisabled;
	pBss->WPA.RsnCapability = 0;
	pBss->WPA.bMixMode      = FALSE;

	// Init WPA2 setting
	pBss->WPA2.PairCipher    = Ndis802_11WEPDisabled;
	pBss->WPA2.PairCipherAux = Ndis802_11WEPDisabled;
	pBss->WPA2.GroupCipher   = Ndis802_11WEPDisabled;
	pBss->WPA2.RsnCapability = 0;
	pBss->WPA2.bMixMode      = FALSE;
	Length = (INT) pBss->VarIELen;

        // Double check Var IE length, it must be no less than 22
        if (Length < 22)
        return (Ndis802_11WEPDisabled);
    
	while (Length > 0)
	{
		// Parse cipher suite base on WPA1 & WPA2, they should be parsed differently
		pTmp = ((PUCHAR) pBss->VarIEs) + pBss->VarIELen - Length;
		pEid = (PEID_STRUCT) pTmp;
		switch (pEid->Eid)
		{
			case IE_WPA:
				if (NdisEqualMemory(pEid->Octet, WPA_OUI, 4) != 1)
				{
					// if unsupported vendor specific IE
					break;
				}				
    // Skip OUI, version, and multicast suite
    // This part should be improved in the future when AP supported multiple cipher suite.
    // For now, it's OK since almost all APs have fixed cipher suite supported.
				// pTmp = (PUCHAR) pEid->Octet;
				pTmp   += 11;

	// Cipher Suite Selectors from Spec P802.11i/D3.2 P26.
	//  Value      Meaning
	//  0           None 
	//  1           WEP-40
	//  2           Tkip
	//  3           WRAP
	//  4           AES
	//  5           WEP-104
				// Parse group cipher
				switch (*pTmp)
				{
					case 1:
					case 5:	// Although WEP is not allowed in WPA related auth mode, we parse it anyway
						pBss->WPA.GroupCipher = Ndis802_11Encryption1Enabled;
						break;
					case 2:
						pBss->WPA.GroupCipher = Ndis802_11Encryption2Enabled;
						break;
					case 4:
						pBss->WPA.GroupCipher = Ndis802_11Encryption3Enabled;
						break;
					default:
						break;
				}
				// number of unicast suite
				pTmp   += 1;

				// skip all unicast cipher suites
				//Count = *(PUSHORT) pTmp;
			        NdisMoveMemory(&Count, pTmp, sizeof(USHORT));	
#ifdef BIG_ENDIAN
				Count = SWAP16(Count);		
#endif
				pTmp   += sizeof(USHORT);

				// Parsing all unicast cipher suite
				while (Count > 0)
				{
					// Skip OUI
					pTmp += 3;
					TmpCipher = Ndis802_11WEPDisabled;
					switch (*pTmp)
					{
						case 1:
						case 5: // Although WEP is not allowed in WPA related auth mode, we parse it anyway
							TmpCipher = Ndis802_11Encryption1Enabled;
							break;
						case 2:
							TmpCipher = Ndis802_11Encryption2Enabled;
							break;
						case 4:
							TmpCipher = Ndis802_11Encryption3Enabled;
							break;
						default:
							break;
					}
					if (TmpCipher > pBss->WPA.PairCipher)
					{
						// Move the lower cipher suite to PairCipherAux
						pBss->WPA.PairCipherAux = pBss->WPA.PairCipher;
						pBss->WPA.PairCipher    = TmpCipher;
					}
					else
					{
						pBss->WPA.PairCipherAux = TmpCipher;
					}
					pTmp++;
					Count--;
				}
				break;

			case IE_RSN:
				pRsnHeader = (PRSN_IE_HEADER_STRUCT) pTmp;
				
				// 0. Version must be 1
#ifdef BIG_ENDIAN
				pRsnHeader->Version = SWAP16(pRsnHeader->Version);
#endif								
				if (pRsnHeader->Version != 1)
					break;
				pTmp   += sizeof(RSN_IE_HEADER_STRUCT);

				// 1. Check group cipher
				pCipher = (PCIPHER_SUITE_STRUCT) pTmp;
				if (!RTMPEqualMemory(pTmp, RSN_OUI, 3))
					break;

				// Parse group cipher
				switch (pCipher->Type)
				{
					case 1:
					case 5:	// Although WEP is not allowed in WPA related auth mode, we parse it anyway
						pBss->WPA2.GroupCipher = Ndis802_11Encryption1Enabled;
						break;
					case 2:
						pBss->WPA2.GroupCipher = Ndis802_11Encryption2Enabled;
						break;
					case 4:
						pBss->WPA2.GroupCipher = Ndis802_11Encryption3Enabled;
						break;
					default:
						break;
				}
				// set to correct offset for next parsing
				pTmp   += sizeof(CIPHER_SUITE_STRUCT);

				// 2. Get pairwise cipher counts
				//Count = *(PUSHORT) pTmp;
				NdisMoveMemory(&Count, pTmp, sizeof(USHORT));
#ifdef BIG_ENDIAN
				Count = SWAP16(Count);		
#endif
				pTmp   += sizeof(USHORT);
				
				// 3. Get pairwise cipher
				// Parsing all unicast cipher suite
				while (Count > 0)
				{
					// Skip OUI
					pCipher = (PCIPHER_SUITE_STRUCT) pTmp;
					TmpCipher = Ndis802_11WEPDisabled;
					switch (pCipher->Type)
					{
						case 1:
						case 5: // Although WEP is not allowed in WPA related auth mode, we parse it anyway
							TmpCipher = Ndis802_11Encryption1Enabled;
							break;
						case 2:
							TmpCipher = Ndis802_11Encryption2Enabled;
							break;
						case 4:
							TmpCipher = Ndis802_11Encryption3Enabled;
							break;
						default:
							break;
					}
					if (TmpCipher > pBss->WPA2.PairCipher)
					{
						// Move the lower cipher suite to PairCipherAux
						pBss->WPA2.PairCipherAux = pBss->WPA2.PairCipher;
						pBss->WPA2.PairCipher    = TmpCipher;
					}
					else
					{
						pBss->WPA2.PairCipherAux = TmpCipher;
					}
					pTmp += sizeof(CIPHER_SUITE_STRUCT);
					Count--;
				}
				break;

			default:
				break;
		}
		Length -= (pEid->Len + 2);
	}
	if ((pBss->WPA.PairCipher == Ndis802_11WEPDisabled) &&  (pBss->WPA2.PairCipher == Ndis802_11WEPDisabled))
    return (Ndis802_11WEPDisabled);
	else if (pBss->WPA.PairCipher == Ndis802_11WEPDisabled)
	{
	   if (pBss->WPA2.PairCipherAux != Ndis802_11WEPDisabled)
	       return Ndis802_11Encryption4Enabled;
	   else
	       return pBss->WPA2.PairCipher;
	}
	else if (pBss->WPA2.PairCipher == Ndis802_11WEPDisabled)
	{
	   return pBss->WPA.PairCipher;
	}
	else
	{
	   if (pBss->WPA2.PairCipherAux != Ndis802_11WEPDisabled)
	       return Ndis802_11Encryption4Enabled;
	   else
	       return pBss->WPA2.PairCipher;
	}
}

/*
    ========================================================================

    Routine Description:
        Set Rx antenna for software diversity

    Arguments:
        pAd                 Pointer to our adapter
        Pair1				0: for E1;	1:for E2
        Pair2				0: for E4;	1:for E3

    Return Value:
        None
        
    ========================================================================
*/
VOID AsicSetRxAnt(
    IN PRTMP_ADAPTER	pAd,
    IN UCHAR			Pair1,
    IN UCHAR			Pair2)
{
	ULONG 	data;

	DBGPRINT(RT_DEBUG_TRACE, "AsicSetRxAnt, pair1=%d, pair2=%d\n", Pair1, Pair2);
	
	if ((pAd->RfIcType == RFIC_2529) && (pAd->Antenna.field.NumOfAntenna == 0)
		&& (pAd->Antenna.field.RxDefaultAntenna != SOFTWARE_DIVERSITY) && (pAd->Antenna.field.TxDefaultAntenna != SOFTWARE_DIVERSITY))
	{		
	    RTMP_IO_READ32(pAd, MAC_CSR13, &data);
	    data &= ~0x00001818;	// clear Bit 3,4,11,12

	    if (Pair1 == 0)			// pair1 Primary Ant  
		{//	Ant E1
			// data;
		}
		else
		{//	Ant E2
			data |= 0x10;
		}
		if (Pair2 == 0)			// pair2 Primary Ant  
		{//	Ant E3
			data |= 0x08;
		}
		else
		{//	Ant E4
			// data;
		}
		RTMP_IO_WRITE32(pAd, MAC_CSR13, data);
    }
    else
    {
    	UCHAR	R77;
    	
    	// end of evaluation, swicth to selected antenna pair
	    RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R77, &R77);
    	R77	&= ~0x03;		// clear Bit 0,1
    	
    	if (pAd->PortCfg.BandState == BG_BAND)
    	{
	    	if (pAd->RxAnt.Pair1PrimaryRxAnt == 0)
			{//	Ant A
				R77 = R77 | 0x03;	// <Bit1:Bit0> = <1:1>
			}
			else
			{//	Ant B
				// R77;				// <Bit1:Bit0> = <0:0>
			}
    	}
    	else
    	{
	    	if (pAd->RxAnt.Pair1PrimaryRxAnt == 0)
			{//	Ant A
				// R77;				// <Bit1:Bit0> = <0:0>
			}
			else
			{//	Ant B
				R77 = R77 | 0x03;	// <Bit1:Bit0> = <1:1>
			}
    	}
    	
    	// driver must disable Rx when switching antenna, otherwise ASIC will keep default state
		// after switching, driver needs to re-enable Rx later
		RTMP_IO_WRITE32(pAd, TXRX_CSR0, 0x0257b032);        // Disable Rx
	    RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R77, R77);
		RTMP_IO_WRITE32(pAd, TXRX_CSR0, 0x0256b032);       // enable RX of MAC block
    }
}

/*
    ========================================================================

    Routine Description:
        Radar detection routine
        This function should only be called when A mode

    Arguments:
        pAd     Pointer to our adapter

    Return Value:

    ========================================================================
*/
VOID RadarDetectionStart(
    IN PRTMP_ADAPTER	pAd)
{
	DBGPRINT(RT_DEBUG_TRACE, "RadarDetectionStart--->\n");
	
	RTMP_IO_WRITE32(pAd, TXRX_CSR0, 0x0257b032);        // Disable Rx
	
	// Set all relative BBP register to enter into radar detection mode
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R82, 0x20);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R83, 0);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R84, 0x40);

	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R18, &pAd->PortCfg.RadarDetect.BBPR18);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R18, 0xFF);
	
	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R21, &pAd->PortCfg.RadarDetect.BBPR21);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R21, 0x3F);
	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R22, &pAd->PortCfg.RadarDetect.BBPR22);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R22, 0x3F);
	
	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R16, &pAd->PortCfg.RadarDetect.BBPR16);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R16, 0xBD);
	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R17, &pAd->PortCfg.RadarDetect.BBPR17);
	if (pAd->NicConfig2.field.ExternalLNAForA)
	{
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R17, 0x28 + pAd->AModeGainValue); // if external LNA enable, this value need to be offset 0x10 (default)
	}
	else
	{
    	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R17, 0x28);
	}

	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R64, &pAd->PortCfg.RadarDetect.BBPR64);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R64, 0x21);

	RTMP_IO_WRITE32(pAd, TXRX_CSR0, 0x0256b032);       // enable RX of MAC block
}

/*
    ========================================================================

    Routine Description:
        Radar detection routine

    Arguments:
        pAd     Pointer to our adapter

    Return Value:
        TRUE    Found radar signal
        FALSE   Not found radar signal

    ========================================================================
*/
BOOLEAN RadarDetectionStop(
    IN PRTMP_ADAPTER	pAd)
{
	UCHAR	R66;

	// Need to read the result of detection first
	// If restore all registers first, then the result will be reset.
	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R66, &R66);

	// Restore all relative BBP register to exit radar detection mode
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R16, pAd->PortCfg.RadarDetect.BBPR16);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R17, pAd->PortCfg.RadarDetect.BBPR17);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R18, pAd->PortCfg.RadarDetect.BBPR18);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R21, pAd->PortCfg.RadarDetect.BBPR21);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R22, pAd->PortCfg.RadarDetect.BBPR22);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R64, pAd->PortCfg.RadarDetect.BBPR64);

	if (R66 == 1)
		return TRUE;
	else
		return FALSE;
}

/*
    ========================================================================

    Routine Description:
        Radar channel check routine

    Arguments:
        pAd     Pointer to our adapter

    Return Value:
        TRUE    need to do radar detect
        FALSE   need not to do radar detect

    ========================================================================
*/
BOOLEAN RadarChannelCheck(
    IN PRTMP_ADAPTER	pAd,
    IN UCHAR			Ch)
{
	INT		i;
	UCHAR	Channel[15]={52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140};

	for (i=0; i<15; i++)
	{
		if (Ch == Channel[i])
		{
			break;
		}
	}

	if (i != 15)
		return TRUE;
	else
		return FALSE;
}


static ULONG 	RadarSignalDurationTable[][3]=
#if 0
{//Pattern num, Theorical duration, Tolerance
 
	{0, 0x6F9B, 0x10},
	{0, 0x2B67, 0x10},
	{0, 0xECBE, 0x10},
	{0, 0x12C7B, 0x10},	// for japan
};
#else
{//Pattern num, Theorical duration, Tolerance
	{CE, 0x682A, 	0x10},
	{CE, 0x186A0, 	0x10},
	{CE, 0x1046A, 	0x10},
	{CE, 0x9C40,  	0x10},
	{CE, 0x61A8,  	0x10},
	{CE, 0x4E20,  	0x10},
	{CE, 0x411A,	0x10},
	{CE, 0x3415, 	0x10},
	{CE, 0x30D4,	0x10},
	{CE, 0x21F7,	0x10},
	{CE, 0x1A0A,	0x10},
	{CE, 0x1652,	0x10},
	{CE, 0x1388,	0x10},
	{CE, 0x2710,	0x10},
	//FCC
	{FCC, 0x36B5, 	0x10},
	{FCC, ((0x208D5+0x153AC)/2), ((0x208D5-0x153AC)/2) + 0x10},
	{FCC, ((0x186A0+0x9C40)/2), ((0x186A0-0x9C40)/2) + 0x10},
	{FCC, ((0x4E20+0x2710)/2), ((0x4E20-0x2710)/2) + 0x10},
	{FCC, 0xEA9C,	0x10},
	//Japan
	{JAP, 0x6F9B,	0x10},
	{JAP, 0x12C7B,	0x10}
};
#endif 
UCHAR RD_TAB_SIZE = (sizeof(RadarSignalDurationTable) / (sizeof(ULONG ) *3 ));
// Periodic Radar detection, switch channel will occur in RTMPHandleTBTTInterrupt()
// Before switch channel, driver needs doing channel switch announcement.
VOID RadarDetectPeriodic(
	IN PRTMP_ADAPTER	pAd)
{
	INT	i;
	ULONG 	index, RadarSignalDuration;
	UCHAR	R67, R68, R69;

	pAd->PortCfg.RadarDetect.InServiceMonitorCount++;

	for (i=0; i<pAd->ChannelListNum; i++)
	{
		if (pAd->ChannelList[i].RemainingTimeForUse > 0)
		{
			pAd->ChannelList[i].RemainingTimeForUse --;
			if ((pAd->Mlme.PeriodicRound%5) == 0)
			{
				DBGPRINT(RT_DEBUG_TRACE,"RadarDetectPeriodic - ch=%d, RemainingTimeForUse=%d\n", pAd->ChannelList[i].Channel, pAd->ChannelList[i].RemainingTimeForUse);
			}
		}
	}
	
	// need to check channel availability, after switch channel
	if (pAd->PortCfg.RadarDetect.RDMode == RD_SILENCE_MODE)
	{
		pAd->PortCfg.RadarDetect.RDCount++;
		
		// channel availability check time is 60sec, use 65 for assurance
		if (pAd->PortCfg.RadarDetect.RDCount > 65)
		{
			if (RadarDetectionStop(pAd))
	    	{
	    		RTMP_BBP_IO_READ8_BY_REG_ID(pAd, 67, &R67);
				RTMP_BBP_IO_READ8_BY_REG_ID(pAd, 68, &R68);
				RTMP_BBP_IO_READ8_BY_REG_ID(pAd, 69, &R69);
				RadarSignalDuration = (R67 << 16) + (R68 << 8) + R69;
				
	    		for (index=0; index<RD_TAB_SIZE; index++)
				{
					if ((RadarSignalDuration > (RadarSignalDurationTable[index][1] - RadarSignalDurationTable[index][2])) 
						&& (RadarSignalDuration < (RadarSignalDurationTable[index][1] + RadarSignalDurationTable[index][2]))
						&& (RadarSignalDurationTable[index][0] == pAd->PortCfg.RadarDetect.RDDurRegion))
						break;
				}
				if (index < RD_TAB_SIZE)
				{
		    		for (i=0; i<pAd->ChannelListNum; i++)
					{
						if (pAd->PortCfg.Channel == pAd->ChannelList[i].Channel)
						{
							pAd->ChannelList[i].RemainingTimeForUse = 1800;//30 min = 1800 sec
							break;
						}
					}
			    	pAd->PortCfg.Channel = APAutoSelectChannel(pAd);
					ApStop(pAd, TRUE);
					ApStartUp(pAd, TRUE);

					DBGPRINT(RT_DEBUG_TRACE,"Found radar signal(%02x:%02x:%02x), will switch to ch%d !!!\n\n\n", R67, R68, R69, pAd->PortCfg.Channel);
				}
				else
				{
					DBGPRINT(RT_DEBUG_INFO,"Mis-trigger radar detection(duration =%x) !!!\n", RadarSignalDuration);
#ifdef CARRIER_DETECTION_SUPPORT
					if (pAd->PortCfg.CarrierDetect.Enable == TRUE)
					{
						// trun on Carrier-Detection. (Carrier-Detect with CTS protection).
						CarrierDetectionStart(pAd, 1);
					}
#endif // CARRIER_DETECTON_SUPPORT //

					AsicEnableBssSync(pAd);
					pAd->PortCfg.RadarDetect.RDMode = RD_NORMAL_MODE;
					RTMP_IO_WRITE32(pAd, 0x29a0, 0);		// clear result
					// reserve the 1st CTS Frame buffer for Carrier-Sense. change command from 0x00 to 0x01.
	    			AsicSendCommandToMcu(pAd, 0x60, 0xff, 0x01, 0x01/*0x00*/);   // send start-RD command to MCU
					pAd->PortCfg.RadarDetect.DetectPattern = 0;
				}
	    	}
			else
			{
				DBGPRINT(RT_DEBUG_TRACE,"Not found radar signal, start send beacon and radar detection in service monitor\n\n");
#ifdef CARRIER_DETECTION_SUPPORT
				if (pAd->PortCfg.CarrierDetect.Enable == TRUE)
				{
					// trun on Carrier-Detection. (Carrier-Detect with CTS protection).
					CarrierDetectionStart(pAd, 1);
				}
#endif // CARRIER_DETECTION_SUPPORT //

				AsicEnableBssSync(pAd);
				pAd->PortCfg.RadarDetect.RDMode = RD_NORMAL_MODE;
				RTMP_IO_WRITE32(pAd, 0x29a0, 0);		// clear result
				// reserve the 1st CTS Frame buffer for Carrier-Sense. change command from 0x00 to 0x01.
	    		AsicSendCommandToMcu(pAd, 0x60, 0xff, 0x01, 0x01/*0x00*/);   // send start-RD command to MCU
				pAd->PortCfg.RadarDetect.DetectPattern = 0;
			}
		}
	}
}

// Periodic Radar detection, switch channel will occur in RTMPHandleTBTTInterrupt()
// Before switch channel, driver needs doing channel switch announcement.
#ifndef RTL865X_SOC
VOID RadarSMDetectPeriodic(
#else
static inline VOID RadarSMDetectPeriodic(
#endif
	IN	unsigned long data)
{
	RTMP_ADAPTER	*pAd = (RTMP_ADAPTER *)data;
	INT	i;
	ULONG 	index, RadarSignalDuration;

	DBGPRINT(RT_DEBUG_INFO,"RadarSMDetectPeriodic \n");

	// In service monitor
	if (pAd->PortCfg.RadarDetect.RDMode == RD_NORMAL_MODE
#ifdef APCLI_SUPPORT
		// ApCli will dynamic switch to new channel specifed in SWITCH-ANNOUNCEMNET IE.
		// So Our AP leave the DFS function to ApCli while ApCli interface conntected to a root-AP.
		&& pAd->ApCliTab.ApCliEntry[0].Valid == FALSE
#endif // APCLI_SUPPORT //
		)
	{
		ULONG 	RDResult;
		
		RTMP_IO_READ32(pAd, 0x29a0, &RDResult);
		if (RDResult & 0x000000FF)
		{
			RadarSignalDuration = (RDResult >> 8) & 0x00FFFFFF;

			for (index=0; index<RD_TAB_SIZE; index++)
			{
				if ((RadarSignalDuration > (RadarSignalDurationTable[index][1] - RadarSignalDurationTable[index][2])) 
					&& (RadarSignalDuration < (RadarSignalDurationTable[index][1] + RadarSignalDurationTable[index][2]))
					&& (RadarSignalDurationTable[index][0] == pAd->PortCfg.RadarDetect.RDDurRegion))
					break;
			}
			if (index < RD_TAB_SIZE)
			{
				for (i=0; i<pAd->ChannelListNum; i++)
				{
					if (pAd->PortCfg.Channel == pAd->ChannelList[i].Channel)
					{
						pAd->ChannelList[i].RemainingTimeForUse = 1800;//30 min = 1800 sec
						break;
					}
				}
			
				AsicSendCommandToMcu(pAd, 0x60, 0xff, 0x00, 0x00);   // send stop-RD command to MCU
				mdelay(5);
				AsicSendCommandToMcu(pAd, 0x60, 0xff, 0x00, 0x00);   // To Make sure command is sucessful
				
				pAd->PortCfg.Channel = APAutoSelectChannel(pAd);
				pAd->PortCfg.RadarDetect.RDMode = RD_SWITCHING_MODE;
				pAd->PortCfg.RadarDetect.CSCount = 0;
				DBGPRINT(RT_DEBUG_TRACE,"Radar signal(Duration=%x) exist when in service monitor, will switch to ch%d !!!\n\n\n", RadarSignalDuration, pAd->PortCfg.Channel);
			}
			else
			{
				DBGPRINT(RT_DEBUG_INFO,"Mis-trigger radar detection(duration =%x) !!!\n", RadarSignalDuration);
			}
		}
		
		RTMP_IO_WRITE32(pAd, 0x29a0, 0);		// clear result
	}

	RTMPAddTimer(&pAd->Mlme.DFSPeriodicTimer, HZ/10);
}

VOID QuickResponeForRateUp(
    IN	unsigned long data)
{
    RTMP_ADAPTER	*pAd = (RTMP_ADAPTER *)data;
	INT		i;
    UCHAR	UpRate, DownRate, CurrRate;
    
    //
    // walk through MAC table, see if need to change AP's TX rate toward each entry
    //
    for (i=0; i<MAX_LEN_OF_MAC_TABLE; i++)
    {
        USHORT TxTotalCnt, TxErrorRatio=0;
        MAC_TABLE_ENTRY *pEntry = &pAd->MacTab.Content[i];

        // only associated STA counts
        if ((pEntry->Valid == FALSE) || (pEntry->Sst != SST_ASSOC))
            continue;

        TxTotalCnt = pEntry->OneSecTxOkCount + pEntry->OneSecTxRetryOkCount + pEntry->OneSecTxFailCount;

        // skip those STA that has no traffic in the past period
        if (TxTotalCnt == 0)
        {
            pEntry->TxRateUpPenalty = 0;
            continue;
        }
        
        // decide the next upgrade rate and downgrade rate, if any
        CurrRate = pEntry->CurrTxRate;
        if (pAd->PortCfg.Channel > 14)  // must be in 802.11A band
        {
            if (Phy11ANextRateUpward[CurrRate] <= pEntry->MaxSupportedRate)
				UpRate = Phy11ANextRateUpward[CurrRate];
        	else
        		UpRate = CurrRate;
            DownRate = Phy11ANextRateDownward[CurrRate];
        }
        else
        {
            if (pEntry->MaxSupportedRate < RATE_FIRST_OFDM_RATE)
	        {
	        	if (Phy11BNextRateUpward[CurrRate] <= pEntry->MaxSupportedRate)
					UpRate = Phy11BNextRateUpward[CurrRate];
	        	else
	        		UpRate = CurrRate;
	            DownRate = Phy11BNextRateDownward[CurrRate];
	        }
	        else 
	        {
	        	if (Phy11BGNextRateUpward[CurrRate] <= pEntry->MaxSupportedRate)
					UpRate = Phy11BGNextRateUpward[CurrRate];
	        	else
	        		UpRate = CurrRate;
	            DownRate = Phy11BGNextRateDownward[CurrRate];
	        }
        }

        //
        // PART 1. Decide TX Quality
        //   decide TX quality based on Tx PER when enough samples are available
        //
        if (TxTotalCnt > 15)
        {
            TxErrorRatio = ((pEntry->OneSecTxRetryOkCount + pEntry->OneSecTxFailCount) *100) / TxTotalCnt;

            // downgrade TX quality if PER >= Rate-Down threshold
            if (TxErrorRatio >= RateDownPER[CurrRate])
            {
                pEntry->TxQuality[CurrRate] = DRS_TX_QUALITY_WORST_BOUND;
            }
        }

        pEntry->PER[CurrRate] = (UCHAR)TxErrorRatio;

        //
        // PART 2. Perform TX rate switching
        //   perform rate switching
        //
		if ((pEntry->TxQuality[CurrRate] >= DRS_TX_QUALITY_WORST_BOUND) && (CurrRate != DownRate))
        {
            pEntry->LastTxRate = pEntry->CurrTxRate;//record last tx rate
            pEntry->CurrTxRate = DownRate;
        }
		
        // PART 3. Post-processing if TX rate switching did happen
        //     if rate-up happen, clear all bad history of all TX rates
        //     if rate-down happen, only clear DownRate's bad history
        if (pEntry->CurrTxRate < CurrRate)
        {
            DBGPRINT(RT_DEBUG_TRACE,"DRS: [AID#%d] --TX rate = %d Mbps\n", pEntry->Aid, RateIdToMbps[DownRate]);

            // shorter stable time require more penalty in next rate UP criteria
            pEntry->TxRateUpPenalty = 0;           // no penalty

            pEntry->CurrTxRateStableTime = 0;
            pEntry->LastSecTxRateChangeAction = 2; // rate down
            pEntry->TxQuality[pEntry->CurrTxRate] = 0;
            pEntry->PER[pEntry->CurrTxRate] = 0;
        }
        else
            pEntry->LastSecTxRateChangeAction = 0; // rate no change
        
        // reset all OneSecxxx counters
        pEntry->OneSecTxFailCount = 0;
        pEntry->OneSecTxOkCount = 0;
        pEntry->OneSecTxRetryOkCount = 0;
	}
    
	pAd->QuickResponeForRateUpTimerRunning = FALSE;
}


VOID MlmeRadioOff(
	IN PRTMP_ADAPTER pAd)
{
	DBGPRINT(RT_DEBUG_TRACE,"MlmeRadioOff()\n");

	// Set Radio off flag
	RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_RADIO_OFF);

	// Link down first if any association exists
	ApStop(pAd, TRUE);

	// Abort Tx, disable RX, turn off Radio
	RTMP_IO_WRITE32(pAd, TX_CNTL_CSR, 0x001f0000);      // abort all TX rings

	// Disable Rx
	RTMP_IO_WRITE32(pAd, TXRX_CSR0, 0x0257b032);
		
	RTMP_IO_WRITE32(pAd, MAC_CSR10, 0x00001818);        // Turn off radio
}

VOID MlmeRadioOn(
	IN PRTMP_ADAPTER pAd)
{
	DBGPRINT(RT_DEBUG_TRACE, "MlmeRadioOn()\n");

	// Turn on radio, Abort TX, Disable RX
	RTMP_IO_WRITE32(pAd, MAC_CSR10, 0x00000718);        // turn on radio
	RTMP_IO_WRITE32(pAd, TX_CNTL_CSR, 0x001f0000);      // abort all TX rings

	// Disable Rx	
	RTMP_IO_WRITE32(pAd, TXRX_CSR0, 0x0257b032);

	RTMPRingCleanUp(pAd, QID_AC_BK);
	RTMPRingCleanUp(pAd, QID_AC_BE);
	RTMPRingCleanUp(pAd, QID_AC_VI);
	RTMPRingCleanUp(pAd, QID_AC_VO);
	RTMPRingCleanUp(pAd, QID_HCCA);
	RTMPRingCleanUp(pAd, QID_MGMT);
	RTMPRingCleanUp(pAd, QID_RX);

	NICResetFromError(pAd);

	// Kick Rx
	RTMP_IO_WRITE32(pAd, RX_CNTL_CSR, 0x00000001);     // enable RX of DMA block
	// enable RX of MAC block
	RTMP_IO_WRITE32(pAd, TXRX_CSR0, 0x0256b032);
	
	//
	// Enable Interrupt: to fix the bug of RadioOff/RadioOn problem (EYL).
	//
	RTMP_IO_WRITE32(pAd, INT_SOURCE_CSR, 0xffffffff);  // clear garbage interrupts
 	NICEnableInterrupt(pAd, 0);
 	
	// Clear Radio off flag
	RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_RADIO_OFF);

	ApStartUp(pAd, TRUE);
}

#ifndef RTL865X_SOC
VOID BeaconUpdateExec(
#else
static inline VOID BeaconUpdateExec(
#endif
	IN	unsigned long data)
{
	RTMP_ADAPTER *pAd = (RTMP_ADAPTER *)data;

	UpdateAllBeaconFrame(pAd);
	pAd->Mlme.bBeaconUpdate = 0;
}

#ifdef CARRIER_DETECTION_SUPPORT
#if 0	// remove to rtmp.h and change to static inline
INT isCarrierDetectExist(
	IN PRTMP_ADAPTER pAd)
{
	if (pAd->PortCfg.CarrierDetect.CD_State != CD_NORMAL)
		return TRUE;
	else
		return FALSE;
}
#endif

INT CarrierDetectReset(
	IN PRTMP_ADAPTER pAd)
{
	pAd->PortCfg.CarrierDetect.CD_State = CD_NORMAL;
	return 0;
}

VOID CarrierDetectionFsm(
	IN PRTMP_ADAPTER pAd,
	IN UINT32  CurFalseCCA)
{
	TXRX_CSR9_STRUC csr;
//	int CdResult;
	CD_STATE CurrentState = pAd->PortCfg.CarrierDetect.CD_State;
	UINT32  AvgFalseCCA;
	UINT32  FalseCCAThreshold;
	static int Cnt = 0;
	static UINT32  FalseCCA[3] = {0};

	FalseCCA[(Cnt++) % 3] = CurFalseCCA;
	AvgFalseCCA = (FalseCCA[0] + FalseCCA[1] + FalseCCA[2]) / 3;
	DBGPRINT(RT_DEBUG_INFO, "%s: AvgFalseCCA=%d, CurFalseCCA=%d\n", __FUNCTION__, AvgFalseCCA, CurFalseCCA);

	if ((pAd->PortCfg.PhyMode == PHY_11A)
		&& (pAd->PortCfg.RadarDetect.IEEE80211H == TRUE))
	{
		FalseCCAThreshold = CARRIER_DETECT_HIGH_BOUNDARY_1;
	}
	else
	{
		FalseCCAThreshold = CARRIER_DETECT_HIGH_BOUNDARY_2;
	}

	switch (CurrentState)
	{
		case CD_NORMAL:
			if (AvgFalseCCA > FalseCCAThreshold)
			{
				// disconnect all STAs behind AP.
				MacTableReset(pAd, FALSE);

				// change state to CD_SILENCE.
				pAd->PortCfg.CarrierDetect.CD_State = CD_SILENCE;

				// Stop sending CTS for Carrier Detection.
				CarrierDetectionStart(pAd, 0);

				// stop all TX actions including Beacon sending.
				AsicDisableSync(pAd);
				if ((pAd->PortCfg.PhyMode == PHY_11A)
					&& (pAd->PortCfg.RadarDetect.IEEE80211H == TRUE))
				{
					AsicSendCommandToMcu(pAd, 0x60, 0xff, 0x00, 0x00);
				}

				DBGPRINT(RT_DEBUG_TRACE, "Carrier signal detected. Change State to CD_SILENCE.\n");
			}
			break;

		case CD_SILENCE:
			// check that all TX been blocked properly.
			RTMP_IO_READ32(pAd, TXRX_CSR9, &csr.word);
			if (csr.field.bBeaconGen == 1)
			{
				// Stop sending CTS for Carrier Detection.
				CarrierDetectionStart(pAd, 0);

				AsicDisableSync(pAd);
				if ((pAd->PortCfg.PhyMode == PHY_11A)
					&& (pAd->PortCfg.RadarDetect.IEEE80211H == TRUE))
				{
					AsicSendCommandToMcu(pAd, 0x60, 0xff, 0x00, 0x00);
				}
			}

			// check carrier signal.
			if (AvgFalseCCA < CARRIER_DETECT_LOW_BOUNDARY)
{
				// change state to CD_NORMAL.
				pAd->PortCfg.CarrierDetect.CD_State = CD_NORMAL;

				// Start sending CTS for Carrier Detection.
				CarrierDetectionStart(pAd, 1);

				// start all TX actions.
				MakeAllBssBeacon(pAd);
	UpdateAllBeaconFrame(pAd);
				AsicEnableBssSync(pAd);

				if ((pAd->PortCfg.PhyMode == PHY_11A)
					&& (pAd->PortCfg.RadarDetect.IEEE80211H == TRUE))
				{
					// reserve the 1st CTS Frame buffer for Carrier-Sense. change command from 0x00 to 0x01.
					AsicSendCommandToMcu(pAd, 0x60, 0xff, 0x01, 0x01/*0x00*/);
				}

				DBGPRINT(RT_DEBUG_TRACE, "Carrier signal gone. Change State to CD_NORMAL.\n");
			}
			break;

		default:
			DBGPRINT(RT_DEBUG_ERROR, "Unknow Carrier Detection state.\n");
			break;
	}
}
#if 0 // remove to rtmp.h and change to static inline
VOID CarrierDetectionStart(
	IN PRTMP_ADAPTER pAd,
	IN BOOLEAN CTS_Protect)
{
	uint8 rt17_threshold;
	uint8 CTSSending = 0x01;

	CTSSending = (CTS_Protect == 0 ? 0x02 : 0x01);
	if (pAd->PortCfg.PhyMode == PHY_11A)
		rt17_threshold = 0x40;
	else
		rt17_threshold = 0x40;
	
	AsicSendCommandToMcu(pAd, 0x61, 0xff, CTSSending, rt17_threshold);
	mdelay(5);

	return;
}

VOID CarrierDetectionStop(
	IN PRTMP_ADAPTER	pAd)
{
	uint8 rt17_threshold;

	CarrierDetectReset(pAd);
	if (pAd->PortCfg.PhyMode == PHY_11A)
		rt17_threshold = 0x40;
	else
		rt17_threshold = 0x40;

	AsicSendCommandToMcu(pAd, 0x61, 0xff, 0x00, rt17_threshold);
	mdelay(5);

	return;
}
#endif

VOID CarrierDetectionCheck(
	IN PRTMP_ADAPTER pAd)
{
	UINT32  CarrierDetectInt;
	STA_CSR1_STRUC StaCsr1;

	if (pAd->PortCfg.RadarDetect.RDMode != RD_NORMAL_MODE)
		return;

	RTMP_IO_READ32(pAd, 0x2218, &CarrierDetectInt);
	if (CarrierDetectInt & 0xff)
	{
		RTMP_IO_WRITE32(pAd, 0x2218, 0);
		RTMP_IO_READ32(pAd, STA_CSR1, &StaCsr1.word);
		pAd->RalinkCounters.OneSecFalseCCACnt = (ULONG)StaCsr1.field.FalseCca;
		CarrierDetectionFsm(pAd, StaCsr1.field.FalseCca);

		DBGPRINT(RT_DEBUG_INFO, "OneSecFalseCCACnt = %ld\n", pAd->RalinkCounters.OneSecFalseCCACnt);
	}
}
#endif // CARRIER_DETECTION_SUPPORT //

