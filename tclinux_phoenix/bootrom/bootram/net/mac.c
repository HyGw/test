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
#include "mac.h"

#define printf prom_printf

static macAdapter_t DummyAdapter;
static macAdapter_t *mac_p = &DummyAdapter;
static macMemPool_t enetMemPool __attribute__ ((__aligned__(16)));

static void pause(int ms)
{
	unsigned int i;
	for (i = 0; i < ms * 100000; i++)
		;
}

PUBLIC void 
miiStationWrite(
	uint32 enetPhyAddr, 
	uint32 phyReg,
	uint32 miiData
)
{
	uint32 cnt=1000;
 	macPhyCtrlReg_t      macPHYCtrl;
	macPhyWDataReg_t     macPHYWData;

	if(phyReg>24){
		printf("ERROR:miiStationWrite() phyReg is out of range(16)");
	}

	macPHYWData.bits.miiwdata = miiData & 0x0000ffff;
	tc_outl(CR_MAC_PHYWDATA, macPHYWData.value);

	macPHYCtrl.value = 0;
	macPHYCtrl.bits.phyad = enetPhyAddr & 0x1f;
	macPHYCtrl.bits.regad = phyReg & 0x1f;
	macPHYCtrl.bits.miird = 0;
	macPHYCtrl.bits.miiwr = 1; 
	tc_outl(CR_MAC_PHYCR, macPHYCtrl.value);

	do
	{
	   pause(1);
 	   macPHYCtrl.value=tc_inl(CR_MAC_PHYCR);
	   cnt--;
	} while (macPHYCtrl.bits.miiwr==1&&cnt>0);

	if(cnt==0){
		printf("Error:miiStationWrite Timeout!\n");
	}
}

PUBLIC uint32 
miiStationRead(
	uint32 enetPhyAddr,
	uint32 phyReg
)
{
	macPhyCtrlReg_t   macPHYCtrl;
	uint32 cnt=1000;

	if(phyReg>24){
		printf("ERROR:miiStationWrite() phyReg is out of range(16)\n");
	}

	 macPHYCtrl.value = 0;
	 macPHYCtrl.bits.phyad = enetPhyAddr & 0x1f;
	 macPHYCtrl.bits.regad = phyReg & 0x1f;
	 macPHYCtrl.bits.miird = 1;
	 macPHYCtrl.bits.miiwr = 0;

	 tc_outl(CR_MAC_PHYCR, macPHYCtrl.value);

	 do
	 {
          pause(1);
	      macPHYCtrl.value=tc_inl(CR_MAC_PHYCR);
	      cnt--;
		
	 } while (macPHYCtrl.bits.miird==1&&cnt>0);

	if(cnt==0){
		printf("Error:miiStationRead Timeout!\n");
	}

	return macPHYCtrl.bits.miirdata & 0x0000ffff;
}

PRIVATE uint32 byte_swap(uint32 x)
{
	volatile uint32 tmp=0;
	tmp|=(x&0x000000FF)<<24;
	tmp|=(x&0x0000FF00)<<8;
	tmp|=(x&0x00FF0000)>>8;
	tmp|=(x&0xFF000000)>>24;
	return tmp;
}

PRIVATE void macSetHashTable(void)
{	  
	 // --- setup HASH table ---
	tc_outl(CR_MAC_MAHT0, 0); 
	tc_outl(CR_MAC_MAHT1, 0);
}

