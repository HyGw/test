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
#ifdef TCSUPPORT_UART1_ENHANCE
#include <linux/proc_fs.h>
#include <linux/kthread.h>
#include <asm/errno.h>
#include <linux/random.h>
#endif
#include <asm/tc3162/tc3162.h>

#define TC3162_NR_PORTS				1

#define TC3162_UART_SIZE			0x30

#define PORT_TC3162					3162
#ifdef CONFIG_TC3162_ADSL
void (*send_uart_msg)(char* msg, int len);
EXPORT_SYMBOL(send_uart_msg);
static char tuart_buf[1024];
#endif

#ifdef TCSUPPORT_UART1_ENHANCE
#define ENABLE 							(1)
#define DISABLE 						(0)
#define SUCCESS							(0)
#define FALSE							(1)

#define PRINT_BUFFER_INIT_LEN_KB		(4 << 10)

static int flush_ring_buffer(void);

static unsigned char			isRingBufferMode = DISABLE;
static wait_queue_head_t		printQueue;
static struct task_struct		*printTask;
static int						printFlag;
static volatile struct circ_buf	ringBuffer;
static int						ringBufferDbg = DISABLE;
static spinlock_t				flushLock;
#endif

#ifdef TCSUPPORT_MT7510_E1
#define READ_OTHER(x) ((x & 0xc) + 0xbfb003a0)
#endif
static void tc3162ser_stop_tx(struct uart_port *port)
{
#ifdef TCSUPPORT_MT7510_E1
	unsigned int tmp;
	tmp = VPint(READ_OTHER(CR_UART_IER));
	wmb();
	VPchar(CR_UART_IER) &= ~IER_THRE_INTERRUPT_ENABLE;	
	wmb();
#else
	VPchar(CR_UART_IER) &= ~IER_THRE_INTERRUPT_ENABLE;	
#endif
}

