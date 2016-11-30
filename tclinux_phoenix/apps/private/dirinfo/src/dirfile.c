#include <stdio.h>
#include "dirfile.h"
#include <stdarg.h>
#include "threadpool.h"
#include <sys/time.h>
#include <pthread.h>
#include <signal.h>

int ggLevel = DIR_ERR;
//static char buf[DBG_MSG_LEN] = {0};

void print(dbglevel_t level, char *fmt ,...) {
	char *buf = NULL;
	
	if(level > ggLevel) {
		return;
	}
	
	buf = malloc(DBG_MSG_LEN);

	if(buf == NULL){
		printf("===>%s func %s line %d malloc!\n", __FILE__, __FUNCTION__, __LINE__);
		return;
	}
	va_list vargs;
	
	va_start(vargs, fmt);
	vsprintf(buf, fmt, vargs);
	va_end(vargs);
	printf(buf);
	free(buf);
	
}

#if 0

int main() {
	
	threadpool_init(&gPool, MAX_THREAD_NUM);
	pthread_mutex_lock(&waitsql);
	threadpool_add(&gPool, createlist, "/proj/mtk11339/test");
	//print(DIR_ERR, "%s====> lock %d\n", __FUNCTION__, __LINE__);
	pthread_mutex_lock(&waitsql);
	//print(DIR_ERR, "%s====> getlock %d\n", __FUNCTION__, __LINE__);
	pthread_mutex_unlock(&waitsql);
	//print(DIR_ERR, "%s====> getlock1 %d\n", __FUNCTION__, __LINE__);
	printf("sql dir finsh\n");
	
	printf("enter 2\n");
	printTotalList(PIC);
	printf("enter 3\n");
	printTotalList(VED);
	printf("enter 4\n");
	printTotalList(AUD);
    printf("enter 5\n");
	threadpool_add(&gPool, createlist, "/proj/mtk11339/test");
	sleep(10);
	printf("sql dir finsh2\n");
	delDir("/proj/mtk11339/test/aa/ss");
	printTotalList(PIC);
	delFile("/proj/mtk11339/test/aa", "b.png");
	printf("enter 6\n");
	printTotalList(PIC);
	delFile("/proj/mtk11339/test/aa", "q.jpg");
	printf("enter 7\n");
	printTotalList(PIC);
	//moveDir("/proj/mtk11339/test", "/proj/mtk11339/test/aa/ss");
	//printTotalList(PIC);
	//copyDir("/proj/mtk11339/test", "/proj/mtk11339/test/aa/ss");
	//printTotalList(PIC);
	
}
#endif
