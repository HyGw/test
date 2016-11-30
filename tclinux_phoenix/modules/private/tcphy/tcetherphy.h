/*
** $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/modules/private/tcphy/tcetherphy.h#1 $
*/

/************************************************************************
*
*	Copyright (C) 2009 TrendChip Technologies Corp.
*	All Rights Reserved.
*
************************************************************************
*/

// used in tc3162l2mac.c & tcetherphy.c 

#ifndef _TCETHERPHY_H_
#define _TCETHERPHY_H_
//#define MT7510FE_SUPPORT

/************************************************************************
* 1. TCPHY_SUPPORT : turn ON for basic TCPHY function
*                              and phy type selection
* 2. TCPHY_DEBUG : turn ON for RD debug, turn OFF for code-shrink
* 3. TCPHY_FIELD_DBGTEST : default OFF, turn ON for FAE remote debug,
* 4. TCPHY_PHY2PHY : turn ON for Phy-to-Phy connection without transformer
*
*************************************************************************/
#ifdef LINUX_OS
/*Lions porting to linux relate funtion mapping.*/
#define printf printk
#define stricmp(x,y) strcmp(x,y)
#define pause(x) mdelay(x)
#define atoi(x)  simple_strtoul(x, NULL,10)

#define dbg_pline_1(x)		printk("%s", x)
#define dbg_plineb_1(x, y)	printk("%s%02x", x, (uint8) y)
#define dbg_plinew_1(x, y)	printk("%s%04x", x, (uint16) y)
#define dbg_plinel_1(x, y)		printk("%s0x%08lx", x, (uint32) y)
#if  defined(CONFIG_MIPS_TC3182) ||  defined(CONFIG_MIPS_TC3262)
/*MII interface MACRO*/
#define PHY_CONTROL_REG		MII_BMCR
#define PHY_RESET		BMCR_RESET
#define MIIDR_AUTO_NEGOTIATE	(BMCR_ANRESTART | BMCR_ANENABLE)
#define PHY_STATUS_REG		MII_BMSR 
#define PHY_REMOTE_CAP_REG		MII_LPA
#endif

/*end Lions porting to linux relate funtion mapping.*/

//#if defined(TCSUPPORT_CPU_MT7505)  // allen_20140210
#define MT7505FE_SUPPORT
//#endif
//#if defined(TCSUPPORT_CPU_MT7510) // allen_20140219
#define MT7510FE_SUPPORT
#define MT7510Ge_SUPPORT
//#endif
//#if  defined(MT7530_SUPPORT) // allen_20140303
#define SW_MT7530_SUPPORT 
//#endif

#if defined(TC2206_SUPPORT)|| defined(TC2031_SUPPORT) || defined(RT63365_SUPPORT) || defined(MT7505FE_SUPPORT)
#define TCPHY_SUPPORT	1 // All TC PHY initial & patch 
#endif

#if 0
#ifdef TC2206_SUPPORT
#define TCPHY_4PORT 1    //   TC2206
#endif
#endif

#if defined(TC2031_SUPPORT) /* UE model */
#define TCPHY_1PORT 1    //  LEH/LEM
//#define TCPHY_TESTCHIP 1 //   TC2104SD
#endif

#if defined(RT63365_SUPPORT)|| defined(MT7505FE_SUPPORT)
#define TCPHY_5PORT 1    
#endif

#if defined(TC2206_SUPPORT)||defined(MT7510FE_SUPPORT)||defined(SW_MT7530_SUPPORT)||defined(MT7510Ge_SUPPORT)
#define TCPHY_4PORT 1    
#endif


#if defined(TCPHY_DEBUG) && defined(TCPHY_SUPPORT)
#define TCPHY_DEBUG		1 // All TC PHY debug (OFF if code-size is limited)
#define TCPHY_DEBUG_DISP_LEVEL 2 // Level=0..6 (default:OFF)
#endif

