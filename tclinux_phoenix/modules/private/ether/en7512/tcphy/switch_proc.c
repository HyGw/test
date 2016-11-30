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
#include "switch_proc.h"
    
    
#ifdef TCSUPPORT_AUTOBENCH
#include "../auto_bench/autobench.h"
#endif
    
        
#include "tcconsole.h"
#include "../eth_proc.h"
        
#ifdef LOOPBACK_SUPPORT
#include "../fe_verify.h"
#endif
    
#include "phy_api.h"

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
extern uint8 macInitialized;
extern macAdapter_t *mac_p;
extern macAdapter_t *mac_wan_p;
extern int wan_port_id;
extern int g_port_reverse;
extern macAdapter_t *mac_p;
extern macAdapter_t *mac_wan_p;
extern uint8 use_ext_switch;
#if defined(TCSUPPORT_FH_ENV) && defined(TCSUPPORT_CPU_EN7521) && defined(TCSUPPORT_WAN_ETHER)
extern int eth1_active_op(int active);
#endif

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


int eth_stats_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int index = 0;
	off_t pos = 0;
	off_t begin = 0;

	if (!macInitialized) {
		*eof = 1;
		return 0;
	}


	if(isMT7520G || isMT7525G)
	{
		index += sprintf(buf+index, "inOctets              = 0x%08lx, ",
			ext_cal_cnt(EXT_GSW_RX_OCL(0)));
		CHK_BUF();

		index += sprintf(buf+index, "inUnicastPkts         = 0x%08lx\n",
			ext_cal_cnt(EXT_GSW_RX_UNIC(0)));
		CHK_BUF();

		index += sprintf(buf+index, "inMulticastPkts       = 0x%08lx, ",
			ext_cal_cnt(EXT_GSW_RX_MULC(0)));
		CHK_BUF();

		index += sprintf(buf+index, "inDiscards            = 0x%08lx\n",
			ext_cal_cnt(EXT_GSW_RX_DROC(0)));
		CHK_BUF();



		index += sprintf(buf+index, "outOctets             = 0x%08lx\n",
			ext_cal_cnt(EXT_GSW_TX_OCL(0)));
		CHK_BUF();

		index += sprintf(buf+index, "outUnicastPkts        = 0x%08lx, ",
			ext_cal_cnt(EXT_GSW_TX_UNIC(0)));
		CHK_BUF();

		//use port 6 to count multicast, include etherwan.
		index += sprintf(buf+index, "outMulticastPkts      = 0x%08lx\n",
			ext_cal_cnt(EXT_GSW_RX_MULC(6))); 
		CHK_BUF();
		
		index += sprintf(buf+index, "outDiscards           = 0x%08lx, ",
			ext_cal_cnt(EXT_GSW_TX_DROC(0)));
		CHK_BUF();
		
		index += sprintf(buf+index, "outErrors             = 0x%08lx\n",
			ext_cal_cnt(EXT_GSW_TX_COLC(0)));
		CHK_BUF();
	}
	else
	{
		index += sprintf(buf+index, "inOctets              = 0x%08lx, ",
			calcnt(GSW_RX_OCL(6)));
		CHK_BUF();

		index += sprintf(buf+index, "inUnicastPkts         = 0x%08lx\n",
			calcnt(GSW_RX_UNIC(6)));
		CHK_BUF();

		index += sprintf(buf+index, "inMulticastPkts       = 0x%08lx, ",
			calcnt(GSW_RX_MULC(6)));
		CHK_BUF();
		
		index += sprintf(buf+index, "inDiscards            = 0x%08lx\n",
			calcnt(GSW_RX_DROC(6)));
		CHK_BUF();

		index += sprintf(buf+index, "inErrors              = 0x%08lx, ",
			calcnt(GSW_RX_ALIGE(6)) + calcnt(GSW_RX_CRC(6)) + 
			calcnt(GSW_RX_RUNT(6)) + calcnt(GSW_RX_FRGE(6)) + 
			calcnt(GSW_RX_LONG(6)));
		CHK_BUF();
		
		index += sprintf(buf+index, "outOctets             = 0x%08lx\n",
			calcnt(GSW_TX_OCL(6)));
		CHK_BUF();

		index += sprintf(buf+index, "outUnicastPkts        = 0x%08lx, ",
			calcnt(GSW_TX_UNIC(6)));
		CHK_BUF();

		//use port 6 to count multicast, include etherwan.
		index += sprintf(buf+index, "outMulticastPkts      = 0x%08lx\n",
			calcnt(GSW_RX_MULC(6))); 
		CHK_BUF();

		index += sprintf(buf+index, "outDiscards           = 0x%08lx, ",
			calcnt(GSW_TX_DROC(6)));
		CHK_BUF();
		
		index += sprintf(buf+index, "outErrors             = 0x%08lx\n",
			calcnt(GSW_TX_COLC(6)));
		CHK_BUF();
	}
	
	index += sprintf(buf+index, "PDMA inOctets         = 0x%08lx, ",
 		mac_p->macStat.MIB_II.inOctets);
	CHK_BUF();
	index += sprintf(buf+index, "PDMA inUnicastPkts    = 0x%08lx\n",
 		mac_p->macStat.MIB_II.inUnicastPkts);
	CHK_BUF();

	index += sprintf(buf+index, "PDMA inMulticastPkts  = 0x%08lx, ",
 		mac_p->macStat.MIB_II.inMulticastPkts);
	CHK_BUF();
	index += sprintf(buf+index, "PDMA inBroadcastPkts  = 0x%08lx\n",
		mac_p->macStat.MIB_II.inBroadcastPkts);
	CHK_BUF();
	index += sprintf(buf+index, "PDMA inDiscards       = 0x%08lx, ",
 		mac_p->macStat.MIB_II.inDiscards);
	CHK_BUF();

	index += sprintf(buf+index, "PDMA inErrors         = 0x%08lx\n",
 		mac_p->macStat.MIB_II.inErrors);
	CHK_BUF();
	index += sprintf(buf+index, "PDMA outOctets        = 0x%08lx, ",
 		mac_p->macStat.MIB_II.outOctets);
	CHK_BUF();

	index += sprintf(buf+index, "PDMA outUnicastPkts   = 0x%08lx\n",
 		mac_p->macStat.MIB_II.outUnicastPkts);
	CHK_BUF();
	index += sprintf(buf+index, "PDMA outMulticastPkts = 0x%08lx, ",
 		mac_p->macStat.MIB_II.outMulticastPkts);
	CHK_BUF();
	index += sprintf(buf+index, "PDMA outBroadcastPkts = 0x%08lx\n",
 		mac_p->macStat.MIB_II.outBroadcastPkts);
	CHK_BUF();

	index += sprintf(buf+index, "PDMA outDiscards      = 0x%08lx, ",
 		mac_p->macStat.MIB_II.outDiscards);
	CHK_BUF();
	index += sprintf(buf+index, "PDMA outErrors        = 0x%08lx\n",
 		mac_p->macStat.MIB_II.outErrors);
	CHK_BUF();
	index += sprintf(buf+index, "\n[ Statistics Display ]\n");
	CHK_BUF();
	index += sprintf(buf+index, "txJabberTimeCnt       = 0x%08lx  ",
 		mac_p->macStat.inSilicon.txJabberTimeCnt);
	CHK_BUF();
	index += sprintf(buf+index, "txLossOfCarrierCnt    = 0x%08lx\n",
 		mac_p->macStat.inSilicon.txLossOfCarrierCnt);
	CHK_BUF();
	index += sprintf(buf+index, "txNoCarrierCnt        = 0x%08lx  ",
 		mac_p->macStat.inSilicon.txNoCarrierCnt);
	CHK_BUF();
	index += sprintf(buf+index, "txLateCollisionCnt    = 0x%08lx\n",
 		mac_p->macStat.inSilicon.txLateCollisionCnt);
	CHK_BUF();
	index += sprintf(buf+index, "txExCollisionCnt      = 0x%08lx  ",
 		mac_p->macStat.inSilicon.txExCollisionCnt);
	CHK_BUF();
	index += sprintf(buf+index, "txHeartbeatFailCnt    = 0x%08lx\n",
 		mac_p->macStat.inSilicon.txHeartbeatFailCnt);
	CHK_BUF();
	index += sprintf(buf+index, "txCollisionCnt        = 0x%08lx  ",
 		mac_p->macStat.inSilicon.txCollisionCnt);
	CHK_BUF();
	index += sprintf(buf+index, "txExDeferralCnt       = 0x%08lx\n",
 		mac_p->macStat.inSilicon.txExDeferralCnt);
	CHK_BUF();
	index += sprintf(buf+index, "txUnderRunCnt         = 0x%08lx  ",
 		mac_p->macStat.inSilicon.txUnderRunCnt);
	CHK_BUF();

	index += sprintf(buf+index, "rxAlignErr            = 0x%08lx\n",
 		mac_p->macStat.inSilicon.rxAlignErr);
	CHK_BUF();
	index += sprintf(buf+index, "rxDribblingErr        = 0x%08lx  ",
 		mac_p->macStat.inSilicon.rxDribblingErr);
	CHK_BUF();
	index += sprintf(buf+index, "rxSymbolErr           = 0x%08lx\n",
 		mac_p->macStat.inSilicon.rxSymbolErr);
	CHK_BUF();
	index += sprintf(buf+index, "rxMiiErr              = 0x%08lx  ",
 		mac_p->macStat.inSilicon.rxMiiErr);
	CHK_BUF();
	index += sprintf(buf+index, "rxCollisionErr        = 0x%08lx\n",
 		mac_p->macStat.inSilicon.rxCollisionErr);
	CHK_BUF();
	index += sprintf(buf+index, "rxCrcErr              = 0x%08lx  ",
 		mac_p->macStat.inSilicon.rxCrcErr);
	CHK_BUF();
	index += sprintf(buf+index, "rxEtherFrameLengthErr = 0x%08lx\n",
 		mac_p->macStat.inSilicon.rxEtherFrameLengthErr);
	CHK_BUF();
	index += sprintf(buf+index, "rx802p3FrameLengthErr = 0x%08lx  ",
 		mac_p->macStat.inSilicon.rx802p3FrameLengthErr);
	CHK_BUF();
	index += sprintf(buf+index, "rxPktIPChkSumErr      = 0x%08lx\n",
 		mac_p->macStat.inSilicon.rxPktIPChkSumErr);
	CHK_BUF();
	index += sprintf(buf+index, "rxRuntErr             = 0x%08lx  ",
 		mac_p->macStat.inSilicon.rxRuntErr);
	CHK_BUF();
	index += sprintf(buf+index, "rxLongErr             = 0x%08lx\n",
 		mac_p->macStat.inSilicon.rxLongErr);
	CHK_BUF();

	index += sprintf(buf+index, "\n[ Extra information ]\n");
	CHK_BUF();
 	index += sprintf(buf+index, "Rx Enqueued cnt       = 0x%08lx  ",
 		mac_p->macStat.inSilicon.rxEnQueueNum);
	CHK_BUF();
  	index += sprintf(buf+index, "Tx Enqueued cnt       = 0x%08lx\n",
 		mac_p->macStat.inSilicon.txEnQueueNum);		
	CHK_BUF();
  	index += sprintf(buf+index, "Rx Dequeued cnt       = 0x%08lx  ",
 		mac_p->macStat.inSilicon.rxDeQueueNum);
	CHK_BUF();
  	index += sprintf(buf+index, "Tx Dequeued cnt       = 0x%08lx\n",
 		mac_p->macStat.inSilicon.txDeQueueNum);
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


