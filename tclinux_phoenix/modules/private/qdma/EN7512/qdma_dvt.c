/***************************************************************
Copyright Statement:

This software/firmware and related documentation (EcoNet Software) 
are protected under relevant copyright laws. The information contained herein 
is confidential and proprietary to EcoNet (HK) Limited (EcoNet) and/or 
its licensors. Without the prior written permission of EcoNet and/or its licensors, 
any reproduction, modification, use or disclosure of EcoNet Software, and 
information contained herein, in whole or in part, shall be strictly prohibited.

EcoNet (HK) Limited  EcoNet. ALL RIGHTS RESERVED.

BY OPENING OR USING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY 
ACKNOWLEDGES AND AGREES THAT THE SOFTWARE/FIRMWARE AND ITS 
DOCUMENTATIONS (ECONET SOFTWARE) RECEIVED FROM ECONET 
AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON AN AS IS 
BASIS ONLY. ECONET EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, 
WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, 
OR NON-INFRINGEMENT. NOR DOES ECONET PROVIDE ANY WARRANTY 
WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTIES WHICH 
MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE ECONET SOFTWARE. 
RECEIVER AGREES TO LOOK ONLY TO SUCH THIRD PARTIES FOR ANY AND ALL 
WARRANTY CLAIMS RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES 
THAT IT IS RECEIVERS SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD 
PARTY ALL PROPER LICENSES CONTAINED IN ECONET SOFTWARE.

ECONET SHALL NOT BE RESPONSIBLE FOR ANY ECONET SOFTWARE RELEASES 
MADE TO RECEIVERS SPECIFICATION OR CONFORMING TO A PARTICULAR 
STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND 
ECONET'S ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE ECONET 
SOFTWARE RELEASED HEREUNDER SHALL BE, AT ECONET'S SOLE OPTION, TO 
REVISE OR REPLACE THE ECONET SOFTWARE AT ISSUE OR REFUND ANY SOFTWARE 
LICENSE FEES OR SERVICE CHARGES PAID BY RECEIVER TO ECONET FOR SUCH 
ECONET SOFTWARE.
***************************************************************/

/************************************************************************
*                  I N C L U D E S
*************************************************************************
*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/dma-mapping.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/random.h>
#include <linux/skbuff.h>
#include <linux/proc_fs.h>
#include <linux/jiffies.h>
#include <asm/tc3162/tc3162.h>
#include <linux/version.h> 
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,18,19) 
#include "qdma_bmgr.h"
#include "qdma_api.h"
#else
#include "qdma_api.h"
#include "qdma_bmgr.h"
#endif
#include "qdma_dev.h"
#include "qdma_dvt.h"
#include <ecnt_hook/ecnt_hook_fe.h>


#ifdef CONFIG_SUPPORT_SELF_TEST

/************************************************************************
*                  D E F I N E S   &   C O N S T A N T S
*************************************************************************
*/
#define DBG_CNTR_LOOP_NUM 3
#define PACKET_NUM 2000  

#ifdef QDMA_LAN
#define CSR3_ROUND_CNT	100
#else
#define CSR3_ROUND_CNT	500
#endif

/************************************************************************
*                  M A C R O S
*************************************************************************
*/

/************************************************************************
*                  D A T A	 T Y P E S
*************************************************************************
*/
int (*bmRecvCallbackFunction)(void *, uint, struct sk_buff *, uint) ;
int (*bmEventCallbackFunction)(void *, uint, struct sk_buff *, uint) ;


/************************************************************************
*                  E X T E R N A L	 D A T A   D E C L A R A T I O N S
*************************************************************************
*/

/************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
*************************************************************************
*/
static int qdma_dvt_netdev_open(struct net_device *dev);
static int qdma_dvt_netdev_close(struct net_device *dev);
static int qdma_dvt_netdev_set_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd);
static int qdma_dvt_netdev_start(struct net_device *dev);

/************************************************************************
*                  P U B L I C   D A T A
*************************************************************************
*/
uint selfTestFlag=0;
uint umacTestFlag=0;
uint loopCnt=0;
uint delayTime=0, nlen=0;
uint msec=0, aveTime=0;
QDMA_DbgCounters_T counters[CONFIG_QDMA_CHANNEL][CONFIG_QDMA_QUEUE] ;
QDMA_DbgCounters_T chain_counter[2];
uint rxCounter[RX_RING_NUM];
struct net_device *en7512NapiDev = NULL;
uint8 defMacAddr[] = {0x00, 0x00, 0xaa, 0xbb, 0xcc, 0xff};

#if defined(TCSUPPORT_AUTOBENCH) && defined(TCSUPPORT_CPU_MT7520)
int pon_loopback_result = 0;
EXPORT_SYMBOL(pon_loopback_result);
#endif


/************************************************************************
*                  P R I V A T E   D A T A
*************************************************************************
*/
static DEFINE_SPINLOCK(napiLock);

static const struct net_device_ops qdmaDvt_netdev_ops = {
	.ndo_init               = qdma_dvt_netdev_start,
	.ndo_open               = qdma_dvt_netdev_open,
	.ndo_stop               = qdma_dvt_netdev_close,
	.ndo_do_ioctl           = qdma_dvt_netdev_set_ioctl,
	.ndo_start_xmit         = NULL,
	.ndo_change_mtu         = NULL,
	.ndo_validate_addr      = NULL,
};


/************************************************************************
*                  F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/

/******************************************************************************
******************************************************************************/
static void __st_dump_skb(struct sk_buff *skb) 
{
	unchar n=0, *p = (unchar *)skb->data ;
	int i=0 ;

	for(i=0 ; i<skb->len ; i++) {
		n = i & 0x0f ;
		
		if(n == 0x00) 		printk("%.4x: ", i) ;
		else if(n == 0x08) 	printk(" ") ;

		printk("%.2x ", *p++) ; 
		
		if(n == 0x0F)	printk("\n") ;
	}
	
	if(n != 0x0F) 	printk("\n") ;
}

/******************************************************************************
******************************************************************************/
static ushort __st_in_csum(unsigned short *ptr, int nbytes)
{
	register int			sum=0;		/* assumes long == 32 bits */
	unsigned short			oddbyte=0;
	register unsigned short	answer=0; 	/* assumes u_short == 16 bits */

	/*
	 * Our algorithm is simple, using a 32-bit accumulator (sum),
	 * we add sequential 16-bit words to it, and at the end, fold back
	 * all the carry bits from the top 16 bits into the lower 16 bits.
	 */

	sum = 0;

	while (nbytes > 1)	{
		sum += *ptr++;
		nbytes -= 2;
	}

	/* mop up an odd byte, if necessary */
	if (nbytes == 1) {
		oddbyte = 0;		/* make sure top half is zero */
		*((unsigned char *) &oddbyte) = *(unsigned char *)ptr;   /* one byte only */
		sum += oddbyte;
	}

	/*
	 * Add back carry outs from top 16 bits to low 16 bits.
	 */

	sum  = (sum >> 16) + (sum & 0xffff);	/* add high-16 to low-16 */
	sum += (sum >> 16); 		/* add carry */
	answer = ~sum;		/* ones-complement, then truncate to 16 bits */
	return(answer);
}

/******************************************************************************
******************************************************************************/
static void __st_print_counters(void)
{
	int i=0, j=0 ;
	uint tx=0, rx=0, rx_err=0 ;
	
	msleep(2000) ;
	
	printk(" C   Q     Tx Frames    Rx OK Frames    Rx Err Frames \n") ;
	for(i=0 ; i<CONFIG_QDMA_CHANNEL ; i++) {
		for(j=0 ; j<CONFIG_QDMA_QUEUE ; j++) {
			tx += counters[i][j].tx_frames ;
			rx += counters[i][j].rx_frames ;
			rx_err += counters[i][j].rx_err_frames ;
			
			if(counters[i][j].tx_frames || counters[i][j].rx_frames) {
				printk(" %2d  %d   %12d    %12d    %12d\n", i, j, counters[i][j].tx_frames, 
															counters[i][j].rx_frames, 
															counters[i][j].rx_err_frames) ;
			}
		}
	}
	printk(" Total   %12d    %12d    %12d\n", tx, rx, rx_err) ;
	printk("\n") ;
}

/******************************************************************************
******************************************************************************/
static int __st_check_counters(void)
{
	int i=0, j=0 ;
	
	msleep(1000) ;
	
	for(i=0 ; i<CONFIG_QDMA_CHANNEL ; i++) {
		for(j=0 ; j<CONFIG_QDMA_QUEUE ; j++) {
			if(counters[i][j].rx_err_frames!=0 || counters[i][j].tx_frames!=counters[i][j].rx_frames) {
				return -1 ;
			}
		}
	}

	return 0 ;
}

static int qdma_polling_tx_recycle(void) 
{
	while(gpQdmaPriv->devCfg.flags.isTxPolling == QDMA_ENABLE) {
		qdma_bm_transmit_done(0) ;
		msleep(500) ;
	}
	
	return 0 ;
}


static int qdma_dvt_netdev_open(struct net_device *dev)
{
	QDMA_Adapter_T *qdmaApt = netdev_priv(dev);	
	
	napi_enable(&qdmaApt->napi);

	netif_start_queue(dev);

  	return 0;
}


static int qdma_dvt_netdev_close(struct net_device *dev)
{
	QDMA_Adapter_T *qdmaApt = netdev_priv(dev);
	
	printk("mt7510_netdev_close\n");

	printk("%s: stoping interface.\n", dev->name);

	netif_stop_queue(dev);

	napi_disable(&qdmaApt->napi);

	return 0;
}


static int qdma_dvt_netdev_set_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd)
{
	/* Not implemented yet */
  	return 0;
}


static int qdma_dvt_netdev_start(struct net_device *dev)
{
	uint8 *flashMacAddr = (uint8 *) (0xbc00ff48);
	
	if (
		flashMacAddr[0] == 0 && flashMacAddr[1] == 0 && flashMacAddr[2] == 0 &&
		flashMacAddr[3] == 0 && flashMacAddr[4] == 0 && flashMacAddr[5] == 0
	)
		printk(KERN_INFO "\nThe MAC address in flash is null!\n");	    
	else    
  		memcpy(defMacAddr, flashMacAddr, 6);  	

	memcpy(dev->dev_addr, defMacAddr, 6);
	dev->addr_len = 6;

	return 0;
}


void
delay1ms(
	int ms
)
{
	volatile uint32 timer_now=0, timer_last=0;
	volatile uint32 tick_acc=0;
	uint32 one_tick_unit = 1 * SYS_HCLK * 1000 / 2;
	volatile uint32 tick_wait = ms * one_tick_unit;
	volatile uint32 timer1_ldv = VPint(CR_TIMER1_LDV);

	tick_acc = 0;
 	timer_last = VPint(CR_TIMER1_VLR);
	do {
   		timer_now = VPint(CR_TIMER1_VLR);
       	if (timer_last >= timer_now)
       		tick_acc += timer_last - timer_now;
      	else
       		tick_acc += timer1_ldv - timer_now + timer_last;
     	timer_last = timer_now;
	} while (tick_acc < tick_wait);
}

static int qdma_st_cb_EventHandler(QDMA_EventType_t qdmaEventType)
{
	unsigned long flags=0;
	QDMA_Adapter_T *qdmaApt = netdev_priv(en7512NapiDev);
	struct ECNT_QDMA_Data qdma_data ;

	if ((qdmaEventType == QDMA_EVENT_RECV_PKTS) ||
        (qdmaEventType == QDMA_EVENT_NO_RX_BUFFER))
	{
        if (qdmaEventType == QDMA_EVENT_NO_RX_BUFFER)
            QDMA_MSG(DBG_WARN, "QDMA_EVENT_NO_RX_BUFFER\n");
    
		spin_lock_irqsave(&napiLock, flags);
		QDMA_MSG(DBG_MSG, "qdma_st_cb_EventHandler come in, qdmaEventType is %s\n"
			, ((qdmaEventType == QDMA_EVENT_RECV_PKTS)?"QDMA_EVENT_RECV_PKTS":"QDMA_EVENT_NO_RX_BUFFER"));

		/* because no dev info available, we always
		 * use nas8's poll function. */
		if (napi_schedule_prep(&qdmaApt->napi))
		{
			QDMA_MSG(DBG_MSG, "napi_schedule_prep come in\n");
			qdma_disable_rxpkt_int(&qdma_data); /* disable RX interrupt. */
			__napi_schedule(&qdmaApt->napi);
		}
		
		spin_unlock_irqrestore(&napiLock, flags);
	}
	else if (qdmaEventType == QDMA_EVENT_TX_CROWDED)
	{
		QDMA_MSG(DBG_WARN, "QDMA_EVENT_TX_CROWDED\n");
	}
	else
		QDMA_MSG(DBG_WARN, "Wrong QDMA Event Type: %d\n", qdmaEventType);

	return 0;
}

static int mt7510_qdma_napiPoll(struct napi_struct *napi, int budget)
{
	int n=0;
	unsigned long flags=0;
	struct ECNT_QDMA_Data qdma_data ;

	/* call mt7510_ptm_rx to receive a packet 
	 * until all packets are received */
	qdma_data.qdma_private.cnt = budget ;
	n = qdma_receive_packets(&qdma_data);
	QDMA_MSG(DBG_MSG, "mt7510_qdma_napiPoll come in, n is %d, budget is %d\n", n, budget);

	if (n < budget)
	{
		spin_lock_irqsave(&napiLock, flags);
	
		__napi_complete(napi);
		qdma_enable_rxpkt_int(&qdma_data);

		spin_unlock_irqrestore(&napiLock, flags);
	}
	
	return n;
}

