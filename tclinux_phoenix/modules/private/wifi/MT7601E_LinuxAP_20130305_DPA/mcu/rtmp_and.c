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
	rtmp_and.c

	Abstract:
	on-chip CPU related codes

	Revision History:
	Who         When          What
	--------    ----------    ----------------------------------------------
*/

#include	"rt_config.h"

#ifdef RTMP_MAC_PCI
#define ANDES_FIRMWARE_IMAGE_BASE     0x80000
#endif /* RTMP_MAC_PCI */

#ifdef RT_BIG_ENDIAN
typedef struct GNU_PACKED _TXINFO_NMAC_CMD_PKT{
	UINT32 info_type:2;
	UINT32 d_port:3;
	UINT32 cmd_type:7;
	UINT32 cmd_seq:4;
	UINT32 pkt_len:16;
}TXINFO_NMAC_CMD_PKT;
#else
typedef struct GNU_PACKED _TXINFO_NMAC_CMD_PKT {
	UINT32 pkt_len:16;
	UINT32 cmd_seq:4;
	UINT32 cmd_type:7;
	UINT32 d_port:3;
	UINT32 info_type:2;
}TXINFO_NMAC_CMD_PKT;
#endif /* RT_BIG_ENDIAN */


#ifdef RTMP_PCI_SUPPORT
/*
	========================================================================
	
	Routine Description:
		erase on-chip firmware image in MAC ASIC

	Arguments:
		Adapter						Pointer to our adapter

	IRQL = PASSIVE_LEVEL
		
	========================================================================
*/
INT ral_pci_andes_erasefw(RTMP_ADAPTER *pAd)
{
	NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
	UINT32 ILMLen, DLMLen;
	USHORT FWVersion, BuildVersion;
	UINT32 Loop = 0, idx = 0, val = 0;
	UINT32 MACValue;
	UINT32 StartOffset, EndOffset;
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;
	
	ILMLen = (*(pChipCap->FWImageName + 3) << 24) | (*(pChipCap->FWImageName + 2) << 16) |
			 (*(pChipCap->FWImageName + 1) << 8) | (*pChipCap->FWImageName);

	DLMLen = (*(pChipCap->FWImageName + 7) << 24) | (*(pChipCap->FWImageName + 6) << 16) |
			 (*(pChipCap->FWImageName + 5) << 8) | (*(pChipCap->FWImageName + 4));

	FWVersion = (*(pChipCap->FWImageName + 11) << 8) | (*(pChipCap->FWImageName + 10));

	BuildVersion = (*(pChipCap->FWImageName + 9) << 8) | (*(pChipCap->FWImageName + 8));
	
	DBGPRINT(RT_DEBUG_TRACE, ("FW Version:%d.%d.%02d ", (FWVersion & 0xf000) >> 8,
						(FWVersion & 0x0f00) >> 8, FWVersion & 0x00ff));
	DBGPRINT(RT_DEBUG_TRACE, ("Build:%x\n", BuildVersion));
	DBGPRINT(RT_DEBUG_TRACE, ("Build Time:"));

	for (Loop = 0; Loop < 16; Loop++)
		DBGPRINT(RT_DEBUG_TRACE, ("%c", *(pChipCap->FWImageName + 16 + Loop)));

	DBGPRINT(RT_DEBUG_TRACE, ("\n"));

	DBGPRINT(RT_DEBUG_TRACE, ("ILM Length = %d(bytes)\n", ILMLen));
	DBGPRINT(RT_DEBUG_TRACE, ("DLM Length = %d(bytes)\n", DLMLen));
	
	RTMP_IO_WRITE32(pAd, PCIE_REMAP_BASE4, 0x0);

	if (pChipCap->IsComboChip)
	{
		RTMP_IO_WRITE32(pAd, CPU_CTL, 0x0);
		RTMP_IO_WRITE32(pAd, CPU_CTL, (0x1<<20));
		RTMP_IO_WRITE32(pAd, RESET_CTL, 0x10);
		RTMP_IO_WRITE32(pAd, RESET_CTL, 0x300);
		RTMP_IO_WRITE32(pAd, COM_REG0, 0x0);
	
		StartOffset = 96;
	}
	else
	{
		RTMP_IO_WRITE32(pAd, RESET_CTL, 0x10);	/* Reset FCE */
		RTMP_IO_WRITE32(pAd, RESET_CTL, 0x200);	/* Reset CPU */
		RTMP_IO_WRITE32(pAd, COM_REG0, 0x0);	/* Clear MCU ready bit */

		StartOffset = 32;
	}

	EndOffset = 32 + ILMLen;
	
	/* Erase ILM code */
	for (idx = StartOffset; idx < EndOffset; idx += 4)
		RTMP_IO_WRITE32(pAd, 0x80000 + (idx - 32), 0);

	if (pChipCap->IsComboChip)
	{
		/* Loading IVB part into last 64 bytes of ILM */
		StartOffset = 32;
		EndOffset = 96;
	
		for (idx = StartOffset; idx < EndOffset; idx += 4)
			RTMP_IO_WRITE32(pAd, 0x80000 + (0x54000 - 0x40) + (idx - 32), 0);
	}

	RTMP_IO_WRITE32(pAd, PCIE_REMAP_BASE4, 0x80000);

	StartOffset = 32 + ILMLen;
	EndOffset = 32 + ILMLen + DLMLen;
	
	/* Load DLM code */
	for (idx = StartOffset; idx < EndOffset; idx += 4)
	{
		val = (*(pChipCap->FWImageName + idx)) +
		   (*(pChipCap->FWImageName + idx + 3) << 24) +
		   (*(pChipCap->FWImageName + idx + 2) << 16) +
		   (*(pChipCap->FWImageName + idx + 1) << 8);

		RTMP_IO_WRITE32(pAd, 0x80000 + (idx - 32 - ILMLen), 0);
	}
	
	RTMP_IO_READ32(pAd, COM_REG0, &val);
	DBGPRINT(RT_DEBUG_TRACE, ("%s: COM_REG0(0x%x) = 0x%x\n", __FUNCTION__, COM_REG0, val));
	RTMP_IO_READ32(pAd, RESET_CTL, &val);
	DBGPRINT(RT_DEBUG_TRACE, ("%s: RESET_CTL(0x%x) = 0x%x\n", __FUNCTION__, RESET_CTL, val));
	
	return Status;
}



