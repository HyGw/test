/*
 * init.c: early initialisation code for R39XX Class PDAs
 *
 * Copyright (C) 1999 Harald Koerfgen
 *
 * $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/bootrom/bootram/io/init.c#1 $
 */
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/kernel_stat.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/random.h>
#include <asm/bitops.h>
#include <asm/bootinfo.h>
#include <asm/irq.h>
#include <asm/mipsregs.h>
#include <asm/system.h>
#include <linux/circ_buf.h>
#include <asm/tc3162.h>
#include <asm/io.h>

#define CAL_DBG_PRINT

#ifndef BOOTROM_IN_SRAM //Code shrink for bootrom in SRAM
static unsigned long uclk_65000[13]={ // 65000*(b*16*3)/1000000
	359424,		// Baud rate 115200
	179712,		// Baud rate 57600
	119808,		// Baud rate 38400
	89856,		// Baud rate 28800
	59904,		// Baud rate 19200
	44928,		// Baud rate 14400
	29952,		// Baud rate 9600
	14976,		// Baud rate 4800
	7488,		// Baud rate 2400
	3744,		// Baud rate 1200
	1872,		// Baud rate 600
	936,		// Baud rate 300
	343			// Baud rate 110
};
#endif

/* crystal clock is 20Mhz */
static unsigned long uclk_20M[13]={ // 65000*(b*16*1)/2000000
	59904,		// Baud rate 115200
	29952,		// Baud rate 57600
	19968,		// Baud rate 38400
	14976,		// Baud rate 28800
	9984,		// Baud rate 19200
	7488,		// Baud rate 14400
	4992,		// Baud rate 9600
	2496,		// Baud rate 4800
	1248,		// Baud rate 2400
	624,		// Baud rate 1200
	312,		// Baud rate 600
	156,		// Baud rate 300
	57			// Baud rate 110
};

void uart_init(void)
{
	int i;
	unsigned long	div_x, div_y;
	unsigned long	word;

//	pause(100);
// Set FIFO controo enable, reset RFIFO, TFIFO, 16550 mode, watermark=0x00 (1 byte)
	tc_outb(CR_UART_FCR, UART_FCR|UART_WATERMARK);

// Set modem control to 0
	tc_outb(CR_UART_MCR, UART_MCR);

// Disable IRDA, Disable Power Saving Mode, RTS , CTS flow control
	tc_outb(CR_UART_MISCC, UART_MISCC);

// Set interrupt Enable to, enable Tx, Rx and Line status
	tc_outb(CR_UART_IER, UART_IER);	
//	

	/* access the bardrate divider */

	tc_outb(CR_UART_LCR, UART_BRD_ACCESS);

	div_y = UART_XYD_Y;
	if (isRT63165 || isRT63365 || isMT751020 || isMT7505 || isEN751221)
		div_x = (unsigned int)(uclk_20M[0]);
	#ifndef BOOTROM_IN_SRAM//Code shrink for bootrom in SRAM
	else
		div_x = (unsigned int)(uclk_65000[0]/get_SYS_HCLK())*2;
	#endif
	word = (div_x<<16)|div_y;
	tc_outl(CR_UART_XYD, word);

	if (isRT63165 || isRT63365 || isMT751020 || isMT7505 || isEN751221) {
	/* Set Baud Rate Divisor to 1*16		*/
		tc_outb(CR_UART_BRDL, UART_BRDL_20M);
		tc_outb(CR_UART_BRDH, UART_BRDH_20M);
	}
	#ifndef BOOTROM_IN_SRAM//Code shrink for bootrom in SRAM
	else {
	/* Set Baud Rate Divisor to 3*16		*/
		tc_outb(CR_UART_BRDL, UART_BRDL);
		tc_outb(CR_UART_BRDH, UART_BRDH);
	}
	#endif

/* Set DLAB = 0, clength = 8, stop =1, no parity check 	*/
	tc_outb(CR_UART_LCR, UART_LCR);
}

void serial_outc(char c)
{
	while (!(tc_inb(CR_UART_LSR) & LSR_THRE))
			;
	tc_outb(CR_UART_THR, c);
	
#ifdef TCSUPPORT_UART2
		while (!(tc_inb(CR_UART2_LSR) & LSR_THRE))
				;
		tc_outb(CR_UART2_THR, c);
#endif
}

