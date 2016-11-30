/************************************************************************
 *
 *	Copyright (C) 2009 Trendchip Technologies, Corp.
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
#include <stdio.h>
#include <stdlib.h>
#include <linux/mii.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

#ifdef ETHCMD_DBG
#include <sys/stat.h>
#include <unistd.h>
#endif

#include "libcmd.h"
#include "libmii.h"
#include "ethercmd.h"
#include "ether.h"

#ifdef TCSUPPORT_MT7530_SWITCH_API
#include <modules/eth_global_def.h>
#include <errno.h>
#endif

#ifndef TCSUPPORT_3_18_21_KERNEL
#ifdef TCSUPPORT_2_6_36_KERNEL
#include "../../../linux-2.6.36/arch/mips/include/asm/tc3162/tc3162.h"
#else
#include "../../../linux/include/asm-mips/tc3162/tc3162.h"
#endif
#endif

#define CR_AHB_BASE       	0xBFB00000
#define CR_RSTCTRL2    		(CR_AHB_BASE + 0x834)
#define EPHY_RST    					(1<<24)
#define ESW_RST    						(1<<23)
#define FE_RST    						(1<<21)
#define PATTERN_NUM						4

static int doEth(int argc, char *argv[], void *p);
static int doAddwan(int argc, char *argv[], void *p);
static int doDelwan(int argc, char *argv[], void *p);
static int doPhyMediaType(int argc, char *argv[], void *p);
static int doPhyReset(int argc, char *argv[], void *p);
static int doVlanpt(int argc, char *argv[], void *p);
static int doLanchip(int argc, char *argv[], void *p);
static int doStatus(int argc, char *argv[], void *p);
static int doIgmpsnoop(int argc, char *argv[], void *p);
static int doHelp(int argc, char *argv[], void *p);
static int doVersion(int argc, char *argv[], void *p);
#ifdef ETHCMD_DBG
static int doEthcmdDbg(int argc, char *argv[], void *p);
void initEthcmdDbg(void);
#endif
static int doTCMiiRead(int argc, char *argv[], void *p);
static int doTCMiiWrite(int argc, char *argv[], void *p);
#if defined(TCSUPPORT_CPU_RT65168)
static int doTCMiiBPRead(int argc, char *argv[], void *p);
static int doTCMiiBPWrite(int argc, char *argv[], void *p);
#endif
static int doTCMiiRead2(int argc, char *argv[], void *p);  //MTK120625 ///YM
static int doTCMiiWrite2(int argc, char *argv[], void *p);  //MTK120625 ///YM
static int doGSWRead(int argc, char *argv[], void *p);
static int doGSWWrite(int argc, char *argv[], void *p);
static int doGSWRead2(int argc, char *argv[], void *p); //MTK120625 ///YM
static int doGSWWrite2(int argc, char *argv[], void *p); //MTK120625 ///YM
static int doGSWPhyRead(int argc, char *argv[], void *p);
static int doGSWPhyWrite(int argc, char *argv[], void *p);
#ifdef MT7530_PHY_VERIFY
static int doTCMiiRead45(int argc, char *argv[], void *p);
static int doTCMiiWrite45(int argc, char *argv[], void *p);
#endif
/*VLAN related ci-cmd*/
static int doVlan(int argc, char *argv[], void *p);
static int doGSWRegTest(int argc, char *argv[], void *p);
static int doVlanSetActive(int argc, char *argv[], void *p);
static int doVlanSetPvid(int argc, char *argv[], void *p);
static int doVlanSetVid(int argc, char *argv[], void *p);
static int doVlanDisp(int argc, char *argv[], void *p);
/*end vlan related ci-cmd*/
#ifdef ETHCMD_EXTENSION
static int doPortBinding(int argc, char *argv[], void *p);
#endif

void cmdHelp(void);
static int ethup_addwan(char *szBaseDev,int iVid,char *szVDev);
static int setLanchip(char* type, int portid);
static int setVport(char* type);
static int getEtherPort(void);
static int ethcmdQueryPortCount(int portid);
static u16 ethcmdGetMediaOption(char *option);
static int showMediaSetting(int phy_id);
static findNegoPortID(void);

//#if defined(SQA_VERIFY)
static int doGen1588Pkt(int argc, char *argv[], void *p);	//MTK20120829_MT7530_1588pkt_generation
static int doSwitchRst(int argc, char *argv[], void *p);
static int doRateControl(int argc, char *argv[], void *p);
static int doVlanSetPortAttr(int argc, char *argv[], void *p);
static int doVlanSetPortMode(int argc, char *argv[], void *p);
static int doVlanSetEgressTagPVC(int argc, char *argv[], void *p);
static int doVlanSetEgressTagPCR(int argc, char *argv[], void *p);
static int doVlanSetAccFrm(int argc, char *argv[], void *p);
static int doAcl(int argc, char *argv[], void *p);
static int doAclSetPortEn(int argc, char *argv[], void *p);
static int doAclSetMultiHit(int argc, char *argv[], void *p);
static int doAclTblClr(int argc, char *argv[], void *p);
static int doAclTblDisp(int argc, char *argv[], void *p);
static int doAclAclTblAdd(int argc, char *argv[], void *p);
static int doAclMaskTblAdd(int argc, char *argv[], void *p);
static int doAclRuleTblAdd(int argc, char *argv[], void *p);
static int doAclRateTblAdd(int argc, char *argv[], void *p);
static int doArl(int argc, char *argv[], void *p);
static int doArlAging(int argc, char *argv[], void *p);
static int doArlChkLen(int argc, char *argv[], void *p);
static int doArlMACTblAdd(int argc, char *argv[], void *p);
static int doArlMACTblDisp(int argc, char *argv[], void *p);
static int doArlMACTblClr(int argc, char *argv[], void *p);
static int doArlDIPTblAdd(int argc, char *argv[], void *p);
static int doArlDIPTblDisp(int argc, char *argv[], void *p);
static int doArlDIPTblClr(int argc, char *argv[], void *p);
static int doArlSIPTblAdd(int argc, char *argv[], void *p);
static int doArlSIPTblDisp(int argc, char *argv[], void *p);
static int doArlSIPTblClr(int argc, char *argv[], void *p);
static int doArlAddrTblDisp(int ac_mat);
static int doArlAddrTblDisp2(u32 ext_switch, int ac_mat);
static int doArlAddrTblClr(int ac_mat);
static int doArlAddrTblClr2(u32 ext_switch, int ac_mat);
static int doMacControl(int argc, char *argv[], void *p);
static int doMacColLmt(int argc, char *argv[], void *p);
static int doMacFC(int argc, char *argv[], void *p);
static int doPortMirror(int argc, char *argv[], void *p);
static int doMirrorEn(int argc, char *argv[], void *p);
static int doMirrorPortBased(int argc, char *argv[], void *p);
static int doPhySettings(int argc, char *argv[], void *p);
static int doPhyANEn(int argc, char *argv[], void *p);
static int doPhyFCEn(int argc, char *argv[], void *p);
static int doStp(int argc, char *argv[], void *p);
static int doQos(int argc, char *argv[], void *p);
static int doQosType(int argc, char *argv[], void *p);
static int doQosBase(int argc, char *argv[], void *p);
static int doQosSetPrioQmap(int argc, char *argv[], void *p);
static int doQosSetPrioTagmap(int argc, char *argv[], void *p);
static int doQosSetPrioDSCPmap(int argc, char *argv[], void *p);
//static int doQosSetVpriQmap(int argc, char *argv[], void *p);
static int doQosSetPortPrio(int argc, char *argv[], void *p);
static int doQosSetPortWeight(int argc, char *argv[], void *p);
static int doQosSetDSCPPrio(int argc, char *argv[], void *p);
static int doMib(int argc, char *argv[], void *p);
static int doMibClr(int argc, char *argv[], void *p);
//#endif //SQA_VERIFY

#ifdef TCSUPPORT_MT7530_SWITCH_API
static int doP6Cal(int argc, char *argv[], void *p);
static int doDsTrtcmSetActive(int argc, char *argv[], void *p);
static int doDsTrtcmSetRule(int argc, char *argv[], void *p);
static int doDsTrtcmGetRule(int argc, char *argv[], void *p);
static int doDsTrtcm(int argc, char *argv[], void *p);
static int doDsP5Qos(int argc, char *argv[], void *p);
static int doDsP5QosSetActive(int argc, char *argv[], void *p);
static int doDsP5RateLimit(int argc, char *argv[], void *p);
static int doDsP5RlSetActive(int argc, char *argv[], void *p);
static int doDsP5RlSetValue(int argc, char *argv[], void *p);
static int doWPtest(int argc, char *argv[], void *p);

static int doPortMode(int argc, char *argv[], void *p);
static int doSetPortSpeed(int argc, char *argv[], void *p);
static int doGetPortSpeed(int argc, char *argv[], void *p);
static int doSetPortDuplex(int argc, char *argv[], void *p);
static int doGetPortDuplex(int argc, char *argv[], void *p);


static int doStormCtrl(int argc, char *argv[], void *p);
static int doStormCtrlUnit(int argc, char *argv[], void *p);
static int doStormCtrlType(int argc, char *argv[], void *p);
static int doStormCtrlMode(int argc, char *argv[], void *p);
static int doStormCtrlPeriod(int argc, char *argv[], void *p);
static int doStormCtrlPort(int argc, char *argv[], void *p);
static int doPortMatrix(int argc, char *argv[], void *p);
static int doTrafficSel(int argc, char *argv[], void *p);
static int doRgmiiSetting(int argc, char *argv[], void *p);
static int doTrafficSet(int argc, char *argv[], void *p);
static int doTrafficDisplay(int argc, char *argv[], void *p);
#endif

static int doDstq(int argc, char *argv[], void *p);

#ifdef JUDGE_SWITCH_SCENARIO_BY_751020_SUBMODEL
extern u32 mt751020_submodel_id;    // 0 means uninitialized/invalid
extern  u32 np_hw_id;
extern u32 efuse_extreme_mode;
#endif

extern u8 use_extern_switch;

extern swic_api_t swic;

extern int lan_portmap[6];

cmds_t Cmds[] = {
	{LAN_IF,		doEth,			0x12,	0,	NULL},
	{"addwan",		doAddwan,		0x12,	3,	"addwan vid devname vlan-priority"},
	{"delwan",		doDelwan,		0x12,	1,	"delwan devname"},
	{"miir",		doTCMiiRead,		0x12,	2,	"miir <phyAddr> <reg>"},
	{"miiw",		doTCMiiWrite,		0x12,	3,	"miiw <phyAddr> <reg> <value>"},
#if defined(TCSUPPORT_CPU_RT65168)
	{"miibpr",		doTCMiiBPRead,		0x12,	2,	"miibpr <phyAddr> <reg>"},
	{"miibpw",		doTCMiiBPWrite,		0x12,	3,	"miibpw <phyAddr> <reg> <value>"},
#endif
	{"gswr",		doGSWRead,		0x12,	1,	"gswr <reg>"},
	{"gsww",		doGSWWrite,		0x12,	2,	"gsww <reg> <value>"},
	{"miir2",		doTCMiiRead2,		0x12,	3,	"miir2 <ext_switch:0|1> <phyAddr> <reg>"},
	{"miiw2",		doTCMiiWrite2,		0x12,	4,	"miiw2 <ext_switch:0|1> <phyAddr> <reg> <value>"},
	{"gswr2",		doGSWRead2,		0x12,	2,	"gswr2 <ext_switch:0|1> <reg>"},
	{"gsww2",		doGSWWrite2,		0x12,	3,	"gsww2 <ext_switch:0|1> <reg> <value>"},

//	#if defined(SQA_VERIFY)
	{"vlan",		doVlan,			0x12,	0,	NULL},
	{"acl",			doAcl,			0x12,	0,	NULL},
	{"qos", 		doQos,			0x12,	0,	NULL},
	{"arl",			doArl,			0x12,	0,	NULL},
	{"reset",		doSwitchRst,		0x12,	0,	HELP_SWITCH_RESET},
	{"ratectl",		doRateControl,		0x12,	3,	HELP_RATE_CONTROLL},
	{"macctl",		doMacControl,		0x12,	0,	NULL},
	{"portmirror",		doPortMirror,		0x12,	0,	NULL},
	{"stp",			doStp,			0x12,	3,	HELP_STP},
	{"phyctl",		doPhySettings,		0x12,	0,	NULL},
	{"mib",			doMib,			0x12,	0,	NULL},
	{"gen_pkt",		doGen1588Pkt,		0x12,	1,	"gen ieee 1588 protocol packet: gen_pkt <type>"},	//MTK20120829_MT7530_1588pkt_generation

	{"pbrw-test",		doGSWRegTest,		0x12,	3,	"pbrw-test <reg> <cnt> <mask>"},
//	#endif //SQA_VERIFY

	#if defined(SQA_VERIFY)
	{"pbr",			doGSWRead,		0x12,	1,	"pbr <reg>"},
	{"pbw",			doGSWWrite,		0x12,	2,	"pbw <reg> <value>"},
//	{"pmiir",		doGSWPhyRead,		0x12,	2,	"pmiir <phy_addr> <phy_reg>"},
	{"pmiir",		doTCMiiRead,		0x12,	2,	"pmiir <phy_addr> <phy_reg>"},
//	{"pmiiw",		doGSWPhyWrite,		0x12,	3,	"pmiiw <phy_addr> <phy_reg> <value>"},
	{"pmiiw",		doTCMiiWrite,		0x12,	3,	"pmiiw <phy_addr> <phy_reg> <value>"},
	#endif //SQA_VERIFY
	#ifdef MT7530_PHY_VERIFY
	{"miir45",		doTCMiiRead45,		0x12,	3,	"miir45 <portAddr> <devAddr> <regAddr>"},
	{"miiw45",		doTCMiiWrite45,		0x12,	4,	"miiw45 <portAddr> <devAddr> <regAddr> <miiData>"},
	#endif

	#ifdef ETHCMD_EXTENSION
	/*Extend ci-cmd */
	#if defined(TCSUPPORT_2PORTS) || defined(TCSUPPORT_CT_2PWIFI)
	{"portbinding",doPortBinding,		0x12,	4,	HELP_PORTBINDING_INFO},
	#else
	{"portbinding",doPortBinding,		0x12,	4,	HELP_PORTBINDING},
	#endif
	/*end extend ci-cmd*/
	#endif
	{"version",	doVersion,	0x12,	0,	"Display ethphxcmd version."},
	{"help",		doHelp,		0x12,	0,	"ethphxcmd help."},
	#ifdef ETHCMD_DBG
	{"dbg",		doEthcmdDbg,		0x12,	0,	""},
	#endif
	#ifdef TCSUPPORT_MT7530_SWITCH_API
	{"portmode",		doPortMode,		0x12,	0,	"WPLin MT7530 switch API test"},
	{"wptest",		doWPtest,		0x12,	0,	"WPLin MT7530 switch API test"},
	{"dsp5qos", 		doDsP5Qos,			0x12,	0,	NULL},
	{"dstrtcm", 	doDsTrtcm,			0x12,	0,	NULL},
	{"stormctrl", 		doStormCtrl,			0x12,	0,	NULL},
	{"traffic",     doTrafficSel,   0x12,   1,  "Trgmii/rgmii traffic setting, subcmd: set,rgmii,display"},
	{"portmatrix",		doPortMatrix,		0x12,	4,	HELP_PORT_MATRIX},
	#endif
	{"dstq",		doDstq,		0x12,	2,	HELP_DSTQ},
	{NULL,		NULL,		0x10,	0,	NULL},	
};

static const cmds_t ethCmds[] = {
#if defined(TCSUPPORT_2PORTS) || defined(TCSUPPORT_CT_2PWIFI)
	{"media-type",	doPhyMediaType,	0x12,	0,	HELP_MEDIA_TYPE_INFO},
	{"reset",			doPhyReset,		0x12,	0,	HELP_RESET_INFO},
	{"lanchip",		doLanchip,		0x12,	0,	HELP_LANCHIP_INFO},
#else
	{"media-type",	doPhyMediaType,	0x12,	0,	HELP_MEDIA_TYPE},
	{"reset",			doPhyReset,		0x12,	0,	HELP_RESET},
	{"lanchip",		doLanchip,		0x12,	0,	HELP_LANCHIP},	
#endif
	{"vlanpt",		doVlanpt,		0x12,	1,	HELP_VLANPT},	
	{"status",		doStatus,		0x12,	0,	HELP_STATUS},
	{"igmpsnoop",		doIgmpsnoop,	0x12,	1,	HELP_IGMPSNOOP},
	{NULL,	NULL,		0x10,	0,	NULL},	
};

static const cmds_t vlanCmds[] = {
	{"active",		doVlanSetActive,	0x12,	1,	HELP_VLAN_ACTIVE},
	{"pvid",		doVlanSetPvid,		0x12,	2,	HELP_VLAN_PVID},
//#if defined(SQA_VERIFY)
//	{"vid",			doVlanSetVid,		0x12,	7,	HELP_VLAN_VID},
//#else
	{"vid",			doVlanSetVid,		0x12,	5,	HELP_VLAN_VID},
//#endif
	{"disp",		doVlanDisp,		0x12,	0,	NULL},
	{"port-attr",		doVlanSetPortAttr,	0x12,	2,	HELP_VLAN_PORT_ATTR},
	{"port-mode",		doVlanSetPortMode,	0x12,	2,	HELP_VLAN_PORT_MODE},
	{"eg-tag",		doVlanSetEgressTagPVC,	0x12,	2,	HELP_VLAN_EGRESS_TAG_PVC},		// will be obsolete
	{"eg-tag-pvc",		doVlanSetEgressTagPVC,	0x12,	2,	HELP_VLAN_EGRESS_TAG_PVC},
	{"eg-tag-pcr",		doVlanSetEgressTagPCR,	0x12,	2,	HELP_VLAN_EGRESS_TAG_PCR},
	{"acc-frm",		doVlanSetAccFrm,	0x12,	2,	HELP_VLAN_ACC_FRM},
	{NULL,			NULL,			0x10,	0,	NULL},
};
//#if defined(SQA_VERIFY)
static const cmds_t aclCmds[] = {
	{"enable",		doAclSetPortEn,		0x12,	2,	HELP_ACL_SETPORTEN},
	{"multi-hit",		doAclSetMultiHit,	0x12,	1,	HELP_ACL_SETMULTIHIT},
	{"tbl-clr",		doAclTblClr,		0x12,	0,	"acl tbl-clr : to clear all acl; mask, rule ctrl, rate ctrl; and trTCM table entries"},
	{"tbl-disp",		doAclTblDisp,		0x12,	1,	"acl tbl-disp ext_switch: to display all acl; mask, rule ctrl, rate ctrl; and trTCM table entries"},
	{"acltbl-add",		doAclAclTblAdd,		0x12,	3,	HELP_ACL_ACL_TBL_ADD},
	{"masktbl-add",		doAclMaskTblAdd,	0x12,	3,	HELP_ACL_MASK_TBL_ADD},
	{"ruletbl-add",		doAclRuleTblAdd,	0x12,	3,	HELP_ACL_RULE_TBL_ADD},
	{"ratetbl-add",		doAclRateTblAdd,	0x12,	3,	HELP_ACL_RATE_TBL_ADD},
	{NULL,			NULL,			0x10,	0,	NULL},
};

static const cmds_t qosCmds[] = {
	{"type",		doQosType,		0x12,	1,	HELP_QOS_TYPE},
	{"base",		doQosBase,		0x12,	1,	HELP_QOS_BASE},
	{"prio-qmap",		doQosSetPrioQmap,	0x12,	2,	HELP_QOS_PRIO_QMAP},
	{"prio-tagmap",		doQosSetPrioTagmap,	0x12,	2,	HELP_QOS_PRIO_TAGMAP},
	{"prio-dscpmap",	doQosSetPrioDSCPmap,	0x12,	2,	HELP_QOS_PRIO_DSCPMAP},
//	{"vprio-qmap",	doQosSetVpriQmap,	0x12,	2,	HELP_QOS_VPRI_QMAP},
	{"port-prio",		doQosSetPortPrio,	0x12,	2,	HELP_QOS_PORT_PRIO},
	{"port-weight",		doQosSetPortWeight,	0x12,	9,	HELP_QOS_PORT_WEIGHT},
	{"dscp-prio",		doQosSetDSCPPrio,	0x12,	2,	HELP_QOS_DSCP_PRIO},
	{NULL,			NULL,			0x10,	0,	NULL},
};
//#endif  //SQA_VERIFY

//#if defined(SQA_VERIFY)
static const cmds_t arlCmds[] = {
	{"aging",		doArlAging,		0x12,	2,	HELP_ARL_AGING},
	{"l2len-chk",		doArlChkLen,		0x12,	1,	HELP_ARL_L2LEN_CHK},
	{"addrtbl-add",		doArlMACTblAdd,		0x12,	2,	HELP_ARL_MAC_TBL_ADD},
	{"addrtbl-disp",	doArlMACTblDisp,	0x12,	0,	"arl addrtbl-disp\n"},
	{"mactbl-add",		doArlMACTblAdd,		0x12,	2,	HELP_ARL_MAC_TBL_ADD},
	{"mactbl-disp",		doArlMACTblDisp,	0x12,	0,	"arl mactbl-disp\n"},
	{"mactbl-clr",		doArlMACTblClr,		0x12,	0,	"arl mactbl-clr\n"},
	{"diptbl-add",		doArlDIPTblAdd,		0x12,	6,	HELP_ARL_DIP_TBL_ADD},
	{"diptbl-disp",		doArlDIPTblDisp,	0x12,	0,	"arl diptbl-disp\n"},
	{"diptbl-clr",		doArlDIPTblClr,		0x12,	0,	"arl diptbl-clr\n"},
	{"siptbl-add",		doArlSIPTblAdd,		0x12,	4,	HELP_ARL_SIP_TBL_ADD},
	{"siptbl-disp",		doArlSIPTblDisp,	0x12,	0,	"arl siptbl-disp\n"},
	{"siptbl-clr",		doArlSIPTblClr,		0x12,	0,	"arl siptbl-clr\n"},
	{NULL,			NULL,			0x10,	0,	NULL},
};

static const cmds_t macCmds[] = {
	{"col-lmt",		doMacColLmt,		0x12,	1,	HELP_MACCTL_COLLMT},
	{"fc",			doMacFC,		0x12,	1,	HELP_MACCTL_FC},
	{NULL,			NULL,			0x10,	0,	NULL},
};

static const cmds_t mirrorCmds[] = {
	{"enable",		doMirrorEn,		0x12,	2,	HELP_MIRROR_EN},
	{"port-based",		doMirrorPortBased,	0x12,	6,	HELP_MIRROR_PORTBASED},
	{NULL,			NULL,			0x10,	0,	NULL},
};

static const cmds_t phyCmds[] = {
	{"an",			doPhyANEn,		0x12,	2,	HELP_PHY_AN_EN},
	{"fc",			doPhyFCEn,		0x12,	2,	HELP_PHY_FC_EN},
	{NULL,			NULL,			0x10,	0,	NULL},
};

static const cmds_t mibCmds[] = {
	{"clr",			doMibClr,		0x12,	0,	"mib clr"},
	{NULL,			NULL,			0x10,	0,	NULL},
};
//#endif //SQA_VERIFY

#ifdef ETHCMD_EXTENSION
static struct portMap tcPortMap[]={
		{"e0", "eth0.1"},
		{"e1", "eth0.2"},
#if !defined(TCSUPPORT_CT_2PWIFI) && !defined(TCSUPPORT_2PORTS)
		{"e2", "eth0.3"},
		{"e3", "eth0.4"},
#endif
		{"p0", "nas0"},
		{"p1", "nas1"},
		{"p2", "nas2"},
		{"p3", "nas3"},
		{"p4", "nas4"},
		{"p5", "nas5"},
		{"p6", "nas6"},
		{"p7", "nas7"},
		{"w0", "ra0"},
		{"w1", "ra1"},
		{"w2", "ra2"},
		{"w3", "ra3"},
		{"u0", "usb0"},
		{NULL, NULL},
};

#define MAX_IFS	(sizeof(tcPortMap)/sizeof(tcPortMap[0])) 
#define PM_START_IF_ARG	3
#endif

static const char *media_names[] = {
	"10baseT", "10baseT-FD", "100baseTx", "100baseTx-FD", "100baseT4",
	"Flow-control", 0,
};

#ifdef ETHCMD_DBG
u8 ethcmdDbg=0;
#define ETHCMDDBG_FILE	"/var/tmp/ethdbg"
#define ETH_DEBUG(fmt,args...) if(ethcmdDbg) printf("\n %s %d: " fmt, __FUNCTION__ ,__LINE__, ##args)
#endif
/*___________________________________________________________________
**      function name: cmdHelp
**      descriptions:
**         Ethcmd ci-cmd helpful message.
**
**      parameters:
**         None
**  
**      global:
**           Cmds
**             
**      return:
**         None
**	    
**      call:
**           None
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
void cmdHelp(){
	const cmds_t *cmdp=NULL;
	const cmds_t *ethcmdp=NULL;
	
	printf("Usage: ethphxcmd <eth0|addwan|delwan> <command> [arg]\n\n");
	for(cmdp = Cmds;cmdp->name != NULL;cmdp++){
		if(!strcmp(LAN_IF, cmdp->name)){
			for(ethcmdp = ethCmds;ethcmdp->name != NULL;ethcmdp++){
				printf("  %s %s\n\n", ethcmdp->name, ethcmdp->argc_errmsg);
			}
		}
		else if(strcmp("help", cmdp->name)!=0){
			printf("  %s %s\n\n", cmdp->name, cmdp->argc_errmsg);
		}
	}
	#ifdef SUPPORT_ATP_WANETH
	ethup_wanHelp();
	#endif
	printf("  ethphxcmd help\n");
}/*end cmdHelp*/
/*___________________________________________________________________
**      function name: doEth
**      descriptions:
**         Register ethcmd subcmd.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments 
**         p:function pointer
**         
**      global:
**           ethCmds
**             
**      return:
**         0:succuces
**         -1:fail
**	    
**      call:
**           subcmd
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
static int doEth(int argc, char *argv[], void *p){
	return subcmd(ethCmds, argc, argv, p);
}/*end doEth*/

