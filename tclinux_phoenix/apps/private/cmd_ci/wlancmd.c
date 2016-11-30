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
#include	<stdio.h>
#include	<stdlib.h>
#include	<ctype.h>
#include	<string.h>
#include	<netinet/in.h>    /* for sockaddr_in       */
#include	<fcntl.h>
#include	<unistd.h>
#include	<sys/socket.h>    /* for connect and socket*/
#include	<sys/stat.h>
#include	<err.h>
#include	<errno.h>
#include	<asm/types.h>
#include	<linux/if.h>
#include	<linux/wireless.h>
#include	<sys/ioctl.h>
#include	<arpa/inet.h>
#include        "../../../version/tcversion.h"

#include	"oid.h"
#include "function.h"
#include "global.h"

#define WLANCMD_LOCK_FILE "/tmp/wlanlockfd"


TcWlanMode	GVWlanModeList[] =
{
	{"mixed"       , 0 },	
	{"bonly"        , 1 },
	{"gonly"        , 4 },
	{"nonly"        , 6 },		
	{"bgmixed"    , 0 },
	{"gnmixed"    , 7 },
	{"bgnmixed"  , 9 }
};

TcWlanChannelMode	GVWlanChannelList[] =
{
	{"1", 	0},
	{"2", 	1},
	{"5.5",	2},
	{"11",	3},
	{"6",	0},
	{"9",	1},
	{"12",	2},
	{"18",	3},
	{"24",	4},
	{"36",	5},
	{"48",	6},
	{"54",	7},
	#if 0
	{"6.5", 	0},
	{"13",	1},
	{"19.5",	2},
	{"26",	3},
	{"39",	4},
	{"52",	5},
	{"58.5",	6},
	{"65",	7},
	{"13",	8},
	{"26",	9},
	{"39",	10},
	{"52",	11},
	{"78",	12},
	{"104",	13},
	{"117",	14},
	{"130",	15}, 
	{"300",	32},//(BW=1, SGI=0) HT duplicate 6Mbps
	#endif
	{"0",	33}
};	

TcWlanCountry		WlanCountryChannelList[] =
{
	//"countrycode","countryregion";"channels"
     {"AL" , '1'}, //AL = 'ALBANIA';1-13
     {"DZ" , '1'}, //DZ = 'ALGERIA'; 1-13
     {"AR" , '1'}, //AR = 'ARGENTINA'; 1-13
     {"AM" , '1'}, //AM = 'ARMENIA'; 1-13
     {"AU" , '1'}, //AU = 'AUSTRALIA'; 1-13
     {"AT" , '1'}, //AT = 'AUSTRIA'; 1-13
     {"AZ" , '1'}, //AZ = 'AZERBAIJAN'; 1-13
     {"BH" , '1'}, //BH = 'BAHRAIN'; 1-13
     {"BY" , '1'}, //BY = 'BELARUS'; 1-13
     {"BE" , '1'}, //BE = 'BELGIUM'; 1-13
     {"BZ" , '1'}, //BZ = 'BELIZE'; 1-13
     {"BO" , '1'}, //BO = 'BOLIVIA'; 1-13
     {"BR" , '1'}, //BR = 'BRAZIL'; 1-13
     {"BN" , '1'}, //BN = 'BRUNEI'; 1-13
     {"BG" , '1'}, //BG = 'BULGARIA'; 1-13
     {"CA" , '0'}, //CA = 'CANADA'; 1-11
     {"CL" , '1'}, //CL = 'CHILE'; 1-13
     {"CN" , '1'}, //CN = 'CHINA'; 1-13
     {"CO" , '1'}, //CO = 'COLOMBIA'; 1-13 
     {"CR" , '1'}, //CR = 'COSTA RICA'; 1-13 
     {"HR" , '1'}, //HR = 'CROATIA';1-13
     {"CY" , '1'}, //CY = 'CYPRUS';1-13
     {"CZ" , '1'}, //CZ = 'CZECH REPUBLIC'; 1-13
     {"DK" , '1'}, //DK = 'DENMARK'; 1-13
     {"DO" , '0'}, //DO = 'DOMINICAN REPUBLIC'; 1-11
     {"EC" , '1'}, //EC = 'ECUADOR'; 1-13
     {"EG" , '1'}, //EG = 'EGYPT';1-13
     {"SV" , '1'}, //SV = 'EL SALVADOR'; 1-13
     {"EE" , '1'}, //EE = 'ESTONIA'; 1-13
     {"FI" , '1'}, //FI = 'FINLAND'; 1-13
     {"FR" , '3'}, //FR = 'FRANCE'; 10~13
     {"GE" , '1'}, //GE = 'GEORGIA';1-13
     {"DE" , '1'}, //DE = 'GERMANY'; 1-13
     {"GR" , '1'}, //GR = 'GREECE';1-13
     {"GT" , '0'}, //GT = 'GUATEMALA';1-11
     {"HN" , '1'}, //HN = 'HONDURAS';1-13
     {"HK" , '1'}, //HK = 'CHINA HONGKONG';1-13
     {"HU" , '1'}, //HU = 'HUNGARY';1-13
     {"IS" , '1'}, //IS = 'ICELAND';1-13
     {"IN" , '1'}, //IN = 'INDIA';1-13
     {"ID" , '1'}, //ID = 'INDONESIA'; 1-13
     {"IR" , '1'}, //IR = 'IRAN';1-13 
     {"IE" , '1'}, //IE = 'IRELAND';1-13
     {"IL" , '6'}, //IL = 'ISRAEL';3~9
     {"IT" , '1'}, //IT = 'ITALY';1-13
     {"JP" , '5'}, //JP = 'JAPAN';1-14
     {"JO" , '3'}, //JO = 'JORDAN';10~13
     {"KZ" , '1'}, //KZ = 'KAZAKHSTAN'; 1-13
     {"KP" , '1'}, //KP = 'NORTH KOREA';1-13
     {"KR" , '1'}, //KR = 'KOREA REPUBLIC';1-13
     {"KW" , '1'}, //KW = 'KUWAIT';1-13
     {"LV" , '1'}, //LV = 'LATVIA';1-13
     {"LB" , '1'}, //LB = 'LEBANON';1-13
     {"LI" , '1'}, //LI = 'LIECHTENSTEIN';1-13
     {"LT" , '1'}, //LT = 'LITHUANIA';1-13
     {"LU" , '1'}, //LU = 'LUXEMBOURG';1-13
     {"GB" , '1'}, //MO = 'CHINA MACAO';1-13
     {"MO" , '1'}, //MK = 'MACEDONIA';1-13
     {"MY" , '1'}, //MY = 'MALAYSIA';1-13
     {"MX" , '0'}, //MX = 'MEXICO';1-11
     {"MC" , '1'}, //MC = 'MONACO';1-13
     {"MA" , '1'}, //MA = 'MOROCCO';1-13
     {"NL" , '1'}, //NL = 'NETHERLANDS'; 1-13
     {"NZ" , '1'}, //NZ = 'NEW ZEALAND';1-13
     {"NO" , '1'}, //NO = 'NORWAY';1-13
     {"OM" , '1'}, //OM = 'OMAN';1-13
     {"PK" , '1'}, //PK = 'PAKISTAN';1-13
     {"PA" , '0'}, //PA = 'PANAMA';1-11
     {"PE" , '1'}, //PE = 'PERU';1-13
     {"PH" , '1'}, //PH = 'PHILIPPINES';1-13
     {"PL" , '1'}, //PL = 'POLAND';1-13
     {"PT" , '1'}, //PT = 'PORTUGAL';1-13
     {"PR" , '0'}, //PR = 'PUERTO RICO';1-11
     {"QA" , '1'}, //QA = 'QATAR'; 1-13
     {"RO" , '1'}, //RO = 'ROMANIA'; 1-13
     {"RU" , '1'}, //RU = 'RUSSIA'; 1-13
     {"SA" , '1'}, //SA = 'SAUDI ARABIA'; 1-13
     {"SG" , '1'}, //SG = 'SINGAPORE'; 1-13
     {"SK" , '1'}, //SK = 'SLOVAKIA'; 1-13
     {"SI" , '1'}, //SI = 'SLOVENIA'; 1-13
     {"ZA" , '1'}, //ZA = 'SOUTH AFRICA'; 1-13
     {"ES" , '2'}, //ES = 'SPAIN'; 10~11
     {"SE" , '1'}, //SE = 'SWEDEN'; 1-13
     {"CH" , '1'}, //CH = 'SWITZERLAND'; 1-13
     {"SY" , '1'}, //SY = 'SYRIA ARAB REPUBLIC'; 1-13
     {"TW" , '1'}, //TW = 'CHINA TAIWAN'; 1-13
     {"TH" , '1'}, //TH = 'THAILAND';1-13
     {"TT" , '1'}, //TT = 'TRINIDAD AND TOBAGO'; 1-13
     {"TN" , '1'}, //TN = 'TUNISIA'; 1-13
     {"TR" , '1'}, //TR = 'TURKEY'; 1-13
     {"UA" , '1'}, //UA = 'UKRAINE'; 1-13
     {"AE" , '1'}, //AE = 'UNITED ARAB EMIRATES'; 1-13
     {"GB" , '1'}, //GB = 'UNITED KINGDOM'; 1-13
     {"US" , '0'}, //US = 'UNITED STATES';1-11
     {"UY" , '1'}, //UY = 'URUGUAY'; 1-13
     {"UZ" , '1'}, //UZ = 'UZBEKISTAN'; 1-13 
     {"VE" , '1'}, //VE = 'VENEZUELA'; 1-13
     {"VN" , '1'}, //VN = 'VIETNAM'; 1-13 
     {"YE" , '1'}, //YE = 'YEMEN'; 1-13
     {"ZW" , '1'}, //ZW = 'ZIMBABWE'; 1-13 
        /*{  "Other"         , '1' },*/
};

static int doWlanVer(int argc, char *argv[], void *p);
static int doWlanDriverver(int argc, char *argv[], void *p);
static int doWlanFirmwarever(int argc, char *argv[], void *p);
static int doWlanUp(int argc, char *argv[], void *p);
static int doWlanDown(int argc, char *argv[], void *p);
static int doWlanMode(int argc, char *argv[], void *p);
static int doWlanRate(int argc, char *argv[], void *p);
static int doWlanRateset(int argc, char *argv[], void *p);
static int doWlanChannel(int argc, char *argv[], void *p);
static int doWlanRegion(int argc, char *argv[], void *p);
static int doWlanPhytype(int argc, char *argv[], void *p);
static int doWlanTxpwr(int argc, char *argv[], void *p);
static int doWlanBssid(int argc, char *argv[], void *p);
static int doWlanSsid(int argc, char *argv[], void *p);
static int doWlanHide(int argc, char *argv[], void *p);
static int doWlanAssoclist(int argc, char *argv[], void *p);
static int doWlanSecmode(int argc, char *argv[], void *p);
static int doWlanWepkey(int argc, char *argv[], void *p);
static int doWlanWepkeyindex(int argc, char *argv[], void *p);
static int doWlanPskkey(int argc, char *argv[], void *p);
static int doWlanWpaencryption(int argc, char *argv[], void *p);
static int doWlanHelp(int argc, char *argv[], void *p);
static int doWlanFltmacctl(int argc, char *argv[], void *p);
static int doWlanFltmacmode(int argc, char *argv[], void *p);
static int doWlanFltmac(int argc, char *argv[], void *p);
static int doWlanAssocctl(int argc, char *argv[], void *p);
#if 1
static int doWlanMBss(int argc, char *argv[], void *p);
#endif
static int doWlanBgprotection(int argc, char *argv[], void *p);
static int doWlanRtsthreshold(int argc, char *argv[], void *p);
static int doWlanBeaconperiod(int argc, char *argv[], void *p);
static int doWlanFragthreshold(int argc, char *argv[], void *p);
static int doWlantxBurst(int argc, char *argv[], void *p);
static int doWlanDtimperiod(int argc, char *argv[], void *p);
static int doWlanWmm(int argc, char *argv[], void *p);
//#ifdef SUPPORT_ATP_WPS
static int doWlanWpsNvram(int argc, char *argv[], void *p);
static int doWlanWpsMode(int argc, char *argv[], void *p);
static int doWlanWpsCtl(int argc, char *argv[], void *p);
static int doWlanWpsPinval(int argc, char *argv[], void *p);
static int doWlanWpsSwitch(int argc, char *argv[], void *p);
static int doWlanWpsConf(int argc, char *argv[], void *p);
//#endif
static int doWlanApIsolate(int argc, char *argv[], void *p);
static int doWlanHTGI(int argc, char *argv[], void *p);
static int doWlanHTBW(int argc, char *argv[], void *p);
static int doWlanHTMCS(int argc, char *argv[], void *p);


