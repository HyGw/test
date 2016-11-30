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
	Fonchi		2006-06-26		Modify for RT61-APCli
*/

#include "rt_config.h"
#include <stdarg.h>



// ===========================================================================================
// sta_state_machine.c
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
VOID StaStateMachineInit(
	IN STA_STATE_MACHINE *S, 
	IN STA_STATE_MACHINE_FUNC Trans[], 
	IN ULONG StNr,
	IN ULONG MsgNr,
	IN STA_STATE_MACHINE_FUNC DefFunc, 
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

	return;
}

VOID ApCliCurrentStateInit(
	IN ULONG InitState,
	OUT PULONG pCurrState)
{
	*pCurrState = InitState;
	return;
}

/*! \brief This function fills in the function pointer into the cell in the state machine 
 *  \param *S   pointer to the state machine
 *  \param St   state
 *  \param Msg  incoming message
 *  \param f    the function to be executed when (state, message) combination occurs at the state machine
 *  \pre *S should be a legal pointer to the state machine, st, msg, should be all within the range, Base should be set in the initial state
 *  \post
 */
VOID StaStateMachineSetAction(
	IN STA_STATE_MACHINE *S, 
	IN ULONG St, 
	IN ULONG Msg, 
	IN STA_STATE_MACHINE_FUNC Func) 
{
	ULONG MsgIdx;
    
	MsgIdx = Msg - S->Base;

	if (St < S->NrState && MsgIdx < S->NrMsg) 
	{
		// boundary checking before setting the action
		S->TransFunc[St * S->NrMsg + MsgIdx] = Func;
	}

	return;
}

/*! \brief   This function does the state transition
 *  \param   *Adapter the NIC adapter pointer
 *  \param   *S       the state machine
 *  \param   *Elem    the message to be executed
 *  \return   None
 */
VOID StaStateMachinePerformAction(
	IN PRTMP_ADAPTER	pAd, 
	IN STA_STATE_MACHINE *S, 
	IN MLME_QUEUE_ELEM *Elem,
	USHORT ifIndex,
	PULONG pCurrState)
{
	(*(S->TransFunc[(*pCurrState) * S->NrMsg + Elem->MsgType - S->Base]))(pAd, Elem, pCurrState, ifIndex);

	return;
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
BOOLEAN StaMlmeEnqueue(
	IN	PRTMP_ADAPTER	pAd,
	IN ULONG Machine, 
	IN ULONG MsgType, 
	IN ULONG MsgLen, 
	IN VOID *Msg,
	IN USHORT ifIndex)
{
    INT Tail;
    UMAXLONG		IrqFlags;
	MLME_QUEUE	*Queue = (MLME_QUEUE *)&pAd->Mlme.Queue;

    // Do nothing if the driver is starting halt state.
    // This might happen when timer already been fired before cancel timer with mlmehalt
    if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS))
        return FALSE;

	// First check the size, it MUST not exceed the mlme queue size
	if (MsgLen > MAX_LEN_OF_MLME_BUFFER)
	{
        DBGPRINT_ERR("StaMlmeEnqueue: msg too large, size = %d \n", MsgLen);
		return FALSE;
	}
	
    if (MlmeQueueFull(Queue)) 
    {
        DBGPRINT_ERR("StaMlmeEnqueue: full, msg dropped and may corrupt MLME\n");
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
    DBGPRINT(RT_DEBUG_INFO, "StaMlmeEnqueue, num=%d\n",Queue->Num);
 
    Queue->Entry[Tail].Occupied = TRUE;
    Queue->Entry[Tail].Machine = Machine;
    Queue->Entry[Tail].MsgType = MsgType;
    Queue->Entry[Tail].MsgLen  = MsgLen;
    Queue->Entry[Tail].ifIndex  = ifIndex;
    if (Msg != NULL)
        NdisMoveMemory(Queue->Entry[Tail].Msg, Msg, MsgLen);

    RTMP_SEM_UNLOCK(&(Queue->Lock), IrqFlags);

    return TRUE;
}

/*
    ==========================================================================
    Description:
        The drop function, when machine executes this, the message is simply 
        ignored. This function does nothing, the message is freed in 
        StateMachinePerformAction()
    ==========================================================================
 */
VOID StaDrop(
    IN PRTMP_ADAPTER pAd,
    IN MLME_QUEUE_ELEM *Elem,
	PULONG pCurrState,
	USHORT ifIndex)
{
	return;
}


SHORT ApCliIfLookUp(
	IN PRTMP_ADAPTER pAd, 
	IN VOID *Msg)
{
	SHORT i;
	SHORT ifIndex = -1;
	PFRAME_802_11 pFrame = (PFRAME_802_11)Msg;

	do
	{
		for(i = 0; i < MAX_APCLI_ENTRY; i++)
		{
			//if(pAd->ApCliTab.ApCliEntry[i].Enable == FALSE)
			//	continue;

			if(	MAC_ADDR_EQUAL(pAd->ApCliTab.ApCliEntry[i].CurrentAddress, pFrame->Hdr.Addr1))
			{
				ifIndex = i;
				DBGPRINT(RT_DEBUG_TRACE, "(%s) ApCliIfIndex = %d\n", __FUNCTION__, ifIndex);
				break;
			}
		}
	} while (FALSE);

	return ifIndex;
}

BOOLEAN isValidApCliIf(
	SHORT ifIndex)
{
	if((ifIndex >= 0) && (ifIndex < MAX_APCLI_ENTRY))
		return TRUE;
	else
		return FALSE;
}

