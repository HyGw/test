#include <linux/gfp.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <asm/tc3162/tc3162.h>

#define U3_PHY_LIB
#include "mtk-phy.h"

#ifdef CONFIG_A60810_SUPPORT
#include "mtk-phy-a60810.h"
#endif

#ifdef CONFIG_PROJECT_7621
#include "mtk-phy-7621.h"
#endif

#ifdef CONFIG_A60810_SUPPORT
static const struct u3phy_operator a60810_operators = {
	.init = phy_init_a60810,
	.change_pipe_phase = phy_change_pipe_phase_a60810,
	.eyescan_init = eyescan_init_a60810,
	.eyescan = phy_eyescan_a60810,
	.u2_connect = u2_connect_a60810,
	.u2_disconnect = u2_disconnect_a60810,	
	.u2_slew_rate_calibration = u2_slew_rate_calibration_a60810,
};
#endif

#ifdef CONFIG_PROJECT_PHY
static struct u3phy_operator project_operators = {
	.init = phy_init,
	.change_pipe_phase = phy_change_pipe_phase,
	.eyescan_init = eyescan_init,
	.eyescan = phy_eyescan,
	//.u2_connect = u2_connect,
	//.u2_disconnect = u2_disconnect,	
	.u2_slew_rate_calibration = u2_slew_rate_calibration,
};
#endif

extern int (*I2CWriterPtr)(u8 DevAddr, u8 WordAddr, u8* data_value, u8 data_len);
extern int (*I2CReaderPtr)(u8 DevAddr, u8 WordAddr, u8* data_value, u8 data_len);
#define USB_PHY_DEV_ADDR	        0x60

void static setup_25M_PLL(void)
{
		if(readl(0xbfb0008c)&0x01){
			U3HWriteReg8(0xbfa80c1c, 0x18);
			U3HWriteReg8(0xbfa80c1d, 0x18);
			U3HWriteReg8(0xbfa80c1f, 0x18);
			U3HWriteReg32(0xbfa80c24, 0x18000000);
			U3HWriteReg32(0xbfa80c28, 0x18000000);
			U3HWriteReg32(0xbfa80c30, 0x18000000);
			U3HWriteReg32(0xbfa80c38, 0x004a004a);
			U3HWriteReg8(0xbfa80c3e, 0x4a);
			U3HWriteReg8(0xbfa80c3f, 0x0);
			U3HWriteReg8(0xbfa80c42, 0x48);
			U3HWriteReg8(0xbfa80c43, 0x0);
			U3HWriteReg8(0xbfa80c44, 0x48);
			U3HWriteReg8(0xbfa80c45, 0x0);
			U3HWriteReg8(0xbfa80c48, 0x48);
			U3HWriteReg8(0xbfa80c49, 0x0);

			U3HWriteReg8(0xbfa80b24, 0x90);
			U3HWriteReg8(0xbfa80b25, 0x1);
			U3HWriteReg32(0xbfa80b10, 0x1c000000);
			U3HWriteReg8(0xbfa80b0b, 0xe);
		}
    return;
}

