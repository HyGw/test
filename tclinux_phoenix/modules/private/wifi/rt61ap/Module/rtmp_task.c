#include "rt_config.h"


#ifdef THREAD_ISR
static void rtmp_rx_done_tasklet(unsigned long data);
static void rtmp_tx_done_tasklet(unsigned long data);
static void rtmp_mgmt_done_tasklet(unsigned long data);
static void rtmp_ac0_dma_done_tasklet(unsigned long data);
static void rtmp_ac1_dma_done_tasklet(unsigned long data);
static void rtmp_ac2_dma_done_tasklet(unsigned long data);
static void rtmp_ac3_dma_done_tasklet(unsigned long data);
static void rtmp_hcca_dma_done_tasklet(unsigned long data);



#define RTMP_MASK_INT_TX_DONE			(1<<0)		// bit 0	
#define RTMP_MASK_INT_RX_DONE			(1<<1)		// bit 1
#define RTMP_MASK_INT_AC0_DMA_DONE		(1<<16)		// bit 16
#define RTMP_MASK_INT_AC1_DMA_DONE		(1<<17)		// bit 17
#define RTMP_MASK_INT_AC2_DMA_DONE		(1<<18)		// bit 18
#define RTMP_MASK_INT_AC3_DMA_DONE		(1<<19)		// bit 19
#define RTMP_MASK_INT_MGMT_DONE			(1<<20)		// bit 20
#define RTMP_MASK_INT_HCCA_DMA_DONE		(1<<21)		// bit 21

spinlock_t	Rtmp_Irq_Lock;


static void rtmp_rx_done_tasklet(unsigned long data)
{
	UINT32		irqsave;
	UINT32		irqMask;

	PRTMP_ADAPTER pAdapter = (PRTMP_ADAPTER) data; 

	/* device had been closed */
	if (RTMP_TEST_FLAG(pAdapter, fRTMP_ADAPTER_REMOVE_IN_PROGRESS)) {
		return;
	}
	
	RTMPHandleRxDoneInterrupt(pAdapter);

	RTMP_IRQ_LOCK(irqsave);

	/*
	 * double check to avoid rotting packet 
	 */

	if ((pAdapter->Rtmp_Masked_Int & RTMP_MASK_INT_RX_DONE) || pAdapter->Rx_Early_Done) {
		pAdapter->Rtmp_Masked_Int &= ~RTMP_MASK_INT_RX_DONE;
		tasklet_hi_schedule(&pAdapter->rx_done_task);
	} else {
		/* enable rx interrupt */
		RTMP_IO_READ32(pAdapter, INT_MASK_CSR,  &irqMask);
		RTMP_IO_WRITE32(pAdapter, INT_MASK_CSR,  irqMask & ~RTMP_MASK_INT_RX_DONE); 		
		/* flush the register */
		RTMP_IO_READ32(pAdapter, INT_MASK_CSR,  &irqMask);
		pAdapter->rx_done_running = FALSE;				
	}
		
	RTMP_IRQ_UNLOCK(irqsave);
}

static void rtmp_tx_done_tasklet(unsigned long data)
{
	UINT32		irqsave;
	UINT32		irqMask;
	PRTMP_ADAPTER pAdapter = (PRTMP_ADAPTER) data;

	/* device had been closed */
	if (RTMP_TEST_FLAG(pAdapter, fRTMP_ADAPTER_REMOVE_IN_PROGRESS)) {
		return;
	}

	RTMPHandleMgmtRingDmaDoneInterrupt(pAdapter);	


	RTMPHandleTxDoneInterrupt(pAdapter);
		
	RTMP_IRQ_LOCK(irqsave);

	/*
	 * double check to avoid rotting packet 
	 */

	if (pAdapter->Rtmp_Masked_Int & RTMP_MASK_INT_TX_DONE) {	
		pAdapter->Rtmp_Masked_Int &= ~RTMP_MASK_INT_TX_DONE;	
		tasklet_hi_schedule(&pAdapter->tx_done_task);
	} else {
		/* enable tx interrupt */
		RTMP_IO_READ32(pAdapter, INT_MASK_CSR,  &irqMask);
		RTMP_IO_WRITE32(pAdapter, INT_MASK_CSR,  irqMask & ~RTMP_MASK_INT_TX_DONE);
		pAdapter->tx_done_running = FALSE;
	}		

	RTMP_IRQ_UNLOCK(irqsave);
}

