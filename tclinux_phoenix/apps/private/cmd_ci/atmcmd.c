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


#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include "global.h"
#include "../../../version/tcversion.h"


#if 0
typedef struct {
    char *name;
    int (*func)(int argc,char *argv[],void *p);
    int flags;
    int argcmin;
    char *argc_errmsg;
} cmds_t;
#endif

#define ATMCMD_LOCK_FILE "/tmp/atmlockfd"

static int doAtmBegin(int argc, char *argv[], void *p);
static int doAtmEnd(int argc, char *argv[], void *p);
static int doAtmPvc(int argc, char *argv[], void *p);
static int doAtmOam(int argc, char *argv[], void *p);
static int doAtmVer(int argc, char *argv[], void *p);
static int doAtmHelp(int argc, char *argv[], void *p);
static int doAtmAuto(int argc, char *argv[], void *p);
static int doAtmPvcAdd(int argc, char *argv[], void *p);
static int doAtmPvcAddAal(int argc, char *argv[], void *p);
static int doAtmPvcAddEncap(int argc, char *argv[], void *p);
static int doAtmPvcAddAtmqos(int argc, char *argv[], void *p);
static int doAtmPvcAddPcr(int argc, char *argv[], void *p);
static int doAtmPvcAddScr(int argc, char *argv[], void *p);
static int doAtmPvcAddMbs(int argc, char *argv[], void *p);
static int doAtmPvcAddVlanid(int argc, char *argv[], void *p);
static int doAtmPvcAddPriority(int argc, char *argv[], void *p);
static int doAtmPvcAddState(int argc, char *argv[], void *p);
static int doAtmPvcAddLinktype(int argc, char *argv[], void *p);
static int doAtmPvcDel(int argc, char *argv[], void *p);
static int doAtmPvcShow(int argc, char *argv[], void *p);
static int doAtmPvcStats(int argc, char *argv[], void *p);
static int doAtmOamStart(int argc, char *argv[], void *p);
static int doAtmOamShow(int argc, char *argv[], void *p);
static int doAtmOamStop(int argc, char *argv[], void *p);
static int doAtmOamStartRep(int argc, char *argv[], void *p);
static int doAtmOamStartTimeout(int argc, char *argv[], void *p);
static int doAtmAutoPvc(int argc, char *argv[], void *p);
static int doAtmAutoEncap(int argc, char *argv[], void *p);
static int doAtmAutoProtocol(int argc, char *argv[], void *p);
static int doAtmAutoMac(int argc, char *argv[], void *p);
static int doAtmAutoTimeout(int argc, char *argv[], void *p);

#define	ENCAP_VC			0x00
#define	ENCAP_LLC			0x01
#define	ENCAP_OTHER			0x02
#define	ATM_QOS_UBR			0x00
#define ATM_QOS_UBRPCR  	0x01
#define ATM_QOS_CBR     	0x02
#define ATM_QOS_RTVBR   	0x03
#define ATM_QOS_NRTVBR  	0x04
#define LINK_TYPE_RFC2684BR	0x00
#define LINK_TYPE_RFC2684RT 0x01
#define LINK_TYPE_RFC2364	0x02
#define LINK_TYPE_OTHER   	0x03

unsigned char 	vpi, state_vpi, vc_enable, encap, QoS, linktype, oam_f4f5, oam_type;
unsigned short	vci, state_vci, pcr, scr, mbs;
unsigned int 	oam_rep_num, oam_timeout;
unsigned char vcs_in_pool = 1, vcpool[32][2], dsec;
//uint16	vlanid, priority;


static cmds_t AtmCmds[] = {
    {"begin",                doAtmBegin,       0x10,   0,  NULL},
    {"end",                  doAtmEnd,         0x10,   0,  NULL},
    {"pvc",		             doAtmPvc,    	 0x10,   0,  NULL},
    {"oam",                  doAtmOam,       	 0x10,   0,  NULL},
	{"version",              doAtmVer,	     0x10,   0,  NULL},
    {"help",                 doAtmHelp,        0x10,   0,  NULL},
	{"autosearch",           doAtmAuto,        0x10,   0,  NULL},
    {NULL,          NULL,               0x10,   0,  NULL},
};

static cmds_t AtmPvcCmds[] = {
    {"--add",                doAtmPvcAdd,      0x10,   0,  NULL},
    {"--delete",        	 doAtmPvcDel,      0x10,   0,  NULL},
    {"--show",           	 doAtmPvcShow,  	 0x10,   0,  NULL},
    {"--stats",              doAtmPvcStats,    0x10,   0,  NULL},
    {NULL,          NULL,               0x10,   0,  NULL},
};