NDIS_STATUS PCILoadFirmwareToAndes(RTMP_ADAPTER *pAd)
{
	NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
	UINT32 ILMLen, DLMLen;
	USHORT FWVersion, BuildVersion;
	UINT32 Loop = 0, idx = 0, val = 0;
	UINT32 MACValue;
	UINT32 StartOffset, EndOffset;
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;

	if (pChipCap->IsComboChip)
	{
loadfw_protect:
		RTMP_IO_READ32(pAd, SEMAPHORE_00, &MACValue);
		Loop++;

		if (((MACValue & 0x01) == 0) && (Loop < 10000))
			goto loadfw_protect;
	}

	/* check MCU if ready */
	//RTMP_IO_READ32(pAd, RESET_CTL, &val);

	//RTMP_IO_WRITE32(pAd, RESET_CTL, 0x200);	/* CPU reset/enable */
	//RTMP_IO_WRITE32(pAd, 0x9c8, 0x10000);	/* CPU reset/enable */
	//RTMP_IO_READ32(pAd, 0x9c4, &MACValue);	/* CPU reset/enable */
	//MACValue &=~0x5;
	//RTMP_IO_WRITE32(pAd, 0x9c4, 0x10000);	/* CPU reset/enable */
	//RTMP_IO_WRITE32(pAd, RESET_CTL, 0x300);	/* Reset FCE */
	RTMP_IO_READ32(pAd, COM_REG0, &MACValue);
	
	//DBGPRINT(RT_DEBUG_TRACE, ("%s: RESET_CTL(0x%x) = 0x%x, chip %d\n", __FUNCTION__, RESET_CTL, val, pChipCap->IsComboChip));
	if ((MACValue == 0x01) && (pChipCap->IsComboChip))
	//if (0)//MACValue == 0x01)
	{
		RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_MCU_SEND_IN_BAND_CMD);
		goto done;
	}

	ILMLen = (*(pChipCap->FWImageName + 3) << 24) | (*(pChipCap->FWImageName + 2) << 16) |
			 (*(pChipCap->FWImageName + 1) << 8) | (*pChipCap->FWImageName);

	DLMLen = (*(pChipCap->FWImageName + 7) << 24) | (*(pChipCap->FWImageName + 6) << 16) |
			 (*(pChipCap->FWImageName + 5) << 8) | (*(pChipCap->FWImageName + 4));

	FWVersion = (*(pChipCap->FWImageName + 11) << 8) | (*(pChipCap->FWImageName + 10));

	BuildVersion = (*(pChipCap->FWImageName + 9) << 8) | (*(pChipCap->FWImageName + 8));

	DBGPRINT(RT_DEBUG_OFF, ("FW Version:%d.%d.%02d ", (FWVersion & 0xf000) >> 8,
						(FWVersion & 0x0f00) >> 8, FWVersion & 0x00ff));
	DBGPRINT(RT_DEBUG_OFF, ("Build:%x\n", BuildVersion));
	DBGPRINT(RT_DEBUG_OFF, ("Build Time:"));

	for (Loop = 0; Loop < 16; Loop++)
		DBGPRINT(RT_DEBUG_OFF, ("%c", *(pChipCap->FWImageName + 16 + Loop)));

	DBGPRINT(RT_DEBUG_OFF, ("\n"));

	DBGPRINT(RT_DEBUG_OFF, ("ILM Length = %d(bytes)\n", ILMLen));
	DBGPRINT(RT_DEBUG_OFF, ("DLM Length = %d(bytes)\n", DLMLen));
	
	RTMP_IO_WRITE32(pAd, PCIE_REMAP_BASE4, 0x0);	/* map to RAM */

	if (pChipCap->IsComboChip)
		StartOffset = 96;
	else
		StartOffset = 32;	

	EndOffset = 32 + ILMLen;

	/* Load ILM code */
	for (idx = StartOffset; idx < EndOffset; idx += 4)
	{
		MACValue = (*(pChipCap->FWImageName + idx)) +
		   (*(pChipCap->FWImageName + idx +3) << 24) +
		   (*(pChipCap->FWImageName + idx + 2) << 16) +
		   (*(pChipCap->FWImageName + idx + 1) << 8);

		RTMP_IO_WRITE32(pAd, 0x80000 + (idx - 32), MACValue);
	}

	//DBGPRINT(RT_DEBUG_TRACE, ("%s: RESET_CTL(0x%x) = 0x%x, chip %d\n", __FUNCTION__, RESET_CTL, val, pChipCap->IsComboChip));
	if (pChipCap->IsComboChip)
	{
		/* Loading IVB part into last 64 bytes of ILM */
		StartOffset = 32;
		EndOffset = 96;
	
		for (idx = StartOffset; idx < EndOffset; idx += 4)
		{
			MACValue = (*(pChipCap->FWImageName + idx)) +
				(*(pChipCap->FWImageName + idx + 3) << 24) +
				(*(pChipCap->FWImageName + idx + 2) << 16) +
				(*(pChipCap->FWImageName + idx + 1) << 8);

			RTMP_IO_WRITE32(pAd, 0x80000 + (0x54000 - 0x40) + (idx - 32), MACValue);
		}
	}

	RTMP_IO_WRITE32(pAd, PCIE_REMAP_BASE4, 0x80000);

	StartOffset = 32 + ILMLen;
	EndOffset = 32 + ILMLen + DLMLen;
	
	/* Load DLM code */
	for (idx = StartOffset; idx < EndOffset; idx += 4)
	{
		MACValue = (*(pChipCap->FWImageName + idx)) +
		   (*(pChipCap->FWImageName + idx + 3) << 24) +
		   (*(pChipCap->FWImageName + idx + 2) << 16) +
		   (*(pChipCap->FWImageName + idx + 1) << 8);

		RTMP_IO_WRITE32(pAd, 0x80000 + (idx - 32 - ILMLen), MACValue);
	}
	

	RTMP_IO_WRITE32(pAd, PCIE_REMAP_BASE4, 0x0);

	if (pChipCap->IsComboChip)
	{
		/* Trigger Firmware */
		RTMP_IO_WRITE32(pAd, INT_LEVEL, 0x03);
	}
	else
	{
		printk("\x1b[31m%s: ....\x1b[m\n", __FUNCTION__);
		/* need to reset FCE for each FW loading */
		RTMP_IO_WRITE32(pAd, RESET_CTL, 0x10);  /* reset FCE */
		RTMP_IO_WRITE32(pAd, RESET_CTL, 0x300);	/* CPU reset/enable */
	}

	/* check MCU if ready */
	Loop = 0;
	do
	{
		RTMP_IO_READ32(pAd, COM_REG0, &MACValue);
		if (MACValue == 0x1)
			break;
		RtmpOsMsDelay(10);
		Loop++;
	} while (Loop <= 20);

	DBGPRINT(RT_DEBUG_TRACE, ("%s: COM_REG0(0x%x) = 0x%x\n", __FUNCTION__, COM_REG0, MACValue));
	
	if (MACValue != 0x1)
	{
		RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_MCU_SEND_IN_BAND_CMD);
		Status = NDIS_STATUS_FAILURE;
	}
	else
		RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_MCU_SEND_IN_BAND_CMD);

	RTMP_IO_READ32(pAd, 0x9a8, &MACValue);
	printk("\x1b[31m%s: 0x9a8 ....%x\x1b[m\n", __FUNCTION__, MACValue);
	RTMP_IO_READ32(pAd, 0x9ac, &MACValue);
	printk("\x1b[31m%s: 0x9ac ....%x\x1b[m\n", __FUNCTION__, MACValue);
done:
	if (pChipCap->IsComboChip)
		RTMP_IO_WRITE32(pAd, SEMAPHORE_00, 0x1);
	
	return Status;
}

static UCHAR *txinfo_type_str[]={"PKT", "CMD", "RSV"};
static UCHAR *txinfo_d_port_str[]={"WLAN", "CPU_RX", "CPU_TX", "HOST", "VIRT_RX", "VIRT_TX", "DROP"};

VOID dump_cmd_txinfo(RTMP_ADAPTER *pAd, TXINFO_STRUC *pTxInfo)
{
	DBGPRINT(RT_DEBUG_OFF, ("TxInfo:\n"));
	{
		struct _TXINFO_NMAC_CMD *cmd_txinfo = (struct _TXINFO_NMAC_CMD *)pTxInfo;
	
		hex_dump("Raw Data: ", (UCHAR *)pTxInfo, sizeof(TXINFO_STRUC));
		DBGPRINT(RT_DEBUG_OFF, ("\t Info_Type=%d(%s)\n", cmd_txinfo->info_type, txinfo_type_str[cmd_txinfo->info_type]));
		DBGPRINT(RT_DEBUG_OFF, ("\t d_port=%d(%s)\n", cmd_txinfo->d_port, txinfo_d_port_str[cmd_txinfo->d_port]));
		DBGPRINT(RT_DEBUG_OFF, ("\t cmd_type=%d\n", cmd_txinfo->cmd_type));
		DBGPRINT(RT_DEBUG_OFF, ("\t cmd_seq=%d\n", cmd_txinfo->cmd_seq));
		DBGPRINT(RT_DEBUG_OFF, ("\t pkt_len=0x%x\n", cmd_txinfo->pkt_len));
	}

#ifdef RTMP_MAC
	DBGPRINT(RT_DEBUG_OFF, ("\t"));
#endif /* RTMP_MAC */
}

INT PCIKickOutCmd(
	PRTMP_ADAPTER pAd, 
	UCHAR *Buf, 
	UINT32 Len)
{
	NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
	ULONG	IrqFlags = 0;
	BOOLEAN bIntContext = FALSE;
	ULONG FreeNum;
	ULONG SwIdx = 0, SrcBufPA;
	UCHAR *pSrcBufVA;
	UINT SrcBufLen = 0;
	PACKET_INFO PacketInfo;
	TXD_STRUC *pTxD;
	TXINFO_STRUC *pTxInfo;
	PNDIS_PACKET pPacket;
#ifdef RT_BIG_ENDIAN
	TXD_STRUC *pDestTxD;
	UCHAR hw_hdr_info[TXD_SIZE];
#endif
	
	FreeNum = GET_CTRLRING_FREENO(pAd);	

	if (FreeNum == 0)
	{
		DBGPRINT(RT_DEBUG_WARN, ("%s FreeNum == 0 (TxCpuIdx = %d, TxDmaIdx = %d, TxSwFreeIdx = %d)\n", 
			__FUNCTION__, pAd->CtrlRing.TxCpuIdx, pAd->CtrlRing.TxDmaIdx, pAd->CtrlRing.TxSwFreeIdx));
		return NDIS_STATUS_FAILURE;
	}

	RTMP_IRQ_LOCK(&pAd->CtrlRingLock, IrqFlags);

	Status = RTMPAllocateNdisPacket(pAd, &pPacket, NULL, 0, Buf+TXINFO_SIZE, Len-TXINFO_SIZE);
	if (Status != NDIS_STATUS_SUCCESS)
	{
		DBGPRINT(RT_DEBUG_WARN, ("PCIKickOutCmd (error:: can't allocate NDIS PACKET)\n"));
		return NDIS_STATUS_FAILURE;
	}

	RTMP_QueryPacketInfo(pPacket, &PacketInfo, &pSrcBufVA, &SrcBufLen);
	if (pSrcBufVA == NULL)
	{
		RELEASE_NDIS_PACKET(pAd, pPacket, NDIS_STATUS_FAILURE);
		return NDIS_STATUS_FAILURE;
	}
	SwIdx = pAd->CtrlRing.TxCpuIdx;
	
#ifdef RT_BIG_ENDIAN
	pDestTxD  = (PTXD_STRUC)pAd->CtrlRing.Cell[SwIdx].AllocVa;
	NdisMoveMemory(&hw_hdr_info[0], pDestTxD, TXD_SIZE);
	pTxD = (TXD_STRUC *)&hw_hdr_info[0];
#else
	pTxD  = (PTXD_STRUC) pAd->CtrlRing.Cell[SwIdx].AllocVa;
#endif /* RT_BIG_ENDIAN */

	pTxInfo = (TXINFO_STRUC *)((UCHAR *)pTxD + sizeof(TXD_STRUC));
	NdisMoveMemory(pTxInfo, Buf, TXINFO_SIZE);

	//hex_dump("TxInfo", Buf, TXINFO_SIZE);
	//hex_dump("TxInfo", pTxInfo, TXINFO_SIZE);
#ifdef RT_BIG_ENDIAN
	RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
#endif /* RT_BIG_ENDIAN */
	//hex_dump("TxInfo", pTxInfo, TXINFO_SIZE);

	pAd->CtrlRing.Cell[SwIdx].pNdisPacket = pPacket;
	pAd->CtrlRing.Cell[SwIdx].pNextNdisPacket = NULL;

	SrcBufPA = PCI_MAP_SINGLE(pAd, (pSrcBufVA), (SrcBufLen), 0, RTMP_PCI_DMA_TODEVICE);

	pTxD->LastSec0 = 1;
	pTxD->LastSec1 = 0;
	pTxD->SDLen0 = SrcBufLen;
	pTxD->SDLen1 = 0;
	pTxD->SDPtr0 = SrcBufPA;
	pTxD->DMADONE = 0;


#ifdef RT_BIG_ENDIAN
	RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
	WriteBackToDescriptor((PUCHAR)pDestTxD, (PUCHAR)pTxD, FALSE, TYPE_TXD);
#endif

	/* flush dcache if no consistent memory is supported */
	RTMP_DCACHE_FLUSH(SrcBufPA, Len);
	RTMP_DCACHE_FLUSH(pAd->CtrlRing.Cell[SwIdx].AllocPa, TXD_SIZE);

   	/* Increase TX_CTX_IDX, but write to register later.*/
	INC_RING_INDEX(pAd->CtrlRing.TxCpuIdx, CTRL_RING_SIZE);

	RTMP_IO_WRITE32(pAd, TX_CTRL_CIDX,  pAd->CtrlRing.TxCpuIdx);

	//snowpin test
	if(0){
		UINT32 reg_val = 0;
		RTMP_IO_FORCE_READ32(pAd, TX_CTRL_CIDX, &reg_val);
		printk("sn %s - 0x%x = 0x%x\n", __FUNCTION__, TX_CTRL_CIDX, reg_val);
		RTMP_IO_FORCE_READ32(pAd, TX_CTRL_DIDX, &reg_val);
		printk("sn %s - 0x%x = 0x%x\n", __FUNCTION__, TX_CTRL_DIDX, reg_val);
	}
	//snowpin test

	RTMP_IRQ_UNLOCK(&pAd->CtrlRingLock, IrqFlags);

	return Status;
}