static cmds_t WlanCmds[] = {
	
	{"cmdver", doWlanVer, 0x10,   0,  NULL},/*show wlan cmd version*/	
	{"driverver", doWlanDriverver, 0x10,   0,  NULL},/*show wlan driver version*/	
	{"firmwarever", doWlanFirmwarever, 0x10,   0,  NULL},/*show wlan fireware version*/	
	{"up", doWlanUp, 0x10,   0,  NULL},/*enable wlan*/	
	{"down", doWlanDown, 0x10,   0,  NULL},    /* disable wlan */
	{"mode", doWlanMode, 0x10,   0,  NULL},  	/* set or display wlan mode*/
	{"rate", doWlanRate,0x10,   0,  NULL },/* set or display a fixed rate*/	
	{"rateset", doWlanRateset, 0x10,   0,  NULL },/*set or display the basic rateset, (b) indicates basic*/
	{"channel", doWlanChannel, 0x10,   0,  NULL },/*set or display wlan channel*/	
	{"region", doWlanRegion,  0x10,   0,  NULL},/*set or display wlan Country code*/	
	{"phytype", doWlanPhytype, 0x10,   0,  NULL},/*display wlan phy type*/	
	{"txpwr", doWlanTxpwr, 0x10,   0,  NULL},/*set or display tx power in various units*/	
	{"bssid", doWlanBssid, 0x10,   0,  NULL},/*display wlan bssid*/	
	{"ssid", doWlanSsid, 0x10,   0,  NULL},/*set or display a configuration's SSID*/	
	{"hide", doWlanHide, 0x10,   0,  NULL},/*hides the network from active scans, enable or disable*/	                    	
	{"assoclist", doWlanAssoclist, 0x10,   0,  NULL},/*Get the list of linked MACaddr*/	
	{"secmode", doWlanSecmode, 0x10,   0,  NULL},/*set or display one SSID's secure mode*/	
	{"wepkey", doWlanWepkey, 0x10,   0,  NULL},/*set or display one SSID wepkey*/	
	{"wepkeyindex", doWlanWepkeyindex, 0x10,   0,  NULL},/*set or display one SSID wepkey index number*/	
	{"pskkey", doWlanPskkey, 0x10,   0,  NULL},/*set or display one SSID pskkey*/	
	{"wpaencryption", doWlanWpaencryption, 0x10,   0,  NULL},/*set or display one SSID WPA encryption Mode*/	
	{"help", doWlanHelp,  0x10,   0,  NULL},/*show wlancmd help*/	
	{"fltmacctl",doWlanFltmacctl,0x10,   0,  NULL},/*set or display the mac filter enable or disable*/	
	{"fltmacmode",doWlanFltmacmode,0x10,   0,  NULL},/*set or display the format of mac filter allow or deny*/	
	{"fltmac",doWlanFltmac,0x10,   0,  NULL},/*set or display the soure macaddress*/	
	{"assocctl",doWlanAssocctl,0x10,   0,  NULL},/*set or display the assoc device ctl*/
	#if 1
	{"mbss",doWlanMBss,0x10,   0,  NULL},/*set or display the bss*/
	#endif
	{"bgprotection", doWlanBgprotection,0x10,   0,  NULL},/*set protection*/
	{"rtsthreshold", doWlanRtsthreshold,0x10,   0,  NULL},/*set rts threadold*/
	{"fragthreshold", doWlanFragthreshold,0x10,   0,  NULL},/*set fragmetation threadold*/
	{"beaconperiod", doWlanBeaconperiod,0x10,   0,  NULL},/*set beacon interval*/
	{"txburst", doWlantxBurst,0x10,   0,  NULL},/*set txburst*/
	{"dtim", doWlanDtimperiod,0x10,   0,  NULL},/*set dtim period*/
	{"wme", doWlanWmm,0x10,   0,  NULL},/*set wmm function*/
//#ifdef SUPPORT_ATP_WPS
	{"wps_nvram",doWlanWpsNvram,0x10,   0,  NULL},/*init wlan nvram*/
	{"wps_ctl",doWlanWpsCtl,0x10,   0,  NULL},/*set or display wps status,enable or disable*/
	{"wps_mode",doWlanWpsMode,0x10,   0,  NULL},	/*set wps mode,ap-pin or ap-pbc*/	
	{"wps_pinval",doWlanWpsPinval,0x10,   0,  NULL},/*set the pin value as wps under the pin mode*/	
	{"wps_switch",doWlanWpsSwitch,0x10,   0,  NULL},/*start the wsc course*/	
	{"wps_conf",doWlanWpsConf,0x10,   0,  NULL},/*set wps parameter*/
//#endif
	{"ap_isolate",doWlanApIsolate,0x10,   0,  NULL},/*set or display AP isolation*/
	{"HT_GI",doWlanHTGI,0x10,   0,  NULL},/*set or display long or short Guard Interval*/
	{"HT_BW",doWlanHTBW,0x10,   0,  NULL},/*set or display 11n channel width*/
	{"HT_MCS",doWlanHTMCS,0x10,   0,  NULL},/*set or display 11n MCS*/
	{NULL,          NULL,               0x10,   0,  NULL},
};

/*------------------------/
/		 common function	   /										
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

void ConvertStrToShellStr(char *str, char *buf) {
   if ( buf == NULL ) return;

   int len = strlen(str);
   int i = 0, j = 0;

   for ( i = 0; i < len; i++ ) {
      if ( str[i] != '\'' ) {
         buf[j++] = '\'';
         buf[j++] = str[i];
         buf[j++] = '\'';
      } else {
         buf[j++] = '"';
         buf[j++] = str[i];
         buf[j++] = '"';
      }
   }

   buf[j]  = '\0';
}

int GetPhyMode(void)
{
#if 0
	int socket_id;
	int ret;
	char data[2048] = {0};
	char cmd[5] = {0};
	struct iwreq wrq;
	RT_802_11_MAC_TABLE *mp;
	int i,j;
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);
	strcpy(wrq.ifr_name, INIC_MAINIFNAME);
	wrq.u.data.length = 2048;
	wrq.u.data.pointer = data;
	wrq.u.data.flags = 0;
	ret = ioctl(socket_id, RTPRIV_IOCTL_GET_MAC_TABLE, &wrq);
	if(ret != 0)
	{
		printf("GetPhyMode display error\n");
		close(socket_id);
		return FAIL;
	}
  
	mp = (RT_802_11_MAC_TABLE *)wrq.u.data.pointer;
	ret = mp->PhyMode;
	close(socket_id);
	return ret;
#endif
#ifndef BBUTOWBU
	int socket_id;
	int ret;
	unsigned int modestatus = 0;
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);
	if(OidQueryInformation(RT_OID_GET_PHY_MODE,socket_id,INIC_MAINIFNAME,&modestatus,0)<0)
		return -1;
	else
		return modestatus;
#else
	struct iwreq wrq;
	int ret=0;
	char data[8];
	int socket_id;
	unsigned int modestatus = 0;
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);
	if(socket_id==-1){
		printf("==>GetPhyMode: Create socket fail\n");
		return -1;
	}
	strcpy(wrq.ifr_name, INIC_MAINIFNAME);
       wrq.u.data.length = sizeof(data);
       wrq.u.data.pointer = data;
       wrq.u.data.flags = RT_OID_GET_PHY_MODE;
       ret=ioctl(socket_id, RT_PRIV_IOCTL , &wrq);
	if ( ret != 0 ) {
		printf("==>GetPhyMode: ioctl open fail\n");
    		close(socket_id);
		return -1;
	}
	modestatus=wrq.u.mode;
	close(socket_id);
	return modestatus;
			
#endif			
}

void SetHtMcs(char *Mode)
{

	char	cIwprivValue[3];
	int     socket_id;
	char cmd[64] = {0};
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);
	
	
	if(!strcmp(Mode,"B"))
	{
		sprintf(cmd,"iwpriv %s set HtOpMode=1",INIC_MAINIFNAME);
    		system(cmd);  
		sprintf(cmd,"iwpriv %s set BasicRate=3",INIC_MAINIFNAME);
       	system(cmd); 
		sprintf(cmd,"iwpriv %s set FixedTxMode=CCK",INIC_MAINIFNAME);
        	system(cmd); //CCK
        
    	}
    	if(!strcmp(Mode,"G"))
    	{
    		sprintf(cmd,"iwpriv %s set HtOpMode=0",INIC_MAINIFNAME);
    		system(cmd);  
		sprintf(cmd,"iwpriv %s set BasicRate=351",INIC_MAINIFNAME);
       	system(cmd); 
		sprintf(cmd,"iwpriv %s set FixedTxMode=OFDM",INIC_MAINIFNAME);
        	system(cmd); //OFDM
    	}
    	if(!strcmp(Mode,"N"))
    	{
		sprintf(cmd,"iwpriv %s set HtOpMode=1",INIC_MAINIFNAME);
    		system(cmd);  
		sprintf(cmd,"iwpriv %s set BasicRate=15",INIC_MAINIFNAME);
       	system(cmd); 
		sprintf(cmd,"iwpriv %s set FixedTxMode=0",INIC_MAINIFNAME);
        	system(cmd); 
    	}
    	if(!strcmp(Mode,"BGN"))
    	{
    		sprintf(cmd,"iwpriv %s set HtOpMode=0",INIC_MAINIFNAME);
    		system(cmd);  
		sprintf(cmd,"iwpriv %s set BasicRate=15",INIC_MAINIFNAME);
       	system(cmd); 
		sprintf(cmd,"iwpriv %s set FixedTxMode=0",INIC_MAINIFNAME);
        	system(cmd); 
    	}
}

int  setSSID(char *ucInterface)
{
 	struct iwreq wrq;
	int socket_id;
	char data[WLAN_SYS_LEN];
	char ssid[132];
	char acCmd[132];
	
	memset(data,0,sizeof(data));
	memset(ssid,0,sizeof(ssid));
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);
       strcpy(wrq.ifr_name, ucInterface);
       wrq.u.data.length = 255;
       wrq.u.data.pointer = data;
       wrq.u.data.flags = 0;
       ioctl(socket_id, SIOCGIWESSID , &wrq);
       ConvertStrToShellStr(wrq.u.essid.pointer, ssid);
       sprintf(acCmd, "iwpriv %s set SSID=%s", ucInterface, ssid);
       system(acCmd);
	close(socket_id);
	return 0;
}

void showInfo(int argc, char *argv[], void *p)
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

/*------------------------/
/		 ioctl function		   /										
/------------------------*/
int OidQueryInformation(unsigned long OidQueryCode, int socket_id, char *DeviceName, void *ptr, unsigned long PtrLength)
{
    struct iwreq wrq;

    strcpy(wrq.ifr_name, DeviceName);
    wrq.u.data.length = PtrLength;
    wrq.u.data.pointer = (caddr_t) ptr;
    wrq.u.data.flags = OidQueryCode;

    return (ioctl(socket_id, RT_PRIV_IOCTL,&wrq));
}

