/*
 *	Serial driver for TC3162 SoC
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ioport.h>
#include <linux/serial.h>
#include <linux/serial_core.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/console.h>

#include <asm/tc3162/tc3162.h>

#define TC3162_NR_PORTS				1

#define TC3162_UART_SIZE			0x30

#define PORT_TC3162					3162
#if 0 // def CONFIG_TC3162_ADSL
void (*send_uart_msg)(char* msg, int len);
EXPORT_SYMBOL(send_uart_msg);
static char tuart_buf[1024];
#endif

#define UART_BAUDRATE		115200

#define	UART_BRDL_20M		0x01
#define	UART_BRDH_20M		0x00
#define UART_CRYSTAL_CLK_20M	20000000
#define UART_CRYSTAL_CLK_DIV	10

/* crystal clock is 20Mhz */
/*---------------------
| uclk_20M | baudrate |
|---------------------|
| 59904    | 115200   |
| 29952    | 57600    |
| 19968    | 38400    |
| 14976    | 28800    |
| 9984     | 19200    |
| 7488     | 14400    |
| 4992     | 9600     |
| 2496     | 4800     |
| 1248     | 2400     |
| 624      | 1200     |
| 312      | 600      |
| 156      | 300      |
| 57       | 110      |
---------------------*/
static unsigned long tc3162_get_uclk_20M(unsigned long long baud)
{
	unsigned long uclk_20M = 0;

	uclk_20M = (baud * UART_XYD_Y * (UART_BRDH_20M << 8 | UART_BRDL_20M) * 16) / (UART_CRYSTAL_CLK_20M / UART_CRYSTAL_CLK_DIV);

	return uclk_20M;
}
#ifdef TCSUPPORT_MT7510_E1
#define READ_OTHER(x) ((x & 0xc) + 0xbfb003a0)
#endif
static void tc3162ser_stop_tx(struct uart_port *port)
{
#ifdef TCSUPPORT_MT7510_E1
	unsigned int tmp;
	tmp = VPint(READ_OTHER(CR_UART2_IER));
	wmb();
	VPchar(CR_UART2_IER) &= ~IER_THRE_INTERRUPT_ENABLE;	
	wmb();
#else
	VPchar(CR_UART2_IER) &= ~IER_THRE_INTERRUPT_ENABLE;	
#endif
}

