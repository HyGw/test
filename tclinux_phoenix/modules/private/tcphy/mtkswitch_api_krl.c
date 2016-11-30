/*
** $Id: $
*/
/************************************************************************
 *
 *	Copyright (C) 2012 Mediatek Inc.
 *	All Rights Reserved.
 *
 * Mediatek Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of Trendchip Technologies, Co. and shall
 * not be reproduced, copied, disclosed, or used in whole or in part
 * for any reason without the prior express written permission of
 * Mediatek Inc.
 *
 *************************************************************************/
/*
** $Log$
**
 */

#ifdef TCSUPPORT_MT7530_SWITCH_API

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
//#include <linux/spinlock.h>
//#include <linux/interrupt.h>
//#include <linux/signal.h>
//#include <linux/sched.h>
//#include <linux/mm.h>
//#include <linux/init.h>
//#include <linux/timer.h>
#include <linux/delay.h>
//#include <linux/proc_fs.h>
//#include <linux/dma-mapping.h>
#include <linux/mii.h>
//#include <linux/if_vlan.h>
//#include <asm/spram.h>
//#include <asm/io.h>
#include <asm/tc3162/tc3162.h>
//#include <asm/tc3162/ledcetrl.h>
//#include <asm/tc3162/TCIfSetQuery_os.h>
#include "../../version/tcversion.h"


#ifdef TCPHY_SUPPORT
#include <asm/tc3162/cmdparse.h>
//#include "../tcphy/tcetherphy.h"
//#include "../tcphy/tcswitch.h"
#endif

//#include "../tcphy/tcconsole.h"

//#include "../tc3262/tc3262gmac.h"

#include "mtkswitch_api.h"
#include <modules/eth_global_def.h>



/************************************************************************
*                          C O N S T A N T S
*************************************************************************
*/

/* define  */
#define GSW_BASE		0xBFB58000

#if defined(TCSUPPORT_CUC) || defined(TCSUPPORT_CT_PON_CY)
#define ENABLE_LAN_MASK 0xFFFFF7FF
#endif

/************************************************************************
*                            M A C R O S
*************************************************************************
*/

/************************************************************************
*                         D A T A   T Y P E S
*************************************************************************
*/

extern int g_port_reverse;


/************************************************************************
*              F U N C T I O N   D E C L A R A T I O N S
*************************************************************************
*/
extern uint32 gswPbusRead(uint32 pbus_addr);
extern int gswPbusWrite(uint32 pbus_addr, uint32 pbus_data);
extern uint32 gswPmiRead(uint32 phy_addr, uint32 phy_reg);
extern uint32 gswPmiWrite(uint32 phy_addr, uint32 phy_reg, uint32 phy_data);
extern uint32 tcMiiStationRead(uint32 enetPhyAddr, uint32 phyReg);
extern void tcMiiStationWrite(uint32 enetPhyAddr, uint32 phyReg, uint32 miiData);

/************************************************************************
*                        P U B L I C   D A T A
*************************************************************************
*/

/************************************************************************
*                      E X T E R N A L   D A T A
*************************************************************************
*/

/************************************************************************
*                       P R I V A T E   D A T A
*************************************************************************
*/
static u8 macMT7530_APIDbgDumpEn = 0;

// This table Used by macMT7530SetAutoDetection() and macMT7530GetAutoDetection()
/*               *Speed*    *Duplex*
**        0x00 - Auto;      Auto
**        0x01 - 10 only;   Full duplex only
**        0x02 - 100 only;  Full duplex only
**        0x03 - 1000 only; Full duplex only
**        0x04 - Auto;      Full duplex only
**        0x10 - 10 only;   Auto
**        0x11 - 10 only;   Half duplex only
**        0x12 - 100 only;  Half duplex only
**        0x13 - 1000 only; Half duplex only
**        0x14 - Auto;      Half duplex only
**        0x20 - 1000 only; Auto
**        0x30 - 100 only;  Auto
*/
const static mt7530_switch_MIILinkType MIIMediaOption[]={
//	{index_param, MII_BMCR_value(0x00), MII_ADVERTISE_value(0x04), MII_CTRL1000_value(0x09)}
	{0x00, (BMCR_ANENABLE), (ADVERTISE_CSMA | ADVERTISE_10FULL | ADVERTISE_10HALF | ADVERTISE_100FULL | ADVERTISE_100HALF), (ADVERTISE_1000FULL | ADVERTISE_1000HALF)},
//	{0x00, (BMCR_ANENABLE), 0, 0},	// if ADVERTISE_CSMA = 0, means all?
	{0x01, (BMCR_ANENABLE), (ADVERTISE_CSMA | ADVERTISE_10FULL), 0},
	{0x02, (BMCR_ANENABLE), (ADVERTISE_CSMA | ADVERTISE_100FULL), 0},
	{0x03, (BMCR_ANENABLE), (ADVERTISE_CSMA), (ADVERTISE_1000FULL)},
	{0x04, (BMCR_ANENABLE), (ADVERTISE_CSMA | ADVERTISE_10FULL | ADVERTISE_100FULL), (ADVERTISE_1000FULL)},
	{0x10, (BMCR_ANENABLE), (ADVERTISE_CSMA | ADVERTISE_10FULL | ADVERTISE_10HALF), 0},
	{0x11, (BMCR_ANENABLE), (ADVERTISE_CSMA | ADVERTISE_10HALF), 0},
	{0x12, (BMCR_ANENABLE), (ADVERTISE_CSMA | ADVERTISE_100HALF), 0},
	{0x13, (BMCR_ANENABLE), (ADVERTISE_CSMA), (ADVERTISE_1000HALF)},
	{0x14, (BMCR_ANENABLE), (ADVERTISE_CSMA | ADVERTISE_10HALF | ADVERTISE_100HALF), (ADVERTISE_1000HALF)},
	{0x20, (BMCR_ANENABLE), (ADVERTISE_CSMA), (ADVERTISE_1000FULL | ADVERTISE_1000HALF)},
	{0x30, (BMCR_ANENABLE), (ADVERTISE_CSMA | ADVERTISE_100FULL | ADVERTISE_100HALF), 0},
};
#define MIIMediaOption_Entries (sizeof(MIIMediaOption) / sizeof(MIIMediaOption[0]))

#if (0)
// MII_BMCR(0x00) : Basic mode control register.
#define BMCR_SPEED100           0x2000  /* Select 100Mbps              */
#define BMCR_SPEED1000		0x0040  /* MSB of Speed (1000)         */
#define BMCR_FULLDPLX           0x0100  /* Full duplex                 */
#define BMCR_ANENABLE           0x1000  /* Enable auto negotiation     */
#define BMCR_ANRESTART          0x0200  /* Auto negotiation restart    */
#define BMCR_RESET              0x8000  /* Reset the DP83840           */
// MII_ADVERTISE(0x04) : Advertisement control register.
#define ADVERTISE_CSMA          0x0001  /* Only selector supported     */
#define ADVERTISE_10FULL        0x0040  /* Try for 10mbps full-duplex  */
#define ADVERTISE_10HALF        0x0020  /* Try for 10mbps half-duplex  */
#define ADVERTISE_100FULL       0x0100  /* Try for 100mbps full-duplex */
#define ADVERTISE_100HALF       0x0080  /* Try for 100mbps half-duplex */
#define ADVERTISE_1000XFULL     0x0020  /* Try for 1000BASE-X full-duplex */
#define ADVERTISE_1000XHALF     0x0040  /* Try for 1000BASE-X half-duplex */
// MII_CTRL1000(0x09) : 1000BASE-T Control register
#define ADVERTISE_1000FULL      0x0200  /* Advertise 1000BASE-T full duplex */
#define ADVERTISE_1000HALF      0x0100  /* Advertise 1000BASE-T half duplex */
#endif


// This table Used by macMT7530GetConfigurationStatus()
/*
**         0x01 : 10BaseT full duplex
**         0x02 : 100BaseT full duplex
**         0x03 : Gigabit Ethernet full duplex
**         0x11 : 10BaseT half duplex
**         0x12 : 100BaseT half duplex
**         0x13 : Gigabit Ethernet half duplex
*/
#define mt7530MediaStatus_Entries (6)
const static u8 mt7530MediaStatus[mt7530MediaStatus_Entries][2] = {
	//user-defined param, 0x3n08[3:0]
	{0x01, ((0 << 2) + (1 << 1) + 1)},
	{0x02, ((1 << 2) + (1 << 1) + 1)},
	{0x03, ((2 << 2) + (1 << 1) + 1)},
	{0x11, ((0 << 2) + (0 << 1) + 1)},
	{0x12, ((1 << 2) + (0 << 1) + 1)},
	{0x13, ((2 << 2) + (0 << 1) + 1)},
};
//#define mt7530MediaStatus_Entries (sizeof(mt7530MediaStatus) / sizeof(mt7530MediaStatus[0]))

#if (0)
const static u16 MIIMediaStatus[][3] ={
	{0x01, (BMSR_10FULL), 0},
	{0x02, (BMSR_100FULL), 0},
	{0x03, 0, (LPA_1000FULL)},
	{0x11, (BMSR_10HALF), 0},
	{0x12, (BMSR_100HALF), 0},
	{0x13, 0, (LPA_1000HALF)},
};
#define MIIMediaStatus_Entries (sizeof(MIIMediaStatus) / sizeof(MIIMediaStatus[0]))

#if (0)
#define MII_BMSR            0x01        /* Basic mode status register  */
#define MII_STAT1000        0x0a        /* 1000BASE-T status           */
/* MII_BMSR (0x01) : Basic mode status register. */
#define BMSR_10HALF             0x0800  /* Can do 10mbps, half-duplex  */
#define BMSR_10FULL             0x1000  /* Can do 10mbps, full-duplex  */
#define BMSR_100HALF            0x2000  /* Can do 100mbps, half-duplex */
#define BMSR_100FULL            0x4000  /* Can do 100mbps, full-duplex */
/* MII_STAT1000 (0x0a) : 1000BASE-T Status register */
#define LPA_1000FULL            0x0800  /* Link partner 1000BASE-T full duplex */
#define LPA_1000HALF            0x0400  /* Link partner 1000BASE-T half duplex */
#endif
#endif

// This table Used by macMT7530GetConfigurationStatus()
/*
**         0 : DCE or MDI-X.
**         1 : DTE or MDI.
**         2 : Automatic selection. (default)
*/
#define mt7530DTEDCEInd_Entries (4)
const static u8 mt7530DTEDCEInd[mt7530DTEDCEInd_Entries][2] = {
	//user-defined param, PHY register tReg05[4:3]
	{0, (3 << 3)},
	{1, (2 << 3)},
	{2, (0 << 3)},
	{2, (1 << 3)},
};

static u8 fcStatus = 0;

/************************************************************************
*        F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/

#define read_reg_word(reg) 		VPint(reg)
#define write_reg_word(reg, wdata) 	VPint(reg)=wdata

/*___________________________________________________________________
**      function name:
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/06/01
**_______________________________________________________________
*/

/*___________________________________________________________________
**      function name:
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/07/19
**_______________________________________________________________
*/
#if (1)
u32 macMT7530ReadReg(u32 gsw_reg)
{
	#if defined(JUDGE_SWITCH_SCENARIO_BY_751020_SUBMODEL)
	if (DefaultUseExtMT7530)
	{
		return (gswPbusRead(gsw_reg));
	}
	else
	{
		return (read_reg_word(GSW_BASE + gsw_reg));
	}
	#elif defined(DEFAULT_USE_EXT_SWIC)
		return (gswPbusRead(gsw_reg));
	#else
		return (read_reg_word(GSW_BASE + gsw_reg));
	#endif
}

int macMT7530WriteReg(u32 gsw_reg, u32 gsw_data)
{
	#if defined(JUDGE_SWITCH_SCENARIO_BY_751020_SUBMODEL)
	if (DefaultUseExtMT7530)
	{
		return (gswPbusWrite(gsw_reg, gsw_data));
	}
	else
	{
		return (write_reg_word((GSW_BASE + gsw_reg), gsw_data));
	}
	#elif defined(DEFAULT_USE_EXT_SWIC)
		return (gswPbusWrite(gsw_reg, gsw_data));
	#else
		return (write_reg_word((GSW_BASE + gsw_reg), gsw_data));
	#endif
}

u32 macMT7530PHYReadReg(u32 phy_addr, u32 phy_reg)
{
	#ifdef EXT_SWITCH_PHY_CONNECT_INT_MDIO
		#if defined(JUDGE_SWITCH_SCENARIO_BY_751020_SUBMODEL)
	if (DefaultUseExtMT7530)
	{
		phy_addr += EXT_SWITCH_PHY_ADDR_OFFSET;
	}
		#elif defined(DEFAULT_USE_EXT_SWIC)
	phy_addr += EXT_SWITCH_PHY_ADDR_OFFSET;
		#else
		#endif
	return (tcMiiStationRead(phy_addr, phy_reg));
	#else
		#if defined(JUDGE_SWITCH_SCENARIO_BY_751020_SUBMODEL)
	if (DefaultUseExtMT7530)
	{
		return (tcMiiStationRead(phy_addr, phy_reg));
//		return (gswPmiRead(phy_addr, phy_reg));
	}
	else
	{
		return (tcMiiStationRead(phy_addr, phy_reg));
	}
		#elif defined(DEFAULT_USE_EXT_SWIC)
	return (gswPmiRead(phy_addr, phy_reg));
		#else
	return (tcMiiStationRead(phy_addr, phy_reg));
		#endif
	#endif
}

u32 macMT7530PHYWriteReg(u32 phy_addr, u32 phy_reg, u32 phy_data)
{
	#ifdef EXT_SWITCH_PHY_CONNECT_INT_MDIO
		#if defined(JUDGE_SWITCH_SCENARIO_BY_751020_SUBMODEL)
	if (DefaultUseExtMT7530)
	{
		phy_addr += EXT_SWITCH_PHY_ADDR_OFFSET;
	}
		#elif defined(DEFAULT_USE_EXT_SWIC)
	phy_addr += EXT_SWITCH_PHY_ADDR_OFFSET;
		#else
		#endif
	tcMiiStationWrite(phy_addr, phy_reg, phy_data);
	#else
		#if defined(JUDGE_SWITCH_SCENARIO_BY_751020_SUBMODEL)
	if (DefaultUseExtMT7530)
	{
		tcMiiStationWrite(phy_addr, phy_reg, phy_data);
//		return (gswPmiWrite(phy_addr, phy_reg, phy_data));
	}
	else
	{
		tcMiiStationWrite(phy_addr, phy_reg, phy_data);
	}
		#elif defined(DEFAULT_USE_EXT_SWIC)
	return (gswPmiWrite(phy_addr, phy_reg, phy_data));
		#else
	tcMiiStationWrite(phy_addr, phy_reg, phy_data);
		#endif
	#endif
		return (0);
}
#else
#define macMT7530ReadReg(switch_no, gsw_reg) ((gswPbusRead(gsw_reg)))
#define macMT7530WriteReg(witch_no, gsw_reg, gsw_data) (gswPbusWrite(gsw_reg, gsw_data)))
#define macMT7530PHYReadReg(switch_no, phy_addr, phy_reg) (gswPmiRead(phy_addr, phy_reg)))
#define macMT7530PHYWriteReg(switch_no, phy_addr, phy_reg, phy_data) (gswPmiWrite(phy_addr, phy_reg, phy_data)))
#endif

/*___________________________________________________________________
**      function name: macMT7530GetBrglearningInd
**      descriptions:
**         get port bridge learning status (disable/enable)
**
**      corresponding register:
**         0x2P0C[4] SA_DIS
**
**      parameters:
**         u8 port
**
**      global:
**
**      return:
**         0:disable, 1:enable
**         -1: failure
**
**      call:
**
**      revision:
**      1. WPLin 2012/06/22
**_______________________________________________________________
*/
int macMT7530GetBrglearningInd(u8 port)
{
	u32 reg, value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);
	reg = 0x200C + (0x100 * port);
	value = macMT7530ReadReg(reg);
	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%08X) : %08X\n", __FUNCTION__, port, reg, value);
	value = (value >> 4) & 0x01;
	return value;
}

int macMT7530SetBrglearningInd(u8 port, u8 Ind)
{
	u32 reg, value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);
	reg = 0x200C + (0x100 * port);
	value = ((macMT7530ReadReg(reg) & ~(1 << 4))) | ((Ind ? 1 : 0) << 4);
	macMT7530WriteReg(reg, value);
	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%08X) : %08X\n", __FUNCTION__, port, reg, value);
	return (0);
}

/*___________________________________________________________________
**      function name: macMT7530GetPortBrgInd
**      descriptions:
**         get bridging between UNI ports status (disable/enable)
**
**      corresponding register:
**         0x3P00[14] MAC_TX_EN_Pn
**         0x3P00[13] MAC_RX_EN_Pn
**
**      parameters:
**         u8 port
**
**      global:
**
**      return:
**         0:disable, 1:enable
**         -1: failure
**
**      call:
**
**      revision:
**      1. WPLin 2012/06/22
**_______________________________________________________________
*/
int macMT7530GetPortBrgInd(u8 port, u8 *Ind)
{
	u32 reg, value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);
	reg = 0x2004 + (0x100 * port);
	value = macMT7530ReadReg(reg);
	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%08X) : %08X\n", __FUNCTION__, port, reg, value);

	*Ind = (value >> 16) & 0xff;
	return 0;
}

int macMT7530SetPortBrgInd(u8 port, u8 Ind)
{
	u32 reg, value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);
	reg = 0x2004 + (0x100 * port);

	value = macMT7530ReadReg(reg) & 0xff00fffc | (Ind << 16);
	
	macMT7530WriteReg(reg, value);
	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%08X) : %08X\n", __FUNCTION__, port, reg, value);
	return (0);
}

#ifdef TCSUPPORT_PON_VLAN
EXPORT_SYMBOL(macMT7530GetPortBrgInd);
EXPORT_SYMBOL(macMT7530SetPortBrgInd);
#endif

/*___________________________________________________________________
**      function name: macMT7530GetDiscardUnknownMacInd
**      descriptions:
**         get discard unknown Mac address status (disable/enable)
**
**      corresponding register:
**         0x0010[31:24] BC_FFP
**         0x0010[23:16] MC_FFP
**         0x0010[15:8]  UC_FFP
**
**      parameters:
**         u8 port
**
**      global:
**
**      return:
**         0:Discard, 1:Not Discard
**         -1: failure
**
**      call:
**
**      revision:
**      1. WPLin 2012/06/25
**_______________________________________________________________
*/
int macMT7530GetDiscardUnknownMacInd(u8 port)
{
	u32 reg, value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);
	reg = 0x0010;
	value = macMT7530ReadReg(reg);
	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%08X) : %08X\n", __FUNCTION__, port, reg, value);
	value = (((value >> port) & 0x01010100) == 0x01010100) ? 1 : 0;
	return value;
}

int macMT7530SetDiscardUnknownMacInd(u8 port, u8 Ind)
{
	u32 reg, value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);
	reg = 0x0010;
	value = (macMT7530ReadReg(reg) & ~(0x01010100 << port)) | ((Ind ? 0x01010100 : 0) << port);
	macMT7530WriteReg(reg, value);
	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%08X) : %08X\n", __FUNCTION__, port, reg, value);
	return (0);
}

/*___________________________________________________________________
**      function name: macMT7530GetAgeTime
**      descriptions:
**         get address table age time (in seconds)
**
**      corresponding register:
**         0x00A0[19:12] AGE_CNT, default=0x95
**         0x00A0[11:0]  AGE_UNIT, default=1
**         --> (AGE_CNT+1)*(AGE_UNIT+1) seconds
**
**      parameters:
**         u32 *MaxAgeTime
**
**      global:
**
**      return:
**         0: ok
**         -1: failure
**
**         u32 *outptr_MaxAgeTime: MaxAgeTime (in seconds)
**
**      call:
**
**      revision:
**      1. WPLin 2012/06/25
**_______________________________________________________________
*/
int macMT7530GetAgeTime(u32 *outptr_MaxAgeTime)
{
	u32 reg, value, age_cnt, age_unit;

	reg = 0x00A0;
	value = macMT7530ReadReg(reg);
	if (macMT7530_APIDbgDumpEn) printk("%s() (%08X) : %08X\n", __FUNCTION__, reg, value);
	age_cnt = 1 + ((value >> 12) & 0x00FF);
	age_unit = 1 + (value & 0x0FFF);
	(*outptr_MaxAgeTime) = age_cnt * age_unit;
	return value;
}

int macMT7530SetAgeTime(u32 MaxAgeTime)
{
	u32 reg, value, age_cnt, age_unit;
	int i;
	int ret_val=0;

	if ((MaxAgeTime > (1 << 20)) || (MaxAgeTime == 0))
	{
		ret_val = -1;
	}
	else
	{
		reg = 0x00A0;
		value = macMT7530ReadReg(reg);
		age_unit = 1 + (value & 0x0FFF);
		age_cnt = MaxAgeTime / age_unit;
		// try to keep previous age_unit

		if ((age_cnt > (1 << 8)) || (age_cnt < 1))	// if unit too small or too large
		{
			for (i = 20; i > 0; i --)	// find leading first nonzero bit
			{
				if ((MaxAgeTime >> i) & 1) break;
			}
			age_unit = (i > 8) ? (1 << (i - 8)) : 1;
			age_cnt = MaxAgeTime / age_unit;
		}
	}
	if (!ret_val)
	{
		// (AGE_CNT+1)*(AGE_UNIT+1) seconds
		age_cnt = (age_cnt) ? ((age_cnt - 1) & 0x00FF) : 0;
		age_unit = (age_unit) ? ((age_unit - 1) & 0x0FFF) : 0;
		value = (value & ~0x001FFFFF) + (age_cnt << 12) + (age_unit);	// also clear AGE_DIS=0
		macMT7530WriteReg(reg, value);
		if (macMT7530_APIDbgDumpEn) printk("%s() (%08X) : %08X\n", __FUNCTION__, reg, value);
	}

	return ret_val;
}

/*___________________________________________________________________
**      function name: macMT7530GetPortMac
**      descriptions:
**         get port mac address
**
**      corresponding register:
**         0x0080, 0x0084, 0x0088, 0x008C
**
**      parameters:
**         u8 port
**         u8 *PortMac
**
**      global:
**
**      return:
**         0: ok
**         -1: failure
**
**         u8 *PortMac
**
**      call:
**
**      revision:
**      1. WPLin 2012/06/26
**_______________________________________________________________
*/
int macMT7530GetPortMac(u8 port, u8 *outptr_PortMac)	// "u8 portMac[6];
{
	u32 value;
	int ret_val=0, loop_timeout;

	if (port >= MT7530_TOTAL_PORTS) return (-1);
	loop_timeout = 1000000;
	while (macMT7530ReadReg(REG_ATC_ADDR) & REG_ATC_BUSY_MASK)	// wait until not busy
	{
		if (--loop_timeout == 0)
		{
			ret_val = -1;
			break;
		}
	}

	// trigger 1st search
	value = (1 << REG_ATC_BUSY_OFFT) + (0 << REG_ATC_AC_MAT_OFFT) + (4 << REG_ATC_AC_CMD_OFFT);	// 3'b100 : Start search command (reset to 1st entry)
	macMT7530WriteReg(REG_ATC_ADDR, value);
	loop_timeout = 1000000;
	while (macMT7530ReadReg(REG_ATC_ADDR) & REG_ATC_BUSY_MASK)	// wait until not busy
	{
		if (--loop_timeout == 0)
		{
			ret_val = -1;
			break;
		}
	}

	while (1)
	{
		value = macMT7530ReadReg(REG_ATC_ADDR);
		if (!(value & REG_AT_SRCH_HIT_MASK) || (value & REG_AT_SRCH_END_MASK))
		{
			ret_val = -1;

			// printk("...Address Table end...\n", value);
			break;
		}

		// read Portmap
		value = (macMT7530ReadReg(REG_ATRD_ADDR) & REG_ATWD_PORT_MASK) >> REG_ATWD_PORT_OFFT;

		if (value & (1 << port))
		{
			value = macMT7530ReadReg(REG_TSRA1_ADDR);
			outptr_PortMac[0] = (value >> 24) & 0xFF;
			outptr_PortMac[1] = (value >> 16) & 0xFF;
			outptr_PortMac[2] = (value >>  8) & 0xFF;
			outptr_PortMac[3] = (value      ) & 0xFF;

			value = macMT7530ReadReg(REG_TSRA2_ADDR);
			outptr_PortMac[4] = (value >> 24) & 0xFF;
			outptr_PortMac[5] = (value >> 16) & 0xFF;

			break;
		}

		// if not found, trigger next search
		value = (1 << REG_ATC_BUSY_OFFT) + (0 << REG_ATC_AC_MAT_OFFT) + (5 << REG_ATC_AC_CMD_OFFT);	// 3'b101 : Next search command (next entry)
		macMT7530WriteReg(REG_ATC_ADDR, value);

		loop_timeout = 1000000;
		while (macMT7530ReadReg(REG_ATC_ADDR) & REG_ATC_BUSY_MASK)	// wait until not busy
		{
			if (--loop_timeout == 0)
			{
				ret_val = -1;
				break;
			}
		}
	}

	return ret_val;
}

