#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "event.h"
#include "list.h"

#define CLIENT_MAX_EVENTS 4

typedef struct client {
	int fd;  //socket fd
	event_epoll_t *ep;	
	list_head_t _head;
}client_t;

typedef struct client_list {
	int totalnum;
	list_head_t _head;
}client_list_t;

void client_init(void);
int Sendclientpipe(void);

#endif/**_CLIENT_H_**/

