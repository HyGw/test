#include <linux/types.h>
#include <linux/interrupt.h>
#include "tc3262gmac.h"
#include "switch_slt.h"

// MT7530 Switch SLT Program Function
#if defined(MT7530_SLT)

#define printf prom_printf
#define tcMiiStationRead(x, y) 		miiStationRead(x, y)
#define tcMiiStationWrite(x, y, z) 	miiStationWrite(x, y, z)

typedef struct autobench_gpio{
	unsigned long chipId;
	int gpio_type;
	int gpio_1;
	int gpio_2;
	int gpio_3;
	unsigned char gpio_op;
} autobench_gpio_t;

autobench_gpio_t gpio_set[]={
	{MT7530_CHIP, GPIO_BIN0,	23,		24,		25,		0x0},
	{MT7530_CHIP, GPIO_BIN1,	23, 	24,		25,		0x1},
	{MT7530_CHIP, GPIO_BIN2, 	23, 	24,		25,		0x2},
	{MT7530_CHIP, GPIO_BIN3, 	23, 	24,		25,		0x3},
	{MT7530_CHIP, GPIO_BIN4, 	23, 	24,		25,		0x4},
	{MT7530_CHIP, GPIO_BIN5, 	23, 	24,		25,		0x5},
	{MT7530_CHIP, GPIO_BIN6, 	23, 	24,		25,		0x6},
	{MT7530_CHIP, GPIO_BIN7, 	23, 	24,		25,		0x7},
	{MT7530_CHIP, GPIO_EOT, 	26, 	-1,		-1,		0x0},
	{0, 0, 0, 0},
};

uint32 		test_pkt[3];
uint32 		pkt_loss_limit[3];
spinlock_t 	global_phy_lock;
spinlock_t 	global_pbus_lock;

void delay1ms(int ms)
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


/* 
 *  function name: gswPbusWrite
 *  description:   read MT7530 external switch register 
 */
uint32 gswPbusRead(uint32 pbus_addr)
{
	uint32 pbus_data, phyaddr, reg, value;
	unsigned long flags;

	spin_lock_irqsave(&global_pbus_lock, flags);

	phyaddr = 31;
    
	// 1. write high-bit page address
	reg = 31;
	value = (pbus_addr >> 6);
	tcMiiStationWrite(phyaddr, reg, value);

	// 2. read low DWord
	reg = (pbus_addr>>2) & 0x000f;
	value = tcMiiStationRead(phyaddr, reg);
	pbus_data = value;

	// 3. read high DWord
	reg = 16;
	value = tcMiiStationRead(phyaddr, reg);
	pbus_data = (pbus_data) | (value<<16);

	spin_unlock_irqrestore(&global_pbus_lock, flags);
	return pbus_data;
} 

/* 
 *  function name: gswPbusWrite
 *  description:   write MT7530 external switch register 
 */
int gswPbusWrite(uint32 pbus_addr, uint32 pbus_data)
{
	uint32 phyaddr;
	uint32 reg;
	uint32 value;
	unsigned long flags;

	spin_lock_irqsave(&global_pbus_lock, flags);

	phyaddr = 31;

	// 1. write high-bit page address
	reg = 31;
	value = (pbus_addr >> 6);
	tcMiiStationWrite(phyaddr, reg, value);

	// 2. write low DWord
	reg = (pbus_addr>>2) & 0x000f;
	value = pbus_data & 0xffff;
	tcMiiStationWrite(phyaddr, reg, value);

	// 3. write high DWord
	reg = 16;
	value = (pbus_data>>16) & 0xffff;
	tcMiiStationWrite(phyaddr, reg, value);

	spin_unlock_irqrestore(&global_pbus_lock, flags);

	return 0;
}


/* 
 *  function name: tcMiiBpw
 *  description:   write MT7530 external switch phy register by pass through 
 */
void tcMiiBpw(uint32 enetPhyAddr, uint32 phyReg, uint32 miiData)
{
	uint32 reg;
	uint32 cnt=10000;
	
	if (isTC3169) {
		reg = (miiData & PHYOR_DATA) | (phyReg << PHYOR_REG_SHIFT) | 
			  (enetPhyAddr << PHYOR_ADDR_SHIFT) | PHYOR_WRITE | PHYOR_START;

		VPint(CR_GMAC_PHYOR)=reg;

		do {
			reg=VPint(CR_GMAC_PHYOR);
			if (--cnt == 0)
			{
				prom_printf("\n\n\n!!! %s hang : wait busy bit timeout for %lX/%lX !!!\n\n\n", __FUNCTION__, enetPhyAddr, phyReg);
				break;
			}

		} while (reg & PHYOR_START);
	} else {
		reg = (miiData & NEW_PHYOR_DATA) |
				(phyReg << NEW_PHYOR_REG_SHIFT) | (enetPhyAddr << NEW_PHYOR_ADDR_SHIFT) |
				(NEW_PHYOR_OP_WR << NEW_PHYOR_OP_SHIFT) | (NEW_PHYOR_ST_IEEE_OLD << NEW_PHYOR_ST_SHIFT) |
				NEW_PHYOR_BUSY;
		VPint(CR_GMAC_PHYOR) = reg;

		do {
			//do not need pause here because throughput decrease much. shnwind 20101109.
			reg=VPint(CR_GMAC_PHYOR);
			if (--cnt == 0)
			{
				prom_printf("\n\n\n!!! %s hang : wait busy bit timeout for %lX/%lX !!!\n\n\n", __FUNCTION__, enetPhyAddr, phyReg);
				break;
			}
		} while (reg & NEW_PHYOR_BUSY);
	}
}


/* 
 *  function name: tcMiiBpr
 *  description:   read MT7530 external switch phy register by pass through 
 */
int tcMiiBpr(uint32 enetPhyAddr, uint32 phyReg)
{
	uint32 reg;
	uint32 cnt=10000;

	if (isTC3169) {
		reg = (phyReg << PHYOR_REG_SHIFT) | (enetPhyAddr << PHYOR_ADDR_SHIFT) | PHYOR_START;
		VPint(CR_GMAC_PHYOR) = reg;

		do {
			//do not need pause here because throughput decrease much. shnwind 20101109.
			reg=VPint(CR_GMAC_PHYOR);
			if (--cnt == 0)
			{
				prom_printf("\n\n\n!!! %s hang : wait busy bit timeout for %lX/%lX !!!\n\n\n", __FUNCTION__, enetPhyAddr, phyReg);
				break;
			}
		} while (reg & PHYOR_START);

		reg = (reg & PHYOR_DATA);
	} else {
		reg = (phyReg << NEW_PHYOR_REG_SHIFT) | (enetPhyAddr << NEW_PHYOR_ADDR_SHIFT) |
				(NEW_PHYOR_OP_RD << NEW_PHYOR_OP_SHIFT) | (NEW_PHYOR_ST_IEEE_OLD << NEW_PHYOR_ST_SHIFT) |
				NEW_PHYOR_BUSY;
		VPint(CR_GMAC_PHYOR)=reg;

		do {
			//pause(1);
			reg=VPint(CR_GMAC_PHYOR);
			if (--cnt == 0)
			{
				prom_printf("\n\n\n!!! %s hang : wait busy bit timeout for %lX/%lX !!!\n\n\n", __FUNCTION__, enetPhyAddr, phyReg);
				break;
			}
		} while (reg & NEW_PHYOR_BUSY);

		reg = (reg & NEW_PHYOR_DATA);
	}
    return reg;
}