int iwpriv_set(int socket_id, char *DeviceName, char *parameter, char *value)
{
	unsigned char	data[WLAN_SYS_LEN];
	struct 			iwreq wrq;
	int 			ret;
	
	memset(data, 0x00, WLAN_SYS_LEN);
	sprintf(data, "%s=%s", parameter, value);

	strcpy(wrq.ifr_name, DeviceName);
	wrq.u.data.length = strlen(data)+1;
	wrq.u.data.pointer = data;
	wrq.u.data.flags = 0;
	ret = ioctl(socket_id, RTPRIV_IOCTL_SET, &wrq);
	if(ret != 0)
	{
		printf("\nerror:set %s \n\n", parameter);
	}
	return 0;
}

/*------------------------/
/		cmd function		   /										
/------------------------*/
int doWlanVer(int argc, char *argv[], void *p)
{
	showInfo(argc,argv,p);
	if((argc == 2)&&(!strcmp(argv[1] , "display") ))
	{
		printf(TCWlancmdVer);
		return SUCCESS;
	}
	else
	{
		printf("\r\nUsage:wlanphxcmd cmdver display\n");
		return FAIL;
	}
}

static int doWlanFirmwarever(int argc, char *argv[], void *p)
{
	showInfo(argc,argv,p);
	if((argc == 2) && (!strcmp(argv[1] , "display")))
	{
		printf(TCWLANDriverVer);
		return SUCCESS;
	}
	else
	{
		printf("\r\nUsage:wlanphxcmd firmwarever display\n");
		return FAIL;
	}
}

static int doWlanDriverver(int argc, char *argv[], void *p)
{
	char buf[32] = {0};
	int  socket_id;
	char interfacebuf[10] = {0};
	
	showInfo(argc,argv,p);
	if((argc == 2) && (!strcmp(argv[1] , "display")))
	{
		socket_id = socket(AF_INET, SOCK_DGRAM, 0);
		sprintf(interfacebuf,"%s",INIC_MAINIFNAME);
		if (OidQueryInformation(RT_OID_VERSION_INFO, socket_id, interfacebuf, buf, 0) < 0)
		{
			printf("ERROR:RT_OID_VERSION_INFO\n");
		}
		else
		{
			printf("%s\n", buf);
		}
	}
	else
	{
		printf("\r\nUsage:wlanphxcmd driverver display\n");
	}
	close(socket_id);
	return SUCCESS;
}

static int doWlanUp(int argc, char *argv[], void *p)
{
	unsigned char cmd[WLAN_32_LEN];
	
	showInfo(argc,argv,p);
	
	if((argc == 3)&&(!strcmp(argv[1],"-i")))
	{
		memset(cmd, 0, WLAN_32_LEN);
		sprintf(cmd, "ifconfig %s%s up", INIC_INTERFACE,argv[2]);
		system(cmd);	
	}
	else
	{
		printf("\r\nUsage:wlanphxcmd up [-i ssidindex]\n");
	}
	return SUCCESS;
}

static int doWlanDown(int argc, char *argv[], void *p)
{
	unsigned char cmd[WLAN_32_LEN];

	showInfo(argc,argv,p);
	if((argc == 3)&&(!strcmp(argv[1],"-i")))
	{
		//system("killall wpsd 2>/dev/null");
		memset(cmd, 0, WLAN_32_LEN);
		sprintf(cmd, "ifconfig %s%s down", INIC_INTERFACE,argv[2]);
		system(cmd);	
	}
	else
	{
		printf("\r\nUsage:wlanphxcmd down [-i ssidindex]\n");
	}
	return SUCCESS;
}

static int doWlanMode(int argc, char *argv[], void *p)
{
	int     iWMNumbers;
	int     iWMIndex;
	char*   cIdeaWM;
	char	cIwprivValue[WLAN_16_LEN];
	int     socket_id;
	char    acCmd[WLAN_SYS_LEN] = {0};	
   	char statusstr[5] = {0}; 
   	unsigned int modestatus=0;
	
	showInfo(argc,argv,p);
	if(argc<2)
	{
	 	printf("\r\nUsage:wlanphxcmd mode set {bonly|gonly|nonly|mixed|gnmixed|bgnmixed}");
		printf("\r\nUsage:wlanphxcmd mode display");
		close(socket_id);
		return FAIL;
	}
	
	iWMNumbers = sizeof(GVWlanModeList)/sizeof(GVWlanModeList[0]);
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);
		
	if((argc == 3)&&(!strcmp(argv[1], "set")))
	{
            for( iWMIndex = 0 ; iWMIndex < iWMNumbers ; iWMIndex++ )
            {
                cIdeaWM = GVWlanModeList[iWMIndex].cWirelessMode;                  	// Get Idea Command

                if(!strcmp(argv[2], cIdeaWM))
                {			
			memset(cIwprivValue, 0x00, WLAN_16_LEN);
                	sprintf(cIwprivValue, "%d", GVWlanModeList[iWMIndex].iWirelessModeNumber);
			#if 0
                    	sprintf(acCmd, "echo WirelessMode=%s >> %s", cIwprivValue,WLANDATPATH);
                     system(acCmd);
			#endif
                	iwpriv_set(socket_id, INIC_MAINIFNAME, "WirelessMode", cIwprivValue);
			#if 0
                	if(strcmp(cIwprivValue, "6") && strcmp(cIwprivValue, "7") && strcmp(cIwprivValue, "9"))
                	{
                	    memset(acCmd, 0 , sizeof(acCmd));
                	    for( iWMIndex = 0 ; iWMIndex < 4 ; iWMIndex++ )
                        {
        	           	sprintf(acCmd, "ifconfig %s%d down", INIC_INTERFACE,iWMIndex);
        	           	system(acCmd);
                        }
                        memset(acCmd, 0 , sizeof(acCmd));
                	    for( iWMIndex = 0 ; iWMIndex < 4 ; iWMIndex++ )
                        {
        	           	sprintf(acCmd, "ifconfig %s%d up", INIC_INTERFACE,iWMIndex);
        	           	system(acCmd);
                        }
                	}
			#endif
                }
            }
	}
	else if(!strcmp(argv[1], "display"))	
	{
            //unsigned int status=0;
            if(OidQueryInformation(RT_OID_GET_PHY_MODE,socket_id,INIC_MAINIFNAME,&modestatus,0)<0)
            {
                printf("ERROR:RT_OID_GET_PHY_MODE\n");
            }
            else
            {
                printf("PHY mode status=%d\n", modestatus);
            }
	}
	else
	{
              printf("\r\nUsage:wlanphxcmd mode set {bonly|gonly|nonly|mixed|gnmixed|bgnmixed}");
		printf("\r\nUsage:wlanphxcmd mode display\n");
		close(socket_id);
		return FAIL;
	}
	
    close(socket_id);
    return SUCCESS;
}

static int doWlanRate(int argc, char *argv[], void *p)
{
	int     		socket_id;
	char			cIwprivValue[WLAN_16_LEN];
	unsigned long	Mcs_tmp;
	char                   *cIdeaWM;
	int 			iWMNumbers;
	int 			iWMIndex;
	int             ret;
	int             ChannelValue;

    	memset(cIwprivValue, 0x00, WLAN_16_LEN);
		
	showInfo(argc,argv,p);

	if(argc<2)
	{
	 	printf("\r\nUsage:wlanphxcmd rate set {rate value}");
	 	printf("\r\n\t802.11b :(1, 2, 5.5, 11)");
		printf("\r\n\t802.11g :(1, 2, 5.5, 6, 9, 11, 12, 18, 24, 36, 48, 54)");
		printf("\r\n\t0 default: automatically select the best rate\n");
		close(socket_id);
		return FAIL;
	}
	
	iWMNumbers = sizeof(GVWlanChannelList)/sizeof(GVWlanChannelList[0]);
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);	

	if((argc == 3)&&(!strcmp(argv[1], "set")))
	{
       	ret = GetPhyMode();		
       	ChannelValue = atoi(argv[2]);
		if(ret < 0)
		{
			printf("\r\nno get phy mode");
			close(socket_id);	
			return FAIL;
		}
	       else if(ret == 0)	//B/G mixed
		 {
			 
			   if(strcmp(argv[2],GVWlanChannelList[0].cChannelRate) == 0
					   || strcmp(argv[2],GVWlanChannelList[1].cChannelRate) == 0
					   || strcmp(argv[2],GVWlanChannelList[2].cChannelRate) == 0
					   || strcmp(argv[2],GVWlanChannelList[3].cChannelRate) == 0)
				{	
					SetHtMcs("B");
				}
			   	else{
	         		SetHtMcs("G");
				}
	       }
		else if(ret == 4)
	       {
	         	SetHtMcs("G");
	       }
	       else if(ret == 1)	//B only
	       {
	       	SetHtMcs("B");
	       }
		/*because n mode can't set in the following way*/
	       else if(ret == 6)	//N only
	       {
	       	SetHtMcs("N");
	       	return SUCCESS;
	       }
	       else if(ret == 9)	//BGN MIXED
	       {
	       	SetHtMcs("BGN");
	        	return SUCCESS;
	       }
		   
		for( iWMIndex = 0 ; iWMIndex < iWMNumbers ; iWMIndex++ )
		{
			cIdeaWM = GVWlanChannelList[iWMIndex].cChannelRate;          
			if(strcmp(argv[2], cIdeaWM) == 0)
			{	
				memset(cIwprivValue, 0x00, WLAN_16_LEN);
				sprintf(cIwprivValue, "%d", GVWlanChannelList[iWMIndex].iRateCount);	
				iwpriv_set(socket_id, INIC_MAINIFNAME, "HtMcs", cIwprivValue);
			}
		}
	}
	else if(!strcmp(argv[1], "display"))	
	{
    		unsigned char ucname[30];
    		unsigned char data[255];
    		struct iwreq wrq;
		memset(ucname, 0x00, 30);
		strcpy(ucname, INIC_MAINIFNAME);
		memset(data, 0x00, 255);
		strcpy(wrq.ifr_name, ucname);
    		wrq.u.data.length = 255;
    		wrq.u.data.pointer = data;
    		wrq.u.data.flags = 0;
		ioctl(socket_id, SIOCGIWRATE , &wrq);
    		printf("\n%dMbps\n", (int)(wrq.u.bitrate.value/1000000));
	}
	else
	{        
	   	printf("\r\nUsage:wlanphxcmd rate set {rate value}");
	 	printf("\r\n\t802.11b :(1, 2, 5.5, 11)");
		printf("\r\n\t802.11g :(1, 2, 5.5, 6, 9, 11, 12, 18, 24, 36, 48, 54)");
		printf("\r\n\t0 default: automatically select the best rate\n");
  	       close(socket_id);	
		return FAIL;
	}
		
  	close(socket_id);	
	return SUCCESS;
}

static int doWlanRateset(int argc, char *argv[], void *p)
{
	int     		socket_id;
	char			cIwprivValue[WLAN_16_LEN];
	char temp[40] = {0};

	showInfo(argc,argv,p);
	/*do setplcphdr*/
	sprintf(temp,"iwpriv %s set ShortSlot=1",INIC_MAINIFNAME);
    	system(temp);
	
	if(((argc == 2) && (!strcmp(argv[1], "set")))||
	(argc < 2) ||
	((argc != 3) && ((argv[2][0]>0x3A)||(argv[2][0]<0x30))))
	{
		printf("\r\nUsage:wlanphxcmd rateset [all | default | set {arbitrary rateset}]\n");
		return FAIL;
	}

	socket_id = socket(AF_INET, SOCK_DGRAM, 0);
	memset(cIwprivValue, 0x00, WLAN_16_LEN);
	/*all is default ,default is all*/
	if(!strcmp(argv[1], "all"))
		sprintf(cIwprivValue, "%s", "15");
	else if(!strcmp(argv[1], "default"))
		sprintf(cIwprivValue, "%s", "4095");	
	else if((argc == 3)&&(!strcmp(argv[1], "set")))
	{
		strcpy(cIwprivValue, argv[2]);	
	}
	else
	{
		close(socket_id);	
		return FAIL;
	}

	iwpriv_set(socket_id, INIC_MAINIFNAME, "BasicRate", cIwprivValue);	
				
  	close(socket_id);	
	return SUCCESS;
}

