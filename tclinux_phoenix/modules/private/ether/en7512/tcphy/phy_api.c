/***************************************************************
Copyright Statement:

This software/firmware and related documentation (¡°EcoNet Software¡±) 
are protected under relevant copyright laws. The information contained herein 
is confidential and proprietary to EcoNet (HK) Limited (¡°EcoNet¡±) and/or 
its licensors. Without the prior written permission of EcoNet and/or its licensors, 
any reproduction, modification, use or disclosure of EcoNet Software, and 
information contained herein, in whole or in part, shall be strictly prohibited.

EcoNet (HK) Limited  EcoNet. ALL RIGHTS RESERVED.

BY OPENING OR USING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY 
ACKNOWLEDGES AND AGREES THAT THE SOFTWARE/FIRMWARE AND ITS 
DOCUMENTATIONS (¡°ECONET SOFTWARE¡±) RECEIVED FROM ECONET 
AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON AN ¡°AS IS¡± 
BASIS ONLY. ECONET EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, 
WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, 
OR NON-INFRINGEMENT. NOR DOES ECONET PROVIDE ANY WARRANTY 
WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTIES WHICH 
MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE ECONET SOFTWARE. 
RECEIVER AGREES TO LOOK ONLY TO SUCH THIRD PARTIES FOR ANY AND ALL 
WARRANTY CLAIMS RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES 
THAT IT IS RECEIVER¡¯S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD 
PARTY ALL PROPER LICENSES CONTAINED IN ECONET SOFTWARE.

ECONET SHALL NOT BE RESPONSIBLE FOR ANY ECONET SOFTWARE RELEASES 
MADE TO RECEIVER¡¯S SPECIFICATION OR CONFORMING TO A PARTICULAR 
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
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/dma-mapping.h>
#include <linux/mii.h>
#include <asm/io.h>
#include <asm/tc3162/tc3162.h>
#include <asm/tc3162/ledcetrl.h>
#include <asm/tc3162/TCIfSetQuery_os.h>
#include <linux/if_vlan.h>

    
#ifdef TCPHY_SUPPORT
#include <asm/tc3162/cmdparse.h>
#include "../tcphy/tcswitch.h"
#endif
#ifdef TCSUPPORT_AUTOBENCH
#include "../auto_bench/autobench.h"
#endif
        
#ifdef TCSUPPORT_RA_HWNAT
#include <linux/foe_hook.h>
#endif
        
#include "tcconsole.h"
#include "../eth_lan.h"
#include "../eth_proc.h"
#include "phy_api.h"


#ifdef MT7510_DMA_DSCP_CACHE
#include <asm/r4kcache.h>
#endif
        
#ifdef LOOPBACK_SUPPORT
#include "../fe_verify.h"
#endif
        

/************************************************************************
*                  D E F I N E S   &   C O N S T A N T S
*************************************************************************
*/

/************************************************************************
*                  M A C R O S
*************************************************************************
*/

/************************************************************************
*                  D A T A   T Y P E S
*************************************************************************
*/

/************************************************************************
*                  E X T E R N A L   D A T A   D E C L A R A T I O N S
*************************************************************************
*/
extern spinlock_t phy_lock;
extern macAdapter_t *mac_wan_p;
extern macAdapter_t *mac_p;


/************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
*************************************************************************
*/

/************************************************************************
*                  P U B L I C   D A T A
*************************************************************************
*/

/************************************************************************
*                  P R I V A T E   D A T A
*************************************************************************
*/

/************************************************************************
*                  F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/


/************************************************************************
    Funtcion:
    Description:    
    Calls:
    Called by:      
    Input:
    Output:
    Return:
    Others:     
************************************************************************/
void tcMiiStationWrite(uint32 phy_addr, uint32 phy_reg, uint32 phy_data)
{
	uint32 reg;
	uint32 cnt=10000;
	u32 GSW_CFG_PIAC_addr;

//	GSW_CFG_PIAC_addr = (isMT7530 || isMT7530ext) ? (GSW_CFG_BASE + 0x001C) : GSW_CFG_PIAC;
	GSW_CFG_PIAC_addr = (isMT7530) ? (GSW_CFG_BASE + 0x001C) : GSW_CFG_PIAC;

	spin_lock_bh(&phy_lock);
	do {
		reg=read_reg_word (GSW_CFG_PIAC_addr);
		cnt--;
	} while ((reg & PHY_ACS_ST) && (cnt != 0));

	reg = PHY_ACS_ST | (MDIO_ST_START << MDIO_ST_SHIFT) | 
    	    (MDIO_CMD_WRITE<<MDIO_CMD_SHIFT) | 
    		(phy_addr << MDIO_PHY_ADDR_SHIFT) | 
    		(phy_reg << MDIO_REG_ADDR_SHIFT) | 
    		(phy_data & MDIO_RW_DATA);
	write_reg_word (GSW_CFG_PIAC_addr, reg);

	cnt = 10000;
	do {
		//pause(1);
		reg=read_reg_word (GSW_CFG_PIAC_addr);
		cnt--;
	} while ((reg & PHY_ACS_ST) && (cnt != 0));
	spin_unlock_bh(&phy_lock);
	if (cnt==0) 
		printk("EER: tcMiiStationWrite timeout!\n");
}
EXPORT_SYMBOL(tcMiiStationWrite);