/*_____________________________________________________________________________
**      function name: tcMiiExtStationFillAddr
**      descriptions:
**         Fill the address to prepare aceess the phy register.
**      parameters:
**            portAddr : Port Address
**            devAddr : Device Address
**            regAddr : Register Address
**
**      global:
**            None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**      write_reg_word
**      read_reg_word
**      DEBUGMSG
**
**      revision:
**      1. WPLin 2012/08/01
**____________________________________________________________________________
*/
int tcMiiExtStationFillAddr(uint32 portAddr, uint32 devAddr, uint32 regAddr)
{
#if defined(CONFIG_MIPS_RT65168)
	uint32 value;
	uint32 cnt = 10000;

	if (isRT65168) {
		value = (regAddr & NEW_PHYOR_DATA) | (devAddr << NEW_PHYOR_REG_SHIFT) | 
				(portAddr << NEW_PHYOR_ADDR_SHIFT) | (NEW_PHYOR_OP_EEE_FA << NEW_PHYOR_OP_SHIFT) | 
				(NEW_PHYOR_ST_IEEE_NEW << NEW_PHYOR_ST_SHIFT) | NEW_PHYOR_BUSY;

		VPint(CR_GMAC_PHYOR)=value;

		do {
			value=VPint(CR_GMAC_PHYOR);
			if (--cnt == 0)
			{
				prom_printf("\n\n\n!!! %s hang : wait busy bit timeout for %lX/%lX/%lX !!!\n\n\n", __FUNCTION__, portAddr, devAddr, regAddr);
				break;
			}
		} while (value & NEW_PHYOR_BUSY);
	}
#endif

	return (0);
}/*end tcMiiExtStationFillAddr*/

/*_____________________________________________________________________________
**      function name: tcMiiExtStationFillAddr_ext
**      descriptions:
**         Fill the address to prepare aceess the phy register.
**      parameters:
**            portAddr : Port Address
**            devAddr : Device Address
**            regAddr : Register Address
**
**      global:
**            None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**      write_reg_word
**      read_reg_word
**      DEBUGMSG
**
**      revision:
**      1. WPLin 2012/08/01
**____________________________________________________________________________
*/
int tcMiiExtStationFillAddr_ext(uint32 portAddr, uint32 devAddr, uint32 regAddr)
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
}/*end tcMiiExtStationFillAddr_ext*/

/*_____________________________________________________________________________
**      function name: tcMiiExtStationWrite_ext
**      descriptions:
**        Used 45.3 method to write the phy register
**      parameters:
**            portAddr : Port Address
**            devAddr : Device Address
**            regAddr : Register Address
**            miiData : Write Data
**
**      global:
**            None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**      write_reg_word
**      read_reg_word
**      DEBUGMSG
**
**      revision:
**      1. WPLin 2012/08/01
**____________________________________________________________________________
*/
void tcMiiExtStationWrite_ext(uint32 portAddr, uint32 devAddr, uint32 regAddr, uint32 miiData)
{
	uint32 pbus_addr;
	uint32 pbus_data;
	uint32 phy_acs_st;
	uint32 max_wait_cnt = 10000;
	portAddr &= 0x1F;
	devAddr &= 0x1F;
	regAddr &= 0xFFFF;

	tcMiiExtStationFillAddr_ext(portAddr, devAddr, regAddr);

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
			prom_printf("\n\n\n!!! %s hang : wait busy bit timeout for %lX/%lX/%lX !!!\n\n\n", __FUNCTION__, portAddr, devAddr, regAddr);
			break;
		}
	}

	// 3. complete

}/*end tcMiiExtStationWrite_ext*/

/*_____________________________________________________________________________
**      function name: tcMiiExtStationRead_ext
**      descriptions:
**        Used 45.3 method to read the phy register
**      parameters:
**            portAddr : Port Address
**            devAddr : Device Address
**            regAddr : Register Address
**            op : 0:Normal read
**			 1:Post read
**
**
**      global:
**            None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**      write_reg_word
**      read_reg_word
**      DEBUGMSG
**
**      revision:
**      1. WPLin 2012/08/01
**____________________________________________________________________________
*/
uint32 tcMiiExtStationRead_ext(uint32 portAddr, uint32 devAddr, uint32 regAddr, uint8 op)
{
	uint32 pbus_addr;
	uint32 pbus_data;
	uint32 phy_acs_st;
	uint32 max_wait_cnt = 10000;

	portAddr &= 0x1F;
	devAddr &= 0x1F;
	regAddr &= 0xFFFF;

	if (op != POST_READ){
		/*POST READ command only to fill the address once, the address will be
		increased once automatically.*/
		tcMiiExtStationFillAddr_ext(portAddr, devAddr, regAddr);
	}

	pbus_addr = 0x701C;

	// 1. write phy_addr & phy_reg & phy_data
	// b31    - phy access 1:start&busy, 0:complete&idle
	// b29:25 - mdio phy reg addr
	// b24:20 - mdio phy addr
	// b19:18 - (Clause45) 2'b00:address, 2'b01:write, 2'b11:read, 2'b10:post_read_inc_addr
	// b17:16 - start field, 2'b00:Clause45, 2'b01:Clause22
	// b15:0  - data
	pbus_data = (op == POST_READ) ? 0x80080000 : 0x800C0000;
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

	// 3. return data
	return (pbus_data & 0xFFFF);
}/*end tcMiiExtStationRead_ext*/



/* 
 *  function name: mtEmiiRegWrite
 *  description:   write external phy register in MT7530 external switch 
 */
void mtEMiiRegWrite(uint32 port_num, uint32 dev_num, uint32 reg_num, uint32 reg_data)
{
	tcMiiExtStationWrite_ext( port_num, dev_num, reg_num, reg_data);
}

/* 
 *  function name: mtEmiiRegRead
 *  description:   read external phy register in MT7530 external switch 
 */
uint32 mtEMiiRegRead(uint32 port_num, uint32 dev_num, uint32 reg_num)
{
	return(tcMiiExtStationRead_ext( port_num, dev_num, reg_num, NORMAL_READ));
}


/* 
 *  function name: gswPmiRead
 *  description:   read MT7530 switch phy register by pbus
 */
uint32 gswPmiRead(uint32 phy_addr, uint32 phy_reg)
{
	uint32 pbus_addr;
	uint32 pbus_data;

	uint32 phy_data;
	uint32 phy_acs_st;
	uint32 max_wait_cnt = 10000;

	pbus_addr = 0x701c;
	// b31    - phy access 1:start&busy, 0:complete&idle
	// b29:25 - mdio phy reg addr
	// b24:20 - mdio phy addr
	// b19:18 - 2'b01: write, 2'b10: read
	// b17:16 - start field, always 2'b01
	// b15:0  - data

	phy_addr = phy_addr & 0x1f;
	phy_reg  = phy_reg & 0x1f;

	// 1. write phy_addr & phy_reg
	pbus_data = 0x80090000; // read
	pbus_data = pbus_data | (phy_addr<<20);
	pbus_data = pbus_data | (phy_reg<<25);

	gswPbusWrite(pbus_addr,pbus_data);

	// 2. check phy_acs_st
	phy_acs_st = 1;
	while (phy_acs_st){
		pbus_data = gswPbusRead(pbus_addr);
		phy_acs_st = (pbus_data>>31) & 0x1;
		if (--max_wait_cnt == 0)
		{
			prom_printf("\n\n\n!!! %s hang : wait busy bit timeout for %lX/%lX !!!\n\n\n", __FUNCTION__, phy_addr, phy_reg);
			break;;
		}
	}

	// 3. return data
	phy_data = pbus_data & 0xffff;

	return phy_data;
}


/* 
 *  function name: gswPmiWrite
 *  description:   write MT7530 switch phy register by pbus
 */
