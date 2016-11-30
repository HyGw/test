/*
** $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/modules/private/tcci/tcadslcmd.c#1 $
*/
/************************************************************************
 *
 *	Copyright (C) 2006 Trendchip Technologies, Corp.
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
** $Log: tcadslcmd.c,v $
** Revision 1.34  2011/10/27 00:34:26  shnwind
** rt65168_autobench support
**
** Revision 1.33  2011/10/14 04:51:05  xmdai_nj
** #11867:Add VDSL/PTM TR69 node.
**
** Revision 1.32  2011/10/03 13:57:46  fredli
** [Enhancement]#11754.modify specified cmds with ***phxcmd
**
** Revision 1.31  2011/09/15 12:54:57  treychen_hc
** 3095 autobench --Trey
**
** Revision 1.30  2011/08/12 07:02:18  xmdai_nj
** #11297:Upstream throughput can not reach to 100% when ADSL is set to Annex M mode.
**
** Revision 1.29  2011/08/05 03:14:44  xyzhu_nj
** #11006: ct com merge to main trunk.(phase 3)
** 1. Add TCSUPPORT_COMPILE tag for ct com compile option;
** 2. Delete files in filesystem that CT_COM not used;
** 3. Merge the lastes bugfix from ct com branch to main trunk.(TLB7.1.2.20-51 -- 57);
** 4. update mic.
**
** Revision 1.28  2011/07/29 07:45:24  treychen_hc
** bug11147 --Trey
**
** Revision 1.27  2011/07/24 12:01:47  xyzhu_nj
** #11006: Merge China telecom branch to Main trunk.
**
** Revision 1.26  2011/07/22 10:23:42  treychen_hc
** bug#10858--Trey
**
** Revision 1.25  2011/07/07 07:46:39  shnwind
** RT63260 & RT63260 auto_bench support
**
** Revision 1.24  2011/06/17 06:34:26  lyxue_sz
** #8413, #7047
**
** Revision 1.23  2011/06/16 00:52:07  xhshi
** 10506
**
** Revision 1.22  2011/06/08 08:42:47  lino
** add RT65168 support
**
** Revision 1.17  2011/06/03 02:21:25  lino
** add RT65168 support
**
** Revision 1.16  2011/05/26 03:12:05  xhshi
** #10220
**
** Revision 1.15  2011/05/17 09:47:20  yzwang_nj
** [Bugfix]#6429
**  Wan adsl linedata neer value display incorrect if the value is negative.
**  Resolved by guangkai
**
** Revision 1.14  2011/05/13 01:34:28  xqxu
** Commit by xqxu:
** #10251
**
** Revision 1.13  2011/05/12 05:36:05  xqxu
** Commit by xqxu:
** ADSL LinkRetrain times.
**
** Revision 1.12  2011/04/19 11:49:06  yzwang_nj
** [Bugfix]#9955
** BSP Release check & compile error
**
** Revision 1.11  2011/04/15 12:03:03  tzzhang_nj
** [Enhancement]#9828:
** Add node InternetGatewayDevice.WANDevice.{i}.WANDSLInterfaceConfig.Stats.Showtime.LOF
**
** Revision 1.10  2011/03/25 08:42:43  yzwang_nj
** [Bugfix]#9564
** The UAS value get by command "adslcmd info --show" is wrong
**
** Revision 1.9  2011/03/17 14:05:32  here
** [Ehancement]Accroding the CHT spec to operate the DSL LED. The dsl link down that  the DSL led is off. The dsl link is up that DSL led is on. The DSL TX/RX traffic that the DSL LED is blinking.
**
** Revision 1.8  2011/03/10 02:02:53  yzwang_nj
** [Bugfix]#
** tcadslcmd Compile error
**
** Revision 1.7  2011/03/09 15:29:29  here
** [Ehancement] Add a new "LED_DSL_ALARM"  marco led define with 48th.
**
** Revision 1.6  2011/03/09 14:03:15  yzwang_nj
** [Feature]#9126
** 1.Add ES,SES and UAS adsl info in GUI.
** 2.Add AR5211 adsl snmp node for comtrend.
** 3.Update tc3162_dmt for comtrend.
** 4.Modify script to support 4/16M firmware upgrade.
**
** Revision 1.5  2011/03/04 03:16:57  yzwang_nj
** [Feature]#9126 Support adsl snmp node. Add related proc file and cfg node.
**
** Revision 1.4  2011/02/11 10:13:43  xyyou
** prepare for next BSP release, fix compile error for customer release and releasebsp
**
** Revision 1.3  2011/01/13 06:24:22  xmdai_nj
** #8439:It is no use when user set CPE adsl mode T1.413 by using command "adslcmd begin --modulation t --up"
**
** Revision 1.2  2010/12/31 08:09:25  xflu_sz
** merge from branch sz: bugfree:#7047
**
** Revision 1.1.1.1  2010/09/30 21:14:54  josephxu
** modules/public, private
**
** Revision 1.8  2010/09/03 14:06:26  xyyou
** add new version control
**
** Revision 1.7  2010/08/09 13:00:21  xhshi
** #6548,#5964
**
** Revision 1.6  2010/07/28 06:08:43  frankliao_hc
** BUGFIX:
** modify MAC interrupt enable/disable problem
**
** Revision 1.5  2010/07/22 03:05:37  xyyou
** bug fix :#6345
**
** Revision 1.4  2010/07/07 09:07:32  frankliao_hc
** BUGFIX:
** fixed sar loopback test fail
**
** Revision 1.3  2010/04/16 07:10:34  xhshi
** #5431 support driver interface
**
** Revision 1.2  2010/04/14 02:36:39  xhshi
** #5431 support driver interface
**
** Revision 1.1.1.1  2010/04/09 09:35:28  feiyan
** New TC Linux Make Flow Trunk
**
** Revision 1.10  2010/02/05 10:16:33  ian
** Remove keyword "Huawei" for adslcmd and atmcmd
**
** Revision 1.9  2010/02/03 12:33:55  yzwang_nj
** [Bug#4813] Query near/far ituid cause crash(3085 DMT)
**
** Revision 1.8  2010/02/03 11:39:09  yuren_nj
** [Enhancement]#4788 Add tr69 parameters.
**
** Revision 1.7  2010/02/03 11:27:31  yuren_nj
** [Enhancement]#4788 Add tr69 parameters.
**
** Revision 1.6  2010/02/03 10:12:41  yuren_nj
** [Enhancement]#4788 Add tr69 parameters.
**
** Revision 1.5  2010/02/02 15:26:05  here
** [Bugfix]User use ci-cmd "w ad fw" to get dmt verison at TC3086 chip, it's will be got "tc3084" model name.
**
** Revision 1.4  2010/01/21 06:25:08  yuren_nj
** [Enhancement]#4615 Add tr69 parameters.
**
** Revision 1.3  2009/12/30 12:33:21  yzwang_nj
** [Bug#4409] Add TR064 Parameters
**
** Revision 1.2  2009/12/30 02:22:40  xhshi
** [#4365]
** Implement DSL Diagnostic function in linux platform
**
** Revision 1.1.1.1  2009/12/17 01:48:05  josephxu
** 20091217, from Hinchu ,with VoIP
**
** Revision 1.1.1.1  2007/04/12 09:42:02  ian
** TCLinuxTurnkey2007
**
** Revision 1.5  2006/07/06 07:24:23  lino
** update copyright year
**
** Revision 1.4  2006/07/06 04:38:28  lino
** add kernel module support
**
** Revision 1.3  2006/01/04 06:10:18  lino
** add diag, neatituid and farituid ci command
**
** Revision 1.2  2005/11/03 11:18:03  lino
** show adsl uptime
**
** Revision 1.1.1.1  2005/11/02 05:45:19  lino
** no message
**
*/
#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/proc_fs.h>
#include <linux/version.h>
#include <asm/addrspace.h>
#include <asm/tc3162/tc3162.h>
#include <asm/tc3162/TCIfSetQuery_os.h>
#include <asm/tc3162/ledcetrl.h>
#include <asm/tc3162/cmdparse.h>
#include <linux/slab.h>
#include "../../../version/tcversion.h"
#ifdef CMD_API
#include "tcci.h"
#endif
#ifdef CONFIG_TC3162_ADSL
#ifdef CMD_API
#define ADSLBUFSIZEMIN 32
#define ADSLBUFSIZELITTLE 256
#define ADSLBUFSIZEMAX 4096
#define ADSLCMD_DIAGNOSTIC_FILE "/tmp/adslFileDiag"

#define ADSL_DIAGNOSTIC_RESULT 	0x0001
#define ADSL_DIAGNOSTIC_OTHER 	0x0002
#define ADSL_DIAGNOSTIC_SNR	 	0x0004
#define ADSL_DIAGNOSTIC_QLN 		0x0008
#define ADSL_DIAGNOSTIC_HLIN	 	0x0010
#define ADSL_DIAGNOSTIC_BITS 		0x0020
#define ADSL_DIAGNOSTIC_GAINS 		0x0040
#define ADSL_DIAGNOSTIC_ACTATP 	0x0080

#define LINE0                   0
#define LINE1                   1

uint32 (*ptm_L2encap_gen_hook_func) (uint8* selfFormData, uint32 selfFormDataLen) = NULL;
EXPORT_SYMBOL(ptm_L2encap_gen_hook_func);

#if defined(TCSUPPORT_CPU_MT7510) || defined(TCSUPPORT_CPU_MT7505) || defined(TCSUPPORT_FTTDP_V2)
int isFpgaAdslVdslUp = 0;
int fpgaAdslUpRate = 1024;
#endif

#ifdef TCSUPPORT_AUTOBENCH
unsigned int adsl_rate[3] = {0};
EXPORT_SYMBOL(adsl_rate);
static int adsl_bench_write_proc(struct file *file, const char *buffer,
    unsigned long count, void *data);
static int adsl_bench_read_proc(char *buf, char **start, off_t off, int count,
    int *eof, void *data);
#endif


#if defined(TCSUPPORT_AUTOBENCH_AFE) || defined(TCSUPPORT_AUTOBENCH_MT7510) || defined(TCSUPPORT_AUTOBENCH_MT7505) || defined(TCSUPPORT_AUTOBENCH_MAIN_CHIP)
unsigned int vdsl_rate[4] = {0};
EXPORT_SYMBOL(vdsl_rate);
unsigned int vdsl_period = 0;
EXPORT_SYMBOL(vdsl_period);
static int vdsl_bench_write_proc(struct file *file, const char *buffer, unsigned long count, void *data);
static int vdsl_bench_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data);
static int vdsl_period_write_proc(struct file *file, const char *buffer, unsigned long count, void *data);
static int vdsl_period_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data);

#define VDSL_QUERY_TR098_DSL_INTERFACE_CONFIG  0x2001
typedef struct
{
  uint8 CurrentProfiles;
  uint16 UPBOKLE;
  uint8 TRELLISds;
  uint8 TRELLISus;
  uint8 ACTSNRMODEds;
  uint8 ACTSNRMODEus;
  uint16 ACTUALCE;
  int16 SNRMpbds[5];
  int16 SNRMpbus[5];
} TR098_VDSL_INTERFACE_CFG;
#endif


//#define ADSL_DIAGNOSTIC_ERROR 	0x8000
char adslinfobuf[ADSLBUFINFO];
#endif
unsigned int adslUpTime = 0;
unsigned int adslActiveTime = 0;
uint8 initLinkRetrain_flag = 1;
unsigned int initLinkRetrainCount = 0;

#ifdef CMD_API
uint8 delt_start_flag = 0;
#endif

#define NARG		40

#define FALSE		0
#define TRUE		1

extern int subcmd(const cmds_t tab[], int argc, char *argv[], void *p);
#ifdef CMD_API
extern int cosubcmd(const cmds_t tab[], int argc, char *argv[], void *p);
#endif
extern int cmd_register(cmds_t *cmds_p);
extern int RtsCiCmd(int argc, char *argv[], void *p);
extern int dmt_ci_cmd(int argc, char *argv[], void *p);
extern int dmt2_ci_cmd(int argc, char *argv[], void *p);
extern int hw_ci_cmd(int argc, char *argv[], void *p);
extern int SwCiCmd(int argc, char *argv[], void *p);
extern int ghs_ci_cmd(int argc, char *argv[], void *p);
extern int TCIfCiCmd(int argc, char *argv[], void *p);

#if defined(TCSUPPORT_CPU_MT7510) && defined(TCSUPPORT_BONDING)
/*************************** slave command **********************************/
extern int RtsCiCmd_slave(int argc, char *argv[], void *p);
extern int dmt_ci_cmd_slave(int argc, char *argv[], void *p);
extern int dmt2_ci_cmd_slave(int argc, char *argv[], void *p);
extern int hw_ci_cmd_slave(int argc, char *argv[], void *p);
extern int SwCiCmd_slave(int argc, char *argv[], void *p);
extern int ghs_ci_cmd_slave(int argc, char *argv[], void *p);
extern int TCIfCiCmd_slave(int argc, char *argv[], void *p);
/****************************************************************************/
#endif

//extern void atmPktsClear(void);
//extern void ethPktsClear(void);
/* frankliao modified 20100728 */
#ifndef CONFIG_MIPS_TC3262
// franliao added 20100707
extern void tc3162_enable_irq(unsigned int irq);
extern void tc3162_disable_irq(unsigned int irq);
#endif
#if defined(TCSUPPORT_WLAN_GPIO)
#if defined(RT5392) || defined(RT5390) || defined(RT5390HL)	|| defined(MT7592)
extern int chkWlanGpioMode(uint8 led_gpio);
extern uint8 getDslLedGpio(void);
extern void set_wlan_gpio_mode(void);
#endif
#endif
static int doWan(int argc, char *argv[], void *p);

static int doAdsl(int argc, char *argv[], void *p);
static int doAdslReset(int argc, char *argv[], void *p);
static int doAdslOpenCmd(int argc, char *argv[], void *p);
static int doAdslOpen(int argc, char *argv[], void *p);
static int doAdslClose(int argc, char *argv[], void *p);
static int doAdslGlite(int argc, char *argv[], void *p);
static int doAdslT1413(int argc, char *argv[], void *p);
static int doAdslGdmt(int argc, char *argv[], void *p);
static int doAdslMultimode(int argc, char *argv[], void *p);
static int doAdslAdsl2(int argc, char *argv[], void *p);//Jason_931110
static int doAdslAdsl2plus(int argc, char *argv[], void *p);//Jason_931110
#ifdef CONFIG_RALINK_VDSL
static int doAdslVdsl2(int argc, char *argv[], void *p);
#endif
#if defined(TCSUPPORT_CWMP_VDSL)
static int doAdsl2plusMulti(int argc, char *argv[], void *p);
#else
#if defined(TCSUPPORT_CT_WAN_PTM)
static int doAdsl2plusMulti(int argc, char *argv[], void *p);
#endif
#endif
static int doAdslFwVersion(int argc, char *argv[], void *p);
static int doAdslStatus(int argc, char *argv[], void *p);
static int doAdslChanndata(int argc, char *argv[], void *p);
static int doAdslPerfdata(int argc, char *argv[], void *p);
static void doAdslUpTimeCal(char* tmpUpTimeStrRet);
static void doAdslActiveTimeCal(char* tmpUpTimeStrRet);
static void timeFormatSec(char* strTime, int time);
static void timeFormatRet(char* strRet,int tmpTime);
static int doAdslLinedata(int argc, char *argv[], void *p);
static int doAdslLinedataFar(int argc, char *argv[], void *p);
static int doAdslLinedataNear(int argc, char *argv[], void *p);
static int doAdslOpmode(int argc, char *argv[], void *p);
#if defined(TCSUPPORT_CPU_MT7510) && defined(TCSUPPORT_BONDING)
static int doBondingBacp(int argc, char *argv[], void *p);
#endif
static int doAdslDiag(int argc, char *argv[], void *p);
static int doAdslNearItuId(int argc, char *argv[], void *p);
static int doAdslFarItuId(int argc, char *argv[], void *p);
#ifdef CMD_API
static int doAdslCmd(int argc, char *argv[], void *p);
static int doAdslBegin(int argc, char *argv[], void *p);
static int doAdslEnd(int argc, char *argv[], void *p);
static int doAdslConnect(int argc, char *argv[], void *p);
static int doAdslInfo(int argc, char *argv[], void *p);
static int doAdslDelt(int argc, char *argv[], void *p);
static int doAdslFwversion(int argc, char *argv[], void *p);
static int doAdslDriverversion(int argc, char *argv[], void *p);
static int doAdslVersion(int argc, char *argv[], void *p);
static int doAdslHelp(int argc, char *argv[], void *p);
static int doAdslBeginUp(int argc, char *argv[], void *p);
static int doAdslBeginMod(int argc, char *argv[], void *p);
static int doAdslBeginModA(int argc, char *argv[], void *p);
static int doAdslBeginModD(int argc, char *argv[], void *p);
static int doAdslBeginModL(int argc, char *argv[], void *p);
static int doAdslBeginModT(int argc, char *argv[], void *p);
static int doAdslBeginMod2(int argc, char *argv[], void *p);
static int doAdslBeginModP(int argc, char *argv[], void *p);
static int doAdslBeginModE(int argc, char *argv[], void *p);
static int doAdslBeginModM(int argc, char *argv[], void *p);
static int doAdslBeginBswap(int argc, char *argv[], void *p);
static int doAdslBeginSra(int argc, char *argv[], void *p);
static int doAdslBeginBswapOn(int argc, char *argv[], void *p);
static int doAdslBeginBswapOff(int argc, char *argv[], void *p);
static int doAdslBeginSraOn(int argc, char *argv[], void *p);
static int doAdslBeginSraOff(int argc, char *argv[], void *p);
static int doAdslConnectUp(int argc, char *argv[], void *p);
static int doAdslConnectDown(int argc, char *argv[], void *p);
static int doAdslInfoState(int argc, char *argv[], void *p);
static int doAdslInfoShow(int argc, char *argv[], void *p);
static int doAdslInfoStats(int argc, char *argv[], void *p);
static int doAdslDeltStart(int argc, char *argv[], void *p);
static int doAdslDeltStatus(int argc, char *argv[], void *p);
static int doAdslDeltShow(int argc, char *argv[], void *p);
static int doAdslDeltShowSnr(int argc, char *argv[], void *p);
static int doAdslDeltShowQln(int argc, char *argv[], void *p);
static int doAdslDeltShowHlin(int argc, char *argv[], void *p);
static int doAdslDeltShowBits(int argc, char *argv[], void *p);
static int doAdslDeltShowActatp(int argc, char *argv[], void *p);
static int doAdslDeltShowGains(int argc, char *argv[], void *p);
static int doAdslDeltShowOthers(int argc, char *argv[], void *p);
static int adslDeltShowInfo(int flag);
#endif
//static int doAdslPktsClear(int argc, char *argv[], void *p);

typedef struct xdsl_fw_ver_s{
	uint16 id;
	char* ver_name;
}xdsl_fw_ver_t;

xdsl_fw_ver_t xdsl_fw_ver[]=
{
	{0x20, "TC3084"},
	{0x40, "TC3085"},
	{0x60, "TC3086"},
	{0x80, "TC3087"},
#ifdef TCSUPPORT_CPU_MT7505	
	{0xc0, "MT7550"},
#else
	{0xc0, "TC3095"},
#endif
	{0xE0, "A60901"},
	{0x0, NULL},
};
static cmds_t wanCmds[] = {
	{"adsl",		doAdsl, 		0x10,	0,	NULL},
	/* reserved space for adsl ci commands */
#if 1
	{NULL,			NULL, 			0x10,	0,	NULL},
	{NULL, 			NULL,			0x10,	0,	NULL},
	{NULL, 			NULL,			0x10,	0,	NULL},
	{NULL, 			NULL,			0x10,	0,	NULL},
	{NULL, 			NULL,			0x10,	0,	NULL},
	{NULL,			NULL,			0x10,	0,	NULL},
	{NULL,			NULL,			0x10,	0,	NULL},
#ifdef CONFIG_RALINK_VDSL
	{NULL,			NULL,			0x10,	0,	NULL},
#endif
#else
	{"rts",			RtsCiCmd, 		0x10,	0,	NULL},
	{"dmt", 		dmt_ci_cmd,		0x10,	0,	NULL},
	{"dmt2", 		dmt2_ci_cmd,	0x10,	0,	NULL},
	{"hwdmt", 		hw_ci_cmd,		0x10,	0,	NULL},
	{"swdmt", 		SwCiCmd,		0x10,	0,	NULL},
	{"ghs",			ghs_ci_cmd,		0x10,	0,	NULL},
	{"tcif",		TCIfCiCmd,		0x10,	0,	NULL},
#endif

#if defined(TCSUPPORT_CPU_MT7510) && defined(TCSUPPORT_BONDING)
	{"adsl_slave",	doAdsl, 		0x10,	0,	NULL},
	{NULL,			NULL, 			0x10,	0,	NULL},
	{NULL, 			NULL,			0x10,	0,	NULL},
	{NULL, 			NULL,			0x10,	0,	NULL},
	{NULL, 			NULL,			0x10,	0,	NULL},
	{NULL, 			NULL,			0x10,	0,	NULL},
	{NULL,			NULL,			0x10,	0,	NULL},
	{NULL,			NULL,			0x10,	0,	NULL},
#if defined(CONFIG_RALINK_VDSL)
	{NULL,			NULL,			0x10,	0,	NULL},
#endif
#endif
	{NULL,			NULL,			0x10,	0,	NULL},
};

static const cmds_t adslCmds[] = {
	{"reset",		doAdslReset,	0x02,  	0,  NULL},
	{"open",		doAdslOpen,		0x02,  	0,  NULL},
	{"opencmd",		doAdslOpenCmd,	0x12,  	0,  NULL},
	{"fwversion",	doAdslFwVersion,0x02,  	0,  NULL},
	{"status",		doAdslStatus,	0x02,  	0,  NULL},
	{"chandata", 	doAdslChanndata,0x02,  	0,  NULL},
	{"perfdata", 	doAdslPerfdata,	0x02,  	0,  NULL},
	{"linedata", 	doAdslLinedata,	0x12,  	0,  NULL},
	{"opmode", 		doAdslOpmode,	0x02,  	0,  NULL},
	{"close",		doAdslClose,	0x02,  	0,  NULL},
	{"diag",		doAdslDiag,		0x02,  	0,  NULL},
	{"nearituid",	doAdslNearItuId,0x02,  	0,  NULL},
	{"farituid",	doAdslFarItuId,	0x02,  	0,  NULL},
#if defined(TCSUPPORT_CPU_MT7510) && defined(TCSUPPORT_BONDING)
	{"bondingBacp",	doBondingBacp,	0x02,  	0,  NULL},
#endif
//	{"pktclear",	doAdslPktsClear,	0x02,  	0,  NULL},
	{NULL,			NULL,			0x10,	0,	NULL},
};

static const cmds_t adslOpenCmds[] =
{
	{"glite",		doAdslGlite,	0x02,   0,  NULL},
	{"t1.413",		doAdslT1413,	0x02,	0,  NULL},
   	{"gdmt",		doAdslGdmt,		0x02,   0,  NULL},
   	{"multimode",	doAdslMultimode,0x02,  	0,  NULL},
   	{"adsl2",		doAdslAdsl2,	0x02,   0,  NULL},//Jason_931110
   	{"adsl2plus",	doAdslAdsl2plus,0x02, 	0,  NULL},//Jason_931110
#ifdef CONFIG_RALINK_VDSL
   	{"vdsl2",		doAdslVdsl2,	0x02,  	0,  NULL},
#endif
#if defined(TCSUPPORT_CWMP_VDSL)
	{"adsl2plus_multi",	doAdsl2plusMulti,0x02,  0,  NULL},
#else
#if defined(TCSUPPORT_CT_WAN_PTM)
	{"adsl2plus_multi", doAdsl2plusMulti,0x02,	0,	NULL},
#endif
#endif
	{NULL,			NULL,			0x10,	0,	NULL},
};

static const cmds_t adslLinedataCmds[] =
{
	{"far",			doAdslLinedataFar,	0x02,   0,  NULL},
	{"near",		doAdslLinedataNear,	0x02,   0,  NULL},
	{NULL,			NULL,				0x10,	0,	NULL},
};

#ifdef CMD_API
static cmds_t AdslCmds[] = {
	{"begin",       		 doAdslBegin,       0x10,   0,  NULL},
	{"end",         		 doAdslEnd,         0x10,   0,  NULL},
	{"connect",     		 doAdslConnect,     0x10,   0,  NULL},
	{"info",        		 doAdslInfo,        0x10,   0,  NULL},
	{"delt",        		 doAdslDelt,        0x10,   0,  NULL},
	{"fwversion",   		 doAdslFwversion,   0x10,   0,  NULL},
	{"driverversion",        doAdslDriverversion,         0x10,   0,  NULL},
	{"version",        		 doAdslVersion,     0x10,   0,  NULL},
	{"help", 	             doAdslHelp,        0x10,   0,  NULL},
	{NULL,          NULL,               0x10,   0,  NULL},
};

static cmds_t AdslBeginCmds[] = {
	{"--up",                doAdslBeginUp,      0x10,   0,  NULL},
	{"--modulation",        doAdslBeginMod,     0x10,   0,  NULL},
	{"--bitswap",           doAdslBeginBswap,   0x10,   0,  NULL},
	{"--sra",               doAdslBeginSra,     0x10,   0,  NULL},
	{NULL,          NULL,               0x10,   0,  NULL},
};

static cmds_t AdslConnectCmds[] = {
    {"--up",       	        doAdslConnectUp,    0x10,   0,  NULL},
    {"--down",       	 	doAdslConnectDown,  0x10,   0,  NULL},
    {NULL,          NULL,               0x10,   0,  NULL},
};

static cmds_t AdslInfoCmds[] = {
    {"--state",             doAdslInfoState,    0x10,   0,  NULL},
    {"--show",        		doAdslInfoShow,     0x10,   0,  NULL},
    {"--stats",           	doAdslInfoStats,    0x10,   0,  NULL},
    {NULL,          NULL,               0x10,   0,  NULL},
};

static cmds_t AdslDeltCmds[] = {
    {"--start",             doAdslDeltStart,    0x10,   0,  NULL},
    {"--status",        	doAdslDeltStatus,   0x10,   0,  NULL},
    {"--show",           	doAdslDeltShow,     0x10,   0,  NULL},
    {NULL,          NULL,               0x10,   0,  NULL},
};

static cmds_t AdslDeltShowCmds[] = {
    {"snr",             	doAdslDeltShowSnr,  	0x10,   0,  NULL},
    {"qln",            		doAdslDeltShowQln,  	0x10,   0,  NULL},
    {"hlin",              	doAdslDeltShowHlin, 	0x10,   0,  NULL},
	{"bits",              	doAdslDeltShowBits, 	0x10,   0,  NULL},
	{"actatp",              doAdslDeltShowActatp, 0x10,   0,  NULL},
    {NULL,          NULL,               0x10,   0,  NULL},
};

static cmds_t AdslBeginModCmds[] = {
    {"a",          			doAdslBeginModA,        0x10,   0,  NULL},
    {"d",        			doAdslBeginModD,       0x10,   0,  NULL},
    {"l",           		doAdslBeginModL,     0x10,   0,  NULL},
    {"t",               	doAdslBeginModT,       0x10,   0,  NULL},
	{"2",               	doAdslBeginMod2,       0x10,   0,  NULL},
	{"p",               	doAdslBeginModP,       0x10,   0,  NULL},
	{"e",               	doAdslBeginModE,       0x10,   0,  NULL},
	{"m",               	doAdslBeginModM,       0x10,   0,  NULL},
    {NULL,          NULL,               0x10,   0,  NULL},
};

static cmds_t AdslBeginBswapOnOffCmds[] = {
    {"on",                	doAdslBeginBswapOn,      0x10,   0,  NULL},
	{"off",        			doAdslBeginBswapOff,     0x10,   0,  NULL},
	{NULL,          NULL,               0x10,   0,  NULL},
};

static cmds_t AdslBeginSraOnOffCmds[] = {
    {"on",                  doAdslBeginSraOn,      0x10,   0,  NULL},
    {"off",                 doAdslBeginSraOff,     0x10,   0,  NULL},
    {NULL,          NULL,               0x10,   0,  NULL},
};

#endif

int doWan(int argc, char *argv[], void *p)
{
	return subcmd(wanCmds, argc, argv, p);
}
#ifdef CMD_API
#define ADSLCMD_BEGIN_MODULATION_A  0x01
#define ADSLCMD_BEGIN_MODULATION_D  0x02
#define ADSLCMD_BEGIN_MODULATION_L  0x04
#define ADSLCMD_BEGIN_MODULATION_T  0x08
#define ADSLCMD_BEGIN_MODULATION_2  0x10
#define ADSLCMD_BEGIN_MODULATION_P  0x20
#define ADSLCMD_BEGIN_MODULATION_E  0x40
#define ADSLCMD_BEGIN_MODULATION_M  0x80

#define  NO_DEFECT       0
#define  LOF(SEF)        1
#define  LOSQ(LOM)   	 2
#define  LOS             3
#define  LPR(LOP)        4
#define  UNKNOWN       	 5
#define  NOT_LINK_UP     255

#define  L0 0
#define  L2 2
#define  L3 3

#define ADSL_QUERY_TR69_WLAN_DSL_INTERFACE_Config	0x001F
#define ADSL_QUERY_PMS_PARAM						0x0020
#define ADSL_QUERY_TR69_WLAN_DSL_DIAGNOSTIC			0x0021
#define ADSL_QUERY_SHOW_STATE_PARAM					0x0022
#define ADSL_QUERY_LINERATE							0x0023
#define ADSL_QUERY_PM_STATE  						0x0025
#define ADSL_QUERY_LAST_DROP_REASON					0x002a
#define ADSL_QUERY_CELL_CNT1						0x002b

#if defined(TCSUPPORT_CT_WAN_PTM)
#define ADSL_QUERY_TR69_WLAN_DSL_DIAGNOSTIC_2			0x002f
#define VDSL_QUERY_TR098_VDSL_DIAGNOSTIC			0x0102
#endif


#define	MAX_TASKQ_SIZE	8

static uint8	adsl_up = 0;
static uint8 	begin_modulation = 0;
#else
#define ADSL_QUERY_TR69_WLAN_DSL_DIAGNOSTIC			0x0021
#endif

#define ADSL_QUERY_ATTAIN_RATE   0x0027 

#define ADSL_QUERY_STATISTICS_2   0x0032

#if defined(TCSUPPORT_SNMP_ATMADSL)
/*Add for snmp adsl mib*/
#define ADSL_QUERY_SNMP_ADSL_NODE   0x0030 
#define SNMP_ADSL_INTERVAL_VAILD_DATA	1

/*adslState proc*/
#define ATUR_CHAN_INTERL_DELAY  "atur chan interleave delay: %d\n"
#define ATUR_CHAN_PREV_RXRATE   "atur chan prev tx rate: %u kbps\n"
#define ATUR_CHAN_CURR_RXRATE   "atur chan curr tx rate: %u kbps\n"
#define ATUR_CRC_BLK_LEN        "atur chan crc block len: %u\n"
/*adslShow proc*/
#define ATUR_CHAN_CURR_RX_BLKS		"atur chan curr rx blocks: %lu\n"
#define ATUR_CHAN_CURR_TX_BLKS		"atur chan curr tx blocks: %lu\n"
#define ATUR_CHAN_CURR_CRT_BLKS		"atur chan curr correct blocks: %u\n"
#define ATUR_CHAN_CURR_UNCRT_BLKS	"atur chan curr uncorrect blocks: %u\n"
#define CURDAY_TIME_ELAPSED		"curdaySt TimeElapsed: %ld\n"
#define QURTHOUR_TIME_ELAPSED		"quthour ATUR Chan TimeElapsed: %ld\n"
#define PREVDAY_ATUR_CHAN_MONISECS	"prevdaySt ATUR Chan MoniSecs: %d\n"
#define ATUR_CHAN_VALID_ITLS		"ATUR Chan Perf Valid Intervals: %ld\n"
#define ATUR_CHAN_INVALID_ITLS		"ATUR Chan Perf Invalid Intervals: %d\n"
/*adslTotalShow proc*/
#define TOTAL_ATUR_CHAN_RX_BLKS		"total ATUR Chan RxBlocks: %lu\n"
#define TOTAL_ATUR_CHAN_TX_BLKS		"total ATUR Chan TxBlocks: %lu\n"
#define TOTAL_ATUR_CHAN_CRT_BLKS	"total ATUR Chan CorrectBlocks: %u\n"
#define TOTAL_ATUR_CHAN_UNCRT_BLKS	"total ATUR Chan UnCorrectBlocks: %u\n"
/*adslCurrDayStart proc*/
#define CURRDAY_ATUR_CHAN_RX_BLKS	"curdaySt ATUR Chan RxBlocks: %lu\n"
#define CURRDAY_ATUR_CHAN_TX_BLKS	"curdaySt ATUR Chan TxBlocks: %lu\n"
#define CURRDAY_ATUR_CHAN_CRT_BLKS	"curdaySt ATUR Chan CorrectBlocks: %u\n"
#define CURRDAY_ATUR_CHAN_UNCRT_BLKS	"curdaySt ATUR Chan UnCorrectBlocks: %u\n"
/*adslQutHourStart proc*/
#define QUTHOUR_ATUR_CHAN_RX_BLKS	"quthour ATUR Chan RxBlocks: %lu\n"
#define QUTHOUR_ATUR_CHAN_TX_BLKS	"quthour ATUR Chan TxBlocks: %lu\n"
#define QUTHOUR_ATUR_CHAN_CRT_BLKS	"quthour ATUR Chan CorrectBlocks: %u\n"
#define QUTHOUR_ATUR_CHAN_UNCRT_BLKS	"quthour ATUR Chan UnCorrectBlocks: %u\n"
/*adslPrevDayStart proc*/
#define PREVDAY_ADSL_INFO		"\nThe most recent previous 1 day Start Showtime\n"
#define PREVDAY_ATUR_CHAN_RX_BLKS	"prevdaySt ATUR Chan RxBlocks: %lu\n"
#define PREVDAY_ATUR_CHAN_TX_BLKS	"prevdaySt ATUR Chan TxBlocks: %lu\n"
#define PREVDAY_ATUR_CHAN_CRT_BLKS	"prevdaySt ATUR Chan CorrectBlocks: %u\n"
#define PREVDAY_ATUR_CHAN_UNCRT_BLKS	"prevdaySt ATUR Chan UnCorrectBlocks: %u\n"
/*adslIntervalStart proc*/
#define ATUR_CHAN_ITL_INFO		"\nThe recent previous interval %d Start Showtime\n"
#define ATUR_CHAN_ITL_CNT		"\nThe valid interval cnt is %ld\n"
#define ATUR_CHAN_ITL_RX_BLKS		"ATUR Chan interval %d RxBlocks: %lu\n"
#define ATUR_CHAN_ITL_TX_BLKS		"ATUR Chan interval %d TxBlocks: %lu\n"
#define ATUR_CHAN_ITL_CRT_BLKS		"ATUR Chan interval %d CorrectBlocks: %u\n"
#define ATUR_CHAN_ITL_UNCRT_BLKS	"ATUR Chan interval %d UnCorrectBlocks: %u\n"