static cmds_t AtmPvcAddCmds[] = {
	{"--aal",                doAtmPvcAddAal,        0x10,   0,  NULL},
	{"--encap",              doAtmPvcAddEncap,      0x10,   0,  NULL},
	{"--atmqos",             doAtmPvcAddAtmqos,     0x10,   0,  NULL},
	{"--pcr",              	 doAtmPvcAddPcr,    	  0x10,   0,  NULL},
	{"--scr",              	 doAtmPvcAddScr,    	  0x10,   0,  NULL},
	{"--mbs",              	 doAtmPvcAddMbs,    	  0x10,   0,  NULL},
	{"--vlanid",             doAtmPvcAddVlanid,     0x10,   0,  NULL},
	{"--priority",           doAtmPvcAddPriority,   0x10,   0,  NULL},
	{"--state",              doAtmPvcAddState,      0x10,   0,  NULL},
	{"--linktype",           doAtmPvcAddLinktype,   0x10,   0,  NULL},
	{NULL,          NULL,               0x10,   0,  NULL},
};

static cmds_t AtmOamCmds[] = {
    {"--start",              doAtmOamStart,    0x10,   0,  NULL},
    {"--show",             	 doAtmOamShow,     0x10,   0,  NULL},
    {"--stop",               doAtmOamStop,   	 0x10,   0,  NULL},
    {NULL,          NULL,               0x10,   0,  NULL},
};

static cmds_t AtmOamStartCmds[] = {
    {"--repetition",         doAtmOamStartRep,    0x10,   0,  NULL},
    {"--timeout",            doAtmOamStartTimeout,0x10,   0,  NULL},
    {NULL,          NULL,               0x10,   0,  NULL},
};

static cmds_t AtmAutoCmds[] = {
    {"--pvc",         doAtmAutoPvc,    0x10,   0,  NULL},
    {"--encap",       doAtmAutoEncap, 0x10,   0,  NULL},
	{"--protocol",    doAtmAutoProtocol,0x10,   0,  NULL},
	{"--mac",         doAtmAutoMac, 0x10,   0,  NULL},
	{"--timeout",     doAtmAutoTimeout, 0x10,   0,  NULL},
    {NULL,          NULL,               0x10,   0,  NULL},
};

static char *MultiparaCmds[] = {"--state", "--start", "--pvc"};
//static unsigned short vc_map[8][2];

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

int cosubcmd  (
    const cmds_t tab[], int argc, char *argv[], void *p
)
{
    register const cmds_t *cmdp;
    int found = 0;
    int i, j, ret = 0;
	char *parent_cmd = argv[0];

	argv++;
	argc--;
    /* Strip off first token and pass rest of line to subcommand */
    if (argc < 2) {
        if (argc < 1)
            printf("SUBCMD - Don't know what to do?\n");
    	else{
        	char buf[66];

			printf("valid subcommands of %s:\n", parent_cmd);
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
		}
		printf("return -1\r\n");
		return -1;
	}
	for(i = 1; i < argc; i++){
	    found = 0;
	    for(cmdp = tab; cmdp->name != NULL; cmdp++){
#if 0			
			printf("cmdp->name = %s, argv[%d] = %s\r\n", cmdp->name, i, argv[i]);
#endif
			if(strncmp(argv[i],cmdp->name,strlen(argv[i])) == 0){
	        	found = 1;
	        	if((*cmdp->func)(argc-i,argv+i,p))
	           		ret = -1;
	            break;
			}
		}
		if(!found){
#if 0			
			printf("found == 0\r\n");
			printf("argv[i-1] = %s\r\n", argv[i-1]);
			printf("argv[i-2] = %s\r\n", argv[i-2]);
#endif
			if(argv[i-1][0] != '-'){
			   for(j = 0; j < 3; j++){
				   if(strncmp(argv[i-2], MultiparaCmds[j], strlen(argv[i-2])) == 0){
					   ret = 0;
					   break;
				   }
			   }
			   if(j == 2){//Skip to a command next to --pvc
#if 0				   
					printf("Skip to a command next to --pvc\r\n");
#endif
					i += (vcs_in_pool-3);
#if 0					
					printf("After Skip: i = %d\r\n", i);
#endif					
//					argv += (vcs_in_pool+1);
//					printf("After Skip: argc = %d, argv[0] = %s\r\n", argc, argv[0]);
			   }
			   if(j == 3){
		      		printf("invalid subcommand \"%s\" of command \"%s\"\r\n", argv[i], argv[0]);
		      		ret = -1;
			   }
		   }
		   else
		   	  ret = 0;
		}
		if(ret)
		   break;
	}
	return ret;
}