PHY_INT32 u3phy_config()
{

	//Patch TxDetRx Timing for E1, from DR 20160421, Biker_20160516
	U3HWriteReg32(0xbfa80a28,  ((U3HReadReg32(0xbfa80a28) &(~(0x1ff<<9)) )|(0x10<<9) ));//rg_ssusb_rxdet_stb2_set[8:0]
	U3HWriteReg32(0xbfa80a2c,  ((U3HReadReg32(0xbfa80a2c) &(~0x1ff) )|0x10 ));//rg_ssusb_rxdet_stb2_set_p3[8:0]

	//Patch LFPS Filter Threshold for E1, from DR 20160421, Biker_20160516
	U3HWriteReg32 (0xbfa8090c,((U3HReadReg32(0xbfa8090c) &(~(0x3f<<16)) )|(0x34<<16) ));//rg_ssusb_fwake_th[5:0]


	setup_25M_PLL(); //Biker_20160516
		
	if(isEN7526D || isEN7526G || isEN7513 || isEN7513G || isEN7521G || isEN7586){
		writel(0xC0240008, 0xBFA8081C); /* port0 */
		writel(0xC0240000, 0xBFA8101C); /* port1 */
		printk(KERN_ERR "BGA USB PHY config");
		return 1;
	}else if(isEN7526F || isEN7512){
		writel(0xC0241580, 0xBFA8081C);
		writel(0xC0240000, 0xBFA8101C);
		printk(KERN_ERR "QFP USB PHY config");
		return 1;
	}else
		return 0;
}
PHY_INT32 u3phy_config_fpga(){
	u8 u1Value[4] = {0, 0, 0, 0};
	
	if(!I2CWriterPtr || !I2CReaderPtr)
		return 0;
	/********************************************************
	*				u3phy_config_Port0
	*********************************************************/
	u1Value[0] = 0x0; 
	I2CWriterPtr(USB_PHY_DEV_ADDR, 0xff, u1Value, 1);
	
	u1Value[0] = 0x55; 
	I2CWriterPtr(USB_PHY_DEV_ADDR, 0x05, u1Value, 1);

	u1Value[0] = 0x84; 
	I2CWriterPtr(USB_PHY_DEV_ADDR, 0x18, u1Value, 1);

	u1Value[0] = 0x10; 
	I2CWriterPtr(USB_PHY_DEV_ADDR, 0xff, u1Value, 1);

	u1Value[0] = 0x84; 
	I2CWriterPtr(USB_PHY_DEV_ADDR, 0x0a, u1Value, 1);

	u1Value[0] = 0x40; 
	I2CWriterPtr(USB_PHY_DEV_ADDR, 0xff, u1Value, 1);
	/*********************************************************/
	u1Value[0] = 0x46; 
	u1Value[1] = 0x0; 
	I2CWriterPtr(USB_PHY_DEV_ADDR, 0x38, u1Value, 2);

	u1Value[0] = 0x40; 
	u1Value[1] = 0x0; 
	I2CWriterPtr(USB_PHY_DEV_ADDR, 0x42, u1Value, 2);

	u1Value[0] = 0xab; 
	u1Value[1] = 0x0c; 
	I2CWriterPtr(USB_PHY_DEV_ADDR, 0x08, u1Value, 2);
	/*********************************************************/
	u1Value[0] = 0x71; 
	u1Value[1] = 0xe7; 
	u1Value[2] = 0x4f; 
	I2CWriterPtr(USB_PHY_DEV_ADDR, 0x0c, u1Value, 3);
	/*********************************************************/
	u1Value[0] = 0xe1; 
	I2CWriterPtr(USB_PHY_DEV_ADDR, 0x10, u1Value, 1);

	u1Value[0] = 0x5f; 
	I2CWriterPtr(USB_PHY_DEV_ADDR, 0x14, u1Value, 1);

	u1Value[0] = 0x60; 
	I2CWriterPtr(USB_PHY_DEV_ADDR, 0xff, u1Value, 1);

	u1Value[0] = 0x03; 
	I2CWriterPtr(USB_PHY_DEV_ADDR, 0x14, u1Value, 1);

	u1Value[0] = 0x0; 
	I2CWriterPtr(USB_PHY_DEV_ADDR, 0xff, u1Value, 1);

	u1Value[0] = 0x40; 
	I2CWriterPtr(USB_PHY_DEV_ADDR, 0x15, u1Value, 1);

	u1Value[0] = 0x0; 
	I2CWriterPtr(USB_PHY_DEV_ADDR, 0xff, u1Value, 1);

	u1Value[0] = 0x08; 
	I2CWriterPtr(USB_PHY_DEV_ADDR, 0x68, u1Value, 1);

	u1Value[0] = 0x04; 
	u1Value[1] = 0x0; 
	I2CWriterPtr(USB_PHY_DEV_ADDR, 0x6a, u1Value, 2);

	u1Value[0] = 0x10; 
	I2CWriterPtr(USB_PHY_DEV_ADDR, 0xff, u1Value, 1);

	u1Value[0] = 0x10; 
	u1Value[0] = 0x44; 
	I2CWriterPtr(USB_PHY_DEV_ADDR, 0x42, u1Value, 2);
	
	/*********************************************************/
	u1Value[0] = 0x50; 
	I2CWriterPtr(USB_PHY_DEV_ADDR, 0xff, u1Value, 1);

	u1Value[0] = 0x10; 
	I2CWriterPtr(USB_PHY_DEV_ADDR, 0x02, u1Value, 1);
	
	u1Value[0] = 0x4c; 
	I2CWriterPtr(USB_PHY_DEV_ADDR, 0x03, u1Value, 1);
	/********************************************************
	*				u3phy_config_Port1
	*********************************************************/
	I2cWriteReg(0x60, 0x6a, 0x00);	
	I2cWriteReg(0x60, 0x6b, 0x02);	
	I2cWriteReg(0x50, 0xab, 0x03);	
	return 1;
}

