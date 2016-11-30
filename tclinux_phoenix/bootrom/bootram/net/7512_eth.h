



#include "skbuff.h"

/* ----- PHY-Chip Definition ----- */
#define PHY_WRITE 				0x0400
#define PHY_BUSY 				0x0800

#define PHY_CONTROL_REG 		0		/* MII Control Register */
#define PHY_RESET				0x8000
#define PHY_POWER_DOWN			0x0800
#define PHY_AN_ENABLE 			0x1000
#define PHY_RESTART_AN 			0x200
#define PHY_SPEED_SELECT_100 	0x2000
#define PHY_FULL_DUPLEX			0x100
#define PHY_ISOLATE 			0x400
#define PHY_LOOPBACK 			0x4000

#define PHY_STATUS_REG 			1		/* MII Status Register */
#define PHY_AN_COMPLETE			0x0020
#define PHY_LINK_STATUS			0x0004

#define PHY_ID_REG1 			2		/* Identifier register 1 */
#define PHY_ID_REG2 			3		/* Identifier register 2 */

#define PHY_REMOTE_CAP_REG 		5		/* Auto-Negotiation Link Partner Ability Reg */
#define PHY_100_BASE_T4			0x200
#define PHY_100_BASE_TX_FD 		0x100
#define PHY_100_BASE_TX_HD 		0x80
#define PHY_10_BASE_T_FD 		0x40
#define PHY_10_BASE_T_HD 		0x20

#define PHY_CONFIG_REG			16
#define PHY_TP_LOOPBACK			0x0100

/* ----- 100/10Mbps Mode ----- */
//#define AUTO_SENSE		  			0
//#define TEN_MBPS             		100000
//#define ONE_HUNDRED_MBPS    		1000000
#define MIIDR_RESTART_AUTO_NEGOTIATE	0x0200
#define MIIDR_ENABLE_AUTO_NEGOTIATE		0x1000
#define MIIDR_10MBps					0x0000
#define MIIDR_100MBps					0x2000
#define MIIDR_AUTO_NEGOTIATE			( MIIDR_RESTART_AUTO_NEGOTIATE| MIIDR_ENABLE_AUTO_NEGOTIATE )
#define MIIDR_FULL_DUPLEX				0x0100
#define MIIDR_HALF_DUPLEX				0x0000
#define MIIAR_WRITE						0x2
#define MIIAR_READ						0x0
#define MIIAR_IS_BUSY					0x1
#define	LINK_SPEED						AUTO_SENSE

#define CONFIG_QDMA_CHANNEL					8
#define CONFIG_QDMA_QUEUE					8
#define CONFIG_HWFWD_DSCP_NUM				(4) //shoule not more than 4096
#define CONFIG_HWFWD_DSCP_SIZE				(16) //shoule not more than 4096
#define CONFIG_MAX_PKT_LENS					(2048)

#define CONFIG_DSCP_NUM_MAX					(4096)
#define CONFIG_IRQ_NUM_MAX					(4095)
#define CONFIG_TX0_DSCP_NUM					(16)
#define CONFIG_RX0_DSCP_NUM					(16)
#define CONFIG_TX0_DSCP_SIZE				(32) //4 //4desc + 4msg
#define CONFIG_RX0_DSCP_SIZE				(32) //4desc + 4msg

#define CONFIG_IRQ_DEPTH					(32)
#define CONFIG_MAX_PKT_LENS					(2048)
#define HWFWD_LOW_THRESHOLD					(1)
#define QDMA_RX_DSCP_MSG_LENS			(16)
#define QDMA_TX_DSCP_MSG_LENS			(8)

#define QDMA_TX_THRESHOLD				(4)
#define DESC_INFONODE_SIZE  			(16)
#define DESC_INFO_SIZE 					((CONFIG_TX0_DSCP_NUM + CONFIG_RX0_DSCP_NUM)*DESC_INFONODE_SIZE)
#define DESC_TOTAL_SIZE 				(CONFIG_TX0_DSCP_NUM*CONFIG_TX0_DSCP_SIZE + CONFIG_RX0_DSCP_NUM*CONFIG_RX0_DSCP_SIZE)
#define HWFWD_TOTAL_SIZE 				(CONFIG_MAX_PKT_LENS + CONFIG_HWFWD_DSCP_SIZE)* CONFIG_HWFWD_DSCP_NUM
#define MEM_POOL_SIZE 					(DESC_TOTAL_SIZE + (CONFIG_IRQ_DEPTH<<2) + HWFWD_TOTAL_SIZE)
#define RING_IDX_0 0


#define QDMA_ENABLE 1
#define QDMA_DISABLE 0
#define CONFIG_IRQ_DEF_VALUE			(0xFFFFFFFF)


#define read_reg_word(reg) 				VPint(reg)
#define write_reg_word(reg, wdata) 		VPint(reg)=wdata
#define printf prom_printf
#define wmb() 							__asm__ __volatile("sync");
#define K0_TO_K1(x)						(((uint32)x) | 0xa0000000)
#define K1_TO_PHY(x)					(((uint32)x) & 0x1fffffff)


#define IO_GREG(reg)							read_reg_word((reg))
#define IO_SREG(reg, value)						write_reg_word((reg), value)
#define IO_GMASK(reg, mask, shift)				((read_reg_word((reg)) & mask) >> shift)
#define IO_SMASK(reg, mask, shift, value)		{ uint t = read_reg_word((reg)); write_reg_word((reg), ((t&~(mask))|((value<<shift)&mask))); }
#define IO_SBITS(reg, bit)						{ uint t = read_reg_word((reg)); write_reg_word((reg), (t|bit)); }


#ifdef CONFIG_DEBUG
	
	#define QDMA_MSG(level, F, B...)	{ \
												if(gpQdmaPriv->dbgLevel >= level)	\
													printf("%s [%d]: " F, strrchr(__FILE__, '/')+1, __LINE__, ##B) ; \
											}
	#define QDMA_ERR(F, B...)			printf("%s [%d]: " F, strrchr(__FILE__, '/')+1, __LINE__, ##B)
	#define QDMA_LOG(F, B...)			printf("%s [%d]: " F, strrchr(__FILE__, '/')+1, __LINE__, ##B)
