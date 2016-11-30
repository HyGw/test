#include "libapi_lib_switchmgr.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define HELP_QOS_ENABLE		"qos enable <port:0~7> <enable:1|disable:0>\n"
#define HELP_RATE_CONTROLL	"ratectl <ingress:1|exgress:0> <port: 0-6> <rate(kbps): 1-1000000)>\n"
#define HELP_PORT_ENABLE    "portenable <port:0~4> <enable:1|disable:0>\n"
#define HELP_LOOPBACK_ENABLE    "loopbackrt <port:0~4> <enable:1|disable:0>\n"
#define HELP_SPEED_MODE    "speedmode <port:0~4> <mode:1:auto-ne 2:100FD 3:100HD 4:10FD 5:10HD>\n"
#define HELP_LINK_STATE    "linkstate <port:0~4> \n"
#define HELP_FC_GLOBAL		"fcglobal <get|set> <enable:1|disable:0>"
#define HELP_FOLLOWCONTROL_ENABLE    "followcontrol <port:0~6> <enable:1|disable:0>\n"
#define HELP_LAN2LAN   "lan2lan show|<srcport:0~6><dstport:0~6><rx|tx|rxtx>\n"
#define HELP_PORT_STATISTIC    "statistic <port:0~6>/clear\n"

typedef struct {
    const char * subCmd;
    const char * desp;
    int(*cbFunc)(int argc, char ** argv);
} subCmd_t;

typedef struct {
    char *name;
    int (*func)(int argc,char *argv[],void *p);
    int flags;
    int argcmin;
    char *argc_errmsg;
} cmds_t;

static int doQosEnable(int argc, char *argv[], void *p){
	int enable = 0;
	int port = 0;

	port = atoi(argv[1]);
	enable = atoi(argv[2]);
	
	switchmgr_lib_set_port_qos(port, enable);
	
	return 0;
}


static const cmds_t qosCmds[] = {
	{"enable",		doQosEnable,		0x12,	2,	HELP_QOS_ENABLE},
	{NULL,			NULL,			0x10,	0,	NULL},
};

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

	if(argc < 3)
	{
		printf("%s", HELP_RATE_CONTROLL);
		return 0;
	}

	dir=atoi(argv[1]);
	port=atoi(argv[2]);
	if(argc > 3)
		rate=atoi(argv[3]);
	

	if(argc == 3)
	{
		switchmgr_lib_get_port_ratelimit(port, dir, &rate);
		printf("port %d %s ratelimit is %d kbps\n", port, (dir?"RX":"TX"), rate);
	}
	else
	{
		printf("set port %d %s ratelimit %d kbps\n", port, (dir?"RX":"TX"), rate);
		switchmgr_lib_set_port_ratelimit(port, rate, dir);

	}	


	return 0;
}

/*___________________________________________________________________
**      function name: doPortEnable
**      descriptions:
**
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
**    
**_______________________________________________________________
*/	
static int doPortEnable(int argc, char *argv[], void *p)
{
	u8 port=0;
	u8 enable=0;
	u8 ret = 0;

	if(argc < 2)
	{
		printf("%s", HELP_PORT_ENABLE);
		return 0;
	}

	port=atoi(argv[1]);
	if(argc > 2)
		enable=atoi(argv[2]);

	if(argc == 2)
	{
		ret = switchmgr_lib_get_port_enable(port, &enable);
		if(!ret)
			printf("port %d is %s\n", port, (enable?"enable":"disable"));
	}
	else
	{
		ret = switchmgr_lib_set_port_enable(port, enable);
		if(!ret)
			printf("set port %d %s\n", port, (enable?"enable":"disable"));

	}	


	return 0;
}

static int doLoopBackRT(int argc, char *argv[], void *p)
{
	u8 port=0;
	u8 enable=0;
	u8 ret = 0;

	if(argc < 2)
	{
		printf("%s", HELP_LOOPBACK_ENABLE);
		return 0;
	}

	port=atoi(argv[1]);
	if(argc > 2)
		enable=atoi(argv[2]);

	if(argc == 2)
	{
		ret = switchmgr_lib_get_loopback(port, &enable);
		if(!ret)
			printf("port %d  remote loopback is %s\n", port, (enable?"enable":"disable"));
	}
	else
	{
		ret = switchmgr_lib_set_loopback(port, enable);
		if(!ret)
			printf("set port %d remote loopback %s\n", port, (enable?"enable":"disable"));

	}	


	return 0;
}

