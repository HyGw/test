/*
 * SPIFLASH support for TC3162
 */

/*
 * SPI Flash Memory support header file.
 *
 * Copyright (c) 2005, Atheros Communications Inc.
 * Copyright (C) 2006 FON Technology, SL.
 * Copyright (C) 2006 Imre Kaloz <kaloz@openwrt.org>
 *
 * This code is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#ifndef __NEWSPIFLASH_H
#define __NEWSPIFLASH_H

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
#define SF_CLK_CHANGE				0xBFA20068

#define SF_READ_MODE_VALUE				VPint(SF_READ_MODE)
#define SF_READ_IDLE_EN_VALUE     		VPint(SF_READ_IDLE_EN)
#define SF_SIDLY_VALUE		     		VPint(SF_SIDLY)
#define SF_CSHEXT_VALUE					VPint(SF_CSHEXT)
#define SF_CSLEXT_VALUE					VPint(SF_CSLEXT)
#define SF_MTX_MODE_TOG_VALUE			VPint(SF_MTX_MODE_TOG)
#define SF_RDCTL_FSM_VALUE				VPint(SF_RDCTL_FSM)
#define SF_MACMUX_SEL_VALUE				VPint(SF_MACMUX_SEL)
#define SF_MANUAL_EN_VALUE				VPint(SF_MANUAL_EN)
#define SF_MANUAL_OPFIFO_EMPTY_VALUE	VPint(SF_MANUAL_OPFIFO_EMPTY)
#define SF_MANUAL_OPFIFO_WDATA_VALUE	VPint(SF_MANUAL_OPFIFO_WDATA)
#define SF_MANUAL_OPFIFO_FULL_VALUE		VPint(SF_MANUAL_OPFIFO_FULL)
#define SF_MANUAL_OPFIFO_WR_VALUE		VPint(SF_MANUAL_OPFIFO_WR)
#define SF_MANUAL_DFIFO_FULL_VALUE		VPint(SF_MANUAL_DFIFO_FULL)
#define SF_MANUAL_DFIFO_WDATA_VALUE		VPint(SF_MANUAL_DFIFO_WDATA)
#define SF_MANUAL_DFIFO_EMPTY_VALUE		VPint(SF_MANUAL_DFIFO_EMPTY)
#define SF_MANUAL_DFIFO_RD_VALUE		VPint(SF_MANUAL_DFIFO_RD)
#define SF_MANUAL_DFIFO_RDATA_VALUE		VPint(SF_MANUAL_DFIFO_RDATA)
#define SF_DUMMY_VALUE					VPint(SF_DUMMY)
#define SF_ADDR_3B4B_VALUE				VPint(SF_ADDR_3B4B)
#define SF_PROBE_SEL_VALUE				VPint(SF_PROBE_SEL)
#define SF_CFG3B4B_EN_VALUE				VPint(SF_CFG3B4B_EN)
#define SF_INTERRUPT_VALUE				VPint(SF_INTERRUPT)
#define SF_INTERRUPT_EN_VALUE			VPint(SF_INTERRUPT_EN)
#define SF_SI_CK_SEL_VALUE				VPint(SF_SI_CK_SEL)
#define SF_CLK_CHANGE_VALUE				VPint(SF_CLK_CHANGE)

#define SF_OP_WR_ENABLE					0x06		/* Write Enable */
#define SF_OP_WR_DISABLE				0x04		/* Write Disable */
#define SF_OP_RD_STATUS1				0x05		/* Read Status 1 */
#define SF_OP_WR_STATUS1				0x01		/* Write Status 1 */
#define SF_OP_RD_STATUS2				0x35		/* Read Status 2 */
#define SF_OP_WR_STATUS2				0x31		/* Write Status 2 */
#define SF_OP_RD_STATUS3				0x15		/* Read Status 3 */
#define SF_OP_WR_STATUS3				0x11		/* Write Status 3 */
#define SF_OP_RD_STATUSEX				0xC8		/* Read extend Status */  // only for W25Q256 & N25Q256
#define SF_OP_WR_STATUSEX				0xC5		/* Write extend Status */  // only for W25Q256 & N25Q256
#define SF_OP_RD_BANKREG				0x16		/* Read Bank Register */  // only for S25FL256S
#define SF_OP_WR_BANKREG				0x17		/* Write Bank Register */  // only for S25FL256S
#define SF_OP_RD_ENHANCED_CFGREG		0x65		/* Read Enhanced Register */  // only for N25Q256
#define SF_OP_WR_ENHANCED_CFGREG		0x61		/* Write Enhanced Register */  // only for N25Q256
#define SF_OP_RD_VOLATILE_CFGREG		0x85		/* Read Volatile Register */  // only for N25Q256
#define SF_OP_WR_VOLATILE_CFGREG		0x81		/* Write Volatile Register */  // only for N25Q256
#define SF_OP_RD_NONVOLATILE_CFGREG		0xB5		/* Read Non-Volatile Register */  // only for N25Q256
#define SF_OP_WR_NONVOLATILE_CFGREG		0xB1		/* Write Non-Volatile Register */  // only for N25Q256
#define SF_OP_RD_DATA					0x03		/* Read Data */
#define SF_OP_FAST_RD_DATA				0x0B		/* Fast Read Data */
#define SF_OP_FAST_RD_DUAL_OUT			0x3B		/* Fast Read Data Dual Output */
#define SF_OP_FAST_RD_DUAL_IO			0xBB		/* Fast Read Data Dual I/O */
#define SF_OP_PAGE_PROGRAM				0x02		/* Page Program */
#define SF_OP_SECTOR_ERASE				0xD8		/* Sector Erase */
#define SF_OP_BULK_ERASE				0xC7		/* Bulk Erase */
#define SF_OP_4BYTES_RD_DATA			0x13		/* Read Data in 4Bytes Address */
#define SF_OP_4BYTES_FAST_RD_DATA		0x0C		/* Fast Read Data in 4Bytes Address */
#define SF_OP_4BYTES_FAST_RD_DUAL_OUT	0x3C		/* Fast Read Data Dual Output in 4Bytes Address */
#define SF_OP_4BYTES_FAST_RD_DUAL_IO	0xBC		/* Fast Read Data Dual I/O in 4Bytes Address */
#define SF_OP_4BYTES_PAGE_PROGRAM		0x12		/* Page Program in 4Bytes Address  */
#define SF_OP_4BYTES_SECTOR_ERASE		0xDC		/* Sector Erase in 4Bytes Address */
#define SF_OP_ENTER_4BYTES_MODE			0xB7		/* enter 4bytes address mode */
#define SF_OP_EXIT_4BYTES_MODE			0xE9		/* exit 4bytes address mode */
#define SF_OP_POWER_DOWN				0xB9		/* Power Down to reduce power  */
#define SF_OP_RELEASE_POWER_DOWN		0xAB		/* release power down */
#define SF_OP_RD_ID						0x9F		/* Read JEDEC ID */


