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
#include "tc3162l2mac.h"

#define printf prom_printf

static macAdapter_t DummyAdapter;
macAdapter_t *mac_p = &DummyAdapter;
static macMemPool_t enetMemPool __attribute__ ((__aligned__(16)));

#define read_reg_word(reg) VPint(reg);
#define write_reg_word(reg, wdata) VPint(reg)=wdata;

extern void pause(int ms);

PUBLIC void 
miiStationWrite(
	uint32 enetPhyAddr, 
	uint32 phyReg,
	uint32 miiData
)
{
  	uint32 cnt=1000;
	uint32 reg=0;
	macPhyWDataReg_t     macPHYWData;

	macPHYWData.bits.miiwdata = miiData & 0x0000ffff;
	write_reg_word (CR_MAC_PHYWDATA, macPHYWData.value);

	if(isTC3162U || isRT63260){
		/*New format to access mii register*/	
		reg=((phyReg & 0x1f)<<NEW_PHYOR_REG_SHIFT) |((enetPhyAddr & 0x1f) <<NEW_PHYOR_ADDR_SHIFT)\
		|(NEW_PHYOR_OP_WR<<NEW_PHYOR_OP_SHIFT) |(NEW_PHYOR_ST_IEEE_OLD <<NEW_PHYOR_ST_SHIFT);
		write_reg_word (CR_MAC_PHYCR, reg);
		do{
	 	   reg=read_reg_word (CR_MAC_PHYCR);
		   cnt--;
		}while ((reg&NEW_PHYOR_BUSY)&&cnt>0);
	}
	else{
		/*Old format to accress mii register*/
		reg=((phyReg & 0x1f)<<PHYOR_REG_SHIFT) |((enetPhyAddr & 0x1f) <<PHYOR_ADDR_SHIFT) \
		|PHYOR_WRITE;
		write_reg_word (CR_MAC_PHYCR, reg);
		do{
	 	   reg=read_reg_word (CR_MAC_PHYCR);
		   cnt--;
		}while ((reg & PHYOR_WRITE)&&cnt>0);
	}
	
	if(cnt==0){
		printf("Error:miiStationWrite Timeout!");
	}
}

PUBLIC uint32 
miiStationRead(
	uint32 enetPhyAddr,
	uint32 phyReg
)
{
   	uint32 reg=0;
	uint32 cnt=1000;

	if(isTC3162U || isRT63260){
		/*New format to access mii register*/	
		reg=((phyReg & 0x1f)<<NEW_PHYOR_REG_SHIFT) |((enetPhyAddr & 0x1f) <<NEW_PHYOR_ADDR_SHIFT)\
		|(NEW_PHYOR_OP_RD<<NEW_PHYOR_OP_SHIFT) |(NEW_PHYOR_ST_IEEE_OLD <<NEW_PHYOR_ST_SHIFT);
		write_reg_word (CR_MAC_PHYCR, reg);
		do{
	 	   reg=read_reg_word (CR_MAC_PHYCR);
		   cnt--;
		}while ((reg&NEW_PHYOR_BUSY)&&cnt>0);
	}
	else{
		/*Old format to accress mii register*/
		reg=((phyReg & 0x1f)<<PHYOR_REG_SHIFT) |((enetPhyAddr & 0x1f) <<PHYOR_ADDR_SHIFT) \
		|PHYOR_READ;
		write_reg_word (CR_MAC_PHYCR, reg);
		do{
		    reg=read_reg_word (CR_MAC_PHYCR);
		    cnt--;
		}while ((reg&PHYOR_READ)&&cnt>0);
	}
	
	if(cnt==0){
		printf("Error:miiStationRead Timeout!");
	}

	return (reg & 0xffff);
}

void macResetSwMAC(void)
{
	macCtrlReg_t	macCtrl;

	// Disable DMA Enable
	macCtrl.value = 0;
	write_reg_word (CR_MAC_MACCR, macCtrl.value);
	pause(2);

	// Reset Mac
//	macCtrl.value = 0;
	macCtrl.bits.sw_rst = 1;
	write_reg_word (CR_MAC_MACCR, macCtrl.value);
	pause(2);
}