uint32 tcMiiStationRead(uint32 phy_addr, uint32 phy_reg)
{
	uint32 reg;
	uint32 cnt=10000;
	u32 GSW_CFG_PIAC_addr;

//	GSW_CFG_PIAC_addr = (isMT7530 || isMT7530ext) ? (GSW_CFG_BASE + 0x001C) : GSW_CFG_PIAC;
	GSW_CFG_PIAC_addr = (isMT7530) ? (GSW_CFG_BASE + 0x001C) : GSW_CFG_PIAC;

	spin_lock_bh(&phy_lock);
	do {
		reg=read_reg_word (GSW_CFG_PIAC_addr);
		cnt--;
	} while ((reg & PHY_ACS_ST) && (cnt != 0));

	reg = PHY_ACS_ST | (MDIO_ST_START << MDIO_ST_SHIFT) | 
    	    (MDIO_CMD_READ<<MDIO_CMD_SHIFT) | 
    		(phy_addr << MDIO_PHY_ADDR_SHIFT) | 
    		(phy_reg << MDIO_REG_ADDR_SHIFT);
	write_reg_word (GSW_CFG_PIAC_addr, reg);

	cnt = 10000;
	do {
		//pause(1);
		reg=read_reg_word (GSW_CFG_PIAC_addr);
		cnt--;
	} while ((reg & PHY_ACS_ST) && (cnt != 0));
	reg = reg & MDIO_RW_DATA;
	spin_unlock_bh(&phy_lock);

	if (cnt == 0) 
		printk("EER: tcMiiStationRead timeout!\n");

	return reg;
}
EXPORT_SYMBOL(tcMiiStationRead);


/************************************************************************
    Funtcion:       miiStationWrite
    Description:    
    Calls:
    Called by:      phy driver
    Input:
    Output:
    Return:
    Others:     
************************************************************************/
void miiStationWrite(macAdapter_t *mac_p, uint32 phy_reg, uint32 phy_data)
{
	tcMiiStationWrite(mac_p->enetPhyAddr, phy_reg, phy_data);
}


/************************************************************************
    Funtcion:       miiStationRead
    Description:    
    Calls:
    Called by:      phy driver
    Input:
    Output:
    Return:
    Others:     
************************************************************************/
uint32 miiStationRead(macAdapter_t *mac_p, uint32 phy_reg)
{
	return tcMiiStationRead(mac_p->enetPhyAddr, phy_reg);
}



int mdio_read(struct net_device *dev, int phy_id, int reg_num)
{
	return tcMiiStationRead(phy_id, reg_num);
}

void mdio_write(struct net_device *dev, int phy_id, int reg_num, int val)
{
	tcMiiStationWrite(phy_id, reg_num, val);
}

/************************************************************************
    Funtcion:       mdio_cl22_read
    Description:    
    Calls:
    Called by:      trgmii setting
    Input:
    Output:
    Return:
    Others:     
************************************************************************/
uint32 mdio_cl22_read(uint32 port_num, uint32 dev_num, uint32 reg_num)
{

    const uint16 MMD_Control_register=0xD;
    const uint16 MMD_addr_data_register=0xE;
  	const uint16 page_reg=31;
	
	uint32 value=0;

	tcMiiStationWrite(port_num, page_reg, 0x00); //switch to main page
	tcMiiStationWrite(port_num, MMD_Control_register, (0<<14)+dev_num);
	tcMiiStationWrite(port_num, MMD_addr_data_register, reg_num);
	tcMiiStationWrite(port_num, MMD_Control_register, (1<<14)+dev_num);
	value = tcMiiStationRead(port_num, MMD_addr_data_register);
//	printf("* doPhyMMDRead_CL22=>phyaddr=%d,  dev_addr=%d, data_addr=0x%04lX , value=0x%04lX\r\n", port_num, dev_num, reg_num, value);
}