#endif /* RTMP_PCI_SUPPORT */




VOID MCUCtrlInit(PRTMP_ADAPTER pAd)
{
	struct MCU_CTRL *MCtrl = &pAd->MCUCtrl;

	RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_POLL_IDLE);
	NdisZeroMemory(MCtrl, sizeof(*MCtrl));
	MCtrl->CmdSeq = 0;
	RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_MCU_SEND_IN_BAND_CMD);
	NdisAllocateSpinLock(pAd, &MCtrl->CmdRspEventListLock);
	DlListInit(&MCtrl->CmdRspEventList);
}


VOID MCUCtrlExit(PRTMP_ADAPTER pAd)
{
	struct MCU_CTRL *MCtrl = &pAd->MCUCtrl;
	struct CMD_RSP_EVENT *CmdRspEvent, *CmdRspEventTmp;
	INT32 Ret;
	unsigned long IrqFlags;

	RtmpOsMsDelay(30);
	
	RTMP_IRQ_LOCK(&MCtrl->CmdRspEventListLock, IrqFlags);
	DlListForEachSafe(CmdRspEvent, CmdRspEventTmp, &MCtrl->CmdRspEventList, struct CMD_RSP_EVENT, List)
	{
		if (!CmdRspEvent->NeedWait)
		{
			DlListDel(&CmdRspEvent->List);
			os_free_mem(NULL, CmdRspEvent);
		}
	}
	RTMP_IRQ_UNLOCK(&MCtrl->CmdRspEventListLock, IrqFlags);

	NdisFreeSpinLock(&MCtrl->CmdRspEventListLock);
	NdisZeroMemory(MCtrl, sizeof(*MCtrl));
}


BOOLEAN IsInBandCmdProcessing(PRTMP_ADAPTER pAd)
{
	struct MCU_CTRL *MCtrl = &pAd->MCUCtrl;
	unsigned long IrqFlags;
	BOOLEAN Ret;

	RTMP_IRQ_LOCK(&MCtrl->CmdRspEventListLock, IrqFlags);
	
	if (DlListEmpty(&MCtrl->CmdRspEventList))
		Ret =  FALSE;
	else
		Ret = TRUE;

	RTMP_IRQ_UNLOCK(&MCtrl->CmdRspEventListLock, IrqFlags);

	return Ret;
}


UCHAR GetCmdRspNum(PRTMP_ADAPTER pAd)
{
	struct MCU_CTRL *MCtrl = &pAd->MCUCtrl;
	unsigned long IrqFlags;
	UCHAR Num = 0;
	Num = DlListLen(&MCtrl->CmdRspEventList);

	return Num;
}


static inline UCHAR GetCmdSeq(PRTMP_ADAPTER pAd)
{
	struct MCU_CTRL *MCtrl = &pAd->MCUCtrl;
	struct CMD_RSP_EVENT *CmdRspEvent, *CmdRspEventTmp;
	unsigned long IrqFlags;
	UCHAR TryCount = 0;

	RTMP_IRQ_LOCK(&MCtrl->CmdRspEventListLock, IrqFlags);
get_seq:
#ifdef HDR_TRANS_SUPPORT
	/*
		CMD_SEQ[19] is modified to be 1 by ASIC, if header translation is enabled.
		Due to this limitation, driver can only use CMD_SEQ[18:16] when header translation is enabled.
	*/
	MCtrl->CmdSeq == 0x7 ? MCtrl->CmdSeq = 1 : MCtrl->CmdSeq++;
#else /* HDR_TRANS_SUPPORT */
	MCtrl->CmdSeq == 0xf ? MCtrl->CmdSeq = 1 : MCtrl->CmdSeq++;
#endif /* !HDR_TRANS_SUPPORT */
	TryCount++;
	DlListForEachSafe(CmdRspEvent, CmdRspEventTmp, &MCtrl->CmdRspEventList, struct CMD_RSP_EVENT, List)
	{
		if (CmdRspEvent->CmdSeq == MCtrl->CmdSeq)
		{
			DBGPRINT(RT_DEBUG_ERROR, ("Command(seq: %d) is still running\n", MCtrl->CmdSeq));
			
			if (TryCount > 128)
			{
				break;
			}
			else
			{
				printk("CmdRspNum = %d\n", GetCmdRspNum(pAd));
				goto get_seq;
			}
		}
	}
	RTMP_IRQ_UNLOCK(&MCtrl->CmdRspEventListLock, IrqFlags);

	return MCtrl->CmdSeq;
}




INT AsicSendCmdToAndes(PRTMP_ADAPTER pAd, struct CMD_UNIT *CmdUnit)
{
	UINT32 VarLen;
	UCHAR *Pos, *Buf;
	TXINFO_NMAC_CMD *TxInfoCmd;
	INT32 Ret = NDIS_STATUS_SUCCESS;
	struct MCU_CTRL *MCtrl = &pAd->MCUCtrl;
	struct CMD_RSP_EVENT *CmdRspEvent;
	ULONG Expire;
	unsigned long IrqFlags;

	if ((!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_MCU_SEND_IN_BAND_CMD)) 
						&& (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_IDLE_RADIO_OFF)))
	{
		DBGPRINT(RT_DEBUG_ERROR, ("%s: !fRTMP_ADAPTER_MCU_SEND_IN_BAND_CMD && fRTMP_ADAPTER_IDLE_RADIO_OFF\n", __FUNCTION__));
		return NDIS_STATUS_FAILURE;
	}

	if (pAd->PM_FlgSuspend)
		return NDIS_STATUS_FAILURE;

	if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST | fRTMP_ADAPTER_IDLE_RADIO_OFF | fRTMP_ADAPTER_HALT_IN_PROGRESS))
		return NDIS_STATUS_FAILURE;

	VarLen = sizeof(*TxInfoCmd) + CmdUnit->u.ANDES.CmdPayloadLen;

	os_alloc_mem(pAd, (UCHAR **)&Buf, VarLen);
	if (Buf == NULL)
	{
		return NDIS_STATUS_RESOURCES;
	}
	
	NdisZeroMemory(Buf, VarLen);

	Pos = Buf;
	TxInfoCmd = (TXINFO_NMAC_CMD *)Pos;
	TxInfoCmd->info_type = CMD_PACKET;
	TxInfoCmd->d_port = CPU_TX_PORT;
	TxInfoCmd->cmd_type = CmdUnit->u.ANDES.Type;

	//CmdUnit->u.ANDES.NeedRsp = FALSE;
	//CmdUnit->u.ANDES.NeedWait = FALSE;

	if (CmdUnit->u.ANDES.NeedRsp)
	{
		TxInfoCmd->cmd_seq = GetCmdSeq(pAd);


		os_alloc_mem(NULL, (UCHAR **)&CmdRspEvent, sizeof(*CmdRspEvent));

		if (!CmdRspEvent)
		{
			DBGPRINT(RT_DEBUG_ERROR, ("%s Not available memory\n", __FUNCTION__));
			Ret = NDIS_STATUS_RESOURCES;
			goto error;
		}

		NdisZeroMemory(CmdRspEvent, sizeof(*CmdRspEvent));

		CmdRspEvent->CmdSeq = TxInfoCmd->cmd_seq;
		CmdRspEvent->Timeout = CmdUnit->u.ANDES.Timeout;
		CmdRspEvent->RspPayload = &CmdUnit->u.ANDES.RspPayload;
		CmdRspEvent->RspPayloadLen = &CmdUnit->u.ANDES.RspPayloadLen;

		if (CmdUnit->u.ANDES.NeedWait)
		{
			CmdRspEvent->NeedWait = TRUE;
#ifdef RTMP_PCI_SUPPORT
			CmdRspEvent->bAckDone = FALSE;
#endif /* RTMP_PCI_SUPPORT */
		}

		RTMP_IRQ_LOCK(&MCtrl->CmdRspEventListLock, IrqFlags);
		DlListAddTail(&MCtrl->CmdRspEventList, &CmdRspEvent->List);
		RTMP_IRQ_UNLOCK(&MCtrl->CmdRspEventListLock, IrqFlags);
	}
	else
	{	
		TxInfoCmd->cmd_seq = 0;
	}


	//printk("\x1b[31m%s: cmd_seq %d, len %d, type %d\x1b[m\n", __FUNCTION__, TxInfoCmd->cmd_seq, CmdUnit->u.ANDES.CmdPayloadLen, TxInfoCmd->cmd_type);
	//printk("\x1b[33m%s: need wait/rsp %d/%d\x1b[m\n", __FUNCTION__, CmdUnit->u.ANDES.NeedWait, CmdUnit->u.ANDES.NeedRsp);

	TxInfoCmd->pkt_len = CmdUnit->u.ANDES.CmdPayloadLen;

