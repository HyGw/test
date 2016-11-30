#include <linux/errno.h>
#include <linux/init.h>
#include <linux/kernel_stat.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/random.h>
#include <linux/string.h>	

#include <asm/bitops.h>
#include <asm/bootinfo.h>
#include <asm/irq.h>
#include <asm/mipsregs.h>
#include <asm/system.h>
#include <linux/circ_buf.h>
#include <asm/tc3162.h>
#include <asm/io.h>

typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned int U32;

#define FALSE 	0
#define TRUE 	1

#define REG_BASE    0xbf900000
#define x4 * 4

#define RG_AFE_DIGITAL_BASE			(REG_BASE+0x01d0 x4)
#define RG_AFE_WR_CODE			((volatile U32*)(RG_AFE_DIGITAL_BASE + 0x0c x4))
#define RG_AFE_WR_STATUS			((volatile U32*)(RG_AFE_DIGITAL_BASE + 0x0d x4))
#define RG_AFE_RD_DATA				((volatile U32*)(RG_AFE_DIGITAL_BASE + 0x0e x4))
#define RG_AFE_WR_CODE0_D			((volatile U32*)(RG_AFE_DIGITAL_BASE + 0x0f x4))
#define RG_AFE_WR_CODE0_A			((volatile U32*)(RG_AFE_DIGITAL_BASE + 0x10 x4))

#define MT7550_REG_READ_MODE 1
#define MT7550_REG_WRITE_MODE 0

#define AFE_WRITE_CHECK_CNT 10
#define AFE_READ_CHECK_CNT 20

static U8 AfeMT7550RWCmdDoneFlag(U8 mode)
{
	U8 cmd_done =0; 

	U32 temp = (*RG_AFE_WR_STATUS); 
	
	if(mode == MT7550_REG_WRITE_MODE)
	{
		cmd_done = ((temp>> 8) & 0x00000001);
	}
	else if(mode == MT7550_REG_READ_MODE)
	{
		cmd_done = ((temp>> 0) & 0x00000001);
	}

	return cmd_done;
}

U8 AfeMT7550WriteReg(unsigned int reg, unsigned int data)
{
	U8 valid=1;
	U16 wait_cnt = 0;
			
	*RG_AFE_WR_CODE0_A = reg; 
	*RG_AFE_WR_CODE0_D = data; 

	*RG_AFE_WR_CODE = (((*RG_AFE_WR_CODE)& 0xfffffefc)|(MT7550_REG_WRITE_MODE<<8));
	
	while(1)
	{		
		//prom_printf("| ");
		pause(1);
		if(AfeMT7550RWCmdDoneFlag(MT7550_REG_WRITE_MODE) == TRUE)	
		{	  
			break;
		}
			
		wait_cnt ++;		
		
		if(wait_cnt >= AFE_WRITE_CHECK_CNT) 
		{  
			valid = 0;
			break;
		}  
	}
	//prom_printf("\r\n");
	
	return valid;
	
}


U8 AfeMT7550ReadReg(unsigned int reg, U8 *data)
{
	U8 valid = 1;
	U32 read_cnt = 0;

	
	*RG_AFE_WR_CODE0_A = reg;

	*RG_AFE_WR_CODE = (((*RG_AFE_WR_CODE)& 0xfffffeff)|(MT7550_REG_READ_MODE<<8));
	
	while(1)
	{		
		//prom_printf("| ");
		pause(1);
		if(AfeMT7550RWCmdDoneFlag(MT7550_REG_READ_MODE) == TRUE)	
		{	  
			break;				
		}
		
		read_cnt ++;		
	
		if(read_cnt >= AFE_READ_CHECK_CNT) 
		{  
			valid = 0; 
			break;
		}  
	}
	//prom_printf("\r\n");
	if(valid == 1)
	{
		*data = ((*RG_AFE_RD_DATA)& 0xff);
	}

	return valid;
}	
