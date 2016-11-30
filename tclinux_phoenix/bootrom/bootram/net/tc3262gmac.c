/*
** $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/bootrom/bootram/net/tc3262gmac.c#1 $
*/
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
/*
** $Log: tc3262gmac.c,v $
** Revision 1.5  2011/06/03 02:46:31  lino
** add RT65168 support
**
** Revision 1.4  2010/06/25 06:36:46  lino
** refine pause function, change packet buffer size to 1776, show correct phy address
**
** Revision 1.3  2010/06/15 14:35:45  shnwind
** correct pause function
**
** Revision 1.2  2010/05/24 15:00:29  here
** [Ehancemenet] Support the TC2206F swithc ic with TC3262/TC3162 platform with bootloader and sprate the switch ic into tcswitch.c files
**
** Revision 1.1.1.1  2010/04/09 09:31:07  feiyan
** New TC Linux Make Flow Trunk
**
** Revision 1.2  2010/01/15 08:20:22  here
** [BugFix]Fixed TC3162U Platform the latsch up problem.To avoid the large current problems.
**
** Revision 1.1.1.1  2009/12/17 01:47:29  josephxu
** 20091217, from Hinchu ,with VoIP
**
 */
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/kernel_stat.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/random.h>
#include <asm/bitops.h>
#include <asm/bootinfo.h>
#include <asm/irq.h>
#include <asm/mipsregs.h>
#include <asm/system.h>
#include <linux/circ_buf.h>
#include <asm/tc3162.h>
#include <asm/io.h>

#include "skbuff.h"
#include "eth.h"
#include "tc3262gmac.h"

#define printf prom_printf

static macAdapter_t DummyAdapter;
macAdapter_t *mac_p = &DummyAdapter;
static macMemPool_t enetMemPool __attribute__ ((__aligned__(32)));

#define read_reg_word(reg) 			VPint(reg)
#define write_reg_word(reg, wdata) 	VPint(reg)=wdata

extern void pause(int ms);

void miiStationWrite(uint32 enetPhyAddr, uint32 phyReg, uint32 miiData)
{
	uint32 reg;
	uint32 cnt=1000;

	if (isTC3169) {
		reg = (miiData & PHYOR_DATA) | (phyReg << PHYOR_REG_SHIFT) | (enetPhyAddr << PHYOR_ADDR_SHIFT) | PHYOR_WRITE | PHYOR_START;
		write_reg_word (CR_GMAC_PHYOR, reg);

		do {
			//pause(1);
			reg=read_reg_word (CR_GMAC_PHYOR);
			cnt--;
		} while (reg & PHYOR_START);
	} else {
		reg = (miiData & NEW_PHYOR_DATA) | 
				(phyReg << NEW_PHYOR_REG_SHIFT) | (enetPhyAddr << NEW_PHYOR_ADDR_SHIFT) | 
				(NEW_PHYOR_OP_WR << NEW_PHYOR_OP_SHIFT) | (NEW_PHYOR_ST_IEEE_OLD << NEW_PHYOR_ST_SHIFT) |
				NEW_PHYOR_BUSY;
		write_reg_word (CR_GMAC_PHYOR, reg);

		do {
			//pause(1);
			reg=read_reg_word (CR_GMAC_PHYOR);
			cnt--;
		} while (reg & NEW_PHYOR_BUSY);
	}
}

uint32 miiStationRead(uint32 enetPhyAddr, uint32 phyReg)
{
	uint32 reg;
	uint32 cnt=1000;

	if (isTC3169) {
		reg = (phyReg << PHYOR_REG_SHIFT) | (enetPhyAddr << PHYOR_ADDR_SHIFT) | PHYOR_START;
		write_reg_word (CR_GMAC_PHYOR, reg);

		do {
			//pause(1);
			reg=read_reg_word (CR_GMAC_PHYOR);
			cnt--;
		} while (reg & PHYOR_START);

		return (reg & PHYOR_DATA);
	} else {
		reg = (phyReg << NEW_PHYOR_REG_SHIFT) | (enetPhyAddr << NEW_PHYOR_ADDR_SHIFT) | 
				(NEW_PHYOR_OP_RD << NEW_PHYOR_OP_SHIFT) | (NEW_PHYOR_ST_IEEE_OLD << NEW_PHYOR_ST_SHIFT) |
				NEW_PHYOR_BUSY;
		write_reg_word (CR_GMAC_PHYOR, reg);

		do {
			//pause(1);
			reg=read_reg_word (CR_GMAC_PHYOR);
			cnt--;
		} while (reg & NEW_PHYOR_BUSY);

		return (reg & NEW_PHYOR_DATA);
	}
}

