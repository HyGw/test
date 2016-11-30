#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__
#include <pthread.h>
#include "list.h"
#include <errno.h>

#define MAX_THREAD_NUM    20
#define DIR_MAX_LEN 250


typedef struct {
	void(*func)(void * work);
	char name[DIR_MAX_LEN];
	int fd;
	list_head_t list;
}work_data_t, *pwork_data_t;

typedef struct {
	pthread_mutex_t mutex;
	pthread_cond_t  con;
	int count;
	int max;
	int run;
	int free;
	int flag;
	list_head_t list;
}threadpool_t;
extern threadpool_t gPool;
extern threadpool_t gPool_sock;

extern void * threadpoolroutine(void *args);
extern void * threadpoolsock(void *args);

#endif