#define SF_STATUS_WIP       			0x01       	/* Write-In-Progress */
#define SF_STATUS_WEL       			0x02       	/* Write Enable Latch */
#define SF_STATUS_BP0       			0x04       	/* Block Protect 0 */
#define SF_STATUS_BP1       			0x08       	/* Block Protect 1 */
#define SF_STATUS_BP2       			0x10       	/* Block Protect 2 */
#define SF_STATUS_SRWD      			0x80       	/* Status Register Write Disable */

#define SF_STATUS_QE      		 		0x02       	/* Quad Enable */


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

#define OP_SHIFT			(9)
#define	OP_REPEAT_ONE		(1)
#define	OP_REPEAT_TWO		(2)
#define	OP_REPEAT_THREE		(3)
#define	OP_REPEAT_FOUR		(4)
#define	OP_REPEAT_FIVE		(5)

#define	OP_ENABLE		(1)
#define	OP_DISABLE		(0)
#define OP_CMD_MASK		(0x1f)
#define OP_LEN_MASK		(0x1ff)
#define	SF_RD_MAX		(0x100)//(0x1ff)
#define	SF_PP_MAX		(0x100)

#define	DDATA_ENABLE	(1)
#define	DDATA_DISABLE	(0)
#define DDATA_MASK		(0xff)

#define SI_SEL_MASK		(0x7)

#define DUMMY_DATA		0x0

#define AUTO_MTXMODE		0x0
#define AUTO_MANUALEN		0x0
#define MANUAL_MTXMODE 		0x9
#define MANUAL_MANUALEN		0x1
#define MANUAL_MACMUXSEL	0x1

#define	READ_DATA			0x0
#define	FAST_READ			0x1
#define	FAST_READ_DUALOUT	0x2
#define	FAST_READ_DUALIO	0x3

#define CFG3B4B_DIS		(0)
#define CFG3B4B_EN		(1)
#define AUTO_3BMODE		(0)
#define AUTO_4BMODE		(1)
#define RD_IDLE_DIS		(0)
#define RD_IDLE_EN		(1)
#define	CUR_AUTO_MODE	(0)
#define	CUR_MANUAL_MODE	(1)


#define EXTSYNC_RFIFO_UNF_INTR_EN		(0x100)
#define EXTSYNC_RFIFO_OVF_INTR_EN		(0x80)
#define EXTSYNC_TPFIFO_UNF_INTR_EN		(0x40)
#define EXTSYNC_TPFIFO_OVF_INTR_EN		(0x20)
#define MANUAL_DFIFO_UNF_INTR_EN			(0x10)
#define MANUAL_DFIFO_OVF_INTR_EN			(0x8)
#define MANUAL_OPFIFO_UNF_INTR_EN			(0x4)
#define MANUAL_OPFIFO_OVF_INTR_EN			(0x2)
#define AUTO_MANUAL_INTR_EN				(0x1)

#define AUTO_READ					(0)
#define AUTO_FAST_READ				(1)
#define AUTO_FAST_READ_DUALOUT		(2)
#define AUTO_FAST_READ_DUALIO		(3)

#define SF_SR1_DEFAULT		0x0
#define SF_SR2_DEFAULT		0x0
#define SF_BANKREG_3B		0x0
#define SF_BANKREG_4B		0x80

#define	WriteReg(reg, data)			(VPint(reg) = data)
#define	ReadReg(reg)				(VPint(reg))
#define	bReadReg(reg, mask)			(VPint(reg) & mask)

#define wLoWord(data)		((data) & 0xffff)
#define wHiWord(data)		(((data)>>16) & 0xffff)
#define bLoByte(data)		((data) & 0xff)
#define bHiByte(data)		(((data)>>8) & 0xff)

// Send change sf clk to 25MHZ command
#define SEND_CLK_25MHZ_CMD                				\
		{												\
         /* change sf clk to 25MHZ */						\
           WriteReg(SF_CLK_CHANGE, 6); 					\
		}

// Send change sf clk to 50MHZ command
#define SEND_CLK_50MHZ_CMD                				\
		{												\
         /* change sf clk to 50MHZ */						\
           WriteReg(SF_CLK_CHANGE, 3); 					\
		}

