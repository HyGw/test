#include <asm/asm.h>
#include <asm/regdef.h>
#include <asm/mipsregs.h>   

#define SECOND_BOOT_IN_SRAM_ADDR		0xBfa30800
#define BOOTROM_START_ADDR				0xBFA30040
#define BOOTROM_END_ADDR				0xBFA3C000
#define SRAM_START_ADDR					0xBFA30000

#define SET_MIPS3() .##set mips3
#define SET_MIPS0() .##set mips0
#define SET_PUSH()  .##set push
#define SET_POP()   .##set pop
	    
	    	    
	    
#define	OP_CSH			0x00
#define	OP_CSL			0x01
#define	OP_CK			0x02
#define	OP_OUTS			0x08
#define	OP_OUTD			0x09
#define	OP_OUTQ			0x0A
#define	OP_INS			0x0C
#define	OP_INS0			0x0D
#define	OP_IND			0x0E
#define	OP_INQ			0x0F
#define	OP_OS2IS		0x10
#define	OP_OS2ID		0x11
#define	OP_OS2IQ		0x12

#define	OP_OD2IS		0x13
#define	OP_OD2ID		0x14
#define	OP_OD2IQ		0x15
#define	OP_OQ2IS		0x16
#define	OP_OQ2ID		0x17
#define	OP_OQ2IQ		0x18
#define	OP_OSNIS		0x19
#define	OP_ODNID		0x1A

#define SF_OP_4BYTES_RD_DATA			0x13	/* Read Data in 4Bytes Address */
#define SF_OP_RD_DATA				0x03		/* Read Data */
#define SF_OP_RD_PAGE				0x13		/* Read Page into Cache (Only for NAND) */
#define SF_OP_GET_FEATURE			0x0F		/* Get FEATURE (Only for NAND) */
#define SF_COMMAND_STATUS			0xC0		/* Status register (Only for NAND) */
#define SF_STATUS_OIP				0x1			/* OIP (Operation in Processing (Only for NAND) */

#define SF_BASE						0xBFA10000
#define SF_READ_MODE     			(SF_BASE + 0x0000)
#define SF_READ_IDLE_EN     		(SF_BASE + 0x0004)
#define SF_SIDLY		     		(SF_BASE + 0x0008)
#define SF_CSHEXT					(SF_BASE + 0x000C)
#define SF_CSLEXT					(SF_BASE + 0x0010)
#define SF_MTX_MODE_TOG				(SF_BASE + 0x0014)
#define SF_RDCTL_FSM				(SF_BASE + 0x0018)
#define SF_MACMUX_SEL				(SF_BASE + 0x001C)
#define SF_MANUAL_EN				(SF_BASE + 0x0020)
#define SF_MANUAL_OPFIFO_EMPTY		(SF_BASE + 0x0024)
#define SF_MANUAL_OPFIFO_WDATA		(SF_BASE + 0x0028)
#define SF_MANUAL_OPFIFO_FULL		(SF_BASE + 0x002C)
#define SF_MANUAL_OPFIFO_WR			(SF_BASE + 0x0030)
#define SF_MANUAL_DFIFO_FULL		(SF_BASE + 0x0034)
#define SF_MANUAL_DFIFO_WDATA		(SF_BASE + 0x0038)
#define SF_MANUAL_DFIFO_EMPTY		(SF_BASE + 0x003C)
#define SF_MANUAL_DFIFO_RD			(SF_BASE + 0x0040)
#define SF_MANUAL_DFIFO_RDATA		(SF_BASE + 0x0044)
#define SF_DUMMY					(SF_BASE + 0x0080)
#define SF_ADDR_3B4B				(SF_BASE + 0x0084)
#define SF_PROBE_SEL				(SF_BASE + 0x0088)
#define SF_CFG3B4B_EN				(SF_BASE + 0x008C)
#define SF_INTERRUPT				(SF_BASE + 0x0090)
#define SF_INTERRUPT_EN				(SF_BASE + 0x0094)
#define SF_SI_CK_SEL				(SF_BASE + 0x009C)
#define SF_STRAP			(SF_BASE + 0x0114)

