/** \file sys_service.c
 * sys_service.c
 *
 *  This file implements the required system services for the API-II using a
 * Linux OS running on the UVB.  The user should replace the functions provided
 * here with the equivalent based on their OS and hardware.
 *
 * Copyright (c) 2010, Zarlink Semiconductor, Inc.
 */
#include "vp_api_types.h"
#include "sys_service.h"
#include "linux/spinlock.h"
#include "vp_api.h"//Roger_lt


/*
 * VpSysWait() function implementation is needed only for CSLAC devices
 * (880, 790). For other devices this function could be commented.
 */
void _TDI_delay(int t){
    if(t<1000){
    	udelay(t);
    }
    else{
    	mdelay(t/1000);
    	udelay(t%1000);
    }
}
void
VpSysWait(
    uint8 time)  /* Time specified in increments of 125uS (e.g. 4 = 500uS) */
{
#ifdef D2MOD_VP
    uint32  n;
    /*
     * Convert the time from 125uS to be one usec
     */
    n = time * 125;
    _TDI_delay(n);     /* D2MOD, _TIC support for udelay() */
    return;
#else
    /* Blocking delay function added here based on OS */
#endif
}
void VpSysSemaphoreInit(int semaphoreId)
{
#if 0
	uint8 i,j;

	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			spin_lock_init(&VP_CRITICAL_SEC_lock[i][j]);
		}
	}
#endif
	return;
}

/*
 * VpSysEnterCritical(), VpSysExitCritical():
 *
 *  These functions allow for disabling interrupts while executing nonreentrant
 * portions of VoicePath API code. Note that the following implementations of
 * enter/exit critical section functions are simple implementations. These
 * functions could be expanded (if required) to handle different critical
 * section types differently.
 *
 * Params:
 *  VpDeviceIdType deviceId: Device Id (chip select ID)
 *  VpCriticalSecType: Critical section type
 *
 * Return:
 *  Number of critical sections currently entered for the device.
 */

uint8
VpSysEnterCritical(
    VpDeviceIdType deviceId,
    VpCriticalSecType criticalSecType)
{
#if 0
	uint8 i,j;
	switch (criticalSecType){
		case VP_MPI_CRITICAL_SEC:
			VP_CRITICAL_SEC_depth[deviceId][VP_MPI_CRITICAL_SEC]++;
			spin_lock_irqsave(&VP_CRITICAL_SEC_lock[deviceId][VP_MPI_CRITICAL_SEC], VP_CRITICAL_SEC_flags[deviceId][VP_MPI_CRITICAL_SEC]);
			break;
		case VP_HBI_CRITICAL_SEC:
			VP_CRITICAL_SEC_depth[deviceId][VP_HBI_CRITICAL_SEC]++;
			spin_lock_irqsave(&VP_CRITICAL_SEC_lock[deviceId][VP_HBI_CRITICAL_SEC], VP_CRITICAL_SEC_flags[deviceId][VP_HBI_CRITICAL_SEC]);
			break;
		case VP_CODE_CRITICAL_SEC:
			VP_CRITICAL_SEC_depth[deviceId][VP_CODE_CRITICAL_SEC]++;
			spin_lock_irqsave(&VP_CRITICAL_SEC_lock[deviceId][VP_CODE_CRITICAL_SEC], VP_CRITICAL_SEC_flags[deviceId][VP_CODE_CRITICAL_SEC]);
			break;
		default:
			
			break;
	}
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			if(VP_CRITICAL_SEC_depth[i][j]>1){
				printk("VP_CRITICAL_SEC_depth[%d][%d]= %d >1 Error!\r\n",i,j,VP_CRITICAL_SEC_depth[i][j]);
			}
		}
	}
