/***************************************************************
Copyright Statement:

This software/firmware and related documentation (EcoNet Software) 
are protected under relevant copyright laws. The information contained herein 
is confidential and proprietary to EcoNet (HK) Limited (EcoNet) and/or 
its licensors. Without the prior written permission of EcoNet and/or its licensors, 
any reproduction, modification, use or disclosure of EcoNet Software, and 
information contained herein, in whole or in part, shall be strictly prohibited.

EcoNet (HK) Limited  EcoNet. ALL RIGHTS RESERVED.

BY OPENING OR USING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY 
ACKNOWLEDGES AND AGREES THAT THE SOFTWARE/FIRMWARE AND ITS 
DOCUMENTATIONS (ECONET SOFTWARE) RECEIVED FROM ECONET 
AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON AN AS IS 
BASIS ONLY. ECONET EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, 
WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, 
OR NON-INFRINGEMENT. NOR DOES ECONET PROVIDE ANY WARRANTY 
WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTIES WHICH 
MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE ECONET SOFTWARE. 
RECEIVER AGREES TO LOOK ONLY TO SUCH THIRD PARTIES FOR ANY AND ALL 
WARRANTY CLAIMS RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES 
THAT IT IS RECEIVERS SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD 
PARTY ALL PROPER LICENSES CONTAINED IN ECONET SOFTWARE.

ECONET SHALL NOT BE RESPONSIBLE FOR ANY ECONET SOFTWARE RELEASES 
MADE TO RECEIVERS SPECIFICATION OR CONFORMING TO A PARTICULAR 
STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND 
ECONET'S ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE ECONET 
SOFTWARE RELEASED HEREUNDER SHALL BE, AT ECONET'S SOLE OPTION, TO 
REVISE OR REPLACE THE ECONET SOFTWARE AT ISSUE OR REFUND ANY SOFTWARE 
LICENSE FEES OR SERVICE CHARGES PAID BY RECEIVER TO ECONET FOR SUCH 
ECONET SOFTWARE.
***************************************************************/
#ifndef _QDMA_GLB_H_
#define _QDMA_GLB_H_

/************************************************************************
*                  I N C L U D E S
*************************************************************************
*/

/************************************************************************
*                  D E F I N E S   &   C O N S T A N T S
*************************************************************************
*/

#if defined(QDMA_LAN) // qdma1
    #define CONFIG_QDMA_CHANNEL					8
    #define CONFIG_QDMA_QUEUE					8
#if defined(CONFIG_SUPPORT_SELF_TEST)
    #define CONFIG_HWFWD_DSCP_NUM				(1024)	// for csr2 test
#else
    #define CONFIG_HWFWD_DSCP_NUM				(1024) //shoule not more than 4096
#endif

    #define CONFIG_DSCP_NUM_MAX					(4096)
    #define CONFIG_IRQ_NUM_MAX					(4095)

    #define CONFIG_TX1_DSCP_NUM					(128)
    #define CONFIG_TX0_DSCP_NUM					(128)
#ifdef MT7615E	    
#if (defined(TCSUPPORT_VOIP) || defined(WAN_GPON) || defined(WAN_EPON))
    #define CONFIG_RX1_DSCP_NUM					(1024) 
#else /* TCSUPPORT_VOIP */
    #define CONFIG_RX1_DSCP_NUM					(896)
#endif /* TCSUPPORT_VOIP */

#else
    #define CONFIG_RX1_DSCP_NUM					(512) 
#endif	
    #define CONFIG_RX0_DSCP_NUM					(128)
    #define CONFIG_IRQ_DEPTH					(256)
    #define CONFIG_MAX_PKT_LENS					(2048)
	#define HWFWD_LOW_THRESHOLD					(128)
	#define CONFIG_MAX_CNTR_NUM					(40)

#else // qdma2
    #define CONFIG_QDMA_CHANNEL					32
    #define CONFIG_QDMA_QUEUE					8
#if defined(CONFIG_SUPPORT_SELF_TEST)
    #define CONFIG_HWFWD_DSCP_NUM				(1000)	// for csr2 test
#else
    #define CONFIG_HWFWD_DSCP_NUM				(4096) //shoule not more than 4096
#endif
    
    #define CONFIG_DSCP_NUM_MAX					(4096)
    #define CONFIG_IRQ_NUM_MAX					(4095)
    
    #define CONFIG_TX1_DSCP_NUM					(128)
    #define CONFIG_TX0_DSCP_NUM					(1024)
    #define CONFIG_RX1_DSCP_NUM					(256)
    #define CONFIG_RX0_DSCP_NUM					(256)
    #define CONFIG_IRQ_DEPTH					(512)
    #define CONFIG_MAX_PKT_LENS					(2048)
	#define HWFWD_LOW_THRESHOLD					(256)
	#define CONFIG_MAX_CNTR_NUM					(40)
#endif

#define QUEUE_ALL_NUM (CONFIG_QDMA_CHANNEL*CONFIG_QDMA_QUEUE)


/************************************************************************
*                  M A C R O S
*************************************************************************
*/

/************************************************************************
*                  D A T A	 T Y P E S
*************************************************************************
*/

/************************************************************************
*                  D A T A   D E C L A R A T I O N S
*************************************************************************
*/

/************************************************************************
*                  F U N C T I O N    D E C L A R A T I O N S
#                  I N L I N E    F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/

#endif /* _QDMA_GLB_H_ */

