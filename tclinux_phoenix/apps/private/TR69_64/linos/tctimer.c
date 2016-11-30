/******************************************************************************/
/*
 * Copyright (C) 1994-2008 TrendChip Technologies, Corp.
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
/*
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "tctimer.h"
*/
#include "Global_res.h"
#ifdef THREAD_REDUCE
#include <signal.h>
#endif

uint32 tctimer_free_mib = 0;
uint32 tctimer_malloc_mib = 0;

typedef enum{
TIMER_ID_NULL = -1,
TIMER_ID_COUNT = 16
}TIMER_ID_ENUM;

typedef struct _timer_list {
	struct timeval time; //time when expire
	TimerCallback cb; /*Timer callback function*/
	void *param;  /*callback function param*/
	UInt32 interval;	/*save interval for period timer*/
	int timerid;	/*timer ID*/
	int PeriodFlag;	/*Period Timer Flag*/
	char timername[16];	/*timer name*/
	struct _timer_list *next;	/*next point of timer list*/
}timer_list;

#define STATE_USED 1
#define STATE_FREE 0

typedef struct timer_context{
#ifndef THREAD_REDUCE
	pthread_t thread_id;		/*save timer thread ID*/
#endif
	pthread_mutex_t  *p_timer_mutex;	/*timer list mutex, when process timer list, will lock this mutes*/
	Bool abort;	/*timer exit flag*/
	Bool inited;	/*timer init flag*/
	timer_list *timerhead;	/*save timer list head*/
	unsigned char timeridtable[TIMER_ID_COUNT];	/*timer ID table for add timer*/
	int timerlistnum;	/*timer number of timer list*/
}Timer_Context;



void ReRegisterTimer(timer_list *timeout);
void ThreadedTimerCheck(void);
int findTimerID();



/*Global Timer context variable*/
static Timer_Context timer_context = {0};

void ReRegisterTimer(timer_list *timeout)
{
	timer_list *tmp, *prev;
	UInt32 interval_sec, interval_usec;
	int interval = timeout->interval;
	/*Caculte Delay Time*/
	gettimeofday(&timeout->time, NULL);	
	interval_sec = interval/1000;
	interval_usec = (interval % 1000) * 1000;

	/*ReFill time struct parameter*/
	timeout->time.tv_sec += interval_sec;
	timeout->time.tv_usec += interval_usec;

#if 0
	timeout->param = param;
	timeout->cb = callback;
	timeout->timerid = findTimerID();
#endif
	timeout->next = NULL;
	timeout->interval = interval;

	if (timer_context.timerhead == NULL) {
		timer_context.timerhead = timeout;
//		tc_printf("ReRegisterTimer: Success! Timer id is [%d] interval is [%d]\n", timeout->timerid, interval);
		return;
	}

	prev = NULL;
	tmp = timer_context.timerhead;
	while (tmp != NULL)
    {
		if (timercmp(&timeout->time, &tmp->time, <))
			break;
		prev = tmp;
		tmp = tmp->next;
	}

	if (prev == NULL)
    {
		timeout->next = timer_context.timerhead;
		timer_context.timerhead  = timeout;
	}
    else
    {
		timeout->next = prev->next;
		prev->next = timeout;
	}
//	tc_printf("ReRegisterTimer: Success! Timer id is [%d] interval is [%d]\n", timeout->timerid, interval);
}

#define TIMECHANGERANGE 60

void checkCPETimeChange(struct timeval now, struct timeval last)
{
	timer_list *timeout = NULL;
	int dval = now.tv_sec - last.tv_sec;
	if(abs(dval) >= TIMECHANGERANGE)
	{
		tc_printf("checkCPETimeChange: CPE Time Modified!\n");
		tc_printf("checkCPETimeChange: Now tv_sec is [%d] u_sec is [%d]\n", now.tv_sec, now.tv_usec);
		tc_printf("checkCPETimeChange: Last tv_sec is [%d] u_sec is [%d]\n", last.tv_sec, last.tv_usec);
		
		/*Update Timer List expire time*/
		timeout = timer_context.timerhead;
		while(timeout != NULL)
		{
			timeout->time.tv_sec += dval;
			timeout = timeout->next;
		}
		
	}

}

