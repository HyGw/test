#include "threadpool.h"
#include "dirfile.h"
#include <sys/types.h>
#include <pthread.h>
//#include <errno.h>
#include <time.h>
#include "function.h"

extern list_ctl_t glist_ctl;
threadpool_t gPool;
threadpool_t gPool_sock;


void * threadpoolsock(void *args) {
	threadpool_t *pool = (threadpool_t *)args;

	while(1) {
		pthread_mutex_lock(&pool->mutex);
		while(pool->count == 0 && pool->run == 1) {
			pthread_cond_wait(&pool->con, &pool->mutex);
		}
		if(!pool->run) {
			print(DIR_INF, "%s thread 0x%x exit.....\n", __FUNCTION__, pthread_self());
			pthread_cond_signal(&pool->con);
			pthread_mutex_unlock(&pool->mutex);
			pthread_exit(NULL);
		}
		work_data_t *workdata = list_first_entry(&pool->list, work_data_t, list);
		list_del(&workdata->list);
		pool->count--;
		pthread_mutex_unlock(&pool->mutex);
		print(DIR_ERR, "%s thread 0x%x .....\n", __FUNCTION__, pthread_self());
		workdata->func(workdata);
		free(workdata);	
	}
	pthread_exit(NULL);
}


void * threadpoolroutine(void *args) {
	threadpool_t *pool = (threadpool_t *)args;

	while(1) {
		pthread_mutex_lock(&pool->mutex);
		while(pool->count == 0 && pool->run == 1) {
			print(DIR_ERR, "%s thread 0x%x wait free = %d.....\n", __FUNCTION__, pthread_self(), pool->free);
			if(pool->flag==1 && pool->free == 0) {
				pool->run = 0;
				pthread_mutex_unlock(&glist_ctl.sema);
				print(DIR_ERR, "%s snd free = %d2.....\n", __FUNCTION__,  pool->free);
				pthread_cond_signal(&pool->con);
				print(DIR_ERR, "%s snd free = %d1.....\n", __FUNCTION__,  pool->free);
				pthread_mutex_unlock(&pool->mutex);
				print(DIR_ERR, "%s snd free = %d3.....\n", __FUNCTION__,  pool->free);
				pthread_exit(NULL);
			}
			pthread_cond_wait(&pool->con, &pool->mutex);
		}
		if(!pool->run) {
			print(DIR_INF, "%s thread 0x%x exit.....\n", __FUNCTION__, pthread_self());
			pthread_cond_signal(&pool->con);
			pthread_mutex_unlock(&pool->mutex);
			pthread_exit(NULL);
		}
		if(pool->flag == 0) {
			pool->flag = 1;
		}
		pool->free++;
		work_data_t *workdata = list_first_entry(&pool->list, work_data_t, list);
		list_del(&workdata->list);
		pool->count--;
		pthread_mutex_unlock(&pool->mutex);
		print(DIR_ERR, "%s thread 0x%x name %s.....\n", __FUNCTION__, pthread_self(), workdata->name);
		workdata->func(workdata);
		print(DIR_ERR, "%s thread 0x%x name %s addr = %p.....\n", __FUNCTION__, pthread_self(), workdata->name, workdata);
		free(workdata);	
		pthread_mutex_lock(&pool->mutex);
		pool->free--;
		pthread_mutex_unlock(&pool->mutex);
	}
	pthread_exit(NULL);
}

void threadpool_add(threadpool_t *pool, void(*func)(pwork_data_t), pwork_data_t work) {
	work_data_t *data = work;

	if(!data) {
		print(DIR_ERR, "%s====> malloc fail %d\n", __FUNCTION__, __LINE__);
		return;
	}
	data->func = func;
	print(DIR_ERR, "%s====> name  %s\n", __FUNCTION__, data->name);
	pthread_mutex_lock(&pool->mutex);
	list_add_tail(&data->list, &pool->list);
	pool->count++;
	pthread_mutex_unlock(&pool->mutex);
	pthread_cond_signal(&pool->con);
}

int  threadpool_init(threadpool_t *pool, int max, void*(*routine)(void *)) {
	int retCode = 0, i = 0;
	pthread_attr_t attr;
	pthread_t pid;

	retCode += pthread_mutex_init(&pool->mutex, NULL);
	retCode += pthread_cond_init(&pool->con, NULL);
	if(retCode) {
		pthread_cond_destroy(&pool->con);
		pthread_mutex_destroy(&pool->mutex);
		return -1;
	}
	pool->max = max;
	pool->count = 0;
	pool->run = 1;
	pool->flag = 0;
	pool->free = 0;
	list_init_head(&pool->list);
	for(i=0; i<max; i++) {
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		retCode = pthread_create(&pid, &attr, routine, pool);
		pthread_attr_destroy(&attr);
		if((retCode == 0) && (pthread_detach(pid) == EINVAL)) {
			continue;
		}
		print(DIR_ERR, "%s=====> create thread fail %d\n", __FUNCTION__, i);
	}
	return 0;
}