PRIVATE void macIMRMask(void)
{
	
	macIntrMaskReg_t macIntrMask;

	macIntrMask.value       = 0;
#if 1
	macIntrMask.bits.rpkt_finish = 1;
	macIntrMask.bits.norxbuf	 = 0;
	macIntrMask.bits.notxbuf	 = 0;
	macIntrMask.bits.xpkt_finish = 0;
	macIntrMask.bits.xpkt_lost	 = 0;
	macIntrMask.bits.rpkt_lost	 = 0;
	macIntrMask.bits.xpkt_save   = 0;
	macIntrMask.bits.rpkt_save   = 0;
	macIntrMask.bits.ahb_err	 = 0;
	macIntrMask.bits.physts_chg  = 0;
#else
	macIntrMask.bits.rpkt_finish = 1;
	macIntrMask.bits.norxbuf     = 1;
	macIntrMask.bits.notxbuf     = 1;
	macIntrMask.bits.xpkt_finish = 1;
	macIntrMask.bits.xpkt_lost   = 1;
	macIntrMask.bits.rpkt_lost   = 1;
	macIntrMask.bits.xpkt_save =1;
	macIntrMask.bits.rpkt_save =1;
	macIntrMask.bits.ahb_err     = 1;
	macIntrMask.bits.physts_chg  = 1;
#endif
	tc_outl(CR_MAC_IMR, macIntrMask.value);
}

void macResetSwMAC(void)
{
	macCtrlReg_t	macCtrl;

	// Disable DMA Enable
	macCtrl.value   = 0;
	tc_outl(CR_MAC_MACCR, macCtrl.value);
	pause(2);

	// Reset Mac
	macCtrl.value   = 0;
	macCtrl.bits.sw_rst  = 1;
	tc_outl(CR_MAC_MACCR, macCtrl.value);
	pause(2);
}

PRIVATE macConfigPHYAdReg(macAdapter_t *mac_p)
{
	macPhyAdvertismentReg_t   phyAdvert;
	
	phyAdvert.value                 = 0;
	phyAdvert.bits.selector_field        = 0x01;
	phyAdvert.bits.half_duplex_10baseT   = 1;      // 10Mbps with half duplex
	phyAdvert.bits.full_duplex_10baseT   = 1;      // 10Mbps with full duplex
	phyAdvert.bits.half_duplex_100baseT  = 1;      // 100Mbps with half duplex
	phyAdvert.bits.full_duplex_100baseT  = 1;      // 100Mbps with full duplex
	phyAdvert.bits.flow_control          = 1;      // flow-control capable
	miiStationWrite( mac_p->enetPhyAddr, 0x4, phyAdvert.value );
}

PRIVATE void macConfigPHYControlReg(macAdapter_t *mac_p)
{
	macPhyCR_t phyCtrl;
	
	phyCtrl.value         = 0;
	phyCtrl.bits.duplex_mode   = 1;      // work in full duplex
	phyCtrl.bits.reset_ANEN    = 1;      // restart Auto-negotiation
	phyCtrl.bits.enable_ANEN   = 1;      // enable Auto-negotiation
	phyCtrl.bits.speed_select  = 1;      // work in 100Mbps
	phyCtrl.bits.loopback      = 0;
	miiStationWrite( mac_p->enetPhyAddr, 0x0, phyCtrl.value );	
}

PRIVATE void macResetSwPHY(macAdapter_t *mac_p)
{
	macPhyCR_t phyCtrl;
	
	phyCtrl.value         	= 0;
	phyCtrl.bits.phy_reset  = 1;
	miiStationWrite( mac_p->enetPhyAddr, 0x0, phyCtrl.value );
	
	pause(2);	
}
PRIVATE void macSetIntTimer(void)
{
	mac_intr_timer_ctrl_reg_t macIntrTimerCtrl;	

	macIntrTimerCtrl.value = 0;
	tc_outl(CR_MAC_ITC, macIntrTimerCtrl.value);
}

PRIVATE void macSetAutoPollTmr(void)
{
    // --- setup AUTO polling timer ---
    mac_autopoll_timer_ctrl_reg_t macAutoPollTimer;
   
    // not to auto polling
    macAutoPollTimer.value            = 0;    
    // macAutoPollTimer.value            = 0x1018;    
    tc_outl(CR_MAC_APTC, macAutoPollTimer.value);    
}

