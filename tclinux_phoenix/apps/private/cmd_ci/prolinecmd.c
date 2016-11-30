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
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include "global.h"
#include "prolinecmd.h"
#include "../../../version/tcversion.h"

#ifdef TCSUPPORT_PRODUCTIONLINE
static int doProductClass(int argc, char *argv[], void *p);
static int doManufacturerOUI(int argc, char *argv[], void *p);
static int doSerialnum(int argc, char *argv[], void *p);
static int doClear(int argc, char *argv[], void *p);
static int doProLineVer(int argc, char *argv[], void *p);
static int doProLineHelp(int argc, char *argv[], void *p);
#if defined(TCSUPPORT_FON) || defined(TCSUPPORT_FON_V2)
static int doFonKeyword(int argc, char *argv[], void *p);
static int doFonMac(int argc, char *argv[], void *p);
#endif

#if defined(TCSUPPORT_RESERVEAREA_EXTEND) || defined(TCSUPPORT_C7) || (defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505))
static int doSsid(int argc, char *argv[], void *p);
static int doSsid2nd(int argc, char *argv[], void *p);
static int doWpaKey(int argc, char *argv[], void *p);
static int doWpaKey2nd(int argc, char *argv[], void *p);
static int doHwVersion(int argc, char *argv[], void *p);
#if defined(TCSUPPORT_CT)
#ifdef TCSUPPORT_MT7570
static int doMt7570Bob(int argc, char *argv[], void *p);
#endif
#endif
#if !defined(TCSUPPORT_C7)
#if !(defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505))
static int doPppUsrname(int argc, char *argv[], void *p);
static int doPppPwd(int argc, char *argv[], void *p);
static int doCfeUsrname(int argc, char *argv[], void *p);
static int doCfePwd(int argc, char *argv[], void *p);
static int doXponSn(int argc, char *argv[], void *p);
static int doXponPwd(int argc, char *argv[], void *p);
static int doMacAddr(int argc, char *argv[], void *p);
static int doXponMode(int argc, char *argv[], void *p);
static int doRomfileselect(int argc, char *argv[], void *p);
#endif
#endif
#endif
#if !defined(TCSUPPORT_CY)
static int doWebAccount(int argc, char *argv[], void *p);
static int doWebPwd(int argc, char *argv[], void *p);
static int doRestore(int argc, char *argv[], void *p);
#endif
#if defined(TCSUPPORT_WAN_GPON) || defined(TCSUPPORT_WAN_EPON)
#ifndef TCSUPPORT_RESERVEAREA_EXTEND
static int doPonMacNum(int argc, char *argv[], void *p);
static int doPonUserAdminPasswd(int argc, char *argv[], void *p);
#endif
#if defined(TCSUPPORT_WAN_GPON)
#ifndef TCSUPPORT_RESERVEAREA_EXTEND
static int doGponSN(int argc, char *argv[], void *p);
static int doGponPasswd(int argc, char *argv[], void *p);
#endif
#endif
#endif


#if defined(TCSUPPORT_RESERVEAREA_EXTEND) || defined(TCSUPPORT_CY)
static int doproductBarCode(int argc,char *argv[], void *p);
#endif

static cmds_t ProLineCmds[] = {
	{"productclass", doProductClass, 0x10,   0,  NULL},/*set or display productclass*/	
	{"manufacturerOUI", doManufacturerOUI, 0x10,   0,  NULL},/*set or display manufacturerOUI*/	
	{"serialnum", doSerialnum, 0x10,   0,  NULL},/*set or display serialnum*/
#if defined(TCSUPPORT_FON) || defined(TCSUPPORT_FON_V2)
	{"fonKeyword", doFonKeyword, 0x10,   0,  NULL},/*set or display FON keyword*/
	{"fonMac", doFonMac, 0x10,   0,  NULL},/*set or display FON mac*/
#endif
#if defined(TCSUPPORT_WAN_GPON) || defined(TCSUPPORT_WAN_EPON)
#ifndef TCSUPPORT_RESERVEAREA_EXTEND
	{"ponmacnum",doPonMacNum,0x10,   0,  NULL},
	{"ponuseradminpasswd",doPonUserAdminPasswd,0x10,   0,  NULL},
#endif
#if defined(TCSUPPORT_WAN_GPON)
#ifndef TCSUPPORT_RESERVEAREA_EXTEND
	{"gponsn",doGponSN,0x10,   0,  NULL},
	{"gponpasswd",doGponPasswd,0x10,   0,  NULL},
#endif
#endif
#endif
#if defined(TCSUPPORT_RESERVEAREA_EXTEND) || defined(TCSUPPORT_CY)
	{"barCode", doproductBarCode, 0x10, 0, NULL},
#endif
#if defined(TCSUPPORT_RESERVEAREA_EXTEND) || defined(TCSUPPORT_C7) || (defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505))
#if defined(TCSUPPORT_CT)
#ifdef TCSUPPORT_MT7570
	{"mt7570bob", doMt7570Bob, 0x10,   0,  NULL},
#endif
#endif
	{"ssid", doSsid, 0x10,   0,  NULL},/*set or display SSID*/
	{"ssid2nd", doSsid2nd, 0x10,   0,  NULL},/*set or display SSID*/
	{"wpakey", doWpaKey, 0x10,   0,  NULL},/*set or display wpakey*/
	{"wpakey2nd", doWpaKey2nd, 0x10,   0,  NULL},/*set or display wpakey*/
	{"hwver", doHwVersion, 0x10,   0,  NULL},/*set or display hwVersion */
#if !defined(TCSUPPORT_C7) && !(defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505))
	{"pppusrname", doPppUsrname, 0x10,   0,  NULL},/*set or display ppp username*/
	{"ppppwd", doPppPwd, 0x10,   0,  NULL},/*set or display ppp password*/
	{"cfeusrname", doCfeUsrname, 0x10,   0,  NULL},/*set or display bootloader username*/
	{"cfepwd", doCfePwd, 0x10,   0,  NULL},/*set or display bootloader password*/
	{"xponsn", doXponSn, 0x10,   0,  NULL},/*set or display XPON serialnumber*/
	{"xponpwd", doXponPwd, 0x10,   0,  NULL},/*set or display XPON password*/
	{"macaddr", doMacAddr, 0x10,   0,  NULL},/*set or display bootloader password*/
	{"xponmode", doXponMode, 0x10, 0, NULL},/*set or display xpon mode*/
	{"romfileselect", doRomfileselect, 0x10, 0, NULL},/*set or display selected romfile*/
#endif
#endif
	{"clear", doClear, 0x10,   0,  NULL},/*clear para*/		
	{"version", doProLineVer, 0x10,   0,  NULL},/*show proline cmd version*/	
	{"help", doProLineHelp, 0x10,   0,  NULL},/*show proline cmd help*/	
#if !defined(TCSUPPORT_CY)
	{"webpwd", doWebPwd, 0x10,   0,  NULL},/*set or display web password*/
	{"webAccount", doWebAccount, 0x10,   0,  NULL},/*set or display web Account*/
    {"restore", doRestore, 0x10,   0,  NULL},/*set or display bootloader password*/
#endif
	{NULL,          NULL,               0x10,   0,  NULL},
};

/*------------------------/
/        common function  /                                     
/------------------------*/
int subcmd  (
    const cmds_t tab[], int argc, char *argv[], void *p
)
{
    register const cmds_t *cmdp;
    int found = 0;
    int i;

    /* Strip off first token and pass rest of line to subcommand */
    if (argc < 2) {
        if (argc < 1)
            printf("SUBCMD - Don't know what to do?\n");
    	else{
            argv++;
            goto print_out_cmds;
    	}
    	return -1;
    }
	argc--;
	argv++;
	for(cmdp = tab;cmdp->name != NULL;cmdp++){
	    if(strncmp(argv[0],cmdp->name,strlen(argv[0])) == 0){
	       found = 1;
	       break;
		}
	}
	if(!found){
        char buf[66];

print_out_cmds:
        printf("valid subcommands of %s:\n", (argv-1)[0]);
        memset(buf,' ',sizeof(buf));
        buf[64] = '\n';
        buf[65] = '\0';
        for(i=0,cmdp = tab;cmdp->name != NULL;cmdp++){
            strncpy(&buf[i*16],cmdp->name,strlen(cmdp->name));
            if(i == 3){
		       printf(buf);
		       memset(buf,' ',sizeof(buf));
		       buf[64] = '\n';
		       buf[65] = '\0';
		    }
		    i = (i+1)%4;
		}
		if(i != 0)
		   printf(buf);
		return -1;
	}
	if(argc <= cmdp->argcmin){
	    if(cmdp->argc_errmsg != NULL)
	       printf("Usage: %s\n",cmdp->argc_errmsg);
	    return -1;
	}
	return (*cmdp->func)(argc,argv,p);
}

static void showInfo(int argc, char *argv[], void *p)
{
#if 0
	int i;
	printf("\r\nthe argc is %d",argc);
	for(i = 0; i < argc; i++)
	{
		printf("\r\nthe argv[%d] is %s",i,argv[i]);
	}
#endif
}