/*___________________________________________________________________
**      function name: macMT7530GetPortStatus
**      descriptions:
**         get port status
**
**      corresponding register:
**         0x2P00[1:0] FID0_PST
**         0x3P08[0] MAC_LNK_STS_Pn
**
**      parameters:
**         u8 port
**
**      global:
**
**      return:
**         0: Disabled
**         1: Listening
**         2: Learning
**         3: Forwarding
**         4: Blocking (same as 1 for MT7530 h/w)
**         5: Linkdown
**         6: (R)Stp_off (N/A)
**         -1: failure
**
**      call:
**
**      revision:
**      1. WPLin 2012/06/26
**_______________________________________________________________
*/
int macMT7530GetPortStatus(u8 port)
{
	u32 reg, value;
	int ret_val=0;
	const u8 stp_state_mapping[4] =
	{
		0,	// Disable/Discarding
		1, 	// Blocking/Listening/Discarding
		2,	// Learning
		3,	// Forwarding
	};

	if (port >= MT7530_TOTAL_PORTS) return (-1);
	reg = 0x2000 + (0x100 * port);
	value = macMT7530ReadReg(reg);
	ret_val = stp_state_mapping[value & 0x0003];

	reg = 0x3008  + (0x100 * port);
	value = macMT7530ReadReg(reg);
	if ((value & 0x0001) == 0)	// =0: link down
	{
		ret_val = 5;
	}

	return ret_val;
}

/*___________________________________________________________________
**      function name: macMT7530SetPortStatus
**      descriptions:
**         set port  stp status
**
**      corresponding register:
**         0x2P00[1:0] FID0_PST
**         0x3P08[0] MAC_LNK_STS_Pn
**
**      parameters:
**         u8 port
**         u8 stp_Status
**         0: Disabled
**         1: // Blocking/Listening/Discarding
**         2: Learning
**         3: Forwarding
**      call:
**
**      revision:
**_______________________________________________________________
*/
int macMT7530SetPortStatus(u8 port, u8 stp_Status)
{
	u32 reg, value;
	int ret_val=0;
	
	if (port >= MT7530_TOTAL_PORTS) return (-1);
	reg = 0x2000 + (0x100 * port);
	value = macMT7530ReadReg(reg);
	
	value = ((value & (~(0x0003))) | (stp_Status & 0x03));
	macMT7530WriteReg(reg, value);
	
	if (macMT7530_APIDbgDumpEn) printk("%s() (%08X) : %08X\n", __FUNCTION__, reg, value);

	return ret_val;
}

/*___________________________________________________________________
**      function name: macMT7530GetMIBCounter
**      descriptions:
**         ...
**
**      parameters:
**         mt7530_switch_api_MibCntType MibCntType
**         u8 port
**         u32 *Out_Cnt_ptr
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/06/01
**_______________________________________________________________
*/
int macMT7530GetMIBCounter(mt7530_switch_api_MibCntType MibCntType, u8 port, u32 *outptr_Cnt)
{
	u32 reg, value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);
//	reg = 0x4000 + (0x100 * port) + (MibCntType * 4);
	reg = MibCntType + (0x100 * port);
	value = macMT7530ReadReg(reg);
	(*outptr_Cnt) = value;
	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%08X) : %08X\n", __FUNCTION__, port, reg, value);
	return (0);
}

/*___________________________________________________________________
**      function name: macMT7530SetQueuePriority
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/07/10
**_______________________________________________________________
*/
int macMT7530SetQueuePriority(u16 priority, u8 queueId)
{
	u32 reg, value;
	u32 bit_offset;

	if ((priority > 7) || (queueId >= MT7530_QUEUE_NUM)) return (-1);

	reg = 0x0048 + ((priority / 2)*4);
	bit_offset = (priority % 2) ? 24 : 8;
	value = (macMT7530ReadReg(reg) & ~(0x7 << bit_offset)) | (queueId << bit_offset);
	macMT7530WriteReg(reg, value);
	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%08X) : %08X\n", __FUNCTION__, queueId, reg, value);
	return (0);
}

/*___________________________________________________________________
**      function name: macMT7530GetMaximumQueueSize
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/07/10
**_______________________________________________________________
*/
int macMT7530GetMaximumQueueSize(u8 queueId)
{
	// 8 queues share total 256 units
	// every unit = 2 blocks = 2 * 256 bytes
	// every queue max 256 * 2 * 256 / 8 = 16384 bytes (0x4000)
	// but FC_BLK_THD_Q0~Q7 only have 4 bits to select units = 15 * 2 * 256 = 7680 (0x1E00)
	return (15 * MT7530_FC_BLK_UNIT_SIZE);
}

/*___________________________________________________________________
**      function name: macMT7530SetAllocatedQueueSize
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/07/10
**_______________________________________________________________
*/
int macMT7530SetAllocatedQueueSize(u16 AllocatedQueueSize, u8 queueId)
{
	u32 reg, value;

	if (queueId >= MT7530_QUEUE_NUM) return (-1);
#if (1)
	AllocatedQueueSize = ((u32)AllocatedQueueSize + MT7530_FC_BLK_UNIT_SIZE - 1) / MT7530_FC_BLK_UNIT_SIZE;
	if (AllocatedQueueSize > 15) return (-1);	// every queue threshold max 15 units (every unit = 2 * 256 bytes)
	if (queueId >= MT7530_QUEUE_NUM) return (-1);

	reg = 0x1FE4;
	value = (macMT7530ReadReg(reg) & ~(0x0F << (queueId * 4))) | (AllocatedQueueSize << (queueId * 4));
	macMT7530WriteReg(reg, value);
#else
	AllocatedQueueSize = ((u32)AllocatedQueueSize + (128 * 2) - 1) / (128 * 2);
	if (AllocatedQueueSize > (256 / 8)) return (-1);	// every queue max 32 units (every unit = 2 * 128 bytes)

	reg = 0x1FF4;
	value = macMT7530ReadReg(reg) & 0xFF;
	value |= (AllocatedQueueSize & 0xFF);
	macMT7530WriteReg(reg, value);
#endif

	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%08X) : %08X\n", __FUNCTION__, queueId, reg, value);
	return (0);
}

/*___________________________________________________________________
**      function name: macMT7530GetAllocatedQueueSize
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/07/10
**_______________________________________________________________
*/
int macMT7530GetAllocatedQueueSize(u8 queueId)
{
	u32 reg, value;

	if (queueId >= MT7530_QUEUE_NUM) return (-1);

#if (1)
	reg = 0x1FE4;
	value = macMT7530ReadReg(reg);
	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%08X) : %08X\n", __FUNCTION__, queueId, reg, value);
	value = ((value >> (queueId * 4)) & 0x0F) * MT7530_FC_BLK_UNIT_SIZE;
#else
	reg = 0x1FF4;
	value = macMT7530ReadReg(reg) & 0x00FF;
	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%08X) : %08X\n", __FUNCTION__, queueId, reg, value);
	value *= 128 * 2;	// max 255*2*128=65280(0xFF00)
#endif
	return (value);
}

/*___________________________________________________________________
**      function name: macMT7530SetWeight
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/07/12
**_______________________________________________________________
*/
int macMT7530SetWeight(u8 weight, u8 queueId, u8 port)
{
	u32 reg, value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);
	if (queueId >= MT7530_QUEUE_NUM) return (-1);
	if ((weight > 16) || (weight == 0)) return (-1);	// weight : 1~16

	reg = 0x1004 + (0x0100 * port) + (0x0008 * queueId);
	value = (macMT7530ReadReg(reg) & ~(0x0F << 24)) | ((weight - 1) << 24);
	macMT7530WriteReg(reg, value);
	if (macMT7530_APIDbgDumpEn) printk("%s() %2d,%2d (%08X) : %08X\n", __FUNCTION__, port, queueId, reg, value);

	return (0);
}

/*___________________________________________________________________
**      function name: macMT7530GetWeight
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/07/12
**_______________________________________________________________
*/
int macMT7530GetWeight(u8 queueId, u8 port)
{
	u32 reg, value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);
	if (queueId >= MT7530_QUEUE_NUM) return (-1);

	reg = 0x1004 + (0x0100 * port) + (0x0008 * queueId);
	value = macMT7530ReadReg(reg);
	if (macMT7530_APIDbgDumpEn) printk("%s() %2d,%2d (%08X) : %08X\n", __FUNCTION__, port, queueId, reg, value);
	value = 1 + ((value >> 24) & 0x0F);

	return (value);
}

/*___________________________________________________________________
**      function name: macMT7530SetBackPressure
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/07/10
**_______________________________________________________________
*/
int macMT7530SetBackPressure(mt7530_switch_api_backPressure_t *backPressure_p, u8 queueId, u8 port)
{
	u32 reg, value;
	u32 max_threshold, min_threshold;

	if (port >= MT7530_TOTAL_PORTS) return (-1);
	if (queueId >= MT7530_QUEUE_NUM) return (-1);

	max_threshold = (backPressure_p->MaxQueueThreshold) / MT7530_FC_BLK_UNIT_SIZE;
	min_threshold = (backPressure_p->MinQueueThreshold) / MT7530_FC_BLK_UNIT_SIZE;
#if (1)
	if ((max_threshold > 255) || (min_threshold > 15)) return (-1);

	reg = 0x1FE0;
	value = (macMT7530ReadReg(reg) & ~(1 << 31)) | ((backPressure_p->Enable) ? (1 << 31) : 0);
	macMT7530WriteReg(reg, value);

	reg = 0x3000 + (0x100 * port);
	value = (macMT7530ReadReg(reg) & ~(1 << 8)) | ((backPressure_p->Enable) ? (1 << 8) : 0);
	macMT7530WriteReg(reg, value);

	reg = 0x1FE4;
	value = (macMT7530ReadReg(reg) & ~(0x0F << (queueId * 4))) | (min_threshold << (queueId * 4));
	macMT7530WriteReg(reg, value);

	reg = 0x1FF4;
	value = (macMT7530ReadReg(reg) & ~(0xFF)) | max_threshold;
	macMT7530WriteReg(reg, value);
#else
	if ((max_threshold > 255) || (min_threshold > 255)) return (-1);

	reg = 0x1FE0;
	value = macMT7530ReadReg(reg);
	value &= ~((1 << 31) | (0xFF << 8) | (0xFF));
	value |= (((backPressure_p->Enable) ? 1 : 0) << 31) | (max_threshold << 8) | (min_threshold);
	macMT7530WriteReg(reg, value);
	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%08X) : %08X\n", __FUNCTION__, queueId, reg, value);
#endif
	return (0);
}

/*___________________________________________________________________
**      function name: macMT7530GetBackPressure
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/07/10
**_______________________________________________________________
*/
int macMT7530GetBackPressure(mt7530_switch_api_backPressure_t *backPressure_p, u8 queueId, u8 port)
{
	u32 reg, value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);
	if (queueId >= MT7530_QUEUE_NUM) return (-1);

#if (1)
	backPressure_p->Enable = 0;

	reg = 0x1FE0;
	value = macMT7530ReadReg(reg);
	if (value & (1 << 31))	// FC_EN
	{
		reg = 0x3000 + (0x100 * port);
		value = macMT7530ReadReg(reg);
		if (value & (1 << 8))	// BACKPR_EN_Pn
		{
			backPressure_p->Enable = 1;
		}
	}

	reg = 0x1FE4;
	value = macMT7530ReadReg(reg);
	backPressure_p->MinQueueThreshold = MT7530_FC_BLK_UNIT_SIZE * ((value >> (4 * queueId)) & 0x0F);

	reg = 0x1FF4;
	value = macMT7530ReadReg(reg);
	backPressure_p->MaxQueueThreshold = MT7530_FC_BLK_UNIT_SIZE * (value & 0xFF);
#else
	reg = 0x1FE0;
	value = macMT7530ReadReg(reg);
	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%08X) : %08X\n", __FUNCTION__, queueId, reg, value);

	backPressure_p->Enable = (value & (1 << 31)) ? 1 : 0;
	backPressure_p->MinQueueThreshold = value & 0xFF;
	backPressure_p->MaxQueueThreshold = (value >> 8) & 0xFF;
#endif

	reg = 0x3008 + (0x100 * port);
	value = (macMT7530ReadReg(reg) >> 2) & 0x03;
	switch (value)
	{	// return result in us; back-pressure fixed 64 bytes length + 96 bytes IPG
		case 0 :
			backPressure_p->time = ((64 + 96) * 8) / 10;
			break;
		case 1 :
			backPressure_p->time = ((64 + 96) * 8) / 100;
			break;
		case 2 :
			backPressure_p->time = ((64 + 96) * 8) / 1000;
			break;
		default :
			backPressure_p->time = ((64 + 96) * 8) / 10;
			break;
	}

	return (0);
}

static mt7530_switch_DropPolicy_t DropPoliySettings[MT7530_TOTAL_PORTS][MT7530_QUEUE_NUM];

static mt7530_switch_api_trafficDescriptor_t TrafficDescSettings[MT7530_TOTAL_PORTS][2];

const static int PortQueueACLRuleTblEntry[MT7530_TOTAL_PORTS][MT7530_QUEUE_NUM] =
{
	{ 0,  1,  2,  3,  4,  5,  6,  7},
	{ 8,  9, 10, 11, 12, 13, 14, 15},
	{16, 17, 18, 19, 20, 21, 22, 23},
	{24, 25, 26, 27, 28, 29, 30, 31},
	{32, 33, 34, 35, 36, 37, 38, 39},
	{40, 41, 42, 43, 44, 45, 46, 47},
	{48, 49, 50, 51, 52, 53, 54, 55},
};

const static int PortQueueACLActionTblEntry[MT7530_TOTAL_PORTS][MT7530_QUEUE_NUM] =
{
	{ 0,  1,  2,  3,  4,  5,  6,  7},
	{ 8,  9, 10, 11, 12, 13, 14, 15},
	{16, 17, 18, 19, 20, 21, 22, 23},
	{24, 25, 26, 27, 28, 29, 30, 31},
	{32, 33, 34, 35, 36, 37, 38, 39},
	{40, 41, 42, 43, 44, 45, 46, 47},
	{48, 49, 50, 51, 52, 53, 54, 55},
};

static int macMT7530ACLAllocateEntry(u8 port, u8 queueId)
{
	if (port >= MT7530_TOTAL_PORTS) return (-1);
	if (queueId >= MT7530_QUEUE_NUM) return (-1);
	if ((PortQueueACLRuleTblEntry[port][queueId] == -1) || (PortQueueACLActionTblEntry[port][queueId] == -1))
	{
		// use fixed table
	}
	if ((PortQueueACLRuleTblEntry[port][queueId] < 0) || (PortQueueACLRuleTblEntry[port][queueId] >= MT7530_ACL_RULE_NUM)) return (-1);
	if ((PortQueueACLActionTblEntry[port][queueId] < 0) || (PortQueueACLActionTblEntry[port][queueId] >= MT7530_ACL_ACTION_NUM)) return (-1);
	return (PortQueueACLRuleTblEntry[port][queueId]);
}

static int macMT7530ACLReleaseEntry(u8 port, u8 queueId)
{
	// use fixed table
	return (0);
}

static int macMT7530ACLEntryWrite(u8 table_type_rw, u8 acl_entry, u32 word_lo, u32 word_hi)
{
	u32 reg, value, timeout;

	if (acl_entry >= MT7530_ACL_RULE_NUM) return (-1);

	reg = 0x90;	// VTCR
	timeout = 100000;
	while (-- timeout) {
		value = macMT7530ReadReg(reg);		// VTCR
		if ((value & (1 << 31)) == 0) break;	// table busy finish?
		if (timeout == 0) return (-1);		// timeout?
	}

	macMT7530WriteReg(0x94, word_lo);
	macMT7530WriteReg(0x98, word_hi);

	value = (1 << 31) | (table_type_rw << 12) | acl_entry;
	macMT7530WriteReg(reg, value);
	timeout = 100000;
	while (-- timeout) {
		value = macMT7530ReadReg(reg);		// VTCR
		if ((value & (1 << 31)) == 0) break;	// table busy finish?
		if (timeout == 0) return (-1);		// timeout?
	}

	return (0);
}

static int macMT7530ACLPortEnable(u8 port, u8 enable)
{
	u32 reg, value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);

	reg = 0x2004 + (0x100 * port);
	value = macMT7530ReadReg(reg);
	value = (enable) ? (value | (1 << 10)) : (value & ~(1 << 10));	// ACL_EN 0x2n04[10]
	macMT7530WriteReg(reg, value);

	return (0);
}

static int macMT7530QueuePriorityInit(void)
{
	u32 reg, value;

	reg = 0x48;
	value = macMT7530ReadReg(reg);
	value &= ~((7 << 8) | (7 << 11) | (7 << 24) | (7 << 27));
	value |=  ((0 << 8) | (0 << 11) | (1 << 24) | (1 << 27)) ;
	macMT7530WriteReg(reg, value);

	reg = 0x4C;
	value = macMT7530ReadReg(reg);
	value &= ~((7 << 8) | (7 << 11) | (7 << 24) | (7 << 27));
	value |=  ((2 << 8) | (2 << 11) | (3 << 24) | (3 << 27)) ;
	macMT7530WriteReg(reg, value);

	reg = 0x50;
	value = macMT7530ReadReg(reg);
	value &= ~((7 << 8) | (7 << 11) | (7 << 24) | (7 << 27));
	value |=  ((4 << 8) | (4 << 11) | (5 << 24) | (5 << 27)) ;
	macMT7530WriteReg(reg, value);

	reg = 0x54;
	value = macMT7530ReadReg(reg);
	value &= ~((7 << 8) | (7 << 11) | (7 << 24) | (7 << 27));
	value |=  ((6 << 8) | (6 << 11) | (7 << 24) | (7 << 27)) ;
	macMT7530WriteReg(reg, value);

	return (0);
}

static int macMT7530ACLRuleEntryEnable(u8 acl_rule_entry, u8 enable)
{
	u32 reg, value, timeout;

	if (acl_rule_entry >= MT7530_ACL_RULE_NUM) return (-1);

	reg = 0x90;	// VTCR
	timeout = 100000;
	while (-- timeout) {
		value = macMT7530ReadReg(reg);		// VTCR
		if ((value & (1 << 31)) == 0) break;	// table busy finish?
		if (timeout == 0) return (-1);		// timeout?
	}

	value = (1 << 31) | (MT7530_ACL_RULE_TBL_READ << 12) | acl_rule_entry;
	macMT7530WriteReg(reg, value);
	timeout = 100000;
	while (-- timeout) {
		value = macMT7530ReadReg(reg);		// VTCR
		if ((value & (1 << 31)) == 0) break;	// table busy finish?
		if (timeout == 0) return (-1);		// timeout?
	}

	value = macMT7530ReadReg(0x98);			// ACL Rule Table bit [32+19] : "EN"
	value = (enable) ? (value | (1 << 19)) : (value & ~(1 << 19));
	macMT7530WriteReg(0x98, value);

	value = (1 << 31) | (MT7530_ACL_RULE_TBL_WRITE << 12) | acl_rule_entry;
	macMT7530WriteReg(reg, value);
	timeout = 100000;
	while (-- timeout) {
		value = macMT7530ReadReg(reg);		// VTCR
		if ((value & (1 << 31)) == 0) break;	// table busy finish?
		if (timeout == 0) return (-1);		// timeout?
	}

	return (0);
}

static int macMT7530ACLActionEntryEnable(u8 acl_action_entry, u8 enable)
//static int macMT7530ACLActionEntryEnable(u8 acl_action_entry, u32 mask_wordlo, u32 mask_wordhi)
{
	u32 reg, value, timeout;

	if (acl_action_entry >= MT7530_ACL_ACTION_NUM) return (-1);

	reg = 0x90;	// VTCR
	timeout = 100000;
	while (-- timeout) {
		value = macMT7530ReadReg(reg);		// VTCR
		if ((value & (1 << 31)) == 0) break;	// table busy finish?
		if (timeout == 0) return (-1);		// timeout?
	}

	if (enable)
	{
		if (acl_action_entry < 32)
		{
			macMT7530WriteReg(0x94, (1 << acl_action_entry));	// [31:0]
			macMT7530WriteReg(0x98, 0x00000000);			// [63:32]
		}
		else
		{
			macMT7530WriteReg(0x94, 0x00000000);
			macMT7530WriteReg(0x98, (1 << (acl_action_entry - 32)));
		}
	}
	else
	{
		macMT7530WriteReg(0x94, 0x00000000);
		macMT7530WriteReg(0x98, 0x00000000);
	}
//	macMT7530WriteReg(0x94, mask_wordlo);
//	macMT7530WriteReg(0x98, mask_wordhi);

	value = (1 << 31) | (MT7530_ACL_MASK_TBL_WRITE << 12) | acl_action_entry;	// 9 : write specific ACL Mask Table entry
	macMT7530WriteReg(reg, value);
	timeout = 100000;
	while (-- timeout) {
		value = macMT7530ReadReg(reg);		// VTCR
		if ((value & (1 << 31)) == 0) break;	// table busy finish?
		if (timeout == 0) return (-1);		// timeout?
	}

	return (0);
}

static int macMT7530MarkDEIYellow(u8 port)
{
	int i;
	int acl_rule_entry, acl_action_entry;
	u32 word_lo, word_hi;
	u32 rule_mask;

	if (port >= MT7530_TOTAL_PORTS) return (-1);

	macMT7530ACLPortEnable(port, 1);

	for (i = 0; i < MT7530_QUEUE_NUM; i ++)
	{
		macMT7530ACLRuleEntryEnable(PortQueueACLRuleTblEntry[port][i], 0);
		macMT7530ACLActionEntryEnable(PortQueueACLActionTblEntry[port][i], 0);
		macMT7530ACLReleaseEntry(port, i);
	}
	macMT7530ACLAllocateEntry(port, 0);

	acl_rule_entry = PortQueueACLRuleTblEntry[port][0];
	acl_action_entry = PortQueueACLActionTblEntry[port][0];

	if ((acl_rule_entry == -1) || (acl_action_entry == -1)) return (-1);

	word_lo = (0xFFFF << 16) | 0x8100;			// double check 0x8100 tag
	word_hi = (1 << 19) | (0 << 16) | (port << 8) | 0x0C;	// check BP 0x0C
	macMT7530ACLEntryWrite(MT7530_ACL_RULE_TBL_WRITE, (acl_rule_entry + 7), word_lo, word_hi);

	word_lo = (0x1000 << 16) | 0x1000;			// check DEI bit
	word_hi = (1 << 19) | (0 << 16) | (port << 8) | 0x0E;	// check BP 0x0E
	macMT7530ACLEntryWrite(MT7530_ACL_RULE_TBL_WRITE, acl_rule_entry, word_lo, word_hi);

	rule_mask = (1 << 7) | (1 << 0);		// entry (8N+7) && (8N+0)
	if (port < 4)
	{
		word_lo = rule_mask << (port << 3);
		word_hi = 0x00000000;
	}
	else
	{
		word_lo = 0x00000000;
		word_hi = rule_mask << ((port - 4) << 3);
	}
	macMT7530ACLEntryWrite(MT7530_ACL_MASK_TBL_WRITE, acl_action_entry, word_lo, word_hi);

	word_lo = 0x0000;
	word_hi = (2 << 17);	// force yellow
	macMT7530ACLEntryWrite(MT7530_ACL_CTRL_TBL_WRITE, acl_action_entry, word_lo, word_hi);

	return (0);
}

