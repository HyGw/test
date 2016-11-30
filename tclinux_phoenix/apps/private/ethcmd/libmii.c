/************************************************************************
 *
 *	Copyright (C) 2006 Trendchip Technologies, Corp.
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
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/mii.h>
#include <linux/sockios.h>
#include <fcntl.h>

#include "libmii.h"
#include "libcmd.h"
#include "ether.h"
#include "ethercmd.h"

//#include <asm/tc3162/tc3162.h>

#ifndef TCSUPPORT_3_18_21_KERNEL
#ifdef TCSUPPORT_2_6_36_KERNEL
#include "../../../linux-2.6.36/arch/mips/include/asm/tc3162/tc3162.h"
#else
#include "../../../linux/include/asm-mips/tc3162/tc3162.h"
#endif
#else
#ifdef JUDGE_SWITCH_SCENARIO_BY_751020_SUBMODEL
#define MT751020_SUBMODEL_REG   (0xbfb000f8)
#endif
typedef unsigned long int uint32; 		/* 32-bit unsigned integer      */
typedef unsigned short uint16;          /* 16-bit unsigned integer      */
#endif

#ifdef ETHCMD_DBG
extern u8 ethcmdDbg;
#endif

extern u8 use_extern_switch;

static struct swconfig swcfg;
swic_api_t swic;
int lan_portmap[6];

void tc2206_init_api(swic_api_t* swic);
void rt63365_init_api(swic_api_t* swic);
void ip175c_init_api(swic_api_t* swic);
#if defined(SQA_VERIFY)
void rt62806_init_api(swic_api_t* swic);
#endif
void mt7530_init_api(swic_api_t* swic);

struct vender swic_id[]=
{
	{4, 2, 0x243, 4, 3, 0xd80, "IP175C", IP175C, ip175c_init_api},
	{31, 31, 0x2206, 31, 31, 0x2206, "TC2206", TC2206, tc2206_init_api},
	// for MT7530 FPGA with RT63365 PHY chip, MT7530 ID shall be checked before RT63365
	{-1, 0x7ffc, 0x2806, -1, 0x7ffc, 0x2806, "RT62806", RT62806, mt7530_init_api},
	{-1, 0x7ffc, 0x7530, -1, 0x7ffc, 0x7530, "MT7530", MT7530, mt7530_init_api},
	{0, 2, 0x3a2, 0, 3, 0x940b, "RT63365", SWIC_RT63365, rt63365_init_api},
	{6, 2, 0x1c, 6, 3, 0xc912, "RT63365", SWIC_RT63365, rt63365_init_api},
	{0, 2, 0x243, 0, 3, 0xc54, "RT63365", SWIC_RT63365, rt63365_init_api},
	#if defined(SQA_VERIFY)
	{-1, 0x7ffc, 0x28060001, -1, 0x7ffc, 0x28060001, "RT62806", RT62806, rt62806_init_api}
	#endif
};

/*Number of switch IC*/
#define NSWIC (sizeof(swic_id)/sizeof(swic_id[0]))

#ifdef JUDGE_SWITCH_SCENARIO_BY_751020_SUBMODEL
u32 np_hw_id			= 0;	// 0 means uninitialized/invalid
u32 mt751020_submodel_id	= 0;	// 0 means uninitialized/invalid
u32 efuse_extreme_mode = 0;	// 0 means uninitialized/invalid
#endif
/*___________________________________________________________________
**      function name: getSwcfg
**      descriptions:
**         Get the global swcfg variable.
**
**      parameters:
**        None
**  
**      global:
**           swcfg
**             
**      return:
**          The address of global swcfg variable.
**	    
**      call:
**          None
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
struct swconfig* getSwcfg(void){
	return &swcfg;
}/*end getSwcfg*/
#define PORT_REVERSE_PATH	"/proc/tc3162/port_reverse"
#define STAG_TO_VTAG_PATH	"/proc/tc3162/stag_to_vtag"
#define FAIL 			-1
/*_____________________________________________________________________________
**      function name: fileRead
**      descriptions:
**           Read file context and copy information into buf.
**
**      parameters:
**            path:  File path
**            buf:  Specify the buffer that you want to store it.
**            size: Specify the size that your want to read form file.
**
**      global:
**             None
**
**      return:
**             None
**
**      call:
**   	 None
**
**      revision:
**      1. Here 2008/5/7
**____________________________________________________________________________
*/
void
fileRead(char *path, char *buf, int size){
	int  fd=0, ret=0;

	memset(buf,0, size);
	fd = open(path,O_RDONLY);
	if(fd == FAIL){
		return;
	}

	ret = read(fd, buf, size);
	if(ret <= 0){
		close(fd);
		return;
	}

	close(fd);
}/*end fileRead*/

/*___________________________________________________________________
**      function name: mdio_read
**      descriptions:
**         Use mdio interface to read mii register.
**
**      parameters:
**         skfd: Ioctl socket descriptor.
**         ifr: Interfce 
**         phy_id: PHY address 
**         reg_num: Register  address
**  
**      global:
**           ethcmdDbg
**             
**      return:
**          The value of mii register.
**	    
**      call:
**           None
**      
**      revision:
**      1. Here 2009/11/7
**      2. WPLin 2012/08/12
**_______________________________________________________________
*/
int
mdio_read(int skfd, struct ifreq ifr, u32 phy_id, u32 reg_num)
{
	struct mii_data mii;

	ifr.ifr_data = (char *)(&mii);
	mii.phy_id = phy_id;
	mii.reg_num = reg_num;

	#ifdef EXT_SWITCH_PHY_CONNECT_INT_MDIO
		#if defined(JUDGE_SWITCH_SCENARIO_BY_751020_SUBMODEL)
	if (_DefaultUseExtMT7530)
	{
		mii.phy_id += EXT_SWITCH_PHY_ADDR_OFFSET;
	}
	else
	{
//		mii.phy_id += INT_SWITCH_PHY_ADDR_OFFSET;
	}
		#elif defined(DEFAULT_USE_EXT_SWIC)
	mii.phy_id += EXT_SWITCH_PHY_ADDR_OFFSET;
		#else
//	mii.phy_id += INT_SWITCH_PHY_ADDR_OFFSET;
		#endif

	if (ioctl(skfd, SIOCGMIIREG, &ifr) < 0) {
		fprintf(stderr, "SIOCGMIIREG on %s failed: %s\n", ifr.ifr_name, strerror(errno));
		return (-1);
	}
	#else
		#if defined(JUDGE_SWITCH_SCENARIO_BY_751020_SUBMODEL)
	if (_DefaultUseExtMT7530)
	{
		if (ioctl(skfd, RAETH_GSWEXT_PHY_READ, &ifr) < 0) {
			fprintf(stderr, "RAETH_GSWEXT_PHY_READ on %s failed: %s\n", ifr.ifr_name, strerror(errno));
			return (-1);
		}
	}
	else
	{
		if (ioctl(skfd, SIOCGMIIREG, &ifr) < 0) {
			fprintf(stderr, "SIOCGMIIREG on %s failed: %s\n", ifr.ifr_name, strerror(errno));
			return (-1);
		}
	}
		#elif defined(DEFAULT_USE_EXT_SWIC)
	if (ioctl(skfd, RAETH_GSWEXT_PHY_READ, &ifr) < 0) {
		fprintf(stderr, "RAETH_GSWEXT_PHY_READ on %s failed: %s\n", ifr.ifr_name, strerror(errno));
		return (-1);
	}
		#else
	if (ioctl(skfd, SIOCGMIIREG, &ifr) < 0) {
		fprintf(stderr, "SIOCGMIIREG on %s failed: %s\n", ifr.ifr_name, strerror(errno));
		return (-1);
	}
		#endif
	#endif	//EXT_SWITCH_PHY_CONNECT_INT_MDIO

	#ifdef ETHCMD_DBG
	if(ethcmdDbg){
		fprintf(stderr, "mdio_read: phyid:%x reg_num:%x value:%x\r\n", phy_id, reg_num, mii.val_out);
	}
	#endif
	return mii.val_out;
}