#define ES_DOWNSTREAM		"ES downstream: %lu\n"
#define ES_UPSTREAM		"ES upstream: %u\n"
#define SES_DOWNSTREAM	        "SES downstream: %lu\n"
#define SES_UPSTREAM		"SES upstream: %u\n"
#define UAS_DOWNSTREAM	        "UAS downstream: %lu\n"
#define UAS_UPSTREAM		"UAS upstream: %lu\n"
#define RX_BITERRCNT		"RX BitError Count: %lu\n"
#define TX_BITERRCNT		"TX BitError Count: %lu\n"

#define TOTAL_ES_DOWNSTREAM	"total ES downstream: %lu\n"
#define TOTAL_ES_UPSTREAM	"total ES upstream: %u\n"
#define TOTAL_SES_DOWNSTREAM	"total SES downstream: %lu\n"
#define TOTAL_SES_UPSTREAM	"total SES upstream: %u\n"
#define TOTAL_UAS_DOWNSTREAM	"total UAS downstream: %lu\n"
#define TOTAL_UAS_UPSTREAM	"total UAS upstream: %lu\n"
#endif

#define ADSL_LINK_STATUS_DOWN		"ADSL link status: down\n"
#define ADSL_LINK_STATUS_WAIT_INIT	"ADSL link status: wait for init\n"
#define ADSL_LINK_STATUS_INIT		"ADSL link status: initializing\n"
#define ADSL_LINK_STATUS_UP		"ADSL link status: up\n"
#define ADSL_LINK_STATUS_UNKNOWN	"ADSL link status: unknown\n"

/************************************************************************
*		     A D S L   D A T A   S T R U C T U R E
*************************************************************************
*/
typedef struct
{
	uint32 rxTotalCells;   
	uint32 rxDropCells;      
	uint32 txTotalCells;     
	uint32 txDropCells;      
	uint16 DsOcd;  
	uint16 UsOcd;   
	uint16 DsLcd;   
	uint16 UsLcd;   
	uint32 DsEs;
	uint16 UsEs;
	uint32 DsSes;
	uint16 UsSes;
} CELL_COUNT1;

typedef struct
{
 	uint32 RxSuperFrameCnt;
 	uint32 TxSuperFrameCnt;
 	uint32 RxUAS;
 	uint32 TxUAS;
 	uint32 RxBitErrorCnt;
 	uint32 TxBitErrorCnt;	
} SNMP_ADSL_NODE_2;

typedef struct {
	uint8 nearstate;
	uint8 farstate;
	uint8 nearchange;
	uint8 farchange;
}T_adsl_def_bitmap_set;

static struct timer_list adsl_timer;
typedef struct {
	 uint32          rxAttainRate;
	 uint32          txAttainRate;
}T_adslAttainRate;

//#ifdef CONFIG_RALINK_VDSL
#if defined(CONFIG_RALINK_VDSL) || defined(TCSUPPORT_CPU_MT7505)
typedef struct {
	uint32          actualBitrateNearInt;
	uint32          actualBitrateNearFast;
	uint32          actualBitrateFarInt;
	uint32          actualBitrateFarFast;
} T_adslChannelOperData;

typedef struct {
    uint32          FecNotInterleaved;
    uint32          FecInterleaved;
    uint32          CrcNotInterleaved;
    uint32          CrcInterleaved;
    uint32          HecNotInterleaved;
    uint32          HecInterleaved;
} T_adsl_def_counters;
#else
typedef struct {
	uint16          actualBitrateNearInt;
	uint16          actualBitrateNearFast;
	uint16          actualBitrateFarInt;
	uint16          actualBitrateFarFast;
} T_adslChannelOperData;

typedef struct {
    uint16          FecNotInterleaved;
    uint16          FecInterleaved;
    uint16          CrcNotInterleaved;
    uint16          CrcInterleaved;
    uint16          HecNotInterleaved;
    uint16          HecInterleaved;
} T_adsl_def_counters;
#endif

typedef struct {
    T_adsl_def_counters     near_end;
    T_adsl_def_counters     far_end;
} T_adsl_def_counter_set;

typedef struct {
        uint16          relCapacityOccupationDnstr;
        signed short     noiseMarginDnstr;//Jason_931028
        signed short     outputPowerUpstr;//Jason_931028
        uint16           attenuationDnstr;//Jason_931028
} T_NTadslNearEndLineOperData;

typedef struct {
        uint16          relCapacityOccupationUpstr;
        signed short     noiseMarginUpstr;//Jason_931028
        signed short     outputPowerDnstr;//Jason_931028
        uint16           attenuationUpstr;//Jason_931028
        uint8           carrierLoad[256]; //Jason_931130
} T_NTadslFarEndLineOperData;

typedef struct {
		uint8 	TCM;
		uint8 	K_us;
		uint8 	R_us;
		float 	S_us;
		uint16 	D_us;
		uint8	M_us;
		uint8	T_us;
		uint8	msgc_us;
		uint32	L_us;
		float	delay_us;
		uint32	RSWords_us;
		uint32	SuperFramesErrors_us;
        	uint8   K_ds;
        	uint8   R_ds;
        	float   S_ds;
        	uint16  D_ds;
        	uint8   M_ds;
      		uint8   T_ds;
		uint8   msgc_ds;
		uint32  L_ds;
		float   delay_ds;
		uint32  RSWords_ds;
		uint32  SuperFramesErrors_ds;
} PMS_PARAMS;

typedef struct
{
 	uint32 ReceiveBlock;
	uint32 TransmitBlock;
	uint32 CellDelin;
	uint32 LinkRetrain;
	uint32 InitErrors;
	uint32 InitTimeouts;
	uint32 LossOfFraming;
	uint32 ErroredSecs;
	uint32 SeverelyErroredSecs;
} TR98_WAN_DSL_INTERFACE_CFG_STS_ST;

#if defined(TCSUPPORT_SNMP_ATMADSL)
/*Add for snmp adsl mib*/
typedef struct
{
 	uint16 CrcBlockLength;
	uint16 UcFecCnt;
} SNMP_ADSL_NODE;

typedef struct
{
	uint32 ATURChanRxBlks;
	uint32 ATURChanTxBlks;
 	uint16 ATURChanCorrectBlks;
	uint16 ATURChanUnCorrectBlks;	
} SNMP_ADSL_MIB_ATURCHAN_NODE;

typedef struct
{
 	uint8 modemst;
	char *statusInfo;
} ADSL_STATUS_INFO;

typedef struct
{
	uint32 DsEs;
	uint32 DsSes;
	uint16 UsEs;
	uint16 UsSes;

 	uint32 RxUAS;
 	uint32 TxUAS;
 	//uint32 RxBitErrorCnt;
 	//uint32 TxBitErrorCnt;	
} SNMP_ADSL_MIB_CTAR5211_NODE;

#define ADSLMIB_CHANPERFINTERVAL_MAXNUM   4
 
SNMP_ADSL_MIB_ATURCHAN_NODE 		SnmpADSLAturChanInterval[ADSLMIB_CHANPERFINTERVAL_MAXNUM+1];  //intervals
SNMP_ADSL_MIB_ATURCHAN_NODE 		SnmpADSLAturChanTotal;		//since agent reset
SNMP_ADSL_MIB_ATURCHAN_NODE 		SnmpADSLAturChanCurrDay;	//within the current 1 day period
SNMP_ADSL_MIB_ATURCHAN_NODE 		SnmpADSLAturChanPrevDay;	//within the most recent previous 1 day period
SNMP_ADSL_MIB_ATURCHAN_NODE 		SnmpADSLAturChanQutHour;	//within the currren 15 minutes interval

SNMP_ADSL_MIB_CTAR5211_NODE 		SnmpADSLCTAR5211Total;		

uint16 adslAturChanPrevTxRate = 0;

static int32 snmp_curr_day_timer_cnt = 0;
static int32 snmp_qut_hr_timer_cnt = 0;
static int32 snmp_qut_hr_intervels_cnt = 0;//The num of previous 15-minute intervals in the interval table for which data was collected

PMS_PARAMS g_pms_patams;
T_adsl_def_counter_set g_def_counter_set;
SNMP_ADSL_NODE g_snmpNode;

CELL_COUNT1 g_cellOperdata;
SNMP_ADSL_NODE_2 g_snmpAdslNode_2;
#endif

#ifdef CMD_API
typedef struct {
		uint8	DiagnosticState[16];
		short	ACTPSDds;
		short   ACTPSDus;
		short	ACTATPds;
		short	ACTATPus;
		uint16	HLINSCds;
		short	HLINpsds[512][2];
		uint8	QLINpsds[512];
		uint8	SNRpsds[512];
		uint8	BITSpsds[512];
		uint16	GAINSpsds[512];
} TR98_WAN_DSL_DIAGNOSTICS;
TR98_WAN_DSL_DIAGNOSTICS    dsl_diag;

#if defined(TCSUPPORT_CT_WAN_PTM)
typedef struct {
		uint16	HLINSCus;
		uint8	HLINGds;
		uint8	HLINGus;
		uint8	HLOGGds;
		uint8	HLOGGus;
		uint16	HLOGpsds[512];
		uint16	HLOGpsus[64];
		uint16	HLOGMTds;
		uint16	HLOGMTus;
		uint16	SATNds;
		uint16	SATNus;
		short	HLINpsus[64][2];
		uint8	QLNGds;
		uint8	QLNGus;
		uint8	QLNpsus[64];
		uint16	QLNMTds;
		uint16	QLNMTus;
		uint8	SNRGds;
		uint8	SNRGus;
		uint8	SNRpsus[64];
		uint16	SNRMTds;
		uint16	SNRMTus;
		uint8	BITSpsus[64];
} TR98_WAN_DSL_DIAGNOSTICS_2;
TR98_WAN_DSL_DIAGNOSTICS_2    dsl_diag2;

typedef struct {
	uint16  HLOGpsus[512];
	short 	HLINpsus[512][2];
	uint8	QLNpsus[512];
	uint8	SNRpsus[512];
	uint16 	SATNds[5];
	uint16 	SATNus[5];
	uint16 	LATNds[5];
	uint16 	LATNus[5];
} TR098_WAN_VDSL_DIAGNOSTICS;
TR098_WAN_VDSL_DIAGNOSTICS    vdsl_diag;


#endif

typedef struct
{
	uint16 FecNotInterleaved;
	uint16 FecInterleaved;
	uint16 CrcNotInterleaved;
	uint16 CrcInterleaved;
	uint16 HecNotInterleaved;
	uint16 HecInterleaved;
} DEFECT_CTR;

typedef struct
{
	DEFECT_CTR near_end;
  	DEFECT_CTR far_end;
} DEFECT_CNT_SET;

#else
typedef struct {
		uint8	DiagnosticState[16];
		short	ACTPSDds;
		short   ACTPSDus;
		short	ACTATPds;
		short	ACTATPus;
		uint16	HLINSCds;
		short	HLINpsds[512][2];
		uint8	QLINpsds[512];
		uint8	SNRpsds[512];
		uint8	BITSpsds[512];
		uint16	GAINSpsds[512];
} TR98_WAN_DSL_DIAGNOSTICS;
#endif
#ifdef CWMP
//add by brian for dsl diagnostic
typedef signed short sint16;
//add by brian
TR98_WAN_DSL_DIAGNOSTICS p_diagnostic_set;

#if defined(TCSUPPORT_CT_WAN_PTM)
TR98_WAN_DSL_DIAGNOSTICS_2 p_diagnostic_set2;
TR098_WAN_VDSL_DIAGNOSTICS p_diagnostic_set3;
#endif

//renyu_20100113
typedef struct
{
	uint8 Enable;
	char Status[17];
	uint32 ATURANSIStd;
	uint32 ATUCANSIStd;
}TR98_WAN_DSL_INTERFACE_CFG; 

unsigned int adslShowTimeStart = 0;
unsigned int adslTotalStart = 0;


static uint8 needInitADSLInfoFlag = 1;
//uint8 lastModemst;
uint8 flagLastShowTimeStart = 0;

TR98_WAN_DSL_INTERFACE_CFG_STS_ST WanIfcfgStatsLastShowTime_ShowtimeRate;
TR98_WAN_DSL_INTERFACE_CFG_STS_ST WanIfcfgStatsTotal_ShowtimeRate;
TR98_WAN_DSL_INTERFACE_CFG_STS_ST WanIfcfgStatsCurdaySt_ShowtimeRate;
TR98_WAN_DSL_INTERFACE_CFG_STS_ST WanIfcfgStatsQuthrSt_ShowtimeRate;

T_adsl_def_counter_set WanIfcfgStatsLastShowTime_counter_set;
T_adsl_def_counter_set WanIfcfgStatsTotal_counter_set;
T_adsl_def_counter_set WanIfcfgStatsCurdaySt_counter_set;
T_adsl_def_counter_set WanIfcfgStatsQuthrSt_counter_set;

/*to record the statistics when adsl status change from up to down*/
static TR98_WAN_DSL_INTERFACE_CFG_STS_ST p_ShowtimeRate;
static T_adsl_def_counter_set p_counter_set;

static int32 cwmp_curr_day_timer_cnt = 0;
static int32 cwmp_qut_hr_timer_cnt = 0;
#endif

#if defined(TCSUPPORT_CWMP_VDSL)
#define VDSL2_PROFILE_8A	0
#define VDSL2_PROFILE_8B	1
#define VDSL2_PROFILE_8C	2
#define VDSL2_PROFILE_8D	3
#define VDSL2_PROFILE_12A	4
#define VDSL2_PROFILE_12B	5
#define VDSL2_PROFILE_17A	6
#define VDSL2_PROFILE_30A	7
#else
#if defined(TCSUPPORT_CT_WAN_PTM)
#define VDSL2_PROFILE_8A	0
#define VDSL2_PROFILE_8B	1
#define VDSL2_PROFILE_8C	2
#define VDSL2_PROFILE_8D	3
#define VDSL2_PROFILE_12A	4
#define VDSL2_PROFILE_12B	5
#define VDSL2_PROFILE_17A	6
#define VDSL2_PROFILE_30A	7
#endif
#endif

#if defined(CWMP) || defined(TCSUPPORT_SNMP_ATMADSL)
static int32 currDayLeftTime = -1;//count as second, default is -1.
static int32 qutHourLeftTime = -1;//count as second, default is -1.

#define SECONDS_OF_MINUTE 60
#define SECONDS_OF_HOUR 60*SECONDS_OF_MINUTE
#define SECONDS_OF_DAY 24*SECONDS_OF_HOUR

int32 getCurrDayLeftTime(void)
{
	return currDayLeftTime;
}

void setCurrDayLeftTime(int32 value)
{
	currDayLeftTime = value;
	return;
}

int32 getQutHrLeftTime(void)
{
	return qutHourLeftTime;
}

void setQutHrLeftTime(int32 value)
{
	qutHourLeftTime = value;
	return;
}
#endif

#ifdef CWMP
void resetWanDslIfcCfgTotalADSLInfo(void){
	memset(&WanIfcfgStatsTotal_ShowtimeRate, 0, sizeof(TR98_WAN_DSL_INTERFACE_CFG_STS_ST));
	memset(&WanIfcfgStatsTotal_counter_set, 0, sizeof(T_adsl_def_counter_set));
}

void resetWanDslIfcCfgLastShowTimeADSLInfo(void){
	memset(&WanIfcfgStatsLastShowTime_ShowtimeRate, 0, sizeof(TR98_WAN_DSL_INTERFACE_CFG_STS_ST));
	memset(&WanIfcfgStatsLastShowTime_counter_set, 0, sizeof(T_adsl_def_counter_set));
}

void StructTWanDslIfcCfgInfoAdd(TR98_WAN_DSL_INTERFACE_CFG_STS_ST* p_ShowtimeRate_1, T_adsl_def_counter_set* p_counter_set_1, TR98_WAN_DSL_INTERFACE_CFG_STS_ST* p_ShowtimeRate_2, T_adsl_def_counter_set* p_counter_set_2)
{
	p_ShowtimeRate_1->CellDelin += p_ShowtimeRate_2->CellDelin;
	p_ShowtimeRate_1->ErroredSecs += p_ShowtimeRate_2->ErroredSecs;
	p_ShowtimeRate_1->InitErrors += p_ShowtimeRate_2->InitErrors;
	p_ShowtimeRate_1->InitTimeouts += p_ShowtimeRate_2->InitTimeouts;
	p_ShowtimeRate_1->LinkRetrain += p_ShowtimeRate_2->LinkRetrain;
	p_ShowtimeRate_1->LossOfFraming += p_ShowtimeRate_2->LossOfFraming;
	p_ShowtimeRate_1->SeverelyErroredSecs += p_ShowtimeRate_2->SeverelyErroredSecs;
	p_ShowtimeRate_1->ReceiveBlock += p_ShowtimeRate_2->ReceiveBlock;
	p_ShowtimeRate_1->SeverelyErroredSecs += p_ShowtimeRate_2->SeverelyErroredSecs;
	p_ShowtimeRate_1->TransmitBlock += p_ShowtimeRate_2->TransmitBlock;
	
	p_counter_set_1->near_end.CrcInterleaved += p_counter_set_2->near_end.CrcInterleaved;
	p_counter_set_1->near_end.CrcNotInterleaved += p_counter_set_2->near_end.CrcNotInterleaved;
	p_counter_set_1->near_end.FecInterleaved += p_counter_set_2->near_end.FecInterleaved;
	p_counter_set_1->near_end.FecNotInterleaved += p_counter_set_2->near_end.FecNotInterleaved;
	p_counter_set_1->near_end.HecInterleaved += p_counter_set_2->near_end.HecInterleaved;
	p_counter_set_1->near_end.HecNotInterleaved += p_counter_set_2->near_end.HecNotInterleaved;

	p_counter_set_1->far_end.CrcInterleaved += p_counter_set_2->far_end.CrcInterleaved;
	p_counter_set_1->far_end.CrcNotInterleaved += p_counter_set_2->far_end.CrcNotInterleaved;
	p_counter_set_1->far_end.FecInterleaved += p_counter_set_2->far_end.FecInterleaved;
	p_counter_set_1->far_end.FecNotInterleaved += p_counter_set_2->far_end.FecNotInterleaved;
	p_counter_set_1->far_end.HecInterleaved += p_counter_set_2->far_end.HecInterleaved;
	p_counter_set_1->far_end.HecNotInterleaved += p_counter_set_2->far_end.HecNotInterleaved;
}

void initWanDslIfcCfgCurrDayADSLInfo(void){
	memset(&WanIfcfgStatsCurdaySt_ShowtimeRate, 0, sizeof(TR98_WAN_DSL_INTERFACE_CFG_STS_ST));
	memset(&WanIfcfgStatsCurdaySt_counter_set, 0, sizeof(T_adsl_def_counter_set));
}
void initWanDslIfcCfgQuthrADSLInfo(void){
	memset(&WanIfcfgStatsQuthrSt_ShowtimeRate, 0, sizeof(TR98_WAN_DSL_INTERFACE_CFG_STS_ST));
	memset(&WanIfcfgStatsQuthrSt_counter_set, 0, sizeof(T_adsl_def_counter_set));
}
void cntWanDslIfcCfgCurrDayStartInfo(void){
	WanIfcfgStatsCurdaySt_ShowtimeRate.ReceiveBlock = WanIfcfgStatsTotal_ShowtimeRate.ReceiveBlock + p_ShowtimeRate.ReceiveBlock;
	WanIfcfgStatsCurdaySt_ShowtimeRate.TransmitBlock = WanIfcfgStatsTotal_ShowtimeRate.TransmitBlock + p_ShowtimeRate.TransmitBlock;
	WanIfcfgStatsCurdaySt_ShowtimeRate.CellDelin = WanIfcfgStatsTotal_ShowtimeRate.CellDelin + p_ShowtimeRate.CellDelin;
	WanIfcfgStatsCurdaySt_ShowtimeRate.LinkRetrain = WanIfcfgStatsTotal_ShowtimeRate.LinkRetrain + p_ShowtimeRate.LinkRetrain;
	WanIfcfgStatsCurdaySt_ShowtimeRate.InitErrors = WanIfcfgStatsTotal_ShowtimeRate.InitErrors + p_ShowtimeRate.InitErrors;
	WanIfcfgStatsCurdaySt_ShowtimeRate.InitTimeouts = WanIfcfgStatsTotal_ShowtimeRate.InitTimeouts + p_ShowtimeRate.InitTimeouts;
	WanIfcfgStatsCurdaySt_ShowtimeRate.LossOfFraming = WanIfcfgStatsTotal_ShowtimeRate.LossOfFraming + p_ShowtimeRate.LossOfFraming;
	WanIfcfgStatsCurdaySt_ShowtimeRate.ErroredSecs = WanIfcfgStatsTotal_ShowtimeRate.ErroredSecs + p_ShowtimeRate.ErroredSecs;
	WanIfcfgStatsCurdaySt_ShowtimeRate.SeverelyErroredSecs = WanIfcfgStatsTotal_ShowtimeRate.SeverelyErroredSecs + p_ShowtimeRate.SeverelyErroredSecs;
	
	WanIfcfgStatsCurdaySt_counter_set.near_end.CrcInterleaved = WanIfcfgStatsTotal_counter_set.near_end.CrcInterleaved + p_counter_set.near_end.CrcInterleaved;
	WanIfcfgStatsCurdaySt_counter_set.near_end.CrcNotInterleaved = WanIfcfgStatsTotal_counter_set.near_end.CrcNotInterleaved + p_counter_set.near_end.CrcNotInterleaved;
	WanIfcfgStatsCurdaySt_counter_set.near_end.FecInterleaved = WanIfcfgStatsTotal_counter_set.near_end.FecInterleaved + p_counter_set.near_end.FecInterleaved;
	WanIfcfgStatsCurdaySt_counter_set.near_end.FecNotInterleaved = WanIfcfgStatsTotal_counter_set.near_end.FecNotInterleaved + p_counter_set.near_end.FecNotInterleaved;
	WanIfcfgStatsCurdaySt_counter_set.near_end.HecInterleaved = WanIfcfgStatsTotal_counter_set.near_end.HecInterleaved + p_counter_set.near_end.HecInterleaved;
	WanIfcfgStatsCurdaySt_counter_set.near_end.HecNotInterleaved = WanIfcfgStatsTotal_counter_set.near_end.HecNotInterleaved + p_counter_set.near_end.HecNotInterleaved;
	
	WanIfcfgStatsCurdaySt_counter_set.far_end.CrcInterleaved = WanIfcfgStatsTotal_counter_set.far_end.CrcInterleaved + p_counter_set.far_end.CrcInterleaved;
	WanIfcfgStatsCurdaySt_counter_set.far_end.CrcNotInterleaved = WanIfcfgStatsTotal_counter_set.far_end.CrcNotInterleaved + p_counter_set.far_end.CrcNotInterleaved;
	WanIfcfgStatsCurdaySt_counter_set.far_end.FecInterleaved = WanIfcfgStatsTotal_counter_set.far_end.FecInterleaved + p_counter_set.far_end.FecInterleaved;
	WanIfcfgStatsCurdaySt_counter_set.far_end.FecNotInterleaved = WanIfcfgStatsTotal_counter_set.far_end.FecNotInterleaved + p_counter_set.far_end.FecNotInterleaved;
	WanIfcfgStatsCurdaySt_counter_set.far_end.HecInterleaved = WanIfcfgStatsTotal_counter_set.far_end.HecInterleaved + p_counter_set.far_end.HecInterleaved;
	WanIfcfgStatsCurdaySt_counter_set.far_end.HecNotInterleaved = WanIfcfgStatsTotal_counter_set.far_end.HecNotInterleaved + p_counter_set.far_end.HecNotInterleaved;
}

void cntWanDslIfcCfgQuthrStartInfo(void){
	WanIfcfgStatsQuthrSt_ShowtimeRate.ReceiveBlock = WanIfcfgStatsTotal_ShowtimeRate.ReceiveBlock + p_ShowtimeRate.ReceiveBlock;
	WanIfcfgStatsQuthrSt_ShowtimeRate.TransmitBlock = WanIfcfgStatsTotal_ShowtimeRate.TransmitBlock + p_ShowtimeRate.TransmitBlock;
	WanIfcfgStatsQuthrSt_ShowtimeRate.CellDelin = WanIfcfgStatsTotal_ShowtimeRate.CellDelin + p_ShowtimeRate.CellDelin;
	WanIfcfgStatsQuthrSt_ShowtimeRate.LinkRetrain = WanIfcfgStatsTotal_ShowtimeRate.LinkRetrain + p_ShowtimeRate.LinkRetrain;
	WanIfcfgStatsQuthrSt_ShowtimeRate.InitErrors = WanIfcfgStatsTotal_ShowtimeRate.InitErrors + p_ShowtimeRate.InitErrors;
	WanIfcfgStatsQuthrSt_ShowtimeRate.InitTimeouts = WanIfcfgStatsTotal_ShowtimeRate.InitTimeouts + p_ShowtimeRate.InitTimeouts;
	WanIfcfgStatsQuthrSt_ShowtimeRate.LossOfFraming = WanIfcfgStatsTotal_ShowtimeRate.LossOfFraming + p_ShowtimeRate.LossOfFraming;
	WanIfcfgStatsQuthrSt_ShowtimeRate.ErroredSecs = WanIfcfgStatsTotal_ShowtimeRate.ErroredSecs + p_ShowtimeRate.ErroredSecs;
	WanIfcfgStatsQuthrSt_ShowtimeRate.SeverelyErroredSecs = WanIfcfgStatsTotal_ShowtimeRate.SeverelyErroredSecs + p_ShowtimeRate.SeverelyErroredSecs;
	
	WanIfcfgStatsQuthrSt_counter_set.near_end.CrcInterleaved = WanIfcfgStatsTotal_counter_set.near_end.CrcInterleaved + p_counter_set.near_end.CrcInterleaved;
	WanIfcfgStatsQuthrSt_counter_set.near_end.CrcNotInterleaved = WanIfcfgStatsTotal_counter_set.near_end.CrcNotInterleaved + p_counter_set.near_end.CrcNotInterleaved;
	WanIfcfgStatsQuthrSt_counter_set.near_end.FecInterleaved = WanIfcfgStatsTotal_counter_set.near_end.FecInterleaved + p_counter_set.near_end.FecInterleaved;
	WanIfcfgStatsQuthrSt_counter_set.near_end.FecNotInterleaved = WanIfcfgStatsTotal_counter_set.near_end.FecNotInterleaved + p_counter_set.near_end.FecNotInterleaved;
	WanIfcfgStatsQuthrSt_counter_set.near_end.HecInterleaved = WanIfcfgStatsTotal_counter_set.near_end.HecInterleaved + p_counter_set.near_end.HecInterleaved;
	WanIfcfgStatsQuthrSt_counter_set.near_end.HecNotInterleaved = WanIfcfgStatsTotal_counter_set.near_end.HecNotInterleaved + p_counter_set.near_end.HecNotInterleaved;

	WanIfcfgStatsQuthrSt_counter_set.far_end.CrcInterleaved = WanIfcfgStatsTotal_counter_set.far_end.CrcInterleaved + p_counter_set.far_end.CrcInterleaved;
	WanIfcfgStatsQuthrSt_counter_set.far_end.CrcNotInterleaved = WanIfcfgStatsTotal_counter_set.far_end.CrcNotInterleaved + p_counter_set.far_end.CrcNotInterleaved;
	WanIfcfgStatsQuthrSt_counter_set.far_end.FecInterleaved = WanIfcfgStatsTotal_counter_set.far_end.FecInterleaved + p_counter_set.far_end.FecInterleaved;
	WanIfcfgStatsQuthrSt_counter_set.far_end.FecNotInterleaved = WanIfcfgStatsTotal_counter_set.far_end.FecNotInterleaved + p_counter_set.far_end.FecNotInterleaved;
	WanIfcfgStatsQuthrSt_counter_set.far_end.HecInterleaved = WanIfcfgStatsTotal_counter_set.far_end.HecInterleaved + p_counter_set.far_end.HecInterleaved;
	WanIfcfgStatsQuthrSt_counter_set.far_end.HecNotInterleaved = WanIfcfgStatsTotal_counter_set.far_end.HecNotInterleaved + p_counter_set.far_end.HecNotInterleaved;
}

void setCwmpStartInfo(void)
{
	/*
	  *For Cwmp Node: InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.Stats.CurrentDay.
	  *
	  *1.It will count current day's left time(the current left time will be update when NTP synchronize time successfully), 
	  *2.var cwmp_curr_day_timer_cnt will add 1 when 0.5s and we use the value to check current day is passed or not.
	  *3.Call initWanDslIfcCfgCurrDayADSLInfo() to reset the current day's ADSL info if a new day comes.
	  *4.Call cntWanDslIfcCfgCurrDayStartInfo() to count the current day's ADSL info.
	  */
	if((cwmp_curr_day_timer_cnt >> 1) < getCurrDayLeftTime()){
		cwmp_curr_day_timer_cnt++;
	}else{
		//printk("\n currentday timeour: %d \n", cwmp_curr_day_timer_cnt);
		cwmp_curr_day_timer_cnt = 0;
		setCurrDayLeftTime(SECONDS_OF_DAY);
		//setCurrDayLeftTime(60*5);
		initWanDslIfcCfgCurrDayADSLInfo();
		cntWanDslIfcCfgCurrDayStartInfo();
	}
	/*For Cwmp Node: InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.Stats.QuarterHour.*/
	  
	if((cwmp_qut_hr_timer_cnt >> 1) < getQutHrLeftTime()){
		cwmp_qut_hr_timer_cnt++;
	}else{
		//printk("\n currentday timeour: %d \n", cwmp_qut_hr_timer_cnt);
		cwmp_qut_hr_timer_cnt = 0;
		setQutHrLeftTime(SECONDS_OF_HOUR/4);
		//setQutHrLeftTime(60*2);
		initWanDslIfcCfgQuthrADSLInfo();
		cntWanDslIfcCfgQuthrStartInfo();
	}
}
#endif

#if defined(TCSUPPORT_SNMP_ATMADSL)
/*Add for snmp adsl mib*/
void SnmpADSLAturChanStatsInfoUpdate(SNMP_ADSL_MIB_ATURCHAN_NODE* p_snmp_mib_aturchan_node, PMS_PARAMS* p_pms_params, T_adsl_def_counter_set* p_counter_set, SNMP_ADSL_NODE* p_snmp_node)
{
	p_snmp_mib_aturchan_node->ATURChanRxBlks = p_pms_params->RSWords_ds; 
	p_snmp_mib_aturchan_node->ATURChanTxBlks = p_pms_params->RSWords_us;
	p_snmp_mib_aturchan_node->ATURChanCorrectBlks = p_counter_set->near_end.FecNotInterleaved; 
	p_snmp_mib_aturchan_node->ATURChanUnCorrectBlks = p_snmp_node->UcFecCnt; 
	
}

void initSnmpADSLAturChanCurrDayInfo(void){
	memset(&SnmpADSLAturChanCurrDay, 0, sizeof(SNMP_ADSL_MIB_ATURCHAN_NODE));
}

void initSnmpADSLAturChanPrevDayInfo(void){
	memset(&SnmpADSLAturChanPrevDay, 0, sizeof(SNMP_ADSL_MIB_ATURCHAN_NODE));
}

void initSnmpADSLAturChanQutHourInfo(void){
	memset(&SnmpADSLAturChanQutHour, 0, sizeof(SNMP_ADSL_MIB_ATURCHAN_NODE));
}

void initSnmpADSLAturChanTotalInfo(void){
	memset(&SnmpADSLAturChanTotal, 0, sizeof(SNMP_ADSL_MIB_ATURCHAN_NODE));
}

void initSnmpADSLAturChanIntervalInfo(void){
	memset(&SnmpADSLAturChanInterval, 0, sizeof(SNMP_ADSL_MIB_ATURCHAN_NODE)*(ADSLMIB_CHANPERFINTERVAL_MAXNUM+1));
}

void updateSnmpADSLAturChanCurrDayInfo(void){

	SnmpADSLAturChanCurrDay.ATURChanRxBlks = SnmpADSLAturChanTotal.ATURChanRxBlks + g_pms_patams.RSWords_ds;
	SnmpADSLAturChanCurrDay.ATURChanTxBlks = SnmpADSLAturChanTotal.ATURChanTxBlks + g_pms_patams.RSWords_us;
	SnmpADSLAturChanCurrDay.ATURChanCorrectBlks = SnmpADSLAturChanTotal.ATURChanCorrectBlks + g_def_counter_set.near_end.FecNotInterleaved;
	SnmpADSLAturChanCurrDay.ATURChanUnCorrectBlks = SnmpADSLAturChanTotal.ATURChanUnCorrectBlks + g_snmpNode.UcFecCnt;

}