void macResetSwMAC(void)
{
	uint32 reg;

	// Disable DMA Enable
	reg = 0;
	write_reg_word (CR_GMAC_GCTR, reg);
	pause(2);

	// Reset Mac
	reg = GCTR_HW_RESET;
	write_reg_word (CR_GMAC_GCTR, reg);
	pause(10);
}

#if defined(TCSUPPORT_MULTI_UPGRADE) || defined(TCSUPPORT_MULTI_BOOT)
static void pause3(int ms)
{
	unsigned int i;
	for (i = 0; i < ms * 100000; i++)
		;
}


void resetSwMAC3262(void)
{
	uint32 reg;

	// Disable DMA Enable
	reg = 0;
	write_reg_word (CR_GMAC_GCTR, reg);
	pause3(2);

	// Reset Mac
	reg = GCTR_HW_RESET;
	write_reg_word (CR_GMAC_GCTR, reg);

	pause3(10);	
}
#endif

void macControllerStop(macAdapter_t *mac_p)
{
	uint32 reg;

	reg = read_reg_word(CR_GMAC_GCTR);
	/* disable DMA tx& rx */
	reg &= ~GCTR_TX_DMA_EN;
	reg &= ~GCTR_RX_DMA_EN;
	/* disable mac tx& rx */
	reg &= ~GCTR_TX_EN;
	reg &= ~GCTR_RX_EN;
	write_reg_word(CR_GMAC_GCTR, reg);
}

void macDefaultParaSet(macAdapter_t *mac_p)
{
	mac_p->rxDescrSize = MAC_RXDESCP_SIZE;
	mac_p->txDescrSize = MAC_TXDESCP_SIZE;
	mac_p->rxRingSize  = MAC_RXDESCP_NO;
	mac_p->txRingSize  = MAC_TXDESCP_NO;
}

void macGetMacAddr(macAdapter_t *mac_p, unsigned char *macAddr)
{
	int i;

	for (i = 0; i < 6; i++)
		mac_p->macAddr[i] = macAddr[i];
}

extern void flush_dcache_range(unsigned long start, unsigned long end);

