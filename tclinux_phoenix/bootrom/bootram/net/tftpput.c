#include "tftpput.h"

#define printf	prom_printf

#define TFTP_BUF_BASE	(0x80020000)
//#define TFTP_BUF_LIMIT	(0x500000)

extern unsigned long tftp_mode;
extern unsigned long tftp_param;
int tftp_break;
extern int tftp_datalen;
extern unsigned long calculate_dram_size(void);

static unsigned char *buf;
static int data_len;
static int put_begin = 0;
static unsigned long limit_size;

int tftp_put_begin(void)
{
	put_begin = 1;
	
	printf("\r\nStarting the TFTP download...\r\n");
	limit_size = ((calculate_dram_size()<<19)); //half dram size
	//printf("limit size %x\n",(caculate_dram_size()<<19));

	buf = (unsigned char *)TFTP_BUF_BASE;
	data_len = 0;

	return 0;
}

int tftp_put(unsigned char *data, int len)
{
	static int count = 0;

	if (!put_begin)
		return -1;

	if (count == 0)
		printf(".");
	count += len;
	if (count >= 32 * 1024)
		count = 0;

	/*add support for size bigger than 4M. shnwind*/
//	if ((unsigned long)(data_len + len) > TFTP_BUF_LIMIT) {	
	if ((unsigned long)(data_len + len) > limit_size) {
		printf("\r\nTFTP buffer overflow, stop!\r\n");
		tftp_break = 1;
		return -1;
	}
	memcpy(buf + data_len, data, len);
	data_len += len;

	return 0;
}

int tftp_put_end(void)
{
	if (!put_begin)
		return -1;
	
	printf("\r\n");
	printf("Total %d (0x%X) bytes received\r\n", data_len, data_len);
	put_begin = 0;
	tftp_break = 1;

	return 0;
}
