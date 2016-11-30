#ifdef CONFIG_U3_PHY_GPIO_SUPPORT
#include <linux/gfp.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/dmapool.h>
#include "mtk-phy.h"

/* TEST CHIP PHY define, edit this in different platform */
#define U3_PHY_I2C_DEV			0x60
#define U3_PHY_PAGE				0xff
#define GPIO_BASE           	0xBFA80700
#define SSUSB_I2C_OUT			GPIO_BASE+0xd0
#define SSUSB_I2C_IN			GPIO_BASE+0xd4

extern int (*I2CWriterPtr)(u8 DevAddr, u8 WordAddr, u8* data_value, u8 data_len);
extern int (*I2CReaderPtr)(u8 DevAddr, u8 WordAddr, u8* data_value, u8 data_len);

/****************************************************************/

#define OUTPUT		1
#define INPUT		0 

#define SDA    		0        /* GPIO #0: I2C data pin */
#define SCL    		1        /* GPIO #1: I2C clock pin */
 
/****************************************************************/

#define SDA_OUT		(1<<0)
#define SDA_OEN		(1<<1)
#define SCL_OUT		(1<<2)
#define SCL_OEN		(1<<3)

#define SDA_IN_OFFSET		0
#define SCL_IN_OFFSET		1

#define os_writel(addr,data) {\
		(*((volatile PHY_UINT32*)(addr)) = data);\
	}
#define os_readl(addr)  *((volatile PHY_UINT32*)(addr))
#define os_writelmsk(addr, data, msk) \
		{ os_writel(addr, ((os_readl(addr) & ~(msk)) | ((data) & (msk)))); \
	}
#define os_setmsk(addr, msk) \
	{ os_writel(addr, os_readl(addr) | msk); \
	}
#define os_clrmsk(addr, msk) \
   { os_writel(addr, os_readl(addr) &~ msk); \
   }
/*msk the data first, then umsk with the umsk.*/
#define os_writelmskumsk(addr, data, msk, umsk) \
{\
   os_writel(addr, ((os_readl(addr) & ~(msk)) | ((data) & (msk))) & (umsk));\
}

PHY_INT32 U3HWriteReg32(PHY_UINT32 addr, PHY_UINT32 data)
{
	os_writel(addr, data);

	return 0;
}

PHY_INT32 U3HReadReg32(PHY_UINT32 addr)
{
	return os_readl(addr);
}

PHY_INT32 U3HWriteReg8(PHY_UINT32 addr, PHY_UINT8 data)
{
	os_writelmsk(addr&0xfffffffc, data<<((addr%4)*8), 0xff<<((addr%4)*8));
	
	return 0;
}

PHY_INT8 U3HReadReg8(PHY_UINT32 addr)
{
	return ((os_readl(addr)>>((addr%4)*8))&0xff);
}

PHY_INT32 _U3Write_Reg(PHY_INT32 address, PHY_INT32 value);
PHY_INT32 _U3Read_Reg(PHY_INT32 address);

void gpio_dir_set(PHY_INT32 pin){
	PHY_INT32 addr, temp;
	addr = SSUSB_I2C_OUT;
	temp = DRV_Reg32(addr);
	if(pin == SDA){
		temp |= SDA_OEN;
		DRV_WriteReg32(addr,temp);
	}
	else{
		temp |= SCL_OEN;
		DRV_WriteReg32(addr,temp);
	}
}

void gpio_dir_clr(PHY_INT32 pin){
	PHY_INT32 addr, temp;
	addr = SSUSB_I2C_OUT;
	temp = DRV_Reg32(addr);
	if(pin == SDA){
		temp &= ~SDA_OEN;
		DRV_WriteReg32(addr,temp);
	}
	else{
		temp &= ~SCL_OEN;
		DRV_WriteReg32(addr,temp);
	}
}

void gpio_dout_set(PHY_INT32 pin){
	PHY_INT32 addr, temp;
	addr = SSUSB_I2C_OUT;
	temp = DRV_Reg32(addr);
	if(pin == SDA){
		temp |= SDA_OUT;
		DRV_WriteReg32(addr,temp);
	}
	else{
		temp |= SCL_OUT;
		DRV_WriteReg32(addr,temp);
	}
}

void gpio_dout_clr(PHY_INT32 pin){
	PHY_INT32 addr, temp;
	addr = SSUSB_I2C_OUT;
	temp = DRV_Reg32(addr);
	if(pin == SDA){
		temp &= ~SDA_OUT;
		DRV_WriteReg32(addr,temp);
	}
	else{
		temp &= ~SCL_OUT;
		DRV_WriteReg32(addr,temp);
	}
}

PHY_INT32 gpio_din(PHY_INT32 pin){
	PHY_INT32 addr, temp;
	addr = SSUSB_I2C_IN;
	temp = DRV_Reg32(addr);
	if(pin == SDA){
		temp = (temp >> SDA_IN_OFFSET) & 1;
	}
	else{
		temp = (temp >> SCL_IN_OFFSET) & 1;
	}
	return temp;
}

