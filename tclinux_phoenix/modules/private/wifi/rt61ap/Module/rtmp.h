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
    rtmp.h

    Abstract:
    Miniport generic portion header file

    Revision History:
    Who         When          What
    --------    ----------    ----------------------------------------------
    Paul Lin    2002-08-01    created
	James Tan   2002-09-06    modified (Revise NTCRegTable)
    John Chang  2004-09-06    modified for RT2600     
*/
#ifndef __RTMP_H__
#define __RTMP_H__

#include "mlme.h"
#include "oid.h"
#ifdef APCLI_SUPPORT
#include "sta_mlme.h"
#endif

#ifdef IGMP_SNOOP_SUPPORT
#include "rtmp_igmpSnoop.h"
#endif // IGMP_SNOOP_SUPPORT //

#ifdef BLOCK_NET_IF
#include "netif_block.h"
#endif // BLOCK_NET_IF //


#ifdef WSC_SUPPORT
#include "wsc.h"
#endif // WSC_SUPPORT //

//
//  Queue structure and macros
//
typedef struct  _QUEUE_ENTRY    {
    struct _QUEUE_ENTRY     *Next;
}   QUEUE_ENTRY, *PQUEUE_ENTRY;

// Queue structure
typedef struct  _QUEUE_HEADER   {
    PQUEUE_ENTRY    Head;
    PQUEUE_ENTRY    Tail;
    ULONG            Number;
}   QUEUE_HEADER, *PQUEUE_HEADER;

#define InitializeQueueHeader(QueueHeader)              \
{                                                       \
    (QueueHeader)->Head = (QueueHeader)->Tail = NULL;   \
    (QueueHeader)->Number = 0;                          \
}

#define RemoveHeadQueue(QueueHeader)                \
(QueueHeader)->Head;                                \
{                                                   \
    PQUEUE_ENTRY pNext;                             \
    if ((QueueHeader)->Head != NULL)				\
    {												\
        pNext = (QueueHeader)->Head->Next;              \
        (QueueHeader)->Head = pNext;                    \
        if (pNext == NULL)                              \
            (QueueHeader)->Tail = NULL;                 \
        (QueueHeader)->Number--;                        \
    }						                        \
}

#define InsertHeadQueue(QueueHeader, QueueEntry)            \
{                                                           \
    ((PQUEUE_ENTRY)QueueEntry)->Next = (QueueHeader)->Head; \
    (QueueHeader)->Head = (PQUEUE_ENTRY)(QueueEntry);       \
    if ((QueueHeader)->Tail == NULL)						\
        (QueueHeader)->Tail = (PQUEUE_ENTRY)(QueueEntry);	\
	(QueueHeader)->Number++;                                \
}

#define InsertTailQueue(QueueHeader, QueueEntry)                \
{                                                               \
    ((PQUEUE_ENTRY)QueueEntry)->Next = NULL;                    \
    if ((QueueHeader)->Tail)                                    \
        (QueueHeader)->Tail->Next = (PQUEUE_ENTRY)(QueueEntry); \
    else                                                        \
        (QueueHeader)->Head = (PQUEUE_ENTRY)(QueueEntry);       \
    (QueueHeader)->Tail = (PQUEUE_ENTRY)(QueueEntry);           \
    (QueueHeader)->Number++;                                    \
}

//
//  MACRO for debugging information
//
#ifdef DBG
extern ULONG     RTDebugLevel;