uint32 gswPmiWrite(uint32 phy_addr, uint32 phy_reg, uint32 phy_data)
{
	uint32 pbus_addr;
	uint32 pbus_data;

	uint32 phy_acs_st;
	uint32 max_wait_cnt = 10000;

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

	// 2. check phy_acs_st
	phy_acs_st = 1;
	while (phy_acs_st){
		pbus_data = gswPbusRead(pbus_addr);
		phy_acs_st = (pbus_data>>31) & 0x1;
		if (--max_wait_cnt == 0)
		{
			prom_printf("\n\n\n!!! %s hang : wait busy bit timeout for %lX/%lX !!!\n\n\n", __FUNCTION__, phy_addr, phy_reg);
			break;;
		}
	}

	// 3. complete
	return 0;
}

/* 
 *  function name: tcMiiStationRead_ext
 *  description:   wrappter function for gswPmiRead
 */
uint32 tcMiiStationRead_ext(uint32 enetPhyAddr, uint32 phyReg)
{
	uint32 return_value;
	unsigned long flags;

	spin_lock_irqsave(&global_phy_lock, flags);
	return_value = gswPmiRead(enetPhyAddr, phyReg);
	spin_unlock_irqrestore(&global_phy_lock, flags);

	return return_value;
}

/* 
 *  function name: tcMiiStationWrite_ext
 *  description:   wrappter function for gswPmiWrite
 */
void tcMiiStationWrite_ext(uint32 enetPhyAddr, uint32 phyReg, uint32 miiData)
{
	unsigned long flags;

	spin_lock_irqsave(&global_phy_lock, flags);
	gswPmiWrite(enetPhyAddr, phyReg, miiData);
	spin_unlock_irqrestore(&global_phy_lock, flags);
}

/* 
 *  function name: mtMiiRegWrite
 *  description:   wrappter function for tcMiiStationWrite_ext
 */
void mtMiiRegWrite(uint32 port_num, uint32 reg_num, uint32 reg_data)
{
	tcMiiStationWrite_ext( port_num,reg_num, reg_data);
}

/* 
 *  function name: mtMiiRegRead
 *  description:   wrappter function for tcMiiStationRead_ext
 */
uint32 mtMiiRegRead(uint32 port_num,uint8 reg_num)
{
	return(tcMiiStationRead_ext( port_num,reg_num));
}


/* 
 *  function name: getMiiPage
 *  description:   get mii page number by command parameter
 */
uint16
getMiiPage (char *page)
{
    // g0,g1,g2,g3 = 0x0000~0x3000
    // l0,l1,l2,l3 = 0x8000~0xb000
	if( strcmp(page, "g0") == 0 ){
	    return (0x0000);
	}
	else if( strcmp(page, "g1") == 0 ){
	    return (0x1000);
	}
	else if( strcmp(page, "g2") == 0 ){
	    return (0x2000);
	}
	else if( strcmp(page, "g3") == 0 ){
	    return (0x3000);
	}
	else if( strcmp(page, "g4") == 0 ){
	    return (0x4000);
	}
	else if( strcmp(page, "l0") == 0 ){
	    return (0x8000);
	}
	else if( strcmp(page, "l1") == 0 ){
	    return (0x9000);
	}
	else if( strcmp(page, "l2") == 0 ){
	    return (0xa000);
	}
	else if( strcmp(page, "l3") == 0 ){
	    return (0xb000);
	}
	else if( strcmp(page, "ext") == 0 ){//Extended Page Registers
	    return (0x0001);
	}
	else if( strcmp(page, "misc") == 0 ){//Misc Page Registers
	    return (0x0002);
	}
	else if( strcmp(page, "lpi") == 0 ){//LPI Page Registers
	    return (0x0003);
	}
	else if( strcmp(page, "test") == 0 ){//Test Page Registers
	    return (0x2a30);
	}
	else if( strcmp(page, "tr") == 0 ){//Token Ring Debug Registers
		return (0x52b5);
	}
	else {
		prom_printf("* Wrong PageNo(%s).\r\n", page);
		prom_printf("* MT7530 PageNo=><g0|ext|misc|lpi|test|tr> \r\n");
		prom_printf("* Tcxxx PageNo=><g0|g1|g2|g3|g4|l0|l1|l2|l3> \r\n");
		return (0xFFFF);
    }
}


/* 
 *  function name: mtPhyChkVal
 *  description:   check phy register value (debug)
 */
void mtPhyChkVal (uint32 port_num)
{
	uint32 val;
	uint32 mr02,mr03;
	uint8 mr_anen, mr_dplx, mr_speed;
	uint8 mr_an_capable;
	uint8 mr_lp_an_capable;
	uint8 mr_lp_anen;
	//uint8 lr_force_mdix;
	uint8 lr_linkup, lr_speed, lr_dplx, lr_mdix;
	// show phy_id
	
	mtMiiRegWrite(port_num, 31, 0);
  
	val = mtMiiRegRead(port_num, 2); 
	mr02 = val&(0xffff);
	val = mtMiiRegRead(port_num, 3); 
	mr03 = val&(0xffff);
	printf(" mtPhy ID: %lx %lx\r\n",mr02,mr03); 
	
	val = mtMiiRegRead(port_num, 0);
	mr_anen = (val>>12)&0x01;
	mr_dplx = (val>>8)&0x01;
	//mr_speed = (val>>13)&0x01;
	mr_speed = ((val>>13)|(val>>5))&0x03;//00:10,01:100,10:1000
	// reg4h
	val = mtMiiRegRead(port_num, 0x04);
	mr_an_capable = (val>>5)&0x0F; //100F,100H,10F,10H

	// reg9h
	val = mtMiiRegRead(port_num, 0x09);
	mr_an_capable = mr_an_capable | ((val>>4)&0x30);//1000F,1000H

	// reg5h
	val = mtMiiRegRead(port_num, 0x05);
	mr_lp_an_capable = (val>>5)&0x0F; //100F,100H,10F,10H
	// regAh
	val = mtMiiRegRead(port_num, 0x0A);
	mr_lp_an_capable = mr_lp_an_capable | ((val>>6)&0x30); //1000F,1000H

	// reg6h
	val = mtMiiRegRead(port_num, 0x06);
	mr_lp_anen = (val)&0x01;

	val = mtEMiiRegRead(port_num, 0x1E, 0xA2);
//	val = mtPhyReadGReg(port_num,0x1E,0xA2);
  
    printf(" tcPhyChkVal= 0x%04lx",val);
  
	lr_linkup = (val>>9)&0x07;//dev1e_A2[9]:LinkStatus10_OK,dev1e_A2[10]:LinkStatus100_OK,dev1e_A2[11]:LinkStatus1000_OK
	lr_speed = (val>>1)&0x07; //dev1e_A2[1]:final_speed_10,,dev1e_A2[2]:dev1e_A2[2]:final_speed_100,dev1e_A2[3]:final_speed_1000
	lr_dplx = (val>>5)&0x01;
	lr_mdix = (val>>6)&0x01;

			
	printf(" TcPhy mode:");
	if (mr_anen){ // Auto-neg
	    printf(" AN-(");
	    printf(" %s,",((mr_an_capable>>5)&0x01)?"1000F":"");
	    printf(" %s,",((mr_an_capable>>4)&0x01)?"1000H":"");
	    printf(" %s,",((mr_an_capable>>3)&0x01)?"100F":"");
	    printf(" %s,",((mr_an_capable>>2)&0x01)?"100H":"");     
	    printf(" %s,",((mr_an_capable>>1)&0x01)?"10F":"");      
	    printf(" %s)\r\n",((mr_an_capable>>0)&0x01)?"10H":"");  
	}
	else { // Force-speed
		if (mr_speed==0x00){
		 	printf(" Force-%d%s\r\n",10,(mr_dplx?"F":"H"));
		}
		else if (mr_speed==0x01){
			printf(" Force-%d%s\r\n",100,(mr_dplx?"F":"H"));
		}
		else if (mr_speed==0x02){
			 printf(" Force-%d%s\r\n",1000,(mr_dplx?"F":"H"));
	 	}
	}
		
	if (!lr_linkup){ // link-down
		printf(" *** Link is down!\r\n");
    }
	else { // link-up
		if (lr_speed == 0x04){
			printf(" TCphy is link-up at 1000 Master ");
		} else if (lr_speed == 0x02){
			printf(" TCphy is link-up at 100 %s ",(lr_dplx?"F":"H"));
		} else if (lr_speed == 0x01){	
			printf(" TCphy is link-up at 10 %s ",(lr_dplx?"F":"H"));
		} else {
			printf(".\r\n");
		}
    }
        
	if(mr_lp_anen) {
		printf(" Link-partner supports AN-(");
		printf(" %s,",((mr_lp_an_capable>>5)&0x01)?"1000F":"");
		printf(" %s,",((mr_lp_an_capable>>4)&0x01)?"1000H":"");
		printf(" %s,",((mr_lp_an_capable>>3)&0x01)?"100F":"");
		printf(" %s,",((mr_lp_an_capable>>2)&0x01)?"100H":"");  
		printf(" %s,",((mr_lp_an_capable>>1)&0x01)?"10F":"");       
		printf(" %s)\r\n",((mr_lp_an_capable>>0)&0x01)?"10H":"");   
	}
	else {
		printf(" Link-partner operates in Force mode.\r\n");
	}
}

