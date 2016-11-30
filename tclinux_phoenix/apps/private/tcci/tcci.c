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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include "../../../version/tcversion.h"   
#include "../lib/libtcapi.h"
#include <fcntl.h>
#include "../../../tools/trx/trx.h"


#include "../../public/mtd/tc_partition.h"




#ifdef TCSUPPORT_C1_MS
#define FWVER_PATH	"/etc/fwver_c1.conf"
#endif
typedef int (*func_t)(int argc,char *argv[]);

static int doSysMac(int argc, char *argv[]);
static int doSysOnuType(int argc, char *argv[]);
static int dummy(int argc, char *argv[]);
#ifdef TC2031_DEBUG
static int tcipaddr(int argc, char *argv[]);
static int tcsetip(int argc, char *argv[]);
static int tcping(int argc, char *argv[]);
#endif
#if defined(TCSUPPORT_LED_BTN_CHECK) || defined(TCSUPPORT_TEST_LED_ALL) 
#if defined(TCSUPPORT_LED_CHECK) || defined(TCSUPPORT_TEST_LED_ALL) 
static int 	doLedCheck(int argc, char *argv[]);
#endif
#if defined(TCSUPPORT_BTN_CHECK)
static int 	doButtonCheck(int argc, char *argv[]);
#endif
#endif

#if defined(TCSUPPORT_CMDPROMPT)
static int 
doSysSubcmdsPrompt(int argc, char *argv[]);
static int 
doWan2lanPrompt(int argc, char *argv[]);
#endif



#ifdef TCSUPPORT_PON_TEST
static int doPonTempTest(int argc, char *argv[]);
static int doSysAtbp(int argc, char *argv[]);
static int doSysPonLinkStatus(int argc, char *argv[]);
#endif


static int doImageInfo(int argc,char* argv[]);
static int doVoipTest(int argc, char *argv[]);
static int doBindShowTask(int argc, char *argv[]);
static int bindTask(void);
static int showTask(void);
#define VOIPTASKNUM 5
#define CPUXTASKNUM 3
char *voip_task_array[VOIPTASKNUM] = {
"fxs_task",
"ORTP_TASK",
"DspDlTask",
"DspUlTask",
"DSPProc"
};
char *cpux_task_array[CPUXTASKNUM] = {
"ksoftirqd",
"kworker",
"migration"
};
static int doSysBobCarlibration(int argc, char *argv[]);

#if defined(TCSUPPORT_CT_UPG_PINGPONG)
static int doSysBootFlag(int argc, char *argv[]);
#endif


func_t ci_func[] ={
    dummy,
    doSysMac,
#ifdef TC2031_DEBUG
    tcipaddr,
    tcsetip,
    tcping,
#else/* To ensure index */
    NULL,
    NULL,
    NULL,
#endif    
#if !defined(TCSUPPORT_C1_NEW_GUI) && !defined(TCSUPPORT_PON_TEST) 
    NULL,
#endif
#if !defined(TCSUPPORT_BOOTLOADER_MODIFY_PVNAME) 
	NULL,
	NULL,
	NULL,
#endif
#if defined(TCSUPPORT_LED_BTN_CHECK) || defined(TCSUPPORT_TEST_LED_ALL) 
#if defined(TCSUPPORT_LED_CHECK) || defined(TCSUPPORT_TEST_LED_ALL) 
	doLedCheck,
#else
	NULL,
#endif
#if defined(TCSUPPORT_BTN_CHECK)
	doButtonCheck,
#endif
#else
	NULL,
	NULL,
#endif
#if !defined(TCSUPPORT_C2_TRUE) 
	NULL,
#endif
#if defined(TCSUPPORT_CMDPROMPT)
	doSysSubcmdsPrompt,
	doWan2lanPrompt,
#else
	NULL,
	NULL,
#endif
#if !defined(TCSUPPORT_C1_OBM) && !defined(TCSUPPORT_PON_TEST) 
	NULL,
#endif
#if !defined(USB_AUTOMOUNT) || !defined(TCSUPPORT_CZ_GENERAL) 
	NULL,
#endif
    doSysOnuType,
#ifdef TCSUPPORT_PON_TEST
    doPonTempTest,
    doSysAtbp,
    doSysPonLinkStatus,
#else
	NULL,
	NULL,
	NULL,
#endif
#if !defined(TCSUPPORT_OI_C9) && !defined(TCSUPPORT_OI_C7) && !defined(TCSUPPORT_CDS) 
	NULL,
#endif
	doImageInfo,
	doVoipTest,
	doSysBobCarlibration,
#if defined(TCSUPPORT_CT_UPG_PINGPONG)
	doSysBootFlag,
#else
	NULL,
#endif
	doBindShowTask,
#if !defined(TCSUPPORT_WAN_UPSTREAM_REMARK) 
	NULL,
#endif
    NULL  
};

static int dummy(int argc, char *argv[]){
    printf("I am dummy!!\n");
    return 0;
}



#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)
int prevSipStatus_tcci=0;

int getSipStatus_tcci()
{
	char tmp[32]={0};
	
	tcapi_get("VoIPBasic_Common", "SIPProtocol", tmp);
	if(strcmp(tmp,"H.248") == 0)
		prevSipStatus_tcci = 1;
	else 
		prevSipStatus_tcci = 0;
	//tcdbg_printf("func[getSipStatus_tcci]VoIPBasic_Common SIPProtocol is %s\r\n", tmp);
	//tcdbg_printf("func[getSipStatus_tcci]voip_basic_boot: prevSipStatus is %d\r\n", prevSipStatus_tcci);
	return prevSipStatus_tcci;

}
#endif



