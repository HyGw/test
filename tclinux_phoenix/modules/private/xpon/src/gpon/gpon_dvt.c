/*
* File Name: gpon_dvt.c
* Description: 
*
******************************************************************/
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/skbuff.h>
#include <linux/delay.h>

#include "common/drv_global.h"
#include "gpon/gpon.h"
#include "gpon/gpon_ploam.h"
#include "xmcs/xmcs_gpon.h"

/******************************************************************************
******************************************************************************/
int gpon_dvt_gemport(int times)
{
	GPONDEV_GemPortInfo_T gemPort[256] ;
	unchar valid, encrypt ;
	int n, i, ret ;
	int tmp = 0 ;
	
	for(n=0 ; n<times ; n++) {
		tmp = 0;
		for(i=0 ; i<256 ; i++) {
			tmp += (i==0) ? (random32()%16) : ((random32()%16)+1) ;
			gemPort[i].gemPortId = tmp ;
			gemPort[i].isValid = 1 ;
			gemPort[i].isEncrypted = (random32()%2) ? 1 : 0 ;
			
			ret = gponDevSetGemInfo(gemPort[i].gemPortId, gemPort[i].isValid, gemPort[i].isEncrypted) ;
			if(ret != 0) {
				printk("Set GEM port:%d, ID:%d Encrypt:%d failed\n", i, tmp, gemPort[i].isEncrypted) ;	
				return -EFAULT ;
			}
		}

		for(i=0 ; i<256 ; i++) {
			ret = gponDevGetGemInfo(gemPort[i].gemPortId, &valid, &encrypt) ;
			if(ret != 0) {
				printk("Get GEM port ID:%d failed\n", i) ;	
				return -EFAULT ;
			} else {
				if(gemPort[i].isValid!=valid || gemPort[i].isEncrypted!=encrypt) {
					printk("The GEM port ID:%d, field mismatch.\n", i) ;	
					return -EFAULT ;
				}
			}
		}

		if((n%1000) == 999) {
			printk("Repeat to verification Gem Port CSR (%d times)\n", i) ;
		}
	}

	printk("Gem Port CSR verification successful (%d times)\n", times) ;
	return 0 ;
}

/******************************************************************************
******************************************************************************/
int gpon_dvt_onu_connection(int times)
{
	int num=0, omci=0 ;
	int i, j ;
	PWAN_NetPriv_T *pNetPriv = netdev_priv(gpWanPriv->pPonNetDev[PWAN_IF_OMCI]) ;
	
	gpGponPriv->gponCfg.flags.dvtGponLosFlag = 1 ;
	while(gpGponPriv->gponCfg.flags.dvtGponLosFlag) {
		gpon_enable() ;
		
		for(i=0 ; i<300 ; i++) {
			if(GPON_CURR_STATE != GPON_STATE_O5) {
				msleep(1000) ;
			} else {
				break ;	
			}
		}
		if(GPON_CURR_STATE != GPON_STATE_O5) {
			printk("%d:ONU activation failed. The current state:O%d.\n", num, GPON_CURR_STATE) ;
			break ;
		}
		
		for(j=0 ; j<10 ; j++) {
			if(omci != pNetPriv->stats.tx_packets) {
				omci = pNetPriv->stats.tx_packets ;
				j = 0 ;
			} 
			msleep(1000) ;
		}
		
		num++ ;
		printk("%d:Finish ONU activation. The current state:O%d and receive %d OMCI messages\n", num, GPON_CURR_STATE, (uint)pNetPriv->stats.tx_packets) ;
		pNetPriv->stats.tx_packets = 0 ;
		pNetPriv->stats.tx_bytes = 0 ;
		gpon_disable() ;
		msleep(1000) ;
		
		if(times!=0 && num==times) {
			gpGponPriv->gponCfg.flags.dvtGponLosFlag = 0 ;
		}
	}
	
	return 0 ;
}