// 2. DBUGE : advanced debug function
#if (!defined(PURE_BRIDGE) && defined(TCPHY_SUPPORT)) && defined(TCPHY_DEBUG)
#define TCPHY_DEBUG		1 // All TC PHY debug (OFF if code-size is limited)
#define TCPHY_DEBUG_DISP_LEVEL 2 // Level=0..6 (default:OFF)
#endif

//#define SW_MT7530_SUPPORT 
//#define MT7510Ge_SUPPORT
#define PHYPART_DEBUG  //for check FW hang in phy or others
//#define PHYPART_DEBUG_SW_PATCH  //for check FW hang in phy or others

// 1. SUPPORT : basic function
#if defined( SW_MT7530_SUPPORT)
#define MTPHY_SUPPORT	1 
#define MTPHY_DEBUG		1 // All TC PHY debug (OFF if code-size is limited)
//#define MTPHY_DEBUG_DISP_LEVEL 2 // Level=0..6 (default:OFF) //CML_20130312
#define CL45_CMD_SUPPORT  1
#define TCPHY_DEBUG_DISP_LEVEL 2 // Level=0..6 (default:OFF) //CML_20130312
#endif

#define TCPHY_DEBUG		1
#define MTPHY_DEBUG		1


#if defined( MT7510Ge_SUPPORT)
#define MTPHY_SUPPORT	1 
#define MTPHY_DEBUG		1 // All TC PHY debug (OFF if code-size is limited)
//#define MTPHY_DEBUG_DISP_LEVEL 2 // Level=0..6 (default:OFF) //CML_20130312
#define CL45_CMD_SUPPORT  1
#define TCPHY_DEBUG_DISP_LEVEL 2 // Level=0..6 (default:OFF) //CML_20130312
#endif

#else //linos os
#define uint		unsigned int

//#if defined(TCSUPPORT_CPU_MT7505) // allen_20140210 // allen_20140303
//#if defined(MT7505) // allen_20140210 //TCSUPPORT_CPU_MT7505 for linux; MT7505 for Linos // allen_20140303
//#if define(MT7505)
#define MT7505FE_SUPPORT
//#endif
//#if defined(TCSUPPORT_CPU_MT7510)  // allen_20140219
#define MT7510FE_SUPPORT
#define MT7510Ge_SUPPORT
//#endif
//#if  defined(MT7530_SUPPORT)  // allen_20140219 // allen_20140303
#define SW_MT7530_SUPPORT 
//#endif

//#define MT7505FE_SUPPORT // allen_20140303
#define TC6508_SUPPORT
#define PHYPART_DEBUG  //for check FW hang in phy or others
//#define PHYPART_DEBUG_SW_PATCH  //for check FW hang in phy or others

// 1. SUPPORT : basic function
#if defined(SW_MT7530_SUPPORT)
#define MTPHY_SUPPORT	1 
#define MTPHY_DEBUG		1 // All TC PHY debug (OFF if code-size is limited)
//#define MTPHY_DEBUG_DISP_LEVEL 2 // Level=0..6 (default:OFF) //CML_20130312
#define CL45_CMD_SUPPORT  1
#define TCPHY_DEBUG_DISP_LEVEL 2 // Level=0..6 (default:OFF) //CML_20130312
#endif
#if defined(MT7510Ge_SUPPORT)
#define MTPHY_SUPPORT	1 
#define MTPHY_DEBUG		1 // All TC PHY debug (OFF if code-size is limited)
//#define MTPHY_DEBUG_DISP_LEVEL 2 // Level=0..6 (default:OFF) //CML_20130312
#define CL45_CMD_SUPPORT  1
#define TCPHY_DEBUG_DISP_LEVEL 2 // Level=0..6 (default:OFF) //CML_20130312
#endif


#if defined(LEH_3052) || !(defined(WLAN_RT2561_SUPPORT) || defined(RT2880_SUPPORT) || defined(WLAN_RT3390_SUPPORT))||defined(TC2206_SUPPORT)|| defined(RT63365_SUPPORT)   /* UE model */
// add for all TC ethernet PHY, used in tc3162l2mac.c & tcetherphy.c
#ifndef PURE_BRIDGE
#define TCPHY_SUPPORT	1 // All TC PHY initial & patch 
#endif
#endif

