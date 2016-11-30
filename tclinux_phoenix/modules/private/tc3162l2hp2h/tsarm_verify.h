/************************************************************************
 *
 *	Copyright (C) 2006 Trendchip Technologies, Corp.
 *	All Rights Reserved.
 *
 * Trendchip Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of Trendchip Technologies, Co. and shall
 * not be reproduced, copied, disclosed, or used in whole or in part
 * for any reason without the prior express written permission of
 * Trendchip Technologeis, Co.
 *
 *************************************************************************/
 #ifndef _TSARM_VERIFY_H_
#define _TSARM_VERIFY_H_

#define NP_ONLY_MODE	(1<<28)


#define DATA_CELL		(1<<0)
#define OAM_CELL		(1<<1)
#define CC_CELL		(1<<2)

#define OAM_CELL_SIZE	48
#define CC_CELL_SIZE		50
#define RAW_CELL_SIZE	1776
#define MIN_DATA_SIZE	15
#define MAX_SKB_Q_SIZE	512

#define MCELL_BUF_RING_SIZE	32

/*Pattern gerneator header format*/
/*
		Pattern Format
|Offset(8	)	|Seq(24)				|
|Seq(8)		| len(16)		|sed(8)	|
|Pri(8)		| Data.....			|
|			........				|
*/

#define OFFSET_LEN	1
#define SEQ_LEN		4
#define PATTERN_LEN	2
#define SED_LEN		1
#define PRIO_LEN		1
#define TOTAL_PAT_LEN	9
/*end Pattern gerneator header format*/

#define FIX_DCELL_FG			(1<<0)
#define RAND_DCELL_FG		(1<<1)
#define TX_START_ADDR_FG	(1<<2)
#define RX_START_ADDR_FG	(1<<3)
#define RAW_CELL_FG			(1<<4)
#define MPOA_ON_FG			(1<<5)
#define OAM_CELL_FG			(1<<6)
#define CC_CELL_FG			(1<<7)
#define VLAN_TAG_FG			(1<<8)
#define HW_MPOA_FG			(1<<9)
#define HW_VLAN_UNTAG_FG	(1<<10)
#define HW_WRR_RULE_FG		(1<<11)
#define CC_CONT_FG			(1<<12)


#define AX4K_LBPK_DN		0
#define AX4K_LBPK_EN		(1<<0)

#define IP_TOS_INDEX	25

#define AIS_TYPE	0x10
#define RDI_TYPE	0x11
#define LPBKREQ_TYPE	0x18
#define CONTCHKREQ_TYPE	0x14


/************************************************************************
*				CI-CMD
*************************************************************************
*/
typedef struct reg_check_s{
	char* name; /*Register Name*/
	#define	VC_TYPE	(1 << 0)
	#define	RO			(1 << 1)
	#define	WO			(1 << 2)
	#define	RW			(1 << 3)
	#define	NO_DEF		(1 << 4)
	uint8 type;/*0:VC_TYPE, 1:Read Only, 2: Write Only, 3: No default value*/
	uint32	addr;/*Register location*/
	uint32	def_value;/*Default value*/
	uint32	mask;/*For read/write test*/
} reg_check_t;

typedef struct irq_event_s{
	uint32 id;
	char* name;
}irq_event_s;


void 
dumpCell(uint8* src, int len);

void 
dumpIRQEvent(uint8 vc,uint32 irq_report);

struct sk_buff  *
atmDataInd(struct sk_buff *skb,uint8 vc,uint32 len);

void
cmpAtmCell(uint8 type, uint8* src, int len);

uint8
atmAal5DataReqVerify(struct sk_buff *skb,uint8 vc,uint8 priority);

void
atmCcLpbkHandler(uint8 *cellp);

void
atmOamLpbkHandler(atmRxCcDescr_t *atmRxCcDescrp, uint8 vc);

void 
sarVerifyInit(void);

void 
rxBufAddrOffset(struct sk_buff *skb);

uint8
isWrrPriorityPkt(struct sk_buff* skb,uint8 priority);

void
setSarVerifyDataDescrip(volatile atmTxDescr_t* atmTxDescrp);

void 
enHwVlanUntag(uint32 vc);

void 
dumpDataCell(struct sk_buff* skb, uint8 priority);

struct sk_buff *
atmDataLpbkHandler(register atmRxDescr_t* atmRxDescrp, struct sk_buff *skb, uint8 vc);

void 
dumpMCellAddr(uint8* data_p, uint8 type);

void 
sarVerifyExit(void);

#endif