#if defined(TCSUPPORT_MULTI_UPGRADE) || defined(TCSUPPORT_MULTI_BOOT)
static void pause2(int ms)
{
	unsigned int i;
	for (i = 0; i < ms * 100000; i++)
		;
}


void resetSwMAC(void)
{
	macCtrlReg_t	macCtrl;

	// Disable DMA Enable
	macCtrl.value = 0;
	write_reg_word (CR_MAC_MACCR, macCtrl.value);
	pause2(2);

	// Reset Mac
	macCtrl.bits.sw_rst = 1;
	write_reg_word (CR_MAC_MACCR, macCtrl.value);

	pause2(2);	
}
#endif


PUBLIC void
macControllerStop(macAdapter_t *mac_p)
{
	macCtrlReg_t macCtrl;

	macCtrl.value = VPint(CR_MAC_MACCR);
	/* disable DMA tx& rx */
	macCtrl.bits.rdma_en = 0;
	macCtrl.bits.xdma_en = 0;
	/* disable mac tx& rx */
	macCtrl.bits.rcv_en = 0;
	macCtrl.bits.xmt_en = 0;
	VPint(CR_MAC_MACCR) = macCtrl.value;
}

PUBLIC void
macDefaultParaSet
(
	macAdapter_t *mac_p
)
{
	mac_p->rxDescrSize = MAC_RXDESCP_SIZE;
	mac_p->txDescrSize = MAC_TXDESCP_SIZE;
	mac_p->rxRingSize  = MAC_RXDESCP_NO;
	mac_p->txRingSize  = MAC_TXDESCP_NO;
}

PUBLIC void 
macGetMacAddr(
	macAdapter_t *mac_p, unsigned char *macAddr
)
{
	int i;

	for (i = 0; i < 6; i++)
		mac_p->macAddr[i] = macAddr[i];
}

PRIVATE BOOLEAN
macDrvDescripInit
(
	macAdapter_t *mac_p
)
{
	macRxDescr_t *pRxDescp;
  	macTxDescr_t *pTxDescp;
  	uint32 physicalAddr;
  	uint32 i;
	sk_buff *skb;

	mac_p->txDescrRingBaseVAddr = K0_TO_K1((uint32)&mac_p->macMemPool_p->descrPool[0]);

	mac_p->rxDescrRingBaseVAddr	= mac_p->txDescrRingBaseVAddr + (mac_p->txDescrSize * mac_p->txRingSize);

	/* init. tx descriptor, don't allocate memory */
	pTxDescp = (macTxDescr_t*) mac_p->txDescrRingBaseVAddr;
	mac_p->txCurrDescp = pTxDescp;
	
	for ( i = 0 ; i < mac_p->txRingSize ; i++, pTxDescp++ )
	{
    	// Init descriptor
    	pTxDescp->tdes0.word = 0;
    	pTxDescp->tdes1.word = 0;
    	pTxDescp->tdes2.word = 0;

    	// Assign flag
    	pTxDescp->tdes0.bits.ownerDma = 0;  /* owned by CPU */
    	pTxDescp->tdes1.bits.sw_txtag_en = 0; /* SW Tag */
    	pTxDescp->tdes1.bits.ipcs_en     = 0; /* IP Chksum */
    	pTxDescp->tdes1.bits.vtag_en     = 0; /* VLAN Tag */
    	pTxDescp->skb = NULL;
	}

	/* init. Rx descriptor, allocate memory for each descriptor */
	pRxDescp = (macRxDescr_t*) mac_p->rxDescrRingBaseVAddr;

	for( i = 0 ; i< mac_p->rxRingSize ; i++, pRxDescp++)
	{
		// Init Descriptor
		pRxDescp->rdes0.word = 0;
		pRxDescp->rdes1.word = 0;
		pRxDescp->rdes2.word = 0;

		// Assign flag
		pRxDescp->rdes0.bits.ownerDma = 1;  /* owned by DMA */
		pRxDescp->rdes1.bits.rxBufSize = 1776;
		
		if( (skb = alloc_skb(2000)) == NULL ){
			printf("skb is null\n");
		}

		skb_reserve(skb, 2);
		pRxDescp->rdes2.buf_p = K1_TO_PHY(skb->data);
		pRxDescp->skb = skb;
	}

	mac_p->txCurrentDescp     = 0;
	mac_p->txUnReleasedDescp  = 0;
	mac_p->txUnReleasedBufCnt = 0;
	mac_p->rxCurrentDescp     = 0;

	return TRUE;
}