//int getProLineCwmpPara(proline_cwmpPara*buf)
static int getProLinePara(proLineFlag *plpara,proline_Para*buf)
{
	char cmdbuf[200] = {0};
	FILE *fp = NULL;
	sprintf(cmdbuf,TC_FLASH_READ_CMD,plpara->para.readfile,plpara->para.flashsize,plpara->para.flashoffset,plpara->para.zonename);
	system(cmdbuf);
			
	//open file
	fp = fopen(plpara->para.readfile, "r");
	if (fp == NULL) 
	{
		printf("\r\ngetProLineCwmpPara:open file(%s) fail!",plpara->para.readfile);
		return FAIL;
	}
	
	switch(plpara->type)
	{
		case PL_CP:
			//read cwmp para 
			fread((proline_Para*)buf, sizeof(proline_Para), 1, fp);
			break;
			
		default:
			printf("\r\n[getProLinePara]Not support this para!!");
			break;
	}

	//close file and unlink it
	fclose(fp);
	unlink(plpara->para.readfile);
	return SUCCESS;	
}
#if defined(TCSUPPORT_CT)
#ifdef TCSUPPORT_MT7570
#define MT7570BOB_CONF  "/tmp/7570_bob.conf"
#endif
#endif
static int clearProLinePara(int flag)
{	
	FILE *fp = NULL;
	char cmdbuf[200] = {0};
	switch(flag)
	{
		case 1:
		{
#if defined(TCSUPPORT_RESERVEAREA_EXTEND) || defined(TCSUPPORT_C7) || (defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505))
			proline_Para temp;
#else
			proline_cwmpPara temp;	
#endif
			memset(&temp,0,sizeof(temp));
#if defined(TCSUPPORT_CT)
#ifdef TCSUPPORT_MT7570		
			fp = fopen(MT7570BOB_CONF, "r");
			if(fp != NULL){
				if(MT7570BOBLEN != fread(temp.mt7570bob, MT7570BOBLEN, 1, fp)){
					printf("\r\nclearProLinePara:read file(mt7570_bob.conf) fail!");		
				}
				fclose(fp);
			}
			else{
				printf("\r\nclearProLinePara:open file(mt7570_bob.conf) fail!");		
			}
#endif
#endif
			//open file and read para to struct
			fp = fopen(PROLINECMD_CWMPPARA_FILE, "w+");
			if (fp == NULL) 
			{
				printf("\r\nsetProLineCwmpPara:open file(%s) fail!",PROLINECMD_CWMPPARA_FILE);
				return FAIL;
			}
#if defined(TCSUPPORT_RESERVEAREA_EXTEND) || defined(TCSUPPORT_C7) || (defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505))
			fwrite(&temp, sizeof(proline_Para), 1, fp);
#else
			fwrite(&temp, sizeof(proline_cwmpPara), 1, fp);
#endif
			fclose(fp);
			sprintf(cmdbuf,TC_FLASH_WRITE_CMD,PROLINECMD_CWMPPARA_FILE,getProlineCwmparaRaSize(),getProlineCwmparaRaOffset(),RESERVEAREA_NAME);
			system(cmdbuf);
			unlink(PROLINECMD_CWMPPARA_FILE);
			break;
		}
		default:
			printf("\r\nclearProLinePara:not support this type!");
			break;
	}
	
	return SUCCESS;
}

//int setProLineCwmpPara(void*buf,int len,int flag)
static int setProLinePara(proLineFlag *plpara,void*buf,int len)
{
	char cmdbuf[200] = {0};
	proline_Para temp;
	FILE *fpread = NULL;
	FILE *fpwrite = NULL;
	
	memset(&temp,0,sizeof(temp));
	//read para from reservearea to file first
	sprintf(cmdbuf,TC_FLASH_READ_CMD,plpara->para.readfile,plpara->para.flashsize,plpara->para.flashoffset,plpara->para.zonename);
	system(cmdbuf);
	
	//open file and read para to struct
	fpread = fopen(plpara->para.readfile, "r");
	if (fpread == NULL) 
	{
		printf("\r\nsetProLineCwmpPara:open file(%s) fail!",plpara->para.readfile);
		return FAIL;
	}

	//open file and read para to struct
	fpwrite = fopen(plpara->para.writefile, "w+");
	if (fpwrite == NULL) 
	{
		printf("\r\nsetProLineCwmpPara:open file(%s) fail!",plpara->para.writefile);
		fclose(fpread);
		return FAIL;
	}
	
	//fprintf(stderr,"plpara->type is %d",plpara->type);
	//fprintf(stderr,"plpara->flag is %d",plpara->flag);
	switch(plpara->type)
	{
		case PL_CP:
			//read all cwmpara and replace any other para according to flag
			fread(&temp, sizeof(proline_Para), 1, fpread);
			switch(plpara->flag)
			{
				case PL_CP_PRODUCTCLASS:
					memset(temp.productclass,0,sizeof(temp.productclass));
					strncpy(temp.productclass,(char*)buf,len);
					temp.flag |= PL_CP_PRODUCTCLASS_FLAG;
					break;
					
				case PL_CP_MANUFACUREROUI:
					memset(temp.manufacturerOUI,0,sizeof(temp.manufacturerOUI));
					strncpy(temp.manufacturerOUI,(char*)buf,len);
					temp.flag |= PL_CP_MANUFACUREROUI_FLAG;
					break;
					
				case PL_CP_SERIALNUM:
					memset(temp.serialnum,0,sizeof(temp.serialnum));
					strncpy(temp.serialnum,(char*)buf,len);
					temp.flag |= PL_CP_SERIALNUM_FLAG;
					break;
#if !defined(TCSUPPORT_CY)
				case PL_CP_WEBPWD:
					memset(temp.webpwd,0,sizeof(temp.webpwd));
					strncpy(temp.webpwd,(char*)buf,len);
					temp.flag |= PL_CP_WEBPWD_FLAG;
					break;
				case PL_CP_WEB_ACCOUNT:
					memset(temp.webAcct,0,sizeof(temp.webAcct));
					strncpy(temp.webAcct,(char*)buf,len);
					temp.flag |= PL_CP_WEB_ACCOUNT_FLAG;
					break;				
#endif
#if defined(TCSUPPORT_RESERVEAREA_EXTEND) || defined(TCSUPPORT_C7) || (defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505))
				case PL_CP_SSID:
					memset(temp.ssid,0,sizeof(temp.ssid));
					strncpy(temp.ssid,(char*)buf,len);
					temp.flag |= PL_CP_SSID_FLAG;
					break;
				case PL_CP_SSID2nd:
					memset(temp.ssid2nd,0,sizeof(temp.ssid2nd));
					strncpy(temp.ssid2nd,(char*)buf,len);
					temp.flag |= PL_CP_SSID2nd_FLAG;
					break;
				case PL_CP_WPAKEY:
					memset(temp.wpakey,0,sizeof(temp.wpakey));
					strncpy(temp.wpakey,(char*)buf,len);
					temp.flag |= PL_CP_WPAKEY_FLAG;
					break;
				case PL_CP_WPAKEY2nd:
					memset(temp.wpakey2nd,0,sizeof(temp.wpakey2nd));
					strncpy(temp.wpakey2nd,(char*)buf,len);
					temp.flag |= PL_CP_WPAKEY2nd_FLAG;
					break;
				case PL_CP_HW_VER:
					memset(temp.hwver, 0, sizeof(temp.hwver));
					strncpy(temp.hwver, (char*)buf, len);
					temp.flag |= PL_CP_HW_VER_FLAG;
					break;
				case PL_CP_ROMFILE_SELECT:
					memset(temp.romfileselect, 0, sizeof(temp.romfileselect));
					strncpy(temp.romfileselect, (char*)buf, len);
					temp.flag |= PL_CP_ROMFILE_SELECT_FLAG;
					break;
#if !defined(TCSUPPORT_C7) && !(defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505))
				case PL_CP_PPPUSRNAME:
					memset(temp.pppusrname,0,sizeof(temp.pppusrname));
					strncpy(temp.pppusrname,(char*)buf,len);
					temp.flag |= PL_CP_PPPUSRNAME_FLAG;
					break;
				case PL_CP_PPPPWD:
					memset(temp.ppppwd,0,sizeof(temp.ppppwd));
					strncpy(temp.ppppwd,(char*)buf,len);
					temp.flag |= PL_CP_PPPPWD_FLAG;
					break;
				case PL_CP_CFEUSRNAME:
					memset(temp.cfeusrname,0,sizeof(temp.cfeusrname));
					strncpy(temp.cfeusrname,(char*)buf,len);
					temp.flag |= PL_CP_CFEUSRNAME_FLAG;
					break;
				case PL_CP_CFEPWD:
					memset(temp.cfepwd,0,sizeof(temp.cfepwd));
					strncpy(temp.cfepwd,(char*)buf,len);
					temp.flag |= PL_CP_CFEPWD_FLAG;
					break;
				case PL_CP_XPONSN:
					memset(temp.xponsn,0,sizeof(temp.xponsn));
					strncpy(temp.xponsn,(char*)buf,len);
					temp.flag |= PL_CP_XPONSN_FLAG;
					break;
				case PL_CP_XPONPWD:
					memset(temp.xponpwd,0,sizeof(temp.xponpwd));
					strncpy(temp.xponpwd,(char*)buf,len);
					temp.flag |= PL_CP_XPONPWD_FLAG;
					break;
				case PL_CP_MACADDR:
					memset(temp.macaddr,0,sizeof(temp.macaddr));
					strncpy(temp.macaddr,(char*)buf,len);
					temp.flag |= PL_CP_MACADDR_FLAG;
					break;
				case PL_CP_TELNET:
					memset(temp.telnet,0,sizeof(temp.telnet));
					strncpy(temp.telnet,(char*)buf,len);
					temp.flag |= PL_CP_TELNET_FLAG;
					break;
				case PL_CP_XPONMODE:
					memset(temp.xponmode,0,sizeof(temp.xponmode));
					strncpy(temp.xponmode,(char*)buf,len);
					temp.flag |= PL_CP_XPONMODE_FLAG;
					break;
#endif // #if !defined(TCSUPPORT_C7)
#endif // #if defined(TCSUPPORT_RESERVEAREA_EXTEND) || defined(TCSUPPORT_C7)
#if defined(TCSUPPORT_FON) || defined(TCSUPPORT_FON_V2)
				case PL_FON_KEYWORD:
					memset(temp.fonKeyword,0,sizeof(temp.fonKeyword));
					strncpy(temp.fonKeyword,(char*)buf,len);
					temp.flag |= PL_FON_KEYWORD_FLAG;
					break;
				case PL_FON_MAC:
					memset(temp.fonMac,0,sizeof(temp.fonMac));
					strncpy(temp.fonMac,(char*)buf,len);
					temp.flag |= PL_FON_MAC_FLAG;
					break;
#endif
#if defined(TCSUPPORT_WAN_GPON) || defined(TCSUPPORT_WAN_EPON)
#ifndef TCSUPPORT_RESERVEAREA_EXTEND
				case PL_PON_MACNUM:
					memset(temp.ponMacNum, 0, sizeof(temp.ponMacNum));
					strncpy(temp.ponMacNum, (char*)buf, len);
					temp.flag |= PL_PON_MACNUM_FLAG;
					break;
				case PL_PON_USERADMIN_PASSWD:
					memset(temp.ponUseradminPasswd, 0, sizeof(temp.ponUseradminPasswd));
					strncpy(temp.ponUseradminPasswd, (char*)buf, len);
					temp.flag |= PL_PON_USERADMIN_PASSWD_FLAG;
					break;
#endif
#if defined(TCSUPPORT_WAN_GPON)
#ifndef TCSUPPORT_RESERVEAREA_EXTEND
				case PL_GPON_SN:
					memset(temp.gponSn, 0, sizeof(temp.gponSn));
					strncpy(temp.gponSn, (char*)buf, len);
					temp.flag |= PL_GPON_SN_FLAG;
					break;
				case PL_GPON_PASSWD:
					if(len >= GPONPASSWDLEN)
					{
						strncpy(temp.gponPasswd, buf, GPONPASSWDLEN-1);
						strncpy(temp.gponExtPwd, buf + GPONPASSWDLEN-1, len - GPONPASSWDLEN + 1);
						temp.gponPasswd[GPONPASSWDLEN-1]='\0';
						temp.gponExtPwd[len-GPONPASSWDLEN+1]='\0';
					}
					else
					{
						strncpy(temp.gponPasswd, (char*)buf, len);
						memset(temp.gponExtPwd, 0, sizeof(temp.gponExtPwd));	
					}
					temp.flag |= PL_GPON_EXTPWD_FLAG;
					temp.flag &= ~(PL_GPON_PASSWD_FLAG);
					break;
#endif
#endif
#endif

#if defined(TCSUPPORT_RESERVEAREA_EXTEND) || defined(TCSUPPORT_CY)
				case PL_CP_BARCODE:
					memset(temp.barcode, 0, sizeof(temp.barcode));
					strncpy(temp.barcode, (char*)buf, len);
					temp.flag |= PL_CP_BARCODE_FLAG;
					break;
#endif
				default:
					printf("\r\n[setProLinePara]Not support this para in cwmp para!!");
					break;		
			}
			temp.magic = CWMPPARAMAGIC;
			fwrite(&temp, sizeof(proline_Para), 1, fpwrite);
			break;
			
		default:
			printf("\r\n[setProLinePara]Not support this para!!");
			break;
	}

	fclose(fpwrite);
	fclose(fpread);
	
	//write para from reservearea to file first
	sprintf(cmdbuf,TC_FLASH_WRITE_CMD,plpara->para.writefile,plpara->para.flashsize,plpara->para.flashoffset,plpara->para.zonename);
	system(cmdbuf);

	unlink(plpara->para.writefile);
	unlink(plpara->para.readfile);
	return SUCCESS;
}