int doAtmBegin(int argc, char *argv[], void *p)
{
	printf("Atm Begin\r\n");
	return 0;
}

int doAtmEnd(int argc, char *argv[], void *p)
{
	printf("Atm End\r\n");
    return 0;
}

int doAtmPvc(int argc, char *argv[], void *p)
{
	return subcmd(AtmPvcCmds, argc, argv, p);
}

int doAtmOam(int argc, char *argv[], void *p)
{
    return subcmd(AtmOamCmds, argc, argv, p);
}

int doAtmVer(int argc, char *argv[], void *p)
{
    printf("atmphxcmd version: %s\r\n",MODULE_VERSION_ATMCMD);
	return 0;
}

int doAtmHelp(int argc, char *argv[], void *p)
{
	printf("Usage: atmphxcmd begin \r\n"    
		   "       atmphxcmd end \r\n"
		   "       atmphxcmd pvc  --add {vpi.vci}  [--aal {aal5}}]\r\n"
		   "                                    [--encap <vc|llc|other>]\r\n"
		   "                                    [--atmqos {ubr|ubr_pcr|cbr|rtvbr|nrtvbr>]\r\n"
		   "                                    [--pcr {PeakCellRate}]\r\n" 
		   "                                    [--scr {sustainedCellRate}]\r\n" 
		   "                                    [--mbs {MaxBurstSize}\r\n"
		   "       atmphxcmd pvc  --delete {vpi.vci}\r\n"
		   "       atmphxcmd pvc  --show {all|{vpi.vci}}\r\n"
		   "       atmphxcmd pvc  --stats {all|{vpi.vci}}\r\n"
		   "       atmphxcmd autosearch  [--pvc {vpi.vci}*] [--encap <vc|llc>] \r\n"
		   "						  [--protocol <pppoe|dhcp>] [--mac 00:11:22:33:44:00] \r\n"
		   "						  [--timeout 20]\r\n"
	       "       atmphxcmd oam  --start {vpi.vci} {f4|f5} {--type {etoe|segment}}\r\n"
	       "                   [--repetition {Repetition number}] [--timeout {timeout}]\r\n" 
	       "       atmphxcmd oam  --show {vpi.vci}\r\n" 
		   "       atmphxcmd oam  --stop {vpi.vci}\r\n"
	       "       atmphxcmd version \n"
	       "       atmphxcmd help\n\n");

    return 0;
}

int doAtmAuto(int argc, char *argv[], void *p)
{
	int i, fin;
	FILE *file_fin;

	system("sys autopvc active 1");
	i = cosubcmd(AtmAutoCmds, argc+1, argv-1, p);
	system("sys autopvc probe vcpool");
	while(dsec--){
		file_fin = fopen("/proc/tc3162/autopvc_fin", "r");
		fscanf(file_fin, "%u", &fin);
		fclose(file_fin);
		if(fin)
			break;
		sleep(1);
	}
	system("sys autopvc active 0");
    return i;
}

int delPvc(unsigned char vpi, unsigned short vci){
	char cmd[50], *path = "/proc/tc3162/tsarm_pvc_del";
	FILE *pvc_del;
	int pvc_idx, pid;

	sprintf(cmd, "echo %d %d 0 > %s", vpi, vci, path);
	system(cmd);
	pvc_del = fopen(path, "r");
	fscanf(pvc_del, "%u %u", &pvc_idx, &pid);
	fclose(pvc_del);
	if(pid != -1){
		sprintf(cmd, "kill -9 %d", pid);
		system(cmd);
		return 0;
	}
	else
		return -1;

}