int macDrvDescripInit(macAdapter_t *mac_p)
{
	macRxDescr_t *pRxDescp;
  	macTxDescr_t *pTxDescp;
  	uint32 physicalAddr;
  	uint32 i;
	sk_buff *skb;

	flush_dcache_range((uint32)&mac_p->macMemPool_p->descrPool[0], 
						(uint32)&mac_p->macMemPool_p->descrPool[0] + sizeof(macMemPool_t));
	mac_p->rxDescrRingBaseVAddr = K0_TO_K1((uint32)&mac_p->macMemPool_p->descrPool[0]);

	mac_p->txDescrRingBaseVAddr	= mac_p->rxDescrRingBaseVAddr + (mac_p->rxDescrSize * mac_p->rxRingSize);

	/* init. tx descriptor, don't allocate memory */
	pTxDescp = (macTxDescr_t*) mac_p->txDescrRingBaseVAddr;
	mac_p->txCurrDescp = pTxDescp;
	
	for (i = 0 ; i < mac_p->txRingSize ; i++, pTxDescp++) {
    	// Init descriptor
    	pTxDescp->tdes0.word = 0;
    	pTxDescp->tdes1.word = 0;
    	pTxDescp->tdes2.word = 0;
    	pTxDescp->tdes3.word = 0;
		mac_p->txskbs[i] = NULL;
	}

	/* init. Rx descriptor, allocate memory for each descriptor */
	pRxDescp = (macRxDescr_t*) mac_p->rxDescrRingBaseVAddr;

	for (i = 0 ; i< mac_p->rxRingSize ; i++, pRxDescp++) {
		// Init Descriptor
		pRxDescp->rdes0.word = 0;
		pRxDescp->rdes1.word = 0;
		pRxDescp->rdes2.word = 0;
		pRxDescp->rdes3.word = 0;
		pRxDescp->rdes4.word = 0;
		pRxDescp->rdes5.word = 0;
		pRxDescp->rdes6.word = 0;
		pRxDescp->rdes7.word = 0;

		// Assign flag
		pRxDescp->rdes0.bits.owner = 1;  /* owned by DMA */
		pRxDescp->rdes1.bits.rx_buf_size = 1776;
		
		if( (skb = alloc_skb(2000)) == NULL ) {
			printf("skb is null\n");
			return 1;
		}

		skb_reserve(skb, 2);
		pRxDescp->rdes2.rxbuf_addr = K1_TO_PHY(skb->data);
		mac_p->rxskbs[i] = skb;
	}

	mac_p->txCurrentDescp     = 0;
	mac_p->txUnReleasedDescp  = 0;
	mac_p->txUnReleasedBufCnt = 0;
	mac_p->rxCurrentDescp     = 0;

	return 0;
}

void macPhyReset(void)
{
	eth_gpioReset();
	miiStationWrite(mac_p->enetPhyAddr, PHY_CONTROL_REG, PHY_RESET );
	pause(100);
	miiStationWrite(mac_p->enetPhyAddr, PHY_CONTROL_REG, PHY_RESTART_AN | PHY_AN_ENABLE);
}

void macIMRMask(void)
{
	uint32 reg = 0;

	reg |= GIMR_RX_DONE | GIMR_RX_FIFO_OVR | GIMR_RX_RING_FULL;
	write_reg_word(CR_GMAC_GIMR, reg);
}

void macSetIntConf(void)
{
	uint32 reg = 0;

	reg = (1<<GPRCTR_RX_PKT_DONE_SHIFT);
	write_reg_word(CR_GMAC_GPRCTR, reg);

	/* RX 32us timeout to interrupt */
	reg = 0x320;
	write_reg_word(CR_GMAC_GRIETR, reg);
}

// Assign Tx Rx Descriptor Control Registers
void macSetDMADescrCtrlReg(macAdapter_t *mac_p)
{
  	write_reg_word(CR_GMAC_GTRBR, K1_TO_PHY(mac_p->txDescrRingBaseVAddr));
  	write_reg_word(CR_GMAC_GRRBR, K1_TO_PHY(mac_p->rxDescrRingBaseVAddr));
	
  	write_reg_word(CR_GMAC_GTRS01R, mac_p->txRingSize);
  	write_reg_word(CR_GMAC_GTRS23R, 0);
  	write_reg_word(CR_GMAC_GTRS45R, 0);
  	write_reg_word(CR_GMAC_GTRS67R, 0);
  	write_reg_word(CR_GMAC_GRRSR, mac_p->rxRingSize);

	/* write tx/rx descriptor size in unit of word */
	write_reg_word(CR_GMAC_GRDLR, mac_p->rxDescrSize>>2);
	write_reg_word(CR_GMAC_GTDLR, mac_p->txDescrSize>>2);

	write_reg_word(CR_GMAC_GRTPR, 0x8000);
	write_reg_word(CR_GMAC_GTTPR(0), 0);
}

void macSetMACCR(macAdapter_t *map_p)
{
	uint32 reg;

	reg = GCTR_TX_FCS | GCTR_FULL_DUPLEX | GCTR_RX_RUNT | 
		GCTR_RX_LONG | GCTR_RX_ALL | GCTR_RX_BCAST | GCTR_RX_MCAST;
	write_reg_word(CR_GMAC_GCTR, reg);

	reg = GSPR_FC_EN | GSPR_CFG_SPD_100M | (3 << GSPR_MTCC_LMT_SHIFT) | 
		(7 << GSPR_CFG_CR_IFG_SHIFT);
	write_reg_word(CR_GMAC_GSPR, reg);

	/* set TX under run register to 1.5KB */
	write_reg_word(CR_GMAC_GTLWR, 0x180);
}