#if defined(TCSUPPORT_CT)
#ifdef TCSUPPORT_MT7570
static int setProLinePara2(proLineFlag *plpara, int offset, void*buf, int len)
{
	char cmdbuf[200] = {0};
	proline_Para temp;
	char *p = NULL;
	FILE *fpread = NULL, *fp = NULL;
	FILE *fpwrite = NULL;
	fprintf(stderr,"buf is:%s\t",buf);
	fprintf(stderr,"len is:%d\n",len);
	
	memset(&temp,0,sizeof(temp));
	//read para from reservearea to file first
	sprintf(cmdbuf,TC_FLASH_READ_CMD,plpara->para.readfile,plpara->para.flashsize,plpara->para.flashoffset,plpara->para.zonename);
	system(cmdbuf);
	
	//open file and read para to struct
	fpread = fopen(plpara->para.readfile, "r");
	if (fpread == NULL) 
	{
		printf("\r\nsetProLineCwmpPara:open file(%s) fail!",plpara->para.readfile);
		return FAIL;
	}

	//open file and read para to struct
	fpwrite = fopen(plpara->para.writefile, "w+");
	if (fpwrite == NULL) 
	{
		printf("\r\nsetProLineCwmpPara:open file(%s) fail!",plpara->para.writefile);
		fclose(fpread);
		return FAIL;
	}
	
	switch(plpara->type)
	{
		case PL_CP:
			//read all cwmpara and replace any other para according to flag
			fread(&temp, sizeof(proline_Para), 1, fpread);
			if(plpara->flag <= PL_CP_END  &&  plpara->flag >= PL_CP_PRODUCTCLASS ){
				p =(char *) &temp + offset;
				memset(p,  0,  len);
				memcpy(p, (char*)buf, len);
				p [len-1]= '\0';
				temp.flag |= 1 << plpara->flag;
			}
			
			temp.magic = CWMPPARAMAGIC;
			fwrite(&temp, sizeof(proline_Para), 1, fpwrite);
			break;
		default:
			printf("\r\n[setProLinePara]Not support this para!!");
			break;
	}

	fclose(fpwrite);
	fclose(fpread);
	
	//write para from reservearea to file first
	sprintf(cmdbuf,TC_FLASH_WRITE_CMD,plpara->para.writefile,plpara->para.flashsize,plpara->para.flashoffset,plpara->para.zonename);
	system(cmdbuf);

	unlink(plpara->para.writefile);
	unlink(plpara->para.readfile);
	return SUCCESS;
}
#endif
#endif
static void setPLCwmpParaDefault(proLineFlag*paraptr)
{
	paraptr->type = PL_CP;
	paraptr->flag = PL_CP_PRODUCTCLASS;
	paraptr->para.flashsize = getProlineCwmparaRaSize();
	paraptr->para.flashoffset = getProlineCwmparaRaOffset();
	strcpy(paraptr->para.zonename,RESERVEAREA_NAME);
	strcpy(paraptr->para.readfile,PROLINECMD_CWMPPARAREAD_FILE);
	strcpy(paraptr->para.writefile,PROLINECMD_CWMPPARAWRITE_FILE);
	return;
}
/*------------------------/
/		cmd function		   /										
/------------------------*/
static int doProductClass(int argc, char *argv[], void *p)
{
#if defined(TCSUPPORT_RESERVEAREA_EXTEND) || defined(TCSUPPORT_C7) || (defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505))
	proline_Para cwmppara;
#else
	proline_cwmpPara cwmppara;
#endif
	proLineFlag plpara;

	showInfo(argc,argv,p);
	memset(&plpara,0,sizeof(proLineFlag));
#if defined(TCSUPPORT_RESERVEAREA_EXTEND) || defined(TCSUPPORT_C7) || (defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505))
	memset(&cwmppara,0,sizeof(proline_Para));
#else
	memset(&cwmppara,0,sizeof(proline_cwmpPara));
#endif
	
	setPLCwmpParaDefault(&plpara);
	
	if((argc == 2)&&(!strcmp(argv[1] , "display") ))
	{
		getProLinePara(&plpara,&cwmppara);
		if(CWMPPARAMAGIC == cwmppara.magic)
		{
			printf("productclass:%s\r\n",cwmppara.productclass);
		}
		else
		{
			printf("NotValue\r\n");
		}
	}
	else if((argc == 3)&&(!strcmp(argv[1] , "set") ))
	{
		if(strlen(argv[2]) > PRDDUCTCLASSLEN-1)
			goto errorHandle;
		strcpy(cwmppara.productclass,argv[2]);
		cwmppara.magic = CWMPPARAMAGIC;
		setProLinePara(&plpara,argv[2],strlen(argv[2]));
	}
	else
	{
		goto errorHandle;
	}

	return SUCCESS;

errorHandle:
	printf("\r\nUsage:prolinecmd productclass set <value(1~63 characters)>\n");
	return FAIL;
}

static int doManufacturerOUI(int argc, char *argv[], void *p)
{
#if defined(TCSUPPORT_RESERVEAREA_EXTEND) || defined(TCSUPPORT_C7) || (defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505))
	proline_Para cwmppara;
#else
	proline_cwmpPara cwmppara;
#endif
	proLineFlag plpara;

	showInfo(argc,argv,p);
	memset(&plpara,0,sizeof(proLineFlag));
#if defined(TCSUPPORT_RESERVEAREA_EXTEND) || defined(TCSUPPORT_C7) || (defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505))
	memset(&cwmppara,0,sizeof(proline_Para));
#else
	memset(&cwmppara,0,sizeof(proline_cwmpPara));
#endif
	
	setPLCwmpParaDefault(&plpara);
	plpara.flag = PL_CP_MANUFACUREROUI;
	if((argc == 2)&&(!strcmp(argv[1] , "display") ))
	{
		getProLinePara(&plpara,&cwmppara);
		if(CWMPPARAMAGIC == cwmppara.magic)
		{
			printf("ManufacturerOUI:%s\r\n",cwmppara.manufacturerOUI);
		}
		else
		{
			printf("NotValue\r\n");
		}
	}
	else if((argc == 3)&&(!strcmp(argv[1] , "set") ))
	{
		if(strlen(argv[2]) > MANUFACUREROUILEN-1)
			goto errorHandle;
		strcpy(cwmppara.manufacturerOUI,argv[2]);
		cwmppara.magic = CWMPPARAMAGIC;
		setProLinePara(&plpara,argv[2],strlen(argv[2]));
	}
	else
	{
		goto errorHandle;
	}

	return SUCCESS;

errorHandle:
	printf("\r\nUsage:prolinecmd ManufacturerOUI set <value(1~63 characters)>\n");
	return FAIL;
}