static int macMT7530MarkPriorityYellow(u8 port, u8 Priority)
{
	int acl_rule_entry, acl_action_entry;
	u32 word_lo, word_hi;
	u32 rule_mask;

	if ((Priority != 4) && (Priority != 2) && (Priority != 0)) return (-1);

	acl_rule_entry = PortQueueACLRuleTblEntry[port][0];
	acl_action_entry = PortQueueACLActionTblEntry[port][0];

	if ((acl_rule_entry == -1) || (acl_action_entry == -1)) return (-1);

	word_lo = (0xFFFF << 16) | 0x8100;			// double check 0x8100 tag
	word_hi = (1 << 19) | (0 << 16) | (port << 8) | 0x0C;	// check BP 0x0C
	macMT7530ACLEntryWrite(MT7530_ACL_RULE_TBL_WRITE, (acl_rule_entry + 7), word_lo, word_hi);

	acl_rule_entry += Priority;
	acl_action_entry += Priority;

	if (Priority == 4)	// check 4DE
	{
		word_lo = ((0x07 << 13) << 16) | (4 << 13);		// check PCP=4
		word_hi = (1 << 19) | (0 << 16) | (port << 8) | 0x0E;	// check BP 0x0E
		macMT7530ACLEntryWrite(MT7530_ACL_RULE_TBL_WRITE, acl_rule_entry, word_lo, word_hi);
	}
	else if (Priority == 2)	// check 2DE
	{
		word_lo = ((0x07 << 13) << 16) | (2 << 13);		// check PCP=2
		word_hi = (1 << 19) | (0 << 16) | (port << 8) | 0x0E;	// check BP 0x0E
		macMT7530ACLEntryWrite(MT7530_ACL_RULE_TBL_WRITE, acl_rule_entry, word_lo, word_hi);
	}
	else if (Priority == 0)	// check 0DE
	{
		word_lo = ((0x07 << 13) << 16) | (0 << 13);		// check PCP=0
		word_hi = (1 << 19) | (0 << 16) | (port << 8) | 0x0E;	// check BP 0x0E
		macMT7530ACLEntryWrite(MT7530_ACL_RULE_TBL_WRITE, acl_rule_entry, word_lo, word_hi);
	}

	rule_mask = (1 << 7) | (1 << Priority);		// entry (8N+7) && (8N+x)
	if (port < 4)
	{
		word_lo = rule_mask << (port << 3);
		word_hi = 0x00000000;
	}
	else
	{
		word_lo = 0x00000000;
		word_hi = rule_mask << ((port - 4) << 3);
	}
	macMT7530ACLEntryWrite(MT7530_ACL_MASK_TBL_WRITE, acl_action_entry, word_lo, word_hi);

	word_lo = 0x0000;
	word_hi = (2 << 17);	// force yellow
	macMT7530ACLEntryWrite(MT7530_ACL_CTRL_TBL_WRITE, acl_action_entry, word_lo, word_hi);

	return (0);
}

/*___________________________________________________________________
**      function name: macMT7530SetXPYD
**      descriptions:
**         ...
**
**      parameters:
**         DropPrecdenceColourMarking :
**           3:8P0D (DEI as yellow)
**           4:7P1D (DEI + 4DE as yellow)
**           5:6P2D (DEI + 4DE + 2DE as yellow)
**           6:5P3D (DEI + 4DE + 2DE +0DE as yellow)
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/11/27
**_______________________________________________________________
*/
static int macMT7530SetXPYD(u8 port, u8 DropPrecdenceColourMarking)
{
	u8 i;

	if (port >= MT7530_TOTAL_PORTS) return (-1);
	if ((DropPrecdenceColourMarking < 3) || (DropPrecdenceColourMarking > 6)) return (-1);

	macMT7530QueuePriorityInit();
	macMT7530ACLPortEnable(port, 1);

	for (i = 0; i < MT7530_QUEUE_NUM; i ++)
	{
		macMT7530ACLRuleEntryEnable(PortQueueACLRuleTblEntry[port][i], 0);
		macMT7530ACLActionEntryEnable(PortQueueACLActionTblEntry[port][i], 0);
		macMT7530ACLReleaseEntry(port, i);
	}
	macMT7530ACLAllocateEntry(port, 0);

	if (DropPrecdenceColourMarking >= 4)			// 7P1D, 6P2D, 5P3D
	{
		macMT7530MarkPriorityYellow(port, 4);		// check 4DE

		if (DropPrecdenceColourMarking >= 5)		// 6P2D, 5P3D
		{
			macMT7530MarkPriorityYellow(port, 2);	// check 2DE

			if (DropPrecdenceColourMarking >= 6)	// 5P3D
			{
				macMT7530MarkPriorityYellow(port, 0);	// check 0DE
			}
		}
	}

	return (0);
}

static int macMT7530SetDropPercent(u8 port, u8 queueId, mt7530_switch_DropPolicy_t *DropPolicy)
{
	u32 reg, value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);
	if (queueId >= MT7530_QUEUE_NUM) return (-1);

	reg = 0x180C + (0x100 * port) + (0x04 * queueId);
	value = DropPolicy->yellowPacketDropMaxn_p;	// parameter in 8 bits : 255=100%
	value = value >> 5;				// hw in 3 bits : 7=87.5%
	value |= (100 << 12) | (50 << 0);		// high threshold 100 pages, low threshold 50 pages
	macMT7530WriteReg(reg, value);

	reg = 0x180C + (0x100 * port);
	value = macMT7530ReadReg(reg);
	value |= (1 << 31);		// enable Pn_DP_en
	macMT7530WriteReg(reg, value);

	return (0);
}

/*___________________________________________________________________
**      function name: macMT7530SetDropPolicy
**      descriptions:
**         ...
**
**      parameters:
**         DropPrecdenceColourMarking :
**           0:No Marking
**           1:from Traffic Descriptor
**           2:DEI Marking
**           3:8P0D, 4:7P1D, 5:6P2D, 6:5P3D
**           7:reserved
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/11/26
**_______________________________________________________________
*/
//int macMT7530SetDropPolicy(u8 port, u8 queueId, u16 greenPacketDropQueueMaxThr, u16 greenPacketDropQueueMinThr, u16 yellowPacketDropQueueMaxThr, u16 yellowPacketDropQueueMinThr, u8 greenPacketDropMax_p, u8 yellowPacketDropMaxn_p, u8 QueueDropW_q, u8 DropPrecdenceColourMarking)
int macMT7530SetDropPolicy(u8 port, u8 queueId, mt7530_switch_DropPolicy_t *DropPolicy)
{
	int value, i, acl_rule_entry, acl_action_entry;

	if (port >= MT7530_TOTAL_PORTS) return (-1);
	if (queueId >= MT7530_QUEUE_NUM) return (-1);
	if ((DropPolicy->DropPrecdenceColourMarking == 7) || (DropPolicy->DropPrecdenceColourMarking >= 8)) return (-1);	// reserved or invalid

	value = macMT7530ACLAllocateEntry(port, queueId);
	if (value == -1)
	{
		macMT7530ACLReleaseEntry(port, queueId);
		return (-1);
	}
	acl_rule_entry = PortQueueACLRuleTblEntry[port][queueId];
	acl_action_entry = PortQueueACLActionTblEntry[port][queueId];

	switch (DropPolicy->DropPrecdenceColourMarking)
	{
		case 0 :	// "No Marking"
			macMT7530ACLRuleEntryEnable(acl_rule_entry, 0);
			macMT7530ACLActionEntryEnable(acl_action_entry, 0);
			macMT7530ACLReleaseEntry(port, queueId);
			break;

		case 1 :	// "from Traffic Descriptor"
			// ...
			break;

		case 2 :	// "DEI Marking"
			macMT7530MarkDEIYellow(port);
			break;

		case 3 :	// "8P0D"
			macMT7530SetXPYD(port, DropPolicy->DropPrecdenceColourMarking);
			break;

		case 4 :	// "7P1D"
			macMT7530SetXPYD(port, DropPolicy->DropPrecdenceColourMarking);
			break;

		case 5 :	// "6P2D"
			macMT7530SetXPYD(port, DropPolicy->DropPrecdenceColourMarking);
			break;

		case 6 :	// "5P3D"
			macMT7530SetXPYD(port, DropPolicy->DropPrecdenceColourMarking);
			break;

		default :	// reserved/invalid
			return (-1);
	}

	if ((DropPolicy->DropPrecdenceColourMarking) == 2)	// "DEI Marking"
	{
		for (i = 0; i < MT7530_QUEUE_NUM; i ++)
		{
			macMT7530SetDropPercent(port, i, DropPolicy);
		}
	}
	else
	{
		macMT7530SetDropPercent(port, queueId, DropPolicy);
	}


	// backup parameters for macMT7530GetDropPolicy()
	DropPoliySettings[port][queueId].greenPacketDropQueueMaxThr = DropPolicy->greenPacketDropQueueMaxThr;
	DropPoliySettings[port][queueId].greenPacketDropQueueMinThr = DropPolicy->greenPacketDropQueueMinThr;
	DropPoliySettings[port][queueId].yellowPacketDropQueueMaxThr = DropPolicy->yellowPacketDropQueueMaxThr;
	DropPoliySettings[port][queueId].yellowPacketDropQueueMinThr = DropPolicy->yellowPacketDropQueueMinThr;
	DropPoliySettings[port][queueId].greenPacketDropMax_p = DropPolicy->greenPacketDropMax_p;
	DropPoliySettings[port][queueId].yellowPacketDropMaxn_p = DropPolicy->yellowPacketDropMaxn_p;
	DropPoliySettings[port][queueId].QueueDropW_q = DropPolicy->QueueDropW_q;
	DropPoliySettings[port][queueId].DropPrecdenceColourMarking = DropPolicy->DropPrecdenceColourMarking;

	return (0);
}

/*___________________________________________________________________
**      function name: macMT7530GetDropPolicy
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/11/27
**_______________________________________________________________
*/
//int macMT7530GetDropPolicy(u8 port, u8 queueId, u16 *outptr_greenPacketDropQueueMaxThr, u16 *outptr_greenPacketDropQueueMinThr, u16 *outptr_yellowPacketDropQueueMaxThr, u16 *outptr_yellowPacketDropQueueMinThr, u8 *outptr_greenPacketDropMax_p, u8 *outptr_yellowPacketDropMaxn_p, u8 *outptr_QueueDropW_q, u8 *outptr_DropPrecdenceColourMarking)
int macMT7530GetDropPolicy(u8 port, u8 queueId, mt7530_switch_DropPolicy_t *outptr_DropPolicy)
{
	if (port >= MT7530_TOTAL_PORTS) return (-1);
	if (queueId >= MT7530_QUEUE_NUM) return (-1);

	(outptr_DropPolicy->greenPacketDropQueueMaxThr) = DropPoliySettings[port][queueId].greenPacketDropQueueMaxThr;
	(outptr_DropPolicy->greenPacketDropQueueMinThr) = DropPoliySettings[port][queueId].greenPacketDropQueueMinThr;
	(outptr_DropPolicy->yellowPacketDropQueueMaxThr) = DropPoliySettings[port][queueId].yellowPacketDropQueueMaxThr;
	(outptr_DropPolicy->yellowPacketDropQueueMinThr) = DropPoliySettings[port][queueId].yellowPacketDropQueueMinThr;
	(outptr_DropPolicy->greenPacketDropMax_p) = DropPoliySettings[port][queueId].greenPacketDropMax_p;
	(outptr_DropPolicy->yellowPacketDropMaxn_p) = DropPoliySettings[port][queueId].yellowPacketDropMaxn_p;
	(outptr_DropPolicy->QueueDropW_q) = DropPoliySettings[port][queueId].QueueDropW_q;
	(outptr_DropPolicy->DropPrecdenceColourMarking) = DropPoliySettings[port][queueId].DropPrecdenceColourMarking;

	return (0);
}

/*___________________________________________________________________
struct trafficDescriptor_s{
u32 CIR;
u32 PIR;
u32 CBS;
u32 PBS;
u8 colourMode;
u8 ingressColourMarking;
u8 engressColourMarking;
u8 meterType;
}
**_______________________________________________________________
*/

/*___________________________________________________________________
**      function name: macMT7530SetTrafficDescriptor
**      descriptions:
**         ...
**
**      parameters:


**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/11/26
**_______________________________________________________________
*/
int macMT7530SetTrafficDescriptor(u8 port, mt7530_switch_api_trafficDescriptor_t *trafficDescriptor_p)
{
	u32 reg, value;
	u32 cir, cbs, cir_bit16;
	u8 direction = trafficDescriptor_p->direction;	// 0:TX, 1:RX

	if (port >= MT7530_TOTAL_PORTS) return (-1);
	if (direction > 1) return (-1);

	reg = ((direction == 0) ? (0x1040) : (0x1800)) + (0x100 * port);	// 0:TX, 1:RX

	cir = (((trafficDescriptor_p->CIR) * 8) + 0x7FFF) >> 15;	// cir register in unit of 32768(=2^15) bps; but function parameter is given as byte/s
	if (cir > 0x1FFFF) cir = 0x1FFFF;	// cir register max 2^17 units;
	cir_bit16 = cir >> 16;			// cir register bit 16 is separated
	cir &= 0xFFFF;

	if (trafficDescriptor_p->CBS == 0) // ONU factory policy
	{
		cbs = 0xF;
	}
	else
	{
	cbs = (trafficDescriptor_p->CBS + 0x1FF) >> 9;		// cbs register in unit of 512(=2^9) bytes
	if (cbs > 0x7F) cbs = 0x7F;		// cbs register max 2^7 units
	}


	value = macMT7530ReadReg(reg) & 0x0000E080;
	value |= (cir << 16) | (cir_bit16 << 12) | (4 << 8) | (1 << 7) | (cbs << 0);	// TB_EN=1; TB_T=1/8ms
	if (trafficDescriptor_p->CIR == 0)  // ONU factory policy
		value &= ~(1<<15);
	else
	value |= (1 << 15);              // rate limit control enable.
	macMT7530WriteReg(reg, value);
//	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%02X) : %08X\n", __FUNCTION__, port, reg, value);

	// backup parameters for macMT7530GetTrafficDescriptor()
	TrafficDescSettings[port][direction].CIR = trafficDescriptor_p->CIR;
	TrafficDescSettings[port][direction].PIR = trafficDescriptor_p->PIR;
	TrafficDescSettings[port][direction].CBS = trafficDescriptor_p->CBS;
	TrafficDescSettings[port][direction].PBS = trafficDescriptor_p->PBS;
	TrafficDescSettings[port][direction].colourMode = trafficDescriptor_p->colourMode;
	TrafficDescSettings[port][direction].ingressColourMarking = trafficDescriptor_p->ingressColourMarking;
	TrafficDescSettings[port][direction].engressColourMarking = trafficDescriptor_p->engressColourMarking;
	TrafficDescSettings[port][direction].meterType = trafficDescriptor_p->meterType;
	TrafficDescSettings[port][direction].direction = trafficDescriptor_p->direction;

	return (0);
}

int macMT7530GetTrafficDescriptor(u8 port, mt7530_switch_api_trafficDescriptor_t *outptr_trafficDescriptor_p)
{
	u8 direction = outptr_trafficDescriptor_p->direction;	// 0:TX, 1:RX

	if (port >= MT7530_TOTAL_PORTS) return (-1);
	if (direction > 1) return (-1);

 	outptr_trafficDescriptor_p->CIR = TrafficDescSettings[port][direction].CIR;
 	outptr_trafficDescriptor_p->PIR = TrafficDescSettings[port][direction].PIR;
 	outptr_trafficDescriptor_p->CBS = TrafficDescSettings[port][direction].CBS;
 	outptr_trafficDescriptor_p->PBS = TrafficDescSettings[port][direction].PBS;
 	outptr_trafficDescriptor_p->colourMode = TrafficDescSettings[port][direction].colourMode;
 	outptr_trafficDescriptor_p->ingressColourMarking = TrafficDescSettings[port][direction].ingressColourMarking;
 	outptr_trafficDescriptor_p->engressColourMarking = TrafficDescSettings[port][direction].engressColourMarking;
 	outptr_trafficDescriptor_p->meterType = TrafficDescSettings[port][direction].meterType;
 	outptr_trafficDescriptor_p->direction = TrafficDescSettings[port][direction].direction;

	return (0);
}

int macMT7530CleanTrafficDescriptor(u8 port, u8 direction)
{
	u32 reg, value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);
	if (direction > 1) return (-1);

	reg = ((direction == 0) ? (0x1040) : (0x1800)) + (0x100 * port);	// 0:TX, 1:RX
	value = macMT7530ReadReg(reg) & ~(1 << 7);	// disable EGC_TBEN/IGN_TBEN
	value &= ~(1 << 15);           // rate limit control diable.
	macMT7530WriteReg(reg, value);

	return (0);
}


/*___________________________________________________________________
**      function name: macMT7530SetAutoDetection
**      descriptions:
**         ...
**
**      parameters:
**        AutoDetction :
**        0x00 - Auto;      Auto
**        0x01 - 10 only;   Full duplex only
**        0x02 - 100 only;  Full duplex only
**        0x03 - 1000 only; Full duplex only
**        0x04 - Auto;      Full duplex only
**        0x10 - 10 only;   Auto
**        0x11 - 10 only;   Half duplex only
**        0x12 - 100 only;  Half duplex only
**        0x13 - 1000 only; Half duplex only
**        0x14 - Auto;      Half duplex only
**        0x20 - 1000 only; Auto
**        0x30 - 100 only;  Auto
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/07/17
**_______________________________________________________________
*/
int macMT7530SetAutoDetection(u8 AutoDetction, u8 port)
{
	u32 phy_addr, phy_reg, phy_value;
	int i;

	if (port >= MT7530_TOTAL_PORTS) return (-1);
	phy_addr = macMT7530GetPortPhyAddr(port);

	for (i = 0; i < MIIMediaOption_Entries; i ++)
	{
		if (AutoDetction == MIIMediaOption[i].index_param)
		{
			break;
		}

	}
	if (i == MIIMediaOption_Entries) return (-1);	// can not found

	if ((MIIMediaOption[i].MII_BMCR_value) & BMCR_ANENABLE)
	{
		phy_reg = MII_ADVERTISE;
		phy_value = macMT7530PHYReadReg(phy_addr, phy_reg);
		phy_value &= ~(ADVERTISE_CSMA | ADVERTISE_10HALF | ADVERTISE_10FULL | ADVERTISE_100HALF | ADVERTISE_100FULL);
		phy_value |= MIIMediaOption[i].MII_ADVERTISE_value;
		macMT7530PHYWriteReg(phy_addr, phy_reg, phy_value);

		phy_reg = MII_CTRL1000;
		phy_value = macMT7530PHYReadReg(phy_addr, phy_reg);
		phy_value &= ~(ADVERTISE_1000FULL | ADVERTISE_1000HALF);
		phy_value |= MIIMediaOption[i].MII_CTRL1000_value;
		macMT7530PHYWriteReg(phy_addr, phy_reg, phy_value);
	}

	phy_reg = MII_BMCR;
	phy_value = macMT7530PHYReadReg(phy_addr, phy_reg);
	phy_value &= ~(BMCR_ANENABLE | BMCR_SPEED100 | BMCR_SPEED1000 | BMCR_FULLDPLX);
	phy_value |= BMCR_ANRESTART + MIIMediaOption[i].MII_BMCR_value;
	macMT7530PHYWriteReg(phy_addr, phy_reg, phy_value);

	return (0);
}

int macMT7530GetAutoDetection(u8 *outptr_AutoDetction, u8 port)
{
	u32 phy_addr, phy_bmcr, phy_advertise, phy_ctrl1000;
	int i;

	if (port >= MT7530_TOTAL_PORTS) return (-1);
	phy_addr = macMT7530GetPortPhyAddr(port);

	phy_bmcr = macMT7530PHYReadReg(phy_addr, MII_BMCR) & (BMCR_ANENABLE | BMCR_SPEED100 | BMCR_SPEED1000 | BMCR_FULLDPLX);
	phy_advertise = macMT7530PHYReadReg(phy_addr, MII_ADVERTISE) & (ADVERTISE_CSMA | ADVERTISE_10HALF | ADVERTISE_10FULL | ADVERTISE_100HALF | ADVERTISE_100FULL);
	phy_ctrl1000 = macMT7530PHYReadReg(phy_addr, MII_CTRL1000) & (ADVERTISE_1000FULL | ADVERTISE_1000HALF);

	for (i = 0; i < MIIMediaOption_Entries; i ++)
	{
		if (!(phy_bmcr & BMCR_ANENABLE) && (phy_bmcr != MIIMediaOption[i].MII_BMCR_value))
		{
			continue;
		}
		if ((phy_bmcr & BMCR_ANENABLE) && !(MIIMediaOption[i].MII_BMCR_value & BMCR_ANENABLE))
		{
			continue;
		}
		if ((phy_bmcr & BMCR_ANENABLE) && (phy_advertise != MIIMediaOption[i].MII_ADVERTISE_value))
		{
			continue;
		}
#if (1)	// FPGA platform phy chip not support 1000M AN, related register contain garbage value
		if ((phy_bmcr & BMCR_ANENABLE) && (phy_ctrl1000 != MIIMediaOption[i].MII_CTRL1000_value))
		{
			continue;
		}
#endif
		(*outptr_AutoDetction) = MIIMediaOption[i].index_param;
		return (0);
	}
	return (-1);	// can not found
}

/*___________________________________________________________________
**      function name: macMT7530SetLoopbackConf
**      descriptions:
**         PHY Loopback
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/07/23
**_______________________________________________________________
*/
int macMT7530SetLoopbackConf(u8 enable, u8 port)
{
	u32 phy_addr, phy_reg, phy_value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);

	phy_addr = macMT7530GetPortPhyAddr(port);
	phy_reg = MII_BMCR;
	phy_value = (macMT7530PHYReadReg(phy_addr, phy_reg) & ~(BMCR_LOOPBACK)) | (enable ? (BMCR_LOOPBACK) : 0);
	macMT7530PHYWriteReg(phy_addr, phy_reg, phy_value);
	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%02X, %02X) : %04X\n", __FUNCTION__, port, phy_addr, phy_reg, phy_value);

	return (0);
}

int macMT7530GetLoopbackConf(u8 *outptr_mode, u8 port)
{
	u32 phy_addr, phy_reg, phy_value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);

	phy_addr = macMT7530GetPortPhyAddr(port);
	phy_reg = MII_BMCR;
	phy_value = macMT7530PHYReadReg(phy_addr, phy_reg);
	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%02X, %02X) : %04X\n", __FUNCTION__, port, phy_addr, phy_reg, phy_value);
	(*outptr_mode) = (phy_value & BMCR_LOOPBACK) ? 1 : 0;

	return (0);
}

/*___________________________________________________________________
**      function name: macMT7530GetConfigurationStatus
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0x01 : 10BaseT full duplex
**         0x02 : 100BaseT full duplex
**         0x03 : Gigabit Ethernet full duplex
**         0x11 : 10BaseT half duplex
**         0x12 : 100BaseT half duplex
**         0x13 : Gigabit Ethernet half duplex
**
**      call:
**
**      revision:
**      1. WPLin 2012/07/23
**_______________________________________________________________
*/
int macMT7530GetConfigurationStatus(u8 port)
{
	int i;
#if (1)
	u32 reg, value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);

	reg = 0x3008 + (0x100 * port);
	value = macMT7530ReadReg(reg);
	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%08X) : %08X\n", __FUNCTION__, port, reg, value);
	value &= 0x000F;

	for (i = 0; i < mt7530MediaStatus_Entries; i ++)
	{
		if (value != mt7530MediaStatus[i][1])
		{
			continue;
		}
		return (mt7530MediaStatus[i][0]);
	}
	return (-1);	// can not found
#else
	u32 phy_addr, phy_reg, phy_value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);

	phy_addr = macMT7530GetPortPhyAddr(port);
	phy_reg = MII_BMSR;
	phy_value = macMT7530PHYReadReg(phy_addr, phy_reg);
#endif

	return (0);
}

/*___________________________________________________________________
**      function name: macMT7530SetLoopbackConf
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/07/24
**_______________________________________________________________
*/
int macMT7530SetMaxFrameSize(u16 MaxFrameSize, u8 port)
{
	u32 reg, value, rx_pkt_len;

	switch (MaxFrameSize)
	{
		case 1518 :	// 1518(untag)/1522(tag)
			rx_pkt_len = 0;
			break;

		case 1536 :
			rx_pkt_len = 1;
			break;

		case 1552 :
			rx_pkt_len = 2;
			break;

		default :
			return (-1);	// Lisa said unsupported size will return fail
			break;
	}

	reg = 0x30E0;
	value = macMT7530ReadReg(reg);
	value = (value & ~(0x03)) | rx_pkt_len;
	macMT7530WriteReg(reg, value);
	if (macMT7530_APIDbgDumpEn) printk("%s() (%08X) : %08X\n", __FUNCTION__, reg, value);

	return (0);
}