static int doSpeedMode(int argc, char *argv[], void *p)
{
	u8 port=0;
	u8 mode=0;
	u8 ret = 0;

	if(argc < 2)
	{
		printf("%s", HELP_SPEED_MODE);
		return 0;
	}

	port=atoi(argv[1]);
	if(argc > 2)
		mode=atoi(argv[2]);

	if(argc == 2)
	{
		ret = switchmgr_lib_get_port_speed_mode(port, &mode);
		if(!ret)
			printf("port %d  speedmode is %d\n", port, mode);
	}
	else
	{
		ret = switchmgr_lib_set_port_speed_mode(port, mode);
		if(!ret)
			printf("set port %d speedmode %d\n", port, mode);

	}	


	return 0;
}

static int doLinkState(int argc, char *argv[], void *p)
{
	u8 port=0;
	u8 linkst=0;
	u8 speed=0;
	u8 ret = 0;

	if(argc < 2)
	{
		printf("%s", HELP_LINK_STATE);
		return 0;
	}

	port=atoi(argv[1]);

	if(argc == 2)
	{
		ret = switchmgr_lib_get_link_state(port, &linkst, &speed);
		if(!ret)
			printf("port %d  linkst is %s, speed is %d\n", port, linkst?"up":"down", speed);
	}

	return 0;
}


static int doFCGlobal(int argc, char *argv[], void *p)
{
	u8 enable=0;
	u8 ret = 0;

	if(argc < 2)
	{
		printf("%s", HELP_FC_GLOBAL);
		return 0;
	}

	if(strcmp(argv[1], "get") == 0)
	{
		ret = switchmgr_lib_get_port_follow_control_global(&enable);
		if(!ret)
			printf("follow control global is %d\n", enable);
	}
	else if((strcmp(argv[1], "set") == 0) && (argc > 2))
	{
		enable=atoi(argv[2]);
		ret = switchmgr_lib_set_port_follow_control_global(enable);
		if(!ret)
			printf("set follow control global %d\n", enable);
	}	
	else{
		printf("%s", HELP_FC_GLOBAL);
		return 0;
	}

	return 0;
}

static int doFollowControl(int argc, char *argv[], void *p)
{
	u8 port=0;
	u8 enable=0;
	u8 ret = 0;

	if(argc < 2)
	{
		printf("%s", HELP_FOLLOWCONTROL_ENABLE);
		return 0;
	}

	port=atoi(argv[1]);
	if(argc > 2)
		enable=atoi(argv[2]);

	if(argc == 2)
	{
		ret = switchmgr_lib_get_port_follow_control(port, &enable);
		if(!ret)
			printf("port %d  follow control is %d\n", port, enable);
	}
	else
	{
		ret = switchmgr_lib_set_port_follow_control(port, enable);
		if(!ret)
			printf("set port %d follow control %d\n", port, enable);

	}	


	return 0;
}

static int doStatistic(int argc, char *argv[], void *p)
{
	u8 port=0;
	switch_api_cnt portcnt;
	u8 ret = 0;

	if(argc < 2)
	{
		printf("%s", HELP_PORT_STATISTIC);
		return 0;
	}

	if(strcmp(argv[1], "clear") == 0)
	{
		ret = switchmgr_lib_set_all_port_statistics_clear();
		if(!ret)
			printf("port_statistics_clear\n");

		return 0;		
	}

	port=atoi(argv[1]);

	ret = switchmgr_lib_get_all_port_statistics(port, &portcnt);
	if(ret)
		return 0;
	/*tx show*/
	printf("TxPktsCnt:              %08x    TxUniPktsCnt:          %08x\n", portcnt.TxPktsCnt, portcnt.TxUniPktsCnt);
	printf("TxBroadPktsCnt:         %08x    TxMultiPktsCnt:        %08x\n", portcnt.TxBroadPktsCnt, portcnt.TxMultiPktsCnt);
	printf("TxNonUniPktsCnt:        %08x\n", portcnt.TxNonUniPktsCnt);
	
	printf("TxUnderSizePktsCnt:     %08x    Tx64BytePktsCnt:       %08x\n", portcnt.TxUnderSizePktsCnt, portcnt.Tx64BytePktsCnt);
	printf("Tx65_127BytePktsCnt:    %08x    Tx128_255BytePktsCnt:  %08x\n", portcnt.Tx65_127BytePktsCnt, portcnt.Tx128_255BytePktsCnt);
	printf("Tx256_511BytePktsCnt:   %08x    Tx512_1023BytePktsCnt: %08x\n", portcnt.Tx256_511BytePktsCnt, portcnt.Tx512_1023BytePktsCnt);
	printf("Tx1024_1518BytePktsCnt: %08x    TxOverSizePktsCnt:     %08x\n", portcnt.Tx1024_1518BytePktsCnt, portcnt.TxOverSizePktsCnt);

	printf("TxDropFramesCnt:        %08x    TxPauseFramesCnt:      %08x\n", portcnt.TxDropFramesCnt, portcnt.TxPauseFramesCnt);
	printf("TxCRCFramesCnt:         %08x    TxSingleCollsionEvent: %08x\n", portcnt.TxCRCFramesCnt, portcnt.TxSingleCollsionEvent);
	printf("TxMultiCollsionEvent:   %08x\n", portcnt.TxMultiCollsionEvent);

	/*rx show*/
	printf("\n");
	printf("RxPktsCnt:              %08x    RxUniPktsCnt:          %08x\n", portcnt.RxPktsCnt, portcnt.RxUniPktsCnt);
	printf("RxBroadPktsCnt:         %08x    RxMultiPktsCnt:        %08x\n", portcnt.RxBroadPktsCnt, portcnt.RxMultiPktsCnt);
	printf("RxNonUniPktsCnt:        %08x\n", portcnt.RxNonUniPktsCnt);

	printf("RxUnderSizePktsCnt:     %08x    Rx64BytePktsCnt:       %08x\n", portcnt.RxUnderSizePktsCnt, portcnt.Rx64BytePktsCnt);
	printf("Rx65_127BytePktsCnt:    %08x    Rx128_255BytePktsCnt:  %08x\n", portcnt.Rx65_127BytePktsCnt, portcnt.Rx128_255BytePktsCnt);
	printf("Rx256_511BytePktsCnt:   %08x    Rx512_1023BytePktsCnt: %08x\n", portcnt.Rx256_511BytePktsCnt, portcnt.Rx512_1023BytePktsCnt);
	printf("Rx1024_1518BytePktsCnt: %08x    RxOverSizePktsCnt:     %08x\n", portcnt.Rx1024_1518BytePktsCnt, portcnt.RxOverSizePktsCnt);

	printf("RxDropFramesCnt:        %08x    RxPauseFramesCnt:      %08x\n", portcnt.RxDropFramesCnt, portcnt.RxPauseFramesCnt);
	printf("RxCRCFramesCnt:         %08x\n", portcnt.RxCRCFramesCnt);
	
	return 0;
}