void
macPhyReset(
	void
)
{
	if (isTC3162U) {
		VPint(CR_AHB_SSR) |= (1<<11)|(1<<10)|(1<<9)|(1<<8);
		pause(10);
	}else if(isRT63260){
		VPint(CR_AHB_SSR) |= (1<<24);
		pause(10);
	}
	eth_gpioReset();
	miiStationWrite(mac_p->enetPhyAddr, PHY_CONTROL_REG, PHY_RESET );
	miiStationWrite(mac_p->enetPhyAddr, PHY_CONTROL_REG, MIIDR_AUTO_NEGOTIATE );
}

PRIVATE void macIMRMask(void)
{
	macIntrMaskReg_t macIntrMask;
	macIntrMask.value = 0;
	macIntrMask.bits.rpkt_finish = 1;
#if 0	
    macIntrMask.bits.rpkt_ok_by_thrhold = 0;
    macIntrMask.bits.rpkt_ok_by_timeout = 0;
    macIntrMask.bits.xpkt_ok_by_thrhold = 0;
    macIntrMask.bits.xpkt_ok_by_timeout = 0;
  	macIntrMask.bits.norxbuf	 = 1;
  	macIntrMask.bits.notxbuf	 = 1;
  	macIntrMask.bits.xpkt_finish = 1;
  	macIntrMask.bits.xpkt_lost	 = 1;
  	macIntrMask.bits.rpkt_lost	 = 1;
  	macIntrMask.bits.xpkt_save   = 1;
  	macIntrMask.bits.rpkt_save   = 1;
  	macIntrMask.bits.ahb_err	 = 1;
  	macIntrMask.bits.physts_chg  = 1;
#endif	
	write_reg_word(CR_MAC_IMR, macIntrMask.value);
}

PRIVATE void macSetIntTimer(void)
{
	mac_intr_timer_ctrl_reg_t macIntrTimerCtrl;

    macIntrTimerCtrl.value = 0;
	write_reg_word (CR_MAC_ITC, macIntrTimerCtrl.value);
}

// Assign Tx Rx Descriptor Control Registers
PRIVATE void macSetDMADescrCtrlReg(macAdapter_t *mac_p)
{
  	write_reg_word(CR_MAC_TXR_BADR, K1_TO_PHY( mac_p->txDescrRingBaseVAddr));
  	write_reg_word(CR_MAC_RXR_BADR, K1_TO_PHY( mac_p->rxDescrRingBaseVAddr));
	
  	VPint(CR_MAC_TXR_SIZE) = mac_p->txRingSize;
  	VPint(CR_MAC_RXR_SIZE) = mac_p->rxRingSize;
  	VPint(CR_MAC_TXDESP_SIZE) = mac_p->txDescrSize;
  	VPint(CR_MAC_RXDESP_SIZE) = mac_p->rxDescrSize;
}