PRIVATE void macSetupDMABurstArbitration(void)
{
	
    mac_dma_arb_ctrl_reg_t macDMA;
    	
    // --- setup DMA burst and arbitration ---
    macDMA.value    = 0;   
	macDMA.bits.rx_thr_en = 1;
	macDMA.bits.rxfifo_thr = 2;
    tc_outl(CR_MAC_DBLAC, macDMA.value);
}

// Assign Tx Rx Descriptor Base

PRIVATE void macSetDMADescrBase(macAdapter_t *mac_p)
{
	tc_outl(CR_MAC_TXR_BADR, K1_TO_PHY( mac_p->txDescrRingBaseVAddr));
	tc_outl(CR_MAC_RXR_BADR, K1_TO_PHY( mac_p->rxDescrRingBaseVAddr));
 }

PRIVATE void macDMADisable()
{
	macCtrlReg_t	macCtrl;

 	macCtrl.value   = 0;
	macCtrl.value   = tc_inl(CR_MAC_MACCR);
	
	macCtrl.bits.rdma_en = 0;
	macCtrl.bits.xdma_en = 0;
	tc_outl(CR_MAC_MACCR, macCtrl.value);
	
	pause(2);
}

PRIVATE void macSetMACCR(macAdapter_t *map_p)
{
	
    macCtrlReg_t macCtrl;
    phyStatusReg_t phyStatus;

     // --- configure MAC ---
    phyStatus.value = miiStationRead ( mac_p->enetPhyAddr, 0x0);

    // clear reg
    macCtrl.value       = 0x3df23;
    // set flag
    /*
    macCtrl.bits.xdma_en     = 0;
    macCtrl.bits.rdma_en     = 0;
    macCtrl.bits.loop_en     = 0;
    macCtrl.bits.xmt_en      = 1;
    macCtrl.bits.rcv_en      = 1;
    macCtrl.bits.ht_multi_en = 1;
    macCtrl.bits.rx_runt     = 1;
    macCtrl.bits.rx_ftl      = 1;
    macCtrl.bits.rcv_all     = 1;
    macCtrl.bits.crc_apd     = 1;
    macCtrl.bits.fulltx      = (phyStatus.bits.full_duplex_100baseT || \
                           	phyStatus.bits.full_duplex_10baseT) ? 1 : 0;
    macCtrl.bits.rx_multipkt = 1;
    macCtrl.bits.rx_broadpkt = 1;
    */
    tc_outl(CR_MAC_MACCR, macCtrl.value);
 }

PRIVATE void macRegDMADescrpClear()
{
    macRxDescr_t *rx; 
    macTxDescr_t *tx;
    uint32 i;

    rx=(macRxDescr_t*)mac_p->rxDescrRingBaseVAddr;
    tx=(macTxDescr_t*)mac_p->txDescrRingBaseVAddr;
        
    for(i=0;i<RX_DESCP_NO;i++){
    
       rx->rdes0.word=0;
       rx->rdes1.word=0;
       rx->rdes2.word=0;
       rx++;
    
    }

    for(i=0;i<TX_DESCP_NO;i++){
 
       tx->tdes0.word=0;
       tx->tdes1.word=0;
       tx->tdes2.word=0;
       tx++;	
    }
}

 PUBLIC void
macControllerStop(macAdapter_t *mac_p)
{
	macCtrlReg_t macCtrl;

	macCtrl.value = tc_inl(CR_MAC_MACCR);
	/* disable DMA tx& rx */
	macCtrl.bits.rdma_en = 0;
	macCtrl.bits.xdma_en = 0;
	/* disable mac tx& rx */
	macCtrl.bits.rcv_en = 0;
	macCtrl.bits.xmt_en = 0;
	tc_outl(CR_MAC_MACCR, macCtrl.value);
}

