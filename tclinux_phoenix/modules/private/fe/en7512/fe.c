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
#include <asm/tc3162/cmdparse.h>
#include <asm/tc3162/tc3162.h>
#include <linux/mtd/rt_flash.h>


#ifdef TCSUPPORT_RA_HWNAT
#include <linux/foe_hook.h>
#endif
#include <ecnt_hook/ecnt_hook.h>
#include <ecnt_hook/ecnt_hook_fe.h>
#include "fe.h"
#include "fe_api.h"

/************************************************************************
*                  D E F I N E S   &   C O N S T A N T S
*************************************************************************
*/

/************************************************************************
*                  M A C R O S
*************************************************************************
*/
#define FE_MAJOR 226

/************************************************************************
*                  D A T A   T Y P E S
*************************************************************************
*/

/************************************************************************
*                  E X T E R N A L   D A T A   D E C L A R A T I O N S
*************************************************************************
*/

/************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
*************************************************************************
*/

long fe_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) ;
/************************************************************************
*                  P U B L I C   D A T A
*************************************************************************
*/
/* ADSL RTS dump function */

void TCConsole(uint8 mode)
{
}
EXPORT_SYMBOL(TCConsole);

void uartMacPutchar(int ch)
{
}
EXPORT_SYMBOL(uartMacPutchar);

uint32 GetIpAddr(void)
{
    return 0;
}
EXPORT_SYMBOL(GetIpAddr);


/************************************************************************
*                  P R I V A T E   D A T A
*************************************************************************
*/
struct file_operations fe_fops = {
	.owner			= THIS_MODULE,
	.unlocked_ioctl = fe_ioctl,
};