/******************************************************************************
******************************************************************************/
int gpon_dvt_transceiver_on_off_connection(int times)
{
	int i, j ;
	REG_DBG_CAP_SETTING gponCapSetting ;
	uint gpioCfg ;
	uint txPacket, tmp ;
	
	gpioCfg = IO_GREG(0xBFBF0200) ;
	gpioCfg |=  0x00010000 ;
	IO_SREG(0xBFBF0200, gpioCfg) ;
	
	gpioCfg = IO_GREG(0xBFBF0214) ;
	gpioCfg |=  0x00000100 ;
	IO_SREG(0xBFBF0214, gpioCfg) ;
	
	for(i=0 ; i<times ; i++) {
		/* power the trancevier on */
		gpioCfg = IO_GREG(0xBFBF0204) ;
		gpioCfg &=  ~0x00000100 ;
		IO_SREG(0xBFBF0204, gpioCfg) ;
		
		/* Setting the MIB is packet mode */
		gponCapSetting.Raw = IO_GREG(DBG_CAP_SETTING) ;
		gponCapSetting.Bits.mib_frame_type = 1 ;
		IO_SREG(DBG_CAP_SETTING, gponCapSetting.Raw) ;
		
		/* waiting for O5 state */
		for(j=0 ; j<1500 ; j++) {
			if(GPON_CURR_STATE != GPON_STATE_O5) {
				msleep(200) ;
			} else {
				break ;
			}
		}
		if(GPON_CURR_STATE != GPON_STATE_O5) {
			printk("%d:ONU activation failed. The current state:O%d.\n", i+1, GPON_CURR_STATE) ;
			goto ret ;
		}
		
		msleep(3000) ;
		
		/* check the MAC gem tx counter */
		for(j=0 ; j<40 ; j++) {
			gponDevGetGemPortCounter(gpWanPriv->gpon.gemPort[1].info.portId, GEMPORT_TX_FRAME_CNT, &tmp, &txPacket) ;
			if(txPacket < 500000) {
				msleep(200) ;
			} else {
				break ;
			}
		}
		if(j == 10) {
			printk("%d:ONU activation failed. The gpon tx packet is %d.\n", i+1, txPacket) ;
			goto ret ;
		}
			
		printk("%d:ONU activation successful. The gpon tx packet is %d.\n", i+1, txPacket) ;
		
		/* power the tranceiver off */
		gpioCfg = IO_GREG(0xBFBF0204) ;
		gpioCfg |=  0x00000100 ;
		IO_SREG(0xBFBF0204, gpioCfg) ;

		msleep(2000) ;
	}
	
ret:
	/* power the tranceiver off */
	gpioCfg = IO_GREG(0xBFBF0204) ;
	gpioCfg |=  0x00000100 ;
	IO_SREG(0xBFBF0204, gpioCfg) ;

	return 0 ;
}

/******************************************************************************
******************************************************************************/
int gpon_dvt_pcp_auto_testing(void)
{
	unchar str1[3][7] = {"CDM Tx\0", "CDM Rx\0", "GDM Rx\0"} ;
	unchar str2[4][5] = {"8P0D\0", "7P1D\0", "6P2D\0", "5P3D\0"} ;
	int i, j ;

	for(i=0 ; i<3 ; i++) {
		for(j=0 ; j<4 ; j++) {
			IO_SREG(0xbfb51514, (1<<(i*4+j))) ;
			gpGponPriv->gponCfg.dvtPcpCheck = ((1<<(i+4)) | (1<<j)) ;

			msleep(120000) ;
			
			gpGponPriv->gponCfg.dvtPcpCheck = 0 ;
			msleep(10000) ;
			
			printk("%s, %s PCP auto check correct counters:%d, error counters:%d\n", str1[i], str2[j], gpGponPriv->gponCfg.dvtPcpCounter, gpGponPriv->gponCfg.dvtPcpCheckErr) ;
			gpGponPriv->gponCfg.dvtPcpCounter = 0 ;
			gpGponPriv->gponCfg.dvtPcpCheckErr = 0 ;
		}
	}
	
	IO_SREG(0xbfb51514, 0) ;
	return 0 ;
}