/************************************************************************
    Funtcion:       mdio_cl22_write
    Description:    
    Calls:
    Called by:      trgmii setting
    Input:
    Output:
    Return:
    Others:     
************************************************************************/
void mdio_cl22_write(uint32 port_num,uint32 dev_num,uint32 reg_num,uint32 reg_data)
{
    const uint16 MMD_Control_register=0xD;
    const uint16 MMD_addr_data_register=0xE;
  	const uint16 page_reg=31;


	tcMiiStationWrite(port_num, page_reg, 0x00); //switch to main page
	tcMiiStationWrite(port_num, MMD_Control_register, (0<<14)+dev_num);
	tcMiiStationWrite(port_num, MMD_addr_data_register, reg_num);
	tcMiiStationWrite(port_num, MMD_Control_register, (1<<14)+dev_num);
	tcMiiStationWrite(port_num, MMD_addr_data_register, reg_data);
//	printf("* doPhyMMDWrite_CL22=> phyaddr=%d, dev_addr=%d, data_addr=0x%04lX , value=0x%04lX\r\n", port_num, dev_num, reg_num, reg_data);
}



#if 0


void tcAdmMiiStationWrite(uint32 admReg, uint32 miiData)
{
	uint32 phyaddr;
	uint32 reg;

	phyaddr = admReg >> 5;
	reg = admReg & 0x1f;

	tcMiiStationWrite(phyaddr, reg, miiData);
}

uint32 tcAdmMiiStationRead(uint32 admReg)
{
	uint32 phyaddr;
	uint32 reg;
	
	phyaddr = admReg >> 5;
	reg = admReg & 0x1f;

	return tcMiiStationRead(phyaddr, reg);
}
#if 1
//#ifdef TCSUPPORT_MT7510_FE
//#define SUPPORT_CLAUSE45_API
#ifdef SUPPORT_CLAUSE45_API
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
**      	Here_20090625
**____________________________________________________________________________
*/
int tcMiiExtStationFillAddr(uint32 portAddr, uint32 devAddr, uint32 regAddr)
{
	uint32 value, cnt;
	u32 GSW_CFG_PIAC_addr;

//	GSW_CFG_PIAC_addr = (isMT7530 || isMT7530ext) ? (GSW_CFG_BASE + 0x001C) : GSW_CFG_PIAC;
	GSW_CFG_PIAC_addr = (isMT7530) ? (GSW_CFG_BASE + 0x001C) : GSW_CFG_PIAC;
	portAddr &= 0x1F;
	devAddr &= 0x1F;

	spin_lock_bh(&phy_lock);
	cnt = 10000;
	do {
		//pause(1);
		value = read_reg_word(GSW_CFG_PIAC_addr);
		cnt --;
	} while ((value & PHY_ACS_ST) && (cnt != 0));

	value = PHY_ACS_ST | (MDIO_CL45_ST_START << MDIO_ST_SHIFT) | 
	        (MDIO_CL45_CMD_ADDR << MDIO_CMD_SHIFT) |
	        (portAddr << MDIO_PHY_ADDR_SHIFT) | 
	        (devAddr << MDIO_REG_ADDR_SHIFT) |
            (regAddr & MDIO_RW_DATA);
	write_reg_word (GSW_CFG_PIAC_addr, value);

	cnt = 10000;
	do {
		//pause(1);
		value = read_reg_word(GSW_CFG_PIAC_addr);
		cnt --;
	} while ((value & PHY_ACS_ST) && (cnt != 0));
	spin_unlock_bh(&phy_lock);

	if (cnt==0){
		printk("EER: tcMiiExtStationFillAddr timeout!\n");
		return -1;
	}
	return 0;
}/*end tcMiiExtStationFillAddr*/
EXPORT_SYMBOL(tcMiiExtStationFillAddr);

/*_____________________________________________________________________________
**      function name: tcMiiExtStationWrite
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
**      	Here_20090625
**____________________________________________________________________________
*/
void tcMiiExtStationWrite(uint32 portAddr, uint32 devAddr, uint32 regAddr, uint32 miiData)
{
	uint32 value, cnt;
	u32 GSW_CFG_PIAC_addr;

//	GSW_CFG_PIAC_addr = (isMT7530 || isMT7530ext) ? (GSW_CFG_BASE + 0x001C) : GSW_CFG_PIAC;
	GSW_CFG_PIAC_addr = (isMT7530) ? (GSW_CFG_BASE + 0x001C) : GSW_CFG_PIAC;
	portAddr &= 0x1F;
	devAddr &= 0x1F;

	tcMiiExtStationFillAddr(devAddr, portAddr, regAddr);

	spin_lock_bh(&phy_lock);
	cnt = 10000;
	do {
		//pause(1);
		value = read_reg_word(GSW_CFG_PIAC_addr);
		cnt --;
	} while ((value & PHY_ACS_ST) && (cnt != 0));

	value = PHY_ACS_ST | (MDIO_CL45_ST_START << MDIO_ST_SHIFT) | 
    	    (MDIO_CL45_CMD_WRITE << MDIO_CMD_SHIFT) |
    		(portAddr << MDIO_PHY_ADDR_SHIFT) | 
    		(devAddr << MDIO_REG_ADDR_SHIFT) |
    		(miiData & MDIO_RW_DATA);
	write_reg_word(GSW_CFG_PIAC_addr, value);

	cnt = 10000;
	do {
		//pause(1);
		value = read_reg_word(GSW_CFG_PIAC_addr);
		cnt --;
	} while ((value & PHY_ACS_ST) && (cnt != 0));
	spin_unlock_bh(&phy_lock);

	if (cnt==0){
		printk("EER: tcMiiStationWrite timeout!\n");
	}
}
EXPORT_SYMBOL(tcMiiExtStationWrite);

