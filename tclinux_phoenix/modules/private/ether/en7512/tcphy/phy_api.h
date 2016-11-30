/***************************************************************
Copyright Statement:

This software/firmware and related documentation (¡°EcoNet Software¡±) 
are protected under relevant copyright laws. The information contained herein 
is confidential and proprietary to EcoNet (HK) Limited (¡°EcoNet¡±) and/or 
its licensors. Without the prior written permission of EcoNet and/or its licensors, 
any reproduction, modification, use or disclosure of EcoNet Software, and 
information contained herein, in whole or in part, shall be strictly prohibited.

EcoNet (HK) Limited  EcoNet. ALL RIGHTS RESERVED.

BY OPENING OR USING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY 
ACKNOWLEDGES AND AGREES THAT THE SOFTWARE/FIRMWARE AND ITS 
DOCUMENTATIONS (¡°ECONET SOFTWARE¡±) RECEIVED FROM ECONET 
AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON AN ¡°AS IS¡± 
BASIS ONLY. ECONET EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, 
WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, 
OR NON-INFRINGEMENT. NOR DOES ECONET PROVIDE ANY WARRANTY 
WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTIES WHICH 
MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE ECONET SOFTWARE. 
RECEIVER AGREES TO LOOK ONLY TO SUCH THIRD PARTIES FOR ANY AND ALL 
WARRANTY CLAIMS RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES 
THAT IT IS RECEIVER¡¯S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD 
PARTY ALL PROPER LICENSES CONTAINED IN ECONET SOFTWARE.

ECONET SHALL NOT BE RESPONSIBLE FOR ANY ECONET SOFTWARE RELEASES 
MADE TO RECEIVER¡¯S SPECIFICATION OR CONFORMING TO A PARTICULAR 
STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND 
ECONET'S ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE ECONET 
SOFTWARE RELEASED HEREUNDER SHALL BE, AT ECONET'S SOLE OPTION, TO 
REVISE OR REPLACE THE ECONET SOFTWARE AT ISSUE OR REFUND ANY SOFTWARE 
LICENSE FEES OR SERVICE CHARGES PAID BY RECEIVER TO ECONET FOR SUCH 
ECONET SOFTWARE.
***************************************************************/
#ifndef _PHY_API_H
#define _PHY_API_H

/************************************************************************
*               I N C L U D E S
*************************************************************************
*/


/************************************************************************
*               D E F I N E S   &   C O N S T A N T S
*************************************************************************
*/
    /* ADMTEK6996M register */
#define ADM_PORT0_BASIC		0x01
#define ADM_PORT1_BASIC		0x03
#define ADM_PORT2_BASIC		0x05
#define ADM_PORT3_BASIC		0x07
#define ADM_PORT4_BASIC		0x08
#define ADM_PORT5_BASIC		0x09
    
#define ADM_CHIP_ID0		0xa0
#define ADM_CHIP_ID1		0xa1

/************************************************************************
*               M A C R O S
*************************************************************************
*/

/************************************************************************
*               D A T A   T Y P E S
*************************************************************************
*/

/************************************************************************
*               D A T A   D E C L A R A T I O N S
*************************************************************************
*/

/************************************************************************
*               F U N C T I O N   D E C L A R A T I O N S
                I N L I N E  F U N C T I O N  D E F I N I T I O N S
*************************************************************************
*/
uint32 gswPmiRead(uint32 phy_addr, uint32 phy_reg);
uint32 gswPmiWrite(uint32 phy_addr, uint32 phy_reg, uint32 phy_data);

uint32 tcMiiStationRead(uint32 phy_addr, uint32 phy_reg);
void tcMiiStationWrite(uint32 phy_addr, uint32 phy_reg, uint32 phy_data);

int mdio_read(struct net_device *dev, int phy_id, int reg_num);
void mdio_write(struct net_device *dev, int phy_id, int reg_num, int val);



#endif /*_PHY_API_H*/