void macSetMacReg(macAdapter_t *mac_p)
{
	write_reg_word(CR_GMAC_GMAR, mac_p->macAddr[0]<<24 | mac_p->macAddr[1]<<16 | \
                               mac_p->macAddr[2]<<8  | mac_p->macAddr[3]<<0);
	write_reg_word(CR_GMAC_GMAR1, mac_p->macAddr[4]<<8  | mac_p->macAddr[5]<<0);
}

int macDrvRegInit(macAdapter_t *mac_p)
{
    // ----- setup interrupt mask ---
    //  ---- Setup HASH table------------------------------------------
    //  -----Setup Interrupt Timer-------------------------------------
    //  -----Setup AUTO polling timer----------------------------------
    //  ---- Setup DMA burst and arbitration---------------------------
    //  -----Setup DMA Descriptor Base Address Assign------------------    
    //  -----Setup MACCR-----------------------------------------------
    
	macResetSwMAC(); 

    macIMRMask();
    
    //macSetHashTable();

    macSetIntConf();

    //macSetAutoPollTmr();

    //macSetupDMABurstArbitration();

    macSetDMADescrCtrlReg(mac_p);

    macSetMACCR(mac_p);
    
    // --- setup MAC address ---
    macSetMacReg(mac_p);      

    return 0;
}

void macDrvStart(void)
{
	uint32 reg;

	reg = read_reg_word(CR_GMAC_GCTR);
	/* enable DMA tx& rx */
	reg |= GCTR_TX_DMA_EN;
	reg |= GCTR_RX_DMA_EN;
	/* enable mac tx& rx */
	reg |= GCTR_TX_EN;
	reg |= GCTR_RX_EN;
	write_reg_word(CR_GMAC_GCTR, reg);
}

int macSetRxMode(void)
{
	return 0;
} /* macSetRxMode */

macTxDescr_t *macTxRingProc(macAdapter_t *mac_p)
{
	volatile macTxDescr_t *pTxDescp;
	sk_buff *freeskb;

	pTxDescp = ((macTxDescr_t*)mac_p->txDescrRingBaseVAddr) + mac_p->txUnReleasedDescp;

  	while (mac_p->txUnReleasedBufCnt != 0) {
	  	if (pTxDescp->tdes0.bits.owner) 
			return 0; // owned by MAC engine, something wrong here!

	  	if (pTxDescp->tdes0.bits.ur_abort || pTxDescp->tdes0.bits.txpkt_ur) { 
			printf("ERR TX tx curr=%d tx=%08x\n", mac_p->txUnReleasedDescp, pTxDescp);
			printf(" tdes0=%08x\n", pTxDescp->tdes0.word);
			printf(" tdes1=%08x\n", pTxDescp->tdes1.word);
			printf(" tdes2=%08x\n", pTxDescp->tdes2.word);
			printf(" tdes3=%08x\n", pTxDescp->tdes3.word);
		}

		freeskb = mac_p->txskbs[mac_p->txUnReleasedDescp];
		free_skb(freeskb);

		pTxDescp->tdes2.txbuf_addr = NULL;
		mac_p->txskbs[mac_p->txUnReleasedDescp] = NULL;

		if (mac_p->txUnReleasedDescp == (mac_p->txRingSize -1))
			mac_p->txUnReleasedDescp = 0;
		else
			mac_p->txUnReleasedDescp++;
		mac_p->txUnReleasedBufCnt--;

		pTxDescp = ((macTxDescr_t*)mac_p->txDescrRingBaseVAddr) + mac_p->txUnReleasedDescp;
	} 

	return (macTxDescr_t*) pTxDescp;
}