//MTK120625 start ///YM
/*___________________________________________________________________
**      function name: mdio_read2
**      descriptions:
**         Use mdio interface to read mii register.
**
**      parameters:
**         skfd: Ioctl socket descriptor.
**         ifr: Interfce
**         u32 ext_switch: 0(int)/1(ext).
**         phy_id: PHY address
**         reg_num: Register  address
**
**      global:
**           ethcmdDbg
**
**      return:
**          The value of mii register.
**
**      call:
**           None
**
**      revision:
**      1. YM_Chan 2012/06/25
**      2. WPLin 2012/08/12
**_______________________________________________________________
*/
int
mdio_read2(int skfd, struct ifreq ifr, u32 ext_switch, u32 phy_id, u32 reg_num)
{
	struct mii_data mii;

	ifr.ifr_data = (char *)(&mii);
	mii.phy_id = phy_id;
	mii.reg_num = reg_num;

	#ifdef EXT_SWITCH_PHY_CONNECT_INT_MDIO
    #if defined(JUDGE_SWITCH_SCENARIO_BY_751020_SUBMODEL)
	if (_DefaultUseExtMT7530)
	{
		mii.phy_id += (ext_switch ? EXT_SWITCH_PHY_ADDR_OFFSET : 
		    INT_SWITCH_PHY_ADDR_OFFSET);
	}
    #elif defined(DEFAULT_USE_EXT_SWIC)
	mii.phy_id += (ext_switch ? EXT_SWITCH_PHY_ADDR_OFFSET : 
	    INT_SWITCH_PHY_ADDR_OFFSET);
    #else
    #endif
	if (ioctl(skfd, SIOCGMIIREG, &ifr) < 0) {
		fprintf(stderr, "SIOCGMIIREG on %s failed: %s\n", ifr.ifr_name, strerror(errno));
		return (-1);
	}
	#else
	if (ext_switch)
	{
		if (ioctl(skfd, RAETH_GSWEXT_PHY_READ, &ifr) < 0) {
			fprintf(stderr, "RAETH_GSWEXT_PHY_READ on %s failed: %s\n", ifr.ifr_name, strerror(errno));
			return (-1);
		}
	}
	else
	{
		if (ioctl(skfd, SIOCGMIIREG, &ifr) < 0) {
			fprintf(stderr, "SIOCGMIIREG on %s failed: %s\n", ifr.ifr_name, strerror(errno));
			return (-1);
		}
	}
	#endif	//EXT_SWITCH_PHY_CONNECT_INT_MDIO

	#ifdef ETHCMD_DBG
	if(ethcmdDbg){
		fprintf(stderr, "mdio_read2: phyid:%x reg_num:%x value:%x\r\n", phy_id, reg_num, mii.val_out);
	}
	#endif
	return mii.val_out;
}
//MTK120625 end///YM

/*___________________________________________________________________
**      function name: mdio_write
**      descriptions:
**         Use mdio interface to write mii register.
**
**      parameters:
**         skfd: Ioctl socket descriptor.
**         ifr: Interfce
**         phy_id: PHY address
**         reg_num: Register  address
**         value: Specify the value to fill into mii register.
**
**      global:
**           ethcmdDbg
**
**      return:
**          None
**
**      call:
**           None
**
**      revision:
**      1. Here 2009/11/7
**      2. WPLin 2012/08/12
**_______________________________________________________________
*/
void
mdio_write(int skfd, struct ifreq ifr, u32 phy_id, u32 reg_num, u32 value)
{
	struct mii_data mii;

	ifr.ifr_data = (char *)(&mii);
	mii.phy_id = phy_id;
	mii.reg_num = reg_num;
	mii.val_in = value;

	#ifdef EXT_SWITCH_PHY_CONNECT_INT_MDIO
		#if defined(JUDGE_SWITCH_SCENARIO_BY_751020_SUBMODEL)
	if (_DefaultUseExtMT7530)
	{
		mii.phy_id += EXT_SWITCH_PHY_ADDR_OFFSET;
	}
	else
	{
//		mii.phy_id += INT_SWITCH_PHY_ADDR_OFFSET;
	}
		#elif defined(DEFAULT_USE_EXT_SWIC)
	mii.phy_id += EXT_SWITCH_PHY_ADDR_OFFSET;
		#else
//	mii.phy_id += INT_SWITCH_PHY_ADDR_OFFSET;
		#endif

	if (ioctl(skfd, SIOCSMIIREG, &ifr) < 0) {
		fprintf(stderr, "SIOCSMIIREG on %s failed: %s\n", ifr.ifr_name, strerror(errno));
		return;
	}
	#else
		#if defined(JUDGE_SWITCH_SCENARIO_BY_751020_SUBMODEL)
	if (_DefaultUseExtMT7530)
	{
		if (ioctl(skfd, RAETH_GSWEXT_PHY_WRITE, &ifr) < 0) {
			fprintf(stderr, "RAETH_GSWEXT_PHY_WRITE on %s failed: %s\n", ifr.ifr_name, strerror(errno));
			return;
		}
	}
	else
	{
		if (ioctl(skfd, SIOCSMIIREG, &ifr) < 0) {
			fprintf(stderr, "SIOCSMIIREG on %s failed: %s\n", ifr.ifr_name, strerror(errno));
			return;
		}
	}
		#elif defined(DEFAULT_USE_EXT_SWIC)
	if (ioctl(skfd, RAETH_GSWEXT_PHY_WRITE, &ifr) < 0) {
		fprintf(stderr, "RAETH_GSWEXT_PHY_WRITE on %s failed: %s\n", ifr.ifr_name, strerror(errno));
		return;
	}
		#else
	if (ioctl(skfd, SIOCSMIIREG, &ifr) < 0) {
		fprintf(stderr, "SIOCSMIIREG on %s failed: %s\n", ifr.ifr_name, strerror(errno));
		return;
	}
		#endif
	#endif	//EXT_SWITCH_PHY_CONNECT_INT_MDIO

	#ifdef ETHCMD_DBG
	if(ethcmdDbg){
		fprintf(stderr, "mdio_write: phyid:%x reg_num:%x value:%x\r\n", phy_id, reg_num, value);
	}
	#endif
}/*end mdio_write*/

