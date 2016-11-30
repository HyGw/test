#include <linux/gfp.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#define U3_PHY_LIB
#include "mtk-phy.h"

#ifdef CONFIG_A60810_SUPPORT
#include "mtk-phy-a60810.h"
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
	.u2_connect = u2_connect,
	.u2_disconnect = u2_disconnect,	
	.u2_slew_rate_calibration = u2_slew_rate_calibration,
};
#endif
PHY_INT32 clear_reset()
{
	PHY_INT32 value;
	value = readl(0xbfb00834);
	
	value |= (0x1<<22);
	writel(value, 0xbfb00834);

	value = readl(0xbfb00834);
	
	value = 0;
	writel(value, 0xbfb00834);
	
	value = readl(0xbfa80700);
	
	value = 0x10c00;
	writel(value, 0xbfa80700);

	value = readl(0xbfa80704);
	
	value = 0;
	writel(value, 0xbfa80704);
	
	value = readl(0xbfa80730);
	
	value = 0xc;
	writel(value, 0xbfa80730);
}

extern int (*I2CWriterPtr)(u8 DevAddr, u8 WordAddr, u8* data_value, u8 data_len);
extern int (*I2CReaderPtr)(u8 DevAddr, u8 WordAddr, u8* data_value, u8 data_len);
#define USB_PHY_DEV_ADDR	        0x60

PHY_INT32 u3phy_config_FPGA(){
	u8 u1Value[4] = {0, 0, 0,0};
	
	if(!I2CWriterPtr || !I2CReaderPtr)
		return;
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
/***********************************************************/
	u1Value[0] = 0x46; 
	u1Value[1] = 0x0; 
	I2CWriterPtr(USB_PHY_DEV_ADDR, 0x38, u1Value, 2);

	u1Value[0] = 0x40; 
	u1Value[1] = 0x0; 
	I2CWriterPtr(USB_PHY_DEV_ADDR, 0x42, u1Value, 2);

	u1Value[0] = 0xab; 
	u1Value[1] = 0x0c; 
	I2CWriterPtr(USB_PHY_DEV_ADDR, 0x08, u1Value, 2);
/***********************************************************/
	u1Value[0] = 0x71; 
	u1Value[1] = 0xe7; 
	u1Value[2] = 0x4f; 
	I2CWriterPtr(USB_PHY_DEV_ADDR, 0x0c, u1Value, 3);
/***********************************************************/
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
/***********************************************************/
	u1Value[0] = 0x50; 
	I2CWriterPtr(USB_PHY_DEV_ADDR, 0xff, u1Value, 1);

	u1Value[0] = 0x10; 
	u1Value[1] = 0x54; 
	I2CWriterPtr(USB_PHY_DEV_ADDR, 0x02, u1Value, 2);
/***********************************************************/
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
	
}

#define BGA_TYPE 		1
PHY_INT32 u3phy_config(){
#if BGA_TYPE
	writel(0xC0240008, 0xBFA8081C);/* prot0 */
	writel(0xC0240000, 0xBFA8101C);/* port1 */
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
#else
	writel(0xC0241580, 0xBFA8081C);
	writel(0xC0240000, 0xBFA8101C);
#endif
}

PHY_INT32 u3phy_init_FPGA(){
	PHY_INT32 value;
	value = readl(0xbfb40004);
	
	value |= (0x1<<18);
	writel(value, 0xbfb40004);
	
	value = readl(0xbfa80700);
	
	value &= ~(0x1<<0);
	writel(value, 0xbfa80700);
	
	value = readl(0xbfa80704);
	value &= ~(0x1<<0);
	writel(value, 0xbfa80704);
	
	value = readl(0xbfa80750);
	value &= ~(0x3<<0);
	writel(value, 0xbfa80750);

	value = readl(0xbfb90430);
	value |= (0x1<<9);
	writel(value, 0xbfb90430);
		
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
	u3phy_ops = &project_operators;

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
	printk(KERN_ERR "phy version: %x\n", u3phy_version);
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
	new_value = (cur_value & (~mask)) | (value << offset);
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