/******************************************************************************
******************************************************************************/
static int qdma_st_packet_gen(uint channel, uint queue, uint npackets, uint lens, unchar pattern, int ringIdx)
{
	int i=0, k=0;
	struct sk_buff *skb=NULL;
	int tx_len=0;
	unchar *tx_data=NULL;
	unchar tx_seed=0;
	int offset=0;
	ushort chksum=0;
	QDMA_TxMsg_T pTxMsg ;
	int times=0 ;
	struct ECNT_QDMA_Data qdma_data;

	if(umacTestFlag == 1)
		channel = channel % CONFIG_QDMA_RX_CHANNEL;

	QDMA_MSG(DBG_ST, "Loopback test packets=%d, lens=%d, jiffies:%d, channel:%d, queue:%d\n", npackets, lens, (uint)jiffies, channel, queue);

	for (i=0 ; i<npackets ; i++) {
		times = 0 ;
		memset(pTxMsg.msg, 0, sizeof(QDMA_TxMsg_T)) ;
#if defined(TCSUPPORT_AUTOBENCH)
		pTxMsg.raw.channel =0;
		pTxMsg.raw.queue = 0;
#else
		pTxMsg.raw.channel = (channel == CONFIG_QDMA_CHANNEL)?(random32() % CONFIG_QDMA_CHANNEL):channel;
		pTxMsg.raw.queue = (queue == CONFIG_QDMA_QUEUE)?(random32() % CONFIG_QDMA_QUEUE):queue;
#endif
#ifdef QDMA_LAN
		pTxMsg.raw.fport = 0x1 ;
#else
		pTxMsg.raw.fport = 0x2 ;
#endif
		if(loopCnt != 0) {
            pTxMsg.raw.loopcnt= loopCnt;
        }
		
		skb = dev_alloc_skb(CONFIG_LOOPBACK_MAX_PKT_LENS+2);
		if (skb == NULL) {			
			QDMA_MSG(DBG_ST, "skb == NULL, continue;\n") ;
			continue;
		}
		skb->dropcount = 0xdeadbeef;

		if (random32() & 0x1)
			offset = 2;
		else 
			offset = 0;

		skb_reserve(skb, offset);

		if (lens == 0) {
			tx_len = random32() % (CONFIG_LOOPBACK_MAX_PKT_LENS+1);
		} else if(lens == 1) {
			tx_len = (48+i) % (CONFIG_LOOPBACK_MAX_PKT_LENS+1);
		} else {
			tx_len = lens % (CONFIG_LOOPBACK_MAX_PKT_LENS+1);
		}
#if defined(TCSUPPORT_AUTOBENCH) && defined(TCSUPPORT_CPU_MT7520)
		tx_len = (tx_len<64)?64:tx_len ;
#else
		tx_len = (tx_len<48)?48:tx_len ;
#endif
		tx_data = skb_put(skb, tx_len);
		for (k = 0; k < 6; k++)
			tx_data[k] = 0x11;
		for (k = 6; k < 12; k++)
			tx_data[k] = 0x22;

		tx_data[12] = 0x08;
		tx_data[13] = 0x00;

		tx_data[14] = pTxMsg.raw.channel;
		tx_data[15] = pTxMsg.raw.queue;


		
		tx_data[16] = (tx_len)>>8;
		tx_data[17] = (tx_len);

		tx_data[18] = (i+1)>>24;
		tx_data[19] = (i+1)>>16;
		tx_data[20] = (i+1)>>8;
		tx_data[21] = (i+1);

		tx_data[22] = 0;
		tx_data[23] = 0;

		if(pattern == 0x01) {
			tx_data[24] = 0x01 ;	
			tx_seed = (unchar) random32() ;
			tx_data[25] = tx_seed;
            tx_data[26] = 0xAA;
            tx_data[27] = 0xBB;
            tx_data[28] = ringIdx;

			for(k=29 ; k<tx_len ; k++) {
				tx_seed++;
				tx_data[k] = (unchar) (tx_seed & 0xff);
			}
		} else {
			tx_data[24] = 0x00;
			tx_data[25] = pattern;

			for(k=26 ; k<tx_len ; k++) {
				tx_data[k] = pattern;
			}
		}

		chksum = __st_in_csum((unsigned short *) (skb->data), tx_len-4);
		tx_data[22] = (chksum >> 8) & 0xff;
		tx_data[23] = chksum & 0xff;

		if(ringIdx == 1)
			pTxMsg.raw.oam = 1 ;
		do {
			if(i%10000 == 9999) {
				QDMA_MSG(DBG_ST, "TX PKT: there are %d packets has been sent.\n", i+1) ;
				msleep(100) ;
			}
			qdma_data.qdma_private.qdma_transmit.msg0 = pTxMsg.msg[0];
			qdma_data.qdma_private.qdma_transmit.msg1 = pTxMsg.msg[1];
			qdma_data.qdma_private.qdma_transmit.skb = skb;
			qdma_data.qdma_private.qdma_transmit.pMacInfo = NULL;
			if(qdma_transmit_packet(&qdma_data) == 0) {
				break;
			}
		} while(times--);
		if(times < 0) {
			dev_kfree_skb_any(skb);
		}
		if(delayTime) {
			msleep(delayTime);
		}
	}
	QDMA_MSG(DBG_ST, "TX PKT: there are %d packets has been sent.\n", i);

	return 0;
}

/******************************************************************************
******************************************************************************/
static int qdma_st_cb_rx_packet(void *msg_p, uint msg_len, struct sk_buff *skb, uint rx_len)
{
	int k=0, len=0;
	unchar *rx_data=NULL;
	unchar rx_seed=0, pattern=0;
	int idx, channel=0, queue=0, lens=0 ;
    int ring_index=0;
	ushort chksum=0;
	volatile uint timer_now=0, timer_last=0, passtime=0;

	skb_put(skb, rx_len) ;
	rx_data = skb->data;
	if(rx_len > nlen)
		QDMA_MSG(DBG_ST, "rx_len error: rx_len is %d\n", rx_len);
		
	for (k = 0; k < 6; k++) {
		if (rx_data[k] != 0x11) {
			QDMA_ERR("loopback fail: dst mac unmatch\n");
			goto err;
		}
	}
	
	for (k = 6; k < 12; k++) {
		if (rx_data[k] != 0x22) {
			QDMA_ERR("loopback fail: src mac unmatch\n");
			goto err;
		}
	}
	
	if ((rx_data[12] != 0x08) || (rx_data[13] != 0x00)) {
		QDMA_ERR("loopback fail: ether type unmatch\n");
		goto err;
	}
	
	channel = rx_data[14]&(CONFIG_QDMA_CHANNEL-1);
	queue = rx_data[15]&(CONFIG_QDMA_QUEUE-1);
	lens = (rx_data[16]<<8) | rx_data[17];
	idx = (rx_data[18]<<24) | (rx_data[19]<<16) | (rx_data[20]<<8) | rx_data[21];
	chksum = (rx_data[22]<<8) | rx_data[23];
	if(msec) {
		delay1ms(msec);
	} else {
	 	timer_last = VPint(CR_TIMER1_VLR);
		
		if(rx_data[24] == 0x01) {
			rx_seed = rx_data[25];
			len = rx_len;
            if((rx_data[26] == 0xAA) && (rx_data[27] == 0xBB)){
                ring_index = rx_data[28];
                rxCounter[ring_index]++;
            }
			for(k=29 ; k<len ; k++) {
				rx_seed++;
				if (rx_data[k] != (unchar) (rx_seed & 0xff)) {
					QDMA_ERR("loopback fail: random payload unmatch pos=%04x\n", k);
					goto err;
				}
			}
		}
		else {
			pattern = rx_data[25] ;
			for(k=26 ; k<rx_len ; k++) {
				if (rx_data[k] != pattern) {
					QDMA_ERR("loopback fail: payload unmatch pos=%04x, pattern=%.2x\n", k, pattern);
					goto err;
				} 
			}		
		}
		
   		timer_now = VPint(CR_TIMER1_VLR);
       	if (timer_last >= timer_now)
       		passtime = timer_last - timer_now;
      	else
       		passtime = VPint(CR_TIMER1_LDV) - timer_now + timer_last;
		aveTime += passtime;
		if(passtime > (VPint(CR_TIMER1_LDV)/5))
			QDMA_MSG(DBG_ST, "too long passtime is 0x%08x\n", passtime);
	}

	if (__st_in_csum((unsigned short *) (skb->data), lens-4) != 0) {
		QDMA_ERR("loopback fail: ip checksum unmatch\n");
		goto err;
	}
	counters[channel][queue].rx_frames++ ;
	
	if((idx%10000) == 0) {
		QDMA_MSG(DBG_ST, "RX PKT: the %d packet has been received. jiffies:%d\n", idx, (uint)jiffies);
	}
	dev_kfree_skb_any(skb);
	return 0;
	
err:
	printk("Raw data for receive packet, skb->len:%d, rxDscp.ctrl.pkt_len:%d, lens:%d\n", skb->len, rx_len, lens) ;
	counters[channel][queue].rx_err_frames++ ;
	__st_dump_skb(skb);
	dev_kfree_skb_any(skb);
	return -1;
}


/*****************************************************************************
******************************************************************************/
#if 0
static int qdma_st_cb_tx_finished(void *msg_p)
{
	QDMA_TxMsg_T *pTxMsg = (QDMA_TxMsg_T *)msg_p ;
	uint channel=0, queue=0 ;
	
	if (pTxMsg == NULL)
		QDMA_ERR("unexpected error: pTxMsg is NULL, pTxMsg is 0x%08x\r\n", (uint)pTxMsg);
	
	channel = pTxMsg->raw.channel ;
	queue = pTxMsg->raw.queue ;
	counters[channel][queue].tx_frames++;
	
	return 0 ;
}
#endif

#ifndef CONFIG_TX_POLLING_BY_MAC
/******************************************************************************
******************************************************************************/
static int qdma_st_rx_polling(void) 
{
	struct ECNT_QDMA_Data qdma_data ;

	while(gpQdmaPriv->devCfg.flags.isRxPolling == QDMA_ENABLE) {
		qdma_data.qdma_private.cnt = 128 ;
		qdma_receive_packets(&qdma_data) ;
		msleep(random32()%10) ;
	}
	
	return 0 ;
}
#endif /* CONFIG_TX_POLLING_BY_MAC */

/****************************************************************************
 to verify read/write int mask register
*****************************************************************************/
static void qdma_st_csr_intmask(uint times)
{
	uint value = 0xA215A ;
	uint reg = 0 ;
	uint i = 0 ;
	uint base = gpQdmaPriv->csrBaseAddr ;
	
	while(times--) {
		iowrite32(value, (void __iomem *)(QDMA_CSR_INT_ENABLE(base))) ;
		
		reg = ioread32((void __iomem *)(QDMA_CSR_INT_ENABLE(base))) ;
		if(reg != value) {
			printk("========>Get the INT_ENABLE register error: Write:%.8x, Read:%.8x\n", value, reg) ;
			i++ ;
		}
	}
	
	if(i) {
		printk("Failed to verify the INT_ENABLE register read/write, %d\n", i) ;
	} else {
		printk("Successful to verify the INT_ENABLE register read/write\n") ;
	}
	
	return ;	
}

/******************************************************************************
******************************************************************************/
static int qdma_st_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
	int i = 0, j = 0;
	int index = 0;
	off_t pos = 0;
	off_t begin = 0;

	index += sprintf(buf+index, " C   Q     Tx Frames    Rx OK Frames    Rx Err Frames \n");
	CHK_BUF();
	for(i=0 ; i<CONFIG_QDMA_CHANNEL ; i++) {
		for(j=0 ; j<CONFIG_QDMA_QUEUE ; j++) {
			if(counters[i][j].tx_frames) {
				index += sprintf(buf+index, " %2d  %d   %12d    %12d    %12d\n", i, j, counters[i][j].tx_frames, 
																						counters[i][j].rx_frames, 
																						counters[i][j].rx_err_frames) ;
				CHK_BUF();
			}
		}
	}

	*eof = 1;

done:
	*start = buf + (off - begin);
	index -= (off - begin);
	if (index<0) 
		index = 0;
	if (index>count) 
		index = count;
	return index;
}

/******************************************************************************
 Descriptor:	It's used to configure dbg counter, which has 40 groups to set.
 Input Args:		- cntrIdx:
 				- cntrEn:
 				- cntrSrc:
 				- chnlIdx:
 				- queueIdx:
 				- dscpRingIdx:
 				- isChnlAll:
 				- isQueueAll:
 				- isDscpRingAll:
 Ret Value:		0: successful, otherwise failed.
******************************************************************************/
int qdma_set_dbg_cntr_info(QDMA_DBG_CNTR_T *dbgCntrPtr)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	unchar idx = dbgCntrPtr->cntrIdx;
	
	if(dbgCntrPtr->cntrIdx<0 || dbgCntrPtr->cntrIdx>=CONFIG_MAX_CNTR_NUM) {
		QDMA_ERR("Fault: dbg Cntr counter index shoule between 0 and %d\n", CONFIG_MAX_CNTR_NUM-1) ; 
		return -EINVAL ;
	}
	if(dbgCntrPtr->cntrSrc<0 || dbgCntrPtr->cntrSrc>=QDMA_DBG_CNTR_SRC_ITEMS) {
		QDMA_ERR("Fault: dbg Cntr source shoule between 0 and %d\n", QDMA_DBG_CNTR_SRC_ITEMS-1) ; 
		return -EINVAL ;
	}
	if((dbgCntrPtr->isChnlAll == QDMA_DISABLE) && (dbgCntrPtr->chnlIdx<0 || dbgCntrPtr->chnlIdx>=CONFIG_QDMA_CHANNEL)) {
		QDMA_ERR("Fault: Channel index shoule between 0 and %d\n", CONFIG_QDMA_CHANNEL-1) ; 
		return -EINVAL ;
	}
	if((dbgCntrPtr->isQueueAll == QDMA_DISABLE) && (dbgCntrPtr->queueIdx<0 || dbgCntrPtr->queueIdx>=CONFIG_QDMA_QUEUE)) {
		QDMA_ERR("Fault: Queue index shoule between 0 and %d\n", CONFIG_QDMA_QUEUE-1) ; 
		return -EINVAL ;
	}
	if((dbgCntrPtr->isDscpRingAll == QDMA_DISABLE) && (dbgCntrPtr->dscpRingIdx<0 || dbgCntrPtr->dscpRingIdx>1)) {
		QDMA_ERR("Fault: ringIdx shoule be 0 or 1\n") ; 
		return -EINVAL ;
	}

	qdmaSetCntrSrc(base, idx, dbgCntrPtr->cntrSrc);
	qdmaSetCntrChnlIdx(base, idx, dbgCntrPtr->chnlIdx);
	qdmaSetCntrQueueIdx(base, idx, dbgCntrPtr->queueIdx);
	qdmaSetCntrDscpIdx(base, idx, dbgCntrPtr->dscpRingIdx);
	
	if(dbgCntrPtr->isChnlAll == QDMA_DISABLE) {
		qdmaSetCntrChnlSingle(base,idx);
	} else {
		qdmaSetCntrChnlAll(base,idx);
	}
	
	if(dbgCntrPtr->isQueueAll == QDMA_DISABLE) {
		qdmaSetCntrQueueSingle(base,idx);
	} else {
		qdmaSetCntrQueueAll(base,idx);
	}
	
	if(dbgCntrPtr->isDscpRingAll == QDMA_DISABLE) {
		qdmaSetCntrDscpSingle(base,idx);
	} else {
		qdmaSetCntrDscpAll(base,idx);
	}
	
	if(dbgCntrPtr->cntrEn == QDMA_DISABLE) {
		qdmaDisableCntrCfg(base, idx);
		qdmaClearCntrCounter(base, idx);
	} else {
		qdmaEnableCntrCfg(base, idx);
	}
	
	return 0 ;
}

int qdma_clear_and_enable_dbg_cntr_info(QDMA_DBG_CNTR_T *dbgCntrPtr)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	unchar idx = dbgCntrPtr->cntrIdx;
	
	if(dbgCntrPtr->cntrIdx<0 || dbgCntrPtr->cntrIdx>=CONFIG_MAX_CNTR_NUM) {
		QDMA_ERR("Fault: dbg Cntr counter index shoule between 0 and %d\n", CONFIG_MAX_CNTR_NUM-1) ; 
		return -EINVAL ;
	}
	
	qdmaDisableCntrCfg(base, idx);
	qdmaClearCntrCounter(base, idx);
	dbgCntrPtr->cntrEn = QDMA_ENABLE ;
	qdma_set_dbg_cntr_info(dbgCntrPtr);

	return 0 ;
}