//MTK120625 start
/*___________________________________________________________________
**      function name: mdio_write2
**      descriptions:
**         Use mdio interface to write mii register.
**
**      parameters:
**         skfd: Ioctl socket descriptor.
**         ifr: Interfce
**         u32 ext_switch: 0(int)/1(ext).
**         phy_id: PHY address
**         reg_num: Register  address
**         value: Specify the value to fill into mii register.
**
**      global:
**           ethcmdDbg
**
**      return:
**          None
**
**      call:
**           None
**
**      revision:
**      1. YM_Chan 2012/06/25
**      2. WPLin 2012/08/12
**_______________________________________________________________
*/
void
mdio_write2(int skfd, struct ifreq ifr, u32 ext_switch, u32 phy_id, u32 reg_num, u32 value)
{
	struct mii_data mii;

	ifr.ifr_data = (char *)(&mii);
	mii.phy_id = phy_id;
	mii.reg_num = reg_num;
	mii.val_in = value;

	#ifdef EXT_SWITCH_PHY_CONNECT_INT_MDIO
		#if defined(JUDGE_SWITCH_SCENARIO_BY_751020_SUBMODEL)
	if (_DefaultUseExtMT7530)
	{
		mii.phy_id += (ext_switch ? EXT_SWITCH_PHY_ADDR_OFFSET : INT_SWITCH_PHY_ADDR_OFFSET);
	}
		#elif defined(DEFAULT_USE_EXT_SWIC)
	mii.phy_id += (ext_switch ? EXT_SWITCH_PHY_ADDR_OFFSET : INT_SWITCH_PHY_ADDR_OFFSET);
		#else
		#endif
	if (ioctl(skfd, SIOCSMIIREG, &ifr) < 0) {
		fprintf(stderr, "SIOCSMIIREG on %s failed: %s\n", ifr.ifr_name, strerror(errno));
		return;
	}
	#else
	if (ext_switch)
	{
		if (ioctl(skfd, RAETH_GSWEXT_PHY_WRITE, &ifr) < 0) {
			fprintf(stderr, "RAETH_GSWEXT_PHY_WRITE on %s failed: %s\n", ifr.ifr_name, strerror(errno));
			return;
		}
	}
	else
	{
		if (ioctl(skfd, SIOCSMIIREG, &ifr) < 0) {
			fprintf(stderr, "SIOCSMIIREG on %s failed: %s\n", ifr.ifr_name, strerror(errno));
			return;
		}
	}
	#endif	//EXT_SWITCH_PHY_CONNECT_INT_MDIO

	#ifdef ETHCMD_DBG
	if(ethcmdDbg){
		fprintf(stderr, "mdio_write2: phyid:%x reg_num:%x value:%x\r\n", phy_id, reg_num, value);
	}
	#endif
}/*end mdio_write*/
//MTK120625 end

#ifdef MT7530_PHY_VERIFY
/*___________________________________________________________________
**      function name: mdio_read45
**      descriptions:
**         Use mdio interface to read mii register.
**
**      parameters:
**         skfd: Ioctl socket descriptor.
**         ifr: Interfce
**         phy_id: PHY address
**         reg_num: Register  address
**
**      global:
**           ethcmdDbg
**
**      return:
**          The value of mii register.
**
**      call:
**           None
**
**      revision:
**      1. WPLin 2012/08/30
**_______________________________________________________________
*/
int
mdio_read45(int skfd, struct ifreq ifr, u32 portAddr, u32 devAddr, u32 regAddr)
{
	struct _gsw_phy_cl45 mii45;

	ifr.ifr_data = (char *)(&mii45);
	mii45.portAddr = portAddr;
	mii45.devAddr = devAddr;
	mii45.regAddr = regAddr;

	if (ioctl(skfd, RAETH_GSWEXT_PHY_READ_45, &ifr) < 0) {
		fprintf(stderr, "RAETH_GSWEXT_PHY_READ_45 on %s failed: %s\n", ifr.ifr_name, strerror(errno));
		return -1;
	}

//	#ifdef ETHCMD_DBG
//	if(ethcmdDbg){
		fprintf(stderr, "mdio_read45(): portAddr:0x%02x devAddr:0x%02x regAddr:0x%04x value:0x%04x\r\n", portAddr, devAddr, regAddr, mii45.miiData);
//	}
//	#endif
	return mii45.miiData;
}

/*___________________________________________________________________
**      function name: mdio_write45
**      descriptions:
**         Use mdio interface to write mii register.
**
**      parameters:
**         skfd: Ioctl socket descriptor.
**         ifr: Interfce
**         phy_id: PHY address
**         reg_num: Register  address
**         value: Specify the value to fill into mii register.
**
**      global:
**           ethcmdDbg
**
**      return:
**          None
**
**      call:
**           None
**
**      revision:
**      1. WPLin 2012/08/30
**_______________________________________________________________
*/
void
mdio_write45(int skfd, struct ifreq ifr, u32 portAddr, u32 devAddr, u32 regAddr, u32 miiData)
{
	struct _gsw_phy_cl45 mii45;

	ifr.ifr_data = (char *)(&mii45);
	mii45.portAddr = portAddr;
	mii45.devAddr = devAddr;
	mii45.regAddr = regAddr;
	mii45.miiData = miiData;

	if (ioctl(skfd, RAETH_GSWEXT_PHY_WRITE_45, &ifr) < 0) {
		fprintf(stderr, "RAETH_GSWEXT_PHY_WRITE_45 on %s failed: %s\n", ifr.ifr_name, strerror(errno));
	}

//	#ifdef ETHCMD_DBG
//	if(ethcmdDbg){
		fprintf(stderr, "mdio_write(): portAddr:0x%02x devAddr:0x%02x regAddr:0x%04x value:0x%04x\r\n", portAddr, devAddr, regAddr, miiData);
//	}
//	#endif
}/*end mdio_write*/

#endif