static int doWlanChannel(int argc, char *argv[], void *p)
{
	int     		socket_id;
	char			cIwprivValue[WLAN_8_LEN] = {0};
	unsigned long	Channel_tmp;
	char acCmd[WLAN_SYS_LEN] = {0};
	char temp[64] = {0};

	showInfo(argc,argv,p);
		
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);	
	if(((argc == 2) && (!strcmp(argv[1], "set")))||
	(argc < 2) ||
	((argc == 3) && ((argv[2][0]>0x3A)||(argv[2][0]<0x30))))
	{
		printf("\r\nUsage:wlanphxcmd channel set {channel value}");
		printf("\r\n\tthe channel scope is according to different country code");
		printf("\r\n\t0 means auto channel\n");
		return FAIL;
	}
	
	if((argc == 3)&&(!strcmp(argv[1], "set")))
	{
		sprintf(cIwprivValue, "%s", argv[2]);	
		iwpriv_set(socket_id, INIC_MAINIFNAME, "Channel", cIwprivValue);
		if ((strcmp(cIwprivValue, "12") == 0) || (strcmp(cIwprivValue, "13") == 0))
		{
			sprintf(temp,"iwpriv %s set HtExtcha=0",INIC_MAINIFNAME);
           		system(temp);
		}
    		else
    		{
    			sprintf(temp,"iwpriv %s set HtExtcha=1",INIC_MAINIFNAME);
     			system(temp);
    		}
	}
	else if(!strcmp(argv[1], "display"))	
	{
		struct iwreq wrq;
		char     data[WLAN_SYS_LEN];
		memset(data, 0x00, WLAN_SYS_LEN);
		strcpy(wrq.ifr_name, INIC_MAINIFNAME);
    		wrq.u.data.length = 255;
    		wrq.u.data.pointer = data;
    		wrq.u.data.flags = 0;
		ioctl(socket_id, SIOCGIWFREQ , &wrq);
		printf("\r\ncurrent mac channel is %d", wrq.u.freq.m);
		printf("\r\ntarget channnel %d\n", wrq.u.freq.m);
	}
	else if(!strcmp(argv[1], "list"))
	{
        	printf("1 2 3 4 5 6 7 8 9 10 11 12 13 14 \n");
	}
	else
	{
		printf("\r\nUsage:wlanphxcmd channel set {channel value}\n");
  		close(socket_id);	
		return FAIL;	
	}	

  	close(socket_id);	
	return SUCCESS;
}

static int doWlanRegion(int argc, char *argv[], void *p)
{
	int     		socket_id;
	int                     iWMNumbers,iWMIndex;
	char			cIwprivValue[WLAN_8_LEN];
    	char 	        ucInterface[WLAN_8_LEN];
	int                     ret;
	char                   *cIdeaWM;
    	char CountryRegion = '1';
	char cmd[130] = {0};
	
	showInfo(argc,argv,p);
#if 1//why need to set igmpsnoop function?
	for( iWMIndex = 0 ; iWMIndex < 4 ; iWMIndex++ )
	{
		if(iWMIndex == 0)
		{
            		/*do igmpsnenable first*/
			memset(cmd,0,sizeof(cmd));
            		sprintf(cmd, "iwpriv %s set IgmpSnEnable=1", INIC_MAINIFNAME);
            		system(cmd);
           		 /*end of do igmpsnenable*/
        	}
        	else
        	{
			/*do igmpsnenable first*/
			memset(cmd,0,sizeof(cmd));
            		sprintf(cmd, "iwpriv %s%d set IgmpSnEnable=1", INIC_INTERFACE, iWMIndex);
            		system(cmd);
            		/*end of do igmpsnenable*/
        	}
	}
#endif
	if(((argc == 2) && (!strcmp(argv[1], "set")))||(argc < 2))
	{
		printf("\r\nUsage:wlanphxcmd region set {region value}\n");
		return FAIL;
	}
	
	iWMNumbers = sizeof(WlanCountryChannelList)/sizeof(WlanCountryChannelList[0]);
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);	

	if((argc == 3)&&(!strcmp(argv[1], "set")))
	{
		for( iWMIndex = 0 ; iWMIndex < iWMNumbers ; iWMIndex++ )
		{
			cIdeaWM = WlanCountryChannelList[iWMIndex].cCountryString;       
			if(!strcmp(argv[2], cIdeaWM))
			{			
				memset(cIwprivValue, 0x00, WLAN_8_LEN);
				sprintf(cIwprivValue, "%c", WlanCountryChannelList[iWMIndex].iRegionChannel);
				CountryRegion = WlanCountryChannelList[iWMIndex].iRegionChannel;
				//countrycode must be satified with countryregion
				iwpriv_set(socket_id, INIC_MAINIFNAME, "CountryRegion", cIwprivValue);
				iwpriv_set(socket_id, INIC_MAINIFNAME, "CountryCode", argv[2]);
                		break;
			}
		}
       	if(iWMIndex == iWMNumbers)    
       	{
            		printf("iWirelessModeNumber=other\n");
			CountryRegion = '1';
			iwpriv_set(socket_id, INIC_MAINIFNAME, "CountryRegion", "1");
			iwpriv_set(socket_id, INIC_MAINIFNAME, "CountryCode", argv[2]);
        	}
		#if 0
	    	sprintf(cmd, "echo Default > %s",WLANDATPATH);
       	system(cmd);
        	sprintf(cmd, "echo CountryRegion=%c >> %s", CountryRegion,WLANDATPATH);
        	system(cmd);
        	sprintf(cmd, "echo CountryCode=%s >> %s", argv[2],WLANDATPATH);
        	system(cmd);   
		#endif
	}
	else
	{
		printf("\r\nUsage:wlanphxcmd region set {region value}\n");
  		close(socket_id);	
		return FAIL;	
	}	
	
   	close(socket_id);	
	return SUCCESS;	
}

static int doWlanPhytype(int argc, char *argv[], void *p)
{
	int ret;
	char mode[10] = {0};
	
	showInfo(argc,argv,p);
	if((argc == 2)&&(!strcmp(argv[1], "display")))
	{
		ret = GetPhyMode();
		switch(ret)
		{
			case 1://b
				strcpy(mode,"b");
				break;
			case 4://g
			case 0://bg
				strcpy(mode,"g");
				break;
			case 6://n
			case 7://gn
			case 9://bgn
				strcpy(mode,"n");
				break;
			default:
				return FAIL ;
		}
		printf("PhyMode=%d(%s)\n", ret,mode);
	}
	else
	{
		printf("\r\nUsage:wlanphxcmd phytype display\n");
		return FAIL;
	}

	return SUCCESS;
}

static int doWlanTxpwr(int argc, char *argv[], void *p)
{
	int     		socket_id;
	
	showInfo(argc,argv,p);
       if(((argc == 2) && (!strcmp(argv[1], "set")))||
	(argc < 2) ||
	((argc == 4) && ((argv[3][0]>0x3A)||(argv[3][0]<0x30))))
	{
		printf("\r\nUsage:wlanphxcmd txpwr set {-p value(1~100)}");
		return FAIL;
	}	
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);	

	if((argc == 4)&& (!strcmp(argv[1], "set")) &&(!strcmp(argv[2], "-p")))
	{
		//100~90=>EEPROM default,90~60=>-1db,60~30=>-3db,30~15=>-6db,15~9=>-9db,9~0=>-12db
		iwpriv_set(socket_id, INIC_MAINIFNAME, "TxPower", argv[3]);
	}
	else
	{
		printf("\r\nUsage:wlanphxcmd txpwr set {-p value(1~100)}");
  		close(socket_id);	
		return FAIL;		
	}

    	close(socket_id);	
	return SUCCESS;
}

static int doWlanBssid(int argc, char *argv[], void *p)
{
	int    socket_id;
    	char  ucInterface[WLAN_8_LEN];
    	struct iwreq wrq;
    	char   data[WLAN_SYS_LEN];

	showInfo(argc,argv,p);
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);	
	if((argc == 2)&&(!strcmp(argv[1], "display")))
	{
		memset(data, 0x00, WLAN_SYS_LEN);
		strcpy(wrq.ifr_name, INIC_MAINIFNAME);
    		wrq.u.data.length = 255;
    		wrq.u.data.pointer = data;
    		wrq.u.data.flags = 0;
		ioctl(socket_id, SIOCGIFHWADDR , &wrq);
		printf("BSSID: \"%02X:%02X:%02X:%02X:%02X:%02X\"\n", 
		(unsigned char)wrq.u.ap_addr.sa_data[0],(unsigned char)wrq.u.ap_addr.sa_data[1],
		(unsigned char)wrq.u.ap_addr.sa_data[2],(unsigned char)wrq.u.ap_addr.sa_data[3],
		(unsigned char)wrq.u.ap_addr.sa_data[4],(unsigned char)wrq.u.ap_addr.sa_data[5]); 
	}
	else 
	{
		if( (argc == 4)&&(!strcmp(argv[1], "display"))&&(!strcmp(argv[2], "-i")))
            	{
			memset(data, 0x00, WLAN_SYS_LEN);
			sprintf(wrq.ifr_name,"%s%s",INIC_INTERFACE, argv[3]);
			wrq.u.data.length = 255;
                     wrq.u.data.pointer = data;
			wrq.u.data.flags = 0;
			ioctl(socket_id, SIOCGIFHWADDR , &wrq);
			printf("BSSID: \"%02X:%02X:%02X:%02X:%02X:%02X\"\n", 
			(unsigned char)wrq.u.ap_addr.sa_data[0],(unsigned char)wrq.u.ap_addr.sa_data[1],
			(unsigned char)wrq.u.ap_addr.sa_data[2],(unsigned char)wrq.u.ap_addr.sa_data[3],
		       (unsigned char)wrq.u.ap_addr.sa_data[4],(unsigned char)wrq.u.ap_addr.sa_data[5]); 
        	}
            	else
            	{	
            		printf("\r\nUsage:wlanphxcmd bssid display [-i ssidindex]\n");
               	close(socket_id);	
                	return FAIL;	
            	}
        }
        return SUCCESS;
}

static int doWlanSsid(int argc, char *argv[], void *p)
{
  	int     		socket_id;
   	 char 	ucInterface[WLAN_8_LEN];
   	struct iwreq wrq;
    	char      ucname[WLAN_32_LEN],data[WLAN_SYS_LEN];

	showInfo(argc,argv,p);
	if(((argc == 2) && (!strcmp(argv[1], "set") ))||(argc < 2))
	{
		printf("\r\n Usage:wlanphxcmd ssid set [-i ssidindex] {ssid value}");
		printf("\r\n Usage:wlanphxcmd ssid display [-i ssidindex]\n");
		return FAIL;
	}
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);	

	if((argc == 3) &&(!strcmp(argv[1], "set")))
	{
		if(strlen(argv[2]) > 32 || strlen(argv[2]) < 1)
		{
			printf("1 <= The valid length of SSID <= 32\n");
			return FAIL;
		}
		iwpriv_set(socket_id, INIC_MAINIFNAME, "SSID", argv[2]);
	}
	else if((argc == 5)&&(!strcmp(argv[1], "set"))&&(!strcmp(argv[2], "-i")))
	{
       
		memset(ucInterface, 0x00, WLAN_8_LEN);
               sprintf(ucInterface, "%s%s", INIC_INTERFACE, argv[3]);
		if(strlen(argv[4]) > 32 || strlen(argv[4]) < 1)
		{
			printf("1 <= The valid length of SSID <= 32\n");
			return FAIL;
		}
		iwpriv_set(socket_id, ucInterface, "SSID", argv[4]);			
	}
       else 
	{
		if((argc == 2)&& (!strcmp(argv[1], "display")))
            	{
               	memset(data, 0x00, WLAN_SYS_LEN);
                	strcpy(wrq.ifr_name, INIC_MAINIFNAME);
                	wrq.u.data.length = 255;
                	wrq.u.data.pointer = data;
                	wrq.u.data.flags = 0;
                	ioctl(socket_id, SIOCGIWESSID , &wrq);
                	printf("\nSSID[%s]: %s\n", INIC_MAINIFNAME , wrq.u.essid.pointer);
            }
            else 
            {
			if((argc == 4)&&(!strcmp(argv[1], "display"))&&(!strcmp(argv[2], "-i")))
               	{
                    		memset(ucname, 0x00, WLAN_32_LEN);
                    		sprintf(ucname, "%s%s", INIC_INTERFACE, argv[3]);           
                    		memset(data, 0x00, WLAN_SYS_LEN);
                    		strcpy(wrq.ifr_name, ucname);
                    		wrq.u.data.length = 255;
                    		wrq.u.data.pointer = data;
                    		wrq.u.data.flags = 0;
                    		ioctl(socket_id, SIOCGIWESSID , &wrq);
                    		printf("\nSSID[%s]: %s\n", ucname , wrq.u.essid.pointer);
                	}	
                	else
                	{	
                		printf("\r\n Usage:wlanphxcmd ssid set [-i ssidindex] {ssid value}");
				printf("\r\n Usage:wlanphxcmd ssid display [-i ssidindex]\n");
                    		close(socket_id);	
                    		return FAIL;	
                	}	
            }
        }

    	close(socket_id);	
	return SUCCESS;
}