int macMT7530GetMaxFrameSize(u16 *outptr_MaxFrameSize, u8 port)
{
	u32 reg, value;

	reg = 0x30E0;
	value = macMT7530ReadReg(reg);
	if (macMT7530_APIDbgDumpEn) printk("%s() (%08X) : %08X\n", __FUNCTION__, reg, value);

	switch (value & 0x03)
	{
		case 0 :	// 1518(untag)/1522(tag)
			(*outptr_MaxFrameSize) = 1518;
			break;

		case 1 :
			(*outptr_MaxFrameSize) = 1536;
			break;

		case 2 :
			(*outptr_MaxFrameSize) = 1552;
			break;

		case 3 :
			(*outptr_MaxFrameSize) = 1024 * ((value >> 2) & 0x0F);
			break;

		default :
			break;
	}

	return (0);
}

/*___________________________________________________________________
**      function name: macMT7530SetLoopbackConf
**      descriptions:
**         ...
**
**      parameters:
**         0 : DCE or MDI-X.
**         1 : DTE or MDI.
**         2 : Automatic selection. (default)
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/07/24
**_______________________________________________________________
*/
int macMT7530SetDTEDCEInd(u8 ind, u8 port)
{
	u32 phy_addr, phy_reg, phy_value;
	int i;

	if (port >= MT7530_TOTAL_PORTS) return (-1);

	for (i = 0; i < mt7530DTEDCEInd_Entries; i ++)
	{
		if (ind == mt7530DTEDCEInd[i][0])
		{
			break;
		}

	}
	if (i == mt7530DTEDCEInd_Entries) return (-1);	// invalid parameter

	phy_addr = macMT7530GetPortPhyAddr(port);

	// need enter PHY chip test mode to change this settings
	macMT7530PHYWriteReg(phy_addr, 0x1F, 0x2A30);

	phy_reg = 0x05;
	phy_value = (macMT7530PHYReadReg(phy_addr, phy_reg) & ~(0x0018)) | (mt7530DTEDCEInd[i][1]);
	macMT7530PHYWriteReg(phy_addr, phy_reg, phy_value);
	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%02X, %02X) : %04X\n", __FUNCTION__, port, phy_addr, phy_reg, phy_value);

	// leave PHY chip test mode
	macMT7530PHYWriteReg(phy_addr, 0x1F, 0x0000);

	return (0);
}

int macMT7530GetDTEDCEInd(u8 *outptr_ind, u8 port)
{
	u32 phy_addr, phy_reg, phy_value;
	int i;

	if (port >= MT7530_TOTAL_PORTS) return (-1);

	phy_addr = macMT7530GetPortPhyAddr(port);

	// need enter PHY chip test mode to change this settings
	macMT7530PHYWriteReg(phy_addr, 0x1F, 0x2A30);

	phy_reg = 0x05;
	phy_value = macMT7530PHYReadReg(phy_addr, phy_reg);
	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%02X, %02X) : %04X\n", __FUNCTION__, port, phy_addr, phy_reg, phy_value);
	phy_value &= 0x0018;

	// leave PHY chip test mode
	macMT7530PHYWriteReg(phy_addr, 0x1F, 0x0000);

	for (i = 0; i < mt7530DTEDCEInd_Entries; i ++)
	{
		if (phy_value == mt7530DTEDCEInd[i][1])
		{
			break;
		}

	}
	if (i == mt7530DTEDCEInd_Entries) return (-1);	// can not found

	(*outptr_ind) = mt7530DTEDCEInd[i][0];

	return (0);
}

/*___________________________________________________________________
**      function name: macMT7530SetPauseTime
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/07/25
**_______________________________________________________________
*/
int macMT7530SetPauseTime(u16 pauseTime, u8 port)
{
	u32 phy_addr, phy_reg, phy_value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);
	phy_addr = macMT7530GetPortPhyAddr(port);
	phy_reg = MII_ADVERTISE;
	phy_value = (macMT7530PHYReadReg(phy_addr, phy_reg) & ~(ADVERTISE_PAUSE_CAP)) | (pauseTime ? (ADVERTISE_PAUSE_CAP) : 0);
	macMT7530PHYWriteReg(phy_addr, phy_reg, phy_value);

	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%02X, %02X) : %04X\n", __FUNCTION__, port, phy_addr, phy_reg, phy_value);

	return (0);
}

int macMT7530GetPauseTime(u16 *outptr_pauseTime, u8 port)
{
	u32 phy_addr, phy_reg, phy_value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);

	phy_addr = macMT7530GetPortPhyAddr(port);
	phy_reg = MII_ADVERTISE;
	phy_value = macMT7530PHYReadReg(phy_addr, phy_reg);
	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%02X, %02X) : %04X\n", __FUNCTION__, port, phy_addr, phy_reg, phy_value);
	(*outptr_pauseTime) = (phy_value & ADVERTISE_PAUSE_CAP) ? 1 : 0;

	return (0);
}

/*___________________________________________________________________
**      function name: macMT7530SetPortAdmin
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. Andy 2014/01/01
**_______________________________________________________________
*/
int macMT7530SetPortAdmin(u8 admin, u8 port)
{
	u32 phy_addr, phy_reg, phy_value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);

	phy_addr = macMT7530GetPortPhyAddr(port);
	phy_reg = MII_BMCR;
	phy_value = (macMT7530PHYReadReg(phy_addr, phy_reg) & ~(BMCR_PDOWN)) | (admin ? (BMCR_PDOWN) : 0);
	macMT7530PHYWriteReg(phy_addr, phy_reg, phy_value);

	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%02X, %02X) : %04X\n", __FUNCTION__, port, phy_addr, phy_reg, phy_value);

	return (0);
}

int macMT7530GetPortAdmin(u8 *outptr_admin, u8 port)
{
	u32 phy_addr, phy_reg, phy_value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);

	phy_addr = macMT7530GetPortPhyAddr(port);
	phy_reg = MII_BMCR;
	phy_value = macMT7530PHYReadReg(phy_addr, phy_reg);
	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%02X, %02X) : %04X\n", __FUNCTION__, port, phy_addr, phy_reg, phy_value);
	(*outptr_admin) = (phy_value & BMCR_PDOWN) ? 1 : 0;

	return (0);
}


int macMT7530GetPortChangedCnt(u8 port, u32 *outptr_Cnt)
{
	return (0);	// Lightman_Wang delete request
}
/*___________________________________________________________________
**      function name: macMT7530GetPortMacLimit
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/07/26
**_______________________________________________________________
*/
int macMT7530GetPortMacLimit(u8 port)
{
	u32 reg, value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);
	
	reg = 0x200c + (0x0100 * port);
	value = macMT7530ReadReg(reg);
	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%08X) : %08X\n", __FUNCTION__, port, reg, value);

	if (value & 0x10) // disable source mac addr learning
		return 0; // disable learning 
	
	if (value & 0x20){ // enable rx sa learning counter
		value = (value & 0xFFF00) >> 8;
		if (value == 0)
			return 0; // disable learning 
		else if (value == 0xFFF)
			return 0xFFFF; // no limit
		return value;
	}else
		return 0xFFFF; // no limit
}

int macMT7530CleanMactblByPort(u8 port){
	
	u32 reg, value;
	int ac_mat = 0xc;
	reg = REG_ATC_ADDR;
	while (1){	// wait until not busy
		value = macMT7530ReadReg(reg);
		if ((value & REG_ATC_BUSY_MASK) == 0){
			break;
		}
	}
	
	macMT7530WriteReg(REG_ATA1_ADDR, 1<<port);
	
	// clean all entries
	reg = REG_ATC_ADDR;
	value = (1 << REG_ATC_BUSY_OFFT) + (ac_mat << REG_ATC_AC_MAT_OFFT) + (2 << REG_ATC_AC_CMD_OFFT);	// 3'b010 : clean command (multiple entries)
	macMT7530WriteReg(reg, value);
	
	while (1){	// wait until not busy
	value = macMT7530ReadReg(reg);
		if ((value & REG_ATC_BUSY_MASK) == 0){
			break;
		}
	}

}
int macMT7530SetPortMacLimit(u8 port, u32 cnt)
{
	u32 reg, value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);

	reg = 0x200c + (0x0100 * port);
	value = macMT7530ReadReg(reg);
	value &= 0x3;
	if (cnt == 0) // disable learing
		value = value |(0xFFF<<8);
	else if (cnt >= 0xFFF)// no limit
		value = value |(0xFFF<<8)|0x20;
	else
		value = value | (cnt<<8)|0x24;
	
	macMT7530WriteReg(reg, 0xfff00);
	macMT7530CleanMactblByPort(port);
	macMT7530WriteReg(reg, value);
	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%08X) : %08X\n", __FUNCTION__, port, reg, value);
	return 0;
}
int macMT7530SetPortSpeedMode(u8 port, u8 mode)
{
	u32 reg, value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);

	reg = 0x3000 + (0x0100 * port);
	value = macMT7530ReadReg(reg);

	value &= 0xFFFFFF01; // clear speed & duplex
	switch (mode){
		case 0x01: // 10M half
		value = value | (0x01<<15) ;
		break;
		case 0x02: // 10M full
		value = value | (0x01<<15) | 0x02;
		break;
		case 0x03: //100M half
		value = value | (0x01<<15) | 0x04;
		break;
		case 0x04: // 100M full
		value = value | (0x01<<15) | 0x06;
		break;
		case 0x05: // 1g
		value = value | (0x01<<15) | 0x0A; // 1000M full
		break;
		default:
			return 0;
	}
	
	macMT7530WriteReg(reg, value);
	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%08X) : %08X\n", __FUNCTION__, port, reg, value);
	return 0;
}

/*___________________________________________________________________
**      function name: macMT7530GetPause
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/07/26
**_______________________________________________________________
*/
int macMT7530GetPortPause(u8 port)
{
	u32 phy_addr, phy_reg, phy_value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);

	phy_addr = macMT7530GetPortPhyAddr(port);
	phy_reg = MII_ADVERTISE;
	phy_value = macMT7530PHYReadReg(phy_addr, phy_reg);
	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%02X, %02X) : %04X\n", __FUNCTION__, port, phy_addr, phy_reg, phy_value);

	return (phy_value & ADVERTISE_PAUSE_CAP) ? 1 : 0;
}

int macMT7530SetPortPause(u8 port, u8 enable)
{
	u32 phy_addr, phy_reg, phy_value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);

	phy_addr = macMT7530GetPortPhyAddr(port);
	phy_reg = MII_ADVERTISE;
	phy_value = (macMT7530PHYReadReg(phy_addr, phy_reg) & ~(ADVERTISE_PAUSE_CAP)) | (enable ? (ADVERTISE_PAUSE_CAP) : 0);
	macMT7530PHYWriteReg(phy_addr, phy_reg, phy_value);

	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%02X, %02X) : %04X\n", __FUNCTION__, port, phy_addr, phy_reg, phy_value);

	return (0);
}

/*___________________________________________________________________
**      function name: macMT7530GetPortPolicingEnable
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/10/30
**_______________________________________________________________
*/
int macMT7530GetPortPolicingEnable(u8 port)
{
	u32 reg, value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);

	reg = 0x1800 + (0x0100 * port);
	value = macMT7530ReadReg(reg);
	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%02X) : %08X\n", __FUNCTION__, port, reg, value);

	return ((value & (1 << 15)) ? 1 : 0);
}

/*___________________________________________________________________
**      function name: macMT7530SetPortPolicingEnable
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/10/30
**_______________________________________________________________
*/
int macMT7530SetPortPolicingEnable(u8 port, u8 enable)
{
	u32 reg, value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);

	reg = 0x1800 + (0x0100 * port);
	value = macMT7530ReadReg(reg);
	value = (enable) ? (value | (1 << 15)) : (value & ~(1 << 15));
	macMT7530WriteReg(reg, value);
	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%02X) : %08X\n", __FUNCTION__, port, reg, value);

	return (0);
}

/*___________________________________________________________________
**      function name: macMT7530GetPortPolicing
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/10/30
**_______________________________________________________________
*/
int macMT7530GetPortPolicing(u8 port, u32 *outptr_cir, u32 *outptr_cbs, u32 *outptr_ebs)
{
	u32 reg, value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);

	reg = 0x1800 + (0x0100 * port);
	value = macMT7530ReadReg(reg);
	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%02X) : %08X\n", __FUNCTION__, port, reg, value);

	if ((value & (1 << 7)) == 0) return (-1);		// TB_EN=0 : not bucket mode

	(*outptr_cir) = ((value >> 16) & 0xFFFF);
	if (value & (1 << 12)) (*outptr_cir) |= (1 << 16);	// cir register bit 16 is separated
	(*outptr_cir) <<= 5;					// cir register in unit of 32768(=2^15) bps; but function parameter is given as kbps

	(*outptr_cbs) = (value & 0x7F) << 9;			// cbs register in unit of 512(=2^9) bytes

	(*outptr_ebs) = 0;					// ebs fixed 0

	return (0);
}

/*___________________________________________________________________
**      function name: macMT7530SetPortPolicing
**      descriptions:
**         ...
**
**      parameters:
**         cir : in kbps
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/10/30
**_______________________________________________________________
*/
int macMT7530SetPortPolicing(u8 port, u32 cir, u32 cbs, u32 ebs)
{
	u32 reg, value;
	u32 cir_bit16;

	if (port >= MT7530_TOTAL_PORTS) return (-1);

	cir = (cir + 0x1F) >> 5;		// cir register in unit of 32768(=2^15) bps; but function parameter is given as kbps
	if (cir > 0x1FFFF) cir = 0x1FFFF;	// cir register max 2^17 units;
	cir_bit16 = cir >> 16;			// cir register bit 16 is separated
	cir &= 0xFFFF;

	cbs = (cbs + 0x1FF) >> 9;		// cbs register in unit of 512(=2^9) bytes
	if (cbs > 0x7F) cbs = 0x7F;		// cbs register max 2^7 units

	reg = 0x1800 + (0x0100 * port);
	value = macMT7530ReadReg(reg) & 0x0000E080;
	value |= (cir << 16) | (cir_bit16 << 12) | (4 << 8) | (1 << 7) | (cbs << 0);	// TB_EN=1; TB_T=1/8ms
	macMT7530WriteReg(reg, value);
	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%02X) : %08X\n", __FUNCTION__, port, reg, value);

	return (0);
}

/*___________________________________________________________________
**      function name: macMT7530GetPortRatelimitingEnable
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/10/30
**_______________________________________________________________
*/
int macMT7530GetPortRatelimitingEnable(u8 port)
{
	u32 reg, value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);

	reg = 0x1040 + (0x0100 * port);
	value = macMT7530ReadReg(reg);
	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%02X) : %08X\n", __FUNCTION__, port, reg, value);

	return ((value & (1 << 15)) ? 1 : 0);
}

/*___________________________________________________________________
**      function name: macMT7530SetPortRatelimitingEnable
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/10/30
**_______________________________________________________________
*/
#define PORT_FLOWCTL_BIT 0x00000030
#define PORT6_MAC_CTL_REG 0x3600
int macMT7530SetPortRatelimitingEnable(u8 port, u8 enable)
{
	u32 reg, value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);

	reg = 0x1040 + (0x0100 * port);
	value = macMT7530ReadReg(reg);
	value = (enable) ? (value | (1 << 15)) : (value & ~(1 << 15));
	macMT7530WriteReg(reg, value);

	/* when enable DS ratelimit function, disable port6 flow control */
	reg = PORT6_MAC_CTL_REG;
	value = macMT7530ReadReg(PORT6_MAC_CTL_REG);
	value = (value & ~PORT_FLOWCTL_BIT) | (enable?0:PORT_FLOWCTL_BIT);
	macMT7530WriteReg(reg, value);
	
	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%02X) : %08X\n", __FUNCTION__, port, reg, value);

	return (0);
}

/*___________________________________________________________________
**      function name: macMT7530GetPortRatelimiting
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/10/30
**_______________________________________________________________
*/
int macMT7530GetPortRatelimiting(u8 port, u32 *outptr_cir, u32 *outptr_pir)
{
	u32 reg, value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);

	reg = 0x1040 + (0x0100 * port);
	value = macMT7530ReadReg(reg);
	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%02X) : %08X\n", __FUNCTION__, port, reg, value);

	if ((value & (1 << 7)) == 0) return (-1);		// TB_EN=0 : not bucket mode

	(*outptr_cir) = ((value >> 16) & 0xFFFF);
	if (value & (1 << 12)) (*outptr_cir) |= (1 << 16);	// cir register bit 16 is separated
	(*outptr_cir) <<= 5;					// cir register in unit of 32768(=2^15) bps; but function parameter is given as kbps

	(*outptr_pir) = (*outptr_cir);				// pir == cir

	return (0);
}

/*___________________________________________________________________
**      function name: macMT7530SetPortRatelimiting
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/10/30
**_______________________________________________________________
*/
int macMT7530SetPortRatelimiting(u8 port, u32 cir, u32 pir)
{
	u32 reg, value;
	u32 cir_bit16;

	if (port >= MT7530_TOTAL_PORTS) return (-1);

	cir = (cir + 0x1F) >> 5;		// cir register in unit of 32768(=2^15) bps; but function parameter is given as kbps
	if (cir >= 0x20000) cir = 0x1FFFF;	// cir register max 2^17 units;
	cir_bit16 = cir >> 16;			// cir register bit 16 is separated
	cir &= 0xFFFF;

	reg = 0x1040 + (0x0100 * port);
	value = macMT7530ReadReg(reg) & 0x0000E080;
	value |= (cir << 16) | (cir_bit16 << 12) | (4 << 8) | (1 << 7) | (0x7F << 0);	//TB_EN=1; ; TB_T=1/8ms; TB_CBS=max
	macMT7530WriteReg(reg, value);
	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%02X) : %08X\n", __FUNCTION__, port, reg, value);

	return (0);
}

/*___________________________________________________________________
**      function name: macMT7530GetPortLoopDetect
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/07/25
**_______________________________________________________________
*/
int macMT7530GetPortLoopDetect(u8 port)
{
#if (0)	// Lightman_Wang delete request
	u32 phy_addr, phy_reg, phy_value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);

	phy_addr = macMT7530GetPortPhyAddr(port);
	phy_reg = MII_BMCR;
	phy_value = macMT7530PHYReadReg(phy_addr, phy_reg);
	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%02X, %02X) : %04X\n", __FUNCTION__, port, phy_addr, phy_reg, phy_value);
	return ((phy_value & BMCR_LOOPBACK) ? 1 : 0);
#else
	return (-1);
#endif
}

int macMT7530SetPortLoopDetect(u8 port, u8 enable)
{
#if (0)	// Lightman_Wang delete request
	u32 phy_addr, phy_reg, phy_value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);

	phy_addr = macMT7530GetPortPhyAddr(port);
	phy_reg = MII_BMCR;
	phy_value = (macMT7530PHYReadReg(phy_addr, phy_reg) & ~(BMCR_LOOPBACK)) | (enable ? (BMCR_LOOPBACK) : 0);
	macMT7530PHYWriteReg(phy_addr, phy_reg, phy_value);
	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%02X, %02X) : %04X\n", __FUNCTION__, port, phy_addr, phy_reg, phy_value);

	return (0);
#else
	return (-1);
#endif
}

int macMT7530SetPortDisableLooped(u8 port, u8 enable)
{
	return (-1);	// Lightman_Wang delete request
}

/*___________________________________________________________________
**      function name: macMT7530GetPortActive
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/07/25
**_______________________________________________________________
*/
int macMT7530GetPortActive(u8 port)
{
	u32 reg, value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);

	reg = 0x3000 + (0x0100 * port);
#if defined(TCSUPPORT_CUC) || defined(TCSUPPORT_CT_PON_CY)
 	value = macMT7530GetPortPhyAddr(port);

	return	((value & (~ENABLE_LAN_MASK)) ? 1 : 0);
#else
	value = macMT7530ReadReg(reg);

	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%08X) : %08X\n", __FUNCTION__, port, reg, value);

	return ((value & (0x03 << 13)) ? 1 : 0);	
#endif
}

int macMT7530SetPortActive(u8 port, u8 enable)
{
	u32 reg, value;
#if defined(TCSUPPORT_CUC) || defined(TCSUPPORT_CT_PON_CY)
	u32 phy_addr;
#endif

	if (port >= MT7530_TOTAL_PORTS) return (-1);

	reg = 0x3000 + (0x0100 * port);

#if defined(TCSUPPORT_CUC) || defined(TCSUPPORT_CT_PON_CY)
	phy_addr = macMT7530GetPortPhyAddr(port);

	if(enable == 1){
		value = macMT7530PHYReadReg(phy_addr, reg);
		value &= ENABLE_LAN_MASK;
   }else{
	   value = macMT7530PHYReadReg(phy_addr, reg);
	   value |= ~ENABLE_LAN_MASK;
   }	
   
   macMT7530PHYWriteReg(phy_addr, reg, value);
   
#else
	value = (macMT7530ReadReg(reg) & ~(0x03 << 13)) | (enable ? (0x03 << 13) : 0);
	macMT7530WriteReg(reg, value);
#endif


	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%08X) : %08X\n", __FUNCTION__, port, reg, value);

	return (0);
}

int macMT7530GetPortAutoNegState(u8 port){
	u32 phy_addr, phy_reg, phy_value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);

	phy_addr = macMT7530GetPortPhyAddr(port);
	phy_reg = MII_BMCR;
	phy_value = macMT7530PHYReadReg(phy_addr, phy_reg);
	return ((phy_value & BMCR_ANENABLE) ? 1 : 0);
}
/*___________________________________________________________________
**      function name: macMT7530PortAutoNegActive
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/07/25
**_______________________________________________________________
*/
int macMT7530PortAutoNegActive(u8 port, u8 enable)
{
	u32 phy_addr, phy_reg, phy_value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);

	phy_addr = macMT7530GetPortPhyAddr(port);
	phy_reg = MII_BMCR;
	phy_value = macMT7530PHYReadReg(phy_addr, phy_reg);
	phy_value = (enable) ? (phy_value | BMCR_ANENABLE) : (phy_value & ~(BMCR_ANENABLE));
	macMT7530PHYWriteReg(phy_addr, phy_reg, phy_value);

	return (0);
}

/*___________________________________________________________________
**      function name: macMT7530PortAutoNegRestartAutoConfig
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/07/25
**_______________________________________________________________
*/
int macMT7530PortAutoNegRestartAutoConfig(u8 port)
{
	u32 phy_addr, phy_reg, phy_value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);

	phy_addr = macMT7530GetPortPhyAddr(port);
	phy_reg = MII_BMCR;
	phy_value = macMT7530PHYReadReg(phy_addr, phy_reg);
	phy_value |= (BMCR_ANENABLE | BMCR_ANRESTART);
	macMT7530PHYWriteReg(phy_addr, phy_reg, phy_value);

	return (0);
}

/*___________________________________________________________________
**      function name: macMT7530GetEthPortAutoNegFailure
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/08/
**_______________________________________________________________
*/
int macMT7530GetEthPortAutoNegFailure(u8 port)
{
	u32 phy_addr, phy_reg;
	u8 an_enable, link_up;

	if (port >= MT7530_TOTAL_PORTS) return (-1);

	phy_addr = macMT7530GetPortPhyAddr(port);
	phy_reg = MII_BMCR;
	an_enable = macMT7530PHYReadReg(phy_addr, phy_reg) & BMCR_ANENABLE;
	phy_reg = MII_BMSR;
	link_up = macMT7530PHYReadReg(phy_addr, phy_reg) & BMSR_LSTATUS;

	if (an_enable && !link_up)
	{
		return (1);
	}
	else
	{
		return (0);
	}
}

/*___________________________________________________________________
**      function name: macMT7530GetEthPortLos
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:Signal Not Loss
**         1:Signal Loss
**
**      call:
**
**      revision:
**      1. WPLin 2012/08/
**_______________________________________________________________
*/
int macMT7530GetEthPortLos(u8 port)
{
	u32 reg, value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);

	reg = 0x3008 + (0x0100 * port);
	value = macMT7530ReadReg(reg);
	if (macMT7530_APIDbgDumpEn) printk("%s() %2d (%08X) : %08X\n", __FUNCTION__, port, reg, value);

	return ((value & 0x01) ? 0 : 1);
}