PRIVATE void macSetMACCR
(
  macAdapter_t *map_p
)
{
  macCtrlReg_t macCtrl;

  // --- configure MAC ---
  // clear reg
//macCtrl.value = 0x3df23;
  macCtrl.value = 0x0;
  // set flag
  macCtrl.bits.rx_broadpkt = 1;
  macCtrl.bits.rx_multipkt = 1;

  macCtrl.bits.fulltx  = 1;
  macCtrl.bits.crc_apd = 1;
  macCtrl.bits.mdc_sel = 0;
  macCtrl.bits.rcv_all = 1;

  macCtrl.bits.rx_ftl      = 1;
  macCtrl.bits.rx_runt     = 1;
  macCtrl.bits.ht_multi_en = 1;
  macCtrl.bits.rcv_en      = 0;

  macCtrl.bits.l2_mode        = 1;
  macCtrl.bits.enrx_in_halftx = 0;
  macCtrl.bits.xmt_en       	= 0;
  macCtrl.bits.crc_dis       	= 0;

  macCtrl.bits.loop_en = 0;
  macCtrl.bits.sw_rst  = 0;
  macCtrl.bits.rdma_en = 0;
  macCtrl.bits.xdma_en = 0;

  macCtrl.bits.txfifo_thr = 0x0e;
  macCtrl.bits.rx_untag   = 0;
  macCtrl.bits.drop_local = 0;

  write_reg_word (CR_MAC_MACCR, macCtrl.value);
}

PRIVATE void macSetMacReg
(
  macAdapter_t *mac_p
)
{
	write_reg_word(CR_MAC_MADR,mac_p->macAddr[0]<<8  | mac_p->macAddr[1]<<0);
	write_reg_word(CR_MAC_LADR,mac_p->macAddr[2]<<24 | mac_p->macAddr[3]<<16 | \
                               mac_p->macAddr[4]<<8  | mac_p->macAddr[5]<<0);
}

PUBLIC int
macDrvRegInit(
	macAdapter_t *mac_p
)
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

    macSetIntTimer();

    //macSetAutoPollTmr();

    //macSetupDMABurstArbitration();

    macSetDMADescrCtrlReg(mac_p);

    macSetMACCR(mac_p);
    
    // --- setup MAC address ---
    macSetMacReg(mac_p);      

    return 0;
}

PUBLIC void
macDrvStart
(
	void
)
{
    macCtrlReg_t macCtrl;

	macCtrl.value = VPint(CR_MAC_MACCR);
	/* enable DMA tx& rx */
	macCtrl.bits.rdma_en = 1;
	macCtrl.bits.xdma_en = 1;
	/* enable mac tx& rx */
	macCtrl.bits.rcv_en = 1;
	macCtrl.bits.xmt_en = 1;
	VPint(CR_MAC_MACCR) = macCtrl.value;
}

PUBLIC int 
macSetRxMode(
	void
)
{
	macCtrlReg_t mac_cr;
	  
	mac_cr.value=tc_inl(CR_MAC_MACCR);			/* Accept all packet */
	mac_cr.bits.rcv_all=0;
	mac_cr.bits.ht_multi_en=0;
	mac_cr.bits.rx_broadpkt=1;
	mac_cr.bits.rcv_en=1;
	tc_outl(CR_MAC_MACCR, mac_cr.value);

	return 0;
} /* macSetRxMode */

PRIVATE macTxDescr_t*
macTxRingProc
(
	macAdapter_t *mac_p
)
{
  	uint32 i;
	volatile macTxDescr_t *pTxDescp;

	pTxDescp = ((macTxDescr_t*)mac_p->txDescrRingBaseVAddr) + mac_p->txUnReleasedDescp;

  	while ( mac_p->txUnReleasedBufCnt != 0 )
	{
	  	if ( (pTxDescp->tdes0.word)&(1<<31) )
		{
			return 0; // owned by MAC engine, something wrong here!
		}

		free_skb(pTxDescp->skb);

		pTxDescp->tdes2.buf_p = NULL;
		pTxDescp->skb = NULL;

		if ( mac_p->txUnReleasedDescp == ( mac_p->txRingSize - 1) )
		{
			mac_p->txUnReleasedDescp = 0;
		}
		else
		{
			mac_p->txUnReleasedDescp++;
		}
		mac_p->txUnReleasedBufCnt--;

		pTxDescp = ((macTxDescr_t*)mac_p->txDescrRingBaseVAddr) + mac_p->txUnReleasedDescp;
	} // while

	return (macTxDescr_t*)pTxDescp;
}

