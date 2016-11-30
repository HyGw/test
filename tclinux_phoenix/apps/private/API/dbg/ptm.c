
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
int DoPtmCmd(int argc, char **argv, void *p);  
static int DoPtmSet(int argc, char **argv, void *p);
static int DoPtmGet(int argc, char **argv, void *p);
static int DoGetReg(int argc, char **argv, void *p);
static int DoGetTCCnt(int argc, char **argv, void *p);  
static int DoSetLvl(int argc, char **argv, void *p);
static int DoSetCntClr(int argc, char **argv, void *p); 
static int DoSetMacReset(int argc, char **argv, void *p);



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
static cmds_t PtmDbgCmds[] = {
    {"set",           DoPtmSet,        0x10,   0,  NULL},
    {"get",           DoPtmGet,        0x10,   0,  NULL},
    {NULL,            NULL,            0x10,   0,  NULL},
};

static cmds_t PtmSetCmds[] = {
    {"msglvl",        DoSetLvl,             0x10,   1,  NULL},
    {"reset",         DoSetMacReset,        0x10,   0,  NULL},
    {"clrcnt",        DoSetCntClr,          0x10,   0,  NULL},
    {NULL,            NULL,                 0x10,   0,  NULL},
};

static cmds_t PtmGetCmds[] = {
    {"mac_cnt",       DoGetReg,          0x10,   0,  NULL},
    {"tc_cnt",        DoGetTCCnt,        0x10,   0,  NULL},
    {NULL,            NULL,              0x10,   0,  NULL},
};


int DoPtmCmd(int argc, char **argv, void *p)  
{
    return subcmd(PtmDbgCmds, argc, argv, p);   
}


static int DoPtmSet(int argc, char **argv, void *p)
{
    return subcmd(PtmSetCmds, argc, argv, p);
}

static int DoPtmGet(int argc, char **argv, void *p)
{
    return subcmd(PtmGetCmds, argc, argv, p);
}