#else
	#define QDMA_MSG(level, F, B...)			
	#define QDMA_ERR(F,B...)			printf(F, ##B) ; 
	#define QDMA_LOG(F,B...)			printf(F, ##B) ; 
#endif

typedef struct {
	uint 	pkt_addr ;
	struct {
#ifdef __BIG_ENDIAN
		uint ctx				: 1 ;
		uint resv				: 2 ;
		uint ctx_ring			: 1 ;
		uint ctx_idx			: 12 ;
		uint pkt_len			: 16 ;
#else
		uint pkt_len			: 16 ;
		uint ctx_idx			: 12 ;
		uint ctx_ring			: 1 ;
		uint resv				: 2 ;
		uint ctx				: 1 ;
#endif /* __BIG_ENDIAN */
	} ctrl ;
	uint msg[2] ;
} QDMA_HWFWD_DMA_DSCP_T ;



typedef struct {
	ushort	IrqQueueAsynchronous ;
	ushort	txIrqQueueIdxErrs ;
	uint	txCounts ;
	uint	txRecycleCounts ;
	uint	txQdmaDropCounts ;
	uint	rxCounts ;
	ushort	txDscpIncorrect ;
	ushort	rxDscpIncorrect ;
	ushort	rxPktErrs ;
	ushort	noTxDscps ;
	ushort	noRxDscps ;
} BM_Counters_T ;

	

typedef union {
	struct {
#ifdef __BIG_ENDIAN
		uint32 resv1		: 1;
		uint32 tsid 		: 5;
		uint32 tse			: 1;
		uint32 dei			: 1;
		uint32 resv2		: 12;
		uint32 oam			: 1;
		uint32 channel		: 8;
		uint32 queue		: 3;
#else
		uint32 queue		: 3;
		uint32 channel		: 8;
		uint32 oam			: 1;
		uint32 resv2		: 12;
		uint32 dei			: 1;
		uint32 tse			: 1;
		uint32 tsid 		: 5;
		uint32 resv1		: 1;
#endif /* __BIG_ENDIAN */
		
#ifdef __BIG_ENDIAN
		uint32 ico			: 1;
		uint32 uco			: 1;
		uint32 tco			: 1;
		uint32 tso			: 1;
		uint32 resv3		: 6;
		uint32 fPort		: 3;
		uint32 vlanEn		: 1;
		uint32 vlanTpID 	: 2;
		uint32 vlanTag		: 16;
#else
		uint32 vlanTag		: 16;
		uint32 vlanTpID 	: 2;
		uint32 vlanEn		: 1;
		uint32 fPort		: 3;
		uint32 resv3		: 6;
		uint32 tso			: 1;
		uint32 tco			: 1;
		uint32 uco			: 1;
		uint32 ico			: 1;
#endif /* __BIG_ENDIAN */
	} raw ;
	uint msg[2] ;
} ethTxMsg_t ;

typedef union
{
	uint32 word;

} rxMsgWord0_t;

typedef union
{	
	uint32 word;

} rxMsgWord1_t;

typedef union
{
	uint32 word;

} rxMsgWord2_t;


typedef struct ethRxMsg_s
{
	rxMsgWord0_t rxMsgW0;
    rxMsgWord1_t rxMsgW1;
    rxMsgWord2_t rxMsgW2;
    uint32  resv;
} ethRxMsg_t;


typedef struct {
	uint 	resv1 ;
	struct {
#ifdef __BIG_ENDIAN
		uint done				: 1 ;
		uint drop_pkt			: 1 ;
		uint resv2				: 14 ;
		uint pkt_len			: 16 ;
#else
		uint pkt_len			: 16 ;
		uint resv2				: 14 ;
		uint drop_pkt			: 1 ;
		uint done				: 1 ;
#endif /* __BIG_ENDIAN */
	} ctrl ;
	uint 	pkt_addr ;
#ifdef __BIG_ENDIAN
	uint resv3					: 20 ;
	uint next_idx				: 12 ;
#else
	uint next_idx				: 12 ;
	uint resv3					: 20 ;
#endif /* __BIG_ENDIAN */
	uint msg[4] ;
} QDMA_DMA_DSCP_T ;


struct QDMA_DscpInfo_S {
	QDMA_DMA_DSCP_T				*dscpPtr ;
	uint						dscpIdx ;

	sk_buff				*skb ;
	struct QDMA_DscpInfo_S		*next ;
} ;


typedef struct {
	uint						csrBaseAddr ;
	ushort						txDscpNum ;				/* Total TX DSCP number */
	ushort						rxDscpNum ;				/* Total RX DSCP number */
	ushort						hwFwdDscpNum ;
	ushort						irqDepth ;				/* Max depth for IRQ queue */
	ushort						hwPktSize ;
	uint						dscpInfoAddr ; 			/* Start pointer for DSCP information node */
	uint						txBaseAddr ;
	uint						rxBaseAddr ;
	uint						irqQueueAddr ;			/* IRQ queue address */
	uint						hwFwdBaseAddr ;			/* Base address of the hardware forwarding */
	uint						hwFwdBuffAddr ;			/* Base address of the hardware forwarding Buffer*/
	uint						hwFwdPayloadSize ;		/* Payload size of the hardware forwarding Buffer*/
	struct QDMA_DscpInfo_S		*txHeadPtr ;			/* Head node for unused tx desc. */
	struct QDMA_DscpInfo_S		*txTailPtr ;			/* Tail node for unused tx desc. */
	struct QDMA_DscpInfo_S		*txUsingPtr ;			/* TXDMA using DSCP node. */
	struct QDMA_DscpInfo_S		*rxStartPtr ;			/* Start using node for rx desc. */
	struct QDMA_DscpInfo_S		*rxEndPtr ;				/* End using node for rx desc. */
	struct QDMA_DscpInfo_S		*rxUsingPtr ;			/* RXDMA using DSCP node. */

	BM_Counters_T				counters ;

} QDMA_Private_T ;

typedef struct macMemPool_s
{
	uint8 descrPool[MEM_POOL_SIZE];	/* Descr pool area */
} macMemPool_t;