int eth_stats_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[32];

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';

	if (!macInitialized) {
		return 0;
	}

	memset(&mac_p->macStat.MIB_II, 0, sizeof(macMIB_II_t));
	memset(&mac_p->macStat.inSilicon, 0, sizeof(inSiliconStat_t));

	return count;
}



#if defined(TCSUPPORT_WAN_ETHER)
int eth1_stats_read_proc(char *buf, char **start, off_t off, int count,
    int *eof, void *data)
{
    int index = 0;
    off_t pos = 0;
    off_t begin = 0;

    if (!macInitialized) {
        *eof = 1;
        return 0;
    }

#if defined(TCSUPPORT_FH_ENV) && defined(TCSUPPORT_CPU_EN7521)
    index += sprintf(buf+index, "ETH1 Status = %s\n", 
        -1 == wan_port_id ? "Deactived" : "Actived");
    CHK_BUF();
#endif

    index += sprintf(buf+index, "inOctets              = 0x%08lx, ", 
        switch_reg_read(GSW_RX_OCL(wan_port_id)));
    CHK_BUF();
    
    index += sprintf(buf+index, "inUnicastPkts         = 0x%08lx\n", 
        switch_reg_read(GSW_RX_UNIC(wan_port_id)));
    CHK_BUF();

    index += sprintf(buf+index, "inMulticastPkts       = 0x%08lx, ", 
        switch_reg_read(GSW_RX_MULC(wan_port_id)));
    CHK_BUF();
    
    index += sprintf(buf+index, "inDiscards            = 0x%08lx\n", 
        switch_reg_read(GSW_RX_DROC(wan_port_id)));
    CHK_BUF();

    index += sprintf(buf+index, "inErrors              = 0x%08lx, ", 
        switch_reg_read(GSW_RX_ALIGE(wan_port_id)) + 
        switch_reg_read(GSW_RX_CRC(wan_port_id)) + 
        switch_reg_read(GSW_RX_RUNT(wan_port_id)) + 
        switch_reg_read(GSW_RX_FRGE(wan_port_id)) + 
        switch_reg_read(GSW_RX_LONG(wan_port_id)));
    CHK_BUF();
    
    index += sprintf(buf+index, "outOctets             = 0x%08lx\n", 
        switch_reg_read(GSW_TX_OCL(wan_port_id)));
    CHK_BUF();

    index += sprintf(buf+index, "outUnicastPkts        = 0x%08lx, ", 
        switch_reg_read(GSW_TX_UNIC(wan_port_id)));
    CHK_BUF();
    
    index += sprintf(buf+index, "outMulticastPkts      = 0x%08lx\n", 
        switch_reg_read(GSW_TX_MULC(wan_port_id))); 

    index += sprintf(buf+index, "outDiscards           = 0x%08lx, ", 
        switch_reg_read(GSW_TX_DROC(wan_port_id)));
    CHK_BUF();
    index += sprintf(buf+index, "outErrors             = 0x%08lx\n", 
        switch_reg_read(GSW_TX_COLC(wan_port_id)));
    CHK_BUF();

    index += sprintf(buf+index, "PDMA inOctets         = 0x%08lx, ", 
        mac_wan_p->macStat.MIB_II.inOctets);
    CHK_BUF();
    index += sprintf(buf+index, "PDMA inUnicastPkts    = 0x%08lx\n", 
        mac_wan_p->macStat.MIB_II.inUnicastPkts);
    CHK_BUF();

    index += sprintf(buf+index, "PDMA inMulticastPkts  = 0x%08lx, ", 
        mac_wan_p->macStat.MIB_II.inMulticastPkts);
    CHK_BUF();

    index += sprintf(buf+index, "PDMA inBoradcastPkts  = 0x%08lx\n", 
        mac_wan_p->macStat.MIB_II.inBroadcastPkts);
    CHK_BUF();
    index += sprintf(buf+index, "PDMA inDiscards       = 0x%08lx, ", 
        mac_wan_p->macStat.MIB_II.inDiscards);
    CHK_BUF();

    index += sprintf(buf+index, "PDMA inErrors         = 0x%08lx\n", 
        mac_wan_p->macStat.MIB_II.inErrors);
    CHK_BUF();
    index += sprintf(buf+index, "PDMA outOctets        = 0x%08lx, ", 
     mac_wan_p->macStat.MIB_II.outOctets);
    CHK_BUF();

    index += sprintf(buf+index, "PDMA outUnicastPkts   = 0x%08lx\n", 
        mac_wan_p->macStat.MIB_II.outUnicastPkts);
    CHK_BUF();
    index += sprintf(buf+index, "PDMA outMulticastPkts = 0x%08lx, ", 
        mac_wan_p->macStat.MIB_II.outMulticastPkts);
    CHK_BUF();
    index += sprintf(buf+index, "PDMA outBroadcastPkts = 0x%08lx\n", 
        mac_wan_p->macStat.MIB_II.outBroadcastPkts);
    CHK_BUF();

    index += sprintf(buf+index, "PDMA outDiscards      = 0x%08lx, ", 
        mac_wan_p->macStat.MIB_II.outDiscards);
    CHK_BUF();
    index += sprintf(buf+index, "PDMA outErrors        = 0x%08lx\n", 
        mac_wan_p->macStat.MIB_II.outErrors);
    CHK_BUF();
    
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

int eth1_stats_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[32];
	char cmd[12] = {0}, val[20] = {0};

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';

	if (!macInitialized) {
		return 0;
	}

#if defined(TCSUPPORT_FH_ENV) && defined(TCSUPPORT_CPU_EN7521)
	sscanf(val_string, "%s %s", cmd, val);

	if ( 0 == strcmp("Enable", cmd) )
	{
		if ( 0 == strcmp("Yes", val) )
		{
			eth1_active_op(1);
		}
	}
#endif

	memset(&mac_wan_p->macStat.MIB_II, 0, sizeof(macMIB_II_t));
	memset(&mac_wan_p->macStat.inSilicon, 0, sizeof(inSiliconStat_t));

	return count;
}
#endif


