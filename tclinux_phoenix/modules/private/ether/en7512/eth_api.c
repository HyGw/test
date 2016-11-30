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

/************************************************************************
*                  I N C L U D E S
*************************************************************************
*/
#include "eth_lan.h"

#include "./tcphy/phy_api.h"
#include "./tcphy/mtkswitch_api.h"
/************************************************************************
*                  D E F I N E S   &   C O N S T A N T S
*************************************************************************
*/

/************************************************************************
*                  M A C R O S
*************************************************************************
*/

/************************************************************************
*                  D A T A   T Y P E S
*************************************************************************
*/

/************************************************************************
*                  E X T E R N A L   D A T A   D E C L A R A T I O N S
*************************************************************************
*/

extern uint8 ratelimit_En ;
extern unsigned char mac_sync_en;

/************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
*************************************************************************
*/

/************************************************************************
*                  P U B L I C   D A T A
*************************************************************************
*/

/************************************************************************
*                  P R I V A T E   D A T A
*************************************************************************
*/

/************************************************************************
*                  F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/
int eth_mdio_read(struct ECNT_ETHER_Data *eth_data)
{
    eth_data->ether_private.phy.data = tcMiiStationRead(eth_data->ether_private.phy.add,
                                            eth_data->ether_private.phy.reg);
    return 0;
}

int eth_mdio_write(struct ECNT_ETHER_Data *eth_data)
{
    tcMiiStationWrite(eth_data->ether_private.phy.add,
        eth_data->ether_private.phy.reg,
        eth_data->ether_private.phy.data);
    return 0;
}

int eth_mac_send(struct ECNT_ETHER_Data *eth_data)
{
    macSend(WAN2LAN_CH_ID,eth_data->ether_private.skb);
    return 0;
}

int eth_gsw_pbus_read(struct ECNT_ETHER_Data *eth_data)
{
    eth_data->ether_private.phy.data = gswPbusRead(eth_data->ether_private.phy.add);
    return 0;
}

int eth_gsw_pbus_write(struct ECNT_ETHER_Data *eth_data)
{
    eth_data->retValue = gswPbusWrite(eth_data->ether_private.phy.add, eth_data->ether_private.phy.data);
    return 0;
}

int eth_ext_gsw_pbus_read(struct ECNT_ETHER_Data *eth_data)
{
    eth_data->ether_private.phy.data = gswPbusRead2(eth_data->ether_private.phy.add);
    return 0;
}

int eth_ext_gsw_pbus_write(struct ECNT_ETHER_Data *eth_data)
{
    eth_data->retValue = gswPbusWrite2(eth_data->ether_private.phy.add, eth_data->ether_private.phy.data);
    return 0;
}

/*ratelimit_En == 1: means downstream total bandwidth ratelimit works*/
int eth_set_ratelimit_switch(struct ECNT_ETHER_Data *eth_data)
{
	unsigned char tmp_En = eth_data->ether_private.ratelimit_En ;

	/*1. set the switch_value for channel selection, send to channel 0 or send channel by port mask*/
	ratelimit_En = tmp_En ;

	/*2. set flow control enable or not , port6*/
	macMT7530SetMACForceFC( 6 , (!tmp_En) );

	/*3. clear hw_enty*/
#if defined(TCSUPPORT_RA_HWNAT_ENHANCE_HOOK)
	{
		extern int (*ra_sw_nat_hook_clean_table)(void);
		if (ra_sw_nat_hook_clean_table)
		{
			ra_sw_nat_hook_clean_table();
		}
	}
#endif

    return 0;
}

int eth_set_mactable_sync_en(struct ECNT_ETHER_Data *eth_data)
{
    mac_sync_en = eth_data->ether_private.enable ;

    return 0;
}

int rgmii_traffic_set(struct ECNT_ETHER_Data *eth_data)
{
    rgmii_setting(eth_data->ether_private.traffic_setting.wan_type, 
        eth_data->ether_private.traffic_setting.interface);
    return 0;
}

int rgmii_mode(struct ECNT_ETHER_Data *eth_data)
{
    if(eth_data->ether_private.traffic_setting.interface == RGMII){
        rgmii_disable(eth_data->ether_private.traffic_setting.mode);
    }
    return 0;
}
