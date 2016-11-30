#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdarg.h>
#include <pthread.h>
#include <sys/types.h>
#include <assert.h>

#include "list.h"

#define THREAD_MAXNUM 5

#define THREADPOOL_DBG 0
#if THREADPOOL_DBG
#define thread_printf(x, args...) mobile_printf(x, ## args) 
#else
#define thread_printf(x, args...) 
#endif

#define EXEC_SYSTEM  0

typedef struct worker_data {
	void *ptr;
	int data;
} worker_data_t;

typedef void*  (start_routine)(worker_data_t *argv);

/*
*worker
*/
typedef struct worker
{
    start_routine *exec_func;
    worker_data_t argv;
    list_head_t _head;
} ThreadWorker_t;

typedef struct
{
    pthread_mutex_t mutex;
    pthread_cond_t condition;
    list_head_t list_worker;

    int shutdown;  
    int max_thread_num;
    int cur_queue_size;
}ThreadPool_t;

extern ThreadPool_t gThreadPool;

int ThreadPoolInit (ThreadPool_t *tp, int max_thread_num);
int ThreadPoolAddWork(ThreadPool_t *tp, start_routine *func, worker_data_t *dataptr);
#endif
