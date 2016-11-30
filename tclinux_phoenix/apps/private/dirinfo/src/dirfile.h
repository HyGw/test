#ifndef __DIRFILE_H__
#define __DIRFILE_H__
#include <stdarg.h>
#include "list.h"
#include <time.h>
#include <pthread.h>
#include "threadpool.h"


#define PIC   0
#define VED	  1
#define AUD   2
#define OTR   3
#define GBK         "GBK"
#define UTF8        "utf-8"
#define DBG_MSG_LEN 20000


typedef enum {
	DIR_ERR=0,
	DIR_INF,
	DIR_WRN
}dbglevel_t;

extern int ggLevel;
extern pthread_mutex_t waitsql;
extern void print(dbglevel_t level, char *fmt ,...) ;
extern void createlist(pwork_data_t work);
typedef struct{
	char *name;
	struct list_head list;
	struct list_head child;
}dirlist_t, *pdirlist_t;


#endif