int doAtmPvcAdd(int argc, char *argv[], void *p)
{
	char *pch_vpi, *pch_vci, *vc_map_path = "/proc/tc3162/tsarm_pvc";
	char *pid_map_path = "/proc/tc3162/tsarm_pvc_del";
	char vc_map_entry[15], cmd[300], atmqos[10];
    FILE *vc_map_file, *pid_map_file;
	int i, pvc_idx, pid;
	unsigned char e;

	encap = ENCAP_LLC;
    QoS = ATM_QOS_UBR;
    pcr = 0;
    scr = 0;
    mbs = 0;
    vc_enable = 1;
    linktype = LINK_TYPE_RFC2684BR;
							
	if(!argv[1]){
		printf("Wrong format for {vpi.vci} input\r\n");
        return -1;
	}
	pch_vpi = strtok(argv[1], ".");
	pch_vci = strtok(NULL, ".");
	if(!pch_vpi || !pch_vci){
		printf("Wrong format for {vpi.vci} input\r\n");
		return -1;
	}
	else{
		for(i = 0; i < strlen(pch_vpi); i++){
			if(!isdigit(pch_vpi[i])){
				printf("invalid vpi\r\n");
				return -1;
			}
		}
		for(i = 0; i < strlen(pch_vci); i++){
            if(!isdigit(pch_vci[i])){
                printf("invalid vci\r\n");
                return -1;
            }
        }
		vpi = atoi(pch_vpi);
		vci = atoi(pch_vci);
		if(vpi == 0 && vci == 0){
			printf("Wrong vpi/vci to add\r\n");
			return -1;
		}

	}
	if(argc > 2){
		if(!cosubcmd(AtmPvcAddCmds, argc, argv, p)){
		}
	}
	vc_map_file = fopen(vc_map_path, "r");
	fscanf(vc_map_file, "%u", &pvc_idx);
	fclose(vc_map_file);
	switch(QoS){
		case ATM_QOS_UBR:
		case ATM_QOS_UBRPCR:
			sprintf(atmqos, "ubr");
			break;
		case ATM_QOS_CBR:
			sprintf(atmqos, "cbr");
			break;
		case ATM_QOS_RTVBR:
			sprintf(atmqos, "rt-vbr");
			break;
		case ATM_QOS_NRTVBR:
			sprintf(atmqos, "nrt-vbr");
			break;
	}
	switch(linktype){
		case LINK_TYPE_RFC2684BR:
		case LINK_TYPE_OTHER:
			switch(encap){
				case ENCAP_VC:
					e = 1;
					break;
				case ENCAP_LLC:
				case ENCAP_OTHER:
					e = 0;
					break;
			}
			break;
		case LINK_TYPE_RFC2684RT:
			switch(encap){
			    case ENCAP_VC:
			        e = 3;
			        break;
			    case ENCAP_LLC:
			    case ENCAP_OTHER:
			        e = 2;
			        break;
			}
			break;
		case LINK_TYPE_RFC2364:
			switch(encap){
				case ENCAP_VC:
				    e = 5;//PPPoA VC
				    break;
				case ENCAP_LLC:
				case ENCAP_OTHER:
				    e = 4;//PPPoA LLC
				    break;
			}
			break;
	}
	switch(QoS){
		case ATM_QOS_UBR:
		case ATM_QOS_UBRPCR:
			if(e >= 4)
				sprintf(atmqos, "UBR");
			else
				sprintf(atmqos, "ubr");
			break;
		case ATM_QOS_CBR:
			if(e >= 4)
				sprintf(atmqos, "CBR");
			else
		    	sprintf(atmqos, "cbr");
		    break;
		case ATM_QOS_RTVBR:
			if(e >= 4)
				sprintf(atmqos, "VBR");
			else
		    	sprintf(atmqos, "rt-vbr");
		    break;
		case ATM_QOS_NRTVBR:
			if(e >= 4)
		        sprintf(atmqos, "NRTVBR");
		    else
			    sprintf(atmqos, "nrt-vbr");
		    break;
   	}
	if(e >= 4){
		sprintf(cmd, "br2684ctl -c %d -e %d -t %s -p %d -q %d -m %d -a 0.%d.%d",
				pvc_idx, e, atmqos, pcr, scr, mbs, vpi, vci);
	}
	else{
		sprintf(cmd, "br2684ctl -c %d -e %d -t %s -p %d -q %d -m %d -a 0.%d.%d &",
            	pvc_idx, e, atmqos, pcr, scr, mbs, vpi, vci);
		system(cmd);
		usleep(100000);
		system("pidof br2684ctl > /proc/tc3162/tsarm_pvc_pid");
		pid_map_file = fopen("/proc/tc3162/tsarm_pvc_pid", "r");
        fscanf(pid_map_file, "%u", &pid);
        fclose(pid_map_file);						
		sprintf(cmd, "echo %d %d %d > %s", vpi, vci, pid, pid_map_path);
		system(cmd);
		pid_map_file = fopen(pid_map_path, "r");
	    fscanf(pid_map_file, "%u %u", &pvc_idx, &pid);
		fclose(pid_map_file);
		if(pvc_idx == -1 || pid == -1){
			printf("Fail to add new wan interface!!\r\n");
			return -1;
		}
	}

	sprintf(cmd, "ifconfig nas%d 0.0.0.0", pvc_idx);
	system(cmd);

	if(!(state_vpi == 0 && state_vci == 0)){
		int pvc, pid;
		FILE *pvc_del;

		sprintf(cmd, "echo %d %d -1 > %s", state_vpi, state_vci, pid_map_path);
    	system(cmd);
    	pvc_del = fopen(pid_map_path, "r");
    	fscanf(pvc_del, "%u %u", &pvc, &pid);
    	fclose(pvc_del);
    	if(pvc != -1){
			if(vc_enable)
    			sprintf(cmd, "ifconfig nas%d up", pvc);
			else
				sprintf(cmd, "ifconfig nas%d down", pvc);
			system(cmd);
			return 0;
		}
		else
			return -1;
	}

    return 0;
}