/* ----- Ethernet Link Profile ----- */
typedef struct macPhyLinkProfile_s {
	uint32 linkSpeed;							/* 10Mbps or 100Mbps */
	uint32 duplexMode;							/* Half/Full Duplex Mode */
	uint32 enetMode;
	uint32 ANCompFlag;							/* auto_negotiation complete Flag */
	uint32 PollCount;							/* auto_negotiation polling check count */
} macPhyLinkProfile_t;



typedef struct macAdapter_s
{
  uint8  macAddr[6];  /* MAC-Address */
  macPhyLinkProfile_t *macPhyLinkProfile_p;
  macMemPool_t        *macMemPool_p;
  uint32 enetPhyAddr;
  uint8  enetPhyId;
} macAdapter_t;

typedef struct phyDeviceList_s {
	uint16 companyId;
	char vendorName[30];
} phyDeviceList_t;

#define GDMA1_BASE     		(0xBFB50500)
#define GDMA1_FWD_CFG       (GDMA1_BASE + 0x00)
#define GDMA1_SHRP_CFG      (GDMA1_BASE + 0x04)
#define GDMA1_MAC_ADRL      (GDMA1_BASE + 0x08)
#define GDMA1_MAC_ADRH      (GDMA1_BASE + 0x0c)
#define GDMA1_VLAN_GEN    (GDMA1_BASE + 0x10)
#define GDMA1_LEN_CFG       (GDMA1_BASE + 0x14)



#define CONFIG_QDMA_BASE_ADDR                       (0x1FB54000)
#define QDMA_REG_BASE                               (0xBFB54000)
#define RING_OFFSET(idx)                            (idx<<8)



/*******************************************************
 CSR for QDMA
********************************************************/
#define QDMA_CSR_INFO(base)							(base+0x0000)
#define QDMA_CSR_GLB_CFG(base)						(base+0x0004)
#define QDMA_CSR_TX_DSCP_BASE(base, idx)			(base+0x0008+RING_OFFSET(idx))
#define QDMA_CSR_RX_DSCP_BASE(base, idx)			(base+0x000C+RING_OFFSET(idx))
#define QDMA_CSR_TX_CPU_IDX(base, idx)				(base+0x0010+RING_OFFSET(idx))
#define QDMA_CSR_TX_DMA_IDX(base, idx)				(base+0x0014+RING_OFFSET(idx))
#define QDMA_CSR_RX_CPU_IDX(base, idx)				(base+0x0018+RING_OFFSET(idx))
#define QDMA_CSR_RX_DMA_IDX(base, idx)				(base+0x001C+RING_OFFSET(idx))

#define QDMA_CSR_HWFWD_DSCP_BASE(base)				(base+0x0020)
#define QDMA_CSR_HWFWD_BUFF_BASE(base)				(base+0x0024)
#define QDMA_CSR_HWFWD_DSCP_CFG(base)				(base+0x0028)
#define QDMA_CSR_LMGR_INIT_CFG(base)				(base+0x0030)
#define QDMA_CSR_LMGR_CHNL_RETIRE(base)				(base+0x0040)

#define QDMA_CSR_INT_STATUS(base)					(base+0x0050)
#define QDMA_CSR_INT_ENABLE(base)					(base+0x0054)
#define QDMA_CSR_TX_DELAY_INT_CFG(base)				(base+0x0058)
#define QDMA_CSR_RX_DELAY_INT_CFG(base)				(base+0x005C)
#define QDMA_CSR_IRQ_BASE(base)						(base+0x0060)
#define QDMA_CSR_IRQ_CFG(base)						(base+0x0064)
#define QDMA_CSR_IRQ_CLEAR_LEN(base)				(base+0x0068)
#define QDMA_CSR_IRQ_STATUS(base)					(base+0x006C)
#define QDMA_CSR_IRQ_PTIME(base)					(base+0x0070)

#define QDMA_CSR_TXWRR_MODE_CFG(base)				(base+0x0080)
#define QDMA_CSR_TXWRR_WEIGHT_CFG(base)				(base+0x0088)
#define QDMA_CSR_PSE_BUF_USAGE_CFG(base)			(base+0x0090)
#define QDMA_CSR_EGRESS_RATEMETER_CFG(base)			(base+0x0094)
#define QDMA_CSR_EGRESS_RATELIMIT_CFG(base)			(base+0x0098)
#define QDMA_CSR_RATELIMIT_PARAMETER_CFG(base)		(base+0x009C)

#define QDMA_CSR_TXQ_CNGST_CFG(base)				(base+0x00A0)
#define QDMA_CSR_TXQ_CNGST_TOTALTHR(base)			(base+0x00A4)
#define QDMA_CSR_TXQ_DYN_CHNLTHR_CFG(base)			(base+0x00A8)
#define QDMA_CSR_TXQ_DYN_QUEUETHR_CFG(base)			(base+0x00AC)
#define QDMA_CSR_STATIC_QUEUE_THR(base, i)			(base+0x00B0+(i<<2))

#define QDMA_CSR_DBG_LMGR_STATUS(base)				(base+0x00F0)
#define QDMA_CSR_DBG_QDMA_PROBE_LO(base)			(base+0x00F8)
#define QDMA_CSR_DBG_QDMA_PROBE_HI(base)			(base+0x00FC)

#define QDMA_CSR_RX_RING_CFG(base)					(base+0x0100)
#define QDMA_CSR_RX_RING_THR(base)					(base+0x0104)

#define QDMA_CSR_PERCHNL_QOS_MODE(base, i)			(base+0x0180+(i<<2))
#define QDMA_CSR_PERCHNL_DATARATE(base, i)			(base+0x0200+(i<<2))
#define QDMA_CSR_DBG_CNTR_CFG(base, i)				(base+0x0300+(i<<3))
#define QDMA_CSR_DBG_CNTR_VAR(base, i)				(base+0x0304+(i<<3))

#define QDMA_CSR_INT_STATUS(base)					(base+0x0050)
#define QDMA_CSR_INT_ENABLE(base)					(base+0x0054)



