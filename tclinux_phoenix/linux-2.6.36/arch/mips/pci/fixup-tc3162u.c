#include <linux/init.h>
#include <linux/types.h>
#include <linux/pci.h>
#include <asm/tc3162/tc3162.h>
#include <linux/delay.h>
#if defined(TCSUPPORT_CPU_MT7505) || defined(TCSUPPORT_CPU_EN7512) || defined(TCSUPPORT_CPU_EN7521)
#define isRC0_LINKUP		((regRead32(0xbfb80050) & 0x2) ? 1 : 0)  
#define isRC1_LINKUP		((regRead32(0xbfb80050) & 0x4) ? 1 : 0)
#else 
#define isRC0_LINKUP		((regRead32(0xbfb82050) & 0x1) ? 1 : 0)
#define isRC1_LINKUP		((regRead32(0xbfb83050) & 0x1) ? 1 : 0)
#endif


unsigned long int pcie_read_config_word(unsigned char type, unsigned char bus, unsigned char devnum, unsigned int regnum);
int pcie_write_config_word(unsigned char type, unsigned char bus, unsigned char devnum, unsigned int regnum, unsigned long int value);

extern int mt7512_pcie_is_slave();
extern int dual_band_support;

#ifndef PCIE_PCI_COEXIT
/* 
static char irq_tab_tc3162_pcie[] __initdata = {
    //[1] = PCI_A_INT,
    //[2] = PCI_B_INT,
    [0] = PCIE_A_INT,//krammer
};
*/
int __init pcibios_map_irq(struct pci_dev *dev, u8 slot, u8 pin)
{
	//if (slot <= 0)
#if 0
	if (slot < 0)
		return -1;
#endif
    	//return irq_tab_tc3162[slot];
	return PCIE_A_INT;
}
#endif


static void tc3162_pcie_fixup(struct pci_dev *dev)
{
	/* setup COMMAND register */
	pci_write_config_word(dev, PCI_COMMAND,
		(PCI_COMMAND_IO | PCI_COMMAND_MEMORY | PCI_COMMAND_MASTER));

	/* setup CACHE_LINE_SIZE register */
	pci_write_config_byte(dev, PCI_CACHE_LINE_SIZE, 0x8);

	/* setup LATENCY_TIMER register */
	pci_write_config_byte(dev, PCI_LATENCY_TIMER, 0x20);

	/* setup BARS */
//	pci_write_config_dword(dev, PCI_BASE_ADDRESS_0, 0);
//	pci_write_config_dword(dev, PCI_BASE_ADDRESS_1, 0x1FBA0000);
//	pci_write_config_dword(dev, PCI_BASE_ADDRESS_2, 0x1FBB0000);
}
static void tc3162_pcie_fixup_ra63165(struct pci_dev *dev)
{
	uint32 tmp;
#if defined(TCSUPPORT_BONDING)
	int i;
#endif

	/* setup COMMAND register */
	pci_write_config_word(dev, PCI_COMMAND,
		(PCI_COMMAND_IO | PCI_COMMAND_MEMORY | PCI_COMMAND_MASTER));

#if defined(TCSUPPORT_BONDING)
	//Enable slave RC ECRC count . //bus1, dev1
	regWrite32(0xbfb80020, 0x1080118);
	tmp = regRead32(0xbfb80024);
	regWrite32(0xbfb80024, (tmp | (1<<8)));

	//config PCIe RC/EP VC mapping
	//set bus0, dev0, fun0, reg154 (setup VC0)
	regWrite32(0xbfb80020, 0x154);
	regWrite32(0xbfb80024, 0X80000001);
	//set bus0, dev0, fun0, reg160 (setup VC1)
	regWrite32(0xbfb80020, 0x160);
	regWrite32(0xbfb80024, 0X81000002);
	//set bus1, dev0, fun0, reg154 (setup VC0)
	regWrite32(0xbfb80020, 0x1000154);
	regWrite32(0xbfb80024, 0X80000001);
	//set bus1, dev0, fun0, reg160 (setup VC1)
	regWrite32(0xbfb80020, 0x1000160);
	regWrite32(0xbfb80024, 0X81000002);

	//config slave chip EP MSI
	regWrite32(0xbfb80020, 0x1000050);
	tmp = regRead32(0xbfb80024);
	regWrite32(0xbfb80020, 0x1000050);
	regWrite32(0xbfb80024, (tmp | 0x510000));
	regWrite32(0xbfb80020, 0x1000054);
	regWrite32(0xbfb80024, 0x20af1000);
	regWrite32(0xbfb80020, 0x100005c);
	regWrite32(0xbfb80024, 0x0);

	//setup RC0 MSI address reg
	regWrite32(0xbfb82090, 0x20af1000);

	//setup RC0 Pbus/Rbus VC mapping
	regWrite32(0xbfb82094, 0x1);
	regWrite32(0xbfb83094, 0x0);

	//wait RC0 VC1 set up OK
	for(i=0 ; i<1000 ; i++){
		mdelay(1);
		regWrite32(0xbfb80020, 0x164);
		if((regRead32(0xbfb80024) & (1<<17)) == 0){
			break;
		}
	}
#endif

	//pci-e interrupt enable_dma
	if(isRT63365 || isMT751020 || isMT7505){
#if defined(TCSUPPORT_BONDING)
		if((regRead32(0xbfb82050) & 0x1) != 0){
			/* slave dmt */
			tmp = regRead32(0xbfb8000c);
			regWrite32(0xbfb8000c, (tmp | (1<<23)));
			/* slave gdma */
			tmp = regRead32(0xbfb8000c);
			regWrite32(0xbfb8000c, (tmp | (1<<25)));

			if(regRead32(0xbfb80050) == 1){
				/* wifi 0 (slave)*/
				tmp = regRead32(0xbfb8000c);
				regWrite32(0xbfb8000c, (tmp | (1<<22)));
			}
		}
#else
		//if((regRead32(0xbfb82050) & 0x1) != 0){
		if(isRC0_LINKUP){
			tmp = regRead32(0xbfb8000c);
			regWrite32(0xbfb8000c, (tmp | (1<<20)));
		}
#endif
		//second band
		if(dual_band_support){
			if((regRead32(0xbfb83050) & 0x1) != 0){
				if(isMT751020 || isMT7505){
					tmp = regRead32(0xbfb8000c);
					regWrite32(0xbfb8000c, (tmp | (1<<26)));
				}else{
					tmp = regRead32(0xbfb8000c);
					regWrite32(0xbfb8000c, (tmp | (1<<21)));
				}
			}
		}
	}else{
		tmp = regRead32(0xbfb8100c);
		regWrite32(0xbfb8100c, (tmp | (1<<20)));
	}

	if(isMT751020 || isMT7505){
		pcie_timeout_disable();
		aer_config(1);
	}
}