#define     GPIO_DIR_SET(pin)   gpio_dir_set(pin)
#define     GPIO_DOUT_SET(pin)  gpio_dout_set(pin);
#define     GPIO_DIR_CLR(pin)   gpio_dir_clr(pin)
#define     GPIO_DOUT_CLR(pin)  gpio_dout_clr(pin)
#define     GPIO_DIN(pin)       gpio_din(pin)


PHY_UINT32 i2c_dummy_cnt;

#define I2C_DELAY 10
#define I2C_DUMMY_DELAY(_delay) for (i2c_dummy_cnt = ((_delay)) ; i2c_dummy_cnt!=0; i2c_dummy_cnt--)

void GPIO_InitIO(PHY_UINT32 dir, PHY_UINT32 pin)
{  
    if (dir == OUTPUT)
    {   
        GPIO_DIR_SET(pin);
    }
    else
    {   
        GPIO_DIR_CLR(pin);
    }
    I2C_DUMMY_DELAY(100);
}

void GPIO_WriteIO(PHY_UINT32 data, PHY_UINT32 pin)
{
    if (data == 1){
		GPIO_DOUT_SET(pin);
    }
    else{
        GPIO_DOUT_CLR(pin);
    }
}

PHY_UINT32 GPIO_ReadIO( PHY_UINT32 pin)
{
    PHY_UINT16 data;
    data=GPIO_DIN(pin);
    return (PHY_UINT32)data;
}


void SerialCommStop(void)
{
    GPIO_InitIO(OUTPUT,SDA);
    GPIO_WriteIO(0,SCL);
    I2C_DUMMY_DELAY(I2C_DELAY);
    GPIO_WriteIO(0,SDA);
    I2C_DUMMY_DELAY(I2C_DELAY);
    GPIO_WriteIO(1,SCL);
    I2C_DUMMY_DELAY(I2C_DELAY);
    GPIO_WriteIO(1,SDA);
    I2C_DUMMY_DELAY(I2C_DELAY);
	GPIO_InitIO(INPUT,SCL);
    GPIO_InitIO(INPUT,SDA);
}

void SerialCommStart(void) /* Prepare the SDA and SCL for sending/receiving */
{
	GPIO_InitIO(OUTPUT,SCL);
    GPIO_InitIO(OUTPUT,SDA);
    GPIO_WriteIO(1,SDA);
    I2C_DUMMY_DELAY(I2C_DELAY);
    GPIO_WriteIO(1,SCL);
    I2C_DUMMY_DELAY(I2C_DELAY);
    GPIO_WriteIO(0,SDA);
    I2C_DUMMY_DELAY(I2C_DELAY);
    GPIO_WriteIO(0,SCL);
    I2C_DUMMY_DELAY(I2C_DELAY);
}

PHY_UINT32 SerialCommTxByte(PHY_UINT8 data) /* return 0 --> ack */
{
    PHY_INT32 i, ack;
    
    GPIO_InitIO(OUTPUT,SDA);

    for(i=8; --i>0;){
        GPIO_WriteIO((data>>i)&0x01, SDA);
        I2C_DUMMY_DELAY(I2C_DELAY);
        GPIO_WriteIO( 1, SCL); /* high */
        I2C_DUMMY_DELAY(I2C_DELAY);
        GPIO_WriteIO( 0, SCL); /* low */
        I2C_DUMMY_DELAY(I2C_DELAY);
    }
    GPIO_WriteIO((data>>i)&0x01, SDA);
    I2C_DUMMY_DELAY(I2C_DELAY);
    GPIO_WriteIO( 1, SCL); /* high */
    I2C_DUMMY_DELAY(I2C_DELAY);
    GPIO_WriteIO( 0, SCL); /* low */
    I2C_DUMMY_DELAY(I2C_DELAY);
    
    GPIO_WriteIO(0, SDA);
    I2C_DUMMY_DELAY(I2C_DELAY);
    GPIO_InitIO(INPUT,SDA);
    I2C_DUMMY_DELAY(I2C_DELAY);
    GPIO_WriteIO(1, SCL);
    I2C_DUMMY_DELAY(I2C_DELAY);
    ack = GPIO_ReadIO(SDA); /* ack 1: error , 0:ok */
    GPIO_WriteIO(0, SCL);
    I2C_DUMMY_DELAY(I2C_DELAY);
    
    if(ack==1)
        return PHY_FALSE;
    else
        return PHY_TRUE;  
}