void updateSnmpADSLAturChanQutHourInfo(void){

	SnmpADSLAturChanQutHour.ATURChanRxBlks = SnmpADSLAturChanTotal.ATURChanRxBlks + g_pms_patams.RSWords_ds; 
	SnmpADSLAturChanQutHour.ATURChanTxBlks = SnmpADSLAturChanTotal.ATURChanTxBlks + g_pms_patams.RSWords_us; 
	SnmpADSLAturChanQutHour.ATURChanCorrectBlks = SnmpADSLAturChanTotal.ATURChanCorrectBlks + g_def_counter_set.near_end.FecNotInterleaved; 
	SnmpADSLAturChanQutHour.ATURChanUnCorrectBlks = SnmpADSLAturChanTotal.ATURChanUnCorrectBlks + g_snmpNode.UcFecCnt; 
}

void updateSnmpADSLAturChanPrevDayInfo(void){
	SnmpADSLAturChanPrevDay.ATURChanRxBlks = SnmpADSLAturChanCurrDay.ATURChanRxBlks; 
	SnmpADSLAturChanPrevDay.ATURChanTxBlks = SnmpADSLAturChanCurrDay.ATURChanTxBlks; 
	SnmpADSLAturChanPrevDay.ATURChanCorrectBlks = SnmpADSLAturChanCurrDay.ATURChanCorrectBlks; 
	SnmpADSLAturChanPrevDay.ATURChanUnCorrectBlks = SnmpADSLAturChanCurrDay.ATURChanUnCorrectBlks; 
}



void updateSnmpADSLAturChanIntervalInfo(void){
	int i;

	if(snmp_qut_hr_intervels_cnt <= ADSLMIB_CHANPERFINTERVAL_MAXNUM) {
		if(snmp_qut_hr_intervels_cnt == 1) {
			//printk("\n==>updateSnmpADSLAturChanIntervalInfo,111, %ld, update 0\n", snmp_qut_hr_intervels_cnt);
			SnmpADSLAturChanInterval[0].ATURChanRxBlks = 0; 
			SnmpADSLAturChanInterval[0].ATURChanTxBlks = 0; 
			SnmpADSLAturChanInterval[0].ATURChanCorrectBlks = 0; 
			SnmpADSLAturChanInterval[0].ATURChanUnCorrectBlks = 0; 		
		}

		i = snmp_qut_hr_intervels_cnt;
		//printk("\n==>updateSnmpADSLAturChanIntervalInfo,111, %ld, update %d\n", snmp_qut_hr_intervels_cnt, i);
		SnmpADSLAturChanInterval[i].ATURChanRxBlks = SnmpADSLAturChanTotal.ATURChanRxBlks + g_pms_patams.RSWords_ds; 
		SnmpADSLAturChanInterval[i].ATURChanTxBlks = SnmpADSLAturChanTotal.ATURChanTxBlks + g_pms_patams.RSWords_us; 
		SnmpADSLAturChanInterval[i].ATURChanCorrectBlks = SnmpADSLAturChanTotal.ATURChanCorrectBlks + g_def_counter_set.near_end.FecNotInterleaved; 
		SnmpADSLAturChanInterval[i].ATURChanUnCorrectBlks = SnmpADSLAturChanTotal.ATURChanUnCorrectBlks + g_snmpNode.UcFecCnt; 
	}
	else {

		for(i=0; i<ADSLMIB_CHANPERFINTERVAL_MAXNUM; i++) {
			//printk("\n==>updateSnmpADSLAturChanIntervalInfo,222, %ld, update %d\n", snmp_qut_hr_intervels_cnt, i);
			memcpy(&SnmpADSLAturChanInterval[i], &SnmpADSLAturChanInterval[i+1], sizeof(SNMP_ADSL_MIB_ATURCHAN_NODE));
		}
		
		//printk("\n==>updateSnmpADSLAturChanIntervalInfo,222, %ld, update 4\n", snmp_qut_hr_intervels_cnt);
		SnmpADSLAturChanInterval[ADSLMIB_CHANPERFINTERVAL_MAXNUM].ATURChanRxBlks = SnmpADSLAturChanTotal.ATURChanRxBlks + g_pms_patams.RSWords_ds; 
		SnmpADSLAturChanInterval[ADSLMIB_CHANPERFINTERVAL_MAXNUM].ATURChanTxBlks = SnmpADSLAturChanTotal.ATURChanTxBlks + g_pms_patams.RSWords_us; 
		SnmpADSLAturChanInterval[ADSLMIB_CHANPERFINTERVAL_MAXNUM].ATURChanCorrectBlks = SnmpADSLAturChanTotal.ATURChanCorrectBlks + g_def_counter_set.near_end.FecNotInterleaved; 
		SnmpADSLAturChanInterval[ADSLMIB_CHANPERFINTERVAL_MAXNUM].ATURChanUnCorrectBlks = SnmpADSLAturChanTotal.ATURChanUnCorrectBlks + g_snmpNode.UcFecCnt; 		

	}
}

void initSnmpADSLAR5211TotalInfo(void){
	memset(&SnmpADSLCTAR5211Total, 0, sizeof(SNMP_ADSL_MIB_CTAR5211_NODE));
}

void SnmpADSLAR5211StatsInfoUpdate(SNMP_ADSL_MIB_CTAR5211_NODE* p_snmp_mib_ar5211_node, CELL_COUNT1* p_cell_count1, SNMP_ADSL_NODE_2* p_snmp_node2)
{
	p_snmp_mib_ar5211_node->DsEs = p_cell_count1->DsEs; 
	p_snmp_mib_ar5211_node->DsSes = p_cell_count1->DsSes;
	p_snmp_mib_ar5211_node->UsEs = p_cell_count1->UsEs;
	p_snmp_mib_ar5211_node->UsSes = p_cell_count1->UsSes;
	
	p_snmp_mib_ar5211_node->RxUAS = p_snmp_node2->RxUAS;
	p_snmp_mib_ar5211_node->TxUAS = p_snmp_node2->TxUAS;
}

int querySnmpAdslInfo(void)
{
	if (adsl_dev_ops == NULL)
		return 0;

	adsl_dev_ops->query(ADSL_QUERY_PMS_PARAM, &g_pms_patams, NULL);
	adsl_dev_ops->query(ADSL_QUERY_DEFECT_CNT, &g_def_counter_set, NULL );
	adsl_dev_ops->query(ADSL_QUERY_SNMP_ADSL_NODE, &g_snmpNode, NULL);

	adsl_dev_ops->query(ADSL_QUERY_CELL_CNT1, &g_cellOperdata, NULL);
	adsl_dev_ops->query(ADSL_QUERY_STATISTICS_2, &g_snmpAdslNode_2, NULL);
	return 0;
}

void setSnmpAdslStatsInfo(void)
{
	/*
	  *For Snmp adsl mib Node
	  */
	/*Check curr day*/
	if((snmp_curr_day_timer_cnt >>1) < (getCurrDayLeftTime())){
		snmp_curr_day_timer_cnt++;
	}else{
		//printk("\n currentday timeour: %ld \n", snmp_curr_day_timer_cnt);
		snmp_curr_day_timer_cnt = 0;
		setCurrDayLeftTime(SECONDS_OF_DAY);
		querySnmpAdslInfo();		

		initSnmpADSLAturChanPrevDayInfo();
		updateSnmpADSLAturChanPrevDayInfo();

		initSnmpADSLAturChanCurrDayInfo();
		updateSnmpADSLAturChanCurrDayInfo();
	}
	  
	/*Check 15 mins*/
	if((snmp_qut_hr_timer_cnt >>1) < (getQutHrLeftTime())){
		snmp_qut_hr_timer_cnt++;
	}else{
		//printk("\n quarterHour timeour: %d \n", snmp_qut_hr_timer_cnt);
		snmp_qut_hr_timer_cnt = 0;
		
		snmp_qut_hr_intervels_cnt++;
		
		setQutHrLeftTime(SECONDS_OF_HOUR>>2);
		querySnmpAdslInfo();		
		
		initSnmpADSLAturChanQutHourInfo();
		updateSnmpADSLAturChanQutHourInfo();

		updateSnmpADSLAturChanIntervalInfo();
	}
}
#endif

int
getXdslSpeed(void)
{
	uint8 modemst;
	T_adslChannelOperData adslChannelOperData;

#if defined(TCSUPPORT_CPU_MT7510) || defined(TCSUPPORT_CPU_MT7505) || defined(TCSUPPORT_FTTDP_V2)
if (isFPGA)
{
	return fpgaAdslUpRate;
}
else
{
#endif
	if (adsl_dev_ops == NULL)
		return 0;

	adsl_dev_ops->query(ADSL_QUERY_STATUS, &modemst, NULL);		   //Jason_920527
	if (modemst != ADSL_MODEM_STATE_UP)
		return 0;

	adsl_dev_ops->query(ADSL_QUERY_CH_OP_DATA, &adslChannelOperData, NULL); 		 //Jason_920527
	return (adslChannelOperData.actualBitrateFarFast+adslChannelOperData.actualBitrateFarInt);
#if defined(TCSUPPORT_CPU_MT7510) || defined(TCSUPPORT_CPU_MT7505) || defined(TCSUPPORT_FTTDP_V2)
}
#endif
}

uint8
getXdslModeType(void)
{
	uint8 modemst;
	uint8 annex_set;

	if (adsl_dev_ops == NULL)
		return 0;

	adsl_dev_ops->query(ADSL_QUERY_STATUS, &modemst, NULL);		   //Jason_920527
	if (modemst != ADSL_MODEM_STATE_UP)
		return 0;

	adsl_dev_ops->query(ADSL_QUERY_ANNEX, &annex_set, NULL);   

	if(!annex_set)
	{
		return 0;
	}

	return annex_set;
}

void os_TCIfQuery(unsigned short query_id, void *result1, void *result2)
{
	if (adsl_dev_ops == NULL)
		return;

	adsl_dev_ops->query(query_id, result1, result2);
}
EXPORT_SYMBOL(os_TCIfQuery);

void os_TCIfSet(unsigned short set_id, void *value1, void *value2)
{
	if (adsl_dev_ops == NULL)
		return;

	adsl_dev_ops->set(set_id, value1, value2);
}
EXPORT_SYMBOL(os_TCIfSet);

void adslTimer(unsigned long data)
{
	static uint8 lastModemst = 0xFF; /*init lastModemst as a value not same as any valid modem state*/
	uint8 modemst;

	#ifdef CWMP
	if (needInitADSLInfoFlag == 1){
		resetWanDslIfcCfgTotalADSLInfo();
		resetWanDslIfcCfgLastShowTimeADSLInfo();
		//initWanDslIfcCfgCurrDayADSLInfo();
		needInitADSLInfoFlag = 0;
	}
	#endif
	
	if (adsl_dev_ops == NULL)
		goto down_proc;

	adsl_dev_ops->query(ADSL_QUERY_STATUS, &modemst, NULL);
	switch (modemst) {
		case ADSL_MODEM_STATE_DOWN:
down_proc:
#if defined(TCSUPPORT_C1_NEW_GUI) || defined(TCSUPPORT_CT)
			if (lastModemst != ADSL_MODEM_STATE_DOWN){
				ledTurnOff(LED_DSL_HANDSHAKE_STATUS);
				ledTurnOff(LED_DSL_TRAINING_STATUS);
				ledTurnOff(LED_DSL_SHOWTIME_STATUS);
			}
#else
			ledTurnOn(LED_DSL_IDLE_STATUS);
#endif
			if (ledGetMode(LED_DSL_IDLE_STATUS) == LED_MODE_NOT_USED)
				ledTurnOff(LED_DSL_STATUS);
			ledTurnOff(LED_DSL_ACT_STATUS);
			ledTurnOff(LED_DSL_NOACT_STATUS);
			ledTurnOff(LED_PPP_STATUS);
			ledTurnOff(LED_PPP_ACT_STATUS);
			ledTurnOff(LED_PPP_NOACT_STATUS);
			ledTurnOn(LED_DSL_ALARM);

			#if defined(CWMP) || defined(TCSUPPORT_SNMP_ATMADSL)
			if (lastModemst == ADSL_MODEM_STATE_UP)
			{
				flagLastShowTimeStart = 1;  /*1 means last show time start*/
				resetWanDslIfcCfgLastShowTimeADSLInfo();
				
				StructTWanDslIfcCfgInfoAdd(&WanIfcfgStatsLastShowTime_ShowtimeRate, &WanIfcfgStatsLastShowTime_counter_set, &p_ShowtimeRate, &p_counter_set);
				StructTWanDslIfcCfgInfoAdd(&WanIfcfgStatsTotal_ShowtimeRate, &WanIfcfgStatsTotal_counter_set, &p_ShowtimeRate, &p_counter_set);

			#if defined(TCSUPPORT_SNMP_ATMADSL)
				querySnmpAdslInfo();
				SnmpADSLAturChanStatsInfoUpdate(&SnmpADSLAturChanTotal, &g_pms_patams, &g_def_counter_set, &g_snmpNode);

				SnmpADSLAR5211StatsInfoUpdate(&SnmpADSLCTAR5211Total, &g_cellOperdata, &g_snmpAdslNode_2);
				/*reset the adsl info*/
				memset(&g_pms_patams, 0, sizeof(PMS_PARAMS));
				memset(&g_def_counter_set, 0, sizeof(T_adsl_def_counter_set));
				memset(&g_snmpNode, 0, sizeof(SNMP_ADSL_NODE));

				memset(&g_cellOperdata, 0, sizeof(CELL_COUNT1));
				memset(&g_snmpAdslNode_2, 0, sizeof(SNMP_ADSL_NODE_2));
			#endif	
			}
			#endif
			lastModemst = ADSL_MODEM_STATE_DOWN;
			break;
		case ADSL_MODEM_STATE_WAIT_INIT:
#if !defined(TCSUPPORT_C1_NEW_GUI) 
			ledTurnOn(LED_DSL_IDLE_STATUS);
#endif
#if !defined(TCSUPPORT_CHT_DSL_LED) 
			if (ledGetMode(LED_DSL_IDLE_STATUS) == LED_MODE_NOT_USED)
				ledTurnOff(LED_DSL_STATUS);
			ledTurnOff(LED_DSL_ACT_STATUS);
#endif
			ledTurnOff(LED_DSL_NOACT_STATUS);
			ledTurnOff(LED_PPP_STATUS);
			ledTurnOff(LED_PPP_ACT_STATUS);
			ledTurnOff(LED_PPP_NOACT_STATUS);
			/*pork 20090325 added*/
			adslActiveTime = jiffies_to_msecs(jiffies);
			/*pork 20090325 added over*/
			/* polling ethernet */
			//if (!(VPint(CR_INTC_IMR) & (1 << MAC_INT)))
			//		tc3162_emac_poll();
			lastModemst = ADSL_MODEM_STATE_WAIT_INIT;
			break;
		case ADSL_MODEM_STATE_INIT:
			ledTurnOn(LED_DSL_TRAINING_STATUS);

			/* polling ethernet */
			//if (!(VPint(CR_INTC_IMR) & (1 << MAC_INT)))
			//	tc3162_emac_poll();
			lastModemst = ADSL_MODEM_STATE_INIT;
			break;
		case ADSL_MODEM_STATE_UP:
			if (lastModemst != ADSL_MODEM_STATE_UP) {
				adslUpTime = jiffies_to_msecs(jiffies);
				#ifdef CWMP
				adslShowTimeStart = adslUpTime - adslActiveTime;
				adslTotalStart += adslShowTimeStart;
				#endif
				ledTurnOn(LED_DSL_SHOWTIME_STATUS);
				ledTurnOn(LED_DSL_NOACT_STATUS);
				ledTurnOff(LED_DSL_ALARM);
			}
			#if defined(TCSUPPORT_WLAN_GPIO)
			#if defined(RT5392) || defined(RT5390) || defined(RT5390HL)	|| defined(MT7592)
			if(chkWlanGpioMode(getDslLedGpio())==0){
				set_wlan_gpio_mode();
				ledTurnOn(LED_DSL_SHOWTIME_STATUS);
				ledTurnOn(LED_DSL_NOACT_STATUS);
				ledTurnOff(LED_DSL_ALARM);	
			}
			#endif
			#endif
			#ifdef CWMP
			//add totalstart and showtimestart of tr069  		
			if (adsl_dev_ops == NULL) {
				printk( "\nADSL link status: down\n");
			}else{			
				adsl_dev_ops->query(ADSL_QUERY_TR69_WLAN_DSL_INTERFACE_Config, &p_ShowtimeRate, NULL );
				adsl_dev_ops->query(ADSL_QUERY_DEFECT_CNT, &p_counter_set, NULL );
				
				if ((initLinkRetrain_flag ==1) && (lastModemst != ADSL_MODEM_STATE_UP)) {
					initLinkRetrainCount = p_ShowtimeRate.LinkRetrain;
					initLinkRetrain_flag = 0;
				}
			}
			#endif
			lastModemst = ADSL_MODEM_STATE_UP;
			break;
	}
	#ifdef CWMP
	setCwmpStartInfo();
	#endif
#if defined(TCSUPPORT_SNMP_ATMADSL)
	setSnmpAdslStatsInfo();	
#endif
	mod_timer(&adsl_timer, jiffies + (HZ / 2));
}

/************************************************************************
*		     A D S L   C I   C O M M A N D
*************************************************************************
*/
/* frank add line bond support 20120801 */
static adsldev_ops* get_adsl_dev_ops(void *p)
{
	char *str = (char *)p;
	if (p == NULL)
	{
        printk("Error(%s): NULL is returned\n", __FUNCTION__);
		return NULL;
	}
	
#if defined(TCSUPPORT_CPU_MT7510) && defined(TCSUPPORT_BONDING)	
	if (strcmp(str, "adsl_slave")==0){
		return adsl_dev_ops_slave;
	} else 
#endif
		return adsl_dev_ops;
	
}

adsldev_ops* adsl_dev_ops_get(int lineId)
{
	if (lineId == LINE0)
	{
		return adsl_dev_ops;
	}
#if defined(TCSUPPORT_CPU_MT7510) && defined(TCSUPPORT_BONDING)
	else if (lineId == LINE1)
	{
		return adsl_dev_ops_slave;
	}
#endif
	else
	{
		printk("\nWrong: Line ID: %d\n", lineId);
		return NULL;
	}
}
EXPORT_SYMBOL(adsl_dev_ops_get);

static void adslReset(adsldev_ops *ops)
{
	uint32 mask = 0xFFFFFFFE;
	uint16 flag = 1;	

	if (ops == NULL)
		return;

	ops->set(ADSL_SET_DMT_CLOSE, NULL, NULL);
	ops->set(ADSL_SET_R_SOFT_RESET, &flag, NULL);
   	ops->set(ADSL_SET_INT_MASK0, &mask, NULL );
}

int doAdsl(int argc, char *argv[], void *p)
{
	p = argv[0];
	return subcmd(adslCmds, argc, argv, p);
}

int doAdslReset(int argc, char *argv[], void *p)
{
	char *str = (char *)p;
	adsldev_ops *ops = get_adsl_dev_ops(p);

	if (ops==NULL)
		return 0;

	printk("string: %s\n", str);
	adslReset(ops);
	return 0;
}

int doAdslOpenCmd(int argc, char *argv[], void *p)
{
	return subcmd(adslOpenCmds, argc, argv, p);
}

int doAdslOpen(int argc, char *argv[], void *p)
{
	uint8 modemst;
	uint32 mask = 0xFFFFFFFE;
	uint16 flag = 1;
	adsldev_ops *ops = get_adsl_dev_ops(p);

	if (ops == NULL)
		return 0;

	ops->query(ADSL_QUERY_STATUS, &modemst, NULL );
	if (modemst == ADSL_MODEM_STATE_UP) {
		#ifdef CMD_API
		memset(adslinfobuf,0,sizeof(adslinfobuf));
		strcpy(adslinfobuf,"ADSL line is up.\n");
		#else
		printk("ADSL line is up.\n");
		#endif
		return 0;
	}
	
	ops->set(ADSL_SET_R_SOFT_RESET, &flag, NULL);
   	ops->set(ADSL_SET_INT_MASK0, &mask, NULL );
	return 0;
}

int doAdslClose(int argc, char *argv[], void *p)
{
	adsldev_ops *ops = get_adsl_dev_ops(p);

	if (ops == NULL)
		return 0;

	ops->set(ADSL_SET_DMT_CLOSE, NULL, NULL);
	return 0;
}

int doAdslGlite(int argc, char *argv[], void *p)
{
	adsldev_ops *ops = get_adsl_dev_ops(p);

	if (ops == NULL)
		return 0;

	ops->set(ADSL_SET_MODE_LITE, NULL, NULL);
	adslReset(ops);
	return 0;
}

int doAdslT1413(int argc, char *argv[], void *p)
{
	adsldev_ops *ops = get_adsl_dev_ops(p);

	if (ops == NULL)
		return 0;

	ops->set(ADSL_SET_MODE_ANSI, NULL, NULL);
	adslReset(ops);
	return 0;
}

int doAdslGdmt(int argc, char *argv[], void *p)
{
	adsldev_ops *ops = get_adsl_dev_ops(p);

	if (ops == NULL)
		return 0;

	ops->set(ADSL_SET_MODE_A43, NULL, NULL);
	adslReset(ops);
	return 0;
}

int doAdslMultimode(int argc, char *argv[], void *p)
{
	adsldev_ops *ops = get_adsl_dev_ops(p);

	if (ops == NULL)
		return 0;

	ops->set(ADSL_SET_MODE_MULTIMODE, NULL, NULL);
	adslReset(ops);
	return 0;
}

int doAdslAdsl2(int argc, char *argv[], void *p)
{
	adsldev_ops *ops = get_adsl_dev_ops(p);
	if (ops == NULL)
		return 0;

	ops->set(ADSL_SET_MODE_ADSL2, NULL, NULL);
	adslReset(ops);
	return 0;
}

int doAdslAdsl2plus(int argc, char *argv[], void *p)
{
	adsldev_ops *ops = get_adsl_dev_ops(p);
	if (ops == NULL)
		return 0;

	ops->set(ADSL_SET_MODE_ADSL2PLUS, NULL, NULL);
	adslReset(ops);
	return 0;
}

#ifdef CONFIG_RALINK_VDSL
int doAdslVdsl2(int argc, char *argv[], void *p)
{
	adsldev_ops *ops = get_adsl_dev_ops(p);
	if (ops == NULL)
		return 0;

	ops->set(ADSL_SET_MODE_VDSL2, NULL, NULL);
	adslReset(ops);
	return 0;
}
#endif
#if defined(TCSUPPORT_CWMP_VDSL)
int doAdsl2plusMulti(int argc, char *argv[], void *p)
{
	adsldev_ops *ops = get_adsl_dev_ops(p);

	if (ops == NULL)
		return 0;

	ops->set(ADSL_SET_MODE_ADSL2PLUS_MULTI, NULL, NULL);
	adslReset(ops);
	return 0;
}
#else
#if defined(TCSUPPORT_CT_WAN_PTM)
int doAdsl2plusMulti(int argc, char *argv[], void *p)
{
	if (adsl_dev_ops == NULL)
		return 0;

	adsl_dev_ops->set(ADSL_SET_MODE_ADSL2PLUS_MULTI, NULL, NULL);
	adslReset(adsl_dev_ops);
	return 0;
}
#endif
#endif
int
getADSLFwVer(char *modemCode, adsldev_ops *ops)
{
	uint16	index=0;

	int i=0;
	uint8 fw_ver_r, fw_ver_p, fw_rev, tdm_ver, fdm_ver, annex_afe, b_ver;
	uint8   major;
	uint8   minor;

	if (modemCode == NULL)
	{
		printk("\n(%s) modemCode is NULL\n", __FUNCTION__);
		return 0;
	}
	else
		modemCode[0] = '\0';

	if (ops == NULL)
		return 0;

	ops->query(ADSL_QUERY_HW_VER, &major, &minor);           //Jason_920529
   	ops->query(ADSL_QUERY_FW_VER, &fw_ver_r, &fw_ver_p);     //Jason_920527
   	ops->query(ADSL_QUERY_FW_REV, &fw_rev, &annex_afe);      //Jason_920527
   	ops->query(ADSL_QUERY_TDM_FDM_VER, &tdm_ver, &fdm_ver);  //Jason_920527
   	ops->query(ADSL_QUERY_BUILD_VERSION, &b_ver, NULL ); //Jason_931108

	index = sprintf( modemCode," FwVer:");
	index += sprintf( modemCode+index, "%d.", fw_ver_r);
	index += sprintf( modemCode+index, "%d.", fw_ver_p);
	index += sprintf( modemCode+index, "%d.", fw_rev);
	index += sprintf( modemCode+index, "%d_", b_ver);
	index += sprintf( modemCode+index, "%c_", (annex_afe & 0x02)?'A':'B');

	annex_afe = annex_afe & 0xe0;
#if 0
	if (annex_afe == 0x20)
		index += sprintf( modemCode+index, "%s", "TC3084");
	else if (annex_afe == 0x40)
		index += sprintf( modemCode+index, "%s", "TC3085");
	else
		index += sprintf( modemCode+index, "%s", "TC3084");
#else
	for(i=0; xdsl_fw_ver[i].ver_name!=NULL; i++){
		if(annex_afe == xdsl_fw_ver[i].id){
			index += sprintf( modemCode+index, "%s", xdsl_fw_ver[i].ver_name);
			break;
		}
	}
#endif
	index += sprintf( modemCode+index, " HwVer:T%d.F%d_%d.%d\n", tdm_ver, fdm_ver, major, minor);

	return index;
}

int doAdslFwVersion(int argc, char *argv[], void *p)
{
	char fwver[80];
	adsldev_ops *ops = get_adsl_dev_ops(p);

	getADSLFwVer(fwver, ops);
	printk("%s", fwver);
	return 0;
}

int doAdslStatus(int argc, char *argv[], void *p)
{
	uint8 modemst;
	adsldev_ops *ops = get_adsl_dev_ops(p);

	if (ops == NULL) {
		printk("ADSL link status: down\n");
		return 0;
	}

	ops->query(ADSL_QUERY_STATUS, &modemst, NULL );
	if (modemst == ADSL_MODEM_STATE_DOWN)
		printk("ADSL link status: down\n");
	else if (modemst == ADSL_MODEM_STATE_WAIT_INIT)
		printk("ADSL link status: wait for init\n");
	else if (modemst == ADSL_MODEM_STATE_INIT)
		printk("ADSL link status: initializing\n");
	else if (modemst == ADSL_MODEM_STATE_UP)
		printk("ADSL link status: up\n");
	else
		printk("ADSL link status: unknown\n");
	return 0;
}

int doAdslChanndata(int argc, char *argv[], void *p)
{
	uint8 modemst;
	T_adslChannelOperData adslChannelOperData;
	adsldev_ops *ops = get_adsl_dev_ops(p);

	if (ops == NULL)
		return 0;

	ops->query(ADSL_QUERY_STATUS, &modemst, NULL );
	if (modemst != ADSL_MODEM_STATE_UP)
		return 0;

	ops->query(ADSL_QUERY_CH_OP_DATA, &adslChannelOperData, NULL);

	printk("near-end interleaved channel bit rate: %u kbps\n",
				adslChannelOperData.actualBitrateNearInt);
	printk("near-end fast channel bit rate: %u kbps\n",
				adslChannelOperData.actualBitrateNearFast);
	printk("far-end interleaved channel bit rate: %u kbps\n",
				adslChannelOperData.actualBitrateFarInt);
	printk("far-end fast channel bit rate: %u kbps\n",
				adslChannelOperData.actualBitrateFarFast);
	return 0;
}

int doAdslPerfdata(int argc, char *argv[], void *p)
{
	uint8 modemst;
    T_adsl_def_counter_set adsl_def_counter_set;
#if 0
	unsigned int now = jiffies_to_msecs(jiffies);
	int updays, uphours, upminutes;
#endif
	char tmpTimeStr[32];
	adsldev_ops *ops = get_adsl_dev_ops(p);
	if (ops == NULL)
		return 0;

	ops->query(ADSL_QUERY_STATUS, &modemst, NULL );
	if (modemst != ADSL_MODEM_STATE_UP)
		return 0;

    ops->query(ADSL_QUERY_DEFECT_CNT, &adsl_def_counter_set, NULL);

	printk("near-end FEC error fast: %u\n",
						adsl_def_counter_set.near_end.FecNotInterleaved);
	printk("near-end FEC error interleaved: %u\n",
						adsl_def_counter_set.near_end.FecInterleaved);
	printk("near-end CRC error fast: %u\n",
						adsl_def_counter_set.near_end.CrcNotInterleaved);
	printk("near-end CRC error interleaved: %u\n",
						adsl_def_counter_set.near_end.CrcInterleaved);
	printk("near-end HEC error fast: %u\n",
						adsl_def_counter_set.near_end.HecNotInterleaved);
	printk("near-end HEC error interleaved: %u\n",
						adsl_def_counter_set.near_end.HecInterleaved);
	printk("far-end FEC error fast: %u\n",
						adsl_def_counter_set.far_end.FecNotInterleaved);
	printk("far-end FEC error interleaved: %u\n",
						adsl_def_counter_set.far_end.FecInterleaved);
	printk("far-end CRC error fast: %u\n",
						adsl_def_counter_set.far_end.CrcNotInterleaved);
	printk("far-end CRC error interleaved: %u\n",
						adsl_def_counter_set.far_end.CrcInterleaved);
	printk("far-end HEC error fast: %u\n",
						adsl_def_counter_set.far_end.HecNotInterleaved);
	printk("far-end HEC error interleaved: %u\n",
						adsl_def_counter_set.far_end.HecInterleaved);
	doAdslUpTimeCal(tmpTimeStr);
	printk("ADSL uptime :%s",tmpTimeStr);

#if 0
	now = now - adslUpTime;
	now = now / MSEC_PER_SEC;

	printk("ADSL uptime : ");
	tmpStrIdx += sprintf( tmpADSLUpTime+tmpStrIdx, "ADSL uptime :");
	updays = (int) now / (60*60*24);
	if (updays){
		printk("%d day%s, ", updays, (updays != 1) ? "s" : "");
		tmpStrIdx += sprintf( tmpADSLUpTime+tmpStrIdx,"%d day%s, ", updays, (updays != 1) ? "s" : "");
	}
	upminutes = (int) now / 60;
	uphours = (upminutes / 60) % 24;
	upminutes %= 60;
	if(uphours){
		printk("%2d:%02d, ", uphours, upminutes);
		tmpStrIdx += sprintf( tmpADSLUpTime+tmpStrIdx,"%2d:%02d, ", uphours, upminutes);
	}
	else{
		printk("%d min, ", upminutes);
		tmpStrIdx += sprintf( tmpADSLUpTime+tmpStrIdx,"%d min, ", upminutes);
	}
	printk("%d secs\n", now % 60);
	tmpStrIdx += sprintf( tmpADSLUpTime+tmpStrIdx,"%d secs\n", now % 60);
	strcpy(tmp);
#endif
	return 0;
}
void doAdslUpTimeCal(char* tmpUpTimeStrRet){
	unsigned int now = jiffies_to_msecs(jiffies);
	now = now - adslUpTime;
	timeFormatRet(tmpUpTimeStrRet,now);

}
void doAdslActiveTimeCal(char* tmpUpTimeStrRet){
	unsigned int tmpTime = adslUpTime - adslActiveTime;
	timeFormatRet(tmpUpTimeStrRet,tmpTime);

}

void timeFormatSec(char* strTime, int time){
	int tmpTime = time;
	tmpTime = time / MSEC_PER_SEC;
	sprintf(strTime, "%d", tmpTime);
}
void timeFormatRet(char* strRet,int tmpTime){
	int tmpStrIdx = 0;
	int updays, uphours, upminutes;
	tmpTime = tmpTime / MSEC_PER_SEC;
	updays = (int) tmpTime / (60*60*24);
	if (updays){
		tmpStrIdx += sprintf( strRet+tmpStrIdx,"%d day%s, ", updays, (updays != 1) ? "s" : "");
	}
	upminutes = (int) tmpTime / 60;
	uphours = (upminutes / 60) % 24;
	upminutes %= 60;
	if(uphours){
		tmpStrIdx += sprintf( strRet+tmpStrIdx,"%2d:%02d, ", uphours, upminutes);
	}
	else{
		tmpStrIdx += sprintf( strRet+tmpStrIdx,"%d min, ", upminutes);
	}
	tmpStrIdx += sprintf(strRet+tmpStrIdx,"%d secs\n", tmpTime % 60);

}
int doAdslLinedata(int argc, char *argv[], void *p)
{
	return subcmd(adslLinedataCmds, argc, argv, p);
}

int doAdslLinedataFar(int argc, char *argv[], void *p)
{
	uint8 modemst;
	T_NTadslFarEndLineOperData adslFarEndLineOperData;
	uint8 link_mode;
	int i, j, line_num;

	adsldev_ops *ops = get_adsl_dev_ops(p);
	if (ops == NULL)
		return 0;

	ops->query(ADSL_QUERY_STATUS, &modemst, NULL );
	if (modemst != ADSL_MODEM_STATE_UP)
		return 0;

	ops->query(ADSL_QUERY_FAR_OP_DATA, &adslFarEndLineOperData, NULL);

	printk("relative capacity occupation: %u%% \n",
			adslFarEndLineOperData.relCapacityOccupationUpstr);
	printk("noise margin upstream: %d.%d dB\n",
			(adslFarEndLineOperData.noiseMarginUpstr/10),(adslFarEndLineOperData.noiseMarginUpstr%10)); //Jason_931028
	printk("output power downstream: %d.%d dbm\n",
			(adslFarEndLineOperData.outputPowerDnstr/10),(adslFarEndLineOperData.outputPowerDnstr%10)); //Jason_931028
	printk("attenuation upstream: %d.%d dB\n",
			(adslFarEndLineOperData.attenuationUpstr/10),(adslFarEndLineOperData.attenuationUpstr%10));//Jason_931028

	printk("\nBit table:\n");
	ops->query(ADSL_QUERY_MODE, &link_mode, NULL);//Jason_931130
	if( link_mode == ME_CMD_ADSL2PLUS )    		 //Jason_931130
		line_num = 16;
	else
		line_num = 8;
	printk("carrier load: number of bits per tone\n");
	for(i=0; i<line_num; i++) {	//Jason_931130
		printk("tone %3d-%3d: ",i*32, i*32+31);
		for (j=0; j<16; j++) {
			printk("%1x", adslFarEndLineOperData.carrierLoad[i*16+j] & 0xF);
			printk("%1x ", (adslFarEndLineOperData.carrierLoad[i*16+j] & 0xF0) >> 4);
		}
		printk("\n");
	}

	return 0;
}

