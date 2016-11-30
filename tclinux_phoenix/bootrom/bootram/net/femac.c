/*
** $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/bootrom/bootram/net/femac.c#1 $
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
** $Log: femac.c,v $
** Revision 1.1.2.9  2011/08/02 06:07:38  lino
** add RT63365 support: SDRAM/DDR1/DDR2 parameter
**
** Revision 1.1.2.8  2011/07/08 02:31:26  lino
** add RT63365 support: skip gpio reset
**
** Revision 1.1.2.7  2011/06/16 05:15:02  lino
** add RT63365 support: add 6 port phy board support
**
** Revision 1.1.2.6  2011/06/16 05:07:20  lino
** add RT63365 support: rewrite mac reset function
**
** Revision 1.1.2.5  2011/05/10 02:28:27  lino
** add RT63365 frame engine support
**
** Revision 1.1.2.4  2011/05/04 08:34:41  lino
** add RT65168 support: HWID=4
**
** Revision 1.1.2.3  2011/04/20 10:53:00  lino
** add RT63365 support
**
** Revision 1.1.2.2  2011/04/20 09:57:27  lino
** add RT63365 support
**
** Revision 1.1.2.1  2011/04/20 02:46:31  lino
** add RT63365 support
**
** Revision 1.4.6.1  2010/09/28 12:53:37  lino
** add RT63165 support
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
#ifdef TCSUPPORT_MT7510_FE
#include "femac_7510.h"
#else 
#include "femac.h"
#endif

#ifdef __BIG_ENDIAN
#define FE_BYTE_SWAP
#endif

#define printf prom_printf

static macAdapter_t DummyAdapter;
macAdapter_t *mac_p = &DummyAdapter;
static macMemPool_t enetMemPool __attribute__ ((__aligned__(32)));

#define read_reg_word(reg) 			VPint(reg)
#define write_reg_word(reg, wdata) 	VPint(reg)=wdata


#define isMT751020 	(((read_reg_word(0xbfb00064)&0xffff0000))==0x00050000)
#define isMT752XG	(((read_reg_word(0xbfb000f8)&0x3)==0x3) && isMT751020)


extern void pause(int ms);

static void
delay1ms(
	int ms
)
{
	volatile uint32 timer_now, timer_last;
	volatile uint32 tick_acc;
	uint32 one_tick_unit = 1 * get_SYS_HCLK() * 1000 / 2;
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


void miiStationWrite(uint32 enetPhyAddr, uint32 phyReg, uint32 miiData)
{
	uint32 reg;
	uint32 cnt=10000;

	do {
		reg=read_reg_word (GSW_CFG_PIAC);
		cnt--;
	} while ((reg & PHY_ACS_ST) && (cnt != 0));

	reg = PHY_ACS_ST | (MDIO_ST_START << MDIO_ST_SHIFT) | (MDIO_CMD_WRITE<<MDIO_CMD_SHIFT) | 
		(enetPhyAddr << MDIO_PHY_ADDR_SHIFT) | (phyReg << MDIO_REG_ADDR_SHIFT) | 
		(miiData & MDIO_RW_DATA);
	write_reg_word (GSW_CFG_PIAC, reg);

	cnt = 10000;
	do {
		//pause(1);
		reg=read_reg_word (GSW_CFG_PIAC);
		cnt--;
	} while ((reg & PHY_ACS_ST) && (cnt != 0));
}

uint32 miiStationRead(uint32 enetPhyAddr, uint32 phyReg)
{
	uint32 reg;
	uint32 cnt=10000;

	do {
		reg=read_reg_word (GSW_CFG_PIAC);
		cnt--;
	} while ((reg & PHY_ACS_ST) && (cnt != 0));

	reg = PHY_ACS_ST | (MDIO_ST_START << MDIO_ST_SHIFT) | (MDIO_CMD_READ<<MDIO_CMD_SHIFT) | 
		(enetPhyAddr << MDIO_PHY_ADDR_SHIFT) | (phyReg << MDIO_REG_ADDR_SHIFT);
	write_reg_word (GSW_CFG_PIAC, reg);

	cnt = 10000;
	do {
		//pause(1);
		reg=read_reg_word (GSW_CFG_PIAC);
		cnt--;
	} while ((reg & PHY_ACS_ST) && (cnt != 0));
	reg = reg & MDIO_RW_DATA;

	return reg;
}

#if defined(TCSUPPORT_BOOTROM_LARGE_SIZE)
uint32 gswPbusRead(uint32 pbus_addr)
{
	uint32 pbus_data;

	uint32 phyaddr;
	uint32 reg;
	uint32 value;

	phyaddr = 31;
	// 1. write high-bit page address
	reg = 31;
	value = (pbus_addr >> 6);
  	miiStationWrite(phyaddr, reg, value);

	// 2. read low DWord
	reg = (pbus_addr>>2) & 0x000f;
	value = miiStationRead(phyaddr, reg);
	pbus_data = value;

	// 3. read high DWord
	reg = 16;
	value = miiStationRead(phyaddr, reg);

	pbus_data = (pbus_data) | (value<<16);

	return pbus_data;
} 
#endif

//7520G/7525G will use this func
int gswPbusWrite(uint32 pbus_addr, uint32 pbus_data)
{
	uint32 phyaddr;
	uint32 reg;
	uint32 value;

	phyaddr = 31;

	// 1. write high-bit page address
	reg = 31;
	value = (pbus_addr >> 6);
	miiStationWrite(phyaddr, reg, value);

	// 2. write low DWord
	reg = (pbus_addr>>2) & 0x000f;
	value = pbus_data & 0xffff;
	miiStationWrite(phyaddr, reg, value);

	// 3. write high DWord
	reg = 16;
	value = (pbus_data>>16) & 0xffff;
	miiStationWrite(phyaddr, reg, value);

  	return 0;
} 

#if defined(MT7530_SUPPORT) || defined(TCSUPPORT_BOOTROM_LARGE_SIZE)
//PHY2 read/write
uint32 gswPmiRead(uint32 phy_addr, uint32 phy_reg)
{
	uint32 pbus_addr;
	uint32 pbus_data;
	uint32 phy_data;
	uint32 phy_acs_st;

	pbus_addr = 0x701c;
	// b31	- phy access 1:start&busy, 0:complete&idle
	// b29:25 - mdio phy reg addr
	// b24:20 - mdio phy addr
	// b19:18 - 2'b01: write, 2'b10: read
	// b17:16 - start field, always 2'b01
	// b15:0	- data

	phy_addr = phy_addr & 0x1f;
	phy_reg  = phy_reg & 0x1f;

	// 1. write phy_addr & phy_reg
	pbus_data = 0x80090000; // read
	pbus_data = pbus_data | (phy_addr<<20);
	pbus_data = pbus_data | (phy_reg<<25);

	gswPbusWrite(pbus_addr,pbus_data);

	// 2. check phy_acs_st
	phy_acs_st = 1;
	while (phy_acs_st) {
		pbus_data = gswPbusRead(pbus_addr);
		phy_acs_st = (pbus_data>>31) & 0x1;
	}

	// 3. return data
	phy_data = pbus_data & 0xffff;
	return phy_data;
} 


uint32 gswPmiWrite(uint32 phy_addr, uint32 phy_reg, uint32 phy_data)
{
	uint32 pbus_addr;
	uint32 pbus_data;
//	uint32 phy_acs_st;

	pbus_addr = 0x701c;
	// b31    - phy access 1:start&busy, 0:complete&idle
	// b29:25 - mdio phy reg addr
	// b24:20 - mdio phy addr
	// b19:18 - 2'b01: write, 2'b10: read
	// b17:16 - start field, always 2'b01
	// b15:0  - data

	phy_addr = phy_addr & 0x1f;
	phy_reg  = phy_reg & 0x1f;
	phy_data = phy_data & 0xffff;

	// 1. write phy_addr & phy_reg & phy_data
	pbus_data = 0x80050000; // write
	pbus_data = pbus_data | (phy_addr<<20);
	pbus_data = pbus_data | (phy_reg<<25);
	pbus_data = pbus_data | (phy_data);

	gswPbusWrite(pbus_addr,pbus_data);

	return 0;
}
#endif

void macControllerStop(macAdapter_t *mac_p)
{
	uint32 reg;

	reg = read_reg_word(PDMA_GLO_CFG);
	reg &= ~(TX_WB_DDONE | RX_DMA_EN | TX_DMA_EN);
	write_reg_word(PDMA_GLO_CFG, reg);
}
#ifdef TCSUPPORT_CPU_MT7520
 void pause3(int ms)
{
	unsigned int i;
	for (i = 0; i < ms * 100000; i++)
		;
}
#endif
void macResetSwMAC(void)
{
	uint32 reg;

#ifdef TCSUPPORT_CPU_MT7520
	if(isMT7520G || isMT7525G)
	{
		write_reg_word(0xbfb00358, 0x55550001);
		pause3(200);
		write_reg_word(0xbfb00358, 0x55551001);
		pause3(10);
	}
	
	/* reset ethernet phy, ethernet switch, frame engine */
	reg = read_reg_word(CR_RSTCTRL2);
	reg |= (EPHY_RST | ESW_RST | FE_RST);
	write_reg_word(CR_RSTCTRL2, reg);

	if(isMT7520G || isMT7525G)
		pause3(200);
	/* de-assert reset ethernet phy, ethernet switch, frame engine */
	reg = read_reg_word(CR_RSTCTRL2);
	reg &= ~(EPHY_RST | ESW_RST | FE_RST);
	write_reg_word(CR_RSTCTRL2, reg);
	if(isMT7520G || isMT7525G)
		pause3(50);