static void tc3162ser_irq_rx(struct uart_port *port)
{
	struct tty_struct *tty = port->state->port.tty;
	unsigned int ch, flg;

#ifdef TCSUPPORT_MT7510_E1
	unsigned int tmp;

	while (1) {
		tmp = VPint(READ_OTHER(CR_UART2_LSR));
		wmb();
		if(!(LSR_INDICATOR2 & LSR_RECEIVED_DATA_READY)){
			wmb();
			break;
		}
#else
	while (LSR_INDICATOR2 & LSR_RECEIVED_DATA_READY) {
#endif
		/* 
		 * We need to read rds before reading the 
		 * character from the fifo
		 */
#ifdef TCSUPPORT_MT7510_E1
		tmp = VPint(READ_OTHER(CR_UART2_RBR));
		wmb();
		ch = VPchar(CR_UART2_RBR);
		wmb();
#else
		ch = VPchar(CR_UART2_RBR);
#endif
		port->icount.rx++;

		if (tty->low_latency)
			tty_flip_buffer_push(tty);

		flg = TTY_NORMAL;

		tty_insert_flip_char(tty, ch, flg);
	}
	tty_flip_buffer_push(tty);
}

static void tc3162ser_irq_tx(struct uart_port *port)
{
	struct circ_buf *xmit = &port->state->xmit;
	int count;

#if 0 // def CONFIG_TC3162_ADSL
	int len=0;
	memset(tuart_buf, 0, sizeof(tuart_buf));
#endif
	if (port->x_char) {
		VPchar(CR_UART2_THR) = port->x_char;
#ifdef TCSUPPORT_MT7510_E1
		wmb();
#endif
		port->icount.tx++;
		port->x_char = 0;
		return;
	}
	if (uart_circ_empty(xmit) || uart_tx_stopped(port)) {
		tc3162ser_stop_tx(port);
		return;
	}

	count = port->fifosize;
	do {
		VPchar(CR_UART2_THR) = 
		    xmit->buf[xmit->tail];
#ifdef TCSUPPORT_MT7510_E1
		wmb();
#endif
#if 0 // def CONFIG_TC3162_ADSL
		if((void *)send_uart_msg){
			tuart_buf[len] = xmit->buf[xmit->tail];
			len++;
		}
#endif
		xmit->tail = (xmit->tail + 1) & (UART_XMIT_SIZE - 1);
		port->icount.tx++;
		if (uart_circ_empty(xmit))
			break;
	} while (--count > 0);
#if 0 // def CONFIG_TC3162_ADSL
	if((void *)send_uart_msg){
		send_uart_msg(tuart_buf, len);
	}
#endif

	if (uart_circ_chars_pending(xmit) < WAKEUP_CHARS)
		uart_write_wakeup(port);

	if (uart_circ_empty(xmit))
		tc3162ser_stop_tx(port);
}

static irqreturn_t tc3162ser_irq(int irq, void *dev_id)
{
	struct uart_port *port = dev_id;
	uint8 iir;

#ifdef TCSUPPORT_MT7510_E1
	unsigned int tmp;
	tmp = VPint(READ_OTHER(CR_UART2_IIR));
	wmb();
	iir = IIR_INDICATOR2;
	wmb();
#else
	iir = IIR_INDICATOR2;
#endif

	if (((iir & IIR_RECEIVED_DATA_AVAILABLE) == IIR_RECEIVED_DATA_AVAILABLE) ||
		((iir & IIR_RECEIVER_IDLE_TRIGGER) == IIR_RECEIVER_IDLE_TRIGGER)) 
    {   		tc3162ser_irq_rx(port);
    }
	if ((iir & IIR_TRANSMITTED_REGISTER_EMPTY) == IIR_TRANSMITTED_REGISTER_EMPTY) 
    {
		tc3162ser_irq_tx(port);    
    }

	return IRQ_HANDLED;
}

static unsigned int tc3162ser_tx_empty(struct uart_port *port)
{
#ifdef TCSUPPORT_MT7510_E1
	unsigned int tmp;
        tmp = VPint(READ_OTHER(CR_UART2_IIR));
	wmb();
#endif
	unsigned int ret;

        ret = (LSR_INDICATOR2 & LSR_THRE) ? TIOCSER_TEMT : 0;
#ifdef TCSUPPORT_MT7510_E1
	wmb();
#endif	
        return ret;
}

static void tc3162ser_set_mctrl(struct uart_port *port, unsigned int mctrl)
{
}

static unsigned int tc3162ser_get_mctrl(struct uart_port *port)
{
	unsigned int result = 0;
	return result;
}

static void tc3162ser_start_tx(struct uart_port *port)
{
#ifdef TCSUPPORT_MT7510_E1
        unsigned int tmp;
        tmp = VPint(READ_OTHER(CR_UART2_IER));
	wmb();
	VPchar(CR_UART2_IER) |= IER_THRE_INTERRUPT_ENABLE;
	wmb();
#else
	VPchar(CR_UART2_IER) |= IER_THRE_INTERRUPT_ENABLE;	
#endif
}

static void tc3162ser_stop_rx(struct uart_port *port)
{
#ifdef TCSUPPORT_MT7510_E1
        unsigned int tmp;
        tmp = VPint(READ_OTHER(CR_UART2_IER));
	wmb();
	VPchar(CR_UART2_IER) |= IER_THRE_INTERRUPT_ENABLE;
	wmb();
#else
	VPchar(CR_UART2_IER) &= ~IER_RECEIVED_DATA_INTERRUPT_ENABLE;	
#endif
}

static void tc3162ser_enable_ms(struct uart_port *port)
{
}

static void tc3162ser_break_ctl(struct uart_port *port, int break_state)
{
	unsigned long flags;

	spin_lock_irqsave(&port->lock, flags);
	spin_unlock_irqrestore(&port->lock, flags);
}

#ifdef CONFIG_MIPS_TC3262
static void tc3162ser_irq_dispatch(void)
{
	do_IRQ(HSUART_INT);
}
#endif

static int tc3162ser_startup(struct uart_port *port)
{
	int ret;

#ifdef CONFIG_MIPS_TC3262
	if (cpu_has_vint)  
		set_vi_handler(port->irq, tc3162ser_irq_dispatch);
#endif

	ret = request_irq(port->irq, tc3162ser_irq, 0, "TC3162 UART2", port);
	if (ret) {
		printk(KERN_ERR "Couldn't get irq %d ret=%d\n", port->irq, ret);
		return ret;
	}
#ifdef TCSUPPORT_MT7510_E1
	unsigned int tmp;
	tmp = VPint(READ_OTHER(CR_UART2_IER));
	wmb();
	VPchar(CR_UART2_IER) |= IER_RECEIVED_DATA_INTERRUPT_ENABLE;
	wmb();
#else
	VPchar(CR_UART2_IER) |= IER_RECEIVED_DATA_INTERRUPT_ENABLE;	
#endif
	return 0;
}

static void tc3162ser_shutdown(struct uart_port *port)
{
#ifdef TCSUPPORT_MT7510_E1
        unsigned int tmp;
        tmp = VPint(READ_OTHER(CR_UART2_IER));
	wmb();
	VPchar(CR_UART2_IER) &= ~IER_RECEIVED_DATA_INTERRUPT_ENABLE;
	wmb();
#else
	VPchar(CR_UART2_IER) &= ~IER_RECEIVED_DATA_INTERRUPT_ENABLE;	
#endif
	free_irq(port->irq, port);
}

static void tc3162ser_set_termios(struct uart_port *port,
    struct ktermios *termios, struct ktermios *old)
{
	unsigned int baud, quot;
	unsigned long flags;

	termios->c_cflag |= CREAD;

	baud = UART_BAUDRATE;
	quot = uart_get_divisor(port, baud);

	spin_lock_irqsave(&port->lock, flags);

	/*
	 * Update the per-port timeout.
	 */
	uart_update_timeout(port, termios->c_cflag, baud);

	/*
	 * Characters to ignore
	 */
	port->ignore_status_mask = 0;

	spin_unlock_irqrestore(&port->lock, flags);
}

static const char *tc3162ser_type(struct uart_port *port)
{
	return port->type == PORT_TC3162 ? "TC3162" : NULL;
}

static void tc3162ser_config_port(struct uart_port *port, int flags)
{
	if (flags & UART_CONFIG_TYPE)
		port->type = PORT_TC3162;
}

static void tc3162ser_release_port(struct uart_port *port)
{
	release_mem_region(port->iobase, TC3162_UART_SIZE);
}

static int tc3162ser_request_port(struct uart_port *port)
{
	return request_mem_region(port->iobase, TC3162_UART_SIZE,
	    "tc3162-uart2") != NULL ? 0 : -EBUSY; 
}


static struct uart_ops tc3162ser_ops = {
	.tx_empty =		tc3162ser_tx_empty,
	.set_mctrl =	tc3162ser_set_mctrl,
	.get_mctrl =	tc3162ser_get_mctrl,
	.stop_tx =		tc3162ser_stop_tx,
	.start_tx =		tc3162ser_start_tx,
	.stop_rx =		tc3162ser_stop_rx,
	.enable_ms =	tc3162ser_enable_ms,
	.break_ctl =	tc3162ser_break_ctl,
	.startup =		tc3162ser_startup,
	.shutdown =		tc3162ser_shutdown,
	.set_termios =	tc3162ser_set_termios,
	.type =			tc3162ser_type,
	.config_port =	tc3162ser_config_port,
	.release_port =	tc3162ser_release_port,
	.request_port =	tc3162ser_request_port,
};

#if 0
static void tc3162_console_put(const char c)
{
#ifdef TCSUPPORT_MT7510_E1
        unsigned int tmp;
	while (1){
		tmp = VPint(READ_OTHER(CR_UART2_IER));
		wmb();
		if((LSR_INDICATOR2 & LSR_THRE)){
			wmb();
			break;
		}
	}
	VPchar(CR_UART2_THR) = c;
	wmb();
#else
	while (!(LSR_INDICATOR2 & LSR_THRE))
		;
	VPchar(CR_UART2_THR) = c;
#endif
}

static void tc3162_console_write(struct console *con, const char *s,
    unsigned int count)
{
#if 0 // def CONFIG_TC3162_ADSL
	/*The prink message is hook this funcion.*/
	if((void *)send_uart_msg){
		send_uart_msg((char *)s, count);
	}
#endif
	while (count--) {
		if (*s == '\n')
			tc3162_console_put('\r');
		tc3162_console_put(*s);
		s++;
	}
}

static int tc3162_console_setup(struct console *con, char *options)
{
	return 0;
}

static struct uart_driver tc3162ser_reg;

static struct console tc3162_serconsole = {
	.name =		"ttyS2",
	.write =	tc3162_console_write,
	.device =	uart_console_device,
	.setup =	tc3162_console_setup,
	.flags =	CON_PRINTBUFFER,
	.cflag =	B115200 | CS8 | CREAD,
	.index =	-1,
	.data =		&tc3162ser_reg,
};

static int __init tc3162_console_init(void)
{
	register_console(&tc3162_serconsole);
	return 0;
}

console_initcall(tc3162_console_init);
#endif

static struct uart_port tc3162ser_ports[] = {
	{
        .iobase =	0xbfbf0303,
        .irq =		HSUART_INT,
		.uartclk =	115200,
		.fifosize =	1,
		.ops =		&tc3162ser_ops,
		.line =		0,
		.flags =	ASYNC_BOOT_AUTOCONF,
	}
};

static struct uart_driver tc3162ser_reg = {
	.owner	=		THIS_MODULE,
	.driver_name =	"ttyS2",
	.dev_name =		"ttyS2",
	.major =		TTY_MAJOR,
	.minor =		65,
	.nr =			1,
};

static int __init tc3162ser_init(void)
{
	int ret, i;

	/* UART2 Initial Start*/
	/*if(isEN751221)
	{
	*/
		unsigned long div_x = 0, div_y = 0;
		unsigned long word = 0;
		unsigned int reg = 0;

		reg = VPint(0xbfa20104); //GPIO_SHR_SCH
		reg |= (0x1<<18); //UART2_MODE
		reg &= ~((0x01 << 3) | (0x01 << 7)); /* Disable Lan0_LED GE_Led */
		VPint(0xbfa20104) = reg;

		/* Set FIFO control enable, reset RFIFO, TFIFO, 16550 mode, watermark=0x00 (1 byte) */
		tc_outb(CR_UART2_FCR, UART_FCR|UART_WATERMARK);

		/* Set modem control to 0 */
		tc_outb(CR_UART2_MCR, UART_MCR);

		/* Disable IRDA, Disable Power Saving Mode, RTS , CTS flow control */
		tc_outb(CR_UART2_MISCC, UART_MISCC);

		/* Set interrupt Enable to, enable Tx, Rx and Line status */
		tc_outb(CR_UART2_IER, UART_IER);

		/* access the bardrate divider */
		tc_outb(CR_UART2_LCR, UART_BRD_ACCESS);

		div_y = UART_XYD_Y;

		div_x = tc3162_get_uclk_20M(UART_BAUDRATE);

		word = (div_x<<16)|div_y;
		tc_outl(CR_UART2_XYD, word);

		/* Set Baud Rate Divisor to 1*16 */
		tc_outb(CR_UART2_BRDL, UART_BRDL_20M);
		tc_outb(CR_UART2_BRDH, UART_BRDH_20M);

		/* Set DLAB = 0, clength = 8, stop =1, no parity check */
		tc_outb(CR_UART2_LCR, UART_LCR);
	/*}*/
	/* UART2 Initial End */

	ret = uart_register_driver(&tc3162ser_reg);
	if (!ret) {
		for (i = 0; i < TC3162_NR_PORTS; i++)
			uart_add_one_port(&tc3162ser_reg, &tc3162ser_ports[i]);
	}

	return ret;
}

#if 0 // def CONFIG_TC3162_ADSL
/*_____________________________________________________________________________
**      function name: tcconsole_cmd
**      descriptions:
**             This function is used the send command to uart drivers that is used
**		tcconsole utility.
**
**      parameters:
**             cmd: 	Specify the command line strings.
**             len: 	The length of command.
**
**      global:
**             tc3162ser_ports
**
**      return:
**             none
**
**      call:
**      	tty_flip_buffer_push
**      	tty_insert_flip_char
**
**      revision:
**      1. Here 2010/9/23
**____________________________________________________________________________
*/
void 
tcconsole_cmd(char* cmd, int len){
	struct uart_port *port= &tc3162ser_ports[0];
	struct tty_struct *tty = port->state->port.tty;
	unsigned int ch, flg;
	int i;
    
	/*Ignore the line  feed character*/	
	for(i=0; i<len-1; i++){
		ch = cmd[i];
		port->icount.rx++;

		if (tty->low_latency)
			tty_flip_buffer_push(tty);

		flg = TTY_NORMAL;

		tty_insert_flip_char(tty, ch, flg);
	}
	tty_flip_buffer_push(tty);
}/*end tcconsole_cmd*/
EXPORT_SYMBOL(tcconsole_cmd);
#endif 

__initcall(tc3162ser_init);