/*_____________________________________________________________________________
**      function name: tcMiiExtStationRead
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
**      	Here_20090625
**____________________________________________________________________________
*/
uint32 tcMiiExtStationRead(uint32 portAddr, uint32 devAddr, uint32 regAddr, uint8 op)
{
	uint32 value, cnt;
	u32 GSW_CFG_PIAC_addr;

//	GSW_CFG_PIAC_addr = (isMT7530 || isMT7530ext) ? (GSW_CFG_BASE + 0x001C) : GSW_CFG_PIAC;
	GSW_CFG_PIAC_addr = (isMT7530) ? (GSW_CFG_BASE + 0x001C) : GSW_CFG_PIAC;
	portAddr &= 0x1F;
	devAddr &= 0x1F;

	if (op != POST_READ){
		/*POST READ command only to fill the address once, the address will be
		increased once automatically.*/
		tcMiiExtStationFillAddr(devAddr, portAddr, regAddr);
	}

	spin_lock_bh(&phy_lock);
	cnt = 10000;
	do {
		//pause(1);
		value = read_reg_word(GSW_CFG_PIAC_addr);
		cnt --;
	} while ((value & PHY_ACS_ST) && (cnt != 0));

	if(op==POST_READ){
		value = PHY_ACS_ST | (MDIO_CL45_ST_START << MDIO_ST_SHIFT) | 
		        (MDIO_CL45_CMD_READ << MDIO_CMD_SHIFT) |
			    (portAddr << MDIO_PHY_ADDR_SHIFT) | 
			    (devAddr << MDIO_REG_ADDR_SHIFT);

	}
	else{/*Normal Read*/
		value = PHY_ACS_ST | (MDIO_CL45_ST_START << MDIO_ST_SHIFT) | 
		        (MDIO_CL45_CMD_READ << MDIO_CMD_SHIFT) |
			    (portAddr << MDIO_PHY_ADDR_SHIFT) | 
			    (devAddr << MDIO_REG_ADDR_SHIFT);
	}
	write_reg_word (GSW_CFG_PIAC_addr, value);

	cnt = 10000;
	do {
		//pause(1);
		value = read_reg_word(GSW_CFG_PIAC_addr);
		cnt --;
	} while ((value & PHY_ACS_ST) && (cnt != 0));
	spin_unlock_bh(&phy_lock);

	if (cnt==0){
		printk("EER: tcMiiExtStationRead timeout!\n");
	}

	return (value & MDIO_RW_DATA);
}/*end tcMiiExtStationRead*/
EXPORT_SYMBOL(tcMiiExtStationRead);

void tcMiiStationCL45Address(uint32 portAddr, uint32 devAddr, uint32 regAddr)
{
	uint32 value, cnt;
	u32 GSW_CFG_PIAC_addr;

//	GSW_CFG_PIAC_addr = (isMT7530 || isMT7530ext) ? (GSW_CFG_BASE + 0x001C) : GSW_CFG_PIAC;
	GSW_CFG_PIAC_addr = (isMT7530) ? (GSW_CFG_BASE + 0x001C) : GSW_CFG_PIAC;
	portAddr &= 0x1F;
	devAddr &= 0x1F;

	spin_lock_bh(&phy_lock);
	cnt = 10000;
	do {
		//pause(1);
		value = read_reg_word(GSW_CFG_PIAC_addr);
		cnt --;
	} while ((value & PHY_ACS_ST) && (cnt != 0));

	value = PHY_ACS_ST | (MDIO_CL45_ST_START << MDIO_ST_SHIFT) | 
    	    (MDIO_CL45_CMD_ADDR << MDIO_CMD_SHIFT) |
    		(portAddr << MDIO_PHY_ADDR_SHIFT) | 
    		(devAddr << MDIO_REG_ADDR_SHIFT) |
    		(regAddr & MDIO_RW_DATA);
	write_reg_word (GSW_CFG_PIAC_addr, value);

	cnt = 10000;
	do {
		//pause(1);
		value = read_reg_word(GSW_CFG_PIAC_addr);
		cnt --;
	} while ((value & PHY_ACS_ST) && (cnt != 0));
	spin_unlock_bh(&phy_lock);
	if (cnt == 0) printk("ERR: %s() timeout!\n", __FUNCTION__);
	#if (0)
	#ifdef TCPHY_4PORT
	/*TC2206 switch IC can't be direct to do PHY reset, we must
	 * avoid ESD software patch be trigger.
	 */
	refillPhyDefVal(enetPhyAddr, phyReg, miiData);
	#endif
	#endif
}
EXPORT_SYMBOL(tcMiiStationCL45Address);