#ifdef RT_BIG_ENDIAN
	//printk("\x1b[31m%s: TXINFO_NMAC_CMD %d\x1b[m\n", __FUNCTION__,  sizeof(TXINFO_NMAC_CMD));
	//NdisMoveMemory(&hw_tx_info[0], TxInfoCmd, sizeof(TXINFO_NMAC_CMD));
	//RTMPDescriptorEndianChange(&hw_tx_info[0], TYPE_TXINFO);
	//NdisMoveMemory(TxInfoCmd, &hw_tx_info[0], sizeof(TXINFO_NMAC_CMD));
	//*(UINT32 *)TxInfoCmd = SWAP32(*(UINT32 *)TxInfoCmd);
	*(UINT32 *)TxInfoCmd = le2cpu32(*(UINT32 *)TxInfoCmd);
#endif

	Pos += sizeof(*TxInfoCmd);
	
	NdisMoveMemory(Pos, CmdUnit->u.ANDES.CmdPayload, CmdUnit->u.ANDES.CmdPayloadLen);
	

#ifdef RTMP_PCI_SUPPORT
	Ret = PCIKickOutCmd(pAd, Buf, VarLen);
	if ((Ret != NDIS_STATUS_SUCCESS) &&
		(CmdUnit->u.ANDES.NeedWait))
	{
		//printk("\x1b[33m%s: PCIKickOutCmd fail...\x1b[m\n", __FUNCTION__);
		RTMP_IRQ_LOCK(&MCtrl->CmdRspEventListLock, IrqFlags);
		DlListDel(&CmdRspEvent->List);
		os_free_mem(NULL, CmdRspEvent);
		RTMP_IRQ_UNLOCK(&MCtrl->CmdRspEventListLock, IrqFlags);
		goto error;
	}
#endif /* RTMP_PCI_SUPPORT */

	/* Wait for Command Rsp */
	if (CmdUnit->u.ANDES.NeedWait) {
		ULONG Timeout = CmdUnit->u.ANDES.Timeout;

#ifdef RTMP_PCI_SUPPORT
		Expire = Timeout ? Timeout : 100/*20*/;
		//printk("\x1b[33m%s: expire %d\x1b[m\n", __FUNCTION__, Expire);
		while ((--Expire) != 0)
		{
			if (CmdRspEvent->bAckDone == TRUE)
			{
				break;
			}
			OS_WAIT(1); /* 1ms */
		}
		
		//printk("\x1b[33m%s: expire %d\x1b[m\n", __FUNCTION__, Expire);
		if (Expire == 0)
			DBGPRINT(RT_DEBUG_ERROR, ("Wait for command response timeout(20ms)\n"));
#endif /* RTMP_PCI_SUPPORT */
	
		RTMP_IRQ_LOCK(&MCtrl->CmdRspEventListLock, IrqFlags);
		DlListDel(&CmdRspEvent->List);
		os_free_mem(NULL, CmdRspEvent);
		RTMP_IRQ_UNLOCK(&MCtrl->CmdRspEventListLock, IrqFlags);
	}

error:
	os_free_mem(NULL, Buf);

	return Ret;
}

static VOID CmdDoneHandler(PRTMP_ADAPTER pAd, UCHAR *Data)
{


}


static VOID CmdErrorHandler(PRTMP_ADAPTER pAd, UCHAR *Data)
{


}


static VOID CmdRetryHandler(PRTMP_ADAPTER pAd, UCHAR *Data)
{


}


static VOID PwrRspEventHandler(PRTMP_ADAPTER pAd, UCHAR *Data)
{


}


static VOID WowRspEventHandler(PRTMP_ADAPTER pAd, UCHAR *Data)
{


}


static VOID CarrierDetectRspEventHandler(PRTMP_ADAPTER pAd, UCHAR *Data)
{



}


static VOID DFSDetectRspEventHandler(PRTMP_ADAPTER pAd, UCHAR *Data)
{



}


CMD_RSP_HANDLER CmdRspHandlerTable[] =
{
	CmdDoneHandler,
	CmdErrorHandler,
	CmdRetryHandler,
	PwrRspEventHandler,
	WowRspEventHandler,
	CarrierDetectRspEventHandler,
	DFSDetectRspEventHandler,
};


INT AndesBurstWrite(PRTMP_ADAPTER pAd, UINT32 Offset, UINT32 *Data, UINT32 Cnt)
{
	struct CMD_UNIT CmdUnit;
	CHAR *Pos, *Buf, *CurHeader;
	UINT32 VarLen, OffsetNum, CurLen = 0, SentLen;
	UINT32 Value, i, CurIndex = 0;
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;
	INT32 Ret;
	BOOLEAN LastPacket = FALSE;

	OffsetNum = Cnt / ((pChipCap->InbandPacketMaxLen - sizeof(Offset)) / 4);

	if (Cnt % ((pChipCap->InbandPacketMaxLen - sizeof(Offset)) / 4))
		VarLen = sizeof(Offset) * (OffsetNum + 1) + 4 * Cnt;
	else
		VarLen = sizeof(Offset) * OffsetNum + 4 * Cnt;

	os_alloc_mem(pAd, (UCHAR **)&Buf, VarLen);
	if (Buf == NULL)
	{
		return NDIS_STATUS_RESOURCES;
	}

	Pos = Buf;
	
	while (CurLen < VarLen)
	{
		SentLen = (VarLen - CurLen) > pChipCap->InbandPacketMaxLen 
									? pChipCap->InbandPacketMaxLen : (VarLen - CurLen);
		
		if ((SentLen < pChipCap->InbandPacketMaxLen) || (CurLen + pChipCap->InbandPacketMaxLen) == VarLen)
			LastPacket = TRUE;
	
		CurHeader = Pos;

		Value = cpu2le32(Offset + pChipCap->WlanMemmapOffset + CurIndex * 4);
		NdisMoveMemory(Pos, &Value, 4);
		Pos += 4;

		for (i = 0; i < ((SentLen - 4) / 4); i++)
		{
			Value = cpu2le32(Data[i + CurIndex]);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 4;
		};

		NdisZeroMemory(&CmdUnit, sizeof(CmdUnit));
	
		CmdUnit.u.ANDES.Type = CMD_BURST_WRITE;
		CmdUnit.u.ANDES.CmdPayloadLen = SentLen;
		CmdUnit.u.ANDES.CmdPayload = CurHeader;

		if (LastPacket && (Cnt > 1))
		{
			CmdUnit.u.ANDES.NeedRsp = TRUE;
			CmdUnit.u.ANDES.NeedWait = TRUE;
			CmdUnit.u.ANDES.Timeout = 0;
		}

		Ret = AsicSendCmdToAndes(pAd, &CmdUnit);

		if (Ret != NDIS_STATUS_SUCCESS)
			goto error;
		
		CurIndex += ((SentLen - 4) / 4);
		CurLen += pChipCap->InbandPacketMaxLen;
	}

error:
	os_free_mem(NULL, Buf);

	return NDIS_STATUS_SUCCESS;
}