/*___________________________________________________________________
**      function name: macMT7530GetEthPortFailure
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/08/09
**_______________________________________________________________
*/
int macMT7530GetEthPortFailure(u8 port)
{
	u32 phy_addr, phy_reg, phy_value;

	if (port >= MT7530_TOTAL_PORTS) return (-1);

	phy_addr = macMT7530GetPortPhyAddr(port);
	phy_reg = MII_BMSR;
	phy_value = macMT7530PHYReadReg(phy_addr, phy_reg);

	// In current MT/RT GSW PHY chip, BMSR reg msb 7 bits = 0111,100x,xxxx,xxxx
	return (((phy_value & 0xFE00) == 0x7800) ? 0 : 1);
}

/*___________________________________________________________________
**      function name: macMT7530GetEthPortCongestion
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/08/
**_______________________________________________________________
*/
int macMT7530GetEthPortCongestion(u8 port)
{
	static u32 PrevPauseCnt[MT7530_TOTAL_PORTS][2];	//{MIB_ID_TX_PAUSE_CNT, MIB_ID_RX_PAUSE_CNT}
	u32 reg, value, tx_pause_cnt, rx_pause_cnt;

	if (port >= MT7530_TOTAL_PORTS) return (-1);

	//reg = 0x4000 + (0x100 * port) + (MIB_ID_TX_PAUSE_CNT * 4);
	reg = MIB_ID_TX_PAUSE_CNT + (0x100 * port);
	tx_pause_cnt = macMT7530ReadReg(reg);

	//reg = 0x4000 + (0x100 * port) + (MIB_ID_RX_PAUSE_CNT * 4);
	reg = MIB_ID_RX_PAUSE_CNT + (0x100 * port);
	rx_pause_cnt = macMT7530ReadReg(reg);

	value =  ((tx_pause_cnt != PrevPauseCnt[port][0]) || (rx_pause_cnt != PrevPauseCnt[port][1])) ? 1 : 0;
	PrevPauseCnt[port][0] = tx_pause_cnt;
	PrevPauseCnt[port][1] = rx_pause_cnt;

	return (value);
}

/*___________________________________________________________________
**      function name: macMT7530GetTxTimestamp
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/08/01
**_______________________________________________________________
*/
int macMT7530GetTxTimestamp(mt7530_switch_api_timestamp_fifo *outptr_tx_ts)
{
	u32 reg, value;

	reg = 0x1F90;
	value = macMT7530ReadReg(reg);
	if (value & (1 << 6)) return (-1);	//TMSTMP_TFIFO_EMPTY 0x1F90[6]

	reg = 0x1F98;
	value = macMT7530ReadReg(reg);
	outptr_tx_ts->seq_id = (value >> 16) & 0x07;	//TMSTMP_TFIFO_SEQ 0x1F98[18:16]
	outptr_tx_ts->msg_type = value & 0x03;		//TMSTMP_TFIFO_SEQ 0x1F98[1:0]

	reg = 0x1F9C;
	value = macMT7530ReadReg(reg);
	(outptr_tx_ts->tstamp).seconds = (value >> 28) & 0x0F;		//TMSTMP_TFIFO_S_LSB 0x1F9C[31:28]
	(outptr_tx_ts->tstamp).nanoseconds = (value & 0x0FFFFFFF) << 2;	//TMSTMP_TFIFO_NS 0x1F9C[27:0]

	reg = 0x1F90;
	value = 0x0001;
	macMT7530WriteReg(reg, value);		//TMSTMP_TFIFO_LOAD 0x1F90[0]

	return (0);
}

/*___________________________________________________________________
**      function name: macMT7530GetRxTimestamp
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/08/01
**_______________________________________________________________
*/
int macMT7530GetRxTimestamp(mt7530_switch_api_timestamp_fifo *outptr_rx_ts)
{
	u32 reg, value;

	reg = 0x1FA0;
	value = macMT7530ReadReg(reg);
	if (value & (1 << 6)) return (-1);	//TMSTMP_RFIFO_EMPTY 0x1FA0[6]

	reg = 0x1FA8;
	value = macMT7530ReadReg(reg);
	outptr_rx_ts->seq_id = (value >> 16) & 0x07;	//TMSTMP_RFIFO_SEQ 0x1FA8[18:16]
	outptr_rx_ts->msg_type = value & 0x03;		//TMSTMP_RFIFO_SEQ 0x1FA8[1:0]

	reg = 0x1FAC;
	value = macMT7530ReadReg(reg);
	(outptr_rx_ts->tstamp).seconds = (value >> 28) & 0x0F;		//TMSTMP_RFIFO_S_LSB 0x1FAC[31:28]
	(outptr_rx_ts->tstamp).nanoseconds = (value & 0x0FFFFFFF) << 2;	//TMSTMP_RFIFO_NS 0x1FAC[27:0]

	reg = 0x1FA0;
	value = 0x0001;
	macMT7530WriteReg(reg, value);		//TMSTMP_RFIFO_LOAD 0x1FA0[0]

	return (0);
}

/*___________________________________________________________________
**      function name: macMT7530GetCurrTime
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/08/01
**_______________________________________________________________
*/
int macMT7530GetCurrTime(mt7530_switch_api_timestamp *outptr_ts)
{
	u32 reg, value;

	reg = 0x1F60;
	value = macMT7530ReadReg(reg);		// read PTP_CTRL to latch TIMESTAMP_LSB
	if ((value & (1 << 31)) == 0) return (-1);	//TIMESTAMP_EN 0x1F60[31]

	reg = 0x1F68;
	value = macMT7530ReadReg(reg);
	outptr_ts->seconds = (value >> 28) & 0x0F;		//TMSTMP_S_LSB 0x1F68[31:28]
	outptr_ts->nanoseconds = (value & 0x0FFFFFFF) << 2;	//TMSTMP_NS 0x1F68[27:0]

	return (0);
}

/*___________________________________________________________________
**      function name: macMT7530SetCurrTime
**      descriptions:
**         ...
**
**      parameters:
**        type: 0(immediate); 1(next 1pps)
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/08/01
**_______________________________________________________________
*/
int macMT7530SetCurrTime(int type, mt7530_switch_api_timestamp *ts)
{
	u32 reg, value, ts4ns;

	ts4ns = ts->nanoseconds >> 2;
	if ((ts->seconds > 0x0F) || (ts4ns > 0x0FFFFFFF)) return (-1);

	reg = 0x1F70;
//20120927 YM_Chan : spec update
//spec update	value = ((type) ? (1 << 2) : 0) | (0 << 0);		//TMSTMP_OFFSET_MODE 0x1F70[0] = 0 : Absolute mode; TMSTMP_ABS_MODE 0x1F70[2] = (0:next clock; 1:next 1pps)
	value = ((type) ? (1 << 2) : 0) | (1 << 0);		//TMSTMP_OFFSET_MODE 0x1F70[0] = 1 : Absolute mode; TMSTMP_ABS_MODE 0x1F70[2] = (0:next clock; 1:next 1pps)
	macMT7530WriteReg(reg, value);

	reg = 0x1F78;
	value = (ts->seconds << 28) | ts4ns;			//TMSTMP_OFFSET_LSB 0x1F78[31:28], TMSTMP_NS_OFFSET 0x1F78[27:0]
	macMT7530WriteReg(reg, value);

	return (0);
}

/*___________________________________________________________________
**      function name: macMT7530SetOffset
**      descriptions:
**         ...
**
**      parameters:
**         1:add, 0:sub
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/08/01
**_______________________________________________________________
*/
int macMT7530SetOffset(int opt, mt7530_switch_api_timestamp *ts)
{
	u32 reg, value, ts4ns;

	reg = 0x1F70;
//20120927 YM_Chan : spec update
//spec update	value = (1 << 0);		//TMSTMP_OFFSET_MODE 0x1F70[0] = 1 : Add/Sub mode
	value = (0 << 0);		//TMSTMP_OFFSET_MODE 0x1F70[0] = 0 : Add/Sub mode
	macMT7530WriteReg(reg, value);

	reg = 0x1F78;
	if (opt)	//opt=1:add
	{
		ts4ns = ts->nanoseconds >> 2;
		if (ts4ns > 0x0FFFFFFF) return (-1);
		value = 0 | ts4ns;			//TMSTMP_OFFSET_LSB 0x1F78[31:28] = 0(Add mode), 0xF(Sub mode)
							//TMSTMP_NS_OFFSET 0x1F78[27:0]
	}
	else	//opt=1:sub
	{
		ts4ns = (1000000000L - ts->nanoseconds) >> 2;
		if (ts4ns > 0x0FFFFFFF) return (-1);
		value = (0xF << 28) | ts4ns;		//TMSTMP_OFFSET_LSB 0x1F78[31:28] = 0(Add mode), 0xF(Sub mode)
							//TMSTMP_NS_OFFSET 0x1F78[27:0]
	}
	macMT7530WriteReg(reg, value);

	return (0);
}

/*___________________________________________________________________
**      function name: macMT7530GetPTPState
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/08/03
**_______________________________________________________________
*/
int macMT7530GetPTPState(int *outptr_EnablePort)
{
	u32 reg, value;
	u8 i;

	reg = 0x1F60;
	value = macMT7530ReadReg(reg);
	if (((value & (1 << 31)) == 0) || ((value & 0x7F) == 0))	//TIMESTAMP_EN 0x1F60[31]; PTPn_EN 0x1F60[6:0]
	{
		(*outptr_EnablePort) = -1;
	}
	else
	{
		value &= 0x7F;
		for (i = 0; i < MT7530_TOTAL_PORTS; i ++)
		{
			if (value & 1)
			{
				(*outptr_EnablePort) = i;
				break;
			}
			value >>= 1;
		}
	}

	return (0);
}

/*___________________________________________________________________
**      function name: macMT7530SetPTPState
**      descriptions:
**         ...
**
**      parameters:
**         -1 : disable all port 1588 feature
**         0~6 : select port to enable 1588 feature
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/08/03
**_______________________________________________________________
*/
int macMT7530SetPTPState(int EnablePort)
{
	u32 reg, value;

	if ((EnablePort >= MT7530_TOTAL_PORTS) || (EnablePort < -1)) return (-1);

	reg = 0x1F60;
	value = macMT7530ReadReg(reg);
	value &= ~((1 << 31) | 0x7F);
	if (EnablePort != -1)
	{
		value |= (1 << 31) | (1 << EnablePort);		//TIMESTAMP_EN 0x1F60[31]; PTPn_EN 0x1F60[6:0]
	}
	macMT7530WriteReg(reg, value);

	return (0);
}
static mt7530_switch_api_trtcm_acl_t TrtcmAclSetting;
/*___________________________________________________________________
**      function name: macMT7530SetDsTrtcmEnable
**      descriptions:
**         ...
**
**      parameters:
**         enable : 
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. Andy Yi 2014/03/24
**_______________________________________________________________
*/
int macMT7530SetDsTrtcmEnable(u8 enable)
{
	u32 reg, value = 0;
	int rule, i = 0;

	if ((!isMT7525G) && (!isMT7520G))
		return 0;

	memset(&TrtcmAclSetting, 0xff, sizeof(TrtcmAclSetting));
	TrtcmAclSetting.enable = enable ? 1 : 0;

	if (enable)
	{		
		/* internal switch trtcm enable */
		reg = 0x9c ;
		value = read_reg_word(GSW_BASE+reg);
		value = (value | (1 << 31));
		write_reg_word(GSW_BASE+reg, value);			

		/* internal switch port5 Q0-Q7 control 10(yellow)-11(red)*/
		for (i = 0; i < 8; i++)
		{
			reg = 0x1d0c + i * 4;
			value = 0x20000;
			write_reg_word(GSW_BASE+reg, value);						
		}
		
		reg = 0x1d0c ;
		value = read_reg_word(GSW_BASE+reg);
		value = (value | (1 << 31));
		write_reg_word(GSW_BASE+reg, value);

		for (i = 0; i < 8; i++)
		{
			reg = 0x1d2c + i * 4;
			value = 0;
			write_reg_word(GSW_BASE+reg, value);						
		}
		
		reg = 0x1d2c ;
		value = read_reg_word(GSW_BASE+reg);
		value = (value | (1 << 31));
		write_reg_word(GSW_BASE+reg, value);	

		/* enable internal switch port 6 ACL, and set 32 acl rule */
		reg = 0x2604 ;
		value = read_reg_word(GSW_BASE+reg);
		value = (value | (1 << 10));
		write_reg_word(GSW_BASE+reg, value);

		for (rule = 0; rule < TRTCM_RULE_NUM; rule++)
		{
			reg = 0x94;
			value = 0x7c000000 + (rule << 9);
			write_reg_word(GSW_BASE+reg, value);

			reg = 0x98;
			value = 0x8400c;
			write_reg_word(GSW_BASE+reg, value);

			reg = 0x90;
			value = 0x80005000 + rule;
			write_reg_word(GSW_BASE+reg, value);
		}

		for (rule = 0; rule < TRTCM_RULE_NUM; rule++)
		{
			reg = 0x94;
			value = (1 << rule);
			write_reg_word(GSW_BASE+reg, value);

			reg = 0x98;
			value = 0x0;
			write_reg_word(GSW_BASE+reg, value);

			reg = 0x90;
			value = 0x80009000 + rule;
			write_reg_word(GSW_BASE+reg, value);
		}

		for (rule = 0; rule < TRTCM_RULE_NUM; rule++)
		{
			reg = 0x94;
			if (rule < 8)
				value = 0x18000000 + (rule << 24) + (rule << 4);
			else
				value = 0x10000000 ;
				
			write_reg_word(GSW_BASE+reg, value);

			reg = 0x98;
			value = 0x00010000 + (rule << 18);
			write_reg_word(GSW_BASE+reg, value);

			reg = 0x90;
			value = 0x8000b000 + rule;
			write_reg_word(GSW_BASE+reg, value);
		}		

		/* trtcm value set to default 0xFFFF */
		for (rule = 0; rule < TRTCM_RULE_NUM; rule++)
		{
			reg = 0x94;
			value = 0xffffffff;
			write_reg_word(GSW_BASE+reg, value);
			
			reg = 0x98;
			value = 0xffffffff;
			write_reg_word(GSW_BASE+reg, value);

			reg = 0x90;
			value = 0x80007000 + rule;
			write_reg_word(GSW_BASE+reg, value);
		}
	}
	else
	{
		/* internal switch trtcm disable */
		reg = 0x9c ;
		value = read_reg_word(GSW_BASE+reg);
		value &= (~(1 << 31));
		write_reg_word(GSW_BASE+reg, value);	
		
		/*restore internal switch port 5 Q0-Q7 control 10-11 */
		for (i = 0; i < 8; i++)
		{
			reg = 0x1d0c + i * 4;
			value = 0;
			write_reg_word(GSW_BASE+reg, value);						
		}

		for (i = 0; i < 8; i++)
		{
			reg = 0x1d2c + i * 4;
			value = 0;
			write_reg_word(GSW_BASE+reg, value);						
		}
	
		/* undo internal switch ACL rule */
		reg = 0x2604 ;
		value = read_reg_word(GSW_BASE+reg);
		value &= (~(1 << 10));
		write_reg_word(GSW_BASE+reg, value);
	}

	return 0;
}
/*___________________________________________________________________
**      function name: macMT7530GetDsTrtcm
**      descriptions:
**         ...
**
**      parameters:
**         rate : rate <kbps>
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. Andy Yi 2014/03/11
**_______________________________________________________________
*/
int macMT7530GetDsTrtcm(mt7530_switch_api_trtcm_acl_t *trtcm_acl_p)
{
	u32 reg, value = 0;
	int i = 0;

	if ((!isMT7525G) && (!isMT7520G))
		return 0;

	if (trtcm_acl_p == NULL)
		return -1;
	
	trtcm_acl_p->enable = TrtcmAclSetting.enable;
	for (i = 0; i < TRTCM_RULE_NUM; i++)
	{	
		trtcm_acl_p->trtcm_acl_table[i].trtcmId = TrtcmAclSetting.trtcm_acl_table[i].trtcmId;
		trtcm_acl_p->trtcm_acl_table[i].CBS = TrtcmAclSetting.trtcm_acl_table[i].CBS;
		trtcm_acl_p->trtcm_acl_table[i].PBS = TrtcmAclSetting.trtcm_acl_table[i].PBS;
		trtcm_acl_p->trtcm_acl_table[i].CIR = TrtcmAclSetting.trtcm_acl_table[i].CIR;
		trtcm_acl_p->trtcm_acl_table[i].PIR = TrtcmAclSetting.trtcm_acl_table[i].PIR;
	}
	
	return 0;
}

/*___________________________________________________________________
**      function name: macMT7530SetDsTrtcm
**      descriptions:
**         ...
**
**      parameters:
**         rate : rate <kbps>
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. Andy Yi 2014/03/11
**_______________________________________________________________
*/
int macMT7530SetDsTrtcm(mt7530_switch_api_trtcm_t *trtcm_p)
{
	u32 reg, value = 0;

	if ((!isMT7525G) && (!isMT7520G))
		return 0;

	if (trtcm_p == NULL)
		return -1;

	if (trtcm_p->trtcmId > TRTCM_RULE_NUM)
		return -1;
	
	TrtcmAclSetting.trtcm_acl_table[trtcm_p->trtcmId].trtcmId = trtcm_p->trtcmId;
	TrtcmAclSetting.trtcm_acl_table[trtcm_p->trtcmId].CBS = trtcm_p->CBS;
	TrtcmAclSetting.trtcm_acl_table[trtcm_p->trtcmId].PBS = trtcm_p->PBS;
	TrtcmAclSetting.trtcm_acl_table[trtcm_p->trtcmId].CIR = trtcm_p->CIR;
	TrtcmAclSetting.trtcm_acl_table[trtcm_p->trtcmId].PIR = trtcm_p->PIR;
	
	reg = 0x94;
	value = ((trtcm_p->CBS << 16) | (trtcm_p->PBS));
	write_reg_word(GSW_BASE+reg, value);
		
	reg = 0x98;
	value = ((trtcm_p->CIR << 16) | (trtcm_p->PIR));
	write_reg_word(GSW_BASE+reg, value);
	
	reg = 0x90;
	value = 0x80007000 + trtcm_p->trtcmId;
	write_reg_word(GSW_BASE+reg, value);

	return 0;
}


/*___________________________________________________________________
**      function name: macMT7530SetDsP5RateLimitEnable
**      descriptions:
**         ...
**
**      parameters:
**         enable : 
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. Andy Yi 2014/03/11
**_______________________________________________________________
*/
int macMT7530SetDsP5RateLimitEnable(u8 enable)
{
	u32 reg, value = 0;

	if ((!isMT7525G) && (!isMT7520G))
		return 0;
	
	reg = 0x1540 ;
	value = read_reg_word(GSW_BASE+reg);
	value = (enable) ? (value | (1 << 15)) : (value & ~(1 << 15));
	write_reg_word(GSW_BASE+reg, value);	

	return 0;
}

/*___________________________________________________________________
**      function name: macMT7530SetDsP5RateLimit
**      descriptions:
**         ...
**
**      parameters:
**         rate : rate <kbps>
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. Andy Yi 2014/03/11
**_______________________________________________________________
*/
int macMT7530SetDsP5RateLimit(u32 rate)
{
	u32 reg, value = 0;

	if ((!isMT7525G) && (!isMT7520G))
		return 0;
	
	reg = 0x1540 ;
	value = read_reg_word(GSW_BASE+reg);
	value = ((rate/32)<<16) + (1<<15) + (0<<8) + (1<<7) + 0xf;
	write_reg_word(GSW_BASE+reg, value);	

	reg = 0x10E0;
	value = macMT7530ReadReg(reg) & 0xFFFFFF00;
	value |= 0x18;
	macMT7530WriteReg(reg, value);

	return 0;
}


/*___________________________________________________________________
**      function name: macMT7530SetDsPort5QosEnable
**      descriptions:
**         ...
**
**      parameters:
**         0 : disable internal switch port5 qos feature
**         1 : enable internal switch port5 qos feature
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. Andy Yi 2014/03/11
**_______________________________________________________________
*/
int macMT7530SetDsP5Qos(u8 enable)
{
	u32 reg, value;
	int queue = 0, rule = 0;

	if ((!isMT7525G) && (!isMT7520G))
		return 0;

	if (enable)
	{
		fcStatus=0x80;
		/* close flow control*/
		reg = 0x1fe0 ;
		value = read_reg_word(GSW_BASE+reg);
		fcStatus |= (value & (1<<31)) ? (1<<0) : 0;
		value &= ~(1<<31);
		write_reg_word(GSW_BASE+reg, value);

		reg = 0x3600 ;
		value = read_reg_word(GSW_BASE+reg);
		fcStatus |= (value & (3<<4)) ? (1<<1) : 0;
		value &= ~(3<<4);
		write_reg_word(GSW_BASE+reg, value);	

		reg = 0x3500 ;
		value = read_reg_word(GSW_BASE+reg);
		fcStatus |= (value & (3<<4)) ? (1<<2) : 0;
		value &= ~(3<<4);
		write_reg_word(GSW_BASE+reg, value);

		reg = 0x1fe0 ;
		value = macMT7530ReadReg(reg);
		fcStatus |= (value & (1<<31)) ? (1<<3) : 0;
		value &= ~(1<<31);
		macMT7530WriteReg(reg, value);

		reg = 0x3600 ;
		value = macMT7530ReadReg(reg);
		fcStatus |= (value & (3<<4)) ? (1<<4) : 0;
		value &= ~(3<<4);
		macMT7530WriteReg(reg, value);		

		/* internal switch priority based acl */	
		reg = 0x44;
		value = read_reg_word(GSW_BASE+reg);
		value &= (~((0x7<<0) | (0x7<<4) | (0x7<<8) | (0x7<<12) | (0x7<<16)));
		value |= ((0x7<<0) | (0x2<<4) | (0x2<<8) | (0x2<<12) | (0x2<<16));
		write_reg_word(GSW_BASE+reg, value);

		/* internal switch port 5 scheduler method is SP */
		for (queue=0; queue<8; queue++)
		{
			reg = 0x1500 + queue*8;
			value = read_reg_word(GSW_BASE+reg);
			value &= (0x0);
			value |= (1<<31);
			write_reg_word(GSW_BASE+reg, value);

			reg = 0x1504 + queue*8;
			value = read_reg_word(GSW_BASE+reg);
			value &= (0x0);
			write_reg_word(GSW_BASE+reg, value);			
		}

		/* enable internal switch port 6 ACL, and set eight acl rule */
		reg = 0x2604;
		value = read_reg_word(GSW_BASE+reg);
		value |= (1<<10);
		write_reg_word(GSW_BASE+reg, value);

		for (rule = 0; rule < 8; rule++)
		{
			reg = 0x94;
			value = 0xe0000000 + (rule <<13);
			write_reg_word(GSW_BASE+reg, value);

			reg = 0x98;
			value = 0x94000;
			write_reg_word(GSW_BASE+reg, value);

			reg = 0x90;
			value = 0x80005000 + rule;
			write_reg_word(GSW_BASE+reg, value);
		}

		for (rule = 0; rule < 8; rule++)
		{
			reg = 0x94;
			value = (1 << rule);
			write_reg_word(GSW_BASE+reg, value);

			reg = 0x98;
			value = 0x0;
			write_reg_word(GSW_BASE+reg, value);

			reg = 0x90;
			value = 0x80009000 + rule;
			write_reg_word(GSW_BASE+reg, value);
		}

		for (rule = 0; rule < 8; rule++)
		{
			reg = 0x94;
			value = 0x18000000 + (rule << 24) + (rule << 4);
			write_reg_word(GSW_BASE+reg, value);

			reg = 0x98;
			value = 0x0;
			write_reg_word(GSW_BASE+reg, value);

			reg = 0x90;
			value = 0x8000b000 + rule;
			write_reg_word(GSW_BASE+reg, value);
		}		
	}
	else
	{
		/* restore flow control value */
		if (fcStatus & 0x80)
		{
			fcStatus &= ~(0x80);
			reg = 0x1fe0 ;
			value = read_reg_word(GSW_BASE+reg);
			if (fcStatus & (1<<0))
				value |= (1<<31);
			else
				value &= ~(1<<31);
			write_reg_word(GSW_BASE+reg, value);

			reg = 0x3600 ;
			value = read_reg_word(GSW_BASE+reg);
			if (fcStatus & (1<<1))
				value |= (3<<4);
			else
				value &= ~(3<<4);
			write_reg_word(GSW_BASE+reg, value);	

			reg = 0x3500 ;
			value = read_reg_word(GSW_BASE+reg);
			if (fcStatus & (1<<2))
				value |= (3<<4);
			else
				value &= ~(3<<4);
			write_reg_word(GSW_BASE+reg, value);

			reg = 0x1fe0 ;
			value = macMT7530ReadReg(reg);
			if (fcStatus & (1<<3))
				value |= (1<<31);
			else
				value &= ~(1<<31);
			macMT7530WriteReg(reg, value);

			reg = 0x3600 ;
			value = macMT7530ReadReg(reg);
			if (fcStatus & (1<<4))
				value |= (3<<4);
			else
				value &= ~(3<<4);
			macMT7530WriteReg(reg, value);		
		}
		
		/* restore internal switch port 5 scheduler method*/
		for (queue=0; queue<8; queue++)
		{
			reg = 0x1500 + queue*8;
			value = read_reg_word(GSW_BASE+reg);
			value &= (0x0);
			write_reg_word(GSW_BASE+reg, value);
		
			reg = 0x1504 + queue*8;
			value = read_reg_word(GSW_BASE+reg);
			value &= (0x0);
			write_reg_word(GSW_BASE+reg, value); 		
		}

		/* disable internal switch port 6 ACL */
		reg = 0x2604;
		value = read_reg_word(GSW_BASE+reg);
		value &= ~(1<<10);
		write_reg_word(GSW_BASE+reg, value);
	}

	return (0);
}