void tcMiiStationCL45Write(uint32 portAddr, uint32 devAddr, uint32 miiData)
{
	uint32 value, cnt;
	u32 GSW_CFG_PIAC_addr;

//	GSW_CFG_PIAC_addr = (isMT7530 || isMT7530ext) ? (GSW_CFG_BASE + 0x001C) : GSW_CFG_PIAC;
	GSW_CFG_PIAC_addr = (isMT7530) ? (GSW_CFG_BASE + 0x001C) : GSW_CFG_PIAC;
	portAddr &= 0x1F;
	devAddr &= 0x1F;

	spin_lock_bh(&phy_lock);
	cnt = 10000;
	do {
		//pause(1);
		value = read_reg_word(GSW_CFG_PIAC_addr);
		cnt --;
	} while ((value & PHY_ACS_ST) && (cnt != 0));

	value = PHY_ACS_ST | (MDIO_CL45_ST_START << MDIO_ST_SHIFT) | 
    	    (MDIO_CL45_CMD_WRITE << MDIO_CMD_SHIFT) |
    		(portAddr << MDIO_PHY_ADDR_SHIFT) | 
    		(devAddr << MDIO_REG_ADDR_SHIFT) |
    		(miiData & MDIO_RW_DATA);
	write_reg_word(GSW_CFG_PIAC_addr, value);

	cnt = 10000;
	do {
		//pause(1);
		value = read_reg_word(GSW_CFG_PIAC_addr);
		cnt--;
	} while ((value & PHY_ACS_ST) && (cnt != 0));
	spin_unlock_bh(&phy_lock);
	if (cnt == 0) 
	    printk("ERR: %s() timeout!\n", __FUNCTION__);

}
EXPORT_SYMBOL(tcMiiStationCL45Write);

uint32 tcMiiStationCL45Read(uint32 portAddr, uint32 devAddr)
{
	uint32 value, cnt;
	u32 GSW_CFG_PIAC_addr;

//	GSW_CFG_PIAC_addr = (isMT7530 || isMT7530ext) ? (GSW_CFG_BASE + 0x001C) : GSW_CFG_PIAC;
	GSW_CFG_PIAC_addr = (isMT7530) ? (GSW_CFG_BASE + 0x001C) : GSW_CFG_PIAC;
	portAddr &= 0x1F;
	devAddr &= 0x1F;

	spin_lock_bh(&phy_lock);
	cnt = 10000;
	do {
		//pause(1);
		value = read_reg_word(GSW_CFG_PIAC_addr);
		cnt --;
	} while ((value & PHY_ACS_ST) && (cnt != 0));

	value = PHY_ACS_ST | (MDIO_CL45_ST_START << MDIO_ST_SHIFT) | 
	        (MDIO_CL45_CMD_READ << MDIO_CMD_SHIFT) |
		    (portAddr << MDIO_PHY_ADDR_SHIFT) | 
		    (devAddr << MDIO_REG_ADDR_SHIFT);
	write_reg_word(GSW_CFG_PIAC_addr, value);

	cnt = 10000;
	do {
		//pause(1);
		value = read_reg_word(GSW_CFG_PIAC_addr);
		cnt --;
	} while ((value & PHY_ACS_ST) && (cnt != 0));
	spin_unlock_bh(&phy_lock);
	if (cnt == 0) printk("ERR: %s() timeout!\n", __FUNCTION__);

	return (value & MDIO_RW_DATA);
}
EXPORT_SYMBOL(tcMiiStationCL45Read);

