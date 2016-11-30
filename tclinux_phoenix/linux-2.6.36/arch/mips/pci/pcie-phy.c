#include <asm/tc3162/tc3162.h>
#include <linux/delay.h>

//#define PCIE_PHY_DEBUG 1

#define ENABLE 1
#define DISABLE 0

#if defined(TCSUPPORT_BONDING)
extern unsigned long slaveVirBaseAddr;
#endif


#if 0 //for FPGA auto scan timing
#define PCIe_AUTO_SCAN_TIMING 1
#define PCIE_7510_FPGA_TIMING_PHY_P0 12
#define PCIE_7510_FPGA_TIMING_PHY_P1 13
#define PHY_PORT0 0
#define PHY_PORT1 1

static int  pcie_reset_link(check_addr)
{
	int i;
	uint32 tmp;
	//PCI-E reset
	/* enabled PCIe port 1 */
	VPint(0xbfb00088) |= (1<<22);
	mdelay(1);
	/*first reset to default*/
	VPint(0xbfb00834) &= ~((1<<26) | (1<<27) | (1<<29));
	mdelay(1);
	VPint(0xbfb00834) |= ((1<<26) | (1<<27) | (1<<29));
	mdelay(1);
	VPint(0xbfb00834) &= ~((1<<26) | (1<<27) | (1<<29));
	mdelay(1);

	//relese pcie device reset
	VPint(0xbfb80000) &= ~((1<<1) | (1<<2));
	mdelay(1);

	//start to check pcie if link up
	//wait device link up
	for(i=1 ; i<601 ; i++){
		mdelay(1);
		if((VPint(check_addr) & 0x1) != 0){
			break;
		}   
		if((i%100)==0){//reset device PCIe MAC if not link up every 100ms biker_20120816
			//pcie reset again
			VPint(0xbfb80000) |= ((1<<1) | (1<<2));
			mdelay(1);

			VPint(0xbfb80000) &= ~((1<<1) | (1<<2));
			mdelay(1);
		}
	}
	if(i == 601)
		return -1;
	else
		return 0;

}
#endif