int doAtmPvcAddAal(int argc, char *argv[], void *p)
{
	if(!argv[1] || strcmp(argv[1], "aal5")){
		printf("--aal {aal5}\r\n");
		return -1;
	}
    return 0;
}

int doAtmPvcAddEncap(int argc, char *argv[], void *p)
{
	if(!argv[1]){
		printf("--encap {vc|llc|other}\r\n");
		return -1;
	}
	if(!strcmp(argv[1], "vc")){
		encap = ENCAP_VC;
	}
	else if(!strcmp(argv[1], "llc")){
		encap = ENCAP_LLC;
	}
	else if(!strcmp(argv[1], "other")){
		encap = ENCAP_OTHER;
	}
	else{
		printf("--encap {vc|llc|other}\r\n");
		return -1;
	}
    return 0;
}

int doAtmPvcAddAtmqos(int argc, char *argv[], void *p)
{
	if(!argv[1]){
	   printf("--atmqos {ubr|ubr_pcr|cbr|rtvbr|nrtvbr}\r\n");
	   return -1;
	}

	if(!strcmp(argv[1], "ubr")){
		QoS = ATM_QOS_UBR;
    }
    else if(!strcmp(argv[1], "ubr_pcr")){
		QoS = ATM_QOS_UBRPCR;
    }
    else if(!strcmp(argv[1], "cbr")){
		QoS = ATM_QOS_CBR;
    }
	else if(!strcmp(argv[1], "rtvbr")){
		QoS = ATM_QOS_RTVBR;
    }
	else if(!strcmp(argv[1], "nrtvbr")){
		QoS = ATM_QOS_NRTVBR;
    }
    else{
        printf("--atmqos {ubr|ubr_pcr|cbr|rtvbr|nrtvbr}\r\n");
		return -1;
    }

    return 0;
}

int doAtmPvcAddPcr(int argc, char *argv[], void *p)
{
	int i;

	if(!argv[1]){
	   printf("--pcr {pcr}\r\n");
	   return -1;
	}
	for(i = 0; i < strlen(argv[1]); i++){
	    if(!isdigit(argv[1][i])){
	       printf("invalid pcr\r\n");
	       return -1;
	    }
	}

	pcr = atoi(argv[1]);
    return 0;
}

int doAtmPvcAddScr(int argc, char *argv[], void *p)
{
	int i;

	if(!argv[1]){
	   printf("--scr {scr}\r\n");
	   return -1;
	}
	for(i = 0; i < strlen(argv[1]); i++){
	    if(!isdigit(argv[1][i])){
	       printf("invalid scr\r\n");
	       return -1;
	    }
	}

	scr = atoi(argv[1]);
    return 0;
}

int doAtmPvcAddMbs(int argc, char *argv[], void *p)
{
	int i;

	if(!argv[1]){
	   printf("--mbs {mbs}\r\n");
	   return -1;
	}
	for(i = 0; i < strlen(argv[1]); i++){
	    if(!isdigit(argv[1][i])){
	       printf("invalid pcr\r\n");
	       return -1;
	    }
	}

	mbs = atoi(argv[1]);
    return 0;
}

int doAtmPvcAddVlanid(int argc, char *argv[], void *p)
{
	printf("Currently we don't support vlanid\r\n");
    return 0;
}

int doAtmPvcAddPriority(int argc, char *argv[], void *p)
{
	printf("Currently we don't support priority\r\n");
    return 0;
}