/******************************************************************************
******************************************************************************/
int gpon_dvt_pcp_check(unchar mode, struct sk_buff *skb)
{
	unchar *p = (unchar *)skb->data ;
	unchar source = *(p + 11) - 1 ;
	unchar modified = (*(p + 14) >> 4) ;
	unchar value = 0 ;
	unchar decode[4][8] = {{0x0, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe},
						   {0x0, 0x2, 0x4, 0x6, 0x9, 0x8, 0xc, 0xe},
						   {0x0, 0x2, 0x5, 0x4, 0x9, 0x8, 0xc, 0xe},
						   {0x1, 0x0, 0x5, 0x4, 0x9, 0x8, 0xc, 0xe}} ;
	unchar encode[4][16] = {{0x0, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x0, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe},
						    {0x0, 0x2, 0x4, 0x6, 0xa, 0xa, 0xc, 0xe, 0x0, 0x2, 0x4, 0x6, 0x8, 0x8, 0xc, 0xe},
						    {0x0, 0x2, 0x6, 0x6, 0xa, 0xa, 0xc, 0xe, 0x0, 0x2, 0x4, 0x4, 0x8, 0x8, 0xc, 0xe},
						    {0x2, 0x2, 0x6, 0x6, 0xa, 0xa, 0xc, 0xe, 0x0, 0x0, 0x4, 0x4, 0x8, 0x8, 0xc, 0xe}} ;
						   	
	if(mode & 0x10) { /* CDM Tx Encode */
		if(source <= 15) {
			if(mode & 0x1) {
				value = encode[0][source] ;
			} else if(mode & 0x02) {
				value = encode[1][source] ;
			} else if(mode & 0x04) {
				value = encode[2][source] ;
			} else if(mode & 0x08) {
				value = encode[3][source] ;
			}
			if(modified == value) {
				gpGponPriv->gponCfg.dvtPcpCounter++ ;
				return 0 ;
			} else {
				printk("Check the PCP CDM Tx Encode error, %d, %d\n", modified, value) ;
#ifdef XPON_MAC_CONFIG_DEBUG
				if(gpPonSysData->debugLevel & MSG_WARN) {
					__dump_skb(skb, 32) ;
				}
#endif
				gpGponPriv->gponCfg.dvtPcpCheckErr++ ;
			}	
		}
	} else if((mode&0x20) || (mode&0x40)) { /* CDM/GDM Rx Decode */
		if(source <= 7) {
			if(mode & 0x1) {
				value = decode[0][source] ;
			} else if(mode & 0x02) {
				value = decode[1][source] ;
			} else if(mode & 0x04) {
				value = decode[2][source] ;
			} else if(mode & 0x08) {
				value = decode[3][source] ;
			}
			if(modified == value) {
				gpGponPriv->gponCfg.dvtPcpCounter++ ;
				return 0 ;
			} else {
				printk("Check the PCP %s Rx decode error, %d, %d\n", (mode&0x20)?"CDM":"GDM", modified, value) ;
#ifdef XPON_MAC_CONFIG_DEBUG
				if(gpPonSysData->debugLevel & MSG_WARN) {
					__dump_skb(skb, 32) ;
				}
#endif
				gpGponPriv->gponCfg.dvtPcpCheckErr++ ;
			}	
		}
	}
	
	return -1 ;
}

/******************************************************************************
******************************************************************************/
int gpon_dvt_eqd_adjustment(uint neqEqd)
{
	if(GPON_CURR_STATE == GPON_STATE_O5) {
		ploam_eqd_adjustment(neqEqd) ;
		return 0 ;
	} else {
		return -1 ;
	}
}

/******************************************************************************
******************************************************************************/
void gpon_dvt_show_aes_key(GPON_DEV_ENCRYPT_KEY_INFO_T aesKey)
{
	/* Show active super frame counter */
	printk("AES CFG aes_spf_cnt(%08x):%08x\n", G_AES_CFG, aesKey.aesSpf) ;
	
	/* Show SHADOW key */
	printk("AES SHADOW KEY0(%08x):%08x\n", G_AES_SHADOW_KEY0, aesKey.shadowKey[0]) ;
	printk("AES SHADOW KEY1(%08x):%08x\n", G_AES_SHADOW_KEY1, aesKey.shadowKey[1]) ;
	printk("AES SHADOW KEY2(%08x):%08x\n", G_AES_SHADOW_KEY2, aesKey.shadowKey[2]) ;
	printk("AES SHADOW KEY3(%08x):%08x\n", G_AES_SHADOW_KEY3, aesKey.shadowKey[3]) ;

	/* Show active key */
	printk("AES ACTIVE KEY0(%08x):%08x\n", G_AES_ACTIVE_KEY0, aesKey.activeKey[0]) ;
	printk("AES ACTIVE KEY1(%08x):%08x\n", G_AES_ACTIVE_KEY1, aesKey.activeKey[1]) ;
	printk("AES ACTIVE KEY2(%08x):%08x\n", G_AES_ACTIVE_KEY2, aesKey.activeKey[2]) ;
	printk("AES ACTIVE KEY3(%08x):%08x\n", G_AES_ACTIVE_KEY3, aesKey.activeKey[3]) ;
}