#define DBGPRINT(Level, fmt, args...)                   \
{                                                       \
    if (Level <= RTDebugLevel)                          \
    {                                                   \
        printk(NIC_DBG_STRING);                         \
        printk(KERN_DEBUG fmt, ## args);                \
    }                                                   \
}

#define DBGPRINT_ERR(fmt, args...)  \
{                                   \
    printk("ERROR!!! ");			\
    printk(KERN_DEBUG fmt, ## args);\
}
#else
#define DBGPRINT(Level, fmt, args...)
#define DBGPRINT_ERR(fmt, args...)
#endif

//
//  spin_lock enhanced for Nested spin lock
//

#define NdisAllocateSpinLock(lock)      \
{                                       \
    spin_lock_init(lock);               \
}

#define NdisFreeSpinLock(lock)          \
{                                       \
}

#define RTMPFreeSkbBuffer(skb)          \
{                                       \
    dev_kfree_skb_any(skb);             \
}

//  Assert MACRO to make sure program running
//
#undef  ASSERT
#define ASSERT(x)                                                               \
{                                                                               \
    if (!(x))                                                                   \
    {                                                                           \
        printk(KERN_WARNING __FILE__ ":%d assert " #x "failed\n", __LINE__);    \
    }                                                                           \
}

//
//  Macros for flag and ref count operations
//
#define RTMP_SET_FLAG(_M, _F)       ((_M)->Flags |= (_F))
#define RTMP_CLEAR_FLAG(_M, _F)     ((_M)->Flags &= ~(_F))
#define RTMP_CLEAR_FLAGS(_M)        ((_M)->Flags = 0)
#define RTMP_TEST_FLAG(_M, _F)      (((_M)->Flags & (_F)) != 0)
#define RTMP_TEST_FLAGS(_M, _F)     (((_M)->Flags & (_F)) == (_F))

#define OPSTATUS_SET_FLAG(_pAd, _F)     ((_pAd)->PortCfg.OpStatusFlags |= (_F))
#define OPSTATUS_CLEAR_FLAG(_pAd, _F)   ((_pAd)->PortCfg.OpStatusFlags &= ~(_F))
#define OPSTATUS_TEST_FLAG(_pAd, _F)    (((_pAd)->PortCfg.OpStatusFlags & (_F)) != 0)

#define CLIENT_STATUS_SET_FLAG(_pEntry,_F)      ((_pEntry)->ClientStatusFlags |= (_F))
#define CLIENT_STATUS_CLEAR_FLAG(_pEntry,_F)    ((_pEntry)->ClientStatusFlags &= ~(_F))
#define CLIENT_STATUS_TEST_FLAG(_pEntry,_F)     (((_pEntry)->ClientStatusFlags & (_F)) != 0)

#define INC_RING_INDEX(_idx, _RingSize)    \
{                                          \
    (_idx)++;                              \
    if ((_idx) >= (_RingSize)) _idx=0;     \
}

// Increase TxTsc value for next transmission
// TODO: 
// When i==6, means TSC has done one full cycle, do re-keying stuff follow specs
// Should send a special event microsoft defined to request re-key
#define INC_TX_TSC(_tsc)                                \
{                                                       \
    INT i=0;                                            \
	while (++_tsc[i] == 0x0)                            \
    {                                                   \
        i++;                                            \
		if (i == 6)                                     \
			break;                                      \
	}                                                   \
}

#define NdisEqualMemory(Source1, Source2, Length)   RTMPEqualMemory(Source1, Source2, Length)

//
// MACRO for 32-bit PCI register read / write
#if 0
#ifdef RTMP_EMBEDDED
#define RTMP_IO_READ32(_A, _R, _pV)     (*_pV = PCIMemRead32(__mem_pci((_A)->CSRBaseAddress + (_R))))
#define RTMP_IO_WRITE32(_A, _R, _V)     (PCIMemWrite32(__mem_pci((_A)->CSRBaseAddress + (_R)), _V))
#define RTMP_IO_WRITE8(_A, _R, _V)		(PCIMemWrite8((PUCHAR)__mem_pci((_A)->CSRBaseAddress + (_R)), (_V)))
#define RTMP_IO_WRITE16(_A, _R, _V)		(PCIMemWrite16((PUSHORT)__mem_pci((_A)->CSRBaseAddress + (_R)), (_V)))

#else
#define RTMP_IO_READ32(_A, _R, _pV)		(*_pV = readl((void *)((_A)->CSRBaseAddress + (_R))))
#define RTMP_IO_WRITE32(_A, _R, _V)		(writel(_V, (void *)((_A)->CSRBaseAddress + (_R))))
#define RTMP_IO_WRITE8(_A, _R, _V)		(writeb((_V), (PUCHAR)((_A)->CSRBaseAddress + (_R))))
#define RTMP_IO_WRITE16(_A, _R, _V)		(writew((_V), (PUSHORT)((_A)->CSRBaseAddress + (_R))))
#endif
#else
//Patch for ASIC turst read/write bug, needs to remove after metel fix
#define RTMP_IO_READ32(_A, _R, _pV)								\
{																\
	(*_pV = readl((void *)((_A)->CSRBaseAddress + MAC_CSR0)));			\
	(*_pV = readl((void *)((_A)->CSRBaseAddress + (_R))));					\
}
#define RTMP_IO_READ8(_A, _R, _pV)								\
{																\
	(*_pV = readl((void *)((_A)->CSRBaseAddress + MAC_CSR0)));			\
	(*_pV = readb((void *)((_A)->CSRBaseAddress + (_R))));				\
}
#define RTMP_IO_WRITE32(_A, _R, _V)												\
{																				\
	UMAXLONG 	Val;																\
	Val = readl((void *)((_A)->CSRBaseAddress + MAC_CSR0));			\
	writel(_V, (void *)((_A)->CSRBaseAddress + (_R)));								\
}
#define RTMP_IO_WRITE8(_A, _R, _V)												\
{																				\
	UMAXLONG 	Val;																\
	Val = readl((void *)((_A)->CSRBaseAddress + MAC_CSR0));			\
	writeb((_V), (PUCHAR)((_A)->CSRBaseAddress + (_R)));		\
}
#define RTMP_IO_WRITE16(_A, _R, _V)												\
{																				\
	UMAXLONG 	Val;																\
	Val = readl((void *)((_A)->CSRBaseAddress + MAC_CSR0));			\
	writew((_V), (PUSHORT)((_A)->CSRBaseAddress + (_R)));	\
}
#endif

//
// BBP & RF are using indirect access. Before write any value into it.
// We have to make sure there is no outstanding command pending via checking busy bit.
//
#define	MAX_BUSY_COUNT	100			// Number of retry before failing access BBP & RF indirect register
//
#define	RTMP_RF_IO_WRITE32(_A, _V)				    \
{												    \
	PHY_CSR4_STRUC	Value;						    \
	ULONG 			BusyCnt = 0;				    \
	do {										    \
		RTMP_IO_READ32(_A, PHY_CSR4, &Value.word);  \
		if (Value.field.Busy == IDLE)		        \
			break;								    \
		BusyCnt++;								    \
	}	while (BusyCnt < MAX_BUSY_COUNT);		    \
	if (BusyCnt < MAX_BUSY_COUNT)				    \
	{											    \
		RTMP_IO_WRITE32(_A, PHY_CSR4, _V);			\
	}											    \
}

// Read BBP register by register's ID
#define	RTMP_BBP_IO_READ8_BY_REG_ID(_A, _I, _pV)	    \
{												        \
	PHY_CSR3_STRUC	BbpCsr;                             \
	INT             i, k;                               \
	for (i=0; i<MAX_BUSY_COUNT; i++)                    \
	{                                                   \
        RTMP_IO_READ32(_A, PHY_CSR3, &BbpCsr.word);     \
        if (BbpCsr.field.Busy == BUSY)                  \
        {                                               \
            continue;                                   \
        }                                               \
    	BbpCsr.word = 0;							    \
    	BbpCsr.field.fRead = 1;		                    \
    	BbpCsr.field.Busy = 1;					        \
    	BbpCsr.field.RegNum = _I;				        \
    	RTMP_IO_WRITE32(_A, PHY_CSR3, BbpCsr.word);     \
    	for (k=0; k<MAX_BUSY_COUNT; k++)                \
    	{                                               \
            RTMP_IO_READ32(_A, PHY_CSR3, &BbpCsr.word); \
            if (BbpCsr.field.Busy == IDLE)              \
                break;                                  \
    	}                                               \
    	if ((BbpCsr.field.Busy == IDLE) &&              \
            (BbpCsr.field.RegNum == _I))                \
    	{                                               \
            *(_pV) = (UCHAR)BbpCsr.field.Value;         \
        	break;                                      \
    	}                                               \
	}                                                   \
	if (BbpCsr.field.Busy == BUSY)                      \
	{                                                   \
        DBGPRINT_ERR("BBP read fail\n");                \
        *(_pV) = (_A)->BbpWriteLatch[_I];               \
	}                                                   \
}

// Write BBP register by register's ID & value
#define	RTMP_BBP_IO_WRITE8_BY_REG_ID(_A, _I, _V)	    \
{												        \
	PHY_CSR3_STRUC	BbpCsr;						        \
	INT             BusyCnt;                            \
    for (BusyCnt=0; BusyCnt<MAX_BUSY_COUNT; BusyCnt++)  \
    {                                                   \
        RTMP_IO_READ32(_A, PHY_CSR3, &BbpCsr.word);     \
        if (BbpCsr.field.Busy == BUSY)                  \
            continue;                                   \
        BbpCsr.word = 0;                                \
        BbpCsr.field.fRead = 0;                         \
        BbpCsr.field.Busy = 1;                          \
        BbpCsr.field.Value = _V;                        \
        BbpCsr.field.RegNum = _I;                       \
        RTMP_IO_WRITE32(_A, PHY_CSR3, BbpCsr.word);     \
        (_A)->BbpWriteLatch[_I] = _V;                   \
        break;                                          \
    }                                                   \
}

// When write BBP r17, write the value to share memory at the same time
// Firmware will this value for radar detection
#define RTMP_WRITE_BBP_R17(_A, _V)	    				\
{														\
	ULONG 	BBPReg = 0x49112c10;						\
														\
	RTMP_BBP_IO_WRITE8_BY_REG_ID(_A, BBP_R17, _V);		\
														\
	BBPReg = (BBPReg & 0x00FFFFFF) | (_V << 24);		\
	RTMP_IO_WRITE32(_A, 0x29bc, BBPReg);				\
														\
	DBGPRINT(RT_DEBUG_INFO, "RTMP_WRITE_BBP_R17(BBPReg=%x)\n", BBPReg);	\
														\
}

#define NIC_MAX_PHYS_BUF_COUNT              8

typedef struct _RTMP_SCATTER_GATHER_ELEMENT {
    PVOID		Address;
    ULONG		Length;
    PULONG		Reserved;
} RTMP_SCATTER_GATHER_ELEMENT, *PRTMP_SCATTER_GATHER_ELEMENT;

typedef struct _RTMP_SCATTER_GATHER_LIST {
    ULONG  NumberOfElements;
    PULONG Reserved;
    RTMP_SCATTER_GATHER_ELEMENT Elements[NIC_MAX_PHYS_BUF_COUNT];
} RTMP_SCATTER_GATHER_LIST, *PRTMP_SCATTER_GATHER_LIST;

//
//  Some utility macros
//
#ifndef min
#define min(_a, _b)     (((_a) < (_b)) ? (_a) : (_b))
#endif

#ifndef max
#define max(_a, _b)     (((_a) > (_b)) ? (_a) : (_b))
#endif

#define INC_COUNTER(Val)    (Val.QuadPart++)
#define	INC_COUNTER64(Val)	        (Val.QuadPart++)

#define MIN_NET_DEVICE_FOR_AID			0x00		//0x00~0x3f
#define MIN_NET_DEVICE_FOR_MBSSID		0x00		//0x00,0x10,0x20,0x30
#define MIN_NET_DEVICE_FOR_WDS			0x10		//0x40,0x50,0x60,0x70
#define MIN_NET_DEVICE_FOR_APCLI		0x20

#ifdef RTL865X_FAST_PATH
/* reserve cb: 0~3, 28~47 */
#define CB_OFF	28
#else
#ifdef TC_SOC
#define CB_OFF	28
#else
#define CB_OFF  0
#endif
#endif

#define RTMP_SET_PACKET_FRAGMENTS(_p, number)       ((_p)->cb[10+CB_OFF] = number)
#define RTMP_GET_PACKET_FRAGMENTS(_p)               ((_p)->cb[10+CB_OFF])
#define RTMP_SET_PACKET_RTS(_p, number)             ((_p)->cb[11+CB_OFF] = number)
#define RTMP_GET_PACKET_RTS(_p)                     ((_p)->cb[11+CB_OFF])
#define RTMP_SET_PACKET_SOURCE(_p, pktsrc)          ((_p)->cb[12+CB_OFF] = pktsrc)
#define RTMP_GET_PACKET_SOURCE(_p)                  ((_p)->cb[12+CB_OFF])
#define RTMP_SET_PACKET_TXRATE(_p, rate)            ((_p)->cb[13+CB_OFF] = rate)
#define RTMP_GET_PACKET_TXRATE(_p)                  ((_p)->cb[13+CB_OFF])
#define RTMP_SET_PACKET_UP(_p, _prio)               ((_p)->cb[14+CB_OFF] = _prio)
#define RTMP_GET_PACKET_UP(_p)                      ((_p)->cb[14+CB_OFF])
#define RTMP_SET_PACKET_NET_DEVICE_MBSSID(_p, idx)  ((_p)->cb[15+CB_OFF] = idx)
#define RTMP_SET_PACKET_NET_DEVICE_WDS(_p, idx)     ((_p)->cb[15+CB_OFF] = (idx + MIN_NET_DEVICE_FOR_WDS))
#define RTMP_SET_PACKET_NET_DEVICE_APCLI(_p, idx)   ((_p)->cb[15+CB_OFF] = (idx + MIN_NET_DEVICE_FOR_APCLI))
#define RTMP_GET_PACKET_NET_DEVICE(_p)              ((_p)->cb[15+CB_OFF])
#define RTMP_SET_PACKET_AID(_p, idx)				((_p)->cb[16+CB_OFF] = idx)
#define RTMP_GET_PACKET_AID(_p)						((_p)->cb[16+CB_OFF])

#define RTMP_SET_PACKET_MOREDATA(_p, _morebit)		((_p)->cb[17+CB_OFF] = _morebit)
#define RTMP_GET_PACKET_MOREDATA(_p)				((_p)->cb[17+CB_OFF])

//iversonadd
#define RTMP_SET_PACKET_ENCRYPT(_p, _morebit)           ((_p)->cb[18+CB_OFF] = _morebit)
#define RTMP_GET_PACKET_ENCRYPT(_p)                             ((_p)->cb[18+CB_OFF])

#define PKTSRC_NDIS             0x7f
#define PKTSRC_DRIVER           0x0f
#define PKTSRC_MCAST            0xf7 // a unigue ID for MCAST packet.

#define	MAKE_802_3_HEADER(_p, _pMac1, _pMac2, _pType)					\
{																		\
	NdisMoveMemory(_p, _pMac1, MAC_ADDR_LEN);							\
	NdisMoveMemory((_p + MAC_ADDR_LEN), _pMac2, MAC_ADDR_LEN);	        \
	NdisMoveMemory((_p + MAC_ADDR_LEN * 2), _pType, LENGTH_802_3_TYPE);	\
}

// if pData has no LLC/SNAP (neither RFC1042 nor Bridge tunnel), keep it that way.
// else if the received frame is LLC/SNAP-encaped IPX or APPLETALK, preserve the LLC/SNAP field 
// else remove the LLC/SNAP field from the result Ethernet frame
// Patch for WHQL only, which did not turn on Netbios but use IPX within its payload
// Note:
//     _pData & _DataSize may be altered (remove 8-byte LLC/SNAP) by this MACRO
//     _pRemovedLLCSNAP: pointer to removed LLC/SNAP; NULL is not removed
#define CONVERT_TO_802_3(_p8023hdr, _pDA, _pSA, _pData, _DataSize, _pRemovedLLCSNAP)      \
{                                                                       \
    char LLC_Len[2];                                                    \
                                                                        \
    _pRemovedLLCSNAP = NULL;                                            \
	if (NdisEqualMemory(SNAP_802_1H, _pData, 6)  ||                     \
	    NdisEqualMemory(SNAP_BRIDGE_TUNNEL, _pData, 6))                 \
	{                                                                   \
	    PUCHAR pProto = _pData + 6;                                     \
					                                                    \
		if ((NdisEqualMemory(IPX, pProto, 2) || NdisEqualMemory(APPLE_TALK, pProto, 2)) &&  \
		    NdisEqualMemory(SNAP_802_1H, _pData, 6))                    \
		{                                                               \
			LLC_Len[0] = (UCHAR)(_DataSize / 256);                      \
			LLC_Len[1] = (UCHAR)(_DataSize % 256);                      \
			MAKE_802_3_HEADER(_p8023hdr, _pDA, _pSA, LLC_Len);          \
		}                                                               \
		else                                                            \
		{                                                               \
			MAKE_802_3_HEADER(_p8023hdr, _pDA, _pSA, pProto);           \
			_pRemovedLLCSNAP = _pData;                                  \
			_DataSize -= LENGTH_802_1_H;                                \
			_pData += LENGTH_802_1_H;                                   \
		}                                                               \
	}                                                                   \
	else                                                                \
	{                                                                   \
		LLC_Len[0] = (UCHAR)(_DataSize / 256);                          \
		LLC_Len[1] = (UCHAR)(_DataSize % 256);                          \
		MAKE_802_3_HEADER(_p8023hdr, _pDA, _pSA, LLC_Len);              \
	}                                                                   \
}

#define MAC_ADDR_EQUAL(pAddr1,pAddr2)           RTMPEqualMemory((PVOID)(pAddr1), (PVOID)(pAddr2), MAC_ADDR_LEN)
#define COPY_MAC_ADDR(Addr1, Addr2)             memcpy((Addr1), (Addr2), MAC_ADDR_LEN)
#define SSID_EQUAL(ssid1, len1, ssid2, len2)    ((len1==len2) && (RTMPEqualMemory(ssid1, ssid2, len1)))

#define RELEASE_NDIS_PACKET(_pAd, _pSkb)                             	\
{                                                                       \
    if (RTMP_GET_PACKET_SOURCE(_pSkb) == PKTSRC_NDIS)                	\
    {                                                                   \
        RTMPFreeSkbBuffer(_pSkb);                                    	\
        _pAd->RalinkCounters.PendingNdisPacketCount --;                 \
    }                                                                   \
    else                                                                \
        RTMPFreeSkbBuffer(_pSkb);                                    	\
}

//
// Register set pair for initialzation register set definition
//
typedef struct  _RTMP_REG_PAIR
{
    ULONG    Register;
    ULONG    Value;
}   RTMP_REG_PAIR, *PRTMP_REG_PAIR;

typedef	struct	_BBP_REG_PAIR
{
	UCHAR	Register;
	UCHAR	Value;
}	BBP_REG_PAIR, *PBBP_REG_PAIR;

//
// Register set pair for initialzation register set definition
//
typedef struct  _RTMP_RF_REGS
{
    UCHAR   Channel;
    ULONG   R1;
    ULONG   R2;
    ULONG   R3;
    ULONG   R4;
}   RTMP_RF_REGS, *PRTMP_RF_REGS;

//
//  Data buffer for DMA operation, the buffer must be contiguous physical memory
//  Both DMA to / from CPU use the same structure.
//
typedef struct  _RTMP_DMABUF
{
    ULONG                   AllocSize;
    PVOID                   AllocVa;            // TxBuf virtual address
    dma_addr_t              AllocPa;            // TxBuf physical address
    struct sk_buff			*pSkb;
}   RTMP_DMABUF, *PRTMP_DMABUF;

//
// Control block (Descriptor) for all ring descriptor DMA operation, buffer must be 
// contiguous physical memory. NDIS_PACKET stored the binding Rx packet descriptor
// which won't be released, driver has to wait until upper layer return the packet
// before giveing up this rx ring descriptor to ASIC. NDIS_BUFFER is assocaited pair
// to describe the packet buffer. For Tx, NDIS_PACKET stored the tx packet descriptor
// which driver should ACK upper layer when the tx is physically done or failed.
//
typedef struct _RTMP_DMACB
{
    ULONG                   AllocSize;          // Control block size
    PVOID                   AllocVa;            // Control block virtual address
    dma_addr_t              AllocPa;            // Control block physical address
    RTMP_DMABUF             DmaBuf;             // Associated DMA buffer structure

/* move pSkb & pNextSkb from TXD_STRUC is for 64-bit PC use, CONFIG_64BIT */
#ifdef CONFIG_64BIT
	struct sk_buff          *pSkb;
	struct sk_buff          *pNextSkb;
#endif // CONFIG_64BIT //
}   RTMP_DMACB, *PRTMP_DMACB;

typedef struct _RTMP_TX_BUF
{
    PQUEUE_ENTRY    Next;
    UCHAR           Index;
    ULONG           AllocSize;          // Control block size
    PVOID           AllocVa;            // Control block virtual address
    dma_addr_t      AllocPa;            // Control block physical address
}   RTMP_TXBUF, *PRTMP_TXBUF;

typedef struct _RTMP_TX_RING
{
    RTMP_DMACB  Cell[TX_RING_SIZE];
    UINT        CurTxIndex;
    UINT        NextTxDmaDoneIndex;
}   RTMP_TX_RING, *PRTMP_TX_RING;

typedef struct _RTMP_RX_RING
{
    RTMP_DMACB  Cell[RX_RING_SIZE];
    UINT        CurRxIndex;
}   RTMP_RX_RING, *PRTMP_RX_RING;

typedef struct _RTMP_MGMT_RING
{
    RTMP_DMACB  Cell[MGMT_RING_SIZE];
    UINT        CurTxIndex;
    UINT        NextTxDmaDoneIndex;
} RTMP_MGMT_RING, *PRTMP_MGMT_RING;

//
//  Statistic counter structure
//
typedef struct _COUNTER_802_3
{
    // General Stats
	UMAXLONG		GoodTransmits;
	UMAXLONG		GoodReceives;
	UMAXLONG		TxErrors;
	UMAXLONG		RxErrors;
	UMAXLONG		RxNoBuffer;

    // Ethernet Stats
	UMAXLONG		RcvAlignmentErrors;
	UMAXLONG		OneCollision;
	UMAXLONG		MoreCollisions;

}   COUNTER_802_3, *PCOUNTER_802_3;

typedef struct _COUNTER_802_11 {
    UMAXLONG    Length;
    UMAXLONG	TransmittedFragmentCount;
    UMAXLONG	MulticastTransmittedFrameCount;
    UMAXLONG	FailedCount;
    UMAXLONG	RetryCount;
    UMAXLONG	MultipleRetryCount;
    UMAXLONG	RTSSuccessCount;
    UMAXLONG	RTSFailureCount;
    UMAXLONG	ACKFailureCount;
    UMAXLONG	FrameDuplicateCount;
    UMAXLONG	ReceivedFragmentCount;
    UMAXLONG	MulticastReceivedFrameCount;
    UMAXLONG	FCSErrorCount;
} COUNTER_802_11, *PCOUNTER_802_11;

typedef struct _COUNTER_RALINK {
    UMAXLONG           TransmittedByteCount;   // both successful and failure, used to calculate TX throughput
    UMAXLONG           ReceivedByteCount;      // both CRC okay and CRC error, used to calculate RX throughput
    UMAXLONG           BeenDisassociatedCount;
    UMAXLONG           BadCQIAutoRecoveryCount;
    UMAXLONG           PoorCQIRoamingCount;
    UMAXLONG           MgmtRingFullCount;
    UMAXLONG           RxCount;
    UMAXLONG           RxRingErrCount;
    UMAXLONG           KickTxCount;
    UMAXLONG           TxRingErrCount;
	UMAXLONG		   RealFcsErrCount;
    UMAXLONG           PendingNdisPacketCount;

    UMAXLONG           OneSecOsTxCount[NUM_OF_TX_RING];
    UMAXLONG           OneSecDmaDoneCount[NUM_OF_TX_RING];
    UMAXLONG           OneSecTxDoneCount;
    UMAXLONG           OneSecTxAggregationCount;
    UMAXLONG           OneSecRxAggregationCount;

    UMAXLONG           OneSecTxNoRetryOkCount;
    UMAXLONG           OneSecTxRetryOkCount;
    UMAXLONG           OneSecTxFailCount;
	UMAXLONG		    OneSecFalseCCACnt;	// CCA error count, for debug purpose, might move to global counter
    UMAXLONG           OneSecRxOkCnt;       // RX without error
	UMAXLONG           OneSecRxFcsErrCnt;   // CRC error
	UMAXLONG           OneSecBeaconSentCnt;
} COUNTER_RALINK, *PCOUNTER_RALINK;

typedef struct _COUNTER_DRS {
    // to record the each TX rate's quality. 0 is best, the bigger the worse.
    USHORT          TxQuality[MAX_LEN_OF_SUPPORTED_RATES];
    UCHAR           PER[MAX_LEN_OF_SUPPORTED_RATES];
    UCHAR           TxRateUpPenalty;      // extra # of second penalty due to last unstable condition
    ULONG           CurrTxRateStableTime; // # of second in current TX rate
    BOOLEAN         fNoisyEnvironment;
    UCHAR           LastSecTxRateChangeAction; // 0: no change, 1:rate UP, 2:rate down
} COUNTER_DRS, *PCOUNTER_DRS;

//
//  Arcfour Structure Added by PaulWu
//
typedef struct _ARCFOUR
{
    UINT            X;
    UINT            Y;
    UCHAR           STATE[256];
}   ARCFOURCONTEXT, *PARCFOURCONTEXT;

typedef	struct	PACKED _IV_CONTROL_
{
	union PACKED
	{
		struct PACKED
		{
			UCHAR		rc0;
			UCHAR		rc1;
			UCHAR		rc2;

			union PACKED
			{
				struct PACKED
				{
#ifdef BIG_ENDIAN
					UCHAR	KeyID:2;
					UCHAR	ExtIV:1;
					UCHAR	Rsvd:5;
#else
					UCHAR	Rsvd:5;
					UCHAR	ExtIV:1;
					UCHAR	KeyID:2;
#endif
				}	field;
				UCHAR		Byte;
			}	CONTROL;
		}	field;
		
		ULONG	word;
	}	IV16;
	
	ULONG	IV32;
}	TKIP_IV, *PTKIP_IV;

typedef struct _CIPHER_KEY {
	UCHAR	Mac[6];	            // as lookup key, for pairwise key only
	UCHAR   BssId[6];
	UCHAR   CipherAlg;          // 0-none, 1:WEP64, 2:WEP128, 3:TKIP, 4:AES, 5:CKIP64, 6:CKIP128
	UCHAR	KeyLen;				// Key length for each key, 0: entry is invalid
	UCHAR	Key[16];			// right now we implement 4 keys, 128 bits max
	UCHAR	RxMic[8];
	UCHAR	TxMic[8];
	UCHAR	TxTsc[6];			// 48bit TSC value
	UCHAR	RxTsc[6];			// 48bit TSC value
	UCHAR   Type;               // Indicate Pairwise/Group when reporting MIC error
	UCHAR   WepKeyType;         // 0: Hex type, 1: Ascii type
}	CIPHER_KEY, *PCIPHER_KEY;

typedef struct _SOFT_RX_ANT_DIVERSITY_STRUCT {
    UCHAR     EvaluatePeriod;		// 0:not evalute status, 1: evaluate status, 2: switching status
    UCHAR     Pair1PrimaryRxAnt;     // 0:Ant-E1, 1:Ant-E2
    UCHAR     Pair1SecondaryRxAnt;   // 0:Ant-E1, 1:Ant-E2
    UCHAR     Pair2PrimaryRxAnt;     // 0:Ant-E3, 1:Ant-E4
    UCHAR     Pair2SecondaryRxAnt;   // 0:Ant-E3, 1:Ant-E4
    SHORT     Pair1AvgRssi[2];       // AvgRssi[0]:E1, AvgRssi[1]:E2
    SHORT     Pair2AvgRssi[2];       // AvgRssi[0]:E3, AvgRssi[1]:E4
    ULONG     RcvPktNumWhenEvaluate;
    BOOLEAN   FirstPktArrivedWhenEvaluate;
    struct timer_list    RxAntDiversityTimer;
} SOFT_RX_ANT_DIVERSITY, *PSOFT_RX_ANT_DIVERSITY;

typedef struct _VIRTUAL_ADAPTER
{
	struct net_device		*RtmpDev;
	struct net_device		*VirtualDev;
} VIRTUAL_ADAPTER, PVIRTUAL_ADAPTER;

typedef struct _MAC_TABLE_ENTRY {
    BOOLEAN         Valid;

    BOOLEAN         RetryTimerRunning;  
	BOOLEAN         EnqueueStartForPSKTimerRunning;  // Enqueue EAPoL-Start for triggering PSK SM
	
    // record which entry revoke MIC Failure , if it leaves the BSS itself, AP won't update aMICFailTime MIB
    UCHAR           CMTimerRunning;
    UCHAR           RSNIE_Len;
    UCHAR           RSN_IE[MAX_LEN_OF_RSNIE];
    UCHAR           ANonce[32];
    UCHAR           R_Counter[LEN_KEY_DESC_REPLAY];
    UCHAR           PTK[64];
    UCHAR           ReTryCounter;   

    struct timer_list                 RetryTimer;
	struct timer_list                 EnqueueStartForPSKTimer;	// A timer which enqueue EAPoL-Start for triggering PSK SM
	
    NDIS_802_11_AUTHENTICATION_MODE     AuthMode;   // This should match to whatever microsoft defined
    NDIS_802_11_WEP_STATUS              WepStatus;
    WPA_STATE       WpaState;
    GTK_STATE       GTKState;
    USHORT          PortSecured;
    NDIS_802_11_PRIVACY_FILTER  PrivacyFilter;      // PrivacyFilter enum for 802.1X
    CIPHER_KEY		PairwiseKey;
    UMAXLONG	   	pAd;    // save pAdapter's address for retrytimer usage
    INT				PMKID_CacheIdx;
    UCHAR			PMKID[LEN_PMKID];
    
    UCHAR			Addr[MAC_ADDR_LEN];
    UCHAR           PsMode;
    SST             Sst;
    AUTH_STATE      AuthState; // for SHARED KEY authentication state machine used only
    USHORT          Aid;
    USHORT          CapabilityInfo;
    UCHAR           MaxSupportedRate;
    UCHAR           CurrTxRate;
    UCHAR           LastTxRate;
    UCHAR           LastRssi;
    ULONG           NoDataIdleCount;
    ULONG           PsQIdleCount;
    QUEUE_HEADER    PsQueue;
    struct _MAC_TABLE_ENTRY *pNext;

    // to record the each TX rate's quality. 0 is best, the bigger the worse.
    USHORT          TxQuality[MAX_LEN_OF_SUPPORTED_RATES];
    UCHAR           PER[MAX_LEN_OF_SUPPORTED_RATES];
    USHORT          OneSecTxOkCount;
    USHORT          OneSecTxRetryOkCount;
    USHORT          OneSecTxFailCount;
    UCHAR           TxRateUpPenalty;      // extra # of second penalty due to last unstable condition
    ULONG           CurrTxRateStableTime; // # of second in current TX rate
    BOOLEAN         fNoisyEnvironment;
    UCHAR           LastSecTxRateChangeAction; // 0: no change, 1:rate UP, 2:rate down

    // Multiple SSID
    UCHAR           ApIdx;
    COUNTER_HOTSPOT HSCounter;

    // a bitmap of BOOLEAN flags. each bit represent an operation status of a particular 
    // BOOLEAN control, either ON or OFF. These flags should always be accessed via
    // CLIENT_STATUS_TEST_FLAG(), CLIENT_STATUS_SET_FLAG(), CLIENT_STATUS_CLEAR_FLAG() macros.
    // see fOP_STATUS_xxx in RTMP_DEF.C for detail bit definition
    ULONG           ClientStatusFlags;

    BOOLEAN			bAPSDCapablePerAC[4];		// 0:AC_BK, 1:AC_BE, 2:AC_VI, 3:AC_VO
    UCHAR			MaxSPLength;
//iverson
   BOOLEAN			bAPSDFlagEOSPOK;
   BOOLEAN			bAPSDFlagSPStart;
//iverson end


    BOOLEAN			bDlsInit;
    BOOLEAN			bDot1xDynamicWep;
#ifdef WSC_SUPPORT
    BOOLEAN         bWscCapable;
    BOOLEAN         EnqueueEapolStartTimerForWscRunning; 
    struct timer_list   EnqueueEapolStartTimerForWsc;
    UCHAR           Receive_EapolStart_EapRspId;
#endif // WSC_SUPPORT //
} MAC_TABLE_ENTRY, *PMAC_TABLE_ENTRY;

typedef struct _MAC_TABLE {
    UCHAR           Size;
    MAC_TABLE_ENTRY *Hash[HASH_TABLE_SIZE];
    MAC_TABLE_ENTRY Content[MAX_LEN_OF_MAC_TABLE];
    QUEUE_HEADER    McastPsQueue;
    ULONG           PsQIdleCount;
    BOOLEAN         fAnyStationInPsm;   // TRUE, then outgoing BCAST/MCAST frame should 
                                        // be put into McastPsQueue until DtimCount=0
	ULONG 			PsmBitmap;			
	ULONG 			PsmBitmap2;		
										
} MAC_TABLE, *PMAC_TABLE;

typedef struct {
    BOOLEAN     Enable;
    UCHAR       Delta;
    BOOLEAN     PlusSign;
} CCK_TX_POWER_CALIBRATE, *PCCK_TX_POWER_CALIBRATE;

// SYNC State Machine Aux data structure
typedef struct _SYNC_AUX {
    UCHAR             Addr[MAC_ADDR_LEN];
    UCHAR             Bssid[MAC_ADDR_LEN];
    UCHAR               BssType;
    UCHAR               SsidLen;
    CHAR                Ssid[MAX_LEN_OF_SSID];
    UCHAR               ScanType;
    UCHAR               Channel;
    struct timer_list   BeaconTimer, ScanTimer;
} SYNC_AUX;

// CNTL State Machine Aux data structure
typedef struct _CNTL_AUX {
    UCHAR               Ssid[MAX_LEN_OF_SSID];
    UCHAR               SsidLen;
    UCHAR             Bssid[MAC_ADDR_LEN];
    ULONG               BssIdx;
    ULONG               RoamIdx;
    BOOLEAN             CurrReqIsFromNdis; // TRUE - then we should call NdisMSetInformationComplete()
                                           // FALSE - req is from driver itself. 
                                           // no NdisMSetInformationComplete() is required
} CNTL_AUX, *PCNTL_AUX;

// AUTH-RSP State Machine Aux data structure
typedef struct _AUTH_RSP_AUX {
    UCHAR             Addr[MAC_ADDR_LEN];
    USHORT              Alg;
    CHAR                Challenge[CIPHER_TEXT_LEN];
} AUTH_RSP_AUX, *PAUTH_RSP_AUX;

typedef struct _MLME_STRUCT {
	STATE_MACHINE           AssocMachine, AuthMachine, AuthRspMachine, SyncMachine, WpaPskMachine, DlsMachine, WscMachine;
	
    STATE_MACHINE_FUNC      AssocFunc[ASSOC_FUNC_SIZE], DlsFunc[DLS_FUNC_SIZE];
    STATE_MACHINE_FUNC      AuthFunc[AUTH_FUNC_SIZE], AuthRspFunc[AUTH_RSP_FUNC_SIZE];
    STATE_MACHINE_FUNC      SyncFunc[SYNC_FUNC_SIZE], WpaPskFunc[WPA_FUNC_SIZE];
#ifdef WSC_SUPPORT
    STATE_MACHINE_FUNC      WscFunc[WSC_FUNC_SIZE];
#endif // WSC_SUPPORT //

#ifdef APCLI_SUPPORT
	STA_STATE_MACHINE       ApCliAuthMachine;
	STA_STATE_MACHINE       ApCliAssocMachine;
	STA_STATE_MACHINE       ApCliCtrlMachine;
	STA_STATE_MACHINE       ApCliSyncMachine;
	STA_STATE_MACHINE		ApCliWpaPskMachine;

	STA_STATE_MACHINE_FUNC  ApCliAuthFunc[STA_AUTH_FUNC_SIZE];
	STA_STATE_MACHINE_FUNC  ApCliAssocFunc[STA_ASSOC_FUNC_SIZE];
	STA_STATE_MACHINE_FUNC  ApCliCtrlFunc[STA_CTRL_FUNC_SIZE];
	STA_STATE_MACHINE_FUNC  ApCliSyncFunc[STA_SYNC_FUNC_SIZE];
	STA_STATE_MACHINE_FUNC	ApCliWpaPskFunc[STA_WPA_PSK_FUNC_SIZE];
#endif

    SYNC_AUX                SyncAux;
    AUTH_RSP_AUX            AuthRspAux;

    UCHAR                   ErpIeContent;
	UMAXLONG				Now32;
	UMAXLONG				LastOLBCDetectTime;

    ULONG                   ChannelQuality;  // 0..100, Channel Quality Indication for Roaming

    BOOLEAN                 bRunning;
    spinlock_t              TaskLock;
    spinlock_t              MemLock;            		//Memory handler spinlock
    MLME_QUEUE              Queue;

    UINT                    ShiftReg;

    struct timer_list       PeriodicTimer;
	struct timer_list       DFSPeriodicTimer;
	struct timer_list		BeaconUpdateTimer;
	BOOLEAN					bBeaconUpdate;
#if 0
	// Fix WiFi WPA2 4.4.2 because beacon time is not accurate
	UINT		    		TbttCount;
#endif
    ULONG                   PeriodicRound;
#ifdef GEM_LEDMODE
    BOOLEAN                 bLEDblinking;
    struct timer_list       LEDTimer;
#endif

    UCHAR                   DtimCount;      // 0.. DtimPeriod-1
    UCHAR                   DtimPeriod;     // default = 3
} MLME_STRUCT, *PMLME_STRUCT;

//
// Receive Tuple Cache Format
//
typedef struct _TUPLE_CACHE    {
    BOOLEAN         Valid;
    UCHAR			MacAddress[MAC_ADDR_LEN];
    USHORT          Sequence; 
    USHORT          Frag;
}   TUPLE_CACHE, *PTUPLE_CACHE;

//
// Fragment Frame structure
//
typedef struct _FRAGMENT_FRAME {
    UCHAR       Header802_3[14];
    UCHAR       Header_LLC[8];
    UCHAR       Buffer[LENGTH_802_3 + MAX_FRAME_SIZE];
    ULONG       RxSize;
    USHORT      Sequence;
    USHORT      LastFrag;
    ULONG       Flags;          // Some extra frame information. bit 0: LLC presented
}   FRAGMENT_FRAME, *PFRAGMENT_FRAME;

//
// Tkip Key structure which RC4 key & MIC calculation
//
typedef struct _TKIP_KEY_INFO  {
    UINT        nBytesInM;  // # bytes in M for MICKEY
    ULONG       IV16;
    ULONG       IV32;   
    ULONG       K0;         // for MICKEY Low
    ULONG       K1;         // for MICKEY Hig
    ULONG       L;          // Current state for MICKEY
    ULONG       R;          // Current state for MICKEY
    ULONG       M;          // Message accumulator for MICKEY
    UCHAR       RC4KEY[16];
    UCHAR       MIC[8];
}   TKIP_KEY_INFO, *PTKIP_KEY_INFO;

//
// Private / Misc data, counters for driver internal use
//
typedef struct  __PRIVATE_STRUC {
    UMAXLONG       SystemResetCnt;         // System reset counter
    UMAXLONG       TxRingFullCnt;          // Tx ring full occurrance number
    UMAXLONG       ResetCountDown;         // Count down before issue reset
    UMAXLONG       CCAErrCnt;              // CCA error count, for debug purpose, might move to global counter
    UMAXLONG       PhyRxErrCnt;            // PHY Rx error count, for debug purpose, might move to global counter
    UMAXLONG       PhyTxErrCnt;            // PHY Tx error count, for debug purpose, might move to global counter
    // Variables for WEP encryption / decryption in rtmp_wep.c
    UMAXLONG        FCSCRC32;
    UMAXLONG        RxSetCnt;
    UMAXLONG        DecryptCnt;
    ARCFOURCONTEXT  WEPCONTEXT;
    // Tkip stuff
    TKIP_KEY_INFO   Tx;
    TKIP_KEY_INFO   Rx;
}   PRIVATE_STRUC, *PPRIVATE_STRUC;

#ifdef RALINK_ATE
typedef	struct _ATE_INFO {
	UCHAR	Mode;
	CHAR	TxPower;
	UCHAR	Addr1[6];
	UCHAR	Addr2[6];
	UCHAR	Addr3[6];
    UCHAR	Channel;
	ULONG	TxLength;
	ULONG	TxCount;
	ULONG	TxDoneCount;
	ULONG	TxRate;
	ULONG	RFFreqOffset;
	BOOLEAN	bRxFer;
	ULONG	RxTotalCnt;
	ULONG	RxCntPerSec;
}	ATE_INFO, *PATE_INFO;
#endif	// RALINK_ATE

// structure to tune BBP R17 "RX AGC VGC init"
typedef struct _BBP_R17_TUNING {
    BOOLEAN     bEnable;
    UCHAR       R17LowerBoundG;
    UCHAR       R17LowerBoundA;
    UCHAR       R17UpperBoundG;
    UCHAR       R17UpperBoundA;
//    UCHAR       LastR17Value;
//    SHORT       R17Dec;     // R17Dec = 0x79 - RssiToDbm, for old version R17Dec = 0.
//                            // This is signed value
    USHORT      FalseCcaLowerThreshold;  // default 100
    USHORT      FalseCcaUpperThreshold;  // default 512
    UCHAR       R17Delta;                // R17 +- R17Delta whenever false CCA over UpperThreshold or lower than LowerThreshold
} BBP_R17_TUNING, *PBBP_R17_TUNING;

// structure for radar detection and channel switch
typedef struct _RADAR_DETECT_STRUCT {
    BOOLEAN		IEEE80211H;			// 0: disable, 1: enable IEEE802.11h
	UCHAR		CSCount;			//Channel switch counter
	UCHAR		CSPeriod;			//Channel switch period (beacon count)
	UCHAR		RDCount;			//Radar detection counter
	UCHAR		RDMode;				//Radar Detection mode
	UCHAR		RDDurRegion;		//Radar detection duration region
	UCHAR		BBPR16;
	UCHAR		BBPR17;
	UCHAR		BBPR18;
	UCHAR		BBPR21;
	UCHAR		BBPR22;
	UCHAR		BBPR64;
	ULONG		InServiceMonitorCount; // unit: sec
	UCHAR		DetectPattern;
} RADAR_DETECT_STRUCT, *PRADAR_DETECT_STRUCT;

// structure to store channel TX power
typedef struct _CHANNEL_TX_POWER {
    UCHAR      Channel;
    UCHAR       Power;
    USHORT		RemainingTimeForUse;		//unit: sec
}	CHANNEL_TX_POWER, *PCHANNEL_TX_POWER;

//
// Multiple SSID structure
//
typedef struct _MULTISSID_STRUCT {
	UCHAR								Bssid[MAC_ADDR_LEN];
    UCHAR                               SsidLen;
    CHAR                                Ssid[MAX_LEN_OF_SSID];
    USHORT                              CapabilityInfo;
    
    struct net_device                   *MSSIDDev;

	NDIS_802_11_AUTHENTICATION_MODE     AuthMode;
	NDIS_802_11_WEP_STATUS              WepStatus;
	NDIS_802_11_WEP_STATUS				GroupKeyWepStatus;

	UMAXLONG							TxOkCount;
	UMAXLONG							RxOkCount;
	UMAXLONG							ReceivedByteCount;
	UMAXLONG							TransmittedByteCount;
	UMAXLONG							TxErrors;
	UMAXLONG							RxErrors;	
	UMAXLONG							TxDrops;
	UMAXLONG							RxDrops;
	
	CIPHER_KEY                          SharedKey[SHARE_KEY_NUM];
	UCHAR                               DefaultKeyId;

	UCHAR								IndividualWepKeyIdx;

	UCHAR								TxRate;       // RATE_1, RATE_2, RATE_5_5, RATE_11, ...
	UCHAR     							DesiredRates[MAX_LEN_OF_SUPPORTED_RATES];// OID_802_11_DESIRED_RATES
	UCHAR								DesiredRatesIndex;
	UCHAR     							MaxTxRate;            // RATE_1, RATE_2, RATE_5_5, RATE_11

	ULONG            					TimBitmap;      // bit0 for broadcast, 1 for AID1, 2 for AID2, ...so on
    ULONG            					TimBitmap2;     // b0 for AID32, b1 for AID33, ... and so on

	UCHAR								TrafficInd;		// The Traffic Indicator of TIM element

    // WPA
    UCHAR                               GMK[32];
    UCHAR                               PMK[64];
    BOOLEAN                             IEEE8021X;
    BOOLEAN                             PreAuth;
    UCHAR                               GNonce[32];
    UCHAR                               PortSecured;
    NDIS_802_11_PRIVACY_FILTER          PrivacyFilter;
    UCHAR                               BANClass3Data;
    ULONG                               IsolateInterStaTraffic;

    UCHAR                               RSNIE_Len[2];
    UCHAR                               RSN_IE[2][MAX_LEN_OF_RSNIE];

    UCHAR                   			TimIELocationInBeacon;
    UCHAR                   			CapabilityInfoLocationInBeacon;

    NDIS_802_11_PMKID					PMKIDCache;

    // outgoing BEACON frame buffer and corresponding TXD 
    TXD_STRUC                           BeaconTxD;
    CHAR                                BeaconBuf[256]; // NOTE: BeaconBuf should be 4-byte aligned

    BOOLEAN                             bHideSsid;

    USHORT                              VLAN_VID;
    USHORT                              VLAN_Priority;

    RT_802_11_ACL						AccessControlList;

	// EDCA Qos
    BOOLEAN								bWmmCapable;	// 0:disable WMM, 1:enable WMM
    BOOLEAN								bDLSCapable;	// 0:disable DLS, 1:enable DLS

	UCHAR           					DlsPTK[64];		// Due to windows dirver count on meetinghouse to handle 4-way shake
	
#ifdef RTL865X_SOC
	unsigned int						mylinkid;
#endif 
#ifdef IGMP_SNOOP_SUPPORT
	BOOLEAN								IgmpSnoopEnable;	// 0: disable, 1: enable.
#endif // IGMP_SNOOP_SUPPORT //
#ifdef WSC_SUPPORT
	WSC_LV_INFO                         WscIEBeacon;
    WSC_LV_INFO                         WscIEProbeResp;
    // WSC control block
	WSC_CTRL	            WscControl;
#endif // WSC_SUPPORT //
} MULTISSID_STRUCT, *PMULTISSID_STRUCT;

// structure to define WPA Group Key Rekey Interval
typedef struct _RT_802_11_WPA_REKEY {
    ULONG  ReKeyMethod;          // mechanism for rekeying: 0:disable, 1: time-based, 2: packet-based
    ULONG  ReKeyInterval;        // time-based: seconds, packet-based: kilo-packets
} RT_WPA_REKEY,*PRT_WPA_REKEY,RT_802_11_WPA_REKEY, *PRT_802_11_WPA_REKEY;

typedef enum _ABGBAND_STATE_ {
	UNKNOWN_BAND,
	BG_BAND,
	A_BAND,
} ABGBAND_STATE;

// PortConfig
typedef struct _PORT_CONFIG {
    USHORT              BeaconPeriod;

    // Multiple SSID
    UCHAR               BssidNum;
    MULTISSID_STRUCT    MBSSID[MAX_MBSSID_NUM];
    BOOLEAN             bEnableHSCounter;
    
    UCHAR               R_Counter[LEN_KEY_DESC_REPLAY];
    struct timer_list   CounterMeasureTimer;

    UCHAR               CMTimerRunning;
    
    UMAXLONG            aMICFailTime;
    UMAXLONG            PrevaMICFailTime;
    UMAXLONG            MICFailureCounter;
    
    // for Group Rekey 
    RT_WPA_REKEY        WPAREKEY;
    ULONG               REKEYCOUNTER;
    struct timer_list   REKEYTimer;
    UCHAR               REKEYTimerRunning;

    USHORT              RtsThreshold;       // in units of BYTE
    USHORT              FragmentThreshold;
    UINT                AllowFragSize;  // payload allowed for each fragment, precalculate can save time
    BOOLEAN             bFragmentZeroDisable;		// Microsoft use 0 as disable 
    
    // MIB:ieee802dot11.dot11phy(4).dot11PhyTxPowerTable(3)
    ULONG 				TxPowerPercentage;        // 0~100%
    
    // MIB:ieee802dot11.dot11phy(4).dot11PhyDSSSTable(5)
    UCHAR				Channel;        // current (I)BSS channel used in the station
    UCHAR				CountryRegion;	// Enum of country region, 0:FCC, 1:IC, 2:ETSI, 3:SPAIN, 4:France, 5:MKK, 6:MKK1, 7:Israel
    UCHAR				CountryRegionForABand;
    
    // MIB:ieee802dot11.dot11phy(4).dot11SupportedDataRatesTxTable(9)
    // MIB:ieee802dot11.dot11phy(4).dot11SupportedDataRatesRxTable(10)
    UCHAR				SupportedRates[MAX_LEN_OF_SUPPORTED_RATES];    // Supported rates
    ULONG 				BasicRateBitMap;
    UCHAR				SupportedRatesLen;
    UCHAR				ExpectedACKRate[MAX_LEN_OF_SUPPORTED_RATES];

    //
    // other parameters not defined in standard MIB
    //
    UCHAR   Bssid[MAC_ADDR_LEN];
    UCHAR   Broadcast[MAC_ADDR_LEN];            // FF:FF:FF:FF:FF:FF
    UCHAR     LastRssi;             // last received BEACON's RSSI
    USHORT    AvgRssi;
    USHORT      		AvgRssiX8;
    UCHAR				LastRssi2;	// last received BEACON's RSSI
    USHORT    AvgRssi2;
    USHORT      		AvgRssi2X8;
    UCHAR     MlmeRate;
    UCHAR     RtsRate;

    // PHY specification
    UCHAR     PhyMode;    // PHY_11A, PHY_11B, PHY_11BG_MIXED, PHY_ABG_MIXED
    USHORT    Dsifs;      // in units of usec
    USHORT    TxPreamble; // Rt802_11PreambleLong, Rt802_11PreambleShort
    USHORT    TxPreambleInUsed; // Rt802_11PreambleLong, Rt802_11PreambleShort

	UCHAR					ReqVarIELen;				// Length of next VIE include EID & Length
	UCHAR					ReqVarIEs[MAX_VIE_LEN];
	UCHAR					ResVarIELen;				// Length of next VIE include EID & Length
	UCHAR					ResVarIEs[MAX_VIE_LEN];

    ULONG                   EnableTurboRate;      // 1: enable 72/100 Mbps whenever applicable, 0: never use 72/100 Mbps
    ULONG                   IsolateInterStaTrafficBTNBSSID;
    ULONG                   UseBGProtection;      // 0:AUTO, 1-always ON,2-always OFF
    ULONG 		            DisableOLBCDetect;	  // 0: enable OLBC detect; 1 disable OLBC detect 
    ULONG                   UseShortSlotTime;     // 0: disable, 1 - use short slot (9us)

    UCHAR                   Key_Counter[32];  //jan for wpa

    ULONG 					PMKCachePeriod;

    BOOLEAN                 bAutoChannelAtBootup;  // 0: disable, 1: enable
    ULONG                   EepromVersion;        // byte 0: version, byte 1: revision, byte 2~3: unused

	UCHAR					MaxTxPowerLevel;		// It shall indicate the maximum power, in dBm, allowed to be transmitted 

	// EDCA Qos
	EDCA_PARM				BssEdcaParm;	// EDCA parameters to be announced to its local BSS
    QOS_CAPABILITY_PARM		APQosCapability;    // QOS capability of the current associated AP
    EDCA_PARM				APEdcaParm;         // EDCA parameters of the current associated AP
    QBSS_LOAD_PARM			APQbssLoad;         // QBSS load of the current associated AP

    BOOLEAN					bAPSDCapable;

    BOOLEAN					bEnableTxBurst;        // 0: disable, 1: enable TX PACKET BURST
    BOOLEAN					bAggregationCapable;      // 1: enable TX aggregation when the peer supports it
	// a bitmap of BOOLEAN flags. each bit represent an operation status of a particular 
    // BOOLEAN control, either ON or OFF. These flags should always be accessed via
    // OPSTATUS_TEST_FLAG(), OPSTATUS_SET_FLAG(), OP_STATUS_CLEAR_FLAG() macros.
    // see fOP_STATUS_xxx in RTMP_DEF.C for detail bit definition
    ULONG					OpStatusFlags;

	UCHAR					AckPolicy[4];       // ACK policy of the specified AC. see ACK_xxx

	RADAR_DETECT_STRUCT		RadarDetect;
		
	BOOLEAN					bCountryFlag;
	CHAR                    CountryCode[3];

    // temporary latch for Auto channel selection
    UCHAR                   AutoChannel_MaxRssi;  // max RSSI during Auto Channel Selection period
    UCHAR                   AutoChannel_Channel;  // channel number during Auto Channel Selection
    ABGBAND_STATE			BandState;		// For setting BBP used on B/G or A mode.

    ULONG					NumOfAvgRssiSample;
    BOOLEAN					bRetryLimit;
    ULONG 					TxQueueSize;
#ifdef RTL865X_SOC    
    BOOLEAN					bEthWithVLANTag;	// 0: disable, 1: enable
#endif

    BOOLEAN                 bGetAPConfig;

#ifdef CARRIER_DETECTION_SUPPORT
	CARRIER_DETECTION		CarrierDetect;
#endif // CARRIER_DETECTION_SUPPORT //
} PORT_CONFIG, *PPORT_CONFIG;

typedef struct _RT_802_11_WDS_ENTRY {
    struct net_device       *dev;
#ifdef RTL865X_SOC
	unsigned int			mylinkid;
#endif 
    UCHAR                   Valid;
    UCHAR                   WdsAddr[ETH_LENGTH_OF_ADDRESS];
    UCHAR                   MaxSupportedRate;
    UCHAR                   CurrTxRate;
    USHORT                  TxQuality[MAX_LEN_OF_SUPPORTED_RATES];
    USHORT                  OneSecTxOkCount;
    USHORT                  OneSecTxRetryOkCount;
    USHORT                  OneSecTxFailCount;
    ULONG                   CurrTxRateStableTime; // # of second in current TX rate
    UMAXLONG                WdsRcvBeaconTime;
    UCHAR                   TxRateUpPenalty;      // extra # of second penalty due to last unstable condition
    BOOLEAN                 RestrictEntry;

	UMAXLONG                 RxOkCount;
    UMAXLONG                 TxOkCount;
    UMAXLONG                 ReceivedByteCount;
    UMAXLONG                 TransmittedByteCount;
    UMAXLONG                 RxErrors;
    UMAXLONG                 TxErrors;
    UMAXLONG                 RxDrops;
    UMAXLONG                 TxDrops;
} RT_802_11_WDS_ENTRY, *PRT_802_11_WDS_ENTRY;

typedef struct _RT_802_11_APCLI_ENTRY {
    struct net_device       *dev;
#ifdef RTL865X_SOC
    unsigned int            mylinkid;
#endif
    ULONG                   Enable;
    UCHAR                   Valid;
    UCHAR                   SsidLen;
    CHAR                    Ssid[MAX_LEN_OF_SSID];
    UCHAR                   ApCliBssid[ETH_LENGTH_OF_ADDRESS];
    UCHAR                   CfgSsidLen;
    CHAR                    CfgSsid[MAX_LEN_OF_SSID];
    UCHAR                   CfgApCliBssid[ETH_LENGTH_OF_ADDRESS];
    UCHAR                   CurrentAddress[ETH_LENGTH_OF_ADDRESS];
    UCHAR                   MaxSupportedRate;
    UCHAR                   CurrTxRate;
    USHORT                  TxQuality[MAX_LEN_OF_SUPPORTED_RATES];
    USHORT                  OneSecTxOkCount;
    USHORT                  OneSecTxRetryOkCount;
    USHORT                  OneSecTxFailCount;
    ULONG                   CurrTxRateStableTime; // # of second in current TX rate
    UMAXLONG                ApCliRcvBeaconTime;
    UCHAR                   TxRateUpPenalty; // extra # of second penalty due to last unstable condition
    ULONG					NumOfAvgRssiSample;
    BOOLEAN                 RestrictEntry;
    BOOLEAN                 InterfaceReady; // indicate the corresponding interface status. 0: interface down, 1:interface up.

    ULONG                   CtrlCurrState;
    ULONG                   SyncCurrState;
    ULONG                   AuthCurrState;
    ULONG                   AssocCurrState;
    ULONG					WpaPskCurrState;

	USHORT                  AuthReqCnt;
	USHORT                  AssocReqCnt;

	NDIS_802_11_AUTHENTICATION_MODE     AuthMode;
    // Add to support different cipher suite for WPA2/WPA mode
	NDIS_802_11_ENCRYPTION_STATUS		GroupCipher;		// Multicast cipher suite
	NDIS_802_11_ENCRYPTION_STATUS		PairCipher;			// Unicast cipher suite
	BOOLEAN								bMixCipher;			// Indicate current Pair & Group use different cipher suites
	USHORT								RsnCapability;
	
    UCHAR		PSK[100];				// reserve PSK key material
    UCHAR       PSKLen;
    UCHAR       PMK[32];                // WPA PSK mode PMK
    UCHAR       PTK[64];                // WPA PSK mode PTK
    UCHAR		GTK[32];				// GTK from authenticator

	CIPHER_KEY		PairwiseKey;
    CIPHER_KEY      SharedKey[SHARE_KEY_NUM];; 
    UCHAR           DefaultKeyId;

    // WPA 802.1x port control, WPA_802_1X_PORT_SECURED, WPA_802_1X_PORT_NOT_SECURED
    UCHAR       PortSecured;
    
    // store RSN_IE form AP
    UCHAR		RSN_IE_from_AP[MAX_LEN_OF_RSNIE];
	UCHAR		RSN_IELen_from_AP;

	// constructed RSN_IE from ApCliAuthMode and ApCliEncrypType
    UCHAR		RSN_IE[MAX_LEN_OF_RSNIE];
	UCHAR		RSN_IELen; 

    // For WPA countermeasures
    UMAXLONG    LastMicErrorTime;   // record last MIC error time
    ULONG       MicErrCnt;          // Should be 0, 1, 2, then reset to zero (after disassoiciation).
	BOOLEAN                 bBlockAssoc; // Block associate attempt for 60 seconds after counter measure occurred.

    // For WPA-PSK supplicant state
    SS_WPA_STATE   	WpaState;           // Default is SS_NOTUSE
    UCHAR       	ReplayCounter[8];
    UCHAR       	ANonce[32];         // ANonce for WPA-PSK from authenticator
    UCHAR       	SNonce[32];         // SNonce for WPA-PSK
    UCHAR			GNonce[32];			// GNonce for WPA-PSK from authenticator

	UMAXLONG		TxOkCount;
	UMAXLONG		TransmittedByteCount;
	UMAXLONG		RxOkCount;
	UMAXLONG		ReceivedByteCount;	
	UMAXLONG		TxErrors;
	UMAXLONG		RxErrors;	
	UMAXLONG		TxDrops;
	UMAXLONG		RxDrops;
// For AP Client support WPS Modification
#ifdef WSC_SUPPORT
	WSC_CTRL	            WscControl;
#endif // WSC_SUPPORT //
// --
} RT_802_11_APCLI_ENTRY, *PRT_802_11_APCLI_ENTRY;

typedef struct _RT_802_11_APCLI_TABLE {
//    ULONG                   Mode;
//    NDIS_802_11_WEP_STATUS  WepStatus;
//    CIPHER_KEY              Wpa_key;
    RT_802_11_APCLI_ENTRY   ApCliEntry[MAX_APCLI_ENTRY];
} RT_802_11_APCLI_TABLE, *PRT_802_11_APCLI_TABLE;

typedef struct _RT_802_11_WDS_TABLE {
    ULONG                   Mode;
    NDIS_802_11_WEP_STATUS  WepStatus;
    CIPHER_KEY              Wpa_key;
    RT_802_11_WDS_ENTRY     WdsEntry[MAX_WDS_ENTRY];
} RT_802_11_WDS_TABLE, *PRT_802_11_WDS_TABLE;

typedef struct _RT_SIGNAL_STRUC {
	USHORT					Sequence;
    UCHAR					MacAddr[MAC_ADDR_LEN];
    UCHAR					CurrAPAddr[MAC_ADDR_LEN];
    UCHAR					Sig;
} RT_SIGNAL_STRUC, *PRT_SIGNAL_STRUC;

//
//  The miniport adapter structure
//
typedef struct _RTMP_ADAPTER
{
	// linux specific
	struct pci_dev          *pPci_Dev;
	struct net_device       *net_dev;
	
#if WIRELESS_EXT >= 12
    struct iw_statistics iw_stats;
#endif
    struct net_device_stats stats;

    INT						chip_id;
    UMAXLONG                CSRBaseAddress;

    // resource for DMA operation
    RTMP_TX_RING            TxRing[NUM_OF_TX_RING];                  // AC0~4 + HCCA
    RTMP_RX_RING            RxRing;
    RTMP_MGMT_RING          MgmtRing;
    RTMP_DMABUF             TxDescRing[NUM_OF_TX_RING]; // Shared memory for Tx descriptors
    RTMP_DMABUF             TxBufSpace[NUM_OF_TX_RING]; // Shared memory of all 1st pre-allocated TxBuf associated with each TXD
    RTMP_DMABUF             RxDescRing;                 // Shared memory for RX descriptors
    RTMP_DMABUF             MgmtDescRing;               // Shared memory for MGMT descriptors

    // resource for software backlog queues
    QUEUE_HEADER            TxSwQueue[NUM_OF_TX_RING];  // 4 AC + 1 HCCA
    QUEUE_HEADER            NullFrameTxSwQueue;          // dedicated to transmit NULL fram.
    
    // SpinLocks
	spinlock_t				TxRingLock;					// Tx Ring spinlock
	spinlock_t				MgmtRingLock;				// Prio Ring spinlock
	spinlock_t				RxRingLock;					// Rx Ring spinlock
	spinlock_t				TxSwQueueLock;				// TxSwQueue spinlock
    
    // configuration: read from Registry & E2PROM
    UCHAR                   CurrentAddress[MAC_ADDR_LEN];      // User changed MAC address

    MLME_STRUCT             Mlme;

    // RX Tuple chahe for duplicate frame check
	TUPLE_CACHE				TupleCache[MAX_CLIENT];		// Maximum number of tuple caches, only useful in Ad-Hoc
	UCHAR                   TupleCacheLastUpdateIndex;  // 0..MAX_CLIENT-1

	// RX re-assembly buffer for fragmentation
	FRAGMENT_FRAME			FragFrame;					// Frame storage for fragment frame
	
    // various Counters 
    COUNTER_802_3           Counters8023;               // 802.3 counters
    COUNTER_802_11          WlanCounters;				// 802.11 MIB counters
    COUNTER_RALINK          RalinkCounters;				// Ralink propriety counters
    COUNTER_DRS             DrsCounters;                // counters for Dynamic TX Rate Switching
	PRIVATE_STRUC			PrivateInfo;				// Private information & counters

    // flags, see fRTMP_ADAPTER_xxx flags
    ULONG                   Flags;                      // Represent current device status

    // current TX sequence #
	USHORT					Sequence;

	// Control disconnect / connect event generation
	UMAXLONG				LinkDownTime;
	ULONG					LastRxRate;

    // ---------------------------
    // E2PROM
    // ---------------------------
    ULONG                   EepromVersion;          // byte 0: version, byte 1: revision, byte 2~3: unused
    UCHAR					EEPROMAddressNum;		// 93c46=6	93c66=8
    USHORT					EEPROMDefaultValue[NUM_EEPROM_BBP_PARMS];

    // ---------------------------
    // BBP Control
    // ---------------------------
    UCHAR                   BbpWriteLatch[110];     // record last BBP register value written via BBP_IO_WRITE/BBP_IO_WRITE_VY_REG_ID
    UCHAR                   BbpRssiToDbmDelta;
    BBP_R17_TUNING          BbpTuning;

    // ----------------------------
    // RFIC control
    // ----------------------------
    UCHAR                   RfIcType;       // RFIC_xxx
    ULONG 					RfFreqOffset;	// Frequency offset for channel switching
	BOOLEAN                 bAutoTxAgc;		// Enable driver auto Tx Agc control
    RTMP_RF_REGS            LatchRfRegs;    // latch th latest RF programming value since RF IC doesn't support READ
    CCK_TX_POWER_CALIBRATE  CckTxPowerCalibrate;    // 2004-05-25 add CCK TX power caliberation based on E2PROM settings

	UCHAR					RFProgSeq;
	EEPROM_ANTENNA_STRUC	Antenna;							// Since ANtenna definition is different for a & g. We need to save it for future reference.
	EEPROM_NIC_CONFIG2_STRUC    NicConfig2;
	EEPROM_TX_POWER_DELTA_STRUC TxPowerDelta;
	UCHAR						LatchBBPR94;
	CHANNEL_TX_POWER		TxPower[MAX_NUM_OF_CHANNELS];		// Store Tx power value for all channels.
	CHANNEL_TX_POWER		ChannelList[MAX_NUM_OF_CHANNELS];   // list all supported channels for site survey
	UCHAR	    ChannelTssiRef[MAX_NUM_OF_CHANNELS];		// Store Tssi Reference value for all channels.
	UCHAR	    ChannelTssiDelta;							// Store Tx TSSI delta increment / decrement value
	UCHAR       ChannelListNum;                           // number of channel in ChannelList[]

	BOOLEAN     bAutoTxAgcA;                // Enable driver auto Tx Agc control
	UCHAR	    TssiRefA;					// Store Tssi reference value as 25 tempature.	
	UCHAR	    TssiPlusBoundaryA[5];		// Tssi boundary for increase Tx power to compensate.
	UCHAR	    TssiMinusBoundaryA[5];		// Tssi boundary for decrease Tx power to compensate.
	UCHAR	    TxAgcStepA;					// Store Tx TSSI delta increment / decrement value
	CHAR		TxAgcCompensateA;			// Store the compensation (TxAgcStep * (idx-1))

	BOOLEAN     bAutoTxAgcG;                // Enable driver auto Tx Agc control
	UCHAR	    TssiRefG;					// Store Tssi reference value as 25 tempature.	
	UCHAR	    TssiPlusBoundaryG[5];		// Tssi boundary for increase Tx power to compensate.
	UCHAR	    TssiMinusBoundaryG[5];		// Tssi boundary for decrease Tx power to compensate.
	UCHAR	    TxAgcStepG;					// Store Tx TSSI delta increment / decrement value
	CHAR		TxAgcCompensateG;			// Store the compensation (TxAgcStep * (idx-1))

	UCHAR               IoctlIF;
#ifdef APCLI_SUPPORT
	UCHAR       ApcliIF;
// For AP Client support WPS Modification
	BOOLEAN                 bFromApcli;	// use by ioctl
// --
#endif

    // ----------------------------
    // LED control
    // ----------------------------
	MCU_LEDCS_STRUC		LedCntl;
	UCHAR				LedIndicatorStregth;

    // ----------------------------
    // STA specific configuration
    // ----------------------------
#ifdef APCLI_SUPPORT
	MLME_AUX	MlmeAux;					// temporary settings used during MLME state machine
//2007/07/26:KH Add for fixing the bug that when we turn down the ra0 (ifconfig ra0 down)  
//withoug disable apcli0 in advace, if we want to re-start ra0 and apcli0, the system will create a new
//apcli interface.
	BOOLEAN	      ApCliUp;
#endif

    // This soft Rx Antenna Diversity mechanism is used only when user set 
    // RX Antenna = DIVERSITY ON
    SOFT_RX_ANT_DIVERSITY         RxAnt;

    // ----------------------------
    // DEBUG paramerts
    // ----------------------------
    ULONG                   DebugSetting[4];

    RT_802_11_EVENT_TABLE   EventTab;

    PORT_CONFIG             PortCfg;
    RT_802_11_APCLI_TABLE   ApCliTab;
    RT_802_11_WDS_TABLE     WdsTab;

    MAC_TABLE                           MacTab;
	spinlock_t                          MacTabLock;

	BSS_TABLE				ScanTab;     // BSS Table
	UMAXLONG				LastScanTime;     // Record last scan time for issue BSSID_SCAN_LIST
	
	CHAR					BGRssiOffset1;				// Store B/G RSSI#1 Offset value on EEPROM 0x9Ah
	CHAR					BGRssiOffset2;				// Store B/G RSSI#2 Offset value 
	CHAR					ARssiOffset1;				// Store A RSSI#1 Offset value on EEPROM 0x9Ch
	CHAR					ARssiOffset2;				// Store A RSSI#2 Offset value 
	
	struct timer_list       QuickResponeForRateUpTimer;
	BOOLEAN					QuickResponeForRateUpTimerRunning;

	CHAR					DefaultTxAntenna;
	UCHAR					AModeGainValue;						// 11A Gain value

	RT_SIGNAL_STRUC			RTSignal;
	UMAXLONG				ApdPid;		// store 1.x daemon PID
	UMAXLONG				IappPid;	// store IAPP daemon PID

	UCHAR					dfstest;

	BOOLEAN                 NeedSwapToLittleEndian;

#ifdef RALINK_ATE
	ATE_INFO				ate;
#endif	// RALINK_ATE

#ifdef THREAD_ISR
	UINT					Rtmp_Masked_Int;	/* global interrupt mask */

	struct tasklet_struct 	rx_done_task;
	struct tasklet_struct 	tx_done_task;
	struct tasklet_struct 	mgmt_done_task;
	struct tasklet_struct 	ac0_dma_done_task;
	struct tasklet_struct 	ac1_dma_done_task;
	struct tasklet_struct 	ac2_dma_done_task;
	struct tasklet_struct 	ac3_dma_done_task;
	struct tasklet_struct 	hcca_dma_done_task;

	BOOLEAN					rx_done_running;
	BOOLEAN					tx_done_running;
	BOOLEAN					mgmt_done_running;
	BOOLEAN					ac0_dma_done_running;
	BOOLEAN					ac1_dma_done_running;
	BOOLEAN					ac2_dma_done_running;
	BOOLEAN					ac3_dma_done_running;
	BOOLEAN					hcca_dma_done_running;

	BOOLEAN					Rx_Early_Done;
#endif /* THREAD_ISR */

	BOOLEAN					flg_mbss_init;
	BOOLEAN					flg_wds_init;

#ifdef IGMP_SNOOP_SUPPORT
	PMULTICAST_FILTER_TABLE	pMulticastFilterTable;
	UCHAR					IgmpGroupTxRate;
	UCHAR					StaCount[MAX_MBSSID_NUM];
#endif // IGMP_SNOOP_SUPPORT //

#ifdef BLOCK_NET_IF
	BLOCK_QUEUE_ENTRY		blockQueueTab[NUM_OF_TX_RING];
#endif // BLOCK_NET_IF //

#ifdef WSC_SUPPORT
    pid_t                   write_dat_file_pid;
    int                     time_to_die;
    struct semaphore	    write_dat_file_semaphore;		/* to sleep thread on	*/
    struct completion	    write_dat_file_notify;
	BOOLEAN             	WriteWscCfgToDatFile;
#endif // WSC_SUPPORT //

#ifdef TC_SOC // Add polling mode support
	struct timer_list	PollingModeDetect;
	BOOLEAN			PollingModeDetectRunning;
	struct work_struct		PollingDataBH;
#endif

}   RTMP_ADAPTER, *PRTMP_ADAPTER;

//
// SHA context
// for GPL md5 use
/*
typedef struct
{
    ULONG       H[5];
    ULONG       W[80];
    INT         lenW;
    ULONG       sizeHi, sizeLo;
}   SHA_CTX;
*/

//
// Enable & Disable NIC interrupt via writing interrupt mask register
// Since it use ADAPTER structure, it have to be put after structure definition.
//
VOID    NICDisableInterrupt(
    IN  PRTMP_ADAPTER   pAdapter);

VOID    NICEnableInterrupt(
    IN  PRTMP_ADAPTER   pAdapter,
    IN	UINT32 			int_mask);

NDIS_STATUS NICLoadFirmware(
    IN  PRTMP_ADAPTER   pAdapter);

BOOLEAN NICCheckForHang(
    IN  PRTMP_ADAPTER   pAd);

VOID NICUpdateRawCounters(
    IN  PRTMP_ADAPTER   pAdapter);

INT     RT61_close(
    IN  struct net_device *net_dev);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
irqreturn_t RTMPIsr(
    IN  INT             irq, 
    IN  VOID            *dev_instance, 
    IN  struct pt_regs  *rgs);
#else
VOID    RTMPIsr(
    IN  INT             irq, 
    IN  VOID            *dev_instance, 
    IN  struct pt_regs  *rgs);
#endif

INT     RT61_open(
    IN  struct net_device *net_dev);

INT     RTMPSendPackets(
    IN  struct sk_buff *skb, 
    IN  struct net_device *net_dev);

INT     RT61_probe(
    IN  struct pci_dev              *pPci_Dev, 
    IN  const struct pci_device_id  *ent);

INT     RT61_ioctl(
    IN  struct net_device   *net_dev, 
    IN  OUT struct ifreq    *rq, 
    IN  INT                 cmd);

VOID RT61_Init_MSSID(
	IN	PRTMP_ADAPTER	pAd,
	IN	struct net_device *net_dev);

#ifdef WDS_SUPPORT
VOID RT61_Init_WDS(
	IN	PRTMP_ADAPTER	pAd,
	IN	struct net_device *net_dev);
VOID RT61_Init_WDSTAB(
	IN	PRTMP_ADAPTER	pAd);
#endif

VOID RT61_Init_APCLI(
	IN	PRTMP_ADAPTER	pAd,
	IN	struct net_device *net_dev);

INT	VirtualIF_open(
	IN	struct net_device *dev);

INT	VirtualIF_close(
	IN	struct net_device *dev);

INT VirtualIF_ioctl(
	IN	struct net_device   *net_dev,
	IN	struct ifreq        *rq, 
	IN	INT                 cmd);

INT	VirtualIFSendPackets(
	IN	struct sk_buff      *skb, 
	IN	struct net_device   *net_dev);

struct net_device_stats *VirtualIF_get_ether_stats(
	IN	struct net_device *net_dev);

VOID VirtualIF_set_rx_mode(
	IN  struct net_device *net_dev);

VOID    RTMPRingCleanUp(
    IN  PRTMP_ADAPTER   pAdapter,
    IN  UCHAR           RingType);

#if WIRELESS_EXT >= 12
struct iw_statistics *RT61_get_wireless_stats(
    IN  struct net_device *net_dev);
#endif

struct net_device_stats *RT61_get_ether_stats(
    IN  struct net_device *net_dev);

VOID    RT61_set_rx_mode(
    IN  struct net_device *net_dev);

VOID REPORT_ETHERNET_FRAME_TO_LLC(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			p8023hdr,
	IN	PUCHAR			pData,
	IN	ULONG 			DataSize,
	IN	struct net_device	*net_dev);

VOID REPORT_ETHERNET_FRAME_TO_LLC_WITH_NON_COPY(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			p8023hdr,
	IN	PUCHAR			pData,
	IN	ULONG 			DataSize,
	IN	struct net_device	*net_dev);

NDIS_STATUS RTMPAllocAdapterBlock(
    IN	PRTMP_ADAPTER   pAdapter);

NDIS_STATUS RTMPAllocDMAMemory(
    IN  PRTMP_ADAPTER   pAd);

VOID    RTMPFreeDMAMemory(
    IN  PRTMP_ADAPTER   pAd);

VOID    NICReadEEPROMParameters(
    IN  PRTMP_ADAPTER       pAdapter);

VOID    NICInitAsicFromEEPROM(
    IN  PRTMP_ADAPTER       pAdapter);

VOID    NICInitTxRxRingAndBacklogQueue(
    IN  PRTMP_ADAPTER   pAdapter);

NDIS_STATUS	NICInitializeAdapter(
    IN  PRTMP_ADAPTER   pAdapter);

VOID    NICInitializeAsic(
    IN  PRTMP_ADAPTER   pAdapter);

VOID    NICIssueReset(
    IN  PRTMP_ADAPTER   pAdapter);

VOID	PortCfgInit(
	IN	PRTMP_ADAPTER pAdapter);

VOID    NICResetFromError(
    IN  PRTMP_ADAPTER   pAdapter);

PUCHAR  RTMPFindSection(
    IN  PCHAR   buffer);

INT RTMPGetKeyParameter(
    IN  PCHAR   key,
    OUT PCHAR   dest,   
    IN  INT     destsize,
    IN  PCHAR   buffer);

//iverson
INT RTMPGetKeyParameter_SSID(
    IN  PCHAR   key,
    OUT PCHAR   dest,   
    IN  INT     destsize,
    IN  PCHAR   buffer);

//iverson end
VOID    RTMPReadParametersFromFile(
    IN  PRTMP_ADAPTER   pAd);
    
ULONG   RTMPEqualMemory(
    IN  PVOID   pSrc1,
    IN  PVOID   pSrc2,
    IN  ULONG   Length);

ULONG   RTMPCompareMemory(
    IN  PVOID   pSrc1,
    IN  PVOID   pSrc2,
    IN  ULONG   Length);

VOID    NdisZeroMemory(
    IN  PVOID   pSrc,
    IN  ULONG   Length);

VOID    NdisMoveMemory(
    OUT PVOID   pDest,
    IN  PVOID   pSrc,
    IN  ULONG   Length);

VOID AtoH(
	IN CHAR		*src,
	OUT UCHAR	*dest,
	IN INT		destlen);

VOID HtoA(
	IN UCHAR	*src,
	OUT CHAR	*dest,
	IN INT		destlen);

UCHAR BtoH(
	IN CHAR		ch);

CHAR HtoB(
	IN CHAR		ch);

VOID RTMPSetLED(
	IN PRTMP_ADAPTER 	pAd, 
	IN UCHAR			Status);

VOID	RTMPBuildDesireRate(
	IN PRTMP_ADAPTER 	pAd,
	IN UCHAR			apidx,
	IN UCHAR			rate);

VOID RTMPPrepareRadarDetectParams(
	IN PRTMP_ADAPTER	pAd);

//
// Private routines in rtmp_data.c
//
VOID    RTMPHandleRxDoneInterrupt(
    IN  PRTMP_ADAPTER   pAdapter);

VOID	RTMPHandleTxDoneInterrupt(
	IN	PRTMP_ADAPTER	pAdapter);

VOID    RTMPHandleTxRingDmaDoneInterrupt(
	IN	PRTMP_ADAPTER	pAdapter,
	IN  INT_SOURCE_CSR_STRUC TxRingBitmap);

VOID	RTMPHandleMgmtRingDmaDoneInterrupt(
	IN	PRTMP_ADAPTER	pAdapter);

NDIS_STATUS RTMPSendPacket(
    IN  PRTMP_ADAPTER   pAdapter,
    IN  struct sk_buff *skb);
    
 VOID    RTMPDeQueuePacket(
    IN  PRTMP_ADAPTER   pAdapter,
    IN  BOOLEAN			bMoreData,
    IN	BOOLEAN			bIntContext);

NDIS_STATUS	RTMPFreeTXDRequest(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	UCHAR			RingType,
	IN	UCHAR			NumberRequired,
    IN 	PUCHAR          FreeNumberIs);

NDIS_STATUS	MlmeHardTransmit(
	IN	PRTMP_ADAPTER	pAdapter,
	IN  struct sk_buff	*pPacket);

USHORT  RTMPCalcDuration(
    IN  PRTMP_ADAPTER   pAdapter,
    IN  UCHAR           Rate,
    IN  ULONG           Size);

#ifdef APCLI_SUPPORT
VOID	RTMPCheckRates(
	IN PRTMP_ADAPTER  pAd,
	IN OUT UCHAR      SupRate[],
	IN OUT UCHAR      *SupRateLen);
#endif

#ifdef CONFIG_64BIT
VOID	RTMPWriteTxDescriptor(
    IN  PRTMP_ADAPTER pAd,
	IN	PTXD_STRUC	pSourceTxD,
	IN	UCHAR		CipherAlg,
	IN  UCHAR       KeyTable,
	IN  UCHAR       KeyIdx,
	IN	BOOLEAN		Ack,
	IN	BOOLEAN		Fragment,
	IN  BOOLEAN     InsTimestamp,
	IN	UCHAR		RetryMode,
	IN	UCHAR		Ifs,
	IN	UINT		Rate,
//	IN	UCHAR		Service,
	IN	ULONG		Length,
	IN  UCHAR       AccessCategory,
	IN  UCHAR       PID,
	IN  struct sk_buff  *pSkb,
	IN  struct sk_buff  *pNextSkb,
	IN  BOOLEAN     bWmmCapable,
	IN  BOOLEAN     bPiggyBack,
	IN	BOOLEAN		bAfterRTSCTS,
	IN	BOOLEAN		bBurstMode,
	IN  RTMP_DMACB *pDmacb);
#else
	VOID	RTMPWriteTxDescriptor(
    IN  PRTMP_ADAPTER pAd,
	IN	PTXD_STRUC	pSourceTxD,
	IN	UCHAR		CipherAlg,
	IN  UCHAR       KeyTable,
	IN  UCHAR       KeyIdx,
	IN	BOOLEAN		Ack,
	IN	BOOLEAN		Fragment,
	IN  BOOLEAN     InsTimestamp,
	IN	UCHAR		RetryMode,
	IN	UCHAR		Ifs,
	IN	UINT		Rate,
//	IN	UCHAR		Service,
	IN	ULONG		Length,
	IN  UCHAR       AccessCategory,
	IN  UCHAR       PID,
	IN  struct sk_buff  *pSkb,
	IN  struct sk_buff  *pNextSkb,
	IN  BOOLEAN     bWmmCapable,
	IN  BOOLEAN     bPiggyBack,
	IN	BOOLEAN		bAfterRTSCTS,
	IN	BOOLEAN		bBurstMode);
#endif // CONFIG_64BIT

BOOLEAN RTMPSearchTupleCache(
    IN  PRTMP_ADAPTER   pAdapter,
    IN  PHEADER_802_11  pHeader);

VOID    RTMPUpdateTupleCache(
    IN  PRTMP_ADAPTER   pAdapter,
    IN  PHEADER_802_11  pHeader);

VOID    RTMPSuspendMsduTransmission(
    IN  PRTMP_ADAPTER   pAdapter);

VOID    RTMPResumeMsduTransmission(
    IN  PRTMP_ADAPTER   pAdapter);

NDIS_STATUS	MiniportMMRequest(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	PUCHAR			pData,
	IN	UINT			Length);

NDIS_STATUS	RTMPCheckRxError(
	IN	PRXD_STRUC		pRxD);

PQUEUE_HEADER   RTMPCheckTxSwQueue(
    IN  PRTMP_ADAPTER   pAdapter,
    OUT ULONG           *Number,
    OUT UCHAR           *AccessCategory,
    IN	UCHAR		    Order);

#ifndef RTL865X_SOC 
NDIS_STATUS RTMPCloneNdisPacket(
    IN  PRTMP_ADAPTER   pAdapter,
    IN  struct sk_buff	*pInSkb,
    OUT struct sk_buff	**ppOutSkb);
#endif

NDIS_STATUS RTMPAllocateNdisPacket(
    IN  PRTMP_ADAPTER   pAdapter,
    IN  struct sk_buff	**ppSkb,
    IN  PUCHAR          pData,
    IN  UINT            DataLen);

VOID RTMPFreeTXDUponTxDmaDone(
    IN PRTMP_ADAPTER    pAd, 
    IN UCHAR            QueIdx,
    IN UINT             RingSize);

VOID	RTMPPrepareRDCTSFrame(
	IN	PRTMP_ADAPTER	pAd,
	IN	PUCHAR			pDA,
	IN	ULONG			Duration,
	IN  UCHAR           RTSRate,
	IN  UCHAR           CTSIdx,
	IN  UCHAR			FrameGap);

VOID RTMPHandleUAPSDTriggerFrame(
	IN  PRTMP_ADAPTER   pAdapter,
	IN  MAC_TABLE_ENTRY *pEntry,
    IN	UCHAR			OldUP);
//iverson
VOID UAPSD_QoSNullTxDoneHandle(
	IN PRTMP_ADAPTER pAd,
	IN struct sk_buff *pSkb,
	IN UCHAR *dst_mac_p);
	
//iverson end



//
// Private routines in rtmp_wep.c
//
VOID    RTMPInitWepEngine(
    IN  PRTMP_ADAPTER   pAdapter,   
    IN  PUCHAR          pKey,
    IN  UCHAR           KeyId,
    IN  UCHAR           KeyLen, 
    IN  PUCHAR          pDest);

VOID    RTMPEncryptData(
    IN  PRTMP_ADAPTER   pAdapter,   
    IN  PUCHAR          pSrc,
    IN  PUCHAR          pDest,
    IN  UINT            Len);

BOOLEAN	RTMPDecryptData(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	PUCHAR			pSrc,
	IN	UINT			Len);

VOID    RTMPSetICV(
    IN  PRTMP_ADAPTER   pAdapter,
    IN  PUCHAR          pDest);

VOID    ARCFOUR_INIT(
    IN  PARCFOURCONTEXT Ctx,
    IN  PUCHAR          pKey,
    IN  UINT            KeyLen);

UCHAR   ARCFOUR_BYTE(
    IN  PARCFOURCONTEXT     Ctx);

VOID    ARCFOUR_DECRYPT(
    IN  PARCFOURCONTEXT Ctx,
    IN  PUCHAR          pDest, 
    IN  PUCHAR          pSrc,
    IN  UINT            Len);

VOID    ARCFOUR_ENCRYPT(
    IN  PARCFOURCONTEXT Ctx,
    IN  PUCHAR          pDest,
    IN  PUCHAR          pSrc,
    IN  UINT            Len);

ULONG   RTMP_CALC_FCS32(
    IN  ULONG   Fcs,
    IN  PUCHAR  Cp,
    IN  INT     Len);

// Asic/RF/BBP related functions

VOID AsicAdjustTxPower(
    IN PRTMP_ADAPTER pAd);

VOID    AsicSwitchChannel(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN UCHAR Channel);

VOID    AsicLockChannel(
    IN PRTMP_ADAPTER pAd, 
    IN UCHAR Channel) ;

VOID	AsicAntennaSelect(
    IN	PRTMP_ADAPTER	pAdapter,
    IN	UCHAR			Channel);

VOID	AsicAntennaSetting(
    IN	PRTMP_ADAPTER	pAd,
    IN	ABGBAND_STATE	BandState);

VOID    AsicSetBssid(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  PUCHAR pBssid);

VOID    AsicDisableSync(
    IN  PRTMP_ADAPTER   pAdapter);

VOID    AsicEnableBssSync(
    IN  PRTMP_ADAPTER   pAdapter);

VOID AsicSetEdcaParm(
    IN PRTMP_ADAPTER pAd,
    IN PEDCA_PARM    pEdcaParm);

VOID AsicSetSlotTime(
    IN PRTMP_ADAPTER pAd,
    IN BOOLEAN UseShortSlotTime);

VOID AsicResetBbpTuning(
	IN PRTMP_ADAPTER pAd);

VOID AsicBbpTuning(
    IN PRTMP_ADAPTER pAd);

VOID AsicAddSharedKeyEntry(
    IN PRTMP_ADAPTER pAd,
    IN UCHAR         BssIndex,
    IN UCHAR         KeyIdx,
    IN UCHAR         CipherAlg,
    IN PUCHAR        pKey,
    IN PUCHAR        pTxMic,
    IN PUCHAR        pRxMic);

VOID AsicRemoveSharedKeyEntry(
    IN PRTMP_ADAPTER pAd,
    IN UCHAR         BssIndex,
    IN UCHAR         KeyIdx);

VOID AsicAddPairwiseKeyEntry(
    IN PRTMP_ADAPTER pAd,
    IN PUCHAR        pAddr,
    IN UCHAR         KeyIdx,
    IN UCHAR         CipherAlg,
    IN PUCHAR        pKey,
    IN PUCHAR        pTxMic,
    IN PUCHAR        pRxMic);

VOID AsicRemovePairwiseKeyEntry(
    IN PRTMP_ADAPTER  pAd,
    IN UCHAR         KeyIdx);

BOOLEAN AsicSendCommandToMcu(
    IN PRTMP_ADAPTER pAd,
    IN UCHAR         Command,
    IN UCHAR         Token,
    IN UCHAR         Arg0,
    IN UCHAR         Arg1);

VOID    MgtMacHeaderInit(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN OUT PHEADER_802_11 pHdr80211, 
    IN UCHAR Subtype, 
    IN UCHAR ToDs, 
//  IN UCHAR AddrType, 
    IN PUCHAR pDA, 
    IN PUCHAR pBssid);

#ifdef APCLI_SUPPORT
VOID ApCliMgtMacHeaderInit(
    IN PRTMP_ADAPTER pAd,
    IN OUT PHEADER_802_11 pHdr80211,
    IN UCHAR SubType,
    IN UCHAR ToDs,
    IN PUCHAR pDA,
    IN PUCHAR pBssid,
    IN USHORT ifIndex);
#endif

NDIS_STATUS  MlmeQueueInit(
    IN MLME_QUEUE *Queue);

VOID  MlmeQueueDestroy(
    IN MLME_QUEUE *Queue);

BOOLEAN MlmeEnqueue(
    IN PRTMP_ADAPTER pAdapter, 
    IN ULONG Machine, 
    IN ULONG MsgType, 
    IN ULONG MsgLen, 
    IN VOID *Msg);

BOOLEAN MlmeEnqueueForRecv(
    IN	PRTMP_ADAPTER	pAd, 
    IN ULONG TimeStampHigh, 
    IN ULONG TimeStampLow,
    IN UCHAR Rssi, 
    IN ULONG MsgLen, 
    IN VOID *Msg,
    IN UCHAR Offset);

#ifdef WSC_SUPPORT
BOOLEAN MlmeEnqueueForWsc(
	IN PRTMP_ADAPTER	pAd,
	IN ULONG eventID,
	IN LONG senderID,
	IN ULONG Machine,
	IN ULONG MsgType,
	IN ULONG MsgLen,
	IN VOID *Msg);
#endif // WSC_SUPPORT //

BOOLEAN MlmeDequeue(
    IN MLME_QUEUE *Queue, 
    OUT MLME_QUEUE_ELEM **Elem);

BOOLEAN MlmeQueueEmpty(
    IN MLME_QUEUE *Queue);

BOOLEAN MlmeQueueFull(
    IN MLME_QUEUE *Queue);

BOOLEAN  MsgTypeSubst(
    IN PRTMP_ADAPTER pAd, 
    IN PFRAME_802_11 pFrame, 
    OUT INT *Machine, 
    OUT INT *MsgType,
    IN UCHAR Offset);

#ifdef APCLI_SUPPORT
BOOLEAN StaMlmeEnqueue(
    IN	PRTMP_ADAPTER	pAd,
    IN ULONG Machine,
    IN ULONG MsgType,
    IN ULONG MsgLen,
    IN VOID *Msg,
    IN USHORT ifIndex);

BOOLEAN ApCliMsgTypeSubst(
    IN PRTMP_ADAPTER  pAd,
    IN PFRAME_802_11 pFrame, 
    OUT INT *Machine, 
    OUT INT *MsgType,
    IN UCHAR Offset);

BOOLEAN preCheckMsgTypeSubset(
    IN PRTMP_ADAPTER  pAd,
    IN PFRAME_802_11 pFrame, 
    OUT INT *Machine, 
    OUT INT *MsgType,
    IN UCHAR Offset);
#endif

VOID StateMachineInit(
    IN STATE_MACHINE *Sm, 
    IN STATE_MACHINE_FUNC Trans[], 
    IN ULONG StNr, 
    IN ULONG MsgNr, 
    IN STATE_MACHINE_FUNC DefFunc, 
    IN ULONG InitState, 
    IN ULONG Base);

VOID StateMachineSetAction(
    IN STATE_MACHINE *S, 
    IN ULONG St, 
    ULONG Msg, 
    IN STATE_MACHINE_FUNC F);

VOID StateMachinePerformAction(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN STATE_MACHINE *S, 
    IN MLME_QUEUE_ELEM *Elem);

#ifdef APCLI_SUPPORT
VOID StaStateMachinePerformAction(
    IN PRTMP_ADAPTER	pAd, 
    IN STA_STATE_MACHINE *S, 
    IN MLME_QUEUE_ELEM *Elem,
    USHORT ifIndex,
    PULONG pCurrState);

SHORT ApCliIfLookUp(
    IN PRTMP_ADAPTER pAd, 
    IN VOID *Msg);

BOOLEAN isValidApCliIf(
    SHORT ifIndex);

VOID StaDrop(
    IN PRTMP_ADAPTER pAd,
    IN MLME_QUEUE_ELEM *Elem,
    PULONG pCurrState,
    USHORT ifIndex);
#endif

VOID Drop(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN MLME_QUEUE_ELEM *Elem);

VOID StateMachineDestroy(
    IN STATE_MACHINE *Sm);

//
// Private routines in assoc.c
//
VOID  AssocStateMachineInit(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  STATE_MACHINE *Sm, 
    OUT STATE_MACHINE_FUNC Trans[]);

VOID PeerAssocReqAction(
    IN PRTMP_ADAPTER pAd, 
    IN  MLME_QUEUE_ELEM *Elem);

VOID PeerReassocReqAction(
    IN PRTMP_ADAPTER pAd, 
    IN  MLME_QUEUE_ELEM *Elem);

VOID PeerDisassocReqAction(
    IN PRTMP_ADAPTER pAd, 
    IN  MLME_QUEUE_ELEM *Elem);

VOID  MlmeDisassocReqAction(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  MLME_QUEUE_ELEM *Elem);

VOID  Cls3errAction(
    IN	PRTMP_ADAPTER	pAdapter, 
    IN  PUCHAR			pAddr1,
    IN  PUCHAR			pAddr2);

USHORT AssignAid(
    IN PRTMP_ADAPTER pAd,
    IN PMAC_TABLE_ENTRY pEntry);

USHORT BuildAssociation(
    IN PRTMP_ADAPTER pAd,
    IN PUCHAR      pAddr,
    IN  USHORT CapabilityInfo, 
    IN UCHAR         MaxSupportedRateIn500Kbps,
    IN UCHAR         *RSN,
    IN UCHAR         *pRSNLen,
    IN BOOLEAN			bWmmCapable,
    IN ULONG         ClientRalinkIe,
    OUT USHORT       *pAid);

//
// Private routines in auth.c
//
VOID AuthStateMachineInit(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN PSTATE_MACHINE sm, 
    OUT STATE_MACHINE_FUNC Trans[]);

VOID MlmeDeauthReqAction(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  MLME_QUEUE_ELEM *Elem);

VOID Cls2errAction(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  PUCHAR			pAddr1,
    IN  PUCHAR			pAddr2);

//
// Private routines in auth_rsp.c
//
VOID AuthRspStateMachineInit(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  PSTATE_MACHINE Sm, 
    IN  STATE_MACHINE_FUNC Trans[]);

VOID PeerAuthAtAuthRspIdleAction(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  MLME_QUEUE_ELEM *Elem);

VOID PeerDeauthReqAction(
    IN PRTMP_ADAPTER pAd, 
    IN PMLME_QUEUE_ELEM Elem);

VOID PeerAuthSimpleRspGenAndSend(
    IN PRTMP_ADAPTER pAd, 
    IN PHEADER_802_11 Hdr, 
    IN USHORT Alg, 
    IN USHORT Seq, 
    IN USHORT StatusCode,
    IN UCHAR apidx);

//
// Private routines in connect.c
//
VOID MakeAllBssBeacon(
    IN PRTMP_ADAPTER pAd);

VOID UpdateAllBeaconFrame(
    IN PRTMP_ADAPTER pAd);

VOID MakeBssBeacon(
    IN PRTMP_ADAPTER	pAd,
    IN INT				apidx);

VOID UpdateBeaconFrame(
    IN PRTMP_ADAPTER	pAd,
    IN INT				apidx);

#ifdef APCLI_SUPPORT
VOID ApCliLinkDown(
    IN 	PRTMP_ADAPTER 	pAd,
    IN	UCHAR			ifIndex);

VOID AssocParmFill(
    IN PRTMP_ADAPTER pAd, 
    IN OUT MLME_ASSOC_REQ_STRUCT *AssocReq, 
    IN PUCHAR pAddr, 
    IN USHORT CapabilityInfo, 
    IN ULONG Timeout, 
    IN USHORT ListenIntv);

VOID DisassocParmFill(
    IN PRTMP_ADAPTER pAd, 
    IN OUT MLME_DISASSOC_REQ_STRUCT *DisassocReq, 
    IN PUCHAR pAddr, 
    IN USHORT Reason);

VOID JoinParmFill(
    IN PRTMP_ADAPTER pAd, 
    IN OUT MLME_JOIN_REQ_STRUCT *JoinReq, 
    PUCHAR Bssid,
    UCHAR SsidLen,
    PCHAR Ssid);
#endif

//
// Private routines in Sanity.c
//
BOOLEAN MlmeScanReqSanity(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  VOID *Msg, 
    IN  ULONG MsgLen, 
    OUT UCHAR *BssType, 
    OUT CHAR ssid[], 
    OUT UCHAR *SsidLen, 
    OUT UCHAR *ScanType);

BOOLEAN PeerAuthSanity(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  VOID *Msg, 
    IN  ULONG MsgLen, 
    OUT PUCHAR pAddr1,
    OUT PUCHAR pAddr2,
    OUT USHORT *Alg, 
    OUT USHORT *Seq, 
    OUT USHORT *Status, 
    OUT CHAR ChlgText[]);

BOOLEAN PeerProbeReqSanity(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  VOID *Msg, 
    IN  ULONG MsgLen, 
    OUT PUCHAR pAddr2,
    OUT CHAR Ssid[], 
    OUT UCHAR *SsidLen); 

BOOLEAN PeerBeaconAndProbeRspSanity(
    IN PRTMP_ADAPTER pAd, 
    IN VOID *Msg, 
    IN ULONG MsgLen, 
    OUT PUCHAR pAddr2, 
    OUT PUCHAR pBssid, 
    OUT CHAR Ssid[], 
    OUT UCHAR *SsidLen, 
    OUT UCHAR *BssType, 
    OUT USHORT *BeaconPeriod, 
    OUT UCHAR *Channel, 
    OUT LARGE_INTEGER *Timestamp, 
    OUT CF_PARM *Cf, 
    OUT USHORT *AtimWin, 
    OUT USHORT *CapabilityInfo, 
    OUT UCHAR *Erp,
    OUT UCHAR *DtimCount, 
    OUT UCHAR *DtimPeriod, 
    OUT UCHAR *BcastFlag, 
    OUT UCHAR *MessageToMe, 
    OUT UCHAR SupRate[],
    OUT UCHAR *SupRateLen,
    OUT UCHAR ExtRate[],
    OUT UCHAR *ExtRateLen,
    OUT UCHAR *CkipFlag,
    OUT UCHAR *AironetCellPowerLimit,   
    OUT PEDCA_PARM       pEdcaParm,
    OUT PQBSS_LOAD_PARM  pQbssLoad,
    OUT PQOS_CAPABILITY_PARM pQosCapability,
    OUT ULONG *pRalinkIe,
    OUT UCHAR *LengthVIE,
    OUT PNDIS_802_11_VARIABLE_IEs pVIE);

UCHAR ChannelSanity(
    IN PRTMP_ADAPTER pAd, 
    IN UCHAR channel);

BOOLEAN PeerAssocReqSanity(
    IN PRTMP_ADAPTER pAd, 
    IN VOID *Msg, 
    IN ULONG MsgLen, 
    OUT PUCHAR pAddr1,
    OUT PUCHAR pAddr2,
    OUT USHORT *pCapabilityInfo, 
    OUT USHORT *pListenInterval, 
    OUT UCHAR *pSsidLen,
    OUT char *Ssid,
    OUT UCHAR *pRatesLen,
    OUT UCHAR Rates[],
    OUT UCHAR *RSN,
    OUT UCHAR *pRSNLen,
    OUT BOOLEAN *pbWmmCapable,
#ifdef WSC_SUPPORT
    OUT BOOLEAN *pWscCapable,
#endif // WSC_SUPPORT //
    OUT ULONG  *pRalinkIe);

BOOLEAN PeerReassocReqSanity(
    IN	PRTMP_ADAPTER	pAd, 
    IN  VOID *Msg, 
    IN  ULONG MsgLen, 
    OUT PUCHAR pAddr1, 
    OUT PUCHAR pAddr2, 
    OUT USHORT *pCapabilityInfo, 
    OUT USHORT *pListenInterval, 
    OUT PUCHAR pApAddr,
    OUT UCHAR *pSsidLen,
    OUT char *Ssid,
    OUT UCHAR *pRatesLen,
    OUT UCHAR Rates[],
    OUT UCHAR *RSN,
    OUT UCHAR *pRSNLen,
    OUT BOOLEAN *pbWmmCapable,
#ifdef WSC_SUPPORT
    OUT BOOLEAN *pWscCapable,
#endif // WSC_SUPPORT //
    OUT ULONG  *pRalinkIe);

BOOLEAN PeerDisassocReqSanity(
    IN PRTMP_ADAPTER pAd, 
    IN VOID *Msg, 
    IN ULONG MsgLen, 
    OUT PUCHAR Addr2, 
    OUT USHORT *Reason);

BOOLEAN PeerDeauthReqSanity(
    IN PRTMP_ADAPTER pAd, 
    IN VOID *Msg, 
    IN ULONG MsgLen, 
    OUT PUCHAR Addr2, 
    OUT USHORT *Reason);

BOOLEAN PeerDlsReqSanity(
    IN PRTMP_ADAPTER pAd, 
    IN VOID *Msg, 
    IN ULONG MsgLen,
    OUT PUCHAR pDA,
    OUT PUCHAR pSA,
    OUT USHORT *pCapabilityInfo, 
    OUT USHORT *pDlsTimeout);

BOOLEAN PeerDlsRspSanity(
    IN PRTMP_ADAPTER pAd, 
    IN VOID *Msg, 
    IN ULONG MsgLen,
    OUT PUCHAR pDA,
    OUT PUCHAR pSA,
    OUT USHORT *pCapabilityInfo, 
    OUT USHORT *pStatus);

BOOLEAN PeerDlsTearDownSanity(
    IN PRTMP_ADAPTER pAd, 
    IN VOID *Msg, 
    IN ULONG MsgLen,
    OUT PUCHAR pDA,
    OUT PUCHAR pSA,
    OUT USHORT *pReason);

#ifdef APCLI_SUPPORT
BOOLEAN PeerDeauthSanity(
    IN PRTMP_ADAPTER pAd, 
    IN VOID *Msg, 
    IN ULONG MsgLen, 
    OUT PUCHAR pAddr2, 
    OUT USHORT *pReason);

BOOLEAN MlmeAuthReqSanity(
    IN PRTMP_ADAPTER pAd, 
    IN VOID *Msg, 
    IN ULONG MsgLen, 
    OUT PUCHAR pAddr, 
    OUT ULONG *pTimeout, 
    OUT USHORT *pAlg);

BOOLEAN MlmeAssocReqSanity(
    IN PRTMP_ADAPTER pAd, 
    IN VOID *Msg, 
    IN ULONG MsgLen, 
    OUT PUCHAR pApAddr, 
    OUT USHORT *pCapabilityInfo, 
    OUT ULONG *pTimeout, 
    OUT USHORT *pListenIntv);

BOOLEAN PeerAssocRspSanity(
    IN PRTMP_ADAPTER pAd, 
    IN VOID *pMsg, 
    IN ULONG MsgLen, 
    OUT PUCHAR pAddr2, 
    OUT USHORT *pCapabilityInfo, 
    OUT USHORT *pStatus, 
    OUT USHORT *pAid, 
    OUT UCHAR SupRate[], 
    OUT UCHAR *pSupRateLen,
    OUT UCHAR ExtRate[], 
    OUT UCHAR *pExtRateLen,
    OUT PEDCA_PARM pEdcaParm);

BOOLEAN PeerDisassocSanity(
    IN PRTMP_ADAPTER pAd, 
    IN VOID *Msg, 
    IN ULONG MsgLen, 
    OUT PUCHAR pAddr2, 
    OUT USHORT *pReason);
#endif

//
// Private routines in dls.c
//
VOID DLSStateMachineInit(
    IN	PRTMP_ADAPTER		pAd, 
    IN  STATE_MACHINE		*S, 
    OUT STATE_MACHINE_FUNC	Trans[]);

VOID PeerDlsReqAction(
    IN PRTMP_ADAPTER	pAd, 
    IN MLME_QUEUE_ELEM	*Elem);

VOID PeerDlsRspAction(
    IN PRTMP_ADAPTER	pAd, 
    IN MLME_QUEUE_ELEM	*Elem);

VOID PeerDlsTearDownAction(
    IN PRTMP_ADAPTER	pAd, 
    IN MLME_QUEUE_ELEM	*Elem);

//
// Private routines in Sanity.c
//
VOID SyncStateMachineInit(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  STATE_MACHINE *Sm, 
    OUT STATE_MACHINE_FUNC Trans[]);

VOID ScanTimeout(
    IN  unsigned long data);

VOID InvalidStateWhenScan(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  MLME_QUEUE_ELEM *Elem);

VOID ScanTimeoutAction(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  MLME_QUEUE_ELEM *Elem);

VOID PeerBeaconAtScanAction(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  MLME_QUEUE_ELEM *Elem);

VOID ScanNextChannel(
    IN  PRTMP_ADAPTER   pAdapter);

UCHAR FirstChannel(
    IN  PRTMP_ADAPTER   pAdapter);

UCHAR NextChannel(
    IN  PRTMP_ADAPTER   pAdapter,
    IN  UCHAR channel);

VOID BuildChannelList(
    IN PRTMP_ADAPTER pAdapter);

VOID MlmeScanReqAction(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  MLME_QUEUE_ELEM *Elem);

VOID PeerProbeReqAction(
    IN  PRTMP_ADAPTER pAd, 
    IN  MLME_QUEUE_ELEM *Elem);

VOID PeerBeaconAction(
    IN PRTMP_ADAPTER pAd, 
    IN MLME_QUEUE_ELEM *Elem);

CHAR	ConvertToRssi(
	IN  PRTMP_ADAPTER pAd,
    IN  UCHAR   Rssi,
    IN  UCHAR    RssiNumber);

VOID ApSiteSurvey(
	IN   PRTMP_ADAPTER   pAd);

#ifdef APCLI_SUPPORT
//
// Private routines in apcli_auth.c
//
VOID StaAuthStateMachineInit(
    IN PRTMP_ADAPTER pAd,
    IN PSTA_STATE_MACHINE Sm,
    OUT STA_STATE_MACHINE_FUNC Trans[]);

VOID StaAuthTimeout(
    IN  unsigned long data);

VOID StaMlmeAuthReqAction(
    IN PRTMP_ADAPTER pAd, 
    IN MLME_QUEUE_ELEM *Elem,
    OUT PULONG pCurrState,
    IN USHORT ifIndex);

VOID StaPeerAuthRspAtSeq2Action(
    IN PRTMP_ADAPTER pAd, 
    IN MLME_QUEUE_ELEM *Elem,
    OUT PULONG pCurrState,
    IN USHORT ifIndex);

VOID StaPeerAuthRspAtSeq4Action(
    IN PRTMP_ADAPTER pAd, 
    IN MLME_QUEUE_ELEM *Elem,
    OUT PULONG pCurrState,
    IN USHORT ifIndex);

VOID StaPeerDeauthAction(
	IN PRTMP_ADAPTER pAd,
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex);

VOID StaAuthTimeoutAction(
    IN PRTMP_ADAPTER pAd, 
    IN MLME_QUEUE_ELEM *Elem,
    OUT PULONG pCurrState,
    IN USHORT ifIndex);

VOID StaInvalidStateWhenAuth(
    IN PRTMP_ADAPTER pAd, 
    IN MLME_QUEUE_ELEM *Elem,
    OUT PULONG pCurrState,
    IN USHORT ifIndex);

//
// Private routines in apcli_assoc.c
//
VOID StaAssocStateMachineInit(
    IN	PRTMP_ADAPTER	pAd, 
    IN  STA_STATE_MACHINE *S, 
    OUT STA_STATE_MACHINE_FUNC Trans[]);
    
VOID StaAssocTimeout(
    IN	unsigned long data);

VOID StaMlmeAssocReqAction(
    IN PRTMP_ADAPTER pAd, 
    IN MLME_QUEUE_ELEM *Elem,
    OUT PULONG pCurrState,
    IN USHORT ifIndex);

VOID StaMlmeDisassocReqAction(
    IN PRTMP_ADAPTER pAd, 
    IN MLME_QUEUE_ELEM *Elem,
    OUT PULONG pCurrState,
    IN USHORT ifIndex);

VOID StaPeerAssocRspAction(
    IN PRTMP_ADAPTER pAd, 
    IN MLME_QUEUE_ELEM *Elem,
    OUT PULONG pCurrState,
    IN USHORT ifIndex);

VOID StaPeerDisassocAction(
    IN PRTMP_ADAPTER pAd, 
    IN MLME_QUEUE_ELEM *Elem,
    OUT PULONG pCurrState,
    IN USHORT ifIndex);

VOID StaAssocTimeoutAction(
    IN PRTMP_ADAPTER pAd, 
    IN MLME_QUEUE_ELEM *Elem,
    OUT PULONG pCurrState,
    IN USHORT ifIndex);

VOID StaInvalidStateWhenAssoc(
    IN PRTMP_ADAPTER pAd, 
    IN MLME_QUEUE_ELEM *Elem,
    OUT PULONG pCurrState,
    IN USHORT ifIndex); 

VOID StaInvalidStateWhenDisassociate(
    IN PRTMP_ADAPTER pAd, 
    IN MLME_QUEUE_ELEM *Elem,
    OUT PULONG pCurrState,
    IN USHORT ifIndex); 

VOID AssocPostProc(
    IN PRTMP_ADAPTER pAd, 
    IN PUCHAR pAddr2, 
    IN USHORT CapabilityInfo, 
    IN USHORT Aid, 
    IN UCHAR SupRate[], 
    IN UCHAR SupRateLen,
    IN UCHAR ExtRate[],
    IN UCHAR ExtRateLen,
    IN PEDCA_PARM pEdcaParm);

//
// Private routines in apcli_sync.c
//
VOID StaSyncStateMachineInit(
    IN PRTMP_ADAPTER pAd, 
    IN STA_STATE_MACHINE *Sm, 
    OUT STA_STATE_MACHINE_FUNC Trans[]);

VOID StaProbeTimeout(
    IN  unsigned long data);

VOID StaMlmeProbeReqAction(
    IN PRTMP_ADAPTER pAd, 
    IN MLME_QUEUE_ELEM *Elem,
    OUT PULONG pCurrState,
    IN USHORT ifIndex);

VOID StaPeerProbeRspAtJoinAction(
    IN PRTMP_ADAPTER pAd, 
    IN MLME_QUEUE_ELEM *Elem,
    OUT PULONG pCurrState,
    IN USHORT ifIndex);

VOID StaProbeTimeoutAtJoinAction(
    IN PRTMP_ADAPTER pAd,
    IN MLME_QUEUE_ELEM *Elem,
    OUT PULONG pCurrState,
    IN USHORT ifIndex);

VOID StaInvalidStateWhenJoin(
    IN PRTMP_ADAPTER pAd, 
    IN MLME_QUEUE_ELEM *Elem,
    OUT PULONG pCurrState,
    IN USHORT ifIndex);

VOID EnqueueProbeRequest(
    IN PRTMP_ADAPTER pAd,
    IN UCHAR SsidLen,
    IN PCHAR Ssid,
    IN USHORT ifIndex);

//
// Private routines in apcli_ctrl.c
//
VOID StaCtrlStateMachineInit(
	IN PRTMP_ADAPTER pAd,
	IN STA_STATE_MACHINE *Sm,
	OUT STA_STATE_MACHINE_FUNC Trans[]);

VOID StaCtrlJoinReqAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex);

VOID StaCtrlJoinReqTimeoutAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex);

VOID StaCtrlProbeRspAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex);

