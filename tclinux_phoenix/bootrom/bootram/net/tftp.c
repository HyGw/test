#include "skbuff.h"
#include "eth.h"
#include "arp.h"
#include "ip.h"
#include "udp.h"
#include "tftp.h"
#include "tftpput.h"

#include "osdep.h"
#ifdef BOOT_LZMA_SUPPORT
/* frankliao added 20100511 */
#include "flashhal.h"       		/* for tftp_move_recdata() */
#include "../../../tools/trx/trx.h" /* for trx header size */
#include <linux/string.h>   		/* for tftp_rcv_wrq() */
#endif
#include "common.h"

unsigned long client_ip;
unsigned short client_port;
unsigned short client_block;

#ifdef BOOT_LZMA_SUPPORT
/* the name of the received data */
static char filename[ FILE_NAME_LENGTH ];
/* the size of the received data */
static int rcvdata_size = 0;
/* file type :  TCBOOT : tcboot.bin
 *              TCLINUX : tclinux.bin */
static short filetype = 0;

uint32_t crc32buf(char *buf, size_t len, int flags);
static int tftp_write_flash(void);
int checkfile(short filetype, int rcvdata_size, char *start);
#endif

#if defined(TCSUPPORT_CT_BOOTLOADER_UPGRADE)
int do_go(int argc, char *argv[]);
#endif
int tftp_send_ack(struct tftphdr *tftp_hdr, int block)
{
	struct tftphdr *tftp_ack;
	sk_buff *skb;

	skb = alloc_skb(ETH_FRAME_LEN);
	udp_skb_reserve(skb);
	tftp_ack = (struct tftphdr *)skb_put(skb, sizeof(struct tftphdr));
	tftp_ack->th_opcode = htons(ACK);
	tftp_ack->th_block = htons(block);

	udp_send(skb, client_ip, TFTP, client_port);

	return 0;
}

int tftp_rcv_wrq(sk_buff *skb)
{
	struct tftphdr *tftp_hdr;
#ifdef BOOT_LZMA_SUPPORT
	/*
	 * frankliao added 20100511
	 * the pointer is a temporary pointer for
	 * getting filename and mode from skb->data
	 */
    unsigned char* pointer;
#endif

	client_ip = ip_get_source_ip(skb);
	client_port = udp_get_source_port(skb);

	//prom_printf("client_ip=%X\n", client_ip);
	//prom_printf("client_port=%X\n", client_port);
#ifdef BOOT_LZMA_SUPPORT
	/*
	 * frankliao added 20100511
	 * skb->data started in opcode (16 bit),
	 * (skb->data + 2) is the address of filename
	 */
	pointer = skb->data + 2;
	/* get filename */
	strncpy(filename, pointer, sizeof (filename) - 1);

	/*
	 * frankliao added 20100511
	 * set filetype as a identifier when checking file
	 */
	if( !strcmp(filename, TCBOOT_NAME) )
		filetype = TCBOOT;
	else if( !strcmp(filename, TCLINUX_NAME) )
		filetype = TCLINUX;
#if defined(TCSUPPORT_FWC_ENV)
    else if( !strcmp(filename, FH_NW_PRODUCT_BOOT_NAME))
    {
        filetype = FH_NW_PRODUCT_BOOT_FLAG;
    }
    else if( !strcmp(filename, FH_NW_PRODUCT_ROOTFS_NAME))
    {
        filetype = FH_NW_PRODUCT_ROOTFS_FLAG;
    }
    else if( !strcmp(filename, FH_NW_PRODUCT_APP1_NAME))
    {
        filetype = FH_NW_PRODUCT_APP1_FLAG;
    }
    else if( !strcmp(filename, FH_NW_PRODUCT_APP2_NAME))
    {
        filetype = FH_NW_PRODUCT_APP2_FLAG;
    }
    else if( !strcmp(filename, FH_NW_PRODUCT_KERNEL_NAME))
    {
        filetype = FH_NW_PRODUCT_KERNEL_FLAG;
    }
    else if( !strcmp(filename, FH_NW_PRODUCT_CONF_NAME))
    {
        filetype = FH_NW_PRODUCT_CONF_FLAG;
    }
#endif
	else
		filetype = ERRFILE;
#endif
	tftp_hdr = (struct tftphdr *)skb->data;
	tftp_send_ack(tftp_hdr, 0);
	client_block = 1;

	tftp_put_begin();

	return 0;
}