uint32 tcMiiStationCL45PostReadIncAddr(uint32 portAddr, uint32 devAddr)
{
	uint32 value, cnt;
	u32 GSW_CFG_PIAC_addr;

//	GSW_CFG_PIAC_addr = (isMT7530 || isMT7530ext) ? (GSW_CFG_BASE + 0x001C) : GSW_CFG_PIAC;
	GSW_CFG_PIAC_addr = (isMT7530) ? (GSW_CFG_BASE + 0x001C) : GSW_CFG_PIAC;
	portAddr &= 0x1F;
	devAddr &= 0x1F;

	spin_lock_bh(&phy_lock);
	cnt = 10000;
	do {
		//pause(1);
		value = read_reg_word(GSW_CFG_PIAC_addr);
		cnt --;
	} while ((value & PHY_ACS_ST) && (cnt != 0));

	value = PHY_ACS_ST | (MDIO_CL45_ST_START << MDIO_ST_SHIFT) | 
	        (MDIO_CL45_CMD_POSTREAD_INCADDR << MDIO_CMD_SHIFT) |
		    (portAddr << MDIO_PHY_ADDR_SHIFT) | 
		    (devAddr << MDIO_REG_ADDR_SHIFT);
	write_reg_word(GSW_CFG_PIAC_addr, value);

	cnt = 10000;
	do {
		//pause(1);
		value = read_reg_word(GSW_CFG_PIAC_addr);
		cnt --;
	} while ((value & PHY_ACS_ST) && (cnt != 0));
	spin_unlock_bh(&phy_lock);
	if (cnt == 0) printk("ERR: %s() timeout!\n", __FUNCTION__);

	return (value & MDIO_RW_DATA);
}
EXPORT_SYMBOL(tcMiiStationCL45PostReadIncAddr);

void miiExtStationFillAddr(macAdapter_t *mac_p, uint32 devAddr, uint32 regAddr)
{
	tcMiiExtStationFillAddr(mac_p->enetPhyAddr, devAddr, regAddr);
}

void miiExtStationWrite(macAdapter_t *mac_p, uint32 devAddr, 
    uint32 regAddr, uint32 miiData)
{
	tcMiiExtStationWrite(mac_p->enetPhyAddr, devAddr, regAddr, miiData);
}

uint32 miiExtStationRead(macAdapter_t *mac_p, uint32 devAddr, 
    uint32 regAddr, uint8 op)
{
	return tcMiiExtStationRead(mac_p->enetPhyAddr, devAddr, regAddr, op);
}
#endif 	//#ifdef SUPPORT_CLAUSE45_API

//#ifdef SUPPORT_CLAUSE45_API
//static spinlock_t phy_lock_clause45;

int tcMiiExtStationFillAddr_ext(uint32 portAddr, uint32 devAddr, uint32 regAddr)
{
	return (0);
}
EXPORT_SYMBOL(tcMiiExtStationFillAddr_ext);


EXPORT_SYMBOL(tcMiiExtStationWrite_ext);


EXPORT_SYMBOL(tcMiiExtStationRead_ext);


EXPORT_SYMBOL(tcMiiStationWrite_ext);


EXPORT_SYMBOL(tcMiiStationRead_ext);

//#endif //#ifdef SUPPORT_CLAUSE45_API

#endif  //#ifdef TCSUPPORT_MT7510_FE
#endif

#ifdef TCSUPPORT_INIC_HOST
struct net_device *get_tc3262_gmac_dev()
{
	return lan_dev;
}
EXPORT_SYMBOL(get_tc3262_gmac_dev);
#endif




//** MTK120625 start,
static spinlock_t pbus_lock;
/* frank modify for rt62806 */
uint32
gswPbusRead(uint32 pbus_addr)
{
	uint32 pbus_data;

	uint32 phyaddr;
	uint32 reg;
	uint32 value;

	spin_lock_bh(&pbus_lock);

	phyaddr = 31;
	// 1. write high-bit page address
	reg = 31;
	value = (pbus_addr >> 6);
  	tcMiiStationWrite(phyaddr, reg, value);
	//mdelay(5);
	//DBG_PRINTF("1. miiw phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);
	//printk("1. miiw phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);

	// 2. read low DWord
	reg = (pbus_addr>>2) & 0x000f;
	value = tcMiiStationRead(phyaddr, reg);
	//mdelay(5);
	//DBG_PRINTF("2. miir phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);
	//printk("2. miir phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);
	pbus_data = value;

	// 3. read high DWord
	reg = 16;
		value = tcMiiStationRead(phyaddr, reg);
	//mdelay(5);
	//DBG_PRINTF("3. miir phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);
	//printk("3. miir phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);

	pbus_data = (pbus_data) | (value<<16);

	//DBG_PRINTF("# pbus read addr=0x%04x data=0x%04x_%04x\r\n", (pbus_addr&0xfffc), (pbus_data>>16), (pbus_data&0xffff));
	//printk("# pbus read addr=0x%04x data=0x%04x_%04x\r\n", (pbus_addr&0xfffc), (pbus_data>>16), (pbus_data&0xffff));
//	printk("gswPbusRead read data:\n");

//	printk("pbus_data: %x\n", pbus_data);

	spin_unlock_bh(&pbus_lock);
	return pbus_data;
} /* end frank modify for rt62806 */