VOID StaCtrlAuthRspAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex);

VOID StaCtrlAuth2RspAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex);

VOID StaCtrlAuthReqTimeoutAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex);

VOID StaCtrlAuth2ReqTimeoutAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex);

VOID StaCtrlAssocRspAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex);

VOID StaCtrlDeAssocRspAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex);

VOID StaCtrlAssocReqTimeoutAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex);

VOID StaCtrlDeAssocAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex);

VOID StaCtrlDisconnectReqAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex);

VOID StaCtrlPeerDeAssocReqAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex);

VOID ApCliIfUp(
	IN PRTMP_ADAPTER pAd);

VOID ApCliIfDown(
	IN PRTMP_ADAPTER pAd);

VOID ApCliIfMonitor(
	IN PRTMP_ADAPTER pAd);

VOID ChannelSwitchAnnouncementProc(
	IN PRTMP_ADAPTER pAd,
	IN NDIS_802_11_VARIABLE_IEs	*pVIE,
	UCHAR LenVIE);

#endif	// #ifdef APCLI_SUPPORT

//=========================================

VOID MlmeCntlInit(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  STATE_MACHINE *S, 
    OUT STATE_MACHINE_FUNC Trans[]);

VOID MlmeCntlMachinePerformAction(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  STATE_MACHINE *S, 
    IN  MLME_QUEUE_ELEM *Elem);