/*___________________________________________________________________
**      function name: gsw_phy_read
**      descriptions:
**         Use mdio interface to read mii register.
**
**      parameters:
**         skfd: Ioctl socket descriptor.
**         ifr: Interfce
**         phy_id: PHY address
**         reg_num: Register  address
**
**      global:
**           ethcmdDbg
**
**      return:
**          The value of mii register.
**
**      call:
**           None
**
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
int
gsw_phy_read(int skfd, struct ifreq ifr,int phy_addr, int phy_reg)
{
	struct mii_data mii;

	ifr.ifr_data = (char *)(&mii);
	mii.phy_id = phy_addr;
	mii.reg_num = phy_reg;

	if (ioctl(skfd, RAETH_GSW_PHY_READ, &ifr) < 0) {
		fprintf(stderr, "RAETH_GSW_PHY_READ on %s failed: %s\n", ifr.ifr_name,
			strerror(errno));
		return -1;
	}
	#ifdef ETHCMD_DBG
	if(ethcmdDbg){
		fprintf(stderr, "gsw_phy_read: phy_addr:%x phy_reg:%x value:%x\r\n", phy_addr, phy_reg, mii.val_out);
	}
	#endif
	return mii.val_out;
}
/*___________________________________________________________________
**      function name: gsw_phy_write
**      descriptions:
**         Use mdio interface to write mii register.
**
**      parameters:
**         skfd: Ioctl socket descriptor.
**         ifr: Interfce
**         phy_id: PHY address
**         reg_num: Register  address
**         value: Specify the value to fill into mii register.
**
**      global:
**           ethcmdDbg
**
**      return:
**          None
**
**      call:
**           None
**
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
void
gsw_phy_write(int skfd, struct ifreq ifr,int phy_addr, int phy_reg, int value)
{
	struct mii_data mii;

	ifr.ifr_data = (char *)(&mii);
	mii.phy_id = phy_addr;
	mii.reg_num = phy_reg;
	mii.val_in = value;

	if (ioctl(skfd, RAETH_GSW_PHY_WRITE, &ifr) < 0) {
		fprintf(stderr, "RAETH_GSW_PHY_WRITE on %s failed: %s\n", ifr.ifr_name,
		strerror(errno));
	}
	#ifdef ETHCMD_DBG
	if(ethcmdDbg){
		fprintf(stderr, "gsw_reg_write: phy_addr:%x phy_reg:%x value:%x\r\n", phy_addr, phy_reg, mii.val_out);
	}
	#endif
}/*end gsw_reg_write*/

#if defined(TCSUPPORT_CPU_RT65168)
/*___________________________________________________________________
**      function name: gsw_phy_bp_read
**      descriptions:
**         Use mdio interface to read mii register in bypass mode.
**
**      parameters:
**         skfd: Ioctl socket descriptor.
**         ifr: Interfce
**         phy_id: PHY address
**         reg_num: Register  address
**
**      global:
**           ethcmdDbg
**
**      return:
**          The value of mii register.
**
**      call:
**           None
**
**      revision:
**      1. PTChen 2013/01/30
**_______________________________________________________________
*/
int
gsw_phy_bp_read(int skfd, struct ifreq ifr,int phy_addr, int phy_reg)
{
	struct mii_data mii;

	ifr.ifr_data = (char *)(&mii);
	mii.phy_id = phy_addr;
	mii.reg_num = phy_reg;

	if (ioctl(skfd, RAETH_GSW_PHY_BP_READ, &ifr) < 0) {
		fprintf(stderr, "RAETH_GSW_PHY_BP_READ on %s failed: %s\n", ifr.ifr_name,
			strerror(errno));
		return -1;
	}
	#ifdef ETHCMD_DBG
	if(ethcmdDbg){
		fprintf(stderr, "gsw_phy_bp_read: phy_addr:%x phy_reg:%x value:%x\r\n", phy_addr, phy_reg, mii.val_out);
	}
	#endif
	return mii.val_out;
}
/*___________________________________________________________________
**      function name: gsw_phy_bp_write
**      descriptions:
**         Use mdio interface to write in bypass mode.
**
**      parameters:
**         skfd: Ioctl socket descriptor.
**         ifr: Interfce
**         phy_id: PHY address
**         reg_num: Register  address
**         value: Specify the value to fill into mii register.
**
**      global:
**           ethcmdDbg
**
**      return:
**          None
**
**      call:
**           None
**
**      revision:
**      1. PTChen 2013/01/30
**_______________________________________________________________
*/
void
gsw_phy_bp_write(int skfd, struct ifreq ifr,int phy_addr, int phy_reg, int value)
{
	struct mii_data mii;

	ifr.ifr_data = (char *)(&mii);
	mii.phy_id = phy_addr;
	mii.reg_num = phy_reg;
	mii.val_in = value;

	if (ioctl(skfd, RAETH_GSW_PHY_BP_WRITE, &ifr) < 0) {
		fprintf(stderr, "RAETH_GSW_PHY_BP_WRITE on %s failed: %s\n", ifr.ifr_name,
		strerror(errno));
	}
	#ifdef ETHCMD_DBG
	if(ethcmdDbg){
		fprintf(stderr, "gsw_reg_write: phy_addr:%x phy_reg:%x value:%x\r\n", phy_addr, phy_reg, mii.val_out);
	}
	#endif
}/*end gsw_reg_bp_write*/
#endif


/*___________________________________________________________________
**      function name: mdio_reg_write
**      descriptions:
**         Use mdio interface to read mii register.
**
**      parameters:
**         skfd: Ioctl socket descriptor.
**         ifr: Interfce 
**         phy_id: PHY address 
**         reg_num: Register  address
**  
**      global:
**           ethcmdDbg
**             
**      return:
**          The value of mii register.
**	    
**      call:
**           None
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
int 
mdio_reg_read(int skfd, struct ifreq ifr,int offset)
{
	gsw_reg reg;
	ifr.ifr_data = (char *)(&reg);
	reg.off = offset;

	if (ioctl(skfd, RAETH_REG_READ, &ifr) < 0) {
		fprintf(stderr, "RAETH_REG_READ on %s failed: %s\n", ifr.ifr_name,
			strerror(errno));
		return -1;
	}
	#ifdef ETHCMD_DBG
	if(ethcmdDbg){
		fprintf(stderr, "gsw_reg_read: offset:%x value:%x\r\n", offset, reg.val);
	}
	#endif
	return reg.val;
}

/*___________________________________________________________________
**      function name: mdio_reg_write
**      descriptions:
**         Use mdio interface to write mii register.
**
**      parameters:
**         skfd: Ioctl socket descriptor.
**         ifr: Interfce 
**         phy_id: PHY address 
**         reg_num: Register  address
**         value: Specify the value to fill into mii register.
**  
**      global:
**           ethcmdDbg
**             
**      return:
**          None
**	    
**      call:
**           None
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
void
mdio_reg_write(int skfd, struct ifreq ifr,int offset, int value)
{
	gsw_reg reg;
	ifr.ifr_data = (char *)(&reg);
	reg.off = offset;
	reg.val = value;

	if (ioctl(skfd, RAETH_REG_WRITE, &ifr) < 0) {
		fprintf(stderr, "RAETH_REG_WRITE on %s failed: %s\n", ifr.ifr_name,
		strerror(errno));
	}
	#ifdef ETHCMD_DBG
	if(ethcmdDbg){
		fprintf(stderr, "gsw_reg_write: offset:%x value:%x\r\n", offset, value);
	}
	#endif
}/*end mdio_reg_write*/