/* 
 *  function name: macSetGSW
 *  description:   set MT7530 switch register for SLT program
 *                 1. enable flooding port
 *                 2. use transparanet port
 *                 3. enable p5 & p6 auto polling
 */

void macSetGSW(void)
{
	gswPbusWrite(0x2004, 0xff0000);
	gswPbusWrite(0x2104, 0xff0000);
	gswPbusWrite(0x2204, 0xff0000);
	gswPbusWrite(0x2304, 0xff0000);
	gswPbusWrite(0x2404, 0xff0000);

	gswPbusWrite(0x2010, 0x810000c0);
	gswPbusWrite(0x2110, 0x810000c0);
	gswPbusWrite(0x2210, 0x810000c0);
	gswPbusWrite(0x2310, 0x810000c0);
	gswPbusWrite(0x2410, 0x810000c0);

	gswPbusWrite(0x7018, 0x7F7F8600);	
	gswPbusWrite(0x7b00, 0x00000002);	
}


/* 
 *  function name: phyMiiRead
 *  description:   read MT7530 switch phy page register 
 */
int phyMiiRead(uint16 phyaddr, uint16 page_val, uint32 reg)
{
	const uint16 page_reg=31;
	uint32 value=0;

    if (reg != 31){
		mtMiiRegWrite(phyaddr, page_reg, 0);
	}

    // check parameters
    if (phyaddr<=31 && reg<=31 && page_val!=0xffff){
        // set page
		mtMiiRegWrite(phyaddr, page_reg, page_val);
		value = mtMiiRegRead(phyaddr, reg);
        return value;           
    }   
    else { // error message
        prom_printf("phyMiiRead: Error\r\n");
        return 0;           
    }
}

/* 
 *  function name: phyMiiWrite
 *  description:   write MT7530 switch phy page register 
 */
int phyMiiWrite(uint16 phyaddr, uint16 page_val, uint32 reg, uint32 value)
{
	const uint16 page_reg=31;

    // check parameters and write
    if(reg != 31){
		mtMiiRegWrite(phyaddr, page_reg, 0);
	}
    
    if ((phyaddr<=31) && (reg<=31) && (page_val!=0xffff)){
        // set page
		mtMiiRegWrite(phyaddr, page_reg, page_val);
        // write data
        mtMiiRegWrite(phyaddr, reg, value);
    }
    return 0;
}

int MT7530_EEEPROM_Test()
{
	uint32 pbus_data, pbus_addr;

	prom_printf("MT7530_EEEPROM_Test\n\n");

	// Change to EEEPROM Mode
	mtEMiiRegWrite(0x0000, 0x001f, 0x0103, 0x0020);
	phyMiiWrite(0x0000, 0x0001, 0x0016, 0x0000);
	phyMiiWrite(0x0004, 0x0001, 0x0016, 0x0000);
	mtEMiiRegWrite(0x0000, 0x001f, 0x0104, 0x0608);
	mtEMiiRegWrite(0x0000, 0x001f, 0x0104, 0x2608);


	pbus_data = gswPbusRead(0x10);
	prom_printf("0x10: %x\n", pbus_data);

/****************************************************************/
	prom_printf("1. Indirect access EEPROM\n");

	pbus_data = gswPbusRead(0x7120);
	prom_printf("0x7120: %x\n", pbus_data);

	// read chip id
	pbus_addr = 0x7120;
	pbus_data = 0xC0003075;
	prom_printf("write 0xC0003075 value into 0x7120 register\n");
	gswPbusWrite(pbus_addr, pbus_data);
	delay1ms(100);

	pbus_addr = 0x7120;
	pbus_data = 0x80000000;
	prom_printf("write 0x80000000 value into 0x7120 register\n");
	gswPbusWrite(pbus_addr, pbus_data);
	delay1ms(100);

	prom_printf("read 0x7120 reg value\n");
	pbus_data = gswPbusRead(0x7120);
	prom_printf("0x7120 reg value: %x\n", pbus_data);

	if ((pbus_data & 0xffff) != 0x3075){
		prom_printf("Indirect access EEEPROM Fail\n");
		return -1;
	}

/****************************************************************/

	prom_printf("\n2. Power on and check load content\n");
	
	// eeeprom preload check
	pbus_addr = 0x7120;
	pbus_data = 0xC0003075;
	prom_printf("write 0xC0003075 value into 0x7120 register\n");
	gswPbusWrite(pbus_addr, pbus_data);

	pbus_addr = 0x7120;
	pbus_data = 0x80000000;
	prom_printf("write 0x80000000 value into 0x7120 register\n");
	gswPbusWrite(pbus_addr, pbus_data);

	pbus_addr = 0x7120;
	pbus_data = 0xC0021000;
	prom_printf("write 0xC0021000 value into 0x7120 register\n");
	gswPbusWrite(pbus_addr, pbus_data);

	prom_printf("Test correct chip id preload in eeeprom\n");
	pbus_addr = 0x7120;
	pbus_data = 0xC0045555;
	prom_printf("write 0xC0045555 value into 0x7120 register\n");
	gswPbusWrite(pbus_addr, pbus_data);

	pbus_data = gswPbusRead(0x10);
	prom_printf("0x10 reg value: %x\n", pbus_data);

	prom_printf("Reset Switch\n");
	pbus_addr = 0x7000;
	pbus_data = 0x80000003;
	gswPbusWrite(pbus_addr, pbus_data);
	delay1ms(100);

	pbus_data = gswPbusRead(0x10);
	prom_printf("0x10 reg value: %x\n", pbus_data);
	prom_printf("===============================================\n");
	if ((pbus_data & 0xffff) != 0x5555){
		prom_printf("EEEProm Proload Fail\n");
		return -1;
	}

	// error chip id cannot preload eeeprom
	prom_printf("Test error chip id preload in eeeprom\n");
	pbus_addr = 0x7120;
	pbus_data = 0xC0000000;
	prom_printf("write 0xC0000000 value into 0x7120 register\n");
	gswPbusWrite(pbus_addr, pbus_data);

	pbus_data = gswPbusRead(0x10);
	prom_printf("0x10 reg value: %x\n", pbus_data);

	prom_printf("Reset Switch\n");
	pbus_addr = 0x7000;
	pbus_data = 0x80000003;
	gswPbusWrite(pbus_addr, pbus_data);
	delay1ms(100);

	pbus_data = gswPbusRead(0x10);
	prom_printf("0x10 reg value: %x\n", pbus_data);
	prom_printf("===============================================\n");
	if (pbus_data != 0xffffff00){
		prom_printf("EEEPROM Error Chip ID Preload Test Fail\n");
		return -1;
	}

	// match escap condition
	prom_printf("Test match escap condition\n");
	pbus_addr = 0x7120;
	pbus_data = 0xC0020080;
	prom_printf("write 0xC0020080 value into 0x7120 register\n");
	gswPbusWrite(pbus_addr, pbus_data);

	pbus_data = gswPbusRead(0x10);
	prom_printf("0x10 reg: %x\n", pbus_data);

	prom_printf("Reset Switch\n");
	pbus_addr = 0x7000;
	pbus_data = 0x80000003;
	gswPbusWrite(pbus_addr, pbus_data);
	delay1ms(100);

	pbus_data = gswPbusRead(0x10);
	prom_printf("0x10 reg value: %x\n", pbus_data);
	prom_printf("================================================\n");
	if (pbus_data != 0xffffff00){
		prom_printf("EEEPROM Match Escap Condition Test Fail\n");
		return -1;
	}

	return 0;
}