ULONG MakeOutgoingFrame(
    OUT CHAR *Buffer, 
    OUT ULONG *Length, ...);

VOID  LfsrInit(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  ULONG Seed);

UCHAR RandomByte(
    IN  PRTMP_ADAPTER   pAdapter);

VOID  MlmePeriodicExec(
    IN  unsigned long data);

VOID MlmeDynamicTxRateSwitching(
    IN PRTMP_ADAPTER pAd);

VOID MlmeSetTxPreamble(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN USHORT TxPreamble);

VOID MlmeUpdateTxRates(
    IN PRTMP_ADAPTER pAdapter);

NDIS_STATUS MlmeInit(
    IN  PRTMP_ADAPTER   pAdapter);

VOID MlmeHandler(
    IN  PRTMP_ADAPTER   pAdapter);

VOID MlmeHalt(
    IN  PRTMP_ADAPTER   pAdapter);

VOID MlmeRadioOff(
	IN PRTMP_ADAPTER pAd);

VOID MlmeRadioOn(
	IN PRTMP_ADAPTER pAd);

// For AP Client support WPS Modification
// Init WSC timer.
#ifdef WSC_SUPPORT
VOID RTMPWscInitTimer(
	IN PWSC_CTRL pWscControl,
	IN struct timer_list *timer,
	IN PVOID timerFunc);