/************************************************************************
*                  F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/
static int fe_reg_dump_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int index = 0;
	off_t pos = 0;
	off_t begin = 0;
	int i;
	uint32 reg;


	index += sprintf(buf+index, "FE_DMA_GLO_CFG   (0x%08x) = 0x%08lx\n", FE_DMA_GLO_CFG, read_reg_word(FE_DMA_GLO_CFG));
	CHK_BUF();
	index += sprintf(buf+index, "FE_RST_GLO       (0x%08x) = 0x%08lx\n", FE_RST_GLO, read_reg_word(FE_RST_GLO)); 	
	CHK_BUF();
	index += sprintf(buf+index, "FE_INT_STATUS    (0x%08x) = 0x%08lx\n", FE_INT_STATUS, read_reg_word(FE_INT_STATUS)); 	
	CHK_BUF();
	index += sprintf(buf+index, "FE_INT_Enable    (0x%08x) = 0x%08lx\n", FE_INT_ENABLE, read_reg_word(FE_INT_ENABLE));
	CHK_BUF();
	index += sprintf(buf+index, "FOE_TS_T         (0x%08x) = 0x%08lx\n", FE_FOE_TS, read_reg_word(FE_FOE_TS));
	CHK_BUF();
	index += sprintf(buf+index, "FOE_IPv6_EXT     (0x%08x) = 0x%08lx\n", FE_IPV6_EXT, read_reg_word(FE_IPV6_EXT));
	CHK_BUF();
	index += sprintf(buf+index, "FOE_CNT_CFG      (0x%08x) = 0x%08lx\n", FE_CNT_CFG, read_reg_word(FE_CNT_CFG));
	CHK_BUF();

	
	index += sprintf(buf+index, "PSE_FQFC_CFG	  (0x%08x) = 0x%08lx\n", PSE_FQFC_CFG, read_reg_word(PSE_FQFC_CFG));
	CHK_BUF();
	index += sprintf(buf+index, "PSE_IQ_REV_VIP	  (0x%08x) = 0x%08lx\n", PSE_IQ_REV_VIP, read_reg_word(PSE_IQ_REV_VIP));
	CHK_BUF();
	index += sprintf(buf+index, "PSE_IQ_REV1	  (0x%08x) = 0x%08lx\n", PSE_IQ_REV1, read_reg_word(PSE_IQ_REV1));
	CHK_BUF();
	index += sprintf(buf+index, "PSE_IQ_REV2	  (0x%08x) = 0x%08lx\n", PSE_IQ_REV2, read_reg_word(PSE_IQ_REV2));
	CHK_BUF();
	index += sprintf(buf+index, "PSE_IQ_STA1	  (0x%08x) = 0x%08lx\n", PSE_IQ_STA1, read_reg_word(PSE_IQ_STA1));
	CHK_BUF();
	index += sprintf(buf+index, "PSE_IQ_STA2	  (0x%08x) = 0x%08lx\n", PSE_IQ_STA2, read_reg_word(PSE_IQ_STA2));
	CHK_BUF();
	index += sprintf(buf+index, "PSE_IQ PER PORT 0:0x%02lx 1:0x%02lx 2:0x%02lx 5:0x%02lx\n", (read_reg_word(PSE_IQ_STA1) & 0xff), ((read_reg_word(PSE_IQ_STA1)>>8) & 0xff), ((read_reg_word(PSE_IQ_STA1)>>16) & 0xff), ((read_reg_word(PSE_IQ_STA2)>>8) & 0xff));
	CHK_BUF();
	index += sprintf(buf+index, "PSE_OQ_STA1	  (0x%08x) = 0x%08lx\n", PSE_OQ_STA1, read_reg_word(PSE_OQ_STA1));
	CHK_BUF();
	index += sprintf(buf+index, "PSE_OQ_STA2	  (0x%08x) = 0x%08lx\n", PSE_OQ_STA2, read_reg_word(PSE_OQ_STA2));
	CHK_BUF();
	index += sprintf(buf+index, "PSE_OQ PER PORT 0:0x%02lx 1:0x%02lx 2:0x%02lx 3:0x%02lx 4:0x%02lx 5:0x%02lx 6:0x%02lx\n", (read_reg_word(PSE_OQ_STA1) & 0xff), ((read_reg_word(PSE_OQ_STA1)>>8) & 0xff), ((read_reg_word(PSE_OQ_STA1)>>16) & 0xff), ((read_reg_word(PSE_OQ_STA1)>>24) & 0xff), (read_reg_word(PSE_OQ_STA2) & 0xff), ((read_reg_word(PSE_OQ_STA2)>>8) & 0xff), ((read_reg_word(PSE_OQ_STA2)>>16) & 0xff));
	CHK_BUF();
	index += sprintf(buf+index, "PSE_MISC_CFG	  (0x%08x) = 0x%08lx\n", PSE_MISC_CFG, read_reg_word(PSE_MISC_CFG));
	CHK_BUF();
	index += sprintf(buf+index, "FE_PROBE_L		  (0x%08x) = 0x%08lx\n", FE_PROBE_L, read_reg_word(FE_PROBE_L));
	CHK_BUF();
	index += sprintf(buf+index, "FE_PROBE_H		  (0x%08x) = 0x%08lx\n", FE_PROBE_H, read_reg_word(FE_PROBE_H));
	CHK_BUF();
	index += sprintf(buf+index, "PSE_FQ_MIN		  (0x%08x) = 0x%08lx\n", PSE_FQ_MIN, read_reg_word(PSE_FQ_MIN));
	CHK_BUF();
	index += sprintf(buf+index, "PSE_IQ_MAX1	  (0x%08x) = 0x%08lx\n", PSE_IQ_MAX1, read_reg_word(PSE_IQ_MAX1));
	CHK_BUF();
	index += sprintf(buf+index, "FE_PROBE_H		  (0x%08x) = 0x%08lx\n", PSE_IQ_MAX2, read_reg_word(PSE_IQ_MAX2));
	CHK_BUF();
	index += sprintf(buf+index, "PSE_DROP_COUNT_0	  (0x%08x) = 0x%08lx\n", PSE_DROP_COUNT_0, read_reg_word(PSE_DROP_COUNT_0));
	CHK_BUF();
	index += sprintf(buf+index, "PSE_DROP_COUNT_1	  (0x%08x) = 0x%08lx\n", PSE_DROP_COUNT_1, read_reg_word(PSE_DROP_COUNT_1));
	CHK_BUF();
	index += sprintf(buf+index, "PSE_DROP_COUNT_2	  (0x%08x) = 0x%08lx\n", PSE_DROP_COUNT_2, read_reg_word(PSE_DROP_COUNT_2));
	CHK_BUF();
	index += sprintf(buf+index, "PSE_DROP_COUNT_4	  (0x%08x) = 0x%08lx\n", PSE_DROP_COUNT_4, read_reg_word(PSE_DROP_COUNT_4));
	CHK_BUF();
	index += sprintf(buf+index, "PSE_DROP_COUNT_5	  (0x%08x) = 0x%08lx\n", PSE_DROP_COUNT_5, read_reg_word(PSE_DROP_COUNT_5));
	CHK_BUF();

/* L2u, L2Br, VIP do not dump */


	/* CDMA1 Reg */
	index += sprintf(buf+index, "CDMA_VLAN_CTRL       (0x%08x) = 0x%08lx\n", CDMA1_VLAN_CTRL, read_reg_word(CDMA1_VLAN_CTRL));
	CHK_BUF();
	index += sprintf(buf+index, "CDMA_PPP_GEN         (0x%08x) = 0x%08lx\n", CDMA1_PPP_GEN, read_reg_word(CDMA1_PPP_GEN));
	CHK_BUF();
	index += sprintf(buf+index, "CDMA_FWD_CFG         (0x%08x) = 0x%08lx\n", CDMA1_FWD_CFG, read_reg_word(CDMA1_FWD_CFG));
	CHK_BUF();
	index += sprintf(buf+index, "CDMA_CRSN_QSEL       (0x%08x) = 0x%08lx\n", CDMA1_CRSN_QSEL, read_reg_word(CDMA1_CRSN_QSEL));
	CHK_BUF();
	index += sprintf(buf+index, "CDMA_HWF_CHN_EN      (0x%08x) = 0x%08lx\n", CDMA1_HWF_CHN_EN, read_reg_word(CDMA1_HWF_CHN_EN));
	CHK_BUF();
	
	/* GDMA1 Reg */
	index += sprintf(buf+index, "GDMA1_FWD_CFG        (0x%08x) = 0x%08lx\n", GDMA1_FWD_CFG, read_reg_word(GDMA1_FWD_CFG));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA1_SHRP_CFG       (0x%08x) = 0x%08lx\n", GDMA1_SHRP_CFG, read_reg_word(GDMA1_SHRP_CFG));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA1_MAC_ADRL       (0x%08x) = 0x%08lx\n", GDMA1_MAC_ADRL, read_reg_word(GDMA1_MAC_ADRL));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA1_MAC_ADRH       (0x%08x) = 0x%08lx\n", GDMA1_MAC_ADRH, read_reg_word(GDMA1_MAC_ADRH));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA1_VLAN_CHECK     (0x%08x) = 0x%08lx\n", GDMA1_VLAN_CHECK, read_reg_word(GDMA1_VLAN_CHECK));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA1_LEN_CFG        (0x%08x) = 0x%08lx\n", GDMA1_LEN_CFG, read_reg_word(GDMA1_LEN_CFG));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA1_LAN_PCP        (0x%08x) = 0x%08lx\n", GDMA1_LAN_PCP, read_reg_word(GDMA1_LAN_PCP));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA1_LPBK_CFG       (0x%08x) = 0x%08lx\n", GDMA1_LPBK_CFG, read_reg_word(GDMA1_LPBK_CFG));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA1_CHN_RLS       (0x%08x) = 0x%08lx\n", GDMA1_CHN_RLS, read_reg_word(GDMA1_CHN_RLS));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA1_TXCHN_EN 	 (0x%08x) = 0x%08lx\n", GDMA1_TXCHN_EN, read_reg_word(GDMA1_TXCHN_EN));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA1_RXCHN_EN 	 (0x%08x) = 0x%08lx\n", GDMA1_RXCHN_EN, read_reg_word(GDMA1_RXCHN_EN));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA1_RXCHN_FP 	 (0x%08x) = 0x%08lx\n", GDMA1_RXCHN_FP, read_reg_word(GDMA1_RXCHN_FP));
	CHK_BUF();
	index += sprintf(buf+index, "FE_CPORT_CFG 	     (0x%08x) = 0x%08lx\n", FE_CPORT_CFG, read_reg_word(FE_CPORT_CFG));
	CHK_BUF();
	index += sprintf(buf+index, "FE_CPORT_CHN_MAP 	 (0x%08x) = 0x%08lx\n", FE_CPORT_CHN_MAP, read_reg_word(FE_CPORT_CHN_MAP));
	CHK_BUF();
	index += sprintf(buf+index, "FE_CPORT_SHRP_CFG 	 (0x%08x) = 0x%08lx\n", FE_CPORT_SHRP_CFG, read_reg_word(FE_CPORT_SHRP_CFG));
	CHK_BUF();
	index += sprintf(buf+index, "GDM1_TX_CHN_VLD 	 (0x%08x) = 0x%08lx\n", GDMA1_TX_CHN_VLD, read_reg_word(GDMA1_TX_CHN_VLD));
	CHK_BUF();
	index += sprintf(buf+index, "GDM1_RX_CHN_VLD     (0x%08x) = 0x%08lx\n", GDMA1_RX_CHN_VLD, read_reg_word(GDMA1_RX_CHN_VLD));
	CHK_BUF();

		
	index += sprintf(buf+index, "CDMA1_TX_OK_CNT	 (0x%08x) = 0x%08lx\n", CDMA1_TX_OK_CNT, read_reg_word(CDMA1_TX_OK_CNT));
	CHK_BUF();
	index += sprintf(buf+index, "CDMA1_RXCPU_OK_CNT	 (0x%08x) = 0x%08lx\n", CDMA1_RXCPU_OK_CNT, read_reg_word(CDMA1_RXCPU_OK_CNT));
	CHK_BUF();
	index += sprintf(buf+index, "CDMA1_RXHWF_OK_CNT	 (0x%08x) = 0x%08lx\n", CDMA1_RXHWF_OK_CNT, read_reg_word(CDMA1_RXHWF_OK_CNT));
	CHK_BUF();
	index += sprintf(buf+index, "CDMA1_RXCPU_KA_CNT	 (0x%08x) = 0x%08lx\n", CDMA1_RXCPU_KA_CNT, read_reg_word(CDMA1_RXCPU_KA_CNT));
	CHK_BUF();
	index += sprintf(buf+index, "CDMA1_RXCPU_DROP_CNT(0x%08x) = 0x%08lx\n", CDMA1_RXCPU_DROP_CNT, read_reg_word(CDMA1_RXCPU_DROP_CNT));
	CHK_BUF();
	index += sprintf(buf+index, "CDMA1_RXHWF_DROP_CNT(0x%08x) = 0x%08lx\n", CDMA1_RXHWF_DROP_CNT, read_reg_word(CDMA1_RXHWF_DROP_CNT));
	CHK_BUF();

	index += sprintf(buf+index, "GDMA1_MIB_CLER       (0x%08x) = Write Only\n", GDMA1_MIB_CLER);
	CHK_BUF();
	/* GDMA1 count display */
	index += sprintf(buf+index, "GDMA1_TX_GET_CNT        (0x%08x) = 0x%08lx\n", GDMA1_TX_GET_CNT , read_reg_word(GDMA1_TX_GET_CNT));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA1_TX_OK_CNT         (0x%08x) = 0x%08lx\n", GDMA1_TX_OK_CNT, read_reg_word(GDMA1_TX_OK_CNT));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA1_TX_DROP_CNT       (0x%08x) = 0x%08lx\n", GDMA1_TX_DROP_CNT, read_reg_word(GDMA1_TX_DROP_CNT));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA1_TX_OK_BYTE_CNT    (0x%08x) = 0x%08lx\n", GDMA1_TX_OK_BYTE_CNT, read_reg_word(GDMA1_TX_OK_BYTE_CNT));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA1_RX_OK_CNT	     (0x%08x) = 0x%08lx\n", GDMA1_RX_OK_CNT , read_reg_word(GDMA1_RX_OK_CNT));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA1_RX_FC_DROP_CNT    (0x%08x) = 0x%08lx\n", GDMA1_RX_FC_DROP_CNT, read_reg_word(GDMA1_RX_FC_DROP_CNT));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA1_RX_RC_DROP_CNT    (0x%08x) = 0x%08lx\n", GDMA1_RX_RC_DROP_CNT, read_reg_word(GDMA1_RX_RC_DROP_CNT));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA1_RX_OVER_DROP_CNT  (0x%08x) = 0x%08lx\n", GDMA1_RX_OVER_DROP_CNT, read_reg_word(GDMA1_RX_OVER_DROP_CNT));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA1_RX_ERROR_DROP_CNT (0x%08x) = 0x%08lx\n", GDMA1_RX_ERROR_DROP_CNT, read_reg_word(GDMA1_RX_ERROR_DROP_CNT));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA1_RX_BYTECNT        (0x%08x) = 0x%08lx\n", GDMA1_RX_BYTECNT, read_reg_word(GDMA1_RX_BYTECNT));
	CHK_BUF();
	

	/* CDMA2 */ 
	index += sprintf(buf+index, "CDMA2_VLAN_CTRL  (0x%08x) = 0x%08lx\n", CDMA2_VLAN_CTRL, read_reg_word(CDMA2_VLAN_CTRL));
	CHK_BUF();
	index += sprintf(buf+index, "CDMA2_PPP_GEN    (0x%08x) = 0x%08lx\n", CDMA2_PPP_GEN, read_reg_word(CDMA2_PPP_GEN));
	CHK_BUF();
	index += sprintf(buf+index, "CDMA2_FWD_CFG    (0x%08x) = 0x%08lx\n", CDMA2_FWD_CFG , read_reg_word(CDMA2_FWD_CFG));
	CHK_BUF();
	index += sprintf(buf+index, "CDMA2_HWF_CHN_EN (0x%08x) = 0x%08lx\n", CDMA2_HWF_CHN_EN, read_reg_word(CDMA2_HWF_CHN_EN));
	CHK_BUF();
	index += sprintf(buf+index, "CDMA2_CRSN_QSEL0  (0x%08x) = 0x%08lx\n", CDMA2_CRSN_QSEL0, read_reg_word(CDMA2_CRSN_QSEL0));
	CHK_BUF();
	index += sprintf(buf+index, "CDMA2_CRSN_QSEL1  (0x%08x) = 0x%08lx\n", CDMA2_CRSN_QSEL1, read_reg_word(CDMA2_CRSN_QSEL1));
	CHK_BUF();

	/* GDMA2 */
	index += sprintf(buf+index, "GDMA2_FWD_CFG        (0x%08x) = 0x%08lx\n", GDMA2_FWD_CFG, read_reg_word(GDMA2_FWD_CFG));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA2_SHRP_CFG       (0x%08x) = 0x%08lx\n", GDMA2_SHRP_CFG, read_reg_word(GDMA2_SHRP_CFG));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA2_MAC_ADRL       (0x%08x) = 0x%08lx\n", GDMA2_MAC_ADRL, read_reg_word(GDMA2_MAC_ADRL));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA2_MAC_ADRH       (0x%08x) = 0x%08lx\n", GDMA2_MAC_ADRH, read_reg_word(GDMA2_MAC_ADRH));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA2_VLAN_CHECK     (0x%08x) = 0x%08lx\n", GDMA2_VLAN_CHECK, read_reg_word(GDMA2_VLAN_CHECK));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA2_LEN_CFG        (0x%08x) = 0x%08lx\n", GDMA2_LEN_CFG, read_reg_word(GDMA2_LEN_CFG));
	CHK_BUF();
	index += sprintf(buf+index, "FE_WAN_PCP           (0x%08x) = 0x%08lx\n", GDMA2_WAN_PCP, read_reg_word(GDMA2_WAN_PCP));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA2_LPBP_CFG       (0x%08x) = 0x%08lx\n", GDMA2_LPBP_CFG, read_reg_word(GDMA2_LPBP_CFG));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA2_CHN_RLS        (0x%08x) = 0x%08lx\n", GDMA2_CHN_RLS, read_reg_word(GDMA2_CHN_RLS));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA2_TXCHN_EN       (0x%08x) = 0x%08lx\n", GDMA2_TXCHN_EN, read_reg_word(GDMA2_TXCHN_EN));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA2_RXCHN_EN       (0x%08x) = 0x%08lx\n", GDMA2_RXCHN_EN, read_reg_word(GDMA2_RXCHN_EN));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA2_RXCHN_FP       (0x%08x) = 0x%08lx\n", GDMA2_RXCHN_FP, read_reg_word(GDMA2_RXCHN_FP));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA2_RXUC_SHPR_CFG  (0x%08x) = 0x%08lx\n", GDMA2_RXUC_SHPR_CFG, read_reg_word(GDMA2_RXUC_SHPR_CFG));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA2_RXBC_SHPR_CFG  (0x%08x) = 0x%08lx\n", GDMA2_RXBC_SHPR_CFG, read_reg_word(GDMA2_RXBC_SHPR_CFG));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA2_RXMC_SHPR_CFG  (0x%08x) = 0x%08lx\n", GDMA2_RXMC_SHPR_CFG, read_reg_word(GDMA2_RXMC_SHPR_CFG));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA2_RXOC_SHPR_CFG  (0x%08x) = 0x%08lx\n", GDMA2_RXOC_SHPR_CFG, read_reg_word(GDMA2_RXOC_SHPR_CFG));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA2_TX_CHN_VLD     (0x%08x) = 0x%08lx\n", GDMA2_TX_CHN_VLD, read_reg_word(GDMA2_TX_CHN_VLD));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA2_RX_CHN_VLD     (0x%08x) = 0x%08lx\n", GDMA2_RX_CHN_VLD, read_reg_word(GDMA2_RX_CHN_VLD));
	CHK_BUF();
	index += sprintf(buf+index, "CDMA2_TX_OK_CNT      (0x%08x) = 0x%08lx\n", CDMA2_TX_OK_CNT, read_reg_word(CDMA2_TX_OK_CNT));
	CHK_BUF();
	index += sprintf(buf+index, "CDMA2_RXCPU_OK_CNT   (0x%08x) = 0x%08lx\n", CDMA2_RXCPU_OK_CNT, read_reg_word(CDMA2_RXCPU_OK_CNT));
	CHK_BUF();
	index += sprintf(buf+index, "CDMA2_RXHWF_OK_CNT   (0x%08x) = 0x%08lx\n", CDMA2_RXHWF_OK_CNT, read_reg_word(CDMA2_RXHWF_OK_CNT));
	CHK_BUF();
	index += sprintf(buf+index, "CDMA2_RXCPU_KA_CNT   (0x%08x) = 0x%08lx\n", CDMA2_RXCPU_KA_CNT, read_reg_word(CDMA2_RXCPU_KA_CNT));
	CHK_BUF();
	index += sprintf(buf+index, "CDMA2_RXCPU_DROP_CNT (0x%08x) = 0x%08lx\n", CDMA2_RXCPU_DROP_CNT, read_reg_word(CDMA2_RXCPU_DROP_CNT));
	CHK_BUF();
	index += sprintf(buf+index, "CDMA2_RXHWF_DROP_CNT (0x%08x) = 0x%08lx\n", CDMA2_RXHWF_DROP_CNT, read_reg_word(CDMA2_RXHWF_DROP_CNT));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA2_MIB_CLER       (0x%08x) = Write Only\n", GDMA2_MIB_CLER);
	CHK_BUF();

	
	/* GDMA2 count display */
	/* GDMA2_TX_GETCNT */
	index += sprintf(buf+index, "GDMA2_TX_GETCNT      (0x%08x) = 0x%08lx\n", GDMA2_TX_GETCNT, read_reg_word(GDMA2_TX_GETCNT));
	CHK_BUF();
	/* GDMA2_TX_GOKCNT */
	index += sprintf(buf+index, "GDMA2_TX_GOKCNT      (0x%08x) = 0x%08lx\n", GDMA2_TX_OKCNT, read_reg_word(GDMA2_TX_OKCNT));
	CHK_BUF();
	/* GDMA2_TX_DROPCNT */	
	index += sprintf(buf+index, "GDMA2_TX_DROPCNT     (0x%08x) = 0x%08lx\n", GDMA2_TX_DROPCNT, read_reg_word(GDMA2_TX_DROPCNT));
	CHK_BUF();
    /* GDMA2_TX_OKBYTESCNT */
	/* wait polling mod */
	index += sprintf(buf+index, "GDMA2_TX_OKBYTE_CNT	  (0x%08x) = 0x%08lx\n", GDMA2_TX_OKBYTE_CNT, read_reg_word(GDMA2_TX_OKBYTE_CNT));
	CHK_BUF();
	/* GDMA2_RX_OKCNT */
	index += sprintf(buf+index, "GDMA2_RX_OKCNT       (0x%08x) = 0x%08lx\n", GDMA2_RX_OKCNT, read_reg_word(GDMA2_RX_OKCNT));
	CHK_BUF();

    /* GDMA2_RX_FLOWCTRLDROPCNT */
    index += sprintf(buf+index, "GDMA2_RX_FCDROPCNT   (0x%08x) = 0x%08lx\n", GDMA2_RX_FCDROPCNT, read_reg_word(GDMA2_RX_FCDROPCNT));
    CHK_BUF();
    /* GDMA2_RX_RATECTRLDROPCNT */
    index += sprintf(buf+index, "GDMA2_RX_RCDROPCNT   (0x%08x) = 0x%08lx\n", GDMA2_RX_RCDROPCNT, read_reg_word(GDMA2_RX_RCDROPCNT));
    CHK_BUF();

	/* GDMA2_RX_OVDROPCNT */		
	index += sprintf(buf+index, "GDMA2_RX_OVDROPCNT   (0x%08x) = 0x%08lx\n", GDMA2_RX_OVDROPCNT, read_reg_word(GDMA2_RX_OVDROPCNT));
	CHK_BUF();
	/* GDMA2_RX_ERRDROPCNT */
		
	index += sprintf(buf+index, "GDMA2_RX_ERRDROPCNT  (0x%08x) = 0x%08lx\n", GDMA2_RX_ERRDROPCNT, read_reg_word(GDMA2_RX_ERRDROPCNT));
	CHK_BUF();
    /* GDMA2_RX_RXOKBYTECNT */	
	index += sprintf(buf+index, "GDMA2_RX_OKBYTECNT  (0x%08x) = 0x%08lx\n", GDMA2_RX_OKBYTECNT, read_reg_word(GDMA2_RX_OKBYTECNT));
	CHK_BUF();


	*eof = 1;