#define qdmaSetTxDscpBase(base, idx, val)	IO_SREG(QDMA_CSR_TX_DSCP_BASE(base, idx), val)
#define qdmaGetTxDscpBase(base, idx)		IO_GREG(QDMA_CSR_TX_DSCP_BASE(base, idx))
#define qdmaSetRxDscpBase(base, idx, val)	IO_SREG(QDMA_CSR_RX_DSCP_BASE(base, idx), val)
#define qdmaGetRxDscpBase(base, idx)		IO_GREG(QDMA_CSR_RX_DSCP_BASE(base, idx))
#define qdmaSetTxCpuIdx(base, idx, val)		IO_SMASK(QDMA_CSR_TX_CPU_IDX(base, idx), TX_CPU_IDX_MASK, TX_CPU_IDX_SHIFT, val)
#define qdmaGetTxCpuIdx(base, idx)			IO_GMASK(QDMA_CSR_TX_CPU_IDX(base, idx), TX_CPU_IDX_MASK, TX_CPU_IDX_SHIFT)
#define qdmaSetTxDmaIdx(base, idx, val)		IO_SMASK(QDMA_CSR_TX_DMA_IDX(base, idx), TX_DMA_IDX_MASK, TX_DMA_IDX_SHIFT, val)
#define qdmaGetTxDmaIdx(base, idx)			IO_GMASK(QDMA_CSR_TX_DMA_IDX(base, idx), TX_DMA_IDX_MASK, TX_DMA_IDX_SHIFT)
#define qdmaSetRxCpuIdx(base, idx, val)		IO_SMASK(QDMA_CSR_RX_CPU_IDX(base, idx), RX_CPU_IDX_MASK, RX_CPU_IDX_SHIFT, val)
#define qdmaGetRxCpuIdx(base, idx)			IO_GMASK(QDMA_CSR_RX_CPU_IDX(base, idx), RX_CPU_IDX_MASK, RX_CPU_IDX_SHIFT)
#define qdmaSetRxDmaIdx(base, idx, val)		IO_SMASK(QDMA_CSR_RX_DMA_IDX(base, idx), RX_DMA_IDX_MASK, RX_DMA_IDX_SHIFT, val)
#define qdmaGetRxDmaIdx(base, idx)			IO_GMASK(QDMA_CSR_RX_DMA_IDX(base, idx), RX_DMA_IDX_MASK, RX_DMA_IDX_SHIFT)

#define qdmaSetRxRingSize(base, idx, val)	IO_SMASK(QDMA_CSR_RX_RING_CFG(base), RX_RING_SIZE_MASK(idx), RX_RING_SIZE_SHIFT(idx), val)
#define qdmaGetRxRingSize(base, idx)		IO_GMASK(QDMA_CSR_RX_RING_CFG(base), RX_RING_SIZE_MASK(idx), RX_RING_SIZE_SHIFT(idx))
#define qdmaSetRxRingThrh(base, idx, val)	IO_SMASK(QDMA_CSR_RX_RING_THR(base), RX_RING_LOW_THR_MASK(idx), RX_RING_LOW_THR_SHIFT(idx), val)
#define qdmaGetRxRingThrh(base, idx)		IO_GMASK(QDMA_CSR_RX_RING_THR(base), RX_RING_LOW_THR_MASK(idx), RX_RING_LOW_THR_SHIFT(idx))

#define qdmaSetHwDscpBase(base, val)	IO_SREG(QDMA_CSR_HWFWD_DSCP_BASE(base), val)
#define qdmaGetHwDscpBase(base)			IO_GREG(QDMA_CSR_HWFWD_DSCP_BASE(base))
#define qdmaSetHwBuffBase(base, val)	IO_SREG(QDMA_CSR_HWFWD_BUFF_BASE(base), val)
#define qdmaGetHwBuffBase(base)			IO_GREG(QDMA_CSR_HWFWD_BUFF_BASE(base))
#define qdmaSetHwPayloadSize(base, val)	IO_SMASK(QDMA_CSR_HWFWD_DSCP_CFG(base), HWFWD_PAYLOAD_SIZE_MASK, HWFWD_PAYLOAD_SIZE_SHIFT, val)
#define qdmaGetHwPayloadSize(base)		IO_GMASK(QDMA_CSR_HWFWD_DSCP_CFG(base), HWFWD_PAYLOAD_SIZE_MASK, HWFWD_PAYLOAD_SIZE_SHIFT)
#define qdmaSetHwLowThrshld(base, val)	IO_SMASK(QDMA_CSR_HWFWD_DSCP_CFG(base), HWFWD_DSCP_LOW_THRSHLD_MASK, HWFWD_DSCP_LOW_THRSHLD_SHIFT, val)
#define qdmaGetHwLowThrshld(base)		IO_GMASK(QDMA_CSR_HWFWD_DSCP_CFG(base), HWFWD_DSCP_LOW_THRSHLD_MASK, HWFWD_DSCP_LOW_THRSHLD_SHIFT)
#define qdmaSetHwDscpNum(base, val)		IO_SMASK(QDMA_CSR_LMGR_INIT_CFG(base), HWFWD_DSCP_NUM_MASK, HWFWD_DSCP_NUM_SHIFT, val)
#define qdmaGetHwDscpNum(base)			IO_GMASK(QDMA_CSR_LMGR_INIT_CFG(base), HWFWD_DSCP_NUM_MASK, HWFWD_DSCP_NUM_SHIFT)
#define qdmaSetHWInitStart(base)		IO_SBITS(QDMA_CSR_LMGR_INIT_CFG(base), LMGR_INIT_START)
#define qdmaGetHWInitStart(base)		(IO_GREG(QDMA_CSR_LMGR_INIT_CFG(base)) & LMGR_INIT_START)
#define qdmaSetGlbCfg(base, val)		IO_SREG(QDMA_CSR_GLB_CFG(base), val)
#define qdmaGetIntStatus(base)			IO_GREG(QDMA_CSR_INT_STATUS(base))
#define qdmaGetIrqEntryLen(base)		IO_GMASK(QDMA_CSR_IRQ_STATUS(base), IRQ_STATUS_ENTRY_LEN_MASK, IRQ_STATUS_ENTRY_LEN_SHIFT)
#define qdmaClearIntStatus(base, val)	IO_SREG(QDMA_CSR_INT_STATUS(base), val)
#define qdmaSetIntMask(base, val)		IO_SREG(QDMA_CSR_INT_ENABLE(base), val)
#define qdmaGetIrqStatus(base)			IO_GREG(QDMA_CSR_IRQ_STATUS(base))
#define qdmaGetIntMask(base)			IO_GREG(QDMA_CSR_INT_ENABLE(base))
#define qdmaSetIrqBase(base, val)		IO_SREG(QDMA_CSR_IRQ_BASE(base), val)
#define qdmaSetIrqDepth(base, val)		IO_SMASK(QDMA_CSR_IRQ_CFG(base), IRQ_CFG_DEPTH_MASK, IRQ_CFG_DEPTH_SHIFT, val)
#define qdmaSetIrqClearLen(base, val)	IO_SMASK(QDMA_CSR_IRQ_CLEAR_LEN(base), IRQ_CLEAR_LEN_MASK, IRQ_CLEAR_LEN_SHIFT, val)