static int port2phyID(int portID)
{
    int phyID = portID;
    etherChannVlanCap_t*  vlanCap=getEtherVlanCap();

    if ((portID < 0) || (portID >= vlanCap->max_port_no))
    {
        printf("\n(%s):Wrong portID:%d!\n", __FUNCTION__, portID);
        return 0;
    }

    if (_isMT751x)
    {
        if (!_DefaultUseExtMT7530)
            phyID += 8;
    }
    else if (_isMT7525 || _isMT7520 || _isMT7520S)
        phyID += 9;    
	else if (_isMT7505)
		phyID += 8;
    else if(_isEN751221){
        if(!use_extern_switch){
            phyID += 8;
        }
    }
    return phyID;
}

#if (defined TCSUPPORT_CPU_RT63365 || defined TCSUPPORT_CPU_MT7520)
static int lanPortSwap(int portID)
{
    int i, lanPortNum=0;
    int tmp[6];
    etherChannVlanCap_t*  vlanCap=getEtherVlanCap();

    if ((portID < 0) || (portID >= vlanCap->max_port_no))
    {
        printf("\n(%s):Wrong portID:%d!\n", __FUNCTION__, portID);
        return -1;
    }

    if (portID == get_wan_port()) //no need to swap wan_port
        return -1;

    memset(tmp, -1, sizeof(tmp));

    for (i = 0; i < 6; i++)
        if (lan_portmap[i] != -1)
            lanPortNum++;

    if (lanPortNum == 0)
    {
        printf("\nError(%s): no LAN ports found!\n", __FUNCTION__);
        return -1;
    }

    for (i = 0; i < 6; i++)
    {
        if (lan_portmap[i] != -1)
        {
            tmp[--lanPortNum] = lan_portmap[i];
            if (lanPortNum == 0)
                break;
        }
    }

    return tmp[portID];
}
#endif

/*___________________________________________________________________
**      function name: doAddwan
**      descriptions:
**         Add vlan virtual interface for wan interface. 
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments 
**         p:function pointer
**         
**      global:
**           None
**             
**      return:
**         0:succuces
**         -1:fail
**	    
**      call:
**           ETH_DEBUG
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
static int doAddwan(int argc, char *argv[], void *p){
	int iVid = 0;
	int iVlanPriority = 0;
	int iRet = -1;
	char szCmd[ETHUP_CMDSIZE];
	char szBaseDev[ETHUP_NAMESIZE];
	char szVDev[ETHUP_NAMESIZE];
	
	memset(szBaseDev,0,ETHUP_NAMESIZE);
	memset(szVDev,0,ETHUP_NAMESIZE); 
	memset(szCmd,0,ETHUP_CMDSIZE);
	
	iVlanPriority = atoi(argv[3]);
	/*if create nas0 do nothing "ethcmd addwan 65535 nas0 f "*/
	if(MAX_VID_VALUE == atoi(argv[1])){
	    printf("\n create wan without vlan id ok \n");
	    return 0;
	}

	if((NULL == argv[3]) || ( ETHUP_NAMESIZE < strlen(argv[2])) ||  \
	    ETHUP_MAXPRORITY < iVlanPriority || ETHUP_MINPRORITY > iVlanPriority)
	{
	    printf("\n ERROR: %s,%s wrong parameter .%d \n ",__FILE__,__FUNCTION__,__LINE__);
	    return -1;
	}

	memcpy(szBaseDev,ETHUP_WAN,ETHUP_NAMESIZE);
	memcpy(szVDev, argv[2], ETHUP_NAMESIZE);
	iVid = atoi(argv[1]);

	if (VLAN_GROUP_ARRAY_LEN-2 < iVid || 0 > iVid  \
	    || ETHUP_NAMESIZE < strlen(szBaseDev) || (ETHUP_NAMESIZE < strlen(szVDev))){
		printf("\n ERROR: Addwan device error. %s,%s,%d \n ",__FILE__,__FUNCTION__,__LINE__);    
		printf("\n ERROR: %s,%s,%d \n ",__FILE__,__FUNCTION__,__LINE__);
		return -1;
	}
	
	sprintf(szCmd,"vconfig add %s %d", szVDev, iVid);
	#ifdef ETHCMD_DBG
	ETH_DEBUG("%s",szCmd);
	#endif
	system(szCmd);
	
	sprintf(szCmd,"ifconfig %s up",szVDev);
	#ifdef ETHCMD_DBG
	ETH_DEBUG("%s",szCmd);
	#endif
	system(szCmd);

	/*now deal with prority*/
	sprintf(szCmd,"vconfig set_egress_map %s.%d 0 %d",szVDev,iVid,iVlanPriority);
	#ifdef ETHCMD_DBG
	ETH_DEBUG("%s",szCmd);
	#endif
	system(szCmd);

	sprintf(szCmd,"vconfig set_ingress_map %s.%d 0 %d",szVDev,iVid,iVlanPriority);
	#ifdef ETHCMD_DBG
	ETH_DEBUG("%s",szCmd);
	#endif
	system(szCmd);

	printf("\n ethcmd add wan interface %s ok \n",szVDev);
	return 0;
}/*end doAddwan*/
/*___________________________________________________________________
**      function name: doDelwan
**      descriptions:
**         Delete vlan virtual interface for wan interface. 
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments 
**         p:function pointer
**         
**      global:
**           None
**             
**      return:
**         0:succuces
**         -1:fail
**	    
**      call:
**           None
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
static int doDelwan(int argc, char *argv[], void *p){
	char szCmd[MAX_CMD_LEN];
	char wanIf[ETHUP_NAMESIZE];
	int i=0;
	int find=ETHCMD_ERR;
	memset(szCmd,0,sizeof(szCmd));
	memset(wanIf,0,sizeof(wanIf));

	for(i=0;i<MAX_PVCS; i++){
		sprintf(wanIf,"nas%d",i);
		if(0 == strcmp(argv[1], wanIf)){
			find=0;
			break;
		}
	}
	if(find==0){/*send to wan site is no vlan header*/
		sprintf(szCmd,"ifconfig %s 0.0.0.0",wanIf);
		system(szCmd);
	}
	else{
		sprintf(szCmd,"ifconfig %s down", argv[1]);
		system(szCmd);
		sleep(1);
		sprintf(szCmd,"vconfig rem %s", argv[1]);
		system(szCmd);
	}
	printf("\n ethcmd delete wan interface %s ok \n",argv[1]);
	return 0;
}/*end doDelwan*/
/*___________________________________________________________________
**      function name: findNegoPortID
**      descriptions:
**         get the phy negotiation port id
**
**      parameters:
**         None
**         
**      global:
**         None
**             
**      return:
**         i: the phy negotiation port id
**         -1:fail
**	    
**      call:
**           getSwcfg
**           mdio_read
**         
**      
**      revision:
**      1. xflu 2011/07/21
**_______________________________________________________________
*/

int findNegoPortID(void)
{
	int physid = 0;
	int i = 0;
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();
	
	for(i=0; i<MAX_PHY_NUM; i++){
		physid =mdio_read(swcfg->skfd, swcfg->ifr, i, MII_PHYSID1);
		if(physid == 0)
			physid = mdio_read(swcfg->skfd, swcfg->ifr, i, MII_PHYSID2);
		if( physid == PHY_COMPANYID ){
			return i;
		} 
	}
	
	return -1;
	
}
/*___________________________________________________________________
**      function name: doPhyMediaType
**      descriptions:
**         Set/query Ethernet PHY link state.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments 
**         p:function pointer
**         
**      global:
**           None
**             
**      return:
**         0:succuces
**         -1:fail
**	    
**      call:
**           getSwcfg
**           ethcmdGetMediaOption
**           showMediaSetting
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
static int doPhyMediaType(int argc, char *argv[], void *p){
	int portid =0;
	u16 val=0;
	etherChannVlanCap_t*  vlanCap=getEtherVlanCap();
	
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	int flag = 0;
	int singlePID = 0;
	singlePID = findNegoPortID();
	int phy_id=0;

    /*Set media-type */
	if(((argc==4) && (strcmp(argv[3],"swap")))||
	    ((argc==5) && (!strcmp(argv[4],"swap")))){
		val=ethcmdGetMediaOption(argv[1]);
		if(val==ERR_MEDIAY_TYPE){
			#if defined(TCSUPPORT_2PORTS) || defined(TCSUPPORT_CT_2PWIFI)
			printf(HELP_MEDIA_TYPE, argv[0], vlanCap->max_port_no-1,vlanCap->max_port_no-1);
			#else
			printf("  %s %s\n\n", argv[0], HELP_MEDIA_TYPE);
			#endif
			return -1;
		}
		else{
			portid=atoi(argv[3]);
			if((strcmp(argv[2],ETHCMD_PORT)==0) && ((portid==0) ||
			    (ethcmdQueryPortCount(portid)==SWIC_PORTS_NUM))){
				if(argc == 5 || (swcfg->flag & PORT_REVERSE)){//swap
                #if (defined TCSUPPORT_CPU_RT63365 || defined TCSUPPORT_CPU_MT7520)
                    if (lanPortSwap(portid) != -1)
                        portid = lanPortSwap(portid);
                #endif
                    phy_id = port2phyID(portid);
                    mdio_write(swcfg->skfd, swcfg->ifr, phy_id, MII_BMCR, val);
					flag = 1;
				}else{
                #if (defined TCSUPPORT_CPU_RT63365 || defined TCSUPPORT_CPU_MT7520)
					portid = lan_portmap[portid];
					if (portid == -1){
						printf("Invalied port: pordid is -1\n");
						return -1;
					}
				#endif					
					if(vlanCap->max_port_no==SINGLE_PORT)
						mdio_write(swcfg->skfd, swcfg->ifr, singlePID, MII_BMCR, val);
					else
						mdio_write(swcfg->skfd, swcfg->ifr, port2phyID(portid), MII_BMCR, val);
				}
				/*waiting the ethernet link up*/
				sleep(ETHUP_WAIT_PHY_READY);
			}
			else{
				#if defined(TCSUPPORT_2PORTS) || defined(TCSUPPORT_CT_2PWIFI)
				printf(HELP_MEDIA_TYPE, argv[0], vlanCap->max_port_no-1,vlanCap->max_port_no-1);
				#else
				printf("  %s %s\n\n", argv[0], HELP_MEDIA_TYPE);
				#endif
				return -1;
			}
		}
	}
	else if(argc==3 ||((argc==4) &&(!strcmp(argv[3],"swap"))) ){/*Query media-type */
		portid=atoi(argv[2]);
		if((strcmp(argv[1],ETHCMD_PORT)!=0)\
			||(ethcmdQueryPortCount(portid)<=portid)){
			printf("interface %s is Ethernet Switch, please use port [0-%d] argument\n", swcfg->ifr.ifr_name, (vlanCap->max_port_no-1));
			#if defined(TCSUPPORT_2PORTS) || defined(TCSUPPORT_CT_2PWIFI)
			printf(HELP_MEDIA_TYPE, argv[0], vlanCap->max_port_no-1,vlanCap->max_port_no-1);
			#else
			printf("  %s %s\n\n", argv[0], HELP_MEDIA_TYPE);
			#endif
			return -1;
		}
		else{
			if(argc==4 || /*swcfg->reverse_flag == 1*/(swcfg->flag & PORT_REVERSE)){
				//because argv[3] must be swap here!!!
				flag = 1;
            #if (defined TCSUPPORT_CPU_RT63365 || defined TCSUPPORT_CPU_MT7520)
                if (lanPortSwap(portid) != -1)
                    portid = lanPortSwap(portid);
            #endif

			}
        #if (defined TCSUPPORT_CPU_RT63365 || defined TCSUPPORT_CPU_MT7520)
            else
            {
                portid = lan_portmap[portid];
                if (portid == -1){
                    printf("Invalied port: pordid is -1\n");
                    return -1;
                }
            }
        #endif
		}
	}
	else{
		printf("interface %s is Ethernet Switch, please use port [0-%d] argument\n", swcfg->ifr.ifr_name, (vlanCap->max_port_no-1));
		#if defined(TCSUPPORT_2PORTS) || defined(TCSUPPORT_CT_2PWIFI)
		printf(HELP_MEDIA_TYPE, argv[0], vlanCap->max_port_no-1,vlanCap->max_port_no-1);
		#else
		printf("  %s %s\n\n", argv[0], HELP_MEDIA_TYPE);
		#endif
		return -1;
	}
	/*The single port's portid==0, otherwise the switch is not limit it*/
	if((portid==0)||(vlanCap->max_port_no==SWIC_PORTS_NUM))
	{
		if(flag){
            phy_id = port2phyID(portid);
			showMediaSetting(phy_id);
		}else{
			if(vlanCap->max_port_no==SINGLE_PORT)
				showMediaSetting(singlePID);
			else
                showMediaSetting(port2phyID(portid));
        }
	}
	return 0;
}/*end doPhyMediaType*/
/*___________________________________________________________________
**      function name: doPhyReset
**      descriptions:
**        Ethernet PHY Reset to use mii reigster to control.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments 
**         p:function pointer
**         
**      global:
**           None
**             
**      return:
**         0:succuces
**         -1:fail
**	    
**      call:
**           getSwcfg
**           showMediaSetting
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
static int doPhyReset(int argc, char *argv[], void *p){
	int portid =0;
	int val=0;
	int phy_id=0;
	etherChannVlanCap_t*  vlanCap=getEtherVlanCap();
	
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();
	int singlePID = 0;
	singlePID = findNegoPortID();
	
	int flag = 0;
	if(argc==3||((argc==4) &&(!strcmp(argv[3],"swap")))){/*PHY Reset*/
		portid=atoi(argv[2]);
		if((strcmp(argv[1],ETHCMD_PORT)!=0)\
			||(ethcmdQueryPortCount(portid)<=portid)){
			#if defined(TCSUPPORT_2PORTS) || defined(TCSUPPORT_CT_2PWIFI)
			printf(HELP_RESET, argv[0], vlanCap->max_port_no-1,vlanCap->max_port_no-1);
			#else
			printf(HELP_RESET, argv[0], vlanCap->max_port_no-1);
			#endif
			return -1;
		}
		else{
			if(argc==4 || /*swcfg->reverse_flag == 1*/(swcfg->flag & PORT_REVERSE)){
				flag = 1;
				}
		
		}
	}
	else{
		printf("interface %s is Ethernet Switch, please use port [0-%d] argument\n", swcfg->ifr.ifr_name, (vlanCap->max_port_no-1));
		#if defined(TCSUPPORT_2PORTS) || defined(TCSUPPORT_CT_2PWIFI)
		printf(HELP_RESET, argv[0], vlanCap->max_port_no-1,vlanCap->max_port_no-1);
		#else
		printf(HELP_RESET, argv[0], vlanCap->max_port_no-1);
		#endif
		return -1;
	}
	
	/*The single port's portid==0, otherwise the switch is not limit it*/
	if((portid==0)||(vlanCap->max_port_no==SWIC_PORTS_NUM)){
		/*Specify the PHY Reset*/
		if(flag)
		{
        #if (defined TCSUPPORT_CPU_RT63365 || defined TCSUPPORT_CPU_MT7520)
            if (lanPortSwap(portid) != -1)
                portid = lanPortSwap(portid);
        #endif
            phy_id = port2phyID(portid);
			val = mdio_read(swcfg->skfd, swcfg->ifr, phy_id, MII_BMCR);
			val |= BMCR_RESET; // set bit15=1
			mdio_write(swcfg->skfd, swcfg->ifr, phy_id, MII_BMCR, val);
		}
		else
		{
			if(vlanCap->max_port_no==SINGLE_PORT){
				val = mdio_read(swcfg->skfd, swcfg->ifr, singlePID, MII_BMCR);
				val |= BMCR_RESET; // set bit15=1
				mdio_write(swcfg->skfd, swcfg->ifr, singlePID, MII_BMCR, val);
			}
			else{
                val = mdio_read(swcfg->skfd, swcfg->ifr, port2phyID(portid), MII_BMCR);
                val |= BMCR_RESET; // set bit15=1
                mdio_write(swcfg->skfd, swcfg->ifr, port2phyID(portid), MII_BMCR, val);
			}
		}
		/*Waitfor PHY link up and ready*/
		sleep(ETHUP_WAIT_PHY_READY);
		if(flag)
		{
        #if (defined TCSUPPORT_CPU_RT63365 || defined TCSUPPORT_CPU_MT7520)
            if (lanPortSwap(portid) != -1)
                portid = lanPortSwap(portid);
        #endif
            phy_id = port2phyID(portid);
			showMediaSetting(phy_id);
		}
		else{
			if(vlanCap->max_port_no==SINGLE_PORT)
				showMediaSetting(singlePID);
			else
                showMediaSetting(port2phyID(portid));
		}	
	}
	return 0;
}/*end doPhyReset*/

#if defined(TCSUPPORT_CPU_MT7520)
/*__________________________________________________________________
**	function name: dis_flow_ctrl / dis_ext_flow_ctrl
**	descriptions:
**		Only for MT7520 support etherwan.
**		Disable Inner Switch Global Flow Control: 0x1FE0 bit31
**		Disable External Switch Global Flow Control: 0x1FE0 bit31
**		Disbale External Switch Port6 Flow Control: 0x3600 bit4&bit5
**		Disbale Inner Port5 Flow Control: 0x3500 bit4&bit5
**__________________________________________________________________
*/
static void dis_ext_flow_ctrl(struct swconfig *swcfg)
{
	u32 value = 0, reg = 0;

	/* Disable External Switch Global Flow Control */
	reg = 0x1FE0;
	value = gsw_reg_read2(swcfg->skfd, swcfg->ifr, 1, reg);
	value &= ~(1 << 31);
	gsw_reg_write2(swcfg->skfd, swcfg->ifr, 1, reg, value);
	/* Disbale External Switch Port6 Flow Control */
	reg = 0x3600;
	value = gsw_reg_read2(swcfg->skfd, swcfg->ifr, 1, reg);
	value &= ~(0x30);
	gsw_reg_write2(swcfg->skfd, swcfg->ifr, 1, reg, value);
	/* Disbale Inner Port5 Flow Control */
	reg = 0x3500;
	value = gsw_reg_read2(swcfg->skfd, swcfg->ifr, 0, reg);
	value &= ~(0x30);
	gsw_reg_write2(swcfg->skfd, swcfg->ifr, 0, reg, value);
}

static void dis_flow_ctrl(struct swconfig *swcfg)
{
	u32 value = 0, reg = 0;

	reg = 0x1FE0;
	/* Disable Inner Switch Global Flow Control */
	value = gsw_reg_read2(swcfg->skfd, swcfg->ifr, 0, reg);
	value &= ~(1 << 31);
	gsw_reg_write2(swcfg->skfd, swcfg->ifr, 0, reg, value);

	#if defined(JUDGE_SWITCH_SCENARIO_BY_751020_SUBMODEL)
	if (_DefaultUseExtMT7530)
	{
		dis_ext_flow_ctrl(swcfg);
	}
	#elif defined(DEFAULT_USE_EXT_SWIC)
	dis_ext_flow_ctrl(swcfg);
	#endif
}
#endif /* defined(TCSUPPORT_CPU_MT7520) */

/*___________________________________________________________________
**      function name: doVlanpt
**      descriptions:
**        Ethernet PHY Reset to use mii reigster to control.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments 
**         p:function pointer
**         
**      global:
**           None
**             
**      return:
**         0:succuces
**         -1:fail
**	    
**      call:
**           None
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
static int doVlanpt(int argc, char *argv[], void *p){
	char cmd[MAX_CMD_LEN]={0};
	int retval=0;
	int i=0;
	etherChannVlanCap_t*  vlanCap=getEtherVlanCap();
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	int flag = 0;
#if defined(TCSUPPORT_CPU_RT63365)||  defined(TCSUPPORT_CPU_MT7505)  || \
    defined(TCSUPPORT_CPU_MT7510) || defined(TCSUPPORT_CPU_MT7520) || \
    defined(TCSUPPORT_CPU_MT7510_FAKE)
	int portmap, wan_port=-1;
#endif	
	/*single port*/
	if(vlanCap->max_port_no ==SINGLE_PORT){
		printf("eth0 is not Switch chip.\n");
		return 0;
	}
#ifdef TCSUPPORT_CPU_MT7520	
	if (_isMT7520S && !strcmp(argv[1], ETHCMD_ENABLE)){
		printf("MT7520S is single port!\n");
		if (-1 == get_wan_port())
			return 0;
	}