void pcie_phy_force_mode_en(char enable, char rc_num){

	if(isFPGA)
		return;

	//printk("pcie_phy_force_mode_en %x %x \n",enable,rc_num);
	if(enable == ENABLE) //means disable
	{
		switch (rc_num){
			case 0:
				regWrite32(0xbfaf202c, 0x1010);
				regWrite32(0xbfaf202c, 0x1090);
				regWrite32(0xbfaf202c, 0x1010);
				break;
			case 1:
				regWrite32(0xbfaf212c, 0x1010);
				regWrite32(0xbfaf212c, 0x1090);
				regWrite32(0xbfaf212c, 0x1010);
				break;
#if defined(TCSUPPORT_BONDING)

			case 2: //external chip use for bonding
				if(slaveVirBaseAddr != 0){
					regWrite32(slaveVirBaseAddr + 0xaf202c, 0x1010); 
					regWrite32(slaveVirBaseAddr + 0xaf202c, 0x1090);
					regWrite32(slaveVirBaseAddr + 0xaf202c, 0x1010);
					
					regWrite32(slaveVirBaseAddr + 0xaf212c, 0x1010); 
					regWrite32(slaveVirBaseAddr + 0xaf212c, 0x1090);
					regWrite32(slaveVirBaseAddr + 0xaf212c, 0x1010);
				}	
				break;
#endif
			default:
				printk("RC number %d Error !! \n",rc_num);
				break;
		}
		
	}
	else
	{
		switch (rc_num){
			case 0:
				regWrite32(0xbfaf202c, 0x0);
				regWrite32(0xbfaf202c, 0x80);
				regWrite32(0xbfaf202c, 0x0);
				break;
			case 1:
				regWrite32(0xbfaf212c, 0x0);
				regWrite32(0xbfaf212c, 0x80);
				regWrite32(0xbfaf212c, 0x0);
				break;
#if defined(TCSUPPORT_BONDING)

			case 2: //external
				if(slaveVirBaseAddr != 0){
					regWrite32(slaveVirBaseAddr + 0xaf202c, 0x0); 
					regWrite32(slaveVirBaseAddr + 0xaf202c, 0x80);
					regWrite32(slaveVirBaseAddr + 0xaf202c, 0x0);
					
					regWrite32(slaveVirBaseAddr + 0xaf212c, 0x0); 
					regWrite32(slaveVirBaseAddr + 0xaf212c, 0x80);
					regWrite32(slaveVirBaseAddr + 0xaf212c, 0x0);
				}
				break;
#endif
			default:
				printk("RC number %d Error !! \n",rc_num);
				break;
		}
	}
}
 void init_60901(void)
{
	uint32 tmp;
	#ifdef PCIE_PHY_DEBUG
	printk("biker1_pcie_phy_init debug message: master init \n");
	printk("debug: read address 0xbfaf24a0 (380389); value is: %lx\n",VPint(0xbfaf24a0));
	printk("debug: read address 0xbfaf24a8 (E80E8); value is: %lx\n",VPint(0xbfaf24a8));
	#endif
	//init Master
	//init 20MHz or 25MHz
	tmp = (regRead32(0xbfb0008c)&(0x3 <<22))>>22;
	if (tmp == 0x1 ) 
	{
		//xtal is 20MHz 
		#ifdef PCIE_PHY_DEBUG
		printk("xtal is 20MHz \n");
		#endif

		//I2C      70    0xA8[11:00]   0x74      RW      RG_PE1_H_LCDDS_SSC_DELTA        //Improve SSC deviation	
		//tmp = regRead32(0xbfaf24a8);
		regWrite32(0xbfaf24a8, 0x740074);
		//VPint(0xbfaf24a8) = (VPint(0xbfaf24a8) & (~0xfff )) | (0x74);
		mdelay(1);	
		//I2C      70    0xA8[23:16]   0x74      RW      RG_PE1_H_LCDDS_SSC_DELTA1       //Improve SSC deviation	
		//VPint(0xbfaf24a8) =  (VPint(0xbfaf24a8) & (~(0xfff <<16))) | (0x74<<16);
		//I2C      70    0xA0[19:19]   0x00      RW      RG_PE1_H_LCDDS_SSC_EN           //Disable SSC	
		tmp = regRead32(0xbfaf24a0);
		regWrite32(0xbfaf24a0, tmp & (~(0x1 <<19)) );
		//VPint(0xbfaf24a0) =  VPint(0xbfaf24a0) & (~(0x1 <<19)) ;
		mdelay(1);	
		//I2C      70    0xA0[19:19]   0x01      RW      RG_PE1_H_LCDDS_SSC_EN           //Enable SSC	
		tmp = regRead32(0xbfaf24a0);
		regWrite32(0xbfaf24a0, tmp | (0x1<<19) );
		//VPint(0xbfaf24a0) =  VPint(0xbfaf24a0)  | (0x1<<19);
		mdelay(1);
	}else if(tmp == 0x0 )
	{   
		//xtal is 25MHz 
		#ifdef PCIE_PHY_DEBUG
		printk("xtal is 25MHz \n");
		#endif
//	I2C      70    0xFC[31:24]   0x00      RW                                      //Change bank address to 0x00
//	I2C      70    0x00[04:04]   0x01      RW      rg_pe1_frc_phy_en               //Force Port 0 disable control
		regWrite32(0xbfaf2000,  (regRead32(0xbfaf2000) &(~(0x1 <<4)) ) | (0x1<<4) );
		mdelay(1);
//	I2C      70    0xFC[31:24]   0x01      RW                                      //Change bank address to 0x01
//	I2C      70    0x00[04:04]   0x01      RW      rg_pe1_frc_phy_en               //Force Port 1 disable control
		regWrite32(0xbfaf2100,  (regRead32(0xbfaf2100) &(~(0x1 <<4)) ) | (0x1<<4) );
		mdelay(1);

//	I2C      70    0xFC[31:24]   0x00      RW                                      //Change bank address to 0x00
//	I2C      70    0x00[05:05]   0x00      RW      rg_pe1_phy_en                   //Port 0 disable
		regWrite32(0xbfaf2000,  (regRead32(0xbfaf2000) &(~(0x1 <<5)) )  );
		mdelay(1);

//	I2C      70    0xFC[31:24]   0x01      RW                                      //Change bank address to 0x01
//	I2C      70    0x00[05:05]   0x00      RW      rg_pe1_phy_en                   //Port 1 disable
		regWrite32(0xbfaf2100,  (regRead32(0xbfaf2100) &(~(0x1 <<5)) ) );
		mdelay(1);

//	I2C      70    0xFC[31:24]   0x04      RW                                      //Change bank address to 0x04
//	I2C      70    0x00[08:08]   0x01      RW      rg_pe1_frc_h_xtal_type          //
		regWrite32(0xbfaf2400,  (regRead32(0xbfaf2400) &(~(0x1 <<8)) ) | (0x1<<8) );
		mdelay(1);

//	I2C      70    0x00[10:09]   0x00      RW      rg_pe1_h_xtal_type              //
		regWrite32(0xbfaf2400,  (regRead32(0xbfaf2400) &(~(0x3 <<9)) )  );
		mdelay(1);

//	I2C      70    0x90[15:12]   0x0A      RW      RG_PE1_H_PLL_IR                 //
		regWrite32(0xbfaf2490,  (regRead32(0xbfaf2490) &(~(0xf <<12)) ) | (0xA<<12) );
		mdelay(1);

//	I2C      70    0xAC[18:16]   0x01      RW      RG_PE1_H_PLL_BR                 //
		regWrite32(0xbfaf24AC,  (regRead32(0xbfaf24AC) &(~(0x7 <<16)) ) | (0x1<<16) );
		mdelay(1);
		
//	I2C      70    0x9C[30:16]   0x1900    RW      RG_PE1_H_LCDDS_PCW_NCPO         //
		regWrite32(0xbfaf249C,  (regRead32(0xbfaf249C) &(~(0x7fff <<16)) ) | (0x1900<<16) );
		mdelay(1);
		
//	I2C      70    0x9C[31:31]   0x00      RW      RG_PE1_H_LCDDS_PCW_NCPO_CHG     //
		regWrite32(0xbfaf249C,  (regRead32(0xbfaf249C) &(~(0x1 <<31)) ) );
		mdelay(1);

//	I2C      70    0x9C[31:31]   0x01      RW      RG_PE1_H_LCDDS_PCW_NCPO_CHG     //
		regWrite32(0xbfaf249C,  regRead32(0xbfaf249C)  | (0x1<<31) );
		mdelay(1);

//	I2C      70    0xA4[15:00]   0x018D    RW      RG_PE1_H_LCDDS_SSC_PRD          //
		regWrite32(0xbfaf24A4,  (regRead32(0xbfaf24A4) &(~(0xffff )) ) | (0x18D) );
		mdelay(1);

//	I2C      70    0xA8[11:00]   0x004A    RW      RG_PE1_H_LCDDS_SSC_DELTA        //
		regWrite32(0xbfaf24A8,  (regRead32(0xbfaf24A8) &(~(0xfff )) ) | (0x4A) );
		mdelay(1);

//	I2C      70    0xA8[27:16]   0x004A    RW      RG_PE1_H_LCDDS_SSC_DELTA1       //
		regWrite32(0xbfaf24A8,  (regRead32(0xbfaf24A8) &(~(0xfff <<16)) ) | (0x4A<<16) );
		mdelay(1);

//	I2C      70    0x00[12:12]   0x01      RW      rg_pe1_mstck_25m                //value of mstck_25m when force mode enable
		regWrite32(0xbfaf2400,  (regRead32(0xbfaf2400)  ) | (0x1<<12) );
		mdelay(1);

//	I2C      70    0x00[11:11]   0x01      RW      rg_pe1_frc_mstck_25m            //force mode enable of mstck_25m
		regWrite32(0xbfaf2400,  (regRead32(0xbfaf2400)  ) | (0x1<<11) );
		mdelay(1);

//	I2C      70    0xFC[31:24]   0x00      RW                                      //Change bank address to 0x00
//	I2C      70    0x00[05:05]   0x01      RW      rg_pe1_phy_en                   //Port 0 enable
		regWrite32(0xbfaf2000,  (regRead32(0xbfaf2000) |(0x1 <<5) )  );
		mdelay(1);
//	I2C      70    0xFC[31:24]   0x01      RW                                      //Change bank address to 0x01
//	I2C      70    0x00[05:05]   0x01      RW      rg_pe1_phy_en                   //Port 1 enable
		regWrite32(0xbfaf2100,  (regRead32(0xbfaf2100) |(0x1 <<5) ));
		mdelay(1);

//	I2C      70    0xFC[31:24]   0x00      RW                                      //Change bank address to 0x00
//	I2C      70    0x00[04:04]   0x00      RW      rg_pe1_frc_phy_en               //Force Port 0 disable control
		//regWrite32(0xbfaf2000,  (regRead32(0xbfaf2000) &(~(0x1 <<4)) ));
		//mdelay(1);

//	I2C      70    0xFC[31:24]   0x01      RW                                      //Change bank address to 0x01
//	I2C      70    0x00[04:04]   0x00      RW      rg_pe1_frc_phy_en               //Force Port 1 disable control
		//regWrite32(0xbfaf2100,  (regRead32(0xbfaf2100) &(~(0x1 <<4)) ));
		//mdelay(1);

//	I2C      70    0xFC[31:24]   0x00      RW                                      //Change bank address to 0x00

	}
	#ifdef PCIE_PHY_DEBUG
	printk("pcie_phy_init debug message:\n");
	printk("debug: read address 0xbfaf24a0 (380389); value is: %lx\n",VPint(0xbfaf24a0));
	printk("debug: read address 0xbfaf24a8 (740074); value is: %lx\n",VPint(0xbfaf24a8));
	#endif
	
	//init Slave
	
}