#ifndef THREAD_REDUCE
void ThreadedTimerCheck(void)
{
	timer_list *tmp;	
	struct timeval now;
	struct timeval last;
	gettimeofday(&last, NULL);
	gettimeofday(&now, NULL);
	do{
		/*Sleep 100ms for check next timer expire*/
		usleep(100000);
		pthread_mutex_lock(timer_context.p_timer_mutex);
		gettimeofday(&now, NULL);
		/* check if registered timeouts have occurred */
		if(timer_context.timerhead)
		{
			checkCPETimeChange(now, last);	
			if( timercmp(&now, &timer_context.timerhead->time, >=) )
            {
				tmp = timer_context.timerhead;
				timer_context.timerhead= timer_context.timerhead->next;
				tmp->cb(tmp->param);
				if(tmp->PeriodFlag == TRUE)
				{
					ReRegisterTimer(tmp);
				}
				else
				{
					if(tmp->timerid < 0 || tmp->timerid >= TIMER_ID_COUNT)
					{
						tc_printf("Fatal ERROR: timer id not in range!\n");
					}
					timer_context.timeridtable[tmp->timerid] = STATE_FREE;
					os_free(tmp);
					tctimer_free_mib++;
					timer_context.timerlistnum--;
				}
			}

		}
		/*Update last timeval*/
		last = now;
		pthread_mutex_unlock(timer_context.p_timer_mutex);
		if(timer_context.abort)
		{
			tc_printf("Timer Thread Exit");
			break;
		}
	}while(1);
	pthread_mutex_destroy(timer_context.p_timer_mutex);
	os_free(timer_context.p_timer_mutex);
	tctimer_free_mib++;
	timer_context.p_timer_mutex = NULL;
	if(timer_context.timerhead)
	{
		while(timer_context.timerhead)
		{
			tmp = timer_context.timerhead;
			timer_context.timerhead = tmp->next;
			os_free(tmp);
			tctimer_free_mib++;
		}
	}
	pthread_exit(0);
	tc_printf( "ThreadedTimerCheck: return .\n");
}
#else
void ThreadedTimerCheck(void)
{
	timer_list *tmp;	
	struct timeval now;
	static struct timeval last = {0};
	if(last.tv_sec == 0 && last.tv_usec == 0)
	{
		tcdbg_printf("ThreadedTimerCheck: get last for first time\n");
		gettimeofday(&last, NULL);
	}

	pthread_mutex_lock(timer_context.p_timer_mutex);
	gettimeofday(&now, NULL);
	/* check if registered timeouts have occurred */
	if(timer_context.timerhead)
	{
		checkCPETimeChange(now, last);		
		//tcdbg_printf("ThreadedTimerCheck Entered! now time val sec is [%d] usec is [%d]\n",  now.tv_sec,  now.tv_usec);
		//tcdbg_printf("ThreadedTimerCheck Entered! timerhead time val sec is [%d] usec is [%d]\n", timer_context.timerhead->time.tv_sec, timer_context.timerhead->time.tv_usec);
		if(timercmp(&now, &timer_context.timerhead->time, >=))
        {
			tmp = timer_context.timerhead;
			timer_context.timerhead = timer_context.timerhead->next;
			tmp->cb(tmp->param);
			if(tmp->PeriodFlag == TRUE)
			{
				ReRegisterTimer(tmp);
			}
			else
			{
				if(tmp->timerid < 0 || tmp->timerid >= TIMER_ID_COUNT)
				{
					tc_printf("Fatal ERROR: timer id not in range!\n");
				}
				timer_context.timeridtable[tmp->timerid] = STATE_FREE;
				os_free(tmp);
				tctimer_free_mib++;
				timer_context.timerlistnum--;
			}
		}
		else
		{
			//tcdbg_printf("ThreadedTimerCheck: No Timer Expire, we wait for next\n");		
		}

	}
	else
	{
		tcdbg_printf("ThreadedTimerCheck: No Timer in list\n");
	}
	/*Update last timeval*/
	last = now;
	pthread_mutex_unlock(timer_context.p_timer_mutex);
}
#endif