int doAtmPvcAddState(int argc, char *argv[], void *p)
{
	char *pch_vpi, *pch_vci;
	int i;

	if(!argv[1]){
	   printf("Wrong format for {vpi.pci} input\r\n");
	   return -1;
	}

	pch_vpi = strtok(argv[1], ".");
    pch_vci = strtok(NULL, ".");
    if(!pch_vpi || !pch_vci){
       printf("Wrong format for {vpi.pci} input\r\n");
       return -1;
    }
	else{
		for(i = 0; i < strlen(pch_vpi); i++){
			if(!isdigit(pch_vpi[i])){
			   printf("invalid vpi\r\n");
			   return -1;
			}
		}
		for(i = 0; i < strlen(pch_vci); i++){
		    if(!isdigit(pch_vci[i])){
		       printf("invalid vci\r\n");
		       return -1;
		    }
		}

	   	state_vpi = atoi(pch_vpi);
	   	state_vci = atoi(pch_vci);
		if(state_vpi == 0 && state_vci == 0){
			printf("Wrong vpi/vci for --state\r\n");
			return -1;
		}
	}
	if(!strcmp(argv[2], "enable")){
		printf("VC %d.%d is enabled\r\n", state_vpi, state_vci);
		vc_enable = 1;
	}
	else if(!strcmp(argv[2], "disable")){
		printf("VC %d.%d is disabled\r\n", state_vpi, state_vci);
		vc_enable = 0;
	}
	else{
		printf("--state {vpi.vci}{enable|disable}\r\n");
	}

    return 0;
}

int doAtmPvcAddLinktype(int argc, char *argv[], void *p)
{
	if(!argv[1]){
	   printf("--linktype {rfc2684br|rfc2684rt|rfc2364|other}\r\n");
	   return -1;
	}

	if(!strcmp(argv[1], "rfc2684br")){
		linktype = LINK_TYPE_RFC2684BR;
    }
    else if(!strcmp(argv[1], "rfc2684rt")){
		linktype = LINK_TYPE_RFC2684RT;
    }
    else if(!strcmp(argv[1], "rfc2364")){
		linktype = LINK_TYPE_RFC2364;
    }
    else if(!strcmp(argv[1], "other")){
		linktype = LINK_TYPE_OTHER;
    }
    else{
        printf("--linktype {rfc2684br|rfc2684rt|rfc2364|other}\r\n");
    }
					
    return 0;
}

int doAtmPvcDel(int argc, char *argv[], void *p)
{
	char *pch_vpi, *pch_vci;
	char cmd[50];
	int i;

	if(!argv[1]){
    	printf("Wrong format for {vpi.pci} input\r\n");
		return -1;
	}
	pch_vpi = strtok(argv[1], ".");
	pch_vci = strtok(NULL, ".");
	if(!pch_vpi || !pch_vci){
		printf("Wrong format for {vpi.pci} input\r\n");
		return -1;
    }
    else{
		for(i = 0; i < strlen(pch_vpi); i++){
			if(!isdigit(pch_vpi[i])){
				printf("invalid vpi\r\n");
				return -1;
			}
		}
		for(i = 0; i < strlen(pch_vci); i++){
			if(!isdigit(pch_vci[i])){
				printf("invalid vci\r\n");
				return -1;
			}
		}
		vpi = atoi(pch_vpi);
		vci = atoi(pch_vci);
		if(vpi == 0 && vci == 0)
		{
			printf("Wrong vpi/vci for --delete\r\n");
			return -1;
		}
	}
	if(delPvc(vpi, vci) == -1)
		return -1;
    
	return 0;
}

int doAtmPvcShow(int argc, char *argv[], void *p)
{
    return 0;
}

int doAtmPvcStats(int argc, char *argv[], void *p)
{
	char *pch_vpi, *pch_vci;
	char pvcstats_cmd[50];
	int i;

	
	if(argc < 2){
		printf("--stats {all|{vpi.vci}}\r\n");
		return -1;
	}

	if(strncmp(argv[1], "all", strlen(argv[1]))){
		pch_vpi = strtok(argv[1], ".");
    	pch_vci = strtok(NULL, ".");
    	if(!pch_vpi || !pch_vci){
        	printf("Wrong format for {vpi.pci} input\r\n");
        	return -1;
    	}
    	else{
        	for(i = 0; i < strlen(pch_vpi); i++){
	        	if(!isdigit(pch_vpi[i])){
	        		printf("invalid vpi\r\n");
	         	  	return -1;
				}
			}
			for(i = 0; i < strlen(pch_vci); i++){
				if(!isdigit(pch_vci[i])){
					printf("invalid vci\r\n");
					return -1;
				}
			}
			vpi = atoi(pch_vpi);
			vci = atoi(pch_vci);
		}
	}
	#if 0
	sprintf(pvcstats_cmd, "echo %d %d > /proc/tc3162/tsarm_pvc", vpi, vci);
	system(pvcstats_cmd);
	#endif
	sprintf(pvcstats_cmd, "echo %d %d > /proc/tc3162/tsarm_pvc_info", vpi, vci);
	system(pvcstats_cmd);
	usleep(100000);
	system("cat /proc/tc3162/tsarm_pvc_info");
    return 0;
}