static int doSerialnum(int argc, char *argv[], void *p)
{
#if defined(TCSUPPORT_RESERVEAREA_EXTEND) || defined(TCSUPPORT_C7) || (defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505))
	proline_Para cwmppara;
#else
	proline_cwmpPara cwmppara;
#endif
	proLineFlag plpara;

	showInfo(argc,argv,p);
	memset(&plpara,0,sizeof(proLineFlag));
#if defined(TCSUPPORT_RESERVEAREA_EXTEND) || defined(TCSUPPORT_C7) || (defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505))
	memset(&cwmppara,0,sizeof(proline_Para));
#else
	memset(&cwmppara,0,sizeof(proline_cwmpPara));
#endif
	
	setPLCwmpParaDefault(&plpara);
	plpara.flag = PL_CP_SERIALNUM;
	if((argc == 2)&&(!strcmp(argv[1] , "display") ))
	{
		getProLinePara(&plpara,&cwmppara);
		if(CWMPPARAMAGIC == cwmppara.magic)
		{
			printf("SerialNum:%s\r\n",cwmppara.serialnum);
		}
		else
		{
			printf("NotValue\r\n");
		}
	}
	else if((argc == 3)&&(!strcmp(argv[1] , "set") ))
	{
		if(strlen(argv[2]) > SERIALNUMLEN-1)
			goto errorHandle;
//		strcpy(cwmppara.serialnum,argv[2]);
//		cwmppara.magic = CWMPPARAMAGIC;
		setProLinePara(&plpara,argv[2],strlen(argv[2]));
	}
	else
	{
		goto errorHandle;
	}

	return SUCCESS;

errorHandle:
	printf("\r\nUsage:prolinecmd serialnum set <value(1~127 characters)>\n");
	return FAIL;
}

#if defined(TCSUPPORT_FON) || defined(TCSUPPORT_FON_V2)
static int doFonKeyword(int argc, char *argv[], void *p)
{
	proline_cwmpPara cwmppara;
	proLineFlag plpara;

	showInfo(argc,argv,p);
	memset(&plpara,0,sizeof(proLineFlag));
	memset(&cwmppara,0,sizeof(proline_cwmpPara));
	
	setPLCwmpParaDefault(&plpara);
	plpara.flag = PL_FON_KEYWORD;
	if((argc == 2)&&(!strcmp(argv[1] , "display") ))
	{
		getProLinePara(&plpara,&cwmppara);
		if(CWMPPARAMAGIC == cwmppara.magic)
		{
			printf("Fon. Keyword:%s\r\n",cwmppara.fonKeyword);
		}
		else
		{
			printf("NotValue\r\n");
		}
	}
	else if((argc == 3)&&(!strcmp(argv[1] , "set") ))
	{
		if(strlen(argv[2]) != FONKEYWORDLEN-1)
			goto errorHandle;
		strncpy(cwmppara.fonKeyword,argv[2],sizeof(cwmppara.fonKeyword));
		cwmppara.magic = CWMPPARAMAGIC;
		setProLinePara(&plpara,argv[2],strlen(argv[2]));
	}
	else
	{
		goto errorHandle;
	}

	return SUCCESS;

errorHandle:
	printf("\r\nUsage:prolinecmd fonKeyword set <value(64 characters)>\n");
	return FAIL;
}

static int doFonMac(int argc, char *argv[], void *p)
{
	proline_cwmpPara cwmppara;
	proLineFlag plpara;
	char * tmp = argv[2];
	int i;

	showInfo(argc,argv,p);
	memset(&plpara,0,sizeof(proLineFlag));
	memset(&cwmppara,0,sizeof(proline_cwmpPara));
	
	setPLCwmpParaDefault(&plpara);
	plpara.flag = PL_FON_MAC;
	if((argc == 2)&&(!strcmp(argv[1] , "display") ))
	{
		getProLinePara(&plpara,&cwmppara);
		if(CWMPPARAMAGIC == cwmppara.magic)
		{
			printf("FON. Mac:%s\r\n",cwmppara.fonMac);
		}
		else
		{
			printf("NotValue\r\n");
		}
	}
	else if((argc == 3)&&(!strcmp(argv[1] , "set") ))
	{
		if(strlen(argv[2]) != FONMACLEN-1)
			goto errorHandle;
		for(i=2 ;i<17; i+=3){		
			if(*(tmp+i) != '-')
				goto errorHandle;	
		}
		strncpy(cwmppara.fonMac,argv[2],sizeof(cwmppara.fonMac));
		cwmppara.magic = CWMPPARAMAGIC;
		setProLinePara(&plpara,argv[2],strlen(argv[2]));
	}
	else
	{
		goto errorHandle;
	}

	return SUCCESS;

errorHandle:
	printf("\r\nUsage:prolinecmd fonMac set <value(17 characters), xx-xx-xx-xx-xx-xx>\n");
	return FAIL;
}
#endif	

#if defined(TCSUPPORT_RESERVEAREA_EXTEND) || defined(TCSUPPORT_CY)
static int doproductBarCode(int argc,char *argv[], void *p)
{
#ifdef TCSUPPORT_RESERVEAREA_EXTEND
	proline_Para cwmppara;
#else
	proline_cwmpPara cwmppara;
#endif
	proLineFlag plpara;

	showInfo(argc,argv,p);
	memset(&plpara,0,sizeof(proLineFlag));
#ifdef TCSUPPORT_RESERVEAREA_EXTEND
	memset(&cwmppara,0,sizeof(proline_Para));
#else
	memset(&cwmppara,0,sizeof(proline_cwmpPara));
#endif
	
	setPLCwmpParaDefault(&plpara);
	plpara.flag = PL_CP_BARCODE;
	
	if((argc == 2)&&(!strcmp(argv[1] , "display") ))
	{
		getProLinePara(&plpara,&cwmppara);
		if(CWMPPARAMAGIC == cwmppara.magic)
		{
			printf("barcode:%s\r\n",cwmppara.barcode);
		}
		else
		{
			printf("NotValue\r\n");
		}
	}
	else if((argc == 3)&&(!strcmp(argv[1] , "set") ))
	{
		if(strlen(argv[2]) > BARCODELEN - 1)
			goto errorHandle;
		strcpy(cwmppara.barcode, argv[2]);
		cwmppara.magic = CWMPPARAMAGIC;
		setProLinePara(&plpara,argv[2],strlen(argv[2]));
	}
	else
	{
		goto errorHandle;
	}

	return SUCCESS;

errorHandle:
	printf("\r\nUsage:prolinecmd barcode set <value(1~31 characters)>\n");
	return FAIL;

}
#endif
#if defined(TCSUPPORT_RESERVEAREA_EXTEND) || defined(TCSUPPORT_C7) || (defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505))
static int doSsid(int argc, char *argv[], void *p)
{
	proline_Para cwmppara;
	proLineFlag plpara;

	showInfo(argc,argv,p);
	memset(&plpara,0,sizeof(proLineFlag));
	memset(&cwmppara,0,sizeof(proline_Para));
	
	setPLCwmpParaDefault(&plpara);
	plpara.flag = PL_CP_SSID;
	if((argc == 2)&&(!strcmp(argv[1] , "display") ))
	{
		getProLinePara(&plpara,&cwmppara);
		if(CWMPPARAMAGIC == cwmppara.magic)
		{
			printf("SSID:%s\r\n",cwmppara.ssid);
		}
		else
		{
			printf("NotValue\r\n");
		}
	}
	else if((argc == 3)&&(!strcmp(argv[1] , "set") ))
	{
		if(strlen(argv[2]) > SSIDLEN-1)
			goto errorHandle;
//		strcpy(cwmppara.ssid,argv[2]);
//		cwmppara.magic = CWMPPARAMAGIC;
		setProLinePara(&plpara,argv[2],strlen(argv[2]));
	}
	else
	{
		goto errorHandle;
	}

	return SUCCESS;

errorHandle:
	printf("\r\nUsage:prolinecmd ssid set <value(1~63 characters)>\n");
	return FAIL;

}

static int doSsid2nd(int argc, char *argv[], void *p)
{
	proline_Para cwmppara;
	proLineFlag plpara;

	showInfo(argc,argv,p);
	memset(&plpara,0,sizeof(proLineFlag));
	memset(&cwmppara,0,sizeof(proline_Para));
	
	setPLCwmpParaDefault(&plpara);
	plpara.flag = PL_CP_SSID2nd;
	if((argc == 2)&&(!strcmp(argv[1] , "display") ))
	{
		getProLinePara(&plpara,&cwmppara);
		if(CWMPPARAMAGIC == cwmppara.magic)
		{
			printf("SSID2nd:%s\r\n",cwmppara.ssid2nd);
		}
		else
		{
			printf("NotValue\r\n");
		}
	}
	else if((argc == 3)&&(!strcmp(argv[1] , "set") ))
	{
		if(strlen(argv[2]) > SSIDLEN-1)
			goto errorHandle;
//		strcpy(cwmppara.ssid,argv[2]);
//		cwmppara.magic = CWMPPARAMAGIC;
		setProLinePara(&plpara,argv[2],strlen(argv[2]));
	}
	else
	{
		goto errorHandle;
	}

	return SUCCESS;

errorHandle:
	printf("\r\nUsage:prolinecmd ssid2nd set <value(1~63 characters)>\n");
	return FAIL;

}