#define SF_CLK_CHANGE		0xBFA20068

#define	SF_RD_MAX		(0x100)

#define	DDATA_ENABLE	(0x1)
#define	DDATA_DISABLE	(0x0)
#define DDATA_MASK		(0xff)

#define OP_SHIFT			(0x9)
#define	OP_REPEAT_ONE		(0x1)
#define	OP_REPEAT_TWO		(0x2)
#define	OP_REPEAT_THREE		(0x3)
#define	OP_REPEAT_FOUR		(0x4)
#define	OP_REPEAT_FIVE		(0x5)

#define	OP_ENABLE		(0x1)
#define	OP_DISABLE		(0x0)
#define OP_CMD_MASK		(0x1f)
#define OP_LEN_MASK		(0x1ff)
#define	SF_RD_MAX		(0x100)//(0x1ff)
#define	SF_PP_MAX		(0x100)	    

//#define DEBUG 1


#ifdef DEBUG
                li t1, 'm'
                jal disp_character2
                nop
                

                li t1, 'a'
                jal disp_character2
                nop      

                li t1, 'q'
                jal disp_character2
                nop                               
#endif


Init_SFC:
#if 0
		/* set SFC Clock to 25MHZ  */
		la	t1, SF_CLK_CHANGE
		li	t2, 0x6
		sw	t2, 0(t1)
		nop
#endif		

		/* set Tn to 1	*/
		la	t1,	SF_SI_CK_SEL
		li	t2,	0x9
		sw	t2,	0(t1)
		nop


/*Change to manual mode */

		/*Disable read idle enable */
		la t1, SF_READ_IDLE_EN
		li t2, 0x0

		sw t2, 0(t1)

fsm_idle:		
		/*  wait until auto read status is IDLE - start */
		la  t1, SF_RDCTL_FSM
		lw  t7, 0(t1)
		li  t2, 0x0
		bne t7, t2, fsm_idle
	    /*  wait until auto read status is IDLE - end */

#ifdef DEBUG
        li t1, '1'
        jal disp_character2
        nop 
#endif        

sf_mtx:
		/* manual begin */
		la t1, SF_MTX_MODE_TOG
		li t2, 0x9
		sw t2, 0(t1)		/* 0xBFA10014 = 0x9 */
		nop

#ifdef DEBUG
        li t1, '2'
        jal disp_character2
        nop
#endif        

macmux_sel:		
		la t1, SF_MACMUX_SEL	
		li t2, 0x1
		sw t2, 0(t1)		/* 0xBFA1001C = 0x1 */
		nop
		
#ifdef DEBUG
        li t1, '3'
        jal disp_character2
        nop 		
#endif        

		la t1, SF_MANUAL_EN
		li t2, 0x1
		sw t2, 0(t1)		/* 0xBFA10020 = 0x1 */

#ifdef DEBUG
        li 	t1, '4'
        jal disp_character2
        nop 		
#endif            


GET_INFO:
		la		t1, SF_STRAP		/* 0xBFA10114 */
		lw		t4, 0(t1)
		andi		t4, t4, 0x2		/* Store Information=> 0: SPI NOR , 1: SPI NAND */

		lw		t8, 0(t1)
		andi		t8, t8, 0x1		/* Store Information=> 0: 3Byte Mode , 1: 4Byte Mode */

		lw		t0, 0(t1)
		andi		t0, t0, 0x4		/* Store Information=> 0: Dummy byte prepend, 1: Dummy byte append */		

START_LOAD:		
		beqz		t4, READ_DATA		/* 0: SPI NOR , 1: SPI NAND */
		nop

LOAD_PAGE:
		bgt		s1, s2, LOAD_DONE	/*If S1 > S2, goto LOAD_DONE*/
		
		/* jal		delay_period  */
		nop
		/* 1. CSL : Send Chip Select Low command */
		li		t5, OP_CSL
		li		t7, OP_REPEAT_ONE
		jal		send_opfifo_write_cmd
		nop		

		/* 2. Send 13h OP code */
		li		t5, OP_OUTS
		li		t7, OP_REPEAT_ONE
		jal		send_opfifo_write_cmd	
		nop

		li		t5, SF_OP_RD_PAGE	/* 13H op code (Means load page in SPI NAND) */
		jal		send_datafifo_write_cmd
		nop 