/* 
 *  function name: Set_Test_Param
 *  description:   set test parameter for SLT program
 *                 1. test packet number 
 *                 2. delay seconds 
 */
void Set_Test_Param(int link_speed, uint16* pkt_unit, uint32* delay_ms)
{
	switch (link_speed)
	{	
		case LINK_SPEED_10M:
			*pkt_unit = 5;
			*delay_ms = 750;
			break;
		case LINK_SPEED_100M:
			*pkt_unit = 10;
			*delay_ms = 150;
			break;
		case LINK_SPEED_1000M:
			*pkt_unit = 10;
			*delay_ms = 50;
			break;
	}
}

/* 
 *  function name: Set_GswPhy_Speed
 *  description:   set link speed of gsw & phy
 */
int Set_GswPhy_Speed(char link_speed, int mode)
{
	int port_num, value=0;
	int offset=0x100;

	switch(link_speed)
	{
		case LINK_SPEED_10M:
			prom_printf("Test 10M Link Speed Mode\n");
			for (port_num=0; port_num<7; port_num++){
				gswPbusWrite((GSW_MAC_CTRL+(port_num*offset)), 0x5e333);
				phyMiiWrite(port_num, 0x0000, 0x0000, 0x100);
			}
			break;
		case LINK_SPEED_100M:
			prom_printf("Test 100M Link Speed Mode\n");
			for (port_num=0; port_num<7; port_num++){
				gswPbusWrite((GSW_MAC_CTRL+(port_num*offset)), 0x5e337);
				phyMiiWrite(port_num, 0x0000, 0x0000, 0x2100);
			}
			break;
		case LINK_SPEED_1000M:
			prom_printf("Test 1000M Link Speed Mode\n");
			if (mode==RELAY_P0P4){
				gswPbusWrite((GSW_MAC_CTRL+(0x0000*offset)), 0x5e33b);
				gswPbusWrite((GSW_MAC_CTRL+(0x0004*offset)), 0x5e33b);
				gswPbusWrite((GSW_MAC_CTRL+(0x0005*offset)), 0x5e33b);
				phyMiiWrite(0x0000, 0x0000, 0x0000, 0x1140);
				phyMiiWrite(0x0004, 0x0000, 0x0000, 0x1140);
				phyMiiWrite(0x0005, 0x0000, 0x0000, 0x1140);
			} else {
				for (port_num=0; port_num<7; port_num++){
					gswPbusWrite((GSW_MAC_CTRL+(port_num*offset)), 0x5e33b);
					if ((mode==RELAY_LPBK) || (mode==RELAY_LONG && port_num<4)){
						phyMiiWrite(port_num, 0x0000, 0x0009, 0x1f00);
						phyMiiWrite(port_num, 0x0000, 0x0018, 0x0001);
						phyMiiWrite(port_num, 0x52b5, 0x0011, 0x8001);
						phyMiiWrite(port_num, 0x52b5, 0x0012, 0x0002);
						phyMiiWrite(port_num, 0x52b5, 0x0010, 0x9782);
					}
					if ((mode==RELAY_LONG) && (port_num>=4)){
						phyMiiWrite(port_num, 0x0000, 0x0000, 0x1140);
					} else {
						phyMiiWrite(port_num, 0x0000, 0x0000, 0x0140);
					}
				}
			}
			break;
		default:
			break;
	}
	return 0;
}


/* 
 *  function name: phy_packet_gen
 *  description:   generate packet form MT7530 switch phy
 */
void phy_packet_gen(uint32 port_num, uint16 unit){
	phyMiiWrite(port_num, 0x0001, 0x0011, (unit-1));
	phyMiiWrite(port_num, 0x0001, 0x001a, 0x13e7);
	phyMiiWrite(port_num, 0x0001, 0x001d, 0x8842);
	phyMiiWrite(port_num, 0x0001, 0x001d, 0xc842);
}

/* 
 *  function name: MT7530_Data_Flow_Test
 *  description:   MT7530 SLT Data Flow Test Function
 */
int	MT7530_Data_Flow_Test(int link_speed)
{
	uint32 delay_ms=0, offset=0x100, port_num=0, ret=0;
	uint32 last_cnt[7], cnt[7], pkt_loss[7];
	uint16 pkt_unit=0;

	gswPbusWrite(0x10, 0x0000000d);
	gswPbusWrite(0x2404, 0x00ff0100);

	for (port_num=0; port_num<7; port_num++){
		last_cnt[port_num] = gswPbusRead((MIB_RX_PKT_64_CNT + (port_num * offset)));
	}

	Set_GswPhy_Speed(link_speed, RELAY_LPBK);

	for (port_num=0; port_num<5; port_num++){
		if (!Chk_Link_Status(port_num)){
			prom_printf("Port %d Link Time out\n", port_num);
			goto fail;
		}
	}
	
	prom_printf("Test Port 0~5 Data Flow\n");

	Set_Test_Param(link_speed, &pkt_unit, &delay_ms);

	for (port_num=0; port_num<5; port_num++){
		phy_packet_gen(port_num, pkt_unit);
	}

	delay1ms(delay_ms);

	for (port_num=0; port_num<6; port_num++){
		cnt[port_num] = gswPbusRead((MIB_RX_PKT_64_CNT + (port_num * offset)));
		cnt[port_num] -= last_cnt[port_num];
		pkt_loss[port_num] = test_pkt[link_speed] - cnt[port_num];
	}

	for (port_num=0; port_num<6; port_num++){
		prom_printf("port %d test cnt: %d\n", port_num, cnt[port_num]);
		prom_printf("port %d pkt loss: %d\n", port_num, pkt_loss[port_num]);
	}

	prom_printf("\n");
	for (port_num=0; port_num<5; port_num++){
		if (pkt_loss[port_num] >= pkt_loss_limit[link_speed]){
			prom_printf("port %d data flow test fail\n", port_num);
			goto fail;
		}
	}

	prom_printf("=========================================================\n\n");

	prom_printf("Test Port 6 Data Flow\n");
	// set port6 as mirror port
	gswPbusWrite(0x10, 0x0000000e);

	phy_packet_gen(0x0004, pkt_unit);
	delay1ms(delay_ms);

	cnt[6] = gswPbusRead((MIB_RX_PKT_64_CNT + (6 * offset)));
	cnt[6] -= last_cnt[6];
	pkt_loss[6] = test_pkt[link_speed] - cnt[6];

	prom_printf("port 6 test cnt: %d\n", cnt[6]);
	prom_printf("port 6 pkt loss: %d\n", pkt_loss[6]);

	if (pkt_loss[6] >= pkt_loss_limit[link_speed]){
		prom_printf("port 6 data flow test fail\n");
		goto fail;
	}

	prom_printf("=========================================================\n\n");
	return 0;

fail:

	prom_printf("=========================================================\n\n");
	return -1;
}

