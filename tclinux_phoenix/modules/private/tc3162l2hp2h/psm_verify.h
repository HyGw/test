/************************************************************************
 *
 *	Copyright (C) 2008 Trendchip Technologies, Corp.
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

#ifndef _PSM_VERIFY_H
#define _PSM_VERIFY_H

#ifdef EEE_SUPPORT
#define EN_PSM	(1<<0)
#define EEE_RESTART_AN	(1<<1)
#define EEE_WAIT_FIN 		(1<<2)
#define EEE_LINKDOWN2DIS	(1<<3)

#define EEE_AN_ABILITY 0x2
#define EEE_PSM_MODE	(1<<0)
  
#define LOOPBACK_TX (1<<0)
#define LOOPBACK_RX_CHK (1<<1)
#define LOOPBACK_MODE(x) ((x) & 0x0f)

#define EEE_PSM_TX_MODE	(1<<1)
#define EEE_THRESHOLD_MASK	0xfff
#define EEE_WAKEUPTIME_MASK	0xffff

#define STOP_RX_CLK (1<<10)
#define NORMAL_READ (1<<0)
#define POST_READ (1<<1)

#define EN_MII_LPBK	0x1
#define DIS_MII_LPBK	0x2

#ifdef EEE_DEBUG
#define EEE_RX_ENTER_PSM (1<<15)
#define EEE_RX_LEAVE_PSM (1<<16)
#define EEE_TX_ENTER_PSM (1<<17)
#define EEE_TX_LEAVE_PSM (1<<18)
#define EEE_ALL_EVENT 0x78000

#define DUMP_IRQ_EVENT	0x1
#define DUMP_RX_PKT	0x2
#define DUMP_TX_PKT	0x4

typedef struct reg_check_s{
	char* name; /*Register Name*/
	#define	RO	(1 << 0)
	#define	WO	(1 << 1)
	#define	RW	(1 << 2)
	#define	NO_DEF	(1 << 3)
	uint8 type;/*0:Read Only, 1: Write Only, 2: No default value*/
	uint32	addr;/*Register location*/
	uint32	def_value;/*Default value*/
	uint32	mask;/*For read/write test*/
} reg_check_t;

typedef struct irq_event_s{
	uint32 id;
	char* name;
}irq_event_s;
#endif

/*ci-cmd function prototype*/
int doPsmMode (int argc, char *argv[], void *p);
int doPsmIntMask (int argc, char *argv[], void *p);
int doTxPsmMode (int argc, char *argv[], void *p);
int doPsmThreshold (int argc, char *argv[], void *p);
int doPsmWakeUpTime (int argc, char *argv[], void *p);
int doExtendMiir(int argc, char *argv[], void *p);
int doExtendMiiw(int argc, char *argv[], void *p);
int doStopRxClk(int argc, char *argv[], void *p); 

#ifdef EEE_DEBUG
int doDbgPSM(int argc, char *argv[], void *p);
int doRegCheck (int argc, char *argv[], void *p);
int doPsmWaitCnt (int argc, char *argv[], void *p);
int doPsmLpbk(int argc, char *argv[], void *p);
int doPktGen(int argc, char *argv[], void *p);
#endif


void psmCmdInit(void);
int tc3162_mac_loopback_chk(struct sk_buff *skb, struct net_device *dev);
void delay1us(int us);
unsigned short in_csum(unsigned short *ptr, int nbytes);
void dump_skb(struct sk_buff *skb); 
void dumpIRQEvent(uint32 irq_report);
int swPsmMode(int type);

uint32
tcMiiNewStationRead(uint32 devAddr,uint32 enetPhyAddr,uint32 phyReg);
void
tcMiiNewStationWrite(uint32 devAddr, uint32 enetPhyAddr, uint32 phyReg, uint32 miiData);

void
chkLinkStatus(void);
#endif/*EEE_SUPPORT*/
#endif