INT AndesBurstRead(PRTMP_ADAPTER pAd, UINT32 Offset, UINT32 Cnt, UINT32 *Data)
{
	struct CMD_UNIT CmdUnit;
	UINT32 CurLen = 0, CmdLen, RspLen, OffsetNum, ReceiveLen;
	CHAR *Pos, *Pos1, *CmdBuf, *RspBuf, *CurCmdHeader, *CurRspHeader;
	UINT32 i, Value, Status = NDIS_STATUS_SUCCESS, CurIndex = 0;
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;
	INT32 Ret;

	OffsetNum = Cnt / ((pChipCap->InbandPacketMaxLen - sizeof(Offset)) / 4);

	if (Cnt % ((pChipCap->InbandPacketMaxLen - sizeof(Offset)) / 4))
	{
		CmdLen = 8 * (OffsetNum + 1); 
		RspLen = sizeof(Offset) * (OffsetNum + 1) + 4 * Cnt;
	}
	else
	{
		CmdLen = 8 * OffsetNum;
		RspLen = sizeof(Offset) * OffsetNum + 4 * Cnt;
	}

	os_alloc_mem(pAd, (UCHAR **)&CmdBuf, CmdLen);

	if (CmdBuf == NULL)
	{
		return NDIS_STATUS_RESOURCES;
	}

	os_alloc_mem(pAd, (UCHAR **)&RspBuf, RspLen);
	
	if (RspBuf == NULL)
	{
		os_free_mem(NULL, CmdBuf);
		return NDIS_STATUS_RESOURCES;
	}
	
	Pos = CmdBuf;
	Pos1 = RspBuf;

	while (CurLen < RspLen)
	{
		ReceiveLen = (RspLen - CurLen) > pChipCap->InbandPacketMaxLen 
									   ? pChipCap->InbandPacketMaxLen 
									   : (RspLen - CurLen);

		CurCmdHeader = Pos;
		CurRspHeader = Pos1;
		
		Value = cpu2le32(Offset + pChipCap->WlanMemmapOffset + CurIndex * 4);
		NdisMoveMemory(Pos, &Value, 4);
		Pos += 4;

		Value = cpu2le32((ReceiveLen - 4) / 4);
		NdisMoveMemory(Pos, &Value, 4);
		Pos += 4;

		NdisZeroMemory(&CmdUnit, sizeof(CmdUnit));
		CmdUnit.u.ANDES.Type = CMD_BURST_READ;
		CmdUnit.u.ANDES.CmdPayloadLen = 8;
		CmdUnit.u.ANDES.CmdPayload = CurCmdHeader;
		CmdUnit.u.ANDES.RspPayload = CurRspHeader;
		CmdUnit.u.ANDES.RspPayloadLen = ReceiveLen;
		CmdUnit.u.ANDES.NeedRsp = TRUE;
		CmdUnit.u.ANDES.NeedWait = TRUE;
		CmdUnit.u.ANDES.Timeout = 0;

		Ret = AsicSendCmdToAndes(pAd, &CmdUnit);

		if (Ret != NDIS_STATUS_SUCCESS)
			goto error;

		if (CmdUnit.u.ANDES.RspPayloadLen == ReceiveLen)
		{
			NdisMoveMemory(&Data[CurIndex], CmdUnit.u.ANDES.RspPayload + 4, CmdUnit.u.ANDES.RspPayloadLen - 4);
			Pos1 += ReceiveLen;

			for (i = 0; i < (ReceiveLen - 4) / 4; i++)
			{
				Data[i + CurIndex] = le2cpu32(Data[i + CurIndex]);
			}
		}
		else
		{
			DBGPRINT(RT_DEBUG_ERROR, ("%s: Rsp len(%d) != Expect len (%d)\n", 
				__FUNCTION__, CmdUnit.u.ANDES.RspPayloadLen, ReceiveLen));

			Status = NDIS_STATUS_FAILURE;

			goto error;
		}
		
		CurIndex += ((ReceiveLen - 4) / 4);
		CurLen += pChipCap->InbandPacketMaxLen;
	}

error:

	os_free_mem(NULL, CmdBuf);
	os_free_mem(NULL, RspBuf);

	return Status;
}


INT AndesRandomRead(PRTMP_ADAPTER pAd, RTMP_REG_PAIR *RegPair, UINT32 Num)
{
	struct CMD_UNIT CmdUnit;
	UINT32 VarLen = Num * 8, CurLen = 0, ReceiveLen;
	CHAR *Pos, *Pos1, *CmdBuf, *RspBuf, *CurCmdHeader, *CurRspHeader;
	UINT32 i, Value, Status = NDIS_STATUS_SUCCESS, CurIndex = 0;
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;
	INT32 Ret;

	os_alloc_mem(pAd, (UCHAR **)&CmdBuf, VarLen);

	if (CmdBuf == NULL)
	{
		return NDIS_STATUS_RESOURCES;
	}

	os_alloc_mem(pAd, (UCHAR **)&RspBuf, VarLen);

	if (RspBuf == NULL)
	{
		os_free_mem(NULL, CmdBuf);
		return NDIS_STATUS_RESOURCES;
	}

	NdisZeroMemory(CmdBuf, VarLen);

	Pos = CmdBuf;
	Pos1 = RspBuf;

	while (CurLen < VarLen)
	{
		ReceiveLen = (VarLen - CurLen) > pChipCap->InbandPacketMaxLen 
									   ? pChipCap->InbandPacketMaxLen 
									   : (VarLen - CurLen);

		CurCmdHeader = Pos;
		CurRspHeader = Pos1;

		for (i = 0; i < ReceiveLen / 8; i++)
		{
			Value = cpu2le32(RegPair[i + CurIndex].Register + pChipCap->WlanMemmapOffset);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 8;
		}
	
		NdisZeroMemory(&CmdUnit, sizeof(CmdUnit));
		CmdUnit.u.ANDES.Type = CMD_RANDOM_READ;
		CmdUnit.u.ANDES.CmdPayloadLen = ReceiveLen;
		CmdUnit.u.ANDES.CmdPayload = CurCmdHeader;
		CmdUnit.u.ANDES.RspPayload = CurRspHeader;
		CmdUnit.u.ANDES.RspPayloadLen = ReceiveLen;
		CmdUnit.u.ANDES.NeedRsp = TRUE;
		CmdUnit.u.ANDES.NeedWait = TRUE;
		CmdUnit.u.ANDES.Timeout = 0;

		Ret = AsicSendCmdToAndes(pAd, &CmdUnit);
		
		if (Ret != NDIS_STATUS_SUCCESS)
			goto error;
	
		if (CmdUnit.u.ANDES.RspPayloadLen == ReceiveLen)
		{
			for (i = 0; i < ReceiveLen / 8; i++)
			{
				NdisMoveMemory(&RegPair[i + CurIndex].Value, CmdUnit.u.ANDES.RspPayload + 8 * i + 4, 4);
				RegPair[i + CurIndex].Value = le2cpu32(RegPair[i + CurIndex].Value);
			}
		}
		else
		{
			DBGPRINT(RT_DEBUG_ERROR, ("%s: Rsp len(%d) != Expect len (%d)\n", 
				__FUNCTION__, CmdUnit.u.ANDES.RspPayloadLen, ReceiveLen));

			Status = NDIS_STATUS_FAILURE;

			goto error;
		}
		
		CurIndex += ReceiveLen / 8;
		CurLen += pChipCap->InbandPacketMaxLen;
	}

error:	
	os_free_mem(NULL, CmdBuf);
	os_free_mem(NULL, RspBuf);

	return Status;
}


INT AndesRFRandomRead(PRTMP_ADAPTER pAd, BANK_RF_REG_PAIR *RegPair, UINT32 Num)
{
	struct CMD_UNIT CmdUnit;
	UINT32 VarLen = Num * 8, CurLen = 0, ReceiveLen;
	CHAR *Pos, *Pos1, *CmdBuf, *RspBuf, *CurCmdHeader, *CurRspHeader;
	UINT32 i, Value, Status = NDIS_STATUS_SUCCESS, CurIndex = 0;
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;
	INT32 Ret;

	os_alloc_mem(pAd, (UCHAR **)&CmdBuf, VarLen);

	if (CmdBuf == NULL)
	{
		return NDIS_STATUS_RESOURCES;
	}

	os_alloc_mem(pAd, (UCHAR **)&RspBuf, VarLen);

	if (RspBuf == NULL)
	{
		os_free_mem(NULL, CmdBuf);
		return NDIS_STATUS_RESOURCES;
	}

	NdisZeroMemory(CmdBuf, VarLen);
	Pos = CmdBuf;
	Pos1 = RspBuf;

	while (CurLen < VarLen)
	{
		ReceiveLen = (VarLen - CurLen) > pChipCap->InbandPacketMaxLen 
									   ? pChipCap->InbandPacketMaxLen 
									   : (VarLen - CurLen);

		CurCmdHeader = Pos;
		CurRspHeader = Pos1;

		for (i = 0; i < ReceiveLen / 8; i++)
		{
			Value = 0;
	
			/* RF selection */
			Value = (Value & ~0x80000000) | 0x80000000;

			/* RF bank */
			Value = (Value & ~0x00ff0000) | (RegPair[i + CurIndex].Bank << 16);

			/* RF Index */
			Value = (Value & ~0x0000ffff) | RegPair[i + CurIndex].Register;

			Value = cpu2le32(Value);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 8;
		}
	
		NdisZeroMemory(&CmdUnit, sizeof(CmdUnit));
		CmdUnit.u.ANDES.Type = CMD_RANDOM_READ;
		CmdUnit.u.ANDES.CmdPayloadLen = ReceiveLen;
		CmdUnit.u.ANDES.CmdPayload = CurCmdHeader;
		CmdUnit.u.ANDES.RspPayload = CurRspHeader;
		CmdUnit.u.ANDES.RspPayloadLen = ReceiveLen;
		CmdUnit.u.ANDES.NeedRsp = TRUE;
		CmdUnit.u.ANDES.NeedWait = TRUE;
		CmdUnit.u.ANDES.Timeout = 0;

		Ret = AsicSendCmdToAndes(pAd, &CmdUnit);

		if (Ret != NDIS_STATUS_SUCCESS)
			goto error;
	
	
		if (CmdUnit.u.ANDES.RspPayloadLen == ReceiveLen)
		{
			for (i = 0; i < ReceiveLen / 8; i++)
			{
				NdisMoveMemory(&RegPair[i + CurIndex].Value, CmdUnit.u.ANDES.RspPayload + 8 * i + 4, 1);
			}
		}
		else
		{
			DBGPRINT(RT_DEBUG_ERROR, ("%s: Rsp len(%d) != Expect len (%d)\n", 
				__FUNCTION__, CmdUnit.u.ANDES.RspPayloadLen, ReceiveLen));

			Status = NDIS_STATUS_FAILURE;

			goto error;
		}
		
		CurIndex += ReceiveLen / 8;
		CurLen += pChipCap->InbandPacketMaxLen;
	}
	
error:
	os_free_mem(NULL, CmdBuf);
	os_free_mem(NULL, RspBuf);

	return Status;
}