PUBLIC void
macRxRingProc
(
	void
)
{
	volatile macRxDescr_t *rxDescrp;
	uint32 frameSize;
	sk_buff *newMbp, *oldMbp;
	uint32 procFrameCnt = 0;
	uint32 rdes0_error_check, rdes1_error_check;

	uint32 i = 0;
	uint8* pdata;

	rxDescrp = ((macRxDescr_t*)mac_p->rxDescrRingBaseVAddr) + mac_p->rxCurrentDescp;

	rdes0_error_check = (1<<27|1<<22|1<<21|1<<20|1<<19|1<<18);
  	rdes1_error_check = 0;
  	while ((rxDescrp->rdes0.word & (1<<31)) == 0) // ownerDMA for TCMAC
  	{

	    if ( (rxDescrp->rdes0.word & rdes0_error_check) == 0 &&
        	(rxDescrp->rdes1.word & rdes1_error_check) == 0 )
	    {	
	    	// For removing Rx VLAN tag, we need to check if the remain length
	    	// is not smaller than minimun packet length.
	    	// rxDescrp->rdes0.bits.vlan_hit
	    	if (rxDescrp->rdes0.word & (1<<30))
	    	{
	      		frameSize = rxDescrp->rdes0.bits.rfl;
	      		frameSize = (frameSize > 60) ? frameSize : 60;
	      	}
	 		else
 	 			frameSize = rxDescrp->rdes0.bits.rfl;
			
	      	/* lino: make VLAN friendly */
	      	if ( (frameSize < 60) || (frameSize > 1518) )
	      	{
	        	// Discard this packet & Repost this mbuf
	        	(volatile sk_buff *)newMbp = (volatile sk_buff *)(rxDescrp->skb);
	        	goto DISCARD;
	      	}

	      	(volatile sk_buff *)oldMbp = (volatile sk_buff *)(rxDescrp->skb);
	      	oldMbp->len = frameSize;
			
			newMbp = (sk_buff *)alloc_skb(2000);
			if ( !newMbp ) {	/* faild to allocate more mbuf-> drop this pkt */
				newMbp = oldMbp;
				goto RECVOK;
			}
			skb_reserve(newMbp, 2);

			eth_rcv(oldMbp);

DISCARD:

RECVOK:

	      	rxDescrp->rdes2.buf_p = K1_TO_PHY(newMbp->data);
			rxDescrp->skb = newMbp;
		} else {
			prom_printf("error\n");
			prom_printf("rdes0=%08x\n", rxDescrp->rdes0.word);
			prom_printf("rdes1=%08x\n", rxDescrp->rdes1.word);
			prom_printf("rdes2=%08x\n", rxDescrp->rdes2.word);
			prom_printf("rdes3=%08x\n", rxDescrp->rdes3.word);
		}
	
	    rxDescrp->rdes0.word |= (1<<31);
	
 		/* next descriptor*/
		if ( mac_p->rxCurrentDescp == ( mac_p->rxRingSize - 1 ))
			mac_p->rxCurrentDescp = 0;
		else
			mac_p->rxCurrentDescp++;
		
		rxDescrp = ((macRxDescr_t*)mac_p->rxDescrRingBaseVAddr) + mac_p->rxCurrentDescp;
	} /* while loop  */

    VPint(CR_MAC_RXR_SWIDX) = mac_p->rxCurrentDescp;
}