#ifdef TCSUPPORT_CPU_EN7521
/******************************************************************************
 Descriptor:	Test GPON MAC AES key switch by software for EN7521.
 Input Args:	none.
 Ret Value:	  0: Success.
               	-1: HW has something error.
******************************************************************************/
int gpon_dvt_aes_key_switch_by_sw(void)
{
	GPON_AES_KEY_SWITCH_BY_SW_DONE_t	aesSwitchKeyResult;
	GPON_DEV_ENCRYPT_KEY_INFO_T			aesKey;
	unchar								key[GPON_ENCRYPT_KEY_LENS] ;
    int RETRY = (3 << 10) ;

	printk("Before setting AES key switch by SW.\n") ;
	
	/* Show AES key info */
	gponDevGetEncryptKey(&aesKey);
	gpon_dvt_show_aes_key(aesKey);
	
	printk("Setting new AES key.\n") ;

	/* Generate a random number for AES key */
	get_random_bytes(key, GPON_ENCRYPT_KEY_LENS) ;
	/* Set the new AES key */
	gponDevSetEncryptKey(key) ;

	/* Show AES key info */
	gponDevGetEncryptKey(&aesKey);
	gpon_dvt_show_aes_key(aesKey);

	printk("Enable AES key switch by SW.\n") ;
	
	/* Set AES key switch by sw */
	aesSwitchKeyResult = gponDevSetAesKeySwitchBySw();
	if(aesSwitchKeyResult == GPON_AES_KEY_SWITCH_BY_SW_DONE_PROCESSING) {
		/* There is something wrong with set AES key switch by SW */
		PON_MSG(MSG_ERR, "There is something wrong with set AES key switch by SW\n");
		return -1;
	}

	printk("After enable AES key switch by SW.\n") ;

	/* Show active super frame counter */
	gponDevGetEncryptKey(&aesKey);
	gpon_dvt_show_aes_key(aesKey);
	
	return 0;
}

/******************************************************************************
 Descriptor:	Test GPON MAC HW gemport table.
 Input Args:	none.
 Ret Value:	  0: Success.
                  -1: set gem table fail.
******************************************************************************/
int test_gem_info(void)
{
	unchar valid, encrypt ;
	ushort id;
	int i ;
	
	for(i=0 ; i < GPON_MAX_GEM_ID ; i++) {
		encrypt = random32() & 0x01;
		valid = random32() & 0x01;
		id = random32() & 0xFFF;
		if(gponDevSetGemInfo(id, valid, encrypt) != 0) {
			return -1 ;
		}
	}

	return 0 ;
}

/******************************************************************************
 Descriptor:	Test GPON MAC HW tcont table.
 Input Args:	none.
 Ret Value:	  0: Success.
                  -1: set tcont table fail.
******************************************************************************/
int test_tcont_info(void)
{
	int i;
	GPON_TCONT_t valid;
	ushort id;

	for(i = 0 ; i < CONFIG_GPON_MAX_TCONT ; i++) {
		/* set tcont status */
		valid = random32() & 0x01;
		id = random32() & 0xFFF;
		if(gponDevSetTCont(valid, i, id) != GPON_TCONT_CMD_SUCCESS) {
			return -1;
		}
	}

	return 0 ;
}