static int doLan2lanClear(int argc, char *argv[], void *p)
{
	u8 ret = 0;
	
	ret = switchmgr_lib_set_lan2lan_clear();
	if(!ret)
		printf("lan2lan_clear\n");

	return 0;
}

static int doLan2lan(int argc, char *argv[], void *p)
{
	switch_lan2lan lan2lan = {0};
	switch_lan2lan_info lan2laninfo = {0};
	u8 srcport, dstport, i;


	if(argc < 2)
	{
		printf("%s", HELP_LAN2LAN);
		return 0;
	}
	else if(argc == 2)
	{
		if(strcmp(argv[1], "show") == 0){
			switchmgr_lib_get_lan2lan(&lan2laninfo);
			if(lan2laninfo.enable)
				printf("lan2lan enable\n");
			else{
				printf("lan2lan disable\n");
				return -1;
			}
			printf("dest port %d\n", lan2laninfo.dstport);
			if(lan2laninfo.srcportnum>4)
			{
				printf("srcportnum %d error\n", lan2laninfo.srcportnum);
				return -1;
			}
			else
				printf("srcportnum %d\n", lan2laninfo.srcportnum);
			for(i=0;i<lan2laninfo.srcportnum;i++)
			{
				printf("src port %d, rx_enable: %d, tx_enable: %d\n", lan2laninfo.srcport[i].port, lan2laninfo.srcport[i].rx_enable,lan2laninfo.srcport[i].tx_enable);
			}
		}
	}
	else if(argc == 3)
	{
		srcport=atoi(argv[1]);
		dstport=atoi(argv[2]);
		lan2lan.srcport = srcport;
		lan2lan.dstport = dstport;
		lan2lan.rx_enable = 1;
		lan2lan.tx_enable = 1;
		switchmgr_lib_set_lan2lan(lan2lan);
	}
	else if(argc > 3)
	{
		srcport=atoi(argv[1]);
		dstport=atoi(argv[2]);
		lan2lan.srcport = srcport;
		lan2lan.dstport = dstport;
		if(strcmp(argv[3], "rx") == 0){
			lan2lan.rx_enable = 1;
			lan2lan.tx_enable = 0;
		}
		else if(strcmp(argv[3], "tx") == 0){
			lan2lan.tx_enable = 1;
			lan2lan.rx_enable = 0;
		}
		else
		{
			lan2lan.rx_enable = 1;
			lan2lan.tx_enable = 1;
		}
		switchmgr_lib_set_lan2lan(lan2lan);
	}	

	return 0;
}

static int doQos(int argc, char *argv[], void *p){
	return subcmd(qosCmds, argc, argv, p);
}


static int doAgeout(int argc, char *argv[], void *p){
    u32 time;
    if(argc < 2)
	{
		return 0;
	}
	time = atoi(argv[1]);
	switchmgr_lib_set_mac_ageout_time(time);
	return 0;
}