PRIVATE void macSetMacReg(macAdapter_t *mac_p)
{
	tc_outl(CR_MAC_MADR,mac_p->macAddr[0]<<8  | mac_p->macAddr[1]<<0);
	tc_outl(CR_MAC_LADR,mac_p->macAddr[2]<<24 | mac_p->macAddr[3]<<16 | \
	                 mac_p->macAddr[4]<<8  | mac_p->macAddr[5]<<0);
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

PRIVATE void 
macDrvDescripInit(
	macAdapter_t *mac_p
)
{

	//
	//  Local Var.
	//

	macRxDescr_t * pRxDescp;
	macTxDescr_t * pTxDescp;
	uint32 physicalAddr;
	uint32 i;
	sk_buff *skb;

	mac_p->txDescrRingBaseVAddr = 
		K0_TO_K1((uint32)&mac_p->macMemPool_p->descrPool[0]);
 
	mac_p->rxDescrRingBaseVAddr =  mac_p->txDescrRingBaseVAddr + (MAC_DESCP_SIZE * TX_DESCP_NO);
	  	
	/* init. tx descriptor, don't allocate memory */
	pTxDescp = (macTxDescr_t*) mac_p->txDescrRingBaseVAddr;
	mac_p->txCurrDescp = pTxDescp; 
	

	for( i = 0 ; i< TX_DESCP_NO ; i++, pTxDescp++){
  		
		// Init descriptor
		pTxDescp->tdes0.word=0;
		pTxDescp->tdes1.word=0;
		pTxDescp->tdes2.word=0;
		
		// Assign flag
		pTxDescp->tdes0.bits.ownerDma = 0;		/* owned by Host */
		pTxDescp->tdes1.bits.fts=1;				/* last segment */
		pTxDescp->tdes1.bits.lts=1; 				/* first segment */
		pTxDescp->skb = NULL;
	}
	/* set the last tx descriptor */
	pTxDescp--;
	pTxDescp->tdes1.bits.edotr= 1; /* end of ring */			

	// 
	// Descriptor Byte Swap (Big Endian)
	// 

	
	pTxDescp = (macTxDescr_t*) mac_p->txDescrRingBaseVAddr;
	for( i = 0 ; i< TX_DESCP_NO ; i++, pTxDescp++){		
	    pTxDescp->tdes0.word=byte_swap(pTxDescp->tdes0.word);
	   	pTxDescp->tdes1.word=byte_swap(pTxDescp->tdes1.word);
	    pTxDescp->tdes2.word=byte_swap(pTxDescp->tdes2.word);		
	}

	/* init. Rx descriptor, allocate memory for each descriptor */
	pRxDescp = (macRxDescr_t*) mac_p->rxDescrRingBaseVAddr;

	for( i = 0 ; i< RX_DESCP_NO ; i++, pRxDescp++){
		
		// Init Descriptor
		pRxDescp->rdes0.word=0;
		pRxDescp->rdes1.word=0;
		pRxDescp->rdes2.word=0;
		
		// Assign flag
		pRxDescp->rdes0.bits.ownerDma = 1;		/* owned by DMA */
		pRxDescp->rdes1.bits.rxBufSize = 1996;
		if( (skb = alloc_skb(2000)) == NULL ){
			printf("skb is null\n");
		}
					  
		pRxDescp->rdes2.buf_p=K1_TO_PHY(skb->data);
		pRxDescp->skb = skb;
	}

	/* set the last rx descriptor */
	pRxDescp--;
	pRxDescp->rdes1.bits.edorr= 1; /* end of ring */

	// 
	// Descriptor Byte Swap (Big Endian)
	//
	 
	pRxDescp = (macRxDescr_t*) mac_p->rxDescrRingBaseVAddr;
	for( i = 0 ; i< RX_DESCP_NO ; i++ ){
	    pRxDescp->rdes0.word=byte_swap(pRxDescp->rdes0.word);
	   	pRxDescp->rdes1.word=byte_swap(pRxDescp->rdes1.word);
	    pRxDescp->rdes2.word=byte_swap(pRxDescp->rdes2.word);	
	    pRxDescp++;
	}

	mac_p->txCurrentDescp = 0;
	mac_p->txUnReleasedDescp = 0;
	mac_p->txUnReleasedBufCnt = 0;
	mac_p->rxCurrentDescp = 0;
}

PUBLIC void 
macDrvStart(
	void
)
{	
    macCtrlReg_t macCtrl;

	macCtrl.value = tc_inl(CR_MAC_MACCR);
	/* enable DMA tx& rx */
	macCtrl.bits.rdma_en = 1;
	macCtrl.bits.xdma_en = 1;
	/* enable mac tx& rx */
	macCtrl.bits.rcv_en = 1;
	macCtrl.bits.xmt_en = 1;
	tc_outl(CR_MAC_MACCR, macCtrl.value);
}

PUBLIC void 
macDrvStop(
	void
)
{	
    macCtrlReg_t macCtrl;

	macCtrl.value = tc_inl(CR_MAC_MACCR);
	/* enable DMA tx& rx */
	macCtrl.bits.rdma_en = 0;
	macCtrl.bits.xdma_en = 0;
	/* enable mac tx& rx */
	macCtrl.bits.rcv_en = 0;
	macCtrl.bits.xmt_en = 0;
	tc_outl(CR_MAC_MACCR, macCtrl.value);
}

void
macPhyReset(
	void
)
{
	miiStationWrite(mac_p->enetPhyAddr, PHY_CONTROL_REG, PHY_RESET );
	miiStationWrite(mac_p->enetPhyAddr, PHY_CONTROL_REG, MIIDR_AUTO_NEGOTIATE );
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
    
    macSetHashTable();

    macSetIntTimer();

    //macSetAutoPollTmr();

    macSetupDMABurstArbitration();

    macSetDMADescrBase(mac_p);

    macSetMACCR(mac_p);
    
    // --- setup MAC address ---
    macSetMacReg(mac_p);      

    return 0;
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

PUBLIC void
macRxRingProc(
	void
)
{
	volatile macRxDescr_t *rxDescrp;
	uint32 frameSize;
	sk_buff *newMbp,*oldMbp;	

	rxDescrp = ((macRxDescr_t*)mac_p->rxDescrRingBaseVAddr) + mac_p->rxCurrentDescp;
	
    while ( (rxDescrp->rdes0.word & (1<<7)) == 0) { // ownerDMA
		// crc error | rx_error | runt | ftl | txOddNb
    	if(  (rxDescrp->rdes0.word & (1<<11 | 1<<10 | 1<<13 | 1<<12 | 1<<14)) == 0 ) {
		   	frameSize=0;
		   	frameSize |=((rxDescrp->rdes0.word)&0xFF000000)>>24;  // rfl
		  	frameSize |=((rxDescrp->rdes0.word)&0x00070000)>>8;
		 	
   	        if ( (frameSize < 60) || (frameSize > 1518) ) 
   	        {
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

			eth_rcv(oldMbp);

DISCARD:
/* ----- Do something to count discard ? ----- */
RECVOK:
			rxDescrp->rdes2.buf_p = byte_swap(K1_TO_PHY(newMbp->data));
			rxDescrp->skb = newMbp;

		}
		
        rxDescrp->rdes0.word|=(1<<7);		
		
		if( mac_p->rxCurrentDescp == ( RX_DESCP_NO - 1 )){
			mac_p->rxCurrentDescp = 0;
			rxDescrp = (macRxDescr_t*)mac_p->rxDescrRingBaseVAddr;
		}
		else {
			mac_p->rxCurrentDescp++;
			rxDescrp ++;
		}	
	} /* while loop  */		
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
//	if(isr.bits.notxbuf 	)prom_printf("\r\n--->NOTXBUF	  ");
//	if(isr.bits.xpkt_save	)prom_printf("\r\n--->XPKT_SAVE   ");
	if(isr.bits.norxbuf 	)prom_printf("\r\n--->NORXBUF	  ");
//	if(isr.bits.rpkt_finish )prom_printf("\r\n--->RPKT_FINISH ");
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

PRIVATE macTxDescr_t*
macTxRingProc(
	macAdapter_t *mac_p
)
{   
	volatile macTxDescr_t * pTxDescp;
	uint32 collisions;
	uint32 status;
	
	pTxDescp = ((macTxDescr_t*)mac_p->txDescrRingBaseVAddr) + mac_p->txUnReleasedDescp;

	if ( mac_p->txUnReleasedBufCnt != 0 ) {
		if ( (pTxDescp->tdes0.word)&(1<<7) ){
			return 0;
		}

		free_skb(pTxDescp->skb);
		pTxDescp->tdes2.buf_p = NULL;
		pTxDescp->skb = NULL;	
	
		if( mac_p->txUnReleasedDescp == ( TX_DESCP_NO - 1) ){
			mac_p->txUnReleasedDescp = 0;
		}else{
			mac_p->txUnReleasedDescp++;
		}

		mac_p->txUnReleasedBufCnt--;
		pTxDescp = ((macTxDescr_t*)mac_p->txDescrRingBaseVAddr) + mac_p->txUnReleasedDescp;	
	}

	return (macTxDescr_t*)pTxDescp;
}

struct irqaction mac_irq = {mac_interrupt, NULL, 0, "mac", NULL, NULL};

int tc3162_eth_init(unsigned char *mac_addr)
{
	int i;
	
	macControllerStop(mac_p);
	
	mac_p->macMemPool_p = &enetMemPool;
	mac_p->enetPhyAddr = 0;

 	macGetMacAddr(mac_p, mac_addr);
	macDrvDescripInit(mac_p);

	macPhyReset();
	pause(100);

	macDrvRegInit(mac_p);

	request_IRQ(MAC_INT, &mac_irq, NULL);
	macDrvStart();

	macSetRxMode();

	return 0;
}

int tc3162_eth_send(sk_buff *skb)
{
	macTxDescr_t *currDescrp = NULL;
	uint32 length = skb->len;
	uint8 *bufAddrp = skb->data;

	if( skb->data == NULL ){
		printf("\n\rTx a empty mbuf"); 	
	}

	/* ----- Use the maxTxBufInUse to remove interrupts of Tx ----- */
	macTxRingProc(mac_p);

	if ( mac_p->txUnReleasedBufCnt == (TX_DESCP_NO - 1 )) {
		free_skb(skb);
		return 1;
	}

	/* ----- Get the transmit descriptor ----- */
	currDescrp = ((macTxDescr_t*)mac_p->txDescrRingBaseVAddr) + mac_p->txCurrentDescp;

	if( (currDescrp->tdes0.word)&(1<<7) ){
		free_skb(skb);
		return 1;
	}	

	// tx buffer size
	currDescrp->tdes1.word&=~0xFF070000;
	currDescrp->tdes1.word|=(length&0x000000ff)<<24;
	currDescrp->tdes1.word|=(length&0x00000700)<<8;
	
	//txic
	//currDescrp->tdes1.word|=(1<<6);
	currDescrp->tdes2.buf_p = byte_swap(K1_TO_PHY(skb->data));
	currDescrp->skb = skb;

	if( mac_p->txCurrentDescp == ( TX_DESCP_NO - 1 )){
		mac_p->txCurrentDescp = 0;	
	}else{
		mac_p->txCurrentDescp ++;
	}
	
	mac_p->txUnReleasedBufCnt++;  
	
	currDescrp->tdes0.word|=(1<<7); 

	MAC_TX_POLL_DEMAND_(1);  
	return 0;
}

int tc3162_eth_exit(void)
{
	return 0;
}