/******************************************************************************
 Descriptor:	Test GPON MAC software reset(register bfb643a0 bit0) for EN7521.
 Input Args:	none.
 Ret Value:	  0: Success.
******************************************************************************/
int test_gpon_mac_reg(void)
{
	if(test_gem_info() != 0) {
		return -1;
	}

	if(test_tcont_info() != 0) {
		return -1;
	}
	
	IO_SREG(G_ONU_ID,						random32()) ;
	IO_SREG(G_GBL_CFG,						random32()) ;
	IO_SREG(G_INT_STATUS,					random32()) ;
	IO_SREG(G_INT_ENABLE,					random32()) ;
	IO_SREG(G_OMCI_ID,						random32()) ;
	IO_SREG(G_AES_CFG,						random32()) ;
	IO_SREG(G_AES_ACTIVE_KEY0,				random32()) ;
	IO_SREG(G_AES_ACTIVE_KEY1,				random32()) ;
	IO_SREG(G_AES_ACTIVE_KEY2,				random32()) ;
	IO_SREG(G_AES_ACTIVE_KEY3,				random32()) ;
	IO_SREG(G_AES_SHADOW_KEY0,				random32()) ;
	IO_SREG(G_AES_SHADOW_KEY1,				random32()) ;
	IO_SREG(G_AES_SHADOW_KEY2,				random32()) ;
	IO_SREG(G_AES_SHADOW_KEY3,				random32()) ;
#ifdef TCSUPPORT_CPU_EN7521
	IO_SREG(G_AES_KEY_SWITCH_BY_SW,			random32()) ;
#endif

	IO_SREG(G_PLOu_OVERHEAD,				random32()) ;
	IO_SREG(G_PLOu_GUARD_BIT,				random32()) ;
	IO_SREG(G_PLOu_PRMBL_TYPE1_2,			random32()) ;
	IO_SREG(G_PLOu_PRMBL_TYPE3,				random32()) ;
	IO_SREG(G_PLOu_DELM_BIT,				random32()) ;
	IO_SREG(G_PRE_ASSIGNED_DLY,				random32()) ;
	IO_SREG(G_EQD,							random32()) ;
	IO_SREG(G_RSP_TIME,						random32()) ;
	IO_SREG(G_VENDOR_ID,					random32()) ;
	IO_SREG(G_VS_SN,						random32()) ;
	IO_SREG(G_SN_MSG_CFG,					random32()) ;
	IO_SREG(G_ACTIVATION_ST,				random32()) ;

	IO_SREG(G_TOD_CFG,						random32()) ;
	IO_SREG(G_NEW_TOD_SEC_L32,				random32()) ;
	IO_SREG(G_NEW_TOD_NANO_SEC,				random32()) ;
	IO_SREG(G_CUR_TOD_SEC_L32,				random32()) ;
	IO_SREG(G_CUR_TOD_NANO_SEC,				random32()) ;
	IO_SREG(G_TOD_CLK_PERIOD,				random32()) ;

	IO_SREG(G_TX_FCS_TBL_INIT,				random32()) ;

	IO_SREG(G_MIB_CTRL_STS,					random32()) ;
	IO_SREG(G_MIB_RDATA_L32,				random32()) ;
	IO_SREG(G_MIB_RDATA_H32,				random32()) ;
	IO_SREG(G_MIB_WDATA_L32,				random32()) ;
	IO_SREG(G_MIB_WDATA_H32,				random32()) ;
	IO_SREG(G_MIB_TBL_INIT,					random32()) ;

	IO_SREG(G_GPIDX_TBL_CTRL,				random32()) ;
	IO_SREG(G_GPIDX_TBL_STS,				random32()) ;
	IO_SREG(G_GPIDX_TBL_INIT,				random32()) ;

	IO_SREG(G_MBI_STOP,						random32()) ;

	IO_SREG(DBG_CAP_SETTING,				random32()) ;
	IO_SREG(DBG_US_PREF_OFS,				random32()) ;
	IO_SREG(DBG_DLY,						random32()) ;
	IO_SREG(DBG_IDLE_GEM_THLD,				random32()) ;
	IO_SREG(DBG_US_NO_MSG0,					random32()) ;
	IO_SREG(DBG_US_NO_MSG1,					random32()) ;
	IO_SREG(DBG_US_NO_MSG2,					random32()) ;
	IO_SREG(DBG_US_DYING_GASP_CTRL,			random32()) ;
	IO_SREG(DBG_BWM_FILTER_CTRL,			random32()) ;
#ifdef TCSUPPORT_CPU_EN7521
	IO_SREG(DBG_BWM_SFIFO_STS,				random32()) ;
#else
	IO_SREG(DBG_BWM_FIFO_STS,				random32()) ;
#endif
	IO_SREG(DBG_GRP_0,						random32()) ;
	IO_SREG(DBG_GRP_1,						random32()) ;
	IO_SREG(DBG_TXFIFO_MAX_USED,			random32()) ;
	IO_SREG(DBG_BWM_GNT_STS,				random32()) ;

#ifdef TCSUPPORT_CPU_EN7521
	IO_SREG(DBG_BWM_BFIFO_STS,				random32()) ;
#endif

	IO_SREG(DBG_ERR_CTRL,					random32()) ;
	IO_SREG(DBG_SW_RDI_CTRL,				random32()) ;

	IO_SREG(DBG_RX_GEM_CNT,					random32()) ;
	IO_SREG(DBG_RX_CRC_ERR_CNT,				random32()) ;
	IO_SREG(DBG_RX_GTC_CNT,					random32()) ;
	IO_SREG(DBG_TX_GEM_CNT,					random32()) ;
	IO_SREG(DBG_TX_BST_CNT,					random32()) ;

	IO_SREG(DBG_GEM_HEC_ONE_ERR_CNT,		random32()) ;
	IO_SREG(DBG_GEM_HEC_TWO_ERR_CNT,		random32()) ;
	IO_SREG(DBG_GEM_HEC_UC_ERR_CNT,			random32()) ;

#ifdef TCSUPPORT_CPU_EN7521
	IO_SREG(DBG_DBA_BACK_DOOR,				random32()) ;
	IO_SREG(DBG_DBA_TOTAL_BUF_SIZE,			random32()) ;
	IO_SREG(DBG_DBA_GREEN_SIZE,				random32()) ;
	IO_SREG(DBG_DBA_YELLOW_SIZE,			random32()) ;
	IO_SREG(DBG_SLIGHT_MODIFY_SIZE_1,		random32()) ;
	IO_SREG(DBG_SLIGHT_MODIFY_SIZE_2,		random32()) ;
 	IO_SREG(DBG_DS_SPF_CNT,					random32()) ;
 	IO_SREG(DBG_TX_SYNC_OFFSET,				random32()) ;
	IO_SREG(DBG_PLOAMD_FILTER_IN_O5,		random32()) ;
	IO_SREG(DBG_SW_RESYNC,					random32()) ;
	IO_SREG(DBG_GTC_ETH_EXTR,				random32()) ;
	IO_SREG(DBG_DS_GTC_EXTR_ETH_HDR,		random32()) ;
	IO_SREG(DBG_US_GTC_EXTR_ETH_HDR,		random32()) ;
	IO_SREG(DBG_DS_GTC_EXTR_ETH_CNT,		random32()) ;
	IO_SREG(DBG_US_GTC_EXTR_ETH_CNT,		random32()) ;
	IO_SREG(SNIFF_GTC_GTC_INVLD_GEM_BYTE,	random32()) ;
	IO_SREG(DBG_SNIFF_DBG_REG,				random32()) ;
	IO_SREG(DBG_SNIFF_US_INFO_FIFO,			random32()) ;
	IO_SREG(SNIFF_US_HDR_WIDO,				random32()) ;
	IO_SREG(SNIFF_TX_DA_SA,					random32()) ;
	IO_SREG(SNIFF_RX_DA_SA,					random32()) ;
	IO_SREG(SNIFF_TX_RX_PID,				random32()) ;
	IO_SREG(SNIFF_CNT_EN,					random32()) ;
	IO_SREG(SNIFF_RX_TX_TPID,				random32()) ;
	IO_SREG(DBG_GPON_MAC_SET,				random32() | GPON_MAC_SW_RESET_DISABLE) ;
	IO_SREG(SLEEP_GLB_CFG,					random32()) ;
	IO_SREG(SLEEP_CNT,						random32()) ;
	IO_SREG(TX_SYNC_OPT,					random32()) ;
	IO_SREG(O3_O4_PLOAMU_CTRL,				random32()) ;
	if (isEN7526c)
		IO_SREG(TOD_1PPS_WD_CTRL,				random32()) ;
#endif

	return 0;
}