done:
	*start = buf + (off - begin);
	index -= (off - begin);
	if (index<0) 
		index = 0;
	if (index>count) 
		index = count;
	return index;
}


static int fe_debug_reg_dump_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int index = 0;
	off_t pos = 0;
	off_t begin = 0;
	int i;
	uint32 reg;


	index += sprintf(buf+index, "PSE_IQ_STA1             (0x%08x) = 0x%08lx\n", PSE_IQ_STA1, read_reg_word(PSE_IQ_STA1));
	CHK_BUF();
	index += sprintf(buf+index, "PSE_IQ_STA2             (0x%08x) = 0x%08lx\n", PSE_IQ_STA2, read_reg_word(PSE_IQ_STA2));
	CHK_BUF();
	index += sprintf(buf+index, "PSE_OQ_STA1             (0x%08x) = 0x%08lx\n", PSE_OQ_STA1, read_reg_word(PSE_OQ_STA1));
	CHK_BUF();
	index += sprintf(buf+index, "PSE_OQ_STA2             (0x%08x) = 0x%08lx\n\n", PSE_OQ_STA2, read_reg_word(PSE_OQ_STA2));
	CHK_BUF();

        
	index += sprintf(buf+index, "CDMA1_TX_OK_CNT         (0x%08x) = 0x%08lx\n", CDMA1_TX_OK_CNT, read_reg_word(CDMA1_TX_OK_CNT));
	CHK_BUF();
	index += sprintf(buf+index, "CDMA1_RXCPU_OK_CNT      (0x%08x) = 0x%08lx\n", CDMA1_RXCPU_OK_CNT, read_reg_word(CDMA1_RXCPU_OK_CNT));
	CHK_BUF();
	index += sprintf(buf+index, "CDMA1_RXHWF_OK_CNT      (0x%08x) = 0x%08lx\n", CDMA1_RXHWF_OK_CNT, read_reg_word(CDMA1_RXHWF_OK_CNT));
	CHK_BUF();
	index += sprintf(buf+index, "CDMA1_RXCPU_DROP_CNT    (0x%08x) = 0x%08lx\n", CDMA1_RXCPU_DROP_CNT, read_reg_word(CDMA1_RXCPU_DROP_CNT));
	CHK_BUF();
	index += sprintf(buf+index, "CDMA1_RXHWF_DROP_CNT    (0x%08x) = 0x%08lx\n\n", CDMA1_RXHWF_DROP_CNT, read_reg_word(CDMA1_RXHWF_DROP_CNT));
	CHK_BUF();


	index += sprintf(buf+index, "GDMA1_TX_GET_CNT        (0x%08x) = 0x%08lx\n", GDMA1_TX_GET_CNT , read_reg_word(GDMA1_TX_GET_CNT));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA1_TX_OK_CNT         (0x%08x) = 0x%08lx\n", GDMA1_TX_OK_CNT, read_reg_word(GDMA1_TX_OK_CNT));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA1_TX_DROP_CNT       (0x%08x) = 0x%08lx\n", GDMA1_TX_DROP_CNT, read_reg_word(GDMA1_TX_DROP_CNT));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA1_RX_OK_CNT         (0x%08x) = 0x%08lx\n", GDMA1_RX_OK_CNT , read_reg_word(GDMA1_RX_OK_CNT));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA1_RX_FC_DROP_CNT    (0x%08x) = 0x%08lx\n", GDMA1_RX_FC_DROP_CNT, read_reg_word(GDMA1_RX_FC_DROP_CNT));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA1_RX_RC_DROP_CNT    (0x%08x) = 0x%08lx\n", GDMA1_RX_RC_DROP_CNT, read_reg_word(GDMA1_RX_RC_DROP_CNT));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA1_RX_OVER_DROP_CNT  (0x%08x) = 0x%08lx\n", GDMA1_RX_OVER_DROP_CNT, read_reg_word(GDMA1_RX_OVER_DROP_CNT));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA1_RX_ERROR_DROP_CNT (0x%08x) = 0x%08lx\n\n", GDMA1_RX_ERROR_DROP_CNT, read_reg_word(GDMA1_RX_ERROR_DROP_CNT));
	CHK_BUF();

    
	index += sprintf(buf+index, "CDMA2_TX_OK_CNT         (0x%08x) = 0x%08lx\n", CDMA2_TX_OK_CNT, read_reg_word(CDMA2_TX_OK_CNT));
	CHK_BUF();
	index += sprintf(buf+index, "CDMA2_RXCPU_OK_CNT      (0x%08x) = 0x%08lx\n", CDMA2_RXCPU_OK_CNT, read_reg_word(CDMA2_RXCPU_OK_CNT));
	CHK_BUF();
	index += sprintf(buf+index, "CDMA2_RXHWF_OK_CNT      (0x%08x) = 0x%08lx\n", CDMA2_RXHWF_OK_CNT, read_reg_word(CDMA2_RXHWF_OK_CNT));
	CHK_BUF();
	index += sprintf(buf+index, "CDMA2_RXCPU_DROP_CNT    (0x%08x) = 0x%08lx\n", CDMA2_RXCPU_DROP_CNT, read_reg_word(CDMA2_RXCPU_DROP_CNT));
	CHK_BUF();
	index += sprintf(buf+index, "CDMA2_RXHWF_DROP_CNT    (0x%08x) = 0x%08lx\n\n", CDMA2_RXHWF_DROP_CNT, read_reg_word(CDMA2_RXHWF_DROP_CNT));
	CHK_BUF();

    	
	index += sprintf(buf+index, "GDMA2_TX_GETCNT         (0x%08x) = 0x%08lx\n", GDMA2_TX_GETCNT, read_reg_word(GDMA2_TX_GETCNT));
	CHK_BUF();
	/* GDMA2_TX_GOKCNT */		
	index += sprintf(buf+index, "GDMA2_TX_GOKCNT         (0x%08x) = 0x%08lx\n", GDMA2_TX_OKCNT, read_reg_word(GDMA2_TX_OKCNT));
	CHK_BUF();
	/* GDMA2_TX_DROPCNT */		
	index += sprintf(buf+index, "GDMA2_TX_DROPCNT        (0x%08x) = 0x%08lx\n", GDMA2_TX_DROPCNT, read_reg_word(GDMA2_TX_DROPCNT));
	CHK_BUF();
	/* GDMA2_RX_OKCNT */		
	index += sprintf(buf+index, "GDMA2_RX_OKCNT          (0x%08x) = 0x%08lx\n", GDMA2_RX_OKCNT, read_reg_word(GDMA2_RX_OKCNT));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA2_RX_FCDROPCNT      (0x%08x) = 0x%08lx\n", GDMA2_RX_FCDROPCNT, read_reg_word(GDMA2_RX_FCDROPCNT));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA2_RX_RCDROPCNT      (0x%08x) = 0x%08lx\n", GDMA2_RX_RCDROPCNT, read_reg_word(GDMA2_RX_RCDROPCNT));
	CHK_BUF();
	/* GDMA2_RX_OVDROPCNT */		
	index += sprintf(buf+index, "GDMA2_RX_OVDROPCNT      (0x%08x) = 0x%08lx\n", GDMA2_RX_OVDROPCNT, read_reg_word(GDMA2_RX_OVDROPCNT));
	CHK_BUF();
	/* GDMA2_RX_ERRDROPCNT */		
	index += sprintf(buf+index, "GDMA2_RX_ERRDROPCNT     (0x%08x) = 0x%08lx\n\n", GDMA2_RX_ERRDROPCNT, read_reg_word(GDMA2_RX_ERRDROPCNT));
	CHK_BUF();


	*eof = 1;