/* 
 *  function name: MT7530_Performance_Test
 *  description:   MT7530 SLT Performace Test Function
 */
int MT7530_Performance_Test(int link_speed)
{
	uint32 value, delay_ms=0, offset=0x100, port_num=0, ret=0;
	uint32 last_cnt[6], cnt[6], pkt_loss[6];
	uint16 pkt_unit=0;

	// set port5 as mirror port
	gswPbusWrite(0x10, 0x0000000d);
	// set port5 port matrix portmap: 0x01
	gswPbusWrite(0x2504, 0x00ff0100);

	// disable local port function
	value = gswPbusRead(0x0000000c);
	value |= (1<<7);
	gswPbusWrite(0x0000000c, value);

	for (port_num=0; port_num<6; port_num++){
		last_cnt[port_num] = gswPbusRead((MIB_RX_PKT_64_CNT + (port_num * offset)));
	}

	Set_GswPhy_Speed(link_speed, RELAY_LONG);
	
	for (port_num=0; port_num<5; port_num++){
		if (!Chk_Link_Status(port_num)){
			prom_printf("Port %d Link Time out\n", port_num);
			goto fail;
		}
	}

	prom_printf("Test Port 0~5 Performance\n");

	Set_Test_Param(link_speed, &pkt_unit, &delay_ms);
	for (port_num=0; port_num<5; port_num++){
		phy_packet_gen(port_num, pkt_unit);
	}

	delay1ms(delay_ms);

	for (port_num=0; port_num<5; port_num++){
		cnt[port_num] = gswPbusRead((MIB_RX_PKT_64_CNT + (port_num * offset)));
		cnt[port_num] -= last_cnt[port_num];
		pkt_loss[port_num] = test_pkt[link_speed] - cnt[port_num];
	}

	for (port_num=0; port_num<5; port_num++){
		prom_printf("port %d test cnt: %d\n", port_num, cnt[port_num]);
		prom_printf("port %d pkt loss: %d\n", port_num, pkt_loss[port_num]);
	}

	prom_printf("\n");

	for (port_num=0; port_num<5; port_num++){
		if (pkt_loss[port_num] > pkt_loss_limit[link_speed]){
			prom_printf("port %d performance test fail\n", port_num);
			cnt[port_num] = gswPbusRead((MIB_RX_PKT_64_CNT + (port_num * offset)));
			goto fail;
		}
	}

	prom_printf("=========================================================\n\n");
	return 0;

fail:

	prom_printf("=========================================================\n\n");
	return -1;
}

/* 
 *  function name: MT7530_Gphy_P5_Test
 *  description:   MT7530_Gphy_P5 Test Function
 */
int MT7530_P5_Gphy_Test(int link_speed, uint32 outPort, uint32 inPort)
{
	uint32 delay_ms=0, offset=0x100, ret=0;
	uint32 last_cnt, cnt, pkt_loss, value;
	uint16 pkt_unit=0;

	// change trap to software control
	value =	gswPbusRead(0x7804);
	value |= (1<<16);
	gswPbusWrite(0x7804, value);

	// enable port5
	value =	gswPbusRead(0x7804);
	value &= (~(1<<6));
	gswPbusWrite(0x7804, value);

	// connect port5 to GPHY0/GPHY4
	value =	gswPbusRead(0x7804);
	value &= (~(1<<13));
	gswPbusWrite(0x7804, value);

	// select PHY4 as port5's PHY
	value =	gswPbusRead(0x7804);
	if (outPort == 4)
		value |= (1<<20);
	else
		value &= (~(1<<20));
	gswPbusWrite(0x7804, value);
	
	// set port5 as mirror port
	gswPbusWrite(0x0010, 0x0010000d);
	// set outport port matrix portmap: 0xff
	gswPbusWrite((0x2004 + outPort*0x100), 0x00ff0000);
	// set inport port matrix portmap: 0x00
	gswPbusWrite((0x2004 + inPort*0x100), 0x00000000);
	// set port5 port matrix portmap: 0x10
	gswPbusWrite(0x2504, 0x00100000);

	last_cnt = gswPbusRead((MIB_RX_PKT_64_CNT + (outPort * offset)));

	Set_GswPhy_Speed(link_speed, RELAY_P0P4);
	
	if (!Chk_Link_Status(inPort)){
		prom_printf("Port %d Link Time out\n", inPort);
		goto fail;
	}

	if (!Chk_Link_Status(outPort)){
		prom_printf("Port %d Link Time out\n", outPort);
		goto fail;
	}

	prom_printf("Test Port P%d-P%d-P5 Traffic\n", outPort, inPort);

	Set_Test_Param(link_speed, &pkt_unit, &delay_ms);
	phy_packet_gen(outPort, pkt_unit);
	delay1ms(delay_ms);

	cnt = gswPbusRead((MIB_RX_PKT_64_CNT + (outPort * offset)));
	cnt -= last_cnt;
	pkt_loss = test_pkt[link_speed] - cnt;

	prom_printf("P%d-P%d-P5 test cnt: %d\n", outPort, inPort, cnt);
	if (cnt < pkt_loss_limit[link_speed]){
		prom_printf("P%d-P%d-P5 traffic test fail\n", outPort, inPort);
		prom_printf("P%d-P%d-P5 pkt loss: %d\n", outPort, inPort, pkt_loss);
		goto fail;
	}

	prom_printf("=========================================================\n\n");
	return 0;

fail:

	prom_printf("=========================================================\n\n");
	return -1;
}

/* 
 *  function name: MT7530_Speed_High
 *  description:   Switch MT7530 Switch Frequence to 500Mhz
 */
void MT7530_Speed_High()
{
	gswPbusWrite(0x7808, 0x00000000);
	// change GSW CLK to 500MHz
	gswPbusWrite(0x7804, 0x01017e8f);
	gswPbusWrite(0x7808, 0x00000001);

	// PLL bias on
	tcMiiBpw(0x0000, 0x000d, 0x001f);
	tcMiiBpw(0x0000, 0x000e, 0x0403);
	tcMiiBpw(0x0000, 0x000d, 0x401f);
	tcMiiBpw(0x0000, 0x000e, 0x1800);

	// Bias LPF en
	tcMiiBpw(0x0000, 0x000d, 0x001f);
	tcMiiBpw(0x0000, 0x000e, 0x0403);
	tcMiiBpw(0x0000, 0x000d, 0x401f);
	tcMiiBpw(0x0000, 0x000e, 0x1c00);

	// sys PLL en
	tcMiiBpw(0x0000, 0x000d, 0x001f);
	tcMiiBpw(0x0000, 0x000e, 0x0401);
	tcMiiBpw(0x0000, 0x000d, 0x401f);
	tcMiiBpw(0x0000, 0x000e, 0xc020);

	// LCDDDS PWDB
	tcMiiBpw(0x0000, 0x000d, 0x001f);
	tcMiiBpw(0x0000, 0x000e, 0x0406);
	tcMiiBpw(0x0000, 0x000d, 0x401f);
	tcMiiBpw(0x0000, 0x000e, 0xa030);

	// gsw_2x_clk
	tcMiiBpw(0x0000, 0x000d, 0x001f);
	tcMiiBpw(0x0000, 0x000e, 0x0410);
	tcMiiBpw(0x0000, 0x000d, 0x401f);
	tcMiiBpw(0x0000, 0x000e, 0x0003);

	gswPbusWrite(0x7830, 0x00000001);
}

/* 
 *  function name: MT7530_P6_Speed_Test
 *  description:   SLT P6 Speed Change Test Function
 */