/******************************************************************************
 Descriptor:	Test GPON MAC software reset(register bfb643a0 bit0) for EN7521.
 Input Args:	none.
 Ret Value:	  0: Success.
                 	-1: test sw reset fail.
******************************************************************************/
int gpon_dvt_sw_reset(void)
{
	/* write GPON MAC register */
	if(test_gpon_mac_reg() != 0) {
		return -1;
	}

	/* dump CSR */
	printk("before sw reset, dump CSR\n");
	if(gponDevDumpCsr() != 0) {
		return -1;
	}

	/* dump hw gemport table info */
	printk("before sw reset, dump hw gemport table info\n");
	if(gponDevDumpGemInfo() != 0) {
		return -1;
	}

	/* dump hw tcont table info */
	printk("before sw reset, dump hw tcont table info\n");
	if(gponDevDumpTcontInfo() != 0) {
		return -1;
	}

	/* GPON MAC SW reset */
	if(gponDevSwReset() != 0) {
		return -1;
	}

	/* dump CSR */
	printk("after sw reset, dump CSR\n");
	if(gponDevDumpCsr() != 0) {
		return -1;
	}

	/* dump hw gemport table info */
	printk("after sw reset, dump hw gemport table info\n");
	if(gponDevDumpGemInfo() != 0) {
		return -1;
	}

	/* dump hw tcont table info */
	printk("after sw reset, dump hw tcont table info\n");
	if(gponDevDumpTcontInfo() != 0) {
		return -1;
	}
	
	return 0;
}