int timer_init(void)
{
	int ret = 0;
#ifndef THREAD_REDUCE
	pthread_attr_t p_megq_attr;
#endif
	if(timer_context.inited == TRUE)
	{
		tc_printf("Timer has been inited!\n");
		return ERROR;
	}
	memset(&timer_context, 0, sizeof(timer_context));
	timer_context.p_timer_mutex = os_malloc(sizeof(pthread_mutex_t));
	if(!timer_context.p_timer_mutex)  
	{
		tc_printf("[%s]---malloc faile for  p_timer_mutex  \n", __FUNCTION__);
		return ERROR;
	}
	tctimer_malloc_mib++;
#ifndef THREAD_REDUCE
	ret = pthread_mutex_init(timer_context.p_timer_mutex, NULL);
	if(ret != 0)
	{
		tc_printf("timer_init: mutex_init failed!\n");
		os_free(timer_context.p_timer_mutex);
		return ERROR;
	}

	/*create thread*/
	ret = pthread_attr_init(&p_megq_attr);
	if(ret != 0)
	{
		tc_printf("\r\nThread attribute creation fail!");
		pthread_mutex_destroy(timer_context.p_timer_mutex);
		os_free(timer_context.p_timer_mutex);
		return ERROR;
	}

	ret = pthread_attr_setdetachstate(&p_megq_attr,PTHREAD_CREATE_DETACHED);
	if(ret != 0)
	{
		tc_printf("\r\nSet attribute fail!");
		pthread_mutex_destroy(timer_context.p_timer_mutex);		
		os_free(timer_context.p_timer_mutex);
		return ERROR;
	}
		
	if( pthread_create(&timer_context.thread_id, &p_megq_attr, (void *)ThreadedTimerCheck, (void *)NULL))
	{
		tc_printf("[timer_init]-->pthread_create  ThreadedTimerCheck failed!\n");
		pthread_mutex_destroy(timer_context.p_timer_mutex);
		os_free(timer_context.p_timer_mutex);
		return ERROR;
	}

#endif
	timer_context.inited = TRUE;
	return OK;
}

void timer_exit(void)
{
	timer_context.abort = TRUE;
	timer_context.inited = FALSE;
#ifdef THREAD_REDUCE
	timer_list *tmp;
	pthread_mutex_lock(timer_context.p_timer_mutex);
	if(timer_context.timerhead)
	{
		while(timer_context.timerhead)
		{
			tmp = timer_context.timerhead;
			timer_context.timerhead = tmp->next;
			os_free(tmp);
			tctimer_free_mib++;
		}
	}
	pthread_mutex_unlock(timer_context.p_timer_mutex);
	pthread_mutex_destroy(timer_context.p_timer_mutex);
	os_free(timer_context.p_timer_mutex);
	tctimer_free_mib++;
	timer_context.p_timer_mutex = NULL;
#endif
	return;
}

int findTimerID()
{
	int i = 0;
	for(i = 0; i < TIMER_ID_COUNT; i++)
	{
		if(timer_context.timeridtable[i] == STATE_FREE)
		{
			timer_context.timeridtable[i] = STATE_USED;
			return i;
		}
	}
	tc_printf("findTimerID: Failed to find Time ID for New Timer\n");
	return TIMER_ID_NULL;
}