int switch_port_mask=0x7f;
#define MAX_SWITCH_PORT_NUM 7

/*___________________________________________________________________
**      function name: macMT7530SetStormCtrlPort
**      descriptions:
**         ...
**
**      parameters:
**         0x00~0x7f: bit0~bit6 means port0~port6,default value is 0x7f
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. kenny 2014/03/11
**_______________________________________________________________
*/

int macMT7530SetStormCtrlPort(u16 port)
{
	if ((!isMT7525G) && (!isMT7520G))
		return -1;
	
	if(port > 0x7f)
		return -1;
	
	switch_port_mask = port;

	printk("set switch port_mask: 0x%x\r\n", switch_port_mask);
	return (0);
}




/*___________________________________________________________________
**      function name: macMT7530SetStormCtrlUnit
**      descriptions:
**         ...
**
**      parameters:
**         0 : 64 packet per second
**         1 : 256 packets per second
**         2 : 1 k packet per second
**         3 : 4 k packets per second
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. kenny 2014/03/11
**_______________________________________________________________
*/

int macMT7530SetStormCtrlUnit(u8 unit)
{
	u32 reg, value;
	int i=0;

	if ((!isMT7525G) && (!isMT7520G))
		return -1;
	
	if(unit > 3)
		return -1;
	
	for(i=0; i<MAX_SWITCH_PORT_NUM; i++)
	{
		if(switch_port_mask & (1<<i))
		{
			reg = 0x201c + (i*0x100);
			value = macMT7530ReadReg(reg);
			
//			printk("1 set storm contrl unit: 0x%lx\r\n", value);
			value &= ~(3 << 24);
			
//			printk("2 set storm contrl unit: 0x%lx\r\n", value);
			value |= (unit << 24);

//			printk("3 set storm contrl unit: 0x%lx\r\n", value);
			macMT7530WriteReg(reg, value);
		}

	}
	printk("set storm contrl unit: 0x%x\r\n", unit);
	return (0);
}


/*___________________________________________________________________
**      function name: macMT7530SetStormCtrlType
**      descriptions:
**         ...
**
**      parameters:
**         4 : broadcast
**         2 : multicast
**         1 : unknow mac unicast
**       
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. kenny 2014/03/11
**_______________________________________________________________
*/

int macMT7530SetStormCtrlType(u8 type)
{
	u32 reg, value;
	int i=0;

	if ((!isMT7525G) && (!isMT7520G))
		return -1;

	if(type > 7)
		return -1;
	
	for(i=0; i<MAX_SWITCH_PORT_NUM; i++)
	{
		if(switch_port_mask & (1<<i))
		{
			reg = 0x201c + (i*0x100);
			value = macMT7530ReadReg(reg);

			value &= ~(7 << 28);
			value |= (type << 28);

			macMT7530WriteReg(reg, value);
		}

	}
	printk("set storm contrl type: 0x%x\r\n", type);

	return (0);
}


/*___________________________________________________________________
**      function name: macMT7530SetStormCtrlMode
**      descriptions:
**         ...
**
**      parameters:
**         0 : Packet-base
**         1 : rate-base
**       
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. kenny 2014/03/11
**_______________________________________________________________
*/

int macMT7530SetStormCtrlMode(u8 mode)
{
	u32 reg, value;
	int i=0;

	if ((!isMT7525G) && (!isMT7520G))
		return -1;

	if(mode > 1)
		return -1;
	
	for(i=0; i<MAX_SWITCH_PORT_NUM; i++)
	{
		if(switch_port_mask & (1<<i))
		{
			reg = 0x201c + (i*0x100);
			value = macMT7530ReadReg(reg);

			value &= ~(1 << 31);
			value |= (mode << 31);

			macMT7530WriteReg(reg, value);
		}

	}
	printk("set storm contrl mode: 0x%x\r\n", mode);
	return (0);
}


/*___________________________________________________________________
**      function name: macMT7530SetStormCtrlPeriod
**      descriptions:
**         ...
**
**      parameters:
**         0 : 1 second
**         1 : 125 us
**       
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. kenny 2014/03/11
**_______________________________________________________________
*/

int macMT7530SetStormCtrlPeriod(u8 period)
{
	u32 reg, value;
	int i=0;

	if ((!isMT7525G) && (!isMT7520G))
		return -1;

	if(period > 1)
		return -1;
	
	for(i=0; i<MAX_SWITCH_PORT_NUM; i++)
	{
		if(switch_port_mask & (1<<i))
		{
			reg = 0x201c + (i*0x100);
			value = macMT7530ReadReg(reg);

			value &= ~(1 << 26);
			value |= (period << 26);

			macMT7530WriteReg(reg, value);
		}

	}
	printk("set storm contrl period: 0x%x\r\n", period);

	return (0);
}


/*___________________________________________________________________
**      function name: macMT7530VLan_Entry_Enable
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/09/17
**_______________________________________________________________
*/
int macMT7530VLan_Entry_Enable(u16 vid, u8 sw)
{
	u32 reg, value, timeout;

	if ((vid >= 4096) || (vid == 0)) return (-1);

	reg = 0x90;	// VTCR
	timeout = 100000;
	while (-- timeout) {
		value = macMT7530ReadReg(reg);		// VTCR
		if ((value & (1 << 31)) == 0) break;	// table busy finish?
		if (timeout == 0) return (-1);		// timeout?
	}

	if (sw)
	{
		value = (1 << 31) | (3 << 12) | vid;	// 3 : validate specific VLAN entry
	}
	else
	{
		value = (1 << 31) | (2 << 12) | vid;	// 2 : invalidate specific VLAN entry
	}
	macMT7530WriteReg(reg, value);
	timeout = 100000;
	while (-- timeout) {
		value = macMT7530ReadReg(reg);		// VTCR
		if ((value & (1 << 31)) == 0) break;	// table busy finish?
		if (timeout == 0) return (-1);		// timeout?
	}

	return (0);
}
EXPORT_SYMBOL(macMT7530VLan_Entry_Enable);

/*___________________________________________________________________
**      function name: macMT7530VLan_Entry_SetETag
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/09/17
**_______________________________________________________________
*/
int macMT7530VLan_Entry_SetETag(u16 vid, u8 port, u8 etag)
{
	u32 reg, value, value_vawd1, value_vawd2, timeout;

	if ((vid >= 4096) || (vid == 0) || (port >= MT7530_TOTAL_PORTS) || (etag >= 4)) return (-1);

	reg = 0x90;	// VTCR
	timeout = 100000;
	while (-- timeout) {
		value = macMT7530ReadReg(reg);		// VTCR
		if ((value & (1 << 31)) == 0) break;	// table busy finish?
		if (timeout == 0) return (-1);		// timeout?
	}

	value = (1 << 31) | (0 << 12) | vid;	// 0 : read specific VLAN entry
	macMT7530WriteReg(reg, value);
	timeout = 100000;
	while (-- timeout) {
		value = macMT7530ReadReg(reg);		// VTCR
		if ((value & (1 << 31)) == 0) break;	// table busy finish?
		if (timeout == 0) return (-1);		// timeout?
	}
	if (value & (1 << 16)) return (-1);	// means this index is out of valid index

	value_vawd1 = macMT7530ReadReg(0x94);
	value_vawd2 = macMT7530ReadReg(0x98);

	value_vawd1 = (value_vawd1 & ~(1 << 29) ) | (1 << 28);	// VTAG_EN(b28)=1, EG_CON(b29)=0
	value_vawd2 = (value_vawd2 & ~(3 << (port * 2))) | (etag << (port * 2));	// set EG_TAG (2 bits per port)

	macMT7530WriteReg(0x94, value_vawd1);
	macMT7530WriteReg(0x98, value_vawd2);

	value = (1 << 31) | (1 << 12) | vid;	// 1 : write specific VLAN entry
	macMT7530WriteReg(reg, value);
	timeout = 100000;
	while (-- timeout) {
		value = macMT7530ReadReg(reg);		// VTCR
		if ((value & (1 << 31)) == 0) break;	// table busy finish?
		if (timeout == 0) return (-1);		// timeout?
	}

	return (0);
}
EXPORT_SYMBOL(macMT7530VLan_Entry_SetETag);

/*___________________________________________________________________
**      function name: macMT7530VLan_Entry_GetETag
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/09/17
**_______________________________________________________________
*/
int macMT7530VLan_Entry_GetETag(u16 vid, u8 port, u8* outptr_etag)
{
	u32 reg, value, value_vawd1, value_vawd2, timeout;

	if ((vid >= 4096) || (vid == 0) || (port >= MT7530_TOTAL_PORTS)) return (-1);

	reg = 0x90;	// VTCR
	timeout = 100000;
	while (-- timeout) {
		value = macMT7530ReadReg(reg);		// VTCR
		if ((value & (1 << 31)) == 0) break;	// table busy finish?
		if (timeout == 0) return (-1);		// timeout?
	}

	value = (1 << 31) | (0 << 12) | vid;	// 0 : read specific VLAN entry
	macMT7530WriteReg(reg, value);
	timeout = 100000;
	while (-- timeout) {
		value = macMT7530ReadReg(reg);		// VTCR
		if ((value & (1 << 31)) == 0) break;	// table busy finish?
		if (timeout == 0) return (-1);		// timeout?
	}
	if (value & (1 << 16)) return (-1);	// means this index is out of valid index

	value_vawd1 = macMT7530ReadReg(0x94);
	value_vawd2 = macMT7530ReadReg(0x98);

	if (!(value_vawd1 & (1 << 28)) || (value_vawd1 & (1 << 29))) return (-1);	// VTAG_EN(b28)=1, EG_CON(b29)=0

	(*outptr_etag) = (value_vawd2 >> (port * 2)) & 3;	// EG_TAG (2 bits per port)

	return (0);
}
EXPORT_SYMBOL(macMT7530VLan_Entry_GetETag);

/*___________________________________________________________________
**      function name: macMT7530VLan_Entry_SetSTag
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/09/17
**_______________________________________________________________
*/
int macMT7530VLan_Entry_SetSTag(u16 vid, u8 port, u8 stag)
{
	u32 reg, value, value_vawd1, value_vawd2, timeout;
	u8 bit_offset;

	if ((vid >= 4096) || (vid == 0) || (port >= MT7530_TOTAL_PORTS) || (stag >= 4)) return (-1);

	reg = 0x90;	// VTCR
	timeout = 100000;
	while (-- timeout) {
		value = macMT7530ReadReg(reg);		// VTCR
		if ((value & (1 << 31)) == 0) break;	// table busy finish?
		if (timeout == 0) return (-1);		// timeout?
	}

	value = (1 << 31) | (0 << 12) | vid;	// 0 : read specific VLAN entry
	macMT7530WriteReg(reg, value);
	timeout = 100000;
	while (-- timeout) {
		value = macMT7530ReadReg(reg);		// VTCR
		if ((value & (1 << 31)) == 0) break;	// table busy finish?
		if (timeout == 0) return (-1);		// timeout?
	}
	if (value & (1 << 16)) return (-1);	// means this index is out of valid index

	value_vawd1 = macMT7530ReadReg(0x94);
	value_vawd2 = macMT7530ReadReg(0x98);

	#if (0)	//20120918 Lidong_Hu confirm do not need following checking or setting
	if (~(value_vawd1 & (1 << 31))) return (-1);	// PORT_STAG shall be 1
	#endif
	#if (1)	//20120925 Lidong_Hu again request to set
	value_vawd1 |= (1 << 31);			// PORT_STAG shall be 1
	macMT7530WriteReg(0x94, value_vawd1);
	#endif

	if (port < 4)	// S_TAG1 for port 0~3
	{
		bit_offset = 4 + (port * 3);
		value_vawd1 = (value_vawd1 & ~(7 << bit_offset)) | (stag << bit_offset);	// set S_TAG1 (3 bits per port)
		macMT7530WriteReg(0x94, value_vawd1);
	}
	else		// S_TAG2 for port 4~7
	{
		bit_offset = 16 + ((port - 4) * 3);
		value_vawd2 = (value_vawd2 & ~(7 << bit_offset)) | (stag << bit_offset);	// set S_TAG2 (3 bits per port)
		macMT7530WriteReg(0x98, value_vawd2);
	}

	value = (1 << 31) | (1 << 12) | vid;	// 1 : write specific VLAN entry
	macMT7530WriteReg(reg, value);
	timeout = 100000;
	while (-- timeout) {
		value = macMT7530ReadReg(reg);		// VTCR
		if ((value & (1 << 31)) == 0) break;	// table busy finish?
		if (timeout == 0) return (-1);		// timeout?
	}

	return (0);
}
EXPORT_SYMBOL(macMT7530VLan_Entry_SetSTag);

/*___________________________________________________________________
**      function name: macMT7530VLan_Entry_GetSTag
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/09/17
**_______________________________________________________________
*/
int macMT7530VLan_Entry_GetSTag(u16 vid, u8 port, u8* outptr_stag)
{
	u32 reg, value, value_vawd1, value_vawd2, timeout;

	if ((vid >= 4096) || (vid == 0) || (port >= MT7530_TOTAL_PORTS)) return (-1);

	reg = 0x90;	// VTCR
	timeout = 100000;
	while (-- timeout) {
		value = macMT7530ReadReg(reg);		// VTCR
		if ((value & (1 << 31)) == 0) break;	// table busy finish?
		if (timeout == 0) return (-1);		// timeout?
	}

	value = (1 << 31) | (0 << 12) | vid;	// 0 : read specific VLAN entry
	macMT7530WriteReg(reg, value);
	timeout = 100000;
	while (-- timeout) {
		value = macMT7530ReadReg(reg);		// VTCR
		if ((value & (1 << 31)) == 0) break;	// table busy finish?
		if (timeout == 0) return (-1);		// timeout?
	}
	if (value & (1 << 16)) return (-1);	// means this index is out of valid index

	value_vawd1 = macMT7530ReadReg(0x94);
	value_vawd2 = macMT7530ReadReg(0x98);

	#if (1)
	//20120918 Lidong_Hu confirm do not need following checking or setting
	//20120925 Lidong_Hu again request to check
	if (~(value_vawd1 & (1 << 31))) return (-1);	// PORT_STAG shall be 1
	#endif

	if (port < 4)	// S_TAG1 for port 0~3
	{
		(*outptr_stag) = (value_vawd1 >> (4 + (port * 3))) & 7;		// S_TAG1 (3 bits per port)
	}
	else		// S_TAG2 for port 4~7
	{
		(*outptr_stag) = (value_vawd2 >> (16 + ((port - 4) * 3))) & 7;	// S_TAG2 (3 bits per port)
	}

	return (0);
}
EXPORT_SYMBOL(macMT7530VLan_Entry_GetSTag);

/*___________________________________________________________________
**      function name: macMT7530VLan_Port_AddSTag
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/09/17
**_______________________________________________________________
*/
int macMT7530VLan_Port_AddSTag(u8 port, u8 stag, u16 vid)
{
	u32 reg, value;

	if ((vid >= 4096) || (vid == 0) || (port >= MT7530_TOTAL_PORTS) || (stag >= 4)) return (-1);

	reg = 0x2020 + (0x100 * port) + (4 * (stag >> 1));
	value = macMT7530ReadReg(reg);
	if (stag % 2)
	{
		value = (value & ~(0xFFF << 12)) | (vid << 12);
	}
	else
	{
		value = (value & ~0xFFF) | vid;
	}
	macMT7530WriteReg(reg, value);

	return (0);
}
EXPORT_SYMBOL(macMT7530VLan_Port_AddSTag);

/*___________________________________________________________________
**      function name: macMT7530VLan_Entry_ETagMode
**      descriptions:
**         ...
**
**      parameters:
**         0: set vid Entry to EG_CON mode
**         1: set vid Entry to EG_TAG mode
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/09/25
**_______________________________________________________________
*/
int macMT7530VLan_Entry_SetETagMode(u16 vid, u8 etag_mode)
{
	u32 reg, value, value_vawd1, timeout;

	if ((vid >= 4096) || (vid == 0)) return (-1);

	reg = 0x90;	// VTCR
	timeout = 100000;
	while (-- timeout) {
		value = macMT7530ReadReg(reg);		// VTCR
		if ((value & (1 << 31)) == 0) break;	// table busy finish?
		if (timeout == 0) return (-1);		// timeout?
	}

	value = (1 << 31) | (0 << 12) | vid;	// 0 : read specific VLAN entry
	macMT7530WriteReg(reg, value);
	timeout = 100000;
	while (-- timeout) {
		value = macMT7530ReadReg(reg);		// VTCR
		if ((value & (1 << 31)) == 0) break;	// table busy finish?
		if (timeout == 0) return (-1);		// timeout?
	}
	if (value & (1 << 16)) return (-1);	// means this index is out of valid index

	value_vawd1 = macMT7530ReadReg(0x94);
	value_vawd1 &= ~(1 << 29);			// bit29 : EG_CON : "when EG_CON is set, EG_TAG field will be ignored"
	value_vawd1 |= ((etag_mode) ? (0 << 29) : (1 << 29));	// etag_mode = inverse of EG_CON
	macMT7530WriteReg(0x94, value_vawd1);

	value = (1 << 31) | (1 << 12) | vid;	// 1 : write specific VLAN entry
	macMT7530WriteReg(reg, value);
	timeout = 100000;
	while (-- timeout) {
		value = macMT7530ReadReg(reg);		// VTCR
		if ((value & (1 << 31)) == 0) break;	// table busy finish?
		if (timeout == 0) return (-1);		// timeout?
	}

	return (0);
}
EXPORT_SYMBOL(macMT7530VLan_Entry_SetETagMode);

/*___________________________________________________________________
**      function name: macMT7530EnableMIBCounter
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/07/05
**_______________________________________________________________
*/
int macMT7530EnableMIBCounter(u32 enable)
{
	u32 reg, value;

	reg = 0x4FE0;
	value = macMT7530ReadReg(reg);
	value = (value & (~(1 << 31))) | ((enable) ? (1 << 31) : 0);
	macMT7530WriteReg(reg, value);
	if (macMT7530_APIDbgDumpEn) printk("%s() (%08X) : %08X\n", __FUNCTION__, reg, value);
	return (0);
}

/*___________________________________________________________________
**      function name: macMT7530ClearMIBCounter
**      descriptions:
**         ...
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/07/05
**_______________________________________________________________
*/
int macMT7530ClearMIBCounter(void)
{
	u32 reg, value;

	reg = 0x4FE0;
	value = macMT7530ReadReg(reg);
	macMT7530WriteReg(reg, (value & (~(1 << 31))));
	macMT7530WriteReg(reg, (value | (1 << 31)));
	return (0);
}

/*___________________________________________________________________
**      function name: macMT7530SetRxOctMode
**      descriptions:
**         select which mode will count into MIB_ID_RX_OCT_CNT_H/MIB_ID_RX_OCT_CNT_L
**
**      parameters:
**         mt7530_switch_api_RxTxOctMode mode = 1 (OCTMODE_BAD_ONLY)
**                                              2 (OCTMODE_GOOD_ONLY)
**                                              3 (OCTMODE_BAD_GOOD_BOTH)
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/07/05
**_______________________________________________________________
*/
int macMT7530SetRxOctMode(mt7530_switch_api_RxTxOctMode mode)
{
	u32 reg, value;

	if (mode > OCTMODE_BAD_GOOD_BOTH) return (-1);

	reg = 0x4FE0;
	value = macMT7530ReadReg(reg);
	value = (value & ~(3 << 6)) | ((u32)mode << 6);
	macMT7530WriteReg(reg, value);
	return (0);
}

/*___________________________________________________________________
**      function name: macMT7530SetTxOctMode
**      descriptions:
**         select which mode will count into MIB_ID_TX_OCT_CNT_H/MIB_ID_TX_OCT_CNT_L
**
**      parameters:
**         mt7530_switch_api_RxTxOctMode mode = 1 (OCTMODE_BAD_ONLY)
**                                              2 (OCTMODE_GOOD_ONLY)
**                                              3 (OCTMODE_BAD_GOOD_BOTH)
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/07/05
**_______________________________________________________________
*/
int macMT7530SetTxOctMode(mt7530_switch_api_RxTxOctMode mode)
{
	u32 reg, value;

	if (mode > OCTMODE_BAD_GOOD_BOTH) return (-1);

	reg = 0x4FE0;
	value = macMT7530ReadReg(reg);
	value = (value & ~(3 << 4)) | ((u32)mode << 4);
	macMT7530WriteReg(reg, value);
	return (0);
}

/*___________________________________________________________________
**      function name: macMT7530SetBxPktMode
**      descriptions:
**         select which mode will count into BX_PKT_xxx_CNT
**
**      parameters:
**         mt7530_switch_api_BxPktMode mode = 1 (BXMODE_TX_ONLY)
**                                            2 (BXMODE_RX_ONLY)
**                                            3 (BXMODE_TX_RX_BOTH)
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/07/09
**_______________________________________________________________
*/
int macMT7530SetBxPktMode(mt7530_switch_api_BxPktMode mode)
{
#if (MT7530_MIBCNT_VER == 1)
	u32 reg, value;

	if (mode > BXMODE_TX_RX_BOTH) return (-1);

	reg = 0x4FE0;
	value = macMT7530ReadReg(reg);
	value = (value & (~3)) | (u32)mode;
	macMT7530WriteReg(reg, value);
	return (0);
#endif
#if (MT7530_MIBCNT_VER == 2)
	return (-1);
#endif
}

/*___________________________________________________________________
**      function name: macMT7530GetPortPhyAddr
**      descriptions:
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/07/09
**_______________________________________________________________
*/
int macMT7530GetPortPhyAddr(u8 port)
{
	if (port >= MT7530_TOTAL_PORTS) return (-1);

	// PHY_POLL 0x7018[4:0] = PHY_ST_ADDR
	return (port + (macMT7530ReadReg(0x7018) & 0x001F));
}

/*___________________________________________________________________
**      function name: macMT7530APIDbgDumpEn
**      descriptions:
**         ...
**
**      parameters:
**         u8 DbgDump_enable
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/06/21
**_______________________________________________________________
*/
int macMT7530APIDbgDumpEn(u32 DbgDump_enable)
{
	macMT7530_APIDbgDumpEn = DbgDump_enable;
	printk("%s(%d)\n", __FUNCTION__, DbgDump_enable);
	return (0);
}

/*___________________________________________________________________
**      function name: doP6Cal
**      descriptions:
**         ...
**
**      parameters:
**         u8 DbgDump_enable
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**      1. WPLin 2012/06/21
**_______________________________________________________________
*/

#define EXTERNAL 1
#define INTERNAL 0

#define SOC_TO_EXT 0
#define EXT_TO_SOC 1

#define HEAD    1
#define TAIL    2
#define FINISH  3

u32 macMT7530ReadReg2(u32 gsw_reg, u8 external)
{
    if(external){
        return gswPbusRead(gsw_reg);
    }else{
        return read_reg_word(GSW_BASE + gsw_reg);
    }
}

int macMT7530WriteReg2(u32 gsw_reg, u32 gsw_data, u8 external)
{
    if(external){
        return gswPbusWrite(gsw_reg, gsw_data);
    }else{
        write_reg_word((GSW_BASE + gsw_reg), gsw_data);
        return 0;
    }
}

