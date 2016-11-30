
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

/************************************************************************
*                  I N C L U D E S
*************************************************************************
*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <libapi_lib_utility.h>
#include <libapi_lib_vdslmgr.h>

/************************************************************************
*                  D E F I N E S   &   C O N S T A N T S
*************************************************************************
*/


/************************************************************************
*                  M A C R O S
*************************************************************************
*/
#define BONDING_FOLW_CTL_ENABLE      (0x1)
#define BONDING_FOLW_CTL_DISABLE     (0x0)
/*****************************************************************************
 * Master Bonding Registers 
 * -(BONDING_TXPKTCFG) Operator 
 * for bonding tx fragmentation algrithm to improve performace
 *         bfb6_f0bc[8] = 0 :use old fragmentation algrithm
 *         bfb6_f0bc[8] = 1 :use new fragmentation algrithm
 *****************************************************************************/
#define BONDING_TX_FRAG_NEW_ALG      (0x1)
#define BONDING_TX_FRAG_OLD_ALG      (0x0)

#define TO_LINE0            (0)
#define TO_LINE1            (1)
#define TO_BOTH_LINES       (2)
#define TO_NO_BONDING       (3)
#define BONDING_LINE0_MODE  (6)
#define BONDING_LINE1_MODE  (5)
#define BONDING_MODE        (7)

#define MASTER_MODE (0)

/************************************************************************
*                  D A T A	 T Y P E S
*************************************************************************
*/

/************************************************************************
*                  E X T E R N A L	 D A T A   D E C L A R A T I O N S
*************************************************************************
*/



/************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
*************************************************************************
*/
int DobondingCmd(int argc, char **argv, void *p);  
static int DoBondingSet(int argc, char **argv, void *p);
static int DoBondingGet(int argc, char **argv, void *p);

static int DoSetFlowCtl(int argc, char **argv, void *p);
static int DoSetFragment(int argc, char **argv, void *p);  
static int DoSetLinePath(int argc, char **argv, void *p);
static int DoSetMacReset(int argc, char **argv, void *p);

static int DoGetReg(int argc, char **argv, void *p);
static int DoGetFlowCtl(int argc, char **argv, void *p);
static int DoGetFragment(int argc, char **argv, void *p);  
static int DoGetLinePath(int argc, char **argv, void *p);


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
static cmds_t BondingDbgCmds[] = {
    {"set",           DoBondingSet,        0x10,   0,  NULL},
    {"get",           DoBondingGet,        0x10,   0,  NULL},
    {NULL,            NULL,                0x10,   0,  NULL},
};

static cmds_t BondingSetCmds[] = {
    {"flowctl",       DoSetFlowCtl,         0x10,   1,  NULL},
    {"reset",         DoSetMacReset,        0x10,   0,  NULL},
    {"fragment",      DoSetFragment,        0x10,   0,  NULL},
    {"linepath",      DoSetLinePath,        0x10,   1,  NULL},
    {NULL,            NULL,                 0x10,   1,  NULL},
};

static cmds_t BondingGetCmds[] = {
    {"flowctl",       DoGetFlowCtl,         0x10,   0,  NULL},
    {"fragment",      DoGetFragment,        0x10,   0,  NULL},
    {"linepath",      DoGetLinePath,        0x10,   0,  NULL},
    {"mac_cnt",       DoGetReg,             0x10,   0,  NULL},
    {NULL,            NULL,                 0x10,   0,  NULL},
};


int DoBondingCmd(int argc, char **argv, void *p)  
{
    return subcmd(BondingDbgCmds, argc, argv, p);   
}


static int DoBondingSet(int argc, char **argv, void *p)
{
    return subcmd(BondingSetCmds, argc, argv, p);
}

static int DoBondingGet(int argc, char **argv, void *p)
{
    return subcmd(BondingGetCmds, argc, argv, p);
}