#ifndef PCIE_PCI_COEXIT
int pcibios_plat_dev_init(struct pci_dev *dev)
{
	return 0;
}
#endif


int mt7512_pcie_get_pos(char bus,char dev)
{
	unsigned int val,pos;

	val = pcie_read_config_word(0,bus,dev,0x34);
	pos = val&0xff;
	while(pos && pos != 0xff)
	{
		val = pcie_read_config_word(0,bus,dev,pos);
		if ( (val&0xff) == 0x10)
			return pos;
		pos = (val >> 0x08) & 0xff;
	}
	return 0;
}

int  mt7512_pcie_rc1_retrain(void)
{
	unsigned int pos = 0, ppos = 0,bus;
	unsigned int  linkcap, plinkcap,plinksta;

	ppos = mt7512_pcie_get_pos(0,1);
	if (mt7512_pcie_is_slave()==1)
		bus = 1;
	else
		bus =  2;
	
	pos = mt7512_pcie_get_pos(bus,0);
	
	if (pos <0x40 || ppos < 0x40)
		return 0;
	
	plinkcap =  pcie_read_config_word(0,0,1,ppos+0x0c);
	linkcap = pcie_read_config_word(0,bus,0,pos+0x0c);
	
	printk("\n mt7512_pcie_rc1_retrain: %x = %08x %x = %08x",pos,linkcap,ppos,plinkcap);
	
	if ((linkcap&0x0f)== 1 || (plinkcap&0x0f)==1)
		return 0;
	
	plinksta = pcie_read_config_word(0,0,1,ppos+0x10);
	if( ((plinksta>>16)&0x0f) ==  (plinkcap&0x0f))
		return 0;

	plinksta =  pcie_read_config_word(0,0,1,ppos+0x10);
	plinksta |= 0x20;
	pcie_write_config_word(0,0,1,ppos+0x10,plinksta);
	
	mdelay(1000); 
	
	plinksta =  pcie_read_config_word(0,0,1,ppos+0x10);
	
	printk("\nRC1 Link Traing Result: %08x",plinksta);
	
	return 1;
}