int tftp_rcv_data(sk_buff *skb)
{
	struct tftphdr *tftp_hdr;
	int len;

	if (client_ip != ip_get_source_ip(skb))
		return -1;
	if (client_port != udp_get_source_port(skb))
		return -1;

	tftp_hdr = (struct tftphdr *)skb->data;
	if (client_block == ntohs(tftp_hdr->th_block)) {
		len = skb->len - sizeof(struct tftphdr);
		if (tftp_put(tftp_hdr->th_data, len))
			return -1;
#ifdef BOOT_LZMA_SUPPORT
		/* 
		 * frankliao added 20100511
		 * record received data size
		 */
		rcvdata_size += len;
#endif
		tftp_send_ack(tftp_hdr, client_block);
		client_block++;

		if (len < SEGSIZE) {
			tftp_put_end();
#ifdef BOOT_LZMA_SUPPORT
			/*
			 * frankliao added 20100511
			 * check received data by crc
			 */
			if( !checkfile(filetype, rcvdata_size, TFTP_BUF_BASE) )
				tftp_write_flash();
#endif				
        }

	} else if (client_block > ntohs(tftp_hdr->th_block)) {

		tftp_send_ack(tftp_hdr, ntohs(tftp_hdr->th_block));

	} else {

		tftp_send_ack(tftp_hdr, client_block);
	}

	return 0;
}

int tftp_rcv_packet(sk_buff *skb)
{
	struct tftphdr *tftp_hdr;
	
	tftp_hdr = (struct tftphdr *)skb->data;

	switch (ntohs(tftp_hdr->th_opcode)) {

	case RRQ:
		break;
	case WRQ:
		tftp_rcv_wrq(skb);
#ifdef BOOT_LZMA_SUPPORT		
	   /*
		* frankliao added 20100511
		* reset the variable as a counter for 
		* recording the received data size
		*/
		rcvdata_size = 0;
#endif		
		break;
	case DATA:
		tftp_rcv_data(skb);
		break;
	case ACK:
		break;
	case ERROR:
		break;
	default:
		break;
	}

	return 0;
}

#ifdef BOOT_LZMA_SUPPORT
/**************************************************************
 Function name: 
			checkfile
 Description:
			Use checksum to check if received data is valid
 Parameters: 
			filetype : TCBOOT : tcboot.bin
					   TCLINUX : tclinux.bin
			rcvdata_size : the size of checkfile
			start : the start address of checkfile
 Return value: 
			0: success
			others: fail

 frankliao added 20100511
***************************************************************/
int checkfile(short filetype, int rcvdata_size, char *start)
{
	unsigned int checksum;
	unsigned char * crc;
	unsigned int checkdata_size;

	prom_printf("Received file: %s\n", filename);
	prom_printf("rcvdata_size = %d\r\n", rcvdata_size);
	prom_printf("start = 0x%X\r\n", (unsigned long)start);

#if defined(TCSUPPORT_FWC_ENV)
	prom_printf("filetype = %d\r\n", filetype);
    if ((FH_NW_PRODUCT_BOOT_FLAG <= filetype) && (FH_NW_PRODUCT_CONF_FLAG >=  filetype))
    {
    	prom_printf("the file name is ok , prepare to upload\n...\n");
    	return 0;
    }
#endif
	
	/* check tcboot.bin */
	if (filetype == TCBOOT) {
		if ((rcvdata_size > TCBOOT_SIZE) || (rcvdata_size < 0)) {
			prom_printf("File %s: format error\n", filename);
			return -1;
		}
		crc = (unsigned char*)(start + rcvdata_size - CKSUM_LENGTH);
		checksum = (*crc)<<24 | (*(crc+1))<<16 | (*(crc+2))<<8 | (*(crc+3))<<0;
		checkdata_size = rcvdata_size - CKSUM_LENGTH;
	/* check tclinux.bin */
	} else if (filetype == TCLINUX) {
		if (rcvdata_size <= TCBOOT_SIZE) {
			prom_printf("File %s: format error\n", filename);
			return -1;
		}
		crc = &(((struct trx_header*)start)->crc32);
		checksum = (*crc)<<24 | (*(crc+1))<<16 | (*(crc+2))<<8 | (*(crc+3))<<0;
		checkdata_size = rcvdata_size - sizeof(struct trx_header);
		start += sizeof(struct trx_header);
	/* error file handle */
	} else {
		prom_printf("Invalid filename, upload failed\n\n");
		prom_printf("Valid filename: Bootloader  : tcboot.bin\
				   \n                Linux Kernel: tclinux.bin\n...\n");

		return -1;
	}

	/* frankliao modify 20100805 */
	prom_printf("Real crc code: %X\n", checksum);
	if (checksum == crc32buf(start, checkdata_size, 1)) {
		
		prom_printf("Check data success, prepare to upload\n...\n");
		return 0;
	} else {
		prom_printf("Check data fail, upload failed\n...\n");
		return -1;
	}

}