#if 0 //for FPGA auto scan timing
uint8 pcie_60901_auto_scan(uint8 PHY_PORT, uint8 win_range, uint8 win_loop, uint32 check_addr)
{
	uint8 win_init = 0;
	uint8 win_start = 0;	
	uint8 win_len = 0; 	
	uint8 max_win_start = 0;	
	uint8 max_win_len = 0; 	
	uint8 i = 0;
	uint8 timing = 0;

	printk(">>>>>Start to scan PCIe timing setting<<<<<\n");
		for (i=win_init; i<win_range; i++){
			if (PHY_PORT == PHY_PORT0)
				init_60901((uint32) (i%win_loop),0);
			else
				init_60901(0,(uint32) (i%win_loop));
			//printk("timing setting: %2d ",i);
			if (pcie_reset_link(check_addr) == 0){
				win_len++;				
				if(win_len > max_win_len){					
					max_win_len = win_len;					
					max_win_start = win_start;				
				}				
				printk("Link up! \n");		
			}else{				
			win_len = 0;				
			win_start = i + 1;				
			printk("Not link \n");			
			}		
		}		
		timing = (uint8) ((max_win_start%win_loop) + ((max_win_len%win_loop)>>1));
		printk("The pcie timing window start at : %d \n",max_win_start);		
		printk("Window length is : %d \n",max_win_len);		
		printk("The chosen PCIe: %d \n ",timing);
		return timing;
}
#endif

 void init_7505(void)
{
	//For WCN ch14 de-sence issue, default Disable SSC, bit 28 => 1
	printk("Disable PCIe SSC\n");
	regWrite32(0xbfaf2414,  (regRead32(0xbfaf2414) |(0x1 <<28) ));
	
	//change MSTSET from 1 to 5;
	mdelay(1);
	regWrite32(0xbfaf2498,  ((regRead32(0xbfaf2498) &(~0xff) )|(0x5) ));

	//decrease PLL BW to improve jitter; by biker_20140721; add 25MHz_20140815
	mdelay(1);
	regWrite32(0xbfaf2400,  ((regRead32(0xbfaf2400) &(~(0x1<<8)) )|(0x1<<8) ));
	mdelay(1);
	regWrite32(0xbfaf2400,  ((regRead32(0xbfaf2400) &(~(0x3<<9)) )|(0x0<<9) ));
	mdelay(1);
	regWrite32(0xbfaf2000,  ((regRead32(0xbfaf2000) &(~(0x1<<4)) )|(0x1<<4) ));
	mdelay(1);
	regWrite32(0xbfaf2000,  ((regRead32(0xbfaf2000) &(~(0x1<<5)) )|(0x0<<5) ));
	mdelay(1);
	regWrite32(0xbfaf24ac,  ((regRead32(0xbfaf24ac) &(~(0x7<<16)) )|(0x3<<16) ));
	mdelay(1);
	regWrite32(0xbfaf24bc,  ((regRead32(0xbfaf24bc) &(~(0xff<<24)) )|(0x64<<24) ));
	mdelay(1);
	regWrite32(0xbfaf2490,  ((regRead32(0xbfaf2490) &(~(0xf<<12)) )|(0xa<<12) ));
	mdelay(1);
	regWrite32(0xbfaf2490,  ((regRead32(0xbfaf2490) &(~(0x3<<6)) )|(0x0<<6) ));
	mdelay(1);
	regWrite32(0xbfaf24c0,  ((regRead32(0xbfaf24c0) &(~(0xffffffff<<0)) )|(0x19000000<<0) ));
	mdelay(1);
	regWrite32(0xbfaf24a4,  ((regRead32(0xbfaf24a4) &(~(0xffff<<0)) )|(0x18d<<0) ));
	mdelay(1);
	regWrite32(0xbfaf24a8,  ((regRead32(0xbfaf24a8) &(~(0xfff<<0)) )|(0x4a<<0) ));
	mdelay(1);
	regWrite32(0xbfaf24a8,  ((regRead32(0xbfaf24a8) &(~(0xfff<<16)) )|(0x4a<<16) ));
	mdelay(1);
	regWrite32(0xbfaf2000,  ((regRead32(0xbfaf2000) &(~(0x1<<5)) )|(0x1<<5) ));
	mdelay(1);
	regWrite32(0xbfaf2000,  ((regRead32(0xbfaf2000) &(~(0x1<<4)) )|(0x0<<4) ));
	mdelay(1);


	/*MT7505 input crystall is 40Mhz*/
	//Fix frequence setting bug when hardware trap is 40MHz XTAL ; 
#if 0	//Already done in bootloader
	if(VPint(CR_AHB_HWCONF)&(1<<1)){
	VPint(0xbfaf2400) = (VPint(0xbfaf2400) | 0x100); /*force mode enable xtal type*/
	}
#endif
		
}
void init_751221(void)
{
	printk("PCIe driver version: 751221.3.20160516\n");//Biker_20160516

	//LCDDS_CLK_PH_INV
	regWrite32(0xbfaf24a0,  ((regRead32(0xbfaf24a0) &(~(0x1<<5)) )|(0x1<<5) ));
	mdelay(1);

	//Patch TxDetRx Timing for E1, from DR 20160421, Biker_20160516
	regWrite32(0xbfac0a28,  ((regRead32(0xbfac0a28) &(~(0x1ff<<9)) )|(0x10<<9) ));//rg_ssusb_rxdet_stb2_set[8:0]
	mdelay(1);
	regWrite32(0xbfac0a2c,  ((regRead32(0xbfac0a2c) &(~0x1ff) )|0x10 ));//rg_ssusb_rxdet_stb2_set_p3[8:0]
	mdelay(1);
	
	if(isEN7512 || isEN7526F) //Disable gen2 port, Biker_20160516
	{
		//7512 is QFP IC, use gen1 port only, disable gen2 port for COC test
		regWrite32(0xbfac030c,  (regRead32(0xbfac030c) |(0x1 <<31) )); //PHY IP_SW_RESET	
		mdelay(1);
	}
	

	if(isEN7521S || isEN7521F) // Disable gen1 & gen2 port, Biker_20160516
	{
		//disable gen2 port
		regWrite32(0xbfac030c,  (regRead32(0xbfac030c) |(0x1 <<31) )); //PHY IP_SW_RESET
		mdelay(1);
		//disable gen1 port
		regWrite32(0xbfaf2000,  ((regRead32(0xbfaf2000)  )|(0x1<<4) )); //rg_pe1_frc_phy_en
		mdelay(1);		
	}
}

int pciePhyInit(void){

#if 0	//for FPGA auto scan timing	
#ifdef PCIe_AUTO_SCAN_TIMING
		timing_p0 = pcie_60901_auto_scan(PHY_PORT0,36,18,0xbfb83050);
		timing_p1 = pcie_60901_auto_scan(PHY_PORT1,36,18,0xbfb82050);
#endif
#endif

	if(isEN751221) {
		
		if (!isFPGA) {
			init_751221();
		}
	}

	if(isMT7505) {
		
		if (!isFPGA) {
			init_7505();
		}
	}
	
	if(isMT751020){
#if 0	//for FPGA auto scan timing	
#ifdef PCIe_AUTO_SCAN_TIMING
		timing_p0 = pcie_60901_auto_scan(PHY_PORT0,36,18,0xbfb83050);
		timing_p1 = pcie_60901_auto_scan(PHY_PORT1,36,18,0xbfb82050);
#endif
#endif
		init_60901();

	}

	return 0;
}