#endif

	if(!strcmp(argv[1], ETHCMD_ENABLE)){
		for(i=START_VLAN_VID;i<(vlanCap->max_port_no+START_VLAN_VID);i++){
#if defined(TCSUPPORT_WAN_ETHER) || defined(TCSUPPORT_CPU_MT7520) || defined(TCSUPPORT_CT) 
			if(i>4)
				continue;
#endif				
#if defined(TCSUPPORT_INIC_HOST) || defined(TCSUPPORT_INIC_CLIENT)
			if(i>2)
				continue;
#endif

#if defined(TCSUPPORT_CPU_MT7520)
			if (-1 != get_wan_port())
			{
				int port_id = 0, max_port = 4;

			#if defined(TCSUPPORT_INIC_HOST) || defined(TCSUPPORT_INIC_CLIENT)
				max_port = 2;
			#endif

				if (swcfg->flag & PORT_REVERSE)
					port_id = lan_portmap[max_port - i];
				else
					port_id = lan_portmap[i - START_VLAN_VID];

				if (-1 == port_id)
					continue;
			}
#endif /* defined(TCSUPPORT_CPU_MT7520) */

#ifndef TCSUPPORT_SEPERATED_ETH_ITF
			/*Create 4 virtual vlan interface*/
			sprintf(cmd, "/userfs/bin/vconfig add eth0 %d", i);
			system(cmd);
#endif
			/*Let virtual vlan ethernet interface up*/
			sprintf(cmd, "ifconfig eth0.%d 0.0.0.0 up", i);
			system(cmd);
			/*Binding virtual vlan ethernet interface into br0 interface*/
			sprintf(cmd, "brctl addif br0 eth0.%d", i);
			system(cmd);
		}
		/*Remove the eth0 ethernet interface from br0 interface*/
		system("brctl delif br0 eth0");
		
		/*Active the switch vlan function*/
		if((swic.vlan_active!=NULL) && (swic.vlan_pvid!=NULL)&&(swic.vlan_vid!=NULL)){
			swic.vlan_active(ACTIVED);
	
#if 1//ndef VPORT			
			if (!(swcfg->flag & STAG_TO_VTAG)) {
				for(i=0; i<vlanCap->max_port_no; i++){
					/*Set the default PVID for each port, Default pvid value is start at 2.*/
					if((argc == 3 && !strcmp(argv[2],"swap")) || /*swcfg->reverse_flag == 1*/(swcfg->flag & PORT_REVERSE))
					{
						swic.vlan_pvid(i, START_VLAN_VID+vlanCap->max_port_no -i -1);
						flag = 1;
					}
					else
					{
						swic.vlan_pvid(i, i+START_VLAN_VID);
					}
					
					if(swcfg->swic_id==IP175C){
						/*Set the default vlan table rules*/
						if(flag)
						#if defined(EXTEND_SETVID_PARAM)
							swic.vlan_vid(i+START_VLAN_VID, ACTIVED, START_VLAN_VID+vlanCap->max_port_no -i -1, (1<<i), 0x0, 0, 0, 0);
						#else
							swic.vlan_vid(i+START_VLAN_VID, ACTIVED, START_VLAN_VID+vlanCap->max_port_no -i -1, (1<<i), 0x0);
						#endif
						else
						#if defined(EXTEND_SETVID_PARAM)
							swic.vlan_vid(i+START_VLAN_VID, ACTIVED, i+START_VLAN_VID, (1<<i), 0x0, 0, 0, 0);
						#else
							swic.vlan_vid(i+START_VLAN_VID, ACTIVED, i+START_VLAN_VID, (1<<i), 0x0);
						#endif
					}
					else{
						/*Set the default vlan table rules*/
						if(flag)
						#if defined(EXTEND_SETVID_PARAM)
							swic.vlan_vid(i, ACTIVED, START_VLAN_VID+vlanCap->max_port_no -i -1, (1<<i), 0x0, 0, 0, 0);
						#else
							swic.vlan_vid(i, ACTIVED, START_VLAN_VID+vlanCap->max_port_no -i -1, (1<<i), 0x0);
						#endif
						else
						#if defined(EXTEND_SETVID_PARAM)
							swic.vlan_vid(i, ACTIVED, i+START_VLAN_VID, (1<<i), 0x0, 0, 0, 0);
						#else
							swic.vlan_vid(i, ACTIVED, i+START_VLAN_VID, (1<<i), 0x0);
						#endif
					}
				}
#if defined(TCSUPPORT_CPU_RT63365) ||  defined(TCSUPPORT_CPU_MT7505)  ||  defined(TCSUPPORT_CPU_MT7510) || defined(TCSUPPORT_CPU_MT7520) || defined(TCSUPPORT_CPU_MT7510_FAKE)
				//bind all lan port to cpu port
			#if defined(EXTEND_SETVID_PARAM)
				swic.vlan_vid(i, ACTIVED, i+START_VLAN_VID, ((1<<0) | (1<<1) | (1<<2) | (1<<3)), 0x0, 0, 0, 0);
			#else
				swic.vlan_vid(i, ACTIVED, i+START_VLAN_VID, ((1<<0) | (1<<1) | (1<<2) | (1<<3)), 0x0);
			#endif
#endif
			}
			else{
#if !defined(TCSUPPORT_CPU_MT7520) &&  !defined(TCSUPPORT_CPU_EN7521) && !defined(TCSUPPORT_CPU_EN7512)
    // there is no ether wan port, and no need to set VLAN ID
#if defined(TCSUPPORT_CPU_RT63365) ||  defined(TCSUPPORT_CPU_MT7505)  || defined(TCSUPPORT_CPU_MT7510) || defined(TCSUPPORT_CPU_MT7510_FAKE)
				portmap = (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5);
				wan_port = get_wan_port();
				if(wan_port == -1){
					//bind all lan port to cpu port
				#if defined(EXTEND_SETVID_PARAM)
					swic.vlan_vid(0, ACTIVED, START_VLAN_VID, portmap, 0x0, 0, 0, 0);
				#else
					swic.vlan_vid(0, ACTIVED, START_VLAN_VID, portmap, 0x0);
				#endif
				}else{
					portmap &= ~(1<<wan_port);
				#if defined(EXTEND_SETVID_PARAM)
					//bind all lan port to cpu port
					swic.vlan_vid(0, ACTIVED, START_VLAN_VID, portmap, 0x0, 0, 0, 0);
					//bind etherwan port to cpu port. wan port vid 5
					swic.vlan_vid(1, ACTIVED, 5, (1<<wan_port), 0x0, 0, 0, 0);
				#else
					//bind all lan port to cpu port
					swic.vlan_vid(0, ACTIVED, START_VLAN_VID, portmap, 0x0);
					//bind etherwan port to cpu port. wan port vid 5
					swic.vlan_vid(1, ACTIVED, 5, (1<<wan_port), 0x0);
				#endif
				}
#endif				
#elif defined(TCSUPPORT_CPU_MT7520) /* TCSUPPORT_CPU_MT7520 */
				portmap = 0x3f; /* (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5) */
				wan_port = get_wan_port();

				if (-1 != wan_port)
				{
					u32 value = 0, reg = 0;

					portmap &= ~(1 << wan_port);
				#if defined(EXTEND_SETVID_PARAM)
					/* bind all lan port to cpu port */
					swic.vlan_vid(0, ACTIVED, START_VLAN_VID, portmap, 0x0, 0, 0, 0);
					/* bind etherwan port to cpu port. wan port vid 5 */
					swic.vlan_vid(1, ACTIVED, 5, (1 << wan_port), 0x0, 0, 0, 0);
				#else
					/* bind all lan port to cpu port */
					swic.vlan_vid(0, ACTIVED, START_VLAN_VID, portmap, 0x0);
					/* bind etherwan port to cpu port. wan port vid 5 */
					swic.vlan_vid(1, ACTIVED, 5, (1 << wan_port), 0x0);
				#endif

					dis_flow_ctrl(swcfg);

					if (5 == wan_port)
					{
						gsw_reg_write(swcfg->skfd, swcfg->ifr, 0x3500, 0x5e30b);
					}
				}
#endif // ndef TCSUPPORT_CPU_MT7520
			}
#endif			
		}
		sprintf(cmd, "echo \"1\" > %s", VPORT_FILE);
		system(cmd);
	}
	else if(!strcmp(argv[1], ETHCMD_DISABLE)){
		for(i=START_VLAN_VID;i<(vlanCap->max_port_no+START_VLAN_VID);i++){
#if defined(TCSUPPORT_CPU_MT7520)
			if(i>4)
				continue;
#endif				
#if defined(TCSUPPORT_INIC_HOST) || defined(TCSUPPORT_INIC_CLIENT)
			if(i>2)
				continue;
#endif
			/*Let virtual vlan ethernet interface down*/
			sprintf(cmd, "ifconfig eth0.%d down", i);
			system(cmd);
			/*Remove the virtual vlan ethernet interface from bridge interface*/
			sprintf(cmd, "brctl delif br0 eth0.%d", i);
			system(cmd);
			
#ifndef TCSUPPORT_SEPERATED_ETH_ITF
			/*Delete 4 virtual vlan interface*/
			sprintf(cmd, "vconfig rem eth0.%d", i);
			system(cmd);
#endif
		}
		/*Binding the eth0 ethernet interface into br0 interface*/
		system("brctl addif br0 eth0");
		sprintf(cmd, "echo \"0\" > %s", VPORT_FILE);
		system(cmd);
		/*Disabled the switch vlan function*/
		if(swic.vlan_active!=NULL){
			swic.vlan_active(DEACTIVED);
		}
	}
	else if(!strcmp(argv[1], ETHCMD_QUERY)){
		printf("\nVport Number is:%d\n", vlanCap->max_port_no);
	}
	else{		
		retval=-1;
	}
	return retval;
}/*end doVlanpt*/
/*___________________________________________________________________
**      function name: doLanchip
**      descriptions:
**        Enable/disable ethernet port to use mii regsiter to control it.(Use power
**        down).
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments 
**         p:function pointer
**         
**      global:
**           None
**             
**      return:
**         0:succuces
**         -1:fail
**	    
**      call:
**           getSwcfg
**           setLanchip
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
static int doLanchip(int argc, char *argv[], void *p){
	int portid =0;
	int val=0;
	int i=0;
	etherChannVlanCap_t*  vlanCap=getEtherVlanCap();
	struct swconfig *swcfg=NULL;
	int flag = -1;
	int singlePID = 0;
    int phy_id;

	singlePID = findNegoPortID();	
	swcfg=getSwcfg();

	if(argc==2){/*LAN Chip 4ports-switch Enable/Disable*/
		if(vlanCap->max_port_no == SWIC_PORTS_NUM){
			for(i=0;i<vlanCap->max_port_no; i++){
				if(setLanchip(argv[1], port2phyID(i))<0){
					break;
				}	
			}
		}
		else{/*for single port just set Lan chip port 1*/
			setLanchip(argv[1], singlePID);
			i = 1;
		}
		if(i !=vlanCap->max_port_no){
			#if defined(TCSUPPORT_2PORTS) || defined(TCSUPPORT_CT_2PWIFI)
			printf(HELP_LANCHIP, argv[0],vlanCap->max_port_no-1);
			#else
			printf("  %s %s\n\n", argv[0], HELP_LANCHIP);
			#endif
			return -1;
		}
	}
	else if((argc==4)||((argc == 5) && !strcmp(argv[4],"swap"))){/*Specify the phy port Enable/Disable*/
		portid=atoi(argv[3]);
		if((strcmp(argv[2],ETHCMD_PORT)==0)\
		&&((portid==0) ||(ethcmdQueryPortCount(portid)==SWIC_PORTS_NUM))){
			if(argc == 5 || /*swcfg->reverse_flag == 1*/(swcfg->flag & PORT_REVERSE))
            {
            #if (defined TCSUPPORT_CPU_RT63365 || defined TCSUPPORT_CPU_MT7520)
                if (lanPortSwap(portid) != -1)
                    portid = lanPortSwap(portid);
            #endif
                phy_id = port2phyID(portid);
				flag = setLanchip(argv[1], phy_id);
            }
			else{
#if (defined TCSUPPORT_CPU_RT63365 || defined TCSUPPORT_CPU_MT7520)
				#ifdef TCSUPPORT_CT_PON_CZ_GD
				portid = lan_portmap[portid]-1;
				#else
				portid = lan_portmap[portid];
				#endif			
				if (portid == -1){
					printf("Invalied port: pordid is -1\n");
					goto err_exit;
				}
			#endif				
				if(vlanCap->max_port_no==SINGLE_PORT)
					flag = setLanchip(argv[1], singlePID);
				else
                    flag = setLanchip(argv[1], port2phyID(portid));
			}
			if(flag<0){
				goto err_exit;
			}
		}
	}
	else{
		goto err_exit;
	}
	return 0;

err_exit:
	
	printf("interface %s is Ethernet Switch, please use port [0-%d] argument\n", swcfg->ifr.ifr_name, (vlanCap->max_port_no-1));
	#if defined(TCSUPPORT_2PORTS) || defined(TCSUPPORT_CT_2PWIFI)
	printf(HELP_LANCHIP, argv[0],vlanCap->max_port_no-1);
	#else
	printf("  %s %s\n\n", argv[0], HELP_LANCHIP);
	#endif
	return -1;
}/*end doLanchip*/

/*___________________________________________________________________
**      function name: doStatus
**      descriptions:
**        Get the link status of ethernet port.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments 
**         p:function pointer
**         
**      global:
**           None
**             
**      return:
**         0:succuces
**         -1:fail
**	    
**      call:
**           getSwcfg
**           mdio_read
**           ethcmdQueryPortCount
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
static int doStatus(int argc, char *argv[], void *p){
	int i=0;
	int val=0;
	int portid=0;
	etherChannVlanCap_t*  vlanCap=getEtherVlanCap();
	struct swconfig *swcfg=NULL;
	int singlePID = 0;
    int phy_id;

	singlePID = findNegoPortID();	
	swcfg=getSwcfg();
	if((argc==1) ||(argc==2 &&(!strcmp(argv[1],"swap")))){/*Get the Status of port 0~port 3 */
		if(vlanCap->max_port_no == SWIC_PORTS_NUM){
			for(i=0;i<vlanCap->max_port_no; i++){
				if(argc==2 || /*swcfg->reverse_flag == 1*/(swcfg->flag & PORT_REVERSE))
                {
                #if (defined TCSUPPORT_CPU_RT63365 || defined TCSUPPORT_CPU_MT7520)
                    if (lanPortSwap(i) != -1)
                        portid = lanPortSwap(i);
                    else
                #endif
                        portid = i;
                    phy_id = port2phyID(portid);
					val=mdio_read(swcfg->skfd, swcfg->ifr,phy_id, MII_BMSR);
                }
                else
					val=mdio_read(swcfg->skfd, swcfg->ifr, port2phyID(i), MII_BMSR);
				printf("%d ", ((val & BMSR_LSTATUS) ?1:0));
			}
		}
		else{/*fot single port*/
			val=mdio_read(swcfg->skfd, swcfg->ifr, singlePID, MII_BMSR);
			printf("%d ", ((val & BMSR_LSTATUS) ?1:0));
		}
		printf("\r\n");
	}
	else if(argc==3||(argc==4 &&(!strcmp(argv[1],"swap")))){/*Specify the phy port number*/
		portid=atoi(argv[2]);
		if((strcmp(argv[1],ETHCMD_PORT)==0)\
		&&((portid==0) ||(ethcmdQueryPortCount(portid)==SWIC_PORTS_NUM))){
			if(argc==4 || /*swcfg->reverse_flag == 1*/(swcfg->flag & PORT_REVERSE))
            {
            #if (defined TCSUPPORT_CPU_RT63365 || defined TCSUPPORT_CPU_MT7520)
                if (lanPortSwap(portid) != -1)
                    portid = lanPortSwap(portid);
            #endif
                phy_id = port2phyID(portid);
                val=mdio_read(swcfg->skfd, swcfg->ifr, phy_id, MII_BMSR);
			}
			else{
				if(vlanCap->max_port_no==SINGLE_PORT)
					val=mdio_read(swcfg->skfd, swcfg->ifr, singlePID, MII_BMSR);
				else
					val=mdio_read(swcfg->skfd, swcfg->ifr, port2phyID(portid), MII_BMSR);
			}
			printf("%d\r\n", ((val & BMSR_LSTATUS) ?1:0));
		}
		else{
			printf("  %s %s\n\n", argv[0], HELP_STATUS);
			return -1;
		}
	}
	else{
		printf("  %s %s\n\n", argv[0], HELP_STATUS);
		return -1;
	}
	return 0;
}/*end doStatus*/

/*___________________________________________________________________
**      function name: doIgmpsnoop
**      descriptions:
**        Enable/disable/query igmpsnoop for lanswitch
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments 
**         p:function pointer
**         
**      global:
**           swic
**             
**      return:
**         0:succuces
**         -1:fail
**	    
**      call:
**           getSwcfg
**           hw_igmpsnoop
**           hw_fastleave
**      
**      revision:
**      1. Here 2009/11/28
**_______________________________________________________________
*/
static int doIgmpsnoop(int argc, char *argv[], void *p){
	int retval=0;
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	if((swcfg->swic_id !=TC2206) 
	|| (swic.hw_igmpsnoop ==NULL)
	|| (swic.hw_fastleave ==NULL)){
		printf("%s is not support igmpsnoop funciton.\n", swcfg->ifr.ifr_name);
		return ETHCMD_ERR;
	}
	
	if(!strcmp(argv[1], ETHCMD_ENABLE)){
		swic.hw_igmpsnoop(ACTIVED);
		swic.hw_fastleave(ACTIVED);
	}
	else if(!strcmp(argv[1], ETHCMD_DISABLE)){
		swic.hw_igmpsnoop(DEACTIVED);
		swic.hw_fastleave(DEACTIVED);
	}
	else if(!strcmp(argv[1], ETHCMD_QUERY)){
		/*do nothing, just to display the igmpsnoop settings.*/
		;
	}
	else{
		printf("  %s %s\n\n", argv[0], HELP_IGMPSNOOP);
		retval=ETHCMD_ERR;
	}
	printf("Igmpsnoop function is :%s\n", (swic.hw_igmpsnoop(QUERY)?ETHCMD_ENABLE:ETHCMD_DISABLE));
	return retval;
}/*end doIgmpsnoop*/
/*___________________________________________________________________
**      function name: doHelp
**      descriptions:
**        Ethcmd helpful message sub ci-cmd.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments 
**         p:function pointer
**         
**      global:
**           None
**             
**      return:
**         0:succuces
**         -1:fail
**	    
**      call:
**           cmdHelp
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
static int doHelp(int argc, char *argv[], void *p){
	cmdHelp();
	return 0;
}/*end doHelp*/
/*___________________________________________________________________
**      function name: doVersion
**      descriptions:
**        Ethcmd show the ethcmd version.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments 
**         p:function pointer
**         
**      global:
**           None
**             
**      return:
**         0:succuces
**         -1:fail
**	    
**      call:
**           cmdHelp
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
static int doVersion(int argc, char *argv[], void *p){
	printf("\r\nethcmd version: %s.\n", MODULE_VERSION_ETHCMD);
	showSwicName();
	return 0;
}/*end doVersion*/

#ifdef ETHCMD_DBG
/*___________________________________________________________________
**      function name: initEthcmdDbg
**      descriptions:
**        Debug the ethcmd ci-cmd, it's used the /var/tmp/ethdbg files exist to 
**        show debug message ,otherwise is not show debug message.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments 
**         p:function pointer
**         
**      global:
**           ethcmdDbg
**             
**      return:
**         None
**	    
**      call:
**           None
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
void initEthcmdDbg(){
	struct stat fd_st;
	if(stat(ETHCMDDBG_FILE,&fd_st)==0){
		ethcmdDbg=1;/*Enable ethcmd debug message*/
	}
	else{
		ethcmdDbg=0;/*Disable ethcmd debug message*/
	}
}/*end initEthcmdDbg*/
/*___________________________________________________________________
**      function name: doEthcmdDbg
**      descriptions:
**        Debug the ethcmd ci-cmd,show the debug flag is enabled or not.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments 
**         p:function pointer
**         
**      global:
**           ethcmdDbg
**             
**      return:
**         0:succuces
**         -1:fail
**	    
**      call:
**           None
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
int doEthcmdDbg(int argc, char *argv[], void *p){
	printf("\r\nethcmdDbg: %s.\n", ((ethcmdDbg)?"on":"off"));
	return 0;
}/*end doEthcmdDbg*/
#endif
/*___________________________________________________________________
**      function name: doTCMiiRead
**      descriptions:
**        Basic command to read the ethernet mii register.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments 
**         p:function pointer
**         
**      global:
**           None
**             
**      return:
**         0:succuces
**         -1:fail
**	    
**      call:
**           getSwcfg
**           mdio_read
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
static int doTCMiiRead(int argc, char *argv[], void *p){
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();
	printf("*PhyAddr=%d Reg=%d value:%x\r\n", atoi(argv[1]) , atoi(argv[2]), \
		mdio_read(swcfg->skfd, swcfg->ifr, atoi(argv[1]), atoi(argv[2])));
	return 0;
}/*end doTCMiiRead*/

#if defined(TCSUPPORT_CPU_RT65168)
/*___________________________________________________________________
**      function name: doTCMiiBPRead
**      descriptions:
**        Basic command to read the ethernet mii register in bypass mode.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments
**         p:function pointer
**
**      global:
**           None
**
**      return:
**         0:succuces
**         -1:fail
**
**      call:
**           getSwcfg
**           mdio_read
**
**      revision:
**      1. PTChen 2013/01/30
**_______________________________________________________________
*/
static int doTCMiiBPRead(int argc, char *argv[], void *p){
	printf("A1\n");
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();
	printf("A2\n");
	printf("*PhyAddr=%d Reg=%d value:%x\r\n", atoi(argv[1]) , atoi(argv[2]), \
		gsw_phy_bp_read(swcfg->skfd, swcfg->ifr, atoi(argv[1]), atoi(argv[2])));
	return 0;
}/*end doTCMiiBPRead*/
#endif

//MTK120625 ///YM start
/*___________________________________________________________________
**      function name: doTCMiiRead2
**      descriptions:
**        Basic command to read the ethernet mii register.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments
**         p:function pointer
**
**      global:
**           None
**
**      return:
**         0:succuces
**         -1:fail
**
**      call:
**           getSwcfg
**           mdio_read
**
**      revision:
**      1.
**_______________________________________________________________
*/
static int doTCMiiRead2(int argc, char *argv[], void *p){
	struct swconfig *swcfg = NULL;
	int ext_switch, phyAddr, reg, value;

	swcfg = getSwcfg();
	ext_switch = atoi(argv[1]);
	phyAddr = atoi(argv[2]);
	reg = atoi(argv[3]);

	value = mdio_read2(swcfg->skfd, swcfg->ifr, ext_switch, phyAddr, reg);
	printf("*PhyAddr=%d Reg=%d value:%x (ext_switch:%d)\r\n", phyAddr, reg, value, ext_switch);

	return 0;
}/*end doTCMiiRead2*/
//MTK120625 ///YM end

/*___________________________________________________________________
**      function name: doTCMiiWrite
**      descriptions:
**        Basic command to write the ethernet mii register.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments 
**         p:function pointer
**         
**      global:
**           None
**             
**      return:
**         0:succuces
**         -1:fail
**	    
**      call:
**           getSwcfg
**           mdio_write
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
static int doTCMiiWrite(int argc, char *argv[], void *p){
	struct swconfig *swcfg=NULL;
	u16 value=0;
	swcfg=getSwcfg();
	
	value=strtoul((const char*)(argv[3]), (char **)NULL, 16);
	mdio_write(swcfg->skfd, swcfg->ifr, atoi(argv[1]), atoi(argv[2]),value);
	printf("*PhyAddr=%d Reg=%d value:%x\r\n", atoi(argv[1]) , atoi(argv[2]), value);
	return 0;
}/*end doTCMiiWrite*/


#if defined(TCSUPPORT_CPU_RT65168)
/*___________________________________________________________________
**      function name: doTCMiiBPWrite
**      descriptions:
**        Basic command to write the ethernet mii register in bypass mode.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments
**         p:function pointer
**
**      global:
**           None
**
**      return:
**         0:succuces
**         -1:fail
**
**      call:
**           getSwcfg
**           mdio_write
**
**      revision:
**      1. PTChen 2013/01/30
**_______________________________________________________________
*/
static int doTCMiiBPWrite(int argc, char *argv[], void *p){
	struct swconfig *swcfg=NULL;
	u16 value=0;
	swcfg=getSwcfg();

	value=strtoul((const char*)(argv[3]), (char **)NULL, 16);
	gsw_phy_bp_write(swcfg->skfd, swcfg->ifr, atoi(argv[1]), atoi(argv[2]),value);
	printf("*PhyAddr=%d Reg=%d value:%x\r\n", atoi(argv[1]) , atoi(argv[2]), value);
	return 0;
}/*end doTCMiiBPWrite*/
#endif

//MTK120625 ///YM start
/*___________________________________________________________________
**      function name: doTCMiiWrite2
**      descriptions:
**        Basic command to write the ethernet mii register.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments
**         p:function pointer
**
**      global:
**           None
**
**      return:
**         0:succuces
**         -1:fail
**
**      call:
**           getSwcfg
**           mdio_write
**
**      revision:
**      1.
**_______________________________________________________________
*/
static int doTCMiiWrite2(int argc, char *argv[], void *p){
	struct swconfig *swcfg = NULL;
	int ext_switch, phyAddr, reg, value;

	swcfg = getSwcfg();
	ext_switch = atoi(argv[1]);
	phyAddr = atoi(argv[2]);
	reg = atoi(argv[3]);
	value = strtoul((const char*)(argv[4]), (char **)NULL, 16);

	mdio_write2(swcfg->skfd, swcfg->ifr, ext_switch, phyAddr, reg, value);
	printf("*PhyAddr=%d Reg=%d value:%x (ext_switch:%d)\r\n", phyAddr, reg, value, ext_switch);

	return 0;
}/*end doTCMiiWrite2*/
//MTK120625 ///YM end

#ifdef MT7530_PHY_VERIFY
/*___________________________________________________________________
**      function name: doTCMiiRead45
**      descriptions:
**        Basic command to read the ethernet mii register.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments
**         p:function pointer
**
**      global:
**           None
**
**      return:
**         0:succuces
**         -1:fail
**
**      call:
**           getSwcfg
**           mdio_read
**
**      revision:
**      1. WPLin 2012/08/30
**_______________________________________________________________
*/
static int doTCMiiRead45(int argc, char *argv[], void *p){
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	printf("*portAddr:0x%02x devAddr:0x%02x regAddr:0x%04x value:0x%04x\r\n", atoi(argv[1]) , atoi(argv[2]), atoi(argv[3]), \
		mdio_read45(swcfg->skfd, swcfg->ifr, atoi(argv[1]), atoi(argv[2]), atoi(argv[3])));
	return 0;
}/*end doTCMiiRead45*/

/*___________________________________________________________________
**      function name: doTCMiiWrite45
**      descriptions:
**        Basic command to write the ethernet mii register.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments
**         p:function pointer
**
**      global:
**           None
**
**      return:
**         0:succuces
**         -1:fail
**
**      call:
**           getSwcfg
**           mdio_write
**
**      revision:
**      1. WPLin 2012/08/30
**_______________________________________________________________
*/
static int doTCMiiWrite45(int argc, char *argv[], void *p){
	struct swconfig *swcfg=NULL;
	u16 value=0;
	swcfg=getSwcfg();

	value=strtoul((const char*)(argv[4]), (char **)NULL, 16);
	mdio_write45(swcfg->skfd, swcfg->ifr, atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), value);
	printf("*portAddr:0x%02x devAddr:0x%02x regAddr:0x%04x value:0x%04x\r\n", atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), value);
	return 0;
}/*end doTCMiiWrite45*/
#endif

/*___________________________________________________________________
**      function name: doGSWPhyRead
**      descriptions:
**        Basic command to read the ethernet mii register.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments 
**         p:function pointer
**
**      global:
**           None
**
**      return:
**         0:succuces
**         -1:fail
**
**      call:
**           getSwcfg
**           mdio_read
**
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
static int doGSWPhyRead(int argc, char *argv[], void *p){
	struct swconfig *swcfg=NULL;
	u32 phy_addr;
	u32 phy_reg;
	swcfg=getSwcfg();

	phy_addr=strtoul((const char*)(argv[1]), (char **)NULL, 16);
	phy_reg=strtoul((const char*)(argv[2]), (char **)NULL, 16);
	printf("*phy_addr=%08lx phy_reg=%08lx value:%08lx\r\n", phy_addr, phy_reg,
		gsw_phy_read(swcfg->skfd, swcfg->ifr, phy_addr, phy_reg));
	return 0;
}/*end doTCMiiRead*/

/*___________________________________________________________________
**      function name: doGSWPhyWrite
**      descriptions:
**        Basic command to write the ethernet mii register.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments
**         p:function pointer
**
**      global:
**           None
**
**      return:
**         0:succuces
**         -1:fail
**
**      call:
**           getSwcfg
**           mdio_write
**
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
static int doGSWPhyWrite(int argc, char *argv[], void *p){
	struct swconfig *swcfg=NULL;
	u32 phy_addr;
	u32 phy_reg=0;
	u32 value;
	swcfg=getSwcfg();

	phy_addr=strtoul((const char*)(argv[1]), (char **)NULL, 16);
	phy_reg=strtoul((const char*)(argv[2]), (char **)NULL, 16);
	value=strtoul((const char*)(argv[3]), (char **)NULL, 16);
	gsw_phy_write(swcfg->skfd, swcfg->ifr, phy_addr, phy_reg, value);
	printf("*phy_addr=%08lx phy_reg=%08lx value:%08lx\r\n", phy_addr, phy_reg, value);
	return 0;
}/*end doGSWWrite*/

/*___________________________________________________________________
**      function name: doGSWRead
**      descriptions:
**        Basic command to read the ethernet mii register.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments
**         p:function pointer
**
**      global:
**           None
**
**      return:
**         0:succuces
**         -1:fail
**	    
**      call:
**           getSwcfg
**           mdio_read
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
static int doGSWRead(int argc, char *argv[], void *p){
	struct swconfig *swcfg=NULL;
	u32 offset;
	swcfg=getSwcfg();

	offset=strtoul((const char*)(argv[1]), (char **)NULL, 16);
	printf("*reg=%08lx value:%08lx\r\n", offset, 
		gsw_reg_read(swcfg->skfd, swcfg->ifr, offset));
	return 0;
}/*end doTCMiiRead*/

//MTK120625 ///YM start
/*___________________________________________________________________
**      function name: doGSWRead2
**      descriptions:
**        Basic command to read the ethernet mii register.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments
**         p:function pointer
**
**      global:
**           None
**
**      return:
**         0:succuces
**         -1:fail
**
**      call:
**           getSwcfg
**           mdio_read
**
**      revision:
**      1.
**_______________________________________________________________
*/
static int doGSWRead2(int argc, char *argv[], void *p){
	struct swconfig *swcfg=NULL;
	u32 ext_switch, reg, value;

	swcfg = getSwcfg();
	ext_switch = atoi(argv[1]);
	reg = strtoul((const char*)(argv[2]), (char **)NULL, 16);

	value = gsw_reg_read2(swcfg->skfd, swcfg->ifr, ext_switch, reg);
	printf("*reg=%08lx value:%08lx (ext_switch:%d)\r\n", reg, value, ext_switch);

	return 0;
}/*end doTCMiiRead2*/
//MTK120625 ///YM end