PHY_INT32 u2_slew_rate_calibration(void){
	PHY_INT32 i=0;
	PHY_INT32 j=0;
	PHY_INT32 fgRet = 0;
	PHY_INT32 u4FmOut = 0;	
	PHY_INT32 u4Tmp = 0;
	PHY_INT32 U2_PHYA_CR0[U2_port_num]={0xbfa80810, 0xbfa81010};

	for(j=0;j<U2_port_num;j++)
	{
		printk(KERN_ERR "port %d u2_slew_rate_calibration\n",j);
		// => RG_USB20_HSTX_SRCAL_EN = 1
		// enable HS TX SR calibration
		U3HWriteReg32((void *)(U2_PHYA_CR0[j]), (U3HReadReg32((void *)(U2_PHYA_CR0[j]))&(~RG_USB20_HSTX_SRCAL_EN)|((0x1)<<RG_USB20_HSTX_SRCAL_EN_OFST)));
		DRV_MSLEEP(1);	
		//printk("%x\n",U2_PHYA_CR0[j]);
		//printk("%x\n",U3HReadReg32((void *)(U2_PHYA_CR0[j])));
		// => RG_FRCK_EN = 1    
		// Enable free run clock
		U3HWriteReg32(0xbfa80110, (U3HReadReg32(0xbfa80110)&(~RG_FRCK_EN)|((0x1)<<RG_FRCK_EN_OFST)));
		//printk("%x\n",U3HReadReg32(0xbfa80110));
		// => RG_MONCLK_SEL = 0x0/0x1 for port0/port1
		// Setting MONCLK_SEL
		U3HWriteReg32(0xbfa80100, (U3HReadReg32(0xbfa80100)&(~RG_MONCLK_SEL)|((j)<<RG_MONCLK_SEL_OFST)));
		// => RG_CYCLECNT = 0x400
		// Setting cyclecnt = 0x400
		U3HWriteReg32(0xbfa80100, (U3HReadReg32(0xbfa80100)&(~RG_CYCLECNT)|((0x400)<<RG_CYCLECNT_OFST)));
		// => RG_FREQDET_EN = 1
		// Enable frequency meter
		U3HWriteReg32(0xbfa80100, (U3HReadReg32(0xbfa80100)&(~RG_FREQDET_EN)|((0x1)<<RG_FREQDET_EN_OFST)));
		//printk("%x\n",U3HReadReg32(0xbfa80100));
		// wait for FM detection done, set 10ms timeout
		for(i=0; i<10; i++){
			u4FmOut = U3HReadReg32(0xbfa8010c);
			// check if FM detection done 
			if (u4FmOut != 0)
			{
				// => u4FmOut = USB_FM_OUT
				// read FM_OUT
				printk(KERN_ERR "FM_OUT value = %d(0x%08X)\n", u4FmOut, u4FmOut);
				fgRet = 0;
				//printk(KERN_ERR "FM detection done! loop = %d\n", i);
				break;
			}

			fgRet = 1;
			DRV_MSLEEP(1);
		}
		// => RG_FREQDET_EN = 0
		// disable frequency meter
		U3HWriteReg32(0xbfa80100, (U3HReadReg32(0xbfa80100)&(~RG_FREQDET_EN)|((0x0)<<RG_FREQDET_EN_OFST)));

		// => RG_FRCK_EN = 0
		// disable free run clock
		U3HWriteReg32(0xbfa80110, (U3HReadReg32(0xbfa80110)&(~RG_FRCK_EN)|((0x0)<<RG_FRCK_EN_OFST)));

		// => RG_USB20_HSTX_SRCAL_EN = 0
		// disable HS TX SR calibration
		U3HWriteReg32((void *)(U2_PHYA_CR0[j]), (U3HReadReg32((void *)(U2_PHYA_CR0[j]))&(~RG_USB20_HSTX_SRCAL_EN)|((0x0)<<RG_USB20_HSTX_SRCAL_EN_OFST)));
		DRV_MSLEEP(1);

		if(u4FmOut == 0){
			U3HWriteReg32((void *)(U2_PHYA_CR0[j]), (U3HReadReg32((void *)(U2_PHYA_CR0[j]))&(~RG_USB20_HSTX_SRCTRL)|((0x4)<<RG_USB20_HSTX_SRCTRL_OFST)));
			fgRet = 1;
		}
		else{
			// set reg = (1024/FM_OUT) * REF_CK * U2_SR_COEF (round to the nearest digits)
			u4Tmp = (((1024 * REF_CK * U2_SR_COEF) / u4FmOut) + 500) / 1000; 
			printk(KERN_ERR "SR calibration value = %d\n", (PHY_UINT8)u4Tmp);
			U3HWriteReg32((void *)(U2_PHYA_CR0[j]), (U3HReadReg32((void *)(U2_PHYA_CR0[j]))&(~RG_USB20_HSTX_SRCTRL)|((u4Tmp&0x7)<<RG_USB20_HSTX_SRCTRL_OFST)));
		}
	}
	return fgRet;
}