static int doWpaKey(int argc, char *argv[], void *p)
{
	proline_Para cwmppara;
	proLineFlag plpara;

	showInfo(argc,argv,p);
	memset(&plpara,0,sizeof(proLineFlag));
	memset(&cwmppara,0,sizeof(proline_Para));
	
	setPLCwmpParaDefault(&plpara);
	plpara.flag = PL_CP_WPAKEY;
	if((argc == 2)&&(!strcmp(argv[1] , "display") ))
	{
		getProLinePara(&plpara,&cwmppara);
		if(CWMPPARAMAGIC == cwmppara.magic)
		{
			printf("Wpakey:%s\r\n",cwmppara.wpakey);
		}
		else
		{
			printf("NotValue\r\n");
		}
	}
	else if((argc == 3)&&(!strcmp(argv[1] , "set") ))
	{
		if(strlen(argv[2]) > WPAKEYLEN-1)
			goto errorHandle;
//		strcpy(cwmppara.wpakey,argv[2]);
//		cwmppara.magic = CWMPPARAMAGIC;
		setProLinePara(&plpara,argv[2],strlen(argv[2]));
	}
	else
	{
		goto errorHandle;
	}

	return SUCCESS;

errorHandle:
	printf("\r\nUsage:prolinecmd wpakey set <value(1~63 characters)>\n");
	return FAIL;

}

static int doWpaKey2nd(int argc, char *argv[], void *p)
{
	proline_Para cwmppara;
	proLineFlag plpara;

	showInfo(argc,argv,p);
	memset(&plpara,0,sizeof(proLineFlag));
	memset(&cwmppara,0,sizeof(proline_Para));
	
	setPLCwmpParaDefault(&plpara);
	plpara.flag = PL_CP_WPAKEY2nd;
	if((argc == 2)&&(!strcmp(argv[1] , "display") ))
	{
		getProLinePara(&plpara,&cwmppara);
		if(CWMPPARAMAGIC == cwmppara.magic)
		{
			printf("Wpakey2nd:%s\r\n",cwmppara.wpakey2nd);
		}
		else
		{
			printf("NotValue\r\n");
		}
	}
	else if((argc == 3)&&(!strcmp(argv[1] , "set") ))
	{
		if(strlen(argv[2]) > WPAKEYLEN-1)
			goto errorHandle;
//		strcpy(cwmppara.wpakey,argv[2]);
//		cwmppara.magic = CWMPPARAMAGIC;
		setProLinePara(&plpara,argv[2],strlen(argv[2]));
	}
	else
	{
		goto errorHandle;
	}

	return SUCCESS;

errorHandle:
	printf("\r\nUsage:prolinecmd wpakey2nd set <value(1~63 characters)>\n");
	return FAIL;

}

static int doHwVersion(int argc, char *argv[], void *p)
{
	proline_Para cwmppara;
	proLineFlag plpara;

	showInfo(argc,argv,p);
	memset(&plpara,0,sizeof(proLineFlag));
	memset(&cwmppara,0,sizeof(proline_Para));
	
	setPLCwmpParaDefault(&plpara);
	plpara.flag = PL_CP_HW_VER;
	if((argc == 2)&&(!strcmp(argv[1] , "display") ))
	{
		getProLinePara(&plpara,&cwmppara);
		if(CWMPPARAMAGIC == cwmppara.magic)
		{
			printf("hwver:%s\r\n",cwmppara.hwver);
		}
		else
		{
			printf("NotValue\r\n");
		}
	}
	else if((argc == 3)&&(!strcmp(argv[1] , "set") ))
	{
		if(strlen(argv[2]) > HWVERLEN -1)
			goto errorHandle;
//		strcpy(cwmppara.wpakey,argv[2]);
//		cwmppara.magic = CWMPPARAMAGIC;
		setProLinePara(&plpara,argv[2],strlen(argv[2]));
	}
	else
	{
		goto errorHandle;
	}

	return SUCCESS;

errorHandle:
	printf("\r\nUsage:prolinecmd hwver set <value(1~31 characters)>\n");
	return FAIL;

}


#if !defined(TCSUPPORT_C7)
static int doPppUsrname(int argc, char *argv[], void *p)
{
	proline_Para cwmppara;
	proLineFlag plpara;

	showInfo(argc,argv,p);
	memset(&plpara,0,sizeof(proLineFlag));
	memset(&cwmppara,0,sizeof(proline_Para));
	
	setPLCwmpParaDefault(&plpara);
	plpara.flag = PL_CP_PPPUSRNAME;
	if((argc == 2)&&(!strcmp(argv[1] , "display") ))
	{
		getProLinePara(&plpara,&cwmppara);
		if(CWMPPARAMAGIC == cwmppara.magic)
		{
			printf("PPPusrname:%s\r\n",cwmppara.pppusrname);
		}
		else
		{
			printf("NotValue\r\n");
		}
	}
	else if((argc == 3)&&(!strcmp(argv[1] , "set") ))
	{
		if(strlen(argv[2]) > PPPUSRNAMELEN-1)
			goto errorHandle;
//		strcpy(cwmppara.pppusrname,argv[2]);
//		cwmppara.magic = CWMPPARAMAGIC;
		setProLinePara(&plpara,argv[2],strlen(argv[2]));
	}
	else
	{
		goto errorHandle;
	}

	return SUCCESS;

errorHandle:
	printf("\r\nUsage:prolinecmd pppusrname set <value(1~63 characters)>\n");
	return FAIL;

}

static int doPppPwd(int argc, char *argv[], void *p)
{
	proline_Para cwmppara;
	proLineFlag plpara;

	showInfo(argc,argv,p);
	memset(&plpara,0,sizeof(proLineFlag));
	memset(&cwmppara,0,sizeof(proline_Para));
	
	setPLCwmpParaDefault(&plpara);
	plpara.flag = PL_CP_PPPPWD;
	if((argc == 2)&&(!strcmp(argv[1] , "display") ))
	{
		getProLinePara(&plpara,&cwmppara);
		if(CWMPPARAMAGIC == cwmppara.magic)
		{
			printf("PPPpwd:%s\r\n",cwmppara.ppppwd);
		}
		else
		{
			printf("NotValue\r\n");
		}
	}
	else if((argc == 3)&&(!strcmp(argv[1] , "set") ))
	{
		if(strlen(argv[2]) > PPPPWDLEN-1)
			goto errorHandle;
//		strcpy(cwmppara.ppppwd,argv[2]);
//		cwmppara.magic = CWMPPARAMAGIC;
		setProLinePara(&plpara,argv[2],strlen(argv[2]));
	}
	else
	{
		goto errorHandle;
	}

	return SUCCESS;

errorHandle:
	printf("\r\nUsage:prolinecmd ppppwd set <value(1~63 characters)>\n");
	return FAIL;

}

static int doCfeUsrname(int argc, char *argv[], void *p)
{
	proline_Para cwmppara;
	proLineFlag plpara;

	showInfo(argc,argv,p);
	memset(&plpara,0,sizeof(proLineFlag));
	memset(&cwmppara,0,sizeof(proline_Para));
	
	setPLCwmpParaDefault(&plpara);
	plpara.flag = PL_CP_CFEUSRNAME;
	if((argc == 2)&&(!strcmp(argv[1] , "display") ))
	{
		getProLinePara(&plpara,&cwmppara);
		if(CWMPPARAMAGIC == cwmppara.magic)
		{
			printf("Cfeusrname:%s\r\n",cwmppara.cfeusrname);
		}
		else
		{
			printf("NotValue\r\n");
		}
	}
	else if((argc == 3)&&(!strcmp(argv[1] , "set") ))
	{
		if(strlen(argv[2]) > CFEUSRNAMELEN-1)
			goto errorHandle;
//		strcpy(cwmppara.cfeusrname,argv[2]);
//		cwmppara.magic = CWMPPARAMAGIC;
		setProLinePara(&plpara,argv[2],strlen(argv[2]));
	}
	else
	{
		goto errorHandle;
	}

	return SUCCESS;

errorHandle:
	printf("\r\nUsage:prolinecmd cfeusrname set <value(1~63 characters)>\n");
	return FAIL;

}

static int doCfePwd(int argc, char *argv[], void *p)
{
	proline_Para cwmppara;
	proLineFlag plpara;

	showInfo(argc,argv,p);
	memset(&plpara,0,sizeof(proLineFlag));
	memset(&cwmppara,0,sizeof(proline_Para));
	
	setPLCwmpParaDefault(&plpara);
	plpara.flag = PL_CP_CFEPWD;
	if((argc == 2)&&(!strcmp(argv[1] , "display") ))
	{
		getProLinePara(&plpara,&cwmppara);
		if(CWMPPARAMAGIC == cwmppara.magic)
		{
			printf("Cfepwd:%s\r\n",cwmppara.cfepwd);
		}
		else
		{
			printf("NotValue\r\n");
		}
	}
	else if((argc == 3)&&(!strcmp(argv[1] , "set") ))
	{
		if(strlen(argv[2]) > CFEPWDLEN-1)
			goto errorHandle;
//		strcpy(cwmppara.cfepwd,argv[2]);
//		cwmppara.magic = CWMPPARAMAGIC;
		setProLinePara(&plpara,argv[2],strlen(argv[2]));
	}
	else
	{
		goto errorHandle;
	}

	return SUCCESS;

errorHandle:
	printf("\r\nUsage:prolinecmd ppppwd set <value(1~63 characters)>\n");
	return FAIL;

}