/******************************************************************************
 Description:	Input a self test command and start the QDMA self testing. 
 				This test process is generated packets by QDMA, send to 
 				TXDMA, receive from RXDMA and check the receive contents.
 Proc Command:	"init": 			Init the QDMA TX/RX DSCP and driver
 				"csr1 pkts lens":	Verification for NO_RX_CPU_DSCP_INT and NO_TX_CPU_DSCP_INT
 				"csr2 pkts lens":	Verification for NO_LINK_DSCP interrupt
 				"csr3":				Verification for CHCCK_DONE bit enable/disable
 				"csr4":				Verification for for RX_DONE_INT and RX_DLY_INT
 				"csr5":				Verification for for TX_DONE_INT and TX_DLY_INT
 				"tc1 pkts lens":	Process the test case 1
 				"tc2":				Process the test case 2
 				"tc3 pkts lens":	Process the test case 3
 				"tc4 pkts lens":	Process the test case 4
 				"tc5 pkts lens":	Process the test case 5
 				"tc6 pkts lens":	Process the test case 6
 				"tcall pkts lens":	Process all test case in sequential. (tc6, tc1, tc2~tc5)
******************************************************************************/
static int tc_init(char subcmd[32], uint base)
{
	struct net_device *dev=NULL;
	QDMA_Adapter_T *qdmaApt=NULL;
    uint dbg=0 ;
    QDMA_InitCfg_t initCfg ;
	int error=0;
	int ringIdx=0;
	struct ECNT_QDMA_Data qdma_data;
	
    qdmaSetIrqPtime(base, 0x64);
    msleep(200);
    memset(&initCfg, 0, sizeof(QDMA_InitCfg_t)) ;
    //initCfg.cbXmitFinish = qdma_st_cb_tx_finished ;
    initCfg.cbRecvPkts = qdma_st_cb_rx_packet ;
    initCfg.cbEventHandler = qdma_st_cb_EventHandler;
	qdma_data.qdma_private.pInitCfg = &initCfg;
	qdma_init(&qdma_data) ;
    umacTestFlag = 0;
    if(!strcmp(subcmd, "umac")) {
		qdma_data.qdma_private.lbMode = QDMA_LOOPBACK_UMAC ;
		qdma_loopback_mode(&qdma_data) ;
    	FE_API_SET_DROP_UDP_CHKSUM_ERR_ENABLE(FE_GDM_SEL_GDMA2, FE_DISABLE);
    	FE_API_SET_DROP_TCP_CHKSUM_ERR_ENABLE(FE_GDM_SEL_GDMA2, FE_DISABLE);
    	FE_API_SET_DROP_IP_CHKSUM_ERR_ENABLE(FE_GDM_SEL_GDMA2, FE_DISABLE);
    	FE_API_SET_DROP_CRC_ERR_ENABLE(FE_GDM_SEL_GDMA2, FE_DISABLE);
    	FE_API_SET_DROP_RUNT_ENABLE(FE_GDM_SEL_GDMA2, FE_DISABLE);
    	FE_API_SET_DROP_LONG_ENABLE(FE_GDM_SEL_GDMA2, FE_DISABLE);
    	umacTestFlag = 1;
#if defined(TCSUPPORT_AUTOBENCH) && defined(TCSUPPORT_CPU_MT7520)
    } else if(!strcmp(subcmd, "eponmac")) {
		qdma_data.qdma_private.lbMode = QDMA_LOOPBACK_DISABLE ;
		qdma_loopback_mode(&qdma_data) ;
    	 /* switch to epon mac */
    	IO_SREG(0xbfb00070,1); 
    	 /* set epon mac loopback */
    	IO_SREG(0xbfb66000,0x46);	
    	/* disable epon mac isr */
    	qdmaDisableInt(base, INT_MASK_EPON_MAC) ; 
    	IO_SREG(0xbfb66008,0);  
    	
    	FE_API_SET_DROP_UDP_CHKSUM_ERR_ENABLE(FE_GDM_SEL_GDMA2, FE_DISABLE);
    	FE_API_SET_DROP_TCP_CHKSUM_ERR_ENABLE(FE_GDM_SEL_GDMA2, FE_DISABLE);
    	FE_API_SET_DROP_IP_CHKSUM_ERR_ENABLE(FE_GDM_SEL_GDMA2, FE_DISABLE);
    } else if(!strcmp(subcmd, "eponphy")) {
		qdma_data.qdma_private.lbMode = QDMA_LOOPBACK_DISABLE ;
		qdma_loopback_mode(&qdma_data) ;
    	/* switch to epon mac */
    	IO_SREG(0xbfb00070,1);
    	/* set epon phy */
    	IO_SREG(0xbfaf0124,0x7f000000);
    	IO_SREG(0xbfaf0108,0xcd810110);
    	mdelay(1);
    	IO_SREG(0xbfaf0108,0x45810110);
    	IO_SREG(0xbfaf0428,0x1);
    	
    	/* disable epon mac isr */
    	qdmaDisableInt(base, INT_MASK_EPON_MAC) ; 
    	IO_SREG(0xbfb66008,0);  

    	IO_SREG(0xbfb66000,0x43);

    	FE_API_SET_DROP_UDP_CHKSUM_ERR_ENABLE(FE_GDM_SEL_GDMA2, FE_DISABLE);
    	FE_API_SET_DROP_TCP_CHKSUM_ERR_ENABLE(FE_GDM_SEL_GDMA2, FE_DISABLE);
    	FE_API_SET_DROP_IP_CHKSUM_ERR_ENABLE(FE_GDM_SEL_GDMA2, FE_DISABLE);
#endif
    } else{
		qdma_data.qdma_private.lbMode = QDMA_LOOPBACK_QDMA ;
		qdma_loopback_mode(&qdma_data) ;
    }
	
	qdma_data.qdma_private.mode = QDMA_ENABLE ;
	qdma_tx_dma_mode(&qdma_data) ;
	qdma_data.qdma_private.mode = QDMA_ENABLE ;
	qdma_rx_dma_mode(&qdma_data) ;
    if(QDMA_TX_MODE == QDMA_TX_POLLING) {
    	kernel_thread((int (*)(void *))qdma_polling_tx_recycle, NULL, 0) ;
    }

    if(QDMA_RX_MODE == QDMA_RX_NAPI) {
    	dev = alloc_netdev(sizeof(QDMA_Adapter_T), test_dev, ether_setup);
    	if (!dev)
    	{
    		printk("\n\nFAILED: qdma_dvt net_device allocation\n\n");
    		return -ENOMEM;
    	}

    	en7512NapiDev = dev;
    	qdmaApt = netdev_priv(dev);
    	memset(qdmaApt, 0, sizeof(QDMA_Adapter_T));
    	
    	en7512NapiDev->netdev_ops = &qdmaDvt_netdev_ops;
    	qdmaApt->napi.weight = QDMA_NAPI_WEIGHT;
    	netif_napi_add(dev, &qdmaApt->napi, mt7510_qdma_napiPoll, qdmaApt->napi.weight);
    	
    	error = register_netdev(dev);
    	if (error)
    	{
    		printk("\n\nFAILED: qdma_dvt net_device register fail\n\n");
    		free_netdev(dev);
    		return error;
    	}
    	
    	set_bit(__LINK_STATE_START, &en7512NapiDev->state);
    	
    	if (en7512NapiDev->netdev_ops->ndo_open){
    		error = en7512NapiDev->netdev_ops->ndo_open(en7512NapiDev);
    		if (error)
    			clear_bit(__LINK_STATE_START, &en7512NapiDev->state);
    	}
    }

    QDMA_MSG(DBG_ST, "Prepare %d receive packet buffers in QDMA init stage.\n", dbg) ;
    QDMA_MSG(DBG_ST, "CSR info: INFO:%.8x, GLG:%.8x, INT_MASK:%.8x\n", 
    														qdmaGetQdmaInfo(base), 
    														qdmaGetGlbCfg(base), 
    														qdmaGetIntMask(base)) ;
    for(ringIdx=0; ringIdx<TX_RING_NUM; ringIdx++) {
    	QDMA_MSG(DBG_ST, "CSR info_%d: TX_DSCP_BASE:%.8x, TX_CPU_IDX:%d, TX_DMA_IDX:%d\n",
    															ringIdx, 
    															qdmaGetTxDscpBase(base, ringIdx), 
    															qdmaGetTxCpuIdx(base, ringIdx), 
    															qdmaGetTxDmaIdx(base, ringIdx)) ;
    }
    for(ringIdx=0; ringIdx<RX_RING_NUM; ringIdx++) {
    	QDMA_MSG(DBG_ST, "CSR info_%d: RX_DSCP_BASE:%.8x, RX_CPU_IDX:%d, RX_DMA_IDX:%d\n", 
    															ringIdx, 
    															qdmaGetRxDscpBase(base, ringIdx), 
    															qdmaGetRxCpuIdx(base, ringIdx), 
    															qdmaGetRxDmaIdx(base, ringIdx)) ;
    }
    return 0;
}

static int tc_csr1(uint base, uint packets, uint lens, int ringIdx)
{
    uint oldInt = qdmaGetIntMask(base) ; 
    uint channel = random32() % CONFIG_QDMA_CHANNEL ;
    uint queue = random32() % CONFIG_QDMA_QUEUE ;
    
    qdmaSetIntMask(base, (  INT_STATUS_NO_RX1_CPU_DSCP|
                            INT_STATUS_NO_TX1_CPU_DSCP|
                            INT_STATUS_TX1_DONE|
                            INT_STATUS_NO_RX0_CPU_DSCP|
                            INT_STATUS_NO_TX0_CPU_DSCP|
                            INT_STATUS_TX0_DONE)) ;

    memset(counters, 0, sizeof(QDMA_DbgCounters_T)*CONFIG_QDMA_CHANNEL*CONFIG_QDMA_QUEUE) ;
    gpQdmaPriv->counters.intNoRxDscp[ringIdx] = 0 ;
    gpQdmaPriv->counters.intNoTxDscp[ringIdx] = 0 ;
    gpQdmaPriv->devCfg.waitTxMaxNums = 0 ;
    gpQdmaPriv->devCfg.countDown = (random32()%(gpQdmaPriv->txDscpNum[ringIdx]-1))+1 ;
    printk("1st countDown  %d \n",gpQdmaPriv->devCfg.countDown);
    gpQdmaPriv->devCfg.countDownRound = 0 ;
    qdma_st_packet_gen(channel, queue, packets, lens, 0x01, ringIdx) ;

    /* in order to set the TX_CPU_PTR to HW register */
    gpQdmaPriv->devCfg.countDown = 1 ;
    gpQdmaPriv->devCfg.waitTxMaxNums = 1 ;
    qdma_st_packet_gen(channel, queue, 1, lens, 0x01, ringIdx) ;
    qdma_bm_receive_packets(0, ringIdx) ;
    msleep(1000) ;
    printk("INT_CSR:%.8x, Packets:%d, NO_TX_DSCP_INT Cnt:%d, NO_RX_DSCP_INT Cnt:%d, CntDownRound:%d\n\n", 
        qdmaGetIntStatus(base), packets, gpQdmaPriv->counters.intNoTxDscp[ringIdx], 
        gpQdmaPriv->counters.intNoRxDscp[ringIdx], gpQdmaPriv->devCfg.countDownRound) ;
    __st_print_counters() ;

#if 0
    if(ringIdx == 0)
        intNums = packets+1-(CONFIG_RX0_DSCP_NUM-1)+1;
    else
        intNums = packets+1-(CONFIG_RX1_DSCP_NUM-1)+1;
#endif
    if((__st_check_counters() == 0) && (gpQdmaPriv->counters.intNoRxDscp[ringIdx] > 0) && 
        (gpQdmaPriv->counters.intNoTxDscp[ringIdx] == gpQdmaPriv->devCfg.countDownRound)) {
        QDMA_LOG("csr1: Test Result:Pass\n\n") ;
    } else {
        QDMA_LOG("csr1: Test Result:Failed\n\n") ;
    }
    qdmaSetIntMask(base, oldInt) ;
    return 0;
}

static int tc_csr2(uint base, uint packets, uint lens, int ringIdx)
{
    uint oldInt = qdmaGetIntMask(base) ; 
	uint channel = random32() % CONFIG_QDMA_CHANNEL ;
	uint queue = random32() % CONFIG_QDMA_QUEUE ;
	uint intNums = 0;

	if(gpQdmaPriv->txDscpNum[0] < 1024) {
		printk("Failed: the number of TX DSCP must larger than 1024.\n") ;
		return 0 ;
	}
	
	qdmaSetIntMask(base, (INT_STATUS_TX0_DONE|INT_STATUS_HWFWD_DSCP_EMPTY)) ;

	memset(counters, 0, sizeof(QDMA_DbgCounters_T)*CONFIG_QDMA_CHANNEL*CONFIG_QDMA_QUEUE) ;
	gpQdmaPriv->counters.intNoLinkDscp = 0 ;
	qdma_st_packet_gen(channel, queue, packets, lens, 0x01, 0) ;
	msleep(1000) ;
	printk("receive %d packet\n",qdma_bm_receive_packets(0,0)) ;
	msleep(1000) ;

	intNums = packets/(CONFIG_HWFWD_DSCP_NUM+CONFIG_RX0_DSCP_NUM);
	if(CONFIG_TX0_DSCP_NUM <= CONFIG_HWFWD_DSCP_NUM)
		intNums = 0;
	printk("INT_CSR:%.8x, Packets:%d, NO_LINK_DSCP_INT Count:%d intNums_expect:%d\n\n", 
	    qdmaGetIntStatus(base), packets, gpQdmaPriv->counters.intNoLinkDscp,intNums) ;
	__st_print_counters() ;

	if((__st_check_counters() == 0) && (gpQdmaPriv->counters.intNoLinkDscp >= intNums) && 
        (gpQdmaPriv->counters.intNoLinkDscp <= (intNums*2))) {
		QDMA_LOG("csr2: Test Result:Pass\n\n") ;
	} else {
		QDMA_LOG("csr2: Test Result:Failed\n\n") ;
	}

	qdmaSetIntMask(base, oldInt) ;
    return 0;
}

static int tc_csr3(uint base, uint packets, uint lens, int ringIdx)
{
    uint oldInt = qdmaGetIntMask(base) ; 
    uint oldGlb = qdmaGetGlbCfg(base) ;
    uint channel = random32() % CONFIG_QDMA_CHANNEL ;
    uint queue = random32() % CONFIG_QDMA_QUEUE ;
	uint csr3_round_cnt = 0 ;
    uint csr[2] = {(oldGlb&=~GLB_CFG_CHECK_DONE), (oldGlb|GLB_CFG_CHECK_DONE)} ;
    uint i=0, j=0, flag=0 ;
    qdmaSetIntMask(base, (INT_STATUS_TX0_DONE|INT_MASK_RX0_COHERENT|INT_MASK_TX0_COHERENT|
        INT_STATUS_TX1_DONE|INT_MASK_RX1_COHERENT|INT_MASK_TX1_COHERENT)) ;
    qdmaSetGlbCfg(base, (oldGlb&=~GLB_CFG_CHECK_DONE)) ;

	if(ringIdx == RING_IDX_0)
		csr3_round_cnt = CSR3_ROUND_CNT ;
	else
		csr3_round_cnt = 60 ;

    memset(counters, 0, sizeof(QDMA_DbgCounters_T)*CONFIG_QDMA_CHANNEL*CONFIG_QDMA_QUEUE) ;

	printk("wait for all dscp's ctrl done set to 1...\n");
    /* set all DSCP done bit to 1 */
    for(i=0 ; i<(gpQdmaPriv->txDscpNum[1]+gpQdmaPriv->txDscpNum[0]+
        gpQdmaPriv->rxDscpNum[1]+gpQdmaPriv->rxDscpNum[0]) ; i++) {
        ((struct QDMA_DscpInfo_S *)gpQdmaPriv->dscpInfoAddr + i)->dscpPtr->ctrl.done = 1 ;
    }
    gpQdmaPriv->devCfg.rxDscpDoneBit = 1 ;
    gpQdmaPriv->devCfg.txDscpDoneBit = 1 ;
    delayTime = 100;

	printk("ctrl done set to 1 finished, csr3 test begin\n");
    for(i=0 ; i<2 ; i++) {
        qdmaSetGlbCfg(base, csr[i]) ;
        
        gpQdmaPriv->counters.intRxCoherent[ringIdx] = 0 ;
        gpQdmaPriv->counters.intTxCoherent[ringIdx] = 0 ;
        for(j=0 ; j<packets/csr3_round_cnt ; j++) {
            qdma_st_packet_gen(channel, queue, csr3_round_cnt, lens, 0x01, ringIdx) ;
            qdma_bm_receive_packets(csr3_round_cnt, ringIdx) ;
        }
        printk("CHECK_DONE:%d, GLB_CSR:%.8x, INT_CSR:%.8x, RX_COHERENT_INT Count:%d, \
            TX_COHERENT_INT Count:%d\n\n", i, qdmaGetGlbCfg(base), qdmaGetIntStatus(base), 
            gpQdmaPriv->counters.intRxCoherent[ringIdx], gpQdmaPriv->counters.intTxCoherent[ringIdx]) ;
		if((i == 0) && (gpQdmaPriv->counters.intRxCoherent[ringIdx] == 0) 
					&& (gpQdmaPriv->counters.intTxCoherent[ringIdx] == 0)){
			flag++;
		}
		if((i == 1) && (gpQdmaPriv->counters.intRxCoherent[ringIdx] == packets/csr3_round_cnt*csr3_round_cnt) 
					&& (gpQdmaPriv->counters.intTxCoherent[ringIdx] == packets/csr3_round_cnt*csr3_round_cnt*2)){
			flag++;
		}
    }
    __st_print_counters() ;

	if(flag == 2){
		QDMA_LOG("csr3: Test Result:Pass\n\n") ;
	} else {
		QDMA_LOG("csr3: Test Result:Failed\n\n") ;
	}

	printk("csr3 test finished, wait for ctrl done clear to 0...\n");
    /* clear all DSCP done bit to 0 */
    for(i=0 ; i<(gpQdmaPriv->txDscpNum[1]+gpQdmaPriv->txDscpNum[0]+
        gpQdmaPriv->rxDscpNum[1]+gpQdmaPriv->rxDscpNum[0]) ; i++) {
        ((struct QDMA_DscpInfo_S *)gpQdmaPriv->dscpInfoAddr + i)->dscpPtr->ctrl.done = 0 ;
    }
    gpQdmaPriv->devCfg.rxDscpDoneBit = 0 ;
    gpQdmaPriv->devCfg.txDscpDoneBit = 0 ;
    delayTime = 0;
	printk("ctrl done clear to 0 finished\n");
    
    qdmaClearIntStatus(base, 0xFFFFFFFF) ;
    qdmaSetGlbCfg(base, oldGlb) ;
    qdmaSetIntMask(base, oldInt) ;
    return 0;
}