// Send opfifo write command
#define SEND_OPFIFO_WRITE_CMD(op_cmd, op_len)                														 \
        {                                                   														 \
       	 /* write op_cmd to register OPFIFO_WDATA */																	 \
          WriteReg(SF_MANUAL_OPFIFO_WDATA, ((((op_cmd) & OP_CMD_MASK) << OP_SHIFT) | ((op_len) & OP_LEN_MASK)));  	 \
         /* wait until opfifo is not full */																				 \
          while(ReadReg(SF_MANUAL_OPFIFO_FULL)) ; 																		 \
      	 /* enable write from register OPFIFO_WDATA to opfifo */														 \
          WriteReg(SF_MANUAL_OPFIFO_WR, OP_ENABLE);       			 													 \
		/* wait until opfifo is empty */																					 \
          while(!ReadReg(SF_MANUAL_OPFIFO_EMPTY));																		 \
        }

// Send dfifo write command
// Parameters:
//		data:	type is char
#define SEND_DFIFO_WRITE_CMD(data)                					 \
        {                                                   		 \
         /* wait until dfifo is not full */								 \
          while(ReadReg(SF_MANUAL_DFIFO_FULL)) ; 					 \
       	 /* write data  to register DFIFO_WDATA */					 \
          WriteReg(SF_MANUAL_DFIFO_WDATA, ((data) & DDATA_MASK));  	 \
        }

// Send dfifo read command
// Parameters:
//		data:	type is char
#define SEND_DFIFO_READ_CMD(data)                					 \
        {                                                   		 \
         /* wait until dfifo is not empty */								 \
          while(ReadReg(SF_MANUAL_DFIFO_EMPTY)) ; 					 \
       	 /* read from dfifo to register DFIFO_RDATA */					 \
          (data) = ReadReg(SF_MANUAL_DFIFO_RDATA);	  				 \
       	 /* enable register DFIFO_RD to read next byte */				 \
          WriteReg(SF_MANUAL_DFIFO_RD, DDATA_ENABLE);	  			 \
        }

// Send manual mode begin command
#define SEND_MANUAL_BEGIN_CMD									\
        {                                                  		\
        	sf_manual_begin();									\
        }

// Send manual mode end command
#define SEND_MANUAL_END_CMD										\
        {                                       	   			\
        	sf_manual_end();   	   								\
        }

// Send manual mode begin command
#define SEND_MANUAL_WREN_BEGIN_CMD								\
        {                                                  		\
        	sf_manual_wren_begin();								\
        }

// Send manual mode end command
#define SEND_MANUAL_WREN_END_CMD								\
        {                                       	   			\
        	sf_manual_wren_end();   	   						\
        }

// Send CSL command
#define SEND_CSL_CMD											\
        {                                                  		\
          SEND_OPFIFO_WRITE_CMD(OP_CSL, OP_REPEAT_ONE)	 		\
          SEND_OPFIFO_WRITE_CMD(OP_CSL, OP_REPEAT_ONE)	 		\
        }

// Send CSH command
#define SEND_CSH_CMD											\
        {                                       	   			\
          SEND_OPFIFO_WRITE_CMD(OP_CSH, OP_REPEAT_ONE)			\
          SEND_OPFIFO_WRITE_CMD(OP_CK, OP_REPEAT_FIVE)			\
        }

// Send write enable command
#define SEND_WREN_CMD											\
        {														\
          send_single_opcode_cmd(SF_OP_WR_ENABLE);				\
        }

// Send write disable command
#define SEND_WRDIS_CMD											\
        {														\
          SEND_MANUAL_BEGIN_CMD									\
          send_single_opcode_cmd(SF_OP_WR_DISABLE);				\
          SEND_MANUAL_END_CMD									\
        }

// Send read register internal command 
// Parameters:
//		data:	type is char
#define SEND_READ_REGISTER_INTERNAL_CMD(op_cmd, data)			\
        {														\
          SEND_CSL_CMD											\
          /* write op_cmd */										\
          SEND_OPFIFO_WRITE_CMD(OP_OS2IS, OP_REPEAT_ONE)		\
          SEND_DFIFO_WRITE_CMD(op_cmd)							\
          /* read register */										\
          SEND_OPFIFO_WRITE_CMD(OP_INS, OP_REPEAT_ONE)			\
          SEND_DFIFO_READ_CMD(data)								\
          SEND_CSH_CMD											\
        }

// Send general read register command 
// Parameters:
//		data:	type is char
#define SEND_READ_REGISTER_CMD(op_cmd, data)					\
        {														\
          SEND_MANUAL_BEGIN_CMD									\
          SEND_READ_REGISTER_INTERNAL_CMD(op_cmd, data)			\
          SEND_MANUAL_END_CMD									\
        }

// Send write register internal command 
// Parameters:
//		data:	type is char
#define SEND_WRITE_REGISTER_INTERNAL_CMD(op_cmd, data)			\
        {														\
          SEND_CSL_CMD											\
          /* write op_cmd */										\
          SEND_OPFIFO_WRITE_CMD(OP_OUTS, OP_REPEAT_ONE)			\
          SEND_DFIFO_WRITE_CMD(op_cmd)							\
          /* write register */										\
          SEND_OPFIFO_WRITE_CMD(OP_OUTS, OP_REPEAT_ONE)			\
          SEND_DFIFO_WRITE_CMD(data)							\
          SEND_CSH_CMD											\
        }

// Send general write register command 
// Parameters:
//		data:	type is char
#define SEND_WRITE_REGISTER_CMD(op_cmd, data)					\
        {														\
          SEND_MANUAL_WREN_BEGIN_CMD							\
          SEND_WRITE_REGISTER_INTERNAL_CMD(op_cmd, data)		\
          SEND_MANUAL_WREN_END_CMD								\
        }

