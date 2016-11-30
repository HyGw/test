#include "xhci-mtk-power.h"
#include "xhci.h"
#include <linux/kernel.h>       /* printk() */
#include <linux/slab.h>
#include <linux/delay.h>

#define CON_HOST_DEV	0

#if CON_HOST_DEV
//filter those ports assigned to device
int getU3PortNumber(){
	int port_num;
	int real_port_num;
	int i, temp;
	
	//check if any port assigned to device
	port_num = SSUSB_U3_PORT_NUM(readl(SSUSB_IP_CAP));
	real_port_num = port_num;
	for(i=0; i<port_num; i++){
		temp = readl(SSUSB_U3_CTRL(i));
		if(!(temp & SSUSB_U3_PORT_HOST_SEL)){
			real_port_num--;
		}
	}
	return real_port_num;
}

//filter those ports assigned to device
int getU2PortNumber(){
	int port_num;
	int real_port_num;
	int i, temp;
	
	//check if any port assigned to device
	port_num = SSUSB_U2_PORT_NUM(readl(SSUSB_IP_CAP));
	real_port_num = port_num;
	for(i=0; i<port_num; i++){
		temp = readl(SSUSB_U2_CTRL(i));
		if(!(temp & SSUSB_U2_PORT_HOST_SEL)){
			real_port_num--;
		}
	}
	return real_port_num;
}

//filter those ports assigned to device
int getRealPortIndex(int port_index, int port_rev){
	int real_port_index, tmp_port_index;
	int i, temp;
	int portNum;

	real_port_index = 0;
	tmp_port_index = 0;
	if(port_rev == 0x3){
		//SS port
		portNum = getU3PortNumber();
		for(i=0; i<portNum; i++){
			temp = SSUSB_U3_CTRL(i);
			tmp_port_index++;
			if(temp & SSUSB_U3_PORT_HOST_SEL){
				real_port_index++;
				if(real_port_index == port_index){
					return tmp_port_index;
				}
			}
		}
	}
	else{
		//HS port
		portNum = getU2PortNumber();
		for(i=0; i<portNum; i++){
			temp = SSUSB_U2_CTRL(i);
			tmp_port_index++;
			if(temp & SSUSB_U2_PORT_HOST_SEL){
				real_port_index++;
				if(real_port_index == port_index){
					return tmp_port_index;
				}
			}
		}
	}
	return port_index;
}

#endif

/* set 1 to PORT_POWER of PORT_STATUS register of each port */
void enableXhciAllPortPower(struct xhci_hcd *xhci){
	int i;
	u32 port_id, temp;
	u32 __iomem *addr;

#if CON_HOST_DEV
	g_num_u3_port = getU3PortNumber();
	g_num_u2_port = getU2PortNumber();
#else
	g_num_u3_port = SSUSB_U3_PORT_NUM(readl(SSUSB_IP_CAP));
	g_num_u2_port = SSUSB_U2_PORT_NUM(readl(SSUSB_IP_CAP));
#endif
	for(i=1; i<=g_num_u3_port; i++){
		port_id=i;
		addr = &xhci->op_regs->port_status_base + NUM_PORT_REGS*(port_id-1 & 0xff);
		temp = xhci_readl(xhci, addr);
		temp = xhci_port_state_to_neutral(temp);
		temp |= PORT_POWER;
		xhci_writel(xhci, temp, addr);
	}
	for(i=1; i<=g_num_u2_port; i++){
		port_id=i+g_num_u3_port;
		addr = &xhci->op_regs->port_status_base + NUM_PORT_REGS*(port_id-1 & 0xff);
		temp = xhci_readl(xhci, addr);
		temp = xhci_port_state_to_neutral(temp);
		temp |= PORT_POWER;
		xhci_writel(xhci, temp, addr);
	}
}