void macRxRingProc(void)
{
	volatile macRxDescr_t *rxDescrp;
	uint32 frameSize;
	sk_buff *newMbp, *oldMbp;
	uint32 rdes0_error_check, rdes1_error_check;
	uint32 reg;

	rxDescrp = ((macRxDescr_t*)mac_p->rxDescrRingBaseVAddr) + mac_p->rxCurrentDescp;

	rdes0_error_check = (1<<22|1<<21|1<<20|1<<19|1<<18);
  	rdes1_error_check = (1<<28|1<<27);
  	while (rxDescrp->rdes0.bits.owner == 0) {
	    if ((rxDescrp->rdes0.word & rdes0_error_check) == 0 && 
			(rxDescrp->rdes1.word & rdes1_error_check) == 0 ) {	
	    	// For removing Rx VLAN tag, we need to check if the remain length
	    	// is not smaller than minimun packet length.
	    	// rxDescrp->rdes0.bits.vlan_hit
	    	if (rxDescrp->rdes0.word & (1<<30)) {
	      		frameSize = rxDescrp->rdes0.bits.rx_length;
	      		frameSize = (frameSize > 60) ? frameSize : 60;
	      	} else {
 	 			frameSize = rxDescrp->rdes0.bits.rx_length;
			}
			
	      	if ((frameSize < 60) || (frameSize > 1518)) {
	        	// Discard this packet & Repost this mbuf
				newMbp = mac_p->rxskbs[mac_p->rxCurrentDescp];
	        	goto DISCARD;
	      	}

	      	oldMbp = (sk_buff *)(mac_p->rxskbs[mac_p->rxCurrentDescp]);
	      	oldMbp->len = frameSize;

			newMbp = (sk_buff *)alloc_skb(2000);
			if ( !newMbp ) {	/* faild to allocate more mbuf-> drop this pkt */
				printf("mbuf alloc fails\n");
				newMbp = oldMbp;
				goto RECVOK;
			} else {
				skb_reserve(newMbp, 2);
			}

			eth_rcv(oldMbp);
DISCARD:

RECVOK:

	      	rxDescrp->rdes2.rxbuf_addr = K1_TO_PHY(newMbp->data);
			mac_p->rxskbs[mac_p->rxCurrentDescp] = newMbp;
		} else {
			printf("\t\tERR rx curr=%d rx=%08x\n", mac_p->rxCurrentDescp, rxDescrp);
			printf("\t\t rdes0=%08x\n", rxDescrp->rdes0.word);
			printf("\t\t rdes1=%08x\n", rxDescrp->rdes1.word);
			printf("\t\t rdes2=%08x\n", rxDescrp->rdes2.word);
			printf("\t\t rdes3=%08x\n", rxDescrp->rdes3.word);
		}
	
	    rxDescrp->rdes0.word |= (1<<31);
	
 		/* next descriptor*/
		reg = read_reg_word(CR_GMAC_GRTPR);
		if ( mac_p->rxCurrentDescp == ( mac_p->rxRingSize - 1 )) {
			mac_p->rxCurrentDescp = 0;
			reg = ((reg ^ (1<<15)) & 0x8000) | mac_p->rxCurrentDescp;
		} else {
			mac_p->rxCurrentDescp++;
			reg = (reg & 0x8000) | mac_p->rxCurrentDescp;
		}
		write_reg_word(CR_GMAC_GRTPR, reg);
		/* process one packet */
		write_reg_word(CR_GMAC_GRPPCR, 1);
		
		rxDescrp = ((macRxDescr_t*)mac_p->rxDescrRingBaseVAddr) + mac_p->rxCurrentDescp;
	} /* while loop  */
}

void mac_interrupt(int irq, void *dev_id, struct pt_regs *regs)
{           
	uint32 reg;

	VPint(CR_INTC_IMR) &= ~(1<<MAC_INT);

	reg = read_reg_word(CR_GMAC_GISR);
	write_reg_word(CR_GMAC_GISR, reg & read_reg_word(CR_GMAC_GIMR));

	// ----------Packet Received----------------------
	if (reg & (GIMR_RX_DONE | GIMR_RX_FIFO_OVR | GIMR_RX_RING_FULL)) {
		macRxRingProc();
	} else {
		printf("err mac_interrupt reg=%08x\n", reg);
	}

	VPint(CR_INTC_IMR) |= (1<<MAC_INT);
}