int MT7530_P6_Speed_Test(int link_speed)
{
	uint32 value, offset=0x100, ret=0;
	uint32 last_cnt6, cnt6, pkt_loss6;

	gswPbusWrite(0x10, 0x0000000e);
	gswPbusWrite(0x2004, 0x00ff0100);

	Set_GswPhy_Speed(link_speed, RELAY_LPBK);

	if (!Chk_Link_Status(0)){
		prom_printf("Port 0 Link Time out\n");
		goto fail;
	}


	prom_printf("Test Port 6 Speed Function\n");

	prom_printf("Slow Test\n");
	last_cnt6 = gswPbusRead((MIB_RX_PKT_64_CNT + (6 * offset)));

	// packet genenrate from port 0
	phy_packet_gen(0x0000, 0xa);
	delay1ms(15);

	cnt6 = gswPbusRead((MIB_RX_PKT_64_CNT + (6 * offset)));
	cnt6 -= last_cnt6;
	pkt_loss6 = test_pkt[link_speed] - cnt6;

	prom_printf("P6 Speed test cnt: %d\n", cnt6);
	prom_printf("P6 Speed pkt loss: %d\n", pkt_loss6);
	if (pkt_loss6 > pkt_loss_limit[link_speed]){
		prom_printf("P6 Speed test fail\n");
		goto fail;
	}

	prom_printf("\nFast Test\n");
	MT7530_Speed_High();

	last_cnt6 = gswPbusRead((MIB_RX_PKT_64_CNT + (6 * offset)));

	// change to extend page, use mii by pass mode
	tcMiiBpw(0x0000, 0x001f, 0x0001);
	// set packet unit as 10
	tcMiiBpw(0x0000, 0x0011, 0x0009);
	// set packet number as 1000 packet unit
	tcMiiBpw(0x0000, 0x001a, 0x13e7);
	tcMiiBpw(0x0000, 0x001d, 0x8842);
	// packet genenrate from port 0
	tcMiiBpw(0x0000, 0x001d, 0xc842);
	delay1ms(15);

	cnt6 = gswPbusRead((MIB_RX_PKT_64_CNT + (6 * offset)));
	cnt6 -= last_cnt6;
	pkt_loss6 = test_pkt[link_speed] - cnt6;
	
	prom_printf("P6 Speed test cnt: %d\n", cnt6);
	prom_printf("P6 Speed pkt loss: %d\n", pkt_loss6);
	if (pkt_loss6 > pkt_loss_limit[link_speed]){
		prom_printf("P6 Speed test fail\n");
		goto fail;
	}

	return 0;

fail:

	return -1;
}


/* 
 *  function name: binClassify
 *  description:   check SLT test result and classify 
 */
void binClassify(int gpio_type){

	int id;

	if(gpio_set[id].gpio_op & 0x1){
		GPIO_HIGH(gpio_set[id].gpio_1);
	}
	else{
		GPIO_LOW(gpio_set[id].gpio_1);
	}

	if(gpio_set[id].gpio_op & 0x2){
		GPIO_HIGH(gpio_set[id].gpio_2);
	}
	else{
		GPIO_LOW(gpio_set[id].gpio_2);
	}

	if(gpio_set[id].gpio_op & 0x4){
		GPIO_HIGH(gpio_set[id].gpio_3);
	}
	else{
		GPIO_LOW(gpio_set[id].gpio_3);
	}

	prom_printf("Classify to Bin %d\r\n", gpio_type);

	GPIO_HIGH(26);
}


/* 
 *  function name: relaySetting
 *  description:   Set Relay mode of SLT Test board
 */
void relaySetting(int type){

	unsigned long gpio1;
	unsigned long gpio2;
	unsigned long gpio3;
	
	gpio1 = 2;
	gpio2 = 17;
	gpio3 = 21;
	
	switch(type)
	{
		case RELAY_LONG:
			prom_printf("GPIO RELAY LONG Mode\n");
			GPIO_LOW(gpio1);
			GPIO_LOW(gpio2);
			GPIO_LOW(gpio3);
			break;
		case RELAY_LPBK:
			prom_printf("GPIO RELAY LPBK Mode\n");
			GPIO_LOW(gpio1);
			GPIO_HIGH(gpio2);
			GPIO_HIGH(gpio3);
			break;
		case RELAY_OPEN:
			prom_printf("GPIO RELAY OPEN Mode\n");
			GPIO_HIGH(gpio1);
			GPIO_HIGH(gpio2);
			GPIO_LOW(gpio3);
			break;
		case RELAY_P0P4:
			prom_printf("GPIO RELAY P0P4 Mode\n");
			GPIO_HIGH(gpio1);
			GPIO_LOW(gpio2);
			GPIO_HIGH(gpio3);
			break;
		default:	
			prom_printf("Error GPIO RELAY Mode\n");
			break;
	}

	delay1ms(2000);
}

void MT7530_DC_offset_SNR_Test(int link_speed)
{
	uint32 value, port_num, snr1, snr2;
	uint32 overSumA, overSumB, overSumC, overSumD;

	relaySetting(RELAY_OPEN);

	Set_GswPhy_Speed(link_speed, RELAY_OPEN);
	
	delay1ms(3000);

	prom_printf("\nDc Offset Test:\n");

	/* read port 0 dc offset value */
	mtEMiiRegWrite(0x0000, 0x001f, 0x0015, 0x108b);
	value = mtEMiiRegRead(0x0000, 0x001f, 0x001a);
	prom_printf("Port 0 Dc Offset Value: %x\n", value);

	/* read port 1 dc offset value */
	mtEMiiRegWrite(0x0000, 0x001f, 0x0015, 0x308b);
	value = mtEMiiRegRead(0x0000, 0x001f, 0x001a);
	prom_printf("Port 1 Dc Offset Value: %x\n", value);

	/* read port 2 dc offset value */
	mtEMiiRegWrite(0x0000, 0x001f, 0x0015, 0x508b);
	value = mtEMiiRegRead(0x0000, 0x001f, 0x001a);
	prom_printf("Port 2 Dc Offset Value: %x\n", value);

	/* read port 3 dc offset value */
	mtEMiiRegWrite(0x0000, 0x001f, 0x0015, 0x708b);
	value = mtEMiiRegRead(0x0000, 0x001f, 0x001a);
	prom_printf("Port 3 Dc Offset Value: %x\n", value);

	/* read port 4 dc offset value */
	mtEMiiRegWrite(0x0000, 0x001f, 0x0015, 0x908b);
	value = mtEMiiRegRead(0x0000, 0x001f, 0x001a);
	prom_printf("Port 4 Dc Offset Value: %x\n", value);

	relaySetting(RELAY_LPBK);
	
	prom_printf("\nSNR Test:\n");

	/* read snr value */
	for (port_num=0; port_num<6; port_num++){
		snr1 = mtEMiiRegRead(port_num, 0x001e, 0x009b);	
		snr2 = mtEMiiRegRead(port_num, 0x001e, 0x009c);	
		prom_printf("Port %d SNR1: %x, SNR2: %x\n", port_num, snr1, snr2);
	}

	prom_printf("\nSlicer Err Over Sum Check\n");

	/* read error counter */
	for (port_num=0; port_num<6; port_num++){
		overSumA = mtEMiiRegRead(port_num, 0x001e, 0x009e);	
		overSumB = mtEMiiRegRead(port_num, 0x001e, 0x009f);	
		overSumC = mtEMiiRegRead(port_num, 0x001e, 0x00a0);	
		overSumD = mtEMiiRegRead(port_num, 0x001e, 0x00a1);	
		prom_printf("Port %d overSum A: %x, B: %x, C: %x, D: %x\n", port_num, overSumA, overSumB, overSumC, overSumD);
	}
}


/* 
 *  function name: Chk_Link_Status
 *  description:   check MT7530 switch phy link status
 *                 1. if timeout -> return fail
 *                 2. if link up -> return success
 */