done:
	*start = buf + (off - begin);
	index -= (off - begin);
	if (index<0) 
		index = 0;
	if (index>count) 
		index = count;
	return index;
}


void fe_reset(void)
{
	unsigned int regVal;

	regVal = regRead32(CR_RSTCTRL2);
	regVal |= (QDMA1_RST | QDMA2_RST | FE_RST | ESW_RST);
	regWrite32(CR_RSTCTRL2, regVal);
	mdelay(1);	
	regVal &= ~(QDMA1_RST | QDMA2_RST | FE_RST | ESW_RST);
	mdelay(1);
	regWrite32(CR_RSTCTRL2, regVal);

}
#if defined(TCSUPPORT_FWC_ENV)
extern int fhdrv_kdrv_get_cpu_mac(unsigned char *mac, unsigned int len);
#endif

void setup_mac_reg(void){
	unsigned char mac[6];
	int i;

#if defined(TCSUPPORT_FWC_ENV)
    fhdrv_kdrv_get_cpu_mac((unsigned char *)(mac), 6);
#else
    for (i=0; i<6; i++) {
        mac[i] = READ_FLASH_BYTE(flash_base + 0xff48 + i);
    }
#endif
	/***GDMA1**/
	write_reg_word(GDMA1_MAC_ADRL, (mac[2]<<24) | (mac[3]<<16) | (mac[4]<<8) | (mac[5]<<0));
	write_reg_word(GDMA1_MAC_ADRH, (0xf8<<16) | (mac[0]<<8) | (mac[1]<<0)); /* need to mask 3 bits for ethWan */

	/***GDMA2, default mask 3 bit**/
	write_reg_word(GDMA2_MAC_ADRL, (mac[2]<<24) | (mac[3]<<16) | (mac[4]<<8) | (mac[5]<<0));
	write_reg_word(GDMA2_MAC_ADRH, (0xf8<<16) | (mac[0]<<8) | (mac[1]<<0));

}