#ifdef TC2031_DEBUG
/*_____________________________________________________________________________
**      function name: tcipaddr
**      descriptions:
**           show lan ip addr 
**             
**      parameters:
**            argc : argument number
**            argv : argument point 
**		 p     :  no use
**      global:
**            None
**             
**      return:
**            none
**	     
**      call:
**      	
**      revision:
**      
**____________________________________________________________________________
*/
static int tcipaddr(int argc, char *argv[]){
	
	FILE *fp;
	char buf[256];
	char *point;
	char x=0;
	
	system("ifconfig br0 > /tmp/var/ifconfig.tmp");
	fp = fopen("/tmp/var/ifconfig.tmp","r");
	if(fp != NULL){
		while(fgets(buf,256,fp)){
			point=strstr(buf,"inet addr:");
			if(point != NULL){
				printf("%s(set)\n",strtok(point+strlen("inet addr:")," "));
				break;
			}else{
				continue;
			}
			x++;
			if(x>10){
				break;
			}
		}
		fclose(fp);
		unlink("/tmp/var/ifconfig.tmp");
	}else{
		printf("Get ip addr fail!!\n");
	}

	return 0;
}

/*_____________________________________________________________________________
**      function name: tcsetip
**      descriptions:
**           set lan ip addr 
**             
**      parameters:
**            argc : argument number
**            argv : argument point 
**		 p     :  no use
**      global:
**            None
**             
**      return:
**            none
**	     
**      call:
**      	
**      revision:
**      
**____________________________________________________________________________
*/
static int tcsetip(int argc, char *argv[]){
	char buf[256];

	if(argc > 4){
		/*
		sprintf(buf,"ifconfig br0 %s netmask %s",argv[3],argv[4]);
		*/
		sprintf(buf,"ifconfig br0 %s netmask %s;tcapi set Lan_Entry0 IP %s;tcapi save;", \
				argv[3], argv[4],argv[3]);
	}else{
		sprintf(buf,"ifconfig br0 %s ",argv[3]);
	}

	system(buf);
	return 0;
}
/*_____________________________________________________________________________
**      function name: tcping
**      descriptions:
**           do ping action
**             
**      parameters:
**            argc : argument number
**            argv : argument point 
**		 p     :  no use
**      global:
**            None
**             
**      return:
**            none
**	     
**      call:
**      	
**      revision:
**      
**____________________________________________________________________________
*/
static int tcping(int argc, char *argv[]){
	char buf[256];
	int interval;
	
	if(argc >= 5){	
		interval = atoi(argv[4]);
		if(interval < 0){
			interval = 0;
		}
	}else{
		interval = 0;
	}
	
	sprintf(buf,"/userfs/bin/sendicmp %s %s %s %d",argv[1],argv[2],argv[3],interval);
	system(buf);
	return 0;
}

#endif

/*
#if defined(TCSUPPORT_FON)
int hextoInt(char ch)
{
	int ret = 0;
	if (ch >= '0' && ch <= '9')
		ret = ch - '0';
	if (ch >= 'a' && ch <= 'f')
		ret = 10 + ch - 'a';
	if (ch >= 'A'  && ch <= 'F')
		ret = 10 + ch - 'A';
	return ret;
}

char intToHex(int i)
{
	char ret = 0;
	if(i>=0 && i <=9)
		ret = i + '0';
	if(i>=10 && i <=15)
		ret = i+'a'-10;
	return ret;
}
#endif
*/

/**
 * is_zero_ether_addr - Determine if give Ethernet address is all zeros.
 * @addr: Pointer to a six-byte array containing the Ethernet address
 *
 * Return true if the address is all zeroes.
 */
static int is_zero_ether_addr(const unsigned char *addr)
{
	return !(addr[0] | addr[1] | addr[2] | addr[3] | addr[4] | addr[5]);
}

/**
 * is_multicast_ether_addr - Determine if the Ethernet address is a multicast.
 * @addr: Pointer to a six-byte array containing the Ethernet address
 *
 * Return true if the address is a multicast address.
 * By definition the broadcast address is also a multicast address.
 */
static int is_multicast_ether_addr(const unsigned char *addr)
{
	return (0x01 & addr[0]);
}

/**
 * is_broadcast_ether_addr - Determine if the Ethernet address is broadcast
 * @addr: Pointer to a six-byte array containing the Ethernet address
 *
 * Return true if the address is the broadcast address.
 */
static int is_broadcast_ether_addr(const unsigned char *addr)
{
	return (addr[0] & addr[1] & addr[2] & addr[3] & addr[4] & addr[5]) == 0xff;
}

/**
 * is_valid_ether_addr - Determine if the given Ethernet address is valid
 * @addr: Pointer to a six-byte array containing the Ethernet address
 *
 * Check that the Ethernet address (MAC) is not 00:00:00:00:00:00, is not
 * a multicast address, and is not FF:FF:FF:FF:FF:FF.
 *
 * Return true if the address is valid.
 */
static int is_valid_ether_addr(const unsigned char *addr)
{
	/* FF:FF:FF:FF:FF:FF is a multicast address so we don't need to
	 * explicitly check for it here. */
	return !is_multicast_ether_addr(addr) && !is_zero_ether_addr(addr) && !is_broadcast_ether_addr(addr);
}