int ecnt_u3h_phy_init(void)
{
	int ret = 0;
	
	if(isFPGA){
		ret = u3phy_config_fpga();
	}else{
		ret = u3phy_config();
	}
	u2_slew_rate_calibration();
	printk(KERN_ERR "USB driver version: 751221.2.20160516");
	return ret;
}


PHY_INT32 u3phy_init(){
#ifndef CONFIG_PROJECT_PHY
	PHY_INT32 u3phy_version;
#endif
	if(u3phy != NULL){
		return PHY_TRUE;
	}

	u3phy = kmalloc(sizeof(struct u3phy_info), GFP_NOIO);
	u3phy_p1 = kmalloc(sizeof(struct u3phy_info), GFP_NOIO);
#ifdef CONFIG_U3_PHY_GPIO_SUPPORT
	u3phy->phyd_version_addr = 0x2000e4;
	u3phy_p1->phyd_version_addr = 0x2000e4;
#else
	u3phy->phyd_version_addr = U3_PHYD_B2_BASE + 0xe4;
	u3phy_p1->phyd_version_addr = U3_PHYD_B2_BASE_P1 + 0xe4;
#endif
#ifdef CONFIG_PROJECT_PHY
	u3phy->u2phy_regs = (struct u2phy_reg *)U2_PHY_BASE;
	u3phy->u3phyd_regs = (struct u3phyd_reg *)U3_PHYD_BASE;
	u3phy->u3phyd_bank2_regs = (struct u3phyd_bank2_reg *)U3_PHYD_B2_BASE;
	u3phy->u3phya_regs = (struct u3phya_reg *)U3_PHYA_BASE;
	u3phy->u3phya_da_regs = (struct u3phya_da_reg *)U3_PHYA_DA_BASE;
	u3phy->sifslv_chip_regs = (struct sifslv_chip_reg *)SIFSLV_CHIP_BASE;		
	u3phy->sifslv_fm_regs = (struct sifslv_fm_feg *)SIFSLV_FM_FEG_BASE;	
	u3phy_ops = (struct u3phy_operator *)&project_operators;
	u3phy_p1->u2phy_regs = (struct u2phy_reg *)U2_PHY_BASE_P1;
	u3phy_p1->u3phyd_regs = (struct u3phyd_reg *)U3_PHYD_BASE_P1;
	u3phy_p1->u3phyd_bank2_regs = (struct u3phyd_bank2_reg *)U3_PHYD_B2_BASE_P1;
	u3phy_p1->u3phya_regs = (struct u3phya_reg *)U3_PHYA_BASE_P1;
	u3phy_p1->u3phya_da_regs = (struct u3phya_da_reg *)U3_PHYA_DA_BASE_P1;
	u3phy_p1->sifslv_chip_regs = (struct sifslv_chip_reg *)SIFSLV_CHIP_BASE;
	u3phy_p1->sifslv_fm_regs = (struct sifslv_fm_feg *)SIFSLV_FM_FEG_BASE;
#else
	
	/* parse phy version */
	u3phy_version = U3PhyReadReg32(u3phy->phyd_version_addr);
	printk(KERN_ERR "phy version: %x = %x\n", u3phy->phyd_version_addr, u3phy_version);
	u3phy->phy_version = u3phy_version;
	
	if(u3phy_version == 0xa60810a){
#ifdef CONFIG_A60810_SUPPORT
#ifdef CONFIG_U3_PHY_GPIO_SUPPORT
		u3phy->u2phy_regs_a60810 = (struct u2phy_reg_a60810 *)0x0;
		u3phy->u3phyd_regs_a60810 = (struct u3phyd_reg_a60810 *)0x100000;
		u3phy->u3phyd_bank2_regs_a60810 = (struct u3phyd_bank2_reg_a60810 *)0x200000;
		u3phy->u3phya_regs_a60810 = (struct u3phya_reg_a60810 *)0x300000;
		u3phy->u3phya_da_regs_a60810 = (struct u3phya_da_reg_a60810 *)0x400000;
		u3phy->sifslv_chip_regs_a60810 = (struct sifslv_chip_reg_a60810 *)0x500000;
		u3phy->spllc_regs_a60810 = (struct spllc_reg_a60810 *)0x600000;
		u3phy->sifslv_fm_regs_a60810 = (struct sifslv_fm_feg_a60810 *)0xf00000;		
#else
		u3phy->u2phy_regs_a60810 = (struct u2phy_reg_a60810 *)U2_PHY_BASE;
		u3phy->u3phyd_regs_a60810 = (struct u3phyd_reg_a60810 *)U3_PHYD_BASE;
		u3phy->u3phyd_bank2_regs_a60810 = (struct u3phyd_bank2_reg_a60810 *)U3_PHYD_B2_BASE;
		u3phy->u3phya_regs_a60810 = (struct u3phya_reg_a60810 *)U3_PHYA_BASE;
		u3phy->u3phya_da_regs_a60810 = (struct u3phya_da_reg_a60810 *)U3_PHYA_DA_BASE;
		u3phy->sifslv_chip_regs_a60810 = (struct sifslv_chip_reg_a60810 *)SIFSLV_CHIP_BASE;		
		u3phy->sifslv_fm_regs_a60810 = (struct sifslv_fm_feg_a60810 *)SIFSLV_FM_FEG_BASE;	
#endif
		u3phy_ops = (struct u3phy_operator *)&a60810_operators;
#endif
	}
	else{
		printk(KERN_ERR "No match phy version\n");
		return PHY_FALSE;
	}
	
#endif
	return PHY_TRUE;
}