void setup_pse_IQ_reservation(void)
{
    unsigned int reg;

    /* decrease PSE p2 and increase PSE p5 IQ reservation
     * in order to prevent from upstream packet drop at
     * GPON multi-tcom test */
    reg = read_reg_word(PSE_IQ_REV1);
    reg &= ~(0xff<<PSE_IQ_RES1_P2_OFFSET);
    reg |= (0x20<<PSE_IQ_RES1_P2_OFFSET);
    write_reg_word(PSE_IQ_REV1, reg);

    reg = read_reg_word(PSE_IQ_REV2);
    reg &= ~(0xff<<PSE_IQ_RES2_P5_OFFSET);
    reg |= (0x40<<PSE_IQ_RES2_P5_OFFSET);
    write_reg_word(PSE_IQ_REV2, reg);

    return;
}
void fe_reg_setup(void)
{
	setup_mac_reg();
    setup_pse_IQ_reservation();

    return;
}

void fe_vip_setup(void)
{
	unsigned int value;

	write_reg_word(FE_VIP_PATN(0),0x01);/* IP ->ICMP(0x01) */
	value = (0x01<<VIP_EN_CPU_OFFSET)|(0x02<< VIP_EN_TYPE_OFFSET)|(0x01<<VIP_EN_ENABLE_OFFSET);
	write_reg_word(FE_VIP_EN(0),value);
	
	write_reg_word(FE_VIP_PATN(1),0x0806);/* Ether ->ARP(0x0806) */
	value = (0x01<<VIP_EN_CPU_OFFSET)|(0x0<< VIP_EN_TYPE_OFFSET)|(0x01<<VIP_EN_ENABLE_OFFSET);
	write_reg_word(FE_VIP_EN(1),value);

	write_reg_word(FE_VIP_PATN(2),0x02);/* IP ->IGMP(0x02) */
	value = (0x01<<VIP_EN_CPU_OFFSET)|(0x02<< VIP_EN_TYPE_OFFSET)|(0x01<<VIP_EN_ENABLE_OFFSET);
	write_reg_word(FE_VIP_EN(2),value);

	write_reg_word(FE_VIP_PATN(3),0x8863);/* Ether ->PPP(0x8863) */
	value = (0x01<<VIP_EN_CPU_OFFSET)|(0x0<< VIP_EN_TYPE_OFFSET)|(0x01<<VIP_EN_ENABLE_OFFSET);
	write_reg_word(FE_VIP_EN(3),value);

	write_reg_word(FE_VIP_PATN(4),0xc021);/* PPP ->LCP(0xc021) */
	value = (0x01<<VIP_EN_CPU_OFFSET)|(0x01<< VIP_EN_TYPE_OFFSET)|(0x01<<VIP_EN_ENABLE_OFFSET);
	write_reg_word(FE_VIP_EN(4),value);
	
	write_reg_word(FE_VIP_PATN(5),0x3a);/* IPv6 ->ICMPv6(0x3a) */
	value = (0x01<<VIP_EN_CPU_OFFSET)|(0x02<< VIP_EN_TYPE_OFFSET)|(0x01<<VIP_EN_ENABLE_OFFSET);
	write_reg_word(FE_VIP_EN(5),value);

}