#else
	/* reset ethernet phy, ethernet switch, frame engine */
	reg = read_reg_word(CR_RSTCTRL2);
	reg |= (EPHY_RST | ESW_RST | FE_RST);
	write_reg_word(CR_RSTCTRL2, reg);

	/* de-assert reset ethernet phy, ethernet switch, frame engine */
	reg = read_reg_word(CR_RSTCTRL2);
	reg &= ~(EPHY_RST | ESW_RST | FE_RST);
	write_reg_word(CR_RSTCTRL2, reg);
#endif
}

#if defined(TCSUPPORT_MULTI_UPGRADE) || defined(TCSUPPORT_MULTI_BOOT)
void resetSwMAC3262(void)
{
	write_reg_word(GSW_PMCR(6), read_reg_word(GSW_PMCR(6)) & ~(FORCE_LNK_PN));	

	macResetSwMAC();

#if defined(TCSUPPORT_CPU_MT7505)
	delay1ms(1);
#endif
	
	if(isMT7505){
		miiStationWrite(8,0x1f,0x4000);
		miiStationWrite(8,0x10,0x5c);
		miiStationWrite(8,0x1f,0x0);
		miiStationWrite(8,0x10,0x30);
		miiStationWrite(9,0x1f,0x4000);
		miiStationWrite(9,0x10,0x5c);
		miiStationWrite(9,0x1f,0x0);
		miiStationWrite(9,0x10,0x30);
		miiStationWrite(10,0x1f,0x4000);
		miiStationWrite(10,0x10,0x5c);
		miiStationWrite(10,0x1f,0x0);
		miiStationWrite(10,0x10,0x30);
		miiStationWrite(11,0x1f,0x4000);
		miiStationWrite(11,0x10,0x5c);
		miiStationWrite(11,0x1f,0x0);
		miiStationWrite(11,0x10,0x30);
		miiStationWrite(12,0x1f,0x4000);
		miiStationWrite(12,0x10,0x5c);
		miiStationWrite(12,0x1f,0x0);
		miiStationWrite(12,0x10,0x30);
	}
}
#endif

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
    	pTxDescp->txd_info1.word = 0;
    	pTxDescp->txd_info2.word = 0;
    	pTxDescp->txd_info3.word = 0;
    	pTxDescp->txd_info4.word = 0;

    	pTxDescp->txd_info2.bits.LS0_bit = 1;
    	pTxDescp->txd_info2.bits.DDONE_bit = 1;

		/* CPU */
    	//pTxDescp->txd_info4.bits.PN = 0;
		/* GDMA1 */
    	pTxDescp->txd_info4.bits.PN = DPORT_GDMA1;