static int doWlanHide(int argc, char *argv[], void *p)
{
	int     socket_id;
	char 	ucInterface[WLAN_8_LEN];
    	char    data[255];
    	struct iwreq wrq;
    	char acCmd[132];
    	char ssid[132];

	memset(acCmd, 0, sizeof(acCmd));
	memset(ssid, 0, sizeof(ssid));
	
    	showInfo(argc,argv,p);
	if(((argc == 2) && (!strcmp(argv[1], "set")))||(argc < 2))
	{
		printf("\r\n Usage:wlanphxcmd hide set [-i ssidindex] {enable|disable}\n");
		return FAIL;
	}	
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);	

	if((argc == 3)&& (!strcmp(argv[1], "set")))
	{
		if(!strcmp(argv[2], "enable"))
			strcpy(argv[2],"1");
		else if(!strcmp(argv[2], "disable"))
			strcpy(argv[2],"0");
		else
		{
			printf("\r\nvalue should be enable or disable[3]\n");
			return  FAIL;
		}
		/* note:
		1.do up interface first ,becouse hide must be set when interface is up*/ 
		#if 0
		sprintf(acCmd,"ifconfig %s up",INIC_MAINIFNAME);
		system(acCmd);
		#endif
		iwpriv_set(socket_id, INIC_MAINIFNAME, "HideSSID", argv[2]);	
		/* 
		2.do set ssid after setting, to let it know which ssid will be hide*/
		sprintf(ucInterface, "%s",INIC_MAINIFNAME);
        	setSSID(ucInterface);
	}
	else if((argc == 5)&&(!strcmp(argv[1], "set"))&&(!strcmp(argv[2], "-i")))
	{
		memset(ucInterface, 0x00, WLAN_8_LEN);
		sprintf(ucInterface, "%s%s",INIC_INTERFACE,argv[3]);
		if(!strcmp(argv[4] , "enable"))
			strcpy(argv[4],"1");
		else if(!strcmp(argv[4] , "disable"))
			strcpy(argv[4],"0");
		else
		{
			printf("\r\nvalue should be enable or disable[5]\n");
			return  FAIL;
		}
		#if 0        
            	sprintf(acCmd, "ifconfig %s up", ucInterface);     
            	system(acCmd);  
		#endif
		iwpriv_set(socket_id, ucInterface, "HideSSID", argv[4]);	
		//need to set ssid at last!
 		setSSID(ucInterface);
 		#if 0
		/* do down interface*/             
            sprintf(acCmd, "ifconfig %s down", ucInterface);     
            system(acCmd);

		/* do up interface ,becouse other option must be set when interface is up*/             
            sprintf(acCmd, "ifconfig %s up", ucInterface);     
            system(acCmd);
		#endif
	}
    	else
	{
		printf("\r\n Usage:wlanphxcmd hide set [-i ssidindex] {hide value}\n");
  	    	close(socket_id);	
		return FAIL;	
	}
		
    	close(socket_id);	
	return SUCCESS;
}

static int doWlanAssoclist(int argc, char *argv[], void *p)
{
	int socket_id;
	int ret, Apindex = 0;
	char data[2048] = {0};
  	struct iwreq wrq;
	RT_802_11_MAC_TABLE *mp;
	int i,j;
	
	showInfo(argc,argv,p);
	if(argc < 4)
	{
		printf("\r\nUsage:wlanphxcmd assoclist display [-i ssidindex]\n");
		return FAIL;
	}
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);
	if((argc == 4)&&(!strcmp(argv[1], "display")))
	{		
		if(strcmp(argv[2], "-i") == 0)
		{       
            		if(!strcmp(argv[3], "0"))
                		Apindex=0;
            		if(!strcmp(argv[3], "1"))
                		Apindex=1;
            		if(!strcmp(argv[3], "2"))
                		Apindex=2;
            		if(!strcmp(argv[3], "3"))
                		Apindex=3;
        	}
     
        	sprintf(wrq.ifr_name, "%s%s", INIC_INTERFACE,argv[3]);
	 	wrq.u.data.length = 2048;
	 	wrq.u.data.pointer = data;
	 	wrq.u.data.flags = 0;
    		ret = ioctl(socket_id, RTPRIV_IOCTL_GET_MAC_TABLE, &wrq);
        	if(ret != 0)
        	{
		    	printf("WlanCmdAssoclist ioctl open fail\n");
    	    		close(socket_id);	
			return FAIL;
        	}
		mp = (RT_802_11_MAC_TABLE *)wrq.u.data.pointer;
        	if(mp->Num == 0)
        	{
            		printf("WlanCmd:Assoclist no sta existance\n"); 
            		close(socket_id);	
            		return FAIL;
        	}
       	for(i = 0,j = 0; j < MAX_LEN_OF_MAC_TABLE; j++ )
        	{
	        #if defined(RT5392)|| defined(MT7592)
	            	if(Apindex == (int)mp->Entry[j].ApIdx)

		#else
			if(Apindex == (int)mp->Entry[j].Psm)
		#endif
	            	{
		       	if(mp->Entry[j].Aid == 0)
	                   		goto EXIT_ASSOCLIST;
	                	i++;
			       printf("%s %02X:%02X:%02X:%02X:%02X:%02X\n", "assoclist",
	                	(unsigned char)mp->Entry[j].Addr[0], (unsigned char)mp->Entry[j].Addr[1],
			       (unsigned char)mp->Entry[j].Addr[2], (unsigned char)mp->Entry[j].Addr[3],
			       (unsigned char)mp->Entry[j].Addr[4], (unsigned char)mp->Entry[j].Addr[5]);  
	            }
		}
	
        	if (0 == i)
       	{
            		printf("%s %s\n", "assoclist", "No STA Connected");
            		printf("\r\n  Total Assoclist Number:%d \r\n", i);
        	}
	}
	else
	{
		printf("\r\nUsage:wlanphxcmd assoclist display [-i ssidindex]\n");
    		close(socket_id);	
		return FAIL;
		
	}

EXIT_ASSOCLIST:
    close(socket_id);	
	return SUCCESS;
}

static int doWlanSecmode(int argc, char *argv[], void *p)
{
	int     		socket_id;
	char 	ucInterface[WLAN_8_LEN], data[WLAN_SYS_LEN];
    	char acCmd[WLAN_SYS_LEN];
    	struct iwreq wrq;
    	char ssid[132];

	showInfo(argc,argv,p);	
    	memset(ssid, 0, sizeof(ssid));
	
	if(((argc == 2) && (!strcmp(argv[1], "set")))||(argc < 2))
	{
		printf("\r\nUsage:wlanphxcmd secmode set [-i ssidindex]{open|wep|wpapsk|wpa2psk|wpapskwpa2psk}\n");
		return FAIL;
	}
		
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);	
	if((argc == 5)&&(!strcmp(argv[1],"set"))&&(!strcmp(argv[2],"-i")))
	{	
		memset(ucInterface, 0x00, WLAN_8_LEN);
		sprintf(ucInterface, "%s%s", INIC_INTERFACE, argv[3]);
		if(!strcmp(argv[4] , "wep"))
		{
			iwpriv_set(socket_id, ucInterface, "AuthMode", "wepauto");	
			iwpriv_set(socket_id, ucInterface, "EncrypType", "wep");		
		}
		else if((!strcmp(argv[4] , "wpapsk")) ||(!strcmp(argv[4] , "wpa2psk"))||(!strcmp(argv[4] , "wpapskwpa2psk")))
		{
			iwpriv_set(socket_id, ucInterface, "AuthMode", argv[4]);		
		}
#if defined(TCSUPPORT_CT_WLAN_WAPI)
		else if((!strcmp(argv[4] , "WAICERT")) || (!strcmp(argv[4] , "WAI-PSK")))
		{
			iwpriv_set(socket_id, ucInterface, "AuthMode", argv[4]);
			iwpriv_set(socket_id, ucInterface, "EncrypType", "SMS4");
		}	
#endif
		else
		{
			iwpriv_set(socket_id, ucInterface, "AuthMode", "open");	
			iwpriv_set(socket_id, ucInterface, "EncrypType", "none");
		}
	}
	else if((argc == 3)&&(!strcmp(argv[1],"set")))
	{
		memset(ucInterface, 0x00, WLAN_8_LEN);
		sprintf(ucInterface, "%s", INIC_MAINIFNAME);
		if(!strcmp(argv[2] , "wep"))
		{
			iwpriv_set(socket_id, ucInterface, "AuthMode", "wepauto");	
			iwpriv_set(socket_id, ucInterface, "EncrypType", "wep");		
		}
		else if((!strcmp(argv[2] , "wpapsk")) ||(!strcmp(argv[2] , "wpa2psk"))||(!strcmp(argv[2] , "wpapskwpa2psk")))
		{
			iwpriv_set(socket_id, ucInterface, "AuthMode", argv[2]);	
		}
#if defined(TCSUPPORT_CT_WLAN_WAPI)
		else if((!strcmp(argv[2] , "WAICERT")) || (!strcmp(argv[2] , "WAI-PSK")))
		{
			iwpriv_set(socket_id, ucInterface, "AuthMode", argv[2]);
			iwpriv_set(socket_id, ucInterface, "EncrypType", "SMS4");
		}	
#endif
		else
		{
			iwpriv_set(socket_id, ucInterface, "AuthMode", "open");	
			iwpriv_set(socket_id, ucInterface, "EncrypType", "none");
		}
	}
	else
	{
		printf("\r\nUsage:wlanphxcmd secmode set [-i ssidindex]{open|wep|wpapsk|wpa2psk|wpapskwpa2psk}\n");
		close(socket_id);	
		return FAIL;
	}
  	//set the ssid at last
       setSSID(ucInterface);
	return SUCCESS;	
}

static int doWlanWepkey(int argc, char *argv[], void *p)
{
	int     	socket_id;
	char 	ucInterface[WLAN_8_LEN], ucKeyNumber[WLAN_8_LEN];

	showInfo(argc,argv,p);
	if(((argc == 2) && (!strcmp(argv[1], "set")))||(argc < 2))
	{
		printf("\r\nUsage:wlanphxcmd wepkey set [-i ssidindex] [-k keyindex]{key value}\n");
		return FAIL;
	}
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);	

	if((argc == 5)&&(!strcmp(argv[1], "set")))
	{
		if(!strcmp(argv[2], "-k"))
		{
			memset(ucKeyNumber, 0x00, WLAN_8_LEN);
			sprintf(ucKeyNumber, "Key%s", argv[3]);		
			iwpriv_set(socket_id, INIC_MAINIFNAME, ucKeyNumber, argv[4]);				
		}
		else
		{
			printf("\r\nUsage:wlanphxcmd wepkey set [-i ssidindex] [-k keyindex]{key value}\n");
    			close(socket_id);	
			return FAIL;
		}		
	}
	else if((argc == 7)&&(!strcmp(argv[1], "set"))&&(!strcmp(argv[2], "-i"))&&(!strcmp(argv[4], "-k")))
	{
		memset(ucInterface, 0x00, WLAN_8_LEN);
            	sprintf(ucInterface, "%s%s", INIC_INTERFACE, argv[3]);         
		memset(ucKeyNumber, 0x00, WLAN_8_LEN);
		sprintf(ucKeyNumber, "Key%s", argv[5]);		
		iwpriv_set(socket_id, ucInterface, ucKeyNumber, argv[6]);						
	}
	else
	{
		printf("\r\nUsage:wlanphxcmd wepkey set [-i ssidindex] [-k keyindex]{key value}\n");
    		close(socket_id);	
		return FAIL;	
	}

	close(socket_id);	
	return SUCCESS;
}