/*___________________________________________________________________
**      function name: gsw_reg_read
**      descriptions:
**         Use mdio interface to read mii register.
**
**      parameters:
**         skfd: Ioctl socket descriptor.
**         ifr: Interfce 
**         phy_id: PHY address 
**         reg_num: Register  address
**  
**      global:
**           ethcmdDbg
**             
**      return:
**          The value of mii register.
**	    
**      call:
**           None
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
int
gsw_reg_read(int skfd, struct ifreq ifr, u32 offset)
{
	gsw_reg reg;
	ifr.ifr_data = (char *)(&reg);
	reg.off = offset;

#if defined(TCSUPPORT_CPU_EN7512) || defined(TCSUPPORT_CPU_EN7521)
    if(use_extern_switch){
        if (ioctl(skfd, RAETH_GSWEXT_REG_READ, &ifr) < 0) {
			fprintf(stderr, "RAETH_GSWEXT_REG_READ on %s failed: %s\n", ifr.ifr_name, strerror(errno));
			return (-1);
		}
    }else{
        if (ioctl(skfd, RAETH_GSW_REG_READ, &ifr) < 0) {
			fprintf(stderr, "RAETH_GSW_REG_READ on %s failed: %s\n", ifr.ifr_name, strerror(errno));
			return (-1);
		}
    }
#else
	#if defined(JUDGE_SWITCH_SCENARIO_BY_751020_SUBMODEL)
	if (_DefaultUseExtMT7530)
	{
		if (ioctl(skfd, RAETH_GSWEXT_REG_READ, &ifr) < 0) {
			fprintf(stderr, "RAETH_GSWEXT_REG_READ on %s failed: %s\n", ifr.ifr_name, strerror(errno));
			return (-1);
		}
	}
	else
	{
		if (ioctl(skfd, RAETH_GSW_REG_READ, &ifr) < 0) {
			fprintf(stderr, "RAETH_GSW_REG_READ on %s failed: %s\n", ifr.ifr_name, strerror(errno));
			return (-1);
		}
	}
	#elif defined(DEFAULT_USE_EXT_SWIC)
	if (ioctl(skfd, RAETH_GSWEXT_REG_READ, &ifr) < 0) {
		fprintf(stderr, "RAETH_GSWEXT_REG_READ on %s failed: %s\n", ifr.ifr_name, strerror(errno));
		return (-1);
	}
	#else
	if (ioctl(skfd, RAETH_GSW_REG_READ, &ifr) < 0) {
		fprintf(stderr, "RAETH_GSW_REG_READ on %s failed: %s\n", ifr.ifr_name, strerror(errno));
		return (-1);
	}
	#endif

	#ifdef ETHCMD_DBG
	if(ethcmdDbg){
		fprintf(stderr, "gsw_reg_read: offset:%x value:%x\r\n", offset, reg.val);
	}
	#endif
#endif	
	return reg.val;
}
//MTK120625 start
/*___________________________________________________________________
**      function name: gsw_reg_read2
**      descriptions:
**         Use mdio interface to read mii register.
**
**      parameters:
**         skfd: Ioctl socket descriptor.
**         ifr: Interfce
**         u32 ext_switch: 0(int)/1(ext).
**         phy_id: PHY address
**         reg_num: Register  address
**
**      global:
**           ethcmdDbg
**
**      return:
**          The value of mii register.
**
**      call:
**           None
**
**      revision:
**      1. YM_Chan 2012/06/25
**      2. WPLin 2012/08/12
**_______________________________________________________________
*/
int
gsw_reg_read2(int skfd, struct ifreq ifr, u32 ext_switch, u32 offset)
{
	gsw_reg reg;
	ifr.ifr_data = (char *)(&reg);
	reg.off = offset;

	if (ext_switch)
	{
		if (ioctl(skfd, RAETH_GSWEXT_REG_READ, &ifr) < 0) {
			fprintf(stderr, "RAETH_GSWEXT_REG_READ on %s failed: %s\n", ifr.ifr_name, strerror(errno));
			return (-1);
		}
	}
	else
	{
		if (ioctl(skfd, RAETH_GSW_REG_READ, &ifr) < 0) {
			fprintf(stderr, "RAETH_GSW_REG_READ on %s failed: %s\n", ifr.ifr_name, strerror(errno));
			return (-1);
		}
	}

	#ifdef ETHCMD_DBG
	if(ethcmdDbg){
		fprintf(stderr, "gsw_reg_read2: offset:%x value:%x\r\n", offset, reg.val);
	}
	#endif
	return reg.val;
}
//MTK120625 end

/*___________________________________________________________________
**      function name: gsw_reg_write
**      descriptions:
**         Use mdio interface to write mii register.
**
**      parameters:
**         skfd: Ioctl socket descriptor.
**         ifr: Interfce 
**         phy_id: PHY address 
**         reg_num: Register  address
**         value: Specify the value to fill into mii register.
**  
**      global:
**           ethcmdDbg
**             
**      return:
**          None
**	    
**      call:
**           None
**      
**      revision:
**      1. Here 2009/11/7
**      2. WPLin 2012/08/12
**_______________________________________________________________
*/
void
gsw_reg_write(int skfd, struct ifreq ifr, u32 offset, u32 value)
{
	gsw_reg reg;
	ifr.ifr_data = (char *)(&reg);
	reg.off = offset;
	reg.val = value;
#if defined(TCSUPPORT_CPU_EN7512) || defined(TCSUPPORT_CPU_EN7521)
	if(use_extern_switch){
		if (ioctl(skfd, RAETH_GSWEXT_REG_WRITE, &ifr) < 0) {
			fprintf(stderr, "RAETH_GSWEXT_REG_WRITE on %s failed: %s\n", ifr.ifr_name, strerror(errno));
			return;
		}
	}else{
		if (ioctl(skfd, RAETH_GSW_REG_WRITE, &ifr) < 0) {
			fprintf(stderr, "RAETH_GSW_REG_WRITE on %s failed: %s\n", ifr.ifr_name, strerror(errno));
			return;
		}
	}
#else

	#if defined(JUDGE_SWITCH_SCENARIO_BY_751020_SUBMODEL)
	if (_DefaultUseExtMT7530)
	{
		if (ioctl(skfd, RAETH_GSWEXT_REG_WRITE, &ifr) < 0) {
			fprintf(stderr, "RAETH_GSWEXT_REG_WRITE on %s failed: %s\n", ifr.ifr_name, strerror(errno));
			return;
		}
	}
	else
	{
		if (ioctl(skfd, RAETH_GSW_REG_WRITE, &ifr) < 0) {
			fprintf(stderr, "RAETH_GSW_REG_WRITE on %s failed: %s\n", ifr.ifr_name, strerror(errno));
			return;
		}
	}
	#elif defined(DEFAULT_USE_EXT_SWIC)
	if (ioctl(skfd, RAETH_GSWEXT_REG_WRITE, &ifr) < 0) {
		fprintf(stderr, "RAETH_GSWEXT_REG_WRITE on %s failed: %s\n", ifr.ifr_name, strerror(errno));
		return;
	}
	#else
	if (ioctl(skfd, RAETH_GSW_REG_WRITE, &ifr) < 0) {
		fprintf(stderr, "RAETH_GSW_REG_WRITE on %s failed: %s\n", ifr.ifr_name, strerror(errno));
		return;
	}
	#endif
#endif	

	#ifdef ETHCMD_DBG
	if(ethcmdDbg){
		fprintf(stderr, "gsw_reg_write: offset:%x value:%x\r\n", offset, value);
	}
	#endif
}/*end gsw_reg_write*/