struct ecnt_hook_ops ecnt_fe_api_op = {
	.name = "fe_api_hook",
	.is_execute = 1,
	.hookfn = ecnt_fe_api_hook,
	.maintype = ECNT_FE,
	.subtype = ECNT_FE_API,
	.priority = 1
};

static int __init fe_init(void)
{
    unsigned int reg;
	unsigned int regVal;
	int ret;

	if(ecnt_register_hook(&ecnt_fe_api_op)) {
		printk("ecnt_dev_fe_api_op register fail\n");
		return 0 ;
	}

	fe_reset();
	fe_reg_setup();

	fe_vip_setup();

	/* set rx queue for lan->wifi traffic to Q1L */
	/* CDMA1_CRSN_QSEL */
	regVal = read_reg_word(CDMA1_CRSN_QSEL);
	regVal &= ~(CDMA1_CRSN_QSEL_CPU_REASON_16_MASK);
	regVal |= (CDMA1_CRSN_QSEL_Q1L<<CDMA1_CRSN_QSEL_CPU_REASON_16_OFFSET);
	write_reg_word(CDMA1_CRSN_QSEL, regVal);

#ifdef TCSUPPORT_FTTDP_V2
/* let FE CPORT add 4-byte CRC for packets from GSW to FE,
 * so that, packets from PTM_LAN or GSW can both have CRC,
 * which will be stripped at GDM1. */
    reg = read_reg_word(FE_CPORT_CFG);
    reg &= ~(1<<30);
    write_reg_word(FE_CPORT_CFG, reg);

/* disable STAG indication for FE, because packets from GSW
 * and PTM_LAN should both don't have STAG */
    reg = read_reg_word(GDMA1_VLAN_CHECK);
    reg &= ~(1<<0);
    write_reg_word(GDMA1_VLAN_CHECK, reg);

/* set LAN as Arbitor mode, so that PTM and GSW can work together, 
 * set WAN as GPON mode */
    reg = read_reg_word(0xbfb00070);
    reg &= ~((0x7<<0)|(0x3<<4));
    reg |= (0<<0)|(0x3<<4);
    write_reg_word(0xbfb00070, reg);
#endif
    reg = read_reg_word(FE_CPORT_CFG);
    reg = (reg & (~(0x3<<24))) | (0x2 <<24);
    write_reg_word(FE_CPORT_CFG, reg);

	ret = register_chrdev(FE_MAJOR, "/dev/fe", &fe_fops);
	
	if (ret < 0) {
		printk(KERN_WARNING "pcie: can't get major %d\n", FE_MAJOR);
		return ret;
	}
	create_proc_read_entry("tc3162/fe_reg", 0, NULL, fe_reg_dump_proc, NULL);

    create_proc_read_entry("tc3162/fe_debug_reg", 0, NULL, fe_debug_reg_dump_proc, NULL);
    
	if(isEN7526c){
		/*fe set CDMA1 remove stag enable*/
		regVal = read_reg_word(0xbfb50400);
		regVal |= 0x3;/*bfb50400[0] [1] set to 1*/
		write_reg_word(0xbfb50400,regVal);
	}
	return 0;
}

static void __exit fe_exit(void) 
{
	remove_proc_entry("tc3162/fe_reg", 0);
	
	return;
}


/* Register startup/shutdown routines */
module_init(fe_init);
module_exit(fe_exit);
MODULE_LICENSE("Proprietary");