// Send wait WIP bit idle command 
// Parameters:
//		reg_val:		type is char
#define SEND_WAIT_WIP_IDLE_INTERNAL_CMD(reg_val)						\
        {																\
          do {															\
          	SEND_READ_REGISTER_INTERNAL_CMD(SF_OP_RD_STATUS1, reg_val)	\
          } while (reg_val & SF_STATUS_WIP);							\
        }

// Send read data command
// Parameters:
//		dAddr: 	3bytes or 4bytes length, depend the value of mode
//		data:	type is char*
//		len: 	the value is [1, FlashSize-1]
//		mode:	1 means 4bytes, 0 means 3bytes
#define SEND_READ_CMD(dAddr, data, len, mode)						\
		{															\
		  SEND_MANUAL_BEGIN_CMD										\
		  SEND_CSL_CMD												\
		  /* write op_cmd 0x03 */									\
		  SEND_OPFIFO_WRITE_CMD(OP_OUTS, OP_REPEAT_ONE)				\
		  SEND_DFIFO_WRITE_CMD(SF_OP_RD_DATA)						\
		  /* write address */										\
		  SEND_OPFIFO_WRITE_CMD(OP_OUTS, ((mode)?0x03:0x02))		\
		  if(mode){													\
			 SEND_DFIFO_WRITE_CMD(bHiByte(wHiWord(dAddr))) 			\
		  }															\
		  SEND_DFIFO_WRITE_CMD(bLoByte(wHiWord(dAddr)))				\
		  SEND_DFIFO_WRITE_CMD(bHiByte(wLoWord(dAddr)))				\
		  SEND_OPFIFO_WRITE_CMD(OP_OS2IS, OP_REPEAT_ONE)			\
		  SEND_DFIFO_WRITE_CMD(bLoByte(wLoWord(dAddr)))				\
		  /* read data */											\
		  while((len) > 0) {										\
			 if((len) % SF_RD_MAX) {								\
				SEND_OPFIFO_WRITE_CMD(OP_INS, ((len)%SF_RD_MAX))	\
				do													\
				{													\
					SEND_DFIFO_READ_CMD(*((data)++)) 				\
				} while ((--(len)) % SF_RD_MAX); 					\
			 } else { 												\
				SEND_OPFIFO_WRITE_CMD(OP_INS, SF_RD_MAX)			\
				do													\
				{													\
					SEND_DFIFO_READ_CMD(*((data)++)) 				\
				} while ((--(len)) % SF_RD_MAX); 					\
			 }														\
		  }															\
		  SEND_CSH_CMD												\
		  SEND_MANUAL_END_CMD										\
		}
		
// Send fast read command 
// Parameters:
//		dAddr: 	3bytes or 4bytes length, depend the value of mode
//		data:	type is char*
//		len: 	the value is [1, FlashSize-1]
//		mode:	1 means 4bytes, 0 means 3bytes
#define SEND_FASTREAD_CMD(dAddr, data, len, mode)					\
		{															\
		  SEND_MANUAL_BEGIN_CMD										\
		  SEND_CSL_CMD												\
		  /* write op_cmd 0x0B */									\
		  SEND_OPFIFO_WRITE_CMD(OP_OUTS, OP_REPEAT_ONE) 			\
		  SEND_DFIFO_WRITE_CMD(SF_OP_FAST_RD_DATA)					\
		  /* write address */										\
		  SEND_OPFIFO_WRITE_CMD(OP_OUTS, ((mode)?0x04:0x03))		\
		  if(mode){													\
			 SEND_DFIFO_WRITE_CMD(bHiByte(wHiWord(dAddr))) 			\
		  }															\
		  SEND_DFIFO_WRITE_CMD(bLoByte(wHiWord(dAddr))) 			\
		  SEND_DFIFO_WRITE_CMD(bHiByte(wLoWord(dAddr))) 			\
		  SEND_DFIFO_WRITE_CMD(bLoByte(wLoWord(dAddr))) 			\
		  /* write dummy data */									\
		  SEND_OPFIFO_WRITE_CMD(OP_OS2IS, OP_REPEAT_ONE)			\
		  SEND_DFIFO_WRITE_CMD(DUMMY_DATA) 							\
		  /* read data */											\
		  while((len) > 0) {										\
			 if((len) % SF_RD_MAX) {								\
				SEND_OPFIFO_WRITE_CMD(OP_INS, ((len)%SF_RD_MAX))	\
				do													\
				{													\
					SEND_DFIFO_READ_CMD(*((data)++)) 				\
				} while ((--(len)) % SF_RD_MAX); 					\
			 } else { 												\
				SEND_OPFIFO_WRITE_CMD(OP_INS, SF_RD_MAX)			\
				do													\
				{													\
					SEND_DFIFO_READ_CMD(*((data)++)) 				\
				} while ((--(len)) % SF_RD_MAX); 					\
			 }														\
		  }															\
		  SEND_CSH_CMD												\
		  SEND_MANUAL_END_CMD										\
		}

