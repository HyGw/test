#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/un.h>
#include <stdarg.h>
#include "ecnt_lib_dbg.h"

#define ECNT_LIB_DBG_ENABLE_ALL  "/tmp/ecntlib_dbg_enable"
#define ECNT_LIB_DBG_LEVEL_FILE	"/tmp/ecntlib_dbg"
#define ECNT_LIB_PRINT_TYPE_FILE	"/tmp/ecntlib_print_type"
#define ECNT_PRINT_BUFF_SIZE  2048


char* ecnt_dbg_lib_file[ECNT_DBG_TYPE_MAX] = 
{
	"/tmp/pcie_lib_dbg_enable",
	"/tmp/flow_lib_dbg_enable",
	"/tmp/voip_lib_dbg_enable",
	"/tmp/gpon_lib_dbg_enable",
	"/tmp/brtable_lib_dbg_enable"
};


int get_itf_print_type(void)
{
	FILE *fp=NULL;	
	char tmp[8];
	
	fp = fopen(ECNT_LIB_PRINT_TYPE_FILE, "r");
	if(fp == NULL)
	{	
		return 0;
	}
	fgets(tmp, sizeof(tmp), fp);
	fclose(fp);
	
	return atoi(tmp);
}

void ecnt_itf_printf(char *fmt,...)
{
	if(get_itf_print_type() == 0){
		char msg[ECNT_PRINT_BUFF_SIZE];
		va_list args;
		va_start(args, fmt);
		vsnprintf(msg, ECNT_PRINT_BUFF_SIZE, fmt, args);
		va_end(args);	
		printf("%s",msg);
	}
	else{
		FILE *proc_file;
		char msg[ECNT_PRINT_BUFF_SIZE];
		va_list args;

		va_start(args, fmt);

		vsnprintf(msg, ECNT_PRINT_BUFF_SIZE, fmt, args);	

	    proc_file = fopen("/proc/tc3162/dbg_msg", "w");
		if (!proc_file) {
			printf("open /proc/tc3162/dbg_msg fail\n");
			va_end(args);
			return;
		}
		fprintf(proc_file, "%s", msg);
		fclose(proc_file);
		va_end(args);
	}
}


e_ecnt_itf_debug_level_t get_itf_dbg_level(void)
{
	FILE *fp=NULL;	
	char tmp[8];
	
	fp = fopen(ECNT_LIB_DBG_LEVEL_FILE, "r");
	if(fp == NULL)
	{	
		return ECNT_DBG_ERROR;
	}
	fgets(tmp, sizeof(tmp), fp);
	fclose(fp);
	return atoi(tmp);
}

int get_itf_dbg_switch(unsigned int level,unsigned int type)
{
	FILE *fp=NULL;	
	char* fname; 
	
	if ((get_itf_dbg_level() & level) == 0)
		return 0;

  if(access(ECNT_LIB_DBG_ENABLE_ALL, F_OK) == 0) {
  	return 1;	
  }

	if (type >= ECNT_DBG_TYPE_MAX)
		return 0;
	
	fname = ecnt_dbg_lib_file[type];
	
	if (fname == NULL)
		return 0;
	
	fp = fopen(fname, "r");
	if(fp == NULL)
		return 0;
	
	fclose(fp);	
	return 1;
}