static int doWlanWepkeyindex(int argc, char *argv[], void *p)
{
	int     		socket_id;
	char 	ucInterface[WLAN_8_LEN];
    	struct iwreq wrq;

	showInfo(argc,argv,p);
	if(((argc == 2) && (!strcmp(argv[1], "set") ))||(argc < 2))
	{
		printf("\r\nUsage:wlanphxcmd wepkeyindex set [-i ssidindex]{key number}\n");
		return FAIL;
	}	
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);	

	if((argc == 3)&&(!strcmp(argv[1], "set")))
	{
		sprintf(ucInterface, "%s", INIC_MAINIFNAME);    
		iwpriv_set(socket_id, ucInterface, "DefaultKeyID", argv[2]);	
	}
	else if((argc == 5)&&(!strcmp(argv[1], "set"))&&(!strcmp(argv[2], "-i")))
	{
		memset(ucInterface, 0x00, WLAN_8_LEN);
            	sprintf(ucInterface, "%s%s", INIC_INTERFACE, argv[3]);                 		
		iwpriv_set(socket_id, ucInterface, "DefaultKeyID", argv[4]);				
	}
	else
	{
		printf("\r\nUsage:wlanphxcmd wepkeyindex set [-i ssidindex]{key number}\n");
    		close(socket_id);	
		return FAIL;	
	}

	//set the ssid at last
       setSSID(ucInterface);
    	close(socket_id);	
	return SUCCESS;
}

static int doWlanPskkey(int argc, char *argv[], void *p)
{
	int     		socket_id;
	char 	ucInterface[WLAN_8_LEN];
    	char 	acCmd[WLAN_64_LEN];
    	struct iwreq wrq;

	showInfo(argc,argv,p);
	if(((argc == 2) && (!strcmp(argv[1], "set")))||(argc < 2))
	{
		printf("\r\nUsage:wlanphxcmd pskkey set [-i ssidindex]{key number}\n");
		return FAIL;
	}
		
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);	
	memset(ucInterface, 0x00, WLAN_8_LEN);
	memset(acCmd, 0x00, WLAN_64_LEN);
	
	if((argc == 3)&&(!strcmp(argv[1], "set")))
	{
		sprintf(ucInterface, "%s", INIC_MAINIFNAME);     	
		setSSID(ucInterface);
		iwpriv_set(socket_id, INIC_MAINIFNAME, "WPAPSK", argv[2]);
		sprintf(acCmd, "iwpriv %s set DefaultKeyID=2",INIC_MAINIFNAME);//DefaultKeyID must be 2 when mode is wpapsk or wpa2psk
		system(acCmd);		
	}
	else if((argc == 5)&&(!strcmp(argv[1], "set"))&&(!strcmp(argv[2], "-i")))
	{
		//according to ralink's releasenote
		sprintf(ucInterface, "%s%s", INIC_INTERFACE, argv[3]);     				
  			setSSID(ucInterface);
		iwpriv_set(socket_id, ucInterface, "WPAPSK", argv[4]);
		sprintf(acCmd, "iwpriv %s set DefaultKeyID=2", ucInterface);
		system(acCmd);
	}
	else
	{
		printf("\r\nUsage:wlanphxcmd pskkey set [-i ssidindex]{key number}\n");
		close(socket_id);	
		return FAIL;
	}

	//set the ssid at last
	setSSID(ucInterface);
    	close(socket_id);	
	return SUCCESS;	
}

static int doWlanWpaencryption(int argc, char *argv[], void *p)
{
	int     		socket_id;
	char 	ucInterface[WLAN_8_LEN];

	showInfo(argc,argv,p);
	if(((argc == 2) && (!strcmp(argv[1], "set")))||(argc < 2))
	{
		printf("\r\nUsage:wlanphxcmd wpaencryption set [-i ssidindex]{aes|tkip|tkipaes}\n");
		return FAIL;
	}
	
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);	
	
	if((argc == 3)&&(!strcmp(argv[1], "set")))
	{
		if(!strcmp(argv[2], "tkipaes"))
        	{
            		strcpy(argv[2] , "TKIPAES");
        	}
        	else if(!strcmp(argv[2], "tkip"))
       	{
            		strcpy(argv[2] , "TKIP");
        	}
        	else if(!strcmp(argv[2], "aes"))
        	{
            		strcpy(argv[2] , "AES");
        	}
#if defined(TCSUPPORT_CT_WLAN_WAPI)
			else if(!strcmp(argv[2], "SMS4"))
			{
					strcpy(argv[2] , "SMS4");
			}
#endif
        	else
        	{
            		printf("\r\n parse error   \r\n");
            		return FAIL;
        	}
		iwpriv_set(socket_id, INIC_MAINIFNAME, "EncrypType", argv[2]);
	}
	else if((argc == 5)&&(!strcmp(argv[1], "set"))&&(!strcmp(argv[2], "-i")))
	{
    		if(!strcmp(argv[4], "tkipaes"))
    		{
        		strcpy(argv[4] , "TKIPAES");
    		}
    		else if(!strcmp(argv[4], "tkip") )
    		{
       	 	strcpy(argv[4] , "TKIP");
    		}
    		else if(strcmp(argv[4], "aes") == 0)
    		{
        		strcpy(argv[4] , "AES");
    		}
#if defined(TCSUPPORT_CT_WLAN_WAPI)
			else if(!strcmp(argv[4], "SMS4"))
			{
					strcpy(argv[4] , "SMS4");
			}
#endif
   	 	else
    		{
        		printf("\r\n parse error   \r\n");
        		return FAIL;
		}

		memset(ucInterface, 0x00, WLAN_8_LEN);
   		sprintf(ucInterface, "%s%s", INIC_INTERFACE, argv[3]);			
		iwpriv_set(socket_id, ucInterface, "EncrypType", argv[4]);			
	}
	else
	{
		printf("\r\nUsage:wlanphxcmd wpaencryption set [-i ssidindex]{aes|tkip|tkipaes}\n");
		close(socket_id);	
		return FAIL;
	}

    	close(socket_id);	
	return SUCCESS;
}

static int doWlanHelp(int argc, char *argv[], void *p)
{
	printf("\r\nwlanphxcmd version: %s",MODULE_VERSION_WLANCMD);
	printf("\r\nUsage:");
	printf("\r\n\twlanphxcmd cmdver display");
	printf("\r\n\twlanphxcmd driverver display");
	printf("\r\n\twlanphxcmd firmwarever display");
	printf("\r\n\twlanphxcmd up [-i ssidindex]");
	printf("\r\n\twlanphxcmd down [-i ssidindex]");
	printf("\r\n\twlanphxcmd mode set {bonly|gonly|nonly|mixed|gnmixed|bgnmixed}");
	//printf("\r\n\twlanphxcmd rate set {rate value}");	
	//printf("\r\n\twlanphxcmd rateset [all | default | set {arbitrary rateset}]");
	printf("\r\n\twlanphxcmd channel set {channel value}");
	printf("\r\n\twlanphxcmd region set {region value}");
	printf("\r\n\twlanphxcmd phytype display");
	printf("\r\n\twlanphxcmd bssid display [-i ssidindex]");
	printf("\r\n\twlanphxcmd ssid set [-i ssidindex] {ssid value}");
	printf("\r\n\twlanphxcmd hide set [-i ssidindex] {enable|disable}");
	printf("\r\n\twlanphxcmd assoclist display [-i ssidindex]");
	printf("\r\n\twlanphxcmd secmode set [-i ssidindex]{open|wep|wpapsk|wpa2psk|wpapskwpa2psk}");
	printf("\r\n\twlanphxcmd wepkey set [-i ssidindex] [-k keyindex]{key value}");
	printf("\r\n\twlanphxcmd wepkeyindex set [-i ssidindex]{key number}");
	printf("\r\n\twlanphxcmd pskkey set [-i ssidindex]{key number}");
	printf("\r\n\twlanphxcmd wpaencryption set [-i ssidindex]{aes|tkip|tkipaes}");
	printf("\r\n\twlanphxcmd fltmacctl set {ctl value}");
	printf("\r\n\twlanphxcmd fltmacmode set  [-i ssidindex]{allow|deny}");
	printf("\r\n\twlanphxcmd fltmac set [-i ssidindex]{mac value}");
	printf("\r\n\twlanphxcmd assocctl set [-i ssidindex]{devicenum}");
	//printf("\r\n\twlanphxcmd mbss set [-n num] {enable|disable}");
	printf("\r\n\twlanphxcmd bgprotection set {0:auto|1:always on|always off}\n");
	printf("\r\n\twlanphxcmd rtsthreshold set {value(1~2347)}");
	printf("\r\n\twlanphxcmd fragthreshold set {value(256~2346)}");
	printf("\r\n\twlanphxcmd txburst set {1:Enable|0:disable}");
	printf("\r\n\twlanphxcmd dtimperiod set {value(1~255)}");
	printf("\r\n\twlanphxcmd wmm set {1:enable|0:disable}");
	printf("\r\n\twlanphxcmd wps_mode set {ap-pin|ap-pbc}");
	printf("\r\n\twlanphxcmd wps_ctl set {enabled|disabled}");
	printf("\r\n\twlanphxcmd wps_pinval set {value(8 digit)}");																																		
	printf("\r\n\twlanphxcmd wps_switch set {enabled|disabled}");
	printf("\r\n\twlanphxcmd wps_conf set {configured|unconfigured}");
	printf("\r\n\twlanphxcmd ap_isolate set [-i ssidindx] {0:disable|1:enable}");
	printf("\r\n\twlanphxcmd HT_GI set {short | long}");	
	printf("\r\n\twlanphxcmd HT_BW set {20 | 20/40}");
	printf("\r\n\twlanphxcmd HT_MCS set {value}\n");

	return 0;
}

static int doWlanFltmacctl(int argc, char *argv[], void *p)
{
	int     		socket_id;
	char 	ucInterface[WLAN_8_LEN];
    	int Loop = 0;
    	char acCmd[132];
    
    	memset(acCmd, 0x00, 132);
    	memset(ucInterface, 0x00, WLAN_8_LEN);

	showInfo(argc,argv,p);
   	 /* do clear maclist first*/
    	for (Loop = 0; Loop < 4; Loop++)
	{
		sprintf(ucInterface,"%s%d",INIC_INTERFACE,Loop);
           	sprintf(acCmd, "iwpriv %s set ACLClearAll=1", ucInterface);
            	system(acCmd);
	}
		
	if(((argc == 2) && (!strcmp(argv[1], "set")))||(argc < 2))
	{
		printf("\r\nUsage:wlanphxcmd fltmacctl set {ctl value}\n");
		return FAIL;
	}
	
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);	

	if((argc == 3)&&(!strcmp(argv[1], "set")))
	{
		if(!strcmp(argv[2],"enable"))	//Allow
			strcpy(argv[2] , "1");
		if(!strcmp(argv[2],"disable"))	//deny
			strcpy(argv[2] , "0");
		iwpriv_set(socket_id, INIC_MAINIFNAME, "AccessPolicy", argv[2]);	
	}
	else if((argc == 5)&&(!strcmp(argv[1], "set"))&&(!strcmp(argv[2], "-i")))
	{
		memset(ucInterface, 0x00, WLAN_8_LEN);
		sprintf(ucInterface,"%s%s",INIC_INTERFACE, argv[3]);	
		if(!strcmp(argv[4],"enable"))	//Allow
			strcpy(argv[4] , "1");
		if(!strcmp(argv[4],"disable"))	//deny
			strcpy(argv[4] , "0");
		iwpriv_set(socket_id, ucInterface, "AccessPolicy", argv[4]);	
}
	else
	{
		printf("\r\nUsage:wlanphxcmd fltmacctl set {ctl value}\n");
		close(socket_id);	
		return FAIL;
	}
	
  	close(socket_id);	
	return SUCCESS;
}