// Send fast read dual output command 
// Parameters:
//		dAddr: 	3bytes or 4bytes length, depend the value of mode
//		data:	type is char*
//		len: 	the value is [1, FlashSize-1]
//		mode:	1 means 4bytes, 0 means 3bytes
#define SEND_FASTREAD_DUALOUT_CMD(dAddr, data, len, mode)			\
		{															\
		  SEND_MANUAL_BEGIN_CMD										\
		  SEND_CSL_CMD												\
		  /* write op_cmd 0x3B */									\
		  SEND_OPFIFO_WRITE_CMD(OP_OUTS, OP_REPEAT_ONE) 			\
		  SEND_DFIFO_WRITE_CMD(SF_OP_FAST_RD_DUAL_OUT)				\
		  /* write address */										\
		  SEND_OPFIFO_WRITE_CMD(OP_OUTS, ((mode)?0x04:0x03))		\
		  if(mode){													\
			 SEND_DFIFO_WRITE_CMD(bHiByte(wHiWord(dAddr))) 			\
		  }															\
		  SEND_DFIFO_WRITE_CMD(bLoByte(wHiWord(dAddr))) 			\
		  SEND_DFIFO_WRITE_CMD(bHiByte(wLoWord(dAddr))) 			\
		  SEND_DFIFO_WRITE_CMD(bLoByte(wLoWord(dAddr))) 			\
		  /* write dummy data */									\
		  SEND_OPFIFO_WRITE_CMD(OP_OS2ID, OP_REPEAT_ONE)			\
		  SEND_DFIFO_WRITE_CMD(DUMMY_DATA) 							\
		  /* read data */											\
		  while((len) > 0) {										\
			 if((len) % SF_RD_MAX) {								\
				SEND_OPFIFO_WRITE_CMD(OP_IND, ((len)%SF_RD_MAX))	\
				do													\
				{													\
					SEND_DFIFO_READ_CMD(*((data)++)) 				\
				} while ((--(len)) % SF_RD_MAX); 					\
			 } else { 												\
				SEND_OPFIFO_WRITE_CMD(OP_IND, SF_RD_MAX)			\
				do													\
				{													\
					SEND_DFIFO_READ_CMD(*((data)++)) 				\
				} while ((--(len)) % SF_RD_MAX); 					\
			 }														\
		  }															\
		  SEND_CSH_CMD												\
		  SEND_MANUAL_END_CMD										\
		}

// Send fast read dual I/O command 
// Parameters:
//		dAddr: 	3bytes or 4bytes length, depend the value of mode
//		data:	type is char*
//		len: 	the value is [1, FlashSize-1]
//		mode:	1 means 4bytes, 0 means 3bytes
#define SEND_FASTREAD_DUALIO_CMD(dAddr, data, len, mode)			\
		{															\
		  SEND_MANUAL_BEGIN_CMD										\
		  SEND_CSL_CMD												\
		  /* write op_cmd 0xBB */									\
		  SEND_OPFIFO_WRITE_CMD(OP_OUTS, OP_REPEAT_ONE) 			\
		  SEND_DFIFO_WRITE_CMD(SF_OP_FAST_RD_DUAL_IO)				\
		  /* write address */										\
		  SEND_OPFIFO_WRITE_CMD(OP_OUTD, ((mode)?0x04:0x03))		\
		  if(mode){													\
			 SEND_DFIFO_WRITE_CMD(bHiByte(wHiWord(dAddr))) 			\
		  }															\
		  SEND_DFIFO_WRITE_CMD(bLoByte(wHiWord(dAddr))) 			\
		  SEND_DFIFO_WRITE_CMD(bHiByte(wLoWord(dAddr))) 			\
		  SEND_DFIFO_WRITE_CMD(bLoByte(wLoWord(dAddr))) 			\
		  /* write dummy data */									\
		  SEND_OPFIFO_WRITE_CMD(OP_OD2ID, OP_REPEAT_ONE)			\
		  SEND_DFIFO_WRITE_CMD(DUMMY_DATA)							\
		  /* read data */											\
		  while((len) > 0) {										\
			 if((len) % SF_RD_MAX) {								\
				SEND_OPFIFO_WRITE_CMD(OP_IND, ((len)%SF_RD_MAX))	\
				do													\
				{													\
					SEND_DFIFO_READ_CMD(*((data)++)) 				\
				} while ((--(len)) % SF_RD_MAX); 					\
			 } else { 												\
				SEND_OPFIFO_WRITE_CMD(OP_IND, SF_RD_MAX)			\
				do													\
				{													\
					SEND_DFIFO_READ_CMD(*((data)++)) 				\
				} while ((--(len)) % SF_RD_MAX); 					\
			 }														\
		  }															\
		  SEND_CSH_CMD												\
		  SEND_MANUAL_END_CMD										\
		}

// Send page program command 
// Parameters:
//		dAddr: 	3bytes or 4bytes length, depend the value of mode
//		data:	type is char*
//		len: 	the value is [1, 256]
//		mode:	1 means 4bytes, 0 means 3bytes
#define SEND_PAGE_PROGRAM_CMD(dAddr, data, len, mode)				\
		{															\
		  SEND_MANUAL_WREN_BEGIN_CMD								\
		  SEND_CSL_CMD												\
		  /* write op_cmd 0x02 */									\
		  SEND_OPFIFO_WRITE_CMD(OP_OUTS, OP_REPEAT_ONE) 			\
		  SEND_DFIFO_WRITE_CMD(SF_OP_PAGE_PROGRAM) 					\
		  /* write address */										\
		  SEND_OPFIFO_WRITE_CMD(OP_OUTS, ((mode)?0x04:0x03))		\
		  if(mode){													\
			 SEND_DFIFO_WRITE_CMD(bHiByte(wHiWord(dAddr))) 			\
		  }															\
		  SEND_DFIFO_WRITE_CMD(bLoByte(wHiWord(dAddr))) 			\
		  SEND_DFIFO_WRITE_CMD(bHiByte(wLoWord(dAddr))) 			\
		  SEND_DFIFO_WRITE_CMD(bLoByte(wLoWord(dAddr))) 			\
		  /* page program */										\
		  SEND_OPFIFO_WRITE_CMD(OP_OUTS, (len))						\
		  do														\
		  {															\
			 SEND_DFIFO_WRITE_CMD(*((data)++)) 						\
		  } while ((--(len)) > 0); 									\
		  SEND_CSH_CMD												\
		  SEND_MANUAL_WREN_END_CMD									\
		}