// PHY type selection
#if defined(TCPHY_SUPPORT)
#if (defined(TC2206_SUPPORT) && !defined(PURE_BRIDGE))
#define TCPHY_4PORT 1    //   TC2206
#endif

#if defined(LEH_3052) || !(defined(WLAN_RT2561_SUPPORT) || defined(RT2880_SUPPORT)  || defined(WLAN_RT3390_SUPPORT)) /* UE model */
#define TCPHY_1PORT 1    //   LEH, LEM
#endif

#ifndef PURE_BRIDGE
#if defined(TC6501_SUPPORT) || defined(TC6508_SUPPORT)
#define TCPHY_TESTCHIP 1 //   TC2104SD, TC6501, TC6508
#endif
#if defined(TC6501_SUPPORT) || defined(RT63365_SUPPORT) || defined(TC6508_SUPPORT)|| defined(MT7505FE_SUPPORT)
#define TCPHY_5PORT 1    //   TC6501, RT63365, TC6508
#endif
#endif //#ifndef PURE_BRIDGE
#endif //#end TCPHY_SUPPORT

// 2. DBUGE : advanced debug function
#if (!defined(PURE_BRIDGE) && defined(TCPHY_SUPPORT))
#define TCPHY_DEBUG		1 // All TC PHY debug (OFF if code-size is limited)
#define TCPHY_DEBUG_DISP_LEVEL 2 // Level=0..6 (default:OFF)
#endif

#endif //#else linux_os


// 3. DBGTEST : for FAE remote information collection, used in tcetherphy.c
#if (!defined(PURE_BRIDGE) && defined(TCPHY_DEBUG))
//#define TCPHY_FIELD_DBGTEST	1 // Turn On by FAE request (default:OFF)
#endif

// 4. PHY2PHY : for  Phy-to-Phy connection without transformer
#if (defined(SWITCH_3052) && defined(TCPHY_SUPPORT))
#define TCPHY_PHY2PHY 1 // (default:OFF)
#endif



//**********************************************************************//
// Definitions listed below should be changed by ethernet team only
//**********************************************************************//

/************************************************************************
*
* MP chip revision selection ON/OFF
*	LEH, LEM
*
*************************************************************************/

// *** Begin of TCxxx_DEBUG and TCxxx_DBGTEST definition ***
// TCxxx_DEBUG is controlled by TCPHY_DEBUG & TCxxx_SUPPORT
// TCxxx_DBGTEST is controlled by TCPHY_FIELD_DBGTEST & TCxxx_DEBUG

/** LEH PHY **/
#ifdef TCPHY_1PORT
// PHY initial & patch for 3162LEH (phy_id:03a2_9400)
#define TC2031_SUPPORT	1 // LEH 
#endif
#if (defined(TCPHY_DEBUG) && defined(TC2031_SUPPORT))
//#define TC2031_DEBUG	1 // LEH PHY debug
#endif
#if (defined(TCPHY_FIELD_DBGTEST) && defined(TC2031_DEBUG))
//#define TC2031_DBGTEST	1 // LEH PHY field debug test
#endif

/** LEM PHY **/
#ifdef TCPHY_1PORT
// PHY initial & patch for 3162LEM (phy_id:03a2_9401), used in tcetherphy.c
#define TC2101MB_SUPPORT	1 // LEM  
#endif
#if (defined(TCPHY_DEBUG) && defined(TC2101MB_SUPPORT))
#define TC2101MB_DEBUG		1 // LEM PHY debug
#endif
#if (defined(TCPHY_FIELD_DBGTEST) && defined(TC2101MB_DEBUG))
#define TC2101MB_DBGTEST	1 // LEM PHY field debug test
#endif