static int doWlanFltmacmode(int argc, char *argv[], void *p)
{
	int     		socket_id;
	char 	ucInterface[WLAN_8_LEN];	

	showInfo(argc,argv,p);
	if(((argc == 2) && (!strcmp(argv[1], "set")))||(argc < 2))
	{
		printf("\r\nUsage:wlanphxcmd fltmacmode set  [-i ssidindex]{allow|deny}\n");
		return FAIL;
	}	
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);	

	if( (argc == 3)&&(!strcmp(argv[1], "set")))
	{
		if(!strcmp(argv[2],"allow"))		//Allow
				strcpy(argv[2] , "1");
		if(!strcmp(argv[2],"deny"))			//deny
				strcpy(argv[2] , "2");
		iwpriv_set(socket_id, INIC_MAINIFNAME, "AccessPolicy", argv[2]);	
	}
	else if((argc == 5)&&(!strcmp(argv[1], "set"))&&(!strcmp(argv[2], "-i")))
	{
		memset(ucInterface, 0x00, WLAN_8_LEN);
		sprintf(ucInterface,"%s%s",INIC_INTERFACE, argv[3]);	
		if(!strcmp(argv[4],"allow"))	//Allow
			strcpy(argv[4] , "1");
		if(!strcmp(argv[4],"deny"))		//deny
			strcpy(argv[4] , "2");
		iwpriv_set(socket_id, ucInterface, "AccessPolicy", argv[4]);	
	}
	else
	{
		printf("\r\nUsage:wlanphxcmd fltmacmode set  [-i ssidindex]{allow|deny}\n");
		close(socket_id);	
		return FAIL;
	}
	
  	close(socket_id);	
	return SUCCESS;
}

static int doWlanFltmac(int argc, char *argv[], void *p)
{
	int     		socket_id;
	char 			ucInterface[WLAN_32_LEN];
	char                    MACAddressList[WLAN_SYS_LEN];

	showInfo(argc,argv,p);
	if(((argc == 2) && (!strcmp(argv[1], "set")))||(argc < 2))
	{
		printf("\r\nUsage:wlanphxcmd fltmac set [-i ssidindex]{mac value}\n");
		return FAIL;
	}
	
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);	
 
	if( (argc == 3)&&(!strcmp(argv[1], "set")))
	{	
		memset(MACAddressList, 0x00, WLAN_SYS_LEN);
		sprintf(MACAddressList,"%s", argv[2]);
		iwpriv_set(socket_id, INIC_MAINIFNAME, "ACLAddEntry", MACAddressList);
	}
	else if( (argc == 5)&&(!strcmp(argv[1], "set"))&&(!strcmp(argv[2], "-i")))
	{
		memset(ucInterface, 0x00, WLAN_32_LEN);
		memset(MACAddressList, 0x00, WLAN_SYS_LEN);
		sprintf(ucInterface,"%s%s",INIC_INTERFACE, argv[3]);
		sprintf(MACAddressList,"%s", argv[4]);		
		iwpriv_set(socket_id, ucInterface, "ACLAddEntry", MACAddressList);	
	}
	else
	{
		printf("\r\nUsage:wlanphxcmd fltmac set [-i ssidindex]{mac value}\n");
		close(socket_id);	
		return FAIL;
	}

  	close(socket_id);	
	return SUCCESS;
}

static int doWlanAssocctl(int argc, char *argv[], void *p)
{
  	int     		socket_id;
	char 	ucInterface[WLAN_8_LEN];	

	showInfo(argc,argv,p);
	if(((argc == 2) && (!strcmp(argv[1], "set")))||(argc < 2))
	{
		printf("\r\nUsage:wlanphxcmd assocctl set [-i ssidindex]{devicenum}\n");
		return FAIL;
	}
	
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);	

	if((argc == 3)&&(!strcmp(argv[1], "set")))
	{
		memset(ucInterface, 0x00, WLAN_8_LEN);
		strcpy(ucInterface, INIC_MAINIFNAME);
		iwpriv_set(socket_id, ucInterface, "MaxStaNum", argv[2]);					
	}
	else if((argc == 5)&&(!strcmp(argv[1], "set"))&&(!strcmp(argv[2], "-i")))
	{
		memset(ucInterface, 0x00, WLAN_8_LEN);
		sprintf(ucInterface,"%s%s", INIC_INTERFACE,argv[3]);
		iwpriv_set(socket_id, ucInterface, "MaxStaNum", argv[4]);					
	}
	else
	{
		printf("\r\nUsage:wlanphxcmd assocctl set [-i ssidindex]{devicenum}\n");
		close(socket_id);	
		return FAIL;
	}	

   	close(socket_id);	
  	return SUCCESS;
}

#if 1
static int doWlanMBss(int argc, char *argv[], void *p)
{
  	char cmd[WLAN_64_LEN] = {0};
	int i,number = 0;

	showInfo(argc,argv,p);
  	if((argc == 5) && (!strcmp(argv[1], "set"))&&(!strcmp(argv[2],"-n")))
	{
		number = atoi(argv[3]);
		printf("\r\nthe number is %d",number);
		#if 1
		sprintf(cmd, "echo Default > %s",WLANDATPATH);
       	system(cmd);
		sprintf(cmd, "echo BssidNum=%s >> %s", argv[3],WLANDATPATH);
		system(cmd);
		#endif
		for(i = 0; i < number; i++)
		{
			memset(cmd, 0 , sizeof(cmd));
    	              sprintf(cmd, "ifconfig %s%d down", INIC_INTERFACE,i);
    	              system(cmd);
		}

		#ifdef RT5392
		sprintf(cmd, "rmmod rt5390ap.ko");
       		system(cmd);
		sprintf(cmd, "insmod /lib/modules/rt5390ap.ko");
       		system(cmd);
		#elif MT7592
		sprintf(cmd, "rmmod mt7603eap.ko");
			system(cmd);
		sprintf(cmd, "insmod /lib/modules/mt7603eap.ko");
			system(cmd);

		#else
		sprintf(cmd, "rmmod rt3390ap");
       		system(cmd);
		sprintf(cmd, "insmod /lib/modules/rt3390ap.ko");
       		system(cmd);
		#endif
		
		if(!strcmp(argv[4],"enable"))
		{
			for(i = 0; i < number; i++)
			{
				memset(cmd, 0 , sizeof(cmd));
        	              sprintf(cmd, "ifconfig %s%d up", INIC_INTERFACE,i);
        	              system(cmd);
				sprintf(cmd, "brctl addif br0 %s%d", INIC_INTERFACE,i);
        	              system(cmd);	
			}
		}
		else
		{
			memset(cmd, 0 , sizeof(cmd));
			sprintf(cmd, "ifconfig %s up", INIC_MAINIFNAME);
			system(cmd);
			sprintf(cmd, "brctl addif br0 %s", INIC_MAINIFNAME);
			system(cmd);	
		}
	}
	else
	{
		printf("\r\nUsage:wlanphxcmd mbss set [-n num] {enable|disable}\n");
		return FAIL;
	}
	
    	return SUCCESS;
}

#endif

static int doWlanBgprotection(int argc, char *argv[], void *p)
{
	int socket_id;
	
	showInfo(argc,argv,p);
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);	
	if((argc == 3) && (!strcmp(argv[1],"set"))&& ((argv[2][0]<=0x3A)&&(argv[2][0]>=0x30)))
	{
		iwpriv_set(socket_id, INIC_MAINIFNAME, "BGProtection", argv[2]);	
		close(socket_id);	
		return SUCCESS;
	}
	else
	{
		printf("\r\nUsage:wlanphxcmd bgprotection set {0:auto|1:always on|always off}\n");
		return FAIL;		
	}

}

static int doWlanRtsthreshold(int argc, char *argv[], void *p)
{
	int socket_id;
	showInfo(argc,argv,p);
	if((argc == 3) &&(!strcmp(argv[1],"set"))&& ((argv[2][0]<=0x3A)&&(argv[2][0]>=0x30)))
	{
		socket_id = socket(AF_INET, SOCK_DGRAM, 0);	
		iwpriv_set(socket_id, INIC_MAINIFNAME, "RTSThreshold", argv[2]);
    		close(socket_id);	
		return SUCCESS;	
	}
	else
	{
		printf("\r\nUsage:wlanphxcmd rtsthreshold set {value(1~2347)}\n");
		return FAIL;		
	}
}

static int doWlanBeaconperiod(int argc, char *argv[], void *p)
{
	int socket_id;
	showInfo(argc,argv,p);
	if((argc == 3) &&(!strcmp(argv[1],"set"))&& ((argv[2][0]<=0x3A)&&(argv[2][0]>=0x30)))
	{
		socket_id = socket(AF_INET, SOCK_DGRAM, 0);	
		iwpriv_set(socket_id, INIC_MAINIFNAME, "BeaconPeriod", argv[2]);
    		close(socket_id);	
		return SUCCESS;	
	}
	else
	{
		printf("\r\nUsage:wlanphxcmd beaconperiod set {value(20~1024)}\n");
		return FAIL;		
	}
}

static int doWlanFragthreshold(int argc, char *argv[], void *p)
{
	int socket_id;
	showInfo(argc,argv,p);
	if((argc == 3) && (!strcmp(argv[1],"set"))&& ((argv[2][0]<=0x3A)&&(argv[2][0]>=0x30)))
	{
		socket_id = socket(AF_INET, SOCK_DGRAM, 0);	
		iwpriv_set(socket_id, INIC_MAINIFNAME, "FragThreshold", argv[2]);
    		close(socket_id);	
		return SUCCESS;	
	}
	else
	{
		printf("\r\nUsage:wlanphxcmd fragthreshold set {value(256~2346)}\n");
		return FAIL;		
	}
}

static int doWlantxBurst(int argc, char *argv[], void *p)
{
	int socket_id;
	showInfo(argc,argv,p);
	if((argc == 3) && (!strcmp(argv[1],"set"))&& ((argv[2][0]<=0x3A)&&(argv[2][0]>=0x30)))
	{
		socket_id = socket(AF_INET, SOCK_DGRAM, 0);	
		iwpriv_set(socket_id, INIC_MAINIFNAME, "TxBurst", argv[2]);
    		close(socket_id);	
		return SUCCESS;	
	}
	else
	{
		printf("\r\nUsage:wlanphxcmd txburst set {1:Enable|0:disable}\n");
		return FAIL;		
	}
}

static int doWlanDtimperiod(int argc, char *argv[], void *p)
{
	int socket_id;
	showInfo(argc,argv,p);
	if((argc == 3) && (!strcmp(argv[1],"set"))&& ((argv[2][0]<=0x3A)&&(argv[2][0]>=0x30)))
	{
		socket_id = socket(AF_INET, SOCK_DGRAM, 0);	
		iwpriv_set(socket_id, INIC_MAINIFNAME, "DtimPeriod", argv[2]);
    		close(socket_id);	
		return SUCCESS;	
	}
	else
	{
		printf("\r\nUsage:wlanphxcmd dtimperiod set {value(1~255)}\n");
		return FAIL;		
	}
}