INT AndesReadModifyWrite(PRTMP_ADAPTER pAd, R_M_W_REG *RegPair, UINT32 Num)
{
	struct CMD_UNIT CmdUnit;
	CHAR *Pos, *Buf, *CurHeader;
	UINT32 VarLen = Num * 12, CurLen = 0, SentLen;
	UINT32 Value, i, CurIndex = 0;
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;
	INT32 Ret;
	BOOLEAN LastPacket = FALSE;
	
	os_alloc_mem(pAd, (UCHAR **)&Buf, VarLen);

	if (Buf == NULL)
	{
		return NDIS_STATUS_RESOURCES;
	}

	Pos = Buf;

	while (CurLen < VarLen)
	{
		SentLen = (VarLen - CurLen) > pChipCap->InbandPacketMaxLen 
									? pChipCap->InbandPacketMaxLen : (VarLen - CurLen);
		
		if ((SentLen < pChipCap->InbandPacketMaxLen) || (CurLen + pChipCap->InbandPacketMaxLen) == VarLen)
			LastPacket = TRUE;

		CurHeader = Pos;

		for (i = 0; i < (SentLen / 12); i++)
		{
			/* Address */
			Value = cpu2le32(RegPair[i + CurIndex].Register + pChipCap->WlanMemmapOffset);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 4;

			/* ClearBitMask */
			Value = cpu2le32(RegPair[i + CurIndex].ClearBitMask);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 4;

			/* UpdateData */
			Value = cpu2le32(RegPair[i + CurIndex].Value);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 4;
		}

		NdisZeroMemory(&CmdUnit, sizeof(CmdUnit));
	
		CmdUnit.u.ANDES.Type = CMD_READ_MODIFY_WRITE;
		CmdUnit.u.ANDES.CmdPayloadLen = SentLen;
		CmdUnit.u.ANDES.CmdPayload = CurHeader;

		if (LastPacket)
		{
			CmdUnit.u.ANDES.NeedRsp = TRUE;
			CmdUnit.u.ANDES.NeedWait = TRUE;
			CmdUnit.u.ANDES.Timeout = 0;
		}

		Ret = AsicSendCmdToAndes(pAd, &CmdUnit);
		
		if (Ret != NDIS_STATUS_SUCCESS)
			goto error;
	
		CurIndex += (SentLen / 12);
		CurLen += pChipCap->InbandPacketMaxLen;
	}

error:
	os_free_mem(NULL, Buf);

	return NDIS_STATUS_SUCCESS;
}


INT AndesRFReadModifyWrite(PRTMP_ADAPTER pAd, RF_R_M_W_REG *RegPair, UINT32 Num)
{
	struct CMD_UNIT CmdUnit;
	CHAR *Pos, *Buf, *CurHeader;
	UINT32 VarLen = Num * 12, CurLen = 0, SentLen;
	UINT32 Value, i, CurIndex = 0;
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;
	INT32 Ret;
	BOOLEAN LastPacket = FALSE;
	
	os_alloc_mem(pAd, (UCHAR **)&Buf, VarLen);

	if (Buf == NULL)
	{
		return NDIS_STATUS_RESOURCES;
	}

	Pos = Buf;

	while (CurLen < VarLen)
	{
		SentLen = (VarLen - CurLen) > pChipCap->InbandPacketMaxLen 
									? pChipCap->InbandPacketMaxLen : (VarLen - CurLen);
		
		if ((SentLen < pChipCap->InbandPacketMaxLen) || (CurLen + pChipCap->InbandPacketMaxLen) == VarLen)
			LastPacket = TRUE;
		
		CurHeader = Pos;
		
		for (i = 0; i < SentLen / 12; i++)
		{
			Value = 0;
			/* RF selection */
			Value = (Value & ~0x80000000) | 0x80000000;

			/* RF bank */
			Value = (Value & ~0x00ff0000) | (RegPair[i + CurIndex].Bank << 16);

			/* RF Index */
			Value = (Value & ~0x000000ff) | RegPair[i + CurIndex].Register;

			Value = cpu2le32(Value);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 4;

			Value = 0;
			/* ClearBitMask */
			Value = (Value & ~0x000000ff) | RegPair[i + CurIndex].ClearBitMask;
		
			Value = cpu2le32(Value);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 4;

			Value = 0;
			/* UpdateData */
			Value = (Value & ~0x000000ff) | RegPair[i + CurIndex].Value;

			Value = cpu2le32(Value);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 4;
		}
	
		NdisZeroMemory(&CmdUnit, sizeof(CmdUnit));
	
		CmdUnit.u.ANDES.Type = CMD_READ_MODIFY_WRITE;
		CmdUnit.u.ANDES.CmdPayloadLen = SentLen;
		CmdUnit.u.ANDES.CmdPayload = CurHeader;

		if (LastPacket)
		{
			CmdUnit.u.ANDES.NeedRsp = TRUE;
			CmdUnit.u.ANDES.NeedWait = TRUE;
			CmdUnit.u.ANDES.Timeout = 0;
		}

		Ret = AsicSendCmdToAndes(pAd, &CmdUnit);
		
		if (Ret != NDIS_STATUS_SUCCESS)
			goto error;
		
		CurIndex += (SentLen / 12);
		CurLen += pChipCap->InbandPacketMaxLen;
	}

error:
	os_free_mem(NULL, Buf);

	return NDIS_STATUS_SUCCESS;
}


INT AndesRandomWritePair(PRTMP_ADAPTER pAd, RTMP_REG_PAIR *RegPair, UINT32 Num)
{
	struct CMD_UNIT CmdUnit;
	CHAR *Pos, *Buf, *CurHeader;
	UINT32 VarLen = Num * 8, CurLen = 0, SentLen;
	UINT32 Value, i, CurIndex = 0;
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;
	INT32 Ret;
	BOOLEAN LastPacket = FALSE;

	os_alloc_mem(pAd, (UCHAR **)&Buf, VarLen);
	
	if (Buf == NULL)
	{
		return NDIS_STATUS_RESOURCES;
	}

	Pos = Buf;

	while (CurLen < VarLen)
	{
		SentLen = (VarLen - CurLen) > pChipCap->InbandPacketMaxLen 
									? pChipCap->InbandPacketMaxLen : (VarLen - CurLen);

		if ((SentLen < pChipCap->InbandPacketMaxLen) || (CurLen + pChipCap->InbandPacketMaxLen) == VarLen)
			LastPacket = TRUE;

		CurHeader = Pos;
		
		for (i = 0; i < (SentLen / 8); i++)
		{
			/* Address */
			Value = cpu2le32(RegPair[i + CurIndex].Register + pChipCap->WlanMemmapOffset);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 4;

			/* UpdateData */
			Value = cpu2le32(RegPair[i + CurIndex].Value);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 4;
		};

		NdisZeroMemory(&CmdUnit, sizeof(CmdUnit));
	
		CmdUnit.u.ANDES.Type = CMD_RANDOM_WRITE;
		CmdUnit.u.ANDES.CmdPayloadLen = SentLen;
		CmdUnit.u.ANDES.CmdPayload = CurHeader;

		if (LastPacket)
		{
			CmdUnit.u.ANDES.NeedRsp = TRUE;
			CmdUnit.u.ANDES.NeedWait = TRUE;
			CmdUnit.u.ANDES.Timeout = 0;
		}

		Ret = AsicSendCmdToAndes(pAd, &CmdUnit);
	
		if (Ret != NDIS_STATUS_SUCCESS)
			goto error;
	
		CurIndex += (SentLen / 8);
		CurLen += pChipCap->InbandPacketMaxLen;
	}

error:
	os_free_mem(NULL, Buf);

	return NDIS_STATUS_SUCCESS;
}


INT AndesRandomWrite(PRTMP_ADAPTER pAd, UINT32 Num, ...)
{
	struct CMD_UNIT CmdUnit;
	CHAR *Pos, *Buf, *CurHeader;
	UINT32 VarLen = Num * 8, CurLen = 0, SentLen;
	UINT32 Value, i, CurIndex = 0;
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;
	INT32 Ret;
	va_list argptr;
	BOOLEAN LastPacket = FALSE;

	va_start(argptr, Num);

	os_alloc_mem(pAd, (UCHAR **)&Buf, VarLen);
	
	if (Buf == NULL)
	{
		return NDIS_STATUS_RESOURCES;
	}

	Pos = Buf;

	while (CurLen < VarLen)
	{
		SentLen = (VarLen - CurLen) > pChipCap->InbandPacketMaxLen 
									? pChipCap->InbandPacketMaxLen : (VarLen - CurLen);

		if ((SentLen < pChipCap->InbandPacketMaxLen) || (CurLen + pChipCap->InbandPacketMaxLen) == VarLen)
			LastPacket = TRUE;

		CurHeader = Pos;
		
		for (i = 0; i < (SentLen / 8); i++)
		{
			/* Address */
			Value = cpu2le32( va_arg(argptr, UINT32) +pChipCap->WlanMemmapOffset);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 4;

			/* UpdateData */
			Value = cpu2le32(va_arg(argptr, UINT32));
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 4;
		};

		NdisZeroMemory(&CmdUnit, sizeof(CmdUnit));
	
		CmdUnit.u.ANDES.Type = CMD_RANDOM_WRITE;
		CmdUnit.u.ANDES.CmdPayloadLen = SentLen;
		CmdUnit.u.ANDES.CmdPayload = CurHeader;

		if (LastPacket)
		{
			CmdUnit.u.ANDES.NeedRsp = TRUE;
			CmdUnit.u.ANDES.NeedWait = TRUE;
			CmdUnit.u.ANDES.Timeout = 0;
		}

		Ret = AsicSendCmdToAndes(pAd, &CmdUnit);
	
		if (Ret != NDIS_STATUS_SUCCESS)
			goto error;
	
		CurIndex += (SentLen / 8);
		CurLen += pChipCap->InbandPacketMaxLen;
	}

error:
	os_free_mem(NULL, Buf);
	va_end(argptr);

	return NDIS_STATUS_SUCCESS;
}