int doAtmOamStart(int argc, char *argv[], void *p)
{
	char *pch_vpi, *pch_vci;
	char oam_cmd[100];
	int i;

	if (argc < 5){
		printf("atmphxcmd oam  --start {vpi.vci} {f4|f5} {--type {etoe|segment}}\r\n"
			   "[--repetition {Repetition number}] [--timeout {timeout}]\r\n");
		return -1;
	}
	if(!argv[1]){
        printf("Wrong format for {vpi.pci} input\r\n");
        return -1;
    }
    pch_vpi = strtok(argv[1], ".");
    pch_vci = strtok(NULL, ".");
    if(!pch_vpi || !pch_vci){
        printf("Wrong format for {vpi.pci} input\r\n");
        return -1;
    }
    else{
        for(i = 0; i < strlen(pch_vpi); i++){
            if(!isdigit(pch_vpi[i])){
                printf("invalid vpi\r\n");
                return -1;
            }
        }
        for(i = 0; i < strlen(pch_vci); i++){
            if(!isdigit(pch_vci[i])){
                printf("invalid vci\r\n");
	            return -1;
	        }
	    }
		vpi = atoi(pch_vpi);
		vci = atoi(pch_vci);
	}

	if(strncmp(argv[2], "f4", strlen(argv[2])) && strncmp(argv[2], "f5", strlen(argv[2]))){
		printf("{f4|f5}\r\n");
        return -1;
	}
	else if(!strncmp(argv[2], "f4", strlen(argv[2])))
		oam_f4f5 = 0;
	else
		oam_f4f5 = 1;

	if(strncmp(argv[3], "--type", strlen(argv[3]))){
        printf("No type is assigned\r\n");
        return -1;
    }
    else{
		if(strncmp(argv[4], "etoe", strlen(argv[4])) && strncmp(argv[4], "segment", strlen(argv[4]))){
			printf("{etoe|segment}\r\n");
	        return -1;
		}
		else if(!strncmp(argv[4], "etoe", strlen(argv[4])))
        	oam_type = 1;
    	else
	        oam_type = 0;
	}
    if(!cosubcmd(AtmOamStartCmds, argc-3, argv+3, p)){
	}

	sprintf(oam_cmd, "echo \"%d %d %d %d 2 %d\" > /proc/tc3162/oam_ping", 
			vpi, vci, oam_f4f5, oam_type, oam_timeout);
	for(i = 0; i < oam_rep_num; i++)
		system(oam_cmd);

    return 0;
}

int doAtmOamStartRep(int argc, char *argv[], void *p)
{
	oam_rep_num = atoi(argv[1]);
	return 0;
}

int doAtmOamStartTimeout(int argc, char *argv[], void *p)
{
	oam_timeout = atoi(argv[1]);
    return 0;
}

int doAtmOamShow(int argc, char *argv[], void *p)
{
	system("cat /proc/tc3162/oam_ping");
    return 0;
}

int doAtmOamStop(int argc, char *argv[], void *p)
{
	char *pch_vpi, *pch_vci;
	int i;

	if(!argv[1]){
        printf("Wrong format for {vpi.pci} input\r\n");
        return -1;
    }
    pch_vpi = strtok(argv[1], ".");
    pch_vci = strtok(NULL, ".");
    if(!pch_vpi || !pch_vci){
        printf("Wrong format for {vpi.pci} input\r\n");
        return -1;
    }
    else{
        for(i = 0; i < strlen(pch_vpi); i++){
            if(!isdigit(pch_vpi[i])){
                printf("invalid vpi\r\n");
                return -1;
            }
        }
        for(i = 0; i < strlen(pch_vci); i++){
            if(!isdigit(pch_vci[i])){
                printf("invalid vci\r\n");
	            return -1;
	        }
	    }
//		vpi = atoi(pch_vpi);
//		vci = atoi(pch_vci);
	}
	system("killall atmphxcmd");
    return 0;
}