static void rtmp_ac0_dma_done_tasklet(unsigned long data)
{
	UINT32		irqsave;
	UINT32		irqMask;
	PRTMP_ADAPTER pAdapter = (PRTMP_ADAPTER) data;
    INT_SOURCE_CSR_STRUC	IntSource;

	/* device had been closed */
	if (RTMP_TEST_FLAG(pAdapter, fRTMP_ADAPTER_REMOVE_IN_PROGRESS)) {
		return;
	}


	IntSource.word = 0;
	IntSource.field.Ac0DmaDone = 1;

	RTMPHandleTxRingDmaDoneInterrupt(pAdapter, IntSource);
	
	RTMP_IRQ_LOCK(irqsave);

	/*
	 * double check to avoid rotting packet 
	 */

	if (pAdapter->Rtmp_Masked_Int & RTMP_MASK_INT_AC0_DMA_DONE) {	
		pAdapter->Rtmp_Masked_Int &= ~RTMP_MASK_INT_AC0_DMA_DONE;	
		tasklet_hi_schedule(&pAdapter->ac0_dma_done_task);
	} else {
		/* enable ac0 interrupt */
		RTMP_IO_READ32(pAdapter, INT_MASK_CSR,  &irqMask);
		RTMP_IO_WRITE32(pAdapter, INT_MASK_CSR,  irqMask & ~RTMP_MASK_INT_AC0_DMA_DONE);
		pAdapter->ac0_dma_done_running = FALSE;
	}
		
	RTMP_IRQ_UNLOCK(irqsave);
}

static void rtmp_ac1_dma_done_tasklet(unsigned long data)
{
	UINT32		irqsave;
	UINT32		irqMask;
	PRTMP_ADAPTER pAdapter = (PRTMP_ADAPTER) data;
    INT_SOURCE_CSR_STRUC	IntSource;

	/* device had been closed */
	if (RTMP_TEST_FLAG(pAdapter, fRTMP_ADAPTER_REMOVE_IN_PROGRESS)) {
		return;
	}

	IntSource.word = 0;
	IntSource.field.Ac1DmaDone = 1;

	RTMPHandleTxRingDmaDoneInterrupt(pAdapter, IntSource);
	
	RTMP_IRQ_LOCK(irqsave);

	/*
	 * double check to avoid rotting packet 
	 */

	if (pAdapter->Rtmp_Masked_Int & RTMP_MASK_INT_AC1_DMA_DONE) {	
		pAdapter->Rtmp_Masked_Int &= ~RTMP_MASK_INT_AC1_DMA_DONE;	
		tasklet_hi_schedule(&pAdapter->ac1_dma_done_task);
	} else {
		/* enable ac1 interrupt */
		RTMP_IO_READ32(pAdapter, INT_MASK_CSR,  &irqMask);
		RTMP_IO_WRITE32(pAdapter, INT_MASK_CSR,  irqMask & ~RTMP_MASK_INT_AC1_DMA_DONE);
		pAdapter->ac1_dma_done_running = FALSE;
	}
		
	RTMP_IRQ_UNLOCK(irqsave);
}

static void rtmp_ac2_dma_done_tasklet(unsigned long data)
{

	UINT32		irqsave;
	UINT32		irqMask;
	PRTMP_ADAPTER pAdapter = (PRTMP_ADAPTER) data;
    INT_SOURCE_CSR_STRUC	IntSource;

	/* device had been closed */
	if (RTMP_TEST_FLAG(pAdapter, fRTMP_ADAPTER_REMOVE_IN_PROGRESS)) {
		return;
	}

	IntSource.word = 0;
	IntSource.field.Ac2DmaDone = 1;

	RTMPHandleTxRingDmaDoneInterrupt(pAdapter, IntSource);
	
	RTMP_IRQ_LOCK(irqsave);

	/*
	 * double check to avoid rotting packet 
	 */

	if (pAdapter->Rtmp_Masked_Int & RTMP_MASK_INT_AC2_DMA_DONE) {	
		pAdapter->Rtmp_Masked_Int &= ~RTMP_MASK_INT_AC2_DMA_DONE;	
		tasklet_hi_schedule(&pAdapter->ac2_dma_done_task);
	} else {
		/* enable ac2 interrupt */
		RTMP_IO_READ32(pAdapter, INT_MASK_CSR,  &irqMask);
		RTMP_IO_WRITE32(pAdapter, INT_MASK_CSR,  irqMask & ~RTMP_MASK_INT_AC2_DMA_DONE);
		pAdapter->ac2_dma_done_running = FALSE;
	}
		
	RTMP_IRQ_UNLOCK(irqsave);
}