PHY_INT32 U3PhyWriteField8(PHY_INT32 addr, PHY_INT32 offset, PHY_INT32 mask, PHY_INT32 value){
	PHY_INT8 cur_value;
	PHY_INT8 new_value;

	cur_value = U3PhyReadReg8(addr);
	new_value = (cur_value & (~mask))| ((value << offset) & mask);

	U3PhyWriteReg8(addr, new_value);
	return PHY_TRUE;
}

PHY_INT32 U3PhyWriteField32(PHY_INT32 addr, PHY_INT32 offset, PHY_INT32 mask, PHY_INT32 value){
	PHY_INT32 cur_value;
	PHY_INT32 new_value;

	cur_value = U3PhyReadReg32(addr);
	new_value = (cur_value & (~mask)) | ((value << offset) & mask);
	U3PhyWriteReg32(addr, new_value);

	return PHY_TRUE;
}

PHY_INT32 U3PhyReadField8(PHY_INT32 addr,PHY_INT32 offset,PHY_INT32 mask){
	
	return ((U3PhyReadReg8(addr) & mask) >> offset);
}

PHY_INT32 U3PhyReadField32(PHY_INT32 addr, PHY_INT32 offset, PHY_INT32 mask){

	return ((U3PhyReadReg32(addr) & mask) >> offset);
}