static int doSysMac(int argc, char *argv[]){
    unsigned char len, tmp[3], i;
    unsigned char mac_addr[6] = {0};
    FILE *fp; 
#if defined(TCSUPPORT_WAN_EPON) || defined(TCSUPPORT_WAN_GPON) || defined(TCSUPPORT_WAN_PTM) || (defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505))
    int rebootFlag = 1;
#endif

/*	
#if defined(TCSUPPORT_FON)
    unsigned char fon_tmp[80],syscmd[128],j;
    unsigned int seed = 1;
#endif
*/

//    printf("MAC address=%s\n", argv[1]);    
    /* check MAC address is valid or not */
    len = strlen(argv[1]);
	if (len != 12) {
		printf("mac address must be 12 digits\n");
		return -1;
	}
	#ifdef TCSUPPORT_INIC_HOST
		/* in iNIC mode, the iNIC host mac address must be even,
		 * so we check the last digit that must be 0x30, 0x32, ... 0x38.
		 */
		if ((argv[1][11] & 0x01) != 0) {
			printf("In FTTdp, Host MAC address must be even\n");
			return -1;
		}
	#endif
#if defined(TCSUPPORT_WAN_EPON) || defined(TCSUPPORT_WAN_GPON) || defined(TCSUPPORT_WAN_PTM) || (defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505))
	if (argc >= 3){
		if (strncmp(argv[2], "-n", 2) == 0)
			rebootFlag = 0;
	}
#endif

/*	   
#if defined(TCSUPPORT_FON)
	memset(fon_tmp,0,sizeof(fon_tmp));
	memset(syscmd,0,sizeof(syscmd));
	for(i=1,j=0;i<18;i++)
	{
		if (i%3==0)
		{
			fon_tmp[i-1] = '-';
		}
		else
		{
			fon_tmp[i-1] = argv[1][j];
			j++;
		}
	}
	//printf("\r\nfon_mac=%s",fon_tmp);
	sprintf(syscmd,"/usr/bin/prolinecmd fonMac set %s",fon_tmp);
	system(syscmd);

	memset(syscmd,0,sizeof(syscmd));
	memset(fon_tmp,0,sizeof(fon_tmp));
	seed = hextoInt(argv[1][8])*4096+hextoInt(argv[1][9])*256+hextoInt(argv[1][10])*16+hextoInt(argv[1][11]);
	seed += (unsigned) time(NULL) + getpid();
	srand(seed);
	for(i=0;i<64; i++)
	{
		fon_tmp[i] = intToHex(rand()%16);
	}
	//printf("\r\nfon_keyword=%s",fon_tmp);
	sprintf(syscmd,"/usr/bin/prolinecmd fonKeyword set %s",fon_tmp);
	system(syscmd);
#endif
*/    
    /* Modify the MAC address in the bootloader */  
	tmp[2] = 0;
    for(i = 0; i < 6; i++){
		tmp[0] = argv[1][2*i];
		tmp[1] = argv[1][2*i+1];
		mac_addr[i] = (unsigned char)strtoul(tmp, NULL, 16);
	}
	
	printf("new mac addr = ");
    for(i = 0; i < 5; i++)    
        printf("%02x:", mac_addr[i]);
    printf("%02x\n", mac_addr[5]);
	
	if(!is_valid_ether_addr(mac_addr)){
		printf("MAC addr is not valid!!!!!\n");
		return 0;
	}
    /* Read bootloader from flash */
    system("cat /dev/mtd0 > /tmp/boot.bin");	
    
    fp = fopen("/tmp/boot.bin", "r+b");
    if( fp == NULL ){
        printf("open file error!!\n");
        system("rm /tmp/boot.bin");        
        return -1;
    }
    
    fseek(fp, 0x0000ff48L, SEEK_SET);
    fwrite(mac_addr, 6, 1, fp );
    fflush(fp);
    fclose(fp);
        
    /* Write the bootloader back to flash and reboot*/
#if defined(TCSUPPORT_WAN_EPON) || defined(TCSUPPORT_WAN_GPON) || defined(TCSUPPORT_WAN_PTM) || (defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505))
	if (rebootFlag == 0){
		system("/userfs/bin/mtd -f write /tmp/boot.bin bootloader");
	}else
#endif
    system("/userfs/bin/mtd -rf write /tmp/boot.bin bootloader");
   
    return 0;
}
#if defined(TCSUPPORT_LED_BTN_CHECK) || defined(TCSUPPORT_TEST_LED_ALL) 
#if defined(TCSUPPORT_LED_CHECK) || defined(TCSUPPORT_TEST_LED_ALL) 
static int doLedCheck(int argc, char *argv[])
{
	if((argc == 2) && ((strcmp(argv[1], "on") == 0) || (strcmp(argv[1], "off") == 0))){
		if(strcmp(argv[1], "off") == 0){
			printf("All led is turned off! \r\n");	
		 }	
		else{
			printf("All led is turned on! \r\n");
		}
	}
	else{
		printf("Usage: sys led [on||off] \r\n");
		return -1;
	}
	return 0;
}	
#endif
#if defined(TCSUPPORT_BTN_CHECK)
static int doButtonCheck(int argc, char *argv[])
{
	if(argc != 2 || (strcmp(argv[1], "enable") && strcmp(argv[1], "disable"))){
		printf("Usage: sys button [enable||disable]\r\n");
		return -1;
	}
	if (strcmp(argv[1], "disable") == 0){
		printf("All buttons are disabled! \r\n");
	}	
	else{
		printf("All buttons are enabled! \r\n");	
	 }

	return 0;
}	
#endif
#endif

#if defined(TCSUPPORT_CMDPROMPT)
static int doSysSubcmdsPrompt(int argc, char *argv[]){
	system("cat proc/tc3162/sys_subcmds");
	return 0;
}

static int doWan2lanPrompt(int argc, char *argv[]){
	system("cat proc/tc3162/sys_wan2lan");
	return 0;
}
#endif