/******************************************************************************
 Descriptor:	Test GPON MAC software resync(register bfb64640 bit31, and bit0) for EN7521.
 Input Args:	none.
 Ret Value:	  0: Success.
                 	-1: test sw reset fail.
******************************************************************************/
int gpon_dvt_sw_resync(void)
{
	/* dump CSR */
	printk("before sw resync, dump CSR\n");
	if(gponDevDumpCsr() != 0) {
		return -1;
	}

	/* dump hw gemport table info */
	printk("before sw resync, dump hw gemport table info\n");
	if(gponDevDumpGemInfo() != 0) {
		return -1;
	}

	/* dump hw tcont table info */
	printk("before sw resync, dump hw tcont table info\n");
	if(gponDevDumpTcontInfo() != 0) {
		return -1;
	}

	/* GPON MAC SW resync */
	if(gponDevSwResync() != 0) {
		return -1;
	}

	/* dump CSR */
	printk("after sw resync, dump CSR\n");
	if(gponDevDumpCsr() != 0) {
		return -1;
	}

	/* dump hw gemport table info */
	printk("after sw resync, dump hw gemport table info\n");
	if(gponDevDumpGemInfo() != 0) {
		return -1;
	}

	/* dump hw tcont table info */
	printk("after sw resync, dump hw tcont table info\n");
	if(gponDevDumpTcontInfo() != 0) {
		return -1;
	}
	
	return 0;
}
#endif

/******************************************************************************
 Descriptor:	Test Tod. 
                 This function will let tod interrupt(BFB64008[9]) occur after newTime seconds.
 Input Args:	newTime.
 Ret Value:	  0: Success.
******************************************************************************/
int gpon_dvt_tod(uint newTime)
{
	uint 	tod_switch_time ;
	uint	sec ;
	uint	nanosec ;
	uint 	superframe ;

	/* Get current super frame counter */
	gponDevGetSuperframe(&superframe);
	printk("super frame counter[4358] = %x\n", superframe);
	
	/* Get current ToD */
	gponDevGetTodSwitchTime(&tod_switch_time);
	gponDevGetCurrentTod(&sec, &nanosec) ;

	/* Show current ToD */
	printk("Current ToD Information:\n") ;
	printk(" Switch time       [40D0]:0x%.8x\n", tod_switch_time) ;
	printk(" Current second    [40DC]:%u\n", sec) ;
	printk(" Current nanosecond[40E0]:%u\n", nanosec) ;

	/* GTC frame rate is 125us per frame = 8000 frames per second
	 */
	tod_switch_time = superframe + newTime * 8000 ;
	sec = random32() ;
	nanosec = random32() ;

	/* Set new ToD */
	gponDevSetNewTod(tod_switch_time, sec, nanosec) ;
	
	/* Show new ToD */
	printk("New ToD Information:\n") ;
	printk(" Switch time   [40D0]:0x%.8x\n", tod_switch_time) ;
	printk(" New second    [40D4]:%u\n", sec) ;
	printk(" New nanosecond[40D8]:%u\n", nanosec) ;

	return 0;
}