int doAdslLinedataNear(int argc, char *argv[], void *p)
{
	uint8 modemst;
	T_NTadslNearEndLineOperData adslNearEndLineOperData;
	adsldev_ops *ops = get_adsl_dev_ops(p);

	if (ops == NULL)
		return 0;

	ops->query(ADSL_QUERY_STATUS, &modemst, NULL );
	if (modemst != ADSL_MODEM_STATE_UP)
		return 0;

	ops->query(ADSL_QUERY_NEAR_OP_DATA, &adslNearEndLineOperData, NULL);

	printk("relative capacity occupation: %u%% \n",
			adslNearEndLineOperData.relCapacityOccupationDnstr);
	printk("noise margin downstream: %d.%d dB\n",
			adslNearEndLineOperData.noiseMarginDnstr/10, abs(adslNearEndLineOperData.noiseMarginDnstr%10));
	printk("output power upstream: %d.%01d dbm\n",
			adslNearEndLineOperData.outputPowerUpstr/10, abs(adslNearEndLineOperData.outputPowerUpstr%10));
	printk("attenuation downstream: %d.%01d dB\n",
			adslNearEndLineOperData.attenuationDnstr/10, adslNearEndLineOperData.attenuationDnstr%10);

	return 0;
}

int doAdslOpmode(int argc, char *argv[], void *p)
{
	uint8 modemst;
	uint8 mode;
	adsldev_ops *ops = get_adsl_dev_ops(p);

	if (ops == NULL)
		return 0;

	ops->query(ADSL_QUERY_STATUS, &modemst, NULL );
	if (modemst != ADSL_MODEM_STATE_UP)
		return 0;

	ops->query(ADSL_QUERY_MODE, &mode, NULL);
	printk("Opmode: ");
	switch(mode) {
		case ME_CMD_ADSL_ANSI:
	    	printk("ANSI T1.413\n");
			break;
		case ME_CMD_ADSL_OPEN_GDMT:
	   		printk("ITU G.992.1(G.DMT)\n");
			break;
		case ME_CMD_ADSL_OPEN_GLITE:
       		printk("ITU G.992.2(G.Lite)\n");
			break;
		case ME_CMD_ADSL2:
       		printk("ITU G.992.3(ADSL2)\n");
			break;
		case ME_CMD_ADSL2PLUS:
       		printk("ITU G.992.5(ADSL2PLUS)\n");
			break;
		case ME_CMD_VDSL2:
       		printk("ITU G.993.2(VDSL2)\n");
			break;
		case ME_CMD_GVECTOR:
       		printk("ITU G.993.5(G.Vectoring)\n");
			break;		
		case ME_CMD_GVECTOR_GINP:
       		printk("ITU G.993.5(G.Vectoring),G.998.4(G.INP)\n");
			break;
		case ME_CMD_VDSL2_GINP:
       		printk("ITU G.993.2(VDSL2), G.998.4(G.INP)\n");
			break;
		case ME_CMD_ADSL2_GINP:
       		printk("ITU G.992.3(ADSL2) ,G.998.4(G.INP)\n");
			break;		
		case ME_CMD_ADSL2PLUS_GINP:
       		printk("ITU G.992.5(ADSL2PLUS) ,G.998.4(G.INP)\n");
			break;
		default:
			printk("Wrong opmode!\n");
	}
#ifdef CONFIG_RALINK_VDSL
	ops->query(ADSL_QUERY_RX_BEAR_TPSTC_TYPE, &mode, NULL);
	printk("TPSTC type: ");
	switch(mode) {
		case TPSTC_DISABLE:
	    	printk("disable\n");
			break;
		case TPSTC_ATM_TC:
	    	printk("ATM TC\n");
			break;
		case TPSTC_PTM_TC_64_65B:
	    	printk("64/65B PTM TC\n");
			break;
		case TPSTC_PTM_TC_HDLC:
	    	printk("HDLC PTM TC\n");
			break;
	}
#endif
	return 0;
}

#if defined(TCSUPPORT_CPU_MT7510) && defined(TCSUPPORT_BONDING)
int doBondingBacp(int argc, char *argv[], void *p)
{
    char *str = argv[1];
    adsldev_ops *ops = get_adsl_dev_ops(p);
    unsigned char value;


    if (strcmp(str, "0")==0){
        value = BONDING_OFF_BACP_OFF;
        printk("\nBONDING_OFF_BACP_OFF\n");
    }
    else if (strcmp(str, "1")==0){
        value = BONDING_ON_BACP_OFF;
        printk("\nBONDING_ON_BACP_OFF\n");
    }
    else if (strcmp(str, "2")==0){
        value = BONDING_OFF_BACP_ON;
        printk("\nBONDING_OFF_BACP_ON\n");
    }
    else if (strcmp(str, "3")==0){
        value = BONDING_ON_BACP_ON;
        printk("\nBONDING_ON_BACP_ON\n");
    }
    else
    {
        printk("\nWrong value for setting bonding & BACP\n");
        return -1;
    }

    ops->set(TCIF_SET_BONDING_BACP_SUPPORT, &value, NULL);

    return 0;
}
#endif
int doAdslDiag(int argc, char *argv[], void *p)
{
	adsldev_ops *ops = get_adsl_dev_ops(p);

	if (ops == NULL)
		return 0;

	ops->query(ADSL_QUERY_SHOW_DIAG, NULL, NULL);
	return 0;
}

int doAdslNearItuId(int argc, char *argv[], void *p)
{
	int i;
	uint8 *r_vendor_id = NULL;
	adsldev_ops *ops = get_adsl_dev_ops(p);
	
	if (ops == NULL)
		return 0;

	ops->query(ADSL_QUERY_NEAR_ITUID, &r_vendor_id, NULL);
	
	if(r_vendor_id == NULL)
		return 0;

	printk("near end itu identification: ");
    	for (i = 0; i < 8; i++) {
		printk("%x ", r_vendor_id[i]);
    	}
   	printk("\n");
	return 0;
}

int doAdslFarItuId(int argc, char *argv[], void *p)
{
	int i;
	uint8 *c_vendor_id = NULL;
	adsldev_ops *ops = get_adsl_dev_ops(p);

	if (ops == NULL)
		return 0;

    	ops->query(ADSL_QUERY_FAR_ITUID, &c_vendor_id, NULL);

	if(c_vendor_id == NULL)
		return 0;

    	printk("*far end itu identification: ");
    	for (i = 0; i < 8; i++) {
		printk("%x ", c_vendor_id[i]);
    	}
    	printk("\n");
	return 0;
}
#ifdef CMD_API
int doAdslCmd(int argc, char *argv[], void *p)
{
	return subcmd(AdslCmds, argc, argv, p);
}

int doAdslBegin(int argc, char *argv[], void *p)
{
	adsl_up = 0;
	begin_modulation = 0;
	if(!cosubcmd(AdslBeginCmds, argc, argv, p)){
		if(begin_modulation & ADSLCMD_BEGIN_MODULATION_A){
			printk("Multimode\r\n");
			doAdslMultimode(argc, argv, p);
		}
		if(begin_modulation & ADSLCMD_BEGIN_MODULATION_D){
			printk("Gdmat\r\n");
			doAdslGdmt(argc, argv, p);
		}
		if(begin_modulation & ADSLCMD_BEGIN_MODULATION_L){
			printk("Glite\r\n");
			doAdslGlite(argc, argv, p);
		}
		if(begin_modulation & ADSLCMD_BEGIN_MODULATION_2){
			printk("ADSL2\r\n");
			doAdslAdsl2(argc, argv, p);
		}
		if(begin_modulation & ADSLCMD_BEGIN_MODULATION_P){
			printk("ADSL2Plus\r\n");
			doAdslAdsl2plus(argc, argv, p);
		}
		if(begin_modulation & ADSLCMD_BEGIN_MODULATION_E){
			char *annex[4] = {"ghs", "set", "annex", "al"};
			printk("annex al\r\n");
			wanCmds[6].func(4, annex, p);
		}
		if(begin_modulation & ADSLCMD_BEGIN_MODULATION_M){
			char *annex[4] = {"ghs", "set", "annex", "m"};
			printk("annex m\r\n");
			wanCmds[6].func(4, annex, p);
		}
		if(begin_modulation & ADSLCMD_BEGIN_MODULATION_T){
			printk("T1.413\r\n");
			doAdslT1413(argc, argv, p);
		}
		if(adsl_up){
			printk("up right away\r\n");
			doAdslOpen(argc, argv, p);
		}
		else{
			printk("wait other command to let ADSL up\r\n");
			doAdslClose(argc, argv, p);
		}

	}
	return 0;
}

int doAdslEnd(int argc, char *argv[], void *p)
{
	doAdslClose(argc, argv, p);
	return 0;
}

int doAdslConnect(int argc, char *argv[], void *p)
{
	return subcmd(AdslConnectCmds, argc, argv, p);;
}

int doAdslInfo(int argc, char *argv[], void *p)
{
	return subcmd(AdslInfoCmds, argc, argv, p);;
}

int doAdslDelt(int argc, char *argv[], void *p)
{
	return subcmd(AdslDeltCmds, argc, argv, p);;
}

int doAdslFwversion(int argc, char *argv[], void *p)
{
	char fwver[80];
	adsldev_ops *ops = get_adsl_dev_ops(p);

	getADSLFwVer(fwver, ops);
	#if 1
	memset(adslinfobuf,0,sizeof(adslinfobuf));
	sprintf(adslinfobuf,"adslcmd fwversion\r\n%s\r\n",fwver);
	#else
	printk(fwver);

	#endif
	
	return 0;
}

int doAdslDriverversion(int argc, char *argv[], void *p)
{
	char fwver[80];
	char *start, *end;
	uint8 index;
	adsldev_ops *ops = get_adsl_dev_ops(p);

	start = fwver;
	getADSLFwVer(start, ops);
	end = strchr(start, ':');
	index = end - start + 1;
	start += index;
	end = strchr(start, ' ');
	index = end - start;
	start[index] = '\0';
	#if 1
	memset(adslinfobuf,0,sizeof(adslinfobuf));
	sprintf(adslinfobuf,"adsldriver version %s\r\n",fwver);
	#else
	printk("adsldriver version %s\r\n", start);

	#endif
	return 0;
}

int doAdslVersion(int argc, char *argv[], void *p)
{	
	#if 1
	char fwver[80];
	int index = 0;
	char *ptr = adslinfobuf;
	adsldev_ops *ops = get_adsl_dev_ops(p);
	getADSLFwVer(fwver, ops);
	memset(adslinfobuf,0,sizeof(adslinfobuf));
	index += sprintf( ptr+index, "adslphxcmd version: %s\r\n",MODULE_VERSION_ADSLCMD);
	index += sprintf( ptr+index, "ADSL PHY:AnnexA version - %s\r\n",fwver);
	#else
	printk("adslphxcmd version 1.0.0\r\n");
	#endif
	return 0;
}

int doAdslHelp(int argc, char *argv[], void *p)
{
	#if 1
	char *ptr = NULL;
	int i = 0;
	char helpstr[][150] =
	{
		"Usage: adslphxcmd begin [--up] [--modulation {a|d|l|t|2|p|e|m}] [--bitswap {on|off}] [--sra {on|off}]\n",
		"       adslphxcmd end\n",
		"       adslphxcmd connect [--up] [--down]\n",
		"       adslphxcmd info [--state] [--show] [--stats]\n",
		"       adslphxcmd delt [--start] [--status] [--show {snr|qln|hlin|bits|actatp}]\n",
		"       adslphxcmd fwversion\n",
		"       adslphxcmd driverversion\n",
		"       adslphxcmd version\n",
		"       adslphxcmd help\n",
		""//must at the end
	};
	memset(adslinfobuf,0,sizeof(adslinfobuf));
	ptr = helpstr[0];
	while(strcmp(ptr,""))
	{
		strcat(adslinfobuf,helpstr[i]);
		ptr = helpstr[++i];
	}
	#else
	printk(  "Usage: adslphxcmd begin [--up] [--modulation {a|d|l|t|2|p|e|m}]* [--bitswap {on|off}]] [--sra {on|off}]\n"
			 "       adslphxcmd end\n"
			 "       adslphxcmd connect [--up] [--down]\n"
			 "       adslphxcmd info [--state] [--show] [--stats]\n"
			 "       adslphxcmd delt [--start] [--status] [--show {snr|qln|hlin|bits|actatp}]\n"
			 "       adslphxcmd fwversion\n"
			 "       adslphxcmd driverversion\n"
			 "       adslphxcmd version\n"
			 "       adslphxcmd help\n");
	#endif

	return 0;
}

int doAdslBeginUp(int argc, char *argv[], void *p)
{
	adsl_up = 1;
	return 0;
}

int doAdslBeginMod(int argc, char *argv[], void *p)
{
	return subcmd(AdslBeginModCmds, argc, argv, p);
}

int doAdslBeginModA(int argc, char *argv[], void *p)
{
	begin_modulation |= ADSLCMD_BEGIN_MODULATION_A;
	return 0;
}

int doAdslBeginModD(int argc, char *argv[], void *p)
{
	begin_modulation |= ADSLCMD_BEGIN_MODULATION_D;
	return 0;
}

int doAdslBeginModL(int argc, char *argv[], void *p)
{
	begin_modulation |= ADSLCMD_BEGIN_MODULATION_L;
	return 0;
}

int doAdslBeginModT(int argc, char *argv[], void *p)
{
	begin_modulation |= ADSLCMD_BEGIN_MODULATION_T;
	return 0;
}

int doAdslBeginMod2(int argc, char *argv[], void *p)
{
	begin_modulation |= ADSLCMD_BEGIN_MODULATION_2;
	return 0;
}

int doAdslBeginModP(int argc, char *argv[], void *p)
{
	begin_modulation |= ADSLCMD_BEGIN_MODULATION_P;
	return 0;
}

int doAdslBeginModE(int argc, char *argv[], void *p)
{
 	begin_modulation |= ADSLCMD_BEGIN_MODULATION_E;
	return 0;
}

int doAdslBeginModM(int argc, char *argv[], void *p)
{
	begin_modulation |= ADSLCMD_BEGIN_MODULATION_M;
	return 0;
}

int doAdslBeginBswap(int argc, char *argv[], void *p)
{
    	return subcmd(AdslBeginBswapOnOffCmds, argc, argv, p);
}

int doAdslBeginSra(int argc, char *argv[], void *p)
{
    	return subcmd(AdslBeginSraOnOffCmds, argc, argv, p);
}

int doAdslBeginBswapOn(int argc, char *argv[], void *p)
{
	char *bswap[4] = {"dmt2", "set", "olr", "1"};
	wanCmds[3].func(4, bswap, p);
	return 0;
}

int doAdslBeginBswapOff(int argc, char *argv[], void *p)
{
	char *bswap[4] = {"dmt2", "set", "olr", "0"};
	wanCmds[3].func(4, bswap, p);
	return 0;
}

int doAdslBeginSraOn(int argc, char *argv[], void *p)
{
	char *sra[4] = {"dmt2", "set", "olr", "3"};
	wanCmds[3].func(4, sra, p);
	return 0;
}

int doAdslBeginSraOff(int argc, char *argv[], void *p)
{
	char *sra[4] = {"dmt2", "set", "olr", "2"};
	wanCmds[3].func(4, sra, p);
	return 0;
}

int doAdslConnectUp(int argc, char *argv[], void *p)
{
	doAdslOpen(argc, argv, p);
	return 0;
}

int doAdslConnectDown(int argc, char *argv[], void *p)
{
	doAdslClose(argc, argv, p);
	return 0;
}

int doAdslInfoState(int argc, char *argv[], void *p)
{
	uint8 modemst;
	T_adslChannelOperData adslChannelOperData;
	adsldev_ops *ops = get_adsl_dev_ops(p);

	#if 1
	char *ptr = adslinfobuf;
	int index = 0;
	#endif
	if(ops == NULL)
		return 0;

	ops->query(ADSL_QUERY_STATUS, &modemst, NULL );
    	if (modemst == ADSL_MODEM_STATE_UP)
	    	ops->query(ADSL_QUERY_CH_OP_DATA, &adslChannelOperData, NULL);
		
	#if 1
	memset(adslinfobuf,0,sizeof(adslinfobuf));
	index += sprintf( ptr+index, "adslphxcmd: ADSL driver and PHY status\r\nStatus ");
	if (modemst == ADSL_MODEM_STATE_DOWN){
		index += sprintf( ptr+index, "down\r\n");
		return 0;
	}
	else if (modemst == ADSL_MODEM_STATE_WAIT_INIT)
		index += sprintf( ptr+index, "wait for init\r\n");
	else if (modemst == ADSL_MODEM_STATE_INIT)
		index += sprintf( ptr+index, "initializing\r\n");
	else if (modemst == ADSL_MODEM_STATE_UP)
		index += sprintf( ptr+index, "Showtime\r\n");
	else
		index += sprintf( ptr+index, "Unknown\r\n");
	
	if(adslChannelOperData.actualBitrateNearInt){
		index += sprintf( ptr+index, "Channel: Interleave, UpInterleavestream rate = %d Kbps, DownInterleavestream rate = %d Kbps\r\n",
			adslChannelOperData.actualBitrateFarInt+adslChannelOperData.actualBitrateFarFast, adslChannelOperData.actualBitrateNearInt+adslChannelOperData.actualBitrateNearFast);
	}
	else{
		index += sprintf( ptr+index, "Channel: Fast, Upstream rate = %d Kbps, Downstream rate = %d Kbps\r\n",
			adslChannelOperData.actualBitrateFarInt+adslChannelOperData.actualBitrateFarFast, adslChannelOperData.actualBitrateNearInt+adslChannelOperData.actualBitrateNearFast);
	}	

	return index;
	#else
	printk("adslphxcmd: ADSL driver and PHY status\r\nStatus: ");
	if (modemst == ADSL_MODEM_STATE_DOWN){
		printk("down\r\n");
		return 0;
	}
	else if (modemst == ADSL_MODEM_STATE_WAIT_INIT)
		printk("wait for init");
	else if (modemst == ADSL_MODEM_STATE_INIT)
		printk("initializing");
	else if (modemst == ADSL_MODEM_STATE_UP)
		printk("Showtime");
	else
		printk("Unknown");
	
	if(adslChannelOperData.actualBitrateNearInt){
		printk(" Channel: Interleave, UpInterleavestream rate = %d Kbps, DownInterleavestream rate = %d Kbps\r\n",
			adslChannelOperData.actualBitrateFarInt, adslChannelOperData.actualBitrateNearInt);
	}
	else{
		printk(" Channel: Fast, Upstream rate = %d Kbps, Downstream rate = %d Kbps\r\n",
			adslChannelOperData.actualBitrateFarFast, adslChannelOperData.actualBitrateNearFast);
	}
	
	return 0;
	#endif
	
}

int doAdslInfoShow(int argc, char *argv[], void *p)
{
	uint8 modemst, mode, channel, line_state, pm_state;
	uint32 linerate_ds, linerate_us;
	T_adslChannelOperData adslChannelOperData;
	T_NTadslFarEndLineOperData adslFarEndLineOperData;
	T_NTadslNearEndLineOperData adslNearEndLineOperData;
	PMS_PARAMS pms;
	DEFECT_CNT_SET counter_set;
	//TR98_WAN_DSL_INTERFACE_CFG_STS_ST ESs;
	CELL_COUNT1 CellOperdata;
	SNMP_ADSL_NODE_2 snmpAdslNode_2;
	adsldev_ops *ops = get_adsl_dev_ops(p);

	#if 1
	char *ptr = adslinfobuf;
	int index = 0;

	index = doAdslInfoState(argc, argv, p);
	#else
	doAdslInfoState(argc, argv, p);

	#endif
	
	if(ops == NULL)
		return 0;

	ops->query(ADSL_QUERY_STATUS, &modemst, NULL );
//    	if (modemst == ADSL_MODEM_STATE_UP){
		ops->query(ADSL_QUERY_MODE, &mode, NULL);
		ops->query(ADSL_QUERY_CH_OP_DATA, &adslChannelOperData, NULL);
		ops->query(ADSL_QUERY_FAR_OP_DATA, &adslFarEndLineOperData, NULL);
		ops->query(ADSL_QUERY_NEAR_OP_DATA, &adslNearEndLineOperData, NULL);
		if (mode != ME_CMD_VDSL2){
			ops->query(ADSL_QUERY_LINERATE, &linerate_ds, &linerate_us);
		}	
		ops->query(ADSL_QUERY_PMS_PARAM, &pms, NULL);
		ops->query(ADSL_QUERY_DEFECT_CNT, &counter_set, NULL);
		ops->query(ADSL_QUERY_LAST_DROP_REASON, &line_state, NULL);
		if (mode != ME_CMD_VDSL2){
			ops->query(ADSL_QUERY_CELL_CNT1, &CellOperdata, NULL);
			ops->query(ADSL_QUERY_PM_STATE, &pm_state, NULL);
		}	
		ops->query(ADSL_QUERY_STATISTICS_2, &snmpAdslNode_2, NULL);
//	}
	#if 1
	index += sprintf( ptr+index, "Link Power State: \t");
	switch(pm_state){
		case L0:
			index += sprintf( ptr+index, "L0");
			break;
		case L2:
			index += sprintf( ptr+index, "L2");
			break;
		case L3:
			index += sprintf( ptr+index, "L3");
			break;
	}
	index += sprintf( ptr+index, "\r\nMode: \t\t\t");
	switch(mode){
		case ME_CMD_ADSL_ANSI:
			index += sprintf( ptr+index, "ANSI T1.413\r\n");
			break;
        	case ME_CMD_ADSL_OPEN_GDMT:
				index += sprintf( ptr+index, "ITU G.992.1(G.DMT)\r\n");
			break;
        	case ME_CMD_ADSL_OPEN_GLITE:
				index += sprintf( ptr+index, "ITU G.992.2(G.Lite)\r\n");
			break;
        	case ME_CMD_ADSL2:
				index += sprintf( ptr+index, "ITU G.992.3(ADSL2)\r\n");
			break;
        	case ME_CMD_ADSL2PLUS:
				index += sprintf( ptr+index, "ITU G.992.5(ADSL2PLUS)\r\n");
			break;
        	case ME_CMD_VDSL2:
				index += sprintf( ptr+index, "ITU G.993.2(VDSL2)\r\n");
			break;
    		case ME_CMD_GVECTOR:
           		index += sprintf( ptr+index, "ITU G.993.5(G.Vectoring)\r\n");
    			break;		
    		case ME_CMD_GVECTOR_GINP:
           		index += sprintf( ptr+index, "ITU G.993.5(G.Vectoring),G.998.4(G.INP)\r\n");
    			break;
    		case ME_CMD_VDSL2_GINP:
           		index += sprintf( ptr+index, "ITU G.993.2(VDSL2), G.998.4(G.INP)\r\n");
    			break;
    		case ME_CMD_ADSL2_GINP:
           		index += sprintf( ptr+index, "ITU G.992.3(ADSL2) ,G.998.4(G.INP)\r\n");
    			break;		
    		case ME_CMD_ADSL2PLUS_GINP:
           		index += sprintf( ptr+index, "ITU G.992.5(ADSL2PLUS) ,G.998.4(G.INP)\r\n");
    			break;				
		default:
			index += sprintf( ptr+index, "N/A\r\n");
	}
	index += sprintf( ptr+index, "Channel:\t\t");
	if(adslChannelOperData.actualBitrateNearInt){
		index += sprintf( ptr+index, "Interleave\r\n");
	   	channel = 0;
	}
	else if(adslChannelOperData.actualBitrateNearFast){
		index += sprintf( ptr+index, "Fast\r\n");
	   	channel = 1;
	}
	else{
		index += sprintf( ptr+index, "N/A\r\n");
		channel = 1;
	}
	index += sprintf( ptr+index, "Line Status:\t\t");
	switch(line_state){
		case NO_DEFECT:
			index += sprintf( ptr+index, "No Defect");
			break;
		case LOF(SEF):
			index += sprintf( ptr+index, "Loss of Frame");
			break;
		case LOSQ(LOM):
			index += sprintf( ptr+index, "Loss of Signal Quality");
			break;
		case LOS:
			index += sprintf( ptr+index, "Loss of Signal");
			break;
		case LPR(LOP):
			index += sprintf( ptr+index, "Loss of Power");
			break;
		case UNKNOWN:
			index += sprintf( ptr+index, "Unknown");
			break;
		case NOT_LINK_UP:
		default:
			index += sprintf( ptr+index, "Not Link Up");
			break;

	}
	index += sprintf( ptr+index, "\r\nTraining Status:\t");
	if(modemst == ADSL_MODEM_STATE_DOWN){
		index += sprintf( ptr+index, "down\r\n");
		return 0;
	}
	else if (modemst == ADSL_MODEM_STATE_WAIT_INIT)
		index += sprintf( ptr+index, "wait for init\r\n");
	else if (modemst == ADSL_MODEM_STATE_INIT)
		index += sprintf( ptr+index, "initializing\r\n");
	else if (modemst == ADSL_MODEM_STATE_UP)
		index += sprintf( ptr+index, "Showtime\r\n");
	else
		index += sprintf( ptr+index, "Unknown\r\n");

	index += sprintf( ptr+index, "\t\tDown\t\tUp\r\n");
	index += sprintf( ptr+index, "SNR (dB):\t%d.%d\t\t%d.%d\r\n",
		adslNearEndLineOperData.noiseMarginDnstr/10, 
		adslNearEndLineOperData.noiseMarginDnstr%10, 
		adslFarEndLineOperData.noiseMarginUpstr/10,
		adslFarEndLineOperData.noiseMarginUpstr%10);
	index += sprintf( ptr+index, "Attn (dB):\t%d.%d\t\t%d.%d\r\n",
		adslNearEndLineOperData.attenuationDnstr/10,
		adslNearEndLineOperData.attenuationDnstr%10,
		adslFarEndLineOperData.attenuationUpstr/10,
		adslFarEndLineOperData.attenuationUpstr%10);
	index += sprintf( ptr+index, "Pwr (dBm):\t%d.%d\t\t%d.%d\r\n",
		adslFarEndLineOperData.outputPowerDnstr/10,
                adslFarEndLineOperData.outputPowerDnstr%10,
                adslNearEndLineOperData.outputPowerUpstr/10,
                adslNearEndLineOperData.outputPowerUpstr%10);

	index += sprintf( ptr+index, "Max(Kbps):\t%d\t\t%d\r\n", linerate_ds, linerate_us);
	
	if(channel)
		index += sprintf( ptr+index, "Rate (Kbps):\t%d\t\t%d\r\n",
			adslChannelOperData.actualBitrateNearFast,adslChannelOperData.actualBitrateFarFast);
	else
		index += sprintf( ptr+index, "Rate (Kbps):\t%d\t\t%d\r\n",
		adslChannelOperData.actualBitrateNearInt,adslChannelOperData.actualBitrateFarInt);

	index += sprintf( ptr+index, "Intl Depth:\t%d\t\t%d\r\n", pms.D_ds, pms.D_us);
	
	index += sprintf( ptr+index, "\t\t\tG.dmt framing\r\r\n");
	if(mode == ME_CMD_ADSL2 || mode == ME_CMD_ADSL2PLUS){
		index += sprintf( ptr+index, "R:\t\t%d\t\t%d\r\n", 0, 0);
		index += sprintf( ptr+index, "S:\t\t%d\t\t%d\r\n", 0, 0);
		index += sprintf( ptr+index, "D:\t\t%d\t\t%d\r\n", 0, 0);
	}
	else{
		index += sprintf( ptr+index, "R:\t\t%d\t\t%d\r\n",pms.R_ds, pms.R_us);
		index += sprintf( ptr+index, "S:\t\t%d.%02d\t\t%d.%02d\r\n",(uint8)pms.S_ds, (uint16)(pms.S_ds*100)%100, 
			(uint8)pms.S_us, (uint16)(pms.S_us*100)%100);
		index += sprintf( ptr+index, "D:\t\t%d\t\t%d\r\n", pms.D_ds, pms.D_us);
	}
	index += sprintf( ptr+index, "\t\t\tADSL2 framing\r\r\n");
	if(mode == ME_CMD_ADSL2 || mode == ME_CMD_ADSL2PLUS){
		index += sprintf( ptr+index, "MSGc:\t\t%d\t\t%d\r\n",pms.msgc_ds, pms.msgc_us);
		index += sprintf( ptr+index, "R:\t\t%d\t\t%d\r\n", pms.R_ds, pms.R_us);
		index += sprintf( ptr+index, "S:\t\t%d.%02d\t\t%d.%02d\r\n", (uint8)pms.S_ds, (uint16)(pms.S_ds*100)%100, 
			(uint8)pms.S_us, (uint16)(pms.S_us*100)%100);
		index += sprintf( ptr+index, "D:\t\t%d\t\t%d\r\n", pms.D_ds, pms.D_us);
	}
	else{
		index += sprintf( ptr+index, "MSGc:\t\t%d\t\t%d\r\n",0, 0);
		index += sprintf( ptr+index, "R:\t\t%d\t\t%d\r\n", 0, 0);
		index += sprintf( ptr+index, "S:\t\t%d\t\t%d\r\n", 0, 0);
		index += sprintf( ptr+index, "D:\t\t%d\t\t%d\r\n", 0, 0);
	}
	
	index += sprintf( ptr+index, "\t\t\tCounters\r\n");
	index += sprintf( ptr+index, "RS:\t\t%u\t\t%u\r\n",pms.RSWords_ds, pms.RSWords_us);
	if(channel){
		index += sprintf( ptr+index, "RSCorr:\t\t%u\t\t%u\n",counter_set.near_end.FecNotInterleaved,
                        counter_set.far_end.FecNotInterleaved);
		index += sprintf( ptr+index, "RSUnCorr:\t%u\t\t%u\n",counter_set.near_end.CrcNotInterleaved,
		        counter_set.far_end.CrcNotInterleaved);
		index += sprintf( ptr+index, "HEC:\t\t%u\t\t%u\r\n", counter_set.near_end.HecNotInterleaved, 
			counter_set.far_end.HecNotInterleaved);
	}
	else{
		index += sprintf( ptr+index, "RSCorr:\t\t%u\t\t%u\n",counter_set.near_end.FecInterleaved,
                        counter_set.far_end.FecInterleaved);
		index += sprintf( ptr+index, "RSUnCorr:\t%u\t\t%u\n",counter_set.near_end.CrcInterleaved,
                        counter_set.far_end.CrcInterleaved);
		index += sprintf( ptr+index, "HEC:\t\t%u\t\t%u\r\n", counter_set.near_end.HecInterleaved,
                        counter_set.far_end.HecInterleaved);
	}

	index += sprintf( ptr+index, "OCD:\t\t%u\t\t%u\n", CellOperdata.DsOcd, CellOperdata.UsOcd);
	index += sprintf( ptr+index, "LCD:\t\t%u\t\t%u\n", CellOperdata.DsLcd, CellOperdata.UsLcd);
	index += sprintf( ptr+index, "Total Cells:\t%u\t\t%u\n", CellOperdata.rxTotalCells, CellOperdata.txTotalCells);
	index += sprintf( ptr+index, "Drop Cells:\t%u\t\t%u\n", CellOperdata.rxDropCells, CellOperdata.txDropCells);
	index += sprintf( ptr+index, "Bit Errors:\t0\t\t0\r\n\r\n");
	index += sprintf( ptr+index, "ES:\t\t%u\t\t%u\r\n", CellOperdata.DsEs, CellOperdata.UsEs);
	index += sprintf( ptr+index, "SES:\t\t%u\t\t%u\r\n", CellOperdata.DsSes, CellOperdata.UsSes);
	index += sprintf( ptr+index, "UAS:\t\t%lu\t\t%lu\n", snmpAdslNode_2.RxUAS, snmpAdslNode_2.TxUAS);
	if(channel){
		index += sprintf( ptr+index, "CRC:\t\t%u\t\t%u\r\n", counter_set.near_end.CrcNotInterleaved,
                        counter_set.far_end.CrcNotInterleaved);
		index += sprintf( ptr+index, "FEC:\t\t%u\t\t%u\r\n", counter_set.near_end.FecNotInterleaved,
                        counter_set.far_end.FecNotInterleaved);
	}
	else{
		index += sprintf( ptr+index, "CRC:\t\t%u\t\t%u\r\n", counter_set.near_end.CrcInterleaved,
                        counter_set.far_end.CrcInterleaved);
		index += sprintf( ptr+index, "FEC:\t\t%u\t\t%u\r\n", counter_set.near_end.FecInterleaved,
                        counter_set.far_end.FecInterleaved);
	}
	return index;
	
	#else
	printk("Link Power State: \t");
	switch(pm_state){
		case L0:
			printk("L0");
			break;
		case L2:
			printk("L2");
			break;
		case L3:
			printk("L3");
			break;
	}
	printk("\r\n");
	printk("Mode: \t\t\t");
	switch(mode){
		case ME_CMD_ADSL_ANSI:
			printk("ANSI T1.413\r\n");
			break;
        	case ME_CMD_ADSL_OPEN_GDMT:
			printk("ITU G.992.1(G.DMT)\r\n");
			break;
        	case ME_CMD_ADSL_OPEN_GLITE:
			printk("ITU G.992.2(G.Lite)\r\n");
			break;
        	case ME_CMD_ADSL2:
			printk("ITU G.992.3(ADSL2)\r\n");
			break;
        	case ME_CMD_ADSL2PLUS:
			printk("ITU G.992.5(ADSL2PLUS)\r\n");
			break;
        	case ME_CMD_VDSL2:
			printk("ITU G.993.2(VDSL2)\r\n");
			break;
		default:
			printk("N/A\r\n");
	}
	printk("Channel:\t\t");
	if(adslChannelOperData.actualBitrateNearInt){
	   	printk("Interleave\r\n");
	   	channel = 0;
	}
	else if(adslChannelOperData.actualBitrateNearFast){
	   	printk("Fast\r\n");
	   	channel = 1;
	}
	else{
		printk("N/A\r\n");
		channel = 1;
	}
	printk("Line Status:\t\t");
	switch(line_state){
		case NO_DEFECT:
			printk("No Defect");
			break;
		case LOF(SEF):
			printk("Loss of Frame");
			break;
		case LOSQ(LOM):
			printk("Loss of Signal Quality");
			break;
		case LOS:
			printk("Loss of Signal");
			break;
		case LPR(LOP):
			printk("Loss of Power");
			break;
		case UNKNOWN:
			printk("Unknown");
			break;
		case NOT_LINK_UP:
		default:
			printk("Not Link Up");
			break;

	}
	printk("\r\n");
	printk("Training Status:\t");
	if(modemst == ADSL_MODEM_STATE_DOWN){
		printk("down\r\n");
		return 0;
	}
	else if (modemst == ADSL_MODEM_STATE_WAIT_INIT)
		printk("wait for init\r\n");
	else if (modemst == ADSL_MODEM_STATE_INIT)
		printk("initializing\r\n");
	else if (modemst == ADSL_MODEM_STATE_UP)
		printk("Showtime\r\n");
	else
		printk("Unknown\r\n");

	printk("\t\tDown\t\tUp\r\n");
	printk("SNR (dB):\t%d.%d\t\t%d.%d\r\n", adslNearEndLineOperData.noiseMarginDnstr/10, 
		adslNearEndLineOperData.noiseMarginDnstr%10, 
		adslFarEndLineOperData.noiseMarginUpstr/10,
		adslFarEndLineOperData.noiseMarginUpstr%10);
	printk("Attn (dB):\t%d.%d\t\t%d.%d\r\n", adslNearEndLineOperData.attenuationDnstr/10,
		adslNearEndLineOperData.attenuationDnstr%10,
		adslFarEndLineOperData.attenuationUpstr/10,
		adslFarEndLineOperData.attenuationUpstr%10);
	printk("Pwr (dBm):\t%d.%d\t\t%d.%d\r\n", adslFarEndLineOperData.outputPowerDnstr/10,
                adslFarEndLineOperData.outputPowerDnstr%10,
                adslNearEndLineOperData.outputPowerUpstr/10,
                adslNearEndLineOperData.outputPowerUpstr%10);
	
	printk("Max(Kbps):\t%d\t\t%d\r\n", linerate_ds, linerate_us);
	
	if(channel)
		printk("Rate (Kbps):\t%d\t\t%d\r\n", adslChannelOperData.actualBitrateNearFast,
  										   adslChannelOperData.actualBitrateFarFast);
	else
		printk("Rate (Kbps):\t%d\t\t%d\r\n", adslChannelOperData.actualBitrateNearInt,
                        adslChannelOperData.actualBitrateFarInt);
	printk("Intl Depth:\t%d\t\t%d\r\n", pms.D_ds, pms.D_us);
	
	printk("\t\t\tG.dmt framing\r\r\n");
	if(mode == ME_CMD_ADSL2 || mode == ME_CMD_ADSL2PLUS){
		printk("R:\t\t%d\t\t%d\r\n", 0, 0);
        	printk("S:\t\t%d\t\t%d\r\n", 0, 0);
        	printk("D:\t\t%d\t\t%d\r\n", 0, 0);
	}
	else{
		printk("R:\t\t%d\t\t%d\r\n", pms.R_ds, pms.R_us);
		printk("S:\t\t%d.%02d\t\t%d.%02d\r\n", 
			(uint8)pms.S_ds, (uint16)(pms.S_ds*100)%100, 
			(uint8)pms.S_us, (uint16)(pms.S_us*100)%100);
		printk("D:\t\t%d\t\t%d\r\n", pms.D_ds, pms.D_us);
	}
	printk("\t\t\tADSL2 framing\r\r\n");
	if(mode == ME_CMD_ADSL2 || mode == ME_CMD_ADSL2PLUS){
		printk("MSGc:\t\t%d\t\t%d\r\n", pms.msgc_ds, pms.msgc_us);
		printk("R:\t\t%d\t\t%d\r\n", pms.R_ds, pms.R_us);
		printk("S:\t\t%d.%02d\t\t%d.%02d\r\n", 
			(uint8)pms.S_ds, (uint16)(pms.S_ds*100)%100, 
			(uint8)pms.S_us, (uint16)(pms.S_us*100)%100);
		printk("D:\t\t%d\t\t%d\r\n", pms.D_ds, pms.D_us);
	}
	else{
		printk("MSGc:\t\t%d\t\t%d\r\n", 0, 0);
		printk("R:\t\t%d\t\t%d\r\n", 0, 0);
		printk("S:\t\t%d\t\t%d\r\n", 0, 0);
		printk("D:\t\t%d\t\t%d\r\n", 0, 0);
	}

	printk("\t\t\tCounters\r\n");
	printk("RS:\t\t%u\t\t%u\r\n", pms.RSWords_ds, pms.RSWords_us);
	if(channel){
		printk("RSCorr:\t\t%u\t\t%u\n", counter_set.near_end.FecNotInterleaved,
                        counter_set.far_end.FecNotInterleaved);
		printk("RSUnCorr:\t%u\t\t%u\n", counter_set.near_end.CrcNotInterleaved,
		        counter_set.far_end.CrcNotInterleaved);
		printk("HEC:\t\t%u\t\t%u\r\n", counter_set.near_end.HecNotInterleaved, 
			counter_set.far_end.HecNotInterleaved);
	}
	else{
		printk("RSCorr:\t\t%u\t\t%u\n", counter_set.near_end.FecInterleaved,
                        counter_set.far_end.FecInterleaved);
		printk("RSUnCorr:\t%u\t\t%u\n", counter_set.near_end.CrcInterleaved,
                        counter_set.far_end.CrcInterleaved);
		printk("HEC:\t\t%u\t\t%u\r\n", counter_set.near_end.HecInterleaved,
                        counter_set.far_end.HecInterleaved);
	}
	printk("OCD:\t\t%u\t\t%u\n", CellOperdata.DsOcd, CellOperdata.UsOcd);
	printk("LCD:\t\t%u\t\t%u\n", CellOperdata.DsLcd, CellOperdata.UsLcd);
	printk("Total Cells:\t%u\t\t%u\n", CellOperdata.rxTotalCells, CellOperdata.txTotalCells);
	printk("Drop Cells:\t%u\t\t%u\n", CellOperdata.rxDropCells, CellOperdata.txDropCells);
	printk("Bit Errors:\t0\t\t0\r\n\r\n");
	printk("ES:\t\t%u\t\t%u\r\n", CellOperdata.DsEs, CellOperdata.UsEs);
	printk("SES:\t\t%u\t\t%u\r\n", CellOperdata.DsSes, CellOperdata.UsSes);
	printk("UAS:\t\t0\t\t0\n");
	if(channel){
		printk("CRC:\t\t%u\t\t%u\r\n", counter_set.near_end.CrcNotInterleaved,
                        counter_set.far_end.CrcNotInterleaved);
		printk("FEC:\t\t%u\t\t%u\r\n", counter_set.near_end.FecNotInterleaved,
                        counter_set.far_end.FecNotInterleaved);
	}
	else{
		printk("CRC:\t\t%u\t\t%u\r\n", counter_set.near_end.CrcInterleaved,
                        counter_set.far_end.CrcInterleaved);
		printk("FEC:\t\t%u\t\t%u\r\n", counter_set.near_end.FecInterleaved,
                        counter_set.far_end.FecInterleaved);
	}
	return 0;
	#endif	
}

