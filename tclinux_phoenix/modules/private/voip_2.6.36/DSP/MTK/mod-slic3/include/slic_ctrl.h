/*
** $Id: //BBN_Linux/Branch/Branch_MTK_VOIP_20111216/tclinux_phoenix/modules/private/voip/pcm_slic/slic_ctrl.h#1 $
*/
/************************************************************************
 *
 *	Copyright (C) 2008 Trendchip Technologies, Corp.
 *	All Rights Reserved.
 *
 * Trendchip Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of Trendchip Technologies, Co. and shall
 * not be reproduced, copied, disclosed, or used in whole or in part
 * for any reason without the prior express written permission of
 * Trendchip Technologeis, Co.
 *
 *************************************************************************/
/*
** $Log: slic_ctrl.h,v $
** Revision 1.2  2010/06/17 11:41:50  jrchen_hc
** Add TC3182 PCM and SLIC support for VoIP
**
** Revision 1.1  2010/04/09 11:05:00  feiyan
** tclinux_phoenix new Trunk
** Add all voip driver in one folder
**
** Revision 1.1.1.1  2009/12/17 01:47:52  josephxu
** 20091217, from Hinchu ,with VoIP
**
 */
#ifndef _TC3262SLIC_H
#define _TC3262SLIC_H

#define SLIC_TYPE_ZARLINK 1
#define SLIC_TYPE_SILICON 2
#define SLIC_TYPE_LANTIQ  3

/*********************************
 * Return Values *
 *********************************/
#define S_OK		0
#define S_ERROR		-1

/*********************************
 * Reset Values *
 *********************************/
#ifndef PCMDRIVER_H
#define RESET		0
#define ENABLE		1
#endif

/*********************************
 * Register Access Prefix *
 *********************************/
#define VPint			*(volatile unsigned int *)

/*********************************
 * Related GPIO Module Registers *
 *********************************/
#define CR_GPIO_BASE       	0xBFBF0200
#define CR_GPIO_CTRL	    (CR_GPIO_BASE + 0x00)
#define CR_GPIO_DATA	    (CR_GPIO_BASE + 0x04)
#define CR_GPIO_ODRAIN      (CR_GPIO_BASE + 0x14)


/*********************************
 * Related GPIO Number *
 *********************************/
// 3262 ASIC
#ifdef TC3262_PCM
#define SLIC1_GPIO       	10
#elif TC3182_PCM
#define SLIC1_GPIO       	13
#else
#define SLIC1_GPIO       	15
#endif
//3262 FPGA: #define SLIC1_GPIO       	14
//#define SLIC2_GPIO		    15

/*********************************
 * Related SPI Module Registers *
 *********************************/
#define CR_SPI_BASE     	0xBFBC0000
#define SPI_FLASH_CTL       0x00
#define SPI_FLASH_OPCODE    0x04
#define SPI_FLASH_DATA      0x08
#define SPI_FLASH_MM        0x28
#define SPI_FLASH_MBC		0x2c

#define SPI_CTL_START       0x00000100
#define SPI_CTL_BUSY        0x00010000


#endif /* _TC3262GMAC_H */