static void rtmp_ac3_dma_done_tasklet(unsigned long data)
{
	UINT32		irqsave;
	UINT32		irqMask;
	PRTMP_ADAPTER pAdapter = (PRTMP_ADAPTER) data;
    INT_SOURCE_CSR_STRUC	IntSource;

	/* device had been closed */
	if (RTMP_TEST_FLAG(pAdapter, fRTMP_ADAPTER_REMOVE_IN_PROGRESS)) {
		return;
	}

	IntSource.word = 0;
	IntSource.field.Ac3DmaDone = 1;

	RTMPHandleTxRingDmaDoneInterrupt(pAdapter, IntSource);
	
	RTMP_IRQ_LOCK(irqsave);

	/*
	 * double check to avoid rotting packet 
	 */

	if (pAdapter->Rtmp_Masked_Int & RTMP_MASK_INT_AC3_DMA_DONE) {	
		pAdapter->Rtmp_Masked_Int &= ~RTMP_MASK_INT_AC3_DMA_DONE;	
		tasklet_hi_schedule(&pAdapter->ac3_dma_done_task);
	} else {
		/* enable ac3 interrupt */
		RTMP_IO_READ32(pAdapter, INT_MASK_CSR,  &irqMask);
		RTMP_IO_WRITE32(pAdapter, INT_MASK_CSR,  irqMask & ~RTMP_MASK_INT_AC3_DMA_DONE);
		pAdapter->ac3_dma_done_running = FALSE;
	}
		
	RTMP_IRQ_UNLOCK(irqsave);
}

static void rtmp_hcca_dma_done_tasklet(unsigned long data)
{

	UINT32		irqsave;
	UINT32		irqMask;
	PRTMP_ADAPTER pAdapter = (PRTMP_ADAPTER) data;
    INT_SOURCE_CSR_STRUC	IntSource;

	/* device had been closed */
	if (RTMP_TEST_FLAG(pAdapter, fRTMP_ADAPTER_REMOVE_IN_PROGRESS)) {
		return;
	}

	IntSource.word = 0;
	IntSource.field.HccaDmaDone = 1;

	RTMPHandleTxRingDmaDoneInterrupt(pAdapter, IntSource);
	
	RTMP_IRQ_LOCK(irqsave);

	/*
	 * double check to avoid rotting packet 
	 */

	if (pAdapter->Rtmp_Masked_Int & RTMP_MASK_INT_HCCA_DMA_DONE) {	
		pAdapter->Rtmp_Masked_Int &= ~RTMP_MASK_INT_HCCA_DMA_DONE;	
		tasklet_hi_schedule(&pAdapter->hcca_dma_done_task);
	} else {
		/* enable hcca interrupt */
		RTMP_IO_READ32(pAdapter, INT_MASK_CSR,  &irqMask);
		RTMP_IO_WRITE32(pAdapter, INT_MASK_CSR,  irqMask & ~RTMP_MASK_INT_HCCA_DMA_DONE);
		pAdapter->hcca_dma_done_running = FALSE;
	}
		
	RTMP_IRQ_UNLOCK(irqsave);
}




