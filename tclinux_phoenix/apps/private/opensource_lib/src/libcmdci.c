/********   **********************************************************************/
/*
 * Copyright (C) 1994-2014 EcoNet Technologies, Corp.
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
#include <stdlib.h>
#include <fcntl.h>
#include <asm/ioctl.h>
#include "libcmdci.h"
//#include "libtcapi.h"

int ioctl_pmap(int cmd, void *data, int datealen)
{
	int fp = -1;
	int ret = -1;	
	fp = open("/dev/pmap", O_RDONLY);
	if(fp == -1)
		printf("ioctl_pmap: fp is [%d]\n", fp);
	ret = ioctl(fp, cmd, data);
	if(ret < 0)
	{
		printf("\r\nioctl_pmap failed!");
	}
	else
	{
		//printf("\r\nioctl_pmap ok!");
	}
	close(fp);
	return 0;
}

static int getIfNum(char *ifName)
{
	char *d = ",";
	char *p = NULL;
	int num = 0;

	if (ifName == NULL) {
		return -1;
	}

	p = strtok(ifName, d);
	while (p) {
		num++;
		p = strtok(NULL, d );
	}

	return num;
}

int doaddGroup(int argc, char *argv[], void *p)
{
	struct portbindcmdif pbif;
	int ifNameNum = 0, ifTypeNum = 0;
	int ret;

	/* check input parameters num */
	if (argc < 3 || argc > 4) {
		goto Error_Handle;
	}
#ifdef TCSUPPORT_E8B
	if (argc != 4) {
		goto Error_Handle;
	}
#else
	if (argc != 3) {
		goto Error_Handle;
	}
#endif

	/* init data buf */
	memset(&pbif, 0, sizeof(struct portbindcmdif));

	/* store data from input parameters */
	pbif.groupid = atoi(argv[1]);
	strcpy(pbif.ifNameLst, argv[2]);
	
	if (argc == 4) {
		strcpy(pbif.ifTypeLst, argv[3]);
	}

	/* check parameters value */
	if (pbif.groupid < 0 || pbif.groupid > MAX_GROUP_NUM - 1) {
		goto Error_Handle;
	}

	ifNameNum = getIfNum(pbif.ifNameLst);
	pbif.ifNum = ifNameNum;
	
	if (argc == 4) {
		ifTypeNum = getIfNum(pbif.ifTypeLst);
		if (ifNameNum != ifTypeNum) {
			goto Error_Handle;
		}
	}

	/* update if list again, do you known why? */
	strcpy(pbif.ifNameLst, argv[2]);
	
	if (argc == 4) {
		strcpy(pbif.ifTypeLst, argv[3]);
	}

	ret = ioctl_pmap(PORTBIND_IOC_ADD_GROUP, &pbif, sizeof(pbif));
	return ret;
#ifdef TCSUPPORT_E8B
Error_Handle:
	printf("Invalid parameters! \r\n"
		   "Usage: \r\n"
		   "portbindcmd addgroup <group id> <port> <port type> \r\n");
	return -1;
#else
Error_Handle:
	printf("Invalid parameters! \r\n"
		   "Usage: \r\n"
		   "portbindcmd addgroup <group id> <port> \r\n");
	return -1;
#endif
}

int dodelGroup(int argc, char *argv[], void *p)
{
	int ret;
	struct portbindcmdif pbif;

	/* init the struct */
	memset(&pbif, 0, sizeof(pbif));
	pbif.groupid = -1;
	
	/* check parameter num */
	if (argc != 2) {
		goto Error_Handle;
	}

	pbif.groupid = atoi(argv[1]);
	/* check group id */
	if (pbif.groupid < 0 || pbif.groupid > MAX_GROUP_NUM - 1) {
		goto Error_Handle;
	}
	
	ret = ioctl_pmap(PORTBIND_IOC_DEL_GROUP, &pbif, sizeof(pbif));
	return ret;

Error_Handle:
	printf("Invalid parameters! \r\n"
		   "Usage: \r\n"
		   "portbindcmd delgroup <group id> \r\n");
	return -1;
}

int doshowGroup(int argc, char *argv[], void *p)
{
	int ret;
	struct portbindcmdif pbif;

	/* init the struct */
	memset(&pbif, 0, sizeof(struct portbindcmdif));
	pbif.groupid = -1;

	if (argc > 2) {
		goto Error_Handle;
	}
	
	if (argc == 2) {
		pbif.groupid = atoi(argv[1]);
		/* check group id */
		if (pbif.groupid < 0 || pbif.groupid > MAX_GROUP_NUM - 1) {
			goto Error_Handle;
		}
	}
	
	ret = ioctl_pmap(PORTBIND_IOC_SHOW_GROUP, &pbif, sizeof(pbif));
	return ret;

Error_Handle:
	printf("Invalid parameters! \r\n"
		   "Usage: \r\n"
		   "portbindcmd showgroup <group id> \r\n");
	return -1;
}

typedef struct {
    char *name;
    int (*func)(int argc,char *argv[],void *p);
    int flags;
    int argcmin;
    char *argc_errmsg;
} cmds_t;

int subcmd (
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
			if(!(cmdp->flags & 0x10))/*judge hide the subcmd or not*/
				continue;
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