static int doXponSn(int argc, char *argv[], void *p)
{
	proline_Para cwmppara;
	proLineFlag plpara;

	showInfo(argc,argv,p);
	memset(&plpara,0,sizeof(proLineFlag));
	memset(&cwmppara,0,sizeof(proline_Para));
	
	setPLCwmpParaDefault(&plpara);
	plpara.flag = PL_CP_XPONSN;
	if((argc == 2)&&(!strcmp(argv[1] , "display") ))
	{
		getProLinePara(&plpara,&cwmppara);
		if(CWMPPARAMAGIC == cwmppara.magic)
		{
			printf("XponSN:%s\r\n",cwmppara.xponsn);
		}
		else
		{
			printf("NotValue\r\n");
		}
	}
	else if((argc == 3)&&(!strcmp(argv[1] , "set") ))
	{
		if(strlen(argv[2]) > XPONSNLEN-1)
			goto errorHandle;
//		strcpy(cwmppara.cfeusrname,argv[2]);
//		cwmppara.magic = CWMPPARAMAGIC;
		setProLinePara(&plpara,argv[2],strlen(argv[2]));
	}
	else
	{
		goto errorHandle;
	}

	return SUCCESS;

errorHandle:
	printf("\r\nUsage:prolinecmd xponsn set <value(1~31 characters)>\n");
	return FAIL;

}

static int doXponPwd(int argc, char *argv[], void *p)
{
	proline_Para cwmppara;
	proLineFlag plpara;

	showInfo(argc,argv,p);
	memset(&plpara,0,sizeof(proLineFlag));
	memset(&cwmppara,0,sizeof(proline_Para));
	
	setPLCwmpParaDefault(&plpara);
	plpara.flag = PL_CP_XPONPWD;
	if((argc == 2)&&(!strcmp(argv[1] , "display") ))
	{
		getProLinePara(&plpara,&cwmppara);
		if(CWMPPARAMAGIC == cwmppara.magic)
		{
			printf("XponPwd:%s\r\n",cwmppara.xponpwd);
		}
		else
		{
			printf("NotValue\r\n");
		}
	}
	else if((argc == 3)&&(!strcmp(argv[1] , "set") ))
	{
		if(strlen(argv[2]) > XPONPWDLEN-1)
			goto errorHandle;
//		strcpy(cwmppara.cfepwd,argv[2]);
//		cwmppara.magic = CWMPPARAMAGIC;
		setProLinePara(&plpara,argv[2],strlen(argv[2]));
	}
	else
	{
		goto errorHandle;
	}

	return SUCCESS;

errorHandle:
	printf("\r\nUsage:prolinecmd xponpwd set <value(1~31 characters)>\n");
	return FAIL;

}

static int doMacAddr(int argc, char *argv[], void *p)
{
	proline_Para cwmppara;
	proLineFlag plpara;

	showInfo(argc,argv,p);
	memset(&plpara,0,sizeof(proLineFlag));
	memset(&cwmppara,0,sizeof(proline_Para));
	
	setPLCwmpParaDefault(&plpara);
	plpara.flag = PL_CP_MACADDR;
	if((argc == 2)&&(!strcmp(argv[1] , "display") ))
	{
		getProLinePara(&plpara,&cwmppara);
		if(CWMPPARAMAGIC == cwmppara.magic)
		{
			printf("Macaddr:%s\r\n",cwmppara.macaddr);
		}
		else
		{
			printf("NotValue\r\n");
		}
	}
	else if((argc == 3)&&(!strcmp(argv[1] , "set") ))
	{
		if(strlen(argv[2]) > MACADDRLEN-1)
			goto errorHandle;
//		strcpy(cwmppara.macaddr,argv[2]);
//		cwmppara.magic = CWMPPARAMAGIC;
		setProLinePara(&plpara,argv[2],strlen(argv[2]));
	}
	else
	{
		goto errorHandle;
	}

	return SUCCESS;

errorHandle:
	printf("\r\nUsage:prolinecmd macaddr set <value(1~31 characters)>\n");
	return FAIL;

}

static int doXponMode(int argc, char *argv[], void *p)
{
	proline_Para cwmppara;
	proLineFlag plpara;

	showInfo(argc,argv,p);
	memset(&plpara,0,sizeof(proLineFlag));
	memset(&cwmppara,0,sizeof(proline_Para));
	
	setPLCwmpParaDefault(&plpara);
	plpara.flag = PL_CP_XPONMODE;
	if((argc == 2)&&(!strcmp(argv[1] , "display") ))
	{
		getProLinePara(&plpara,&cwmppara);
		if(CWMPPARAMAGIC == cwmppara.magic)
		{
			printf("XponMode:%s\r\n",cwmppara.xponmode);
		}
		else
		{
			printf("NotValue\r\n");
		}
	}
	else if((argc == 3)&&(!strcmp(argv[1] , "set") ))
	{
		if(strlen(argv[2]) > XPONMODELEN-1)
			goto errorHandle;
//		strcpy(cwmppara.macaddr,argv[2]);
//		cwmppara.magic = CWMPPARAMAGIC;
		setProLinePara(&plpara,argv[2],strlen(argv[2]));
	}
	else
	{
		goto errorHandle;
	}

	return SUCCESS;

errorHandle:
	printf("\r\nUsage:prolinecmd xponmode set <value(1~7 characters)>\n");
	return FAIL;

}

static int doRomfileselect(int argc, char *argv[], void *p)
{
	proline_Para cwmppara;
	proLineFlag plpara;
	char syscmd[100]={0};

	showInfo(argc,argv,p);
	memset(&plpara,0,sizeof(proLineFlag));
	memset(&cwmppara,0,sizeof(proline_Para));
	
	setPLCwmpParaDefault(&plpara);
	plpara.flag = PL_CP_ROMFILE_SELECT;
	if((argc == 2)&&(!strcmp(argv[1] , "display") ))
	{
		getProLinePara(&plpara,&cwmppara);
		if(CWMPPARAMAGIC == cwmppara.magic)
		{
			printf("Romfileselect:%s\r\n",cwmppara.romfileselect);
		}
		else
		{
			printf("NotValue\r\n");
		}
	}
	else if((argc == 3)&&(!strcmp(argv[1] , "set") ))
	{
		if(strlen(argv[2]) > ROMFILESELECTLEN-1)
			goto errorHandle;
		setProLinePara(&plpara,argv[2],strlen(argv[2]));

		memset(syscmd,0,sizeof(syscmd));
		sprintf(syscmd,"tcapi set System_Entry romfileSelect %s",argv[2]);
		system(syscmd);
	}
	else
	{
		goto errorHandle;
	}

	return SUCCESS;

errorHandle:
	printf("\r\nUsage:prolinecmd romfileselect set <value(1~63 characters)>\n");
	return FAIL;

}

#if defined(TCSUPPORT_CT)
#ifdef TCSUPPORT_MT7570
static int doMt7570Bob(int argc, char *argv[], void *p)
{
	proline_Para cwmppara;
	proLineFlag plpara;
	char syscmd[100]={0};
	FILE *fp = NULL;
	int i = 0;
	char *pi = NULL;

	showInfo(argc,argv,p);
	memset(&plpara,0,sizeof(proLineFlag));
	memset(&cwmppara,0,sizeof(proline_Para));
	
	setPLCwmpParaDefault(&plpara);
	plpara.flag = PL_CP_MT7570BOB;
	if((argc == 2)&&(!strcmp(argv[1], "display") ))
	{
		fp =  fopen(MT7570BOB_CONF, "r");
		if (fp == NULL) 
		{
			printf("\r\nsetProLineCwmpPara:open file(mt7570_bob.conf) fail!");						
			return FAIL;
		}
		fread(cwmppara.mt7570bob, sizeof(cwmppara.mt7570bob), 1, fp);
		fclose(fp);
		pi = cwmppara.mt7570bob;
		for(i= 0; i< MT7570BOBLEN; i++){
			if((i&0x0f) == 0) printf("\n%08xh:",i);
			printf(" %02X", (unsigned char)pi[i]);			
		}
	}
	else if((argc == 2)&&(!strcmp(argv[1] , "get") ))
	{
		getProLinePara(&plpara,&cwmppara);
		if(CWMPPARAMAGIC == cwmppara.magic)
		{
			pi = cwmppara.mt7570bob;
			for(i= 0; i< MT7570BOBLEN; i++){
				if( (i&0x0f) == 0 ) printf("\n%08xh:",i);
				printf(" %02X", (unsigned char)pi[i]);
				
			}
		}
		else
		{
			printf("NotValue\r\n");
		}
		fp =  fopen(MT7570BOB_CONF, "w+");
		if (fp == NULL) 
		{
			printf("\r\nsetProLineCwmpPara:open file(mt7570_bob.conf) fail!");						
			return FAIL;
		}
		fwrite(cwmppara.mt7570bob, sizeof(cwmppara.mt7570bob), 1, fp);
		fclose(fp);
	}
	else if((argc == 2)&&(!strcmp(argv[1] , "save") ))
	{
		fp = fopen(MT7570BOB_CONF, "r");
		if (fp == NULL) 
		{
			printf("\r\nsetProLineCwmpPara:open file(mt7570_bob.conf) fail!");						
			return FAIL;
		}
		memset(cwmppara.mt7570bob, 0, sizeof(cwmppara.mt7570bob));
		if(MT7570BOBLEN != fread(cwmppara.mt7570bob, MT7570BOBLEN, 1, fp)){
			printf("\r\nsetProLineCwmpPara:read file(mt7570_bob.conf) fail!");		
		}
		fclose(fp);
		setProLinePara2(&plpara, offsetof(proline_Para, mt7570bob), cwmppara.mt7570bob, MT7570BOBLEN);
	}
	else
	{
		goto errorHandle;
	}

	return SUCCESS;

errorHandle:
	printf("\r\nUsage:prolinecmd mt7570bob get/save/display\n");
	return FAIL;

}
#endif
#endif