static int doMacLimit(int argc, char *argv[], void *p){
    u8 port;
    u32 limit_num;
    if(argc < 3)
	{
		return 0;
	}
	port = atoi(argv[1]);
	limit_num = atoi(argv[2]);
	switchmgr_lib_set_port_mac_limit(port, limit_num);
	return 0;
}

static int doPortIsolate(int argc, char *argv[], void *p){
    u8 enable;
    if(argc < 2)
	{
		return 0;
	}
	enable = atoi(argv[1]);
	switchmgr_lib_set_port_isolate(enable);
	return 0;
}


static int doMacTblClr(int argc, char *argv[], void *p){
    switchmgr_lib_clr_mac_table();
    return 0;
}

static int doBrtableDisp(int argc, char *argv[], void *p)
{
	switch_brtable *brtable = NULL;
	int idx = 0;
	u8 portidx = 0;

	brtable = (switch_brtable *)malloc(sizeof(switch_brtable));
	if ( !brtable )
		return -1;

	if ( argc >= 2 )
		portidx = (u8)atoi(argv[1]);

	bzero(brtable, sizeof(switch_brtable));
	switchmgr_lib_get_brtable(brtable, portidx);
	printf("port-no\tmac-addr\t\tageing-timer\tvid\n");

	for ( idx = 0; idx < brtable->count; idx ++ )
	{
		switch_macinfo *fbr = &(brtable->macinfo[idx]);
		printf("%3i\t%.2x:%.2x:%.2x:%.2x:%.2x:%.2x\t%d"
			 , fbr->port
			 , fbr->mac[0], fbr->mac[1], fbr->mac[2]
			 , fbr->mac[3], fbr->mac[4], fbr->mac[5]
			 , fbr->timer);
		if(fbr->vid != 0xFFFF)
		{
			printf("\t\t%d\n", fbr->vid);
		}
		else
		{
			printf("\t\tuntag\n");
		}
	}

	free(brtable);
	brtable = NULL;
    return 0;
}


static cmds_t switchCmds[] = {
	{"qos", 		doQos,			0x12,	0,	HELP_QOS_ENABLE},
	{"ratectl",		doRateControl,		0x12,	0,	HELP_RATE_CONTROLL}, 
	{"portenable",	doPortEnable,		0x12,	0,	HELP_PORT_ENABLE},
	{"loopbackrt",	doLoopBackRT,		0x12,	0,	HELP_LOOPBACK_ENABLE},
	{"speedmode",	doSpeedMode,		0x12,	0,	HELP_SPEED_MODE},
	{"linkstate",	doLinkState,		0x12,	0,	HELP_LINK_STATE},
	{"fcglobal",	doFCGlobal,		0x12,	0,	HELP_FC_GLOBAL},
	{"followcontrol",	doFollowControl,		0x12,	0,	HELP_FOLLOWCONTROL_ENABLE},
	{"lan2lanclear",	doLan2lanClear,		0x12,	0,	NULL},
	{"lan2lan",		doLan2lan,		0x12,	0,	HELP_LAN2LAN},
	{"statistic",	doStatistic,		0x12,	0,	HELP_PORT_STATISTIC},
	{"ageout",      doAgeout,           0x12,   0,  NULL},
	{"maclimit",    doMacLimit,           0x12,   0,  NULL},
	{"isolate",     doPortIsolate,           0x12,   0,  NULL},
	{"mactbl-clr",     doMacTblClr,           0x12,   0,  NULL},
	{"brtable",    doBrtableDisp,       0x12,   0,  NULL},
    {NULL,      NULL,       0x10, 0, NULL}
};

int subcmd(
	const cmds_t tab[], int argc, char *argv[], void *p
)
{
	const cmds_t *cmdp;
	int found = 0;
	int i;

	/* Strip off first token and pass rest of line to subcommand */
	if (argc < 2) {
		if (argc < 1)
			printf("SUBCMD - Don't know what to do?\n");
		else 
			goto print_out_cmds;
			//printk("\"%s\" - takes at least one argument\n",argv[0]);
		return -1;
	}
	argc--;
	argv++;
	for(cmdp = tab;cmdp->name != NULL;cmdp++){
		if(strlen(argv[0]) == strlen(cmdp->name)){		// add length exact match checking, to avoid conflict between "pbr" and "pbrw-test"
			if(strncmp(argv[0],cmdp->name,strlen(argv[0])) == 0){
				found = 1;
				break;
			}
		}
	}
	if(!found){
		char buf[66];
		
print_out_cmds:
		printf("valid subcommands:\n");
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
	
	if(cmdp->func==NULL){
		return 0;
	}
	else{
		return (*cmdp->func)(argc,argv,p);
	}
}/*end subcmd*/

int main(int argc, char **argv) 
{
 //   init_compileoption_val();

	return subcmd(switchCmds, argc, argv, NULL);
}