int doAdslInfoStats(int argc, char *argv[], void *p)
{
	doAdslInfoShow(argc, argv, p);
	return 0;
}

int doAdslDeltStart(int argc, char *argv[], void *p)
{
	char *delt_start[5] = {"ghs", "set", "ldm", "on", "nodisp"};

	wanCmds[6].func(5, delt_start, p);
	delt_start_flag = 1;
	printk("Starting DELT test. [use adslphxcmd delt --status, to check the status]\n");
	return 0;
}

int doAdslDeltStatus(int argc, char *argv[], void *p)
{
	#if 1
	int ret = 0;
	#endif
	adsldev_ops *ops = get_adsl_dev_ops(p);
	
	if(!delt_start_flag){
		printk("ERROR!You must starting DELT test first.\r\n");
		return 0;
	}

	if (ops == NULL)
		return 0;

	#if 1
	adslDeltShowInfo(ADSL_DIAGNOSTIC_RESULT);
	if(ret < 0)
		printk("\r\nnot get diagnostic result!");
	#else
	ops->query(ADSL_QUERY_TR69_WLAN_DSL_DIAGNOSTIC, &dsl_diag, NULL);
	printk("DELT: %s.\r\n", strcmp(dsl_diag.DiagnosticState, "Complete")? "Running":"Completed");
	#endif
	return 0;
}

int adslFileOperate(char*paravalue)
{
	struct file *file = NULL;
	mm_segment_t old_fs;
	int writelen = 0;

	if(paravalue == NULL)
			return 0;
	
	if(file == NULL)
		file = filp_open(ADSLCMD_DIAGNOSTIC_FILE,O_WRONLY|O_APPEND|O_CREAT,0644);
	if(IS_ERR(file))
	{
		printk("\r\nwhen open file %s error!",ADSLCMD_DIAGNOSTIC_FILE);
		return 0;
	}

	old_fs = get_fs();
	set_fs(KERNEL_DS);
	writelen = file->f_op->write(file,(char*)paravalue,strlen(paravalue),&file->f_pos);
	set_fs(old_fs);

	filp_close(file,NULL);
	return writelen;
}

static int adslDeltShowInfo(int flag)
{
	TR98_WAN_DSL_DIAGNOSTICS    dsl_diag;
	int i;
	int retlen;
	char *paravalue = NULL;
	char temp[30] = {0};
	
	if (adsl_dev_ops == NULL)
		return 0;

	memset(&dsl_diag,0,sizeof(dsl_diag));
	
	adsl_dev_ops->query(ADSL_QUERY_TR69_WLAN_DSL_DIAGNOSTIC, &dsl_diag, NULL);

	if(flag & ADSL_DIAGNOSTIC_RESULT)
	{
		paravalue = kmalloc(ADSLBUFSIZEMIN,GFP_KERNEL);
		if(paravalue == NULL)
		{
			printk("\r\nkmalloc error!!!");
			return -1;
		}
		memset(paravalue,0,ADSLBUFSIZEMIN);
		sprintf(paravalue,"DiagnosticState=%s\r\n",strcmp(dsl_diag.DiagnosticState, "Complete")? "Running":"Completed");
	}
	else if(flag & ADSL_DIAGNOSTIC_SNR)
	{
		paravalue = kmalloc(ADSLBUFSIZEMAX,GFP_KERNEL);
		if(paravalue == NULL)
		{
			printk("\r\nkmalloc error!!!");
			return -1;
		}
		memset(paravalue,0,ADSLBUFSIZEMAX);
		
		strcpy(paravalue,"SNRpsds=");
		for(i = 0; i < 512; i++)
		{	
			sprintf(temp,"%d", dsl_diag.SNRpsds[i]);
			strcat(paravalue,temp);
			if(i != 511)
				strcat(paravalue,",");
		}
		strcat(paravalue,"\r\n");
	}
	else if(flag & ADSL_DIAGNOSTIC_QLN)
	{
		paravalue = kmalloc(ADSLBUFSIZEMAX,GFP_KERNEL);
		if(paravalue == NULL)
		{
			printk("\r\nkmalloc error!!!");
			return -1;
		}
		memset(paravalue,0,ADSLBUFSIZEMAX);
		strcpy(paravalue,"QLNpsds=");
		for(i = 0; i < 512; i++)
		{	
			sprintf(temp,"%d", dsl_diag.QLINpsds[i]);
			strcat(paravalue,temp);
			if(i != 511)
				strcat(paravalue,",");
		}
		strcat(paravalue,"\r\n");
	}
	else if(flag & ADSL_DIAGNOSTIC_HLIN)
	{
		paravalue = kmalloc(ADSLBUFSIZEMAX*2,GFP_KERNEL);
		if(paravalue == NULL)
		{
			printk("\r\nkmalloc error!!!");
			return -1;
		}

		memset(paravalue,0,ADSLBUFSIZEMAX*2);
		strcpy(paravalue,"HLINpsds=");
		for(i = 0; i < 512; i++)
		{	
			sprintf(temp,"%d,%d", dsl_diag.HLINpsds[i][0], dsl_diag.HLINpsds[i][1]);
			strcat(paravalue,temp);
			if(i != 511)
				strcat(paravalue,",");
		}
		strcat(paravalue,"\r\n");
	}
	else if(flag & ADSL_DIAGNOSTIC_BITS)
	{
		paravalue = kmalloc(ADSLBUFSIZEMAX,GFP_KERNEL);
		if(paravalue == NULL)
		{
			printk("\r\nkmalloc error!!!");
			return -1;
		}

		
		memset(paravalue,0,ADSLBUFSIZEMAX);
		strcpy(paravalue,"BITSpsds=");
		for(i = 0; i < 512; i++)
		{	
			sprintf(temp,"%d", dsl_diag.BITSpsds[i]);
			strcat(paravalue,temp);
			if(i != 511)
				strcat(paravalue,",");
		}
		strcat(paravalue,"\r\n");
	}
	else if(flag & ADSL_DIAGNOSTIC_GAINS)
	{
		paravalue = kmalloc(ADSLBUFSIZEMAX,GFP_KERNEL);
		if(paravalue == NULL)
		{
			printk("\r\nkmalloc error!!!");
			return -1;
		}
		
		memset(paravalue,0,ADSLBUFSIZEMAX);
		strcpy(paravalue,"GAINSpsds=");
		for(i = 0; i < 512; i++)
		{	
			sprintf(temp,"%d", dsl_diag.GAINSpsds[i]);
			strcat(paravalue,temp);
			if(i != 511)
				strcat(paravalue,",");
		}
		strcat(paravalue,"\r\n");
	}
	else if(flag & ADSL_DIAGNOSTIC_ACTATP)
	{
		paravalue = kmalloc(ADSLBUFSIZELITTLE,GFP_KERNEL);
		if(paravalue == NULL)
		{
			printk("\r\nkmalloc error!!!");
			return -1;
		}
		
		memset(paravalue,0,ADSLBUFSIZELITTLE);
		sprintf(paravalue,"ACTATPds=%d,ACTATPus=%d\r\n", dsl_diag.ACTATPds, dsl_diag.ACTATPus);
	}
	else if(flag & ADSL_DIAGNOSTIC_OTHER)
	{
		paravalue = kmalloc(ADSLBUFSIZELITTLE,GFP_KERNEL);
		if(paravalue == NULL)
		{
			printk("\r\nkmalloc error!!!");
			return -1;
		}
		memset(paravalue,0,ADSLBUFSIZELITTLE);
		sprintf(paravalue,"ACTPSDds=%d,ACTPSDus=%d,ACTATPds=%d,ACTATPus=%d,HLINSCds=%d\r\n",
		dsl_diag.ACTPSDds, dsl_diag.ACTPSDus, dsl_diag.ACTATPds, dsl_diag.ACTATPus, 
		dsl_diag.HLINSCds);
	}
	else
	{
		//do nothing
	}
	
	retlen = adslFileOperate(paravalue);
	
	if(paravalue != NULL)
	{
		kfree(paravalue);
		paravalue = NULL;
	}

	return 0;
}

int doAdslDeltShow(int argc, char *argv[], void *p)
{
	#if 1
	char paravalue[100] = {0};
	#endif
	
	if(!delt_start_flag){
	#if 1
	strcpy(paravalue,"ERROR!You must starting DELT test first.\r\n");
	adslFileOperate(paravalue);
	#else
	   printk("ERROR!You must starting DELT test first.\r\n");
	#endif
	   return 0;
	}

	if(argc == 1){
		doAdslDeltShowOthers(argc, argv, p);
		doAdslDeltShowHlin(argc, argv, p);
		doAdslDeltShowQln(argc, argv, p);
		doAdslDeltShowSnr(argc, argv, p);
		doAdslDeltShowBits(argc, argv, p);
		doAdslDeltShowGains(argc, argv, p);
		return 0;	
	}
	else
		return subcmd(AdslDeltShowCmds, argc, argv, p);
}

int doAdslDeltShowSnr(int argc, char *argv[], void *p)
{
	#if 1
	int ret;
	ret = adslDeltShowInfo(ADSL_DIAGNOSTIC_SNR);
	if(ret < 0)
		printk("\r\nnot get snr diagnostic info!");
	#else
	TR98_WAN_DSL_DIAGNOSTICS    dsl_diag;
	int i;
	adsldev_ops *ops = get_adsl_dev_ops(p);

	if (ops == NULL)
		return 0;

	ops->query(ADSL_QUERY_TR69_WLAN_DSL_DIAGNOSTIC, &dsl_diag, NULL);
		printk("SNRpsds=");
	for(i = 0; i < 512; i++){
		printk("%d", dsl_diag.SNRpsds[i]);
		if(i != 511)
			printk(",");
	}
	printk("\r\n");
	#endif

	return 0;
	
}

int doAdslDeltShowQln(int argc, char *argv[], void *p)
{
	#if 1
	int ret;
	ret = adslDeltShowInfo(ADSL_DIAGNOSTIC_QLN);
	if(ret < 0)
		printk("\r\nnot get qln diagnostic info!");
	#else
	TR98_WAN_DSL_DIAGNOSTICS    dsl_diag;
	int i;
	adsldev_ops *ops = get_adsl_dev_ops(p);

	
	if (ops == NULL)
		return 0;

	ops->query(ADSL_QUERY_TR69_WLAN_DSL_DIAGNOSTIC, &dsl_diag, NULL);
	
	printk("QLNpsds=");
	for(i = 0; i < 512; i++){
		printk("%d", dsl_diag.QLINpsds[i]);
		if(i != 511)
			printk(",");
	}
	printk("\r\n");

	#endif
	return 0;
}

int doAdslDeltShowHlin(int argc, char *argv[], void *p)
{
	#if 1
	int ret;
	ret = adslDeltShowInfo(ADSL_DIAGNOSTIC_HLIN);
	if(ret < 0)
		printk("\r\nnot get hlin diagnostic info!");
	#else
	TR98_WAN_DSL_DIAGNOSTICS    dsl_diag;
	int i;
	adsldev_ops *ops = get_adsl_dev_ops(p);

	if (ops == NULL)
		return 0;

	ops->query(ADSL_QUERY_TR69_WLAN_DSL_DIAGNOSTIC, &dsl_diag, NULL);
	printk("HLINpsds=");
	for(i = 0; i < 512; i++){
		printk("%d,%d", dsl_diag.HLINpsds[i][0], dsl_diag.HLINpsds[i][1]);
		if(i != 511)
			printk(",");
	}
	printk("\r\n");	
	#endif
	
	return 0;
}

int doAdslDeltShowBits(int argc, char *argv[], void *p)
{

	#if 1
	int ret;
	ret = adslDeltShowInfo(ADSL_DIAGNOSTIC_BITS);
	if(ret < 0)
		printk("\r\nnot get bits diagnostic info!");
	#else
	TR98_WAN_DSL_DIAGNOSTICS    dsl_diag;
	int i;
	adsldev_ops *ops = get_adsl_dev_ops(p);

	if (ops == NULL)
		return 0;

	ops->query(ADSL_QUERY_TR69_WLAN_DSL_DIAGNOSTIC, &dsl_diag, NULL);
	
	printk("BITSpsds=");
	for(i = 0; i < 512; i++){
		printk("%d", dsl_diag.BITSpsds[i]);
		if(i != 511)
			printk(",");
	}
	printk("\r\n");
	#endif
	return 0;
}

int doAdslDeltShowGains(int argc, char *argv[], void *p)
{
	#if 1
	int ret;
	ret = adslDeltShowInfo(ADSL_DIAGNOSTIC_GAINS);
	if(ret < 0)
		printk("\r\nnot get gains diagnostic info!");
	#else
	TR98_WAN_DSL_DIAGNOSTICS    dsl_diag;
	int i;
	adsldev_ops *ops = get_adsl_dev_ops(p);

	
	if (ops == NULL)
		return 0;

	ops->query(ADSL_QUERY_TR69_WLAN_DSL_DIAGNOSTIC, &dsl_diag, NULL);
	printk("GAINSpsds=");
	for(i = 0; i < 512; i++){
		printk("%d", dsl_diag.GAINSpsds[i]);
		if(i != 511)
			printk(",");
	}
	printk("\r\n");

	#endif
	return 0;
}

int doAdslDeltShowOthers(int argc, char *argv[], void *p)
{
	#if 1
	int ret;
	ret = adslDeltShowInfo(ADSL_DIAGNOSTIC_OTHER);
	if(ret < 0)
		printk("\r\nnot get other diagnostic info!");
	#else
	TR98_WAN_DSL_DIAGNOSTICS    dsl_diag;
	adsldev_ops *ops = get_adsl_dev_ops(p);

	if (ops == NULL)
		return 0;

	ops->query(ADSL_QUERY_TR69_WLAN_DSL_DIAGNOSTIC, &dsl_diag, NULL);
	printk("ACTPSDds=%d,ACTPSDus=%d,ACTATPds=%d,ACTATPus=%d,HLINSCds=%d\r\n",
		dsl_diag.ACTPSDds, dsl_diag.ACTPSDus, dsl_diag.ACTATPds, dsl_diag.ACTATPus, 
		dsl_diag.HLINSCds);

	
	#endif
	return 0;
}


int doAdslDeltShowActatp(int argc, char *argv[], void *p)
{	
	#if 1
	int ret;
	ret = adslDeltShowInfo(ADSL_DIAGNOSTIC_ACTATP);
	if(ret < 0)
		printk("\r\nnot get actatp diagnostic info!");
	#else
	TR98_WAN_DSL_DIAGNOSTICS    dsl_diag;
	adsldev_ops *ops = get_adsl_dev_ops(p);

	if (ops == NULL)
		return 0;
	ops->query(ADSL_QUERY_TR69_WLAN_DSL_DIAGNOSTIC, &dsl_diag, NULL);
	printk("ACTATPds=%d,ACTATPus=%d\r\n", dsl_diag.ACTATPds, dsl_diag.ACTATPus);

	#endif
	
	return 0;
}

#endif

/*int doAdslPktsClear(int argc, char *argv[], void *p){
	atmPktsClear();
	ethPktsClear();
	return 0;
}*/

/************************************************************************
*		     A D S L   E V E N T   C A L L   F U N C T I O N
*************************************************************************
*/

#define DSL_EVENT_UP			0x1
#define DSL_EVENT_DOWN			0x2
#define DSL_EVENT_TRAIN			0x3

/*
 * Fixed by frankliao 20100707
 *
 * Chip: TC3182
 *
 * Question: While doing F4/F5 loopback test, sar interrupt is disabled by dmt and
 *           this will cause oam/cc cell drop.
 *
 * Root cause: In tc3162, MAC_INT is defined as 21; in tc3262, MAC_INT is defined as 22.
 *             But in tc3162 and tc3262, for mac interrupt, the interrupt mask bit is the 21st bit;
 *             for sar interrupt, the interrupt mask bit is the 22st bit.
 *
 *             So if defined CONFIG_MIPS_TC3262 and the function is called by dmt, dmt will not
 *             enable/disable mac interrupt, otherwise, sar interrupt will be therefore enabled/disabled.
 */
/*
 * Fixed by frankliao 20100728
 *
 * Chip: TC3182 TC3262
 *
 * Root cause: DMT call this function to enable/disable MAC interrupt while doing adsl link.
 *			   But this action is unnessary in TC3182 and TC3262.
 *
 */
uint32 dslEventNotify ( uint32 event, uint32 param1, uint32 param2)
{
#ifndef CONFIG_MIPS_TC3262
	switch (event) {
		case DSL_EVENT_DOWN:
            tc3162_enable_irq(MAC_INT);
            //VPint(CR_INTC_IMR) |= (1<<MAC_INT);
			break;
		case DSL_EVENT_UP:
            tc3162_enable_irq(MAC_INT);
            //VPint(CR_INTC_IMR) |= (1<<MAC_INT);
			break;
		case DSL_EVENT_TRAIN:
            tc3162_disable_irq(MAC_INT);
            //VPint(CR_INTC_IMR) &= ~(1<<MAC_INT);
			break;
	}
#endif
	return 0;
}
EXPORT_SYMBOL(dslEventNotify);
unsigned int getDslUpstreamRate(void){
	T_adslAttainRate p_AttainRate;

	adsl_dev_ops->query(ADSL_QUERY_ATTAIN_RATE, &p_AttainRate, NULL);
	//printk("\nup = %lu", p_AttainRate.txAttainRate);
	return p_AttainRate.txAttainRate;
}
EXPORT_SYMBOL(getDslUpstreamRate);

/************************************************************************
*		     A D S L   P R O C  D E F I N I T I O N S
*************************************************************************
*/

extern struct proc_dir_entry *proc_tc3162;

#ifdef TCSUPPORT_CPU_EN7512
int
getXDSLState(char *stateADSL, uint8 long_fmt, uint8 lineId)
{
        adsldev_ops* ops;
        uint16  index = 0;
        uint8 modemst;
        uint8 mode;

        if (isFPGA)
        {
                if (isFpgaAdslVdslUp == 1) /*(1:1)1=UP , 1=ATM*/
                    index += sprintf( stateADSL + index, "1:1\n");
                else if (isFpgaAdslVdslUp == 2)/*(1:2)1=UP , 2=PTM*/
                    index += sprintf( stateADSL + index, "1:2\n");
                else /*(0:0)0=DOWN , 3=null*/
                    index += sprintf( stateADSL+index, "0:3\n");
                
                return index;
         }
        
         ops = adsl_dev_ops_get(lineId);
         /*(0:0)0 = DOWN , 0 = null*/
	 if (ops == NULL) {
                index += sprintf( stateADSL+index, "0:3\n");
                return index;
	  }

	   ops->query(ADSL_QUERY_STATUS, &modemst, NULL );
	   
            if (modemst == ADSL_MODEM_STATE_UP) 
            {
#ifdef CONFIG_RALINK_VDSL
	       ops->query(ADSL_QUERY_RX_BEAR_TPSTC_TYPE, &mode, NULL);
	       switch(mode) 
                {
		        case TPSTC_ATM_TC:
	    	                index += sprintf(stateADSL+index, "1:1\n");
			       break;
		        case TPSTC_PTM_TC_64_65B:
	    	                index += sprintf(stateADSL+index, "1:2\n");
			       break;
                          default:
                                  index += sprintf( stateADSL + index, "1:3\n");
                                  break;
                }
#else
                index += sprintf( stateADSL + index, "1:3\n");  
#endif 
	   }
            else
                 index += sprintf( stateADSL + index, "0:3\n");

        return index;	

}
#endif /*TCSUPPORT_CPU_EN7512*/