/* ==========================================================
*     Designer: yanfei
*     Time : 2009-11-10
*     Function Name: AddTimer
*     Function Description:
*				Add Timer in Timer List
*     Paramter:
*				Int32 timer_id: TIMER_ID which want to add
*               UInt32 interval: Timer expire interval, unit us
*               TimerCallback callback: callback function of timer
*				void *param: callback function paramter
*				Bool PeriodFlag: timer register is Period 
*                         
*     Return :
*				Success: timer ID for new Timer
*				Failed:  -1
*   ========================================================== */
int AddTimer(char *timer_name, UInt32 interval, TimerCallback callback, void *param, Bool PeriodFlag)
{
	timer_list *timeout, *tmp, *prev;
	UInt32 interval_sec, interval_usec;
	if(timer_context.inited == FALSE)
	{
		tc_printf("AddTimer: Timer don not inited\n");
		return TIMER_ID_NULL;
	}
	if((timer_context.timerlistnum >= TIMER_ID_COUNT) \
		|| (interval <= 0) || (callback==NULL))
	{
		tc_printf("AddTimer: failed!\n");
		return TIMER_ID_NULL;
	}

	pthread_mutex_lock(timer_context.p_timer_mutex);
	timeout = (timer_list *) os_malloc(sizeof(*timeout));
	tctimer_malloc_mib++;
	if (timeout == NULL)
	{
		tc_printf("AddTimer: No resource for timer add\n");
		pthread_mutex_unlock(timer_context.p_timer_mutex);
		return TIMER_ID_NULL;
	}
	memset(timeout, 0, sizeof(*timeout));
	/*Caculte Delay Time*/
	gettimeofday(&timeout->time, NULL);	
	interval_sec = interval/1000;
	interval_usec = (interval % 1000) * 1000;

	/*Fill time struct parameter*/
	timeout->time.tv_sec += interval_sec;
	timeout->time.tv_usec += interval_usec;

	timeout->param = param;
	timeout->cb = callback;
	/*Make sure timerid  > 0*/
	timeout->timerid = findTimerID() + 1;
	timeout->next = NULL;
	timeout->interval = interval;
	if(PeriodFlag == TRUE)
	{
		timeout->PeriodFlag = TRUE;
	}
	if(timer_name != NULL)
	{
		strncpy(timeout->timername, timer_name, 15);
	}

	/*Increase timer_context number*/
	timer_context.timerlistnum++;

	if (timer_context.timerhead == NULL) {
		timer_context.timerhead = timeout;
		pthread_mutex_unlock(timer_context.p_timer_mutex);
		//tc_printf("AddTimer: Success! Timer id is [%d]\n", timeout->timerid);
		return timeout->timerid;
	}

	prev = NULL;
	tmp = timer_context.timerhead;
	while (tmp != NULL)
    {
		if (timercmp(&timeout->time, &tmp->time, <))
			break;
		prev = tmp;
		tmp = tmp->next;
	}

	if (prev == NULL)
    {
		timeout->next = timer_context.timerhead;
		timer_context.timerhead  = timeout;
	}
    else
    {
		timeout->next = prev->next;
		prev->next = timeout;
	}

	pthread_mutex_unlock(timer_context.p_timer_mutex);
	//tc_printf("AddTimer: Success! Timer id is [%d]\n", timeout->timerid);
	return timeout->timerid;
}