static void rtmp_mgmt_done_tasklet(unsigned long data)
{
	UINT32		irqsave;
	UINT32		irqMask;
	PRTMP_ADAPTER pAdapter = (PRTMP_ADAPTER) data;

	/* device had been closed */
	if (RTMP_TEST_FLAG(pAdapter, fRTMP_ADAPTER_REMOVE_IN_PROGRESS)) {
		return;
	}

	RTMPHandleMgmtRingDmaDoneInterrupt(pAdapter);	
	
	RTMP_IRQ_LOCK(irqsave);

	/*
	 * double check to avoid rotting packet 
	 */

	if (pAdapter->Rtmp_Masked_Int & RTMP_MASK_INT_MGMT_DONE) {	
		pAdapter->Rtmp_Masked_Int &= ~RTMP_MASK_INT_MGMT_DONE;	
		tasklet_hi_schedule(&pAdapter->mgmt_done_task);
	} else {
		/* enable mgmt interrupt */
		RTMP_IO_READ32(pAdapter, INT_MASK_CSR,  &irqMask);
		RTMP_IO_WRITE32(pAdapter, INT_MASK_CSR,  irqMask & ~RTMP_MASK_INT_MGMT_DONE);
		pAdapter->mgmt_done_running = FALSE;
	}
		
	RTMP_IRQ_UNLOCK(irqsave);
}


void Rtmp_Init_Thread_Task(IN PRTMP_ADAPTER pAdapter)
{
	tasklet_init(&pAdapter->rx_done_task, rtmp_rx_done_tasklet, (unsigned long)pAdapter);
	tasklet_init(&pAdapter->tx_done_task, rtmp_tx_done_tasklet, (unsigned long)pAdapter);
	tasklet_init(&pAdapter->mgmt_done_task, rtmp_mgmt_done_tasklet, (unsigned long)pAdapter);
	tasklet_init(&pAdapter->ac0_dma_done_task, rtmp_ac0_dma_done_tasklet, (unsigned long)pAdapter);
	tasklet_init(&pAdapter->ac1_dma_done_task, rtmp_ac1_dma_done_tasklet, (unsigned long)pAdapter);
	tasklet_init(&pAdapter->ac2_dma_done_task, rtmp_ac2_dma_done_tasklet, (unsigned long)pAdapter);
	tasklet_init(&pAdapter->ac3_dma_done_task, rtmp_ac3_dma_done_tasklet, (unsigned long)pAdapter);
	tasklet_init(&pAdapter->hcca_dma_done_task, rtmp_hcca_dma_done_tasklet, (unsigned long)pAdapter);

	pAdapter->rx_done_running = FALSE;
	pAdapter->tx_done_running = FALSE;
	pAdapter->mgmt_done_running = FALSE;
	pAdapter->ac0_dma_done_running = FALSE;
	pAdapter->ac1_dma_done_running = FALSE;
	pAdapter->ac2_dma_done_running = FALSE;
	pAdapter->ac3_dma_done_running = FALSE;
	pAdapter->hcca_dma_done_running = FALSE;

	pAdapter->Rx_Early_Done = FALSE;
	pAdapter->Rtmp_Masked_Int = 0;

	RTMP_SEM_INIT(&Rtmp_Irq_Lock);
}


void Rtmp_Kill_Thread_Task(IN PRTMP_ADAPTER pAdapter)
{
	tasklet_kill(&pAdapter->rx_done_task);
	tasklet_kill(&pAdapter->tx_done_task);
	tasklet_kill(&pAdapter->mgmt_done_task);
	tasklet_kill(&pAdapter->ac0_dma_done_task);
	tasklet_kill(&pAdapter->ac1_dma_done_task);
	tasklet_kill(&pAdapter->ac2_dma_done_task);
	tasklet_kill(&pAdapter->ac3_dma_done_task);
	tasklet_kill(&pAdapter->hcca_dma_done_task);
}