/* frank modify for rt62806 */
int
gswPbusWrite(uint32 pbus_addr, uint32 pbus_data)
{
	uint32 phyaddr;
	uint32 reg;
	uint32 value;

	spin_lock_bh(&pbus_lock);

	phyaddr = 31;

	// 1. write high-bit page address
	reg = 31;
	value = (pbus_addr >> 6);
	tcMiiStationWrite(phyaddr, reg, value);
	//mdelay(5);

	//printk("1. miiw phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);
	//DBG_PRINTF("1. miiw phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);

	// 2. write low DWord
	reg = (pbus_addr>>2) & 0x000f;
	value = pbus_data & 0xffff;
	tcMiiStationWrite(phyaddr, reg, value);
	//mdelay(5);
	//printk("2. miiw phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);
	//DBG_PRINTF("2. miiw phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);

	// 3. write high DWord
	reg = 16;
	value = (pbus_data>>16) & 0xffff;
	tcMiiStationWrite(phyaddr, reg, value);
	//mdelay(5);
	//printk("3. miiw phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);
	//DBG_PRINTF("3. miiw phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);

	//DBG_PRINTF("# pbus write addr=0x%04x data=0x%04x_%04x\r\n", (pbus_addr&0xfffc), (pbus_data>>16),(pbus_data&0xffff));
	spin_unlock_bh(&pbus_lock);
  	return 0;
} /* end frank modify for rt62806 */


/* frank modify for rt62806 */
uint32
gswPbusRead2(uint32 pbus_addr)
{
	uint32 pbus_data;

	uint32 phyaddr;
	uint32 reg;
	uint32 value;

	spin_lock_bh(&pbus_lock);

	phyaddr = 23;
	// 1. write high-bit page address
	reg = 31;
	value = (pbus_addr >> 6);
  	tcMiiStationWrite(phyaddr, reg, value);
	//mdelay(5);
	//DBG_PRINTF("1. miiw phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);
	//printk("1. miiw phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);

	// 2. read low DWord
	reg = (pbus_addr>>2) & 0x000f;
	value = tcMiiStationRead(phyaddr, reg);
	//mdelay(5);
	//DBG_PRINTF("2. miir phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);
	//printk("2. miir phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);
	pbus_data = value;

	// 3. read high DWord
	reg = 16;
		value = tcMiiStationRead(phyaddr, reg);
	//mdelay(5);
	//DBG_PRINTF("3. miir phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);
	//printk("3. miir phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);

	pbus_data = (pbus_data) | (value<<16);

	//DBG_PRINTF("# pbus read addr=0x%04x data=0x%04x_%04x\r\n", (pbus_addr&0xfffc), (pbus_data>>16), (pbus_data&0xffff));
	//printk("# pbus read addr=0x%04x data=0x%04x_%04x\r\n", (pbus_addr&0xfffc), (pbus_data>>16), (pbus_data&0xffff));
//	printk("gswPbusRead read data:\n");

//	printk("pbus_data: %x\n", pbus_data);

	spin_unlock_bh(&pbus_lock);
	return pbus_data;
} /* end frank modify for rt62806 */


/* frank modify for rt62806 */
int
gswPbusWrite2(uint32 pbus_addr, uint32 pbus_data)
{
	uint32 phyaddr;
	uint32 reg;
	uint32 value;

	spin_lock_bh(&pbus_lock);

	phyaddr = 23;

	// 1. write high-bit page address
	reg = 31;
	value = (pbus_addr >> 6);
	tcMiiStationWrite(phyaddr, reg, value);
	//mdelay(5);

	//printk("1. miiw phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);
	//DBG_PRINTF("1. miiw phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);

	// 2. write low DWord
	reg = (pbus_addr>>2) & 0x000f;
	value = pbus_data & 0xffff;
	tcMiiStationWrite(phyaddr, reg, value);
	//mdelay(5);
	//printk("2. miiw phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);
	//DBG_PRINTF("2. miiw phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);

	// 3. write high DWord
	reg = 16;
	value = (pbus_data>>16) & 0xffff;
	tcMiiStationWrite(phyaddr, reg, value);
	//mdelay(5);
	//printk("3. miiw phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);
	//DBG_PRINTF("3. miiw phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);

	//DBG_PRINTF("# pbus write addr=0x%04x data=0x%04x_%04x\r\n", (pbus_addr&0xfffc), (pbus_data>>16),(pbus_data&0xffff));
	spin_unlock_bh(&pbus_lock);
  	return 0;
} /* end frank modify for rt62806 */


//PHY2 read/write
uint32 gswPmiRead(uint32 phy_addr, uint32 phy_reg)
{
	uint32 pbus_addr;
	uint32 pbus_data;
	uint32 phy_data;
	uint32 phy_acs_st;
//	uint32 max_wait_cnt = 1000;

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
} /* end frank modify for rt62806 */

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
//  DBG_PRINTF(" pbus write addr=0x%04x data=0x%08x\r\n", pbus_addr, pbus_data);

	return 0;
} /* end frank modify for rt62806 */