int tc3162_eth_send(sk_buff *skb)
{
	macTxDescr_t *currDescrp = NULL;
	uint32 length = skb->len;
	uint8 *bufAddrp = skb->data;
  	macTxDescr_t *tx; // just for debug only
	uint32 reg;
  	uint32 i;

	if (skb->data == NULL) {
		printf("Tx a empty mbuf\n"); 	
		return 1;
	}
  
    macTxRingProc(mac_p);

	if (mac_p->txUnReleasedBufCnt == mac_p->txRingSize) {
		printf("Tx ring full, drop packet\n"); 	
		free_skb(skb);
		return 1;
	}

	/* ----- Get the transmit descriptor ----- */
	currDescrp = ((macTxDescr_t*) mac_p->txDescrRingBaseVAddr) + mac_p->txCurrentDescp;

	if ((currDescrp->tdes0.word) & (1<<31)) {
		printf("Tx drop packet\n"); 	
		free_skb(skb);
		return 1;
	}

	currDescrp->tdes0.word = 0;
  	currDescrp->tdes1.word = (length & 0x000007ff);
  	currDescrp->tdes2.txbuf_addr = K1_TO_PHY(skb->data);
	mac_p->txskbs[mac_p->txCurrentDescp] = skb;

	currDescrp->tdes0.word |= (1<<31);

	reg = read_reg_word(CR_GMAC_GTTPR(0));
	if( mac_p->txCurrentDescp == ( mac_p->txRingSize - 1 )) {
		mac_p->txCurrentDescp = 0;
		reg = ((reg ^ (1<<15)) & 0x8000) | mac_p->txCurrentDescp;
	} else {
		mac_p->txCurrentDescp ++;
		reg = (reg & 0x8000) | mac_p->txCurrentDescp;
	}
	write_reg_word(CR_GMAC_GTTPR(0), reg);

	mac_p->txUnReleasedBufCnt++;
	return 0;
}
#if 0
int32 macSearchPhyAddr(macAdapter_t *mac_p)
{
	uint32 miiReg = 0;
	uint32 phyAddr;

	mac_p->enetPhyAddr = 0;
	for ( mac_p->enetPhyAddr = 0; mac_p->enetPhyAddr < 32; mac_p->enetPhyAddr++ ) {
		miiReg = miiStationRead(mac_p->enetPhyAddr, PHY_ID_REG1);
		if (miiReg == 0)
			miiReg = miiStationRead(mac_p->enetPhyAddr, PHY_ID_REG2);
		if ((miiReg != 0) && (miiReg != 0xffff))
			return 0;
	}

	mac_p->enetPhyAddr = 0;

	return -1;
}
#endif
struct irqaction mac_irq = {mac_interrupt, NULL, 0, "mac", NULL, NULL};

#ifdef ETH_TEST_CMD
int tc3162_eth_testsend(unsigned long len, unsigned long cnt)
{
	unsigned long i;
	unsigned long k;

	for (i = 0; i < cnt; i++) {
		sk_buff *skb;
		int j;

		if( (skb = alloc_skb(2000)) == NULL ) {
			printf("skb is null\n");
			continue;
		}
		for (k = 0; k < len; k++)
			skb->data[k] = k;
		skb->len = len;
		tc3162_eth_send(skb);
	}

	return 0;
}

int tc3162_eth_testsend2(unsigned long len, unsigned long cnt)
{
	unsigned long i;
	unsigned long k;

	for (i = 0; i < cnt; i++) {
		sk_buff *skb;
		int j;

		if( (skb = alloc_skb(2000)) == NULL ) {
			printf("skb is null\n");
			continue;
		}
		/*
		for (k = 0; k < len; k++)
			skb->data[k] = i;
			*/
		skb->data[0]= 0x1;
		skb->len = len;
		tc3162_eth_send(skb);
	}

	return 0;
}