int gsw_stats_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int index = 0;
	off_t pos = 0;
	off_t begin = 0;
	int port;

	if (!macInitialized) {
		*eof = 1;
		return 0;
	}
    
	for (port = 0; port <= 6; port++) {
		index += sprintf(buf+index, "[ Port %d ]\n", port);
		CHK_BUF();
		index += sprintf(buf+index, "Rx Unicase Pkts        = 0x%08lx, ",switch_reg_read(GSW_RX_UNIC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx Multicast Pkts      = 0x%08lx\n",switch_reg_read(GSW_RX_MULC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx Broadcast Pkts      = 0x%08lx, ",switch_reg_read(GSW_RX_BROC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx Align Error         = 0x%08lx\n",switch_reg_read(GSW_RX_ALIGE(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx CRC Error           = 0x%08lx, ",switch_reg_read(GSW_RX_CRC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx Under Size Pkts     = 0x%08lx\n",switch_reg_read(GSW_RX_RUNT(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx Fragment Error      = 0x%08lx, ",switch_reg_read(GSW_RX_FRGE(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx Over Size Pkts      = 0x%08lx\n",switch_reg_read(GSW_RX_LONG(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx Jabber Error        = 0x%08lx, ",switch_reg_read(GSW_RX_JABE(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx Pause Pkts          = 0x%08lx\n",switch_reg_read(GSW_RX_PAUC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx Drop Pkts           = 0x%08lx, ",switch_reg_read(GSW_RX_DROC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx ING Drop Pkts       = 0x%08lx\n",switch_reg_read(GSW_RX_INGC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx ARL Drop Pkts       = 0x%08lx, ",switch_reg_read(GSW_RX_ARLC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx FILTER Drop Pkts    = 0x%08lx\n",switch_reg_read(GSW_RX_FILC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Unicase Pkts        = 0x%08lx, ",switch_reg_read(GSW_TX_UNIC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Multicast Pkts      = 0x%08lx\n",switch_reg_read(GSW_TX_MULC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Broadcast Pkts      = 0x%08lx, ",switch_reg_read(GSW_TX_BROC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Collision           = 0x%08lx\n",switch_reg_read(GSW_TX_COLC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Single Collision    = 0x%08lx, ",switch_reg_read(GSW_TX_SCOLC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Multiple Collision  = 0x%08lx\n",switch_reg_read(GSW_TX_MCOLC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Defer               = 0x%08lx, ",switch_reg_read(GSW_TX_DEFC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Late Collision      = 0x%08lx\n",switch_reg_read(GSW_TX_LCOLC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx eXcessive Collision = 0x%08lx, ",switch_reg_read(GSW_TX_ECOLC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Pause Pkts          = 0x%08lx\n",switch_reg_read(GSW_TX_PAUC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Drop Pkts           = 0x%08lx\n",switch_reg_read(GSW_TX_DROC(port)));
		CHK_BUF();

	}
	
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

int gsw_stats_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[32];

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;
	
	val_string[count] = '\0';

	return count;
}


/* wplin addded 20120703 */
int gsw_mibN_read_proc(char *buf, char **start, off_t off, int count,
    int *eof, void *data, uint portId)
{
	int index = 0;
	off_t pos = 0;
	off_t begin = 0;
	int port;

	if (!macInitialized) {
		*eof = 1;
		return 0;
	}


	port = portId;
	{
		index += sprintf(buf+index, "[ Port %d ]\n", port);
		CHK_BUF();
		if (use_ext_switch) {
			index += sprintf(buf+index, "External switch  \n");
			CHK_BUF();
			index += sprintf(buf+index, "Rx Unicase Pkts        = 0x%08x, ", gswPbusRead(EXT_GSW_RX_UNIC(port)));
			CHK_BUF();
			index += sprintf(buf+index, "Rx Multicast Pkts      = 0x%08x\n", gswPbusRead(EXT_GSW_RX_MULC(port)));
			CHK_BUF();
			index += sprintf(buf+index, "Rx Broadcast Pkts      = 0x%08x, ", gswPbusRead(EXT_GSW_RX_BROC(port)));
			CHK_BUF();
			index += sprintf(buf+index, "Rx Align Error         = 0x%08x\n", gswPbusRead(EXT_GSW_RX_ALIGE(port)));
			CHK_BUF();
			index += sprintf(buf+index, "Rx CRC Error           = 0x%08x, ", gswPbusRead(EXT_GSW_RX_CRC(port)));
			CHK_BUF();
			index += sprintf(buf+index, "Rx Under Size Pkts     = 0x%08x\n", gswPbusRead(EXT_GSW_RX_RUNT(port)));
			CHK_BUF();
			index += sprintf(buf+index, "Rx Fragment Error      = 0x%08x, ", gswPbusRead(EXT_GSW_RX_FRGE(port)));
			CHK_BUF();
			index += sprintf(buf+index, "Rx Over Size Pkts      = 0x%08x\n", gswPbusRead(EXT_GSW_RX_LONG(port)));
			CHK_BUF();
			index += sprintf(buf+index, "Rx Jabber Error        = 0x%08x, ", gswPbusRead(EXT_GSW_RX_JABE(port)));
			CHK_BUF();
			index += sprintf(buf+index, "Rx Pause Pkts          = 0x%08x\n", gswPbusRead(EXT_GSW_RX_PAUC(port)));
			CHK_BUF();
			index += sprintf(buf+index, "Rx Drop Pkts           = 0x%08x, ", gswPbusRead(EXT_GSW_RX_DROC(port)));
			CHK_BUF();
			index += sprintf(buf+index, "Rx ING Drop Pkts       = 0x%08x\n", gswPbusRead(EXT_GSW_RX_INGC(port)));
			CHK_BUF();
			index += sprintf(buf+index, "Rx ARL Drop Pkts       = 0x%08x, ", gswPbusRead(EXT_GSW_RX_ARLC(port)));
			CHK_BUF();
			index += sprintf(buf+index, "Rx FILTER Drop Pkts    = 0x%08x\n", gswPbusRead(EXT_GSW_RX_FILC(port)));
			CHK_BUF();
			index += sprintf(buf+index, "Tx Unicase Pkts        = 0x%08x, ", gswPbusRead(EXT_GSW_TX_UNIC(port)));
			CHK_BUF();
			index += sprintf(buf+index, "Tx Multicast Pkts      = 0x%08x\n", gswPbusRead(EXT_GSW_TX_MULC(port)));
			CHK_BUF();
			index += sprintf(buf+index, "Tx Broadcast Pkts      = 0x%08x, ", gswPbusRead(EXT_GSW_TX_BROC(port)));
			CHK_BUF();
			index += sprintf(buf+index, "Tx Collision           = 0x%08x\n", gswPbusRead(EXT_GSW_TX_COLC(port)));
			CHK_BUF();
			index += sprintf(buf+index, "Tx Single Collision    = 0x%08x, ", gswPbusRead(EXT_GSW_TX_SCOLC(port)));
			CHK_BUF();
			index += sprintf(buf+index, "Tx Multiple Collision  = 0x%08x\n", gswPbusRead(EXT_GSW_TX_MCOLC(port)));
			CHK_BUF();
			index += sprintf(buf+index, "Tx Defer               = 0x%08x, ", gswPbusRead(EXT_GSW_TX_DEFC(port)));
			CHK_BUF();
			index += sprintf(buf+index, "Tx Late Collision      = 0x%08x\n", gswPbusRead(EXT_GSW_TX_LCOLC(port)));
			CHK_BUF();
			index += sprintf(buf+index, "Tx eXcessive Collision = 0x%08x, ", gswPbusRead(EXT_GSW_TX_ECOLC(port)));
			CHK_BUF();
			index += sprintf(buf+index, "Tx Pause Pkts          = 0x%08x\n", gswPbusRead(EXT_GSW_TX_PAUC(port)));
			CHK_BUF();
			index += sprintf(buf+index, "Tx Drop Pkts           = 0x%08x\n", gswPbusRead(EXT_GSW_TX_DROC(port)));
			CHK_BUF();			
		}
        index += sprintf(buf+index, "Internal switch  \n");
        CHK_BUF();
		index += sprintf(buf+index, "Rx Unicase Pkts        = 0x%08lx, ", read_reg_word(GSW_RX_UNIC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx Multicast Pkts      = 0x%08lx\n", read_reg_word(GSW_RX_MULC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx Broadcast Pkts      = 0x%08lx, ", read_reg_word(GSW_RX_BROC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx Align Error         = 0x%08lx\n", read_reg_word(GSW_RX_ALIGE(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx CRC Error           = 0x%08lx, ", read_reg_word(GSW_RX_CRC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx Under Size Pkts     = 0x%08lx\n", read_reg_word(GSW_RX_RUNT(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx Fragment Error      = 0x%08lx, ", read_reg_word(GSW_RX_FRGE(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx Over Size PPkts     = 0x%08lx\n", read_reg_word(GSW_RX_LONG(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx Jabber Error        = 0x%08lx, ", read_reg_word(GSW_RX_JABE(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx Pause Pkts          = 0x%08lx\n", read_reg_word(GSW_RX_PAUC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx Drop Pkts           = 0x%08lx, ", read_reg_word(GSW_RX_DROC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx ING Drop Pkts       = 0x%08lx\n", read_reg_word(GSW_RX_INGC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx ARL Drop Pkts       = 0x%08lx, ", read_reg_word(GSW_RX_ARLC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx FILTER Drop Pkts    = 0x%08lx\n", read_reg_word(GSW_RX_FILC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Unicase Pkts        = 0x%08lx, ", read_reg_word(GSW_TX_UNIC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Multicast Pkts      = 0x%08lx\n", read_reg_word(GSW_TX_MULC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Broadcast Pkts      = 0x%08lx, ", read_reg_word(GSW_TX_BROC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Collision           = 0x%08lx\n", read_reg_word(GSW_TX_COLC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Single Collision    = 0x%08lx, ", read_reg_word(GSW_TX_SCOLC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Multiple Collision  = 0x%08lx\n", read_reg_word(GSW_TX_MCOLC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Deffer              = 0x%08lx, ", read_reg_word(GSW_TX_DEFC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Late Collision      = 0x%08lx\n", read_reg_word(GSW_TX_LCOLC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx eXcessive Collision = 0x%08lx, ", read_reg_word(GSW_TX_ECOLC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Pause Pkts          = 0x%08lx\n", read_reg_word(GSW_TX_PAUC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Drop Pkts           = 0x%08lx\n", read_reg_word(GSW_TX_DROC(port)));
		CHK_BUF();
    }
	index += sprintf(buf+index, "------\n");
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

int gsw_mib0_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
	return gsw_mibN_read_proc(buf, start, off, count, eof, data, 0);
}

int gsw_mib1_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
	return gsw_mibN_read_proc(buf, start, off, count, eof, data, 1);
}
int gsw_mib2_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
	return gsw_mibN_read_proc(buf, start, off, count, eof, data, 2);
}
int gsw_mib3_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
	return gsw_mibN_read_proc(buf, start, off, count, eof, data, 3);
}
int gsw_mib4_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
	return gsw_mibN_read_proc(buf, start, off, count, eof, data, 4);
}
int gsw_mib5_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
	return gsw_mibN_read_proc(buf, start, off, count, eof, data, 5);
}
int gsw_mib6_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
	return gsw_mibN_read_proc(buf, start, off, count, eof, data, 6);
}

uint16 print_link_status(char *buf,uint16 index,uint32 reg)
{
	int speed;
    if (!(reg & MAC_LINK_STS)) {
        index += sprintf(buf+index, "Down\n");
        return index;
    }

    speed = (reg & MAC_SPD_STS) >> MAC_SPD_STS_SHIFT;
    if (speed == 3)
        index += sprintf(buf+index, "invalid(10G)/");
    else if (speed == PN_SPEED_1000M)
        index += sprintf(buf+index, "1000M/");
    else if (speed == PN_SPEED_100M)
        index += sprintf(buf+index, "100M/");
    else 
        index += sprintf(buf+index, "10M/");
    
    if (reg & MAC_DPX_STS)
        index += sprintf(buf+index, "Full Duplex");
    else 
        index += sprintf(buf+index, "Half Duplex");
    
    if (reg & (TX_FC_STS | RX_FC_STS)) {
        index += sprintf(buf+index, " FC:");
        if (reg & TX_FC_STS) 
            index += sprintf(buf+index, " TX");
        if (reg & RX_FC_STS) 
            index += sprintf(buf+index, " RX");
    }
    
    if (reg & EEE100_STS) 
        index += sprintf(buf+index, " EEE100");
    if (reg & EEE1G_STS) 
        index += sprintf(buf+index, " EEE1G");
    
    index += sprintf(buf+index, "\n");
    return index;
}

int getGSWLinkSt(char *buf)
{
	uint16 index = 0;
	
	uint32 reg;
	int port;
#if defined(TCSUPPORT_CUC_C5_2P)
	int tempPort = 0;
#endif

	if (!macInitialized) {
		return index;
	}
    index += sprintf(buf+index, "\n%s switch: \n", use_ext_switch?"External":"Internal");
	for (port = 0; port <= 6; port++) {
		index += sprintf(buf+index, "Port%d : ", port);
		
		if (g_port_reverse == 1 && port <= 3) //only care lan port, by sun.zhu
			reg = switch_reg_read(GSW_PMSR(3 - port));
		else{
#if defined(TCSUPPORT_CUC_C5_2P)
			if(port == 4) tempPort = 1;
			else if(port == 1) tempPort = 2;
			else if(port == 2) tempPort = 3;
			else if(port == 3) tempPort = 4;
			reg = switch_reg_read(GSW_PMSR(tempPort));
#else
			reg = switch_reg_read(GSW_PMSR(port));
#endif
        }
        index = print_link_status(buf, index, reg);
        
	}
	if(use_ext_switch){
        index += sprintf(buf+index, "\n Internal switch: \n");
        for (port = 5; port <= 6; port++) {
            index += sprintf(buf+index, "Port%d : ", port);
            reg = read_reg_word(GSW_PMSR(port));
            index = print_link_status(buf, index, reg);
        }
    }
	return index;
}

int gsw_link_st_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getGSWLinkSt(buf);
	if (len <= off+count) 
		*eof = 1;
	*start = buf + off;
	len -= off;
	if (len>count) 
		len = count;
	if (len<0) 
		len = 0;
	return len;
}