int AddDelayTimer(char *timer_name, UInt32 delay, UInt32 interval, TimerCallback callback, void *param, Bool PeriodFlag)
{
	timer_list *timeout, *tmp, *prev;
	UInt32 interval_sec, interval_usec;
	UInt32 delay_sec = 0, delay_usec = 0;
	if(timer_context.inited == FALSE)
	{
		tc_printf("AddTimer: Timer don not inited\n");
		return TIMER_ID_NULL;
	}
	if((timer_context.timerlistnum >= TIMER_ID_COUNT) \
		|| (interval <= 0) || (callback==NULL))
	{
		tc_printf("AddTimer: failed!\n");
		return TIMER_ID_NULL;
	}

	pthread_mutex_lock(timer_context.p_timer_mutex);
	timeout = (timer_list *) os_malloc(sizeof(*timeout));
	tctimer_malloc_mib++;
	if (timeout == NULL)
	{
		tc_printf("AddTimer: No resource for timer add\n");
		pthread_mutex_unlock(timer_context.p_timer_mutex);
		return TIMER_ID_NULL;
	}
	memset(timeout, 0, sizeof(*timeout));
	/*Caculte Delay Time*/
	
	gettimeofday(&timeout->time, NULL);	
	tc_printf("current tv_sec=%ld,tv_usec=%ld\n",timeout->time.tv_sec,timeout->time.tv_usec);
	
	interval_sec = interval/1000;
	interval_usec = (interval % 1000) * 1000;

	delay_sec = delay/1000;
	delay_usec = (delay % 1000) * 1000;
    
	/*Fill time struct parameter*/
	timeout->time.tv_sec += delay_sec /*+ interval_sec*/;
	timeout->time.tv_usec += delay_usec /*+ interval_usec*/;
	tc_printf("timer tv_sec=%ld,tv_usec=%ld\n",timeout->time.tv_sec,timeout->time.tv_usec);

	timeout->param = param;
	timeout->cb = callback;
	/*Make sure timerid  > 0*/
	timeout->timerid = findTimerID() + 1;
	timeout->next = NULL;
	timeout->interval = interval;
	if(PeriodFlag == TRUE)
	{
		timeout->PeriodFlag = TRUE;
        tc_printf("periodic timer\n");
	}
	if(timer_name != NULL)
	{
		strncpy(timeout->timername, timer_name, 15);
	}

	/*Increase timer_context number*/
	timer_context.timerlistnum++;

	if (timer_context.timerhead == NULL) {
		timer_context.timerhead = timeout;
		pthread_mutex_unlock(timer_context.p_timer_mutex);
		//tc_printf("AddTimer: Success! Timer id is [%d]\n", timeout->timerid);
		return timeout->timerid;
	}

	prev = NULL;
	tmp = timer_context.timerhead;
	while (tmp != NULL)
    {
		if (timercmp(&timeout->time, &tmp->time, <))
			break;
		prev = tmp;
		tmp = tmp->next;
	}

	if (prev == NULL)
    {
		timeout->next = timer_context.timerhead;
		timer_context.timerhead  = timeout;
	}
    else
    {
		timeout->next = prev->next;
		prev->next = timeout;
	}

	pthread_mutex_unlock(timer_context.p_timer_mutex);
	//tc_printf("AddTimer: Success! Timer id is [%d]\n", timeout->timerid);
	return timeout->timerid;
}

/* ==========================================================
*     Designer: yanfei
*     Time: 2009-11-10
*     Function Name: RemoveTimer
*     Function Description:
*				Delete Timer which in Timer List
*     Paramter:
*				Int32 timer_id: TIMER_ID which want to delete
*                         
*     Return :
*				Success: removed timer id
*				Failed:  -1
*   ========================================================== */

int RemoveTimer(int timer_id)
{
	timer_list *timeout, *prev, *next;	
	int delnum = 0;
	if(timer_context.inited == FALSE)
	{
		tc_printf("AddTimer: Timer don not inited\n");
		return TIMER_ID_NULL;
	}
	
	/*To Solve Timer ID is 0 will cause error in previous linos timer ID
	we also increase timer id in AddTimer function
	*/
	if(timer_id -1 <=TIMER_ID_NULL)
	{
		tc_printf("RemoveTimer: timer_id:%d invalid!\n",timer_id);
		return TIMER_ID_NULL;
	}
	
	pthread_mutex_lock(timer_context.p_timer_mutex);
	if (timer_context.timerhead == NULL) {
		pthread_mutex_unlock(timer_context.p_timer_mutex);
		tc_printf("RemoveTimer: Failed! No Timer in list. timer_id:%d invalid!\n", timer_id);
		return TIMER_ID_NULL;
	}

	prev = NULL;
	next = NULL;
	timeout = timer_context.timerhead;
	while (timeout != NULL)
    {
		next = timeout->next;
		if (timeout->timerid == timer_id)
        {
			if (prev == NULL)
				timer_context.timerhead = next;
			else
				prev->next = next;

			/*Kill Timer and free source*/
/*To Solve Timer ID is 0 will cause error in previous linos timer ID
	we also increase timer id in AddTimer function
*/
			timer_context.timeridtable[timeout->timerid - 1] = STATE_FREE;
			os_free(timeout);
			tctimer_free_mib++;
			delnum++;
			timer_context.timerlistnum--;
		} else
			prev = timeout;

		timeout = next;
	}

	pthread_mutex_unlock(timer_context.p_timer_mutex);
	if(delnum > 1)
	{
		tc_printf("Warn: Remove Timer number larger than 1, remove num is [%d]\n", delnum);
	}
	if(delnum == 0)
	{
		tc_printf("RemoveTimer: Failed! No Timer in list. timer_id:%d invalid!\n", timer_id);
		return TIMER_ID_NULL;
	}
	//tc_printf("RemoveTimer: Success! \n");	
	return timer_id;
}