#if 0


int32 macSearchPhyAddr(macAdapter_t *mac_p)
{
	uint32 miiReg = 0;

#if 0
	mac_p->enetPhyAddr = 0;
	for ( mac_p->enetPhyAddr = 0; mac_p->enetPhyAddr < 32; mac_p->enetPhyAddr++ ) {
#endif
		miiReg = miiStationRead(mac_p, MII_PHYSID1);
		if (miiReg == 0)
			miiReg = miiStationRead(mac_p, MII_PHYSID2);
		if (macPhyLookUp(mac_p, miiReg)) {
			return 0;
		}
#if 0
	}
	mac_p->enetPhyAddr = 0x00000000;
#endif
	return -1;
}


int macSetUpPhy(macAdapter_t *mac_p)
{
	/* ----- Hardware reset Ehernet phy chip, this address is defined by h/w engineer ----- */
	ledTurnOn(LED_LAN_RESET);
	pause(100);
	/* ----- Wait for hardware reset completed ----- */
	ledTurnOff(LED_LAN_RESET);
	pause(600);

	/*OSBNB00031891: Add for Multi EPHY setting*/
	mac_p->enetPhyAddr = 0;
	
	if (isFPGA) {//xyyou add for 7505 FPGA phy addr1 and addr2 reset 		
		
		tcMiiStationWrite(1, MII_BMCR, BMCR_RESET); 	
		pause(10);			
		tcMiiStationWrite(1, MII_BMCR, BMCR_ANRESTART | BMCR_ANENABLE); 
		pause(10);			
		tcMiiStationWrite(2, MII_BMCR, BMCR_RESET); 	
		pause(10);			
		tcMiiStationWrite(2, MII_BMCR, BMCR_ANRESTART | BMCR_ANENABLE); 


	}else{	
	for ( mac_p->enetPhyAddr = 0; mac_p->enetPhyAddr < 32; mac_p->enetPhyAddr++ ) {
		if(!macSearchPhyAddr(mac_p)){
			macPhyReset();
			mac_p->enetPhyAddr+= (tcPhyPortNumGet()-1);
		}
	}
	}
#ifdef TCSUPPORT_CPU_MT7505//allen_20140303
	mac_p->enetPhyAddr = 8;
#else
	mac_p->enetPhyAddr = 0;
#endif
	/*OSBNB00031891: Add for Multi EPHY setting over*/
	pause(100);

	/* Detect 4-port switch or single port switch */
	/* detect if IP175C */
	if ((tcMiiStationRead(4, 2) == 0x243) && (tcMiiStationRead(4, 3) == 0xd80)) {
		swicVendor = SWIC_IP175C;
	}
	/* detect if RTL8305 */	
	if ((tcMiiStationRead(4, 2) == 0x1c) && (tcMiiStationRead(4, 3) == 0xc852)) {
		if ((tcMiiStationRead(6, 2) == 0x1c) && (tcMiiStationRead(6, 3) == 0xc852) ){
			swicVendor = SWIC_RTL8306SD;
			/*Let CPU Port Link up*/
			tcMiiStationWrite(5, 22,tcMiiStationRead(5,22)|0x8000);
			tcMiiStationWrite(6, 22,tcMiiStationRead(6,22)|0x8000);
		}
		else{
			swicVendor = SWIC_RTL8305;
		}
	}

	/* detect if MARVEL 88E6060 */	
	if (((tcMiiStationRead(4, 2) == 0x141) || (tcMiiStationRead(20, 2) == 0x141)) && 
	    ((tcMiiStationRead(4, 3) == 0xc87) || (tcMiiStationRead(20, 3) == 0xc87))) {
		swicVendor = SWIC_MARVEL6060;
	}

	if (isRT63365)
		swicVendor = SWIC_RT63365;		    

	if (!(isRT63368)){
		/* detect if MT7530, wplin added 20120712*/
		if (isMT7530 || isMT7530ext) {
			swicVendor = SWIC_MT7530;
		//	printk("swicVendor=SWIC_MT7530\n");
		} /* end wplin added 20120712*/
	}
	return 0;
}


void macPhyReset(void)
{
	#ifdef TCPHY_SUPPORT
	if(getTcPhyFlag()){
		tcPhyInit(mac_p);
	}
	else{  
	#endif	
		miiStationWrite(mac_p, MII_BMCR, BMCR_RESET);
		pause(10);
		miiStationWrite(mac_p, MII_BMCR, BMCR_ANRESTART | BMCR_ANENABLE);
	#ifdef TCPHY_SUPPORT
	}
	#endif
}

#endif