#endif // WSC_SUPPORT //
// --

VOID RTMPInitTimer(
	IN PRTMP_ADAPTER pAd,
	IN struct timer_list *timer,
	IN PVOID timerFunc);

VOID RTMPAddTimer(
	IN struct timer_list *timer,
	IN ULONG Timeout);

VOID RTMPCancelTimer(
	IN struct timer_list *timer);

//
// Prototypes of function definition in eeprom.c
//
VOID RaiseClock(
    IN  PRTMP_ADAPTER   pAd,
    IN  ULONG  *x);

VOID LowerClock(
    IN  PRTMP_ADAPTER   pAd,
    IN  ULONG  *x);

USHORT ShiftInBits(
    IN  PRTMP_ADAPTER   pAd);

VOID ShiftOutBits(
    IN  PRTMP_ADAPTER   pAd,
    IN  USHORT data,
    IN  USHORT count);

VOID EEpromCleanup(
    IN  PRTMP_ADAPTER   pAd);

VOID EWDS(
    IN  PRTMP_ADAPTER   pAd);

VOID EWEN(
    IN  PRTMP_ADAPTER   pAd);
    
USHORT RTMP_EEPROM_READ16(
    IN  PRTMP_ADAPTER   pAd,
    IN  USHORT Offset);

VOID RTMP_EEPROM_WRITE16(
    IN  PRTMP_ADAPTER   pAd,
    IN  USHORT Offset,
    IN  USHORT Data);
    