static int checkRestoreResult(proline_Para *pOldPara)
{
	proline_Para cwmppara;
	proLineFlag plpara;
	int i = 0;
	char *ptrOld = NULL;
	char *ptrNew = NULL;
	unsigned int counter = 0;

	memset(&plpara,0,sizeof(proLineFlag));
	memset(&cwmppara,0,sizeof(proline_Para));
	
	setPLCwmpParaDefault(&plpara);

	getProLinePara(&plpara,&cwmppara);

	// do the checking, compare the new proline paras with the old ones
	// Two ways to do this job:
	// 1: do memcmp directly, this would be faster
	// 2: check the paras one by one according to "cwmppara.flag"
	// we choose method 1 here
	if (CWMPPARAMAGIC != cwmppara.magic)
	{
		printf("\r\nIncorrect magic:%0x",cwmppara.magic);
		return FAIL;
	}
	
	// check telnet flag
	if(0 == (cwmppara.flag & PL_CP_TELNET_FLAG))
	{
		printf("\r\nTelnet flag is not set.");
		return FAIL;
	}
	else
	{
		if(0 != strncmp(cwmppara.telnet,FLAG_TELNET_DISABLE,strlen(FLAG_TELNET_DISABLE)))
		{
			printf("\r\nIncorrect telnet:%s.",cwmppara.telnet);
			return FAIL;
		}
	}

	// point to the first para
	ptrOld = (char *)(pOldPara->productclass);
	ptrNew = (char *)(cwmppara.productclass);
	counter = sizeof(proline_Para)-sizeof(cwmppara.flag)-sizeof(cwmppara.magic);

	// set telnet flag for old para
	memset(pOldPara->telnet, 0, TELNETLEN);
	strncpy(pOldPara->telnet,FLAG_TELNET_DISABLE,strlen(FLAG_TELNET_DISABLE));

	// do memory compare
	if(0 != memcmp(ptrOld,ptrNew,counter))
	{
		// dump all paras data
		printf("\r\n====== old ======\r\n");
		for(i = 0; i < counter; i++)
		{
			printf("%c",ptrOld[i]);
		}
		printf("\r\n====== New ======\r\n");
		for(i = 0; i < counter; i++)
		{
			printf("%c",ptrNew[i]);
		}
		printf("\r\n");
		return FAIL;
	}
	
	return SUCCESS;
}
#endif
#endif // #ifdef TCSUPPORT_RESERVEAREA_EXTEND

#if !defined(TCSUPPORT_CY)
static int doRestore(int argc, char *argv[], void *p)
{
	proline_Para cwmppara;
	proLineFlag plpara;
	char cmdbuf[128] = {0};
	
	showInfo(argc,argv,p);
	memset(&plpara,0,sizeof(proLineFlag));
	memset(&cwmppara,0,sizeof(proline_Para));
	
	setPLCwmpParaDefault(&plpara);
	plpara.flag = PL_CP_TELNET;

	if((argc == 2)&&(!strcmp(argv[1] , "default") ))
	{
		// erase current config
		snprintf(cmdbuf, sizeof(cmdbuf), TC_FLASH_ERASE_SECTOR_CMD, 0, "romfile");
		system(cmdbuf);
		#ifdef TCSUPPORT_BACKUPROMFILE
		// erase backup config
		snprintf(cmdbuf, sizeof(cmdbuf), TC_FLASH_ERASE_SECTOR_CMD, (unsigned long)BACKUPROMFILE_RA_OFFSET, RESERVEAREA_NAME);
		system(cmdbuf);
		#endif
		#if defined(TCSUPPORT_RESERVEAREA_EXTEND)
		// erase log area
		snprintf(cmdbuf, sizeof(cmdbuf), TC_FLASH_ERASE_SECTOR_CMD, (unsigned long)SYSLOG_RA_OFFSET, RESERVEAREA_NAME);
		system(cmdbuf);
		#endif
		#if defined(TCSUPPORT_C7) || defined(TCSUPPORT_CT_PON_SC)
		// erase default config
		snprintf(cmdbuf, sizeof(cmdbuf), TC_FLASH_ERASE_SECTOR_CMD, (unsigned long)DEFAULTROMFILE_RA_OFFSET, RESERVEAREA_NAME);
		system(cmdbuf);
		#endif
		// erase factory config, optional
		/*
		snprintf(cmdbuf, sizeof(cmdbuf), TC_FLASH_ERASE_SECTOR_CMD, DEFAULTROMFILE_RA_OFFSET, RESERVEAREA_NAME);
		system(cmdbuf);
		*/
	}
	else
	{
		goto errorHandle;
	}

	printf("\r\nrestore default success");
#if defined(TCSUPPORT_CT_PON_C9)
	system("reboot -d 2 &");
#else
	system("reboot");
#endif
	return SUCCESS;

errorHandle:
	printf("\r\nUsage:prolinecmd restore default\n");
	return FAIL;

}


static int doWebAccount(int argc, char *argv[], void *p)
{
	proline_Para cwmppara;
	proLineFlag plpara;

	showInfo(argc,argv,p);
	memset(&plpara,0,sizeof(proLineFlag));
	memset(&cwmppara,0,sizeof(proline_Para));
	
	setPLCwmpParaDefault(&plpara);
	plpara.flag = PL_CP_WEB_ACCOUNT;
	if((argc == 2)&&(!strcmp(argv[1] , "display") ))
	{
		getProLinePara(&plpara,&cwmppara);
		if(CWMPPARAMAGIC == cwmppara.magic && (PL_CP_WEB_ACCOUNT_FLAG & cwmppara.flag)){
		    printf("webAccount:%s\r\n",cwmppara.webAcct);
		}else{
		    printf("NotValue\r\n");
		}
	}
	else if((argc == 3)&&(!strcmp(argv[1] , "set") ))
	{
		if(strlen(argv[2]) > WEBACCOUNTLEN-1)
			goto errorHandle;
		setProLinePara(&plpara,argv[2],strlen(argv[2]));
	}
	else
	{
		goto errorHandle;
	}

	return SUCCESS;

errorHandle:
	printf("\nUsage:prolinecmd webAccount set <value(1~%d characters)>\n", WEBACCOUNTLEN-1);
	return FAIL;
}

static int doWebPwd(int argc, char *argv[], void *p)
{
	proline_Para cwmppara;
	proLineFlag plpara;

	showInfo(argc,argv,p);
	memset(&plpara,0,sizeof(proLineFlag));
	memset(&cwmppara,0,sizeof(proline_Para));
	
	setPLCwmpParaDefault(&plpara);
	plpara.flag = PL_CP_WEBPWD;
	if((argc == 2)&&(!strcmp(argv[1] , "display") ))
	{
		getProLinePara(&plpara,&cwmppara);
		if(CWMPPARAMAGIC == cwmppara.magic && (PL_CP_WEBPWD_FLAG & cwmppara.flag))
		{
		    printf("Webpwd:%s\r\n",cwmppara.webpwd);
		}else{
			printf("NotValue\r\n");
		}
	}
	else if((argc == 3)&&(!strcmp(argv[1] , "set") ))
	{
		if(strlen(argv[2]) > WEBPWDLEN-1)
			goto errorHandle;
//		strcpy(cwmppara.webpwd,argv[2]);
//		cwmppara.magic = CWMPPARAMAGIC;
		setProLinePara(&plpara,argv[2],strlen(argv[2]));
	}
	else
	{
		goto errorHandle;
	}

	return SUCCESS;

errorHandle:
	printf("\r\nUsage:prolinecmd webpwd set <value(1~63 characters)>\n");
	return FAIL;

}
#endif

static int doClear(int argc, char *argv[], void *p)
{
	int flag = 0;
	
	showInfo(argc,argv,p);
	if((argc == 2)&&(!strcmp(argv[0] , "clear") ))
	{
		flag = atoi(argv[1]);
		clearProLinePara(flag);
	}
	else
	{
		goto errorHandle;
	}

	return SUCCESS;

errorHandle:
	printf("\r\nUsage:prolinecmd clear <paratype(1:cwmp)>\n");
	return FAIL;
}

static int doProLineVer(int argc, char *argv[], void *p)
{
	showInfo(argc,argv,p);

	
	if((argc == 2)&&(!strcmp(argv[1] , "display") ))
	{
		printf("prolinecmd version: %s\r\n",MODULE_VERSION_PROLINECMD);
		return SUCCESS;
	}
	else
	{
		printf("\r\nUsage:prolinecmd version display\n");
		return FAIL;
	}
}