int
getADSLState(char *stateADSL, uint8 long_fmt, uint8 lineId)
{
	uint16	index=0;

	uint8 modemst;
	T_adslChannelOperData adslChannelOperData;
    T_adsl_def_counter_set adsl_def_counter_set;
	T_NTadslNearEndLineOperData adslNearEndLineOperData;
	T_NTadslFarEndLineOperData adslFarEndLineOperData;
	uint8 link_mode;
	uint8 mode;
	char timeStr[32];
	int i, j, line_num;

	
	uint8 *r_vendor_id = NULL;
	uint8 *c_vendor_id = NULL;//yzwang_20091215
	T_adslAttainRate p_AttainRate;
	//renyu_20100113
	#ifdef CWMP
	uint8  c_vendor_rev;
    	uint8  r_vendor_rev;
	//T_swVersion adslswVersion;
	TR98_WAN_DSL_INTERFACE_CFG swVersionNumber;
	PMS_PARAMS pms;
	char dslStandard[20] = {0};
	#endif
	uint8 standardused;
    	uint8 fw_rev, annex_afe, annex_set;	
		
	#if defined(TCSUPPORT_SNMP_ATMADSL)
	PMS_PARAMS pms_1;
	SNMP_ADSL_NODE snmpNode;
	#endif
    adsldev_ops* ops;
	
#if defined(TCSUPPORT_CPU_MT7510) || defined(TCSUPPORT_CPU_MT7505) || defined(TCSUPPORT_FTTDP_V2)
    if (isFPGA)
    {
        if (isFpgaAdslVdslUp == 1)
        {
            index += sprintf( stateADSL+index, "ADSL link status: up\n");
            index += sprintf(stateADSL+index, "TPSTC type: ");
            index += sprintf(stateADSL+index, "ATM TC\n");
        }
        else if (isFpgaAdslVdslUp == 2)
        {
            index += sprintf( stateADSL+index, "ADSL link status: up\n");
            index += sprintf(stateADSL+index, "TPSTC type: ");
            index += sprintf(stateADSL+index, "64/65B PTM TC\n");
			index += sprintf(stateADSL+index, "Opmode: ITU G.993.2(VDSL2)\n");
        }
        else
            index += sprintf( stateADSL+index, "ADSL link status: down\n");

        return index;
    }
#endif
    if (lineId == LINE1)
        ops = adsl_dev_ops_get(LINE1);
    else
        ops = adsl_dev_ops_get(LINE0);

	if (ops == NULL) {
		index += sprintf( stateADSL+index, "ADSL link status: down\n");
		return index;
	}

	ops->query(ADSL_QUERY_STATUS, &modemst, NULL );
	if (modemst == ADSL_MODEM_STATE_DOWN)
		index += sprintf( stateADSL+index, "ADSL link status: down\n");
	else if (modemst == ADSL_MODEM_STATE_WAIT_INIT)
		index += sprintf( stateADSL+index, "ADSL link status: wait for init\n");
	else if (modemst == ADSL_MODEM_STATE_INIT)
		index += sprintf( stateADSL+index, "ADSL link status: initializing\n");
	else if (modemst == ADSL_MODEM_STATE_UP)
		index += sprintf( stateADSL+index, "ADSL link status: up\n");
	else
		index += sprintf( stateADSL+index, "ADSL link status: unknown\n");

	if (modemst != ADSL_MODEM_STATE_UP) {
		return index;
	}

	ops->query(ADSL_QUERY_CH_OP_DATA, &adslChannelOperData, NULL);
	ops->query(ADSL_QUERY_MODE, &mode, NULL);
    ops->query(ADSL_QUERY_DEFECT_CNT, &adsl_def_counter_set, NULL);
	ops->query(ADSL_QUERY_NEAR_OP_DATA, &adslNearEndLineOperData, NULL);
	ops->query(ADSL_QUERY_FAR_OP_DATA, &adslFarEndLineOperData, NULL);

	index += sprintf(stateADSL+index, "\nOpmode: ");
	switch(mode) {
		case ME_CMD_ADSL_ANSI:
	    	index += sprintf(stateADSL+index, "ANSI T1.413\n");
			break;
		case ME_CMD_ADSL_OPEN_GDMT:
	   		index += sprintf(stateADSL+index, "ITU G.992.1(G.DMT)\n");
			break;
		case ME_CMD_ADSL_OPEN_GLITE:
       		index += sprintf(stateADSL+index, "ITU G.992.2(G.Lite)\n");
			break;
		case ME_CMD_ADSL2:
       		index += sprintf(stateADSL+index, "ITU G.992.3(ADSL2)\n");
			break;
		case ME_CMD_ADSL2PLUS:
       		index += sprintf(stateADSL+index, "ITU G.992.5(ADSL2PLUS)\n");
			break;
		case ME_CMD_VDSL2:
       		index += sprintf(stateADSL+index, "ITU G.993.2(VDSL2)\n");
			break;
		case ME_CMD_GVECTOR:
       		index += sprintf(stateADSL+index, "ITU G.993.5(G.Vectoring)\n");
			break;		
		case ME_CMD_GVECTOR_GINP:
       		index += sprintf(stateADSL+index, "ITU G.993.5(G.Vectoring),G.998.4(G.INP)\n");
			break;
		case ME_CMD_VDSL2_GINP:
       		index += sprintf(stateADSL+index, "ITU G.993.2(VDSL2), G.998.4(G.INP)\n");
			break;
		case ME_CMD_ADSL2_GINP:
       		index += sprintf(stateADSL+index, "ITU G.992.3(ADSL2) ,G.998.4(G.INP)\n");
			break;		
		case ME_CMD_ADSL2PLUS_GINP:
       		index += sprintf(stateADSL+index, "ITU G.992.5(ADSL2PLUS) ,G.998.4(G.INP)\n");
			break;
		default:
			index += sprintf(stateADSL+index, "Wrong opmode!\n");
	}
#ifdef CONFIG_RALINK_VDSL
	ops->query(ADSL_QUERY_RX_BEAR_TPSTC_TYPE, &mode, NULL);
	index += sprintf(stateADSL+index, "TPSTC type: ");
	switch(mode) {
		case TPSTC_DISABLE:
	    	index += sprintf(stateADSL+index, "disable\n");
			break;
		case TPSTC_ATM_TC:
	    	index += sprintf(stateADSL+index, "ATM TC\n");
			break;
		case TPSTC_PTM_TC_64_65B:
	    	index += sprintf(stateADSL+index, "64/65B PTM TC\n");
			break;
		case TPSTC_PTM_TC_HDLC:
	    	index += sprintf(stateADSL+index, "HDLC PTM TC\n");
			break;
	}
#endif

	index += sprintf( stateADSL+index, "\n");
	index += sprintf( stateADSL+index, "near-end interleaved channel bit rate: %u kbps\n",
									adslChannelOperData.actualBitrateNearInt);
	index += sprintf( stateADSL+index, "near-end fast channel bit rate: %u kbps\n",
									adslChannelOperData.actualBitrateNearFast);
	index += sprintf( stateADSL+index, "far-end interleaved channel bit rate: %u kbps\n",
									adslChannelOperData.actualBitrateFarInt);
	index += sprintf( stateADSL+index, "far-end fast channel bit rate: %u kbps\n",
									adslChannelOperData.actualBitrateFarFast);

	index += sprintf( stateADSL+index, "\n");
	index += sprintf( stateADSL+index, "near-end FEC error fast: %u\n",
						adsl_def_counter_set.near_end.FecNotInterleaved);
	index += sprintf( stateADSL+index, "near-end FEC error interleaved: %u\n",
						adsl_def_counter_set.near_end.FecInterleaved);
	index += sprintf( stateADSL+index, "near-end CRC error fast: %u\n",
						adsl_def_counter_set.near_end.CrcNotInterleaved);
	index += sprintf( stateADSL+index, "near-end CRC error interleaved: %u\n",
						adsl_def_counter_set.near_end.CrcInterleaved);
	index += sprintf( stateADSL+index, "near-end HEC error fast: %u\n",
						adsl_def_counter_set.near_end.HecNotInterleaved);
	index += sprintf( stateADSL+index, "near-end HEC error interleaved: %u\n",
						adsl_def_counter_set.near_end.HecInterleaved);
	index += sprintf( stateADSL+index, "far-end FEC error fast: %u\n",
						adsl_def_counter_set.far_end.FecNotInterleaved);
	index += sprintf( stateADSL+index, "far-end FEC error interleaved: %u\n",
						adsl_def_counter_set.far_end.FecInterleaved);
	index += sprintf( stateADSL+index, "far-end CRC error fast: %u\n",
						adsl_def_counter_set.far_end.CrcNotInterleaved);
	index += sprintf( stateADSL+index, "far-end CRC error interleaved: %u\n",
						adsl_def_counter_set.far_end.CrcInterleaved);
	index += sprintf( stateADSL+index, "far-end HEC error fast: %u\n",
						adsl_def_counter_set.far_end.HecNotInterleaved);
	index += sprintf( stateADSL+index, "far-end HEC error interleaved: %u\n",
						adsl_def_counter_set.far_end.HecInterleaved);
	/*pork 20090325 added*/
	doAdslUpTimeCal(timeStr);
	index += sprintf( stateADSL+index, "ADSL uptime :%s",timeStr);
	memset(timeStr,0,sizeof(timeStr));
	doAdslActiveTimeCal(timeStr);
	index += sprintf( stateADSL+index, "ADSL activetime :%s",timeStr);
	/*pork 20090325 added over*/
	if (!long_fmt)
		return index;

	index += sprintf( stateADSL+index, "\nDownstream:\n");
	index += sprintf( stateADSL+index, "relative capacity occupation: %u%% \n",
			adslNearEndLineOperData.relCapacityOccupationDnstr);
	index += sprintf( stateADSL+index, "noise margin downstream: %d.%d dB\n",
			adslNearEndLineOperData.noiseMarginDnstr/10, abs(adslNearEndLineOperData.noiseMarginDnstr%10));
	index += sprintf( stateADSL+index, "output power upstream: %d.%01d dbm\n",
			adslNearEndLineOperData.outputPowerUpstr/10, abs(adslNearEndLineOperData.outputPowerUpstr%10));
	index += sprintf( stateADSL+index, "attenuation downstream: %d.%01d dB\n",
			adslNearEndLineOperData.attenuationDnstr/10, adslNearEndLineOperData.attenuationDnstr%10);

	index += sprintf( stateADSL+index, "\nUpstream:\n");
	index += sprintf( stateADSL+index , "relative capacity occupation: %u%% \n",
			adslFarEndLineOperData.relCapacityOccupationUpstr);
	index += sprintf( stateADSL+index, "noise margin upstream: %d.%d dB\n",
			(adslFarEndLineOperData.noiseMarginUpstr/10),abs(adslFarEndLineOperData.noiseMarginUpstr%10)); //Jason_931028
	index += sprintf( stateADSL+index, "output power downstream: %d.%d dbm\n",
			(adslFarEndLineOperData.outputPowerDnstr/10),abs(adslFarEndLineOperData.outputPowerDnstr%10)); //Jason_931028
	index += sprintf( stateADSL+index, "attenuation upstream: %d.%d dB\n",
			(adslFarEndLineOperData.attenuationUpstr/10),(adslFarEndLineOperData.attenuationUpstr%10));//Jason_931028

	index += sprintf( stateADSL+index, "\nBit table:\n");
	ops->query(ADSL_QUERY_MODE, &link_mode, NULL);//Jason_931130
	if( link_mode == ME_CMD_ADSL2PLUS )    		 //Jason_931130
		line_num = 16;
	else
		line_num = 8;
	index += sprintf( stateADSL+index, "carrier load: number of bits per tone\n");
	for(i=0; i<line_num; i++) {	//Jason_931130
		index += sprintf( stateADSL+index, "tone %3d-%3d: ",i*32, i*32+31);
		for (j=0; j<16; j++) {
			index += sprintf( stateADSL+index, "%1x", adslFarEndLineOperData.carrierLoad[i*16+j] & 0xF);
			index += sprintf( stateADSL+index, "%1x ", (adslFarEndLineOperData.carrierLoad[i*16+j] & 0xF0) >> 4);
		}
		index += sprintf( stateADSL+index, "\n");
	}

	//yzwang_20091215
	ops->query(ADSL_QUERY_NEAR_ITUID, &r_vendor_id, NULL);
	index += sprintf( stateADSL+index, "\nnear end itu identification: ");
	if(r_vendor_id != NULL) {
		for (i = 0; i < 8; i++)
			index += sprintf( stateADSL+index, "%02x", r_vendor_id[i]);
	}
	
	ops->query(ADSL_QUERY_FAR_ITUID, &c_vendor_id, NULL);
	index += sprintf( stateADSL+index, "\nfar end itu identification: ");
	if(c_vendor_id != NULL){
		for (i = 0; i < 8; i++)
			index += sprintf( stateADSL+index, "%02x", c_vendor_id[i]);
	}
	
	index += sprintf( stateADSL+index, "\n");
	ops->query(ADSL_QUERY_ATTAIN_RATE, &p_AttainRate, NULL);
	index += sprintf( stateADSL+index, "attain upstream: %lu\n", p_AttainRate.txAttainRate);
	index += sprintf( stateADSL+index, "attain downstream: %lu\n", p_AttainRate.rxAttainRate);
#ifdef CWMP	
	//renyu_20100113
	index += sprintf( stateADSL+index, "\n");
	memset(timeStr,0,sizeof(timeStr));
	timeFormatSec(timeStr, adslShowTimeStart);
	index += sprintf( stateADSL+index, "ADSL activetime second: %s\n", timeStr);
	memset(timeStr,0,sizeof(timeStr));
	timeFormatSec(timeStr, adslTotalStart);
	index += sprintf( stateADSL+index, "ADSL total ativetime second: %s\n", timeStr);
		
	ops->query(ADSL_QUERY_SW_VER, &c_vendor_rev, &r_vendor_rev );
	index += sprintf( stateADSL+index, "ATURANSIRev: %1x\n", c_vendor_rev);
	index += sprintf( stateADSL+index, "ATUCANSIRev: %1x\n", r_vendor_rev);

	ops->query(ADSL_QUERY_TR098_WAN_DSL_INTERFACE_CONFIG, &swVersionNumber, NULL);
	index += sprintf( stateADSL+index, "ATURANSIStd: %lu\n", swVersionNumber.ATURANSIStd);
	index += sprintf( stateADSL+index, "ATUCANSIStd: %lu\n", swVersionNumber.ATUCANSIStd);

	ops->query(ADSL_QUERY_PMS_PARAM, &pms, NULL);
	index += sprintf( stateADSL+index, "Interleave Depth: %u\n", pms.D_ds);
#endif

#if defined(TCSUPPORT_CT)
	index += sprintf( stateADSL+index, "Interleave Depth Down: %u\n", pms.D_ds);
#endif
#if defined(TCSUPPORT_SNMP_ATMADSL)
	index += sprintf( stateADSL+index, "\n");

	ops->query(ADSL_QUERY_PMS_PARAM, &pms_1, NULL);
	index += sprintf( stateADSL+index, ATUR_CHAN_INTERL_DELAY, (uint8)pms_1.delay_ds);

	index += sprintf( stateADSL+index, ATUR_CHAN_PREV_RXRATE, adslAturChanPrevTxRate);	
	index += sprintf( stateADSL+index, ATUR_CHAN_CURR_RXRATE, adslChannelOperData.actualBitrateFarInt + adslChannelOperData.actualBitrateFarFast);
	
	adslAturChanPrevTxRate = adslChannelOperData.actualBitrateFarInt + adslChannelOperData.actualBitrateFarFast;

	ops->query(ADSL_QUERY_SNMP_ADSL_NODE, &snmpNode, NULL);
	index += sprintf( stateADSL+index, ATUR_CRC_BLK_LEN, snmpNode.CrcBlockLength);
#endif
//#ifdef CWMP
	ops->query(ADSL_QUERY_MODESET, &standardused, NULL);
	index += sprintf( stateADSL+index, "Adsl Standard: ");
	switch (standardused) 
	{
		case ME_CMD_ADSL_OPEN_MULTIMODE:
			index += sprintf( stateADSL+index, "Multi-Mode");
			break;
		case ME_CMD_ADSL_OPEN_GDMT:
			index += sprintf( stateADSL+index, "G.DMT");
			break;
		case ME_CMD_ADSL_OPEN_GLITE:
			index += sprintf( stateADSL+index, "G.LITE");
			break;
		case ME_CMD_ADSL_ANSI:
//#if !defined(ISDN_OVERLAY)
			index += sprintf( stateADSL+index, "ANSI T1.413");
//#else	/* annex B */
//			index += sprintf( stateADSL+index, "ETSI");
//#endif
			break;
		case ME_CMD_ADSL_OPEN:
			index += sprintf( stateADSL+index, "Normal Open");
			break;
		case ME_CMD_ADSL2:
			index += sprintf( stateADSL+index, "ADSL2");
			break;
		case ME_CMD_ADSL2PLUS:
			index += sprintf( stateADSL+index, "ADSL2 PLUS");
			break;
		case ME_CMD_VDSL2:
			index += sprintf( stateADSL+index, "VDSL2");
			break;
		case ME_CMD_GVECTOR:
       		index += sprintf( stateADSL+index, "ITU G.993.5(G.Vectoring)");
			break;		
		case ME_CMD_GVECTOR_GINP:
       		index += sprintf( stateADSL+index, "ITU G.993.5(G.Vectoring),G.998.4(G.INP)");
			break;
		case ME_CMD_VDSL2_GINP:
       		index += sprintf( stateADSL+index, "ITU G.993.2(VDSL2), G.998.4(G.INP)");
			break;
		case ME_CMD_ADSL2_GINP:
       		index += sprintf( stateADSL+index, "ITU G.992.3(ADSL2) ,G.998.4(G.INP)");
			break;		
		case ME_CMD_ADSL2PLUS_GINP:
       		index += sprintf( stateADSL+index, "ITU G.992.5(ADSL2PLUS) ,G.998.4(G.INP)");
			break;			
		default:
			index += sprintf( stateADSL+index, "Error");
			break;
	}
	index += sprintf( stateADSL+index, "\n");
	
	ops->query(ADSL_QUERY_FW_REV, &fw_rev, &annex_afe);      
	ops->query(ADSL_QUERY_ANNEX, &annex_set, NULL);   
	index += sprintf( stateADSL+index, "Adsl Type: ");
	index += sprintf( stateADSL+index, "ANNEX_");
	if(annex_set)
	{
		switch(annex_set )
		{
			case ME_CMD_ADSL_ANNEXA:
				index += sprintf( stateADSL+index, "%c", 'A');
				break;
			case ME_CMD_ADSL_ANNEXB:
				index += sprintf( stateADSL+index, "%c", 'B');
				break;
			case ME_CMD_ADSL_ANNEXM:		
				index += sprintf( stateADSL+index, "%c", 'M');
				break;
			case ME_CMD_ADSL_ANNEXI:
				index += sprintf( stateADSL+index, "%c", 'I');
				break;
			case ME_CMD_ADSL_ANNEXJ:
				index += sprintf( stateADSL+index, "%c", 'J');
				break;
			case ME_CMD_ADSL_ANNEXL:
				index += sprintf( stateADSL+index, "%c", 'L');
				break;
			default:
				break;
		}
	}
	else
	{
		index += sprintf( stateADSL+index, "%c", (annex_afe & 0x02)?'A':'B');
	}
	index += sprintf( stateADSL+index, "\n");
	//#endif
	return index;
}

#ifdef CWMP
//add by brian
int getADSLCwmpDiagnostic(char *cwmpDiagnostic,int procindex)
{
	uint16	index = 0;
	int i = 0;
#if defined(TCSUPPORT_CT_WAN_PTM)
	uint8 link_mode;	//ME_CMD_VDSL2
#endif
	memset(&p_diagnostic_set,0,sizeof(TR98_WAN_DSL_DIAGNOSTICS));
	if (adsl_dev_ops == NULL) {
		return 0;
	}
#if defined(TCSUPPORT_CT_WAN_PTM)
	adsl_dev_ops->query(ADSL_QUERY_MODE, &link_mode, NULL);
	if(link_mode == ME_CMD_VDSL2)
	{
		memset(&p_diagnostic_set3,0,sizeof(TR098_WAN_VDSL_DIAGNOSTICS));
		adsl_dev_ops->query(VDSL_QUERY_TR098_VDSL_DIAGNOSTIC, &p_diagnostic_set3, NULL);
	}
	memset(&p_diagnostic_set2,0,sizeof(TR98_WAN_DSL_DIAGNOSTICS_2));
	adsl_dev_ops->query(ADSL_QUERY_TR69_WLAN_DSL_DIAGNOSTIC_2, &p_diagnostic_set2, NULL);
#endif
	adsl_dev_ops->query(ADSL_QUERY_TR69_WLAN_DSL_DIAGNOSTIC, &p_diagnostic_set, NULL);
if(0 == procindex)
{
	index = 0;
	index += sprintf( cwmpDiagnostic+index, "DiagnosticState=%s\n",p_diagnostic_set.DiagnosticState);
	index += sprintf( cwmpDiagnostic+index, "ACTPSDds=%d\n",p_diagnostic_set.ACTPSDds);
	index += sprintf( cwmpDiagnostic+index, "ACTPSDus=%d\n",p_diagnostic_set.ACTPSDus);
	index += sprintf( cwmpDiagnostic+index, "ACTATPds=%d\n",p_diagnostic_set.ACTATPds);
	index += sprintf( cwmpDiagnostic+index, "ACTATPus=%d\n",p_diagnostic_set.ACTATPus);
	index += sprintf( cwmpDiagnostic+index, "HLINSCds=%u\n",p_diagnostic_set.HLINSCds);
#if defined(TCSUPPORT_CT_WAN_PTM)
	index += sprintf( cwmpDiagnostic+index, "HLINSCus=%u\n",p_diagnostic_set2.HLINSCus);
	index += sprintf( cwmpDiagnostic+index, "HLINGds=%u\n",p_diagnostic_set2.HLINGds);
	index += sprintf( cwmpDiagnostic+index, "HLINGus=%u\n",p_diagnostic_set2.HLINGus);
	index += sprintf( cwmpDiagnostic+index, "HLOGGds=%u\n",p_diagnostic_set2.HLOGGds);
	index += sprintf( cwmpDiagnostic+index, "HLOGGus=%u\n",p_diagnostic_set2.HLOGGus);
	index += sprintf( cwmpDiagnostic+index, "HLOGMTds=%u\n",p_diagnostic_set2.HLOGMTds);
	index += sprintf( cwmpDiagnostic+index, "HLOGMTus=%u\n",p_diagnostic_set2.HLOGMTus);
	index += sprintf( cwmpDiagnostic+index, "QLNGds=%u\n",p_diagnostic_set2.QLNGds);
	index += sprintf( cwmpDiagnostic+index, "QLNGus=%u\n",p_diagnostic_set2.QLNGus);
	index += sprintf( cwmpDiagnostic+index, "QLNMTds=%u\n",p_diagnostic_set2.QLNMTds);
	index += sprintf( cwmpDiagnostic+index, "QLNMTus=%u\n",p_diagnostic_set2.QLNMTus);
	index += sprintf( cwmpDiagnostic+index, "SNRGds=%u\n",p_diagnostic_set2.SNRGds);
	index += sprintf( cwmpDiagnostic+index, "SNRGus=%u\n",p_diagnostic_set2.SNRGus);
	index += sprintf( cwmpDiagnostic+index, "SNRMTds=%u\n",p_diagnostic_set2.SNRMTds);
	index += sprintf( cwmpDiagnostic+index, "SNRMTus=%u\n",p_diagnostic_set2.SNRMTus);
#endif

}
else if(1 == procindex)
{
	index = 0;
	//index += sprintf( cwmpDiagnostic+index, "HLINpsds=%d %d,",p_diagnostic_set.HLINpsds[0][0],p_diagnostic_set.HLINpsds[0][1]);		
	for(i = 0; i < 256; i++)
	{
		index += sprintf( cwmpDiagnostic+index, "%d %d,",p_diagnostic_set.HLINpsds[i][0],p_diagnostic_set.HLINpsds[i][1]);		
	}
	//index += sprintf( cwmpDiagnostic+index, "\n");	
}
else if(2 == procindex)
{
	index = 0;
	//index += sprintf( cwmpDiagnostic+index, "HLINpsds=");	
	for(i = 256; i < 512; i++)
	{
		if(i != 511)
		{
			index += sprintf( cwmpDiagnostic+index, "%d %d,",p_diagnostic_set.HLINpsds[i][0],p_diagnostic_set.HLINpsds[i][1]);	
		}
		else
		{
			index += sprintf( cwmpDiagnostic+index, "%d %d",p_diagnostic_set.HLINpsds[i][0],p_diagnostic_set.HLINpsds[i][1]);	
		}	
	}
	//index += sprintf( cwmpDiagnostic+index, "\n");	
}
else if(3 == procindex)
{
	index = 0;
	//index += sprintf( cwmpDiagnostic+index, "QLINpsds=%d,",p_diagnostic_set.QLINpsds[0]);	
	for(i = 0; i < 512; i++)
	{
		if(i != 511)
		{
			index += sprintf( cwmpDiagnostic+index, "%d,",p_diagnostic_set.QLINpsds[i]);	
		}
		else
		{
			index += sprintf( cwmpDiagnostic+index, "%d",p_diagnostic_set.QLINpsds[i]);	
		}	
	}
	//index += sprintf( cwmpDiagnostic+index, "\n");	
}
else if(4 == procindex)
{
	index = 0;
	//index += sprintf( cwmpDiagnostic+index, "SNRpsds=%d,",p_diagnostic_set.SNRpsds[0]);	
	for(i = 0; i < 512; i++)
	{
		if(i != 511)
		{
			index += sprintf( cwmpDiagnostic+index, "%d,",p_diagnostic_set.SNRpsds[i]);	
		}
		else
		{
			index += sprintf( cwmpDiagnostic+index, "%d",p_diagnostic_set.SNRpsds[i]);	
		}	
	}
	//index += sprintf( cwmpDiagnostic+index, "\n");	
}
else if(5 == procindex)
{
	index = 0;
	//index += sprintf( cwmpDiagnostic+index, "BITSpsds=%d,",p_diagnostic_set.BITSpsds[0]);	
	for(i = 0; i < 512; i++)
	{
		if(i != 511)
		{
			index += sprintf( cwmpDiagnostic+index, "%d,",p_diagnostic_set.BITSpsds[i]);	
		}
		else
		{
			index += sprintf( cwmpDiagnostic+index, "%d",p_diagnostic_set.BITSpsds[i]);	
		}	
	}
	//index += sprintf( cwmpDiagnostic+index, "\n");	
}
else if(6 == procindex)
{
	index = 0;
	//index += sprintf( cwmpDiagnostic+index, "GAINSpsds=%d,",p_diagnostic_set.GAINSpsds[0]);
	for(i = 0; i < 512; i++)
	{
		if(i != 511)
		{
			index += sprintf( cwmpDiagnostic+index, "%d,",p_diagnostic_set.GAINSpsds[i]);	
		}
		else
		{
			index += sprintf( cwmpDiagnostic+index, "%d",p_diagnostic_set.GAINSpsds[i]);	
		}	
	}
	//index += sprintf( cwmpDiagnostic+index, "\n");	
}
#if defined(TCSUPPORT_CT_WAN_PTM)
else if(7 == procindex)
{
	index = 0;
	//index += sprintf( cwmpDiagnostic+index, "HLOGpsds=%d,",p_diagnostic_set2.HLOGpsds[0]);
	for(i = 0; i < 512; i++)
	{
		if(i != 511)
		{
			index += sprintf( cwmpDiagnostic+index, "%d,",p_diagnostic_set2.HLOGpsds[i]);	
		}
		else
		{
			index += sprintf( cwmpDiagnostic+index, "%d",p_diagnostic_set2.HLOGpsds[i]);	
		}	
	}
	//index += sprintf( cwmpDiagnostic+index, "\n");	
}
else if(8 == procindex)
{
	index = 0;
	//index += sprintf( cwmpDiagnostic+index, "HLOGpsus=%d,",p_diagnostic_set2.HLOGpsus[0]);
	if(link_mode == ME_CMD_VDSL2)
	{
		for(i = 0; i < 512; i++)
		{
			if(i != 511)
			{
				index += sprintf( cwmpDiagnostic+index, "%d,",p_diagnostic_set3.HLOGpsus[i]);	
			}
			else
			{
				index += sprintf( cwmpDiagnostic+index, "%d",p_diagnostic_set3.HLOGpsus[i]);	
			}	
		}
	}
	else
	{
		for(i = 0; i < 64; i++)
		{
			if(i != 63)
			{
				index += sprintf( cwmpDiagnostic+index, "%d,",p_diagnostic_set2.HLOGpsus[i]);	
			}
			else
			{
				index += sprintf( cwmpDiagnostic+index, "%d",p_diagnostic_set2.HLOGpsus[i]);	
			}	
		}
	}
	//index += sprintf( cwmpDiagnostic+index, "\n");	
}
else if(9 == procindex)
{
	index = 0;
	//index += sprintf( cwmpDiagnostic+index, "HLINpsus=%d %d,",p_diagnostic_set2.HLINpsus[0][0],p_diagnostic_set2.HLINpsus[0][1]);		
	if(link_mode == ME_CMD_VDSL2)
	{
		for(i = 0; i < 256; i++)
		{
			index += sprintf( cwmpDiagnostic+index, "%d %d,",p_diagnostic_set3.HLINpsus[i][0],p_diagnostic_set3.HLINpsus[i][1]);		
		}
	}
	else
	{
		for(i = 0; i < 32; i++)
		{
			index += sprintf( cwmpDiagnostic+index, "%d %d,",p_diagnostic_set2.HLINpsus[i][0],p_diagnostic_set2.HLINpsus[i][1]);		
		}
	}
	//index += sprintf( cwmpDiagnostic+index, "\n");	
}
else if(10 == procindex)
{
	index = 0;
	//index += sprintf( cwmpDiagnostic+index, "HLINpsus=");	
	if(link_mode == ME_CMD_VDSL2)
	{
		for(i = 256; i < 512; i++)
		{
			if(i != 511)
			{
				index += sprintf( cwmpDiagnostic+index, "%d %d,",p_diagnostic_set3.HLINpsus[i][0],p_diagnostic_set3.HLINpsus[i][1]);	
			}
			else
			{
				index += sprintf( cwmpDiagnostic+index, "%d %d",p_diagnostic_set3.HLINpsus[i][0],p_diagnostic_set3.HLINpsus[i][1]);	
			}	
		}
	}
	else
	{
		for(i = 32; i < 64; i++)
		{
			if(i != 63)
			{
				index += sprintf( cwmpDiagnostic+index, "%d %d,",p_diagnostic_set2.HLINpsus[i][0],p_diagnostic_set2.HLINpsus[i][1]);	
			}
			else
			{
				index += sprintf( cwmpDiagnostic+index, "%d %d",p_diagnostic_set2.HLINpsus[i][0],p_diagnostic_set2.HLINpsus[i][1]);	
			}	
		}
	}
	//index += sprintf( cwmpDiagnostic+index, "\n");	
}
else if(11 == procindex)
{
	index = 0;
	//index += sprintf( cwmpDiagnostic+index, "QLNpsus=%d,",p_diagnostic_set2.QLNpsus[0]);
	if(link_mode == ME_CMD_VDSL2)
	{
		for(i = 0; i < 512; i++)
		{
			if(i != 511)
			{
				index += sprintf( cwmpDiagnostic+index, "%d,",p_diagnostic_set3.QLNpsus[i]);	
			}
			else
			{
				index += sprintf( cwmpDiagnostic+index, "%d",p_diagnostic_set3.QLNpsus[i]);	
			}	
		}
	}
	else
	{
		for(i = 0; i < 64; i++)
		{
			if(i != 63)
			{
				index += sprintf( cwmpDiagnostic+index, "%d,",p_diagnostic_set2.QLNpsus[i]);	
			}
			else
			{
				index += sprintf( cwmpDiagnostic+index, "%d",p_diagnostic_set2.QLNpsus[i]);	
			}	
		}
	}
	//index += sprintf( cwmpDiagnostic+index, "\n");	
}
else if(12 == procindex)
{
	index = 0;
	//index += sprintf( cwmpDiagnostic+index, "SNRpsus=%d,",p_diagnostic_set2.SNRpsus[0]);
	if(link_mode == ME_CMD_VDSL2)
	{
		for(i = 0; i < 512; i++)
		{
			if(i != 511)
			{
				index += sprintf( cwmpDiagnostic+index, "%d,",p_diagnostic_set3.SNRpsus[i]);	
			}
			else
			{
				index += sprintf( cwmpDiagnostic+index, "%d",p_diagnostic_set3.SNRpsus[i]);	
			}	
		}
	}
	else
	{
		for(i = 0; i < 64; i++)
		{
			if(i != 63)
			{
				index += sprintf( cwmpDiagnostic+index, "%d,",p_diagnostic_set2.SNRpsus[i]);	
			}
			else
			{
				index += sprintf( cwmpDiagnostic+index, "%d",p_diagnostic_set2.SNRpsus[i]);	
			}	
		}
	}
	//index += sprintf( cwmpDiagnostic+index, "\n");	
}
else if(13 == procindex)
{
	index = 0;
	if(link_mode == ME_CMD_VDSL2)
	{
		//index += sprintf( cwmpDiagnostic+index, "SATNds=%d,",p_diagnostic_set3.SATNds[0]);
		for(i = 0; i < 5; i++)
		{
			if(i != 4)
			{
				index += sprintf( cwmpDiagnostic+index, "%d,",p_diagnostic_set3.SATNds[i]);	
			}
			else
			{
				index += sprintf( cwmpDiagnostic+index, "%d",p_diagnostic_set3.SATNds[i]);	
			}	
		}
	}
	else
	{
		index += sprintf( cwmpDiagnostic+index, "%d",p_diagnostic_set2.SATNds);
	}
	//index += sprintf( cwmpDiagnostic+index, "\n");	
}
else if(14 == procindex)
{
	index = 0;
	if(link_mode == ME_CMD_VDSL2)
	{
		//index += sprintf( cwmpDiagnostic+index, "SATNus=%d,",p_diagnostic_set3.SATNus[0]);
		for(i = 0; i < 5; i++)
		{
			if(i != 4)
			{
				index += sprintf( cwmpDiagnostic+index, "%d,",p_diagnostic_set3.SATNus[i]);	
			}
			else
			{
				index += sprintf( cwmpDiagnostic+index, "%d",p_diagnostic_set3.SATNus[i]);	
			}	
		}
	}
	else
	{
		index += sprintf( cwmpDiagnostic+index, "%d",p_diagnostic_set2.SATNus);
	}

	//index += sprintf( cwmpDiagnostic+index, "\n");	
}
else if(15 == procindex)
{
	index = 0;
	if(link_mode == ME_CMD_VDSL2)
	{
		//index += sprintf( cwmpDiagnostic+index, "LATNds=%d,",p_diagnostic_set3.LATNds[0]);
		for(i = 0; i < 5; i++)
		{
			if(i != 4)
			{
				index += sprintf( cwmpDiagnostic+index, "%d,",p_diagnostic_set3.LATNds[i]);	
			}
			else
			{
				index += sprintf( cwmpDiagnostic+index, "%d",p_diagnostic_set3.LATNds[i]);	
			}	
		}
	}
	//index += sprintf( cwmpDiagnostic+index, "\n");	
}
else if(16 == procindex)
{
	index = 0;
	if(link_mode == ME_CMD_VDSL2)
	{
		//index += sprintf( cwmpDiagnostic+index, "LATNus=%d,",p_diagnostic_set3.LATNus[0]);
		for(i = 0; i < 5; i++)
		{
			if(i != 4)
			{
				index += sprintf( cwmpDiagnostic+index, "%d,",p_diagnostic_set3.LATNus[i]);	
			}
			else
			{
				index += sprintf( cwmpDiagnostic+index, "%d",p_diagnostic_set3.LATNus[i]);	
			}	
		}
	}
	//index += sprintf( cwmpDiagnostic+index, "\n");	
}


#endif

	//len = index;
	//index += sprintf( cwmpDiagnostic+index, "totallen=%d",len);	
	return index;	
	
}
#endif

#if defined(CWMP) || defined(TCSUPPORT_SNMP_ATMADSL)