int tc3162_eth_dump(void)
{
	macRxDescr_t *pRxDescp;
  	macTxDescr_t *pTxDescp;
  	uint32 i;

	printf("TX Descriptor Ring\n");
	pTxDescp = (macTxDescr_t*) mac_p->txDescrRingBaseVAddr;
	for (i = 0 ; i < mac_p->txRingSize ; i++, pTxDescp++) {
		printf("i= %d descr=%08x\n", i, pTxDescp);
		printf(" tdes0=%08x\n", pTxDescp->tdes0.word);
		printf(" tdes1=%08x\n", pTxDescp->tdes1.word);
		printf(" tdes2=%08x\n", pTxDescp->tdes2.word);
		printf(" tdes3=%08x\n", pTxDescp->tdes3.word);
		printf(" skb  =%08x\n", pTxDescp->skb);
	}
	printf("txCurrentDescp    =%d\n", mac_p->txCurrentDescp);
	printf("txUnReleasedDescp =%d\n", mac_p->txUnReleasedDescp);
	printf("txUnReleasedBufCnt=%d\n", mac_p->txUnReleasedBufCnt);
	printf("CR_GMAC_GTTPR(0)  =%08x\n", VPint(CR_GMAC_GTTPR(0)));
	printf("CR_GMAC_GTHPR(0)  =%08x\n", VPint(CR_GMAC_GTHPR(0)));

	printf("RX Descriptor Ring\n");
	pRxDescp = (macRxDescr_t*) mac_p->rxDescrRingBaseVAddr;
	for( i = 0 ; i< mac_p->rxRingSize ; i++, pRxDescp++) {
		printf("i= %d descr=%08x\n", i, pRxDescp);
		printf(" rdes0=%08x\n", pRxDescp->rdes0.word);
		printf(" rdes1=%08x\n", pRxDescp->rdes1.word);
		printf(" rdes2=%08x\n", pRxDescp->rdes2.word);
		printf(" rdes3=%08x\n", pRxDescp->rdes3.word);
		printf(" rdes3=%08x\n", pRxDescp->skb);
	}
	printf("rxCurrentDescp    =%d\n", mac_p->rxCurrentDescp);
	printf("CR_GMAC_GRTPR     =%08x\n", VPint(CR_GMAC_GRTPR));
	printf("CR_GMAC_GRHPR     =%08x\n", VPint(CR_GMAC_GRHPR));

	return 0;
}

#endif
#if 0 /*move into tcswitch.c*/
int macSetUpPhy(macAdapter_t *mac_p)
{
	/* detect if RTL8305 or RTL8306 Switch*/	
	if ((miiStationRead(4, 2) == 0x1c) && (miiStationRead(4, 3) == 0xc852)) {
		if ((miiStationRead(6, 2) == 0x1c) && (miiStationRead(6, 3) == 0xc852) ){
    			/*swicVendor = SWIC_RTL8306SD;Let CPU Port Link up*/
    			miiStationWrite(5, 22,miiStationRead(5,22)|0x8000);
    			miiStationWrite(6, 22,miiStationRead(6,22)|0x8000);    		
        	}
	}
  return 0;
}/*end macSetUpPhy*/
#endif 
int tc3162_eth_init(unsigned char *mac_addr)
{
	int i;
	uint8 k;
	
	macControllerStop(mac_p);
	
	mac_p->macMemPool_p = &enetMemPool;

	/* ----- Set up the paramters ----- */
	macDefaultParaSet(mac_p);

 	macGetMacAddr(mac_p, mac_addr);
	macDrvDescripInit(mac_p);

	macDrvRegInit(mac_p);
	macPhyReset();
	pause(100);

	macSearchPhyAddr(mac_p);
	printf("Search PHY addr and found PHY addr=%d\n", mac_p->enetPhyAddr);
	if (macSetUpPhy(mac_p)){
		return -1;
	}

	request_IRQ(MAC_INT, &mac_irq, NULL);
	macDrvStart();

	macSetRxMode();

	return 0;
}

int tc3162_eth_exit(void)
{
	return 0;
}