/**************************************************
ENUM_SYSTEM_ONU_TYPE_SFU				=1
ENUM_SYSTEM_ONU_TYPE_HGU				=2
store onu type to flash bfc0ff9c
***************************************************/
static int doSysOnuType(int argc, char *argv[]){
    unsigned char type;
    FILE *fp; 

	if(NULL == argv[1]){
		printf("Bad command!\nUsage:sys onutype sfu|hgu \n");
		return -1;
	}

	if(!strcmp(argv[1],"sfu"))	{
		type = 1;//sfu
	}else if(!strcmp(argv[1],"hgu"))	{
		type = 2;//hgu
	}else{
		printf("Bad command!\nUsage:sys onutype sfu|hgu \n");
		return -1;
	}
		
    /* Read bootloader from flash */
    system("cat /dev/mtd0 > /tmp/boot.bin");
    
 
    fp = fopen("/tmp/boot.bin", "r+b");
    if( fp == NULL ){
        printf("open file error!!\n");
        system("rm /tmp/boot.bin");        
        return -1;
    }
    
    fseek(fp, 0x0000ff9cL, SEEK_SET);
    fwrite(&type, sizeof(type), 1, fp );
    fflush(fp);
    fclose(fp);
        
    /* Write the bootloader back to flash and reboot*/
    system("/userfs/bin/mtd -rf write /tmp/boot.bin bootloader");
   
    return 0;
}


#ifdef TCSUPPORT_PON_TEST
static int doPonTempTest(int argc, char *argv[])
{
	char cmd[256], buf[30], tmpMask[20] = {0};
	unsigned int testmask = 0;
	
	if (argc < 5) goto Usage;
	
	if(!strcmp(argv[1],"gpon"))	{
		if (!strcmp(argv[2], "sn")){
			sprintf(cmd, "echo sn %s %s > /proc/gpon/debug", argv[3], argv[4]);
			system(cmd);
		}
	}
#ifdef TCSUPPORT_WAN_EPON
	else if(!strcmp(argv[1],"epon")){
		if (!strcmp(argv[2], "tmpmac")){
			sprintf(cmd, "echo epon tmpmac %s %s > /proc/gpon/debug", argv[3], argv[4]);

#ifdef TCSUPPORT_EPON_OAM
			tcapi_get("Info_Oam", "testmask", buf);
			testmask = atoi(buf);
			if (1==atoi(argv[3]))
				testmask |= 1; // TESTMASK_TEMPMAC
			else if (0 == atoi(argv[3])){
				testmask &= ~1; // TESTMASK_TEMPMAC
			}else
				return -1;

			/*SET tmpmac & testmask to restart epon_oam*/
			sprintf(tmpMask, "%u", testmask);
			if (0!=tcapi_set("Info_Oam", "tmpmac", argv[4]) ||
				0!=tcapi_set("Info_Oam", "testmask", tmpMask))
				return -1;
			system("eponcmd oam restart");
#endif		
			system(cmd);
		}
#ifdef TCSUPPORT_EPON_OAM_CTC		
		else if (!strcmp(argv[2], "tmploid")){
#ifdef TCSUPPORT_EPON_OAM
			tcapi_get("Info_Oam", "testmask", buf);
			testmask = atoi(buf);
			if (1==atoi(argv[3]))
				testmask |= 2; // TESTMASK_TEMPLOID
			else if (0 == atoi(argv[3])){
				testmask &= ~2; // TESTMASK_TEMPLOID
			}else
				return -1;

			/*SET tmpmac & testmask to restart epon_oam*/
			sprintf(tmpMask, "%u", testmask);
			if (0!=tcapi_set("Info_Oam", "LOID0", argv[4]) ||
				0!=tcapi_set("Info_Oam", "testmask", tmpMask))
				return -1;
			system("eponcmd oam restart");
/*			if (0==tcapi_get("XPON_LinkCfg", "LinkSta", buf) && 2==atoi(buf))
				system("epon mpcp wanreset");
*/
#endif		
		}
#endif		
	}
#endif
	else{
		goto Usage;
	}

	system("/userfs/bin/ponmgr system set startup stop");
	system("/userfs/bin/ponmgr system set startup start");
	return 0;
Usage:
	printf("Usage:\n  sys pontest gpon sn xxxx xxxxxxxx\n");
#ifdef TCSUPPORT_WAN_EPON
	printf("  sys pontest epon tmpmac 0/1 xx:xx:xx:xx:xx\n");
#ifdef TCSUPPORT_EPON_OAM_CTC		
	printf("  sys pontest epon tmploid 0/1 xxxxxxxxxx\n");
#endif	
#endif	
	return -1;
}

static int doSysAtbp(int argc, char *argv[]){
	char buf[128];
	char info[128];

	if (0 == tcapi_get("Info_Ether", "mac", buf)){
		printf("macAddr        : %s\n", buf);
	}
#ifdef TCSUPPORT_PRODUCTIONLINE
	if (0== tcapi_get("Info_proLineInfo", "SerialNum", buf)){
		printf("SerialNum      : %s\n", buf);
	}
#if defined(TCSUPPORT_WAN_GPON)||defined(TCSUPPORT_WAN_EPON)	
	if (0==tcapi_get("Info_proLineInfo", "PonMacNum", buf)){
		printf("macNum         : %s\n", buf);
	}
	if (0==tcapi_get("Info_proLineInfo", "PonUserAdminPasswd", buf)){
		printf("UserAdminPasswd: %s\n", buf);
	}
#if defined(TCSUPPORT_WAN_GPON)
	if (0==tcapi_get("Info_proLineInfo", "GponSn", buf)){
		printf("GponSn         : %s\n", buf);
	}
	if (0==tcapi_get("Info_proLineInfo", "GponPasswd", buf)){
		printf("GponPassword   : %s\n", buf);
	}
#endif	
#endif	
#endif
	return 0;
}