#endif

    /*
     * Code to check if semaphore can be taken, and if so increment. Block if
     * semaphore cannot be taken for this process ID.
     */
    /*
     * criticalDepth++;
     * return criticalDepth;
     */
    /* Prevent compiler from generating error */

    return 0;
} /* VpSysEnterCritical() */
uint8
VpSysExitCritical(
    VpDeviceIdType deviceId,
    VpCriticalSecType criticalSecType)
{
    /* Code to decrement semaphore */
#if 0
	switch (criticalSecType){
		case VP_MPI_CRITICAL_SEC:
			VP_CRITICAL_SEC_depth[deviceId][VP_MPI_CRITICAL_SEC]--;
			spin_unlock_irqrestore(&VP_CRITICAL_SEC_lock[deviceId][VP_MPI_CRITICAL_SEC], VP_CRITICAL_SEC_flags[deviceId][VP_MPI_CRITICAL_SEC]);
			break;
		case VP_HBI_CRITICAL_SEC:
			VP_CRITICAL_SEC_depth[deviceId][VP_HBI_CRITICAL_SEC]--;
			spin_unlock_irqrestore(&VP_CRITICAL_SEC_lock[deviceId][VP_HBI_CRITICAL_SEC], VP_CRITICAL_SEC_flags[deviceId][VP_HBI_CRITICAL_SEC]);
			break;
		case VP_CODE_CRITICAL_SEC:
			VP_CRITICAL_SEC_depth[deviceId][VP_CODE_CRITICAL_SEC]--;
			spin_unlock_irqrestore(&VP_CRITICAL_SEC_lock[deviceId][VP_CODE_CRITICAL_SEC], VP_CRITICAL_SEC_flags[deviceId][VP_CODE_CRITICAL_SEC]);
			break;
		default:
			
			break;
	}
#endif
	/*
     * criticalDepth--;
     * return criticalDepth;
     */
    /* Prevent compiler from generating error */
    return 0;
} /* VpSysExitCritical() */
/**
 * VpSysDisableInt(), VpSysEnableInt(), and VpSysTestInt()
 *
 *  These functions are used by the CSLAC device family for interrupt driven
 * polling modes. These are called by the API to detect when a non-masked
 * device status has changed.  If using SIMPLE_POLL mode, these functions do not
 * require implementation.
 *
 * Preconditions:
 *  None. The implementation of these functions is architecture dependent.
 *
 * Postconditions:
 *  VpSysDisableInt() - The interrupt associated with the deviceId passed is
 * disabled.
 *
 * VpSysEnableInt() - The interrupt associated with the deviceId passed is
 * enabled.
 *
 * VpSysTestInt() - The return value is TRUE if an interrupt occurred, otherwise
 * return FALSE.
 *
 * These functions are needed only for CSLAC devices
 * (880, 790). For other devices these functions could be commented.
 *
 */
void
VpSysDisableInt(
    VpDeviceIdType deviceId)
{
    return;
}
void
VpSysEnableInt(
    VpDeviceIdType deviceId)
{
    return;
}
bool
VpSysTestInt(
    VpDeviceIdType deviceId)
{
    return FALSE;
}
/**
 * VpSysDtmfDetEnable(), VpSysDtmfDetDisable()
 *
 *  These functions are used by the CSLAC device family for devices that do not
 * internally detect DTMF. It is used for Caller ID type-II and is provided to
 * enable external DTMF detection.
 *
 * Preconditions:
 *  None. The implementation of these functions is application dependent.
 *
 * Postconditions:
 *  VpSysDtmfDetEnable() - The device/channel resource for DTMF detection is
 * enabled.
 *
 *  VpSysDtmfDetDisable() - The device/channel resource for DTMF detection is
 * disabled.
 *
 * These functions are needed only for CSLAC devices
 * (880, 790). For other devices these functions could be commented.
 *
 */
void
VpSysDtmfDetEnable(
    VpDeviceIdType deviceId,
    uint8 channelId)
{
}
void
VpSysDtmfDetDisable(
    VpDeviceIdType deviceId,
    uint8 channelId)
{
}
/*
 * The following functions VpSysTestHeapAcquire(),  VpSysTestHeapRelease()
 * VpSysPcmCollectAndProcess() and are needed only for CSLAC devices
 * (880). For other devices these functions could be commented. Please see
 * the LineTest API documentation for function requirements.
 */
 #if 0 //Roger_lt
void *
VpSysTestHeapAcquire(
    uint8 *pHeapId)
{
    return VP_NULL;
} /* VpSysTestHeapAcquire() */
bool
VpSysTestHeapRelease(
    uint8 heapId)
{
    return TRUE;
} /* VpSysTestHeapRelease() */
#else
	static VpTestHeapType gTestHeap;
	
	/**
	 *	Function: VpSysTestHeapAcquire ()
	 *
	 *	This function is called by the VP-API when an internal PCM collect test
	 *	buffer is required.
	 *
	 * Parameters:
	 *	pHeapId 	 - not used in this app
	 *
	 * Returns:
	 *
	 */
	void *
	VpSysTestHeapAcquire(
		uint8 *pHeapId)
	{
	//	  printf("VpSysTestHeapAcquire()\n");
		return &gTestHeap;
	} /* VpSysTestHeapRelease() */
	
	/**
	 *	Function: VpSysTestHeapAcquire ()
	 *
	 *	This function is called by the VP-API when the VP-API is done with the
	 *	internal test heap.
	 *
	 * Parameters:
	 *	pHeapId 	 - not used in this app
	 *
	 * Returns:
	 *
	 */
	bool
	VpSysTestHeapRelease(
		uint8 heapId)
	{
	//	  printf("VpSysTestHeapRelease()\n");
		return TRUE;
	} /* VpSysTestHeapRelease() */
#endif

void
VpSysPcmCollectAndProcess(
    void *pLineCtx,
    VpDeviceIdType deviceId,
    uint8 channelId,
    uint8 startTimeslot,
    uint16 operationTime,
    uint16 settlingTime,
    uint16 operationMask)
{
} /* VpSysPcmCollectAndProcess() */