#ifndef TCSUPPORT_MT7510_FE	
    	pTxDescp->txd_info4.bits.QN = 0;
#endif

#ifndef FE_BYTE_SWAP
		pTxDescp->txd_info1.word = cpu_to_le32(pTxDescp->txd_info1.word);
		pTxDescp->txd_info2.word = cpu_to_le32(pTxDescp->txd_info2.word);
		pTxDescp->txd_info3.word = cpu_to_le32(pTxDescp->txd_info3.word);
		pTxDescp->txd_info4.word = cpu_to_le32(pTxDescp->txd_info4.word);
#endif

		mac_p->txskbs[i] = NULL;
	}

	/* init. Rx descriptor, allocate memory for each descriptor */
	pRxDescp = (macRxDescr_t*) mac_p->rxDescrRingBaseVAddr;

	for (i = 0 ; i< mac_p->rxRingSize ; i++, pRxDescp++) {
		// Init Descriptor
		pRxDescp->rxd_info1.word = 0;
		pRxDescp->rxd_info2.word = 0;
		pRxDescp->rxd_info3.word = 0;
		pRxDescp->rxd_info4.word = 0;

		// Assign flag
		pRxDescp->rxd_info2.bits.LS0 = 1;  
		
		if( (skb = alloc_skb(2000)) == NULL ) {
			printf("skb is null\n");
			return 1;
		}

		skb_reserve(skb, 2);
		pRxDescp->rxd_info1.bits.PDP0 = K1_TO_PHY(skb->data);
#ifdef TCSUPPORT_MT7510_FE
                pRxDescp->rxd_info2.bits.PLEN0  = 1518;
#endif	
#ifndef FE_BYTE_SWAP
		pRxDescp->rxd_info1.word = cpu_to_le32(pRxDescp->rxd_info1.word);
		pRxDescp->rxd_info2.word = cpu_to_le32(pRxDescp->rxd_info2.word);
		pRxDescp->rxd_info3.word = cpu_to_le32(pRxDescp->rxd_info3.word);
		pRxDescp->rxd_info4.word = cpu_to_le32(pRxDescp->rxd_info4.word);
#endif

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


	return;
}

#if defined(TCSUPPORT_BOOTROM_LARGE_SIZE)
int tc_mii_ext_station_fill_addr_ext(uint32 portAddr, uint32 devAddr, uint32 regAddr)
{
	uint32 pbus_addr;
	uint32 pbus_data;
	uint32 phy_acs_st;
	uint32 max_wait_cnt = 10000;

	pbus_addr = 0x701C;

	// 1. write phy_addr & phy_reg & phy_data
	// b31    - phy access 1:start&busy, 0:complete&idle
	// b29:25 - mdio phy reg addr
	// b24:20 - mdio phy addr
	// b19:18 - (Clause45) 2'b00:address, 2'b01:write, 2'b11:read, 2'b10:post_read_inc_addr
	// b17:16 - start field, 2'b00:Clause45, 2'b01:Clause22
	// b15:0  - data
	pbus_data = 0x80000000; // write
	pbus_data = pbus_data | ((portAddr & 0x1F) << 20);
	pbus_data = pbus_data | ((devAddr & 0x1F) << 25);
	pbus_data = pbus_data | (regAddr & 0xFFFF);
	gswPbusWrite(pbus_addr, pbus_data);

	// 2. check phy_acs_st
	phy_acs_st = 1;
	while (phy_acs_st) {
		pbus_data = gswPbusRead(pbus_addr);
		phy_acs_st = (pbus_data >> 31) & 0x1;
		if (--max_wait_cnt == 0)
		{
			prom_printf("\n\n\n!!! %s hang : wait busy bit timeout for %lX/%lX/%lX !!!\n\n\n", __FUNCTION__, portAddr, devAddr, regAddr);
			break;
		}
	}

	// 3. complete
	return (0);
}/*end tc_mii_ext_station_fill_addr_ext*/

void tc_mii_ext_station_write_ext(uint32 portAddr, uint32 devAddr, uint32 regAddr, uint32 miiData)
{
	uint32 pbus_addr;
	uint32 pbus_data;
	uint32 phy_acs_st;
	uint32 max_wait_cnt = 10000;
	portAddr &= 0x1F;
	devAddr &= 0x1F;
	regAddr &= 0xFFFF;

	tc_mii_ext_station_fill_addr_ext(portAddr, devAddr, regAddr);

	pbus_addr = 0x701C;

	// 1. write phy_addr & phy_reg & phy_data
	// b31    - phy access 1:start&busy, 0:complete&idle
	// b29:25 - mdio phy reg addr
	// b24:20 - mdio phy addr
	// b19:18 - (Clause45) 2'b00:address, 2'b01:write, 2'b11:read, 2'b10:post_read_inc_addr
	// b17:16 - start field, 2'b00:Clause45, 2'b01:Clause22
	// b15:0  - data
	pbus_data = 0x80040000; // write
	pbus_data = pbus_data | (portAddr << 20);
	pbus_data = pbus_data | (devAddr << 25);
	pbus_data = pbus_data | (miiData & 0xFFFF);
	gswPbusWrite(pbus_addr, pbus_data);

	// 2. check phy_acs_st
	phy_acs_st = 1;
	while (phy_acs_st) {
		pbus_data = gswPbusRead(pbus_addr);
		phy_acs_st = (pbus_data >> 31) & 0x1;
		if (--max_wait_cnt == 0)
		{
			//prom_printf("\n\n\n!!! %s hang : wait busy bit timeout for %lX/%lX/%lX !!!\n\n\n", __FUNCTION__, portAddr, devAddr, regAddr);
			break;
		}
	}

	// 3. complete

}/*end tc_mii_ext_station_write_ext*/