PAGE_NUMBER:
		/* 3. Caculate the Page number */
		li		t5, 0xbfc00000
		subu 		t6, s1, t5			/* must sub the base address */
		srl		t6, t6, 11			/* Page size = 2KB = 2^(11) bytes */


		/* 4. Send Page number with 13H */
		li		t5, OP_OUTS
		li		t7, OP_REPEAT_THREE
		jal		send_opfifo_write_cmd
		nop		

		srl		t5, t6, 16
		andi 		t5, t5, 0xff
		jal		send_datafifo_write_cmd
		nop

		srl		t5, t6, 8
		andi 		t5, t5, 0xff
		jal		send_datafifo_write_cmd
		nop

		andi 		t5, t6, 0xff
		jal		send_datafifo_write_cmd
		nop
		
		/* 5. CSH : Send Chip Select High command */
		li		t5, OP_CSH
		li		t7, OP_REPEAT_ONE
		jal		send_opfifo_write_cmd
		nop       

		li		t5, OP_CK
		li		t7, OP_REPEAT_FIVE
		jal		send_opfifo_write_cmd
		nop
		
GET_FEATURE:
		/* 1. CSL : Send Chip Select Low command */
		li		t5, OP_CSL
		li		t7, OP_REPEAT_ONE
		jal		send_opfifo_write_cmd
		nop		

		/* 2. Send 0Fh OP code */
		li		t5, OP_OUTS
		li		t7, OP_REPEAT_ONE
		jal		send_opfifo_write_cmd	
		nop

		li		t5, SF_OP_GET_FEATURE	/* 0FH op code (Status register in SPI NAND) */
		jal		send_datafifo_write_cmd
		nop 

		/* 3. Send C0h command address */
		li		t5, OP_OUTS
		li		t7, OP_REPEAT_ONE
		jal		send_opfifo_write_cmd	
		nop

		li		t5, SF_COMMAND_STATUS	/* 0FH op code (Status register in SPI NAND) */
		jal		send_datafifo_write_cmd
		nop
		
		/* 4. Read Status */
		li		t5, OP_INS
		li		t7, 0x1
		jal		send_opfifo_write_cmd
		nop

		jal		send_datafifo_read_cmd
		nop
		
		andi	t9, t5, SF_STATUS_OIP
		
		/* 5. CSH : Send Chip Select High command */
		li		t5, OP_CSH
		li		t7, OP_REPEAT_ONE
		jal		send_opfifo_write_cmd
		nop       

		li		t5, OP_CK
		li		t7, OP_REPEAT_FIVE
		jal		send_opfifo_write_cmd
		nop						
										
		bnez	t9, GET_FEATURE						
		

READ_DATA:
READ_FROM_CACHE:
		bgt		s1, s2, LOAD_DONE	/*If S1 > S2, goto LOAD_DONE*/
		nop
		/* jal		delay_period  */
		nop
		/* 1. CSL : Send Chip Select Low command */
		li		t5, OP_CSL
		li		t7, OP_REPEAT_ONE
		jal		send_opfifo_write_cmd
		nop

		/* 2. Caculate the address */
		li		t5, 0xbfc00000
		subu 		t9, s1, t5			/* must sub the base address */
		beqz		t4, ADDR_OPCODE			/* 0: SPI NOR , 1: SPI NAND */
		nop		
		andi		t9, t9, 0x7FF			/* SPI NAND address format(just page offset) */
		
ADDR_OPCODE:				
		/* 1. Send OP Code */
		li		t5, OP_OUTS
		li		t7, OP_REPEAT_ONE
		jal		send_opfifo_write_cmd
		nop


		bnez		t8, FOUR_BYTE_OP			/* 0: 3Byte Mode , 1: 4Byte Mode */		
		nop
	        /* write op_cmd 0x03 */
		li		t5, SF_OP_RD_DATA		/* 03H */
		jal		send_datafifo_write_cmd
		nop
		j		SEND_ADDR
		