/*___________________________________________________________________
**      function name: doGSWWrite
**      descriptions:
**        Basic command to write the ethernet mii register.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments 
**         p:function pointer
**         
**      global:
**           None
**             
**      return:
**         0:succuces
**         -1:fail
**	    
**      call:
**           getSwcfg
**           mdio_write
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
static int doGSWWrite(int argc, char *argv[], void *p){
	struct swconfig *swcfg=NULL;
	u32 offset;
	u32 value=0;
	swcfg=getSwcfg();
	
	offset=strtoul((const char*)(argv[1]), (char **)NULL, 16);
	value=strtoul((const char*)(argv[2]), (char **)NULL, 16);
	gsw_reg_write(swcfg->skfd, swcfg->ifr, offset, value);
	printf("*reg=%08lx value:%08lx\r\n", offset, value);
	return 0;
}/*end doGSWWrite*/

//MTK120625 ///YM start
/*___________________________________________________________________
**      function name: doGSWWrite2
**      descriptions:
**        Basic command to write the ethernet mii register.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments
**         p:function pointer
**
**      global:
**           None
**
**      return:
**         0:succuces
**         -1:fail
**
**      call:
**           getSwcfg
**           mdio_write
**
**      revision:
**      1.
**_______________________________________________________________
*/
static int doGSWWrite2(int argc, char *argv[], void *p){
	struct swconfig *swcfg=NULL;
	u32 ext_switch, reg, value;

	swcfg = getSwcfg();
	ext_switch = atoi(argv[1]);
	reg = strtoul((const char*)(argv[2]), (char **)NULL, 16);
	value = strtoul((const char*)(argv[3]), (char **)NULL, 16);

	gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, value);
	printf("*reg=%08lx value:%08lx (ext_switch:%d)\r\n", reg, value, ext_switch);

	return 0;
}/*end doGSWWrite2*/
//MTK120625 ///YM end

/*___________________________________________________________________
**      function name: doGSWRegTest
**      descriptions:
**        Basic command to read the ethernet mii register.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments
**         p:function pointer
**
**      global:
**           None
**
**      return:
**         0:succuces
**         -1:fail
**
**      call:
**           getSwcfg
**           mdio_read
**
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
static int doGSWRegTest(int argc, char *argv[], void *p){
	struct swconfig *swcfg=NULL;
	u32 reg, count, mask, r_data, w_data, i, j;
	u32 pattern[ PATTERN_NUM ] = {0xffffffff, 0x00000000, 0x5a5a5a5a, 0xa5a5a5a5};
	swcfg=getSwcfg();

	reg=strtoul((const char*)(argv[1]), (char **)NULL, 16);
	count=strtoul((const char*)(argv[2]), (char **)NULL, 16);
	mask=strtoul((const char*)(argv[3]), (char **)NULL, 16);

	printf("reg: %x, count: %x, mask: %x\n", reg, count, mask);
	w_data = 0;
	for (i=0; i<count; i++){
		for (j=0; j<PATTERN_NUM; j++) {
//			printf("round %x\n", index);
			w_data = pattern[j];
//			printf("write reg: %x, value: %x\n", reg, w_data);
			gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, w_data);
			r_data = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
//			printf("read reg: %x, value: %x\n", reg, r_data);
			if ((w_data&mask) != (r_data&mask))
				break;
		}
		if (j != PATTERN_NUM)
			break;
	}

	if (i==count && j==PATTERN_NUM){
		printf("# switch reg %x test done.\r\n", reg);
	} else {
		printf("# Register %x Read/Write Test Error!\n", reg);
		printf("Wrtie Data: %x\n", w_data);
		printf("Mask: %x\n", mask);
		printf("Read Data: %x\n", r_data);

		r_data = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
		printf("read again\n");
		printf("# Register %x Read/Write Test Error!\n", reg);
		printf("Wrtie Data: %x\n", w_data);
		printf("Mask: %x\n", mask);
		printf("Read Data: %x\n", r_data);

		r_data = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
		printf("read again\n");
		printf("# Register %x Read/Write Test Error!\n", reg);
		printf("Wrtie Data: %x\n", w_data);
		printf("Mask: %x\n", mask);
		printf("Read Data: %x\n", r_data);
	}

	return 0;
}/*end doGSWRegTest*/

//#if defined(SQA_VERIFY)

//MTK20120829_MT7530_1588pkt_generation, Start[
/*___________________________________________________________________
**      function name: doGen1588Pkt
**      descriptions:
**         Generate 1588 pkt.
**
**      parameters:
**
**      global:
**
**      return:
**         0:succuces
**         -1:fail
**
**      call:
**           subcmd
**
**      revision:
**      1. Frank 2012/08/29
**_______________________________________________________________
*/
static int doGen1588Pkt(int argc, char *argv[], void *p){
	u32 type;
	char cmd[32];

	type=atoi(argv[1]);

	sprintf(cmd, "echo %d > /proc/tc3162/gen_1588_pkt\n", type);

	printf("%s\n", cmd);
	system(cmd);

	return 0;
}
//MTK20120829_MT7530_1588pkt_generation, ]End

/*QOS related ci-cmds*/
/*___________________________________________________________________
**      function name: doQos
**      descriptions:
**         Register ARL subcmd.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments
**         p:function pointer
**
**      global:
**           ethCmds
**
**      return:
**         0:succuces
**         -1:fail
**
**      call:
**           subcmd
**
**      revision:
**      1. Frank 2012/03/20
**_______________________________________________________________
*/
static int doQos(int argc, char *argv[], void *p){
	return subcmd(qosCmds, argc, argv, p);
}/*end doVlan*/

static int doQosType(int argc, char *argv[], void *p){
	u8 type=0, port, queue;
	u32 value, reg;
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	type=atoi(argv[1]);

	if((type!=0 && type!=1)){
		printf(HELP_QOS_TYPE);
		return -1;
	}

	printf("\r\nswitch qos type: %s.\n", ((type)?"sp":"wrr"));

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
		if (type) {	/* strict priority */
			for (queue=0; queue<8; queue++){
				/* MT7530 total 7 port */
				for (port=0; port<7; port++){
					reg = GSW_MMSCR0_Q(queue) + 0x100*port;
					value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
				//	value |= (1<<15);
				    value &= (0x0);
					value |= (1<<31);
					
					gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
				}

				/* MT7530 total 7 port */
				for (port=0; port<7; port++){
					reg = GSW_MMSCR1_Q(queue) + 0x100*port;
					value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
					//value |= (1<<31);
					value &= (0x0);
					gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
				}
			}
		} else {
			for (queue=0; queue<8; queue++){
				for (port=0; port<7; port++){
					reg = GSW_MMSCR0_Q(queue) + 0x100*port;
					value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
					value &= (0xfffff080);
					value |= (1<<15);
					gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
				}

				for (port=0; port<7; port++){
					reg = GSW_MMSCR1_Q(queue) + 0x100*port;
					value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
					value &= (~(1<<31));
					gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
				}
			}
		}
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}

	return 0;
}

static int doQosBase(int argc, char *argv[], void *p){
	u8 base=0, port;
	u32 value;
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	base=atoi(argv[1]);

#ifdef MT7530_UPW_REG_UPDATE	//20121011 YM_Chan request for MT7530 E2 h/w register spec modification
	if((base<0 || base>6))
#else
	if((base<0 || base>5))
#endif
	{
		printf(HELP_QOS_BASE);
		return -1;
	}

#ifdef MT7530_UPW_REG_UPDATE	//20121011 YM_Chan request for MT7530 E2 h/w register spec modification
	printf("\r\nswitch qos base : %d. (port-based:0, tag-based:1, dscp-based:2, acl-based:3, arl-based:4, stag-based:5)\n", base);
#else
	printf("\r\nswitch qos base : %d. (port-based:0, tag-based:1, dscp-based:2, acl-based:3, arl-based:4)\n", base);
#endif

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
		value = gsw_reg_read(swcfg->skfd, swcfg->ifr, GSW_UPW);
#ifdef MT7530_UPW_REG_UPDATE	//20121011 YM_Chan request for MT7530 E2 h/w register spec modification
		value &= (~((0x7<<0) | (0x7<<4) | (0x7<<8) | (0x7<<12) | (0x7<<16) | (0x7<<20)));
		switch (base)
		{
			case 0 : /* port-based 0x0044[18:16] */
				value |= ((0x2<<0) | (0x2<<4) | (0x2<<8) | (0x2<<12) | (0x7<<16) | (0x2<<20));
				break;
			case 1 : /* tagged-based 0x0044[10:8] */
				value |= ((0x2<<0) | (0x2<<4) | (0x7<<8) | (0x2<<12) | (0x2<<16) | (0x2<<20));
				break;
			case 2 : /* DSCP-based 0x0044[14:12] */
				value |= ((0x2<<0) | (0x2<<4) | (0x2<<8) | (0x7<<12) | (0x2<<16) | (0x2<<20));
				break;
			case 3 : /* acl-based 0x0044[2:0] */
				value |= ((0x7<<0) | (0x2<<4) | (0x2<<8) | (0x2<<12) | (0x2<<16) | (0x2<<20));
				break;
			case 4 : /* arl-based 0x0044[22:20] */
				value |= ((0x2<<0) | (0x2<<4) | (0x2<<8) | (0x2<<12) | (0x2<<16) | (0x7<<20));
				break;
			case 5 : /* stag-based 0x0044[6:4] */
				value |= ((0x2<<0) | (0x7<<4) | (0x2<<8) | (0x2<<12) | (0x2<<16) | (0x2<<20));
				break;
			default :
				break;
#else
		value &= (~((0x7<<0) | (0x7<<4) | (0x7<<8) | (0x7<<12) | (0x7<<16)));
		switch (base)
		{
			case 0 : /* port-based 0x0044[14:12] */
				value |= ((0x2<<0) | (0x2<<4) | (0x2<<8) | (0x7<<12) | (0x2<<16));
				break;
			case 1 : /* tagged-based 0x0044[6:4] */
				value |= ((0x2<<0) | (0x7<<4) | (0x2<<8) | (0x2<<12) | (0x2<<16));
				break;
			case 2 : /* DSCP-based 0x0044[10:8] */
				value |= ((0x2<<0) | (0x2<<4) | (0x7<<8) | (0x2<<12) | (0x2<<16));
				break;
			case 3 : /* acl-based 0x0044[2:0] */
				value |= ((0x7<<0) | (0x2<<4) | (0x2<<8) | (0x2<<12) | (0x2<<16));
				break;
			case 4 : /* arl-based 0x0044[18:16] */
				value |= ((0x2<<0) | (0x2<<4) | (0x2<<8) | (0x2<<12) | (0x7<<16));
				break;
			default :
				break;
#endif
		}
		gsw_reg_write(swcfg->skfd, swcfg->ifr, GSW_UPW, value);
		printf("write reg: %x, value: %x\n", GSW_UPW, value);
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}

	return 0;
}

static int doQosSetPrioQmap(int argc, char *argv[], void *p){
	u8 prio, queue, pem_n;
	u32 value;
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	prio=atoi(argv[1]);
	queue=atoi(argv[2]);

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
		if ((prio<0 || prio>7) || (queue<0 || queue>7)) {	/* MT7530 total 7 port */
			printf(HELP_QOS_PRIO_QMAP);
			return -1;
		}

		pem_n = prio/2;
		value = gsw_reg_read(swcfg->skfd, swcfg->ifr, GSW_PEM(pem_n));
		if (prio%2){
			value &= (~(0x7<<24));
			value |= ((queue&0x7)<<24);
		} else {
			value &= (~(0x7<<8));
			value |= ((queue&0x7)<<8);
		}
		gsw_reg_write(swcfg->skfd, swcfg->ifr, GSW_PEM(pem_n), value);
		printf("write reg: %x, value: %x\n", GSW_PEM(pem_n), value);
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}

	return 0;
}

static int doQosSetPrioTagmap(int argc, char *argv[], void *p){
	u8 prio, tag, pem_n;
	u32 value;
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	prio=atoi(argv[1]);
	tag=atoi(argv[2]);

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
		if ((prio<0 || prio>7) || (tag<0 || tag>7)) {	/* MT7530 total 7 port */
			printf(HELP_QOS_PRIO_QMAP);
			return -1;
		}

		pem_n = prio/2;
		value = gsw_reg_read(swcfg->skfd, swcfg->ifr, GSW_PEM(pem_n));
		if (prio%2){
			value &= (~(0x7<<27));
			value |= ((tag&0x7)<<27);
		} else {
			value &= (~(0x7<<11));
			value |= ((tag&0x7)<<11);
		}
		gsw_reg_write(swcfg->skfd, swcfg->ifr, GSW_PEM(pem_n), value);
		printf("write reg: %x, value: %x\n", GSW_PEM(pem_n), value);
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}

	return 0;
}

static int doQosSetPrioDSCPmap(int argc, char *argv[], void *p){
	u8 prio, dscp, pem_n;
	u32 value;
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	prio=atoi(argv[1]);
	dscp=atoi(argv[2]);

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
		if ((prio<0 || prio>7) || (dscp<0 || dscp>63)){
			printf(HELP_QOS_PRIO_QMAP);
			return -1;
		}

		pem_n = prio/2;
		value = gsw_reg_read(swcfg->skfd, swcfg->ifr, GSW_PEM(pem_n));
		if (prio%2){
			value &= (~(0x3F<<16));
			value |= ((dscp&0x3F)<<16);
		} else {
			value &= (~0x3F);
			value |= (dscp&0x3F);
		}
		gsw_reg_write(swcfg->skfd, swcfg->ifr, GSW_PEM(pem_n), value);
		printf("write reg: %x, value: %x\n", GSW_PEM(pem_n), value);
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}

	return 0;
}

#if 0
static int doQosSetVpriQmap(int argc, char *argv[], void *p){
	u8 prio, queue, pem_n;
	u32 value;
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	prio=atoi(argv[1]);
	queue=atoi(argv[2]);

	/* rt62806 total 7 port */
	if ((prio<0 || prio>7) || (queue<0 || queue>7)){
		printf(HELP_QOS_VPRI_QMAP);
		return -1;
	}

	pem_n = prio/2;
	value = gsw_reg_read(swcfg->skfd, swcfg->ifr, GSW_PEM(pem_n));
	if (prio%2){
		value &= (~(0x7<<27));
		value |= ((queue&0x7)<<27);
	} else {
		value &= (~(0x7<<11));
		value |= ((queue&0x7)<<11);
	}
	gsw_reg_write(swcfg->skfd, swcfg->ifr, GSW_PEM(pem_n), value);

	return 0;
}
#endif

static int doQosSetPortPrio(int argc, char *argv[], void *p){
	u8 port, prio;
	u32 value;
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	port=atoi(argv[1]);
	prio=atoi(argv[2]);

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
		if ((port<0 || port>=7) || (prio<0 || prio>7)) {	/* MT7530 total 7 port */
			printf(HELP_QOS_PORT_PRIO);
			return -1;
		}

		value = gsw_reg_read(swcfg->skfd, swcfg->ifr, GSW_PCR(port));
		value &= (~(0x7<<24));
		value |= (prio<<24);
		gsw_reg_write(swcfg->skfd, swcfg->ifr, GSW_PCR(port), value);
		printf("write reg: %x, value: %x\n", GSW_PCR(port), value);
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}

	return 0;
}

static int doQosSetPortWeight(int argc, char *argv[], void *p){
	u8 port, queue, weight[8], i;
	u32 reg, value;
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	port=atoi(argv[1]);

	for (i=0; i<8; i++){
		weight[i]=atoi(argv[i+2]);
	}

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
		if (port<0 || port>6) {	/* MT7530 total 7 port */
			printf(HELP_QOS_PORT_WEIGHT);
			return -1;
		}

		for (i=0; i<8; i++){
			if (weight[i]<1 || weight[i]>16){
				printf(HELP_QOS_PORT_WEIGHT);
				return -1;
			}
		}

		printf("port: %x, q0: %x, q1: %x, q2: %x, q3: %x, q4: %x, q5: %x, q6: %x, q7: %x\n",
			port, weight[0], weight[1], weight[2], weight[3], weight[4], weight[5], weight[6], weight[7]);

		for (queue=0; queue<8; queue++){
			reg = GSW_MMSCR1_Q(queue) + 0x100*port;
			value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
			value &= (~(0xf<<24));
			value |= (((weight[queue]-1)&0xf)<<24);
			gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
		}
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}

	return 0;
}

static int doQosSetDSCPPrio(int argc, char *argv[], void *p){
	u8 prio, dscp, pim_n, pim_offset;
	u32 reg, value;
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	dscp=atoi(argv[1]);
	prio=atoi(argv[2]);

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
		if ((dscp<0 || dscp>63) || (prio<0 || prio>7)) {	/* MT7530 total 7 port */
			printf(HELP_QOS_DSCP_PRIO);
			return -1;
		}

		pim_n = dscp/10;	// every PIM register contain 10 dscp settings
		pim_offset = (dscp - pim_n*10) * 3;	// every dscp priority setting use 3 bits
		reg = 0x0058 + pim_n*4;

		value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
		value &= (~(0x7<<pim_offset));
		value |= ((prio&0x7)<<pim_offset);
		gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
		printf("write reg: %x, value: %x\n", reg, value);
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}

	return 0;
}

/*ARL related ci-cmds*/
/*___________________________________________________________________
**      function name: doArl
**      descriptions:
**         Register ARL subcmd.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments 
**         p:function pointer
**         
**      global:
**           ethCmds
**             
**      return:
**         0:succuces
**         -1:fail
**	    
**      call:
**           subcmd
**      
**      revision:
**      1. Frank 2012/03/20
**_______________________________________________________________
*/
static int doArl(int argc, char *argv[], void *p){
	return subcmd(arlCmds, argc, argv, p);
}/*end doVlan*/

static int doArlChkLen(int argc, char *argv[], void *p){
	u8 lenchk_en=0;
	u32 value, reg; 

	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();
	
	lenchk_en=atoi(argv[1]);

	printf("lenchk_en: %x\n", lenchk_en);

	/*Check the input parameters is right or not.*/
	if((lenchk_en!=0 && lenchk_en!=1)){
		printf(HELP_ARL_L2LEN_CHK);
		return -1;
	}

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
		reg = 0x0c;
		value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
		value &= (~(1<<4));
		value |= (lenchk_en<<4);

		printf("write reg: %x, value: %x\n", reg, value);
		gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}
	return 0;
}

static int doArlAging(int argc, char *argv[], void *p){
	u8 aging_en=0;
	u32 time=0, aging_cnt=0, aging_unit=0, value, reg;

	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();
	
	aging_en=atoi(argv[1]);
	time=atoi(argv[2]);
	printf("aging_en: %x, aging time: %x\n", aging_en, time);

	/*Check the input parameters is right or not.*/
	if((aging_en!=0 && aging_en!=1) || (time<=0 || time>65536)){
		printf(HELP_ARL_AGING);
		return -1;
	}

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
		reg = 0xa0;
		value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
		value &= (~(1<<20));
		if (!aging_en){
			value |= (1<<20);
		}

		aging_unit = (time/0x100) + 1;
		aging_cnt = (time/aging_unit);
		aging_unit--;
		aging_cnt--;

		value &= (0xfff00000);
		value |= ((aging_cnt<<12) | aging_unit);

		printf("aging_unit: %x, aging_cnt: %x\n", aging_unit, aging_cnt);
		printf("write reg: %x, value: %x\n", reg, value);

		gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}

	return 0;
}

/*___________________________________________________________________
**      function name: doArlMACTblAdd
**      descriptions:
**         Add Entry in MAC Address Table (Provide MAC Address and Destination Port Map).
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments
**         p:function pointer
**
**      global:
**           swic
**
**      return:
**         0:succuces
**         -1:fail
**
**      call:
**           subcmd
**
**      revision:
**      1. WPLin 2012/04/09
**_______________________________________________________________
*/
static int doArlMACTblAdd(int argc, char *argv[], void *p){
	u32 value, reg;
	u32 mac[6], destportmap, i, j;
	u32 leaky_en=0, eg_tag=0, usr_pri=0, sa_mir_en=0, sa_port_fw=0;
	char temp_str[3] = {0,0,0};

	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();


	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
		reg = REG_ATC_ADDR;
		while (1){	// wait until not busy
			value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
			if ((value & REG_ATC_BUSY_MASK) == 0){
				break;
			}
		}

		for (i = 0, j = 0; i < 6; i ++){
			temp_str[0] = ((const char*)(argv[1]))[j++];
			temp_str[1] = ((const char*)(argv[1]))[j++];
			mac[i] = strtoul(temp_str, (char **)NULL, 16);
		}

		printf("MAC address: %02X:%02X:%02X:%02X:%02X:%02X\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

		reg = REG_ATA1_ADDR;
		value = (mac[0] << 24) | (mac[1] << 16) | (mac[2] << 8) | mac[3];
		gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);

		reg = REG_ATA2_ADDR;
		value = (mac[4] << 24) | (mac[5] << 16);
		gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);

		destportmap=atoi(argv[2]);
		//Check the input parameters is right or not.

		printf("dest port map: %d\n", destportmap);
		if (destportmap > REG_ATWD_PORT_RELMASK){
			printf(HELP_ARL_MAC_TBL_ADD);
			return -1;
		}

		if (argc >= 4)
		{
			leaky_en=atoi(argv[3]) & REG_ATWD_LEAKY_EN_RELMASK;
			if (argc >= 5)
			{
				eg_tag=atoi(argv[4]) & REG_ATWD_EG_TAG_RELMASK;
				if (argc >= 6)
				{
					usr_pri=atoi(argv[5]) & REG_ATWD_USR_PRI_RELMASK;
					if (argc >= 7)
					{
						sa_mir_en=atoi(argv[6]) & REG_ATWD_SA_MIR_EN_RELMASK;
						if (argc >= 8)
						{
							sa_port_fw=atoi(argv[7]) & REG_ATWD_SA_PORT_FW_RELMASK;
						}
					}
				}
			}
		}

		// set other parameters
		reg = REG_ATWD_ADDR;
		value = 0;
		value |= (destportmap << REG_ATWD_PORT_OFFT);
		value |= (3 << REG_ATWD_STATUS_OFFT);		// 3 : Entry is static and won't aged out or changed by the hardware
		value |= (leaky_en << REG_ATWD_LEAKY_EN_OFFT) + (eg_tag << REG_ATWD_EG_TAG_OFFT) + (usr_pri << REG_ATWD_USR_PRI_OFFT);
		value |= (sa_mir_en << REG_ATWD_SA_MIR_EN_OFFT) + (sa_port_fw << REG_ATWD_SA_PORT_FW_OFFT);
		gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
		printf("*reg=%08lx value:%08lx\r\n", reg, value);

		// trigger
		reg = REG_ATC_ADDR;
		value = (1 << REG_ATC_BUSY_OFFT) + (0 << REG_ATC_AC_SAT_OFFT) + (1 << REG_ATC_AC_CMD_OFFT);	// 3'b001 : Write command (single entry)
		gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
		while (1){	// wait until not busy
			value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
			if ((value & REG_ATC_BUSY_MASK) == 0){
				break;
			}
		}
		printf("Add one static MAC Address entry OK.\n");
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}

	return 0;
}/*end doArlMACTblAdd*/

/*___________________________________________________________________
**      function name: doArlMACTblDisp
**      descriptions:
**         Display Entries in MAC Address Table.
**
**      parameters:
**         argc: The arguments number.
**         p:function pointer
**
**      global:
**           swic
**
**      return:
**         0:succuces
**      call:
**           subcmd
**
**      revision:
**      1. WPLin 2012/04/16
**      2. WPLin 2012/05/03
**_______________________________________________________________
*/
static int doArlMACTblDisp(int argc, char *argv[], void *p){
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	if ((swcfg->swic_id == MT7530) && (argc >= 2)){	// optional argv[1] : ext_switch
		if (atoi(argv[1]) == 255){
			doArlAddrTblDisp2(0, 0);	// 2'b00 : MAC Address Entry
			doArlAddrTblDisp2(1, 0);	// 2'b00 : MAC Address Entry
		}
		else{
			doArlAddrTblDisp2(atoi(argv[1]), 0);	// 2'b00 : MAC Address Entry
		}
	}
	else{
		doArlAddrTblDisp(0);	// 2'b00 : MAC Address Entry
	}

	return 0;
}/*end doArlMACTblDisp*/

/*___________________________________________________________________
**      function name: doArlMACTblClr
**      descriptions:
**         Clean All Entries in MAC Address Table.
**
**      parameters:
**         argc: The arguments number.
**         p:function pointer
**
**      global:
**           swic
**
**      return:
**         0:succuces
**      call:
**           subcmd
**
**      revision:
**      1. WPLin 2012/05/04
**_______________________________________________________________
*/
static int doArlMACTblClr(int argc, char *argv[], void *p){
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	if ((swcfg->swic_id == MT7530) && (argc >= 2)){	// optional argv[1] : ext_switch
		if (atoi(argv[1]) == 255){
			doArlAddrTblClr2(0, 0);		// 2'b00 : MAC Address Entry
			doArlAddrTblClr2(1, 0);		// 2'b00 : MAC Address Entry
		}
		else{
			doArlAddrTblClr2(atoi(argv[1]), 0);	// 2'b00 : MAC Address Entry
		}
	}
	else{
		doArlAddrTblClr(0);	// 2'b00 : MAC Address Entry
	}

	return 0;
}/*end doArlMACTblClr*/