#define qdmaEnableRxDma(base)			IO_SBITS(QDMA_CSR_GLB_CFG(base), GLB_CFG_RX_DMA_EN)
#define qdmaDisableRxDma(base)			IO_CBITS(QDMA_CSR_GLB_CFG(base), GLB_CFG_RX_DMA_EN)
#define qdmaEnableTxDma(base)			IO_SBITS(QDMA_CSR_GLB_CFG(base), GLB_CFG_TX_DMA_EN)
#define qdmaDisableTxDma(base)			IO_CBITS(QDMA_CSR_GLB_CFG(base), GLB_CFG_TX_DMA_EN)



/* define GDMA port */
#define GDM_P_PDMA						(0x0)
#define GDM_P_GDMA1						(0x1)
#define GDM_P_GDMA2						(0x2)
#define GDM_P_PPE						(0x4)
#define GDM_P_QDMA						(0x5)
#define GDM_P_DISCARD					(0x7)
#define GDM_P_CPU						GDM_P_PDMA



/* QDMA_CSR_HWFWD_DSCP_CFG(base) */
#define HWFWD_PAYLOAD_SIZE_SHIFT					(28)
#define HWFWD_PAYLOAD_SIZE_MASK						(0x3<<HWFWD_PAYLOAD_SIZE_SHIFT)
#define HWFWD_PAYLOAD_SIZE_2K						(0x0)
#define HWFWD_PAYLOAD_SIZE_4K						(0x1)
#define HWFWD_PAYLOAD_SIZE_8K						(0x2)
#define HWFWD_PAYLOAD_SIZE_16K						(0x3)
#define HWFWD_DSCP_LOW_THRSHLD_SHIFT				(0)
#define HWFWD_DSCP_LOW_THRSHLD_MASK					(0x1FFF<<HWFWD_DSCP_LOW_THRSHLD_SHIFT)


/* QDMA_CSR_INT_STATUS(base) */
#define INT_STATUS_XPON_PHY							(1<<24)
#define INT_STATUS_EPON_MAC							(1<<17)
#define INT_STATUS_GPON_MAC							(1<<16)
#define INT_STATUS_RX1_COHERENT						(1<<15)
#define INT_STATUS_TX1_COHERENT						(1<<14)
#define INT_STATUS_RX0_COHERENT						(1<<13)
#define INT_STATUS_TX0_COHERENT						(1<<12)
#define INT_STATUS_HWFWD_DSCP_LOW					(1<<10)
#define INT_STATUS_IRQ_FULL							(1<<9)
#define INT_STATUS_HWFWD_DSCP_EMPTY					(1<<8)
#define INT_STATUS_NO_RX1_CPU_DSCP					(1<<7)
#define INT_STATUS_NO_TX1_CPU_DSCP					(1<<6)
#define INT_STATUS_RX1_DONE							(1<<5)
#define INT_STATUS_TX1_DONE							(1<<4)
#define INT_STATUS_NO_RX0_CPU_DSCP					(1<<3)
#define INT_STATUS_NO_TX0_CPU_DSCP					(1<<2)
#define INT_STATUS_RX0_DONE							(1<<1)
#define INT_STATUS_TX0_DONE							(1<<0)
#define INT_STATUS_QDMA_DONE						(0x00000033)
#define INT_STATUS_QDMA_FAULT						(0x0000F740)
#define INT_STATUS_EXTERNAL							(0x01030000)

/* QDMA_CSR_GLB_CFG(base) */
#define GLB_CFG_RX_2B_OFFSET						(1<<31)
#define GLB_CFG_DMA_PREFERENCE_SHIFT				(29)
#define GLB_CFG_DMA_PREFERENCE_MASK					(0x3<<GLB_CFG_DMA_PREFERENCE_SHIFT)
#define PREFER_ROIND_ROBIN							(0x00)
#define PREFER_FWD_TX1_TX0							(0x01)
#define PREFER_TX1_FWD_TX0							(0x10)
#define PREFER_TX1_TX0_FWD							(0x11)
#define GLB_CFG_MSG_WORD_SWAP						(1<<28)
#define GLB_CFG_DSCP_BYTE_SWAP						(1<<27)
#define GLB_CFG_PAYLOAD_BYTE_SWAP					(1<<26)
#define GLB_CFG_SLM_RELEASE_EN						(1<<21)
#define GLB_CFG_TX_IMMEDIATE_DONE					(1<<20)
#define GLB_CFG_IRQ_EN								(1<<19)
#define GLB_CFG_LOOPCNT_EN							(1<<18)
#define GLB_CFG_UMAC_LOOPBACK						(1<<17)
#define GLB_CFG_QDMA_LOOPBACK						(1<<16)
#define GLB_CFG_CHECK_DONE							(1<<7)
#define GLB_CFG_TX_WB_DONE							(1<<6)
#define GLB_CFG_BST_SE_SHIFT						(4)
#define GLB_CFG_BST_SE_MASK							(0x3<<GLB_CFG_BST_SE_SHIFT)
#define VAL_BST_4_DWORD								(0x0)
#define VAL_BST_8_DWORD								(0x1)
#define VAL_BST_16_DWARD							(0x2)
#define VAL_BST_32_DWARD							(0x3)
#define GLB_CFG_RX_DMA_BUSY							(1<<3)
#define GLB_CFG_RX_DMA_EN							(1<<2)
#define GLB_CFG_TX_DMA_BUSY							(1<<1)
#define GLB_CFG_TX_DMA_EN							(1<<0)