#define DSTQ_ARG_TYPE 1
#define DSTQ_ARG_VAL  2
int
gsw_dstq_write(int skfd, struct ifreq ifr, int argc, char *argv[])
{
	gsw_reg reg;
	ifr.ifr_data = (char *)(&reg);

	if(!strcmp(argv[DSTQ_ARG_TYPE],"mode")){
		/* [off] 0: switch, 1: FC */
		reg.off = 0;	
		if( (argv[DSTQ_ARG_VAL][0]!='0') && (argv[DSTQ_ARG_VAL][0]!='1') && (argv[DSTQ_ARG_VAL][0]!='2')){
			return -1;
		}else{
			/* [val] 0: disable, 1: to gdm, 2: to qdma */
			reg.val = argv[DSTQ_ARG_VAL][0]-'0';	
		}
	}else if(!strcmp(argv[DSTQ_ARG_TYPE],"fc")){
		/* [off] 0: switch, 1: FC */
		reg.off = 1;
		if( (argv[DSTQ_ARG_VAL][0]!='0') && (argv[DSTQ_ARG_VAL][0]!='1')){	
			return -1;
		}else{
			/* [val] 0: disable, 1: enable */
			reg.val = argv[DSTQ_ARG_VAL][0]-'0';
		}
	}else{
		return -1;
	}

	if (ioctl(skfd, RAETH_DSTQ, &ifr) < 0) {
		fprintf(stderr, "RAETH_GSWEXT_REG_WRITE on %s failed: %s\n", ifr.ifr_name, strerror(errno));
		return 0;
	}
	return 0;
}/*end gsw_dstq_write*/

//MTK120625 start
/*___________________________________________________________________
**      function name: gsw_reg_write2
**      descriptions:
**         Use mdio interface to write mii register.
**
**      parameters:
**         skfd: Ioctl socket descriptor.
**         ifr: Interfce
**         u32 ext_switch: 0(int)/1(ext).
**         phy_id: PHY address
**         reg_num: Register  address
**         value: Specify the value to fill into mii register.
**
**      global:
**           ethcmdDbg
**
**      return:
**          None
**
**      call:
**           None
**
**      revision:
**      1. YM_Chan 2012/06/25
**      2. WPLin 2012/08/12
**_______________________________________________________________
*/
void
gsw_reg_write2(int skfd, struct ifreq ifr, u32 ext_switch, u32 offset, u32 value)
{
	gsw_reg reg;
	ifr.ifr_data = (char *)(&reg);
	reg.off = offset;
	reg.val = value;

	if (ext_switch)
	{
		if (ioctl(skfd, RAETH_GSWEXT_REG_WRITE, &ifr) < 0) {
			fprintf(stderr, "RAETH_GSWEXT_REG_WRITE on %s failed: %s\n", ifr.ifr_name, strerror(errno));
			return;
		}
	}
	else
	{
		if (ioctl(skfd, RAETH_GSW_REG_WRITE, &ifr) < 0) {
			fprintf(stderr, "RAETH_GSW_REG_WRITE on %s failed: %s\n", ifr.ifr_name, strerror(errno));
			return;
		}
	}

	#ifdef ETHCMD_DBG
	if(ethcmdDbg){
		fprintf(stderr, "gsw_reg_write2: offset:%x value:%x\r\n", offset, value);
	}
	#endif
}/*end gsw_reg_write*/
//MTK120625 end