/******************************************************************************
 Descriptor:	It's used to set the PTM MAC debug level.
 Input Args:	see PtmSetCmds.
 Ret Value:	success: VDSLMGR_SUCCESS
                       fail: VDSLMGR_FAIL
******************************************************************************/
static int DoSetLvl(int argc, char **argv, void *p)
{
        ushort _level = (atoi(argv[1]) & 0xf);
       if(_level < 1 || _level > 5)
       {
               printf("input error: input value :1~5, input error: %d\n", atoi(argv[1])) ;
               return VDSLMGR_FAIL;
       }
#if API_CMD_VDSLMGR_DBG
        printf("enter %s.\n", __func__);
#endif
        if(vdslmgr_lib_set_ptm_msglevel(_level) == VDSLMGR_FAIL) {
                printf("Exec. Failed: Set the ptm msglevel command failed\n") ;
                return VDSLMGR_FAIL;
        }
                
                printf("Exec. Successful\n") ;

        return VDSLMGR_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to reset the PTM MAC.
 Input Args:	see PtmSetCmds.
 Ret Value:	success: VDSLMGR_SUCCESS
                       fail: VDSLMGR_FAIL
******************************************************************************/
static int DoSetMacReset(int argc, char **argv, void *p)
{
#if API_CMD_VDSLMGR_DBG
	printf("enter %s.\n", __func__);
#endif
        if(vdslmgr_lib_set_ptm_reset() == VDSLMGR_FAIL) {
                printf("Exec. Failed: Set the ptm reset command failed\n") ;
                return VDSLMGR_FAIL;
        }
                
                printf("Exec. Successful\n") ;

	return VDSLMGR_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to clean the PTM MAC counter.
 Input Args:	see PtmSetCmds.
 Ret Value:	success: VDSLMGR_SUCCESS
                       fail: VDSLMGR_FAIL
******************************************************************************/
static int DoSetCntClr(int argc, char **argv, void *p)
{
#if API_CMD_VDSLMGR_DBG
	printf("enter %s.\n", __func__);
#endif
        if(vdslmgr_lib_set_ptm_clean_counter() == VDSLMGR_FAIL) {
                printf("Exec. Failed: Clean the ptm counter command failed\n") ;
                return VDSLMGR_FAIL;
        }
                
                printf("Exec. Successful\n") ;

	return VDSLMGR_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to get ptm detail counter info.
 Input Args:	see DoPtmGet.
 Ret Value:	none.
******************************************************************************/
static int DoGetReg(int argc, char **argv, void *p)
{
         char tmp_buf[100];
	PTMReg_t ptmReg;
	_procDump_t *dumpx;
         int i = 0;
         _procDump_t ptm_reg_dumps[] = {
		{"PTM_CTRL_REG          = 0x%08lx\n\n", &ptmReg._CTRL_REG},
		{"PTM_TX_UBUF_WR_CNT_L0 = 0x%08lx\t", &ptmReg._TX_UBUF_WR_CNT_L0},
		{"PTM_TX_UBUF_RD_CNT_L0 = 0x%08lx\n", &ptmReg._TX_UBUF_RD_CNT_L0},
		{"PTM_TX_UBUF_WR_CNT_L1 = 0x%08lx\t", &ptmReg._TX_UBUF_WR_CNT_L1},
		{"PTM_TX_UBUF_RD_CNT_L1 = 0x%08lx\n", &ptmReg._TX_UBUF_RD_CNT_L1},
		{"PTM_RX_UBUF_CNT       = 0x%08lx\n\n", &ptmReg._RX_UBUF_CNT},
		{NULL, 0}
	};
         
#if API_CMD_VDSLMGR_DBG 
	printf("enter %s.\n", __func__);
#endif
	memset(&ptmReg, 0, sizeof(ptmReg)) ;
	if(vdslmgr_lib_get_ptm_counter(&ptmReg) == VDSLMGR_FAIL) {
		printf("Exec. Failed: Get the 'PTM counter' command failed\n") ;
		return VDSLMGR_FAIL;
	}
	
	/* ptm_reg_dumps */
	for (dumpx = ptm_reg_dumps; dumpx->name != NULL; dumpx++) {
                 sprintf(tmp_buf, dumpx->name, (*dumpx->reg_addr));
	        printf("%s",tmp_buf);
          }
	
	/* ptm_tx_mac_dumps + ptm_rx_mac_dumps + ptm_rx_crc_mac_dumps */
	for (i = 0; i < TX_QUEUE_NUM; i++) {
		sprintf(tmp_buf, "TMAC_PKT_CNT_P%d  = 0x%08lx\%c", i,
			ptmReg._TMAC_PKT_CNT_P[i], '\t');
		printf("%s",tmp_buf);

		sprintf(tmp_buf, "RMAC_PKT_CNT_P%d  = 0x%08lx\%c", i,
			ptmReg._RMAC_PKT_CNT_P[i], '\n');
		printf("%s",tmp_buf);

		sprintf(tmp_buf, "RMAC_CRCE_CNT_P%d = 0x%08lx\%c", i,
			ptmReg._RMAC_CRCE_CNT_P[i], '\n');
		printf("%s",tmp_buf);
	}
	return VDSLMGR_SUCCESS;
}
/******************************************************************************
 Descriptor:	It's used to get ptm-tc detail counter info.
 Input Args:	see DoPtmGet.
 Ret Value:	none.
******************************************************************************/

static int DoGetTCCnt(int argc, char **argv, void *p)
{
         char tmp_buf[100];
	TCReg_t tcReg;
	_procDump_t *dumpx;
         int i = 0;
         _procDump_t tc_reg_dumps[] = {
		{"TPSTC_TX_B0_OK            = 0x%08lx\n", &tcReg._TPSTC_TX_B0_OK},
		{"TPSTC_TX_B1_OK            = 0x%08lx\n", &tcReg._TPSTC_TX_B1_OK},
		{"TPSTC_TX_B0_FIFO_UNDERRUN = 0x%08lx\n", &tcReg._TPSTC_TX_B0_FIFO_UNDERRUN},
		{"TPSTC_TX_B1_FIFO_UNDERRUN = 0x%08lx\n", &tcReg._TPSTC_TX_B1_FIFO_UNDERRUN},
		{"TPSTC_RX_B0_OK            = 0x%08lx\n", &tcReg._TPSTC_RX_B0_OK},
		{"TPSTC_RX_B1_OK            = 0x%08lx\n", &tcReg._TPSTC_RX_B1_OK},
		{"TPSTC_RX_FIFO_FULL        = 0x%08lx\n", &tcReg._TPSTC_RX_FIFO_FULL},
		{NULL, 0}
	};
#if API_CMD_VDSLMGR_DBG 
	printf("enter %s.\n", __func__);
#endif
	memset(&tcReg, 0, sizeof(tcReg)) ;
	if(vdslmgr_lib_get_tc_counter(&tcReg) == VDSLMGR_FAIL) {
		printf("Exec. Failed: Get the 'PTM-TC counter' command failed\n") ;
		return VDSLMGR_FAIL;
	}
	/* ptm_tc_reg_dumps */
	for (dumpx = tc_reg_dumps; dumpx->name != NULL; dumpx++) {
                 sprintf(tmp_buf, dumpx->name, (*dumpx->reg_addr));
	        printf("%s",tmp_buf);
          }
	
	return VDSLMGR_SUCCESS;
}