static int doWlanWmm(int argc, char *argv[], void *p)
{
	int socket_id;
	int i;	
	char acCmd[132] = {0};
	showInfo(argc,argv,p);
	if((argc == 3) && (!strcmp(argv[1],"set"))&& ((argv[2][0]<=0x3A)&&(argv[2][0]>=0x30)))
	{
		socket_id = socket(AF_INET, SOCK_DGRAM, 0);	
		if(!strcmp(argv[2], "-1"))
		{
			printf("not support WMM AUTO\n");	
    			close(socket_id);	
			return FAIL;
		}
		else
		{
			if(!strcmp(argv[2], "1"))
			{
				for(i = 0; i < 4; i++)
				{
					memset(acCmd,0,sizeof(acCmd));
					sprintf(acCmd,"iwpriv %s%d set WmmCapable=1",INIC_INTERFACE,i);
					system(acCmd);
				}

				sprintf(acCmd,"iwpriv %s set TxBurst=0",INIC_MAINIFNAME);
				system(acCmd);
			}
			else
			{
				for(i = 0; i < 4; i++)
				{
					memset(acCmd,0,sizeof(acCmd));
					sprintf(acCmd,"iwpriv %s%d set WmmCapable=0",INIC_INTERFACE,i);
					system(acCmd);
				}

				sprintf(acCmd,"iwpriv %s set TxBurst=1",INIC_MAINIFNAME);
				system(acCmd);
			}
		}	
	}
	else
	{
		printf("\r\nUsage:wlanphxcmd wme set {1:enable|0:disable}\n");
		return FAIL;		
	}
	
	close(socket_id);	
	return SUCCESS;	
}

//#ifdef SUPPORT_ATP_WPS
static int doWlanWpsNvram(int argc, char *argv[], void *p)
{
	return SUCCESS;	
}

static int doWlanWpsMode(int argc, char *argv[], void *p)
{
	int     		socket_id;
	showInfo(argc,argv,p);
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);	
	if((argc == 3)&&(!strcmp(argv[1], "set")))
	{
		if(!strcmp(argv[2], "ap-pin"))
		{
			iwpriv_set(socket_id, INIC_MAINIFNAME, "WscMode", "1");//PIN mode
		}
		else if(!strcmp(argv[2], "ap-pbc"))
		{
			iwpriv_set(socket_id, INIC_MAINIFNAME, "WscMode", "2");//PBC mode
		}
		else
		{
			close(socket_id);	
			return FAIL;
		} 
	}
	else
	{
		printf("\r\nUsage:wlanphxcmd wps_mode set {ap-pin|ap-pbc}\n");
		close(socket_id);	
		return FAIL;
	}

  	
    	close(socket_id);
  	return SUCCESS;
}

static int doWlanWpsCtl(int argc, char *argv[], void *p)
{
	int     		socket_id;
       char 	        cmd[WLAN_64_LEN];
	   
	showInfo(argc,argv,p);
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);	
	if((argc == 3)&&(!strcmp(argv[1], "set")))
	{
		if(!strcmp(argv[2], "enabled"))
		{
            		iwpriv_set(socket_id, INIC_MAINIFNAME, "WscConfMode", "7");//according to releasenode 62~63
    			memset(cmd, 0x00, WLAN_64_LEN);
    	    		sprintf(cmd, "iwpriv %s set WscConfStatus=2",INIC_MAINIFNAME);//ap is configured		
    			system(cmd);
		}
		else if(!strcmp(argv[2], "disabled"))
		{
			iwpriv_set(socket_id, INIC_MAINIFNAME, "WscConfMode", "0");
		}
		else
		{
			close(socket_id);	
			return FAIL;
		}  
	}
	else
	{
		printf("\r\nUsage:wlanphxcmd wps_ctl set {enabled|disabled}\n");
		close(socket_id);	
		return FAIL;
	}
	
  	close(socket_id);	
    	return SUCCESS;
}

static int doWlanWpsPinval(int argc, char *argv[], void *p)
{
	int     		socket_id;
    	char 	ucInterface[WLAN_64_LEN] = {0};
	unsigned int pincode = 0;
	
	showInfo(argc,argv,p);
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);	
	if((argc == 3)&&(!strcmp(argv[1], "set")))
	{
		iwpriv_set(socket_id, INIC_MAINIFNAME, "WscPinCode", argv[2]);
		//sprintf(ucInterface, "iwpriv %s set WscGetConf=1",INIC_MAINIFNAME);
    		//system(ucInterface);
	}
	else if((argc == 2)&&(!strcmp(argv[1], "display")))
	{
		if (OidQueryInformation(RT_OID_WSC_PIN_CODE, socket_id, INIC_MAINIFNAME, &pincode, 0) < 0)
		{
			printf("ERROR:RT_OID_VERSION_INFO\n");
		}
		else
		{
			printf("self pincode:%d\n", pincode);
		}
	}
	else
	{
		printf("\r\nUsage:wlanphxcmd wps_pinval set {value(8 digit)}");
		printf("\r\nUsage:wlanphxcmd wps_pinval display\n");
		close(socket_id);	
		return FAIL;
	}
	
  	close(socket_id);	
  	return SUCCESS;
}

static int doWlanWpsSwitch(int argc, char *argv[], void *p)
{
	int     		socket_id;
    	char 	ucInterface[WLAN_64_LEN] = {0};

	showInfo(argc,argv,p);
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);	
	if((argc == 3)&&(!strcmp(argv[1], "set")))
	{
		if(!strcmp(argv[2], "enabled"))
		{
			sprintf(ucInterface, "iwpriv %s set WscGetConf=1",INIC_MAINIFNAME);//start wps
    			system(ucInterface);
		}
		else	if(!strcmp(argv[2], "disabled"))
		{
			sprintf(ucInterface, "iwpriv %s set WscStop",INIC_MAINIFNAME);//Stop wps when wps is runing
    			system(ucInterface);
		}
		else
		{
			close(socket_id);	
			return FAIL;
		}
	}
	else
	{
		printf("\r\nUsage:wlanphxcmd wps_switch set {enabled|disabled}\n");
		close(socket_id);	
		return FAIL;
	}
	
  	close(socket_id);	
  	return SUCCESS;
}

static int doWlanWpsConf(int argc, char *argv[], void *p)
{
	int     		socket_id;
    	char 	ucInterface[WLAN_64_LEN] = {0};

	showInfo(argc,argv,p);
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);	
	if((argc == 3)&&(!strcmp(argv[1], "set")))
	{
		if(!strcmp(argv[2], "configured"))
		{
			sprintf(ucInterface, "iwpriv %s set WsConfStatus=2",INIC_MAINIFNAME);//only send
    			system(ucInterface);
		}
		else	if(!strcmp(argv[2], "unconfigured"))
		{
			sprintf(ucInterface, "iwpriv %s set WsConfStatus=1",INIC_MAINIFNAME);//both send and receives
    			system(ucInterface);
		}
		else
		{
			close(socket_id);	
			return FAIL;
		}
	}
	else
	{
		printf("\r\nUsage:wlanphxcmd wps_conf set {configured|unconfigured}\n");
		close(socket_id);	
		return FAIL;
	}
	
  	close(socket_id);	
  	return SUCCESS;
}
//#endif

static int doWlanApIsolate(int argc, char *argv[], void *p)
{
	int     		socket_id;
	char 	ucInterface[5];

	showInfo(argc,argv,p);
	if((argc == 5)&&(!strcmp(argv[1], "set"))&&(!strcmp(argv[2], "-i")))
	{
		socket_id = socket(AF_INET, SOCK_DGRAM, 0);		
		memset(ucInterface, 0x00, 5);
		sprintf(ucInterface, "%s%s",INIC_INTERFACE,argv[3]);
		iwpriv_set(socket_id, ucInterface, "NoForwarding", argv[4]);	
		close(socket_id);	
		return SUCCESS;	
	}
	else
	{
		printf("\r\nUsage:wlanphxcmd ap_isolate set [-i ssidindx] {0:disable|1:enable}\n");
		return FAIL;		
	}	
		
}

static int doWlanHTGI(int argc, char *argv[], void *p)
{
	int     		socket_id;
    	char acCmd[132] = {0};

	showInfo(argc,argv,p);
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);	
	if((argc == 3) && (!strcmp(argv[1], "set")))
	{	
		if(!strcmp(argv[2], "short"))
			strcpy(argv[2], "1");
		else if (!strcmp(argv[2], "long"))
			strcpy(argv[2], "0");
		else
		{
			close(socket_id);	
			return FAIL;
		}
		iwpriv_set(socket_id, INIC_MAINIFNAME, "HtGi", argv[2]);
		#if 0
		sprintf(acCmd, "echo HT_GI=%s >> %s", argv[2],WLANDATPATH);
             	system(acCmd);
		#endif
			
	}
	else
	{
		printf("\r\nUsage:wlanphxcmd HT_GI set {short | long}\n");
		close(socket_id);	
		return FAIL;
	}
	
  	close(socket_id);	
  	return SUCCESS;
}

static int doWlanHTBW(int argc, char *argv[], void *p)
{
	int     		socket_id;
	int iWMIndex = 0;
	char acCmd[132] = {0};

	showInfo(argc,argv,p);
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);	
	if((argc == 3)&&(!strcmp(argv[1], "set")))
	{	
		if(!strcmp(argv[2], "20"))
		{
			strcpy(argv[2], "0");
		}
		else if (!strcmp(argv[2], "20/40"))
		{
			strcpy(argv[2], "1");
			#if 0
                	sprintf(acCmd, "echo HT_BW=1 >> %s",WLANDATPATH);
                	system(acCmd);
					#endif
		}
		else
		{
			close(socket_id);	
			return FAIL;
		}
		iwpriv_set(socket_id, INIC_MAINIFNAME, "HtBw", argv[2]);	
		sprintf(acCmd,"iwpriv %s set HtAutoBa=1",INIC_MAINIFNAME);
        	system(acCmd);
        #if 0
       	for( iWMIndex = 0 ; iWMIndex < 4 ; iWMIndex++ )
        	{
              	sprintf(acCmd, "ifconfig %s%d down",INIC_INTERFACE, iWMIndex);
        		system(acCmd);
        	}
        	memset(acCmd, 0 , sizeof(acCmd));
     		for( iWMIndex = 0 ; iWMIndex < 4 ; iWMIndex++ )
        	{
              	sprintf(acCmd, "ifconfig %s%d up",INIC_INTERFACE, iWMIndex);
        		system(acCmd);
        	}
			#endif
	}
	else
	{
		printf("\r\nUsage:wlanphxcmd HT_BW set {20 | 20/40}\n");
		close(socket_id);	
		return FAIL;
	}

  	close(socket_id);	
  	return SUCCESS;
}

static int doWlanHTMCS(int argc, char *argv[], void *p)
{
  	int socket_id;
	char acCmd[132] = {0};
	
	showInfo(argc,argv,p);
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);	
	if((argc == 3)&&(!strcmp(argv[1], "set")))
	{	
		iwpriv_set(socket_id, INIC_MAINIFNAME, "HtMcs", argv[2]);
		sprintf(acCmd,"iwpriv %s set HtRdg=1",INIC_MAINIFNAME);
        	system(acCmd);
		sprintf(acCmd,"iwpriv %s set HtStbc=1",INIC_MAINIFNAME);
        	system(acCmd);
	}
	else
	{
		printf("\r\nUsage:wlanphxcmd HT_MCS set {0~7,33}\n");
		close(socket_id);
		return FAIL;
	}

  	close(socket_id);	
  	return SUCCESS;	
}

int main(int argc, char **argv) 
{
	void *p;
	int ret = -1;
	int pidfd;
	pidfd =open(WLANCMD_LOCK_FILE,O_RDWR | O_CREAT);
	if(pidfd < 0)
	{
		printf("\r\nopen lock file error!");
		ret = subcmd(WlanCmds, argc, argv, p);
	}
	else
	{
		writew_lock(pidfd,0,SEEK_SET,0);
		ret = subcmd(WlanCmds, argc, argv, p);
		un_lock(pidfd,0,SEEK_SET,0);
		//close(pidfd);
	}
	return ret;
}