void mt7512_pcie_fixup(void)
{
	unsigned int val = 0,tmp = 0 ,i = 0;
	
	if (isRC0_LINKUP)
	{
	
		val =  pcie_read_config_word(0,0,0,0x20);
		tmp = ((val&0xffff)<<16);
		val = (val&0xffff0000) + 0x100000;
		val = val - tmp;
		i = 0;
		while(i < 32)
		{
			if((1<<i) >= val)
				break;
			i++;
		}
		regWrite32(0xbfb81438,tmp | i);   //config RC0 to EP Addr window
		mdelay(1);
		regWrite32(0xbfb81448,0x80);     //enable EP to RC0 access
		printk("\n mt7512_pcie_fixup: 0x1438 = %x ",tmp | i);
	}
	
	if (isRC1_LINKUP)
	{
	
		val =  pcie_read_config_word(0,0,1,0x20);
		tmp = ((val&0xffff)<<16);
		val = (val&0xffff0000) + 0x100000;
		val = val - tmp;
		i = 0;
		while(i < 32)
		{
			if((1<<i) >= val)
				break;
			i++;
		}
		regWrite32(0xbfb83438,tmp | i);   //config RC1 to EP Addr window
		mdelay(1);
		regWrite32(0xbfb83448,0x80);     //enable EP to RC1 access
		printk("\n mt7512_pcie_fixup: 0x3438 = %x ",tmp | i);
		
		mt7512_pcie_rc1_retrain();
		
	}

	return ;
}

EXPORT_SYMBOL(mt7512_pcie_fixup);



void mt7512_pcie_fixup_rc0(struct pci_dev *dev)
{

	unsigned int val = 0,tmp = 0 ,i = 0;

	if (isRC0_LINKUP)
	{
		val =  pcie_read_config_word(0,0,0,0x20);
		tmp = ((val&0xffff)<<16);
		val = (val&0xffff0000) + 0x100000;
		val = val - tmp;
		i = 0;
		
		while(i < 32)
		{
			if((1<<i) >= val)
				break;
			i++;
		}
		
		/* config RC1 to EP Addr window */	
		regWrite32(0xbfb81438,tmp | i);  
		mdelay(1);

	 	/* enable EP to RC1 access */	
		regWrite32(0xbfb81448,0x80);	
		printk("\n mt7512_pcie_fixup: 0x1438 = %x ",tmp | i);
	}
	return;
}


void mt7512_pcie_fixup_rc1(struct pci_dev *dev)
{

	unsigned int val = 0,tmp = 0 ,i = 0;


	if (isRC1_LINKUP)
	{
		val =  pcie_read_config_word(0,0,1,0x20);
		tmp = ((val&0xffff)<<16);
		val = (val&0xffff0000) + 0x100000;
		val = val - tmp;
		i = 0;
		while(i < 32)
		{
			if((1<<i) >= val)
				break;
			i++;
		}
		
		/* config RC1 to EP Addr window */
		regWrite32(0xbfb83438,tmp | i);   
		mdelay(1);

	 	/* enable EP to RC1 access */
		regWrite32(0xbfb83448,0x80);	
		printk("\n mt7512_pcie_fixup: 0x3438 = %x ",tmp | i);
		
		mt7512_pcie_rc1_retrain();
	}
	return;
	
}



DECLARE_PCI_FIXUP_FINAL(PCI_VENDOR_ID_SIS, PCI_DEVICE_ID_SIS,
          tc3162_pcie_fixup);
DECLARE_PCI_FIXUP_FINAL(PCI_VENDOR_ID_RT, PCI_DEVICE_ID_RT,
          tc3162_pcie_fixup_ra63165);
DECLARE_PCI_FIXUP_FINAL(PCI_VENDOR_ID_MTK, PCI_DEVICE_ID_MTK,
		tc3162_pcie_fixup_ra63165);

DECLARE_PCI_FIXUP_FINAL(0x14c3,0x0810,mt7512_pcie_fixup_rc0);
DECLARE_PCI_FIXUP_FINAL(0x14c3,0x0811,mt7512_pcie_fixup_rc1);