int ErrChk(u32 RegAddr, u8 RxSwitch){
	u32 ErrCnt, RegVal;

	ErrCnt = 0;

	RegVal = macMT7530ReadReg2(RegAddr, RxSwitch);
	
	RegVal = RegVal | 0x40000000;
	macMT7530WriteReg2(RegAddr, RegVal, RxSwitch);

	RegVal = RegVal & 0xBFFFFFFF;
	macMT7530WriteReg2(RegAddr, RegVal, RxSwitch);

	ErrCnt = macMT7530ReadReg2(RegAddr, RxSwitch);
	ErrCnt = (ErrCnt & 0x00000F00) >> 8;

	return ErrCnt;
}

int macMT7530doP6Cal(void)
{
	u32 RxChannel, DAC, index, RxRegAddr, TxRegAddrl, TxRegAddr2, Regvalue, ErrCnt, Val;
    u8 TxSwitch=0,RxSwitch=0,direction=0;
    u8 delay_windows[5][2]={0},state;
    u16 delay_tap=0;
	RxChannel = 0;
	DAC = 0;

    for (direction=0;direction < 2;direction++){
        printk("Direction %s \n",direction?"EXT_TO_SOC":"SOC_TO_EXT");     
        if(direction == SOC_TO_EXT){
            TxSwitch = INTERNAL;
            RxSwitch = EXTERNAL;
        }else{
            TxSwitch = EXTERNAL;
            RxSwitch = INTERNAL;
        }
    	//Tx training mode enable
    	TxRegAddrl = 0x7A40;
    	Regvalue = macMT7530ReadReg2(TxRegAddrl, TxSwitch);
    	Regvalue = Regvalue | 0x80000000;
    	macMT7530WriteReg2(TxRegAddrl, Regvalue, TxSwitch);

    	//Scan
    	for (RxChannel = 0 ; RxChannel <= 4 ; RxChannel++) {
    	    switch(RxChannel){
    	        case 0:
    	            RxRegAddr = 0x7A10;
    				TxRegAddr2 = 0x7A50;
    	            break;
    	        case 1:
    	            RxRegAddr = 0x7A18;
    				TxRegAddr2 = 0x7A58;
    	            break;	            
    	        case 2:
    	            RxRegAddr = 0x7A20;
    				TxRegAddr2 = 0x7A60;
    	            break;
    	        case 3:
    	            RxRegAddr = 0x7A28;
    				TxRegAddr2 = 0x7A68;
    	            break;
    	        default:
    	            RxRegAddr = 0x7A30;
    				TxRegAddr2 = 0x7A70;
    	            break;
    	    }
            state = HEAD;
            for (DAC = 1 ; DAC <= 127 ; DAC++) {
                ErrCnt = 0;
    			Regvalue = macMT7530ReadReg2(TxRegAddr2,TxSwitch);
    			Regvalue = (Regvalue & 0xFFFFFF00) | 0x55;
    			macMT7530WriteReg2(TxRegAddr2, Regvalue, TxSwitch);
    			
    			Regvalue = macMT7530ReadReg2(RxRegAddr, RxSwitch);
    			Regvalue = Regvalue & 0xFFFFFF80;
    			Regvalue = Regvalue | DAC;
    			macMT7530WriteReg2(RxRegAddr, Regvalue, RxSwitch);
    			ErrCnt = ErrChk(RxRegAddr, RxSwitch);
    			Val = (macMT7530ReadReg2(RxRegAddr, RxSwitch) & 0x00FF0000) >> 16;
    			printk("RxChannel=%8x, DAC=%8x, Val=%8x, ErrCnt=%8x \n", 
    			    RxChannel, DAC, Val, ErrCnt);
    			
			    switch(state){
			        case HEAD:
			            if((Val == 0x55) && (ErrCnt == 0)){
			                delay_windows[RxChannel][0]=DAC;
			                state = TAIL;
			            }
			            break;
			        case TAIL:
			            if((Val == 0x55) && (ErrCnt == 0)){
			                delay_windows[RxChannel][1]=DAC;			                
			            }else{
			                state = FINISH;
			            }
			            break;
			        default:
			            break;
                }
    		}
    		Regvalue = macMT7530ReadReg2(RxRegAddr, RxSwitch);
            Regvalue = Regvalue & 0xFFFFFF80;
    		if((state == FINISH) && (delay_windows[RxChannel][0]!=delay_windows[RxChannel][1])){
                delay_tap = (delay_windows[RxChannel][0] + delay_windows[RxChannel][1]) >> 1;
                printk("RxChannel=%8x, delay windows 0x%2X ~ 0x%2X \n",
                    RxChannel, delay_windows[RxChannel][0], delay_windows[RxChannel][1]);
    		}else{
    		    delay_tap = 0;
    		    printk("RxChannel=%8x, delay windows fail, set to default \n",RxChannel);
    		}
            Regvalue = Regvalue | delay_tap;
            macMT7530WriteReg2(RxRegAddr, Regvalue, RxSwitch);
    	}
        
        //Tx training mode disable
    	TxRegAddrl = 0x7A40;
    	Regvalue = macMT7530ReadReg2(TxRegAddrl, TxSwitch);
    	Regvalue = Regvalue & ~0x80000000;
    	macMT7530WriteReg2(TxRegAddrl, Regvalue, TxSwitch);
    }
    
	return 0;
	
}

#if defined(CONFIG_MIPS_RT63365)
#include "tcswitch.h"
/* map to switch ports */
extern char lan_port_map[ RT63365_SWIC_PORTNUM ];
inline u32 macMT7530LanPortMap2Switch(u32 portId){
	int switch_port ;
	if (portId < RT63365_SWIC_PORTNUM-1){
		switch_port = lan_port_map[portId];
		if (0 <= switch_port &&  switch_port < RT63365_SWIC_PORTNUM-1)
			return switch_port;
	}
	return portId;
}
EXPORT_SYMBOL(macMT7530LanPortMap2Switch);

#endif
/*___________________________________________________________________
**      function name: macMT7530gswAPIDispatch
**      descriptions:
**         Called by tc3262_gmac_ioctl(cmd == RAETH_GSW_CTLAPI)
**
**      parameters:
**         struct ifreq *ifr: The arguments number.
**
**      global:
**
**      return:
**         0:succuces
**         -1:fail
**
**      call:
**
**      revision:
**      1. WPLin 2012/06/01
**_______________________________________________________________
*/
int macMT7530gswAPIDispatch(struct ifreq *ifr)
{
	int ret_value = 0;
	mt7530_switch_api_params api_param;
	u32 portId = 0;

#if defined(TCSUPPORT_CUC) || defined(TCSUPPORT_CT_PON_CY)
	int temport;
#endif

	copy_from_user(&api_param, ifr->ifr_data, sizeof(mt7530_switch_api_params));
//	if (macMT7530_APIDbgDumpEn) printk(KERN_DEBUG "%s(%04X)\n", __FUNCTION__, api_param.cmdid);
	if (macMT7530_APIDbgDumpEn) printk("%s(%04X)\n", __FUNCTION__, api_param.cmdid);

	portId = api_param.PortQueueId;
#if defined(CONFIG_MIPS_RT63365)
	if (isMT7520S || isMT7520 || isMT7525 || isMT7520G || isMT7525G){
		portId = macMT7530LanPortMap2Switch(portId);
	}
#endif

#if defined(TCSUPPORT_CUC) || defined(TCSUPPORT_CT_PON_CY)
	temport = portId;
#if defined(TCSUPPORT_CUC_C5_2P)
	if(portId == 1) temport = 4;
	else if(portId == 2) temport = 1;
	else if(portId == 3) temport = 2;
	else if(portId == 4) temport = 3;
	portId = temport;
#endif
#if defined(TCSUPPORT_CT_PON_CY) || defined(TCSUPPORT_CUC_C5_4P)
	if(g_port_reverse && (portId < 5)){
		temport = 5 - portId; 
		portId = temport;
	}
#endif
#endif
	switch (api_param.cmdid)
	{
		// user API start from ID 0x0010
		case CMD_GET_BRGLEARNINGIND :
		{
			ret_value = macMT7530GetBrglearningInd(portId);
		}
			break;

		case CMD_SET_BRGLEARNINGIND :
		{
			ret_value = macMT7530SetBrglearningInd(portId, (u8)(u32)(api_param.paramext_ptr));
		}
			break;

		case CMD_GET_PORTBRGIND :
		{
			u8 ind;
			ret_value = macMT7530GetPortBrgInd(portId, &ind);
			
			if (copy_to_user(api_param.paramext_ptr, &ind, sizeof(ind)))
			{
				ret_value = -1;
//				goto err_exit;
			}
		}
			break;

		case CMD_SET_PORTBRGIND :
		{
			ret_value = macMT7530SetPortBrgInd(portId, (u8)(u32)(api_param.paramext_ptr));
		}
			break;

		case CMD_GET_DISCARDUNKNOWNMACIND :
		{
			ret_value = macMT7530GetDiscardUnknownMacInd(portId);
		}
			break;

		case CMD_SET_DISCARDUNKNOWNMACIND :
		{
			ret_value = macMT7530SetDiscardUnknownMacInd(portId, (u8)(u32)(api_param.paramext_ptr));
		}
			break;

		case CMD_GET_AGETIME :
		{
			u32 MaxAgeTime;
			ret_value = macMT7530GetAgeTime(&MaxAgeTime);
			if (ret_value == -1) goto err_exit;
			if (copy_to_user(api_param.paramext_ptr, &MaxAgeTime, sizeof(MaxAgeTime)))
			{
				ret_value = -1;
//				goto err_exit;
			}
		}
			break;

		case CMD_SET_AGETIME :
		{
			ret_value = macMT7530SetAgeTime((u32)(api_param.paramext_ptr));
		}
			break;

		case CMD_GET_PORTMAC :
		{
			u8 portMac[6];
			ret_value = macMT7530GetPortMac(portId, portMac);
			if (ret_value == -1) goto err_exit;
			if (copy_to_user(api_param.paramext_ptr, portMac, sizeof(portMac)))
			{
				ret_value = -1;
//				goto err_exit;
			}
		}
			break;

		case CMD_GET_PORTSTATUS :
		{
			ret_value = macMT7530GetPortStatus(portId);
		}
			break;

		case CMD_GET_MIBCNT :
		{
			mt7530_switch_GetMibCnt_param GetMibCnt_param;
			if (copy_from_user(&GetMibCnt_param, api_param.paramext_ptr, sizeof(mt7530_switch_GetMibCnt_param)))
			{
				ret_value = -1;
				goto err_exit;
			}
			ret_value = macMT7530GetMIBCounter(GetMibCnt_param.MibCntType, portId, &(GetMibCnt_param.Out__Cnt));
			if (ret_value == -1) goto err_exit;
			if (copy_to_user(api_param.paramext_ptr, &GetMibCnt_param, sizeof(mt7530_switch_GetMibCnt_param)))
			{
				ret_value = -1;
//				goto err_exit;
			}
		}
			break;

		case CMD_SET_QUEUE_PRIORITY :
		{
			ret_value = macMT7530SetQueuePriority((u32)(api_param.paramext_ptr), api_param.PortQueueId);
		}
			break;

		case CMD_GET_QUEUE_MAX_SIZE :
		{
			ret_value = macMT7530GetMaximumQueueSize(api_param.PortQueueId);
		}
			break;

		case CMD_SET_ALLOC_QUEUE_SIZE :
		{
			ret_value = macMT7530SetAllocatedQueueSize((u16)(u32)(api_param.paramext_ptr), api_param.PortQueueId);
		}
			break;

		case CMD_GET_ALLOC_QUEUE_SIZE :
		{
			ret_value = macMT7530GetAllocatedQueueSize(api_param.PortQueueId);
		}
			break;

		case CMD_SET_WEIGHT :
		{
			portId = macMT7530LanPortMap2Switch((api_param.PortQueueId >> 16) & 0xFF);
			ret_value = macMT7530SetWeight((u8)(u32)(api_param.paramext_ptr), (u8)(api_param.PortQueueId & 0xFF), (u8)portId);
		}
			break;

		case CMD_GET_WEIGHT :
		{
			portId = macMT7530LanPortMap2Switch((api_param.PortQueueId >> 16) & 0xFF);
			ret_value = macMT7530GetWeight((u8)(api_param.PortQueueId & 0xFF), (u8)portId);
		}
			break;

		case CMD_SET_BACK_PRESSURE :
		{
			mt7530_switch_api_backPressure_t backPressure;
			if (copy_from_user(&backPressure, api_param.paramext_ptr, sizeof(mt7530_switch_api_backPressure_t)))
			{
				ret_value = -1;
				goto err_exit;
			}
			portId = macMT7530LanPortMap2Switch((api_param.PortQueueId >> 16) & 0xFF);
			ret_value = macMT7530SetBackPressure(&backPressure, (u8)(api_param.PortQueueId & 0xFF), (u8)portId);
		}
			break;

		case CMD_GET_BACK_PRESSURE :
		{
			mt7530_switch_api_backPressure_t backPressure;
			portId = macMT7530LanPortMap2Switch((api_param.PortQueueId >> 16) & 0xFF);
			ret_value = macMT7530GetBackPressure(&backPressure, (api_param.PortQueueId & 0xFF), portId);
			if (ret_value == -1) goto err_exit;
			if (copy_to_user(api_param.paramext_ptr, &backPressure, sizeof(mt7530_switch_api_backPressure_t)))
			{
				ret_value = -1;
//				goto err_exit;
			}
		}
			break;

		case CMD_SET_DROP_POLICY :
		{
			mt7530_switch_DropPolicy_t DropPolicy;
			if (copy_from_user(&DropPolicy, api_param.paramext_ptr, sizeof(mt7530_switch_DropPolicy_t)))
			{
				ret_value = -1;
				goto err_exit;
			}
			portId = macMT7530LanPortMap2Switch((api_param.PortQueueId >> 16) & 0xFF);
			ret_value = macMT7530SetDropPolicy(portId, (api_param.PortQueueId & 0xFF), &DropPolicy);
		}
			break;

		case CMD_GET_DROP_POLICY :
		{
			mt7530_switch_DropPolicy_t DropPolicy;
			portId = macMT7530LanPortMap2Switch((api_param.PortQueueId >> 16) & 0xFF);
			ret_value = macMT7530GetDropPolicy(portId, (api_param.PortQueueId & 0xFF), &DropPolicy);
			if (ret_value == -1) goto err_exit;
			if (copy_to_user(api_param.paramext_ptr, &DropPolicy, sizeof(mt7530_switch_DropPolicy_t)))
			{
				ret_value = -1;
//				goto err_exit;
			}
		}
			break;

		case CMD_SET_TRAFFIC_DESCRIPTOR :
		{
			mt7530_switch_api_trafficDescriptor_t trafficDescriptor;
			if (copy_from_user(&trafficDescriptor, api_param.paramext_ptr, sizeof(trafficDescriptor)))
			{
				ret_value = -1;
				goto err_exit;
			}
			ret_value = macMT7530SetTrafficDescriptor(portId, &trafficDescriptor);
		}
			break;

		case CMD_GET_TRAFFIC_DESCRIPTOR :
		{
			mt7530_switch_api_trafficDescriptor_t trafficDescriptor;
			ret_value = macMT7530GetTrafficDescriptor(portId, &trafficDescriptor);
			if (ret_value == -1) goto err_exit;
			if (copy_to_user(api_param.paramext_ptr, &trafficDescriptor, sizeof(trafficDescriptor)))
			{
				ret_value = -1;
//				goto err_exit;
			}
		}
			break;

		case CMD_CLR_TRAFFIC_DESCRIPTOR :
		{
			ret_value = macMT7530CleanTrafficDescriptor(portId, (u8)(u32)(api_param.paramext_ptr));
		}
			break;

		case CMD_SET_AUTO_DETECT :
		{
			ret_value = macMT7530SetAutoDetection((u8)(u32)(api_param.paramext_ptr), portId);
		}
			break;

		case CMD_GET_AUTO_DETECT :
		{
			u8 AutoDetction;
			ret_value = macMT7530GetAutoDetection(&AutoDetction, portId);
			if (ret_value == -1) goto err_exit;
			if (copy_to_user(api_param.paramext_ptr, &AutoDetction, sizeof(AutoDetction)))
			{
				ret_value = -1;
//				goto err_exit;
			}
		}
			break;

		case CMD_SET_LOOPBACK_CONF :
		{
			ret_value = macMT7530SetLoopbackConf((u8)(u32)(api_param.paramext_ptr), portId);
		}
			break;

		case CMD_GET_LOOPBACK_CONF :
		{
			u8 mode;
			ret_value = macMT7530GetLoopbackConf(&mode, portId);
			if (ret_value == -1) goto err_exit;
			if (copy_to_user(api_param.paramext_ptr, &mode, sizeof(mode)))
			{
				ret_value = -1;
//				goto err_exit;
			}
		}
			break;

		case CMD_GET_CONFIG_STAT :
		{
			ret_value = macMT7530GetConfigurationStatus(portId);
		}
			break;

		case CMD_SET_MAX_FRAME_SIZE :
		{
			ret_value = macMT7530SetMaxFrameSize((u16)(u32)(api_param.paramext_ptr), portId);
		}
			break;

		case CMD_GET_MAX_FRAME_SIZE :
		{
			u16 MaxFrameSize;
			ret_value = macMT7530GetMaxFrameSize(&MaxFrameSize, portId);
			if (ret_value == -1) goto err_exit;
			if (copy_to_user(api_param.paramext_ptr, &MaxFrameSize, sizeof(MaxFrameSize)))
			{
				ret_value = -1;
//				goto err_exit;
			}
		}
			break;

		case CMD_SET_DTEDCE_Ind :
		{
			ret_value = macMT7530SetDTEDCEInd((u8)(u32)(api_param.paramext_ptr), portId);
		}
			break;

		case CMD_GET_DTEDCE_Ind :
		{
			u8 ind;
			ret_value = macMT7530GetDTEDCEInd(&ind, portId);
			if (ret_value == -1) goto err_exit;
			if (copy_to_user(api_param.paramext_ptr, &ind, sizeof(ind)))
			{
				ret_value = -1;
//				goto err_exit;
			}
		}
			break;

		case CMD_SET_PAUSE_TIME :
		{
			ret_value = macMT7530SetPauseTime((u16)(u32)(api_param.paramext_ptr), portId);
		}
			break;

		case CMD_GET_PAUSE_TIME :
		{
			u16 pauseTime;
			ret_value = macMT7530GetPauseTime(&pauseTime, portId);
			if (ret_value == -1) goto err_exit;
			if (copy_to_user(api_param.paramext_ptr, &pauseTime, sizeof(pauseTime)))
			{
				ret_value = -1;
//				goto err_exit;
			}
		}
			break;

		case CMD_SET_PORT_ADMIN :
		{
			ret_value = macMT7530SetPortAdmin((u16)(u32)(api_param.paramext_ptr), portId);
		}
			break;

		case CMD_GET_PORT_ADMIN :
		{
			u8 admin;
			ret_value = macMT7530GetPortAdmin(&admin, portId);
			if (ret_value == -1) goto err_exit;
			if (copy_to_user(api_param.paramext_ptr, &admin, sizeof(admin)))
			{
				ret_value = -1;
//				goto err_exit;
			}
		}
			break;

		case CMD_GET_PORT_CHANGEDCNT :
		{
			u32 cnt;
			ret_value = macMT7530GetPortChangedCnt(portId, &cnt);
			if (ret_value == -1) goto err_exit;
			if (copy_to_user(api_param.paramext_ptr, &cnt, sizeof(cnt)))
			{
				ret_value = -1;
//				goto err_exit;
			}
		}
			break;

		case CMD_GET_PORT_MACLMT:
		{
			ret_value = macMT7530GetPortMacLimit(portId);
		}
			break;
			
		case CMD_SET_PORT_MACLMT:
		{
			ret_value = macMT7530SetPortMacLimit(portId, (u32)(api_param.paramext_ptr));
		}
			break;

		case CMD_SET_PORT_SPDMD:
		{
			ret_value = macMT7530SetPortSpeedMode(portId, (u32)(api_param.paramext_ptr));
		}
			break;

		case CMD_GET_PORT_PAUSE :
		{
			ret_value = macMT7530GetPortPause(portId);
		}
			break;

		case CMD_SET_PORT_PAUSE :
		{
			ret_value = macMT7530SetPortPause(portId, (u8)(u32)(api_param.paramext_ptr));
		}
			break;

		case CMD_GET_PORT_POLEN :
		{
			ret_value = macMT7530GetPortPolicingEnable(portId);
		}
			break;

		case CMD_SET_PORT_POLEN :
		{
			ret_value = macMT7530SetPortPolicingEnable(portId, (u8)(u32)(api_param.paramext_ptr));
		}
			break;

		case CMD_GET_PORT_POL :
		{
			mt7530_switch_api_paramext4 paramext;
			u32 cir, cbs, ebs;
			if (copy_from_user(&paramext, api_param.paramext_ptr, sizeof(paramext)))
			{
				ret_value = -1;
				goto err_exit;
			}
			ret_value = macMT7530GetPortPolicing(portId, &cir, &cbs, &ebs);
			if (ret_value == -1) goto err_exit;
			if (copy_to_user((u32 *)(paramext.p1), &cir, sizeof(cir)))
			{
				ret_value = -1;
//				goto err_exit;
			}
			if (copy_to_user((u32 *)(paramext.p2), &cbs, sizeof(cbs)))
			{
				ret_value = -1;
//				goto err_exit;
			}
			if (copy_to_user((u32 *)(paramext.p3), &ebs, sizeof(ebs)))
			{
				ret_value = -1;
//				goto err_exit;
			}
		}
			break;

		case CMD_SET_PORT_POL :
		{
			mt7530_switch_api_paramext4 paramext;
			if (copy_from_user(&paramext, api_param.paramext_ptr, sizeof(paramext)))
			{
				ret_value = -1;
				goto err_exit;
			}
			ret_value = macMT7530SetPortPolicing(portId, paramext.p1, paramext.p2, paramext.p3);
		}
			break;

		case CMD_GET_PORT_RATELMTEN :
		{
			ret_value = macMT7530GetPortRatelimitingEnable(portId);
		}
			break;

		case CMD_SET_PORT_RATELMTEN :
		{
			ret_value = macMT7530SetPortRatelimitingEnable(portId, (u8)(u32)(api_param.paramext_ptr));
		}
			break;

		case CMD_GET_PORT_RATELMT :
		{
			mt7530_switch_api_paramext2 paramext;
			u32 cir, pir;
			if (copy_from_user(&paramext, api_param.paramext_ptr, sizeof(paramext)))
			{
				ret_value = -1;
				goto err_exit;
			}
			ret_value = macMT7530GetPortRatelimiting(portId, &cir, &pir);
//			if (ret_value == -1) goto err_exit;
			if (copy_to_user((u32 *)(paramext.p1), &cir, sizeof(cir)))
			{
				ret_value = -1;
//				goto err_exit;
			}
			if (copy_to_user((u32 *)(paramext.p2), &pir, sizeof(pir)))
			{
				ret_value = -1;
//				goto err_exit;
			}
		}
			break;

		case CMD_SET_PORT_RATELMT :
		{
			mt7530_switch_api_paramext2 paramext;
			if (copy_from_user(&paramext, api_param.paramext_ptr, sizeof(paramext)))
			{
				ret_value = -1;
				goto err_exit;
			}
			ret_value = macMT7530SetPortRatelimiting(portId, paramext.p1, paramext.p2);
		}
			break;

		case CMD_GET_PORT_LOOPDET :
		{
			ret_value = macMT7530GetPortLoopDetect(portId);
		}
			break;

		case CMD_SET_PORT_LOOPDET :
		{
			ret_value = macMT7530SetPortLoopDetect(portId, (u8)(u32)(api_param.paramext_ptr));
		}
			break;

		case CMD_SET_PORT_DISLOOPED :
		{
			ret_value = macMT7530SetPortDisableLooped(portId, (u8)(u32)(api_param.paramext_ptr));
		}
			break;

		case CMD_GET_PORT_ACT :
		{
			ret_value = macMT7530GetPortActive(portId);
		}
			break;

		case CMD_SET_PORT_ACT :
		{
			ret_value = macMT7530SetPortActive(portId, (u8)(u32)(api_param.paramext_ptr));
		}
			break;

		case CMD_GET_PORT_AN :
		{
			ret_value = macMT7530GetPortAutoNegState(portId);
		}
			break;
			
		case CMD_SET_PORT_AN :
		{
			ret_value = macMT7530PortAutoNegActive(portId, (u8)(u32)(api_param.paramext_ptr));
		}
			break;

		case CMD_SET_PORT_AN_RESTART :
		{
			ret_value = macMT7530PortAutoNegRestartAutoConfig(portId);
		}
			break;

		case CMD_GET_PORT_AN_FAIL :
		{
			ret_value = macMT7530GetEthPortAutoNegFailure(portId);
		}
			break;

		case CMD_GET_PORT_LINK_LOSS :
		{
			ret_value = macMT7530GetEthPortLos(portId);
		}
			break;

		case CMD_GET_PORT_FAIL :
		{
			ret_value = macMT7530GetEthPortFailure(portId);
		}
			break;

		case CMD_GET_PORT_CONGESTION :
		{
			ret_value = macMT7530GetEthPortCongestion(portId);
		}
			break;

		case CMD_GET_TX_TIMESTAMP :
		{
			mt7530_switch_api_timestamp_fifo tx_ts;
			ret_value = macMT7530GetTxTimestamp(&tx_ts);
			if (ret_value == -1) goto err_exit;
			if (copy_to_user(api_param.paramext_ptr, &tx_ts, sizeof(mt7530_switch_api_timestamp_fifo)))
			{
				ret_value = -1;
//				goto err_exit;
			}
		}
			break;

		case CMD_GET_RX_TIMESTAMP :
		{
			mt7530_switch_api_timestamp_fifo rx_ts;
			ret_value = macMT7530GetRxTimestamp(&rx_ts);
			if (ret_value == -1) goto err_exit;
			if (copy_to_user(api_param.paramext_ptr, &rx_ts, sizeof(mt7530_switch_api_timestamp_fifo)))
			{
				ret_value = -1;
//				goto err_exit;
			}
		}
			break;

		case CMD_GET_CURRTIME :
		{
			mt7530_switch_api_timestamp ts;
			ret_value = macMT7530GetCurrTime(&ts);
			if (ret_value == -1) goto err_exit;
			if (copy_to_user(api_param.paramext_ptr, &ts, sizeof(mt7530_switch_api_timestamp)))
			{
				ret_value = -1;
//				goto err_exit;
			}
		}
			break;

		case CMD_SET_CURRTIME :
		{
			mt7530_switch_api_timestamp ts;
			if (copy_from_user(&ts, api_param.paramext_ptr, sizeof(mt7530_switch_api_timestamp)))
			{
				ret_value = -1;
				goto err_exit;
			}
			ret_value = macMT7530SetCurrTime((int)(api_param.PortQueueId), &ts);
		}
			break;

		case CMD_SET_OFFSET :
		{
			mt7530_switch_api_timestamp ts;
			if (copy_from_user(&ts, api_param.paramext_ptr, sizeof(mt7530_switch_api_timestamp)))
			{
				ret_value = -1;
				goto err_exit;
			}
			ret_value = macMT7530SetOffset((int)(api_param.PortQueueId),&ts);
		}
			break;

		case CMD_GET_PTPSTATE :
		{
			int EnablePort;
			ret_value = macMT7530GetPTPState(&EnablePort);
			if (ret_value == -1) goto err_exit;
			if (copy_to_user(api_param.paramext_ptr, &EnablePort, sizeof(int)))
			{
				ret_value = -1;
//				goto err_exit;
			}
		}
			break;

		case CMD_SET_PTPSTATE :
		{
			ret_value = macMT7530SetPTPState((int)(api_param.paramext_ptr));
		}
			break;

		case CMD_SET_DS_TRTCM_ENABLE :
		{
			ret_value = macMT7530SetDsTrtcmEnable((u8)(u32)(api_param.paramext_ptr));
		}
			break;
#if defined(TCSUPPORT_CT_JOYME_BANDWIDTH)			
		case CMD_SET_DEV_TRTCM_RULE :
		{
			mt7530_switch_dev_speed_acl_t dev_trtcm_entry;
			if (copy_from_user(&dev_trtcm_entry, api_param.paramext_ptr, sizeof(dev_trtcm_entry)))
			{
				ret_value = -1;
				goto err_exit;
			}
			ret_value = macMT7530SetMacMatchACL(&dev_trtcm_entry);
		}
			break;
#endif
		case CMD_SET_DS_TRTCM :
		{
			mt7530_switch_api_trtcm_t trtcm_entry;
			if (copy_from_user(&trtcm_entry, api_param.paramext_ptr, sizeof(trtcm_entry)))
			{
				ret_value = -1;
				goto err_exit;
			}
			ret_value = macMT7530SetDsTrtcm(&trtcm_entry);
		}
			break;

		case CMD_GET_DS_TRTCM :
		{
			mt7530_switch_api_trtcm_acl_t trtcm_acl_entry;
			ret_value = macMT7530GetDsTrtcm(&trtcm_acl_entry);
			if (ret_value == -1) goto err_exit;
			if (copy_to_user(api_param.paramext_ptr, &trtcm_acl_entry, sizeof(trtcm_acl_entry)))
			{
				ret_value = -1;
//				goto err_exit;
			}
		}
			break;

		case CMD_SET_DSP5_RATELIMIT_ENABLE:
		{
			ret_value = macMT7530SetDsP5RateLimitEnable((u8)(u32)(api_param.paramext_ptr));
		}
			break;

		case CMD_SET_DSP5_RATELIMIT:
		{
			ret_value = macMT7530SetDsP5RateLimit((u32)(api_param.paramext_ptr));
		}
			break;

		case CMD_SET_DSP5_QOS :
		{
			ret_value = macMT7530SetDsP5Qos((u8)(u32)(api_param.paramext_ptr));
		}
			break;

// add for storm control
		case CMD_SET_STORM_CTRL_PORT :
		{
			ret_value = macMT7530SetStormCtrlPort((u8)(u32)(api_param.paramext_ptr));
		}
			break;	

		case CMD_SET_STORM_CTRL_UNIT :
		{
			ret_value = macMT7530SetStormCtrlUnit((u8)(u32)(api_param.paramext_ptr));
		}
			break;		
			
		case CMD_SET_STORM_CTRL_TYPE :
		{
			ret_value = macMT7530SetStormCtrlType((u8)(u32)(api_param.paramext_ptr));
		}
			break;	
			
		case CMD_SET_STORM_CTRL_MODE :
		{
			ret_value = macMT7530SetStormCtrlMode((u8)(u32)(api_param.paramext_ptr));
		}
			break;	
			
		case CMD_SET_STORM_CTRL_PERIOD :
		{
			ret_value = macMT7530SetStormCtrlPeriod((u8)(u32)(api_param.paramext_ptr));
		}
			break;			
// end storm control
        case CMD_SET_PORTSTATUS:
        {
            ret_value = macMT7530SetPortStatus(portId, (u16)(u32)(api_param.paramext_ptr));
            break;
        }
		case CMD_SET_VLAN_ENTRY_ENABLE :
		{
			mt7530_switch_api_paramext2 paramext;
			if (copy_from_user(&paramext, api_param.paramext_ptr, sizeof(paramext)))
			{
				ret_value = -1;
				goto err_exit;
			}
			ret_value = macMT7530VLan_Entry_Enable((u16)(paramext.p1), (u8)(paramext.p2));
		}
			break;

		case CMD_SET_VLAN_ENTRY_EGTAG :
		{
			mt7530_switch_api_paramext2 paramext;
			if (copy_from_user(&paramext, api_param.paramext_ptr, sizeof(paramext)))
			{
				ret_value = -1;
				goto err_exit;
			}
			ret_value = macMT7530VLan_Entry_SetETag((u16)(paramext.p1), (u8)(portId), (u8)(paramext.p2));
		}
			break;

		case CMD_GET_VLAN_ENTRY_EGTAG :
		{
			mt7530_switch_api_paramext2 paramext;
			u8 etag;
			if (copy_from_user(&paramext, api_param.paramext_ptr, sizeof(paramext)))
			{
				ret_value = -1;
				goto err_exit;
			}
			ret_value = macMT7530VLan_Entry_GetETag((u16)(paramext.p1), (u8)(portId), &etag);
			if (copy_to_user((u8 *)(u32 *)(paramext.p2), &etag, sizeof(etag)))
			{
				ret_value = -1;
//				goto err_exit;
			}
		}
			break;

		case CMD_SET_VLAN_ENTRY_STAG :
		{
			mt7530_switch_api_paramext2 paramext;
			if (copy_from_user(&paramext, api_param.paramext_ptr, sizeof(paramext)))
			{
				ret_value = -1;
				goto err_exit;
			}
			ret_value = macMT7530VLan_Entry_SetSTag((u16)(paramext.p1), (u8)(portId), (u8)(paramext.p2));
		}
			break;

		case CMD_GET_VLAN_ENTRY_STAG :
		{
			mt7530_switch_api_paramext2 paramext;
			u8 stag;
			if (copy_from_user(&paramext, api_param.paramext_ptr, sizeof(paramext)))
			{
				ret_value = -1;
				goto err_exit;
			}
			ret_value = macMT7530VLan_Entry_GetSTag((u16)(paramext.p1), (u8)(portId), &stag);
			if (copy_to_user((u8 *)(u32 *)(paramext.p2), &stag, sizeof(stag)))
			{
				ret_value = -1;
//				goto err_exit;
			}
		}
			break;

		case CMD_ADD_VLAN_PORT_STAG :
		{
			mt7530_switch_api_paramext2 paramext;
			if (copy_from_user(&paramext, api_param.paramext_ptr, sizeof(paramext)))
			{
				ret_value = -1;
				goto err_exit;
			}
			ret_value = macMT7530VLan_Port_AddSTag((u8)(portId), (u8)(paramext.p1), (u16)(paramext.p2));
		}
			break;

		case CMD_SET_VLAN_ENTRY_ETAGMODE :
		{
			ret_value = macMT7530VLan_Entry_SetETagMode((u16)(api_param.PortQueueId), (u8)(u32)(api_param.paramext_ptr));
		}
			break;

		// =========== internal API start from ID 0x1000 ============
		case CMD_SET_MIBCNT_EN :
		{
			ret_value = macMT7530EnableMIBCounter((u32)(api_param.paramext_ptr));
		}
			break;

		case CMD_SET_MIBCNT_CLR :
		{
			ret_value = macMT7530ClearMIBCounter();
		}
			break;

		case CMD_SET_RXOCT_MODE :
		{
			ret_value = macMT7530SetRxOctMode((u32)(api_param.paramext_ptr));
		}
			break;

		case CMD_SET_TXOCT_MODE :
		{
			ret_value = macMT7530SetTxOctMode((mt7530_switch_api_RxTxOctMode)(api_param.paramext_ptr));
		}
			break;

		case CMD_SET_BXPKT_MODE :
		{
			ret_value = macMT7530SetBxPktMode((mt7530_switch_api_RxTxOctMode)(api_param.paramext_ptr));
		}
			break;

		// =========== debug API start from ID 0x7F00 =============
		case CMD_APIDBGDUMP_EN :
		{
			ret_value = macMT7530APIDbgDumpEn((u32)(api_param.paramext_ptr));
		}
			break;

		case CMD_DO_P6Cal:
		{
			ret_value = macMT7530doP6Cal();
		}
			break;

		default :
			break;
	}

err_exit:
	api_param.ret_value = ret_value;
	copy_to_user(ifr->ifr_data, &api_param, sizeof(mt7530_switch_api_params));

	return ret_value;
}