static int tc_csr4(uint base, uint packets, uint lens, int ringIdx)
{

    uint oldRxDly = qdmaGetRxDelayIntCfg(base) ;
    uint channel = random32() % CONFIG_QDMA_CHANNEL ;
    uint queue = random32() % CONFIG_QDMA_QUEUE ;
    uint prevIntRxDone = 0 ;
    uint itemMax[2] = {5, 10} ;
    uint i=0, j=0 ;
	uint result=0 ;
    uint csr[11] = {0, (DLY_INT_RXDLY_INT_EN | (25<<DLY_INT_RXMAX_PINT_SHIFT) | (0<<DLY_INT_RXMAX_PTIME_SHIFT)), 
                      (DLY_INT_RXDLY_INT_EN | (50<<DLY_INT_RXMAX_PINT_SHIFT) | (0<<DLY_INT_RXMAX_PTIME_SHIFT)), 
                      (DLY_INT_RXDLY_INT_EN | (100<<DLY_INT_RXMAX_PINT_SHIFT) | (0<<DLY_INT_RXMAX_PTIME_SHIFT)), 
                      (DLY_INT_RXDLY_INT_EN | (127<<DLY_INT_RXMAX_PINT_SHIFT) | (0<<DLY_INT_RXMAX_PTIME_SHIFT)), 
                    0, (DLY_INT_RXDLY_INT_EN | (0<<DLY_INT_RXMAX_PINT_SHIFT) | (40<<DLY_INT_RXMAX_PTIME_SHIFT)), 
                      (DLY_INT_RXDLY_INT_EN | (0<<DLY_INT_RXMAX_PINT_SHIFT) | (80<<DLY_INT_RXMAX_PTIME_SHIFT)), 
                      (DLY_INT_RXDLY_INT_EN | (0<<DLY_INT_RXMAX_PINT_SHIFT) | (200<<DLY_INT_RXMAX_PTIME_SHIFT)), 
                      (DLY_INT_RXDLY_INT_EN | (0<<DLY_INT_RXMAX_PINT_SHIFT) | (255<<DLY_INT_RXMAX_PTIME_SHIFT))} ;

    for(i=0, j=0 ; j<2 ; j++) {
        if(j == 0)
            printk("RX_PTIME Test: PINT incress, RX_INT Count should decress!\n\n");
        else
            printk("RX_PINT Test: PTIME incress, RX_INT Count should decress!\n\n");
        for( ; i<itemMax[j] ; i++) {
            qdmaSetRxDelayIntCfg(base, ringIdx, csr[i]) ;
            gpQdmaPriv->counters.intRxDone[ringIdx] = 0 ;
            qdma_st_packet_gen(channel, queue, packets, 0, 0x01, ringIdx) ;
            msleep(100) ;
            printk("Rx Delay Pkt:%d, Time:%d*20us, DlyCSR:%.8x, Packets:%d, RX_INT Count:%d\n\n", 
                                        ((qdmaGetRxDelayIntCfg(base)&DLY_INT_RXMAX_PINT_MASK)>>DLY_INT_RXMAX_PINT_SHIFT) ,
                                        ((qdmaGetRxDelayIntCfg(base)&DLY_INT_RXMAX_PTIME_MASK)>>DLY_INT_RXMAX_PTIME_SHIFT),
                                        qdmaGetRxDelayIntCfg(base), packets, gpQdmaPriv->counters.intRxDone[ringIdx]) ;
            if((i != 0) && (i != 5)) {
                if(gpQdmaPriv->counters.intRxDone[ringIdx] > prevIntRxDone)
                    result = -1;
            }               
            prevIntRxDone = gpQdmaPriv->counters.intRxDone[ringIdx] ;
            msleep(2000) ;
        }
    }

    if(result == 0)
        QDMA_LOG("csr4: Test Result:Pass\n\n") ;
    else
        QDMA_LOG("csr4: Test Result:Failed\n\n") ;
    
    qdmaSetRxDelayIntVal(base, oldRxDly) ;
    return 0;
}

static int tc_csr5(uint base, uint packets, uint lens, int ringIdx)
{
    uint oldGlb = qdmaGetGlbCfg(base) ;
    uint oldTxDly = qdmaGetTxDelayIntCfg(base) ;
    uint channel = random32() % CONFIG_QDMA_CHANNEL ;
    uint queue = random32() % CONFIG_QDMA_QUEUE ;
    uint csr[5] = {0, (DLY_INT_TXDLY_INT_EN(ringIdx) | (25<<DLY_INT_TXMAX_PINT_SHIFT(ringIdx)) | (0<<DLY_INT_TXMAX_PTIME_SHIFT(ringIdx))), 
                      (DLY_INT_TXDLY_INT_EN(ringIdx) | (50<<DLY_INT_TXMAX_PINT_SHIFT(ringIdx)) | (0<<DLY_INT_TXMAX_PTIME_SHIFT(ringIdx))), 
                      (DLY_INT_TXDLY_INT_EN(ringIdx) | (50<<DLY_INT_TXMAX_PINT_SHIFT(ringIdx)) | (80<<DLY_INT_TXMAX_PTIME_SHIFT(ringIdx))), 
                      (DLY_INT_TXDLY_INT_EN(ringIdx) | (50<<DLY_INT_TXMAX_PINT_SHIFT(ringIdx)) | (200<<DLY_INT_TXMAX_PTIME_SHIFT(ringIdx)))} ;
    uint i=0;
    qdmaDisableIrqEn(base);
    for(i=0 ; i<5 ; i++) {
        qdmaSetTxDelayIntCfg(base, ringIdx, csr[i]) ;
        gpQdmaPriv->counters.intTxDone[ringIdx] = 0 ;
        qdma_st_packet_gen(channel, queue, packets, 0, 0x01, ringIdx) ;
        printk("Tx Delay Pkt:%d, Time:%d*20us, DlyCSR:%.8x, Packets:%d, TX_INT Count:%d\n\n", 
                                    ((qdmaGetTxDelayIntCfg(base)&DLY_INT_TXMAX_PINT_MASK(ringIdx))>>DLY_INT_TXMAX_PINT_SHIFT(ringIdx)) ,
                                    ((qdmaGetTxDelayIntCfg(base)&DLY_INT_TXMAX_PTIME_MASK(ringIdx))>>DLY_INT_TXMAX_PTIME_SHIFT(ringIdx)),
                                    qdmaGetTxDelayIntCfg(base), packets, gpQdmaPriv->counters.intTxDone[ringIdx]) ;
        msleep(2000) ;
    }
    qdmaSetGlbCfg(base, oldGlb) ;
    qdmaSetTxDelayIntVal(base, oldTxDly) ;
    printk("Please reboot the system to verify another test case\n") ;
    return 0;
}


static int tc_tk1(uint base, uint packets, uint lens, int ringIdx)
{
    uint channel = 0 ;
    uint queue = 0;

    memset(counters, 0, sizeof(QDMA_DbgCounters_T)*CONFIG_QDMA_CHANNEL*CONFIG_QDMA_QUEUE) ;
    for(channel=0; channel<CONFIG_QDMA_CHANNEL; channel++) {
        for(queue=0; queue<CONFIG_QDMA_QUEUE; queue++) {
            qdma_st_packet_gen(channel, queue, packets, lens, 0x01, ringIdx) ;
            msleep(10);
        }
    }
    qdmaSetIrqPtime(base, 0x64);
    __st_print_counters() ;
    if(__st_check_counters() == 0) {
        QDMA_LOG("tk1: Test Result:Pass\n\n") ;
    } else {
        QDMA_LOG("tk1: Test Result:Failed\n\n") ;
    }
    return 0;
}

static int tc_tk2(uint base, uint packets, uint lens, int ringIdx)
{
	uint channel = random32() % CONFIG_QDMA_CHANNEL ;
	uint queue = random32() % CONFIG_QDMA_QUEUE ;
	uint entryLen = 0, headIdx = 0, headIdxBase = 0;
	uint thrshldLen = 0, depthLen = 0;
	uint thrshldLenPrev = 0, ptimePrev = 0, depthLenPrev = 0;
	uint irqQueueAddr = 0, irqDepth = 0;
	uint npacket = 0;
	uint result = 0 ;
	dma_addr_t irqDmaAddrPrev = 0 ;
	
	irqDmaAddrPrev = qdmaGetIrqBase(base);
	depthLenPrev = qdmaGetIrqDepth(base);
	ptimePrev = qdmaGetIrqPtime(base);
	thrshldLenPrev = qdmaGetIrqThreshold(base);
	irqQueueAddr = 0;
	delayTime = 10;
	
	/* enable IRQ_EN */
	qdmaEnableIrqEn(base);
	depthLen = depthLenPrev;
	
	for(thrshldLen=packets; thrshldLen<=32; thrshldLen=thrshldLen<<1) {
		if(depthLen < thrshldLen)
			continue;
		QDMA_ERR("\r\ndepthLen is %d, thrshldLen is %d.\n", depthLen, thrshldLen) ;
		
		qdmaSetIrqThreshold(base, (thrshldLen));
		qdmaSetIrqPtime(base, 0);
		QDMA_ERR("IRQ_EN_1: Base:0x%08x, Depth:%d, Threshold:%d, EntryLen:%d, HeadIdx:%d, Ptime:%d\n", 
			qdmaGetIrqBase(base), qdmaGetIrqDepth(base), qdmaGetIrqThreshold(base), 
			qdmaGetIrqEntryLen(base), qdmaGetIrqHeadIdx(base), qdmaGetIrqPtime(base)) ;
		
		/*  send thrshldLen*25 packet, expect intTxDone:25 */
		npacket = thrshldLen*25;
		gpQdmaPriv->counters.intTxDone[ringIdx]=0;
		gpQdmaPriv->counters.txCounts[ringIdx]=0;
		qdma_st_packet_gen(channel, queue, npacket, lens, 0x01, ringIdx) ;
		msleep(100);
		if(gpQdmaPriv->counters.intTxDone[ringIdx] == npacket*2/thrshldLen)
		{
			QDMA_ERR("IRQ_EN_1 test pass: npacket:%d, intTxDone:%d, txCounts:%d\r\n",
			npacket, gpQdmaPriv->counters.intTxDone[ringIdx], gpQdmaPriv->counters.txCounts[ringIdx]);
		} else {
			QDMA_ERR("IRQ_EN_1 test fail: npacket:%d, intTxDone:%d, txCounts:%d\r\n",
			npacket, gpQdmaPriv->counters.intTxDone[ringIdx], gpQdmaPriv->counters.txCounts[ringIdx]);
			result = -1;
			goto tk2_finish_1;
		}
		
		if(thrshldLen==1)
			continue;
		
		/*  send thrshldLen-1 packet, expect intTxDone:0, entryLen:thrshldLen-1 */
		gpQdmaPriv->counters.intTxDone[ringIdx]=0;		
		qdma_st_packet_gen(channel, queue, thrshldLen-1, lens, 0x01, ringIdx) ;
		msleep(3000);
		entryLen = qdmaGetIrqEntryLen(base);
		headIdxBase = qdmaGetIrqHeadIdx(base);
		if((gpQdmaPriv->counters.intTxDone[ringIdx] == 0) && (entryLen == thrshldLen-1))
			QDMA_ERR("IRQ_EN_2 test pass.\r\n");
		else {
			QDMA_ERR("IRQ_EN_2 test fail: npacket:%d, gpQdmaPriv->counters.intTxDone:%d, entryLen:%d\r\n", 
                npacket, gpQdmaPriv->counters.intTxDone[ringIdx], entryLen);
			result = -1;
			goto tk2_finish_1;
		}
		
		/*  clear (thrshldLen+1)/2 Irq, expect entryLen:(thrshldLen-1)-(thrshldLen-1)/2, headIdx: increace (thrshldLen+1)/2 */
		qdmaSetIrqClearLen(base, (thrshldLen+1)/2);
		entryLen = qdmaGetIrqEntryLen(base);
		headIdx = qdmaGetIrqHeadIdx(base);
		if((entryLen == (thrshldLen-1)-(thrshldLen+1)/2) && (headIdx == (headIdxBase+(thrshldLen+1)/2)%512))
			QDMA_ERR("IRQ_EN_3 test pass.\r\n");
		else {
			QDMA_ERR("IRQ_EN_3 test fail: entryLen:%d, headIdxBase:%d, headIdx:%d\r\n", entryLen, headIdxBase, headIdx);
			result = -1;
			goto tk2_finish_1;
		}
		
		/*  clear (thrshldLen+1)/2 Irq, expect entryLen:0 headIdx: increace  thrshldLen-1 */
		qdmaSetIrqClearLen(base, (thrshldLen+1)/2);
		entryLen = qdmaGetIrqEntryLen(base);
		headIdx = qdmaGetIrqHeadIdx(base);
		if((entryLen == 0) && (headIdx == (headIdxBase+thrshldLen-1)%512))
			QDMA_ERR("IRQ_EN_4 test pass.\r\n");
		else {
			QDMA_ERR("IRQ_EN_4 test fail: entryLen:%d, headIdxBase:%d, headIdx:%d\r\n", 
                entryLen, headIdxBase, headIdx);
			result = -1;
			goto tk2_finish_1;
		}
		
		/*  set Ptime as 1, send thrshldLen-1 packet, expect intTxDone:1 */
		qdmaSetIrqPtime(base, 1);
		gpQdmaPriv->counters.intTxDone[ringIdx]=0;		
		qdma_st_packet_gen(channel, queue, thrshldLen-1, lens, 0x01, ringIdx) ;
		msleep(3000);
		if(gpQdmaPriv->counters.intTxDone[ringIdx] == (thrshldLen-1)*2)
			QDMA_ERR("IRQ_EN_5 test pass: gpQdmaPriv->counters.intTxDone:%d\r\n", 
			gpQdmaPriv->counters.intTxDone[ringIdx]);
		else {
			QDMA_ERR("IRQ_EN_5 test fail: gpQdmaPriv->counters.intTxDone:%d\r\n", 
                gpQdmaPriv->counters.intTxDone[ringIdx]);
			result = -1;
			goto tk2_finish_1;
		}
		
		/*  set Ptime as 0xFFFF, send thrshldLen-1 packet, expect intTxDone:1 */
		qdmaSetIrqPtime(base, 0xFFFF);
		gpQdmaPriv->counters.intTxDone[ringIdx]=0;		
		qdma_st_packet_gen(channel, queue, thrshldLen-1, lens, 0x01, ringIdx) ;
		msleep(3000);
		if(gpQdmaPriv->counters.intTxDone[ringIdx] == 2)
			QDMA_ERR("IRQ_EN_6 test pass: gpQdmaPriv->counters.intTxDone:%d\r\n", 
			gpQdmaPriv->counters.intTxDone[ringIdx]);
		else {
			QDMA_ERR("IRQ_EN_6 test fail: gpQdmaPriv->counters.intTxDone:%d\r\n", 
                gpQdmaPriv->counters.intTxDone[ringIdx]);
			result = -1;
			goto tk2_finish_1;
		}
	}

tk2_finish_1:
	if(irqQueueAddr) {
		QDMA_ERR("irqQueueAddr free\r\n");
		dma_free_coherent(NULL, 4*irqDepth, (void *)irqQueueAddr, qdmaGetIrqBase(gpQdmaPriv->csrBaseAddr)) ;
		irqQueueAddr = 0 ;
	}

	if(result == 0)
		QDMA_LOG("\r\ntk2 test pass.\r\n");
	else
		QDMA_LOG("\r\ntk2 test fail.\r\n");
	
	delayTime = 0;
	qdmaSetIrqPtime(base, ptimePrev);
	qdmaSetIrqThreshold(base, thrshldLenPrev);
    return 0;
}