//
// Prototypes of function definition in rtmp_tkip.c
//
VOID    RTMPInitTkipEngine(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  PUCHAR          pTKey,
    IN  UCHAR           KeyId,
    IN  PUCHAR          pTA,
    IN  PUCHAR          pMICKey,
    IN  PUCHAR          pTSC,
    OUT PULONG          pIV16,
    OUT PULONG          pIV32);

VOID	RTMPInitMICEngine(
	IN	PRTMP_ADAPTER	pAdapter,	
	IN	PUCHAR			pKey,
	IN	PUCHAR			pDA,
	IN	PUCHAR			pSA,
	IN  UCHAR           UserPriority,
	IN	PUCHAR			pMICKey);

BOOLEAN RTMPTkipCompareMICValue(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  PUCHAR          pSrc,
    IN  PUCHAR          pDA,
    IN  PUCHAR          pSA,
    IN  PUCHAR          pMICKey,
    IN  UINT            Len);

VOID	RTMPCalculateMICValue(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct sk_buff  *pSkb,
	IN	PUCHAR			pEncap,
	IN	PCIPHER_KEY		pKey);

BOOLEAN RTMPSoftDecryptTKIP(
	IN PRTMP_ADAPTER pAd,
	IN PUCHAR	pData,
	IN ULONG	DataByteCnt, 
	IN UCHAR    UserPriority,
	IN PCIPHER_KEY	pWpaKey);

BOOLEAN RTMPSoftDecryptAES(
	IN PRTMP_ADAPTER pAd,
	IN PUCHAR	pData,
	IN ULONG	DataByteCnt, 
	IN PCIPHER_KEY	pWpaKey);

BOOLEAN	RTMPTkipCompareMICValueWithLLC(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	PUCHAR			pLLC,
	IN	PUCHAR			pSrc,
	IN	PUCHAR			pDA,
	IN	PUCHAR			pSA,
	IN	PUCHAR			pMICKey,
	IN	UINT			Len);

VOID    RTMPTkipAppend( 
    IN  PTKIP_KEY_INFO  pTkip,  
    IN  PUCHAR          pSrc,
    IN  UINT            nBytes);

VOID    RTMPTkipGetMIC( 
    IN  PTKIP_KEY_INFO  pTkip);

VOID    RTMPSetPhyMode(
    IN	PRTMP_ADAPTER pAdapter, 
    IN  ULONG 	phymode,
    IN	UCHAR	apidx);

VOID    RTMPSetDesiredRates(
    IN  PRTMP_ADAPTER   pAdapter,
    IN  LONG            Rates);

INT RTMPSetInformation(
    IN  PRTMP_ADAPTER pAdapter,
    IN  OUT struct ifreq    *rq,
    IN  INT             cmd);

INT RTMPQueryInformation(
    IN  PRTMP_ADAPTER pAdapter,
    IN  OUT struct ifreq    *rq,
    IN  INT             cmd);

CHAR *GetEncryptType(
	IN CHAR				enc);
//
// Prototypes of function definition for *iwpriv* in rtmp_info.c
//
INT Set_DriverVersion_Proc(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  PUCHAR          arg);

INT Set_CountryRegion_Proc(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  PUCHAR          arg);

INT Set_CountryRegionABand_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg);

INT Set_CountryString_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg);

INT Set_CountryCode_Proc(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  PUCHAR          arg);

INT Set_SSID_Proc(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  PUCHAR          arg);

INT Set_WirelessMode_Proc(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  PUCHAR          arg);

INT Set_TxRate_Proc(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  PUCHAR          arg);

INT Set_AdhocModeRate_Proc(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  PUCHAR          arg);

INT Set_Channel_Proc(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  PUCHAR			arg);

#ifdef DBG
INT Set_Debug_Proc(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  PUCHAR          arg);
#endif

INT	Set_OLBCDetection_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg);

INT Set_BGProtection_Proc(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  PUCHAR          arg);

INT Set_TxPreamble_Proc(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  PUCHAR          arg);

INT Set_StaWithEtherBridge_Proc(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  PUCHAR          arg);

INT Set_RTSThreshold_Proc(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  PUCHAR          arg);

INT Set_FragThreshold_Proc(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  PUCHAR          arg);

INT Set_TxBurst_Proc(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  PUCHAR          arg);

#ifdef AGGREGATION_SUPPORT
INT Set_PktAggregate_Proc(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  PUCHAR          arg);
#endif 

INT Set_TurboRate_Proc(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  PUCHAR          arg);

INT Set_NetworkType_Proc(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  PUCHAR          arg);
    
INT Set_AuthMode_Proc(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  PUCHAR          arg);

INT Set_EncrypType_Proc(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  PUCHAR          arg);

INT Set_DefaultKeyID_Proc(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  PUCHAR          arg);

INT Set_Key1_Proc(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  PUCHAR          arg);

INT Set_Key2_Proc(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  PUCHAR          arg);

INT Set_Key3_Proc(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  PUCHAR          arg);

INT Set_Key4_Proc(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  PUCHAR          arg);

INT Set_WPAPSK_Proc(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  PUCHAR          arg);

INT Set_BasicRate_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg);

INT	Set_BeaconPeriod_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg);

INT	Set_DtimPeriod_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg);

INT	Set_TxPower_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg);

INT	Set_TxAntenna_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg);

INT	Set_RxAntenna_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg);

INT	Set_NoForwardingBTNSSID_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg);

INT	Set_NoForwarding_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg);

INT	Set_WmmCapable_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg);

INT	Set_HideSSID_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg);

INT	Set_ShortSlot_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg);

INT	Set_IEEE8021X_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg);

INT	Set_IEEE80211H_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg);

INT	Set_RDDurRegion_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg);

INT	Set_CSPeriod_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg);

INT	Set_PreAuth_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg);

INT	Set_VLANID_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg);

INT	Set_VLANPriority_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg);

INT	Set_HSCounter_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg);

INT Set_RetryLimit_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg);

INT Set_TxQueueSize_Proc(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  PUCHAR          arg);

INT	Set_AccessPolicy_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg);

INT	Set_AccessControlList_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg);

INT	Set_ResetStatCounter_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg);

INT	Set_RadioOn_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg);

INT	Set_dfstest_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg);

INT	Set_RekeyMethod_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg);

INT	Set_RekeyInterval_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg);

INT	Set_PMKCachePeriod_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg);

INT Set_SiteSurvey_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg);

VOID RTMPIoctlAddPMKIDCache(
	IN	PRTMP_ADAPTER	pAd, 
	IN	struct iwreq	*wrq);

VOID RTMPIoctlAddWPAKey(
	IN	PRTMP_ADAPTER	pAd, 
	IN	struct iwreq	*wrq);

VOID RTMPIoctlStaticWepCopy(
	IN	PRTMP_ADAPTER	pAd, 
	IN	struct iwreq	*wrq);

VOID	RTMPIoctlRadiusData(
	IN PRTMP_ADAPTER	pAd, 
	IN struct iwreq		*wrq);

VOID RTMPIoctlGetMacTable(
	IN PRTMP_ADAPTER pAdapter, 
	IN struct iwreq *wrq);

VOID RTMPIoctlGetSiteSurvey(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	struct iwreq	*wrq);

VOID RTMPIoctlStatistics(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	struct iwreq	*wrq);

VOID RTMP_SIOCGIWSCAN(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	struct iwreq	*wrq);

VOID RTMP_SIOCGIWSTATS(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	struct iwreq	*wrq);

VOID RTMP_SIOCGIWAPLIST(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	struct iwreq	*wrq);

#ifdef DBG
VOID RTMPIoctlBBP(
    IN  PRTMP_ADAPTER   pAdapter,
    IN  struct iwreq    *wrq);

VOID RTMPIoctlMAC(
    IN  PRTMP_ADAPTER   pAdapter,
    IN  struct iwreq    *wrq);

VOID RTMPIoctlE2PROM(
    IN  PRTMP_ADAPTER   pAdapter,
    IN  struct iwreq    *wrq);

VOID RTMPIoctlCHKICVER(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	struct iwreq	*wrq);
#endif

VOID    HMAC_SHA1(
    IN  UCHAR   *text,
    IN  UINT    text_len,
    IN  UCHAR   *key,
    IN  UINT    key_len,
    IN  UCHAR   *digest);

ULONG	RTMPTkipGetUInt32( 	
	IN	PUCHAR	pMICKey);

char * rtstrstr(
	IN	const char * s1,
	IN	const char * s2);

char * rstrtok(
	IN	char * s,
	IN	const char * ct);

VOID DBGPRINT_TX_RING(
    IN PRTMP_ADAPTER  pAdapter,
    IN UCHAR          QueIdx);

VOID	RTMPusecDelay(
	IN		ULONG	usec);

#ifdef RALINK_ATE
INT	Set_ATE_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg);

INT	Set_ATE_DA_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg);

INT	Set_ATE_SA_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg);

INT	Set_ATE_BSSID_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg);

INT	Set_ATE_CHANNEL_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg);

INT	Set_ATE_TX_POWER_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg);

INT	Set_ATE_TX_FREQOFFSET_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg);

INT	Set_ATE_TX_LENGTH_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg);

INT	Set_ATE_TX_COUNT_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg);

INT	Set_ATE_TX_RATE_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg);

INT	Set_ATE_RX_FER_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg);

VOID ATEAsicSwitchChannel(
    IN PRTMP_ADAPTER pAd, 
    IN UCHAR Channel);
#endif	// RALINK_ATE

NDIS_STATUS ApInitialize(
	IN	PRTMP_ADAPTER	pAd);

VOID ApShutdown(
    IN PRTMP_ADAPTER pAd);

VOID ApStartUp(
    IN PRTMP_ADAPTER	pAd,
    IN BOOLEAN			bDFSRestart);

VOID ApStop(
    IN PRTMP_ADAPTER	pAd,
    IN BOOLEAN			bDFSRestart);

VOID SendSingalToDaemon(
	IN PRTMP_ADAPTER	pAd,
	IN INT				sig,
	IN ULONG			pid);

VOID CleanupPsQueue(
    IN  PRTMP_ADAPTER   pAd,
    IN  PQUEUE_HEADER   pQueue);

VOID MacTableReset(
    IN  PRTMP_ADAPTER  pAd,
    IN	BOOLEAN		bDFSRestart);

MAC_TABLE_ENTRY *MacTableInsertEntry(
    IN	PRTMP_ADAPTER	pAd, 
    IN  PUCHAR        pAddr,
    IN  UCHAR           apidx);

MAC_TABLE_ENTRY *MacTableLookup(
    IN PRTMP_ADAPTER pAd, 
    IN PUCHAR pAddr);

BOOLEAN MacTableDeleteEntry(
    IN PRTMP_ADAPTER pAd, 
    IN PUCHAR pAddr);

MAC_TABLE_ENTRY *SsPsInquiry(
    IN	PRTMP_ADAPTER	pAd, 
    IN PUCHAR pAddr, 
    OUT SST   *Sst, 
    OUT USHORT *Aid,
    OUT UCHAR *PsMode,
    OUT UCHAR *Rate);

VOID ApLogEvent(
    IN PRTMP_ADAPTER pAd,
    IN PUCHAR   pAddr,
    IN USHORT   Event,
    IN UCHAR    apidx);

VOID ApUpdateCapabilityAndErpIe(
    IN PRTMP_ADAPTER pAd);

BOOLEAN ApCheckAccessControlList(
    IN PRTMP_ADAPTER pAd,
    IN PUCHAR		 pAddr,
    IN UCHAR         Apidx);

VOID ApUpdateAccessControlList(
    IN PRTMP_ADAPTER pAd,
    IN UCHAR         Apidx);

VOID ApEnqueueNullFrame(
    IN PRTMP_ADAPTER pAd,
    IN PUCHAR		 pAddr,
    IN UCHAR         TxRate,
    IN UCHAR         PID,
    IN UCHAR         Apidx,
    IN BOOLEAN       bQosNull,
    IN BOOLEAN       bEOSP,
    IN UCHAR         OldUP);

VOID    ApSendFrame(
    IN  PRTMP_ADAPTER   pAd,
    IN  PVOID           pBuffer,
    IN  ULONG           Length,
    IN  UCHAR           TxRate,
    IN  UCHAR           PID);

UCHAR APAutoSelectChannel(
    IN PRTMP_ADAPTER 	pAd);

VOID UpdateBasicRateBitmap(
    IN PRTMP_ADAPTER    pAd);

VOID MacTableMaintenance(
    IN PRTMP_ADAPTER pAd);

VOID	WPAARCFOUR_ENCRYPT(
	IN	PARCFOURCONTEXT	Ctx,
	IN	PUCHAR			pDest,
	IN	PUCHAR			pSrc,
	IN	UINT			Len);

NDIS_802_11_WEP_STATUS  BssCipherParse(
    IN 	PBSS_ENTRY	pBss);

VOID AsicSetRxAnt(
    IN PRTMP_ADAPTER	pAd,
    IN UCHAR			Pair1,
    IN UCHAR			Pair2);

VOID RadarDetectionStart(
    IN PRTMP_ADAPTER	pAd);

BOOLEAN RadarDetectionStop(
    IN PRTMP_ADAPTER	pAd);

BOOLEAN RadarChannelCheck(
    IN PRTMP_ADAPTER	pAd,
    IN UCHAR			Ch);

VOID RadarDetectPeriodic(
	IN PRTMP_ADAPTER	pAd);

VOID  QuickResponeForRateUp(
    IN  unsigned long data);


VOID BssTableInit(
    IN BSS_TABLE *Tab);

ULONG BssTableSearch(
    IN BSS_TABLE *Tab, 
    IN PUCHAR Bssid);

VOID BssTableDeleteEntry(
    IN OUT  BSS_TABLE *Tab, 
    IN PUCHAR Bssid);

VOID BssEntrySet(
    IN  PRTMP_ADAPTER   pAd, 
    OUT PBSS_ENTRY pBss, 
    IN PUCHAR pBssid, 
    IN CHAR Ssid[], 
    IN UCHAR SsidLen, 
    IN UCHAR BssType, 
    IN USHORT BeaconPeriod, 
    IN PCF_PARM CfParm, 
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
    IN PNDIS_802_11_VARIABLE_IEs pVIE);

ULONG BssTableSetEntry(
    IN  PRTMP_ADAPTER   pAd, 
    OUT PBSS_TABLE pTab, 
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
    IN UCHAR Channel,
    IN UCHAR Rssi,
    IN LARGE_INTEGER TimeStamp,
    IN UCHAR CkipFlag,
    IN PEDCA_PARM pEdcaParm,
    IN PQOS_CAPABILITY_PARM pQosCapability,
    IN PQBSS_LOAD_PARM pQbssLoad,
    IN UCHAR LengthVIE,
    IN PNDIS_802_11_VARIABLE_IEs pVIE);

#ifndef RTL865X_SOC
BOOLEAN RTMPBridgeToWdsAndWirelessSta(
    IN  PRTMP_ADAPTER   pAdapter,
    IN  PUCHAR          pHeader802_3,
    IN  UINT            HdrLen,
    IN  PUCHAR          pData,
    IN  UINT            DataLen,
    IN  UINT            FromWhichBSSID);

BOOLEAN RTMPCheckDHCPFrame(
	IN	PRTMP_ADAPTER	pAd, 
	IN  struct sk_buff	*pSkb);
	
VOID RadarSMDetectPeriodic(
	IN	unsigned long data);	
#endif

BOOLEAN ApCheckLongPreambleSTA(
    IN PRTMP_ADAPTER pAd);

NDIS_STATUS Sniff2BytesFromNdisBuffer(
    IN  struct sk_buff	*pFirstSkb,
    IN  UCHAR           DesiredOffset,
    OUT PUCHAR          pByte0,
    OUT PUCHAR          pByte1);

VOID RTMPHandleNotify8021xDiscardSta(
    IN  PRTMP_ADAPTER	pAdapter,
    IN  MAC_TABLE_ENTRY *pEntry);

NDIS_STATUS NullFrameHardTransmit(
	IN PRTMP_ADAPTER pAd,
	IN struct sk_buff *pSkb,
	IN UCHAR TxRate,
	IN UCHAR PID);

#ifdef WSC_SUPPORT
INT	Set_WscConfMode_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg);

INT	Set_WscConfStatus_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg);

INT	Set_WscMode_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg);

INT	Set_WscStatus_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg);

INT	Set_WscGetConf_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg);

INT	Set_WscConfigClient_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg);

INT	Set_WscPinCode_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg);

INT	Set_WscOOB_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg);