void macCfgExtSwitch(void)
{
	//reset ext switch
	write_reg_word(0xbfb00358, 0x55550001);
	pause(1);
	write_reg_word(0xbfb00358, 0x55551001);
	pause(1);
	
	gswPbusWrite(0x7018, 0x7f7f8600);
	gswPbusWrite(0x7804, 0x000160ef);
	//workaround for 7530 E2
	tc_mii_ext_station_write_ext(0,0x1f,0x103,0x0020);
	tc_mii_ext_station_write_ext(0,0x1f,0x104,0x0608);
	tc_mii_ext_station_write_ext(0,0x1f,0x104,0x2608);

	write_reg_word(0xBFB58000 + 0x7018, 0x7f7f8c08);
	gswPbusWrite(0x3600, 0x5e33b);
	write_reg_word(0xBFB58000 + 0x3500, 0x5e33b);
	write_reg_word(0xBFB58000 + 0x3600, 0x5e33b);
	return;
}
#endif


void macIMRMask(void)
{
	uint32 reg = 0;

	reg |= RX_COHERENT | RX_DLY_INT | TX_COHERENT |
			RX_DONE_INT1 | RX_DONE_INT0;
	write_reg_word(INT_MASK, reg);
}

void macSetIntConf(void)
{
}

// Assign Tx Rx Descriptor Control Registers
void macSetDMADescrCtrlReg(macAdapter_t *mac_p)
{
  	write_reg_word(TX_BASE_PTR(0), K1_TO_PHY(mac_p->txDescrRingBaseVAddr));
  	write_reg_word(RX_BASE_PTR(0), K1_TO_PHY(mac_p->rxDescrRingBaseVAddr));
	
  	write_reg_word(TX_MAX_CNT(0), mac_p->txRingSize);
  	write_reg_word(RX_MAX_CNT(0), mac_p->rxRingSize);

	write_reg_word(TX_CTX_IDX(0), 0);
	write_reg_word(RX_CALC_IDX(0), mac_p->rxRingSize - 1);

	write_reg_word(PDMA_RST_IDX, RST_DTX_IDX(0));
	write_reg_word(PDMA_RST_IDX, RST_DRX_IDX(0));
}

void macSetGSW(macAdapter_t *mac_p)
{
	uint32 reg;
#ifdef TCSUPPORT_MT7510_FE
	if(isMT7520 || isMT7520G || isMT7525 || isMT7525G || isMT7520S){
		write_reg_word(GSW_ERLCR(6),0x1408488);//set p6 egress traffic limit (cir=10mbps,cbs=4kbyte)
	}
#endif

	/* set port 6 as 1Gbps, FC on */
	reg = (IPG_CFG_SHORT<<IPG_CFG_PN_SHIFT) | MAC_MODE_PN | FORCE_MODE_PN | 
		MAC_TX_EN_PN | MAC_RX_EN_PN | BKOFF_EN_PN | BACKPR_EN_PN | 
		ENABLE_RX_FC_PN | ENABLE_TX_FC_PN | (PN_SPEED_1000M<<FORCE_SPD_PN_SHIFT) | 
		FORCE_DPX_PN | FORCE_LNK_PN;
	write_reg_word(GSW_PMCR(6), reg);

	/* set cpu port as port 6 */
	reg = (0xff<<MFC_BC_FFP_SHIFT) | (0xff<<MFC_UNM_FFP_SHIFT) | (0xff<<MFC_UNU_FFP_SHIFT) |
			MFC_CPU_EN	| (6<<MFC_CPU_PORT_SHIFT);
	write_reg_word(GSW_MFC, reg);

#if !defined(TCSUPPORT_CT_WAN_PTM)
	/* check if FPGA */
	if (isFPGA) {
#ifdef TCSUPPORT_MT7510_FE
		/*decrease mdc/mdio clock*/
		reg = read_reg_word(GSW_CFG_PPSC);
		reg &= ~((1<<6) | (1<<7));
		write_reg_word(GSW_CFG_PPSC, reg);
		if ((miiStationRead(1, 2) == 0x4d) && (miiStationRead(1, 3) == 0xd072)){
			/* auto polling enable, 2 PHY ports, start PHY addr=6 and end PHY addr=7 */
			reg = read_reg_word(GSW_CFG_PPSC);
			reg &= ~((1<<24) | (1<<25)| (1<<26)| (1<<27)| (1<<28)| (1<<29));
			reg |= ((1<<24) | (1<<25));
			reg &= ~(PHY_END_ADDR | PHY_ST_ADDR);
			reg |= (2<<PHY_END_ADDR_SHIFT) | (1<<PHY_ST_ADDR_SHIFT);
		} 
		write_reg_word(GSW_CFG_PPSC, reg);
#else
		/* auto polling enable, 2 PHY ports, start PHY addr=6 and end PHY addr=7 */
		reg = read_reg_word(GSW_CFG_PPSC);
		reg |= PHY_AP_EN | EMB_AN_EN;
		reg &= ~(PHY_END_ADDR | PHY_ST_ADDR);
		/* check 6 PHY ports board or 2 PHY port board */
		if ((miiStationRead(0, 2) == 0x243) && (miiStationRead(0, 3) == 0xc54)) 
			reg |= (5<<PHY_END_ADDR_SHIFT) | (0<<PHY_ST_ADDR_SHIFT);
		else
			reg |= (7<<PHY_END_ADDR_SHIFT) | (6<<PHY_ST_ADDR_SHIFT);
		write_reg_word(GSW_CFG_PPSC, reg);
#endif
	}
	#ifdef TCSUPPORT_MT7510_FE
	else {
		/* Sideband signal error for Port 3, which need the auto polling */
		write_reg_word(GSW_BASE+0x7018, 0x7f7f8c08);
	}
	#endif
#else
	write_reg_word(GSW_BASE+0x7018, 0x7f7f8c08);
#endif

#ifndef TCSUPPORT_MT7510_FE
	/* set port 5 giga port RX clock phase to degree 0 */
	reg = read_reg_word(GSW_CFG_GPC);
#if defined(MT7530_SUPPORT)
	reg |= RX_CLK_MODE | (0x3f<<24);
#else
	reg |= RX_CLK_MODE;
#endif
	write_reg_word(GSW_CFG_GPC, reg);
#endif	

#if defined(MT7530_SUPPORT)
	// temp workaround for avoiding MT7530 7 seconds auto-powerdown
	gswPbusWrite(0x30F0, 0x00001E02);   // CKGCR : clear bit 0


	//Disable Port 5/Port 6 SA Learning
	write_reg_word(GSW_PSC(5), 0xfff10);
	write_reg_word(GSW_PSC(6), 0xfff10);

	/* set port 5 as 1000Mbps */
	reg = (IPG_CFG_SHORT<<IPG_CFG_PN_SHIFT) | MAC_MODE_PN | FORCE_MODE_PN | 
		MAC_TX_EN_PN | MAC_RX_EN_PN | BKOFF_EN_PN | BACKPR_EN_PN | 
		(PN_SPEED_100M<<FORCE_SPD_PN_SHIFT) | FORCE_DPX_PN | FORCE_LNK_PN;
		
	write_reg_word(GSW_PMCR(5), reg);
	gswPbusWrite(0x3600, reg); //external port6
#endif
}