static int doSysPonLinkStatus(int argc, char *argv[]){
	char buf[128];
	char info[128];
	char phyStatus[32]={0}, trafficStatus[20] = {0};
	int curLinkMode = 0, phyMode=0, trafficUp=0;
	
#ifdef TCSUPPORT_PONMGR
	if (0 == tcapi_get("XPON_LinkCfg", "LinkSta", buf)){
		curLinkMode = atoi(buf);
		if (curLinkMode == 0) // no link 
			goto PONDOWN;
	}else{
		goto PONDOWN;
	}
	if (0==tcapi_get("XPON_Common", "phyStatus", phyStatus)){
		if (!strcmp(phyStatus, "gpon_phy_up")){
			phyMode = 1; // gpon mode
		}else if (!strcmp(phyStatus, "epon_phy_up")){
			phyMode = 2; // epon mode
		}else{
			goto PONDOWN;
		}
	}else{
		goto PONDOWN;
	}

	if (0==tcapi_get("XPON_Common", "trafficStatus", trafficStatus)){
		if (!strcmp(trafficStatus, "up")){
			trafficUp = 1; // traffic up
		}
	}else{
		goto PONDOWN;
	}

	if (curLinkMode == 1 && phyMode == 1){
		if (trafficUp == 1){
			printf("Gpon link up!!\n");
		}else{
			printf("Gpon is linking ...\n");
		}
		return 0;
	}else if (curLinkMode == 2 && phyMode == 2){
		if (trafficUp == 1){
			printf("Epon link up!!\n");
		}else{
			printf("Epon is linking ...\n");
		}
		return 0;
	}
#endif
PONDOWN:
	printf("Pon: Current link down!!\n");
	return 0;
}

#endif

#define TCLINUX "tclinux"
#define TCLINUX_SLAVE "tclinux_slave"
int mtd_open(const char *mtd, int flags)
{
	FILE *fp;
	char dev[256];
	int i;

	if ((fp = fopen("/proc/mtd", "r"))) {
		while (fgets(dev, sizeof(dev), fp)) {
			if (sscanf(dev, "mtd%d:", &i) && strstr(dev, mtd)) {
				snprintf(dev, sizeof(dev), "/dev/mtd%d", i);
				fclose(fp);
				return open(dev, flags);
			}
		}
		fclose(fp);
	}

	return open(mtd, flags);
}

int read_mtd_info(const char *mtd,struct trx_header *p_trx_header)
{
	int mtd_fd = -1;
	int trx_header_len = sizeof(struct trx_header);
	
	if(-1 == (mtd_fd = mtd_open(mtd,O_RDONLY)))
	{
		printf("ERROR!Open mtd %s fail",mtd);
		return -1;
	}

	memset(p_trx_header,0,trx_header_len);
	
	if(trx_header_len != read(mtd_fd,p_trx_header,trx_header_len))
	{
		printf("ERROR!Read mtd %s fail",mtd);
		close(mtd_fd);
		return -1;
	}

	close(mtd_fd);
	return 0;
}

int doImageInfo(int argc,char* argv[])
{
	struct trx_header trx_header_buff;

	if(0 != read_mtd_info(TCLINUX,&trx_header_buff))
		return -1;
	
	printf("os1:%s",trx_header_buff.version);

	if(0 != read_mtd_info(TCLINUX_SLAVE,&trx_header_buff))
		return -1;

	printf("os2:%s",trx_header_buff.version);

	return 0;
}
#if defined(TCSUPPORT_CT_PON_C9)
typedef struct country_to_hwprof_s{
	char         country[24];
}country_to_hwprof_t;

static country_to_hwprof_t gCountryToHwPofTbl[] = {
	{"BEL-BELIGIUM"},
	{"BRA-BRAZIL"},
	{"CHL-CHILE"},
	{"DNK-DENMARK"},
	{"ETS-ETSI"},
	{"ESP-SPAIN"},
	{"FIN-FINLAND"},
	{"FRA-FRANCE"},
	{"DEU-GERMANY"},
	{"HUN-HUNGARY"},
	{"ITA-ITALY"},
	{"NLD-NETHERLANDS"},
	{"SWE-SWEDEN"},
	{"CHE-SWITZERLAND"},
	{"GBR-UK"},
	{"NOR-NORWAY"},
	{"CZE-CZECH"},
	{"JPN-JAPAN"},
	{"CHN-CHINA"},
	{"NEL-NEWZEALAND"},
	{"AUS-AUSTRAILIA"},
	{"IND-INDIA"},
	{"USA-NORTHAMERICA"},
	{"T57-TR57"},
	{"TWN-TAIWAN"},
	{""},
};