/*___________________________________________________________________
**      function name: doArlDIPTblAdd
**      descriptions:
**         Add Entry in DIP Address Table (Provide DIP Address and Destination Port Map).
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments
**         p:function pointer
**
**      global:
**           swic
**
**      return:
**         0:succuces
**         -1:fail
**
**      call:
**           subcmd
**
**      revision:
**      1. WPLin 2012/05/03
**_______________________________________________________________
*/
static int doArlDIPTblAdd(int argc, char *argv[], void *p){
	u32 value, reg;
	u32 dip, destportmap, status, leaky_en, eg_tag, usr_pri, i;
	char *str_ptr;

	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
		reg = REG_ATC_ADDR;
		while (1){	// wait until not busy
			value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
			if ((value & REG_ATC_BUSY_MASK) == 0){
				break;
			}
		}

		str_ptr = (char*)(argv[1]);
		dip = 0;
		for (i = 0; i < 4; i ++)
		{
			dip = (dip << 8) | (strtoul(str_ptr, &str_ptr, 10) & 0xFF);
			if (*(str_ptr++) != '.') break;		// skip '.' and advance to next digits
		}
		printf("DIP address: %08X\n", dip);

		reg = REG_ATA1_ADDR;
		value = dip;
		gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);

		destportmap=atoi(argv[2]);
		status=atoi(argv[3]);
		leaky_en=atoi(argv[4]) & REG_ATWD_LEAKY_EN_RELMASK;
		eg_tag=atoi(argv[5]) & REG_ATWD_EG_TAG_RELMASK;
		usr_pri=atoi(argv[6]) & REG_ATWD_USR_PRI_RELMASK;
		//Check the input parameters is right or not.

		printf("dest port map: %d, status: %d\n", destportmap, status);
		if ((destportmap > REG_ATWD_PORT_RELMASK) || (status > REG_ATWD_STATUS_RELMASK)){
			printf(HELP_ARL_DIP_TBL_ADD);
			return -1;
		}

		// set other parameters
		reg = REG_ATWD_ADDR;
		value = 0;
		value |= (destportmap << REG_ATWD_PORT_OFFT);
		value |= (status << REG_ATWD_STATUS_OFFT);
		value |= (leaky_en << REG_ATWD_LEAKY_EN_OFFT) + (eg_tag << REG_ATWD_EG_TAG_OFFT) + (usr_pri << REG_ATWD_USR_PRI_OFFT);
		gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);

		// trigger
		reg = REG_ATC_ADDR;
		value = (1 << REG_ATC_BUSY_OFFT) + (1 << REG_ATC_AC_SAT_OFFT) + (1 << REG_ATC_AC_CMD_OFFT);	// 3'b001 : Write command (single entry)
		gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
		while (1){	// wait until not busy
			value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
			if ((value & REG_ATC_BUSY_MASK) == 0){
				break;
			}
		}
		printf("Add one static DIP Address entry OK.\n");
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}

	return 0;
}/*end doArlDIPTblAdd*/

/*___________________________________________________________________
**      function name: doArlDIPTblDisp
**      descriptions:
**         Display Entries in DIP Address Table.
**
**      parameters:
**         argc: The arguments number.
**         p:function pointer
**
**      global:
**           swic
**
**      return:
**         0:succuces
**      call:
**           subcmd
**
**      revision:
**      1. WPLin 2012/04/16
**      2. WPLin 2012/05/03
**_______________________________________________________________
*/
static int doArlDIPTblDisp(int argc, char *argv[], void *p){

	doArlAddrTblDisp(1);	// 2'b01 : DIP Address Entry

	return 0;
}/*end doArlDIPTblDisp*/

/*___________________________________________________________________
**      function name: doArlDIPTblClr
**      descriptions:
**         Clean All Entries in DIP Address Table.
**
**      parameters:
**         argc: The arguments number.
**         p:function pointer
**
**      global:
**           swic
**
**      return:
**         0:succuces
**      call:
**           subcmd
**
**      revision:
**      1. WPLin 2012/05/04
**_______________________________________________________________
*/
static int doArlDIPTblClr(int argc, char *argv[], void *p){

	doArlAddrTblClr(1);	// 2'b01 : DIP Address Entry

	return 0;
}/*end doArlDIPTblClr*/

/*___________________________________________________________________
**      function name: doArlSIPTblAdd
**      descriptions:
**         Add Entry in SIP Address Table (Provide SIP Address and Destination Port Map).
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments
**         p:function pointer
**
**      global:
**           swic
**
**      return:
**         0:succuces
**         -1:fail
**
**      call:
**           subcmd
**
**      revision:
**      1. WPLin 2012/05/03
**_______________________________________________________________
*/
static int doArlSIPTblAdd(int argc, char *argv[], void *p){
	u32 value, reg;
	u32 dip, sip, destportmap, status, i;
	char *str_ptr;

	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
		reg = REG_ATC_ADDR;
		while (1){	// wait until not busy
			value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
			if ((value & REG_ATC_BUSY_MASK) == 0){
				break;
			}
		}

		str_ptr = (char*)(argv[1]);
		dip = 0;
		for (i = 0; i < 4; i ++)
		{
			dip = (dip << 8) | (strtoul(str_ptr, &str_ptr, 10) & 0xFF);
			if (*(str_ptr++) != '.') break;		// skip '.' and advance to next digits
		}
		str_ptr = (char*)(argv[2]);
		sip = 0;
		for (i = 0; i < 4; i ++)
		{
			sip = (sip << 8) | (strtoul(str_ptr, &str_ptr, 10) & 0xFF);
			if (*(str_ptr++) != '.') break;		// skip '.' and advance to next digits
		}
		printf("DIP address: %08X, SIP address: %08X\n", dip, sip);

		reg = REG_ATA1_ADDR;
		value = dip;
		gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);

		reg = REG_ATA2_ADDR;
		value = sip;
		gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);

		destportmap=atoi(argv[3]);
		status=atoi(argv[4]);
		//Check the input parameters is right or not.

		printf("dest port map: %d, status: %d\n", destportmap, status);
		if ((destportmap > REG_ATWD_PORT_RELMASK) || (status > REG_ATWD_STATUS_RELMASK)){
			printf(HELP_ARL_SIP_TBL_ADD);
			return -1;
		}

		// set other parameters
		reg = REG_ATWD_ADDR;
		value = 0;
		value |= (destportmap << REG_ATWD_PORT_OFFT);
		value |= (status << REG_ATWD_STATUS_OFFT);
		gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);

		// trigger
		reg = REG_ATC_ADDR;
		value = (1 << REG_ATC_BUSY_OFFT) + (2 << REG_ATC_AC_SAT_OFFT) + (1 << REG_ATC_AC_CMD_OFFT);	// 3'b001 : Write command (single entry)
		gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
		while (1){	// wait until not busy
			value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
			if ((value & REG_ATC_BUSY_MASK) == 0){
				break;
			}
		}
		printf("Add one static SIP Address entry OK.\n");
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}

	return 0;
}/*end doArlSIPTblAdd*/

/*___________________________________________________________________
**      function name: doArlSIPTblDisp
**      descriptions:
**         Display Entries in SIP Address Table.
**
**      parameters:
**         argc: The arguments number.
**         p:function pointer
**
**      global:
**           swic
**
**      return:
**         0:succuces
**      call:
**           subcmd
**
**      revision:
**      1. WPLin 2012/05/03
**_______________________________________________________________
*/
static int doArlSIPTblDisp(int argc, char *argv[], void *p){

	doArlAddrTblDisp(2);	// 2'b10 : SIP Address Entry

	return 0;
}/*end doArlSIPTblDisp*/

/*___________________________________________________________________
**      function name: doArlSIPTblClr
**      descriptions:
**         Clean All Entries in SIP Address Table.
**
**      parameters:
**         argc: The arguments number.
**         p:function pointer
**
**      global:
**           swic
**
**      return:
**         0:succuces
**      call:
**           subcmd
**
**      revision:
**      1. WPLin 2012/05/04
**_______________________________________________________________
*/
static int doArlSIPTblClr(int argc, char *argv[], void *p){

	doArlAddrTblClr(2);	// 2'b10 : SIP Address Entry

	return 0;
}/*end doArlSIPTblClr*/

/*___________________________________________________________________
**      function name: doArlAddrTblDisp
**      descriptions:
**         Display Entries in seleced Address Table.
**
**      parameters:
**         ac_mat: 0(mac)/1(dip)/2(sip)/3(specific).
**
**      global:
**           swic
**
**      return:
**         0:succuces
**      call:
**           subcmd
**
**      revision:
**      1. WPLin 2012/04/16
**      2. WPLin 2012/05/03
**_______________________________________________________________
*/
static int doArlAddrTblDisp(int ac_mat){
	u32 value, reg;
	u32 addr1, addr2;

	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	if (ac_mat > 3){
		return 0;
	}

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
		reg = REG_ATC_ADDR;
		while (1){	// wait until not busy
			value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
			if ((value & REG_ATC_BUSY_MASK) == 0){
				break;
			}
		}

		// trigger 1st search
		reg = REG_ATC_ADDR;
		value = (1 << REG_ATC_BUSY_OFFT) + (ac_mat << REG_ATC_AC_MAT_OFFT) + (4 << REG_ATC_AC_CMD_OFFT);	// 3'b100 : Start search command (reset to 1st entry)
		gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
		while (1){	// wait until not busy
			value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
			if ((value & REG_ATC_BUSY_MASK) == 0){
				break;
			}
		}

		while (1){
			if (!(value & REG_AT_SRCH_HIT_MASK) || (value & REG_AT_SRCH_END_MASK))
			{
				printf("...Address Table end...\n", value);
				break;
			}

			reg = REG_TSRA1_ADDR;
			addr1 = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);

			reg = REG_TSRA2_ADDR;
			addr2 = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);

			// other parameters
			reg = REG_ATRD_ADDR;
			value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);

			// ac_mat: 0(mac)/1(dip)/2(sip)
			if (ac_mat == 0){
				printf("MAC %08X%04X :", addr1, (addr2 >> 16));
				printf(" TIMER:%d, SA_PORT_FW:%d, SA_MIR_EN:%d, USER_PRI:%d,\n",
				  (value>>24)&0xFF, (value>>20)&7, (value>>19)&1, (value>>16)&7);
				printf("                   EG_TAG:%d, LEAKY_EN:%d, PORT:%X, STATUS:%d, TYPE:%d\n",
				  (value>>13)&7, (value>>12)&1, (value>>4)&0xFF, (value>>2)&3, (value)&3);
			} else if (ac_mat == 1){
				printf("DIP %08X :", addr1);
				printf(" tmr:%d, flag:%X, cnt%X", (addr2>>24)&0x0F, (addr2>>16)&0x0F, addr2&0xFF);
				printf(" USER_PRI:%d, EG_TAG:%d, LEAKY_EN:%d, PORT:%X, STATUS:%d, TYPE:%d\n",
				  (value>>16)&7, (value>>13)&7, (value>>12)&1, (value>>4)&0xFF, (value>>2)&3, (value)&3);
			} else if (ac_mat == 2){
				printf("DIP %08X, SIP %08X :", addr1, addr2);
				printf(" PORT:%X, STATUS:%d, TYPE:%d\n",
				  (value>>4)&0xFF, (value>>2)&3, (value)&3);
			}

			// trigger next
			reg = REG_ATC_ADDR;
			value = (1 << REG_ATC_BUSY_OFFT) + (ac_mat << REG_ATC_AC_MAT_OFFT) + (5 << REG_ATC_AC_CMD_OFFT);	// 3'b101 : Next search command (next entry)
			gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
			while (1){	// wait until not busy
				value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
				if ((value & REG_ATC_BUSY_MASK) == 0){
					break;
				}
			}
		}
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}

	return 0;
}/*end doArlAddrTblDisp*/

/*___________________________________________________________________
**      function name: doArlAddrTblDisp2
**      descriptions:
**         Display Entries in seleced Address Table.
**
**      parameters:
**         u32 ext_switch: 0(int)/1(ext).
**         ac_mat: 0(mac)/1(dip)/2(sip)/3(specific).
**
**      global:
**           swic
**
**      return:
**         0:succuces
**      call:
**           subcmd
**
**      revision:
**      1. WPLin 2012/11/16
**_______________________________________________________________
*/
static int doArlAddrTblDisp2(u32 ext_switch, int ac_mat){
	u32 value, reg;
	u32 addr1, addr2;

	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	if (ac_mat > 3){
		return 0;
	}
	printf("(ext_switch:%d)\r\n", ext_switch);

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
		reg = REG_ATC_ADDR;
		while (1){	// wait until not busy
			value = gsw_reg_read2(swcfg->skfd, swcfg->ifr, ext_switch, reg);
			if ((value & REG_ATC_BUSY_MASK) == 0){
				break;
			}
		}

		// trigger 1st search
		reg = REG_ATC_ADDR;
		value = (1 << REG_ATC_BUSY_OFFT) + (ac_mat << REG_ATC_AC_MAT_OFFT) + (4 << REG_ATC_AC_CMD_OFFT);	// 3'b100 : Start search command (reset to 1st entry)
		gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, value);
		while (1){	// wait until not busy
			value = gsw_reg_read2(swcfg->skfd, swcfg->ifr, ext_switch, reg);
			if ((value & REG_ATC_BUSY_MASK) == 0){
				break;
			}
		}

		while (1){
			if (!(value & REG_AT_SRCH_HIT_MASK) || (value & REG_AT_SRCH_END_MASK))
			{
				printf("...Address Table end...\n", value);
				break;
			}

			reg = REG_TSRA1_ADDR;
			addr1 = gsw_reg_read2(swcfg->skfd, swcfg->ifr, ext_switch, reg);

			reg = REG_TSRA2_ADDR;
			addr2 = gsw_reg_read2(swcfg->skfd, swcfg->ifr, ext_switch, reg);

			// other parameters
			reg = REG_ATRD_ADDR;
			value = gsw_reg_read2(swcfg->skfd, swcfg->ifr, ext_switch, reg);

			// ac_mat: 0(mac)/1(dip)/2(sip)
			if (ac_mat == 0){
				printf("MAC %08X%04X :", addr1, (addr2 >> 16));
				printf(" TIMER:%d, SA_PORT_FW:%d, SA_MIR_EN:%d, USER_PRI:%d,\n",
				  (value>>24)&0xFF, (value>>20)&7, (value>>19)&1, (value>>16)&7);
				printf("                   EG_TAG:%d, LEAKY_EN:%d, PORT:%X, STATUS:%d, TYPE:%d\n",
				  (value>>13)&7, (value>>12)&1, (value>>4)&0xFF, (value>>2)&3, (value)&3);
			} else if (ac_mat == 1){
				printf("DIP %08X :", addr1);
				printf(" tmr:%d, flag:%X, cnt%X", (addr2>>24)&0x0F, (addr2>>16)&0x0F, addr2&0xFF);
				printf(" USER_PRI:%d, EG_TAG:%d, LEAKY_EN:%d, PORT:%X, STATUS:%d, TYPE:%d\n",
				  (value>>16)&7, (value>>13)&7, (value>>12)&1, (value>>4)&0xFF, (value>>2)&3, (value)&3);
			} else if (ac_mat == 2){
				printf("DIP %08X, SIP %08X :", addr1, addr2);
				printf(" PORT:%X, STATUS:%d, TYPE:%d\n",
				  (value>>4)&0xFF, (value>>2)&3, (value)&3);
			}

printf("  reg dump 0x%08X, 0x%08X, 0x%04X\n", addr1, addr2, value);

			// trigger next
			reg = REG_ATC_ADDR;
			value = (1 << REG_ATC_BUSY_OFFT) + (ac_mat << REG_ATC_AC_MAT_OFFT) + (5 << REG_ATC_AC_CMD_OFFT);	// 3'b101 : Next search command (next entry)
			gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, value);
			while (1){	// wait until not busy
				value = gsw_reg_read2(swcfg->skfd, swcfg->ifr, ext_switch, reg);
				if ((value & REG_ATC_BUSY_MASK) == 0){
					break;
				}
			}
		}
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}

	return 0;
}/*end doArlAddrTblDisp2*/

/*___________________________________________________________________
**      function name: doArlAddrTblClr
**      descriptions:
**         Clean All Entries in selected Address Table.
**
**      parameters:
**         ac_mat: 0(mac)/1(dip)/2(sip)/3(specific).
**
**      global:
**           swic
**
**      return:
**         0:succuces
**      call:
**           subcmd
**
**      revision:
**      1. WPLin 2012/05/04
**_______________________________________________________________
*/
static int doArlAddrTblClr(int ac_mat){
	u32 value, reg;
	u32 addr1, addr2;

	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	if (ac_mat > 3){
		return 0;
	}

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
		reg = REG_ATC_ADDR;
		while (1){	// wait until not busy
			value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
			if ((value & REG_ATC_BUSY_MASK) == 0){
				break;
			}
		}

		// clean all entries
		reg = REG_ATC_ADDR;
		value = (1 << REG_ATC_BUSY_OFFT) + (ac_mat << REG_ATC_AC_MAT_OFFT) + (2 << REG_ATC_AC_CMD_OFFT);	// 3'b010 : clean command (multiple entries)
		gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
		while (1){	// wait until not busy
		value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
			if ((value & REG_ATC_BUSY_MASK) == 0){
				break;
			}
		}
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}

	return 0;
}/*end doArlAddrTblClr*/

/*___________________________________________________________________
**      function name: doArlAddrTblClr2
**      descriptions:
**         Clean All Entries in selected Address Table.
**
**      parameters:
**         u32 ext_switch: 0(int)/1(ext).
**         ac_mat: 0(mac)/1(dip)/2(sip)/3(specific).
**
**      global:
**           swic
**
**      return:
**         0:succuces
**      call:
**           subcmd
**
**      revision:
**      1. WPLin 2012/05/04
**_______________________________________________________________
*/
static int doArlAddrTblClr2(u32 ext_switch, int ac_mat){
	u32 value, reg;
	u32 addr1, addr2;

	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	if (ac_mat > 3){
		return 0;
	}
	printf("(ext_switch:%d)\r\n", ext_switch);

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
		reg = REG_ATC_ADDR;
		while (1){	// wait until not busy
			value = gsw_reg_read2(swcfg->skfd, swcfg->ifr, ext_switch, reg);
			if ((value & REG_ATC_BUSY_MASK) == 0){
				break;
			}
		}

		// clean all entries
		reg = REG_ATC_ADDR;
		value = (1 << REG_ATC_BUSY_OFFT) + (ac_mat << REG_ATC_AC_MAT_OFFT) + (2 << REG_ATC_AC_CMD_OFFT);	// 3'b010 : clean command (multiple entries)
		gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, value);
		while (1){	// wait until not busy
		value = gsw_reg_read2(swcfg->skfd, swcfg->ifr, ext_switch, reg);
			if ((value & REG_ATC_BUSY_MASK) == 0){
				break;
			}
		}
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}

	return 0;
}/*end doArlAddrTblClr2*/

/*ACL related ci-cmds*/
/*___________________________________________________________________
**      function name: doAcl
**      descriptions:
**         Register ACL subcmd.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments
**         p:function pointer
**
**      global:
**           ethCmds
**
**      return:
**         0:succuces
**         -1:fail
**
**      call:
**           subcmd
**
**      revision:
**      1. WPLin 2012/07/26
**_______________________________________________________________
*/
static int doAcl(int argc, char *argv[], void *p)
{
	return subcmd(aclCmds, argc, argv, p);
}/*end doAcl*/

/*___________________________________________________________________
**      function name: doAclSetPortEn
**      descriptions:
**         Enable ACL for port n : 0x2n04[10].
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments
**         p:function pointer
**
**      global:
**           ethCmds
**
**      return:
**         0:succuces
**         -1:fail
**
**      call:
**           subcmd
**
**      revision:
**      1. WPLin 2012/07/26
**_______________________________________________________________
*/
static int doAclSetPortEn(int argc, char *argv[], void *p)
{
	u32 value, reg;
	u8 acl_port, acl_en;
	etherChannVlanCap_t*  vlanCap=getEtherVlanCap();
	struct swconfig *swcfg=NULL;

	swcfg=getSwcfg();

	acl_port = atoi(argv[1]);
	acl_en = atoi(argv[2]);

	printf("acl_port:%d, acl_en:%d\n", acl_port, acl_en);

	/*Check the input parameters is right or not.*/
	if ((acl_port > vlanCap->max_port_no) || (acl_en > 1)){
		printf(HELP_ACL_SETPORTEN);
		return -1;
	}

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
		reg = REG_PCR_P0_ADDR + (0x100 * acl_port);	// 0x2004[10]
		value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
		value &= (~REG_PORT_ACL_EN_MASK);
		value |= (acl_en << REG_PORT_ACL_EN_OFFT);

		printf("write reg: %x, value: %x\n", reg, value);
		gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}

	return 0;
}

/*___________________________________________________________________
**      function name: doAclSetMultiHit
**      descriptions:
**         allow multiple ACL hit on Rate, Interrupt and MIB : 0x000C[5]
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments
**         p:function pointer
**
**      global:
**           ethCmds
**
**      return:
**         0:succuces
**         -1:fail
**
**      call:
**           subcmd
**
**      revision:
**      1. WPLin 2012/07/26
**_______________________________________________________________
*/
static int doAclSetMultiHit(int argc, char *argv[], void *p)
{
	u32 value, reg;
	u8 acl_multi;
	struct swconfig *swcfg=NULL;

	swcfg=getSwcfg();

	acl_multi = atoi(argv[1]);

	printf("acl_multi:%d\n", acl_multi);

	/*Check the input parameters is right or not.*/
	if (acl_multi > 1){
		printf(HELP_ACL_SETMULTIHIT);
		return -1;
	}

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
		reg = 0x000C;
		value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
		value &= (~(1 << 5));
		value |= (acl_multi << 5);

		printf("write reg: %x, value: %x\n", reg, value);
		gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}

	return 0;
}

/*___________________________________________________________________
**      function name: doAclTblClr
**      descriptions:
**         Clear all acl; mask, rule ctrl, rate ctrl; and trTCM table entries.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments
**         p:function pointer
**
**      global:
**           ethCmds
**
**      return:
**         0:succuces
**         -1:fail
**
**      call:
**           subcmd
**
**      revision:
**      1. WPLin 2012/07/26
**_______________________________________________________________
*/
static int doAclTblClr(int argc, char *argv[], void *p)
{
	u32 value, reg;
	int i, j;
	const u32 tbl_field[5][2] =
	{
		{(REG_VTCR_BUSY_MASK | (0x05 << REG_VTCR_FUNC_OFFT)), 64},	// ACL table entry 0~63
		{(REG_VTCR_BUSY_MASK | (0x07 << REG_VTCR_FUNC_OFFT)), 32},	// trTCM entry 0~31
		{(REG_VTCR_BUSY_MASK | (0x09 << REG_VTCR_FUNC_OFFT)), 32},	// ACL Mask entry 0~31
		{(REG_VTCR_BUSY_MASK | (0x0B << REG_VTCR_FUNC_OFFT)), 32},	// ACL Rule Control entry 0~31
		{(REG_VTCR_BUSY_MASK | (0x0D << REG_VTCR_FUNC_OFFT)), 32},	// ACL Rate Control entry 0~31
	};

	struct swconfig *swcfg=NULL;

	swcfg=getSwcfg();

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
		gsw_reg_write(swcfg->skfd, swcfg->ifr, REG_VAWD1_ADDR, 0x00000000);
		gsw_reg_write(swcfg->skfd, swcfg->ifr, REG_VAWD2_ADDR, 0x00000000);

		reg = REG_VTCR_ADDR;
		while (1){	// wait until not busy
			value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
			if ((value & REG_VTCR_BUSY_MASK) == 0){
				break;
			}
		}
		// Clear different kinds of table entries
		for (j = 0; j < 5; j ++){
			for (i = 0; i < tbl_field[j][1]; i ++){
				value = tbl_field[j][0] | i;
				gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
				while (1){	// wait until not busy
					value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
					if ((value & REG_VTCR_BUSY_MASK) == 0){
						break;
					}
				}
			}
		}
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}

	return 0;
}

/*___________________________________________________________________
**      function name: doAclTblDisp
**      descriptions:
**         Display all acl; mask, rule ctrl, rate ctrl; and trTCM table entries.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments
**         p:function pointer
**
**      global:
**           ethCmds
**
**      return:
**         0:succuces
**         -1:fail
**
**      call:
**           subcmd
**
**      revision:
**      1. WPLin 2012/07/26
**_______________________________________________________________
*/
static int doAclTblDisp(int argc, char *argv[], void *p)
{
	u32 value, vawd1, vawd2, rule_vawd1, rule_vawd2, rate_vawd1, rate_vawd2;
	int i;
	struct swconfig *swcfg=NULL;
	u8 portMac[6], ext_switch = 0;

	swcfg=getSwcfg();
	ext_switch = atoi(argv[1]);
	
	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
		while (1){	// wait until not busy
			value = gsw_reg_read2(swcfg->skfd, swcfg->ifr, ext_switch, REG_VTCR_ADDR);
			if ((value & REG_VTCR_BUSY_MASK) == 0){
				break;
			}
		}

		// Display ACL table entry 0~63
		printf("Found following ACL table entries (ext_switch :%d):\n", ext_switch);
		for (i = 0; i < 64; i ++){
			value = REG_VTCR_BUSY_MASK | (0x04 << REG_VTCR_FUNC_OFFT) | i;	// read ACL table (0x04)
			gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, REG_VTCR_ADDR, value);
			while (1){	// wait until not busy
				value = gsw_reg_read2(swcfg->skfd, swcfg->ifr, ext_switch, REG_VTCR_ADDR);
				if ((value & REG_VTCR_BUSY_MASK) == 0){
					break;
				}
			}
			vawd1 = gsw_reg_read2(swcfg->skfd, swcfg->ifr, ext_switch,  REG_VAWD1_ADDR);
			vawd2 = gsw_reg_read2(swcfg->skfd, swcfg->ifr, ext_switch, REG_VAWD2_ADDR);

			if (vawd2 & (1 << 19)){	// EN==1
				printf("[#%2d] CMP_SEL:%d, SP_map:%02X, OFST_TP:%d, WORD_OFST:%3d, BIT_MASK:%04X, CMP_PAT:%04X\n",
					i, (vawd2 & 1), ((vawd2>>8) & 0xFF), ((vawd2>>16) & 0x07), ((vawd2>>1) & 0x3F), ((vawd1>>16) & 0xFFFF), (vawd1 & 0xFFFF));
			}
		}

		// Display Mask, Rule Ctrl, and Rate Ctrl table entry 0~31
		printf("Found following Mask, Rule ctrl, Rate ctrl table entries : (in vawd1,vawd2)\n");
		for (i = 0; i < 32; i ++){
			value = REG_VTCR_BUSY_MASK | (0x08 << REG_VTCR_FUNC_OFFT) | i;	// read Mask Table (0x08)
			gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, REG_VTCR_ADDR, value);
			while (1){	// wait until not busy
				value = gsw_reg_read2(swcfg->skfd, swcfg->ifr, ext_switch, REG_VTCR_ADDR);
				if ((value & REG_VTCR_BUSY_MASK) == 0){
					break;
				}
			}
			vawd1 = gsw_reg_read2(swcfg->skfd, swcfg->ifr, ext_switch, REG_VAWD1_ADDR);
			vawd2 = gsw_reg_read2(swcfg->skfd, swcfg->ifr, ext_switch, REG_VAWD2_ADDR);

			if (vawd1 || vawd2){	// ACL_MASK[63:0] any bit != 0
				value = REG_VTCR_BUSY_MASK | (0x0A << REG_VTCR_FUNC_OFFT) | i;	// read Rule Ctrl Table (0x0A)
				gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, REG_VTCR_ADDR, value);
				while (1){	// wait until not busy
					value = gsw_reg_read2(swcfg->skfd, swcfg->ifr, ext_switch, REG_VTCR_ADDR);
					if ((value & REG_VTCR_BUSY_MASK) == 0){
						break;
					}
				}
				rule_vawd1 = gsw_reg_read2(swcfg->skfd, swcfg->ifr, ext_switch, REG_VAWD1_ADDR);
				rule_vawd2 = gsw_reg_read2(swcfg->skfd, swcfg->ifr, ext_switch, REG_VAWD2_ADDR);

				value = REG_VTCR_BUSY_MASK | (0x0C << REG_VTCR_FUNC_OFFT) | i;	// read Rate Ctrl Table (0x0C)
				gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, REG_VTCR_ADDR, value);
				while (1){	// wait until not busy
					value = gsw_reg_read2(swcfg->skfd, swcfg->ifr, ext_switch, REG_VTCR_ADDR);
					if ((value & REG_VTCR_BUSY_MASK) == 0){
						break;
					}
				}
				rate_vawd1 = gsw_reg_read2(swcfg->skfd, swcfg->ifr, ext_switch, REG_VAWD1_ADDR);
				rate_vawd2 = gsw_reg_read2(swcfg->skfd, swcfg->ifr, ext_switch, REG_VAWD2_ADDR);

				printf("[#%2d] Mask:%08X,%08X; Rule:%08X,%08X; Rate:%08X,%08X\n",
					i, vawd1, vawd2, rule_vawd1, rule_vawd2, rate_vawd1, rate_vawd2);
			}
		}
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}

	return 0;
}