int
getADSLStateShow(char *stateADSL, uint8 long_fmt)
{
	uint16	index=0;

	TR98_WAN_DSL_INTERFACE_CFG_STS_ST p_ShowtimeRate;
    	T_adsl_def_counter_set adsl_def_counter_set;
	uint8 modemst;
	T_adsl_def_bitmap_set bitMapSet;
	
#if defined(TCSUPPORT_SNMP_ATMADSL)
	/*Add for snmp adsl mib*/
	PMS_PARAMS pms;
	T_adsl_def_counter_set def_counter_set;
	SNMP_ADSL_NODE snmpNode;
	int32 interval_count = 0;
	
	CELL_COUNT1 CellOperdata;
	SNMP_ADSL_NODE_2 snmpAdslNode_2;
#endif

	if (adsl_dev_ops == NULL) {
		index += sprintf( stateADSL+index, "ADSL link status: down\n");
		return index;
	}

	adsl_dev_ops->query(ADSL_QUERY_STATUS, &modemst, NULL );
	if (modemst == ADSL_MODEM_STATE_DOWN)
		index += sprintf( stateADSL+index, "ADSL link status: down\n");
	else if (modemst == ADSL_MODEM_STATE_WAIT_INIT)
		index += sprintf( stateADSL+index, "ADSL link status: wait for init\n");
	else if (modemst == ADSL_MODEM_STATE_INIT)
		index += sprintf( stateADSL+index, "ADSL link status: initializing\n");
	else if (modemst == ADSL_MODEM_STATE_UP)
		index += sprintf( stateADSL+index, "ADSL link status: up\n");
	else
		index += sprintf( stateADSL+index, "ADSL link status: unknown\n");

	if (modemst != ADSL_MODEM_STATE_UP) {
		return index;
	}

	if (!long_fmt)
		return index;
	
	adsl_dev_ops->query(ADSL_QUERY_DEFECT, &bitMapSet, NULL);
	adsl_dev_ops->query(ADSL_QUERY_TR69_WLAN_DSL_INTERFACE_Config, &p_ShowtimeRate, NULL);
    	adsl_dev_ops->query(ADSL_QUERY_DEFECT_CNT, &adsl_def_counter_set, NULL);
		
	index += sprintf( stateADSL+index, "\nshowtime\n");
	index += sprintf( stateADSL+index, "ReceiveBlock: %lu\n",
						p_ShowtimeRate.ReceiveBlock);
	index += sprintf( stateADSL+index, "TransmitBlock: %lu\n",
						p_ShowtimeRate.TransmitBlock);
	index += sprintf( stateADSL+index, "CellDelin: %lu\n",
						p_ShowtimeRate.CellDelin);
#if defined(CWMP) && defined(TCSUPPORT_TTNET)
	index += sprintf( stateADSL+index, "LinkRetrain: %lu\n",
						p_ShowtimeRate.LinkRetrain - initLinkRetrainCount);
#else
	index += sprintf( stateADSL+index, "LinkRetrain: %lu\n",
						p_ShowtimeRate.LinkRetrain);
#endif
	index += sprintf( stateADSL+index, "InitErrors: %lu\n",
						p_ShowtimeRate.InitErrors);
	index += sprintf( stateADSL+index, "InitTimeouts: %lu\n",
						p_ShowtimeRate.InitTimeouts);
	index += sprintf( stateADSL+index, "LossOfFraming: %lu\n",
						p_ShowtimeRate.LossOfFraming);
	index += sprintf( stateADSL+index, "LOF: %d\n",
						(bitMapSet.nearstate & 0x40)>>6);
	index += sprintf( stateADSL+index, "ErroredSecs: %lu\n",
						p_ShowtimeRate.ErroredSecs);
	index += sprintf( stateADSL+index, "SeverelyErroredSecs: %lu\n",
						p_ShowtimeRate.SeverelyErroredSecs);
	index += sprintf( stateADSL+index, "near-end FEC error fast: %u\n",
						adsl_def_counter_set.near_end.FecNotInterleaved);
	index += sprintf( stateADSL+index, "near-end FEC error interleaved: %u\n",
						adsl_def_counter_set.near_end.FecInterleaved);
	index += sprintf( stateADSL+index, "near-end CRC error fast: %u\n",
						adsl_def_counter_set.near_end.CrcNotInterleaved);
	index += sprintf( stateADSL+index, "near-end CRC error interleaved: %u\n",
						adsl_def_counter_set.near_end.CrcInterleaved);
	index += sprintf( stateADSL+index, "near-end HEC error fast: %u\n",
						adsl_def_counter_set.near_end.HecNotInterleaved);
	index += sprintf( stateADSL+index, "near-end HEC error interleaved: %u\n",
						adsl_def_counter_set.near_end.HecInterleaved);
	index += sprintf( stateADSL+index, "far-end FEC error fast: %u\n",
						adsl_def_counter_set.far_end.FecNotInterleaved);
	index += sprintf( stateADSL+index, "far-end FEC error interleaved: %u\n",
						adsl_def_counter_set.far_end.FecInterleaved);
	index += sprintf( stateADSL+index, "far-end CRC error fast: %u\n",
						adsl_def_counter_set.far_end.CrcNotInterleaved);
	index += sprintf( stateADSL+index, "far-end CRC error interleaved: %u\n",
						adsl_def_counter_set.far_end.CrcInterleaved);
	index += sprintf( stateADSL+index, "far-end HEC error fast: %u\n",
						adsl_def_counter_set.far_end.HecNotInterleaved);
	index += sprintf( stateADSL+index, "far-end HEC error interleaved: %u\n",
						adsl_def_counter_set.far_end.HecInterleaved);

#if defined(TCSUPPORT_SNMP_ATMADSL)
	/*Add for snmp adsl mib*/

	adsl_dev_ops->query(ADSL_QUERY_PMS_PARAM, &pms, NULL);
	index += sprintf( stateADSL+index, ATUR_CHAN_CURR_RX_BLKS, pms.RSWords_ds);
	index += sprintf( stateADSL+index, ATUR_CHAN_CURR_TX_BLKS, pms.RSWords_us);

	adsl_dev_ops->query(ADSL_QUERY_DEFECT_CNT, &def_counter_set, NULL );
	index += sprintf( stateADSL+index, ATUR_CHAN_CURR_CRT_BLKS, def_counter_set.near_end.FecNotInterleaved);

	adsl_dev_ops->query(ADSL_QUERY_SNMP_ADSL_NODE, &snmpNode, NULL);
	index += sprintf( stateADSL+index, ATUR_CHAN_CURR_UNCRT_BLKS, snmpNode.UcFecCnt);
	
	index += sprintf( stateADSL+index, CURDAY_TIME_ELAPSED, snmp_curr_day_timer_cnt >> 1);
	index += sprintf( stateADSL+index, QURTHOUR_TIME_ELAPSED, snmp_qut_hr_timer_cnt >> 1);	
	index += sprintf( stateADSL+index, PREVDAY_ATUR_CHAN_MONISECS, 0);
	interval_count = (snmp_qut_hr_intervels_cnt<ADSLMIB_CHANPERFINTERVAL_MAXNUM) ?snmp_qut_hr_intervels_cnt:ADSLMIB_CHANPERFINTERVAL_MAXNUM;
	index += sprintf( stateADSL+index, ATUR_CHAN_VALID_ITLS, interval_count);
	index += sprintf( stateADSL+index, ATUR_CHAN_INVALID_ITLS, 0);

	adsl_dev_ops->query(ADSL_QUERY_CELL_CNT1, &CellOperdata, NULL);
	index += sprintf( stateADSL+index, ES_DOWNSTREAM, CellOperdata.DsEs);
	index += sprintf( stateADSL+index, ES_UPSTREAM, CellOperdata.UsEs);
	index += sprintf( stateADSL+index, SES_DOWNSTREAM, CellOperdata.DsSes);
	index += sprintf( stateADSL+index, SES_UPSTREAM, CellOperdata.UsSes);

	adsl_dev_ops->query(ADSL_QUERY_STATISTICS_2, &snmpAdslNode_2, NULL);
	index += sprintf( stateADSL+index, UAS_DOWNSTREAM, snmpAdslNode_2.RxUAS);
	index += sprintf( stateADSL+index, UAS_UPSTREAM, snmpAdslNode_2.TxUAS);
	index += sprintf( stateADSL+index, RX_BITERRCNT, snmpAdslNode_2.RxBitErrorCnt);
	index += sprintf( stateADSL+index, TX_BITERRCNT, snmpAdslNode_2.TxBitErrorCnt);
	
	#endif
	return index;
}
int
getADSLStateLast(char *stateADSL, uint8 long_fmt)
{
	uint16	index=0;

	uint8 modemst;
	
	if (adsl_dev_ops == NULL) {
		index += sprintf( stateADSL+index, "ADSL link status: down\n");
		return index;
	}

	adsl_dev_ops->query(ADSL_QUERY_STATUS, &modemst, NULL );
	if (modemst == ADSL_MODEM_STATE_DOWN)
		index += sprintf( stateADSL+index, "ADSL link status: down\n");
	else if (modemst == ADSL_MODEM_STATE_WAIT_INIT)
		index += sprintf( stateADSL+index, "ADSL link status: wait for init\n");
	else if (modemst == ADSL_MODEM_STATE_INIT)
		index += sprintf( stateADSL+index, "ADSL link status: initializing\n");
	else if (modemst == ADSL_MODEM_STATE_UP)
		index += sprintf( stateADSL+index, "ADSL link status: up\n");
	else
		index += sprintf( stateADSL+index, "ADSL link status: unknown\n");

	if (!long_fmt)
		return index;
	
	index += sprintf( stateADSL+index, "\nLast showtime\n");
	index += sprintf( stateADSL+index, "last showtime flag: %u\n", flagLastShowTimeStart);
	index += sprintf( stateADSL+index, "total ReceiveBlock: %lu\n",
						WanIfcfgStatsLastShowTime_ShowtimeRate.ReceiveBlock);
	index += sprintf( stateADSL+index, "last TransmitBlock: %lu\n",
						WanIfcfgStatsLastShowTime_ShowtimeRate.TransmitBlock);
	index += sprintf( stateADSL+index, "last CellDelin: %lu\n",
						WanIfcfgStatsLastShowTime_ShowtimeRate.CellDelin);
	index += sprintf( stateADSL+index, "last LinkRetrain: %lu\n",
						WanIfcfgStatsLastShowTime_ShowtimeRate.LinkRetrain);
	index += sprintf( stateADSL+index, "last InitErrors: %lu\n",
						WanIfcfgStatsLastShowTime_ShowtimeRate.InitErrors);
	index += sprintf( stateADSL+index, "last InitTimeouts: %lu\n",
						WanIfcfgStatsLastShowTime_ShowtimeRate.InitTimeouts);
	index += sprintf( stateADSL+index, "last LossOfFraming: %lu\n",
						WanIfcfgStatsLastShowTime_ShowtimeRate.LossOfFraming);
	index += sprintf( stateADSL+index, "last ErroredSecs: %lu\n",
						WanIfcfgStatsLastShowTime_ShowtimeRate.ErroredSecs);
	index += sprintf( stateADSL+index, "last SeverelyErroredSecs: %lu\n",
						WanIfcfgStatsLastShowTime_ShowtimeRate.SeverelyErroredSecs);
	index += sprintf( stateADSL+index, "last near-end FEC error fast: %u\n",
						WanIfcfgStatsLastShowTime_counter_set.near_end.FecNotInterleaved);
	index += sprintf( stateADSL+index, "last near-end FEC error interleaved: %u\n",
						WanIfcfgStatsLastShowTime_counter_set.near_end.FecInterleaved);
	index += sprintf( stateADSL+index, "last near-end CRC error fast: %u\n",
						WanIfcfgStatsLastShowTime_counter_set.near_end.CrcNotInterleaved);
	index += sprintf( stateADSL+index, "last near-end CRC error interleaved: %u\n",
						WanIfcfgStatsLastShowTime_counter_set.near_end.CrcInterleaved);
	index += sprintf( stateADSL+index, "last near-end HEC error fast: %u\n",
						WanIfcfgStatsLastShowTime_counter_set.near_end.HecNotInterleaved);
	index += sprintf( stateADSL+index, "last near-end HEC error interleaved: %u\n",
						WanIfcfgStatsLastShowTime_counter_set.near_end.HecInterleaved);
	index += sprintf( stateADSL+index, "last far-end FEC error fast: %u\n",
						WanIfcfgStatsLastShowTime_counter_set.far_end.FecNotInterleaved);
	index += sprintf( stateADSL+index, "last far-end FEC error interleaved: %u\n",
						WanIfcfgStatsLastShowTime_counter_set.far_end.FecInterleaved);
	index += sprintf( stateADSL+index, "last far-end CRC error fast: %u\n",
						WanIfcfgStatsLastShowTime_counter_set.far_end.CrcNotInterleaved);
	index += sprintf( stateADSL+index, "last far-end CRC error interleaved: %u\n",
						WanIfcfgStatsLastShowTime_counter_set.far_end.CrcInterleaved);
	return index;
}

int
getADSLStateTotal(char *stateADSL, uint8 long_fmt)
{
	uint16	index=0;

	uint8 modemst;
	
	if (adsl_dev_ops == NULL) {
		index += sprintf( stateADSL+index, "ADSL link status: down\n");
		return index;
	}

	adsl_dev_ops->query(ADSL_QUERY_STATUS, &modemst, NULL );
	if (modemst == ADSL_MODEM_STATE_DOWN)
		index += sprintf( stateADSL+index, "ADSL link status: down\n");
	else if (modemst == ADSL_MODEM_STATE_WAIT_INIT)
		index += sprintf( stateADSL+index, "ADSL link status: wait for init\n");
	else if (modemst == ADSL_MODEM_STATE_INIT)
		index += sprintf( stateADSL+index, "ADSL link status: initializing\n");
	else if (modemst == ADSL_MODEM_STATE_UP)
		index += sprintf( stateADSL+index, "ADSL link status: up\n");
	else
		index += sprintf( stateADSL+index, "ADSL link status: unknown\n");

	if (!long_fmt)
		return index;

	index += sprintf( stateADSL+index, "\nTotal Showtime\n");
	index += sprintf( stateADSL+index, "total ReceiveBlock: %lu\n",
						WanIfcfgStatsTotal_ShowtimeRate.ReceiveBlock);
	index += sprintf( stateADSL+index, "total TransmitBlock: %lu\n",
						WanIfcfgStatsTotal_ShowtimeRate.TransmitBlock);
	index += sprintf( stateADSL+index, "total CellDelin: %lu\n",
						WanIfcfgStatsTotal_ShowtimeRate.CellDelin);
	index += sprintf( stateADSL+index, "total LinkRetrain: %lu\n",
						WanIfcfgStatsTotal_ShowtimeRate.LinkRetrain);
	index += sprintf( stateADSL+index, "total InitErrors: %lu\n",
						WanIfcfgStatsTotal_ShowtimeRate.InitErrors);
	index += sprintf( stateADSL+index, "total InitTimeouts: %lu\n",
						WanIfcfgStatsTotal_ShowtimeRate.InitTimeouts);
	index += sprintf( stateADSL+index, "total LossOfFraming: %lu\n",
						WanIfcfgStatsTotal_ShowtimeRate.LossOfFraming);
	index += sprintf( stateADSL+index, "total ErroredSecs: %lu\n",
						WanIfcfgStatsTotal_ShowtimeRate.ErroredSecs);
	index += sprintf( stateADSL+index, "total SeverelyErroredSecs: %lu\n",
						WanIfcfgStatsTotal_ShowtimeRate.SeverelyErroredSecs);
	index += sprintf( stateADSL+index, "total near-end FEC error fast: %u\n",
						WanIfcfgStatsTotal_counter_set.near_end.FecNotInterleaved);
	index += sprintf( stateADSL+index, "total near-end FEC error interleaved: %u\n",
						WanIfcfgStatsTotal_counter_set.near_end.FecInterleaved);
	index += sprintf( stateADSL+index, "total near-end CRC error fast: %u\n",
						WanIfcfgStatsTotal_counter_set.near_end.CrcNotInterleaved);
	index += sprintf( stateADSL+index, "total near-end CRC error interleaved: %u\n",
						WanIfcfgStatsTotal_counter_set.near_end.CrcInterleaved);
	index += sprintf( stateADSL+index, "total near-end HEC error fast: %u\n",
						WanIfcfgStatsTotal_counter_set.near_end.HecNotInterleaved);
	index += sprintf( stateADSL+index, "total near-end HEC error interleaved: %u\n",
						WanIfcfgStatsTotal_counter_set.near_end.HecInterleaved);
	index += sprintf( stateADSL+index, "total far-end FEC error fast: %u\n",
						WanIfcfgStatsTotal_counter_set.far_end.FecNotInterleaved);
	index += sprintf( stateADSL+index, "total far-end FEC error interleaved: %u\n",
						WanIfcfgStatsTotal_counter_set.far_end.FecInterleaved);
	index += sprintf( stateADSL+index, "total far-end CRC error fast: %u\n",
						WanIfcfgStatsTotal_counter_set.far_end.CrcNotInterleaved);
	index += sprintf( stateADSL+index, "total far-end CRC error interleaved: %u\n",
						WanIfcfgStatsTotal_counter_set.far_end.CrcInterleaved);

#if defined(TCSUPPORT_SNMP_ATMADSL)
	/*Add for snmp adsl mib*/
	index += sprintf( stateADSL+index, "\n");

	//since agent reset
	index += sprintf( stateADSL+index, TOTAL_ATUR_CHAN_RX_BLKS,
						SnmpADSLAturChanTotal.ATURChanRxBlks);
	index += sprintf( stateADSL+index, TOTAL_ATUR_CHAN_TX_BLKS,
						SnmpADSLAturChanTotal.ATURChanTxBlks);
	index += sprintf( stateADSL+index, TOTAL_ATUR_CHAN_CRT_BLKS,
						SnmpADSLAturChanTotal.ATURChanCorrectBlks);
	index += sprintf( stateADSL+index, TOTAL_ATUR_CHAN_UNCRT_BLKS,
						SnmpADSLAturChanTotal.ATURChanUnCorrectBlks);
	index += sprintf( stateADSL+index, "\n");
	index += sprintf( stateADSL+index, TOTAL_ES_DOWNSTREAM,
						SnmpADSLCTAR5211Total.DsEs);
	index += sprintf( stateADSL+index, TOTAL_ES_UPSTREAM,
						SnmpADSLCTAR5211Total.UsEs);
	index += sprintf( stateADSL+index, TOTAL_SES_DOWNSTREAM,
						SnmpADSLCTAR5211Total.DsSes);
	index += sprintf( stateADSL+index, TOTAL_SES_UPSTREAM,
						SnmpADSLCTAR5211Total.UsSes);	
	index += sprintf( stateADSL+index, TOTAL_UAS_DOWNSTREAM,
						SnmpADSLCTAR5211Total.RxUAS);
	index += sprintf( stateADSL+index, TOTAL_UAS_UPSTREAM,
						SnmpADSLCTAR5211Total.TxUAS);
#endif
	return index;
}

int
getADSLStateCurrdayStart(char *stateADSL, uint8 long_fmt)
{
	uint16	index=0;
	
	if (!long_fmt)
		return index;

	index += sprintf( stateADSL+index, "\nCurrent day Start Showtime\n");
	index += sprintf( stateADSL+index, "curdaySt ReceiveBlock: %lu\n",
						WanIfcfgStatsCurdaySt_ShowtimeRate.ReceiveBlock);
	index += sprintf( stateADSL+index, "curdaySt TransmitBlock: %lu\n",
						WanIfcfgStatsCurdaySt_ShowtimeRate.TransmitBlock);
	index += sprintf( stateADSL+index, "curdaySt CellDelin: %lu\n",
						WanIfcfgStatsCurdaySt_ShowtimeRate.CellDelin);
	index += sprintf( stateADSL+index, "curdaySt LinkRetrain: %lu\n",
						WanIfcfgStatsCurdaySt_ShowtimeRate.LinkRetrain);
	index += sprintf( stateADSL+index, "curdaySt InitErrors: %lu\n",
						WanIfcfgStatsCurdaySt_ShowtimeRate.InitErrors);
	index += sprintf( stateADSL+index, "curdaySt InitTimeouts: %lu\n",
						WanIfcfgStatsCurdaySt_ShowtimeRate.InitTimeouts);
	index += sprintf( stateADSL+index, "curdaySt LossOfFraming: %lu\n",
						WanIfcfgStatsCurdaySt_ShowtimeRate.LossOfFraming);
	index += sprintf( stateADSL+index, "curdaySt ErroredSecs: %lu\n",
						WanIfcfgStatsCurdaySt_ShowtimeRate.ErroredSecs);
	index += sprintf( stateADSL+index, "curdaySt SeverelyErroredSecs: %lu\n",
						WanIfcfgStatsCurdaySt_ShowtimeRate.SeverelyErroredSecs);
	index += sprintf( stateADSL+index, "curdaySt near-end FEC error fast: %u\n",
						WanIfcfgStatsCurdaySt_counter_set.near_end.FecNotInterleaved);
	index += sprintf( stateADSL+index, "curdaySt near-end FEC error interleaved: %u\n",
						WanIfcfgStatsCurdaySt_counter_set.near_end.FecInterleaved);
	index += sprintf( stateADSL+index, "curdaySt near-end CRC error fast: %u\n",
						WanIfcfgStatsCurdaySt_counter_set.near_end.CrcNotInterleaved);
	index += sprintf( stateADSL+index, "curdaySt near-end CRC error interleaved: %u\n",
						WanIfcfgStatsCurdaySt_counter_set.near_end.CrcInterleaved);
	index += sprintf( stateADSL+index, "curdaySt near-end HEC error fast: %u\n",
						WanIfcfgStatsCurdaySt_counter_set.near_end.HecNotInterleaved);
	index += sprintf( stateADSL+index, "curdaySt near-end HEC error interleaved: %u\n",
						WanIfcfgStatsCurdaySt_counter_set.near_end.HecInterleaved);
	index += sprintf( stateADSL+index, "curdaySt far-end FEC error fast: %u\n",
						WanIfcfgStatsCurdaySt_counter_set.far_end.FecNotInterleaved);
	index += sprintf( stateADSL+index, "curdaySt far-end FEC error interleaved: %u\n",
						WanIfcfgStatsCurdaySt_counter_set.far_end.FecInterleaved);
	index += sprintf( stateADSL+index, "curdaySt far-end CRC error fast: %u\n",
						WanIfcfgStatsCurdaySt_counter_set.far_end.CrcNotInterleaved);
	index += sprintf( stateADSL+index, "curdaySt far-end CRC error interleaved: %u\n",
						WanIfcfgStatsCurdaySt_counter_set.far_end.CrcInterleaved);


#if defined(TCSUPPORT_SNMP_ATMADSL)
	/*Add for snmp adsl mib*/
	index += sprintf( stateADSL+index, "\n");
	//within the current 1 day period
	index += sprintf( stateADSL+index, CURRDAY_ATUR_CHAN_RX_BLKS,
						SnmpADSLAturChanCurrDay.ATURChanRxBlks);
	index += sprintf( stateADSL+index, CURRDAY_ATUR_CHAN_TX_BLKS,
						SnmpADSLAturChanCurrDay.ATURChanTxBlks);
	index += sprintf( stateADSL+index, CURRDAY_ATUR_CHAN_CRT_BLKS,
						SnmpADSLAturChanCurrDay.ATURChanCorrectBlks);
	index += sprintf( stateADSL+index, CURRDAY_ATUR_CHAN_UNCRT_BLKS,
						SnmpADSLAturChanCurrDay.ATURChanUnCorrectBlks);	
#endif
	return index;
}

int
getADSLStateQuthourStart(char *stateADSL, uint8 long_fmt)
{
	uint16	index=0;
	
	if (!long_fmt)
		return index;

	index += sprintf( stateADSL+index, "\nQuarter hour Start Showtime\n");
	index += sprintf( stateADSL+index, "quthour ReceiveBlock: %lu\n",
						WanIfcfgStatsQuthrSt_ShowtimeRate.ReceiveBlock);
	index += sprintf( stateADSL+index, "quthour TransmitBlock: %lu\n",
						WanIfcfgStatsQuthrSt_ShowtimeRate.TransmitBlock);
	index += sprintf( stateADSL+index, "quthour CellDelin: %lu\n",
						WanIfcfgStatsQuthrSt_ShowtimeRate.CellDelin);
	index += sprintf( stateADSL+index, "quthour LinkRetrain: %lu\n",
						WanIfcfgStatsQuthrSt_ShowtimeRate.LinkRetrain);
	index += sprintf( stateADSL+index, "quthour InitErrors: %lu\n",
						WanIfcfgStatsQuthrSt_ShowtimeRate.InitErrors);
	index += sprintf( stateADSL+index, "quthour InitTimeouts: %lu\n",
						WanIfcfgStatsQuthrSt_ShowtimeRate.InitTimeouts);
	index += sprintf( stateADSL+index, "quthour LossOfFraming: %lu\n",
						WanIfcfgStatsQuthrSt_ShowtimeRate.LossOfFraming);
	index += sprintf( stateADSL+index, "quthour ErroredSecs: %lu\n",
						WanIfcfgStatsQuthrSt_ShowtimeRate.ErroredSecs);
	index += sprintf( stateADSL+index, "quthour SeverelyErroredSecs: %lu\n",
						WanIfcfgStatsQuthrSt_ShowtimeRate.SeverelyErroredSecs);
	index += sprintf( stateADSL+index, "quthour near-end FEC error fast: %u\n",
						WanIfcfgStatsQuthrSt_counter_set.near_end.FecNotInterleaved);
	index += sprintf( stateADSL+index, "quthour near-end FEC error interleaved: %u\n",
						WanIfcfgStatsQuthrSt_counter_set.near_end.FecInterleaved);
	index += sprintf( stateADSL+index, "quthour near-end CRC error fast: %u\n",
						WanIfcfgStatsQuthrSt_counter_set.near_end.CrcNotInterleaved);
	index += sprintf( stateADSL+index, "quthour near-end CRC error interleaved: %u\n",
						WanIfcfgStatsQuthrSt_counter_set.near_end.CrcInterleaved);
	index += sprintf( stateADSL+index, "quthour near-end HEC error fast: %u\n",
						WanIfcfgStatsQuthrSt_counter_set.near_end.HecNotInterleaved);
	index += sprintf( stateADSL+index, "quthour near-end HEC error interleaved: %u\n",
						WanIfcfgStatsQuthrSt_counter_set.near_end.HecInterleaved);
	index += sprintf( stateADSL+index, "quthour far-end FEC error fast: %u\n",
						WanIfcfgStatsQuthrSt_counter_set.far_end.FecNotInterleaved);
	index += sprintf( stateADSL+index, "quthour far-end FEC error interleaved: %u\n",
						WanIfcfgStatsQuthrSt_counter_set.far_end.FecInterleaved);
	index += sprintf( stateADSL+index, "quthour far-end CRC error fast: %u\n",
						WanIfcfgStatsQuthrSt_counter_set.far_end.CrcNotInterleaved);
	index += sprintf( stateADSL+index, "quthour far-end CRC error interleaved: %u\n",
						WanIfcfgStatsQuthrSt_counter_set.far_end.CrcInterleaved);

#if defined(TCSUPPORT_SNMP_ATMADSL)
	/*Add for snmp adsl mib*/
	index += sprintf( stateADSL+index, "\n");
	//within the currren 15 minutes interval
	index += sprintf( stateADSL+index, QUTHOUR_ATUR_CHAN_RX_BLKS,
						SnmpADSLAturChanQutHour.ATURChanRxBlks);
	index += sprintf( stateADSL+index, QUTHOUR_ATUR_CHAN_TX_BLKS,
						SnmpADSLAturChanQutHour.ATURChanTxBlks);
	index += sprintf( stateADSL+index, QUTHOUR_ATUR_CHAN_CRT_BLKS,
						SnmpADSLAturChanQutHour.ATURChanCorrectBlks);
	index += sprintf( stateADSL+index, QUTHOUR_ATUR_CHAN_UNCRT_BLKS,
						SnmpADSLAturChanQutHour.ATURChanUnCorrectBlks);	
#endif
	return index;
}
#endif

#if defined(TCSUPPORT_SNMP_ATMADSL)
int getADSLStatusInfo(char *adsl_stas)
{
	int i;
	uint8 modemst;

	ADSL_STATUS_INFO adslStatus[] = {
		{ADSL_MODEM_STATE_DOWN, ADSL_LINK_STATUS_DOWN},
		{ADSL_MODEM_STATE_WAIT_INIT, ADSL_LINK_STATUS_WAIT_INIT},
		{ADSL_MODEM_STATE_INIT, ADSL_LINK_STATUS_INIT},
		{ADSL_MODEM_STATE_UP, ADSL_LINK_STATUS_UP},
		{0, NULL},
	};
	
	if (adsl_dev_ops == NULL) {
		strcpy(adsl_stas, ADSL_LINK_STATUS_DOWN);
		return -1;
	}

	adsl_dev_ops->query(ADSL_QUERY_STATUS, &modemst, NULL );
	for(i=0; adslStatus[i].statusInfo != NULL; i++){
		if(modemst == adslStatus[i].modemst) {
			strcpy(adsl_stas, adslStatus[i].statusInfo);
			break;
		}
	}
	if(adslStatus[i].statusInfo == NULL)//unknown status
		strcpy(adsl_stas, ADSL_LINK_STATUS_UNKNOWN);

	return 0;
}
//within the most recent previous 1 day period
int
getADSLStatePrevdayStart(char *stateADSL, uint8 long_fmt)
{
	uint16	index=0;
	char adslStatus[64] = {0};

	if(getADSLStatusInfo(adslStatus) == 0)
		index += sprintf( stateADSL+index, adslStatus);
	else
		return index;

	if (!long_fmt)
		return index;

	index += sprintf( stateADSL+index, PREVDAY_ADSL_INFO);
	index += sprintf( stateADSL+index, PREVDAY_ATUR_CHAN_RX_BLKS,
						SnmpADSLAturChanPrevDay.ATURChanRxBlks);
	index += sprintf( stateADSL+index, PREVDAY_ATUR_CHAN_TX_BLKS,
						SnmpADSLAturChanPrevDay.ATURChanTxBlks);
	index += sprintf( stateADSL+index, PREVDAY_ATUR_CHAN_CRT_BLKS,
						SnmpADSLAturChanPrevDay.ATURChanCorrectBlks);
	index += sprintf( stateADSL+index, PREVDAY_ATUR_CHAN_UNCRT_BLKS,
						SnmpADSLAturChanPrevDay.ATURChanUnCorrectBlks);
	return index;
}


int
getADSLStateIntervalStart(char *stateADSL, uint8 long_fmt)
{
	uint16	index=0;
	int i;
	char adslStatus[64] = {0};

	if(getADSLStatusInfo(adslStatus) == 0)
		index += sprintf( stateADSL+index, adslStatus);
	else
		return index;
	
	if (!long_fmt)
		return index;
	
	index += sprintf( stateADSL+index, ATUR_CHAN_ITL_CNT, snmp_qut_hr_intervels_cnt);
	for(i=0; i<ADSLMIB_CHANPERFINTERVAL_MAXNUM+1; i++){
		index += sprintf( stateADSL+index, ATUR_CHAN_ITL_INFO, i+1);
		index += sprintf( stateADSL+index, ATUR_CHAN_ITL_RX_BLKS, i, SnmpADSLAturChanInterval[i].ATURChanRxBlks);
		index += sprintf( stateADSL+index, ATUR_CHAN_ITL_TX_BLKS, i, SnmpADSLAturChanInterval[i].ATURChanTxBlks);
		index += sprintf( stateADSL+index, ATUR_CHAN_ITL_CRT_BLKS, i, SnmpADSLAturChanInterval[i].ATURChanCorrectBlks);
		index += sprintf( stateADSL+index, ATUR_CHAN_ITL_UNCRT_BLKS, i, SnmpADSLAturChanInterval[i].ATURChanUnCorrectBlks);		
	}		

	return index;
}
#endif

#if defined(TCSUPPORT_CPU_MT7510) || defined(TCSUPPORT_CPU_MT7505) || defined(TCSUPPORT_FTTDP_V2)
static int fpga_adsl_proc_stats(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[4];
				    
	if (count > sizeof(val_string) - 1)
		return -EINVAL;
									        
	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;
				        
	val_string[count] = '\0';
													    
	if (sscanf(val_string, "%d", &isFpgaAdslVdslUp) != 1)
    {
        printk("\nUsage: echo x > /proc/tc3162u/fpga_adsl_stats\n");
		return count;
    }

    if (isFpgaAdslVdslUp == 1)
        printk("\nADSL (ATM FPGA) is Link Up!\n");
    else if (isFpgaAdslVdslUp == 2)
        printk("\nVDSL (PTM FPGA) is Link Up!\n");
    else
    {
        isFpgaAdslVdslUp = 0;
        printk("\nLink is down (FPGA)!\n");
    }
    
	return count;
}

static int fpga_adsl_up_rate_proc_stats(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[8];
				    
	if (count > sizeof(val_string) - 1)
		return -EINVAL;
									        
	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;
				        
	val_string[count] = '\0';
													    
	if (sscanf(val_string, "%d", &fpgaAdslUpRate) != 1)
    {
        printk("\nUsage: echo x > /proc/tc3162u/fpga_adsl_up_rate (Kbps)\n");
		return count;
    }

	printk("fpgaAdslUpRate %d\n", fpgaAdslUpRate);

	
    if (fpgaAdslUpRate < 0){
        printk("\nADSL Up Rate Value is Wrong\n");
		printk("Using Default ADSL Up Rate Value (1024)\n");
    } 
    
	return count;
}