void macSetMACCR(macAdapter_t *map_p)
{
	uint32 reg;

	reg = (12<<GDM_JMB_LEN_SHIFT) | GDM_STRPCRC |
		(GDM_P_CPU<<GDM_UFRC_P_SHIFT) | (GDM_P_CPU<<GDM_BFRC_P_SHIFT) | 
		(GDM_P_CPU<<GDM_MFRC_P_SHIFT) | (GDM_P_CPU<<GDM_OFRC_P_SHIFT);
	write_reg_word(GDMA1_FWD_CFG, reg);

	/* check if FPGA */
#if !defined(TCSUPPORT_CT_WAN_PTM)
	if (isFPGA) {
		/* set 1us clock for FPGA */
		reg = read_reg_word(CR_CLK_CFG);
		reg &= ~(0x3f000000);
#ifdef TCSUPPORT_MT7510_FE
		reg |= (0x31<<24);
#else
		reg |= (0x32<<24);
#endif
		write_reg_word(CR_CLK_CFG, reg);
	}
#endif
}

void macSetMacReg(macAdapter_t *mac_p)
{
	write_reg_word(GDMA1_MAC_ADRL, mac_p->macAddr[2]<<24 | mac_p->macAddr[3]<<16 | \
                               mac_p->macAddr[4]<<8  | mac_p->macAddr[5]<<0);
	write_reg_word(GDMA1_MAC_ADRH, mac_p->macAddr[0]<<8  | mac_p->macAddr[1]<<0);

	/* fill in switch's MAC address */
	write_reg_word(GSW_SMACCR0, mac_p->macAddr[2]<<24 | mac_p->macAddr[3]<<16 | \
                               mac_p->macAddr[4]<<8  | mac_p->macAddr[5]<<0);
	write_reg_word(GSW_SMACCR1, mac_p->macAddr[0]<<8  | mac_p->macAddr[1]<<0);
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
    
#if defined(MT7530_SUPPORT)
	eth_gpioReset();
#endif
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

   	macSetGSW(mac_p);

    return 0;
}

void macDrvStart(void)
{
	uint32 reg;

	reg = read_reg_word(PDMA_GLO_CFG);
	reg |= TX_WB_DDONE | RX_DMA_EN | TX_DMA_EN;
#ifdef FE_BYTE_SWAP
	reg |= PDMA_BYTE_SWAP;
#endif
#ifdef TCSUPPORT_MT7510_FE
		reg |= RX_2BYTE_OFFSET;
#endif
	write_reg_word(PDMA_GLO_CFG, reg);
}

int macSetRxMode(void)
{
	return 0;
} /* macSetRxMode */