/********************************************
 Function name: 
			tftp_write_flash
 Description: 
			write received data to suitable address in flash
 Parameters: 
			None
 Return value:    
			0: success
	   others: fail

 frankliao added 20100511
 *******************************************/

static int tftp_write_flash(void)
{
	unsigned long retlen;
	unsigned char upgrade_flag = 0;
	
#if defined(TCSUPPORT_IS_FH_PON) || defined(TCSUPPORT_FWC_ENV)
	switch(filetype)
	{
#if defined(TCSUPPORT_FWC_ENV)
        case FH_NW_PRODUCT_BOOT_FLAG:  /*boot*/
        {
            prom_printf("boot Write to flash from 0x%X with 0x%X bytes\n",TCBOOT_BASE, rcvdata_size);
            flash_erase(TCBOOT_BASE, FH_NW_PRODUCT_BOOT_SIZE);
            prom_printf("\n");

			flash_write(TCBOOT_BASE, (unsigned long)rcvdata_size, &retlen,
						(const unsigned char*)TFTP_BUF_BASE);  
            upgrade_flag = 1;
            break;
        }
        case FH_NW_PRODUCT_KERNEL_FLAG:  /*uImage*/
        {
            prom_printf("KernelA Write to flash from 0x%X with 0x%X bytes\n", FH_NW_PRODUCT_KERNELA_ADDR, rcvdata_size);
            flash_erase((unsigned long)FH_NW_PRODUCT_KERNELA_ADDR, FH_NW_PRODUCT_KERNEL_SIZE);
            prom_printf("\n");
            flash_write((unsigned long)FH_NW_PRODUCT_KERNELA_ADDR, (unsigned long)rcvdata_size, &retlen,
            			(const unsigned char*)TFTP_BUF_BASE);
            prom_printf("\n");

            prom_printf("KernelB Write to flash from 0x%X with 0x%X bytes\n", FH_NW_PRODUCT_KERNELB_ADDR, rcvdata_size);
            flash_erase((unsigned long)FH_NW_PRODUCT_KERNELB_ADDR, FH_NW_PRODUCT_KERNEL_SIZE);
            prom_printf("\n");
            flash_write((unsigned long)FH_NW_PRODUCT_KERNELB_ADDR, (unsigned long)rcvdata_size, &retlen,
            			(const unsigned char*)TFTP_BUF_BASE);
            prom_printf("\n");  
            upgrade_flag = 1;
            break;
        }
        case FH_NW_PRODUCT_ROOTFS_FLAG:  /*rootfs*/
        {
            prom_printf("RootfsA Write to flash from 0x%X with 0x%X bytes\n", FH_NW_PRODUCT_ROOTFSA_ADDR, rcvdata_size);
            flash_erase((unsigned long)FH_NW_PRODUCT_ROOTFSA_ADDR, FH_NW_PRODUCT_ROOTFS_SIZE );
            prom_printf("\n");
            flash_write((unsigned long)FH_NW_PRODUCT_ROOTFSA_ADDR, (unsigned long)rcvdata_size, &retlen,
            			(const unsigned char*)TFTP_BUF_BASE);
            prom_printf("\n");

            prom_printf("RootfsB Write to flash from 0x%X with 0x%X bytes\n", FH_NW_PRODUCT_ROOTFSB_ADDR, rcvdata_size);
            flash_erase((unsigned long)FH_NW_PRODUCT_ROOTFSB_ADDR, FH_NW_PRODUCT_ROOTFS_SIZE );
            prom_printf("\n");
            flash_write((unsigned long)FH_NW_PRODUCT_ROOTFSB_ADDR, (unsigned long)rcvdata_size, &retlen,
            			(const unsigned char*)TFTP_BUF_BASE);
            prom_printf("\n");    
            upgrade_flag = 1;
            break;
        }
		case FH_NW_PRODUCT_APP1_FLAG:  /*aImage0*/
        {
            prom_printf("app1A Write to flash from 0x%X with 0x%X bytes\n", FH_NW_PRODUCT_APP1A_ADDR, rcvdata_size);
            flash_erase((unsigned long)FH_NW_PRODUCT_APP1A_ADDR, FH_NW_PRODUCT_APP1_SIZE );
            prom_printf("\n");
            flash_write((unsigned long)FH_NW_PRODUCT_APP1A_ADDR, (unsigned long)rcvdata_size, &retlen,
            			(const unsigned char*)TFTP_BUF_BASE);
            prom_printf("\n");

            prom_printf("app1B Write to flash from 0x%X with 0x%X bytes\n",FH_NW_PRODUCT_APP1B_ADDR, rcvdata_size);
            flash_erase((unsigned long)FH_NW_PRODUCT_APP1B_ADDR, FH_NW_PRODUCT_APP1_SIZE );
            prom_printf("\n");
            flash_write((unsigned long)FH_NW_PRODUCT_APP1B_ADDR, (unsigned long)rcvdata_size, &retlen,
            			(const unsigned char*)TFTP_BUF_BASE);
            prom_printf("\n");    
            upgrade_flag = 1;
            break;
        }
		case FH_NW_PRODUCT_APP2_FLAG:  /*aImage1*/
		{
			prom_printf("app2A Write to flash from 0x%X with 0x%X bytes\n", FH_NW_PRODUCT_APP2A_ADDR, rcvdata_size);
			flash_erase((unsigned long)FH_NW_PRODUCT_APP2A_ADDR, FH_NW_PRODUCT_APP2_SIZE );
			prom_printf("\n");
			flash_write((unsigned long)FH_NW_PRODUCT_APP2A_ADDR, (unsigned long)rcvdata_size, &retlen,
						(const unsigned char*)TFTP_BUF_BASE);
			prom_printf("\n");

			prom_printf("app2B Write to flash from 0x%X with 0x%X bytes\n", FH_NW_PRODUCT_APP2B_ADDR, rcvdata_size);
			flash_erase((unsigned long)FH_NW_PRODUCT_APP2B_ADDR, FH_NW_PRODUCT_APP2_SIZE );
			prom_printf("\n");
			flash_write((unsigned long)FH_NW_PRODUCT_APP2B_ADDR, (unsigned long)rcvdata_size, &retlen,
						(const unsigned char*)TFTP_BUF_BASE);
			prom_printf("\n");    
			upgrade_flag = 1;
			break;
		}
		case FH_NW_PRODUCT_CONF_FLAG:  /*fhcfg*/
		{
			prom_printf("Configuration Write to flash from 0x%X with %X bytes\n", FH_NW_PRODUCT_CONFIGURATION_ADDR, rcvdata_size);
			flash_erase((unsigned long)FH_NW_PRODUCT_CONFIGURATION_ADDR, FH_NW_PRODUCT_CONFIGURATION_SIZE );
			prom_printf("\n");
			flash_write((unsigned long)FH_NW_PRODUCT_CONFIGURATION_ADDR, (unsigned long)rcvdata_size, &retlen,
						(const unsigned char*)TFTP_BUF_BASE);
			prom_printf("\n");
			
			upgrade_flag = 1;
			break;
		}
#endif
        default :
		{
			upgrade_flag = 0;
            break;
		}
	}
#endif
	
	/* move tcboot.bin to the base address of bootloader */
	if (filetype == TCBOOT) {

		prom_printf("Write to flash from 0x80020000 to 0x0 with %X bytes\n", rcvdata_size);
		flash_erase(TCBOOT_BASE, rcvdata_size);
		prom_printf("\n");
		flash_write(TCBOOT_BASE, (unsigned long)rcvdata_size, &retlen,
									(const unsigned char*)TFTP_BUF_BASE);
#if defined(TCSUPPORT_CT_BOOTLOADER_UPGRADE)
		do_go(0,NULL);
#endif

	/* move tclinux.bin to the base address of kernel */
	} else if (filetype == TCLINUX) {

#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
        prom_printf("Write to flash from 0x80020000 to 0x280000 with %X bytes\n", rcvdata_size);
        flash_erase(0x280000, rcvdata_size );
        prom_printf("\n");
        flash_write(0x280000, (unsigned long)rcvdata_size, &retlen,
					(const unsigned char*)TFTP_BUF_BASE);
#elif defined(TCSUPPORT_NAND_RT63368)

		prom_printf("Write to flash from 0x80020000 to 0x40000 with %X bytes\n", rcvdata_size);
		flash_erase(0x40000, rcvdata_size );
		prom_printf("\n");
		flash_write(0x40000, (unsigned long)rcvdata_size, &retlen,
					(const unsigned char*)TFTP_BUF_BASE);

#else
		prom_printf("Write to flash from 0x80020000 to 0x20000 with %X bytes\n", rcvdata_size);
		flash_erase((unsigned long)flash_tclinux_start, rcvdata_size );
		prom_printf("\n");
		flash_write((unsigned long)flash_tclinux_start, (unsigned long)rcvdata_size, &retlen,
					(const unsigned char*)TFTP_BUF_BASE);
#endif

#if defined(TCSUPPORT_CT_BOOTLOADER_UPGRADE)
		do_go(0,NULL);
#endif

	} else {
#if !defined(TCSUPPORT_IS_FH_PON)
#if defined(TCSUPPORT_FWC_ENV)
        if(0 == upgrade_flag)
#endif
        {
		    prom_printf("Error file, cannot write into flash\n");
            return -1;
        }
#endif
	}

	return 0;
}
#endif