static int doAclAclTblAdd(int argc, char *argv[], void *p)
{
	u32 value, reg, vawd1, vawd2;
	u8 tbl_idx;

	struct swconfig *swcfg=NULL;

	swcfg=getSwcfg();

	tbl_idx = atoi(argv[1]);
	vawd1 = strtoul(argv[2], (char **)NULL, 16);
	vawd2 = strtoul(argv[3], (char **)NULL, 16);

	printf("tbl_idx:%d\n", tbl_idx);

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
		if (tbl_idx > 63) {	/*Check the input parameters is right or not.*/
			printf(HELP_ACL_ACL_TBL_ADD);
			return -1;
		}

		reg = REG_VTCR_ADDR;
		while (1){	// wait until not busy
			value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
			if ((value & REG_VTCR_BUSY_MASK) == 0){
				break;
			}
		}

		gsw_reg_write(swcfg->skfd, swcfg->ifr, REG_VAWD1_ADDR, vawd1);
		gsw_reg_write(swcfg->skfd, swcfg->ifr, REG_VAWD2_ADDR, vawd2);

		reg = REG_VTCR_ADDR;
		value = REG_VTCR_BUSY_MASK | (0x05 << REG_VTCR_FUNC_OFFT) | tbl_idx;
		gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
		while (1){	// wait until not busy
			value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
			if ((value & REG_VTCR_BUSY_MASK) == 0){
				break;
			}
		}
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}

	return 0;
}

static int doAclMaskTblAdd(int argc, char *argv[], void *p)
{
	u32 value, reg, vawd1, vawd2;
	u8 tbl_idx;

	struct swconfig *swcfg=NULL;

	swcfg=getSwcfg();

	tbl_idx = atoi(argv[1]);
	vawd1 = strtoul(argv[2], (char **)NULL, 16);
	vawd2 = strtoul(argv[3], (char **)NULL, 16);

	printf("tbl_idx:%d\n", tbl_idx);

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
		if (tbl_idx > 31) {	/*Check the input parameters is right or not.*/
			printf(HELP_ACL_MASK_TBL_ADD);
			return -1;
		}

		reg = REG_VTCR_ADDR;
		while (1){	// wait until not busy
			value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
			if ((value & REG_VTCR_BUSY_MASK) == 0){
				break;
			}
		}

		gsw_reg_write(swcfg->skfd, swcfg->ifr, REG_VAWD1_ADDR, vawd1);
		gsw_reg_write(swcfg->skfd, swcfg->ifr, REG_VAWD2_ADDR, vawd2);

		reg = REG_VTCR_ADDR;
		value = REG_VTCR_BUSY_MASK | (0x09 << REG_VTCR_FUNC_OFFT) | tbl_idx;
		gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
		while (1){	// wait until not busy
			value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
			if ((value & REG_VTCR_BUSY_MASK) == 0){
				break;
			}
		}
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}

	return 0;
}

static int doAclRuleTblAdd(int argc, char *argv[], void *p)
{
	u32 value, reg, vawd1, vawd2;
	u8 tbl_idx;

	struct swconfig *swcfg=NULL;

	swcfg=getSwcfg();

	tbl_idx = atoi(argv[1]);
	vawd1 = strtoul(argv[2], (char **)NULL, 16);
	vawd2 = strtoul(argv[3], (char **)NULL, 16);

	printf("tbl_idx:%d\n", tbl_idx);

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
		if (tbl_idx > 31) {	/*Check the input parameters is right or not.*/
			printf(HELP_ACL_RULE_TBL_ADD);
			return -1;
		}

		reg = REG_VTCR_ADDR;
		while (1){	// wait until not busy
			value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
			if ((value & REG_VTCR_BUSY_MASK) == 0){
				break;
			}
		}

		gsw_reg_write(swcfg->skfd, swcfg->ifr, REG_VAWD1_ADDR, vawd1);
		gsw_reg_write(swcfg->skfd, swcfg->ifr, REG_VAWD2_ADDR, vawd2);

		reg = REG_VTCR_ADDR;
		value = REG_VTCR_BUSY_MASK | (0x0B << REG_VTCR_FUNC_OFFT) | tbl_idx;
		gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
		while (1){	// wait until not busy
			value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
			if ((value & REG_VTCR_BUSY_MASK) == 0){
				break;
			}
		}
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}

	return 0;
}

static int doAclRateTblAdd(int argc, char *argv[], void *p)
{
	u32 value, reg, vawd1, vawd2;
	u8 tbl_idx;

	struct swconfig *swcfg=NULL;

	swcfg=getSwcfg();

	tbl_idx = atoi(argv[1]);
	vawd1 = strtoul(argv[2], (char **)NULL, 16);
	vawd2 = strtoul(argv[3], (char **)NULL, 16);

	printf("tbl_idx:%d\n", tbl_idx);

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
		if (tbl_idx > 31) {	/*Check the input parameters is right or not.*/
			printf(HELP_ACL_RATE_TBL_ADD);
			return -1;
		}

		reg = REG_VTCR_ADDR;
		while (1){	// wait until not busy
			value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
			if ((value & REG_VTCR_BUSY_MASK) == 0){
				break;
			}
		}

		gsw_reg_write(swcfg->skfd, swcfg->ifr, REG_VAWD1_ADDR, vawd1);
		gsw_reg_write(swcfg->skfd, swcfg->ifr, REG_VAWD2_ADDR, vawd2);

		reg = REG_VTCR_ADDR;
		value = REG_VTCR_BUSY_MASK | (0x0D << REG_VTCR_FUNC_OFFT) | tbl_idx;
		gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
		while (1){	// wait until not busy
			value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
			if ((value & REG_VTCR_BUSY_MASK) == 0){
				break;
			}
		}
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}

	return 0;
}

//#endif //SQA_VERIFY

/*VLAN related ci-cmds*/
/*___________________________________________________________________
**      function name: doVlan
**      descriptions:
**         Register vlan subcmd.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments 
**         p:function pointer
**         
**      global:
**           ethCmds
**             
**      return:
**         0:succuces
**         -1:fail
**	    
**      call:
**           subcmd
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
static int doVlan(int argc, char *argv[], void *p){
	return subcmd(vlanCmds, argc, argv, p);
}/*end doVlan*/
/*___________________________________________________________________
**      function name: doVlanSetActive
**      descriptions:
**         Enable/disable the swtich tagged base vlan function.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments 
**         p:function pointer
**         
**      global:
**           swic
**             
**      return:
**         0:succuces
**         -1:fail
**	    
**      call:
**           subcmd
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
static int doVlanSetActive(int argc, char *argv[], void *p){
	u8 mode=0;
	mode=(u8)atoi(argv[1]);

	if(mode>ACTIVED){
		printf(HELP_VLAN_ACTIVE);
		return -1;
	}
	if(swic.vlan_active!=NULL){
		swic.vlan_active(mode);
	}
	#ifdef ETHCMD_DBG
	ETH_DEBUG("%s","ci-cmd:doVlanSetActive\r\n");
	#endif
	return 0;
}/*end doVlanSetActive*/
/*___________________________________________________________________
**      function name: doVlanSetPvid
**      descriptions:
**         Set the Etherent port PVID value.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments 
**         p:function pointer
**         
**      global:
**           ethCmds
**             
**      return:
**         0:succuces
**         -1:fail
**	    
**      call:
**           subcmd
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
static int doVlanSetPvid(int argc, char *argv[], void *p){
	u8 port=0;
	u16 pvid=0;
	etherChannVlanCap_t*  vlanCap=getEtherVlanCap();
	
	port=atoi(argv[1]);
	pvid=atoi(argv[2]);
	/*Check the input parameters is right or not.*/
	if((port>=vlanCap->max_port_no)||(pvid>vlanCap->max_vid)){
		printf(HELP_VLAN_PVID);
		return -1;
	}
	
	if(swic.vlan_pvid!=NULL){
		swic.vlan_pvid(port, pvid);
	}
	#ifdef ETHCMD_DBG
	if(ethcmdDbg)
		printf("port:%d pvid:%d, vlancap: max_port:%d maxvid:%d\r\n",port, pvid, vlanCap->max_port_no, vlanCap->max_vid);
	#endif
	return 0;
}/*end doVlanSetPvid*/
/*___________________________________________________________________
**      function name: doVlanSetVid
**      descriptions:
**         Update a vlan table rule accroding the table index.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments 
**         p:function pointer
**         
**      global:
**           ethCmds
**             
**      return:
**         0:succuces
**         -1:fail
**	    
**      call:
**           subcmd
**      
**      revision:
**      1. Here 2009/11/7
**      2. WPLin 2012/08/31
**_______________________________________________________________
*/
static int doVlanSetVid(int argc, char *argv[], void *p){
	u8 index=0;
	u8 active=0;
	u8 portMap=0;
	u8 tagPortMap=0;
	u16 vid=0;
	#if defined(EXTEND_SETVID_PARAM) || defined(SQA_VERIFY)
	u8 ivl_en=0;
	u8 fid=0;
	u16 stag=0;
	#endif
	etherChannVlanCap_t*  vlanCap=getEtherVlanCap();
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

#if (0)
	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
		active=atoi(argv[1]);
		vid=atoi(argv[2]);

		/*Check the input parameters is right or not.*/
		if((vid>=vlanCap->max_vid) || (active>ACTIVED)){
			printf(HELP_VLAN_VID);
			return -1;
		}

		/*CPU Port is always the membership*/
		portMap=atoi(argv[3]);
		tagPortMap=atoi(argv[4]);

		ivl_en=atoi(argv[5]);
		fid=atoi(argv[6]);
		stag=atoi(argv[7]);
	}
	else {
#endif
		index=atoi(argv[1]);
		active=atoi(argv[2]);
		vid=atoi(argv[3]);

		/*Check the input parameters is right or not.*/
		if((index>=MAX_VLAN_RULE)
		|| (vid>=vlanCap->max_vid)
		|| (active>ACTIVED)){
			printf(HELP_VLAN_VID);
			return -1;
		}

		/*CPU Port is always the membership*/
		portMap=atoi(argv[4]);
		tagPortMap=atoi(argv[5]);

		#if defined(EXTEND_SETVID_PARAM)
		printf("subcmd parameter argc = %d\r\n", argc);
		if (argc >= 7)
		{
			ivl_en=atoi(argv[6]);
			if (argc >= 8)
			{
				fid=atoi(argv[7]);
				if (argc >= 9)
				{
					stag=atoi(argv[8]);
				}
			}
		}
		#else
			#if defined(SQA_VERIFY)
		ivl_en=atoi(argv[6]);
		fid=atoi(argv[7]);
		stag=atoi(argv[8]);
			#endif
		#endif
#if (0)
	}
#endif

	#if defined(EXTEND_SETVID_PARAM)
	if(swic.vlan_vid!=NULL){
		swic.vlan_vid(index, active, vid, portMap, tagPortMap, ivl_en, fid, stag);
	}
	#else

		#if defined(SQA_VERIFY)
	if(swic.sqa_vlan_vid!=NULL){
		swic.sqa_vlan_vid(index, active, vid, portMap, tagPortMap, ivl_en, fid, stag);
	}
		#else
	if(swic.vlan_vid!=NULL){
		swic.vlan_vid(index, active, vid, portMap, tagPortMap);
	}
		#endif
	#endif

	#ifdef ETHCMD_DBG
	if(ethcmdDbg)
		printf("index:%d active:%d vid:%d\r\n",index, active, vid);
	#endif
	return 0;
}/*end doVlanSetVid*/

//#if defined(SQA_VERIFY)
static int doVlanSetPortAttr(int argc, char *argv[], void *p){
	u8 port=0;
	u8 attr=0;
	u32 value; 
	u32 reg; 
	etherChannVlanCap_t*  vlanCap=getEtherVlanCap();
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();
	
	port=atoi(argv[1]);
	attr=atoi(argv[2]);

	printf("port: %x, attr: %x\n", port, attr);

	/*Check the input parameters is right or not.*/
	if((port<0 || port>vlanCap->max_port_no) || (attr<0 || attr>3)){
		printf(HELP_VLAN_PORT_ATTR);
		return -1;
	}

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806) || (swcfg->swic_id == SWIC_RT63365)) {
		reg = 0x2010 + port*0x100;
		value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
		value &= (0xffffff3f);
		value |= (attr<<6);

		printf("write reg: %x, value: %x\n", reg, value);

		gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}

	return 0;
}

static int doVlanSetPortMode(int argc, char *argv[], void *p){
	u8 port=0;
	u8 mode=0;
	u32 value;
	u32 reg;
//	int portmap=0;
	etherChannVlanCap_t*  vlanCap=getEtherVlanCap();
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	port=atoi(argv[1]);
	mode=atoi(argv[2]);
//	portmap=atoi(argv[3]);

	printf("port: %x, mode: %x\n", port, mode);

	/*Check the input parameters is right or not.*/
	if((port<0 || port>vlanCap->max_port_no) || (mode<0 || mode>3)){
		printf(HELP_VLAN_PORT_MODE);
		return -1;
	}

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806) || (swcfg->swic_id == SWIC_RT63365)) {
		reg = 0x2004 + port*0x100;
		value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
		value &= (~((1<<0) | (1<<1)));
		value |= (mode & 0x3);

#if 0
	if (portmap != -1){
		value &= (0xff00ffff);
		value |= (portmap << 16);
	}
#endif	
	
	printf("write reg: %x, value: %x\n", reg, value);
	gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}

	return 0;
}

/*___________________________________________________________________
**      function name: doVlanSetEgressTagPVC
**      descriptions:
**         Set Port-Based Egress VLAN Tag Attribute in PVC Register
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments
**         p:function pointer
**
**      global:
**           swic
**
**      return:
**         0:succuces
**         -1:fail
**
**      call:
**           subcmd
**
**      revision:
**      1. WPLin 2012/05/07
**_______________________________________________________________
*/
static int doVlanSetEgressTagPVC(int argc, char *argv[], void *p){
	u8 port=0;
	u8 eg_tag=0;
	u32 value;
	u32 reg;

	etherChannVlanCap_t* vlanCap=getEtherVlanCap();
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	port=atoi(argv[1]);
	eg_tag=atoi(argv[2]);

	printf("port: %d, eg_tag: %d\n", port, eg_tag);

	/*Check the input parameters is right or not.*/
	if ((port > vlanCap->max_port_no) || (eg_tag > REG_PVC_EG_TAG_RELMASK)){
		printf(HELP_VLAN_EGRESS_TAG_PVC);
		return -1;
	}

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806) || (swcfg->swic_id == SWIC_RT63365)) {
		reg = REG_PVC_P0_ADDR + port*0x100;
		value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
		value &= (~REG_PVC_EG_TAG_MASK);
		value |= ((u32)eg_tag << REG_PVC_EG_TAG_OFFT);

		printf("write reg: %x, value: %x\n", reg, value);
		gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}

	return 0;
}/*end doVlanSetEgressTagPVC*/

/*___________________________________________________________________
**      function name: doVlanSetEgressTagPCR
**      descriptions:
**         Set Port-Based Egress VLAN Tag Attribute in PCR Register
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments
**         p:function pointer
**
**      global:
**           swic
**
**      return:
**         0:succuces
**         -1:fail
**
**      call:
**           subcmd
**
**      revision:
**      1. WPLin 2012/05/07
**_______________________________________________________________
*/
static int doVlanSetEgressTagPCR(int argc, char *argv[], void *p){
	u8 port=0;
	u8 eg_tag=0;
	u32 value;
	u32 reg;

	etherChannVlanCap_t* vlanCap=getEtherVlanCap();
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	port=atoi(argv[1]);
	eg_tag=atoi(argv[2]);

	printf("port: %d, eg_tag: %d\n", port, eg_tag);

	/*Check the input parameters is right or not.*/
	if ((port > vlanCap->max_port_no) || (eg_tag > REG_PCR_EG_TAG_RELMASK)){
		printf(HELP_VLAN_EGRESS_TAG_PCR);
		return -1;
	}

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806) || (swcfg->swic_id == SWIC_RT63365)) {
		reg = REG_PCR_P0_ADDR + port*0x100;
		value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
		value &= (~REG_PCR_EG_TAG_MASK);
		value |= ((u32)eg_tag << REG_PCR_EG_TAG_OFFT);

		printf("write reg: %x, value: %x\n", reg, value);
		gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}

	return 0;
}/*end doVlanSetEgressTagPCR*/

/*___________________________________________________________________
**      function name: doVlanSetAccFrm
**      descriptions:
**         Set VLAN ACC_FRM "Acceptable Frame Type".
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments
**         p:function pointer
**
**      global:
**           swic
**
**      return:
**         0:succuces
**         -1:fail
**
**      call:
**           subcmd
**
**      revision:
**      1. WPLin 2012/04/18
**_______________________________________________________________
*/
static int doVlanSetAccFrm(int argc, char *argv[], void *p){
	u8 port=0;
	u8 type=0;
	u32 value;
	u32 reg;

	etherChannVlanCap_t* vlanCap=getEtherVlanCap();
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	port=atoi(argv[1]);
	type=atoi(argv[2]);

	printf("port: %d, type: %d\n", port, type);

	/*Check the input parameters is right or not.*/
	if ((port > vlanCap->max_port_no) || (type > REG_PVC_ACC_FRM_RELMASK)){
		printf(HELP_VLAN_ACC_FRM);
		return -1;
	}

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806) || (swcfg->swic_id == SWIC_RT63365)) {
		reg = REG_PVC_P0_ADDR + port*0x100;
		value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
		value &= (~REG_PVC_ACC_FRM_MASK);
		value |= ((u32)type << REG_PVC_ACC_FRM_OFFT);

		printf("write reg: %x, value: %x\n", reg, value);
		gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}

	return 0;
}/*end doVlanSetAccFrm*/

//#endif //SQA_VERIFY

/*___________________________________________________________________
**      function name: doVlanDisp
**      descriptions:
**         Display the pvid and port binding information.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments 
**         p:function pointer
**         
**      global:
**           ethCmds
**             
**      return:
**         0:succuces
**         -1:fail
**	    
**      call:
**           subcmd
**      
**      revision:
**      1. Here 2009/11/10
**_______________________________________________________________
*/
static int doVlanDisp(int argc, char *argv[], void *p){
	if(swic.vlan_disp!=NULL){
		swic.vlan_disp();
	}
	return 0;
}/*end doVlanDisp*/
#ifdef ETHCMD_EXTENSION
/*___________________________________________________________________
**      function name: doPortBinding
**      descriptions:
**         Update a vlan table rule accroding the table index.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments 
**         p:function pointer
**         
**      global:
**           ethCmds
**             
**      return:
**         0:succuces
**         -1:fail
**	    
**      call:
**           subcmd
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
static int doPortBinding(int argc, char *argv[], void *p){
	int index=0;
	int vid=0;
	int i=0,j=0;
	int err=ETHCMD_ERR;
	char cmd[MAX_CMD_LEN]={0};
	int op=0;
	int pm_if[MAX_IFS]={0};
	u32 pm_flag=0;
	u8 portMap=0;

	#if defined(TCSUPPORT_2PORTS) || defined(TCSUPPORT_CT_2PWIFI)
	etherChannVlanCap_t*  vlanCap=getEtherVlanCap();
	#endif
	for(i=0;i<MAX_IFS;i++){
		pm_if[i]=NONE_IF;
	}
	
	index=atoi(argv[1]);
	op=atoi(argv[2]);
	
	if((op==ACTIVED) ||(op==DEACTIVED)
		&&(index<MAX_VLAN_RULE)){
		err=0;
	}
	/*lookup the interface name*/
	for (i=PM_START_IF_ARG; i<argc; i++){
		for(j=0; tcPortMap[j].ifname!=NULL; j++){
			if(strcmp(argv[i], tcPortMap[j].ifname)==0){
				err=0;
				/*Record the tcPortMap index of real ifname,starting at arg 3*/
				pm_flag|=(1<<j);
				pm_if[i-PM_START_IF_ARG]=j;
			}
		}
	}
	
	#ifdef ETHCMD_DBG
	if(ethcmdDbg){
		for(i=0; i<MAX_IFS; i++){
			printf("pm_if[%d]=%d ", i, pm_if[i]);
			if(i%5==4)
				printf("\r\n");
		}
		printf("\r\n err:%d\r\n",err);
	}
	#endif
	
	/*checking the interface name is right or not.*/
	if(err==ETHCMD_ERR){
		#if defined(TCSUPPORT_2PORTS) || defined(TCSUPPORT_CT_2PWIFI)
		printf(HELP_PORTBINDING,vlanCap->max_port_no-1);
		#else
		printf(HELP_PORTBINDING);
		#endif
		printf("\n");
		return ETHCMD_ERR;
	}
	
	if(op==DEACTIVED){
		/*Remove the virtual interface from bridge interface*/
		for(i=0; pm_if[i]!=NONE_IF; i++){
			sprintf(cmd,"brctl delif br%d %s",index, tcPortMap[pm_if[i]].real_name);
			system(cmd);
			#ifdef ETHCMD_DBG
			ETH_DEBUG("%s",cmd);
			#endif
		}
		if(index!=0){/*We can't delete default br0 bridge interface*/
			/*Let bridge interface down*/
			sprintf(cmd,"ifconfig br%d down",index);
			system(cmd);
			#ifdef ETHCMD_DBG
			ETH_DEBUG("%s",cmd);
			#endif
			/*Delete the bridge interface*/
			sprintf(cmd,"brctl delbr br%d",index);
			system(cmd);	
			#ifdef ETHCMD_DBG
			ETH_DEBUG("%s",cmd);
			#endif
		}
	}
	else{
		/*Add a new bridge interface to binding interface*/
		if(index!=0){/*We can't delete default br0 bridge interface*/
			/*Adding the bridge interface*/
			sprintf(cmd,"brctl addbr br%d",index);
			system(cmd);
			/*Let bridge interface up*/
			sprintf(cmd,"ifconfig br%d 0.0.0.0 up",index);
			system(cmd);
		}
		
		/*binding the virtual interface into bridge interface*/
		/*Note: Need to check the pvc operate at bridge mode or routing mode, Only
		operate at bridge mode can be enslave by bridge interface.Routing mode is 
		need used iptable to filter it.*/
		for(i=0; pm_if[i]!=NONE_IF; i++){
			sprintf(cmd,"brctl addif br%d %s",index, tcPortMap[pm_if[i]].real_name);
			system(cmd);
			#ifdef ETHCMD_DBG
			ETH_DEBUG("%s",cmd);
			#endif
		}
	}
	return 0;
}/*end doPortBinding*/
#endif