INT	Set_WscOpenOOB_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg);
// For AP Client support WPS Modification
INT	Set_WscStop_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg);
// --
VOID RTMPIoctlWscProfile(
	IN PRTMP_ADAPTER pAdapter, 
	IN struct iwreq *wrq);

#ifdef DBG
INT	Set_Test_With_Intel_SDK_Proc(
    IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg);
#endif
#endif // WSC_SUPPORT //

extern ULONG  BIT32[];
extern UCHAR BIT8[];
extern char* CipherName[];
extern UCHAR  RateIdToMbps[];
extern USHORT RateIdTo500Kbps[];
extern UCHAR EAPOL[];
extern UCHAR SNAP_802_1H[];
extern UCHAR Phy11BGNextRateUpward[];
extern UCHAR Phy11ANextRateUpward[];
extern RTMP_RF_REGS RF5225RegTable[];
extern RTMP_RF_REGS RF5225RegTable_1[];
extern UCHAR NUM_OF_5225_CHNL;
extern UCHAR NUM_OF_5225_CHNL_1;
extern UCHAR PowerConstraintIE[];
extern UCHAR MapUserPriorityToAccessCategory[];
extern UCHAR BROADCAST_ADDR[MAC_ADDR_LEN];
extern UCHAR ZERO_MAC_ADDR[MAC_ADDR_LEN];

#define	RTMP_SEM_INIT(__lock)			\
{                                       \
	spin_lock_init(__lock);				\
}


#ifndef THREAD_ISR

#define RTMP_SEM_UNLOCK(__lock, __irqflag)		\
{												\
	spin_unlock_irqrestore(__lock, ((unsigned long)__irqflag));	\
}

#define RTMP_SEM_LOCK(__lock, __irqflag)					\
{												\
	spin_lock_irqsave(__lock, ((unsigned long)__irqflag));		\
}

#else /* THREAD_ISR */

extern spinlock_t	Rtmp_Irq_Lock;

#define RTMP_SEM_UNLOCK(__lock, __irqflag)		\
{												\
	__irqflag = 0;								\
	spin_unlock_bh(__lock);						\
}

#define RTMP_SEM_LOCK(__lock, __irqflag)		\
{												\
	__irqflag = 0;								\
	spin_lock_bh(__lock);						\
}

#define RTMP_IRQ_LOCK(__irqflag)					\
{													\
	spin_lock_irqsave(&Rtmp_Irq_Lock, (/*(unsigned long)*/__irqflag));	\
}

#define RTMP_IRQ_UNLOCK(__irqflag)						\
{														\
	spin_unlock_irqrestore(&Rtmp_Irq_Lock, ((unsigned long)__irqflag));	\
}
#endif /* THREAD_ISR */


#ifdef THREAD_ISR
void 	Rtmp_Init_Thread_Task(IN PRTMP_ADAPTER pAdapter);
void 	Rtmp_Kill_Thread_Task(IN PRTMP_ADAPTER pAdapter);
UINT32 	Rtmp_Thread_Isr(IN PRTMP_ADAPTER pAdapter, 
					 	INT_SOURCE_CSR_STRUC	IntSource);
#endif

#ifdef RTL865X_SOC
// Macro for rx indication
VOID REPORT_ETHERNET_FRAME_TO_LLC_RTL865X(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			p8023hdr,
	IN	PUCHAR			pData,
	IN	ULONG			DataSize,
	IN	struct net_device	*net_dev);
#endif

#ifdef BIG_ENDIAN
static inline VOID	RTMPDescriptorEndianChange(
    IN  PUCHAR          pData,
    IN  ULONG           DescriptorType)
{
	int size = (DescriptorType == TYPE_TXD) ? TXD_SIZE : RXD_SIZE;
	int i;
	for (i=1; i<size/4; i++) {
		/*
		 * Handle IV and EIV with little endian
		 */
		if (DescriptorType == TYPE_TXD) {		
			 /* Skip Word 3 IV and Word 4 EIV of TXD */			
			if (i==3||i==4)  
				continue; 
		}
		else {	
			 /* Skip Word 2 IV and Word 3 EIV of RXD */	
			if (i==2||i==3)  
				continue; 
		}
		*((ULONG  *)(pData + i*4)) = SWAP32(*((ULONG  *)(pData + i*4)));
	}
    *(ULONG  *)pData = SWAP32(*(ULONG  *)pData);
}

static inline VOID	WriteBackToDescriptor(
	IN  PUCHAR			Dest,
 	IN	PUCHAR			Src,
    IN  BOOLEAN			DoEncrypt,
	IN  ULONG           DescriptorType)
{
	PULONG p1, p2;
	UCHAR i;
	int size = (DescriptorType == TYPE_TXD) ? TXD_SIZE : RXD_SIZE;

	p1 = ((PULONG )Dest) + 1;
	p2 = ((PULONG )Src) + 1;
	for (i = 1; i < size/4 ; i++)
		*p1++ = *p2++;
	*(PULONG )Dest = *(PULONG )Src;		// Word 0; this must be written back last
}

/*
	========================================================================

	Routine	Description:
		Endian conversion of all kinds of 802.11 frames .
		
	Arguments:
		pAdapter		Pointer	to our adapter
		pData			Pointer	to the 802.11 frame structure
		Dir				Direction of the frame
		FromRxDoneInt   Caller is from RxDone interrupt
		
	Return Value:
		None
		
	Note:
	    Call this function when read or update buffer data
	========================================================================
*/
static inline VOID	RTMPFrameEndianChange(
	IN  PRTMP_ADAPTER   pAdapter, 
	IN  PUCHAR          pData, 
	IN  ULONG           Dir,
	IN  BOOLEAN         FromRxDoneInt)
{
    PFRAME_802_11	pFrame;
    PUCHAR			pMacHdr;

    // swab 16 bit fields - Frame Control field
    if(Dir == DIR_READ)
    {
        *(USHORT *)pData = SWAP16(*(USHORT *)pData);
    }

    pFrame = (PFRAME_802_11) pData;
    pMacHdr = (PUCHAR) pFrame;

    // swab 16 bit fields - Duration/ID field
    *(USHORT *)(pMacHdr + 2) = SWAP16(*(USHORT *)(pMacHdr + 2));

    // swab 16 bit fields - Sequence Control field
    *(USHORT *)(pMacHdr + 22) = SWAP16(*(USHORT *)(pMacHdr + 22));

    if(pFrame->Hdr.FC.Type == BTYPE_MGMT)
    {
        switch(pFrame->Hdr.FC.SubType)
        {
            case SUBTYPE_ASSOC_REQ:
            case SUBTYPE_REASSOC_REQ:
                // swab 16 bit fields - CapabilityInfo field
                pMacHdr += LENGTH_802_11;
                *(USHORT *)pMacHdr = SWAP16(*(USHORT *)pMacHdr);

                // swab 16 bit fields - Listen Interval field
                pMacHdr += 2;
                *(USHORT *)pMacHdr = SWAP16(*(USHORT *)pMacHdr);
                break;

            case SUBTYPE_ASSOC_RSP:
            case SUBTYPE_REASSOC_RSP:
                // swab 16 bit fields - CapabilityInfo field
                pMacHdr += LENGTH_802_11;
                *(USHORT *)pMacHdr = SWAP16(*(USHORT *)pMacHdr);

                // swab 16 bit fields - Status Code field
                pMacHdr += 2;
                *(USHORT *)pMacHdr = SWAP16(*(USHORT *)pMacHdr);
                
                // swab 16 bit fields - AID field
                pMacHdr += 2;
                *(USHORT *)pMacHdr = SWAP16(*(USHORT *)pMacHdr);
                break;

            case SUBTYPE_AUTH:
                // If from RTMPHandleRxDoneInterrupt routine, it is still a encrypt format.
                // The convertion is delayed to RTMPHandleDecryptionDoneInterrupt.
                if(!FromRxDoneInt && pAdapter->NeedSwapToLittleEndian == TRUE)
                {
                    // swab 16 bit fields - Auth Alg No. field
                    pMacHdr += LENGTH_802_11;
                    *(USHORT *)pMacHdr = SWAP16(*(USHORT *)pMacHdr);

                    // swab 16 bit fields - Auth Seq No. field
                    pMacHdr += 2;
                    *(USHORT *)pMacHdr = SWAP16(*(USHORT *)pMacHdr);

                    // swab 16 bit fields - Status Code field
                    pMacHdr += 2;
                    *(USHORT *)pMacHdr = SWAP16(*(USHORT *)pMacHdr);
            	}
            	break;

            case SUBTYPE_BEACON:
            case SUBTYPE_PROBE_RSP:
            	// swab 16 bit fields - BeaconInterval field
            	pMacHdr += LENGTH_802_11 + TIMESTAMP_LEN;
            	*(USHORT *)pMacHdr = SWAP16(*(USHORT *)pMacHdr);

            	// swab 16 bit fields - CapabilityInfo field
            	pMacHdr += sizeof(USHORT);
            	*(USHORT *)pMacHdr = SWAP16(*(USHORT *)pMacHdr);
            	break;

            case SUBTYPE_DEAUTH:
            case SUBTYPE_DISASSOC:
            	// swab 16 bit fields - Reason code field
            	pMacHdr += LENGTH_802_11;
            	*(USHORT *)pMacHdr = SWAP16(*(USHORT *)pMacHdr);
            	break;
        }
    }
    else if(pFrame->Hdr.FC.Type == BTYPE_DATA)
    {
    }
    else if(pFrame->Hdr.FC.Type == BTYPE_CNTL)
    {
    }
    else
    {
        DBGPRINT(RT_DEBUG_ERROR,"Invalid Frame Type!!!\n");
    }

    // swab 16 bit fields - Frame Control
    if(Dir == DIR_WRITE)
    {
        *(USHORT *)pData = SWAP16(*(USHORT *)pData);
    }
}
#endif	// #ifdef BIG_ENDIAN

/*
 * 	Note:	When THREAD_ISR was defined, caller must disable IRQ before using this
 *		  function.
 */

static inline VOID	RTMPSendRTSFrame(
	IN	PRTMP_ADAPTER	pAd,
	IN	PUCHAR			pDA,
	IN	unsigned int	NextMpduSize,
	IN  UCHAR           TxRate,
	IN  UCHAR           RTSRate,
	IN  USHORT          AckDuration,
	IN  UCHAR           QueIdx,
	IN  UCHAR			FrameGap,
	IN  UCHAR           apidx)
{
	PRTS_FRAME		    pRtsFrame;
	PTXD_STRUC		    pTxD;
#ifdef BIG_ENDIAN
    PTXD_STRUC      	pDestTxD;
    TXD_STRUC       	TxD;
#endif
	PUCHAR              pBuf;
	PRTMP_TX_RING       pTxRing = &pAd->TxRing[QueIdx];
#ifndef THREAD_ISR
	UMAXLONG			IrqFlags;
#endif

	// Make sure Tx ring resource won't be used by other threads
#ifndef THREAD_ISR
	RTMP_SEM_LOCK(&pAd->TxRingLock, IrqFlags);
#else
//	RTMP_IRQ_LOCK(IrqFlags);
#endif

   	pRtsFrame = (PRTS_FRAME) pTxRing->Cell[pTxRing->CurTxIndex].DmaBuf.AllocVa;
   	pBuf = (PUCHAR) pRtsFrame;
#ifndef BIG_ENDIAN
    pTxD  = (PTXD_STRUC) pTxRing->Cell[pTxRing->CurTxIndex].AllocVa;
#else
    pDestTxD = (PTXD_STRUC) pTxRing->Cell[pTxRing->CurTxIndex].AllocVa;
    TxD = *pDestTxD;
    pTxD = &TxD;
    RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
#endif
				
	if ((pTxD->Owner == DESC_OWN_NIC) || (pTxD->bWaitingDmaDoneInt == 1))
	{
		// Descriptor owned by NIC. No descriptor avaliable
		// This should not happen since caller guaranteed.
        DBGPRINT_ERR("RTMPSendRTSFrame: TX RING full\n");
		pAd->RalinkCounters.TxRingErrCount++;
#ifndef THREAD_ISR
		RTMP_SEM_UNLOCK(&pAd->TxRingLock, IrqFlags);
#else
//		RTMP_IRQ_UNLOCK(IrqFlags);
#endif
		return;
	}
		
	NdisZeroMemory(pRtsFrame, sizeof(RTS_FRAME));
	pRtsFrame->FC.Type    = BTYPE_CNTL;
	// CTS-to-self's duration = SIFS + MPDU
	pRtsFrame->Duration = (2 * pAd->PortCfg.Dsifs) + RTMPCalcDuration(pAd, TxRate, NextMpduSize) + AckDuration;// SIFS + Data + SIFS + ACK

//iverson fix
        pTxD->BufCount = 1;
        pTxD->BufLen0 = sizeof(RTS_FRAME);

//iverson
	// Write Tx descriptor
	// Don't kick tx start until all frames are prepared
	// RTS has to set more fragment bit for fragment burst
	// RTS did not encrypt		
	if (ERP_IS_USE_PROTECTION(pAd->Mlme.ErpIeContent))
	{
		DBGPRINT(RT_DEBUG_TRACE,"IF(ra%d) Making CTS-to-self Frame\n", apidx);
		pRtsFrame->FC.SubType = SUBTYPE_CTS;
		COPY_MAC_ADDR(pRtsFrame->Addr1, pAd->PortCfg.MBSSID[apidx].Bssid);
#ifdef BIG_ENDIAN
        RTMPFrameEndianChange(pAd, (PUCHAR)pRtsFrame, DIR_WRITE, FALSE);
        RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
        *pDestTxD = TxD;
        pTxD = pDestTxD;
#endif
#ifdef CONFIG_64BIT
		{
        RTMP_DMACB *dmacb_p = &pTxRing->Cell[pTxRing->CurTxIndex];

		RTMPWriteTxDescriptor(pAd, pTxD, CIPHER_NONE, 0, 0, FALSE, TRUE, FALSE, SHORT_RETRY, FrameGap, RTSRate,
			10, QueIdx, PTYPE_SPECIAL|PSUBTYPE_OTHER_CNTL, NULL, NULL, pAd->PortCfg.MBSSID[apidx].bWmmCapable, FALSE, FALSE, TRUE, dmacb_p);
		}
#else
		RTMPWriteTxDescriptor(pAd, pTxD, CIPHER_NONE, 0, 0, FALSE, TRUE, FALSE, SHORT_RETRY, FrameGap, RTSRate,
			10, QueIdx, PTYPE_SPECIAL|PSUBTYPE_OTHER_CNTL, NULL, NULL, pAd->PortCfg.MBSSID[apidx].bWmmCapable, FALSE, FALSE, TRUE);
#endif // CONFIG_64BIT //
	}
	else
	{
	    if ((*pDA) & 0x01)
		{
            // should not use RTS/CTS to protect MCAST frame since no one will reply CTS
#ifndef THREAD_ISR
	        RTMP_SEM_UNLOCK(&pAd->TxRingLock, IrqFlags);
#else
//			RTMP_IRQ_UNLOCK(IrqFlags);
#endif
	        return;
	    }
        DBGPRINT(RT_DEBUG_TRACE,"IF(ra%d) Making RTS Frame\n", apidx);
		pRtsFrame->FC.SubType = SUBTYPE_RTS;        
	    COPY_MAC_ADDR(pRtsFrame->Addr1, pDA);
	    COPY_MAC_ADDR(pRtsFrame->Addr2, pAd->PortCfg.MBSSID[apidx].Bssid);
        // RTS's duration need to include and extra (SIFS + CTS) time
        pRtsFrame->Duration += (pAd->PortCfg.Dsifs + RTMPCalcDuration(pAd, RTSRate, 14)); // SIFS + CTS-Duration
#ifdef BIG_ENDIAN
        RTMPFrameEndianChange(pAd, (PUCHAR)pRtsFrame, DIR_WRITE, FALSE);
        RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
        *pDestTxD = TxD;
        pTxD = pDestTxD;
#endif
#ifdef CONFIG_64BIT
{
        RTMP_DMACB *dmacb_p = &pTxRing->Cell[pTxRing->CurTxIndex];

		RTMPWriteTxDescriptor(pAd, pTxD, CIPHER_NONE, 0, 0, TRUE, TRUE, FALSE, SHORT_RETRY,
			FrameGap, RTSRate, sizeof(RTS_FRAME), QueIdx, PTYPE_SPECIAL|PSUBTYPE_RTS,
			NULL, NULL, pAd->PortCfg.MBSSID[apidx].bWmmCapable, FALSE, FALSE, FALSE, dmacb_p);
}
#else
		RTMPWriteTxDescriptor(pAd, pTxD, CIPHER_NONE, 0, 0, TRUE, TRUE, FALSE, SHORT_RETRY,
			FrameGap, RTSRate, sizeof(RTS_FRAME), QueIdx, PTYPE_SPECIAL|PSUBTYPE_RTS,
			NULL, NULL, pAd->PortCfg.MBSSID[apidx].bWmmCapable, FALSE, FALSE, FALSE);
#endif // CONFIG_64BIT //
	}
		
	INC_RING_INDEX(pAd->TxRing[QueIdx].CurTxIndex, TX_RING_SIZE);
	pAd->RalinkCounters.KickTxCount++;

    // leave the KICK action until the protected MPDU is ready
#ifndef THREAD_ISR    
    RTMP_SEM_UNLOCK(&pAd->TxRingLock, IrqFlags);
#else
//	RTMP_IRQ_UNLOCK(IrqFlags);
#endif
}

/*
  ========================================================================
  Description:
    This routine checks if a received frame causes class 2 or class 3
    error, and perform error action (DEAUTH or DISASSOC) accordingly
  ========================================================================
*/
static inline BOOLEAN CheckClass2Class3Error(
    IN  PRTMP_ADAPTER   pAd,
    IN  PHEADER_802_11  pHeader,
    IN  MAC_TABLE_ENTRY *pEntry)
{
    SST Sst;
                                             
    if (pEntry)
    	Sst = pEntry->Sst;
    else
    	Sst = SST_NOT_AUTH;
    
    if (Sst == SST_ASSOC)
        ; // okay to receive this DATA frame
    else if (Sst == SST_AUTH)
    {
        Cls3errAction(pAd, pHeader->Addr1, pHeader->Addr2);
        return TRUE; 
    }
    else
    {
        Cls2errAction(pAd, pHeader->Addr1, pHeader->Addr2);
        return TRUE; 
    }
    return FALSE;
}

static inline VOID RTMPHandleTBTTInterrupt(
    IN PRTMP_ADAPTER pAd)
{
#if 0
	TXRX_CSR9_STRUC csr9;
#endif
    DBGPRINT(RT_DEBUG_INFO, "RTMPHandleTBTTInterrupt...\n");

	if ((pAd->PortCfg.PhyMode == PHY_11A) && (pAd->PortCfg.RadarDetect.IEEE80211H == TRUE) && (pAd->PortCfg.RadarDetect.RDMode == RD_SWITCHING_MODE))
	{
		if (pAd->PortCfg.RadarDetect.CSCount >= pAd->PortCfg.RadarDetect.CSPeriod)
		{
			// JStream request AP Disassociate to All STAs behinded.
			// Other RDMode except RD_NORMAL_MODE will block all TX path.
			// So set RDMode be RD_NORMAL_NODE here for Deassociation sending.
			pAd->PortCfg.RadarDetect.RDMode = RD_NORMAL_MODE;

			ApStop(pAd, TRUE);
			ApStartUp(pAd, TRUE);
		}
		pAd->PortCfg.RadarDetect.CSCount++;
		DBGPRINT(RT_DEBUG_TRACE, "RTMPHandleTBTTInterrupt::Channel Switching...(%d/%d)\n", pAd->PortCfg.RadarDetect.CSCount, pAd->PortCfg.RadarDetect.CSPeriod);
	}
	
#if 0
	// Fix WiFi WPA2 4.4.2 because beacon time is not accurate
	// reset the beacon time stamp every 10 beacons
	if (!((pAd->Mlme.TbttCount++) % 10))
	{
		RTMP_IO_READ32(pAd, TXRX_CSR9, &csr9.word);
		csr9.field.bTsfTicking = 0;
		RTMP_IO_WRITE32(pAd, TXRX_CSR9, csr9.word);
		csr9.field.bTsfTicking = 1;
		RTMP_IO_WRITE32(pAd, TXRX_CSR9, csr9.word);
	}
#endif
		
	if (pAd->Mlme.bBeaconUpdate == 0)
	{
		pAd->Mlme.bBeaconUpdate = 1;
		RTMPAddTimer(&pAd->Mlme.BeaconUpdateTimer, ((pAd->PortCfg.BeaconPeriod>>1)*(1*HZ))/1000);
	}
//   	UpdateAllBeaconFrame(pAd);
}

static inline VOID RTMPHandleRxPsPoll(
    IN  PRTMP_ADAPTER   pAd,
    IN  PUCHAR          pAddr,
    IN  MAC_TABLE_ENTRY *pMacEntry,
    IN	BOOLEAN			isActive)
{
	PQUEUE_ENTRY	pEntry;
	USHORT			Aid;
	BOOLEAN			bMoreData  = FALSE;
	UMAXLONG		IrqFlags;
#ifndef THREAD_ISR
	UMAXLONG		IrqFlags2;
#endif
    
	if (!pMacEntry)
		return;
    
    DBGPRINT(RT_DEBUG_TRACE,"(%d):rcv PS-POLL(%d) from %02x:%02x:%02x:%02x:%02x:%02x\n", 
		isActive, pMacEntry->PsQueue.Number,
        pAddr[0], pAddr[1], pAddr[2], pAddr[3], pAddr[4], pAddr[5]);

#ifndef THREAD_ISR
    RTMP_SEM_LOCK(&pAd->MacTabLock, IrqFlags);
#else
	/* 
	 *  Protect TxSwQueue0 & McastPsQueue 
	 *  because use them in interrupt context 
	 */		
	RTMP_IRQ_LOCK(IrqFlags);
#endif	/* THREAD_ISR */

    if (pMacEntry)
    {
        Aid = pMacEntry->Aid;
#ifndef THREAD_ISR
        RTMP_SEM_LOCK(&pAd->TxSwQueueLock, IrqFlags2);
#endif

        // cleanup all backlogged frames in PSQ

        if ((isActive == FALSE)) // && (((pAddr[0] == 0x00) && (pAddr[1]==0x30) && (pAddr[2]== 0xb4)) || ((pAddr[0] == 0x00) && (pAddr[1] == 0x10) && (pAddr[2] == 0x18))))
		{
			if (pMacEntry->PsQueue.Head)
			{
				if (pAd->TxSwQueue[QID_AC_BE].Number <= (pAd->PortCfg.TxQueueSize + (MAX_PACKETS_IN_PS_QUEUE>>1))) {	
					pEntry = RemoveHeadQueue(&pMacEntry->PsQueue);
					if (pMacEntry->PsQueue.Number != 0)
					RTMP_SET_PACKET_MOREDATA((struct sk_buff *) pEntry, TRUE);
					InsertTailQueue(&pAd->TxSwQueue[QID_AC_BE], pEntry);
				}
			}
		}
		else
		{
			while (pMacEntry->PsQueue.Head)
			{
				if (pAd->TxSwQueue[QID_AC_BE].Number <= (pAd->PortCfg.TxQueueSize + (MAX_PACKETS_IN_PS_QUEUE>>1))) {	
					pEntry = RemoveHeadQueue(&pMacEntry->PsQueue);
					InsertTailQueue(&pAd->TxSwQueue[QID_AC_BE],pEntry);
				} else {
					break;
				}
			}
		}
        
#ifndef THREAD_ISR
        RTMP_SEM_UNLOCK(&pAd->TxSwQueueLock, IrqFlags2);
#endif

        if ((Aid > 0) && (Aid < MAX_LEN_OF_MAC_TABLE) && (pMacEntry->PsQueue.Number == 0))
        {
            // clear corresponding TIM bit
            if (Aid >= 32)
                pAd->PortCfg.MBSSID[pMacEntry->ApIdx].TimBitmap2 &= (~BIT32[Aid-32]);  
            else
                pAd->PortCfg.MBSSID[pMacEntry->ApIdx].TimBitmap &= (~BIT32[Aid]);  
            pMacEntry->PsQIdleCount = 0;
        }
#ifndef THREAD_ISR      
        RTMP_SEM_UNLOCK(&pAd->MacTabLock, IrqFlags);
#else
		RTMP_IRQ_UNLOCK(IrqFlags);
#endif	/* THREAD_ISR */

        // Dequeue outgoing frames from TxSwQueue0..3 queue and process it
        // TODO: 2004-12-27 it's not a good idea to handle "More Data" bit here. because the
        // RTMPDeQueue process doesn't guarantee to de-queue the desired MSDU from the corresponding
        // TxSwQueue/PsQueue when QOS in-used. We should consider "HardTransmt" this MPDU
        // using MGMT queue or things like that.
        RTMPDeQueuePacket(pAd, bMoreData, FALSE);

    }
}