static int doVoipTest(int argc, char *argv[]){
	char testFlag[16]={0};
	int countryId=0;
	int i=0;
	int maxCountryNum=sizeof(gCountryToHwPofTbl)/sizeof(country_to_hwprof_t)-1;
	char nodeName[32]={0};

#if defined(TCSUPPORT_CT_VOIP_SIP) || defined(TCSUPPORT_ECN_MEGACO)
	if(getSipStatus_tcci()==0){
		strcpy(nodeName,"InfoVoIP_Common");
	}
	else if(getSipStatus_tcci()==1){
		strcpy(nodeName,"InfoVoIPH248_Common");
	}
#endif

	if(2 == argc)
	{
		if((strcmp(argv[1],"on")== 0) || (strcmp(argv[1],"off")==0))
			tcapi_set(nodeName, "c9teston",argv[1]);
		else
			goto usage;
		
			
	}else if(3 == argc){
		if(strcmp(argv[1],"setcountry")==0){
			tcapi_get(nodeName, "c9teston",testFlag);
			if(strcmp(testFlag,"on")!=0){
				printf("please set c9voiptest on first\n");
				goto usage;
			}
			
			sscanf(argv[2],"%d",&countryId);
			if(countryId < maxCountryNum)
				tcapi_set("VoIPAdvanced_Common", "VoIPRegion",gCountryToHwPofTbl[countryId].country);
			else 
				tcapi_set("VoIPAdvanced_Common", "VoIPRegion","CHN-CHINA");
			tcapi_commit("VoIPAdvanced");
#if defined(TCSUPPORT_ECN_MEGACO)
			tcapi_set(nodeName,"countryset","on");
#endif
			
		}else{
			goto usage;
		}
		
	}else{
		goto usage;
	}
	return 0;
usage:
		printf("usage: c9voiptest [on|off|setcountry countryid]\n");
		printf("countryid:\n");
		for(i=0;i<maxCountryNum;i++){
			printf("%2d  : %s\n",i, gCountryToHwPofTbl[i].country);
		}
	return 0;
}
#else
static void DisMacLearning(void)
{
	char tmp[80] = {0};

	sprintf(tmp,"sys memwl bfb5a00c 0xfff10");
	system(tmp);

	memset(tmp,0,80);
	sprintf(tmp,"sys memwl bfb5a10c 0xfff10");
	system(tmp);

	memset(tmp,0,80);
	sprintf(tmp,"sys memwl bfb5a20c 0xfff10");
	system(tmp);

	memset(tmp,0,80);
	sprintf(tmp,"sys memwl bfb5a30c 0xfff10");
	system(tmp);

	memset(tmp,0,80);
	sprintf(tmp,"sys memwl bfb5a40c 0xfff10");
	system(tmp);

	memset(tmp,0,80);
	sprintf(tmp,"sys memwl bfb5a50c 0xfff10");
	system(tmp);

	memset(tmp,0,80);
	sprintf(tmp,"sys memwl bfb5a60c 0xfff10");
	system(tmp);

}

static int doVoipTest(int argc, char *argv[]){
	char tmp[160] = {0};
	if(argc != 2){
		printf("Usage: voiptest [on||tester] \r\n");
		return -1;
	}
		
	if((strcmp(argv[1],"on")==0) || (strcmp(argv[1],"tester")==0))
	{
		DisMacLearning();
		sprintf(tmp,"ethphxcmd arl mactbl-clr");
		system(tmp);		
		memset(tmp,0,160);
		if(strcmp(argv[1],"on")==0)
		{
			sprintf(tmp,"ifconfig br0 192.168.1.2");
		}
		else
		{
			sprintf(tmp,"ifconfig br0 192.168.1.1");
		}
		system(tmp);
		memset(tmp,0,160);
		sprintf(tmp,"rmmod loopdetect");
		system(tmp);
		memset(tmp,0,160);
		
#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)
			if(getSipStatus_tcci()==0){
				sprintf(tmp,"killall -9 sipclient");	
				system(tmp);
			}
			else if(getSipStatus_tcci()==1){
				sprintf(tmp,"killall -9 mgapp");
				system(tmp);
				memset(tmp,0,160);
				sprintf(tmp,"killall -9 tcVoIPApiServer");
				system(tmp);	
			}
#endif
		memset(tmp,0,160);
		sprintf(tmp,"evcom %s &",argv[1]);
		system(tmp);
	}
	else
	{
		printf("Usage: voiptest [on | tester] \r\n");
		return -1;
	}
	return 0;

}
#endif

static int doBindShowTask(int argc, char *argv[])
{
    if(argc != 2){
        printf("Usage: bindshowtask [bind|show] \r\n");
        return -1;
    }		
    if(strcmp(argv[1],"bind")==0)
    {
        printf("start binding all tasks...\n");
        system("ps >/tmp/task.txt");
        bindTask();
    }else if(strcmp(argv[1],"show")==0){
        printf("Tasks bind to CPU:\n");
        system("ps >/tmp/task.txt");
        showTask();
    }else
    {
        printf("Usage: bindshowtask [bind|show] \r\n");
        return -1;
    }
    return 0;	
}

/*bind voip kernel task to CPU3
   bind no voip kernel task to CPU0/CPU1/CPU2
   bind app task to CPU1
*/
static int bindTask(void)
{
    FILE *fp = NULL;
    char *str = NULL;
    char pid_s[10] = {0};
    char *p = NULL;
    int i=0;
    int j=0;
    int flag_tasktype = 0;/*0:init|1:voip_task|2:cpux_task*/
    char tmp[160] = {0};

    fp = fopen("/tmp/task.txt", "r");
    if(fp==NULL){
        printf("open file error!!\n");
        return -1;
    }
    str = malloc(512*sizeof(char));
    while(!feof(fp))
    {
        fgets(str,512,fp);
        //printf("%s",str);
        p=str;
        while(*p==' ')p++;
        if((*p<'0')||(*p>'9'))
        {
            continue;
        }
	/*get pid*/
        while(*p!=' ')
        {
            pid_s[i]=*p;
            p++;
            i++;
        }
        pid_s[i]='\0';
        if(strstr(str,"["))
        {
            for(j=0;j<VOIPTASKNUM;j++)
            {
                if(strstr(str,voip_task_array[j])){
                    flag_tasktype = 1;
                    break;
                }
            }
            for(j=0;j<CPUXTASKNUM;j++)
            {
                if(strstr(str,cpux_task_array[j])){
                    flag_tasktype = 2;
                    break;
                }
            }
            if(flag_tasktype == 1){
                flag_tasktype = 0;
                memset(tmp,0,160);
                sprintf(tmp,"taskset -p 0x8 %s > /dev/null",pid_s);
                system(tmp);
            }else if(flag_tasktype == 2){
                flag_tasktype = 0;
                p=str;
                while((*p)!='/')p++;
                switch(*(++p))
                {
                    case '0':
                        memset(tmp,0,160);
                        sprintf(tmp,"taskset -p 0x1 %s > /dev/null",pid_s);
                        system(tmp);
                        break;
                    case '1':
                        memset(tmp,0,160);
                        sprintf(tmp,"taskset -p 0x2 %s > /dev/null",pid_s);
                        system(tmp);
                        break;					
                    case '2':
                        memset(tmp,0,160);
                        sprintf(tmp,"taskset -p 0x4 %s > /dev/null",pid_s);
                        system(tmp);
                        break;
                    case '3':
                        memset(tmp,0,160);
                        sprintf(tmp,"taskset -p 0x8 %s > /dev/null",pid_s);
                        system(tmp);
                        break;
                    case 'u':
                        break;
                    default:break;						
                }
            }else if(!strstr(str,"<")){
                memset(tmp,0,160);
                sprintf(tmp,"taskset -p 0x7 %s > /dev/null",pid_s);
                system(tmp);
            }
        }else{
            if(!strstr(str,"ps")){
                memset(tmp,0,160);
                sprintf(tmp,"taskset -p 0x2 %s > /dev/null",pid_s);
                system(tmp);
            }
        }
        i=0;
        j=0;	
    }
    free(str);
    fclose(fp);	
    printf("finish binding all tasks\n");
    return 0;
}