int doAtmAutoPvc(int argc, char *argv[], void *p)
{
	char *pch_vpi, *pch_vci, buf[100], *comma;
	int i;

	vcs_in_pool = 1;
	
	while(vcs_in_pool < argc && argv[vcs_in_pool][0] != '-'){
		if((comma = strchr(argv[vcs_in_pool], ',')) == NULL || comma[1] == '\0'){
			printf("Invalid VPI/VCI input format\r\n");
			return -1;
		}
		pch_vpi = strtok(argv[vcs_in_pool], ",");
    	pch_vci = strtok(NULL, ",");
		pch_vpi++;
		pch_vci[strlen(pch_vci)-1] = '\0';
		if(!pch_vpi || !pch_vci){
        	printf("Wrong format for {vpi.pci} input\r\n");
        	return -1;
    	}
    	else{
        	for(i = 0; i < strlen(pch_vpi); i++){
            	if(!isdigit(pch_vpi[i])){
                	printf("invalid vpi\r\n");
                	return -1;
            	}
        	}
        	for(i = 0; i < strlen(pch_vci); i++){
            	if(!isdigit(pch_vci[i])){
                	printf("invalid vci\r\n");
	            	return -1;
	        	}
	    	}
			vcpool[vcs_in_pool-1][0] = atoi(pch_vpi);
			vcpool[vcs_in_pool-1][1] = atoi(pch_vci);
		}
		sprintf(buf, "sys autopvc set vcpool %d %d %d", vcs_in_pool-1, vcpool[vcs_in_pool-1][0], vcpool[vcs_in_pool-1][1]);
		system(buf);
		if(++vcs_in_pool > 32)
			break;
	}
    return 0;
}

int doAtmAutoEncap(int argc, char *argv[], void *p)
{
	if(argc < 2){
		printf("usage: --encap <vc|llc>\r\n");
		return -1;
	}
	if(!strcmp(argv[1], "llc"))
		system("sys autopvc encap 0");
	else if(!strcmp(argv[1], "vc"))
		system("sys autopvc encap 1");
	else{
		printf("usage: --encap <vc|llc>\r\n");
		return -1;
	}

    return 0;
}

int doAtmAutoProtocol(int argc, char *argv[], void *p)
{
	if(argc < 2){
		printf("usage: --protocol <pppoe|dhcp>\r\n");
		return -1;
	}
	if(!strcmp(argv[1], "pppoe")){
//		system("tcapi set AutoPVC_Common Probe_PPPOE 1");
//		system("tcapi set AutoPVC_Common Probe_DHCP 0");
		system("sys autopvc service 2");
	}
	else if(!strcmp(argv[1], "dhcp")){
//		system("tcapi set AutoPVC_Common Probe_PPPOE 0");
//		system("tcapi set AutoPVC_Common Probe_DHCP 1");
		system("sys autopvc service 8");
	}
	else{
		printf("usage: --protocol <pppoe|dhcp>\r\n");
		return -1;
	}
    return 0;
}

int doAtmAutoMac(int argc, char *argv[], void *p)
{
	int i;
	char *MAC[6], buf[100];

	if(argc < 2){
		printf("usage example: --mac 00:11:22:33:44:00\r\n");
		return -1;
	}
	MAC[0] = strtok(argv[1], ":");
	if(!MAC[0]){
		printf("usage example: --mac 00:11:22:33:44:00\r\n");
		return -1;
	}
	for(i = 1; i < 6; i++){
   		MAC[i] = strtok(NULL, ":");
		if(!MAC[i]){
			printf("usage example: --mac 00:11:22:33:44:00\r\n");
			return -1;
		}
#if 0
		printf("MAC[%d] = %s\r\n", i, MAC[i]);
#endif		
	}
	sprintf(buf, "sys autopvc set mac %s %s %s %s %s %s", MAC[0], MAC[1], MAC[2], MAC[3], MAC[4], MAC[5]);
	system(buf);

    return 0;
}

int doAtmAutoTimeout(int argc, char *argv[], void *p)
{
	dsec = atoi(argv[1]);
    return 0;
}

int main(int argc, char **argv) 
{
	void *p;
	int ret = -1;
	int pidfd;
	pidfd =open(ATMCMD_LOCK_FILE,O_RDWR | O_CREAT);
	if(pidfd < 0)
	{
		printf("\r\nopen lock file error!");
		ret = subcmd(AtmCmds, argc, argv, p);
	}
	else
	{
		writew_lock(pidfd,0,SEEK_SET,0);
		ret = subcmd(AtmCmds, argc, argv, p);
		un_lock(pidfd,0,SEEK_SET,0);
		//close(pidfd);
	}

	return ret;
}
