
#ifndef	__BBU_SOC_H__
#define	__BBU_SOC_H__


//#include "rt_config.h"


#define POLLING_MODE_DETECT_INTV  300

/* mark PS poll frame sent in mix mode */
#ifdef RTMP_MAC_PCI
/*
	Note:
	(1) When SP is not started, try to mark a flag to record if the legacy ps
		packet is handled in statistics handler;
	(2) When SP is started, increase the UAPSD count number for the legacy PS.
*/

#if defined(BB_SOC) && defined(MULTI_CORE_SUPPORT)

#define UAPSD_MR_MIX_PS_POLL_RCV(__pAd, __pMacEntry)						\
		if ((__pMacEntry)->bAPSDFlagSpRoughUse == 0)						\
		{																	\
			if ((__pMacEntry)->bAPSDFlagSPStart == 0)						\
			{																\
				if ((__pMacEntry)->bAPSDFlagLegacySent == 1)				\
				(__pMacEntry)->bAPSDFlagLegacySent = 1;						\
			}																\
			else															\
			{																\
				(__pMacEntry)->UAPSDTxNum ++;								\
			}																\
		}
#endif /*MULTI_CORE_SUPPORT*/
#endif /* RTMP_MAC_PCI */

#define BAENTRY_FREE_PROTECT(__pAd,__pBAEntry, __pEntry)						\
	__pBAEntry =&__pAd->BATable.BAOriEntry[Idx];						\
	NdisAcquireSpinLock(&__pAd->BATabLock);							\
	if (__pBAEntry->ORI_BA_Status != Originator_NONE)					\
	{																\
		__pEntry = &pAd->MacTab.Content[__pBAEntry->Wcid];			\
		__pEntry->BAOriWcidArray[__pBAEntry->TID] = 0;				\
		pEntry->BAOriWcidArray[__pBAEntry->TID] = 0;					\
		if (__pBAEntry->ORI_BA_Status == Originator_Done)				\
		{															\
			pAd->BATable.numDoneOriginator -= 1;					\
		 	pEntry->TXBAbitmap &= (~(1<<(__pBAEntry->TID) ));			\
			DBGPRINT(RT_DEBUG_TRACE, ("BATableFreeOriEntry numAsOriginator= %ld\n", pAd->BATable.numAsOriginator));	\
		}															\
		ASSERT(pAd->BATable.numAsOriginator != 0);					\
		pAd->BATable.numAsOriginator -= 1;							\
		__pBAEntry->ORI_BA_Status = Originator_NONE;					\
		__pBAEntry->Token = 0;										\
	}																\
	NdisReleaseSpinLock(&pAd->BATabLock);

#define BA_ORISESS_SETUP_PROTECT(__pAd,__pEntry,__Idx,__TID)		\
	NdisAcquireSpinLock(&__pAd->BATabLock);							\
	__pEntry->BAOriWcidArray[__TID] = __Idx;						\
	NdisReleaseSpinLock(&__pAd->BATabLock);

extern void os_TCIfQuery (unsigned short query_id, void* result, void* result2);


typedef struct _BBUPollingMode{
	RALINK_TIMER_STRUCT			PollingModeDetect;	
	BOOLEAN							PollingModeDetectRunning;	
	struct work_struct					PollingDataBH;
	UINT32   							pAd_va;
	NDIS_SPIN_LOCK          			PollingModeLock;
}BBUPollingMode,*PBBUPollingMode;

extern void tc3162_enable_irq(unsigned int irq);
extern void tc3162_disable_irq(unsigned int irq);
extern VOID PeriodicPollingModeDetect(
	IN PVOID SystemSpecific1, 
    IN PVOID FunctionContext, 
    IN PVOID SystemSpecific2, 
    IN PVOID SystemSpecific3);
DECLARE_TIMER_FUNCTION(PeriodicPollingModeDetect);

extern VOID PollingModeIsr(struct work_struct *work);
/*
    ========================================================================

    Routine Description:
       Trendchip DMT Trainning status detect

    Arguments:
        data                     Point to RTMP_ADAPTER

    Return Value:
        NONE

    Note: 

    ========================================================================
*/
extern void BBU_PCIE_Init(void);
extern VOID BBUPollingModeClose(IN RTMP_ADAPTER *pAd);
extern VOID BBUPollingModeInit(IN RTMP_ADAPTER *pAd);
extern VOID BBUPollingModeStart(IN RTMP_ADAPTER *pAd);
extern VOID BBUPrepareMAC(IN RTMP_ADAPTER *pAd, PUCHAR macaddr );
#if defined(TCSUPPORT_WLAN_GPIO)
extern int BBUAfterInsMod(IN RTMP_ADAPTER *pAd);
extern int BBUBeforeRmMod(IN RTMP_ADAPTER *pAd);
#endif
#endif