macTxDescr_t *macTxRingProc(macAdapter_t *mac_p)
{
	volatile macTxDescr_t *pTxDescp;
	volatile macTxDescr_t pTxDescpTmpVal;
	volatile macTxDescr_t *pTxDescpTmp = &pTxDescpTmpVal;
	sk_buff *freeskb;

	pTxDescp = ((macTxDescr_t*)mac_p->txDescrRingBaseVAddr) + mac_p->txUnReleasedDescp;

#ifdef FE_BYTE_SWAP
	pTxDescpTmp = pTxDescp;
#else
	pTxDescpTmp->txd_info1.word = le32_to_cpu(pTxDescp->txd_info1.word);
	pTxDescpTmp->txd_info2.word = le32_to_cpu(pTxDescp->txd_info2.word);
	pTxDescpTmp->txd_info3.word = le32_to_cpu(pTxDescp->txd_info3.word);
	pTxDescpTmp->txd_info4.word = le32_to_cpu(pTxDescp->txd_info4.word);
#endif
  	while (mac_p->txUnReleasedBufCnt != 0) {
  		if (!pTxDescpTmp->txd_info2.bits.DDONE_bit) 
			return 0; // owned by MAC engine, something wrong here!

		/*
		printf("ERR TX tx curr=%d tx=%08x\n", mac_p->txUnReleasedDescp, pTxDescp);
		printf(" tdes1=%08x\n", pTxDescpTmp->txd_info1.word);
		printf(" tdes2=%08x\n", pTxDescpTmp->txd_info2.word);
		printf(" tdes3=%08x\n", pTxDescpTmp->txd_info3.word);
		printf(" tdes4=%08x\n", pTxDescpTmp->txd_info4.word);
		*/

		freeskb = mac_p->txskbs[mac_p->txUnReleasedDescp];
		free_skb(freeskb);

  		//pTxDescpTmp->txd_info1.bits.SDP0 = NULL;
		mac_p->txskbs[mac_p->txUnReleasedDescp] = NULL;

		if (mac_p->txUnReleasedDescp == (mac_p->txRingSize -1))
			mac_p->txUnReleasedDescp = 0;
		else
			mac_p->txUnReleasedDescp++;
		mac_p->txUnReleasedBufCnt--;

		pTxDescp = ((macTxDescr_t*)mac_p->txDescrRingBaseVAddr) + mac_p->txUnReleasedDescp;

#ifdef FE_BYTE_SWAP
		pTxDescpTmp = pTxDescp;
#else
		pTxDescpTmp->txd_info1.word = le32_to_cpu(pTxDescp->txd_info1.word);
		pTxDescpTmp->txd_info2.word = le32_to_cpu(pTxDescp->txd_info2.word);
		pTxDescpTmp->txd_info3.word = le32_to_cpu(pTxDescp->txd_info3.word);
		pTxDescpTmp->txd_info4.word = le32_to_cpu(pTxDescp->txd_info4.word);
#endif
	} 

	return (macTxDescr_t*) pTxDescp;
}