static inline VOID RTMPHandleRxPsMcast(
    IN  PRTMP_ADAPTER   pAd,
    IN  MAC_TABLE_ENTRY *pMacEntry)
{
	USHORT		Aid;
	UCHAR		apidx;
	
	Aid = pMacEntry->Aid;
	apidx = pMacEntry->ApIdx;
	
	// mark corresponding bit to record PS status    
    if ((Aid > 0) && (Aid < MAX_LEN_OF_MAC_TABLE))
	{
		if (pMacEntry->PsMode == PWR_SAVE)
		{
			if (Aid >= 32)
	            pAd->MacTab.PsmBitmap2 |= BIT32[Aid-32];
	        else
	            pAd->MacTab.PsmBitmap |= BIT32[Aid];
	        DBGPRINT(RT_DEBUG_INFO, "Record (AID=%d) as PS, bitmap=%02x-%02x\n", Aid, pAd->MacTab.PsmBitmap, pAd->MacTab.PsmBitmap2);
		}
		else
		{
			// clear corresponding bit
			if (Aid >= 32)
	            pAd->MacTab.PsmBitmap2 &= (~BIT32[Aid-32]); 
		    else
		        pAd->MacTab.PsmBitmap &= (~BIT32[Aid]);
	    	DBGPRINT(RT_DEBUG_INFO, "Record (AID=%d) as Active, bitmap=%02x-%02x\n", Aid, pAd->MacTab.PsmBitmap, pAd->MacTab.PsmBitmap2);
		}
    }
	        
	// check if there's any associated STA in power-save mode. this affects outgoing
    // MCAST/BCAST frames should be stored in PSQ till DtimCount=0
    if (pAd->MacTab.PsmBitmap == 0 && pAd->MacTab.PsmBitmap2 == 0)
        pAd->MacTab.fAnyStationInPsm = FALSE;
	else 
		pAd->MacTab.fAnyStationInPsm = TRUE;
	DBGPRINT(RT_DEBUG_INFO, "fAnyStationInPsm = %d\n", pAd->MacTab.fAnyStationInPsm);
	
}

/*
    ==========================================================================
    Description:
        Update the station current power save mode. Calling this routine also
        prove the specified client is still alive. Otherwise AP will age-out
        this client once IdleCount exceeds a threshold.
    ==========================================================================
 */
static inline UCHAR PsIndicate(
    IN PRTMP_ADAPTER pAd, 
    IN PUCHAR pAddr, 
    IN UCHAR Rssi,
    IN UCHAR Psm) 
{
    MAC_TABLE_ENTRY *pEntry;
    UCHAR			oldPsMode = PWR_ACTIVE;

    pEntry = MacTableLookup(pAd, pAddr);
    if (pEntry) 
    {
    	oldPsMode = pEntry->PsMode;
    	
        if ((pEntry->PsMode == PWR_SAVE) && (Psm == PWR_ACTIVE))
        {
            DBGPRINT(RT_DEBUG_TRACE, "PsIndicate - %02x:%02x:%02x:%02x:%02x:%02x wakes up, act like rx PS-POLL\n", pAddr[0],pAddr[1],pAddr[2],pAddr[3],pAddr[4],pAddr[5]);
            // sleep station awakes, move all pending frames from PSQ to TXQ if any
            RTMPHandleRxPsPoll(pAd, pAddr, pEntry, TRUE);
        }
        else if ((pEntry->PsMode != PWR_SAVE) && (Psm == PWR_SAVE))
        {
            DBGPRINT(RT_DEBUG_TRACE, "PsIndicate - %02x:%02x:%02x:%02x:%02x:%02x sleeps\n", pAddr[0],pAddr[1],pAddr[2],pAddr[3],pAddr[4],pAddr[5]);
        }
        
        pEntry->NoDataIdleCount = 0;
        pEntry->PsMode = Psm;
        pEntry->LastRssi = Rssi;

		// Check whether there is any station in PS mode	
		RTMPHandleRxPsMcast(pAd, pEntry);
		
    } 
    else 
    {
        // not in table, try to learn it ???? why bother?
    }
    return oldPsMode;
}

/* Get Config API */
VOID getCfgCountryRegion(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgCountryRegionABand(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgCountryCode(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgSSID(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgAP1WirelessMode(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgTxRate(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgChannel(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgBasicRate(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgBeaconPeriod(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgDtimPeriod(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgTxPower(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgDisableOLBC(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgBGProtection(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgTxAntenna(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgTxPreamble(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgRTSThreshold(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgFragThreshold(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgTxBurst(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgPktAggregate(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgTurboRate(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgWmmCapable(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgAPAifsn(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgAPCwmin(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgAPCwmax(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgAPTxop(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgAPACM(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgBSSAifsn(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgBSSCwmin(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgBSSCwmax(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgBSSTxop(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgBSSACM(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgAckPolicy(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgNoForwarding(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgNoForwardingBTNBSSID(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgHideSSID(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgShortSlot(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgAutoChannelSelect(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgIEEE8021X(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgIEEE80211H(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgCSPeriod(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgPreAuth(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgAuthMode(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgEncrypType(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgRekeyInterval(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgRekeyMethod(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgPMKCachePeriod(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgWPAPSK(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgDefaultKeyID(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgKey1Type(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgKey2Type(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgKey3Type(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgKey4Type(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgKey1Str(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgKey2Str(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgKey3Str(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgKey4Str(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
VOID getCfgHSCounter(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput);
BOOLEAN getCfgDataForSTA(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pDataBuf);
/* Get Config API */

#ifdef APCLI_SUPPORT
PUCHAR UWRMacUpdaterForTxPkt(
	IN PRTMP_ADAPTER	pAd,
	IN struct sk_buff	*pSkb,
	IN PTXD_STRUC		pTxD);

PUCHAR UWRMacConverterForRxPkt(
	IN  PRTMP_ADAPTER		pAd,
	IN	struct sk_buff 		*pSkb,
	IN  struct net_device	*net_dev);

NDIS_STATUS APCLIConvertEngineExit(VOID);
NDIS_STATUS APCLIConvertEngineInit(VOID);
#endif	//endif of APCLI_SUPPORT

typedef struct CountryCodeToCountryRegion {
	ULONG		countryNum;
    UCHAR   	isoName[3];
    UCHAR   	CountryName[32];
	BOOLEAN		support11a;
    ULONG		regDomainNum11a;
    BOOLEAN  	support11g;
    ULONG		regDomainNum11g;
} COUNTRY_CODE_TO_COUNTRY_REGION;

VOID SetNullFrameAID(
	IN struct sk_buff *pSkb,
	IN UCHAR PID);

UCHAR GetNullFramePID(
	IN struct sk_buff *pSkb);

#ifdef CARRIER_DETECTION_SUPPORT
static inline INT isCarrierDetectExist(
	IN PRTMP_ADAPTER pAd)
{
	if (pAd->PortCfg.CarrierDetect.CD_State != CD_NORMAL)
		return TRUE;
	else
		return FALSE;
}		

INT CarrierDetectReset(
	IN PRTMP_ADAPTER pAd);

VOID CarrierDetectionFsm(
	IN PRTMP_ADAPTER pAd,
	IN UINT32  CurFalseCCA);

static inline VOID CarrierDetectionStart(
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

static inline VOID CarrierDetectionStop(
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

VOID CarrierDetectionCheck(
	IN PRTMP_ADAPTER pAd);
#endif // CARRIER_DETECTION_SUPPORT //

#ifndef RTL865X_SOC
VOID BeaconUpdateExec(
	IN	unsigned long data);
#endif

#ifdef WSC_SUPPORT
//
// prototype in wsc.c
//
BOOLEAN	WscMsgTypeSubst(
	IN	UCHAR	EAPType,
	IN	UCHAR	EAPCode,
	OUT	ULONG	*MsgType);

VOID    WscStateMachineInit(
	IN	PRTMP_ADAPTER		pAd, 
	IN	STATE_MACHINE		*S, 
	OUT STATE_MACHINE_FUNC Trans[]);

VOID    WscEAPOLStartAction(
    IN  PRTMP_ADAPTER    pAd, 
    IN  MLME_QUEUE_ELEM  *Elem);

VOID    WscEAPAction(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	MLME_QUEUE_ELEM *Elem);

// For AP Client support WPS Modification
//VOID WscEapUnConfiguredApAction(
VOID WscEapEnrolleeAction(
// --
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	MLME_QUEUE_ELEM	*Elem,
	IN  UCHAR	        MsgType,
	IN  MAC_TABLE_ENTRY *pEntry,
// For AP Client support WPS Modification
	IN  PWSC_CTRL       pWscControl,
	BOOLEAN         bFromApCli);
// --

VOID    WscEapApProxyAction(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	MLME_QUEUE_ELEM	*Elem,
	IN  UCHAR	        MsgType,
	IN  MAC_TABLE_ENTRY *pEntry);

VOID    WscEapApRegistrarAction(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	MLME_QUEUE_ELEM	*Elem,
	IN  UCHAR	        MsgType,
	IN  MAC_TABLE_ENTRY *pEntry);
	
VOID    WscEnqueueEapolStart(
    IN  unsigned long data);
    
VOID    WscEAPOLTimeOutAction(
    IN  unsigned long data);

VOID    Wsc2MinsTimeOutAction(
    IN  unsigned long data);

UCHAR	WscRxMsgType(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PMLME_QUEUE_ELEM	pElem);

VOID	WscInitRegistrarPair(
// For AP Client support WPS Modification
//	IN	PRTMP_ADAPTER		pAdapter);
	IN	PRTMP_ADAPTER		pAdapter,
	IN	BOOLEAN         bFromApCli,
	IN	PWSC_CTRL           pWscControl);
// --

VOID    WscInitEntryFunc(
    IN  PMAC_TABLE_ENTRY pEntry);

BOOLEAN WscCheckWSCHeader(
    IN	PRTMP_ADAPTER		pAdapter,
    IN  PUCHAR              pData);

VOID	WscSendEapReqId(
	IN	PRTMP_ADAPTER		pAdapter, 
	IN	PMAC_TABLE_ENTRY	pEntry);

// For AP Client support WPS Modification
#ifdef APCLI_SUPPORT
VOID    WscSendEapolStart(
	IN	PRTMP_ADAPTER	pAdapter,
	IN  PUCHAR          pBssid,
	IN  BOOLEAN         bFromApCli);

VOID	WscSendEapRspId(
	IN	PRTMP_ADAPTER		pAdapter,
	IN  PWSC_CTRL           pWscControl);

INT	Set_ApCli_AuthMode_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg);

INT	Set_ApCli_EncrypType_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg);
// --
#endif // APCLI_SUPPORT //

VOID	WscMacHeaderInit(
	IN	PRTMP_ADAPTER	pAd, 
	IN OUT	PHEADER_802_11	Hdr, 
	IN	PUCHAR 			pAddr1,
// For AP Client support WPS Modification
//	IN  PUCHAR          pBSSID);
	IN  PUCHAR          pBSSID,
	IN      BOOLEAN         bFromApCli);
// --

VOID	WscSendMessage(
	IN	PRTMP_ADAPTER		pAdapter, 
	IN  UCHAR               OpCode,
	IN  PUCHAR				pData,
	IN  INT					Len,
// For AP Client support WPS Modification
	IN  PWSC_CTRL           pWscControl,
	IN	BOOLEAN         bFromApcli);
// --

VOID	WscSendEapReqAck(
	IN	PRTMP_ADAPTER		pAdapter, 
	IN	PMAC_TABLE_ENTRY	pEntry);
	
VOID	WscSendEapReqDone(
	IN	PRTMP_ADAPTER		pAdapter, 
	IN	PMLME_QUEUE_ELEM	pElem);

VOID	WscSendEapFail(
	IN	PRTMP_ADAPTER		pAdapter);


VOID WscUPnPM2DTimeOutAction(
    IN unsigned long data);

VOID WscUPnPMsgTimeOutAction(
	IN unsigned long data);

int WscSendUPnPConfReqMsg(
	IN PRTMP_ADAPTER pAd,
	IN PUCHAR ssidStr,
	IN PUCHAR macAddr,
	IN INT	  Status,
	IN ULONG eventID);

	
int WscSendUPnPMessage(
	IN	PRTMP_ADAPTER		pAd,
	IN  USHORT				msgType,
	IN  USHORT				msgSubType,
	IN  PUCHAR				pData,
	IN  INT					dataLen,
	IN	ULONG				eventID,
	IN	ULONG				toIPAddr);

VOID WscUPnPErrHandle(
	IN PRTMP_ADAPTER 	pAd,
	IN ULONG eventID,
// For AP Client support WPS Modification
	IN  PWSC_CTRL       pWscControl);
// --


VOID    WscBuildBeaconIE(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	UCHAR b_configured,
	IN	BOOLEAN b_selRegistrar,
	IN	USHORT devPwdId,
	IN	USHORT selRegCfgMethods);

VOID    WscBuildProbeRespIE(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	UCHAR respType,
	IN	UCHAR scState,
	IN	BOOLEAN b_selRegistrar,
	IN	USHORT devPwdId,
	IN	USHORT selRegCfgMethods);

VOID	WscSelectedRegistrar(
	IN	PRTMP_ADAPTER	pAd,
	IN	PUCHAR	RegInfo,
	IN	UINT	length);

VOID    WscInformFromWPA(
    IN  PMAC_TABLE_ENTRY    pEntry);

INT 
ComputeChecksum(
	IN UINT PIN);

#ifdef APCLI_SUPPORT
// For AP Client support WPS Modification
VOID WscMakeProbeReqIE(
	IN	PRTMP_ADAPTER	pAd,
	OUT	PUCHAR			pOutBuf,
	OUT	PUCHAR			pIeLen);
// --
#endif // APCLI_SUPPORT //

VOID WscStop(
// For AP Client support WPS Modification
//	IN	PRTMP_ADAPTER	pAd);
	IN	PRTMP_ADAPTER	pAd,
	IN  PWSC_CTRL       pWscControl,
	IN  BOOLEAN         bFromApCli);
// --

VOID WscInit(
// For AP Client support WPS Modification
//	IN	PRTMP_ADAPTER	pAd);
	IN	PRTMP_ADAPTER	pAd,
	IN	BOOLEAN         bFromApCli,
	IN	PWSC_CTRL       pWscControl);
// --

BOOLEAN
ValidateChecksum(
	IN UINT PIN);

UINT
WscGeneratePinCode(
	IN	PRTMP_ADAPTER	pAd,
// For AP Client support WPS Modification
	IN	BOOLEAN         bFromApcli,
// --
	IN	UCHAR	apidx);

// For Re-Generate Pin-Code Modification
UINT WscRandomGeneratePinCode(
	IN	PRTMP_ADAPTER	pAd,
	IN	UCHAR	apidx);
// --

int BuildMessageM1(
	IN	PRTMP_ADAPTER		pAdapter,
// For AP Client support WPS Modification
	IN  PWSC_CTRL           pWscControl,
// --
	OUT	VOID *pbuf);

int BuildMessageM2(
	IN	PRTMP_ADAPTER		pAdapter,
// For AP Client support WPS Modification
	IN  PWSC_CTRL           pWscControl,
// --
	OUT	VOID *pbuf);

int BuildMessageM2D(
	IN	PRTMP_ADAPTER		pAdapter,
// For AP Client support WPS Modification
	IN  PWSC_CTRL           pWscControl,
// --
	OUT	VOID *pbuf);

int BuildMessageM3(
	IN	PRTMP_ADAPTER		pAdapter,
// For AP Client support WPS Modification
	IN  PWSC_CTRL           pWscControl,
// --
	OUT	VOID *pbuf);

int BuildMessageM4(
	IN	PRTMP_ADAPTER		pAdapter,
// For AP Client support WPS Modification
	IN  PWSC_CTRL           pWscControl,
// --
	OUT	VOID *pbuf);

int BuildMessageM5(
	IN	PRTMP_ADAPTER		pAdapter,
// For AP Client support WPS Modification
	IN  PWSC_CTRL           pWscControl,
// --
	OUT	VOID *pbuf);

int BuildMessageM6(
	IN	PRTMP_ADAPTER		pAdapter,
// For AP Client support WPS Modification
	IN  PWSC_CTRL           pWscControl,
// --
	OUT	VOID *pbuf);

int BuildMessageM7(
	IN	PRTMP_ADAPTER		pAdapter,
// For AP Client support WPS Modification
	IN  PWSC_CTRL           pWscControl,
// --
	OUT	VOID *pbuf);

int BuildMessageM8(
	IN	PRTMP_ADAPTER		pAdapter,
// For AP Client support WPS Modification
	IN  PWSC_CTRL           pWscControl,
// --
	OUT	VOID *pbuf);

int BuildMessageDONE(
	IN	PRTMP_ADAPTER		pAdapter,
// For AP Client support WPS Modification
	IN  PWSC_CTRL           pWscControl,
// --
	OUT	VOID *pbuf);

int BuildMessageACK(
	IN	PRTMP_ADAPTER		pAdapter,
// For AP Client support WPS Modification
	IN  PWSC_CTRL           pWscControl,
// --
	OUT	VOID *pbuf);

int BuildMessageNACK(
	IN	PRTMP_ADAPTER		pAdapter,
// For AP Client support WPS Modification
	IN  PWSC_CTRL           pWscControl,
// --
	OUT	VOID *pbuf);

int ProcessMessageM1(
	IN	PRTMP_ADAPTER		pAdapter,
	IN	VOID *precv,
	IN	INT Length,
	OUT	PWSC_REG_DATA pReg);

int ProcessMessageM2(
	IN	PRTMP_ADAPTER		pAdapter,
	IN	VOID *precv,
	IN	INT Length,
	OUT	PWSC_REG_DATA pReg);

int ProcessMessageM2D(
	IN	PRTMP_ADAPTER		pAdapter,
	IN	VOID *precv,
	IN	INT Length,
	OUT	PWSC_REG_DATA pReg);

int ProcessMessageM3(
	IN	PRTMP_ADAPTER		pAdapter,
	IN	VOID *precv,
	IN	INT Length,
	OUT	PWSC_REG_DATA pReg);

int ProcessMessageM4(
	IN	PRTMP_ADAPTER		pAdapter,
// For AP Client support WPS Modification
	IN  PWSC_CTRL           pWscControl,
// --
	IN	VOID *precv,
	IN	INT Length,
	OUT	PWSC_REG_DATA pReg);

int ProcessMessageM5(
	IN	PRTMP_ADAPTER		pAdapter,
// For AP Client support WPS Modification
	IN  PWSC_CTRL           pWscControl,
// --
	IN	VOID *precv,
	IN	INT Length,
	OUT	PWSC_REG_DATA pReg);

int ProcessMessageM6(
	IN	PRTMP_ADAPTER		pAdapter,
// For AP Client support WPS Modification
	IN  PWSC_CTRL           pWscControl,
// --
	IN	VOID *precv,
	IN	INT Length,
	OUT	PWSC_REG_DATA pReg);

int ProcessMessageM7(
	IN	PRTMP_ADAPTER		pAdapter,
// For AP Client support WPS Modification
	IN  PWSC_CTRL           pWscControl,
// --
	IN	VOID *precv,
	IN	INT Length,
	OUT	PWSC_REG_DATA pReg);

int ProcessMessageM8(
	IN	PRTMP_ADAPTER		pAdapter,
// For AP Client support WPS Modification
	IN  PWSC_CTRL           pWscControl,
// --
	IN	VOID *precv,
	IN	INT Length,
	OUT	PWSC_REG_DATA pReg);

USHORT  WscGetAuthType(
    IN  NDIS_802_11_AUTHENTICATION_MODE authType);

USHORT  WscGetEncryType(
    IN  NDIS_802_11_WEP_STATUS encryType);

void    WscWriteConfToDatFile(
    IN  PRTMP_ADAPTER pAd);

void    start_write_dat_file_thread(
    IN  PRTMP_ADAPTER pAd);

int     AppendWSCTLV(
    IN  USHORT index, 
    OUT UCHAR * obuf, 
    IN  UCHAR * ibuf, 
    IN  u16 varlen);

VOID    WscGetRegDataPIN(
    IN  PRTMP_ADAPTER   pAdapter,
    IN  UINT            PinCode,
// For AP Client support WPS Modification
    IN  PWSC_CTRL       pWscControl);
// --

// <<WCN vista logo>> ++, add by johnli
INT WscGenerateUUID(
	RTMP_ADAPTER *pAd, 
	UCHAR *uuidHexStr, 
	UCHAR *uuidAscStr, 
	int apIdx);
// <<WCN vista logo>> --

#ifdef DBG
extern BOOLEAN     g_b_test_with_intel_sdk;
#endif
#endif // WSC_SUPPORT //

#ifdef TC_SOC // Add polling mode support
VOID  PeriodicPollingModeDetect(
    IN  unsigned long data);

VOID PollingModeIsr(struct work_struct *work);
#endif

#endif  // __RTMP_H__