void SerialCommRxByte(PHY_UINT8 *data, PHY_UINT8 ack)
{
   PHY_INT32 i;
   PHY_UINT32 dataCache;
   
   dataCache = 0;
   GPIO_InitIO(INPUT,SDA);
   for(i=8; --i>=0;){
      dataCache <<= 1;
      I2C_DUMMY_DELAY(I2C_DELAY);
      GPIO_WriteIO(1, SCL);
      I2C_DUMMY_DELAY(I2C_DELAY);
      dataCache |= GPIO_ReadIO(SDA);
      GPIO_WriteIO(0, SCL);
      I2C_DUMMY_DELAY(I2C_DELAY);
   }
   GPIO_InitIO(OUTPUT,SDA);
   GPIO_WriteIO(ack, SDA);
   I2C_DUMMY_DELAY(I2C_DELAY);
   GPIO_WriteIO(1, SCL);
   I2C_DUMMY_DELAY(I2C_DELAY);
   GPIO_WriteIO(0, SCL);
   I2C_DUMMY_DELAY(I2C_DELAY);
   *data = (unsigned char)dataCache;
}


PHY_INT32 I2cWriteReg(PHY_UINT8 dev_id, PHY_UINT8 Addr, PHY_UINT8 Data)
{
    PHY_INT8 acknowledge=0;
	acknowledge = I2CWriterPtr(dev_id, Addr, &Data, 1);
	if(acknowledge)
    	{
      	  	return PHY_FALSE;
    	}
    	else
    	{    
        	return PHY_TRUE;
    	}

}

PHY_INT32 I2cReadReg(PHY_UINT8 dev_id, PHY_UINT8 Addr, PHY_UINT8 *Data)
{
    PHY_INT32 acknowledge = 0;   
	acknowledge = I2CReaderPtr(dev_id, Addr, Data, 1);
    return acknowledge;
} 


void _U3_Write_Bank(PHY_INT32 bankValue){
	I2cWriteReg(U3_PHY_I2C_DEV, U3_PHY_PAGE, bankValue);	
}

PHY_INT32 _U3Write_Reg(PHY_INT32 address, PHY_INT32 value){
	I2cWriteReg(U3_PHY_I2C_DEV, address, value);
}

PHY_INT32 _U3Read_Reg(PHY_INT32 address){
	PHY_INT8 *pu1Buf;
	PHY_INT32 ret;
	
	pu1Buf = (char *)kmalloc(1, GFP_NOIO);
	ret = I2cReadReg(U3_PHY_I2C_DEV, address, pu1Buf);
	if(ret == PHY_FALSE){
		printk(KERN_ERR "Read failed\n");
		return PHY_FALSE;
	}
	ret = (unsigned char)pu1Buf[0];
	kfree(pu1Buf);
	return ret;
	
}

PHY_INT32 U3PhyWriteReg32(PHY_UINT32 addr, PHY_UINT32 data){
	PHY_INT32 bank;
	PHY_INT32 addr8;
	PHY_INT8 data_0, data_1, data_2, data_3;

	bank = (addr >> 16) & 0xff;
	addr8 = addr & 0xff;
	data_0 = data & 0xff;
	data_1 = (data>>8) & 0xff;
	data_2 = (data>>16) & 0xff;
	data_3 = (data>>24) & 0xff;
	
#if 0	
	HW_I2C_writer(U3_PHY_I2C_DEV, U3_PHY_PAGE, bank, 1);
	HW_I2C_writer(U3_PHY_I2C_DEV, addr8, data_0, 1);
	HW_I2C_writer(U3_PHY_I2C_DEV, addr8+1, data_1, 1);
	HW_I2C_writer(U3_PHY_I2C_DEV, addr8+2, data_2, 1);
	HW_I2C_writer(U3_PHY_I2C_DEV, addr8+3, data_3, 1);
#else
	_U3_Write_Bank(bank);
	_U3Write_Reg(addr8, data_0);
	_U3Write_Reg(addr8+1, data_1);
	_U3Write_Reg(addr8+2, data_2);
	_U3Write_Reg(addr8+3, data_3);
#endif

	return 0;
}

PHY_INT32 U3PhyReadReg32(PHY_UINT32 addr){
	PHY_INT32 bank;
	PHY_INT32 addr8;
	PHY_INT32 data;

	bank = (addr >> 16) & 0xff;
	addr8 = addr & 0xff;

	_U3_Write_Bank(bank);
	data = _U3Read_Reg(addr8);
	data |= (_U3Read_Reg(addr8+1) << 8);
	data |= (_U3Read_Reg(addr8+2) << 16);
	data |= (_U3Read_Reg(addr8+3) << 24);
	return data;
}

PHY_INT32 U3PhyWriteReg8(PHY_UINT32 addr, PHY_UINT8 data)
{
	PHY_INT32 bank;
	PHY_INT32 addr8;
	
	bank = (addr >> 16) & 0xff;
	addr8 = addr & 0xff;
	_U3_Write_Bank(bank);
	_U3Write_Reg(addr8, data);
	
	return PHY_TRUE;
}

PHY_INT8 U3PhyReadReg8(PHY_UINT32 addr){
	PHY_INT32 bank;
	PHY_INT32 addr8;
	PHY_INT32 data;

	bank = (addr >> 16) & 0xff;
	addr8 = addr & 0xff;
	_U3_Write_Bank(bank);
	data = _U3Read_Reg(addr8);
	return data;
}
#endif