/*  return 0 pass ,return 1 fail */
static int loopcnt_test(uint base, uint lens, int ringIdx, uint channel, uint queue)
{
    uint8 index,i=0;
    uint loop_test[2] = {100,255};
	uint cpuTxCnt = 0, cpuRxCnt = 0, fwdTxCnt = 0, fwdRxCnt = 0 ;
    QDMA_DBG_CNTR_T dbgCntr;

    for(index=0;index<2;index++){
    	/*  send 100 packets */
    	dbgCntr.cntrEn = DBG_CNTR_ENABLE ;
    	dbgCntr.isChnlAll = QDMA_ENABLE ;
    	dbgCntr.isQueueAll = QDMA_ENABLE ;
    	dbgCntr.isDscpRingAll = QDMA_ENABLE ;
    	dbgCntr.chnlIdx = channel ;
    	dbgCntr.queueIdx = queue ;
    	dbgCntr.dscpRingIdx = 0 ;
    	for(i=0;i<40;i+=4){
        	dbgCntr.cntrIdx = i;
        	dbgCntr.cntrSrc = DBG_CNTR_SRC_CPU_TX;
        	qdma_clear_and_enable_dbg_cntr_info(&dbgCntr);
        	dbgCntr.cntrIdx = i + 1;
        	dbgCntr.cntrSrc = DBG_CNTR_SRC_CPU_RX;
        	qdma_clear_and_enable_dbg_cntr_info(&dbgCntr);
        	dbgCntr.cntrIdx = i + 2;
        	dbgCntr.cntrSrc = DBG_CNTR_SRC_FWD_TX;
        	qdma_clear_and_enable_dbg_cntr_info(&dbgCntr);
        	dbgCntr.cntrIdx = i + 3;
        	dbgCntr.cntrSrc = DBG_CNTR_SRC_FWD_RX;
        	qdma_clear_and_enable_dbg_cntr_info(&dbgCntr);
        }
        loopCnt = loop_test[index];
    	qdmaSetIrqPtime(base, 0);
    	qdmaSetIrqThreshold(base, 100);
    	qdma_st_packet_gen(channel, queue, 1, lens, 0x01, ringIdx) ;
        msleep(200);
    	qdmaSetIrqPtime(base, 100);
    	/*  cpu tx=100, cpu rx=1, hw_fwd = 99 */
    	for(i=0;i<40;i+=4){
        	dbgCntr.cntrIdx = i ;
        	qdma_get_dbg_cntr_info(&dbgCntr) ;
        	cpuTxCnt = dbgCntr.cntrVal;
                
        	dbgCntr.cntrIdx = i + 1 ;
        	qdma_get_dbg_cntr_info(&dbgCntr) ;
        	cpuRxCnt = dbgCntr.cntrVal ;
                
        	dbgCntr.cntrIdx = i + 2 ;
        	qdma_get_dbg_cntr_info(&dbgCntr) ;
        	fwdTxCnt = dbgCntr.cntrVal ;
                
        	dbgCntr.cntrIdx = i + 3 ;
        	qdma_get_dbg_cntr_info(&dbgCntr) ;
        	fwdRxCnt = dbgCntr.cntrVal ;

    		if((cpuTxCnt == 1) && (cpuRxCnt == 1) && (fwdTxCnt == loopCnt) && (fwdRxCnt == (loopCnt-1)))
    			QDMA_LOG("%2d loopcnt_%d test pass.cpuTxCnt:%d, cpuRxCnt:%d, fwdTxCnt:%d, fwdRxCnt:%d\r\n",
    			    i, loopCnt, cpuTxCnt, cpuRxCnt, fwdTxCnt, fwdRxCnt);
    	    else {
    			QDMA_LOG("%2d loopcnt_%d test fail: cpuTxCnt:%d, cpuRxCnt:%d, fwdTxCnt:%d, fwdRxCnt:%d\r\n", 
                    i, loopCnt, cpuTxCnt, cpuRxCnt, fwdTxCnt, fwdRxCnt);
    		    return 1;
    		}
		}	
    }
    return 0;
}

static int dbg_cntr_config(
	unchar				cntrIdx,
	QDMA_DbgCntrSrc_t	cntrSrc,
	unchar				isChnlAll,
	unchar				isQueueAll,
	unchar				isDscpRingAll,
	unchar				chnlIdx,
	unchar				queueIdx,
	unchar				dscpRingIdx)
{
    QDMA_DBG_CNTR_T dbgCntr;
    dbgCntr.cntrIdx = cntrIdx;
    dbgCntr.cntrSrc = cntrSrc;
    dbgCntr.isChnlAll = isChnlAll ;
    dbgCntr.isQueueAll = isQueueAll ;
    dbgCntr.isDscpRingAll = isDscpRingAll ;
    dbgCntr.chnlIdx = chnlIdx ;
    dbgCntr.queueIdx = queueIdx ;
    dbgCntr.dscpRingIdx = dscpRingIdx ;
    qdma_clear_and_enable_dbg_cntr_info(&dbgCntr);

    return 0;
}

static int dbg_cntr_init(void) {
	/*  CPU TX Config */
	dbg_cntr_config(0, DBG_CNTR_SRC_CPU_TX, QDMA_ENABLE, QDMA_ENABLE, QDMA_ENABLE, -1, -1, -1); 	/*  0: CPU TX ALL */
	dbg_cntr_config(1, DBG_CNTR_SRC_CPU_TX, QDMA_ENABLE, QDMA_ENABLE, QDMA_DISABLE, -1, -1, 0); 	/*  1: CPU_TX Ring0 All */
	dbg_cntr_config(2, DBG_CNTR_SRC_CPU_TX, QDMA_ENABLE, QDMA_ENABLE, QDMA_DISABLE, -1, -1, 1); 	/*  2: CPU_TX Ring1 All */
	dbg_cntr_config(3, DBG_CNTR_SRC_CPU_TX, QDMA_DISABLE, QDMA_ENABLE, QDMA_ENABLE, 0, -1, -1);		/*  3: CPU_TX Channel_0 All */
	dbg_cntr_config(4, DBG_CNTR_SRC_CPU_TX, QDMA_DISABLE, QDMA_ENABLE, QDMA_ENABLE, 1, -1, -1);		/*  4: CPU_TX Channel_1 All */
	dbg_cntr_config(5, DBG_CNTR_SRC_CPU_TX, QDMA_DISABLE, QDMA_ENABLE, QDMA_ENABLE, 7, -1, -1);		/*  5: CPU_TX Channel_7 All */
	dbg_cntr_config(6, DBG_CNTR_SRC_CPU_TX, QDMA_ENABLE, QDMA_DISABLE, QDMA_ENABLE, -1, 0, -1);		/*  6: CPU_TX Queue_0 All */
	dbg_cntr_config(7, DBG_CNTR_SRC_CPU_TX, QDMA_ENABLE, QDMA_DISABLE, QDMA_ENABLE, -1, 1, -1);		/*  7: CPU_TX Queue_1 All */
	dbg_cntr_config(8, DBG_CNTR_SRC_CPU_TX, QDMA_ENABLE, QDMA_DISABLE, QDMA_ENABLE, -1, 7, -1);		/*  8: CPU_TX Queue_7 All */

	dbg_cntr_config(9, DBG_CNTR_SRC_CPU_TX, QDMA_DISABLE, QDMA_DISABLE, QDMA_ENABLE, 0, 0, -1); 	/*  9: CPU_TX channel_0  queue_0 */
	dbg_cntr_config(10, DBG_CNTR_SRC_CPU_TX, QDMA_DISABLE, QDMA_DISABLE, QDMA_ENABLE, 0, 7, -1);	/*  10: CPU_TX channel_0  queue_7 */
	dbg_cntr_config(11, DBG_CNTR_SRC_CPU_TX, QDMA_DISABLE, QDMA_DISABLE, QDMA_ENABLE, 1, 0, -1);	/*  11: CPU_TX channel_1  queue_0 */
	dbg_cntr_config(12, DBG_CNTR_SRC_CPU_TX, QDMA_DISABLE, QDMA_DISABLE, QDMA_ENABLE, 1, 7, -1);	/*  12: CPU_TX channel_1  queue_7 */
	dbg_cntr_config(13, DBG_CNTR_SRC_CPU_TX, QDMA_DISABLE, QDMA_DISABLE, QDMA_ENABLE, 7, 0, -1);	/*  13: CPU_TX channel_7  queue_0 */
	dbg_cntr_config(14, DBG_CNTR_SRC_CPU_TX, QDMA_DISABLE, QDMA_DISABLE, QDMA_ENABLE, 7, 1, -1);	/*  14: CPU_TX channel_7  queue_1 */
	dbg_cntr_config(15, DBG_CNTR_SRC_CPU_TX, QDMA_DISABLE, QDMA_DISABLE, QDMA_ENABLE, 7, 7, -1);	/*  15: CPU_TX channel_7  queue_7 */
	
	/*  FWD TX Config */
	dbg_cntr_config(16, DBG_CNTR_SRC_FWD_TX, QDMA_ENABLE, QDMA_ENABLE, QDMA_ENABLE, -1, -1, -1);	/*  16: FWD TX ALL */
	dbg_cntr_config(17, DBG_CNTR_SRC_FWD_TX, QDMA_ENABLE, QDMA_ENABLE, QDMA_DISABLE, -1, -1, 0);	/*  17: FWD_TX Ring0 All */
	dbg_cntr_config(18, DBG_CNTR_SRC_FWD_TX, QDMA_ENABLE, QDMA_ENABLE, QDMA_DISABLE, -1, -1, 1);	/*  18: FWD_TX Ring1 All */
	dbg_cntr_config(19, DBG_CNTR_SRC_FWD_TX, QDMA_DISABLE, QDMA_ENABLE, QDMA_ENABLE, 0, -1, -1); 	/*  19: FWD_TX Channel_0 All */
	dbg_cntr_config(20, DBG_CNTR_SRC_FWD_TX, QDMA_DISABLE, QDMA_ENABLE, QDMA_ENABLE, 1, -1, -1); 	/*  20: FWD_TX Channel_1 All */
	dbg_cntr_config(21, DBG_CNTR_SRC_FWD_TX, QDMA_DISABLE, QDMA_ENABLE, QDMA_ENABLE, 7, -1, -1); 	/*  21: FWD_TX Channel_7 All */
	dbg_cntr_config(22, DBG_CNTR_SRC_FWD_TX, QDMA_ENABLE, QDMA_DISABLE, QDMA_ENABLE, -1, 0, -1); 	/*  22: FWD_TX Queue_0 All */
	dbg_cntr_config(23, DBG_CNTR_SRC_FWD_TX, QDMA_ENABLE, QDMA_DISABLE, QDMA_ENABLE, -1, 1, -1); 	/*  23: FWD_TX Queue_1 All */
	dbg_cntr_config(24, DBG_CNTR_SRC_FWD_TX, QDMA_ENABLE, QDMA_DISABLE, QDMA_ENABLE, -1, 7, -1); 	/*  24: FWD_TX Queue_7 All */
	
	dbg_cntr_config(25, DBG_CNTR_SRC_FWD_TX, QDMA_DISABLE, QDMA_DISABLE, QDMA_ENABLE, 0, 0, -1);	/*  25: FWD_TX channel_0  queue_0 */
	dbg_cntr_config(26, DBG_CNTR_SRC_FWD_TX, QDMA_DISABLE, QDMA_DISABLE, QDMA_ENABLE, 0, 7, -1);	/*  26: FWD_TX channel_0  queue_7 */
	dbg_cntr_config(27, DBG_CNTR_SRC_FWD_TX, QDMA_DISABLE, QDMA_DISABLE, QDMA_ENABLE, 1, 0, -1);	/*  27: FWD_TX channel_1  queue_0 */
	dbg_cntr_config(28, DBG_CNTR_SRC_FWD_TX, QDMA_DISABLE, QDMA_DISABLE, QDMA_ENABLE, 1, 7, -1);	/*  28: FWD_TX channel_1  queue_7 */
	dbg_cntr_config(29, DBG_CNTR_SRC_FWD_TX, QDMA_DISABLE, QDMA_DISABLE, QDMA_ENABLE, 7, 0, -1);	/*  29: FWD_TX channel_7  queue_0 */
	dbg_cntr_config(30, DBG_CNTR_SRC_FWD_TX, QDMA_DISABLE, QDMA_DISABLE, QDMA_ENABLE, 7, 1, -1);	/*  30: FWD_TX channel_7  queue_1 */
	dbg_cntr_config(31, DBG_CNTR_SRC_FWD_TX, QDMA_DISABLE, QDMA_DISABLE, QDMA_ENABLE, 7, 7, -1);	/*  31: FWD_TX channel_7  queue_7 */
	
	/*  CPU & FWD RX Config */
	dbg_cntr_config(32, DBG_CNTR_SRC_CPU_RX, QDMA_ENABLE, QDMA_ENABLE, QDMA_ENABLE, -1, -1, -1);	/*  32: CPU RX ALL */
	dbg_cntr_config(33, DBG_CNTR_SRC_FWD_RX, QDMA_ENABLE, QDMA_ENABLE, QDMA_ENABLE, -1, -1, -1);	/*  33: FWD RX ALL */
	dbg_cntr_config(34, DBG_CNTR_SRC_CPU_RX, QDMA_ENABLE, QDMA_ENABLE, QDMA_DISABLE, -1, -1, 0);	/*  34: CPU_RX Ring0 All */
	dbg_cntr_config(35, DBG_CNTR_SRC_CPU_RX, QDMA_ENABLE, QDMA_ENABLE, QDMA_DISABLE, -1, -1, 1);	/*  35: CPU_RX Ring1 All */
	dbg_cntr_config(36, DBG_CNTR_SRC_CPU_RX, QDMA_DISABLE, QDMA_DISABLE, QDMA_ENABLE, 1, 7, -1);	/*  36: CPU_RX channel_1  queue_7 */
	dbg_cntr_config(37, DBG_CNTR_SRC_CPU_RX, QDMA_DISABLE, QDMA_DISABLE, QDMA_ENABLE, 1, 0, -1);	/*  37: CPU_RX channel_1  queue_0 */
	dbg_cntr_config(38, DBG_CNTR_SRC_FWD_RX, QDMA_DISABLE, QDMA_DISABLE, QDMA_ENABLE, 1, 7, -1);	/*  38: FWD_RX channel_1  queue_7 */
	dbg_cntr_config(39, DBG_CNTR_SRC_FWD_RX, QDMA_DISABLE, QDMA_DISABLE, QDMA_ENABLE, 1, 0, -1);	/*  39: FWD_RX channel_1  queue_0 */

	return 0;
}