// Send sector erase command
// Parameters:
//		dAddr: 	3bytes or 4bytes length, depend the value of mode
//		mode:	1 means 4bytes, 0 means 3bytes
#define SEND_SECTOR_ERASE_CMD(dAddr, mode)						\
		{														\
		  SEND_MANUAL_WREN_BEGIN_CMD							\
		  SEND_CSL_CMD											\
		  /* write op_cmd 0xD8 */								\
		  SEND_OPFIFO_WRITE_CMD(OP_OUTS, OP_REPEAT_ONE) 		\
		  SEND_DFIFO_WRITE_CMD(SF_OP_SECTOR_ERASE) 				\
		  /* write address */									\
		  SEND_OPFIFO_WRITE_CMD(OP_OUTS, ((mode)?0x04:0x03))	\
		  if(mode){												\
			 SEND_DFIFO_WRITE_CMD(bHiByte(wHiWord(dAddr))) 		\
		  }														\
		  SEND_DFIFO_WRITE_CMD(bLoByte(wHiWord(dAddr))) 		\
		  SEND_DFIFO_WRITE_CMD(bHiByte(wLoWord(dAddr))) 		\
		  SEND_DFIFO_WRITE_CMD(bLoByte(wLoWord(dAddr))) 		\
		  SEND_CSH_CMD											\
		  SEND_MANUAL_WREN_END_CMD								\
		}

// Send bulk erase command 
#define SEND_BULK_ERASE_CMD										\
		{														\
		  SEND_MANUAL_WREN_BEGIN_CMD							\
		  send_single_opcode_cmd(SF_OP_BULK_ERASE);				\
		  SEND_MANUAL_WREN_END_CMD								\
		}

// Send 4-Bytes read data command 
// Parameters:
//		dAddr: 	4bytes length
//		data:	type is char*
//		len: 	the value is [1, FlashSize-1]
#define SEND_4BYTES_READ_CMD(dAddr, data, len)						\
		{															\
		  SEND_MANUAL_BEGIN_CMD										\
		  SEND_CSL_CMD												\
		  /* write op_cmd 0x13 */									\
		  SEND_OPFIFO_WRITE_CMD(OP_OUTS, OP_REPEAT_ONE) 			\
		  SEND_DFIFO_WRITE_CMD(SF_OP_4BYTES_RD_DATA)				\
		  /* write address */										\
		  SEND_OPFIFO_WRITE_CMD(OP_OUTS, OP_REPEAT_THREE)			\
		  SEND_DFIFO_WRITE_CMD(bHiByte(wHiWord(dAddr))) 			\
		  SEND_DFIFO_WRITE_CMD(bLoByte(wHiWord(dAddr))) 			\
		  SEND_DFIFO_WRITE_CMD(bHiByte(wLoWord(dAddr))) 			\
		  SEND_OPFIFO_WRITE_CMD(OP_OS2IS, OP_REPEAT_ONE)			\
		  SEND_DFIFO_WRITE_CMD(bLoByte(wLoWord(dAddr))) 			\
		  /* read data */											\
		  while((len) > 0) {										\
			 if((len) % SF_RD_MAX) {								\
				SEND_OPFIFO_WRITE_CMD(OP_INS, ((len)%SF_RD_MAX))	\
				do													\
				{													\
					SEND_DFIFO_READ_CMD(*((data)++)) 				\
				} while ((--(len)) % SF_RD_MAX); 					\
			 } else { 												\
				SEND_OPFIFO_WRITE_CMD(OP_INS, SF_RD_MAX)			\
				do													\
				{													\
					SEND_DFIFO_READ_CMD(*((data)++)) 				\
				} while ((--(len)) % SF_RD_MAX); 					\
			 }														\
		  }															\
		  SEND_CSH_CMD												\
		  SEND_MANUAL_END_CMD										\
		}
		
// Send 4-Bytes fast read command 
// Parameters:
//		dAddr: 	4bytes length
//		data:	type is char*
//		len: 	the value is [1, FlashSize-1]
#define SEND_4BYTES_FASTREAD_CMD(dAddr, data, len)					\
		{															\
		  SEND_MANUAL_BEGIN_CMD										\
		  SEND_CSL_CMD												\
		  /* write op_cmd 0x0C */									\
		  SEND_OPFIFO_WRITE_CMD(OP_OUTS, OP_REPEAT_ONE) 			\
		  SEND_DFIFO_WRITE_CMD(SF_OP_4BYTES_FAST_RD_DATA)			\
		  /* write address */										\
		  SEND_OPFIFO_WRITE_CMD(OP_OUTS, OP_REPEAT_FOUR)			\
		  SEND_DFIFO_WRITE_CMD(bHiByte(wHiWord(dAddr))) 			\
		  SEND_DFIFO_WRITE_CMD(bLoByte(wHiWord(dAddr))) 			\
		  SEND_DFIFO_WRITE_CMD(bHiByte(wLoWord(dAddr))) 			\
		  SEND_DFIFO_WRITE_CMD(bLoByte(wLoWord(dAddr))) 			\
		  /* write dummy data */									\
		  SEND_OPFIFO_WRITE_CMD(OP_OS2IS, OP_REPEAT_ONE)			\
		  SEND_DFIFO_WRITE_CMD(DUMMY_DATA)							\
		  /* read data */											\
		  while((len) > 0) {										\
			 if((len) % SF_RD_MAX) {								\
				SEND_OPFIFO_WRITE_CMD(OP_INS, ((len)%SF_RD_MAX))	\
				do													\
				{													\
					SEND_DFIFO_READ_CMD(*((data)++)) 				\
				} while ((--(len)) % SF_RD_MAX); 					\
			 } else { 												\
				SEND_OPFIFO_WRITE_CMD(OP_INS, SF_RD_MAX)			\
				do													\
				{													\
					SEND_DFIFO_READ_CMD(*((data)++)) 				\
				} while ((--(len)) % SF_RD_MAX); 					\
			 }														\
		  }															\
		  SEND_CSH_CMD												\
		  SEND_MANUAL_END_CMD										\
		}