/* QDMA_CSR_TX_CPU_IDX(base) */
#define TX_CPU_IDX_SHIFT							(0)
#define TX_CPU_IDX_MASK								(0xFFF<<TX_CPU_IDX_SHIFT)

/* QDMA_CSR_TX_DMA_IDX(base) */
#define TX_DMA_IDX_SHIFT							(0)
#define TX_DMA_IDX_MASK								(0xFFF<<TX_DMA_IDX_SHIFT)

/* QDMA_CSR_RX_CPU_IDX(base) */
#define RX_CPU_IDX_SHIFT							(0)
#define RX_CPU_IDX_MASK								(0xFFF<<RX_CPU_IDX_SHIFT)

/* QDMA_CSR_RX_DMA_IDX(base) */
#define RX_DMA_IDX_SHIFT							(0)
#define RX_DMA_IDX_MASK								(0xFFF<<RX_DMA_IDX_SHIFT)

/* QDMA_CSR_LMGR_INIT_CFG(base) */
#define LMGR_INIT_START								(1<<31)
#define HWFWD_DSCP_NUM_SHIFT						(0)
#define HWFWD_DSCP_NUM_MASK							(0x1FFF<<HWFWD_DSCP_NUM_SHIFT)


/* QDMA_CSR_RX_RING_CFG */
#define RX_RING_SIZE_SHIFT(idx)						(idx<<4)
#define RX_RING_SIZE_MASK(idx)						(0xFFF<<RX_RING_SIZE_SHIFT(idx))

/* QDMA_CSR_RX_RING_THR */
#define RX_RING_LOW_THR_SHIFT(idx)					(idx<<4)
#define RX_RING_LOW_THR_MASK(idx)					(0xFFF<<RX_RING_SIZE_SHIFT(idx))


/* QDMA_CSR_IRQ_CFG(base) */
#define IRQ_CFG_THRESHOLD_SHIFT						(16)
#define IRQ_CFG_THRESHOLD_MASK						(0xFFF<<IRQ_CFG_THRESHOLD_SHIFT)
#define IRQ_CFG_DEPTH_SHIFT							(0)
#define IRQ_CFG_DEPTH_MASK							(0xFFF<<IRQ_CFG_DEPTH_SHIFT)

/* QDMA_CSR_IRQ_CLEAR_LEN(base) */
#define IRQ_CLEAR_LEN_SHIFT							(0)
#define IRQ_CLEAR_LEN_MASK							(0xFF<<IRQ_CLEAR_LEN_SHIFT)


/* GDMA1_FWD_CFG or GDMA2_FWD_CFG */
#define GDM_JMB_LEN_SHIFT				(28)
#define GDM_JMB_LEN						(0xf<<GDM_JMB_LEN_SHIFT)
#define GDM_20US_TICK_SLT				(1<<25)

#define GDM_INSV_EN						(1<<26)
#define GDM_UNTAG_EN					(1<<25)
#define GDM_STAG_EN						(1<<24)
#define GDM_ICS_EN						(1<<22)
#define GDM_TCS_EN						(1<<21)
#define GDM_UCS_EN						(1<<20)
#define GDM_DROP_256B					(1<<19)
#define GDM_DISPAD						(1<<18)
#define GDM_DISCRC						(1<<17)
#define GDM_STRPCRC						(1<<16)
#define GDM_UFRC_P_SHIFT				(12)
#define GDM_UFRC_P						(0xf<<GDM_UFRC_P_SHIFT)
#define GDM_BFRC_P_SHIFT				(8)
#define GDM_BFRC_P						(0xf<<GDM_BFRC_P_SHIFT)
#define GDM_MFRC_P_SHIFT				(4)
#define GDM_MFRC_P						(0xf<<GDM_MFRC_P_SHIFT)
#define GDM_OFRC_P_SHIFT				(0)
#define GDM_OFRC_P						(0xf<<GDM_MFRC_P_SHIFT)



/********************************
 * Giga Switch Module Registers *
 ********************************/

#define GSW_BASE     		0xBFB58000
#define GSW_ARL_BASE     	(GSW_BASE + 0x0000)
#define GSW_BMU_BASE     	(GSW_BASE + 0x1000)
#define GSW_PORT_BASE     	(GSW_BASE + 0x2000)
#define GSW_MAC_BASE     	(GSW_BASE + 0x3000)
#define GSW_MIB_BASE     	(GSW_BASE + 0x4000)
#define GSW_CFG_BASE     	(GSW_BASE + 0x7000)


#define GSW_ERLCR(n)			(GSW_BASE + (n)*0x100 + 0x1040)

#define GSW_MFC     		(GSW_ARL_BASE + 0x10)

#define GSW_IMC     		(GSW_ARL_BASE + 0x1c)
#define IMC_IGMP_RPT_FW_SHIFT			(12)

#define GSW_PSC(n)     		(GSW_PORT_BASE + (n)*0x100 + 0x0C)

#define GSW_PMCR(n)     	(GSW_MAC_BASE + (n)*0x100)
#define GSW_PMSR(n)     	(GSW_MAC_BASE + (n)*0x100 + 0x08)
#define GSW_PINT_EN(n)     	(GSW_MAC_BASE + (n)*0x100 + 0x10)
#define GSW_SMACCR0     	(GSW_MAC_BASE + 0xe4)
#define GSW_SMACCR1     	(GSW_MAC_BASE + 0xe8)
#define GSW_CKGCR			(GSW_MAC_BASE + 0xf0)