UINT32 Rtmp_Thread_Isr(IN PRTMP_ADAPTER pAdapter, 
					   INT_SOURCE_CSR_STRUC	IntSource)
{

	UINT32	int_mask = 0;

	if (IntSource.field.MgmtDmaDone)
	{
		pAdapter->Rtmp_Masked_Int |= RTMP_MASK_INT_MGMT_DONE;
		if (pAdapter->mgmt_done_running == FALSE){
			pAdapter->mgmt_done_running = TRUE;
			pAdapter->Rtmp_Masked_Int &= ~RTMP_MASK_INT_MGMT_DONE;
			/* mask out mgmt interrupt */
			int_mask |= RTMP_MASK_INT_MGMT_DONE;
			tasklet_hi_schedule(&pAdapter->mgmt_done_task);	
	    }
	}

    if (IntSource.field.RxDone)
	{
		pAdapter->Rtmp_Masked_Int |= RTMP_MASK_INT_RX_DONE;
		if (pAdapter->rx_done_running == FALSE){
			pAdapter->rx_done_running = TRUE;
			pAdapter->Rtmp_Masked_Int &= ~RTMP_MASK_INT_RX_DONE;
			/* mask out rx interrupt */
			int_mask |= RTMP_MASK_INT_RX_DONE;
			tasklet_hi_schedule(&pAdapter->rx_done_task);
	    }
	}

	if (IntSource.field.TxDone)
	{
		pAdapter->Rtmp_Masked_Int |= RTMP_MASK_INT_TX_DONE;
		if (pAdapter->tx_done_running == FALSE){
			pAdapter->tx_done_running = TRUE;
			pAdapter->Rtmp_Masked_Int &= ~RTMP_MASK_INT_TX_DONE;
			/* mask out rx interrupt */
			int_mask |= RTMP_MASK_INT_TX_DONE;
			tasklet_hi_schedule(&pAdapter->tx_done_task);	
	    }
	}

    if (IntSource.field.Ac0DmaDone)
	{
		pAdapter->Rtmp_Masked_Int |= RTMP_MASK_INT_AC0_DMA_DONE;
		if (pAdapter->ac0_dma_done_running == FALSE){
			pAdapter->ac0_dma_done_running = TRUE;
			pAdapter->Rtmp_Masked_Int &= ~RTMP_MASK_INT_AC0_DMA_DONE;
			/* mask out ac0 interrupt */
			int_mask |= RTMP_MASK_INT_AC0_DMA_DONE;
			tasklet_hi_schedule(&pAdapter->ac0_dma_done_task);	
	    }
	}


    if (IntSource.field.HccaDmaDone)
	{
		pAdapter->Rtmp_Masked_Int |= RTMP_MASK_INT_HCCA_DMA_DONE;
		if (pAdapter->hcca_dma_done_running == FALSE){
			pAdapter->hcca_dma_done_running = TRUE;
			pAdapter->Rtmp_Masked_Int &= ~RTMP_MASK_INT_HCCA_DMA_DONE;
			/* mask out hcca interrupt */
			int_mask |= RTMP_MASK_INT_HCCA_DMA_DONE;
			tasklet_hi_schedule(&pAdapter->hcca_dma_done_task);	
	    }
	}

    if (IntSource.field.Ac3DmaDone)
	{
		pAdapter->Rtmp_Masked_Int |= RTMP_MASK_INT_AC3_DMA_DONE;
		if (pAdapter->ac3_dma_done_running == FALSE){
			pAdapter->ac3_dma_done_running = TRUE;
			pAdapter->Rtmp_Masked_Int &= ~RTMP_MASK_INT_AC3_DMA_DONE;
			/* mask out ac3 interrupt */
			int_mask |= RTMP_MASK_INT_AC3_DMA_DONE;
			tasklet_hi_schedule(&pAdapter->ac3_dma_done_task);	
	    }
	}


    if (IntSource.field.Ac2DmaDone)
	{
		pAdapter->Rtmp_Masked_Int |= RTMP_MASK_INT_AC2_DMA_DONE;
		if (pAdapter->ac2_dma_done_running == FALSE){
			pAdapter->ac2_dma_done_running = TRUE;
			pAdapter->Rtmp_Masked_Int &= ~RTMP_MASK_INT_AC2_DMA_DONE;
			/* mask out ac2 interrupt */
			int_mask |= RTMP_MASK_INT_AC2_DMA_DONE;
			tasklet_hi_schedule(&pAdapter->ac2_dma_done_task);	
	    }
	}

	if (IntSource.field.Ac1DmaDone)
	{
		pAdapter->Rtmp_Masked_Int |= RTMP_MASK_INT_AC1_DMA_DONE;
		if (pAdapter->ac1_dma_done_running == FALSE){
			pAdapter->ac1_dma_done_running = TRUE;
			pAdapter->Rtmp_Masked_Int &= ~RTMP_MASK_INT_AC1_DMA_DONE;
			/* mask out ac1 interrupt */
			int_mask |= RTMP_MASK_INT_AC1_DMA_DONE;
			tasklet_hi_schedule(&pAdapter->ac1_dma_done_task);	
	    }
	}

	return (int_mask);
}
#endif /* THREAD_ISR */


