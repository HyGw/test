/******************************************************************************/
/*
 * Copyright (C) 1994-2008 TrendChip Technologies, Corp.
 * All Rights Reserved.
 *
 * TrendChip Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of TrendChip Technologies, Corp. and
 * shall not be reproduced, copied, disclosed, or used in whole or
 * in part for any reason without the prior express written permission of
 * TrendChip Technologies, Corp.
 */
/******************************************************************************/

#include <time.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/param.h>
#include <linux/types.h>
#ifndef TCSUPPORT_UPGRADE_WDOGREBOOT
#include <linux/reboot.h>
#include <sys/syscall.h>
#endif
#include <dirent.h>

#include "web.h"
#include "cfg_manager.h"
#include "utility.h"
/* Add by mtk06404 */
#include "cc.h"
#include "../../../tools/trx/trx.h"

#if defined(TCSUPPORT_CT_GUIACCESSLIMIT)
#include "../TR69_64/main/cwmp_extra.h"
#include "../TR69_64/cwmp/cwmpParameter_ct/Includes/cwmpParameter_table-share.h"
//#include "../TR69_64/rompager/Includes/AsTypes.h"
#endif

//here_20070327
#include <linux/version.h>

#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7592)
/*xyzhu_nj_20100415*/
#include	<sys/socket.h>    /* for connect and socket*/
#include	<sys/stat.h>
#include	<err.h>
#include	<errno.h>
#include	<asm/types.h>
#include	<linux/if.h>
#include	<linux/wireless.h>
#include	"oid.h"
#endif
#if defined(TCSUPPORT_CT_E8GUI)
#include <arpa/inet.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

#if defined(TCSUPPORT_CT_PON)
#if defined(TCSUPPORT_PONMGR) && (defined(TCSUPPORT_WAN_GPON) || defined(TCSUPPORT_WAN_EPON))
#include "api/mgr_api.h"
#endif
#endif
#if defined(TCSUPPORT_CT_DS_LIMIT)
#include <linux/qos_type.h>
#endif

#if KERNEL_VERSION(2,6,0) <= LINUX_VERSION_CODE
#define IS_KERNEL26 1
#else
#define IS_KERNEL26 0
#endif

#if IS_KERNEL26
#include <mtd/mtd-user.h>
#else
#include <linux/mtd/mtd.h>
#endif
#include <setjmp.h>
#if defined(TCSUPPORT_CT_PON)
#if defined(TCSUPPORT_PONMGR) && (defined(TCSUPPORT_WAN_GPON) || defined(TCSUPPORT_WAN_EPON))
#include "api/mgr_api.h"
#include "xpon.h"
#endif
#endif
#if defined(TCSUPPORT_CT_PON_CY_JX)
#include "wan_monitor.h"
#endif

extern int ntp_server_ret;
#if defined(TCSUPPORT_CT_PON)
#if defined(TCSUPPORT_WAN_GPON) || defined(TCSUPPORT_WAN_EPON)
	extern mxml_node_t **gTreePtr;
#endif	 
#endif

#if defined(TCSUPPORT_PRODUCTIONLINE) && defined(TCSUPPORT_RESERVEAREA_EXTEND)
extern Key_Parameters_Info prolinePara[];
#endif
#if defined(TCSUPPORT_CT_PON_SC)
#if !defined(TCSUPPORT_RESERVEAREA_EXTEND)
#error "TCSUPPORT_CT_PON_SC must compile with TCSUPPORT_RESERVEAREA_EXTEND!"
#endif
#endif

#if defined(TCSUPPORT_CT_E8GUI)

#if defined(TCSUPPORT_CT_PON_GD)
int gFlagBandRegDiag = 0;
#endif
int isbootset = 0;

#if defined(TCSUPPORT_CT_PON_CY_JX)
extern unsigned long secclock();
extern char * getUpDateTime(void);
#endif

#ifdef TCSUPPORT_WLAN_AC
void
check_wireless_ac_card(char *retval);
#endif

#if defined(TCSUPPORT_PON_ROSTELECOM)
/*0:normal check flag;2:check if added node, then form new romfile;1:new romfile formed*/
extern int add_node_if ;
#endif

void pingDiag(char *convertIP, char *buf_route, char *buf_times, int wanIPmode){
	char addrouteCMD[128]={0};
	char pingCMD[128]={0};
	char devName[64] = {0};

	snprintf(devName, sizeof(devName) - 1, "%s%s", 
		( (4 == wanIPmode) ? "ds." : "" ), buf_route);

	//execute [route add cmd]	
	sprintf(addrouteCMD,ADDROUTE_CMD, convertIP,devName);
	system(addrouteCMD);
	
	//execute [ping cmd]
#if defined(TCSUPPORT_CT_PON_GD)
	if(gFlagBandRegDiag)
	{
		snprintf(pingCMD, sizeof(pingCMD), REG_PING_CMD_TIMES, buf_times, convertIP, TMP_PING_OTHER_PATH);
	}
	else
	{
		snprintf(pingCMD, sizeof(pingCMD), PING_CMD_TIMES, buf_times, convertIP, TMP_PING_OTHER_PATH);
	}
#else
	sprintf(pingCMD,PING_CMD_TIMES, buf_times, convertIP, TMP_PING_OTHER_PATH);
#endif
	system(pingCMD);

	//execute [route delete cmd]			
	sprintf(addrouteCMD,DELROUTE_CMD, convertIP);
	system(addrouteCMD);
}

void ping6Diag(mxml_node_t *top, char *convertIP, char *buf_route, char *buf_times){
	char pingCMD[128]={0};
	char nodeName[3][MAX_NODE_NAME] = {0};
	char ip[40] = {0};
	int index = get_wanindex_by_name(buf_route);
	sprintf(nodeName[0], "WanInfo");
	sprintf(nodeName[1], "Entry%d", index);
	
	if(getAttrValue(top, nodeName,"GateWay6", ip) != TCAPI_PROCESS_OK)
		return;

	sprintf(pingCMD, ROUTE6_ADD_CMD, convertIP, ip, buf_route);
	tcdbg_printf("\n%s\n", pingCMD);
	system(pingCMD);
	memset(pingCMD, 0, sizeof(pingCMD));
	sprintf(pingCMD,PING6_CMD_TIMES, buf_route, buf_times, convertIP, TMP_PING_OTHER_PATH);
	tcdbg_printf("\n%s\n", pingCMD);
	system(pingCMD);
	memset(pingCMD, 0, sizeof(pingCMD));
	sprintf(pingCMD,ROUTE6_DEL_CMD, convertIP);
	tcdbg_printf("\n%s\n", pingCMD);
	system(pingCMD);
	
}

void tracerouteDiag(char *convertIP, char *buf_route, int wanIPmode){
	char traceroutebuf[128]={0};
	char devName[64] = {0};

	snprintf(devName, sizeof(devName) - 1, "%s%s", 
		( (4 == wanIPmode) ? "ds." : "" ), buf_route);

	//execute [route add cmd]	
	sprintf(traceroutebuf,ADDROUTE_CMD, convertIP,devName);
	system(traceroutebuf);
	
	//execute [traceroute cmd]
	memset(traceroutebuf,0,sizeof(traceroutebuf));
	sprintf(traceroutebuf,TRACE_CMD, convertIP, TMP_TRACE_PATH);
	system(traceroutebuf);

	//execute [route delete cmd]	
	memset(traceroutebuf,0,sizeof(traceroutebuf));
	sprintf(traceroutebuf,DELROUTE_CMD, convertIP);
	system(traceroutebuf);
}

void traceroute6Diag(mxml_node_t *top, char *convertIP, char *buf_route){
	char traceroutebuf[128]={0};
	char ip[40] = {0};
	char nodeName[3][MAX_NODE_NAME] = {0};
	int index = get_wanindex_by_name(buf_route);

	sprintf(nodeName[0], "WanInfo");
	sprintf(nodeName[1], "Entry%d", index);
	
	if(getAttrValue(top, nodeName,"GateWay6", ip) != TCAPI_PROCESS_OK)
		return;

	memset(traceroutebuf, 0, sizeof(traceroutebuf));
	sprintf(traceroutebuf, ROUTE6_ADD_CMD, convertIP, ip, buf_route);
	tcdbg_printf("\n%s\n", traceroutebuf);
	system(traceroutebuf);
	memset(traceroutebuf, 0, sizeof(traceroutebuf));
	sprintf(traceroutebuf,TRACE6_CMD, buf_route, convertIP, TMP_TRACE_PATH);
	tcdbg_printf("\n%s\n", traceroutebuf);
	system(traceroutebuf);
	memset(traceroutebuf, 0, sizeof(traceroutebuf));
	sprintf(traceroutebuf,ROUTE6_DEL_CMD, convertIP);
	tcdbg_printf("\n%s\n", traceroutebuf);
	system(traceroutebuf);
}
#endif

#define BOOT 0

#if defined(TCSUPPORT_CT_DEFAULTPARA_STORE) || defined(TCSUPPORT_KEYPARA_STORE)
mxml_node_t *g_fsDefaultRomfileTree  = NULL;//filesystem default romfile tree
int keyParaRestoreFlag = 0;
int operateNodePara(Key_Parameters_Info_Ptr parainfo,mxml_node_t* oldtree,mxml_node_t* newtree)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	mxml_node_t *parentNode = NULL;
	mxml_node_t *curNode = NULL;
	mxml_node_t *newCurNode = NULL;
	mxml_attr_t *attr = NULL;
	int i,j;
	int idx = 0, found_ignore = 0;
	ignore_list *pList = NULL;

	if(!(parainfo->flag & OP_NODE))
	{
		tcdbg_printf("\r\noperateNodePara:the function only do node operation,you use the wrong function ,please check it!!!");
		return -1;
	}

	memset(nodeName, 0, sizeof(nodeName));
	splitName(parainfo->node, nodeName, "_");

	newCurNode = newtree;
	curNode = oldtree;
	for(i = 0; i < MAX_ARG_NUM; i++)
	{
		if(strlen(nodeName[i])==0)
			break;
		curNode = mxmlFindElement(curNode, curNode, nodeName[i],
			NULL, NULL, MXML_DESCEND);
		if(!curNode)
		{
			tcdbg_printf("\r\noperateNodePara:the node(%s) is not in running file(means no change)",parainfo->node);
			return 0;//must return 0,because curNode ==NULL not means error!!!
		}
		
		parentNode = newCurNode;
		newCurNode = mxmlFindElement(newCurNode, newCurNode, nodeName[i],
			NULL, NULL, MXML_DESCEND);
	
		if(parainfo->flag & OP_NODE_LEVEL_SINGLE)
		{
			//because we set the all attributes of the node,so we need to create it if it's not exist!!! 
			if(!newCurNode)
			{
				newCurNode = mxmlNewElement(parentNode,nodeName[i]);
				if(newCurNode == NULL )
				{  
					tcdbg_printf("\r\noperateAttributePara:create a new node(%s) failed",parainfo->node);
					return -1;
				}
			}
		}	
	}

	if(parainfo->flag & OP_NODE_LEVEL_ALL)
	{
		/*First remove from running tree, then add to default tree*/
		//because we replace the node(include sub node) with running romfile,so we need to delete it first!!
		if(newCurNode)
		{
			mxmlDelete(newCurNode);
		}
		mxmlRemove(curNode);
		mxmlAdd(parentNode, MXML_ADD_AFTER, MXML_ADD_TO_PARENT, curNode);
	}
	else if(parainfo->flag & OP_NODE_LEVEL_SINGLE)
	{
		attr = curNode->value.element.attrs;
		for(j = 0; j < curNode->value.element.num_attrs; j++ )
		{
			if ( parainfo->flag & OP_ATTRIBUTE_IGNORE_LIST )
			{
				found_ignore = 0;
				pList = (ignore_list *)parainfo->attr;
				if ( pList )
				{
					for ( idx = 0; idx < pList->nodelen; idx ++ )
					{
						if ( 0 == strcmp(pList->attrlist[idx], attr->name) )
						{
							found_ignore = 1;
							break;
						}
					}
				}

				if ( found_ignore )
				{
					attr++;
					continue;
				}
			}
			mxmlElementSetAttr(newCurNode, attr->name, attr->value);
			attr++;
		}
	}
	else
	{
		tcdbg_printf("\r\noperateNodePara:not support this type!!");
	}

	return 0;
	
}

int operateAttributePara(Key_Parameters_Info_Ptr parainfo,mxml_node_t* oldtree,mxml_node_t* newtree)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	mxml_node_t *parentNode = NULL;
	mxml_node_t *newCurNode = NULL;
	mxml_node_t *curNode = NULL;
	int i;

	if(!(parainfo->flag & OP_ATTRIBUTE))
	{
		tcdbg_printf("\r\noperateAttributePara:the function only do attribute operation,you use the wrong function ,please check it!!!");
		return -1;
	}
	
	memset(nodeName, 0, sizeof(nodeName));
	splitName(parainfo->node, nodeName, "_");
	
	newCurNode = newtree;
	curNode = oldtree;
	for(i = 0; i < MAX_ARG_NUM; i++)
	{
		if(strlen(nodeName[i])==0)
			break;
		curNode = mxmlFindElement(curNode, curNode, nodeName[i],
			NULL, NULL, MXML_DESCEND);
		if(!curNode)
		{
			tcdbg_printf("\r\noperateAttributePara:the node(%s) is not in running file(means no change)",parainfo->node);
			return 0;
		}
		
		parentNode = newCurNode;
		newCurNode = mxmlFindElement(newCurNode, newCurNode, nodeName[i],
			NULL, NULL, MXML_DESCEND);
		if(!newCurNode)
		{
			newCurNode = mxmlNewElement(parentNode,nodeName[i]);
			if(newCurNode == NULL )
			{  
				tcdbg_printf("\r\noperateAttributePara:create a new node(%s) failed",parainfo->node);
				return -1;
			}
		}
	}
	
	
#if 0
	attr = curNode->value.element.attrs;
	for(j = 0; j < curNode->value.element.num_attrs; j++ )
	{
		if(!strcmp(attr->name,parainfo->attr))
		{
			mxmlElementSetAttr(newCurNode, attr->name, attr->value);
			break;
		}
		attr++;
	}

	if(j == curNode->value.element.num_attrs)
	{
		tcdbg_printf("\r\noperateAttributePara:not find attribute(%s)",parainfo->attr);
	}
#endif
	mxmlElementSetAttr(newCurNode, parainfo->attr, mxmlElementGetAttr(curNode,parainfo->attr));
	//tcdbg_printf("\n==>operateAttributePara, attr: %s, value: %s\n", parainfo->attr, mxmlElementGetAttr(curNode,parainfo->attr));
	return 0;
	
}

#if (defined(TCSUPPORT_CT_PON) || defined(TCSUPPORT_CT_E8B_ADSL)) || defined(TCSUPPORT_CUC)
int operateNodeParaEnhance(Key_Parameters_Info_Enhance_Ptr parainfo,mxml_node_t* oldtree,mxml_node_t* newtree)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char nodeName_new[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	mxml_node_t *parentNode = NULL;
	mxml_node_t *curNode = NULL;
	mxml_node_t *newCurNode = NULL;
	mxml_attr_t *attr = NULL;
	int i,j;

	if(!(parainfo->flag & OP_NODE))
	{
		tcdbg_printf("\r\noperateNodeParaEnhance:the function only do node operation,you use the wrong function ,please check it!!!");
		return -1;
	}

	memset(nodeName, 0, sizeof(nodeName));
	splitName(parainfo->node, nodeName, "_");
	memset(nodeName_new, 0, sizeof(nodeName_new));
	splitName(parainfo->nodenew, nodeName_new, "_");

	newCurNode = newtree;
	curNode = oldtree;
	for(i = 0; i < MAX_ARG_NUM; i++)
	{
		if(strlen(nodeName[i])==0)
			break;
		curNode = mxmlFindElement(curNode, curNode, nodeName[i],
			NULL, NULL, MXML_DESCEND);
		if(!curNode)
		{
			tcdbg_printf("\r\noperateNodeParaEnhance:the node(%s) is not in running file(means no change)",parainfo->node);
			return 0;//must return 0,because curNode ==NULL not means error!!!
		}

		parentNode = newCurNode;
		newCurNode = mxmlFindElement(newCurNode, newCurNode, nodeName_new[i],
			NULL, NULL, MXML_DESCEND);
	
		if(parainfo->flag & OP_NODE_LEVEL_SINGLE)
		{
			//because we set the all attributes of the node,so we need to create it if it's not exist!!! 
			if(!newCurNode)
			{
				newCurNode = mxmlNewElement(parentNode,nodeName_new[i]);
				if(newCurNode == NULL )
				{  
					tcdbg_printf("\r\noperateNodeParaEnhance:create a new node(%s) failed",parainfo->node);
					return -1;
				}
			}
		}	
	}

	if(parainfo->flag & OP_NODE_LEVEL_SINGLE)
	{
		attr = curNode->value.element.attrs;
		for(j = 0; j < curNode->value.element.num_attrs; j++ )
		{
			mxmlElementSetAttr(newCurNode, attr->name, attr->value);
			attr++;
		}
	}
	else
	{
		tcdbg_printf("\r\noperateNodeParaEnhance:not support this type!!");
	}

	return 0;
}
#endif

mxml_node_t *keypara_load_romfile(char *pathname)
{
	FILE *fp=NULL;
	mxml_node_t *tree=NULL;

#if defined(TCSUPPORT_CT_BACKUPROMFILEENCHANCEMENT)
	int flag = -1;
	/*Load romfile  file*/
	flag = check_checksum(pathname,LOAD_ROMFILE_FILE_NAME);
	if(flag <0)
	{
		tcdbg_printf(ROMFILE_CHECKSUM_ERR_MSG);
		unlink(LOAD_ROMFILE_FILE_NAME);
		return NULL;
	}
	fp = fopen(LOAD_ROMFILE_FILE_NAME, "r");
#else
	fp = fopen(pathname, "r");
#endif

	if(fp == NULL){
		tcdbg_printf("\r\nkeypara_load_romfile:open file %s fail!",pathname);
		return NULL;
	}

	tree = mxmlLoadFile(NULL, fp, MXML_TEXT_CALLBACK);
	if(verify_romfile(tree) == FAIL)
	{
		tcdbg_printf("\r\nkeypara_load_romfile:verify file %s fail!",pathname);
		if(tree)
		{
			mxmlDelete(tree);
			tree = NULL;
		}
	}
	
	fclose(fp);
#if defined(TCSUPPORT_CT_BACKUPROMFILEENCHANCEMENT)
	unlink(LOAD_ROMFILE_FILE_NAME);
#endif
	return tree;
}
#endif

#if defined(TCSUPPORT_KEYPARA_STORE)
char *ignoreWLan_Common_attr[] = 
{
	  "APOn"
};

ignore_list ignoreWLanList = 
{
	  ignoreWLan_Common_attr
	, sizeof(ignoreWLan_Common_attr)/sizeof(char *)
};

Key_Parameters_Info keyPara[]=
{
	//follow is node parameters
#if defined(TCSUPPORT_CT_DSL_EX)
	{OP_NODE|OP_NODE_LEVEL_ALL,"VirServerATM",NULL,operateNodePara},//VirServer information
	{OP_NODE|OP_NODE_LEVEL_ALL,"VirServerPTM",NULL,operateNodePara},//VirServer information
#endif
	{OP_NODE|OP_NODE_LEVEL_ALL,"VirServer",NULL,operateNodePara},//VirServer information
	{OP_NODE|OP_NODE_LEVEL_ALL,"Wan",NULL,operateNodePara},//wan information
	{OP_NODE|OP_NODE_LEVEL_ALL,"CtDiagnostic",NULL,operateNodePara},//remote link information
#if defined(CT_COM_DEVICEREG)
	{OP_NODE|OP_NODE_LEVEL_ALL,"deviceAccount",NULL,operateNodePara},//remote link information
#endif
#if defined(TCSUPPORT_CT_PON_GD) || defined(TCSUPPORT_CT_PON_CZ_HN) || defined(TCSUPPORT_CT_PON_CZ_CQ)
	{OP_NODE|OP_NODE_LEVEL_SINGLE,"Account_Entry0",NULL,operateNodePara},//admin information
#else
	{OP_NODE|OP_NODE_LEVEL_SINGLE|OP_LONG_RESETBAK|OP_JSAccount_ONLY,"Account_Entry0",NULL,operateNodePara},//admin information
#endif
	{OP_NODE|OP_NODE_LEVEL_SINGLE|OP_ATTRIBUTE_IGNORE_LIST,
	"WLan_Common", (char *)&ignoreWLanList,operateNodePara}, /* wlan information */
	{OP_NODE|OP_NODE_LEVEL_SINGLE,"WLan_Entry1",NULL,operateNodePara},//wlan information
	{OP_NODE|OP_NODE_LEVEL_SINGLE,"WLan_Entry2",NULL,operateNodePara},//wlan information
	{OP_NODE|OP_NODE_LEVEL_SINGLE,"WLan_Entry3",NULL,operateNodePara},//wlan information
#if defined(TCSUPPORT_CT_DSL_EX)
	{OP_NODE|OP_NODE_LEVEL_ALL,"WanATM",NULL,operateNodePara},//wan atm information
	{OP_NODE|OP_NODE_LEVEL_ALL,"WanPTM",NULL,operateNodePara},//wan ptm information
#endif
	//follow is attribute parameters	
#ifdef TCSUPPORT_CWMP
#if defined(TCSUPPORT_CT_PON_GD) || defined(TCSUPPORT_CT_PON_CZ_HN) || defined(TCSUPPORT_CT_PON_CZ_CQ)
	{OP_ATTRIBUTE,"Cwmp_Entry","acsUrl",operateAttributePara},//acs url
	{OP_ATTRIBUTE,"Cwmp_Entry","acsUserName",operateAttributePara},//tr069 certification information
	{OP_ATTRIBUTE,"Cwmp_Entry","acsPassword",operateAttributePara},
	{OP_ATTRIBUTE,"Cwmp_Entry","conReqUserName",operateAttributePara},
	{OP_ATTRIBUTE,"Cwmp_Entry","conReqPassword",operateAttributePara},
#else
#if defined(TCSUPPORT_CUC)
	{OP_ATTRIBUTE|OP_LONG_RESETBAK|OP_JSCWMP_ONLY|OP_CWMP_ONLY,"Cwmp_Entry","acsUrl",operateAttributePara},//acs url
	{OP_ATTRIBUTE|OP_LONG_RESETBAK|OP_CWMP_ONLY,"Cwmp_Entry","acsUserName",operateAttributePara},//tr069 certification information
	{OP_ATTRIBUTE|OP_LONG_RESETBAK|OP_CWMP_ONLY,"Cwmp_Entry","acsPassword",operateAttributePara},
	{OP_ATTRIBUTE|OP_LONG_RESETBAK|OP_JSCWMP_ONLY|OP_CWMP_ONLY,"Cwmp_Entry","conReqUserName",operateAttributePara},
	{OP_ATTRIBUTE|OP_LONG_RESETBAK|OP_JSCWMP_ONLY|OP_CWMP_ONLY,"Cwmp_Entry","conReqPassword",operateAttributePara},
#else
	{OP_ATTRIBUTE|OP_LONG_RESETBAK|OP_JSCWMP_ONLY,"Cwmp_Entry","acsUrl",operateAttributePara},//acs url
	{OP_ATTRIBUTE|OP_LONG_RESETBAK|OP_JSCWMP_ONLY,"Cwmp_Entry","acsUserName",operateAttributePara},//tr069 certification information
	{OP_ATTRIBUTE|OP_LONG_RESETBAK|OP_JSCWMP_ONLY,"Cwmp_Entry","acsPassword",operateAttributePara},
	{OP_ATTRIBUTE|OP_LONG_RESETBAK|OP_JSCWMP_ONLY,"Cwmp_Entry","conReqUserName",operateAttributePara},
	{OP_ATTRIBUTE|OP_LONG_RESETBAK|OP_JSCWMP_ONLY,"Cwmp_Entry","conReqPassword",operateAttributePara},
#endif
#endif
#if !defined(TCSUPPORT_C1_CUC) && !defined(TCSUPPORT_CUC)
    {OP_ATTRIBUTE,"Cwmp_Entry","cwmpflags",operateAttributePara},
#endif
#endif
	{OP_ATTRIBUTE,"Accesslimit_Common","totalnum",operateAttributePara},
#if defined(TCSUPPORT_CT_PON) || defined(TCSUPPORT_CT_WAN_PTM)
#if !defined(TCSUPPORT_CUC)
	{OP_ATTRIBUTE,"Timezone_Entry","SERVER",operateAttributePara},
#endif
#if defined(TCSUPPORT_CT_2NTP)
#if !defined(TCSUPPORT_CUC)
	{OP_ATTRIBUTE,"Timezone_Entry","SERVER2",operateAttributePara},
#endif
#if defined(TCSUPPORT_CT_5NTP)
	{OP_ATTRIBUTE,"Timezone_Entry","SERVER3",operateAttributePara},
	{OP_ATTRIBUTE,"Timezone_Entry","SERVER4",operateAttributePara},
	{OP_ATTRIBUTE,"Timezone_Entry","SERVER5",operateAttributePara},
#endif
#endif
#if defined(TCSUPPORT_CT_NTPSERVERTYPE)
#if !defined(TCSUPPORT_CUC)
	{OP_ATTRIBUTE,"Timezone_Entry","NTPServerType",operateAttributePara},
#endif
#else
	{OP_ATTRIBUTE,"Timezone_Entry","AddRoute",operateAttributePara},
#endif
	{OP_ATTRIBUTE,"Timezone_Entry","SYNCTIME",operateAttributePara},
#endif
#if defined(TCSUPPORT_CT_PON)
#ifdef TCSUPPORT_EPON_OAM_CTC
	{OP_ATTRIBUTE|OP_LONG_RESETBAK|OP_CWMP_ONLY,"EPON_LOIDAuth","LOID0",operateAttributePara},
	{OP_ATTRIBUTE|OP_LONG_RESETBAK|OP_CWMP_ONLY,"EPON_LOIDAuth","Password0",operateAttributePara},
#endif
#if defined(TCSUPPORT_CMCC)
	{OP_ATTRIBUTE|OP_LONG_RESETBAK|OP_CWMP_ONLY,"GPON_ONU","Password",operateAttributePara},
#else
	{OP_ATTRIBUTE|OP_LONG_RESETBAK|OP_CWMP_ONLY,"GPON_LOIDAuth","LOID",operateAttributePara},
	{OP_ATTRIBUTE|OP_LONG_RESETBAK|OP_CWMP_ONLY,"GPON_LOIDAuth","Password",operateAttributePara},
#endif
#if VOIP
	{OP_NODE|OP_NODE_LEVEL_ALL,"VoIPBasic",NULL,operateNodePara},//VirServer information	
	{OP_ATTRIBUTE,"VoIPAdvanced_Common","SIPDomain",operateAttributePara},
	{OP_NODE|OP_NODE_LEVEL_ALL,"VoIPH248",NULL,operateNodePara},
#endif
#endif
#if defined(TCSUPPORT_CT_DSL_EX)
	{OP_ATTRIBUTE|OP_LONG_RESETBAK,"Sys_Entry","DslMode",operateAttributePara},
#endif
#ifdef IPV6
#if !defined(TCSUPPORT_CUC)
	{OP_ATTRIBUTE,"Radvd_Entry","AutoPrefix",operateAttributePara},
	{OP_ATTRIBUTE,"Radvd_Entry","DelegatedWanConnection",operateAttributePara},
	{OP_ATTRIBUTE,"Dhcp6s_Entry","DNSType",operateAttributePara},
	{OP_ATTRIBUTE,"Dhcp6s_Entry","DNSWANConnection",operateAttributePara},
#endif
#endif
#if !defined(TCSUPPORT_CT_PON)
	{OP_NODE|OP_NODE_LEVEL_SINGLE,"QoS_Entry22",NULL,operateNodePara},
	{OP_NODE|OP_NODE_LEVEL_SINGLE,"QoS_Entry23",NULL,operateNodePara},
	{OP_NODE|OP_NODE_LEVEL_SINGLE,"QoS_Entry24",NULL,operateNodePara},
	{OP_NODE|OP_NODE_LEVEL_SINGLE,"QoS_Entry25",NULL,operateNodePara},
	{OP_NODE|OP_NODE_LEVEL_SINGLE,"QoS_Entry26",NULL,operateNodePara},
	{OP_NODE|OP_NODE_LEVEL_SINGLE,"QoS_Entry27",NULL,operateNodePara},
	{OP_NODE|OP_NODE_LEVEL_SINGLE,"QoS_Entry28",NULL,operateNodePara},
	{OP_NODE|OP_NODE_LEVEL_SINGLE,"QoS_Entry29",NULL,operateNodePara},
#endif
#if defined(TCSUPPORT_IGMP_PROXY)
	{OP_ATTRIBUTE,"IGMPproxy_Entry","UpstreamIF",operateAttributePara},
#if defined(TCSUPPORT_CT_WAN_PTM)
	{OP_ATTRIBUTE,"IGMPproxyATM_Entry","UpstreamIF",operateAttributePara},
	{OP_ATTRIBUTE,"IGMPproxyPTM_Entry","UpstreamIF",operateAttributePara},
#endif
#endif
#if defined(TCSUPPORT_MLD_PROXY)
	{OP_ATTRIBUTE,"MLDproxy_Entry","UpstreamIF",operateAttributePara},
#if defined(TCSUPPORT_CT_WAN_PTM)
	{OP_ATTRIBUTE,"MLDproxyATM_Entry","UpstreamIF",operateAttributePara},
	{OP_ATTRIBUTE,"MLDproxyPTM_Entry","UpstreamIF",operateAttributePara},
#endif
#endif
#if defined(TCSUPPORT_CT_VLAN_BIND)
	{OP_NODE|OP_NODE_LEVEL_ALL,"VlanBind",NULL,operateNodePara},
#endif
#if !defined(TCSUPPORT_CUC)
	{OP_NODE|OP_NODE_LEVEL_SINGLE,"LogicID_Entry",NULL,operateNodePara},
#endif
#if defined(TCSUPPORT_CT_LONG_RESETBTN)
#if defined(CT_COM_DEVICEREG)
	{OP_ATTRIBUTE|OP_LONG_RESETBAK|OP_CWMP_ONLY,"deviceAccount_Entry","userName",operateAttributePara},
	{OP_ATTRIBUTE|OP_LONG_RESETBAK|OP_CWMP_ONLY,"deviceAccount_Entry","userPasswordDEV",operateAttributePara},
#endif
#if !defined(TCSUPPORT_CUC) && !defined(TCSUPPORT_CMCC)
	{OP_ATTRIBUTE|OP_LONG_RESETBAK|OP_CWMP_ONLY,"LogicID_Entry","Username",operateAttributePara},
	{OP_ATTRIBUTE|OP_LONG_RESETBAK|OP_CWMP_ONLY,"LogicID_Entry","Password",operateAttributePara},
#if defined(TCSUPPORT_CT_PON_JS)
	{OP_ATTRIBUTE|OP_LONG_RESETBAK|OP_CWMP_ONLY,"LogicID_Entry","isRegistered",operateAttributePara},
#endif
#endif
#else
#if defined(CT_COM_DEVICEREG)
	{OP_ATTRIBUTE|OP_CWMP_ONLY,"deviceAccount_Entry","userName",operateAttributePara},
	{OP_ATTRIBUTE|OP_CWMP_ONLY,"deviceAccount_Entry","userPasswordDEV",operateAttributePara},
#endif
#if !defined(TCSUPPORT_CUC)
	{OP_ATTRIBUTE|OP_CWMP_ONLY,"LogicID_Entry","Username",operateAttributePara},
	{OP_ATTRIBUTE|OP_CWMP_ONLY,"LogicID_Entry","Password",operateAttributePara},
#if defined(TCSUPPORT_CT_PON_JS)
	{OP_ATTRIBUTE|OP_CWMP_ONLY,"LogicID_Entry","isRegistered",operateAttributePara},
#endif
#endif
#endif
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
	{OP_ATTRIBUTE,"SimCard_Entry","CardKey",operateAttributePara},
	{OP_ATTRIBUTE,"SimCard_Entry","CardSN",operateAttributePara},
	{OP_ATTRIBUTE,"SimCard_Entry","cpeFlag",operateAttributePara},
#endif
#if defined(TCSUPPORT_CT_PHONEAPP)
#if defined(TCSUPPORT_CT_JOYME)
	{OP_NODE|OP_NODE_LEVEL_ALL, "VPN", NULL, operateNodePara},
	{OP_ATTRIBUTE, MOBILE_ENTRY, "InterfaceRight", operateAttributePara},
	{OP_ATTRIBUTE, MOBILE_OSRIGHTENTRY, "CPU", operateAttributePara},
	{OP_ATTRIBUTE, MOBILE_OSRIGHTENTRY, "RAM", operateAttributePara},
	{OP_ATTRIBUTE, MOBILE_OSRIGHTENTRY, "FLASH", operateAttributePara},
	{OP_ATTRIBUTE, MOBILE_OSRIGHTENTRY, "SOCKET", operateAttributePara},
	{OP_ATTRIBUTE, MOBILE_OSRIGHTENTRY, "PORT", operateAttributePara},
	{OP_ATTRIBUTE, MOBILE_OSRIGHTENTRY, "thread", operateAttributePara},
#else
	{OP_ATTRIBUTE|OP_LONG_RESETBAK,"mobile_Entry","InterfaceRight",operateAttributePara},
#endif
	{OP_ATTRIBUTE|OP_LONG_RESETBAK|OP_CWMP_ONLY,"mobile_Entry","MgtURL",operateAttributePara},
	{OP_ATTRIBUTE|OP_LONG_RESETBAK|OP_CWMP_ONLY,"mobile_Entry","Port",operateAttributePara},
	{OP_ATTRIBUTE|OP_LONG_RESETBAK,"mobile_Entry","Heartbeat",operateAttributePara},
	{OP_ATTRIBUTE|OP_LONG_RESETBAK,"mobile_Entry","Ability",operateAttributePara},
	{OP_ATTRIBUTE|OP_LONG_RESETBAK,"mobile_Entry","LocatePort",operateAttributePara},
	{OP_ATTRIBUTE|OP_LONG_RESETBAK,"mobile_Entry","OperateSAddr",operateAttributePara},
	{OP_ATTRIBUTE|OP_LONG_RESETBAK,"mobile_Entry","OperateSPort",operateAttributePara},
	{OP_NODE|OP_NODE_LEVEL_SINGLE|OP_CWMP_ONLY,"mobile_DnsRedirect",NULL,operateNodePara},
#endif
	{OP_AT_END,NULL,NULL,NULL}
};


#if defined(TCSUPPORT_CT_LONG_RESETBTN) || defined(TCSUPPORT_CUC)
int TR69_NodeIdx = -1;
#if (defined(TCSUPPORT_CT_PON) || defined(TCSUPPORT_CT_E8B_ADSL)) || defined(TCSUPPORT_CUC)
char TR69_VLANMODE[12] = {0}, TR69_VLANID[12] = {0}, TR69_PBIT[12] = {0};
int currEmptyPVC_index = -1, restore_TR69NodeIdx = -1;
#define LONGRESET_TR69_KEY1 "VLANMode"
#if defined(TCSUPPORT_CT_E8B_ADSL)
#define LONGRESET_TR69_KEY2 "VPI"
#define LONGRESET_TR69_KEY3 "VCI"
#else
#define LONGRESET_TR69_KEY2 "VLANID"
#define LONGRESET_TR69_KEY3 "DOT1P"
#endif
#endif
#if defined(TCSUPPORT_CT_DSL_EX)
int TR69_NodeIdxATM = -1, TR69_NodeIdxPTM = -1;

int get_all_wan_index_forDSLExt(int isATM, int indexbuf[], int buf_size)
{
	int pvc_index, entry_index, if_index, if_num = 0;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	mxml_node_t *curNode = NULL;
	mxml_node_t *top = get_node_top();
	if(!indexbuf)
		return 0;
	
	for(pvc_index = 0; pvc_index < PVC_NUM; pvc_index++){
		for(entry_index = 0; entry_index < MAX_SMUX_NUM; entry_index++){
			memset(nodeName, 0, sizeof(nodeName));
		  	strcpy(nodeName[0], (1 == isATM) ? "WanATM" : "WanPTM" );
		  	sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
			sprintf(nodeName[2], "%s%d", WAN_INTERFACE, entry_index);

			curNode = mxmlFindElement(top, top, nodeName[0],
				NULL, NULL, MXML_DESCEND);
			if(curNode){
				curNode = mxmlFindElement(curNode, curNode, nodeName[1],
				NULL, NULL, MXML_DESCEND);
				if(curNode){
					curNode = mxmlFindElement(curNode, curNode, nodeName[2],
					NULL, NULL, MXML_DESCEND);
					if(curNode){
						//tcdbg_printf("get_all_wan_index(): find Wan_%s_%s,continue!\n", nodeName[1], nodeName[2]);
						/*calculate interface index based on pvc index and entry index*/
						if_index = pvc_index * MAX_SMUX_NUM + entry_index;
						*indexbuf = if_index;
						indexbuf++;
						if_num++;
						buf_size--;
						if(buf_size  <= 0)
							break;
					}
				}
			}
		}
	}
	return if_num;
}
#endif

void tcapi_save_internal(mxml_node_t* top)
{
	tcapi_msg_t msg;
	memset(&msg,0,sizeof(msg)) ;

	tcapi_save_req(top, &msg);
}
int backupTR69WanIndex()
{
	mxml_node_t *top = NULL;
	int valid_if_num = 0;
	int idx = 0;
	int pvc_index = 0;
	int entry_index = 0;
	int if_index = 0;
	int valid_if[MAX_WAN_IF_INDEX] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char serviceList[64] = {0};

	top = get_node_top();

	// important: save current to mtd
	tcapi_save_internal(top);

	valid_if_num = get_all_wan_index(valid_if, MAX_WAN_IF_INDEX);
	for( idx = 0; idx < valid_if_num; idx ++)
	{
		if_index = valid_if[idx];
		if ( if_index > MAX_WAN_IF_INDEX )
			return;
		pvc_index = if_index / MAX_SMUX_NUM;
		entry_index = if_index % MAX_SMUX_NUM;

		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0], WAN);
		sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
		sprintf(nodeName[2], "%s%d", WAN_INTERFACE, entry_index);

		if( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, "ServiceList", serviceList)
			&& NULL != strstr(serviceList, "TR069"))
		{
			TR69_NodeIdx = if_index;
#if (defined(TCSUPPORT_CT_PON) || defined(TCSUPPORT_CT_E8B_ADSL)) || defined(TCSUPPORT_CUC)
			memset(nodeName, 0, sizeof(nodeName));
			strcpy(nodeName[0], WAN);
			sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
			memset(TR69_VLANMODE, 0, sizeof(TR69_VLANMODE));
			memset(TR69_VLANID, 0, sizeof(TR69_VLANID));
			memset(TR69_PBIT, 0, sizeof(TR69_PBIT));
#if defined(TCSUPPORT_CT_E8B_ADSL)
			if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, LONGRESET_TR69_KEY2, TR69_VLANID) )
				memset(TR69_VLANID, 0, sizeof(TR69_VLANID));
			if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, LONGRESET_TR69_KEY3, TR69_PBIT) )
				memset(TR69_PBIT, 0, sizeof(TR69_PBIT));
#else
			if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, LONGRESET_TR69_KEY1, TR69_VLANMODE) )
				memset(TR69_VLANMODE, 0, sizeof(TR69_VLANMODE));
			if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, LONGRESET_TR69_KEY2, TR69_VLANID) )
				memset(TR69_VLANID, 0, sizeof(TR69_VLANID));
			if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, LONGRESET_TR69_KEY3, TR69_PBIT) )
				memset(TR69_PBIT, 0, sizeof(TR69_PBIT));
#endif
#endif
			break;
		}
	}

#if defined(TCSUPPORT_CT_DSL_EX)
	valid_if_num = get_all_wan_index_forDSLExt(1, valid_if, MAX_WAN_IF_INDEX);
	for( idx = 0; idx < valid_if_num; idx ++)
	{
		if_index = valid_if[idx];
		if ( if_index > MAX_WAN_IF_INDEX )
			return;
		pvc_index = if_index / MAX_SMUX_NUM;
		entry_index = if_index % MAX_SMUX_NUM;

		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0], "WanATM");
		sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
		sprintf(nodeName[2], "%s%d", WAN_INTERFACE, entry_index);

		if( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, "ServiceList", serviceList)
			&& NULL != strstr(serviceList, "TR069"))
		{
			TR69_NodeIdxATM = if_index;
			break;
		}
	}

	valid_if_num = get_all_wan_index_forDSLExt(0, valid_if, MAX_WAN_IF_INDEX);
	for( idx = 0; idx < valid_if_num; idx ++)
	{
		if_index = valid_if[idx];
		if ( if_index > MAX_WAN_IF_INDEX )
			return;
		pvc_index = if_index / MAX_SMUX_NUM;
		entry_index = if_index % MAX_SMUX_NUM;

		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0], "WanPTM");
		sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
		sprintf(nodeName[2], "%s%d", WAN_INTERFACE, entry_index);

		if( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, "ServiceList", serviceList)
			&& NULL != strstr(serviceList, "TR069"))
		{
			TR69_NodeIdxPTM = if_index;
			break;
		}
	}
#endif

	return 0;
}

#if (defined(TCSUPPORT_CT_PON) || defined(TCSUPPORT_CT_E8B_ADSL)) || defined(TCSUPPORT_CUC)
int delTR69andChkBackupTR69Position(mxml_node_t* newtree)
{
	mxml_node_t *curNode = NULL;
	mxml_node_t *curNode_pvc = NULL;
	mxml_node_t *curNode_entry = NULL;
	int pvc_idx = 0, entry_idx = 0;
	char WanNodePVC[32] = {0};
	char WanNodeEntry[32] = {0};
	char *attr1=NULL, *attr2 = NULL, *attr3 = NULL;
	int found_PVC = 0;
	int isSinglePVC = 1;
	int restore_PVC = -1, restore_Entry = -1;
	//int empty_pvc = -1;

	if ( NULL == newtree )
		return -1;

	// we shouldn't delete Wan_Common node.
	if ( -1 != TR69_NodeIdx )
	{
		curNode = mxmlFindElement(newtree, newtree, WAN, NULL, NULL, MXML_DESCEND);
		if( curNode )
		{
			for ( pvc_idx = 0; pvc_idx < MAX_PVC_NUM; pvc_idx++ )
			{
				sprintf(WanNodePVC, "PVC%d", pvc_idx);
				curNode_pvc = mxmlFindElement(curNode, curNode, WanNodePVC, NULL, NULL, MXML_DESCEND);
				if( curNode_pvc )
				{
					found_PVC = 0;
					// check PVC entry
					attr1 = mxmlElementGetAttr(curNode_pvc, LONGRESET_TR69_KEY1);
					attr2 = mxmlElementGetAttr(curNode_pvc, LONGRESET_TR69_KEY2);
					attr3 = mxmlElementGetAttr(curNode_pvc, LONGRESET_TR69_KEY3);
					if (
#if defined(TCSUPPORT_CT_E8B_ADSL)
						attr2 && 0 == strcmp(attr2, TR69_VLANID)
						&& attr3 && 0 == strcmp(attr3, TR69_PBIT)
#else
						attr1 && 0 == strcmp(attr1, TR69_VLANMODE)
						&& attr2 && 0 == strcmp(attr2, TR69_VLANID)
						&& attr3 && 0 == strcmp(attr3, TR69_PBIT)
#endif
						&& -1 == restore_PVC )
					{
						restore_PVC = pvc_idx;
						found_PVC = 1;
					}

					isSinglePVC = 1;
					for ( entry_idx = 0; entry_idx < MAX_SMUX_NUM; entry_idx ++ )
					{
						sprintf(WanNodeEntry, "Entry%d", entry_idx);
						curNode_entry = mxmlFindElement(curNode_pvc, curNode_pvc, WanNodeEntry, NULL, NULL, MXML_DESCEND);
						if ( curNode_entry )
						{
							attr1 = mxmlElementGetAttr(curNode_entry, "ServiceList");
							if( attr1 && NULL != strstr(attr1, "TR069") )
							{
								mxmlDelete(curNode_entry); // remove current TR69.
								if ( 1 == found_PVC && -1 == restore_Entry )
									restore_Entry = entry_idx;
							}
							else
								isSinglePVC = 0;
						}
						else
						{
							if ( 1 == found_PVC && -1 == restore_Entry )
								restore_Entry = entry_idx;
						}
					}

					if ( isSinglePVC && -1 == restore_PVC && -1 == restore_Entry )
						mxmlDelete(curNode_pvc);// remove single PVC.
				}
				else
				{
					if ( -1 == currEmptyPVC_index )
					{
						// backup empty pvc index
						currEmptyPVC_index = pvc_idx;
					}
				}
			}
		}

		if ( -1 == restore_PVC || -1 == restore_Entry )
		{
			if ( -1 == currEmptyPVC_index )
			{
				tcdbg_printf("\nlong press reset fatal error: there is no wan space to restore TR69 wan[%s]-[%s]-[%s] \n",
					TR69_VLANMODE, TR69_VLANID, TR69_PBIT);
				return -1;
			}
			else
				restore_TR69NodeIdx = currEmptyPVC_index * MAX_SMUX_NUM;
		}
		else
			restore_TR69NodeIdx = restore_PVC * MAX_SMUX_NUM + restore_Entry;
	}

	return 0;
}

int resetTR69NodeAttribute(mxml_node_t* newtree)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	mxml_node_t *curNode = NULL;
	mxml_attr_t *attr = NULL;
	int i = 0;
	int pvc_index = 0;
	int entry_index = 0;
	char tr69newWanNodePVC[32] = {0};
	char tr69newWanNodePVCEntry[32] = {0};
	char tempval[32] = {0};

	if ( NULL == newtree )
		return -1;

	if ( -1 != TR69_NodeIdx
		&& -1 != restore_TR69NodeIdx )
	{
		pvc_index = restore_TR69NodeIdx / MAX_SMUX_NUM;
		entry_index = restore_TR69NodeIdx % MAX_SMUX_NUM;

		sprintf(tr69newWanNodePVCEntry, "Wan_PVC%d_Entry%d", pvc_index, entry_index);
	}
	else
		return -1;

	memset(nodeName, 0, sizeof(nodeName));
	splitName(tr69newWanNodePVCEntry, nodeName, "_");

	curNode = newtree;
	for(i = 0; i < MAX_ARG_NUM; i++)
	{
		if( strlen(nodeName[i])==0 )
			break;
		curNode = mxmlFindElement(curNode, curNode, nodeName[i],
			NULL, NULL, MXML_DESCEND);
		if( !curNode )
			return 0;
	}

	attr = mxmlElementGetAttr(curNode, "NASName");
	if ( attr )
	{
		sprintf(tempval, "nas%d_%d", pvc_index, entry_index);
		mxmlElementSetAttr(curNode, "NASName", tempval);
	}

	attr = mxmlElementGetAttr(curNode, "IFName");
	if ( attr )
	{
		if ( NULL != strstr(attr, "ppp") )
			sprintf(tempval, "ppp%d", restore_TR69NodeIdx);
		else
			sprintf(tempval, "nas%d_%d", pvc_index, entry_index);
		mxmlElementSetAttr(curNode, "IFName", tempval);
	}

	attr = mxmlElementGetAttr(curNode, "PPPUNIT");
	if ( attr )
	{
		sprintf(tempval, "%d", restore_TR69NodeIdx);
		mxmlElementSetAttr(curNode, "PPPUNIT", tempval);
	}

	return 0;
}

#endif


int restoreTR69WanIndex(mxml_node_t* oldtree, mxml_node_t* newtree)
{
#if (defined(TCSUPPORT_CT_PON) || defined(TCSUPPORT_CT_E8B_ADSL)) || defined(TCSUPPORT_CUC)
	Key_Parameters_Info_Enhance_Ptr pkeyPara = NULL;
	Key_Parameters_Info_Enhance keyPara_restore[]=
	{
		{OP_NODE|OP_NODE_LEVEL_SINGLE, "Wan_PVCx", "Wan_PVCx", NULL, operateNodeParaEnhance},
		{OP_NODE|OP_NODE_LEVEL_SINGLE, "Wan_PVCx_Entryx", "Wan_PVCx_Entryx", NULL, operateNodeParaEnhance},
		{OP_AT_END,NULL,NULL,NULL}
	};
#else
	Key_Parameters_Info_Ptr pkeyPara = NULL;
	Key_Parameters_Info keyPara_restore[]=
	{
		{OP_NODE|OP_NODE_LEVEL_SINGLE, "Wan_PVCx", NULL, operateNodePara},
		{OP_NODE|OP_NODE_LEVEL_SINGLE, "Wan_PVCx_Entryx", NULL, operateNodePara},
#if defined(TCSUPPORT_CT_DSL_EX)
		{OP_NODE|OP_NODE_LEVEL_SINGLE, "WanATM_PVCx", NULL, operateNodePara},
		{OP_NODE|OP_NODE_LEVEL_SINGLE, "WanATM_PVCx_Entryx", NULL, operateNodePara},
		{OP_NODE|OP_NODE_LEVEL_SINGLE, "WanPTM_PVCx", NULL, operateNodePara},
		{OP_NODE|OP_NODE_LEVEL_SINGLE, "WanPTM_PVCx_Entryx", NULL, operateNodePara},
#endif
		{OP_AT_END,NULL,NULL,NULL}
	};
#endif
	char tr69WanNodePVC[32] = {0};
	char tr69WanNodePVCEntry[32] = {0};
#if defined(TCSUPPORT_CT_DSL_EX)
	char tr69WanNodeATMPVC[32] = {0};
	char tr69WanNodeATMPVCEntry[32] = {0};
	char tr69WanNodePTMPVC[32] = {0};
	char tr69WanNodePTMPVCEntry[32] = {0};
#endif
#if (defined(TCSUPPORT_CT_PON) || defined(TCSUPPORT_CT_E8B_ADSL)) || defined(TCSUPPORT_CUC)
	char tr69newWanNodePVC[32] = {0};
	char tr69newWanNodePVCEntry[32] = {0};
#endif
	int pvc_index = 0;
	int entry_index = 0;

	if ( NULL == oldtree || NULL == newtree
		|| (-1 == TR69_NodeIdx
#if defined(TCSUPPORT_CT_DSL_EX)
			&& -1 == TR69_NodeIdxATM && -1 == TR69_NodeIdxATM
#endif
		))
		return -1;

#if (defined(TCSUPPORT_CT_PON) || defined(TCSUPPORT_CT_E8B_ADSL)) || defined(TCSUPPORT_CUC)
	if ( -1 != TR69_NodeIdx
		&& -1 != restore_TR69NodeIdx )
	{
		pvc_index = TR69_NodeIdx / MAX_SMUX_NUM;
		entry_index = TR69_NodeIdx % MAX_SMUX_NUM;

		sprintf(tr69WanNodePVC, "Wan_PVC%d", pvc_index);
		keyPara_restore[0].node = tr69WanNodePVC;
		sprintf(tr69WanNodePVCEntry, "Wan_PVC%d_Entry%d", pvc_index, entry_index);
		keyPara_restore[1].node = tr69WanNodePVCEntry;

		pvc_index = restore_TR69NodeIdx / MAX_SMUX_NUM;
		entry_index = restore_TR69NodeIdx % MAX_SMUX_NUM;

		sprintf(tr69newWanNodePVC, "Wan_PVC%d", pvc_index);
		keyPara_restore[0].nodenew = tr69newWanNodePVC;
		sprintf(tr69newWanNodePVCEntry, "Wan_PVC%d_Entry%d", pvc_index, entry_index);
		keyPara_restore[1].nodenew = tr69newWanNodePVCEntry;
	}
	else
	{
		keyPara_restore[0].funp = NULL;
		keyPara_restore[1].funp = NULL;
	}

#else

	if ( -1 != TR69_NodeIdx )
	{
		pvc_index = TR69_NodeIdx / MAX_SMUX_NUM;
		entry_index = TR69_NodeIdx % MAX_SMUX_NUM;

		sprintf(tr69WanNodePVC, "Wan_PVC%d", pvc_index);
		keyPara_restore[0].node = tr69WanNodePVC;
		sprintf(tr69WanNodePVCEntry, "Wan_PVC%d_Entry%d", pvc_index, entry_index);
		keyPara_restore[1].node = tr69WanNodePVCEntry;
	}
	else
	{
		keyPara_restore[0].funp = NULL;
		keyPara_restore[1].funp = NULL;
	}
#if defined(TCSUPPORT_CT_DSL_EX)
	if ( -1 != TR69_NodeIdxATM )
	{
		pvc_index = TR69_NodeIdxATM / MAX_SMUX_NUM;
		entry_index = TR69_NodeIdxATM % MAX_SMUX_NUM;

		sprintf(tr69WanNodeATMPVC, "WanATM_PVC%d", pvc_index);
		keyPara_restore[2].node = tr69WanNodeATMPVC;
		sprintf(tr69WanNodeATMPVCEntry, "WanATM_PVC%d_Entry%d", pvc_index, entry_index);
		keyPara_restore[3].node = tr69WanNodeATMPVCEntry;
	}
	else
	{
		keyPara_restore[2].funp = NULL;
		keyPara_restore[3].funp = NULL;
	}

	if ( -1 != TR69_NodeIdxPTM )
	{
		pvc_index = TR69_NodeIdxPTM / MAX_SMUX_NUM;
		entry_index = TR69_NodeIdxPTM % MAX_SMUX_NUM;

		sprintf(tr69WanNodePTMPVC, "WanPTM_PVC%d", pvc_index);
		keyPara_restore[4].node = tr69WanNodePTMPVC;
		sprintf(tr69WanNodePTMPVCEntry, "WanPTM_PVC%d_Entry%d", pvc_index, entry_index);
		keyPara_restore[5].node = tr69WanNodePTMPVCEntry;
	}
	else
	{
		keyPara_restore[4].funp = NULL;
		keyPara_restore[5].funp = NULL;
	}
#endif
#endif

	for ( pkeyPara = keyPara_restore ; (pkeyPara->flag & OP_AT_END) == 0; pkeyPara++ )
	{
		if(pkeyPara->funp != NULL)
		{
			if ( pkeyPara->funp(pkeyPara, oldtree, newtree) < 0 )
				return -1;
		}
	}
#if (defined(TCSUPPORT_CT_PON) || defined(TCSUPPORT_CT_E8B_ADSL)) || defined(TCSUPPORT_CUC)
	// must reset the specal attribute
	resetTR69NodeAttribute(newtree);
#endif

	return 0;
}
int del_wan_node(mxml_node_t* newtree)
{
	mxml_node_t *curNode = NULL;
	mxml_node_t *curNode_pvc = NULL;
	int pvc_idx = 0;
	char WanNodePVC[32] = {0};

	if ( NULL == newtree )
		return -1;

	// we shouldn't delete Wan_Common node.
	if ( -1 != TR69_NodeIdx )
	{
		curNode = mxmlFindElement(newtree, newtree, WAN, NULL, NULL, MXML_DESCEND);
		if( curNode )
		{
			for ( pvc_idx = 0; pvc_idx < MAX_PVC_NUM; pvc_idx++ )
			{
				sprintf(WanNodePVC, "PVC%d", pvc_idx);
				curNode_pvc = mxmlFindElement(curNode, curNode, WanNodePVC, NULL, NULL, MXML_DESCEND);
				if( curNode_pvc )
					mxmlDelete(curNode_pvc);
			}
		}
	}

#if defined(TCSUPPORT_CT_DSL_EX)
	if ( -1 != TR69_NodeIdxATM )
	{
		curNode = mxmlFindElement(newtree, newtree, "WanATM", NULL, NULL, MXML_DESCEND);
		if( curNode )
		{
			for ( pvc_idx = 0; pvc_idx < MAX_PVC_NUM; pvc_idx++ )
			{
				sprintf(WanNodePVC, "PVC%d", pvc_idx);
				curNode_pvc = mxmlFindElement(curNode, curNode, WanNodePVC, NULL, NULL, MXML_DESCEND);
				if( curNode_pvc )
					mxmlDelete(curNode_pvc);
			}
		}
	}

	if ( -1 != TR69_NodeIdxPTM )
	{
		curNode = mxmlFindElement(newtree, newtree, "WanPTM", NULL, NULL, MXML_DESCEND);
		if( curNode )
		{
			for ( pvc_idx = 0; pvc_idx < MAX_PVC_NUM; pvc_idx++ )
			{
				sprintf(WanNodePVC, "PVC%d", pvc_idx);
				curNode_pvc = mxmlFindElement(curNode, curNode, WanNodePVC, NULL, NULL, MXML_DESCEND);
				if( curNode_pvc )
					mxmlDelete(curNode_pvc);
			}
		}
	}
#endif

	return 0;
}
void setCwmpLongResetEvent(mxml_node_t* newtree)
{
	mxml_node_t *curNode = NULL;

	if ( NULL == newtree )
		return -1;

	curNode = mxmlFindElement(newtree, newtree, "Cwmp", NULL, NULL, MXML_DESCEND);
	if ( curNode )
	{
		curNode = mxmlFindElement(curNode, curNode, "Entry", NULL, NULL, MXML_DESCEND);
		if ( curNode )
			mxmlElementSetAttr(curNode, "longresetbtn", "1");
	}
}
#endif

#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
int SimCardResetFail = 0;
extern void setNeedReadSimCard(int value);

void setSimCardResetFailAlarmEvent(mxml_node_t* newtree)
{
	mxml_node_t *curNode = NULL;

	if ( NULL == newtree )
		return -1;

	curNode = mxmlFindElement(newtree, newtree, "Cwmp", NULL, NULL, MXML_DESCEND);
	if ( curNode )
	{
		curNode = mxmlFindElement(curNode, curNode, "Entry", NULL, NULL, MXML_DESCEND);
		if ( curNode ){
			mxmlElementSetAttr(curNode, "simcardrstfail", "1");
		}
	}
}
void simCardReset(int resetType)
{
	int ret = 1;
	if(resetType == BOOT2DEFSET){
		tcdbg_printf("\r\nshort reset sim card rom");
		//ret = simcard_romrst(2);
		ret = system("/userfs/bin/simCard romreset 2");

	}
	else if(resetType == LONGRESET_BOOT2DEFSET){
		tcdbg_printf("\r\nlong reset sim card rom");
		//ret = simcard_romrst(0);
		ret = system("/userfs/bin/simCard romreset 0");
	}
	else if(resetType == BOOT2DEFSETTR69){
		tcdbg_printf("\r\nTR69 reset sim card rom");
		//ret = simcard_romrst(0);
		ret = system("/userfs/bin/simCard romreset 0");
	}
	if(ret != 0){
		tcdbg_printf("\r\nreset sim card fail, will signal infrom");
		SimCardResetFail = 1;
	}
}

#endif

int keyParaRestore(int mode)
{
	mxml_node_t *defaultTree  = NULL;
	mxml_node_t *runningTree  = NULL;
	mxml_node_t *defaultCurNode = NULL;
	mxml_node_t *runningCurNode = NULL;
	Key_Parameters_Info_Ptr pkeyPara = NULL;
	char cmd[80] = {0};
	FILE *fp = NULL;
	int retvalue = -1;
	char romfilepath[64]={0};
#if defined(TCSUPPORT_CT_PHONEAPP)
	Key_Parameters_Info WLanSch[] =
	{
		{OP_ATTRIBUTE|OP_CWMP_ONLY,"mobile_Entry","WLanStartTime",operateAttributePara},
		{OP_ATTRIBUTE|OP_CWMP_ONLY,"mobile_Entry","WLanEndTime",operateAttributePara},
		{OP_ATTRIBUTE|OP_CWMP_ONLY,"mobile_Entry","WLanScheduleEable",operateAttributePara},
		{OP_AT_END,NULL,NULL,NULL}
	};
#endif

	//weather we need to free the running romfile tree in the memory for reduing memory??
	//because after calling the function 'keyParaRestore()',cpe will reboot 
	//free_romfile_tree();
	
	//create a new tree from "/userfs/romfile.cfg".
	if(g_fsDefaultRomfileTree != NULL) {
		defaultTree = g_fsDefaultRomfileTree;//The default romfile which has been restored by defaultParaRestore
	}
	else {
		getDefaultRomfile(romfilepath);
		defaultTree = keypara_load_romfile(romfilepath);
		if(defaultTree == NULL)
		{
			tcdbg_printf("\r\nkeyParaRestore:load default romfile error");
			goto errorHandle;
		}
#if defined(TCSUPPORT_RESERVEAREA_EXTEND) || defined(TCSUPPORT_C7) || (defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505))   // we use parameters from proline para block
		else
		{
			handle_proline_paras(defaultTree);
		}
#endif
	}
	
	//create a new tree from "/mtd/mtdblock1".
	runningTree = keypara_load_romfile(RUNNING_ROMFILE_PATH);
	if(runningTree == NULL)
	{
		tcdbg_printf("\r\nkeyParaRestore:load running romfile error");
		goto errorHandle;
	}

	defaultCurNode = defaultTree;
	runningCurNode = runningTree;
	
	//store key parameters

	for (pkeyPara = keyPara ; (pkeyPara->flag & OP_AT_END) == 0; pkeyPara++ )
	{
		if ( BOOT2DEFSETTR69 == mode )
		{
			if ( !(pkeyPara->flag & OP_CWMP_ONLY) )
				continue;
		}
#if defined(TCSUPPORT_CT_LONG_RESETBTN)
		if ( LONGRESET_BOOT2DEFSET == mode )
		{
			if ( !(pkeyPara->flag & OP_LONG_RESETBAK) )
				continue;
		}
#endif

		if(pkeyPara->funp != NULL)
		{
			retvalue = pkeyPara->funp(pkeyPara,runningCurNode,defaultCurNode);
			if(retvalue < 0)
			{
				if(pkeyPara->attr != NULL)
				{
					//tcdbg_printf("\r\nkeyParaRestore:exectue function error!node name is %s,attribute is %s",pkeyPara->node,pkeyPara->attr);
				}
				else
				{
					//tcdbg_printf("\r\nkeyParaRestore:exectue function error!node name is %s",pkeyPara->node);
				}
				goto errorHandle;
			}
		}
	}
#if defined(TCSUPPORT_CT_PHONEAPP)
	if ( BOOT2DEFSETTR69 == mode )
	{
		for (pkeyPara = WLanSch ; (pkeyPara->flag & OP_AT_END) == 0; pkeyPara++ )
		{
			if(pkeyPara->funp != NULL)
			{
				retvalue = pkeyPara->funp(pkeyPara,runningCurNode,defaultCurNode);
				if(retvalue < 0)
				{
					if(pkeyPara->attr != NULL)
					{
						//tcdbg_printf("\r\nkeyParaRestore:exectue function error!node name is %s,attribute is %s",pkeyPara->node,pkeyPara->attr);
					}
					else
					{
						//tcdbg_printf("\r\nkeyParaRestore:exectue function error!node name is %s",pkeyPara->node);
					}
					goto errorHandle;
				}
			}
		}
	}
#endif

#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
		// add sim card reset fail alarm cwmp inform event.
		if(SimCardResetFail == 1){
			setSimCardResetFailAlarmEvent(defaultCurNode);
		}
#endif

#if defined(TCSUPPORT_CT_LONG_RESETBTN)
	if ( LONGRESET_BOOT2DEFSET == mode ) // restore tr69 wan.
	{
		// add long reset button cwmp inform event.
		setCwmpLongResetEvent(defaultCurNode);

		if ( -1 != TR69_NodeIdx
#if defined(TCSUPPORT_CT_DSL_EX)
			|| ( -1 != TR69_NodeIdxATM || -1 != TR69_NodeIdxATM )
#endif
			)
		{
#if (defined(TCSUPPORT_CT_PON) || defined(TCSUPPORT_CT_E8B_ADSL)) && !defined(TCSUPPORT_CUC)
			// 1. delete tr69 wan node and check backup tr69 wan position  from new tree.
			delTR69andChkBackupTR69Position(defaultCurNode);
#else
			// 1. delete wan node from new tree.
			if ( 0 != del_wan_node(defaultCurNode) )
				goto errorHandle;
#endif
			// 2. restore tr69 wan.
			if ( 0 != restoreTR69WanIndex(runningCurNode, defaultCurNode) )
				goto errorHandle;
		}
	}
#endif

#if defined(TCSUPPORT_CUC)
	if ( BOOT2DEFSETTR69 == mode && ( -1 != TR69_NodeIdx))
	{
		// 1. delete tr69 wan node and check backup tr69 wan position  from new tree.
		delTR69andChkBackupTR69Position(defaultCurNode);
			
		if ( 0 != restoreTR69WanIndex(runningCurNode, defaultCurNode) )
				goto errorHandle;
	}
#endif

	//Create a romfile.cfg and store into "/mtd/mtdblock1"
	fp = fopen(ROMFILE_PATH, "w");
	if(fp == NULL)
	{
		tcdbg_printf("\r\nkeyParaRestore:exectue function error!node name is %s",pkeyPara->node);
		goto errorHandle;
	}
	else
	{
		retvalue = mxmlSaveFile(defaultTree, fp,  whitespace_cb);
		fclose(fp);
	}

#if defined(TCSUPPORT_CT_BACKUPROMFILEENCHANCEMENT)
	retvalue = compute_checksum(ROMFILE_PATH,STORE_ROMFILE_FILE_NAME);
	if(retvalue < 0)
	{
		tcdbg_printf("\r\nkeyParaRestore:Fail to compute checksum when save to flash!\n");
		goto errorHandle;
	}
#endif
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)	
	//set default Tree as running Tree
	if(BOOT2DEFSET == mode && SimCardResetFail == 0){
		if(*gTreePtr!= NULL){
			mxmlDelete(*gTreePtr);
		}
		*gTreePtr = defaultTree;
	}
	else
#endif
	{//free the default tree if need
		if((g_fsDefaultRomfileTree == NULL) && defaultTree)
			mxmlDelete(defaultTree);
	}
	//free the runningTree tree
	if(runningTree)
		mxmlDelete(runningTree);
	
#if defined(TCSUPPORT_CT_BACKUPROMFILEENCHANCEMENT)
	sprintf(cmd,UPDATE_ROMFILE_CMD, STORE_ROMFILE_FILE_NAME);
	system(cmd);
	usleep(100000);

	sprintf(cmd, TC_FLASH_ERASE_SECTOR_CMD,(unsigned long)BACKUPROMFILE_RA_OFFSET, RESERVEAREA_NAME);
	system(cmd);

	memset(cmd,0,sizeof(cmd));
	sprintf(cmd, TC_FLASH_WRITE_CMD,STORE_ROMFILE_FILE_NAME,(unsigned long)BACKUPROMFILE_RA_SIZE,(unsigned long)BACKUPROMFILE_RA_OFFSET, RESERVEAREA_NAME);
	system(cmd);
	//system("reboot");
#else
	#ifdef TCSUPPORT_BACKUPROMFILE
	sprintf(cmd,UPDATE_ROMFILE_CMD, ROMFILE_PATH);
	system(cmd);
	usleep(100000);
	sprintf(cmd, TC_FLASH_ERASE_SECTOR_CMD,(unsigned long)BACKUPROMFILE_RA_OFFSET, RESERVEAREA_NAME);
	system(cmd);

	memset(cmd,0,sizeof(cmd));
	sprintf(cmd, TC_FLASH_WRITE_CMD,ROMFILE_PATH,(unsigned long)BACKUPROMFILE_RA_SIZE,(unsigned long)BACKUPROMFILE_RA_OFFSET, RESERVEAREA_NAME);
	system(cmd);
	//system("reboot");
	#else
	sprintf(cmd,UPDATE_ROMFILE_CMD, ROMFILE_PATH);	
	system(cmd);
	#endif
#endif

#if defined(TCSUPPORT_CT_BACKUPROMFILEENCHANCEMENT)
	unlink(STORE_ROMFILE_FILE_NAME);
#endif

	keyParaRestoreFlag = 1;//Have done running or backup romfile update
	
	return 0;
	
errorHandle:

	if((g_fsDefaultRomfileTree == NULL) && defaultTree) {
		mxmlDelete(defaultTree);
	}
	if(runningTree)
		mxmlDelete(runningTree);
	return -1;
}
#endif

int getDefaultRomfile(char *path){
	char nodeName[3][MAX_NODE_NAME] = {0};
	char romfileSelect[64]={0};
	char def_romfile[64]={0};
	mxml_node_t *top = NULL;
	int  fd=0;
	
#if defined(TCSUPPORT_CT_PON)
#if defined(TCSUPPORT_PRODUCTIONLINE) && defined(TCSUPPORT_RESERVEAREA_EXTEND)
	proline_Para para;

	memset(&para, 0, sizeof(para));
	tcgetProLinePara(&para,0);
	if((PRODUCTLINEPARAMAGIC == para.magic) && (para.flag & PL_CP_ROMFILE_SELECT_FLAG))
	{
		strcpy(romfileSelect, para.romfileselect);
		sprintf(def_romfile,"/userfs/%s", romfileSelect);
		fd = open(def_romfile,O_RDONLY);
		if(fd != FAIL){
			close(fd);
			strcpy(path,def_romfile);
			return 0;
		}
	}

	if((PRODUCTLINEPARAMAGIC == para.magic) && (para.flag & PL_CP_XPONMODE_FLAG))
	{
		if( 0 == strcmp("EPON", para.xponmode))
		{
			fd = open(DEF_ROMFILE_PATH_EPON,O_RDONLY);
			if(fd != FAIL){
				close(fd);
				strcpy(path,DEF_ROMFILE_PATH_EPON);
				return 1;
			}
		}
	}
#endif	
#endif
	sprintf(def_romfile,"%s", DEF_ROMFILE_PATH);
	strcpy(path,def_romfile);
	return 2;
}

#if defined(TCSUPPORT_CT_DEFAULTPARA_STORE)
Key_Parameters_Info defaultPara[]=
{
	//follow is node parameters
	//follow is attribute parameters	
	{OP_ATTRIBUTE,"WLan_Entry0","SSID",operateAttributePara},//wlan ssid
	{OP_ATTRIBUTE,"WLan_Entry0","WPAPSK",operateAttributePara},//wlan key
	{OP_ATTRIBUTE,"WLan_Entry0","DefaultCfg",operateAttributePara},//wlan auto ssid flag
	{OP_ATTRIBUTE,"Account_Entry1","web_passwd",operateAttributePara},//useradmin web password
#if defined(TCSUPPORT_CY)
	{OP_ATTRIBUTE,"Account_TelnetEntry","Active",operateAttributePara},
#endif
	{OP_AT_END,NULL,NULL,NULL}
};

int defaultParaRestore()
{
	char buf[128] = {0};
#if !defined(TCSUPPORT_RESERVEAREA_EXTEND) && !defined(TCSUPPORT_C7) && !(defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505))  // we use parameters from proline para block
	mxml_node_t *flashDefaultTree  = NULL;
#endif
	mxml_node_t *defaultCurNode = NULL;
	mxml_node_t *curNode = NULL;
	Key_Parameters_Info_Ptr pkeyPara = NULL;
	int retvalue = -1;
	char romfilepath[64]={0};

	/*Restore default parameter from flash default romfile*/
#if !defined(TCSUPPORT_RESERVEAREA_EXTEND) && !defined(TCSUPPORT_C7) && !(defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505))  // we use parameters from proline para block
	/*Read  flash default romfile from reservearea block2 */
	sprintf(buf, TC_FLASH_READ_CMD,DR_FILE_NAME,(unsigned long)DEFAULTROMFILE_RA_SIZE,(unsigned long)DEFAULTROMFILE_RA_OFFSET,RESERVEAREA_NAME);
	system(buf);
	usleep(100000);
	
	//create a new tree from flash default romfile "reservearea block2".
	flashDefaultTree = keypara_load_romfile(DR_FILE_NAME);
	if(flashDefaultTree == NULL) {
		tcdbg_printf("\r\ndefaultParaRestore:load flash default romfile %s error", DR_FILE_NAME);
		goto errorHandle;
	}
#endif

	//create a new tree from filesystem default romfile, such as "/userfs/ctromfile.cfg".
	getDefaultRomfile(romfilepath);
	g_fsDefaultRomfileTree = keypara_load_romfile(romfilepath);
	if(g_fsDefaultRomfileTree == NULL) {
		tcdbg_printf("\r\ndefaultParaRestore:load filesystem default romfile %s error", DEF_ROMFILE_PATH);
		goto errorHandle;
	}

	defaultCurNode = g_fsDefaultRomfileTree;//defaultCurNode is filesystem default romfile tree

#if defined(TCSUPPORT_RESERVEAREA_EXTEND) || defined(TCSUPPORT_C7) || (defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505))  // we use parameters from proline para block
	handle_proline_paras(defaultCurNode);
#else
	curNode = flashDefaultTree;	//curNode is flash default romfile tree
		
	for (pkeyPara = defaultPara ; (pkeyPara->flag & OP_AT_END) == 0; pkeyPara++ )
	{
		if(pkeyPara->funp != NULL)
		{
			/*Default Para Restore: restore default parameters from flash default romfile to filesystem default romfile*/
			retvalue = pkeyPara->funp(pkeyPara,curNode,defaultCurNode);			
			if(retvalue < 0)
			{
				if(pkeyPara->attr != NULL)
				{
					//tcdbg_printf("\r\ndefaultParaRestore:exectue function error!node name is %s,attribute is %s",pkeyPara->node,pkeyPara->attr);
				}
				else
				{
					//tcdbg_printf("\r\ndefaultParaRestore:exectue function error!node name is %s",pkeyPara->node);
				}
				goto errorHandle;
			}
		}
	}

	if(flashDefaultTree)
		mxmlDelete(flashDefaultTree);
#endif

	return 0;

errorHandle:

	if(g_fsDefaultRomfileTree) {
		mxmlDelete(g_fsDefaultRomfileTree);
		g_fsDefaultRomfileTree = NULL;
	}
#if !defined(TCSUPPORT_RESERVEAREA_EXTEND) && !defined(TCSUPPORT_C7) && !(defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505))  // we use parameters from proline para block
	if(flashDefaultTree)
		mxmlDelete(flashDefaultTree);
#endif
	return -1;
}

int defaultParaRomfileUpdate()
{
	char cmd[80] = {0};
	FILE *fp = NULL;
	int retvalue = -1;

	if(g_fsDefaultRomfileTree == NULL)
		goto errorHandle;

	//Create a romfile.cfg and store into "/mtd/mtdblock1"
	fp = fopen(ROMFILE_PATH, "w");
	if(fp == NULL)
	{
		tcdbg_printf("\r\ndefaultParaRestore:exectue function error!");
		goto errorHandle;
	}
	else
	{
		//save the restored default romfile tree
		retvalue = mxmlSaveFile(g_fsDefaultRomfileTree, fp,  whitespace_cb);
		fclose(fp);
	}

#if defined(TCSUPPORT_CT_BACKUPROMFILEENCHANCEMENT)
	retvalue = compute_checksum(ROMFILE_PATH,STORE_ROMFILE_FILE_NAME);
	if(retvalue < 0)
	{
		tcdbg_printf("\r\ndefaultParaRestore:Fail to compute checksum when save to flash!\n");
		goto errorHandle;
	}
#endif
	
	//free the tree
	if(g_fsDefaultRomfileTree) {
		mxmlDelete(g_fsDefaultRomfileTree);
		g_fsDefaultRomfileTree = NULL;
	}
	
#if defined(TCSUPPORT_CT_BACKUPROMFILEENCHANCEMENT)
	sys_update_backup_romfile(STORE_ROMFILE_FILE_NAME);
#else
	#ifdef TCSUPPORT_BACKUPROMFILE
	sys_update_backup_romfile(ROMFILE_PATH);
	#else
	sprintf(cmd,UPDATE_ROMFILE_CMD, ROMFILE_PATH);	
	system(cmd);
	#endif
#endif

#if defined(TCSUPPORT_CT_BACKUPROMFILEENCHANCEMENT)
	unlink(STORE_ROMFILE_FILE_NAME);
#endif

	return 0;
	
errorHandle:

	if(g_fsDefaultRomfileTree) {
		mxmlDelete(g_fsDefaultRomfileTree);
		g_fsDefaultRomfileTree = NULL;
	}

	return -1;
}

#endif


#if 0
#define TRX_MAGIC       0x30524448      /* "HDR0" */
#define TRX_MAGIC1       0x31524448      /* "HDR1" */
struct trx_header {
	uint32_t magic;		/* "HDR0" */
	uint32_t len;		/* Length of file including header */
	uint32_t crc32;		/* 32-bit CRC from flag_version to end of file */
	uint32_t flag_version;	/* 0:15 flags, 16:31 version */
	uint32_t offsets[3];    /* Offsets of partitions from start of header */
};

int
image_check_tc(int imagefd, const char *mtd);

#endif

int
mtd_open(const char *mtd, int flags);

int
upgarde_fw(int type);

int
sys_upgrade_firmware(mxml_node_t *top, char *value);

unsigned long int
get_key_vlaue(char *str, char *keyword, int base);

/*xyzhu add 091116*/
unsigned long int
get_key_ifvlaue(char *str, char *keyword, int base);

int
get_key_macTabvlaue(char *str, char *keyword, int base);

/*krammer add 2008 10 05*/

int
checkPpp(int cur_pvc);
/*krammer add 2008 10 05*/

void
check_wireless_card(char *retval);

void
test_atm_oam_ping(mxml_node_t *tree,char *name, int type, char *buf);

char*
getDefGW(char* gateway);

char*
getWanInfo(int type, char* device, char* buf);

mxml_node_t *
get_web_node(mxml_node_t *top, char name[][MAX_NODE_NAME]);

void
del_sub_node(mxml_node_t *top, char name[][MAX_NODE_NAME]);

int
get_parameter_value(char *buf, char *paremeter, char *ret_val, int buf_size);

int
html_post_parser(char *file_path, unsigned long *data_start, unsigned long *data_stop, unsigned char *firmware_type);

unsigned long
choose_type(char *file_path, unsigned char *firmware_type);


int
image_check_tc(int imagefd, char firmware_type, char *file_path,char *mtd, unsigned  int length
#if defined(TCSUPPORT_CT_UPLOAD_ALLINONE)
				, unsigned int *tclinuxbin_len
#endif
				);

char system_attr[][16]=
{
	{"reboot_type"},
	{"skbmgr_hot_len"},
	//{"bkrom"},
	{""}
};
char webCurSet_attr[][16]=
	{
		{"wan_pvc"},
#ifndef PURE_BRIDGE
		{"nat_pvc"},
#endif
		{"diag_pvc"},
		{"dev_pvc"},
#ifndef PURE_BRIDGE
		{"virServ_id"},
		//{"ipAddr_id"},
		{"route_id"},
#ifdef IPV6
		{"route6_id"},
#endif
#endif
		{"statis_type"},
		{"lan_id"},
#ifdef ALIAS_IP
		{"lanAlias_id"},
#endif
#ifndef PURE_BRIDGE
		{"wlan_id"},
#if defined(TCSUPPORT_WLAN_AC)
		{"wlan_ac_id"},
#endif
		#if QOS_REMARKING  /*Rodney_20090724*/
		{"qos_id"},
		#endif
#if 0// def TCSUPPORT_PORTBIND
		{"portbind_id"},
#endif
		{"acl_id"},/*20080825 racing add*/
		{"ipfilter_id"},/*20080926 racing add*/
		{"url_filter_id"},/*20081021 krammer add*/
#endif
		#if VOIP//shnwind add. 20091023
		{"SIPIndexBasic"},
		{"SIPIndexCall"},
		{"SIPIndexMedia"},
		{"SIPIndexSpeed"},
		#endif
		{"CurrentAccess"},//Ian_20100121
		{"autopvc_id"},
#if defined(TCSUPPORT_CT_NETWORKMANAGESERVICE)
		{"account_id"},
#endif
#if defined(TCSUPPORT_CT_E8GUI)
		{"ddns_id"},
#endif
#if defined(TCSUPPORT_RIPD)
		{"ripd_id"},
#endif
#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
		{"ippdiag_id"},
#endif
#if defined(TCSUPPORT_CT_ALARMMONITOR)
		{"alarm_id"},
		{"monitor_id"},
#endif
#if defined(TCSUPPORT_CT_USB_BACKUPRESTORE)
		{"rstresult"},
		{"opt"},
#endif
#ifdef CWMP
		{"switchpara_id"},
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
		{"user_id"},
		{"macfilter_user"},	
		{"ipfilter_user"},
		{"whiteurl_user"},
		{"blackurl_user"},		
#endif
#endif
		{""}
	};

char dynDisp_attr[][16] = 
	{
		{"CurPage"},
		{"MainMaskBit0"},
		{"MainMaskBit1"},
		{"MainMaskBit2"},
		{"MainMaskBit3"},
		{"MainMaskBit4"},
		{"MainMaskBit5"},
		{"MainMaskBit6"},
		{"MainMaskBit7"},
		{"CurMaskBit0"},
		{"CurMaskBit1"},
		{"CurMaskBit2"},
		{"CurMaskBit3"},
		{"CurMaskBit4"},
		{"CurMaskBit5"},
		{"CurMaskBit6"},
		{"CurMaskBit7"},
#if defined(TCSUPPORT_CT_PON_CY_JX)
		{"CurMaskBit8"},
		{"CurMaskBit9"},
#else
#if defined(TCSUPPORT_CT_PHONEAPP)
		{"CurMaskBit8"},
#endif
#endif
		{""}
	};

enum wanIFType_en{
	Ip_type=0,
	Netmask_type,
	Pridns_type,
	Secdns_type, //xyzhu_20091225 support Secondary dns server.
	Mac_type,
	PTP_type,	//get PPP Peer address for gateway
};


int
sys_boot(mxml_node_t *top, char *value);

int
sys_skbmgr(mxml_node_t *top, char *value);


#ifdef TCSUPPORT_CT_PON
int
sys_avaTest(mxml_node_t *top, char *value);
#endif

//int
//sys_bkrom(mxml_node_t *top, char *value);
int
sys_start_up(mxml_node_t *top,char *value);

typedef struct
web_node_s{
	char attr[ATTR_SIZE];
	char value[MAX_VALUE_SIZE];
	int (*action)(mxml_node_t* top, char* value);
}web_node_t;

web_node_t
system_node[]=
{
	{"reboot_type","0",sys_boot},
	{"skbmgr_hot_len","512",sys_skbmgr},
	//{"bkrom","0",sys_bkrom},
	{"upgrade_fw","0",sys_upgrade_firmware},
	{"start_up","0",sys_start_up},
#ifdef TCSUPPORT_CT_PON
	{"avalanchTest", "0", sys_avaTest},
#endif
	{"","",NULL}
};

//"System",	WEB_TYPE|SINGLE_TYPE,	NULL, NULL,system_read, NULL, NULL, system_execute },
/*______________________________________________________________________________
**	system_init
**
**	descriptions:
**		the init function of system.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
system_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="System",
			.type=WEB_TYPE|SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=system_read,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=system_execute,
			.cfg_boot=system_init_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}
/*______________________________________________________________________________
**	system_init_boot
**
**	descriptions:
**		the boot function of system.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
system_init_boot(mxml_node_t *top){

	tcapi_msg_t msg;
#ifndef PURE_BRIDGE
	/*backup up now romfile at boot*/
	if(create_romfile(top) == FAIL){
		fprintf(stderr, CREATE_ROMFILE_ERR_MSG);
		return FAIL;
	}
#endif
	/*init system upgrade status*/
	strcpy(msg.node,"System_Entry");
	strcpy(msg.attr,"upgrade_fw_status");
	sprintf(msg.value,"%s","NONE");
	tcapi_set_req(top, &msg);

	return SUCCESS;

}

#if defined(TCSUPPORT_CT_PON)
int readRomfileList(mxml_node_t *top, char nodeName[][MAX_NODE_NAME], char *basePath)
{
    DIR *dir = NULL;
    struct dirent *ptr = NULL;
	char attrName[32] = {0};
	char attrValue[64] = {0};
	int i = 0;
#if defined(TCSUPPORT_PRODUCTIONLINE) && defined(TCSUPPORT_RESERVEAREA_EXTEND)
	proline_Para para;
#endif

    if ((dir=opendir(basePath)) == NULL)
    {
        //perror("Open dir error...");
        return FAIL;
    }

    while ((ptr=readdir(dir)) != NULL)
    {
        if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    //current dir OR parrent dir
            continue;
        else if(ptr->d_type == 8){    	//file
#if defined(TCSUPPORT_CT_BACKUPROMFILEENCHANCEMENT)
			if(NULL!=strstr(ptr->d_name,"ctromfile")
#else
			if(NULL!=strstr(ptr->d_name,"romfile")
#endif
        	 && NULL!=strstr(ptr->d_name,".cfg") && NULL==strstr(ptr->d_name,"_f.cfg")){
				i++;
				sprintf(attrName, "romfile%d", i);
				setAttrValue(top, nodeName, attrName, ptr->d_name);				
        	}
        }
        else if(ptr->d_type == 10)    	//link file
            continue;
        else if(ptr->d_type == 4)    	//dir
        	continue;
    }
	sprintf(attrValue, "%d", i);
	setAttrValue(top, nodeName, "romfileCount", attrValue);
#if defined(TCSUPPORT_PRODUCTIONLINE) && defined(TCSUPPORT_RESERVEAREA_EXTEND)
	memset(&para, 0, sizeof(para));
	tcgetProLinePara(&para,0);
	if((PRODUCTLINEPARAMAGIC == para.magic) && (para.flag & PL_CP_ROMFILE_SELECT_FLAG))
	{
		sprintf(attrValue, "%s", para.romfileselect);
		setAttrValue(top, nodeName, "romfileSelect", attrValue);
	}
#endif	
    closedir(dir);
    return SUCCESS;
}

int setSelectRomfile(char *filename){
	char syscmd[100]={0};

	memset(syscmd,0,sizeof(syscmd));
	sprintf(syscmd,"prolinecmd romfileselect set %s",filename);
	system(syscmd);
	
	memset(syscmd,0,sizeof(syscmd));
	if(strstr(filename,"epon")!=NULL)			
		sprintf(syscmd,"prolinecmd xponmode set %s","EPON");
	else
		sprintf(syscmd,"prolinecmd xponmode set %s","GPON");
	system(syscmd);
	
	memset(syscmd,0,sizeof(syscmd));
	sprintf(syscmd,"prolinecmd restore default");
	system(syscmd);	
	return SUCCESS;
}
#endif

/*_____________________________________________________________________________
**      function name: system_read
**      descriptions:
**            To create system cfg node.
**
**      parameters:
**            top:    Put the top of cfg node tree.
**            name:  NULL
**
**      global:
**             system_attr
**
**      return:
**             None
**
**      call:
**   	 get_web_node
**   	 mxmlElementSetAttr
**
**      revision:
**      1. Here 2008/5/7
**____________________________________________________________________________
*/
int
system_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr){
	mxml_node_t *curNode=NULL;
  	char nodeName[3][MAX_NODE_NAME];
	int i=0;

	memset(nodeName,0, sizeof(nodeName));

	strcpy(nodeName[0], SYSTEM);
	strcpy(nodeName[1], SUB_NODE_NAME);
	/*Create a system node and Get the system cfg node address pointer*/
	curNode=get_web_node(top, nodeName);

	if(curNode){
		/*Init system node attribute-value*/
		for(i=0; strlen(system_node[i].attr)!=0; i++){
			mxmlElementSetAttr(curNode, system_node[i].attr, system_node[i].value);
		}
	}else{
		return FAIL;
	}

#if defined(TCSUPPORT_CT_PON)
	readRomfileList(top,nodeName,"/userfs/");
#endif

	return SUCCESS;
}/*end system_read*/

/*_____________________________________________________________________________
**      function name: system_execute
**      descriptions:
**            To setup reboot and skbmgr_hot_list_len information.
**
**      parameters:
**            top:   Put the system of cfg node tree that is used to execute system information.
**            name: Put the system cfg node name.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**
**      revision:
**      1. Here 2008/5/19
**____________________________________________________________________________
*/
int
system_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	int i=0;
	int retval=0;
	int xxxx=0;
	char attrName[32] = {0};
	char attrValue[64] = {0};
	char oriValue[64] = {0};

	/*Get the attribute-value*/
	for(i=0; strlen(system_node[i].attr)!=0; i++){
		xxxx=getAttrValue(top, name, system_node[i].attr, system_node[i].value);
#ifdef DEBUG
		fprintf(stderr,"xxxx:%d attr:%s value:%s\r\n",xxxx,system_node[i].attr, system_node[i].value);
#endif
	}

	/*Execute the action*/
	for(i=0; strlen(system_node[i].attr)!=0; i++){
		if(system_node[i].action!=NULL){
			retval +=system_node[i].action(top, system_node[i].value);
		}
	}

#if defined(TCSUPPORT_CT_PON)
	if( TCAPI_PROCESS_OK == getAttrValue(top, name, "newRomfileSelect", attrValue)
		&& '\0' != attrValue[0] )
	{
		getAttrValue(top, name, "romfileSelect", oriValue);
		if(strcmp(attrValue,oriValue)!=0){
			setAttrValue(top, name, "romfileSelect", attrValue);
			setSelectRomfile(attrValue);
		}
	}
#endif

	#ifdef DEBUG
	fprintf(stderr,"system_execute retval:%d\r\n",retval);
	#endif
	return retval;
}/*end system_execute*/

#if defined(TCSUPPORT_CT_PON_C9)
int
reginfo_init(void)
{
	int ret=0;
	cfg_node_t node={
			.name="RegInfo",
			.type=SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=reginfo_execute,
			.cfg_boot=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}

int
reginfo_execute(mxml_node_t * top,char name [ ] [ MAX_NODE_NAME ])
{
	char cmd[256]={0};
	char tmp[128]={0};
	
	memset(cmd, 0, sizeof(cmd));	
	memset(tmp, 0, sizeof(tmp));
	sprintf(cmd,"prolinecmd clear 1");
	system(cmd);
	if(getAttrValue(top, name, "productclass", tmp) == TCAPI_PROCESS_OK)
	{
		sprintf(cmd,"prolinecmd productclass set %s",tmp);
		system(cmd);
	}
	if(getAttrValue(top, name, "manufacturerOUI", tmp) == TCAPI_PROCESS_OK)
	{
		sprintf(cmd,"prolinecmd manufacturerOUI set %s",tmp);
		system(cmd);
	}
	if(getAttrValue(top, name, "serialnum", tmp) == TCAPI_PROCESS_OK)
	{
		sprintf(cmd,"prolinecmd serialnum set %s",tmp);
		system(cmd);
	}

	return SUCCESS;
}
#endif

#ifdef TCSUPPORT_BACKUPROMFILE
int sys_update_backup_romfile(char *romfilePath) 
{
	char cmd[128];

	memset(cmd, 0, sizeof(cmd));
	sprintf(cmd,UPDATE_ROMFILE_CMD,romfilePath);	
	system(cmd);
	usleep(100000);

	sprintf(cmd, TC_FLASH_ERASE_SECTOR_CMD,(unsigned long)BACKUPROMFILE_RA_OFFSET, RESERVEAREA_NAME);
	system(cmd);

	memset(cmd,0,sizeof(cmd));
	sprintf(cmd, TC_FLASH_WRITE_CMD,romfilePath,(unsigned long)BACKUPROMFILE_RA_SIZE,(unsigned long)BACKUPROMFILE_RA_OFFSET, RESERVEAREA_NAME);
	system(cmd);
	usleep(100000);

	return 0;
}
#endif
#if defined(TCSUPPORT_CT_BACKUPROMFILEENCHANCEMENT)
int xponRomfileMode(){
	int  fd=0;
	int xponMode = 2;	//default gpon

	fd = open(DEF_ROMFILE_PATH_EPON,O_RDONLY);
	if(fd == FAIL){
		//tcdbg_printf("\n/userfs/ctromfile_epon.cfg not exist");
		return xponMode;
	}
	close(fd);
#if defined(TCSUPPORT_PRODUCTIONLINE) && defined(TCSUPPORT_RESERVEAREA_EXTEND)
	proline_Para para;

	memset(&para, 0, sizeof(para));
	tcgetProLinePara(&para,0);

	if((PRODUCTLINEPARAMAGIC == para.magic) && (para.flag & PL_CP_XPONMODE_FLAG))
	{
		if( 0 == strcmp("EPON", para.xponmode))
		{
			//tcdbg_printf("\r\n %s %d update EPON romfile.",__FUNCTION__,__LINE__);
			xponMode = 1;
		}
	}
#endif
	//tcdbg_printf("\r\n %s %d xpon mode = %d.",__FUNCTION__,__LINE__, xponMode);
	return xponMode;
}
#endif


#if defined(TCSUPPORT_ECN_SIP)
extern void deRegBeforeReboot(mxml_node_t *top);
#endif

static void _sys_delay_reboot(mxml_node_t *top){
    int delayRebootSec = 0;
    char cmd[64] = {0};
    char tmp[32] = {0};
    
    char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
    
    
    memset(nodeName,0,sizeof(nodeName));
    strcpy(nodeName[0],"VoIPBasic");
    strcpy(nodeName[1],"Common");

#if defined(TCSUPPORT_ECN_SIP)
    deRegBeforeReboot(top);
#endif

    if(getAttrValue(top, nodeName, "DelayRebootTime", tmp) == TCAPI_PROCESS_OK){
        delayRebootSec = strtoul(tmp, 0, 16);
    }else
        delayRebootSec = 3;
    
    if (delayRebootSec)
        sprintf(cmd,"reboot -d %d &",delayRebootSec);
    else
        sprintf(cmd,"reboot");

    tcdbg_printf("reboot [cmd:%s]\n", cmd);
    system(cmd);
}

#if defined(TCSUPPORT_CT_PON_SC)
/*
* reset current romfie with defaultromfie in reserve area.
*/
void restore2Defaulromile()
{
	char buf[256] = {0};
	mxml_node_t *tmptree = NULL;
#define TMP_DF_ROMFILE_NAME "/tmp/tc_defromfile_tims"
	/*
	* 1. read romfile form defaultromfile area
	* TC_FLASH_READ_CMD --> "/userfs/bin/mtd readflash %s %lu %lu %s"
	*/
	snprintf(buf, sizeof(buf) - 1, TC_FLASH_READ_CMD,
			TMP_DF_ROMFILE_NAME,
			getDefaultRomfileRaSize(),
			getDefaultRomfileRaOffset(),
			RESERVEAREA_NAME);

	system(buf);
	usleep(100000);

	/*
	* 2. checksum and parse tree.
	*/
	tmptree = check_romfile(TMP_DF_ROMFILE_NAME);
	if ( !tmptree )
	{
		tcdbg_printf("\n invalid default romfile, do nothing!\n");
		return; /* romfile is bad, do nothing. */
	}
	mxmlDelete(tmptree);

	/*
	* 3. write it to current running romfile.
	* UPDATE_ROMFILE_CMD --> "/userfs/bin/mtd write %s romfile"
	*/
	snprintf(buf, sizeof(buf) - 1,
			UPDATE_ROMFILE_CMD, TMP_DF_ROMFILE_NAME);	
	system(buf);
	usleep(100000);

	unlink(TMP_DF_ROMFILE_NAME);

	return;
}
#endif

#if defined(TCSUPPORT_CUC)
/* check facotry reset mode. */
int cuc_check_factory_reset(mxml_node_t *top)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char tempValue[24] = {0};

	bzero(nodeName, sizeof(nodeName));
	strcpy(nodeName[0], "Sys");
	strcpy(nodeName[1], "Entry");

	if ( TCAPI_PROCESS_OK ==
		getAttrValue(top, nodeName, "FacResetMode", tempValue)
		&& 0 == strcmp(tempValue, "0") )
	{
		return 0;
	}

	return 1;
}
#endif


/*_____________________________________________________________________________
**      function name: sys_boot
**      descriptions:
**            To write default/current settings romfile into flash than  reboot system.
**
**      parameters:
**            top:   Put the system of cfg node tree that is used to execute system information.
**            value:
**				1:  Reboot with current settings
**				2:  Reboot with default factory settings
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**
**      revision:
**      1. Here 2008/5/19
**____________________________________________________________________________
*/
int
sys_boot(mxml_node_t *top,char *value)
{
#if defined(TCSUPPORT_CT_BACKUPROMFILEENCHANCEMENT)
	int flag = -1;
#endif
	char cmd[128];
	int mode = atoi(value);
	char romfilepath[64]={0};
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char cpeFlag[8] = {0};
	char cardExist[8] = {0};
	int needReset = 0;
	
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], "SimCard");
	strcpy(nodeName[1], "Entry");
	if(getAttrValue(top, nodeName, "cpeFlag", cpeFlag) == TCAPI_PROCESS_OK && strcmp(cpeFlag, "1") == 0
		&& getAttrValue(top, nodeName, "cardExist", cardExist) == TCAPI_PROCESS_OK && strcmp(cardExist, "1") == 0){
		needReset = 1;
	}
#endif
	
	memset(cmd, 0, sizeof(cmd));
	/*Reboot type*/
	switch(mode){
		case BOOT2CURSET:/*Reboot with current settings*/
			create_romfile(top);
			#ifdef TCSUPPORT_BACKUPROMFILE
#if defined(TCSUPPORT_CT_BACKUPROMFILEENCHANCEMENT)
			flag = compute_checksum(ROMFILE_PATH,STORE_ROMFILE_FILE_NAME);
			if(flag < 0)
			{
				fprintf(stderr, "\r\nsys_boot:Fail to compute checksum!\n");
				return FAIL;
			}
			sys_update_backup_romfile(STORE_ROMFILE_FILE_NAME);
			unlink(STORE_ROMFILE_FILE_NAME);
			_sys_delay_reboot(top);
#else
			sys_update_backup_romfile(ROMFILE_PATH);
			_sys_delay_reboot(top);
#endif
			#else
			sprintf(cmd,REBOOT_CMD, ROMFILE_PATH);
			system(cmd);
			#endif
			break;
		case BOOT2DEFSET:/*Reboot with default factory settings*/
#if defined(TCSUPPORT_CT_LONG_RESETBTN)
		case LONGRESET_BOOT2DEFSET:
#endif
#if defined(TCSUPPORT_CT_LONG_RESETBTN)
#if !defined(TCSUPPORT_CT_PON_GD) && !defined(TCSUPPORT_CT_PON_CZ_HN) && !defined(TCSUPPORT_CT_PON_CZ_CQ)
			if ( LONGRESET_BOOT2DEFSET == mode )
				backupTR69WanIndex();
#endif
#endif
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
			if(needReset)
				simCardReset(mode);
#endif
#if defined(TCSUPPORT_CT_JOYME)
			restoreOSGIRight(mode);
#endif
#if defined(TCSUPPORT_CT_DEFAULTPARA_STORE) || defined(TCSUPPORT_KEYPARA_STORE)
#if defined(TCSUPPORT_CT_DEFAULTPARA_STORE)	
			defaultParaRestore();	
#endif
#if defined(TCSUPPORT_KEYPARA_STORE)
			keyParaRestore(mode);	
#endif
#if defined(TCSUPPORT_CT_DEFAULTPARA_STORE)
		if(keyParaRestoreFlag == 0)//Have not done romfile update yet, so do update
			defaultParaRomfileUpdate();
#endif
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
			/* tony add to restore no keypara from sim card */
			if(needReset && BOOT2DEFSET == mode && SimCardResetFail == 0)
				setNeedReadSimCard(1);
			else
				system("reboot");
				
#else
			_sys_delay_reboot(top);
#endif
#else
			getDefaultRomfile(romfilepath);
		#ifdef TCSUPPORT_BACKUPROMFILE
			sys_update_backup_romfile(romfilepath);
			system("reboot");
		#else
			sprintf(cmd,REBOOT_CMD, romfilepath);
			system(cmd);
		#endif
#endif
			isbootset = 1;
			break;
		case WARM_REBOOT:/*Reboot with current settings*/
			system("reboot");
			break;
		case BOOT2DEFSETTR69:/*Reboot with default factory settings for tr069*/
#if defined(TCSUPPORT_CT_PON)
#if defined(TCSUPPORT_CUC)
			/* check facotry reset mode. */
			if ( 0 == cuc_check_factory_reset(top) )
			{
				snprintf(cmd, sizeof(cmd), TC_FLASH_ERASE_SECTOR_CMD, 0, "romfile");
				system(cmd);
				usleep(100000);
				snprintf(cmd, sizeof(cmd), TC_FLASH_ERASE_SECTOR_CMD,(unsigned long)BACKUPROMFILE_RA_OFFSET, RESERVEAREA_NAME);
				system(cmd);
				usleep(100000);

				_sys_delay_reboot(top);
				break;
			}
#endif
#if !defined(TCSUPPORT_CUC) && !defined(TCSUPPORT_CMCC)
			if (getPonState() == 1) {
				saveLoid_internal();
			}
#endif
#endif
#if defined(TCSUPPORT_CT_JOYME)
			restoreOSGIRight(mode);
#endif
#if defined(TCSUPPORT_CT_DEFAULTPARA_STORE)	
#if defined(TCSUPPORT_CUC)	
			backupTR69WanIndex();
#endif
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
			if(needReset)
				simCardReset(BOOT2DEFSETTR69);	
#endif
#if defined(TCSUPPORT_RESERVEAREA_EXTEND) && defined(TCSUPPORT_CT_SIMCARD_SEPARATION) && !defined(TCSUPPORT_CT_PHONEAPP)
			snprintf(cmd, sizeof(cmd), TC_FLASH_ERASE_SECTOR_CMD, 0, "romfile");
			system(cmd);
			usleep(100000);
			snprintf(cmd, sizeof(cmd), TC_FLASH_ERASE_SECTOR_CMD,(unsigned long)BACKUPROMFILE_RA_OFFSET, RESERVEAREA_NAME);
			system(cmd);
			usleep(100000);
#else
			defaultParaRestore();	
#if defined(TCSUPPORT_KEYPARA_STORE)
			keyParaRestore(mode);	
#endif
#if defined(TCSUPPORT_CT_DEFAULTPARA_STORE)
			if(keyParaRestoreFlag == 0)//Have not done romfile update yet, so do update
				defaultParaRomfileUpdate();
#endif
#endif
			_sys_delay_reboot(top);		
#else
#if defined(TCSUPPORT_C1_CUC)
			keyParaRestore(mode);	
#else
			getDefaultRomfile(romfilepath);
			#ifdef TCSUPPORT_BACKUPROMFILE
			sys_update_backup_romfile(romfilepath);
			system("reboot");
			#else
			sprintf(cmd,REBOOT_CMD, romfilepath);
			system(cmd);
#endif
			#endif
#endif
			break;			
#if defined(TCSUPPORT_CT_PON_SC)
			case BOOT2DEFSETITMS:
			restore2Defaulromile();
			_sys_delay_reboot(top);
			break;
#endif
#if defined(TCSUPPORT_CT_PON_SK)
		case BOOT2PUREFACTORY:
			system("/usr/bin/prolinecmd restore default");
			break;
#endif
#if defined(TCSUPPORT_CUC)
		case CUC_LONGRESET_BOOT2DEFSET:
			system("/usr/bin/prolinecmd restore default");
			break;
#endif

		case 0:/*Default value or error case, Do nothing*/
		default:
			break;
	}
	return SUCCESS;
}/*end sys_boot*/
/*_____________________________________________________________________________
**      function name: sys_skbmgr
**      descriptions:
**            To  setup skbmgr_hot_list_len information.
**
**      parameters:
**            top:   Put the system of cfg node tree that is used to execute system information.
**            value: You want to update skbmgr_hot_list_len value
**
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**
**      revision:
**      1. Here 2008/5/19
**____________________________________________________________________________
*/
int
sys_skbmgr(mxml_node_t *top,char *value)
{
	char cmd[64];

	memset(cmd, 0, sizeof(cmd));
	/*
	Update skbmgr_hot_list_len value
	ext: echo 512 > /proc/net/skbmgr_hot_list_len
	*/
	if(atoi(value) <= MAX_SKBMGR_LEN){
		sprintf(cmd ,SKBMGR_CMD, value);
	system(cmd);
	}
	else{
		return FAIL;
	}
	return SUCCESS;
}/*end sys_skbmgr*/

#ifdef TCSUPPORT_CT_PON
int
sys_avaTest(mxml_node_t *top, char *value)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char xponmode[10] = {0};
	int xpon_flag = 0;
	
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], "XPON");
	strcpy(nodeName[1], "LinkCfg");
	if (getAttrValue(top, nodeName, "LinkSta", xponmode) == TCAPI_PROCESS_OK && strcmp(xponmode, "2") == 0) {
		xpon_flag = 2; /* 0, auto, 1, gpon, 2, epon */
	}

	if (atoi(value) == 1) {
#if defined(TCSUPPORT_CT_JOYME)
		tcdbg_printf("\r\nfor avalanch test");
		system("hw_nat -U 1 1 1 1");
		system("hw_nat -N 1");
		system("hw_nat -T 1");
		system("sys memwl bfb518a0 c000000");
		system("echo 300000 > /proc/sys/net/nf_conntrack_max");
		system("rmmod maxnetdpi");
		system("accesslimitcmd mode 0");
		system("rmmod urlfilter");
		system("rmmod loopdetect");
		system("echo 8 > /proc/irq/22/smp_affinity");
		system("echo 4 > /proc/irq/23/smp_affinity");
		system("iptables -t filter -F");
		system("iptables -t filter -X");
		system("iptables -t mangle -F");
		system("iptables -t mangle -X");
		system("killall -9 jamvm");
		system("killall mobile-manager");
		system("killall usb_monitor");
		system("killall tr69");
		system("ponvlancmd disable");
		system("killall bandwidth");
		system("killall plugin_monitor");
		system("sys memwl bfb59fe0 83020");
		system("sys memwl bfb5b600 5e30b");
		system("/userfs/bin/ponmgr pwan set stormctrl 128 10");
		system("sys memwl bfb50e2c 19003200");
		system("sys memwl bfb50e30 00010c80");
		system("ebtables -F");
		system("ebtables -X");
		if (xpon_flag == 2) {
			tcdbg_printf("\r\nsys_avaTest(): current mode is epon");
			system("echo epon staticrpt 1 > /proc/gpon/debug");
		}
		system("echo 1024 > /proc/net/skbmgr_hot_list_len");
		system("echo 0 256 64 128 1 > /proc/tc3162/eth_rx_packet_number");
		
#else
		tcdbg_printf("\r\nconfig onu for avalanch test");
		system("hw_nat -U 1 1 1 1");
		system("hw_nat -N 1");
		system("hw_nat -T 1");
		system("echo 1 >/proc/tc3162/avalanche_test");
#endif
	}

	return SUCCESS;
}
#endif

#if 0
/*_____________________________________________________________________________
**      function name: sys_bkrom
**      descriptions:
**            Create the current settings romfile on /var/romfile.cfg.
**
**      parameters:
**            top:   Put the system of cfg node tree that is used to execute system information.
**            value: NULL
**
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**
**      revision:
**      1. Here 2008/5/19
**____________________________________________________________________________
*/
int
sys_bkrom(mxml_node_t *top, char *value)
{
	/*Create a current settings romfile*/
	if(atoi(value) == BAKROM_ON){
		if(create_romfile(top) == FAIL){
			fprintf(stderr, CREATE_ROMFILE_ERR_MSG);
			return FAIL;
		}
	}
	return SUCCESS;
}/*end sys_bkrom*/
#endif
/*_____________________________________________________________________________
**      function name: sys_upgrade_firmware
**      descriptions:
**            To upgrade firmware(romfile.cfg or linux.7z.trx or rootfs.trx).
**
**      parameters:
**            top:   Put the system of cfg node tree that is used to execute system information.
**            value:
**				1:  Upgrade romfile.cfg
**				2:  Upgrade linux.7z.trx
**				3:  Upgrade rootfs.trx
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**
**      revision:
**      1. Here 2008/5/19
**____________________________________________________________________________
*/
int
sys_upgrade_firmware(mxml_node_t *top, char *value)
{
	int retval=0;
	tcapi_msg_t msg;

	retval=upgarde_fw(atoi(value));
	if(retval == SUCCESS){
		/*set firmware upgrade status*/
		strcpy(msg.node,"System_Entry");
		strcpy(msg.attr,"upgrade_fw_status");
		if ( NO_OPERATION == atoi(value) )
			strcpy(msg.value,"NONE");
		else
		strcpy(msg.value,"SUCCESS");
		tcapi_set_req(top, &msg);
	}else{
		/*set firmware upgrade status*/
		strcpy(msg.node,"System_Entry");
		strcpy(msg.attr,"upgrade_fw_status");
		strcpy(msg.value,"FAIL");
		tcapi_set_req(top, &msg);

	}
	/*reset the upgrade_fw flag*/
	strcpy(msg.node,"System_Entry");
	strcpy(msg.attr,"upgrade_fw");
	sprintf(msg.value,"%d",NO_OPERATION);
	tcapi_set_req(top, &msg);

#if !defined(TCSUPPORT_UPGRADE_NO_REBOOT)

#ifdef PURE_BRIDGE
	/*Reboot after upload file wether the result is success or fail.*/
	if(retval == FAIL){
		system("reboot -d 1 &");
	}
#endif
#ifdef WITHVOIP
	/*Reboot after upload file wether the result is success or fail.*/
	if(retval == FAIL){
		system("reboot -d 1 &");
	}
#endif
#if defined(TCSUPPORT_CT_E8GUI)
#if defined(TCSUPPORT_CT_FW_UPGRADE_16M)
	if(retval == FAIL){
		system("reboot -d 1 &");
	}
#endif
#endif

#endif

	return retval;
}/*end sys_upgrade_firmware*/

#if 0
int write_to_flash(char come_from_web, char *file_path, char *img, int length, int offset){
	int fd=0;
	char cmd_buf[128];
	//struct stat buft;
	memset(cmd_buf,0,sizeof(cmd_buf));
	fd = open(file_path, O_RDONLY);
	if(fd == FAIL){
		return FAIL;
	}else{

		if(come_from_web){
			lseek(fd,offset,SEEK_CUR);
		}
		if(image_check_tc(fd,img) == FAIL){
			/*Firmware is illegal, so we need to delete it.*/
			unlink(file_path);
			close(fd);
			return FAIL;
		}else{
			/*Use mtd utility to upgrade firmware into flash*/
			//lseek(fd,0,SEEK_SET);
			close(fd);
			if(come_from_web){
				sprintf(cmd_buf,"/userfs/bin/mtd -r write %s %d %d %s",file_path, length, offset ,img);

				//tcdbg_printf("%s\n",cmd_buf);
			}else{
				sprintf(cmd_buf,"/userfs/bin/mtd -r write %s %s",file_path, img);
			}
			system(cmd_buf);
			unlink(file_path);
		}
	}

	return SUCCESS;
}


#endif

#if defined(TCSUPPORT_CT_PHONEAPP)
unsigned long determine_type(char *file_path, unsigned char *firmware_type)
{
	struct stat buf;

	if(stat(file_path,&buf) == 0)
	{
		*firmware_type = UG_TCLINUX;
	}

	return buf.st_size;
}
#endif

/*_____________________________________________________________________________
**      function name: choose_type
**      descriptions:
**           Choose the upgrade type according to it file name..
**
**      parameters:
**          file_path:  The oath of file.
**          firmware_type: User can get firmware_type by read this value.
**			length: User can get length of file by read this value.
**
**      global:
**             None
**
**      return:
**            length of file.
**
**      call:
**
**      revision:
**      1.shnwind 2009.3.31
**____________________________________________________________________________
*/

unsigned long choose_type(char *file_path, unsigned char *firmware_type){
	struct stat buf;

	if(stat(UPLOAD_ROMFILE_PATH,&buf) == 0){
		*firmware_type = UG_ROMFILE;
		strcpy(file_path,UPLOAD_ROMFILE_PATH);
#if 0		
	}else if(stat(KERNEL_PATH,&buf) == 0){
		*firmware_type = UG_KERNEL;
		strcpy(file_path,KERNEL_PATH);
	}else if(stat(ROOTFS_PATH,&buf) == 0){
		*firmware_type = UG_FS;
		strcpy(file_path,ROOTFS_PATH);
#endif		
	}else if(stat(TCLINUX_PATH,&buf) == 0){
		*firmware_type = UG_TCLINUX;
		strcpy(file_path,TCLINUX_PATH);
	}
	return buf.st_size;
}
#ifndef TCSUPPORT_UPGRADE_WDOGREBOOT
void start_reboot()
{
	sleep(2);
	fflush(stdout);
	fflush(stderr);
	syscall(SYS_reboot,LINUX_REBOOT_MAGIC1,LINUX_REBOOT_MAGIC2,LINUX_REBOOT_CMD_RESTART,NULL);
}
#endif

#if defined(TCSUPPORT_CT_UPLOAD_ALLINONE)
int upgrade_done = 0;
void delay_kill_cfg_manager()
{

	while( 0 == upgrade_done )
	{
		sleep(1);
	}
	sleep(3);
	system("/usr/bin/killall -9 cfg_manager 2>/dev/null");

}
#endif


/*_____________________________________________________________________________
**      function name: sys_upgrade_firmware
**      descriptions:
**           Check the firmware is validly or not and write into flash then reboot system.Otherwise
**           delete the illegal file on filesystem.
**
**      parameters:
**            top:   Put the system of cfg node tree that is used to execute system information.
**            type:
**				0:  Upgrade linux.7z.trx
**				1:  Upgrade rootfs.trx
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**
**      revision:
**      1. Here 2008/5/19
**		2.shnwind 2009/4/3
**____________________________________________________________________________
*/
int
upgarde_fw(int type){
#ifndef TCSUPPORT_UPGRADE_WDOGREBOOT
	pthread_t thread_id;
#endif
	mxml_node_t *top = get_node_top();
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char web_upgrade_flag[5];
#if defined(TCSUPPORT_CT_ZIPROMFILE)
	int ret = 0;
#endif
	int fd=0;
	char cmd_buf[128];
	char img[32];
	char file_path[32];
	unsigned int offset=0 , length=0;
	unsigned char firmware_type;
	unsigned long data_start, data_stop;
	int header_len=0;
#if defined(TCSUPPORT_CT)	
#ifdef CWMP	
	int n = 0;
	cwmp_msg_t message;
	#define CWMP_REBOOT  10
	memset(&message,0,sizeof(cwmp_msg_t));
	message.cwmptype = CWMP_REBOOT;
#endif
#endif
#if defined(TCSUPPORT_CT_UPLOAD_ALLINONE)
	tcapi_msg_t msg;
	unsigned int tclinux_len = 0;
#if defined(TCSUPPORT_CT_DUAL_IMAGE) || defined(TCSUPPORT_CUC_DUAL_IMAGE)
	long tclinux_slave_len = 0;
#endif
#endif
#if defined(TCSUPPORT_CT_JOYME)
	int eventInform = 0;
	char upgrade_ID[128] = {0}, newversion[64] = {0};
	char currversion[64] = {0};
	char nodeUPGName[MAX_ARG_NUM][MAX_NODE_NAME];
	char attrName[32] = "version=";
#endif

	memset(cmd_buf,0,sizeof(cmd_buf));
	memset(img,0,sizeof(img));
	memset(file_path, 0, sizeof(file_path));
	memset(nodeName, 0, sizeof(nodeName));
	memset(web_upgrade_flag, 0, sizeof(web_upgrade_flag));

#if defined(TCSUPPORT_CT_JOYME)
	if ( PHONE_NO_HEADER == type )
	{
		bzero(nodeUPGName, sizeof(nodeUPGName));
		strcpy(nodeUPGName[0], "OSGIUpgrade");
		strcpy(nodeUPGName[1], "Entry");

		bzero(upgrade_ID, sizeof(upgrade_ID));
		getAttrValue(top, nodeUPGName, "upgrade_ID", upgrade_ID);
		bzero(newversion, sizeof(newversion));
		getAttrValue(top, nodeUPGName, "fwver", newversion);
		bzero(currversion, sizeof(currversion));
		get_profile_str_new( attrName, currversion, sizeof(currversion)
			, DEVICEPARASTATIC_PATH);

		/* report error if download fail */
		if ( 0 != getDownloadstate() )
		{
			sendEventInform("1", currversion, INFORM_ERR_DLDERR, upgrade_ID);
			sleep(1);
			return FAIL;
		}
		else if ( 0 == strcmp(currversion, newversion) )
		{
			sendEventInform(INFORM_RESULT_FAIL, currversion
				, INFORM_ERR_SAMEVERSION, upgrade_ID);
			sleep(1);
			return FAIL;
		}
	}
#endif
	

	strcpy(nodeName[0],	"WebCustom");
	strcpy(nodeName[1],	"Entry");
	if(getAttrValue(top, nodeName, "web_upgrade", web_upgrade_flag) != TCAPI_PROCESS_OK)
		sprintf(web_upgrade_flag, "%d", 0);
	else
		setAttrValue(top, nodeName, "web_upgrade", "0");
	
	#ifdef DEBUG
	fprintf(stderr,"sys_upgrade_firmware type:%d\r\n", type);
	#endif

	/*decide firmware type, length, start offset, and file path. */
	switch(type){
		case NO_OPERATION:
			return SUCCESS;
		case NO_HEADER:
			length = choose_type(file_path, &firmware_type);
			offset = 0;
			break;
		case HTML_HEADER:
			strcpy(file_path,"/tmp/boa-temp");
			html_post_parser(file_path,&data_start,&data_stop,&firmware_type);
			unlink(MULTI_TO_NORMAL_TEMP);
			offset = data_start;
			length = data_stop - data_start + 1;
			//tcdbg_printf("start %ld stop %ld type %d length %d\n",data_start,data_stop,firmware_type,length);
			break;
#if defined(TCSUPPORT_CT_PHONEAPP)
		case PHONE_NO_HEADER:
			strcpy(file_path,"/tmp/phone-temp");
			length = determine_type(file_path, &firmware_type);
			offset = 0;
#if defined(TCSUPPORT_CT_JOYME)
			eventInform = 1;
#endif
			break;			
#endif
		default:
			unlink(file_path);
			return FAIL;
	}

#if defined(TCSUPPORT_CT_UPLOAD_ALLINONE)
	if ( UG_TCLINUX_ALLINONE == firmware_type )
		fd = open(file_path, O_RDWR);
	else
#endif
	/*check the firmware.*/
	fd = open(file_path, O_RDONLY);
	if(fd < 0){
		unlink(file_path);
#if defined(TCSUPPORT_CT_JOYME)
		if (eventInform )
			sendEventInform("1", currversion, INFORM_ERR_BINERR, upgrade_ID);
#endif
		return FAIL;
	}else{
#if defined(TCSUPPORT_CT_ZIPROMFILE)
		if(strcmp(web_upgrade_flag, "1") == 0 && firmware_type == UG_ROMFILE){
			ret = update_UploadedRomfile(fd, offset, length, "/tmp/tmp_romfile.cfg");
			if(ret < 0)
			{
				return FAIL;
			}else{
				close(fd);				
				unlink(file_path);
				
				compute_checksum("/tmp/tmp_romfile.cfg", file_path);

				unlink("/tmp/tmp_romfile.cfg");

				sprintf(cmd_buf, "cp -f %s %s", file_path, "/tmp/tmp_romfile.cfg");
				system(cmd_buf);

				fd = open(file_path, O_RDONLY);
				if(fd < 0){
					unlink(file_path);
					return FAIL;
				}
			}
		}
		else{
		lseek(fd,offset,SEEK_CUR);
		}
		
#else
		lseek(fd,offset,SEEK_CUR);
#endif

#if defined(TCSUPPORT_PON_ROSTELECOM)
		//tcdbg_printf("upgrade_fw:if some nodes missing ,new romfile should be formed\n");
		add_node_if = 2 ;
#endif

#if defined(TCSUPPORT_CT_UPLOAD_ALLINONE)
		header_len = image_check_tc(fd,firmware_type,file_path,img,length, &tclinux_len);
#else
		header_len = image_check_tc(fd,firmware_type,file_path,img,length);
#endif
		if( header_len == FAIL){
			close(fd);
			unlink(file_path);
#if defined(TCSUPPORT_CT_JOYME)
			if ( eventInform )
				sendEventInform("1", currversion, INFORM_ERR_BINERR, upgrade_ID);
#endif
			return FAIL;
		}
		close(fd);
		/*remove romfile tmp file*/
		unlink(UPLOAD_ROMFILE_PATH);
		
#if defined(TCSUPPORT_CT_ZIPROMFILE)
		if(strcmp(web_upgrade_flag, "1") == 0 && firmware_type == UG_ROMFILE){
			unlink(file_path);

			sprintf(cmd_buf, "mv -f %s %s", "/tmp/tmp_romfile.cfg", file_path);
			system(cmd_buf);
		}
#endif

	}
	/*Decide write length and offset.*/
	switch(firmware_type){
#if 0		
		case UG_KERNEL:
		case UG_FS:
#endif			
		case UG_TCLINUX:
#if 0
			/*skip trx header*/
			offset += header_len;
			length -= header_len;
#endif			
			break;
		case UG_ROMFILE:
		default:
			break;
	}
#if defined(TCSUPPORT_CT_JOYME)
	if ( eventInform )
		sendEventInform("0", newversion, "", upgrade_ID);
#endif

#if defined(TCSUPPORT_FW_INTERNET_LED)
	system("echo 0 8 > /proc/tc3162/led_internet");
	tcdbg_printf("upgrade_fw or romfile\n");
#endif

	if(strcmp(web_upgrade_flag, "1") == 0 && (firmware_type == UG_TCLINUX
#if defined(TCSUPPORT_CT_UPLOAD_ALLINONE)
	|| UG_TCLINUX_ALLINONE == firmware_type
#endif
	)){
		system("/usr/bin/killall -9 tftpd 2>/dev/null");
#ifndef TCSUPPORT_UPGRADE_WDOGREBOOT
		system("killall -9 tcwdog");
#endif
	}

#if defined(TCSUPPORT_CT)	
#if defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505)
	if(NO_HEADER == type)
	{
		system("killall -9 dnsmasq");
	}
#endif
#if 1	//def CWMP	
	tcdbg_printf("Please Waiting,CPE is erasing flash......\n");
#if defined(TCSUPPORT_CT_ZIPROMFILE)
	if(strcmp(web_upgrade_flag, "1") == 0 && firmware_type == UG_ROMFILE)
		sprintf(cmd_buf,"/userfs/bin/mtd -f write %s %d %d %s",file_path, length, 0 ,img);
	else
#endif
#if defined(TCSUPPORT_CT_UPLOAD_ALLINONE)
	if ( UG_TCLINUX_ALLINONE == firmware_type )
	{
		pthread_create(&thread_id, NULL, (void *)delay_kill_cfg_manager, NULL);

		// write bootloader
		sprintf(cmd_buf,"/userfs/bin/mtd -f write %s %d %d %s", file_path, TCBOOT_SIZE, offset, "bootloader");
		system(cmd_buf);
		// write romfile
		sprintf(cmd_buf,"/userfs/bin/mtd -f write %s %d %d %s", file_path, ROMFILE_SIZE, offset+TCBOOT_SIZE, "romfile");
		system(cmd_buf);
		// write tclinux
		sprintf(cmd_buf,"/userfs/bin/mtd -f write %s %d %d %s", file_path, tclinux_len, offset+TCBOOT_SIZE+ROMFILE_SIZE, "tclinux");
#if defined(TCSUPPORT_CT_DUAL_IMAGE) || defined(TCSUPPORT_CUC_DUAL_IMAGE)
		if ( (tclinux_slave_len = length-TCBOOT_SIZE-ROMFILE_SIZE-TCLINUX_SIZE-CKSUM_LENGTH) > 0 )
		{
			system(cmd_buf);
			// write tclinux
			sprintf(cmd_buf,"/userfs/bin/mtd -f write %s %d %d %s", file_path, tclinux_slave_len, offset+TCBOOT_SIZE+ROMFILE_SIZE+TCLINUX_SIZE, "tclinux_slave");
		}
#endif
	}
	else
#endif
	sprintf(cmd_buf,"/userfs/bin/mtd -f write %s %d %d %s",file_path, length, offset ,img);
#else
	sprintf(cmd_buf,"/userfs/bin/mtd -rf write %s %d %d %s &",file_path, length, offset ,img);
#endif
#else
	sprintf(cmd_buf,"/userfs/bin/mtd -rf write %s %d %d %s &",file_path, length, offset ,img);
#endif

#if defined(TCSUPPORT_PON_ROSTELECOM)
	/*1.if type = romfile,and has new romfile, use new romfile to upgrade*/
	/*2.html_post_parser,get paras; compute checksum*/
	/*3.erase flash and delete file*/
	//tcdbg_printf("firmware_type = %c , add_node_if = %d\n",firmware_type , add_node_if);
	if( (UG_ROMFILE == firmware_type) && (add_node_if == 1) )
	{
		memset(file_path, 0, sizeof(file_path));
		memset(cmd_buf, 0, sizeof(cmd_buf));
		strcpy(file_path,FRAG_ROMFILE_PATH);
		
		html_post_parser(file_path,&data_start,&data_stop,&firmware_type);
		unlink(MULTI_TO_NORMAL_TEMP);
		offset = data_start;
		length = data_stop - data_start + 1;

		compute_checksum(file_path , "/tmp/boa-temp-ct");
		
		//tcdbg_printf("start %ld stop %ld type %d length %d\n",data_start,data_stop,firmware_type,length);	
		sprintf(cmd_buf,"/userfs/bin/mtd -f write %s %d %d %s","/tmp/boa-temp-ct", length, 0 ,img);
	}
	add_node_if = 0 ;
	//tcdbg_printf("upgrade_fw:%s\n",cmd_buf);
#endif

	system(cmd_buf);
#if defined(TCSUPPORT_CT)
#if defined(TCSUPPORT_CT_UPG_PINGPONG)
         set_boot_flag(img);
#endif
#ifdef CWMP
	n = sendmegq(1,&message, 0);
	if(n < 0)
		tcdbg_printf("send message error\n");
#endif	
#endif

#if defined(TCSUPPORT_IPV6)
	if(NO_HEADER == type)
	{
		upgSysIPVersionHandle();
	}
#endif
	if(strcmp(web_upgrade_flag, "1") == 0)
	{
#if defined(TCSUPPORT_CT_UPLOAD_ALLINONE)
		strcpy(msg.node,"System_Entry");
		strcpy(msg.attr,"upgrade_fw_type");
		sprintf(msg.value, "%d", firmware_type);
		tcapi_set_req(top, &msg);
		if ( UG_TCLINUX_ALLINONE == firmware_type )
		{
			//pthread_create(&thread_id, NULL, (void *)delay_kill_cfg_manager, NULL);
			upgrade_done = 1;
		}
		else
#endif
		{
#ifndef TCSUPPORT_UPGRADE_WDOGREBOOT
		if( pthread_create(&thread_id, NULL, (void *)start_reboot, NULL) != 0 ){
			system("reboot -d 2 &");
			tcdbg_printf("pthread_create error!!\n");
		}
#endif
	}
	}
	return SUCCESS;

#if 0
	/*upgrade firmware by boa*/
	if(stat("/tmp/boa-temp",&buf) == 0){
		come_from_web = 1;
		fp = fopen("/tmp/offset_length", "r");
		if(fp == NULL ){
			return FAIL;
		}
		fgets(cmd_buf,sizeof(cmd_buf),fp);
		offset = atoi(cmd_buf);
		fgets(cmd_buf,sizeof(cmd_buf),fp);
		length = atoi(cmd_buf);
		fclose(fp);

	}

	switch(type){
		case BOOT:
			/*do nothing*/
			return SUCCESS;
		case UG_ROMFILE:
			strcpy(img,"romfile");
			if(come_from_web){
				strcpy(file_path, "/tmp/boa-temp");
			}else{
				strcpy(file_path, UPLOAD_ROMFILE_PATH);
			}
			break;
		case UG_KERNEL:
			tcdbg_printf("cfg kernel\n");
			strcpy(img,"kernel");
			if(come_from_web){
				strcpy(file_path, "/tmp/boa-temp");
			}else{
				strcpy(file_path, KERNEL_PATH);
			}
			break;
		case UG_FS:
			strcpy(img,"rootfs");
			if(come_from_web){
				strcpy(file_path, "/tmp/boa-temp");
			}else{
				strcpy(file_path, ROOTFS_PATH);
			}
			break;
		case UG_RAS:
			//strcpy(img,"ras");
			if(come_from_web){
				strcpy(file_path, "/tmp/boa-temp");
				//strcpy(file_path2, "/tmp/boa-temp");
			}else{
				strcpy(file_path, ROOTFS_PATH);
				//strcpy(file_path2, ROOTFS_PATH);
			}
			//tcdbg_printf("===========%d %d\n",offset,length);
			//return SUCCESS;
			if(come_from_web){
				sprintf(cmd_buf,"/userfs/bin/mtd -f write %s %d %d kernel",file_path, length, offset );
				//tcdbg_printf("cmd %s\n",cmd_buf);
			}else{
				sprintf(cmd_buf,"/userfs/bin/mtd -f write %s kernel",file_path);
				//tcdbg_printf("cmd %s\n",cmd_buf);
			}
			system(cmd_buf);
			//sleep(1);
			fp = fopen("/tmp/offset_length2", "r");
			if(fp == NULL ){
				return FAIL;
			}
			fgets(cmd_buf,sizeof(cmd_buf),fp);
			offset = atoi(cmd_buf);
			//fprintf(stderr,"offset = %d\n",offset);
			fgets(cmd_buf,sizeof(cmd_buf),fp);
			length = atoi(cmd_buf);
			//fprintf(stderr,"length = %d\n",length);
			fclose(fp);
			//tcdbg_printf("============%d %d\n",offset,length);
			if(come_from_web){
				sprintf(cmd_buf,"/userfs/bin/mtd -rf write %s %d %d rootfs",
							file_path, length, offset);
				//tcdbg_printf("cmd %s\n",cmd_buf);
			}else{
				sprintf(cmd_buf,"/userfs/bin/mtd -rf write %s rootfs",file_path);
				//tcdbg_printf("cmd %s\n",cmd_buf);
			}
			system(cmd_buf);
			unlink(file_path);
			return SUCCESS;
			break;
		default :
			/*None support this type*/
			return FAIL;
	}

	return write_to_flash(come_from_web, file_path, img, length, offset);


	if(type == BOOT){
		/*do nothing*/
		return SUCCESS;
	}
	else if(type==UG_ROMFILE){
		strcpy(img,"romfile");
		strcpy(file_path, UPLOAD_ROMFILE_PATH);
	}
	else if(type==UG_KERNEL){
		strcpy(img,"kernel");
		strcpy(file_path, KERNEL_PATH);
	}
	else if(type==UG_FS){
		strcpy(img,"rootfs");
		strcpy(file_path, ROOTFS_PATH);
	}

	else{
		/*None support this type*/
		return FAIL;
	}
	fd=open(file_path, O_RDONLY);
	if(fd == FAIL){
		return FAIL;
	}else{
		if(image_check_tc(fd,img)==FAIL){
			/*Firmware is illegal, so we need to delete it.*/
			unlink(file_path);
			close(fd);

			return FAIL;
		}
		else{
			/*Use mtd utility to upgrade firmware into flash*/
			close(fd);
			sprintf(cmd_buf,"%s %s %s &",MTD_WRITE_WITH_REBOOT,file_path, img);
			system(cmd_buf);
		}
	}
#endif
	return SUCCESS;
}/*end upgarde_fw*/

#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)
int
sys_start_up(mxml_node_t *tree,char *value){

	mxml_node_t *node=NULL;
	cfg_node_t *all_cfg_node=NULL;
	cfg_node_t *ptr=NULL;
	#if defined(TCSUPPORT_CPU_MT7510) || defined(TCSUPPORT_CPU_MT7520) || defined(TCSUPPORT_CPU_MT7505)
	pthread_t thread_id;
	pthread_attr_t thread_attr;
	#endif
	if(atoi(value) == 1){
		all_cfg_node=getAllCfgTable();
		if(all_cfg_node){
			for(ptr = all_cfg_node; ptr->name!= NULL; ptr++){
				node = mxmlFindElement(tree, tree, ptr->name,
			  		NULL, NULL,MXML_DESCEND);
				if((node !=NULL)
				&&(ptr->cfg_boot!=NULL)){
					ptr->cfg_boot(tree);
				}
			}
		}
#if defined(TCSUPPORT_CPU_MT7510) || defined(TCSUPPORT_CPU_MT7520) ||  defined(TCSUPPORT_CPU_MT7505)
		if(0 == pre_sys_state.cc_is_run)
		{
			pthread_attr_init(&thread_attr);
			pthread_attr_setstacksize(&thread_attr, 16384);
			pthread_attr_setdetachstate(&thread_attr,PTHREAD_CREATE_DETACHED); 

			if( pthread_create(&thread_id, &thread_attr, (void *)start_cc, NULL) != 0 ){
				fprintf(stderr, "pthread_create error!!\n");
				exit(0);
			}
			pre_sys_state.cc_is_run = 1;
		}
#endif
	}

/*****************************************************************************************/
	else if(atoi(value) == 2)
	{		
		all_cfg_node=getAllCfgTable();
		if(all_cfg_node)
		{
			for(ptr = all_cfg_node; ptr->name!= NULL; ptr++)
			{
				node = mxmlFindElement(tree, tree, ptr->name,
			  		NULL, NULL,MXML_DESCEND);
				if((strcmp(ptr->name, VOIPSYSPARAM) == 0) && (node !=NULL) && (ptr->cfg_boot != NULL))
				{
					ptr->cfg_boot(tree);
				}
			}
		}	
	}
/******************************************************************************************/

	return SUCCESS;
}/*end getAllCfgTable*/
#else
int
sys_start_up(mxml_node_t *tree,char *value){

	mxml_node_t *node=NULL;
	cfg_node_t *all_cfg_node=NULL;
	cfg_node_t *ptr=NULL;
	if(atoi(value)){
		all_cfg_node=getAllCfgTable();
		if(all_cfg_node){
			for(ptr = all_cfg_node; ptr->name!= NULL; ptr++){
				node = mxmlFindElement(tree, tree, ptr->name,
			  		NULL, NULL,MXML_DESCEND);
				if((node !=NULL)
				&&(ptr->cfg_boot!=NULL)){
					ptr->cfg_boot(tree);
				}
			}
		}
	}

	return SUCCESS;
}/*end getAllCfgTable*/


#endif
/*_____________________________________________________________________________
**      function name: image_check_tc
**      descriptions:
**           Check the firmware is validly or not.
**
**      parameters:
**            imagefd:   Put the firmware file descrptor.
**            mtd: kernel or rootfs
**
**
**      global:
**             None
**
**      return:
**            Success:        trx_header_len
**            Otherwise:     -1
**
**      call:
**
**      revision:
**      1. Here 2008/5/19
** 		2.shnwind 2009.3.31
**____________________________________________________________________________
*/
#if defined(TCSUPPORT_CT_BACKUPROMFILEENCHANCEMENT)
#if defined(TCSUPPORT_CT_UPLOAD_ALLINONE)
int getCurMAC(unsigned char *mac)
{
	FILE *fp = NULL;

	if ( !mac )
		return -1;

	memset(mac, 0, 6);
    system("cat /dev/mtd0 > /tmp/boot.bin");

    fp = fopen("/tmp/boot.bin", "rb");
    if( !fp )
	{
		tcdbg_printf("[%s]: open boot.bin file fauked\n", __FUNCTION__);
        system("rm -f /tmp/boot.bin");
        return -2;
    }

    fseek(fp, MAC_IN_TCBOOT, SEEK_SET);
	fread(mac, 6, 1, fp);
	system("rm -f /tmp/boot.bin");

	return 0;
}
#endif

#if defined(TCSUPPORT_CT_UPG_PINGPONG)

#define DUAL_IMG_BOOT_FLAG_FILE 	"/tmp/dual_image_boot_flag"
char get_boot_flag()
{
	char cmds[256] = {0};
	FILE *fp=NULL;
	char boot_flag = -1;

	sprintf(cmds, TC_FLASH_READ_CMD, DUAL_IMG_BOOT_FLAG_FILE, (unsigned long)IMG_BOOT_FLAG_SIZE, (unsigned long)IMG_BOOT_FLAG_OFFSET, RESERVEAREA_NAME);

	system(cmds);

	fp = fopen(DUAL_IMG_BOOT_FLAG_FILE, "r");
	if (fp == NULL)
	{
		return -1;
	}
	
	if (1 != fread(&boot_flag, 1, 1, fp))
	{
		fclose(fp);
		return -1;
	}
	fclose(fp);
	
	boot_flag = boot_flag - '0';

	return boot_flag;	
}

int set_boot_flag(char *mtd)
{
	char cmds[256] = {0};
	FILE *fp=NULL;
	char boot_flag = -1;
	
	if(NULL == mtd)
	{
		tcdbg_printf("%s: invalid parameter.\n",__FUNCTION__);
		return -1;
	}

	if(0 == strcmp(mtd,"tclinux_slave"))
	{
		boot_flag = '1';
	}
	else if(0 == strcmp(mtd,"tclinux"))
	{
		boot_flag = '0';
	}
	else
	{
		tcdbg_printf("%s: invalid mtd:%s\n",__FUNCTION__,mtd);
		return -1;
	}
	
	fp = fopen(DUAL_IMG_BOOT_FLAG_FILE, "w");
	if (fp == NULL)
	{
		return -1;
	}
	
	if (1 != fwrite(&boot_flag, 1, 1, fp))
	{
		fclose(fp);
		return -1;
	}
	fclose(fp);

	snprintf(cmds, sizeof(cmds), TC_FLASH_WRITE_CMD, DUAL_IMG_BOOT_FLAG_FILE, (unsigned long)IMG_BOOT_FLAG_SIZE, (unsigned long)IMG_BOOT_FLAG_OFFSET, RESERVEAREA_NAME);

	system(cmds);

	return 0;
	
}
#endif

/*modify the return value 
before-------return trx_header_len if success
now---------return 0 if success
*/
int
image_check_tc(int imagefd, char firmware_type, char *file_path,char *mtd, unsigned  int length
#if defined(TCSUPPORT_CT_UPLOAD_ALLINONE)
					, unsigned int *tclinuxbin_len
#endif
					)
{
	char img_buf[sizeof(struct trx_header)];
	struct trx_header *trx = (struct trx_header *) img_buf;
	struct mtd_info_user mtdInfo;
	int fd, buflen;
	mxml_node_t *tree=NULL;
	int readmtd = 1;
#if defined(TCSUPPORT_CT_UPLOAD_ALLINONE)
	unsigned long cal_crc = 0;
	unsigned long filecrc = 0;
	unsigned char fcrcbuf[CKSUM_LENGTH] = {0};
	int allinone_hdr = 0;
	unsigned char curmac[6] = {0};
#endif

	if(firmware_type == UG_ROMFILE)
	{
		strcpy(mtd,"romfile");
	}
	else if(firmware_type == UG_TCLINUX)
	{
#if defined(TCSUPPORT_CT_UPG_PINGPONG)
		int bootFlag = get_boot_flag();

		if (bootFlag == 1)
		{
			strcpy(mtd,"tclinux");
		}
		else
		{
			strcpy(mtd,"tclinux_slave");
		}
#else
		strcpy(mtd,"tclinux");
#endif
	}
#if defined(TCSUPPORT_CT_UPLOAD_ALLINONE)
	else if ( UG_TCLINUX_ALLINONE == firmware_type )
	{
		readmtd = 0; // no need to read mtd info for allinone
		allinone_hdr = lseek(imagefd, 0, SEEK_CUR); // backup allinone header
		if ( -1 == allinone_hdr )
		{
			tcdbg_printf("[%s]: backup allinone header failed\n", __FUNCTION__);
			return FAIL;
		}
	}
#endif
	else
	{
		tcdbg_printf("image_check_tc:not support this type\n");
		return FAIL;
	}
	
	if ( readmtd )
	{
	/* check if image(or romfile) fits to mtd device */
	fd = mtd_open(mtd, O_RDWR | O_SYNC);
	if(fd < 0) {
		tcdbg_printf("Could not open mtd device: %s\n", mtd);
		return FAIL;
	}

	if(ioctl(fd, MEMGETINFO, &mtdInfo)) {
		close(fd);
		tcdbg_printf("Could not get MTD device info from %s\n", mtd);
		return FAIL;
	}

	close(fd);
	}
	
	switch(firmware_type){
		case UG_ROMFILE:
			/*Check the romfile image is vaildly or not.*/
			close(imagefd);//first close,because in function 'check_romfile' will open the file
			tree = check_romfile(file_path);
			if(tree == NULL)
			{
				tcdbg_printf("image_check_tc:romfile check failed!\n");
				return FAIL;
			}
			break;
			
		case UG_TCLINUX:
			/*Check the tclinux is vaildly or not.*/
			buflen = read(imagefd, img_buf, sizeof(struct trx_header));
			//max len check
			if(mtdInfo.size < trx->len)
			{
				tcdbg_printf("Image too big for partition: %s\n", mtd);
				return FAIL;
			}
			//mix len check
			if (buflen < sizeof(struct trx_header)) 
			{
				tcdbg_printf("Could not get image header, file too small (%d bytes)\n", buflen);
				return FAIL;
			}
			//magic check
			if ((trx->magic != TRX_MAGIC2) || (trx->len < sizeof(struct trx_header))) {
				tcdbg_printf("filesystem: Bad trx header\n");
				return FAIL;
			}
			//crc check
			if(trx->crc32 != calculate_crc32(imagefd, (trx->len-sizeof(struct trx_header))))
			{
				tcdbg_printf("crc32 check fail\n");
				return FAIL;
			}
			break;
#if defined(TCSUPPORT_CT_UPLOAD_ALLINONE)
		case UG_TCLINUX_ALLINONE:
			// 1. move to tclinux area then check
			if( -1L == lseek(imagefd, TCBOOT_SIZE + ROMFILE_SIZE, SEEK_CUR) )
			{
				tcdbg_printf("[%s]: lseek to tclinuxbin header failed\n", __FUNCTION__);
				return FAIL;
			}
			// check the tclinux is vaildly or not.
			buflen = read(imagefd, img_buf, sizeof(struct trx_header));
			//mix len check
			if (buflen < sizeof(struct trx_header)) 
			{
				tcdbg_printf("[%s]: could not get image header, file too small (%d bytes)\n", __FUNCTION__, buflen);
				return FAIL;
			}
			//magic check
			if ((trx->magic != TRX_MAGIC2) || (trx->len < sizeof(struct trx_header)))
			{
				tcdbg_printf("[%s]: Bad trx header trx->magic=[%X], TRX_MAGIC2=[%X], trx->len=[%X] \n", __FUNCTION__, trx->magic, TRX_MAGIC2, trx->len);
				return FAIL;
			}
			// 2.move to allinone header again to calculate crc
			if( -1L == lseek(imagefd, allinone_hdr, SEEK_SET) )
			{
				tcdbg_printf("[%s]: lseek to allinone header failed\n", __FUNCTION__);
				return FAIL;
			}
			// check allinone crc
			cal_crc = calculate_crc32(imagefd, length - CKSUM_LENGTH);
			buflen = read(imagefd, fcrcbuf, CKSUM_LENGTH);
			filecrc |= fcrcbuf[0];
			filecrc |= fcrcbuf[1] << 8;
			filecrc |= fcrcbuf[2] << 16;
			filecrc |= fcrcbuf[3] << 24;
			if ( buflen != CKSUM_LENGTH
				|| cal_crc != filecrc )
			{
				tcdbg_printf("[%s]: crc32 check fail cal_crc=[%X], filecrc=[%X].\n", __FUNCTION__, cal_crc, filecrc);
				return FAIL;
			}
			*tclinuxbin_len = trx->len;
			// all is done, modify the MAC address.
			if( -1L == lseek(imagefd, allinone_hdr+MAC_IN_TCBOOT, SEEK_SET) )
			{
				tcdbg_printf("[%s]: lseek to mac in tcboot failed.\n", __FUNCTION__);
				return FAIL;
			}
			if ( 0 != getCurMAC(curmac) )
				return FAIL;
			if ( 6 != write(imagefd, curmac, 6) )
			{
				tcdbg_printf("[%s]: change the mac in tcboot failed.\n", __FUNCTION__);
				return FAIL;
			}
			break;
#endif
			
		default:
			/*Not supprot this type*/
			return FAIL;
	}

	return SUCCESS;
}/*end image_check_tc*/
#else
int
image_check_tc(int imagefd, char firmware_type, char *file_path,char *mtd, unsigned  int length)
{
	char img_buf[sizeof(struct trx_header)];
	struct trx_header *trx = (struct trx_header *) img_buf;
	struct mtd_info_user mtdInfo;
	int fd, buflen;
	mxml_node_t *tree=NULL;
	FILE *fp=NULL;
	struct stat fs;
	int romfile_length, get_len;

	/*get trx header*/

	if(firmware_type != UG_ROMFILE){
		buflen = read(imagefd, img_buf, sizeof(struct trx_header));

		if (buflen < sizeof(struct trx_header)) {
			tcdbg_printf("Could not get image header, file too small (%d bytes)\n", buflen);
			return FAIL;
		}
	}

	switch(firmware_type){
		case UG_ROMFILE:
			/*get romfile for verify*/
			romfile_length = length;
			fp = fopen(UPLOAD_ROMFILE_PATH, "w");
			if(fp == NULL){
				return FAIL;
			}
			while(romfile_length > 0){
					if(romfile_length >= sizeof(struct trx_header)){
						get_len = sizeof(struct trx_header);
					}else{
						get_len = romfile_length;
					}
					romfile_length -= get_len;

					read(imagefd, img_buf, get_len);
					fwrite(img_buf,get_len,1,fp);
			}
			fclose(fp);
			/*Check the romfile image is vaildly or not.*/
			fp = fopen(UPLOAD_ROMFILE_PATH, "r");
			if(fp == NULL){
				return FAIL;
			}

			tree = mxmlLoadFile(NULL, fp, MXML_TEXT_CALLBACK);
			fclose(fp);
			if(tree == NULL){
				return FAIL;
			}else{
				if( verify_romfile(tree) == FAIL){
					tcdbg_printf("Romfile:wrong format");
					/*Romfile format is not vaild, so we need to delete it.*/
					unlink(UPLOAD_ROMFILE_PATH);
					return FAIL;
				}
				/*Delete cfg node tree that is used to verfiy_upload_romfile*/
				mxmlDelete(tree);
			}

			strcpy(mtd,"romfile");
			break;
#if 0			
		case UG_KERNEL:
			/*Check the kernel image is vaildly or not.*/
			if ((trx->magic != TRX_MAGIC) || (trx->len < sizeof(struct trx_header))) {
				tcdbg_printf("kernel: Bad trx header\n");
				return FAIL;
			}
			strcpy(mtd,"kernel");
			break;
		case UG_FS:
			/*Check the rootfs is vaildly or not.*/
			if ((trx->magic != TRX_MAGIC1) || (trx->len < sizeof(struct trx_header))) {
				tcdbg_printf("filesystem: Bad trx header\n");
				return FAIL;
			}

			strcpy(mtd,"rootfs");
			break;
#endif			
		case UG_TCLINUX:
			/*Check the tclinux is vaildly or not.*/
			if ((trx->magic != TRX_MAGIC2) || (trx->len < sizeof(struct trx_header))) {
				tcdbg_printf("filesystem: Bad trx header\n");
				return FAIL;
			}
			strcpy(mtd,"tclinux");
			break;
		default:
			/*Not supprot this type*/
			return FAIL;
	}
	/*check crc32*/
	//tcdbg_printf("head len %d version %s \n",trx->header_len,trx->version);
	if(firmware_type != UG_ROMFILE){
		/*ROMFILE does not have trx header*/
		if(trx->crc32 != calculate_crc32(imagefd, (trx->len-sizeof(struct trx_header)))){
			tcdbg_printf("crc32 check fail\n");
			return FAIL;
		}
	}

	/* check if image fits to mtd device */
	fd = mtd_open(mtd, O_RDWR | O_SYNC);
	if(fd < 0) {
		tcdbg_printf("Could not open mtd device: %s\n", mtd);
		return FAIL;
	}

	if(ioctl(fd, MEMGETINFO, &mtdInfo)) {
		close(fd);
		tcdbg_printf("Could not get MTD device info from %s\n", mtd);
		return FAIL;
	}
	close(fd);

	if(firmware_type == UG_ROMFILE){
		/*ROMFILE isn't include trx header, so we check the file size*/
		if(stat(UPLOAD_ROMFILE_PATH, &fs) == 0){
			if(mtdInfo.size < fs.st_size){
				tcdbg_printf("Image too big for partition: romfile\n");
				return FAIL;
			}
		}

	}else{
		/*Check the image size*/
		if(mtdInfo.size < trx->len) {
			tcdbg_printf("Image too big for partition: %s\n", mtd);
			return FAIL;
		}
	}

	return trx->header_len;
}/*end image_check_tc*/
#endif
#if 0
int
image_check_tc(int imagefd, const char *mtd)
{
	char img_buf[sizeof(struct trx_header)];
	struct trx_header *trx = (struct trx_header *) img_buf;
	struct mtd_info_user mtdInfo;
	int fd;
	int buflen;
	mxml_node_t *tree=NULL;
	FILE *fp=NULL;
	struct stat fs;


	buflen = read(imagefd, img_buf, sizeof(struct trx_header));
	if (buflen < sizeof(struct trx_header)) {
		fprintf(stderr, "Could not get image header, file too small (%d bytes)\n", buflen);
		return FAIL;
	}

	if(strcmp(mtd, "kernel") == 0){
		/*Check the kernel image is vaildly or not.*/
		if ((trx->magic != TRX_MAGIC) || (trx->len < sizeof(struct trx_header))) {
			fprintf(stderr, "kernel: Bad trx header\n");
			return FAIL;
		}
	}
	else if(strcmp(mtd, "rootfs") == 0){
		/*Check the rootfs is vaildly or not.*/
		if ((trx->magic != TRX_MAGIC1) || (trx->len < sizeof(struct trx_header))) {
			fprintf(stderr, "filesystem: Bad trx header\n");
			return FAIL;
		}
	}
	else if(strcmp(mtd, "romfile") == 0){
		/*Check the romfile image is vaildly or not.*/
		fp = fopen(UPLOAD_ROMFILE_PATH, "r");
		if(fp == NULL){
			return FAIL;
		}

		tree = mxmlLoadFile(NULL, fp, MXML_TEXT_CALLBACK);
		fclose(fp);
		if(tree==NULL){
			return FAIL;
		}
		else{
			if( verify_romfile(tree) == FAIL){
				fprintf(stderr, "Romfile:wrong format");
				/*Romfile format is not vaild, so we need to delete it.*/
				unlink(UPLOAD_ROMFILE_PATH);
				return FAIL;
			}
			/*Delete cfg node tree that is used to verfiy_upload_romfile*/
			mxmlDelete(tree);
		}
	}
	else{
		/*None supprot this type*/
		return FAIL;
	}

	/* check if image fits to mtd device */
	fd = mtd_open(mtd, O_RDWR | O_SYNC);
	if(fd < 0) {
		fprintf(stderr, "Could not open mtd device: %s\n", mtd);
		return FAIL;
	}

	if(ioctl(fd, MEMGETINFO, &mtdInfo)) {
		fprintf(stderr, "Could not get MTD device info from %s\n", mtd);
		return FAIL;
	}

	if(strcmp(mtd,"romfile")==0){
		/*ROMFILE isn't include trx header, so we check the file size*/
		if(stat(UPLOAD_ROMFILE_PATH, &fs) == 0){
			if(mtdInfo.size < fs.st_size){
				return FAIL;
			}
		}
	}
	else{
		/*Check the image size*/
		if(mtdInfo.size < trx->len) {
			fprintf(stderr, "Image too big for partition: %s\n", mtd);
			close(fd);
			return FAIL;
		}
	}

	close(fd);
	return SUCCESS;
}/*end image_check_tc*/
#endif
/*_____________________________________________________________________________
**      function name: mtd_open
**      descriptions:
**           Check the firmware is validly or not.
**
**      parameters:
**            imagefd:   Put the firmware file descrptor.
**            mtd: kernel or rootfs
**
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**
**      revision:
**      1. Here 2008/5/19
**____________________________________________________________________________
*/
int
mtd_open(const char *mtd, int flags)
{
	FILE *fp;
	char dev[PATH_MAX];
	int i;

	if ((fp = fopen("/proc/mtd", "r"))) {
		while (fgets(dev, sizeof(dev), fp)) {
			/*Get the image of mtd block*/
			if (sscanf(dev, "mtd%d:", &i) && strstr(dev, mtd)) {
				snprintf(dev, sizeof(dev), "/dev/mtd%d", i);
				fclose(fp);
				return open(dev, flags);
			}
		}
		fclose(fp);
	}

	return open(mtd, flags);
}/*end mtd_open*/

//"Diagnostic",	UPDATE_TYPE|WEB_TYPE|PVC_TYPE,	NULL, diag_merge_table,diagnostic_read, NULL, NULL,  diagnostic_execute},
char *
diag_merge_table[MAX_NODE_NAME]=
{
	"diag_pvc","",
};
/*______________________________________________________________________________
**	diagnostic_init
**
**	descriptions:
**		the init function of diagnostic.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
diagnostic_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="Diagnostic",
			.type=UPDATE_TYPE|WEB_TYPE|PVC_TYPE,
			.adv_table=NULL,
			.merge_table=diag_merge_table,
			.cfg_read=diagnostic_read,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=diagnostic_execute,
			.cfg_boot=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}
/*_____________________________________________________________________________
**      function name: diagnostic_read
**      descriptions:
**            To create diagnostic cfg node.
**
**      parameters:
**            top:    Put the top of cfg node tree.
**            name:  NULL
**
**      global:
**             None
**
**      return:
**             None
**
**      call:
**   	 mxmlFindElement
**   	 mxmlNewElement
**
**      revision:
**      1. Here 2008/5/7
**____________________________________________________________________________
*/
int
diagnostic_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr){
	mxml_node_t *curNode = NULL;

	/*Create diagnostic cfg node*/
	curNode = mxmlFindElement(top, top, DIAGNOSTIC,
				NULL, NULL, MXML_DESCEND);
	if(curNode==NULL){
		curNode =mxmlNewElement(top,  DIAGNOSTIC);
	}

	return SUCCESS;
}/*end diagnostic_read*/

static sigjmp_buf jmpbuf;
static void alarm_func()
{
	siglongjmp(jmpbuf,1);
}

/*This function only for ping/traceroute diagnose,and it is Not reentrant */
struct hostent *tc_gethostbyname(char *hostname, int timeout)
{
	struct hostent *IpHostEnt;
	signal(SIGALRM, alarm_func);
	if(sigsetjmp(jmpbuf, 1) != 0)
	{
		alarm(0);//timeout
		signal(SIGALRM, SIG_IGN);
		return NULL;
	}
	alarm(timeout);
	IpHostEnt = gethostbyname(hostname);
	signal(SIGALRM, SIG_IGN);

	return IpHostEnt;
}

/*This function only for ping/traceroute diagnose, and it is Not reentrant*/
int tc_getaddrinfo(const char *hostname, const char *service, const struct addrinfo *hints, struct addrinfo **result, int timeout)
{
	int reval = -1;
	signal(SIGALRM, alarm_func);
	if(sigsetjmp(jmpbuf, 1) != 0)
	{
		alarm(0);//timeout
		signal(SIGALRM, SIG_IGN);
		return reval;
	}
	alarm(timeout);
	reval = getaddrinfo(hostname, service, hints, result);
	signal(SIGALRM, SIG_IGN);

	return reval;
}

#if defined(TCSUPPORT_CT_DSLITE)
extern int get_dslite_stateviacfg(mxml_node_t *top, char nodeName[][MAX_NODE_NAME]);
#endif
/*This function is used to check if wan is active and have ip*/
int checkWan_by_name(char *wan_if_name, int *ipMode){
	int if_index;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char temp[32] = {0};	
	mxml_node_t *top;
	char str_ipversion[16] = {0};
	
	if_index = get_wanindex_by_name(wan_if_name);
	if(get_waninfo_by_index(if_index, "Active", temp) == SUCCESS && strcmp(temp, ACTIVE) == 0){	
		if(if_index < 0 || if_index > PVC_NUM * MAX_SMUX_NUM)
			return FAIL;
		
		top =   get_node_top();

		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0], "Wan");
		sprintf(nodeName[1], "PVC%d", if_index/MAX_SMUX_NUM);
		sprintf(nodeName[2], "Entry%d", if_index%MAX_SMUX_NUM);

		if ( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "IPVERSION", str_ipversion) )
			strcpy(str_ipversion, "IPv4");

		if ( 0 == strcmp(str_ipversion,"IPv4") )
			*ipMode= 1;
		else
		{
#if defined(TCSUPPORT_CT_DSLITE)
			if ( 1 == get_dslite_stateviacfg(top, nodeName) )
				*ipMode = 4;
			else
#endif
			{
				if ( 0 == strcmp(str_ipversion,"IPv6") )
					*ipMode= 2;
				else
					*ipMode= 3;
			}
		}

		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0],WAN_IF_NODE);
		sprintf(nodeName[1], "Entry%d", if_index);	
		memset(temp, 0, sizeof(temp));
		if(getAttrValue(top, nodeName, "GateWay", temp) == TCAPI_PROCESS_OK){
			if(strlen(temp) != 0 && strcmp(temp,"N/A")){
					return SUCCESS;
			}
		}
		if(getAttrValue(top, nodeName, "GateWay6", temp) == TCAPI_PROCESS_OK){
			if(strlen(temp) != 0 && strcmp(temp,"N/A")){
					return SUCCESS;
			}
		}

	}
	
	return FAIL;	
}

/*_____________________________________________________________________________
**      function name: diagnostic_execute
**      descriptions:
**            To diagnostic the current PVC connection status.
**
**      parameters:
**            top:   Put the diagnostic of cfg node tree that is used to execute system information.
**            name: Put the diagnostic cfg node name.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**      getAttrValue
**      fileRead
**      test_atm_oam_ping
**      getWanInfo
**      mxmlElementSetAttr
**
**
**      revision:
**      1. Here 2008/5/19
**____________________________________________________________________________
*/
int
diagnostic_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){

	char diagnostic_attr[][20]=
	{
		{"EtherLanConn"},
#if defined(TCSUPPORT_WAN_GPON) || defined(TCSUPPORT_WAN_EPON)
		{"xPONLinkSta"},
		{"xPONAppLinkSta"},
#endif
		{"PingPriDNS"},{"PingYahoo"},
		{"PingOther"},
#if defined(TCSUPPORT_CT_E8GUI)
		{"TraceResult"},
#endif
		{""}  /*rodney_20090506 added*/	
	};

	enum diagnostic_en{
		EthLanConn=0,
#if defined(TCSUPPORT_WAN_GPON) || defined(TCSUPPORT_WAN_EPON)
		XPONLinkStatus,
		XPONAppLinkStatus,
#endif

		PingPriDNS,
		PingYahoo,
		PingOther,  /*rodney_20090506 added*/
#if defined(TCSUPPORT_CT_E8GUI)
		TraceResult,
#endif
		Max_diag,
	};
	mxml_node_t *curNode = NULL;
	char nodeName[3][MAX_NODE_NAME];
	char buf[64]={0};
	char *tmpBuf=NULL;
	char dev[8]={0};
	//char pingCMD[128]={0};
	char diag_value[Max_diag][8];
	int i=0;
	char pvc[4]={0};
	int flagRet = 0;
	char cmdbuf[128] = {0};
	int ipVersion = 1;
#if defined(TCSUPPORT_CT_E8GUI)
	char buf_times[32]={0};
	char buf_route[32]={0};
	//char traceCMD[64]={0};
	//char addrouteCMD[128]={0};
	//struct hostent *retval;
	struct sockaddr_in cvtaddr;
	struct sockaddr_in *cvtaddr2  = NULL;
	//struct in_addr addr;
	char convertIP[64]={0};
	char filebuf[512]={0};
	char filler1[16]={0},filler2[16]={0},filler3[16]={0},filler4[16]={0},filler5[16]={0},
	     filler6[16]={0},filler7[16]={0},filler8[16]={0},filler9[16]={0};
	struct sockaddr_in6 ipv6addr;
	struct sockaddr_in6 *ipv6addr2 = NULL;
	struct addrinfo hints;
	struct addrinfo *res = NULL;
#if defined(TCSUPPORT_CT_PON_GDV20)
	struct addrinfo hintsv6;
	struct addrinfo *resv6 = NULL;	
	char convertIPv6[64]={0};
#endif
#endif

	
	/*Init Test value*/
	for(i=0; strlen(diagnostic_attr[i])!=0; i++){
		strcpy(diag_value[i], TEST_FAIL);
	}

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], WEBCURSET);
	sprintf(nodeName[1], SUB_NODE_NAME);
	/*Get the pvc value that is user try to test*/
	getAttrValue(top, nodeName, "diag_pvc", pvc);

	curNode=get_web_node(top, name);

	/*EtherLanConn*/
	fileRead(ETHLINK_PATH, buf, sizeof(buf));
	if((strlen(buf) > 0) && (!strstr(buf,"Down"))){
		strcpy(diag_value[EthLanConn],TEST_OK);
	}

#if defined(TCSUPPORT_WAN_GPON) || defined(TCSUPPORT_WAN_EPON)
	// get xpon driver mpcp/ploam status
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], XPON_NODE);
	sprintf(nodeName[1], PON_LINK_CFG);
	if (getAttrValue(top, nodeName, PONCOMMON_ATTR_XPON_LINK_STA, buf) == TCAPI_PROCESS_OK){
		if (atoi(buf) == XPON_LINK_GPON || atoi(buf) == XPON_LINK_EPON){ //GPON link || EPON link
			strcpy(diag_value[XPONLinkStatus], TEST_OK);
		}
	}

	// get xpon app oam/omci status
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], XPON_NODE);
	sprintf(nodeName[1], PONCOMMON);
	if (getAttrValue(top, nodeName, PONCOMMON_ATTR_DATA_TRAFFIC_STA, buf) == TCAPI_PROCESS_OK){
		if (strcmp(buf, ONU_WAN_DATA_LINK_UP) == 0){ // traffic link status
			strcpy(diag_value[XPONAppLinkStatus], TEST_OK);
		}
	}
	if (strcmp(buf, ONU_WAN_DATA_LINK_UP) == 0) {
#endif

		memset(nodeName,0,sizeof(nodeName));
		strcpy(nodeName[0], WAN);
		strcpy(nodeName[1], name[1]);/*PVC0~PVC7*/
		getAttrValue(top, nodeName, WAN_ISP, buf);
		if(atoi(buf) == 2){
			/*PPPOE/PPPOA Mode*/
			sprintf(dev,PPP_IF, atoi(pvc));
		}else{
			sprintf(dev,NAS_IF, atoi(pvc));
		}


#if defined(TCSUPPORT_CT_E8GUI)
		memset(buf,0,sizeof(buf));
		memset(nodeName,0,sizeof(nodeName));
		strcpy(nodeName[0],DIAGNOSTIC);
		strcpy(nodeName[1],name[1]);
		
		if(getAttrValue(top, nodeName, "TestType", buf) == TCAPI_PROCESS_OK)
		{
			/*do ping ip test*/
#if defined(TCSUPPORT_CT_PON_GD)
			if((strcmp(buf,"2") == 0) || (strcmp(buf,"1") == 0))
#else
	  		if(strcmp(buf,"2") == 0)
#endif
			{
#if defined(TCSUPPORT_CT_PON_GD)
				if (strcmp(buf,"1") == 0)
				{
					gFlagBandRegDiag = 1;
				}
				else
			{
					gFlagBandRegDiag = 0;
				}
#endif
				memset(buf,0,sizeof(buf));
				memset(buf_route,0,sizeof(buf_route));
				memset(buf_times,0,sizeof(buf_times));
				strcpy(diag_value[PingOther],TEST_FAIL);
				
				if(getAttrValue(top, nodeName, "PingOtherType", buf) == TCAPI_PROCESS_OK){
					if(strcmp(buf,"Yes") == 0){
						if(getAttrValue(top, nodeName, "PingOtherIPaddr", buf) 
						== TCAPI_PROCESS_OK && 
						getAttrValue(top, nodeName, "PingOtherTimes", buf_times) 
						== TCAPI_PROCESS_OK && 
						getAttrValue(top, nodeName, "AddRoute", buf_route) 
						== TCAPI_PROCESS_OK)
						{							
							char cmd[64] = {0};
							//delete old ping file
							unlink(TMP_PING_OTHER_PATH);
							if(checkWan_by_name(buf_route, &ipVersion) == FAIL)
							{
								flagRet = 1;
								goto save_result;
							}

#if 1
							sprintf(cmd, "/bin/echo \"%s %s\" > %s", buf, buf_route, TMP_DNS_DIAG_PATH);
							//tcdbg_printf("%s\n", cmd);
							system(cmd);
							//sleep(3);
#endif
							if(inet_aton(buf,&cvtaddr.sin_addr))
							{	
								//ping diagnostic address is ipv4
								strcpy(convertIP,buf);
								pingDiag(convertIP,buf_route,buf_times, ipVersion);
							}
							else if( inet_pton(AF_INET6, buf,(struct sockaddr *) &ipv6addr.sin6_addr) >0)
							{	
								//ping diagnostic address is ipv6
								strcpy(convertIP,buf);
								ping6Diag(top,convertIP,buf_route,buf_times);
							}
							else{
								//ping diagnostic address is a domain name
								memset( &hints, 0 , sizeof(hints));
								hints.ai_family = ( 2 == ipVersion ) ? AF_INET6 : AF_INET;
								hints.ai_socktype = SOCK_DGRAM;  		//DNS is UDP packet
								hints.ai_protocol = IPPROTO_UDP;
#if defined(TCSUPPORT_CT_PON_GDV20)
								memset( &hintsv6, 0 , sizeof(hintsv6));
								hintsv6.ai_family = AF_INET6;
								hintsv6.ai_socktype = SOCK_DGRAM;		//DNS is UDP packet
								hintsv6.ai_protocol = IPPROTO_UDP;	
								memset(convertIPv6,0,sizeof(convertIPv6));
#endif

								if ( 1 == ipVersion
									|| 2 == ipVersion ) // ipv4 only or ipv6 only
								{
									if ( tc_getaddrinfo(buf, NULL, &hints, &res, 5) )
									{
										flagRet = 1;
										goto save_result;
									}
								}
								else // ipv4/ipv6 or dslite+ipv6
								{
									if ( tc_getaddrinfo(buf, NULL, &hints, &res, 5) )
									{
										hints.ai_family = AF_INET6;
										if( tc_getaddrinfo(buf, NULL, &hints, &res, 5) )
										{
											flagRet = 1;
											goto save_result;
										}
									}
#if defined(TCSUPPORT_CT_PON_GDV20)
									else
									{
										if( tc_getaddrinfo(buf, NULL, &hintsv6, &resv6, 5) == 0 )
										{
											ipv6addr2 = (struct sockaddr_in6 *)resv6->ai_addr;
											memset(buf, 0, sizeof(buf));
											inet_ntop(AF_INET6,&ipv6addr2->sin6_addr, buf, INET6_ADDRSTRLEN);
											strcpy(convertIPv6, buf);
										}
									}
#endif
								}

								switch (res->ai_family)
								{
									case AF_INET:			//ipv4
										cvtaddr2 = (struct sockaddr_in *) res->ai_addr;
										strcpy(convertIP,inet_ntoa(cvtaddr2->sin_addr));
										pingDiag(convertIP,buf_route,buf_times, ipVersion);
										break;	
									case AF_INET6:			//ipv6
										ipv6addr2 = (struct sockaddr_in6 *)res->ai_addr;
										inet_ntop(AF_INET6,&ipv6addr2->sin6_addr, buf, INET6_ADDRSTRLEN);
										strcpy(convertIP,buf);
										ping6Diag(top,convertIP,buf_route,buf_times);
										break;
									default:	
										flagRet = 1;
										goto save_result;
										
								}
							}
#if defined(TCSUPPORT_CT_PON_GDV20)
						while(1){
#endif
							
							tmpBuf=malloc(MAX_BUF_SIZE);
							if(tmpBuf == NULL){
								//return FAIL;
								flagRet = 1;
								goto save_result;
							}
							//judge test result
							memset(filebuf,'\0',BUF_SIZE_512);
							FILE *fp = NULL;
							fp=fopen(TMP_PING_OTHER_PATH, "r");
							if(fp == NULL)
							{
								//return FAIL;
								flagRet = 1;
								goto save_result;
							}
							while (fgets(filebuf, BUF_SIZE_512, fp))
							{
								if(!strstr(filebuf,RECVICMPPKT))
								{
									continue;
								}
								sscanf(filebuf, "%s %s %s %s %s %s %s %s %s",filler1, filler2, filler3, filler4,filler5,filler6,filler7,
								filler8,filler9);
								if(strcmp(filler4,"0"))
								{
									strcpy(diag_value[PingOther],TEST_OK);
								}
								
							}
							fclose(fp);
								
							free(tmpBuf);
#if defined(TCSUPPORT_CT_PON_GDV20)
							if(strcmp(filler4,"0"))
							{
								break;
							}
							else if(strlen(convertIPv6))
							{
								ping6Diag(top,convertIPv6,buf_route,buf_times);
								memset(convertIPv6,0,sizeof(convertIPv6));
							}
							else
								break;
						}
#endif
						}
					}
					else{
						strcpy(diag_value[PingOther],TEST_SKIP);
					}
				}
			}
		}

		memset(buf,0,sizeof(buf));
		memset(buf_route,0,sizeof(buf_route));
		if(getAttrValue(top, nodeName, "TestType", buf) == TCAPI_PROCESS_OK)
		{
			/*do tracert ip test*/
	  		if(strcmp(buf,"3") == 0)
			{  	
				memset(buf,0,sizeof(buf));
				strcpy(diag_value[TraceResult],TEST_FAIL);
				
				if(getAttrValue(top, nodeName, "TraceIPaddr", buf) 
				== TCAPI_PROCESS_OK &&
				   getAttrValue(top, nodeName, "AddRoute", buf_route) 
				== TCAPI_PROCESS_OK)
				{
					//delete old traceroute file
					unlink(TMP_TRACE_PATH);
					if(checkWan_by_name(buf_route, &ipVersion) == FAIL)
					{
						flagRet = 1;
						goto save_result;
					}

					sprintf(cmdbuf, "/bin/echo \"%s %s\" > %s", buf, buf_route, TMP_DNS_DIAG_PATH);
					system(cmdbuf);

					if(inet_aton(buf,&cvtaddr.sin_addr))
					{	
						//traceroute diagnostic address is ipv4
						strcpy(convertIP,buf);
						tracerouteDiag(convertIP,buf_route, ipVersion);
					}
					else if( inet_pton(AF_INET6, buf,(struct sockaddr *) &ipv6addr.sin6_addr) >0)
					{	
						//traceroute diagnostic address is ipv6
						strcpy(convertIP,buf);
						traceroute6Diag(top,convertIP,buf_route);
					}
					else{
						//ping diagnostic address is a domain name
						memset( &hints, 0 , sizeof(hints));
						hints.ai_family = ( 2 == ipVersion ) ? AF_INET6 : AF_INET;
						hints.ai_socktype = SOCK_DGRAM;  		//DNS is UDP packet
						hints.ai_protocol = IPPROTO_UDP;

						if ( 1 == ipVersion
							|| 2 == ipVersion ) // ipv4 only or ipv6 only
						{
							if ( tc_getaddrinfo(buf, NULL, &hints, &res, 5) )
							{
								flagRet = 1;
								goto save_result;
							}
						}
						else // ipv4/ipv6 or dslite+ipv6
						{
							if ( tc_getaddrinfo(buf, NULL, &hints, &res, 5) )
							{
								hints.ai_family = AF_INET6;
								if( tc_getaddrinfo(buf, NULL, &hints, &res, 5) )
								{
									flagRet = 1;
									goto save_result;
								}
							}
						}

						switch (res->ai_family)
						{
							case AF_INET:			//ipv4
								cvtaddr2 = (struct sockaddr_in *) res->ai_addr;
								strcpy(convertIP,inet_ntoa(cvtaddr2->sin_addr));
								tracerouteDiag(convertIP,buf_route, ipVersion);
								break;	
							case AF_INET6:			//ipv6
								ipv6addr2 = (struct sockaddr_in6 *)res->ai_addr;
								inet_ntop(AF_INET6,&ipv6addr2->sin6_addr, buf, INET6_ADDRSTRLEN);
								strcpy(convertIP,buf);
								traceroute6Diag(top,convertIP,buf_route);
								break;
							default:	
								flagRet = 1;
								goto save_result;
								
						}
					}
					tmpBuf=malloc(MAX_BUF_SIZE);
					if(tmpBuf == NULL){
						//return FAIL;
						flagRet = 1;
						goto save_result;
					}
					//judge test result
					memset(tmpBuf,'\0',MAX_BUF_SIZE);
					fileRead(TMP_TRACE_PATH, tmpBuf, MAX_BUF_SIZE);
					
					if(strcmp(tmpBuf,"")){
						strcpy(diag_value[TraceResult],TEST_OK);
					}
					free(tmpBuf);
				}
			}
		}
#else
		/*rodney_20090506 modified*/
		/*ping DHCP Primary DNS*/
	   	memset(buf,0,sizeof(buf));
	  	getWanInfo(Pridns_type, dev, buf);
	    sprintf(pingCMD,PING_CMD, buf, TMP_PING_DNS_PATH);
	    system(pingCMD);
	   	tmpBuf=malloc(MAX_BUF_SIZE);
	   	if(tmpBuf == NULL){
	  		//return FAIL;
	  		flagRet = 1;
			goto save_result;
	 	}
    	fileRead(TMP_PING_DNS_PATH, tmpBuf, MAX_BUF_SIZE);
		if(strstr(tmpBuf, RECV1ICMPPKT)){
			strcpy(diag_value[PingPriDNS],TEST_OK);
   		}
   		unlink(TMP_PING_DNS_PATH);

  		sprintf(pingCMD,PING_CMD, YAHOO_DNS, TMP_PING_YAH_PATH);  /*ping yahoo*/
    	system(pingCMD);
    	fileRead(TMP_PING_YAH_PATH, tmpBuf, MAX_BUF_SIZE);
    	if(strstr(tmpBuf, RECV1ICMPPKT)){
    		strcpy(diag_value[PingYahoo],TEST_OK);
    	}
    	unlink(TMP_PING_YAH_PATH);
	 	free(tmpBuf);

		/*do ping other ip test*/
		memset(nodeName,0,sizeof(nodeName));
	  	strcpy(nodeName[0],DIAGNOSTIC);
	  	strcpy(nodeName[1],name[1]);
	  	memset(buf,0,sizeof(buf));
	  	strcpy(diag_value[PingOther],TEST_FAIL);
	  	if(getAttrValue(top, nodeName, "PingOtherType", buf) == TCAPI_PROCESS_OK){
	  		if(strcmp(buf,"Yes") == 0){  /*user want to ping other IP*/
	  			if(getAttrValue(top, nodeName, "PingOtherIPaddr", buf) == TCAPI_PROCESS_OK){
	  				sprintf(pingCMD,PING_CMD, buf, TMP_PING_OTHER_PATH);
	  				system(pingCMD);
	  				tmpBuf=malloc(MAX_BUF_SIZE);
	  				if(tmpBuf == NULL){
	  					//return FAIL;
	  					flagRet = 1;
						goto save_result;
	  				}
	  				fileRead(TMP_PING_OTHER_PATH, tmpBuf, MAX_BUF_SIZE);
	  				if(strstr(tmpBuf, RECV1ICMPPKT)){
	  					strcpy(diag_value[PingOther],TEST_OK);
	  				}
	  				free(tmpBuf);
	  				unlink(TMP_PING_OTHER_PATH);
	  			}
	  		}
	  		else{
	  			strcpy(diag_value[PingOther],TEST_SKIP);
	  		}
		}
#endif
	}
save_result:
	//unlink the tmp diagdns.conf file created
		unlink(TMP_DNS_DIAG_PATH);
	
	/*Fill the test result  into diagnostic */
	for(i=0;strlen(diagnostic_attr[i]) != 0;i++){
		mxmlElementSetAttr(curNode, diagnostic_attr[i], diag_value[i]);
	}

	if(flagRet == 1)
		return FAIL;
	
	return SUCCESS;
}/*end diagnostic_execute*/

void F5LoopbacktestResult(char *path,char *result)
{
	FILE  *fp = NULL;
	int index = 0;
	char buf[128] = {0};
	char *pbegin = NULL;
	char *pend = NULL;

	fp = fopen(path,"r");
	if(fp  == NULL){
		return;
	}

	while (fgets(buf, sizeof(buf), fp)) 
	{
		if((pbegin = strstr(buf,"SuccessCount:")) == NULL)
			continue;
		pend = strstr(buf," ");
		index = pend - pbegin -strlen("SuccessCount:");
		strncpy(result,pbegin+strlen("SuccessCount:"),index);
	}
	
	fclose(fp);
	return;
}
/*_____________________________________________________________________________
**      function name: test_atm_oam_ping
**      descriptions:
**           Test the ATM OAM PING ability.
**
**      parameters:
**            tree:  Put the top of cfg node tree.
**            name: PVC0~PVC7
**            type: 0:ping ATM OAM Segment , 1:ping ATM OAM end to end
**            buf: Specify the buffer that will be get test result.
**
**      global:
**             None
**
**      return:
**             None
**
**      call:
**   	 getAttrValue
**   	 fileRead
**
**      revision:
**      1. Here 2008/5/7
**____________________________________________________________________________
*/
void
test_atm_oam_ping(mxml_node_t *tree,char *name,int type, char *buf){
	char arg[32]={0};
	#ifdef CMD_API
	char value[10]={0};
	#else
	char value[4]={0};
	#endif
	char ping_cmd[64]={0};
	char wanNodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char wanInfo[3][8];/*0:Active status, 1:VPI,2:VCI*/
	//char wanAttr[][8]={NODE_ACTIVE,"VPI","VCI",""};
	int i=0;
	char ping_para_keywd[][32]=
	{
		{"PVCACTIVE"},
		{"VPI"},
		{"VCI"},
		{""}
	};

	memset(wanInfo,0,sizeof(wanInfo));
	memset(wanNodeName, 0, sizeof(wanNodeName));

	strcpy(wanNodeName[0], WAN);
	strcpy(wanNodeName[1], name);

#if 0
	for(i=0;strlen(wanAttr[i])!=0; i++){
		getAttrValue(tree, wanNodeName, wanAttr[i], wanInfo[i]);
	}
#else
	for(i=0;strlen(ping_para_keywd[i])!=0; i++){
		getAttrValue(tree, wanNodeName, ping_para_keywd[i], wanInfo[i]);
	}
#endif

	if(strcmp( wanInfo[0], ACTIVE) ==0){
		/*This PVC status is actived by user setting.*/
		#ifdef CMD_API
		memset(arg, 0, sizeof(arg));
		/*usage: <vpi> <vci> <f5> <end-to-end> <type:0(AIS) 1(RDI) 2(Loopback)><timeout>*/
		sprintf(arg,"%s %s 1 %d 2 500",wanInfo[1], wanInfo[2], type);
		sprintf(ping_cmd,"echo %s > %s",arg, OAM_PING_PATH);
		system(ping_cmd);
		/*Need to wait response*/
		sleep(1);
		F5LoopbacktestResult(OAM_PING_PATH,value);
		if(atoi(value) != 0)
		{
			strcpy(buf, TEST_OK);
			return;
		}
		#else
		memset(arg, 0, sizeof(arg));
		/*usage: <vpi> <vci> <f5> <end-to-end> <type:0(AIS) 1(RDI) 2(Loopback)>*/
		sprintf(arg,"%s %s 1 %d 2",wanInfo[1], wanInfo[2], type);
		sprintf(ping_cmd,"echo %s > %s",arg, OAM_PING_PATH);
		system(ping_cmd);
		/*Need to wait response*/
		sleep(1);
		fileRead(OAM_PING_PATH,value, sizeof(value));
		if(strncmp(value, CONNECTED,1) == 0){
			strcpy(buf, TEST_OK);
			return;
		}
		#endif
	}
	strcpy(buf, TEST_FAIL);
}/*end test_atm_oam_ping*/

//"DeviceInfo",	UPDATE_TYPE|WEB_TYPE|PVC_TYPE,	NULL, dev_merge_table, deviceInfo_read, NULL, NULL,  deviceInfo_execute},
char *
dev_merge_table[MAX_NODE_NAME]=
{
	"dev_pvc","",
};
/*pork 20090309 added*/
/*______________________________________________________________________________
**	sysInfo_init
**
**	descriptions:
**		the init function of sysInfo.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int sysInfo_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name=SYS_INFO_NODE_NAME,
			.type=SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=sysInfo_write,
			.cfg_verify=NULL,
			.cfg_execute=sysInfo_execute,
			.cfg_boot=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}
/*______________________________________________________________________________
**	sysInfo_write
**
**	descriptions:
**		the write function of sysInfo.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int sysInfo_write(mxml_node_t *top,mxml_node_t *parant){
	mxml_node_t *node = NULL;
	time_t sysStarttime = time((time_t *)NULL);
	char tmpSysTime[20] = {0};
	node = mxmlFindElement(parant, parant, SUB_NODE_NAME,NULL, NULL, MXML_DESCEND);
	if(node != NULL){
		sprintf(tmpSysTime,"%d",(int)sysStarttime);
		mxmlElementSetAttr(node,SYSTEMSTARTUPTIME,tmpSysTime);
		return SUCCESS;
	}
	else{
		return FAIL;
	}
}
int sysInfo_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	char attrName[][MAX_VALUE_SIZE] = {{"etherPktsClearSet"},{"adslPktsClearSet"},{"usbPktsClearSet"},{""}};
	char procLocation[][MAX_VALUE_SIZE] = {{"/proc/tc3162/eth_pktsclear"},{"/proc/tc3162/tsarm_pktsclear"},{"/proc/tc3162/usb_link_state_clear"},{""}};
	char cmd[32];
	tcapi_msg_t msg;
	int i;
	memset(&msg,0,sizeof(tcapi_msg_t));
	sprintf(msg.node,"%s_%s",SYS_INFO_NODE_NAME,SUB_NODE_NAME);
	for(i = 0;strlen(attrName[i])!=0;i++){
		memset(msg.attr,0,sizeof(msg.attr));
		memset(msg.value,0,sizeof(msg.value));
		strcpy(msg.attr,attrName[i]);
		tcapi_get_req(top,&msg);
		if(msg.retval != TCAPI_PROCESS_OK){
			/*tcdbg_printf("A \n");*/
			continue;
		}
		else{
			if(!strcmp(msg.value,ACTIVE)){
				memset(cmd,0,sizeof(cmd));
				sprintf(cmd,"echo 1 > %s",procLocation[i]);
				system(cmd);
			}
			strcpy(msg.value,DEACTIVE);
			tcapi_set_req(top, &msg);
		}
	}
	return SUCCESS;
}
extern int autopvc_state;
/*_____________________________________________________________________________
**      function name: deviceParaDynamic_read
**      descriptions:
**            To get default config value of cpe. 
**		if define compilt option "TCSUPPORT_PRODUCTIONLINE":
**			there information store in reservearea,if reservearea is not configured,
**			use parameters in cwmp node,if cwmp node is not configured,use default paramters according mac info
**		if not define compilt option "TCSUPPORT_PRODUCTIONLINE":
**			there information store in etc/deviceParaDynamic.conf	
**      parameters:
**            node:   Put the child cfg node of Info.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**      		
**
**      revision:
**      1. Brian 2010/11/11
**____________________________________________________________________________
*/
int
deviceParaDynamic_read(mxml_node_t *top,mxml_node_t *node)
{
	int notUsed = 0;
#ifdef TCSUPPORT_PRODUCTIONLINE
	proline_Para para;
#endif
	unsigned char nm_MAC[7] ;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char buf[64] = {0};

	char deviceParaDynamic_keywd[][32]=
	{
		{"ManufacturerOUI"},
		{"SerialNum"},
		{"ProductClass"},
		{""}
	};

	enum deviceParaDynamic_eu{
		ManufacturerOUI = 0,
		SerialNum,	
		ProductClass,
	};

#ifdef TCSUPPORT_PRODUCTIONLINE
	tcgetProLinePara(&para,0);
	//tcdbg_printf("\r\npara.magic is %x",para.magic);
	//tcdbg_printf("\r\npara.flag is %x",para.flag);
	if(PRODUCTLINEPARAMAGIC == para.magic)
	{
		notUsed = 1;
	}
	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], "Cwmp");
	strcpy(nodeName[1], "Entry");
	
	if(notUsed && (para.flag & PL_CP_MANUFACUREROUI_FLAG))
	{
		//tcdbg_printf("\r\npara.manufacturerOUI1 is %s",para.manufacturerOUI);
		mxmlElementSetAttr(node, deviceParaDynamic_keywd[ManufacturerOUI], para.manufacturerOUI);
	}
	else
	{
		getAttrValue(top, nodeName, "ManufacturerOUI", buf);
		if(buf[0] == '\0')
		{
			GetEtherAddr(nm_MAC);
			sprintf(buf,"%02X%02X%02X",nm_MAC[0],nm_MAC[1],nm_MAC[2]);
		}
		//tcdbg_printf("\r\npara.manufacturerOUI2 is %s",buf);
		mxmlElementSetAttr(node, deviceParaDynamic_keywd[ManufacturerOUI], buf);
	}

	if(notUsed &&(para.flag & PL_CP_SERIALNUM_FLAG))
	{
		//tcdbg_printf("\r\npara.serialnum1 is %s",para.serialnum);
		mxmlElementSetAttr(node, deviceParaDynamic_keywd[SerialNum], para.serialnum);
	}
	else
	{
		getAttrValue(top, nodeName, "SerialNum", buf);
		if(buf[0] == '\0')
		{
			GetEtherAddr(nm_MAC);
			sprintf(buf,"%02X%02X%02X%02X%02X%02X",nm_MAC[0],nm_MAC[1],nm_MAC[2],nm_MAC[3],nm_MAC[4],nm_MAC[5]);
		}
		//tcdbg_printf("\r\npara.serialnum2 is %s",buf);
		mxmlElementSetAttr(node, deviceParaDynamic_keywd[SerialNum], buf);
	}

	if(notUsed &&(para.flag & PL_CP_PRODUCTCLASS_FLAG))
	{
		//tcdbg_printf("\r\npara.productclass1 is %s",para.productclass);
		mxmlElementSetAttr(node, deviceParaDynamic_keywd[ProductClass], para.productclass);
	}
	else
	{
		getAttrValue(top, nodeName, "ProductClass", buf);
		if(buf[0] == '\0')
		{
			GetEtherAddr(nm_MAC);
			sprintf(buf,"%02x%02x%02x",nm_MAC[3],nm_MAC[4],nm_MAC[5]);
		}
		//tcdbg_printf("\r\npara.productclass2 is %s",buf);
		mxmlElementSetAttr(node, deviceParaDynamic_keywd[ProductClass],buf);
	}
#else
	//auto generate
	GetEtherAddr(nm_MAC);
	sprintf(buf,"%02X%02X%02X",nm_MAC[0],nm_MAC[1],nm_MAC[2]);
	mxmlElementSetAttr(node, deviceParaDynamic_keywd[ManufacturerOUI], buf);
	
	GetEtherAddr(nm_MAC);
	sprintf(buf,"%02X%02X%02X-%02X%02X%02X%02X%02X%02X",nm_MAC[0],nm_MAC[1],nm_MAC[2],nm_MAC[0],nm_MAC[1],nm_MAC[2],nm_MAC[3],nm_MAC[4],nm_MAC[5]);
	mxmlElementSetAttr(node, deviceParaDynamic_keywd[SerialNum], buf);
	
	GetEtherAddr(nm_MAC);
	sprintf(buf,"%02x%02x%02x",nm_MAC[3],nm_MAC[4],nm_MAC[5]);
	mxmlElementSetAttr(node, deviceParaDynamic_keywd[ProductClass],buf);
#endif
	return SUCCESS;
}

/*_____________________________________________________________________________
**      function name: deviceParaDynamic_read
**      descriptions:
**            To get default config value of cpe. 
**		if define compilt option "TCSUPPORT_PRODUCTIONLINE":
**			there information store in reservearea,if reservearea is not configured,
**			use parameters in cwmp node,if cwmp node is not configured,use default paramters according mac info
**		if not define compilt option "TCSUPPORT_PRODUCTIONLINE":
**			there information store in etc/deviceParaDynamic.conf	
**      parameters:
**            node:   Put the child cfg node of Info.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**      		
**
**      revision:
**      1. Brian 2010/11/11
**____________________________________________________________________________
*/
int
deviceParaStatic_read(mxml_node_t *top,mxml_node_t *node)
{
	char attrName[32] = {0};
	char attrVal[128] = {0};
	int i, j, ret;

	char deviceParaStatic_keywd[][32]=
	{
		{"Manufacturer"},{"ModelName"},
		{"CustomerSWVersion"},{"CustomerHWVersion"},
#if defined(TCSUPPORT_CT_E8GUI)
		/*{"DvID"},*/{"BatCode"},
#endif
#if defined(TCSUPPORT_CT_FJ)
		{"DevDescribtion"},
#endif
		{"Lanports"},
		{""}
	};

#if defined(TCSUPPORT_PRODUCTIONLINE) && ((defined(TCSUPPORT_CT_PON) && defined(TCSUPPORT_RESERVEAREA_EXTEND)) || (defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505)))
	int notUsed = 0;
	proline_Para para;
	tcgetProLinePara(&para,0);
	if(PRODUCTLINEPARAMAGIC == para.magic)
	{
		notUsed = 1;
	}
#endif

	for ( i = 0; strlen(deviceParaStatic_keywd[i]) != 0; i++ ) 
	{	
		memset( attrName, 0, sizeof(attrName) );
		memset( attrVal, 0, sizeof(attrVal) );
		snprintf( attrName, sizeof(attrName), "%s=", deviceParaStatic_keywd[i] );
		//ret = get_profile_str( attrName, attrVal, sizeof(attrVal), NO_QMARKS, DEVICEPARASTATIC_PATH);
		ret = get_profile_str_new( attrName, attrVal, sizeof(attrVal), DEVICEPARASTATIC_PATH);
		if ( ret != -1 )
		{
#if defined(TCSUPPORT_PRODUCTIONLINE) && ((defined(TCSUPPORT_CT_PON) && defined(TCSUPPORT_RESERVEAREA_EXTEND)) || (defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505)))
			if (!strcmp(deviceParaStatic_keywd[i], "CustomerHWVersion")) {
				//tcdbg_printf("\r\npara.hwver = %s, notUsed = %d, para.flag = %x", para.hwver, notUsed, para.flag);
				if(notUsed && (para.flag & PL_CP_HW_VER_FLAG)) {
					strncpy(attrVal, para.hwver, sizeof(attrVal)-1);
				}
			}
			else if (!strcmp(deviceParaStatic_keywd[i], "ModelName")) {
				if(notUsed && (para.flag & PL_CP_PRODUCTCLASS_FLAG)) {
					strncpy(attrVal, para.productclass, sizeof(attrVal)-1);
				}
			}
#endif
#if defined(TCSUPPORT_CT_MIDWARE)
			if(i == 2 || i == 3){
				for ( j = 0; j < strlen(attrVal); j ++ )
				{
					if ( ' ' == attrVal[j] )
						attrVal[j] = '_';
				}
			}
#endif
			mxmlElementSetAttr(node, deviceParaStatic_keywd[i], attrVal);
		}
	}
	return SUCCESS;
}
/*pork 20090309 added over*/
/*______________________________________________________________________________
**	deviceInfo_init
**
**	descriptions:
**		the init function of deviceInfo.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
deviceInfo_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="DeviceInfo",
			.type=UPDATE_TYPE|WEB_TYPE|PVC_TYPE,
			.adv_table=NULL,
			.merge_table=dev_merge_table,
			.cfg_read=deviceInfo_read,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=deviceInfo_execute,
			.cfg_boot=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}
/*_____________________________________________________________________________
**      function name: deviceInfo_read
**      descriptions:
**           To create DeviceInfo cfg node.
**
**      parameters:
**            top:  Put the top of cfg node tree.
**            name: The name of deviceInfo cfg node.
**
**      global:
**             None
**
**      return:
**             None
**
**      call:
**   	 mxmlFindElement
**   	 mxmlNewElement
**   	 fileRead
**   	 mxmlElementSetAttr
**   	 get_web_node
**   	 getDefGW
**   	 getWanInfo
**
**      revision:
**      1. Here 2008/5/7
**	2. krammer 2008/10/06
**	3. Renyu 2009/12/31
**____________________________________________________________________________
*/
int
deviceInfo_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr){

	enum dev_pvc_info_en{
		Status=0,
		PppConnTime,
		DispConnBtnType,
		DispPPPconn,
		DispBtnType,
		WanIP,
		WanSubMask,
		DNSIP,
		SECDNSIP, //xyzhu_20091225
		WanDefGW,
		connType,
		Gateway, //gateway info for none default route PVC
	};
	char dev_pvc_attr[][16]=
	{
		{"Status"},{"pppConnTime"},{"DispConnBtnType"},
		{"DispPPPconn"},{"DispBtnType"},
		{"WanIP"},{"WanSubMask"},
		{"DNSIP"},{"SECDNSIP"},/*support second dns server.*/
		{"WanDefGW"},{"connType"},
		{"gateway"},
		{""},
	};
	mxml_node_t *parentNode = NULL;
	mxml_node_t *curNode = NULL;
	mxml_node_t *curDevParaNode = NULL;
	char fwVer[32]={0};
#if defined(TCSUPPORT_CT_PROLINE_SUPPORT) || defined(TCSUPPORT_CT_PON_CZ_GD)
	char compileTime[64] = {0};
#endif
#if defined(TCSUPPORT_CT_E8GUI)
	#if 0
	char dvId[32]={0};
	char hdVer[32]={0};
	char sdVer[32]={0};
	char batCode[32]={0};
	char modelName[32]={0};
	#endif
	char gateWayName[32]={0};
	char DeviceInfoTemp[32]={0};
#endif
	char adsl_stat[32]={0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];  /*rodney_20090507 modified wanNodeName to nodeName for common use*/
	char buf[32]={0};
	char pppUptimePath[32]={0};
	char dev[8]={0};
	char server[64]={0};  /*Record the time server*/ /*rodney_20090507 added*/
	char tz_type[4]={0};  /*Record the type*/
	char *tmpBuf=NULL;  /*rodney_20090507 added*/
	char pingCMD[64]={0};  /*rodney_20090507 added*/
	char pvc_stat[4]={0};/*Record the PVC status is active or not*/
	int cur_pvc=0;
	char pvc_info[16]={0};/*use to record temp pvc_info*/
	char isp_conf_path[20]={0};
	int i=0;
	time_t timep;
/*pork 20090309 added*/
	time_t tmpSysStartTime;
	time_t passedTime;
	mxml_node_t *sysInfoNode = NULL;
	const char* tmpTimeStr = NULL;
	char tmpTimeFormatStr[20] = {0};
	int timeFormatIdx = 0;
/*pork 20090309 added over*/
	int now_time,start_time,connect_time;/*for caclated ppp link-up time*/
       int day,hour,min,sec;
	struct tm *pTm;
#if defined(TCSUPPORT_CT_PON)
#if defined(TCSUPPORT_WAN_GPON) || defined(TCSUPPORT_WAN_EPON)
	tcapi_msg_t msg;
#endif	 
#endif
#if defined(TCSUPPORT_CT_PON_CY_JX)
	char ponuptimetemp[32] = {0};
	char ponuptime[32] = {0};
#endif
	#ifdef DEBUG
	fprintf(stderr,"deviceInfo_read func name:%s %s %s\r\n",name[0],name[1],name[2]);
	#endif

#if defined(TCSUPPORT_CT_E8GUI)
#if defined(TCSUPPORT_CT_GUIPSWSHOW)
	memset(nodeName, 0, sizeof(nodeName));	
	strcpy(nodeName[0], WEBCURSET);
	strcpy(nodeName[1], SUB_NODE_NAME);
	setAttrValue(top, nodeName, "IsSupportShowPSW", "1");
#endif
#endif
	
	parentNode = mxmlFindElement(top, top, name[0],
				NULL, NULL, MXML_DESCEND);
	if(parentNode==NULL){
		parentNode =mxmlNewElement(top,  name[0]);
	}

	if(strcmp(name[1], DEVICEPARADYNAMICNAME) == 0) {
		curDevParaNode=get_web_node(top, name);
		if(curDevParaNode==NULL){
			return FAIL;
		}
		deviceParaDynamic_read(top,curDevParaNode);
	}
	else if(strcmp(name[1], DEVICEPARASTATICNAME) == 0) {
		curDevParaNode=get_web_node(top, name);
		if(curDevParaNode==NULL){
			return FAIL;
		}
		deviceParaStatic_read(top,curDevParaNode);
	}
	

	fileRead(FWVER_PATH, fwVer, sizeof(fwVer));
	mxmlElementSetAttr(parentNode, "FwVer", fwVer);
#if defined(TCSUPPORT_CT_PON_CY_JX)
	fileRead("/tmp/pon_uptime.conf", ponuptimetemp, sizeof(ponuptimetemp));
	parsedStr(ponuptimetemp,ponuptime);
	mxmlElementSetAttr(parentNode, "ponUpTime", ponuptime);
	mxmlElementSetAttr(parentNode, "CurTime", (const char *)getUpDateTime());
#endif
#if defined(TCSUPPORT_CT_PROLINE_SUPPORT) || defined(TCSUPPORT_CT_PON_CZ_GD)
	fileRead(COMPILE_TIME_PATH, compileTime, sizeof(compileTime));
	mxmlElementSetAttr(parentNode, "CompileTime", compileTime);
#endif
#if defined(TCSUPPORT_CT_E8GUI)
	#if 0
	fileRead(DVID_PATH, dvId, sizeof(dvId));
	mxmlElementSetAttr(parentNode, "DvID", dvId);
	
	fileRead(HDVER_PATH, hdVer, sizeof(hdVer));
	mxmlElementSetAttr(parentNode, "HdVer", hdVer);
	
	fileRead(SDVER_PATH, sdVer, sizeof(sdVer));
	mxmlElementSetAttr(parentNode, "SdVer", sdVer);
	
	fileRead(BATCODE_PATH, batCode, sizeof(batCode));
	mxmlElementSetAttr(parentNode, "BatCode", batCode);	
	#endif
	fileRead(GATEWAY_PATH, DeviceInfoTemp, sizeof(DeviceInfoTemp));
	parsedStr(DeviceInfoTemp,gateWayName);
	mxmlElementSetAttr(parentNode, "GateWay", gateWayName);
	#if 0
	fileRead(MODEL_PATH, modelName, sizeof(modelName));
	mxmlElementSetAttr(parentNode, "Model", modelName);
	#endif
#endif
	
	now_time=time(&timep);
	if(attr != NULL){ /*rodney_20090507 added*/  /*get the time server address, then ping it to check it exist or not*/
		if(strcmp(attr,"cur_time") == 0){
			memset(nodeName,0,sizeof(nodeName));
			strcpy(nodeName[0], TIMEZONE);
			strcpy(nodeName[1], SUB_NODE_NAME);
			if(getAttrValue(top, nodeName, "TYPE", tz_type) != TCAPI_PROCESS_OK ){
				return FAIL;
			}

			if(atoi(tz_type) == TZ_MANUAL_TYPE){	
				parentNode = mxmlFindElement(top, top, DEVICEINFO,NULL, NULL, MXML_DESCEND);
				if(parentNode==NULL){
		 			parentNode =mxmlNewElement(top,  DEVICEINFO);
				}
				mxmlElementSetAttr(parentNode,"cur_time", ctime(&timep));	
			}else if(atoi(tz_type) == TZ_NTP_TYPE){
#if 0
				memset(nodeName,0,sizeof(nodeName));
				strcpy(nodeName[0], TIMEZONE);
				strcpy(nodeName[1], SUB_NODE_NAME);
			getAttrValue(top, nodeName, TIMEZONE_ATTR_SERVER, server);
			if(strcmp(server,"0.0.0.0")){  /*use the designated time server*/
				sprintf(pingCMD, PING_CMD, server, TMP_PING_TS_PATH);
			}
			else{  /*use the default time server*/
				sprintf(pingCMD, PING_CMD, NTP_DEF_SERVER, TMP_PING_TS_PATH);
			}
			system(pingCMD);
			tmpBuf=malloc(MAX_BUF_SIZE);
			if(tmpBuf == NULL){
				return FAIL;
			}
			fileRead(TMP_PING_TS_PATH, tmpBuf, MAX_BUF_SIZE);
			if(strstr(tmpBuf, RECV1ICMPPKT)){  /*If time server exist, then update the current time*/
				mxmlElementSetAttr(parentNode,"cur_time", ctime(&timep));
			}
			else{  /*If time server don't exist, then display N/A*/
				mxmlElementSetAttr(parentNode,"cur_time", "N/A (Can't find NTP server)");
			}
			free(tmpBuf);
			unlink(TMP_PING_TS_PATH);
#endif
		if(ntp_server_ret == 0){
			tcdbg_printf("get right time\n");
				mxmlElementSetAttr(parentNode,"cur_time", ctime(&timep));
			}
			else{
				tcdbg_printf("ntp failed\n");
				mxmlElementSetAttr(parentNode,"cur_time", "N/A (Can't find NTP server)");
			}
	
		}
	}
		else if( 0 == strcmp(attr, "cur_time2") ) 
		{
			memset(buf, 0, sizeof(buf));
			pTm = gmtime(&timep);
			if ( pTm ) {
			   sprintf(buf, "%d/%d/%d/%d/%d", 1+pTm->tm_mon, pTm->tm_mday, 1900+pTm->tm_year, pTm->tm_hour, pTm->tm_min);
			   mxmlElementSetAttr(parentNode, attr, buf);
			}
		}
	}

	/*pork 20090309 added*/
	/*get system startup time to diff with current time*/
	sysInfoNode = mxmlFindElement(top,top,SYS_INFO_NODE_NAME,NULL, NULL, MXML_DESCEND);
	if(sysInfoNode != NULL){
		sysInfoNode = mxmlFindElement(sysInfoNode,sysInfoNode,SUB_NODE_NAME,NULL, NULL, MXML_DESCEND);
		if(sysInfoNode != NULL){
			tmpTimeStr = mxmlElementGetAttr(sysInfoNode,SYSTEMSTARTUPTIME);
			if(tmpTimeStr != NULL)
			{
				tmpSysStartTime = (time_t)(atoi(tmpTimeStr));
				passedTime = now_time - tmpSysStartTime;
				min = (int) passedTime / 60;
				hour = (min / 60) % 24;
				min %= 60;
				if(hour){
				timeFormatIdx += sprintf(tmpTimeFormatStr+timeFormatIdx,"%2d:%02d",hour, min);
				}
				else{
					if(min){
						timeFormatIdx += sprintf(tmpTimeFormatStr+timeFormatIdx,"00:%02d",min);
					}
					else{
						timeFormatIdx += sprintf(tmpTimeFormatStr+timeFormatIdx,"00:00");
					}
				}
				timeFormatIdx += sprintf(tmpTimeFormatStr+timeFormatIdx,":%d", (int)passedTime % 60);
				/*after getting passed system time from startup, write into deviceInfo node*/
				mxmlElementSetAttr(parentNode,"passedSystime",tmpTimeFormatStr);
			}
			else{
				return FAIL;
			}
		}
		else{
			return FAIL;
		}
	}
	else{
		return FAIL;
	}

	/*pork 20090309 added over*/
	if(strlen(name[1])==0||strcmp(name[1],DEVICEPARADYNAMICNAME) == 0 || strcmp(name[1],DEVICEPARASTATICNAME) == 0){
		/*
			None PVC0~PVC7 information,
			User only try to query FwVer and cur_time
		*/
		return SUCCESS;
	}

	/*
		Must to cleanup sub configure node information,
		because we need to update current attribute-values.
	*/
	del_sub_node(top, name);
	curNode=get_web_node(top, name);
	if(curNode==NULL){
		return FAIL;
	}
	memset(nodeName,0,sizeof(nodeName));  /*rodney_20090507 modified wanNodeName to nodeName for common use*/
	strcpy(nodeName[0], WAN);
	strcpy(nodeName[1], name[1]);
	getAttrValue(top, nodeName, NODE_ACTIVE, pvc_stat);
#if defined(TCSUPPORT_CT_PON)
#if defined(TCSUPPORT_WAN_GPON) || defined(TCSUPPORT_WAN_EPON)
	memset(adsl_stat, 0, sizeof(adsl_stat));
	memset(&msg,0,sizeof(tcapi_msg_t));
	snprintf(msg.node,MAXLEN_NODE_NAME,"XPON_Common");
	memset(msg.attr,0,sizeof(msg.attr));
	memset(msg.value,0,sizeof(msg.value));
	strcpy(msg.attr,"trafficStatus");
	tcapi_get_req(top,&msg);
	if(msg.retval == TCAPI_PROCESS_OK){
		strcpy(adsl_stat,msg.value);
	}
#endif	
#else
	fileRead(ADSL_STATE_PATH, adsl_stat, sizeof(adsl_stat));
#endif


	/*Init device_info from Status to DispBtnType*/
	memset(pvc_info, 0, sizeof(pvc_info));
	for(i=Status; i <= DispBtnType; i++){
		mxmlElementSetAttr(curNode, dev_pvc_attr[i], INIT);
	}

	cur_pvc= atoi (name[1]+strlen("PVC"));
	sprintf(isp_conf_path, ISPCONFIG_PATH, cur_pvc);
	get_profile_str("ISP=",buf, sizeof(buf), QMARKS,isp_conf_path);

	if((strcmp(pvc_stat, ACTIVE)==0) && (strstr(adsl_stat,"up")) ){
		/*
			If the pvc is actived and aslo adsl link is up.
			We are start to filled those configure node information.
		*/
		#if 0
		switch(atoi(buf)){
			case 0:/*Dynamic IP Address*/
				strcpy(pvc_info[DispBtnType],CONNECTED);
				/* FALL THROUGH */
			case 1:/*Static IP Address*/
				sprintf(dev,NAS_IF, cur_pvc);
				getWanInfo(Ip_type,dev, pvc_info[WanIP]);
				if(strlen(pvc_info[WanIP]) > 0){
					strcpy(pvc_info[Status],CONNECTED);
					getDefGW(pvc_info[WanDefGW]);
					getWanInfo(Netmask_type,dev, pvc_info[WanSubMask]);
					getWanInfo(Pridns_type,dev, pvc_info[DNSIP]);
				}
				break;
			case 2:/*PPPoA && PPPoE*/
				/**********20081001 racing add************/
				if(checkPpp(cur_pvc))
				{

				/**********20081001 racing add************/
				sprintf(dev,PPP_IF, cur_pvc);
				sprintf(pppUptimePath,"/tmp/pppuptime-ppp%d",cur_pvc);
				fileRead(pppUptimePath, pvc_info[PppConnTime], sizeof(pvc_info[PppConnTime]));
				/*If this file is exist, it's mean the cpe is dialup PPP Connection.*/
				if(strlen(pvc_info[PppConnTime]) > 0){
					/*Caculate ppp link-up time*/
		                  start_time=atoi(pvc_info[PppConnTime]);
		                  connect_time=now_time-start_time;
		                  day=connect_time/SEC_PER_DAY;
		                  hour=(connect_time-SEC_PER_DAY*day)/SEC_PER_HOUR;
		                  min=(connect_time-SEC_PER_DAY*day-SEC_PER_HOUR*hour)/SEC_PER_MIN;
		                  sec=connect_time-SEC_PER_DAY*day-SEC_PER_HOUR*hour-SEC_PER_MIN*min;
		                  sprintf(pvc_info[PppConnTime],"%d:%2d:%2d:%2d",day,hour,min,sec);
					/*End caculate ppp link-up time*/

					/*If the pppuptime-ppp0~7 file is exist, it's mean the ppp is dial up.*/
					getDefGW(pvc_info[WanDefGW]);
					strcpy(pvc_info[Status],CONNECTED);
					strcpy(pvc_info[DispPPPconn],CONNECTED);
					getWanInfo(Ip_type,dev, pvc_info[WanIP]);
					getWanInfo(Netmask_type,dev, pvc_info[WanSubMask]);
					getWanInfo(Pridns_type,dev, pvc_info[DNSIP]);
				}
				}
				break;
			case 3:/*bridge mode*/
				strcpy(pvc_info[Status], CONNECTED);
				break;
			default:
				break;
		}
		#else
		/*krammer change:
		change the getWanInfo,getDefGW to return a char pointer, by this way, we can use only
		one dimension array and do not need to save the value of attribute twice.
		*/
		switch(atoi(buf)){
#ifndef PURE_BRIDGE
			case WAN_ENCAP_DYN:/*Dynamic IP Address*/
				mxmlElementSetAttr(curNode, dev_pvc_attr[DispBtnType], CONNECTED);
				#ifdef TCSUPPORT_DNSEACHPVC
				sprintf(dev,NAS_IF, cur_pvc);				
				if(strlen(getWanInfo(Ip_type,dev, pvc_info))>0){
					mxmlElementSetAttr(curNode, dev_pvc_attr[WanIP],getWanInfo(Ip_type,dev, pvc_info));
					mxmlElementSetAttr(curNode, dev_pvc_attr[Status], CONNECTED);
					mxmlElementSetAttr(curNode, dev_pvc_attr[WanDefGW],getDefGW(pvc_info));
					mxmlElementSetAttr(curNode, dev_pvc_attr[WanSubMask],getWanInfo(Netmask_type,dev, pvc_info));
					//renyu_20100409 for dns for each pvc
					mxmlElementSetAttr(curNode, dev_pvc_attr[DNSIP], getDNSInfo("Pridns","Dynamic", dev, pvc_info));
					mxmlElementSetAttr(curNode, dev_pvc_attr[SECDNSIP], getDNSInfo("Secdns","Dynamic", dev, pvc_info));

					/*Add for set gateway info*/
					mxmlElementSetAttr(curNode, dev_pvc_attr[Gateway], getPVCGateway(cur_pvc)); 	
				}
				break;
				#endif
				/* FALL THROUGH */
			case WAN_ENCAP_STATIC:/*Static IP Address*/
				sprintf(dev,NAS_IF, cur_pvc);
				if(strlen(getWanInfo(Ip_type,dev, pvc_info))>0){
					mxmlElementSetAttr(curNode, dev_pvc_attr[WanIP],getWanInfo(Ip_type,dev, pvc_info));
					mxmlElementSetAttr(curNode, dev_pvc_attr[Status], CONNECTED);
					mxmlElementSetAttr(curNode, dev_pvc_attr[WanDefGW],getDefGW(pvc_info));
					mxmlElementSetAttr(curNode, dev_pvc_attr[WanSubMask],getWanInfo(Netmask_type,dev, pvc_info));
					#ifndef TCSUPPORT_DNSEACHPVC
					mxmlElementSetAttr(curNode, dev_pvc_attr[DNSIP], getWanInfo(Pridns_type,dev, pvc_info));
					mxmlElementSetAttr(curNode, dev_pvc_attr[SECDNSIP], getWanInfo(Secdns_type,dev, pvc_info)); //xyzhu_20091225
					#else
					//renyu_20100409 for dns for each pvc
					mxmlElementSetAttr(curNode, dev_pvc_attr[DNSIP], getDNSInfo("Pridns","Static", dev, pvc_info));
					mxmlElementSetAttr(curNode, dev_pvc_attr[SECDNSIP], getDNSInfo("Secdns","Static", dev, pvc_info));
					#endif
					/*Add for set gateway info*/
					mxmlElementSetAttr(curNode, dev_pvc_attr[Gateway], getPVCGateway(cur_pvc)); 	
				}
				break;
			case WAN_ENCAP_PPP:/*PPPoA && PPPoE*/
				/**********20081001 racing add************/
				if(checkPpp(cur_pvc))
				{

				/**********20081001 racing add************/
				sprintf(dev,PPP_IF, cur_pvc);
				sprintf(pppUptimePath,"/tmp/pppuptime-ppp%d",cur_pvc);
				fileRead(pppUptimePath, pvc_info, sizeof(pvc_info));
				/*If this file is exist, it's mean the cpe is dialup PPP Connection.*/
				if(strlen(pvc_info) > 0){
					/*Caculate ppp link-up time*/
		                  start_time=atoi(pvc_info);
		                  connect_time=now_time-start_time;
		                  day=connect_time/SEC_PER_DAY;
		                  hour=(connect_time-SEC_PER_DAY*day)/SEC_PER_HOUR;
		                  min=(connect_time-SEC_PER_DAY*day-SEC_PER_HOUR*hour)/SEC_PER_MIN;
		                  sec=connect_time-SEC_PER_DAY*day-SEC_PER_HOUR*hour-SEC_PER_MIN*min;
		                  sprintf(pvc_info,"%d:%2d:%2d:%2d",day,hour,min,sec);
				   mxmlElementSetAttr(curNode, dev_pvc_attr[PppConnTime],pvc_info);
					/*End caculate ppp link-up time*/

					/*If the pppuptime-ppp0~7 file is exist, it's mean the ppp is dial up.*/
					mxmlElementSetAttr(curNode, dev_pvc_attr[WanDefGW],getDefGW(pvc_info));
					mxmlElementSetAttr(curNode, dev_pvc_attr[Status],CONNECTED);
					mxmlElementSetAttr(curNode, dev_pvc_attr[DispPPPconn],CONNECTED);
					mxmlElementSetAttr(curNode, dev_pvc_attr[WanIP],getWanInfo(Ip_type,dev, pvc_info));
					mxmlElementSetAttr(curNode, dev_pvc_attr[WanSubMask],getWanInfo(Netmask_type,dev, pvc_info));
					
					#ifndef TCSUPPORT_DNSEACHPVC
					mxmlElementSetAttr(curNode, dev_pvc_attr[DNSIP],getWanInfo(Pridns_type,dev, pvc_info));
					mxmlElementSetAttr(curNode, dev_pvc_attr[SECDNSIP], getWanInfo(Secdns_type,dev, pvc_info)); //xyzhu_20091225
					#else
					//renyu_20100409 for dns for each pvc
					mxmlElementSetAttr(curNode, dev_pvc_attr[DNSIP], getDNSInfo("Pridns","PPP", dev, pvc_info));
					mxmlElementSetAttr(curNode, dev_pvc_attr[SECDNSIP], getDNSInfo("Secdns","PPP", dev, pvc_info));
					#endif

					/*Add for set gateway info*/
					mxmlElementSetAttr(curNode, dev_pvc_attr[Gateway], getPVCGateway(cur_pvc)); 	

				}
				}
				break;
#endif
			case WAN_ENCAP_BRIDGE:/*bridge mode*/
				mxmlElementSetAttr(curNode, dev_pvc_attr[Status],CONNECTED);
				break;
			default:
				break;
		}
		#endif
	}


	/*2008 10 03 krammer add
	the following is to get the ISP type by reading /etc/isp(cur_pvc).conf
	if the ISP type is PPPoA or PPPoE, get the ENCAP value to decide the
	type is PPPoA or PPPoE. This connection type will show on web page
	even if pvc dosen't be actived and adsl isn't up, so I use other side to
	deside connection type instead of putting it into upper switch.
	*/
	switch(atoi(buf)){
#ifndef PURE_BRIDGE
			case WAN_ENCAP_DYN:/*Dynamic IP Address*/
				mxmlElementSetAttr(curNode, dev_pvc_attr[connType],"Dynamic IP");
				break;
			case WAN_ENCAP_STATIC:/*Static IP Address*/
				mxmlElementSetAttr(curNode, dev_pvc_attr[connType],"Static IP");
				break;
			case WAN_ENCAP_PPP:/*PPPoA && PPPoE*/
				memset(buf,0,sizeof(buf));
				sprintf(isp_conf_path, ISPCONFIG_PATH, cur_pvc);
				get_profile_str("ENCAP=",buf, sizeof(buf), QMARKS,isp_conf_path);
				if(!(strcmp(buf, PPPoE_LLC)&&strcmp(buf, PPPoE_VC_Mux)))
				{
					mxmlElementSetAttr(curNode, dev_pvc_attr[connType],"PPPoE");
				}
				else if(!(strcmp(buf, PPPoA_LLC)&&strcmp(buf, PPPoA_VC_Mux)))
				{
					mxmlElementSetAttr(curNode, dev_pvc_attr[connType],"PPPoA");
				}
				else
				{
					mxmlElementSetAttr(curNode, dev_pvc_attr[connType],"Unknown type");
				}
				get_profile_str("CONNECTION=",buf, sizeof(buf), QMARKS,isp_conf_path);
				if(!strcmp(buf, CONNECT_TYPE_MANUALLY))
				{
					mxmlElementSetAttr(curNode, dev_pvc_attr[DispConnBtnType], CONNECT_DISP_BUTTON);
				}
				break;
#endif
			case WAN_ENCAP_BRIDGE:/*bridge mode*/
			default:
				mxmlElementSetAttr(curNode, dev_pvc_attr[connType],"Bridge");
				break;
		}
	return SUCCESS;
}/*end system_read*/
#ifndef PURE_BRIDGE
/*_____________________________________________________________________________
**      function name: getDefGW
**      descriptions:
**           Get the defualt gateway ip address information.
**
**      parameters:
**            gateway:  Put the buffer of gateway.
**
**      global:
**             None
**
**      return:
**             a char pointer point to the defualt gateway ip address information.
**
**      call:
**             None
**
**      revision:
**      1. Here 2008/5/7
**	  2. krammer 2008/10/06
**____________________________________________________________________________
*/
char*
getDefGW(char* gateway){
	enum routeInfo_en{
		DstIP=0,
		Gw,
		Mask,
		Flags,
		Metric,
		Ref,
		Use,
		Iface,
		Usr_def,
	};

	int i=0;
	char *buf=NULL;
	char routeInfo[8][16];
	FILE *fp=NULL;

	memset(routeInfo,0, sizeof(routeInfo));
	strcpy(gateway, "");
	system(SYSROUTE_CMD);

	fp=fopen(SYSROUTE_PATH, "r");
	if(fp==NULL){
		return gateway;
	}
	else{
		buf= malloc(512);
		if(buf==NULL){
			fclose(fp);
			unlink(SYSROUTE_PATH);
			return gateway;
		}
		/*
		Kernel IP routing table
		Destination	Gateway	Genmask			Flags	Metric	Ref	Use	Iface
		10.10.10.0	0.0.0.0	255.255.255.0		U		0		0	0	br0
		127.0.0.0		0.0.0.0	255.255.0.0		U		0		0	0	lo
		*/
		while (fgets (buf, 512, fp) != NULL){
			if(i > 1){/*Skip the comment information*/
				sscanf(buf, "%s %s %s %s %s %s %s %s",
				routeInfo[DstIP], routeInfo[Gw], routeInfo[Mask],
				routeInfo[Flags], routeInfo[Metric], routeInfo[Ref],
				routeInfo[Use], routeInfo[Iface]);
				if(strcmp(routeInfo[Flags],DEF_GW_FLAG) == 0){
					strcpy(gateway, routeInfo[Gw]);
					break;
				}
			}
			i++;
		}
	}
	free(buf);
	fclose(fp);
	unlink(SYSROUTE_PATH);
	return gateway;
}/*end getDefGW*/

#endif
char PVC_GATEWAY[32] = {0};

char *getPVCGateway(int pvcindex)
{
	char pvc_stat[32]={0};
	char adsl_stat[32]={0};
	char buf[32]={0};
	char dev[16] = {0};
	char ispfile[32] = {0};
	char gateway[32] = {0};
	char gatewayfile[32] = {0};
	char pvc_info[16]={0};/*use to record temp pvc_info*/
#if defined(TCSUPPORT_CT_PON)
#if defined(TCSUPPORT_WAN_GPON) || defined(TCSUPPORT_WAN_EPON)
	mxml_node_t * top = *gTreePtr;
	tcapi_msg_t msg;
#endif	 
#endif
	
	memset(PVC_GATEWAY, 0, sizeof(PVC_GATEWAY));
	strcpy(gateway, "N/A");
	strcpy(PVC_GATEWAY, "N/A");

	if(pvcindex<0 || pvcindex>7)
	{
		tcdbg_printf("getPVCGateway: pvcindex error is [%d],  range should be 0-7\n", pvcindex);
		return PVC_GATEWAY;

	}
	
	sprintf(ispfile, ISPCONFIG_PATH, pvcindex);
	get_profile_str("Active=",pvc_stat, sizeof(pvc_stat), QMARKS, ispfile);
	get_profile_str("ISP=",buf, sizeof(buf), QMARKS, ispfile);
	
#if defined(TCSUPPORT_CT_PON)
#if defined(TCSUPPORT_WAN_GPON) || defined(TCSUPPORT_WAN_EPON)
		memset(adsl_stat, 0, sizeof(adsl_stat));
		memset(&msg,0,sizeof(tcapi_msg_t));
		snprintf(msg.node,MAXLEN_NODE_NAME,"XPON_Common");
		memset(msg.attr,0,sizeof(msg.attr));
		memset(msg.value,0,sizeof(msg.value));
		strcpy(msg.attr,"trafficStatus");
		tcapi_get_req(top,&msg);
		if(msg.retval == TCAPI_PROCESS_OK){
			strcpy(adsl_stat,msg.value);
		}
#endif	
#else
	fileRead(ADSL_STATE_PATH, adsl_stat, sizeof(adsl_stat));
#endif

	//tcdbg_printf("getPVCGateway: adsl_stat is [%s], pvc_stat is [%s]\n", adsl_stat, pvc_stat);

	if((strcmp(pvc_stat, ACTIVE)==0) && (strstr(adsl_stat,"up")) ){
		switch(atoi(buf)){
#ifndef PURE_BRIDGE
			case WAN_ENCAP_DYN:/*Dynamic IP Address*/
			case WAN_ENCAP_STATIC:/*Static IP Address*/
				sprintf(dev, NAS_IF, pvcindex);
				if(strlen(getWanInfo(Ip_type, dev, pvc_info))>0){
					strcpy(gateway, "N/A");
					if(atoi(buf) == WAN_ENCAP_STATIC) /*Static IP, we get gateway from isp$i.conf*/
					{
						sprintf(gatewayfile, ISPCONFIG_PATH, pvcindex);
						get_profile_str("GATEWAY=", gateway, sizeof(gateway), QMARKS, gatewayfile);
						//tcdbg_printf("getPVCGateway:  WAN_ENCAP_STATIC: we get gateway from file [%s], gateway is [%s]\n", gatewayfile, gateway);
					}
					else/*Dynamic IP, we get from /etc/nas$i_gateway.conf file*/
					{
						sprintf(gatewayfile, NAS_GATEWAYFILE, pvcindex);
						get_profile_str(NAS_GATEWAY, gateway, sizeof(gateway), NO_QMARKS, gatewayfile);
						//tcdbg_printf("getPVCGateway:  WAN_ENCAP_DYN: we get gateway from file [%s], gateway is [%s]\n", gatewayfile, gateway);
					}
					
				}
				break;
			case WAN_ENCAP_PPP:/*PPPoA && PPPoE*/
				/**********20081001 racing add************/
				if(checkPpp(pvcindex))
				{
					sprintf(dev, PPP_IF, pvcindex);
					getWanInfo(PTP_type, dev, gateway);
					//tcdbg_printf("getPVCGateway: WAN_ENCAP_PPP: we get gateway fromgetWanInfo dev is[%s], gateway is [%s]\n", dev, gateway);
					#if 0
					/*Check Static PPP IP or Dynamic IP*/
					sprintf(gatewayfile, ISPCONFIG_PATH, pvcindex);
					get_profile_str("PPPGETIP=", gateway, sizeof(gateway), QMARKS, gatewayfile);
					tcdbg_printf("getPVCGateway: WAN_ENCAP_PPP: we get PPPGETIP from file [%s], PPPGETIP is [%s]\n", gatewayfile, gateway);

					if(strcmp(gateway, "Static") == 0) /*Static IP, we get gateway from isp$i.conf*/
					{
						strcpy(gateway, "N/A");
						sprintf(gatewayfile, ISPCONFIG_PATH, pvcindex);
						get_profile_str("GATEWAY=", gateway, sizeof(gateway), QMARKS, gatewayfile);
						tcdbg_printf("getPVCGateway: WAN_ENCAP_PPP: Static IP we get gateway from file [%s], gateway is [%s]\n", gatewayfile, gateway);
					}
					else/*Dynamic IP, we get from /etc/nas$i_gateway.conf file*/
					{
						strcpy(gateway, "N/A");
						sprintf(gatewayfile, NAS_GATEWAYFILE, pvcindex);
						get_profile_str(NAS_GATEWAY, gateway, sizeof(gateway), NO_QMARKS, gatewayfile);
						tcdbg_printf("getPVCGateway: WAN_ENCAP_PPP: Dynamic IP, we get gateway from file [%s], gateway is [%s]\n", gatewayfile, gateway);
					}
					#endif

				}
				break;
#endif
			case WAN_ENCAP_BRIDGE:/*bridge mode*/
				break;
			default:
				break;
		}
	}
	strcpy(PVC_GATEWAY, gateway);
	return PVC_GATEWAY;

}
#ifdef TCSUPPORT_DNSEACHPVC
/*_____________________________________________________________________________
**      function name: getDNSInfo
**      descriptions:
**           Get the cpe wan's dns information.
**
**      parameters:
**            type: Pridns
**            	     Secdns
**             isp:  isp type, ext: Dynamic or PPP
**            dev:  interface type, ext: nas0 or ppp0
**
**      global:
**             None
**
**      return:
**             a char pointer (wan's interface information)
**
**      call:
**             fileRead
**
**      revision:
**      1. Renyu 2010/4/9
**____________________________________________________________________________
*/
char*
getDNSInfo(char* type, char* isp, char* dev, char* buf){
	//for test
	//char nodeName[32] = {0};
	//char value[10] = {0};
	char fileName[50] = {0};
	char sPVC[3] = {0};
	char tmpBuf[512] = {0};
	char dnsInfo[2][16];

	int iPVC = 0;

	strcpy(buf,"");
	memset(dnsInfo,0, sizeof(dnsInfo));
	strcpy(sPVC, dev+3);
	iPVC = atoi(sPVC);
	
	if(strcmp(type, "Pridns") == 0){
		if(strcmp(isp, "Static") == 0){	//get from Wan_PVC node
		
			//sprintf(nodeName, "Wan_PVC%d", iPVC);
			//if(tcapi_get(nodeName, "Pridns", value) == TCAPI_PROCESS_OK)
			//	strcpy(buf, value);
			//for test
			strcpy(buf, "168.95.1.1");
		}else if(strcmp(isp, "Dynamic") == 0){	//get from /etc/resolve_nasi.conf file
			sprintf(fileName, "/etc/resolv_nas%d.conf", iPVC);
			fileRead(fileName, tmpBuf, sizeof(tmpBuf));			
			if(strstr(tmpBuf, DNS_ATTR)){
				sscanf(strstr(tmpBuf, DNS_ATTR),"%s %s", dnsInfo[0], dnsInfo[1]);
				strcpy(buf, dnsInfo[1]);	
			}
		}else{	//ppp: get from /etc/ppp/resolve_pppi.conf file
			sprintf(fileName, "/etc/ppp/resolv_ppp%d.conf", iPVC);
			fileRead(fileName, tmpBuf, sizeof(tmpBuf));
			if(strstr(tmpBuf, DNS_ATTR)){
				sscanf(strstr(tmpBuf, DNS_ATTR),"%s %s", dnsInfo[0], dnsInfo[1]);
				strcpy(buf, dnsInfo[1]);	
			}
		}
	}else{
		if(strcmp(isp, "Static") == 0){
			//sprintf(nodeName, "Wan_PVC%d", iPVC);
			//if(tcapi_get(nodeName, "Secdns", value) == TCAPI_PROCESS_OK)
			//	strcpy(buf, value);
			//for test 
			strcpy(buf, "168.95.1.2");
		}else if(strcmp(isp, "Dynamic") == 0){
			sprintf(fileName, "/etc/resolv_nas%d.conf", iPVC);
			memset(tmpBuf, 0, sizeof(tmpBuf));
			fileRead(fileName, tmpBuf, sizeof(tmpBuf));
			if(strstr(tmpBuf + strlen(DNS_ATTR), DNS_ATTR)){
					sscanf(strstr(tmpBuf + strlen(DNS_ATTR), DNS_ATTR), "%s %s", dnsInfo[0], dnsInfo[1]);
					strcpy(buf, dnsInfo[1]);
				}
		}else{	//"ppp"
			sprintf(fileName, "/etc/ppp/resolv_ppp%d.conf", iPVC);
			fileRead(fileName, tmpBuf, sizeof(tmpBuf));
			if(strstr(tmpBuf + strlen(DNS_ATTR), DNS_ATTR)){
				sscanf(strstr(tmpBuf + strlen(DNS_ATTR), DNS_ATTR), "%s %s", dnsInfo[0], dnsInfo[1]);
				strcpy(buf, dnsInfo[1]);
			}
		}
	}
	return buf;
}
#endif
/*_____________________________________________________________________________
**      function name: getWanInfo
**      descriptions:
**           Get the cpe wan's interface information.
**
**      parameters:
**            type: Ip_type: Get wan interface ip address.
**            	     Netmask_type: Get the wan interface netmask information.
**            	     Pridns_type: Get primary DNS information
**            device:  Wan interface name, ext: nas0 or ppp0
**            buf:  Put the buffer that will be got information.
**
**      global:
**             None
**
**      return:
**             a char pointer (wan's interface information)
**
**      call:
**             fileRead
**
**      revision:
**      1. Here 2008/5/7
**	2. krammer 2008/10/06
**____________________________________________________________________________
*/
char*
getWanInfo(int type, char* device, char* buf){
	char cmd[32]={0};
	char tmpBuf[512]={0};
	char *pValue=NULL;
	FILE * fp=NULL;
	char wanInfo[3][24];
	char dnsInfo[2][16];
	char macInfo[2][18];

	memset(wanInfo,0, sizeof(wanInfo));
	memset(dnsInfo,0, sizeof(dnsInfo));
	memset(macInfo,0, sizeof(macInfo));

	sprintf(cmd,"ifconfig %s > %s", device, TMP_IF_PATH);
	system(cmd);
	strcpy(buf,"");

	fp=fopen(TMP_IF_PATH, "r");
	if(fp==NULL){
		return buf;
	}
	else{
		while (fgets (tmpBuf, sizeof(tmpBuf), fp) != NULL){
			/*krammer add*/
			if((pValue=strstr(tmpBuf,"HWaddr"))){
			       /*tmpBuf format: HWaddr 00:AA:BB:01:23:45*/
				sscanf(pValue,"%s %s", macInfo[0],macInfo[1]);
                        }
			/*krammer add*/
			if((pValue=strstr(tmpBuf,"addr:"))){
				/*tmpBuf format: inet addr:10.10.10.1  Bcast:10.10.10.255  Mask:255.255.255.0*/
				sscanf(pValue,"%s %s %s", wanInfo[0],wanInfo[1], wanInfo[2]);
				break;
			}
		}
	}
	fclose(fp);
	memset(tmpBuf,0, sizeof(tmpBuf));
	#if DEBUG
		fprintf(stderr,"wanInfo[0]:%s wanInfo[1]:%s wanInfo[2]:%s\r\n", wanInfo[0],wanInfo[1], wanInfo[2]);
	#endif
	switch(type){
		case Ip_type:/*IP address information*/
			/*20090717: pork modified for TC3262*/
			pValue=strchr(wanInfo[0],':');
			if(pValue){
				strcpy(buf, pValue+1);
			}
			break;
		case PTP_type:/*PPP connection, get PTP Addr*/
			pValue=strchr(wanInfo[1],':');
			if(pValue){
				strcpy(buf, pValue+1);
			}
			break;
		case Netmask_type:/*netmask address*/
			/*20090717: pork modified for TC3262*/
			pValue=strchr(wanInfo[2],':');
			if(pValue){
				strcpy(buf, pValue+1);
			}
			break;
		case Pridns_type: /*IP address of DNS Server*/
		case Secdns_type: //xyzhu_20091225 support secondary dns.
			/*if(strstr(device,WAN_NAS)){*/  /*rodney_20090506 modified*/
			/*No matter CPE use PPP or MER, the actual DNS for system use is /etc/resolv.conf. So status should display the real DNS of system use. */
			/*If CPE use PPP, /etc/resolv.conf will be replaced by /etc/ppp/resolv.conf automatically.*/
				fileRead(DNS_INFO_PATH, tmpBuf, sizeof(tmpBuf));
#if 0
			}
			else{
				fileRead(PPP_DNS_INFO_PATH, tmpBuf,sizeof(tmpBuf));
			}
#endif
			//krammer change for bug list #107
			if(strstr(tmpBuf, DNS_ATTR)){
				/*tmpBuf format: nameserver x.x.x.x*/
#if 0
				sscanf(tmpBuf,"%s %s",dnsInfo[0],dnsInfo[1]);
#endif
				sscanf(strstr(tmpBuf, DNS_ATTR),"%s %s",dnsInfo[0],dnsInfo[1]);
				strcpy(buf, dnsInfo[1]);
				/*xyzhu_20091225 support secondary dns server*/
				if(type == Secdns_type){
					memset(buf, 0, strlen(buf) + 1);
					if(strstr(tmpBuf + strlen(DNS_ATTR), DNS_ATTR)){
						sscanf(strstr(tmpBuf + strlen(DNS_ATTR), DNS_ATTR), "%s %s", dnsInfo[0], dnsInfo[1]);
						strcpy(buf, dnsInfo[1]);
					}
				}
			}
			break;
		case Mac_type:/*Mac address*/
			if(strlen(macInfo[1])>0){
				strcpy(buf, macInfo[1]);
			}
			break;
		default:
			break;
	}
	unlink(TMP_IF_PATH);
	return buf;
}/*end getWanInfo*/

/*_____________________________________________________________________________
**      function name: deviceInfo_execute
**      descriptions:
**            Trigger CPE to update wan interface's  IP address or
**            Trigger CPE to release wan interface's IP address.
**
**      parameters:
**            top:   Put the top of cfg node tree that is used to execute system information.
**            name: Put the deviceInfo cfg node name.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**      	getAttrValue
**      	fileRead
**
**      revision:
**      1. Here 2008/5/19
**      2. Renyu 2009/12/31
**____________________________________________________________________________
*/
int
deviceInfo_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
#ifndef PURE_BRIDGE
	char path[32]={0};
	char pid[8]={0};
	char tmp[8]={0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];

	char string[32] = {0};
	int cur_vc;

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],WEBCURSET);
	strcpy(nodeName[1],SUB_NODE_NAME);
	getAttrValue(top, nodeName, "dev_pvc", tmp);
	sprintf(path, UDHCPC_PID_PATH, tmp);

	cur_vc = atoi(tmp);
	//manual connect
	memset(tmp, 0, sizeof(tmp));	
	if(getAttrValue(top, name, "DispConnBtnType", tmp) != TCAPI_PROCESS_OK)
	{
		return FAIL;
	}
	if(0 != atoi(tmp))
	{
		/*stop wan PVCn*/
		sprintf(string, "%s %d",WAN_STOP_SH, cur_vc);
		system(string);
		
		switch(atoi(tmp))
		{
			case CONNECT:				
				/* start wan*/
				//tcdbg_printf("start connection.\n");
				sprintf(string, "%s %d",PPP_START_SH, cur_vc);
				system(string);
				break;
			default:
				break;
		}
	}
	/*Get udhcpc process-id to send signal to release ip or renew ip*/
	fileRead(path, pid, sizeof(pid));
	if(strlen(pid)==0){
		fprintf(stderr,"None active udhcpc process\r\n");
		return FAIL;
	}

	memset(tmp, 0, sizeof(tmp));
	getAttrValue(top, name, "DispBtnType", tmp);

	if(atoi(tmp)==RENEW_IP){
		/*Renew WAN IP*/
		kill(atoi(pid), SIGUSR1);
	}else if(atoi(tmp)==RELEASE_IP){
		/*Release WAN IP*/
		kill(atoi(pid), SIGUSR2);
	}
#endif
	return SUCCESS;
}/*end deviceInfo_execute*/
int
dynDisp_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="dynDisp",
			.type=WEB_TYPE|SINGLE_TYPE|UPDATE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=dynDisp_read,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=NULL,
			.cfg_boot=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}

int
dynDisp_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr){

	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], dispMask[25],  accEntry[7], curAccess[2], curPage[2];
#if defined(TCSUPPORT_CT_PON_CY_JX)
	short bitMask[] = { (1<<0), (1<<1), (1<<2), (1<<3), (1<<4)
					, (1<<5), (1<<6), (1<<7) , (1<<8) , (1<<9) };
#else
#if defined(TCSUPPORT_CT_PHONEAPP)
	short bitMask[9] = {(1<<0), (1<<1), (1<<2), (1<<3), (1<<4), (1<<5), (1<<6), (1<<7), (1<<8)};
#else
	char bitMask[8] = {(1<<0), (1<<1), (1<<2), (1<<3), (1<<4), (1<<5), (1<<6), (1<<7)};
#endif
#endif
	mxml_node_t *curNode=NULL;
	int i=0, dispBytesIdx, mainMask, dispMaskBytes[8];

	memset(nodeName,0,sizeof(nodeName));

	strcpy(nodeName[0], DYNDISP);
	strcpy(nodeName[1], SUB_NODE_NAME);

	curNode=get_web_node(top,nodeName);

	if(curNode==NULL){
		return FAIL;
	}

	strcpy(nodeName[0], "WebCurSet");
	strcpy(nodeName[1], SUB_NODE_NAME);
	getAttrValue(top, nodeName, "CurrentAccess", curAccess);
	sprintf(accEntry, "%s%s", SUB_NODE_NAME, curAccess);
	strcpy(nodeName[0], "Account");
	strcpy(nodeName[1], accEntry);
	getAttrValue(top, nodeName, "display_mask", dispMask);
	sscanf(dispMask, "%x %x %x %x %x %x %x %x %x", &mainMask, &dispMaskBytes[0], &dispMaskBytes[1], &dispMaskBytes[2], &dispMaskBytes[3], &dispMaskBytes[4], &dispMaskBytes[5], &dispMaskBytes[6], &dispMaskBytes[7]);

	strcpy(nodeName[0], "dynDisp");
	strcpy(nodeName[1], SUB_NODE_NAME);
	getAttrValue(top, nodeName, "CurPage", curPage);
	dispBytesIdx = atoi(curPage);
	if(dispBytesIdx < 0 || dispBytesIdx > 7){
		fprintf(stderr, "Wrong dispBytesIdx\r\n");
		return FAIL;
	}
	
	sprintf(curPage, "%d", dispBytesIdx);	
	mxmlElementSetAttr(curNode, dynDisp_attr[0], curPage);
	for(i=1; i < 9; i++){
		char bit[2];

		sprintf(bit, "%d", (mainMask & bitMask[i-1]) >> (i-1));
		mxmlElementSetAttr(curNode, dynDisp_attr[i], bit);
	}
	for(i=9; strlen(dynDisp_attr[i])!=0; i++){
		char bit[2];

		sprintf(bit, "%d", (dispMaskBytes[dispBytesIdx] & bitMask[i-9]) >> (i-9));
		mxmlElementSetAttr(curNode, dynDisp_attr[i], bit);
	}

	return SUCCESS;
}

#if defined(TCSUPPORT_CT_GUIACCESSLIMIT)
#if 0
static Unsigned8 ConvertBase64Character(Unsigned8 theInputCharacter) {
    Unsigned8   theResult;

    theResult = 0;
    if (theInputCharacter >= kAscii_A &&
        theInputCharacter <= kAscii_Z ) {
        theResult = theInputCharacter - kAscii_A;
    }
    else if (theInputCharacter >= kAscii_a &&
                theInputCharacter <= kAscii_z) {
        theResult = theInputCharacter - kAscii_a + 26;
    }
    else if (theInputCharacter >= kAscii_0 &&
                theInputCharacter <= kAscii_9) {
        theResult = theInputCharacter - kAscii_0 + 52;
    }
    else if (theInputCharacter == kAscii_Plus) {
        theResult = 62;
    }
    else if (theInputCharacter == kAscii_Slash) {
        theResult = 63;
    }
    return theResult;
}

/*
    RpDecodeBase64Data decodes an array of characters encoded as described
    in section 5.2 of RFC 1521 (MIME Part One)
    "Base64 Content-Transfer-Encoding"

    * theInputPtr points to an array of bytes that is at least as long
      as theInputCount
    * theInputPtr must not contain any white space that should be ignored
    * theOutputPtr is long enough to accept the result including a
      terminating null
*/

Unsigned16 RpDecodeBase64Data(char *theInputPtr, Unsigned16 theInputCount,
        char *theOutputPtr) {
    Unsigned32  thePackedGroup;
    char *      theOriginalOutputPtr;

    theOriginalOutputPtr = theOutputPtr;

    /*
        Make sure theInputCount is a multiple of 4.
    */
    theInputCount -= (Unsigned16) (theInputCount % 4);

    while (theInputCount > 0) {
        thePackedGroup =
                (Unsigned32) ConvertBase64Character(*theInputPtr++) << 18;
        thePackedGroup |=
                (Unsigned32) ConvertBase64Character(*theInputPtr++) << 12;
        *theOutputPtr++ = (Unsigned8) (thePackedGroup >> 16);
        if (*theInputPtr != kAscii_Equal) {
            thePackedGroup |=
                    (Unsigned32) ConvertBase64Character(*theInputPtr++) << 6;
            *theOutputPtr++ = (Unsigned8) (thePackedGroup >> 8);
            if (*theInputPtr != kAscii_Equal) {
                thePackedGroup |=
                        (Unsigned32) ConvertBase64Character(*theInputPtr++);
                *theOutputPtr++ = (Unsigned8) (thePackedGroup);
            }
        }
        theInputCount -= 4;
    }
    *theOutputPtr = '\0';

    return theOutputPtr - theOriginalOutputPtr;
}


/*
    RpDecodeBase64DataNoNull decodes an array of characters encoded as described
    in section 5.2 of RFC 1521 (MIME Part One)
    "Base64 Content-Transfer-Encoding"

    * theInputPtr points to an array of bytes that is at least as long
      as theInputCount
    * theInputPtr must not contain any white space that should be ignored
    * theOutputPtr length equal to the results of decode.
       The results don't include a  terminating null.
*/

Unsigned16 RpDecodeBase64DataNoNull(char *theInputPtr, Unsigned16 theInputCount,
        char *theOutputPtr) {
    Unsigned32  thePackedGroup;
    char *      theOriginalOutputPtr;

    theOriginalOutputPtr = theOutputPtr;

    /*
        Make sure theInputCount is a multiple of 4.
    */
    theInputCount -= (Unsigned16) (theInputCount % 4);

    while (theInputCount > 0) {
        thePackedGroup =
                (Unsigned32) ConvertBase64Character(*theInputPtr++) << 18;
        thePackedGroup |=
                (Unsigned32) ConvertBase64Character(*theInputPtr++) << 12;
        *theOutputPtr++ = (Unsigned8) (thePackedGroup >> 16);
        if (*theInputPtr != kAscii_Equal) {
            thePackedGroup |=
                    (Unsigned32) ConvertBase64Character(*theInputPtr++) << 6;
            *theOutputPtr++ = (Unsigned8) (thePackedGroup >> 8);
            if (*theInputPtr != kAscii_Equal) {
                thePackedGroup |=
                        (Unsigned32) ConvertBase64Character(*theInputPtr++);
                *theOutputPtr++ = (Unsigned8) (thePackedGroup);
            }
        }
        theInputCount -= 4;
    }
    return theOutputPtr - theOriginalOutputPtr;
}
#endif
int
dynCwmpAttr_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="dynCwmpAttr",
			.type=WEB_TYPE|SINGLE_TYPE|UPDATE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=dynCwmpAttr_read,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=NULL,
			.cfg_boot=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}

int
dynCwmpAttr_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], curAttrValue[1024]={0}, setValue[4]={0};
	mxml_node_t *curNode=NULL;
	int i=0, idx=0, count=0;
	char tmp[32]={0};

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], DYNCWMPATTR);
	strcpy(nodeName[1], SUB_NODE_NAME);
	curNode=get_web_node(top,nodeName);
	if(curNode==NULL){
		return FAIL;
	}

	memset(nodeName,0,sizeof(nodeName));
	sprintf(nodeName[0], "WanInfo");
	strcpy(nodeName[1], "Common");
	memset(curAttrValue,0,sizeof(curAttrValue));
	if(TCAPI_PROCESS_OK == getAttrValue(top, nodeName, "NoGUIAccessLimit", curAttrValue))
	{
		if(strcmp(curAttrValue, "1") == 0)	//close the GUIAccessLimit Function
		{
			sprintf(setValue, "%d", 0);
			//cwmp
			mxmlElementSetAttr(curNode, "aURL", setValue);
			mxmlElementSetAttr(curNode, "aUsername", setValue);
			mxmlElementSetAttr(curNode, "aPassword", setValue);
			mxmlElementSetAttr(curNode, "aConnectionRequestUsername", setValue);
			mxmlElementSetAttr(curNode, "aConnectionRequestPassword", setValue);
#if defined(TCSUPPORT_CT_PON_GD) || defined(TCSUPPORT_CT_PON_CZ_GD)
			mxmlElementSetAttr(curNode, "aPeriodicInformInterval", setValue);
#endif
			sprintf(setValue, "%d", 1);

		#ifdef TCSUPPORT_WLAN
			//wlan
			mxmlElementSetAttr(curNode, "aSSIDEnable", setValue);
			mxmlElementSetAttr(curNode, "aWLanBeaconType", setValue);
			mxmlElementSetAttr(curNode, "aWLanSSID", setValue);
		#endif

			//wan
			mxmlElementSetAttr(curNode, "aPPPUsername", setValue);
			mxmlElementSetAttr(curNode, "aPPPPassword", setValue);			

			//vlanid
#if defined(TCSUPPORT_CT_PON) || defined(TCSUPPORT_CT_WAN_PTM)
			mxmlElementSetAttr(curNode, "aVLANIDMark", setValue);
#endif
			return SUCCESS;
		}
	}
	
	//cwmp
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], "TR069Attr");
	strcpy(nodeName[1], SUB_NODE_NAME);
	memset(curAttrValue,0,sizeof(curAttrValue));
	getAttrValue(top, nodeName, "cwmpAttr", curAttrValue);
	cwmpAttr *cwmpflag = NULL;
	//cwmpflag = (cwmpAttr*)malloc(sizeof(cwmpAttr));
	cwmpflag = (cwmpAttr*)malloc(sizeof(curAttrValue));
	if(cwmpflag == NULL){
		tcdbg_printf("\r\ndynCwmpAttr_read: malloc cwmpAttr fails.\n");
		return FAIL;
	}
	RpDecodeBase64DataNoNull(curAttrValue, strlen(curAttrValue), (char*)cwmpflag);
	if(cwmpflag->magic != CWMP_MAGIC_ATTR)
	{
		tcdbg_printf("\r\ndynCwmpAttr_read: cwmpflag.magic is wrong now is [%04X] should be [%04X]!\n", cwmpflag->magic, CWMP_MAGIC);	
		free(cwmpflag);
		cwmpflag = NULL;
		return FAIL;
	}

#if defined(TCSUPPORT_CT_PON_GD) || defined(TCSUPPORT_CT_PON_CZ_GD)
	if(cwmpflag->aPeriodicInformInterval&0x01)
		sprintf(setValue, "%d", 0);
	else
		sprintf(setValue, "%d", 1);
	mxmlElementSetAttr(curNode, "aPeriodicInformInterval", setValue);
#endif

	if(cwmpflag->aURL&0x01)
		sprintf(setValue, "%d", 0);
	else
		sprintf(setValue, "%d", 1);
	mxmlElementSetAttr(curNode, "aURL", setValue);

	if(cwmpflag->aUsername&0x01)
		sprintf(setValue, "%d", 0);
	else
		sprintf(setValue, "%d", 1);
	mxmlElementSetAttr(curNode, "aUsername", setValue);

	if(cwmpflag->aPassword&0x01)
		sprintf(setValue, "%d", 0);
	else
		sprintf(setValue, "%d", 1);
	mxmlElementSetAttr(curNode, "aPassword", setValue);

	if(cwmpflag->aConnectionRequestUsername&0x01)
		sprintf(setValue, "%d", 0);
	else
		sprintf(setValue, "%d", 1);
	mxmlElementSetAttr(curNode, "aConnectionRequestUsername", setValue);

	if(cwmpflag->aConnectionRequestPassword&0x01)
		sprintf(setValue, "%d", 0);
	else
		sprintf(setValue, "%d", 1);
	mxmlElementSetAttr(curNode, "aConnectionRequestPassword", setValue);
	free(cwmpflag);
	cwmpflag = NULL;

#ifdef TCSUPPORT_WLAN
	//wlan
	memset(curAttrValue,0,sizeof(curAttrValue));
	getAttrValue(top, nodeName, "wlanAttr", curAttrValue);
	wlanAttr *wlanflag = NULL;
	//wlanflag = (wlanAttr*)malloc(sizeof(wlanAttr));
	wlanflag = (wlanAttr*)malloc(sizeof(curAttrValue));
	if(wlanflag == NULL){
		tcdbg_printf("\r\ndynCwmpAttr_read: malloc wlanAttr fails.\n");
		return FAIL;
	}
	RpDecodeBase64DataNoNull(curAttrValue, strlen(curAttrValue), (char*)wlanflag);
	if(wlanflag->magic != WLAN_MAGIC_ATTR)
	{
		tcdbg_printf("\r\ndynCwmpAttr_read: wlanflag.magic is wrong now is [%04X] should be [%04X]!\n", wlanflag->magic, WLAN_MAGIC_ATTR);	
		free(wlanflag);
		wlanflag = NULL;
		return FAIL;
	}
	memset(nodeName,0,sizeof(nodeName));
  	strcpy(nodeName[0],WEBCURSET);
  	strcpy(nodeName[1],SUB_NODE_NAME);
  	if(getAttrValue(top, nodeName, "wlan_id", tmp) == TCAPI_PROCESS_OK){
    	idx = atoi(tmp);
  	}else{
  		tcdbg_printf("\r\ndynCwmpAttr_read: Get wlan_id fail!\n");
		free(wlanflag);
		wlanflag = NULL;
		return FAIL;
	}
	
	if(wlanflag->aSSIDEnable[idx]&0x01) /* from WLanEnable to SSIDEnable */
		sprintf(setValue, "%d", 0);
	else
		sprintf(setValue, "%d", 1);
	mxmlElementSetAttr(curNode, "aSSIDEnable", setValue); /* from WLanEnable to SSIDEnable */

	if(wlanflag->aWLanBeaconType[idx]&0x01)
		sprintf(setValue, "%d", 0);
	else
		sprintf(setValue, "%d", 1);
	mxmlElementSetAttr(curNode, "aWLanBeaconType", setValue);

	if(wlanflag->aWLanSSID[idx]&0x01)
		sprintf(setValue, "%d", 0);
	else
		sprintf(setValue, "%d", 1);
	mxmlElementSetAttr(curNode, "aWLanSSID", setValue);
	free(wlanflag);
	wlanflag = NULL;
#endif

	//wan
	memset(nodeName,0,sizeof(nodeName));
  	strcpy(nodeName[0],WAN_IF_NODE);
  	strcpy(nodeName[1],WAN_IF_COMMON);	
  	if(getAttrValue(top, nodeName, WAN_IF_COM_CUR_INDEX, tmp) == TCAPI_PROCESS_OK){
    	idx = atoi(tmp);
  	}else{
  		tcdbg_printf("\r\ndynCwmpAttr_read: Get CurIFIndex fail!\n");
    	return FAIL;
	}	
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], "TR069Attr");
	strcpy(nodeName[1], SUB_NODE_NAME);
  	if(getAttrValue(top, nodeName, "ipConAttrLen", tmp) == TCAPI_PROCESS_OK){
    	count = atoi(tmp);
  	}else{
  		tcdbg_printf("\r\ndynCwmpAttr_read: Get ipConAttrLen fail!\n");
    	return FAIL;
	}	
	int attrLen = sizeof(ipConAttr);
	int len = attrLen + (attrLen >> 1); //len equals attrLen*1.5
	char *buf = NULL;
	buf = (char*)malloc(len);
	if(buf == NULL){
		tcdbg_printf("\r\ndynCwmpAttr_read: malloc fails.\n");
		return FAIL;
	}
	memset(buf, 0, len);
	for(i = 0; i < count; i++){
		sprintf(tmp, "ipConAttr%d", i);
		memset(curAttrValue,0,sizeof(curAttrValue));
		getAttrValue(top, nodeName, tmp, curAttrValue);
		strcat(buf, curAttrValue);
	}
	ipConAttr *ipConflag = NULL;
	//ipConflag = (ipConAttr*)malloc(sizeof(ipConAttr));
	ipConflag = (ipConAttr*)malloc(len);
	if(ipConflag == NULL){
		tcdbg_printf("\r\ndynCwmpAttr_read: malloc ipConAttr fails.\n");
		free(buf);
		buf = NULL;
		return FAIL;
	}
	RpDecodeBase64DataNoNull(buf, strlen(buf), (char*)ipConflag);
	free(buf);
	buf = NULL;	
	if(ipConflag->magic != IPCON_MAGIC_ATTR)
	{
		tcdbg_printf("\r\ndynCwmpAttr_read: ipConflag.magic is wrong now is [%04X] should be [%04X]!\n", ipConflag->magic, IPCON_MAGIC_ATTR);
		free(ipConflag);
		ipConflag = NULL;
		return FAIL;
	}
	
	if(ipConflag->aWANPPPCnt1_1Username[idx]&0x01)
		sprintf(setValue, "%d", 0);
	else
		sprintf(setValue, "%d", 1);
	mxmlElementSetAttr(curNode, "aPPPUsername", setValue);

	if(ipConflag->aWANPPPCnt1_1Password[idx]&0x01)
		sprintf(setValue, "%d", 0);
	else
		sprintf(setValue, "%d", 1);
	mxmlElementSetAttr(curNode, "aPPPPassword", setValue);
	free(ipConflag);
	ipConflag = NULL;

#if defined(TCSUPPORT_CT_PON) || defined(TCSUPPORT_CT_WAN_PTM)
	idx = idx / MAX_SMUX_NUM;	//WanIF -> PVC
#endif

#if defined(TCSUPPORT_CT_PON)
	//vlanid
	memset(curAttrValue,0,sizeof(curAttrValue));
	getAttrValue(top, nodeName, "CTPonLinkAttr", curAttrValue);
	CT_PonLinkAttr *CTPonLinkflag = NULL;
	//CTPonLinkflag = (CT_PonLinkAttr*)malloc(sizeof(CT_PonLinkAttr));
	CTPonLinkflag = (CT_PonLinkAttr*)malloc(sizeof(curAttrValue));
	if(CTPonLinkflag == NULL){
		tcdbg_printf("\r\ndynCwmpAttr_read: malloc CT_PonLinkAttr fails.\n");
		return FAIL;
	}
	RpDecodeBase64DataNoNull(curAttrValue, strlen(curAttrValue), (char*)CTPonLinkflag);
	if(CTPonLinkflag->magic != CT_PONLLINK_MAGIC_ATTR)
	{
		tcdbg_printf("\r\ndynCwmpAttr_read: CTPonLinkflag.magic is wrong now is [%04X] should be [%04X]!\n", CTPonLinkflag->magic, CT_PONLLINK_MAGIC_ATTR);
		free(CTPonLinkflag);
		CTPonLinkflag = NULL;
		return FAIL;
	}	

	if(CTPonLinkflag->aWANPonLinkCfg1VLANIDMark[idx]&0x01)
		sprintf(setValue, "%d", 0);
	else
		sprintf(setValue, "%d", 1);
	mxmlElementSetAttr(curNode, "aVLANIDMark", setValue);
	free(CTPonLinkflag);
	CTPonLinkflag = NULL;
#endif
	
#if defined(TCSUPPORT_CT_WAN_PTM)
		//vlanid
		memset(curAttrValue,0,sizeof(curAttrValue));
		getAttrValue(top, nodeName, "CTVdslLinkAttr", curAttrValue);
		CT_VdslLinkAttr *CTVdslLinkflag = NULL;
		//CTVdslLinkflag = (CT_VdslLinkAttr*)malloc(sizeof(CT_VdslLinkAttr));
		CTVdslLinkflag = (CT_VdslLinkAttr*)malloc(sizeof(curAttrValue));
		if(CTVdslLinkflag == NULL){
			tcdbg_printf("\r\ndynCwmpAttr_read: malloc CT_VdslLinkAttr fails.\n");
			return FAIL;
		}
		RpDecodeBase64DataNoNull(curAttrValue, strlen(curAttrValue), (char*)CTVdslLinkflag);
		if(CTVdslLinkflag->magic != CT_VDSLLINK_MAGIC_ATTR)
		{
			tcdbg_printf("\r\ndynCwmpAttr_read: CTVdslLinkflag.magic is wrong now is [%04X] should be [%04X]!\n", CTVdslLinkflag->magic, CT_VDSLLINK_MAGIC_ATTR);
			free(CTVdslLinkflag);
			CTVdslLinkflag = NULL;
			return FAIL;
		}
		
		if(CTVdslLinkflag->aWANVdslLinkCfg1VLANIDMark[idx]&0x01)
			sprintf(setValue, "%d", 0);
		else
			sprintf(setValue, "%d", 1);
		mxmlElementSetAttr(curNode, "aVLANIDMark", setValue);
		free(CTVdslLinkflag);
		CTVdslLinkflag = NULL;
#endif

	
	return SUCCESS;
}
#endif

//"WebCurSet",WEB_TYPE|SINGLE_TYPE,	NULL, NULL,webCurSet_read, NULL, NULL, NULL},
/*______________________________________________________________________________
**	webCurSet_init
**
**	descriptions:
**		the init function of webCurSet.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
webCurSet_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="WebCurSet",
			.type=WEB_TYPE|SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=webCurSet_read,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=NULL,
			.cfg_boot=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}

/*_____________________________________________________________________________
**      function name: webCurSet_read
**      descriptions:
**            To create WebCurSet cfg node and init those attribution value.
**
**      parameters:
**            top:   Put the top of cfg node tree.
**            name: NULL
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**      	get_web_node
**      	mxmlElementSetAttr
**
**      revision:
**      1. Here 2008/5/19
**____________________________________________________________________________
*/
int
webCurSet_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr){
#if 0	
	/*move this big array to global. shnwind*/
	char webCurSet_attr[][16]=
	{
		{"wan_pvc"},
#ifndef PURE_BRIDGE
		{"nat_pvc"},
#endif
		{"diag_pvc"},
		{"dev_pvc"},
#ifndef PURE_BRIDGE
		{"virServ_id"},
		//{"ipAddr_id"},
		{"route_id"},
#endif
		{"statis_type"},
		{"lan_id"},
#ifndef PURE_BRIDGE
		{"wlan_id"},
		#if QOS_REMARKING  /*Rodney_20090724*/
		{"qos_id"},
		#endif
#ifdef TCSUPPORT_QOS
		{"qos_id"},
#endif
		{"acl_id"},/*20080825 racing add*/
		{"ipfilter_id"},/*20080926 racing add*/
		{"url_filter_id"},/*20081021 krammer add*/
#endif
		{"autopvc_id"},
		{""}
	};
#endif	
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	mxml_node_t *curNode=NULL;
	int i=0;
#if defined(TCSUPPORT_CT_PON_JS)	
	char attrValue[32] = {0};
	char str_wanmode[32] = {0};
	char str_servicelist[32] = {0};
	int valid_if[MAX_WAN_IF_INDEX] = {0}, valid_if_num = 0;
	int pvc_index = 0, entry_index = 0;
#endif
	memset(nodeName,0,sizeof(nodeName));

	strcpy(nodeName[0], WEBCURSET);
	strcpy(nodeName[1], SUB_NODE_NAME);

	curNode=get_web_node(top,nodeName);

	if(curNode==NULL){
		return FAIL;
	}
	/*Init WebCurSet cfg node attribute-value*/
	for(i=0; strlen(webCurSet_attr[i])!=0; i++){
		mxmlElementSetAttr(curNode, webCurSet_attr[i], "0");
	}

	mxmlElementSetAttr(curNode, "dmz_ifidx", "999");
#if defined(TCSUPPORT_CT_PON_JS)
	valid_if_num = get_all_wan_index(valid_if, MAX_WAN_IF_INDEX);
	for(i = 0; i < valid_if_num; i++){	
		pvc_index = valid_if[i] / MAX_SMUX_NUM;
		entry_index = valid_if[i] % MAX_SMUX_NUM;
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0],WAN);
		sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
		sprintf(nodeName[2], "%s%d", WAN_INTERFACE, entry_index);
		if(getAttrValue(top, nodeName, "WanMode", str_wanmode) != TCAPI_PROCESS_OK ){
			continue;
		}
		if(getAttrValue(top, nodeName, "ServiceList", str_servicelist) != TCAPI_PROCESS_OK ){
			continue;
		}
		if((strstr(str_servicelist,"TR069")==NULL||strstr(str_servicelist,"INTERNET")!=NULL) && strstr(str_wanmode,"Route")!=NULL){
			sprintf(attrValue, "%d", valid_if[i]);
			break;
		}
	}
	
	if(strlen(attrValue)>0)
		mxmlElementSetAttr(curNode, "nat_ifidx", attrValue);
	else
		mxmlElementSetAttr(curNode, "nat_ifidx", "999");
#else
	mxmlElementSetAttr(curNode, "nat_ifidx", "999");
#endif
	return SUCCESS;
}/*end webCurSet_read*/

#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
int checkSIMWEBAuth( mxml_node_t *top, mxml_node_t *cur_node )
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char cmdpasswd[150] ={0};
	char webAuthRet[32] ={0};
	char password[128] = {0};
	int ret = -1;

	if ( !top || !cur_node )
		return FAIL;
	// default failed
	mxmlElementSetAttr(cur_node, "webAuthRet", "0");
	// get web login password
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], "WebCurSet");
	strcpy(nodeName[1], "Entry");
	if ( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "WebPSW", password) )
		return -1;
	// check simcard auth
	specialCharacterHandle(password);
	sprintf(cmdpasswd,"/userfs/bin/simCard loginAuth telecomadmin \"%s\" notcapi", password);
	ret = system(cmdpasswd);
	// get simcard auth state
	ret = WEXITSTATUS(ret);

	if( 0 == ret )
		mxmlElementSetAttr(cur_node, "webAuthRet", "1");

	return 0;
}
#endif

//"Info",		UPDATE_TYPE|WEB_TYPE,	NULL, NULL, info_read, NULL, NULL, NULL},
/*______________________________________________________________________________
**	info_init
**
**	descriptions:
**		the init function of info.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
info_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="Info",
			.type=UPDATE_TYPE|WEB_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=info_read,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=NULL,
			.cfg_boot=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
/*_____________________________________________________________________________
**      function name: memory_read
**      descriptions:
**            To get memoryUsage form "/proc/meminfo".
**
**      parameters:
**            memoryUsage:   return value.
**
**      global:
**             None
**
**      return:
**            Success:        memoryUsage
**            Otherwise:      memoryUsage
**
**      revision:
**      1. mhgao 2010/7/27
**____________________________________________________________________________
*/
int memory_read(char *memoryUsage){
        FILE *fp = NULL;
        char buf[40] = {0};
        char temp[40] = {0};
        unsigned int memTotal = 0;
        unsigned int memFree = 0;
        unsigned int buffers = 0;
        unsigned int cached = 0;
        int i = 0;
        
        fp = fopen("/proc/meminfo","r");
        if(NULL == fp)
        {       
                printf("\r\nopen file error!");
                return -1;
        }
 
        while( !(feof(fp)) && (i<4) )
        {
                memset(buf,0, sizeof(buf));
                memset(temp,0, sizeof(temp));
                fgets(buf , sizeof(buf) , fp);
                sscanf(buf,"%*s%s",temp);
                if(strstr(buf,"MemTotal:") != NULL){
                        memTotal = atoi(temp);
                } else if(strstr(buf,"MemFree:") != NULL){
                        memFree = atoi(temp);
                } else if(strstr(buf,"Buffers:") != NULL){
                        buffers = atoi(temp);
                } else if(strstr(buf,"Cached:") != NULL){
                        cached = atoi(temp);
                } else {
                        tcdbg_printf("memory_read can't find the key word!\n");
                }
                
                i++;
        }
        sprintf(memoryUsage,"%d",((memTotal-memFree-buffers-cached)*100)/memTotal);
        fclose(fp);
        return 0;
}
#endif


/*_____________________________________________________________________________
**      function name: info_read
**      descriptions:
**            To create Info cfg node and filled those attribution value.
**
**      parameters:
**            top:   Put the top of cfg node tree.
**            name: Put the sub Info cfg node name.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**      	del_sub_node
**      	get_web_node
**      	etherInfo_read
**      	adslInfo_read
**      	wlanInfo_read
**
**      revision:
**      1. Here 2008/5/19
**____________________________________________________________________________
*/
int
info_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr){
	mxml_node_t *curNode = NULL;
	
//	del_sub_node(top,name);
	curNode=get_web_node(top, name);
	if(curNode==NULL){
		return FAIL;
	}

	if(strlen(name[1])==0){
		return SUCCESS;
	}

	#if 0
	if(strcmp(name[1],"Ether")==0){
		etherInfo_read(curNode);
	}
	else if(strcmp(name[1],"Adsl")==0){
		adslInfo_read(top,curNode);
	}
	else if(strcmp(name[1],"WLan") == 0){
		wlanInfo_read(curNode);
	}

	return SUCCESS;
	#endif

#ifndef PURE_BRIDGE
		if(strcmp(name[1],"Ether")==0){
			etherInfo_read(curNode);
		}
#else
		if(strcmp(name[1],"Usb")==0){
			usbInfo_read(curNode);
		}
#endif
		else if(strcmp(name[1],"Adsl")==0){
			//fprintf(stderr,"run adslinfo_read\n");
			adslInfo_read(top,curNode);
		}
#if defined(TCSUPPORT_CT_PON)
		else if(strcmp(name[1],"Xpon")==0){
			//fprintf(stderr,"run xponinfo_read\n");
			xponInfo_read(top,curNode);
		}
#endif
#if defined(TCSUPPORT_CT_WAN_PTM)
		else if(strcmp(name[1],"Ptm")==0){
			//fprintf(stderr,"run ptminfo_read\n");
			ptmInfo_read(top,curNode);
		}
#endif
#ifdef CWMP
		else if(strcmp(name[1], "Showtime") == 0)
			adslStatsShow_read(top, curNode);
		else if(strcmp(name[1], "LastShow") == 0)
			adslStatsLast_read(top, curNode);
		else if(strcmp(name[1], "TotalShow") == 0)
			adslStatsTotal_read(top, curNode);
		else if(strcmp(name[1], "CurdayStShow") == 0)
			adslStatsCurdaySt_read(top, curNode);
		else if(strcmp(name[1], "QutHourStShow") == 0)
			adslStatsQutHourSt_read(top, curNode);
		else if(strcmp(name[1], "RomfileCheck") == 0)
			checkCwmpUploadedRomfile(top, curNode);
		
#endif
		else if(strcmp(name[1],"AutoPVC")==0){
			autoPVCInfo_read(curNode);
		}
#if defined(TCSUPPORT_CT_PON)
#if defined(TCSUPPORT_PONMGR) && (defined(TCSUPPORT_WAN_GPON) || defined(TCSUPPORT_WAN_EPON))	
		else if(strcmp(name[1], "PonPhy") == 0){
			curNode=get_web_node(top, name);
			if(curNode==NULL){
				return FAIL;
			}
			return ponPhyInfo_read(curNode, attr);
		}
		else if (strcmp(name[1], "PonWanStats") == 0){
			curNode = get_web_node(top, name);
			if(curNode==NULL){
				return FAIL;
			}
			return ponWanStatsInfo_read(curNode, attr);
		}
#endif
#if defined(TCSUPPORT_WAN_EPON) && defined(TCSUPPORT_EPON_OAM)
		else if (strcmp(name[1], "Oam") == 0){
			return SUCCESS;
		}
#endif
#endif
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
		else if(strcmp(name[1], "SIMWebAuth") == 0)
			checkSIMWEBAuth(top, curNode);
#endif
			/*krammer chenge for bug 1093
	do not kill WLan node because the WLan counter should not
	be zero when WLan is deactivate*/
#ifndef PURE_BRIDGE
#if defined(TCSUPPORT_WLAN)
	else if(strcmp(name[1],"WLan") == 0){
		curNode=get_web_node(top, name);
		if(curNode==NULL){
			return FAIL;
		}
		if(strlen(name[1])==0){
			return SUCCESS;
		}
		wlanInfo_read(curNode);
	}
#if defined(TCSUPPORT_WLAN_AC)
		 else if(strcmp(name[1], "WLan11ac") == 0){
					curNode=get_web_node(top, name);
					if(curNode==NULL){
							return FAIL;
					}
					if(strlen(name[1])==0){
							return SUCCESS;
					}
					wlan11acInfo_read(curNode);
					doCheck11acHtExtcha(top,IS_AUTO_CHANNEL);
			}
#endif /* TCSUPPORT_WLAN_AC */
#endif
	/*Get default device inforamtion of the cpe from config file /etc/devInf.conf. */
	else if(strcmp(name[1], "devDefInf") == 0) {
		curNode=get_web_node(top, name);
		if(curNode==NULL){
			return FAIL;
		}
		if(strlen(name[1])==0){
			return SUCCESS;
		}
		devDefInf_read(curNode);
	}
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	else if(strcmp(name[1], "SysDevice") == 0){
        curNode=get_web_node(top, name);
        if(curNode==NULL){
                return FAIL;
        }
        if(strlen(name[1])==0){
                return SUCCESS;
        }
        sysDeviceInfo_read(curNode,attr);
     }
#endif
	#if 0
	#ifdef TCSUPPORT_PRODUCTIONLINE
	/*Get default device inforamtion of the cpe from config file /etc/devInf.conf. */
	else if(strcmp(name[1], "proLineInfo") == 0) {
		curNode=get_web_node(top, name);
		if(curNode==NULL){
			return FAIL;
		}
		proLineInfo_read(top,curNode);
	}
	#endif
	#endif
	/*xyzhu: To get stats of wifi/sar Tx/Rx packets/bytes from "ifconfig".
	*/
	else if( (strstr(name[1], "ra") != 0) ||(strstr(name[1], "nas") != 0) ||
		 (strstr(name[1], "eth") != 0) ||(strstr(name[1], "ppp") != 0) ) {

		curNode=get_web_node(top, name);
		if(curNode==NULL){
			return FAIL;
		}
		if(strlen(name[1])==0){
			return SUCCESS;
		}
		ifInfo_read(curNode, name[1]);
	}

	else{
#endif
		//fprintf(stderr,"run before del_sub_node\n");
		del_sub_node(top,name);
		curNode=get_web_node(top, name);
		if(curNode==NULL){
			return FAIL;
		}

		if(strlen(name[1])==0){
			return SUCCESS;
		}
#ifndef PURE_BRIDGE
	}

#endif
	return SUCCESS;
}/*end info_read*/

#if defined(TCSUPPORT_CT_PON)
#if defined(TCSUPPORT_PONMGR) && (defined(TCSUPPORT_WAN_GPON) || defined(TCSUPPORT_WAN_EPON))	
int ponPhyInfo_read(mxml_node_t *curNode, char *attr)
{
	char buf[20] = {0};
	int fecMode = 0;
	enum phy_info{
		Temperature = 0,
		SupplyVoltage,
		TxBiasCurrent,
		RxPower,
		TxPower,
		
		PhyMaxAttr,
	};
	char phy_attr[][32] = {
		{"Temperature"},
		{"SupplyVoltage"},
		{"TxBiasCurrent"},
		{"RxPower"},
		{"TxPower"},
		{""},
	};
	
	MACRO_CMD_STRUCT(PhyTransParameters) getEntry;
	MACRO_CMD_STRUCT(PhyFecConfig) getFecCfgEntry;
	MACRO_CMD_STRUCT(SysLinkConfig) sysLinkCfg ;
	
	/************* PON PHY Trans Param Info *************/
	if(PonApi_PhyTransParameters_Get(&getEntry) != EXEC_OK) {
		tcdbg_printf("PonApi_PhyTransParamInfo_Get failed\n") ;
		return -1 ;
	}
	sprintf(buf, "%u", getEntry.Temperature);
	mxmlElementSetAttr(curNode, phy_attr[Temperature], buf);

	memset(buf, 0 , 20);
	sprintf(buf, "%u", getEntry.Voltage);
	mxmlElementSetAttr(curNode, phy_attr[SupplyVoltage], buf);

	memset(buf, 0 , 20);
	sprintf(buf, "%u", getEntry.TxCurrent);
	mxmlElementSetAttr(curNode, phy_attr[TxBiasCurrent], buf);

	memset(buf, 0 , 20);
	if (getEntry.TxPower == 0)
		sprintf(buf, "%u", 1);
	else
		sprintf(buf, "%u", getEntry.TxPower);
	mxmlElementSetAttr(curNode, phy_attr[TxPower], buf);

	memset(buf, 0 , 20);
	if (getEntry.RxPower == 0)
		sprintf(buf, "%u", 1);
	else
		sprintf(buf, "%u", getEntry.RxPower);
	mxmlElementSetAttr(curNode, phy_attr[RxPower], buf);

	if (PonApi_SysLinkConfig_Get(&sysLinkCfg) != EXEC_OK){
		tcdbg_printf("PonApi_SysLinkConfig_Get failed\n");
		return -1;
	}

	if (sysLinkCfg.LinkStatus == ENUM_SYSTEM_LINK_EPON){
		fecMode = eponApiGetLlidFec(0); // get llid 0's fec status
		if (fecMode == -1) return -1;
		
		memset(buf, 0, 20);
		sprintf(buf, "%u", fecMode); // 0 is disable, 1 is enable
		mxmlElementSetAttr(curNode, "FecStatus", buf);
		
	}else{ // default is gpon
		if(PonApi_PhyFecConfig_Get(&getFecCfgEntry) != EXEC_OK) {
			tcdbg_printf("PonApi_PhyFecConfig_Get failed\n") ;
			return -1 ;
		}

		memset(buf, 0, 20);
		sprintf(buf, "%u", getFecCfgEntry.RxFecStatus);
		mxmlElementSetAttr(curNode, "FecStatus", buf);
	}
	return SUCCESS;
}

int ponWanStatsInfo_read(mxml_node_t *curNode, char *attr)
{
	char buf[20] = {0};
	enum stats_info{
		TxFrameCnt = 0,
		TxFrameLen,
 		TxDropCnt,
 		TxBroadcastCnt,
 		TxMulticastCnt,
 		TxLess64Cnt,
 		TxMore1518Cnt,
 		Tx64Cnt,
 		Tx65To127Cnt,
 		Tx128To255Cnt,
 		Tx256To511Cnt,
 		Tx512To1023Cnt,
 		Tx1024To1518Cnt,
 		RxFrameCnt,
 		RxFrameLen,
 		RxDropCnt,
 		RxBroadcastCnt,
 		RxMulticastCnt,
 		RxCrcCnt,
 		RxFragFameCnt,
 		RxJabberFameCnt,
 		RxLess64Cnt,
 		RxMore1518Cnt,
 		Rx64Cnt,
 		Rx65To127Cnt,
 		Rx128To255Cnt,
 		Rx256To511Cnt,
 		Rx512To1023Cnt,
 		Rx1024To1518Cnt,
		RxHecErrorCnt,
		RxFecErrorCnt,
		WanStatsMaxAttr,
	};
	char stats_attr[][32] = {
		{"TxFrameCnt"},
		{"TxFrameLen"},
 		{"TxDropCnt"},
 		{"TxBroadcastCnt"},
 		{"TxMulticastCnt"},
 		{"TxLess64Cnt"},
 		{"TxMore1518Cnt"},
 		{"Tx64Cnt"},
 		{"Tx65To127Cnt"},
 		{"Tx128To255Cnt"},
 		{"Tx256To511Cnt"},
 		{"Tx512To1023Cnt"},
 		{"Tx1024To1518Cnt"},
 		{"RxFrameCnt"},
 		{"RxFrameLen"},
 		{"RxDropCnt"},
 		{"RxBroadcastCnt"},
 		{"RxMulticastCnt"},
 		{"RxCrcCnt"},
 		{"RxFragFameCnt"},
 		{"RxJabberFameCnt"},
 		{"RxLess64Cnt"},
 		{"RxMore1518Cnt"},
 		{"Rx64Cnt"},
 		{"Rx65To127Cnt"},
 		{"Rx128To255Cnt"},
 		{"Rx256To511Cnt"},
 		{"Rx512To1023Cnt"},
 		{"Rx1024To1518Cnt"},
		{"RxHecErrorCnt"},
		{"RxFecErrorCnt"},
		{""},
	};
	
	MACRO_CMD_STRUCT(PWanCntStats) getEntry;
	
	/************* PON WAN Counter Stats Info *************/
	if(PonApi_PWanCntStats_Get(&getEntry) != EXEC_OK) {
		tcdbg_printf("PonApi_PWanCntStats_Get failed\n\n") ;
		return -1 ;
	}
	
	sprintf(buf, "%u", getEntry.TxFrameCnt);
	mxmlElementSetAttr(curNode, stats_attr[TxFrameCnt], buf);

	memset(buf, 0 , 20);
	sprintf(buf, "%u", getEntry.TxFrameLen);
	mxmlElementSetAttr(curNode, stats_attr[TxFrameLen], buf);

	memset(buf, 0 , 20);
	sprintf(buf, "%u", getEntry.TxDropCnt);
	mxmlElementSetAttr(curNode, stats_attr[TxDropCnt], buf);

	memset(buf, 0 , 20);
	sprintf(buf, "%u", getEntry.TxBroadcastCnt);
	mxmlElementSetAttr(curNode, stats_attr[TxBroadcastCnt], buf);

	memset(buf, 0 , 20);
	sprintf(buf, "%u", getEntry.TxMulticastCnt);
	mxmlElementSetAttr(curNode, stats_attr[TxMulticastCnt], buf);
	
	memset(buf, 0 , 20);
	sprintf(buf, "%u", getEntry.TxLess64Cnt);
	mxmlElementSetAttr(curNode, stats_attr[TxLess64Cnt], buf);
	
	memset(buf, 0 , 20);
	sprintf(buf, "%u", getEntry.TxMore1518Cnt);
	mxmlElementSetAttr(curNode, stats_attr[TxMore1518Cnt], buf);
	
	memset(buf, 0 , 20);
	sprintf(buf, "%u", getEntry.Tx64Cnt);
	mxmlElementSetAttr(curNode, stats_attr[Tx64Cnt], buf);
	
	memset(buf, 0 , 20);
	sprintf(buf, "%u", getEntry.Tx65To127Cnt);
	mxmlElementSetAttr(curNode, stats_attr[Tx65To127Cnt], buf);
	
	memset(buf, 0 , 20);
	sprintf(buf, "%u", getEntry.Tx128To255Cnt);
	mxmlElementSetAttr(curNode, stats_attr[Tx128To255Cnt], buf);
	
	memset(buf, 0 , 20);
	sprintf(buf, "%u", getEntry.Tx256To511Cnt);
	mxmlElementSetAttr(curNode, stats_attr[Tx256To511Cnt], buf);
	
	memset(buf, 0 , 20);
	sprintf(buf, "%u", getEntry.Tx512To1023Cnt);
	mxmlElementSetAttr(curNode, stats_attr[Tx512To1023Cnt], buf);
	
	memset(buf, 0 , 20);
	sprintf(buf, "%u", getEntry.Tx1024To1518Cnt);
	mxmlElementSetAttr(curNode, stats_attr[Tx1024To1518Cnt], buf);
	
	memset(buf, 0 , 20);
	sprintf(buf, "%u", getEntry.RxFrameCnt);
	mxmlElementSetAttr(curNode, stats_attr[RxFrameCnt], buf);
	
	memset(buf, 0 , 20);
	sprintf(buf, "%u", getEntry.RxFrameLen);
	mxmlElementSetAttr(curNode, stats_attr[RxFrameLen], buf);
	
	memset(buf, 0 , 20);
	sprintf(buf, "%u", getEntry.RxDropCnt);
	mxmlElementSetAttr(curNode, stats_attr[RxDropCnt], buf);
	
	memset(buf, 0 , 20);
	sprintf(buf, "%u", getEntry.RxBroadcastCnt);
	mxmlElementSetAttr(curNode, stats_attr[RxBroadcastCnt], buf);
	
	memset(buf, 0 , 20);
	sprintf(buf, "%u", getEntry.RxMulticastCnt);
	mxmlElementSetAttr(curNode, stats_attr[RxMulticastCnt], buf);
	
	memset(buf, 0 , 20);
	sprintf(buf, "%u", getEntry.RxCrcCnt);
	mxmlElementSetAttr(curNode, stats_attr[RxCrcCnt], buf);

	memset(buf, 0 , 20);
	sprintf(buf, "%u", getEntry.RxFragFameCnt);
	mxmlElementSetAttr(curNode, stats_attr[RxFragFameCnt], buf);

	memset(buf, 0 , 20);
	sprintf(buf, "%u", getEntry.RxJabberFameCnt);
	mxmlElementSetAttr(curNode, stats_attr[RxJabberFameCnt], buf);

	memset(buf, 0 , 20);
	sprintf(buf, "%u", getEntry.RxLess64Cnt);
	mxmlElementSetAttr(curNode, stats_attr[RxLess64Cnt], buf);

	memset(buf, 0 , 20);
	sprintf(buf, "%u", getEntry.RxMore1518Cnt);
	mxmlElementSetAttr(curNode, stats_attr[RxMore1518Cnt], buf);

	memset(buf, 0 , 20);
	sprintf(buf, "%u", getEntry.Rx64Cnt);
	mxmlElementSetAttr(curNode, stats_attr[Rx64Cnt], buf);

	memset(buf, 0 , 20);
	sprintf(buf, "%u", getEntry.Rx65To127Cnt);
	mxmlElementSetAttr(curNode, stats_attr[Rx65To127Cnt], buf);

	memset(buf, 0 , 20);
	sprintf(buf, "%u", getEntry.Rx128To255Cnt);
	mxmlElementSetAttr(curNode, stats_attr[Rx128To255Cnt], buf);

	memset(buf, 0 , 20);
	sprintf(buf, "%u", getEntry.Rx256To511Cnt);
	mxmlElementSetAttr(curNode, stats_attr[Rx256To511Cnt], buf);

	memset(buf, 0 , 20);
	sprintf(buf, "%u", getEntry.Rx512To1023Cnt);
	mxmlElementSetAttr(curNode, stats_attr[Rx512To1023Cnt], buf);

	memset(buf, 0 , 20);
	sprintf(buf, "%u", getEntry.Rx1024To1518Cnt);
	mxmlElementSetAttr(curNode, stats_attr[Rx1024To1518Cnt], buf);

#ifdef TCSUPPORT_WAN_GPON
	memset(buf, 0 , 20);
	sprintf(buf, "%u", getEntry.RxHecErrorCnt);
	mxmlElementSetAttr(curNode, stats_attr[RxHecErrorCnt], buf);
#endif

	memset(buf, 0 , 20);
	sprintf(buf, "%u", getEntry.RxFecErrorCnt);
	mxmlElementSetAttr(curNode, stats_attr[RxFecErrorCnt], buf);

	return SUCCESS;
}

#endif
#endif

#ifndef PURE_BRIDGE
/*_____________________________________________________________________________
**      function name: etherInfo_read
**      descriptions:
**            To fill the ether status information into Info cfg node.
**
**      parameters:
**            node:   Put the child cfg node of Info.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**      	fileRead
**      	get_key_vlaue
**      	mxmlElementSetAttr
**      	adslInfo_read
**      	wlanInfo_read
**
**      revision:
**      1. Here 2008/5/19
**____________________________________________________________________________
*/
int
etherInfo_read(mxml_node_t *node){

	char etherInfo_keywd[][17]=
	{
		{"inOctets"},{"inUnicastPkts"},
		{"inMulticastPkts"},{"outOctets"},
		{"outUnicastPkts"},{"outMulticastPkts"},
		{"outErrors"},{"txCollisionCnt"},
		{"txUnderRunCnt"},{"rxCrcErr"},
		{""}
	};

	enum ethInfo_en{
		InOctets=0,
		InUnicastPkts,
		InMulticastPkts,
		OutOctets,
		OutUnicastPkts,
		OutMulticastPkts,
		OutErrors,
		TxCollisionCnt,
		TxUnderRunCnt,
		RxCrcErr,
	};
	char value[32];
	char *buf = NULL;
	int i=0;
	unsigned long int ethStatus[10];
#if defined(TCSUPPORT_CT_E8GUI)
	// get port connection status
	ifInfo_ethcmd_read(node);
#endif

	if(node){
		buf = (char *)malloc(MAX_BUF_SIZE);
		if(buf==NULL){
			perror("malloc fail");
			return FAIL;
		}
		memset(buf,0, MAX_BUF_SIZE);
		//get status information
		fileRead(ETHLINK_PATH, buf, MAX_BUF_SIZE);
		if(strlen(buf)!=0){
			if(!strstr(buf,"Down"))
				mxmlElementSetAttr(node, "status", "Up");
			else
				mxmlElementSetAttr(node, "status", "NoLink");
		}

		memset(buf,0, MAX_BUF_SIZE);
		fileRead(ETHERINFO_PATH, buf, MAX_BUF_SIZE);
		if(strlen(buf)!=0){
			for(i=0; strlen(etherInfo_keywd[i])!=0; i++){
				ethStatus[i]=get_key_vlaue(buf, etherInfo_keywd[i], 16);
				sprintf(value,"%lu", ethStatus[i]);
				mxmlElementSetAttr(node, etherInfo_keywd[i], value);
			}
			/*rx frame=InUnicastPkts+InMulticastPkts */
			sprintf(value,"%lu", ethStatus[InUnicastPkts]+ethStatus[InMulticastPkts]);
			mxmlElementSetAttr(node, ETHINFO_RXFRAM, value);
			/*tx frame=OutUnicastPkts+OutMulticastPkts */
			sprintf(value,"%lu", ethStatus[OutUnicastPkts]+ethStatus[OutMulticastPkts]);
			mxmlElementSetAttr(node, ETHINFO_TXFRAM, value);
		}
		/* 2008 10 1 krammer
		read the ip and MAC in ifconfig ci command
		and set to ip and MAC attr*/
		memset(value,0,sizeof(value));
		mxmlElementSetAttr(node, ETHINFO_IP, getWanInfo(Ip_type, LAN_IF, buf));
		mxmlElementSetAttr(node, ETHINFO_MAC, getWanInfo(Mac_type, LAN_IF, buf));
		/*2008 10 1 krammer*/
		#ifdef IPV6
		mxmlElementSetAttr(node, "isIPv6Supported", "Yes");
		#endif
#if defined(TCSUPPORT_VIR_SERVER)
		mxmlElementSetAttr(node, "isVirServerSupported", "Yes");
#endif
		
		free(buf);
	}
	else{
		return FAIL;
	}
	return SUCCESS;
}/*end etherInfo_read*/
#else
/*_____________________________________________________________________________
**      function name: usbInfo_read
**      descriptions:
**            To fill the usb status information into Info cfg node.
**
**      parameters:
**            node:   Put the child cfg node of Info.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**      	fileRead
**      	get_key_vlaue
**      	mxmlElementSetAttr
**      	adslInfo_read
**      	wlanInfo_read
**
**      revision:
**      1. Here 2008/5/19
**      2. Pork 2009/3/17
**____________________________________________________________________________
*/
int
usbInfo_read(mxml_node_t *node){

	char usbInfo_keywd[][17]=
	{
		{"inPkts"},{"outPkts"},
		{""}
	};

	enum ethInfo_en{
		InPkts=0,
		OutPkts
	};
	char value[32];
	char *buf = NULL;
	int i=0;
	unsigned long int usbStatus[10];

	if(node){
		buf = (char *)malloc(MAX_BUF_SIZE);
		if(buf==NULL){
			perror("malloc fail");
			return FAIL;
		}
		memset(buf,0, MAX_BUF_SIZE);

		fileRead(USBINFO_PATH, buf, MAX_BUF_SIZE);
		if(strlen(buf)!=0){
			for(i=0; strlen(usbInfo_keywd[i])!=0; i++){
				usbStatus[i]=get_key_vlaue(buf, usbInfo_keywd[i], 16);
				sprintf(value,"%lu", usbStatus[i]);
				mxmlElementSetAttr(node, usbInfo_keywd[i], value);
			}
		}
		/* 2008 10 1 krammer
		read the ip and MAC in ifconfig ci command
		and set to ip and MAC attr*/
		memset(value,0,sizeof(value));
		mxmlElementSetAttr(node, USBINFO_IP, getWanInfo(Ip_type, LAN_IF, buf));
		mxmlElementSetAttr(node, USBINFO_MAC, getWanInfo(Mac_type, LAN_IF, buf));
		/*2008 10 1 krammer*/
		free(buf);
	}
	else{
		return FAIL;
	}
	return SUCCESS;
}/*end usb_read*/
#endif
/*_____________________________________________________________________________
**      function name: adslInfo_read
**      descriptions:
**            To fill the Adsl status information into Info cfg node.
**
**      parameters:
**            node:   Put the child cfg node of Info.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**      	fileRead
**      	get_key_vlaue
**      	mxmlElementSetAttr
**      	get_profile_str
**
**      revision:
**      1. Here 2008/5/19
**      2. yzwang 2009/12/15
**____________________________________________________________________________
*/
int
adslInfo_read(mxml_node_t *top, mxml_node_t *node){
	char adslInfo_keywd[][16]=
	{
		{"outPkts"},{"inPkts"},
		{"outDiscards"},{"inDiscards"},
		{"outBytes"},{"inBytes"},//yzwang_20091125
		{""}
	};
	/*
	 * It is used to insert into Infol_Adsl Node
	 * */
	char adslStat_attr[][20]=
	{
		{"lineState"},{"Opmode"},
		{"SNRMarginDown"},{"AttenDown"},
		{"SNRMarginUp"},{"AttenUp"},
		{"DataRateDown"},{"DataRateUp"},
/*pork 20090309 added*/
		{"WanListMode"},
		{"FECDown"},{"FECUp"},
		{"CRCDown"},{"CRCUp"},
		{"HECDown"},{"HECUp"},
		{"ADSLUpTime"},{"ADSLActiveTime"},
		{"PowerDown"},{"PowerUp"},
		/*pork 20090309 added over*/
		{"ATURID"},{"ATUCID"},//yzwang_20091215
		{"AttainUp"},{"AttainDown"},
		//renyu_20100113
		#ifdef CWMP
		{"ShowtimeStart"}, {"TotalStart"},
		{"ATURANSIRev"},{"ATUCANSIRev"},
		{"ATURANSIStd"},{"ATUCANSIStd"},
		{"InterleaveDepth"},
#if defined(TCSUPPORT_CT_ADSLSETTING)
		{"AdslStandard"},
		{"AdslType"},
#endif
		#endif
#if defined(TCSUPPORT_CT)
		{"InterleaveDepthDown"},
#endif
		{""}
	};
	/*
	 * It is used to get attributes from ADSL's /proc
	 * */
	char adslState_keywd[][40]=
	{
		{"ADSL link status: "},{"Opmode: "},
		{"noise margin downstream: "},{"attenuation downstream: "},
		{"noise margin upstream: "},{"attenuation upstream: "},
		{"near-end interleaved channel bit rate: "},{"near-end fast channel bit rate: "},
		{"far-end interleaved channel bit rate: "},{"far-end fast channel bit rate: "},
		/*pork 20090309 added*/
		{"near-end FEC error fast: "},{"near-end FEC error interleaved: "},
		{"far-end FEC error fast: "},{"far-end FEC error interleaved: "},
		{"near-end CRC error fast: "},{"near-end CRC error interleaved: "},
		{"far-end CRC error fast: "},{"far-end CRC error interleaved: "},
		{"near-end HEC error fast: "},{"near-end HEC error interleaved: "},
		{"far-end HEC error fast: "},{"far-end HEC error interleaved: "},
		{"ADSL uptime :"},{"ADSL activetime :"},
		{"output power downstream: "},{"output power upstream: "},
		/*pork 20090309 added over*/
		{"near end itu identification: "},{"far end itu identification: "},//yzwang_20091225
		{"attain upstream: "},{"attain downstream: "},
		//renyu_20100113
		#ifdef CWMP 
		{"ADSL activetime second: "},{"ADSL total ativetime second: "},
		{"ATURANSIRev: "},{"ATUCANSIRev: "},
		{"ATURANSIStd: "},{"ATUCANSIStd: "},
		{"Interleave Depth: "},
		#endif
#if defined(TCSUPPORT_CT_ADSLSETTING)
		{"Adsl Standard: "},
		{"Adsl Type: "},
#endif
#if defined(TCSUPPORT_CT)
		{"Interleave Depth Down:"},
#endif
		{""}
	};

#if defined(TCSUPPORT_CT_WAN_PTM)
		char attrName[24] = {0};
		char vdslInterfaceCfg_keywd[][20]=
		{
			{"CurrentProfiles:"},{"UPBOKLE:"},
			{"TRELLISds:"},{"TRELLISus:"},
			{"ACTSNRMODEds:"}, {"ACTSNRMODEus:"},
			{"ACTUALCE:"},{"SNRMpbds:"},
			{"SNRMpbus:"},
			{""}
		};
	
		char vdslInterfaceCfg_attr[][20]=
		{
			{"CurrentProfiles"},{"UPBOKLE"},
			{"TRELLISds"},{"TRELLISus"},
			{"ACTSNRMODEds"}, {"ACTSNRMODEus"},
			{"ACTUALCE"},{"SNRMpbds"},
			{"SNRMpbus"},
			{""}
		};

#endif
	const char* opModeStr = NULL;

	enum adslStat_en{
		Adsl_stat=0,
		Opmode,
		Noise_m_down,
		Atten_down,
		Noise_m_up,
		Atten_up,
		Near_end_inter,
		Near_end_fast,
		Far_end_inter,
/*pork 20090309 added */
		Far_end_fast,
		Near_end_FEC_fast,
		Near_end_FEC_inter,
		Far_end_FEC_fast,
		Far_end_FEC_inter,
		Near_end_CRC_fast,
		Near_end_CRC_inter,
		Far_end_CRC_fast,
		Far_end_CRC_inter,
		Near_end_HEC_fast,
		Near_end_HEC_inter,
		Far_end_HEC_fast,
		Far_end_HEC_inter,
		Adsl_uptime,
		Adsl_activetime,
		Power_down,
		Power_up,
/*pork 20090309 added over*/
		ATUR_ID,
		ATUC_ID,//yzwang_20091215
		Attain_up,
		Attain_down,
		#ifdef CWMP
		Show_time_start,
		Total_start,
		ATURANSIRev,
		ATUCANSIRev,
		ATURANSIStd,
		ATUCANSIStd,
		InterleaveDepth,
		#endif
		};

	char value[32];
	char *buf = NULL;
#if 0
	int j = 0;
#else
	int stat_attr_index=0;
#endif
	int i=0;
	char fwVer[50]={0};
	char adslRateInfo[2][16];

/*pork 20090309 added*/
	int modeRecord=2;
	int data_loss_rate[2]={0};
	int loss_rate_index=0;
	char tmpLossRateStr[5] = {0};
	mxml_node_t* adslNode;
	const char* modulationType = NULL;
	const char* annexType = NULL;
	char tmpStandardStr[40] = {0};
	int upStat = 1;
	char adslState_keyword_attr[sizeof(adslState_keywd)/(sizeof(char)*40)][64];
/*pork 20090309 added over*/

	if(node){
		buf = (char *)malloc(MAX_BUF_SIZE);
		if(buf==NULL){
			perror("malloc fail");
			return FAIL;
		}
		memset(buf,0, MAX_BUF_SIZE);

		fileRead(ADSLINFO_PATH, buf, MAX_BUF_SIZE-1);
		if(strlen(buf)!=0){
			for(i=0; strlen(adslInfo_keywd[i])!=0; i++){
				sprintf(value,"%lu", get_key_vlaue(buf, adslInfo_keywd[i], 16));
				mxmlElementSetAttr(node, adslInfo_keywd[i], value);
			}
		}
		fileRead(ADSL_FWVER_PATH, fwVer, sizeof(fwVer));
		mxmlElementSetAttr(node, "fwVer", fwVer);
		/*pork 20090320 added */
		memset(adslState_keyword_attr,0,sizeof(adslState_keyword_attr));
		/*get all attribute value via passing keyword to get profile string*/
		for(i=0; strlen(adslState_keywd[i])!=0; i++){
			get_profile_str(adslState_keywd[i],  adslState_keyword_attr[i] , sizeof(adslState_keyword_attr[i]), NO_QMARKS, ADSL_STATE_PATH);
			if((i==0)&&(strcmp(adslState_keyword_attr[i],"up")!=0)){
				upStat = 0;
				break;
			}
		}
		/*After catching all of values corresponding to ADSL's proc keyword,
		 *  Use there attributes to get the attributes' value in Info_Adsl node.
		 *  stat_attr_index is indicated to the adsl_state array.
		 * */
		for(i=0; strlen(adslState_keywd[i])!=0; i++){
			if(upStat == 0){
				/*If the ADSL link isn't up, there is no other information*/
				mxmlElementSetAttr(node, adslStat_attr[stat_attr_index], adslState_keyword_attr[i]);
				break;
			}
			if(i < Near_end_inter){
				mxmlElementSetAttr(node, adslStat_attr[stat_attr_index], adslState_keyword_attr[i]);
				stat_attr_index++;
			}
			else if((i >= Near_end_inter) && (i <= Far_end_fast)){
				/*
					To set DataRateDown &&  DataRateUp attribute-value.
					We need to check the CPE at interleaved/fast channel mode.
					So we use j variable to stroe current attribute index value.
				*/
				memset(adslRateInfo, 0, sizeof(adslRateInfo));
				/*ext: 7645 kbps*/
				sscanf(adslState_keyword_attr[i],"%s %s", adslRateInfo[0], adslRateInfo[1]);
				if(atoi(adslRateInfo[0]) > 0){
					if((i == Near_end_inter) || (i == Far_end_inter)){
						modeRecord --;
					}
					mxmlElementSetAttr(node, adslStat_attr[stat_attr_index],adslState_keyword_attr[i]);
					stat_attr_index++;
				}
				/*pork 20090309 added */
				if(i == Far_end_fast){
					if(modeRecord){
						mxmlElementSetAttr(node, adslStat_attr[stat_attr_index],"1");
						stat_attr_index++;
					}
					else{
						mxmlElementSetAttr(node, adslStat_attr[stat_attr_index],"0");
						stat_attr_index++;
					}
				}
			}
			else if((i >= Near_end_FEC_fast) && (i <= Far_end_HEC_inter)){
				data_loss_rate[loss_rate_index] = atoi(adslState_keyword_attr[i]);
				if(loss_rate_index == 1){
					memset(tmpLossRateStr,0,sizeof(tmpLossRateStr));
					sprintf(tmpLossRateStr,"%d",data_loss_rate[0]+data_loss_rate[1]);
					mxmlElementSetAttr(node, adslStat_attr[stat_attr_index], tmpLossRateStr);
					stat_attr_index++;
				}
				loss_rate_index = (loss_rate_index+1)%2;
			}
			/*pork 20090309 added over*/
			else{
				mxmlElementSetAttr(node, adslStat_attr[stat_attr_index], adslState_keyword_attr[i]);
				stat_attr_index++;
			}
		}

#if defined(TCSUPPORT_CT_WAN_PTM)
		for(i=0; strlen(vdslInterfaceCfg_keywd[i])!=0; i++){
			get_profile_str(vdslInterfaceCfg_keywd[i],  value ,sizeof(value), NO_QMARKS, VDSL_INTERFACE_CFG_PATH);
			//tcdbg_printf("value=%s,vdslInterfaceCfg_keywd[%d]=%s\n",value,i,vdslInterfaceCfg_keywd[i]);
			mxmlElementSetAttr(node, vdslInterfaceCfg_attr[i], value);
		}
#endif

#if 0
		/*pork 20090320 added over*/
		for(i=0; strlen(adslState_keywd[i])!=0; i++){
			memset(adsl_stat,0, sizeof(adsl_stat));
			get_profile_str(adslState_keywd[i],  adsl_stat , sizeof(adsl_stat), NO_QMARKS, ADSL_STATE_PATH);
			if((i==0)&&(strcmp(adsl_stat,"up")!=0)){
				/*If the ADSL link isn't up, there is no other information*/
				mxmlElementSetAttr(node, adslStat_attr[stat_attr_index], adsl_stat);
				/*pork 20090309 added*/
				upStat = 0;
				/*pork 20090309 added over*/
				break;
			}
			if(i < Near_end_inter){
				mxmlElementSetAttr(node, adslStat_attr[stat_attr_index], adsl_stat);
				stat_attr_index++;
			}
			else if((i >= Near_end_inter) && (i <= Far_end_fast)){
				/*
					To set DataRateDown &&  DataRateUp attribute-value.
					We need to check the CPE at interleaved/fast channel mode.
					So we use j variable to stroe current attribute index value.
				*/
				memset(adslRateInfo, 0, sizeof(adslRateInfo));
				/*ext: 7645 kbps*/
				sscanf(adsl_stat,"%s %s", adslRateInfo[0], adslRateInfo[1]);
				if(atoi(adslRateInfo[0]) > 0){
					if((i == Near_end_inter) || (i == Far_end_inter)){
						modeRecord --;
					}
					mxmlElementSetAttr(node, adslStat_attr[stat_attr_index], adsl_stat);
					stat_attr_index++;
				}
				/*pork 20090309 added */
				if(i == Far_end_fast){
					if(modeRecord){
						mxmlElementSetAttr(node, adslStat_attr[stat_attr_index],"1");
						stat_attr_index++;
					}
					else{
						mxmlElementSetAttr(node, adslStat_attr[stat_attr_index],"0");
						stat_attr_index++;
					}
				}
				/*pork 20090309 added over*/
			}
			/*pork 20090309 added*/
			else if((i >= Near_end_FEC_fast) && (i <= Far_end_HEC_inter)){
				data_loss_rate[loss_rate_index] = atoi(adsl_stat);
				if(loss_rate_index == 1){
					memset(tmpLossRateStr,0,sizeof(tmpLossRateStr));
					sprintf(tmpLossRateStr,"%d",data_loss_rate[0]+data_loss_rate[1]);
					mxmlElementSetAttr(node, adslStat_attr[stat_attr_index], tmpLossRateStr);
					stat_attr_index++;
				}
				loss_rate_index = (loss_rate_index+1)%2;
			}
			/*pork 20090309 added over*/
			else{
				mxmlElementSetAttr(node, adslStat_attr[stat_attr_index], adsl_stat);
				stat_attr_index++;
			}
		}
/*pork 20090309 added*/
#endif
		if(upStat == 1){
			adslNode = mxmlFindElement(top, top, ADSLNODE,NULL, NULL,MXML_DESCEND);
			adslNode = mxmlFindElement(adslNode, adslNode,SUB_NODE_NAME,NULL, NULL,MXML_DESCEND);
			if(adslNode != NULL){
				modulationType = mxmlElementGetAttr(adslNode,"MODULATIONTYPE");
				annexType = mxmlElementGetAttr(adslNode,"ANNEXTYPEA");
				if((modulationType!=NULL) && (annexType!=NULL)){
					if(!strcmp(modulationType,MODULATIONTYPE_T1_413)){
						strcpy(tmpStandardStr,MODULATIONTYPE_T1_413);
					}
					else if(!strcmp(modulationType,MODULATIONTYPE_G_DMT)){
						strcpy(tmpStandardStr,MODULATIONTYPE_G_DMT);
					}
					else if(!strcmp(modulationType,MODULATIONTYPE_G_LITE)){
						strcpy(tmpStandardStr,MODULATIONTYPE_G_LITE);
					}
					else if(!strcmp(modulationType,MODULATIONTYPE_ADSL2)){
						if((!strcmp(annexType,"ANNEX A/L")) || (!strcmp(annexType,"ANNEX A/I/J/L/M"))){
							sprintf(tmpStandardStr,"%s (%s)",MTENSTANDARD_ADSL2,ANNEXTYPEA_L);
						}
						else if(!strcmp(annexType,"ANNEX M")){
							sprintf(tmpStandardStr,"%s (%s)",MTENSTANDARD_ADSL2,ANNEXTYPEA_M);
						}
						else{
							sprintf(tmpStandardStr,"%s",MTENSTANDARD_ADSL2);
						}
					}
					else if(!strcmp(modulationType,MODULATIONTYPE_AUTOSYNCUP)){
						opModeStr = mxmlElementGetAttr(node, "Opmode");
						if(!strcmp(opModeStr, "ITU G.993.2(VDSL2)"))
						{
							sprintf(tmpStandardStr, "%s", "ITU G.993.2(VDSL2)");
						}
						else if(!strcmp(opModeStr, "ITU G.993.5(G.Vectoring)"))
						{
							sprintf(tmpStandardStr, "%s", "ITU G.993.5(G.Vectoring)");
						}
						else if(!strcmp(opModeStr, "ITU G.993.5(G.Vectoring),G.998.4(G.INP)")) {
							sprintf(tmpStandardStr, "%s", "ITU G.993.5(G.Vectoring),G.998.4(G.INP)");
						}
						else if(!strcmp(opModeStr, "ITU G.993.2(VDSL2), G.998.4(G.INP)")) {
							sprintf(tmpStandardStr, "%s", "ITU G.993.2(VDSL2), G.998.4(G.INP)");
						}
						else if(!strcmp(opModeStr, "ITU G.992.3(ADSL2) ,G.998.4(G.INP)")) {
							sprintf(tmpStandardStr, "%s", "ITU G.992.3(ADSL2) ,G.998.4(G.INP)");
						}
						else if(!strcmp(opModeStr, "ITU G.992.5(ADSL2PLUS) ,G.998.4(G.INP)")) {
							sprintf(tmpStandardStr, "%s", "ITU G.992.5(ADSL2PLUS) ,G.998.4(G.INP)");
						}
						else if(!strcmp(opModeStr, "ANSI T1.413"))
						{
							sprintf(tmpStandardStr, "%s", MODULATIONTYPE_T1_413);
						}
						else if(!strcmp(opModeStr, "ITU G.992.1(G.DMT)"))
						{
							sprintf(tmpStandardStr, "%s", MODULATIONTYPE_G_DMT);
						}	
						else if(!strcmp(opModeStr, "ITU G.992.2(G.Lite)"))
						{
							sprintf(tmpStandardStr, "%s", MODULATIONTYPE_G_LITE);
						}	
						else if(!strcmp(opModeStr, "ITU G.992.3(ADSL2)"))
						{
							if((!strcmp(annexType,"ANNEX A/L")) || (!strcmp(annexType,"ANNEX A/I/J/L/M"))){
								sprintf(tmpStandardStr,"%s (%s)",MTENSTANDARD_ADSL2,ANNEXTYPEA_L);
							}
							else if(!strcmp(annexType,"ANNEX M")){
								sprintf(tmpStandardStr,"%s (%s)",MTENSTANDARD_ADSL2,ANNEXTYPEA_M);
							}
							else{
								sprintf(tmpStandardStr,"%s",MTENSTANDARD_ADSL2);
							}
						}	
						else if(!strcmp(opModeStr, "ITU G.992.5(ADSL2PLUS)"))
						{
							if((!strcmp(annexType,"ANNEX A/L")) || (!strcmp(annexType,"ANNEX A/I/J/L/M"))){
								sprintf(tmpStandardStr,"%s (%s)",MTENSTANDARD_ADSL2PLUS,ANNEXTYPEA_L);
							}
							else if(!strcmp(annexType,"ANNEX M")){
								sprintf(tmpStandardStr,"%s (%s)",MTENSTANDARD_ADSL2PLUS,ANNEXTYPEA_M);
							}
							else{
								sprintf(tmpStandardStr,"%s",MTENSTANDARD_ADSL2PLUS);
							}
						}	
						else
						{
							if((!strcmp(annexType,"ANNEX A/L")) || (!strcmp(annexType,"ANNEX A/I/J/L/M"))){
								sprintf(tmpStandardStr,"%s (%s)",MTENSTANDARD_ADSL2PLUS,ANNEXTYPEA_L);
							}
							else if(!strcmp(annexType,"ANNEX M")){
								sprintf(tmpStandardStr,"%s (%s)",MTENSTANDARD_ADSL2PLUS,ANNEXTYPEA_M);
							}
							else{
								sprintf(tmpStandardStr,"%s",MTENSTANDARD_ADSL2PLUS);
							}
						}
					}
					else{
						if((!strcmp(annexType,"ANNEX A/L")) || (!strcmp(annexType,"ANNEX A/I/J/L/M"))){
							sprintf(tmpStandardStr,"%s (%s)",MTENSTANDARD_ADSL2PLUS,ANNEXTYPEA_L);
						}
						else if(!strcmp(annexType,"ANNEX M")){
							sprintf(tmpStandardStr,"%s (%s)",MTENSTANDARD_ADSL2PLUS,ANNEXTYPEA_M);
						}
						else{
							sprintf(tmpStandardStr,"%s",MTENSTANDARD_ADSL2PLUS);
						}
					}
				}
				else{
					return FAIL;
				}
			}
			else{
				return FAIL;
			}
			mxmlElementSetAttr(node,"mtenStandard",tmpStandardStr);
		}
/*pork 20090309 added over*/
#if 0
		for(i=0; strlen(adslState_keywd[i])!=0; i++){
			memset(adsl_stat,0, sizeof(adsl_stat));
			get_profile_str(adslState_keywd[i],  adsl_stat , sizeof(adsl_stat), NO_QMARKS, ADSL_STATE_PATH);
			if((i==0)&&(strcmp(adsl_stat,"up")!=0)){
				/*If the ADSL link isn't up, there is no other information*/
				mxmlElementSetAttr(node, adslStat_attr[i], adsl_stat);
				break;
			}
			if(i < Near_end_inter){
				mxmlElementSetAttr(node, adslStat_attr[i], adsl_stat);
				j++;
			}else{
				/*
					To set DataRateDown &&  DataRateUp attribute-value.
					We need to check the CPE at interleaved/fast channel mode.
					So we use j variable to stroe current attribute index value.
				*/
				memset(adslRateInfo, 0, sizeof(adslRateInfo));
				/*ext: 7645 kbps*/
				sscanf(adsl_stat,"%s %s", adslRateInfo[0], adslRateInfo[1]);
				if(atoi(adslRateInfo[0]) > 0){
					mxmlElementSetAttr(node, adslStat_attr[j], adsl_stat);
					j++;
				}
			}
		}
#endif
		free(buf);
	}
	else{
		return FAIL;
	}
	return SUCCESS;
}/*end adslInfo_read*/

#if defined(TCSUPPORT_CT_PON)
int
xponInfo_read(mxml_node_t *top, mxml_node_t *node)
{
	return 0;
}
#endif

#if defined(TCSUPPORT_CT_WAN_PTM)
int
ptmInfo_read(mxml_node_t *top, mxml_node_t *node)
{
	char vdslInfo_keywd[][20]=
	{
		{"outOctets"},{"inOctets"},
		{"outErrors"},{"inErrors"},
		{"outUnicastPkts"},{"inUnicastPkts"},
		{"outDiscards"},{"inDiscards"},
		{"outMulticastPkts"}, {"inMulticastPkts"},
		{""}
	};

	char ptmInfo_keywd[][40]=
	{
		{"BytesSent"},{"BytesReceived"},
		{"ErrorsSent"},{"ErrorsReceived"},
		{"UnicastPacketsSent"},{"UnicastPacketsReceived"},
		{"DiscardPacketsSent"},{"DiscardPacketsReceived"},
		{"MulticastPacketsSent"},{"MulticastPacketsReceived"},		
		{"FramesSent"},{"FramesReceived"},
		{"HWAddr"},
		{""}
	};

	char value[32] = {0};
	char *buf = NULL;
	char *buf1 = NULL;
	unsigned long int counter = 0;
	int i = 0;
	char cmd[64] = {0};
	char hwAddr[20] = {0};
	char *hwAddrPtr = NULL;
	
	if(node)
	{
		buf = (char *)malloc(MAX_BUF_SIZE);
				
		if(buf==NULL) { 
			perror("malloc fail");
			return FAIL;
		}
					
		buf1 = (char *)malloc(MAX_BUF_SIZE);
					
		if(buf1==NULL) 
		{
			free(buf); 
			perror("malloc fail");
			return FAIL;
		}
	
		memset(buf,0, MAX_BUF_SIZE);
		fileRead(VDSLINFO_B0_PATH, buf, MAX_BUF_SIZE-1);
	
		memset(buf1,0, MAX_BUF_SIZE);
		fileRead(VDSLINFO_B1_PATH, buf1, MAX_BUF_SIZE-1);
	
		if(strlen(buf)!=0 || strlen(buf1)!=0)
		{
			for(i=0; i<10; i++){
				counter = 0;
				counter += get_key_vlaue(buf, vdslInfo_keywd[i], 16);
				counter += get_key_vlaue(buf1, vdslInfo_keywd[i], 16);
				sprintf(value,"%lu", counter);
				mxmlElementSetAttr(node, ptmInfo_keywd[i], value);
			}
	
			counter = 0;
			counter += get_key_vlaue(buf, "outUnicastPkts", 16);
			counter += get_key_vlaue(buf, "outMulticastPkts", 16);
			counter += get_key_vlaue(buf1, "outUnicastPkts", 16);
			counter += get_key_vlaue(buf1, "outMulticastPkts", 16);
			sprintf(value,"%lu", counter);
			mxmlElementSetAttr(node, "FramesSent", value);
										  
			counter = 0;
			counter += get_key_vlaue(buf, "inUnicastPkts", 16);
			counter += get_key_vlaue(buf, "inMulticastPkts", 16);
			counter += get_key_vlaue(buf1, "inUnicastPkts", 16);
			counter += get_key_vlaue(buf1, "inMulticastPkts", 16);
			sprintf(value,"%lu", counter);
			mxmlElementSetAttr(node, "FramesReceived", value);

		}
		
		sprintf(cmd,"/sbin/ifconfig %s > %s", "ptm0", IFINFO_PATH);
		system(cmd);

		memset(buf,0, MAX_BUF_SIZE);
		fileRead(IFINFO_PATH, buf, MAX_BUF_SIZE-1);
		if(strstr(buf, "Device not found") != NULL)
		{
			free(buf);
			free(buf1);
			return FAIL;
		}

		if(strlen(buf)!=0){
			hwAddrPtr = strstr(buf, "HWaddr");
			if(hwAddrPtr){
				strncpy(hwAddr, hwAddrPtr + strlen("HWaddr") + 1, 17); //the length of mac addr is 17.
				mxmlElementSetAttr(node, "HWAddr", hwAddr);
			}
		}
			
		free(buf1);		
		free(buf);				

	}else
		return FAIL;
	
	return SUCCESS;
}
#endif

#ifdef CWMP
/*_____________________________________________________________________________
**      function name: adslStatsShow_read
**      descriptions:
**            To fill the Adsl status information into Info cfg node.
**
**      parameters:
**            node:   Put the child cfg node of Info.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**      	fileRead
**      	get_key_vlaue
**      	mxmlElementSetAttr
**      	get_profile_str
**
**      revision:
**      1. renyu 2010/01/28
**____________________________________________________________________________
*/
int
adslStatsShow_read(mxml_node_t *top, mxml_node_t *node){

	char adslStat_attr[][30]=
	{
		{"lineState"},
		{"ReceiveBlock"},{"TransmitBlock"},
		{"CellDelin"},{"LinkRetrain"},
		{"InitErrors"},{"InitTimeouts"},
		{"LossOfFraming"},{"ErroredSecs"},
		{"SeveErrSecs"},
		{"FECDown"},{"FECUp"},
		{"CRCDown"},{"CRCUp"},
		{"HECDown"},
		{""}
	};
	/*
	 * It is used to get attributes from ADSL's /proc
	 * */
	char adslState_keywd[][40]=
	{
		{"ADSL link status: "},
		{"ReceiveBlock: "},{"TransmitBlock: "},
		{"CellDelin: "},{"LinkRetrain: "},
		{"InitErrors: "},{"InitTimeouts: "},
		{"LossOfFraming: "},{"ErroredSecs: "},
		{"SeverelyErroredSecs: "},
		{"near-end FEC error fast: "},{"near-end FEC error interleaved: "},
		{"far-end FEC error fast: "},{"far-end FEC error interleaved: "},
		{"near-end CRC error fast: "},{"near-end CRC error interleaved: "},
		{"far-end CRC error fast: "},{"far-end CRC error interleaved: "},
		{"near-end HEC error fast: "},{"near-end HEC error interleaved: "},
		{""}
	};

	enum adslStat_en{
		Adsl_stat=0,
		ReceiveBlock,
		TransmitBlock,
		CellDelin,
		LinkRetrain,
		InitErrors,
		InitTimeouts,
		LossOfFraming,
		ErroredSecs,
		SeveErrSecs,
		Near_end_FEC_fast,
		Near_end_FEC_inter,
		Far_end_FEC_fast,
		Far_end_FEC_inter,
		Near_end_CRC_fast,
		Near_end_CRC_inter,
		Far_end_CRC_fast,
		Far_end_CRC_inter,
		Near_end_HEC_fast,
		Near_end_HEC_inter,
		};

	char *buf = NULL;

	int stat_attr_index=0;
	int i=0;

	int data_loss_rate[2]={0};
	int loss_rate_index=0;
	char tmpLossRateStr[5] = {0};
	int upStat = 1;
	char adslState_keyword_attr[sizeof(adslState_keywd)/(sizeof(char)*40)][32];

	if(node){
		buf = (char *)malloc(MAX_BUF_SIZE);
		if(buf==NULL){
			perror("malloc fail");
			return FAIL;
		}
		memset(buf,0, MAX_BUF_SIZE);

		memset(adslState_keyword_attr,0,sizeof(adslState_keyword_attr));
		/*get all attribute value via passing keyword to get profile string*/
		for(i=0; strlen(adslState_keywd[i])!=0; i++){
			get_profile_str(adslState_keywd[i],  adslState_keyword_attr[i] , sizeof(adslState_keyword_attr[i]), NO_QMARKS, ADSL_STATE_SHOW_PATH);
			if((i==0)&&(strcmp(adslState_keyword_attr[i],"up")!=0)){
				upStat = 0;
				break;
			}
		}
		/*After catching all of values corresponding to ADSL's proc keyword,
		 *  Use there attributes to get the attributes' value in Info_Adsl node.
		 *  stat_attr_index is indicated to the adsl_state array.
		 * */
		for(i=0; strlen(adslState_keywd[i])!=0; i++){
			if(upStat == 0){
				/*If the ADSL link isn't up, there is no other information*/
				mxmlElementSetAttr(node, adslStat_attr[stat_attr_index], adslState_keyword_attr[i]);
				break;
			}
			if(i < Near_end_FEC_fast){
				mxmlElementSetAttr(node, adslStat_attr[stat_attr_index], adslState_keyword_attr[i]);
				stat_attr_index++;
			}
			else if((i >= Near_end_FEC_fast) && (i <= Near_end_HEC_inter)){
				data_loss_rate[loss_rate_index] = atoi(adslState_keyword_attr[i]);
				if(loss_rate_index == 1){
					memset(tmpLossRateStr,0,sizeof(tmpLossRateStr));
					sprintf(tmpLossRateStr,"%d",data_loss_rate[0]+data_loss_rate[1]);
					mxmlElementSetAttr(node, adslStat_attr[stat_attr_index], tmpLossRateStr);
					stat_attr_index++;
				}
				loss_rate_index = (loss_rate_index+1)%2;
			}
			else{
				mxmlElementSetAttr(node, adslStat_attr[stat_attr_index], adslState_keyword_attr[i]);
				stat_attr_index++;
			}
		}

		free(buf);
	}
	else{
		return FAIL;
	}
	return SUCCESS;
}/*end adslStatsShow_read*/
/*_____________________________________________________________________________
**      function name: adslStatsLast_read
**      descriptions:
**            To fill the Adsl status information into Info cfg node.
**
**      parameters:
**            node:   Put the child cfg node of Info.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**      	fileRead
**      	get_key_vlaue
**      	mxmlElementSetAttr
**      	get_profile_str
**
**      revision:
**      1. renyu 2010/01/28
**____________________________________________________________________________
*/
int
adslStatsLast_read(mxml_node_t *top, mxml_node_t *node){

	char adslStat_attr[][30]=
	{
		{"lineState"},
		{"LastShowtimeFlag"},
		{"LastReceiveBlock"},{"LastTransmitBlock"},
		{"LastCellDelin"},{"LastLinkRetrain"},
		{"LastInitErrors"},{"LastInitTimeouts"},
		{"LastLossOfFraming"},{"LastErroredSecs"},
		{"LastSeveErrSecs"},
		{"LastFECDown"},{"LastFECUp"},
		{"LastCRCDown"},{"LastCRCUp"},
		{"LastHECDown"},
		{""}
	};
	/*
	 * It is used to get attributes from ADSL's /proc
	 * */
	char adslState_keywd[][45]=
	{
		{"ADSL link status: "},
		{"last showtime flag: "},
		{"last ReceiveBlock: "},{"last TransmitBlock: "},
		{"last CellDelin: "},{"last LinkRetrain: "},
		{"last InitErrors: "},{"last InitTimeouts: "},
		{"last LossOfFraming: "},{"last ErroredSecs: "},
		{"last SeverelyErroredSecs: "},
		{"last near-end FEC error fast: "},{"last near-end FEC error interleaved: "},
		{"last far-end FEC error fast: "},{"last far-end FEC error interleaved: "},
		{"last near-end CRC error fast: "},{"last near-end CRC error interleaved: "},
		{"last far-end CRC error fast: "},{"last far-end CRC error interleaved: "},
		{"last near-end HEC error fast: "},{"last near-end HEC error interleaved: "},
		{""}
	};

	enum adslStat_en{
		Adsl_stat=0,
		LastShowtimeFlag,
		Last_ReceiveBlock,
		Last_TransmitBlock,
		Last_CellDelin,
		Last_LinkRetrain,
		Last_InitErrors,
		Last_InitTimeouts,
		Last_LossOfFraming,
		Last_ErroredSecs,
		Last_SeveErrSecs,
		Last_Near_end_FEC_fast,
		Last_Near_end_FEC_inter,
		Last_Far_end_FEC_fast,
		Last_Far_end_FEC_inter,
		Last_Near_end_CRC_fast,
		Last_Near_end_CRC_inter,
		Last_Far_end_CRC_fast,
		Last_Far_end_CRC_inter,
		Last_Near_end_HEC_fast,
		Last_Near_end_HEC_inter,
		};

	char *buf = NULL;

	int stat_attr_index=0;
	int i=0;

	int data_loss_rate[2]={0};
	int loss_rate_index=0;
	char tmpLossRateStr[5] = {0};
	int upStat = 1;
	char adslState_keyword_attr[sizeof(adslState_keywd)/(sizeof(char)*40)][32];

	if(node){
		buf = (char *)malloc(MAX_BUF_SIZE);
		if(buf==NULL){
			perror("malloc fail");
			return FAIL;
		}
		memset(buf,0, MAX_BUF_SIZE);

		memset(adslState_keyword_attr,0,sizeof(adslState_keyword_attr));
		/*get all attribute value via passing keyword to get profile string*/
		for(i=0; strlen(adslState_keywd[i])!=0; i++){
			get_profile_str(adslState_keywd[i],  adslState_keyword_attr[i] , sizeof(adslState_keyword_attr[i]), NO_QMARKS, ADSL_STATE_LAST_PATH);
			if((i==0)&&(strcmp(adslState_keyword_attr[i],"up")!=0)){
				upStat = 0;
				break;
			}
		}
		/*After catching all of values corresponding to ADSL's proc keyword,
		 *  Use there attributes to get the attributes' value in Info_Last node.
		 *  stat_attr_index is indicated to the adsl_last array.
		 * */
		for(i=0; strlen(adslState_keywd[i])!=0; i++){
			if(upStat == 0){
				/*If the ADSL link isn't up, there is no other information*/
				mxmlElementSetAttr(node, adslStat_attr[stat_attr_index], adslState_keyword_attr[i]);
				break;
			}
			if(i < Last_Near_end_FEC_fast){
				mxmlElementSetAttr(node, adslStat_attr[stat_attr_index], adslState_keyword_attr[i]);
				stat_attr_index++;
			}
			else if((i >= Last_Near_end_FEC_fast) && (i <= Last_Near_end_HEC_inter)){
				data_loss_rate[loss_rate_index] = atoi(adslState_keyword_attr[i]);
				if(loss_rate_index == 1){
					memset(tmpLossRateStr,0,sizeof(tmpLossRateStr));
					sprintf(tmpLossRateStr,"%d",data_loss_rate[0]+data_loss_rate[1]);
					mxmlElementSetAttr(node, adslStat_attr[stat_attr_index], tmpLossRateStr);
					stat_attr_index++;
				}
				loss_rate_index = (loss_rate_index+1)%2;
			}
			else{
				mxmlElementSetAttr(node, adslStat_attr[stat_attr_index], adslState_keyword_attr[i]);
				stat_attr_index++;
			}
		}

		free(buf);
	}
	else{
		return FAIL;
	}
	return SUCCESS;
}/*end adslStatsLast_read*/
/*_____________________________________________________________________________
**      function name: adslStatsTotal_read
**      descriptions:
**            To fill the Adsl status information into Info cfg node.
**
**      parameters:
**            node:   Put the child cfg node of Info.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**      	fileRead
**      	get_key_vlaue
**      	mxmlElementSetAttr
**      	get_profile_str
**
**      revision:
**      1. renyu 2010/01/28
**____________________________________________________________________________
*/
int
adslStatsTotal_read(mxml_node_t *top, mxml_node_t *node){

	char adslStat_attr[][30]=
	{
		{"lineState"},
		{"TotalReceiveBlock"},{"TotalTransmitBlock"},
		{"TotalCellDelin"},{"TotalLinkRetrain"},
		{"TotalInitErrors"},{"TotalInitTimeouts"},
		{"TotalLossOfFraming"},{"TotalErroredSecs"},
		{"TotalSeveErrSecs"},
		{"TotalFECDown"},{"TotalFECUp"},
		{"TotalCRCDown"},{"TotalCRCUp"},
		{"TotalHECDown"},//{"TotalHECUp"},
		{""}
	};
	/*
	 * It is used to get attributes from ADSL's /proc
	 * */
	char adslState_keywd[][45]=
	{
		{"ADSL link status: "},
		{"total ReceiveBlock: "},{"total TransmitBlock: "},
		{"total CellDelin: "},{"total LinkRetrain: "},
		{"total InitErrors: "},{"total InitTimeouts: "},
		{"total LossOfFraming: "},{"total ErroredSecs: "},
		{"total SeverelyErroredSecs: "},
		{"total near-end FEC error fast: "},{"total near-end FEC error interleaved: "},
		{"total far-end FEC error fast: "},{"total far-end FEC error interleaved: "},
		{"total near-end CRC error fast: "},{"total near-end CRC error interleaved: "},
		{"total far-end CRC error fast: "},{"total far-end CRC error interleaved: "},
		{"total near-end HEC error fast: "},{"total near-end HEC error interleaved: "},
		//{"total far-end HEC error fast: "},{"total far-end HEC error interleaved: "},
		{""}
	};

	enum adslStat_en{
		Adsl_stat=0,
		Total_ReceiveBlock,
		Total_TransmitBlock,
		Total_CellDelin,
		Total_LinkRetrain,
		Total_InitErrors,
		Total_InitTimeouts,
		Total_LossOfFraming,
		Total_ErroredSecs,
		Total_SeveErrSecs,
		Total_Near_end_FEC_fast,
		Total_Near_end_FEC_inter,
		Total_Far_end_FEC_fast,
		Total_Far_end_FEC_inter,
		Total_Near_end_CRC_fast,
		Total_Near_end_CRC_inter,
		Total_Far_end_CRC_fast,
		Total_Far_end_CRC_inter,
		Total_Near_end_HEC_fast,
		Total_Near_end_HEC_inter,
		//Total_Far_end_HEC_fast,
		//Total_Far_end_HEC_inter,
		};

	char *buf = NULL;

	int stat_attr_index=0;
	int i=0;

	int data_loss_rate[2]={0};
	int loss_rate_index=0;
	char tmpLossRateStr[5] = {0};
	int upStat = 1;
	char adslState_keyword_attr[sizeof(adslState_keywd)/(sizeof(char)*40)][32];

	if(node){
		buf = (char *)malloc(MAX_BUF_SIZE);
		if(buf==NULL){
			perror("malloc fail");
			return FAIL;
		}
		memset(buf,0, MAX_BUF_SIZE);

		memset(adslState_keyword_attr,0,sizeof(adslState_keyword_attr));
		/*get all attribute value via passing keyword to get profile string*/
		for(i=0; strlen(adslState_keywd[i])!=0; i++){
			get_profile_str(adslState_keywd[i],  adslState_keyword_attr[i] , sizeof(adslState_keyword_attr[i]), NO_QMARKS, ADSL_STATE_TOTAL_PATH);
			if((i==0)&&(strcmp(adslState_keyword_attr[i],"up")!=0)){
				upStat = 0;
				break;
			}
		}
		/*After catching all of values corresponding to ADSL's proc keyword,
		 *  Use there attributes to get the attributes' value in Info_Total node.
		 *  stat_attr_index is indicated to the adsl_ Total array.
		 * */
		for(i=0; strlen(adslState_keywd[i])!=0; i++){
			if(upStat == 0){
				/*If the ADSL link isn't up, there is no other information*/
				mxmlElementSetAttr(node, adslStat_attr[stat_attr_index], adslState_keyword_attr[i]);
				break;
			}
			if(i < Total_Near_end_FEC_fast){
				mxmlElementSetAttr(node, adslStat_attr[stat_attr_index], adslState_keyword_attr[i]);
				stat_attr_index++;
			}
			else if((i >= Total_Near_end_FEC_fast) && (i <= Total_Near_end_HEC_inter)){
				data_loss_rate[loss_rate_index] = atoi(adslState_keyword_attr[i]);
				if(loss_rate_index == 1){
					memset(tmpLossRateStr,0,sizeof(tmpLossRateStr));
					sprintf(tmpLossRateStr,"%d",data_loss_rate[0]+data_loss_rate[1]);
					mxmlElementSetAttr(node, adslStat_attr[stat_attr_index], tmpLossRateStr);
					stat_attr_index++;
				}
				loss_rate_index = (loss_rate_index+1)%2;
			}
			else{
				mxmlElementSetAttr(node, adslStat_attr[stat_attr_index], adslState_keyword_attr[i]);
				stat_attr_index++;
			}
		}

		free(buf);
	}
	else{
		return FAIL;
	}
	return SUCCESS;
}/*end adslStatsTotal_read*/
/*_____________________________________________________________________________
**      function name: adslStatsCurdaySt_read
**      descriptions:
**            To fill the Adsl status information into Info cfg node.
**
**      parameters:
**            node:   Put the child cfg node of Info.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**      	fileRead
**      	get_key_vlaue
**      	mxmlElementSetAttr
**      	get_profile_str
**
**      revision:
**      1. renyu 2010/01/28
**____________________________________________________________________________
*/
int
adslStatsCurdaySt_read(mxml_node_t *top, mxml_node_t *node){

	char adslStat_attr[][30]=
	{
		{"CurdayStReceiveBlock"},{"CurdayStTransmitBlock"},
		{"CurdayStCellDelin"},{"CurdayStLinkRetrain"},
		{"CurdayStInitErrors"},{"CurdayStInitTimeouts"},
		{"CurdayStLossOfFraming"},{"CurdayStErroredSecs"},
		{"CurdayStSeveErrSecs"},
		{"CurdayStFECDown"},{"CurdayStFECUp"},
		{"CurdayStCRCDown"},{"CurdayStCRCUp"},
		{"CurdayStHECDown"},
		{""}
	};
	/*
	 * It is used to get attributes from ADSL's /proc
	 * */
	char adslState_keywd[][45]=
	{
		{"curdaySt ReceiveBlock: "},{"curdaySt TransmitBlock: "},
		{"curdaySt CellDelin: "},{"curdaySt LinkRetrain: "},
		{"curdaySt InitErrors: "},{"curdaySt InitTimeouts: "},
		{"curdaySt LossOfFraming: "},{"curdaySt ErroredSecs: "},
		{"curdaySt SeverelyErroredSecs: "},
		{"curdaySt near-end FEC error fast: "},{"curdaySt near-end FEC error interleaved: "},
		{"curdaySt far-end FEC error fast: "},{"curdaySt far-end FEC error interleaved: "},
		{"curdaySt near-end CRC error fast: "},{"curdaySt near-end CRC error interleaved: "},
		{"curdaySt far-end CRC error fast: "},{"curdaySt far-end CRC error interleaved: "},
		{"curdaySt near-end HEC error fast: "},{"curdaySt near-end HEC error interleaved: "},
		{""}
	};

	enum adslStat_en{
		CurdaySt_ReceiveBlock=0,
		CurdaySt_TransmitBlock,
		CurdaySt_CellDelin,
		CurdaySt_LinkRetrain,
		CurdaySt_InitErrors,
		CurdaySt_InitTimeouts,
		CurdaySt_LossOfFraming,
		CurdaySt_ErroredSecs,
		CurdaySt_SeveErrSecs,
		CurdaySt_Near_end_FEC_fast,
		CurdaySt_Near_end_FEC_inter,
		CurdaySt_Far_end_FEC_fast,
		CurdaySt_Far_end_FEC_inter,
		CurdaySt_Near_end_CRC_fast,
		CurdaySt_Near_end_CRC_inter,
		CurdaySt_Far_end_CRC_fast,
		CurdaySt_Far_end_CRC_inter,
		CurdaySt_Near_end_HEC_fast,
		CurdaySt_Near_end_HEC_inter,
		};

	char *buf = NULL;

	int stat_attr_index=0;
	int i=0;

	int data_loss_rate[2]={0};
	int loss_rate_index=0;
	char tmpLossRateStr[5] = {0};
	int upStat = 1;
	char adslState_keyword_attr[sizeof(adslState_keywd)/(sizeof(char)*40)][32];

	if(node){
		buf = (char *)malloc(MAX_BUF_SIZE);
		if(buf==NULL){
			perror("malloc fail");
			return FAIL;
		}
		memset(buf,0, MAX_BUF_SIZE);

		memset(adslState_keyword_attr,0,sizeof(adslState_keyword_attr));
		/*get all attribute value via passing keyword to get profile string*/
		for(i=0; strlen(adslState_keywd[i])!=0; i++)
			get_profile_str(adslState_keywd[i],  adslState_keyword_attr[i] , sizeof(adslState_keyword_attr[i]), NO_QMARKS, ADSL_STATE_CURDAYST_PATH);

		/*After catching all of values corresponding to ADSL's proc keyword,
		 *  Use there attributes to get the attributes' value in Info_CurdaySt node.
		 *  stat_attr_index is indicated to the adsl_state array.
		 * */
		for(i=0; strlen(adslState_keywd[i])!=0; i++){
			if(upStat == 0){
				/*If the ADSL link isn't up, there is no other information*/
				mxmlElementSetAttr(node, adslStat_attr[stat_attr_index], adslState_keyword_attr[i]);
				break;
			}
			if(i < CurdaySt_Near_end_FEC_fast){
				mxmlElementSetAttr(node, adslStat_attr[stat_attr_index], adslState_keyword_attr[i]);
				stat_attr_index++;
			}
			else if((i >= CurdaySt_Near_end_FEC_fast) && (i <= CurdaySt_Near_end_HEC_inter)){
				data_loss_rate[loss_rate_index] = atoi(adslState_keyword_attr[i]);
				if(loss_rate_index == 1){
					memset(tmpLossRateStr,0,sizeof(tmpLossRateStr));
					sprintf(tmpLossRateStr,"%d",data_loss_rate[0]+data_loss_rate[1]);
					mxmlElementSetAttr(node, adslStat_attr[stat_attr_index], tmpLossRateStr);
					stat_attr_index++;
				}
				loss_rate_index = (loss_rate_index+1)%2;
			}
			else{
				mxmlElementSetAttr(node, adslStat_attr[stat_attr_index], adslState_keyword_attr[i]);
				stat_attr_index++;
			}
		}

		free(buf);
	}
	else{
		return FAIL;
	}
	return SUCCESS;
}/*end adslStatsTotal_read*/

/*_____________________________________________________________________________
**      function name: adslStatsQuthourSt_read
**      descriptions:
**            To fill the Adsl status information into Info cfg node.
**
**      parameters:
**            node:   Put the child cfg node of Info.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**      	fileRead
**      	get_key_vlaue
**      	mxmlElementSetAttr
**      	get_profile_str
**
**      revision:
**      1. renyu 2010/01/28
**____________________________________________________________________________
*/
int
adslStatsQutHourSt_read(mxml_node_t *top, mxml_node_t *node){

	char adslStat_attr[][30]=
	{
		{"QutHrStReceiveBlock"},{"QutHrStTransmitBlock"},
		{"QutHrStCellDelin"},{"QutHrStLinkRetrain"},
		{"QutHrStInitErrors"},{"QutHrStInitTimeouts"},
		{"QutHrStLossOfFraming"},{"QutHrStErroredSecs"},
		{"QutHrStSeveErrSecs"},
		{"QutHrStFECDown"},{"QutHrStFECUp"},
		{"QutHrStCRCDown"},{"QutHrStCRCUp"},
		{"QutHrStHECDown"},
		{""}
	};
	/*
	 * It is used to get attributes from ADSL's /proc
	 * */
	char adslState_keywd[][45]=
	{
		{"quthour ReceiveBlock: "},{"quthour TransmitBlock: "},
		{"quthour CellDelin: "},{"quthour LinkRetrain: "},
		{"quthour InitErrors: "},{"quthour InitTimeouts: "},
		{"quthour LossOfFraming: "},{"quthour ErroredSecs: "},
		{"quthour SeverelyErroredSecs: "},
		{"quthour near-end FEC error fast: "},{"quthour near-end FEC error interleaved: "},
		{"quthour far-end FEC error fast: "},{"quthour far-end FEC error interleaved: "},
		{"quthour near-end CRC error fast: "},{"quthour near-end CRC error interleaved: "},
		{"quthour far-end CRC error fast: "},{"quthour far-end CRC error interleaved: "},
		{"quthour near-end HEC error fast: "},{"quthour near-end HEC error interleaved: "},
		{""}
	};

	enum adslStat_en{
		QutHrSt_ReceiveBlock=0,
		QutHrSt_TransmitBlock,
		QutHrSt_CellDelin,
		QutHrSt_LinkRetrain,
		QutHrSt_InitErrors,
		QutHrSt_InitTimeouts,
		QutHrSt_LossOfFraming,
		QutHrSt_ErroredSecs,
		QutHrSt_SeveErrSecs,
		QutHrSt_Near_end_FEC_fast,
		QutHrSt_Near_end_FEC_inter,
		QutHrSt_Far_end_FEC_fast,
		QutHrSt_Far_end_FEC_inter,
		QutHrSt_Near_end_CRC_fast,
		QutHrSt_Near_end_CRC_inter,
		QutHrSt_Far_end_CRC_fast,
		QutHrSt_Far_end_CRC_inter,
		QutHrSt_Near_end_HEC_fast,
		QutHrSt_Near_end_HEC_inter,
		};

	char *buf = NULL;

	int stat_attr_index=0;
	int i=0;

	int data_loss_rate[2]={0};
	int loss_rate_index=0;
	char tmpLossRateStr[5] = {0};
	int upStat = 1;
	char adslState_keyword_attr[sizeof(adslState_keywd)/(sizeof(char)*40)][32];

	if(node){
		buf = (char *)malloc(MAX_BUF_SIZE);
		if(buf==NULL){
			perror("malloc fail");
			return FAIL;
		}
		memset(buf,0, MAX_BUF_SIZE);

		memset(adslState_keyword_attr,0,sizeof(adslState_keyword_attr));
		/*get all attribute value via passing keyword to get profile string*/
		for(i=0; strlen(adslState_keywd[i])!=0; i++)
			get_profile_str(adslState_keywd[i],  adslState_keyword_attr[i] , sizeof(adslState_keyword_attr[i]), NO_QMARKS, ADSL_STATE_QUTHOURST_PATH);

		/*After catching all of values corresponding to ADSL's proc keyword,
		 *  Use there attributes to get the attributes' value in Info_CurdaySt node.
		 *  stat_attr_index is indicated to the adsl_state array.
		 * */
		for(i=0; strlen(adslState_keywd[i])!=0; i++){
			if(upStat == 0){
				/*If the ADSL link isn't up, there is no other information*/
				mxmlElementSetAttr(node, adslStat_attr[stat_attr_index], adslState_keyword_attr[i]);
				break;
			}
			if(i < QutHrSt_Near_end_FEC_fast){
				mxmlElementSetAttr(node, adslStat_attr[stat_attr_index], adslState_keyword_attr[i]);
				stat_attr_index++;
			}
			else if((i >= QutHrSt_Near_end_FEC_fast) && (i <= QutHrSt_Near_end_HEC_inter)){
				data_loss_rate[loss_rate_index] = atoi(adslState_keyword_attr[i]);
				if(loss_rate_index == 1){
					memset(tmpLossRateStr,0,sizeof(tmpLossRateStr));
					sprintf(tmpLossRateStr,"%d",data_loss_rate[0]+data_loss_rate[1]);
					mxmlElementSetAttr(node, adslStat_attr[stat_attr_index], tmpLossRateStr);
					stat_attr_index++;
				}
				loss_rate_index = (loss_rate_index+1)%2;
			}
			else{
				mxmlElementSetAttr(node, adslStat_attr[stat_attr_index], adslState_keyword_attr[i]);
				stat_attr_index++;
			}
		}

		free(buf);
	}
	else{
		return FAIL;
	}
	return SUCCESS;
}/*end adslStatsTotal_read*/

/*_____________________________________________________________________________
**      function name: checkCwmpUploadedRomfile
**      descriptions:
**            verfify romfile uploaded by tr69.
**
**      parameters:
**            node:   Put the child cfg node of Info.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**      	fopen
**      	mxmlLoadFile
**      	mxmlElementSetAttr
**      	verify_romfile
**		mxmlDelete
**
**      revision:
**      1. 2013/06/07
**____________________________________________________________________________
*/
int
checkCwmpUploadedRomfile(mxml_node_t *top, mxml_node_t *node){
	mxml_node_t *tree=NULL;
	FILE *fp=NULL;
#if defined(TCSUPPORT_CT_CWMP_ZIPROMFILE)
	int flag = -1;
#endif

#if defined(TCSUPPORT_PON_ROSTELECOM)
	char file_path[32];
#endif

	if(node)
	{
		mxmlElementSetAttr(node, "Result", "0");
#if defined(TCSUPPORT_CT_CWMP_ZIPROMFILE)
		flag = check_checksum(UPLOAD_ROMFILE_PATH, CT_CWMP_DOWNLOAD_CHKROMFILE_FILE_NAME);
		if( flag <0 )
			return FAIL;

		fp = fopen(CT_CWMP_DOWNLOAD_CHKROMFILE_FILE_NAME, "r");
		if( NULL == fp )
			return FAIL;
#else
		fp = fopen(UPLOAD_ROMFILE_PATH, "r");
		if(fp == NULL){
			return FAIL;
		}
#endif

		tree = mxmlLoadFile(NULL, fp, MXML_TEXT_CALLBACK);
		fclose(fp);
#if defined(TCSUPPORT_CT_CWMP_ZIPROMFILE)
		unlink(CT_CWMP_DOWNLOAD_CHKROMFILE_FILE_NAME);
#endif
		if(tree == NULL){
			return FAIL;
		}
		else
		{
#if defined(TCSUPPORT_PON_ROSTELECOM)
			add_node_if = 2 ;
			//tcdbg_printf("checkCwmpUploadedRomfile: verify romfile for cwmp upgrade\n");
#endif
			if( verify_romfile(tree) == FAIL)
			{
				/*Romfile format is not vaild, so we need to delete it.*/
				unlink(UPLOAD_ROMFILE_PATH);
				mxmlDelete(tree);
				return FAIL;
			}
			/*Delete cfg node tree that is used to verfiy_upload_romfile*/
			mxmlDelete(tree);

#if defined(TCSUPPORT_PON_ROSTELECOM)			
			if( add_node_if == 1 )/*new romfile has been formed*/
			{
				memset(file_path, 0, sizeof(file_path));
				strcpy(file_path,FRAG_ROMFILE_PATH);
				compute_checksum(file_path , "/tmp/boa-temp-ct");
				/*set Result = 2 to inform tr069 using new romfile to upgrade*/
				mxmlElementSetAttr(node, "Result", "2");
			}
			else
			{
#endif
				/*set Result = 1 to inform tr069 checking file succeed, use orig file to upgrade*/
				mxmlElementSetAttr(node, "Result", "1");
#if defined(TCSUPPORT_PON_ROSTELECOM)	
			}
			add_node_if = 0;
#endif
			
		}			
	}
	else
	{
		return FAIL;
	}
	return SUCCESS;		
}/*end checkCwmpUploadedRomfile*/
#endif
/*_____________________________________________________________________________
**      function name: autoPVCInfo_read
**      descriptions:
**            To create autoPVCInfo cfg node and init those attribution value.
**
**      parameters:
**            node: Put the child cfg node of Info.
**
**      global:
**             autopvc_state
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**      	mxmlElementSetAttr
**
**      revision:
**      1. yzwang 2009/12/08
**____________________________________________________________________________
*/
int
autoPVCInfo_read(mxml_node_t *node){
	enum autopvc_info_en{
		State=0,
	};
	char autopvc_attr[][16]=
	{
		{"State"},
		{""},
	};
	
	if(node){
		switch(autopvc_state){
			case AUTOPVC_STATE_STOP:
				mxmlElementSetAttr(node, autopvc_attr[State], "idle");
				break;
			case AUTOPVC_STATE_PROBE_PVC:
			case AUTOPVC_STATE_PROBE_VC_POOL:
				mxmlElementSetAttr(node, autopvc_attr[State], "testing");
				break;
			case AUTOPVC_STATE_FINISH:
				mxmlElementSetAttr(node, autopvc_attr[State], "success");
				break;
			default:
				break;
		}
	}
	else{
		return FAIL;
	}
	return SUCCESS;

}

#ifndef PURE_BRIDGE

#ifdef TCSUPPORT_WLAN_MULTI_WPS
typedef struct GNU_PACKED _WSC_PEER_DEV_INFO {
		unsigned char WscPeerDeviceName[32];
		unsigned char WscPeerManufacturer[64];
		unsigned char WscPeerModelName[32];
		unsigned char WscPeerModelNumber[32];
		unsigned char WscPeerSerialNumber[32];
		unsigned char WscPeerMAC[6];
} WSC_PEER_DEV_INFO;
#define RT_OID_WSC_QUERY_PEER_INFO_ON_RUNNING		0x075C
#endif

#ifdef BBUTOWBU
/*_____________________________________________________________________________
**      function name: UpdateChannel
**      descriptions:
**            To update current channel with Ioctl
**
**      parameters:
**            outchannel:   get the driver's current channel
**
**      global:
**             None
**
**      return:
**            void
**      call:
**
**      revision:
**      1. xyyou 2011/2/14
**____________________________________________________________________________
*/

void UpdateChannel(char * outchannel){

	struct iwreq wrq;
	int ret=0;
	char data[16];
	int socket_id;
	int defaultchannel=6;
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);
	if(socket_id==-1){
		printf("==>UpdateChannel: Create socket fail\n");
		sprintf(outchannel,"%d",defaultchannel);
		return;
	}

	strcpy(wrq.ifr_name, "ra0");
       wrq.u.data.length = sizeof(data);
       wrq.u.data.pointer = data;
       wrq.u.data.flags = 0;
       ret=ioctl(socket_id, SIOCGIWFREQ , &wrq);

	if ( ret != 0 ) {
		printf("==>UpdateChannel: ioctl open fail\n");
    		close(socket_id);
		sprintf(outchannel,"%d",defaultchannel);
		return;
	}
	sprintf(outchannel,"%d",wrq.u.freq.m);
	close(socket_id);
	return;

}
#ifdef WSC_AP_SUPPORT
/*_____________________________________________________________________________
**      function name: UpdatePinCode
**      descriptions:
**            To update current PinCode with Ioctl
**
**      parameters:
**            outPinCode:   get the driver's current PinCode
**
**      global:
**             None
**
**      return:
**            void
**      call:
**
**      revision:
**      1. xyyou 2011/2/14
**____________________________________________________________________________
*/

void UpdatePinCode(char * outPinCode){
	
	struct iwreq wrq;
	int ret=0;
	char data[16];
	int socket_id;
	int defaultPinCode=0;
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);
	if(socket_id==-1){
		printf("==>UpdatePinCode: Create socket fail\n");
		sprintf(outPinCode,"%d",defaultPinCode);
		return;
	}

	strcpy(wrq.ifr_name, "ra0");
       wrq.u.data.length = sizeof(data);
       wrq.u.data.pointer = data;
       wrq.u.data.flags = RT_OID_WSC_PIN_CODE;
       ret=ioctl(socket_id, RT_PRIV_IOCTL , &wrq);

	if ( ret != 0 ) {
		printf("==>UpdatePinCode: ioctl open fail\n");
    		close(socket_id);
		sprintf(outPinCode,"%d",defaultPinCode);
		return;
	}
	sprintf(outPinCode,"%08u",*((unsigned int *)(data)));
	close(socket_id);
	return;
	
}
/*_____________________________________________________________________________
**      function name: UpdateWpsStatus
**      descriptions:
**            To update current Wps status with Ioctl
**
**      parameters:
**            outPinCode:   get the driver's current wps status
**
**      global:
**             None
**
**      return:
**            void
**      call:
**
**      revision:
**      1. xyyou 2011/2/14
**____________________________________________________________________________
*/
#ifdef TCSUPPORT_WLAN_MULTI_WPS

void UpdateWpsStatus(int * outWpsStatus, char *outWpsMac, int i){
	
	struct iwreq wrq;
	int ret=0;
	char data[20];
	WSC_PEER_DEV_INFO datainfo = {0};
	int socket_id;
	int defaultWpsStatus=0;
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);
	if(socket_id==-1){
		printf("==>UpdateWpsStatus: Create socket fail\n");
		*outWpsStatus=defaultWpsStatus;
		outWpsMac[0] = '\0';
		return;
	}

	sprintf(wrq.ifr_name, "ra%d",i);
       wrq.u.data.length = sizeof(data);
       wrq.u.data.pointer = data;
       wrq.u.data.flags = RT_OID_WSC_QUERY_STATUS;
       ret=ioctl(socket_id, RT_PRIV_IOCTL , &wrq);

	if ( ret != 0 ) {
		printf("==>UpdateWpsStatus: ioctl open fail\n");
    		close(socket_id);
		*outWpsStatus=defaultWpsStatus;
		outWpsMac[0] = '\0';
		return;
	}
	*outWpsStatus=*((int *)data);

	/* get the mac when in progress or configured*/
	if(*outWpsStatus > 2){
		wrq.u.data.length = sizeof(datainfo);
		wrq.u.data.pointer = &datainfo;
		wrq.u.data.flags = RT_OID_WSC_QUERY_PEER_INFO_ON_RUNNING;
		ret=ioctl(socket_id, RT_PRIV_IOCTL , &wrq);

		if ( ret != 0 ) {
			printf("==>UpdateWpsMac: ioctl open fail:%d\n",ret);
	    	close(socket_id);
			outWpsMac[0] = '\0';
			return ;
		}
		sprintf(data,"%02X:%02X:%02X:%02X:%02X:%02X",
			datainfo.WscPeerMAC[0],datainfo.WscPeerMAC[1],datainfo.WscPeerMAC[2],datainfo.WscPeerMAC[3],datainfo.WscPeerMAC[4],datainfo.WscPeerMAC[5]);
		memcpy(outWpsMac,data, 18);
	}
	close(socket_id);
	return;
	
}
#else
void UpdateWpsStatus(int * outWpsStatus){
	
	struct iwreq wrq;
	int ret=0;
	char data[16];
	int socket_id;
	int defaultWpsStatus=0;
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);
	if(socket_id==-1){
		printf("==>UpdateWpsStatus: Create socket fail\n");
		*outWpsStatus=defaultWpsStatus;
		return;
	}

	strcpy(wrq.ifr_name, "ra0");
       wrq.u.data.length = sizeof(data);
       wrq.u.data.pointer = data;
       wrq.u.data.flags = RT_OID_WSC_QUERY_STATUS;
       ret=ioctl(socket_id, RT_PRIV_IOCTL , &wrq);

	if ( ret != 0 ) {
		printf("==>UpdateWpsStatus: ioctl open fail\n");
    		close(socket_id);
		*outWpsStatus=defaultWpsStatus;
		return;
	}
	*outWpsStatus=*((int *)data);

	close(socket_id);
	return;
	
}

#endif
#endif
#endif
#if defined(TCSUPPORT_WLAN_AC)
/*_____________________________________________________________________________
**      function name: UpdateChannelAC
**      descriptions:
**            To update current channel with Ioctl
**
**      parameters:
**            outchannel:   get the driver's current channel
**
**      global:
**             None
**
**      return:
**            void
**      call:
**
**      revision:
**      1. evan 2013/8/5
**____________________________________________________________________________
*/

void UpdateChannelAC(char * outchannel){

	struct iwreq wrq;
	int ret=0;
	char data[16];
	int socket_id;
	int defaultchannel=6;
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);
	if(socket_id==-1){
		printf("==>UpdateChannelAC: Create socket fail\n");
		sprintf(outchannel,"%d",defaultchannel);
		return;
	}

	strcpy(wrq.ifr_name, "rai0");
       wrq.u.data.length = sizeof(data);
       wrq.u.data.pointer = data;
       wrq.u.data.flags = 0;
       ret=ioctl(socket_id, SIOCGIWFREQ , &wrq);

	if ( ret != 0 ) {
		printf("==>UpdateChannelAC: ioctl open fail\n");
    		close(socket_id);
		sprintf(outchannel,"%d",defaultchannel);
		return;
	}
	sprintf(outchannel,"%d",wrq.u.freq.m);
	close(socket_id);
	return;

}
#ifdef WSC_AP_SUPPORT
/*_____________________________________________________________________________
**      function name: UpdatePinCodeAC
**      descriptions:
**            To update current PinCode with Ioctl
**
**      parameters:
**            outPinCode:   get the driver's current PinCode
**
**      global:
**             None
**
**      return:
**            void
**      call:
**
**      revision:
**      1. evan 2013/8/5
**____________________________________________________________________________
*/

void UpdatePinCodeAC(char * outPinCode){
	
	struct iwreq wrq;
	int ret=0;
	char data[16];
	int socket_id;
	int defaultPinCode=0;
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);
	if(socket_id==-1){
		printf("==>UpdatePinCodeAC: Create socket fail\n");
		sprintf(outPinCode,"%d",defaultPinCode);
		return;
	}

	strcpy(wrq.ifr_name, "rai0");
       wrq.u.data.length = sizeof(data);
       wrq.u.data.pointer = data;
       wrq.u.data.flags = RT_OID_WSC_PIN_CODE;
       ret=ioctl(socket_id, RT_PRIV_IOCTL , &wrq);

	if ( ret != 0 ) {
		printf("==>UpdatePinCodeAC: ioctl open fail\n");
    		close(socket_id);
		sprintf(outPinCode,"%d",defaultPinCode);
		return;
	}
	sprintf(outPinCode,"%08u",*((unsigned int *)(data)));
	close(socket_id);
	return;
	
}
/*_____________________________________________________________________________
**      function name: UpdateWpsStatusAC
**      descriptions:
**            To update current Wps status with Ioctl
**
**      parameters:
**            outPinCode:   get the driver's current wps status
**
**      global:
**             None
**
**      return:
**            void
**      call:
**
**      revision:
**      1. evan 2013/8/5
**____________________________________________________________________________
*/

#ifdef TCSUPPORT_WLAN_MULTI_WPS

void UpdateWpsStatusAC(int * outWpsStatus, char *outWpsMac, int i){
	
	struct iwreq wrq;
	int ret=0;
	char data[20];
	WSC_PEER_DEV_INFO datainfo = {0};
	int socket_id;
	int defaultWpsStatus=0;
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);
	
	
	if(socket_id==-1){
		printf("==>UpdateWpsStatus: Create socket fail\n");
		*outWpsStatus=defaultWpsStatus;
		outWpsMac[0] = '\0';
		return;
	}

	sprintf(wrq.ifr_name, "rai%d",i);
	wrq.u.data.length = sizeof(data);
	wrq.u.data.pointer = data;
	wrq.u.data.flags = RT_OID_WSC_QUERY_STATUS;
	ret=ioctl(socket_id, RT_PRIV_IOCTL , &wrq);
	
	if ( ret != 0 ) {
		printf("==>UpdateWpsStatus: ioctl open fail\n");
    	close(socket_id);
		*outWpsStatus=defaultWpsStatus;
		outWpsMac[0] = '\0';
		return;
	}
	*outWpsStatus=*((int *)data);

	/* get the mac when in progress or configured*/
	if(*outWpsStatus > 2){
		wrq.u.data.length = sizeof(datainfo);
		wrq.u.data.pointer = &datainfo;
		wrq.u.data.flags = RT_OID_WSC_QUERY_PEER_INFO_ON_RUNNING;
		ret=ioctl(socket_id, RT_PRIV_IOCTL , &wrq);

		if ( ret != 0 ) {
			printf("==>UpdateWpsMac: ioctl open fail:%d\n",ret);
	    	close(socket_id);
			outWpsMac[0] = '\0';
			return ;
		}
		
		sprintf(data,"%02X:%02X:%02X:%02X:%02X:%02X",
			datainfo.WscPeerMAC[0],datainfo.WscPeerMAC[1],datainfo.WscPeerMAC[2],datainfo.WscPeerMAC[3],datainfo.WscPeerMAC[4],datainfo.WscPeerMAC[5]);
		memcpy(outWpsMac,data, 18);
	}
	close(socket_id);
	return;
}
#else
void UpdateWpsStatusAC(int * outWpsStatus){
	
	struct iwreq wrq;
	int ret=0;
	char data[16];
	int socket_id;
	int defaultWpsStatus=0;
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);
	if(socket_id==-1){
		printf("==>UpdateWpsStatusAC: Create socket fail\n");
		*outWpsStatus=defaultWpsStatus;
		return;
	}

	strcpy(wrq.ifr_name, "rai0");
       wrq.u.data.length = sizeof(data);
       wrq.u.data.pointer = data;
       wrq.u.data.flags = RT_OID_WSC_QUERY_STATUS;
       ret=ioctl(socket_id, RT_PRIV_IOCTL , &wrq);

	if ( ret != 0 ) {
		printf("==>UpdateWpsStatusAC: ioctl open fail\n");
    		close(socket_id);
		*outWpsStatus=defaultWpsStatus;
		return;
	}
	*outWpsStatus=*((int *)data);

	close(socket_id);
	return;
	
}
#endif
#endif /* WSC_AP_SUPPORT */
#endif /* TCSUPPORT_WLAN_AC */

/*_____________________________________________________________________________
**      function name: wlanInfo_read
**      descriptions:
**            To fill the wlan status information into Info cfg node.
**
**      parameters:
**            node:   Put the child cfg node of Info.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**      	fileRead
**      	get_key_vlaue
**      	check_wireless_card
**
**      revision:
**      1. Here 2008/5/19
**____________________________________________________________________________
*/
int
wlanInfo_read(mxml_node_t *node){

	char wlanInfo_keywd[][32]=
	{
		{"Tx success  "},{"Tx fail"},
		{"Rx success"},{"Rx with CRC"},
		{"Rx drop"},{"Channel"},
	#ifdef WSC_AP_SUPPORT//add by xyyou
		{"Enrollee PinCode(ra0)"},
	#ifdef TCSUPPORT_WLAN_MULTI_WPS	
		{"WPS Query Status(ra0)"},
		{"WPS Query Status(ra1)"},	
		{"WPS Query Status(ra2)"},
		{"WPS Query Status(ra3)"},		
		{"WPS Wsc2MinsTimerRunning(ra0)"},
		{"WPS Wsc2MinsTimerRunning(ra1)"},
		{"WPS Wsc2MinsTimerRunning(ra2)"},
		{"WPS Wsc2MinsTimerRunning(ra3)"},
	#else
		{"WPS Query Status(ra0)"},
		{"WPS Wsc2MinsTimerRunning(ra0)"},
	#endif
	#endif
		{""}
	};
#if 0
	char wlanInfo_attr[][17]=
	{
		{"wlanTxFrames"},{"wlanTxErrFrames"},
		{"wlanTxDropFrames"},{"wlanRxFrames"},
		{"wlanRxErrFrames"},{"wlanRxDropFrames"},
		{"isExist"},{""}
	};
#endif

	char wlanInfo_attr[][32]=
	{
		{"wlanTxFrames"},{"wlanTxErrFrames"},
		{"wlanTxDropFrames"},{"wlanRxFrames"},
		{"wlanRxErrFrames"},{"wlanRxDropFrames"},
		{"CurrentChannel"},//add by xyyou to support AutoChannel
	#ifdef WSC_AP_SUPPORT//add by xyyou
		{"wlanSelfPinCode"},
	#ifdef TCSUPPORT_WLAN_MULTI_WPS		
		{"wlanWPSStatus_0"},{"wlanWPSStatus_1"},{"wlanWPSStatus_2"},{"wlanWPSStatus_3"},
		{"wlanWPStimerRunning_0"},{"wlanWPStimerRunning_1"},{"wlanWPStimerRunning_2"},{"wlanWPStimerRunning_3"},
	#else	
		{"wlanWPSStatus"},{"wlanWPStimerRunning"},//add by xyyou to support wps
	#endif	
	#endif
		{""}
	};

	enum wlanInfo_en{
		TxSuccess=0,
		TxFail,
		RxSuccess,
		RxCRC,
		RxDrop,
		Channel,
	#ifdef WSC_AP_SUPPORT//add by xyyou
		PinCode,
	#ifdef TCSUPPORT_WLAN_MULTI_WPS		
		WPSStatus_0,
		WPSStatus_1,
		WPSStatus_2,	
		WPSStatus_3,
		WPSTimerRunning_0,
		WPSTimerRunning_1,
		WPSTimerRunning_2,
		WPSTimerRunning_3,
	#else	
		WPSStatus,
		WPSTimerRunning,
	#endif
	#endif
	};

	char value[16];
	char macvalue[20] = {0};
	char *buf = NULL;
	int i=0;
	#ifdef TCSUPPORT_WLAN_MULTI_WPS		
	unsigned long int wlanStatus[14];
	#else
	unsigned long int wlanStatus[8];
	#endif
	char cmd[64]={0};
	#ifdef WSC_AP_SUPPORT//add by xyyou
	int wpsStatus=0;
	#endif

	if(node){
		buf = (char *)malloc(MAX_BUF_SIZE);
		if(buf==NULL){
			perror("malloc fail");
			return FAIL;
		}
		memset(buf,0, MAX_BUF_SIZE);

		/*krammer change : check ifconfig ra0 for isExist attr*/
		check_wireless_card(value);
		mxmlElementSetAttr(node, ISEXIST, value);
#if defined(TCSUPPORT_C1_CUC)
		mxmlElementSetAttr(node, "isCUCSupport","Yes");
#endif
#if defined(TCSUPPORT_TRUE_LANDING_PAGE)
		mxmlElementSetAttr(node, "isLandingPageSupport","Yes");
#endif

		/**/

		/*
			Create a tmp file to get current wlan traffic statistics information.
			ext: /userfs/bin/iwpriv ra0 stat  > /tmp/wlan_stats
		*/
		sprintf(cmd,"%s > %s", GET_WLAN_INFO_CMD, WLANINFO_PATH);
		system(cmd);

		fileRead(WLANINFO_PATH, buf, MAX_BUF_SIZE);
#if defined(TCSUPPORT_CT_E8GUI)
		ifInfo_read(node, /*ifra0*/"ra0");
#endif
		if(strlen(buf)!=0){
			for(i=0; strlen(wlanInfo_keywd[i])!=0; i++){
				wlanStatus[i]=get_key_vlaue(buf, wlanInfo_keywd[i], 10);
			}

			for(i=0; strlen(wlanInfo_attr[i])!=0; i++){
				switch(i){
					case 0:/*wlanTxFrames*/
						sprintf(value,"%lu", wlanStatus[TxSuccess]);
						break;
					case 1:/*wlanTxErrFrames*/
					case 2:/*wlanTxDropFrames*/
						sprintf(value,"%lu", wlanStatus[TxFail]);
						break;
					case 3:/*wlanRxFrames*/
						sprintf(value,"%lu", wlanStatus[RxSuccess]);
						break;
					case 4:/*wlanRxErrFrames*/
						sprintf(value,"%lu", wlanStatus[RxCRC]);
						break;
					case 5:/*wlanRxDropFrames*/
						sprintf(value,"%lu", wlanStatus[RxCRC]+wlanStatus[RxDrop]);
						break;
					case 6:/*CurrentChannel */
						//sprintf(value,"%d", wlanStatus[Channel]);
					#ifndef BBUTOWBU
						sprintf(value,"%lu", wlanStatus[Channel]);
					#else
						UpdateChannel(value);
					#endif

						break;
				#ifdef WSC_AP_SUPPORT//add by xyyou
					case 7:/*PinCode */
#ifndef BBUTOWBU
						sprintf(value,"%08lu", wlanStatus[PinCode]);
#else
						UpdatePinCode(value);
#endif
						
						break;

#ifdef TCSUPPORT_WLAN_MULTI_WPS
					case 8:/*wpsstatus,ssid0*/
					case 9:/*wpsstatus,ssid1*/
					case 10:/*wpsstatus,ssid2*/
					case 11:/*wpsstatus,ssid3*/
						UpdateWpsStatus(&wpsStatus, macvalue, i-8);
						if(wpsStatus==0||wpsStatus==1){
							//sprintf(value, "%s", "Not Used");
						//}else if(wpsStatus==1){
							   sprintf(value, "%s", "Idle");					
						}else if(wpsStatus==2){
							sprintf(value, "%s", "WPS Process Fail");
						}else if((wpsStatus>2) && (wpsStatus<34)){
							sprintf(value, "%s", "In progress");
						}else{
							sprintf(value, "%s", "Configured");
						}
						if( macvalue[0]){
							mxmlElementSetAttr(node, "wpsmac", macvalue);
						}
						break;
					case 12:/*wps timer running-ra0*/
						sprintf(value, "%d", wlanStatus[WPSTimerRunning_0]);
						break;
					case 13:/*wps timer running-ra1*/
						sprintf(value, "%d", wlanStatus[WPSTimerRunning_1]);
						break;
					case 14:/*wps timer running-ra2*/
						sprintf(value, "%d", wlanStatus[WPSTimerRunning_2]);
						break;
					case 15:/*wps timer running-ra3*/
						sprintf(value, "%d", wlanStatus[WPSTimerRunning_3]);
						break;
#else
					case 8:/*wpsstatus*/
#ifndef BBUTOWBU
						wpsStatus = wlanStatus[WPSStatus];
#else
						UpdateWpsStatus(&wpsStatus);
#endif
						if(wpsStatus==0||wpsStatus==1){
							//sprintf(value, "%s", "Not Used");
						//}else if(wpsStatus==1){
						       sprintf(value, "%s", "Idle");					
						}else if(wpsStatus==2){
							sprintf(value, "%s", "WPS Process Fail");
						}else if((wpsStatus>2) && (wpsStatus<34)){
							sprintf(value, "%s", "In progress");
						}else{
							sprintf(value, "%s", "Configured");
						}
						break;
					case 9:/*wps timer running*/
						sprintf(value, "%d", wlanStatus[WPSTimerRunning]);
						break;
				#endif
				#endif
#if 0
					case 6:/*isExist*/
						check_wireless_card(value);
						break;
#endif
					default:
						memset(value, 0, sizeof(value));
						break;
				}
				mxmlElementSetAttr(node, wlanInfo_attr[i], value);
			}/*end for(i=0; strlen(wlanInfo_attr...*/
#ifdef WSC_AP_SUPPORT
			mxmlElementSetAttr(node, "isWPSSupported", "YES");
#endif
		}/*end if(strlen(buf)!=0)*/
		unlink(WLANINFO_PATH);
		free(buf);
#if defined(TCSUPPORT_WLAN_MAXSTANUM_GUI)
		mxmlElementSetAttr(node, "maxStaNumSupported", MAX_STA_NUM);
#endif
		
	}/*end if(node)*/
	else{
		return FAIL;
	}
	return SUCCESS;
}/*end wlanInfo_read*/

#if defined(TCSUPPORT_WLAN_AC)
/*_____________________________________________________________________________
**      function name: wlan11acInfo_read
**      descriptions:
**            To fill the wlan status information into Info cfg node.
**
**      parameters:
**            node:   Put the child cfg node of Info.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**      	fileRead
**      	get_key_vlaue
**      	check_wireless_ac_card
**
**      revision:
**      1. evan.jiang 2013/08/01
**____________________________________________________________________________
*/
int
wlan11acInfo_read(mxml_node_t *node){

	enum wlanInfo_en{
		TxSuccess=0,
		TxFail,
		RxSuccess,
		RxCRC,
		RxDrop,
		Channel,
		RetryCount,
		RTSSuccess,
		RTSFail,
		SNR0,
		SNR1,
		SNR2,
	#ifdef WSC_AP_SUPPORT//add by xyyou
		PinCode,
	#ifdef TCSUPPORT_WLAN_MULTI_WPS		
		WPSStatus_0,
		WPSStatus_1,
		WPSStatus_2,	
		WPSStatus_3,
		WPSTimerRunning_0,
		WPSTimerRunning_1,
		WPSTimerRunning_2,
		WPSTimerRunning_3,
	#else	
		WPSStatus,
		WPSTimerRunning,
	#endif
	#endif
        RxPackets,
        TxPackets,
        RxBytes,
        TxBytes,
        
        /* used to define the size of wlanStatus[],
         * so don't enumerate any variables behind it */
        LastOne
	};
	char wlan11acInfo_keywd[][32]=
	{
			{"Tx success  "},{"Tx fail"},
			{"Rx success"},{"Rx with CRC"},
			{"Rx drop"},{"Channel"},
			{"Tx retry count"},{"RTS Success Rcv CTS"},
			{"RTS Fail Rcv CTS"},
			{"RSSI -A"},{"RSSI -B"},{"RSSI -C"},
    #ifdef WSC_AP_SUPPORT/*add by xyyou*/
			{"Enrollee PinCode(rai0)"},
    	#ifdef TCSUPPORT_WLAN_MULTI_WPS	
				{"WPS Query Status(rai0)"},
				{"WPS Query Status(rai1)"}, 
				{"WPS Query Status(rai2)"},
				{"WPS Query Status(rai3)"}, 	
				{"WPS Wsc2MinsTimerRunning(rai0)"},
				{"WPS Wsc2MinsTimerRunning(rai1)"},
				{"WPS Wsc2MinsTimerRunning(rai2)"},
				{"WPS Wsc2MinsTimerRunning(rai3)"},
		#else
			{"WPS Query Status(rai0)"},
			{"WPS Wsc2MinsTimerRunning(rai0)"},
        #endif
    #endif
			{"Packets Received"},
			{"Packets Sent"},
			{"Bytes Received"},
			{"Bytes Sent"}, 		 
			{""}
	};
	char wlanInfo_attr_ac[][32]=
	{
			{"wlanTxFrames"},{"wlanTxErrFrames"},
			{"wlanTxDropFrames"},{"wlanRxFrames"},
			{"wlanRxErrFrames"},{"wlanRxDropFrames"},
			{"CurrentChannel"},//add by xyyou to support AutoChannel
	
			{"wlanTxRetryCount"},{"wlanRTSSuccessRcvCTS"},
			{"wlanRTSFailRcvCTS"},
			{"wlanSNR0"},{"wlanSNR1"},{"wlanSNR2"},
	
    #ifdef WSC_AP_SUPPORT//add by xyyou
			{"wlanSelfPinCode"},
    #ifdef TCSUPPORT_WLAN_MULTI_WPS		
			{"wlanWPSStatus_0"},{"wlanWPSStatus_1"},{"wlanWPSStatus_2"},{"wlanWPSStatus_3"},
			{"wlanWPStimerRunning_0"},{"wlanWPStimerRunning_1"},{"wlanWPStimerRunning_2"},{"wlanWPStimerRunning_3"},
	#else
			{"wlanWPSStatus"},{"wlanWPStimerRunning"},//add by xyyou to support wps
    #endif
    #endif
	#ifdef WSC_AP_SUPPORT
	#endif
			{"wlanRxPackets"},
			{"wlanTxPackets"},
			{"wlanRxBytes"},
			{"wlanTxBytes"},
			{""}
	};

	char value[16];
	char macvalue[20] = {0};
	char *buf = NULL;
	int i=0;
    unsigned long int wlanStatus[LastOne];

	char cmd[64]={0};
	#ifdef WSC_AP_SUPPORT//add by xyyou
	int wpsStatus=0;
	#endif
	mxml_node_t* wlanNode;
	const char* rt_device = NULL;
	if(node){
#ifdef TCSUPPORT_WLAN_WDS/* later to modify //gcyin*/
		mxmlElementSetAttr(node, "isWDSSupported", "Yes"); 
#endif
		buf = (char *)malloc(MAX_BUF_SIZE);
		if(buf==NULL){
			perror("malloc fail");
			return FAIL;
		}
		memset(buf,0, MAX_BUF_SIZE);
		/*krammer change : check ifconfig rai0 for isExist attr*/
		check_wireless_ac_card(value);
		mxmlElementSetAttr(node, ISEXIST, value);
		/**/

		/*
			Create a tmp file to get current wlan traffic statistics information.
			ext: /userfs/bin/iwpriv rai0 stat  > /tmp/wlan11ac_stats
		*/
		sprintf(cmd,"%s > %s", GET_WLAN_AC_INFO_CMD, WLAN11ACINFO_PATH);
		system(cmd);

		fileRead(WLAN11ACINFO_PATH, buf, MAX_BUF_SIZE);
		if(strlen(buf)!=0){
			for(i=0; strlen(wlan11acInfo_keywd[i])!=0; i++){
				wlanStatus[i]=get_key_vlaue(buf, wlan11acInfo_keywd[i], 10);
			}
			for(i=0; strlen(wlanInfo_attr_ac[i])!=0; i++){
				switch(i){
					case TxSuccess:/*wlanTxFrames*/
						sprintf(value,"%lu", wlanStatus[TxSuccess]);
						break;
					case TxFail:/*wlanTxErrFrames*/
					case (TxFail+1):/*wlanTxDropFrames*/
						sprintf(value,"%lu", wlanStatus[TxFail]);
						break;
					case (RxSuccess+1):/*wlanRxFrames*/
						sprintf(value,"%lu", wlanStatus[RxSuccess]);
						break;
					case (RxCRC+1):/*wlanRxErrFrames*/
						sprintf(value,"%lu", wlanStatus[RxCRC]);
						break;
					case (RxDrop+1):/*wlanRxDropFrames*/
						sprintf(value,"%lu", wlanStatus[RxCRC]+wlanStatus[RxDrop]);
						break;
					case (Channel+1):/*CurrentChannel */
					#ifndef BBUTOWBU
						sprintf(value,"%lu", wlanStatus[Channel]);
					#else
						UpdateChannelAC(value);
					#endif
						break;
					case (RetryCount+1):
						sprintf(value,"%lu", wlanStatus[RetryCount]);
						break;
					case (RTSSuccess+1):
						sprintf(value,"%lu", wlanStatus[RTSSuccess]);
						break;
					case (RTSFail+1):
						sprintf(value,"%lu", wlanStatus[RTSFail]);
						break;
					case (SNR0+1):
						sprintf(value,"%lu", wlanStatus[SNR0]);
						break;
					case (SNR1+1):
						sprintf(value,"%lu", wlanStatus[SNR1]);
						break;
					case (SNR2+1):
						sprintf(value,"%lu", wlanStatus[SNR2]);
						break;
					
				#ifdef WSC_AP_SUPPORT/*add by xyyou*/
					case (PinCode+1):
						#ifndef BBUTOWBU
						sprintf(value,"%08lu", wlanStatus[PinCode]);
						#else
						UpdatePinCodeAC(value);
						#endif
						break;
				#ifdef TCSUPPORT_WLAN_MULTI_WPS
					case (WPSStatus_0+1):/*wpsstatus,ssid0*/
					case (WPSStatus_1+1):/*wpsstatus,ssid1*/
					case (WPSStatus_2+1):/*wpsstatus,ssid2*/
					case (WPSStatus_3+1):/*wpsstatus,ssid3*/
						UpdateWpsStatusAC(&wpsStatus, macvalue, i-WPSStatus_0-1);
						if(wpsStatus==0||wpsStatus==1){
						       sprintf(value, "%s", "Idle");					
						}else if(wpsStatus==2){
							sprintf(value, "%s", "WPS Process Fail");
						}else if((wpsStatus>2) && (wpsStatus<34)){
							sprintf(value, "%s", "In progress");
						}else{
							sprintf(value, "%s", "Configured");
						}

						if( macvalue[0]){
							mxmlElementSetAttr(node, "wpsmac", macvalue);
						}
						break;
					case (WPSTimerRunning_0+1):/*wps timer running-rai0*/
						sprintf(value, "%d", wlanStatus[WPSTimerRunning_0]);
						break;
					case (WPSTimerRunning_1+1):/*wps timer running-rai1*/
						sprintf(value, "%d", wlanStatus[WPSTimerRunning_1]);
						break;
					case (WPSTimerRunning_2+1):/*wps timer running-rai2*/
						sprintf(value, "%d", wlanStatus[WPSTimerRunning_2]);
						break;
					case (WPSTimerRunning_3+1):/*wps timer running-rai3*/
						sprintf(value, "%d", wlanStatus[WPSTimerRunning_3]);
						break;
				#else	
					case (WPSStatus+1):/*wpsstatus*/
						#ifndef BBUTOWBU
						wpsStatus = wlanStatus[WPSStatus];
						#else
						UpdateWpsStatusAC(&wpsStatus);
						#endif
						if(wpsStatus==0||wpsStatus==1){
							//sprintf(value, "%s", "Not Used");
						//}else if(wpsStatus==1){
						       sprintf(value, "%s", "Idle");					
						}else if(wpsStatus==2){
							sprintf(value, "%s", "WPS Process Fail");
						}else if((wpsStatus>2) && (wpsStatus<34)){
							sprintf(value, "%s", "In progress");
						}else{
							sprintf(value, "%s", "Configured");
						}
						break;
					case (WPSTimerRunning+1):/*wps timer running*/
						sprintf(value, "%lu", wlanStatus[WPSTimerRunning]);
						break;

				#endif						
				#endif

#if 0
					case 6:/*isExist*/
						check_wireless_card(value);
						break;
#endif
                    case (RxPackets+1):/*Packets Received*/
                        sprintf(value, "%lu", wlanStatus[RxPackets]);
                        break;
                    case (TxPackets+1):/*Packets Sent*/
                        sprintf(value, "%lu", wlanStatus[TxPackets]);
                        break;
                    case (RxBytes+1):/*Bytes Received*/
                        sprintf(value, "%lu", wlanStatus[RxBytes]);
                        break;
                    case (TxBytes+1):/*Bytes Sent*/
                        sprintf(value, "%lu", wlanStatus[TxBytes]);
                        break;
					default:
						memset(value, 0, sizeof(value));
						break;
				}
				mxmlElementSetAttr(node, wlanInfo_attr_ac[i], value);
			}/*end for(i=0; strlen(wlanInfo_attr_ac...*/
#ifdef WSC_AP_SUPPORT
			mxmlElementSetAttr(node, "isWPSSupported", "YES");
#endif
#if defined(TCSUPPORT_WLAN_8021X)
			mxmlElementSetAttr(node, "isDot1XSupported", "No"); //  later to support.
#if defined(TCSUPPORT_WLAN_8021X_EXT)
			mxmlElementSetAttr(node, "isDot1XEnhanceSupported", "No"); //  later to support.
#endif
#endif
		}/*end if(strlen(buf)!=0)*/
		unlink(WLAN11ACINFO_PATH);
		free(buf);
		#ifdef QOS_DLS_SUPPORT
		mxmlElementSetAttr(node, "DLS", "1");
		#else
		mxmlElementSetAttr(node, "DLS", "0");
		#endif
		#ifdef IGMP_SNOOP_SUPPORT
		mxmlElementSetAttr(node, "M2U", "1");
		#else
		mxmlElementSetAttr(node, "M2U", "0");
		#endif
		#ifdef DFS_SUPPORT
		mxmlElementSetAttr(node, "DFS", "1");
		#else
		mxmlElementSetAttr(node, "DFS", "0");
		#endif
		#ifdef CARRIER_DETECTION_SUPPORT
		mxmlElementSetAttr(node, "Carrier", "1");
		#else
		mxmlElementSetAttr(node, "Carrier", "0");
		#endif
#if defined(TCSUPPORT_WLAN_MAXSTANUM_GUI)
			mxmlElementSetAttr(node, "maxStaNumSupported", MAX_STA_NUM);
#endif
		wlanNode = mxmlFindElement(node->parent->parent, node->parent->parent, "WLan11ac",NULL, NULL,MXML_DESCEND);
		wlanNode = mxmlFindElement(wlanNode, wlanNode,COMMON,NULL,NULL,MXML_DESCEND);
		if(wlanNode != NULL){
			rt_device = mxmlElementGetAttr(wlanNode,"rt_device");
			if((rt_device!=NULL)&&(strlen(rt_device)>0)){
				if(rt_device[strlen(rt_device)-1]=='2'){
					mxmlElementSetAttr(node, "TXPath", "2");
					mxmlElementSetAttr(node, "RXPath", "2");
				}
				else{
					mxmlElementSetAttr(node, "TXPath", "1");
					mxmlElementSetAttr(node, "RXPath", "1");
				}
			}
		}
	}/*end if(node)*/
	else{
		return FAIL;
	}
	return SUCCESS;
}
#endif /* TCSUPPORT_WLAN_AC */

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
/*_____________________________________________________________________________
**      function name: sysDeviceInfo_read
**      descriptions:
**            To get the system device information about cpu usage, memory usage,
**            uptime, auto pvc state.
**      parameters:
**            node: 
**            attr:
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**              
**
**      revision:
**      1. mhgao 2010/08/24
**____________________________________________________________________________
*/
extern unsigned int CpuUsage;
int
sysDeviceInfo_read(mxml_node_t *node, char *attr){
        char value[80] = {0};

        memset(value,0, sizeof(value));
        if(node)
        {
            sprintf(value, "%d", CpuUsage);
            mxmlElementSetAttr(node, "CpuUsage", value);
			   
            if(memory_read(value) != 0){
	             return FAIL;
            }
            mxmlElementSetAttr(node, "MemoryUsage", value);
        }
        else
        {
            return FAIL;
        }
        return SUCCESS;
}
#endif
#if defined(TCSUPPORT_CT_FJ)
/*
  * descriptions: searches the string str1 for n occurrence of the string str2.
  * 
  *           This function is used to get tx and rx error packets in ifconfig. 
  *           It can also be used to get dropped packets and overruns.
  * parameters:
  *       str1: 
  *       str2: string to find
  *        dst: output
  *          n: lenth of output
  *       base: 10 base or 16 base
  * return:
  *             number of str2 in str1.
  */
int get_errors(char *str1, char *str2, void *dst, int n, int base)
{
        int i=0;
        int len = strlen(str2);
        char tmp[11] = {0};
        unsigned long int *out = (unsigned long int*)dst;
                
        while(i<n)
        {
                str1 = strstr(str1, str2);
                str1 +=len+1;
                if(str1 != NULL)
                {
                        strncpy(tmp,str1, 10);
                        out[i] = strtoul(tmp,NULL, base);
                        i++;
                }
                else
                        return i;                       
        }
        return i;
}
#endif

/*_____________________________________________________________________________
**      function name: ifInfo_read
**      descriptions:
**            To fill the wlan status information into Info cfg node.
**
**      parameters:
**            node:   Put the child cfg node of Info.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**      	fileRead
**      	get_key_vlaue
**      	check_wireless_card
**
**      revision:
**      1. xyzhu 2009/11/16
**____________________________________________________________________________
*/
int
ifInfo_read(mxml_node_t *node, char *type)
{
	char ifInfo_keywd[][16]=
	{
		{"RX packets"},{"TX packets"},
		{"RX bytes"},{"TX bytes"},
		{"HWaddr"},{""}
	};
	/*set attr name same as keywd name*/
	char ifInfo_attr[][16]=
	{
		{"rxpackets"},{"txpackets"},
		{"rxbytes"},{"txbytes"},
		{"hwaddr"},
#if defined(TCSUPPORT_CT_FJ)
		{"rxerrors"},{"txerrors"},
		{"rxdropped"},{"txdropped"},
#endif
		{""}
	};
	
	enum ifInfo_en{
		RXpackets=0,
		TXpackets,
		RXbytes,
		TXbytes,
		HWAddr,
#if defined(TCSUPPORT_CT_FJ)
		RXerrors,
		TXerrors,
		RXdropped,
		TXdropped,
#endif
	};

	char value[20];
	char *buf = NULL;
	int i=0;
	unsigned long int ifStatus[5];
	char hwAddr[20];
	char *hwAddrPtr = NULL;
	char cmd[64]={0};
#if defined(TCSUPPORT_CT_LOOPDETECT)
	char flag = 0;
#endif

#if 1   //LAN_ETHERNET_INTERFACE
	char *info_path = "/tmp/ether_status";
	char tempvalue[2];
	char *tmp = NULL;
	int index,iStatus;	
	FILE *fp = NULL;
#endif
	char *pt = NULL;
	char nasnode[12] = {0};
	memset(value, 0, sizeof(value));
	memset(hwAddr, 0, sizeof(hwAddr));
	if(node){
		buf = (char *)malloc(MAX_BUF_SIZE);
		if(buf==NULL){
			perror("malloc fail");
			return FAIL;
		}
		memset(buf,0, MAX_BUF_SIZE);

#if 0
		if(type >=0 && type <=3){
			check_wlan_status(value, type);
			mxmlElementSetAttr(node, ISEXIST, value);
		}
#endif
		if ( NULL != strstr(type, "nas"))
		{
			strcpy(nasnode, type);
			if ( NULL != (pt = strstr(nasnode, ".")) )
				pt[0] = '_';
			sprintf(cmd,"/sbin/ifconfig %s > %s", nasnode, IFINFO_PATH);
		}	
		else
		sprintf(cmd,"/sbin/ifconfig %s > %s", type, IFINFO_PATH);
#if 0
		switch(type){
			case ifra0:
				sprintf(cmd,"/sbin/ifconfig ra0 > %s", IFINFO_PATH);
				break;
			case ifra1:
				sprintf(cmd,"/sbin/ifconfig ra1 > %s", IFINFO_PATH);
				break;
			case ifra2:
				sprintf(cmd,"/sbin/ifconfig ra2 > %s", IFINFO_PATH);
				break;
			case ifra3:
				sprintf(cmd,"/sbin/ifconfig ra3 > %s", IFINFO_PATH);
				break;
			case ifnas0:
				sprintf(cmd,"/sbin/ifconfig nas0 > %s", IFINFO_PATH);
				break;
			case ifnas1:
				sprintf(cmd,"/sbin/ifconfig nas1 > %s", IFINFO_PATH);
				break;
			case ifnas2:
				sprintf(cmd,"/sbin/ifconfig nas2 > %s", IFINFO_PATH);
				break;
			case ifnas3:
				sprintf(cmd,"/sbin/ifconfig nas3 > %s", IFINFO_PATH);
				break;
			case ifnas4:
				sprintf(cmd,"/sbin/ifconfigg nas4 > %s", IFINFO_PATH);
				break;
			case ifnas5:
				sprintf(cmd,"/sbin/ifconfig nas5 > %s", IFINFO_PATH);
				break;
			case ifnas6:
				sprintf(cmd,"/sbin/ifconfig nas6 > %s", IFINFO_PATH);
				break;
			case ifnas7:
				sprintf(cmd,"/sbin/ifconfig nas7 > %s", IFINFO_PATH);
				break;
			case ifeth0:
				sprintf(cmd,"/sbin/ifconfig eth0 > %s", IFINFO_PATH);
				break;
			
			default:
				free(buf);
				return FAIL;
		}
#endif
		system(cmd);

		fileRead(IFINFO_PATH, buf, MAX_BUF_SIZE);
		if(strstr(buf, "Device not found") != NULL)
		{
			free(buf);
			return FAIL;
		}
		
		if(strlen(buf)!=0){
			for(i=0; strlen(ifInfo_keywd[i])!=0; i++){
				if (!strcmp(ifInfo_keywd[i], "HWaddr")) {
					hwAddrPtr = strstr(buf, "HWaddr");
					if(hwAddrPtr){
						strncpy(hwAddr, hwAddrPtr + strlen("HWaddr") + 1, 17); //the length of mac addr is 17.
					}
				}
				else{
					ifStatus[i] = get_key_ifvlaue(buf, ifInfo_keywd[i], 10);
				}
			}
#if defined(TCSUPPORT_CT_FJ)
			get_errors(buf, "errors", ifStatus+RXerrors, 2, 10);
			get_errors(buf, "errors", ifStatus+TXerrors, 2, 10);
			get_errors(buf, "dropped", ifStatus+RXdropped, 2, 10);
			get_errors(buf, "dropped", ifStatus+TXdropped, 2, 10);
#endif
			for(i=0; strlen(ifInfo_attr[i])!=0; i++){
				switch(i){
					case 0:/*RXpackets*/
						sprintf(value,"%lu", ifStatus[RXpackets]);
						break;
					case 1:/*TXpackets*/
						sprintf(value,"%lu", ifStatus[TXpackets]);
						break;
					case 2:/*RXbytes*/
						sprintf(value,"%lu", ifStatus[RXbytes]);
						break;
					case 3:/*TXbytes*/
						sprintf(value,"%lu", ifStatus[TXbytes]);
						break;
					case 4:/*HWaddr*/
						strcpy(value, hwAddr);
						break;
#if defined(TCSUPPORT_CT_FJ)
					case 5:/*RXerrors*/
						sprintf(value,"%lu", ifStatus[RXerrors]);
						break;
					case 6:/*TXerrors*/
						sprintf(value,"%lu", ifStatus[TXerrors]);
						break;
					case 7:/*RXdropped*/
						sprintf(value,"%lu", ifStatus[RXdropped]);
						break;
					case 8:/*TXdropped*/
						sprintf(value,"%lu", ifStatus[TXdropped]);
						break;
#endif
					default:
						memset(value, 0, sizeof(value));
						break;
				}
				mxmlElementSetAttr(node, ifInfo_attr[i], value);
			}/*end for(i=0; strlen(ifInfo_attr...*/
		}/*end if(strlen(buf)!=0)*/
		unlink(IFINFO_PATH);
#if 1  //CT_COM_LAN_ETHERNET_INTERFACE
		if((tmp=strstr(type,"eth0.")) != NULL){
			tempvalue[0] = *(tmp + strlen("eth0.") );
			tempvalue[1] = '\0';
			index = atoi(tempvalue) - 1;
			sprintf(cmd, "/userfs/bin/ethphxcmd eth0 status port %d > %s", index, info_path);
			system(cmd);

			fp = fopen(info_path,"r");
			if(fp != NULL)
			{
				fscanf(fp,"%d",&iStatus);
				sprintf(value,"%d", iStatus);
				fclose(fp);
			}	
			mxmlElementSetAttr(node, "Status", value);
			unlink(info_path);

#if defined(TCSUPPORT_CT_LOOPDETECT)
			fp = fopen("/proc/tc3162/loop_detect_status", "r");
			if(fp != NULL)
			{
				fscanf(fp, "%s", value);
				flag = value[index];
				sprintf(value, "%c", flag);
				mxmlElementSetAttr(node, "detectionStatus", value);
				fclose(fp);
			}
		}else if((tmp=strstr(type,"ra")) != NULL){
			fp = fopen("/proc/tc3162/loop_detect_status", "r");
			if(fp != NULL)
			{
				tempvalue[0] = *(tmp + strlen("ra") );
				tempvalue[1] = '\0';
				index = atoi(tempvalue) - 1;
				fscanf(fp, "%s", value);
				flag = value[index+4];
				sprintf(value, "%c", flag);
				mxmlElementSetAttr(node, "detectionStatus", value);
				fclose(fp);
			}
#endif
		}
#endif
		free(buf);
	}/*end if(node)*/
	else{
		return FAIL;
	}
	return SUCCESS;
}

#if defined(TCSUPPORT_CT_E8GUI)
/*_____________________________________________________________________________
**      function name: ifInfo_ethcmd_read
**      descriptions:
**            To fill the lan status information into Info ether node.
**
**      parameters:
**            node:   Put the child ether node of Info.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**      	fileRead
**
**      revision:
**      1. cwzhuang 2010/07/13
**____________________________________________________________________________
*/
int
ifInfo_ethcmd_read(mxml_node_t *node)
{
	char ifInfo_ethcmd_attr[][16]=
	{
		{"Port1Status"},{"Port2Status"},
		{"Port3Status"},{"Port4Status"},
		{""}
	};
	
	char ifInfo_PortInfo_attr[][16] = 
	{
		{"PortMac"},{"PortIP"},
		{"PortDHCP"},{"PortExpire"},
		{"PortMac2"},
		{""}
	};

	char buf[BUF_SIZE_512]={0};
	char ip[40]={0},dev[15]={0},br0[15]={0},lladdr[15]={0},mac[20]={0},stale[15]={0};
	char port1status[4]={0},port2status[4]={0},port3status[4]={0},port4status[4]={0};
	int currline=0;
	int i=0;
	char cmd[64]={0};
	
	char *pBuf[5];
	char buftemp[32] = {0};
	char expireTime[32] = {0};
	char temp[80] = {0};
	int	ret = 0;

	for(i=0; i<5; i++){
		pBuf[i] = NULL;
		pBuf[i] = (char *)malloc(BUF_SIZE_512*2);
		if(NULL == pBuf[i])
			goto FreeBUF;
		memset(pBuf[i], 0x00, BUF_SIZE_512*2);
	}

	if(node)
	{
#ifdef TCSUPPORT_CPU_MT7510
		get3262LanStatus(temp);
		sprintf(cmd,"echo \"%s\" > %s", temp, IFINFO_ETHCMD_PATH);
#else
#if defined(TCSUPPORT_CT_PON)
		getPonLanStatus(temp);
		sprintf(cmd,"echo \"%s\" > %s", temp, IFINFO_ETHCMD_PATH);
#else
		sprintf(cmd,"/userfs/bin/ethphxcmd eth0 status > %s", IFINFO_ETHCMD_PATH);
#endif
#endif
		system(cmd);

		sprintf(cmd,"/usr/bin/ip neigh show dev br0 >> %s", IFINFO_ETHCMD_PATH);
		system(cmd);
		
		FILE *fp = NULL;
		fp=fopen(IFINFO_ETHCMD_PATH, "r");
	
		if(fp == NULL)
		{
			goto FreeBUF;
		}
		while (fgets(buf, BUF_SIZE_512, fp))
		{
			// get ip and mac info from file
			if(currline)
			{
				ret = sscanf(buf, "%s %s %s %s",ip,lladdr,mac,stale);
				if((ret == 4) && (inet_addr(ip) != INADDR_NONE))
				{
					memset(buftemp, 0x00, sizeof(buftemp));
					sprintf(buftemp, "%s,", ip);
					if((strlen(pBuf[1]) + strlen(buftemp)) > BUF_SIZE_512*2)
					{
						tcdbg_printf("%s: the length excess malloc size!\n", __FUNCTION__);
						goto SetAttrVal;
					}
					strcat(pBuf[1], buftemp);
					
					memset(buftemp, 0x00, sizeof(buftemp));
					sprintf(buftemp, "%s,", mac);
					if((strlen(pBuf[0]) + strlen(buftemp)) > BUF_SIZE_512*2)
					{
						if((strlen(pBuf[4]) + strlen(buftemp)) > BUF_SIZE_512*2)
						{
							tcdbg_printf("%s: the length excess malloc size!\n", __FUNCTION__);
							goto SetAttrVal;
						}
						else
						{
							strcat(pBuf[4], buftemp);
						}
					}
					else
					{
						strcat(pBuf[0], buftemp);
					}
					
					memset(buftemp, 0x00, sizeof(buftemp));
					memset(expireTime, 0x00, sizeof(expireTime));
					if (!get_dhcpLease_status(ip, expireTime))
						strcpy(buftemp, "0,");
					else
						strcpy(buftemp, "1,");
					strcat(pBuf[2], buftemp);
					memset(buftemp, 0x00, sizeof(buftemp));
					if(strlen(expireTime) != 0)
						sprintf(buftemp, "%s,", expireTime);
					else
						strcpy(buftemp, "0,");
					strcat(pBuf[3], buftemp);	
				}
				
			}
			// get port status info from file
			else
			{
				sscanf(buf, "%s %s %s %s",port1status,port2status,port3status,port4status);
				mxmlElementSetAttr(node, ifInfo_ethcmd_attr[0], port1status);
				mxmlElementSetAttr(node, ifInfo_ethcmd_attr[1], port2status);
				mxmlElementSetAttr(node, ifInfo_ethcmd_attr[2], port3status);
				mxmlElementSetAttr(node, ifInfo_ethcmd_attr[3], port4status);
			}
			currline++;
			memset(buf, 0, sizeof(buf));
		}
SetAttrVal:
		for(i=0; strlen(ifInfo_PortInfo_attr[i])!=0; i++)
			mxmlElementSetAttr(node, ifInfo_PortInfo_attr[i], pBuf[i]);
		fclose(fp);
		unlink(IFINFO_ETHCMD_PATH);	
	}
	else{

	}
FreeBUF:
	for(i=0; i<5; i++){
		if(NULL != pBuf[i]){
			free(pBuf[i]);
			pBuf[i] = NULL;
		}
	}	
	return SUCCESS;
}

/*_____________________________________________________________________________
**      function name: get_dhcpLease_status
**      descriptions:
**            Get the status of dhcpLease
**
**      parameters:
**            strIP:   IP wish to Check.
**
**      global:
**             None
**
**      return:
**            Success:        1
**            Otherwise:      0
**
**      call:
**   	 None

**____________________________________________________________________________
*/
int
get_dhcpLease_status(const char *strIP, char *pTime){
	char buf[MAXSIZE]={0},mac[17], ip[16],expire[10], hostname[32];
	//time_t curTime = time(0);
	time_t curTime;
	struct timespec curtime;
	int timeLeft;
	int day,hour,min,sec;
	FILE *fp = NULL;
	fp=fopen(DHCPLEASE_PATH, "r");
	
	if(fp == NULL)
	{
		return 0;
	}

	clock_gettime(CLOCK_MONOTONIC,&curtime);
	curTime = curtime.tv_sec;

	while (fgets(buf, MAXSIZE, fp))
	{
		sscanf(buf, "%s %s %s %s",mac, ip, expire, hostname);
		timeLeft = atoi(expire) - curTime;
		if(timeLeft > 0)
		{
			trimIP(strIP);
			trimIP(ip);
			if(!strcmp(strIP,ip))
			{
				day=timeLeft/SEC_PER_DAY;
		                hour=(timeLeft-SEC_PER_DAY*day)/SEC_PER_HOUR;
		                min=(timeLeft-SEC_PER_DAY*day-SEC_PER_HOUR*hour)/SEC_PER_MIN;
		                sec=timeLeft-SEC_PER_DAY*day-SEC_PER_HOUR*hour-SEC_PER_MIN*min;
				memset(buf, 0, sizeof(buf));
				sprintf(buf,"%d:%d:%d:%d",day,hour,min,sec);
				strcpy(pTime, buf);
				fclose(fp);
				return 1;
			}
		}	
		memset(buf, 0, sizeof(buf));
	}
	fclose(fp);

	return 0;	
}/*end get_dhcpLease_status*/

/*_____________________________________________________________________________
**      function name: trimIP
**      descriptions:
**            Trim Space From Ip Address String
**
**      parameters:
**            str:   IP Address.
**
**      global:
**             None
**
**      return:
**		trimed Ip
**
**      call:
**   	 None

**____________________________________________________________________________
*/
void trimIP(char* str)
{
	char* c = str + strlen(str) - 1;
	while(isspace(*c) && c>str)
	{
		*c ='\0';
		c--;
	}
}/*end trimIP*/
#endif

#ifdef TCSUPPORT_PRODUCTIONLINE
int tcgetProLinePara(void*buf,int flag)
{
	char cmdbuf[200] = {0};
	FILE *fp = NULL;	
	switch(flag)
	{
		case 0:
#if defined(TCSUPPORT_RESERVEAREA_EXTEND) || defined(TCSUPPORT_C7) || (defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505))
			sprintf(cmdbuf,TC_FLASH_READ_CMD,PRODUCTLINECWMPFILEPATH,sizeof(proline_Para),PROLINE_CWMPPARA_RA_OFFSET,RESERVEAREA_NAME);
#else
			sprintf(cmdbuf,TC_FLASH_READ_CMD,PRODUCTLINECWMPFILEPATH,PROLINE_CWMPPARA_RA_SIZE,PROLINE_CWMPPARA_RA_OFFSET,RESERVEAREA_NAME);
#endif
			
			system(cmdbuf);
			
			//open file
			fp = fopen(PRODUCTLINECWMPFILEPATH, "r");
			if (fp == NULL) 
			{
				printf("\r\ngetProLineCwmpPara:open file(%s) fail!",PRODUCTLINECWMPFILEPATH);
				return FAIL;
			}
			//read cwmp para 
			fread((proline_Para*)buf, sizeof(proline_Para), 1, fp);
			break;
		
		default:
			printf("\r\n[getProLinePara]Not support this para!!");
			break;

		
	}

	//close file and unlink it
	fclose(fp);
	unlink(PRODUCTLINECWMPFILEPATH);
	return SUCCESS;	
}
#endif
void GetEtherAddr(unsigned char *mac_addr)
{
	char macaddr[32] = {0};
	int i;
	char *macptr = NULL;
	char *macmove = NULL;
	
	char cmd[32]={0};
	char tmpBuf[512]={0};
	char *pValue=NULL;
	FILE * fp=NULL;
	char macInfo[2][18];
	char temp[]="/tmp/test.conf";

	memset(macInfo,0, sizeof(macInfo));	
	sprintf(cmd,"ifconfig eth0 > %s", temp);
	system(cmd);	
	fp=fopen(temp, "r");
	if(fp==NULL){
		return;
	}
	else{
		while (fgets (tmpBuf, sizeof(tmpBuf), fp) != NULL){
			/*krammer add*/
			if((pValue=strstr(tmpBuf,"HWaddr"))){
			       /*tmpBuf format: HWaddr 00:AA:BB:01:23:45*/
				sscanf(pValue,"%s %s", macInfo[0],macInfo[1]);
                        }
		}
	}
	fclose(fp);
	if(strlen(macInfo[1])>0){
		strcpy(macaddr, macInfo[1]);
	}	
	unlink(temp);
	
	macptr = macmove = macaddr;
	
	for(i = 0; i < 6; i++)
	{
		if((macmove = strchr(macptr,':')) == NULL)
			break;
		*macmove = '\0';
		mac_addr[i] = (unsigned char)strtoul(macptr,NULL,16);
		macptr = macmove + 1;
	}
	mac_addr[i] = (unsigned char)strtoul(macptr,NULL,16);
	return;
}

#if 0
/*_____________________________________________________________________________
**      function name: proLineInfo_read
**      descriptions:
**            To get default config value of cpe. There information store
**			  in reservearea,if reservearea is not configured,use parameters in cwmp node,if cwmp node is
**			  not configured,use default paramters according mac info
**      parameters:
**            node:   Put the child cfg node of Info.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**      		
**
**      revision:
**      1. Brian 2010/10/27
**____________________________________________________________________________
*/
int
proLineInfo_read(mxml_node_t *top,mxml_node_t *node)
{
	int notUsed = 0;
	proline_Para para;
	unsigned char nm_MAC[7] ;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char buf[64] = {0};
	char proLineInfo_keywd[][32]=
	{
		{"ManufacturerOUI"},
		{"SerialNum"},
		{"ProductClass"},
		{""}
	};

	enum proLineInfo_eu{
		ManufacturerOUI = 0,
		SerialNum,	
		ProductClass,
		};
	
	tcgetProLinePara(&para,0);
	//tcdbg_printf("\r\npara.magic is %x",para.magic);
	//tcdbg_printf("\r\npara.flag is %x",para.flag);
	if(PRODUCTLINEPARAMAGIC == para.magic)
	{
		notUsed = 1;
	}
	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], "Cwmp");
	strcpy(nodeName[1], "Entry");
	
	if(notUsed && (para.flag & PL_CP_MANUFACUREROUI_FLAG))
	{
		//tcdbg_printf("\r\npara.manufacturerOUI1 is %s",para.manufacturerOUI);
		mxmlElementSetAttr(node, proLineInfo_keywd[ManufacturerOUI], para.manufacturerOUI);
	}
	else
	{
		getAttrValue(top, nodeName, "ManufacturerOUI", buf);
		if(buf[0] == '\0')
		{
			GetEtherAddr(nm_MAC);
			sprintf(buf,"%02X%02X%02X",nm_MAC[0],nm_MAC[1],nm_MAC[2]);
		}
		//tcdbg_printf("\r\npara.manufacturerOUI2 is %s",buf);
		mxmlElementSetAttr(node, proLineInfo_keywd[ManufacturerOUI], buf);
	}

	if(notUsed &&(para.flag & PL_CP_SERIALNUM_FLAG))
	{
		//tcdbg_printf("\r\npara.serialnum1 is %s",para.serialnum);
		mxmlElementSetAttr(node, proLineInfo_keywd[SerialNum], para.serialnum);
	}
	else
	{
		getAttrValue(top, nodeName, "SerialNum", buf);
		if(buf[0] == '\0')
		{
			GetEtherAddr(nm_MAC);
			sprintf(buf,"%02x%02x%02x%02x%02x%02x",nm_MAC[0],nm_MAC[1],nm_MAC[2],nm_MAC[3],nm_MAC[4],nm_MAC[5]);
		}
		//tcdbg_printf("\r\npara.serialnum2 is %s",buf);
		mxmlElementSetAttr(node, proLineInfo_keywd[SerialNum], buf);
	}

	if(notUsed &&(para.flag & PL_CP_PRODUCTCLASS_FLAG))
	{
		//tcdbg_printf("\r\npara.productclass1 is %s",para.productclass);
		mxmlElementSetAttr(node, proLineInfo_keywd[ProductClass], para.productclass);
	}
	else
	{
		getAttrValue(top, nodeName, "ProductClass", buf);
		if(buf[0] == '\0')
		{
			GetEtherAddr(nm_MAC);
			sprintf(buf,"%02x%02x%02x",nm_MAC[3],nm_MAC[4],nm_MAC[5]);
		}
		//tcdbg_printf("\r\npara.productclass2 is %s",buf);
		mxmlElementSetAttr(node, proLineInfo_keywd[ProductClass],buf);
	}
	
	return SUCCESS;
}
#endif

/*_____________________________________________________________________________
**      function name: devDefInf_read
**      descriptions:
**            To get default config value of cpe. There information may store
**			  in bootbase, but here we config there information in filesystem.
**			  Get there config information to config node Info_devDefInf.
**      parameters:
**            node:   Put the child cfg node of Info.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**      	fileRead
**      	get_key_vlaue
**
**      revision:
**      1. xyzhu 2010/01/14
**____________________________________________________________________________
*/
int
devDefInf_read(mxml_node_t *node)
{
	char attrName[32];
	char attrVal[128];
	int i, ret;
	
	char devDefInfo_keywd[][32]=
	{
		{"Manufacturer"},{"ModelName"},
		{"ProductClass"},{"CustomerSoftwareVersion"},
		{"CustomerSwVersionTime"},{""}
	};

	memset( attrName, 0, sizeof(attrName) );
	memset( attrVal, 0, sizeof(attrVal) );

	for ( i = 0; strlen(devDefInfo_keywd[i]) != 0; i++ ) {
		
		sprintf( attrName, "%s=", devDefInfo_keywd[i] );
		ret = get_profile_str( attrName, attrVal, sizeof(attrVal), NO_QMARKS, DEVDEF_PATH);
		if ( ret != -1 ) {
			mxmlElementSetAttr(node, devDefInfo_keywd[i], attrVal);
		}
		
	}
	
	return SUCCESS;	
}

#endif
/*_____________________________________________________________________________
**      function name: get_web_node
**      descriptions:
**            To get the cfg node, if there is no this cfg node then we will to create a cfg node.
**
**      parameters:
**            top:   Put the top of cfg node tree.
**            name: Put the cfg node name.
**
**      global:
**             None
**
**      return:
**            User specify the configure node address pointer.
**
**      call:
**      	mxmlFindElement
**      	mxmlNewElement
**
**      revision:
**      1. Here 2008/5/19
**____________________________________________________________________________
*/
mxml_node_t *
get_web_node(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	int i=0;
	mxml_node_t *parent = NULL;
	mxml_node_t *curNode = top;

	for(i=0; i < MAX_ARG_NUM; i++){
		parent=curNode;
		if(strlen(name[i])==0){
			break;
		}
		curNode=mxmlFindElement(parent, parent, name[i],
				NULL, NULL, MXML_DESCEND);
		if(curNode==NULL){
			curNode = mxmlNewElement(parent,  name[i] );;
		}
	}
	return  curNode;
}/*end get_web_node*/
/*_____________________________________________________________________________
**      function name: del_sub_node
**      descriptions:
**            Delete cfg node entry.
**
**      parameters:
**            top:   Put the top of cfg node tree.
**            name: Put the cfg node name.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**      	mxmlFindElement
**      	mxmlDelete
**
**      revision:
**      1. Here 2008/5/19
**____________________________________________________________________________
*/
void
del_sub_node(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	int i=0;
	mxml_node_t *curNode=top;

	for(i=0;i < MAX_ARG_NUM;i++){
		if(strlen(name[i])==0){
			break;
		}
		curNode=mxmlFindElement(curNode, curNode, name[i],
				NULL, NULL, MXML_DESCEND);
	}

	if(curNode){
		mxmlDelete(curNode);
	}

}/*end del_sub_node*/

/*_____________________________________________________________________________
**      function name: get_key_vlaue
**      descriptions:
**            Get the value of keyword
**
**      parameters:
**            str:   Put the top of cfg node tree that is used to find out value.
**            keyword:  Put the keyword
**            base: 10 base or 16 base
**
**      global:
**             None
**
**      return:
**             Get the value
**
**      call:
**   	 None
**
**      revision:
**      1. Here 2008/5/7
**____________________________________________________________________________
*/
unsigned long int
get_key_vlaue(char *str, char *keyword, int base){
	char *pch_itemname = NULL;
	char *pch_temp = NULL;
	char value[11]={0};

	pch_itemname = strstr(str, keyword);
	if (pch_itemname != NULL){
		pch_temp = strstr(pch_itemname,"= ");
		if (pch_temp != NULL){
			strncpy(value,pch_temp+2, 10);
			return strtoul(value,NULL, base);
		}
	}
	return SUCCESS;
}/*end get_key_value*/

/*_____________________________________________________________________________
**      function name: get_key_ifvlaue
**      descriptions:
**            Get the value of keyword
**
**      parameters:
**            str:   Put the top of cfg node tree that is used to find out value.
**            keyword:  Put the keyword
**            base: 10 base or 16 base
**
**      global:
**             None
**
**      return:
**             Get the value
**
**      call:
**   	 None
**
**      revision:
**      1. xyzhu 2009/11/16
**____________________________________________________________________________
*/
unsigned long int
get_key_ifvlaue(char *str, char *keyword, int base){
	char *pch_itemname = NULL;
	char *pch_temp = NULL;
	char value[11]={0};

	pch_itemname = strstr(str, keyword);
	if (pch_itemname != NULL){
		pch_temp = strstr(pch_itemname,":");
		if (pch_temp != NULL){
			strncpy(value,pch_temp+1, 10);
			return strtoul(value,NULL, base);
		}
	}
	return SUCCESS;
}/*end get_key_ifvlaue*/


int
get_key_macTabvlaue(char *str, char *keyword, int base){
	char *pch_itemname = NULL;
	char *pch_temp = NULL;
	char value[11]={0};

	pch_itemname = strstr(str, keyword);
	if (pch_itemname != NULL){
		pch_temp = strstr(pch_itemname,":");
		if (pch_temp != NULL){
			strncpy(value,pch_temp+1, 10);
			return strtoul(value,NULL, base);
		}
	}
	return -1;
}/*end get_key_ifvlaue*/

#ifndef PURE_BRIDGE
/*_____________________________________________________________________________
**      function name: checkPpp
**      descriptions:
**            Check the connection of ppp
**
**      parameters:
**            cur_pvc:  the ppp on which pvc we want to check
**
**      global:
**             None
**
**      return:
**             FAIL:-1
**		  0:interface is down
**		  1:interface is up
**
**      call:
**   		 None
**
**      revision:
**      1. krammer 2008/10/03
**____________________________________________________________________________
*/
int
checkPpp(int cur_pvc){
	char ifconfig_ppp_cmd[32]={0};
	sprintf(ifconfig_ppp_cmd, "ifconfig ppp%d > %s", cur_pvc, TMP_PPP_PATH);
	system(ifconfig_ppp_cmd);
	char* buf = (char *)malloc(MAX_BUF_SIZE);
	if(buf==NULL){
		perror("malloc fail");
		unlink(TMP_PPP_PATH);
		return FAIL;
	}
	memset(buf,0, MAX_BUF_SIZE);
	fileRead(TMP_PPP_PATH, buf, MAX_BUF_SIZE);
	unlink(TMP_PPP_PATH);
	if(strstr(buf,"inet addr:")==0){/*ppp is down*/
		free(buf);
		return 0;
	}
	else{/* ppp up*/
		free(buf);
		return 1;
	}

}/*end get_ifconfig_value*/

/*_____________________________________________________________________________
**      function name: check_wireless_card
**      descriptions:
**            This function is use for checking the CPE has wireless card or not.
**
**      parameters:
**            retval: Put buf string that will be got the wlan status information.
**
**      global:
**             None
**
**      return:
**            None
**
**      call:
**   	 None
**
**      revision:
**      1.shnwind 20080417
**      2.Here 20080611
**____________________________________________________________________________
*/
void
check_wireless_card(char *retval){
	int fd,len;
	char stream[64];

	system("/sbin/ifconfig ra0 > /tmp/wireless_stat.tmp");
	memset(stream,0,sizeof(stream));
	fd = open("/tmp/wireless_stat.tmp",O_RDONLY);
	if (fd == FAIL){
		strcpy(retval, "Off");
		return;
	}

	len=read(fd,stream,64);
	close(fd);
	remove("/tmp/wireless_stat.tmp");
	if((len == 0) || (len == -1)){
		strcpy(retval, "Off");
		return;
	}

	if((strstr(stream,"HWaddr FF:FF:FF:FF:FF:FF") != 0)
	||(strstr(stream,"HWaddr 00:00:00:00:00:00") != 0)){
		fprintf(stderr,"There are some error when CPE getting data from EEPROM");
		strcpy(retval, "Error");
		return;
	}else{
		strcpy(retval, "On");
	}
}/*end check_wireless_card*/
#ifdef TCSUPPORT_WLAN_AC
/*_____________________________________________________________________________
**      function name: check_wireless_ac_card
**      descriptions:
**            This function is use for checking the CPE has wireless card or not.
**
**      parameters:
**            retval: Put buf string that will be got the wlan status information.
**
**      global:
**             None
**
**      return:
**            None
**
**      call:
**   	 None
**
**      revision:
**      1.evan.jiang 20130801
**____________________________________________________________________________
*/
void
check_wireless_ac_card(char *retval){
	int fd,len;
	char stream[64];
#ifdef TCSUPPORT_WLAN_PERSSID_SWITCH
	int i;
	char isexist = 0;
	char BssidNum[4] = {0};
	char tmp[16]={0};
	char checkwifiisexist[128] = {0};
#endif
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	mxml_node_t *top = get_node_top();
	char apswitch[4] = {0};

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], "WLan11ac");
	strcpy(nodeName[1], "Common");
	/*if wifi function is off,return disable directly*/
	if(getAttrValue(top, nodeName,WLANSWITCHNAME, apswitch) == TCAPI_PROCESS_OK)
	{
		if(atoi(apswitch) == 0)
		{
			strcpy(retval, "Disable");
			return;
		}
	}
	else
	{
		strcpy(retval, "Off");
		return;
	}
	
	#ifdef TCSUPPORT_WLAN_PERSSID_SWITCH
	if(getAttrValue(top, nodeName,"BssidNum", BssidNum) == TCAPI_PROCESS_OK)
	{
		for(i = 0; i < atoi(BssidNum); i++)
		{
			sprintf(stream,"%s%d", SUB_NODE_NAME, i);
			strcpy(nodeName[1], stream);
			getAttrValue(top, nodeName,"EnableSSID", tmp);
			if(atoi(tmp) == 1)
			{
				sprintf(checkwifiisexist,WIFI_AC_IS_EXIST_CMD, i);
				isexist = 1;
				break;
			}
		}
	}
	else
	{
		strcpy(retval, "Off");
		return;
	}
	
	if(!isexist)	
	{
		//wifi load but all ssid is disable,so return on let webpage know it
		strcpy(retval, "On");
		return;
	}
#endif
#ifdef TCSUPPORT_WLAN_PERSSID_SWITCH
	system(checkwifiisexist);
#else
	system("/sbin/ifconfig rai0 > /tmp/wireless_ac_stat.tmp");
#endif
	memset(stream,0,sizeof(stream));
	fd = open("/tmp/wireless_ac_stat.tmp",O_RDONLY);
	if (fd == FAIL){
		strcpy(retval, "Off");
		return;
	}

	len=read(fd,stream,64);
	close(fd);
	remove("/tmp/wireless_ac_stat.tmp");
	if((len == 0) || (len == -1)){
		strcpy(retval, "Off");
		return;
	}

	if((strstr(stream,"HWaddr FF:FF:FF:FF:FF:FF") != 0)
	||(strstr(stream,"HWaddr 00:00:00:00:00:00") != 0)){
		fprintf(stderr,"There are some error when CPE getting data from EEPROM");
		strcpy(retval, "Error");
		return;
	}else{
		strcpy(retval, "On");
	}
}/*end check_wireless_ac_card*/
#endif /* check_wireless_ac_card */
#endif

/*______________________________________________________________________________
**function name:get_parameter_value
**
**description:
*   find the value to the specific parameter
**parameters:
*     buf:the string need search
*    	paremeter:specific parameter
*	ret_val:reture value
**global:
*    none
**return:
*    0:success
*	-1:fail
**call:
*    none
**revision:
*     1.shnwind 20080716
**____________________________________________________________________________*/

int
get_parameter_value(char *buf, char *paremeter, char *ret_val, int buf_size){
	char *ret, *ret2;
	int i;
   	ret = strstr(buf,paremeter);

	if(ret != NULL){
		ret = ret +strlen(paremeter);
		/*filename will be xxxxx&xxxxx when use ie to upgrade fw.
			It will cause CPE get wrong file name. 
			2009.9.15. shnwind modify*/
		ret2 = strchr(ret,'=');
		
		if(ret2 == NULL){
			ret2 = buf + buf_size;//last byte
		}
		for(i=0; i < (ret2 - ret) ; i++){
			if(*(ret2 - i) == '&'){
				ret2 -= i;
				break;
			}	
		}
		//ret2 = strchr(ret,'&');
		if((ret2 == NULL) || (ret2 == ret) || (ret2-ret > buf_size)){
			return FAIL;
		}
		memset(ret_val,0,buf_size);
		memcpy(ret_val,ret,ret2-ret);
		ret_val[ret2-ret] = 0;
	}else{
		return FAIL;
	}
	return SUCCESS;
}

/*______________________________________________________________________________
**function name:html_post_parser
**
**description:
*   parser the data which user transfer by web browser.
**parameters:
*    	file_path:file need to paser.
*	data_start : data start point
*	data_stop : data stop point
*	firmware_type : firmware type , link linux.7z .....
**global:
*    none
**return:
*    0:success
* 	-1:fail
**call:
*    none
**revision:
*     1.shnwind 20090410
**____________________________________________________________________________*/

int html_post_parser(char *file_path, unsigned long *data_start, unsigned long *data_stop, unsigned char *firmware_type)
{
	char value[256];
	char *tmp;
	int fd, content_len;
	struct stat buf_st;

	if(stat(MULTI_TO_NORMAL_TEMP, &buf_st) != 0){
		return FAIL;
	}

	content_len = buf_st.st_size;
	tmp = (char*)malloc(content_len+1);
	if(tmp == NULL){
		return FAIL;
	}
	fd = open(MULTI_TO_NORMAL_TEMP,O_RDONLY);
	if(fd < 0){
		free(tmp);
		return FAIL;
	}

	if(read(fd,tmp,content_len) < 0){
		free(tmp);
		close(fd);
		return FAIL;
	}
	close(fd);
	/*get filename*/
	if(get_parameter_value(tmp,"bfilename=", value, (content_len+1)) == FAIL){
		free(tmp);
		return FAIL;
	}

	/*choose upgrade type by name*/
	if(strstr(value,"romfile") != NULL){
		*firmware_type = UG_ROMFILE;
#if 0		
	}else if(strstr(value,"linux.7z") != NULL){
		*firmware_type = UG_KERNEL;
	}else if(strstr(value,"rootfs") != NULL){
		*firmware_type = UG_FS;
#endif		
	}
#if defined(TCSUPPORT_CT_UPLOAD_ALLINONE)
	else if( NULL != strstr(value,"tclinux")
		&& NULL == strstr(value,"tclinux_allinone") )
	{
		*firmware_type = UG_TCLINUX;
	}
	else if( NULL != strstr(value,"tclinux_allinone") )
	{
		*firmware_type = UG_TCLINUX_ALLINONE;
	}
#else
	else if(strstr(value,"tclinux") != NULL){
		*firmware_type = UG_TCLINUX;
	}
#endif
	else{
		tcdbg_printf("not support firmware\n");
		free(tmp);
		return FAIL;
	}
	/*get data start*/
	if(get_parameter_value(tmp,"bdata_start=", value, (content_len+1)) == FAIL){
		free(tmp);
		return FAIL;
	}

	*data_start = strtol(value, NULL, 10);
	/*get data end*/
	if(get_parameter_value(tmp,"bdata_end=", value, (content_len+1)) == FAIL){
		free(tmp);
		return FAIL;
	}
	*data_stop = strtol(value, NULL, 10);

	free(tmp);

	return SUCCESS;
}
//"wifiMacTab",		UPDATE_TYPE|WEB_TYPE,	NULL, NULL, wifiMacTab_read, NULL, NULL, NULL},
/*______________________________________________________________________________
**	info_init
**
**	descriptions:
**		the init function of info.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7601E)|| defined(MT7592)
char *macTab_adv_table[MAX_NODE_NAME]=
{
	"Common","",
};
char *macTab_merge_table[MAX_NODE_NAME]=
{
	"macTab_id","",
};
#endif

int
wifiMacTab_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="wifiMacTab",
		#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7601E)|| defined(MT7592)
			.type=WEB_TYPE|UPDATE_TYPE|ADV_FLAG|MAX_LEN_OF_MAC_TABLE,
			.adv_table=macTab_adv_table,
			.merge_table=macTab_merge_table,
		#else
			.type=WEB_TYPE|UPDATE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
		#endif
			.cfg_read=wifiMacTab_read,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=NULL,
			.cfg_boot=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}

#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7601E)|| defined(MT7592)
mac_table_t gMacTable[4];
#endif

int
wifiMacTab_read(mxml_node_t *node, char node_Name[][MAX_NODE_NAME], char *attr)
{
#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7601E)|| defined(MT7592)
	tcapi_msg_t msg;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char NUM[4];
	int i,maxSSIDNum = 4;

	memset( nodeName,0,sizeof(nodeName) );
	strcpy( nodeName[0],WIFIMACTABLE );
	strcpy( nodeName[1],COMMON );
	memset( NUM, 0, sizeof(NUM) );

	/*init mac entry num*/
		if(strcmp(attr,"valueChanged")!=0){
				int wlanIndex, macNum;
				char *p;
				char NUMAttr[8]={0},IPAttr[8]={0},MACAttr[8]={0};
#if defined(TCSUPPORT_CT_PHONEAPP)
				char RSSIAAttr[14]={0},RSSIBAttr[14]={0},RSSICAttr[14]={0};
#endif
				if((p=strstr(attr,WIFIMACTABLE_NUM))!=NULL)
					p=p+strlen(WIFIMACTABLE_NUM);
				else if((p=strstr(attr,WIFIMACTABLE_IP))!=NULL)
					p=p+strlen(WIFIMACTABLE_IP);
				else if((p=strstr(attr,WIFIMACTABLE_MAC))!=NULL)
					p=p+strlen(WIFIMACTABLE_MAC);
#if defined(TCSUPPORT_CT_PHONEAPP)
				else if((p=strstr(attr,WIFIMACTABLE_RSSIA))!=NULL)
					p=p+strlen(WIFIMACTABLE_RSSIA);
				else if((p=strstr(attr,WIFIMACTABLE_RSSIB))!=NULL)
					p=p+strlen(WIFIMACTABLE_RSSIB);
				else if((p=strstr(attr,WIFIMACTABLE_RSSIC))!=NULL)
					p=p+strlen(WIFIMACTABLE_RSSIC);
#endif
				else{
					tcdbg_printf("The attribute is not in wifiMACTab\n");
					return FAIL;
				}

				wlanIndex=atoi(p);
				if((wlanIndex < 0) || (wlanIndex > maxSSIDNum -1)){
					tcdbg_printf("now we only support 4 ssid!\n");
					return FAIL;

				}
							
				//get the old wlanindex.
				getAttrValue(node, nodeName, "oldIndex", NUM);
				sprintf(NUMAttr,"NUM%d",atoi(NUM));
				memset( NUM, 0, sizeof(NUM) );
				getAttrValue(node, nodeName, NUMAttr, NUM);
				macNum = atoi(NUM);

				/*dele the old entries*/
				for ( i = 0; i < macNum; i++ ) {
					memset( &msg, 0, sizeof(msg) );
					sprintf( msg.node, "wifiMacTab_Entry%d", i );	
					tcapi_unset_req( node, &msg );
				}
				//clear the old NUM value	
				memset( &msg, 0, sizeof(msg) );
				strcpy( msg.node, "wifiMacTab_Common" );
				strcpy( msg.attr, NUMAttr);
				sprintf( msg.value, "0");
			 	tcapi_set_req( node, &msg );


				memset( &NUMAttr, 0, sizeof(NUMAttr) );
				sprintf(NUMAttr,"NUM%d",wlanIndex);
				sprintf(IPAttr,"IP%d",wlanIndex);
				sprintf(MACAttr,"MAC%d",wlanIndex);
#if defined(TCSUPPORT_CT_PHONEAPP)
				sprintf(RSSIAAttr,"RSSIA%d",wlanIndex);
				sprintf(RSSIBAttr,"RSSIB%d",wlanIndex);
				sprintf(RSSICAttr,"RSSIC%d",wlanIndex);
#endif

				/*get new entries*/
				mac_table_t macTable;
				memset( &macTable, 0, sizeof(macTable) );
				getMacEntryByIndex( &macTable, wlanIndex );

				/*get entry num and set it to node*/
				memset( &msg, 0, sizeof(msg) );
				strcpy( msg.node, "wifiMacTab_Common" );
				strcpy( msg.attr, NUMAttr);
				sprintf( msg.value, "%lu", macTable.Num );
			 	tcapi_set_req( node, &msg );

				for ( i = 0; i < macTable.Num; i++ ) {
					/*get mac addr and set it to node*/
					memset( &msg, 0, sizeof(msg) );
					sprintf( msg.node, "wifiMacTab_Entry%d", i );
					strcpy( msg.attr, MACAttr);
					sprintf( msg.value, "%02X:%02X:%02X:%02X:%02X:%02X", macTable.macEntry[i].Addr[0],\
																		 macTable.macEntry[i].Addr[1],\
																		 macTable.macEntry[i].Addr[2],\
																		 macTable.macEntry[i].Addr[3],\
																		 macTable.macEntry[i].Addr[4],\
																		 macTable.macEntry[i].Addr[5] );
					tcapi_set_req( node, &msg );

					/*get ip addr and set it to node*/
					char ipAddr[16];
					memset( ipAddr, 0, sizeof(ipAddr) );
					getIpFromArp( msg.value, ipAddr );
					memset( &msg, 0, sizeof(msg) );
					sprintf( msg.node, "wifiMacTab_Entry%d", i );
					strcpy( msg.attr, IPAttr );
					strcpy( msg.value, ipAddr );
					tcapi_set_req( node, &msg );
					
#if defined(TCSUPPORT_CT_PHONEAPP)
					memset( &msg, 0, sizeof(msg) );
					sprintf( msg.node, "wifiMacTab_Entry%d", i );
					strcpy( msg.attr, RSSIAAttr);
					sprintf( msg.value, "%d",  macTable.macEntry[i].AvgRssi0);
					tcapi_set_req( node, &msg );	
					
					memset( &msg, 0, sizeof(msg) );
					sprintf( msg.node, "wifiMacTab_Entry%d", i );
					strcpy( msg.attr, RSSIBAttr);
					sprintf( msg.value, "%d",  macTable.macEntry[i].AvgRssi1);
					tcapi_set_req( node, &msg );
					
					memset( &msg, 0, sizeof(msg) );
					sprintf( msg.node, "wifiMacTab_Entry%d", i );
					strcpy( msg.attr, RSSICAttr);
					sprintf( msg.value, "%d",  macTable.macEntry[i].AvgRssi2);
					tcapi_set_req( node, &msg );
#endif
					
				}
				//set the current wlan index.
				memset( &msg, 0, sizeof(msg) );
				strcpy( msg.node, "wifiMacTab_Common" );
				strcpy( msg.attr, "oldIndex");
				sprintf( msg.value, "%lu", wlanIndex );
			 	tcapi_set_req( node, &msg );


				
			//}
		}
		else {
			/*it means we want to get whether wifi mac table change*/
			int macTableChg = 0;
#if defined(TCSUPPORT_CT_PHONEAPP)
			int j = 0;
#endif
			for ( i = 0; i < 4; i++ ) {
				/*now we only support 4 ssid*/
				mac_table_t macTab;
				memset( &macTab, 0, sizeof(macTab) );

				getMacEntryByIndex( &macTab, i );
#if defined(TCSUPPORT_CT_PHONEAPP)
				if(macTab.Num != gMacTable[i].Num){
					memcpy( (void*)&gMacTable[i], (void*)&macTab, sizeof(mac_table_t) );
					macTableChg = 1;
					continue;
				}
				for(j=0; j<macTab.Num; j++){
					if ( memcmp(macTab.macEntry[j].Addr, gMacTable[i].macEntry[j].Addr, MAC_ADDR_LENGTH) ) {
						memcpy( (void*)&gMacTable[i], (void*)&macTab, sizeof(mac_table_t) );
						macTableChg = 1;
						break;
					}					
				}
#else
				if ( memcmp((void*)&macTab, (void*)&gMacTable[i], sizeof(mac_table_t)) ) {
					/*mac table changed, so update it to "gMacTable"*/
					memcpy( (void*)&gMacTable[i], (void*)&macTab, sizeof(mac_table_t) );
					macTableChg = 1;
				}
#endif
			}
			if ( macTableChg == 1 ) {
				/*it means wifi mac table change*/
				memset( &msg, 0, sizeof(msg) );
				strcpy( msg.node, "wifiMacTab_Common" );
				strcpy( msg.attr, "valueChanged" );
				strcpy( msg.value, "1" );
			    tcapi_set_req( node, &msg );
			}
			else {
				/*it means wifi mac table not change*/
				memset( &msg, 0, sizeof(msg) );
				strcpy( msg.node, "wifiMacTab_Common" );
				strcpy( msg.attr, "valueChanged" );
				strcpy( msg.value, "0" );
				tcapi_set_req( node, &msg );
			}
		}
//	}
	return SUCCESS;
#else
	mxml_node_t *parent;
	mxml_node_t *curNode; //current node like wifiMacTab_Entry{i}/wifiMacTab_Common
	char cmd[64]={0};
	char flag[4] = {'\0'};
	char index[4] = {'\0'};
	int wlanIndex = -1;
	int isMacTabChanged = 0;	//this flag show if the wifi mac table is changed.
	char macTabKeyWord[20] = {"MacTabChanged"};
	char attrName[20] = {'\0'};
	char attrValue[10] = {'\0'};
	//char entry[10] = {'\0'};
	char temp[100] = {'\0'};
	char macAddr[18] = {'\0'};
	int i,ii;
	char buf[200] = {0};
	FILE *fp = NULL;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];

	for(i = 0; i < 4; i++){//now we support four ssid.
		sprintf(cmd,GET_WLAN_MAC_TABLE, i, WLAN_MAC_TABLE_PATH);
		system(cmd);
				
		fp = fopen(WLAN_MAC_TABLE_PATH, "r");
		if(fp == NULL){
			unlink(WLAN_MAC_TABLE_PATH);
			continue;
		}
		if(fgets(buf, 200, fp) == NULL){	//fp pointer to "ra{i} get mac tab"
			fclose(fp);						
			unlink(WLAN_MAC_TABLE_PATH);
			continue;
		}
		fgets(buf, 200, fp);				//fp pointer to "MacTabChanged:{flag}"
				
		fclose(fp);
		unlink(WLAN_MAC_TABLE_PATH);
		
		isMacTabChanged = get_key_ifvlaue(buf, macTabKeyWord, 10);
		if(isMacTabChanged == 1)  //mac table have been changed.
			break;

	}
		
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],WIFIMACTABLE);
	strcpy(nodeName[1],COMMON);
	
	curNode=get_web_node(node, nodeName);
	if(curNode != NULL){

		
		strcpy(attrName, "valueChanged");
		sprintf(attrValue, "%d", isMacTabChanged);
		mxmlElementSetAttr(curNode, attrName, attrValue);
		//set entry num to 0 here.
		strcpy(attrName, "NUM");
		strcpy(attrValue, "0");
		mxmlElementSetAttr(curNode, attrName, attrValue);
	}
/*	
	if(isMacTabChanged == 0){//if mac table is not changed, just return.
		return SUCCESS;

	}

*/
	if( getAttrValue(node, nodeName, "flag", flag) == TCAPI_PROCESS_OK
	 && getAttrValue(node, nodeName, "index", index) == TCAPI_PROCESS_OK ){
		if(!strcmp(flag, MACTABLEDATA)){
//get mac table data	
			//delete all Entry nodes
			/*find parent node*/
			parent = mxmlFindElement(node, node, nodeName[0],
				NULL, NULL, MXML_DESCEND);
				
			i = 0;
			/*delete all child node*/
			while(1){
				sprintf(nodeName[1], "Entry%d", i);
				curNode = mxmlFindElement(parent, parent, nodeName[1],
				NULL, NULL, MXML_DESCEND);
				if(curNode != NULL){				
					mxmlDelete(curNode);
				}
				else{
					break;
				}
				i++;
			}
			//delete end.

			//wifi ssid index.
			wlanIndex = atoi(index);
			sprintf(cmd,GET_WLAN_MAC_TABLE, wlanIndex, WLAN_MAC_TABLE_PATH);

			
			system(cmd);
			fp = fopen(WLAN_MAC_TABLE_PATH, "r");	
			if(fp == NULL){
				unlink(WLAN_MAC_TABLE_PATH);
				return SUCCESS;	
			}
			
			i = 0;
			while(fgets(buf, 100, fp) != NULL){ // get a line.
				/*
				mac table format:
				
				ra{i}	get_mac_table
				MacTabChanged:0
				MAC			AID PSM LDT		RXB		TXB		CTxR	 LTxR
				*******		***	***	***		***		***		***		 ***
				*/
				if(i > 2){
					memset(macAddr, 0, 18);
					strncpy(temp,buf,12); //mac addr len is 12.
					
					//trans temp to upper case
					for(ii = 0; ii < 12; ii++){
						if(temp[ii] >= 'a' && temp[ii] <= 'z')
							temp[ii] = temp[ii] - 32;
					}
					sprintf(macAddr,"%c%c:%c%c:%c%c:%c%c:%c%c:%c%c",temp[0],temp[1],temp[2],temp[3],temp[4],temp[5],temp[6],temp[7],temp[8],temp[9],temp[10],temp[11]);

					if(strlen(macAddr) > 1){ //there may have '\n' if there is no mac addr,we should skip it.
						//get mac entry node,like ENTRY0/ENTRY1/...
						sprintf(nodeName[1], "Entry%d", i - 3);
						//strcpy(nodeName[1],entry);
						curNode=get_web_node(node, nodeName);

						if(curNode != NULL){							
							mxmlElementSetAttr(curNode, "MAC", macAddr);

							/*we should set ip here.*/
							memset(temp, 0, sizeof(temp));
							//transfer macAddr to upper string.
							//strupr(macAddr);
							if(getIpFromArp(macAddr, temp) == 1){
								mxmlElementSetAttr(curNode, "IP", temp);
							}
							else{
								//we have not find this ip in arp table
								//send rarp request here. it may need in future.
							}
						}
					}
				}
				i++;
			}
			fclose(fp);
			unlink(WLAN_MAC_TABLE_PATH);
			//update mac table entry num.
			if(i > 4){// i is always larger than 3.
				strcpy(nodeName[1], COMMON);
				curNode = get_web_node(node, nodeName);
				strcpy(attrName, "NUM");
				sprintf(attrValue, "%d", i - 4); 
				mxmlElementSetAttr(curNode, attrName, attrValue);
			}
		}
	}

	
	return SUCCESS;	
	
#endif
}

#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7601E)|| defined(MT7592)
void getMacEntryByIndex( mac_table_t *pMacTable, int index )
{
	
	if ( pMacTable == NULL || (index > 4 || index < 0) ) {
		return;
	}

	struct iwreq wrq;
	RT_802_11_MAC_TABLE *mp = NULL;
	int i, j, ret, socket_id;
	//char data[2048];
	char *data;

	if ( pMacTable == NULL || (index > 4 || index < 0) ) {
		return;
	}

	data = (char*)malloc(2048);
	if (data == NULL)
		return;
	
	memset( data, 0, sizeof(data) );
	sprintf(wrq.ifr_name, "%s%d", "ra",index);
	wrq.u.data.length = sizeof(data);
 	wrq.u.data.pointer = data;
 	wrq.u.data.flags = 0;

	socket_id = socket( AF_INET, SOCK_DGRAM, 0 );
	if ( socket_id == -1 ) {
		printf( "==>getMacEntryByIndex(): create socket fail.\n" );
		free(data);
		return;
	}
	ret = ioctl( socket_id, RTPRIV_IOCTL_GET_MAC_TABLE, &wrq );
	if ( ret != 0 ) {
		printf("==>getMacEntryByIndex(): ioctl open fail\n");
		free(data);
    	close(socket_id);
		return;
	}

	mp = (RT_802_11_MAC_TABLE *)wrq.u.data.pointer;
	if ( mp->Num == 0 ) {
		printf("==>getMacEntryByIndex(): no sta existance\n");
		free(data);
		close(socket_id);	
		return;
	}

	for ( i = 0, j = 0; j < MAX_LEN_OF_MAC_TABLE; j++ ) {
	#if defined(RT5392) || defined(MT7601E)|| defined(MT7592)
		if ( index == (int)mp->Entry[j].ApIdx ) {
	#else
		if ( index == (int)mp->Entry[j].Psm ) {
	#endif
			if ( mp->Entry[j].Aid == 0 ) {
				 //close(socket_id);	
				 break;
			}
			memcpy(pMacTable->macEntry[i].Addr, mp->Entry[j].Addr, MAC_ADDR_LENGTH);
#if defined(TCSUPPORT_CT_PHONEAPP)
			pMacTable->macEntry[i].AvgRssi0 = mp->Entry[i].AvgRssi0;
			pMacTable->macEntry[i].AvgRssi1 = mp->Entry[i].AvgRssi1;
			pMacTable->macEntry[i].AvgRssi2 = mp->Entry[i].AvgRssi2;
#endif
			i++;
		}
	}
	pMacTable->Num = i;
	free(data);
	close(socket_id);
	
	return;
}
#endif

int getIpFromArp(char *macAddr, char *ip)
{
	FILE *fp = NULL;
	char cmd[64];
	char buf[100];
	char mactemp[18]={'\0'};
	int i=0;

	memset(cmd, 0, sizeof(cmd));
	memset(buf, 0, sizeof(buf));
	strncpy(mactemp, macAddr, 18);
	for(i = 0; i < 18; i ++)
	{
	   mactemp[i] = tolower(mactemp[i]);
	}
	
	sprintf(cmd, "/bin/cp /proc/net/arp %s", ARP_TABLE_PATH);
	system(cmd);

	fp = fopen(ARP_TABLE_PATH, "r");
	while(fgets(buf, 100, fp) != NULL){		
	
		if((strstr(buf, macAddr) != NULL) ||(strstr(buf, mactemp) != NULL)){
			strncpy(ip, buf, 15); //IP addr max length is 15.
			fclose(fp);
			return 1;
		}
		memset(buf,0,sizeof(buf));
	}
	fclose(fp);
	return 0;
}
#if defined (CWMP) && defined(TR111)
//"DhcpClientLimit", SINGLE_TYPE,NULL, NULL, NULL, NULL, NULL, NULL},
/*______________________________________________________________________________
**	dhcpClientLimit_init
**
**	descriptions:
**		the init function of DhcpClientLimit.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
dhcpClientLimit_init(void){
	int ret=0;
	cfg_node_t node={
			.name="DhcpClientLimit",
			.type=SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=NULL,
			.cfg_boot=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}
//"DhcpClient",	UPDATE_TYPE,NULL, NULL, device_read, NULL, NULL, NULL},
/*______________________________________________________________________________
**	dhcpClient_init
**
**	descriptions:
**		the init function of dhcp device.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
dhcpClient_init(void){
	int ret=0;
	cfg_node_t node={
			.name="DhcpClient",
			.type=UPDATE_TYPE|WEB_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=dhcpClient_read,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=NULL,
			.cfg_boot=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}

/*_____________________________________________________________________________
**      function name: dhcpClient_read
**      descriptions:
**            To create Info cfg node and filled those attribution value.
**
**      parameters:
**            top:   Put the top of cfg node tree.
**            name: Put the sub Info cfg node name.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**      	dhcpClientInfo_read
**
**      revision:
**      1. Renyu 2009/11/19
**____________________________________________________________________________
*/
int
dhcpClient_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr){
	mxml_node_t *curNode = NULL;
	mxml_node_t *parent;
	unsigned long iCount = 0;
	char attrStr[16] = {0};
	char *buf = NULL;
	char value[65] = {0}; //the maximum of oui, sn, pclass
	struct stat fileStat;
	unsigned long i;
	
	strcpy(name[1],"Info");
	
 	parent = mxmlFindElement(top, top, name[0], NULL, NULL, MXML_DESCEND);
	
	curNode = mxmlFindElement(parent, parent, name[1],NULL, NULL, MXML_DESCEND);
	if(curNode != NULL){
		//tcdbg_printf("\nunset node: DhcpClient\n");
		mxmlDelete(curNode);
	}	

	curNode = get_web_node(top, name);
	if(curNode==NULL){
		tcdbg_printf("\ncreate new Node: Info failed");
		return FAIL;
	}
	
	//set attribute: Count for default value
	sprintf(value, "%lu", iCount);
	mxmlElementSetAttr(curNode, "Count", value);

	//get file size
	memset(&fileStat, 0, sizeof(struct stat));
	if(stat(DEVICE_INFO_FILE, &fileStat) != 0) {
		return FAIL;
	}
	if(fileStat.st_size == 0)
		return FAIL;
	//tcdbg_printf("\nfile length is %d****\n", fileStat.st_size);
	buf = (char *)malloc(fileStat.st_size);
	if(buf==NULL){
		perror("malloc fail");
		return FAIL;
	}
	memset(buf,0, fileStat.st_size);
	fileRead(DEVICE_INFO_FILE, buf, fileStat.st_size);
	if(0 == strlen(buf))
	{
		free(buf);
		return FAIL;
	}
	//get total count of Dhcp client
	iCount = get_dhcpClient_count_vlaue(buf, "totalCount", COUNT_LENGTH, 10);

	//set attribute: Count
	if(0 != iCount)
	{
		sprintf(value, "%lu", iCount);
		mxmlElementSetAttr(curNode, "Count", value);
	}
	
	//set Attributes: oui(i) sn(i) pclass(i)
	for(i = 0; i < iCount; i++)
	{
		//set oui value
		memset(value, 0, sizeof(value));
		sprintf(attrStr, "oui%lu", i);
		get_dhcpClient_Info_vlaue(buf, attrStr, OUI_LENGTH, value);
		mxmlElementSetAttr(curNode, attrStr, value);

		//set sn value
		memset(value, 0, sizeof(value));
		sprintf(attrStr, "sn%lu", i);
		get_dhcpClient_Info_vlaue(buf, attrStr, SN_LENGTH, value);
		mxmlElementSetAttr(curNode, attrStr, value);

		//set pclass value
		memset(value, 0, sizeof(value));
		sprintf(attrStr, "pclass%lu", i);
		get_dhcpClient_Info_vlaue(buf, attrStr, PCLASS_LENGTH, value);
		mxmlElementSetAttr(curNode, attrStr, value);
	}

	free(buf);
	return SUCCESS;
}/*end info_read*/

/*_____________________________________________________________________________
**      function name: get_dhcpClient_count_vlaue
**      descriptions:
**            Get the value of keyword
**
**      parameters:
**            str:   Put the top of cfg node tree that is used to find out value.
**            keyword:  Put the keyword
**            base: 10 base or 16 base
**
**      global:
**             None
**
**      return:
**             Get the value
**
**      call:
**   	 None
**
**      revision:
**      1. Renyu 2009/11/20
**____________________________________________________________________________
*/
unsigned long
get_dhcpClient_count_vlaue(char *str, char *keyword, int length, int base){
	char *pch_itemname = NULL;
	char *pch_temp = NULL;
	char value[11]={0};
	
	pch_itemname = strstr(str, keyword);
	if (pch_itemname != NULL){
		pch_temp = strstr(pch_itemname,"= ");
		if (pch_temp != NULL){
			strncpy(value,pch_temp+2, length);
			return strtoul(value,NULL, base);
		}
	}
	return 0;
}/*end get_key_value*/
/*_____________________________________________________________________________
**      function name: get_dhcpClient_Info_vlaue
**      descriptions:
**            Get the value of keyword
**
**      parameters:
**            str:   Put the top of cfg node tree that is used to find out value.
**            keyword:  Put the keyword
**            val: value os the keyword
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**   	 None
**
**      revision:
**      1. Renyu 2009/11/20
**____________________________________________________________________________
*/
int
get_dhcpClient_Info_vlaue(char *str, char *keyword, int length, char *val){
	char *pch_itemname = NULL;
	char *pch_temp = NULL;
	
	pch_itemname = strstr(str, keyword);
	if (pch_itemname != NULL){
		pch_temp = strstr(pch_itemname,"= ");
		if (pch_temp != NULL){
			strncpy(val,pch_temp+2, length);
			val[length] = '\0';
			return SUCCESS;
		}
	}
	return FAIL;
}/*end get_key_value*/
#endif
#ifdef CWMP
//add by brian
int
cwmp_f5loopback_diagnostic_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="CATMDiagnostic",
			.type=WEB_TYPE|UPDATE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=cwmp_f5loopback_diagnostic_read,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=NULL,
			.cfg_boot=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}

int
cwmp_f5loopback_diagnostic_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr)
{
	char retbuf[30] = {0};
	char attrName[32] = {0};
	mxml_node_t *curNode = NULL;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	
	memset(nodeName,0,sizeof(nodeName));

	strcpy(nodeName[0], CWMPATMDIAGNOSTIC);
	strcpy(nodeName[1], DIAGNOSTICSUB);

	curNode=(mxml_node_t *)get_web_node(top,nodeName);

	if(curNode==NULL)
	{
		tcdbg_printf("\r\nno node!");
		return FAIL;
	}
	
	get_profile_str("DiagnosticState=",retbuf, sizeof(retbuf), NO_QMARKS,ATM_F5LOOPBACK_DIAGNOSTIC_PATH);
	//tcdbg_printf("\r\nDiagnosticState is %s",retbuf);
	strcpy(attrName,"DiagnosticState");
	mxmlElementSetAttr(curNode,attrName, retbuf);

	get_profile_str("DiagnosticTime=",retbuf, sizeof(retbuf), NO_QMARKS,ATM_F5LOOPBACK_DIAGNOSTIC_PATH);
	//tcdbg_printf("\r\nDiagnosticTime is %s",retbuf);
	if(strtoul(retbuf,NULL,0))
	{
		strcpy(attrName,"DiagnosticTime");
		mxmlElementSetAttr(curNode,attrName, retbuf);
	}
	return SUCCESS;
}

//add by brian
void splitValue(char *strbuf,char*attr,mxml_node_t *curNode)
{ 
	int i = 0;
	char *tmpBuf_total = NULL;
	int reallen = 0,splitnum = 0;
	char attrName[20] = {'\0'};
	char attrNameBuf[64] = {'\0'};
	char attrNumBuf[10] = {'\0'};
	char attrBuf[576] = {'\0'};
	char index[10] = {'\0'};
	//mxml_node_t *curNode = NULL;
	tmpBuf_total = strbuf;
	reallen = strlen(tmpBuf_total);
	strcpy(attrName,attr);

	if(reallen % MAX_SPLIT_LEN)
		splitnum = (reallen >> MAX_SPLIT_OFFSET) + 1;
	else
		splitnum = reallen >> MAX_SPLIT_OFFSET;

	if(splitnum == 1)//if there is only one attr of this struct, set buf to cfg node.
	{ 
		mxmlElementSetAttr(curNode,attrName, tmpBuf_total);
	}
	else
	{
		//set the split number
		strcpy(attrNameBuf, attrName);
		strcat(attrNameBuf, "Len");
		sprintf(attrNumBuf, "%d", splitnum);
		mxmlElementSetAttr(curNode,attrNameBuf, attrNumBuf);
		
		//split the data and store
		for(i = 0; i < splitnum; i++)//set attr like attrName0,attrName1... to cfg node.
		{	
			if(i == splitnum - 1)
			{
				strncpy(attrBuf, tmpBuf_total + i*MAX_SPLIT_LEN, reallen % MAX_SPLIT_LEN);
			}
			else
			{
				strncpy(attrBuf, tmpBuf_total + i*MAX_SPLIT_LEN, MAX_SPLIT_LEN);
			}
			strcpy(attrNameBuf, attrName);
			sprintf(index, "%d", i);
			strcat(attrNameBuf, index);
			mxmlElementSetAttr(curNode,attrNameBuf, attrBuf);
			memset(attrBuf, 0, sizeof(attrBuf));
		}
	}
}

int
cwmp_dsl_diagnostic_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="CDSLDiagnostic",
			.type=WEB_TYPE|UPDATE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=cwmp_dsl_diagnostic_read,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=NULL,
			.cfg_boot=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}

int
cwmp_dsl_diagnostic_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr)
{
	char *tmpBuf = NULL;
	char *tmpBuf_total = NULL;
	char tmppath[100] = {0};
	char retbuf[30] = {0};
	char attrName[32] = {0};
	mxml_node_t *curNode = NULL;
	
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	int i=0;

	memset(nodeName,0,sizeof(nodeName));

	strcpy(nodeName[0], CWMPDSLDIAGNOSTIC);
	strcpy(nodeName[1], DIAGNOSTICSUB);

	curNode=(mxml_node_t *)get_web_node(top,nodeName);

	if(curNode==NULL)
	{
		tcdbg_printf("\r\nno node!");
		return FAIL;
	}
	
	if(attr != NULL)
	{
		tcdbg_printf("the attr is %s",attr);
		if(strncmp(attr,"HLINpsds",8) == 0)
		{
			tmpBuf_total = malloc(MAX_BUF_SIZE*4);
			if(tmpBuf_total == NULL)
			{
				tcdbg_printf("\r\nerror!");
				return FAIL;
			}
			
			tmpBuf = malloc(MAX_BUF_SIZE*2);
			if(tmpBuf == NULL)
			{
				tcdbg_printf("\r\nerror!");
				free(tmpBuf_total);
				return FAIL;
			}
			for(i = 1; i < 3 ;i++)
			{
				memset(tmpBuf,0,sizeof(MAX_BUF_SIZE*2));
				sprintf(tmppath,"%s%d",DSL_DIAGNOSTIC_PATH,i);
				fileRead(tmppath, tmpBuf, MAX_BUF_SIZE*2);
				sprintf(tmpBuf_total,"%s",tmpBuf);
			}
			free(tmpBuf);
			strcpy(attrName,"HLINpsds");
			splitValue(tmpBuf_total,attrName,curNode);
			free(tmpBuf_total);		
		}
		else if(strncmp(attr,"QLNpsds",7) == 0)
		{
			tmpBuf_total = malloc(MAX_BUF_SIZE*2);
			if(tmpBuf_total == NULL)
			{
				tcdbg_printf("\r\nerror!");
				return FAIL;
			}

			memset(tmpBuf_total,0,sizeof(MAX_BUF_SIZE*2));
			sprintf(tmppath,"%s%d",DSL_DIAGNOSTIC_PATH,QLINpsds_PROFILE_INDEX);
			fileRead(tmppath, tmpBuf_total, MAX_BUF_SIZE*2);
			strcpy(attrName,"QLNpsds");
			splitValue(tmpBuf_total,attrName,curNode);
			free(tmpBuf_total);
		}
		else if(strncmp(attr,"SNRpsds",7) == 0)
		{
			tmpBuf_total = malloc(MAX_BUF_SIZE*2);
			if(tmpBuf_total == NULL)
			{
				tcdbg_printf("\r\nerror!");
				return FAIL;
			}
			tcdbg_printf("\r\nenter SNRpsds");
			memset(tmpBuf_total,0,sizeof(MAX_BUF_SIZE*2));
			sprintf(tmppath,"%s%d",DSL_DIAGNOSTIC_PATH,SNRpsds_PROFILE_INDEX);
			fileRead(tmppath, tmpBuf_total, MAX_BUF_SIZE*2);

			strcpy(attrName,"SNRpsds");
			splitValue(tmpBuf_total,attrName,curNode);
			free(tmpBuf_total);
		}
		else if(strncmp(attr,"BITSpsds",8) == 0)
		{
			tmpBuf_total = malloc(MAX_BUF_SIZE*2);
			if(tmpBuf_total == NULL)
			{
				tcdbg_printf("\r\nerror!");
				return FAIL;
			}
			tcdbg_printf("\r\nenter BITSpsds");
			memset(tmpBuf_total,0,sizeof(MAX_BUF_SIZE*2));
			sprintf(tmppath,"%s%d",DSL_DIAGNOSTIC_PATH,BITSpsds_PROFILE_INDEX);
			fileRead(tmppath, tmpBuf_total, MAX_BUF_SIZE*2);

			strcpy(attrName,"BITSpsds");
			splitValue(tmpBuf_total,attrName,curNode);
			free(tmpBuf_total);
		}
		else if(strncmp(attr,"GAINSpsds",9) == 0)
		{
			tmpBuf_total = malloc(MAX_BUF_SIZE*2);
			if(tmpBuf_total == NULL)
			{
				tcdbg_printf("\r\nerror!");
				return FAIL;
			}

			memset(tmpBuf_total,0,sizeof(MAX_BUF_SIZE*2));
			sprintf(tmppath,"%s%d",DSL_DIAGNOSTIC_PATH,GAINSpsds_PROFILE_INDEX);
			fileRead(tmppath, tmpBuf_total, MAX_BUF_SIZE*2);

			strcpy(attrName,"GAINSpsds");
			splitValue(tmpBuf_total,attrName,curNode);
			free(tmpBuf_total);
		}
#if defined(TCSUPPORT_CT_WAN_PTM)
		else if(strncmp(attr,"HLOGpsds",8) == 0)
		{
			tmpBuf_total = malloc(MAX_BUF_SIZE*2);
			if(tmpBuf_total == NULL)
			{
				tcdbg_printf("\r\nerror!");
				return FAIL;
			}
		
			memset(tmpBuf_total,0,sizeof(MAX_BUF_SIZE*2));
			sprintf(tmppath,"%s%d",DSL_DIAGNOSTIC_PATH,HLOGpsds_PROFILE_INDEX);
			fileRead(tmppath, tmpBuf_total, MAX_BUF_SIZE*2);
			
			strcpy(attrName,"HLOGpsds");
			splitValue(tmpBuf_total,attrName,curNode);
			free(tmpBuf_total);
		}
		else if(strncmp(attr,"HLOGpsus",8) == 0)
		{
			tmpBuf_total = malloc(MAX_BUF_SIZE*2);
			if(tmpBuf_total == NULL)
			{
				tcdbg_printf("\r\nerror!");
				return FAIL;
			}
		
			memset(tmpBuf_total,0,sizeof(MAX_BUF_SIZE*2));
			sprintf(tmppath,"%s%d",DSL_DIAGNOSTIC_PATH,HLOGpsus_PROFILE_INDEX);
			fileRead(tmppath, tmpBuf_total, MAX_BUF_SIZE*2);
			
			strcpy(attrName,"HLOGpsus");
			splitValue(tmpBuf_total,attrName,curNode);
			free(tmpBuf_total);
		}
		if(strncmp(attr,"HLINpsus",8) == 0)
		{
			tmpBuf_total = malloc(MAX_BUF_SIZE*4);
			if(tmpBuf_total == NULL)
			{
				tcdbg_printf("\r\nerror!");
				return FAIL;
			}
			
			tmpBuf = malloc(MAX_BUF_SIZE*2);
			if(tmpBuf == NULL)
			{
				tcdbg_printf("\r\nerror!");
				free(tmpBuf_total);
				return FAIL;
			}
			for(i = 9; i < 11 ;i++)
			{
				memset(tmpBuf,0,sizeof(MAX_BUF_SIZE*2));
				sprintf(tmppath,"%s%d",DSL_DIAGNOSTIC_PATH,i);
				fileRead(tmppath, tmpBuf, MAX_BUF_SIZE*2);
				sprintf(tmpBuf_total,"%s",tmpBuf);
			}
			free(tmpBuf);
			strcpy(attrName,"HLINpsus");
			splitValue(tmpBuf_total,attrName,curNode);
			free(tmpBuf_total);		
		}
		else if(strncmp(attr,"QLNpsus",7) == 0)
		{
			tmpBuf_total = malloc(MAX_BUF_SIZE*2);
			if(tmpBuf_total == NULL)
			{
				tcdbg_printf("\r\nerror!");
				return FAIL;
			}
		
			memset(tmpBuf_total,0,sizeof(MAX_BUF_SIZE*2));
			sprintf(tmppath,"%s%d",DSL_DIAGNOSTIC_PATH,QLNpsus_PROFILE_INDEX);
			fileRead(tmppath, tmpBuf_total, MAX_BUF_SIZE*2);
			
			strcpy(attrName,"QLNpsus");
			splitValue(tmpBuf_total,attrName,curNode);
			free(tmpBuf_total);
		}
		else if(strncmp(attr,"SNRpsus",7) == 0)
		{
			tmpBuf_total = malloc(MAX_BUF_SIZE*2);
			if(tmpBuf_total == NULL)
			{
				tcdbg_printf("\r\nerror!");
				return FAIL;
			}
		
			memset(tmpBuf_total,0,sizeof(MAX_BUF_SIZE*2));
			sprintf(tmppath,"%s%d",DSL_DIAGNOSTIC_PATH,SNRpsus_PROFILE_INDEX);
			fileRead(tmppath, tmpBuf_total, MAX_BUF_SIZE*2);
			
			strcpy(attrName,"SNRpsus");
			splitValue(tmpBuf_total,attrName,curNode);
			free(tmpBuf_total);
		}
		else if(strncmp(attr,"SATNds",6) == 0)
		{
			tmpBuf_total = malloc(MAX_BUF_SIZE*2);
			if(tmpBuf_total == NULL)
			{
				tcdbg_printf("\r\nerror!");
				return FAIL;
			}
		
			memset(tmpBuf_total,0,sizeof(MAX_BUF_SIZE*2));
			sprintf(tmppath,"%s%d",DSL_DIAGNOSTIC_PATH,SATNds_PROFILE_INDEX);
			fileRead(tmppath, tmpBuf_total, MAX_BUF_SIZE*2);
			
			strcpy(attrName,"SATNds");
			splitValue(tmpBuf_total,attrName,curNode);
			free(tmpBuf_total);
		}
		else if(strncmp(attr,"SATNus",6) == 0)
		{
			tmpBuf_total = malloc(MAX_BUF_SIZE*2);
			if(tmpBuf_total == NULL)
			{
				tcdbg_printf("\r\nerror!");
				return FAIL;
			}
		
			memset(tmpBuf_total,0,sizeof(MAX_BUF_SIZE*2));
			sprintf(tmppath,"%s%d",DSL_DIAGNOSTIC_PATH,SATNus_PROFILE_INDEX);
			fileRead(tmppath, tmpBuf_total, MAX_BUF_SIZE*2);
			
			strcpy(attrName,"SATNus");
			splitValue(tmpBuf_total,attrName,curNode);
			free(tmpBuf_total);
		}
		else if(strncmp(attr,"LATNds",6) == 0)
		{
			tmpBuf_total = malloc(MAX_BUF_SIZE*2);
			if(tmpBuf_total == NULL)
			{
				tcdbg_printf("\r\nerror!");
				return FAIL;
			}
		
			memset(tmpBuf_total,0,sizeof(MAX_BUF_SIZE*2));
			sprintf(tmppath,"%s%d",DSL_DIAGNOSTIC_PATH,LATNds_PROFILE_INDEX);
			fileRead(tmppath, tmpBuf_total, MAX_BUF_SIZE*2);
			
			strcpy(attrName,"LATNds");
			splitValue(tmpBuf_total,attrName,curNode);
			free(tmpBuf_total);
		}
		else if(strncmp(attr,"LATNus",6) == 0)
		{
			tmpBuf_total = malloc(MAX_BUF_SIZE*2);
			if(tmpBuf_total == NULL)
			{
				tcdbg_printf("\r\nerror!");
				return FAIL;
			}
		
			memset(tmpBuf_total,0,sizeof(MAX_BUF_SIZE*2));
			sprintf(tmppath,"%s%d",DSL_DIAGNOSTIC_PATH,LATNus_PROFILE_INDEX);
			fileRead(tmppath, tmpBuf_total, MAX_BUF_SIZE*2);
			
			strcpy(attrName,"LATNus");
			splitValue(tmpBuf_total,attrName,curNode);
			free(tmpBuf_total);
		}
#endif
		else
		{
			sprintf(tmppath,"%s%d",DSL_DIAGNOSTIC_PATH,OTHER_PROFILE_INDEX);
			//set the diagnosticstate
			//memset(retbuf,0,sizeof(retbuf));//no used,because do it in function get_profile_str()
			get_profile_str("DiagnosticState=",retbuf, sizeof(retbuf), NO_QMARKS,tmppath);
			strcpy(attrName,"DiagnosticState");
			mxmlElementSetAttr(curNode,attrName, retbuf);
			//set ACTPSDds
			get_profile_str("ACTPSDds=",retbuf, sizeof(retbuf), NO_QMARKS,tmppath);
			strcpy(attrName,"ACTPSDds");
			mxmlElementSetAttr(curNode,attrName, retbuf);
			//set ACTPSDus
			get_profile_str("ACTPSDus=",retbuf, sizeof(retbuf), NO_QMARKS,tmppath);
			strcpy(attrName,"ACTPSDus");
			mxmlElementSetAttr(curNode,attrName, retbuf);
			//set ACTATPds
			get_profile_str("ACTATPds=",retbuf, sizeof(retbuf), NO_QMARKS,tmppath);
			strcpy(attrName,"ACTATPds");
			mxmlElementSetAttr(curNode,attrName, retbuf);
			//set ACTATPus
			get_profile_str("ACTATPus=",retbuf, sizeof(retbuf), NO_QMARKS,tmppath);
			strcpy(attrName,"ACTATPus");
			mxmlElementSetAttr(curNode,attrName, retbuf);
			//set HLINSCds
			get_profile_str("HLINSCds=",retbuf, sizeof(retbuf), NO_QMARKS,tmppath);
			strcpy(attrName,"HLINSCds");
			mxmlElementSetAttr(curNode,attrName, retbuf);
#if defined(TCSUPPORT_CT_WAN_PTM)
			//set HLINSCus
			get_profile_str("HLINSCus=",retbuf, sizeof(retbuf), NO_QMARKS,tmppath);
			strcpy(attrName,"HLINSCus");
			mxmlElementSetAttr(curNode,attrName, retbuf);
			//set HLINGds
			get_profile_str("HLINGds=",retbuf, sizeof(retbuf), NO_QMARKS,tmppath);
			strcpy(attrName,"HLINGds");
			mxmlElementSetAttr(curNode,attrName, retbuf);
			//set HLINGus
			get_profile_str("HLINGus=",retbuf, sizeof(retbuf), NO_QMARKS,tmppath);
			strcpy(attrName,"HLINGus");
			mxmlElementSetAttr(curNode,attrName, retbuf);
			//set HLOGGds
			get_profile_str("HLOGGds=",retbuf, sizeof(retbuf), NO_QMARKS,tmppath);
			strcpy(attrName,"HLOGGds");
			mxmlElementSetAttr(curNode,attrName, retbuf);
			//set HLOGGus
			get_profile_str("HLOGGus=",retbuf, sizeof(retbuf), NO_QMARKS,tmppath);
			strcpy(attrName,"HLOGGus");
			mxmlElementSetAttr(curNode,attrName, retbuf);
			//set HLOGMTds
			get_profile_str("HLOGMTds=",retbuf, sizeof(retbuf), NO_QMARKS,tmppath);
			strcpy(attrName,"HLOGMTds");
			mxmlElementSetAttr(curNode,attrName, retbuf);
			//set HLOGMTus
			get_profile_str("HLOGMTus=",retbuf, sizeof(retbuf), NO_QMARKS,tmppath);
			strcpy(attrName,"HLOGMTus");
			mxmlElementSetAttr(curNode,attrName, retbuf);
			//set QLNGds
			get_profile_str("QLNGds=",retbuf, sizeof(retbuf), NO_QMARKS,tmppath);
			strcpy(attrName,"QLNGds");
			mxmlElementSetAttr(curNode,attrName, retbuf);
			//set QLNGus
			get_profile_str("QLNGus=",retbuf, sizeof(retbuf), NO_QMARKS,tmppath);
			strcpy(attrName,"QLNGus");
			mxmlElementSetAttr(curNode,attrName, retbuf);
			//set QLNMTds
			get_profile_str("QLNMTds=",retbuf, sizeof(retbuf), NO_QMARKS,tmppath);
			strcpy(attrName,"QLNMTds");
			mxmlElementSetAttr(curNode,attrName, retbuf);
			//set QLNMTus
			get_profile_str("QLNMTus=",retbuf, sizeof(retbuf), NO_QMARKS,tmppath);
			strcpy(attrName,"QLNMTus");
			mxmlElementSetAttr(curNode,attrName, retbuf);
			//set SNRGds
			get_profile_str("SNRGds=",retbuf, sizeof(retbuf), NO_QMARKS,tmppath);
			strcpy(attrName,"SNRGds");
			mxmlElementSetAttr(curNode,attrName, retbuf);
			//set SNRGus
			get_profile_str("SNRGus=",retbuf, sizeof(retbuf), NO_QMARKS,tmppath);
			strcpy(attrName,"SNRGus");
			mxmlElementSetAttr(curNode,attrName, retbuf);
			//set SNRMTds
			get_profile_str("SNRMTds=",retbuf, sizeof(retbuf), NO_QMARKS,tmppath);
			strcpy(attrName,"SNRMTds");
			mxmlElementSetAttr(curNode,attrName, retbuf);
			//set SNRMTus
			get_profile_str("SNRMTus=",retbuf, sizeof(retbuf), NO_QMARKS,tmppath);
			strcpy(attrName,"SNRMTus");
			mxmlElementSetAttr(curNode,attrName, retbuf);
#endif
		}
	}
	else
	{
		tcdbg_printf("the attr is NULL!");
	}

	return SUCCESS;
}
//"LanHostIPInterface",	SINGLE_TYPE,NULL, NULL, NULL, NULL, NULL, NULL},
/*______________________________________________________________________________
**	lanHostIPInterface_init
**
**	descriptions:
**		the init function of dhcp device.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
IPInterface_init(void){
//tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="IPInterface",
			.type=SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=NULL,
			.cfg_boot=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}
#endif
#ifdef CWMP
char *switchpara_adv_table[MAX_NODE_NAME]=
{
	"Common","",
};
char *switchpara_merge_table[MAX_NODE_NAME]=
{
	"switchpara_id","",
};
/*______________________________________________________________________________
**	switchpara_boot
**
**	descriptions:
**		the boot function of switchpara.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**_______________________________________________________________________________
*/
int
switchpara_boot(mxml_node_t *top){
	int i = 0;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	for(i = 0; i < SWITCHPARA_MAX_ENTRY; i++)
	{
		memset(nodeName, 0, sizeof(nodeName));
	  	strcpy(nodeName[0],SWITCHPARA_NODE_NAME);
	  	sprintf(nodeName[1], "%s%d",SUB_NODE_NAME,i);
		switchpara_execute(top,nodeName);
	}
	return SUCCESS;
}

/*______________________________________________________________________________
**	switchpara_init
**
**	descriptions:
**		the init function of operation switch.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
switchpara_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="SwitchPara",
			.type=ADV_FLAG|ENTRY_TYPE|SWITCHPARA_MAX_ENTRY,
			.adv_table=switchpara_adv_table,
			.merge_table=switchpara_merge_table,
			.cfg_read=NULL,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=switchpara_execute,
			.cfg_boot=switchpara_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}
/*_____________________________________________________________________________
**      function name: switchpara_execute
**      descriptions:
**            To execute switchpara node service.
**
**      parameters:
**            node:   Put the switchpara of cfg node tree that is used to switchpara information.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**____________________________________________________________________________
*/
int
switchpara_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	char cmdbuf[100] = {0}; 
	int i = 0;
	int spid = -1;
	char tmp[32] = {0};
	char switchflag[8] = {0},maxbitrate[10] = {0},duplexmode[10] = {0},enable[8] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], SWITCHPARA_NODE_NAME);
	strcpy(nodeName[1], SWITCHPARA_ADV_NODE_NAME);

	if(getAttrValue(top, nodeName, "SwitchNeedOperation", switchflag) != TCAPI_PROCESS_OK)
	{
		tcdbg_printf("\r\nswitchpara_boot:get switch error!");
		return FAIL;
	}
		
	if(!strcmp(switchflag,"Yes"))
	{
		/*get current ipping index*/
		if(get_entry_number(name[1],SUB_NODE_NAME,&spid) != SUCCESS)
		{
			memset(nodeName,0,sizeof(nodeName));
			strcpy(nodeName[0],WEBCURSET);
			strcpy(nodeName[1],SUB_NODE_NAME);
			if(getAttrValue(top, nodeName, "switchpara_id", tmp) == TCAPI_PROCESS_OK)
			{
				spid = atoi(tmp);
			}
			else
			{
				tcdbg_printf("\r\nswitchpara_execute:not fetch index!");
				return FAIL;
			}
		}

		//get parameters and execute the cmd
		strcpy(nodeName[0], SWITCHPARA_NODE_NAME);
		sprintf(nodeName[1], "%s%d",SUB_NODE_NAME,spid);
		
		if((getAttrValue(top, nodeName, "enable", enable) == TCAPI_PROCESS_OK)
			&&(getAttrValue(top, nodeName, "maxBitRate", maxbitrate) == TCAPI_PROCESS_OK)
			&&(getAttrValue(top, nodeName, "duplexMode", duplexmode) == TCAPI_PROCESS_OK))
		{
			if(strcmp(enable,"Yes"))
			{
				sprintf(cmdbuf,SWITCHPARA_CMD_ENABLE,"disable",spid);
				system(cmdbuf);
				return SUCCESS;
			}
			else
			{
				sprintf(cmdbuf,SWITCHPARA_CMD_ENABLE,"enable",spid);
				system(cmdbuf);
				usleep(100000);;
			}	
				
			if(!strcmp(maxbitrate,"10") && !strcmp(duplexmode,"Half"))
			{
				sprintf(cmdbuf,SWITCHPARA_CMD,"10HD",spid);
			}
			else if(!strcmp(maxbitrate,"10") && !strcmp(duplexmode,"Full"))
			{
				sprintf(cmdbuf,SWITCHPARA_CMD,"10FD",spid);
			}
			else if(!strcmp(maxbitrate,"100") && !strcmp(duplexmode,"Half"))
			{
				sprintf(cmdbuf,SWITCHPARA_CMD,"100HD",spid);
			}
			else if(!strcmp(maxbitrate,"100") && !strcmp(duplexmode,"Full"))
			{
				sprintf(cmdbuf,SWITCHPARA_CMD,"100FD",spid);
			}
			else
			{
				sprintf(cmdbuf,SWITCHPARA_CMD,"auto",spid);
			}
			//tcdbg_printf("\r\n the cmd is  %s",cmdbuf);
			system(cmdbuf);
		}
	}
	
	return SUCCESS;
}
#endif

#ifdef TCSUPPORT_SYSLOG
#if defined(TCSUPPORT_C1_CUC)
typedef struct syslog_node_s
{
	char attr[ATTR_SIZE];
	char value[SYSLOG_VALUE_LEN];
}syslog_node_t;
#endif

//"syslog",	SINGLE_TYPE,	NULL, NULL,NULL, syslog_write, syslog_verify, syslog_execute},
/*______________________________________________________________________________
**	syslog_boot
**
**	descriptions:
**		the boot function of syslog.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**_______________________________________________________________________________
*/
int
syslog_boot(mxml_node_t *top){
	char buf[8]; 
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];

	memset(buf, 0, sizeof(buf));
	memset(nodeName, 0, sizeof(nodeName));
	
	strcpy(nodeName[0], SYSLOG_NODE_NAME);
	strcpy(nodeName[1], SUB_NODE_NAME);

	if(getAttrValue(top, nodeName, "LogEnable", buf) != TCAPI_PROCESS_OK){
		return FAIL;
	}
	if(!strcmp(buf,"Yes")){
		system("/sbin/syslogd -m 0 &");
		system("/sbin/klogd &");
	}
	
	return SUCCESS;
}

/*______________________________________________________________________________
**	syslog_init
**
**	descriptions:
**		the init function of syslog.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
syslog_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="SysLog",
#if defined(TCSUPPORT_C1_CUC)
			.type=SINGLE_TYPE|UPDATE_TYPE,
#else
                     .type=SINGLE_TYPE,
#endif
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=syslog_read,
			.cfg_write=syslog_write,
			.cfg_verify=syslog_verify,
			.cfg_execute=syslog_execute,
			.cfg_boot=syslog_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}
/*_____________________________________________________________________________
**      function name: syslog_read
**      descriptions:
**            To create syslog cfg node.
**
**      parameters:
**            top:    Put the top of cfg node tree.
**            name:  NULL
**
**      return:
**             Success: 	0
**		  Otherwise: 	-1
**____________________________________________________________________________
*/
int
syslog_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr)
{
#if defined(TCSUPPORT_C1_CUC)
	syslog_node_t syslog_node[]=
	{
		{"LogEnable","No"},
		{"WriteLevel","7"},
		{"DisplayLevel","7"},
		{"WriteFlashInterval","300"},
		{"UpdateInterval","5"},
		{"LogUploadEnable","No"},
		{"LogUploadInterval","60"},
		{"AlarmEnable","Yes"},
		{"AlarmLevel","3"},
		{"UploadServer",""},
		{"Username",""},
		{"Password",""},
		{"NumberOfAlarmInfo","100"},
		{"",""}
	};
  	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	int i=0;
	char temp[128]={0};

	memset(nodeName,0, sizeof(nodeName));
	strcpy(nodeName[0], SYSLOG_NODE_NAME);
	strcpy(nodeName[1], ENTRY_NODE_NAME);

	for (i=0; strlen(syslog_node[i].attr)!=0; i++)
	{
		if (getAttrValue(top, nodeName, syslog_node[i].attr, temp) != TCAPI_PROCESS_OK)
		{
			setAttrValue(top,nodeName, syslog_node[i].attr, syslog_node[i].value);	
		}

	}
#endif
	return SUCCESS;
}/*end syslog_read*/

/*______________________________________________________________________________
**	syslog_write
**
**	descriptions:
**		
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**		parant: 	Specify the parant node of  syslog cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
syslog_write(mxml_node_t *top, mxml_node_t *parant){
	int 	i=0;
	mxml_node_t *node = NULL;
	const char *tmp = NULL;
	FILE *fp = NULL;
	char buf[32] = {0};

	enum syslog_param {
		WRITE_LEVEL,
		WRITE_INTERVAL,
		UPDATE_INTERVAL,			
  		SYSLOG_PARAM_NUM,
	};
	
	char syslog_param[][ATTR_SIZE]=
	{
		{"WriteLevel"},
		{"WriteFlashInterval"},
		{"UpdateInterval"},					
		{""},
	};
	char param_value[SYSLOG_PARAM_NUM][SYSLOG_VALUE_LEN];

	node = mxmlFindElement(parant, parant, SUB_NODE_NAME,
		NULL, NULL,MXML_DESCEND);
	if(node){
		for(i=0; i<SYSLOG_PARAM_NUM; i++) {
			memset(param_value[i], 0, SYSLOG_VALUE_LEN);
			tmp=mxmlElementGetAttr(node, syslog_param[i]);
			if(tmp != NULL)
				strncpy(param_value[i], tmp, SYSLOG_VALUE_LEN);
			else
				printf("\n==>syslog_write error, get %s value failed\n", syslog_param[i]);
		}
	}
	else
		return FAIL;

	/* Write syslog config file "/var/log/syslog.conf"
  	* e.g.
  	* WriteLevel 3
	*/
	fp = fopen(SYSLOG_CONF_PATH, "w");
	if(fp != NULL) {
		for(i=0; i<SYSLOG_PARAM_NUM; i++) {
			sprintf(buf, "%s %s\n", syslog_param[i], param_value[i]);
			fputs(buf, fp);
		}
		fclose(fp);
	}
	else
		return FAIL;

	return SUCCESS;
}/* end syslog_write */

/*______________________________________________________________________________
**	syslog_verify
**
**	descriptions:
**		To check the system parameter of syslog element  is validly or not.
**
**	parameters:
**		node: 		Specify the root of mxml_node_t structure.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
syslog_verify(mxml_node_t *node){
	return SUCCESS;
}/* end syslog_verify */

/*_____________________________________________________________________________
**      function name: syslog_execute
**      descriptions:
**            To execute syslog node service.
**
**      parameters:
**            node:   Put the syslog of cfg node tree that is used to syslog information.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**
**      revision:
**      1. yzwang 2010/7/19
**____________________________________________________________________________
*/
int
syslog_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){

	char buf[8], nodeName[MAX_ARG_NUM][MAX_NODE_NAME];

	/* First, kill the syslogd process */
	system("killall -9 syslogd");
	system("killall -9 klogd");
	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], SYSLOG_NODE_NAME);
	strcpy(nodeName[1], SUB_NODE_NAME);

	if(getAttrValue(top, nodeName, "LogEnable", buf) != TCAPI_PROCESS_OK){
		return FAIL;
	}
	if(!strcmp(buf,"Yes")){/* If syslog is enable, restart the syslogd process */
		unlink("/var/log/messages");
		unlink("/var/log/currLogFile");
		system("/sbin/syslogd -m 0 &");
		system("/sbin/klogd &");
	}
	else {/*syslog disable*/
		system("killall -9 syslogd");
		system("killall -9 klogd");	

		unlink("/var/log/messages");
		unlink("/var/log/currLogFile");	
	}
		
	
	return SUCCESS;
}/* end syslog_execute */
#if defined(TCSUPPORT_C1_CUC)
char *alarmlog_adv_table[MAX_NODE_NAME]=
{
	"Common","",
};

//"AlarmLog",	ADV_FLAG|WEB_TYPE|ENTRY_TYPE|UPDATE_TYPE,	NULL, NULL,NULL, NULL, NULL, NULL},
/*______________________________________________________________________________
**	AlarmLog_init
**
**	descriptions:
**		
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
alarmlog_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="AlarmLog",
			.type=ADV_FLAG|WEB_TYPE|ENTRY_TYPE|UPDATE_TYPE|ALARMLOG_INSTANCE_NUM,
			.adv_table=alarmlog_adv_table,
			.merge_table=NULL,
			.cfg_read=alarmlog_read,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=NULL,
			.cfg_boot=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}
/*_____________________________________________________________________________
**      function name: alarmlog_read
**      descriptions:
**            To create alarmlog cfg node.
**
**      parameters:
**            top:    Put the top of cfg node tree.
**            name:  NULL
**
**      global:
**             None
**
**      return:
**             Success: 	0
**		   FAIL: -1
**
**____________________________________________________________________________
*/
int
alarmlog_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr){
	mxml_node_t *curNode = NULL;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char temp[SYSLOG_VALUE_LEN] = {0};

	memset(nodeName,0,sizeof(nodeName));

	strcpy(nodeName[0],ALARMLOG);
	strcpy(nodeName[1], COMMON_NODE_NAME);
	/*Create a alarmlog node and Get the alarmlog cfg node address pointer*/
	curNode=get_web_node(top, nodeName);
	
	if (curNode)
	{
		if (getAttrValue(top, nodeName, "AlarmNum", temp) != TCAPI_PROCESS_OK)
		{
			setAttrValue(top,nodeName,"AlarmNum","0");
		}
		
		if (getAttrValue(top, nodeName, "CurrALogIndex", temp) != TCAPI_PROCESS_OK)
		{
			setAttrValue(top,nodeName,"CurrALogIndex","0");
		}

		if (getAttrValue(top, nodeName, "AlarmIdNum", temp) != TCAPI_PROCESS_OK)
		{
			setAttrValue(top,nodeName,"AlarmIdNum","1");
		}
	}
	else
	{
		return FAIL;
	}

	return SUCCESS;
}/*end alarmlog_read*/
#endif
#endif
#if defined(TCSUPPORT_TRUE_LANDING_PAGE)
void addNatRuleForLandingPage()
{
        /*for Landing page mechanism
         * to add a nat chain to redirect WAN-access to required web page*/
        system("iptables -t nat -N PRE_PORTAL");
        system("iptables -t nat -A PREROUTING -j PRE_PORTAL");
}
#endif
#if defined(TCSUPPORT_CT_PORTAL_MANAGEMENT) || defined(TCSUPPORT_TRUE_LANDING_PAGE)
/*"Portal", SINGLE_TYPE,NULL, NULL, NULL, NULL, NULL, NULL},*/
/*______________________________________________________________________________
**	portal_boot
**
**	descriptions:
**		the boot function of portal.
**	parameters:
**		top: 		Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
portal_boot(mxml_node_t *top)
{
#if defined(TCSUPPORT_CT_PORTAL_MANAGEMENT)
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char portal_status[2]={0};
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],"Portal");
	strcpy(nodeName[1],"Entry");
	if(getAttrValue(top, nodeName, "Enable", portal_status) != TCAPI_PROCESS_OK)
	{
		tcdbg_printf("boot Fail\n");
		return FAIL;
	}
	system("iptables -t nat -N PRE_PORTAL");
	system("iptables -t nat -A PREROUTING -j PRE_PORTAL");
	if(!strcmp(portal_status,"1"))
	{
		system("iptables -t nat -A PRE_PORTAL -i br0 -p tcp --dport 80 -j REDIRECT");
	}
#endif
#if defined(TCSUPPORT_TRUE_LANDING_PAGE)
	   setLandingPageEnableFlag(top);
        if(1 == isLandingPageEnable())
        {
                addNatRuleForLandingPage();               
        }
#endif
	return SUCCESS;
}
/*______________________________________________________________________________
**	portal_init
**
**	descriptions:
**		the init function of Portal.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
portal_init(void){
	int ret=0;
	cfg_node_t node={
			.name="Portal",
			.type=SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=NULL,
			.cfg_verify=NULL,
#if defined(TCSUPPORT_CT_PORTAL_MANAGEMENT)
			.cfg_execute=portal_execute,
#else
			.cfg_execute=NULL,
#endif
			.cfg_boot=portal_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}
#endif

#if defined(TCSUPPORT_CT_PORTAL_MANAGEMENT)
/*_____________________________________________________________________________
**      function name: portal_execute
**      descriptions:
**            To execute portal node service.
**
**      parameters:
**            node:   Put the portal of cfg node tree that is used to execute portal settings.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**
**      revision:
**      1. agyi 2010/9/29
**____________________________________________________________________________
*/
int
portal_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char portal_status[2]={0};
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],"Portal");
	strcpy(nodeName[1],"Entry");
	if(getAttrValue(top, nodeName, "Enable", portal_status) != TCAPI_PROCESS_OK)
	{
		tcdbg_printf("FAIL");
		return FAIL;
	}
	if(!strcmp(portal_status,"1"))
	{
		system("iptables -t nat -D PRE_PORTAL -i br0 -p tcp --dport 80 -j REDIRECT");
		system("iptables -t nat -A PRE_PORTAL -i br0 -p tcp --dport 80 -j REDIRECT");
	}
	else{
		system("iptables -t nat -F PRE_PORTAL");
	}
	
	return SUCCESS;
}
#endif

/*______________________________________________________________________________
**	webCustom_init
**
**	descriptions:
**		the init function of webCustom.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**______________________________________________________________________________
*/
int
webCustom_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="WebCustom",
			.type=WEB_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=webCustom_read,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=NULL,
			.cfg_boot=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}

char webCustom_attr[][ATTR_SIZE] = 
{
#if defined(TCSUPPORT_CT_E8GUI_TY)
	{"isE8TYSupported"},
#endif
#if defined(TCSUPPORT_CT_MIDWARE)
	{"isMidwareSupported"},
#endif
#if defined(TCSUPPORT_CT_2NTP)
#if defined(TCSUPPORT_CT_NTPSERVERTYPE)
	{"isNtpServerTypeSupported"},
#endif
#endif
#ifdef WSC_AP_SUPPORT
	{"isWPSSupported"},
#endif
#if defined(TCSUPPORT_WLAN_MAXSTANUM_GUI)
	{"isMaxStaNumSupported"},
#endif
#ifdef TCSUPPORT_DMS
	{"isDMSSupported"},
#endif
#if defined(TCSUPPORT_CT_E8B_ADSL)
	{"isAdslVer"},
#endif
#if defined(TCSUPPORT_CT_DSLITE)
	{"isDSLiteSupported"},
#endif
#if defined(TCSUPPORT_CT_DSL_EX)
	{"isDslEx"},
#endif
#if defined(TCSUPPORT_CY)
	{"isCYSupported"},
#endif
#if defined(TCSUPPORT_CUC_2PORT)
	{"isCuc2PortSupported"},
#endif
#if defined(TCSUPPORT_WLAN)
	{"isWLanSupported"},
#if defined(TCSUPPORT_WLAN_AC)
	{"isWLanACSupported"},
#endif
#endif
#if defined(TCSUPPORT_CT_USB)
	{"isCTUsbSupported"},
#endif
#if defined(TCSUPPORT_CT_DHCP_PORT_FILTER)
	{"isCTDHCPPortFilterSupported"},
#endif
#if defined(TCSUPPORT_CT_PPPOEPROXY)
	{"isPPPoEProxySupported"},
#endif
#if defined(TCSUPPORT_CUC_TIME_DISPLAY)
	{"isCUCTimeDispSupported"},
#endif
#if defined(TCSUPPORT_CY_E8_SFU)
	{"isCYE8SFUSupported"},
#endif
#if defined(TCSUPPORT_ECN_SIP)	
	{"isMultiDigitMap"},
#endif
#if defined(TCSUPPORT_CT_PPP_ONDEMAND)
	{"isPPPoEOnDemandSupported"},
#if defined(TCSUPPORT_CT_PPP_ONDEMAND_WEBUI)
	{"isPPPoEOnDemandWEBUISupported"},
#endif
#endif
#if defined(TCSUPPORT_CT_FJ)
	{"isCTFJSupported"},
#endif
#if defined(TCSUPPORT_CT_ECN_GZ)
	{"isCTECNGZSupported"},
#endif
#if defined(TCSUPPORT_CT_2PORTS)
	{"isCT2PORTSSupported"},
#endif
#if defined(TCSUPPORT_CT_1PORT)
	{"isCT1PORTSupported"},
#endif
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
	{"isCTSIMCARDSEPARATIONSupported"},
#endif
#if defined(TCSUPPORT_CT_PON)
	{"isCTPONSupported"},
#endif
#if defined(TCSUPPORT_CT_MONITORCOLLECTOR)
	{"isCTMONITORCOLLECTORSupported"},
#endif
#if defined(TCSUPPORT_CT_PON_C9)
	{"isCTPONC9Supported"},
#endif
#if defined(TCSUPPORT_CT_PON_GD)
	{"isCTGDSupported"},
#endif
#if defined(TCSUPPORT_CT_PON_SC)
	{"isCTSCSupported"},
#endif
#if defined(TCSUPPORT_CT_PROLINE_SUPPORT)
	{"isCTPROLINESupported"},
#endif
#if defined(TCSUPPORT_CT_UPLOAD_ALLINONE)
	{"isAllinoneUploadSupported"},
#endif
#if defined(TCSUPPORT_CT_PON_SN)
	{"isCTPONSNSupported"},
#endif
#if defined(TCSUPPORT_CT_PON_NMG)
	{"isCTPONNMGSupported"},
#endif
#if defined(TCSUPPORT_CT_PON_C7)
	{"isCTPONC7Supported"},
#endif
#if defined(TCSUPPORT_CT_PON_YN)
	{"isCTPONYNSupported"},
#if defined(TCSUPPORT_CT_1PORT)
	{"isCTPONYN1PortSupported"},
#endif
#endif
#if defined(TCSUPPORT_CT_PON_JS)
	{"isCTPONJSSupported"},
#endif
#if defined(TCSUPPORT_CT_PON_GX)
	{"isCTPONGXSupported"},
#endif
#if defined(TCSUPPORT_CT_PON_CY)
	{"isCTPONCYSupported"},
#endif
#if defined(TCSUPPORT_CT_SFU_C9)
	{"isCTSFUC9Supported"},
#endif
#if defined(TCSUPPORT_CT_C9)
	{"isCTC9Supported"},
#endif
#if defined(TCSUPPORT_ECN_SIP) && defined(TCSUPPORT_ECN_MEGACO)
	{"isCTCUCSIPH248Supported"},
#endif
#if defined(TCSUPPORT_CT_PHONEAPP)
	{"isCTPHONEAPPSupported"},
#endif
#if (0 == VOIP)
	{"isCTPONNOVOIP"},
#endif
#if defined(TCSUPPORT_CT_PON_BIND2)
	{"isCTPONBIND2Supported"},
#if defined(TCSUPPORT_CT_PON_BIND2_WEB)
	{"isCTPONBIND2WebSupported"},
#endif
#endif
#if defined(TCSUPPORT_CT_PON_JX)
	{"isCTPONJXSupported"},
#endif
#if defined(TCSUPPORT_CT_1FXS)
	{"isCT1FXSSupported"},
#endif
#if defined(TCSUPPORT_VOIP)
	{"isVOIPSupported"},
#endif
#if defined(TCSUPPORT_CT_ADSL_HN)
	{"isCTADSLHNSupported"},
#endif
#if defined(TCSUPPORT_C7_CUC)
	{"isC7CUCSupported"},
#endif
#if defined(TCSUPPORT_CT_PON_CN_JS)	
	{"isCTPONCNJSSupported"},
#endif
#if defined(TCSUPPORT_CT_ADSL_BIND1)
	{"isXBIND1Supported"},
#endif
#if defined(TCSUPPORT_CT_ADSL_TJ)
	{"isCTADTJSupported"},
#endif
#if defined(TCSUPPORT_CT_PON_HLJ)
	{"isCTPONHLJSupported"},
#endif
#if defined(TCSUPPORT_CMCC)
	{"isCMCCSupported"},
#endif
#if defined(TCSUPPORT_CT_PON_CY_JX)
	{"isCTPONCYJXSupported"},
#endif
#if defined(TCSUPPORT_CT_PON_CQ)
	{"isCTPONCYCQSupported"},
#endif
#if defined(TCSUPPORT_CT_PON_CAU)
	{"isCTPONCAUSupported"},
#endif
#if defined(TCSUPPORT_CUC_PON_SD)
	{"isCUCPONSDSupported"},
#endif
#if defined(TCSUPPORT_CT_JOYME)
	{"isCTJOYMESupported"},
#endif
#if defined(TCSUPPORT_CT_PON_NX)
	{"isCTPONNXSupported"},
#endif
#if defined(TCSUPPORT_CT_PON_CZ_GD)
	{"isCTPONCZGDSupported"},
#endif
#if defined(TCSUPPORT_IPV6)
	{"isIPv6Supported"},
#endif
#if defined(TCSUPPORT_CT_PON_CZ_GD)
	{"isCTPONCZGDSupported"},
#endif
#if defined(TCSUPPORT_CT_TY_LOGO)
	{"isCTPONTYLOGOSupported"},
#endif
#if defined(TCSUPPORT_CT_PON_CZ_GZ) || defined(TCSUPPORT_CT_PON_CZ_QH)
	{"isCTPONOnlyTwoSSIDSupported"},
#endif
#if defined(TCSUPPORT_CT_PON_CZ_GX)
	{"isCTPONCZGXSupported"},
#endif
#if defined(TCSUPPORT_CT_PON_CZ_NX)
	{"isCTPONCZNXSupported"},
#endif
#if defined(TCSUPPORT_CT_PON_SN) || defined(TCSUPPORT_CT_PON_CZ_GX) || defined(TCSUPPORT_CT_PON_CZ_GDCS) || defined(TCSUPPORT_CT_PON_CZ_CQ)
	{"isCTPONOnlyOneSSIDSupported"},
#endif
#if defined(TCSUPPORT_CT_PON_CZ_GD) || defined(TCSUPPORT_CY_E8_SFU)
	{"isWebTYLOGOSupported"},
#endif
#if defined(TCSUPPORT_CT_PON_CZ_GDCS)
	{"isCTPONGDTemperatureSupported"},
#endif
#if defined(TCSUPPORT_CPU_EN7521)
	{"isCTCPU7521Supported"},
#endif
#if defined(TCSUPPORT_CT_PON_CZ_HN)	
	{"isCTPONHNSupported"},
#endif
#if defined(TCSUPPORT_WLAN_MULTI_WPS)
	{"isMultiSupported"},
#endif

	/*Add your attributes above*/
	{""}
};

/*______________________________________________________________________________
**      function name: webCustom_read
**      descriptions:
**            To create webCustom cfg node and filled those attribution value.
**
**      parameters:
**            top:   Put the top of cfg node tree.
**            name: Put the sub webCustom cfg node name.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**
**      revision:
**      
**____________________________________________________________________________
*/
int
webCustom_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr){
	mxml_node_t *curNode = NULL;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char nodeName2[MAX_ARG_NUM][MAX_NODE_NAME];
	int i;
	char buf[10] = {0};
	
	memset(nodeName,0,sizeof(nodeName));

	strcpy(nodeName[0], WEBCURT);
	strcpy(nodeName[1], SUB_NODE_NAME);

	curNode=get_web_node(top,nodeName);

	if(curNode==NULL){
		return FAIL;
	}
	for(i=0; strlen(webCustom_attr[i]) != 0; i++) {
		mxmlElementSetAttr(curNode, webCustom_attr[i], SUPPORTED_YES);
		//tcdbg_printf("==>webCustom_read, set attr %s to %s\n", webCustom_attr[i], SUPPORTED_YES);
	}

	/* set bind2 attribute to yes if no need to register
	memset(nodeName2, 0, sizeof(nodeName));
	strcpy(nodeName2[0], "deviceAccount");
	strcpy(nodeName2[1], "Entry");
	if (getAttrValue(top, nodeName2, "isNoReg", buf) == TCAPI_PROCESS_OK) {
		if (!strcmp(buf, "Yes")) {
			mxmlElementSetAttr(curNode, "isCTPONBIND2Supported", SUPPORTED_YES);
		}
	} */

	return SUCCESS;
}

#if defined(TCSUPPORT_CT_LOOPDETECT)
void loopdetect_modify(mxml_node_t *top)
{
	char loopdetectbuf[260]={0};
	char valuebuf[256] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], "LoopDetect");
	strcpy(nodeName[1], "Entry");

	memset(valuebuf, 0, sizeof(valuebuf));
	memset(loopdetectbuf, 0, sizeof(loopdetectbuf));
	if(getAttrValue(top, nodeName, "LoopExistPeriod", valuebuf) != TCAPI_PROCESS_OK){
		return FAIL;
	}
	sprintf(loopdetectbuf, LOOPDETECT_CMD, LOOP_EXIST_PERIOD, valuebuf);
	system(loopdetectbuf);	

	memset(valuebuf, 0, sizeof(valuebuf));
	memset(loopdetectbuf, 0, sizeof(loopdetectbuf));
	if(getAttrValue(top, nodeName, "LoopCancelPeriod", valuebuf) != TCAPI_PROCESS_OK){
		return FAIL;
	}
	sprintf(loopdetectbuf, LOOPDETECT_CMD, LOOP_CANCEL_PERIOD, valuebuf);
	system(loopdetectbuf);	

	memset(valuebuf, 0, sizeof(valuebuf));
	memset(loopdetectbuf, 0, sizeof(loopdetectbuf));
	if(getAttrValue(top, nodeName, "VlanTag", valuebuf) != TCAPI_PROCESS_OK){
		return FAIL;
	}
	sprintf(loopdetectbuf, LOOPDETECT_CMD, LOOP_VLANTAG, valuebuf);
	system(loopdetectbuf);		

	memset(valuebuf, 0, sizeof(valuebuf));
	memset(loopdetectbuf, 0, sizeof(loopdetectbuf));
	if(getAttrValue(top, nodeName, "EthernetType", valuebuf) != TCAPI_PROCESS_OK){
		return FAIL;
	}
	sprintf(loopdetectbuf, LOOPDETECT_CMD, LOOP_ETHERNET_TYPE, valuebuf);
	system(loopdetectbuf);

	memset(valuebuf, 0, sizeof(valuebuf));
	memset(loopdetectbuf, 0, sizeof(loopdetectbuf));
	if(getAttrValue(top, nodeName, "Enable", valuebuf) != TCAPI_PROCESS_OK){
		return FAIL;
	}
	sprintf(loopdetectbuf, LOOPDETECT_CMD, LOOP_ENABLE, valuebuf);
	system(loopdetectbuf);
	
#if defined(TCSUPPORT_CUC)
	memset(valuebuf, 0, sizeof(valuebuf));
	memset(loopdetectbuf, 0, sizeof(loopdetectbuf));
	if(getAttrValue(top, nodeName, "LoopDetectMask", valuebuf) != TCAPI_PROCESS_OK){
		return FAIL;
	}
	sprintf(loopdetectbuf, LOOPDETECT_CMD, LOOP_DETECT_EANBLE_PORT, valuebuf);
	system(loopdetectbuf);
	
	memset(valuebuf, 0, sizeof(valuebuf));
	memset(loopdetectbuf, 0, sizeof(loopdetectbuf));
	if(getAttrValue(top, nodeName, "LoopAutoShutMask", valuebuf) != TCAPI_PROCESS_OK){
		return FAIL;
	}
	sprintf(loopdetectbuf, LOOPDETECT_CMD, LOOP_AUTO_SHUT_LAN, valuebuf);
	system(loopdetectbuf);

#endif
	return 0;
}
int
loopdetect_boot(mxml_node_t *top){
	loopdetect_modify(top);
	
	return SUCCESS;
}


/*______________________________________________________________________________
**	loopbackdetect_init
**
**	descriptions:
**		the init function of LoopbackDetection.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
loopbackdetect_init(void)
{
	int ret=0;
	cfg_node_t node={
			.name="LoopDetect",
			.type=SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=loopdetect_execute,
			.cfg_boot=loopdetect_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}

int loopdetect_execute(mxml_node_t * top,char name [ ] [ MAX_NODE_NAME ])
{
	loopdetect_modify(top);
	return SUCCESS;
}
#endif

#if defined(TCSUPPORT_PRODUCTIONLINE) && (defined(TCSUPPORT_RESERVEAREA_EXTEND) || defined(TCSUPPORT_C7) || (defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505)))
/* Note: the parameter "flag" is re-defined here */
Key_Parameters_Info prolinePara[]=
{
#ifdef TCSUPPORT_WLAN
	{PL_CP_SSID_FLAG,"WLan_Entry0","SSID",NULL},//SSID
	{PL_CP_WPAKEY_FLAG,"WLan_Entry0","WPAPSK",NULL},//WPAKEY
	{PL_CP_SSID2nd_FLAG,"WLan_Entry1","SSID",NULL},//SSID2nd
	{PL_CP_WPAKEY2nd_FLAG,"WLan_Entry1","WPAPSK",NULL},//WPAKEY2nd
#endif
//	{PL_CP_WEBUSRNAME_FLAG,"Account_Entry1","username",NULL},// Attention: entry1 for user account
	{PL_CP_WEBPWD_FLAG,"Account_Entry1","web_passwd",NULL},// Attention: entry1 for user account
	#if !defined(TCSUPPORT_CT_WAN_PTM)
	{PL_CP_XPONSN_FLAG,"GPON_ONU","SerialNumber",NULL},// Attention: entry1 for user account
	#endif
	{PL_CP_XPONPWD_FLAG,"GPON_ONU","Password",NULL},// Attention: entry1 for user account

	{0,NULL,NULL,NULL}
};

int checkParaValid(char *buf, int len)
{
	int i;
	for (i = 0; i < len - 1; i++) {
		if ((unsigned char)buf[i] == 0) /* go to end */
			break;

		if ((unsigned char)buf[i] == 0xff) /* invalid parameter */
			return 0;
	}

	return 1;
}

int parseProlinePara(int flag, proline_Para *pProline, char *value)
{
#if defined(TCSUPPORT_CUC)
	if ((PRODUCTLINEPARAMAGIC != pProline->magic) || !(pProline->flag & flag)){
		return FAIL;
	}
#endif

	switch(flag)
	{
/*
		case PL_CP_PRODUCTCLASS_FLAG:
			snprintf(value, PRDDUCTCLASSLEN, "%s", pProline->productclass);
			break;
		case PL_CP_MANUFACUREROUI_FLAG:
			snprintf(value, MANUFACUREROUILEN, "%s", pProline->manufacturerOUI);
			break;
		case PL_CP_SERIALNUM_FLAG:
			snprintf(value, SERIALNUMLEN, "%s", pProline->serialnum);
			break
*/
#ifdef TCSUPPORT_WLAN
		case PL_CP_SSID_FLAG:
			if ((PRODUCTLINEPARAMAGIC != pProline->magic) || !(pProline->flag & PL_CP_SSID_FLAG) || (checkParaValid(pProline->ssid, SSIDLEN) == 0))
			{
				snprintf(value, SSIDLEN, "%s", DEFAULT_SSID);
			}
			else
			{
				snprintf(value, SSIDLEN, "%s", pProline->ssid);
			}
			break;
		case PL_CP_WPAKEY_FLAG:
			if ((PRODUCTLINEPARAMAGIC != pProline->magic) || !(pProline->flag & PL_CP_WPAKEY_FLAG) || (checkParaValid(pProline->wpakey, WPAKEYLEN) == 0))
			{
				snprintf(value, WPAKEYLEN, "%s", DEFAULT_WPAKEY);
			}
			else
			{
				snprintf(value, WPAKEYLEN, "%s", pProline->wpakey);
			}
			break;
		case PL_CP_SSID2nd_FLAG:
			if ((PRODUCTLINEPARAMAGIC != pProline->magic) || !(pProline->flag & PL_CP_SSID2nd_FLAG) || (checkParaValid(pProline->ssid2nd, SSIDLEN) == 0))
			{
				snprintf(value, SSIDLEN, "%s", DEFAULT_SSID);
			}
			else
			{
				snprintf(value, SSIDLEN, "%s", pProline->ssid2nd);
			}
			break;
		case PL_CP_WPAKEY2nd_FLAG:
			if ((PRODUCTLINEPARAMAGIC != pProline->magic) || !(pProline->flag & PL_CP_WPAKEY2nd_FLAG) || (checkParaValid(pProline->wpakey2nd, WPAKEYLEN) == 0))
			{
				snprintf(value, WPAKEYLEN, "%s", DEFAULT_WPAKEY);
			}
			else
			{
				snprintf(value, WPAKEYLEN, "%s", pProline->wpakey2nd);
			}
			break;
#endif
		case PL_CP_WEBPWD_FLAG:
			if ((PRODUCTLINEPARAMAGIC != pProline->magic) || !(pProline->flag & PL_CP_WEBPWD_FLAG) || (checkParaValid(pProline->webpwd, WEBPWDLEN) == 0))
			{
				snprintf(value, WEBPWDLEN, "%s", DEFAULT_WEBPWD);
			}
			else
			{
				snprintf(value, WEBPWDLEN, "%s", pProline->webpwd);
			}
			break;
#if !defined(TCSUPPORT_CT_WAN_PTM)
		case PL_CP_PPPUSRNAME_FLAG:
			if ((PRODUCTLINEPARAMAGIC != pProline->magic) || !(pProline->flag & PL_CP_PPPUSRNAME_FLAG) || (checkParaValid(pProline->pppusrname, PPPUSRNAMELEN) == 0))
			{
				snprintf(value, PPPUSRNAMELEN, "%s", DEFAULT_PPPUSRNAME);
			}
			else
			{
				snprintf(value, PPPUSRNAMELEN, "%s", pProline->pppusrname);
			}
			break;
		case PL_CP_PPPPWD_FLAG:
			if ((PRODUCTLINEPARAMAGIC != pProline->magic) || !(pProline->flag & PL_CP_PPPPWD_FLAG) || (checkParaValid(pProline->ppppwd, PPPPWDLEN) == 0))
			{
				snprintf(value, PPPPWDLEN, "%s", DEFAULT_PPPPWD);
			}
			else
			{
				snprintf(value, PPPPWDLEN, "%s", pProline->ppppwd);
			}
			break;
		case PL_CP_CFEUSRNAME_FLAG:
			if ((PRODUCTLINEPARAMAGIC != pProline->magic) || !(pProline->flag & PL_CP_CFEUSRNAME_FLAG) || (checkParaValid(pProline->cfeusrname, CFEUSRNAMELEN) == 0))
			{
				snprintf(value, CFEUSRNAMELEN, "%s", DEFAULT_CFEUSRNAME);
			}
			else
			{
				snprintf(value, CFEUSRNAMELEN, "%s", pProline->cfeusrname);
			}
			break;
		case PL_CP_CFEPWD_FLAG:
			if ((PRODUCTLINEPARAMAGIC != pProline->magic) || !(pProline->flag & PL_CP_CFEPWD_FLAG) || (checkParaValid(pProline->cfepwd, CFEPWDLEN) == 0))
			{
				snprintf(value, CFEPWDLEN, "%s", DEFAULT_CFEPWD);
			}
			else
			{
				snprintf(value, CFEPWDLEN, "%s", pProline->cfepwd);
			}
			break;
		case PL_CP_XPONSN_FLAG:
			if ((PRODUCTLINEPARAMAGIC != pProline->magic) || !(pProline->flag & PL_CP_XPONSN_FLAG))
			{
				snprintf(value, XPONSNLEN, "%s", DEFAULT_XPONSN);
			}
			else
			{
				snprintf(value, XPONSNLEN, "%s", pProline->xponsn);
			}
			break;
		case PL_CP_XPONPWD_FLAG:
			if ((PRODUCTLINEPARAMAGIC != pProline->magic) || !(pProline->flag & PL_CP_XPONPWD_FLAG))
			{
				snprintf(value, XPONPWDLEN, "%s", DEFAULT_XPONPWD);
			}
			else
			{
				snprintf(value, XPONPWDLEN, "%s", pProline->xponpwd);
			}
			break;
		case PL_CP_MACADDR_FLAG:
			if ((PRODUCTLINEPARAMAGIC != pProline->magic) || !(pProline->flag & PL_CP_MACADDR_FLAG) || (checkParaValid(pProline->macaddr, MACADDRLEN) == 0))
			{
				snprintf(value, MACADDRLEN, "%s", DEFAULT_MACADDR);
			}
			else
			{
				snprintf(value, MACADDRLEN, "%s", pProline->macaddr);
			}
			break;		
		case PL_CP_XPONMODE_FLAG:
			if ((PRODUCTLINEPARAMAGIC != pProline->magic) || !(pProline->flag & PL_CP_XPONMODE_FLAG) || (checkParaValid(pProline->xponmode, XPONMODELEN) == 0))
			{
				snprintf(value, XPONMODELEN, "%s", DEFAULT_XPONMODE);
			}
			else
			{
				snprintf(value, XPONMODELEN, "%s", pProline->xponmode);
			}
			break;	
#endif
		default:
			tcdbg_printf("\r\nNo such parameter.");
			return FAIL;
			break;
	}

	return SUCCESS;
}
#endif

void handle_proline_paras(mxml_node_t* defaulttree)/*used by unopen code */
{
#if defined(TCSUPPORT_PRODUCTIONLINE) && (defined(TCSUPPORT_RESERVEAREA_EXTEND) || defined(TCSUPPORT_C7) || (defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505)))
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	mxml_node_t *curNode = NULL;
	int i;
	proline_Para para;
	Key_Parameters_Info *pkeyPara;
	char buff[128] = {0};	             // the size should be the largest among all parameters
	char *oriValue = NULL;
		
	memset(&para, 0, sizeof(para));
	tcgetProLinePara(&para,0);
/*
	if(PRODUCTLINEPARAMAGIC != para.magic)
	{
		tcdbg_printf("\r\nGet proline parameter fail.");
		return;
	}
*/
	for(pkeyPara = prolinePara ; 0 != pkeyPara->flag; pkeyPara++)
	{
		memset(nodeName, 0, sizeof(nodeName));
		splitName(pkeyPara->node, nodeName, "_");
	
		curNode = defaulttree;
		for(i = 0; i < MAX_ARG_NUM; i++)
		{
			if(strlen(nodeName[i])==0)
				break;
			curNode = mxmlFindElement(curNode, curNode, nodeName[i],
				NULL, NULL, MXML_DESCEND);
			if(NULL == curNode)
			{
				tcdbg_printf("\r\n%s:%s not found.",pkeyPara->node,pkeyPara->attr);
				break;            // try next one
			}
		}

		if(NULL != curNode)
		{
		
			oriValue = (char *)mxmlElementGetAttr(curNode, pkeyPara->attr);
			if (NULL != oriValue)
			{
				//if(0 == strlen(oriValue))
				{
					if (SUCCESS == parseProlinePara(pkeyPara->flag, &para, buff))
					{
						tcdbg_printf("\r\n %s %d %s:%s.",__FUNCTION__,__LINE__,pkeyPara->attr,buff);
						mxmlElementSetAttr(curNode, pkeyPara->attr, buff);
					}
					else
					{
						tcdbg_printf("\r\nparse proline para %s fail.",pkeyPara->attr);
					}

					
				}
				//else
				//{
				//	tcdbg_printf("\r\nUsr default value %s for %s.",oriValue, pkeyPara->attr);
				//}
			}
			else
			{
				// to do
				// add this config
				tcdbg_printf("\r\nError--Entry %s value is NULL.",pkeyPara->attr);
			}
		}
	}
#endif
	
	return;
}

/*______________________________________________________________________________
**	dataspeedlimit_init
**
**	descriptions:
**		the init function of DataSpeedLimi.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
dataspeedlimit_init(void)
{
	int ret=0;
	cfg_node_t node={
			.name="DataSpeedLimit",
			.type=SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=NULL,
			.cfg_verify=NULL,
#if defined(TCSUPPORT_CT_DS_LIMIT)
			.cfg_execute=dslimit_execute,
			.cfg_boot=dslimit_boot,
#else
			.cfg_execute=NULL,
			.cfg_boot=NULL,
#endif
	};
	ret=register_cfg_node(&node);
	return ret;
}

#if defined(TCSUPPORT_CT_DS_LIMIT)
/* dataspeed nodes list, max support 6 queues */
static struct dsnode g_dataspeed_nodes[DSLIMIT_MAX_NODES] = {0};
/* queue speed list */
static int g_queue_speed[DSLIMIT_MAX_QUEUE_NUM] = {0};

/*
 get queue index via data speed
 return queue index when data speed match
 create new queue when data speed unmatch
*/
int get_queue_index(int dataspeed)
{
	int idx = 0;
	int actspeed = dataspeed * DS_QUEUE_PER_SPEED;

	if ( dataspeed <=0 )
		return -1; /* no limit */

	for ( idx = 0; idx < DSLIMIT_MAX_QUEUE_NUM; idx ++ )
	{
		/* create new speed queue. */
		if ( 0 == g_queue_speed[idx] )
		{
			g_queue_speed[idx] = actspeed;
			return (idx + 1);
		}
#if 0
		/* match exist queue. */
		if ( g_queue_speed[idx] == actspeed )
			return (idx + 1);
#endif
	}

	return 0; /* queue is full. */
}

/*
 change data speed limit mode
 mode: 0: disabled; 1: base interface; 2: base vlan id; 3: base ip range;
*/
int ioctl_dslimit_setmode(int mode, int up_dw)
{
	int fp = -1;
	int ret = -1;
	/* 0: disabled; 1: base interface; 2: base vlan id; 3: base ip range; */
	int dsmode = mode;

	fp = open("/dev/"QOSDSLIMIT_DEV_NAME, O_RDONLY);
	if ( fp < 0)
	{
		tcdbg_printf("\ndbg:[%s] open error [%d]\n", __FUNCTION__, fp);
		return -1;
	}

	/* up stream mode. */
	if ( DSLIMIT_UP == up_dw )
	{
		ret = ioctl(fp, QOSDSLIMIT_IOC_MODECFG_UP, &dsmode);
		if (ret < 0)
		{
			tcdbg_printf("\ndbg:[%s] ioctl error up [%d]\n", __FUNCTION__, ret);
		}
	}
	else if ( DSLIMIT_DW == up_dw )
	{
		ret = ioctl(fp, QOSDSLIMIT_IOC_MODECFG_DW, &dsmode);
		if (ret < 0)
		{
			tcdbg_printf("\ndbg:[%s] ioctl error dw [%d]\n", __FUNCTION__, ret);
		}
	}

	close(fp);
	return 0;
}

/*
 set data speed limit nodes.
 up_dw: 1 for upstream  2 for downstrem
*/
int ioctl_dslimit_setdsnode(int up_dw)
{
	int fp = -1;
	int ret = -1;

	fp = open("/dev/"QOSDSLIMIT_DEV_NAME, O_RDONLY);
	if ( fp < 0 )
	{
		tcdbg_printf("\ndbg:[%s] open error [%d]\n", __FUNCTION__, fp);
		return -1;
	}

	/* up stream rules. */
	if ( DSLIMIT_UP == up_dw )
	{
		ret = ioctl(fp, QOSDSLIMIT_IOC_DATACFG_UP, g_dataspeed_nodes);
		if (ret < 0)
		{
			tcdbg_printf("\ndbg:[%s] ioctl error up[%d]\n", __FUNCTION__, ret);
		}
	}
	/* down stream rules. */
	else if ( DSLIMIT_DW == up_dw )
	{
		ret = ioctl(fp, QOSDSLIMIT_IOC_DATACFG_DW, g_dataspeed_nodes);
		if ( ret < 0 )
		{
			tcdbg_printf("\ndbg:[%s] ioctl error dw[%d]\n", __FUNCTION__, ret);
		}
	}

	close(fp);
	return 0;
}

/*
 set data speed limit queue for downstream.
*/
int ioctl_dslimit_dw_setdsqueue()
{
	int fp = -1;
	int ret = -1;

	fp = open("/dev/"QOSDSLIMIT_DEV_NAME, O_RDONLY);
	if ( fp < 0 )
	{
		tcdbg_printf("\ndbg:[%s] open error [%d]\n", __FUNCTION__, fp);
		return -1;
	}

	/* down stream queue. */
	ret = ioctl(fp, QOSDSLIMIT_IOC_QUEUECFG_DW, g_queue_speed);
	if ( ret < 0 )
	{
		tcdbg_printf("\ndbg:[%s] ioctl error [%d]\n", __FUNCTION__, ret);
	}

	close(fp);
	return 0;
}


/* add dataspeend limit stream configure
pair detail see linux/qos_type.h.
*/
int add_dslimit_data(int mode,
					int pair0,
					int pair1,
					int pair2,
					int pair3,
					int queue_ds,
					int iptype)
{
	int dsmode = mode;
	int idx  = 0, queue_idx = 0;
	int is_dsnode_full = 1;

	if ( queue_ds <= 0 )
		return -1; /* no limit */

	for ( idx = 0; idx < DSLIMIT_MAX_NODES; idx ++ )
	{
		if ( 0 == g_dataspeed_nodes[idx].ds_used )
		{
			switch ( dsmode)
			{
				case DSLIMIT_MODE_IFACE :
				case DSLIMIT_MODE_VLANID :
				case DSLIMIT_MODE_IPRANGE :
					queue_idx = get_queue_index(queue_ds);
					if ( queue_idx <= 0 ) /* no limit or queue full */
						return -1;

					if ( DSLIMIT_MODE_IPRANGE == dsmode )
					{
						if ( iptype <= 0 )
							return -1; /* ip type invalid*/
						g_dataspeed_nodes[idx].ds_iptype = iptype;
					}
					is_dsnode_full = 0;
					g_dataspeed_nodes[idx].ds_queue = queue_idx;
					/* kbps */
					g_dataspeed_nodes[idx].ds_queue_speed
							= queue_ds * DS_QUEUE_PER_SPEED;
					g_dataspeed_nodes[idx].ds_used = 1;
					break;
				default :
					return -1;
			}

			switch ( dsmode)
			{
				case DSLIMIT_MODE_IFACE :
					/*
					1-4: LAN1-LAN4, 5-8: SSID1-SSID4
					*/
					g_dataspeed_nodes[idx].ds_lanif[0] = pair0;
					break;
				case DSLIMIT_MODE_VLANID :
					g_dataspeed_nodes[idx].ds_vlanid[0] = pair0;
					break;
				case DSLIMIT_MODE_IPRANGE :
					g_dataspeed_nodes[idx].ds_ip4addr[0] = pair0;
					g_dataspeed_nodes[idx].ds_ip4addr[1] = pair1;
					g_dataspeed_nodes[idx].ds_ip4addr[2] = pair2;
					g_dataspeed_nodes[idx].ds_ip4addr[3] = pair3;
					break;
				default :
					return -1;
			
			}
			break;
		}
	}

	if ( is_dsnode_full )
		tcdbg_printf("\ndbg: warning data speed limit nodes full!\n");
	return 0;
}

/*
DataSpeedLimit execute function
*/
int dslimit_execute(mxml_node_t *top, char name[][MAX_NODE_NAME])
{
	return dslimit_configure(top);
}
/*
DataSpeedLimit boot function
*/
int dslimit_boot(mxml_node_t *top)
{
	return dslimit_configure(top);
}
/*
DataSpeedLimit configure function
*/
int dslimit_configure(mxml_node_t *top)
{
	char qosNodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char dsNodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char tmpvAttr[24] = {0}, tmpvValue[24] = {0};
	char dsupmode[12] = {0}, dsdwmode[12] = {0};
	int i_dsupmode = 0, i_dsdwmode = 0;
	int idx = 0, queue_idx = 0;
	tcapi_msg_t msg = {0};

	/* local variable init */
	bzero(qosNodeName, sizeof(qosNodeName));
	bzero(dsNodeName, sizeof(dsNodeName));
	strcpy(qosNodeName[0], QOS);
	strcpy(qosNodeName[1], QOS_COMMON);
	strcpy(dsNodeName[0], "DataSpeedLimit");
	strcpy(dsNodeName[1], "Entry");
	bzero(g_queue_speed, sizeof(g_queue_speed));
	bzero(g_dataspeed_nodes, sizeof(g_dataspeed_nodes));

	if ( TCAPI_PROCESS_OK != 
		getAttrValue(top, dsNodeName, "SpeedLimitModeUP", dsupmode) )
		strcpy(dsupmode, "0");
	i_dsupmode = atoi(dsupmode);

	if ( TCAPI_PROCESS_OK != 
		getAttrValue(top, dsNodeName, "SpeedLimitModeDOWN", dsdwmode) )
		strcpy(dsdwmode, "0");
	i_dsdwmode = atoi(dsdwmode);

	/* always close dataspeed limit up stream first. */
	ioctl_dslimit_setmode( DSLIMIT_MODE_DISABLE, DSLIMIT_UP );

	/* Speed limit for up stream. */
	if ( DSLIMIT_MODE_DISABLE != i_dsupmode )
	{
		switch ( i_dsupmode )
		{
			case DSLIMIT_MODE_IFACE :
				if ( SUCCESS != dslimit_interfacemode(top, DSLIMIT_UP) )
				{
					tcdbg_printf("\ndbg: error dslimit_interfacemode"
									" up failed.\n");
					goto DSCFG_ERROR;
				}
				break;
			case DSLIMIT_MODE_VLANID :
				if ( SUCCESS != dslimit_vlanidmode(top, DSLIMIT_UP) )
				{
					tcdbg_printf("\ndbg: error dslimit_vlanidmode"
									" up failed.\n");
					goto DSCFG_ERROR;
				}		
				break;
			case DSLIMIT_MODE_IPRANGE :
				if ( SUCCESS != dslimit_iprangemode(top, DSLIMIT_UP) )
				{
					tcdbg_printf("\ndbg: error dslimit_iprangemode"
								" up failed.\n");
					goto DSCFG_ERROR;
				}
				break;
			default :
				goto DSCFG_ERROR;
		}

		setAttrValue(top, qosNodeName, "Discipline", "CAR");
		setAttrValue(top, qosNodeName, "QoSOptType", "discRule");

		for ( idx = 1; idx <= DSLIMIT_MAX_QUEUE_NUM; idx ++ )
		{
			queue_idx = idx - 1;
			/* queue index */
			bzero(tmpvAttr, sizeof(tmpvAttr));
			bzero(tmpvValue, sizeof(tmpvValue));
			snprintf(tmpvAttr, sizeof(tmpvAttr), "QueueP1%d", idx);
			snprintf(tmpvValue, sizeof(tmpvValue), "%d", idx);
			setAttrValue(top, qosNodeName, tmpvAttr, tmpvValue);

			/* queue bandwidth */
			bzero(tmpvAttr, sizeof(tmpvAttr));
			bzero(tmpvValue, sizeof(tmpvValue));
			snprintf(tmpvAttr, sizeof(tmpvAttr), "QueueBW%d", idx);

			if ( 0 == g_queue_speed[queue_idx] )
				snprintf(tmpvValue, sizeof(tmpvValue), "%d", 0);
			else
				snprintf(tmpvValue, sizeof(tmpvValue),
							"%d", g_queue_speed[queue_idx]);
			setAttrValue(top, qosNodeName, tmpvAttr, tmpvValue);

			/* queue switch */
			bzero(tmpvAttr, sizeof(tmpvAttr));
			bzero(tmpvValue, sizeof(tmpvValue));
			snprintf(tmpvAttr, sizeof(tmpvAttr), "QueueSW%d", idx);
			if ( 0 == g_queue_speed[queue_idx] )
				snprintf(tmpvValue, sizeof(tmpvValue), "%s", "No");
			else
				snprintf(tmpvValue, sizeof(tmpvValue), "%s", "Yes");
			setAttrValue(top, qosNodeName, tmpvAttr, tmpvValue);
		}

		bzero(&msg, sizeof(msg));
		snprintf(msg.node, sizeof(msg.node), "%s_%s", QOS, QOS_COMMON);
		tcapi_commit_req(top, &msg);

		/* active dataspeed limit . */
		ioctl_dslimit_setdsnode( DSLIMIT_UP );
		ioctl_dslimit_setmode( i_dsupmode, DSLIMIT_UP );

	}

	bzero(g_queue_speed, sizeof(g_queue_speed));
	bzero(g_dataspeed_nodes, sizeof(g_dataspeed_nodes));

	/* always close dataspeed limit down stream first. */
	ioctl_dslimit_setmode( DSLIMIT_MODE_DISABLE, DSLIMIT_DW );

	/* Speed limit for down stream. */
	if ( DSLIMIT_MODE_DISABLE != i_dsdwmode )
	{
		switch ( i_dsdwmode )
		{
			case DSLIMIT_MODE_IFACE :
				if ( SUCCESS != dslimit_interfacemode(top, DSLIMIT_DW) )
				{
					tcdbg_printf("\ndbg: error dslimit_interfacemode"
										" dw failed.\n");
					goto DSCFG_ERROR;
				}
				break;
			case DSLIMIT_MODE_VLANID :
				if ( SUCCESS != dslimit_vlanidmode(top, DSLIMIT_DW) )
				{
					tcdbg_printf("\ndbg: error dslimit_vlanidmode"
										" dw failed.\n");
					goto DSCFG_ERROR;
				}		
				break;
			case DSLIMIT_MODE_IPRANGE :
				if ( SUCCESS != dslimit_iprangemode(top, DSLIMIT_DW) )
				{
					tcdbg_printf("\ndbg: error dslimit_iprangemode"
										" dw failed.\n");
					goto DSCFG_ERROR;
				}
				break;
			default :
				goto DSCFG_ERROR;
		}

		/* active dataspeed limit . */
		ioctl_dslimit_setdsnode( DSLIMIT_DW );
		ioctl_dslimit_setmode( i_dsdwmode, DSLIMIT_DW );
		/* update queue. */
		ioctl_dslimit_dw_setdsqueue();
	}

#if defined(TCSUPPORT_CT_HWQOS)
	system("/userfs/bin/hw_nat -!");
#endif

	return SUCCESS;
	
DSCFG_ERROR:
#if defined(TCSUPPORT_CT_HWQOS)
	system("/userfs/bin/hw_nat -!");
#endif
	return FAIL;
}

/*
get LAN index & speed limit value
return code: -1 failed, 1-4: LAN1~LAN4, 5-8: SSID1~SSID4
*/
int8 interfaceLimitVal_get(char *val, int *limit)
{
	char s_iface[20] = {0};
	char s_limitv[20] = {0};
	char s_iface_c[12] = {0};
	int i_limitv_len = 0;
	int idx = 0;

	if ( NULL == val
		|| '\0' == val[0]
		|| NULL == limit )
		return -1;

	if ( NULL == strstr(val, "/") )
	{
		return -1;
	}
	else
	{
		sscanf(val, "%[^/]/%s", s_iface, s_limitv);
	
		i_limitv_len = strlen(s_limitv);
		for ( idx = 0; idx < i_limitv_len; idx ++ )
		{
			if ( !(isdigit(s_limitv[idx])) )
				return -1;
		}
		*limit = atoi(s_limitv);

		for ( idx = 1; idx <= 4; idx ++ )
		{
			snprintf(s_iface_c, sizeof(s_iface_c), "LAN%d", idx);
			if ( 0 == strcmp(s_iface_c, s_iface) )
				return idx;
			snprintf(s_iface_c, sizeof(s_iface_c), "SSID%d", idx);
			if ( 0 == strcmp(s_iface_c, s_iface) )
				return (idx+4);
		}
	}

	return -1;
}

/*
DataSpeedLimit configure interface function
*/
int dslimit_interfacemode(mxml_node_t *top, int up_dw)
{
	char dsNodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	int val_len = 0, lanidx = 0, lanlimit = 0;
	const int limitval_maxlen = 20;
	char iface[512] = {0}, attri[32] = {0};
	char *pos = NULL;

	/* local variable init */
	bzero(dsNodeName, sizeof(dsNodeName));
	strcpy(dsNodeName[0], "DataSpeedLimit");
	strcpy(dsNodeName[1], "Entry");

	if ( DSLIMIT_UP == up_dw )
		strcpy(attri, "InterfaceLimitUP");
	else if ( DSLIMIT_DW == up_dw )
		strcpy(attri, "InterfaceLimitDOWN");
	else
		return -1;

	if ( TCAPI_PROCESS_OK != 
		getAttrValue(top, dsNodeName, attri, iface)
		|| 0 == iface[0] )
		return -1;

	pos = strtok(iface, ",");
	while ( pos )
	{
		val_len = strlen(pos);
		if ( val_len > limitval_maxlen )
		{
			tcdbg_printf("\ndbg: invalid item len, val=[%s]\n", pos);
			return -1;
		}

		lanidx = 0;
		lanlimit = 0;
		if ( -1 == (lanidx = interfaceLimitVal_get(pos, &lanlimit)) )
		{
			tcdbg_printf("\ndbg: invalid item val chk=[%s]\n", pos);
			return -1;
		}

		add_dslimit_data(DSLIMIT_MODE_IFACE, lanidx, 0, 0, 0, lanlimit, 0);

		pos = strtok(NULL, ",");
	}

	return SUCCESS;
}

/*
get VLANID & speed limit value
return code: -1 failed, 1-4094, 4096(untag)
*/
int vlanidLimitVal_get(char *val, int *limit)
{
	char s_vlan[20] = {0};
	char s_limitv[20] = {0};
	int i_vlantag = 0;
	int tag_len = 0;
	int tag_maxlen = 4;
	int i_limitv_len = 0;
	int idx = 0;

	if ( NULL == val
		|| '\0' == val[0])
		return -1;

	if ( NULL == strstr(val, "/") )
	{
		return -1;
	}
	else
	{
		sscanf(val, "%[^/]/%s", s_vlan, s_limitv);

		i_limitv_len = strlen(s_limitv);
		for ( idx = 0; idx < i_limitv_len; idx ++ )
		{
			if ( !(isdigit(s_limitv[idx])) )
				return -1;
		}
		*limit = atoi(s_limitv);

		if ( 0 == strcmp("untagged", s_vlan) )
			return DS_VLANID_UNTAG;

		tag_len = strlen(s_vlan);
		if ( tag_len <= tag_maxlen )
		{
			i_vlantag = atoi(s_vlan);
			if ( i_vlantag >= 1 && i_vlantag <= 4094 )
				return i_vlantag;
		}
	}

	return -1;
}

/*
DataSpeedLimit configure vlanid function
*/
int dslimit_vlanidmode(mxml_node_t *top, int up_dw)
{
	char dsNodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	int idx = 0, val_len = 0, vlan_id = 0, vlanid_limit = 0;
	const int limitval_maxlen = 20;
	char vlanid[512] = {0}, attri[32] = {0};
	char *pos = NULL;

	/* local variable init */
	bzero(dsNodeName, sizeof(dsNodeName));
	strcpy(dsNodeName[0], "DataSpeedLimit");
	strcpy(dsNodeName[1], "Entry");

	if ( DSLIMIT_UP == up_dw )
		strcpy(attri, "VlanTagLimitUP");
	else if ( DSLIMIT_DW == up_dw )
		strcpy(attri, "VlanTagLimitDOWN");
	else
		return -1;

	if ( TCAPI_PROCESS_OK != 
		getAttrValue(top, dsNodeName, attri, vlanid)
		|| 0 == vlanid[0] )
		return -1;

	pos = strtok(vlanid, ",");
	while ( pos )
	{
		val_len = strlen(pos);
		if ( val_len > limitval_maxlen )
		{
			tcdbg_printf("\ndbg: invalid item len, val=[%s]\n", pos);
			return -1;
		}
		vlan_id= 0;
		vlanid_limit = 0;
		if ( (vlan_id = vlanidLimitVal_get(pos, &vlanid_limit)) <= 0 )
		{
			tcdbg_printf("\ndbg: invalid item val chk=[%s]\n", pos);
			return -1;
		}

		add_dslimit_data(DSLIMIT_MODE_VLANID, vlan_id,
							0, 0, 0, vlanid_limit, 0);

		pos = strtok(NULL, ",");
	}

	return SUCCESS;
}

/*
get ipv4 range
return code: -1 failed, 0 succeed
*/
int8 rangeIPV4addr_get(char *v4addr, int *s_v4addr, int *e_v4addr)
{
	char start_addr[80];
	char end_addr[80];
	struct in_addr in_s_v4addr = {0};
	struct in_addr in_e_v4addr = {0};

	if ( NULL == v4addr
		|| '\0' == v4addr[0]
		|| NULL == s_v4addr
		|| NULL == e_v4addr)
		return -1;

	if ( NULL == strstr(v4addr, "-") ) // single ip
	{
		return -1;
	}
	else // ip range
	{
		sscanf(v4addr, "%[^-]-%s", start_addr, end_addr);

		if ( 1 != inet_pton(AF_INET, start_addr, &in_s_v4addr) )
			return -1;

		if ( 1 != inet_pton(AF_INET, end_addr, &in_e_v4addr) )
			return -1;

		if ( in_s_v4addr.s_addr > in_e_v4addr.s_addr )
			return -1;
	}

	memcpy(s_v4addr, &in_s_v4addr, sizeof(in_s_v4addr));
	memcpy(e_v4addr, &in_e_v4addr, sizeof(in_e_v4addr));

	return 0;
}

/*
get ipv6 range
return code: -1 failed, 0 succeed
*/
int8 rangeIPV6addr_get(char *v6addr,
int *pair0, int *pair1, int *pair2, int *pair3)
{
	char start_addr[80];
	char end_addr[80];
	struct in6_addr in_s_v6addr = {0};
	struct in6_addr in_e_v6addr = {0};
	uint8_t *ptr = NULL;

	if ( NULL == v6addr
		|| '\0' == v6addr[0]
		|| NULL == pair0
		|| NULL == pair1
		|| NULL == pair2
		|| NULL == pair3)
		return -1;

	if ( NULL == strstr(v6addr, "-") ) // single ip
	{
		return -1;
	}
	else // ip range
	{
		sscanf(v6addr, "%[^-]-%s", start_addr, end_addr);
		if ( 1 != inet_pton(AF_INET6, start_addr, &in_s_v6addr) )
			return -1;

		if ( 1 != inet_pton(AF_INET6, end_addr, &in_e_v6addr) )
			return -1;

		if ( memcmp(&in_s_v6addr, &in_e_v6addr, sizeof(struct in6_addr)) > 0 )
			return -1;
	}

	ptr = &in_s_v6addr;
	memcpy(pair0, ptr, 4);
	ptr += 4;
	memcpy(pair1, ptr, 4);
	ptr += 4;
	memcpy(pair2, ptr, 4);
	ptr += 4;
	memcpy(pair3, ptr, 4);

	return 0;
}


/*
get ip range & speed limit value
return code: -1 failed
*/
int iprangeLimitVal_get
(char *val, int *pair0, int *pair1, int *pair2, int *pair3, int *limit)
{
	char s_ip[180] = {0};
	char s_limitv[180] = {0};
	int i_vlantag = 0;
	int tag_len = 0;
	int tag_maxlen = 4;
	int i_limitv_len = 0;
	int idx = 0;

	if ( NULL == val
		|| '\0' == val[0])
		return -1;

	if ( NULL == strstr(val, "/") )
	{
		return -1;
	}
	else
	{
		sscanf(val, "%[^/]/%s", s_ip, s_limitv);

		i_limitv_len = strlen(s_limitv);
		for ( idx = 0; idx < i_limitv_len; idx ++ )
		{
			if ( !(isdigit(s_limitv[idx])) )
				return -1;
		}
		*limit = atoi(s_limitv);

		if ( strlen(s_ip) > 79 )
			return -1;

		if ( 0 == rangeIPV4addr_get(s_ip, pair0, pair1) )
			return DS_IPTYPE_IPV4;

		if ( 0 == rangeIPV6addr_get(s_ip, pair0, pair1, pair2, pair3) )
			return DS_IPTYPE_IPV6;
	}

	return -1;
}

/*
DataSpeedLimit configure ip range function
*/
int dslimit_iprangemode(mxml_node_t *top, int up_dw)
{
	char dsNodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	int val_len = 0, iprange_type = 0, iprange_limit = 0;
	int pair0 = 0, pair1 = 0, pair2 = 0, pair3 = 0;
	const int limitval_maxlen = 175;
	char iprangeup[516] = {0}, attri[32] = {0};
	char *pos = NULL;

	/* local variable init */
	bzero(dsNodeName, sizeof(dsNodeName));
	strcpy(dsNodeName[0], "DataSpeedLimit");
	strcpy(dsNodeName[1], "Entry");

	if ( DSLIMIT_UP == up_dw )
		strcpy(attri, "IPLimitUP");
	else if ( DSLIMIT_DW == up_dw )
		strcpy(attri, "IPLimitDOWN");
	else
		return -1;

	if ( TCAPI_PROCESS_OK != 
		getAttrValue(top, dsNodeName, attri, iprangeup)
		|| 0 == iprangeup[0] )
		return -1;

	pos = strtok(iprangeup, ",");
	while ( pos )
	{
		val_len = strlen(pos);
		if ( val_len > limitval_maxlen )
		{
			tcdbg_printf("\ndbg: invalid item len, val=[%s]\n", pos);
			return -1;
		}
		iprange_type = 0;
		iprange_limit = 0;
		pair0 = 0;
		pair1 = 0;
		pair2 = 0;
		pair3 = 0;
		if ( (iprange_type = 
			iprangeLimitVal_get(pos, &pair0, &pair1, &pair2, &pair3
							, &iprange_limit)) <= 0 )
		{
			tcdbg_printf("\ndbg: invalid item val chk=[%s]\n", pos);
			return -1;
		}

		add_dslimit_data(DSLIMIT_MODE_IPRANGE,
					pair0, pair1, pair2, pair3, iprange_limit, iprange_type);

		pos = strtok(NULL, ",");
	}

	return SUCCESS;
}
#endif

#if defined(TCSUPPORT_IPV6)
int upgSysIPVersionHandle()
{
#if 0
	
	int valid_if_num = 0;
	int idx = 0;
	int pvc_index = 0;
	int entry_index = 0;
	int if_index = 0;
	int valid_if[MAX_WAN_IF_INDEX] = {0};
	char serviceList[64] = {0};
	char ip_version[20] = {0};
	char wanMode[20] = {0};
	char linkMode[20] = {0};
	char isp[10] = {0};
#endif
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char ipVersion[10] = {0};
	mxml_node_t *top = NULL;
	tcapi_msg_t msg;
	char cmd_buf[128] = {0};
	
	top = get_node_top();

	if(NULL == top)
	{
		tcdbg_printf("\r\nupgSysIPVersionHandle:Get top node fail.");
		return -1;
	}
	memset(nodeName, 0, sizeof(nodeName));
  	strcpy(nodeName[0],SYS_NODE);
  	strcpy(nodeName[1], SUB_NODE_NAME);
	if(getAttrValue(top, nodeName, SYS_IP_VERSION, ipVersion) == TCAPI_PROCESS_OK)
	{
		if((0 == strcmp(ipVersion, "1")) || (0 == strcmp(ipVersion, "2")))
		{
			setAttrValue(top,nodeName, SYS_IP_VERSION, "3");        // change to v4/v6
		}
	}

	memset(&msg,0,sizeof(msg));
	strcpy(msg.node, SYS_NODE);
	tcapi_commit_req(top, &msg);
	tcapi_save_req(top, &msg);
	tcdbg_printf("\nSys IPVersion is changed to IPv4/IPv6! \n");
	
#if 0

	valid_if_num = get_all_wan_index(valid_if, MAX_WAN_IF_INDEX);
	for( idx = 0; idx < valid_if_num; idx ++)
	{
		if_index = valid_if[idx];
		if ( if_index > MAX_WAN_IF_INDEX )
			return;
		pvc_index = if_index / MAX_SMUX_NUM;
		entry_index = if_index % MAX_SMUX_NUM;

		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0], WAN);
		sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
		sprintf(nodeName[2], "%s%d", WAN_INTERFACE, entry_index);

		if( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, "IPVERSION", ip_version)
			&& TCAPI_PROCESS_OK == getAttrValue(top, nodeName, "LinkMode", linkMode)
			&& TCAPI_PROCESS_OK == getAttrValue(top, nodeName, "WanMode", wanMode)
			&& TCAPI_PROCESS_OK == getAttrValue(top, nodeName, "ISP", isp))
		{
			if((0 == strcmp(wanMode,WAN_ROUTE_MODE)) && (0 == strcmp(linkMode,"linkIP")) && (0 == strcmp(isp,STATIC_MODE)))
			{
				continue;         // we dont handle route & static connection
			}

			setAttrValue(top,nodeName, "IPVERSION", "IPv4/IPv6");
			
			if(( 0 == strcmp(ip_version, "IPv4") ))      // V4-->v4v6
			{
				setAttrValue(top,nodeName, "GW6_Manual", "No");
				
				if(0 == strcmp(wanMode,WAN_ROUTE_MODE))            // route mode
				{
					setAttrValue(top,nodeName, "PDEnable", "Yes");
					setAttrValue(top,nodeName, "DHCPv6PD", "Yes");
					setAttrValue(top,nodeName, "PDOrigin", "PrefixDelegation");
					setAttrValue(top,nodeName, "DHCPv6", "No");
					setAttrValue(top,nodeName, "DsliteMode", "0");
				}
				else                                                // bridge mode
				{
					setAttrValue(top,nodeName, "DHCPv6", "N/A");
					setAttrValue(top,nodeName, "IPADDR6", "");
					setAttrValue(top,nodeName, "GATEWAY6", "");
					setAttrValue(top,nodeName, "PREFIX6", "");
					setAttrValue(top,nodeName, "DNS6", "");
					setAttrValue(top,nodeName, "SecDNS6", "");
				}
			}
			else if(( 0 == strcmp(ip_version, "IPv6") ))            // v6-->v4v6
			{
				if(0 == strcmp(wanMode,WAN_ROUTE_MODE))            // route mode
				{
					setAttrValue(top,nodeName, "NATENABLE", "Enable");
				}
				else                                               // bridge mode
				{
					// do nothing except change IPVERSION to v4v6
				}
			}

		}
	}
#endif

	return 0;
}
#endif

#if defined(TCSUPPORT_C7)
int
settinginfo_init(void){
	int ret=0;
	cfg_node_t node={
			.name="SettingInfo",
			.type=WEB_TYPE|SINGLE_TYPE,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=settinginfo_execute,
			.cfg_boot=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}

int
settinginfo_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	int retval=SUCCESS;
	char cmd[128]={0};
	char tmp[80]={0}, tmp2[10] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	int flag = 0;
	
	memset(cmd, 0, sizeof(cmd));	
	memset(tmp, 0, sizeof(tmp));
	if(getAttrValue(top, name, "Wan2Lan_flag", tmp) == TCAPI_PROCESS_OK)
	{
		if(!strcmp(tmp,"1")){
			sprintf(cmd,"sys wan2lan on 15");
			system(cmd);
			setAttrValue(top, name, "Wan2Lan_flag", "0");
		}else if(!strcmp(tmp,"2")){
			sprintf(cmd,"sys wan2lan off");
			system(cmd);
			setAttrValue(top, name, "Wan2Lan_flag", "0");
		}
	}
	if(getAttrValue(top, name, "CloseReg_flag", tmp) == TCAPI_PROCESS_OK)
	{
		if(!strcmp(tmp,"1")){	
			memset(nodeName,0,sizeof(nodeName));
			strcpy(nodeName[0], "deviceAccount");
			strcpy(nodeName[1], "Entry");
			setAttrValue(top,nodeName,"registerStatus","0");
			setAttrValue(top,nodeName,"registerResult","1");
			setAttrValue(top,nodeName,"registerStatus_ok","0");
			setAttrValue(top,nodeName,"registerResult_ok","1");
			tcapi_commit_internal(top,"deviceAccount");
				
			memset(nodeName,0,sizeof(nodeName));
			strcpy(nodeName[0], "Portal");
			strcpy(nodeName[1], "Entry");			
			setAttrValue(top,nodeName,"Enable","0");
			
			setAttrValue(top, name, "CloseReg_flag", "0");
		}else if(!strcmp(tmp,"2")){	
			memset(nodeName,0,sizeof(nodeName));
			strcpy(nodeName[0], "deviceAccount");
			strcpy(nodeName[1], "Entry");
			setAttrValue(top,nodeName,"registerStatus","99");
			setAttrValue(top,nodeName,"registerResult","99");
			setAttrValue(top,nodeName,"registerStatus_ok","99");
			setAttrValue(top,nodeName,"registerResult_ok","99");
			tcapi_commit_internal(top,"deviceAccount");
				
			memset(nodeName,0,sizeof(nodeName));
			strcpy(nodeName[0], "Portal");
			strcpy(nodeName[1], "Entry");			
			setAttrValue(top,nodeName,"Enable","1");
			
			setAttrValue(top, name, "CloseReg_flag", "0");
		}
	}
	if(getAttrValue(top, name, "Reset_flag", tmp2) == TCAPI_PROCESS_OK)
	{
		if(!strcmp(tmp2,"1")){
			if(getAttrValue(top, name, "MAC", tmp) == TCAPI_PROCESS_OK && strlen(tmp) != 0)
			{				
				sprintf(cmd,"sys mac %s -n",tmp);
				tcdbg_printf("\r\ncmd = %s", cmd);
				system(cmd);
			}

			if(getAttrValue(top, name, "productclass", tmp) == TCAPI_PROCESS_OK && strlen(tmp) != 0)
			{				
				if(flag == 0){
					flag = 1;					
					system("prolinecmd clear 1");			
				}
			
				sprintf(cmd,"/usr/bin/prolinecmd productclass set %s",tmp);
				tcdbg_printf("\r\ncmd = %s", cmd);
				system(cmd);
			}
			if(getAttrValue(top, name, "manufacturerOUI", tmp) == TCAPI_PROCESS_OK && strlen(tmp) != 0)
			{				
				if(flag == 0){
					flag = 1;					
					system("prolinecmd clear 1");			
				}

			
				sprintf(cmd,"/usr/bin/prolinecmd manufacturerOUI set %s",tmp);
				tcdbg_printf("\r\ncmd = %s", cmd);
				system(cmd);
			}
			if(getAttrValue(top, name, "serialnum", tmp) == TCAPI_PROCESS_OK && strlen(tmp) != 0)
			{				
				if(flag == 0){
					flag = 1;					
					system("prolinecmd clear 1");			
				}
			
				sprintf(cmd,"/usr/bin/prolinecmd serialnum set %s",tmp);
				tcdbg_printf("\r\ncmd = %s", cmd);
				system(cmd);
			}
			if(getAttrValue(top, name, "SSID1", tmp) == TCAPI_PROCESS_OK && strlen(tmp) != 0)
			{				
				if(flag == 0){
					flag = 1;					
					system("prolinecmd clear 1");			
				}

			
				sprintf(cmd,"/usr/bin/prolinecmd ssid set %s",tmp);
				tcdbg_printf("\r\ncmd = %s", cmd);
				system(cmd);
			}
			if(getAttrValue(top, name, "wpakey1", tmp) == TCAPI_PROCESS_OK && strlen(tmp) != 0)
			{				
				if(flag == 0){
					flag = 1;					
					system("prolinecmd clear 1");			
				}

			
				sprintf(cmd,"/usr/bin/prolinecmd wpakey set %s",tmp);
				tcdbg_printf("\r\ncmd = %s", cmd);
				system(cmd);
			}
			if(getAttrValue(top, name, "SSID2", tmp) == TCAPI_PROCESS_OK && strlen(tmp) != 0)
			{				
				if(flag == 0){
					flag = 1;					
					system("prolinecmd clear 1");			
				}
			
				sprintf(cmd,"/usr/bin/prolinecmd ssid2nd set %s",tmp);
				tcdbg_printf("\r\ncmd = %s", cmd);
				system(cmd);
			}
			if(getAttrValue(top, name, "wpakey2", tmp) == TCAPI_PROCESS_OK && strlen(tmp) != 0)
			{				
				if(flag == 0){
					flag = 1;					
					system("prolinecmd clear 1");			
				}
			
				sprintf(cmd,"/usr/bin/prolinecmd wpakey2nd set %s",tmp);
				tcdbg_printf("\r\ncmd = %s", cmd);
				system(cmd);
			}
			
			if(getAttrValue(top, name, "webpwd", tmp) == TCAPI_PROCESS_OK && strlen(tmp) != 0)
			{				
				if(flag == 0){
					flag = 1;					
					system("prolinecmd clear 1");			
				}
			
				sprintf(cmd,"/usr/bin/prolinecmd webpwd set %s",tmp);
				tcdbg_printf("\r\ncmd = %s", cmd);
				system(cmd);
			}
			
			system("prolinecmd restore default");			
			
			//setAttrValue(top, name, "Reset_flag", "0");
		}
	}

	return retval;
}
#endif

#if defined(TCSUPPORT_CT_JOYME)
void  GetMacAddr(unsigned char *macstr, unsigned char *macnum)
{
	int temp[6];
    int i;
        
	sscanf(macstr, "%02x:%02x:%02x:%02x:%02x:%02x", &temp[0],&temp[1],&temp[2],&temp[3],&temp[4],&temp[5]);
	for(i=0;i<6;i++){
        macnum[i] = (unsigned char)temp[i];
	}

}
#define MAX_DEVNUM_RULE 64

char *
laninfo_adv_table[MAX_NODE_NAME]=
{
	"Common", "",
};
char 
laninfo_attr[][20]=
{
	{"ipflag"},
	{"portflag"},
	{"devtypeflag"},
	{"conntypeflag"},
	{""},
};
int
laninfo_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="LanInfo",
            .type=ADV_FLAG|ENTRY_TYPE|MAX_DEVNUM_RULE|UPDATE_TYPE,
			.adv_table=laninfo_adv_table,
			.merge_table=NULL,
			.cfg_read=laninfo_read,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=NULL,
			.cfg_boot=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}	

int
laninfo_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr)
{	
	char NodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	mxml_node_t *group = NULL;
	mxml_node_t *node = NULL;
	int i=0,ret,devnum = 0,j = 0,port,k=0;
	char cmd[64]={0};
	char ip[40]={0},lladdr[15]={0},mac[20]={0},stale[15]={0};
	char buftemp[32] = {0};
	char buf[MAX_BUF_SIZE]={0};
	int isintable = 0,entry_index[64] = {0};
	char *pBuf[MAX_DEVNUM_RULE];
	char *pBuf1[MAX_DEVNUM_RULE];

#if defined(TCSUPPORT_CT_JOYME)
	bzero(NodeName, sizeof(NodeName));
	strcpy(NodeName[0], "LanInfo");
	strcpy(NodeName[1], "Common");
	/* for read attributes fastly. */
	if ( TCAPI_PROCESS_OK == getAttrValue(top,NodeName, "NoRead", buftemp)
 		&&  0 == strcmp(buftemp, "Yes") )
	{
		return SUCCESS;
	}
	bzero(NodeName, sizeof(NodeName));
	bzero(buftemp, sizeof(buftemp));
#endif

	for(i=0; i<MAX_DEVNUM_RULE; i++){
		pBuf[i] = NULL;
		pBuf1[i] = NULL;
		pBuf[i] = (char *)malloc(20);
		pBuf1[i] = (char *)malloc(20);
		if(NULL == pBuf[i] || NULL == pBuf1[i] )
			goto FreeBUF;
		memset(pBuf[i], 0, 20);
		memset(pBuf1[i], 0, 20);
	}
	memset(NodeName, 0, sizeof(NodeName));
	strcpy(NodeName[0],"LanInfo");
	sprintf(cmd,"/usr/bin/ip neigh show dev br0 > %s", IFINFO_ETHCMD_PATH);
	system(cmd);

	FILE *fp = NULL;
	fp=fopen(IFINFO_ETHCMD_PATH, "r");
	if(fp == NULL)
		goto FreeBUF;;

	while (fgets(buf,MAX_BUF_SIZE,fp))
	{
		ret = sscanf(buf, "%s %s %s %s",ip,lladdr,mac,stale);
#if defined(TCSUPPORT_CT_JOYME)
		if(get_devinfo(mac,buftemp,1) != 0) {
			continue;
		}
#endif
		if((ret == 4) && (inet_addr(ip) != INADDR_NONE))
		{
			
			sprintf(pBuf[devnum],"%s",mac);
			isintable = 0;
			k = 0;
			for(i=0;i<MAX_DEVNUM_RULE;i++)
			{
				sprintf(NodeName[1],"%s%d",SUB_NODE_NAME, i);
				memset(buftemp, 0, sizeof(buftemp));
				if(getAttrValue(top,NodeName,"Active",buftemp) == TCAPI_PROCESS_OK &&
					!strcmp(buftemp,"1"))
				{
					memset(buftemp, 0, sizeof(buftemp));
					if(getAttrValue(top,NodeName,"MacAddr",buftemp) == TCAPI_PROCESS_OK  &&
						!strcmp(buftemp,mac))
					{
						
						for(j=0;strlen(laninfo_attr[j])!=0;j++)
						{
							if(getAttrValue(top,NodeName,laninfo_attr[j],buftemp) != TCAPI_PROCESS_OK)
							{
								setAttrValue(top,NodeName,laninfo_attr[j],"0");
							}
						}
						memset(buftemp, 0, sizeof(buftemp));
						sprintf(buftemp, "%s", ip);
						setAttrValue(top,NodeName,"IpAddr",buftemp);

						memset(buftemp, 0, sizeof(buftemp));
						if(get_devinfo(mac,buftemp,1) == 0)
							setAttrValue(top,NodeName,"Port",buftemp);
						else
							setAttrValue(top,NodeName,"Port","0");
						isintable = 1;
						break;
					}
				}
				else{
					isintable = 0;
					entry_index[k] = i; 
					k++;
				}
			}
			if(isintable == 0)
			{
				sprintf(NodeName[1],"%s%d",SUB_NODE_NAME, entry_index[0]);				
				setAttrValue(top,NodeName,"Active","1");
				for(j=0;strlen(laninfo_attr[j])!=0;j++)
				{
					if(getAttrValue(top,NodeName,laninfo_attr[j],buftemp) != TCAPI_PROCESS_OK)
					{
						setAttrValue(top,NodeName,laninfo_attr[j],"0");
					}
				}
				memset(buftemp, 0, sizeof(buftemp));
				sprintf(buftemp, "%s", ip);
				setAttrValue(top,NodeName,"IpAddr",buftemp);
					
				memset(buftemp, 0, sizeof(buftemp));
				sprintf(buftemp, "%s", mac);
				setAttrValue(top,NodeName,"MacAddr",buftemp);

				memset(buftemp, 0, sizeof(buftemp));
				if(get_devinfo(mac,buftemp,1) == 0)
					setAttrValue(top,NodeName,"Port",buftemp);
				else
					setAttrValue(top,NodeName,"Port","0");
			}
			devnum++;
		}
	}

	for(i=0;i<MAX_DEVNUM_RULE;i++)
	{
		sprintf(NodeName[1],"%s%d",SUB_NODE_NAME, i);
		memset(buftemp, 0, sizeof(buftemp));
		if(getAttrValue(top,NodeName,"MacAddr",buftemp) == TCAPI_PROCESS_OK)
			sprintf(pBuf1[i],"%s",buftemp);
		else
			strcpy(pBuf1[i],"");
		isintable = 0;
		for(j=0;j<devnum;j++){
			if(!strcmp(pBuf1[i],pBuf[j]))
				isintable =1;
				
		}
		if(isintable == 0){
			group = mxmlFindElement(top, top, NodeName[0],
				NULL, NULL, MXML_DESCEND); 
			node = mxmlFindElement(group, group, NodeName[1],
				NULL, NULL, MXML_DESCEND);
			if(node)
				mxmlDelete(node);
		}

	}
	
	sprintf(buftemp,"%d",devnum);
	strcpy(NodeName[1],"Common");
	setAttrValue(top,NodeName,"Number",buftemp);
	
FreeBUF:
	for(i=0; i<MAX_DEVNUM_RULE; i++){
		if(NULL != pBuf[i]){
			free(pBuf[i]);
			pBuf[i] = NULL;
		}
		if(NULL != pBuf1[i]){
			free(pBuf1[i]);
			pBuf1[i] = NULL;
		}
	}
	fclose(fp);
	return SUCCESS;
}

int get_devinfo(char * macstr ,char *buf,int flag)
{
	FILE *fp = NULL;
	unsigned char macnum[6] = {0};
	char tempbuf[512] = {0},port[4] = {0};
	char mac[17] = {0}, ip[16] = {0},expire[10] = {0}, hostname[32] = {0}; 
	
#if defined(TCSUPPORT_CT_JOYME)
	int lanStatus[4] = {0}, find = 0;
#endif
	
	if(macstr == NULL || buf == NULL)
		return -1;
	if(flag == 0){
		char *path = "etc/udhcp_lease";
		fp = fopen(path,"r");
		if(fp == NULL)
			return -1;

		while(feof(fp) == 0 ){
			fgets(tempbuf,sizeof(tempbuf),fp);
			sscanf(tempbuf, "%s %s %s %s",mac, ip, expire, hostname);
			GetMacAddr(mac,macnum);
			sprintf(mac,"%02x:%02x:%02x:%02x:%02x:%02x",macnum[0],
				macnum[1],macnum[2],macnum[3],macnum[4],macnum[5]);
			if(strcmp(macstr,mac) == 0)
				strcpy(buf,hostname);
			
		}
		fclose(fp);
	}
	else if(flag == 1){
		char *path = "proc/br_fdb_host/stb_list";
		fp = fopen(path,"r");
		if(fp == NULL)
			return -1;
		while(feof(fp) == 0 ){
			fgets(tempbuf,sizeof(tempbuf),fp);
			sscanf(tempbuf, "%[^=]",port);
			sscanf(tempbuf, "%*[^=]=%s",mac);
			if(strcmp(macstr,mac) == 0) {
				strcpy(buf,port);
#if defined(TCSUPPORT_CT_JOYME)
				if (atoi(port) >= 1 && atoi(port) <= 4) {
					getPonLanStatus2(lanStatus);
					if (lanStatus[atoi(port)-1] == 0) { 
						/* this lan is disconnect */
						tcdbg_printf("\r\nget_devinfo():this lan is disconnect, port = %d", atoi(port));
						return 2;
		                         }
                                 }
				find = 1;
				break;
#endif
                         }
		}
		fclose(fp);
	}
#if defined(TCSUPPORT_CT_JOYME)
	if (find == 0)
		return 2;
#endif
	return 0;
}

/*
	restore osgi right to factory default.
*/
int restoreOSGIRight(int mode)
{
	switch ( mode )
	{
		case BOOT2DEFSETTR69: /* factory default with tr069 */
#if defined(TCSUPPORT_CT_LONG_RESETBTN)
		case LONGRESET_BOOT2DEFSET: /* factory default with long reset button */
#endif
			restoreApiRight1(mode);
			restorePluginBC(mode);
			return 0;
		default:
			break;
	}

	return 0;
}

/*
	restore API right1 to factory default.
*/
int restoreApiRight1(int mode)
{
#define MOBILE_RIGHT1PATH "/usr/osgi"
#define MOBILE_RIGHT1_DEF_PATH "/usr/osgi/def_api_right1"

	/* remove current api right1 */
	system("rm -f "MOBILE_RIGHT1PATH"/*_right1");

	/* restore api right1 */
	system("cp -f "MOBILE_RIGHT1_DEF_PATH"/* "MOBILE_RIGHT1PATH"/");

	return 0;
}

int pluginc_stop_it(char *pid_path)
{
	char pidval[40] = {0};
	int pid_i = 0;
	FILE *fp = NULL;

	if ( !pid_path )
		return -1;

	fp = fopen(pid_path,"r");
	if ( fp )
	{		
		fgets(pidval, sizeof(pidval), fp);
		fclose(fp);
	}
	else
		return -2;

	pid_i = atoi(pidval);
	if ( pid_i )
		kill(pid_i, SIGTERM);

	return 0;
}

extern int get_file_string(char *file_path,
char line_return[][MAX_INFO_LENGTH], int line_num);
/* restore plugin b/c to factory default. */
int restorePluginBC(int mode)
{
#define RESTORE_C_PIDS			"/usr/osgi/plugin-c/%s/pid"  /* pid for plugin c */
#define RESTORE_C_PATHS			"/usr/osgi/plugin-c" 		/* path for plugin c */
#define RESTORE_B_PATHS			"/usr/osgi/plugin-b" 		/* path for plugin b */
#define RESTORE_DEF_PATHS		"/usr/osgi/factory" 		/* path for factory */
#define RESTORE_COPY_FILE		"cp -f "RESTORE_DEF_PATHS"/%s /tmp/"
#define RESTORE_INSTALL_FILE	"/usr/osgi/plugin_install.sh %d %s %s %s"
#define BUNDLE_TYPE 0
#define PLUGIN_C_TYPE 5


	/* restore plugin C: 1->kill all plugins */
	DIR *dir;
	FILE *fp = NULL;
	struct dirent *dirptr;
	unsigned int num = 0; 
	int ret = 0, sizeLen = 0;
	char buffer[256] = {0}, appfolder[128] = {0};
	char version[64] = {0};

	dir = opendir(RESTORE_C_PATHS);
	if (NULL != dir)
	{
		while ((dirptr = readdir(dir)) != NULL)
		{
			if ((strcmp(dirptr->d_name, ".") == 0)
				|| (strcmp(dirptr->d_name, "..") == 0))
				continue;
			if (DT_DIR != dirptr->d_type) 
				continue;
			snprintf(buffer, sizeof(buffer) - 1, RESTORE_C_PIDS, dirptr->d_name);	
			pluginc_stop_it(buffer);
		}
		closedir(dir);
	}

	/* restore plugin B/C: 2->restore factory */
	system("rm -rf "RESTORE_C_PATHS"/*");
	system("rm -rf "RESTORE_B_PATHS"/*");

	dir = opendir(RESTORE_DEF_PATHS);
	if ( NULL != dir )
	{
		while ( (dirptr = readdir(dir)) != NULL )
		{
			if ((strcmp(dirptr->d_name, ".") == 0)
				|| (strcmp(dirptr->d_name, "..") == 0))
				continue;
			if ( DT_DIR == dirptr->d_type )
				continue;

			sizeLen = strlen(dirptr->d_name);
			if ( sizeLen < 5 ) /* *.jar, *.cpk */
				continue;

			bzero(appfolder, sizeof(appfolder));
			strncpy(appfolder, dirptr->d_name, (sizeLen - 4));

			if ( 0 == strcmp(dirptr->d_name + (sizeLen - 4), ".cpk") )
			{
				/* create cpk folder */
				snprintf(buffer, sizeof(buffer) - 1, "mkdir %s/%s",
							RESTORE_C_PATHS, appfolder);
				system(buffer);

				/* copy to tmp install folder */
				snprintf(buffer, sizeof(buffer) - 1, RESTORE_COPY_FILE,
					dirptr->d_name);
				system(buffer);

				/* install it */
				snprintf(buffer, sizeof(buffer) - 1,
					RESTORE_INSTALL_FILE,
					PLUGIN_C_TYPE,  appfolder,  "1.0.0", "Yes");
				system(buffer);
			}
			else if ( 0 == strcmp(dirptr->d_name + (sizeLen - 4), ".jar") )
			{
				/* create cpk folder */
				snprintf(buffer, sizeof(buffer) - 1, "mkdir %s/%s",
							RESTORE_B_PATHS, appfolder);
				system(buffer);

				/* copy to tmp install folder */
				snprintf(buffer, sizeof(buffer) - 1, RESTORE_COPY_FILE,
					dirptr->d_name);
				system(buffer);

				/* get version */
				snprintf(buffer, sizeof(buffer) - 1,
							RESTORE_DEF_PATHS"/%s.info", appfolder);
				snprintf(version, sizeof(version) - 1,
							"%s", "1.0.0");
				bzero(version, sizeof(version));
				get_profile_str_new( "version=", version, sizeof(version)
					, buffer);

				/* install it */
				snprintf(buffer, sizeof(buffer) - 1,
					RESTORE_INSTALL_FILE,
					BUNDLE_TYPE,  appfolder,  version, "Yes");
				system(buffer);
			}
		}
		closedir(dir);
	}


	return 0;
}


#endif