/******************************************************************************
 Descriptor:	It's used to set the bonding flow control.
 Input Args:	see BondingSetCmds.
 Ret Value:	success: VDSLMGR_SUCCESS
                       fail: VDSLMGR_FAIL
******************************************************************************/
static int DoSetFlowCtl(int argc, char **argv, void *p)
{
        ushort _enable;

#if API_CMD_VDSLMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(strncmp(argv[1], "enable", strlen("enable")) == 0) {
		_enable = BONDING_FOLW_CTL_ENABLE;
	} else if(strncmp(argv[1], "disable", strlen("disable")) == 0) {
		_enable = BONDING_FOLW_CTL_DISABLE;
	} else {
		printf("Input Error: must be \"enable\" or \"disable\".\n") ;
		return VDSLMGR_FAIL;
	}

	if(vdslmgr_lib_set_flowctl(_enable) == VDSLMGR_FAIL) {
		printf("Exec. Failed: Set the 'flowctl' command failed\n") ;
		return VDSLMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;
	
	return VDSLMGR_SUCCESS;
}
/******************************************************************************
 Descriptor:	It's used to reset the Bonding MAC.
 Input Args:	see PtmSetCmds.
 Ret Value:	success: VDSLMGR_SUCCESS
                       fail: VDSLMGR_FAIL
******************************************************************************/
static int DoSetMacReset(int argc, char **argv, void *p)
{
#if API_CMD_VDSLMGR_DBG
	printf("enter %s.\n", __func__);
#endif
        if(vdslmgr_lib_set_bonding_reset() == VDSLMGR_FAIL) {
                printf("Exec. Failed: Set the bonding reset command failed\n") ;
                return VDSLMGR_FAIL;
        }
                
                printf("Exec. Successful\n") ;

	return VDSLMGR_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to set the Bonding Fragment.
 Input Args:	see PtmSetCmds.
 Ret Value:	success: VDSLMGR_SUCCESS
                       fail: VDSLMGR_FAIL
******************************************************************************/
static int DoSetFragment(int argc, char **argv, void *p)
{
        ushort _enable;

#if API_CMD_VDSLMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	if(strncmp(argv[1], "new", strlen("new")) == 0) {
		_enable = BONDING_TX_FRAG_NEW_ALG;
	} else if(strncmp(argv[1], "old", strlen("old")) == 0) {
		_enable = BONDING_TX_FRAG_OLD_ALG;
	} else {
		printf("Input Error: must be \"old\" or \"new\".\n") ;
		return VDSLMGR_FAIL;
	}

	if(vdslmgr_lib_set_fragment(_enable) == VDSLMGR_FAIL) {
		printf("Exec. Failed: Set the 'fragment' command failed\n") ;
		return VDSLMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;
	
	return VDSLMGR_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to set the Line Path.
 Input Args:	see PtmSetCmds.
 Ret Value:	success: VDSLMGR_SUCCESS
                       fail: VDSLMGR_FAIL
******************************************************************************/
static int DoSetLinePath(int argc, char **argv, void *p)
{
#if API_CMD_VDSLMGR_DBG
        printf("enter %s.\n", __func__);
#endif

        ushort _mode = (atoi(argv[1]) & 0xf);
        if(_mode < 0 || _mode > 2)
        {
               printf("input error: input value :0~2, input error: %d\n", atoi(argv[1])) ;
               return VDSLMGR_FAIL;
        }
	/* PAF setting (Tx,Rx PAF for path0,1) PAF0[2:0] = 111 paf + L0 + L1 */
	if(vdslmgr_lib_set_line_path(_mode) == VDSLMGR_FAIL) {
		printf("Exec. Failed: Set the 'line path' command failed\n") ;
		return VDSLMGR_FAIL;
	}
	
	printf("Exec. Successful\n") ;
	return VDSLMGR_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to get the flow ctrl.
 Input Args:	see PtmGetCmds.
 Ret Value:	success: VDSLMGR_SUCCESS
                       fail: VDSLMGR_FAIL
******************************************************************************/
static int DoGetFlowCtl(int argc, char **argv, void *p)
{
#if API_CMD_VDSLMGR_DBG
                printf("enter %s.\n", __func__);
#endif

        ushort _enable; 
        if(vdslmgr_lib_get_flowctl(MASTER_MODE, &_enable) == VDSLMGR_FAIL) {
		printf("Exec. Failed: Get the 'Bonding flow control' command failed\n") ;
		return VDSLMGR_FAIL;
	}
        printf("Bonding flow control:%s\n", (_enable) ? "enable" : "disable");
        
        return VDSLMGR_SUCCESS;
}
/******************************************************************************
 Descriptor:	It's used to get the fragment.
 Input Args:	see PtmGetCmds.
 Ret Value:	success: VDSLMGR_SUCCESS
                       fail: VDSLMGR_FAIL
******************************************************************************/
static int DoGetFragment(int argc, char **argv, void *p)
{
#if API_CMD_VDSLMGR_DBG
        printf("enter %s.\n", __func__);
#endif

        ushort _enable; 
        if(vdslmgr_lib_get_fragment(&_enable) == VDSLMGR_FAIL) {
		printf("Exec. Failed: Get the 'Bonding fragment' command failed\n") ;
		return VDSLMGR_FAIL;
	}
        printf("Bonding fragment:%s\n", (_enable) ? "new" : "old");
        
        return VDSLMGR_SUCCESS;
}
/******************************************************************************
 Descriptor:	It's used to get the LinePath.
 Input Args:	see PtmGetCmds.
 Ret Value:	success: VDSLMGR_SUCCESS
                       fail: VDSLMGR_FAIL
******************************************************************************/
static int DoGetLinePath(int argc, char **argv, void *p)
{
#if API_CMD_VDSLMGR_DBG
        printf("enter %s.\n", __func__);
#endif
        ushort _mode; 
        if(vdslmgr_lib_get_linepath(&_mode) == VDSLMGR_FAIL) {
		printf("Exec. Failed: Get the 'Bonding fragment' command failed\n") ;
		return VDSLMGR_FAIL;
	}
        if (_mode == TO_LINE0)
		printf("Bonding only line0\n");
        else if (_mode == TO_LINE1)
		printf("Bonding only line1\n");
        else if (_mode == TO_BOTH_LINES)
		printf("Bonding both line\n");
        else
		printf("Bonding no line %d\n", _mode);
        
        return VDSLMGR_SUCCESS;
}
/******************************************************************************
 Descriptor:	It's used to get the bonding counter.
 Input Args:	see PtmGetCmds.
 Ret Value:	success: VDSLMGR_SUCCESS
                       fail: VDSLMGR_FAIL
******************************************************************************/
static int DoGetReg(int argc, char **argv, void *p)
{
         int i = 0;
         char tmp_buf[100];
         BondingReg_t _BondingReg;
         _procDump_t *dumpx;
#if API_CMD_VDSLMGR_DBG
        printf("enter %s.\n", __func__);
#endif
	_procDump_t bonding_tx_frag_dumps =
	    { "BONDING_TXFRAG%d   = 0x%08lx\%c", NULL };
	_procDump_t bonding_rx_frag_dumps =
	    { "BONDING_RXFRAG%d   = 0x%08lx\%c", NULL };
	_procDump_t bonding_rx_pkt_dumps =
	    { "BONDING_RXPKT%d    = 0x%08lx\%c", NULL};
         _procDump_t bonding_rx_crc_dumps =
             { "BONDING_RXERRPKT%d = 0x%08lx\%c", NULL};
         _procDump_t bonding_reg_dumps[] = {
	    { "\nBONDING_U2R_TX(M)  = 0x%08lx\t", &_BondingReg._BONDING_U2R_TX},
	    { "BONDING_R2U_RX(M)  = 0x%08lx\n", &_BondingReg._BONDING_R2U_RX},
             { "BONDING_R2U_TX(S)  = 0x%08lx\t", &_BondingReg.SLV_BONDING_R2U_TX},
	    { "BONDING_U2R_RX(S)  = 0x%08lx\n", &_BondingReg.SLV_BONDING_U2R_RX},	
             {NULL, 0}
	};

         if(vdslmgr_lib_get_bonding_counter(&_BondingReg) == VDSLMGR_FAIL) {
		printf("Exec. Failed: Get the 'Bonding counter' command failed\n") ;
		return VDSLMGR_FAIL;
	}
	/* bonding_reg_dumps */
         for (i = 0; i < TX_QUEUE_NUM; i++) {
		sprintf(tmp_buf, bonding_tx_frag_dumps.name, i,
			_BondingReg._BONDING_TXFRAG[i], '\t');
		printf("%s",tmp_buf);
		sprintf(tmp_buf, bonding_rx_frag_dumps.name, i,
			_BondingReg._BONDING_RXFRAG[i], '\n');
		printf("%s",tmp_buf);
		sprintf(tmp_buf, bonding_rx_pkt_dumps.name, i,
			_BondingReg._BONDING_RXFRAG[i], '\t');
		printf("%s",tmp_buf);
                  sprintf(tmp_buf, bonding_rx_crc_dumps.name, i,
			_BondingReg._BONDING_RXERRPKT[i], '\n');
		printf("%s",tmp_buf);
	}
         for (dumpx = bonding_reg_dumps; dumpx->name != NULL; dumpx++) {
                 sprintf(tmp_buf, dumpx->name, (*dumpx->reg_addr));
	        printf("%s",tmp_buf);
         }
         return VDSLMGR_SUCCESS;
}