static int showTask(void)
{
    FILE *fp = NULL;
    FILE *fp_mask = NULL;
    char *str = NULL;
    char pid_s[10] = {0};
    char *p = NULL;
    char tmp[160] = {0};
    int i = 0;
    int pid = 0;
    int mask = 0;
	
    fp = fopen("/tmp/task.txt", "r");
    if(fp==NULL){
        printf("open file error!!\n");
        return -1;
    }
    printf("MASK   PID  Uid     VmSize Stat Command\n");    
    str = malloc(512*sizeof(char));
    while(!feof(fp))
    {
        fgets(str,512,fp);
        p=str;
        while(*p == ' ')p++;
        if((*p<'0')||(*p>'9')||(strstr(str,"ps")))
        {
            continue;
        }
        while(*p!=' ')
        {
            pid_s[i]=*p;
            p++;
            i++;
        }
        pid_s[i]='\0';
        memset(tmp,0,160);
        sprintf(tmp,"taskset -p %s > tmp/mask.txt",pid_s);
        system(tmp);
        fp_mask= fopen("/tmp/mask.txt", "r");
        if(fp_mask==NULL){
            printf("open file error!!\n");
            free(str);
            fclose(fp);
            return -1;
        }
        fscanf(fp_mask,"pid %d's current affinity mask: %x",&pid,&mask);
        fclose(fp_mask);
        printf("%x  %s",mask,str);
        i=0;
    }    
    free(str);
    fclose(fp);
    return 0;
}

#if !defined(TCSUPPORT_BOSA_DATA_TO_FLASH) 
static int doSysBobCarlibration(int argc, char *argv[]){
    FILE *fp = NULL; 
	char buf[128] = {0};
	char *pStart = NULL;
	char strBuf[8]={0};
	int pageSize=0;

	if(argc>=2){
		printf("Usage:\nwrite bob data to flash from UX: sys bobcarlibration\n");
		return -1;
	}else if(argc==1){	
#ifdef TCSUPPORT_CT
#if (TCSUPPORT_RESERVEAREA_BLOCK >=5)
		printf("start to do BOB carlibration, please wait!\n");
		memset(buf, 0x00, sizeof(buf));
		sprintf(buf, "/userfs/bin/mtd -f writeflash /tmp/7570_bob.conf %lu %lu reservearea", 
			(unsigned long)BOB_RA_SIZE, (unsigned long)BOB_RA_OFFSET);
		printf(buf);
		system(buf);
#endif
#else	
		printf("Carlibrate area not support!\n");
#endif		
	}
}

#endif