static void tc3162ser_irq_rx(struct uart_port *port)
{
	struct tty_struct *tty = port->state->port.tty;
	unsigned int ch, flg;

#ifdef TCSUPPORT_MT7510_E1
	unsigned int tmp;

	while (1) {
		tmp = VPint(READ_OTHER(CR_UART_LSR));
		wmb();
		if(!(LSR_INDICATOR & LSR_RECEIVED_DATA_READY)){
			wmb();
			break;
		}
#else
	while (LSR_INDICATOR & LSR_RECEIVED_DATA_READY) {
#endif
		/* 
		 * We need to read rds before reading the 
		 * character from the fifo
		 */
#ifdef TCSUPPORT_MT7510_E1
		tmp = VPint(READ_OTHER(CR_UART_RBR));
		wmb();
		ch = VPchar(CR_UART_RBR);
		wmb();
#else
		ch = VPchar(CR_UART_RBR);
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

#ifdef CONFIG_TC3162_ADSL
	int len=0;
	memset(tuart_buf, 0, sizeof(tuart_buf));
#endif
	if (port->x_char) {
		VPchar(CR_UART_THR) = port->x_char;
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
		VPchar(CR_UART_THR) = 
		    xmit->buf[xmit->tail];
#ifdef TCSUPPORT_MT7510_E1
		wmb();
#endif
#ifdef CONFIG_TC3162_ADSL
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
#ifdef CONFIG_TC3162_ADSL
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
	tmp = VPint(READ_OTHER(CR_UART_IIR));
	wmb();
	iir = IIR_INDICATOR;
	wmb();
#else
	iir = IIR_INDICATOR;
#endif
	if (((iir & IIR_RECEIVED_DATA_AVAILABLE) == IIR_RECEIVED_DATA_AVAILABLE) ||
		((iir & IIR_RECEIVER_IDLE_TRIGGER) == IIR_RECEIVER_IDLE_TRIGGER)) 
		tc3162ser_irq_rx(port);
	if ((iir & IIR_TRANSMITTED_REGISTER_EMPTY) == IIR_TRANSMITTED_REGISTER_EMPTY) 
		tc3162ser_irq_tx(port);

	return IRQ_HANDLED;
}

static unsigned int tc3162ser_tx_empty(struct uart_port *port)
{
#ifdef TCSUPPORT_MT7510_E1
	unsigned int tmp;
        tmp = VPint(READ_OTHER(CR_UART_IIR));
	wmb();
#endif
	unsigned int ret;

        ret = (LSR_INDICATOR & LSR_THRE) ? TIOCSER_TEMT : 0;
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
        tmp = VPint(READ_OTHER(CR_UART_IER));
	wmb();
	VPchar(CR_UART_IER) |= IER_THRE_INTERRUPT_ENABLE;
	wmb();
#else
	VPchar(CR_UART_IER) |= IER_THRE_INTERRUPT_ENABLE;	
#endif
}

static void tc3162ser_stop_rx(struct uart_port *port)
{
#ifdef TCSUPPORT_MT7510_E1
        unsigned int tmp;
        tmp = VPint(READ_OTHER(CR_UART_IER));
	wmb();
	VPchar(CR_UART_IER) |= IER_THRE_INTERRUPT_ENABLE;
	wmb();
#else
	VPchar(CR_UART_IER) &= ~IER_RECEIVED_DATA_INTERRUPT_ENABLE;	
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
	do_IRQ(UART_INT);
}	
#endif

static int tc3162ser_startup(struct uart_port *port)
{
	int ret;

#ifdef CONFIG_MIPS_TC3262
	if (cpu_has_vint)
		set_vi_handler(port->irq, tc3162ser_irq_dispatch);
#endif
	ret = request_irq(port->irq, tc3162ser_irq, 0, "TC3162 UART", port);
	if (ret) {
		printk(KERN_ERR "Couldn't get irq %d ret=%d\n", port->irq, ret);
		return ret;
	}
#ifdef TCSUPPORT_MT7510_E1
	unsigned int tmp;
	tmp = VPint(READ_OTHER(CR_UART_IER));
	wmb();
	VPchar(CR_UART_IER) |= IER_RECEIVED_DATA_INTERRUPT_ENABLE;
	wmb();
#else
	VPchar(CR_UART_IER) |= IER_RECEIVED_DATA_INTERRUPT_ENABLE;	
#endif
	return 0;
}

static void tc3162ser_shutdown(struct uart_port *port)
{
#ifdef TCSUPPORT_MT7510_E1
        unsigned int tmp;
        tmp = VPint(READ_OTHER(CR_UART_IER));
	wmb();
	VPchar(CR_UART_IER) &= ~IER_RECEIVED_DATA_INTERRUPT_ENABLE;
	wmb();
#else
	VPchar(CR_UART_IER) &= ~IER_RECEIVED_DATA_INTERRUPT_ENABLE;	
#endif
	free_irq(port->irq, port);
}

static void tc3162ser_set_termios(struct uart_port *port,
    struct ktermios *termios, struct ktermios *old)
{
	unsigned int baud, quot;
	unsigned long flags;

	termios->c_cflag |= CREAD;

	baud = 115200;
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
	    "tc3162-uart") != NULL ? 0 : -EBUSY; 
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

static void tc3162_console_put(const char c)
{
#ifdef TCSUPPORT_MT7510_E1
        unsigned int tmp;
	while (1){
		tmp = VPint(READ_OTHER(CR_UART_IER));
		wmb();
		if((LSR_INDICATOR & LSR_THRE)){
			wmb();
			break;
		}
	}
	VPchar(CR_UART_THR) = c;
	wmb();
#else
	while (!(LSR_INDICATOR & LSR_THRE))
		;
	VPchar(CR_UART_THR) = c;
#endif
}

static void tc3162_console_write(struct console *con, const char *s,
    unsigned int count)
{
#ifdef CONFIG_TC3162_ADSL
	/*The prink message is hook this funcion.*/
	if((void *)send_uart_msg){
		send_uart_msg((char *)s, count);
	}
#endif
#ifdef TCSUPPORT_UART1_ENHANCE
	/* before enter tc3162_console_write function, kernel has 
	  * call spin_lock_irqsave() for prevent printk.
	  */
	if(isRingBufferMode == ENABLE) {
		uart_print_task_wakeup(s, count);
	} 
	else 
#endif
	{		
		while (count--) {
			if (*s == '\n')
				tc3162_console_put('\r');
			tc3162_console_put(*s);
			s++;
		}
	}
}

static int tc3162_console_setup(struct console *con, char *options)
{
	return 0;
}

static struct uart_driver tc3162ser_reg;

static struct console tc3162_serconsole = {
	.name =		"ttyS",
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

static struct uart_port tc3162ser_ports[] = {
	{
		.iobase =	0xbfbf0003,
		.irq =		UART_INT,
		.uartclk =	115200,
		.fifosize =	1,
		.ops =		&tc3162ser_ops,
		.line =		0,
		.flags =	ASYNC_BOOT_AUTOCONF,
	}
};

static struct uart_driver tc3162ser_reg = {
	.owner	=		THIS_MODULE,
	.driver_name =	"ttyS",
	.dev_name =		"ttyS",
	.major =		TTY_MAJOR,
	.minor =		64,
	.nr =			1,
};

#ifdef TCSUPPORT_UART1_ENHANCE
/* The print message uses circular buffer.
 * A 'head' index - the point at which the producer insert items into the
 *                  buffer. 
 * A 'tail' index - the point at which the consumer finds the next item in
 *                  the buffer.
 *
 * example:
 * array[2:5] = "abc\n"
 *  -------------------------------
 * | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
 *  --------------------------------
 * | 0 | 1 | a | b | c | \n | 6 | 7 |
 *  --------------------------------
 *              ¡ô                    ¡ô
 *              tail                head
 *
 * The tail pointer is equal to the head pointer, the buffer is empty.
 * The buffer is full when the head pointer is one less than the tail pointer.
 * The head index is incremented when items are added, and the tail index
 * when items are removed.
 * The tail index should never jump the head index, and both indices should
 * wrapped to 0 when they reach the end of the buffer.
 */

/******************************************************************************
 Descriptor:	It's used to get message length in ring buffer.
 Input Args:	none.
 Ret Value:	Message length in ring buffer.
******************************************************************************/
static int get_ring_buffer_cnt(void)
{
	return CIRC_CNT(ringBuffer.head, ringBuffer.tail, PRINT_BUFFER_INIT_LEN_KB);
}

/******************************************************************************
 Descriptor:	It's used to get free space in ring buffer.
 Input Args:	none.
 Ret Value:	Free space in ring buffer.
******************************************************************************/
static int get_ring_buffer_space(void)
{
	return CIRC_SPACE(ringBuffer.head, ringBuffer.tail, PRINT_BUFFER_INIT_LEN_KB);
}

/******************************************************************************
 Descriptor:	It's used to add string to ring buffer, if the ring buffer
                has enough free space. On the contrary, if the ring buffer has
                no enough free space, it will show error message to console.
 Input Args:	str: The string pointer.
                str_len: The string length
 Ret Value:	none.
******************************************************************************/
static void add_msg_to_ring_buffer(char *str, int str_len)
{
	int offset, start, pos;
	int buf_space = 0;
	char msg[64] = {0};

	if(ringBufferDbg == ENABLE) {
		buf_space = get_ring_buffer_space();
		snprintf(msg, sizeof(msg), "before add(%d) buffer space:%d\n", str_len, buf_space);
		console_put_string(msg);
	}

	if(str_len > get_ring_buffer_space()) {
		/* Has no enough ring buffer space to stroe message */
		flush_ring_buffer();
		if(str_len > get_ring_buffer_space()) {
			/* print insufficient ring buffer message */
			console_put_string("\n\ninsufficient ring buffer.\n\n");
			return;
		}
	}

	/* store log message to buffer */
	start = ringBuffer.head;
	for(offset = 0; offset < str_len; offset++) {
		/* pos = (start + offset) % PRINT_BUFFER_INIT_LEN_KB */
		pos = (start + offset) & (PRINT_BUFFER_INIT_LEN_KB - 1);
		ringBuffer.buf[pos] = str[offset];
	}

	/* shift head pointer */
	ringBuffer.head = ((ringBuffer.head + str_len) & (PRINT_BUFFER_INIT_LEN_KB - 1));

	if(ringBufferDbg == ENABLE) {
		buf_space = get_ring_buffer_space();
		snprintf(msg, sizeof(msg), "after add(%d) buffer space:%d\n", str_len, buf_space);
		console_put_string(msg);
	}
}

/******************************************************************************
 Descriptor:	It's used to print string to console(UART1).
 Input Args:	str: The string pointer.
 Ret Value:	none.
******************************************************************************/
void console_put_string(const char *str)
{
	while(*str) {
		if (*str == '\n')
			tc3162_console_put('\r');
		tc3162_console_put(*str);
		str++;
	}
}

/******************************************************************************
 Descriptor:	It's used to flush data from ring buffer to console(UART1).
 Input Args:	none.
 Ret Value:	SUCCESS: flush ring buffer to console success.
                      FALSE: flush ring buffer to console failed.
******************************************************************************/
static int flush_ring_buffer(void)
{
	int offset, start, pos;
	int ring_buffer_cnt;
	int buf_space = 0;
	char msg[64] = {0};
	unsigned long flags ;

	spin_lock_irqsave(&flushLock, flags) ;

	if(ringBufferDbg == ENABLE) {
		buf_space = get_ring_buffer_space();
		snprintf(msg, sizeof(msg), "before flush buffer space:%d\n", buf_space);
		console_put_string(msg);
	}

	ring_buffer_cnt = get_ring_buffer_cnt();

	if(ring_buffer_cnt == 0) {
		spin_unlock_irqrestore(&flushLock, flags) ;
		return SUCCESS;
	} else if(ring_buffer_cnt < 0) {
		console_put_string("ring buffer count error.\n");
		spin_unlock_irqrestore(&flushLock, flags) ;
		return FALSE;
	}

	start = ringBuffer.tail;
	for(offset = 0; offset < ring_buffer_cnt; offset++) {
		/* pos = (start + offset) % PRINT_BUFFER_INIT_LEN_KB */
		pos = (start + offset) & (PRINT_BUFFER_INIT_LEN_KB - 1);
		if (ringBuffer.buf[pos] == '\n')
			tc3162_console_put('\r');
		tc3162_console_put(ringBuffer.buf[pos]);

		/* shift tail pointer */
		ringBuffer.tail = ((ringBuffer.tail++) & (PRINT_BUFFER_INIT_LEN_KB - 1));

		if(isRingBufferMode == DISABLE) {
			spin_unlock_irqrestore(&flushLock, flags) ;
			return FALSE;
		}
	}

	if(ringBufferDbg == ENABLE) {
		buf_space = get_ring_buffer_space();
		snprintf(msg, sizeof(msg), "after flush buffer space:%d\n", buf_space);
		console_put_string(msg);
	}

	spin_unlock_irqrestore(&flushLock, flags) ;

	return SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to flush data from ring buffer to console(UART1).
 Input Args:	data: Never used.
 Ret Value:	    0: Success.
******************************************************************************/
static int uart_print_task_wait(void *data)
{
	while(!kthread_should_stop()) {	
		wait_event_interruptible(printQueue, (printFlag == ENABLE || kthread_should_stop()));
		/* clear condition */
		printFlag = DISABLE;

		/* print debug msg */
		if(flush_ring_buffer() == FALSE) {
			/* reset head and tail pointer */
			ringBuffer.head = 0;
			ringBuffer.tail = 0;
		}
	}

	return 0;
}

/******************************************************************************
 Descriptor:	It's used to insert string to ring buffer.
 Input Args:	data: Never used.
 Ret Value:	    0: Success.
******************************************************************************/
int uart_print_task_wakeup(const char *str, unsigned int str_len)
{
	/* store log message to buffer */
	add_msg_to_ring_buffer(str, str_len);

	/* wake up task */
	printFlag = ENABLE;
	wake_up_interruptible(&printQueue);
	
	return 0;
}

/******************************************************************************
 Descriptor:	It's used to enable or disable ring buffer mode with flush
                ring buffer.
 Input Args:	enable: Enable or disable ring buffer mode.
 			flush_buffer: Enable or disable flush ring buffer when disable
 				              ring buffer mode.
 Ret Value:	none.
******************************************************************************/
void set_ring_buffer_mode(int enable)
{
	int errno;
	char errMsg[64] = {0};
	
	if(enable == DISABLE) {
		/* kthread_stop() will call might_sleep(), so it can't be called in isr.
		 */
		if(!in_interrupt()) {
			if(printTask) {
				kthread_stop(printTask);
				printTask = NULL;
			}

			if(ringBuffer.buf) {
				kfree(ringBuffer.buf);
			}
		} else {
			/* In this isr situation, .
			 */
			flush_ring_buffer();
		}

		isRingBufferMode = DISABLE;

		/* reset head and tail pointer */
		ringBuffer.head = 0;
		ringBuffer.tail = 0;
	} else {
		if(isRingBufferMode == DISABLE) {
			if(printTask == NULL) {
				ringBuffer.buf = (char *) kmalloc(PRINT_BUFFER_INIT_LEN_KB, GFP_ATOMIC);
				if (ringBuffer.buf == NULL) {
					console_put_string("UART1 kmalloc ring buffer failed\n");
				}

				printTask = kthread_run(uart_print_task_wait, NULL, "uart_print_task");
				if(IS_ERR(printTask)) {
					errno = PTR_ERR(printTask);
					printTask = NULL;
					
					if(ringBuffer.buf) {
						kfree(ringBuffer.buf);
					}

					snprintf(errMsg, sizeof(errMsg), "uart create kernel thread for ring buffer error:%d.\n", errno);
					console_put_string(errMsg);
				} else {
					isRingBufferMode = ENABLE;
				}
			}
		} else {
			isRingBufferMode = ENABLE;
		}
	}
}

/******************************************************************************
 Descriptor:	It's used to disable ring buffer mode with flush ring buffer.
 Input Args:	none.
 Ret Value:	none.
******************************************************************************/
void disable_ring_buffer_mode()
{
	set_ring_buffer_mode(DISABLE);
}
EXPORT_SYMBOL(disable_ring_buffer_mode);

/******************************************************************************
 Descriptor:	It's used to test ring buffer mode. This function will printk a message with random
                 length.
                 Tester uses console information to check whether the
                 ring buffer mode correct or not.
 Input Args:	times: The times of printk.
 Ret Value:	none.
******************************************************************************/
static void random_printk(int times)
{
	/* printk buffer size are 1024  */
	const int printk_size = 1024;
#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 9
	char msg_pattern[1023] = {0};
	char msg_printk[1023] = {0};
#else
	char msg_pattern[printk_size - 1] = {0};
	char msg_printk[printk_size - 1] = {0};
#endif
	int printk_len;
	char msg_printk_len[32] = {0};
	const int ascii_start = 0x21;
	const int ascii_end = 0x7E;
	const int ascii_range = ascii_end - ascii_start + 1;
	int i;

	if(isRingBufferMode == DISABLE) {
		console_put_string("ring buffer mode is disabled.");
		return;
	}

	/* init msg */
	for(i = 0; i < sizeof(msg_pattern); i++) {
		/* assign ASCII from 0x21(!) to 0x7E(~) */
		msg_pattern[i] = (i % ascii_range) + ascii_start;
	}

	while(times--) {
		get_random_bytes(&printk_len, sizeof(printk_len)) ;
		/* minus '\n' length */
		printk_len = (printk_len & (printk_size - 1 - strlen("\n")));

		/* show to console for check printk length */
		/* plus "\r\n" length */
		snprintf(msg_printk_len, sizeof(msg_printk_len), "times:%d printk len:%d\n", (times + 1), printk_len + strlen("\r\n"));
		console_put_string(msg_printk_len);

		/* print to console */
		memcpy(msg_printk, msg_pattern, printk_len);
		msg_printk[printk_len] = '\n';
		msg_printk[printk_len + 1] = '\0';
		printk(msg_printk);

		/* When UART baudrate are 115200,
		 * the costs of printk to console is about 1ms with 12 characters.
		 */
		msleep((printk_size / 12) + 1);
	}
}

static int uart_dbg_msg_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
	int index = 0;

	index += sprintf(buf + index, "isRingBufferMode     : %s\n", ((isRingBufferMode == ENABLE) ? "enable" : "disable"));
	index += sprintf(buf + index, "ring buffer mode size: %dKB\n", (PRINT_BUFFER_INIT_LEN_KB >> 10));
	index += sprintf(buf + index, "ringBufferDbg        : %s\n", ((ringBufferDbg == ENABLE) ? "enable" : "disable"));

	index += sprintf(buf + index, "\nusage:\n");
	index += sprintf(buf + index, "ring_buffer_mode <0:disable, 1:enable>\n");
	index += sprintf(buf + index, "busy_loop_test\n");
	index += sprintf(buf + index, "crash_test\n");
	index += sprintf(buf + index, "ring_buffer_dbg <0:disable, 1:enable>\n");
	index += sprintf(buf + index, "ring_buffer_test <times>\n");
	index += sprintf(buf + index, "ring_buffer_stop_test\n");

	*eof = 1;

	return index;
}

static int uart_dbg_msg_write_proc(struct file *file, const char *buffer, unsigned long count, void *data)
{
	char val_string[64], cmd[32];
	int value, ret ;
	
	if (count > sizeof(val_string) - 1)
		return -EINVAL ;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT ;

	sscanf(val_string, "%s %d", cmd, &value) ;

	if(!strcmp(cmd, "ring_buffer_mode")) {
		if(value == 0) {
			set_ring_buffer_mode(DISABLE);
		} else {
			set_ring_buffer_mode(ENABLE);
		}
		printk("isRingBufferMode:%s\n", ((isRingBufferMode == ENABLE) ? "enable" : "disable"));
	} else if(!strcmp(cmd, "busy_loop_test")) {
		/* use for test printk after occuring watchdog interrupt */
		printk("before busy loop test.\n");
		while(1);
	} else if(!strcmp(cmd, "crash_test")) {
		printk("before crash test.\n");
		/* use for test printk when read error register crash */
		regRead32(0x1234567);
		printk("finished crash test.\n");
	} else if(!strcmp(cmd, "ring_buffer_dbg")) {
		if(value == 0) {
			ringBufferDbg = DISABLE;
		} else {
			ringBufferDbg = ENABLE;
		}
		printk("ringBufferDbg:%s\n", ((ringBufferDbg == ENABLE) ? "enable" : "disable"));
	} else if(!strcmp(cmd, "ring_buffer_test")) {
		random_printk(value);
	} else if(!strcmp(cmd, "ring_buffer_stop_test")) {
		printk("before ring buffer stop.\n");
		set_ring_buffer_mode(DISABLE);
		printk("finished ring buffer stop.\n");
	}
	
	return count ;
}

/******************************************************************************
 Descriptor:	It's used to deinit ring buffer.
 Input Args:	none.
 Ret Value:	none.
******************************************************************************/
void uart_print_buffer_deinit(void)
{
	remove_proc_entry("uart/uart", 0);
	remove_proc_entry("uart", 0);

	if(printTask) {
		kthread_stop(printTask);
	}

	if(ringBuffer.buf) {
		kfree(ringBuffer.buf);
	}
}
#endif

static int __init tc3162ser_init(void)
{
	int ret, i;
#ifdef TCSUPPORT_UART1_ENHANCE
	struct proc_dir_entry *uart_proc = NULL;
	struct proc_dir_entry *uart_proc_dir = NULL;
#endif

	ret = uart_register_driver(&tc3162ser_reg);
	if (!ret) {
		for (i = 0; i < TC3162_NR_PORTS; i++)
			uart_add_one_port(&tc3162ser_reg, &tc3162ser_ports[i]);
	}

#ifdef TCSUPPORT_UART1_ENHANCE
	/* init */
	init_waitqueue_head(&printQueue);
	spin_lock_init(&flushLock);
	printFlag = DISABLE;
	ringBuffer.head = 0;
	ringBuffer.tail = 0;
	
	/* Initial proc file node */
	uart_proc_dir = proc_mkdir("uart", NULL);

	if(uart_proc_dir) {
		uart_proc = create_proc_entry("uart", 0, uart_proc_dir) ;
		if(uart_proc) {
			uart_proc->read_proc = uart_dbg_msg_read_proc ;
			uart_proc->write_proc = uart_dbg_msg_write_proc ;
		}
	}

	console_put_string("ECNT ring buffer init success\n");
#endif

	return ret;
}

#ifdef CONFIG_TC3162_ADSL
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