char serial_inc(void)
{
	int i;

    while (1)
	{
		if 	(tc_inb(CR_UART_LSR) & 0x1)
			break;		
	}	
	i=tc_inb(CR_UART_RBR);

	return (i & 0xff);
}

/* Change #define to fuction for reduce bootbase code size */
int get_SYS_HCLK(void)
{
	return SYS_HCLK;
}

int serial_tstc(void)
{
	return (tc_inb(CR_UART_LSR) & 0x1);
}

#ifdef SPRAM_IMG
#define CAL_DBG_PRINT
void prom_puts(const char *s)
{
	while (*s != 0) {
		char c = *s++;
#ifdef CAL_DBG_PRINT
		if (c == '\n')
			serial_outc('\r');
		serial_outc(c);
#endif
	}

} 

void prom_print_hex(unsigned long val, int len)
{
	//prom_printn(val, 16, (char *)"0123456789abcdef");
	char c;
	int i;
	
	for(i = len - 1; i >= 0; i--) {
		c = (char)((val >> (i * 4)) & 0x0f);

		if(c > 9)
			c += ('a' - 10);
		else
			c += '0';
#ifdef CAL_DBG_PRINT
		serial_outc(c);
#endif
	}
}

void prom_print_dec(unsigned long val)
{
	//prom_printn(val, 10, (char *)"0123456789");
	int leading_zero = 1;
	unsigned long  divisor, result, remainder;

	remainder = val;

	for(divisor = 1000000000; 
	    divisor > 0; 
	    divisor /= 10) {
		result = remainder / divisor;
		remainder %= divisor;

		if(result != 0 || divisor == 1)
			leading_zero = 0;
#ifdef CAL_DBG_PRINT
		if(leading_zero == 0)
			serial_outc((char)(result) + '0');
#endif
	}
}

#else

/*
 * Helpful for debugging :-)
 */
int prom_printf(const char * fmt, ...)
{

	static char buf[1024];
	va_list args;
	char c;
	int i = 0;

	/*
	 * Printing messages via serial port
	 */
	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	//do_printf(buf, fmt, args);
	va_end(args);

	for (i = 0; buf[i] != '\0'; i++) {
		c = buf[i];
		if (c == '\n')
			serial_outc('\r');
		serial_outc(c);
	}

	return i;
}
#endif

#ifdef TCSUPPORT_UART2
void uart2_init(void)
{
	int i;
	unsigned long	div_x, div_y;
	unsigned long	word;

    unsigned int reg;
    if(isEN751221)
    {
        reg = VPint(0xbfa20104); //GPIO_SHR_SCH
        reg |= (0x1<<18); //UART2_MODE
        VPint(0xbfa20104) = reg;  
    }
    else
    {
        reg = VPint(0xbfb00860); //GPIO_SHR_SCH
        reg |= (0x1<<6); //UART2_MODE
        VPint(0xbfb00860) = reg;
    }

// Set FIFO controo enable, reset RFIFO, TFIFO, 16550 mode, watermark=0x00 (1 byte)
	tc_outb(CR_UART2_FCR, UART_FCR|UART_WATERMARK);

// Set modem control to 0
	tc_outb(CR_UART2_MCR, UART_MCR);

// Disable IRDA, Disable Power Saving Mode, RTS , CTS flow control
	tc_outb(CR_UART2_MISCC, UART_MISCC);

// Set interrupt Enable to, enable Tx, Rx and Line status
	tc_outb(CR_UART2_IER, UART_IER);	
//	

	/* access the bardrate divider */

	tc_outb(CR_UART2_LCR, UART_BRD_ACCESS);

	div_y = UART_XYD_Y;
	
	div_x = (unsigned int)(uclk_20M[0]);

	word = (div_x<<16)|div_y;
	tc_outl(CR_UART2_XYD, word);

	/* Set Baud Rate Divisor to 1*16		*/
	tc_outb(CR_UART2_BRDL, UART_BRDL_20M);
	tc_outb(CR_UART2_BRDH, UART_BRDH_20M);

/* Set DLAB = 0, clength = 8, stop =1, no parity check 	*/
	tc_outb(CR_UART2_LCR, UART_LCR);
}
#endif