#define GSW_TX_DROC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x00)
#define GSW_TX_CRC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x04)
#define GSW_TX_UNIC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x08)
#define GSW_TX_MULC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x0c)
#define GSW_TX_BROC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x10)
#define GSW_TX_COLC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x14)
#define GSW_TX_SCOLC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x18)
#define GSW_TX_MCOLC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x1c)
#define GSW_TX_DEFC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x20)
#define GSW_TX_LCOLC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x24)
#define GSW_TX_ECOLC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x28)
#define GSW_TX_PAUC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x2c)
#define GSW_TX_OCL(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x48)
#define GSW_TX_OCH(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x4c)

#define GSW_RX_DROC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x60)
#define GSW_RX_FILC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x64)
#define GSW_RX_UNIC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x68)
#define GSW_RX_MULC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x6c)
#define GSW_RX_BROC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x70)
#define GSW_RX_ALIGE(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x74)
#define GSW_RX_CRC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x78)
#define GSW_RX_RUNT(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x7c)
#define GSW_RX_FRGE(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x80)
#define GSW_RX_LONG(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x84)
#define GSW_RX_JABE(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x88)
#define GSW_RX_PAUC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x8c)
#define GSW_RX_OCL(n)    		(GSW_MIB_BASE + (n)*0x100 + 0xa8)
#define GSW_RX_OCH(n)    		(GSW_MIB_BASE + (n)*0x100 + 0xac)
#define GSW_RX_INGC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0xb4)
#define GSW_RX_ARLC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0xb8)
	


#define EXT_GSW_TX_DROC(n)    		(0x4000 + (n)*0x100)
#define EXT_GSW_TX_CRC(n)    		(0x4004 + (n)*0x100)
#define EXT_GSW_TX_UNIC(n)    		(0x4008 + (n)*0x100)
#define EXT_GSW_TX_MULC(n)    		(0x400c + (n)*0x100)
#define EXT_GSW_TX_BROC(n)    		(0x4010 + (n)*0x100)
#define EXT_GSW_TX_COLC(n)    		(0x4014 + (n)*0x100)
#define EXT_GSW_TX_SCOLC(n)    		(0x4018 + (n)*0x100)
#define EXT_GSW_TX_MCOLC(n)    		(0x401c + (n)*0x100)
#define EXT_GSW_TX_DEFC(n)    		(0x4020 + (n)*0x100)
#define EXT_GSW_TX_LCOLC(n)    		(0x4024 + (n)*0x100)
#define EXT_GSW_TX_ECOLC(n)    		(0x4028 + (n)*0x100)
#define EXT_GSW_TX_PAUC(n)    		(0x402c + (n)*0x100)
#define EXT_GSW_TX_OCL(n)    		(0x4048 + (n)*0x100)
#define EXT_GSW_TX_OCH(n)    		(0x404c + (n)*0x100)

#define EXT_GSW_RX_DROC(n)    		(0x4060 + (n)*0x100)
#define EXT_GSW_RX_FILC(n)    		(0x4064 + (n)*0x100)
#define EXT_GSW_RX_UNIC(n)    		(0x4068 + (n)*0x100)
#define EXT_GSW_RX_MULC(n)    		(0x406c + (n)*0x100)
#define EXT_GSW_RX_BROC(n)    		(0x4070 + (n)*0x100)
#define EXT_GSW_RX_ALIGE(n)    		(0x4074 + (n)*0x100)
#define EXT_GSW_RX_CRC(n)    		(0x4078 + (n)*0x100)
#define EXT_GSW_RX_RUNT(n)    		(0x407c + (n)*0x100)
#define EXT_GSW_RX_FRGE(n)    		(0x4080 + (n)*0x100)
#define EXT_GSW_RX_LONG(n)    		(0x4084 + (n)*0x100)
#define EXT_GSW_RX_JABE(n)    		(0x4088 + (n)*0x100)
#define EXT_GSW_RX_PAUC(n)    		(0x408c + (n)*0x100)
#define EXT_GSW_RX_OCL(n)    		(0x40a8 + (n)*0x100)
#define EXT_GSW_RX_OCH(n)    		(0x40ac + (n)*0x100)
#define EXT_GSW_RX_INGC(n)    		(0x40b4 + (n)*0x100)
#define EXT_GSW_RX_ARLC(n)    		(0x40b8 + (n)*0x100)

#define GSW_PVC(n)     	(0x2010 + (n)*0x100)
#define DEFAULT_TPID  	(0x8100)

#define GSW_CFG_PPSC     	(GSW_CFG_BASE + 0x18)
#define GSW_CFG_PIAC     	(GSW_CFG_BASE + 0x1c)
#define GSW_CFG_GPC     	(GSW_CFG_BASE + 0x14)


#define GSW_VLAN_REG		(GSW_BASE+0x94)
#define GSW_ATA1_REG		(GSW_BASE+0x74)
#define GSW_ATC_REG			(GSW_BASE+0x80)
#define GSW_CFG_CREV        (GSW_CFG_BASE + 0xFFC)
#define EXT_GSW_CFG_CREV    (0x7FFC)


//switch define 
/* GSW_MFC */
#define MFC_BC_FFP_SHIFT				(24)
#define MFC_BC_FFP						(0xff<<MFC_BC_FFP_SHIFT)
#define MFC_UNM_FFP_SHIFT				(16)
#define MFC_UNM_FFP						(0xff<<MFC_UNM_FFP_SHIFT)
#define MFC_UNU_FFP_SHIFT				(8)
#define MFC_UNU_FFP						(0xff<<MFC_UNU_FFP_SHIFT)
#define MFC_CPU_EN						(1<<7)
#define MFC_CPU_PORT_SHIFT				(4)
#define MFC_CPU_PORT					(0x7<<MFC_CPU_PORT_SHIFT)
#define MFC_MIRROR_EN					(1<<3)
#define MFC_MIRROR_PORT_SHIFT			(0)
#define MFC_MIRROT_PORT					(0x7<<MFC_MIRROR_PORT_SHIFT)