FOUR_BYTE_OP:
	        /* write op_cmd 0x03 */
		li		t5, SF_OP_RD_DATA	/* 03H */
		jal		send_datafifo_write_cmd
		nop

SEND_ADDR:
		/* 2. Send Read Address */
		beqz		t8, THREE_BYTE_ADDR			/* 0: 3Byte Mode , 1: 4Byte Mode */
		
		li		t5, OP_OUTS
		li		t7, OP_REPEAT_ONE
		jal		send_opfifo_write_cmd
		nop		

		srl		t5, t9, 24
		andi 		t5, t5, 0xff
		jal		send_datafifo_write_cmd
		nop		

THREE_BYTE_ADDR:		
		
		li		t5, OP_OUTS
		li		t7, OP_REPEAT_THREE
		jal		send_opfifo_write_cmd
		nop		

		bnez		t0, DUMMY_APPEND		/* 0: Dummy byte prepend, 1: Dummy byte append */
		srl		t5, t9, 16
		andi 		t5, t5, 0xff
		jal		send_datafifo_write_cmd		

DUMMY_APPEND:

		srl		t5, t9, 8
		andi 		t5, t5, 0xff
		jal		send_datafifo_write_cmd
		nop

		andi 		t5, t9, 0xff
		jal		send_datafifo_write_cmd
		nop


		beqz		t0, read_one_byte		/* 0: Dummy byte prepend, 1: Dummy byte append */
		li	 	t5, 0x00			/* Dummy byte */
		jal		send_datafifo_write_cmd
		nop			

read_one_byte:
		bgt		s1, s2, LOAD_DONE	/*If S1 > S2, goto LOAD_DONE*/
		li		t5, OP_INS
		li		t7, 0x1
		jal		send_opfifo_write_cmd
		nop

		jal		send_datafifo_read_cmd
		nop

		sb		t5, 0(s3)
		nop

		addiu 		s1, s1, 1
		addiu 		s3, s3, 1
		addiu		t9, t9, 1

		andi		t5, t9, 0x3FF
		li		t2, 0x0
		bne		t5, t2, read_one_byte
		nop
		beq	  	t5, t2, READ_PAGE_DONE				
		nop
		
READ_PAGE_DONE:	

		/* Send Chip Select High command */
		li		t5, OP_CSH
		li		t7, OP_REPEAT_ONE
		jal		send_opfifo_write_cmd
		nop       

		li		t5, OP_CK
		li		t7, OP_REPEAT_FIVE
		jal		send_opfifo_write_cmd
		nop  

		j		START_LOAD

LOAD_DONE:
		/* Send Chip Select High command */
		li		t5, OP_CSH
		li		t7, OP_REPEAT_ONE
		jal		send_opfifo_write_cmd
		nop       

		li		t5, OP_CK
		li		t7, OP_REPEAT_FIVE
		jal		send_opfifo_write_cmd
		nop  

		jr	s4
		nop

#ifdef DEBUG

        li 		t1, '3'
        jal 	disp_character2
        nop
#endif        
        
        
#if 0    
manual_to_auto:        
		/* manual to auto */
		la t1, SF_MTX_MODE_TOG
		li t2, 0x0
		sw t2, 0(t1)
		nop
		
		la t1, SF_MANUAL_EN
		li t2, 0x0
		sw t2, 0(t1)		
		nop
		
		/*Enable read idle enable */
		la t1, SF_READ_IDLE_EN
		li t2, 0x1
		sw t2, 0(t1)
		nop
#endif




/************************************************************************************/
/*                                                                                  */
/*  void send_datafifo_read_cmd(unsigned char *data)              					*/
/*  data = t5				                                                        */
/*                                                                  				*/
/************************************************************************************/
LEAF(send_datafifo_read_cmd)

