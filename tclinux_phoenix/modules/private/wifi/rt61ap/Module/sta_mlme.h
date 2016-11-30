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
	sta_mlme.h

	Abstract:

	Revision History:
	Who			When			What
	--------	----------		----------------------------------------------
	Fonchi		2006-06-26		Modify for RT61-APCli
	
*/
#ifndef __STA_MLME_H__
#define __STA_MLME_H__


typedef VOID (*STA_STATE_MACHINE_FUNC)(VOID *Adaptor, MLME_QUEUE_ELEM *Elem, PULONG pCurrState, USHORT ifIndex);

typedef struct _STA_STATE_MACHINE {
	ULONG                           Base;
	ULONG                           NrState;
	ULONG                           NrMsg;
	ULONG                           CurrState;
	STA_STATE_MACHINE_FUNC          *TransFunc;
} STA_STATE_MACHINE, *PSTA_STATE_MACHINE;



VOID StaStateMachineInit(
	IN STA_STATE_MACHINE *S, 
	IN STA_STATE_MACHINE_FUNC Trans[], 
	IN ULONG StNr,
	IN ULONG MsgNr,
	IN STA_STATE_MACHINE_FUNC DefFunc, 
	IN ULONG InitState, 
	IN ULONG Base);

VOID StaStateMachineSetAction(
	IN STA_STATE_MACHINE *S, 
	IN ULONG St, 
	IN ULONG Msg, 
	IN STA_STATE_MACHINE_FUNC Func);


#endif /* __STA_MLME_H__ */