#if 0
/*Use ebtable to set do it*/
/*______________________________________________________________________________
**	function name:decideModulePath
**
**	description:
*     decide module path according to the kernel version
**	parameters:
*     		vername: used to obtain kernel version
**	global:
*     		none
**	return:
*     		none
**	call:
*     		none
**	revision:
*     1.Ian 20091129
**____________________________________________________________________________*/
void decideModulePath(char* vername){
	FILE * Ver;
	char buf[50], *start, *end;

	Ver = fopen ("/proc/version","r");
	if(!Ver){
		printf("Failed to open /proc/version\r\n");
		return;
	}
	fgets(buf, 50, Ver);
	fclose(Ver);
	start = strchr(buf, ' ');
	start = strchr(start + 1, ' ') + 1;
	end = strchr(start + 1, ' ');
	*end = '\0';
	strcpy(vername, start);
}/*end decideModulePath*/
/*___________________________________________________________________
**      function name: ebtSettings
**      descriptions:
**         Loading/Unloading ebtables and setting the FORWARD chain.
**        1. The FORWARD chain is used to drop the packet for ethernet switch site 
**   	  	  packet.
**
**      parameters:
**         type:ACTIVED/DEACTIVED ebtables.
**         
**      global:
**           None
**             
**      return:
**         None
**	    
**      call:
**           getEtherVlanCap
**           decideModulePath
**      
**      revision:
**      1. Here 2009/11/29
**_______________________________________________________________
*/
void
ebtSettings(u8 type){
	FILE *fp=NULL;
	char getStr[8];
	int isActive=0;
	char linuxVersion[16];
	char cmd[MAX_CMD_LEN]={0};
	int i=0,j=0;
	etherChannVlanCap_t*  vlanCap=getEtherVlanCap();
	char ebt_modules[][32]={"ebtables.ko", "ebtable_filter.ko", ""};

	/*Get the current linux kernel version*/
	decideModulePath(linuxVersion);
	if(type==ACTIVED){
		/*check the modules is load or not*/
		fp=fopen(VPORT_FILE, "r");
		if(fp!=NULL){
			fgets(getStr, sizeof(getStr), fp);
			isActive=atoi(getStr);
			fclose(fp);
		}
		
		if(isActive==DEACTIVED){/*If it's not load ebt modules and to loading it.*/
			for(i=0; strlen(ebt_modules[i])!=0; i++){
				sprintf(cmd, EBT_MODULES_INSTALL, linuxVersion, ebt_modules[i]);
				system(cmd);
			}
		}
		system(EBT_FWD_FLUSH);
		system(EBT_FWD_ACCEPT);
		/*SET FORWARD chain to drop the packet is from ethernet site*/
		for(i=1; i<=vlanCap->max_port_no; i++){
			for(j=1; j<=vlanCap->max_port_no; j++){
				if(i !=j){/*Not set at same interface with input/output interface*/
					sprintf(cmd, EBT_FWD_DROP_RULE, i, j);
					system(cmd);
				}
			}
		}
	}
	else{
		/*FLUSH FORWARD Chain*/
		system(EBT_FWD_FLUSH);
		system(EBT_MODULES_REMOVE);
	}
}/*end ebtSettings*/
#endif
/*___________________________________________________________________
**      function name: setLanchip
**      descriptions:
**        Enable/disable the ethernet port that is used mii register to let ethernet 
**        phy to power down or not.
**
**      parameters:
**         type: enable: Power up the ethernet PHY ,disable: Power down the ethernet phy.
**         portid: Specify the ethernet Port number.
**         
**      global:
**           ethcmdDbg
**             
**      return:
**         0:succuces
**         -1:fail
**	    
**      call:
**           mdio_read
**           mdio_write
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
static int 
setLanchip(char* type, int phyID){
	int val=0;
#ifdef RA_ETHERMEDIATYPE
	int val2=0;
#endif
	int retval=0;
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	if(strcmp(type, ETHCMD_DISABLE)==0){/*Power Down the Ethernet PHY Port*/
		if(swcfg->swic_id==TC2206){
			/*Read FLP register*/
			val=mdio_read(swcfg->skfd, swcfg->ifr, phyID, MII_ADVERTISE);
			/*Set FLP register to disable all capability 0x1*/
			mdio_write(swcfg->skfd, swcfg->ifr, phyID, MII_ADVERTISE, EXPANSION_NWAY);
#ifdef RA_ETHERMEDIATYPE
			/*We do not need always enable AN because this action will cause the 
			port configuarion which user set been midified. shnwind 20110428*/
			val2=(mdio_read(swcfg->skfd, swcfg->ifr, phyID, MII_BMCR)|BMCR_PDOWN);
			mdio_write(swcfg->skfd, swcfg->ifr, phyID, MII_BMCR, val2);
#else			
			/*restart-AN*/
			mdio_write(swcfg->skfd, swcfg->ifr, phyID, MII_BMCR, (BMCR_ANENABLE |BMCR_ANRESTART));
			/*Power Down*/
			mdio_write(swcfg->skfd, swcfg->ifr, phyID, MII_BMCR, (BMCR_ANENABLE |BMCR_PDOWN));
#endif			
			/*Restore FLP register*/
			mdio_write(swcfg->skfd, swcfg->ifr, phyID, MII_ADVERTISE, val);
		}
		else{
			val=(mdio_read(swcfg->skfd, swcfg->ifr, phyID, MII_BMCR)|BMCR_PDOWN);
			mdio_write(swcfg->skfd, swcfg->ifr, phyID, MII_BMCR, val);
		}
	}
	else if(strcmp(type, ETHCMD_ENABLE)==0){/*Power UP the Ethernet PHY Port*/
		val= (mdio_read(swcfg->skfd, swcfg->ifr, phyID, MII_BMCR) & ENABLE_LAN_MASK);
		mdio_write(swcfg->skfd, swcfg->ifr, phyID, MII_BMCR, val);
	}
	else{
		retval=-1;
	}

	return retval;
}/*end setLanchip*/
/*___________________________________________________________________
**      function name: ethcmdQueryPortCount
**      descriptions:
**        Check user specify portid is fit with this platform.
**
**      parameters:
**         portid: port id.
**         
**      global:
**           None
**             
**      return:
**        switch port number, 1: single port , 4: 4ports-switch.
**	    
**      call:
**           getEtherPort
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
static int ethcmdQueryPortCount(int portid){
	etherChannVlanCap_t*  vlanCap=getEtherVlanCap();
	if(vlanCap->max_port_no !=SWIC_PORTS_NUM){
		 printf("eth0 is not Switch chip.\n");
		
	}
	else{
		if((portid<0)||(portid > vlanCap->max_port_no)){
			  printf("Invalid port: should is <%d-%d>", 0, (vlanCap->max_port_no-1));
		}
	}
	return vlanCap->max_port_no;
}/*end ethcmdQueryPortCount*/
/*___________________________________________________________________
**      function name: ethcmdGetMediaOption
**      descriptions:
**        Check user specify media-type option is right or not, if the parameters is
**        return the right value to set mii register.
**
**      parameters:
**         option: user specify the parameters of media-type.
**         
**      global:
**           None
**             
**      return:
**       The value to set mii register to fit the specfiy link type.
**	    
**      call:
**           None
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
static u16 ethcmdGetMediaOption(char *option){
	u16 val=ERR_MEDIAY_TYPE;
	int i=0;
	struct linkType mediaOption[]={
		{"auto", (BMCR_ANENABLE | BMCR_ANRESTART)},
		{"100FD", (BMCR_SPEED100 | BMCR_FULLDPLX|BMCR_ANRESTART)},
		{"100HD", (BMCR_SPEED100|BMCR_ANRESTART)},
		{"10FD", (BMCR_FULLDPLX|BMCR_ANRESTART)},
		{"10HD", 0},
		{NULL, 0},
	};
	
	for(i=0; mediaOption[i].name!=NULL ; i++){
		if(strcmp(option,mediaOption[i].name)==0){
			val=mediaOption[i].val;
			break;
		}
	}
	return val;
}/*end ethcmdGetMediaOption*/
/*___________________________________________________________________
**      function name: showMediaSetting
**      descriptions:
**        Display the ethernet port link type.
**
**      parameters:
**         phy_id: Port ID.
**         
**      global:
**           None
**             
**      return:
**       The value to set mii register to fit the specfiy link type.
**	    
**      call:
**           None
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/	
static int showMediaSetting(int phy_id)
{
	int i;
	int bmcr, bmsr, nway_advert, lkpar;
	int max_capability = 0;
	/* Scan for the highest negotiated capability, highest priority
	(100baseTx-FDX) to lowest (10baseT-HDX). */
	int media_priority[] = {8, 9, 7, 6, 5}; 	/* media_names[i-5] */
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();
	
	 bmcr =mdio_read(swcfg->skfd, swcfg->ifr, phy_id, MII_BMCR);
	 bmsr =mdio_read(swcfg->skfd, swcfg->ifr, phy_id, MII_BMSR);	
    
	if (bmcr == 0xffff  ||  bmsr == 0x0000) {
		printf("  No MII transceiver present!.\n");
		return -1;
	}

	nway_advert = mdio_read(swcfg->skfd, swcfg->ifr, phy_id, MII_ADVERTISE);
	lkpar = mdio_read(swcfg->skfd, swcfg->ifr, phy_id, MII_LPA);

	if (bmcr & BMCR_ANENABLE) {
		printf("auto, ");
		printf("Auto-negotiation enabled.\n");
		/*for some device, bit "ADVERTISE_LPACK" in link partner register when auto negotiation
		 *is not set to 1, and these devices can not show link status when use ethcmd.
		 * mark the condition in order to show link status of all devices in LAN 
		 * xflu @ 20110722
		 */
//		if (lkpar & ADVERTISE_LPACK) {
			int negotiated = nway_advert & lkpar & 
							(ADVERTISE_100BASE4 |
							ADVERTISE_100FULL |
							ADVERTISE_100HALF |
							ADVERTISE_10FULL |
							ADVERTISE_10HALF );
			
			for (i = 0; media_priority[i]; i++) {
				if (negotiated & (1 << media_priority[i])) {
					max_capability = media_priority[i];
					break;
				}
			}
			if (max_capability)
				printf("The autonegotiated media type is %s.\n",
					media_names[max_capability - 5]);
			else
				printf("No common media type was autonegotiated!\n"
				"This is extremely unusual and typically indicates a "
				"configuration error.\n" "Perhaps the advertised "
				"capability set was intentionally limited.\n");
//		}
	} 
	else {
		if(bmcr & BMCR_SPEED100)
			printf("100");
		else
			printf("10");

		if(bmcr & BMCR_FULLDPLX)
			printf("FD, ");
		else
			printf("HD, ");

		printf("Auto-negotiation disabled, with\n"
		" Speed fixed at 10%s mbps, %s-duplex.\n",
		bmcr & BMCR_SPEED100 ? "0" : "",
		bmcr & BMCR_FULLDPLX ? "full":"half");
	}
	bmsr = mdio_read(swcfg->skfd, swcfg->ifr, phy_id, MII_BMSR);
	printf("Link is %s\n", (bmsr & BMSR_LSTATUS) ? "up" : "down");
	return 0;
}/*end showMediaSetting*/
	
//#if defined(SQA_VERIFY)
/*___________________________________________________________________
**      function name: doSwitchRst
**      descriptions:
**         Switch Software Reset
**
**      parameters:
**         None
**         
**      global:
**           None
**             
**      return:
**			0:success
**	    	-1:fail
**
**      call:
**           None
**      
**      revision:
**      1. Frank 2012/03/01
**_______________________________________________________________
*/	
static int doSwitchRst(int argc, char *argv[], void *p)
{
	struct swconfig *swcfg=NULL;
	int reg;
	swcfg=getSwcfg();

	if (swcfg->swic_id == MT7530) {
	#if defined(JUDGE_SWITCH_SCENARIO_BY_751020_SUBMODEL)
		if (_DefaultUseExtMT7530)
		{
			gsw_reg_write(swcfg->skfd, swcfg->ifr, 0x7000, 0x80000003);
		}
		else
		{
			/* reset ethernet phy, ethernet switch, frame engine */
			reg = mdio_reg_read(swcfg->skfd, swcfg->ifr, CR_RSTCTRL2);
//??			reg |= (EPHY_RST | ESW_RST | FE_RST);
//7510 FPGA FE_RST not ready			reg |= (ESW_RST | FE_RST);
			reg |= (ESW_RST);
			mdio_reg_write(swcfg->skfd, swcfg->ifr, CR_RSTCTRL2, reg);

			/* de-assert reset ethernet switch, frame engine */
			reg = mdio_reg_read(swcfg->skfd, swcfg->ifr, CR_RSTCTRL2);
//			reg &= ~(EPHY_RST | ESW_RST | FE_RST);
			reg &= ~(ESW_RST | FE_RST);
			mdio_reg_write(swcfg->skfd, swcfg->ifr, CR_RSTCTRL2, reg);
		}
	#elif defined(DEFAULT_USE_EXT_SWIC)
		gsw_reg_write(swcfg->skfd, swcfg->ifr, 0x7000, 0x80000003);
	#else
		/* reset ethernet phy, ethernet switch, frame engine */
		reg = mdio_reg_read(swcfg->skfd, swcfg->ifr, CR_RSTCTRL2);
//		reg |= (EPHY_RST | ESW_RST | FE_RST);
//7510 FPGA FE_RST not ready		reg |= (ESW_RST | FE_RST);
		reg |= (ESW_RST);
		mdio_reg_write(swcfg->skfd, swcfg->ifr, CR_RSTCTRL2, reg);

		/* de-assert reset ethernet switch, frame engine */
		reg = mdio_reg_read(swcfg->skfd, swcfg->ifr, CR_RSTCTRL2);
//		reg &= ~(EPHY_RST | ESW_RST | FE_RST);
		reg &= ~(ESW_RST | FE_RST);
		mdio_reg_write(swcfg->skfd, swcfg->ifr, CR_RSTCTRL2, reg);
	#endif
		gsw_reg_write(swcfg->skfd, swcfg->ifr, 0x1fe0, 0xa00c7858);
		gsw_reg_write(swcfg->skfd, swcfg->ifr, 0x1ff4, 0x00004b1e);
		gsw_reg_write(swcfg->skfd, swcfg->ifr, 0x000c, 0x00071809);

		printf("Switch Software Reset Action:\n"
			   "	Switch System SOFTWARE RESET\n"
			   "	Enable Global Flow_Control Control\n"
			   "	Change port threshold\n"
			   "	Disable ARL lenghth check\n");

	} else if (swcfg->swic_id == RT62806) {
		gsw_reg_write(swcfg->skfd, swcfg->ifr, 0x7000, 0x80000003);
		gsw_reg_write(swcfg->skfd, swcfg->ifr, 0x1fe0, 0xa00c7858);
		gsw_reg_write(swcfg->skfd, swcfg->ifr, 0x1ff4, 0x00004b1e);
		gsw_reg_write(swcfg->skfd, swcfg->ifr, 0x000c, 0x00071809);

		printf("Switch Software Reset Action:\n"
			   "	Switch System SOFTWARE RESET\n"
			   "	Enable Global Flow_Control Control\n"
			   "	Change port threshold\n"
			   "	Disable ARL lenghth check\n");


	} else if (swcfg->swic_id == SWIC_RT63365){
	
		/* reset ethernet phy, ethernet switch, frame engine */
		reg = mdio_reg_read(swcfg->skfd, swcfg->ifr, CR_RSTCTRL2);
//		reg |= (EPHY_RST | ESW_RST | FE_RST);
		reg |= (ESW_RST | FE_RST);
		mdio_reg_write(swcfg->skfd, swcfg->ifr, CR_RSTCTRL2, reg);

		/* de-assert reset ethernet switch, frame engine */
		reg = mdio_reg_read(swcfg->skfd, swcfg->ifr, CR_RSTCTRL2);
//		reg &= ~(EPHY_RST | ESW_RST | FE_RST);
		reg &= ~(ESW_RST | FE_RST);
		mdio_reg_write(swcfg->skfd, swcfg->ifr, CR_RSTCTRL2, reg);

		printf("Switch Software Reset\n");

	}

	return 0;
}

/*___________________________________________________________________
**      function name: doRateControl
**      descriptions:
**         Switch Software Reset
**
**      parameters:
**         None
**         
**      global:
**           None
**             
**      return:
**			0:success
**	    	-1:fail
**
**      call:
**           None
**      
**      revision:
**      1. Frank 2012/03/01
**_______________________________________________________________
*/	
static int doRateControl(int argc, char *argv[], void *p)
{
	struct swconfig *swcfg=NULL;

	int i, j;
	u8 dir=0;
	u8 port=0;
	u32 reg, val, rate=0, exp=0, mantissa=0;

	int rate_limit[5] = {100, 1000, 10000, 100000, 1000000};
	
	swcfg=getSwcfg();
	dir=atoi(argv[1]);
	port=atoi(argv[2]);
	rate=atoi(argv[3]);

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806) || (swcfg->swic_id == SWIC_RT63365)) {
		if (dir==1){ 			// ingress
			reg = 0x1800 + port*0x100;
		} else if (dir==0) {	// exgress
			reg = 0x1040 + port*0x100;
		} else {
			return -1;
		}

	printf("dir: %d, port: %d, rate: %d\n", dir, port, rate);
	printf("write reg: %x\n", reg);

		if (rate<0)
			return -1;
/*
		for (i=0; i<5; i++){
			if (rate<=rate_limit[i]){
				exp = i;
				mantissa = rate;
				for (j=0; j<i; j++){
					mantissa /= 10;
				}
				break;
			}
		}

		if (mantissa==0){
			printf("input wrong rate value!!\n");
		}

		val = (1<<15) + (exp*0x100) + mantissa;
*/
		val = ((rate/32)<<16) + (1<<15) + (5<<8) + (1<<7) + 0xf; /* modify rate measurement period from 9:4ms to 0:1/128ms */

		printf("write val: %x\n", val);
		gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, val);
		
#if defined(TCSUPPORT_SOC_ETHER_WAN)
		if(4 == port)
		{
			mdio_reg_write(swcfg->skfd, swcfg->ifr, 0XBFB58000+reg, val);
			printf("SOC_ETHER_WAN write val: %x\n", val);

			reg = 0x10E0;
			val = mdio_reg_read(swcfg->skfd, swcfg->ifr, 0XBFB58000+reg)& 0xFFFFFF00;
			val |= 0x18;
			mdio_reg_write(swcfg->skfd, swcfg->ifr, 0XBFB58000+reg, val);
		}
#endif

		reg = 0x10E0;
		val = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg)& 0xFFFFFF00;
		val |= 0x18;
		gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, val);
		
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}

	return 0;
}

static int doQueueRateControl(int argc, char *argv[], void *p)
{
	struct swconfig *swcfg=NULL;

	int i, j;
	u8 queue=0;
	u8 port=0;
	u32 reg, val, rate=0, exp=0, mantissa=0;
	
	swcfg=getSwcfg();
	port=atoi(argv[1]);
	queue=atoi(argv[2]);
	rate=atoi(argv[3]);
	queue = 4-queue;
	
	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806) || (swcfg->swic_id == SWIC_RT63365)) {
		reg = 0x1000 + queue*0x8 + 0x4+ port * 0x100;
		
		printf("write reg: %x\n", reg);

		if (rate>1000000)
			return -1;

		while(rate>127)
		{
			exp++;
			rate/=10;
		}
		mantissa = rate;
		
		val = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg)& (0xfffff080);
		val |= (exp<<8) + mantissa + (0x1<<15);

		printf("write val: %x\n", val);
		gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, val);

#if defined(TCSUPPORT_SOC_ETHER_WAN)
		if(4 == port)
		{
			val = mdio_reg_read(swcfg->skfd, swcfg->ifr, 0XBFB58000+reg);
			val &= 0xfffff080;
			val |= (exp<<8) + mantissa + (0x1<<15);
			mdio_reg_write(swcfg->skfd, swcfg->ifr, 0XBFB58000+reg, val);
			printf("SOC_ETHER_WAN write val: %x\n", val);
		}
#endif
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}

	return 0;
}

/*___________________________________________________________________
**      function name: doMacControl
**      descriptions:
**         MAC Control
**
**      parameters:
**         None
**
**      global:
**           None
**
**      return:
**			0:success
**	    	-1:fail
**
**      call:
**           None
**
**      revision:
**      1. WPLin 2012/04/03
**_______________________________________________________________
*/
static int doMacControl(int argc, char *argv[], void *p)
{
	return subcmd(macCmds, argc, argv, p);
} /*end doMacControl*/

/*___________________________________________________________________
**      function name: doMacColLmt
**      descriptions:
**         Set Maximum Collision Count Limit.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments
**         p:function pointer
**
**      global:
**           swic
**
**      return:
**         0:succuces
**         -1:fail
**
**      call:
**           subcmd
**
**      revision:
**      1. WPLin 2012/04/09
**_______________________________________________________________
*/
static int doMacColLmt(int argc, char *argv[], void *p){
	u8 mtcc_lmt=0;
	u32 value, reg;

	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	mtcc_lmt=atoi(argv[1]);

	printf("mtcc_lmt: %d\n", mtcc_lmt);

	/*Check the input parameters is right or not.*/
	if (mtcc_lmt > REG_MTCC_LMT_RELMASK){
		printf(HELP_MACCTL_COLLMT);
		return -1;
	}

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
		reg = REG_CMACCR_ADDR;
		value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
		value &= (~REG_MTCC_LMT_MASK);
		value |= (mtcc_lmt<<REG_MTCC_LMT_OFFT);

		printf("write reg: %x, value: %x\n", reg, value);
		gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}

	return 0;
}/*end doMacColLmt*/

/*___________________________________________________________________
**      function name: doMacFC
**      descriptions:
**         Set Maximum Collision Count Limit.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments
**         p:function pointer
**
**      global:
**           swic
**
**      return:
**         0:succuces
**         -1:fail
**
**      call:
**           subcmd
**
**      revision:
**      1. WPLin 2012/04/19
**_______________________________________________________________
*/
static int doMacFC(int argc, char *argv[], void *p){
	u8 enable=0;
	u32 value, reg;

	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	enable=atoi(argv[1]);

	printf("enable: %d\n", enable);

	/*Check the input parameters is right or not.*/
	if (enable > 2){
		printf(HELP_MACCTL_FC);
		return -1;
	}

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
		reg = REG_GFCCR0_ADDR;
		value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
		value &= (~REG_FC_EN_MASK);
		value |= (enable<<REG_FC_EN_OFFT);

		printf("write reg: %x, value: %x\n", reg, value);
		gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}

	return 0;
}/*end doMacFC*/

/*___________________________________________________________________
**      function name: doPortMirror
**      descriptions:
**         Port Mirror Settings
**
**      parameters:
**         None
**
**      global:
**           None
**
**      return:
**			0:success
**	    	-1:fail
**
**      call:
**           None
**
**      revision:
**      1. WPLin 2012/05/08
**_______________________________________________________________
*/
static int doPortMirror(int argc, char *argv[], void *p)
{
	return subcmd(mirrorCmds, argc, argv, p);
} /*end doPortMirror*/

/*___________________________________________________________________
**      function name: doMirrorEn
**      descriptions:
**         Set Maximum Collision Count Limit.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments
**         p:function pointer
**
**      global:
**           swic
**
**      return:
**         0:succuces
**         -1:fail
**
**      call:
**           subcmd
**
**      revision:
**      1. WPLin 2012/05/08
**_______________________________________________________________
*/
static int doMirrorEn(int argc, char *argv[], void *p){
	u8 mirror_en;
	u8 mirror_port;
	u32 value, reg;

	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	mirror_en=atoi(argv[1]);
	mirror_port=atoi(argv[2]);

	printf("mirror_en: %d, mirror_port: %d\n", mirror_en, mirror_port);

	/*Check the input parameters is right or not.*/
	if ((mirror_en > 1) || (mirror_port > REG_MFC_MIRROR_PORT_RELMASK)){
		printf(HELP_MIRROR_EN);
		return -1;
	}

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
		reg = REG_MFC_ADDR;
		value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
		value &= (~REG_MFC_MIRROR_EN_MASK);
		value |= (mirror_en << REG_MFC_MIRROR_EN_OFFT);
		value &= (~REG_MFC_MIRROR_PORT_MASK);
		value |= (mirror_port << REG_MFC_MIRROR_PORT_OFFT);

		printf("write reg: %x, value: %x\n", reg, value);
		gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}

	return 0;
}/*end doMirrorEn*/

/*___________________________________________________________________
**      function name: doMirrorPortBased
**      descriptions:
**         Set Maximum Collision Count Limit.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments
**         p:function pointer
**
**      global:
**           swic
**
**      return:
**         0:succuces
**         -1:fail
**
**      call:
**           subcmd
**
**      revision:
**      1. WPLin 2012/05/08
**_______________________________________________________________
*/
static int doMirrorPortBased(int argc, char *argv[], void *p){
	u8 port, port_tx_mir, port_rx_mir, vlan_mis, acl_mir, igmp_mir;
	u32 value, reg;

	etherChannVlanCap_t* vlanCap=getEtherVlanCap();
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	port=atoi(argv[1]);
	port_tx_mir=atoi(argv[2]);
	port_rx_mir=atoi(argv[3]);
	acl_mir=atoi(argv[4]);
	vlan_mis=atoi(argv[5]);
	igmp_mir=atoi(argv[6]);

	printf("port:%d, port_tx_mir:%d, port_rx_mir:%d, acl_mir:%d, vlan_mis:%d, igmp_mir:%d\n", port, port_tx_mir, port_rx_mir, acl_mir, vlan_mis, igmp_mir);

	/*Check the input parameters is right or not.*/
//	if((port >= vlanCap->max_port_no) || (port_tx_mir > 1) || (port_rx_mir > 1) || (acl_mir > 1) || (vlan_mis > 1)){
	if((port >= 7) || (port_tx_mir > 1) || (port_rx_mir > 1) || (acl_mir > 1) || (vlan_mis > 1)){	// also allow CPU port (port6)
		printf(HELP_MIRROR_PORTBASED);
		return -1;
	}

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
		reg = REG_PCR_P0_ADDR + port*0x100;
		value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
		value &= ~(REG_PORT_TX_MIR_MASK | REG_PORT_RX_MIR_MASK | REG_PCR_ACL_MIR_MASK | REG_PCR_VLAN_MIS_MASK);
		value |= (port_tx_mir<<REG_PORT_TX_MIR_OFFT) + (port_rx_mir<<REG_PORT_RX_MIR_OFFT);
		value |= (acl_mir<<REG_PCR_ACL_MIR_OFFT) + (vlan_mis<<REG_PCR_VLAN_MIS_OFFT);

		printf("write reg: %x, value: %x\n", reg, value);
		gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);

		reg = REG_PIC_P0_ADDR + port*0x100;
		value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
		value &= ~(REG_PIC_IGMP_MIR_MASK);
		value |= (igmp_mir<<REG_PIC_IGMP_MIR_OFFT);

		printf("write reg: %x, value: %x\n", reg, value);
		gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}

	return 0;
}/*end doMirrorPortBased*/

/*___________________________________________________________________
**      function name: doPhySettings
**      descriptions:
**         PHY Settings
**
**      parameters:
**         None
**
**      global:
**           None
**
**      return:
**		0:success
**	    	-1:fail
**
**      call:
**           None
**
**      revision:
**      1. WPLin 2012/06/15
**_______________________________________________________________
*/
static int doPhySettings(int argc, char *argv[], void *p)
{
	return subcmd(phyCmds, argc, argv, p);
}/*end doPhySettings*/