dfifo_nonempty_check:
				/* wait until dfifo is not empty */
				la 		t1, SF_MANUAL_DFIFO_EMPTY
				lw 		t2, 0(t1)
				li 		t3, 0x1
				beq		t2, t3, dfifo_nonempty_check
				nop				

				/* read 1 byte from dfifo to register DFIFO_RDATA */	
				la		t1, SF_MANUAL_DFIFO_RDATA
				lw	 	t5, 0(t1)
				andi 	t5, t5, 0xff				
				nop
				
				/* enable register DFIFO_RD to read next byte */
				la		t1, SF_MANUAL_DFIFO_RD
				li		t2, DDATA_ENABLE
				sw		t2, 0(t1)
				nop				


				jr	ra
				nop

END(send_datafifo_read_cmd)


/************************************************************************************/
/*                                                                                  */
/*  void send_opfifo_write_cmd(unsigned int op_cmd, unsigned int op_len)             */
/*  op_cmd = t5, op_len = t7                                                        */
/*                                                                  */
/************************************************************************************/
LEAF(send_opfifo_write_cmd)

				/* (((op_cmd) & OP_CMD_MASK) << OP_SHIFT) | ((op_len) & OP_LEN_MASK) */
				/* write op_cmd to register OPFIFO_WDATA */
				la		t1, SF_MANUAL_OPFIFO_WDATA
				andi		t2, t5, OP_CMD_MASK		/* OP_CMD_MASK = 0x1f */
				nop
				sll		t2, t2, OP_SHIFT		/* OP_SHIFT = 0x9 */
				nop
				andi		t3, t7, OP_LEN_MASK		/* OP_LEN_MASK =0x1ff */
				or		t2, t2, t3				
				nop
				sw		t2, 0(t1)
				nop
				

opfifo_full_check:				
				/* wait until opfifo is not full */
				la		t1, SF_MANUAL_OPFIFO_FULL	/* 0xBFA1002C */
				lw		t2, 0(t1)
				nop
				li		t3, 0x0
				bne		t2, t3, opfifo_full_check
				nop

				/* enable write from register OPFIFO_WDATA to opfifo */
				la		t1, SF_MANUAL_OPFIFO_WR		/* 0xBFA10030 */
				li		t2, OP_ENABLE
				sw		t2, 0(t1)
				nop

opfifo_empty_check:				
				/* wait until opfifo is empty */
				la		t1, SF_MANUAL_OPFIFO_EMPTY
				lw		t2, 0(t1)
				nop
				li		t3, 0x0
				beq		t2, t3, opfifo_empty_check
				nop

				jr ra
				nop
END(send_opfifo_write_cmd)




/************************************************************************************/
/*                                                                                  */
/*  void send_datafifo_write_cmd(unsigned int data)             					*/
/*  data = t5				                                                        */
/*                                                                  				*/
/************************************************************************************/
LEAF(send_datafifo_write_cmd)

dfifo_full_check:
				/* wait until dfifo is not full */
				la 		t1, SF_MANUAL_DFIFO_FULL
				lw 		t2, 0(t1)
				li 		t3, 0x1
				beq		t2, t3, dfifo_full_check
				nop

				/* write data  to register DFIFO_WDATA */
				la		t1, SF_MANUAL_DFIFO_WDATA
				andi 		t2, t5, DDATA_MASK
				sw		t2, 0(t1)
				nop

				jr	ra
				nop

END(send_datafifo_write_cmd)


#if 0
LEAF(delay_period)
				li t5, 0x20000
delay:
				sub t5, t5, 1
				nop
				bnez t5, delay		
				nop						

				jr	ra
				nop

END(delay_period)
#endif


#ifdef DEBUG
/************************************************************************************/
/*                                                                                  */
/*  void disp_character2(unsigned char t1)              							*/
/*  data = t1				                                                        */
/*                                                                  				*/
/************************************************************************************/
LEAF(disp_character2)

                la              k0,     0xbfbf0000
wait:
                lb              t2, 0x17(k0)
                nop
                andi            t2, 0x20
                beqz    		t2, wait
                nop

                sb              t1, 0x03(k0)

                jr    ra
                nop
          
END(disp_character2)
#endif      