void mac_interrupt(int irq, void *dev_id, struct pt_regs *regs)
{           
	macIntrStatusReg_t isr;

	VPint(CR_INTC_IMR) &= ~(1<<MAC_INT);
	// -------Read MACCR--------------------------------
	isr.value=tc_inl(CR_MAC_ISR);

#if 0
	if(isr.bits.physts_chg	)prom_printf("\r\n--->PHYSTS_CHG  ");
	if(isr.bits.ahb_err 	)prom_printf("\r\n--->AHB_ERR	  ");
	if(isr.bits.rpkt_lost	)prom_printf("\r\n--->RPKT_LOST   ");
//	if(isr.bits.rpkt_save	)prom_printf("\r\n--->RPKT_SAVE   ");
	if(isr.bits.xpkt_lost	)prom_printf("\r\n--->XPKT_LOST   ");
//	if(isr.bits.xpkt_finish )prom_printf("\r\n--->XPKT_FINISH ");
	if(isr.bits.notxbuf 	)prom_printf("\r\n--->NOTXBUF	  ");
//	if(isr.bits.xpkt_save	)prom_printf("\r\n--->XPKT_SAVE   ");
	if(isr.bits.norxbuf 	)prom_printf("\r\n--->NORXBUF	  ");
	if(isr.bits.rpkt_finish )prom_printf("\r\n--->RPKT_FINISH ");
#endif
	
	// ----------Packet Received----------------------
	if( isr.bits.rpkt_finish)
	{
		macRxRingProc();
	}
SKIP:	
	VPint(CR_INTC_IMR) |= (1<<MAC_INT);
	MAC_RX_POLL_DEMAND_(1);
}

int tc3162_eth_send(sk_buff *skb)
{
	macTxDescr_t *currDescrp = NULL;
	uint32 length = skb->len;
	uint8 *bufAddrp = skb->data;
  	macTxDescr_t *tx; // just for debug only
  	uint32 i;

	if( skb->data == NULL ){
		printf("\n\rTx a empty mbuf"); 	
	}
  
    macTxRingProc(mac_p);

	if ( mac_p->txUnReleasedBufCnt == mac_p->txRingSize )
	{
		free_skb(skb);
		return 1;
	}

	/* ----- Get the transmit descriptor ----- */
	currDescrp = ((macTxDescr_t*)mac_p->txDescrRingBaseVAddr) + mac_p->txCurrentDescp;

	if( (currDescrp->tdes0.word)&(1<<31) )
	{
		free_skb(skb);
		return 1;
	}

	// tx buffer size
	currDescrp->tdes0.word = 0;
  	/* clear all except edotr, fts, lts */
  	currDescrp->tdes1.word &= (1<<31|1<<28|1<<27);
  	currDescrp->tdes1.word |= (length&0x000007ff);

  	currDescrp->tdes2.buf_p = K1_TO_PHY(skb->data);
	currDescrp->skb = skb;

	currDescrp->tdes0.word |= (1<<31);

	if( mac_p->txCurrentDescp == ( mac_p->txRingSize - 1 ))
	{
		mac_p->txCurrentDescp = 0;
	}
	else
	{
		mac_p->txCurrentDescp ++;
	}

	mac_p->txUnReleasedBufCnt++;

  	MAC_TX_POLL_DEMAND_(1);

	return 0;
}

struct irqaction mac_irq = {mac_interrupt, NULL, 0, "mac", NULL, NULL};


int tc3162_eth_init(unsigned char *mac_addr)
{
	int i;
	
	macControllerStop(mac_p);
	
	mac_p->macMemPool_p = &enetMemPool;
	mac_p->enetPhyAddr = 0;

	/* ----- Set up the paramters ----- */
	macDefaultParaSet(mac_p);

 	macGetMacAddr(mac_p, mac_addr);
	macDrvDescripInit(mac_p);
 	macPhyReset();
	pause(100);

	macSearchPhyAddr(mac_p);
	/* ----- Initialize the SEEQ 80225 phy chip(enet phy) ----- */
	if (macSetUpPhy(mac_p)){
		return -1;
	}
	
	macDrvRegInit(mac_p);

	request_IRQ(MAC_INT, &mac_irq, NULL);
	macDrvStart();

	macSetRxMode();

	return 0;
}

int tc3162_eth_exit(void)
{
	return 0;
}