/*  return 0 pass ,return 1 fail */
static int dbg_cntr_test(uint base, uint packets, uint lens)
{
    uint8 index=0, i=0;
	uint8 txRingIdx=0, rxRingIdx=0 ;
	uint channel=0, queue=0 ;
	uint npackets=0, tx_cpu_total_cnt=0, tx_cpu_channel_cnt=0, tx_cpu_queue_cnt=0, tx_cpu_ring0_cnt=0, tx_cpu_ring1_cnt=0 ;
	uint tx_fwd_total_cnt=0, tx_fwd_channel_cnt=0, tx_fwd_queue_cnt=0, tx_fwd_stream_cnt=0 ;
	uint rx_cpu_total_cnt=0, rx_cpu_ring0_cnt=0, rx_cpu_ring1_cnt=0, rx_fwd_total_cnt=0, rx_fwd_stream_cnt=0 ;
	uint dbgCntrVal[CONFIG_MAX_CNTR_NUM], expectedVal[CONFIG_MAX_CNTR_NUM];
    uint loop_test[DBG_CNTR_LOOP_NUM] = {1, 2, 100};

	for(index=0; index<DBG_CNTR_LOOP_NUM; index++) {
		loopCnt = loop_test[index];
		for(txRingIdx=0; txRingIdx<TX_RING_NUM; txRingIdx++) {
			for(rxRingIdx=0; rxRingIdx<RX_RING_NUM; rxRingIdx++) {
				if(rxRingIdx == 0) {
					qdmaSetLpbkToRX0(base);		/*  force to rx Ring0 */
				} else {
					qdmaSetLpbkToRX1(base);		/*  force to rx Ring1 */
				}
				
				npackets = packets;
				tx_cpu_total_cnt = npackets * CONFIG_QDMA_CHANNEL * CONFIG_QDMA_QUEUE;
				tx_cpu_channel_cnt = npackets * CONFIG_QDMA_QUEUE;
				tx_cpu_queue_cnt = npackets * CONFIG_QDMA_CHANNEL;
				tx_cpu_ring0_cnt = (1-txRingIdx) * tx_cpu_total_cnt;
				tx_cpu_ring1_cnt = txRingIdx * tx_cpu_total_cnt;
				
				tx_fwd_total_cnt = loopCnt * tx_cpu_total_cnt;
				tx_fwd_channel_cnt = loopCnt * tx_cpu_channel_cnt;
				tx_fwd_queue_cnt = loopCnt * tx_cpu_queue_cnt;
				tx_fwd_stream_cnt = loopCnt * npackets;
				
				rx_cpu_total_cnt = tx_cpu_total_cnt;
				rx_cpu_ring0_cnt = (1-rxRingIdx) * rx_cpu_total_cnt;
				rx_cpu_ring1_cnt = rxRingIdx * rx_cpu_total_cnt;
				rx_fwd_total_cnt = (loopCnt-1) * tx_cpu_total_cnt;
				rx_fwd_stream_cnt = (loopCnt-1) * npackets;
				
				dbg_cntr_init();
				printk("\r\nsending Packets.");
				for(channel=0; channel<CONFIG_QDMA_CHANNEL; channel++){
					for(queue=0; queue<CONFIG_QDMA_QUEUE; queue++){
						qdma_st_packet_gen(channel, queue, npackets, lens, 0x01, txRingIdx) ;
						printk(".");
        				msleep(500);
			       	}
			    }
				
				
				for(i=0; i<CONFIG_MAX_CNTR_NUM; i++) {
					dbgCntrVal[i] = qdmaGetCntrCounter(base, i);
				}

				#if 0
				if( (dbgCntrVal[0] == tx_cpu_total_cnt) && (dbgCntrVal[1] == tx_cpu_ring0_cnt) && (dbgCntrVal[2] == tx_cpu_ring1_cnt) && (dbgCntrVal[3] == tx_cpu_channel_cnt)
				 && (dbgCntrVal[4] == tx_cpu_channel_cnt) && (dbgCntrVal[5] == tx_cpu_channel_cnt) && (dbgCntrVal[6] == tx_cpu_queue_cnt) && (dbgCntrVal[7] == tx_cpu_queue_cnt)
				 && (dbgCntrVal[8] == tx_cpu_queue_cnt) && (dbgCntrVal[9] == npackets) && (dbgCntrVal[10] == npackets) && (dbgCntrVal[11] == npackets)
				 && (dbgCntrVal[12] == npackets) && (dbgCntrVal[13] == npackets) && (dbgCntrVal[14] == npackets) && (dbgCntrVal[15] == npackets)
				 && (dbgCntrVal[16] == tx_fwd_total_cnt) && (dbgCntrVal[17] == tx_fwd_total_cnt) && (dbgCntrVal[18] == tx_fwd_total_cnt) && (dbgCntrVal[19] == tx_fwd_channel_cnt)
				 && (dbgCntrVal[20] == tx_fwd_channel_cnt) && (dbgCntrVal[21] == tx_fwd_channel_cnt) && (dbgCntrVal[22] == tx_fwd_queue_cnt) && (dbgCntrVal[23] == tx_fwd_queue_cnt)
				 && (dbgCntrVal[24] == tx_fwd_queue_cnt) && (dbgCntrVal[25] == tx_fwd_stream_cnt) && (dbgCntrVal[26] == tx_fwd_stream_cnt) && (dbgCntrVal[27] == tx_fwd_stream_cnt)
				 && (dbgCntrVal[28] == tx_fwd_stream_cnt) && (dbgCntrVal[29] == tx_fwd_stream_cnt) && (dbgCntrVal[30] == tx_fwd_stream_cnt) && (dbgCntrVal[31] == tx_fwd_stream_cnt)
				 && (dbgCntrVal[32] == rx_cpu_total_cnt) && (dbgCntrVal[33] == rx_fwd_total_cnt) && (dbgCntrVal[34] == rx_cpu_ring0_cnt) && (dbgCntrVal[35] == rx_cpu_ring1_cnt)
				 && (dbgCntrVal[36] == npackets) && (dbgCntrVal[37] == npackets) && (dbgCntrVal[38] == rx_fwd_stream_cnt) && (dbgCntrVal[39] == rx_fwd_stream_cnt) )
				 #endif
				(expectedVal[0] = tx_cpu_total_cnt); (expectedVal[1] = tx_cpu_ring0_cnt); (expectedVal[2] = tx_cpu_ring1_cnt); (expectedVal[3] = tx_cpu_channel_cnt);
				(expectedVal[4] = tx_cpu_channel_cnt); (expectedVal[5] = tx_cpu_channel_cnt); (expectedVal[6] = tx_cpu_queue_cnt); (expectedVal[7] = tx_cpu_queue_cnt);
				(expectedVal[8] = tx_cpu_queue_cnt); (expectedVal[9] = npackets); (expectedVal[10] = npackets); (expectedVal[11] = npackets);
				(expectedVal[12] = npackets); (expectedVal[13] = npackets); (expectedVal[14] = npackets); (expectedVal[15] = npackets);
				(expectedVal[16] = tx_fwd_total_cnt); (expectedVal[17] = tx_fwd_total_cnt); (expectedVal[18] = tx_fwd_total_cnt); (expectedVal[19] = tx_fwd_channel_cnt);
				(expectedVal[20] = tx_fwd_channel_cnt); (expectedVal[21] = tx_fwd_channel_cnt); (expectedVal[22] = tx_fwd_queue_cnt); (expectedVal[23] = tx_fwd_queue_cnt);
				(expectedVal[24] = tx_fwd_queue_cnt); (expectedVal[25] = tx_fwd_stream_cnt); (expectedVal[26] = tx_fwd_stream_cnt); (expectedVal[27] = tx_fwd_stream_cnt);
				(expectedVal[28] = tx_fwd_stream_cnt); (expectedVal[29] = tx_fwd_stream_cnt); (expectedVal[30] = tx_fwd_stream_cnt); (expectedVal[31] = tx_fwd_stream_cnt);
				(expectedVal[32] = rx_cpu_total_cnt); (expectedVal[33] = rx_fwd_total_cnt); (expectedVal[34] = rx_cpu_ring0_cnt); (expectedVal[35] = rx_cpu_ring1_cnt);
				(expectedVal[36] = npackets); (expectedVal[37] = npackets); (expectedVal[38] = rx_fwd_stream_cnt); (expectedVal[39] = rx_fwd_stream_cnt);

				for(i=0; i<CONFIG_MAX_CNTR_NUM; i++){
				 	if(dbgCntrVal[i] != expectedVal[i]) {
						printk("\r\nloopcnt:%d, txRingIdx:%d, rxRingIdx:%d, test Fail!\r\n", loopCnt, txRingIdx, rxRingIdx);
						for(i=0; (i+3)<CONFIG_MAX_CNTR_NUM; i+=4)
							printk("dbgCntrVal[%d]:%d, dbgCntrVal[%d]:%d, dbgCntrVal[%d]:%d, dbgCntrVal[%d]:%d\r\n", i, dbgCntrVal[i], i+1, dbgCntrVal[i+1], i+2, dbgCntrVal[i+2], i+3, dbgCntrVal[i+3]);
						printk("########## expected counter as below ##########\n");
						for(i=0; (i+3)<CONFIG_MAX_CNTR_NUM; i+=4)
							printk("expectedVal[%d]:%d, expectedVal[%d]:%d, expectedVal[%d]:%d, expectedVal[%d]:%d\r\n", i, expectedVal[i], i+1, expectedVal[i+1], i+2, expectedVal[i+2], i+3, expectedVal[i+3]);
						return 1;
					}
				}
				printk("\r\nloopcnt:%d, txRingIdx:%d, rxRingIdx:%d, test Pass!\r\n", loopCnt, txRingIdx, rxRingIdx);
			}
		}
	}

	return 0;
}

static int tc_tk3(uint base, uint packets, uint lens, int ringIdx)
{
	uint channel = 0;
	uint queue = 0;	
	uint result1=0, result2=0;
    
	/*  enable LOOPCNT_EN */
	qdmaEnableLoopCnt(base);
    printk("len %d \n",lens);

    for(channel = 0;channel<CONFIG_QDMA_CHANNEL;channel++){
        for(queue = 0;queue<CONFIG_QDMA_QUEUE;queue++){
            printk("loopcnt test channel %d,queue %d \n",channel,queue);
            result1 = loopcnt_test(base, lens, ringIdx, channel, queue);
            if(result1){
                printk("loopcnt test failed!\r\n");
                goto tk3_finish;
            }
        }
    }

	printk("\n\ndbg counter test begin\n");
	result2 = dbg_cntr_test(base, packets, lens);
	if(result2){
		printk("dbg cntr test failed!\r\n");
	}
	
tk3_finish:
	if((result1 == 0) && (result2 == 0))
		QDMA_LOG("tk3 test pass.\r\n");
	else
		QDMA_LOG("tk3 test fail.\r\n");
		
	/*  disable LOOPCNT_EN */
	qdmaDisableLoopCnt(base);
    return 0;
}

static int tc_tk4(uint base, uint packets, uint lens, int ringIdx)
{
    uint channel = random32() % CONFIG_QDMA_CHANNEL ;
    uint queue = random32() % CONFIG_QDMA_QUEUE ;
    uint round=0 ;
	
    memset(counters, 0, sizeof(QDMA_DbgCounters_T)*CONFIG_QDMA_CHANNEL*CONFIG_QDMA_QUEUE) ;
    gpQdmaPriv->counters.txCounts[RING_IDX_0]=0;
    gpQdmaPriv->counters.txCounts[RING_IDX_1]=0;
    gpQdmaPriv->counters.rxCounts[RING_IDX_0]=0;
    gpQdmaPriv->counters.rxCounts[RING_IDX_1]=0;
    rxCounter[RING_IDX_0]=0;
    rxCounter[RING_IDX_1]=0;
    for(round=0;round<10;round++){
        packets = random32() % 1000;
        qdma_st_packet_gen(channel, queue, packets, lens, 0x01, RING_IDX_0) ;
        msleep(100);

        packets = random32() % 1000;
        qdma_st_packet_gen(channel, queue, packets, lens, 0x01, RING_IDX_1) ;
        msleep(100);
    }
    __st_print_counters() ;
    printk("TX0 %d \t RX0 %d \t TX1 %d \t RX1 %d\n",
    gpQdmaPriv->counters.txCounts[RING_IDX_0],rxCounter[RING_IDX_0],
    gpQdmaPriv->counters.txCounts[RING_IDX_1],rxCounter[RING_IDX_1]);

    if((gpQdmaPriv->counters.txCounts[RING_IDX_0] == rxCounter[RING_IDX_0]) &&
    (gpQdmaPriv->counters.txCounts[RING_IDX_1] == rxCounter[RING_IDX_1])){
        QDMA_LOG("tk4 test pass.\r\n");
    }else{
        QDMA_LOG("tk4 test fail.\r\n");
    }
    return 0;
}

static int tc_tk5(uint base, uint packets, uint lens, int ringIdx)
{
    uint round = 0;
    uint channel = random32() % CONFIG_QDMA_CHANNEL ;
    uint queue = random32() % CONFIG_QDMA_QUEUE ;
    uint oldHxqDyn = qdmaGetTxqDynTotalMinThrh(base);
    uint oldInt = qdmaGetIntMask(base);
	struct ECNT_QDMA_Data qdma_data ;
	
    /*  clear count */
    memset(counters, 0, sizeof(QDMA_DbgCounters_T)*CONFIG_QDMA_CHANNEL*CONFIG_QDMA_QUEUE) ;
    gpQdmaPriv->counters.txCounts[RING_IDX_0]=0;
    gpQdmaPriv->counters.txCounts[RING_IDX_1]=0;
    rxCounter[RING_IDX_0]=0;
    rxCounter[RING_IDX_1]=0;
	
    qdmaSetIntMask(base,INT_STATUS_TX0_DONE);
    qdmaSetTxqDynTotalMinThrh(base, 10);  /*  small enough to block tx low chain */
    /*  fill until rx chain full(512 dscps), linkmgr(1000 dscps) almost full */
    qdma_st_packet_gen(channel, queue, 1000, lens, 0x01, RING_IDX_1);

    printk("\n::TX0 %d, TX1 %d, RX0 %d, RX1 %d\n",gpQdmaPriv->counters.txCounts[RING_IDX_0],
        gpQdmaPriv->counters.txCounts[RING_IDX_1],rxCounter[RING_IDX_0],rxCounter[RING_IDX_1]);
    msleep(1000);

    /*  send 2 packet, recv 1 packet */
    printk("Sending %d packets\n",PACKET_NUM);
    for(round=0;round<PACKET_NUM;round++){
        qdma_st_packet_gen(channel, queue, 1, lens, 0x01, RING_IDX_0);
        qdma_st_packet_gen(channel, queue, 1, lens, 0x01, RING_IDX_1);
        if(round & 0x10){
            msleep(10);
        }
		qdma_data.qdma_private.cnt = 1 ;
        qdma_receive_packets(&qdma_data);
        printk(".");
    }
    /*  tx0 packet was block by linkmgr */
    printk("\nTX0 %d, TX1 %d, RX0 %d, RX1 %d\n",gpQdmaPriv->counters.txCounts[RING_IDX_0],
        gpQdmaPriv->counters.txCounts[RING_IDX_1],rxCounter[RING_IDX_0],rxCounter[RING_IDX_1]);
    msleep(1000);
    /*  should get 2000 from TX1, get 0 from TX0 */
    if((rxCounter[RING_IDX_0] == 0) && (rxCounter[RING_IDX_1] == PACKET_NUM)){
        QDMA_LOG("tk5 test pass.\r\n");
    }else{
        QDMA_LOG("tk5 test fail.\r\n");
    }
    /*  recv the rest packet */
    qdma_bm_receive_packets(0,0);
    qdmaClearIntStatus(base, 0xFFFFFFFF) ;
    qdmaSetTxqDynTotalMinThrh(base,oldHxqDyn);
    qdmaSetIntMask(base,oldInt);
    return 0;
}