INT AndesRFRandomWritePair(PRTMP_ADAPTER pAd, BANK_RF_REG_PAIR *RegPair, UINT32 Num)
{
	struct CMD_UNIT CmdUnit;
	CHAR *Pos, *Buf, *CurHeader;
	UINT32 VarLen = Num * 8, CurLen = 0, SentLen;
	UINT32 Value, i, CurIndex = 0;
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;
	INT32 Ret;
	BOOLEAN LastPacket = FALSE;

	os_alloc_mem(pAd, (UCHAR **)&Buf, VarLen);
	
	if (Buf == NULL)
	{
		return NDIS_STATUS_RESOURCES;
	}

	Pos = Buf;

	while (CurLen < VarLen)
	{
		SentLen = (VarLen - CurLen) > pChipCap->InbandPacketMaxLen 
									? pChipCap->InbandPacketMaxLen : (VarLen - CurLen);
		
		if ((SentLen < pChipCap->InbandPacketMaxLen) || (CurLen + pChipCap->InbandPacketMaxLen) == VarLen)
			LastPacket = TRUE;

		CurHeader = Pos;
		
		for (i = 0; i < (SentLen / 8); i++)
		{
			Value = 0;
			/* RF selection */
			Value = (Value & ~0x80000000) | 0x80000000;

			/* RF bank */
			Value = (Value & ~0x00ff0000) | (RegPair[i + CurIndex].Bank << 16);

			/* RF Index */
			Value = (Value & ~0x000000ff) | RegPair[i + CurIndex].Register;
			
			//printk("Value = %x RF Bank = %d and Index = %d\n", Value, RegPair[i + CurIndex].Bank, RegPair[i + CurIndex].Register);

			Value = cpu2le32(Value);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 4;

			Value = 0;
			/* UpdateData */
			Value = (Value & ~0x000000ff) | RegPair[i + CurIndex].Value;

			Value = cpu2le32(Value);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 4;
		}

		NdisZeroMemory(&CmdUnit, sizeof(CmdUnit));
	
		CmdUnit.u.ANDES.Type = CMD_RANDOM_WRITE;
		CmdUnit.u.ANDES.CmdPayloadLen = SentLen;
		CmdUnit.u.ANDES.CmdPayload = CurHeader;

		if (LastPacket)
		{
			CmdUnit.u.ANDES.NeedRsp = TRUE;
			CmdUnit.u.ANDES.NeedWait = TRUE;
			CmdUnit.u.ANDES.Timeout = 0;
		}

		Ret = AsicSendCmdToAndes(pAd, &CmdUnit);
		
		if (Ret != NDIS_STATUS_SUCCESS)
			goto error;
		
		CurIndex += (SentLen / 8);
		CurLen += pChipCap->InbandPacketMaxLen;
	}


error:
	os_free_mem(NULL, Buf);

	return NDIS_STATUS_SUCCESS;
}


INT AndesRFRandomWrite(PRTMP_ADAPTER pAd, UINT32 Num, ...)
{
	struct CMD_UNIT CmdUnit;
	CHAR *Pos, *Buf, *CurHeader;
	UINT32 VarLen = Num * 8, CurLen = 0, SentLen;
	UINT32 Value, i, CurIndex = 0;
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;
	INT32 Ret;
	va_list argptr;
	BOOLEAN LastPacket = FALSE;

	va_start(argptr, Num);

	os_alloc_mem(pAd, (UCHAR **)&Buf, VarLen);
	
	if (Buf == NULL)
	{
		return NDIS_STATUS_RESOURCES;
	}
	
	Pos = Buf;

	while (CurLen < VarLen)
	{
		SentLen = (VarLen - CurLen) > pChipCap->InbandPacketMaxLen 
									? pChipCap->InbandPacketMaxLen : (VarLen - CurLen);

		if ((SentLen < pChipCap->InbandPacketMaxLen) || (CurLen + pChipCap->InbandPacketMaxLen) == VarLen)
			LastPacket = TRUE;

		CurHeader = Pos;
		
		for (i = 0; i < (SentLen / 8); i++)
		{
			Value = 0;
			/* RF selection */
			Value = (Value & ~0x80000000) | 0x80000000;

			/* RF bank */
			Value = (Value & ~0x00ff0000) | (va_arg(argptr, UINT) << 16);

			/* RF Index */
			Value = (Value & ~0x000000ff) | va_arg(argptr, UINT);
			
			//printk("Value = %x RF Bank = %d and Index = %d\n", Value, RegPair[i + CurIndex].Bank, RegPair[i + CurIndex].Register);

			Value = cpu2le32(Value);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 4;

			Value = 0;
			/* UpdateData */
			Value = (Value & ~0x000000ff) | va_arg(argptr, UINT);

			Value = cpu2le32(Value);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 4;
		}

		NdisZeroMemory(&CmdUnit, sizeof(CmdUnit));
	
		CmdUnit.u.ANDES.Type = CMD_RANDOM_WRITE;
		CmdUnit.u.ANDES.CmdPayloadLen = SentLen;
		CmdUnit.u.ANDES.CmdPayload = CurHeader;

		if (LastPacket)
		{
			CmdUnit.u.ANDES.NeedRsp = TRUE;
			CmdUnit.u.ANDES.NeedWait = TRUE;
			CmdUnit.u.ANDES.Timeout = 0;
		}

		Ret = AsicSendCmdToAndes(pAd, &CmdUnit);
		
		if (Ret != NDIS_STATUS_SUCCESS)
			goto error;
		
		CurIndex += (SentLen / 8);
		CurLen += pChipCap->InbandPacketMaxLen;
	}


error:
	os_free_mem(NULL, Buf);
	va_end(argptr);

	return NDIS_STATUS_SUCCESS;
}


#ifdef MT7601
INT AndesBBPRandomWritePair(PRTMP_ADAPTER pAd, RTMP_REG_PAIR *RegPair, UINT32 Num)
{
	struct CMD_UNIT CmdUnit;
	CHAR *Pos, *Buf, *CurHeader;
	UINT32 VarLen = Num * 8, CurLen = 0, SentLen;
	UINT32 Value, i, CurIndex = 0;
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;
	INT32 Ret;
	BOOLEAN LastPacket = FALSE;

	os_alloc_mem(pAd, (UCHAR **)&Buf, VarLen);
	
	if (Buf == NULL)
	{
		return NDIS_STATUS_RESOURCES;
	}

	Pos = Buf;

	while (CurLen < VarLen)
	{
		SentLen = (VarLen - CurLen) > pChipCap->InbandPacketMaxLen 
									? pChipCap->InbandPacketMaxLen : (VarLen - CurLen);

		if ((SentLen < pChipCap->InbandPacketMaxLen) || (CurLen + pChipCap->InbandPacketMaxLen) == VarLen)
			LastPacket = TRUE;
		
		CurHeader = Pos;
		
		for (i = 0; i < (SentLen / 8); i++)
		{
			Value = 0;
			/* BBP selection */
			Value = (Value & ~0x40000000) | 0x40000000;

			/* BBP Index */
			Value = (Value & ~0x000000ff) | RegPair[i + CurIndex].Register;
			
			Value = cpu2le32(Value);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 4;

			Value = 0;
			/* UpdateData */
			Value = (Value & ~0x000000ff) | RegPair[i + CurIndex].Value;

			Value = cpu2le32(Value);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 4;
		}

		NdisZeroMemory(&CmdUnit, sizeof(CmdUnit));
	
		CmdUnit.u.ANDES.Type = CMD_RANDOM_WRITE;
		CmdUnit.u.ANDES.CmdPayloadLen = SentLen;
		CmdUnit.u.ANDES.CmdPayload = CurHeader;

		if (LastPacket)
		{
			CmdUnit.u.ANDES.NeedRsp = TRUE;
			CmdUnit.u.ANDES.NeedWait = TRUE;
			CmdUnit.u.ANDES.Timeout = 0;
		}

		Ret = AsicSendCmdToAndes(pAd, &CmdUnit);
		
		if (Ret != NDIS_STATUS_SUCCESS)
			goto error;

		CurIndex += (SentLen / 8);
		CurLen += pChipCap->InbandPacketMaxLen;
	}

error:
	os_free_mem(NULL, Buf);

	return NDIS_STATUS_SUCCESS;
}


