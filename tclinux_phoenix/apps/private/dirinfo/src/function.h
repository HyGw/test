#ifndef __FUNCTION_H__
#define __FUNCTION_H__
#include <pthread.h>

#define ROOT_DIR 				 "/mnt"
#define CREATE_DIR_LIST  		 "\"CREATE_DIR_LIST\""
#define DEL_DIR_LIST             "\"DEL_DIR_LIST\""
#define GET_DISC_INF_LOCAL       "\"GET_DISC_INF_LOCAL\""
#define GET_FILE_NUM			 "\"GET_FILE_NUM\""
#define LIST                     "\"LIST\""
#define RENAME                   "\"RENAME\""
#define MOVE  				     "\"MOVE\""
#define REMOVE					 "\"REMOVE\""
#define COPY					 "\"COPY\""	
#define CREATE_FOLDER            "\"CREATE_FOLDER\""
#define RENAME_DISC				 "\"RENAME_DISC\""
#define FORMAT_DISC              "\"FORMAT_DISC\""
#define UPLOAD_FILE				 "\"UPLOAD_FILE\""
#define REQ_FORMAT               "\"REQ_FORMAT\""

typedef struct {
	int flags;
	pthread_mutex_t sema;
	pthread_cond_t con;
}list_ctl_t;

typedef struct {
	char *cmd;
	char *(*func)(char *cmd);
}cmd_t;

extern char *createMountList(char *cmd);
extern char *getMountList(char * cmd);
extern char *getFileNum(char *cmd);
extern char *getDirList(char *cmd);
extern char *copyFile(char *cmd);
extern char *moveFile(char *cmd);
extern char *delFile(char *cmd);
extern char *renameFile(char *cmd);
extern cmd_t cmd[];
#endif