int RemoveTimer_tmp(int timer_id)
{
	return timer_id;
}

#if defined(TCSUPPORT_RANDOM_INFORM)
int AddTimer_tmp(char *timer_name, UInt32 interval, TimerCallback callback, void *param, Bool PeriodFlag)
{
	timer_list *timeout, *tmp, *prev;
	UInt32 interval_sec, interval_usec;

	if(timer_context.inited == FALSE)
	{
		tc_printf("AddTimer: Timer don not inited\n");
		return TIMER_ID_NULL;
	}
	if((timer_context.timerlistnum >= TIMER_ID_COUNT) \
		|| (interval <= 0) || (callback==NULL))
	{
		tc_printf("AddTimer: failed!\n");
		return TIMER_ID_NULL;
	}

//	pthread_mutex_lock(timer_context.p_timer_mutex);
	timeout = (timer_list *) os_malloc(sizeof(*timeout));
	tctimer_malloc_mib++;
	if (timeout == NULL)
	{
		tc_printf("AddTimer: No resource for timer add\n");
//		pthread_mutex_unlock(timer_context.p_timer_mutex);
		return TIMER_ID_NULL;
	}
	memset(timeout, 0, sizeof(*timeout));
	/*Caculte Delay Time*/
	gettimeofday(&timeout->time, NULL);	
	interval_sec = interval/1000;
	interval_usec = (interval % 1000) * 1000;

	/*Fill time struct parameter*/
	timeout->time.tv_sec += interval_sec;
	timeout->time.tv_usec += interval_usec;

	timeout->param = param;
	timeout->cb = callback;
	/*Make sure timerid  > 0*/
	timeout->timerid = findTimerID() + 1;
	timeout->next = NULL;
	timeout->interval = interval;
	if(PeriodFlag == TRUE)
	{
		timeout->PeriodFlag = TRUE;
	}
	if(timer_name != NULL)
	{
		strncpy(timeout->timername, timer_name, 15);
	}

	/*Increase timer_context number*/
	timer_context.timerlistnum++;

	if (timer_context.timerhead == NULL) {
		timer_context.timerhead = timeout;
//		pthread_mutex_unlock(timer_context.p_timer_mutex);
		//tc_printf("AddTimer: Success! Timer id is [%d]\n", timeout->timerid);
		return timeout->timerid;
	}

	prev = NULL;
	tmp = timer_context.timerhead;
	while (tmp != NULL)
    {
		if (timercmp(&timeout->time, &tmp->time, <))
			break;
		prev = tmp;
		tmp = tmp->next;
	}

	if (prev == NULL)
    {
		timeout->next = timer_context.timerhead;
		timer_context.timerhead  = timeout;
	}
    else
    {
		timeout->next = prev->next;
		prev->next = timeout;
	}

//	pthread_mutex_unlock(timer_context.p_timer_mutex);
	//tc_printf("AddTimer: Success! Timer id is [%d]\n", timeout->timerid);
	return timeout->timerid;
}
#endif
#if 0
/* ==========================================================
*     Designer: yanfei
*     Time: 2009-11-10
*     Function Name: RemoveTimerByName
*     Function Description:
*				Delete Timer which in Timer List
*     Paramter:
*				char *TimerName: TIMER Name which want to delete
*                         
*     Return :
*				Success: removed timer id
*				No Timer Find Failed:  -1
*				
*   ========================================================== */