#endif
#ifdef TCSUPPORT_CPU_EN7512 
int xdsl_proc_stats(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getXDSLState(buf, 1, LINE0);
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
#endif

int adsl_proc_stats(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getADSLState(buf, 1, LINE0);
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

#if defined(TCSUPPORT_CPU_MT7510) && defined(TCSUPPORT_BONDING)
int adsl_proc_stats_slave(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getADSLState(buf, 1, LINE1);
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
#endif
int adsl_proc_perf(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getADSLState(buf, 0, LINE0);
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

int adsl_proc_fwver(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getADSLFwVer(buf, adsl_dev_ops);
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
#ifdef CWMP
//add by brian
int adsl_proc_cwmp_diagnostic0(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getADSLCwmpDiagnostic(buf,0);
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
//add by brian
int adsl_proc_cwmp_diagnostic1(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getADSLCwmpDiagnostic(buf,1);
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
//add by brian
int adsl_proc_cwmp_diagnostic2(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getADSLCwmpDiagnostic(buf,2);
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
//add by brian
int adsl_proc_cwmp_diagnostic3(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getADSLCwmpDiagnostic(buf,3);
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

//add by brian
int adsl_proc_cwmp_diagnostic4(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getADSLCwmpDiagnostic(buf,4);
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

//add by brian
int adsl_proc_cwmp_diagnostic5(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getADSLCwmpDiagnostic(buf,5);
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
//add by brian
int adsl_proc_cwmp_diagnostic6(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getADSLCwmpDiagnostic(buf,6);
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

#if defined(TCSUPPORT_CT_WAN_PTM)
int adsl_proc_cwmp_diagnostic7(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getADSLCwmpDiagnostic(buf,7);
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

int adsl_proc_cwmp_diagnostic8(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getADSLCwmpDiagnostic(buf,8);
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

int adsl_proc_cwmp_diagnostic9(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getADSLCwmpDiagnostic(buf,9);
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

int adsl_proc_cwmp_diagnostic10(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getADSLCwmpDiagnostic(buf,10);
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

int adsl_proc_cwmp_diagnostic11(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getADSLCwmpDiagnostic(buf,11);
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

int adsl_proc_cwmp_diagnostic12(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getADSLCwmpDiagnostic(buf,12);
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

int adsl_proc_cwmp_diagnostic13(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getADSLCwmpDiagnostic(buf,13);
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

int adsl_proc_cwmp_diagnostic14(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getADSLCwmpDiagnostic(buf,14);
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

int adsl_proc_cwmp_diagnostic15(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getADSLCwmpDiagnostic(buf,15);
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

int adsl_proc_cwmp_diagnostic16(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getADSLCwmpDiagnostic(buf,16);
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

#endif

#endif

#if defined(CWMP) || defined(TCSUPPORT_SNMP_ATMADSL)
/*add by renyu_20100128 begin */
int adsl_proc_stats_show(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getADSLStateShow(buf,1);
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
int adsl_proc_stats_last(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getADSLStateLast(buf,1);
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
int adsl_proc_stats_total(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getADSLStateTotal(buf,1);
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
int adsl_proc_stats_curdaySt(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getADSLStateCurrdayStart(buf,1);
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
int adsl_proc_stats_quthourSt(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getADSLStateQuthourStart(buf,1);
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
/*add by renyu_20100128 end */
#endif

#if defined(TCSUPPORT_SNMP_ATMADSL)
int adsl_proc_stats_prevdaySt(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getADSLStatePrevdayStart(buf,1);
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

int adsl_proc_stats_intervalSt(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getADSLStateIntervalStart(buf,1);
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
#endif

#if 0
static int adsl_cmd_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	int len;

	len = sprintf(page, "0\n");

	len -= off;
	*start = page + off;

	if (len > count)
		len = count;
	else
		*eof = 1;

	if (len < 0)
		len = 0;

	return len;
}

DEFINE_MUTEX(adsl_mutex);

static int adsl_cmd_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[80];

	mutex_lock(&adsl_mutex);
	if (count > sizeof(val_string) - 1) {
		mutex_unlock(&adsl_mutex);
		return -EINVAL;
	}

	if (copy_from_user(val_string, buffer, count)) {
		mutex_unlock(&adsl_mutex);
		return -EFAULT;
	}

	val_string[count] = '\0';
	//	cmdparse(Cmds, val_string, NULL);
	cmdparse(val_string, NULL);

	mutex_unlock(&adsl_mutex);

	return count;
}
#endif
#ifdef CMD_API
int tcsupport_adsl_info(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = strlen(adslinfobuf);
	buf = adslinfobuf;
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
#endif

#if defined(TCSUPPORT_CWMP_VDSL)
//xmdai_20110825
typedef struct
{
	uint8 CurrentProfiles;
	uint16 UPBOKLE;
	uint8 TRELLISds;
	uint8 TRELLISus;
	uint8 ACTSNRMODEds;
	uint8 ACTSNRMODEus;
	uint16 ACTUALCE;
	short SNRMpbds[5];
	short SNRMpbus[5];
}TR098_VDSL_INTERFACE_CFG; 

int
getVDSLInterfaceCfg(char *vdslCfg)
{
	uint16	index=0;
	TR098_VDSL_INTERFACE_CFG tr98VdslInterfaceCfg;

	adsl_dev_ops->query(VDSL_QUERY_TR098_DSL_INTERFACE_CONFIG, &tr98VdslInterfaceCfg, NULL);
	index += sprintf( vdslCfg+index, "CurrentProfiles: ");
	switch(tr98VdslInterfaceCfg.CurrentProfiles) {
		case VDSL2_PROFILE_8A:
	    		index += sprintf( vdslCfg+index, "8a\n");	
			break;
		case VDSL2_PROFILE_8B:
	   		index += sprintf( vdslCfg+index, "8b\n");
			break;
		case VDSL2_PROFILE_8C:
       			index += sprintf( vdslCfg+index, "8c\n");
			break;
		case VDSL2_PROFILE_8D:
       			index += sprintf( vdslCfg+index, "8d\n");	
			break;
		case VDSL2_PROFILE_12A:
       			index += sprintf( vdslCfg+index, "12a\n");	
			break;
		case VDSL2_PROFILE_12B:
       			index += sprintf( vdslCfg+index, "12b\n");	
			break;
		case VDSL2_PROFILE_17A:
       			index += sprintf( vdslCfg+index, "17a\n");	
			break;
		case VDSL2_PROFILE_30A:
       			index += sprintf( vdslCfg+index, "30a\n");	
			break;
		default:
			index += sprintf( vdslCfg+index, "\n");	
			break;
			
	}
	//index += sprintf( vdslCfg+index, "CurrentProfiles: %d\n", tr98VdslInterfaceCfg.CurrentProfiles);
	index += sprintf( vdslCfg+index, "UPBOKLE: %d.%d\n", tr98VdslInterfaceCfg.UPBOKLE/10,tr98VdslInterfaceCfg.UPBOKLE%10);
	index += sprintf( vdslCfg+index, "TRELLISds: %d\n", tr98VdslInterfaceCfg.TRELLISds);
	index += sprintf( vdslCfg+index, "TRELLISus: %d\n", tr98VdslInterfaceCfg.TRELLISus);
	index += sprintf( vdslCfg+index, "ACTSNRMODEds: %d\n", tr98VdslInterfaceCfg.ACTSNRMODEds);
	index += sprintf( vdslCfg+index, "ACTSNRMODEus: %d\n", tr98VdslInterfaceCfg.ACTSNRMODEus);
	index += sprintf( vdslCfg+index, "ACTUALCE: %d\n", tr98VdslInterfaceCfg.ACTUALCE);
	index += sprintf( vdslCfg+index, "SNRMpbds: %d,%d,%d,%d,%d\n", tr98VdslInterfaceCfg.SNRMpbds[0],tr98VdslInterfaceCfg.SNRMpbds[1],\
		tr98VdslInterfaceCfg.SNRMpbds[2],tr98VdslInterfaceCfg.SNRMpbds[3],tr98VdslInterfaceCfg.SNRMpbds[4]);
	index += sprintf( vdslCfg+index, "SNRMpbus: %d,%d,%d,%d,%d\n", tr98VdslInterfaceCfg.SNRMpbus[0],tr98VdslInterfaceCfg.SNRMpbus[1],\
		tr98VdslInterfaceCfg.SNRMpbus[2],tr98VdslInterfaceCfg.SNRMpbus[3],tr98VdslInterfaceCfg.SNRMpbus[4]);

	return index;
}

int vdsl_proc_interfaceCfg(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getVDSLInterfaceCfg(buf);
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
#else
#if defined(TCSUPPORT_CT_WAN_PTM)
//xmdai_20110825
#if !defined(TCSUPPORT_AUTOBENCH_AFE) && !defined(TCSUPPORT_AUTOBENCH_MT7510)
typedef struct
{
	uint8 CurrentProfiles;
	uint16 UPBOKLE;
	uint8 TRELLISds;
	uint8 TRELLISus;
	uint8 ACTSNRMODEds;
	uint8 ACTSNRMODEus;
	uint16 ACTUALCE;
	short SNRMpbds[5];
	short SNRMpbus[5];
}TR098_VDSL_INTERFACE_CFG; 
#endif

int
getVDSLInterfaceCfg(char *vdslCfg)
{
	uint16	index=0;
	TR098_VDSL_INTERFACE_CFG tr98VdslInterfaceCfg;

	adsl_dev_ops->query(VDSL_QUERY_TR098_DSL_INTERFACE_CONFIG, &tr98VdslInterfaceCfg, NULL);
	index += sprintf( vdslCfg+index, "CurrentProfiles: ");
	switch(tr98VdslInterfaceCfg.CurrentProfiles) {
		case VDSL2_PROFILE_8A:
	    		index += sprintf( vdslCfg+index, "8a\n");	
			break;
		case VDSL2_PROFILE_8B:
	   		index += sprintf( vdslCfg+index, "8b\n");
			break;
		case VDSL2_PROFILE_8C:
       			index += sprintf( vdslCfg+index, "8c\n");
			break;
		case VDSL2_PROFILE_8D:
       			index += sprintf( vdslCfg+index, "8d\n");	
			break;
		case VDSL2_PROFILE_12A:
       			index += sprintf( vdslCfg+index, "12a\n");	
			break;
		case VDSL2_PROFILE_12B:
       			index += sprintf( vdslCfg+index, "12b\n");	
			break;
		case VDSL2_PROFILE_17A:
       			index += sprintf( vdslCfg+index, "17a\n");	
			break;
		case VDSL2_PROFILE_30A:
       			index += sprintf( vdslCfg+index, "30a\n");	
			break;
		default:
			index += sprintf( vdslCfg+index, "\n");	
			break;
			
	}
	//index += sprintf( vdslCfg+index, "CurrentProfiles: %d\n", tr98VdslInterfaceCfg.CurrentProfiles);
	index += sprintf( vdslCfg+index, "UPBOKLE: %d.%d\n", tr98VdslInterfaceCfg.UPBOKLE/10,tr98VdslInterfaceCfg.UPBOKLE%10);
	index += sprintf( vdslCfg+index, "TRELLISds: %d\n", tr98VdslInterfaceCfg.TRELLISds);
	index += sprintf( vdslCfg+index, "TRELLISus: %d\n", tr98VdslInterfaceCfg.TRELLISus);
	index += sprintf( vdslCfg+index, "ACTSNRMODEds: %d\n", tr98VdslInterfaceCfg.ACTSNRMODEds);
	index += sprintf( vdslCfg+index, "ACTSNRMODEus: %d\n", tr98VdslInterfaceCfg.ACTSNRMODEus);
	index += sprintf( vdslCfg+index, "ACTUALCE: %d\n", tr98VdslInterfaceCfg.ACTUALCE);
	index += sprintf( vdslCfg+index, "SNRMpbds: %d,%d,%d,%d,%d\n", tr98VdslInterfaceCfg.SNRMpbds[0],tr98VdslInterfaceCfg.SNRMpbds[1],\
		tr98VdslInterfaceCfg.SNRMpbds[2],tr98VdslInterfaceCfg.SNRMpbds[3],tr98VdslInterfaceCfg.SNRMpbds[4]);
	index += sprintf( vdslCfg+index, "SNRMpbus: %d,%d,%d,%d,%d\n", tr98VdslInterfaceCfg.SNRMpbus[0],tr98VdslInterfaceCfg.SNRMpbus[1],\
		tr98VdslInterfaceCfg.SNRMpbus[2],tr98VdslInterfaceCfg.SNRMpbus[3],tr98VdslInterfaceCfg.SNRMpbus[4]);

	return index;
}

int vdsl_proc_interfaceCfg(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getVDSLInterfaceCfg(buf);
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
#endif
#endif

int tcadslcmd_init(void)
{
	cmds_t wan_cmd;
#ifdef CMD_API
	cmds_t adslcmd;
#endif	

	struct proc_dir_entry *cmd_proc;
	
	/* start adsl timer */
	init_timer(&adsl_timer);
	adsl_timer.expires = jiffies + (HZ * 2);
	adsl_timer.function = adslTimer;
	adsl_timer.data = 0x0;
	add_timer(&adsl_timer);

#if defined(TCSUPPORT_CPU_MT7510) || defined(TCSUPPORT_CPU_MT7505) || defined(TCSUPPORT_FTTDP_V2)
    if (isFPGA)
    {
        cmd_proc = create_proc_entry("tc3162/fpga_adsl_stats", 0, NULL);
        cmd_proc->write_proc    =   fpga_adsl_proc_stats;


        cmd_proc = create_proc_entry("tc3162/fpga_adsl_up_rate", 0, NULL);
        cmd_proc->write_proc    =   fpga_adsl_up_rate_proc_stats;

    }
#endif
	/* adsl related stats */
#ifdef TCSUPPORT_CPU_EN7512
         create_proc_read_entry("tc3162/xdsl_stats", 0, NULL, xdsl_proc_stats, NULL);
#endif
	create_proc_read_entry("tc3162/adsl_stats", 0, NULL, adsl_proc_stats, NULL);
#if defined(TCSUPPORT_CPU_MT7510) && defined(TCSUPPORT_BONDING)
    create_proc_read_entry("tc3162/adsl_stats_slave", 0, NULL, adsl_proc_stats_slave, NULL);
#endif
	/* adsl performance related stats */
	create_proc_read_entry("tc3162/adsl_perf", 0, NULL, adsl_proc_perf, NULL);
	/* adsl firmware version */
	create_proc_read_entry("tc3162/adsl_fwver", 0, NULL, adsl_proc_fwver, NULL);

	/*tr069 dsl diagnostic====add by brian*/
#ifdef CWMP
	create_proc_read_entry("tc3162/adsl_cwmp_diagnostic0", 0, NULL, adsl_proc_cwmp_diagnostic0, NULL);
	create_proc_read_entry("tc3162/adsl_cwmp_diagnostic1", 0, NULL, adsl_proc_cwmp_diagnostic1, NULL);
	create_proc_read_entry("tc3162/adsl_cwmp_diagnostic2", 0, NULL, adsl_proc_cwmp_diagnostic2, NULL);
	create_proc_read_entry("tc3162/adsl_cwmp_diagnostic3", 0, NULL, adsl_proc_cwmp_diagnostic3, NULL);
	create_proc_read_entry("tc3162/adsl_cwmp_diagnostic4", 0, NULL, adsl_proc_cwmp_diagnostic4, NULL);
	create_proc_read_entry("tc3162/adsl_cwmp_diagnostic5", 0, NULL, adsl_proc_cwmp_diagnostic5, NULL);
	create_proc_read_entry("tc3162/adsl_cwmp_diagnostic6", 0, NULL, adsl_proc_cwmp_diagnostic6, NULL);
#if defined(TCSUPPORT_CT_WAN_PTM)
	create_proc_read_entry("tc3162/adsl_cwmp_diagnostic7", 0, NULL, adsl_proc_cwmp_diagnostic7, NULL);
	create_proc_read_entry("tc3162/adsl_cwmp_diagnostic8", 0, NULL, adsl_proc_cwmp_diagnostic8, NULL);
	create_proc_read_entry("tc3162/adsl_cwmp_diagnostic9", 0, NULL, adsl_proc_cwmp_diagnostic9, NULL);
	create_proc_read_entry("tc3162/adsl_cwmp_diagnostic10", 0, NULL, adsl_proc_cwmp_diagnostic10, NULL);
	create_proc_read_entry("tc3162/adsl_cwmp_diagnostic11", 0, NULL, adsl_proc_cwmp_diagnostic11, NULL);
	create_proc_read_entry("tc3162/adsl_cwmp_diagnostic12", 0, NULL, adsl_proc_cwmp_diagnostic12, NULL);
	create_proc_read_entry("tc3162/adsl_cwmp_diagnostic13", 0, NULL, adsl_proc_cwmp_diagnostic13, NULL);
	create_proc_read_entry("tc3162/adsl_cwmp_diagnostic14", 0, NULL, adsl_proc_cwmp_diagnostic14, NULL);
	create_proc_read_entry("tc3162/adsl_cwmp_diagnostic15", 0, NULL, adsl_proc_cwmp_diagnostic15, NULL);
	create_proc_read_entry("tc3162/adsl_cwmp_diagnostic16", 0, NULL, adsl_proc_cwmp_diagnostic16, NULL);
#endif
#endif
#if defined(CWMP) || defined(TCSUPPORT_SNMP_ATMADSL)
	create_proc_read_entry("tc3162/adsl_stas_show", 0, NULL, adsl_proc_stats_show, NULL);
	create_proc_read_entry("tc3162/adsl_stas_last", 0, NULL, adsl_proc_stats_last, NULL);
	create_proc_read_entry("tc3162/adsl_stas_total", 0, NULL, adsl_proc_stats_total, NULL);
	create_proc_read_entry("tc3162/adsl_stas_curdaySt", 0, NULL, adsl_proc_stats_curdaySt, NULL);
	create_proc_read_entry("tc3162/adsl_stas_quthourSt", 0, NULL, adsl_proc_stats_quthourSt, NULL);
#endif	
#if defined(TCSUPPORT_SNMP_ATMADSL)
	/*Add for snmp adsl mib*/
	create_proc_read_entry("tc3162/adsl_stas_prevdaySt", 0, NULL, adsl_proc_stats_prevdaySt, NULL);
	create_proc_read_entry("tc3162/adsl_stas_intervaldaySt", 0, NULL, adsl_proc_stats_intervalSt, NULL);
#endif
#ifdef TCSUPPORT_AUTOBENCH
	/*Prio Enable Switch. shnwind 20110418.*/
	cmd_proc = create_proc_entry("tc3162/adsl_set_bench", 0, NULL);
	cmd_proc->read_proc 	= 	adsl_bench_read_proc;
	cmd_proc->write_proc 	= 	adsl_bench_write_proc;
#endif
#if defined(TCSUPPORT_AUTOBENCH_AFE) || defined(TCSUPPORT_AUTOBENCH_MT7510) || defined(TCSUPPORT_AUTOBENCH_MT7505)
    cmd_proc = create_proc_entry("tc3162/vdsl_set_bench", 0, NULL);
    cmd_proc->read_proc     =   vdsl_bench_read_proc;
    cmd_proc->write_proc    =   vdsl_bench_write_proc;
    cmd_proc = create_proc_entry("tc3162/vdsl_set_period", 0, NULL);
    cmd_proc->read_proc     =   vdsl_period_read_proc;
    cmd_proc->write_proc    =   vdsl_period_write_proc;
#endif	
#ifdef CMD_API
	create_proc_read_entry("tc3162/tcsupport_adslinfo", 0, NULL, tcsupport_adsl_info, NULL);
#endif
#if defined(TCSUPPORT_CWMP_VDSL)
	create_proc_read_entry("tc3162/vdsl_interface_config", 0, NULL, vdsl_proc_interfaceCfg, NULL);
#else
#if defined(TCSUPPORT_CT_WAN_PTM)
	create_proc_read_entry("tc3162/vdsl_interface_config", 0, NULL, vdsl_proc_interfaceCfg, NULL);
#endif
#endif
	/*Init wan  root ci-cmd*/
	wan_cmd.name= "wan";
	wan_cmd.func=doWan;
	wan_cmd.flags=0x12;
	wan_cmd.argcmin=0;
	wan_cmd.argc_errmsg=NULL;

#ifdef CMD_API
	adslcmd.name= "adslphxcmd";
	adslcmd.func=doAdslCmd;
	adslcmd.flags=0x12;
	adslcmd.argcmin=0;
	adslcmd.argc_errmsg=NULL;
#endif	

	/*Register wan & sys ci-cmd*/
	cmd_register(&wan_cmd);
#ifdef CMD_API
	cmd_register(&adslcmd);
#endif	
#if defined(TCSUPPORT_SNMP_ATMADSL)
	initSnmpADSLAturChanPrevDayInfo();
	initSnmpADSLAturChanCurrDayInfo();
	initSnmpADSLAturChanQutHourInfo();
	initSnmpADSLAturChanIntervalInfo();
	initSnmpADSLAturChanTotalInfo();
	
	initSnmpADSLAR5211TotalInfo();
#endif
	return 0;
}

//static void __exit tcadslcmd_exit(void)
void tcadslcmd_exit(void)
{	
    	del_timer(&adsl_timer);
#if defined(TCSUPPORT_CPU_MT7510) || defined(TCSUPPORT_CPU_MT7505) || defined(TCSUPPORT_FTTDP_V2)
    if (isFPGA){
        remove_proc_entry("tc3162/fpga_adsl_stats", NULL);
        remove_proc_entry("tc3162/fpga_adsl_up_rate", NULL);
	}
#if defined(TCSUPPORT_BONDING)
        remove_proc_entry("tc3162/adsl_stats_slave", NULL);
#endif
#endif
#ifdef TCSUPPORT_CPU_EN7512
         remove_proc_entry("tc3162/xdsl_stats", NULL);
#endif
	remove_proc_entry("tc3162/adsl_stats", NULL);
	remove_proc_entry("tc3162/adsl_perf", NULL);
	remove_proc_entry("tc3162/adsl_fwver", NULL);
	//add by brian for dsl diagnostic
#ifdef CWMP
	remove_proc_entry("tc3162/adsl_cwmp_diagnostic0", NULL);
	remove_proc_entry("tc3162/adsl_cwmp_diagnostic1", NULL);
	remove_proc_entry("tc3162/adsl_cwmp_diagnostic2", NULL);
	remove_proc_entry("tc3162/adsl_cwmp_diagnostic3", NULL);
	remove_proc_entry("tc3162/adsl_cwmp_diagnostic4", NULL);
	remove_proc_entry("tc3162/adsl_cwmp_diagnostic5", NULL);
	remove_proc_entry("tc3162/adsl_cwmp_diagnostic6", NULL);
#if defined(TCSUPPORT_CT_WAN_PTM)
	remove_proc_entry("tc3162/adsl_cwmp_diagnostic7", NULL);
	remove_proc_entry("tc3162/adsl_cwmp_diagnostic8", NULL);
	remove_proc_entry("tc3162/adsl_cwmp_diagnostic9", NULL);
	remove_proc_entry("tc3162/adsl_cwmp_diagnostic10", NULL);
	remove_proc_entry("tc3162/adsl_cwmp_diagnostic11", NULL);
	remove_proc_entry("tc3162/adsl_cwmp_diagnostic12", NULL);
	remove_proc_entry("tc3162/adsl_cwmp_diagnostic13", NULL);
	remove_proc_entry("tc3162/adsl_cwmp_diagnostic14", NULL);
	remove_proc_entry("tc3162/adsl_cwmp_diagnostic15", NULL);
	remove_proc_entry("tc3162/adsl_cwmp_diagnostic16", NULL);
#endif
#endif
#ifdef TCSUPPORT_AUTOBENCH
	remove_proc_entry("tc3162/adsl_set_bench", NULL);
#endif
#if defined(CWMP) || defined(TCSUPPORT_SNMP_ATMADSL)
	//renyu_20100128
	remove_proc_entry("tc3162/adsl_stas_show", NULL);
	remove_proc_entry("tc3162/adsl_stas_last", NULL);
	remove_proc_entry("tc3162/adsl_stas_total", NULL);
	remove_proc_entry("tc3162/adsl_stas_curdaySt", NULL);
	remove_proc_entry("tc3162/adsl_stas_quthourSt", NULL);
#endif	
#if defined(TCSUPPORT_SNMP_ATMADSL)
	remove_proc_entry("tc3162/adsl_stas_prevdaySt", NULL);
	remove_proc_entry("tc3162/adsl_stas_intervaldaySt", NULL);
#endif
#ifdef CMD_API
	remove_proc_entry("tc3162/tcsupport_adslinfo", NULL);
#endif
#if defined(TCSUPPORT_CWMP_VDSL)
	remove_proc_entry("tc3162/vdsl_interface_config", NULL);
#else
#if defined(TCSUPPORT_CT_WAN_PTM)
	remove_proc_entry("tc3162/vdsl_interface_config", NULL);
#endif
#endif
}

EXPORT_SYMBOL(getXdslSpeed);
EXPORT_SYMBOL(getXdslModeType);

//module_init (tcadslcmd_init);
//module_exit (tcadslcmd_exit);

#else

int
getXdslSpeed(void)
{
	return 0;
}

EXPORT_SYMBOL(getXdslSpeed);

#endif

//extern adsldev_ops *trey_ops;

void adsl_dev_ops_register(adsldev_ops *ops)
{
	adsl_dev_ops = ops;

//	trey_ops = ops;

#ifdef CONFIG_TC3162_ADSL
	wanCmds[1].name = "rts";  	wanCmds[1].func = ops->rts_cmd;
	wanCmds[2].name = "dmt";  	wanCmds[2].func = ops->dmt_cmd;
	wanCmds[3].name = "dmt2";  	wanCmds[3].func = ops->dmt2_cmd;
	wanCmds[4].name = "hwdmt"; 	wanCmds[4].func = ops->hw_cmd;
	wanCmds[5].name = "swdmt"; 	wanCmds[5].func = ops->sw_cmd;
	wanCmds[6].name = "ghs";  	wanCmds[6].func = ops->ghs_cmd;
	wanCmds[7].name = "tcif";  	wanCmds[7].func = ops->tcif_cmd;
#endif
}

void adsl_dev_ops_deregister(void)
{
	adsl_dev_ops = NULL;

#ifdef CONFIG_TC3162_ADSL
	wanCmds[1].name = NULL;  	wanCmds[1].func = NULL;
	wanCmds[2].name = NULL;  	wanCmds[2].func = NULL;
	wanCmds[3].name = NULL;  	wanCmds[3].func = NULL;
	wanCmds[4].name = NULL; 	wanCmds[4].func = NULL;
	wanCmds[5].name = NULL; 	wanCmds[5].func = NULL;
	wanCmds[6].name = NULL;  	wanCmds[6].func = NULL;
	wanCmds[7].name = NULL;  	wanCmds[7].func = NULL;
#endif
}


#if defined(TCSUPPORT_CPU_MT7510) && defined(TCSUPPORT_BONDING)
void adsl_dev_ops_register_slave(adsldev_ops *ops)
{
	int slave_index = 2;
	adsl_dev_ops_slave = ops;

#if defined(CONFIG_TC3162_ADSL)
	slave_index += 7;

#if defined(CONFIG_RALINK_VDSL)
	slave_index += 1;
#endif
#endif

#ifdef CONFIG_TC3162_ADSL
	wanCmds[slave_index].name = "rts_slave";  		wanCmds[slave_index].func = ops->rts_cmd;
	wanCmds[slave_index+1].name = "dmt_slave";  	wanCmds[slave_index+1].func = ops->dmt_cmd;
	wanCmds[slave_index+2].name = "dmt2_slave";  	wanCmds[slave_index+2].func = ops->dmt2_cmd;
	wanCmds[slave_index+3].name = "hwdmt_slave"; 	wanCmds[slave_index+3].func = ops->hw_cmd;
	wanCmds[slave_index+4].name = "swdmt_slave"; 	wanCmds[slave_index+4].func = ops->sw_cmd;
	wanCmds[slave_index+5].name = "ghs_slave";  	wanCmds[slave_index+5].func = ops->ghs_cmd;
	wanCmds[slave_index+6].name = "tcif_slave";  	wanCmds[slave_index+6].func = ops->tcif_cmd;
#endif

}

void adsl_dev_ops_deregister_slave(void)
{
	int i;
	int slave_index = 2;
	adsl_dev_ops_slave = NULL;

#if defined(CONFIG_TC3162_ADSL)
	slave_index += 7;
#endif

#if defined(CONFIG_RALINK_VDSL)
	slave_index += 1;
#endif

#ifdef CONFIG_TC3162_ADSL
	wanCmds[slave_index].name = NULL;  		wanCmds[slave_index].func = NULL;
	wanCmds[slave_index+1].name = NULL;  	wanCmds[slave_index+1].func = NULL;
	wanCmds[slave_index+2].name = NULL;  	wanCmds[slave_index+2].func = NULL;
	wanCmds[slave_index+3].name = NULL; 	wanCmds[slave_index+3].func = NULL;
	wanCmds[slave_index+4].name = NULL; 	wanCmds[slave_index+4].func = NULL;
	wanCmds[slave_index+5].name = NULL;  	wanCmds[slave_index+5].func = NULL;
	wanCmds[slave_index+6].name = NULL;  	wanCmds[slave_index+6].func = NULL;
#endif

}
#endif

#ifdef CONFIG_RALINK_VDSL
void vdsl_dev_ops_register(vdsldev_ops *ops)
{
	vdsl_dev_ops = ops;

	wanCmds[8].name = "vdsl2"; 	wanCmds[8].func = ops->vdsl2_cmd;
}

void vdsl_dev_ops_deregister(void)
{
	vdsl_dev_ops = NULL;

	wanCmds[8].name = NULL;  	wanCmds[8].func = NULL;
}

EXPORT_SYMBOL(vdsl_dev_ops_register);
EXPORT_SYMBOL(vdsl_dev_ops_deregister);
#if defined(TCSUPPORT_CPU_MT7510) && defined(TCSUPPORT_BONDING)
void vdsl_dev_ops_register_slave(vdsldev_ops *ops)
{
	vdsl_dev_ops_slave = ops;
	int slave_index = 2;
#if defined(CONFIG_TC3162_ADSL)
	slave_index += 7;
#endif

#if defined(CONFIG_RALINK_VDSL)
	slave_index += 1;
#endif

	wanCmds[slave_index+7].name = "vdsl2_slave"; 	wanCmds[slave_index+7].func = ops->vdsl2_cmd;
	return;
}

void vdsl_dev_ops_deregister_slave(void)
{
	vdsl_dev_ops = NULL;
	int slave_index = 2;
#if defined(CONFIG_TC3162_ADSL)
	slave_index += 7;
#endif

#if defined(CONFIG_RALINK_VDSL)
	slave_index += 1;
#endif

	wanCmds[slave_index+7].name = NULL;  	wanCmds[slave_index+7].func = NULL;
	return;
}

EXPORT_SYMBOL(vdsl_dev_ops_register_slave);
EXPORT_SYMBOL(vdsl_dev_ops_deregister_slave);
#endif
#endif


#ifdef CMD_API
EXPORT_SYMBOL(adslinfobuf);
#endif

#if defined(TCSUPPORT_CPU_MT7510) && defined(TCSUPPORT_BONDING)
EXPORT_SYMBOL(adsl_dev_ops_register_slave);
EXPORT_SYMBOL(adsl_dev_ops_deregister_slave);
#endif

EXPORT_SYMBOL(adsl_dev_ops_register);
EXPORT_SYMBOL(adsl_dev_ops_deregister);

#ifdef TCSUPPORT_AUTOBENCH

static int adsl_bench_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data){
	int i;
	char val_string[32];
	//unsigned int tmp[4];
				    
	if (count > sizeof(val_string) - 1)
		return -EINVAL;
									        
	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;
				        
	val_string[count] = '\0';
													    
	if (sscanf(val_string, "%u %u %u", &adsl_rate[0], &adsl_rate[1], &adsl_rate[2]) < 3) {
		printk("usage: <DS_H> <DS_L> <US_L> \n");
	    return count;
	} 
	//printk("<DS_H> %u <DS_L> %u <US_L> %u \n",adsl_rate[0], adsl_rate[1], adsl_rate[2]);
	return count;

}
static int adsl_bench_read_proc(char *buf, char **start, off_t off, int count,
	int *eof, void *data){
	
	int len;

	len = sprintf( buf, "<DS_H> %u <DS_L> %u <US_L> %u \n",adsl_rate[0], adsl_rate[1], adsl_rate[2]);

	len -= off;
	*start = buf + off;

	if (len > count)
		len = count;
	else
	    *eof = 1;

	if (len < 0)
		len = 0;

	return len;
}

int query_adsl_link_rate(int *down_rate, int *up_rate){
	uint8 modemst;
	T_adslChannelOperData adslChannelOperData;

	if (adsl_dev_ops == NULL)
		return -1;

	adsl_dev_ops->query(ADSL_QUERY_STATUS, &modemst, NULL );
	if (modemst != ADSL_MODEM_STATE_UP)
		return -1;

	adsl_dev_ops->query(ADSL_QUERY_CH_OP_DATA, &adslChannelOperData, NULL);
	
	if(adslChannelOperData.actualBitrateNearInt != 0){
		*down_rate = adslChannelOperData.actualBitrateNearInt;
		*up_rate = adslChannelOperData.actualBitrateFarInt;
	}else{
		*down_rate = adslChannelOperData.actualBitrateNearFast;
		*up_rate = adslChannelOperData.actualBitrateFarFast;
	}
		
	return 0;
}
EXPORT_SYMBOL(query_adsl_link_rate);

int query_adsl_link_snr(int *near_end_snr, int *far_end_snr){
	uint8 modemst;
	T_NTadslFarEndLineOperData adslFarEndLineOperData;
        T_NTadslNearEndLineOperData adslNearEndLineOperData;
#if 0
	if (adsl_dev_ops == NULL)
                return -1;

        adsl_dev_ops->query(ADSL_QUERY_STATUS, &modemst, NULL );
        if (modemst != ADSL_MODEM_STATE_UP)
                return -1;
#endif
	adsl_dev_ops->query(ADSL_QUERY_FAR_OP_DATA, &adslFarEndLineOperData, NULL);
        adsl_dev_ops->query(ADSL_QUERY_NEAR_OP_DATA, &adslNearEndLineOperData, NULL);

	*far_end_snr = adslFarEndLineOperData.noiseMarginUpstr;
	*near_end_snr = adslNearEndLineOperData.noiseMarginDnstr;

        return 0;
}
EXPORT_SYMBOL(query_adsl_link_snr);
int do_3095_loopback(void){
	char val_string[256] = {"w hwdmt hwtest vdsl2_lpbk 0 0 4 0 0 8 5 1 1 0 0"};

	cmdparse(NULL, val_string, NULL);
	return 0;
}
EXPORT_SYMBOL(do_3095_loopback);

int do_rt63365_dmt_loopback(void){
	uint8 AfeDmtLpbk;

	if (adsl_dev_ops == NULL)
		return -1;

	adsl_dev_ops->query(ADSL_QUERY_AFE_DMT_LPBK, &AfeDmtLpbk, NULL );

	if (AfeDmtLpbk==0)
		return 0;
	else 
		return -1;

}
EXPORT_SYMBOL(do_rt63365_dmt_loopback);

#endif

#if defined(TCSUPPORT_AUTOBENCH_AFE) || defined(TCSUPPORT_AUTOBENCH_MT7510) || defined(TCSUPPORT_AUTOBENCH_MT7505) || defined(TCSUPPORT_AUTOBENCH_MAIN_CHIP)
//get vdsl link profile
uint8 get_vdsl_link_profile(void)
{
    TR098_VDSL_INTERFACE_CFG vdsl_interface_cfg;
    
    os_TCIfQuery(VDSL_QUERY_TR098_DSL_INTERFACE_CONFIG, &vdsl_interface_cfg, NULL);
    
    return vdsl_interface_cfg.CurrentProfiles;
}
EXPORT_SYMBOL(get_vdsl_link_profile);

//reset xdsl link
void reset_xdsl_link(void)
{
    adslReset(adsl_dev_ops);
}
EXPORT_SYMBOL(reset_xdsl_link);

//R/W test adsl 
char test_xdsl_rw(void)
{
	unsigned char ret=0;
	if(adsl_dev_ops ==NULL)
		return 0; //fail
 	adsl_dev_ops->query(ADSL_QUERY_AFE_R_W, &ret, NULL); 
	return ret;
}
EXPORT_SYMBOL(test_xdsl_rw);

//turn down xdsl link
int down_xdsl_link(void)
{
    return doAdslClose(0, NULL, "adsl");
}
EXPORT_SYMBOL(down_xdsl_link);

static int vdsl_bench_write_proc(struct file *file, const char *buffer, unsigned long count, void *data)
{
    int i;
    char val_string[32];
                    
    if (count > sizeof(val_string) - 1)
        return -EINVAL;
                                            
    if (copy_from_user(val_string, buffer, count))
        return -EFAULT;
                        
    val_string[count] = '\0';
                                                        
    if (sscanf(val_string, "%u %u %u %u", &vdsl_rate[0], &vdsl_rate[1], &vdsl_rate[2], &vdsl_rate[3]) < 4) {
        printk("usage: <DS-17a> <US-17a> <DS-30a> <US-30a> \n");
        return count;
    } 

    return count;
}

static int vdsl_bench_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data)
{ 
    int len;

    len = sprintf( buf, "<DS-17a> %u <US-17a> %u <DS-30a> %u <US-30a> %u\n", vdsl_rate[0], vdsl_rate[1], vdsl_rate[2], vdsl_rate[3]);

    len -= off;
    *start = buf + off;

    if (len > count)
        len = count;
    else
        *eof = 1;

    if (len < 0)
        len = 0;

    return len;
}
static int vdsl_period_write_proc(struct file *file, const char *buffer, unsigned long count, void *data)
{
    int i;
    char val_string[32];
                    
    if (count > sizeof(val_string) - 1)
        return -EINVAL;
                                            
    if (copy_from_user(val_string, buffer, count))
        return -EFAULT;
                        
    val_string[count] = '\0';
                                                        
    if (sscanf(val_string, "%u", &vdsl_period) < 1) {
        printk("usage: <period> seconds\n");
        return count;
    } 

    return count;
}

static int vdsl_period_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data)
{ 
    int len;

    len = sprintf( buf, "%u seconds\n", vdsl_period);

    len -= off;
    *start = buf + off;

    if (len > count)
        len = count;
    else
        *eof = 1;

    if (len < 0)
        len = 0;

    return len;
}
#endif