#ifdef TCSUPPORT_MT7510_FE
#define READ_CNT 3
int readRxDespDoneBit(macRxDescr_t *rxDescrp){
	uint32 cnt = READ_CNT;
	volatile macRxDescr_t *rxDescrpTmp = rxDescrp;

	while(cnt){
		if(rxDescrpTmp->rxd_info2.bits.DDONE_bit){
			return 1;
		}
		cnt--;
	}
	return 0;
}
#endif
void macRxRingProc(void)
{
	volatile macRxDescr_t *rxDescrp;
	volatile macRxDescr_t rxDescrpTmpVal;
	volatile macRxDescr_t *rxDescrpTmp = &rxDescrpTmpVal;
	uint32 frameSize;
	sk_buff *new_skb, *skb;
	uint32 reg;

	rxDescrp = ((macRxDescr_t*)mac_p->rxDescrRingBaseVAddr) + mac_p->rxCurrentDescp;

#ifdef FE_BYTE_SWAP
	rxDescrpTmp = rxDescrp;
#else
	rxDescrpTmp->rxd_info1.word = le32_to_cpu(rxDescrp->rxd_info1.word);
	rxDescrpTmp->rxd_info2.word = le32_to_cpu(rxDescrp->rxd_info2.word);
	rxDescrpTmp->rxd_info3.word = le32_to_cpu(rxDescrp->rxd_info3.word);
	rxDescrpTmp->rxd_info4.word = le32_to_cpu(rxDescrp->rxd_info4.word);
#endif

#ifdef TCSUPPORT_MT7510_FE
	while (readRxDespDoneBit(rxDescrpTmp)) {
#else
  	while (rxDescrpTmp->rxd_info2.bits.DDONE_bit) {
#endif

		frameSize = rxDescrpTmp->rxd_info2.bits.PLEN0;
		
		if ((frameSize < 60) || (frameSize > 1518)) {
			// Discard this packet & Repost this mbuf
			new_skb = mac_p->rxskbs[mac_p->rxCurrentDescp];
			goto DISCARD;
		}

		skb = (sk_buff *)(mac_p->rxskbs[mac_p->rxCurrentDescp]);
		skb->len = frameSize;

		new_skb = (sk_buff *)alloc_skb(2000);
		if ( !new_skb ) {	/* faild to allocate more mbuf-> drop this pkt */
			printf("mbuf alloc fails\n");
			new_skb = skb;
			goto RECVOK;
		} else {
			skb_reserve(new_skb, 2);
		}

		eth_rcv(skb);
DISCARD:

RECVOK:
		rxDescrpTmp->rxd_info1.bits.PDP0 = K1_TO_PHY(new_skb->data);
		mac_p->rxskbs[mac_p->rxCurrentDescp] = new_skb;
	
		rxDescrpTmp->rxd_info2.word = 0;  
		rxDescrpTmp->rxd_info2.bits.LS0 = 1;  
#ifdef TCSUPPORT_MT7510_FE
                rxDescrpTmp->rxd_info2.bits.PLEN0  = 1518;
#endif	
#ifndef FE_BYTE_SWAP
		rxDescrp->rxd_info1.word = le32_to_cpu(rxDescrpTmp->rxd_info1.word);
		rxDescrp->rxd_info2.word = le32_to_cpu(rxDescrpTmp->rxd_info2.word);
		rxDescrp->rxd_info3.word = le32_to_cpu(rxDescrpTmp->rxd_info3.word);
		rxDescrp->rxd_info4.word = le32_to_cpu(rxDescrpTmp->rxd_info4.word);
#endif

		reg = rxDescrp->rxd_info2.word;

		write_reg_word(RX_CALC_IDX(0), mac_p->rxCurrentDescp);
		
 		/* next descriptor*/
		mac_p->rxCurrentDescp = (mac_p->rxCurrentDescp + 1) % mac_p->rxRingSize;

		rxDescrp = ((macRxDescr_t*)mac_p->rxDescrRingBaseVAddr) + mac_p->rxCurrentDescp;

#ifdef FE_BYTE_SWAP
		rxDescrpTmp = rxDescrp;
#else
		rxDescrpTmp->rxd_info1.word = le32_to_cpu(rxDescrp->rxd_info1.word);
		rxDescrpTmp->rxd_info2.word = le32_to_cpu(rxDescrp->rxd_info2.word);
		rxDescrpTmp->rxd_info3.word = le32_to_cpu(rxDescrp->rxd_info3.word);
		rxDescrpTmp->rxd_info4.word = le32_to_cpu(rxDescrp->rxd_info4.word);
#endif
	} /* while loop  */
}

void mac_interrupt(int irq, void *dev_id, struct pt_regs *regs)
{           
	uint32 reg;

	VPint(CR_INTC_IMR) &= ~(1<<MAC_INT);

	reg = read_reg_word(INT_STATUS);
	write_reg_word(INT_STATUS, reg);

	// ----------Packet Received----------------------
	if (reg & (RX_COHERENT | RX_DLY_INT | RX_DONE_INT1 | RX_DONE_INT0)) {
		macRxRingProc();
	}

	if (reg & (RX_COHERENT | TX_COHERENT)) {
		printf("err mac_isr INT_STATUS=%08lx\n", reg);
	}			

	VPint(CR_INTC_IMR) |= (1<<MAC_INT);
}

int tc3162_eth_send(sk_buff *skb)
{
	macTxDescr_t *currDescrp = NULL;
	macTxDescr_t currDescrpTmpVal;
	macTxDescr_t *currDescrpTmp = &currDescrpTmpVal;
	uint32 length = skb->len;
	uint8 *bufAddrp = skb->data;
  	macTxDescr_t *tx; // just for debug only
	uint32 reg;
  	uint32 i;

	if (skb->data == NULL) {
		printf("Tx a empty mbuf\n"); 	
		return 1;
	}

	if (length < 60) {
		length = 60;
	}
  
    macTxRingProc(mac_p);

	if (mac_p->txUnReleasedBufCnt == mac_p->txRingSize) {
		printf("Tx ring full, drop packet\n"); 	
		free_skb(skb);
		return 1;
	}

	/* ----- Get the transmit descriptor ----- */
	currDescrp = ((macTxDescr_t*) mac_p->txDescrRingBaseVAddr) + mac_p->txCurrentDescp;

#ifdef FE_BYTE_SWAP
	currDescrpTmp = currDescrp;
#else
	currDescrpTmp->txd_info1.word = le32_to_cpu(currDescrp->txd_info1.word);
	currDescrpTmp->txd_info2.word = le32_to_cpu(currDescrp->txd_info2.word);
	currDescrpTmp->txd_info3.word = le32_to_cpu(currDescrp->txd_info3.word);
	currDescrpTmp->txd_info4.word = le32_to_cpu(currDescrp->txd_info4.word);
#endif

  	if (!currDescrpTmp->txd_info2.bits.DDONE_bit) {
		printf("Tx drop packet\n"); 	
		free_skb(skb);
		return 1;
	}

  	currDescrpTmp->txd_info1.bits.SDP0 = K1_TO_PHY(skb->data);
  	currDescrpTmp->txd_info2.bits.SDL0 = length;
	mac_p->txskbs[mac_p->txCurrentDescp] = skb;

  	currDescrpTmp->txd_info2.bits.DDONE_bit = 0;

#ifndef FE_BYTE_SWAP
	currDescrp->txd_info1.word = cpu_to_le32(currDescrpTmp->txd_info1.word);
	currDescrp->txd_info2.word = cpu_to_le32(currDescrpTmp->txd_info2.word);
	currDescrp->txd_info3.word = cpu_to_le32(currDescrpTmp->txd_info3.word);
	currDescrp->txd_info4.word = cpu_to_le32(currDescrpTmp->txd_info4.word);
#endif

	reg = currDescrp->txd_info2.word;

	mac_p->txCurrentDescp = (mac_p->txCurrentDescp + 1) % mac_p->txRingSize;

	write_reg_word(TX_CTX_IDX(0), mac_p->txCurrentDescp);

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
  	macRxDescr_t pRxDescpTmpVal;
  	macRxDescr_t *pRxDescpTmp = &pRxDescpTmpVal;
  	macTxDescr_t *pTxDescp;
  	macTxDescr_t pTxDescpTmpVal;
  	macTxDescr_t *pTxDescpTmp = &pTxDescpTmpVal;
  	uint32 i;

	printf("TX Descriptor Ring\n");
	pTxDescp = (macTxDescr_t*) mac_p->txDescrRingBaseVAddr;
	for (i = 0 ; i < mac_p->txRingSize ; i++, pTxDescp++) {
#ifdef FE_BYTE_SWAP
		pTxDescpTmp = pTxDescp;
#else
		pTxDescpTmp->txd_info1.word = le32_to_cpu(pTxDescp->txd_info1.word);
		pTxDescpTmp->txd_info2.word = le32_to_cpu(pTxDescp->txd_info2.word);
		pTxDescpTmp->txd_info3.word = le32_to_cpu(pTxDescp->txd_info3.word);
		pTxDescpTmp->txd_info4.word = le32_to_cpu(pTxDescp->txd_info4.word);
#endif

		printf("i= %d descr=%08x\n", i, pTxDescp);
		printf(" tdes1=%08x\n", pTxDescpTmp->txd_info1.word);
		printf(" tdes2=%08x\n", pTxDescpTmp->txd_info2.word);
		printf(" tdes3=%08x\n", pTxDescpTmp->txd_info3.word);
		printf(" tdes4=%08x\n", pTxDescpTmp->txd_info4.word);
		printf(" skb  =%08x\n", mac_p->txskbs[i]);
	}
	printf("txCurrentDescp    =%d\n", mac_p->txCurrentDescp);
	printf("txUnReleasedDescp =%d\n", mac_p->txUnReleasedDescp);
	printf("txUnReleasedBufCnt=%d\n", mac_p->txUnReleasedBufCnt);
	printf("TX_CTX_IDX(0)     =%08x\n", VPint(TX_CTX_IDX(0)));
	printf("TX_DTX_IDX(0)     =%08x\n", VPint(TX_DTX_IDX(0)));
	printf("TX_MAX_CNT(0)     =%08x\n", VPint(TX_MAX_CNT(0)));

	printf("RX Descriptor Ring\n");
	pRxDescp = (macRxDescr_t*) mac_p->rxDescrRingBaseVAddr;
	for( i = 0 ; i< mac_p->rxRingSize ; i++, pRxDescp++) {
#ifdef FE_BYTE_SWAP
		pRxDescpTmp = pRxDescp;
#else
		pRxDescpTmp->rxd_info1.word = le32_to_cpu(pRxDescp->rxd_info1.word);
		pRxDescpTmp->rxd_info2.word = le32_to_cpu(pRxDescp->rxd_info2.word);
		pRxDescpTmp->rxd_info3.word = le32_to_cpu(pRxDescp->rxd_info3.word);
		pRxDescpTmp->rxd_info4.word = le32_to_cpu(pRxDescp->rxd_info4.word);
#endif

		printf("i= %d descr=%08x\n", i, pRxDescp);
		printf(" rdes1=%08x\n", pRxDescpTmp->rxd_info1.word);
		printf(" rdes2=%08x\n", pRxDescpTmp->rxd_info2.word);
		printf(" rdes3=%08x\n", pRxDescpTmp->rxd_info3.word);
		printf(" rdes4=%08x\n", pRxDescpTmp->rxd_info4.word);
		printf(" skb  =%08x\n", mac_p->rxskbs[i]);
	}
	printf("rxCurrentDescp    =%d\n", mac_p->rxCurrentDescp);
	printf("RX_CALC_IDX(0)    =%08x\n", VPint(RX_CALC_IDX(0)));
	printf("RX_DRX_IDX(0)     =%08x\n", VPint(RX_DRX_IDX(0)));
	printf("RX_MAX_CNT(0)     =%08x\n", VPint(RX_MAX_CNT(0)));

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

#define GSW_CFG_CREV        (GSW_CFG_BASE + 0xFFC)
#define EXT_GSW_CFG_CREV    (0x7FFC)

uint8 use_ext_switch=0;

#if defined(TCSUPPORT_BOOTROM_LARGE_SIZE)
void detect_switch()
{
    uint32 switch_chip_id = 0;

    switch_chip_id = read_reg_word(GSW_CFG_CREV);
    if((switch_chip_id & 0xFFFF0000) == 0x75300000){
        use_ext_switch = 0;
    }
    switch_chip_id = gswPbusRead(EXT_GSW_CFG_CREV);
    if((switch_chip_id & 0xFFFF0000) == 0x75300000){
        use_ext_switch = 1;
    }
}
#endif


int tc3162_eth_init(unsigned char *mac_addr)
{
	int i;
	uint8 k;
#if defined(TC3262) && defined(TCSUPPORT_MULTI_UPGRADE)
	uint32 reg_val = 0;
#endif
	
	macControllerStop(mac_p);
	
	mac_p->macMemPool_p = &enetMemPool;

	/* ----- Set up the paramters ----- */
	macDefaultParaSet(mac_p);

 	macGetMacAddr(mac_p, mac_addr);
	macDrvDescripInit(mac_p);

	macDrvRegInit(mac_p);
	macPhyReset();
	pause(100);
#if defined(TCSUPPORT_BOOTROM_LARGE_SIZE)
    detect_switch();
#endif
#if defined(TC3262) && defined(TCSUPPORT_MULTI_UPGRADE)
	for(i=0;i<7;i++){
		reg_val |= VPint(GSW_MAC_BASE+i*0x100);
		reg_val &=0xffffffcf;/*Disable force TX/RX FC*/
		VPint(GSW_MAC_BASE+i*0x100)=reg_val;
	}	
#endif

	if(use_ext_switch)
	{
        gswPbusWrite(0x7000, 0x3); /* reset external switch*/
#if defined(TCSUPPORT_BOOTROM_LARGE_SIZE)
		macCfgExtSwitch();
#endif
		//set I/O driving strength
		gswPbusWrite(0x7810,0x00010022);
			//printf("\r\nreg 0x7810 value is %08x\r\n",gswPbusRead(0x7810));
	}
#ifdef TCSUPPORT_CPU_MT7520
	/*OSBNB00031891: Add for Multi EPHY setting*/
		mac_p->enetPhyAddr = 0;
		for ( mac_p->enetPhyAddr = 0; mac_p->enetPhyAddr < 32; mac_p->enetPhyAddr++ ) {
			if(!macSearchPhyAddr(mac_p)){
				macPhyReset();
				
				//printf("set enetPhyAddr = %d,tcPhyPortNum=%d \n",mac_p->enetPhyAddr,tcPhyPortNumGet());//
				if (macSetUpPhy(mac_p)){
					return -1;
				}
				mac_p->enetPhyAddr += (tcPhyPortNumGet()-1);

				if(isMT7520 || isMT7520S || isMT7525)
					continue;	// init port 4 phy
				else 
					break;
			}
			
		}
		mac_p->enetPhyAddr = 0;
#else		
	macSearchPhyAddr(mac_p);
	printf("Search PHY addr and found PHY addr=%d\n", mac_p->enetPhyAddr);
	if (macSetUpPhy(mac_p)){
		return -1;
	}
#endif


	request_IRQ(MAC_INT, &mac_irq, NULL);
	macDrvStart();

	macSetRxMode();

	return 0;
}

int tc3162_eth_exit(void)
{
	return 0;
}

