#ifndef __TFTP_H
#define	__TFTP_H

/*
 * Trivial File Transfer Protocol (IEN-133)
 */
#define	SEGSIZE		512		/* data segment size */

/*
 * Packet types.
 */
#define	RRQ	01				/* read request */
#define	WRQ	02				/* write request */
#define	DATA	03				/* data packet */
#define	ACK	04				/* acknowledgement */
#define	ERROR	05				/* error code */

struct	tftphdr {
	short	th_opcode;			/* packet type */
	union {
		unsigned short	tu_block;	/* block # */
		short	tu_code;		/* error code */
		char	tu_stuff[1];		/* request packet stuff */
	} __attribute__ ((__packed__)) th_u;
	char	th_data[0];			/* data or error string */
} __attribute__ ((__packed__));

#define	th_block	th_u.tu_block
#define	th_code		th_u.tu_code
#define	th_stuff	th_u.tu_stuff
#define	th_msg		th_data

/*
 * Error codes.
 */
#define	EUNDEF		0		/* not defined */
#define	ENOTFOUND	1		/* file not found */
#define	EACCESS		2		/* access violation */
#define	ENOSPACE	3		/* disk full or allocation exceeded */
#define	EBADOP		4		/* illegal TFTP operation */
#define	EBADID		5		/* unknown transfer ID */
#define	EEXISTS		6		/* file already exists */
#define	ENOUSER		7		/* no such user */




#ifdef BOOT_LZMA_SUPPORT
/*
 * Max length of filename
 * frankliao added 20100511
 */
#define FILE_NAME_LENGTH        128
#define MODE_NAME_LENGTH        12

/*
 * Base address of tftp buffer
 * frankliao added 20100511
 */
#define TFTP_BUF_BASE           0x80020000


/*
 * The name of tcboot
 * frankliao added 20100511
 */
#define TCBOOT_NAME             "tcboot.bin"


/*
 * The namd of tclinux kernel
 * frankliao added 20100511
 */
#define TCLINUX_NAME            "tclinux.bin"


/*
 * Write file type
 * frankliao added 20100511
 */
#define TCBOOT                  1
#define TCLINUX                 2
#define ERRFILE                -1

/*
 * frankliao added 20100511
 * checksum length (byte)
 */
#define CKSUM_LENGTH 4
#endif

int tftp_send_ack(struct tftphdr *tftp_hdr, int block);
int tftp_rcv_wrq(sk_buff *skb);
int tftp_rcv_data(sk_buff *skb);
int tftp_rcv_packet(sk_buff *skb);

#endif /* __TFTP_H */