// Send 4-Bytes fast read dual output command 
// Parameters:
//		dAddr: 	4bytes length
//		data:	type is char*
//		len: 	the value is [1, FlashSize-1]
#define SEND_4BYTES_FASTREAD_DUALOUT_CMD(dAddr, data, len)			\
		{															\
		  SEND_MANUAL_BEGIN_CMD										\
		  SEND_CSL_CMD												\
		  /* write op_cmd 0x3C */									\
		  SEND_OPFIFO_WRITE_CMD(OP_OUTS, OP_REPEAT_ONE) 			\
		  SEND_DFIFO_WRITE_CMD(SF_OP_4BYTES_FAST_RD_DUAL_OUT)		\
		  /* write address */										\
		  SEND_OPFIFO_WRITE_CMD(OP_OUTS, OP_REPEAT_FOUR)			\
		  SEND_DFIFO_WRITE_CMD(bHiByte(wHiWord(dAddr))) 			\
		  SEND_DFIFO_WRITE_CMD(bLoByte(wHiWord(dAddr))) 			\
		  SEND_DFIFO_WRITE_CMD(bHiByte(wLoWord(dAddr))) 			\
		  SEND_DFIFO_WRITE_CMD(bLoByte(wLoWord(dAddr))) 			\
		  /* write dummy data */									\
		  SEND_OPFIFO_WRITE_CMD(OP_OS2ID, OP_REPEAT_ONE)			\
		  SEND_DFIFO_WRITE_CMD(DUMMY_DATA)							\
		  /* read data */											\
		  while((len) > 0) {										\
			 if((len) % SF_RD_MAX) {								\
				SEND_OPFIFO_WRITE_CMD(OP_IND, ((len)%SF_RD_MAX))	\
				do													\
				{													\
					SEND_DFIFO_READ_CMD(*((data)++)) 				\
				} while ((--(len)) % SF_RD_MAX); 					\
			 } else { 												\
				SEND_OPFIFO_WRITE_CMD(OP_IND, SF_RD_MAX)			\
				do													\
				{													\
					SEND_DFIFO_READ_CMD(*((data)++)) 				\
				} while ((--(len)) % SF_RD_MAX); 					\
			 }														\
		  }															\
		  SEND_CSH_CMD												\
		  SEND_MANUAL_END_CMD										\
		}

// Send 4-Bytes fast read dual I/O command 
// Parameters:
//		dAddr: 	4bytes length
//		data:	type is char*
//		len: 	the value is [1, FlashSize-1]
#define SEND_4BYTES_FASTREAD_DUALIO_CMD(dAddr, data, len)			\
		{															\
		  SEND_MANUAL_BEGIN_CMD										\
		  SEND_CSL_CMD												\
		  /* write op_cmd 0xBC */									\
		  SEND_OPFIFO_WRITE_CMD(OP_OUTS, OP_REPEAT_ONE) 			\
		  SEND_DFIFO_WRITE_CMD(SF_OP_4BYTES_FAST_RD_DUAL_IO)		\
		  /* write address */										\
		  SEND_OPFIFO_WRITE_CMD(OP_OUTD, OP_REPEAT_FOUR)			\
		  SEND_DFIFO_WRITE_CMD(bHiByte(wHiWord(dAddr))) 			\
		  SEND_DFIFO_WRITE_CMD(bLoByte(wHiWord(dAddr))) 			\
		  SEND_DFIFO_WRITE_CMD(bHiByte(wLoWord(dAddr))) 			\
		  SEND_DFIFO_WRITE_CMD(bLoByte(wLoWord(dAddr))) 			\
		  /* write dummy data */									\
		  SEND_OPFIFO_WRITE_CMD(OP_OD2ID, OP_REPEAT_ONE)			\
		  SEND_DFIFO_WRITE_CMD(DUMMY_DATA)							\
		  /* read data */											\
		  while((len) > 0) {										\
			 if((len) % SF_RD_MAX) {								\
				SEND_OPFIFO_WRITE_CMD(OP_IND, ((len)%SF_RD_MAX))	\
				do													\
				{													\
					SEND_DFIFO_READ_CMD(*((data)++)) 				\
				} while ((--(len)) % SF_RD_MAX); 					\
			 } else { 												\
				SEND_OPFIFO_WRITE_CMD(OP_IND, SF_RD_MAX)			\
				do													\
				{													\
					SEND_DFIFO_READ_CMD(*((data)++)) 				\
				} while ((--(len)) % SF_RD_MAX); 					\
			 }														\
		  }															\
		  SEND_CSH_CMD												\
		  SEND_MANUAL_END_CMD										\
		}

// Send enter 4bytes address mode command 
#define SEND_ENTER_4BYTES_MODE_CMD								\
        {														\
          SEND_MANUAL_BEGIN_CMD									\
          send_single_opcode_cmd(SF_OP_ENTER_4BYTES_MODE);		\
          SEND_MANUAL_END_CMD									\
        }
		
// Send exit 4bytes address mode command 
#define SEND_EXIT_4BYTES_MODE_CMD								\
        {														\
          SEND_MANUAL_BEGIN_CMD									\
          send_single_opcode_cmd(SF_OP_EXIT_4BYTES_MODE);		\
          SEND_MANUAL_END_CMD									\
        }
		
