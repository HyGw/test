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
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include "global.h"

#define ADSLCMD_LOCK_FILE "/tmp/adsllockfd"
#define ADSLCMD_DIAGNOSTIC_FILE "/tmp/adslFileDiag"
#define ADSLCMD_INFO_FILE "/proc/tc3162/tcsupport_adslinfo"


int fileToStdout(char*filepath)
{
	int fd;
	//struct stat statbuf;
	int /*readcount = 0 ,*/count = 0;
	char buf[256] = {0};
	
	fd = open(filepath,O_RDONLY);
	if(fd < 0)
	{	
		printf("\r\nopen file error!");
		return -1;
	}

	lseek(fd,0,SEEK_SET);
	//stat(filepath,&statbuf);
	//printf("\r\n the size is %d",statbuf.st_size);
	//while(readcount < statbuf.st_size)
	while(1)
	{
		count = read(fd,buf,sizeof(buf));
		if(count <= 0)
			break;
		buf[count] = '\0';
		printf("%s",buf);
		//readcount += count;
	}
	printf("\r\n");
	close(fd);
	
	return 0;
}

int main(int argc, char **argv) 
{
	FILE *proc_file;
	char cmd[80];
	int i;
	int pidfd;
	char *ptr1 = NULL;
	char *ptr2 = NULL;

	pidfd =open(ADSLCMD_LOCK_FILE,O_RDWR | O_CREAT);
	if(pidfd < 0)
	{
		printf("\r\nopen lock file error!");
		//strcpy(cmd, "argv[0]");
		strcpy(cmd, "adslphxcmd");
		for (i = 1; i < argc; i++) {
			strcat(cmd, " ");
			strcat(cmd, argv[i]);
		}

	    proc_file = fopen("/proc/tc3162/tcci_cmd", "w");
		if (!proc_file) {
			printf("open /proc/tc3162/tcci_cmd fail\n");
			un_lock(pidfd,0,SEEK_SET,0);
			return 0;
		}

		fprintf(proc_file, "%s", cmd);
		fclose(proc_file);
	
		if((ptr1 = strstr(cmd,"delt")) != NULL)
		{
			if((ptr2 = strstr(ptr1,"show")) != NULL ||(ptr2 = strstr(ptr1,"status")) != NULL )
			{
				fileToStdout(ADSLCMD_DIAGNOSTIC_FILE);
				unlink(ADSLCMD_DIAGNOSTIC_FILE);
			}
		}
		else
		{
			//fileToStdout(ADSLCMD_INFO_FILE);
			memset(cmd,0,sizeof(cmd));
			sprintf(cmd,"cat %s",ADSLCMD_INFO_FILE);
			system(cmd);
		}
	}
	else
	{
		writew_lock(pidfd,0,SEEK_SET,0);
		
		//strcpy(cmd, argv[0]);
		strcpy(cmd, "adslphxcmd");
		for (i = 1; i < argc; i++) {
			strcat(cmd, " ");
			strcat(cmd, argv[i]);
		}

	    proc_file = fopen("/proc/tc3162/tcci_cmd", "w");
		if (!proc_file) {
			printf("open /proc/tc3162/tcci_cmd fail\n");
			un_lock(pidfd,0,SEEK_SET,0);
			return 0;
		}

		fprintf(proc_file, "%s", cmd);
		fclose(proc_file);
		

		#if 0//for test
		printf("test begin");
		usleep(20000000);
		printf("test end");
		#endif
		
		if((ptr1 = strstr(cmd,"delt")) != NULL)
		{
			if((ptr2 = strstr(ptr1,"show")) != NULL ||(ptr2 = strstr(ptr1,"status")) != NULL )
			{
				fileToStdout(ADSLCMD_DIAGNOSTIC_FILE);
				unlink(ADSLCMD_DIAGNOSTIC_FILE);
			}
		}
		else
		{
			//fileToStdout(ADSLCMD_INFO_FILE);
			memset(cmd,0,sizeof(cmd));
			sprintf(cmd,"cat %s",ADSLCMD_INFO_FILE);
			system(cmd);
		}

		un_lock(pidfd,0,SEEK_SET,0);
		//close(pidfd);
	}
	return 0;
}