void enableAllClockPower(){

	int i;
	u32 temp;
	int num_u3_port;
	int num_u2_port;
	
	num_u3_port = SSUSB_U3_PORT_NUM(readl(SSUSB_IP_CAP));
	num_u2_port = SSUSB_U2_PORT_NUM(readl(SSUSB_IP_CAP));

	//2.	Enable xHC
	writel(readl(SSUSB_IP_PW_CTRL) | (SSUSB_IP_SW_RST), SSUSB_IP_PW_CTRL);
	writel(readl(SSUSB_IP_PW_CTRL) & (~SSUSB_IP_SW_RST), SSUSB_IP_PW_CTRL);
	writel(readl(SSUSB_IP_PW_CTRL_1) & (~SSUSB_IP_PDN), SSUSB_IP_PW_CTRL_1);
	
	//1.	Enable target ports 
	for(i=0; i<num_u3_port; i++){
		temp = readl(SSUSB_U3_CTRL(i));
#if CON_HOST_DEV
		if(temp & SSUSB_U3_PORT_HOST_SEL){
			temp = temp & (~SSUSB_U3_PORT_PDN) & (~SSUSB_U3_PORT_DIS);
		}
#else
		temp = temp & (~SSUSB_U3_PORT_PDN) & (~SSUSB_U3_PORT_DIS);
#endif
		
		writel(temp, SSUSB_U3_CTRL(i));
	}
	for(i=0; i<num_u2_port; i++){
		temp = readl(SSUSB_U2_CTRL(i));
#if CON_HOST_DEV		
		if(temp & SSUSB_U2_PORT_HOST_SEL){
			temp = temp & (~SSUSB_U2_PORT_PDN) & (~SSUSB_U2_PORT_DIS);
		}
#else
		temp = temp & (~SSUSB_U2_PORT_PDN) & (~SSUSB_U2_PORT_DIS);
#endif
		writel(temp, SSUSB_U2_CTRL(i));

	}
	msleep(100);
}

//called after HC initiated
void disableAllClockPower(){
	int i;
	u32 temp;
	int num_u3_port;
	int num_u2_port;
		
	num_u3_port = SSUSB_U3_PORT_NUM(readl(SSUSB_IP_CAP));
	num_u2_port = SSUSB_U2_PORT_NUM(readl(SSUSB_IP_CAP));

	//disable target ports
	for(i=0; i<num_u3_port; i++){
		temp = readl(SSUSB_U3_CTRL(i));
#if CON_HOST_DEV
		if(temp & SSUSB_U3_PORT_HOST_SEL){
			temp = temp | SSUSB_U3_PORT_PDN;
		}
#else
		temp = temp | SSUSB_U3_PORT_PDN;
#endif
		writel(temp, SSUSB_U3_CTRL(i));
	}
	for(i=0; i<num_u2_port; i++){
		temp = readl(SSUSB_U2_CTRL(i));
#if CON_HOST_DEV
		if(temp & SSUSB_U2_PORT_HOST_SEL){
			temp = temp | SSUSB_U2_PORT_PDN;
		}
#else
		temp = temp | SSUSB_U2_PORT_PDN;
#endif
		writel(temp, SSUSB_U2_CTRL(i));
	}
	msleep(100);
}

//(X)disable clock/power of a port 
//(X)if all ports are disabled, disable IP ctrl power
//disable all ports and IP clock/power, this is just mention HW that the power/clock of port 
//and IP could be disable if suspended.
//If doesn't not disable all ports at first, the IP clock/power will never be disabled
//(some U2 and U3 ports are binded to the same connection, that is, they will never enter suspend at the same time
//port_index: port number
//port_rev: 0x2 - USB2.0, 0x3 - USB3.0 (SuperSpeed)
void disablePortClockPower(int port_index, int port_rev){
	int i;
	u32 temp;
	int real_index;
	
#if CON_HOST_DEV
	real_index = getRealPortIndex(port_index, port_rev);
#else
	real_index = port_index;
#endif


	if(port_rev == 0x3){
		temp = readl(SSUSB_U3_CTRL(real_index));
		temp = temp | (SSUSB_U3_PORT_PDN);
		writel(temp, SSUSB_U3_CTRL(real_index));
	}
	else if(port_rev == 0x2){
		temp = readl(SSUSB_U2_CTRL(real_index));
		temp = temp | (SSUSB_U2_PORT_PDN);
		writel(temp, SSUSB_U2_CTRL(real_index));
	}
	writel(readl(SSUSB_IP_PW_CTRL_1) | (SSUSB_IP_PDN), SSUSB_IP_PW_CTRL_1);
}

//if IP ctrl power is disabled, enable it
//enable clock/power of a port
//port_index: port number
//port_rev: 0x2 - USB2.0, 0x3 - USB3.0 (SuperSpeed)
void enablePortClockPower(int port_index, int port_rev){
	int i;
	u32 temp;
	int real_index;
	
#if CON_HOST_DEV
	real_index = getRealPortIndex(port_index, port_rev);
#else
	real_index = port_index;
#endif

	writel(readl(SSUSB_IP_PW_CTRL_1) & (~SSUSB_IP_PDN), SSUSB_IP_PW_CTRL_1);

	if(port_rev == 0x3){
		temp = readl(SSUSB_U3_CTRL(real_index));
		temp = temp & (~SSUSB_U3_PORT_PDN);
		writel(temp, SSUSB_U3_CTRL(real_index));
	}
	else if(port_rev == 0x2){
		temp = readl(SSUSB_U2_CTRL(real_index));
		temp = temp & (~SSUSB_U2_PORT_PDN);
		writel(temp, SSUSB_U2_CTRL(real_index));
	}
}