INT AndesBBPRandomWrite(PRTMP_ADAPTER pAd, UINT32 Num, ...)
{
	struct CMD_UNIT CmdUnit;
	CHAR *Pos, *Buf, *CurHeader;
	UINT32 VarLen = Num * 8, CurLen = 0, SentLen;
	UINT32 Value, i, CurIndex = 0;
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;
	INT32 Ret;
	va_list argptr;
	BOOLEAN LastPacket = FALSE;

	va_start(argptr, Num);

	os_alloc_mem(pAd, (UCHAR **)&Buf, VarLen);
	
	if (Buf == NULL)
	{
		return NDIS_STATUS_RESOURCES;
	}

	Pos = Buf;

	while (CurLen < VarLen)
	{
		SentLen = (VarLen - CurLen) > pChipCap->InbandPacketMaxLen 
									? pChipCap->InbandPacketMaxLen : (VarLen - CurLen);

		if ((SentLen < pChipCap->InbandPacketMaxLen) || (CurLen + pChipCap->InbandPacketMaxLen) == VarLen)
			LastPacket = TRUE;
		
		CurHeader = Pos;
		
		for (i = 0; i < (SentLen / 8); i++)
		{
			Value = 0;
			/* BBP selection */
			Value = (Value & ~0x40000000) | 0x40000000;

			/* BBP Index */
			Value = (Value & ~0x000000ff) | va_arg(argptr, UINT);
			
			Value = cpu2le32(Value);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 4;

			Value = 0;
			/* UpdateData */
			Value = (Value & ~0x000000ff) | va_arg(argptr, UINT);

			Value = cpu2le32(Value);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 4;
		}

		NdisZeroMemory(&CmdUnit, sizeof(CmdUnit));
	
		CmdUnit.u.ANDES.Type = CMD_RANDOM_WRITE;
		CmdUnit.u.ANDES.CmdPayloadLen = SentLen;
		CmdUnit.u.ANDES.CmdPayload = CurHeader;

		if (LastPacket)
		{
			CmdUnit.u.ANDES.NeedRsp = TRUE;
			CmdUnit.u.ANDES.NeedWait = TRUE;
			CmdUnit.u.ANDES.Timeout = 0;
		}

		Ret = AsicSendCmdToAndes(pAd, &CmdUnit);
		
		if (Ret != NDIS_STATUS_SUCCESS)
			goto error;

		CurIndex += (SentLen / 8);
		CurLen += pChipCap->InbandPacketMaxLen;
	}

error:
	os_free_mem(NULL, Buf);
	va_end(argptr);

	return NDIS_STATUS_SUCCESS;
}

#endif


INT AndesPwrSavingOP(PRTMP_ADAPTER pAd, UINT32 PwrOP, UINT32 PwrLevel, 
					UINT32 ListenInterval, UINT32 PreTBTTLeadTime,
					UINT8 TIMByteOffset, UINT8 TIMBytePattern)
{
	struct CMD_UNIT CmdUnit;
	CHAR *Pos, *Buf;
	UINT32 VarLen;
	UINT32 Value;
	INT32 Ret;

	/* Power operation and Power Level */
	VarLen = 8;

	if (PwrOP == RADIO_OFF_ADVANCE)
	{
		/* Listen interval, Pre-TBTT, TIM info */
		VarLen += 12;
	}

	os_alloc_mem(pAd, (UCHAR **)&Buf, VarLen);

	if (Buf == NULL)
	{
		return NDIS_STATUS_RESOURCES;
	}

	Pos = Buf;

	/* Power operation */
	Value = cpu2le32(PwrOP);
	NdisMoveMemory(Pos, &Value, 4);
	Pos += 4;

	/* Power Level */
	Value = cpu2le32(PwrLevel);
	NdisMoveMemory(Pos, &Value, 4);
	Pos += 4;

	if ( (PwrOP == RADIO_OFF_ADVANCE) || (PwrOP == RADIO_OFF_AUTO_WAKEUP))
	{
		/* Listen interval */
		Value = cpu2le32(ListenInterval);
		NdisMoveMemory(Pos, &Value, 4);
		Pos += 4;


		/* Pre TBTT lead time */
		Value = cpu2le32(PreTBTTLeadTime);
		NdisMoveMemory(Pos, &Value, 4);
		Pos += 4;
	}

	if (PwrOP == RADIO_OFF_ADVANCE)
	{
		/* TIM Info */
		Value = (Value & ~0x000000ff) | TIMBytePattern;
		Value = (Value & ~0x0000ff00) | (TIMByteOffset << 8);
		Value = cpu2le32(Value);
		NdisMoveMemory(Pos, &Value, 4);
		Pos += 4;
	}

	NdisZeroMemory(&CmdUnit, sizeof(CmdUnit));
	
	CmdUnit.u.ANDES.Type = CMD_POWER_SAVING_OP;
	CmdUnit.u.ANDES.CmdPayloadLen = VarLen;
	CmdUnit.u.ANDES.CmdPayload = Buf;

	CmdUnit.u.ANDES.NeedRsp = FALSE;
	CmdUnit.u.ANDES.NeedWait = FALSE;
		
	CmdUnit.u.ANDES.Timeout = 0;

	Ret = AsicSendCmdToAndes(pAd, &CmdUnit);

	os_free_mem(NULL, Buf);

	return NDIS_STATUS_SUCCESS;
}


INT AndesFunSetOP(PRTMP_ADAPTER pAd, UINT32 FunID, UINT32 Param)
{
	struct CMD_UNIT CmdUnit;
	CHAR *Pos, *Buf;
	UINT32 VarLen;
	UINT32 Value;
	INT32 Ret;

	/* Function ID and Parameter */
	VarLen = 8;

	os_alloc_mem(pAd, (UCHAR **)&Buf, VarLen);

	if (Buf == NULL)
	{
		return NDIS_STATUS_RESOURCES;
	}

	Pos = Buf;
	
	/* Function ID */
	Value = cpu2le32(FunID);
	NdisMoveMemory(Pos, &Value, 4);
	Pos += 4;

	/* Parameter */
	Value = cpu2le32(Param);
	NdisMoveMemory(Pos, &Value, 4);
	Pos += 4;
	
	NdisZeroMemory(&CmdUnit, sizeof(CmdUnit));
	
	CmdUnit.u.ANDES.Type = CMD_FUN_SET_OP;
	CmdUnit.u.ANDES.CmdPayloadLen = VarLen;
	CmdUnit.u.ANDES.CmdPayload = Buf;

	if ( FunID == 5 )
	{
		CmdUnit.u.ANDES.NeedRsp = TRUE;
		CmdUnit.u.ANDES.NeedWait = TRUE;
		CmdUnit.u.ANDES.Timeout = 0;
	}

	Ret = AsicSendCmdToAndes(pAd, &CmdUnit);

	os_free_mem(NULL, Buf);

	return NDIS_STATUS_SUCCESS;
}


INT AndesCalibrationOP(PRTMP_ADAPTER pAd, UINT32 CalibrationID, UINT32 Param)
{

	struct CMD_UNIT CmdUnit;
	CHAR *Pos, *Buf;
	UINT32 VarLen;
	UINT32 Value;
	INT32 Ret;

	//printk("\x1b[34m%s: CalibrationID %d\x1b[m\n", __FUNCTION__, CalibrationID);
	/* Calibration ID and Parameter */
	VarLen = 8;

	os_alloc_mem(pAd, (UCHAR **)&Buf, VarLen);

	if (Buf == NULL)
	{
		return NDIS_STATUS_RESOURCES;
	}

	Pos = Buf;
	
	/* Calibration ID */
	Value = cpu2le32(CalibrationID);
	NdisMoveMemory(Pos, &Value, 4);
	Pos += 4;

	/* Parameter */
	Value = cpu2le32(Param);
	NdisMoveMemory(Pos, &Value, 4);
	Pos += 4;
	
	NdisZeroMemory(&CmdUnit, sizeof(CmdUnit));
	
	CmdUnit.u.ANDES.Type = CMD_CALIBRATION_OP;
	CmdUnit.u.ANDES.CmdPayloadLen = VarLen;
	CmdUnit.u.ANDES.CmdPayload = Buf;
	
	CmdUnit.u.ANDES.NeedRsp = TRUE;
	CmdUnit.u.ANDES.NeedWait = TRUE;
	CmdUnit.u.ANDES.Timeout = 0;

	Ret = AsicSendCmdToAndes(pAd, &CmdUnit);

	os_free_mem(NULL, Buf);

	return NDIS_STATUS_SUCCESS;
}

INT AndesLedOP(
	IN PRTMP_ADAPTER pAd,
	IN UCHAR LedIdx,
	IN UCHAR LinkStatus)
{
	struct CMD_UNIT CmdUnit;
	CHAR *Pos, *pBuf;
	UINT32 VarLen;
	UINT32 arg0=0, arg1=0;
	INT32 Ret;
	UINT32 Value;

	/* Calibration ID and Parameter */
	VarLen = 8;
	arg0 = LedIdx;
	arg1 = LinkStatus;
	os_alloc_mem(pAd, (UCHAR **)&pBuf, VarLen);
	if (pBuf == NULL)
	{
		return NDIS_STATUS_RESOURCES;
	}

	NdisZeroMemory(pBuf, VarLen);
	
	Pos = pBuf;

	/* LedIdx */
	Value = cpu2le32(arg0);
	NdisMoveMemory(Pos, &Value, 4);
	Pos += 4;
	
	/* LinkStatus */
	Value = cpu2le32(arg1);	
	NdisMoveMemory(Pos, &Value, 4);
	Pos += 4;

	hex_dump("AndesLedOP: ", pBuf, VarLen);
	NdisZeroMemory(&CmdUnit, sizeof(CmdUnit));
	
	CmdUnit.u.ANDES.Type = CMD_LED_MODE_OP;
	CmdUnit.u.ANDES.CmdPayloadLen = VarLen;
	CmdUnit.u.ANDES.CmdPayload = pBuf;
	
	CmdUnit.u.ANDES.NeedRsp = TRUE;
	CmdUnit.u.ANDES.NeedWait = TRUE;
	CmdUnit.u.ANDES.Timeout = 0;

	Ret = AsicSendCmdToAndes(pAd, &CmdUnit);

	os_free_mem(NULL, pBuf);

	return NDIS_STATUS_SUCCESS;
}