/** TC2101ME PHY **/
#ifdef TCPHY_1PORT
// PHY initial & patch for 3162UE (phy_id:03a2_9404), used in tcetherphy.c
#define TC2101ME_SUPPORT	1 // L6, 62UE 
//#define TC3162U_PSM_SUPPORT 1 /*PSM Support*/
#endif
#if (defined(TCPHY_DEBUG) && defined(TC2101ME_SUPPORT))
#define TC2101ME_DEBUG		1 // 62UE PHY debug
//#define TC3162U_PSM_DEBUG  1 /*PSM Debug*/
#endif
#if (defined(TCPHY_FIELD_DBGTEST) && defined(TC2101ME_DEBUG))
#define TC2101ME_DBGTEST	1 // 62UE PHY field debug test
#endif

/** TC2101MM PHY **/  // allen_20130926 : merge 7502 & 7510/20
#ifdef TCPHY_1PORT
// PHY initial & patch for MT7502 (phy_id:03a2_940e), used in tcetherphy.c
#define TC2101MM_SUPPORT	1 // MT7502 
//#define TC3162U_PSM_SUPPORT 1 /*PSM Support*/
#endif

#if (defined(TCPHY_DEBUG) && defined(TC2101MM_SUPPORT))
#define TC2101MM_DEBUG		1 // 62UE PHY debug
//#define TC3162U_PSM_DEBUG  1 /*PSM Debug*/
#endif

#if (defined(TCPHY_FIELD_DBGTEST) && defined(TC2101MM_DEBUG))
#define TC2101ME_DBGTEST	1 // 62UE PHY field debug test
#endif


/** RT63260 PHY **/
#ifdef TCPHY_1PORT
// PHY initial & patch for RT63260 (phy_id:03a2_940a), used in tcetherphy.c
#define TC2101MI_SUPPORT	1 // RT63260 
//#define TC3162U_PSM_SUPPORT 1 /*PSM Support*/
#endif
#if (defined(TCPHY_DEBUG) && defined(TC2101MI_SUPPORT))
#define TC2101MI_DEBUG		1 // PHY debug
//#define TC3162U_PSM_DEBUG  1 /*PSM Debug*/
#endif
#if (defined(TCPHY_FIELD_DBGTEST) && defined(TC2101MI_DEBUG))
#define TC2101MI_DBGTEST	1 // PHY field debug test
#endif


/** TC2206 PHY **/
#ifdef TCPHY_4PORT
// PHY initial & patch for 2206 (phy_id:03a2_9402), used in tcetherphy.c
#define TC2104MC_SUPPORT	1 // LEM  
#endif
#if (defined(TCPHY_DEBUG) && defined(TC2104MC_SUPPORT))
#define TC2104MC_DEBUG		1 // LEM PHY debug
#endif
#if (defined(TCPHY_FIELD_DBGTEST) && defined(TC2104MC_DEBUG))
#define TC2104MC_DBGTEST	1 // LEM PHY field debug test
#endif

/** TC2206F PHY **/
#ifdef TCPHY_4PORT
// PHY initial & patch for 2206 (phy_id:03a2_9406), used in tcetherphy.c
#define TC2104ME_SUPPORT	1 // 2206F
#endif
#if (defined(TCPHY_DEBUG) && defined(TC2104ME_SUPPORT))
#define TC2104ME_DEBUG		1 // 2206F PHY debug
#endif
#if (defined(TCPHY_FIELD_DBGTEST) && defined(TC2104ME_DEBUG))
#define TC2104ME_DBGTEST	1 // LEM PHY field debug test
#endif



/** TC2101MF PHY **/
#ifdef TCPHY_1PORT
// PHY initial & patch for MF (phy_id:03a2_9407), used in tcetherphy.c
#define TC2101MF_SUPPORT	1 //  
//#define TC3162U_PSM_SUPPORT 1 /*PSM Support*/
#endif
#if (defined(TCPHY_DEBUG) && defined(TC2101MF_SUPPORT))
#define TC2101MF_DEBUG		1 // 62UE PHY debug
//#define TC3162U_PSM_DEBUG  1 /*PSM Debug*/
#endif
#if (defined(TCPHY_FIELD_DBGTEST) && defined(TC2101MF_DEBUG))
#define TC2101MF_DBGTEST	1 // 62UE PHY field debug test
#endif
/** TC2102ME PHY **/
#ifdef TCPHY_1PORT
// PHY initial & patch for MF (phy_id:03a2_9405), used in tcetherphy.c
#define TC2102ME_SUPPORT	1
#endif
#if (defined(TCPHY_DEBUG) && defined(TC2102ME_SUPPORT))
#define TC2102ME_DEBUG		1 // 62UE PHY debug
//#define TC3162U_PSM_DEBUG  1 /*PSM Debug*/
#endif
#if (defined(TCPHY_FIELD_DBGTEST) && defined(TC2102ME_DEBUG))
#define TC2102ME_DBGTEST	1 // 62UE PHY field debug test
#endif

