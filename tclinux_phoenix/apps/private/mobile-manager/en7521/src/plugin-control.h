#ifndef PLUGIN_CTRL_H_
#define PLUGIN_CTRL_H_
#include <pthread.h>

#include "event.h"
#include "urandom.h"
#include "jsonpkt.h"
#include "list.h"
#include "mobile_manager.h"
#include "parameter_table.h"


#define PLUPIN_INIT		0
#define PLUPIN_START		1
#define PLUPIN_RUN		2
#define PLUPIN_STOP		3

typedef struct
{
	pthread_mutex_t pktmutex;
	pthread_mutex_t startmutex;
	int states;
	int fd_pipein;	
}PluginCtrl_t;

#define PLUGIN_MAX_EVENTS 4

extern char gPluginServerAddr[40];
extern char gPluginServerPort[10];

int plugin_init();
int try_plugin_work(void);
int start_plugin_work(void);
void stop_plugin_work(json_t *pkt);
#endif /*PLUGIN_CTRL_H_*/