/*___________________________________________________________________
**      function name: doPhyANEn
**      descriptions:
**         PHY MII Register Bit 0.12 "Auto-Negotiation Enable"
**
**      parameters:
**         None
**
**      global:
**           None
**
**      return:
**		0:success
**	    	-1:fail
**
**      call:
**           None
**
**      revision:
**      1. WPLin 2012/06/15
**_______________________________________________________________
*/
static int doPhyANEn(int argc, char *argv[], void *p)
{
	u8 port, auto_negotiation_en;
	u32 reg, value;
	u32 phy_addr, phy_reg, phy_value;

	etherChannVlanCap_t* vlanCap=getEtherVlanCap();
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	port=atoi(argv[1]);
	auto_negotiation_en=atoi(argv[2]);

	printf("port=%d, auto_negotiation_en:%d\n", port, auto_negotiation_en);

	/*Check the input parameters is right or not.*/
	if((port > vlanCap->max_port_no) || (auto_negotiation_en > 1)){
		printf(HELP_PHY_AN_EN);
		return -1;
	}

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
		// MII register bit 0.12 "Auto-Negotiation Enable"
		phy_addr = port2phyID(port);
		phy_reg = MII_BMCR;
//		phy_value = gsw_phy_read(swcfg->skfd, swcfg->ifr, phy_addr, phy_reg);
		phy_value = mdio_read(swcfg->skfd, swcfg->ifr, phy_addr, phy_reg);
		phy_value &= (~(1 << 12));
		phy_value |= (auto_negotiation_en << 12);
//		gsw_phy_write(swcfg->skfd, swcfg->ifr, phy_addr, phy_reg, phy_value);
		mdio_write(swcfg->skfd, swcfg->ifr, phy_addr, phy_reg, phy_value);
		printf("*phy_addr=%08lx phy_reg=%08lx phy_value:%08lx\r\n", phy_addr, phy_reg, phy_value);
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}

	return 0;
}/*end doPhyANEn*/

/*___________________________________________________________________
**      function name: doPhyFCEn
**      descriptions:
**         PHY MII Register Bit 4.10 "PAUSE operation for full duplex links"
**
**      parameters:
**         None
**
**      global:
**           None
**
**      return:
**		0:success
**	    	-1:fail
**
**      call:
**           None
**
**      revision:
**      1. WPLin 2012/06/18
**_______________________________________________________________
*/
static int doPhyFCEn(int argc, char *argv[], void *p)
{
	u8 port, full_duplex_pause_capable;
	u32 reg, value;
	u32 phy_addr, phy_reg, phy_value;

	etherChannVlanCap_t* vlanCap=getEtherVlanCap();
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	port=atoi(argv[1]);
	full_duplex_pause_capable=atoi(argv[2]);

	printf("port=%d, full_duplex_pause_capable:%d\n", port, full_duplex_pause_capable);

	/*Check the input parameters is right or not.*/
	if((port > vlanCap->max_port_no) || (full_duplex_pause_capable > 1)){
		printf(HELP_PHY_FC_EN);
		return -1;
	}

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
		// MII register 4.10 "PAUSE operation for full duplex links"
		phy_addr = port2phyID(port);
		phy_reg = MII_ADVERTISE;
//		phy_value = gsw_phy_read(swcfg->skfd, swcfg->ifr, phy_addr, phy_reg);
		phy_value = mdio_read(swcfg->skfd, swcfg->ifr, phy_addr, phy_reg);
		phy_value &= (~(1 << 10));
		phy_value |= (full_duplex_pause_capable << 10);
//		gsw_phy_write(swcfg->skfd, swcfg->ifr, phy_addr, phy_reg, phy_value);
		mdio_write(swcfg->skfd, swcfg->ifr, phy_addr, phy_reg, phy_value);
		printf("*phy_addr=%08lx phy_reg=%08lx phy_value:%08lx\r\n", phy_addr, phy_reg, phy_value);
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}

	return 0;
}/*end doPhyFCEn*/

/*___________________________________________________________________
**      function name: doStp
**      descriptions:
**         STP Settings
**
**      parameters:
**         None
**
**      global:
**           None
**
**      return:
**			0:success
**	    	-1:fail
**
**      call:
**           None
**
**      revision:
**      1. WPLin 2012/04/19
**_______________________________________________________________
*/
static int doStp(int argc, char *argv[], void *p)
{
	u8 port=0;
	u8 fid=0;
	u8 state=0;
	u32 value;
	u32 reg;

	etherChannVlanCap_t* vlanCap=getEtherVlanCap();
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	port=atoi(argv[1]);
	fid=atoi(argv[2]);
	state=atoi(argv[3]);

	printf("port: %d, fid: %d, state: %d\n", port, fid, state);

	/*Check the input parameters is right or not.*/
	if ((port > vlanCap->max_port_no) || (fid > 7)|| (state > 3)){
		printf(HELP_STP);
		return -1;
	}

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
		reg = REG_SSC_P0_ADDR + port*0x100;
		value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
		value &= (~(3 << (fid << 2)));
		value |= ((u32)state << (fid << 2));

		printf("write reg: %x, value: %x\n", reg, value);
		gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}

	return 0;
} /*end doStp*/

/*___________________________________________________________________
**      function name: doMib
**      descriptions:
**         MIB Display
**
**      parameters:
**         None
**
**      global:
**           None
**
**      return:
**		0:success
**	    	-1:fail
**
**      call:
**           None
**
**      revision:
**      1. WPLin 2012/09/05
**_______________________________________________________________
*/
static int doMib(int argc, char *argv[], void *p)
{
	return subcmd(mibCmds, argc, argv, p);
}/*end doPhySettings*/

/*___________________________________________________________________
**      function name: doMibClr
**      descriptions:
**         Clear MIB counter.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments
**         p:function pointer
**
**      global:
**           swic
**
**      return:
**         0:succuces
**         -1:fail
**
**      call:
**           subcmd
**
**      revision:
**      1. WPLin 2012/09/05
**_______________________________________________________________
*/
static int doMibClr(int argc, char *argv[], void *p){
	u32 value, reg;

	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();




	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
		if ((argc == 1)){
			reg = 0x4FE0;
			value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
			value &= ~(1 << 31);
			gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
			value |= (1 << 31);
			gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
		}
		else if ((argc == 2)){
			u32 ext_switch = atoi(argv[1]);	// optional argv[1] : ext_switch

			reg = 0x4FE0;
			value = gsw_reg_read2(swcfg->skfd, swcfg->ifr, ext_switch, reg);
			value &= ~(1 << 31);
			gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, value);
			value |= (1 << 31);
			gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, value);
		}
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
	}

	return 0;
}/*end doMibClr*/

//#endif //SQA_VERIFY

static int doDstq(int argc, char *argv[], void *p)
{
 	u32 value, reg;

	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	if(gsw_dstq_write(swcfg->skfd, swcfg->ifr, argc, argv )<0){
		printf(HELP_DSTQ);
	}

}/*end doMirrorEn*/

#ifdef TCSUPPORT_MT7530_SWITCH_API

static int doP6Cal(int argc, char *argv[], void *p)
{
	int ret_value;

	struct swconfig *swcfg=getSwcfg();

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
		return (-1);
	}
	ret_value = macMT7530doP6Cal();
	return 0;
}
static int doDsTrtcmSetActive(int argc, char *argv[], void *p)
{
	int ret_value;
	int mode = 0;

	struct swconfig *swcfg=getSwcfg();

	mode=(u8)atoi(argv[1]);

	if(mode>ACTIVED){
		printf("enable <0/1>.\n");
		return -1;
	}

	if ((swcfg->swic_id == MT7530)) {
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
		return (-1);
	}

	ret_value = macMT7530SetDsTrtcmEnable(mode);
	return 0;
}

static int doDsTrtcmSetRule(int argc, char *argv[], void *p)
{
	int ret_value;
	mt7530_switch_api_trtcm_t trtcm;

	struct swconfig *swcfg=getSwcfg();

	if ((swcfg->swic_id == MT7530)) {
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
		return (-1);
	}

	trtcm.trtcmId = atoi(argv[1]);
	if (trtcm.trtcmId > 31 || trtcm.trtcmId == 0)
	{
		printf("\r\ntrtcmId<1~31>.\n");
		return (-1);
	}
	trtcm.CBS = atoi(argv[2]);
	trtcm.PBS = atoi(argv[3]);
	trtcm.CIR = atoi(argv[4]);
	trtcm.PIR = atoi(argv[5]);
	printf("trtcm:id:%d, CBS:%d, PBS:%d, CIR:%d, PIR:%d\n", trtcm.trtcmId, trtcm.CBS, trtcm.PBS, trtcm.CIR, trtcm.PIR);
	
	ret_value = macMT7530SetDsTrtcm(&trtcm);
	
	return 0;
}

static int doDsTrtcmGetRule(int argc, char *argv[], void *p)
{
	int ret_value;
	int i = 0;
	mt7530_switch_api_trtcm_acl_t trtcm_acl;
	
	struct swconfig *swcfg=getSwcfg();
	
	if ((swcfg->swic_id == MT7530)) {
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
		return (-1);
	}

	ret_value = macMT7530GetDsTrtcm(&trtcm_acl);

	
	printf("Downstream Trtcm:%s\n", trtcm_acl.enable ?"Enable": "Disable");
	printf("      Id       CBS       PBS       CIR       PIR\n");
	for (i = 0; i < TRTCM_RULE_NUM; i++)
	{
		if ((trtcm_acl.trtcm_acl_table[i].trtcmId != 0xff) && (trtcm_acl.trtcm_acl_table[i].trtcmId != 0x0))
			printf("%8d%10d%10d%10d%10d\n", trtcm_acl.trtcm_acl_table[i].trtcmId, trtcm_acl.trtcm_acl_table[i].CBS, 
			trtcm_acl.trtcm_acl_table[i].PBS, trtcm_acl.trtcm_acl_table[i].CIR, trtcm_acl.trtcm_acl_table[i].PIR);
	}

}


static const cmds_t dsDsTrtcmCmds[] = {
	{"active",		doDsTrtcmSetActive,		0x12,	1,	"dstrtcm active <0/1>.\n"},
	{"rule",		doDsTrtcmSetRule,		0x12,	5,	"dstrtcm rule <id:1-31> <CBS> <PBS> <CIR> <PIR>.\n"},
	{"show",		doDsTrtcmGetRule,		0x12,	0,	NULL},
	{NULL,			NULL,			0x10,	0,	NULL},
};

static int doDsTrtcm(int argc, char *argv[], void *p){
	return subcmd(dsDsTrtcmCmds, argc, argv, p);
}/*end doDsP5Qos*/

static const cmds_t dsP5QosCmds[] = {
	{"active",		doDsP5QosSetActive,		0x12,	1,	"Usage: dsp5qos active <0/1>.\n"},
	{"ratectl",		doDsP5RateLimit,		0x12,	0,	NULL},
	{NULL,			NULL,			0x10,	0,	NULL},
};
static const cmds_t dsP5RateLimitCmds[] = {
	{"enable",		doDsP5RlSetActive,		0x12,	1,	"Usage: dsp5qos ratectl enable <0/1>.\n"},
	{"rate",			doDsP5RlSetValue,		0x12,	1,	"Usage: dsp5qos ratectl rate <rate(kbps): 1-1000000)>.\n"},
	{NULL,			NULL,			0x10,	0,	NULL},
};

static int doDsP5RateLimit(int argc, char *argv[], void *p)
{
	return subcmd(dsP5RateLimitCmds, argc, argv, p);
}

static int doDsP5RlSetActive(int argc, char *argv[], void *p)
{
	int ret_value;
	int mode = 0;

	struct swconfig *swcfg=getSwcfg();

	mode=(u8)atoi(argv[1]);

	if(mode>ACTIVED){
		printf("enable <0/1>.\n");
		return -1;
	}

	if ((swcfg->swic_id == MT7530)) {
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
		return (-1);
	}

	ret_value = macMT7530SetDsP5RateLimitEnable(mode);
	return 0;
}

static int doDsP5RlSetValue(int argc, char *argv[], void *p)
{
	int ret_value;
	int rate = 0;

	struct swconfig *swcfg=getSwcfg();

	if ((swcfg->swic_id == MT7530)) {
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
		return (-1);
	}

	rate =atoi(argv[1]);
	ret_value = macMT7530SetDsP5RateLimit(rate);
	return 0;
}

static int doDsP5Qos(int argc, char *argv[], void *p){
	return subcmd(dsP5QosCmds, argc, argv, p);
}/*end doDsP5Qos*/

static int doDsP5QosSetActive(int argc, char *argv[], void *p)
{
	int ret_value;
	int mode = 0;

	struct swconfig *swcfg=getSwcfg();

	mode=(u8)atoi(argv[1]);

	if(mode>ACTIVED){
		printf("active <0/1>.\n");
		return -1;
	}

	if ((swcfg->swic_id == MT7530)) {
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
		return (-1);
	}

	ret_value = macMT7530SetDsP5Qos(mode);
	return 0;
}


static const cmds_t dsStormCtrlCmds[] = {
	{"unit",		doStormCtrlUnit,		0x12,	1,	"stormctrl unit <0/1/2/3> (0: 64, 1: 256, 2: 1K, 3: 4K)\n"},
	{"type",		doStormCtrlType,		0x12,	1,	"stormCtrl type <0xf> (bit0: broadcast, bit1: multicast, bit2: unknow MAC unicast).\n"},
	{"mode",		doStormCtrlMode,		0x12,	1,	"stormctrl mode <0/1> (0: packet-base, 1: rate-base)\n"},
	{"period",		doStormCtrlPeriod,		0x12,	1,	"stormCtrl period <0/1> (0: 1 second, 1: 125us)\n"},
	{"port",		doStormCtrlPort,		0x12,	1,	"stormCtrl port <0xff> (default: 0x7f, bit0~6 means port0~6)\n"},
	{NULL,			NULL,			0x10,	0,	NULL},
};

static const cmds_t portModeCmds[] = {
	{"setspeed",		doSetPortSpeed,		0x12,	2,	"portmode setspeed port <0/1/2/3> speed:100/10\n"},
	{"getspeed",		doGetPortSpeed,		0x12,	1,	"portmode getspeed port <0/1/2/3>\n"},
	{"setduplex",		doSetPortDuplex,	0x12,	2,	"portmode setduplex port <0/1/2/3> duplex:Full/Half\n"},
	{"getduplex",		doGetPortDuplex,	0x12,	1,	"portmode getduplex port <0/1/2/3>\n"},
	{NULL,			NULL,			0x10,	0,	NULL},
};

static int doStormCtrl(int argc, char *argv[], void *p){
	return subcmd(dsStormCtrlCmds, argc, argv, p);
}

static int doPortMode(int argc, char *argv[], void *p){
	return subcmd(portModeCmds, argc, argv, p);
}

static int doStormCtrlPort(int argc, char *argv[], void *p){
	int ret_value;
	int port = 0;

	struct swconfig *swcfg=getSwcfg();

	port=(u16)atoi(argv[1]);

	if(port>0x7f){
		printf("port can't be larger than 0x7f.\n");
		return -1;
	}

	if ((swcfg->swic_id == MT7530)) {
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
		return (-1);
	}

	ret_value = macMT7530SetStormCtrlPort(port);
	return 0;
}

static int doStormCtrlUnit(int argc, char *argv[], void *p){
	int ret_value;
	int unit = 0;

	struct swconfig *swcfg=getSwcfg();

	unit=(u8)atoi(argv[1]);

	if(unit>3){
		printf("unit <0/1/2/3>.\n");
		return -1;
	}

	if ((swcfg->swic_id == MT7530)) {
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
		return (-1);
	}

	ret_value = macMT7530SetStormCtrlUnit(unit);
	return 0;
}

static int doStormCtrlType(int argc, char *argv[], void *p){
	int ret_value;
	int type = 0;

	struct swconfig *swcfg=getSwcfg();

	type=(u8)atoi(argv[1]);

	if(type>7){
		printf("type should not be larger than 7\n");
		return -1;
	}

	if ((swcfg->swic_id == MT7530)) {
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
		return (-1);
	}

	ret_value = macMT7530SetStormCtrlType(type);
	return 0;
}

static int doStormCtrlMode(int argc, char *argv[], void *p){
	int ret_value;
	int mode = 0;

	struct swconfig *swcfg=getSwcfg();

	mode=(u8)atoi(argv[1]);

	if(mode>1){
		printf("unit <0/1>.\n");
		return -1;
	}

	if ((swcfg->swic_id == MT7530)) {
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
		return (-1);
	}

	ret_value = macMT7530SetStormCtrlMode(mode);
	return 0;
}

static int doStormCtrlPeriod(int argc, char *argv[], void *p){
	int ret_value;
	int period = 0;

	struct swconfig *swcfg=getSwcfg();

	period=(u8)atoi(argv[1]);

	if(period>1){
		printf("unit <0/1>.\n");
		return -1;
	}

	if ((swcfg->swic_id == MT7530)) {
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
		return (-1);
	}

	ret_value = macMT7530SetStormCtrlPeriod(period);
	return 0;
}


static int doWPtest(int argc, char *argv[], void *p)
{
	int ret_value;
	u32 u32_value, i;
	u8 u8_value, portMac[6];
	struct swconfig *swcfg=getSwcfg();

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
		return (-1);
	}

	ret_value = macMT7530APIDbgDumpEn(0);

	// test (a)
	usleep(10000);
	ret_value = macMT7530GetMIBCounter(MIB_ID_RX_BCAST_CNT, 0, &u32_value);
	printf("macMT7530GetMIBCounter(MIB_ID_RX_BCAST_CNT, 0) finished : read CNT = %08X\n", u32_value);
	usleep(10000);
	ret_value = macMT7530GetPortRxLackDropPktCnt(0, &u32_value);
	printf("macMT7530GetPortRxLackDropPktCnt(0) finished : read CNT = %08X\n", u32_value);

	// test (b.0)
	usleep(10000);
	ret_value = macMT7530GetPortMac(0, portMac);
	if (ret_value == -1)
	{
		printf("port 0 MAC unknown\n");
	}
	else
	{
		printf("port 0 MAC : %02X%02X%02X%02X%02X%02X\n", portMac[0], portMac[1], portMac[2], portMac[3], portMac[4], portMac[5]);
	}

	// test (b.6)
	usleep(10000);
	ret_value = macMT7530GetPortMac(6, portMac);
	if (ret_value == -1)
	{
		printf("port 6 MAC unknown\n");
	}
	else
	{
		printf("port 6 MAC : %02X%02X%02X%02X%02X%02X\n", portMac[0], portMac[1], portMac[2], portMac[3], portMac[4], portMac[5]);
	}

	// test (c)
	usleep(10000);
	for (i = 0; i < 7; i ++)
	{
		ret_value = macMT7530GetConfigurationStatus(i);
		if (ret_value == -1)
		{
			printf("port %d link speed : (invalid)\n", i);
		}
		else
		{
			printf("port %d link speed : %02X\n", i, ret_value);
		}
	}

#if (0)	// will change phy register page, may cause system abnormal
	// test (c)
	usleep(10000);
	macMT7530GetDTEDCEInd(&u8_value, 0);
	printf("DTE/DCE Ind (port 0) : %02X\n", u8_value);

#endif

	// test (e)
	{
		u32 cir, cbs, ebs, pir;
		printf("---------------------------------------------------------------\n");
//		ret_value = macMT7530APIDbgDumpEn(1);
		usleep(10000);
		macMT7530GetPortPolicing(1, &cir, &cbs, &ebs);
		printf("macMT7530GetPortPolicing(port 1) = cir:%08X, cbs:%08X, ebs:%08X\n", cir, cbs, ebs);
		usleep(10000);
		ret_value = macMT7530GetPortPolicingEnable(1);
		printf("macMT7530GetPortPolicingEnable(port 1) = %08X\n", ret_value);
		usleep(10000);
		macMT7530SetPortPolicing(1, 10000, 2000, 3000);
		usleep(10000);
		macMT7530GetPortPolicing(1, &cir, &cbs, &ebs);
		printf("macMT7530GetPortPolicing(port 1) = cir:%08X, cbs:%08X, ebs:%08X\n", cir, cbs, ebs);
		usleep(10000);
		macMT7530SetPortPolicing(1, 5000, 2800, 3000);
		usleep(10000);
		macMT7530GetPortPolicing(1, &cir, &cbs, &ebs);
		printf("macMT7530GetPortPolicing(port 1) = cir:%08X, cbs:%08X, ebs:%08X\n", cir, cbs, ebs);
	}

	// test (f)
	{
		macMT7530GetAutoDetection(&u8_value, 0);
		usleep(10000);
		printf("macMT7530GetAutoDetection(0) = %02X\n", u8_value);

		macMT7530SetAutoDetection(0x02, 0);
		macMT7530GetAutoDetection(&u8_value, 0);
		usleep(10000);
		printf("macMT7530GetAutoDetection(0) = %02X\n", u8_value);

		macMT7530SetAutoDetection(0x12, 0);
		macMT7530GetAutoDetection(&u8_value, 0);
		usleep(10000);
		printf("macMT7530GetAutoDetection(0) = %02X\n", u8_value);

		macMT7530SetAutoDetection(0x10, 0);
		macMT7530GetAutoDetection(&u8_value, 0);
		usleep(10000);
		printf("macMT7530GetAutoDetection(0) = %02X\n", u8_value);

		macMT7530SetAutoDetection(0x02, 0);
	}

//	gswAPItest();

	return 0;
} // end of doWPtest()


static const cmds_t TrafficCmds[] = {
	{"set",		    doTrafficSet,		0x12,	2,	"Traffic set wan/wifi0/wifi1 trgmii/rgmii.\nwifi0:2.4G,wifi1:5G"},
	{"display",		doTrafficDisplay,	0x12,	0,	"Display current setting"},
    {"rgmii",       doRgmiiSetting,     0x12,   1,  "Rgmii interface enable/disable.\n"},
	{NULL,			NULL,			0x10,	0,	NULL},
};

static int doPortMatrix(int argc, char *argv[], void *p)
{
    int ret_value;
    u8 port_id;
    u8 port_group[4] = {0};
    struct swconfig *swcfg=getSwcfg();

	if ((swcfg->swic_id != MT7530) && (swcfg->swic_id != RT62806))
	{
		printf("\r\nCommand not support by this chip.\n");
		return (-1);
	}
	for(port_id=0; port_id<4; port_id++)
	{
	    port_group[port_id] = atoi(argv[port_id+1]);
	}
	ret_value = macMT7530SetPortMatrix(port_group);
	return 0;
}



static int doTrafficSel(int argc, char *argv[], void *p){
	struct swconfig *swcfg=getSwcfg();
	if (swcfg->swic_id == MT7530) {
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
		return (-1);
	}
    return subcmd(TrafficCmds, argc, argv, p);

}


static int doTrafficSet(int argc, char *argv[], void *p){
    u8 wan_type,interface;
    if(argc != 3){
        printf("Traffic set wan/wifi0/wifi1 trgmii/rgmii.\n\
            wifi0:2.4G,wifi1:5G");
    }
    if(strcmp(argv[1],"wan")==0){
        wan_type = WAN_TO_LAN;
    }else if(strcmp(argv[1],"wifi0")==0){
        wan_type = WIFI0_RA_TO_LAN;
    }else if(strcmp(argv[1],"wifi1")==0){
        wan_type = WIFI1_RAIX_TO_LAN;
    }else{
        printf("traffic type error\n");
        return 0;
    }
    if(strcmp(argv[2],"trgmii")==0){
        interface = TRGMII;
    }else if(strcmp(argv[2],"rgmii")==0){
        interface = RGMII;
    }else{
        printf("interface error\n");
        return 0;
    }
    macMT7530RgmiiTrafficSetting(wan_type, interface);
    macMT7530RgmiiDisplay();
    return 0;
}

static int doTrafficDisplay(int argc, char *argv[], void *p){
    macMT7530RgmiiDisplay();
    return 0;
}

static int doRgmiiSetting(int argc, char *argv[], void *p){
    
    if(strcmp(argv[1],ETHCMD_ENABLE)==0){
        macMT7530RgmiiSetting(ENABLE);
    }else if(strcmp(argv[1],ETHCMD_DISABLE)==0){
        macMT7530RgmiiSetting(DISABLE);
    }
    printf("Rgmii %s success \n", argv[1]);
    return 0;
}

static int doSetPortSpeed(int argc, char *argv[], void *p)
{
	int ret_value;
	u8 port;
	char *speed = NULL;

	struct swconfig *swcfg=getSwcfg();

	port=(u8)atoi(argv[1]);
	speed = argv[2];

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
		return (-1);
	}
	ret_value = macMT7530SetMaxBitRate(port, speed);
	
	return 0;
}

static int doGetPortSpeed(int argc, char *argv[], void *p)
{
	int ret_value;
	u8 port;
	char speed[10];

	struct swconfig *swcfg=getSwcfg();

	port=(u8)atoi(argv[1]);

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
		return (-1);
	}
	ret_value = macMT7530GetMaxBitRate(port, speed);
	printf("port %d speed: %s\n", port, speed);
	
	return 0;
}

static int doSetPortDuplex(int argc, char *argv[], void *p)
{
	int ret_value;
	u8 port;
	char *mode = NULL;

	struct swconfig *swcfg=getSwcfg();

	port=(u8)atoi(argv[1]);
	mode = argv[2];

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
		return (-1);
	}
	ret_value = macMT7530SetDuplexMode(port, mode);
	
	return 0;
}

static int doGetPortDuplex(int argc, char *argv[], void *p)
{
	int ret_value;
	u8 port;
	char mode[10];

	struct swconfig *swcfg=getSwcfg();

	port=(u8)atoi(argv[1]);

	if ((swcfg->swic_id == MT7530) || (swcfg->swic_id == RT62806)) {
	}
	else {
		printf("\r\nCommand not support by this chip.\n");
		return (-1);
	}
	ret_value = macMT7530GetDuplexMode(port, mode);
	printf("port %d mode: %s\n", port, mode);
		
	return 0;
}

#endif