/**TC2105MJ PHY**/
#ifdef TCPHY_5PORT
//PHY initial & patch for MJ (phy_id : 03a2_940b), used in tcetherphy.c
#define TC2105MJ_SUPPORT	1
#endif
#if (defined(TCPHY_DEBUG) && defined(TC2105MJ_SUPPORT))
#define TC2105MJ_DEBUG		1 // PHY debug
#endif
/**MT7510FE PHY**/
#ifdef TCPHY_4PORT
//PHY initial & patch for MJ (phy_id : 03a2_940F), used in tcetherphy.c
//#define MT7510FE_SUPPORT	1   
#endif
#if (defined(TCPHY_DEBUG) && defined(MT7510FE_SUPPORT))
#define MT7510FE_DEBUG		1 // PHY debug
#endif



/************************************************************************
*
* TEST chip revision selection ON/OFF
*	tc2104sd, tc2101me
*
*************************************************************************/
#ifdef TCPHY_TESTCHIP 

/** TC2104SD PHY **/
#ifdef TCPHY_4PORT
// PHY initial & patch for TC2104SD (phy_id:03a2_9403), used in tcetherphy.c
#define TC2104SD_SUPPORT	1 // .11 4port PHY (default:OFF)
#endif

/** TC6501 PHY **/
#ifdef TCPHY_5PORT
// PHY initial & patch for TC6501 (phy_id:03a2_9408), used in tcetherphy.c
#define TC2105SG_SUPPORT	1 // 55nm 5port PHY (default:OFF)
#define TC2105SK_SUPPORT	1 // 55nm 5port PHY (default:OFF)
#endif

#endif // TCPHY_TESTCHIP

/************************************************************************
*
* Port Number
*
*************************************************************************/

// 5. Port number
//#if (defined(TC2105SG_SUPPORT) || defined(TC2105MJ_SUPPORT) || defined(TC2105SK_SUPPORT)) // allen_20140303
#if (defined(TC2105SG_SUPPORT) || defined(TC2105MJ_SUPPORT) || defined(TC2105SK_SUPPORT) || defined(MT7505FE_SUPPORT)) // allen_20140303
#define TCPHY_PORTNUM 5
#else 
#if (defined(TC2104MC_SUPPORT) || defined(TC2104SD_SUPPORT) || defined(TC2104ME_SUPPORT) || defined(MT7510FE_SUPPORT))
#define TCPHY_PORTNUM 4
#else
#define TCPHY_PORTNUM 1
#endif
#endif
#if defined(SW_MT7530_SUPPORT) || defined(MT7510Ge_SUPPORT)|| defined(MT7505FE_SUPPORT)
	#define MTPHY_PORTNUM 5
#endif

/************************************************************************
*
* Config Load for all port or specific port selection 
*
*************************************************************************/
#if (defined(TC2104MC_SUPPORT) || defined(TC2104SD_SUPPORT)|| defined(TC2104ME_SUPPORT) || defined(TC2105MJ_SUPPORT)|| defined(SW_MT7530_SUPPORT)||defined(MT7510Ge_SUPPORT)|| defined(MT7505FE_SUPPORT))
#define DO_PER_PORT 1
#define DO_4_PORT 0
#define DO_5_PORT 2
#endif
#if 1
#define NORMAL_READ (1<<0)
#define POST_READ (1<<1)
#endif


#endif /* _TCETHERPHY_H_ */