int Chk_Link_Status(uint32 port_num)
{
	uint32 delay = 0, value = 0;

	do {
		value = phyMiiRead(port_num, 0x0000, 0x0001);
		if (value & 0x4){
			return 1;
		}

		delay1ms(1000);
		delay++;
		prom_printf("port %d wait %d second\n", port_num, delay);
		if (delay>10){
			break;
		}
	} while(1);

	return 0;
}


/* 
 *  function name: MT7530_SLT_Test
 *  description:   MT7530 SLT Main Program
 */
int MT7530_SLT_Test()
{
	uint32 pbus_data, pbus_addr, port_num, ret=0, value;

	prom_printf("\n\nMT7530_SLT_Test Release 1.1 Version\n");

	test_pkt[LINK_SPEED_10M] = 5000;
	test_pkt[LINK_SPEED_100M] = 10000;
	test_pkt[LINK_SPEED_1000M] = 10000;

	pkt_loss_limit[LINK_SPEED_10M] = 1;
	pkt_loss_limit[LINK_SPEED_100M] = 7;
	pkt_loss_limit[LINK_SPEED_1000M] = 7;

	spin_lock_init(global_phy_lock);
	spin_lock_init(global_pbus_lock);

/******************** MT7530 EEEPROM Test ***********************/
	ret = MT7530_EEEPROM_Test();
	if (ret==-1){
		prom_printf("MT7530_EEEPROM_Test Fail\n\n\n");
		binClassify(GPIO_BIN6);
		goto fail;
	} else {
		prom_printf("MT7530_EEEPROM_Test Success\n\n\n");
	}


/***************** MT7530 DC Offset & SNR Test *******************/
	// Reset PHY
	for (port_num=0; port_num<7; port_num++){
		phyMiiWrite(port_num, 0x0000, 0x0000, 0x8000);
	}
	// Reset Switch
	gswPbusWrite(0X7000, 0X80000003);
	macSetGSW();

	prom_printf("In Giga Mode\n");
	prom_printf("MT7530_DC_offset_SNR_Test\n\n");
	MT7530_DC_offset_SNR_Test(LINK_SPEED_1000M);

	for (port_num=0; port_num<7; port_num++){
		phyMiiWrite(port_num, 0x0000, 0x0000, 0x8000);
	}
	// Reset Switch
	gswPbusWrite(0X7000, 0X80000003);
	macSetGSW();

/******************** MT7530 Data Flow Test ***********************/
	relaySetting(RELAY_LPBK);
	prom_printf("\n\nMT7530_Data_Flow_Test\n\n");

	/*** Link Speed: 10M ***/
	MT7530_Data_Flow_Test(LINK_SPEED_10M);
	if (ret==-1){
		prom_printf("Link Speed 10M: Data Flow Test Fail\n\n");
		binClassify(GPIO_BIN3);
		goto fail;
	} else {
		prom_printf("Link Speed 10M: Data Flow Test Success\n\n");
	}

	/*** Link Speed: 100M ***/
	ret = MT7530_Data_Flow_Test(LINK_SPEED_100M);
	if (ret==-1){
		prom_printf("Link Speed 100M: Data Flow Test Fail\n\n");
		binClassify(GPIO_BIN4);
		goto fail;
	} else {
		prom_printf("Link Speed 100M: Data Flow Test Success\n\n");
	}

	/*** Link Speed: 1000M ***/
	ret = MT7530_Data_Flow_Test(LINK_SPEED_1000M);
	if (ret==-1){
		prom_printf("Link Speed 1000M: Data Flow Test Fail\n\n");
		binClassify(GPIO_BIN5);
		goto fail;
	} else {
		prom_printf("Link Speed 1000M: Data Flow Test Success\n\n");
	}

/******************** MT7530 Performance Test ***********************/
	prom_printf("\n\nMT7530_Performance_Test\n\n");

	// Reset PHY
	for (port_num=0; port_num<7; port_num++){
		phyMiiWrite(port_num, 0x0000, 0x0000, 0x8000);
	}

	// Reset Switch
	gswPbusWrite(0X7000, 0X80000003);
	macSetGSW();

	relaySetting(RELAY_LONG);
	
	/*** Link Speed: 10M ***/
	ret = MT7530_Performance_Test(LINK_SPEED_10M);
	if (ret==-1){
		prom_printf("Link Speed 10M: Performance Test Fail\n\n");
		binClassify(GPIO_BIN3);
		goto fail;

	} else {
		prom_printf("Link Speed 10M: Performance Test Success\n\n");
	}

	/*** Link Speed: 100M ***/
	ret = MT7530_Performance_Test(LINK_SPEED_100M);
	if (ret==-1){
		prom_printf("Link Speed 100M: Performance Test Fail\n\n");
		binClassify(GPIO_BIN4);
		goto fail;
	} else {
		prom_printf("Link Speed 100M: Performance Test Success\n\n");
	}

	/*** Link Speed: 1000M ***/
	ret = MT7530_Performance_Test(LINK_SPEED_1000M);
	if (ret==-1){
		prom_printf("Link Speed 1000M: Performance Test Fail\n\n");
		binClassify(GPIO_BIN5);
		goto fail;
	} else {
		prom_printf("Link Speed 1000M: Performance Test Success\n\n");
	}

/***************** MT7530 P0-P4-P5 Data Flow Test *******************/
	prom_printf("\n\nMT7530_P0_P4_P5_Test\n\n");
	// Reset PHY
	for (port_num=0; port_num<7; port_num++){
		phyMiiWrite(port_num, 0x0000, 0x0000, 0x8000);
	}
	// Reset Switch
	gswPbusWrite(0X7000, 0X80000003);
	macSetGSW();

	relaySetting(RELAY_P0P4);


	/*** link Speed: 1000M ***/
	ret = MT7530_P5_Gphy_Test(LINK_SPEED_1000M, 0x0000, 0x0004);
	if (ret==-1){
		prom_printf("Link Speed 1000M: P0-P4-P5 Traffic Test Fail\n\n");
		binClassify(GPIO_BIN5);
		goto fail;
	} else {
		prom_printf("Link Speed 1000M: P0-P4-P5 Traffic Test Success\n\n");
	}

/***************** MT7530 P0-P4-P5 Data Flow Test *******************/
	prom_printf("\n\nMT7530_P4_P0_P5_Test\n\n");
	// Reset PHY
	for (port_num=0; port_num<7; port_num++){
		phyMiiWrite(port_num, 0x0000, 0x0000, 0x8000);
	}
	// Reset Switch
	gswPbusWrite(0X7000, 0X80000003);
	macSetGSW();

	/*** Link Speed: 1000M ***/
	ret = MT7530_P5_Gphy_Test(LINK_SPEED_1000M, 0x0004, 0x0000);
	if (ret==-1){
		prom_printf("Link Speed 1000M: P5-P4-P0 Traffic Test Fail\n\n");
		binClassify(GPIO_BIN5);
		goto fail;
	} else {
		prom_printf("Link Speed 1000M: P5-P4-P0 Traffic Test Success\n\n");
	}

/***************** MT7530 P6 Speed Change Test *******************/
	prom_printf("\n\nMT7530_P6_Speed_Test\n\n");
	// Reset PHY
	for (port_num=0; port_num<7; port_num++){
		phyMiiWrite(port_num, 0x0000, 0x0000, 0x8000);
	}
	// Reset Switch
	gswPbusWrite(0X7000, 0X80000003);
	macSetGSW();

	relaySetting(RELAY_LPBK);
	ret = MT7530_P6_Speed_Test(LINK_SPEED_1000M);

	if (ret==-1){
		prom_printf("P6 Speed Test Fail\n\n");
		binClassify(GPIO_BIN7);
		goto fail;
	} else {
		prom_printf("P6 Speed Test Success\n\n");
	}

	binClassify(GPIO_BIN1);
	return 0;

fail:
	return -1;
}
#endif