#if defined(TCSUPPORT_CT_DS_LIMIT)
/*
	macMT7530SetDsTrtcmEnable for data speeed limit
*/
int dslimit_macMT7530SetDsTrtcmEnable(u8 enable)
{
	u32 reg, value = 0;
    u32 GDMA1=0,GDMA2=0;
	int rule, i = 0;

	if ((!isMT7525G) && (!isMT7520G)&& (!isMT7520S)&& (!isMT7520)
		&& (!isMT7525))
		return 0;

	memset(&TrtcmAclSetting, 0xff, sizeof(TrtcmAclSetting));
	TrtcmAclSetting.enable = enable ? 1 : 0;

    reg = 0xBFB50500;
    value = read_reg_word(reg);
    GDMA1 = value;
    value = value & 0xFFFF0000 | 0x7777;
    write_reg_word(reg,value);

    reg = 0xBFB51500;
    value = read_reg_word(reg);
    GDMA2 = value;
    value = value & 0xFFFF0000 | 0x7777;
    write_reg_word(reg,value);

	if ( enable )
	{		
		/* enable internal switch port 6 ACL, and set 32 acl rule */
	    reg = 0x2604 ;
	    value = read_reg_word(GSW_BASE+reg);
	    value = (value | (1 << 10));
	    write_reg_word(GSW_BASE+reg, value);

		for (rule = 0; rule < TRTCM_RULE_NUM; rule++)
		{
			reg = 0x94;
        	/* match UPRI */
			value = 0x70000000 + (rule << 12);
			write_reg_word(GSW_BASE+reg, value);

			reg = 0x98;
			value = 0x8400c;
			write_reg_word(GSW_BASE+reg, value);

			reg = 0x90;
			value = 0x80005000 + rule;
			write_reg_word(GSW_BASE+reg, value);
		}

		for (rule = 0; rule < TRTCM_RULE_NUM; rule++)
		{
			reg = 0x94;
			value = (1 << rule);
			write_reg_word(GSW_BASE+reg, value);

			reg = 0x98;
			value = 0x0;
			write_reg_word(GSW_BASE+reg, value);

			reg = 0x90;
			value = 0x80009000 + rule;
			write_reg_word(GSW_BASE+reg, value);
		}

		for (rule = 0; rule < TRTCM_RULE_NUM; rule++)
		{
			reg = 0x94;
			if (rule < 8)
				value = 0x18000000 + (rule << 24) + (rule << 4);
			else
				value = 0x10000000 ;
				
			write_reg_word(GSW_BASE+reg, value);

			reg = 0x98;
            value=0;
			write_reg_word(GSW_BASE+reg, value);

			reg = 0x90;
			value = 0x8000b000 + rule;
			write_reg_word(GSW_BASE+reg, value);
		}		

        /* ACL Rate value set to default 0xFFFF */
		for (rule = 0; rule < TRTCM_RULE_NUM; rule++)
		{
			reg = 0x94;
			value = 0x3FFF;
			write_reg_word(GSW_BASE+reg, value);
			
			reg = 0x98;
			value = 0x0;
			write_reg_word(GSW_BASE+reg, value);

			reg = 0x90;
			value = 0x8000d000 + rule;
			write_reg_word(GSW_BASE+reg, value);
        }
	}
	else
	{
		/* undo internal switch ACL rule */
	    reg = 0x2604 ;
	    value = read_reg_word(GSW_BASE+reg);
	    value &= (~(1 << 10));
	    write_reg_word(GSW_BASE+reg, value);
	}

    reg = 0xBFB50500;
    write_reg_word(reg,GDMA1);

    reg = 0xBFB51500;
    write_reg_word(reg,GDMA2);

	return 0;
}

/*
	macMT7530SetDsTrtcm for data speeed limit
*/
int dslimit_macMT7530SetDsTrtcm(mt7530_switch_api_trtcm_t *trtcm_p)
{
	u32 reg, value = 0;
	u32 GDMA1=0,GDMA2=0;

	if ((!isMT7525G) && (!isMT7520G)&& (!isMT7520S)&& (!isMT7520)
		&& (!isMT7525))
		return 0;

	if (trtcm_p == NULL)
		return -1;

	if (trtcm_p->trtcmId > TRTCM_RULE_NUM)
		return -1;

    reg = 0xBFB50500;
    value = read_reg_word(reg);
    GDMA1 = value;
    value = value & 0xFFFF0000 | 0x7777;
    write_reg_word(reg,value);

    reg = 0xBFB51500;
    value = read_reg_word(reg);
    GDMA2 = value;
    value = value & 0xFFFF0000 | 0x7777;
    write_reg_word(reg,value);
    
	TrtcmAclSetting.trtcm_acl_table[trtcm_p->trtcmId].trtcmId = trtcm_p->trtcmId;
	TrtcmAclSetting.trtcm_acl_table[trtcm_p->trtcmId].CIR = trtcm_p->CIR;

    reg = 0x94;
    if(trtcm_p->CIR == 0x3FFF)
        value = trtcm_p->CIR;
	else
        value = 0x8000 | (trtcm_p->CIR);
	write_reg_word(GSW_BASE+reg, value);
		
	reg = 0x98;
	value = 0;
	write_reg_word(GSW_BASE+reg, value);
	
	reg = 0x90;
	value = 0x8000D000 + trtcm_p->trtcmId;
	write_reg_word(GSW_BASE+reg, value);

    reg = 0xBFB50500;
    write_reg_word(reg,GDMA1);

    reg = 0xBFB51500;
    write_reg_word(reg,GDMA2);

	return 0;
}
#endif
#if defined(TCSUPPORT_CT_JOYME_BANDWIDTH)
void disableACLOfSwitchPort(void){
	u32 reg, value = 0;
	
	/* undo internal switch ACL rule */
    reg = 0x2004 ;
    value = read_reg_word(GSW_BASE+reg);
    value &= (~(1 << 10));
    write_reg_word(GSW_BASE+reg, value);

	reg = 0x2104 ;
    value = read_reg_word(GSW_BASE+reg);
    value &= (~(1 << 10));
    write_reg_word(GSW_BASE+reg, value);
		
	reg = 0x2204 ;
    value = read_reg_word(GSW_BASE+reg);
    value &= (~(1 << 10));
    write_reg_word(GSW_BASE+reg, value);
	
	reg = 0x2304 ;
    value = read_reg_word(GSW_BASE+reg);
    value &= (~(1 << 10));
    write_reg_word(GSW_BASE+reg, value);

	reg = 0x2404 ;
    value = read_reg_word(GSW_BASE+reg);
    value &= (~(1 << 10));
    write_reg_word(GSW_BASE+reg, value);
	
	reg = 0x2504 ;
    value = read_reg_word(GSW_BASE+reg);
    value &= (~(1 << 10));
    write_reg_word(GSW_BASE+reg, value);
		
	reg = 0x2604 ;
    value = read_reg_word(GSW_BASE+reg);
    value &= (~(1 << 10));
    write_reg_word(GSW_BASE+reg, value);
}

void enalbeACLOfSwitchPort(int up_down){	
	u32 reg, value = 0;
	
	switch(up_down){
		case ACL_MAC_UP_STREAM:
			/* enable internal switch port 0~4 ACL for 25F*/
			reg = 0x2004 ;
			value = read_reg_word(GSW_BASE+reg);
			value = (value | (1 << 10));
			write_reg_word(GSW_BASE+reg, value);
	
			reg = 0x2104 ;
			value = read_reg_word(GSW_BASE+reg);
			value = (value | (1 << 10));
			write_reg_word(GSW_BASE+reg, value);
	
			reg = 0x2204 ;
			value = read_reg_word(GSW_BASE+reg);
			value = (value | (1 << 10));
			write_reg_word(GSW_BASE+reg, value);
	
			reg = 0x2304 ;
			value = read_reg_word(GSW_BASE+reg);
			value = (value | (1 << 10));
			write_reg_word(GSW_BASE+reg, value);
	
			reg = 0x2404 ;
			value = read_reg_word(GSW_BASE+reg);
			value = (value | (1 << 10));
			write_reg_word(GSW_BASE+reg, value);

			/*port 5:for 7525G*/
			reg = 0x2504 ;
			value = read_reg_word(GSW_BASE+reg);
			value = (value | (1 << 10));
			write_reg_word(GSW_BASE+reg, value);
			break;
		case ACL_MAC_DOWN_STREAM:
			/* enable internal switch port 6 ACL*/
			reg = 0x2604 ;
			value = read_reg_word(GSW_BASE+reg);
			value = (value | (1 << 10));
			write_reg_word(GSW_BASE+reg, value);
			break;
	}
	
	return;
}

void setACLMacRule(int rule, int up_down, unsigned int* dev_mac, int speed){	
	u32 reg, value = 0;
	int i = 0;
	int ruleIndex = 0;
	int tableIndex = 0;

	if(up_down == ACL_MAC_UP_STREAM){
		ruleIndex = rule*2 + 1; 						/*eg.entry index: 1,3,5,7,9...*/
	}else{
		ruleIndex = rule*2; 							/*eg.entry index: 0,2,4,6,8.*/
	}

	/* 1.set ACL table entry, match dst mac need 3 tables*/
	for (i = 0; i < ACL_MAC_MATCH_TABLE_NUM; i++){
		reg = 0x94;
		value = 0xffff0000 + (dev_mac[i*2]<<8) + dev_mac[i*2+1];			//match mac 
		write_reg_word(GSW_BASE+reg, value);

		reg = 0x98;
		if(up_down == ACL_MAC_DOWN_STREAM){
			value = 0x8ff00+i*2;									//dst mac
		}else{
			value = 0x8ff06+i*2;									//src mac
		}
		write_reg_word(GSW_BASE+reg, value);

		reg = 0x90;	
		tableIndex = ruleIndex*3 + i; 					
		value = 0x80005000 + tableIndex;	
		write_reg_word(GSW_BASE+reg, value);
	}


	
	/* 2.set ACL mask entry,  need match 3 tables*/
	reg = 0x94;
	value = (1 << ruleIndex*3) + (1 << (ruleIndex*3+1)) + (1 << (ruleIndex*3+2));		//match 3 rules	
	write_reg_word(GSW_BASE+reg, value);

	reg = 0x98;
	value = 0x0;	
	write_reg_word(GSW_BASE+reg, value);

	reg = 0x90;
	value = 0x80009000 + ruleIndex;							//acl mask Entry
	write_reg_word(GSW_BASE+reg, value);


	/* 3.set ACL Control entry*/
	reg = 0x94;
	if (ruleIndex < 8)										//only support 8 ACL Event Counter
		value = 0x18000000 + (ruleIndex << 24);			//enable ACL hit count, and set counter group index
	else
		value = 0x10000000 ;	
	write_reg_word(GSW_BASE+reg, value);

	reg = 0x98;
	value=0;
	write_reg_word(GSW_BASE+reg, value);

	reg = 0x90; 										//ACL rule control entry
	value = 0x8000b000 + ruleIndex;	
	write_reg_word(GSW_BASE+reg, value);


	/* 4.set ACL rate entry,  need match 3 tables*/
	reg = 0x94;
	value = 0x8000 | speed;	
	write_reg_word(GSW_BASE+reg, value);
	
	reg = 0x98;
	value = 0x0;
	write_reg_word(GSW_BASE+reg, value);

	reg = 0x90;
	value = 0x8000d000 + ruleIndex;	
	write_reg_word(GSW_BASE+reg, value);


	enalbeACLOfSwitchPort(up_down);
	
}

int macMT7530SetMacMatchACL(mt7530_switch_dev_speed_acl_t* dev_mac_trtcm_p)
{
	u32 reg, value = 0;
    u32 GDMA1=0,GDMA2=0;
	int rule, i = 0;

	/*discard all packet before set ACL rule*/
    reg = 0xBFB50500;
    value = read_reg_word(reg);
    GDMA1 = value;
    value = value & 0xFFFF0000 | 0x7777;
    write_reg_word(reg,value);

    reg = 0xBFB51500;
    value = read_reg_word(reg);
    GDMA2 = value;
    value = value & 0xFFFF0000 | 0x7777;
    write_reg_word(reg,value);

	printk("macMT7530SetMacMatchACL:enable=%d\n",dev_mac_trtcm_p->enable);

	if (dev_mac_trtcm_p->enable)
	{		
		for (rule = 0; rule < TRTCM_MATCH_MAC_RULE_NUM; rule++){
			if(dev_mac_trtcm_p->trtcm_acl_mac_table[rule].ds_queue_speed != 0){
				/*need set downstreams ACL rule*/
				setACLMacRule(rule,ACL_MAC_DOWN_STREAM,dev_mac_trtcm_p->trtcm_acl_mac_table[rule].dev_mac,dev_mac_trtcm_p->trtcm_acl_mac_table[rule].ds_queue_speed);
			}

			if(dev_mac_trtcm_p->trtcm_acl_mac_table[rule].us_queue_speed != 0){
				/*need set upstreams ACL rule*/				
				setACLMacRule(rule,ACL_MAC_UP_STREAM,dev_mac_trtcm_p->trtcm_acl_mac_table[rule].dev_mac,dev_mac_trtcm_p->trtcm_acl_mac_table[rule].us_queue_speed);
			}
		}
	}
	else
	{
		disableACLOfSwitchPort();
	}


	/*restoe GDMA1 & GMDA2 register*/
    reg = 0xBFB50500;
    write_reg_word(reg,GDMA1);

    reg = 0xBFB51500;
    write_reg_word(reg,GDMA2);

	return 0;
}
#endif

#endif // #ifdef TCSUPPORT_MT7530_SWITCH_API