static int tc_tk6(uint base, uint packets, uint lens, int ringIdx)
{
    uint channel = random32() % CONFIG_QDMA_CHANNEL ;
    uint queue = random32() % CONFIG_QDMA_QUEUE ;
    uint round=0 ;
    
    /*  clear count */
    gpQdmaPriv->counters.txCounts[RING_IDX_0]=0;
    gpQdmaPriv->counters.txCounts[RING_IDX_1]=0;
    gpQdmaPriv->counters.rxCounts[RING_IDX_0]=0;
    gpQdmaPriv->counters.rxCounts[RING_IDX_1]=0;
    qdmaSetTxBufChnnelThreshold(base, 0x10);
    qdmaSetTxBufTotalThreshold(base,0x80);

    for(round=0;round<100;round++){
        printk("Sending packet!\n");
        qdma_st_packet_gen(channel, queue, 1000, lens, 0x01, RING_IDX_0);
        if(round & 0x10){
            msleep(10);
        }
    }
    printk("Tx %d, Rx %d \n",gpQdmaPriv->counters.txCounts[RING_IDX_0],
        gpQdmaPriv->counters.rxCounts[RING_IDX_0]);

    qdmaSetTxBufChnnelThreshold(base, 0x10);
    qdmaSetTxBufTotalThreshold(base,0x80);

    for(round=0;round<100;round++){
        qdma_st_packet_gen(channel, queue, 1000, lens, 0x01, RING_IDX_0);
        printk("Sending packet!\n");
        if(round & 0x10){
            msleep(10);
        }
    }
    printk("Tx %d, Rx %d \n",gpQdmaPriv->counters.txCounts[RING_IDX_0],
        gpQdmaPriv->counters.rxCounts[RING_IDX_0]);
    return 0;
}


static int tc_tk7(uint base, uint packets, uint lens, int ringIdx)
{
	int result=0;
	unchar weight[CONFIG_QDMA_QUEUE];
	uint channelIdx=0, queueIdx=0;
	uint weightReg0=0, weightReg1=0, weightMemXs0=0, weightMemXs1=0;
	QDMA_TxQosScheduler_T TxQos;
	struct ECNT_QDMA_Data qdma_data;

	/*  Write: general register;   Read: dbg mem xs */
	/*  0x2: QDMA QoS Weight */
	for(channelIdx=0; channelIdx<CONFIG_QDMA_CHANNEL; channelIdx++){
		/*  Write: general register */
		TxQos.channel = channelIdx;
		TxQos.qosType = QDMA_TXQOS_TYPE_WRR;
		for(queueIdx=0; queueIdx<CONFIG_QDMA_QUEUE; queueIdx++)
			TxQos.queue[queueIdx].weight = random32()%100;
		qdma_data.qdma_private.qdma_tx_qos.pTxQos = &TxQos;
		qdma_set_tx_qos(&qdma_data) ;
		if(result<0)
			goto tk7_finish;
		
		/*  Read: dbg mem xs */
		result = qdmaGetDbgMemXsConfig(QDMA_DBG_MEM_XS_MEMSEL_QOS_WEIGHT_COUNTER, 0, channelIdx, &weightMemXs0);
		if(result<0)
			goto tk7_finish;
		result = qdmaGetDbgMemXsConfig(QDMA_DBG_MEM_XS_MEMSEL_QOS_WEIGHT_COUNTER, 1, channelIdx, &weightMemXs1);
		if(result<0)
			goto tk7_finish;

		/*  check read value right or not */
		for(queueIdx=0; queueIdx<CONFIG_QDMA_QUEUE; queueIdx++) {
			if(queueIdx<4)
				weight[queueIdx] = (weightMemXs0>>(queueIdx<<3)) & 0xFF;
			else
				weight[queueIdx] = (weightMemXs1>>((queueIdx-4)<<3)) & 0xFF;
			if(TxQos.queue[queueIdx].weight != weight[queueIdx]) {
				weightReg0 = ((TxQos.queue[3].weight)<<24) | ((TxQos.queue[2].weight)<<16) | ((TxQos.queue[1].weight)<<8) | (TxQos.queue[0].weight);
				weightReg1 = ((TxQos.queue[7].weight)<<24) | ((TxQos.queue[6].weight)<<16) | ((TxQos.queue[5].weight)<<8) | (TxQos.queue[4].weight);
				printk("channel:%d-MEM_XS_Read, queue_7_4_Reg:0x%08x, queue_3_0_Reg:0x%08x, queue_7_4_Mem_XS:0x%08x, queue_3_0_Mem_XS:0x%08x", channelIdx, weightReg1, weightReg0, weightMemXs1, weightMemXs0);
				goto tk7_finish;
			}
		}
	}
		

	/*  Write: dbg mem xs;   Read: general register */
	/*  0x2: QDMA QoS Weight	 */
	for(channelIdx=0; channelIdx<CONFIG_QDMA_CHANNEL; channelIdx++){
		/*  Write: general register */
		for(queueIdx=0; queueIdx<CONFIG_QDMA_QUEUE; queueIdx++) {
			weight[queueIdx] = random32()%100;
			result = qdmaSetDbgMemXsConfig(QDMA_DBG_MEM_XS_MEMSEL_QOS_WEIGHT_COUNTER, queueIdx, channelIdx, weight[queueIdx], 0);
			if(result<0)
				goto tk7_finish;
		}
		
		/*  Read: dbg mem xs */
		TxQos.channel = channelIdx;
		qdma_data.qdma_private.qdma_tx_qos.pTxQos = &TxQos;
		qdma_get_tx_qos(&qdma_data) ;
		if(result<0)
			goto tk7_finish;

		/*  check read value right or not */
		for(queueIdx=0; queueIdx<CONFIG_QDMA_QUEUE; queueIdx++) {
			if(TxQos.queue[queueIdx].weight != weight[queueIdx]) {
				weightMemXs0 = (weight[3]<<24) | (weight[2]<<16) | (weight[1]<<8) | weight[0];
				weightMemXs1 = (weight[7]<<24) | (weight[6]<<16) | (weight[5]<<8) | weight[4];
				weightReg0 = ((TxQos.queue[3].weight)<<24) | ((TxQos.queue[2].weight)<<16) | ((TxQos.queue[1].weight)<<8) | (TxQos.queue[0].weight);
				weightReg1 = ((TxQos.queue[7].weight)<<24) | ((TxQos.queue[6].weight)<<16) | ((TxQos.queue[5].weight)<<8) | (TxQos.queue[4].weight);
				printk("channel:%d-MEM_XS_Write, queue_7_4_Reg:0x%08x, queue_3_0_Reg:0x%08x, queue_7_4_Mem_XS:0x%08x, queue_3_0_Mem_XS:0x%08x", channelIdx, weightReg1, weightReg0, weightMemXs1, weightMemXs0);
				goto tk7_finish;
			}
		}
	}
		
tk7_finish:
	if(result == 0)
		QDMA_LOG("tk7 test pass.\r\n");
	else
		QDMA_LOG("tk7 test fail.\r\n");
    return 0;
}

static int tc_tc0(uint base, uint packets, uint lens, int ringIdx)
{
    uint channel = random32() % CONFIG_QDMA_CHANNEL ;
    uint queue = random32() % CONFIG_QDMA_QUEUE ;

    memset(counters, 0, sizeof(QDMA_DbgCounters_T)*CONFIG_QDMA_CHANNEL*CONFIG_QDMA_QUEUE) ;
    aveTime=0;
    qdma_st_packet_gen(channel, queue, packets, lens, 0x01, ringIdx) ;
    __st_print_counters() ;
    aveTime = aveTime / packets;
    printk("aveTime is 0x%08x\r\n\r\n", aveTime);
    if(__st_check_counters() == 0)
        QDMA_LOG("tc0 test pass.\r\n");
    else
        QDMA_LOG("tc0 test fail.\r\n");
    return 0;
}

static int tc_tc1(uint base, uint packets, uint lens, int ringIdx)
{
    uint oldInt=0 ;
    oldInt = qdmaGetIntMask(base) ;
    qdma_recycle_packet_mode(QDMA_TX_POLLING, 32) ;     
    qdma_receive_packet_mode(QDMA_RX_POLLING) ;
    printk("Case 1: Polling Mode for TX/RX, Pattern:RAND, Lens:RAND, Packets:%d\n", packets) ;
    kernel_thread((int (*)(void *))qdma_st_rx_polling, NULL, 0) ;
    memset(counters, 0, sizeof(QDMA_DbgCounters_T)*CONFIG_QDMA_CHANNEL*CONFIG_QDMA_QUEUE) ;
    qdma_st_packet_gen(CONFIG_QDMA_CHANNEL, CONFIG_QDMA_QUEUE, packets, lens, 0x01, ringIdx) ;

    /*  recycle the rest dscp because lower than txThrh */
    qdma_bm_transmit_done(0) ;
    msleep(3000) ;
    __st_print_counters() ;
    if(__st_check_counters() == 0) {
        QDMA_LOG("tc1 test pass.\r\n");
    }else{
        QDMA_LOG("tc1 test fail.\r\n");
    }
    qdmaClearIntStatus(base, 0xFFFFFFFF) ;
    qdma_recycle_packet_mode(QDMA_TX_INTERRUPT, 0) ;        
    qdma_receive_packet_mode(QDMA_RX_NAPI) ;
    qdmaSetIntMask(base, oldInt) ;
    return 0;
}

static int tc_tc2(uint base, uint packets, uint lens, int ringIdx)
{
    unchar pattern[5] = {0x00, 0xFF, 0x5A, 0xA5, 0x01} ;
    uint i=0, j=0, k=0 ;
	uint result=0 ;
    printk("Case 2: (Channel, Queue, Pattern, Lens) Any Combination, Packets:%d\n", 
        100000*CONFIG_QDMA_CHANNEL*CONFIG_QDMA_QUEUE*5) ;
    for(i=0 ; i<CONFIG_QDMA_CHANNEL ; i++) {
        for(j=0 ; j<CONFIG_QDMA_QUEUE ; j++) {
            for(k=0 ; k<5 ; k++) {
                memset(counters, 0, sizeof(QDMA_DbgCounters_T)*CONFIG_QDMA_CHANNEL*CONFIG_QDMA_QUEUE) ;
                qdma_st_packet_gen(i, j, 100000, 1, pattern[k], ringIdx) ;
                msleep(1000) ;
                if(counters[i][j].tx_frames == counters[i][j].rx_frames) {
                    printk("Channel:%d, Queue:%d, Pattern:%.2x, Lens:48~2000, Packets:%d, Test Result:Pass\n\n", 
                        i, j, pattern[k], counters[i][j].tx_frames) ;
                } else {
                    printk("Channel:%d, Queue:%d, Pattern:%.2x, Lens:48~2000, Packets:%d, Test Result:Failed\n\n", 
                        i, j, pattern[k], counters[i][j].tx_frames) ;
                    result = -1;
                }
            }
        }
    }
    if(result == 0)
        QDMA_LOG("tc2 test pass.\r\n");
    else
        QDMA_LOG("tc2 test fail.\r\n");
    return 0;
}

#if defined(TCSUPPORT_AUTOBENCH) && defined(TCSUPPORT_CPU_MT7520)
static int tc_tc2_epon(uint base, uint packets, uint lens, int ringIdx)
{
    unchar pattern[5] = {0x00, 0xFF, 0x5A, 0xA5, 0x01} ;
    uint i=0, j=0, k=0 ;
	uint result=0 ;
	int status=0 ;

    printk("Case 2_epon: (Channel, Queue, Pattern, Lens) Any Combination, Packets:%d\n", 100000*16*9*5) ;
    status = 0;
    pon_loopback_result = 0;
    for(i=0 ; i<1 ; i++) {
        for(j=0 ; j<1 ; j++) {
            for(k=0 ; k<5 ; k++) {
                memset(counters, 0, sizeof(QDMA_DbgCounters_T)*CONFIG_QDMA_CHANNEL*CONFIG_QDMA_QUEUE) ;
                qdma_st_packet_gen(i, j, packets, lens, pattern[k], ringIdx) ;
                if(packets>100)
                    msleep(2000) ;
                else
                    msleep(100);
                if((counters[i][j].tx_frames == counters[i][j].rx_frames)
                    && (counters[i][j].tx_frames != 0)){
                    printk("Channel:%d, Queue:%d, Pattern:%.2x, Lens:64~2000, Packets:%d, Test Result:Pass\n\n", 
                        i, j, pattern[k], counters[i][j].tx_frames) ;
                } else {
                    status ++;
                    printk("Channel:%d, Queue:%d, Pattern:%.2x, Lens:64~2000, TxPackets:%d,RxPacktes:%d Test Result:Failed\n\n", 
                        i, j, pattern[k], counters[i][j].tx_frames,counters[i][j].rx_frames) ;
                }
            }
        }
    }
    if(status == 0){
        QDMA_LOG("pon loopback test pass\n");
        pon_loopback_result = 1 ;
    }
    else{
        QDMA_LOG("pon loopback test fail\n");
        pon_loopback_result = 0;
    }
    return 0;
}
#endif 

static int tc_tc3(uint base, uint packets, uint lens, int ringIdx)
{
    unchar pattern[5] = {0x00, 0xFF, 0x5A, 0xA5, 0x01} ;
    uint i = 0 ;

    memset(counters, 0, sizeof(QDMA_DbgCounters_T)*CONFIG_QDMA_CHANNEL*CONFIG_QDMA_QUEUE) ;
    printk("Case 3: RX_2B_Offset:%s, Channel:RAND, Queue:RAND, Lens:Sequential, Pattern:0x00, 0xFF, 0x5A, 0xA5, RAND, Packets:%d\n", 
        (qdmaIsSetRx2bOffset(base))?"Enable":"Disable", packets) ;
    for(i=0 ; i<5 ; i++) {
        qdma_st_packet_gen(CONFIG_QDMA_CHANNEL, CONFIG_QDMA_QUEUE, (packets/5), 1, pattern[i], ringIdx) ;
        msleep(1000) ;
        if(__st_check_counters() == 0) {
            printk("Pattern:%.2x, Packets:%d, Test Result:Pass\n\n", pattern[i], packets/5) ;
        } else {
            printk("Pattern:%.2x, Packets:%d, Test Result:Failed\n\n", pattern[i], packets/5) ;
        }
    }
    __st_print_counters() ;
    if(__st_check_counters() == 0)
        QDMA_LOG("tc3 test pass.\r\n");
    else
        QDMA_LOG("tc3 test fail.\r\n");
    return 0;
}