/* GSW_PMCR */
#define IPG_CFG_PN_SHIFT				(18)
#define IPG_CFG_PN						(0x3<<IPG_CFG_PN_SHIFT)
#define EXT_PHY_PN						(1<<17)
#define MAC_MODE_PN						(1<<16)
#define FORCE_MODE_PN					(1<<15)
#define MAC_TX_EN_PN					(1<<14)
#define MAC_RX_EN_PN					(1<<13)
#define RGMII_MODE_PN					(1<<12)
#define BKOFF_EN_PN						(1<<9)
#define BACKPR_EN_PN					(1<<8)
#define ENABLE_EEE1G_PN					(1<<7)
#define ENABLE_EEE100_PN				(1<<6)
#define ENABLE_RX_FC_PN					(1<<5)
#define ENABLE_TX_FC_PN					(1<<4)
#define FORCE_SPD_PN_SHIFT				(2)
#define FORCE_SPD_PN					(0x3<<FORCE_SPD_PN_SHIFT)
#define FORCE_DPX_PN					(1<<1)
#define FORCE_LNK_PN					(1<<0)

#define IPG_CFG_NORMAL					(0)
#define IPG_CFG_SHORT					(1)
#define IPG_CFG_64BITS                                  (0x2)

#define PN_SPEED_10M					(0)
#define PN_SPEED_100M					(1)
#define PN_SPEED_1000M					(2)

/* GSW_PMSR */
#define EEE1G_STS						(1<<7)
#define EEE100_STS						(1<<6)
#define RX_FC_STS						(1<<5)
#define TX_FC_STS						(1<<4)
#define MAC_SPD_STS_SHIFT				(2)
#define MAC_SPD_STS						(0x3<<MAC_SPD_STS_SHIFT)
#define MAC_DPX_STS						(1<<1)
#define MAC_LINK_STS					(1<<0)


/* GSW_CFG_PPSC */
#define PHY_AP_EN_SHIFT					(1<<24)
#define PHY_AP_EN						(0x7f<<PHY_END_ADDR_SHIFT)

#define PHY_EEE_EN_SHIFT				(1<<16)
#define PHY_EEE_EN						(0x7f<<PHY_END_ADDR_SHIFT)

#define PHY_PRE_EN						(1<<15)
#define PHY_END_ADDR_SHIFT				(8)
#define PHY_END_ADDR					(0x1f<<PHY_END_ADDR_SHIFT)
#define PHY_MDC_CFG_SHIFT				(1<<6)
#define PHY_MDC_CFG						(0x3<<PHY_MDC_CFG_SHIFT)
#define PHY_ST_ADDR_SHIFT				(0)
#define PHY_ST_ADDR						(0x1f<<PHY_ST_ADDR_SHIFT)

/* GSW_CFG_PIAC */
#define PHY_ACS_ST						(1<<31)
#define MDIO_REG_ADDR_SHIFT				(25)
#define MDIO_REG_ADDR					(0x1f<<MDIO_REG_ADDR_SHIFT)
#define MDIO_PHY_ADDR_SHIFT				(20)
#define MDIO_PHY_ADDR					(0x1f<<MDIO_PHY_ADDR_SHIFT)
#define MDIO_CMD_SHIFT					(18)
#define MDIO_CMD						(0x3<<MDIO_CMD_SHIFT)
#define MDIO_ST_SHIFT					(16)
#define MDIO_ST							(0x3<<MDIO_ST_SHIFT)
#define MDIO_RW_DATA_SHIFT				(0)
#define MDIO_RW_DATA					(0xffff<<MDIO_RW_DATA_SHIFT)

#define PHY_ACS_ST_START				(1)
#define MDIO_CMD_WRITE					(1)
#define MDIO_CMD_READ					(2)
#define MDIO_ST_START					(1)

#define MDIO_CL45_CMD_ADDR				(0)
#define MDIO_CL45_CMD_WRITE				(1)
#define MDIO_CL45_CMD_READ				(3)
#define MDIO_CL45_CMD_POSTREAD_INCADDR			(2)
#define MDIO_CL45_ST_START				(0)

#define NORMAL_READ					(1<<0)
#define POST_READ					(1<<1)

/* GSW_CFG_GPC */
#define RX_CLK_MODE						(1<<2)

#define IRQ_CFG_RINGIDX_SHIFT						(16)
#define IRQ_CFG_RINGIDX_MASK						(0x1<<IRQ_CFG_RINGIDX_SHIFT)
#define IRQ_CFG_IDX_MASK							0xFFF


/* QDMA_CSR_IRQ_STATUS(base) */
#define IRQ_STATUS_ENTRY_LEN_SHIFT					(16)
#define IRQ_STATUS_ENTRY_LEN_MASK					(0xFFF<<IRQ_STATUS_ENTRY_LEN_SHIFT)
#define IRQ_STATUS_HEAD_IDX_SHIFT					(0)
#define IRQ_STATUS_HEAD_IDX_MASK					(0xFFF<<IRQ_STATUS_HEAD_IDX_SHIFT)

/* QDMA_CSR_INT_ENABLE(base) */
#define INT_MASK_XPON_PHY							(1<<24)
#define INT_MASK_EPON_MAC							(1<<17)
#define INT_MASK_GPON_MAC							(1<<16)
#define INT_MASK_RX1_COHERENT						(1<<15)
#define INT_MASK_TX1_COHERENT						(1<<14)
#define INT_MASK_RX0_COHERENT						(1<<13)
#define INT_MASK_TX0_COHERENT						(1<<12)
#define INT_MASK_IRQ_FULL							(1<<9)
#define INT_MASK_HWFWD_DSCP_LOW						(1<<8)
#define INT_MASK_NO_RX1_CPU_DSCP					(1<<7)
#define INT_MASK_NO_TX1_CPU_DSCP					(1<<6)
#define INT_MASK_RX1_DONE							(1<<5)
#define INT_MASK_TX1_DONE							(1<<4)
#define INT_MASK_NO_RX0_CPU_DSCP						(1<<3)
#define INT_MASK_NO_TX0_CPU_DSCP						(1<<2)
#define INT_MASK_RX0_DONE							(1<<1)
#define INT_MASK_TX0_DONE							(1<<0)

/* RSTCTRL2 */
#define QDMA1_RST    				(1<<1)
#define QDMA2_RST    				(1<<2)
#define FE_RST    					(1<<21)
#define ESW_RST    					(1<<23)

/* SCU */
#define IOMUX_CONTROL1      0xBFA20104
#define RG_7530_RSTN        0xBFA200F8

#define MT7530_RST                    (1)