// Send enter 4bytes address mode command with Write Enable
#define SEND_ENTER_4BYTES_MODE_WREN_CMD							\
        {														\
          SEND_MANUAL_WREN_BEGIN_CMD							\
          send_single_opcode_cmd(SF_OP_ENTER_4BYTES_MODE);		\
          SEND_MANUAL_WREN_END_CMD								\
        }
		
// Send exit 4bytes address mode command with Write Enable
#define SEND_EXIT_4BYTES_MODE_WREN_CMD							\
        {														\
          SEND_MANUAL_WREN_BEGIN_CMD							\
          send_single_opcode_cmd(SF_OP_EXIT_4BYTES_MODE);		\
          SEND_MANUAL_WREN_END_CMD								\
        }
		

// Send power down command 
#define SEND_POWER_DOWN_CMD										\
        {														\
          SEND_MANUAL_BEGIN_CMD									\
          send_single_opcode_cmd(SF_OP_POWER_DOWN);				\
          SEND_MANUAL_END_CMD									\
        }
		
// Send release power down command 
#define SEND_RELEASE_POWER_DOWN_CMD								\
        {														\
          SEND_MANUAL_BEGIN_CMD									\
          send_single_opcode_cmd(SF_OP_RELEASE_POWER_DOWN);		\
          SEND_MANUAL_END_CMD									\
        }		

// Send read jedec id command 
// Parameters:
//		data:	type is char[3]
#define SEND_RD_ID_CMD(data)									\
        {														\
          SEND_MANUAL_BEGIN_CMD									\
          SEND_CSL_CMD											\
          /* write op_cmd 0x9F */								\
          SEND_OPFIFO_WRITE_CMD(OP_OS2IS, OP_REPEAT_ONE)		\
          SEND_DFIFO_WRITE_CMD(SF_OP_RD_ID)						\
          /* read data */										\
          SEND_OPFIFO_WRITE_CMD(OP_INS, OP_REPEAT_THREE)		\
          SEND_DFIFO_READ_CMD(*(data)) 							\
          SEND_DFIFO_READ_CMD(*((data)+1)) 						\
          SEND_DFIFO_READ_CMD(*((data)+2)) 						\
          SEND_CSH_CMD											\
          SEND_MANUAL_END_CMD									\
        }

// change from auto mode 2 manual mode
#define SEND_AUTO2MANUAL_CMD									\
        {														\
      	 /* set 9 to SF_MTX_MODE_TOG */						\
          WriteReg(SF_MTX_MODE_TOG, MANUAL_MTXMODE);			\
      	 /* enable Manual mode */								\
          WriteReg(SF_MANUAL_EN, MANUAL_MANUALEN);				\
        }

// change from manual mode 2 auto mode
#define SEND_MANUAL2AUTO_CMD									\
        {														\
      	 /* set 0 to SF_MTX_MODE_TOG */						\
          WriteReg(SF_MTX_MODE_TOG, AUTO_MTXMODE);				\
      	 /* enable auto mode */									\
          WriteReg(SF_MANUAL_EN, AUTO_MANUALEN);				\
        }

// change auto mode to 3Byte address
#define SEND_AUTO_3B_CMD										\
        {														\
      	 /* enable auto address change */						\
          WriteReg(SF_CFG3B4B_EN, CFG3B4B_EN);					\
      	 /* aauto address is 3Byte */								\
          WriteReg(SF_ADDR_3B4B, AUTO_3BMODE);					\
        }

// change Tn Value
#define SEND_TN_CMD(data)										\
        {														\
      	 /* set data to SF_SI_CK_SEL */							\
          WriteReg(SF_SI_CK_SEL, ((data) & SI_SEL_MASK));		\
        }

// change auto mode to 4Byte address
#define SEND_AUTO_4B_CMD										\
        {														\
      	 /* enable auto address change */						\
          WriteReg(SF_CFG3B4B_EN, CFG3B4B_EN);					\
      	 /* auto address is 4Byte */								\
          WriteReg(SF_ADDR_3B4B, AUTO_4BMODE);					\
        }

// Enable Auto Mode Read Idle Capability
#define SEND_READ_IDLE_EN_CMD									\
        {														\
      	 /* Enalbe Read IDLE */									\
          WriteReg(SF_READ_IDLE_EN, RD_IDLE_EN);				\
        }

// Disable Auto Mode Read Idle Capability
#define SEND_READ_IDLE_DIS_CMD									\
        {														\
      	 /* Disable Read IDLE */									\
          WriteReg(SF_READ_IDLE_EN, RD_IDLE_DIS);				\
        }

// change auto mode to Read
#define SEND_AUTO_READ_CMD										\
        {														\
      	 /* auto mode is Read */									\
          WriteReg(SF_READ_MODE, AUTO_READ);					\
        }

// change auto mode to Fast Read
#define SEND_AUTO_FASTREAD_CMD									\
        {														\
      	 /* auto mode is Fast Read */							\
          WriteReg(SF_READ_MODE, AUTO_FAST_READ);				\
        }

// change auto mode to Fast Read Dual Output
#define SEND_AUTO_FASTREAD_DUALOUT_CMD							\
        {														\
      	 /* auto mode is Fast Read Dual Output */					\
          WriteReg(SF_READ_MODE, AUTO_FAST_READ_DUALOUT);		\
        }

// change auto mode to Fast Read Dual IO
#define SEND_AUTO_FASTREAD_DUALIO_CMD							\
        {														\
      	 /* auto mode is Fast Read Dual IO */						\
          WriteReg(SF_READ_MODE, AUTO_FAST_READ_DUALIO);		\
        }
#endif