static int tc_tc4(uint base, uint packets, uint lens, int ringIdx)
{
    unchar bs[4] = {VAL_BST_4_DWORD, VAL_BST_8_DWORD, VAL_BST_16_DWARD, VAL_BST_32_DWARD} ;
    uint i = 0 ;

    memset(counters, 0, sizeof(QDMA_DbgCounters_T)*CONFIG_QDMA_CHANNEL*CONFIG_QDMA_QUEUE) ;
    printk("Case 4: Pattern:RAND, Lens:RAND, Burst Size:4, 8, 16, 32, Packets:%d\n", packets) ;
    for(i=0 ; i<4 ; i++) {
        qdmaSetBurstSize(base, bs[i])   ;
        msleep(1000) ;
        
        qdma_st_packet_gen(CONFIG_QDMA_CHANNEL, CONFIG_QDMA_QUEUE, packets/4, 0, 0x01, ringIdx) ;
        msleep(1000) ;
        if(__st_check_counters() == 0) {
            printk("Burst Size:%d, Packets:%d, Test Result:Pass\n\n", qdmaGetBurstSize(base), packets/4) ;
        } else {
            printk("Burst Size:%d, Packets:%d, Test Result:Failed\n\n", qdmaGetBurstSize(base), packets/4) ;
        }
    }
    __st_print_counters() ;
    if(__st_check_counters() == 0)
        QDMA_LOG("tc4 test pass.\r\n");
    else
        QDMA_LOG("tc4 test fail.\r\n");
    return 0;
}

static int tc_tc5(uint base, uint packets, uint lens, int ringIdx)
{
    uint grade[5] = {100, 200, 400, 1000, 0} ;
    uint i = 0 ;

    memset(counters, 0, sizeof(QDMA_DbgCounters_T)*CONFIG_QDMA_CHANNEL*CONFIG_QDMA_QUEUE) ;
    for(i=0 ; i<5 ; i++) {
    	if(grade[i] >= gpQdmaPriv->txDscpNum[ringIdx])
    		continue ;
    		 
    	gpQdmaPriv->devCfg.waitTxMaxNums = (grade[i]>=gpQdmaPriv->txDscpNum[ringIdx]) ? (gpQdmaPriv->txDscpNum[ringIdx]-1) : (grade[i]) ;
    	gpQdmaPriv->devCfg.countDown = (gpQdmaPriv->devCfg.waitTxMaxNums) ? (gpQdmaPriv->devCfg.waitTxMaxNums) : ((random32()%(gpQdmaPriv->txDscpNum[ringIdx]-1))+1) ;
    	printk("Case 5-%d: TX_DSCP Nums:%d, Pattern:RAND, Lens:RAND, Packets:%d, waitTxMaxNums:%d, countDown:%d\n", i+1, grade[i], packets, gpQdmaPriv->devCfg.waitTxMaxNums, gpQdmaPriv->devCfg.countDown) ;
    	qdma_st_packet_gen(CONFIG_QDMA_CHANNEL, CONFIG_QDMA_QUEUE, packets/5, lens, 0x01, ringIdx) ;

    	/* in order to set the TX_CPU_PTR to HW register */
    	gpQdmaPriv->devCfg.countDown = 1 ;
    	gpQdmaPriv->devCfg.waitTxMaxNums = 1 ;
    	qdma_st_packet_gen(CONFIG_QDMA_CHANNEL, CONFIG_QDMA_QUEUE, 1, lens, 0x01, ringIdx) ;
    	msleep(1000) ;
    	if(__st_check_counters() == 0) {
    		printk("Grade Nums:%d, Packets:%d, Test Result:Pass\n\n", grade[i], packets/5) ;
    	} else {
    		printk("Grade Nums:%d, Packets:%d, Test Result:Failed\n\n", grade[i], packets/5) ;
    	}
    }
    __st_print_counters() ;
    if(__st_check_counters() == 0)
        QDMA_LOG("tc5 test pass.\r\n");
    else
        QDMA_LOG("tc5 test fail.\r\n");

    gpQdmaPriv->devCfg.countDown = 1 ;
    gpQdmaPriv->devCfg.waitTxMaxNums = 1 ;
    return 0;
}


static int tc_tc6(uint base, uint packets, uint lens, int ringIdx)
{
    uint grade[3] = {128, 300, 512} ;
    uint oldInt=0, depth=0, entryIndex=0 ;
    uint i=0, j=0 ;

    oldInt = qdmaGetIntMask(base) ;
    depth = qdmaGetIrqDepth(base) ;     
    qdmaSetIntMask(base, (INT_MASK_RX0_DONE|INT_MASK_RX1_DONE|INT_MASK_IRQ_FULL)) ;

    memset(counters, 0, sizeof(QDMA_DbgCounters_T)*CONFIG_QDMA_CHANNEL*CONFIG_QDMA_QUEUE) ;
    for(i=0 ; i<3 ; i++) {
        if(grade[i] >= gpQdmaPriv->txDscpNum[ringIdx])
            continue ;
        
        entryIndex = qdmaGetIrqEntryLen(base);
        if(entryIndex >= grade[i]) {
            j=0;
            while(qdmaGetIrqEntryLen(base) < grade[i]) {
                qdma_st_packet_gen(CONFIG_QDMA_CHANNEL, CONFIG_QDMA_QUEUE, 1, 0, 0x01, ringIdx) ;
                msleep(100);
                j++;
            }
            printk("entryIndex:%d, grade[%d]:%d, sent Packet:%d\n", entryIndex, i, grade[i], j);
        }
            
        qdmaSetIrqDepth(base, grade[i]) ;
        printk("Case 6-%d: IRQ Queue Depth:%d, Pattern:RAND, Lens:RAND, Packets:%d\n", i+1, grade[i], packets) ;
        qdma_st_packet_gen(CONFIG_QDMA_CHANNEL, CONFIG_QDMA_QUEUE, packets/3, 0, 0x01, ringIdx) ;
        msleep(1000) ;
        printk("IRQ_Entry_Len: %d\n", qdmaGetIrqEntryLen(base)) ;
        qdma_bm_transmit_done(0) ;
        __st_print_counters() ;
        printk("IRQ_Entry_Len: %d\n", qdmaGetIrqEntryLen(base)) ;
        if(__st_check_counters() == 0) {
            printk("Grade Nums:%d, Packets:%d, Test Result:Pass\n", grade[i], packets/3) ;
        } else {
            printk("Grade Nums:%d, Packets:%d, Test Result:Failed\n", grade[i], packets/3) ;
        }
    }
    __st_print_counters() ;
    printk("IRQ_Entry_Len: %d\n", qdmaGetIrqEntryLen(base)) ;
    if(__st_check_counters() == 0)
        QDMA_LOG("tc6 test pass.\r\n");
    else
        QDMA_LOG("tc6 test fail.\r\n");

    qdmaSetIrqDepth(base, depth) ;
    qdmaSetIntMask(base, oldInt) ;
    return 0;
}

static int tc_tc7(uint base, uint packets, uint lens, int ringIdx)
{
    uint oldInt=0, curInt=0 ;
	
    oldInt = qdmaGetIntMask(base) ;
	curInt = oldInt & (~(INT_MASK_IRQ_FULL | INT_MASK_TX1_DONE | INT_MASK_TX0_DONE));
    qdmaSetIntMask(base, curInt);
	
    memset(counters, 0, sizeof(QDMA_DbgCounters_T)*CONFIG_QDMA_CHANNEL*CONFIG_QDMA_QUEUE) ;
    qdma_st_packet_gen(CONFIG_QDMA_CHANNEL, CONFIG_QDMA_QUEUE, packets, 1, 1, ringIdx) ;
    msleep(1000) ;	
    printk("Packets:%d sent done\n\n", packets) ;
	
	curInt = oldInt | INT_MASK_IRQ_FULL;
    qdmaSetIntMask(base, curInt);
    msleep(1000) ;
    printk("INT_MASK_IRQ_FULL interrupt trigger done\n\n") ;
	
    if(__st_check_counters() == 0) {
        printk("Pattern:%.2x, Packets:%d, Test Result:Pass\n\n", 1, packets) ;
    } else {
        printk("Pattern:%.2x, Packets:%d, Test Result:Failed\n\n", 1, packets) ;
    }
    
    __st_print_counters() ;
    if(__st_check_counters() == 0)
        QDMA_LOG("tc7 test pass.\r\n");
    else
        QDMA_LOG("tc7 test fail.\r\n");
    qdmaSetIntMask(base, oldInt);
    return 0;
}


static int qdma_st_write_proc(struct file *file, const char *buffer, unsigned long count, void *data)
{
	char val_string[64], cmd[32], subcmd[32] ;
	uint value=0 ;
	uint packets=0, lens=0 ;
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint ptime=0 ;
	int ringIdx=0 ;
	struct ECNT_QDMA_Data qdma_data;
#if defined(TCSUPPORT_AUTOBENCH) && defined(TCSUPPORT_CPU_MT7520)
	int status=0 ;
#endif

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;
    
	sscanf(val_string, "%s %s %d %d %d", cmd, subcmd, &value, &msec, &ringIdx) ;
	packets = simple_strtol(subcmd, NULL, 0) ; 
	nlen = lens = value ;
	printk("msec is %d\n", msec);

	selfTestFlag=1;	
	bmRecvCallbackFunction = gpQdmaPriv->devCfg.bmRecvCallbackFunction;
	bmEventCallbackFunction = gpQdmaPriv->devCfg.bmEventCallbackFunction;
	if(strcmp(cmd, "init") != 0) {
		//qdma_register_callback_function(ECNT_QDMA, QDMA_CALLBACK_TX_FINISHED, qdma_st_cb_tx_finished) ;
		qdma_data.qdma_private.qdma_reg_cb.type = QDMA_CALLBACK_RX_PACKET;
		qdma_data.qdma_private.qdma_reg_cb.pCbFun = qdma_st_cb_rx_packet;
		qdma_register_callback_function(&qdma_data) ;
		qdma_data.qdma_private.qdma_reg_cb.type = QDMA_CALLBACK_EVENT_HANDLER;
		qdma_data.qdma_private.qdma_reg_cb.pCbFun = qdma_st_cb_EventHandler;
		qdma_register_callback_function(&qdma_data) ;
	}
	
	ptime = qdmaGetIrqPtime(base);
	if(!strcmp(cmd, "init")) {
		tc_init(subcmd, base);
	} 

    /* for other csr verification */
    else if(!strcmp(cmd, "csr")) {
        if(!strcmp(subcmd, "intmask")) {
            qdma_st_csr_intmask(value) ;    
        }
    }
    
	/* for NO_RX_CPU_DSCP_INT and NO_TX_CPU_DSCP_INT verification */
	else if(!strcmp(cmd, "csr1")) {
		tc_csr1(base, packets, lens, ringIdx);
	}
    
	/* for NO_LINK_DSCP verification */
	else if(!strcmp(cmd, "csr2")) {
		tc_csr2(base, packets, lens, ringIdx);
	}

	/* CHCCK_DONE bit enable/disable verification  */
	else if(!strcmp(cmd, "csr3")) {
        tc_csr3(base, packets, lens, ringIdx);
	} 
    
	/* for RX_DONE_INT and RX_DLY_INT verification */
	else if(!strcmp(cmd, "csr4")) {
        tc_csr4(base, packets, lens, ringIdx);
	} 
    
    /* for TX_DONE_INT and TX_DLY_INT verification */
    else if(!strcmp(cmd, "csr5")) {
        tc_csr5(base, packets, lens, ringIdx);
    }
	
	else if(!strcmp(cmd, "tk0")) {	/* QDMA Info Test */
		uint qdma_info = 0 ;
		qdma_info = IO_GREG(QDMA_CSR_INFO(base));
		
		//if(qdma_info == 0x75120096)
		if(qdma_info == 0x99)
			printk("tk0 test pass.\r\n");
		else
			printk("tk0 test fail.\r\n");
	}
	else if(!strcmp(cmd, "tk1")) {	/* 32 channel test */
	    tc_tk1(base, packets, lens, ringIdx);
	}
	else if(!strcmp(cmd, "tk2")) {	/* IRQ_EN test */
	    tc_tk2(base, packets, lens, ringIdx);
	}
	else if(!strcmp(cmd, "tk3")) {	/* loop_cnt & dbg_cntr test */
	    tc_tk3(base, packets, lens, ringIdx);
	}
    else if(!strcmp(cmd, "tk4")) {  /* CPU TX/RX chain basic test */
        tc_tk4(base, packets, lens, ringIdx);
    }
    else if(!strcmp(cmd, "tk5")) {  /* TX high and low chain */
        tc_tk5(base, packets, lens, ringIdx);
    }
    else if(!strcmp(cmd, "tk6")) {  /* TX high and low chain */
        tc_tk6(base, packets, lens, ringIdx);
    }
    else if(!strcmp(cmd, "tk7")) {  /* dbg_mem_xs test */
        tc_tk7(base, packets, lens, ringIdx);
    }
	else if(!strcmp(cmd, "tc0")) {  
        tc_tc0(base, packets, lens, ringIdx);
	} 
	else {
		if(!strcmp(cmd, "tc6") || !strcmp(cmd, "tcall")) {
            tc_tc6(base, packets, lens, ringIdx);
		}  
#ifndef CONFIG_TX_POLLING_BY_MAC
		if(!strcmp(cmd, "tc1") || !strcmp(cmd, "tcall")) {
            tc_tc1(base, packets, lens, ringIdx);
		}  
#endif /* CONFIG_TX_POLLING_BY_MAC */
		if(!strcmp(cmd, "tc2") || !strcmp(cmd, "tcall")) {
            tc_tc2(base, packets, lens, ringIdx);
		}

#if defined(TCSUPPORT_AUTOBENCH) && defined(TCSUPPORT_CPU_MT7520)
		if(!strcmp(cmd, "tc2_epon") || !strcmp(cmd, "tcall")) {
            tc_tc2_epon(base, packets, lens, ringIdx);
		}
#endif
		if(!strcmp(cmd, "tc3") || !strcmp(cmd, "tcall")) {
            tc_tc3(base, packets, lens, ringIdx);
		}
		if(!strcmp(cmd, "tc4") || !strcmp(cmd, "tcall")) {
            tc_tc4(base, packets, lens, ringIdx);
		}
		if(!strcmp(cmd, "tc5") || !strcmp(cmd, "tcall")) {
			tc_tc5(base, packets, lens, ringIdx);
		}
		if(!strcmp(cmd, "tc7") || !strcmp(cmd, "tcall")) {
			tc_tc7(base, packets, lens, ringIdx);
		}
	}
	
	if(strcmp(cmd, "init") != 0) {
		qdmaSetIrqPtime(base, 0x64);
		msleep(1000);
		qdmaSetIrqPtime(base, ptime);
	}
	selfTestFlag=0;	
	qdma_data.qdma_private.qdma_reg_cb.type = QDMA_CALLBACK_RX_PACKET;
	qdma_data.qdma_private.qdma_reg_cb.pCbFun = bmRecvCallbackFunction;
	qdma_register_callback_function(&qdma_data) ;
	qdma_data.qdma_private.qdma_reg_cb.type = QDMA_CALLBACK_EVENT_HANDLER;
	qdma_data.qdma_private.qdma_reg_cb.pCbFun = bmEventCallbackFunction;
	qdma_register_callback_function(&qdma_data) ;

	return count ;
}

#endif /* CONFIG_SUPPORT_SELF_TEST */


/******************************************************************************
******************************************************************************/
int qdma_dvt_init(void) 
{
#ifdef CONFIG_SUPPORT_SELF_TEST
	struct proc_dir_entry *qdma_proc = NULL ;
	/* create proc node */
	qdma_proc = create_proc_entry(self_test_path, 0, NULL);
	if(qdma_proc) {
		qdma_proc->read_proc = qdma_st_read_proc;
		qdma_proc->write_proc = qdma_st_write_proc;
	}
#endif /* CONFIG_SUPPORT_SELF_TEST */

	return 0 ;
}


/******************************************************************************
******************************************************************************/
int qdma_dvt_deinit(void) 
{
#ifdef CONFIG_SUPPORT_SELF_TEST
	remove_proc_entry(self_test_path, NULL);
#endif /* CONFIG_SUPPORT_SELF_TEST */

	return 0 ;
}