/*___________________________________________________________________
**      function name: searchSwic
**      descriptions:
**         Accroding the specify mii register to know which is the switch ic.
**
**      parameters:
**         skfd: Ioctl socket descriptor.
**         ifr: Interfce 
**  
**      global:
**           swcfg
**           ethcmdDbg
**             
**      return:
**          Switch Vender ID.
**	    
**      call:
**           mdio_read
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
void
searchSwic(int skfd, struct ifreq ifr){
	int i, found = 0;

	swcfg.swic_id=UNKNOW_SWIC;
	for(i=0; i<NSWIC; i++){
		if((mdio_read(skfd, ifr, swic_id[i].phy_id1, swic_id[i].reg1)==swic_id[i].id1)
		&&(mdio_read(skfd, ifr, swic_id[i].phy_id2, swic_id[i].reg2)==swic_id[i].id2)){
			#ifdef ETHCMD_DBG
			if(ethcmdDbg){
				fprintf(stderr,"%s\r\n",swic_id[i].name);
			}
			#endif
			swcfg.swic_id=swic_id[i].swic_id;
			found = 1;
			break;
		} else if ((((gsw_reg_read(skfd, ifr, swic_id[i].reg1)) >> 16)) == (swic_id[i].id1)) {
			swcfg.swic_id=swic_id[i].swic_id;
			found = 1;
			break;
		} 
		#if defined(TCSUPPORT_CPU_RT65168)
		else if ((gsw_phy_bp_read(skfd, ifr, swic_id[i].phy_id1, swic_id[i].reg1)==swic_id[i].id1)
		&&(gsw_phy_bp_read(skfd, ifr, swic_id[i].phy_id2, swic_id[i].reg2)==swic_id[i].id2)) {
			swcfg.swic_id=swic_id[i].swic_id;
			found = 1;
			break;
		}
		#endif
	}

	#ifdef JUDGE_SWITCH_SCENARIO_BY_751020_SUBMODEL
	if (swcfg.swic_id == MT7530){

		np_hw_id = mdio_reg_read(skfd, ifr, 0xBFB00064);
		mt751020_submodel_id = mdio_reg_read(skfd, ifr, MT751020_SUBMODEL_REG);
		if ((np_hw_id == -1) || (mt751020_submodel_id == -1))
		{
			np_hw_id = 0;			// 0 means uninitialized/invalid
			mt751020_submodel_id = 0;	// 0 means uninitialized/invalid
		}

		if (_isMT751020){
			efuse_extreme_mode = mdio_reg_read(skfd, ifr, 0xBFBF8214/*EFUSE_EXTREME_MODE_REG*/);
			if (efuse_extreme_mode == -1){
				efuse_extreme_mode = 0;
			}
		}
		//printf("np_hw_id = 0x%08X, mt751020_submodel_id = %08X\n", np_hw_id, mt751020_submodel_id);
	}
	#endif
	if (found == 1){
		if(swic_id[i].api_init!=NULL){
			swic_id[i].api_init(&swic);
		}
	}
} /*end searchSwic*/
/*___________________________________________________________________
**      function name: tc2206_init_api
**      descriptions:
**         Register the tc2206 relate function.
**
**      parameters:
**         swic: switch IC api.
**  
**      global:
**           None
**             
**      return:
**          None
**	    
**      call:
**           macTC2206VlanActive
**           macTC2206VlanSetPvid
**           macTC2206VlanSetVid
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
void 
tc2206_init_api(swic_api_t* swic){
	swic->hw_igmpsnoop=macTC2206Igmpsnoop;
	swic->hw_fastleave=macTC2206IgmpFastLeave;
	swic->spTag=NULL;
	swic->vlan_active=macTC2206VlanActive;
	swic->vlan_pvid=macTC2206VlanSetPvid;
	swic->vlan_get_pvid=macTC2206VlanGetPvid;
	swic->vlan_vid=macTC2206VlanSetVid;
	swic->vlan_get_vtbl=macTC2206VlanGetVtbl;
	swic->vlan_pvid_disp=NULL;
	swic->vlan_disp=macTC2206VlanDisp;
	swic->vlan_getCap=macTC2206VlanCap;
}/*end tc2206_init_api*/

/*___________________________________________________________________
**      function name: tc2206_init_api
**      descriptions:
**         Register the tc2206 relate function.
**
**      parameters:
**         swic: switch IC api.
**  
**      global:
**           None
**             
**      return:
**          None
**	    
**      call:
**           macIP175CVlanActive
**           macIP175CVlanSetPvid
**           macIP175CVlanSetVid
**
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
void 
ip175c_init_api(swic_api_t* swic){
	swic->hw_igmpsnoop=NULL;
	swic->hw_fastleave=NULL;
	swic->spTag=NULL;
	swic->vlan_active=macIP175CVlanActive;
	swic->vlan_pvid=macIP175CVlanSetPvid;
	swic->vlan_get_pvid=NULL;
	swic->vlan_vid=macIP175CVlanSetVid;
	swic->vlan_get_vtbl=NULL;
	swic->vlan_pvid_disp=NULL;
	swic->vlan_disp=NULL;
	swic->vlan_getCap=macIP175CVlanCap;
}/*end ip175c_init_api*/

/*___________________________________________________________________
**      function name: rt63365_init_api
**      descriptions:
**         Register the rt63365 relate function.
**
**      parameters:
**         swic: switch IC api.
**  
**      global:
**           None
**             
**      return:
**          None
**	    
**      call:
**           macRT63365VlanActive
**           macRT63365VlanSetPvid
**           macRT63365VlanSetVid
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
void
rt63365_init_api(swic_api_t* swic){
	swic->hw_igmpsnoop=macRT63365Igmpsnoop;
	swic->hw_fastleave=macRT63365IgmpFastLeave;
	swic->spTag=NULL;
	swic->vlan_active=macRT63365VlanActive;
	swic->vlan_pvid=macRT63365VlanSetPvid;
	swic->vlan_get_pvid=macRT63365VlanGetPvid;
	#ifdef EXTEND_SETVID_PARAM
	swic->vlan_vid=macRT63365VlanSetVid;
	#else
		#if defined(SQA_VERIFY)
	swic->sqa_vlan_vid=macRT63365VlanSetVid;
	swic->vlan_vid=NULL;
		#else
	swic->vlan_vid=macRT63365VlanSetVid;
		#endif
	#endif
	swic->vlan_get_vtbl=macRT63365VlanGetVtbl;
	swic->vlan_pvid_disp=NULL;
	swic->vlan_disp=macRT63365VlanDisp;
	swic->vlan_getCap=macRT63365VlanCap;
}/*end tc2206_init_api*/

//#if defined(SQA_VERIFY)
/*___________________________________________________________________
**      function name: mt7530_init_api
**      descriptions:
**         Register the mt7530 relate function.
**
**      parameters:
**         swic: switch IC api.
**
**      global:
**           None
**
**      return:
**          None
**
**      call:
**           macMT7530VlanActive
**           macMT7530VlanSetPvid
**           macMT7530VlanSetVid
**
**      revision:
**      1. Frank 2011/07/06
**_______________________________________________________________
*/
void
mt7530_init_api(swic_api_t* swic){
	swic->hw_igmpsnoop=macRT63365Igmpsnoop;
	swic->hw_fastleave=macRT63365IgmpFastLeave;
	swic->spTag=NULL;
#if defined(TCSUPPORT_CPU_EN7512) || defined(TCSUPPORT_CPU_EN7521)
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();	
	use_extern_switch = detect_switch(swcfg);

	swic->vlan_active=macEN7512VlanActive;
#else
#ifdef JUDGE_SWITCH_SCENARIO_BY_751020_SUBMODEL
	if((_isMT7520S || _isMT7520 || _isMT7520G || _isMT7525 || _isMT7525G) && (-1 == get_wan_port()))
		swic->vlan_active=macRT7530VlanActive;
	else
#endif

		swic->vlan_active=macRT63365VlanActive;
#endif

	swic->vlan_pvid=macRT63365VlanSetPvid;
	swic->vlan_get_pvid=macRT63365VlanGetPvid;
	#ifdef EXTEND_SETVID_PARAM
	#ifdef TCSUPPORT_CPU_MT7505//7505 vlan entry is 16, and set vlan in the same way with 63365
		swic->vlan_vid=macRT63365VlanSetVid;
	#else
	    swic->vlan_vid=macMT7530VlanSetVid;
	#endif
	#else
		#if defined(SQA_VERIFY)
	#ifdef TCSUPPORT_CPU_MT7505//7505 vlan entry is 16, and set vlan in the same way with 63365
	    swic->sqa_vlan_vid=macRT63365VlanSetVid;
	#else
	    swic->sqa_vlan_vid=macMT7530VlanSetVid;
	#endif
	swic->vlan_vid=NULL;
	
		#else
		
	#ifdef TCSUPPORT_CPU_MT7505//7505 vlan entry is 16, and set vlan in the same way with 63365
		swic->vlan_vid=macRT63365VlanSetVid;
		#else
	swic->vlan_vid=macMT7530VlanSetVid;
		#endif
	#endif
	#endif
	
	#ifdef TCSUPPORT_CPU_MT7505//7505 vlan entry is 16, and set vlan in the same way with 63365	
	swic->vlan_get_vtbl=macRT63365VlanGetVtbl;
	swic->vlan_pvid_disp=NULL;
	swic->vlan_disp=macRT63365VlanDisp;
	swic->vlan_getCap=macRT63365VlanCap;
	#else
	swic->vlan_get_vtbl=macMT7530VlanGetVtbl;
	swic->vlan_pvid_disp=NULL;
	swic->vlan_disp=macMT7530VlanDisp;
	swic->vlan_getCap=macMT7530VlanCap;
	#endif
}/*end mt7530_init_api*/