#if defined(TCSUPPORT_CT_UPG_PINGPONG)
static const unsigned long crc_32_tab[] = { /* CRC polynomial 0xedb88320 */
0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

#define UPDC32(octet,crc) (crc_32_tab[((crc) ^ (octet)) & 0xff] ^ ((crc) >> 8))

unsigned long calculate_crc32(int imagefd, long len)
{
	long length=len;	
	int check_len=0, i=0;
	char flag=0;/*for stop loop*/
	unsigned char *buf;
	unsigned long crc;
#define CHECKSUM_TEMP_BUF_SIZE 4096

      	crc = 0xFFFFFFFF;
	
	/*Because read 1 byte at a time will spent much time, 
		we read a bigger size at a time and use this space to 
		do checksum. */
   	buf = (unsigned char *)malloc(CHECKSUM_TEMP_BUF_SIZE);

 	while(flag == 0){
		/*decide add length*/
		if(length <= CHECKSUM_TEMP_BUF_SIZE){
			check_len=length;
			flag = 1;	
		}else{
			check_len = CHECKSUM_TEMP_BUF_SIZE;
		}

		length -= check_len;
		read(imagefd, buf, check_len); 
		
    		for(i=0;i < check_len;i++)
    		{
    			crc = UPDC32(*(buf+i), crc);	
		}  
	}  	

    free(buf);
    //tcdbg_printf(" crc %x\n",crc);
    return ((unsigned long)crc);
}

int checkimageCrc(int img, int *checksum)
{
	int fd = -1;
	unsigned long read_conut = 0, count = 0, buflen = 0;
	char img_buf[sizeof(struct trx_header)];
	struct trx_header *trx = (struct trx_header *) img_buf;

	if ( 0 == img )
		fd = mtd_open("tclinux", O_RDONLY | O_SYNC);
	else
		fd = mtd_open("tclinux_slave", O_RDONLY | O_SYNC);
	
	if(fd < 0) 
	{
		printf("\nerror: Could not open device, image[%d]\n", img);
		return -1;
	}
	lseek(fd,0, SEEK_SET);

	buflen = read(fd, img_buf, sizeof(struct trx_header));
	if (buflen < sizeof(struct trx_header)) 
	{
		close(fd);
		printf("\nerror: Could not get image header, file too small (%d bytes), image[%d]\n", buflen, img);
		return -1;
	}
	//magic check
	if ((trx->magic != TRX_MAGIC2) || (trx->len < sizeof(struct trx_header)))
	{
		close(fd);
		printf("\nerror: Bad trx header, image[%d]\n", img);
		return -1;
	}
	//crc check
	if(trx->crc32 != calculate_crc32(fd, (trx->len-sizeof(struct trx_header))))
	{
		close(fd);
		printf("\nerror: crc32 check fail, checksum error image[%d]\n", img);
		return -1;
	}
	*checksum = trx->crc32;

	close(fd);
	return 0;
}

#define DUAL_IMG_BOOT_FLAG_FILE 	"/tmp/dual_image_boot_flag"
char get_current_boot_flag()
{
	char cmds[256] = {0};
	FILE *fp=NULL;
	char boot_flag = -1;

	sprintf(cmds, TC_FLASH_READ_CMD, DUAL_IMG_BOOT_FLAG_FILE, (unsigned long)IMG_BOOT_FLAG_SIZE, (unsigned long)IMG_BOOT_FLAG_OFFSET, RESERVEAREA_NAME);
	system(cmds);

	fp = fopen(DUAL_IMG_BOOT_FLAG_FILE, "r");
	if (fp == NULL) {
		return -1;
	}
	
	if (1 != fread(&boot_flag, 1, 1, fp)) {
		fclose(fp);
		unlink(DUAL_IMG_BOOT_FLAG_FILE);
		return -1;
	}
	fclose(fp);
	unlink(DUAL_IMG_BOOT_FLAG_FILE);

	boot_flag = boot_flag - '0';
	if ( boot_flag != 0 && boot_flag != 1 )
		boot_flag = 0;

	return boot_flag;	
}

int set_boot_flag_swap(void)
{
	char cmds[256] = {0};
	FILE *fp=NULL;
	char boot_flag = -1;
	
	boot_flag = get_current_boot_flag();
	if (boot_flag == 0)
		boot_flag = '1';
	else
		boot_flag = '0';
	
	fp = fopen(DUAL_IMG_BOOT_FLAG_FILE, "w");
	if (fp == NULL) {
		return -1;
	}
	
	if (1 != fwrite(&boot_flag, 1, 1, fp)) {
		fclose(fp);
		unlink(DUAL_IMG_BOOT_FLAG_FILE);
		return -1;
	}
	fclose(fp);

	snprintf(cmds, sizeof(cmds), TC_FLASH_WRITE_CMD, DUAL_IMG_BOOT_FLAG_FILE, (unsigned long)IMG_BOOT_FLAG_SIZE, (unsigned long)IMG_BOOT_FLAG_OFFSET, RESERVEAREA_NAME);
	system(cmds);
	unlink(DUAL_IMG_BOOT_FLAG_FILE);

	return 0;
	
}

static int doSysBootFlag(int argc, char *argv[])
{
	char boot_flag = -1;
	int checksum = 0;
	
    if ((argc != 2) || (strcmp(argv[1], "read") != 0 && strcmp(argv[1], "swap") != 0 && strcmp(argv[1], "checksum") != 0)){
		printf("Usage:\nread/swap bootflag: sys bootflag read/swap/checksum\n");
		return -1;
	}

	if (!strcmp(argv[1], "swap")) {
		set_boot_flag_swap();
	}
	else if (!strcmp(argv[1], "checksum")) {
		boot_flag = get_current_boot_flag();
		boot_flag = boot_flag ? 0 : 1;
		if ( 0 == checkimageCrc(boot_flag, &checksum) )
			printf("\nimage[%d] checksum=[%x] is ok\n", boot_flag, checksum);
		return 0;
	}

	boot_flag = get_current_boot_flag();
	printf("\r\ncurrent boot_flag = %d\n", boot_flag);

	return 0;
}
#endif

int main(int argc, char **argv) 
{
	FILE *proc_file;
	char cmd[160];
	const char *s;
	const char *applet_name;
	int i;
	char str[160];
	int func_num=0;

	if(argc == 2){
		if(!strcmp(argv[1],"version")){
			printf("\r\n tcci version: %s\n",MODULE_VERSION_TCCI);
			return 0;
		}
	}
	
	applet_name = argv[0];

	for (s = applet_name; *s != '\0';) {
		if (*s++ == '/')
			applet_name = s;
	}

	strcpy(cmd, applet_name);
	for (i = 1; i < argc; i++) {
		strcat(cmd, " ");
		strcat(cmd, argv[i]);
	}

    proc_file = fopen("/proc/tc3162/tcci_cmd", "w");
	if (!proc_file) {
		printf("open /proc/tc3162/tcci_cmd fail\n");
		return 0;
	}

	fprintf(proc_file, "%s", cmd);
	fclose(proc_file);

    proc_file = fopen("/proc/tc3162/tcci_cmd", "r");
	if (!proc_file) {
		printf("open /proc/tc3162/tcci_cmd fail\n");
		return 0;
	}
	fgets(str, 160, proc_file);
	func_num = atoi(str);
//	printf("data=%d\n", func_num);
	fclose(proc_file);
	
	if(func_num == 0 )
	   return 0;
	   
	if( ci_func[func_num] != NULL )	
        ci_func[func_num](argc-1, &argv[1]);
	return 0;
}