int RemoveTimerByName(char *TimerName)
{
	int timer_id = -1;
	timer_list *timeout, *prev, *next;
	int delnum = 0;
	prev = NULL;
	next = NULL;
	timeout = NULL;

	if(timer_context.inited == FALSE)
	{
		tc_printf("AddTimer: Timer don not inited\n");
		return TIMER_ID_NULL;
	}
	
	if(TimerName == NULL)
	{
		tc_printf("RemoveTimer: timer_name invalid!\n");
		return TIMER_ID_NULL;
	}
	
	pthread_mutex_lock(timer_context.p_timer_mutex);
	if (timer_context.timerhead == NULL) {
		pthread_mutex_unlock(timer_context.p_timer_mutex);
		tc_printf("RemoveTimer: Failed! No Timer in list. timer_id:%d invalid!\n", timeout->timerid);
		return TIMER_ID_NULL;
	}

	timeout = timer_context.timerhead;
	while (timeout != NULL)
    {
		next = timeout->next;
		if (strcmp(timeout->timername, TimerName) == 0)
        {
			if (prev == NULL)
				timer_context.timerhead = next;
			else
				prev->next = next;

			timer_id = timeout->timerid;
			/*Kill Timer and free source*/
			free(timeout);
			delnum++;
			timer_context.timerlistnum++;
		} else
			prev = timeout;

		timeout = next;
	}

	pthread_mutex_unlock(timer_context.p_timer_mutex);
	if(delnum > 1)
	{
		tc_printf("Warn: Remove Timer number larger than 1, remove num is [%d]\n", delnum);
	}
	if(delnum == 0)
	{
		tc_printf("RemoveTimer: Failed! No Timer in list.\n");
		return TIMER_ID_NULL;
	}
	tc_printf("RemoveTimer: Success! \n");	
	return timer_id;
}


int TestTimerFunc(void *param)
{
	tc_printf("TestTimerFunc: param is [%s]\n", param);
	return 0;
}

char param[20][64];
int
main (int argc, char* argv[]){

	struct timeval time;
	int i = 0;
	int periodID1 = 0, periodID2 = 0;
	gettimeofday(&time, NULL);
	tc_printf("time.tv_sec is [%d] time.tv_usec is [%d]\n", time.tv_sec, time.tv_usec);
	timer_init();
	periodID1 = timerPeriodicStartS(3500, TestTimerFunc, "Period Timer 1111 Enter");

	for(i = 0; i < 13; i++)
	{
		sprintf(param[i], "Timer [%d]", i+1);
		timerStartS(1000*(i*i), TestTimerFunc, param[i]);
	}
	periodID2 = timerPeriodicStartS(7500, TestTimerFunc, "Period Timer 2222 Enter");
	i = 0;
	while(timer_context.timerlistnum > 0) 
	{
		sleep(1);
		i++;
		if(i == 120)
		{
			RemoveTimer(periodID1);
		}
		if(i == 320)
		{
			RemoveTimer(periodID2);
		}
		tc_printf("1Second Passed, Now Second is [%d]\n", i);
	}

	for(i = 0; i < 20; i++)
	{
		sprintf(param[i], "Timer [%d]", i+1);
		if(i == 10)
		{
			timerPeriodicStartS(3500, TestTimerFunc, "Period Timer 444 Enter");
			
		}
		timerStartS(1000*(i*i), TestTimerFunc, param[i]);
	}
	i = 0;
	while(timer_context.timerlistnum > 0) 
	{
		sleep(1);
		i++;
		tc_printf("1Second Passed, Now Second is [%d]\n", i);
	}
	
	timer_exit();
 	return 0;
}
#endif