static int doProLineHelp(int argc, char *argv[], void *p)
{
	printf("\r\nUsage: \r\n"
		  		"\tprolinecmd productclass set <value>"
		  		"\n\t\tvalue:{1~63characters}\r\n"
				"\tprolinecmd productclass display\r\n"
				"\tprolinecmd manufacturerOUI set <value>"
		  		"\n\t\tvalue:{1~63characters,now only support max characters are 6!}\r\n"
				"\tprolinecmd manufacturerOUI display\r\n"
				"\tprolinecmd serialnum set <value>"
		  		"\n\t\tvalue:{1~63characters}\r\n"
				"\tprolinecmd serialnum display\r\n"
#if defined(TCSUPPORT_RESERVEAREA_EXTEND) || (defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505))
				"\tprolinecmd ssid set <value>"
		  		"\n\t\tvalue:{1~31characters}\r\n"
				"\tprolinecmd ssid display\r\n"
				"\tprolinecmd wpakey set <value>"
				"\n\t\tvalue:{1~63characters}\r\n"
				"\tprolinecmd wpakey display\r\n"
				"\tprolinecmd webpwd set <value>"
		  		"\n\t\tvalue:{1~63characters}\r\n"
				"\tprolinecmd webpwd display\r\n"
#if !(defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505))
				"\tprolinecmd pppusrname set <value>"
		  		"\n\t\tvalue:{1~63characters}\r\n"
				"\tprolinecmd pppusrname display\r\n"
				"\tprolinecmd ppppwd set <value>"
		  		"\n\t\tvalue:{1~63characters}\r\n"
				"\tprolinecmd ppppwd display\r\n"
				"\tprolinecmd cfeusrname set <value>"
		  		"\n\t\tvalue:{1~63characters}\r\n"
				"\tprolinecmd cfeusrname display\r\n"
				"\tprolinecmd cfepwd set <value>"
		  		"\n\t\tvalue:{1~63characters}\r\n"
				"\tprolinecmd cfepwd display\r\n"
				"\tprolinecmd xponsn display\r\n"
				"\tprolinecmd xponsn set <value>"
		  		"\n\t\tvalue:{1~31characters}\r\n"
				"\tprolinecmd xponpwd display\r\n"
				"\tprolinecmd xponpwd set <value>"
		  		"\n\t\tvalue:{1~31characters}\r\n"
				"\tprolinecmd macaddr set <value>"
				"\n\t\tvalue:{1~31characters}\r\n"
				"\tprolinecmd macddr display\r\n"
#endif
				"\tprolinecmd ssid2nd set <value>"
		  		"\n\t\tvalue:{1~31characters}\r\n"
				"\tprolinecmd ssid2nd display\r\n"
				"\tprolinecmd wpakey2nd set <value>"
				"\n\t\tvalue:{1~63characters}\r\n"
				"\tprolinecmd wpakey2nd display\r\n"
#endif
				"\tprolinecmd clear <value>"
				"\n\t\tvalue:{1:cwmp}\r\n"
				"\tprolinecmd help\r\n"
				"\tprolinecmd version display\r\n");
			
	return 0;
}
#if defined(TCSUPPORT_WAN_GPON) || defined(TCSUPPORT_WAN_EPON)
#ifndef TCSUPPORT_RESERVEAREA_EXTEND
 static int doPonMacNum(int argc, char *argv[], void *p)
{
	proline_cwmpPara cwmppara;
	proLineFlag plpara;
	char buf[5];
	int macNum = 0;

	showInfo(argc,argv,p);
	memset(&plpara,0,sizeof(proLineFlag));
	memset(&cwmppara,0,sizeof(proline_cwmpPara));
	
	setPLCwmpParaDefault(&plpara);
	plpara.flag = PL_PON_MACNUM;
	if((argc == 2)&&(!strcmp(argv[1] , "display") ))
	{
		getProLinePara(&plpara,&cwmppara);
		if(CWMPPARAMAGIC == cwmppara.magic)
		{
			printf("Pon. mac num:%s\r\n",cwmppara.ponMacNum);
		}
		else
		{
			printf("NotValue\r\n");
		}
	}
	else if((argc == 3)&&(!strcmp(argv[1] , "set") ))
	{
		macNum =atoi(argv[2]);
		if(macNum > 16 || macNum < 1)
			goto errorHandle;
		sprintf(buf, "%d", macNum);
		setProLinePara(&plpara,buf,strlen(buf));
	}
	else
	{
		goto errorHandle;
	}

	return SUCCESS;

errorHandle:
	printf("\r\nUsage:prolinecmd ponmacnum set <1~16>\n");
	return FAIL;
}
static int doPonUserAdminPasswd(int argc, char *argv[], void *p){
	proline_cwmpPara cwmppara;
	proLineFlag plpara;
	char buf[5];
	int macNum = 0;

	showInfo(argc,argv,p);
	memset(&plpara,0,sizeof(proLineFlag));
	memset(&cwmppara,0,sizeof(proline_cwmpPara));
	
	setPLCwmpParaDefault(&plpara);
	plpara.flag = PL_PON_USERADMIN_PASSWD;
	if((argc == 2)&&(!strcmp(argv[1] , "display") ))
	{
		getProLinePara(&plpara,&cwmppara);
		if(CWMPPARAMAGIC == cwmppara.magic)
		{
			printf("Pon password for UserAdmin: %s\r\n",cwmppara.ponUseradminPasswd);
		}
		else
		{
			printf("NotValue\r\n");
		}
	}
	else if((argc == 3)&&(!strcmp(argv[1] , "set") ))
	{
		if(strlen(argv[2]) > PONUSERADMINPASSWDLEN)
			goto errorHandle;
		setProLinePara(&plpara,argv[2],strlen(argv[2]));
	}
	else
	{
		goto errorHandle;
	}

	return SUCCESS;

errorHandle:
	printf("\r\nUsage:prolinecmd ponuseradminpasswd set <Password:Len 1~30>\n");
	return FAIL;
}
#endif

#if defined(TCSUPPORT_WAN_GPON)
#ifndef TCSUPPORT_RESERVEAREA_EXTEND
static int doGponSN(int argc, char *argv[], void *p)
{
	proline_cwmpPara cwmppara;
	proLineFlag plpara;
	char buf[5];
	int macNum = 0;

	showInfo(argc,argv,p);
	memset(&plpara,0,sizeof(proLineFlag));
	memset(&cwmppara,0,sizeof(proline_cwmpPara));
	
	setPLCwmpParaDefault(&plpara);
	plpara.flag = PL_GPON_SN;
	if((argc == 2)&&(!strcmp(argv[1] , "display") ))
	{
		getProLinePara(&plpara,&cwmppara);
        printf("GPON. sn:%s\r\n",cwmppara.gponSn);
	}
	else if((argc == 3)&&(!strcmp(argv[1] , "set") ))
	{
		if(strlen(argv[2]) > GPONSNLEN)
			goto errorHandle;
		setProLinePara(&plpara,argv[2],strlen(argv[2]));
	}
	else
	{
		goto errorHandle;
	}

	return SUCCESS;

errorHandle:
	printf("\r\nUsage:prolinecmd gponsn set <SN:FTTH00000011>\n");
	return FAIL;
}
 static int doGponPasswd(int argc, char *argv[], void *p)
 {
	 proline_cwmpPara cwmppara;
	 proLineFlag plpara;
 
	 showInfo(argc,argv,p);
	 memset(&plpara,0,sizeof(proLineFlag));
	 memset(&cwmppara,0,sizeof(proline_cwmpPara));
	 
	 setPLCwmpParaDefault(&plpara);
	 plpara.flag = PL_GPON_PASSWD;
	 if((argc == 2)&&(!strcmp(argv[1] , "display") ))
	 {
		 getProLinePara(&plpara,&cwmppara);
		 if(CWMPPARAMAGIC == cwmppara.magic)
		 {
			 printf("GPON. passwd:%s%s\r\n",cwmppara.gponPasswd,cwmppara.gponExtPwd);
		 }
		 else
		 {
			 printf("NotValue\r\n");
		 }
	 }
	 else if((argc == 3)&&(!strcmp(argv[1] , "set") ))
	 {
	 	 if(!strncmp(argv[2],"0x",2)||!strncmp(argv[2],"0X",2))
	 	 {
			 if(strlen(argv[2]) > GPONPASSWDLEN + GPONEXTENDPWDLEN -2)
				 goto errorHandle;
			 setProLinePara(&plpara,argv[2],strlen(argv[2]));
	 	 }
		 else
		 {
			if(strlen(argv[2]) > GPONPASSWDLEN + 1)
				goto errorHandle2;
			setProLinePara(&plpara,argv[2],strlen(argv[2]));
		 }
	 }
	 else
	 {
		 goto errorHandle;
	 }
 
	 return SUCCESS;
 
 errorHandle:
	 printf("\r\nUsage:prolinecmd gponpasswd set <passwd: max hex len %d>\n", GPONPASSWDLEN + GPONEXTENDPWDLEN -2);
	 return FAIL;
 errorHandle2:
	 printf("\r\nUsage:prolinecmd gponpasswd set <passwd: max char len %d>\n", GPONPASSWDLEN + 1);
	 return FAIL;
 }
#endif
#endif
#endif


 int main(int argc, char **argv) 
{
	void *p;
	int ret = -1;
	int pidfd;
	pidfd =open(PROLINECMD_SOCK_PATH,O_RDWR | O_CREAT);
	if(pidfd < 0)
	{
		printf("\r\nopen lock file error!");
		ret = subcmd(ProLineCmds, argc, argv, p);
	}
	else
	{
		writew_lock(pidfd,0,SEEK_SET,0);
		ret = subcmd(ProLineCmds, argc, argv, p);
		un_lock(pidfd,0,SEEK_SET,0);
		//close(pidfd);
	}
	return ret;
}


 #else
  int main(int argc, char **argv) 
 {
 	return 0;
 }
#endif