//#endif //SQA_VERIFY

/*___________________________________________________________________
**      function name: init_swconfig
**      descriptions:
**         Init the switch ic and register switch ic api function.
**
**      parameters:
**         None
**  
**      global:
**           swcfg
**             
**      return:
**          None
**	    
**      call:
**           searchSwic
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
int 
init_swconfig(void){
	char buf[32];
	/*init swcfg*/
	memset(&swcfg, 0, sizeof(struct swconfig));
	
	/* Open a basic socket.*/
	if ((swcfg.skfd = socket(AF_INET, SOCK_DGRAM,0)) < 0) {
		perror("socket");
		exit(-1);
	}

	/* port reverse */
	memset(buf, 0, sizeof(buf));
	fileRead(PORT_REVERSE_PATH, buf, sizeof(buf));
	
	/* dbg info */
	//fprintf(stderr, "buf is %s\n", buf);

	
	if (buf[0] == '1') {
		//swcfg.reverse_flag = 1;
		swcfg.flag |= PORT_REVERSE;
	}

	/* special tag to vlan tag */
	memset(buf, 0, sizeof(buf));
	fileRead(STAG_TO_VTAG_PATH, buf, sizeof(buf));
	if (buf[0] == '1') {
		//swcfg.reverse_flag = 1;
		swcfg.flag |= STAG_TO_VTAG;
	}
	
	strncpy(swcfg.ifr.ifr_name, LAN_IF, IFNAMSIZ);
	if (ioctl(swcfg.skfd, SIOCGMIIPHY, &swcfg.ifr) < 0) {
		if (errno != ENODEV)
			fprintf(stderr, "SIOCGMIIPHY on '%s' failed: %s\n",
			LAN_IF, strerror(errno));
		close(swcfg.skfd);
		return -1;
	}
	/*Find  out the switch vernder id and register api function*/
	searchSwic(swcfg.skfd, swcfg.ifr);
	/*Set VLAN Capability*/
	if(swic.vlan_getCap){
		swic.vlan_getCap();
	}
	else{
		/* single port */
		macDefVlanCap();
	}

#if (defined TCSUPPORT_CPU_RT63365 || defined TCSUPPORT_CPU_MT7520)
	eth_port_init();
#endif
	return 0;
}/*end init_swconfig*/
int get_wan_port(void){
	char buf[4];
	FILE *fp = NULL;

	memset(buf, 0, sizeof(buf));
	fp = fopen("/proc/tc3162/eth_portmap","r");
	if(fp != NULL){
		if(fgets (buf, sizeof(buf), fp) != NULL){
			fclose(fp);	
				return atoi(buf);
		}	
		fclose(fp);	
	}
	return -1;
}

void eth_port_init(void){
	char buf[8];
	FILE *fp = NULL;
	int i, index, port;

	memset(lan_portmap, -1, sizeof(lan_portmap));
	
	fp = fopen("/proc/tc3162/eth_portmap","r");
	if(fp != NULL){
		memset(buf, 0, sizeof(buf));
		if(fgets (buf, sizeof(buf), fp) != NULL){
			memset(buf, 0, sizeof(buf));
			for (i=0; i<6; i++){
				if(fgets(buf, sizeof(buf), fp) != NULL){
					if (sscanf(buf,"%d %d", &index, &port) == 2){
						if (0<=index && index<=5){
							lan_portmap[index] = port;
						}	
					}
				}
			}	
		}	
		fclose(fp);
	}		
}

/*___________________________________________________________________
**      function name: showSwicName
**      descriptions:
**         Display switch IC name.
**
**      parameters:
**         swic: switch IC api.
**
**      global:
**         None
**
**      return:
**         None
**
**      call:
**         None
**
**      revision:
**      1. WPLin 2012/04/23
**_______________________________________________________________
*/
void showSwicName(void){
	int i;

#ifdef TCSUPPORT_CPU_MT7510
	printf("  [defined] TCSUPPORT_CPU_MT7510\n");
#endif
#ifdef TCSUPPORT_CPU_MT7505
	printf("  [defined] TCSUPPORT_CPU_MT7505\n");
#endif
#ifdef TCSUPPORT_CPU_MT7520
	printf("  [defined] TCSUPPORT_CPU_MT7520\n");
#endif
#ifdef TCSUPPORT_CPU_RT65168
	printf("  [defined] TCSUPPORT_CPU_RT65168\n");
#endif
#ifdef TCSUPPORT_CPU_TC3182
	printf("  [defined] TCSUPPORT_CPU_TC3182\n");
#endif
#ifdef DEFAULT_USE_EXT_SWIC
	printf("  [defined] DEFAULT_USE_EXT_SWIC\n");
#endif
#ifdef EXT_SWITCH_PHY_CONNECT_INT_MDIO
	printf("  [defined] EXT_SWITCH_PHY_CONNECT_INT_MDIO\n");
#endif
#ifdef JUDGE_SWITCH_SCENARIO_BY_751020_SUBMODEL
	printf("  [defined] JUDGE_SWITCH_SCENARIO_BY_751020_SUBMODEL\n");
#endif

	if(swcfg.swic_id != UNKNOW_SWIC){
		for(i=0; i<NSWIC; i++){
			if(swcfg.swic_id == swic_id[i].swic_id){
				printf("  detected switch IC : %s.\n", swic_id[i].name);
#ifdef JUDGE_SWITCH_SCENARIO_BY_751020_SUBMODEL
				if (swcfg.swic_id == MT7530){
					printf("  MT751020 submodel=0x%08X : _ExistExtMT7530=%d, _DefaultUseExtMT7530=%d\n", mt751020_submodel_id, (_ExistExtMT7530?1:0), (_DefaultUseExtMT7530?1:0));
				}
#endif

				return;
			}
		}
	}
	printf("  unknown switch IC.\n");
}
