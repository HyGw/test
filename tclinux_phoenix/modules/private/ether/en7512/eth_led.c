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
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/dma-mapping.h>
#include <linux/mii.h>
#include <asm/io.h>
#include <asm/tc3162/tc3162.h>
#include <asm/tc3162/ledcetrl.h>
#include <asm/tc3162/TCIfSetQuery_os.h>

#include "eth_lan.h"
#include "eth_led.h"

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
extern macAdapter_t *mac_p;
extern int g_port_reverse;

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
#ifdef TCSUPPORT_XPON_LED
static struct timer_list eth_led_timer;
static unsigned int eth_pkt_num[4] = {0};
#endif



/************************************************************************
*                  F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/

/************************************************************************
    Funtcion:
    Description:    
    Calls:
    Called by:      eth_led
    Input:
    Output:
    Return:
    Others:     
************************************************************************/


#ifdef TCSUPPORT_XPON_LED
extern uint8 led_ether_up_status[];

void eth_led(unsigned long data){ 
    int i = 0;
    unsigned long pkts, pkts_diff ;
    int tempPort = 0;
 
    for(i = 1; i<5;i++){
        if(isMT7520S && i != 4){
            continue;
        }

        pkts = eth_pkt_num[i-1];

        if(g_port_reverse == 1){
            tempPort = 4- i +1;         
            if (isMT7525G){
                eth_pkt_num[i-1] = gswPbusRead(EXT_GSW_RX_UNIC(tempPort-1)) + 
                    gswPbusRead(EXT_GSW_RX_MULC(tempPort-1)) + 
                    gswPbusRead(EXT_GSW_RX_BROC(tempPort-1)) +
                    gswPbusRead(EXT_GSW_TX_UNIC(tempPort-1)) + 
                    gswPbusRead(EXT_GSW_TX_MULC(tempPort-1)) +
                    gswPbusRead(EXT_GSW_TX_BROC(tempPort-1));
            }
            else
            {
                eth_pkt_num[i-1] = read_reg_word(GSW_RX_UNIC(tempPort)) + 
                    read_reg_word(GSW_RX_MULC(tempPort)) + 
                    read_reg_word(GSW_RX_BROC(tempPort)) + 
                    read_reg_word(GSW_TX_UNIC(tempPort)) +
                    read_reg_word(GSW_TX_MULC(tempPort)) +
                    read_reg_word(GSW_TX_BROC(tempPort));
            }
        
        }else{
            if (isMT7525G){
                eth_pkt_num[i-1] = gswPbusRead(EXT_GSW_RX_UNIC(i-1)) + 
                    gswPbusRead(EXT_GSW_RX_MULC(i-1)) + 
                    gswPbusRead(EXT_GSW_RX_BROC(i-1)) + 
                    gswPbusRead(EXT_GSW_TX_UNIC(i-1)) + 
                    gswPbusRead(EXT_GSW_TX_MULC(i-1)) + 
                    gswPbusRead(EXT_GSW_TX_BROC(i-1));
            }
            else
            {
#if !defined(TCSUPPORT_XPON_HAL_API_EXT) 
                eth_pkt_num[i-1] = read_reg_word(GSW_RX_UNIC(i))+read_reg_word(GSW_RX_MULC(i))+read_reg_word(GSW_RX_BROC(i))
                          + read_reg_word(GSW_TX_UNIC(i))+read_reg_word(GSW_TX_MULC(i))+read_reg_word(GSW_TX_BROC(i));
#endif
            }
        }
        pkts_diff = eth_pkt_num[i-1] - pkts;

#if !defined(TCSUPPORT_TW_BOARD_CDS) 
   {
        if (led_ether_up_status[i-1] == 2){ /*  LINK up*/
            if (pkts_diff > 0){ /* BLINK*/
                {
                ledTurnOn(LED_ETHER_PORT1_ACT_STATUS+2*(i-1));
#if !defined(TCSUPPORT_C5_XPON_AUTH_LED) 
                ledTurnOff(LED_ETHER_PORT1_STATUS + 2*(i-1));
#endif
}       
            }else{ // ON
                {
                ledTurnOff(LED_ETHER_PORT1_ACT_STATUS+2*(i-1));
                ledTurnOn(LED_ETHER_PORT1_STATUS + 2*(i-1));
            }
            }
        }else if (led_ether_up_status[i-1] == 0){ // LINK DOWN
            {       
            ledTurnOff(LED_ETHER_PORT1_ACT_STATUS+2*(i-1));
            ledTurnOff(LED_ETHER_PORT1_STATUS + 2*(i-1));
}
        }
#endif
    }

    }
    /* Schedule for the next time */
    eth_led_timer.expires = jiffies + msecs_to_jiffies(250);
    add_timer(&eth_led_timer);
}
#endif

inline void eth_led_tx_hook(uint8 port_mask)
{
    /***************** light the led *****************/	
  	ledTurnOn(LED_ETHER_ACT_STATUS);
	if (mac_p->macPhyLinkProfile_p->enetMode & LAN_ST_100MB)
		ledTurnOn(LED_ETHER_100M_ACT_STATUS);
	else
		ledTurnOn(LED_ETHER_10M_ACT_STATUS);
#ifdef TCSUPPORT_XPON_LED
	if (isMT7520S){
		if (port_mask & (1<<4) && led_ether_up_status[3] == 2){
			ledTurnOn(LED_ETHER_PORT4_ACT_STATUS);
		}
	}else if (isMT7525 || isMT7520){
		int i = 0;
		for (i = 0; i < 4; i++){
			if ((port_mask & (1<<(i+1))) && led_ether_up_status[i] ==2){
				ledTurnOn(LED_ETHER_PORT1_ACT_STATUS + 2*i);
			}
		}
	}
#endif
}

inline void eth_led_rx_hook(uint8 pvid)
{
#ifdef TCSUPPORT_XPON_LED

	if (isMT7520S && pvid == 0){
		if (led_ether_up_status[3] == 2){
			ledTurnOn(LED_ETHER_PORT4_ACT_STATUS);
		}
	}else if ((isMT7525 || isMT7520) && (pvid>=0 &&pvid < 4)){
		if (led_ether_up_status[pvid] == 2){
			ledTurnOn(LED_ETHER_PORT1_ACT_STATUS + 2*(pvid));
		}
	}	
#endif
}


void eth_led_init(void)
{
#ifdef TCSUPPORT_XPON_LED
    init_timer(&eth_led_timer);
    eth_led_timer.expires = jiffies + msecs_to_jiffies(250);
    eth_led_timer.function = eth_led;
    eth_led_timer.data = 0;
    add_timer(&eth_led_timer);
    memset(&eth_pkt_num, 0, 4*sizeof(unsigned int));
#endif	
}


void eth_led_deinit(void)
{
#ifdef TCSUPPORT_XPON_LED
    del_timer_sync(&eth_led_timer);
#endif	

}
